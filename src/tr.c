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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "tr"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Jim Meyering")

ਗਿਣਤੀ { N_CHARS = UCHAR_MAX + 1 };

/* An unsigned integer type big enough to hold a repeat count or an
   unsigned character.  POSIX requires support for repeat counts as
   high as 2**31 - 1.  Since repeat counts might need to expand to
   match the length of an argument string, we need at least size_t to
   avoid arbitrary internal limits.  It doesn't cost much to use
   uintmax_t, though.  */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uintmax_t ਗਿਣਤੀ_2;

/* The value for Spec_list->state that indicates to
   get_next that it should initialize the tail pointer.
   Its value should be as large as possible to avoid conflict
   a valid value for the state field -- and that may be as
   large as any valid repeat_count.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BEGIN_STATE (UINTMAX_MAX - 1)

/* The value for Spec_list->state that indicates to
   get_next that the element pointed to by Spec_list->tail is
   being considered for the first time on this pass through the
   list -- it indicates that get_next should make any necessary
   initializations.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NEW_ELEMENT (BEGIN_STATE + 1)

/* The maximum possible repeat count.  Due to how the states are
   implemented, it can be as much as BEGIN_STATE.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REPEAT_ਗਿਣਤੀ_ਵੱਧ_ਤੋਂ_ਵੱਧ BEGIN_STATE

/* The following (but not CC_NO_CLASS) are indices into the array of
   valid character class strings.  */
ਗਿਣਤੀ Char_class
  {
    CC_ALNUM = 0, CC_ALPHA = 1, CC_BLANK = 2, CC_CNTRL = 3,
    CC_DIGIT = 4, CC_ਗ੍ਰਾਫ = 5, CC_ਹੇਠਲਾ = 6, CC_PRINT = 7,
    CC_PUNCT = 8, CC_SPACE = 9, CC_ਉੱਤਲਾ = 10, CC_XDIGIT = 11,
    CC_NO_CLASS = 9999
  };

/* Character class to which a character (returned by get_next) belonged;
   but it is set only if the construct from which the character was obtained
   was one of the character classes [:upper:] or [:lower:].  The value
   is used only when translating and then, only to make sure that upper
   and lower class constructs have the same relative positions in string1
   and string2.  */
ਗਿਣਤੀ ਉੱਤਲਾ_ਹੇਠਲਾ_class
  {
    UL_ਹੇਠਲਾ,
    UL_ਉੱਤਲਾ,
    UL_NONE
  };

/* The type of a List_element.  See build_spec_list for more details.  */
ਗਿਣਤੀ Range_element_type
  {
    RE_ਸਾਧਾਰਨ_CHAR,
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
ਬਣਤਰ List_element
  {
    ਗਿਣਤੀ Range_element_type type;
    ਬਣਤਰ List_element *ਅਗਲਾ;
    ਸੰਘ
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਸਾਧਾਰਨ_char;
        ਬਣਤਰ			/* unnamed */
          {
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਪਹਿਲਾ_char;
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਆਖਰੀ_char;
          }
        range;
        ਗਿਣਤੀ Char_class char_class;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ equiv_ਕੋਡ;
        ਬਣਤਰ			/* unnamed */
          {
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ the_repeated_char;
            ਗਿਣਤੀ_2 repeat_ਗਿਣਤੀ;
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
ਬਣਤਰ Spec_list
  {
    /* Points to the head of the list of range elements.
       The first struct is a dummy; its members are never used.  */
    ਬਣਤਰ List_element *ਸਿਰ;

    /* When appending, points to the last element.  When traversing via
       get_next(), points to the element to process next.  Setting
       Spec_list.state to the value BEGIN_STATE before calling get_next
       signals get_next to initialize tail to point to head->next.  */
    ਬਣਤਰ List_element *ਪੂਛ;

    /* Used to save state between calls to get_next.  */
    ਗਿਣਤੀ_2 state;

    /* Length, in the sense that length ('a-z[:digit:]123abc')
       is 42 ( = 26 + 10 + 6).  */
    ਗਿਣਤੀ_2 ਲੰਮਾਈ;

    /* The number of [c*] and [c*0] constructs that appear in this spec.  */
    size_t n_indefinite_repeats;

    /* If n_indefinite_repeats is nonzero, this points to the List_element
       corresponding to the last [c*] or [c*0] construct encountered in
       this spec.  Otherwise it is undefined.  */
    ਬਣਤਰ List_element *indefinite_repeat_element;

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
ਬਣਤਰ E_string
{
  ਅੱਖਰ *s;
  bool *escaped;
  size_t len;
};

/* Return nonzero if the Ith character of escaped string ES matches C
   and is not escaped itself.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
es_ਮੇਲ (ਬਣਤਰ E_string ਅਚਲ *es, size_t i, ਅੱਖਰ c)
{
  ਵਾਪਸ es->s[i] == c && !es->escaped[i];
}

/* When true, each sequence in the input of a repeated character
   (call it c) is replaced (in the output) by a single occurrence of c
   for every c in the squeeze set.  */
ਸਥਿਰ bool squeeze_repeats = false;

/* When true, removes characters in the delete set from input.  */
ਸਥਿਰ bool delete = false;

/* Use the complement of set1 in place of set1.  */
ਸਥਿਰ bool complement = false;

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
ਸਥਿਰ bool truncate_set1 = false;

/* An alias for (!delete && non_option_args == 2).
   It is set in main and used there and in validate().  */
ਸਥਿਰ bool translating;

ਸਥਿਰ ਅੱਖਰ io_buf[BUFSIZ];

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ char_class_name[] =
{
  "alnum", "alpha", "blank", "cntrl", "digit", "graph",
  "lower", "print", "punct", "space", "upper", "xdigit"
};

/* Array of boolean values.  A character 'c' is a member of the
   squeeze set if and only if in_squeeze_set[c] is true.  The squeeze
   set is defined by the last (possibly, the only) string argument
   on the command line when the squeeze option is given.  */
ਸਥਿਰ bool ਵਿੱਚ_squeeze_ਸੈੱਟ[N_CHARS];

/* Array of boolean values.  A character 'c' is a member of the
   delete set if and only if in_delete_set[c] is true.  The delete
   set is defined by the first (or only) string argument on the
   command line when the delete option is given.  */
ਸਥਿਰ bool ਵਿੱਚ_delete_ਸੈੱਟ[N_CHARS];

/* Array of character values defining the translation (if any) that
   tr is to perform.  Translation is performed only when there are
   two specification strings and the delete switch is not given.  */
ਸਥਿਰ ਅੱਖਰ xlate[N_CHARS];

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"complement", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"delete", no_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"squeeze-repeats", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"truncate-set1", no_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... STRING1 [STRING2]\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Translate, squeeze, and/or delete characters from standard ਨਿਵੇਸ਼,\n\
writing ਮੰਜ਼ਿਲ standard output.  STRING1 and STRING2 specify arrays ਦਾ\n\
characters ARRAY1 and ARRAY2 that control the action.\n\
\n\
"), stdout);
      oputs (_("\
  -c, -C, --complement\n\
         use the complement ਦਾ ARRAY1\n\
"));
      oputs (_("\
  -d, --delete\n\
         delete characters ਵਿੱਚ ARRAY1, ਕਰੋ not translate\n\
"));
      oputs (_("\
  -s, --squeeze-repeats\n\
         replace each sequence ਦਾ a repeated character\n\
         that is listed ਵਿੱਚ the ਆਖਰੀ specified ARRAY,\n\
         with a single occurrence ਦਾ that character\n\
"));
      oputs (_("\
  -t, --truncate-set1\n\
         ਪਹਿਲਾ truncate ARRAY1 ਮੰਜ਼ਿਲ ਲੰਮਾਈ ਦਾ ARRAY2\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ARRAYs are specified as ਲੜੀਆਂ ਦਾ characters.  Most represent themselves.\n\
Interpreted sequences are:\n\
\n\
  \\NNN            character with octal ਮੁੱਲ NNN (1 ਮੰਜ਼ਿਲ 3 octal digits)\n\
  \\\\              backslash\n\
  \\a              audible BEL\n\
  \\b              backspace\n\
  \\f              form feed\n\
  \\n              new ਸਤਰ\n\
  \\r              ਵਾਪਸ\n\
  \\t              horizontal tab\n\
"), stdout);
     fputs (_("\
  \\v              vertical tab\n\
  CHAR1-CHAR2     all characters from CHAR1 ਮੰਜ਼ਿਲ CHAR2 ਵਿੱਚ ascending ਕ੍ਰਮ\n\
  [CHAR*]         ਵਿੱਚ ARRAY2, copies ਦਾ CHAR until ਲੰਮਾਈ ਦਾ ARRAY1\n\
  [CHAR*REPEAT]   REPEAT copies ਦਾ CHAR, REPEAT octal ਜੇ starting with 0\n\
  [:alnum:]       all letters and digits\n\
  [:alpha:]       all letters\n\
  [:blank:]       all horizontal whitespace\n\
  [:cntrl:]       all control characters\n\
  [:digit:]       all digits\n\
"), stdout);
     fputs (_("\
  [:ਗ੍ਰਾਫ:]       all printable characters, not including space\n\
  [:ਹੇਠਲਾ:]       all ਹੇਠਲਾ ਹਾਲਤ letters\n\
  [:print:]       all printable characters, including space\n\
  [:punct:]       all punctuation characters\n\
  [:space:]       all horizontal or vertical whitespace\n\
  [:ਉੱਤਲਾ:]       all ਉੱਤਲਾ ਹਾਲਤ letters\n\
  [:xdigit:]      all hexadecimal digits\n\
  [=CHAR=]        all characters which are equivalent ਮੰਜ਼ਿਲ CHAR\n\
"), stdout);
     fputs (_("\
\n\
Translation occurs ਜੇ -d is not given and both STRING1 and STRING2 appear.\n\
-t is only significant when translating.  ARRAY2 is extended ਮੰਜ਼ਿਲ ਲੰਮਾਈ ਦਾ\n\
ARRAY1 by repeating its ਆਖਰੀ character as necessary.  Excess characters\n\
ਦਾ ARRAY2 are ignored.  Character classes expand ਵਿੱਚ unspecified ਕ੍ਰਮ;\n\
ਜਦੋਂ translating, '[:lower:]' and '[:upper:]' may be ਵਰਤਿਆ ਵਿੱਚ ਜੋੜੇ ਮੰਜ਼ਿਲ\n\
specify ਹਾਲਤ conversion.  Squeezing occurs after translation or deletion.\n\
Arguments like '[...]' should be quoted, ਮੰਜ਼ਿਲ avoid potential shell globbing.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Return nonzero if the character C is a member of the
   equivalence class containing the character EQUIV_CLASS.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_equiv_class_member (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ equiv_class, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{
  ਵਾਪਸ (equiv_class == c);
}

/* Return true if the character C is a member of the
   character class CHAR_CLASS.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
is_char_class_member (ਗਿਣਤੀ Char_class char_class, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{
  ਪੂਰਨ ਨਤੀਜਾ;

  ਚੋਣ (char_class)
    {
    ਹਾਲਤ CC_ALNUM:
      ਨਤੀਜਾ = ਅੱਖਰ_ਜਾਂ_ਅੰਕ (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_ALPHA:
      ਨਤੀਜਾ = ਅੱਖਰ_ਹੈ (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_BLANK:
      ਨਤੀਜਾ = isblank (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_CNTRL:
      ਨਤੀਜਾ = iscntrl (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_DIGIT:
      ਨਤੀਜਾ = c_isdigit (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_ਗ੍ਰਾਫ:
      ਨਤੀਜਾ = isgraph (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_ਹੇਠਲਾ:
      ਨਤੀਜਾ = islower (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_PRINT:
      ਨਤੀਜਾ = isprint (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_PUNCT:
      ਨਤੀਜਾ = ispunct (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_SPACE:
      ਨਤੀਜਾ = ਖਾਲੀ_ਥਾਂ_ਹੈ (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_ਉੱਤਲਾ:
      ਨਤੀਜਾ = isupper (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_XDIGIT:
      ਨਤੀਜਾ = c_isxdigit (c);
      ਰੋਕੋ;
    ਹਾਲਤ CC_NO_CLASS: ਮੂਲ:
      affirm (false);
    }

  ਵਾਪਸ !! ਨਤੀਜਾ;
}

ਸਥਿਰ ਖਾਲੀ
es_free (ਬਣਤਰ E_string *es)
{
  ਮੁਕਤ (es->s);
  ਮੁਕਤ (es->escaped);
}

/* Perform the first pass over each range-spec argument S, converting all
   \c and \ddd escapes to their one-byte representations.  If an invalid
   quote sequence is found print an error message and return false;
   Otherwise set *ES to the resulting string and return true.
   The resulting array of characters may contain zero-bytes;
   however, on input, S is assumed to be null-terminated, and hence
   cannot contain actual (non-escaped) zero bytes.  */

ਸਥਿਰ bool
unquote (ਅੱਖਰ ਅਚਲ *s, ਬਣਤਰ E_string *es)
{
  size_t len = ਲੜੀ_ਲੰਬਾਈ (s);

  es->s = xmalloc (len);
  es->escaped = xcalloc (len, ਆਕਾਰ es->escaped[0]);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; s[i]; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c;
      ਪੂਰਨ oct_digit;

      ਚੋਣ (s[i])
        {
        ਹਾਲਤ '\\':
          es->escaped[j] = true;
          ਚੋਣ (s[i + 1])
            {
            ਹਾਲਤ '\\':
              c = '\\';
              ਰੋਕੋ;
            ਹਾਲਤ 'a':
              c = '\a';
              ਰੋਕੋ;
            ਹਾਲਤ 'b':
              c = '\b';
              ਰੋਕੋ;
            ਹਾਲਤ 'f':
              c = '\f';
              ਰੋਕੋ;
            ਹਾਲਤ 'n':
              c = '\n';
              ਰੋਕੋ;
            ਹਾਲਤ 'r':
              c = '\r';
              ਰੋਕੋ;
            ਹਾਲਤ 't':
              c = '\t';
              ਰੋਕੋ;
            ਹਾਲਤ 'v':
              c = '\v';
              ਰੋਕੋ;
            ਹਾਲਤ '0':
            ਹਾਲਤ '1':
            ਹਾਲਤ '2':
            ਹਾਲਤ '3':
            ਹਾਲਤ '4':
            ਹਾਲਤ '5':
            ਹਾਲਤ '6':
            ਹਾਲਤ '7':
              c = s[i + 1] - '0';
              oct_digit = s[i + 2] - '0';
              ਜੇ (0 <= oct_digit && oct_digit <= 7)
                {
                  c = 8 * c + oct_digit;
                  ++i;
                  oct_digit = s[i + 2] - '0';
                  ਜੇ (0 <= oct_digit && oct_digit <= 7)
                    {
                      ਜੇ (8 * c + oct_digit < N_CHARS)
                        {
                          c = 8 * c + oct_digit;
                          ++i;
                        }
                      ਨਹੀਂਤਾਂ
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
              ਰੋਕੋ;
            ਹਾਲਤ '\0':
              error (0, 0, _("warning: an unescaped backslash "
                             "at end of string is not portable"));
              /* POSIX is not clear about this.  */
              es->escaped[j] = false;
              i--;
              c = '\\';
              ਰੋਕੋ;
            ਮੂਲ:
              c = s[i + 1];
              ਰੋਕੋ;
            }
          ++i;
          es->s[j++] = c;
          ਰੋਕੋ;
        ਮੂਲ:
          es->s[j++] = s[i];
          ਰੋਕੋ;
        }
    }
  es->len = j;
  ਵਾਪਸ true;
}

/* If CLASS_STR is a valid character class string, return its index
   in the global char_class_name array.  Otherwise, return CC_NO_CLASS.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਗਿਣਤੀ Char_class
look_ਉੱਪਰ_char_class (ਅੱਖਰ ਅਚਲ *class_str, size_t len)
{
  ਲਈ (ਗਿਣਤੀ Char_class i = 0; i < countof (char_class_name); i++)
    ਜੇ (STREQ_LEN (class_str, char_class_name[i], len)
        && ਲੜੀ_ਲੰਬਾਈ (char_class_name[i]) == len)
      ਵਾਪਸ i;
  ਵਾਪਸ CC_NO_CLASS;
}

/* Return a newly allocated string with a printable version of C.
   This function is used solely for formatting error messages.  */

ਸਥਿਰ ਅੱਖਰ *
make_printable_char (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{
  ਅੱਖਰ *buf = xmalloc (5);

  ਜੇ (isprint (c))
    {
      buf[0] = c;
      buf[1] = '\0';
    }
  ਨਹੀਂਤਾਂ
    {
      sprintf (buf, "\\%03o", c);
    }
  ਵਾਪਸ buf;
}

/* Return a newly allocated copy of S which is suitable for printing.
   LEN is the number of characters in S.  Most non-printing
   (isprint) characters are represented by a backslash followed by
   3 octal digits.  However, the characters represented by \c escapes
   where c is one of [abfnrtv] are represented by their 2-character \c
   sequences.  This function is used solely for printing error messages.  */

ਸਥਿਰ ਅੱਖਰ *
make_printable_str (ਅੱਖਰ ਅਚਲ *s, size_t len)
{
  /* Worst case is that every character expands to a backslash
     followed by a 3-character octal escape sequence.  */
  ਅੱਖਰ *printable_buf = xnmalloc (len + 1, 4);
  ਅੱਖਰ *p = printable_buf;

  ਲਈ (size_t i = 0; i < len; i++)
    {
      ਅੱਖਰ buf[5];
      ਅੱਖਰ ਅਚਲ *tmp = ਨਿਰਾ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = s[i];

      ਚੋਣ (c)
        {
        ਹਾਲਤ '\\':
          tmp = "\\";
          ਰੋਕੋ;
        ਹਾਲਤ '\a':
          tmp = "\\a";
          ਰੋਕੋ;
        ਹਾਲਤ '\b':
          tmp = "\\b";
          ਰੋਕੋ;
        ਹਾਲਤ '\f':
          tmp = "\\f";
          ਰੋਕੋ;
        ਹਾਲਤ '\n':
          tmp = "\\n";
          ਰੋਕੋ;
        ਹਾਲਤ '\r':
          tmp = "\\r";
          ਰੋਕੋ;
        ਹਾਲਤ '\t':
          tmp = "\\t";
          ਰੋਕੋ;
        ਹਾਲਤ '\v':
          tmp = "\\v";
          ਰੋਕੋ;
        ਮੂਲ:
          ਜੇ (isprint (c))
            {
              buf[0] = c;
              buf[1] = '\0';
            }
          ਨਹੀਂਤਾਂ
            sprintf (buf, "\\%03o", c);
          tmp = buf;
          ਰੋਕੋ;
        }
      p = stpcpy (p, tmp);
    }
  ਵਾਪਸ printable_buf;
}

/* Append a newly allocated structure representing a
   character C to the specification list LIST.  */

ਸਥਿਰ ਖਾਲੀ
append_ਸਾਧਾਰਨ_char (ਬਣਤਰ Spec_list *list, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{
  ਬਣਤਰ List_element *new = xmalloc (ਆਕਾਰ *new);
  new->ਅਗਲਾ = ਨਿਰਾ;
  new->type = RE_ਸਾਧਾਰਨ_CHAR;
  new->u.ਸਾਧਾਰਨ_char = c;
  list->ਪੂਛ->ਅਗਲਾ = new;
  list->ਪੂਛ = new;
}

/* Append a newly allocated structure representing the range
   of characters from FIRST to LAST to the specification list LIST.
   Return false if LAST precedes FIRST in the collating sequence,
   true otherwise.  This means that '[c-c]' is acceptable.  */

ਸਥਿਰ bool
append_range (ਬਣਤਰ Spec_list *list, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਪਹਿਲਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਆਖਰੀ)
{
  ਜੇ (ਆਖਰੀ < ਪਹਿਲਾ)
    {
      ਅੱਖਰ *tmp1 = make_printable_char (ਪਹਿਲਾ);
      ਅੱਖਰ *tmp2 = make_printable_char (ਆਖਰੀ);

      error (0, 0,
       _("range-endpoints of '%s-%s' are in reverse collating sequence order"),
             tmp1, tmp2);
      ਮੁਕਤ (tmp1);
      ਮੁਕਤ (tmp2);
      ਵਾਪਸ false;
    }
  ਬਣਤਰ List_element *new = xmalloc (ਆਕਾਰ *new);
  new->ਅਗਲਾ = ਨਿਰਾ;
  new->type = RE_RANGE;
  new->u.range.ਪਹਿਲਾ_char = ਪਹਿਲਾ;
  new->u.range.ਆਖਰੀ_char = ਆਖਰੀ;
  list->ਪੂਛ->ਅਗਲਾ = new;
  list->ਪੂਛ = new;
  ਵਾਪਸ true;
}

/* If CHAR_CLASS_STR is a valid character class string, append a
   newly allocated structure representing that character class to the end
   of the specification list LIST and return true.  If CHAR_CLASS_STR is not
   a valid string return false.  */

ਸਥਿਰ bool
append_char_class (ਬਣਤਰ Spec_list *list,
                   ਅੱਖਰ ਅਚਲ *char_class_str, size_t len)
{
  ਗਿਣਤੀ Char_class char_class = look_ਉੱਪਰ_char_class (char_class_str, len);
  ਜੇ (char_class == CC_NO_CLASS)
    ਵਾਪਸ false;
  ਬਣਤਰ List_element *new = xmalloc (ਆਕਾਰ *new);
  new->ਅਗਲਾ = ਨਿਰਾ;
  new->type = RE_CHAR_CLASS;
  new->u.char_class = char_class;
  list->ਪੂਛ->ਅਗਲਾ = new;
  list->ਪੂਛ = new;
  ਵਾਪਸ true;
}

/* Append a newly allocated structure representing a [c*n]
   repeated character construct to the specification list LIST.
   THE_CHAR is the single character to be repeated, and REPEAT_COUNT
   is a non-negative repeat count.  */

ਸਥਿਰ ਖਾਲੀ
append_repeated_char (ਬਣਤਰ Spec_list *list, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ the_char,
                      ਗਿਣਤੀ_2 repeat_ਗਿਣਤੀ)
{
  ਬਣਤਰ List_element *new = xmalloc (ਆਕਾਰ *new);
  new->ਅਗਲਾ = ਨਿਰਾ;
  new->type = RE_REPEATED_CHAR;
  new->u.repeated_char.the_repeated_char = the_char;
  new->u.repeated_char.repeat_ਗਿਣਤੀ = repeat_ਗਿਣਤੀ;
  list->ਪੂਛ->ਅਗਲਾ = new;
  list->ਪੂਛ = new;
}

/* Given a string, EQUIV_CLASS_STR, from a [=str=] context and
   the length of that string, LEN, if LEN is exactly one, append
   a newly allocated structure representing the specified
   equivalence class to the specification list, LIST and return true.
   If LEN is not 1, return false.  */

ਸਥਿਰ bool
append_equiv_class (ਬਣਤਰ Spec_list *list,
                    ਅੱਖਰ ਅਚਲ *equiv_class_str, size_t len)
{
  ਜੇ (len != 1)
    ਵਾਪਸ false;

  ਬਣਤਰ List_element *new = xmalloc (ਆਕਾਰ *new);
  new->ਅਗਲਾ = ਨਿਰਾ;
  new->type = RE_EQUIV_CLASS;
  new->u.equiv_ਕੋਡ = *equiv_class_str;
  list->ਪੂਛ->ਅਗਲਾ = new;
  list->ਪੂਛ = new;
  ਵਾਪਸ true;
}

/* Search forward starting at START_IDX for the 2-char sequence
   (PRE_BRACKET_CHAR,']') in the string P of length P_LEN.  If such
   a sequence is found, set *RESULT_IDX to the index of the first
   character and return true.  Otherwise return false.  P may contain
   zero bytes.  */

ਸਥਿਰ bool
ਲੱਭੋ_closing_delim (ਅਚਲ ਬਣਤਰ E_string *es, size_t ਸ਼ੁਰੂ_idx,
                    ਅੱਖਰ pre_bracket_char, size_t *ਨਤੀਜਾ_idx)
{
  ਲਈ (size_t i = ਸ਼ੁਰੂ_idx; i < es->len - 1; i++)
    ਜੇ (es->s[i] == pre_bracket_char && es->s[i + 1] == ']'
        && !es->escaped[i] && !es->escaped[i + 1])
      {
        *ਨਤੀਜਾ_idx = i;
        ਵਾਪਸ true;
      }
  ਵਾਪਸ false;
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

ਸਥਿਰ ਪੂਰਨ
ਲੱਭੋ_bracketed_repeat (ਅਚਲ ਬਣਤਰ E_string *es, size_t ਸ਼ੁਰੂ_idx,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *char_ਮੰਜ਼ਿਲ_repeat, ਗਿਣਤੀ_2 *repeat_ਗਿਣਤੀ,
                       size_t *closing_bracket_idx)
{
  affirm (ਸ਼ੁਰੂ_idx + 1 < es->len);
  ਜੇ (!es_ਮੇਲ (es, ਸ਼ੁਰੂ_idx + 1, '*'))
    ਵਾਪਸ -1;

  ਲਈ (size_t i = ਸ਼ੁਰੂ_idx + 2; i < es->len && !es->escaped[i]; i++)
    {
      ਜੇ (es->s[i] == ']')
        {
          size_t digit_str_len = i - ਸ਼ੁਰੂ_idx - 2;

          *char_ਮੰਜ਼ਿਲ_repeat = es->s[ਸ਼ੁਰੂ_idx];
          ਜੇ (digit_str_len == 0)
            {
              /* We've matched [c*] -- no explicit repeat count.  */
              *repeat_ਗਿਣਤੀ = 0;
            }
          ਨਹੀਂਤਾਂ
            {
              /* Here, we have found [c*s] where s should be a string
                 of octal (if it starts with '0') or decimal digits.  */
              ਅੱਖਰ ਅਚਲ *digit_str = &es->s[ਸ਼ੁਰੂ_idx + 2];
              ਅੱਖਰ *d_ਸਿਰਾ;
              ਜੇ ((xstrtoumax (digit_str, &d_ਸਿਰਾ, *digit_str == '0' ? 8 : 10,
                               repeat_ਗਿਣਤੀ, ਨਿਰਾ)
                   != LONGINT_OK)
                  || REPEAT_ਗਿਣਤੀ_ਵੱਧ_ਤੋਂ_ਵੱਧ < *repeat_ਗਿਣਤੀ
                  || digit_str + digit_str_len != d_ਸਿਰਾ)
                {
                  ਅੱਖਰ *tmp = make_printable_str (digit_str, digit_str_len);
                  error (0, 0,
                         _("invalid repeat count %s in [c*n] construct"),
                         quote (tmp));
                  ਮੁਕਤ (tmp);
                  ਵਾਪਸ -2;
                }
            }
          *closing_bracket_idx = i;
          ਵਾਪਸ 0;
        }
    }
  ਵਾਪਸ -1;			/* No bracket found.  */
}

/* Return true if the string at ES->s[IDX] matches the regular
   expression '\*[0-9]*]', false otherwise.  The string does not
   match if any of its characters are escaped.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
ਤਾਰਾ_digits_closebracket (ਅਚਲ ਬਣਤਰ E_string *es, size_t idx)
{
  ਜੇ (!es_ਮੇਲ (es, idx, '*'))
    ਵਾਪਸ false;

  ਲਈ (size_t i = idx + 1; i < es->len; i++)
    ਜੇ (!c_isdigit (es->s[i]) || es->escaped[i])
      ਵਾਪਸ es_ਮੇਲ (es, i, ']');
  ਵਾਪਸ false;
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

ਸਥਿਰ bool
build_spec_list (ਅਚਲ ਬਣਤਰ E_string *es, ਬਣਤਰ Spec_list *ਨਤੀਜਾ)
{
  ਅੱਖਰ ਅਚਲ *p = es->s;

  /* The main for-loop below recognizes the 4 multi-character constructs.
     A character that matches (in its context) none of the multi-character
     constructs is classified as 'normal'.  Since all multi-character
     constructs have at least 3 characters, any strings of length 2 or
     less are composed solely of normal characters.  Hence, the index of
     the outer for-loop runs only as far as LEN-2.  */
  size_t i;
  ਲਈ (i = 0; i + 2 < es->len; /*ਖਾਲੀ_ਥਾਵਾਂ*/)
    {
      ਜੇ (es_ਮੇਲ (es, i, '['))
        {
          bool matched_multi_char_construct;
          size_t closing_bracket_idx;
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ char_ਮੰਜ਼ਿਲ_repeat;
          ਗਿਣਤੀ_2 repeat_ਗਿਣਤੀ;
          ਪੂਰਨ err;

          matched_multi_char_construct = true;
          ਜੇ (es_ਮੇਲ (es, i + 1, ':') || es_ਮੇਲ (es, i + 1, '='))
            {
              size_t closing_delim_idx;

              ਜੇ (ਲੱਭੋ_closing_delim (es, i + 2, p[i + 1], &closing_delim_idx))
                {
                  size_t opnd_str_len = closing_delim_idx - 1 - (i + 2) + 1;
                  ਅੱਖਰ ਅਚਲ *opnd_str = p + i + 2;

                  ਜੇ (opnd_str_len == 0)
                    {
                      ਜੇ (p[i + 1] == ':')
                        error (0, 0, _("missing character class name '[::]'"));
                      ਨਹੀਂਤਾਂ
                        error (0, 0,
                               _("missing equivalence class character '[==]'"));
                      ਵਾਪਸ false;
                    }

                  ਜੇ (p[i + 1] == ':')
                    {
                      /* FIXME: big comment.  */
                      ਜੇ (!append_char_class (ਨਤੀਜਾ, opnd_str, opnd_str_len))
                        {
                          ਜੇ (ਤਾਰਾ_digits_closebracket (es, i + 2))
                            ਜਾਓ try_bracketed_repeat;
                          ਨਹੀਂਤਾਂ
                            {
                              ਅੱਖਰ *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0, _("invalid character class %s"),
                                     quote (tmp));
                              ਮੁਕਤ (tmp);
                              ਵਾਪਸ false;
                            }
                        }
                    }
                  ਨਹੀਂਤਾਂ
                    {
                      /* FIXME: big comment.  */
                      ਜੇ (!append_equiv_class (ਨਤੀਜਾ, opnd_str, opnd_str_len))
                        {
                          ਜੇ (ਤਾਰਾ_digits_closebracket (es, i + 2))
                            ਜਾਓ try_bracketed_repeat;
                          ਨਹੀਂਤਾਂ
                            {
                              ਅੱਖਰ *tmp = make_printable_str (opnd_str,
                                                              opnd_str_len);
                              error (0, 0,
               _("%s: equivalence class operand must be a single character"),
                                     tmp);
                              ਮੁਕਤ (tmp);
                              ਵਾਪਸ false;
                            }
                        }
                    }

                  i = closing_delim_idx + 2;
                  ਜਾਰੀ;
                }
              /* Else fall through.  This could be [:*] or [=*].  */
            }

        try_bracketed_repeat:

          /* Determine whether this is a bracketed repeat range
             matching the RE \[.\*(dec_or_oct_number)?].  */
          err = ਲੱਭੋ_bracketed_repeat (es, i + 1, &char_ਮੰਜ਼ਿਲ_repeat,
                                       &repeat_ਗਿਣਤੀ,
                                       &closing_bracket_idx);
          ਜੇ (err == 0)
            {
              append_repeated_char (ਨਤੀਜਾ, char_ਮੰਜ਼ਿਲ_repeat, repeat_ਗਿਣਤੀ);
              i = closing_bracket_idx + 1;
            }
          ਨਹੀਂਤਾਂ ਜੇ (err == -1)
            {
              matched_multi_char_construct = false;
            }
          ਨਹੀਂਤਾਂ
            {
              /* Found a string that looked like [c*n] but the
                 numeric part was invalid.  */
              ਵਾਪਸ false;
            }

          ਜੇ (matched_multi_char_construct)
            ਜਾਰੀ;

          /* We reach this point if P does not match [:str:], [=c=],
             [c*n], or [c*].  Now, see if P looks like a range '[-c'
             (from '[' to 'c').  */
        }

      /* Look ahead one char for ranges like a-z.  */
      ਜੇ (es_ਮੇਲ (es, i + 1, '-'))
        {
          ਜੇ (!append_range (ਨਤੀਜਾ, p[i], p[i + 2]))
            ਵਾਪਸ false;
          i += 3;
        }
      ਨਹੀਂਤਾਂ
        {
          append_ਸਾਧਾਰਨ_char (ਨਤੀਜਾ, p[i]);
          ++i;
        }
    }

  /* Now handle the (2 or fewer) remaining characters p[i]..p[es->len - 1].  */
  ਲਈ (; i < es->len; i++)
    append_ਸਾਧਾਰਨ_char (ਨਤੀਜਾ, p[i]);

  ਵਾਪਸ true;
}

/* Advance past the current construct.
   S->tail must be non-null.  */
ਸਥਿਰ ਖਾਲੀ
skip_construct (ਬਣਤਰ Spec_list *s)
{
  s->ਪੂਛ = s->ਪੂਛ->ਅਗਲਾ;
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

ਸਥਿਰ ਪੂਰਨ
get_ਅਗਲਾ (ਬਣਤਰ Spec_list *s, ਗਿਣਤੀ ਉੱਤਲਾ_ਹੇਠਲਾ_class *class)
{
  ਬਣਤਰ List_element *p;
  ਪੂਰਨ return_val;
  ਪੂਰਨ i;

  ਜੇ (class)
    *class = UL_NONE;

  ਜੇ (s->state == BEGIN_STATE)
    {
      s->ਪੂਛ = s->ਸਿਰ->ਅਗਲਾ;
      s->state = NEW_ELEMENT;
    }

  p = s->ਪੂਛ;
  ਜੇ (p == ਨਿਰਾ)
    ਵਾਪਸ -1;

  ਚੋਣ (p->type)
    {
    ਹਾਲਤ RE_ਸਾਧਾਰਨ_CHAR:
      return_val = p->u.ਸਾਧਾਰਨ_char;
      s->state = NEW_ELEMENT;
      s->ਪੂਛ = p->ਅਗਲਾ;
      ਰੋਕੋ;

    ਹਾਲਤ RE_RANGE:
      ਜੇ (s->state == NEW_ELEMENT)
        s->state = p->u.range.ਪਹਿਲਾ_char;
      ਨਹੀਂਤਾਂ
        ++(s->state);
      return_val = s->state;
      ਜੇ (s->state == p->u.range.ਆਖਰੀ_char)
        {
          s->ਪੂਛ = p->ਅਗਲਾ;
          s->state = NEW_ELEMENT;
        }
      ਰੋਕੋ;

    ਹਾਲਤ RE_CHAR_CLASS:
      ਜੇ (class)
        {
          ਚੋਣ (+p->u.char_class)
            {
            ਹਾਲਤ CC_ਹੇਠਲਾ:
              *class = UL_ਹੇਠਲਾ;
              ਰੋਕੋ;
            ਹਾਲਤ CC_ਉੱਤਲਾ:
              *class = UL_ਉੱਤਲਾ;
              ਰੋਕੋ;
            }
        }

      ਜੇ (s->state == NEW_ELEMENT)
        {
          ਲਈ (i = 0; i < N_CHARS; i++)
            ਜੇ (is_char_class_member (p->u.char_class, i))
              ਰੋਕੋ;
          affirm (i < N_CHARS);
          s->state = i;
        }
      assure (is_char_class_member (p->u.char_class, s->state));
      return_val = s->state;
      ਲਈ (i = s->state + 1; i < N_CHARS; i++)
        ਜੇ (is_char_class_member (p->u.char_class, i))
          ਰੋਕੋ;
      ਜੇ (i < N_CHARS)
        s->state = i;
      ਨਹੀਂਤਾਂ
        {
          s->ਪੂਛ = p->ਅਗਲਾ;
          s->state = NEW_ELEMENT;
        }
      ਰੋਕੋ;

    ਹਾਲਤ RE_EQUIV_CLASS:
      /* FIXME: this assumes that each character is alone in its own
         equivalence class (which appears to be correct for my
         LC_COLLATE.  But I don't know of any function that allows
         one to determine a character's equivalence class.  */

      return_val = p->u.equiv_ਕੋਡ;
      s->state = NEW_ELEMENT;
      s->ਪੂਛ = p->ਅਗਲਾ;
      ਰੋਕੋ;

    ਹਾਲਤ RE_REPEATED_CHAR:
      /* Here, a repeat count of n == 0 means don't repeat at all.  */
      ਜੇ (p->u.repeated_char.repeat_ਗਿਣਤੀ == 0)
        {
          s->ਪੂਛ = p->ਅਗਲਾ;
          s->state = NEW_ELEMENT;
          return_val = get_ਅਗਲਾ (s, class);
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (s->state == NEW_ELEMENT)
            {
              s->state = 0;
            }
          ++(s->state);
          return_val = p->u.repeated_char.the_repeated_char;
          ਜੇ (s->state == p->u.repeated_char.repeat_ਗਿਣਤੀ)
            {
              s->ਪੂਛ = p->ਅਗਲਾ;
              s->state = NEW_ELEMENT;
            }
        }
      ਰੋਕੋ;

    ਮੂਲ:
      unreachable ();
    }

  ਵਾਪਸ return_val;
}

/* This is a minor kludge.  This function is called from
   get_spec_stats to determine the cardinality of a set derived
   from a complemented string.  It's a kludge in that some of the
   same operations are (duplicated) performed in set_initialize.  */

ਸਥਿਰ ਪੂਰਨ
card_ਦਾ_complement (ਬਣਤਰ Spec_list *s)
{
  ਪੂਰਨ c;
  ਪੂਰਨ cardinality = N_CHARS;
  bool ਵਿੱਚ_ਸੈੱਟ[N_CHARS] = {0};

  s->state = BEGIN_STATE;
  ਜਦੋਂ ((c = get_ਅਗਲਾ (s, ਨਿਰਾ)) != -1)
    {
      cardinality -= (!ਵਿੱਚ_ਸੈੱਟ[c]);
      ਵਿੱਚ_ਸੈੱਟ[c] = true;
    }
  ਵਾਪਸ cardinality;
}

/* Discard the lengths associated with a case conversion,
   as using the actual number of upper or lower case characters
   is problematic when they don't match in some locales.
   Also ensure the case conversion classes in string2 are
   aligned correctly with those in string1.
   Note POSIX says the behavior of 'tr "[:upper:]" "[:upper:]"'
   is undefined.  Therefore we allow it (unlike Solaris)
   and treat it as a no-op.  */

ਸਥਿਰ ਖਾਲੀ
validate_case_classes (ਬਣਤਰ Spec_list *s1, ਬਣਤਰ Spec_list *s2)
{
  size_t n_ਉੱਤਲਾ = 0;
  size_t n_ਹੇਠਲਾ = 0;
  ਪੂਰਨ c1 = 0;
  ਪੂਰਨ c2 = 0;
  MAYBE_ਅਣਵਰਤਿਆ ਗਿਣਤੀ_2 old_s1_len = s1->ਲੰਮਾਈ, old_s2_len = s2->ਲੰਮਾਈ;
  ਬਣਤਰ List_element *s1_ਪੂਛ = s1->ਪੂਛ;
  ਬਣਤਰ List_element *s2_ਪੂਛ = s2->ਪੂਛ;
  bool s1_new_element = true;
  bool s2_new_element = true;

  ਜੇ (complement || !s2->has_char_class)
    ਵਾਪਸ;

  ਲਈ (ਪੂਰਨ i = 0; i < N_CHARS; i++)
    {
      ਜੇ (isupper (i))
        n_ਉੱਤਲਾ++;
      ਜੇ (islower (i))
        n_ਹੇਠਲਾ++;
    }

  s1->state = BEGIN_STATE;
  s2->state = BEGIN_STATE;

  ਜਦੋਂ (c1 != -1 && c2 != -1)
    {
      ਗਿਣਤੀ ਉੱਤਲਾ_ਹੇਠਲਾ_class class_s1, class_s2;

      c1 = get_ਅਗਲਾ (s1, &class_s1);
      c2 = get_ਅਗਲਾ (s2, &class_s2);

      /* If c2 transitions to a new case class, then
         c1 must also transition at the same time.  */
      ਜੇ (s2_new_element && class_s2 != UL_NONE
          && !(s1_new_element && class_s1 != UL_NONE))
        error (EXIT_FAILURE, 0,
               _("misaligned [:upper:] and/or [:lower:] construct"));

      /* If case converting, quickly skip over the elements.  */
      ਜੇ (class_s2 != UL_NONE)
        {
          skip_construct (s1);
          skip_construct (s2);
          /* Discount insignificant/problematic lengths.  */
          s1->ਲੰਮਾਈ -= (class_s1 == UL_ਉੱਤਲਾ ? n_ਉੱਤਲਾ : n_ਹੇਠਲਾ) - 1;
          s2->ਲੰਮਾਈ -= (class_s2 == UL_ਉੱਤਲਾ ? n_ਉੱਤਲਾ : n_ਹੇਠਲਾ) - 1;
        }

      s1_new_element = s1->state == NEW_ELEMENT; /* Next element is new.  */
      s2_new_element = s2->state == NEW_ELEMENT; /* Next element is new.  */
    }

  affirm (old_s1_len >= s1->ਲੰਮਾਈ && old_s2_len >= s2->ਲੰਮਾਈ);

  s1->ਪੂਛ = s1_ਪੂਛ;
  s2->ਪੂਛ = s2_ਪੂਛ;
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

ਸਥਿਰ ਖਾਲੀ
get_spec_stats (ਬਣਤਰ Spec_list *s)
{
  ਗਿਣਤੀ_2 ਲੰਮਾਈ = 0;

  s->n_indefinite_repeats = 0;
  s->has_equiv_class = false;
  s->has_restricted_char_class = false;
  s->has_char_class = false;
  ਲਈ (ਬਣਤਰ List_element *p = s->ਸਿਰ->ਅਗਲਾ; p; p = p->ਅਗਲਾ)
    {
      ਗਿਣਤੀ_2 len = 0;

      ਚੋਣ (p->type)
        {
        ਹਾਲਤ RE_ਸਾਧਾਰਨ_CHAR:
          len = 1;
          ਰੋਕੋ;

        ਹਾਲਤ RE_RANGE:
          affirm (p->u.range.ਆਖਰੀ_char >= p->u.range.ਪਹਿਲਾ_char);
          len = p->u.range.ਆਖਰੀ_char - p->u.range.ਪਹਿਲਾ_char + 1;
          ਰੋਕੋ;

        ਹਾਲਤ RE_CHAR_CLASS:
          s->has_char_class = true;
          ਲਈ (ਪੂਰਨ i = 0; i < N_CHARS; i++)
            ਜੇ (is_char_class_member (p->u.char_class, i))
              ++len;
          ਚੋਣ (+p->u.char_class)
            {
            ਹਾਲਤ CC_ਉੱਤਲਾ:
            ਹਾਲਤ CC_ਹੇਠਲਾ:
              ਰੋਕੋ;
            ਮੂਲ:
              s->has_restricted_char_class = true;
              ਰੋਕੋ;
            }
          ਰੋਕੋ;

        ਹਾਲਤ RE_EQUIV_CLASS:
          ਲਈ (ਪੂਰਨ i = 0; i < N_CHARS; i++)
            ਜੇ (is_equiv_class_member (p->u.equiv_ਕੋਡ, i))
              ++len;
          s->has_equiv_class = true;
          ਰੋਕੋ;

        ਹਾਲਤ RE_REPEATED_CHAR:
          ਜੇ (p->u.repeated_char.repeat_ਗਿਣਤੀ > 0)
            len = p->u.repeated_char.repeat_ਗਿਣਤੀ;
          ਨਹੀਂਤਾਂ
            {
              s->indefinite_repeat_element = p;
              ++(s->n_indefinite_repeats);
            }
          ਰੋਕੋ;

        ਮੂਲ:
          unreachable ();
        }

      /* Check for arithmetic overflow in computing length.  Also, reject
         any length greater than the maximum repeat count, in case the
         length is later used to compute the repeat count for an
         indefinite element.  */
      ਜੇ (ckd_ਜੋੜੋ (&ਲੰਮਾਈ, ਲੰਮਾਈ, len) || REPEAT_ਗਿਣਤੀ_ਵੱਧ_ਤੋਂ_ਵੱਧ < ਲੰਮਾਈ)
        error (EXIT_FAILURE, 0, _("too many characters in set"));
    }

  s->ਲੰਮਾਈ = ਲੰਮਾਈ;
}

ਸਥਿਰ ਖਾਲੀ
get_s1_spec_stats (ਬਣਤਰ Spec_list *s1)
{
  get_spec_stats (s1);
  ਜੇ (complement)
    s1->ਲੰਮਾਈ = card_ਦਾ_complement (s1);
}

ਸਥਿਰ ਖਾਲੀ
get_s2_spec_stats (ਬਣਤਰ Spec_list *s2, ਗਿਣਤੀ_2 len_s1)
{
  get_spec_stats (s2);
  ਜੇ (len_s1 >= s2->ਲੰਮਾਈ && s2->n_indefinite_repeats == 1)
    {
      s2->indefinite_repeat_element->u.repeated_char.repeat_ਗਿਣਤੀ =
        len_s1 - s2->ਲੰਮਾਈ;
      s2->ਲੰਮਾਈ = len_s1;
    }
}

ਸਥਿਰ ਖਾਲੀ
spec_init (ਬਣਤਰ Spec_list *spec_list)
{
  ਬਣਤਰ List_element *new = xmalloc (ਆਕਾਰ *new);
  spec_list->ਸਿਰ = spec_list->ਪੂਛ = new;
  spec_list->ਸਿਰ->ਅਗਲਾ = ਨਿਰਾ;
}

/* This function makes two passes over the argument string S.  The first
   one converts all \c and \ddd escapes to their one-byte representations.
   The second constructs a linked specification list, SPEC_LIST, of the
   characters and constructs that comprise the argument string.  If either
   of these passes detects an error, this function returns false.  */

ਸਥਿਰ bool
parse_str (ਅੱਖਰ ਅਚਲ *s, ਬਣਤਰ Spec_list *spec_list)
{
  ਬਣਤਰ E_string es;
  bool ok = unquote (s, &es) && build_spec_list (&es, spec_list);
  es_free (&es);
  ਵਾਪਸ ok;
}

/* Given two specification lists, S1 and S2, and assuming that
   S1->length > S2->length, append a single [c*n] element to S2 where c
   is the last character in the expansion of S2 and n is the difference
   between the two lengths.
   Upon successful completion, S2->length is set to S1->length.  The only
   way this function can fail to make S2 as long as S1 is when S2 has
   zero-length, since in that case, there is no last character to repeat.
   So S2->length is required to be at least 1.  */

ਸਥਿਰ ਖਾਲੀ
string2_extend (ਅਚਲ ਬਣਤਰ Spec_list *s1, ਬਣਤਰ Spec_list *s2)
{
  ਬਣਤਰ List_element *p;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ char_ਮੰਜ਼ਿਲ_repeat;

  affirm (translating);
  affirm (s1->ਲੰਮਾਈ > s2->ਲੰਮਾਈ);
  affirm (s2->ਲੰਮਾਈ > 0);

  p = s2->ਪੂਛ;
  ਚੋਣ (p->type)
    {
    ਹਾਲਤ RE_ਸਾਧਾਰਨ_CHAR:
      char_ਮੰਜ਼ਿਲ_repeat = p->u.ਸਾਧਾਰਨ_char;
      ਰੋਕੋ;
    ਹਾਲਤ RE_RANGE:
      char_ਮੰਜ਼ਿਲ_repeat = p->u.range.ਆਖਰੀ_char;
      ਰੋਕੋ;
    ਹਾਲਤ RE_CHAR_CLASS:
      /* Note BSD allows extending of classes in string2.  For example:
           tr '[:upper:]0-9' '[:lower:]'
         That's not portable however, contradicts POSIX and is dependent
         on your collating sequence.  */
      error (EXIT_FAILURE, 0,
             _("when translating with string1 longer than string2,\n"
               "the latter string must not end with a character class"));

    ਹਾਲਤ RE_REPEATED_CHAR:
      char_ਮੰਜ਼ਿਲ_repeat = p->u.repeated_char.the_repeated_char;
      ਰੋਕੋ;

    ਹਾਲਤ RE_EQUIV_CLASS: ਮੂਲ:
      /* This shouldn't happen, because validate exits with an error
         if it finds an equiv class in string2 when translating.  */
      affirm (false);
    }

  append_repeated_char (s2, char_ਮੰਜ਼ਿਲ_repeat, s1->ਲੰਮਾਈ - s2->ਲੰਮਾਈ);
  s2->ਲੰਮਾਈ = s1->ਲੰਮਾਈ;
}

/* Return true if S is a non-empty list in which exactly one
   character (but potentially, many instances of it) appears.
   E.g., [X*] or xxxxxxxx.  */

ਸਥਿਰ bool
homogeneous_spec_list (ਬਣਤਰ Spec_list *s)
{
  ਪੂਰਨ b, c;

  s->state = BEGIN_STATE;

  ਜੇ ((b = get_ਅਗਲਾ (s, ਨਿਰਾ)) == -1)
    ਵਾਪਸ false;

  ਜਦੋਂ ((c = get_ਅਗਲਾ (s, ਨਿਰਾ)) != -1)
    ਜੇ (c != b)
      ਵਾਪਸ false;

  ਵਾਪਸ true;
}

/* Die with an error message if S1 and S2 describe strings that
   are not valid with the given command line switches.
   A side effect of this function is that if a valid [c*] or
   [c*0] construct appears in string2, it is converted to [c*n]
   with a value for n that makes s2->length == s1->length.  By
   the same token, if the --truncate-set1 option is not
   given, S2 may be extended.  */

ਸਥਿਰ ਖਾਲੀ
validate (ਬਣਤਰ Spec_list *s1, ਬਣਤਰ Spec_list *s2)
{
  get_s1_spec_stats (s1);
  ਜੇ (s1->n_indefinite_repeats > 0)
    error (EXIT_FAILURE, 0,
           _("the [c*] repeat construct may not appear in string1"));

  ਜੇ (s2)
    {
      get_s2_spec_stats (s2, s1->ਲੰਮਾਈ);

      ਜੇ (s2->n_indefinite_repeats > 1)
        error (EXIT_FAILURE, 0,
               _("only one [c*] repeat construct may appear in string2"));

      ਜੇ (translating)
        {
          ਜੇ (s2->has_equiv_class)
            error (EXIT_FAILURE, 0,
                   _("[=c=] expressions may not appear in string2"
                     " when translating"));

          ਜੇ (s2->has_restricted_char_class)
            error (EXIT_FAILURE, 0,
                   _("when translating, the only character classes"
                     " that may appear in\n"
                     "string2 are 'upper' and 'lower'"));

          validate_case_classes (s1, s2);

          ਜੇ (s1->ਲੰਮਾਈ > s2->ਲੰਮਾਈ)
            {
              ਜੇ (!truncate_set1)
                {
                  /* string2 must be non-empty unless --truncate-set1 is
                     given or string1 is empty.  */

                  ਜੇ (s2->ਲੰਮਾਈ == 0)
                    error (EXIT_FAILURE, 0,
                           _("when not truncating set1,"
                             " string2 must be non-empty"));
                  string2_extend (s1, s2);
                }
            }

          ਜੇ (complement && s1->has_char_class
              && ! (s2->ਲੰਮਾਈ == s1->ਲੰਮਾਈ && homogeneous_spec_list (s2)))
            error (EXIT_FAILURE, 0,
                   _("when translating with complemented character classes,\n"
                     "string2 must map all characters in the domain to one"));
        }
      ਨਹੀਂਤਾਂ
        /* Not translating.  */
        {
          ਜੇ (s2->n_indefinite_repeats > 0)
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

ਸਥਿਰ ਖਾਲੀ
squeeze_filter (ਅੱਖਰ *buf, size_t ਮਾਪ, size_t (*reader) (ਅੱਖਰ *, size_t))
{
  /* A value distinct from any character that may have been stored in a
     buffer as the result of a block-read in the function squeeze_filter.  */
  ਅਚਲ ਪੂਰਨ NOT_A_CHAR = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;

  ਪੂਰਨ char_ਮੰਜ਼ਿਲ_squeeze = NOT_A_CHAR;
  size_t i = 0;
  size_t nr = 0;

  ਜਦੋਂ (true)
    {
      ਜੇ (i >= nr)
        {
          nr = reader (buf, ਮਾਪ);
          ਜੇ (nr == 0)
            ਰੋਕੋ;
          i = 0;
        }

      size_t begin = i;

      ਜੇ (char_ਮੰਜ਼ਿਲ_squeeze == NOT_A_CHAR)
        {
          size_t ਬਾਹਰਲਾ_len;
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
          ਲਈ (; i < nr && !ਵਿੱਚ_squeeze_ਸੈੱਟ[ਮੰਜ਼ਿਲ_uchar (buf[i])]; i += 2)
            ਜਾਰੀ;

          /* There is a special case when i == nr and we've just
             skipped a character (the last one in buf) that is in
             the squeeze set.  */
          ਜੇ (i == nr && ਵਿੱਚ_squeeze_ਸੈੱਟ[ਮੰਜ਼ਿਲ_uchar (buf[i - 1])])
            --i;

          ਜੇ (i >= nr)
            ਬਾਹਰਲਾ_len = nr - begin;
          ਨਹੀਂਤਾਂ
            {
              char_ਮੰਜ਼ਿਲ_squeeze = buf[i];
              /* We're about to output buf[begin..i].  */
              ਬਾਹਰਲਾ_len = i - begin + 1;

              /* But since we stepped by 2 in the loop above,
                 out_len may be one too large.  */
              ਜੇ (i > 0 && buf[i - 1] == char_ਮੰਜ਼ਿਲ_squeeze)
                --ਬਾਹਰਲਾ_len;

              /* Advance i to the index of first character to be
                 considered when looking for a char different from
                 char_to_squeeze.  */
              ++i;
            }
          ਜੇ (ਬਾਹਰਲਾ_len > 0
              && fwrite (&buf[begin], 1, ਬਾਹਰਲਾ_len, stdout) != ਬਾਹਰਲਾ_len)
            write_error ();
        }

      ਜੇ (char_ਮੰਜ਼ਿਲ_squeeze != NOT_A_CHAR)
        {
          /* Advance i to index of first char != char_to_squeeze
             (or to nr if all the rest of the characters in this
             buffer are the same as char_to_squeeze).  */
          ਲਈ (; i < nr && buf[i] == char_ਮੰਜ਼ਿਲ_squeeze; i++)
            ਜਾਰੀ;
          ਜੇ (i < nr)
            char_ਮੰਜ਼ਿਲ_squeeze = NOT_A_CHAR;
          /* If (i >= nr) we've squeezed the last character in this buffer.
             So now we have to read a new buffer and continue comparing
             characters against char_to_squeeze.  */
        }
    }
}

ਸਥਿਰ size_t
plain_read (ਅੱਖਰ *buf, size_t ਮਾਪ)
{
  ssize_t nr = ਪਠ (STDIN_FILENO, buf, ਮਾਪ);
  ਜੇ (nr < 0)
    error (EXIT_FAILURE, errno, _("read error"));
  ਵਾਪਸ nr;
}

/* Read buffers of SIZE bytes from stdin until one is found that
   contains at least one character not in the delete set.  Store
   in the array BUF, all characters from that buffer that are not
   in the delete set, and return the number of characters saved
   or 0 upon EOF.  */

ਸਥਿਰ size_t
read_and_delete (ਅੱਖਰ *buf, size_t ਮਾਪ)
{
  size_t n_saved;

  /* This enclosing do-while loop is to make sure that
     we don't return zero (indicating EOF) when we've
     just deleted all the characters in a buffer.  */
  ਕਰੋ
    {
      size_t nr = plain_read (buf, ਮਾਪ);

      ਜੇ (nr == 0)
        ਵਾਪਸ 0;

      /* This first loop may be a waste of code, but gives much
         better performance when no characters are deleted in
         the beginning of a buffer.  It just avoids the copying
         of buf[i] into buf[n_saved] when it would be a NOP.  */

      size_t i;
      ਲਈ (i = 0; i < nr && !ਵਿੱਚ_delete_ਸੈੱਟ[ਮੰਜ਼ਿਲ_uchar (buf[i])]; i++)
        ਜਾਰੀ;
      n_saved = i;

      ਲਈ (++i; i < nr; i++)
        ਜੇ (!ਵਿੱਚ_delete_ਸੈੱਟ[ਮੰਜ਼ਿਲ_uchar (buf[i])])
          buf[n_saved++] = buf[i];
    }
  ਜਦੋਂ (n_saved == 0);

  ਵਾਪਸ n_saved;
}

/* Read at most SIZE bytes from stdin into the array BUF.  Then
   perform the in-place and one-to-one mapping specified by the global
   array 'xlate'.  Return the number of characters read, or 0 upon EOF.  */

ਸਥਿਰ size_t
read_and_xlate (ਅੱਖਰ *buf, size_t ਮਾਪ)
{
  size_t ਬਾਈਟਾਂ_read = plain_read (buf, ਮਾਪ);

  ਲਈ (size_t i = 0; i < ਬਾਈਟਾਂ_read; i++)
    buf[i] = xlate[ਮੰਜ਼ਿਲ_uchar (buf[i])];

  ਵਾਪਸ ਬਾਈਟਾਂ_read;
}

/* Initialize a boolean membership set, IN_SET, with the character
   values obtained by traversing the linked list of constructs S
   using the function 'get_next'.  IN_SET is expected to have been
   initialized to all zeros by the caller.  If COMPLEMENT_THIS_SET
   is true the resulting set is complemented.  */

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_initialize (ਬਣਤਰ Spec_list *s, bool complement_this_ਸੈੱਟ, bool *ਵਿੱਚ_ਸੈੱਟ)
{
  ਪੂਰਨ c;

  s->state = BEGIN_STATE;
  ਜਦੋਂ ((c = get_ਅਗਲਾ (s, ਨਿਰਾ)) != -1)
    ਵਿੱਚ_ਸੈੱਟ[c] = true;
  ਜੇ (complement_this_ਸੈੱਟ)
    ਲਈ (size_t i = 0; i < N_CHARS; i++)
      ਵਿੱਚ_ਸੈੱਟ[i] = (!ਵਿੱਚ_ਸੈੱਟ[i]);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ c;
  ਪੂਰਨ non_option_ਦਲੀਲਾਂ;
  ਪੂਰਨ min_operands;
  ਪੂਰਨ max_operands;
  ਬਣਤਰ Spec_list buf1, buf2;
  ਬਣਤਰ Spec_list *s1 = &buf1;
  ਬਣਤਰ Spec_list *s2 = &buf2;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((c = getopt_long (argc, argv, "+AcCdstu", long_options, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'A':
          /* Undocumented option, for compatibility with AIX.  */
          setlocale (LC_COLLATE, "C");
          setlocale (LC_CTYPE, "C");
          ਰੋਕੋ;

        ਹਾਲਤ 'c':
        ਹਾਲਤ 'C':
          complement = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'd':
          delete = true;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          squeeze_repeats = true;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          truncate_set1 = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'u':
          /* Undocumented option, for compatibility with FreeBSD/macOS.  */
          setvbuf (stdout, ਨਿਰਾ, _IONBF, 0);
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
          ਰੋਕੋ;
        }
    }

  non_option_ਦਲੀਲਾਂ = argc - optind;
  translating = (non_option_ਦਲੀਲਾਂ == 2 && !delete);
  min_operands = 1 + (delete == squeeze_repeats);
  max_operands = 1 + (delete <= squeeze_repeats);

  ਜੇ (non_option_ਦਲੀਲਾਂ < min_operands)
    {
      ਜੇ (non_option_ਦਲੀਲਾਂ == 0)
        error (0, 0, _("missing operand"));
      ਨਹੀਂਤਾਂ
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

  ਜੇ (max_operands < non_option_ਦਲੀਲਾਂ)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + max_operands]));
      ਜੇ (non_option_ਦਲੀਲਾਂ == 2)
        fprintf (stderr, "%s\n",
                 _("Only one string may be given when "
                   "deleting without squeezing repeats."));
      usage (EXIT_FAILURE);
    }

  spec_init (s1);
  ਜੇ (!parse_str (argv[optind], s1))
    main_exit (EXIT_FAILURE);

  ਜੇ (non_option_ਦਲੀਲਾਂ == 2)
    {
      spec_init (s2);
      ਜੇ (!parse_str (argv[optind + 1], s2))
        main_exit (EXIT_FAILURE);
    }
  ਨਹੀਂਤਾਂ
    s2 = ਨਿਰਾ;

  validate (s1, s2);

  /* Use binary I/O, since 'tr' is sometimes used to transliterate
     non-printable characters, or characters which are stripped away
     by text-mode reads (like CR and ^Z).  */
  xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDIN_FILENO, O_ਦੁਆਧਾਰੀ);
  xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDOUT_FILENO, O_ਦੁਆਧਾਰੀ);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  ਜੇ (squeeze_repeats && non_option_ਦਲੀਲਾਂ == 1)
    {
      ਸੈੱਟ_initialize (s1, complement, ਵਿੱਚ_squeeze_ਸੈੱਟ);
      squeeze_filter (io_buf, ਆਕਾਰ io_buf, plain_read);
    }
  ਨਹੀਂਤਾਂ ਜੇ (delete && non_option_ਦਲੀਲਾਂ == 1)
    {
      ਸੈੱਟ_initialize (s1, complement, ਵਿੱਚ_delete_ਸੈੱਟ);

      ਜਦੋਂ (true)
        {
          size_t nr = read_and_delete (io_buf, ਆਕਾਰ io_buf);
          ਜੇ (nr == 0)
            ਰੋਕੋ;
          ਜੇ (fwrite (io_buf, 1, nr, stdout) != nr)
            write_error ();
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (squeeze_repeats && delete && non_option_ਦਲੀਲਾਂ == 2)
    {
      ਸੈੱਟ_initialize (s1, complement, ਵਿੱਚ_delete_ਸੈੱਟ);
      ਸੈੱਟ_initialize (s2, false, ਵਿੱਚ_squeeze_ਸੈੱਟ);
      squeeze_filter (io_buf, ਆਕਾਰ io_buf, read_and_delete);
    }
  ਨਹੀਂਤਾਂ ਜੇ (translating)
    {
      ਜੇ (complement)
        {
          bool *ਵਿੱਚ_s1 = ਵਿੱਚ_delete_ਸੈੱਟ;

          ਸੈੱਟ_initialize (s1, false, ਵਿੱਚ_s1);
          s2->state = BEGIN_STATE;
          ਲਈ (ਪੂਰਨ i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          ਲਈ (ਪੂਰਨ i = 0; i < N_CHARS; i++)
            {
              ਜੇ (!ਵਿੱਚ_s1[i])
                {
                  ਪੂਰਨ ch = get_ਅਗਲਾ (s2, ਨਿਰਾ);
                  affirm (ch != -1 || truncate_set1);
                  ਜੇ (ch == -1)
                    {
                      /* This will happen when tr is invoked like e.g.
                         tr -cs A-Za-z0-9 '\012'.  */
                      ਰੋਕੋ;
                    }
                  xlate[i] = ch;
                }
            }
        }
      ਨਹੀਂਤਾਂ
        {
          ਪੂਰਨ c1, c2;
          ਗਿਣਤੀ ਉੱਤਲਾ_ਹੇਠਲਾ_class class_s1;
          ਗਿਣਤੀ ਉੱਤਲਾ_ਹੇਠਲਾ_class class_s2;

          ਲਈ (ਪੂਰਨ i = 0; i < N_CHARS; i++)
            xlate[i] = i;
          s1->state = BEGIN_STATE;
          s2->state = BEGIN_STATE;
          ਜਦੋਂ (true)
            {
              c1 = get_ਅਗਲਾ (s1, &class_s1);
              c2 = get_ਅਗਲਾ (s2, &class_s2);

              ਜੇ (class_s1 == UL_ਹੇਠਲਾ && class_s2 == UL_ਉੱਤਲਾ)
                {
                  ਲਈ (ਪੂਰਨ i = 0; i < N_CHARS; i++)
                    ਜੇ (islower (i))
                      xlate[i] = ਵੱਡਾ_ਅੱਖਰ (i);
                }
              ਨਹੀਂਤਾਂ ਜੇ (class_s1 == UL_ਉੱਤਲਾ && class_s2 == UL_ਹੇਠਲਾ)
                {
                  ਲਈ (ਪੂਰਨ i = 0; i < N_CHARS; i++)
                    ਜੇ (isupper (i))
                      xlate[i] = tolower (i);
                }
              ਨਹੀਂਤਾਂ
                {
                  /* The following should have been checked by validate...  */
                  ਜੇ (c1 == -1 || c2 == -1)
                    ਰੋਕੋ;
                  xlate[c1] = c2;
                }

              /* When case-converting, skip the elements as an optimization.  */
              ਜੇ (class_s2 != UL_NONE)
                {
                  skip_construct (s1);
                  skip_construct (s2);
                }
            }
          affirm (c1 == -1 || truncate_set1);
        }
      ਜੇ (squeeze_repeats)
        {
          ਸੈੱਟ_initialize (s2, false, ਵਿੱਚ_squeeze_ਸੈੱਟ);
          squeeze_filter (io_buf, ਆਕਾਰ io_buf, read_and_xlate);
        }
      ਨਹੀਂਤਾਂ
        {
          ਜਦੋਂ (true)
            {
              size_t ਬਾਈਟਾਂ_read = read_and_xlate (io_buf, ਆਕਾਰ io_buf);
              ਜੇ (ਬਾਈਟਾਂ_read == 0)
                ਰੋਕੋ;
              ਜੇ (fwrite (io_buf, 1, ਬਾਈਟਾਂ_read, stdout) != ਬਾਈਟਾਂ_read)
                write_error ();
            }
        }
    }

  ਜੇ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  main_exit (EXIT_SUCCESS);
}
