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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ <regex.h>
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "octhexdigits.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "read-file.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#ವ್ಯಾಖ್ಯಾನಿಸು CHAR_ಗಣ_ಗಾತ್ರ 256

/* Debugging the memory allocator.  */

#ಆದರೆ WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#ಆದರೆ_ಅಂತ್ಯ

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

ಎಣಿಕೆ Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

ಸ್ಥಿರ bool gnu_extensions = true;	/* trigger all GNU extensions */
ಸ್ಥಿರ bool auto_reference = false;	/* refs are 'file_name:line_number:' */
ಸ್ಥಿರ bool ಒಳಹರಿವು_reference = false;	/* refs at beginning of input lines */
ಸ್ಥಿರ bool ಬಲ_reference = false;	/* output refs after right context  */
ಸ್ಥಿರ idx_t ಸಾಲು_ಅಗಲ = -1;		/* output line width in characters */
ಸ್ಥಿರ idx_t gap_ಗಾತ್ರ = 3;	/* number of spaces between output fields */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *truncation_string = "/";
                                /* string used to mark line truncations */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *macro_name = "xx";	/* macro name for roff or TeX output */
ಸ್ಥಿರ ಎಣಿಕೆ Format output_format = UNKNOWN_FORMAT;
                                /* output format */

ಸ್ಥಿರ bool ignore_case = false;	/* fold lower to upper for sorting */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *break_file = ಖಾಲಿ; /* name of the 'Break chars' file */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *only_file = ಖಾಲಿ;	/* name of the 'Only words' file */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ignore_file = ಖಾಲಿ; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
ರಚನೆ regex_ದತ್ತಾಂಶ
{
  /* The original regular expression, as a string.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *string;

  /* The compiled regular expression, and its fastmap.  */
  ರಚನೆ re_ನಮೂನೆ_ಬಫರ್ ನಮೂನೆ_2;
  ಅಕ್ಷರ fastmap[UCHAR_MAX + 1];
};

ಸ್ಥಿರ ರಚನೆ regex_ದತ್ತಾಂಶ context_regex;	/* end of context */
ಸ್ಥಿರ ರಚನೆ regex_ದತ್ತಾಂಶ ಶಬ್ದ_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
  {
    ಅಕ್ಷರ *ಆರಂಭ;		/* pointer to beginning of region */
    ಅಕ್ಷರ *ಕೊನೆ;			/* pointer to end + 1 of region */
  }
ಖಂಡ_2;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
  {
    ಅಕ್ಷರ *ಆರಂಭ;		/* pointer to beginning of region */
    idx_t ಗಾತ್ರ_2;			/* length of the region */
  }
ಶಬ್ದ;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
  {
    idx_t alloc;		/* allocated length */
    idx_t ಉದ್ದಳತೆ;		/* number of used entries */
    ಶಬ್ದ *ಆರಂಭ COUNTED_BY (ಉದ್ದಳತೆ);	/* array of WORDs */
  }
ಶಬ್ದ_ಕೋಷ್ಟಕ;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ folded_chars[CHAR_ಗಣ_ಗಾತ್ರ];

/* End of context pattern register indices.  */
ಸ್ಥಿರ ರಚನೆ re_registers context_regs;

/* Keyword pattern register indices.  */
ಸ್ಥಿರ ರಚನೆ re_registers ಶಬ್ದ_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಶಬ್ದ_fastmap[CHAR_ಗಣ_ಗಾತ್ರ];

/* Maximum length of any word read.  */
ಸ್ಥಿರ idx_t ಗರಿಷ್ಠ_ಶಬ್ದ_ಉದ್ದಳತೆ;

/* Maximum width of any reference used.  */
ಸ್ಥಿರ idx_t reference_max_ಅಗಲ;

/* Ignore and Only word tables.  */

ಸ್ಥಿರ ಶಬ್ದ_ಕೋಷ್ಟಕ ignore_ಕೋಷ್ಟಕ;	/* table of words to ignore */
ಸ್ಥಿರ ಶಬ್ದ_ಕೋಷ್ಟಕ only_ಕೋಷ್ಟಕ;		/* table of words to select */

/* Source text table, and scanning macros.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files;	/* number of text input files */
ಸ್ಥಿರ intmax_t ಒಟ್ಟು_ಸಾಲು_ಎಣಿಕೆ;	/* total number of lines seen so far */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **ಒಳಹರಿವು_file_name;	/* array of text input file names */
ಸ್ಥಿರ intmax_t *file_ಸಾಲು_ಎಣಿಕೆ;	/* array of line count values at end */

ಸ್ಥಿರ ಖಂಡ_2 *ಪಠ್ಯ_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#ವ್ಯಾಖ್ಯಾನಿಸು SKIP_NON_WHITE(cursor, ಎಲ್ಲೆ) \
  ಆಗ (cursor < ಎಲ್ಲೆ && ! ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*cursor)))		\
    cursor++

#ವ್ಯಾಖ್ಯಾನಿಸು SKIP_WHITE(cursor, ಎಲ್ಲೆ) \
  ಆಗ (cursor < ಎಲ್ಲೆ && ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*cursor)))		\
    cursor++

#ವ್ಯಾಖ್ಯಾನಿಸು SKIP_WHITE_BACKWARDS(cursor, ಆರಂಭ) \
  ಆಗ (cursor > ಆರಂಭ && ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#ವ್ಯಾಖ್ಯಾನಿಸು SKIP_SOMETHING(cursor, ಎಲ್ಲೆ) \
  ಆದರೆ (ಶಬ್ದ_regex.string)						\
    {									\
      regoff_t ಎಣಿಕೆ_2;							\
      ಎಣಿಕೆ_2 = re_ಹೊಂದಿಕೆ (&ಶಬ್ದ_regex.ನಮೂನೆ_2, cursor, ಎಲ್ಲೆ - cursor,	\
                        0, ಖಾಲಿ);					\
      ಆದರೆ (ಎಣಿಕೆ_2 == -2)							\
        matcher_error ();						\
      cursor += ಎಣಿಕೆ_2 <= 0 ? 1 : ಎಣಿಕೆ_2;					\
    }									\
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಶಬ್ದ_fastmap[ಗಮ್ಯ_uchar (*cursor)])				\
    ಆಗ (cursor < ಎಲ್ಲೆ && ಶಬ್ದ_fastmap[ಗಮ್ಯ_uchar (*cursor)])		\
      cursor++;								\
  ಇಲ್ಲದಿದ್ದರೆ									\
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

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
  {
    ಶಬ್ದ ಕೀಲಿ;			/* description of the keyword */
    ptrdiff_t ಎಡ;		/* distance to left context start */
    ptrdiff_t ಬಲ;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    ಪೂರ್ಣಾಂಕ file_ಸೂಚ್ಯಂಕ;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

ಸ್ಥಿರ OCCURS *occurs_ಕೋಷ್ಟಕ[1];	/* all words retained from the read text */
ಸ್ಥಿರ idx_t occurs_alloc[1];	/* allocated size of occurs_table */
ಸ್ಥಿರ idx_t ಸಂಖ್ಯೆ_ದ_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
ಸ್ಥಿರ ಅಕ್ಷರ edited_flag[CHAR_ಗಣ_ಗಾತ್ರ];

/* Half of line width, reference excluded.  */
ಸ್ಥಿರ idx_t half_ಸಾಲು_ಅಗಲ;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ಸ್ಥಿರ ptrdiff_t before_max_ಅಗಲ;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ಸ್ಥಿರ ptrdiff_t keyafter_max_ಅಗಲ;

/* Length of string that flags truncation.  */
ಸ್ಥಿರ idx_t truncation_string_ಉದ್ದಳತೆ;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

ಸ್ಥಿರ ಖಂಡ_2 ಬಾಲ;		/* tail field */
ಸ್ಥಿರ bool ಬಾಲ_truncation;	/* flag truncation after the tail field */

ಸ್ಥಿರ ಖಂಡ_2 before;		/* before field */
ಸ್ಥಿರ bool before_truncation;	/* flag truncation before the before field */

ಸ್ಥಿರ ಖಂಡ_2 keyafter;		/* keyword-and-after field */
ಸ್ಥಿರ bool keyafter_truncation; /* flag truncation after the keyafter field */

ಸ್ಥಿರ ಖಂಡ_2 ತಲೆ;		/* head field */
ಸ್ಥಿರ bool ತಲೆ_truncation;	/* flag truncation before the head field */

ಸ್ಥಿರ ಖಂಡ_2 reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
matcher_error (ಶೂನ್ಯ)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

ಸ್ಥಿರ ಶೂನ್ಯ
unescape_string (ಅಕ್ಷರ *string)
{
  ಅಕ್ಷರ *cursor;			/* cursor in result */
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;			/* value of \nnn escape */
  ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ;			/* length of \nnn escape */

  cursor = string;

  ಆಗ (*string)
    {
      ಆದರೆ (*string == '\\')
        {
          string++;
          ಆಯ್ಕೆ (*string)
            {
            ಸ್ಥಿತಿ 'x':		/* \xhhh escape, 3 chars maximum */
              ಮೌಲ್ಯ = 0;
              ಗಾಗಿ (ಉದ್ದಳತೆ = 0, string++;
                   ಉದ್ದಳತೆ < 3 && c_isxdigit (*string);
                   ಉದ್ದಳತೆ++, string++)
                ಮೌಲ್ಯ = ಮೌಲ್ಯ * 16 + fromhex (*string);
              ಆದರೆ (ಉದ್ದಳತೆ == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              ಇಲ್ಲದಿದ್ದರೆ
                *cursor++ = ಮೌಲ್ಯ;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '0':		/* \0ooo escape, 3 chars maximum */
              ಮೌಲ್ಯ = 0;
              ಗಾಗಿ (ಉದ್ದಳತೆ = 0, string++;
                   ಉದ್ದಳತೆ < 3 && isoct (*string);
                   ಉದ್ದಳತೆ++, string++)
                ಮೌಲ್ಯ = ಮೌಲ್ಯ * 8 + fromoct (*string);
              *cursor++ = ಮೌಲ್ಯ;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'c':		/* cancel the rest of the output */
              ಆಗ (*string)
                string++;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\0':		/* lone backslash at end of string */
              /* ignore it */
              ನಿಲ್ಲಿಸು;

            ಪೂರ್ವನಿಯೋಜಿತ:
              *cursor++ = '\\';
              *cursor++ = *string++;
              ನಿಲ್ಲಿಸು;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
compile_regex (ರಚನೆ regex_ದತ್ತಾಂಶ *regex)
{
  ರಚನೆ re_ನಮೂನೆ_ಬಫರ್ *ನಮೂನೆ_2 = &regex->ನಮೂನೆ_2;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *string = regex->string;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ_2;

  ನಮೂನೆ_2->ಬಫರ್ = ಖಾಲಿ;
  ನಮೂನೆ_2->allocated = 0;
  ನಮೂನೆ_2->fastmap = regex->fastmap;
  ನಮೂನೆ_2->translate = ignore_case ? folded_chars : ಖಾಲಿ;

  ಸಂದೇಶ_2 = re_compile_ನಮೂನೆ (string, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (string), ನಮೂನೆ_2);
  ಆದರೆ (ಸಂದೇಶ_2)
    error (EXIT_FAILURE, 0, _("%s (for regexp %s)"), ಸಂದೇಶ_2, quote (string));

  /* The fastmap should be compiled before 're_match'.  The following
     call is not mandatory, because 're_search' is always called sooner,
     and it compiles the fastmap if this has not been done yet.  */

  re_compile_fastmap (ನಮೂನೆ_2);
}

/*------------------------------------------------------------------------.
| This will initialize various tables for pattern match and compiles some |
| regexps.								  |
`------------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
initialize_regex (ಶೂನ್ಯ)
{
  /* Initialize the case folding table.  */

  ಆದರೆ (ignore_case)
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ character = 0; character < CHAR_ಗಣ_ಗಾತ್ರ; character++)
      folded_chars[character] = ದೊಡ್ಡ_ಅಕ್ಷರ (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  ಆದರೆ (context_regex.string)
    {
      ಆದರೆ (!*context_regex.string)
        context_regex.string = ಖಾಲಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (gnu_extensions && !ಒಳಹರಿವು_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  ಇಲ್ಲದಿದ್ದರೆ
    context_regex.string = "\n";

  ಆದರೆ (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  ಆದರೆ (ಶಬ್ದ_regex.string)
    compile_regex (&ಶಬ್ದ_regex);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!break_file)
    {
      ಆದರೆ (gnu_extensions)
        {

          /* Simulate \w+.  */

          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ character = 0; character < CHAR_ಗಣ_ಗಾತ್ರ; character++)
            ಶಬ್ದ_fastmap[character] = !! ಅಕ್ಷರವೇ (character);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {

          /* Simulate [^ \t\n]+.  */

          memset (ಶಬ್ದ_fastmap, 1, CHAR_ಗಣ_ಗಾತ್ರ);
          ಶಬ್ದ_fastmap[' '] = 0;
          ಶಬ್ದ_fastmap['\t'] = 0;
          ಶಬ್ದ_fastmap['\n'] = 0;
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

ಸ್ಥಿರ ಶೂನ್ಯ
swallow_file_ಒಳ_ಸ್ಮರಣೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_name, ಖಂಡ_2 *ಖಂಡ)
{
  size_t ಬಳಸಿದ_ಉದ್ದಳತೆ;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  ಆದರೆ (using_stdin)
    ಖಂಡ->ಆರಂಭ = fread_file (stdin, 0, &ಬಳಸಿದ_ಉದ್ದಳತೆ);
  ಇಲ್ಲದಿದ್ದರೆ
    ಖಂಡ->ಆರಂಭ = read_file (file_name, 0, &ಬಳಸಿದ_ಉದ್ದಳತೆ);

  ಆದರೆ (!ಖಂಡ->ಆರಂಭ)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  ಆದರೆ (using_stdin)
    clearerr (stdin);

  ಖಂಡ->ಕೊನೆ = ಖಂಡ->ಆರಂಭ + ಬಳಸಿದ_ಉದ್ದಳತೆ;
}

/* Sort and search routines.  */

/*--------------------------------------------------------------------------.
| Compare two words, FIRST and SECOND, and return 0 if they are identical.  |
| Return less than 0 if the first word goes before the second; return	    |
| greater than 0 if the first word goes after the second.		    |
|									    |
| If a word is indeed a prefix of the other, the shorter should go first.   |
`--------------------------------------------------------------------------*/

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compare_ಪದಗಳು (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *void_ಮೊದಲ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *void_ಎರಡನೇ)
{
  ಶಬ್ದ ಸ್ಥಿರಾಂಕ *ಮೊದಲ = void_ಮೊದಲ;
  ಶಬ್ದ ಸ್ಥಿರಾಂಕ *ಎರಡನೇ = void_ಎರಡನೇ;
  idx_t ಉದ್ದಳತೆ = MIN (ಮೊದಲ->ಗಾತ್ರ_2, ಎರಡನೇ->ಗಾತ್ರ_2);

  ಆದರೆ (ignore_case)
    {
      ಗಾಗಿ (idx_t counter = 0; counter < ಉದ್ದಳತೆ; counter++)
        {
          ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = (folded_chars[ಗಮ್ಯ_uchar (ಮೊದಲ->ಆರಂಭ[counter])]
                       - folded_chars[ಗಮ್ಯ_uchar (ಎರಡನೇ->ಆರಂಭ[counter])]);
          ಆದರೆ (ಮೌಲ್ಯ != 0)
            ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಗಾಗಿ (idx_t counter = 0; counter < ಉದ್ದಳತೆ; counter++)
        {
          ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = (ಗಮ್ಯ_uchar (ಮೊದಲ->ಆರಂಭ[counter])
                       - ಗಮ್ಯ_uchar (ಎರಡನೇ->ಆರಂಭ[counter]));
          ಆದರೆ (ಮೌಲ್ಯ != 0)
            ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
        }
    }

  ಹಿಂತಿರುಗಿ _GL_CMP (ಮೊದಲ->ಗಾತ್ರ_2, ಎರಡನೇ->ಗಾತ್ರ_2);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compare_occurs (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *void_ಮೊದಲ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *void_ಎರಡನೇ)
{
#ವ್ಯಾಖ್ಯಾನಿಸು ಮೊದಲ ((ಸ್ಥಿರಾಂಕ OCCURS *) void_ಮೊದಲ)
#ವ್ಯಾಖ್ಯಾನಿಸು ಎರಡನೇ ((ಸ್ಥಿರಾಂಕ OCCURS *) void_ಎರಡನೇ)
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;

  ಮೌಲ್ಯ = compare_ಪದಗಳು (&ಮೊದಲ->ಕೀಲಿ, &ಎರಡನೇ->ಕೀಲಿ);
  ಹಿಂತಿರುಗಿ (ಮೌಲ್ಯ ? ಮೌಲ್ಯ
          : _GL_CMP (ಮೊದಲ->ಕೀಲಿ.ಆರಂಭ, ಎರಡನೇ->ಕೀಲಿ.ಆರಂಭ));
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಮೊದಲ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಎರಡನೇ
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
ಹುಡುಕು_ಕೋಷ್ಟಕ (ಶಬ್ದ *ಶಬ್ದ_2, ಶಬ್ದ_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ)
{
  idx_t lo = 0;
  idx_t hi = ಕೋಷ್ಟಕ->ಉದ್ದಳತೆ;
  ಆಗ (lo < hi)
    {
      idx_t ಮಧ್ಯ = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = compare_ಪದಗಳು (ಶಬ್ದ_2, ಕೋಷ್ಟಕ->ಆರಂಭ + ಮಧ್ಯ);
      ಆದರೆ (ಮೌಲ್ಯ < 0)
        hi = ಮಧ್ಯ;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮೌಲ್ಯ > 0)
        lo = ಮಧ್ಯ + 1;
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ true;
    }
  ಹಿಂತಿರುಗಿ false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಂಗಡಿಸು_ಸಿಕ್ಕಿತು_occurs (ಶೂನ್ಯ)
{

  /* Only one language for the time being.  */
  ಆದರೆ (ಸಂಖ್ಯೆ_ದ_occurs[0])
    qsort (occurs_ಕೋಷ್ಟಕ[0], ಸಂಖ್ಯೆ_ದ_occurs[0], ಗಾತ್ರ **occurs_ಕೋಷ್ಟಕ,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
digest_break_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_name)
{
  ಖಂಡ_2 file_contents;		/* to receive a copy of the file */

  swallow_file_ಒಳ_ಸ್ಮರಣೆ (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (ಶಬ್ದ_fastmap, 1, CHAR_ಗಣ_ಗಾತ್ರ);
  ಗಾಗಿ (ಅಕ್ಷರ *cursor = file_contents.ಆರಂಭ; cursor < file_contents.ಕೊನೆ;
       cursor++)
    ಶಬ್ದ_fastmap[ಗಮ್ಯ_uchar (*cursor)] = 0;

  ಆದರೆ (!gnu_extensions)
    {

      /* If GNU extensions are enabled, the only way to avoid newline as
         a break character is to write all the break characters in the
         file with no newline at all, not even at the end of the file.
         If disabled, spaces, tabs and newlines are always considered as
         break characters even if not included in the break file.  */

      ಶಬ್ದ_fastmap[' '] = 0;
      ಶಬ್ದ_fastmap['\t'] = 0;
      ಶಬ್ದ_fastmap['\n'] = 0;
    }

  /* Return the space of the file, which is no more required.  */

  ಬಿಡುಗಡೆ (file_contents.ಆರಂಭ);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
digest_ಶಬ್ದ_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_name, ಶಬ್ದ_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ)
{
  ಖಂಡ_2 file_contents;		/* to receive a copy of the file */
  ಅಕ್ಷರ *cursor;			/* cursor in file copy */
  ಅಕ್ಷರ *ಶಬ್ದ_ಆರಂಭ;		/* start of the current word */

  swallow_file_ಒಳ_ಸ್ಮರಣೆ (file_name, &file_contents);

  ಕೋಷ್ಟಕ->ಆರಂಭ = ಖಾಲಿ;
  ಕೋಷ್ಟಕ->alloc = 0;
  ಕೋಷ್ಟಕ->ಉದ್ದಳತೆ = 0;

  /* Read the whole file.  */

  cursor = file_contents.ಆರಂಭ;
  ಆಗ (cursor < file_contents.ಕೊನೆ)
    {

      /* Read one line, and save the word in contains.  */

      ಶಬ್ದ_ಆರಂಭ = cursor;
      ಆಗ (cursor < file_contents.ಕೊನೆ && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      ಆದರೆ (cursor > ಶಬ್ದ_ಆರಂಭ)
        {
          ಆದರೆ (ಕೋಷ್ಟಕ->ಉದ್ದಳತೆ == ಕೋಷ್ಟಕ->alloc)
            ಕೋಷ್ಟಕ->ಆರಂಭ = xpalloc (ಕೋಷ್ಟಕ->ಆರಂಭ, &ಕೋಷ್ಟಕ->alloc, 1, -1,
                                    ಗಾತ್ರ *ಕೋಷ್ಟಕ->ಆರಂಭ);
          idx_t ಸ್ಥಿರಾಂಕ ಕೋಷ್ಟಕ_idx = ಕೋಷ್ಟಕ->ಉದ್ದಳತೆ++;
          ಕೋಷ್ಟಕ->ಆರಂಭ[ಕೋಷ್ಟಕ_idx].ಆರಂಭ = ಶಬ್ದ_ಆರಂಭ;
          ಕೋಷ್ಟಕ->ಆರಂಭ[ಕೋಷ್ಟಕ_idx].ಗಾತ್ರ_2 = cursor - ಶಬ್ದ_ಆರಂಭ;
        }

      /* This test allows for an incomplete line at end of file.  */

      ಆದರೆ (cursor < file_contents.ಕೊನೆ)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (ಕೋಷ್ಟಕ->ಆರಂಭ, ಕೋಷ್ಟಕ->ಉದ್ದಳತೆ, ಗಾತ್ರ ಕೋಷ್ಟಕ->ಆರಂಭ[0], compare_ಪದಗಳು);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಕಂಡುಹಿಡಿ_occurs_ಒಳ_ಪಠ್ಯ (ಪೂರ್ಣಾಂಕ file_ಸೂಚ್ಯಂಕ)
{
  ಅಕ್ಷರ *ಪರಿಶೀಲನೆ;			/* for scanning the source text */
  ಅಕ್ಷರ *ಸಾಲು_ಆರಂಭ;		/* start of the current input line */
  ಅಕ್ಷರ *ಸಾಲು_ಪರಿಶೀಲನೆ;		/* newlines scanned until this point */
  idx_t reference_ಉದ್ದಳತೆ;	/* length of reference in input mode */
  ಶಬ್ದ possible_ಕೀಲಿ;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  ಅಕ್ಷರ *context_ಆರಂಭ;		/* start of left context */
  ಅಕ್ಷರ *context_ಕೊನೆ;		/* end of right context */
  ಅಕ್ಷರ *ಶಬ್ದ_ಆರಂಭ;		/* start of word */
  ಅಕ್ಷರ *ಶಬ್ದ_ಕೊನೆ;		/* end of word */
  ಅಕ್ಷರ *ಮುಂದಿನ_context_ಆರಂಭ;	/* next start of left context */

  ಸ್ಥಿರಾಂಕ ಖಂಡ_2 *ಪಠ್ಯ_ಬಫರ್ = &ಪಠ್ಯ_buffers[file_ಸೂಚ್ಯಂಕ];

  /* reference_length is always used within 'if (input_reference)'.
     However, GNU C diagnoses that it may be used uninitialized.  The
     following assignment is merely to shut it up.  */

  reference_ಉದ್ದಳತೆ = 0;

  /* Tracking where lines start is helpful for reference processing.  In
     auto reference mode, this allows counting lines.  In input reference
     mode, this permits finding the beginning of the references.

     The first line begins with the file, skip immediately this very first
     reference in input reference mode, to help further rejection any word
     found inside it.  Also, unconditionally assigning these variable has
     the happy effect of shutting up lint.  */

  ಸಾಲು_ಆರಂಭ = ಪಠ್ಯ_ಬಫರ್->ಆರಂಭ;
  ಸಾಲು_ಪರಿಶೀಲನೆ = ಸಾಲು_ಆರಂಭ;
  ಆದರೆ (ಒಳಹರಿವು_reference)
    {
      SKIP_NON_WHITE (ಸಾಲು_ಪರಿಶೀಲನೆ, ಪಠ್ಯ_ಬಫರ್->ಕೊನೆ);
      reference_ಉದ್ದಳತೆ = ಸಾಲು_ಪರಿಶೀಲನೆ - ಸಾಲು_ಆರಂಭ;
      SKIP_WHITE (ಸಾಲು_ಪರಿಶೀಲನೆ, ಪಠ್ಯ_ಬಫರ್->ಕೊನೆ);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  ಗಾಗಿ (ಅಕ್ಷರ *cursor = ಪಠ್ಯ_ಬಫರ್->ಆರಂಭ;
       cursor < ಪಠ್ಯ_ಬಫರ್->ಕೊನೆ;
       cursor = ಮುಂದಿನ_context_ಆರಂಭ)
    {

      /* 'context_start' gets initialized before the processing of each
         line, or once for the whole buffer if no end of line or sentence
         sequence separator.  */

      context_ಆರಂಭ = cursor;

      /* If an end of line or end of sentence sequence is defined and
         non-empty, 'next_context_start' will be recomputed to be the end of
         each line or sentence, before each one is processed.  If no such
         sequence, then 'next_context_start' is set at the end of the whole
         buffer, which is then considered to be a single line or sentence.
         This test also accounts for the case of an incomplete line or
         sentence at the end of the buffer.  */

      ಮುಂದಿನ_context_ಆರಂಭ = ಪಠ್ಯ_ಬಫರ್->ಕೊನೆ;
      ಆದರೆ (context_regex.string)
        ಆಯ್ಕೆ (re_ಹುಡುಕು (&context_regex.ನಮೂನೆ_2, cursor,
                           ಪಠ್ಯ_ಬಫರ್->ಕೊನೆ - cursor,
                           0, ಪಠ್ಯ_ಬಫರ್->ಕೊನೆ - cursor, &context_regs))
          {
          ಸ್ಥಿತಿ -2:
            matcher_error ();

          ಸ್ಥಿತಿ -1:
            ನಿಲ್ಲಿಸು;

          ಸ್ಥಿತಿ 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          ಪೂರ್ವನಿಯೋಜಿತ:
            ಮುಂದಿನ_context_ಆರಂಭ = cursor + context_regs.ಕೊನೆ[0];
            ನಿಲ್ಲಿಸು;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_ಕೊನೆ = ಮುಂದಿನ_context_ಆರಂಭ;
      SKIP_WHITE_BACKWARDS (context_ಕೊನೆ, context_ಆರಂಭ);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      ಆಗ (true)
        {
          ಆದರೆ (ಶಬ್ದ_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_ಹುಡುಕು (&ಶಬ್ದ_regex.ನಮೂನೆ_2, cursor,
                                      context_ಕೊನೆ - cursor,
                                      0, context_ಕೊನೆ - cursor, &ಶಬ್ದ_regs);
              ಆದರೆ (r == -2)
                matcher_error ();
              ಆದರೆ (r == -1)
                ನಿಲ್ಲಿಸು;
              ಶಬ್ದ_ಆರಂಭ = cursor + ಶಬ್ದ_regs.ಆರಂಭ[0];
              ಶಬ್ದ_ಕೊನೆ = cursor + ಶಬ್ದ_regs.ಕೊನೆ[0];
            }
          ಇಲ್ಲದಿದ್ದರೆ

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              ಪರಿಶೀಲನೆ = cursor;
              ಆಗ (ಪರಿಶೀಲನೆ < context_ಕೊನೆ
                     && !ಶಬ್ದ_fastmap[ಗಮ್ಯ_uchar (*ಪರಿಶೀಲನೆ)])
                ಪರಿಶೀಲನೆ++;

              ಆದರೆ (ಪರಿಶೀಲನೆ == context_ಕೊನೆ)
                ನಿಲ್ಲಿಸು;

              ಶಬ್ದ_ಆರಂಭ = ಪರಿಶೀಲನೆ;

              ಆಗ (ಪರಿಶೀಲನೆ < context_ಕೊನೆ
                     && ಶಬ್ದ_fastmap[ಗಮ್ಯ_uchar (*ಪರಿಶೀಲನೆ)])
                ಪರಿಶೀಲನೆ++;

              ಶಬ್ದ_ಕೊನೆ = ಪರಿಶೀಲನೆ;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = ಶಬ್ದ_ಆರಂಭ;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          ಆದರೆ (ಶಬ್ದ_ಕೊನೆ == ಶಬ್ದ_ಆರಂಭ)
            {
              cursor++;
              ಮುಂದುವರಿ;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_ಕೀಲಿ.ಆರಂಭ = cursor;
          possible_ಕೀಲಿ.ಗಾತ್ರ_2 = ಶಬ್ದ_ಕೊನೆ - ಶಬ್ದ_ಆರಂಭ;
          cursor += possible_ಕೀಲಿ.ಗಾತ್ರ_2;

          ಆದರೆ (possible_ಕೀಲಿ.ಗಾತ್ರ_2 > ಗರಿಷ್ಠ_ಶಬ್ದ_ಉದ್ದಳತೆ)
            ಗರಿಷ್ಠ_ಶಬ್ದ_ಉದ್ದಳತೆ = possible_ಕೀಲಿ.ಗಾತ್ರ_2;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          ಆದರೆ (ಒಳಹರಿವು_reference)
            {
              ಆಗ (ಸಾಲು_ಪರಿಶೀಲನೆ < possible_ಕೀಲಿ.ಆರಂಭ)
                ಆದರೆ (*ಸಾಲು_ಪರಿಶೀಲನೆ == '\n')
                  {
                    ಒಟ್ಟು_ಸಾಲು_ಎಣಿಕೆ++;
                    ಸಾಲು_ಪರಿಶೀಲನೆ++;
                    ಸಾಲು_ಆರಂಭ = ಸಾಲು_ಪರಿಶೀಲನೆ;
                    SKIP_NON_WHITE (ಸಾಲು_ಪರಿಶೀಲನೆ, ಪಠ್ಯ_ಬಫರ್->ಕೊನೆ);
                    reference_ಉದ್ದಳತೆ = ಸಾಲು_ಪರಿಶೀಲನೆ - ಸಾಲು_ಆರಂಭ;
                  }
                ಇಲ್ಲದಿದ್ದರೆ
                  ಸಾಲು_ಪರಿಶೀಲನೆ++;
              ಆದರೆ (ಸಾಲು_ಪರಿಶೀಲನೆ > possible_ಕೀಲಿ.ಆರಂಭ)
                ಮುಂದುವರಿ;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          ಆದರೆ (ignore_file && ಹುಡುಕು_ಕೋಷ್ಟಕ (&possible_ಕೀಲಿ, &ignore_ಕೋಷ್ಟಕ))
            ಮುಂದುವರಿ;
          ಆದರೆ (only_file && !ಹುಡುಕು_ಕೋಷ್ಟಕ (&possible_ಕೀಲಿ, &only_ಕೋಷ್ಟಕ))
            ಮುಂದುವರಿ;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          ಆದರೆ (ಸಂಖ್ಯೆ_ದ_occurs[0] == occurs_alloc[0])
            occurs_ಕೋಷ್ಟಕ[0] = xpalloc (occurs_ಕೋಷ್ಟಕ[0], &occurs_alloc[0],
                                       1, -1, ಗಾತ್ರ *occurs_ಕೋಷ್ಟಕ[0]);
          occurs_cursor = occurs_ಕೋಷ್ಟಕ[0] + ಸಂಖ್ಯೆ_ದ_occurs[0];

          /* Define the reference field, if any.  */

          ಆದರೆ (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              ಆಗ (ಸಾಲು_ಪರಿಶೀಲನೆ < possible_ಕೀಲಿ.ಆರಂಭ)
                ಆದರೆ (*ಸಾಲು_ಪರಿಶೀಲನೆ == '\n')
                  {
                    ಒಟ್ಟು_ಸಾಲು_ಎಣಿಕೆ++;
                    ಸಾಲು_ಪರಿಶೀಲನೆ++;
                    ಸಾಲು_ಆರಂಭ = ಸಾಲು_ಪರಿಶೀಲನೆ;
                    SKIP_NON_WHITE (ಸಾಲು_ಪರಿಶೀಲನೆ, ಪಠ್ಯ_ಬಫರ್->ಕೊನೆ);
                  }
                ಇಲ್ಲದಿದ್ದರೆ
                  ಸಾಲು_ಪರಿಶೀಲನೆ++;

              occurs_cursor->reference = ಒಟ್ಟು_ಸಾಲು_ಎಣಿಕೆ;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಳಹರಿವು_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = ಸಾಲು_ಆರಂಭ - possible_ಕೀಲಿ.ಆರಂಭ;
              ಆದರೆ (reference_ಉದ್ದಳತೆ > reference_max_ಅಗಲ)
                reference_max_ಅಗಲ = reference_ಉದ್ದಳತೆ;
            }

          /* Exclude the reference from the context in simple cases.  */

          ಆದರೆ (ಒಳಹರಿವು_reference && ಸಾಲು_ಆರಂಭ == context_ಆರಂಭ)
            {
              SKIP_NON_WHITE (context_ಆರಂಭ, context_ಕೊನೆ);
              SKIP_WHITE (context_ಆರಂಭ, context_ಕೊನೆ);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->ಕೀಲಿ = possible_ಕೀಲಿ;
          occurs_cursor->ಎಡ = context_ಆರಂಭ - possible_ಕೀಲಿ.ಆರಂಭ;
          occurs_cursor->ಬಲ = context_ಕೊನೆ - possible_ಕೀಲಿ.ಆರಂಭ;
          occurs_cursor->file_ಸೂಚ್ಯಂಕ = file_ಸೂಚ್ಯಂಕ;

          ಸಂಖ್ಯೆ_ದ_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_spaces (ptrdiff_t ಸಂಖ್ಯೆ)
{
  ಗಾಗಿ (ptrdiff_t counter = ಸಂಖ್ಯೆ; counter > 0; counter--)
    ಅಕ್ಷರ_ಬರೆ (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_ಕ್ಷೇತ್ರ (ಖಂಡ_2 ಕ್ಷೇತ್ರ)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  ಗಾಗಿ (ಅಕ್ಷರ *cursor = ಕ್ಷೇತ್ರ.ಆರಂಭ; cursor < ಕ್ಷೇತ್ರ.ಕೊನೆ; cursor++)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ character = *cursor;
      ಆದರೆ (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          ಆಯ್ಕೆ (character)
            {
            ಸ್ಥಿತಿ '"':
              /* In roff output format, double any quote.  */
              ಅಕ್ಷರ_ಬರೆ ('"');
              ಅಕ್ಷರ_ಬರೆ ('"');
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '$':
            ಸ್ಥಿತಿ '%':
            ಸ್ಥಿತಿ '&':
            ಸ್ಥಿತಿ '#':
            ಸ್ಥಿತಿ '_':
              /* In TeX output format, precede these with a backslash.  */
              ಅಕ್ಷರ_ಬರೆ ('\\');
              ಅಕ್ಷರ_ಬರೆ (character);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '{':
            ಸ್ಥಿತಿ '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              ಬರೆ ("$\\%c$", character);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              ನಿಲ್ಲಿಸು;

            ಪೂರ್ವನಿಯೋಜಿತ:
              /* Any other flagged character produces a single space.  */
              ಅಕ್ಷರ_ಬರೆ (' ');
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಅಕ್ಷರ_ಬರೆ (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
fix_output_parameters (ಶೂನ್ಯ)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  ಆದರೆ (auto_reference)
    {
      reference_max_ಅಗಲ = 0;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ file_ಸೂಚ್ಯಂಕ = 0; file_ಸೂಚ್ಯಂಕ < ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files; file_ಸೂಚ್ಯಂಕ++)
        {
          intmax_t ಸಾಲು_ordinal = file_ಸಾಲು_ಎಣಿಕೆ[file_ಸೂಚ್ಯಂಕ] + 1;
          ಆದರೆ (file_ಸೂಚ್ಯಂಕ > 0)
            ಸಾಲು_ordinal -= file_ಸಾಲು_ಎಣಿಕೆ[file_ಸೂಚ್ಯಂಕ - 1];
          ಅಕ್ಷರ ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_ಅಗಲ = sprintf (ordinal_string, "%jd", ಸಾಲು_ordinal);
          ಆದರೆ (ಒಳಹರಿವು_file_name[file_ಸೂಚ್ಯಂಕ])
            reference_ಅಗಲ += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಒಳಹರಿವು_file_name[file_ಸೂಚ್ಯಂಕ]);
          ಆದರೆ (reference_ಅಗಲ > reference_max_ಅಗಲ)
            reference_max_ಅಗಲ = reference_ಅಗಲ;
        }
      reference_max_ಅಗಲ++;
      reference.ಆರಂಭ = xmalloc (reference_max_ಅಗಲ + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  ಆದರೆ ((auto_reference || ಒಳಹರಿವು_reference) && !ಬಲ_reference)
    ಸಾಲು_ಅಗಲ = MAX (0, ಸಾಲು_ಅಗಲ - (reference_max_ಅಗಲ + gap_ಗಾತ್ರ));

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

  half_ಸಾಲು_ಅಗಲ = ಸಾಲು_ಅಗಲ >> 1;
  before_max_ಅಗಲ = half_ಸಾಲು_ಅಗಲ - gap_ಗಾತ್ರ;
  keyafter_max_ಅಗಲ = half_ಸಾಲು_ಅಗಲ;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  ಆದರೆ (truncation_string && *truncation_string)
    truncation_string_ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (truncation_string);
  ಇಲ್ಲದಿದ್ದರೆ
    truncation_string = ಖಾಲಿ;

  ಆದರೆ (gnu_extensions)
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

      before_max_ಅಗಲ -= 2 * truncation_string_ಉದ್ದಳತೆ;
      keyafter_max_ಅಗಲ -= 2 * truncation_string_ಉದ್ದಳತೆ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_ಅಗಲ -= 2 * truncation_string_ಉದ್ದಳತೆ + 1;
    }

  ಆದರೆ (before_max_ಅಗಲ < 0)
    before_max_ಅಗಲ = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ character = 0; character < CHAR_ಗಣ_ಗಾತ್ರ; character++)
    edited_flag[character] = !! ಖಾಲಿಯೇ (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  ಆಯ್ಕೆ (output_format)
    {
    ಸ್ಥಿತಿ UNKNOWN_FORMAT:
      /* Should never happen.  */

    ಸ್ಥಿತಿ DUMB_FORMAT:
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ TEX_FORMAT:

      /* Various characters need special processing.  */

      ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[ಗಮ್ಯ_uchar (*cursor)] = 1;

      ನಿಲ್ಲಿಸು;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t ಬಾಲ_max_ಅಗಲ;	/* allowable width of tail field */
  ptrdiff_t ತಲೆ_max_ಅಗಲ;	/* allowable width of head field */
  ಅಕ್ಷರ *cursor;			/* running cursor in source text */
  ಅಕ್ಷರ *ಎಡ_context_ಆರಂಭ;	/* start of left context */
  ಅಕ್ಷರ *ಬಲ_context_ಕೊನೆ;	/* end of right context */
  ಅಕ್ಷರ *ಎಡ_ಕ್ಷೇತ್ರ_ಆರಂಭ;	/* conservative start for 'head'/'before' */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_name;	/* file name for reference */
  intmax_t ಸಾಲು_ordinal;	/* line ordinal for reference */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಫರ್_ಆರಂಭ;	/* start of buffered file for this occurs */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಫರ್_ಕೊನೆ;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.ಆರಂಭ = occurs->ಕೀಲಿ.ಆರಂಭ;
  keyafter.ಕೊನೆ = keyafter.ಆರಂಭ + occurs->ಕೀಲಿ.ಗಾತ್ರ_2;
  ಎಡ_context_ಆರಂಭ = keyafter.ಆರಂಭ + occurs->ಎಡ;
  ಬಲ_context_ಕೊನೆ = keyafter.ಆರಂಭ + occurs->ಬಲ;

  ಬಫರ್_ಆರಂಭ = ಪಠ್ಯ_buffers[occurs->file_ಸೂಚ್ಯಂಕ].ಆರಂಭ;
  ಬಫರ್_ಕೊನೆ = ಪಠ್ಯ_buffers[occurs->file_ಸೂಚ್ಯಂಕ].ಕೊನೆ;

  cursor = keyafter.ಕೊನೆ;
  ಆಗ (cursor < ಬಲ_context_ಕೊನೆ
         && cursor <= keyafter.ಆರಂಭ + keyafter_max_ಅಗಲ)
    {
      keyafter.ಕೊನೆ = cursor;
      SKIP_SOMETHING (cursor, ಬಲ_context_ಕೊನೆ);
    }
  ಆದರೆ (cursor <= keyafter.ಆರಂಭ + keyafter_max_ಅಗಲ)
    keyafter.ಕೊನೆ = cursor;

  keyafter_truncation = truncation_string && keyafter.ಕೊನೆ < ಬಲ_context_ಕೊನೆ;

  SKIP_WHITE_BACKWARDS (keyafter.ಕೊನೆ, keyafter.ಆರಂಭ);

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

  ಆದರೆ (-occurs->ಎಡ > half_ಸಾಲು_ಅಗಲ + ಗರಿಷ್ಠ_ಶಬ್ದ_ಉದ್ದಳತೆ)
    {
      ಎಡ_ಕ್ಷೇತ್ರ_ಆರಂಭ
        = keyafter.ಆರಂಭ - (half_ಸಾಲು_ಅಗಲ + ಗರಿಷ್ಠ_ಶಬ್ದ_ಉದ್ದಳತೆ);
      SKIP_SOMETHING (ಎಡ_ಕ್ಷೇತ್ರ_ಆರಂಭ, keyafter.ಆರಂಭ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಎಡ_ಕ್ಷೇತ್ರ_ಆರಂಭ = keyafter.ಆರಂಭ + occurs->ಎಡ;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.ಆರಂಭ = ಎಡ_ಕ್ಷೇತ್ರ_ಆರಂಭ;
  before.ಕೊನೆ = keyafter.ಆರಂಭ;
  SKIP_WHITE_BACKWARDS (before.ಕೊನೆ, before.ಆರಂಭ);

  ಆಗ (before.ಆರಂಭ + before_max_ಅಗಲ < before.ಕೊನೆ)
    SKIP_SOMETHING (before.ಆರಂಭ, before.ಕೊನೆ);

  ಆದರೆ (truncation_string)
    {
      cursor = before.ಆರಂಭ;
      SKIP_WHITE_BACKWARDS (cursor, ಬಫರ್_ಆರಂಭ);
      before_truncation = cursor > ಎಡ_context_ಆರಂಭ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    before_truncation = false;

  SKIP_WHITE (before.ಆರಂಭ, ಬಫರ್_ಕೊನೆ);

  /* The tail could not take more columns than what has been left in the
     left context field, and a gap is mandatory.  It starts after the
     right context, and does not contain prefixed spaces.  It ends at
     the end of line, the end of buffer or when the tail field is full,
     whichever comes first.  It cannot contain only part of a word, and
     has no suffixed spaces.  */

  ಬಾಲ_max_ಅಗಲ
    = before_max_ಅಗಲ - (before.ಕೊನೆ - before.ಆರಂಭ) - gap_ಗಾತ್ರ;

  ಆದರೆ (ಬಾಲ_max_ಅಗಲ > 0)
    {
      ಬಾಲ.ಆರಂಭ = keyafter.ಕೊನೆ;
      SKIP_WHITE (ಬಾಲ.ಆರಂಭ, ಬಫರ್_ಕೊನೆ);

      ಬಾಲ.ಕೊನೆ = ಬಾಲ.ಆರಂಭ;
      cursor = ಬಾಲ.ಕೊನೆ;
      ಆಗ (cursor < ಬಲ_context_ಕೊನೆ
             && cursor < ಬಾಲ.ಆರಂಭ + ಬಾಲ_max_ಅಗಲ)
        {
          ಬಾಲ.ಕೊನೆ = cursor;
          SKIP_SOMETHING (cursor, ಬಲ_context_ಕೊನೆ);
        }

      ಆದರೆ (cursor < ಬಾಲ.ಆರಂಭ + ಬಾಲ_max_ಅಗಲ)
        ಬಾಲ.ಕೊನೆ = cursor;

      ಆದರೆ (ಬಾಲ.ಕೊನೆ > ಬಾಲ.ಆರಂಭ)
        {
          keyafter_truncation = false;
          ಬಾಲ_truncation = truncation_string && ಬಾಲ.ಕೊನೆ < ಬಲ_context_ಕೊನೆ;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಬಾಲ_truncation = false;

      SKIP_WHITE_BACKWARDS (ಬಾಲ.ಕೊನೆ, ಬಾಲ.ಆರಂಭ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {

      /* No place left for a tail field.  */

      ಬಾಲ.ಆರಂಭ = ಖಾಲಿ;
      ಬಾಲ.ಕೊನೆ = ಖಾಲಿ;
      ಬಾಲ_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  ತಲೆ_max_ಅಗಲ
    = keyafter_max_ಅಗಲ - (keyafter.ಕೊನೆ - keyafter.ಆರಂಭ) - gap_ಗಾತ್ರ;

  ಆದರೆ (ತಲೆ_max_ಅಗಲ > 0)
    {
      ತಲೆ.ಕೊನೆ = before.ಆರಂಭ;
      SKIP_WHITE_BACKWARDS (ತಲೆ.ಕೊನೆ, ಬಫರ್_ಆರಂಭ);

      ತಲೆ.ಆರಂಭ = ಎಡ_ಕ್ಷೇತ್ರ_ಆರಂಭ;
      ಆಗ (ತಲೆ.ಆರಂಭ + ತಲೆ_max_ಅಗಲ < ತಲೆ.ಕೊನೆ)
        SKIP_SOMETHING (ತಲೆ.ಆರಂಭ, ತಲೆ.ಕೊನೆ);

      ಆದರೆ (ತಲೆ.ಕೊನೆ > ತಲೆ.ಆರಂಭ)
        {
          before_truncation = false;
          ತಲೆ_truncation = (truncation_string
                             && ತಲೆ.ಆರಂಭ > ಎಡ_context_ಆರಂಭ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ತಲೆ_truncation = false;

      SKIP_WHITE (ತಲೆ.ಆರಂಭ, ತಲೆ.ಕೊನೆ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {

      /* No place left for a head field.  */

      ತಲೆ.ಆರಂಭ = ಖಾಲಿ;
      ತಲೆ.ಕೊನೆ = ಖಾಲಿ;
      ತಲೆ_truncation = false;
    }

  ಆದರೆ (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = ಒಳಹರಿವು_file_name[occurs->file_ಸೂಚ್ಯಂಕ];
      ಆದರೆ (!file_name)
        file_name = "";

      ಸಾಲು_ordinal = occurs->reference + 1;
      ಆದರೆ (occurs->file_ಸೂಚ್ಯಂಕ > 0)
        ಸಾಲು_ordinal -= file_ಸಾಲು_ಎಣಿಕೆ[occurs->file_ಸೂಚ್ಯಂಕ - 1];

      ಅಕ್ಷರ *file_ಕೊನೆ = stpcpy (reference.ಆರಂಭ, file_name);
      reference.ಕೊನೆ = file_ಕೊನೆ + sprintf (file_ಕೊನೆ, ":%jd", ಸಾಲು_ordinal);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಳಹರಿವು_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.ಆರಂಭ = keyafter.ಆರಂಭ + occurs->reference;
      reference.ಕೊನೆ = reference.ಆರಂಭ;
      SKIP_NON_WHITE (reference.ಕೊನೆ, ಬಲ_context_ಕೊನೆ);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
output_one_roff_ಸಾಲು (ಶೂನ್ಯ)
{
  /* Output the 'tail' field.  */

  ಬರೆ (".%s \"", macro_name);
  print_ಕ್ಷೇತ್ರ (ಬಾಲ);
  ಆದರೆ (ಬಾಲ_truncation)
    fputs (truncation_string, stdout);
  ಅಕ್ಷರ_ಬರೆ ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  ಆದರೆ (before_truncation)
    fputs (truncation_string, stdout);
  print_ಕ್ಷೇತ್ರ (before);
  ಅಕ್ಷರ_ಬರೆ ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_ಕ್ಷೇತ್ರ (keyafter);
  ಆದರೆ (keyafter_truncation)
    fputs (truncation_string, stdout);
  ಅಕ್ಷರ_ಬರೆ ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  ಆದರೆ (ತಲೆ_truncation)
    fputs (truncation_string, stdout);
  print_ಕ್ಷೇತ್ರ (ತಲೆ);
  ಅಕ್ಷರ_ಬರೆ ('"');

  /* Conditionally output the 'reference' field.  */

  ಆದರೆ (auto_reference || ಒಳಹರಿವು_reference)
    {
      fputs (" \"", stdout);
      print_ಕ್ಷೇತ್ರ (reference);
      ಅಕ್ಷರ_ಬರೆ ('"');
    }

  ಅಕ್ಷರ_ಬರೆ ('\n');
}

/*---------------------------------------------------------.
| Output the current output fields as one line for 'TeX'.  |
`---------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
output_one_tex_ಸಾಲು (ಶೂನ್ಯ)
{
  ಖಂಡ_2 ಕೀಲಿ;			/* key field, isolated */
  ಖಂಡ_2 after;			/* after field, isolated */
  ಅಕ್ಷರ *cursor;			/* running cursor in source text */

  ಬರೆ ("\\%s ", macro_name);
  ಅಕ್ಷರ_ಬರೆ ('{');
  print_ಕ್ಷೇತ್ರ (ಬಾಲ);
  fputs ("}{", stdout);
  print_ಕ್ಷೇತ್ರ (before);
  fputs ("}{", stdout);
  ಕೀಲಿ.ಆರಂಭ = keyafter.ಆರಂಭ;
  after.ಕೊನೆ = keyafter.ಕೊನೆ;
  cursor = keyafter.ಆರಂಭ;
  SKIP_SOMETHING (cursor, keyafter.ಕೊನೆ);
  ಕೀಲಿ.ಕೊನೆ = cursor;
  after.ಆರಂಭ = cursor;
  print_ಕ್ಷೇತ್ರ (ಕೀಲಿ);
  fputs ("}{", stdout);
  print_ಕ್ಷೇತ್ರ (after);
  fputs ("}{", stdout);
  print_ಕ್ಷೇತ್ರ (ತಲೆ);
  ಅಕ್ಷರ_ಬರೆ ('}');
  ಆದರೆ (auto_reference || ಒಳಹರಿವು_reference)
    {
      ಅಕ್ಷರ_ಬರೆ ('{');
      print_ಕ್ಷೇತ್ರ (reference);
      ಅಕ್ಷರ_ಬರೆ ('}');
    }
  ಅಕ್ಷರ_ಬರೆ ('\n');
}

/*-------------------------------------------------------------------.
| Output the current output fields as one line for a dumb terminal.  |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
output_one_dumb_ಸಾಲು (ಶೂನ್ಯ)
{
  ಆದರೆ (!ಬಲ_reference)
    {
      ಆದರೆ (auto_reference)
        {

          /* Output the 'reference' field, in such a way that GNU emacs
             next-error will handle it.  The ending colon is taken from the
             gap which follows.  */

          print_ಕ್ಷೇತ್ರ (reference);
          ಅಕ್ಷರ_ಬರೆ (':');
          print_spaces (reference_max_ಅಗಲ
                        + gap_ಗಾತ್ರ
                        - (reference.ಕೊನೆ - reference.ಆರಂಭ)
                        - 1);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {

          /* Output the 'reference' field and its following gap.  */

          print_ಕ್ಷೇತ್ರ (reference);
          print_spaces (reference_max_ಅಗಲ
                        + gap_ಗಾತ್ರ
                        - (reference.ಕೊನೆ - reference.ಆರಂಭ));
        }
    }

  ಆದರೆ (ಬಾಲ.ಆರಂಭ < ಬಾಲ.ಕೊನೆ)
    {
      /* Output the 'tail' field.  */

      print_ಕ್ಷೇತ್ರ (ಬಾಲ);
      ಆದರೆ (ಬಾಲ_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_ಸಾಲು_ಅಗಲ - gap_ಗಾತ್ರ
                    - (before.ಕೊನೆ - before.ಆರಂಭ)
                    - (before_truncation ? truncation_string_ಉದ್ದಳತೆ : 0)
                    - (ಬಾಲ.ಕೊನೆ - ಬಾಲ.ಆರಂಭ)
                    - (ಬಾಲ_truncation ? truncation_string_ಉದ್ದಳತೆ : 0));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    print_spaces (half_ಸಾಲು_ಅಗಲ - gap_ಗಾತ್ರ
                  - (before.ಕೊನೆ - before.ಆರಂಭ)
                  - (before_truncation ? truncation_string_ಉದ್ದಳತೆ : 0));

  /* Output the 'before' field.  */

  ಆದರೆ (before_truncation)
    fputs (truncation_string, stdout);
  print_ಕ್ಷೇತ್ರ (before);

  print_spaces (gap_ಗಾತ್ರ);

  /* Output the 'keyafter' field.  */

  print_ಕ್ಷೇತ್ರ (keyafter);
  ಆದರೆ (keyafter_truncation)
    fputs (truncation_string, stdout);

  ಆದರೆ (ತಲೆ.ಆರಂಭ < ತಲೆ.ಕೊನೆ)
    {
      /* Output the 'head' field.  */

      print_spaces (half_ಸಾಲು_ಅಗಲ
                    - (keyafter.ಕೊನೆ - keyafter.ಆರಂಭ)
                    - (keyafter_truncation ? truncation_string_ಉದ್ದಳತೆ : 0)
                    - (ತಲೆ.ಕೊನೆ - ತಲೆ.ಆರಂಭ)
                    - (ತಲೆ_truncation ? truncation_string_ಉದ್ದಳತೆ : 0));
      ಆದರೆ (ತಲೆ_truncation)
        fputs (truncation_string, stdout);
      print_ಕ್ಷೇತ್ರ (ತಲೆ);
    }
  ಇಲ್ಲದಿದ್ದರೆ

    ಆದರೆ ((auto_reference || ಒಳಹರಿವು_reference) && ಬಲ_reference)
      print_spaces (half_ಸಾಲು_ಅಗಲ
                    - (keyafter.ಕೊನೆ - keyafter.ಆರಂಭ)
                    - (keyafter_truncation ? truncation_string_ಉದ್ದಳತೆ : 0));

  ಆದರೆ ((auto_reference || ಒಳಹರಿವು_reference) && ಬಲ_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_ಗಾತ್ರ);
      print_ಕ್ಷೇತ್ರ (reference);
    }

  ಅಕ್ಷರ_ಬರೆ ('\n');
}

/*------------------------------------------------------------------------.
| Scan the whole occurs table and, for each entry, output one line in the |
| appropriate format.							  |
`------------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
generate_all_output (ಶೂನ್ಯ)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  ಬಾಲ.ಆರಂಭ = ಖಾಲಿ;
  ಬಾಲ.ಕೊನೆ = ಖಾಲಿ;
  ಬಾಲ_truncation = false;

  ತಲೆ.ಆರಂಭ = ಖಾಲಿ;
  ತಲೆ.ಕೊನೆ = ಖಾಲಿ;
  ತಲೆ_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_ಕೋಷ್ಟಕ[0];

  ಗಾಗಿ (idx_t occurs_ಸೂಚ್ಯಂಕ = 0; occurs_ಸೂಚ್ಯಂಕ < ಸಂಖ್ಯೆ_ದ_occurs[0];
       occurs_ಸೂಚ್ಯಂಕ++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      ಆಯ್ಕೆ (output_format)
        {
        ಸ್ಥಿತಿ UNKNOWN_FORMAT:
          /* Should never happen.  */

        ಸ್ಥಿತಿ DUMB_FORMAT:
          output_one_dumb_ಸಾಲು ();
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ROFF_FORMAT:
          output_one_roff_ಸಾಲು ();
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ TEX_FORMAT:
          output_one_tex_ಸಾಲು ();
          ನಿಲ್ಲಿಸು;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [ಒಳಹರಿವು_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [ಒಳಹರಿವು_2 [OUTPUT]]\n"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Output a permuted index, including context, ದ the ಪದಗಳು ಒಳ the ಒಳಹರಿವು files.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --ಸ್ವಯಂಚಾಲಿತ-reference           output automatically generated references\n\
"));
      oputs (_("\
  -G, --traditional              behave ಇನ್ನಷ್ಟು like System V 'ptx'\n\
"));
      oputs (_("\
  -F, --flag-truncation=STRING   use STRING ಗಾಗಿ flagging ಸಾಲು truncations.\n\
                                 The ಪೂರ್ವನಿಯೋಜಿತ is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name ಗಮ್ಯ use instead ದ 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --ಬಲ-side-refs          put references at ಬಲ, not counted ಒಳ -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   ಗಾಗಿ ಕೊನೆ ದ ಸಾಲುಗಳು or ಕೊನೆ ದ sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --ಶಬ್ದ_2-regexp=REGEXP       use REGEXP ಗಮ್ಯ ಹೊಂದಿಕೆ each keyword\n\
"));
      oputs (_("\
  -b, --ನಿಲ್ಲಿಸು-file=FILE          ಶಬ್ದ_2 ನಿಲ್ಲಿಸು characters ಒಳ this FILE\n\
"));
      oputs (_("\
  -f, --ignore-ಸ್ಥಿತಿ              fold ಅಧೋ ಸ್ಥಿತಿ ಗಮ್ಯ ಊರ್ಧ್ವ ಸ್ಥಿತಿ ಗಾಗಿ ವಿಂಗಡಣೆ\n\
"));
      oputs (_("\
  -g, --gap-ಗಾತ್ರ_2=ಸಂಖ್ಯೆ_2          gap ಗಾತ್ರ_2 ಒಳ columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         ಪಠ ignore ಶಬ್ದ_2 list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           ಪಠ only ಶಬ್ದ_2 list from this FILE\n\
"));
      oputs (_("\
  -r, --references               ಮೊದಲ ಕ್ಷೇತ್ರ ದ each ಸಾಲು is a reference\n\
"));
      oputs (_("\
  -t, --typeset-ಬಹುಲಕ_2             change the ಪೂರ್ವನಿಯೋಜಿತ ಅಗಲ from 72 ಗಮ್ಯ 100\n\
"));
      oputs (_("\
  -w, --ಅಗಲ=ಸಂಖ್ಯೆ_2             output ಅಗಲ ಒಳ columns, reference excluded\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/*----------------------------------------------------------------------.
| Main program.  Decode ARGC arguments passed through the ARGV array of |
| strings, then launch execution.				        |
`----------------------------------------------------------------------*/

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"auto-reference", no_ವಾದ, ಖಾಲಿ, 'A'},
  {"break-file", required_ವಾದ, ಖಾಲಿ, 'b'},
  {"flag-truncation", required_ವಾದ, ಖಾಲಿ, 'F'},
  {"ignore-case", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"gap-size", required_ವಾದ, ಖಾಲಿ, 'g'},
  {"ignore-file", required_ವಾದ, ಖಾಲಿ, 'i'},
  {"macro-name", required_ವಾದ, ಖಾಲಿ, 'M'},
  {"only-file", required_ವಾದ, ಖಾಲಿ, 'o'},
  {"references", no_ವಾದ, ಖಾಲಿ, 'r'},
  {"right-side-refs", no_ವಾದ, ಖಾಲಿ, 'R'},
  {"format", required_ವಾದ, ಖಾಲಿ, FORMAT_OPTION},
  {"sentence-regexp", required_ವಾದ, ಖಾಲಿ, 'S'},
  {"traditional", no_ವಾದ, ಖಾಲಿ, 'G'},
  {"typeset-mode", no_ವಾದ, ಖಾಲಿ, 't'},
  {"width", required_ವಾದ, ಖಾಲಿ, 'w'},
  {"word-regexp", required_ವಾದ, ಖಾಲಿ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0},
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ format_ವಾದಗಳು[] =
{
  "roff", "tex", ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ Format ಸ್ಥಿರಾಂಕ format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ optchar;			/* argument character */
  ಪೂರ್ಣಾಂಕ file_ಸೂಚ್ಯಂಕ;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, ಖಾಲಿ),
         optchar != EOF)
    {
      ಆಯ್ಕೆ (optchar)
        {
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);

        ಸ್ಥಿತಿ 'G':
          gnu_extensions = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'b':
          break_file = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
          ignore_case = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'g':
          {
            intmax_t tmp;
            ಆದರೆ (! (xstrtoimax (optarg, ಖಾಲಿ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_ಗಾತ್ರ = tmp;
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ 'i':
          ignore_file = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'o':
          only_file = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'r':
          ಒಳಹರಿವು_reference = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          ಆದರೆ (ಸಾಲು_ಅಗಲ < 0)
            ಸಾಲು_ಅಗಲ = 100;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'w':
          {
            intmax_t tmp;
            ಆದರೆ (! (xstrtoimax (optarg, ಖಾಲಿ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            ಸಾಲು_ಅಗಲ = tmp;
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ 'A':
          auto_reference = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'M':
          macro_name = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'O':
          output_format = ROFF_FORMAT;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'R':
          ಬಲ_reference = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'T':
          output_format = TEX_FORMAT;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'W':
          ಶಬ್ದ_regex.string = optarg;
          unescape_string (optarg);
          ಆದರೆ (!*ಶಬ್ದ_regex.string)
            ಶಬ್ದ_regex.string = ಖಾಲಿ;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_ವಾದಗಳು, format_vals);
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        }
    }

  ಆದರೆ (ಸಾಲು_ಅಗಲ < 0)
    ಸಾಲು_ಅಗಲ = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  ಆದರೆ (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      ಒಳಹರಿವು_file_name = xmalloc (ಗಾತ್ರ *ಒಳಹರಿವು_file_name);
      file_ಸಾಲು_ಎಣಿಕೆ = xmalloc (ಗಾತ್ರ *file_ಸಾಲು_ಎಣಿಕೆ);
      ಪಠ್ಯ_buffers =    xmalloc (ಗಾತ್ರ *ಪಠ್ಯ_buffers);
      ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files = 1;
      ಒಳಹರಿವು_file_name[0] = ಖಾಲಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (gnu_extensions)
    {
      ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files = argc - optind;
      ಒಳಹರಿವು_file_name = xnmalloc (ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files, ಗಾತ್ರ *ಒಳಹರಿವು_file_name);
      file_ಸಾಲು_ಎಣಿಕೆ = xnmalloc (ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files, ಗಾತ್ರ *file_ಸಾಲು_ಎಣಿಕೆ);
      ಪಠ್ಯ_buffers    = xnmalloc (ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files, ಗಾತ್ರ *ಪಠ್ಯ_buffers);

      ಗಾಗಿ (file_ಸೂಚ್ಯಂಕ = 0; file_ಸೂಚ್ಯಂಕ < ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files; file_ಸೂಚ್ಯಂಕ++)
        {
          ಆದರೆ (!*argv[optind] || streq (argv[optind], "-"))
            ಒಳಹರಿವು_file_name[file_ಸೂಚ್ಯಂಕ] = ಖಾಲಿ;
          ಇಲ್ಲದಿದ್ದರೆ
            ಒಳಹರಿವು_file_name[file_ಸೂಚ್ಯಂಕ] = argv[optind];
          optind++;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {

      /* There is one necessary input file.  */

      ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files = 1;
      ಒಳಹರಿವು_file_name = xmalloc (ಗಾತ್ರ *ಒಳಹರಿವು_file_name);
      file_ಸಾಲು_ಎಣಿಕೆ = xmalloc (ಗಾತ್ರ *file_ಸಾಲು_ಎಣಿಕೆ);
      ಪಠ್ಯ_buffers    = xmalloc (ಗಾತ್ರ *ಪಠ್ಯ_buffers);
      ಆದರೆ (!*argv[optind] || streq (argv[optind], "-"))
        ಒಳಹರಿವು_file_name[0] = ಖಾಲಿ;
      ಇಲ್ಲದಿದ್ದರೆ
        ಒಳಹರಿವು_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      ಆದರೆ (optind < argc)
        {
          ಆದರೆ (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      ಆದರೆ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  ಆದರೆ (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  ಆದರೆ (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  ಆದರೆ (ignore_file)
    {
      digest_ಶಬ್ದ_file (ignore_file, &ignore_ಕೋಷ್ಟಕ);
      ಆದರೆ (ignore_ಕೋಷ್ಟಕ.ಉದ್ದಳತೆ == 0)
        ignore_file = ಖಾಲಿ;
    }

  ಆದರೆ (only_file)
    {
      digest_ಶಬ್ದ_file (only_file, &only_ಕೋಷ್ಟಕ);
      ಆದರೆ (only_ಕೋಷ್ಟಕ.ಉದ್ದಳತೆ == 0)
        only_file = ಖಾಲಿ;
    }

  /* Prepare to study all the input files.  */

  ಸಂಖ್ಯೆ_ದ_occurs[0] = 0;
  ಒಟ್ಟು_ಸಾಲು_ಎಣಿಕೆ = 0;
  ಗರಿಷ್ಠ_ಶಬ್ದ_ಉದ್ದಳತೆ = 0;
  reference_max_ಅಗಲ = 0;

  ಗಾಗಿ (file_ಸೂಚ್ಯಂಕ = 0; file_ಸೂಚ್ಯಂಕ < ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files; file_ಸೂಚ್ಯಂಕ++)
    {
      ಖಂಡ_2 *ಪಠ್ಯ_ಬಫರ್ = ಪಠ್ಯ_buffers + file_ಸೂಚ್ಯಂಕ;

      /* Read the file contents into memory, then study it.  */

      swallow_file_ಒಳ_ಸ್ಮರಣೆ (ಒಳಹರಿವು_file_name[file_ಸೂಚ್ಯಂಕ], ಪಠ್ಯ_ಬಫರ್);
      ಕಂಡುಹಿಡಿ_occurs_ಒಳ_ಪಠ್ಯ (file_ಸೂಚ್ಯಂಕ);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      ಒಟ್ಟು_ಸಾಲು_ಎಣಿಕೆ++;
      file_ಸಾಲು_ಎಣಿಕೆ[file_ಸೂಚ್ಯಂಕ] = ಒಟ್ಟು_ಸಾಲು_ಎಣಿಕೆ;
    }

  /* Do the output process phase.  */

  ವಿಂಗಡಿಸು_ಸಿಕ್ಕಿತು_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
