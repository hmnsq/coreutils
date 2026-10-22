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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xbinary-io.h"
#अन्तर्भाव "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "tr"

#परिभाषय AUTHORS proper_name ("Jim Meyering")

गणना { N_CHARS = UCHAR_MAX + 1 };

/* An unsigned integer type big enough to hold a repeat count or an
   unsigned character.  POSIX requires support for repeat counts as
   high as 2**31 - 1.  Since repeat counts might need to expand to
   match the length of an argument string, we need at least size_t to
   avoid arbitrary internal limits.  It doesn't cost much to use
   uintmax_t, though.  */
प्रकारनिर्देश uintmax_t गणनम्;

/* The value for Spec_list->state that indicates to
   get_next that it should initialize the tail pointer.
   Its value should be as large as possible to avoid conflict
   a valid value for the state field -- and that may be as
   large as any valid repeat_count.  */
#परिभाषय BEGIN_STATE (UINTMAX_MAX - 1)

/* The value for Spec_list->state that indicates to
   get_next that the element pointed to by Spec_list->tail is
   being considered for the first time on this pass through the
   list -- it indicates that get_next should make any necessary
   initializations.  */
#परिभाषय NEW_ELEMENT (BEGIN_STATE + 1)

/* The maximum possible repeat count.  Due to how the states are
   implemented, it can be as much as BEGIN_STATE.  */
#परिभाषय REPEAT_गणनम्_अधिकतमम् BEGIN_STATE

/* The following (but not CC_NO_CLASS) are indices into the array of
   valid character class strings.  */
गणना Char_class
  {
    CC_ALNUM = 0, CC_ALPHA = 1, CC_BLANK = 2, CC_CNTRL = 3,
    CC_DIGIT = 4, CC_GRAPH = 5, CC_अधस्तनम् = 6, CC_PRINT = 7,
    CC_PUNCT = 8, CC_SPACE = 9, CC_ऊर्ध्वस्थम् = 10, CC_XDIGIT = 11,
    CC_NO_CLASS = 9999
  };

/* Character class to which a character (returned by get_next) belonged;
   but it is set only if the construct from which the character was obtained
   was one of the character classes [:upper:] or [:lower:].  The value
   is used only when translating and then, only to make sure that upper
   and lower class constructs have the same relative positions in string1
   and string2.  */
गणना ऊर्ध्वस्थम्_अधस्तनम्_class
  {
    UL_अधस्तनम्,
    UL_ऊर्ध्वस्थम्,
    UL_NONE
  };

/* The type of a List_element.  See build_spec_list for more details.  */
गणना Range_element_type
  {
    RE_प्रसामान्यम्_CHAR,
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
संरचना List_element
  {
    गणना Range_element_type type;
    संरचना List_element *अग्र;
    सङ्घ
      {
        अचिह्नित वर्ण प्रसामान्यम्_char;
        संरचना			/* unnamed */
          {
            अचिह्नित वर्ण first_char;
            अचिह्नित वर्ण चरमः_char;
          }
        range;
        गणना Char_class char_class;
        अचिह्नित वर्ण equiv_संहिता;
        संरचना			/* unnamed */
          {
            अचिह्नित वर्ण the_repeated_char;
            गणनम् repeat_गणनम्;
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
संरचना Spec_list
  {
    /* Points to the head of the list of range elements.
       The first struct is a dummy; its members are never used.  */
    संरचना List_element *शीर्ष;

    /* When appending, points to the last element.  When traversing via
       get_next(), points to the element to process next.  Setting
       Spec_list.state to the value BEGIN_STATE before calling get_next
       signals get_next to initialize tail to point to head->next.  */
    संरचना List_element *पुच्छ;

    /* Used to save state between calls to get_next.  */
    गणनम् state;

    /* Length, in the sense that length ('a-z[:digit:]123abc')
       is 42 ( = 26 + 10 + 6).  */
    गणनम् दीर्घता;

    /* The number of [c*] and [c*0] constructs that appear in this spec.  */
    size_t n_indefinite_repeats;

    /* If n_indefinite_repeats is nonzero, this points to the List_element
       corresponding to the last [c*] or [c*0] construct encountered in
       this spec.  Otherwise it is undefined.  */
    संरचना List_element *indefinite_repeat_element;

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
संरचना E_string
{
  वर्ण *s;
  bool *escaped;
  size_t len;
};

/* Return nonzero if the Ith character of escaped string ES matches C
   and is not escaped itself.  */
स्थिर अन्तःस्थ bool
es_मेलनम् (संरचना E_string ध्रुव *es, size_t i, वर्ण c)
{
  प्रत्यागम es->s[i] == c && !es->escaped[i];
}

/* When true, each sequence in the input of a repeated character
   (call it c) is replaced (in the output) by a single occurrence of c
   for every c in the squeeze set.  */
स्थिर bool squeeze_repeats = false;

/* When true, removes characters in the delete set from input.  */
स्थिर bool delete = false;

/* Use the complement of set1 in place of set1.  */
स्थिर bool complement = false;

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
स्थिर bool truncate_set1 = false;

/* An alias for (!delete && non_option_args == 2).
   It is set in main and used there and in validate().  */
स्थिर bool translating;

स्थिर वर्ण io_buf[BUFSIZ];

स्थिर वर्ण ध्रुव *ध्रुव char_class_name[] =
{
  "alnum", "alpha", "blank", "cntrl", "digit", "graph",
  "lower", "print", "punct", "space", "upper", "xdigit"
};

/* Array of boolean values.  A character 'c' is a member of the
   squeeze set if and only if in_squeeze_set[c] is true.  The squeeze
   set is defined by the last (possibly, the only) string argument
   on the command line when the squeeze option is given.  */
स्थिर bool अन्तर्_squeeze_समुच्चयः[N_CHARS];

/* Array of boolean values.  A character 'c' is a member of the
   delete set if and only if in_delete_set[c] is true.  The delete
   set is defined by the first (or only) string argument on the
   command line when the delete option is given.  */
स्थिर bool अन्तर्_delete_समुच्चयः[N_CHARS];

/* Array of character values defining the translation (if any) that
   tr is to perform.  Translation is performed only when there are
   two specification strings and the delete switch is not given.  */
स्थिर वर्ण xlate[N_CHARS];

स्थिर संरचना option ध्रुव long_options[] =
{
  {"complement", no_तर्कः, रिक्त, 'c'},
  {"delete", no_तर्कः, रिक्त, 'd'},
  {"squeeze-repeats", no_तर्कः, रिक्त, 's'},
  {"truncate-set1", no_तर्कः, रिक्त, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... STRING1 [STRING2]\n\
"),
              क्रमादेशः_name);
      fputs (_("\
Translate, squeeze, and/or delete characters from standard निवेशः,\n\
writing गन्तव्यम् standard output.  STRING1 and STRING2 specify arrays सम्बन्धि\n\
characters ARRAY1 and ARRAY2 that control the action.\n\
\n\
"), stdout);
      oputs (_("\
  -c, -C, --complement\n\
         use the complement सम्बन्धि ARRAY1\n\
"));
      oputs (_("\
  -d, --delete\n\
         delete characters अन्तर् ARRAY1, कुरु not translate\n\
"));
      oputs (_("\
  -s, --squeeze-repeats\n\
         replace each sequence सम्बन्धि a repeated character\n\
         that is listed अन्तर् the चरमः specified ARRAY,\n\
         with a single occurrence सम्बन्धि that character\n\
"));
      oputs (_("\
  -t, --truncate-set1\n\
         first truncate ARRAY1 गन्तव्यम् दीर्घता सम्बन्धि ARRAY2\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ARRAYs are specified as वाक्यानि सम्बन्धि characters.  Most represent themselves.\n\
Interpreted sequences are:\n\
\n\
  \\NNN            character with octal मानम् NNN (1 गन्तव्यम् 3 octal digits)\n\
  \\\\              backslash\n\
  \\a              audible BEL\n\
  \\b              backspace\n\
  \\f              form feed\n\
  \\n              new पङ्क्तिः\n\
  \\r              प्रत्यागम\n\
  \\t              horizontal tab\n\
"), stdout);
     fputs (_("\
  \\v              vertical tab\n\
  CHAR1-CHAR2     all characters from CHAR1 गन्तव्यम् CHAR2 अन्तर् ascending क्रमः\n\
  [CHAR*]         अन्तर् ARRAY2, copies सम्बन्धि CHAR until दीर्घता सम्बन्धि ARRAY1\n\
  [CHAR*REPEAT]   REPEAT copies सम्बन्धि CHAR, REPEAT octal यदि starting with 0\n\
  [:alnum:]       all letters and digits\n\
  [:alpha:]       all letters\n\
  [:blank:]       all horizontal whitespace\n\
  [:cntrl:]       all control characters\n\
  [:digit:]       all digits\n\
"), stdout);
     fputs (_("\
  [:graph:]       all printable characters, not including space\n\
  [:अधस्तनम्:]       all अधस्तनम् पक्ष letters\n\
  [:print:]       all printable characters, including space\n\
  [:punct:]       all punctuation characters\n\
  [:space:]       all horizontal or vertical whitespace\n\
  [:ऊर्ध्वस्थम्:]       all ऊर्ध्वस्थम् पक्ष letters\n\
  [:xdigit:]      all hexadecimal digits\n\
  [=CHAR=]        all characters which are equivalent गन्तव्यम् CHAR\n\
"), stdout);
     fputs (_("\
\n\
Translation occurs यदि -d is not given and both STRING1 and STRING2 appear.\n\
-t is only significant when translating.  ARRAY2 is extended गन्तव्यम् दीर्घता सम्बन्धि\n\
ARRAY1 by repeating its चरमः character as necessary.  Excess characters\n\
सम्बन्धि ARRAY2 are ignored.  Character classes expand अन्तर् unspecified क्रमः;\n\
यावत् translating, '[:lower:]' and '[:upper:]' may be प्रयुक्तम् अन्तर् युग्मानि गन्तव्यम्\n\
specify पक्ष conversion.  Squeezing occurs after translation or deletion.\n\
Arguments like '[...]' should be quoted, गन्तव्यम् avoid potential shell globbing.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Return nonzero if the character C is a member of the
   equivalence class containing the character EQUIV_CLASS.  */

स्थिर अन्तःस्थ bool
is_equiv_class_member (अचिह्नित वर्ण equiv_class, अचिह्नित वर्ण c)
{
  प्रत्यागम (equiv_class == c);
}

/* Return true if the character C is a member of the
   character class CHAR_CLASS.  */

ATTRIBUTE_विशुद्धम्
स्थिर bool
is_char_class_member (गणना Char_class char_class, अचिह्नित वर्ण c)
{
  पूर्णाङ्क परिणाम;

  विकल्प (char_class)
    {
    पक्ष CC_ALNUM:
      परिणाम = अक्षरम्_अङ्कः_वा (c);
      भङ्ग;
    पक्ष CC_ALPHA:
      परिणाम = अक्षरम्_वा (c);
      भङ्ग;
    पक्ष CC_BLANK:
      परिणाम = isblank (c);
      भङ्ग;
    पक्ष CC_CNTRL:
      परिणाम = iscntrl (c);
      भङ्ग;
    पक्ष CC_DIGIT:
      परिणाम = c_isdigit (c);
      भङ्ग;
    पक्ष CC_GRAPH:
      परिणाम = isgraph (c);
      भङ्ग;
    पक्ष CC_अधस्तनम्:
      परिणाम = islower (c);
      भङ्ग;
    पक्ष CC_PRINT:
      परिणाम = isprint (c);
      भङ्ग;
    पक्ष CC_PUNCT:
      परिणाम = ispunct (c);
      भङ्ग;
    पक्ष CC_SPACE:
      परिणाम = रिक्तम्_वा (c);
      भङ्ग;
    पक्ष CC_ऊर्ध्वस्थम्:
      परिणाम = isupper (c);
      भङ्ग;
    पक्ष CC_XDIGIT:
      परिणाम = c_isxdigit (c);
      भङ्ग;
    पक्ष CC_NO_CLASS: स्वभाव:
      affirm (false);
    }

  प्रत्यागम !! परिणाम;
}

स्थिर शून्य
es_free (संरचना E_string *es)
{
  मुक्त (es->s);
  मुक्त (es->escaped);
}

/* Perform the first pass over each range-spec argument S, converting all
   \c and \ddd escapes to their one-byte representations.  If an invalid
   quote sequence is found print an error message and return false;
   Otherwise set *ES to the resulting string and return true.
   The resulting array of characters may contain zero-bytes;
   however, on input, S is assumed to be null-terminated, and hence
   cannot contain actual (non-escaped) zero bytes.  */

स्थिर bool
unquote (वर्ण ध्रुव *s, संरचना E_string *es)
{
  size_t len = वाक्यदैर्घ्यम् (s);

  es->s = xmalloc (len);
  es->escaped = xcalloc (len, परिमाण es->escaped[0]);

  अचिह्नित पूर्णाङ्क j = 0;
  प्रति (अचिह्नित पूर्णाङ्क i = 0; s[i]; i++)
    {
      अचिह्नित वर्ण c;
      पूर्णाङ्क oct_digit;

      विकल्प (s[i])
        {
        पक्ष '\\':
          es->escaped[j] = true;
          विकल्प (s[i + 1])
            {
            पक्ष '\\':
              c = '\\';
              भङ्ग;
            पक्ष 'a':
              c = '\a';
              भङ्ग;
            पक्ष 'b':
              c = '\b';
              भङ्ग;
            पक्ष 'f':
              c = '\f';
              भङ्ग;
            पक्ष 'n':
              c = '\n';
              भङ्ग;
            पक्ष 'r':
              c = '\r';
              भङ्ग;
            पक्ष 't':
              c = '\t';
              भङ्ग;
            पक्ष 'v':
              c = '\v';
              भङ्ग;
            पक्ष '0':
            पक्ष '1':
            पक्ष '2':
            पक्ष '3':
            पक्ष '4':
            पक्ष '5':
            पक्ष '6':
            पक्ष '7':
              c = s[i + 1] - '0';
              oct_digit = s[i + 2] - '0';
              यदि (0 <= oct_digit && oct_digit <= 7)
                {
                  c = 8 * c + oct_digit;
                  ++i;
                  oct_digit = s[i + 2] - '0';
                  यदि (0 <= oct_digit && oct_digit <= 7)
                    {
                      यदि (8 * c + oct_digit < N_CHARS)
                        {
                          c = 8 * c + oct_digit;
                          ++i;
                        }
                      अन्यथा
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
              भङ्ग;
            पक्ष '\0':
              error (0, 0, _("warning: an unescaped backslash "
                             "at end of string is not portable"));
              /* POSIX is not clear about this.  */
              es->escaped[j] = false;
              i--;
              c = '\\';
              भङ्ग;
            स्वभाव:
              c = s[i + 1];
              भङ्ग;
            }
          ++i;
          es->s[j++] = c;
          भङ्ग;
        स्वभाव:
          es->s[j++] = s[i];
          भङ्ग;
        }
    }
  es->len = j;
  प्रत्यागम true;
}

/* If CLASS_STR is a valid character class string, return its index
   in the global char_class_name array.  Otherwise, return CC_NO_CLASS.  */

ATTRIBUTE_विशुद्धम्
स्थिर गणना Char_class
look_ऊर्ध्वम्_char_class (वर्ण ध्रुव *class_str, size_t len)
{
  प्रति (गणना Char_class i = 0; i < countof (char_class_name); i++)
    यदि (STREQ_LEN (class_str, char_class_name[i], len)
        && वाक्यदैर्घ्यम् (char_class_name[i]) == len)
      प्रत्यागम i;
  प्रत्यागम CC_NO_CLASS;
}

/* Return a newly allocated string with a printable version of C.
   This function is used solely for formatting error messages.  */

स्थिर वर्ण *
make_printable_char (अचिह्नित वर्ण c)
{
  वर्ण *buf = xmalloc (5);

  यदि (isprint (c))
    {
      buf[0] = c;
      buf[1] = '\0';
    }
  अन्यथा
    {
      sprintf (buf, "\\%03o", c);
    }
  प्रत्यागम buf;
}

/* Return a newly allocated copy of S which is suitable for printing.
   LEN is the number of characters in S.  Most non-printing
   (isprint) characters are represented by a backslash followed by
   3 octal digits.  However, the characters represented by \c escapes
   where c is one of [abfnrtv] are represented by their 2-character \c
   sequences.  This function is used solely for printing error messages.  */

स्थिर वर्ण *
make_printable_str (वर्ण ध्रुव *s, size_t len)
{
  /* Worst case is that every character expands to a backslash
     followed by a 3-character octal escape sequence.  */
  वर्ण *printable_buf = xnmalloc (len + 1, 4);
  वर्ण *p = printable_buf;

  प्रति (size_t i = 0; i < len; i++)
    {
      वर्ण buf[5];
      वर्ण ध्रुव *tmp = रिक्त;
      अचिह्नित वर्ण c = s[i];

      विकल्प (c)
        {
        पक्ष '\\':
          tmp = "\\";
          भङ्ग;
        पक्ष '\a':
          tmp = "\\a";
          भङ्ग;
        पक्ष '\b':
          tmp = "\\b";
          भङ्ग;
        पक्ष '\f':
          tmp = "\\f";
          भङ्ग;
        पक्ष '\n':
          tmp = "\\n";
          भङ्ग;
        पक्ष '\r':
          tmp = "\\r";
          भङ्ग;
        पक्ष '\t':
          tmp = "\\t";
          भङ्ग;
        पक्ष '\v':
          tmp = "\\v";
          भङ्ग;
        स्वभाव:
          यदि (isprint (c))
            {
              buf[0] = c;
              buf[1] = '\0';
            }
          अन्यथा
            sprintf (buf, "\\%03o", c);
          tmp = buf;
          भङ्ग;
        }
      p = stpcpy (p, tmp);
    }
  प्रत्यागम printable_buf;
}

/* Append a newly allocated structure representing a
   character C to the specification list LIST.  */

स्थिर शून्य
append_प्रसामान्यम्_char (संरचना Spec_list *list, अचिह्नित वर्ण c)
{
  संरचना List_element *new = xmalloc (परिमाण *new);
  new->अग्र = रिक्त;
  new->type = RE_प्रसामान्यम्_CHAR;
  new->u.प्रसामान्यम्_char = c;
  list->पुच्छ->अग्र = new;
  list->पुच्छ = new;
}

/* Append a newly allocated structure representing the range
   of characters from FIRST to LAST to the specification list LIST.
   Return false if LAST precedes FIRST in the collating sequence,
   true otherwise.  This means that '[c-c]' is acceptable.  */

स्थिर bool
append_range (संरचना Spec_list *list, अचिह्नित वर्ण first, अचिह्नित वर्ण चरमः)
{
  यदि (चरमः < first)
    {
      वर्ण *tmp1 = make_printable_char (first);
      वर्ण *tmp2 = make_printable_char (चरमः);

      error (0, 0,
       _("range-endpoints of '%s-%s' are in reverse collating sequence order"),
             tmp1, tmp2);
      मुक्त (tmp1);
      मुक्त (tmp2);
      प्रत्यागम false;
    }
  संरचना List_element *new = xmalloc (परिमाण *new);
  new->अग्र = रिक्त;
  new->type = RE_RANGE;
  new->u.range.first_char = first;
  new->u.range.चरमः_char = चरमः;
  list->पुच्छ->अग्र = new;
  list->पुच्छ = new;
  प्रत्यागम true;
}

/* If CHAR_CLASS_STR is a valid character class string, append a
   newly allocated structure representing that character class to the end
   of the specification list LIST and return true.  If CHAR_CLASS_STR is not
   a valid string return false.  */

स्थिर bool
append_char_class (संरचना Spec_list *list,
                   वर्ण ध्रुव *char_class_str, size_t len)
{
  गणना Char_class char_class = look_ऊर्ध्वम्_char_class (char_class_str, len);
  यदि (char_class == CC_NO_CLASS)
    प्रत्यागम false;
  संरचना List_element *new = xmalloc (परिमाण *new);
  new->अग्र = रिक्त;
  new->type = RE_CHAR_CLASS;
  new->u.char_class = char_class;
  list->पुच्छ->अग्र = new;
  list->पुच्छ = new;
  प्रत्यागम true;
}

/* Append a newly allocated structure representing a [c*n]
   repeated character construct to the specification list LIST.
   THE_CHAR is the single character to be repeated, and REPEAT_COUNT
   is a non-negative repeat count.  */

स्थिर शून्य
append_repeated_char (संरचना Spec_list *list, अचिह्नित वर्ण the_char,
                      गणनम् repeat_गणनम्)
{
  संरचना List_element *new = xmalloc (परिमाण *new);
  new->अग्र = रिक्त;
  new->type = RE_REPEATED_CHAR;
  new->u.repeated_char.the_repeated_char = the_char;
  new->u.repeated_char.repeat_गणनम् = repeat_गणनम्;
  list->पुच्छ->अग्र = new;
  list->पुच्छ = new;
}

/* Given a string, EQUIV_CLASS_STR, from a [=str=] context and
   the length of that string, LEN, if LEN is exactly one, append
   a newly allocated structure representing the specified
   equivalence class to the specification list, LIST and return true.
   If LEN is not 1, return false.  */

स्थिर bool
append_equiv_class (संरचना Spec_list *list,
                    वर्ण ध्रुव *equiv_class_str, size_t len)
{
  यदि (len != 1)
    प्रत्यागम false;

  संरचना List_element *new = xmalloc (परिमाण *new);
  new->अग्र = रिक्त;
  new->type = RE_EQUIV_CLASS;
  new->u.equiv_संहिता = *equiv_class_str;
  list->पुच्छ->अग्र = new;
  list->पुच्छ = new;
  प्रत्यागम true;
}

/* Search forward starting at START_IDX for the 2-char sequence
   (PRE_BRACKET_CHAR,']') in the string P of length P_LEN.  If such
   a sequence is found, set *RESULT_IDX to the index of the first
   character and return true.  Otherwise return false.  P may contain
   zero bytes.  */

स्थिर bool
find_closing_delim (ध्रुव संरचना E_string *es, size_t start_idx,
                    वर्ण pre_bracket_char, size_t *परिणाम_idx)
{
  प्रति (size_t i = start_idx; i < es->len - 1; i++)
    यदि (es->s[i] == pre_bracket_char && es->s[i + 1] == ']'
        && !es->escaped[i] && !es->escaped[i + 1])
      {
        *परिणाम_idx = i;
        प्रत्यागम true;
      }
  प्रत्यागम false;
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

स्थिर पूर्णाङ्क
find_bracketed_repeat (ध्रुव संरचना E_string *es, size_t start_idx,
                       अचिह्नित वर्ण *char_गन्तव्यम्_repeat, गणनम् *repeat_गणनम्,
                       size_t *closing_bracket_idx)
{
  affirm (start_idx + 1 < es->len);
  यदि (!es_मेलनम् (es, start_idx + 1, '*'))
    प्रत्यागम -1;

  प्रति (size_t i = start_idx + 2; i < es->len && !es->escaped[i]; i++)
    {
      यदि (es->s[i] == ']')
        {
          size_t digit_str_len = i - start_idx - 2;

          *char_गन्तव्यम्_repeat = es->s[start_idx];
          यदि (digit_str_len == 0)
            {
              /* We've matched [c*] -- no explicit repeat count.  */
              *repeat_गणनम् = 0;
            }
          अन्यथा
            {
              /* Here, we have found [c*s] where s should be a string
                 of octal (if it starts with '0') or decimal digits.  */
              वर्ण ध्रुव *digit_str = &es->s[start_idx + 2];
              वर्ण *d_अन्तः;
              यदि ((xstrtoumax (digit_str, &d_अन्तः, *digit_str == '0' ? 8 : 10,
                               repeat_गणनम्, रिक्त)
                   != LONGINT_OK)
                  || REPEAT_गणनम्_अधिकतमम् < *repeat_गणनम्
                  || digit_str + digit_str_len != d_अन्तः)
                {
                  वर्ण *tmp = make_printable_str (digit_str, digit_str_len);
                  error (0, 0,
                         _("invalid repeat count %s in [c*n] construct"),
                         quote (tmp));
                  मुक्त (tmp);
                  प्रत्यागम -2;
                }
            }
          *closing_bracket_idx = i;
          प्रत्यागम 0;
        }
    }
  प्रत्यागम -1;			/* No bracket found.  */
}

/* Return true if the string at ES->s[IDX] matches the regular
   expression '\*[0-9]*]', false otherwise.  The string does not
   match if any of its characters are escaped.  */

ATTRIBUTE_विशुद्धम्
स्थिर bool
तारकम्_digits_closebracket (ध्रुव संरचना E_string *es, size_t idx)
{
  यदि (!es_मेलनम् (es, idx, '*'))
    प्रत्यागम false;

  प्रति (size_t i = idx + 1; i < es->len; i++)
    यदि (!c_isdigit (es->s[i]) || es->escaped[i])
      प्रत्यागम es_मेलनम् (es, i, ']');
  प्रत्यागम false;
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

स्थिर bool
build_spec_list (ध्रुव संरचना E_string *es, संरचना Spec_list *परिणाम)
{
  वर्ण ध्रुव *p = es->s;

  /* The main for-loop below recognizes the 4 multi-character constructs.
     A character that matches (in its context) none of the multi-character
     constructs is classified as 'normal'.  Since all multi-character
     constructs have at least 3 characters, any strings of length 2 or
     less are composed solely of normal characters.  Hence, the index of
     the outer for-loop runs only as far as LEN-2.  */
  size_t i;
  प्रति (i = 0; i + 2 < es->len; /*रिक्तानि*/)
    {
      यदि (es_मेलनम् (es, i, '['))
        {
          bool matched_multi_char_construct;
          size_t closing_bracket_idx;
          अचिह्नित वर्ण char_गन्तव्यम्_repeat;
          गणनम् repeat_गणनम्;
          पूर्णाङ्क err;

          matched_multi_char_construct = true;
          यदि (es_मेलनम् (es, i + 1, ':') || es_मेलनम् (es, i + 1, '='))
            {
              size_t closing_delim_idx;

              यदि (find_closing_delim (es, i + 2, p[i + 1], &closing_delim_idx))
                {
                  size_t opnd_str_len = closing_delim_idx - 1 - (i + 2) + 1;
                  वर्ण ध्रुव *opnd_str = p + i + 2;

                  यदि (opnd_str_len == 0)
                    {
                      यदि (p[i + 1] == ':')
                        error (0, 0, _("missing character class name '[::]'"));
                      अन्यथा
                        error (0, 0,
                               _("missing equivalence class character '[==]'"));
                      प्रत्यागम false;
                    }

                  यदि (p[i + 1] == ':')
                    {
                      /* FIXME: big comment.  */
                      यदि (!append_char_class (परिणाम, opnd_str, opnd_str_len))
                        {
                          यदि (तारकम्_digits_closebracket (es, i + 2))
                            गच्छ try_bracketed_repeat;
                          अन्यथा
                            {
                              वर्ण *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0, _("invalid character class %s"),
                                     quote (tmp));
                              मुक्त (tmp);
                              प्रत्यागम false;
                            }
                        }
                    }
                  अन्यथा
                    {
                      /* FIXME: big comment.  */
                      यदि (!append_equiv_class (परिणाम, opnd_str, opnd_str_len))
                        {
                          यदि (तारकम्_digits_closebracket (es, i + 2))
                            गच्छ try_bracketed_repeat;
                          अन्यथा
                            {
                              वर्ण *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0,
               _("%s: equivalence class operand must be a single character"),
                                     tmp);
                              मुक्त (tmp);
                              प्रत्यागम false;
                            }
                        }
                    }

                  i = closing_delim_idx + 2;
                  अनुवर्त;
                }
              /* Else fall through.  This could be [:*] or [=*].  */
            }

        try_bracketed_repeat:

          /* Determine whether this is a bracketed repeat range
             matching the RE \[.\*(dec_or_oct_number)?].  */
          err = find_bracketed_repeat (es, i + 1, &char_गन्तव्यम्_repeat,
                                       &repeat_गणनम्,
                                       &closing_bracket_idx);
          यदि (err == 0)
            {
              append_repeated_char (परिणाम, char_गन्तव्यम्_repeat, repeat_गणनम्);
              i = closing_bracket_idx + 1;
            }
          अन्यथा यदि (err == -1)
            {
              matched_multi_char_construct = false;
            }
          अन्यथा
            {
              /* Found a string that looked like [c*n] but the
                 numeric part was invalid.  */
              प्रत्यागम false;
            }

          यदि (matched_multi_char_construct)
            अनुवर्त;

          /* We reach this point if P does not match [:str:], [=c=],
             [c*n], or [c*].  Now, see if P looks like a range '[-c'
             (from '[' to 'c').  */
        }

      /* Look ahead one char for ranges like a-z.  */
      यदि (es_मेलनम् (es, i + 1, '-'))
        {
          यदि (!append_range (परिणाम, p[i], p[i + 2]))
            प्रत्यागम false;
          i += 3;
        }
      अन्यथा
        {
          append_प्रसामान्यम्_char (परिणाम, p[i]);
          ++i;
        }
    }

  /* Now handle the (2 or fewer) remaining characters p[i]..p[es->len - 1].  */
  प्रति (; i < es->len; i++)
    append_प्रसामान्यम्_char (परिणाम, p[i]);

  प्रत्यागम true;
}

/* Advance past the current construct.
   S->tail must be non-null.  */
स्थिर शून्य
skip_construct (संरचना Spec_list *s)
{
  s->पुच्छ = s->पुच्छ->अग्र;
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

स्थिर पूर्णाङ्क
get_अग्र (संरचना Spec_list *s, गणना ऊर्ध्वस्थम्_अधस्तनम्_class *class)
{
  संरचना List_element *p;
  पूर्णाङ्क return_val;
  पूर्णाङ्क i;

  यदि (class)
    *class = UL_NONE;

  यदि (s->state == BEGIN_STATE)
    {
      s->पुच्छ = s->शीर्ष->अग्र;
      s->state = NEW_ELEMENT;
    }

  p = s->पुच्छ;
  यदि (p == रिक्त)
    प्रत्यागम -1;

  विकल्प (p->type)
    {
    पक्ष RE_प्रसामान्यम्_CHAR:
      return_val = p->u.प्रसामान्यम्_char;
      s->state = NEW_ELEMENT;
      s->पुच्छ = p->अग्र;
      भङ्ग;

    पक्ष RE_RANGE:
      यदि (s->state == NEW_ELEMENT)
        s->state = p->u.range.first_char;
      अन्यथा
        ++(s->state);
      return_val = s->state;
      यदि (s->state == p->u.range.चरमः_char)
        {
          s->पुच्छ = p->अग्र;
          s->state = NEW_ELEMENT;
        }
      भङ्ग;

    पक्ष RE_CHAR_CLASS:
      यदि (class)
        {
          विकल्प (+p->u.char_class)
            {
            पक्ष CC_अधस्तनम्:
              *class = UL_अधस्तनम्;
              भङ्ग;
            पक्ष CC_ऊर्ध्वस्थम्:
              *class = UL_ऊर्ध्वस्थम्;
              भङ्ग;
            }
        }

      यदि (s->state == NEW_ELEMENT)
        {
          प्रति (i = 0; i < N_CHARS; i++)
            यदि (is_char_class_member (p->u.char_class, i))
              भङ्ग;
          affirm (i < N_CHARS);
          s->state = i;
        }
      assure (is_char_class_member (p->u.char_class, s->state));
      return_val = s->state;
      प्रति (i = s->state + 1; i < N_CHARS; i++)
        यदि (is_char_class_member (p->u.char_class, i))
          भङ्ग;
      यदि (i < N_CHARS)
        s->state = i;
      अन्यथा
        {
          s->पुच्छ = p->अग्र;
          s->state = NEW_ELEMENT;
        }
      भङ्ग;

    पक्ष RE_EQUIV_CLASS:
      /* FIXME: this assumes that each character is alone in its own
         equivalence class (which appears to be correct for my
         LC_COLLATE.  But I don't know of any function that allows
         one to determine a character's equivalence class.  */

      return_val = p->u.equiv_संहिता;
      s->state = NEW_ELEMENT;
      s->पुच्छ = p->अग्र;
      भङ्ग;

    पक्ष RE_REPEATED_CHAR:
      /* Here, a repeat count of n == 0 means don't repeat at all.  */
      यदि (p->u.repeated_char.repeat_गणनम् == 0)
        {
          s->पुच्छ = p->अग्र;
          s->state = NEW_ELEMENT;
          return_val = get_अग्र (s, class);
        }
      अन्यथा
        {
          यदि (s->state == NEW_ELEMENT)
            {
              s->state = 0;
            }
          ++(s->state);
          return_val = p->u.repeated_char.the_repeated_char;
          यदि (s->state == p->u.repeated_char.repeat_गणनम्)
            {
              s->पुच्छ = p->अग्र;
              s->state = NEW_ELEMENT;
            }
        }
      भङ्ग;

    स्वभाव:
      unreachable ();
    }

  प्रत्यागम return_val;
}

/* This is a minor kludge.  This function is called from
   get_spec_stats to determine the cardinality of a set derived
   from a complemented string.  It's a kludge in that some of the
   same operations are (duplicated) performed in set_initialize.  */

स्थिर पूर्णाङ्क
card_सम्बन्धि_complement (संरचना Spec_list *s)
{
  पूर्णाङ्क c;
  पूर्णाङ्क cardinality = N_CHARS;
  bool अन्तर्_समुच्चयः[N_CHARS] = {0};

  s->state = BEGIN_STATE;
  यावत् ((c = get_अग्र (s, रिक्त)) != -1)
    {
      cardinality -= (!अन्तर्_समुच्चयः[c]);
      अन्तर्_समुच्चयः[c] = true;
    }
  प्रत्यागम cardinality;
}

/* Discard the lengths associated with a case conversion,
   as using the actual number of upper or lower case characters
   is problematic when they don't match in some locales.
   Also ensure the case conversion classes in string2 are
   aligned correctly with those in string1.
   Note POSIX says the behavior of 'tr "[:upper:]" "[:upper:]"'
   is undefined.  Therefore we allow it (unlike Solaris)
   and treat it as a no-op.  */

स्थिर शून्य
validate_case_classes (संरचना Spec_list *s1, संरचना Spec_list *s2)
{
  size_t n_ऊर्ध्वस्थम् = 0;
  size_t n_अधस्तनम् = 0;
  पूर्णाङ्क c1 = 0;
  पूर्णाङ्क c2 = 0;
  MAYBE_अप्रयुक्तम् गणनम् old_s1_len = s1->दीर्घता, old_s2_len = s2->दीर्घता;
  संरचना List_element *s1_पुच्छ = s1->पुच्छ;
  संरचना List_element *s2_पुच्छ = s2->पुच्छ;
  bool s1_new_element = true;
  bool s2_new_element = true;

  यदि (complement || !s2->has_char_class)
    प्रत्यागम;

  प्रति (पूर्णाङ्क i = 0; i < N_CHARS; i++)
    {
      यदि (isupper (i))
        n_ऊर्ध्वस्थम्++;
      यदि (islower (i))
        n_अधस्तनम्++;
    }

  s1->state = BEGIN_STATE;
  s2->state = BEGIN_STATE;

  यावत् (c1 != -1 && c2 != -1)
    {
      गणना ऊर्ध्वस्थम्_अधस्तनम्_class class_s1, class_s2;

      c1 = get_अग्र (s1, &class_s1);
      c2 = get_अग्र (s2, &class_s2);

      /* If c2 transitions to a new case class, then
         c1 must also transition at the same time.  */
      यदि (s2_new_element && class_s2 != UL_NONE
          && !(s1_new_element && class_s1 != UL_NONE))
        error (EXIT_FAILURE, 0,
               _("misaligned [:upper:] and/or [:lower:] construct"));

      /* If case converting, quickly skip over the elements.  */
      यदि (class_s2 != UL_NONE)
        {
          skip_construct (s1);
          skip_construct (s2);
          /* Discount insignificant/problematic lengths.  */
          s1->दीर्घता -= (class_s1 == UL_ऊर्ध्वस्थम् ? n_ऊर्ध्वस्थम् : n_अधस्तनम्) - 1;
          s2->दीर्घता -= (class_s2 == UL_ऊर्ध्वस्थम् ? n_ऊर्ध्वस्थम् : n_अधस्तनम्) - 1;
        }

      s1_new_element = s1->state == NEW_ELEMENT; /* Next element is new.  */
      s2_new_element = s2->state == NEW_ELEMENT; /* Next element is new.  */
    }

  affirm (old_s1_len >= s1->दीर्घता && old_s2_len >= s2->दीर्घता);

  s1->पुच्छ = s1_पुच्छ;
  s2->पुच्छ = s2_पुच्छ;
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

स्थिर शून्य
get_spec_stats (संरचना Spec_list *s)
{
  गणनम् दीर्घता = 0;

  s->n_indefinite_repeats = 0;
  s->has_equiv_class = false;
  s->has_restricted_char_class = false;
  s->has_char_class = false;
  प्रति (संरचना List_element *p = s->शीर्ष->अग्र; p; p = p->अग्र)
    {
      गणनम् len = 0;

      विकल्प (p->type)
        {
        पक्ष RE_प्रसामान्यम्_CHAR:
          len = 1;
          भङ्ग;

        पक्ष RE_RANGE:
          affirm (p->u.range.चरमः_char >= p->u.range.first_char);
          len = p->u.range.चरमः_char - p->u.range.first_char + 1;
          भङ्ग;

        पक्ष RE_CHAR_CLASS:
          s->has_char_class = true;
          प्रति (पूर्णाङ्क i = 0; i < N_CHARS; i++)
            यदि (is_char_class_member (p->u.char_class, i))
              ++len;
          विकल्प (+p->u.char_class)
            {
            पक्ष CC_ऊर्ध्वस्थम्:
            पक्ष CC_अधस्तनम्:
              भङ्ग;
            स्वभाव:
              s->has_restricted_char_class = true;
              भङ्ग;
            }
          भङ्ग;

        पक्ष RE_EQUIV_CLASS:
          प्रति (पूर्णाङ्क i = 0; i < N_CHARS; i++)
            यदि (is_equiv_class_member (p->u.equiv_संहिता, i))
              ++len;
          s->has_equiv_class = true;
          भङ्ग;

        पक्ष RE_REPEATED_CHAR:
          यदि (p->u.repeated_char.repeat_गणनम् > 0)
            len = p->u.repeated_char.repeat_गणनम्;
          अन्यथा
            {
              s->indefinite_repeat_element = p;
              ++(s->n_indefinite_repeats);
            }
          भङ्ग;

        स्वभाव:
          unreachable ();
        }

      /* Check for arithmetic overflow in computing length.  Also, reject
         any length greater than the maximum repeat count, in case the
         length is later used to compute the repeat count for an
         indefinite element.  */
      यदि (ckd_सङ्कलय (&दीर्घता, दीर्घता, len) || REPEAT_गणनम्_अधिकतमम् < दीर्घता)
        error (EXIT_FAILURE, 0, _("too many characters in set"));
    }

  s->दीर्घता = दीर्घता;
}

स्थिर शून्य
get_s1_spec_stats (संरचना Spec_list *s1)
{
  get_spec_stats (s1);
  यदि (complement)
    s1->दीर्घता = card_सम्बन्धि_complement (s1);
}

स्थिर शून्य
get_s2_spec_stats (संरचना Spec_list *s2, गणनम् len_s1)
{
  get_spec_stats (s2);
  यदि (len_s1 >= s2->दीर्घता && s2->n_indefinite_repeats == 1)
    {
      s2->indefinite_repeat_element->u.repeated_char.repeat_गणनम् =
        len_s1 - s2->दीर्घता;
      s2->दीर्घता = len_s1;
    }
}

स्थिर शून्य
spec_init (संरचना Spec_list *spec_list)
{
  संरचना List_element *new = xmalloc (परिमाण *new);
  spec_list->शीर्ष = spec_list->पुच्छ = new;
  spec_list->शीर्ष->अग्र = रिक्त;
}

/* This function makes two passes over the argument string S.  The first
   one converts all \c and \ddd escapes to their one-byte representations.
   The second constructs a linked specification list, SPEC_LIST, of the
   characters and constructs that comprise the argument string.  If either
   of these passes detects an error, this function returns false.  */

स्थिर bool
parse_str (वर्ण ध्रुव *s, संरचना Spec_list *spec_list)
{
  संरचना E_string es;
  bool ok = unquote (s, &es) && build_spec_list (&es, spec_list);
  es_free (&es);
  प्रत्यागम ok;
}

/* Given two specification lists, S1 and S2, and assuming that
   S1->length > S2->length, append a single [c*n] element to S2 where c
   is the last character in the expansion of S2 and n is the difference
   between the two lengths.
   Upon successful completion, S2->length is set to S1->length.  The only
   way this function can fail to make S2 as long as S1 is when S2 has
   zero-length, since in that case, there is no last character to repeat.
   So S2->length is required to be at least 1.  */

स्थिर शून्य
string2_extend (ध्रुव संरचना Spec_list *s1, संरचना Spec_list *s2)
{
  संरचना List_element *p;
  अचिह्नित वर्ण char_गन्तव्यम्_repeat;

  affirm (translating);
  affirm (s1->दीर्घता > s2->दीर्घता);
  affirm (s2->दीर्घता > 0);

  p = s2->पुच्छ;
  विकल्प (p->type)
    {
    पक्ष RE_प्रसामान्यम्_CHAR:
      char_गन्तव्यम्_repeat = p->u.प्रसामान्यम्_char;
      भङ्ग;
    पक्ष RE_RANGE:
      char_गन्तव्यम्_repeat = p->u.range.चरमः_char;
      भङ्ग;
    पक्ष RE_CHAR_CLASS:
      /* Note BSD allows extending of classes in string2.  For example:
           tr '[:upper:]0-9' '[:lower:]'
         That's not portable however, contradicts POSIX and is dependent
         on your collating sequence.  */
      error (EXIT_FAILURE, 0,
             _("when translating with string1 longer than string2,\n"
               "the latter string must not end with a character class"));

    पक्ष RE_REPEATED_CHAR:
      char_गन्तव्यम्_repeat = p->u.repeated_char.the_repeated_char;
      भङ्ग;

    पक्ष RE_EQUIV_CLASS: स्वभाव:
      /* This shouldn't happen, because validate exits with an error
         if it finds an equiv class in string2 when translating.  */
      affirm (false);
    }

  append_repeated_char (s2, char_गन्तव्यम्_repeat, s1->दीर्घता - s2->दीर्घता);
  s2->दीर्घता = s1->दीर्घता;
}

/* Return true if S is a non-empty list in which exactly one
   character (but potentially, many instances of it) appears.
   E.g., [X*] or xxxxxxxx.  */

स्थिर bool
homogeneous_spec_list (संरचना Spec_list *s)
{
  पूर्णाङ्क b, c;

  s->state = BEGIN_STATE;

  यदि ((b = get_अग्र (s, रिक्त)) == -1)
    प्रत्यागम false;

  यावत् ((c = get_अग्र (s, रिक्त)) != -1)
    यदि (c != b)
      प्रत्यागम false;

  प्रत्यागम true;
}

/* Die with an error message if S1 and S2 describe strings that
   are not valid with the given command line switches.
   A side effect of this function is that if a valid [c*] or
   [c*0] construct appears in string2, it is converted to [c*n]
   with a value for n that makes s2->length == s1->length.  By
   the same token, if the --truncate-set1 option is not
   given, S2 may be extended.  */

स्थिर शून्य
validate (संरचना Spec_list *s1, संरचना Spec_list *s2)
{
  get_s1_spec_stats (s1);
  यदि (s1->n_indefinite_repeats > 0)
    error (EXIT_FAILURE, 0,
           _("the [c*] repeat construct may not appear in string1"));

  यदि (s2)
    {
      get_s2_spec_stats (s2, s1->दीर्घता);

      यदि (s2->n_indefinite_repeats > 1)
        error (EXIT_FAILURE, 0,
               _("only one [c*] repeat construct may appear in string2"));

      यदि (translating)
        {
          यदि (s2->has_equiv_class)
            error (EXIT_FAILURE, 0,
                   _("[=c=] expressions may not appear in string2"
                     " when translating"));

          यदि (s2->has_restricted_char_class)
            error (EXIT_FAILURE, 0,
                   _("when translating, the only character classes"
                     " that may appear in\n"
                     "string2 are 'upper' and 'lower'"));

          validate_case_classes (s1, s2);

          यदि (s1->दीर्घता > s2->दीर्घता)
            {
              यदि (!truncate_set1)
                {
                  /* string2 must be non-empty unless --truncate-set1 is
                     given or string1 is empty.  */

                  यदि (s2->दीर्घता == 0)
                    error (EXIT_FAILURE, 0,
                           _("when not truncating set1,"
                             " string2 must be non-empty"));
                  string2_extend (s1, s2);
                }
            }

          यदि (complement && s1->has_char_class
              && ! (s2->दीर्घता == s1->दीर्घता && homogeneous_spec_list (s2)))
            error (EXIT_FAILURE, 0,
                   _("when translating with complemented character classes,\n"
                     "string2 must map all characters in the domain to one"));
        }
      अन्यथा
        /* Not translating.  */
        {
          यदि (s2->n_indefinite_repeats > 0)
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

स्थिर शून्य
squeeze_filter (वर्ण *buf, size_t आकार, size_t (*reader) (वर्ण *, size_t))
{
  /* A value distinct from any character that may have been stored in a
     buffer as the result of a block-read in the function squeeze_filter.  */
  ध्रुव पूर्णाङ्क NOT_A_CHAR = अधिकतम_पूर्णाङ्कः;

  पूर्णाङ्क char_गन्तव्यम्_squeeze = NOT_A_CHAR;
  size_t i = 0;
  size_t nr = 0;

  यावत् (true)
    {
      यदि (i >= nr)
        {
          nr = reader (buf, आकार);
          यदि (nr == 0)
            भङ्ग;
          i = 0;
        }

      size_t begin = i;

      यदि (char_गन्तव्यम्_squeeze == NOT_A_CHAR)
        {
          size_t बहिर्गमनम्_len;
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
          प्रति (; i < nr && !अन्तर्_squeeze_समुच्चयः[गन्तव्यम्_uchar (buf[i])]; i += 2)
            अनुवर्त;

          /* There is a special case when i == nr and we've just
             skipped a character (the last one in buf) that is in
             the squeeze set.  */
          यदि (i == nr && अन्तर्_squeeze_समुच्चयः[गन्तव्यम्_uchar (buf[i - 1])])
            --i;

          यदि (i >= nr)
            बहिर्गमनम्_len = nr - begin;
          अन्यथा
            {
              char_गन्तव्यम्_squeeze = buf[i];
              /* We're about to output buf[begin..i].  */
              बहिर्गमनम्_len = i - begin + 1;

              /* But since we stepped by 2 in the loop above,
                 out_len may be one too large.  */
              यदि (i > 0 && buf[i - 1] == char_गन्तव्यम्_squeeze)
                --बहिर्गमनम्_len;

              /* Advance i to the index of first character to be
                 considered when looking for a char different from
                 char_to_squeeze.  */
              ++i;
            }
          यदि (बहिर्गमनम्_len > 0
              && fwrite (&buf[begin], 1, बहिर्गमनम्_len, stdout) != बहिर्गमनम्_len)
            write_error ();
        }

      यदि (char_गन्तव्यम्_squeeze != NOT_A_CHAR)
        {
          /* Advance i to index of first char != char_to_squeeze
             (or to nr if all the rest of the characters in this
             buffer are the same as char_to_squeeze).  */
          प्रति (; i < nr && buf[i] == char_गन्तव्यम्_squeeze; i++)
            अनुवर्त;
          यदि (i < nr)
            char_गन्तव्यम्_squeeze = NOT_A_CHAR;
          /* If (i >= nr) we've squeezed the last character in this buffer.
             So now we have to read a new buffer and continue comparing
             characters against char_to_squeeze.  */
        }
    }
}

स्थिर size_t
plain_read (वर्ण *buf, size_t आकार)
{
  ssize_t nr = पठ (STDIN_FILENO, buf, आकार);
  यदि (nr < 0)
    error (EXIT_FAILURE, errno, _("read error"));
  प्रत्यागम nr;
}

/* Read buffers of SIZE bytes from stdin until one is found that
   contains at least one character not in the delete set.  Store
   in the array BUF, all characters from that buffer that are not
   in the delete set, and return the number of characters saved
   or 0 upon EOF.  */

स्थिर size_t
read_and_delete (वर्ण *buf, size_t आकार)
{
  size_t n_saved;

  /* This enclosing do-while loop is to make sure that
     we don't return zero (indicating EOF) when we've
     just deleted all the characters in a buffer.  */
  कुरु
    {
      size_t nr = plain_read (buf, आकार);

      यदि (nr == 0)
        प्रत्यागम 0;

      /* This first loop may be a waste of code, but gives much
         better performance when no characters are deleted in
         the beginning of a buffer.  It just avoids the copying
         of buf[i] into buf[n_saved] when it would be a NOP.  */

      size_t i;
      प्रति (i = 0; i < nr && !अन्तर्_delete_समुच्चयः[गन्तव्यम्_uchar (buf[i])]; i++)
        अनुवर्त;
      n_saved = i;

      प्रति (++i; i < nr; i++)
        यदि (!अन्तर्_delete_समुच्चयः[गन्तव्यम्_uchar (buf[i])])
          buf[n_saved++] = buf[i];
    }
  यावत् (n_saved == 0);

  प्रत्यागम n_saved;
}

/* Read at most SIZE bytes from stdin into the array BUF.  Then
   perform the in-place and one-to-one mapping specified by the global
   array 'xlate'.  Return the number of characters read, or 0 upon EOF.  */

स्थिर size_t
read_and_xlate (वर्ण *buf, size_t आकार)
{
  size_t अष्टकानि_read = plain_read (buf, आकार);

  प्रति (size_t i = 0; i < अष्टकानि_read; i++)
    buf[i] = xlate[गन्तव्यम्_uchar (buf[i])];

  प्रत्यागम अष्टकानि_read;
}

/* Initialize a boolean membership set, IN_SET, with the character
   values obtained by traversing the linked list of constructs S
   using the function 'get_next'.  IN_SET is expected to have been
   initialized to all zeros by the caller.  If COMPLEMENT_THIS_SET
   is true the resulting set is complemented.  */

स्थिर शून्य
समुच्चयः_initialize (संरचना Spec_list *s, bool complement_this_समुच्चयः, bool *अन्तर्_समुच्चयः)
{
  पूर्णाङ्क c;

  s->state = BEGIN_STATE;
  यावत् ((c = get_अग्र (s, रिक्त)) != -1)
    अन्तर्_समुच्चयः[c] = true;
  यदि (complement_this_समुच्चयः)
    प्रति (size_t i = 0; i < N_CHARS; i++)
      अन्तर्_समुच्चयः[i] = (!अन्तर्_समुच्चयः[i]);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;
  पूर्णाङ्क non_option_तर्काः;
  पूर्णाङ्क min_operands;
  पूर्णाङ्क max_operands;
  संरचना Spec_list buf1, buf2;
  संरचना Spec_list *s1 = &buf1;
  संरचना Spec_list *s2 = &buf2;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((c = getopt_long (argc, argv, "+AcCdstu", long_options, रिक्त)) != -1)
    {
      विकल्प (c)
        {
        पक्ष 'A':
          /* Undocumented option, for compatibility with AIX.  */
          setlocale (LC_COLLATE, "C");
          setlocale (LC_CTYPE, "C");
          भङ्ग;

        पक्ष 'c':
        पक्ष 'C':
          complement = true;
          भङ्ग;

        पक्ष 'd':
          delete = true;
          भङ्ग;

        पक्ष 's':
          squeeze_repeats = true;
          भङ्ग;

        पक्ष 't':
          truncate_set1 = true;
          भङ्ग;

        पक्ष 'u':
          /* Undocumented option, for compatibility with FreeBSD/macOS.  */
          setvbuf (stdout, रिक्त, _IONBF, 0);
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
          भङ्ग;
        }
    }

  non_option_तर्काः = argc - optind;
  translating = (non_option_तर्काः == 2 && !delete);
  min_operands = 1 + (delete == squeeze_repeats);
  max_operands = 1 + (delete <= squeeze_repeats);

  यदि (non_option_तर्काः < min_operands)
    {
      यदि (non_option_तर्काः == 0)
        error (0, 0, _("missing operand"));
      अन्यथा
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

  यदि (max_operands < non_option_तर्काः)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + max_operands]));
      यदि (non_option_तर्काः == 2)
        fprintf (stderr, "%s\n",
                 _("Only one string may be given when "
                   "deleting without squeezing repeats."));
      usage (EXIT_FAILURE);
    }

  spec_init (s1);
  यदि (!parse_str (argv[optind], s1))
    main_exit (EXIT_FAILURE);

  यदि (non_option_तर्काः == 2)
    {
      spec_init (s2);
      यदि (!parse_str (argv[optind + 1], s2))
        main_exit (EXIT_FAILURE);
    }
  अन्यथा
    s2 = रिक्त;

  validate (s1, s2);

  /* Use binary I/O, since 'tr' is sometimes used to transliterate
     non-printable characters, or characters which are stripped away
     by text-mode reads (like CR and ^Z).  */
  xset_द्विमान_बहुलकम् (STDIN_FILENO, O_द्विमान);
  xset_द्विमान_बहुलकम् (STDOUT_FILENO, O_द्विमान);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  यदि (squeeze_repeats && non_option_तर्काः == 1)
    {
      समुच्चयः_initialize (s1, complement, अन्तर्_squeeze_समुच्चयः);
      squeeze_filter (io_buf, परिमाण io_buf, plain_read);
    }
  अन्यथा यदि (delete && non_option_तर्काः == 1)
    {
      समुच्चयः_initialize (s1, complement, अन्तर्_delete_समुच्चयः);

      यावत् (true)
        {
          size_t nr = read_and_delete (io_buf, परिमाण io_buf);
          यदि (nr == 0)
            भङ्ग;
          यदि (fwrite (io_buf, 1, nr, stdout) != nr)
            write_error ();
        }
    }
  अन्यथा यदि (squeeze_repeats && delete && non_option_तर्काः == 2)
    {
      समुच्चयः_initialize (s1, complement, अन्तर्_delete_समुच्चयः);
      समुच्चयः_initialize (s2, false, अन्तर्_squeeze_समुच्चयः);
      squeeze_filter (io_buf, परिमाण io_buf, read_and_delete);
    }
  अन्यथा यदि (translating)
    {
      यदि (complement)
        {
          bool *अन्तर्_s1 = अन्तर्_delete_समुच्चयः;

          समुच्चयः_initialize (s1, false, अन्तर्_s1);
          s2->state = BEGIN_STATE;
          प्रति (पूर्णाङ्क i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          प्रति (पूर्णाङ्क i = 0; i < N_CHARS; i++)
            {
              यदि (!अन्तर्_s1[i])
                {
                  पूर्णाङ्क ch = get_अग्र (s2, रिक्त);
                  affirm (ch != -1 || truncate_set1);
                  यदि (ch == -1)
                    {
                      /* This will happen when tr is invoked like e.g.
                         tr -cs A-Za-z0-9 '\012'.  */
                      भङ्ग;
                    }
                  xlate[i] = ch;
                }
            }
        }
      अन्यथा
        {
          पूर्णाङ्क c1, c2;
          गणना ऊर्ध्वस्थम्_अधस्तनम्_class class_s1;
          गणना ऊर्ध्वस्थम्_अधस्तनम्_class class_s2;

          प्रति (पूर्णाङ्क i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          s1->state = BEGIN_STATE;
          s2->state = BEGIN_STATE;
          यावत् (true)
            {
              c1 = get_अग्र (s1, &class_s1);
              c2 = get_अग्र (s2, &class_s2);

              यदि (class_s1 == UL_अधस्तनम् && class_s2 == UL_ऊर्ध्वस्थम्)
                {
                  प्रति (पूर्णाङ्क i = 0; i < N_CHARS; i++)
                    यदि (islower (i))
                      xlate[i] = महाक्षरम् (i);
                }
              अन्यथा यदि (class_s1 == UL_ऊर्ध्वस्थम् && class_s2 == UL_अधस्तनम्)
                {
                  प्रति (पूर्णाङ्क i = 0; i < N_CHARS; i++)
                    यदि (isupper (i))
                      xlate[i] = tolower (i);
                }
              अन्यथा
                {
                  /* The following should have been checked by validate...  */
                  यदि (c1 == -1 || c2 == -1)
                    भङ्ग;
                  xlate[c1] = c2;
                }

              /* When case-converting, skip the elements as an optimization.  */
              यदि (class_s2 != UL_NONE)
                {
                  skip_construct (s1);
                  skip_construct (s2);
                }
            }
          affirm (c1 == -1 || truncate_set1);
        }
      यदि (squeeze_repeats)
        {
          समुच्चयः_initialize (s2, false, अन्तर्_squeeze_समुच्चयः);
          squeeze_filter (io_buf, परिमाण io_buf, read_and_xlate);
        }
      अन्यथा
        {
          यावत् (true)
            {
              size_t अष्टकानि_read = read_and_xlate (io_buf, परिमाण io_buf);
              यदि (अष्टकानि_read == 0)
                भङ्ग;
              यदि (fwrite (io_buf, 1, अष्टकानि_read, stdout) != अष्टकानि_read)
                write_error ();
            }
        }
    }

  यदि (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  main_exit (EXIT_SUCCESS);
}
