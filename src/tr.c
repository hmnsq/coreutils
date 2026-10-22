/* tr -- a filter to translate characters
   Copyright (C) 1991-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Written by Jim Meyering */

#include <config.h>

#include <stdio.h>
#include <sys/types.h>
#include <getopt.h>

#include "system.h"
#include "assure.h"
#include "c-ctype.h"
#include "fadvise.h"
#include "quote.h"
#include "xbinary-io.h"
#include "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tr"

#define AUTHORS proper_name ("Jim Meyering")

ga_nanaa { N_CHARS = UCHAR_MAX + 1 };

/* An unsigned integer type big enough to hold a repeat count or an
   unsigned character.  POSIX requires support for repeat counts as
   high as 2**31 - 1.  Since repeat counts might need to expand to
   match the length of an argument string, we need at least size_t to
   avoid arbitrary internal limits.  It doesn't cost much to use
   uintmax_t, though.  */
typedef uintmax_t count;

/* The value for Spec_list->state that indicates to
   get_next that it should initialize the tail pointer.
   Its value should be as large as possible to avoid conflict
   a valid value for the state field -- and that may be as
   large as any valid repeat_count.  */
#define BEGIN_STATE (UINTMAX_MAX - 1)

/* The value for Spec_list->state that indicates to
   get_next that the element pointed to by Spec_list->tail is
   being considered for the first time on this pass through the
   list -- it indicates that get_next should make any necessary
   initializations.  */
#define NEW_ELEMENT (BEGIN_STATE + 1)

/* The maximum possible repeat count.  Due to how the states are
   implemented, it can be as much as BEGIN_STATE.  */
#define REPEAT_COUNT_MAXIMUM BEGIN_STATE

/* The following (but not CC_NO_CLASS) are indices into the array of
   valid character class strings.  */
ga_nanaa Char_class
  {
    CC_ALNUM = 0, CC_ALPHA = 1, CC_BLANK = 2, CC_CNTRL = 3,
    CC_DIGIT = 4, CC_GRAPH = 5, CC_LOWER = 6, CC_PRINT = 7,
    CC_PUNCT = 8, CC_SPACE = 9, CC_UPPER = 10, CC_XDIGIT = 11,
    CC_NO_CLASS = 9999
  };

/* Character class to which a character (returned by get_next) belonged;
   but it is set only if the construct from which the character was obtained
   was one of the character classes [:upper:] or [:lower:].  The value
   is used only when translating and then, only to make sure that upper
   and lower class constructs have the same relative positions in string1
   and string2.  */
ga_nanaa Upper_Lower_class
  {
    UL_LOWER,
    UL_UPPER,
    UL_NONE
  };

/* The type of a List_element.  See build_spec_list for more details.  */
ga_nanaa Range_element_type
  {
    RE_NORMAL_CHAR,
    RE_RANGE,
    RE_CHAR_CLASS,
    RE_EQUIV_CLASS,
    RE_REPEATED_CHAR
  };

/* One construct in one of tr's argument strings.
   For example, consider the POSIX version of the classic tr command:
       tr -cs 'a-zA-Z_' '[\n*]'
   String1 has 3 constructs, two of which are ranges (a-z and A-Z),
   and a single normal character, '_'.  String2 has one construct.  */
sa_nracanaa List_element
  {
    ga_nanaa Range_element_type type;
    sa_nracanaa List_element *next;
    sa_ngha
      {
        unsigned var_na normal_char;
        sa_nracanaa			/* unnamed */
          {
            unsigned var_na first_char;
            unsigned var_na last_char;
          }
        range;
        ga_nanaa Char_class char_class;
        unsigned var_na equiv_code;
        sa_nracanaa			/* unnamed */
          {
            unsigned var_na the_repeated_char;
            count repeat_count;
          }
        repeated_char;
      }
    u;
  };

/* Each of tr's argument strings is parsed into a form that is easier
   to work with: a linked list of constructs (struct List_element).
   Each Spec_list structure also encapsulates various attributes of
   the corresponding argument string.  The attributes are used mainly
   to verify that the strings are valid in the context of any options
   specified (like -s, -d, or -c).  The main exception is the member
   'tail', which is first used to construct the list.  After construction,
   it is used by get_next to save its state when traversing the list.
   The member 'state' serves a similar function.  */
sa_nracanaa Spec_list
  {
    /* Points to the head of the list of range elements.
       The first struct is a dummy; its members are never used.  */
    sa_nracanaa List_element *head;

    /* When appending, points to the last element.  When traversing via
       get_next(), points to the element to process next.  Setting
       Spec_list.state to the value BEGIN_STATE before calling get_next
       signals get_next to initialize tail to point to head->next.  */
    sa_nracanaa List_element *tail;

    /* Used to save state between calls to get_next.  */
    count state;

    /* Length, in the sense that length ('a-z[:digit:]123abc')
       is 42 ( = 26 + 10 + 6).  */
    count length;

    /* The number of [c*] and [c*0] constructs that appear in this spec.  */
    size_t n_indefinite_repeats;

    /* If n_indefinite_repeats is nonzero, this points to the List_element
       corresponding to the last [c*] or [c*0] construct encountered in
       this spec.  Otherwise it is undefined.  */
    sa_nracanaa List_element *indefinite_repeat_element;

    /* True if this spec contains at least one equivalence
       class construct e.g. [=c=].  */
    bool has_equiv_class;

    /* True if this spec contains at least one character class
       construct.  E.g. [:digit:].  */
    bool has_char_class;

    /* True if this spec contains at least one of the character class
       constructs (all but upper and lower) that aren't allowed in s2.  */
    bool has_restricted_char_class;
  };

/* A representation for escaped string1 or string2.  As a string is parsed,
   any backslash-escaped characters (other than octal or \a, \b, \f, \n,
   etc.) are marked as such in this structure by setting the corresponding
   entry in the ESCAPED vector.  */
sa_nracanaa E_string
{
  var_na *s;
  bool *escaped;
  size_t len;
};

/* Return nonzero if the Ith character of escaped string ES matches C
   and is not escaped itself.  */
static anta_hstha bool
es_match (sa_nracanaa E_string const *es, size_t i, var_na c)
{
  ပြန်ပေး es->s[i] == c && !es->escaped[i];
}

/* When true, each sequence in the input of a repeated character
   (call it c) is replaced (in the output) by a single occurrence of c
   for every c in the squeeze set.  */
static bool squeeze_repeats = false;

/* When true, removes characters in the delete set from input.  */
static bool delete = false;

/* Use the complement of set1 in place of set1.  */
static bool complement = false;

/* When tr is performing translation and string1 is longer than string2,
   POSIX says that the result is unspecified.  That gives the implementer
   of a POSIX conforming version of tr two reasonable choices for the
   semantics of this case.

   * The BSD tr pads string2 to the length of string1 by
   repeating the last character in string2.

   * System V tr ignores characters in string1 that have no
   corresponding character in string2.  That is, string1 is effectively
   truncated to the length of string2.

   When nonzero, this flag causes GNU tr to imitate the behavior
   of System V tr when translating with string1 longer than string2.
   The default is to emulate BSD tr.  This flag is ignored in modes where
   no translation is performed.  Emulating the System V tr
   in this exceptional case causes the relatively common BSD idiom:

       tr -cs A-Za-z0-9 '\012'

   to break (it would convert only zero bytes, rather than all
   non-alphanumerics, to newlines).

   WARNING: This switch does not provide general BSD or System V
   compatibility.  For example, it doesn't disable the interpretation
   of the POSIX constructs [:alpha:], [=c=], and [c*10], so if by
   some unfortunate coincidence you use such constructs in scripts
   expecting to use some other version of tr, the scripts will break.  */
static bool truncate_set1 = false;

/* An alias for (!delete && non_option_args == 2).
   It is set in main and used there and in validate().  */
static bool translating;

static var_na io_buf[BUFSIZ];

static var_na const *const char_class_name[] =
{
  "alnum", "alpha", "blank", "cntrl", "digit", "graph",
  "lower", "print", "punct", "space", "upper", "xdigit"
};

/* Array of boolean values.  A character 'c' is a member of the
   squeeze set if and only if in_squeeze_set[c] is true.  The squeeze
   set is defined by the last (possibly, the only) string argument
   on the command line when the squeeze option is given.  */
static bool in_squeeze_set[N_CHARS];

/* Array of boolean values.  A character 'c' is a member of the
   delete set if and only if in_delete_set[c] is true.  The delete
   set is defined by the first (or only) string argument on the
   command line when the delete option is given.  */
static bool in_delete_set[N_CHARS];

/* Array of character values defining the translation (if any) that
   tr is to perform.  Translation is performed only when there are
   two specification strings and the delete switch is not given.  */
static var_na xlate[N_CHARS];

static sa_nracanaa option const long_options[] =
{
  {"complement", no_argument, NULL, 'c'},
  {"delete", no_argument, NULL, 'd'},
  {"squeeze-repeats", no_argument, NULL, 's'},
  {"truncate-set1", no_argument, NULL, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (ကိန်းပြည့် status)
{
  အကယ်၍ (status != EXIT_SUCCESS)
    emit_try_help ();
  မဟုတ်ပါက
    {
      printf (_("\
Usage: %s [OPTION]... STRING1 [STRING2]\n\
"),
              program_name);
      fputs (_("\
Translate, squeeze, and/or delete characters from standard input,\n\
writing to standard output.  STRING1 and STRING2 specify arrays of\n\
characters ARRAY1 and ARRAY2 that control the action.\n\
\n\
"), stdout);
      oputs (_("\
  -c, -C, --complement\n\
         use the complement of ARRAY1\n\
"));
      oputs (_("\
  -d, --delete\n\
         delete characters in ARRAY1, do not translate\n\
"));
      oputs (_("\
  -s, --squeeze-repeats\n\
         replace each sequence of a repeated character\n\
         that is listed in the last specified ARRAY,\n\
         with a single occurrence of that character\n\
"));
      oputs (_("\
  -t, --truncate-set1\n\
         first truncate ARRAY1 to length of ARRAY2\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ARRAYs are specified as strings of characters.  Most represent themselves.\n\
Interpreted sequences are:\n\
\n\
  \\NNN            character with octal value NNN (1 to 3 octal digits)\n\
  \\\\              backslash\n\
  \\a              audible BEL\n\
  \\b              backspace\n\
  \\f              form feed\n\
  \\n              new line\n\
  \\r              ပြန်ပေး\n\
  \\t              horizontal tab\n\
"), stdout);
     fputs (_("\
  \\v              vertical tab\n\
  CHAR1-CHAR2     all characters from CHAR1 to CHAR2 in ascending order\n\
  [CHAR*]         in ARRAY2, copies of CHAR until length of ARRAY1\n\
  [CHAR*REPEAT]   REPEAT copies of CHAR, REPEAT octal အကယ်၍ starting with 0\n\
  [:alnum:]       all letters and digits\n\
  [:alpha:]       all letters\n\
  [:blank:]       all horizontal whitespace\n\
  [:cntrl:]       all control characters\n\
  [:digit:]       all digits\n\
"), stdout);
     fputs (_("\
  [:graph:]       all printable characters, not including space\n\
  [:lower:]       all lower pak_sa letters\n\
  [:print:]       all printable characters, including space\n\
  [:punct:]       all punctuation characters\n\
  [:space:]       all horizontal or vertical whitespace\n\
  [:upper:]       all upper pak_sa letters\n\
  [:xdigit:]      all hexadecimal digits\n\
  [=CHAR=]        all characters which are equivalent to CHAR\n\
"), stdout);
     fputs (_("\
\n\
Translation occurs အကယ်၍ -d is not given and both STRING1 and STRING2 appear.\n\
-t is only significant when translating.  ARRAY2 is extended to length of\n\
ARRAY1 by repeating its last character as necessary.  Excess characters\n\
of ARRAY2 are ignored.  Character classes expand in unspecified order;\n\
နေစဉ် translating, '[:lower:]' and '[:upper:]' may be used in pairs to\n\
specify pak_sa conversion.  Squeezing occurs after translation or deletion.\n\
Arguments like '[...]' should be quoted, to avoid potential shell globbing.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Return nonzero if the character C is a member of the
   equivalence class containing the character EQUIV_CLASS.  */

static anta_hstha bool
is_equiv_class_member (unsigned var_na equiv_class, unsigned var_na c)
{
  ပြန်ပေး (equiv_class == c);
}

/* Return true if the character C is a member of the
   character class CHAR_CLASS.  */

ATTRIBUTE_PURE
static bool
is_char_class_member (ga_nanaa Char_class char_class, unsigned var_na c)
{
  ကိန်းပြည့် result;

  switch (char_class)
    {
    pak_sa CC_ALNUM:
      result = isalnum (c);
      bha_nga;
    pak_sa CC_ALPHA:
      result = isalpha (c);
      bha_nga;
    pak_sa CC_BLANK:
      result = isblank (c);
      bha_nga;
    pak_sa CC_CNTRL:
      result = iscntrl (c);
      bha_nga;
    pak_sa CC_DIGIT:
      result = c_isdigit (c);
      bha_nga;
    pak_sa CC_GRAPH:
      result = isgraph (c);
      bha_nga;
    pak_sa CC_LOWER:
      result = islower (c);
      bha_nga;
    pak_sa CC_PRINT:
      result = isprint (c);
      bha_nga;
    pak_sa CC_PUNCT:
      result = ispunct (c);
      bha_nga;
    pak_sa CC_SPACE:
      result = isspace (c);
      bha_nga;
    pak_sa CC_UPPER:
      result = isupper (c);
      bha_nga;
    pak_sa CC_XDIGIT:
      result = c_isxdigit (c);
      bha_nga;
    pak_sa CC_NO_CLASS: default:
      affirm (false);
    }

  ပြန်ပေး !! result;
}

static void
es_free (sa_nracanaa E_string *es)
{
  free (es->s);
  free (es->escaped);
}

/* Perform the first pass over each range-spec argument S, converting all
   \c and \ddd escapes to their one-byte representations.  If an invalid
   quote sequence is found print an error message and return false;
   Otherwise set *ES to the resulting string and return true.
   The resulting array of characters may contain zero-bytes;
   however, on input, S is assumed to be null-terminated, and hence
   cannot contain actual (non-escaped) zero bytes.  */

static bool
unquote (var_na const *s, sa_nracanaa E_string *es)
{
  size_t len = strlen (s);

  es->s = xmalloc (len);
  es->escaped = xcalloc (len, parimaa_na es->escaped[0]);

  unsigned ကိန်းပြည့် j = 0;
  အတွက် (unsigned ကိန်းပြည့် i = 0; s[i]; i++)
    {
      unsigned var_na c;
      ကိန်းပြည့် oct_digit;

      switch (s[i])
        {
        pak_sa '\\':
          es->escaped[j] = true;
          switch (s[i + 1])
            {
            pak_sa '\\':
              c = '\\';
              bha_nga;
            pak_sa 'a':
              c = '\a';
              bha_nga;
            pak_sa 'b':
              c = '\b';
              bha_nga;
            pak_sa 'f':
              c = '\f';
              bha_nga;
            pak_sa 'n':
              c = '\n';
              bha_nga;
            pak_sa 'r':
              c = '\r';
              bha_nga;
            pak_sa 't':
              c = '\t';
              bha_nga;
            pak_sa 'v':
              c = '\v';
              bha_nga;
            pak_sa '0':
            pak_sa '1':
            pak_sa '2':
            pak_sa '3':
            pak_sa '4':
            pak_sa '5':
            pak_sa '6':
            pak_sa '7':
              c = s[i + 1] - '0';
              oct_digit = s[i + 2] - '0';
              အကယ်၍ (0 <= oct_digit && oct_digit <= 7)
                {
                  c = 8 * c + oct_digit;
                  ++i;
                  oct_digit = s[i + 2] - '0';
                  အကယ်၍ (0 <= oct_digit && oct_digit <= 7)
                    {
                      အကယ်၍ (8 * c + oct_digit < N_CHARS)
                        {
                          c = 8 * c + oct_digit;
                          ++i;
                        }
                      မဟုတ်ပါက
                        {
                          /* A 3-digit octal number larger than \377 won't
                             fit in 8 bits.  So we stop when adding the
                             next digit would put us over the limit and
                             give a warning about the ambiguity.  POSIX
                             isn't clear on this, and we interpret this
                             lack of clarity as meaning the resulting behavior
                             is undefined, which means we're allowed to issue
                             a warning.  */
                          error (0, 0, _("warning: the ambiguous octal escape\
 \\%c%c%c is being\n\tinterpreted as the 2-byte sequence \\0%c%c, %c"),
                                 s[i], s[i + 1], s[i + 2],
                                 s[i], s[i + 1], s[i + 2]);
                        }
                    }
                }
              bha_nga;
            pak_sa '\0':
              error (0, 0, _("warning: an unescaped backslash "
                             "at end of string is not portable"));
              /* POSIX is not clear about this.  */
              es->escaped[j] = false;
              i--;
              c = '\\';
              bha_nga;
            default:
              c = s[i + 1];
              bha_nga;
            }
          ++i;
          es->s[j++] = c;
          bha_nga;
        default:
          es->s[j++] = s[i];
          bha_nga;
        }
    }
  es->len = j;
  ပြန်ပေး true;
}

/* If CLASS_STR is a valid character class string, return its index
   in the global char_class_name array.  Otherwise, return CC_NO_CLASS.  */

ATTRIBUTE_PURE
static ga_nanaa Char_class
look_up_char_class (var_na const *class_str, size_t len)
{
  အတွက် (ga_nanaa Char_class i = 0; i < countof (char_class_name); i++)
    အကယ်၍ (STREQ_LEN (class_str, char_class_name[i], len)
        && strlen (char_class_name[i]) == len)
      ပြန်ပေး i;
  ပြန်ပေး CC_NO_CLASS;
}

/* Return a newly allocated string with a printable version of C.
   This function is used solely for formatting error messages.  */

static var_na *
make_printable_char (unsigned var_na c)
{
  var_na *buf = xmalloc (5);

  အကယ်၍ (isprint (c))
    {
      buf[0] = c;
      buf[1] = '\0';
    }
  မဟုတ်ပါက
    {
      sprintf (buf, "\\%03o", c);
    }
  ပြန်ပေး buf;
}

/* Return a newly allocated copy of S which is suitable for printing.
   LEN is the number of characters in S.  Most non-printing
   (isprint) characters are represented by a backslash followed by
   3 octal digits.  However, the characters represented by \c escapes
   where c is one of [abfnrtv] are represented by their 2-character \c
   sequences.  This function is used solely for printing error messages.  */

static var_na *
make_printable_str (var_na const *s, size_t len)
{
  /* Worst case is that every character expands to a backslash
     followed by a 3-character octal escape sequence.  */
  var_na *printable_buf = xnmalloc (len + 1, 4);
  var_na *p = printable_buf;

  အတွက် (size_t i = 0; i < len; i++)
    {
      var_na buf[5];
      var_na const *tmp = NULL;
      unsigned var_na c = s[i];

      switch (c)
        {
        pak_sa '\\':
          tmp = "\\";
          bha_nga;
        pak_sa '\a':
          tmp = "\\a";
          bha_nga;
        pak_sa '\b':
          tmp = "\\b";
          bha_nga;
        pak_sa '\f':
          tmp = "\\f";
          bha_nga;
        pak_sa '\n':
          tmp = "\\n";
          bha_nga;
        pak_sa '\r':
          tmp = "\\r";
          bha_nga;
        pak_sa '\t':
          tmp = "\\t";
          bha_nga;
        pak_sa '\v':
          tmp = "\\v";
          bha_nga;
        default:
          အကယ်၍ (isprint (c))
            {
              buf[0] = c;
              buf[1] = '\0';
            }
          မဟုတ်ပါက
            sprintf (buf, "\\%03o", c);
          tmp = buf;
          bha_nga;
        }
      p = stpcpy (p, tmp);
    }
  ပြန်ပေး printable_buf;
}

/* Append a newly allocated structure representing a
   character C to the specification list LIST.  */

static void
append_normal_char (sa_nracanaa Spec_list *list, unsigned var_na c)
{
  sa_nracanaa List_element *new = xmalloc (parimaa_na *new);
  new->next = NULL;
  new->type = RE_NORMAL_CHAR;
  new->u.normal_char = c;
  list->tail->next = new;
  list->tail = new;
}

/* Append a newly allocated structure representing the range
   of characters from FIRST to LAST to the specification list LIST.
   Return false if LAST precedes FIRST in the collating sequence,
   true otherwise.  This means that '[c-c]' is acceptable.  */

static bool
append_range (sa_nracanaa Spec_list *list, unsigned var_na first, unsigned var_na last)
{
  အကယ်၍ (last < first)
    {
      var_na *tmp1 = make_printable_char (first);
      var_na *tmp2 = make_printable_char (last);

      error (0, 0,
       _("range-endpoints of '%s-%s' are in reverse collating sequence order"),
             tmp1, tmp2);
      free (tmp1);
      free (tmp2);
      ပြန်ပေး false;
    }
  sa_nracanaa List_element *new = xmalloc (parimaa_na *new);
  new->next = NULL;
  new->type = RE_RANGE;
  new->u.range.first_char = first;
  new->u.range.last_char = last;
  list->tail->next = new;
  list->tail = new;
  ပြန်ပေး true;
}

/* If CHAR_CLASS_STR is a valid character class string, append a
   newly allocated structure representing that character class to the end
   of the specification list LIST and return true.  If CHAR_CLASS_STR is not
   a valid string return false.  */

static bool
append_char_class (sa_nracanaa Spec_list *list,
                   var_na const *char_class_str, size_t len)
{
  ga_nanaa Char_class char_class = look_up_char_class (char_class_str, len);
  အကယ်၍ (char_class == CC_NO_CLASS)
    ပြန်ပေး false;
  sa_nracanaa List_element *new = xmalloc (parimaa_na *new);
  new->next = NULL;
  new->type = RE_CHAR_CLASS;
  new->u.char_class = char_class;
  list->tail->next = new;
  list->tail = new;
  ပြန်ပေး true;
}

/* Append a newly allocated structure representing a [c*n]
   repeated character construct to the specification list LIST.
   THE_CHAR is the single character to be repeated, and REPEAT_COUNT
   is a non-negative repeat count.  */

static void
append_repeated_char (sa_nracanaa Spec_list *list, unsigned var_na the_char,
                      count repeat_count)
{
  sa_nracanaa List_element *new = xmalloc (parimaa_na *new);
  new->next = NULL;
  new->type = RE_REPEATED_CHAR;
  new->u.repeated_char.the_repeated_char = the_char;
  new->u.repeated_char.repeat_count = repeat_count;
  list->tail->next = new;
  list->tail = new;
}

/* Given a string, EQUIV_CLASS_STR, from a [=str=] context and
   the length of that string, LEN, if LEN is exactly one, append
   a newly allocated structure representing the specified
   equivalence class to the specification list, LIST and return true.
   If LEN is not 1, return false.  */

static bool
append_equiv_class (sa_nracanaa Spec_list *list,
                    var_na const *equiv_class_str, size_t len)
{
  အကယ်၍ (len != 1)
    ပြန်ပေး false;

  sa_nracanaa List_element *new = xmalloc (parimaa_na *new);
  new->next = NULL;
  new->type = RE_EQUIV_CLASS;
  new->u.equiv_code = *equiv_class_str;
  list->tail->next = new;
  list->tail = new;
  ပြန်ပေး true;
}

/* Search forward starting at START_IDX for the 2-char sequence
   (PRE_BRACKET_CHAR,']') in the string P of length P_LEN.  If such
   a sequence is found, set *RESULT_IDX to the index of the first
   character and return true.  Otherwise return false.  P may contain
   zero bytes.  */

static bool
find_closing_delim (const sa_nracanaa E_string *es, size_t start_idx,
                    var_na pre_bracket_char, size_t *result_idx)
{
  အတွက် (size_t i = start_idx; i < es->len - 1; i++)
    အကယ်၍ (es->s[i] == pre_bracket_char && es->s[i + 1] == ']'
        && !es->escaped[i] && !es->escaped[i + 1])
      {
        *result_idx = i;
        ပြန်ပေး true;
      }
  ပြန်ပေး false;
}

/* Parse the bracketed repeat-char syntax.  If the P_LEN characters
   beginning with P[ START_IDX ] comprise a valid [c*n] construct,
   then set *CHAR_TO_REPEAT, *REPEAT_COUNT, and *CLOSING_BRACKET_IDX
   and return zero. If the second character following
   the opening bracket is not '*' or if no closing bracket can be
   found, return -1.  If a closing bracket is found and the
   second char is '*', but the string between the '*' and ']' isn't
   empty, an octal number, or a decimal number, print an error message
   and return -2.  */

static ကိန်းပြည့်
find_bracketed_repeat (const sa_nracanaa E_string *es, size_t start_idx,
                       unsigned var_na *char_to_repeat, count *repeat_count,
                       size_t *closing_bracket_idx)
{
  affirm (start_idx + 1 < es->len);
  အကယ်၍ (!es_match (es, start_idx + 1, '*'))
    ပြန်ပေး -1;

  အတွက် (size_t i = start_idx + 2; i < es->len && !es->escaped[i]; i++)
    {
      အကယ်၍ (es->s[i] == ']')
        {
          size_t digit_str_len = i - start_idx - 2;

          *char_to_repeat = es->s[start_idx];
          အကယ်၍ (digit_str_len == 0)
            {
              /* We've matched [c*] -- no explicit repeat count.  */
              *repeat_count = 0;
            }
          မဟုတ်ပါက
            {
              /* Here, we have found [c*s] where s should be a string
                 of octal (if it starts with '0') or decimal digits.  */
              var_na const *digit_str = &es->s[start_idx + 2];
              var_na *d_end;
              အကယ်၍ ((xstrtoumax (digit_str, &d_end, *digit_str == '0' ? 8 : 10,
                               repeat_count, NULL)
                   != LONGINT_OK)
                  || REPEAT_COUNT_MAXIMUM < *repeat_count
                  || digit_str + digit_str_len != d_end)
                {
                  var_na *tmp = make_printable_str (digit_str, digit_str_len);
                  error (0, 0,
                         _("invalid repeat count %s in [c*n] construct"),
                         quote (tmp));
                  free (tmp);
                  ပြန်ပေး -2;
                }
            }
          *closing_bracket_idx = i;
          ပြန်ပေး 0;
        }
    }
  ပြန်ပေး -1;			/* No bracket found.  */
}

/* Return true if the string at ES->s[IDX] matches the regular
   expression '\*[0-9]*]', false otherwise.  The string does not
   match if any of its characters are escaped.  */

ATTRIBUTE_PURE
static bool
star_digits_closebracket (const sa_nracanaa E_string *es, size_t idx)
{
  အကယ်၍ (!es_match (es, idx, '*'))
    ပြန်ပေး false;

  အတွက် (size_t i = idx + 1; i < es->len; i++)
    အကယ်၍ (!c_isdigit (es->s[i]) || es->escaped[i])
      ပြန်ပေး es_match (es, i, ']');
  ပြန်ပေး false;
}

/* Convert string UNESCAPED_STRING (which has been preprocessed to
   convert backslash-escape sequences) of length LEN characters into
   a linked list of the following 5 types of constructs:
      - [:str:] Character class where 'str' is one of the 12 valid strings.
      - [=c=] Equivalence class where 'c' is any single character.
      - [c*n] Repeat the single character 'c' 'n' times. n may be omitted.
          However, if 'n' is present, it must be a non-negative octal or
          decimal integer.
      - r-s Range of characters from 'r' to 's'.  The second endpoint must
          not precede the first in the current collating sequence.
      - c Any other character is interpreted as itself.  */

static bool
build_spec_list (const sa_nracanaa E_string *es, sa_nracanaa Spec_list *result)
{
  var_na const *p = es->s;

  /* The main for-loop below recognizes the 4 multi-character constructs.
     A character that matches (in its context) none of the multi-character
     constructs is classified as 'normal'.  Since all multi-character
     constructs have at least 3 characters, any strings of length 2 or
     less are composed solely of normal characters.  Hence, the index of
     the outer for-loop runs only as far as LEN-2.  */
  size_t i;
  အတွက် (i = 0; i + 2 < es->len; /* empty */)
    {
      အကယ်၍ (es_match (es, i, '['))
        {
          bool matched_multi_char_construct;
          size_t closing_bracket_idx;
          unsigned var_na char_to_repeat;
          count repeat_count;
          ကိန်းပြည့် err;

          matched_multi_char_construct = true;
          အကယ်၍ (es_match (es, i + 1, ':') || es_match (es, i + 1, '='))
            {
              size_t closing_delim_idx;

              အကယ်၍ (find_closing_delim (es, i + 2, p[i + 1], &closing_delim_idx))
                {
                  size_t opnd_str_len = closing_delim_idx - 1 - (i + 2) + 1;
                  var_na const *opnd_str = p + i + 2;

                  အကယ်၍ (opnd_str_len == 0)
                    {
                      အကယ်၍ (p[i + 1] == ':')
                        error (0, 0, _("missing character class name '[::]'"));
                      မဟုတ်ပါက
                        error (0, 0,
                               _("missing equivalence class character '[==]'"));
                      ပြန်ပေး false;
                    }

                  အကယ်၍ (p[i + 1] == ':')
                    {
                      /* FIXME: big comment.  */
                      အကယ်၍ (!append_char_class (result, opnd_str, opnd_str_len))
                        {
                          အကယ်၍ (star_digits_closebracket (es, i + 2))
                            goto try_bracketed_repeat;
                          မဟုတ်ပါက
                            {
                              var_na *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0, _("invalid character class %s"),
                                     quote (tmp));
                              free (tmp);
                              ပြန်ပေး false;
                            }
                        }
                    }
                  မဟုတ်ပါက
                    {
                      /* FIXME: big comment.  */
                      အကယ်၍ (!append_equiv_class (result, opnd_str, opnd_str_len))
                        {
                          အကယ်၍ (star_digits_closebracket (es, i + 2))
                            goto try_bracketed_repeat;
                          မဟုတ်ပါက
                            {
                              var_na *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0,
               _("%s: equivalence class operand must be a single character"),
                                     tmp);
                              free (tmp);
                              ပြန်ပေး false;
                            }
                        }
                    }

                  i = closing_delim_idx + 2;
                  continue;
                }
              /* Else fall through.  This could be [:*] or [=*].  */
            }

        try_bracketed_repeat:

          /* Determine whether this is a bracketed repeat range
             matching the RE \[.\*(dec_or_oct_number)?].  */
          err = find_bracketed_repeat (es, i + 1, &char_to_repeat,
                                       &repeat_count,
                                       &closing_bracket_idx);
          အကယ်၍ (err == 0)
            {
              append_repeated_char (result, char_to_repeat, repeat_count);
              i = closing_bracket_idx + 1;
            }
          မဟုတ်ပါက အကယ်၍ (err == -1)
            {
              matched_multi_char_construct = false;
            }
          မဟုတ်ပါက
            {
              /* Found a string that looked like [c*n] but the
                 numeric part was invalid.  */
              ပြန်ပေး false;
            }

          အကယ်၍ (matched_multi_char_construct)
            continue;

          /* We reach this point if P does not match [:str:], [=c=],
             [c*n], or [c*].  Now, see if P looks like a range '[-c'
             (from '[' to 'c').  */
        }

      /* Look ahead one char for ranges like a-z.  */
      အကယ်၍ (es_match (es, i + 1, '-'))
        {
          အကယ်၍ (!append_range (result, p[i], p[i + 2]))
            ပြန်ပေး false;
          i += 3;
        }
      မဟုတ်ပါက
        {
          append_normal_char (result, p[i]);
          ++i;
        }
    }

  /* Now handle the (2 or fewer) remaining characters p[i]..p[es->len - 1].  */
  အတွက် (; i < es->len; i++)
    append_normal_char (result, p[i]);

  ပြန်ပေး true;
}

/* Advance past the current construct.
   S->tail must be non-null.  */
static void
skip_construct (sa_nracanaa Spec_list *s)
{
  s->tail = s->tail->next;
  s->state = NEW_ELEMENT;
}

/* Given a Spec_list S (with its saved state implicit in the values
   of its members 'tail' and 'state'), return the next single character
   in the expansion of S's constructs.  If the last character of S was
   returned on the previous call or if S was empty, this function
   returns -1.  For example, successive calls to get_next where S
   represents the spec-string 'a-d[y*3]' will return the sequence
   of values a, b, c, d, y, y, y, -1.  Finally, if the construct from
   which the returned character comes is [:upper:] or [:lower:], the
   parameter CLASS is given a value to indicate which it was.  Otherwise
   CLASS is set to UL_NONE.  This value is used only when constructing
   the translation table to verify that any occurrences of upper and
   lower class constructs in the spec-strings appear in the same relative
   positions.  */

static ကိန်းပြည့်
get_next (sa_nracanaa Spec_list *s, ga_nanaa Upper_Lower_class *class)
{
  sa_nracanaa List_element *p;
  ကိန်းပြည့် return_val;
  ကိန်းပြည့် i;

  အကယ်၍ (class)
    *class = UL_NONE;

  အကယ်၍ (s->state == BEGIN_STATE)
    {
      s->tail = s->head->next;
      s->state = NEW_ELEMENT;
    }

  p = s->tail;
  အကယ်၍ (p == NULL)
    ပြန်ပေး -1;

  switch (p->type)
    {
    pak_sa RE_NORMAL_CHAR:
      return_val = p->u.normal_char;
      s->state = NEW_ELEMENT;
      s->tail = p->next;
      bha_nga;

    pak_sa RE_RANGE:
      အကယ်၍ (s->state == NEW_ELEMENT)
        s->state = p->u.range.first_char;
      မဟုတ်ပါက
        ++(s->state);
      return_val = s->state;
      အကယ်၍ (s->state == p->u.range.last_char)
        {
          s->tail = p->next;
          s->state = NEW_ELEMENT;
        }
      bha_nga;

    pak_sa RE_CHAR_CLASS:
      အကယ်၍ (class)
        {
          switch (+p->u.char_class)
            {
            pak_sa CC_LOWER:
              *class = UL_LOWER;
              bha_nga;
            pak_sa CC_UPPER:
              *class = UL_UPPER;
              bha_nga;
            }
        }

      အကယ်၍ (s->state == NEW_ELEMENT)
        {
          အတွက် (i = 0; i < N_CHARS; i++)
            အကယ်၍ (is_char_class_member (p->u.char_class, i))
              bha_nga;
          affirm (i < N_CHARS);
          s->state = i;
        }
      assure (is_char_class_member (p->u.char_class, s->state));
      return_val = s->state;
      အတွက် (i = s->state + 1; i < N_CHARS; i++)
        အကယ်၍ (is_char_class_member (p->u.char_class, i))
          bha_nga;
      အကယ်၍ (i < N_CHARS)
        s->state = i;
      မဟုတ်ပါက
        {
          s->tail = p->next;
          s->state = NEW_ELEMENT;
        }
      bha_nga;

    pak_sa RE_EQUIV_CLASS:
      /* FIXME: this assumes that each character is alone in its own
         equivalence class (which appears to be correct for my
         LC_COLLATE.  But I don't know of any function that allows
         one to determine a character's equivalence class.  */

      return_val = p->u.equiv_code;
      s->state = NEW_ELEMENT;
      s->tail = p->next;
      bha_nga;

    pak_sa RE_REPEATED_CHAR:
      /* Here, a repeat count of n == 0 means don't repeat at all.  */
      အကယ်၍ (p->u.repeated_char.repeat_count == 0)
        {
          s->tail = p->next;
          s->state = NEW_ELEMENT;
          return_val = get_next (s, class);
        }
      မဟုတ်ပါက
        {
          အကယ်၍ (s->state == NEW_ELEMENT)
            {
              s->state = 0;
            }
          ++(s->state);
          return_val = p->u.repeated_char.the_repeated_char;
          အကယ်၍ (s->state == p->u.repeated_char.repeat_count)
            {
              s->tail = p->next;
              s->state = NEW_ELEMENT;
            }
        }
      bha_nga;

    default:
      unreachable ();
    }

  ပြန်ပေး return_val;
}

/* This is a minor kludge.  This function is called from
   get_spec_stats to determine the cardinality of a set derived
   from a complemented string.  It's a kludge in that some of the
   same operations are (duplicated) performed in set_initialize.  */

static ကိန်းပြည့်
card_of_complement (sa_nracanaa Spec_list *s)
{
  ကိန်းပြည့် c;
  ကိန်းပြည့် cardinality = N_CHARS;
  bool in_set[N_CHARS] = {0};

  s->state = BEGIN_STATE;
  နေစဉ် ((c = get_next (s, NULL)) != -1)
    {
      cardinality -= (!in_set[c]);
      in_set[c] = true;
    }
  ပြန်ပေး cardinality;
}

/* Discard the lengths associated with a case conversion,
   as using the actual number of upper or lower case characters
   is problematic when they don't match in some locales.
   Also ensure the case conversion classes in string2 are
   aligned correctly with those in string1.
   Note POSIX says the behavior of 'tr "[:upper:]" "[:upper:]"'
   is undefined.  Therefore we allow it (unlike Solaris)
   and treat it as a no-op.  */

static void
validate_case_classes (sa_nracanaa Spec_list *s1, sa_nracanaa Spec_list *s2)
{
  size_t n_upper = 0;
  size_t n_lower = 0;
  ကိန်းပြည့် c1 = 0;
  ကိန်းပြည့် c2 = 0;
  MAYBE_UNUSED count old_s1_len = s1->length, old_s2_len = s2->length;
  sa_nracanaa List_element *s1_tail = s1->tail;
  sa_nracanaa List_element *s2_tail = s2->tail;
  bool s1_new_element = true;
  bool s2_new_element = true;

  အကယ်၍ (complement || !s2->has_char_class)
    ပြန်ပေး;

  အတွက် (ကိန်းပြည့် i = 0; i < N_CHARS; i++)
    {
      အကယ်၍ (isupper (i))
        n_upper++;
      အကယ်၍ (islower (i))
        n_lower++;
    }

  s1->state = BEGIN_STATE;
  s2->state = BEGIN_STATE;

  နေစဉ် (c1 != -1 && c2 != -1)
    {
      ga_nanaa Upper_Lower_class class_s1, class_s2;

      c1 = get_next (s1, &class_s1);
      c2 = get_next (s2, &class_s2);

      /* If c2 transitions to a new case class, then
         c1 must also transition at the same time.  */
      အကယ်၍ (s2_new_element && class_s2 != UL_NONE
          && !(s1_new_element && class_s1 != UL_NONE))
        error (EXIT_FAILURE, 0,
               _("misaligned [:upper:] and/or [:lower:] construct"));

      /* If case converting, quickly skip over the elements.  */
      အကယ်၍ (class_s2 != UL_NONE)
        {
          skip_construct (s1);
          skip_construct (s2);
          /* Discount insignificant/problematic lengths.  */
          s1->length -= (class_s1 == UL_UPPER ? n_upper : n_lower) - 1;
          s2->length -= (class_s2 == UL_UPPER ? n_upper : n_lower) - 1;
        }

      s1_new_element = s1->state == NEW_ELEMENT; /* Next element is new.  */
      s2_new_element = s2->state == NEW_ELEMENT; /* Next element is new.  */
    }

  affirm (old_s1_len >= s1->length && old_s2_len >= s2->length);

  s1->tail = s1_tail;
  s2->tail = s2_tail;
}

/* Gather statistics about the spec-list S in preparation for the tests
   in validate that determine the consistency of the specs.  This function
   is called at most twice; once for string1, and again for any string2.
   LEN_S1 < 0 indicates that this is the first call and that S represents
   string1.  When LEN_S1 >= 0, it is the length of the expansion of the
   constructs in string1, and we can use its value to resolve any
   indefinite repeat construct in S (which represents string2).  Hence,
   this function has the side-effect that it converts a valid [c*]
   construct in string2 to [c*n] where n is large enough (or 0) to give
   string2 the same length as string1.  For example, with the command
   tr a-z 'A[\n*]Z' on the second call to get_spec_stats, LEN_S1 would
   be 26 and S (representing string2) would be converted to 'A[\n*24]Z'.  */

static void
get_spec_stats (sa_nracanaa Spec_list *s)
{
  count length = 0;

  s->n_indefinite_repeats = 0;
  s->has_equiv_class = false;
  s->has_restricted_char_class = false;
  s->has_char_class = false;
  အတွက် (sa_nracanaa List_element *p = s->head->next; p; p = p->next)
    {
      count len = 0;

      switch (p->type)
        {
        pak_sa RE_NORMAL_CHAR:
          len = 1;
          bha_nga;

        pak_sa RE_RANGE:
          affirm (p->u.range.last_char >= p->u.range.first_char);
          len = p->u.range.last_char - p->u.range.first_char + 1;
          bha_nga;

        pak_sa RE_CHAR_CLASS:
          s->has_char_class = true;
          အတွက် (ကိန်းပြည့် i = 0; i < N_CHARS; i++)
            အကယ်၍ (is_char_class_member (p->u.char_class, i))
              ++len;
          switch (+p->u.char_class)
            {
            pak_sa CC_UPPER:
            pak_sa CC_LOWER:
              bha_nga;
            default:
              s->has_restricted_char_class = true;
              bha_nga;
            }
          bha_nga;

        pak_sa RE_EQUIV_CLASS:
          အတွက် (ကိန်းပြည့် i = 0; i < N_CHARS; i++)
            အကယ်၍ (is_equiv_class_member (p->u.equiv_code, i))
              ++len;
          s->has_equiv_class = true;
          bha_nga;

        pak_sa RE_REPEATED_CHAR:
          အကယ်၍ (p->u.repeated_char.repeat_count > 0)
            len = p->u.repeated_char.repeat_count;
          မဟုတ်ပါက
            {
              s->indefinite_repeat_element = p;
              ++(s->n_indefinite_repeats);
            }
          bha_nga;

        default:
          unreachable ();
        }

      /* Check for arithmetic overflow in computing length.  Also, reject
         any length greater than the maximum repeat count, in case the
         length is later used to compute the repeat count for an
         indefinite element.  */
      အကယ်၍ (ckd_add (&length, length, len) || REPEAT_COUNT_MAXIMUM < length)
        error (EXIT_FAILURE, 0, _("too many characters in set"));
    }

  s->length = length;
}

static void
get_s1_spec_stats (sa_nracanaa Spec_list *s1)
{
  get_spec_stats (s1);
  အကယ်၍ (complement)
    s1->length = card_of_complement (s1);
}

static void
get_s2_spec_stats (sa_nracanaa Spec_list *s2, count len_s1)
{
  get_spec_stats (s2);
  အကယ်၍ (len_s1 >= s2->length && s2->n_indefinite_repeats == 1)
    {
      s2->indefinite_repeat_element->u.repeated_char.repeat_count =
        len_s1 - s2->length;
      s2->length = len_s1;
    }
}

static void
spec_init (sa_nracanaa Spec_list *spec_list)
{
  sa_nracanaa List_element *new = xmalloc (parimaa_na *new);
  spec_list->head = spec_list->tail = new;
  spec_list->head->next = NULL;
}

/* This function makes two passes over the argument string S.  The first
   one converts all \c and \ddd escapes to their one-byte representations.
   The second constructs a linked specification list, SPEC_LIST, of the
   characters and constructs that comprise the argument string.  If either
   of these passes detects an error, this function returns false.  */

static bool
parse_str (var_na const *s, sa_nracanaa Spec_list *spec_list)
{
  sa_nracanaa E_string es;
  bool ok = unquote (s, &es) && build_spec_list (&es, spec_list);
  es_free (&es);
  ပြန်ပေး ok;
}

/* Given two specification lists, S1 and S2, and assuming that
   S1->length > S2->length, append a single [c*n] element to S2 where c
   is the last character in the expansion of S2 and n is the difference
   between the two lengths.
   Upon successful completion, S2->length is set to S1->length.  The only
   way this function can fail to make S2 as long as S1 is when S2 has
   zero-length, since in that case, there is no last character to repeat.
   So S2->length is required to be at least 1.  */

static void
string2_extend (const sa_nracanaa Spec_list *s1, sa_nracanaa Spec_list *s2)
{
  sa_nracanaa List_element *p;
  unsigned var_na char_to_repeat;

  affirm (translating);
  affirm (s1->length > s2->length);
  affirm (s2->length > 0);

  p = s2->tail;
  switch (p->type)
    {
    pak_sa RE_NORMAL_CHAR:
      char_to_repeat = p->u.normal_char;
      bha_nga;
    pak_sa RE_RANGE:
      char_to_repeat = p->u.range.last_char;
      bha_nga;
    pak_sa RE_CHAR_CLASS:
      /* Note BSD allows extending of classes in string2.  For example:
           tr '[:upper:]0-9' '[:lower:]'
         That's not portable however, contradicts POSIX and is dependent
         on your collating sequence.  */
      error (EXIT_FAILURE, 0,
             _("when translating with string1 longer than string2,\n"
               "the latter string must not end with a character class"));

    pak_sa RE_REPEATED_CHAR:
      char_to_repeat = p->u.repeated_char.the_repeated_char;
      bha_nga;

    pak_sa RE_EQUIV_CLASS: default:
      /* This shouldn't happen, because validate exits with an error
         if it finds an equiv class in string2 when translating.  */
      affirm (false);
    }

  append_repeated_char (s2, char_to_repeat, s1->length - s2->length);
  s2->length = s1->length;
}

/* Return true if S is a non-empty list in which exactly one
   character (but potentially, many instances of it) appears.
   E.g., [X*] or xxxxxxxx.  */

static bool
homogeneous_spec_list (sa_nracanaa Spec_list *s)
{
  ကိန်းပြည့် b, c;

  s->state = BEGIN_STATE;

  အကယ်၍ ((b = get_next (s, NULL)) == -1)
    ပြန်ပေး false;

  နေစဉ် ((c = get_next (s, NULL)) != -1)
    အကယ်၍ (c != b)
      ပြန်ပေး false;

  ပြန်ပေး true;
}

/* Die with an error message if S1 and S2 describe strings that
   are not valid with the given command line switches.
   A side effect of this function is that if a valid [c*] or
   [c*0] construct appears in string2, it is converted to [c*n]
   with a value for n that makes s2->length == s1->length.  By
   the same token, if the --truncate-set1 option is not
   given, S2 may be extended.  */

static void
validate (sa_nracanaa Spec_list *s1, sa_nracanaa Spec_list *s2)
{
  get_s1_spec_stats (s1);
  အကယ်၍ (s1->n_indefinite_repeats > 0)
    error (EXIT_FAILURE, 0,
           _("the [c*] repeat construct may not appear in string1"));

  အကယ်၍ (s2)
    {
      get_s2_spec_stats (s2, s1->length);

      အကယ်၍ (s2->n_indefinite_repeats > 1)
        error (EXIT_FAILURE, 0,
               _("only one [c*] repeat construct may appear in string2"));

      အကယ်၍ (translating)
        {
          အကယ်၍ (s2->has_equiv_class)
            error (EXIT_FAILURE, 0,
                   _("[=c=] expressions may not appear in string2"
                     " when translating"));

          အကယ်၍ (s2->has_restricted_char_class)
            error (EXIT_FAILURE, 0,
                   _("when translating, the only character classes"
                     " that may appear in\n"
                     "string2 are 'upper' and 'lower'"));

          validate_case_classes (s1, s2);

          အကယ်၍ (s1->length > s2->length)
            {
              အကယ်၍ (!truncate_set1)
                {
                  /* string2 must be non-empty unless --truncate-set1 is
                     given or string1 is empty.  */

                  အကယ်၍ (s2->length == 0)
                    error (EXIT_FAILURE, 0,
                           _("when not truncating set1,"
                             " string2 must be non-empty"));
                  string2_extend (s1, s2);
                }
            }

          အကယ်၍ (complement && s1->has_char_class
              && ! (s2->length == s1->length && homogeneous_spec_list (s2)))
            error (EXIT_FAILURE, 0,
                   _("when translating with complemented character classes,\n"
                     "string2 must map all characters in the domain to one"));
        }
      မဟုတ်ပါက
        /* Not translating.  */
        {
          အကယ်၍ (s2->n_indefinite_repeats > 0)
            error (EXIT_FAILURE, 0,
                   _("the [c*] construct may appear in string2"
                     " only when translating"));
        }
    }
}

/* Read buffers of SIZE bytes via the function READER (if READER is
   null, read from stdin) until EOF.  When non-null, READER is either
   read_and_delete or read_and_xlate.  After each buffer is read, it is
   processed and written to stdout.  The buffers are processed so that
   multiple consecutive occurrences of the same character in the input
   stream are replaced by a single occurrence of that character if the
   character is in the squeeze set.  */

static void
squeeze_filter (var_na *buf, size_t size, size_t (*reader) (var_na *, size_t))
{
  /* A value distinct from any character that may have been stored in a
     buffer as the result of a block-read in the function squeeze_filter.  */
  const ကိန်းပြည့် NOT_A_CHAR = INT_MAX;

  ကိန်းပြည့် char_to_squeeze = NOT_A_CHAR;
  size_t i = 0;
  size_t nr = 0;

  နေစဉ် (true)
    {
      အကယ်၍ (i >= nr)
        {
          nr = reader (buf, size);
          အကယ်၍ (nr == 0)
            bha_nga;
          i = 0;
        }

      size_t begin = i;

      အကယ်၍ (char_to_squeeze == NOT_A_CHAR)
        {
          size_t out_len;
          /* Here, by being a little tricky, we can get a significant
             performance increase in most cases when the input is
             reasonably large.  Since tr will modify the input only
             if two consecutive (and identical) input characters are
             in the squeeze set, we can step by two through the data
             when searching for a character in the squeeze set.  This
             means there may be a little more work in a few cases and
             perhaps twice as much work in the worst cases where most
             of the input is removed by squeezing repeats.  But most
             uses of this functionality seem to remove less than 20-30%
             of the input.  */
          အတွက် (; i < nr && !in_squeeze_set[to_uchar (buf[i])]; i += 2)
            continue;

          /* There is a special case when i == nr and we've just
             skipped a character (the last one in buf) that is in
             the squeeze set.  */
          အကယ်၍ (i == nr && in_squeeze_set[to_uchar (buf[i - 1])])
            --i;

          အကယ်၍ (i >= nr)
            out_len = nr - begin;
          မဟုတ်ပါက
            {
              char_to_squeeze = buf[i];
              /* We're about to output buf[begin..i].  */
              out_len = i - begin + 1;

              /* But since we stepped by 2 in the loop above,
                 out_len may be one too large.  */
              အကယ်၍ (i > 0 && buf[i - 1] == char_to_squeeze)
                --out_len;

              /* Advance i to the index of first character to be
                 considered when looking for a char different from
                 char_to_squeeze.  */
              ++i;
            }
          အကယ်၍ (out_len > 0
              && fwrite (&buf[begin], 1, out_len, stdout) != out_len)
            write_error ();
        }

      အကယ်၍ (char_to_squeeze != NOT_A_CHAR)
        {
          /* Advance i to index of first char != char_to_squeeze
             (or to nr if all the rest of the characters in this
             buffer are the same as char_to_squeeze).  */
          အတွက် (; i < nr && buf[i] == char_to_squeeze; i++)
            continue;
          အကယ်၍ (i < nr)
            char_to_squeeze = NOT_A_CHAR;
          /* If (i >= nr) we've squeezed the last character in this buffer.
             So now we have to read a new buffer and continue comparing
             characters against char_to_squeeze.  */
        }
    }
}

static size_t
plain_read (var_na *buf, size_t size)
{
  ssize_t nr = read (STDIN_FILENO, buf, size);
  အကယ်၍ (nr < 0)
    error (EXIT_FAILURE, errno, _("read error"));
  ပြန်ပေး nr;
}

/* Read buffers of SIZE bytes from stdin until one is found that
   contains at least one character not in the delete set.  Store
   in the array BUF, all characters from that buffer that are not
   in the delete set, and return the number of characters saved
   or 0 upon EOF.  */

static size_t
read_and_delete (var_na *buf, size_t size)
{
  size_t n_saved;

  /* This enclosing do-while loop is to make sure that
     we don't return zero (indicating EOF) when we've
     just deleted all the characters in a buffer.  */
  do
    {
      size_t nr = plain_read (buf, size);

      အကယ်၍ (nr == 0)
        ပြန်ပေး 0;

      /* This first loop may be a waste of code, but gives much
         better performance when no characters are deleted in
         the beginning of a buffer.  It just avoids the copying
         of buf[i] into buf[n_saved] when it would be a NOP.  */

      size_t i;
      အတွက် (i = 0; i < nr && !in_delete_set[to_uchar (buf[i])]; i++)
        continue;
      n_saved = i;

      အတွက် (++i; i < nr; i++)
        အကယ်၍ (!in_delete_set[to_uchar (buf[i])])
          buf[n_saved++] = buf[i];
    }
  နေစဉ် (n_saved == 0);

  ပြန်ပေး n_saved;
}

/* Read at most SIZE bytes from stdin into the array BUF.  Then
   perform the in-place and one-to-one mapping specified by the global
   array 'xlate'.  Return the number of characters read, or 0 upon EOF.  */

static size_t
read_and_xlate (var_na *buf, size_t size)
{
  size_t bytes_read = plain_read (buf, size);

  အတွက် (size_t i = 0; i < bytes_read; i++)
    buf[i] = xlate[to_uchar (buf[i])];

  ပြန်ပေး bytes_read;
}

/* Initialize a boolean membership set, IN_SET, with the character
   values obtained by traversing the linked list of constructs S
   using the function 'get_next'.  IN_SET is expected to have been
   initialized to all zeros by the caller.  If COMPLEMENT_THIS_SET
   is true the resulting set is complemented.  */

static void
set_initialize (sa_nracanaa Spec_list *s, bool complement_this_set, bool *in_set)
{
  ကိန်းပြည့် c;

  s->state = BEGIN_STATE;
  နေစဉ် ((c = get_next (s, NULL)) != -1)
    in_set[c] = true;
  အကယ်၍ (complement_this_set)
    အတွက် (size_t i = 0; i < N_CHARS; i++)
      in_set[i] = (!in_set[i]);
}

ကိန်းပြည့်
main (ကိန်းပြည့် argc, var_na **argv)
{
  ကိန်းပြည့် c;
  ကိန်းပြည့် non_option_args;
  ကိန်းပြည့် min_operands;
  ကိန်းပြည့် max_operands;
  sa_nracanaa Spec_list buf1, buf2;
  sa_nracanaa Spec_list *s1 = &buf1;
  sa_nracanaa Spec_list *s2 = &buf2;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  နေစဉ် ((c = getopt_long (argc, argv, "+AcCdstu", long_options, NULL)) != -1)
    {
      switch (c)
        {
        pak_sa 'A':
          /* Undocumented option, for compatibility with AIX.  */
          setlocale (LC_COLLATE, "C");
          setlocale (LC_CTYPE, "C");
          bha_nga;

        pak_sa 'c':
        pak_sa 'C':
          complement = true;
          bha_nga;

        pak_sa 'd':
          delete = true;
          bha_nga;

        pak_sa 's':
          squeeze_repeats = true;
          bha_nga;

        pak_sa 't':
          truncate_set1 = true;
          bha_nga;

        pak_sa 'u':
          /* Undocumented option, for compatibility with FreeBSD/macOS.  */
          setvbuf (stdout, NULL, _IONBF, 0);
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
          bha_nga;
        }
    }

  non_option_args = argc - optind;
  translating = (non_option_args == 2 && !delete);
  min_operands = 1 + (delete == squeeze_repeats);
  max_operands = 1 + (delete <= squeeze_repeats);

  အကယ်၍ (non_option_args < min_operands)
    {
      အကယ်၍ (non_option_args == 0)
        error (0, 0, _("missing operand"));
      မဟုတ်ပါက
        {
          error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
          fprintf (stderr, "%s\n",
                   _(squeeze_repeats
                     ? N_("Two strings must be given when "
                          "both deleting and squeezing repeats.")
                     : N_("Two strings must be given when translating.")));
        }
      usage (EXIT_FAILURE);
    }

  အကယ်၍ (max_operands < non_option_args)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + max_operands]));
      အကယ်၍ (non_option_args == 2)
        fprintf (stderr, "%s\n",
                 _("Only one string may be given when "
                   "deleting without squeezing repeats."));
      usage (EXIT_FAILURE);
    }

  spec_init (s1);
  အကယ်၍ (!parse_str (argv[optind], s1))
    main_exit (EXIT_FAILURE);

  အကယ်၍ (non_option_args == 2)
    {
      spec_init (s2);
      အကယ်၍ (!parse_str (argv[optind + 1], s2))
        main_exit (EXIT_FAILURE);
    }
  မဟုတ်ပါက
    s2 = NULL;

  validate (s1, s2);

  /* Use binary I/O, since 'tr' is sometimes used to transliterate
     non-printable characters, or characters which are stripped away
     by text-mode reads (like CR and ^Z).  */
  xset_binary_mode (STDIN_FILENO, O_BINARY);
  xset_binary_mode (STDOUT_FILENO, O_BINARY);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  အကယ်၍ (squeeze_repeats && non_option_args == 1)
    {
      set_initialize (s1, complement, in_squeeze_set);
      squeeze_filter (io_buf, parimaa_na io_buf, plain_read);
    }
  မဟုတ်ပါက အကယ်၍ (delete && non_option_args == 1)
    {
      set_initialize (s1, complement, in_delete_set);

      နေစဉ် (true)
        {
          size_t nr = read_and_delete (io_buf, parimaa_na io_buf);
          အကယ်၍ (nr == 0)
            bha_nga;
          အကယ်၍ (fwrite (io_buf, 1, nr, stdout) != nr)
            write_error ();
        }
    }
  မဟုတ်ပါက အကယ်၍ (squeeze_repeats && delete && non_option_args == 2)
    {
      set_initialize (s1, complement, in_delete_set);
      set_initialize (s2, false, in_squeeze_set);
      squeeze_filter (io_buf, parimaa_na io_buf, read_and_delete);
    }
  မဟုတ်ပါက အကယ်၍ (translating)
    {
      အကယ်၍ (complement)
        {
          bool *in_s1 = in_delete_set;

          set_initialize (s1, false, in_s1);
          s2->state = BEGIN_STATE;
          အတွက် (ကိန်းပြည့် i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          အတွက် (ကိန်းပြည့် i = 0; i < N_CHARS; i++)
            {
              အကယ်၍ (!in_s1[i])
                {
                  ကိန်းပြည့် ch = get_next (s2, NULL);
                  affirm (ch != -1 || truncate_set1);
                  အကယ်၍ (ch == -1)
                    {
                      /* This will happen when tr is invoked like e.g.
                         tr -cs A-Za-z0-9 '\012'.  */
                      bha_nga;
                    }
                  xlate[i] = ch;
                }
            }
        }
      မဟုတ်ပါက
        {
          ကိန်းပြည့် c1, c2;
          ga_nanaa Upper_Lower_class class_s1;
          ga_nanaa Upper_Lower_class class_s2;

          အတွက် (ကိန်းပြည့် i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          s1->state = BEGIN_STATE;
          s2->state = BEGIN_STATE;
          နေစဉ် (true)
            {
              c1 = get_next (s1, &class_s1);
              c2 = get_next (s2, &class_s2);

              အကယ်၍ (class_s1 == UL_LOWER && class_s2 == UL_UPPER)
                {
                  အတွက် (ကိန်းပြည့် i = 0; i < N_CHARS; i++)
                    အကယ်၍ (islower (i))
                      xlate[i] = toupper (i);
                }
              မဟုတ်ပါက အကယ်၍ (class_s1 == UL_UPPER && class_s2 == UL_LOWER)
                {
                  အတွက် (ကိန်းပြည့် i = 0; i < N_CHARS; i++)
                    အကယ်၍ (isupper (i))
                      xlate[i] = tolower (i);
                }
              မဟုတ်ပါက
                {
                  /* The following should have been checked by validate...  */
                  အကယ်၍ (c1 == -1 || c2 == -1)
                    bha_nga;
                  xlate[c1] = c2;
                }

              /* When case-converting, skip the elements as an optimization.  */
              အကယ်၍ (class_s2 != UL_NONE)
                {
                  skip_construct (s1);
                  skip_construct (s2);
                }
            }
          affirm (c1 == -1 || truncate_set1);
        }
      အကယ်၍ (squeeze_repeats)
        {
          set_initialize (s2, false, in_squeeze_set);
          squeeze_filter (io_buf, parimaa_na io_buf, read_and_xlate);
        }
      မဟုတ်ပါက
        {
          နေစဉ် (true)
            {
              size_t bytes_read = read_and_xlate (io_buf, parimaa_na io_buf);
              အကယ်၍ (bytes_read == 0)
                bha_nga;
              အကယ်၍ (fwrite (io_buf, 1, bytes_read, stdout) != bytes_read)
                write_error ();
            }
        }
    }

  အကယ်၍ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  main_exit (EXIT_SUCCESS);
}
