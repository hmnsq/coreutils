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
#define PROGRAM_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#define AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#define CHAR_SET_SIZE 256

/* Debugging the memory allocator.  */

#ಯದಿ WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#endif

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

ಗಣನಾ Format
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
ಸ್ಥಿರ idx_t ಸಾಲು_width = -1;		/* output line width in characters */
ಸ್ಥಿರ idx_t gap_size = 3;	/* number of spaces between output fields */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *truncation_string = "/";
                                /* string used to mark line truncations */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *macro_name = "xx";	/* macro name for roff or TeX output */
ಸ್ಥಿರ ಗಣನಾ Format output_format = UNKNOWN_FORMAT;
                                /* output format */

ಸ್ಥಿರ bool ignore_case = false;	/* fold lower to upper for sorting */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *break_file = ರಿಕ್ತ; /* name of the 'Break chars' file */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *only_file = ರಿಕ್ತ;	/* name of the 'Only words' file */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ignore_file = ರಿಕ್ತ; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
ಸಂರಚನಾ regex_data
{
  /* The original regular expression, as a string.  */
  ವರ್ಣ ಧ್ರುವ *string;

  /* The compiled regular expression, and its fastmap.  */
  ಸಂರಚನಾ re_pattern_buffer pattern;
  ವರ್ಣ fastmap[UCHAR_MAX + 1];
};

ಸ್ಥಿರ ಸಂರಚನಾ regex_data context_regex;	/* end of context */
ಸ್ಥಿರ ಸಂರಚನಾ regex_data word_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
  {
    ವರ್ಣ *start;		/* pointer to beginning of region */
    ವರ್ಣ *end;			/* pointer to end + 1 of region */
  }
BLOCK;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
  {
    ವರ್ಣ *start;		/* pointer to beginning of region */
    idx_t size;			/* length of the region */
  }
WORD;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
  {
    idx_t alloc;		/* allocated length */
    idx_t length;		/* number of used entries */
    WORD *start COUNTED_BY (length);	/* array of WORDs */
  }
WORD_TABLE;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
ಸ್ಥಿರ unsigned ವರ್ಣ folded_chars[CHAR_SET_SIZE];

/* End of context pattern register indices.  */
ಸ್ಥಿರ ಸಂರಚನಾ re_registers context_regs;

/* Keyword pattern register indices.  */
ಸ್ಥಿರ ಸಂರಚನಾ re_registers word_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
ಸ್ಥಿರ ವರ್ಣ word_fastmap[CHAR_SET_SIZE];

/* Maximum length of any word read.  */
ಸ್ಥಿರ idx_t maximum_word_length;

/* Maximum width of any reference used.  */
ಸ್ಥಿರ idx_t reference_max_width;

/* Ignore and Only word tables.  */

ಸ್ಥಿರ WORD_TABLE ignore_table;	/* table of words to ignore */
ಸ್ಥಿರ WORD_TABLE only_table;		/* table of words to select */

/* Source text table, and scanning macros.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files;	/* number of text input files */
ಸ್ಥಿರ intmax_t total_ಸಾಲು_ಎಣಿಕೆ;	/* total number of lines seen so far */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ **ಒಳಹರಿವು_file_name;	/* array of text input file names */
ಸ್ಥಿರ intmax_t *file_ಸಾಲು_ಎಣಿಕೆ;	/* array of line count values at end */

ಸ್ಥಿರ BLOCK *text_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#define SKIP_NON_WHITE(cursor, limit) \
  ಯಾವತ್ (cursor < limit && ! isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE(cursor, limit) \
  ಯಾವತ್ (cursor < limit && isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE_BACKWARDS(cursor, start) \
  ಯಾವತ್ (cursor > start && isspace (to_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#define SKIP_SOMETHING(cursor, limit) \
  ಯದಿ (word_regex.string)						\
    {									\
      regoff_t ಎಣಿಕೆ;							\
      ಎಣಿಕೆ = re_match (&word_regex.pattern, cursor, limit - cursor,	\
                        0, ರಿಕ್ತ);					\
      ಯದಿ (ಎಣಿಕೆ == -2)							\
        matcher_error ();						\
      cursor += ಎಣಿಕೆ <= 0 ? 1 : ಎಣಿಕೆ;					\
    }									\
  ಅನ್ಯಥಾ ಯದಿ (word_fastmap[to_uchar (*cursor)])				\
    ಯಾವತ್ (cursor < limit && word_fastmap[to_uchar (*cursor)])		\
      cursor++;								\
  ಅನ್ಯಥಾ									\
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

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
  {
    WORD ಕೀಲಿ;			/* description of the keyword */
    ptrdiff_t ಎಡ;		/* distance to left context start */
    ptrdiff_t ಬಲ;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    ಪೂರ್ಣಾಙ್ಕ file_index;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

ಸ್ಥಿರ OCCURS *occurs_table[1];	/* all words retained from the read text */
ಸ್ಥಿರ idx_t occurs_alloc[1];	/* allocated size of occurs_table */
ಸ್ಥಿರ idx_t ಸಂಖ್ಯೆ_of_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
ಸ್ಥಿರ ವರ್ಣ edited_flag[CHAR_SET_SIZE];

/* Half of line width, reference excluded.  */
ಸ್ಥಿರ idx_t half_ಸಾಲು_width;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ಸ್ಥಿರ ptrdiff_t before_max_width;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ಸ್ಥಿರ ptrdiff_t keyafter_max_width;

/* Length of string that flags truncation.  */
ಸ್ಥಿರ idx_t truncation_string_length;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

ಸ್ಥಿರ BLOCK tail;		/* tail field */
ಸ್ಥಿರ bool tail_truncation;	/* flag truncation after the tail field */

ಸ್ಥಿರ BLOCK before;		/* before field */
ಸ್ಥಿರ bool before_truncation;	/* flag truncation before the before field */

ಸ್ಥಿರ BLOCK keyafter;		/* keyword-and-after field */
ಸ್ಥಿರ bool keyafter_truncation; /* flag truncation after the keyafter field */

ಸ್ಥಿರ BLOCK head;		/* head field */
ಸ್ಥಿರ bool head_truncation;	/* flag truncation before the head field */

ಸ್ಥಿರ BLOCK reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
matcher_error (ಶೂನ್ಯ)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

ಸ್ಥಿರ ಶೂನ್ಯ
unescape_string (ವರ್ಣ *string)
{
  ವರ್ಣ *cursor;			/* cursor in result */
  ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;			/* value of \nnn escape */
  ಪೂರ್ಣಾಙ್ಕ length;			/* length of \nnn escape */

  cursor = string;

  ಯಾವತ್ (*string)
    {
      ಯದಿ (*string == '\\')
        {
          string++;
          ವಿಕಲ್ಪ (*string)
            {
            ಪಕ್ಷ 'x':		/* \xhhh escape, 3 chars maximum */
              ಮೌಲ್ಯ = 0;
              ಪ್ರತಿ (length = 0, string++;
                   length < 3 && c_isxdigit (*string);
                   length++, string++)
                ಮೌಲ್ಯ = ಮೌಲ್ಯ * 16 + fromhex (*string);
              ಯದಿ (length == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              ಅನ್ಯಥಾ
                *cursor++ = ಮೌಲ್ಯ;
              ಭಙ್ಗ;

            ಪಕ್ಷ '0':		/* \0ooo escape, 3 chars maximum */
              ಮೌಲ್ಯ = 0;
              ಪ್ರತಿ (length = 0, string++;
                   length < 3 && isoct (*string);
                   length++, string++)
                ಮೌಲ್ಯ = ಮೌಲ್ಯ * 8 + fromoct (*string);
              *cursor++ = ಮೌಲ್ಯ;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'c':		/* cancel the rest of the output */
              ಯಾವತ್ (*string)
                string++;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              ಭಙ್ಗ;

            ಪಕ್ಷ 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              ಭಙ್ಗ;

            ಪಕ್ಷ '\0':		/* lone backslash at end of string */
              /* ignore it */
              ಭಙ್ಗ;

            ಸ್ವಭಾವ:
              *cursor++ = '\\';
              *cursor++ = *string++;
              ಭಙ್ಗ;
            }
        }
      ಅನ್ಯಥಾ
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
compile_regex (ಸಂರಚನಾ regex_data *regex)
{
  ಸಂರಚನಾ re_pattern_buffer *pattern = &regex->pattern;
  ವರ್ಣ ಧ್ರುವ *string = regex->string;
  ವರ್ಣ ಧ್ರುವ *message;

  pattern->buffer = ರಿಕ್ತ;
  pattern->allocated = 0;
  pattern->fastmap = regex->fastmap;
  pattern->translate = ignore_case ? folded_chars : ರಿಕ್ತ;

  message = re_compile_pattern (string, strlen (string), pattern);
  ಯದಿ (message)
    error (EXIT_FAILURE, 0, _("%s (for regexp %s)"), message, quote (string));

  /* The fastmap should be compiled before 're_match'.  The following
     call is not mandatory, because 're_search' is always called sooner,
     and it compiles the fastmap if this has not been done yet.  */

  re_compile_fastmap (pattern);
}

/*------------------------------------------------------------------------.
| This will initialize various tables for pattern match and compiles some |
| regexps.								  |
`------------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
initialize_regex (ಶೂನ್ಯ)
{
  /* Initialize the case folding table.  */

  ಯದಿ (ignore_case)
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ character = 0; character < CHAR_SET_SIZE; character++)
      folded_chars[character] = toupper (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  ಯದಿ (context_regex.string)
    {
      ಯದಿ (!*context_regex.string)
        context_regex.string = ರಿಕ್ತ;
    }
  ಅನ್ಯಥಾ ಯದಿ (gnu_extensions && !ಒಳಹರಿವು_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  ಅನ್ಯಥಾ
    context_regex.string = "\n";

  ಯದಿ (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  ಯದಿ (word_regex.string)
    compile_regex (&word_regex);
  ಅನ್ಯಥಾ ಯದಿ (!break_file)
    {
      ಯದಿ (gnu_extensions)
        {

          /* Simulate \w+.  */

          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ character = 0; character < CHAR_SET_SIZE; character++)
            word_fastmap[character] = !! isalpha (character);
        }
      ಅನ್ಯಥಾ
        {

          /* Simulate [^ \t\n]+.  */

          memset (word_fastmap, 1, CHAR_SET_SIZE);
          word_fastmap[' '] = 0;
          word_fastmap['\t'] = 0;
          word_fastmap['\n'] = 0;
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
swallow_file_in_memory (ವರ್ಣ ಧ್ರುವ *file_name, BLOCK *block)
{
  size_t used_length;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  ಯದಿ (using_stdin)
    block->start = fread_file (stdin, 0, &used_length);
  ಅನ್ಯಥಾ
    block->start = read_file (file_name, 0, &used_length);

  ಯದಿ (!block->start)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  ಯದಿ (using_stdin)
    clearerr (stdin);

  block->end = block->start + used_length;
}

/* Sort and search routines.  */

/*--------------------------------------------------------------------------.
| Compare two words, FIRST and SECOND, and return 0 if they are identical.  |
| Return less than 0 if the first word goes before the second; return	    |
| greater than 0 if the first word goes after the second.		    |
|									    |
| If a word is indeed a prefix of the other, the shorter should go first.   |
`--------------------------------------------------------------------------*/

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compare_ಪದಗಳು (ಧ್ರುವ ಶೂನ್ಯ *void_first, ಧ್ರುವ ಶೂನ್ಯ *void_second)
{
  WORD ಧ್ರುವ *first = void_first;
  WORD ಧ್ರುವ *second = void_second;
  idx_t length = MIN (first->size, second->size);

  ಯದಿ (ignore_case)
    {
      ಪ್ರತಿ (idx_t counter = 0; counter < length; counter++)
        {
          ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ = (folded_chars[to_uchar (first->start[counter])]
                       - folded_chars[to_uchar (second->start[counter])]);
          ಯದಿ (ಮೌಲ್ಯ != 0)
            ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
        }
    }
  ಅನ್ಯಥಾ
    {
      ಪ್ರತಿ (idx_t counter = 0; counter < length; counter++)
        {
          ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ = (to_uchar (first->start[counter])
                       - to_uchar (second->start[counter]));
          ಯದಿ (ಮೌಲ್ಯ != 0)
            ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
        }
    }

  ಪ್ರತ್ಯಾಗಮ _GL_CMP (first->size, second->size);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compare_occurs (ಧ್ರುವ ಶೂನ್ಯ *void_first, ಧ್ರುವ ಶೂನ್ಯ *void_second)
{
#define first ((ಧ್ರುವ OCCURS *) void_first)
#define second ((ಧ್ರುವ OCCURS *) void_second)
  ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;

  ಮೌಲ್ಯ = compare_ಪದಗಳು (&first->ಕೀಲಿ, &second->ಕೀಲಿ);
  ಪ್ರತ್ಯಾಗಮ (ಮೌಲ್ಯ ? ಮೌಲ್ಯ
          : _GL_CMP (first->ಕೀಲಿ.start, second->ಕೀಲಿ.start));
#undef first
#undef second
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_PURE
ಸ್ಥಿರ bool
ಹುಡುಕು_table (WORD *word, WORD_TABLE *table)
{
  idx_t lo = 0;
  idx_t hi = table->length;
  ಯಾವತ್ (lo < hi)
    {
      idx_t ಮಧ್ಯ = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ = compare_ಪದಗಳು (word, table->start + ಮಧ್ಯ);
      ಯದಿ (ಮೌಲ್ಯ < 0)
        hi = ಮಧ್ಯ;
      ಅನ್ಯಥಾ ಯದಿ (ಮೌಲ್ಯ > 0)
        lo = ಮಧ್ಯ + 1;
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ true;
    }
  ಪ್ರತ್ಯಾಗಮ false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
sort_found_occurs (ಶೂನ್ಯ)
{

  /* Only one language for the time being.  */
  ಯದಿ (ಸಂಖ್ಯೆ_of_occurs[0])
    qsort (occurs_table[0], ಸಂಖ್ಯೆ_of_occurs[0], ಪರಿಮಾಣ **occurs_table,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
digest_break_file (ವರ್ಣ ಧ್ರುವ *file_name)
{
  BLOCK file_contents;		/* to receive a copy of the file */

  swallow_file_in_memory (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (word_fastmap, 1, CHAR_SET_SIZE);
  ಪ್ರತಿ (ವರ್ಣ *cursor = file_contents.start; cursor < file_contents.end;
       cursor++)
    word_fastmap[to_uchar (*cursor)] = 0;

  ಯದಿ (!gnu_extensions)
    {

      /* If GNU extensions are enabled, the only way to avoid newline as
         a break character is to write all the break characters in the
         file with no newline at all, not even at the end of the file.
         If disabled, spaces, tabs and newlines are always considered as
         break characters even if not included in the break file.  */

      word_fastmap[' '] = 0;
      word_fastmap['\t'] = 0;
      word_fastmap['\n'] = 0;
    }

  /* Return the space of the file, which is no more required.  */

  ಮುಕ್ತ (file_contents.start);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
digest_word_file (ವರ್ಣ ಧ್ರುವ *file_name, WORD_TABLE *table)
{
  BLOCK file_contents;		/* to receive a copy of the file */
  ವರ್ಣ *cursor;			/* cursor in file copy */
  ವರ್ಣ *word_start;		/* start of the current word */

  swallow_file_in_memory (file_name, &file_contents);

  table->start = ರಿಕ್ತ;
  table->alloc = 0;
  table->length = 0;

  /* Read the whole file.  */

  cursor = file_contents.start;
  ಯಾವತ್ (cursor < file_contents.end)
    {

      /* Read one line, and save the word in contains.  */

      word_start = cursor;
      ಯಾವತ್ (cursor < file_contents.end && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      ಯದಿ (cursor > word_start)
        {
          ಯದಿ (table->length == table->alloc)
            table->start = xpalloc (table->start, &table->alloc, 1, -1,
                                    ಪರಿಮಾಣ *table->start);
          idx_t ಧ್ರುವ table_idx = table->length++;
          table->start[table_idx].start = word_start;
          table->start[table_idx].size = cursor - word_start;
        }

      /* This test allows for an incomplete line at end of file.  */

      ಯದಿ (cursor < file_contents.end)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (table->start, table->length, ಪರಿಮಾಣ table->start[0], compare_ಪದಗಳು);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
find_occurs_in_text (ಪೂರ್ಣಾಙ್ಕ file_index)
{
  ವರ್ಣ *scan;			/* for scanning the source text */
  ವರ್ಣ *ಸಾಲು_start;		/* start of the current input line */
  ವರ್ಣ *ಸಾಲು_scan;		/* newlines scanned until this point */
  idx_t reference_length;	/* length of reference in input mode */
  WORD possible_ಕೀಲಿ;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  ವರ್ಣ *context_start;		/* start of left context */
  ವರ್ಣ *context_end;		/* end of right context */
  ವರ್ಣ *word_start;		/* start of word */
  ವರ್ಣ *word_end;		/* end of word */
  ವರ್ಣ *next_context_start;	/* next start of left context */

  ಧ್ರುವ BLOCK *text_buffer = &text_buffers[file_index];

  /* reference_length is always used within 'if (input_reference)'.
     However, GNU C diagnoses that it may be used uninitialized.  The
     following assignment is merely to shut it up.  */

  reference_length = 0;

  /* Tracking where lines start is helpful for reference processing.  In
     auto reference mode, this allows counting lines.  In input reference
     mode, this permits finding the beginning of the references.

     The first line begins with the file, skip immediately this very first
     reference in input reference mode, to help further rejection any word
     found inside it.  Also, unconditionally assigning these variable has
     the happy effect of shutting up lint.  */

  ಸಾಲು_start = text_buffer->start;
  ಸಾಲು_scan = ಸಾಲು_start;
  ಯದಿ (ಒಳಹರಿವು_reference)
    {
      SKIP_NON_WHITE (ಸಾಲು_scan, text_buffer->end);
      reference_length = ಸಾಲು_scan - ಸಾಲು_start;
      SKIP_WHITE (ಸಾಲು_scan, text_buffer->end);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  ಪ್ರತಿ (ವರ್ಣ *cursor = text_buffer->start;
       cursor < text_buffer->end;
       cursor = next_context_start)
    {

      /* 'context_start' gets initialized before the processing of each
         line, or once for the whole buffer if no end of line or sentence
         sequence separator.  */

      context_start = cursor;

      /* If an end of line or end of sentence sequence is defined and
         non-empty, 'next_context_start' will be recomputed to be the end of
         each line or sentence, before each one is processed.  If no such
         sequence, then 'next_context_start' is set at the end of the whole
         buffer, which is then considered to be a single line or sentence.
         This test also accounts for the case of an incomplete line or
         sentence at the end of the buffer.  */

      next_context_start = text_buffer->end;
      ಯದಿ (context_regex.string)
        ವಿಕಲ್ಪ (re_ಹುಡುಕು (&context_regex.pattern, cursor,
                           text_buffer->end - cursor,
                           0, text_buffer->end - cursor, &context_regs))
          {
          ಪಕ್ಷ -2:
            matcher_error ();

          ಪಕ್ಷ -1:
            ಭಙ್ಗ;

          ಪಕ್ಷ 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          ಸ್ವಭಾವ:
            next_context_start = cursor + context_regs.end[0];
            ಭಙ್ಗ;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_end = next_context_start;
      SKIP_WHITE_BACKWARDS (context_end, context_start);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      ಯಾವತ್ (true)
        {
          ಯದಿ (word_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_ಹುಡುಕು (&word_regex.pattern, cursor,
                                      context_end - cursor,
                                      0, context_end - cursor, &word_regs);
              ಯದಿ (r == -2)
                matcher_error ();
              ಯದಿ (r == -1)
                ಭಙ್ಗ;
              word_start = cursor + word_regs.start[0];
              word_end = cursor + word_regs.end[0];
            }
          ಅನ್ಯಥಾ

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              scan = cursor;
              ಯಾವತ್ (scan < context_end
                     && !word_fastmap[to_uchar (*scan)])
                scan++;

              ಯದಿ (scan == context_end)
                ಭಙ್ಗ;

              word_start = scan;

              ಯಾವತ್ (scan < context_end
                     && word_fastmap[to_uchar (*scan)])
                scan++;

              word_end = scan;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = word_start;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          ಯದಿ (word_end == word_start)
            {
              cursor++;
              ಅನುವರ್ತ;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_ಕೀಲಿ.start = cursor;
          possible_ಕೀಲಿ.size = word_end - word_start;
          cursor += possible_ಕೀಲಿ.size;

          ಯದಿ (possible_ಕೀಲಿ.size > maximum_word_length)
            maximum_word_length = possible_ಕೀಲಿ.size;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          ಯದಿ (ಒಳಹರಿವು_reference)
            {
              ಯಾವತ್ (ಸಾಲು_scan < possible_ಕೀಲಿ.start)
                ಯದಿ (*ಸಾಲು_scan == '\n')
                  {
                    total_ಸಾಲು_ಎಣಿಕೆ++;
                    ಸಾಲು_scan++;
                    ಸಾಲು_start = ಸಾಲು_scan;
                    SKIP_NON_WHITE (ಸಾಲು_scan, text_buffer->end);
                    reference_length = ಸಾಲು_scan - ಸಾಲು_start;
                  }
                ಅನ್ಯಥಾ
                  ಸಾಲು_scan++;
              ಯದಿ (ಸಾಲು_scan > possible_ಕೀಲಿ.start)
                ಅನುವರ್ತ;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          ಯದಿ (ignore_file && ಹುಡುಕು_table (&possible_ಕೀಲಿ, &ignore_table))
            ಅನುವರ್ತ;
          ಯದಿ (only_file && !ಹುಡುಕು_table (&possible_ಕೀಲಿ, &only_table))
            ಅನುವರ್ತ;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          ಯದಿ (ಸಂಖ್ಯೆ_of_occurs[0] == occurs_alloc[0])
            occurs_table[0] = xpalloc (occurs_table[0], &occurs_alloc[0],
                                       1, -1, ಪರಿಮಾಣ *occurs_table[0]);
          occurs_cursor = occurs_table[0] + ಸಂಖ್ಯೆ_of_occurs[0];

          /* Define the reference field, if any.  */

          ಯದಿ (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              ಯಾವತ್ (ಸಾಲು_scan < possible_ಕೀಲಿ.start)
                ಯದಿ (*ಸಾಲು_scan == '\n')
                  {
                    total_ಸಾಲು_ಎಣಿಕೆ++;
                    ಸಾಲು_scan++;
                    ಸಾಲು_start = ಸಾಲು_scan;
                    SKIP_NON_WHITE (ಸಾಲು_scan, text_buffer->end);
                  }
                ಅನ್ಯಥಾ
                  ಸಾಲು_scan++;

              occurs_cursor->reference = total_ಸಾಲು_ಎಣಿಕೆ;
            }
          ಅನ್ಯಥಾ ಯದಿ (ಒಳಹರಿವು_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = ಸಾಲು_start - possible_ಕೀಲಿ.start;
              ಯದಿ (reference_length > reference_max_width)
                reference_max_width = reference_length;
            }

          /* Exclude the reference from the context in simple cases.  */

          ಯದಿ (ಒಳಹರಿವು_reference && ಸಾಲು_start == context_start)
            {
              SKIP_NON_WHITE (context_start, context_end);
              SKIP_WHITE (context_start, context_end);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->ಕೀಲಿ = possible_ಕೀಲಿ;
          occurs_cursor->ಎಡ = context_start - possible_ಕೀಲಿ.start;
          occurs_cursor->ಬಲ = context_end - possible_ಕೀಲಿ.start;
          occurs_cursor->file_index = file_index;

          ಸಂಖ್ಯೆ_of_occurs[0]++;
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
  ಪ್ರತಿ (ptrdiff_t counter = ಸಂಖ್ಯೆ; counter > 0; counter--)
    putchar (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_field (BLOCK field)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  ಪ್ರತಿ (ವರ್ಣ *cursor = field.start; cursor < field.end; cursor++)
    {
      unsigned ವರ್ಣ character = *cursor;
      ಯದಿ (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          ವಿಕಲ್ಪ (character)
            {
            ಪಕ್ಷ '"':
              /* In roff output format, double any quote.  */
              putchar ('"');
              putchar ('"');
              ಭಙ್ಗ;

            ಪಕ್ಷ '$':
            ಪಕ್ಷ '%':
            ಪಕ್ಷ '&':
            ಪಕ್ಷ '#':
            ಪಕ್ಷ '_':
              /* In TeX output format, precede these with a backslash.  */
              putchar ('\\');
              putchar (character);
              ಭಙ್ಗ;

            ಪಕ್ಷ '{':
            ಪಕ್ಷ '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              printf ("$\\%c$", character);
              ಭಙ್ಗ;

            ಪಕ್ಷ '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              ಭಙ್ಗ;

            ಸ್ವಭಾವ:
              /* Any other flagged character produces a single space.  */
              putchar (' ');
            }
        }
      ಅನ್ಯಥಾ
        putchar (*cursor);
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

  ಯದಿ (auto_reference)
    {
      reference_max_width = 0;
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ file_index = 0; file_index < ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files; file_index++)
        {
          intmax_t ಸಾಲು_ordinal = file_ಸಾಲು_ಎಣಿಕೆ[file_index] + 1;
          ಯದಿ (file_index > 0)
            ಸಾಲು_ordinal -= file_ಸಾಲು_ಎಣಿಕೆ[file_index - 1];
          ವರ್ಣ ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_width = sprintf (ordinal_string, "%jd", ಸಾಲು_ordinal);
          ಯದಿ (ಒಳಹರಿವು_file_name[file_index])
            reference_width += strlen (ಒಳಹರಿವು_file_name[file_index]);
          ಯದಿ (reference_width > reference_max_width)
            reference_max_width = reference_width;
        }
      reference_max_width++;
      reference.start = xmalloc (reference_max_width + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  ಯದಿ ((auto_reference || ಒಳಹರಿವು_reference) && !ಬಲ_reference)
    ಸಾಲು_width = MAX (0, ಸಾಲು_width - (reference_max_width + gap_size));

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

  half_ಸಾಲು_width = ಸಾಲು_width >> 1;
  before_max_width = half_ಸಾಲು_width - gap_size;
  keyafter_max_width = half_ಸಾಲು_width;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  ಯದಿ (truncation_string && *truncation_string)
    truncation_string_length = strlen (truncation_string);
  ಅನ್ಯಥಾ
    truncation_string = ರಿಕ್ತ;

  ಯದಿ (gnu_extensions)
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

      before_max_width -= 2 * truncation_string_length;
      keyafter_max_width -= 2 * truncation_string_length;
    }
  ಅನ್ಯಥಾ
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_width -= 2 * truncation_string_length + 1;
    }

  ಯದಿ (before_max_width < 0)
    before_max_width = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ character = 0; character < CHAR_SET_SIZE; character++)
    edited_flag[character] = !! isspace (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  ವಿಕಲ್ಪ (output_format)
    {
    ಪಕ್ಷ UNKNOWN_FORMAT:
      /* Should never happen.  */

    ಪಕ್ಷ DUMB_FORMAT:
      ಭಙ್ಗ;

    ಪಕ್ಷ ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      ಭಙ್ಗ;

    ಪಕ್ಷ TEX_FORMAT:

      /* Various characters need special processing.  */

      ಪ್ರತಿ (ವರ್ಣ ಧ್ರುವ *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[to_uchar (*cursor)] = 1;

      ಭಙ್ಗ;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t tail_max_width;	/* allowable width of tail field */
  ptrdiff_t head_max_width;	/* allowable width of head field */
  ವರ್ಣ *cursor;			/* running cursor in source text */
  ವರ್ಣ *ಎಡ_context_start;	/* start of left context */
  ವರ್ಣ *ಬಲ_context_end;	/* end of right context */
  ವರ್ಣ *ಎಡ_field_start;	/* conservative start for 'head'/'before' */
  ವರ್ಣ ಧ್ರುವ *file_name;	/* file name for reference */
  intmax_t ಸಾಲು_ordinal;	/* line ordinal for reference */
  ವರ್ಣ ಧ್ರುವ *buffer_start;	/* start of buffered file for this occurs */
  ವರ್ಣ ಧ್ರುವ *buffer_end;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.start = occurs->ಕೀಲಿ.start;
  keyafter.end = keyafter.start + occurs->ಕೀಲಿ.size;
  ಎಡ_context_start = keyafter.start + occurs->ಎಡ;
  ಬಲ_context_end = keyafter.start + occurs->ಬಲ;

  buffer_start = text_buffers[occurs->file_index].start;
  buffer_end = text_buffers[occurs->file_index].end;

  cursor = keyafter.end;
  ಯಾವತ್ (cursor < ಬಲ_context_end
         && cursor <= keyafter.start + keyafter_max_width)
    {
      keyafter.end = cursor;
      SKIP_SOMETHING (cursor, ಬಲ_context_end);
    }
  ಯದಿ (cursor <= keyafter.start + keyafter_max_width)
    keyafter.end = cursor;

  keyafter_truncation = truncation_string && keyafter.end < ಬಲ_context_end;

  SKIP_WHITE_BACKWARDS (keyafter.end, keyafter.start);

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

  ಯದಿ (-occurs->ಎಡ > half_ಸಾಲು_width + maximum_word_length)
    {
      ಎಡ_field_start
        = keyafter.start - (half_ಸಾಲು_width + maximum_word_length);
      SKIP_SOMETHING (ಎಡ_field_start, keyafter.start);
    }
  ಅನ್ಯಥಾ
    ಎಡ_field_start = keyafter.start + occurs->ಎಡ;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.start = ಎಡ_field_start;
  before.end = keyafter.start;
  SKIP_WHITE_BACKWARDS (before.end, before.start);

  ಯಾವತ್ (before.start + before_max_width < before.end)
    SKIP_SOMETHING (before.start, before.end);

  ಯದಿ (truncation_string)
    {
      cursor = before.start;
      SKIP_WHITE_BACKWARDS (cursor, buffer_start);
      before_truncation = cursor > ಎಡ_context_start;
    }
  ಅನ್ಯಥಾ
    before_truncation = false;

  SKIP_WHITE (before.start, buffer_end);

  /* The tail could not take more columns than what has been left in the
     left context field, and a gap is mandatory.  It starts after the
     right context, and does not contain prefixed spaces.  It ends at
     the end of line, the end of buffer or when the tail field is full,
     whichever comes first.  It cannot contain only part of a word, and
     has no suffixed spaces.  */

  tail_max_width
    = before_max_width - (before.end - before.start) - gap_size;

  ಯದಿ (tail_max_width > 0)
    {
      tail.start = keyafter.end;
      SKIP_WHITE (tail.start, buffer_end);

      tail.end = tail.start;
      cursor = tail.end;
      ಯಾವತ್ (cursor < ಬಲ_context_end
             && cursor < tail.start + tail_max_width)
        {
          tail.end = cursor;
          SKIP_SOMETHING (cursor, ಬಲ_context_end);
        }

      ಯದಿ (cursor < tail.start + tail_max_width)
        tail.end = cursor;

      ಯದಿ (tail.end > tail.start)
        {
          keyafter_truncation = false;
          tail_truncation = truncation_string && tail.end < ಬಲ_context_end;
        }
      ಅನ್ಯಥಾ
        tail_truncation = false;

      SKIP_WHITE_BACKWARDS (tail.end, tail.start);
    }
  ಅನ್ಯಥಾ
    {

      /* No place left for a tail field.  */

      tail.start = ರಿಕ್ತ;
      tail.end = ರಿಕ್ತ;
      tail_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  head_max_width
    = keyafter_max_width - (keyafter.end - keyafter.start) - gap_size;

  ಯದಿ (head_max_width > 0)
    {
      head.end = before.start;
      SKIP_WHITE_BACKWARDS (head.end, buffer_start);

      head.start = ಎಡ_field_start;
      ಯಾವತ್ (head.start + head_max_width < head.end)
        SKIP_SOMETHING (head.start, head.end);

      ಯದಿ (head.end > head.start)
        {
          before_truncation = false;
          head_truncation = (truncation_string
                             && head.start > ಎಡ_context_start);
        }
      ಅನ್ಯಥಾ
        head_truncation = false;

      SKIP_WHITE (head.start, head.end);
    }
  ಅನ್ಯಥಾ
    {

      /* No place left for a head field.  */

      head.start = ರಿಕ್ತ;
      head.end = ರಿಕ್ತ;
      head_truncation = false;
    }

  ಯದಿ (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = ಒಳಹರಿವು_file_name[occurs->file_index];
      ಯದಿ (!file_name)
        file_name = "";

      ಸಾಲು_ordinal = occurs->reference + 1;
      ಯದಿ (occurs->file_index > 0)
        ಸಾಲು_ordinal -= file_ಸಾಲು_ಎಣಿಕೆ[occurs->file_index - 1];

      ವರ್ಣ *file_end = stpcpy (reference.start, file_name);
      reference.end = file_end + sprintf (file_end, ":%jd", ಸಾಲು_ordinal);
    }
  ಅನ್ಯಥಾ ಯದಿ (ಒಳಹರಿವು_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.start = keyafter.start + occurs->reference;
      reference.end = reference.start;
      SKIP_NON_WHITE (reference.end, ಬಲ_context_end);
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

  printf (".%s \"", macro_name);
  print_field (tail);
  ಯದಿ (tail_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  ಯದಿ (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);
  putchar ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_field (keyafter);
  ಯದಿ (keyafter_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  ಯದಿ (head_truncation)
    fputs (truncation_string, stdout);
  print_field (head);
  putchar ('"');

  /* Conditionally output the 'reference' field.  */

  ಯದಿ (auto_reference || ಒಳಹರಿವು_reference)
    {
      fputs (" \"", stdout);
      print_field (reference);
      putchar ('"');
    }

  putchar ('\n');
}

/*---------------------------------------------------------.
| Output the current output fields as one line for 'TeX'.  |
`---------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
output_one_tex_ಸಾಲು (ಶೂನ್ಯ)
{
  BLOCK ಕೀಲಿ;			/* key field, isolated */
  BLOCK after;			/* after field, isolated */
  ವರ್ಣ *cursor;			/* running cursor in source text */

  printf ("\\%s ", macro_name);
  putchar ('{');
  print_field (tail);
  fputs ("}{", stdout);
  print_field (before);
  fputs ("}{", stdout);
  ಕೀಲಿ.start = keyafter.start;
  after.end = keyafter.end;
  cursor = keyafter.start;
  SKIP_SOMETHING (cursor, keyafter.end);
  ಕೀಲಿ.end = cursor;
  after.start = cursor;
  print_field (ಕೀಲಿ);
  fputs ("}{", stdout);
  print_field (after);
  fputs ("}{", stdout);
  print_field (head);
  putchar ('}');
  ಯದಿ (auto_reference || ಒಳಹರಿವು_reference)
    {
      putchar ('{');
      print_field (reference);
      putchar ('}');
    }
  putchar ('\n');
}

/*-------------------------------------------------------------------.
| Output the current output fields as one line for a dumb terminal.  |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
output_one_dumb_ಸಾಲು (ಶೂನ್ಯ)
{
  ಯದಿ (!ಬಲ_reference)
    {
      ಯದಿ (auto_reference)
        {

          /* Output the 'reference' field, in such a way that GNU emacs
             next-error will handle it.  The ending colon is taken from the
             gap which follows.  */

          print_field (reference);
          putchar (':');
          print_spaces (reference_max_width
                        + gap_size
                        - (reference.end - reference.start)
                        - 1);
        }
      ಅನ್ಯಥಾ
        {

          /* Output the 'reference' field and its following gap.  */

          print_field (reference);
          print_spaces (reference_max_width
                        + gap_size
                        - (reference.end - reference.start));
        }
    }

  ಯದಿ (tail.start < tail.end)
    {
      /* Output the 'tail' field.  */

      print_field (tail);
      ಯದಿ (tail_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_ಸಾಲು_width - gap_size
                    - (before.end - before.start)
                    - (before_truncation ? truncation_string_length : 0)
                    - (tail.end - tail.start)
                    - (tail_truncation ? truncation_string_length : 0));
    }
  ಅನ್ಯಥಾ
    print_spaces (half_ಸಾಲು_width - gap_size
                  - (before.end - before.start)
                  - (before_truncation ? truncation_string_length : 0));

  /* Output the 'before' field.  */

  ಯದಿ (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);

  print_spaces (gap_size);

  /* Output the 'keyafter' field.  */

  print_field (keyafter);
  ಯದಿ (keyafter_truncation)
    fputs (truncation_string, stdout);

  ಯದಿ (head.start < head.end)
    {
      /* Output the 'head' field.  */

      print_spaces (half_ಸಾಲು_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0)
                    - (head.end - head.start)
                    - (head_truncation ? truncation_string_length : 0));
      ಯದಿ (head_truncation)
        fputs (truncation_string, stdout);
      print_field (head);
    }
  ಅನ್ಯಥಾ

    ಯದಿ ((auto_reference || ಒಳಹರಿವು_reference) && ಬಲ_reference)
      print_spaces (half_ಸಾಲು_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0));

  ಯದಿ ((auto_reference || ಒಳಹರಿವು_reference) && ಬಲ_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_size);
      print_field (reference);
    }

  putchar ('\n');
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

  tail.start = ರಿಕ್ತ;
  tail.end = ರಿಕ್ತ;
  tail_truncation = false;

  head.start = ರಿಕ್ತ;
  head.end = ರಿಕ್ತ;
  head_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_table[0];

  ಪ್ರತಿ (idx_t occurs_index = 0; occurs_index < ಸಂಖ್ಯೆ_of_occurs[0];
       occurs_index++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      ವಿಕಲ್ಪ (output_format)
        {
        ಪಕ್ಷ UNKNOWN_FORMAT:
          /* Should never happen.  */

        ಪಕ್ಷ DUMB_FORMAT:
          output_one_dumb_ಸಾಲು ();
          ಭಙ್ಗ;

        ಪಕ್ಷ ROFF_FORMAT:
          output_one_roff_ಸಾಲು ();
          ಭಙ್ಗ;

        ಪಕ್ಷ TEX_FORMAT:
          output_one_tex_ಸಾಲು ();
          ಭಙ್ಗ;
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
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [ಒಳಹರಿವು_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [ಒಳಹರಿವು_2 [OUTPUT]]\n"),
              program_name, program_name);
      fputs (_("\
Output a permuted index, including context, of the ಪದಗಳು in the ಒಳಹರಿವು files.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --auto-reference           output automatically generated references\n\
"));
      oputs (_("\
  -G, --traditional              behave more like System V 'ptx'\n\
"));
      oputs (_("\
  -F, --flag-truncation=STRING   use STRING ಪ್ರತಿ flagging ಸಾಲು truncations.\n\
                                 The ಸ್ವಭಾವ is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name to use instead of 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --ಬಲ-side-refs          put references at ಬಲ, not counted in -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   ಪ್ರತಿ end of ಸಾಲುಗಳು or end of sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --word-regexp=REGEXP       use REGEXP to match each keyword\n\
"));
      oputs (_("\
  -b, --ಭಙ್ಗ-file=FILE          word ಭಙ್ಗ characters in this FILE\n\
"));
      oputs (_("\
  -f, --ignore-ಪಕ್ಷ              fold lower ಪಕ್ಷ to upper ಪಕ್ಷ ಪ್ರತಿ sorting\n\
"));
      oputs (_("\
  -g, --gap-size=ಸಂಖ್ಯೆ_2          gap size in columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         ಪಠ ignore word list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           ಪಠ only word list from this FILE\n\
"));
      oputs (_("\
  -r, --references               first field of each ಸಾಲು is a reference\n\
"));
      oputs (_("\
  -t, --typeset-mode             change the ಸ್ವಭಾವ width from 72 to 100\n\
"));
      oputs (_("\
  -w, --width=ಸಂಖ್ಯೆ_2             output width in columns, reference excluded\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/*----------------------------------------------------------------------.
| Main program.  Decode ARGC arguments passed through the ARGV array of |
| strings, then launch execution.				        |
`----------------------------------------------------------------------*/

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"auto-reference", no_argument, ರಿಕ್ತ, 'A'},
  {"break-file", required_argument, ರಿಕ್ತ, 'b'},
  {"flag-truncation", required_argument, ರಿಕ್ತ, 'F'},
  {"ignore-case", no_argument, ರಿಕ್ತ, 'f'},
  {"gap-size", required_argument, ರಿಕ್ತ, 'g'},
  {"ignore-file", required_argument, ರಿಕ್ತ, 'i'},
  {"macro-name", required_argument, ರಿಕ್ತ, 'M'},
  {"only-file", required_argument, ರಿಕ್ತ, 'o'},
  {"references", no_argument, ರಿಕ್ತ, 'r'},
  {"right-side-refs", no_argument, ರಿಕ್ತ, 'R'},
  {"format", required_argument, ರಿಕ್ತ, FORMAT_OPTION},
  {"sentence-regexp", required_argument, ರಿಕ್ತ, 'S'},
  {"traditional", no_argument, ರಿಕ್ತ, 'G'},
  {"typeset-mode", no_argument, ರಿಕ್ತ, 't'},
  {"width", required_argument, ರಿಕ್ತ, 'w'},
  {"word-regexp", required_argument, ರಿಕ್ತ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0},
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ format_args[] =
{
  "roff", "tex", ರಿಕ್ತ
};

ಸ್ಥಿರ ಗಣನಾ Format ಧ್ರುವ format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ optchar;			/* argument character */
  ಪೂರ್ಣಾಙ್ಕ file_index;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, ರಿಕ್ತ),
         optchar != EOF)
    {
      ವಿಕಲ್ಪ (optchar)
        {
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);

        ಪಕ್ಷ 'G':
          gnu_extensions = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'b':
          break_file = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'f':
          ignore_case = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'g':
          {
            intmax_t tmp;
            ಯದಿ (! (xstrtoimax (optarg, ರಿಕ್ತ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_size = tmp;
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ 'i':
          ignore_file = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'o':
          only_file = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'r':
          ಒಳಹರಿವು_reference = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 't':
          ಯದಿ (ಸಾಲು_width < 0)
            ಸಾಲು_width = 100;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'w':
          {
            intmax_t tmp;
            ಯದಿ (! (xstrtoimax (optarg, ರಿಕ್ತ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            ಸಾಲು_width = tmp;
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ 'A':
          auto_reference = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'M':
          macro_name = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'O':
          output_format = ROFF_FORMAT;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'R':
          ಬಲ_reference = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'T':
          output_format = TEX_FORMAT;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'W':
          word_regex.string = optarg;
          unescape_string (optarg);
          ಯದಿ (!*word_regex.string)
            word_regex.string = ರಿಕ್ತ;
          ಭಙ್ಗ;

        ಪಕ್ಷ FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_args, format_vals);
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        }
    }

  ಯದಿ (ಸಾಲು_width < 0)
    ಸಾಲು_width = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  ಯದಿ (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      ಒಳಹರಿವು_file_name = xmalloc (ಪರಿಮಾಣ *ಒಳಹರಿವು_file_name);
      file_ಸಾಲು_ಎಣಿಕೆ = xmalloc (ಪರಿಮಾಣ *file_ಸಾಲು_ಎಣಿಕೆ);
      text_buffers =    xmalloc (ಪರಿಮಾಣ *text_buffers);
      ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files = 1;
      ಒಳಹರಿವು_file_name[0] = ರಿಕ್ತ;
    }
  ಅನ್ಯಥಾ ಯದಿ (gnu_extensions)
    {
      ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files = argc - optind;
      ಒಳಹರಿವು_file_name = xnmalloc (ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files, ಪರಿಮಾಣ *ಒಳಹರಿವು_file_name);
      file_ಸಾಲು_ಎಣಿಕೆ = xnmalloc (ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files, ಪರಿಮಾಣ *file_ಸಾಲು_ಎಣಿಕೆ);
      text_buffers    = xnmalloc (ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files, ಪರಿಮಾಣ *text_buffers);

      ಪ್ರತಿ (file_index = 0; file_index < ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files; file_index++)
        {
          ಯದಿ (!*argv[optind] || streq (argv[optind], "-"))
            ಒಳಹರಿವು_file_name[file_index] = ರಿಕ್ತ;
          ಅನ್ಯಥಾ
            ಒಳಹರಿವು_file_name[file_index] = argv[optind];
          optind++;
        }
    }
  ಅನ್ಯಥಾ
    {

      /* There is one necessary input file.  */

      ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files = 1;
      ಒಳಹರಿವು_file_name = xmalloc (ಪರಿಮಾಣ *ಒಳಹರಿವು_file_name);
      file_ಸಾಲು_ಎಣಿಕೆ = xmalloc (ಪರಿಮಾಣ *file_ಸಾಲು_ಎಣಿಕೆ);
      text_buffers    = xmalloc (ಪರಿಮಾಣ *text_buffers);
      ಯದಿ (!*argv[optind] || streq (argv[optind], "-"))
        ಒಳಹರಿವು_file_name[0] = ರಿಕ್ತ;
      ಅನ್ಯಥಾ
        ಒಳಹರಿವು_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      ಯದಿ (optind < argc)
        {
          ಯದಿ (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      ಯದಿ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  ಯದಿ (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  ಯದಿ (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  ಯದಿ (ignore_file)
    {
      digest_word_file (ignore_file, &ignore_table);
      ಯದಿ (ignore_table.length == 0)
        ignore_file = ರಿಕ್ತ;
    }

  ಯದಿ (only_file)
    {
      digest_word_file (only_file, &only_table);
      ಯದಿ (only_table.length == 0)
        only_file = ರಿಕ್ತ;
    }

  /* Prepare to study all the input files.  */

  ಸಂಖ್ಯೆ_of_occurs[0] = 0;
  total_ಸಾಲು_ಎಣಿಕೆ = 0;
  maximum_word_length = 0;
  reference_max_width = 0;

  ಪ್ರತಿ (file_index = 0; file_index < ಸಂಖ್ಯೆ_ಒಳಹರಿವು_files; file_index++)
    {
      BLOCK *text_buffer = text_buffers + file_index;

      /* Read the file contents into memory, then study it.  */

      swallow_file_in_memory (ಒಳಹರಿವು_file_name[file_index], text_buffer);
      find_occurs_in_text (file_index);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      total_ಸಾಲು_ಎಣಿಕೆ++;
      file_ಸಾಲು_ಎಣಿಕೆ[file_index] = total_ಸಾಲು_ಎಣಿಕೆ;
    }

  /* Do the output process phase.  */

  sort_found_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}
