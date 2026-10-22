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

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <getopt.h>

#समावेश "system.h"
#समावेश "assure.h"
#समावेश "c-ctype.h"
#समावेश "fadvise.h"
#समावेश "quote.h"
#समावेश "xbinary-io.h"
#समावेश "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "tr"

#घोषणा AUTHORS proper_name ("Jim Meyering")

क्रमागत { N_CHARS = अच_अक्षर_उच्च + 1 };

/* An unsigned integer type big enough to hold a repeat count or an
   unsigned character.  POSIX requires support for repeat counts as
   high as 2**31 - 1.  Since repeat counts might need to expand to
   match the length of an argument string, we need at least size_t to
   avoid arbitrary internal limits.  It doesn't cost much to use
   uintmax_t, though.  */
प्रकार uintmax_t गिनती;

/* The value for Spec_list->state that indicates to
   get_next that it should initialize the tail pointer.
   Its value should be as large as possible to avoid conflict
   a valid value for the state field -- and that may be as
   large as any valid repeat_count.  */
#घोषणा BEGIN_STATE (UINTMAX_MAX - 1)

/* The value for Spec_list->state that indicates to
   get_next that the element pointed to by Spec_list->tail is
   being considered for the first time on this pass through the
   list -- it indicates that get_next should make any necessary
   initializations.  */
#घोषणा NEW_ELEMENT (BEGIN_STATE + 1)

/* The maximum possible repeat count.  Due to how the states are
   implemented, it can be as much as BEGIN_STATE.  */
#घोषणा REPEAT_गिनती_MAXIMUM BEGIN_STATE

/* The following (but not CC_NO_CLASS) are indices into the array of
   valid character class strings.  */
क्रमागत Char_class
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
क्रमागत Upper_Lower_class
  {
    UL_LOWER,
    UL_UPPER,
    UL_NONE
  };

/* The type of a List_element.  See build_spec_list for more details.  */
क्रमागत Range_element_type
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
काष्ठा List_element
  {
    क्रमागत Range_element_type type;
    काष्ठा List_element *अग्र;
    जोड़
      {
        अचिन्हित अक्षर normal_char;
        काष्ठा			/* unnamed */
          {
            अचिन्हित अक्षर first_char;
            अचिन्हित अक्षर last_char;
          }
        range;
        क्रमागत Char_class char_class;
        अचिन्हित अक्षर equiv_code;
        काष्ठा			/* unnamed */
          {
            अचिन्हित अक्षर the_repeated_char;
            गिनती repeat_गिनती;
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
काष्ठा Spec_list
  {
    /* Points to the head of the list of range elements.
       The first struct is a dummy; its members are never used.  */
    काष्ठा List_element *शीर्ष;

    /* When appending, points to the last element.  When traversing via
       get_next(), points to the element to process next.  Setting
       Spec_list.state to the value BEGIN_STATE before calling get_next
       signals get_next to initialize tail to point to head->next.  */
    काष्ठा List_element *पुच्छ;

    /* Used to save state between calls to get_next.  */
    गिनती state;

    /* Length, in the sense that length ('a-z[:digit:]123abc')
       is 42 ( = 26 + 10 + 6).  */
    गिनती लंबाई;

    /* The number of [c*] and [c*0] constructs that appear in this spec.  */
    माप_प्रकार n_indefinite_repeats;

    /* If n_indefinite_repeats is nonzero, this points to the List_element
       corresponding to the last [c*] or [c*0] construct encountered in
       this spec.  Otherwise it is undefined.  */
    काष्ठा List_element *indefinite_repeat_element;

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
काष्ठा E_string
{
  अक्षर *s;
  bool *escaped;
  माप_प्रकार len;
};

/* Return nonzero if the Ith character of escaped string ES matches C
   and is not escaped itself.  */
अटल अंतरभूत bool
es_match (काष्ठा E_string स्थिर *es, माप_प्रकार i, अक्षर c)
{
  वापस es->s[i] == c && !es->escaped[i];
}

/* When true, each sequence in the input of a repeated character
   (call it c) is replaced (in the output) by a single occurrence of c
   for every c in the squeeze set.  */
अटल bool squeeze_repeats = false;

/* When true, removes characters in the delete set from input.  */
अटल bool delete = false;

/* Use the complement of set1 in place of set1.  */
अटल bool complement = false;

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
अटल bool truncate_set1 = false;

/* An alias for (!delete && non_option_args == 2).
   It is set in main and used there and in validate().  */
अटल bool translating;

अटल अक्षर io_buf[बफ_मान];

अटल अक्षर स्थिर *स्थिर char_class_name[] =
{
  "alnum", "alpha", "blank", "cntrl", "digit", "graph",
  "lower", "print", "punct", "space", "upper", "xdigit"
};

/* Array of boolean values.  A character 'c' is a member of the
   squeeze set if and only if in_squeeze_set[c] is true.  The squeeze
   set is defined by the last (possibly, the only) string argument
   on the command line when the squeeze option is given.  */
अटल bool in_squeeze_set[N_CHARS];

/* Array of boolean values.  A character 'c' is a member of the
   delete set if and only if in_delete_set[c] is true.  The delete
   set is defined by the first (or only) string argument on the
   command line when the delete option is given.  */
अटल bool in_delete_set[N_CHARS];

/* Array of character values defining the translation (if any) that
   tr is to perform.  Translation is performed only when there are
   two specification strings and the delete switch is not given.  */
अटल अक्षर xlate[N_CHARS];

अटल काष्ठा option स्थिर long_options[] =
{
  {"complement", no_argument, शून्य, 'c'},
  {"delete", no_argument, शून्य, 'd'},
  {"squeeze-repeats", no_argument, शून्य, 's'},
  {"truncate-set1", no_argument, शून्य, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... STRING1 [STRING2]\n\
"),
              program_name);
      ख_माला_दो (_("\
Translate, squeeze, and/or delete characters from standard निवेश,\n\
writing to standard output.  STRING1 and STRING2 specify arrays of\n\
characters ARRAY1 and ARRAY2 that control the action.\n\
\n\
"), मानक_निकास);
      oputs (_("\
  -c, -C, --complement\n\
         use the complement of ARRAY1\n\
"));
      oputs (_("\
  -d, --delete\n\
         delete characters in ARRAY1, करो not translate\n\
"));
      oputs (_("\
  -s, --squeeze-repeats\n\
         replace each sequence of a repeated character\n\
         that is listed in the last specified ARRAY,\n\
         with a single occurrence of that character\n\
"));
      oputs (_("\
  -t, --truncate-set1\n\
         first truncate ARRAY1 to लंबाई of ARRAY2\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
ARRAYs are specified as strings of characters.  Most represent themselves.\n\
Interpreted sequences are:\n\
\n\
  \\NNN            character with octal मूल्य NNN (1 to 3 octal digits)\n\
  \\\\              backslash\n\
  \\a              audible BEL\n\
  \\b              backspace\n\
  \\f              form feed\n\
  \\n              new पंक्ति\n\
  \\r              वापस\n\
  \\t              horizontal tab\n\
"), मानक_निकास);
     ख_माला_दो (_("\
  \\v              vertical tab\n\
  CHAR1-CHAR2     all characters from CHAR1 to CHAR2 in ascending order\n\
  [CHAR*]         in ARRAY2, copies of CHAR until लंबाई of ARRAY1\n\
  [CHAR*REPEAT]   REPEAT copies of CHAR, REPEAT octal अगर starting with 0\n\
  [:alnum:]       all letters and digits\n\
  [:alpha:]       all letters\n\
  [:blank:]       all horizontal whitespace\n\
  [:cntrl:]       all control characters\n\
  [:digit:]       all digits\n\
"), मानक_निकास);
     ख_माला_दो (_("\
  [:graph:]       all printable characters, not including space\n\
  [:lower:]       all lower हाल letters\n\
  [:print:]       all printable characters, including space\n\
  [:punct:]       all punctuation characters\n\
  [:space:]       all horizontal or vertical whitespace\n\
  [:upper:]       all upper हाल letters\n\
  [:xdigit:]      all hexadecimal digits\n\
  [=CHAR=]        all characters which are equivalent to CHAR\n\
"), मानक_निकास);
     ख_माला_दो (_("\
\n\
Translation occurs अगर -d is not given and both STRING1 and STRING2 appear.\n\
-t is only significant when translating.  ARRAY2 is extended to लंबाई of\n\
ARRAY1 by repeating its last character as necessary.  Excess characters\n\
of ARRAY2 are ignored.  Character classes expand in unspecified order;\n\
जबतक translating, '[:lower:]' and '[:upper:]' may be used in pairs to\n\
specify हाल conversion.  Squeezing occurs after translation or deletion.\n\
Arguments like '[...]' should be quoted, to avoid potential shell globbing.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Return nonzero if the character C is a member of the
   equivalence class containing the character EQUIV_CLASS.  */

अटल अंतरभूत bool
is_equiv_class_member (अचिन्हित अक्षर equiv_class, अचिन्हित अक्षर c)
{
  वापस (equiv_class == c);
}

/* Return true if the character C is a member of the
   character class CHAR_CLASS.  */

ATTRIBUTE_PURE
अटल bool
is_char_class_member (क्रमागत Char_class char_class, अचिन्हित अक्षर c)
{
  पूर्णांक परिणाम;

  चयन (char_class)
    {
    हाल CC_ALNUM:
      परिणाम = है_अक्षर_अंक (c);
      अवरोध;
    हाल CC_ALPHA:
      परिणाम = है_अक्षर (c);
      अवरोध;
    हाल CC_BLANK:
      परिणाम = isblank (c);
      अवरोध;
    हाल CC_CNTRL:
      परिणाम = है_नियंत्रण (c);
      अवरोध;
    हाल CC_DIGIT:
      परिणाम = c_isdigit (c);
      अवरोध;
    हाल CC_GRAPH:
      परिणाम = है_चित्र (c);
      अवरोध;
    हाल CC_LOWER:
      परिणाम = है_छोटा (c);
      अवरोध;
    हाल CC_PRINT:
      परिणाम = है_छाप (c);
      अवरोध;
    हाल CC_PUNCT:
      परिणाम = है_विराम (c);
      अवरोध;
    हाल CC_SPACE:
      परिणाम = है_खाली (c);
      अवरोध;
    हाल CC_UPPER:
      परिणाम = है_बड़ा (c);
      अवरोध;
    हाल CC_XDIGIT:
      परिणाम = c_isxdigit (c);
      अवरोध;
    हाल CC_NO_CLASS: शेष:
      affirm (false);
    }

  वापस !! परिणाम;
}

अटल व्योम
es_free (काष्ठा E_string *es)
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

अटल bool
unquote (अक्षर स्थिर *s, काष्ठा E_string *es)
{
  माप_प्रकार len = म_माप (s);

  es->s = xmalloc (len);
  es->escaped = xcalloc (len, माप es->escaped[0]);

  अचिन्हित पूर्णांक j = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; s[i]; i++)
    {
      अचिन्हित अक्षर c;
      पूर्णांक oct_digit;

      चयन (s[i])
        {
        हाल '\\':
          es->escaped[j] = true;
          चयन (s[i + 1])
            {
            हाल '\\':
              c = '\\';
              अवरोध;
            हाल 'a':
              c = '\a';
              अवरोध;
            हाल 'b':
              c = '\b';
              अवरोध;
            हाल 'f':
              c = '\f';
              अवरोध;
            हाल 'n':
              c = '\n';
              अवरोध;
            हाल 'r':
              c = '\r';
              अवरोध;
            हाल 't':
              c = '\t';
              अवरोध;
            हाल 'v':
              c = '\v';
              अवरोध;
            हाल '0':
            हाल '1':
            हाल '2':
            हाल '3':
            हाल '4':
            हाल '5':
            हाल '6':
            हाल '7':
              c = s[i + 1] - '0';
              oct_digit = s[i + 2] - '0';
              अगर (0 <= oct_digit && oct_digit <= 7)
                {
                  c = 8 * c + oct_digit;
                  ++i;
                  oct_digit = s[i + 2] - '0';
                  अगर (0 <= oct_digit && oct_digit <= 7)
                    {
                      अगर (8 * c + oct_digit < N_CHARS)
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
              अवरोध;
            हाल '\0':
              error (0, 0, _("warning: an unescaped backslash "
                             "at end of string is not portable"));
              /* POSIX is not clear about this.  */
              es->escaped[j] = false;
              i--;
              c = '\\';
              अवरोध;
            शेष:
              c = s[i + 1];
              अवरोध;
            }
          ++i;
          es->s[j++] = c;
          अवरोध;
        शेष:
          es->s[j++] = s[i];
          अवरोध;
        }
    }
  es->len = j;
  वापस true;
}

/* If CLASS_STR is a valid character class string, return its index
   in the global char_class_name array.  Otherwise, return CC_NO_CLASS.  */

ATTRIBUTE_PURE
अटल क्रमागत Char_class
look_up_char_class (अक्षर स्थिर *class_str, माप_प्रकार len)
{
  क्रम (क्रमागत Char_class i = 0; i < countof (char_class_name); i++)
    अगर (STREQ_LEN (class_str, char_class_name[i], len)
        && म_माप (char_class_name[i]) == len)
      वापस i;
  वापस CC_NO_CLASS;
}

/* Return a newly allocated string with a printable version of C.
   This function is used solely for formatting error messages.  */

अटल अक्षर *
make_printable_char (अचिन्हित अक्षर c)
{
  अक्षर *buf = xmalloc (5);

  अगर (है_छाप (c))
    {
      buf[0] = c;
      buf[1] = '\0';
    }
  अन्यथा
    {
      प्र_लिखो (buf, "\\%03o", c);
    }
  वापस buf;
}

/* Return a newly allocated copy of S which is suitable for printing.
   LEN is the number of characters in S.  Most non-printing
   (isprint) characters are represented by a backslash followed by
   3 octal digits.  However, the characters represented by \c escapes
   where c is one of [abfnrtv] are represented by their 2-character \c
   sequences.  This function is used solely for printing error messages.  */

अटल अक्षर *
make_printable_str (अक्षर स्थिर *s, माप_प्रकार len)
{
  /* Worst case is that every character expands to a backslash
     followed by a 3-character octal escape sequence.  */
  अक्षर *printable_buf = xnmalloc (len + 1, 4);
  अक्षर *p = printable_buf;

  क्रम (माप_प्रकार i = 0; i < len; i++)
    {
      अक्षर buf[5];
      अक्षर स्थिर *tmp = शून्य;
      अचिन्हित अक्षर c = s[i];

      चयन (c)
        {
        हाल '\\':
          tmp = "\\";
          अवरोध;
        हाल '\a':
          tmp = "\\a";
          अवरोध;
        हाल '\b':
          tmp = "\\b";
          अवरोध;
        हाल '\f':
          tmp = "\\f";
          अवरोध;
        हाल '\n':
          tmp = "\\n";
          अवरोध;
        हाल '\r':
          tmp = "\\r";
          अवरोध;
        हाल '\t':
          tmp = "\\t";
          अवरोध;
        हाल '\v':
          tmp = "\\v";
          अवरोध;
        शेष:
          अगर (है_छाप (c))
            {
              buf[0] = c;
              buf[1] = '\0';
            }
          अन्यथा
            प्र_लिखो (buf, "\\%03o", c);
          tmp = buf;
          अवरोध;
        }
      p = stpcpy (p, tmp);
    }
  वापस printable_buf;
}

/* Append a newly allocated structure representing a
   character C to the specification list LIST.  */

अटल व्योम
append_normal_char (काष्ठा Spec_list *list, अचिन्हित अक्षर c)
{
  काष्ठा List_element *new = xmalloc (माप *new);
  new->अग्र = शून्य;
  new->type = RE_NORMAL_CHAR;
  new->u.normal_char = c;
  list->पुच्छ->अग्र = new;
  list->पुच्छ = new;
}

/* Append a newly allocated structure representing the range
   of characters from FIRST to LAST to the specification list LIST.
   Return false if LAST precedes FIRST in the collating sequence,
   true otherwise.  This means that '[c-c]' is acceptable.  */

अटल bool
append_range (काष्ठा Spec_list *list, अचिन्हित अक्षर first, अचिन्हित अक्षर last)
{
  अगर (last < first)
    {
      अक्षर *tmp1 = make_printable_char (first);
      अक्षर *tmp2 = make_printable_char (last);

      error (0, 0,
       _("range-endpoints of '%s-%s' are in reverse collating sequence order"),
             tmp1, tmp2);
      मुक्त (tmp1);
      मुक्त (tmp2);
      वापस false;
    }
  काष्ठा List_element *new = xmalloc (माप *new);
  new->अग्र = शून्य;
  new->type = RE_RANGE;
  new->u.range.first_char = first;
  new->u.range.last_char = last;
  list->पुच्छ->अग्र = new;
  list->पुच्छ = new;
  वापस true;
}

/* If CHAR_CLASS_STR is a valid character class string, append a
   newly allocated structure representing that character class to the end
   of the specification list LIST and return true.  If CHAR_CLASS_STR is not
   a valid string return false.  */

अटल bool
append_char_class (काष्ठा Spec_list *list,
                   अक्षर स्थिर *char_class_str, माप_प्रकार len)
{
  क्रमागत Char_class char_class = look_up_char_class (char_class_str, len);
  अगर (char_class == CC_NO_CLASS)
    वापस false;
  काष्ठा List_element *new = xmalloc (माप *new);
  new->अग्र = शून्य;
  new->type = RE_CHAR_CLASS;
  new->u.char_class = char_class;
  list->पुच्छ->अग्र = new;
  list->पुच्छ = new;
  वापस true;
}

/* Append a newly allocated structure representing a [c*n]
   repeated character construct to the specification list LIST.
   THE_CHAR is the single character to be repeated, and REPEAT_COUNT
   is a non-negative repeat count.  */

अटल व्योम
append_repeated_char (काष्ठा Spec_list *list, अचिन्हित अक्षर the_char,
                      गिनती repeat_गिनती)
{
  काष्ठा List_element *new = xmalloc (माप *new);
  new->अग्र = शून्य;
  new->type = RE_REPEATED_CHAR;
  new->u.repeated_char.the_repeated_char = the_char;
  new->u.repeated_char.repeat_गिनती = repeat_गिनती;
  list->पुच्छ->अग्र = new;
  list->पुच्छ = new;
}

/* Given a string, EQUIV_CLASS_STR, from a [=str=] context and
   the length of that string, LEN, if LEN is exactly one, append
   a newly allocated structure representing the specified
   equivalence class to the specification list, LIST and return true.
   If LEN is not 1, return false.  */

अटल bool
append_equiv_class (काष्ठा Spec_list *list,
                    अक्षर स्थिर *equiv_class_str, माप_प्रकार len)
{
  अगर (len != 1)
    वापस false;

  काष्ठा List_element *new = xmalloc (माप *new);
  new->अग्र = शून्य;
  new->type = RE_EQUIV_CLASS;
  new->u.equiv_code = *equiv_class_str;
  list->पुच्छ->अग्र = new;
  list->पुच्छ = new;
  वापस true;
}

/* Search forward starting at START_IDX for the 2-char sequence
   (PRE_BRACKET_CHAR,']') in the string P of length P_LEN.  If such
   a sequence is found, set *RESULT_IDX to the index of the first
   character and return true.  Otherwise return false.  P may contain
   zero bytes.  */

अटल bool
find_closing_delim (स्थिर काष्ठा E_string *es, माप_प्रकार start_idx,
                    अक्षर pre_bracket_char, माप_प्रकार *परिणाम_idx)
{
  क्रम (माप_प्रकार i = start_idx; i < es->len - 1; i++)
    अगर (es->s[i] == pre_bracket_char && es->s[i + 1] == ']'
        && !es->escaped[i] && !es->escaped[i + 1])
      {
        *परिणाम_idx = i;
        वापस true;
      }
  वापस false;
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

अटल पूर्णांक
find_bracketed_repeat (स्थिर काष्ठा E_string *es, माप_प्रकार start_idx,
                       अचिन्हित अक्षर *char_to_repeat, गिनती *repeat_गिनती,
                       माप_प्रकार *closing_bracket_idx)
{
  affirm (start_idx + 1 < es->len);
  अगर (!es_match (es, start_idx + 1, '*'))
    वापस -1;

  क्रम (माप_प्रकार i = start_idx + 2; i < es->len && !es->escaped[i]; i++)
    {
      अगर (es->s[i] == ']')
        {
          माप_प्रकार digit_str_len = i - start_idx - 2;

          *char_to_repeat = es->s[start_idx];
          अगर (digit_str_len == 0)
            {
              /* We've matched [c*] -- no explicit repeat count.  */
              *repeat_गिनती = 0;
            }
          अन्यथा
            {
              /* Here, we have found [c*s] where s should be a string
                 of octal (if it starts with '0') or decimal digits.  */
              अक्षर स्थिर *digit_str = &es->s[start_idx + 2];
              अक्षर *d_end;
              अगर ((xstrtoumax (digit_str, &d_end, *digit_str == '0' ? 8 : 10,
                               repeat_गिनती, शून्य)
                   != LONGINT_OK)
                  || REPEAT_गिनती_MAXIMUM < *repeat_गिनती
                  || digit_str + digit_str_len != d_end)
                {
                  अक्षर *tmp = make_printable_str (digit_str, digit_str_len);
                  error (0, 0,
                         _("invalid repeat count %s in [c*n] construct"),
                         quote (tmp));
                  मुक्त (tmp);
                  वापस -2;
                }
            }
          *closing_bracket_idx = i;
          वापस 0;
        }
    }
  वापस -1;			/* No bracket found.  */
}

/* Return true if the string at ES->s[IDX] matches the regular
   expression '\*[0-9]*]', false otherwise.  The string does not
   match if any of its characters are escaped.  */

ATTRIBUTE_PURE
अटल bool
star_digits_closebracket (स्थिर काष्ठा E_string *es, माप_प्रकार idx)
{
  अगर (!es_match (es, idx, '*'))
    वापस false;

  क्रम (माप_प्रकार i = idx + 1; i < es->len; i++)
    अगर (!c_isdigit (es->s[i]) || es->escaped[i])
      वापस es_match (es, i, ']');
  वापस false;
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

अटल bool
build_spec_list (स्थिर काष्ठा E_string *es, काष्ठा Spec_list *परिणाम)
{
  अक्षर स्थिर *p = es->s;

  /* The main for-loop below recognizes the 4 multi-character constructs.
     A character that matches (in its context) none of the multi-character
     constructs is classified as 'normal'.  Since all multi-character
     constructs have at least 3 characters, any strings of length 2 or
     less are composed solely of normal characters.  Hence, the index of
     the outer for-loop runs only as far as LEN-2.  */
  माप_प्रकार i;
  क्रम (i = 0; i + 2 < es->len; /* empty */)
    {
      अगर (es_match (es, i, '['))
        {
          bool matched_multi_char_construct;
          माप_प्रकार closing_bracket_idx;
          अचिन्हित अक्षर char_to_repeat;
          गिनती repeat_गिनती;
          पूर्णांक err;

          matched_multi_char_construct = true;
          अगर (es_match (es, i + 1, ':') || es_match (es, i + 1, '='))
            {
              माप_प्रकार closing_delim_idx;

              अगर (find_closing_delim (es, i + 2, p[i + 1], &closing_delim_idx))
                {
                  माप_प्रकार opnd_str_len = closing_delim_idx - 1 - (i + 2) + 1;
                  अक्षर स्थिर *opnd_str = p + i + 2;

                  अगर (opnd_str_len == 0)
                    {
                      अगर (p[i + 1] == ':')
                        error (0, 0, _("missing character class name '[::]'"));
                      अन्यथा
                        error (0, 0,
                               _("missing equivalence class character '[==]'"));
                      वापस false;
                    }

                  अगर (p[i + 1] == ':')
                    {
                      /* FIXME: big comment.  */
                      अगर (!append_char_class (परिणाम, opnd_str, opnd_str_len))
                        {
                          अगर (star_digits_closebracket (es, i + 2))
                            जाओ try_bracketed_repeat;
                          अन्यथा
                            {
                              अक्षर *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0, _("invalid character class %s"),
                                     quote (tmp));
                              मुक्त (tmp);
                              वापस false;
                            }
                        }
                    }
                  अन्यथा
                    {
                      /* FIXME: big comment.  */
                      अगर (!append_equiv_class (परिणाम, opnd_str, opnd_str_len))
                        {
                          अगर (star_digits_closebracket (es, i + 2))
                            जाओ try_bracketed_repeat;
                          अन्यथा
                            {
                              अक्षर *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0,
               _("%s: equivalence class operand must be a single character"),
                                     tmp);
                              मुक्त (tmp);
                              वापस false;
                            }
                        }
                    }

                  i = closing_delim_idx + 2;
                  जारी;
                }
              /* Else fall through.  This could be [:*] or [=*].  */
            }

        try_bracketed_repeat:

          /* Determine whether this is a bracketed repeat range
             matching the RE \[.\*(dec_or_oct_number)?].  */
          err = find_bracketed_repeat (es, i + 1, &char_to_repeat,
                                       &repeat_गिनती,
                                       &closing_bracket_idx);
          अगर (err == 0)
            {
              append_repeated_char (परिणाम, char_to_repeat, repeat_गिनती);
              i = closing_bracket_idx + 1;
            }
          अन्यथा अगर (err == -1)
            {
              matched_multi_char_construct = false;
            }
          अन्यथा
            {
              /* Found a string that looked like [c*n] but the
                 numeric part was invalid.  */
              वापस false;
            }

          अगर (matched_multi_char_construct)
            जारी;

          /* We reach this point if P does not match [:str:], [=c=],
             [c*n], or [c*].  Now, see if P looks like a range '[-c'
             (from '[' to 'c').  */
        }

      /* Look ahead one char for ranges like a-z.  */
      अगर (es_match (es, i + 1, '-'))
        {
          अगर (!append_range (परिणाम, p[i], p[i + 2]))
            वापस false;
          i += 3;
        }
      अन्यथा
        {
          append_normal_char (परिणाम, p[i]);
          ++i;
        }
    }

  /* Now handle the (2 or fewer) remaining characters p[i]..p[es->len - 1].  */
  क्रम (; i < es->len; i++)
    append_normal_char (परिणाम, p[i]);

  वापस true;
}

/* Advance past the current construct.
   S->tail must be non-null.  */
अटल व्योम
skip_construct (काष्ठा Spec_list *s)
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

अटल पूर्णांक
get_अग्र (काष्ठा Spec_list *s, क्रमागत Upper_Lower_class *class)
{
  काष्ठा List_element *p;
  पूर्णांक return_val;
  पूर्णांक i;

  अगर (class)
    *class = UL_NONE;

  अगर (s->state == BEGIN_STATE)
    {
      s->पुच्छ = s->शीर्ष->अग्र;
      s->state = NEW_ELEMENT;
    }

  p = s->पुच्छ;
  अगर (p == शून्य)
    वापस -1;

  चयन (p->type)
    {
    हाल RE_NORMAL_CHAR:
      return_val = p->u.normal_char;
      s->state = NEW_ELEMENT;
      s->पुच्छ = p->अग्र;
      अवरोध;

    हाल RE_RANGE:
      अगर (s->state == NEW_ELEMENT)
        s->state = p->u.range.first_char;
      अन्यथा
        ++(s->state);
      return_val = s->state;
      अगर (s->state == p->u.range.last_char)
        {
          s->पुच्छ = p->अग्र;
          s->state = NEW_ELEMENT;
        }
      अवरोध;

    हाल RE_CHAR_CLASS:
      अगर (class)
        {
          चयन (+p->u.char_class)
            {
            हाल CC_LOWER:
              *class = UL_LOWER;
              अवरोध;
            हाल CC_UPPER:
              *class = UL_UPPER;
              अवरोध;
            }
        }

      अगर (s->state == NEW_ELEMENT)
        {
          क्रम (i = 0; i < N_CHARS; i++)
            अगर (is_char_class_member (p->u.char_class, i))
              अवरोध;
          affirm (i < N_CHARS);
          s->state = i;
        }
      assure (is_char_class_member (p->u.char_class, s->state));
      return_val = s->state;
      क्रम (i = s->state + 1; i < N_CHARS; i++)
        अगर (is_char_class_member (p->u.char_class, i))
          अवरोध;
      अगर (i < N_CHARS)
        s->state = i;
      अन्यथा
        {
          s->पुच्छ = p->अग्र;
          s->state = NEW_ELEMENT;
        }
      अवरोध;

    हाल RE_EQUIV_CLASS:
      /* FIXME: this assumes that each character is alone in its own
         equivalence class (which appears to be correct for my
         LC_COLLATE.  But I don't know of any function that allows
         one to determine a character's equivalence class.  */

      return_val = p->u.equiv_code;
      s->state = NEW_ELEMENT;
      s->पुच्छ = p->अग्र;
      अवरोध;

    हाल RE_REPEATED_CHAR:
      /* Here, a repeat count of n == 0 means don't repeat at all.  */
      अगर (p->u.repeated_char.repeat_गिनती == 0)
        {
          s->पुच्छ = p->अग्र;
          s->state = NEW_ELEMENT;
          return_val = get_अग्र (s, class);
        }
      अन्यथा
        {
          अगर (s->state == NEW_ELEMENT)
            {
              s->state = 0;
            }
          ++(s->state);
          return_val = p->u.repeated_char.the_repeated_char;
          अगर (s->state == p->u.repeated_char.repeat_गिनती)
            {
              s->पुच्छ = p->अग्र;
              s->state = NEW_ELEMENT;
            }
        }
      अवरोध;

    शेष:
      unreachable ();
    }

  वापस return_val;
}

/* This is a minor kludge.  This function is called from
   get_spec_stats to determine the cardinality of a set derived
   from a complemented string.  It's a kludge in that some of the
   same operations are (duplicated) performed in set_initialize.  */

अटल पूर्णांक
card_of_complement (काष्ठा Spec_list *s)
{
  पूर्णांक c;
  पूर्णांक cardinality = N_CHARS;
  bool in_set[N_CHARS] = {0};

  s->state = BEGIN_STATE;
  जबतक ((c = get_अग्र (s, शून्य)) != -1)
    {
      cardinality -= (!in_set[c]);
      in_set[c] = true;
    }
  वापस cardinality;
}

/* Discard the lengths associated with a case conversion,
   as using the actual number of upper or lower case characters
   is problematic when they don't match in some locales.
   Also ensure the case conversion classes in string2 are
   aligned correctly with those in string1.
   Note POSIX says the behavior of 'tr "[:upper:]" "[:upper:]"'
   is undefined.  Therefore we allow it (unlike Solaris)
   and treat it as a no-op.  */

अटल व्योम
validate_case_classes (काष्ठा Spec_list *s1, काष्ठा Spec_list *s2)
{
  माप_प्रकार n_upper = 0;
  माप_प्रकार n_lower = 0;
  पूर्णांक c1 = 0;
  पूर्णांक c2 = 0;
  MAYBE_UNUSED गिनती old_s1_len = s1->लंबाई, old_s2_len = s2->लंबाई;
  काष्ठा List_element *s1_पुच्छ = s1->पुच्छ;
  काष्ठा List_element *s2_पुच्छ = s2->पुच्छ;
  bool s1_new_element = true;
  bool s2_new_element = true;

  अगर (complement || !s2->has_char_class)
    वापस;

  क्रम (पूर्णांक i = 0; i < N_CHARS; i++)
    {
      अगर (है_बड़ा (i))
        n_upper++;
      अगर (है_छोटा (i))
        n_lower++;
    }

  s1->state = BEGIN_STATE;
  s2->state = BEGIN_STATE;

  जबतक (c1 != -1 && c2 != -1)
    {
      क्रमागत Upper_Lower_class class_s1, class_s2;

      c1 = get_अग्र (s1, &class_s1);
      c2 = get_अग्र (s2, &class_s2);

      /* If c2 transitions to a new case class, then
         c1 must also transition at the same time.  */
      अगर (s2_new_element && class_s2 != UL_NONE
          && !(s1_new_element && class_s1 != UL_NONE))
        error (निकास_त्रुटि, 0,
               _("misaligned [:upper:] and/or [:lower:] construct"));

      /* If case converting, quickly skip over the elements.  */
      अगर (class_s2 != UL_NONE)
        {
          skip_construct (s1);
          skip_construct (s2);
          /* Discount insignificant/problematic lengths.  */
          s1->लंबाई -= (class_s1 == UL_UPPER ? n_upper : n_lower) - 1;
          s2->लंबाई -= (class_s2 == UL_UPPER ? n_upper : n_lower) - 1;
        }

      s1_new_element = s1->state == NEW_ELEMENT; /* Next element is new.  */
      s2_new_element = s2->state == NEW_ELEMENT; /* Next element is new.  */
    }

  affirm (old_s1_len >= s1->लंबाई && old_s2_len >= s2->लंबाई);

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

अटल व्योम
get_spec_stats (काष्ठा Spec_list *s)
{
  गिनती लंबाई = 0;

  s->n_indefinite_repeats = 0;
  s->has_equiv_class = false;
  s->has_restricted_char_class = false;
  s->has_char_class = false;
  क्रम (काष्ठा List_element *p = s->शीर्ष->अग्र; p; p = p->अग्र)
    {
      गिनती len = 0;

      चयन (p->type)
        {
        हाल RE_NORMAL_CHAR:
          len = 1;
          अवरोध;

        हाल RE_RANGE:
          affirm (p->u.range.last_char >= p->u.range.first_char);
          len = p->u.range.last_char - p->u.range.first_char + 1;
          अवरोध;

        हाल RE_CHAR_CLASS:
          s->has_char_class = true;
          क्रम (पूर्णांक i = 0; i < N_CHARS; i++)
            अगर (is_char_class_member (p->u.char_class, i))
              ++len;
          चयन (+p->u.char_class)
            {
            हाल CC_UPPER:
            हाल CC_LOWER:
              अवरोध;
            शेष:
              s->has_restricted_char_class = true;
              अवरोध;
            }
          अवरोध;

        हाल RE_EQUIV_CLASS:
          क्रम (पूर्णांक i = 0; i < N_CHARS; i++)
            अगर (is_equiv_class_member (p->u.equiv_code, i))
              ++len;
          s->has_equiv_class = true;
          अवरोध;

        हाल RE_REPEATED_CHAR:
          अगर (p->u.repeated_char.repeat_गिनती > 0)
            len = p->u.repeated_char.repeat_गिनती;
          अन्यथा
            {
              s->indefinite_repeat_element = p;
              ++(s->n_indefinite_repeats);
            }
          अवरोध;

        शेष:
          unreachable ();
        }

      /* Check for arithmetic overflow in computing length.  Also, reject
         any length greater than the maximum repeat count, in case the
         length is later used to compute the repeat count for an
         indefinite element.  */
      अगर (ckd_add (&लंबाई, लंबाई, len) || REPEAT_गिनती_MAXIMUM < लंबाई)
        error (निकास_त्रुटि, 0, _("too many characters in set"));
    }

  s->लंबाई = लंबाई;
}

अटल व्योम
get_s1_spec_stats (काष्ठा Spec_list *s1)
{
  get_spec_stats (s1);
  अगर (complement)
    s1->लंबाई = card_of_complement (s1);
}

अटल व्योम
get_s2_spec_stats (काष्ठा Spec_list *s2, गिनती len_s1)
{
  get_spec_stats (s2);
  अगर (len_s1 >= s2->लंबाई && s2->n_indefinite_repeats == 1)
    {
      s2->indefinite_repeat_element->u.repeated_char.repeat_गिनती =
        len_s1 - s2->लंबाई;
      s2->लंबाई = len_s1;
    }
}

अटल व्योम
spec_init (काष्ठा Spec_list *spec_list)
{
  काष्ठा List_element *new = xmalloc (माप *new);
  spec_list->शीर्ष = spec_list->पुच्छ = new;
  spec_list->शीर्ष->अग्र = शून्य;
}

/* This function makes two passes over the argument string S.  The first
   one converts all \c and \ddd escapes to their one-byte representations.
   The second constructs a linked specification list, SPEC_LIST, of the
   characters and constructs that comprise the argument string.  If either
   of these passes detects an error, this function returns false.  */

अटल bool
parse_str (अक्षर स्थिर *s, काष्ठा Spec_list *spec_list)
{
  काष्ठा E_string es;
  bool ok = unquote (s, &es) && build_spec_list (&es, spec_list);
  es_free (&es);
  वापस ok;
}

/* Given two specification lists, S1 and S2, and assuming that
   S1->length > S2->length, append a single [c*n] element to S2 where c
   is the last character in the expansion of S2 and n is the difference
   between the two lengths.
   Upon successful completion, S2->length is set to S1->length.  The only
   way this function can fail to make S2 as long as S1 is when S2 has
   zero-length, since in that case, there is no last character to repeat.
   So S2->length is required to be at least 1.  */

अटल व्योम
string2_extend (स्थिर काष्ठा Spec_list *s1, काष्ठा Spec_list *s2)
{
  काष्ठा List_element *p;
  अचिन्हित अक्षर char_to_repeat;

  affirm (translating);
  affirm (s1->लंबाई > s2->लंबाई);
  affirm (s2->लंबाई > 0);

  p = s2->पुच्छ;
  चयन (p->type)
    {
    हाल RE_NORMAL_CHAR:
      char_to_repeat = p->u.normal_char;
      अवरोध;
    हाल RE_RANGE:
      char_to_repeat = p->u.range.last_char;
      अवरोध;
    हाल RE_CHAR_CLASS:
      /* Note BSD allows extending of classes in string2.  For example:
           tr '[:upper:]0-9' '[:lower:]'
         That's not portable however, contradicts POSIX and is dependent
         on your collating sequence.  */
      error (निकास_त्रुटि, 0,
             _("when translating with string1 longer than string2,\n"
               "the latter string must not end with a character class"));

    हाल RE_REPEATED_CHAR:
      char_to_repeat = p->u.repeated_char.the_repeated_char;
      अवरोध;

    हाल RE_EQUIV_CLASS: शेष:
      /* This shouldn't happen, because validate exits with an error
         if it finds an equiv class in string2 when translating.  */
      affirm (false);
    }

  append_repeated_char (s2, char_to_repeat, s1->लंबाई - s2->लंबाई);
  s2->लंबाई = s1->लंबाई;
}

/* Return true if S is a non-empty list in which exactly one
   character (but potentially, many instances of it) appears.
   E.g., [X*] or xxxxxxxx.  */

अटल bool
homogeneous_spec_list (काष्ठा Spec_list *s)
{
  पूर्णांक b, c;

  s->state = BEGIN_STATE;

  अगर ((b = get_अग्र (s, शून्य)) == -1)
    वापस false;

  जबतक ((c = get_अग्र (s, शून्य)) != -1)
    अगर (c != b)
      वापस false;

  वापस true;
}

/* Die with an error message if S1 and S2 describe strings that
   are not valid with the given command line switches.
   A side effect of this function is that if a valid [c*] or
   [c*0] construct appears in string2, it is converted to [c*n]
   with a value for n that makes s2->length == s1->length.  By
   the same token, if the --truncate-set1 option is not
   given, S2 may be extended.  */

अटल व्योम
validate (काष्ठा Spec_list *s1, काष्ठा Spec_list *s2)
{
  get_s1_spec_stats (s1);
  अगर (s1->n_indefinite_repeats > 0)
    error (निकास_त्रुटि, 0,
           _("the [c*] repeat construct may not appear in string1"));

  अगर (s2)
    {
      get_s2_spec_stats (s2, s1->लंबाई);

      अगर (s2->n_indefinite_repeats > 1)
        error (निकास_त्रुटि, 0,
               _("only one [c*] repeat construct may appear in string2"));

      अगर (translating)
        {
          अगर (s2->has_equiv_class)
            error (निकास_त्रुटि, 0,
                   _("[=c=] expressions may not appear in string2"
                     " when translating"));

          अगर (s2->has_restricted_char_class)
            error (निकास_त्रुटि, 0,
                   _("when translating, the only character classes"
                     " that may appear in\n"
                     "string2 are 'upper' and 'lower'"));

          validate_case_classes (s1, s2);

          अगर (s1->लंबाई > s2->लंबाई)
            {
              अगर (!truncate_set1)
                {
                  /* string2 must be non-empty unless --truncate-set1 is
                     given or string1 is empty.  */

                  अगर (s2->लंबाई == 0)
                    error (निकास_त्रुटि, 0,
                           _("when not truncating set1,"
                             " string2 must be non-empty"));
                  string2_extend (s1, s2);
                }
            }

          अगर (complement && s1->has_char_class
              && ! (s2->लंबाई == s1->लंबाई && homogeneous_spec_list (s2)))
            error (निकास_त्रुटि, 0,
                   _("when translating with complemented character classes,\n"
                     "string2 must map all characters in the domain to one"));
        }
      अन्यथा
        /* Not translating.  */
        {
          अगर (s2->n_indefinite_repeats > 0)
            error (निकास_त्रुटि, 0,
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

अटल व्योम
squeeze_filter (अक्षर *buf, माप_प्रकार आकार, माप_प्रकार (*reader) (अक्षर *, माप_प्रकार))
{
  /* A value distinct from any character that may have been stored in a
     buffer as the result of a block-read in the function squeeze_filter.  */
  स्थिर पूर्णांक NOT_A_CHAR = पूर्णांक_उच्च;

  पूर्णांक char_to_squeeze = NOT_A_CHAR;
  माप_प्रकार i = 0;
  माप_प्रकार nr = 0;

  जबतक (true)
    {
      अगर (i >= nr)
        {
          nr = reader (buf, आकार);
          अगर (nr == 0)
            अवरोध;
          i = 0;
        }

      माप_प्रकार begin = i;

      अगर (char_to_squeeze == NOT_A_CHAR)
        {
          माप_प्रकार out_len;
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
          क्रम (; i < nr && !in_squeeze_set[to_uchar (buf[i])]; i += 2)
            जारी;

          /* There is a special case when i == nr and we've just
             skipped a character (the last one in buf) that is in
             the squeeze set.  */
          अगर (i == nr && in_squeeze_set[to_uchar (buf[i - 1])])
            --i;

          अगर (i >= nr)
            out_len = nr - begin;
          अन्यथा
            {
              char_to_squeeze = buf[i];
              /* We're about to output buf[begin..i].  */
              out_len = i - begin + 1;

              /* But since we stepped by 2 in the loop above,
                 out_len may be one too large.  */
              अगर (i > 0 && buf[i - 1] == char_to_squeeze)
                --out_len;

              /* Advance i to the index of first character to be
                 considered when looking for a char different from
                 char_to_squeeze.  */
              ++i;
            }
          अगर (out_len > 0
              && ख_डालो (&buf[begin], 1, out_len, मानक_निकास) != out_len)
            write_error ();
        }

      अगर (char_to_squeeze != NOT_A_CHAR)
        {
          /* Advance i to index of first char != char_to_squeeze
             (or to nr if all the rest of the characters in this
             buffer are the same as char_to_squeeze).  */
          क्रम (; i < nr && buf[i] == char_to_squeeze; i++)
            जारी;
          अगर (i < nr)
            char_to_squeeze = NOT_A_CHAR;
          /* If (i >= nr) we've squeezed the last character in this buffer.
             So now we have to read a new buffer and continue comparing
             characters against char_to_squeeze.  */
        }
    }
}

अटल माप_प्रकार
plain_read (अक्षर *buf, माप_प्रकार आकार)
{
  ssize_t nr = पढ़ो (STDIN_FILENO, buf, आकार);
  अगर (nr < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("read error"));
  वापस nr;
}

/* Read buffers of SIZE bytes from stdin until one is found that
   contains at least one character not in the delete set.  Store
   in the array BUF, all characters from that buffer that are not
   in the delete set, and return the number of characters saved
   or 0 upon EOF.  */

अटल माप_प्रकार
read_and_delete (अक्षर *buf, माप_प्रकार आकार)
{
  माप_प्रकार n_saved;

  /* This enclosing do-while loop is to make sure that
     we don't return zero (indicating EOF) when we've
     just deleted all the characters in a buffer.  */
  करो
    {
      माप_प्रकार nr = plain_read (buf, आकार);

      अगर (nr == 0)
        वापस 0;

      /* This first loop may be a waste of code, but gives much
         better performance when no characters are deleted in
         the beginning of a buffer.  It just avoids the copying
         of buf[i] into buf[n_saved] when it would be a NOP.  */

      माप_प्रकार i;
      क्रम (i = 0; i < nr && !in_delete_set[to_uchar (buf[i])]; i++)
        जारी;
      n_saved = i;

      क्रम (++i; i < nr; i++)
        अगर (!in_delete_set[to_uchar (buf[i])])
          buf[n_saved++] = buf[i];
    }
  जबतक (n_saved == 0);

  वापस n_saved;
}

/* Read at most SIZE bytes from stdin into the array BUF.  Then
   perform the in-place and one-to-one mapping specified by the global
   array 'xlate'.  Return the number of characters read, or 0 upon EOF.  */

अटल माप_प्रकार
read_and_xlate (अक्षर *buf, माप_प्रकार आकार)
{
  माप_प्रकार bytes_read = plain_read (buf, आकार);

  क्रम (माप_प्रकार i = 0; i < bytes_read; i++)
    buf[i] = xlate[to_uchar (buf[i])];

  वापस bytes_read;
}

/* Initialize a boolean membership set, IN_SET, with the character
   values obtained by traversing the linked list of constructs S
   using the function 'get_next'.  IN_SET is expected to have been
   initialized to all zeros by the caller.  If COMPLEMENT_THIS_SET
   is true the resulting set is complemented.  */

अटल व्योम
set_initialize (काष्ठा Spec_list *s, bool complement_this_set, bool *in_set)
{
  पूर्णांक c;

  s->state = BEGIN_STATE;
  जबतक ((c = get_अग्र (s, शून्य)) != -1)
    in_set[c] = true;
  अगर (complement_this_set)
    क्रम (माप_प्रकार i = 0; i < N_CHARS; i++)
      in_set[i] = (!in_set[i]);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक c;
  पूर्णांक non_option_args;
  पूर्णांक min_operands;
  पूर्णांक max_operands;
  काष्ठा Spec_list buf1, buf2;
  काष्ठा Spec_list *s1 = &buf1;
  काष्ठा Spec_list *s2 = &buf2;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((c = getopt_long (argc, argv, "+AcCdstu", long_options, शून्य)) != -1)
    {
      चयन (c)
        {
        हाल 'A':
          /* Undocumented option, for compatibility with AIX.  */
          रखो_क्षेत्र (LC_COLLATE, "C");
          रखो_क्षेत्र (LC_CTYPE, "C");
          अवरोध;

        हाल 'c':
        हाल 'C':
          complement = true;
          अवरोध;

        हाल 'd':
          delete = true;
          अवरोध;

        हाल 's':
          squeeze_repeats = true;
          अवरोध;

        हाल 't':
          truncate_set1 = true;
          अवरोध;

        हाल 'u':
          /* Undocumented option, for compatibility with FreeBSD/macOS.  */
          रखो_भबफ (मानक_निकास, शून्य, _IONBF, 0);
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
          अवरोध;
        }
    }

  non_option_args = argc - optind;
  translating = (non_option_args == 2 && !delete);
  min_operands = 1 + (delete == squeeze_repeats);
  max_operands = 1 + (delete <= squeeze_repeats);

  अगर (non_option_args < min_operands)
    {
      अगर (non_option_args == 0)
        error (0, 0, _("missing operand"));
      अन्यथा
        {
          error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
          ख_लिखो (मानक_त्रुटि, "%s\n",
                   _(squeeze_repeats
                     ? N_("Two strings must be given when "
                          "both deleting and squeezing repeats.")
                     : N_("Two strings must be given when translating.")));
        }
      usage (निकास_त्रुटि);
    }

  अगर (max_operands < non_option_args)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + max_operands]));
      अगर (non_option_args == 2)
        ख_लिखो (मानक_त्रुटि, "%s\n",
                 _("Only one string may be given when "
                   "deleting without squeezing repeats."));
      usage (निकास_त्रुटि);
    }

  spec_init (s1);
  अगर (!parse_str (argv[optind], s1))
    main_exit (निकास_त्रुटि);

  अगर (non_option_args == 2)
    {
      spec_init (s2);
      अगर (!parse_str (argv[optind + 1], s2))
        main_exit (निकास_त्रुटि);
    }
  अन्यथा
    s2 = शून्य;

  validate (s1, s2);

  /* Use binary I/O, since 'tr' is sometimes used to transliterate
     non-printable characters, or characters which are stripped away
     by text-mode reads (like CR and ^Z).  */
  xset_द्विआधारी_mode (STDIN_FILENO, O_द्विआधारी);
  xset_द्विआधारी_mode (STDOUT_FILENO, O_द्विआधारी);
  fadvise (मानक_निवेश, FADVISE_SEQUENTIAL);

  अगर (squeeze_repeats && non_option_args == 1)
    {
      set_initialize (s1, complement, in_squeeze_set);
      squeeze_filter (io_buf, माप io_buf, plain_read);
    }
  अन्यथा अगर (delete && non_option_args == 1)
    {
      set_initialize (s1, complement, in_delete_set);

      जबतक (true)
        {
          माप_प्रकार nr = read_and_delete (io_buf, माप io_buf);
          अगर (nr == 0)
            अवरोध;
          अगर (ख_डालो (io_buf, 1, nr, मानक_निकास) != nr)
            write_error ();
        }
    }
  अन्यथा अगर (squeeze_repeats && delete && non_option_args == 2)
    {
      set_initialize (s1, complement, in_delete_set);
      set_initialize (s2, false, in_squeeze_set);
      squeeze_filter (io_buf, माप io_buf, read_and_delete);
    }
  अन्यथा अगर (translating)
    {
      अगर (complement)
        {
          bool *in_s1 = in_delete_set;

          set_initialize (s1, false, in_s1);
          s2->state = BEGIN_STATE;
          क्रम (पूर्णांक i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          क्रम (पूर्णांक i = 0; i < N_CHARS; i++)
            {
              अगर (!in_s1[i])
                {
                  पूर्णांक ch = get_अग्र (s2, शून्य);
                  affirm (ch != -1 || truncate_set1);
                  अगर (ch == -1)
                    {
                      /* This will happen when tr is invoked like e.g.
                         tr -cs A-Za-z0-9 '\012'.  */
                      अवरोध;
                    }
                  xlate[i] = ch;
                }
            }
        }
      अन्यथा
        {
          पूर्णांक c1, c2;
          क्रमागत Upper_Lower_class class_s1;
          क्रमागत Upper_Lower_class class_s2;

          क्रम (पूर्णांक i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          s1->state = BEGIN_STATE;
          s2->state = BEGIN_STATE;
          जबतक (true)
            {
              c1 = get_अग्र (s1, &class_s1);
              c2 = get_अग्र (s2, &class_s2);

              अगर (class_s1 == UL_LOWER && class_s2 == UL_UPPER)
                {
                  क्रम (पूर्णांक i = 0; i < N_CHARS; i++)
                    अगर (है_छोटा (i))
                      xlate[i] = बड़े (i);
                }
              अन्यथा अगर (class_s1 == UL_UPPER && class_s2 == UL_LOWER)
                {
                  क्रम (पूर्णांक i = 0; i < N_CHARS; i++)
                    अगर (है_बड़ा (i))
                      xlate[i] = छोटे (i);
                }
              अन्यथा
                {
                  /* The following should have been checked by validate...  */
                  अगर (c1 == -1 || c2 == -1)
                    अवरोध;
                  xlate[c1] = c2;
                }

              /* When case-converting, skip the elements as an optimization.  */
              अगर (class_s2 != UL_NONE)
                {
                  skip_construct (s1);
                  skip_construct (s2);
                }
            }
          affirm (c1 == -1 || truncate_set1);
        }
      अगर (squeeze_repeats)
        {
          set_initialize (s2, false, in_squeeze_set);
          squeeze_filter (io_buf, माप io_buf, read_and_xlate);
        }
      अन्यथा
        {
          जबतक (true)
            {
              माप_प्रकार bytes_read = read_and_xlate (io_buf, माप io_buf);
              अगर (bytes_read == 0)
                अवरोध;
              अगर (ख_डालो (io_buf, 1, bytes_read, मानक_निकास) != bytes_read)
                write_error ();
            }
        }
    }

  अगर (बंद (STDIN_FILENO) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("standard input"));

  main_exit (निकास_सफल);
}
