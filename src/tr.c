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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <getopt.h>

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "fadvise.h"
#ضمن "quote.h"
#ضمن "xbinary-io.h"
#ضمن "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "tr"

#تعریف_کرو AUTHORS proper_name ("Jim Meyering")

تعداد { N_CHARS = UCHAR_MAX + 1 };

/* An unsigned integer type big enough to hold a repeat count or an
   unsigned character.  POSIX requires support for repeat counts as
   high as 2**31 - 1.  Since repeat counts might need to expand to
   match the length of an argument string, we need at least size_t to
   avoid arbitrary internal limits.  It doesn't cost much to use
   uintmax_t, though.  */
تعريفنوع uintmax_t گنتی;

/* The value for Spec_list->state that indicates to
   get_next that it should initialize the tail pointer.
   Its value should be as large as possible to avoid conflict
   a valid value for the state field -- and that may be as
   large as any valid repeat_count.  */
#تعریف_کرو BEGIN_STATE (UINTMAX_MAX - 1)

/* The value for Spec_list->state that indicates to
   get_next that the element pointed to by Spec_list->tail is
   being considered for the first time on this pass through the
   list -- it indicates that get_next should make any necessary
   initializations.  */
#تعریف_کرو NEW_ELEMENT (BEGIN_STATE + 1)

/* The maximum possible repeat count.  Due to how the states are
   implemented, it can be as much as BEGIN_STATE.  */
#تعریف_کرو REPEAT_گنتی_ودھ_توں_ودھ BEGIN_STATE

/* The following (but not CC_NO_CLASS) are indices into the array of
   valid character class strings.  */
تعداد Char_class
  {
    CC_ALNUM = 0, CC_ALPHA = 1, CC_BLANK = 2, CC_CNTRL = 3,
    CC_DIGIT = 4, CC_گراف = 5, CC_ہیٹھلا = 6, CC_PRINT = 7,
    CC_PUNCT = 8, CC_SPACE = 9, CC_اتلا = 10, CC_XDIGIT = 11,
    CC_NO_CLASS = 9999
  };

/* Character class to which a character (returned by get_next) belonged;
   but it is set only if the construct from which the character was obtained
   was one of the character classes [:upper:] or [:lower:].  The value
   is used only when translating and then, only to make sure that upper
   and lower class constructs have the same relative positions in string1
   and string2.  */
تعداد اتلا_ہیٹھلا_class
  {
    UL_ہیٹھلا,
    UL_اتلا,
    UL_NONE
  };

/* The type of a List_element.  See build_spec_list for more details.  */
تعداد Range_element_type
  {
    RE_عام_CHAR,
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
بنية List_element
  {
    تعداد Range_element_type type;
    بنية List_element *اگلا;
    اتحاد
      {
        بنانشانی حرف عام_char;
        بنية			/* unnamed */
          {
            بنانشانی حرف پہلا_char;
            بنانشانی حرف آخری_char;
          }
        range;
        تعداد Char_class char_class;
        بنانشانی حرف equiv_کوڈ;
        بنية			/* unnamed */
          {
            بنانشانی حرف the_repeated_char;
            گنتی repeat_گنتی;
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
بنية Spec_list
  {
    /* Points to the head of the list of range elements.
       The first struct is a dummy; its members are never used.  */
    بنية List_element *سر;

    /* When appending, points to the last element.  When traversing via
       get_next(), points to the element to process next.  Setting
       Spec_list.state to the value BEGIN_STATE before calling get_next
       signals get_next to initialize tail to point to head->next.  */
    بنية List_element *پوچھ;

    /* Used to save state between calls to get_next.  */
    گنتی state;

    /* Length, in the sense that length ('a-z[:digit:]123abc')
       is 42 ( = 26 + 10 + 6).  */
    گنتی لمائی;

    /* The number of [c*] and [c*0] constructs that appear in this spec.  */
    size_t n_indefinite_repeats;

    /* If n_indefinite_repeats is nonzero, this points to the List_element
       corresponding to the last [c*] or [c*0] construct encountered in
       this spec.  Otherwise it is undefined.  */
    بنية List_element *indefinite_repeat_element;

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
بنية E_string
{
  حرف *s;
  bool *escaped;
  size_t len;
};

/* Return nonzero if the Ith character of escaped string ES matches C
   and is not escaped itself.  */
ساكن مضمن bool
es_میل (بنية E_string ثابت *es, size_t i, حرف c)
{
  أرجع es->s[i] == c && !es->escaped[i];
}

/* When true, each sequence in the input of a repeated character
   (call it c) is replaced (in the output) by a single occurrence of c
   for every c in the squeeze set.  */
ساكن bool squeeze_repeats = false;

/* When true, removes characters in the delete set from input.  */
ساكن bool delete = false;

/* Use the complement of set1 in place of set1.  */
ساكن bool complement = false;

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
ساكن bool truncate_set1 = false;

/* An alias for (!delete && non_option_args == 2).
   It is set in main and used there and in validate().  */
ساكن bool translating;

ساكن حرف io_buf[BUFSIZ];

ساكن حرف ثابت *ثابت char_class_name[] =
{
  "alnum", "alpha", "blank", "cntrl", "digit", "graph",
  "lower", "print", "punct", "space", "upper", "xdigit"
};

/* Array of boolean values.  A character 'c' is a member of the
   squeeze set if and only if in_squeeze_set[c] is true.  The squeeze
   set is defined by the last (possibly, the only) string argument
   on the command line when the squeeze option is given.  */
ساكن bool وچ_squeeze_سیٹ[N_CHARS];

/* Array of boolean values.  A character 'c' is a member of the
   delete set if and only if in_delete_set[c] is true.  The delete
   set is defined by the first (or only) string argument on the
   command line when the delete option is given.  */
ساكن bool وچ_delete_سیٹ[N_CHARS];

/* Array of character values defining the translation (if any) that
   tr is to perform.  Translation is performed only when there are
   two specification strings and the delete switch is not given.  */
ساكن حرف xlate[N_CHARS];

ساكن بنية option ثابت long_options[] =
{
  {"complement", no_دلیل, عدم, 'c'},
  {"delete", no_دلیل, عدم, 'd'},
  {"squeeze-repeats", no_دلیل, عدم, 's'},
  {"truncate-set1", no_دلیل, عدم, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s [OPTION]... STRING1 [STRING2]\n\
"),
              پروگرام_name);
      fputs (_("\
Translate, squeeze, and/or delete characters from standard داخلہ,\n\
writing منزل standard output.  STRING1 and STRING2 specify arrays دا\n\
characters ARRAY1 and ARRAY2 that control the action.\n\
\n\
"), stdout);
      oputs (_("\
  -c, -C, --complement\n\
         use the complement دا ARRAY1\n\
"));
      oputs (_("\
  -d, --delete\n\
         delete characters وچ ARRAY1, افعل not translate\n\
"));
      oputs (_("\
  -s, --squeeze-repeats\n\
         replace each sequence دا a repeated character\n\
         that is listed وچ the آخری specified ARRAY,\n\
         with a single occurrence دا that character\n\
"));
      oputs (_("\
  -t, --truncate-set1\n\
         پہلا truncate ARRAY1 منزل لمائی دا ARRAY2\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ARRAYs are specified as لڑیاں دا characters.  Most represent themselves.\n\
Interpreted sequences are:\n\
\n\
  \\NNN            character with octal قدر NNN (1 منزل 3 octal digits)\n\
  \\\\              backslash\n\
  \\a              audible BEL\n\
  \\b              backspace\n\
  \\f              form feed\n\
  \\n              new سطر\n\
  \\r              أرجع\n\
  \\t              horizontal tab\n\
"), stdout);
     fputs (_("\
  \\v              vertical tab\n\
  CHAR1-CHAR2     all characters from CHAR1 منزل CHAR2 وچ ascending ترتیب\n\
  [CHAR*]         وچ ARRAY2, copies دا CHAR until لمائی دا ARRAY1\n\
  [CHAR*REPEAT]   REPEAT copies دا CHAR, REPEAT octal إذا starting with 0\n\
  [:alnum:]       all letters and digits\n\
  [:alpha:]       all letters\n\
  [:blank:]       all horizontal whitespace\n\
  [:cntrl:]       all control characters\n\
  [:digit:]       all digits\n\
"), stdout);
     fputs (_("\
  [:گراف:]       all printable characters, not including space\n\
  [:ہیٹھلا:]       all ہیٹھلا حالة letters\n\
  [:print:]       all printable characters, including space\n\
  [:punct:]       all punctuation characters\n\
  [:space:]       all horizontal or vertical whitespace\n\
  [:اتلا:]       all اتلا حالة letters\n\
  [:xdigit:]      all hexadecimal digits\n\
  [=CHAR=]        all characters which are equivalent منزل CHAR\n\
"), stdout);
     fputs (_("\
\n\
Translation occurs إذا -d is not given and both STRING1 and STRING2 appear.\n\
-t is only significant when translating.  ARRAY2 is extended منزل لمائی دا\n\
ARRAY1 by repeating its آخری character as necessary.  Excess characters\n\
دا ARRAY2 are ignored.  Character classes expand وچ unspecified ترتیب;\n\
طالما translating, '[:lower:]' and '[:upper:]' may be ورتیا وچ جوڑے منزل\n\
specify حالة conversion.  Squeezing occurs after translation or deletion.\n\
Arguments like '[...]' should be quoted, منزل avoid potential shell globbing.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Return nonzero if the character C is a member of the
   equivalence class containing the character EQUIV_CLASS.  */

ساكن مضمن bool
is_equiv_class_member (بنانشانی حرف equiv_class, بنانشانی حرف c)
{
  أرجع (equiv_class == c);
}

/* Return true if the character C is a member of the
   character class CHAR_CLASS.  */

ATTRIBUTE_خالص
ساكن bool
is_char_class_member (تعداد Char_class char_class, بنانشانی حرف c)
{
  صحيح نتیجہ;

  بدل (char_class)
    {
    حالة CC_ALNUM:
      نتیجہ = حرف_یا_ہندسہ (c);
      اكسر;
    حالة CC_ALPHA:
      نتیجہ = حرف_اے (c);
      اكسر;
    حالة CC_BLANK:
      نتیجہ = isblank (c);
      اكسر;
    حالة CC_CNTRL:
      نتیجہ = iscntrl (c);
      اكسر;
    حالة CC_DIGIT:
      نتیجہ = c_isdigit (c);
      اكسر;
    حالة CC_گراف:
      نتیجہ = isgraph (c);
      اكسر;
    حالة CC_ہیٹھلا:
      نتیجہ = islower (c);
      اكسر;
    حالة CC_PRINT:
      نتیجہ = isprint (c);
      اكسر;
    حالة CC_PUNCT:
      نتیجہ = ispunct (c);
      اكسر;
    حالة CC_SPACE:
      نتیجہ = خلا_اے (c);
      اكسر;
    حالة CC_اتلا:
      نتیجہ = isupper (c);
      اكسر;
    حالة CC_XDIGIT:
      نتیجہ = c_isxdigit (c);
      اكسر;
    حالة CC_NO_CLASS: افتراضي:
      affirm (false);
    }

  أرجع !! نتیجہ;
}

ساكن فراغ
es_free (بنية E_string *es)
{
  حرر (es->s);
  حرر (es->escaped);
}

/* Perform the first pass over each range-spec argument S, converting all
   \c and \ddd escapes to their one-byte representations.  If an invalid
   quote sequence is found print an error message and return false;
   Otherwise set *ES to the resulting string and return true.
   The resulting array of characters may contain zero-bytes;
   however, on input, S is assumed to be null-terminated, and hence
   cannot contain actual (non-escaped) zero bytes.  */

ساكن bool
unquote (حرف ثابت *s, بنية E_string *es)
{
  size_t len = لڑی_لمبائی (s);

  es->s = xmalloc (len);
  es->escaped = xcalloc (len, حجم es->escaped[0]);

  بنانشانی صحيح j = 0;
  لكل (بنانشانی صحيح i = 0; s[i]; i++)
    {
      بنانشانی حرف c;
      صحيح oct_digit;

      بدل (s[i])
        {
        حالة '\\':
          es->escaped[j] = true;
          بدل (s[i + 1])
            {
            حالة '\\':
              c = '\\';
              اكسر;
            حالة 'a':
              c = '\a';
              اكسر;
            حالة 'b':
              c = '\b';
              اكسر;
            حالة 'f':
              c = '\f';
              اكسر;
            حالة 'n':
              c = '\n';
              اكسر;
            حالة 'r':
              c = '\r';
              اكسر;
            حالة 't':
              c = '\t';
              اكسر;
            حالة 'v':
              c = '\v';
              اكسر;
            حالة '0':
            حالة '1':
            حالة '2':
            حالة '3':
            حالة '4':
            حالة '5':
            حالة '6':
            حالة '7':
              c = s[i + 1] - '0';
              oct_digit = s[i + 2] - '0';
              إذا (0 <= oct_digit && oct_digit <= 7)
                {
                  c = 8 * c + oct_digit;
                  ++i;
                  oct_digit = s[i + 2] - '0';
                  إذا (0 <= oct_digit && oct_digit <= 7)
                    {
                      إذا (8 * c + oct_digit < N_CHARS)
                        {
                          c = 8 * c + oct_digit;
                          ++i;
                        }
                      وإلا
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
              اكسر;
            حالة '\0':
              error (0, 0, _("warning: an unescaped backslash "
                             "at end of string is not portable"));
              /* POSIX is not clear about this.  */
              es->escaped[j] = false;
              i--;
              c = '\\';
              اكسر;
            افتراضي:
              c = s[i + 1];
              اكسر;
            }
          ++i;
          es->s[j++] = c;
          اكسر;
        افتراضي:
          es->s[j++] = s[i];
          اكسر;
        }
    }
  es->len = j;
  أرجع true;
}

/* If CLASS_STR is a valid character class string, return its index
   in the global char_class_name array.  Otherwise, return CC_NO_CLASS.  */

ATTRIBUTE_خالص
ساكن تعداد Char_class
look_اتے_char_class (حرف ثابت *class_str, size_t len)
{
  لكل (تعداد Char_class i = 0; i < countof (char_class_name); i++)
    إذا (STREQ_LEN (class_str, char_class_name[i], len)
        && لڑی_لمبائی (char_class_name[i]) == len)
      أرجع i;
  أرجع CC_NO_CLASS;
}

/* Return a newly allocated string with a printable version of C.
   This function is used solely for formatting error messages.  */

ساكن حرف *
make_printable_char (بنانشانی حرف c)
{
  حرف *buf = xmalloc (5);

  إذا (isprint (c))
    {
      buf[0] = c;
      buf[1] = '\0';
    }
  وإلا
    {
      sprintf (buf, "\\%03o", c);
    }
  أرجع buf;
}

/* Return a newly allocated copy of S which is suitable for printing.
   LEN is the number of characters in S.  Most non-printing
   (isprint) characters are represented by a backslash followed by
   3 octal digits.  However, the characters represented by \c escapes
   where c is one of [abfnrtv] are represented by their 2-character \c
   sequences.  This function is used solely for printing error messages.  */

ساكن حرف *
make_printable_str (حرف ثابت *s, size_t len)
{
  /* Worst case is that every character expands to a backslash
     followed by a 3-character octal escape sequence.  */
  حرف *printable_buf = xnmalloc (len + 1, 4);
  حرف *p = printable_buf;

  لكل (size_t i = 0; i < len; i++)
    {
      حرف buf[5];
      حرف ثابت *tmp = عدم;
      بنانشانی حرف c = s[i];

      بدل (c)
        {
        حالة '\\':
          tmp = "\\";
          اكسر;
        حالة '\a':
          tmp = "\\a";
          اكسر;
        حالة '\b':
          tmp = "\\b";
          اكسر;
        حالة '\f':
          tmp = "\\f";
          اكسر;
        حالة '\n':
          tmp = "\\n";
          اكسر;
        حالة '\r':
          tmp = "\\r";
          اكسر;
        حالة '\t':
          tmp = "\\t";
          اكسر;
        حالة '\v':
          tmp = "\\v";
          اكسر;
        افتراضي:
          إذا (isprint (c))
            {
              buf[0] = c;
              buf[1] = '\0';
            }
          وإلا
            sprintf (buf, "\\%03o", c);
          tmp = buf;
          اكسر;
        }
      p = stpcpy (p, tmp);
    }
  أرجع printable_buf;
}

/* Append a newly allocated structure representing a
   character C to the specification list LIST.  */

ساكن فراغ
append_عام_char (بنية Spec_list *list, بنانشانی حرف c)
{
  بنية List_element *new = xmalloc (حجم *new);
  new->اگلا = عدم;
  new->type = RE_عام_CHAR;
  new->u.عام_char = c;
  list->پوچھ->اگلا = new;
  list->پوچھ = new;
}

/* Append a newly allocated structure representing the range
   of characters from FIRST to LAST to the specification list LIST.
   Return false if LAST precedes FIRST in the collating sequence,
   true otherwise.  This means that '[c-c]' is acceptable.  */

ساكن bool
append_range (بنية Spec_list *list, بنانشانی حرف پہلا, بنانشانی حرف آخری)
{
  إذا (آخری < پہلا)
    {
      حرف *tmp1 = make_printable_char (پہلا);
      حرف *tmp2 = make_printable_char (آخری);

      error (0, 0,
       _("range-endpoints of '%s-%s' are in reverse collating sequence order"),
             tmp1, tmp2);
      حرر (tmp1);
      حرر (tmp2);
      أرجع false;
    }
  بنية List_element *new = xmalloc (حجم *new);
  new->اگلا = عدم;
  new->type = RE_RANGE;
  new->u.range.پہلا_char = پہلا;
  new->u.range.آخری_char = آخری;
  list->پوچھ->اگلا = new;
  list->پوچھ = new;
  أرجع true;
}

/* If CHAR_CLASS_STR is a valid character class string, append a
   newly allocated structure representing that character class to the end
   of the specification list LIST and return true.  If CHAR_CLASS_STR is not
   a valid string return false.  */

ساكن bool
append_char_class (بنية Spec_list *list,
                   حرف ثابت *char_class_str, size_t len)
{
  تعداد Char_class char_class = look_اتے_char_class (char_class_str, len);
  إذا (char_class == CC_NO_CLASS)
    أرجع false;
  بنية List_element *new = xmalloc (حجم *new);
  new->اگلا = عدم;
  new->type = RE_CHAR_CLASS;
  new->u.char_class = char_class;
  list->پوچھ->اگلا = new;
  list->پوچھ = new;
  أرجع true;
}

/* Append a newly allocated structure representing a [c*n]
   repeated character construct to the specification list LIST.
   THE_CHAR is the single character to be repeated, and REPEAT_COUNT
   is a non-negative repeat count.  */

ساكن فراغ
append_repeated_char (بنية Spec_list *list, بنانشانی حرف the_char,
                      گنتی repeat_گنتی)
{
  بنية List_element *new = xmalloc (حجم *new);
  new->اگلا = عدم;
  new->type = RE_REPEATED_CHAR;
  new->u.repeated_char.the_repeated_char = the_char;
  new->u.repeated_char.repeat_گنتی = repeat_گنتی;
  list->پوچھ->اگلا = new;
  list->پوچھ = new;
}

/* Given a string, EQUIV_CLASS_STR, from a [=str=] context and
   the length of that string, LEN, if LEN is exactly one, append
   a newly allocated structure representing the specified
   equivalence class to the specification list, LIST and return true.
   If LEN is not 1, return false.  */

ساكن bool
append_equiv_class (بنية Spec_list *list,
                    حرف ثابت *equiv_class_str, size_t len)
{
  إذا (len != 1)
    أرجع false;

  بنية List_element *new = xmalloc (حجم *new);
  new->اگلا = عدم;
  new->type = RE_EQUIV_CLASS;
  new->u.equiv_کوڈ = *equiv_class_str;
  list->پوچھ->اگلا = new;
  list->پوچھ = new;
  أرجع true;
}

/* Search forward starting at START_IDX for the 2-char sequence
   (PRE_BRACKET_CHAR,']') in the string P of length P_LEN.  If such
   a sequence is found, set *RESULT_IDX to the index of the first
   character and return true.  Otherwise return false.  P may contain
   zero bytes.  */

ساكن bool
لبھو_closing_delim (ثابت بنية E_string *es, size_t شروع_idx,
                    حرف pre_bracket_char, size_t *نتیجہ_idx)
{
  لكل (size_t i = شروع_idx; i < es->len - 1; i++)
    إذا (es->s[i] == pre_bracket_char && es->s[i + 1] == ']'
        && !es->escaped[i] && !es->escaped[i + 1])
      {
        *نتیجہ_idx = i;
        أرجع true;
      }
  أرجع false;
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

ساكن صحيح
لبھو_bracketed_repeat (ثابت بنية E_string *es, size_t شروع_idx,
                       بنانشانی حرف *char_منزل_repeat, گنتی *repeat_گنتی,
                       size_t *closing_bracket_idx)
{
  affirm (شروع_idx + 1 < es->len);
  إذا (!es_میل (es, شروع_idx + 1, '*'))
    أرجع -1;

  لكل (size_t i = شروع_idx + 2; i < es->len && !es->escaped[i]; i++)
    {
      إذا (es->s[i] == ']')
        {
          size_t digit_str_len = i - شروع_idx - 2;

          *char_منزل_repeat = es->s[شروع_idx];
          إذا (digit_str_len == 0)
            {
              /* We've matched [c*] -- no explicit repeat count.  */
              *repeat_گنتی = 0;
            }
          وإلا
            {
              /* Here, we have found [c*s] where s should be a string
                 of octal (if it starts with '0') or decimal digits.  */
              حرف ثابت *digit_str = &es->s[شروع_idx + 2];
              حرف *d_سرا;
              إذا ((xstrtoumax (digit_str, &d_سرا, *digit_str == '0' ? 8 : 10,
                               repeat_گنتی, عدم)
                   != LONGINT_OK)
                  || REPEAT_گنتی_ودھ_توں_ودھ < *repeat_گنتی
                  || digit_str + digit_str_len != d_سرا)
                {
                  حرف *tmp = make_printable_str (digit_str, digit_str_len);
                  error (0, 0,
                         _("invalid repeat count %s in [c*n] construct"),
                         quote (tmp));
                  حرر (tmp);
                  أرجع -2;
                }
            }
          *closing_bracket_idx = i;
          أرجع 0;
        }
    }
  أرجع -1;			/* No bracket found.  */
}

/* Return true if the string at ES->s[IDX] matches the regular
   expression '\*[0-9]*]', false otherwise.  The string does not
   match if any of its characters are escaped.  */

ATTRIBUTE_خالص
ساكن bool
تارا_digits_closebracket (ثابت بنية E_string *es, size_t idx)
{
  إذا (!es_میل (es, idx, '*'))
    أرجع false;

  لكل (size_t i = idx + 1; i < es->len; i++)
    إذا (!c_isdigit (es->s[i]) || es->escaped[i])
      أرجع es_میل (es, i, ']');
  أرجع false;
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

ساكن bool
build_spec_list (ثابت بنية E_string *es, بنية Spec_list *نتیجہ)
{
  حرف ثابت *p = es->s;

  /* The main for-loop below recognizes the 4 multi-character constructs.
     A character that matches (in its context) none of the multi-character
     constructs is classified as 'normal'.  Since all multi-character
     constructs have at least 3 characters, any strings of length 2 or
     less are composed solely of normal characters.  Hence, the index of
     the outer for-loop runs only as far as LEN-2.  */
  size_t i;
  لكل (i = 0; i + 2 < es->len; /*خالی_تھاواں*/)
    {
      إذا (es_میل (es, i, '['))
        {
          bool matched_multi_char_construct;
          size_t closing_bracket_idx;
          بنانشانی حرف char_منزل_repeat;
          گنتی repeat_گنتی;
          صحيح err;

          matched_multi_char_construct = true;
          إذا (es_میل (es, i + 1, ':') || es_میل (es, i + 1, '='))
            {
              size_t closing_delim_idx;

              إذا (لبھو_closing_delim (es, i + 2, p[i + 1], &closing_delim_idx))
                {
                  size_t opnd_str_len = closing_delim_idx - 1 - (i + 2) + 1;
                  حرف ثابت *opnd_str = p + i + 2;

                  إذا (opnd_str_len == 0)
                    {
                      إذا (p[i + 1] == ':')
                        error (0, 0, _("missing character class name '[::]'"));
                      وإلا
                        error (0, 0,
                               _("missing equivalence class character '[==]'"));
                      أرجع false;
                    }

                  إذا (p[i + 1] == ':')
                    {
                      /* FIXME: big comment.  */
                      إذا (!append_char_class (نتیجہ, opnd_str, opnd_str_len))
                        {
                          إذا (تارا_digits_closebracket (es, i + 2))
                            اذهب try_bracketed_repeat;
                          وإلا
                            {
                              حرف *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0, _("invalid character class %s"),
                                     quote (tmp));
                              حرر (tmp);
                              أرجع false;
                            }
                        }
                    }
                  وإلا
                    {
                      /* FIXME: big comment.  */
                      إذا (!append_equiv_class (نتیجہ, opnd_str, opnd_str_len))
                        {
                          إذا (تارا_digits_closebracket (es, i + 2))
                            اذهب try_bracketed_repeat;
                          وإلا
                            {
                              حرف *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0,
               _("%s: equivalence class operand must be a single character"),
                                     tmp);
                              حرر (tmp);
                              أرجع false;
                            }
                        }
                    }

                  i = closing_delim_idx + 2;
                  استمر;
                }
              /* Else fall through.  This could be [:*] or [=*].  */
            }

        try_bracketed_repeat:

          /* Determine whether this is a bracketed repeat range
             matching the RE \[.\*(dec_or_oct_number)?].  */
          err = لبھو_bracketed_repeat (es, i + 1, &char_منزل_repeat,
                                       &repeat_گنتی,
                                       &closing_bracket_idx);
          إذا (err == 0)
            {
              append_repeated_char (نتیجہ, char_منزل_repeat, repeat_گنتی);
              i = closing_bracket_idx + 1;
            }
          وإلا إذا (err == -1)
            {
              matched_multi_char_construct = false;
            }
          وإلا
            {
              /* Found a string that looked like [c*n] but the
                 numeric part was invalid.  */
              أرجع false;
            }

          إذا (matched_multi_char_construct)
            استمر;

          /* We reach this point if P does not match [:str:], [=c=],
             [c*n], or [c*].  Now, see if P looks like a range '[-c'
             (from '[' to 'c').  */
        }

      /* Look ahead one char for ranges like a-z.  */
      إذا (es_میل (es, i + 1, '-'))
        {
          إذا (!append_range (نتیجہ, p[i], p[i + 2]))
            أرجع false;
          i += 3;
        }
      وإلا
        {
          append_عام_char (نتیجہ, p[i]);
          ++i;
        }
    }

  /* Now handle the (2 or fewer) remaining characters p[i]..p[es->len - 1].  */
  لكل (; i < es->len; i++)
    append_عام_char (نتیجہ, p[i]);

  أرجع true;
}

/* Advance past the current construct.
   S->tail must be non-null.  */
ساكن فراغ
skip_construct (بنية Spec_list *s)
{
  s->پوچھ = s->پوچھ->اگلا;
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

ساكن صحيح
get_اگلا (بنية Spec_list *s, تعداد اتلا_ہیٹھلا_class *class)
{
  بنية List_element *p;
  صحيح return_val;
  صحيح i;

  إذا (class)
    *class = UL_NONE;

  إذا (s->state == BEGIN_STATE)
    {
      s->پوچھ = s->سر->اگلا;
      s->state = NEW_ELEMENT;
    }

  p = s->پوچھ;
  إذا (p == عدم)
    أرجع -1;

  بدل (p->type)
    {
    حالة RE_عام_CHAR:
      return_val = p->u.عام_char;
      s->state = NEW_ELEMENT;
      s->پوچھ = p->اگلا;
      اكسر;

    حالة RE_RANGE:
      إذا (s->state == NEW_ELEMENT)
        s->state = p->u.range.پہلا_char;
      وإلا
        ++(s->state);
      return_val = s->state;
      إذا (s->state == p->u.range.آخری_char)
        {
          s->پوچھ = p->اگلا;
          s->state = NEW_ELEMENT;
        }
      اكسر;

    حالة RE_CHAR_CLASS:
      إذا (class)
        {
          بدل (+p->u.char_class)
            {
            حالة CC_ہیٹھلا:
              *class = UL_ہیٹھلا;
              اكسر;
            حالة CC_اتلا:
              *class = UL_اتلا;
              اكسر;
            }
        }

      إذا (s->state == NEW_ELEMENT)
        {
          لكل (i = 0; i < N_CHARS; i++)
            إذا (is_char_class_member (p->u.char_class, i))
              اكسر;
          affirm (i < N_CHARS);
          s->state = i;
        }
      assure (is_char_class_member (p->u.char_class, s->state));
      return_val = s->state;
      لكل (i = s->state + 1; i < N_CHARS; i++)
        إذا (is_char_class_member (p->u.char_class, i))
          اكسر;
      إذا (i < N_CHARS)
        s->state = i;
      وإلا
        {
          s->پوچھ = p->اگلا;
          s->state = NEW_ELEMENT;
        }
      اكسر;

    حالة RE_EQUIV_CLASS:
      /* FIXME: this assumes that each character is alone in its own
         equivalence class (which appears to be correct for my
         LC_COLLATE.  But I don't know of any function that allows
         one to determine a character's equivalence class.  */

      return_val = p->u.equiv_کوڈ;
      s->state = NEW_ELEMENT;
      s->پوچھ = p->اگلا;
      اكسر;

    حالة RE_REPEATED_CHAR:
      /* Here, a repeat count of n == 0 means don't repeat at all.  */
      إذا (p->u.repeated_char.repeat_گنتی == 0)
        {
          s->پوچھ = p->اگلا;
          s->state = NEW_ELEMENT;
          return_val = get_اگلا (s, class);
        }
      وإلا
        {
          إذا (s->state == NEW_ELEMENT)
            {
              s->state = 0;
            }
          ++(s->state);
          return_val = p->u.repeated_char.the_repeated_char;
          إذا (s->state == p->u.repeated_char.repeat_گنتی)
            {
              s->پوچھ = p->اگلا;
              s->state = NEW_ELEMENT;
            }
        }
      اكسر;

    افتراضي:
      unreachable ();
    }

  أرجع return_val;
}

/* This is a minor kludge.  This function is called from
   get_spec_stats to determine the cardinality of a set derived
   from a complemented string.  It's a kludge in that some of the
   same operations are (duplicated) performed in set_initialize.  */

ساكن صحيح
card_دا_complement (بنية Spec_list *s)
{
  صحيح c;
  صحيح cardinality = N_CHARS;
  bool وچ_سیٹ[N_CHARS] = {0};

  s->state = BEGIN_STATE;
  طالما ((c = get_اگلا (s, عدم)) != -1)
    {
      cardinality -= (!وچ_سیٹ[c]);
      وچ_سیٹ[c] = true;
    }
  أرجع cardinality;
}

/* Discard the lengths associated with a case conversion,
   as using the actual number of upper or lower case characters
   is problematic when they don't match in some locales.
   Also ensure the case conversion classes in string2 are
   aligned correctly with those in string1.
   Note POSIX says the behavior of 'tr "[:upper:]" "[:upper:]"'
   is undefined.  Therefore we allow it (unlike Solaris)
   and treat it as a no-op.  */

ساكن فراغ
validate_case_classes (بنية Spec_list *s1, بنية Spec_list *s2)
{
  size_t n_اتلا = 0;
  size_t n_ہیٹھلا = 0;
  صحيح c1 = 0;
  صحيح c2 = 0;
  MAYBE_بن_ورتیا گنتی old_s1_len = s1->لمائی, old_s2_len = s2->لمائی;
  بنية List_element *s1_پوچھ = s1->پوچھ;
  بنية List_element *s2_پوچھ = s2->پوچھ;
  bool s1_new_element = true;
  bool s2_new_element = true;

  إذا (complement || !s2->has_char_class)
    أرجع;

  لكل (صحيح i = 0; i < N_CHARS; i++)
    {
      إذا (isupper (i))
        n_اتلا++;
      إذا (islower (i))
        n_ہیٹھلا++;
    }

  s1->state = BEGIN_STATE;
  s2->state = BEGIN_STATE;

  طالما (c1 != -1 && c2 != -1)
    {
      تعداد اتلا_ہیٹھلا_class class_s1, class_s2;

      c1 = get_اگلا (s1, &class_s1);
      c2 = get_اگلا (s2, &class_s2);

      /* If c2 transitions to a new case class, then
         c1 must also transition at the same time.  */
      إذا (s2_new_element && class_s2 != UL_NONE
          && !(s1_new_element && class_s1 != UL_NONE))
        error (EXIT_FAILURE, 0,
               _("misaligned [:upper:] and/or [:lower:] construct"));

      /* If case converting, quickly skip over the elements.  */
      إذا (class_s2 != UL_NONE)
        {
          skip_construct (s1);
          skip_construct (s2);
          /* Discount insignificant/problematic lengths.  */
          s1->لمائی -= (class_s1 == UL_اتلا ? n_اتلا : n_ہیٹھلا) - 1;
          s2->لمائی -= (class_s2 == UL_اتلا ? n_اتلا : n_ہیٹھلا) - 1;
        }

      s1_new_element = s1->state == NEW_ELEMENT; /* Next element is new.  */
      s2_new_element = s2->state == NEW_ELEMENT; /* Next element is new.  */
    }

  affirm (old_s1_len >= s1->لمائی && old_s2_len >= s2->لمائی);

  s1->پوچھ = s1_پوچھ;
  s2->پوچھ = s2_پوچھ;
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

ساكن فراغ
get_spec_stats (بنية Spec_list *s)
{
  گنتی لمائی = 0;

  s->n_indefinite_repeats = 0;
  s->has_equiv_class = false;
  s->has_restricted_char_class = false;
  s->has_char_class = false;
  لكل (بنية List_element *p = s->سر->اگلا; p; p = p->اگلا)
    {
      گنتی len = 0;

      بدل (p->type)
        {
        حالة RE_عام_CHAR:
          len = 1;
          اكسر;

        حالة RE_RANGE:
          affirm (p->u.range.آخری_char >= p->u.range.پہلا_char);
          len = p->u.range.آخری_char - p->u.range.پہلا_char + 1;
          اكسر;

        حالة RE_CHAR_CLASS:
          s->has_char_class = true;
          لكل (صحيح i = 0; i < N_CHARS; i++)
            إذا (is_char_class_member (p->u.char_class, i))
              ++len;
          بدل (+p->u.char_class)
            {
            حالة CC_اتلا:
            حالة CC_ہیٹھلا:
              اكسر;
            افتراضي:
              s->has_restricted_char_class = true;
              اكسر;
            }
          اكسر;

        حالة RE_EQUIV_CLASS:
          لكل (صحيح i = 0; i < N_CHARS; i++)
            إذا (is_equiv_class_member (p->u.equiv_کوڈ, i))
              ++len;
          s->has_equiv_class = true;
          اكسر;

        حالة RE_REPEATED_CHAR:
          إذا (p->u.repeated_char.repeat_گنتی > 0)
            len = p->u.repeated_char.repeat_گنتی;
          وإلا
            {
              s->indefinite_repeat_element = p;
              ++(s->n_indefinite_repeats);
            }
          اكسر;

        افتراضي:
          unreachable ();
        }

      /* Check for arithmetic overflow in computing length.  Also, reject
         any length greater than the maximum repeat count, in case the
         length is later used to compute the repeat count for an
         indefinite element.  */
      إذا (ckd_جوڑو (&لمائی, لمائی, len) || REPEAT_گنتی_ودھ_توں_ودھ < لمائی)
        error (EXIT_FAILURE, 0, _("too many characters in set"));
    }

  s->لمائی = لمائی;
}

ساكن فراغ
get_s1_spec_stats (بنية Spec_list *s1)
{
  get_spec_stats (s1);
  إذا (complement)
    s1->لمائی = card_دا_complement (s1);
}

ساكن فراغ
get_s2_spec_stats (بنية Spec_list *s2, گنتی len_s1)
{
  get_spec_stats (s2);
  إذا (len_s1 >= s2->لمائی && s2->n_indefinite_repeats == 1)
    {
      s2->indefinite_repeat_element->u.repeated_char.repeat_گنتی =
        len_s1 - s2->لمائی;
      s2->لمائی = len_s1;
    }
}

ساكن فراغ
spec_init (بنية Spec_list *spec_list)
{
  بنية List_element *new = xmalloc (حجم *new);
  spec_list->سر = spec_list->پوچھ = new;
  spec_list->سر->اگلا = عدم;
}

/* This function makes two passes over the argument string S.  The first
   one converts all \c and \ddd escapes to their one-byte representations.
   The second constructs a linked specification list, SPEC_LIST, of the
   characters and constructs that comprise the argument string.  If either
   of these passes detects an error, this function returns false.  */

ساكن bool
parse_str (حرف ثابت *s, بنية Spec_list *spec_list)
{
  بنية E_string es;
  bool ok = unquote (s, &es) && build_spec_list (&es, spec_list);
  es_free (&es);
  أرجع ok;
}

/* Given two specification lists, S1 and S2, and assuming that
   S1->length > S2->length, append a single [c*n] element to S2 where c
   is the last character in the expansion of S2 and n is the difference
   between the two lengths.
   Upon successful completion, S2->length is set to S1->length.  The only
   way this function can fail to make S2 as long as S1 is when S2 has
   zero-length, since in that case, there is no last character to repeat.
   So S2->length is required to be at least 1.  */

ساكن فراغ
string2_extend (ثابت بنية Spec_list *s1, بنية Spec_list *s2)
{
  بنية List_element *p;
  بنانشانی حرف char_منزل_repeat;

  affirm (translating);
  affirm (s1->لمائی > s2->لمائی);
  affirm (s2->لمائی > 0);

  p = s2->پوچھ;
  بدل (p->type)
    {
    حالة RE_عام_CHAR:
      char_منزل_repeat = p->u.عام_char;
      اكسر;
    حالة RE_RANGE:
      char_منزل_repeat = p->u.range.آخری_char;
      اكسر;
    حالة RE_CHAR_CLASS:
      /* Note BSD allows extending of classes in string2.  For example:
           tr '[:upper:]0-9' '[:lower:]'
         That's not portable however, contradicts POSIX and is dependent
         on your collating sequence.  */
      error (EXIT_FAILURE, 0,
             _("when translating with string1 longer than string2,\n"
               "the latter string must not end with a character class"));

    حالة RE_REPEATED_CHAR:
      char_منزل_repeat = p->u.repeated_char.the_repeated_char;
      اكسر;

    حالة RE_EQUIV_CLASS: افتراضي:
      /* This shouldn't happen, because validate exits with an error
         if it finds an equiv class in string2 when translating.  */
      affirm (false);
    }

  append_repeated_char (s2, char_منزل_repeat, s1->لمائی - s2->لمائی);
  s2->لمائی = s1->لمائی;
}

/* Return true if S is a non-empty list in which exactly one
   character (but potentially, many instances of it) appears.
   E.g., [X*] or xxxxxxxx.  */

ساكن bool
homogeneous_spec_list (بنية Spec_list *s)
{
  صحيح b, c;

  s->state = BEGIN_STATE;

  إذا ((b = get_اگلا (s, عدم)) == -1)
    أرجع false;

  طالما ((c = get_اگلا (s, عدم)) != -1)
    إذا (c != b)
      أرجع false;

  أرجع true;
}

/* Die with an error message if S1 and S2 describe strings that
   are not valid with the given command line switches.
   A side effect of this function is that if a valid [c*] or
   [c*0] construct appears in string2, it is converted to [c*n]
   with a value for n that makes s2->length == s1->length.  By
   the same token, if the --truncate-set1 option is not
   given, S2 may be extended.  */

ساكن فراغ
validate (بنية Spec_list *s1, بنية Spec_list *s2)
{
  get_s1_spec_stats (s1);
  إذا (s1->n_indefinite_repeats > 0)
    error (EXIT_FAILURE, 0,
           _("the [c*] repeat construct may not appear in string1"));

  إذا (s2)
    {
      get_s2_spec_stats (s2, s1->لمائی);

      إذا (s2->n_indefinite_repeats > 1)
        error (EXIT_FAILURE, 0,
               _("only one [c*] repeat construct may appear in string2"));

      إذا (translating)
        {
          إذا (s2->has_equiv_class)
            error (EXIT_FAILURE, 0,
                   _("[=c=] expressions may not appear in string2"
                     " when translating"));

          إذا (s2->has_restricted_char_class)
            error (EXIT_FAILURE, 0,
                   _("when translating, the only character classes"
                     " that may appear in\n"
                     "string2 are 'upper' and 'lower'"));

          validate_case_classes (s1, s2);

          إذا (s1->لمائی > s2->لمائی)
            {
              إذا (!truncate_set1)
                {
                  /* string2 must be non-empty unless --truncate-set1 is
                     given or string1 is empty.  */

                  إذا (s2->لمائی == 0)
                    error (EXIT_FAILURE, 0,
                           _("when not truncating set1,"
                             " string2 must be non-empty"));
                  string2_extend (s1, s2);
                }
            }

          إذا (complement && s1->has_char_class
              && ! (s2->لمائی == s1->لمائی && homogeneous_spec_list (s2)))
            error (EXIT_FAILURE, 0,
                   _("when translating with complemented character classes,\n"
                     "string2 must map all characters in the domain to one"));
        }
      وإلا
        /* Not translating.  */
        {
          إذا (s2->n_indefinite_repeats > 0)
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

ساكن فراغ
squeeze_filter (حرف *buf, size_t ناپ, size_t (*reader) (حرف *, size_t))
{
  /* A value distinct from any character that may have been stored in a
     buffer as the result of a block-read in the function squeeze_filter.  */
  ثابت صحيح NOT_A_CHAR = ودھ_توں_ودھ_عدد;

  صحيح char_منزل_squeeze = NOT_A_CHAR;
  size_t i = 0;
  size_t nr = 0;

  طالما (true)
    {
      إذا (i >= nr)
        {
          nr = reader (buf, ناپ);
          إذا (nr == 0)
            اكسر;
          i = 0;
        }

      size_t begin = i;

      إذا (char_منزل_squeeze == NOT_A_CHAR)
        {
          size_t باہرلا_len;
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
          لكل (; i < nr && !وچ_squeeze_سیٹ[منزل_uchar (buf[i])]; i += 2)
            استمر;

          /* There is a special case when i == nr and we've just
             skipped a character (the last one in buf) that is in
             the squeeze set.  */
          إذا (i == nr && وچ_squeeze_سیٹ[منزل_uchar (buf[i - 1])])
            --i;

          إذا (i >= nr)
            باہرلا_len = nr - begin;
          وإلا
            {
              char_منزل_squeeze = buf[i];
              /* We're about to output buf[begin..i].  */
              باہرلا_len = i - begin + 1;

              /* But since we stepped by 2 in the loop above,
                 out_len may be one too large.  */
              إذا (i > 0 && buf[i - 1] == char_منزل_squeeze)
                --باہرلا_len;

              /* Advance i to the index of first character to be
                 considered when looking for a char different from
                 char_to_squeeze.  */
              ++i;
            }
          إذا (باہرلا_len > 0
              && fwrite (&buf[begin], 1, باہرلا_len, stdout) != باہرلا_len)
            write_error ();
        }

      إذا (char_منزل_squeeze != NOT_A_CHAR)
        {
          /* Advance i to index of first char != char_to_squeeze
             (or to nr if all the rest of the characters in this
             buffer are the same as char_to_squeeze).  */
          لكل (; i < nr && buf[i] == char_منزل_squeeze; i++)
            استمر;
          إذا (i < nr)
            char_منزل_squeeze = NOT_A_CHAR;
          /* If (i >= nr) we've squeezed the last character in this buffer.
             So now we have to read a new buffer and continue comparing
             characters against char_to_squeeze.  */
        }
    }
}

ساكن size_t
plain_read (حرف *buf, size_t ناپ)
{
  ssize_t nr = اقرأ (STDIN_FILENO, buf, ناپ);
  إذا (nr < 0)
    error (EXIT_FAILURE, errno, _("read error"));
  أرجع nr;
}

/* Read buffers of SIZE bytes from stdin until one is found that
   contains at least one character not in the delete set.  Store
   in the array BUF, all characters from that buffer that are not
   in the delete set, and return the number of characters saved
   or 0 upon EOF.  */

ساكن size_t
read_and_delete (حرف *buf, size_t ناپ)
{
  size_t n_saved;

  /* This enclosing do-while loop is to make sure that
     we don't return zero (indicating EOF) when we've
     just deleted all the characters in a buffer.  */
  افعل
    {
      size_t nr = plain_read (buf, ناپ);

      إذا (nr == 0)
        أرجع 0;

      /* This first loop may be a waste of code, but gives much
         better performance when no characters are deleted in
         the beginning of a buffer.  It just avoids the copying
         of buf[i] into buf[n_saved] when it would be a NOP.  */

      size_t i;
      لكل (i = 0; i < nr && !وچ_delete_سیٹ[منزل_uchar (buf[i])]; i++)
        استمر;
      n_saved = i;

      لكل (++i; i < nr; i++)
        إذا (!وچ_delete_سیٹ[منزل_uchar (buf[i])])
          buf[n_saved++] = buf[i];
    }
  طالما (n_saved == 0);

  أرجع n_saved;
}

/* Read at most SIZE bytes from stdin into the array BUF.  Then
   perform the in-place and one-to-one mapping specified by the global
   array 'xlate'.  Return the number of characters read, or 0 upon EOF.  */

ساكن size_t
read_and_xlate (حرف *buf, size_t ناپ)
{
  size_t بائٹاں_read = plain_read (buf, ناپ);

  لكل (size_t i = 0; i < بائٹاں_read; i++)
    buf[i] = xlate[منزل_uchar (buf[i])];

  أرجع بائٹاں_read;
}

/* Initialize a boolean membership set, IN_SET, with the character
   values obtained by traversing the linked list of constructs S
   using the function 'get_next'.  IN_SET is expected to have been
   initialized to all zeros by the caller.  If COMPLEMENT_THIS_SET
   is true the resulting set is complemented.  */

ساكن فراغ
سیٹ_initialize (بنية Spec_list *s, bool complement_this_سیٹ, bool *وچ_سیٹ)
{
  صحيح c;

  s->state = BEGIN_STATE;
  طالما ((c = get_اگلا (s, عدم)) != -1)
    وچ_سیٹ[c] = true;
  إذا (complement_this_سیٹ)
    لكل (size_t i = 0; i < N_CHARS; i++)
      وچ_سیٹ[i] = (!وچ_سیٹ[i]);
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  صحيح c;
  صحيح non_option_دلیلاں;
  صحيح min_operands;
  صحيح max_operands;
  بنية Spec_list buf1, buf2;
  بنية Spec_list *s1 = &buf1;
  بنية Spec_list *s2 = &buf2;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((c = getopt_long (argc, argv, "+AcCdstu", long_options, عدم)) != -1)
    {
      بدل (c)
        {
        حالة 'A':
          /* Undocumented option, for compatibility with AIX.  */
          setlocale (LC_COLLATE, "C");
          setlocale (LC_CTYPE, "C");
          اكسر;

        حالة 'c':
        حالة 'C':
          complement = true;
          اكسر;

        حالة 'd':
          delete = true;
          اكسر;

        حالة 's':
          squeeze_repeats = true;
          اكسر;

        حالة 't':
          truncate_set1 = true;
          اكسر;

        حالة 'u':
          /* Undocumented option, for compatibility with FreeBSD/macOS.  */
          setvbuf (stdout, عدم, _IONBF, 0);
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
          اكسر;
        }
    }

  non_option_دلیلاں = argc - optind;
  translating = (non_option_دلیلاں == 2 && !delete);
  min_operands = 1 + (delete == squeeze_repeats);
  max_operands = 1 + (delete <= squeeze_repeats);

  إذا (non_option_دلیلاں < min_operands)
    {
      إذا (non_option_دلیلاں == 0)
        error (0, 0, _("missing operand"));
      وإلا
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

  إذا (max_operands < non_option_دلیلاں)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + max_operands]));
      إذا (non_option_دلیلاں == 2)
        fprintf (stderr, "%s\n",
                 _("Only one string may be given when "
                   "deleting without squeezing repeats."));
      usage (EXIT_FAILURE);
    }

  spec_init (s1);
  إذا (!parse_str (argv[optind], s1))
    main_exit (EXIT_FAILURE);

  إذا (non_option_دلیلاں == 2)
    {
      spec_init (s2);
      إذا (!parse_str (argv[optind + 1], s2))
        main_exit (EXIT_FAILURE);
    }
  وإلا
    s2 = عدم;

  validate (s1, s2);

  /* Use binary I/O, since 'tr' is sometimes used to transliterate
     non-printable characters, or characters which are stripped away
     by text-mode reads (like CR and ^Z).  */
  xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
  xset_ثنائی_منوال (STDOUT_FILENO, O_ثنائی);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  إذا (squeeze_repeats && non_option_دلیلاں == 1)
    {
      سیٹ_initialize (s1, complement, وچ_squeeze_سیٹ);
      squeeze_filter (io_buf, حجم io_buf, plain_read);
    }
  وإلا إذا (delete && non_option_دلیلاں == 1)
    {
      سیٹ_initialize (s1, complement, وچ_delete_سیٹ);

      طالما (true)
        {
          size_t nr = read_and_delete (io_buf, حجم io_buf);
          إذا (nr == 0)
            اكسر;
          إذا (fwrite (io_buf, 1, nr, stdout) != nr)
            write_error ();
        }
    }
  وإلا إذا (squeeze_repeats && delete && non_option_دلیلاں == 2)
    {
      سیٹ_initialize (s1, complement, وچ_delete_سیٹ);
      سیٹ_initialize (s2, false, وچ_squeeze_سیٹ);
      squeeze_filter (io_buf, حجم io_buf, read_and_delete);
    }
  وإلا إذا (translating)
    {
      إذا (complement)
        {
          bool *وچ_s1 = وچ_delete_سیٹ;

          سیٹ_initialize (s1, false, وچ_s1);
          s2->state = BEGIN_STATE;
          لكل (صحيح i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          لكل (صحيح i = 0; i < N_CHARS; i++)
            {
              إذا (!وچ_s1[i])
                {
                  صحيح ch = get_اگلا (s2, عدم);
                  affirm (ch != -1 || truncate_set1);
                  إذا (ch == -1)
                    {
                      /* This will happen when tr is invoked like e.g.
                         tr -cs A-Za-z0-9 '\012'.  */
                      اكسر;
                    }
                  xlate[i] = ch;
                }
            }
        }
      وإلا
        {
          صحيح c1, c2;
          تعداد اتلا_ہیٹھلا_class class_s1;
          تعداد اتلا_ہیٹھلا_class class_s2;

          لكل (صحيح i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          s1->state = BEGIN_STATE;
          s2->state = BEGIN_STATE;
          طالما (true)
            {
              c1 = get_اگلا (s1, &class_s1);
              c2 = get_اگلا (s2, &class_s2);

              إذا (class_s1 == UL_ہیٹھلا && class_s2 == UL_اتلا)
                {
                  لكل (صحيح i = 0; i < N_CHARS; i++)
                    إذا (islower (i))
                      xlate[i] = وڈا_حرف (i);
                }
              وإلا إذا (class_s1 == UL_اتلا && class_s2 == UL_ہیٹھلا)
                {
                  لكل (صحيح i = 0; i < N_CHARS; i++)
                    إذا (isupper (i))
                      xlate[i] = tolower (i);
                }
              وإلا
                {
                  /* The following should have been checked by validate...  */
                  إذا (c1 == -1 || c2 == -1)
                    اكسر;
                  xlate[c1] = c2;
                }

              /* When case-converting, skip the elements as an optimization.  */
              إذا (class_s2 != UL_NONE)
                {
                  skip_construct (s1);
                  skip_construct (s2);
                }
            }
          affirm (c1 == -1 || truncate_set1);
        }
      إذا (squeeze_repeats)
        {
          سیٹ_initialize (s2, false, وچ_squeeze_سیٹ);
          squeeze_filter (io_buf, حجم io_buf, read_and_xlate);
        }
      وإلا
        {
          طالما (true)
            {
              size_t بائٹاں_read = read_and_xlate (io_buf, حجم io_buf);
              إذا (بائٹاں_read == 0)
                اكسر;
              إذا (fwrite (io_buf, 1, بائٹاں_read, stdout) != بائٹاں_read)
                write_error ();
            }
        }
    }

  إذا (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  main_exit (EXIT_SUCCESS);
}
