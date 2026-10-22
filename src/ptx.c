/* Permuted index for GNU, with keywords in their context.
   Copyright (C) 1990-2026 Free Software Foundation, Inc.
   François Pinard <pinard@iro.umontreal.ca>, 1988.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

   François Pinard <pinard@iro.umontreal.ca> */

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <regex.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "octhexdigits.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdio--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHAR_ਸੈੱਟ_ਮਾਪ 256

/* Debugging the memory allocator.  */

#ਜੇ WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#ਜੇ_ਅੰਤ

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

ਗਿਣਤੀ Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

ਸਥਿਰ bool gnu_extensions = true;	/* trigger all GNU extensions */
ਸਥਿਰ bool auto_reference = false;	/* refs are 'file_name:line_number:' */
ਸਥਿਰ bool ਨਿਵੇਸ਼_reference = false;	/* refs at beginning of input lines */
ਸਥਿਰ bool ਸੱਜਾ_reference = false;	/* output refs after right context  */
ਸਥਿਰ idx_t ਸਤਰ_ਚੌੜਾਈ = -1;		/* output line width in characters */
ਸਥਿਰ idx_t gap_ਮਾਪ = 3;	/* number of spaces between output fields */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *truncation_string = "/";
                                /* string used to mark line truncations */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *macro_name = "xx";	/* macro name for roff or TeX output */
ਸਥਿਰ ਗਿਣਤੀ Format output_format = UNKNOWN_FORMAT;
                                /* output format */

ਸਥਿਰ bool ignore_case = false;	/* fold lower to upper for sorting */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *break_file = ਨਿਰਾ; /* name of the 'Break chars' file */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *only_file = ਨਿਰਾ;	/* name of the 'Only words' file */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ignore_file = ਨਿਰਾ; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
ਬਣਤਰ regex_ਅੰਕੜੇ
{
  /* The original regular expression, as a string.  */
  ਅੱਖਰ ਅਚਲ *string;

  /* The compiled regular expression, and its fastmap.  */
  ਬਣਤਰ re_ਨਮੂਨਾ_ਬਫਰ ਨਮੂਨਾ_2;
  ਅੱਖਰ fastmap[UCHAR_MAX + 1];
};

ਸਥਿਰ ਬਣਤਰ regex_ਅੰਕੜੇ context_regex;	/* end of context */
ਸਥਿਰ ਬਣਤਰ regex_ਅੰਕੜੇ ਸ਼ਬਦ_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
  {
    ਅੱਖਰ *ਸ਼ੁਰੂ;		/* pointer to beginning of region */
    ਅੱਖਰ *ਸਿਰਾ;			/* pointer to end + 1 of region */
  }
ਖੰਡ_2;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
  {
    ਅੱਖਰ *ਸ਼ੁਰੂ;		/* pointer to beginning of region */
    idx_t ਮਾਪ;			/* length of the region */
  }
ਸ਼ਬਦ;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
  {
    idx_t alloc;		/* allocated length */
    idx_t ਲੰਮਾਈ;		/* number of used entries */
    ਸ਼ਬਦ *ਸ਼ੁਰੂ COUNTED_BY (ਲੰਮਾਈ);	/* array of WORDs */
  }
ਸ਼ਬਦ_ਸਾਰਣੀ;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ folded_chars[CHAR_ਸੈੱਟ_ਮਾਪ];

/* End of context pattern register indices.  */
ਸਥਿਰ ਬਣਤਰ re_registers context_regs;

/* Keyword pattern register indices.  */
ਸਥਿਰ ਬਣਤਰ re_registers ਸ਼ਬਦ_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
ਸਥਿਰ ਅੱਖਰ ਸ਼ਬਦ_fastmap[CHAR_ਸੈੱਟ_ਮਾਪ];

/* Maximum length of any word read.  */
ਸਥਿਰ idx_t ਵੱਧ_ਤੋਂ_ਵੱਧ_ਸ਼ਬਦ_ਲੰਮਾਈ;

/* Maximum width of any reference used.  */
ਸਥਿਰ idx_t reference_max_ਚੌੜਾਈ;

/* Ignore and Only word tables.  */

ਸਥਿਰ ਸ਼ਬਦ_ਸਾਰਣੀ ignore_ਸਾਰਣੀ;	/* table of words to ignore */
ਸਥਿਰ ਸ਼ਬਦ_ਸਾਰਣੀ only_ਸਾਰਣੀ;		/* table of words to select */

/* Source text table, and scanning macros.  */

ਸਥਿਰ ਪੂਰਨ ਸੰਖਿਆ_ਨਿਵੇਸ਼_files;	/* number of text input files */
ਸਥਿਰ intmax_t ਕੁੱਲ_ਸਤਰ_ਗਿਣਤੀ;	/* total number of lines seen so far */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ **ਨਿਵੇਸ਼_file_name;	/* array of text input file names */
ਸਥਿਰ intmax_t *file_ਸਤਰ_ਗਿਣਤੀ;	/* array of line count values at end */

ਸਥਿਰ ਖੰਡ_2 *ਪਾਠ_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SKIP_NON_WHITE(cursor, ਹੱਦਬੰਦੀ) \
  ਜਦੋਂ (cursor < ਹੱਦਬੰਦੀ && ! ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*cursor)))		\
    cursor++

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SKIP_WHITE(cursor, ਹੱਦਬੰਦੀ) \
  ਜਦੋਂ (cursor < ਹੱਦਬੰਦੀ && ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*cursor)))		\
    cursor++

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SKIP_WHITE_BACKWARDS(cursor, ਸ਼ੁਰੂ) \
  ਜਦੋਂ (cursor > ਸ਼ੁਰੂ && ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SKIP_SOMETHING(cursor, ਹੱਦਬੰਦੀ) \
  ਜੇ (ਸ਼ਬਦ_regex.string)						\
    {									\
      regoff_t ਗਿਣਤੀ_2;							\
      ਗਿਣਤੀ_2 = re_ਮੇਲ (&ਸ਼ਬਦ_regex.ਨਮੂਨਾ_2, cursor, ਹੱਦਬੰਦੀ - cursor,	\
                        0, ਨਿਰਾ);					\
      ਜੇ (ਗਿਣਤੀ_2 == -2)							\
        matcher_error ();						\
      cursor += ਗਿਣਤੀ_2 <= 0 ? 1 : ਗਿਣਤੀ_2;					\
    }									\
  ਨਹੀਂਤਾਂ ਜੇ (ਸ਼ਬਦ_fastmap[ਮੰਜ਼ਿਲ_uchar (*cursor)])				\
    ਜਦੋਂ (cursor < ਹੱਦਬੰਦੀ && ਸ਼ਬਦ_fastmap[ਮੰਜ਼ਿਲ_uchar (*cursor)])		\
      cursor++;								\
  ਨਹੀਂਤਾਂ									\
    cursor++

/* Occurrences table.

   The 'keyword' pointer provides the central word, which is surrounded
   by a left context and a right context.  The 'keyword' and 'length'
   field allow full 8-bit characters keys, even including NULs.  At other
   places in this program, the name 'keyafter' refers to the keyword
   followed by its right context.

   The left context does not extend, towards the beginning of the file,
   further than a distance given by the 'left' value.  This value is
   relative to the keyword beginning, it is usually negative.  This
   insures that, except for white space, we will never have to backward
   scan the source text, when it is time to generate the final output
   lines.

   The right context, indirectly attainable through the keyword end, does
   not extend, towards the end of the file, further than a distance given
   by the 'right' value.  This value is relative to the keyword
   beginning, it is usually positive.

   When automatic references are used, the 'reference' value is the
   overall line number in all input files read so far, in this case, it
   is of type intmax_t.  When input references are used, the 'reference'
   value indicates the distance between the keyword beginning and the
   start of the reference field, and it fits in ptrdiff_t and is usually
   negative.  */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
  {
    ਸ਼ਬਦ ਕੁੰਜੀ;			/* description of the keyword */
    ptrdiff_t ਖੱਬਾ;		/* distance to left context start */
    ptrdiff_t ਸੱਜਾ;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    ਪੂਰਨ file_ਸੂਚਕਾਂਕ;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

ਸਥਿਰ OCCURS *occurs_ਸਾਰਣੀ[1];	/* all words retained from the read text */
ਸਥਿਰ idx_t occurs_alloc[1];	/* allocated size of occurs_table */
ਸਥਿਰ idx_t ਸੰਖਿਆ_ਦਾ_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
ਸਥਿਰ ਅੱਖਰ edited_flag[CHAR_ਸੈੱਟ_ਮਾਪ];

/* Half of line width, reference excluded.  */
ਸਥਿਰ idx_t half_ਸਤਰ_ਚੌੜਾਈ;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ਸਥਿਰ ptrdiff_t before_max_ਚੌੜਾਈ;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ਸਥਿਰ ptrdiff_t keyafter_max_ਚੌੜਾਈ;

/* Length of string that flags truncation.  */
ਸਥਿਰ idx_t truncation_string_ਲੰਮਾਈ;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

ਸਥਿਰ ਖੰਡ_2 ਪੂਛ;		/* tail field */
ਸਥਿਰ bool ਪੂਛ_truncation;	/* flag truncation after the tail field */

ਸਥਿਰ ਖੰਡ_2 before;		/* before field */
ਸਥਿਰ bool before_truncation;	/* flag truncation before the before field */

ਸਥਿਰ ਖੰਡ_2 keyafter;		/* keyword-and-after field */
ਸਥਿਰ bool keyafter_truncation; /* flag truncation after the keyafter field */

ਸਥਿਰ ਖੰਡ_2 ਸਿਰ;		/* head field */
ਸਥਿਰ bool ਸਿਰ_truncation;	/* flag truncation before the head field */

ਸਥਿਰ ਖੰਡ_2 reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

ਸਥਿਰ _Noreturn ਖਾਲੀ
matcher_error (ਖਾਲੀ)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

ਸਥਿਰ ਖਾਲੀ
unescape_string (ਅੱਖਰ *string)
{
  ਅੱਖਰ *cursor;			/* cursor in result */
  ਪੂਰਨ ਮੁੱਲ;			/* value of \nnn escape */
  ਪੂਰਨ ਲੰਮਾਈ;			/* length of \nnn escape */

  cursor = string;

  ਜਦੋਂ (*string)
    {
      ਜੇ (*string == '\\')
        {
          string++;
          ਚੋਣ (*string)
            {
            ਹਾਲਤ 'x':		/* \xhhh escape, 3 chars maximum */
              ਮੁੱਲ = 0;
              ਲਈ (ਲੰਮਾਈ = 0, string++;
                   ਲੰਮਾਈ < 3 && c_isxdigit (*string);
                   ਲੰਮਾਈ++, string++)
                ਮੁੱਲ = ਮੁੱਲ * 16 + fromhex (*string);
              ਜੇ (ਲੰਮਾਈ == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              ਨਹੀਂਤਾਂ
                *cursor++ = ਮੁੱਲ;
              ਰੋਕੋ;

            ਹਾਲਤ '0':		/* \0ooo escape, 3 chars maximum */
              ਮੁੱਲ = 0;
              ਲਈ (ਲੰਮਾਈ = 0, string++;
                   ਲੰਮਾਈ < 3 && isoct (*string);
                   ਲੰਮਾਈ++, string++)
                ਮੁੱਲ = ਮੁੱਲ * 8 + fromoct (*string);
              *cursor++ = ਮੁੱਲ;
              ਰੋਕੋ;

            ਹਾਲਤ 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              ਰੋਕੋ;

            ਹਾਲਤ 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              ਰੋਕੋ;

            ਹਾਲਤ 'c':		/* cancel the rest of the output */
              ਜਦੋਂ (*string)
                string++;
              ਰੋਕੋ;

            ਹਾਲਤ 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              ਰੋਕੋ;

            ਹਾਲਤ 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              ਰੋਕੋ;

            ਹਾਲਤ 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              ਰੋਕੋ;

            ਹਾਲਤ 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              ਰੋਕੋ;

            ਹਾਲਤ 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              ਰੋਕੋ;

            ਹਾਲਤ '\0':		/* lone backslash at end of string */
              /* ignore it */
              ਰੋਕੋ;

            ਮੂਲ:
              *cursor++ = '\\';
              *cursor++ = *string++;
              ਰੋਕੋ;
            }
        }
      ਨਹੀਂਤਾਂ
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
compile_regex (ਬਣਤਰ regex_ਅੰਕੜੇ *regex)
{
  ਬਣਤਰ re_ਨਮੂਨਾ_ਬਫਰ *ਨਮੂਨਾ_2 = &regex->ਨਮੂਨਾ_2;
  ਅੱਖਰ ਅਚਲ *string = regex->string;
  ਅੱਖਰ ਅਚਲ *ਸੁਨੇਹਾ_2;

  ਨਮੂਨਾ_2->ਬਫਰ = ਨਿਰਾ;
  ਨਮੂਨਾ_2->allocated = 0;
  ਨਮੂਨਾ_2->fastmap = regex->fastmap;
  ਨਮੂਨਾ_2->translate = ignore_case ? folded_chars : ਨਿਰਾ;

  ਸੁਨੇਹਾ_2 = re_compile_ਨਮੂਨਾ (string, ਲੜੀ_ਲੰਬਾਈ (string), ਨਮੂਨਾ_2);
  ਜੇ (ਸੁਨੇਹਾ_2)
    error (EXIT_FAILURE, 0, _("%s (for regexp %s)"), ਸੁਨੇਹਾ_2, quote (string));

  /* The fastmap should be compiled before 're_match'.  The following
     call is not mandatory, because 're_search' is always called sooner,
     and it compiles the fastmap if this has not been done yet.  */

  re_compile_fastmap (ਨਮੂਨਾ_2);
}

/*------------------------------------------------------------------------.
| This will initialize various tables for pattern match and compiles some |
| regexps.								  |
`------------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
initialize_regex (ਖਾਲੀ)
{
  /* Initialize the case folding table.  */

  ਜੇ (ignore_case)
    ਲਈ (ਪੂਰਨ character = 0; character < CHAR_ਸੈੱਟ_ਮਾਪ; character++)
      folded_chars[character] = ਵੱਡਾ_ਅੱਖਰ (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  ਜੇ (context_regex.string)
    {
      ਜੇ (!*context_regex.string)
        context_regex.string = ਨਿਰਾ;
    }
  ਨਹੀਂਤਾਂ ਜੇ (gnu_extensions && !ਨਿਵੇਸ਼_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  ਨਹੀਂਤਾਂ
    context_regex.string = "\n";

  ਜੇ (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  ਜੇ (ਸ਼ਬਦ_regex.string)
    compile_regex (&ਸ਼ਬਦ_regex);
  ਨਹੀਂਤਾਂ ਜੇ (!break_file)
    {
      ਜੇ (gnu_extensions)
        {

          /* Simulate \w+.  */

          ਲਈ (ਪੂਰਨ character = 0; character < CHAR_ਸੈੱਟ_ਮਾਪ; character++)
            ਸ਼ਬਦ_fastmap[character] = !! ਅੱਖਰ_ਹੈ (character);
        }
      ਨਹੀਂਤਾਂ
        {

          /* Simulate [^ \t\n]+.  */

          memset (ਸ਼ਬਦ_fastmap, 1, CHAR_ਸੈੱਟ_ਮਾਪ);
          ਸ਼ਬਦ_fastmap[' '] = 0;
          ਸ਼ਬਦ_fastmap['\t'] = 0;
          ਸ਼ਬਦ_fastmap['\n'] = 0;
        }
    }
}

/*------------------------------------------------------------------------.
| This routine will attempt to swallow a whole file name FILE_NAME into a |
| contiguous region of memory and return a description of it into BLOCK.  |
| Standard input is assumed whenever FILE_NAME is null, empty or "-".	  |
|									  |
| Previously, in some cases, white space compression was attempted while  |
| inputting text.  This was defeating some regexps like default end of	  |
| sentence, which checks for two consecutive spaces.  If white space	  |
| compression is ever reinstated, it should be in output routines.	  |
`------------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
swallow_file_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ (ਅੱਖਰ ਅਚਲ *file_name, ਖੰਡ_2 *ਖੰਡ)
{
  size_t ਵਰਤਿਆ_ਲੰਮਾਈ;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  ਜੇ (using_stdin)
    ਖੰਡ->ਸ਼ੁਰੂ = fread_file (stdin, 0, &ਵਰਤਿਆ_ਲੰਮਾਈ);
  ਨਹੀਂਤਾਂ
    ਖੰਡ->ਸ਼ੁਰੂ = read_file (file_name, 0, &ਵਰਤਿਆ_ਲੰਮਾਈ);

  ਜੇ (!ਖੰਡ->ਸ਼ੁਰੂ)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  ਜੇ (using_stdin)
    clearerr (stdin);

  ਖੰਡ->ਸਿਰਾ = ਖੰਡ->ਸ਼ੁਰੂ + ਵਰਤਿਆ_ਲੰਮਾਈ;
}

/* Sort and search routines.  */

/*--------------------------------------------------------------------------.
| Compare two words, FIRST and SECOND, and return 0 if they are identical.  |
| Return less than 0 if the first word goes before the second; return	    |
| greater than 0 if the first word goes after the second.		    |
|									    |
| If a word is indeed a prefix of the other, the shorter should go first.   |
`--------------------------------------------------------------------------*/

ਸਥਿਰ ਪੂਰਨ
compare_ਸ਼ਬਦ (ਅਚਲ ਖਾਲੀ *void_ਪਹਿਲਾ, ਅਚਲ ਖਾਲੀ *void_ਦੂਜਾ)
{
  ਸ਼ਬਦ ਅਚਲ *ਪਹਿਲਾ = void_ਪਹਿਲਾ;
  ਸ਼ਬਦ ਅਚਲ *ਦੂਜਾ = void_ਦੂਜਾ;
  idx_t ਲੰਮਾਈ = MIN (ਪਹਿਲਾ->ਮਾਪ, ਦੂਜਾ->ਮਾਪ);

  ਜੇ (ignore_case)
    {
      ਲਈ (idx_t counter = 0; counter < ਲੰਮਾਈ; counter++)
        {
          ਪੂਰਨ ਮੁੱਲ = (folded_chars[ਮੰਜ਼ਿਲ_uchar (ਪਹਿਲਾ->ਸ਼ੁਰੂ[counter])]
                       - folded_chars[ਮੰਜ਼ਿਲ_uchar (ਦੂਜਾ->ਸ਼ੁਰੂ[counter])]);
          ਜੇ (ਮੁੱਲ != 0)
            ਵਾਪਸ ਮੁੱਲ;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਲਈ (idx_t counter = 0; counter < ਲੰਮਾਈ; counter++)
        {
          ਪੂਰਨ ਮੁੱਲ = (ਮੰਜ਼ਿਲ_uchar (ਪਹਿਲਾ->ਸ਼ੁਰੂ[counter])
                       - ਮੰਜ਼ਿਲ_uchar (ਦੂਜਾ->ਸ਼ੁਰੂ[counter]));
          ਜੇ (ਮੁੱਲ != 0)
            ਵਾਪਸ ਮੁੱਲ;
        }
    }

  ਵਾਪਸ _GL_CMP (ਪਹਿਲਾ->ਮਾਪ, ਦੂਜਾ->ਮਾਪ);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

ਸਥਿਰ ਪੂਰਨ
compare_occurs (ਅਚਲ ਖਾਲੀ *void_ਪਹਿਲਾ, ਅਚਲ ਖਾਲੀ *void_ਦੂਜਾ)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪਹਿਲਾ ((ਅਚਲ OCCURS *) void_ਪਹਿਲਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਦੂਜਾ ((ਅਚਲ OCCURS *) void_ਦੂਜਾ)
  ਪੂਰਨ ਮੁੱਲ;

  ਮੁੱਲ = compare_ਸ਼ਬਦ (&ਪਹਿਲਾ->ਕੁੰਜੀ, &ਦੂਜਾ->ਕੁੰਜੀ);
  ਵਾਪਸ (ਮੁੱਲ ? ਮੁੱਲ
          : _GL_CMP (ਪਹਿਲਾ->ਕੁੰਜੀ.ਸ਼ੁਰੂ, ਦੂਜਾ->ਕੁੰਜੀ.ਸ਼ੁਰੂ));
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਪਹਿਲਾ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਦੂਜਾ
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
ਖੋਜ_ਸਾਰਣੀ (ਸ਼ਬਦ *ਸ਼ਬਦ_2, ਸ਼ਬਦ_ਸਾਰਣੀ *ਸਾਰਣੀ)
{
  idx_t lo = 0;
  idx_t hi = ਸਾਰਣੀ->ਲੰਮਾਈ;
  ਜਦੋਂ (lo < hi)
    {
      idx_t ਵਿਚਕਾਰ = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      ਪੂਰਨ ਮੁੱਲ = compare_ਸ਼ਬਦ (ਸ਼ਬਦ_2, ਸਾਰਣੀ->ਸ਼ੁਰੂ + ਵਿਚਕਾਰ);
      ਜੇ (ਮੁੱਲ < 0)
        hi = ਵਿਚਕਾਰ;
      ਨਹੀਂਤਾਂ ਜੇ (ਮੁੱਲ > 0)
        lo = ਵਿਚਕਾਰ + 1;
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ true;
    }
  ਵਾਪਸ false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
ਛਾਂਟੋ_ਮਿਲਿਆ_occurs (ਖਾਲੀ)
{

  /* Only one language for the time being.  */
  ਜੇ (ਸੰਖਿਆ_ਦਾ_occurs[0])
    qsort (occurs_ਸਾਰਣੀ[0], ਸੰਖਿਆ_ਦਾ_occurs[0], ਆਕਾਰ **occurs_ਸਾਰਣੀ,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
digest_break_file (ਅੱਖਰ ਅਚਲ *file_name)
{
  ਖੰਡ_2 file_contents;		/* to receive a copy of the file */

  swallow_file_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (ਸ਼ਬਦ_fastmap, 1, CHAR_ਸੈੱਟ_ਮਾਪ);
  ਲਈ (ਅੱਖਰ *cursor = file_contents.ਸ਼ੁਰੂ; cursor < file_contents.ਸਿਰਾ;
       cursor++)
    ਸ਼ਬਦ_fastmap[ਮੰਜ਼ਿਲ_uchar (*cursor)] = 0;

  ਜੇ (!gnu_extensions)
    {

      /* If GNU extensions are enabled, the only way to avoid newline as
         a break character is to write all the break characters in the
         file with no newline at all, not even at the end of the file.
         If disabled, spaces, tabs and newlines are always considered as
         break characters even if not included in the break file.  */

      ਸ਼ਬਦ_fastmap[' '] = 0;
      ਸ਼ਬਦ_fastmap['\t'] = 0;
      ਸ਼ਬਦ_fastmap['\n'] = 0;
    }

  /* Return the space of the file, which is no more required.  */

  ਮੁਕਤ (file_contents.ਸ਼ੁਰੂ);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
digest_ਸ਼ਬਦ_file (ਅੱਖਰ ਅਚਲ *file_name, ਸ਼ਬਦ_ਸਾਰਣੀ *ਸਾਰਣੀ)
{
  ਖੰਡ_2 file_contents;		/* to receive a copy of the file */
  ਅੱਖਰ *cursor;			/* cursor in file copy */
  ਅੱਖਰ *ਸ਼ਬਦ_ਸ਼ੁਰੂ;		/* start of the current word */

  swallow_file_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ (file_name, &file_contents);

  ਸਾਰਣੀ->ਸ਼ੁਰੂ = ਨਿਰਾ;
  ਸਾਰਣੀ->alloc = 0;
  ਸਾਰਣੀ->ਲੰਮਾਈ = 0;

  /* Read the whole file.  */

  cursor = file_contents.ਸ਼ੁਰੂ;
  ਜਦੋਂ (cursor < file_contents.ਸਿਰਾ)
    {

      /* Read one line, and save the word in contains.  */

      ਸ਼ਬਦ_ਸ਼ੁਰੂ = cursor;
      ਜਦੋਂ (cursor < file_contents.ਸਿਰਾ && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      ਜੇ (cursor > ਸ਼ਬਦ_ਸ਼ੁਰੂ)
        {
          ਜੇ (ਸਾਰਣੀ->ਲੰਮਾਈ == ਸਾਰਣੀ->alloc)
            ਸਾਰਣੀ->ਸ਼ੁਰੂ = xpalloc (ਸਾਰਣੀ->ਸ਼ੁਰੂ, &ਸਾਰਣੀ->alloc, 1, -1,
                                    ਆਕਾਰ *ਸਾਰਣੀ->ਸ਼ੁਰੂ);
          idx_t ਅਚਲ ਸਾਰਣੀ_idx = ਸਾਰਣੀ->ਲੰਮਾਈ++;
          ਸਾਰਣੀ->ਸ਼ੁਰੂ[ਸਾਰਣੀ_idx].ਸ਼ੁਰੂ = ਸ਼ਬਦ_ਸ਼ੁਰੂ;
          ਸਾਰਣੀ->ਸ਼ੁਰੂ[ਸਾਰਣੀ_idx].ਮਾਪ = cursor - ਸ਼ਬਦ_ਸ਼ੁਰੂ;
        }

      /* This test allows for an incomplete line at end of file.  */

      ਜੇ (cursor < file_contents.ਸਿਰਾ)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (ਸਾਰਣੀ->ਸ਼ੁਰੂ, ਸਾਰਣੀ->ਲੰਮਾਈ, ਆਕਾਰ ਸਾਰਣੀ->ਸ਼ੁਰੂ[0], compare_ਸ਼ਬਦ);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
ਲੱਭੋ_occurs_ਵਿੱਚ_ਪਾਠ (ਪੂਰਨ file_ਸੂਚਕਾਂਕ)
{
  ਅੱਖਰ *ਜਾਂਚ;			/* for scanning the source text */
  ਅੱਖਰ *ਸਤਰ_ਸ਼ੁਰੂ;		/* start of the current input line */
  ਅੱਖਰ *ਸਤਰ_ਜਾਂਚ;		/* newlines scanned until this point */
  idx_t reference_ਲੰਮਾਈ;	/* length of reference in input mode */
  ਸ਼ਬਦ possible_ਕੁੰਜੀ;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  ਅੱਖਰ *context_ਸ਼ੁਰੂ;		/* start of left context */
  ਅੱਖਰ *context_ਸਿਰਾ;		/* end of right context */
  ਅੱਖਰ *ਸ਼ਬਦ_ਸ਼ੁਰੂ;		/* start of word */
  ਅੱਖਰ *ਸ਼ਬਦ_ਸਿਰਾ;		/* end of word */
  ਅੱਖਰ *ਅਗਲਾ_context_ਸ਼ੁਰੂ;	/* next start of left context */

  ਅਚਲ ਖੰਡ_2 *ਪਾਠ_ਬਫਰ = &ਪਾਠ_buffers[file_ਸੂਚਕਾਂਕ];

  /* reference_length is always used within 'if (input_reference)'.
     However, GNU C diagnoses that it may be used uninitialized.  The
     following assignment is merely to shut it up.  */

  reference_ਲੰਮਾਈ = 0;

  /* Tracking where lines start is helpful for reference processing.  In
     auto reference mode, this allows counting lines.  In input reference
     mode, this permits finding the beginning of the references.

     The first line begins with the file, skip immediately this very first
     reference in input reference mode, to help further rejection any word
     found inside it.  Also, unconditionally assigning these variable has
     the happy effect of shutting up lint.  */

  ਸਤਰ_ਸ਼ੁਰੂ = ਪਾਠ_ਬਫਰ->ਸ਼ੁਰੂ;
  ਸਤਰ_ਜਾਂਚ = ਸਤਰ_ਸ਼ੁਰੂ;
  ਜੇ (ਨਿਵੇਸ਼_reference)
    {
      SKIP_NON_WHITE (ਸਤਰ_ਜਾਂਚ, ਪਾਠ_ਬਫਰ->ਸਿਰਾ);
      reference_ਲੰਮਾਈ = ਸਤਰ_ਜਾਂਚ - ਸਤਰ_ਸ਼ੁਰੂ;
      SKIP_WHITE (ਸਤਰ_ਜਾਂਚ, ਪਾਠ_ਬਫਰ->ਸਿਰਾ);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  ਲਈ (ਅੱਖਰ *cursor = ਪਾਠ_ਬਫਰ->ਸ਼ੁਰੂ;
       cursor < ਪਾਠ_ਬਫਰ->ਸਿਰਾ;
       cursor = ਅਗਲਾ_context_ਸ਼ੁਰੂ)
    {

      /* 'context_start' gets initialized before the processing of each
         line, or once for the whole buffer if no end of line or sentence
         sequence separator.  */

      context_ਸ਼ੁਰੂ = cursor;

      /* If an end of line or end of sentence sequence is defined and
         non-empty, 'next_context_start' will be recomputed to be the end of
         each line or sentence, before each one is processed.  If no such
         sequence, then 'next_context_start' is set at the end of the whole
         buffer, which is then considered to be a single line or sentence.
         This test also accounts for the case of an incomplete line or
         sentence at the end of the buffer.  */

      ਅਗਲਾ_context_ਸ਼ੁਰੂ = ਪਾਠ_ਬਫਰ->ਸਿਰਾ;
      ਜੇ (context_regex.string)
        ਚੋਣ (re_ਖੋਜ (&context_regex.ਨਮੂਨਾ_2, cursor,
                           ਪਾਠ_ਬਫਰ->ਸਿਰਾ - cursor,
                           0, ਪਾਠ_ਬਫਰ->ਸਿਰਾ - cursor, &context_regs))
          {
          ਹਾਲਤ -2:
            matcher_error ();

          ਹਾਲਤ -1:
            ਰੋਕੋ;

          ਹਾਲਤ 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          ਮੂਲ:
            ਅਗਲਾ_context_ਸ਼ੁਰੂ = cursor + context_regs.ਸਿਰਾ[0];
            ਰੋਕੋ;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_ਸਿਰਾ = ਅਗਲਾ_context_ਸ਼ੁਰੂ;
      SKIP_WHITE_BACKWARDS (context_ਸਿਰਾ, context_ਸ਼ੁਰੂ);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      ਜਦੋਂ (true)
        {
          ਜੇ (ਸ਼ਬਦ_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_ਖੋਜ (&ਸ਼ਬਦ_regex.ਨਮੂਨਾ_2, cursor,
                                      context_ਸਿਰਾ - cursor,
                                      0, context_ਸਿਰਾ - cursor, &ਸ਼ਬਦ_regs);
              ਜੇ (r == -2)
                matcher_error ();
              ਜੇ (r == -1)
                ਰੋਕੋ;
              ਸ਼ਬਦ_ਸ਼ੁਰੂ = cursor + ਸ਼ਬਦ_regs.ਸ਼ੁਰੂ[0];
              ਸ਼ਬਦ_ਸਿਰਾ = cursor + ਸ਼ਬਦ_regs.ਸਿਰਾ[0];
            }
          ਨਹੀਂਤਾਂ

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              ਜਾਂਚ = cursor;
              ਜਦੋਂ (ਜਾਂਚ < context_ਸਿਰਾ
                     && !ਸ਼ਬਦ_fastmap[ਮੰਜ਼ਿਲ_uchar (*ਜਾਂਚ)])
                ਜਾਂਚ++;

              ਜੇ (ਜਾਂਚ == context_ਸਿਰਾ)
                ਰੋਕੋ;

              ਸ਼ਬਦ_ਸ਼ੁਰੂ = ਜਾਂਚ;

              ਜਦੋਂ (ਜਾਂਚ < context_ਸਿਰਾ
                     && ਸ਼ਬਦ_fastmap[ਮੰਜ਼ਿਲ_uchar (*ਜਾਂਚ)])
                ਜਾਂਚ++;

              ਸ਼ਬਦ_ਸਿਰਾ = ਜਾਂਚ;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = ਸ਼ਬਦ_ਸ਼ੁਰੂ;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          ਜੇ (ਸ਼ਬਦ_ਸਿਰਾ == ਸ਼ਬਦ_ਸ਼ੁਰੂ)
            {
              cursor++;
              ਜਾਰੀ;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_ਕੁੰਜੀ.ਸ਼ੁਰੂ = cursor;
          possible_ਕੁੰਜੀ.ਮਾਪ = ਸ਼ਬਦ_ਸਿਰਾ - ਸ਼ਬਦ_ਸ਼ੁਰੂ;
          cursor += possible_ਕੁੰਜੀ.ਮਾਪ;

          ਜੇ (possible_ਕੁੰਜੀ.ਮਾਪ > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਸ਼ਬਦ_ਲੰਮਾਈ)
            ਵੱਧ_ਤੋਂ_ਵੱਧ_ਸ਼ਬਦ_ਲੰਮਾਈ = possible_ਕੁੰਜੀ.ਮਾਪ;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          ਜੇ (ਨਿਵੇਸ਼_reference)
            {
              ਜਦੋਂ (ਸਤਰ_ਜਾਂਚ < possible_ਕੁੰਜੀ.ਸ਼ੁਰੂ)
                ਜੇ (*ਸਤਰ_ਜਾਂਚ == '\n')
                  {
                    ਕੁੱਲ_ਸਤਰ_ਗਿਣਤੀ++;
                    ਸਤਰ_ਜਾਂਚ++;
                    ਸਤਰ_ਸ਼ੁਰੂ = ਸਤਰ_ਜਾਂਚ;
                    SKIP_NON_WHITE (ਸਤਰ_ਜਾਂਚ, ਪਾਠ_ਬਫਰ->ਸਿਰਾ);
                    reference_ਲੰਮਾਈ = ਸਤਰ_ਜਾਂਚ - ਸਤਰ_ਸ਼ੁਰੂ;
                  }
                ਨਹੀਂਤਾਂ
                  ਸਤਰ_ਜਾਂਚ++;
              ਜੇ (ਸਤਰ_ਜਾਂਚ > possible_ਕੁੰਜੀ.ਸ਼ੁਰੂ)
                ਜਾਰੀ;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          ਜੇ (ignore_file && ਖੋਜ_ਸਾਰਣੀ (&possible_ਕੁੰਜੀ, &ignore_ਸਾਰਣੀ))
            ਜਾਰੀ;
          ਜੇ (only_file && !ਖੋਜ_ਸਾਰਣੀ (&possible_ਕੁੰਜੀ, &only_ਸਾਰਣੀ))
            ਜਾਰੀ;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          ਜੇ (ਸੰਖਿਆ_ਦਾ_occurs[0] == occurs_alloc[0])
            occurs_ਸਾਰਣੀ[0] = xpalloc (occurs_ਸਾਰਣੀ[0], &occurs_alloc[0],
                                       1, -1, ਆਕਾਰ *occurs_ਸਾਰਣੀ[0]);
          occurs_cursor = occurs_ਸਾਰਣੀ[0] + ਸੰਖਿਆ_ਦਾ_occurs[0];

          /* Define the reference field, if any.  */

          ਜੇ (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              ਜਦੋਂ (ਸਤਰ_ਜਾਂਚ < possible_ਕੁੰਜੀ.ਸ਼ੁਰੂ)
                ਜੇ (*ਸਤਰ_ਜਾਂਚ == '\n')
                  {
                    ਕੁੱਲ_ਸਤਰ_ਗਿਣਤੀ++;
                    ਸਤਰ_ਜਾਂਚ++;
                    ਸਤਰ_ਸ਼ੁਰੂ = ਸਤਰ_ਜਾਂਚ;
                    SKIP_NON_WHITE (ਸਤਰ_ਜਾਂਚ, ਪਾਠ_ਬਫਰ->ਸਿਰਾ);
                  }
                ਨਹੀਂਤਾਂ
                  ਸਤਰ_ਜਾਂਚ++;

              occurs_cursor->reference = ਕੁੱਲ_ਸਤਰ_ਗਿਣਤੀ;
            }
          ਨਹੀਂਤਾਂ ਜੇ (ਨਿਵੇਸ਼_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = ਸਤਰ_ਸ਼ੁਰੂ - possible_ਕੁੰਜੀ.ਸ਼ੁਰੂ;
              ਜੇ (reference_ਲੰਮਾਈ > reference_max_ਚੌੜਾਈ)
                reference_max_ਚੌੜਾਈ = reference_ਲੰਮਾਈ;
            }

          /* Exclude the reference from the context in simple cases.  */

          ਜੇ (ਨਿਵੇਸ਼_reference && ਸਤਰ_ਸ਼ੁਰੂ == context_ਸ਼ੁਰੂ)
            {
              SKIP_NON_WHITE (context_ਸ਼ੁਰੂ, context_ਸਿਰਾ);
              SKIP_WHITE (context_ਸ਼ੁਰੂ, context_ਸਿਰਾ);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->ਕੁੰਜੀ = possible_ਕੁੰਜੀ;
          occurs_cursor->ਖੱਬਾ = context_ਸ਼ੁਰੂ - possible_ਕੁੰਜੀ.ਸ਼ੁਰੂ;
          occurs_cursor->ਸੱਜਾ = context_ਸਿਰਾ - possible_ਕੁੰਜੀ.ਸ਼ੁਰੂ;
          occurs_cursor->file_ਸੂਚਕਾਂਕ = file_ਸੂਚਕਾਂਕ;

          ਸੰਖਿਆ_ਦਾ_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
print_spaces (ptrdiff_t ਸੰਖਿਆ)
{
  ਲਈ (ptrdiff_t counter = ਸੰਖਿਆ; counter > 0; counter--)
    ਅੱਖਰ_ਲਿਖੋ (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
print_ਖੇਤਰ (ਖੰਡ_2 ਖੇਤਰ)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  ਲਈ (ਅੱਖਰ *cursor = ਖੇਤਰ.ਸ਼ੁਰੂ; cursor < ਖੇਤਰ.ਸਿਰਾ; cursor++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ character = *cursor;
      ਜੇ (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          ਚੋਣ (character)
            {
            ਹਾਲਤ '"':
              /* In roff output format, double any quote.  */
              ਅੱਖਰ_ਲਿਖੋ ('"');
              ਅੱਖਰ_ਲਿਖੋ ('"');
              ਰੋਕੋ;

            ਹਾਲਤ '$':
            ਹਾਲਤ '%':
            ਹਾਲਤ '&':
            ਹਾਲਤ '#':
            ਹਾਲਤ '_':
              /* In TeX output format, precede these with a backslash.  */
              ਅੱਖਰ_ਲਿਖੋ ('\\');
              ਅੱਖਰ_ਲਿਖੋ (character);
              ਰੋਕੋ;

            ਹਾਲਤ '{':
            ਹਾਲਤ '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              ਲਿਖੋ ("$\\%c$", character);
              ਰੋਕੋ;

            ਹਾਲਤ '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              ਰੋਕੋ;

            ਮੂਲ:
              /* Any other flagged character produces a single space.  */
              ਅੱਖਰ_ਲਿਖੋ (' ');
            }
        }
      ਨਹੀਂਤਾਂ
        ਅੱਖਰ_ਲਿਖੋ (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
fix_output_parameters (ਖਾਲੀ)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  ਜੇ (auto_reference)
    {
      reference_max_ਚੌੜਾਈ = 0;
      ਲਈ (ਪੂਰਨ file_ਸੂਚਕਾਂਕ = 0; file_ਸੂਚਕਾਂਕ < ਸੰਖਿਆ_ਨਿਵੇਸ਼_files; file_ਸੂਚਕਾਂਕ++)
        {
          intmax_t ਸਤਰ_ordinal = file_ਸਤਰ_ਗਿਣਤੀ[file_ਸੂਚਕਾਂਕ] + 1;
          ਜੇ (file_ਸੂਚਕਾਂਕ > 0)
            ਸਤਰ_ordinal -= file_ਸਤਰ_ਗਿਣਤੀ[file_ਸੂਚਕਾਂਕ - 1];
          ਅੱਖਰ ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_ਚੌੜਾਈ = sprintf (ordinal_string, "%jd", ਸਤਰ_ordinal);
          ਜੇ (ਨਿਵੇਸ਼_file_name[file_ਸੂਚਕਾਂਕ])
            reference_ਚੌੜਾਈ += ਲੜੀ_ਲੰਬਾਈ (ਨਿਵੇਸ਼_file_name[file_ਸੂਚਕਾਂਕ]);
          ਜੇ (reference_ਚੌੜਾਈ > reference_max_ਚੌੜਾਈ)
            reference_max_ਚੌੜਾਈ = reference_ਚੌੜਾਈ;
        }
      reference_max_ਚੌੜਾਈ++;
      reference.ਸ਼ੁਰੂ = xmalloc (reference_max_ਚੌੜਾਈ + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  ਜੇ ((auto_reference || ਨਿਵੇਸ਼_reference) && !ਸੱਜਾ_reference)
    ਸਤਰ_ਚੌੜਾਈ = MAX (0, ਸਤਰ_ਚੌੜਾਈ - (reference_max_ਚੌੜਾਈ + gap_ਮਾਪ));

  /* The output lines, minimally, will contain from left to right a left
     context, a gap, and a keyword followed by the right context with no
     special intervening gap.  Half of the line width is dedicated to the
     left context and the gap, the other half is dedicated to the keyword
     and the right context; these values are computed once and for all here.
     There also are tail and head wrap around fields, used when the keyword
     is near the beginning or the end of the line, or when some long word
     cannot fit in, but leave place from wrapped around shorter words.  The
     maximum width of these fields are recomputed separately for each line,
     on a case by case basis.  It is worth noting that it cannot happen that
     both the tail and head fields are used at once.  */

  half_ਸਤਰ_ਚੌੜਾਈ = ਸਤਰ_ਚੌੜਾਈ >> 1;
  before_max_ਚੌੜਾਈ = half_ਸਤਰ_ਚੌੜਾਈ - gap_ਮਾਪ;
  keyafter_max_ਚੌੜਾਈ = half_ਸਤਰ_ਚੌੜਾਈ;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  ਜੇ (truncation_string && *truncation_string)
    truncation_string_ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ (truncation_string);
  ਨਹੀਂਤਾਂ
    truncation_string = ਨਿਰਾ;

  ਜੇ (gnu_extensions)
    {

      /* When flagging truncation at the left of the keyword, the
         truncation mark goes at the beginning of the before field,
         unless there is a head field, in which case the mark goes at the
         left of the head field.  When flagging truncation at the right
         of the keyword, the mark goes at the end of the keyafter field,
         unless there is a tail field, in which case the mark goes at the
         end of the tail field.  Only eight combination cases could arise
         for truncation marks:

         . None.
         . One beginning the before field.
         . One beginning the head field.
         . One ending the keyafter field.
         . One ending the tail field.
         . One beginning the before field, another ending the keyafter field.
         . One ending the tail field, another beginning the before field.
         . One ending the keyafter field, another beginning the head field.

         So, there is at most two truncation marks, which could appear both
         on the left side of the center of the output line, both on the
         right side, or one on either side.  */

      before_max_ਚੌੜਾਈ -= 2 * truncation_string_ਲੰਮਾਈ;
      keyafter_max_ਚੌੜਾਈ -= 2 * truncation_string_ਲੰਮਾਈ;
    }
  ਨਹੀਂਤਾਂ
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_ਚੌੜਾਈ -= 2 * truncation_string_ਲੰਮਾਈ + 1;
    }

  ਜੇ (before_max_ਚੌੜਾਈ < 0)
    before_max_ਚੌੜਾਈ = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  ਲਈ (ਪੂਰਨ character = 0; character < CHAR_ਸੈੱਟ_ਮਾਪ; character++)
    edited_flag[character] = !! ਖਾਲੀ_ਥਾਂ_ਹੈ (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  ਚੋਣ (output_format)
    {
    ਹਾਲਤ UNKNOWN_FORMAT:
      /* Should never happen.  */

    ਹਾਲਤ DUMB_FORMAT:
      ਰੋਕੋ;

    ਹਾਲਤ ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      ਰੋਕੋ;

    ਹਾਲਤ TEX_FORMAT:

      /* Various characters need special processing.  */

      ਲਈ (ਅੱਖਰ ਅਚਲ *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[ਮੰਜ਼ਿਲ_uchar (*cursor)] = 1;

      ਰੋਕੋ;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t ਪੂਛ_max_ਚੌੜਾਈ;	/* allowable width of tail field */
  ptrdiff_t ਸਿਰ_max_ਚੌੜਾਈ;	/* allowable width of head field */
  ਅੱਖਰ *cursor;			/* running cursor in source text */
  ਅੱਖਰ *ਖੱਬਾ_context_ਸ਼ੁਰੂ;	/* start of left context */
  ਅੱਖਰ *ਸੱਜਾ_context_ਸਿਰਾ;	/* end of right context */
  ਅੱਖਰ *ਖੱਬਾ_ਖੇਤਰ_ਸ਼ੁਰੂ;	/* conservative start for 'head'/'before' */
  ਅੱਖਰ ਅਚਲ *file_name;	/* file name for reference */
  intmax_t ਸਤਰ_ordinal;	/* line ordinal for reference */
  ਅੱਖਰ ਅਚਲ *ਬਫਰ_ਸ਼ੁਰੂ;	/* start of buffered file for this occurs */
  ਅੱਖਰ ਅਚਲ *ਬਫਰ_ਸਿਰਾ;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.ਸ਼ੁਰੂ = occurs->ਕੁੰਜੀ.ਸ਼ੁਰੂ;
  keyafter.ਸਿਰਾ = keyafter.ਸ਼ੁਰੂ + occurs->ਕੁੰਜੀ.ਮਾਪ;
  ਖੱਬਾ_context_ਸ਼ੁਰੂ = keyafter.ਸ਼ੁਰੂ + occurs->ਖੱਬਾ;
  ਸੱਜਾ_context_ਸਿਰਾ = keyafter.ਸ਼ੁਰੂ + occurs->ਸੱਜਾ;

  ਬਫਰ_ਸ਼ੁਰੂ = ਪਾਠ_buffers[occurs->file_ਸੂਚਕਾਂਕ].ਸ਼ੁਰੂ;
  ਬਫਰ_ਸਿਰਾ = ਪਾਠ_buffers[occurs->file_ਸੂਚਕਾਂਕ].ਸਿਰਾ;

  cursor = keyafter.ਸਿਰਾ;
  ਜਦੋਂ (cursor < ਸੱਜਾ_context_ਸਿਰਾ
         && cursor <= keyafter.ਸ਼ੁਰੂ + keyafter_max_ਚੌੜਾਈ)
    {
      keyafter.ਸਿਰਾ = cursor;
      SKIP_SOMETHING (cursor, ਸੱਜਾ_context_ਸਿਰਾ);
    }
  ਜੇ (cursor <= keyafter.ਸ਼ੁਰੂ + keyafter_max_ਚੌੜਾਈ)
    keyafter.ਸਿਰਾ = cursor;

  keyafter_truncation = truncation_string && keyafter.ਸਿਰਾ < ਸੱਜਾ_context_ਸਿਰਾ;

  SKIP_WHITE_BACKWARDS (keyafter.ਸਿਰਾ, keyafter.ਸ਼ੁਰੂ);

  /* When the left context is wide, it might take some time to catch up from
     the left context boundary to the beginning of the 'head' or 'before'
     fields.  So, in this case, to speed the catchup, we jump back from the
     keyword, using some secure distance, possibly falling in the middle of
     a word.  A secure backward jump would be at least half the maximum
     width of a line, plus the size of the longest word met in the whole
     input.  We conclude this backward jump by a skip forward of at least
     one word.  In this manner, we should not inadvertently accept only part
     of a word.  From the reached point, when it will be time to fix the
     beginning of 'head' or 'before' fields, we will skip forward words or
     delimiters until we get sufficiently near.  */

  ਜੇ (-occurs->ਖੱਬਾ > half_ਸਤਰ_ਚੌੜਾਈ + ਵੱਧ_ਤੋਂ_ਵੱਧ_ਸ਼ਬਦ_ਲੰਮਾਈ)
    {
      ਖੱਬਾ_ਖੇਤਰ_ਸ਼ੁਰੂ
        = keyafter.ਸ਼ੁਰੂ - (half_ਸਤਰ_ਚੌੜਾਈ + ਵੱਧ_ਤੋਂ_ਵੱਧ_ਸ਼ਬਦ_ਲੰਮਾਈ);
      SKIP_SOMETHING (ਖੱਬਾ_ਖੇਤਰ_ਸ਼ੁਰੂ, keyafter.ਸ਼ੁਰੂ);
    }
  ਨਹੀਂਤਾਂ
    ਖੱਬਾ_ਖੇਤਰ_ਸ਼ੁਰੂ = keyafter.ਸ਼ੁਰੂ + occurs->ਖੱਬਾ;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.ਸ਼ੁਰੂ = ਖੱਬਾ_ਖੇਤਰ_ਸ਼ੁਰੂ;
  before.ਸਿਰਾ = keyafter.ਸ਼ੁਰੂ;
  SKIP_WHITE_BACKWARDS (before.ਸਿਰਾ, before.ਸ਼ੁਰੂ);

  ਜਦੋਂ (before.ਸ਼ੁਰੂ + before_max_ਚੌੜਾਈ < before.ਸਿਰਾ)
    SKIP_SOMETHING (before.ਸ਼ੁਰੂ, before.ਸਿਰਾ);

  ਜੇ (truncation_string)
    {
      cursor = before.ਸ਼ੁਰੂ;
      SKIP_WHITE_BACKWARDS (cursor, ਬਫਰ_ਸ਼ੁਰੂ);
      before_truncation = cursor > ਖੱਬਾ_context_ਸ਼ੁਰੂ;
    }
  ਨਹੀਂਤਾਂ
    before_truncation = false;

  SKIP_WHITE (before.ਸ਼ੁਰੂ, ਬਫਰ_ਸਿਰਾ);

  /* The tail could not take more columns than what has been left in the
     left context field, and a gap is mandatory.  It starts after the
     right context, and does not contain prefixed spaces.  It ends at
     the end of line, the end of buffer or when the tail field is full,
     whichever comes first.  It cannot contain only part of a word, and
     has no suffixed spaces.  */

  ਪੂਛ_max_ਚੌੜਾਈ
    = before_max_ਚੌੜਾਈ - (before.ਸਿਰਾ - before.ਸ਼ੁਰੂ) - gap_ਮਾਪ;

  ਜੇ (ਪੂਛ_max_ਚੌੜਾਈ > 0)
    {
      ਪੂਛ.ਸ਼ੁਰੂ = keyafter.ਸਿਰਾ;
      SKIP_WHITE (ਪੂਛ.ਸ਼ੁਰੂ, ਬਫਰ_ਸਿਰਾ);

      ਪੂਛ.ਸਿਰਾ = ਪੂਛ.ਸ਼ੁਰੂ;
      cursor = ਪੂਛ.ਸਿਰਾ;
      ਜਦੋਂ (cursor < ਸੱਜਾ_context_ਸਿਰਾ
             && cursor < ਪੂਛ.ਸ਼ੁਰੂ + ਪੂਛ_max_ਚੌੜਾਈ)
        {
          ਪੂਛ.ਸਿਰਾ = cursor;
          SKIP_SOMETHING (cursor, ਸੱਜਾ_context_ਸਿਰਾ);
        }

      ਜੇ (cursor < ਪੂਛ.ਸ਼ੁਰੂ + ਪੂਛ_max_ਚੌੜਾਈ)
        ਪੂਛ.ਸਿਰਾ = cursor;

      ਜੇ (ਪੂਛ.ਸਿਰਾ > ਪੂਛ.ਸ਼ੁਰੂ)
        {
          keyafter_truncation = false;
          ਪੂਛ_truncation = truncation_string && ਪੂਛ.ਸਿਰਾ < ਸੱਜਾ_context_ਸਿਰਾ;
        }
      ਨਹੀਂਤਾਂ
        ਪੂਛ_truncation = false;

      SKIP_WHITE_BACKWARDS (ਪੂਛ.ਸਿਰਾ, ਪੂਛ.ਸ਼ੁਰੂ);
    }
  ਨਹੀਂਤਾਂ
    {

      /* No place left for a tail field.  */

      ਪੂਛ.ਸ਼ੁਰੂ = ਨਿਰਾ;
      ਪੂਛ.ਸਿਰਾ = ਨਿਰਾ;
      ਪੂਛ_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  ਸਿਰ_max_ਚੌੜਾਈ
    = keyafter_max_ਚੌੜਾਈ - (keyafter.ਸਿਰਾ - keyafter.ਸ਼ੁਰੂ) - gap_ਮਾਪ;

  ਜੇ (ਸਿਰ_max_ਚੌੜਾਈ > 0)
    {
      ਸਿਰ.ਸਿਰਾ = before.ਸ਼ੁਰੂ;
      SKIP_WHITE_BACKWARDS (ਸਿਰ.ਸਿਰਾ, ਬਫਰ_ਸ਼ੁਰੂ);

      ਸਿਰ.ਸ਼ੁਰੂ = ਖੱਬਾ_ਖੇਤਰ_ਸ਼ੁਰੂ;
      ਜਦੋਂ (ਸਿਰ.ਸ਼ੁਰੂ + ਸਿਰ_max_ਚੌੜਾਈ < ਸਿਰ.ਸਿਰਾ)
        SKIP_SOMETHING (ਸਿਰ.ਸ਼ੁਰੂ, ਸਿਰ.ਸਿਰਾ);

      ਜੇ (ਸਿਰ.ਸਿਰਾ > ਸਿਰ.ਸ਼ੁਰੂ)
        {
          before_truncation = false;
          ਸਿਰ_truncation = (truncation_string
                             && ਸਿਰ.ਸ਼ੁਰੂ > ਖੱਬਾ_context_ਸ਼ੁਰੂ);
        }
      ਨਹੀਂਤਾਂ
        ਸਿਰ_truncation = false;

      SKIP_WHITE (ਸਿਰ.ਸ਼ੁਰੂ, ਸਿਰ.ਸਿਰਾ);
    }
  ਨਹੀਂਤਾਂ
    {

      /* No place left for a head field.  */

      ਸਿਰ.ਸ਼ੁਰੂ = ਨਿਰਾ;
      ਸਿਰ.ਸਿਰਾ = ਨਿਰਾ;
      ਸਿਰ_truncation = false;
    }

  ਜੇ (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = ਨਿਵੇਸ਼_file_name[occurs->file_ਸੂਚਕਾਂਕ];
      ਜੇ (!file_name)
        file_name = "";

      ਸਤਰ_ordinal = occurs->reference + 1;
      ਜੇ (occurs->file_ਸੂਚਕਾਂਕ > 0)
        ਸਤਰ_ordinal -= file_ਸਤਰ_ਗਿਣਤੀ[occurs->file_ਸੂਚਕਾਂਕ - 1];

      ਅੱਖਰ *file_ਸਿਰਾ = stpcpy (reference.ਸ਼ੁਰੂ, file_name);
      reference.ਸਿਰਾ = file_ਸਿਰਾ + sprintf (file_ਸਿਰਾ, ":%jd", ਸਤਰ_ordinal);
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਨਿਵੇਸ਼_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.ਸ਼ੁਰੂ = keyafter.ਸ਼ੁਰੂ + occurs->reference;
      reference.ਸਿਰਾ = reference.ਸ਼ੁਰੂ;
      SKIP_NON_WHITE (reference.ਸਿਰਾ, ਸੱਜਾ_context_ਸਿਰਾ);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
output_one_roff_ਸਤਰ (ਖਾਲੀ)
{
  /* Output the 'tail' field.  */

  ਲਿਖੋ (".%s \"", macro_name);
  print_ਖੇਤਰ (ਪੂਛ);
  ਜੇ (ਪੂਛ_truncation)
    fputs (truncation_string, stdout);
  ਅੱਖਰ_ਲਿਖੋ ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  ਜੇ (before_truncation)
    fputs (truncation_string, stdout);
  print_ਖੇਤਰ (before);
  ਅੱਖਰ_ਲਿਖੋ ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_ਖੇਤਰ (keyafter);
  ਜੇ (keyafter_truncation)
    fputs (truncation_string, stdout);
  ਅੱਖਰ_ਲਿਖੋ ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  ਜੇ (ਸਿਰ_truncation)
    fputs (truncation_string, stdout);
  print_ਖੇਤਰ (ਸਿਰ);
  ਅੱਖਰ_ਲਿਖੋ ('"');

  /* Conditionally output the 'reference' field.  */

  ਜੇ (auto_reference || ਨਿਵੇਸ਼_reference)
    {
      fputs (" \"", stdout);
      print_ਖੇਤਰ (reference);
      ਅੱਖਰ_ਲਿਖੋ ('"');
    }

  ਅੱਖਰ_ਲਿਖੋ ('\n');
}

/*---------------------------------------------------------.
| Output the current output fields as one line for 'TeX'.  |
`---------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
output_one_tex_ਸਤਰ (ਖਾਲੀ)
{
  ਖੰਡ_2 ਕੁੰਜੀ;			/* key field, isolated */
  ਖੰਡ_2 after;			/* after field, isolated */
  ਅੱਖਰ *cursor;			/* running cursor in source text */

  ਲਿਖੋ ("\\%s ", macro_name);
  ਅੱਖਰ_ਲਿਖੋ ('{');
  print_ਖੇਤਰ (ਪੂਛ);
  fputs ("}{", stdout);
  print_ਖੇਤਰ (before);
  fputs ("}{", stdout);
  ਕੁੰਜੀ.ਸ਼ੁਰੂ = keyafter.ਸ਼ੁਰੂ;
  after.ਸਿਰਾ = keyafter.ਸਿਰਾ;
  cursor = keyafter.ਸ਼ੁਰੂ;
  SKIP_SOMETHING (cursor, keyafter.ਸਿਰਾ);
  ਕੁੰਜੀ.ਸਿਰਾ = cursor;
  after.ਸ਼ੁਰੂ = cursor;
  print_ਖੇਤਰ (ਕੁੰਜੀ);
  fputs ("}{", stdout);
  print_ਖੇਤਰ (after);
  fputs ("}{", stdout);
  print_ਖੇਤਰ (ਸਿਰ);
  ਅੱਖਰ_ਲਿਖੋ ('}');
  ਜੇ (auto_reference || ਨਿਵੇਸ਼_reference)
    {
      ਅੱਖਰ_ਲਿਖੋ ('{');
      print_ਖੇਤਰ (reference);
      ਅੱਖਰ_ਲਿਖੋ ('}');
    }
  ਅੱਖਰ_ਲਿਖੋ ('\n');
}

/*-------------------------------------------------------------------.
| Output the current output fields as one line for a dumb terminal.  |
`-------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
output_one_dumb_ਸਤਰ (ਖਾਲੀ)
{
  ਜੇ (!ਸੱਜਾ_reference)
    {
      ਜੇ (auto_reference)
        {

          /* Output the 'reference' field, in such a way that GNU emacs
             next-error will handle it.  The ending colon is taken from the
             gap which follows.  */

          print_ਖੇਤਰ (reference);
          ਅੱਖਰ_ਲਿਖੋ (':');
          print_spaces (reference_max_ਚੌੜਾਈ
                        + gap_ਮਾਪ
                        - (reference.ਸਿਰਾ - reference.ਸ਼ੁਰੂ)
                        - 1);
        }
      ਨਹੀਂਤਾਂ
        {

          /* Output the 'reference' field and its following gap.  */

          print_ਖੇਤਰ (reference);
          print_spaces (reference_max_ਚੌੜਾਈ
                        + gap_ਮਾਪ
                        - (reference.ਸਿਰਾ - reference.ਸ਼ੁਰੂ));
        }
    }

  ਜੇ (ਪੂਛ.ਸ਼ੁਰੂ < ਪੂਛ.ਸਿਰਾ)
    {
      /* Output the 'tail' field.  */

      print_ਖੇਤਰ (ਪੂਛ);
      ਜੇ (ਪੂਛ_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_ਸਤਰ_ਚੌੜਾਈ - gap_ਮਾਪ
                    - (before.ਸਿਰਾ - before.ਸ਼ੁਰੂ)
                    - (before_truncation ? truncation_string_ਲੰਮਾਈ : 0)
                    - (ਪੂਛ.ਸਿਰਾ - ਪੂਛ.ਸ਼ੁਰੂ)
                    - (ਪੂਛ_truncation ? truncation_string_ਲੰਮਾਈ : 0));
    }
  ਨਹੀਂਤਾਂ
    print_spaces (half_ਸਤਰ_ਚੌੜਾਈ - gap_ਮਾਪ
                  - (before.ਸਿਰਾ - before.ਸ਼ੁਰੂ)
                  - (before_truncation ? truncation_string_ਲੰਮਾਈ : 0));

  /* Output the 'before' field.  */

  ਜੇ (before_truncation)
    fputs (truncation_string, stdout);
  print_ਖੇਤਰ (before);

  print_spaces (gap_ਮਾਪ);

  /* Output the 'keyafter' field.  */

  print_ਖੇਤਰ (keyafter);
  ਜੇ (keyafter_truncation)
    fputs (truncation_string, stdout);

  ਜੇ (ਸਿਰ.ਸ਼ੁਰੂ < ਸਿਰ.ਸਿਰਾ)
    {
      /* Output the 'head' field.  */

      print_spaces (half_ਸਤਰ_ਚੌੜਾਈ
                    - (keyafter.ਸਿਰਾ - keyafter.ਸ਼ੁਰੂ)
                    - (keyafter_truncation ? truncation_string_ਲੰਮਾਈ : 0)
                    - (ਸਿਰ.ਸਿਰਾ - ਸਿਰ.ਸ਼ੁਰੂ)
                    - (ਸਿਰ_truncation ? truncation_string_ਲੰਮਾਈ : 0));
      ਜੇ (ਸਿਰ_truncation)
        fputs (truncation_string, stdout);
      print_ਖੇਤਰ (ਸਿਰ);
    }
  ਨਹੀਂਤਾਂ

    ਜੇ ((auto_reference || ਨਿਵੇਸ਼_reference) && ਸੱਜਾ_reference)
      print_spaces (half_ਸਤਰ_ਚੌੜਾਈ
                    - (keyafter.ਸਿਰਾ - keyafter.ਸ਼ੁਰੂ)
                    - (keyafter_truncation ? truncation_string_ਲੰਮਾਈ : 0));

  ਜੇ ((auto_reference || ਨਿਵੇਸ਼_reference) && ਸੱਜਾ_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_ਮਾਪ);
      print_ਖੇਤਰ (reference);
    }

  ਅੱਖਰ_ਲਿਖੋ ('\n');
}

/*------------------------------------------------------------------------.
| Scan the whole occurs table and, for each entry, output one line in the |
| appropriate format.							  |
`------------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
generate_all_output (ਖਾਲੀ)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  ਪੂਛ.ਸ਼ੁਰੂ = ਨਿਰਾ;
  ਪੂਛ.ਸਿਰਾ = ਨਿਰਾ;
  ਪੂਛ_truncation = false;

  ਸਿਰ.ਸ਼ੁਰੂ = ਨਿਰਾ;
  ਸਿਰ.ਸਿਰਾ = ਨਿਰਾ;
  ਸਿਰ_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_ਸਾਰਣੀ[0];

  ਲਈ (idx_t occurs_ਸੂਚਕਾਂਕ = 0; occurs_ਸੂਚਕਾਂਕ < ਸੰਖਿਆ_ਦਾ_occurs[0];
       occurs_ਸੂਚਕਾਂਕ++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      ਚੋਣ (output_format)
        {
        ਹਾਲਤ UNKNOWN_FORMAT:
          /* Should never happen.  */

        ਹਾਲਤ DUMB_FORMAT:
          output_one_dumb_ਸਤਰ ();
          ਰੋਕੋ;

        ਹਾਲਤ ROFF_FORMAT:
          output_one_roff_ਸਤਰ ();
          ਰੋਕੋ;

        ਹਾਲਤ TEX_FORMAT:
          output_one_tex_ਸਤਰ ();
          ਰੋਕੋ;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [ਨਿਵੇਸ਼_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [ਨਿਵੇਸ਼_2 [OUTPUT]]\n"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Output a permuted index, including context, ਦਾ the ਸ਼ਬਦ_4 ਵਿੱਚ the ਨਿਵੇਸ਼ files.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --ਸਵੈਚਾਲਿਤ-reference           output automatically generated references\n\
"));
      oputs (_("\
  -G, --traditional              behave ਹੋਰ like System V 'ptx'\n\
"));
      oputs (_("\
  -F, --flag-truncation=STRING   use STRING ਲਈ flagging ਸਤਰ truncations.\n\
                                 The ਮੂਲ is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name ਮੰਜ਼ਿਲ use instead ਦਾ 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --ਸੱਜਾ-side-refs          put references at ਸੱਜਾ, not counted ਵਿੱਚ -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   ਲਈ ਸਿਰਾ ਦਾ ਸਤਰਾਂ or ਸਿਰਾ ਦਾ sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --ਸ਼ਬਦ_2-regexp=REGEXP       use REGEXP ਮੰਜ਼ਿਲ ਮੇਲ each keyword\n\
"));
      oputs (_("\
  -b, --ਰੋਕੋ-file=FILE          ਸ਼ਬਦ_2 ਰੋਕੋ characters ਵਿੱਚ this FILE\n\
"));
      oputs (_("\
  -f, --ignore-ਹਾਲਤ              fold ਹੇਠਲਾ ਹਾਲਤ ਮੰਜ਼ਿਲ ਉੱਤਲਾ ਹਾਲਤ ਲਈ ਛਾਂਟੀ\n\
"));
      oputs (_("\
  -g, --gap-ਮਾਪ=ਸੰਖਿਆ_2          gap ਮਾਪ ਵਿੱਚ columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         ਪਠ ignore ਸ਼ਬਦ_2 list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           ਪਠ only ਸ਼ਬਦ_2 list from this FILE\n\
"));
      oputs (_("\
  -r, --references               ਪਹਿਲਾ ਖੇਤਰ ਦਾ each ਸਤਰ is a reference\n\
"));
      oputs (_("\
  -t, --typeset-ਬਹੁਲਕ_2             change the ਮੂਲ ਚੌੜਾਈ from 72 ਮੰਜ਼ਿਲ 100\n\
"));
      oputs (_("\
  -w, --ਚੌੜਾਈ=ਸੰਖਿਆ_2             output ਚੌੜਾਈ ਵਿੱਚ columns, reference excluded\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/*----------------------------------------------------------------------.
| Main program.  Decode ARGC arguments passed through the ARGV array of |
| strings, then launch execution.				        |
`----------------------------------------------------------------------*/

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"auto-reference", no_ਦਲੀਲ, ਨਿਰਾ, 'A'},
  {"break-file", required_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"flag-truncation", required_ਦਲੀਲ, ਨਿਰਾ, 'F'},
  {"ignore-case", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"gap-size", required_ਦਲੀਲ, ਨਿਰਾ, 'g'},
  {"ignore-file", required_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"macro-name", required_ਦਲੀਲ, ਨਿਰਾ, 'M'},
  {"only-file", required_ਦਲੀਲ, ਨਿਰਾ, 'o'},
  {"references", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"right-side-refs", no_ਦਲੀਲ, ਨਿਰਾ, 'R'},
  {"format", required_ਦਲੀਲ, ਨਿਰਾ, FORMAT_OPTION},
  {"sentence-regexp", required_ਦਲੀਲ, ਨਿਰਾ, 'S'},
  {"traditional", no_ਦਲੀਲ, ਨਿਰਾ, 'G'},
  {"typeset-mode", no_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"width", required_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  {"word-regexp", required_ਦਲੀਲ, ਨਿਰਾ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0},
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ format_ਦਲੀਲਾਂ[] =
{
  "roff", "tex", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ Format ਅਚਲ format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ optchar;			/* argument character */
  ਪੂਰਨ file_ਸੂਚਕਾਂਕ;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, ਨਿਰਾ),
         optchar != EOF)
    {
      ਚੋਣ (optchar)
        {
        ਮੂਲ:
          usage (EXIT_FAILURE);

        ਹਾਲਤ 'G':
          gnu_extensions = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'b':
          break_file = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
          ignore_case = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'g':
          {
            intmax_t tmp;
            ਜੇ (! (xstrtoimax (optarg, ਨਿਰਾ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_ਮਾਪ = tmp;
            ਰੋਕੋ;
          }

        ਹਾਲਤ 'i':
          ignore_file = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'o':
          only_file = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'r':
          ਨਿਵੇਸ਼_reference = true;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          ਜੇ (ਸਤਰ_ਚੌੜਾਈ < 0)
            ਸਤਰ_ਚੌੜਾਈ = 100;
          ਰੋਕੋ;

        ਹਾਲਤ 'w':
          {
            intmax_t tmp;
            ਜੇ (! (xstrtoimax (optarg, ਨਿਰਾ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            ਸਤਰ_ਚੌੜਾਈ = tmp;
            ਰੋਕੋ;
          }

        ਹਾਲਤ 'A':
          auto_reference = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'M':
          macro_name = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'O':
          output_format = ROFF_FORMAT;
          ਰੋਕੋ;

        ਹਾਲਤ 'R':
          ਸੱਜਾ_reference = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'T':
          output_format = TEX_FORMAT;
          ਰੋਕੋ;

        ਹਾਲਤ 'W':
          ਸ਼ਬਦ_regex.string = optarg;
          unescape_string (optarg);
          ਜੇ (!*ਸ਼ਬਦ_regex.string)
            ਸ਼ਬਦ_regex.string = ਨਿਰਾ;
          ਰੋਕੋ;

        ਹਾਲਤ FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_ਦਲੀਲਾਂ, format_vals);
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        }
    }

  ਜੇ (ਸਤਰ_ਚੌੜਾਈ < 0)
    ਸਤਰ_ਚੌੜਾਈ = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  ਜੇ (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      ਨਿਵੇਸ਼_file_name = xmalloc (ਆਕਾਰ *ਨਿਵੇਸ਼_file_name);
      file_ਸਤਰ_ਗਿਣਤੀ = xmalloc (ਆਕਾਰ *file_ਸਤਰ_ਗਿਣਤੀ);
      ਪਾਠ_buffers =    xmalloc (ਆਕਾਰ *ਪਾਠ_buffers);
      ਸੰਖਿਆ_ਨਿਵੇਸ਼_files = 1;
      ਨਿਵੇਸ਼_file_name[0] = ਨਿਰਾ;
    }
  ਨਹੀਂਤਾਂ ਜੇ (gnu_extensions)
    {
      ਸੰਖਿਆ_ਨਿਵੇਸ਼_files = argc - optind;
      ਨਿਵੇਸ਼_file_name = xnmalloc (ਸੰਖਿਆ_ਨਿਵੇਸ਼_files, ਆਕਾਰ *ਨਿਵੇਸ਼_file_name);
      file_ਸਤਰ_ਗਿਣਤੀ = xnmalloc (ਸੰਖਿਆ_ਨਿਵੇਸ਼_files, ਆਕਾਰ *file_ਸਤਰ_ਗਿਣਤੀ);
      ਪਾਠ_buffers    = xnmalloc (ਸੰਖਿਆ_ਨਿਵੇਸ਼_files, ਆਕਾਰ *ਪਾਠ_buffers);

      ਲਈ (file_ਸੂਚਕਾਂਕ = 0; file_ਸੂਚਕਾਂਕ < ਸੰਖਿਆ_ਨਿਵੇਸ਼_files; file_ਸੂਚਕਾਂਕ++)
        {
          ਜੇ (!*argv[optind] || streq (argv[optind], "-"))
            ਨਿਵੇਸ਼_file_name[file_ਸੂਚਕਾਂਕ] = ਨਿਰਾ;
          ਨਹੀਂਤਾਂ
            ਨਿਵੇਸ਼_file_name[file_ਸੂਚਕਾਂਕ] = argv[optind];
          optind++;
        }
    }
  ਨਹੀਂਤਾਂ
    {

      /* There is one necessary input file.  */

      ਸੰਖਿਆ_ਨਿਵੇਸ਼_files = 1;
      ਨਿਵੇਸ਼_file_name = xmalloc (ਆਕਾਰ *ਨਿਵੇਸ਼_file_name);
      file_ਸਤਰ_ਗਿਣਤੀ = xmalloc (ਆਕਾਰ *file_ਸਤਰ_ਗਿਣਤੀ);
      ਪਾਠ_buffers    = xmalloc (ਆਕਾਰ *ਪਾਠ_buffers);
      ਜੇ (!*argv[optind] || streq (argv[optind], "-"))
        ਨਿਵੇਸ਼_file_name[0] = ਨਿਰਾ;
      ਨਹੀਂਤਾਂ
        ਨਿਵੇਸ਼_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      ਜੇ (optind < argc)
        {
          ਜੇ (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      ਜੇ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  ਜੇ (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  ਜੇ (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  ਜੇ (ignore_file)
    {
      digest_ਸ਼ਬਦ_file (ignore_file, &ignore_ਸਾਰਣੀ);
      ਜੇ (ignore_ਸਾਰਣੀ.ਲੰਮਾਈ == 0)
        ignore_file = ਨਿਰਾ;
    }

  ਜੇ (only_file)
    {
      digest_ਸ਼ਬਦ_file (only_file, &only_ਸਾਰਣੀ);
      ਜੇ (only_ਸਾਰਣੀ.ਲੰਮਾਈ == 0)
        only_file = ਨਿਰਾ;
    }

  /* Prepare to study all the input files.  */

  ਸੰਖਿਆ_ਦਾ_occurs[0] = 0;
  ਕੁੱਲ_ਸਤਰ_ਗਿਣਤੀ = 0;
  ਵੱਧ_ਤੋਂ_ਵੱਧ_ਸ਼ਬਦ_ਲੰਮਾਈ = 0;
  reference_max_ਚੌੜਾਈ = 0;

  ਲਈ (file_ਸੂਚਕਾਂਕ = 0; file_ਸੂਚਕਾਂਕ < ਸੰਖਿਆ_ਨਿਵੇਸ਼_files; file_ਸੂਚਕਾਂਕ++)
    {
      ਖੰਡ_2 *ਪਾਠ_ਬਫਰ = ਪਾਠ_buffers + file_ਸੂਚਕਾਂਕ;

      /* Read the file contents into memory, then study it.  */

      swallow_file_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ (ਨਿਵੇਸ਼_file_name[file_ਸੂਚਕਾਂਕ], ਪਾਠ_ਬਫਰ);
      ਲੱਭੋ_occurs_ਵਿੱਚ_ਪਾਠ (file_ਸੂਚਕਾਂਕ);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      ਕੁੱਲ_ਸਤਰ_ਗਿਣਤੀ++;
      file_ਸਤਰ_ਗਿਣਤੀ[file_ਸੂਚਕਾਂਕ] = ਕੁੱਲ_ਸਤਰ_ਗਿਣਤੀ;
    }

  /* Do the output process phase.  */

  ਛਾਂਟੋ_ਮਿਲਿਆ_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  ਵਾਪਸ EXIT_SUCCESS;
}
