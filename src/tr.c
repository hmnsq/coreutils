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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xbinary-io.h"
#అన్తర్భావ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "tr"

#నిర్వచించు AUTHORS proper_name ("Jim Meyering")

గణన { N_CHARS = UCHAR_MAX + 1 };

/* An unsigned integer type big enough to hold a repeat count or an
   unsigned character.  POSIX requires support for repeat counts as
   high as 2**31 - 1.  Since repeat counts might need to expand to
   match the length of an argument string, we need at least size_t to
   avoid arbitrary internal limits.  It doesn't cost much to use
   uintmax_t, though.  */
రకనిర్వచనం uintmax_t లెక్క;

/* The value for Spec_list->state that indicates to
   get_next that it should initialize the tail pointer.
   Its value should be as large as possible to avoid conflict
   a valid value for the state field -- and that may be as
   large as any valid repeat_count.  */
#నిర్వచించు BEGIN_STATE (UINTMAX_MAX - 1)

/* The value for Spec_list->state that indicates to
   get_next that the element pointed to by Spec_list->tail is
   being considered for the first time on this pass through the
   list -- it indicates that get_next should make any necessary
   initializations.  */
#నిర్వచించు NEW_ELEMENT (BEGIN_STATE + 1)

/* The maximum possible repeat count.  Due to how the states are
   implemented, it can be as much as BEGIN_STATE.  */
#నిర్వచించు REPEAT_లెక్క_గరిష్ఠ BEGIN_STATE

/* The following (but not CC_NO_CLASS) are indices into the array of
   valid character class strings.  */
గణన Char_class
  {
    CC_ALNUM = 0, CC_ALPHA = 1, CC_BLANK = 2, CC_CNTRL = 3,
    CC_DIGIT = 4, CC_లేఖాచిత్రం = 5, CC_క్రింది = 6, CC_PRINT = 7,
    CC_PUNCT = 8, CC_SPACE = 9, CC_పై = 10, CC_XDIGIT = 11,
    CC_NO_CLASS = 9999
  };

/* Character class to which a character (returned by get_next) belonged;
   but it is set only if the construct from which the character was obtained
   was one of the character classes [:upper:] or [:lower:].  The value
   is used only when translating and then, only to make sure that upper
   and lower class constructs have the same relative positions in string1
   and string2.  */
గణన పై_క్రింది_class
  {
    UL_క్రింది,
    UL_పై,
    UL_NONE
  };

/* The type of a List_element.  See build_spec_list for more details.  */
గణన Range_element_type
  {
    RE_సాధారణ_CHAR,
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
నిర్మాణం List_element
  {
    గణన Range_element_type type;
    నిర్మాణం List_element *తదుపరి;
    సంఘం
      {
        గుర్తులేని అక్షరం సాధారణ_char;
        నిర్మాణం			/* unnamed */
          {
            గుర్తులేని అక్షరం మొదటి_char;
            గుర్తులేని అక్షరం చివరి_char;
          }
        range;
        గణన Char_class char_class;
        గుర్తులేని అక్షరం equiv_సంకేతం;
        నిర్మాణం			/* unnamed */
          {
            గుర్తులేని అక్షరం the_repeated_char;
            లెక్క repeat_లెక్క;
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
నిర్మాణం Spec_list
  {
    /* Points to the head of the list of range elements.
       The first struct is a dummy; its members are never used.  */
    నిర్మాణం List_element *తల;

    /* When appending, points to the last element.  When traversing via
       get_next(), points to the element to process next.  Setting
       Spec_list.state to the value BEGIN_STATE before calling get_next
       signals get_next to initialize tail to point to head->next.  */
    నిర్మాణం List_element *తోక;

    /* Used to save state between calls to get_next.  */
    లెక్క state;

    /* Length, in the sense that length ('a-z[:digit:]123abc')
       is 42 ( = 26 + 10 + 6).  */
    లెక్క నిడివి;

    /* The number of [c*] and [c*0] constructs that appear in this spec.  */
    size_t n_indefinite_repeats;

    /* If n_indefinite_repeats is nonzero, this points to the List_element
       corresponding to the last [c*] or [c*0] construct encountered in
       this spec.  Otherwise it is undefined.  */
    నిర్మాణం List_element *indefinite_repeat_element;

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
నిర్మాణం E_string
{
  అక్షరం *s;
  bool *escaped;
  size_t len;
};

/* Return nonzero if the Ith character of escaped string ES matches C
   and is not escaped itself.  */
స్థిర అంతఃవరుస bool
es_సరిపోలిక (నిర్మాణం E_string స్థిరరాశి *es, size_t i, అక్షరం c)
{
  తిరిగి es->s[i] == c && !es->escaped[i];
}

/* When true, each sequence in the input of a repeated character
   (call it c) is replaced (in the output) by a single occurrence of c
   for every c in the squeeze set.  */
స్థిర bool squeeze_repeats = false;

/* When true, removes characters in the delete set from input.  */
స్థిర bool delete = false;

/* Use the complement of set1 in place of set1.  */
స్థిర bool complement = false;

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
స్థిర bool truncate_set1 = false;

/* An alias for (!delete && non_option_args == 2).
   It is set in main and used there and in validate().  */
స్థిర bool translating;

స్థిర అక్షరం io_buf[BUFSIZ];

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి char_class_name[] =
{
  "alnum", "alpha", "blank", "cntrl", "digit", "graph",
  "lower", "print", "punct", "space", "upper", "xdigit"
};

/* Array of boolean values.  A character 'c' is a member of the
   squeeze set if and only if in_squeeze_set[c] is true.  The squeeze
   set is defined by the last (possibly, the only) string argument
   on the command line when the squeeze option is given.  */
స్థిర bool లోపల_squeeze_సమితి[N_CHARS];

/* Array of boolean values.  A character 'c' is a member of the
   delete set if and only if in_delete_set[c] is true.  The delete
   set is defined by the first (or only) string argument on the
   command line when the delete option is given.  */
స్థిర bool లోపల_delete_సమితి[N_CHARS];

/* Array of character values defining the translation (if any) that
   tr is to perform.  Translation is performed only when there are
   two specification strings and the delete switch is not given.  */
స్థిర అక్షరం xlate[N_CHARS];

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"complement", no_వాదం, ఖాళీ, 'c'},
  {"delete", no_వాదం, ఖాళీ, 'd'},
  {"squeeze-repeats", no_వాదం, ఖాళీ, 's'},
  {"truncate-set1", no_వాదం, ఖాళీ, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... STRING1 [STRING2]\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Translate, squeeze, and/or delete characters from standard ఆదానం,\n\
writing గమ్యం standard output.  STRING1 and STRING2 specify arrays యొక్క\n\
characters ARRAY1 and ARRAY2 that control the action.\n\
\n\
"), stdout);
      oputs (_("\
  -c, -C, --complement\n\
         use the complement యొక్క ARRAY1\n\
"));
      oputs (_("\
  -d, --delete\n\
         delete characters లోపల ARRAY1, చేయి not translate\n\
"));
      oputs (_("\
  -s, --squeeze-repeats\n\
         replace each sequence యొక్క a repeated character\n\
         that is listed లోపల the చివరి specified ARRAY,\n\
         with a single occurrence యొక్క that character\n\
"));
      oputs (_("\
  -t, --truncate-set1\n\
         మొదటి truncate ARRAY1 గమ్యం నిడివి యొక్క ARRAY2\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ARRAYs are specified as అక్షరశ్రేణులు యొక్క characters.  Most represent themselves.\n\
Interpreted sequences are:\n\
\n\
  \\NNN            character with octal విలువ NNN (1 గమ్యం 3 octal digits)\n\
  \\\\              backslash\n\
  \\a              audible BEL\n\
  \\b              backspace\n\
  \\f              form feed\n\
  \\n              new పంక్తి\n\
  \\r              తిరిగి\n\
  \\t              horizontal tab\n\
"), stdout);
     fputs (_("\
  \\v              vertical tab\n\
  CHAR1-CHAR2     all characters from CHAR1 గమ్యం CHAR2 లోపల ascending క్రమం\n\
  [CHAR*]         లోపల ARRAY2, copies యొక్క CHAR until నిడివి యొక్క ARRAY1\n\
  [CHAR*REPEAT]   REPEAT copies యొక్క CHAR, REPEAT octal ఒకవేళ starting with 0\n\
  [:alnum:]       all letters and digits\n\
  [:alpha:]       all letters\n\
  [:blank:]       all horizontal whitespace\n\
  [:cntrl:]       all control characters\n\
  [:digit:]       all digits\n\
"), stdout);
     fputs (_("\
  [:లేఖాచిత్రం:]       all printable characters, not including space\n\
  [:క్రింది:]       all క్రింది స్థితి letters\n\
  [:print:]       all printable characters, including space\n\
  [:punct:]       all punctuation characters\n\
  [:space:]       all horizontal or vertical whitespace\n\
  [:పై:]       all పై స్థితి letters\n\
  [:xdigit:]      all hexadecimal digits\n\
  [=CHAR=]        all characters which are equivalent గమ్యం CHAR\n\
"), stdout);
     fputs (_("\
\n\
Translation occurs ఒకవేళ -d is not given and both STRING1 and STRING2 appear.\n\
-t is only significant when translating.  ARRAY2 is extended గమ్యం నిడివి యొక్క\n\
ARRAY1 by repeating its చివరి character as necessary.  Excess characters\n\
యొక్క ARRAY2 are ignored.  Character classes expand లోపల unspecified క్రమం;\n\
అయితే translating, '[:lower:]' and '[:upper:]' may be వాడిన లోపల జతలు గమ్యం\n\
specify స్థితి conversion.  Squeezing occurs after translation or deletion.\n\
Arguments like '[...]' should be quoted, గమ్యం avoid potential shell globbing.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Return nonzero if the character C is a member of the
   equivalence class containing the character EQUIV_CLASS.  */

స్థిర అంతఃవరుస bool
is_equiv_class_member (గుర్తులేని అక్షరం equiv_class, గుర్తులేని అక్షరం c)
{
  తిరిగి (equiv_class == c);
}

/* Return true if the character C is a member of the
   character class CHAR_CLASS.  */

ATTRIBUTE_శుద్ధ
స్థిర bool
is_char_class_member (గణన Char_class char_class, గుర్తులేని అక్షరం c)
{
  పూర్ణాంకం ఫలితం;

  ఎంపిక (char_class)
    {
    స్థితి CC_ALNUM:
      ఫలితం = అక్షరమా_అంకెనా (c);
      ఆపు;
    స్థితి CC_ALPHA:
      ఫలితం = అక్షరమా (c);
      ఆపు;
    స్థితి CC_BLANK:
      ఫలితం = isblank (c);
      ఆపు;
    స్థితి CC_CNTRL:
      ఫలితం = iscntrl (c);
      ఆపు;
    స్థితి CC_DIGIT:
      ఫలితం = c_isdigit (c);
      ఆపు;
    స్థితి CC_లేఖాచిత్రం:
      ఫలితం = isgraph (c);
      ఆపు;
    స్థితి CC_క్రింది:
      ఫలితం = islower (c);
      ఆపు;
    స్థితి CC_PRINT:
      ఫలితం = isprint (c);
      ఆపు;
    స్థితి CC_PUNCT:
      ఫలితం = ispunct (c);
      ఆపు;
    స్థితి CC_SPACE:
      ఫలితం = ఖాళీనా (c);
      ఆపు;
    స్థితి CC_పై:
      ఫలితం = isupper (c);
      ఆపు;
    స్థితి CC_XDIGIT:
      ఫలితం = c_isxdigit (c);
      ఆపు;
    స్థితి CC_NO_CLASS: అప్రమేయ:
      affirm (false);
    }

  తిరిగి !! ఫలితం;
}

స్థిర శూన్య
es_free (నిర్మాణం E_string *es)
{
  విడుదల (es->s);
  విడుదల (es->escaped);
}

/* Perform the first pass over each range-spec argument S, converting all
   \c and \ddd escapes to their one-byte representations.  If an invalid
   quote sequence is found print an error message and return false;
   Otherwise set *ES to the resulting string and return true.
   The resulting array of characters may contain zero-bytes;
   however, on input, S is assumed to be null-terminated, and hence
   cannot contain actual (non-escaped) zero bytes.  */

స్థిర bool
unquote (అక్షరం స్థిరరాశి *s, నిర్మాణం E_string *es)
{
  size_t len = అక్షరశ్రేణి_పొడవు (s);

  es->s = xmalloc (len);
  es->escaped = xcalloc (len, పరిమాణం es->escaped[0]);

  గుర్తులేని పూర్ణాంకం j = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; s[i]; i++)
    {
      గుర్తులేని అక్షరం c;
      పూర్ణాంకం oct_digit;

      ఎంపిక (s[i])
        {
        స్థితి '\\':
          es->escaped[j] = true;
          ఎంపిక (s[i + 1])
            {
            స్థితి '\\':
              c = '\\';
              ఆపు;
            స్థితి 'a':
              c = '\a';
              ఆపు;
            స్థితి 'b':
              c = '\b';
              ఆపు;
            స్థితి 'f':
              c = '\f';
              ఆపు;
            స్థితి 'n':
              c = '\n';
              ఆపు;
            స్థితి 'r':
              c = '\r';
              ఆపు;
            స్థితి 't':
              c = '\t';
              ఆపు;
            స్థితి 'v':
              c = '\v';
              ఆపు;
            స్థితి '0':
            స్థితి '1':
            స్థితి '2':
            స్థితి '3':
            స్థితి '4':
            స్థితి '5':
            స్థితి '6':
            స్థితి '7':
              c = s[i + 1] - '0';
              oct_digit = s[i + 2] - '0';
              ఒకవేళ (0 <= oct_digit && oct_digit <= 7)
                {
                  c = 8 * c + oct_digit;
                  ++i;
                  oct_digit = s[i + 2] - '0';
                  ఒకవేళ (0 <= oct_digit && oct_digit <= 7)
                    {
                      ఒకవేళ (8 * c + oct_digit < N_CHARS)
                        {
                          c = 8 * c + oct_digit;
                          ++i;
                        }
                      లేకపోతే
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
              ఆపు;
            స్థితి '\0':
              error (0, 0, _("warning: an unescaped backslash "
                             "at end of string is not portable"));
              /* POSIX is not clear about this.  */
              es->escaped[j] = false;
              i--;
              c = '\\';
              ఆపు;
            అప్రమేయ:
              c = s[i + 1];
              ఆపు;
            }
          ++i;
          es->s[j++] = c;
          ఆపు;
        అప్రమేయ:
          es->s[j++] = s[i];
          ఆపు;
        }
    }
  es->len = j;
  తిరిగి true;
}

/* If CLASS_STR is a valid character class string, return its index
   in the global char_class_name array.  Otherwise, return CC_NO_CLASS.  */

ATTRIBUTE_శుద్ధ
స్థిర గణన Char_class
look_పైకి_char_class (అక్షరం స్థిరరాశి *class_str, size_t len)
{
  కొరకు (గణన Char_class i = 0; i < countof (char_class_name); i++)
    ఒకవేళ (STREQ_LEN (class_str, char_class_name[i], len)
        && అక్షరశ్రేణి_పొడవు (char_class_name[i]) == len)
      తిరిగి i;
  తిరిగి CC_NO_CLASS;
}

/* Return a newly allocated string with a printable version of C.
   This function is used solely for formatting error messages.  */

స్థిర అక్షరం *
make_printable_char (గుర్తులేని అక్షరం c)
{
  అక్షరం *buf = xmalloc (5);

  ఒకవేళ (isprint (c))
    {
      buf[0] = c;
      buf[1] = '\0';
    }
  లేకపోతే
    {
      sprintf (buf, "\\%03o", c);
    }
  తిరిగి buf;
}

/* Return a newly allocated copy of S which is suitable for printing.
   LEN is the number of characters in S.  Most non-printing
   (isprint) characters are represented by a backslash followed by
   3 octal digits.  However, the characters represented by \c escapes
   where c is one of [abfnrtv] are represented by their 2-character \c
   sequences.  This function is used solely for printing error messages.  */

స్థిర అక్షరం *
make_printable_str (అక్షరం స్థిరరాశి *s, size_t len)
{
  /* Worst case is that every character expands to a backslash
     followed by a 3-character octal escape sequence.  */
  అక్షరం *printable_buf = xnmalloc (len + 1, 4);
  అక్షరం *p = printable_buf;

  కొరకు (size_t i = 0; i < len; i++)
    {
      అక్షరం buf[5];
      అక్షరం స్థిరరాశి *tmp = ఖాళీ;
      గుర్తులేని అక్షరం c = s[i];

      ఎంపిక (c)
        {
        స్థితి '\\':
          tmp = "\\";
          ఆపు;
        స్థితి '\a':
          tmp = "\\a";
          ఆపు;
        స్థితి '\b':
          tmp = "\\b";
          ఆపు;
        స్థితి '\f':
          tmp = "\\f";
          ఆపు;
        స్థితి '\n':
          tmp = "\\n";
          ఆపు;
        స్థితి '\r':
          tmp = "\\r";
          ఆపు;
        స్థితి '\t':
          tmp = "\\t";
          ఆపు;
        స్థితి '\v':
          tmp = "\\v";
          ఆపు;
        అప్రమేయ:
          ఒకవేళ (isprint (c))
            {
              buf[0] = c;
              buf[1] = '\0';
            }
          లేకపోతే
            sprintf (buf, "\\%03o", c);
          tmp = buf;
          ఆపు;
        }
      p = stpcpy (p, tmp);
    }
  తిరిగి printable_buf;
}

/* Append a newly allocated structure representing a
   character C to the specification list LIST.  */

స్థిర శూన్య
append_సాధారణ_char (నిర్మాణం Spec_list *list, గుర్తులేని అక్షరం c)
{
  నిర్మాణం List_element *new = xmalloc (పరిమాణం *new);
  new->తదుపరి = ఖాళీ;
  new->type = RE_సాధారణ_CHAR;
  new->u.సాధారణ_char = c;
  list->తోక->తదుపరి = new;
  list->తోక = new;
}

/* Append a newly allocated structure representing the range
   of characters from FIRST to LAST to the specification list LIST.
   Return false if LAST precedes FIRST in the collating sequence,
   true otherwise.  This means that '[c-c]' is acceptable.  */

స్థిర bool
append_range (నిర్మాణం Spec_list *list, గుర్తులేని అక్షరం మొదటి, గుర్తులేని అక్షరం చివరి)
{
  ఒకవేళ (చివరి < మొదటి)
    {
      అక్షరం *tmp1 = make_printable_char (మొదటి);
      అక్షరం *tmp2 = make_printable_char (చివరి);

      error (0, 0,
       _("range-endpoints of '%s-%s' are in reverse collating sequence order"),
             tmp1, tmp2);
      విడుదల (tmp1);
      విడుదల (tmp2);
      తిరిగి false;
    }
  నిర్మాణం List_element *new = xmalloc (పరిమాణం *new);
  new->తదుపరి = ఖాళీ;
  new->type = RE_RANGE;
  new->u.range.మొదటి_char = మొదటి;
  new->u.range.చివరి_char = చివరి;
  list->తోక->తదుపరి = new;
  list->తోక = new;
  తిరిగి true;
}

/* If CHAR_CLASS_STR is a valid character class string, append a
   newly allocated structure representing that character class to the end
   of the specification list LIST and return true.  If CHAR_CLASS_STR is not
   a valid string return false.  */

స్థిర bool
append_char_class (నిర్మాణం Spec_list *list,
                   అక్షరం స్థిరరాశి *char_class_str, size_t len)
{
  గణన Char_class char_class = look_పైకి_char_class (char_class_str, len);
  ఒకవేళ (char_class == CC_NO_CLASS)
    తిరిగి false;
  నిర్మాణం List_element *new = xmalloc (పరిమాణం *new);
  new->తదుపరి = ఖాళీ;
  new->type = RE_CHAR_CLASS;
  new->u.char_class = char_class;
  list->తోక->తదుపరి = new;
  list->తోక = new;
  తిరిగి true;
}

/* Append a newly allocated structure representing a [c*n]
   repeated character construct to the specification list LIST.
   THE_CHAR is the single character to be repeated, and REPEAT_COUNT
   is a non-negative repeat count.  */

స్థిర శూన్య
append_repeated_char (నిర్మాణం Spec_list *list, గుర్తులేని అక్షరం the_char,
                      లెక్క repeat_లెక్క)
{
  నిర్మాణం List_element *new = xmalloc (పరిమాణం *new);
  new->తదుపరి = ఖాళీ;
  new->type = RE_REPEATED_CHAR;
  new->u.repeated_char.the_repeated_char = the_char;
  new->u.repeated_char.repeat_లెక్క = repeat_లెక్క;
  list->తోక->తదుపరి = new;
  list->తోక = new;
}

/* Given a string, EQUIV_CLASS_STR, from a [=str=] context and
   the length of that string, LEN, if LEN is exactly one, append
   a newly allocated structure representing the specified
   equivalence class to the specification list, LIST and return true.
   If LEN is not 1, return false.  */

స్థిర bool
append_equiv_class (నిర్మాణం Spec_list *list,
                    అక్షరం స్థిరరాశి *equiv_class_str, size_t len)
{
  ఒకవేళ (len != 1)
    తిరిగి false;

  నిర్మాణం List_element *new = xmalloc (పరిమాణం *new);
  new->తదుపరి = ఖాళీ;
  new->type = RE_EQUIV_CLASS;
  new->u.equiv_సంకేతం = *equiv_class_str;
  list->తోక->తదుపరి = new;
  list->తోక = new;
  తిరిగి true;
}

/* Search forward starting at START_IDX for the 2-char sequence
   (PRE_BRACKET_CHAR,']') in the string P of length P_LEN.  If such
   a sequence is found, set *RESULT_IDX to the index of the first
   character and return true.  Otherwise return false.  P may contain
   zero bytes.  */

స్థిర bool
కనుగొను_closing_delim (స్థిరరాశి నిర్మాణం E_string *es, size_t ప్రారంభం_idx,
                    అక్షరం pre_bracket_char, size_t *ఫలితం_idx)
{
  కొరకు (size_t i = ప్రారంభం_idx; i < es->len - 1; i++)
    ఒకవేళ (es->s[i] == pre_bracket_char && es->s[i + 1] == ']'
        && !es->escaped[i] && !es->escaped[i + 1])
      {
        *ఫలితం_idx = i;
        తిరిగి true;
      }
  తిరిగి false;
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

స్థిర పూర్ణాంకం
కనుగొను_bracketed_repeat (స్థిరరాశి నిర్మాణం E_string *es, size_t ప్రారంభం_idx,
                       గుర్తులేని అక్షరం *char_గమ్యం_repeat, లెక్క *repeat_లెక్క,
                       size_t *closing_bracket_idx)
{
  affirm (ప్రారంభం_idx + 1 < es->len);
  ఒకవేళ (!es_సరిపోలిక (es, ప్రారంభం_idx + 1, '*'))
    తిరిగి -1;

  కొరకు (size_t i = ప్రారంభం_idx + 2; i < es->len && !es->escaped[i]; i++)
    {
      ఒకవేళ (es->s[i] == ']')
        {
          size_t digit_str_len = i - ప్రారంభం_idx - 2;

          *char_గమ్యం_repeat = es->s[ప్రారంభం_idx];
          ఒకవేళ (digit_str_len == 0)
            {
              /* We've matched [c*] -- no explicit repeat count.  */
              *repeat_లెక్క = 0;
            }
          లేకపోతే
            {
              /* Here, we have found [c*s] where s should be a string
                 of octal (if it starts with '0') or decimal digits.  */
              అక్షరం స్థిరరాశి *digit_str = &es->s[ప్రారంభం_idx + 2];
              అక్షరం *d_చివర;
              ఒకవేళ ((xstrtoumax (digit_str, &d_చివర, *digit_str == '0' ? 8 : 10,
                               repeat_లెక్క, ఖాళీ)
                   != LONGINT_OK)
                  || REPEAT_లెక్క_గరిష్ఠ < *repeat_లెక్క
                  || digit_str + digit_str_len != d_చివర)
                {
                  అక్షరం *tmp = make_printable_str (digit_str, digit_str_len);
                  error (0, 0,
                         _("invalid repeat count %s in [c*n] construct"),
                         quote (tmp));
                  విడుదల (tmp);
                  తిరిగి -2;
                }
            }
          *closing_bracket_idx = i;
          తిరిగి 0;
        }
    }
  తిరిగి -1;			/* No bracket found.  */
}

/* Return true if the string at ES->s[IDX] matches the regular
   expression '\*[0-9]*]', false otherwise.  The string does not
   match if any of its characters are escaped.  */

ATTRIBUTE_శుద్ధ
స్థిర bool
నక్షత్రం_digits_closebracket (స్థిరరాశి నిర్మాణం E_string *es, size_t idx)
{
  ఒకవేళ (!es_సరిపోలిక (es, idx, '*'))
    తిరిగి false;

  కొరకు (size_t i = idx + 1; i < es->len; i++)
    ఒకవేళ (!c_isdigit (es->s[i]) || es->escaped[i])
      తిరిగి es_సరిపోలిక (es, i, ']');
  తిరిగి false;
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

స్థిర bool
build_spec_list (స్థిరరాశి నిర్మాణం E_string *es, నిర్మాణం Spec_list *ఫలితం)
{
  అక్షరం స్థిరరాశి *p = es->s;

  /* The main for-loop below recognizes the 4 multi-character constructs.
     A character that matches (in its context) none of the multi-character
     constructs is classified as 'normal'.  Since all multi-character
     constructs have at least 3 characters, any strings of length 2 or
     less are composed solely of normal characters.  Hence, the index of
     the outer for-loop runs only as far as LEN-2.  */
  size_t i;
  కొరకు (i = 0; i + 2 < es->len; /*ఖాళీలు*/)
    {
      ఒకవేళ (es_సరిపోలిక (es, i, '['))
        {
          bool matched_multi_char_construct;
          size_t closing_bracket_idx;
          గుర్తులేని అక్షరం char_గమ్యం_repeat;
          లెక్క repeat_లెక్క;
          పూర్ణాంకం err;

          matched_multi_char_construct = true;
          ఒకవేళ (es_సరిపోలిక (es, i + 1, ':') || es_సరిపోలిక (es, i + 1, '='))
            {
              size_t closing_delim_idx;

              ఒకవేళ (కనుగొను_closing_delim (es, i + 2, p[i + 1], &closing_delim_idx))
                {
                  size_t opnd_str_len = closing_delim_idx - 1 - (i + 2) + 1;
                  అక్షరం స్థిరరాశి *opnd_str = p + i + 2;

                  ఒకవేళ (opnd_str_len == 0)
                    {
                      ఒకవేళ (p[i + 1] == ':')
                        error (0, 0, _("missing character class name '[::]'"));
                      లేకపోతే
                        error (0, 0,
                               _("missing equivalence class character '[==]'"));
                      తిరిగి false;
                    }

                  ఒకవేళ (p[i + 1] == ':')
                    {
                      /* FIXME: big comment.  */
                      ఒకవేళ (!append_char_class (ఫలితం, opnd_str, opnd_str_len))
                        {
                          ఒకవేళ (నక్షత్రం_digits_closebracket (es, i + 2))
                            వెళ్ళు try_bracketed_repeat;
                          లేకపోతే
                            {
                              అక్షరం *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0, _("invalid character class %s"),
                                     quote (tmp));
                              విడుదల (tmp);
                              తిరిగి false;
                            }
                        }
                    }
                  లేకపోతే
                    {
                      /* FIXME: big comment.  */
                      ఒకవేళ (!append_equiv_class (ఫలితం, opnd_str, opnd_str_len))
                        {
                          ఒకవేళ (నక్షత్రం_digits_closebracket (es, i + 2))
                            వెళ్ళు try_bracketed_repeat;
                          లేకపోతే
                            {
                              అక్షరం *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0,
               _("%s: equivalence class operand must be a single character"),
                                     tmp);
                              విడుదల (tmp);
                              తిరిగి false;
                            }
                        }
                    }

                  i = closing_delim_idx + 2;
                  కొనసాగు;
                }
              /* Else fall through.  This could be [:*] or [=*].  */
            }

        try_bracketed_repeat:

          /* Determine whether this is a bracketed repeat range
             matching the RE \[.\*(dec_or_oct_number)?].  */
          err = కనుగొను_bracketed_repeat (es, i + 1, &char_గమ్యం_repeat,
                                       &repeat_లెక్క,
                                       &closing_bracket_idx);
          ఒకవేళ (err == 0)
            {
              append_repeated_char (ఫలితం, char_గమ్యం_repeat, repeat_లెక్క);
              i = closing_bracket_idx + 1;
            }
          లేకపోతే ఒకవేళ (err == -1)
            {
              matched_multi_char_construct = false;
            }
          లేకపోతే
            {
              /* Found a string that looked like [c*n] but the
                 numeric part was invalid.  */
              తిరిగి false;
            }

          ఒకవేళ (matched_multi_char_construct)
            కొనసాగు;

          /* We reach this point if P does not match [:str:], [=c=],
             [c*n], or [c*].  Now, see if P looks like a range '[-c'
             (from '[' to 'c').  */
        }

      /* Look ahead one char for ranges like a-z.  */
      ఒకవేళ (es_సరిపోలిక (es, i + 1, '-'))
        {
          ఒకవేళ (!append_range (ఫలితం, p[i], p[i + 2]))
            తిరిగి false;
          i += 3;
        }
      లేకపోతే
        {
          append_సాధారణ_char (ఫలితం, p[i]);
          ++i;
        }
    }

  /* Now handle the (2 or fewer) remaining characters p[i]..p[es->len - 1].  */
  కొరకు (; i < es->len; i++)
    append_సాధారణ_char (ఫలితం, p[i]);

  తిరిగి true;
}

/* Advance past the current construct.
   S->tail must be non-null.  */
స్థిర శూన్య
skip_construct (నిర్మాణం Spec_list *s)
{
  s->తోక = s->తోక->తదుపరి;
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

స్థిర పూర్ణాంకం
get_తదుపరి (నిర్మాణం Spec_list *s, గణన పై_క్రింది_class *class)
{
  నిర్మాణం List_element *p;
  పూర్ణాంకం return_val;
  పూర్ణాంకం i;

  ఒకవేళ (class)
    *class = UL_NONE;

  ఒకవేళ (s->state == BEGIN_STATE)
    {
      s->తోక = s->తల->తదుపరి;
      s->state = NEW_ELEMENT;
    }

  p = s->తోక;
  ఒకవేళ (p == ఖాళీ)
    తిరిగి -1;

  ఎంపిక (p->type)
    {
    స్థితి RE_సాధారణ_CHAR:
      return_val = p->u.సాధారణ_char;
      s->state = NEW_ELEMENT;
      s->తోక = p->తదుపరి;
      ఆపు;

    స్థితి RE_RANGE:
      ఒకవేళ (s->state == NEW_ELEMENT)
        s->state = p->u.range.మొదటి_char;
      లేకపోతే
        ++(s->state);
      return_val = s->state;
      ఒకవేళ (s->state == p->u.range.చివరి_char)
        {
          s->తోక = p->తదుపరి;
          s->state = NEW_ELEMENT;
        }
      ఆపు;

    స్థితి RE_CHAR_CLASS:
      ఒకవేళ (class)
        {
          ఎంపిక (+p->u.char_class)
            {
            స్థితి CC_క్రింది:
              *class = UL_క్రింది;
              ఆపు;
            స్థితి CC_పై:
              *class = UL_పై;
              ఆపు;
            }
        }

      ఒకవేళ (s->state == NEW_ELEMENT)
        {
          కొరకు (i = 0; i < N_CHARS; i++)
            ఒకవేళ (is_char_class_member (p->u.char_class, i))
              ఆపు;
          affirm (i < N_CHARS);
          s->state = i;
        }
      assure (is_char_class_member (p->u.char_class, s->state));
      return_val = s->state;
      కొరకు (i = s->state + 1; i < N_CHARS; i++)
        ఒకవేళ (is_char_class_member (p->u.char_class, i))
          ఆపు;
      ఒకవేళ (i < N_CHARS)
        s->state = i;
      లేకపోతే
        {
          s->తోక = p->తదుపరి;
          s->state = NEW_ELEMENT;
        }
      ఆపు;

    స్థితి RE_EQUIV_CLASS:
      /* FIXME: this assumes that each character is alone in its own
         equivalence class (which appears to be correct for my
         LC_COLLATE.  But I don't know of any function that allows
         one to determine a character's equivalence class.  */

      return_val = p->u.equiv_సంకేతం;
      s->state = NEW_ELEMENT;
      s->తోక = p->తదుపరి;
      ఆపు;

    స్థితి RE_REPEATED_CHAR:
      /* Here, a repeat count of n == 0 means don't repeat at all.  */
      ఒకవేళ (p->u.repeated_char.repeat_లెక్క == 0)
        {
          s->తోక = p->తదుపరి;
          s->state = NEW_ELEMENT;
          return_val = get_తదుపరి (s, class);
        }
      లేకపోతే
        {
          ఒకవేళ (s->state == NEW_ELEMENT)
            {
              s->state = 0;
            }
          ++(s->state);
          return_val = p->u.repeated_char.the_repeated_char;
          ఒకవేళ (s->state == p->u.repeated_char.repeat_లెక్క)
            {
              s->తోక = p->తదుపరి;
              s->state = NEW_ELEMENT;
            }
        }
      ఆపు;

    అప్రమేయ:
      unreachable ();
    }

  తిరిగి return_val;
}

/* This is a minor kludge.  This function is called from
   get_spec_stats to determine the cardinality of a set derived
   from a complemented string.  It's a kludge in that some of the
   same operations are (duplicated) performed in set_initialize.  */

స్థిర పూర్ణాంకం
card_యొక్క_complement (నిర్మాణం Spec_list *s)
{
  పూర్ణాంకం c;
  పూర్ణాంకం cardinality = N_CHARS;
  bool లోపల_సమితి[N_CHARS] = {0};

  s->state = BEGIN_STATE;
  అయితే ((c = get_తదుపరి (s, ఖాళీ)) != -1)
    {
      cardinality -= (!లోపల_సమితి[c]);
      లోపల_సమితి[c] = true;
    }
  తిరిగి cardinality;
}

/* Discard the lengths associated with a case conversion,
   as using the actual number of upper or lower case characters
   is problematic when they don't match in some locales.
   Also ensure the case conversion classes in string2 are
   aligned correctly with those in string1.
   Note POSIX says the behavior of 'tr "[:upper:]" "[:upper:]"'
   is undefined.  Therefore we allow it (unlike Solaris)
   and treat it as a no-op.  */

స్థిర శూన్య
validate_case_classes (నిర్మాణం Spec_list *s1, నిర్మాణం Spec_list *s2)
{
  size_t n_పై = 0;
  size_t n_క్రింది = 0;
  పూర్ణాంకం c1 = 0;
  పూర్ణాంకం c2 = 0;
  MAYBE_వాడని లెక్క old_s1_len = s1->నిడివి, old_s2_len = s2->నిడివి;
  నిర్మాణం List_element *s1_తోక = s1->తోక;
  నిర్మాణం List_element *s2_తోక = s2->తోక;
  bool s1_new_element = true;
  bool s2_new_element = true;

  ఒకవేళ (complement || !s2->has_char_class)
    తిరిగి;

  కొరకు (పూర్ణాంకం i = 0; i < N_CHARS; i++)
    {
      ఒకవేళ (isupper (i))
        n_పై++;
      ఒకవేళ (islower (i))
        n_క్రింది++;
    }

  s1->state = BEGIN_STATE;
  s2->state = BEGIN_STATE;

  అయితే (c1 != -1 && c2 != -1)
    {
      గణన పై_క్రింది_class class_s1, class_s2;

      c1 = get_తదుపరి (s1, &class_s1);
      c2 = get_తదుపరి (s2, &class_s2);

      /* If c2 transitions to a new case class, then
         c1 must also transition at the same time.  */
      ఒకవేళ (s2_new_element && class_s2 != UL_NONE
          && !(s1_new_element && class_s1 != UL_NONE))
        error (EXIT_FAILURE, 0,
               _("misaligned [:upper:] and/or [:lower:] construct"));

      /* If case converting, quickly skip over the elements.  */
      ఒకవేళ (class_s2 != UL_NONE)
        {
          skip_construct (s1);
          skip_construct (s2);
          /* Discount insignificant/problematic lengths.  */
          s1->నిడివి -= (class_s1 == UL_పై ? n_పై : n_క్రింది) - 1;
          s2->నిడివి -= (class_s2 == UL_పై ? n_పై : n_క్రింది) - 1;
        }

      s1_new_element = s1->state == NEW_ELEMENT; /* Next element is new.  */
      s2_new_element = s2->state == NEW_ELEMENT; /* Next element is new.  */
    }

  affirm (old_s1_len >= s1->నిడివి && old_s2_len >= s2->నిడివి);

  s1->తోక = s1_తోక;
  s2->తోక = s2_తోక;
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

స్థిర శూన్య
get_spec_stats (నిర్మాణం Spec_list *s)
{
  లెక్క నిడివి = 0;

  s->n_indefinite_repeats = 0;
  s->has_equiv_class = false;
  s->has_restricted_char_class = false;
  s->has_char_class = false;
  కొరకు (నిర్మాణం List_element *p = s->తల->తదుపరి; p; p = p->తదుపరి)
    {
      లెక్క len = 0;

      ఎంపిక (p->type)
        {
        స్థితి RE_సాధారణ_CHAR:
          len = 1;
          ఆపు;

        స్థితి RE_RANGE:
          affirm (p->u.range.చివరి_char >= p->u.range.మొదటి_char);
          len = p->u.range.చివరి_char - p->u.range.మొదటి_char + 1;
          ఆపు;

        స్థితి RE_CHAR_CLASS:
          s->has_char_class = true;
          కొరకు (పూర్ణాంకం i = 0; i < N_CHARS; i++)
            ఒకవేళ (is_char_class_member (p->u.char_class, i))
              ++len;
          ఎంపిక (+p->u.char_class)
            {
            స్థితి CC_పై:
            స్థితి CC_క్రింది:
              ఆపు;
            అప్రమేయ:
              s->has_restricted_char_class = true;
              ఆపు;
            }
          ఆపు;

        స్థితి RE_EQUIV_CLASS:
          కొరకు (పూర్ణాంకం i = 0; i < N_CHARS; i++)
            ఒకవేళ (is_equiv_class_member (p->u.equiv_సంకేతం, i))
              ++len;
          s->has_equiv_class = true;
          ఆపు;

        స్థితి RE_REPEATED_CHAR:
          ఒకవేళ (p->u.repeated_char.repeat_లెక్క > 0)
            len = p->u.repeated_char.repeat_లెక్క;
          లేకపోతే
            {
              s->indefinite_repeat_element = p;
              ++(s->n_indefinite_repeats);
            }
          ఆపు;

        అప్రమేయ:
          unreachable ();
        }

      /* Check for arithmetic overflow in computing length.  Also, reject
         any length greater than the maximum repeat count, in case the
         length is later used to compute the repeat count for an
         indefinite element.  */
      ఒకవేళ (ckd_కూడు (&నిడివి, నిడివి, len) || REPEAT_లెక్క_గరిష్ఠ < నిడివి)
        error (EXIT_FAILURE, 0, _("too many characters in set"));
    }

  s->నిడివి = నిడివి;
}

స్థిర శూన్య
get_s1_spec_stats (నిర్మాణం Spec_list *s1)
{
  get_spec_stats (s1);
  ఒకవేళ (complement)
    s1->నిడివి = card_యొక్క_complement (s1);
}

స్థిర శూన్య
get_s2_spec_stats (నిర్మాణం Spec_list *s2, లెక్క len_s1)
{
  get_spec_stats (s2);
  ఒకవేళ (len_s1 >= s2->నిడివి && s2->n_indefinite_repeats == 1)
    {
      s2->indefinite_repeat_element->u.repeated_char.repeat_లెక్క =
        len_s1 - s2->నిడివి;
      s2->నిడివి = len_s1;
    }
}

స్థిర శూన్య
spec_init (నిర్మాణం Spec_list *spec_list)
{
  నిర్మాణం List_element *new = xmalloc (పరిమాణం *new);
  spec_list->తల = spec_list->తోక = new;
  spec_list->తల->తదుపరి = ఖాళీ;
}

/* This function makes two passes over the argument string S.  The first
   one converts all \c and \ddd escapes to their one-byte representations.
   The second constructs a linked specification list, SPEC_LIST, of the
   characters and constructs that comprise the argument string.  If either
   of these passes detects an error, this function returns false.  */

స్థిర bool
parse_str (అక్షరం స్థిరరాశి *s, నిర్మాణం Spec_list *spec_list)
{
  నిర్మాణం E_string es;
  bool ok = unquote (s, &es) && build_spec_list (&es, spec_list);
  es_free (&es);
  తిరిగి ok;
}

/* Given two specification lists, S1 and S2, and assuming that
   S1->length > S2->length, append a single [c*n] element to S2 where c
   is the last character in the expansion of S2 and n is the difference
   between the two lengths.
   Upon successful completion, S2->length is set to S1->length.  The only
   way this function can fail to make S2 as long as S1 is when S2 has
   zero-length, since in that case, there is no last character to repeat.
   So S2->length is required to be at least 1.  */

స్థిర శూన్య
string2_extend (స్థిరరాశి నిర్మాణం Spec_list *s1, నిర్మాణం Spec_list *s2)
{
  నిర్మాణం List_element *p;
  గుర్తులేని అక్షరం char_గమ్యం_repeat;

  affirm (translating);
  affirm (s1->నిడివి > s2->నిడివి);
  affirm (s2->నిడివి > 0);

  p = s2->తోక;
  ఎంపిక (p->type)
    {
    స్థితి RE_సాధారణ_CHAR:
      char_గమ్యం_repeat = p->u.సాధారణ_char;
      ఆపు;
    స్థితి RE_RANGE:
      char_గమ్యం_repeat = p->u.range.చివరి_char;
      ఆపు;
    స్థితి RE_CHAR_CLASS:
      /* Note BSD allows extending of classes in string2.  For example:
           tr '[:upper:]0-9' '[:lower:]'
         That's not portable however, contradicts POSIX and is dependent
         on your collating sequence.  */
      error (EXIT_FAILURE, 0,
             _("when translating with string1 longer than string2,\n"
               "the latter string must not end with a character class"));

    స్థితి RE_REPEATED_CHAR:
      char_గమ్యం_repeat = p->u.repeated_char.the_repeated_char;
      ఆపు;

    స్థితి RE_EQUIV_CLASS: అప్రమేయ:
      /* This shouldn't happen, because validate exits with an error
         if it finds an equiv class in string2 when translating.  */
      affirm (false);
    }

  append_repeated_char (s2, char_గమ్యం_repeat, s1->నిడివి - s2->నిడివి);
  s2->నిడివి = s1->నిడివి;
}

/* Return true if S is a non-empty list in which exactly one
   character (but potentially, many instances of it) appears.
   E.g., [X*] or xxxxxxxx.  */

స్థిర bool
homogeneous_spec_list (నిర్మాణం Spec_list *s)
{
  పూర్ణాంకం b, c;

  s->state = BEGIN_STATE;

  ఒకవేళ ((b = get_తదుపరి (s, ఖాళీ)) == -1)
    తిరిగి false;

  అయితే ((c = get_తదుపరి (s, ఖాళీ)) != -1)
    ఒకవేళ (c != b)
      తిరిగి false;

  తిరిగి true;
}

/* Die with an error message if S1 and S2 describe strings that
   are not valid with the given command line switches.
   A side effect of this function is that if a valid [c*] or
   [c*0] construct appears in string2, it is converted to [c*n]
   with a value for n that makes s2->length == s1->length.  By
   the same token, if the --truncate-set1 option is not
   given, S2 may be extended.  */

స్థిర శూన్య
validate (నిర్మాణం Spec_list *s1, నిర్మాణం Spec_list *s2)
{
  get_s1_spec_stats (s1);
  ఒకవేళ (s1->n_indefinite_repeats > 0)
    error (EXIT_FAILURE, 0,
           _("the [c*] repeat construct may not appear in string1"));

  ఒకవేళ (s2)
    {
      get_s2_spec_stats (s2, s1->నిడివి);

      ఒకవేళ (s2->n_indefinite_repeats > 1)
        error (EXIT_FAILURE, 0,
               _("only one [c*] repeat construct may appear in string2"));

      ఒకవేళ (translating)
        {
          ఒకవేళ (s2->has_equiv_class)
            error (EXIT_FAILURE, 0,
                   _("[=c=] expressions may not appear in string2"
                     " when translating"));

          ఒకవేళ (s2->has_restricted_char_class)
            error (EXIT_FAILURE, 0,
                   _("when translating, the only character classes"
                     " that may appear in\n"
                     "string2 are 'upper' and 'lower'"));

          validate_case_classes (s1, s2);

          ఒకవేళ (s1->నిడివి > s2->నిడివి)
            {
              ఒకవేళ (!truncate_set1)
                {
                  /* string2 must be non-empty unless --truncate-set1 is
                     given or string1 is empty.  */

                  ఒకవేళ (s2->నిడివి == 0)
                    error (EXIT_FAILURE, 0,
                           _("when not truncating set1,"
                             " string2 must be non-empty"));
                  string2_extend (s1, s2);
                }
            }

          ఒకవేళ (complement && s1->has_char_class
              && ! (s2->నిడివి == s1->నిడివి && homogeneous_spec_list (s2)))
            error (EXIT_FAILURE, 0,
                   _("when translating with complemented character classes,\n"
                     "string2 must map all characters in the domain to one"));
        }
      లేకపోతే
        /* Not translating.  */
        {
          ఒకవేళ (s2->n_indefinite_repeats > 0)
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

స్థిర శూన్య
squeeze_filter (అక్షరం *buf, size_t కొలత, size_t (*reader) (అక్షరం *, size_t))
{
  /* A value distinct from any character that may have been stored in a
     buffer as the result of a block-read in the function squeeze_filter.  */
  స్థిరరాశి పూర్ణాంకం NOT_A_CHAR = గరిష్ఠ_పూర్ణాంకం;

  పూర్ణాంకం char_గమ్యం_squeeze = NOT_A_CHAR;
  size_t i = 0;
  size_t nr = 0;

  అయితే (true)
    {
      ఒకవేళ (i >= nr)
        {
          nr = reader (buf, కొలత);
          ఒకవేళ (nr == 0)
            ఆపు;
          i = 0;
        }

      size_t begin = i;

      ఒకవేళ (char_గమ్యం_squeeze == NOT_A_CHAR)
        {
          size_t బహిర్గతం_len;
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
          కొరకు (; i < nr && !లోపల_squeeze_సమితి[గమ్యం_uchar (buf[i])]; i += 2)
            కొనసాగు;

          /* There is a special case when i == nr and we've just
             skipped a character (the last one in buf) that is in
             the squeeze set.  */
          ఒకవేళ (i == nr && లోపల_squeeze_సమితి[గమ్యం_uchar (buf[i - 1])])
            --i;

          ఒకవేళ (i >= nr)
            బహిర్గతం_len = nr - begin;
          లేకపోతే
            {
              char_గమ్యం_squeeze = buf[i];
              /* We're about to output buf[begin..i].  */
              బహిర్గతం_len = i - begin + 1;

              /* But since we stepped by 2 in the loop above,
                 out_len may be one too large.  */
              ఒకవేళ (i > 0 && buf[i - 1] == char_గమ్యం_squeeze)
                --బహిర్గతం_len;

              /* Advance i to the index of first character to be
                 considered when looking for a char different from
                 char_to_squeeze.  */
              ++i;
            }
          ఒకవేళ (బహిర్గతం_len > 0
              && fwrite (&buf[begin], 1, బహిర్గతం_len, stdout) != బహిర్గతం_len)
            write_error ();
        }

      ఒకవేళ (char_గమ్యం_squeeze != NOT_A_CHAR)
        {
          /* Advance i to index of first char != char_to_squeeze
             (or to nr if all the rest of the characters in this
             buffer are the same as char_to_squeeze).  */
          కొరకు (; i < nr && buf[i] == char_గమ్యం_squeeze; i++)
            కొనసాగు;
          ఒకవేళ (i < nr)
            char_గమ్యం_squeeze = NOT_A_CHAR;
          /* If (i >= nr) we've squeezed the last character in this buffer.
             So now we have to read a new buffer and continue comparing
             characters against char_to_squeeze.  */
        }
    }
}

స్థిర size_t
plain_read (అక్షరం *buf, size_t కొలత)
{
  ssize_t nr = పఠ (STDIN_FILENO, buf, కొలత);
  ఒకవేళ (nr < 0)
    error (EXIT_FAILURE, errno, _("read error"));
  తిరిగి nr;
}

/* Read buffers of SIZE bytes from stdin until one is found that
   contains at least one character not in the delete set.  Store
   in the array BUF, all characters from that buffer that are not
   in the delete set, and return the number of characters saved
   or 0 upon EOF.  */

స్థిర size_t
read_and_delete (అక్షరం *buf, size_t కొలత)
{
  size_t n_saved;

  /* This enclosing do-while loop is to make sure that
     we don't return zero (indicating EOF) when we've
     just deleted all the characters in a buffer.  */
  చేయి
    {
      size_t nr = plain_read (buf, కొలత);

      ఒకవేళ (nr == 0)
        తిరిగి 0;

      /* This first loop may be a waste of code, but gives much
         better performance when no characters are deleted in
         the beginning of a buffer.  It just avoids the copying
         of buf[i] into buf[n_saved] when it would be a NOP.  */

      size_t i;
      కొరకు (i = 0; i < nr && !లోపల_delete_సమితి[గమ్యం_uchar (buf[i])]; i++)
        కొనసాగు;
      n_saved = i;

      కొరకు (++i; i < nr; i++)
        ఒకవేళ (!లోపల_delete_సమితి[గమ్యం_uchar (buf[i])])
          buf[n_saved++] = buf[i];
    }
  అయితే (n_saved == 0);

  తిరిగి n_saved;
}

/* Read at most SIZE bytes from stdin into the array BUF.  Then
   perform the in-place and one-to-one mapping specified by the global
   array 'xlate'.  Return the number of characters read, or 0 upon EOF.  */

స్థిర size_t
read_and_xlate (అక్షరం *buf, size_t కొలత)
{
  size_t బైట్లు_read = plain_read (buf, కొలత);

  కొరకు (size_t i = 0; i < బైట్లు_read; i++)
    buf[i] = xlate[గమ్యం_uchar (buf[i])];

  తిరిగి బైట్లు_read;
}

/* Initialize a boolean membership set, IN_SET, with the character
   values obtained by traversing the linked list of constructs S
   using the function 'get_next'.  IN_SET is expected to have been
   initialized to all zeros by the caller.  If COMPLEMENT_THIS_SET
   is true the resulting set is complemented.  */

స్థిర శూన్య
సమితి_initialize (నిర్మాణం Spec_list *s, bool complement_this_సమితి, bool *లోపల_సమితి)
{
  పూర్ణాంకం c;

  s->state = BEGIN_STATE;
  అయితే ((c = get_తదుపరి (s, ఖాళీ)) != -1)
    లోపల_సమితి[c] = true;
  ఒకవేళ (complement_this_సమితి)
    కొరకు (size_t i = 0; i < N_CHARS; i++)
      లోపల_సమితి[i] = (!లోపల_సమితి[i]);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం c;
  పూర్ణాంకం non_option_వాదనలు;
  పూర్ణాంకం min_operands;
  పూర్ణాంకం max_operands;
  నిర్మాణం Spec_list buf1, buf2;
  నిర్మాణం Spec_list *s1 = &buf1;
  నిర్మాణం Spec_list *s2 = &buf2;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((c = getopt_long (argc, argv, "+AcCdstu", long_options, ఖాళీ)) != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'A':
          /* Undocumented option, for compatibility with AIX.  */
          setlocale (LC_COLLATE, "C");
          setlocale (LC_CTYPE, "C");
          ఆపు;

        స్థితి 'c':
        స్థితి 'C':
          complement = true;
          ఆపు;

        స్థితి 'd':
          delete = true;
          ఆపు;

        స్థితి 's':
          squeeze_repeats = true;
          ఆపు;

        స్థితి 't':
          truncate_set1 = true;
          ఆపు;

        స్థితి 'u':
          /* Undocumented option, for compatibility with FreeBSD/macOS.  */
          setvbuf (stdout, ఖాళీ, _IONBF, 0);
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
          ఆపు;
        }
    }

  non_option_వాదనలు = argc - optind;
  translating = (non_option_వాదనలు == 2 && !delete);
  min_operands = 1 + (delete == squeeze_repeats);
  max_operands = 1 + (delete <= squeeze_repeats);

  ఒకవేళ (non_option_వాదనలు < min_operands)
    {
      ఒకవేళ (non_option_వాదనలు == 0)
        error (0, 0, _("missing operand"));
      లేకపోతే
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

  ఒకవేళ (max_operands < non_option_వాదనలు)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + max_operands]));
      ఒకవేళ (non_option_వాదనలు == 2)
        fprintf (stderr, "%s\n",
                 _("Only one string may be given when "
                   "deleting without squeezing repeats."));
      usage (EXIT_FAILURE);
    }

  spec_init (s1);
  ఒకవేళ (!parse_str (argv[optind], s1))
    main_exit (EXIT_FAILURE);

  ఒకవేళ (non_option_వాదనలు == 2)
    {
      spec_init (s2);
      ఒకవేళ (!parse_str (argv[optind + 1], s2))
        main_exit (EXIT_FAILURE);
    }
  లేకపోతే
    s2 = ఖాళీ;

  validate (s1, s2);

  /* Use binary I/O, since 'tr' is sometimes used to transliterate
     non-printable characters, or characters which are stripped away
     by text-mode reads (like CR and ^Z).  */
  xset_ద్విమాన_బహుళకం (STDIN_FILENO, O_ద్విమాన);
  xset_ద్విమాన_బహుళకం (STDOUT_FILENO, O_ద్విమాన);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  ఒకవేళ (squeeze_repeats && non_option_వాదనలు == 1)
    {
      సమితి_initialize (s1, complement, లోపల_squeeze_సమితి);
      squeeze_filter (io_buf, పరిమాణం io_buf, plain_read);
    }
  లేకపోతే ఒకవేళ (delete && non_option_వాదనలు == 1)
    {
      సమితి_initialize (s1, complement, లోపల_delete_సమితి);

      అయితే (true)
        {
          size_t nr = read_and_delete (io_buf, పరిమాణం io_buf);
          ఒకవేళ (nr == 0)
            ఆపు;
          ఒకవేళ (fwrite (io_buf, 1, nr, stdout) != nr)
            write_error ();
        }
    }
  లేకపోతే ఒకవేళ (squeeze_repeats && delete && non_option_వాదనలు == 2)
    {
      సమితి_initialize (s1, complement, లోపల_delete_సమితి);
      సమితి_initialize (s2, false, లోపల_squeeze_సమితి);
      squeeze_filter (io_buf, పరిమాణం io_buf, read_and_delete);
    }
  లేకపోతే ఒకవేళ (translating)
    {
      ఒకవేళ (complement)
        {
          bool *లోపల_s1 = లోపల_delete_సమితి;

          సమితి_initialize (s1, false, లోపల_s1);
          s2->state = BEGIN_STATE;
          కొరకు (పూర్ణాంకం i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          కొరకు (పూర్ణాంకం i = 0; i < N_CHARS; i++)
            {
              ఒకవేళ (!లోపల_s1[i])
                {
                  పూర్ణాంకం ch = get_తదుపరి (s2, ఖాళీ);
                  affirm (ch != -1 || truncate_set1);
                  ఒకవేళ (ch == -1)
                    {
                      /* This will happen when tr is invoked like e.g.
                         tr -cs A-Za-z0-9 '\012'.  */
                      ఆపు;
                    }
                  xlate[i] = ch;
                }
            }
        }
      లేకపోతే
        {
          పూర్ణాంకం c1, c2;
          గణన పై_క్రింది_class class_s1;
          గణన పై_క్రింది_class class_s2;

          కొరకు (పూర్ణాంకం i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          s1->state = BEGIN_STATE;
          s2->state = BEGIN_STATE;
          అయితే (true)
            {
              c1 = get_తదుపరి (s1, &class_s1);
              c2 = get_తదుపరి (s2, &class_s2);

              ఒకవేళ (class_s1 == UL_క్రింది && class_s2 == UL_పై)
                {
                  కొరకు (పూర్ణాంకం i = 0; i < N_CHARS; i++)
                    ఒకవేళ (islower (i))
                      xlate[i] = పెద్ద_అక్షరం (i);
                }
              లేకపోతే ఒకవేళ (class_s1 == UL_పై && class_s2 == UL_క్రింది)
                {
                  కొరకు (పూర్ణాంకం i = 0; i < N_CHARS; i++)
                    ఒకవేళ (isupper (i))
                      xlate[i] = tolower (i);
                }
              లేకపోతే
                {
                  /* The following should have been checked by validate...  */
                  ఒకవేళ (c1 == -1 || c2 == -1)
                    ఆపు;
                  xlate[c1] = c2;
                }

              /* When case-converting, skip the elements as an optimization.  */
              ఒకవేళ (class_s2 != UL_NONE)
                {
                  skip_construct (s1);
                  skip_construct (s2);
                }
            }
          affirm (c1 == -1 || truncate_set1);
        }
      ఒకవేళ (squeeze_repeats)
        {
          సమితి_initialize (s2, false, లోపల_squeeze_సమితి);
          squeeze_filter (io_buf, పరిమాణం io_buf, read_and_xlate);
        }
      లేకపోతే
        {
          అయితే (true)
            {
              size_t బైట్లు_read = read_and_xlate (io_buf, పరిమాణం io_buf);
              ఒకవేళ (బైట్లు_read == 0)
                ఆపు;
              ఒకవేళ (fwrite (io_buf, 1, బైట్లు_read, stdout) != బైట్లు_read)
                write_error ();
            }
        }
    }

  ఒకవేళ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  main_exit (EXIT_SUCCESS);
}
