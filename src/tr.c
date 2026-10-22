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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "xbinary-io.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tr"

#define AUTHORS proper_name ("Jim Meyering")

ଗଣନା { N_CHARS = UCHAR_MAX + 1 };

/* An unsigned integer type big enough to hold a repeat count or an
   unsigned character.  POSIX requires support for repeat counts as
   high as 2**31 - 1.  Since repeat counts might need to expand to
   match the length of an argument string, we need at least size_t to
   avoid arbitrary internal limits.  It doesn't cost much to use
   uintmax_t, though.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା uintmax_t ଗଣନା_2;

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
#define REPEAT_ଗଣନା_MAXIMUM BEGIN_STATE

/* The following (but not CC_NO_CLASS) are indices into the array of
   valid character class strings.  */
ଗଣନା Char_class
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
ଗଣନା Upper_Lower_class
  {
    UL_LOWER,
    UL_UPPER,
    UL_NONE
  };

/* The type of a List_element.  See build_spec_list for more details.  */
ଗଣନା Range_element_type
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
ସଂରଚନା List_element
  {
    ଗଣନା Range_element_type type;
    ସଂରଚନା List_element *next;
    ସଂଘ
      {
        unsigned ଅକ୍ଷର normal_char;
        ସଂରଚନା			/* unnamed */
          {
            unsigned ଅକ୍ଷର first_char;
            unsigned ଅକ୍ଷର last_char;
          }
        range;
        ଗଣନା Char_class char_class;
        unsigned ଅକ୍ଷର equiv_code;
        ସଂରଚନା			/* unnamed */
          {
            unsigned ଅକ୍ଷର the_repeated_char;
            ଗଣନା_2 repeat_ଗଣନା;
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
ସଂରଚନା Spec_list
  {
    /* Points to the head of the list of range elements.
       The first struct is a dummy; its members are never used.  */
    ସଂରଚନା List_element *head;

    /* When appending, points to the last element.  When traversing via
       get_next(), points to the element to process next.  Setting
       Spec_list.state to the value BEGIN_STATE before calling get_next
       signals get_next to initialize tail to point to head->next.  */
    ସଂରଚନା List_element *tail;

    /* Used to save state between calls to get_next.  */
    ଗଣନା_2 state;

    /* Length, in the sense that length ('a-z[:digit:]123abc')
       is 42 ( = 26 + 10 + 6).  */
    ଗଣନା_2 length;

    /* The number of [c*] and [c*0] constructs that appear in this spec.  */
    size_t n_indefinite_repeats;

    /* If n_indefinite_repeats is nonzero, this points to the List_element
       corresponding to the last [c*] or [c*0] construct encountered in
       this spec.  Otherwise it is undefined.  */
    ସଂରଚନା List_element *indefinite_repeat_element;

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
ସଂରଚନା E_string
{
  ଅକ୍ଷର *s;
  bool *escaped;
  size_t len;
};

/* Return nonzero if the Ith character of escaped string ES matches C
   and is not escaped itself.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
es_match (ସଂରଚନା E_string ଅଚଳ *es, size_t i, ଅକ୍ଷର c)
{
  ଫେରନ୍ତୁ es->s[i] == c && !es->escaped[i];
}

/* When true, each sequence in the input of a repeated character
   (call it c) is replaced (in the output) by a single occurrence of c
   for every c in the squeeze set.  */
ସ୍ଥିର bool squeeze_repeats = false;

/* When true, removes characters in the delete set from input.  */
ସ୍ଥିର bool delete = false;

/* Use the complement of set1 in place of set1.  */
ସ୍ଥିର bool complement = false;

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
ସ୍ଥିର bool truncate_set1 = false;

/* An alias for (!delete && non_option_args == 2).
   It is set in main and used there and in validate().  */
ସ୍ଥିର bool translating;

ସ୍ଥିର ଅକ୍ଷର io_buf[BUFSIZ];

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ char_class_name[] =
{
  "alnum", "alpha", "blank", "cntrl", "digit", "graph",
  "lower", "print", "punct", "space", "upper", "xdigit"
};

/* Array of boolean values.  A character 'c' is a member of the
   squeeze set if and only if in_squeeze_set[c] is true.  The squeeze
   set is defined by the last (possibly, the only) string argument
   on the command line when the squeeze option is given.  */
ସ୍ଥିର bool in_squeeze_set[N_CHARS];

/* Array of boolean values.  A character 'c' is a member of the
   delete set if and only if in_delete_set[c] is true.  The delete
   set is defined by the first (or only) string argument on the
   command line when the delete option is given.  */
ସ୍ଥିର bool in_delete_set[N_CHARS];

/* Array of character values defining the translation (if any) that
   tr is to perform.  Translation is performed only when there are
   two specification strings and the delete switch is not given.  */
ସ୍ଥିର ଅକ୍ଷର xlate[N_CHARS];

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"complement", no_argument, ଖାଲି, 'c'},
  {"delete", no_argument, ଖାଲି, 'd'},
  {"squeeze-repeats", no_argument, ଖାଲି, 's'},
  {"truncate-set1", no_argument, ଖାଲି, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... STRING1 [STRING2]\n\
"),
              program_name);
      fputs (_("\
Translate, squeeze, and/or delete characters from standard ନିବେଶ,\n\
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
         delete characters in ARRAY1, କର not translate\n\
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
  \\NNN            character with octal ମୂଲ୍ୟ NNN (1 to 3 octal digits)\n\
  \\\\              backslash\n\
  \\a              audible BEL\n\
  \\b              backspace\n\
  \\f              form feed\n\
  \\n              new ଧାଡ଼ି\n\
  \\r              ଫେରନ୍ତୁ\n\
  \\t              horizontal tab\n\
"), stdout);
     fputs (_("\
  \\v              vertical tab\n\
  CHAR1-CHAR2     all characters from CHAR1 to CHAR2 in ascending order\n\
  [CHAR*]         in ARRAY2, copies of CHAR until length of ARRAY1\n\
  [CHAR*REPEAT]   REPEAT copies of CHAR, REPEAT octal ଯଦି starting with 0\n\
  [:alnum:]       all letters and digits\n\
  [:alpha:]       all letters\n\
  [:blank:]       all horizontal whitespace\n\
  [:cntrl:]       all control characters\n\
  [:digit:]       all digits\n\
"), stdout);
     fputs (_("\
  [:graph:]       all printable characters, not including space\n\
  [:lower:]       all lower ସ୍ଥିତି letters\n\
  [:print:]       all printable characters, including space\n\
  [:punct:]       all punctuation characters\n\
  [:space:]       all horizontal or vertical whitespace\n\
  [:upper:]       all upper ସ୍ଥିତି letters\n\
  [:xdigit:]      all hexadecimal digits\n\
  [=CHAR=]        all characters which are equivalent to CHAR\n\
"), stdout);
     fputs (_("\
\n\
Translation occurs ଯଦି -d is not given and both STRING1 and STRING2 appear.\n\
-t is only significant when translating.  ARRAY2 is extended to length of\n\
ARRAY1 by repeating its last character as necessary.  Excess characters\n\
of ARRAY2 are ignored.  Character classes expand in unspecified order;\n\
ଯେତେବେଳେ translating, '[:lower:]' and '[:upper:]' may be used in pairs to\n\
specify ସ୍ଥିତି conversion.  Squeezing occurs after translation or deletion.\n\
Arguments like '[...]' should be quoted, to avoid potential shell globbing.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Return nonzero if the character C is a member of the
   equivalence class containing the character EQUIV_CLASS.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_equiv_class_member (unsigned ଅକ୍ଷର equiv_class, unsigned ଅକ୍ଷର c)
{
  ଫେରନ୍ତୁ (equiv_class == c);
}

/* Return true if the character C is a member of the
   character class CHAR_CLASS.  */

ATTRIBUTE_PURE
ସ୍ଥିର bool
is_char_class_member (ଗଣନା Char_class char_class, unsigned ଅକ୍ଷର c)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;

  ବାଛ (char_class)
    {
    ସ୍ଥିତି CC_ALNUM:
      result = isalnum (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_ALPHA:
      result = isalpha (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_BLANK:
      result = isblank (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_CNTRL:
      result = iscntrl (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_DIGIT:
      result = c_isdigit (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_GRAPH:
      result = isgraph (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_LOWER:
      result = islower (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_PRINT:
      result = isprint (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_PUNCT:
      result = ispunct (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_SPACE:
      result = isspace (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_UPPER:
      result = isupper (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_XDIGIT:
      result = c_isxdigit (c);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CC_NO_CLASS: ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      affirm (false);
    }

  ଫେରନ୍ତୁ !! result;
}

ସ୍ଥିର ଶୂନ୍ୟ
es_free (ସଂରଚନା E_string *es)
{
  ମୁକ୍ତ (es->s);
  ମୁକ୍ତ (es->escaped);
}

/* Perform the first pass over each range-spec argument S, converting all
   \c and \ddd escapes to their one-byte representations.  If an invalid
   quote sequence is found print an error message and return false;
   Otherwise set *ES to the resulting string and return true.
   The resulting array of characters may contain zero-bytes;
   however, on input, S is assumed to be null-terminated, and hence
   cannot contain actual (non-escaped) zero bytes.  */

ସ୍ଥିର bool
unquote (ଅକ୍ଷର ଅଚଳ *s, ସଂରଚନା E_string *es)
{
  size_t len = strlen (s);

  es->s = xmalloc (len);
  es->escaped = xcalloc (len, ଆକାର es->escaped[0]);

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; s[i]; i++)
    {
      unsigned ଅକ୍ଷର c;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା oct_digit;

      ବାଛ (s[i])
        {
        ସ୍ଥିତି '\\':
          es->escaped[j] = true;
          ବାଛ (s[i + 1])
            {
            ସ୍ଥିତି '\\':
              c = '\\';
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 'a':
              c = '\a';
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 'b':
              c = '\b';
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 'f':
              c = '\f';
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 'n':
              c = '\n';
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 'r':
              c = '\r';
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 't':
              c = '\t';
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 'v':
              c = '\v';
              ଭାଙ୍ଗ;
            ସ୍ଥିତି '0':
            ସ୍ଥିତି '1':
            ସ୍ଥିତି '2':
            ସ୍ଥିତି '3':
            ସ୍ଥିତି '4':
            ସ୍ଥିତି '5':
            ସ୍ଥିତି '6':
            ସ୍ଥିତି '7':
              c = s[i + 1] - '0';
              oct_digit = s[i + 2] - '0';
              ଯଦି (0 <= oct_digit && oct_digit <= 7)
                {
                  c = 8 * c + oct_digit;
                  ++i;
                  oct_digit = s[i + 2] - '0';
                  ଯଦି (0 <= oct_digit && oct_digit <= 7)
                    {
                      ଯଦି (8 * c + oct_digit < N_CHARS)
                        {
                          c = 8 * c + oct_digit;
                          ++i;
                        }
                      ଅନ୍ୟଥା
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
              ଭାଙ୍ଗ;
            ସ୍ଥିତି '\0':
              error (0, 0, _("warning: an unescaped backslash "
                             "at end of string is not portable"));
              /* POSIX is not clear about this.  */
              es->escaped[j] = false;
              i--;
              c = '\\';
              ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              c = s[i + 1];
              ଭାଙ୍ଗ;
            }
          ++i;
          es->s[j++] = c;
          ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          es->s[j++] = s[i];
          ଭାଙ୍ଗ;
        }
    }
  es->len = j;
  ଫେରନ୍ତୁ true;
}

/* If CLASS_STR is a valid character class string, return its index
   in the global char_class_name array.  Otherwise, return CC_NO_CLASS.  */

ATTRIBUTE_PURE
ସ୍ଥିର ଗଣନା Char_class
look_up_char_class (ଅକ୍ଷର ଅଚଳ *class_str, size_t len)
{
  ପାଇଁ (ଗଣନା Char_class i = 0; i < countof (char_class_name); i++)
    ଯଦି (STREQ_LEN (class_str, char_class_name[i], len)
        && strlen (char_class_name[i]) == len)
      ଫେରନ୍ତୁ i;
  ଫେରନ୍ତୁ CC_NO_CLASS;
}

/* Return a newly allocated string with a printable version of C.
   This function is used solely for formatting error messages.  */

ସ୍ଥିର ଅକ୍ଷର *
make_printable_char (unsigned ଅକ୍ଷର c)
{
  ଅକ୍ଷର *buf = xmalloc (5);

  ଯଦି (isprint (c))
    {
      buf[0] = c;
      buf[1] = '\0';
    }
  ଅନ୍ୟଥା
    {
      sprintf (buf, "\\%03o", c);
    }
  ଫେରନ୍ତୁ buf;
}

/* Return a newly allocated copy of S which is suitable for printing.
   LEN is the number of characters in S.  Most non-printing
   (isprint) characters are represented by a backslash followed by
   3 octal digits.  However, the characters represented by \c escapes
   where c is one of [abfnrtv] are represented by their 2-character \c
   sequences.  This function is used solely for printing error messages.  */

ସ୍ଥିର ଅକ୍ଷର *
make_printable_str (ଅକ୍ଷର ଅଚଳ *s, size_t len)
{
  /* Worst case is that every character expands to a backslash
     followed by a 3-character octal escape sequence.  */
  ଅକ୍ଷର *printable_buf = xnmalloc (len + 1, 4);
  ଅକ୍ଷର *p = printable_buf;

  ପାଇଁ (size_t i = 0; i < len; i++)
    {
      ଅକ୍ଷର buf[5];
      ଅକ୍ଷର ଅଚଳ *tmp = ଖାଲି;
      unsigned ଅକ୍ଷର c = s[i];

      ବାଛ (c)
        {
        ସ୍ଥିତି '\\':
          tmp = "\\";
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\a':
          tmp = "\\a";
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\b':
          tmp = "\\b";
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\f':
          tmp = "\\f";
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\n':
          tmp = "\\n";
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\r':
          tmp = "\\r";
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\t':
          tmp = "\\t";
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\v':
          tmp = "\\v";
          ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          ଯଦି (isprint (c))
            {
              buf[0] = c;
              buf[1] = '\0';
            }
          ଅନ୍ୟଥା
            sprintf (buf, "\\%03o", c);
          tmp = buf;
          ଭାଙ୍ଗ;
        }
      p = stpcpy (p, tmp);
    }
  ଫେରନ୍ତୁ printable_buf;
}

/* Append a newly allocated structure representing a
   character C to the specification list LIST.  */

ସ୍ଥିର ଶୂନ୍ୟ
append_normal_char (ସଂରଚନା Spec_list *list, unsigned ଅକ୍ଷର c)
{
  ସଂରଚନା List_element *new = xmalloc (ଆକାର *new);
  new->next = ଖାଲି;
  new->type = RE_NORMAL_CHAR;
  new->u.normal_char = c;
  list->tail->next = new;
  list->tail = new;
}

/* Append a newly allocated structure representing the range
   of characters from FIRST to LAST to the specification list LIST.
   Return false if LAST precedes FIRST in the collating sequence,
   true otherwise.  This means that '[c-c]' is acceptable.  */

ସ୍ଥିର bool
append_range (ସଂରଚନା Spec_list *list, unsigned ଅକ୍ଷର first, unsigned ଅକ୍ଷର last)
{
  ଯଦି (last < first)
    {
      ଅକ୍ଷର *tmp1 = make_printable_char (first);
      ଅକ୍ଷର *tmp2 = make_printable_char (last);

      error (0, 0,
       _("range-endpoints of '%s-%s' are in reverse collating sequence order"),
             tmp1, tmp2);
      ମୁକ୍ତ (tmp1);
      ମୁକ୍ତ (tmp2);
      ଫେରନ୍ତୁ false;
    }
  ସଂରଚନା List_element *new = xmalloc (ଆକାର *new);
  new->next = ଖାଲି;
  new->type = RE_RANGE;
  new->u.range.first_char = first;
  new->u.range.last_char = last;
  list->tail->next = new;
  list->tail = new;
  ଫେରନ୍ତୁ true;
}

/* If CHAR_CLASS_STR is a valid character class string, append a
   newly allocated structure representing that character class to the end
   of the specification list LIST and return true.  If CHAR_CLASS_STR is not
   a valid string return false.  */

ସ୍ଥିର bool
append_char_class (ସଂରଚନା Spec_list *list,
                   ଅକ୍ଷର ଅଚଳ *char_class_str, size_t len)
{
  ଗଣନା Char_class char_class = look_up_char_class (char_class_str, len);
  ଯଦି (char_class == CC_NO_CLASS)
    ଫେରନ୍ତୁ false;
  ସଂରଚନା List_element *new = xmalloc (ଆକାର *new);
  new->next = ଖାଲି;
  new->type = RE_CHAR_CLASS;
  new->u.char_class = char_class;
  list->tail->next = new;
  list->tail = new;
  ଫେରନ୍ତୁ true;
}

/* Append a newly allocated structure representing a [c*n]
   repeated character construct to the specification list LIST.
   THE_CHAR is the single character to be repeated, and REPEAT_COUNT
   is a non-negative repeat count.  */

ସ୍ଥିର ଶୂନ୍ୟ
append_repeated_char (ସଂରଚନା Spec_list *list, unsigned ଅକ୍ଷର the_char,
                      ଗଣନା_2 repeat_ଗଣନା)
{
  ସଂରଚନା List_element *new = xmalloc (ଆକାର *new);
  new->next = ଖାଲି;
  new->type = RE_REPEATED_CHAR;
  new->u.repeated_char.the_repeated_char = the_char;
  new->u.repeated_char.repeat_ଗଣନା = repeat_ଗଣନା;
  list->tail->next = new;
  list->tail = new;
}

/* Given a string, EQUIV_CLASS_STR, from a [=str=] context and
   the length of that string, LEN, if LEN is exactly one, append
   a newly allocated structure representing the specified
   equivalence class to the specification list, LIST and return true.
   If LEN is not 1, return false.  */

ସ୍ଥିର bool
append_equiv_class (ସଂରଚନା Spec_list *list,
                    ଅକ୍ଷର ଅଚଳ *equiv_class_str, size_t len)
{
  ଯଦି (len != 1)
    ଫେରନ୍ତୁ false;

  ସଂରଚନା List_element *new = xmalloc (ଆକାର *new);
  new->next = ଖାଲି;
  new->type = RE_EQUIV_CLASS;
  new->u.equiv_code = *equiv_class_str;
  list->tail->next = new;
  list->tail = new;
  ଫେରନ୍ତୁ true;
}

/* Search forward starting at START_IDX for the 2-char sequence
   (PRE_BRACKET_CHAR,']') in the string P of length P_LEN.  If such
   a sequence is found, set *RESULT_IDX to the index of the first
   character and return true.  Otherwise return false.  P may contain
   zero bytes.  */

ସ୍ଥିର bool
find_closing_delim (ଅଚଳ ସଂରଚନା E_string *es, size_t start_idx,
                    ଅକ୍ଷର pre_bracket_char, size_t *result_idx)
{
  ପାଇଁ (size_t i = start_idx; i < es->len - 1; i++)
    ଯଦି (es->s[i] == pre_bracket_char && es->s[i + 1] == ']'
        && !es->escaped[i] && !es->escaped[i + 1])
      {
        *result_idx = i;
        ଫେରନ୍ତୁ true;
      }
  ଫେରନ୍ତୁ false;
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
find_bracketed_repeat (ଅଚଳ ସଂରଚନା E_string *es, size_t start_idx,
                       unsigned ଅକ୍ଷର *char_to_repeat, ଗଣନା_2 *repeat_ଗଣନା,
                       size_t *closing_bracket_idx)
{
  affirm (start_idx + 1 < es->len);
  ଯଦି (!es_match (es, start_idx + 1, '*'))
    ଫେରନ୍ତୁ -1;

  ପାଇଁ (size_t i = start_idx + 2; i < es->len && !es->escaped[i]; i++)
    {
      ଯଦି (es->s[i] == ']')
        {
          size_t digit_str_len = i - start_idx - 2;

          *char_to_repeat = es->s[start_idx];
          ଯଦି (digit_str_len == 0)
            {
              /* We've matched [c*] -- no explicit repeat count.  */
              *repeat_ଗଣନା = 0;
            }
          ଅନ୍ୟଥା
            {
              /* Here, we have found [c*s] where s should be a string
                 of octal (if it starts with '0') or decimal digits.  */
              ଅକ୍ଷର ଅଚଳ *digit_str = &es->s[start_idx + 2];
              ଅକ୍ଷର *d_end;
              ଯଦି ((xstrtoumax (digit_str, &d_end, *digit_str == '0' ? 8 : 10,
                               repeat_ଗଣନା, ଖାଲି)
                   != LONGINT_OK)
                  || REPEAT_ଗଣନା_MAXIMUM < *repeat_ଗଣନା
                  || digit_str + digit_str_len != d_end)
                {
                  ଅକ୍ଷର *tmp = make_printable_str (digit_str, digit_str_len);
                  error (0, 0,
                         _("invalid repeat count %s in [c*n] construct"),
                         quote (tmp));
                  ମୁକ୍ତ (tmp);
                  ଫେରନ୍ତୁ -2;
                }
            }
          *closing_bracket_idx = i;
          ଫେରନ୍ତୁ 0;
        }
    }
  ଫେରନ୍ତୁ -1;			/* No bracket found.  */
}

/* Return true if the string at ES->s[IDX] matches the regular
   expression '\*[0-9]*]', false otherwise.  The string does not
   match if any of its characters are escaped.  */

ATTRIBUTE_PURE
ସ୍ଥିର bool
star_digits_closebracket (ଅଚଳ ସଂରଚନା E_string *es, size_t idx)
{
  ଯଦି (!es_match (es, idx, '*'))
    ଫେରନ୍ତୁ false;

  ପାଇଁ (size_t i = idx + 1; i < es->len; i++)
    ଯଦି (!c_isdigit (es->s[i]) || es->escaped[i])
      ଫେରନ୍ତୁ es_match (es, i, ']');
  ଫେରନ୍ତୁ false;
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

ସ୍ଥିର bool
build_spec_list (ଅଚଳ ସଂରଚନା E_string *es, ସଂରଚନା Spec_list *result)
{
  ଅକ୍ଷର ଅଚଳ *p = es->s;

  /* The main for-loop below recognizes the 4 multi-character constructs.
     A character that matches (in its context) none of the multi-character
     constructs is classified as 'normal'.  Since all multi-character
     constructs have at least 3 characters, any strings of length 2 or
     less are composed solely of normal characters.  Hence, the index of
     the outer for-loop runs only as far as LEN-2.  */
  size_t i;
  ପାଇଁ (i = 0; i + 2 < es->len; /* empty */)
    {
      ଯଦି (es_match (es, i, '['))
        {
          bool matched_multi_char_construct;
          size_t closing_bracket_idx;
          unsigned ଅକ୍ଷର char_to_repeat;
          ଗଣନା_2 repeat_ଗଣନା;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

          matched_multi_char_construct = true;
          ଯଦି (es_match (es, i + 1, ':') || es_match (es, i + 1, '='))
            {
              size_t closing_delim_idx;

              ଯଦି (find_closing_delim (es, i + 2, p[i + 1], &closing_delim_idx))
                {
                  size_t opnd_str_len = closing_delim_idx - 1 - (i + 2) + 1;
                  ଅକ୍ଷର ଅଚଳ *opnd_str = p + i + 2;

                  ଯଦି (opnd_str_len == 0)
                    {
                      ଯଦି (p[i + 1] == ':')
                        error (0, 0, _("missing character class name '[::]'"));
                      ଅନ୍ୟଥା
                        error (0, 0,
                               _("missing equivalence class character '[==]'"));
                      ଫେରନ୍ତୁ false;
                    }

                  ଯଦି (p[i + 1] == ':')
                    {
                      /* FIXME: big comment.  */
                      ଯଦି (!append_char_class (result, opnd_str, opnd_str_len))
                        {
                          ଯଦି (star_digits_closebracket (es, i + 2))
                            ଯାଅ try_bracketed_repeat;
                          ଅନ୍ୟଥା
                            {
                              ଅକ୍ଷର *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0, _("invalid character class %s"),
                                     quote (tmp));
                              ମୁକ୍ତ (tmp);
                              ଫେରନ୍ତୁ false;
                            }
                        }
                    }
                  ଅନ୍ୟଥା
                    {
                      /* FIXME: big comment.  */
                      ଯଦି (!append_equiv_class (result, opnd_str, opnd_str_len))
                        {
                          ଯଦି (star_digits_closebracket (es, i + 2))
                            ଯାଅ try_bracketed_repeat;
                          ଅନ୍ୟଥା
                            {
                              ଅକ୍ଷର *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0,
               _("%s: equivalence class operand must be a single character"),
                                     tmp);
                              ମୁକ୍ତ (tmp);
                              ଫେରନ୍ତୁ false;
                            }
                        }
                    }

                  i = closing_delim_idx + 2;
                  ଜାରି;
                }
              /* Else fall through.  This could be [:*] or [=*].  */
            }

        try_bracketed_repeat:

          /* Determine whether this is a bracketed repeat range
             matching the RE \[.\*(dec_or_oct_number)?].  */
          err = find_bracketed_repeat (es, i + 1, &char_to_repeat,
                                       &repeat_ଗଣନା,
                                       &closing_bracket_idx);
          ଯଦି (err == 0)
            {
              append_repeated_char (result, char_to_repeat, repeat_ଗଣନା);
              i = closing_bracket_idx + 1;
            }
          ଅନ୍ୟଥା ଯଦି (err == -1)
            {
              matched_multi_char_construct = false;
            }
          ଅନ୍ୟଥା
            {
              /* Found a string that looked like [c*n] but the
                 numeric part was invalid.  */
              ଫେରନ୍ତୁ false;
            }

          ଯଦି (matched_multi_char_construct)
            ଜାରି;

          /* We reach this point if P does not match [:str:], [=c=],
             [c*n], or [c*].  Now, see if P looks like a range '[-c'
             (from '[' to 'c').  */
        }

      /* Look ahead one char for ranges like a-z.  */
      ଯଦି (es_match (es, i + 1, '-'))
        {
          ଯଦି (!append_range (result, p[i], p[i + 2]))
            ଫେରନ୍ତୁ false;
          i += 3;
        }
      ଅନ୍ୟଥା
        {
          append_normal_char (result, p[i]);
          ++i;
        }
    }

  /* Now handle the (2 or fewer) remaining characters p[i]..p[es->len - 1].  */
  ପାଇଁ (; i < es->len; i++)
    append_normal_char (result, p[i]);

  ଫେରନ୍ତୁ true;
}

/* Advance past the current construct.
   S->tail must be non-null.  */
ସ୍ଥିର ଶୂନ୍ୟ
skip_construct (ସଂରଚନା Spec_list *s)
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_next (ସଂରଚନା Spec_list *s, ଗଣନା Upper_Lower_class *class)
{
  ସଂରଚନା List_element *p;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା return_val;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ଯଦି (class)
    *class = UL_NONE;

  ଯଦି (s->state == BEGIN_STATE)
    {
      s->tail = s->head->next;
      s->state = NEW_ELEMENT;
    }

  p = s->tail;
  ଯଦି (p == ଖାଲି)
    ଫେରନ୍ତୁ -1;

  ବାଛ (p->type)
    {
    ସ୍ଥିତି RE_NORMAL_CHAR:
      return_val = p->u.normal_char;
      s->state = NEW_ELEMENT;
      s->tail = p->next;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି RE_RANGE:
      ଯଦି (s->state == NEW_ELEMENT)
        s->state = p->u.range.first_char;
      ଅନ୍ୟଥା
        ++(s->state);
      return_val = s->state;
      ଯଦି (s->state == p->u.range.last_char)
        {
          s->tail = p->next;
          s->state = NEW_ELEMENT;
        }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି RE_CHAR_CLASS:
      ଯଦି (class)
        {
          ବାଛ (+p->u.char_class)
            {
            ସ୍ଥିତି CC_LOWER:
              *class = UL_LOWER;
              ଭାଙ୍ଗ;
            ସ୍ଥିତି CC_UPPER:
              *class = UL_UPPER;
              ଭାଙ୍ଗ;
            }
        }

      ଯଦି (s->state == NEW_ELEMENT)
        {
          ପାଇଁ (i = 0; i < N_CHARS; i++)
            ଯଦି (is_char_class_member (p->u.char_class, i))
              ଭାଙ୍ଗ;
          affirm (i < N_CHARS);
          s->state = i;
        }
      assure (is_char_class_member (p->u.char_class, s->state));
      return_val = s->state;
      ପାଇଁ (i = s->state + 1; i < N_CHARS; i++)
        ଯଦି (is_char_class_member (p->u.char_class, i))
          ଭାଙ୍ଗ;
      ଯଦି (i < N_CHARS)
        s->state = i;
      ଅନ୍ୟଥା
        {
          s->tail = p->next;
          s->state = NEW_ELEMENT;
        }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି RE_EQUIV_CLASS:
      /* FIXME: this assumes that each character is alone in its own
         equivalence class (which appears to be correct for my
         LC_COLLATE.  But I don't know of any function that allows
         one to determine a character's equivalence class.  */

      return_val = p->u.equiv_code;
      s->state = NEW_ELEMENT;
      s->tail = p->next;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି RE_REPEATED_CHAR:
      /* Here, a repeat count of n == 0 means don't repeat at all.  */
      ଯଦି (p->u.repeated_char.repeat_ଗଣନା == 0)
        {
          s->tail = p->next;
          s->state = NEW_ELEMENT;
          return_val = get_next (s, class);
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (s->state == NEW_ELEMENT)
            {
              s->state = 0;
            }
          ++(s->state);
          return_val = p->u.repeated_char.the_repeated_char;
          ଯଦି (s->state == p->u.repeated_char.repeat_ଗଣନା)
            {
              s->tail = p->next;
              s->state = NEW_ELEMENT;
            }
        }
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      unreachable ();
    }

  ଫେରନ୍ତୁ return_val;
}

/* This is a minor kludge.  This function is called from
   get_spec_stats to determine the cardinality of a set derived
   from a complemented string.  It's a kludge in that some of the
   same operations are (duplicated) performed in set_initialize.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
card_of_complement (ସଂରଚନା Spec_list *s)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cardinality = N_CHARS;
  bool in_set[N_CHARS] = {0};

  s->state = BEGIN_STATE;
  ଯେତେବେଳେ ((c = get_next (s, ଖାଲି)) != -1)
    {
      cardinality -= (!in_set[c]);
      in_set[c] = true;
    }
  ଫେରନ୍ତୁ cardinality;
}

/* Discard the lengths associated with a case conversion,
   as using the actual number of upper or lower case characters
   is problematic when they don't match in some locales.
   Also ensure the case conversion classes in string2 are
   aligned correctly with those in string1.
   Note POSIX says the behavior of 'tr "[:upper:]" "[:upper:]"'
   is undefined.  Therefore we allow it (unlike Solaris)
   and treat it as a no-op.  */

ସ୍ଥିର ଶୂନ୍ୟ
validate_case_classes (ସଂରଚନା Spec_list *s1, ସଂରଚନା Spec_list *s2)
{
  size_t n_upper = 0;
  size_t n_lower = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c1 = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c2 = 0;
  MAYBE_UNUSED ଗଣନା_2 old_s1_len = s1->length, old_s2_len = s2->length;
  ସଂରଚନା List_element *s1_tail = s1->tail;
  ସଂରଚନା List_element *s2_tail = s2->tail;
  bool s1_new_element = true;
  bool s2_new_element = true;

  ଯଦି (complement || !s2->has_char_class)
    ଫେରନ୍ତୁ;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N_CHARS; i++)
    {
      ଯଦି (isupper (i))
        n_upper++;
      ଯଦି (islower (i))
        n_lower++;
    }

  s1->state = BEGIN_STATE;
  s2->state = BEGIN_STATE;

  ଯେତେବେଳେ (c1 != -1 && c2 != -1)
    {
      ଗଣନା Upper_Lower_class class_s1, class_s2;

      c1 = get_next (s1, &class_s1);
      c2 = get_next (s2, &class_s2);

      /* If c2 transitions to a new case class, then
         c1 must also transition at the same time.  */
      ଯଦି (s2_new_element && class_s2 != UL_NONE
          && !(s1_new_element && class_s1 != UL_NONE))
        error (EXIT_FAILURE, 0,
               _("misaligned [:upper:] and/or [:lower:] construct"));

      /* If case converting, quickly skip over the elements.  */
      ଯଦି (class_s2 != UL_NONE)
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

ସ୍ଥିର ଶୂନ୍ୟ
get_spec_stats (ସଂରଚନା Spec_list *s)
{
  ଗଣନା_2 length = 0;

  s->n_indefinite_repeats = 0;
  s->has_equiv_class = false;
  s->has_restricted_char_class = false;
  s->has_char_class = false;
  ପାଇଁ (ସଂରଚନା List_element *p = s->head->next; p; p = p->next)
    {
      ଗଣନା_2 len = 0;

      ବାଛ (p->type)
        {
        ସ୍ଥିତି RE_NORMAL_CHAR:
          len = 1;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି RE_RANGE:
          affirm (p->u.range.last_char >= p->u.range.first_char);
          len = p->u.range.last_char - p->u.range.first_char + 1;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି RE_CHAR_CLASS:
          s->has_char_class = true;
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N_CHARS; i++)
            ଯଦି (is_char_class_member (p->u.char_class, i))
              ++len;
          ବାଛ (+p->u.char_class)
            {
            ସ୍ଥିତି CC_UPPER:
            ସ୍ଥିତି CC_LOWER:
              ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              s->has_restricted_char_class = true;
              ଭାଙ୍ଗ;
            }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି RE_EQUIV_CLASS:
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N_CHARS; i++)
            ଯଦି (is_equiv_class_member (p->u.equiv_code, i))
              ++len;
          s->has_equiv_class = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି RE_REPEATED_CHAR:
          ଯଦି (p->u.repeated_char.repeat_ଗଣନା > 0)
            len = p->u.repeated_char.repeat_ଗଣନା;
          ଅନ୍ୟଥା
            {
              s->indefinite_repeat_element = p;
              ++(s->n_indefinite_repeats);
            }
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          unreachable ();
        }

      /* Check for arithmetic overflow in computing length.  Also, reject
         any length greater than the maximum repeat count, in case the
         length is later used to compute the repeat count for an
         indefinite element.  */
      ଯଦି (ckd_add (&length, length, len) || REPEAT_ଗଣନା_MAXIMUM < length)
        error (EXIT_FAILURE, 0, _("too many characters in set"));
    }

  s->length = length;
}

ସ୍ଥିର ଶୂନ୍ୟ
get_s1_spec_stats (ସଂରଚନା Spec_list *s1)
{
  get_spec_stats (s1);
  ଯଦି (complement)
    s1->length = card_of_complement (s1);
}

ସ୍ଥିର ଶୂନ୍ୟ
get_s2_spec_stats (ସଂରଚନା Spec_list *s2, ଗଣନା_2 len_s1)
{
  get_spec_stats (s2);
  ଯଦି (len_s1 >= s2->length && s2->n_indefinite_repeats == 1)
    {
      s2->indefinite_repeat_element->u.repeated_char.repeat_ଗଣନା =
        len_s1 - s2->length;
      s2->length = len_s1;
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
spec_init (ସଂରଚନା Spec_list *spec_list)
{
  ସଂରଚନା List_element *new = xmalloc (ଆକାର *new);
  spec_list->head = spec_list->tail = new;
  spec_list->head->next = ଖାଲି;
}

/* This function makes two passes over the argument string S.  The first
   one converts all \c and \ddd escapes to their one-byte representations.
   The second constructs a linked specification list, SPEC_LIST, of the
   characters and constructs that comprise the argument string.  If either
   of these passes detects an error, this function returns false.  */

ସ୍ଥିର bool
parse_str (ଅକ୍ଷର ଅଚଳ *s, ସଂରଚନା Spec_list *spec_list)
{
  ସଂରଚନା E_string es;
  bool ok = unquote (s, &es) && build_spec_list (&es, spec_list);
  es_free (&es);
  ଫେରନ୍ତୁ ok;
}

/* Given two specification lists, S1 and S2, and assuming that
   S1->length > S2->length, append a single [c*n] element to S2 where c
   is the last character in the expansion of S2 and n is the difference
   between the two lengths.
   Upon successful completion, S2->length is set to S1->length.  The only
   way this function can fail to make S2 as long as S1 is when S2 has
   zero-length, since in that case, there is no last character to repeat.
   So S2->length is required to be at least 1.  */

ସ୍ଥିର ଶୂନ୍ୟ
string2_extend (ଅଚଳ ସଂରଚନା Spec_list *s1, ସଂରଚନା Spec_list *s2)
{
  ସଂରଚନା List_element *p;
  unsigned ଅକ୍ଷର char_to_repeat;

  affirm (translating);
  affirm (s1->length > s2->length);
  affirm (s2->length > 0);

  p = s2->tail;
  ବାଛ (p->type)
    {
    ସ୍ଥିତି RE_NORMAL_CHAR:
      char_to_repeat = p->u.normal_char;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି RE_RANGE:
      char_to_repeat = p->u.range.last_char;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି RE_CHAR_CLASS:
      /* Note BSD allows extending of classes in string2.  For example:
           tr '[:upper:]0-9' '[:lower:]'
         That's not portable however, contradicts POSIX and is dependent
         on your collating sequence.  */
      error (EXIT_FAILURE, 0,
             _("when translating with string1 longer than string2,\n"
               "the latter string must not end with a character class"));

    ସ୍ଥିତି RE_REPEATED_CHAR:
      char_to_repeat = p->u.repeated_char.the_repeated_char;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି RE_EQUIV_CLASS: ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
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

ସ୍ଥିର bool
homogeneous_spec_list (ସଂରଚନା Spec_list *s)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା b, c;

  s->state = BEGIN_STATE;

  ଯଦି ((b = get_next (s, ଖାଲି)) == -1)
    ଫେରନ୍ତୁ false;

  ଯେତେବେଳେ ((c = get_next (s, ଖାଲି)) != -1)
    ଯଦି (c != b)
      ଫେରନ୍ତୁ false;

  ଫେରନ୍ତୁ true;
}

/* Die with an error message if S1 and S2 describe strings that
   are not valid with the given command line switches.
   A side effect of this function is that if a valid [c*] or
   [c*0] construct appears in string2, it is converted to [c*n]
   with a value for n that makes s2->length == s1->length.  By
   the same token, if the --truncate-set1 option is not
   given, S2 may be extended.  */

ସ୍ଥିର ଶୂନ୍ୟ
validate (ସଂରଚନା Spec_list *s1, ସଂରଚନା Spec_list *s2)
{
  get_s1_spec_stats (s1);
  ଯଦି (s1->n_indefinite_repeats > 0)
    error (EXIT_FAILURE, 0,
           _("the [c*] repeat construct may not appear in string1"));

  ଯଦି (s2)
    {
      get_s2_spec_stats (s2, s1->length);

      ଯଦି (s2->n_indefinite_repeats > 1)
        error (EXIT_FAILURE, 0,
               _("only one [c*] repeat construct may appear in string2"));

      ଯଦି (translating)
        {
          ଯଦି (s2->has_equiv_class)
            error (EXIT_FAILURE, 0,
                   _("[=c=] expressions may not appear in string2"
                     " when translating"));

          ଯଦି (s2->has_restricted_char_class)
            error (EXIT_FAILURE, 0,
                   _("when translating, the only character classes"
                     " that may appear in\n"
                     "string2 are 'upper' and 'lower'"));

          validate_case_classes (s1, s2);

          ଯଦି (s1->length > s2->length)
            {
              ଯଦି (!truncate_set1)
                {
                  /* string2 must be non-empty unless --truncate-set1 is
                     given or string1 is empty.  */

                  ଯଦି (s2->length == 0)
                    error (EXIT_FAILURE, 0,
                           _("when not truncating set1,"
                             " string2 must be non-empty"));
                  string2_extend (s1, s2);
                }
            }

          ଯଦି (complement && s1->has_char_class
              && ! (s2->length == s1->length && homogeneous_spec_list (s2)))
            error (EXIT_FAILURE, 0,
                   _("when translating with complemented character classes,\n"
                     "string2 must map all characters in the domain to one"));
        }
      ଅନ୍ୟଥା
        /* Not translating.  */
        {
          ଯଦି (s2->n_indefinite_repeats > 0)
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

ସ୍ଥିର ଶୂନ୍ୟ
squeeze_filter (ଅକ୍ଷର *buf, size_t size, size_t (*reader) (ଅକ୍ଷର *, size_t))
{
  /* A value distinct from any character that may have been stored in a
     buffer as the result of a block-read in the function squeeze_filter.  */
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା NOT_A_CHAR = INT_MAX;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା char_to_squeeze = NOT_A_CHAR;
  size_t i = 0;
  size_t nr = 0;

  ଯେତେବେଳେ (true)
    {
      ଯଦି (i >= nr)
        {
          nr = reader (buf, size);
          ଯଦି (nr == 0)
            ଭାଙ୍ଗ;
          i = 0;
        }

      size_t begin = i;

      ଯଦି (char_to_squeeze == NOT_A_CHAR)
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
          ପାଇଁ (; i < nr && !in_squeeze_set[to_uchar (buf[i])]; i += 2)
            ଜାରି;

          /* There is a special case when i == nr and we've just
             skipped a character (the last one in buf) that is in
             the squeeze set.  */
          ଯଦି (i == nr && in_squeeze_set[to_uchar (buf[i - 1])])
            --i;

          ଯଦି (i >= nr)
            out_len = nr - begin;
          ଅନ୍ୟଥା
            {
              char_to_squeeze = buf[i];
              /* We're about to output buf[begin..i].  */
              out_len = i - begin + 1;

              /* But since we stepped by 2 in the loop above,
                 out_len may be one too large.  */
              ଯଦି (i > 0 && buf[i - 1] == char_to_squeeze)
                --out_len;

              /* Advance i to the index of first character to be
                 considered when looking for a char different from
                 char_to_squeeze.  */
              ++i;
            }
          ଯଦି (out_len > 0
              && fwrite (&buf[begin], 1, out_len, stdout) != out_len)
            write_error ();
        }

      ଯଦି (char_to_squeeze != NOT_A_CHAR)
        {
          /* Advance i to index of first char != char_to_squeeze
             (or to nr if all the rest of the characters in this
             buffer are the same as char_to_squeeze).  */
          ପାଇଁ (; i < nr && buf[i] == char_to_squeeze; i++)
            ଜାରି;
          ଯଦି (i < nr)
            char_to_squeeze = NOT_A_CHAR;
          /* If (i >= nr) we've squeezed the last character in this buffer.
             So now we have to read a new buffer and continue comparing
             characters against char_to_squeeze.  */
        }
    }
}

ସ୍ଥିର size_t
plain_read (ଅକ୍ଷର *buf, size_t size)
{
  ssize_t nr = ପଠ (STDIN_FILENO, buf, size);
  ଯଦି (nr < 0)
    error (EXIT_FAILURE, errno, _("read error"));
  ଫେରନ୍ତୁ nr;
}

/* Read buffers of SIZE bytes from stdin until one is found that
   contains at least one character not in the delete set.  Store
   in the array BUF, all characters from that buffer that are not
   in the delete set, and return the number of characters saved
   or 0 upon EOF.  */

ସ୍ଥିର size_t
read_and_delete (ଅକ୍ଷର *buf, size_t size)
{
  size_t n_saved;

  /* This enclosing do-while loop is to make sure that
     we don't return zero (indicating EOF) when we've
     just deleted all the characters in a buffer.  */
  କର
    {
      size_t nr = plain_read (buf, size);

      ଯଦି (nr == 0)
        ଫେରନ୍ତୁ 0;

      /* This first loop may be a waste of code, but gives much
         better performance when no characters are deleted in
         the beginning of a buffer.  It just avoids the copying
         of buf[i] into buf[n_saved] when it would be a NOP.  */

      size_t i;
      ପାଇଁ (i = 0; i < nr && !in_delete_set[to_uchar (buf[i])]; i++)
        ଜାରି;
      n_saved = i;

      ପାଇଁ (++i; i < nr; i++)
        ଯଦି (!in_delete_set[to_uchar (buf[i])])
          buf[n_saved++] = buf[i];
    }
  ଯେତେବେଳେ (n_saved == 0);

  ଫେରନ୍ତୁ n_saved;
}

/* Read at most SIZE bytes from stdin into the array BUF.  Then
   perform the in-place and one-to-one mapping specified by the global
   array 'xlate'.  Return the number of characters read, or 0 upon EOF.  */

ସ୍ଥିର size_t
read_and_xlate (ଅକ୍ଷର *buf, size_t size)
{
  size_t bytes_read = plain_read (buf, size);

  ପାଇଁ (size_t i = 0; i < bytes_read; i++)
    buf[i] = xlate[to_uchar (buf[i])];

  ଫେରନ୍ତୁ bytes_read;
}

/* Initialize a boolean membership set, IN_SET, with the character
   values obtained by traversing the linked list of constructs S
   using the function 'get_next'.  IN_SET is expected to have been
   initialized to all zeros by the caller.  If COMPLEMENT_THIS_SET
   is true the resulting set is complemented.  */

ସ୍ଥିର ଶୂନ୍ୟ
set_initialize (ସଂରଚନା Spec_list *s, bool complement_this_set, bool *in_set)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;

  s->state = BEGIN_STATE;
  ଯେତେବେଳେ ((c = get_next (s, ଖାଲି)) != -1)
    in_set[c] = true;
  ଯଦି (complement_this_set)
    ପାଇଁ (size_t i = 0; i < N_CHARS; i++)
      in_set[i] = (!in_set[i]);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା non_option_args;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା min_operands;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_operands;
  ସଂରଚନା Spec_list buf1, buf2;
  ସଂରଚନା Spec_list *s1 = &buf1;
  ସଂରଚନା Spec_list *s2 = &buf2;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "+AcCdstu", long_options, ଖାଲି)) != -1)
    {
      ବାଛ (c)
        {
        ସ୍ଥିତି 'A':
          /* Undocumented option, for compatibility with AIX.  */
          setlocale (LC_COLLATE, "C");
          setlocale (LC_CTYPE, "C");
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'c':
        ସ୍ଥିତି 'C':
          complement = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'd':
          delete = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 's':
          squeeze_repeats = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 't':
          truncate_set1 = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'u':
          /* Undocumented option, for compatibility with FreeBSD/macOS.  */
          setvbuf (stdout, ଖାଲି, _IONBF, 0);
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
          ଭାଙ୍ଗ;
        }
    }

  non_option_args = argc - optind;
  translating = (non_option_args == 2 && !delete);
  min_operands = 1 + (delete == squeeze_repeats);
  max_operands = 1 + (delete <= squeeze_repeats);

  ଯଦି (non_option_args < min_operands)
    {
      ଯଦି (non_option_args == 0)
        error (0, 0, _("missing operand"));
      ଅନ୍ୟଥା
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

  ଯଦି (max_operands < non_option_args)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + max_operands]));
      ଯଦି (non_option_args == 2)
        fprintf (stderr, "%s\n",
                 _("Only one string may be given when "
                   "deleting without squeezing repeats."));
      usage (EXIT_FAILURE);
    }

  spec_init (s1);
  ଯଦି (!parse_str (argv[optind], s1))
    main_exit (EXIT_FAILURE);

  ଯଦି (non_option_args == 2)
    {
      spec_init (s2);
      ଯଦି (!parse_str (argv[optind + 1], s2))
        main_exit (EXIT_FAILURE);
    }
  ଅନ୍ୟଥା
    s2 = ଖାଲି;

  validate (s1, s2);

  /* Use binary I/O, since 'tr' is sometimes used to transliterate
     non-printable characters, or characters which are stripped away
     by text-mode reads (like CR and ^Z).  */
  xset_ଦ୍ୱିମିକ_mode (STDIN_FILENO, O_ଦ୍ୱିମିକ);
  xset_ଦ୍ୱିମିକ_mode (STDOUT_FILENO, O_ଦ୍ୱିମିକ);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  ଯଦି (squeeze_repeats && non_option_args == 1)
    {
      set_initialize (s1, complement, in_squeeze_set);
      squeeze_filter (io_buf, ଆକାର io_buf, plain_read);
    }
  ଅନ୍ୟଥା ଯଦି (delete && non_option_args == 1)
    {
      set_initialize (s1, complement, in_delete_set);

      ଯେତେବେଳେ (true)
        {
          size_t nr = read_and_delete (io_buf, ଆକାର io_buf);
          ଯଦି (nr == 0)
            ଭାଙ୍ଗ;
          ଯଦି (fwrite (io_buf, 1, nr, stdout) != nr)
            write_error ();
        }
    }
  ଅନ୍ୟଥା ଯଦି (squeeze_repeats && delete && non_option_args == 2)
    {
      set_initialize (s1, complement, in_delete_set);
      set_initialize (s2, false, in_squeeze_set);
      squeeze_filter (io_buf, ଆକାର io_buf, read_and_delete);
    }
  ଅନ୍ୟଥା ଯଦି (translating)
    {
      ଯଦି (complement)
        {
          bool *in_s1 = in_delete_set;

          set_initialize (s1, false, in_s1);
          s2->state = BEGIN_STATE;
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N_CHARS; i++)
            {
              ଯଦି (!in_s1[i])
                {
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ch = get_next (s2, ଖାଲି);
                  affirm (ch != -1 || truncate_set1);
                  ଯଦି (ch == -1)
                    {
                      /* This will happen when tr is invoked like e.g.
                         tr -cs A-Za-z0-9 '\012'.  */
                      ଭାଙ୍ଗ;
                    }
                  xlate[i] = ch;
                }
            }
        }
      ଅନ୍ୟଥା
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା c1, c2;
          ଗଣନା Upper_Lower_class class_s1;
          ଗଣନା Upper_Lower_class class_s2;

          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          s1->state = BEGIN_STATE;
          s2->state = BEGIN_STATE;
          ଯେତେବେଳେ (true)
            {
              c1 = get_next (s1, &class_s1);
              c2 = get_next (s2, &class_s2);

              ଯଦି (class_s1 == UL_LOWER && class_s2 == UL_UPPER)
                {
                  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N_CHARS; i++)
                    ଯଦି (islower (i))
                      xlate[i] = toupper (i);
                }
              ଅନ୍ୟଥା ଯଦି (class_s1 == UL_UPPER && class_s2 == UL_LOWER)
                {
                  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N_CHARS; i++)
                    ଯଦି (isupper (i))
                      xlate[i] = tolower (i);
                }
              ଅନ୍ୟଥା
                {
                  /* The following should have been checked by validate...  */
                  ଯଦି (c1 == -1 || c2 == -1)
                    ଭାଙ୍ଗ;
                  xlate[c1] = c2;
                }

              /* When case-converting, skip the elements as an optimization.  */
              ଯଦି (class_s2 != UL_NONE)
                {
                  skip_construct (s1);
                  skip_construct (s2);
                }
            }
          affirm (c1 == -1 || truncate_set1);
        }
      ଯଦି (squeeze_repeats)
        {
          set_initialize (s2, false, in_squeeze_set);
          squeeze_filter (io_buf, ଆକାର io_buf, read_and_xlate);
        }
      ଅନ୍ୟଥା
        {
          ଯେତେବେଳେ (true)
            {
              size_t bytes_read = read_and_xlate (io_buf, ଆକାର io_buf);
              ଯଦି (bytes_read == 0)
                ଭାଙ୍ଗ;
              ଯଦି (fwrite (io_buf, 1, bytes_read, stdout) != bytes_read)
                write_error ();
            }
        }
    }

  ଯଦି (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  main_exit (EXIT_SUCCESS);
}
