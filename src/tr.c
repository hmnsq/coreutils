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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <getopt.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "xbinary-io.h"
#அந்தர்பாவ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tr"

#define AUTHORS proper_name ("Jim Meyering")

எண்ணிக்கை { N_CHARS = UCHAR_MAX + 1 };

/* An unsigned integer type big enough to hold a repeat count or an
   unsigned character.  POSIX requires support for repeat counts as
   high as 2**31 - 1.  Since repeat counts might need to expand to
   match the length of an argument string, we need at least size_t to
   avoid arbitrary internal limits.  It doesn't cost much to use
   uintmax_t, though.  */
வகைவரை uintmax_t மொத்தம்;

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
#define REPEAT_மொத்தம்_MAXIMUM BEGIN_STATE

/* The following (but not CC_NO_CLASS) are indices into the array of
   valid character class strings.  */
எண்ணிக்கை Char_class
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
எண்ணிக்கை Upper_Lower_class
  {
    UL_LOWER,
    UL_UPPER,
    UL_NONE
  };

/* The type of a List_element.  See build_spec_list for more details.  */
எண்ணிக்கை Range_element_type
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
கட்டமைப்பு List_element
  {
    எண்ணிக்கை Range_element_type type;
    கட்டமைப்பு List_element *next;
    ஒன்றிப்பு
      {
        unsigned எழுத்து normal_char;
        கட்டமைப்பு			/* unnamed */
          {
            unsigned எழுத்து first_char;
            unsigned எழுத்து last_char;
          }
        range;
        எண்ணிக்கை Char_class char_class;
        unsigned எழுத்து equiv_code;
        கட்டமைப்பு			/* unnamed */
          {
            unsigned எழுத்து the_repeated_char;
            மொத்தம் repeat_மொத்தம்;
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
கட்டமைப்பு Spec_list
  {
    /* Points to the head of the list of range elements.
       The first struct is a dummy; its members are never used.  */
    கட்டமைப்பு List_element *head;

    /* When appending, points to the last element.  When traversing via
       get_next(), points to the element to process next.  Setting
       Spec_list.state to the value BEGIN_STATE before calling get_next
       signals get_next to initialize tail to point to head->next.  */
    கட்டமைப்பு List_element *tail;

    /* Used to save state between calls to get_next.  */
    மொத்தம் state;

    /* Length, in the sense that length ('a-z[:digit:]123abc')
       is 42 ( = 26 + 10 + 6).  */
    மொத்தம் length;

    /* The number of [c*] and [c*0] constructs that appear in this spec.  */
    size_t n_indefinite_repeats;

    /* If n_indefinite_repeats is nonzero, this points to the List_element
       corresponding to the last [c*] or [c*0] construct encountered in
       this spec.  Otherwise it is undefined.  */
    கட்டமைப்பு List_element *indefinite_repeat_element;

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
கட்டமைப்பு E_string
{
  எழுத்து *s;
  bool *escaped;
  size_t len;
};

/* Return nonzero if the Ith character of escaped string ES matches C
   and is not escaped itself.  */
நிலையான உள்வரி bool
es_match (கட்டமைப்பு E_string மாறிலி *es, size_t i, எழுத்து c)
{
  திரும்பு es->s[i] == c && !es->escaped[i];
}

/* When true, each sequence in the input of a repeated character
   (call it c) is replaced (in the output) by a single occurrence of c
   for every c in the squeeze set.  */
நிலையான bool squeeze_repeats = false;

/* When true, removes characters in the delete set from input.  */
நிலையான bool delete = false;

/* Use the complement of set1 in place of set1.  */
நிலையான bool complement = false;

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
நிலையான bool truncate_set1 = false;

/* An alias for (!delete && non_option_args == 2).
   It is set in main and used there and in validate().  */
நிலையான bool translating;

நிலையான எழுத்து io_buf[BUFSIZ];

நிலையான எழுத்து மாறிலி *மாறிலி char_class_name[] =
{
  "alnum", "alpha", "blank", "cntrl", "digit", "graph",
  "lower", "print", "punct", "space", "upper", "xdigit"
};

/* Array of boolean values.  A character 'c' is a member of the
   squeeze set if and only if in_squeeze_set[c] is true.  The squeeze
   set is defined by the last (possibly, the only) string argument
   on the command line when the squeeze option is given.  */
நிலையான bool in_squeeze_set[N_CHARS];

/* Array of boolean values.  A character 'c' is a member of the
   delete set if and only if in_delete_set[c] is true.  The delete
   set is defined by the first (or only) string argument on the
   command line when the delete option is given.  */
நிலையான bool in_delete_set[N_CHARS];

/* Array of character values defining the translation (if any) that
   tr is to perform.  Translation is performed only when there are
   two specification strings and the delete switch is not given.  */
நிலையான எழுத்து xlate[N_CHARS];

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"complement", no_argument, வெற்று, 'c'},
  {"delete", no_argument, வெற்று, 'd'},
  {"squeeze-repeats", no_argument, வெற்று, 's'},
  {"truncate-set1", no_argument, வெற்று, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... STRING1 [STRING2]\n\
"),
              program_name);
      fputs (_("\
Translate, squeeze, and/or delete characters from standard உள்ளீடு,\n\
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
         delete characters in ARRAY1, செய் not translate\n\
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
  \\NNN            character with octal மதிப்பு NNN (1 to 3 octal digits)\n\
  \\\\              backslash\n\
  \\a              audible BEL\n\
  \\b              backspace\n\
  \\f              form feed\n\
  \\n              new வரி\n\
  \\r              திரும்பு\n\
  \\t              horizontal tab\n\
"), stdout);
     fputs (_("\
  \\v              vertical tab\n\
  CHAR1-CHAR2     all characters from CHAR1 to CHAR2 in ascending order\n\
  [CHAR*]         in ARRAY2, copies of CHAR until length of ARRAY1\n\
  [CHAR*REPEAT]   REPEAT copies of CHAR, REPEAT octal எனில் starting with 0\n\
  [:alnum:]       all letters and digits\n\
  [:alpha:]       all letters\n\
  [:blank:]       all horizontal whitespace\n\
  [:cntrl:]       all control characters\n\
  [:digit:]       all digits\n\
"), stdout);
     fputs (_("\
  [:graph:]       all printable characters, not including space\n\
  [:lower:]       all lower நிலை letters\n\
  [:print:]       all printable characters, including space\n\
  [:punct:]       all punctuation characters\n\
  [:space:]       all horizontal or vertical whitespace\n\
  [:upper:]       all upper நிலை letters\n\
  [:xdigit:]      all hexadecimal digits\n\
  [=CHAR=]        all characters which are equivalent to CHAR\n\
"), stdout);
     fputs (_("\
\n\
Translation occurs எனில் -d is not given and both STRING1 and STRING2 appear.\n\
-t is only significant when translating.  ARRAY2 is extended to length of\n\
ARRAY1 by repeating its last character as necessary.  Excess characters\n\
of ARRAY2 are ignored.  Character classes expand in unspecified order;\n\
போது translating, '[:lower:]' and '[:upper:]' may be used in pairs to\n\
specify நிலை conversion.  Squeezing occurs after translation or deletion.\n\
Arguments like '[...]' should be quoted, to avoid potential shell globbing.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Return nonzero if the character C is a member of the
   equivalence class containing the character EQUIV_CLASS.  */

நிலையான உள்வரி bool
is_equiv_class_member (unsigned எழுத்து equiv_class, unsigned எழுத்து c)
{
  திரும்பு (equiv_class == c);
}

/* Return true if the character C is a member of the
   character class CHAR_CLASS.  */

ATTRIBUTE_PURE
நிலையான bool
is_char_class_member (எண்ணிக்கை Char_class char_class, unsigned எழுத்து c)
{
  முழு result;

  தேர்வு (char_class)
    {
    நிலை CC_ALNUM:
      result = isalnum (c);
      நிறுத்து;
    நிலை CC_ALPHA:
      result = isalpha (c);
      நிறுத்து;
    நிலை CC_BLANK:
      result = isblank (c);
      நிறுத்து;
    நிலை CC_CNTRL:
      result = iscntrl (c);
      நிறுத்து;
    நிலை CC_DIGIT:
      result = c_isdigit (c);
      நிறுத்து;
    நிலை CC_GRAPH:
      result = isgraph (c);
      நிறுத்து;
    நிலை CC_LOWER:
      result = islower (c);
      நிறுத்து;
    நிலை CC_PRINT:
      result = isprint (c);
      நிறுத்து;
    நிலை CC_PUNCT:
      result = ispunct (c);
      நிறுத்து;
    நிலை CC_SPACE:
      result = isspace (c);
      நிறுத்து;
    நிலை CC_UPPER:
      result = isupper (c);
      நிறுத்து;
    நிலை CC_XDIGIT:
      result = c_isxdigit (c);
      நிறுத்து;
    நிலை CC_NO_CLASS: இயல்பு:
      affirm (false);
    }

  திரும்பு !! result;
}

நிலையான வெறுமை
es_free (கட்டமைப்பு E_string *es)
{
  விடுவி (es->s);
  விடுவி (es->escaped);
}

/* Perform the first pass over each range-spec argument S, converting all
   \c and \ddd escapes to their one-byte representations.  If an invalid
   quote sequence is found print an error message and return false;
   Otherwise set *ES to the resulting string and return true.
   The resulting array of characters may contain zero-bytes;
   however, on input, S is assumed to be null-terminated, and hence
   cannot contain actual (non-escaped) zero bytes.  */

நிலையான bool
unquote (எழுத்து மாறிலி *s, கட்டமைப்பு E_string *es)
{
  size_t len = strlen (s);

  es->s = xmalloc (len);
  es->escaped = xcalloc (len, அளவு es->escaped[0]);

  unsigned முழு j = 0;
  க்கு (unsigned முழு i = 0; s[i]; i++)
    {
      unsigned எழுத்து c;
      முழு oct_digit;

      தேர்வு (s[i])
        {
        நிலை '\\':
          es->escaped[j] = true;
          தேர்வு (s[i + 1])
            {
            நிலை '\\':
              c = '\\';
              நிறுத்து;
            நிலை 'a':
              c = '\a';
              நிறுத்து;
            நிலை 'b':
              c = '\b';
              நிறுத்து;
            நிலை 'f':
              c = '\f';
              நிறுத்து;
            நிலை 'n':
              c = '\n';
              நிறுத்து;
            நிலை 'r':
              c = '\r';
              நிறுத்து;
            நிலை 't':
              c = '\t';
              நிறுத்து;
            நிலை 'v':
              c = '\v';
              நிறுத்து;
            நிலை '0':
            நிலை '1':
            நிலை '2':
            நிலை '3':
            நிலை '4':
            நிலை '5':
            நிலை '6':
            நிலை '7':
              c = s[i + 1] - '0';
              oct_digit = s[i + 2] - '0';
              எனில் (0 <= oct_digit && oct_digit <= 7)
                {
                  c = 8 * c + oct_digit;
                  ++i;
                  oct_digit = s[i + 2] - '0';
                  எனில் (0 <= oct_digit && oct_digit <= 7)
                    {
                      எனில் (8 * c + oct_digit < N_CHARS)
                        {
                          c = 8 * c + oct_digit;
                          ++i;
                        }
                      இல்லையெனில்
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
              நிறுத்து;
            நிலை '\0':
              error (0, 0, _("warning: an unescaped backslash "
                             "at end of string is not portable"));
              /* POSIX is not clear about this.  */
              es->escaped[j] = false;
              i--;
              c = '\\';
              நிறுத்து;
            இயல்பு:
              c = s[i + 1];
              நிறுத்து;
            }
          ++i;
          es->s[j++] = c;
          நிறுத்து;
        இயல்பு:
          es->s[j++] = s[i];
          நிறுத்து;
        }
    }
  es->len = j;
  திரும்பு true;
}

/* If CLASS_STR is a valid character class string, return its index
   in the global char_class_name array.  Otherwise, return CC_NO_CLASS.  */

ATTRIBUTE_PURE
நிலையான எண்ணிக்கை Char_class
look_up_char_class (எழுத்து மாறிலி *class_str, size_t len)
{
  க்கு (எண்ணிக்கை Char_class i = 0; i < countof (char_class_name); i++)
    எனில் (STREQ_LEN (class_str, char_class_name[i], len)
        && strlen (char_class_name[i]) == len)
      திரும்பு i;
  திரும்பு CC_NO_CLASS;
}

/* Return a newly allocated string with a printable version of C.
   This function is used solely for formatting error messages.  */

நிலையான எழுத்து *
make_printable_char (unsigned எழுத்து c)
{
  எழுத்து *buf = xmalloc (5);

  எனில் (isprint (c))
    {
      buf[0] = c;
      buf[1] = '\0';
    }
  இல்லையெனில்
    {
      sprintf (buf, "\\%03o", c);
    }
  திரும்பு buf;
}

/* Return a newly allocated copy of S which is suitable for printing.
   LEN is the number of characters in S.  Most non-printing
   (isprint) characters are represented by a backslash followed by
   3 octal digits.  However, the characters represented by \c escapes
   where c is one of [abfnrtv] are represented by their 2-character \c
   sequences.  This function is used solely for printing error messages.  */

நிலையான எழுத்து *
make_printable_str (எழுத்து மாறிலி *s, size_t len)
{
  /* Worst case is that every character expands to a backslash
     followed by a 3-character octal escape sequence.  */
  எழுத்து *printable_buf = xnmalloc (len + 1, 4);
  எழுத்து *p = printable_buf;

  க்கு (size_t i = 0; i < len; i++)
    {
      எழுத்து buf[5];
      எழுத்து மாறிலி *tmp = வெற்று;
      unsigned எழுத்து c = s[i];

      தேர்வு (c)
        {
        நிலை '\\':
          tmp = "\\";
          நிறுத்து;
        நிலை '\a':
          tmp = "\\a";
          நிறுத்து;
        நிலை '\b':
          tmp = "\\b";
          நிறுத்து;
        நிலை '\f':
          tmp = "\\f";
          நிறுத்து;
        நிலை '\n':
          tmp = "\\n";
          நிறுத்து;
        நிலை '\r':
          tmp = "\\r";
          நிறுத்து;
        நிலை '\t':
          tmp = "\\t";
          நிறுத்து;
        நிலை '\v':
          tmp = "\\v";
          நிறுத்து;
        இயல்பு:
          எனில் (isprint (c))
            {
              buf[0] = c;
              buf[1] = '\0';
            }
          இல்லையெனில்
            sprintf (buf, "\\%03o", c);
          tmp = buf;
          நிறுத்து;
        }
      p = stpcpy (p, tmp);
    }
  திரும்பு printable_buf;
}

/* Append a newly allocated structure representing a
   character C to the specification list LIST.  */

நிலையான வெறுமை
append_normal_char (கட்டமைப்பு Spec_list *list, unsigned எழுத்து c)
{
  கட்டமைப்பு List_element *new = xmalloc (அளவு *new);
  new->next = வெற்று;
  new->type = RE_NORMAL_CHAR;
  new->u.normal_char = c;
  list->tail->next = new;
  list->tail = new;
}

/* Append a newly allocated structure representing the range
   of characters from FIRST to LAST to the specification list LIST.
   Return false if LAST precedes FIRST in the collating sequence,
   true otherwise.  This means that '[c-c]' is acceptable.  */

நிலையான bool
append_range (கட்டமைப்பு Spec_list *list, unsigned எழுத்து first, unsigned எழுத்து last)
{
  எனில் (last < first)
    {
      எழுத்து *tmp1 = make_printable_char (first);
      எழுத்து *tmp2 = make_printable_char (last);

      error (0, 0,
       _("range-endpoints of '%s-%s' are in reverse collating sequence order"),
             tmp1, tmp2);
      விடுவி (tmp1);
      விடுவி (tmp2);
      திரும்பு false;
    }
  கட்டமைப்பு List_element *new = xmalloc (அளவு *new);
  new->next = வெற்று;
  new->type = RE_RANGE;
  new->u.range.first_char = first;
  new->u.range.last_char = last;
  list->tail->next = new;
  list->tail = new;
  திரும்பு true;
}

/* If CHAR_CLASS_STR is a valid character class string, append a
   newly allocated structure representing that character class to the end
   of the specification list LIST and return true.  If CHAR_CLASS_STR is not
   a valid string return false.  */

நிலையான bool
append_char_class (கட்டமைப்பு Spec_list *list,
                   எழுத்து மாறிலி *char_class_str, size_t len)
{
  எண்ணிக்கை Char_class char_class = look_up_char_class (char_class_str, len);
  எனில் (char_class == CC_NO_CLASS)
    திரும்பு false;
  கட்டமைப்பு List_element *new = xmalloc (அளவு *new);
  new->next = வெற்று;
  new->type = RE_CHAR_CLASS;
  new->u.char_class = char_class;
  list->tail->next = new;
  list->tail = new;
  திரும்பு true;
}

/* Append a newly allocated structure representing a [c*n]
   repeated character construct to the specification list LIST.
   THE_CHAR is the single character to be repeated, and REPEAT_COUNT
   is a non-negative repeat count.  */

நிலையான வெறுமை
append_repeated_char (கட்டமைப்பு Spec_list *list, unsigned எழுத்து the_char,
                      மொத்தம் repeat_மொத்தம்)
{
  கட்டமைப்பு List_element *new = xmalloc (அளவு *new);
  new->next = வெற்று;
  new->type = RE_REPEATED_CHAR;
  new->u.repeated_char.the_repeated_char = the_char;
  new->u.repeated_char.repeat_மொத்தம் = repeat_மொத்தம்;
  list->tail->next = new;
  list->tail = new;
}

/* Given a string, EQUIV_CLASS_STR, from a [=str=] context and
   the length of that string, LEN, if LEN is exactly one, append
   a newly allocated structure representing the specified
   equivalence class to the specification list, LIST and return true.
   If LEN is not 1, return false.  */

நிலையான bool
append_equiv_class (கட்டமைப்பு Spec_list *list,
                    எழுத்து மாறிலி *equiv_class_str, size_t len)
{
  எனில் (len != 1)
    திரும்பு false;

  கட்டமைப்பு List_element *new = xmalloc (அளவு *new);
  new->next = வெற்று;
  new->type = RE_EQUIV_CLASS;
  new->u.equiv_code = *equiv_class_str;
  list->tail->next = new;
  list->tail = new;
  திரும்பு true;
}

/* Search forward starting at START_IDX for the 2-char sequence
   (PRE_BRACKET_CHAR,']') in the string P of length P_LEN.  If such
   a sequence is found, set *RESULT_IDX to the index of the first
   character and return true.  Otherwise return false.  P may contain
   zero bytes.  */

நிலையான bool
find_closing_delim (மாறிலி கட்டமைப்பு E_string *es, size_t start_idx,
                    எழுத்து pre_bracket_char, size_t *result_idx)
{
  க்கு (size_t i = start_idx; i < es->len - 1; i++)
    எனில் (es->s[i] == pre_bracket_char && es->s[i + 1] == ']'
        && !es->escaped[i] && !es->escaped[i + 1])
      {
        *result_idx = i;
        திரும்பு true;
      }
  திரும்பு false;
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

நிலையான முழு
find_bracketed_repeat (மாறிலி கட்டமைப்பு E_string *es, size_t start_idx,
                       unsigned எழுத்து *char_to_repeat, மொத்தம் *repeat_மொத்தம்,
                       size_t *closing_bracket_idx)
{
  affirm (start_idx + 1 < es->len);
  எனில் (!es_match (es, start_idx + 1, '*'))
    திரும்பு -1;

  க்கு (size_t i = start_idx + 2; i < es->len && !es->escaped[i]; i++)
    {
      எனில் (es->s[i] == ']')
        {
          size_t digit_str_len = i - start_idx - 2;

          *char_to_repeat = es->s[start_idx];
          எனில் (digit_str_len == 0)
            {
              /* We've matched [c*] -- no explicit repeat count.  */
              *repeat_மொத்தம் = 0;
            }
          இல்லையெனில்
            {
              /* Here, we have found [c*s] where s should be a string
                 of octal (if it starts with '0') or decimal digits.  */
              எழுத்து மாறிலி *digit_str = &es->s[start_idx + 2];
              எழுத்து *d_end;
              எனில் ((xstrtoumax (digit_str, &d_end, *digit_str == '0' ? 8 : 10,
                               repeat_மொத்தம், வெற்று)
                   != LONGINT_OK)
                  || REPEAT_மொத்தம்_MAXIMUM < *repeat_மொத்தம்
                  || digit_str + digit_str_len != d_end)
                {
                  எழுத்து *tmp = make_printable_str (digit_str, digit_str_len);
                  error (0, 0,
                         _("invalid repeat count %s in [c*n] construct"),
                         quote (tmp));
                  விடுவி (tmp);
                  திரும்பு -2;
                }
            }
          *closing_bracket_idx = i;
          திரும்பு 0;
        }
    }
  திரும்பு -1;			/* No bracket found.  */
}

/* Return true if the string at ES->s[IDX] matches the regular
   expression '\*[0-9]*]', false otherwise.  The string does not
   match if any of its characters are escaped.  */

ATTRIBUTE_PURE
நிலையான bool
star_digits_closebracket (மாறிலி கட்டமைப்பு E_string *es, size_t idx)
{
  எனில் (!es_match (es, idx, '*'))
    திரும்பு false;

  க்கு (size_t i = idx + 1; i < es->len; i++)
    எனில் (!c_isdigit (es->s[i]) || es->escaped[i])
      திரும்பு es_match (es, i, ']');
  திரும்பு false;
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

நிலையான bool
build_spec_list (மாறிலி கட்டமைப்பு E_string *es, கட்டமைப்பு Spec_list *result)
{
  எழுத்து மாறிலி *p = es->s;

  /* The main for-loop below recognizes the 4 multi-character constructs.
     A character that matches (in its context) none of the multi-character
     constructs is classified as 'normal'.  Since all multi-character
     constructs have at least 3 characters, any strings of length 2 or
     less are composed solely of normal characters.  Hence, the index of
     the outer for-loop runs only as far as LEN-2.  */
  size_t i;
  க்கு (i = 0; i + 2 < es->len; /* empty */)
    {
      எனில் (es_match (es, i, '['))
        {
          bool matched_multi_char_construct;
          size_t closing_bracket_idx;
          unsigned எழுத்து char_to_repeat;
          மொத்தம் repeat_மொத்தம்;
          முழு err;

          matched_multi_char_construct = true;
          எனில் (es_match (es, i + 1, ':') || es_match (es, i + 1, '='))
            {
              size_t closing_delim_idx;

              எனில் (find_closing_delim (es, i + 2, p[i + 1], &closing_delim_idx))
                {
                  size_t opnd_str_len = closing_delim_idx - 1 - (i + 2) + 1;
                  எழுத்து மாறிலி *opnd_str = p + i + 2;

                  எனில் (opnd_str_len == 0)
                    {
                      எனில் (p[i + 1] == ':')
                        error (0, 0, _("missing character class name '[::]'"));
                      இல்லையெனில்
                        error (0, 0,
                               _("missing equivalence class character '[==]'"));
                      திரும்பு false;
                    }

                  எனில் (p[i + 1] == ':')
                    {
                      /* FIXME: big comment.  */
                      எனில் (!append_char_class (result, opnd_str, opnd_str_len))
                        {
                          எனில் (star_digits_closebracket (es, i + 2))
                            செல் try_bracketed_repeat;
                          இல்லையெனில்
                            {
                              எழுத்து *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0, _("invalid character class %s"),
                                     quote (tmp));
                              விடுவி (tmp);
                              திரும்பு false;
                            }
                        }
                    }
                  இல்லையெனில்
                    {
                      /* FIXME: big comment.  */
                      எனில் (!append_equiv_class (result, opnd_str, opnd_str_len))
                        {
                          எனில் (star_digits_closebracket (es, i + 2))
                            செல் try_bracketed_repeat;
                          இல்லையெனில்
                            {
                              எழுத்து *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0,
               _("%s: equivalence class operand must be a single character"),
                                     tmp);
                              விடுவி (tmp);
                              திரும்பு false;
                            }
                        }
                    }

                  i = closing_delim_idx + 2;
                  தொடர்;
                }
              /* Else fall through.  This could be [:*] or [=*].  */
            }

        try_bracketed_repeat:

          /* Determine whether this is a bracketed repeat range
             matching the RE \[.\*(dec_or_oct_number)?].  */
          err = find_bracketed_repeat (es, i + 1, &char_to_repeat,
                                       &repeat_மொத்தம்,
                                       &closing_bracket_idx);
          எனில் (err == 0)
            {
              append_repeated_char (result, char_to_repeat, repeat_மொத்தம்);
              i = closing_bracket_idx + 1;
            }
          இல்லையெனில் எனில் (err == -1)
            {
              matched_multi_char_construct = false;
            }
          இல்லையெனில்
            {
              /* Found a string that looked like [c*n] but the
                 numeric part was invalid.  */
              திரும்பு false;
            }

          எனில் (matched_multi_char_construct)
            தொடர்;

          /* We reach this point if P does not match [:str:], [=c=],
             [c*n], or [c*].  Now, see if P looks like a range '[-c'
             (from '[' to 'c').  */
        }

      /* Look ahead one char for ranges like a-z.  */
      எனில் (es_match (es, i + 1, '-'))
        {
          எனில் (!append_range (result, p[i], p[i + 2]))
            திரும்பு false;
          i += 3;
        }
      இல்லையெனில்
        {
          append_normal_char (result, p[i]);
          ++i;
        }
    }

  /* Now handle the (2 or fewer) remaining characters p[i]..p[es->len - 1].  */
  க்கு (; i < es->len; i++)
    append_normal_char (result, p[i]);

  திரும்பு true;
}

/* Advance past the current construct.
   S->tail must be non-null.  */
நிலையான வெறுமை
skip_construct (கட்டமைப்பு Spec_list *s)
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

நிலையான முழு
get_next (கட்டமைப்பு Spec_list *s, எண்ணிக்கை Upper_Lower_class *class)
{
  கட்டமைப்பு List_element *p;
  முழு return_val;
  முழு i;

  எனில் (class)
    *class = UL_NONE;

  எனில் (s->state == BEGIN_STATE)
    {
      s->tail = s->head->next;
      s->state = NEW_ELEMENT;
    }

  p = s->tail;
  எனில் (p == வெற்று)
    திரும்பு -1;

  தேர்வு (p->type)
    {
    நிலை RE_NORMAL_CHAR:
      return_val = p->u.normal_char;
      s->state = NEW_ELEMENT;
      s->tail = p->next;
      நிறுத்து;

    நிலை RE_RANGE:
      எனில் (s->state == NEW_ELEMENT)
        s->state = p->u.range.first_char;
      இல்லையெனில்
        ++(s->state);
      return_val = s->state;
      எனில் (s->state == p->u.range.last_char)
        {
          s->tail = p->next;
          s->state = NEW_ELEMENT;
        }
      நிறுத்து;

    நிலை RE_CHAR_CLASS:
      எனில் (class)
        {
          தேர்வு (+p->u.char_class)
            {
            நிலை CC_LOWER:
              *class = UL_LOWER;
              நிறுத்து;
            நிலை CC_UPPER:
              *class = UL_UPPER;
              நிறுத்து;
            }
        }

      எனில் (s->state == NEW_ELEMENT)
        {
          க்கு (i = 0; i < N_CHARS; i++)
            எனில் (is_char_class_member (p->u.char_class, i))
              நிறுத்து;
          affirm (i < N_CHARS);
          s->state = i;
        }
      assure (is_char_class_member (p->u.char_class, s->state));
      return_val = s->state;
      க்கு (i = s->state + 1; i < N_CHARS; i++)
        எனில் (is_char_class_member (p->u.char_class, i))
          நிறுத்து;
      எனில் (i < N_CHARS)
        s->state = i;
      இல்லையெனில்
        {
          s->tail = p->next;
          s->state = NEW_ELEMENT;
        }
      நிறுத்து;

    நிலை RE_EQUIV_CLASS:
      /* FIXME: this assumes that each character is alone in its own
         equivalence class (which appears to be correct for my
         LC_COLLATE.  But I don't know of any function that allows
         one to determine a character's equivalence class.  */

      return_val = p->u.equiv_code;
      s->state = NEW_ELEMENT;
      s->tail = p->next;
      நிறுத்து;

    நிலை RE_REPEATED_CHAR:
      /* Here, a repeat count of n == 0 means don't repeat at all.  */
      எனில் (p->u.repeated_char.repeat_மொத்தம் == 0)
        {
          s->tail = p->next;
          s->state = NEW_ELEMENT;
          return_val = get_next (s, class);
        }
      இல்லையெனில்
        {
          எனில் (s->state == NEW_ELEMENT)
            {
              s->state = 0;
            }
          ++(s->state);
          return_val = p->u.repeated_char.the_repeated_char;
          எனில் (s->state == p->u.repeated_char.repeat_மொத்தம்)
            {
              s->tail = p->next;
              s->state = NEW_ELEMENT;
            }
        }
      நிறுத்து;

    இயல்பு:
      unreachable ();
    }

  திரும்பு return_val;
}

/* This is a minor kludge.  This function is called from
   get_spec_stats to determine the cardinality of a set derived
   from a complemented string.  It's a kludge in that some of the
   same operations are (duplicated) performed in set_initialize.  */

நிலையான முழு
card_of_complement (கட்டமைப்பு Spec_list *s)
{
  முழு c;
  முழு cardinality = N_CHARS;
  bool in_set[N_CHARS] = {0};

  s->state = BEGIN_STATE;
  போது ((c = get_next (s, வெற்று)) != -1)
    {
      cardinality -= (!in_set[c]);
      in_set[c] = true;
    }
  திரும்பு cardinality;
}

/* Discard the lengths associated with a case conversion,
   as using the actual number of upper or lower case characters
   is problematic when they don't match in some locales.
   Also ensure the case conversion classes in string2 are
   aligned correctly with those in string1.
   Note POSIX says the behavior of 'tr "[:upper:]" "[:upper:]"'
   is undefined.  Therefore we allow it (unlike Solaris)
   and treat it as a no-op.  */

நிலையான வெறுமை
validate_case_classes (கட்டமைப்பு Spec_list *s1, கட்டமைப்பு Spec_list *s2)
{
  size_t n_upper = 0;
  size_t n_lower = 0;
  முழு c1 = 0;
  முழு c2 = 0;
  MAYBE_UNUSED மொத்தம் old_s1_len = s1->length, old_s2_len = s2->length;
  கட்டமைப்பு List_element *s1_tail = s1->tail;
  கட்டமைப்பு List_element *s2_tail = s2->tail;
  bool s1_new_element = true;
  bool s2_new_element = true;

  எனில் (complement || !s2->has_char_class)
    திரும்பு;

  க்கு (முழு i = 0; i < N_CHARS; i++)
    {
      எனில் (isupper (i))
        n_upper++;
      எனில் (islower (i))
        n_lower++;
    }

  s1->state = BEGIN_STATE;
  s2->state = BEGIN_STATE;

  போது (c1 != -1 && c2 != -1)
    {
      எண்ணிக்கை Upper_Lower_class class_s1, class_s2;

      c1 = get_next (s1, &class_s1);
      c2 = get_next (s2, &class_s2);

      /* If c2 transitions to a new case class, then
         c1 must also transition at the same time.  */
      எனில் (s2_new_element && class_s2 != UL_NONE
          && !(s1_new_element && class_s1 != UL_NONE))
        error (EXIT_FAILURE, 0,
               _("misaligned [:upper:] and/or [:lower:] construct"));

      /* If case converting, quickly skip over the elements.  */
      எனில் (class_s2 != UL_NONE)
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

நிலையான வெறுமை
get_spec_stats (கட்டமைப்பு Spec_list *s)
{
  மொத்தம் length = 0;

  s->n_indefinite_repeats = 0;
  s->has_equiv_class = false;
  s->has_restricted_char_class = false;
  s->has_char_class = false;
  க்கு (கட்டமைப்பு List_element *p = s->head->next; p; p = p->next)
    {
      மொத்தம் len = 0;

      தேர்வு (p->type)
        {
        நிலை RE_NORMAL_CHAR:
          len = 1;
          நிறுத்து;

        நிலை RE_RANGE:
          affirm (p->u.range.last_char >= p->u.range.first_char);
          len = p->u.range.last_char - p->u.range.first_char + 1;
          நிறுத்து;

        நிலை RE_CHAR_CLASS:
          s->has_char_class = true;
          க்கு (முழு i = 0; i < N_CHARS; i++)
            எனில் (is_char_class_member (p->u.char_class, i))
              ++len;
          தேர்வு (+p->u.char_class)
            {
            நிலை CC_UPPER:
            நிலை CC_LOWER:
              நிறுத்து;
            இயல்பு:
              s->has_restricted_char_class = true;
              நிறுத்து;
            }
          நிறுத்து;

        நிலை RE_EQUIV_CLASS:
          க்கு (முழு i = 0; i < N_CHARS; i++)
            எனில் (is_equiv_class_member (p->u.equiv_code, i))
              ++len;
          s->has_equiv_class = true;
          நிறுத்து;

        நிலை RE_REPEATED_CHAR:
          எனில் (p->u.repeated_char.repeat_மொத்தம் > 0)
            len = p->u.repeated_char.repeat_மொத்தம்;
          இல்லையெனில்
            {
              s->indefinite_repeat_element = p;
              ++(s->n_indefinite_repeats);
            }
          நிறுத்து;

        இயல்பு:
          unreachable ();
        }

      /* Check for arithmetic overflow in computing length.  Also, reject
         any length greater than the maximum repeat count, in case the
         length is later used to compute the repeat count for an
         indefinite element.  */
      எனில் (ckd_add (&length, length, len) || REPEAT_மொத்தம்_MAXIMUM < length)
        error (EXIT_FAILURE, 0, _("too many characters in set"));
    }

  s->length = length;
}

நிலையான வெறுமை
get_s1_spec_stats (கட்டமைப்பு Spec_list *s1)
{
  get_spec_stats (s1);
  எனில் (complement)
    s1->length = card_of_complement (s1);
}

நிலையான வெறுமை
get_s2_spec_stats (கட்டமைப்பு Spec_list *s2, மொத்தம் len_s1)
{
  get_spec_stats (s2);
  எனில் (len_s1 >= s2->length && s2->n_indefinite_repeats == 1)
    {
      s2->indefinite_repeat_element->u.repeated_char.repeat_மொத்தம் =
        len_s1 - s2->length;
      s2->length = len_s1;
    }
}

நிலையான வெறுமை
spec_init (கட்டமைப்பு Spec_list *spec_list)
{
  கட்டமைப்பு List_element *new = xmalloc (அளவு *new);
  spec_list->head = spec_list->tail = new;
  spec_list->head->next = வெற்று;
}

/* This function makes two passes over the argument string S.  The first
   one converts all \c and \ddd escapes to their one-byte representations.
   The second constructs a linked specification list, SPEC_LIST, of the
   characters and constructs that comprise the argument string.  If either
   of these passes detects an error, this function returns false.  */

நிலையான bool
parse_str (எழுத்து மாறிலி *s, கட்டமைப்பு Spec_list *spec_list)
{
  கட்டமைப்பு E_string es;
  bool ok = unquote (s, &es) && build_spec_list (&es, spec_list);
  es_free (&es);
  திரும்பு ok;
}

/* Given two specification lists, S1 and S2, and assuming that
   S1->length > S2->length, append a single [c*n] element to S2 where c
   is the last character in the expansion of S2 and n is the difference
   between the two lengths.
   Upon successful completion, S2->length is set to S1->length.  The only
   way this function can fail to make S2 as long as S1 is when S2 has
   zero-length, since in that case, there is no last character to repeat.
   So S2->length is required to be at least 1.  */

நிலையான வெறுமை
string2_extend (மாறிலி கட்டமைப்பு Spec_list *s1, கட்டமைப்பு Spec_list *s2)
{
  கட்டமைப்பு List_element *p;
  unsigned எழுத்து char_to_repeat;

  affirm (translating);
  affirm (s1->length > s2->length);
  affirm (s2->length > 0);

  p = s2->tail;
  தேர்வு (p->type)
    {
    நிலை RE_NORMAL_CHAR:
      char_to_repeat = p->u.normal_char;
      நிறுத்து;
    நிலை RE_RANGE:
      char_to_repeat = p->u.range.last_char;
      நிறுத்து;
    நிலை RE_CHAR_CLASS:
      /* Note BSD allows extending of classes in string2.  For example:
           tr '[:upper:]0-9' '[:lower:]'
         That's not portable however, contradicts POSIX and is dependent
         on your collating sequence.  */
      error (EXIT_FAILURE, 0,
             _("when translating with string1 longer than string2,\n"
               "the latter string must not end with a character class"));

    நிலை RE_REPEATED_CHAR:
      char_to_repeat = p->u.repeated_char.the_repeated_char;
      நிறுத்து;

    நிலை RE_EQUIV_CLASS: இயல்பு:
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

நிலையான bool
homogeneous_spec_list (கட்டமைப்பு Spec_list *s)
{
  முழு b, c;

  s->state = BEGIN_STATE;

  எனில் ((b = get_next (s, வெற்று)) == -1)
    திரும்பு false;

  போது ((c = get_next (s, வெற்று)) != -1)
    எனில் (c != b)
      திரும்பு false;

  திரும்பு true;
}

/* Die with an error message if S1 and S2 describe strings that
   are not valid with the given command line switches.
   A side effect of this function is that if a valid [c*] or
   [c*0] construct appears in string2, it is converted to [c*n]
   with a value for n that makes s2->length == s1->length.  By
   the same token, if the --truncate-set1 option is not
   given, S2 may be extended.  */

நிலையான வெறுமை
validate (கட்டமைப்பு Spec_list *s1, கட்டமைப்பு Spec_list *s2)
{
  get_s1_spec_stats (s1);
  எனில் (s1->n_indefinite_repeats > 0)
    error (EXIT_FAILURE, 0,
           _("the [c*] repeat construct may not appear in string1"));

  எனில் (s2)
    {
      get_s2_spec_stats (s2, s1->length);

      எனில் (s2->n_indefinite_repeats > 1)
        error (EXIT_FAILURE, 0,
               _("only one [c*] repeat construct may appear in string2"));

      எனில் (translating)
        {
          எனில் (s2->has_equiv_class)
            error (EXIT_FAILURE, 0,
                   _("[=c=] expressions may not appear in string2"
                     " when translating"));

          எனில் (s2->has_restricted_char_class)
            error (EXIT_FAILURE, 0,
                   _("when translating, the only character classes"
                     " that may appear in\n"
                     "string2 are 'upper' and 'lower'"));

          validate_case_classes (s1, s2);

          எனில் (s1->length > s2->length)
            {
              எனில் (!truncate_set1)
                {
                  /* string2 must be non-empty unless --truncate-set1 is
                     given or string1 is empty.  */

                  எனில் (s2->length == 0)
                    error (EXIT_FAILURE, 0,
                           _("when not truncating set1,"
                             " string2 must be non-empty"));
                  string2_extend (s1, s2);
                }
            }

          எனில் (complement && s1->has_char_class
              && ! (s2->length == s1->length && homogeneous_spec_list (s2)))
            error (EXIT_FAILURE, 0,
                   _("when translating with complemented character classes,\n"
                     "string2 must map all characters in the domain to one"));
        }
      இல்லையெனில்
        /* Not translating.  */
        {
          எனில் (s2->n_indefinite_repeats > 0)
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

நிலையான வெறுமை
squeeze_filter (எழுத்து *buf, size_t size, size_t (*reader) (எழுத்து *, size_t))
{
  /* A value distinct from any character that may have been stored in a
     buffer as the result of a block-read in the function squeeze_filter.  */
  மாறிலி முழு NOT_A_CHAR = INT_MAX;

  முழு char_to_squeeze = NOT_A_CHAR;
  size_t i = 0;
  size_t nr = 0;

  போது (true)
    {
      எனில் (i >= nr)
        {
          nr = reader (buf, size);
          எனில் (nr == 0)
            நிறுத்து;
          i = 0;
        }

      size_t begin = i;

      எனில் (char_to_squeeze == NOT_A_CHAR)
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
          க்கு (; i < nr && !in_squeeze_set[to_uchar (buf[i])]; i += 2)
            தொடர்;

          /* There is a special case when i == nr and we've just
             skipped a character (the last one in buf) that is in
             the squeeze set.  */
          எனில் (i == nr && in_squeeze_set[to_uchar (buf[i - 1])])
            --i;

          எனில் (i >= nr)
            out_len = nr - begin;
          இல்லையெனில்
            {
              char_to_squeeze = buf[i];
              /* We're about to output buf[begin..i].  */
              out_len = i - begin + 1;

              /* But since we stepped by 2 in the loop above,
                 out_len may be one too large.  */
              எனில் (i > 0 && buf[i - 1] == char_to_squeeze)
                --out_len;

              /* Advance i to the index of first character to be
                 considered when looking for a char different from
                 char_to_squeeze.  */
              ++i;
            }
          எனில் (out_len > 0
              && fwrite (&buf[begin], 1, out_len, stdout) != out_len)
            write_error ();
        }

      எனில் (char_to_squeeze != NOT_A_CHAR)
        {
          /* Advance i to index of first char != char_to_squeeze
             (or to nr if all the rest of the characters in this
             buffer are the same as char_to_squeeze).  */
          க்கு (; i < nr && buf[i] == char_to_squeeze; i++)
            தொடர்;
          எனில் (i < nr)
            char_to_squeeze = NOT_A_CHAR;
          /* If (i >= nr) we've squeezed the last character in this buffer.
             So now we have to read a new buffer and continue comparing
             characters against char_to_squeeze.  */
        }
    }
}

நிலையான size_t
plain_read (எழுத்து *buf, size_t size)
{
  ssize_t nr = பட (STDIN_FILENO, buf, size);
  எனில் (nr < 0)
    error (EXIT_FAILURE, errno, _("read error"));
  திரும்பு nr;
}

/* Read buffers of SIZE bytes from stdin until one is found that
   contains at least one character not in the delete set.  Store
   in the array BUF, all characters from that buffer that are not
   in the delete set, and return the number of characters saved
   or 0 upon EOF.  */

நிலையான size_t
read_and_delete (எழுத்து *buf, size_t size)
{
  size_t n_saved;

  /* This enclosing do-while loop is to make sure that
     we don't return zero (indicating EOF) when we've
     just deleted all the characters in a buffer.  */
  செய்
    {
      size_t nr = plain_read (buf, size);

      எனில் (nr == 0)
        திரும்பு 0;

      /* This first loop may be a waste of code, but gives much
         better performance when no characters are deleted in
         the beginning of a buffer.  It just avoids the copying
         of buf[i] into buf[n_saved] when it would be a NOP.  */

      size_t i;
      க்கு (i = 0; i < nr && !in_delete_set[to_uchar (buf[i])]; i++)
        தொடர்;
      n_saved = i;

      க்கு (++i; i < nr; i++)
        எனில் (!in_delete_set[to_uchar (buf[i])])
          buf[n_saved++] = buf[i];
    }
  போது (n_saved == 0);

  திரும்பு n_saved;
}

/* Read at most SIZE bytes from stdin into the array BUF.  Then
   perform the in-place and one-to-one mapping specified by the global
   array 'xlate'.  Return the number of characters read, or 0 upon EOF.  */

நிலையான size_t
read_and_xlate (எழுத்து *buf, size_t size)
{
  size_t bytes_read = plain_read (buf, size);

  க்கு (size_t i = 0; i < bytes_read; i++)
    buf[i] = xlate[to_uchar (buf[i])];

  திரும்பு bytes_read;
}

/* Initialize a boolean membership set, IN_SET, with the character
   values obtained by traversing the linked list of constructs S
   using the function 'get_next'.  IN_SET is expected to have been
   initialized to all zeros by the caller.  If COMPLEMENT_THIS_SET
   is true the resulting set is complemented.  */

நிலையான வெறுமை
set_initialize (கட்டமைப்பு Spec_list *s, bool complement_this_set, bool *in_set)
{
  முழு c;

  s->state = BEGIN_STATE;
  போது ((c = get_next (s, வெற்று)) != -1)
    in_set[c] = true;
  எனில் (complement_this_set)
    க்கு (size_t i = 0; i < N_CHARS; i++)
      in_set[i] = (!in_set[i]);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு c;
  முழு non_option_args;
  முழு min_operands;
  முழு max_operands;
  கட்டமைப்பு Spec_list buf1, buf2;
  கட்டமைப்பு Spec_list *s1 = &buf1;
  கட்டமைப்பு Spec_list *s2 = &buf2;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((c = getopt_long (argc, argv, "+AcCdstu", long_options, வெற்று)) != -1)
    {
      தேர்வு (c)
        {
        நிலை 'A':
          /* Undocumented option, for compatibility with AIX.  */
          setlocale (LC_COLLATE, "C");
          setlocale (LC_CTYPE, "C");
          நிறுத்து;

        நிலை 'c':
        நிலை 'C':
          complement = true;
          நிறுத்து;

        நிலை 'd':
          delete = true;
          நிறுத்து;

        நிலை 's':
          squeeze_repeats = true;
          நிறுத்து;

        நிலை 't':
          truncate_set1 = true;
          நிறுத்து;

        நிலை 'u':
          /* Undocumented option, for compatibility with FreeBSD/macOS.  */
          setvbuf (stdout, வெற்று, _IONBF, 0);
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
          நிறுத்து;
        }
    }

  non_option_args = argc - optind;
  translating = (non_option_args == 2 && !delete);
  min_operands = 1 + (delete == squeeze_repeats);
  max_operands = 1 + (delete <= squeeze_repeats);

  எனில் (non_option_args < min_operands)
    {
      எனில் (non_option_args == 0)
        error (0, 0, _("missing operand"));
      இல்லையெனில்
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

  எனில் (max_operands < non_option_args)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + max_operands]));
      எனில் (non_option_args == 2)
        fprintf (stderr, "%s\n",
                 _("Only one string may be given when "
                   "deleting without squeezing repeats."));
      usage (EXIT_FAILURE);
    }

  spec_init (s1);
  எனில் (!parse_str (argv[optind], s1))
    main_exit (EXIT_FAILURE);

  எனில் (non_option_args == 2)
    {
      spec_init (s2);
      எனில் (!parse_str (argv[optind + 1], s2))
        main_exit (EXIT_FAILURE);
    }
  இல்லையெனில்
    s2 = வெற்று;

  validate (s1, s2);

  /* Use binary I/O, since 'tr' is sometimes used to transliterate
     non-printable characters, or characters which are stripped away
     by text-mode reads (like CR and ^Z).  */
  xset_இருமம்_mode (STDIN_FILENO, O_இருமம்);
  xset_இருமம்_mode (STDOUT_FILENO, O_இருமம்);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  எனில் (squeeze_repeats && non_option_args == 1)
    {
      set_initialize (s1, complement, in_squeeze_set);
      squeeze_filter (io_buf, அளவு io_buf, plain_read);
    }
  இல்லையெனில் எனில் (delete && non_option_args == 1)
    {
      set_initialize (s1, complement, in_delete_set);

      போது (true)
        {
          size_t nr = read_and_delete (io_buf, அளவு io_buf);
          எனில் (nr == 0)
            நிறுத்து;
          எனில் (fwrite (io_buf, 1, nr, stdout) != nr)
            write_error ();
        }
    }
  இல்லையெனில் எனில் (squeeze_repeats && delete && non_option_args == 2)
    {
      set_initialize (s1, complement, in_delete_set);
      set_initialize (s2, false, in_squeeze_set);
      squeeze_filter (io_buf, அளவு io_buf, read_and_delete);
    }
  இல்லையெனில் எனில் (translating)
    {
      எனில் (complement)
        {
          bool *in_s1 = in_delete_set;

          set_initialize (s1, false, in_s1);
          s2->state = BEGIN_STATE;
          க்கு (முழு i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          க்கு (முழு i = 0; i < N_CHARS; i++)
            {
              எனில் (!in_s1[i])
                {
                  முழு ch = get_next (s2, வெற்று);
                  affirm (ch != -1 || truncate_set1);
                  எனில் (ch == -1)
                    {
                      /* This will happen when tr is invoked like e.g.
                         tr -cs A-Za-z0-9 '\012'.  */
                      நிறுத்து;
                    }
                  xlate[i] = ch;
                }
            }
        }
      இல்லையெனில்
        {
          முழு c1, c2;
          எண்ணிக்கை Upper_Lower_class class_s1;
          எண்ணிக்கை Upper_Lower_class class_s2;

          க்கு (முழு i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          s1->state = BEGIN_STATE;
          s2->state = BEGIN_STATE;
          போது (true)
            {
              c1 = get_next (s1, &class_s1);
              c2 = get_next (s2, &class_s2);

              எனில் (class_s1 == UL_LOWER && class_s2 == UL_UPPER)
                {
                  க்கு (முழு i = 0; i < N_CHARS; i++)
                    எனில் (islower (i))
                      xlate[i] = toupper (i);
                }
              இல்லையெனில் எனில் (class_s1 == UL_UPPER && class_s2 == UL_LOWER)
                {
                  க்கு (முழு i = 0; i < N_CHARS; i++)
                    எனில் (isupper (i))
                      xlate[i] = tolower (i);
                }
              இல்லையெனில்
                {
                  /* The following should have been checked by validate...  */
                  எனில் (c1 == -1 || c2 == -1)
                    நிறுத்து;
                  xlate[c1] = c2;
                }

              /* When case-converting, skip the elements as an optimization.  */
              எனில் (class_s2 != UL_NONE)
                {
                  skip_construct (s1);
                  skip_construct (s2);
                }
            }
          affirm (c1 == -1 || truncate_set1);
        }
      எனில் (squeeze_repeats)
        {
          set_initialize (s2, false, in_squeeze_set);
          squeeze_filter (io_buf, அளவு io_buf, read_and_xlate);
        }
      இல்லையெனில்
        {
          போது (true)
            {
              size_t bytes_read = read_and_xlate (io_buf, அளவு io_buf);
              எனில் (bytes_read == 0)
                நிறுத்து;
              எனில் (fwrite (io_buf, 1, bytes_read, stdout) != bytes_read)
                write_error ();
            }
        }
    }

  எனில் (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  main_exit (EXIT_SUCCESS);
}
