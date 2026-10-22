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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "tr"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Jim Meyering")

ಎಣಿಕೆ { N_CHARS = UCHAR_MAX + 1 };

/* An unsigned integer type big enough to hold a repeat count or an
   unsigned character.  POSIX requires support for repeat counts as
   high as 2**31 - 1.  Since repeat counts might need to expand to
   match the length of an argument string, we need at least size_t to
   avoid arbitrary internal limits.  It doesn't cost much to use
   uintmax_t, though.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uintmax_t ಎಣಿಕೆ_2;

/* The value for Spec_list->state that indicates to
   get_next that it should initialize the tail pointer.
   Its value should be as large as possible to avoid conflict
   a valid value for the state field -- and that may be as
   large as any valid repeat_count.  */
#ವ್ಯಾಖ್ಯಾನಿಸು BEGIN_STATE (UINTMAX_MAX - 1)

/* The value for Spec_list->state that indicates to
   get_next that the element pointed to by Spec_list->tail is
   being considered for the first time on this pass through the
   list -- it indicates that get_next should make any necessary
   initializations.  */
#ವ್ಯಾಖ್ಯಾನಿಸು NEW_ELEMENT (BEGIN_STATE + 1)

/* The maximum possible repeat count.  Due to how the states are
   implemented, it can be as much as BEGIN_STATE.  */
#ವ್ಯಾಖ್ಯಾನಿಸು REPEAT_ಎಣಿಕೆ_ಗರಿಷ್ಠ BEGIN_STATE

/* The following (but not CC_NO_CLASS) are indices into the array of
   valid character class strings.  */
ಎಣಿಕೆ Char_class
  {
    CC_ALNUM = 0, CC_ALPHA = 1, CC_BLANK = 2, CC_CNTRL = 3,
    CC_DIGIT = 4, CC_ನಕ್ಷೆ = 5, CC_ಅಧೋ = 6, CC_PRINT = 7,
    CC_PUNCT = 8, CC_SPACE = 9, CC_ಊರ್ಧ್ವ = 10, CC_XDIGIT = 11,
    CC_NO_CLASS = 9999
  };

/* Character class to which a character (returned by get_next) belonged;
   but it is set only if the construct from which the character was obtained
   was one of the character classes [:upper:] or [:lower:].  The value
   is used only when translating and then, only to make sure that upper
   and lower class constructs have the same relative positions in string1
   and string2.  */
ಎಣಿಕೆ ಊರ್ಧ್ವ_ಅಧೋ_class
  {
    UL_ಅಧೋ,
    UL_ಊರ್ಧ್ವ,
    UL_NONE
  };

/* The type of a List_element.  See build_spec_list for more details.  */
ಎಣಿಕೆ Range_element_type
  {
    RE_ಸಾಮಾನ್ಯ_CHAR,
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
ರಚನೆ List_element
  {
    ಎಣಿಕೆ Range_element_type type;
    ರಚನೆ List_element *ಮುಂದಿನ;
    ಒಕ್ಕೂಟ
      {
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸಾಮಾನ್ಯ_char;
        ರಚನೆ			/* unnamed */
          {
            ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಮೊದಲ_char;
            ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಕೊನೆಯ_char;
          }
        range;
        ಎಣಿಕೆ Char_class char_class;
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ equiv_ಸಂಕೇತ;
        ರಚನೆ			/* unnamed */
          {
            ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ the_repeated_char;
            ಎಣಿಕೆ_2 repeat_ಎಣಿಕೆ;
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
ರಚನೆ Spec_list
  {
    /* Points to the head of the list of range elements.
       The first struct is a dummy; its members are never used.  */
    ರಚನೆ List_element *ತಲೆ;

    /* When appending, points to the last element.  When traversing via
       get_next(), points to the element to process next.  Setting
       Spec_list.state to the value BEGIN_STATE before calling get_next
       signals get_next to initialize tail to point to head->next.  */
    ರಚನೆ List_element *ಬಾಲ;

    /* Used to save state between calls to get_next.  */
    ಎಣಿಕೆ_2 state;

    /* Length, in the sense that length ('a-z[:digit:]123abc')
       is 42 ( = 26 + 10 + 6).  */
    ಎಣಿಕೆ_2 ಉದ್ದಳತೆ;

    /* The number of [c*] and [c*0] constructs that appear in this spec.  */
    size_t n_indefinite_repeats;

    /* If n_indefinite_repeats is nonzero, this points to the List_element
       corresponding to the last [c*] or [c*0] construct encountered in
       this spec.  Otherwise it is undefined.  */
    ರಚನೆ List_element *indefinite_repeat_element;

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
ರಚನೆ E_string
{
  ಅಕ್ಷರ *s;
  bool *escaped;
  size_t len;
};

/* Return nonzero if the Ith character of escaped string ES matches C
   and is not escaped itself.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
es_ಹೊಂದಿಕೆ (ರಚನೆ E_string ಸ್ಥಿರಾಂಕ *es, size_t i, ಅಕ್ಷರ c)
{
  ಹಿಂತಿರುಗಿ es->s[i] == c && !es->escaped[i];
}

/* When true, each sequence in the input of a repeated character
   (call it c) is replaced (in the output) by a single occurrence of c
   for every c in the squeeze set.  */
ಸ್ಥಿರ bool squeeze_repeats = false;

/* When true, removes characters in the delete set from input.  */
ಸ್ಥಿರ bool delete = false;

/* Use the complement of set1 in place of set1.  */
ಸ್ಥಿರ bool complement = false;

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
ಸ್ಥಿರ bool truncate_set1 = false;

/* An alias for (!delete && non_option_args == 2).
   It is set in main and used there and in validate().  */
ಸ್ಥಿರ bool translating;

ಸ್ಥಿರ ಅಕ್ಷರ io_buf[BUFSIZ];

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ char_class_name[] =
{
  "alnum", "alpha", "blank", "cntrl", "digit", "graph",
  "lower", "print", "punct", "space", "upper", "xdigit"
};

/* Array of boolean values.  A character 'c' is a member of the
   squeeze set if and only if in_squeeze_set[c] is true.  The squeeze
   set is defined by the last (possibly, the only) string argument
   on the command line when the squeeze option is given.  */
ಸ್ಥಿರ bool ಒಳ_squeeze_ಗಣ[N_CHARS];

/* Array of boolean values.  A character 'c' is a member of the
   delete set if and only if in_delete_set[c] is true.  The delete
   set is defined by the first (or only) string argument on the
   command line when the delete option is given.  */
ಸ್ಥಿರ bool ಒಳ_delete_ಗಣ[N_CHARS];

/* Array of character values defining the translation (if any) that
   tr is to perform.  Translation is performed only when there are
   two specification strings and the delete switch is not given.  */
ಸ್ಥಿರ ಅಕ್ಷರ xlate[N_CHARS];

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"complement", no_ವಾದ, ಖಾಲಿ, 'c'},
  {"delete", no_ವಾದ, ಖಾಲಿ, 'd'},
  {"squeeze-repeats", no_ವಾದ, ಖಾಲಿ, 's'},
  {"truncate-set1", no_ವಾದ, ಖಾಲಿ, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... STRING1 [STRING2]\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Translate, squeeze, and/or delete characters from standard ಒಳಹರಿವು,\n\
writing ಗಮ್ಯ standard output.  STRING1 and STRING2 specify arrays ದ\n\
characters ARRAY1 and ARRAY2 that control the action.\n\
\n\
"), stdout);
      oputs (_("\
  -c, -C, --complement\n\
         use the complement ದ ARRAY1\n\
"));
      oputs (_("\
  -d, --delete\n\
         delete characters ಒಳ ARRAY1, ಮಾಡು not translate\n\
"));
      oputs (_("\
  -s, --squeeze-repeats\n\
         replace each sequence ದ a repeated character\n\
         that is listed ಒಳ the ಕೊನೆಯ specified ARRAY,\n\
         with a single occurrence ದ that character\n\
"));
      oputs (_("\
  -t, --truncate-set1\n\
         ಮೊದಲ truncate ARRAY1 ಗಮ್ಯ ಉದ್ದಳತೆ ದ ARRAY2\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ARRAYs are specified as ಅಕ್ಷರಸರಣಿಗಳು ದ characters.  Most represent themselves.\n\
Interpreted sequences are:\n\
\n\
  \\NNN            character with octal ಮೌಲ್ಯ NNN (1 ಗಮ್ಯ 3 octal digits)\n\
  \\\\              backslash\n\
  \\a              audible BEL\n\
  \\b              backspace\n\
  \\f              form feed\n\
  \\n              new ಸಾಲು\n\
  \\r              ಹಿಂತಿರುಗಿ\n\
  \\t              horizontal tab\n\
"), stdout);
     fputs (_("\
  \\v              vertical tab\n\
  CHAR1-CHAR2     all characters from CHAR1 ಗಮ್ಯ CHAR2 ಒಳ ascending ಕ್ರಮ\n\
  [CHAR*]         ಒಳ ARRAY2, copies ದ CHAR until ಉದ್ದಳತೆ ದ ARRAY1\n\
  [CHAR*REPEAT]   REPEAT copies ದ CHAR, REPEAT octal ಆದರೆ starting with 0\n\
  [:alnum:]       all letters and digits\n\
  [:alpha:]       all letters\n\
  [:blank:]       all horizontal whitespace\n\
  [:cntrl:]       all control characters\n\
  [:digit:]       all digits\n\
"), stdout);
     fputs (_("\
  [:ನಕ್ಷೆ:]       all printable characters, not including space\n\
  [:ಅಧೋ:]       all ಅಧೋ ಸ್ಥಿತಿ letters\n\
  [:print:]       all printable characters, including space\n\
  [:punct:]       all punctuation characters\n\
  [:space:]       all horizontal or vertical whitespace\n\
  [:ಊರ್ಧ್ವ:]       all ಊರ್ಧ್ವ ಸ್ಥಿತಿ letters\n\
  [:xdigit:]      all hexadecimal digits\n\
  [=CHAR=]        all characters which are equivalent ಗಮ್ಯ CHAR\n\
"), stdout);
     fputs (_("\
\n\
Translation occurs ಆದರೆ -d is not given and both STRING1 and STRING2 appear.\n\
-t is only significant when translating.  ARRAY2 is extended ಗಮ್ಯ ಉದ್ದಳತೆ ದ\n\
ARRAY1 by repeating its ಕೊನೆಯ character as necessary.  Excess characters\n\
ದ ARRAY2 are ignored.  Character classes expand ಒಳ unspecified ಕ್ರಮ;\n\
ಆಗ translating, '[:lower:]' and '[:upper:]' may be ಬಳಸಿದ ಒಳ ಜೋಡಿಗಳು ಗಮ್ಯ\n\
specify ಸ್ಥಿತಿ conversion.  Squeezing occurs after translation or deletion.\n\
Arguments like '[...]' should be quoted, ಗಮ್ಯ avoid potential shell globbing.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Return nonzero if the character C is a member of the
   equivalence class containing the character EQUIV_CLASS.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_equiv_class_member (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ equiv_class, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c)
{
  ಹಿಂತಿರುಗಿ (equiv_class == c);
}

/* Return true if the character C is a member of the
   character class CHAR_CLASS.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
is_char_class_member (ಎಣಿಕೆ Char_class char_class, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c)
{
  ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;

  ಆಯ್ಕೆ (char_class)
    {
    ಸ್ಥಿತಿ CC_ALNUM:
      ಫಲಿತಾಂಶ = ಅಕ್ಷರವೇ_ಅಂಕಿಯೇ (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_ALPHA:
      ಫಲಿತಾಂಶ = ಅಕ್ಷರವೇ (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_BLANK:
      ಫಲಿತಾಂಶ = isblank (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_CNTRL:
      ಫಲಿತಾಂಶ = iscntrl (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_DIGIT:
      ಫಲಿತಾಂಶ = c_isdigit (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_ನಕ್ಷೆ:
      ಫಲಿತಾಂಶ = isgraph (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_ಅಧೋ:
      ಫಲಿತಾಂಶ = islower (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_PRINT:
      ಫಲಿತಾಂಶ = isprint (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_PUNCT:
      ಫಲಿತಾಂಶ = ispunct (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_SPACE:
      ಫಲಿತಾಂಶ = ಖಾಲಿಯೇ (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_ಊರ್ಧ್ವ:
      ಫಲಿತಾಂಶ = isupper (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_XDIGIT:
      ಫಲಿತಾಂಶ = c_isxdigit (c);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CC_NO_CLASS: ಪೂರ್ವನಿಯೋಜಿತ:
      affirm (false);
    }

  ಹಿಂತಿರುಗಿ !! ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
es_free (ರಚನೆ E_string *es)
{
  ಬಿಡುಗಡೆ (es->s);
  ಬಿಡುಗಡೆ (es->escaped);
}

/* Perform the first pass over each range-spec argument S, converting all
   \c and \ddd escapes to their one-byte representations.  If an invalid
   quote sequence is found print an error message and return false;
   Otherwise set *ES to the resulting string and return true.
   The resulting array of characters may contain zero-bytes;
   however, on input, S is assumed to be null-terminated, and hence
   cannot contain actual (non-escaped) zero bytes.  */

ಸ್ಥಿರ bool
unquote (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, ರಚನೆ E_string *es)
{
  size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (s);

  es->s = xmalloc (len);
  es->escaped = xcalloc (len, ಗಾತ್ರ es->escaped[0]);

  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ j = 0;
  ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; s[i]; i++)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c;
      ಪೂರ್ಣಾಂಕ oct_digit;

      ಆಯ್ಕೆ (s[i])
        {
        ಸ್ಥಿತಿ '\\':
          es->escaped[j] = true;
          ಆಯ್ಕೆ (s[i + 1])
            {
            ಸ್ಥಿತಿ '\\':
              c = '\\';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'a':
              c = '\a';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'b':
              c = '\b';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'f':
              c = '\f';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'n':
              c = '\n';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'r':
              c = '\r';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 't':
              c = '\t';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'v':
              c = '\v';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '0':
            ಸ್ಥಿತಿ '1':
            ಸ್ಥಿತಿ '2':
            ಸ್ಥಿತಿ '3':
            ಸ್ಥಿತಿ '4':
            ಸ್ಥಿತಿ '5':
            ಸ್ಥಿತಿ '6':
            ಸ್ಥಿತಿ '7':
              c = s[i + 1] - '0';
              oct_digit = s[i + 2] - '0';
              ಆದರೆ (0 <= oct_digit && oct_digit <= 7)
                {
                  c = 8 * c + oct_digit;
                  ++i;
                  oct_digit = s[i + 2] - '0';
                  ಆದರೆ (0 <= oct_digit && oct_digit <= 7)
                    {
                      ಆದರೆ (8 * c + oct_digit < N_CHARS)
                        {
                          c = 8 * c + oct_digit;
                          ++i;
                        }
                      ಇಲ್ಲದಿದ್ದರೆ
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
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '\0':
              error (0, 0, _("warning: an unescaped backslash "
                             "at end of string is not portable"));
              /* POSIX is not clear about this.  */
              es->escaped[j] = false;
              i--;
              c = '\\';
              ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
              c = s[i + 1];
              ನಿಲ್ಲಿಸು;
            }
          ++i;
          es->s[j++] = c;
          ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
          es->s[j++] = s[i];
          ನಿಲ್ಲಿಸು;
        }
    }
  es->len = j;
  ಹಿಂತಿರುಗಿ true;
}

/* If CLASS_STR is a valid character class string, return its index
   in the global char_class_name array.  Otherwise, return CC_NO_CLASS.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಎಣಿಕೆ Char_class
look_ಮೇಲೆ_char_class (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *class_str, size_t len)
{
  ಗಾಗಿ (ಎಣಿಕೆ Char_class i = 0; i < countof (char_class_name); i++)
    ಆದರೆ (STREQ_LEN (class_str, char_class_name[i], len)
        && ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (char_class_name[i]) == len)
      ಹಿಂತಿರುಗಿ i;
  ಹಿಂತಿರುಗಿ CC_NO_CLASS;
}

/* Return a newly allocated string with a printable version of C.
   This function is used solely for formatting error messages.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
make_printable_char (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c)
{
  ಅಕ್ಷರ *buf = xmalloc (5);

  ಆದರೆ (isprint (c))
    {
      buf[0] = c;
      buf[1] = '\0';
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      sprintf (buf, "\\%03o", c);
    }
  ಹಿಂತಿರುಗಿ buf;
}

/* Return a newly allocated copy of S which is suitable for printing.
   LEN is the number of characters in S.  Most non-printing
   (isprint) characters are represented by a backslash followed by
   3 octal digits.  However, the characters represented by \c escapes
   where c is one of [abfnrtv] are represented by their 2-character \c
   sequences.  This function is used solely for printing error messages.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
make_printable_str (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, size_t len)
{
  /* Worst case is that every character expands to a backslash
     followed by a 3-character octal escape sequence.  */
  ಅಕ್ಷರ *printable_buf = xnmalloc (len + 1, 4);
  ಅಕ್ಷರ *p = printable_buf;

  ಗಾಗಿ (size_t i = 0; i < len; i++)
    {
      ಅಕ್ಷರ buf[5];
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *tmp = ಖಾಲಿ;
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = s[i];

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ '\\':
          tmp = "\\";
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\a':
          tmp = "\\a";
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\b':
          tmp = "\\b";
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\f':
          tmp = "\\f";
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\n':
          tmp = "\\n";
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\r':
          tmp = "\\r";
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\t':
          tmp = "\\t";
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\v':
          tmp = "\\v";
          ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
          ಆದರೆ (isprint (c))
            {
              buf[0] = c;
              buf[1] = '\0';
            }
          ಇಲ್ಲದಿದ್ದರೆ
            sprintf (buf, "\\%03o", c);
          tmp = buf;
          ನಿಲ್ಲಿಸು;
        }
      p = stpcpy (p, tmp);
    }
  ಹಿಂತಿರುಗಿ printable_buf;
}

/* Append a newly allocated structure representing a
   character C to the specification list LIST.  */

ಸ್ಥಿರ ಶೂನ್ಯ
append_ಸಾಮಾನ್ಯ_char (ರಚನೆ Spec_list *list, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c)
{
  ರಚನೆ List_element *new = xmalloc (ಗಾತ್ರ *new);
  new->ಮುಂದಿನ = ಖಾಲಿ;
  new->type = RE_ಸಾಮಾನ್ಯ_CHAR;
  new->u.ಸಾಮಾನ್ಯ_char = c;
  list->ಬಾಲ->ಮುಂದಿನ = new;
  list->ಬಾಲ = new;
}

/* Append a newly allocated structure representing the range
   of characters from FIRST to LAST to the specification list LIST.
   Return false if LAST precedes FIRST in the collating sequence,
   true otherwise.  This means that '[c-c]' is acceptable.  */

ಸ್ಥಿರ bool
append_range (ರಚನೆ Spec_list *list, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಮೊದಲ, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಕೊನೆಯ)
{
  ಆದರೆ (ಕೊನೆಯ < ಮೊದಲ)
    {
      ಅಕ್ಷರ *tmp1 = make_printable_char (ಮೊದಲ);
      ಅಕ್ಷರ *tmp2 = make_printable_char (ಕೊನೆಯ);

      error (0, 0,
       _("range-endpoints of '%s-%s' are in reverse collating sequence order"),
             tmp1, tmp2);
      ಬಿಡುಗಡೆ (tmp1);
      ಬಿಡುಗಡೆ (tmp2);
      ಹಿಂತಿರುಗಿ false;
    }
  ರಚನೆ List_element *new = xmalloc (ಗಾತ್ರ *new);
  new->ಮುಂದಿನ = ಖಾಲಿ;
  new->type = RE_RANGE;
  new->u.range.ಮೊದಲ_char = ಮೊದಲ;
  new->u.range.ಕೊನೆಯ_char = ಕೊನೆಯ;
  list->ಬಾಲ->ಮುಂದಿನ = new;
  list->ಬಾಲ = new;
  ಹಿಂತಿರುಗಿ true;
}

/* If CHAR_CLASS_STR is a valid character class string, append a
   newly allocated structure representing that character class to the end
   of the specification list LIST and return true.  If CHAR_CLASS_STR is not
   a valid string return false.  */

ಸ್ಥಿರ bool
append_char_class (ರಚನೆ Spec_list *list,
                   ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *char_class_str, size_t len)
{
  ಎಣಿಕೆ Char_class char_class = look_ಮೇಲೆ_char_class (char_class_str, len);
  ಆದರೆ (char_class == CC_NO_CLASS)
    ಹಿಂತಿರುಗಿ false;
  ರಚನೆ List_element *new = xmalloc (ಗಾತ್ರ *new);
  new->ಮುಂದಿನ = ಖಾಲಿ;
  new->type = RE_CHAR_CLASS;
  new->u.char_class = char_class;
  list->ಬಾಲ->ಮುಂದಿನ = new;
  list->ಬಾಲ = new;
  ಹಿಂತಿರುಗಿ true;
}

/* Append a newly allocated structure representing a [c*n]
   repeated character construct to the specification list LIST.
   THE_CHAR is the single character to be repeated, and REPEAT_COUNT
   is a non-negative repeat count.  */

ಸ್ಥಿರ ಶೂನ್ಯ
append_repeated_char (ರಚನೆ Spec_list *list, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ the_char,
                      ಎಣಿಕೆ_2 repeat_ಎಣಿಕೆ)
{
  ರಚನೆ List_element *new = xmalloc (ಗಾತ್ರ *new);
  new->ಮುಂದಿನ = ಖಾಲಿ;
  new->type = RE_REPEATED_CHAR;
  new->u.repeated_char.the_repeated_char = the_char;
  new->u.repeated_char.repeat_ಎಣಿಕೆ = repeat_ಎಣಿಕೆ;
  list->ಬಾಲ->ಮುಂದಿನ = new;
  list->ಬಾಲ = new;
}

/* Given a string, EQUIV_CLASS_STR, from a [=str=] context and
   the length of that string, LEN, if LEN is exactly one, append
   a newly allocated structure representing the specified
   equivalence class to the specification list, LIST and return true.
   If LEN is not 1, return false.  */

ಸ್ಥಿರ bool
append_equiv_class (ರಚನೆ Spec_list *list,
                    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *equiv_class_str, size_t len)
{
  ಆದರೆ (len != 1)
    ಹಿಂತಿರುಗಿ false;

  ರಚನೆ List_element *new = xmalloc (ಗಾತ್ರ *new);
  new->ಮುಂದಿನ = ಖಾಲಿ;
  new->type = RE_EQUIV_CLASS;
  new->u.equiv_ಸಂಕೇತ = *equiv_class_str;
  list->ಬಾಲ->ಮುಂದಿನ = new;
  list->ಬಾಲ = new;
  ಹಿಂತಿರುಗಿ true;
}

/* Search forward starting at START_IDX for the 2-char sequence
   (PRE_BRACKET_CHAR,']') in the string P of length P_LEN.  If such
   a sequence is found, set *RESULT_IDX to the index of the first
   character and return true.  Otherwise return false.  P may contain
   zero bytes.  */

ಸ್ಥಿರ bool
ಕಂಡುಹಿಡಿ_closing_delim (ಸ್ಥಿರಾಂಕ ರಚನೆ E_string *es, size_t ಆರಂಭ_idx,
                    ಅಕ್ಷರ pre_bracket_char, size_t *ಫಲಿತಾಂಶ_idx)
{
  ಗಾಗಿ (size_t i = ಆರಂಭ_idx; i < es->len - 1; i++)
    ಆದರೆ (es->s[i] == pre_bracket_char && es->s[i + 1] == ']'
        && !es->escaped[i] && !es->escaped[i + 1])
      {
        *ಫಲಿತಾಂಶ_idx = i;
        ಹಿಂತಿರುಗಿ true;
      }
  ಹಿಂತಿರುಗಿ false;
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

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಕಂಡುಹಿಡಿ_bracketed_repeat (ಸ್ಥಿರಾಂಕ ರಚನೆ E_string *es, size_t ಆರಂಭ_idx,
                       ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *char_ಗಮ್ಯ_repeat, ಎಣಿಕೆ_2 *repeat_ಎಣಿಕೆ,
                       size_t *closing_bracket_idx)
{
  affirm (ಆರಂಭ_idx + 1 < es->len);
  ಆದರೆ (!es_ಹೊಂದಿಕೆ (es, ಆರಂಭ_idx + 1, '*'))
    ಹಿಂತಿರುಗಿ -1;

  ಗಾಗಿ (size_t i = ಆರಂಭ_idx + 2; i < es->len && !es->escaped[i]; i++)
    {
      ಆದರೆ (es->s[i] == ']')
        {
          size_t digit_str_len = i - ಆರಂಭ_idx - 2;

          *char_ಗಮ್ಯ_repeat = es->s[ಆರಂಭ_idx];
          ಆದರೆ (digit_str_len == 0)
            {
              /* We've matched [c*] -- no explicit repeat count.  */
              *repeat_ಎಣಿಕೆ = 0;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Here, we have found [c*s] where s should be a string
                 of octal (if it starts with '0') or decimal digits.  */
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *digit_str = &es->s[ಆರಂಭ_idx + 2];
              ಅಕ್ಷರ *d_ಕೊನೆ;
              ಆದರೆ ((xstrtoumax (digit_str, &d_ಕೊನೆ, *digit_str == '0' ? 8 : 10,
                               repeat_ಎಣಿಕೆ, ಖಾಲಿ)
                   != LONGINT_OK)
                  || REPEAT_ಎಣಿಕೆ_ಗರಿಷ್ಠ < *repeat_ಎಣಿಕೆ
                  || digit_str + digit_str_len != d_ಕೊನೆ)
                {
                  ಅಕ್ಷರ *tmp = make_printable_str (digit_str, digit_str_len);
                  error (0, 0,
                         _("invalid repeat count %s in [c*n] construct"),
                         quote (tmp));
                  ಬಿಡುಗಡೆ (tmp);
                  ಹಿಂತಿರುಗಿ -2;
                }
            }
          *closing_bracket_idx = i;
          ಹಿಂತಿರುಗಿ 0;
        }
    }
  ಹಿಂತಿರುಗಿ -1;			/* No bracket found.  */
}

/* Return true if the string at ES->s[IDX] matches the regular
   expression '\*[0-9]*]', false otherwise.  The string does not
   match if any of its characters are escaped.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
ನಕ್ಷತ್ರ_digits_closebracket (ಸ್ಥಿರಾಂಕ ರಚನೆ E_string *es, size_t idx)
{
  ಆದರೆ (!es_ಹೊಂದಿಕೆ (es, idx, '*'))
    ಹಿಂತಿರುಗಿ false;

  ಗಾಗಿ (size_t i = idx + 1; i < es->len; i++)
    ಆದರೆ (!c_isdigit (es->s[i]) || es->escaped[i])
      ಹಿಂತಿರುಗಿ es_ಹೊಂದಿಕೆ (es, i, ']');
  ಹಿಂತಿರುಗಿ false;
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

ಸ್ಥಿರ bool
build_spec_list (ಸ್ಥಿರಾಂಕ ರಚನೆ E_string *es, ರಚನೆ Spec_list *ಫಲಿತಾಂಶ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = es->s;

  /* The main for-loop below recognizes the 4 multi-character constructs.
     A character that matches (in its context) none of the multi-character
     constructs is classified as 'normal'.  Since all multi-character
     constructs have at least 3 characters, any strings of length 2 or
     less are composed solely of normal characters.  Hence, the index of
     the outer for-loop runs only as far as LEN-2.  */
  size_t i;
  ಗಾಗಿ (i = 0; i + 2 < es->len; /*ಖಾಲಿಗಳು*/)
    {
      ಆದರೆ (es_ಹೊಂದಿಕೆ (es, i, '['))
        {
          bool matched_multi_char_construct;
          size_t closing_bracket_idx;
          ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ char_ಗಮ್ಯ_repeat;
          ಎಣಿಕೆ_2 repeat_ಎಣಿಕೆ;
          ಪೂರ್ಣಾಂಕ err;

          matched_multi_char_construct = true;
          ಆದರೆ (es_ಹೊಂದಿಕೆ (es, i + 1, ':') || es_ಹೊಂದಿಕೆ (es, i + 1, '='))
            {
              size_t closing_delim_idx;

              ಆದರೆ (ಕಂಡುಹಿಡಿ_closing_delim (es, i + 2, p[i + 1], &closing_delim_idx))
                {
                  size_t opnd_str_len = closing_delim_idx - 1 - (i + 2) + 1;
                  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *opnd_str = p + i + 2;

                  ಆದರೆ (opnd_str_len == 0)
                    {
                      ಆದರೆ (p[i + 1] == ':')
                        error (0, 0, _("missing character class name '[::]'"));
                      ಇಲ್ಲದಿದ್ದರೆ
                        error (0, 0,
                               _("missing equivalence class character '[==]'"));
                      ಹಿಂತಿರುಗಿ false;
                    }

                  ಆದರೆ (p[i + 1] == ':')
                    {
                      /* FIXME: big comment.  */
                      ಆದರೆ (!append_char_class (ಫಲಿತಾಂಶ, opnd_str, opnd_str_len))
                        {
                          ಆದರೆ (ನಕ್ಷತ್ರ_digits_closebracket (es, i + 2))
                            ಹೋಗು try_bracketed_repeat;
                          ಇಲ್ಲದಿದ್ದರೆ
                            {
                              ಅಕ್ಷರ *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0, _("invalid character class %s"),
                                     quote (tmp));
                              ಬಿಡುಗಡೆ (tmp);
                              ಹಿಂತಿರುಗಿ false;
                            }
                        }
                    }
                  ಇಲ್ಲದಿದ್ದರೆ
                    {
                      /* FIXME: big comment.  */
                      ಆದರೆ (!append_equiv_class (ಫಲಿತಾಂಶ, opnd_str, opnd_str_len))
                        {
                          ಆದರೆ (ನಕ್ಷತ್ರ_digits_closebracket (es, i + 2))
                            ಹೋಗು try_bracketed_repeat;
                          ಇಲ್ಲದಿದ್ದರೆ
                            {
                              ಅಕ್ಷರ *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0,
               _("%s: equivalence class operand must be a single character"),
                                     tmp);
                              ಬಿಡುಗಡೆ (tmp);
                              ಹಿಂತಿರುಗಿ false;
                            }
                        }
                    }

                  i = closing_delim_idx + 2;
                  ಮುಂದುವರಿ;
                }
              /* Else fall through.  This could be [:*] or [=*].  */
            }

        try_bracketed_repeat:

          /* Determine whether this is a bracketed repeat range
             matching the RE \[.\*(dec_or_oct_number)?].  */
          err = ಕಂಡುಹಿಡಿ_bracketed_repeat (es, i + 1, &char_ಗಮ್ಯ_repeat,
                                       &repeat_ಎಣಿಕೆ,
                                       &closing_bracket_idx);
          ಆದರೆ (err == 0)
            {
              append_repeated_char (ಫಲಿತಾಂಶ, char_ಗಮ್ಯ_repeat, repeat_ಎಣಿಕೆ);
              i = closing_bracket_idx + 1;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (err == -1)
            {
              matched_multi_char_construct = false;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Found a string that looked like [c*n] but the
                 numeric part was invalid.  */
              ಹಿಂತಿರುಗಿ false;
            }

          ಆದರೆ (matched_multi_char_construct)
            ಮುಂದುವರಿ;

          /* We reach this point if P does not match [:str:], [=c=],
             [c*n], or [c*].  Now, see if P looks like a range '[-c'
             (from '[' to 'c').  */
        }

      /* Look ahead one char for ranges like a-z.  */
      ಆದರೆ (es_ಹೊಂದಿಕೆ (es, i + 1, '-'))
        {
          ಆದರೆ (!append_range (ಫಲಿತಾಂಶ, p[i], p[i + 2]))
            ಹಿಂತಿರುಗಿ false;
          i += 3;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          append_ಸಾಮಾನ್ಯ_char (ಫಲಿತಾಂಶ, p[i]);
          ++i;
        }
    }

  /* Now handle the (2 or fewer) remaining characters p[i]..p[es->len - 1].  */
  ಗಾಗಿ (; i < es->len; i++)
    append_ಸಾಮಾನ್ಯ_char (ಫಲಿತಾಂಶ, p[i]);

  ಹಿಂತಿರುಗಿ true;
}

/* Advance past the current construct.
   S->tail must be non-null.  */
ಸ್ಥಿರ ಶೂನ್ಯ
skip_construct (ರಚನೆ Spec_list *s)
{
  s->ಬಾಲ = s->ಬಾಲ->ಮುಂದಿನ;
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

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_ಮುಂದಿನ (ರಚನೆ Spec_list *s, ಎಣಿಕೆ ಊರ್ಧ್ವ_ಅಧೋ_class *class)
{
  ರಚನೆ List_element *p;
  ಪೂರ್ಣಾಂಕ return_val;
  ಪೂರ್ಣಾಂಕ i;

  ಆದರೆ (class)
    *class = UL_NONE;

  ಆದರೆ (s->state == BEGIN_STATE)
    {
      s->ಬಾಲ = s->ತಲೆ->ಮುಂದಿನ;
      s->state = NEW_ELEMENT;
    }

  p = s->ಬಾಲ;
  ಆದರೆ (p == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ -1;

  ಆಯ್ಕೆ (p->type)
    {
    ಸ್ಥಿತಿ RE_ಸಾಮಾನ್ಯ_CHAR:
      return_val = p->u.ಸಾಮಾನ್ಯ_char;
      s->state = NEW_ELEMENT;
      s->ಬಾಲ = p->ಮುಂದಿನ;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ RE_RANGE:
      ಆದರೆ (s->state == NEW_ELEMENT)
        s->state = p->u.range.ಮೊದಲ_char;
      ಇಲ್ಲದಿದ್ದರೆ
        ++(s->state);
      return_val = s->state;
      ಆದರೆ (s->state == p->u.range.ಕೊನೆಯ_char)
        {
          s->ಬಾಲ = p->ಮುಂದಿನ;
          s->state = NEW_ELEMENT;
        }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ RE_CHAR_CLASS:
      ಆದರೆ (class)
        {
          ಆಯ್ಕೆ (+p->u.char_class)
            {
            ಸ್ಥಿತಿ CC_ಅಧೋ:
              *class = UL_ಅಧೋ;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CC_ಊರ್ಧ್ವ:
              *class = UL_ಊರ್ಧ್ವ;
              ನಿಲ್ಲಿಸು;
            }
        }

      ಆದರೆ (s->state == NEW_ELEMENT)
        {
          ಗಾಗಿ (i = 0; i < N_CHARS; i++)
            ಆದರೆ (is_char_class_member (p->u.char_class, i))
              ನಿಲ್ಲಿಸು;
          affirm (i < N_CHARS);
          s->state = i;
        }
      assure (is_char_class_member (p->u.char_class, s->state));
      return_val = s->state;
      ಗಾಗಿ (i = s->state + 1; i < N_CHARS; i++)
        ಆದರೆ (is_char_class_member (p->u.char_class, i))
          ನಿಲ್ಲಿಸು;
      ಆದರೆ (i < N_CHARS)
        s->state = i;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          s->ಬಾಲ = p->ಮುಂದಿನ;
          s->state = NEW_ELEMENT;
        }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ RE_EQUIV_CLASS:
      /* FIXME: this assumes that each character is alone in its own
         equivalence class (which appears to be correct for my
         LC_COLLATE.  But I don't know of any function that allows
         one to determine a character's equivalence class.  */

      return_val = p->u.equiv_ಸಂಕೇತ;
      s->state = NEW_ELEMENT;
      s->ಬಾಲ = p->ಮುಂದಿನ;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ RE_REPEATED_CHAR:
      /* Here, a repeat count of n == 0 means don't repeat at all.  */
      ಆದರೆ (p->u.repeated_char.repeat_ಎಣಿಕೆ == 0)
        {
          s->ಬಾಲ = p->ಮುಂದಿನ;
          s->state = NEW_ELEMENT;
          return_val = get_ಮುಂದಿನ (s, class);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (s->state == NEW_ELEMENT)
            {
              s->state = 0;
            }
          ++(s->state);
          return_val = p->u.repeated_char.the_repeated_char;
          ಆದರೆ (s->state == p->u.repeated_char.repeat_ಎಣಿಕೆ)
            {
              s->ಬಾಲ = p->ಮುಂದಿನ;
              s->state = NEW_ELEMENT;
            }
        }
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      unreachable ();
    }

  ಹಿಂತಿರುಗಿ return_val;
}

/* This is a minor kludge.  This function is called from
   get_spec_stats to determine the cardinality of a set derived
   from a complemented string.  It's a kludge in that some of the
   same operations are (duplicated) performed in set_initialize.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
card_ದ_complement (ರಚನೆ Spec_list *s)
{
  ಪೂರ್ಣಾಂಕ c;
  ಪೂರ್ಣಾಂಕ cardinality = N_CHARS;
  bool ಒಳ_ಗಣ[N_CHARS] = {0};

  s->state = BEGIN_STATE;
  ಆಗ ((c = get_ಮುಂದಿನ (s, ಖಾಲಿ)) != -1)
    {
      cardinality -= (!ಒಳ_ಗಣ[c]);
      ಒಳ_ಗಣ[c] = true;
    }
  ಹಿಂತಿರುಗಿ cardinality;
}

/* Discard the lengths associated with a case conversion,
   as using the actual number of upper or lower case characters
   is problematic when they don't match in some locales.
   Also ensure the case conversion classes in string2 are
   aligned correctly with those in string1.
   Note POSIX says the behavior of 'tr "[:upper:]" "[:upper:]"'
   is undefined.  Therefore we allow it (unlike Solaris)
   and treat it as a no-op.  */

ಸ್ಥಿರ ಶೂನ್ಯ
validate_case_classes (ರಚನೆ Spec_list *s1, ರಚನೆ Spec_list *s2)
{
  size_t n_ಊರ್ಧ್ವ = 0;
  size_t n_ಅಧೋ = 0;
  ಪೂರ್ಣಾಂಕ c1 = 0;
  ಪೂರ್ಣಾಂಕ c2 = 0;
  MAYBE_ಬಳಸದ ಎಣಿಕೆ_2 old_s1_len = s1->ಉದ್ದಳತೆ, old_s2_len = s2->ಉದ್ದಳತೆ;
  ರಚನೆ List_element *s1_ಬಾಲ = s1->ಬಾಲ;
  ರಚನೆ List_element *s2_ಬಾಲ = s2->ಬಾಲ;
  bool s1_new_element = true;
  bool s2_new_element = true;

  ಆದರೆ (complement || !s2->has_char_class)
    ಹಿಂತಿರುಗಿ;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N_CHARS; i++)
    {
      ಆದರೆ (isupper (i))
        n_ಊರ್ಧ್ವ++;
      ಆದರೆ (islower (i))
        n_ಅಧೋ++;
    }

  s1->state = BEGIN_STATE;
  s2->state = BEGIN_STATE;

  ಆಗ (c1 != -1 && c2 != -1)
    {
      ಎಣಿಕೆ ಊರ್ಧ್ವ_ಅಧೋ_class class_s1, class_s2;

      c1 = get_ಮುಂದಿನ (s1, &class_s1);
      c2 = get_ಮುಂದಿನ (s2, &class_s2);

      /* If c2 transitions to a new case class, then
         c1 must also transition at the same time.  */
      ಆದರೆ (s2_new_element && class_s2 != UL_NONE
          && !(s1_new_element && class_s1 != UL_NONE))
        error (EXIT_FAILURE, 0,
               _("misaligned [:upper:] and/or [:lower:] construct"));

      /* If case converting, quickly skip over the elements.  */
      ಆದರೆ (class_s2 != UL_NONE)
        {
          skip_construct (s1);
          skip_construct (s2);
          /* Discount insignificant/problematic lengths.  */
          s1->ಉದ್ದಳತೆ -= (class_s1 == UL_ಊರ್ಧ್ವ ? n_ಊರ್ಧ್ವ : n_ಅಧೋ) - 1;
          s2->ಉದ್ದಳತೆ -= (class_s2 == UL_ಊರ್ಧ್ವ ? n_ಊರ್ಧ್ವ : n_ಅಧೋ) - 1;
        }

      s1_new_element = s1->state == NEW_ELEMENT; /* Next element is new.  */
      s2_new_element = s2->state == NEW_ELEMENT; /* Next element is new.  */
    }

  affirm (old_s1_len >= s1->ಉದ್ದಳತೆ && old_s2_len >= s2->ಉದ್ದಳತೆ);

  s1->ಬಾಲ = s1_ಬಾಲ;
  s2->ಬಾಲ = s2_ಬಾಲ;
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

ಸ್ಥಿರ ಶೂನ್ಯ
get_spec_stats (ರಚನೆ Spec_list *s)
{
  ಎಣಿಕೆ_2 ಉದ್ದಳತೆ = 0;

  s->n_indefinite_repeats = 0;
  s->has_equiv_class = false;
  s->has_restricted_char_class = false;
  s->has_char_class = false;
  ಗಾಗಿ (ರಚನೆ List_element *p = s->ತಲೆ->ಮುಂದಿನ; p; p = p->ಮುಂದಿನ)
    {
      ಎಣಿಕೆ_2 len = 0;

      ಆಯ್ಕೆ (p->type)
        {
        ಸ್ಥಿತಿ RE_ಸಾಮಾನ್ಯ_CHAR:
          len = 1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ RE_RANGE:
          affirm (p->u.range.ಕೊನೆಯ_char >= p->u.range.ಮೊದಲ_char);
          len = p->u.range.ಕೊನೆಯ_char - p->u.range.ಮೊದಲ_char + 1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ RE_CHAR_CLASS:
          s->has_char_class = true;
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N_CHARS; i++)
            ಆದರೆ (is_char_class_member (p->u.char_class, i))
              ++len;
          ಆಯ್ಕೆ (+p->u.char_class)
            {
            ಸ್ಥಿತಿ CC_ಊರ್ಧ್ವ:
            ಸ್ಥಿತಿ CC_ಅಧೋ:
              ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
              s->has_restricted_char_class = true;
              ನಿಲ್ಲಿಸು;
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ RE_EQUIV_CLASS:
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N_CHARS; i++)
            ಆದರೆ (is_equiv_class_member (p->u.equiv_ಸಂಕೇತ, i))
              ++len;
          s->has_equiv_class = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ RE_REPEATED_CHAR:
          ಆದರೆ (p->u.repeated_char.repeat_ಎಣಿಕೆ > 0)
            len = p->u.repeated_char.repeat_ಎಣಿಕೆ;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              s->indefinite_repeat_element = p;
              ++(s->n_indefinite_repeats);
            }
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          unreachable ();
        }

      /* Check for arithmetic overflow in computing length.  Also, reject
         any length greater than the maximum repeat count, in case the
         length is later used to compute the repeat count for an
         indefinite element.  */
      ಆದರೆ (ckd_ಕೂಡಿಸು (&ಉದ್ದಳತೆ, ಉದ್ದಳತೆ, len) || REPEAT_ಎಣಿಕೆ_ಗರಿಷ್ಠ < ಉದ್ದಳತೆ)
        error (EXIT_FAILURE, 0, _("too many characters in set"));
    }

  s->ಉದ್ದಳತೆ = ಉದ್ದಳತೆ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
get_s1_spec_stats (ರಚನೆ Spec_list *s1)
{
  get_spec_stats (s1);
  ಆದರೆ (complement)
    s1->ಉದ್ದಳತೆ = card_ದ_complement (s1);
}

ಸ್ಥಿರ ಶೂನ್ಯ
get_s2_spec_stats (ರಚನೆ Spec_list *s2, ಎಣಿಕೆ_2 len_s1)
{
  get_spec_stats (s2);
  ಆದರೆ (len_s1 >= s2->ಉದ್ದಳತೆ && s2->n_indefinite_repeats == 1)
    {
      s2->indefinite_repeat_element->u.repeated_char.repeat_ಎಣಿಕೆ =
        len_s1 - s2->ಉದ್ದಳತೆ;
      s2->ಉದ್ದಳತೆ = len_s1;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
spec_init (ರಚನೆ Spec_list *spec_list)
{
  ರಚನೆ List_element *new = xmalloc (ಗಾತ್ರ *new);
  spec_list->ತಲೆ = spec_list->ಬಾಲ = new;
  spec_list->ತಲೆ->ಮುಂದಿನ = ಖಾಲಿ;
}

/* This function makes two passes over the argument string S.  The first
   one converts all \c and \ddd escapes to their one-byte representations.
   The second constructs a linked specification list, SPEC_LIST, of the
   characters and constructs that comprise the argument string.  If either
   of these passes detects an error, this function returns false.  */

ಸ್ಥಿರ bool
parse_str (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, ರಚನೆ Spec_list *spec_list)
{
  ರಚನೆ E_string es;
  bool ok = unquote (s, &es) && build_spec_list (&es, spec_list);
  es_free (&es);
  ಹಿಂತಿರುಗಿ ok;
}

/* Given two specification lists, S1 and S2, and assuming that
   S1->length > S2->length, append a single [c*n] element to S2 where c
   is the last character in the expansion of S2 and n is the difference
   between the two lengths.
   Upon successful completion, S2->length is set to S1->length.  The only
   way this function can fail to make S2 as long as S1 is when S2 has
   zero-length, since in that case, there is no last character to repeat.
   So S2->length is required to be at least 1.  */

ಸ್ಥಿರ ಶೂನ್ಯ
string2_extend (ಸ್ಥಿರಾಂಕ ರಚನೆ Spec_list *s1, ರಚನೆ Spec_list *s2)
{
  ರಚನೆ List_element *p;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ char_ಗಮ್ಯ_repeat;

  affirm (translating);
  affirm (s1->ಉದ್ದಳತೆ > s2->ಉದ್ದಳತೆ);
  affirm (s2->ಉದ್ದಳತೆ > 0);

  p = s2->ಬಾಲ;
  ಆಯ್ಕೆ (p->type)
    {
    ಸ್ಥಿತಿ RE_ಸಾಮಾನ್ಯ_CHAR:
      char_ಗಮ್ಯ_repeat = p->u.ಸಾಮಾನ್ಯ_char;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ RE_RANGE:
      char_ಗಮ್ಯ_repeat = p->u.range.ಕೊನೆಯ_char;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ RE_CHAR_CLASS:
      /* Note BSD allows extending of classes in string2.  For example:
           tr '[:upper:]0-9' '[:lower:]'
         That's not portable however, contradicts POSIX and is dependent
         on your collating sequence.  */
      error (EXIT_FAILURE, 0,
             _("when translating with string1 longer than string2,\n"
               "the latter string must not end with a character class"));

    ಸ್ಥಿತಿ RE_REPEATED_CHAR:
      char_ಗಮ್ಯ_repeat = p->u.repeated_char.the_repeated_char;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ RE_EQUIV_CLASS: ಪೂರ್ವನಿಯೋಜಿತ:
      /* This shouldn't happen, because validate exits with an error
         if it finds an equiv class in string2 when translating.  */
      affirm (false);
    }

  append_repeated_char (s2, char_ಗಮ್ಯ_repeat, s1->ಉದ್ದಳತೆ - s2->ಉದ್ದಳತೆ);
  s2->ಉದ್ದಳತೆ = s1->ಉದ್ದಳತೆ;
}

/* Return true if S is a non-empty list in which exactly one
   character (but potentially, many instances of it) appears.
   E.g., [X*] or xxxxxxxx.  */

ಸ್ಥಿರ bool
homogeneous_spec_list (ರಚನೆ Spec_list *s)
{
  ಪೂರ್ಣಾಂಕ b, c;

  s->state = BEGIN_STATE;

  ಆದರೆ ((b = get_ಮುಂದಿನ (s, ಖಾಲಿ)) == -1)
    ಹಿಂತಿರುಗಿ false;

  ಆಗ ((c = get_ಮುಂದಿನ (s, ಖಾಲಿ)) != -1)
    ಆದರೆ (c != b)
      ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ true;
}

/* Die with an error message if S1 and S2 describe strings that
   are not valid with the given command line switches.
   A side effect of this function is that if a valid [c*] or
   [c*0] construct appears in string2, it is converted to [c*n]
   with a value for n that makes s2->length == s1->length.  By
   the same token, if the --truncate-set1 option is not
   given, S2 may be extended.  */

ಸ್ಥಿರ ಶೂನ್ಯ
validate (ರಚನೆ Spec_list *s1, ರಚನೆ Spec_list *s2)
{
  get_s1_spec_stats (s1);
  ಆದರೆ (s1->n_indefinite_repeats > 0)
    error (EXIT_FAILURE, 0,
           _("the [c*] repeat construct may not appear in string1"));

  ಆದರೆ (s2)
    {
      get_s2_spec_stats (s2, s1->ಉದ್ದಳತೆ);

      ಆದರೆ (s2->n_indefinite_repeats > 1)
        error (EXIT_FAILURE, 0,
               _("only one [c*] repeat construct may appear in string2"));

      ಆದರೆ (translating)
        {
          ಆದರೆ (s2->has_equiv_class)
            error (EXIT_FAILURE, 0,
                   _("[=c=] expressions may not appear in string2"
                     " when translating"));

          ಆದರೆ (s2->has_restricted_char_class)
            error (EXIT_FAILURE, 0,
                   _("when translating, the only character classes"
                     " that may appear in\n"
                     "string2 are 'upper' and 'lower'"));

          validate_case_classes (s1, s2);

          ಆದರೆ (s1->ಉದ್ದಳತೆ > s2->ಉದ್ದಳತೆ)
            {
              ಆದರೆ (!truncate_set1)
                {
                  /* string2 must be non-empty unless --truncate-set1 is
                     given or string1 is empty.  */

                  ಆದರೆ (s2->ಉದ್ದಳತೆ == 0)
                    error (EXIT_FAILURE, 0,
                           _("when not truncating set1,"
                             " string2 must be non-empty"));
                  string2_extend (s1, s2);
                }
            }

          ಆದರೆ (complement && s1->has_char_class
              && ! (s2->ಉದ್ದಳತೆ == s1->ಉದ್ದಳತೆ && homogeneous_spec_list (s2)))
            error (EXIT_FAILURE, 0,
                   _("when translating with complemented character classes,\n"
                     "string2 must map all characters in the domain to one"));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        /* Not translating.  */
        {
          ಆದರೆ (s2->n_indefinite_repeats > 0)
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

ಸ್ಥಿರ ಶೂನ್ಯ
squeeze_filter (ಅಕ್ಷರ *buf, size_t ಗಾತ್ರ_2, size_t (*reader) (ಅಕ್ಷರ *, size_t))
{
  /* A value distinct from any character that may have been stored in a
     buffer as the result of a block-read in the function squeeze_filter.  */
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ NOT_A_CHAR = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;

  ಪೂರ್ಣಾಂಕ char_ಗಮ್ಯ_squeeze = NOT_A_CHAR;
  size_t i = 0;
  size_t nr = 0;

  ಆಗ (true)
    {
      ಆದರೆ (i >= nr)
        {
          nr = reader (buf, ಗಾತ್ರ_2);
          ಆದರೆ (nr == 0)
            ನಿಲ್ಲಿಸು;
          i = 0;
        }

      size_t begin = i;

      ಆದರೆ (char_ಗಮ್ಯ_squeeze == NOT_A_CHAR)
        {
          size_t ಹೊರಹೋಗು_len;
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
          ಗಾಗಿ (; i < nr && !ಒಳ_squeeze_ಗಣ[ಗಮ್ಯ_uchar (buf[i])]; i += 2)
            ಮುಂದುವರಿ;

          /* There is a special case when i == nr and we've just
             skipped a character (the last one in buf) that is in
             the squeeze set.  */
          ಆದರೆ (i == nr && ಒಳ_squeeze_ಗಣ[ಗಮ್ಯ_uchar (buf[i - 1])])
            --i;

          ಆದರೆ (i >= nr)
            ಹೊರಹೋಗು_len = nr - begin;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              char_ಗಮ್ಯ_squeeze = buf[i];
              /* We're about to output buf[begin..i].  */
              ಹೊರಹೋಗು_len = i - begin + 1;

              /* But since we stepped by 2 in the loop above,
                 out_len may be one too large.  */
              ಆದರೆ (i > 0 && buf[i - 1] == char_ಗಮ್ಯ_squeeze)
                --ಹೊರಹೋಗು_len;

              /* Advance i to the index of first character to be
                 considered when looking for a char different from
                 char_to_squeeze.  */
              ++i;
            }
          ಆದರೆ (ಹೊರಹೋಗು_len > 0
              && fwrite (&buf[begin], 1, ಹೊರಹೋಗು_len, stdout) != ಹೊರಹೋಗು_len)
            write_error ();
        }

      ಆದರೆ (char_ಗಮ್ಯ_squeeze != NOT_A_CHAR)
        {
          /* Advance i to index of first char != char_to_squeeze
             (or to nr if all the rest of the characters in this
             buffer are the same as char_to_squeeze).  */
          ಗಾಗಿ (; i < nr && buf[i] == char_ಗಮ್ಯ_squeeze; i++)
            ಮುಂದುವರಿ;
          ಆದರೆ (i < nr)
            char_ಗಮ್ಯ_squeeze = NOT_A_CHAR;
          /* If (i >= nr) we've squeezed the last character in this buffer.
             So now we have to read a new buffer and continue comparing
             characters against char_to_squeeze.  */
        }
    }
}

ಸ್ಥಿರ size_t
plain_read (ಅಕ್ಷರ *buf, size_t ಗಾತ್ರ_2)
{
  ssize_t nr = ಪಠ (STDIN_FILENO, buf, ಗಾತ್ರ_2);
  ಆದರೆ (nr < 0)
    error (EXIT_FAILURE, errno, _("read error"));
  ಹಿಂತಿರುಗಿ nr;
}

/* Read buffers of SIZE bytes from stdin until one is found that
   contains at least one character not in the delete set.  Store
   in the array BUF, all characters from that buffer that are not
   in the delete set, and return the number of characters saved
   or 0 upon EOF.  */

ಸ್ಥಿರ size_t
read_and_delete (ಅಕ್ಷರ *buf, size_t ಗಾತ್ರ_2)
{
  size_t n_saved;

  /* This enclosing do-while loop is to make sure that
     we don't return zero (indicating EOF) when we've
     just deleted all the characters in a buffer.  */
  ಮಾಡು
    {
      size_t nr = plain_read (buf, ಗಾತ್ರ_2);

      ಆದರೆ (nr == 0)
        ಹಿಂತಿರುಗಿ 0;

      /* This first loop may be a waste of code, but gives much
         better performance when no characters are deleted in
         the beginning of a buffer.  It just avoids the copying
         of buf[i] into buf[n_saved] when it would be a NOP.  */

      size_t i;
      ಗಾಗಿ (i = 0; i < nr && !ಒಳ_delete_ಗಣ[ಗಮ್ಯ_uchar (buf[i])]; i++)
        ಮುಂದುವರಿ;
      n_saved = i;

      ಗಾಗಿ (++i; i < nr; i++)
        ಆದರೆ (!ಒಳ_delete_ಗಣ[ಗಮ್ಯ_uchar (buf[i])])
          buf[n_saved++] = buf[i];
    }
  ಆಗ (n_saved == 0);

  ಹಿಂತಿರುಗಿ n_saved;
}

/* Read at most SIZE bytes from stdin into the array BUF.  Then
   perform the in-place and one-to-one mapping specified by the global
   array 'xlate'.  Return the number of characters read, or 0 upon EOF.  */

ಸ್ಥಿರ size_t
read_and_xlate (ಅಕ್ಷರ *buf, size_t ಗಾತ್ರ_2)
{
  size_t ಬೈಟುಗಳು_read = plain_read (buf, ಗಾತ್ರ_2);

  ಗಾಗಿ (size_t i = 0; i < ಬೈಟುಗಳು_read; i++)
    buf[i] = xlate[ಗಮ್ಯ_uchar (buf[i])];

  ಹಿಂತಿರುಗಿ ಬೈಟುಗಳು_read;
}

/* Initialize a boolean membership set, IN_SET, with the character
   values obtained by traversing the linked list of constructs S
   using the function 'get_next'.  IN_SET is expected to have been
   initialized to all zeros by the caller.  If COMPLEMENT_THIS_SET
   is true the resulting set is complemented.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_initialize (ರಚನೆ Spec_list *s, bool complement_this_ಗಣ, bool *ಒಳ_ಗಣ)
{
  ಪೂರ್ಣಾಂಕ c;

  s->state = BEGIN_STATE;
  ಆಗ ((c = get_ಮುಂದಿನ (s, ಖಾಲಿ)) != -1)
    ಒಳ_ಗಣ[c] = true;
  ಆದರೆ (complement_this_ಗಣ)
    ಗಾಗಿ (size_t i = 0; i < N_CHARS; i++)
      ಒಳ_ಗಣ[i] = (!ಒಳ_ಗಣ[i]);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ c;
  ಪೂರ್ಣಾಂಕ non_option_ವಾದಗಳು;
  ಪೂರ್ಣಾಂಕ min_operands;
  ಪೂರ್ಣಾಂಕ max_operands;
  ರಚನೆ Spec_list buf1, buf2;
  ರಚನೆ Spec_list *s1 = &buf1;
  ರಚನೆ Spec_list *s2 = &buf2;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((c = getopt_long (argc, argv, "+AcCdstu", long_options, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'A':
          /* Undocumented option, for compatibility with AIX.  */
          setlocale (LC_COLLATE, "C");
          setlocale (LC_CTYPE, "C");
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'c':
        ಸ್ಥಿತಿ 'C':
          complement = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'd':
          delete = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          squeeze_repeats = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          truncate_set1 = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'u':
          /* Undocumented option, for compatibility with FreeBSD/macOS.  */
          setvbuf (stdout, ಖಾಲಿ, _IONBF, 0);
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
          ನಿಲ್ಲಿಸು;
        }
    }

  non_option_ವಾದಗಳು = argc - optind;
  translating = (non_option_ವಾದಗಳು == 2 && !delete);
  min_operands = 1 + (delete == squeeze_repeats);
  max_operands = 1 + (delete <= squeeze_repeats);

  ಆದರೆ (non_option_ವಾದಗಳು < min_operands)
    {
      ಆದರೆ (non_option_ವಾದಗಳು == 0)
        error (0, 0, _("missing operand"));
      ಇಲ್ಲದಿದ್ದರೆ
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

  ಆದರೆ (max_operands < non_option_ವಾದಗಳು)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + max_operands]));
      ಆದರೆ (non_option_ವಾದಗಳು == 2)
        fprintf (stderr, "%s\n",
                 _("Only one string may be given when "
                   "deleting without squeezing repeats."));
      usage (EXIT_FAILURE);
    }

  spec_init (s1);
  ಆದರೆ (!parse_str (argv[optind], s1))
    main_exit (EXIT_FAILURE);

  ಆದರೆ (non_option_ವಾದಗಳು == 2)
    {
      spec_init (s2);
      ಆದರೆ (!parse_str (argv[optind + 1], s2))
        main_exit (EXIT_FAILURE);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    s2 = ಖಾಲಿ;

  validate (s1, s2);

  /* Use binary I/O, since 'tr' is sometimes used to transliterate
     non-printable characters, or characters which are stripped away
     by text-mode reads (like CR and ^Z).  */
  xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDIN_FILENO, O_ದ್ವಿಮಾನ);
  xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDOUT_FILENO, O_ದ್ವಿಮಾನ);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  ಆದರೆ (squeeze_repeats && non_option_ವಾದಗಳು == 1)
    {
      ಗಣ_initialize (s1, complement, ಒಳ_squeeze_ಗಣ);
      squeeze_filter (io_buf, ಗಾತ್ರ io_buf, plain_read);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (delete && non_option_ವಾದಗಳು == 1)
    {
      ಗಣ_initialize (s1, complement, ಒಳ_delete_ಗಣ);

      ಆಗ (true)
        {
          size_t nr = read_and_delete (io_buf, ಗಾತ್ರ io_buf);
          ಆದರೆ (nr == 0)
            ನಿಲ್ಲಿಸು;
          ಆದರೆ (fwrite (io_buf, 1, nr, stdout) != nr)
            write_error ();
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (squeeze_repeats && delete && non_option_ವಾದಗಳು == 2)
    {
      ಗಣ_initialize (s1, complement, ಒಳ_delete_ಗಣ);
      ಗಣ_initialize (s2, false, ಒಳ_squeeze_ಗಣ);
      squeeze_filter (io_buf, ಗಾತ್ರ io_buf, read_and_delete);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (translating)
    {
      ಆದರೆ (complement)
        {
          bool *ಒಳ_s1 = ಒಳ_delete_ಗಣ;

          ಗಣ_initialize (s1, false, ಒಳ_s1);
          s2->state = BEGIN_STATE;
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N_CHARS; i++)
            {
              ಆದರೆ (!ಒಳ_s1[i])
                {
                  ಪೂರ್ಣಾಂಕ ch = get_ಮುಂದಿನ (s2, ಖಾಲಿ);
                  affirm (ch != -1 || truncate_set1);
                  ಆದರೆ (ch == -1)
                    {
                      /* This will happen when tr is invoked like e.g.
                         tr -cs A-Za-z0-9 '\012'.  */
                      ನಿಲ್ಲಿಸು;
                    }
                  xlate[i] = ch;
                }
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ c1, c2;
          ಎಣಿಕೆ ಊರ್ಧ್ವ_ಅಧೋ_class class_s1;
          ಎಣಿಕೆ ಊರ್ಧ್ವ_ಅಧೋ_class class_s2;

          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          s1->state = BEGIN_STATE;
          s2->state = BEGIN_STATE;
          ಆಗ (true)
            {
              c1 = get_ಮುಂದಿನ (s1, &class_s1);
              c2 = get_ಮುಂದಿನ (s2, &class_s2);

              ಆದರೆ (class_s1 == UL_ಅಧೋ && class_s2 == UL_ಊರ್ಧ್ವ)
                {
                  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N_CHARS; i++)
                    ಆದರೆ (islower (i))
                      xlate[i] = ದೊಡ್ಡ_ಅಕ್ಷರ (i);
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (class_s1 == UL_ಊರ್ಧ್ವ && class_s2 == UL_ಅಧೋ)
                {
                  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N_CHARS; i++)
                    ಆದರೆ (isupper (i))
                      xlate[i] = tolower (i);
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  /* The following should have been checked by validate...  */
                  ಆದರೆ (c1 == -1 || c2 == -1)
                    ನಿಲ್ಲಿಸು;
                  xlate[c1] = c2;
                }

              /* When case-converting, skip the elements as an optimization.  */
              ಆದರೆ (class_s2 != UL_NONE)
                {
                  skip_construct (s1);
                  skip_construct (s2);
                }
            }
          affirm (c1 == -1 || truncate_set1);
        }
      ಆದರೆ (squeeze_repeats)
        {
          ಗಣ_initialize (s2, false, ಒಳ_squeeze_ಗಣ);
          squeeze_filter (io_buf, ಗಾತ್ರ io_buf, read_and_xlate);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆಗ (true)
            {
              size_t ಬೈಟುಗಳು_read = read_and_xlate (io_buf, ಗಾತ್ರ io_buf);
              ಆದರೆ (ಬೈಟುಗಳು_read == 0)
                ನಿಲ್ಲಿಸು;
              ಆದರೆ (fwrite (io_buf, 1, ಬೈಟುಗಳು_read, stdout) != ಬೈಟುಗಳು_read)
                write_error ();
            }
        }
    }

  ಆದರೆ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  main_exit (EXIT_SUCCESS);
}
