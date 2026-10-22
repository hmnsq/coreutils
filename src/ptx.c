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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ <regex.h>
#ଅନ୍ତର୍ଭାଵ "argmatch.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "octhexdigits.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "read-file.h"
#ଅନ୍ତର୍ଭାଵ "stdio--.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#define AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#define CHAR_SET_SIZE 256

/* Debugging the memory allocator.  */

#ଯଦି WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#endif

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

ଗଣନା Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

ସ୍ଥିର bool gnu_extensions = true;	/* trigger all GNU extensions */
ସ୍ଥିର bool auto_reference = false;	/* refs are 'file_name:line_number:' */
ସ୍ଥିର bool ନିବେଶ_reference = false;	/* refs at beginning of input lines */
ସ୍ଥିର bool ଡାହାଣ_reference = false;	/* output refs after right context  */
ସ୍ଥିର idx_t ଧାଡ଼ି_width = -1;		/* output line width in characters */
ସ୍ଥିର idx_t gap_size = 3;	/* number of spaces between output fields */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *truncation_string = "/";
                                /* string used to mark line truncations */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *macro_name = "xx";	/* macro name for roff or TeX output */
ସ୍ଥିର ଗଣନା Format output_format = UNKNOWN_FORMAT;
                                /* output format */

ସ୍ଥିର bool ignore_case = false;	/* fold lower to upper for sorting */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *break_file = ଖାଲି; /* name of the 'Break chars' file */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *only_file = ଖାଲି;	/* name of the 'Only words' file */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ignore_file = ଖାଲି; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
ସଂରଚନା regex_data
{
  /* The original regular expression, as a string.  */
  ଅକ୍ଷର ଅଚଳ *string;

  /* The compiled regular expression, and its fastmap.  */
  ସଂରଚନା re_pattern_buffer pattern;
  ଅକ୍ଷର fastmap[UCHAR_MAX + 1];
};

ସ୍ଥିର ସଂରଚନା regex_data context_regex;	/* end of context */
ସ୍ଥିର ସଂରଚନା regex_data word_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
  {
    ଅକ୍ଷର *start;		/* pointer to beginning of region */
    ଅକ୍ଷର *end;			/* pointer to end + 1 of region */
  }
BLOCK;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
  {
    ଅକ୍ଷର *start;		/* pointer to beginning of region */
    idx_t size;			/* length of the region */
  }
WORD;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
  {
    idx_t alloc;		/* allocated length */
    idx_t length;		/* number of used entries */
    WORD *start COUNTED_BY (length);	/* array of WORDs */
  }
WORD_TABLE;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
ସ୍ଥିର unsigned ଅକ୍ଷର folded_chars[CHAR_SET_SIZE];

/* End of context pattern register indices.  */
ସ୍ଥିର ସଂରଚନା re_registers context_regs;

/* Keyword pattern register indices.  */
ସ୍ଥିର ସଂରଚନା re_registers word_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
ସ୍ଥିର ଅକ୍ଷର word_fastmap[CHAR_SET_SIZE];

/* Maximum length of any word read.  */
ସ୍ଥିର idx_t maximum_word_length;

/* Maximum width of any reference used.  */
ସ୍ଥିର idx_t reference_max_width;

/* Ignore and Only word tables.  */

ସ୍ଥିର WORD_TABLE ignore_table;	/* table of words to ignore */
ସ୍ଥିର WORD_TABLE only_table;		/* table of words to select */

/* Source text table, and scanning macros.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା_ନିବେଶ_files;	/* number of text input files */
ସ୍ଥିର intmax_t total_ଧାଡ଼ି_ଗଣନା;	/* total number of lines seen so far */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ **ନିବେଶ_file_name;	/* array of text input file names */
ସ୍ଥିର intmax_t *file_ଧାଡ଼ି_ଗଣନା;	/* array of line count values at end */

ସ୍ଥିର BLOCK *text_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#define SKIP_NON_WHITE(cursor, limit) \
  ଯେତେବେଳେ (cursor < limit && ! isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE(cursor, limit) \
  ଯେତେବେଳେ (cursor < limit && isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE_BACKWARDS(cursor, start) \
  ଯେତେବେଳେ (cursor > start && isspace (to_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#define SKIP_SOMETHING(cursor, limit) \
  ଯଦି (word_regex.string)						\
    {									\
      regoff_t ଗଣନା_2;							\
      ଗଣନା_2 = re_match (&word_regex.pattern, cursor, limit - cursor,	\
                        0, ଖାଲି);					\
      ଯଦି (ଗଣନା_2 == -2)							\
        matcher_error ();						\
      cursor += ଗଣନା_2 <= 0 ? 1 : ଗଣନା_2;					\
    }									\
  ଅନ୍ୟଥା ଯଦି (word_fastmap[to_uchar (*cursor)])				\
    ଯେତେବେଳେ (cursor < limit && word_fastmap[to_uchar (*cursor)])		\
      cursor++;								\
  ଅନ୍ୟଥା									\
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

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
  {
    WORD ଚାବି;			/* description of the keyword */
    ptrdiff_t ବାମ;		/* distance to left context start */
    ptrdiff_t ଡାହାଣ;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା file_index;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

ସ୍ଥିର OCCURS *occurs_table[1];	/* all words retained from the read text */
ସ୍ଥିର idx_t occurs_alloc[1];	/* allocated size of occurs_table */
ସ୍ଥିର idx_t ସଂଖ୍ୟା_of_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
ସ୍ଥିର ଅକ୍ଷର edited_flag[CHAR_SET_SIZE];

/* Half of line width, reference excluded.  */
ସ୍ଥିର idx_t half_ଧାଡ଼ି_width;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ସ୍ଥିର ptrdiff_t before_max_width;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ସ୍ଥିର ptrdiff_t keyafter_max_width;

/* Length of string that flags truncation.  */
ସ୍ଥିର idx_t truncation_string_length;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

ସ୍ଥିର BLOCK tail;		/* tail field */
ସ୍ଥିର bool tail_truncation;	/* flag truncation after the tail field */

ସ୍ଥିର BLOCK before;		/* before field */
ସ୍ଥିର bool before_truncation;	/* flag truncation before the before field */

ସ୍ଥିର BLOCK keyafter;		/* keyword-and-after field */
ସ୍ଥିର bool keyafter_truncation; /* flag truncation after the keyafter field */

ସ୍ଥିର BLOCK head;		/* head field */
ସ୍ଥିର bool head_truncation;	/* flag truncation before the head field */

ସ୍ଥିର BLOCK reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

ସ୍ଥିର _Noreturn ଶୂନ୍ୟ
matcher_error (ଶୂନ୍ୟ)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

ସ୍ଥିର ଶୂନ୍ୟ
unescape_string (ଅକ୍ଷର *string)
{
  ଅକ୍ଷର *cursor;			/* cursor in result */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;			/* value of \nnn escape */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା length;			/* length of \nnn escape */

  cursor = string;

  ଯେତେବେଳେ (*string)
    {
      ଯଦି (*string == '\\')
        {
          string++;
          ବାଛ (*string)
            {
            ସ୍ଥିତି 'x':		/* \xhhh escape, 3 chars maximum */
              ମୂଲ୍ୟ = 0;
              ପାଇଁ (length = 0, string++;
                   length < 3 && c_isxdigit (*string);
                   length++, string++)
                ମୂଲ୍ୟ = ମୂଲ୍ୟ * 16 + fromhex (*string);
              ଯଦି (length == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              ଅନ୍ୟଥା
                *cursor++ = ମୂଲ୍ୟ;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '0':		/* \0ooo escape, 3 chars maximum */
              ମୂଲ୍ୟ = 0;
              ପାଇଁ (length = 0, string++;
                   length < 3 && isoct (*string);
                   length++, string++)
                ମୂଲ୍ୟ = ମୂଲ୍ୟ * 8 + fromoct (*string);
              *cursor++ = ମୂଲ୍ୟ;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'c':		/* cancel the rest of the output */
              ଯେତେବେଳେ (*string)
                string++;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '\0':		/* lone backslash at end of string */
              /* ignore it */
              ଭାଙ୍ଗ;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              *cursor++ = '\\';
              *cursor++ = *string++;
              ଭାଙ୍ଗ;
            }
        }
      ଅନ୍ୟଥା
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
compile_regex (ସଂରଚନା regex_data *regex)
{
  ସଂରଚନା re_pattern_buffer *pattern = &regex->pattern;
  ଅକ୍ଷର ଅଚଳ *string = regex->string;
  ଅକ୍ଷର ଅଚଳ *message;

  pattern->buffer = ଖାଲି;
  pattern->allocated = 0;
  pattern->fastmap = regex->fastmap;
  pattern->translate = ignore_case ? folded_chars : ଖାଲି;

  message = re_compile_pattern (string, strlen (string), pattern);
  ଯଦି (message)
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

ସ୍ଥିର ଶୂନ୍ୟ
initialize_regex (ଶୂନ୍ୟ)
{
  /* Initialize the case folding table.  */

  ଯଦି (ignore_case)
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା character = 0; character < CHAR_SET_SIZE; character++)
      folded_chars[character] = toupper (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  ଯଦି (context_regex.string)
    {
      ଯଦି (!*context_regex.string)
        context_regex.string = ଖାଲି;
    }
  ଅନ୍ୟଥା ଯଦି (gnu_extensions && !ନିବେଶ_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  ଅନ୍ୟଥା
    context_regex.string = "\n";

  ଯଦି (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  ଯଦି (word_regex.string)
    compile_regex (&word_regex);
  ଅନ୍ୟଥା ଯଦି (!break_file)
    {
      ଯଦି (gnu_extensions)
        {

          /* Simulate \w+.  */

          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା character = 0; character < CHAR_SET_SIZE; character++)
            word_fastmap[character] = !! isalpha (character);
        }
      ଅନ୍ୟଥା
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

ସ୍ଥିର ଶୂନ୍ୟ
swallow_file_in_memory (ଅକ୍ଷର ଅଚଳ *file_name, BLOCK *block)
{
  size_t used_length;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  ଯଦି (using_stdin)
    block->start = fread_file (stdin, 0, &used_length);
  ଅନ୍ୟଥା
    block->start = read_file (file_name, 0, &used_length);

  ଯଦି (!block->start)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  ଯଦି (using_stdin)
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare_ଶବ୍ଦଗୁଡ଼ିକ (ଅଚଳ ଶୂନ୍ୟ *void_first, ଅଚଳ ଶୂନ୍ୟ *void_second)
{
  WORD ଅଚଳ *first = void_first;
  WORD ଅଚଳ *second = void_second;
  idx_t length = MIN (first->size, second->size);

  ଯଦି (ignore_case)
    {
      ପାଇଁ (idx_t counter = 0; counter < length; counter++)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ = (folded_chars[to_uchar (first->start[counter])]
                       - folded_chars[to_uchar (second->start[counter])]);
          ଯଦି (ମୂଲ୍ୟ != 0)
            ଫେରନ୍ତୁ ମୂଲ୍ୟ;
        }
    }
  ଅନ୍ୟଥା
    {
      ପାଇଁ (idx_t counter = 0; counter < length; counter++)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ = (to_uchar (first->start[counter])
                       - to_uchar (second->start[counter]));
          ଯଦି (ମୂଲ୍ୟ != 0)
            ଫେରନ୍ତୁ ମୂଲ୍ୟ;
        }
    }

  ଫେରନ୍ତୁ _GL_CMP (first->size, second->size);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare_occurs (ଅଚଳ ଶୂନ୍ୟ *void_first, ଅଚଳ ଶୂନ୍ୟ *void_second)
{
#define first ((ଅଚଳ OCCURS *) void_first)
#define second ((ଅଚଳ OCCURS *) void_second)
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;

  ମୂଲ୍ୟ = compare_ଶବ୍ଦଗୁଡ଼ିକ (&first->ଚାବି, &second->ଚାବି);
  ଫେରନ୍ତୁ (ମୂଲ୍ୟ ? ମୂଲ୍ୟ
          : _GL_CMP (first->ଚାବି.start, second->ଚାବି.start));
#undef first
#undef second
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_PURE
ସ୍ଥିର bool
ସନ୍ଧାନ_table (WORD *word, WORD_TABLE *table)
{
  idx_t lo = 0;
  idx_t hi = table->length;
  ଯେତେବେଳେ (lo < hi)
    {
      idx_t ମଧ୍ୟ = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ = compare_ଶବ୍ଦଗୁଡ଼ିକ (word, table->start + ମଧ୍ୟ);
      ଯଦି (ମୂଲ୍ୟ < 0)
        hi = ମଧ୍ୟ;
      ଅନ୍ୟଥା ଯଦି (ମୂଲ୍ୟ > 0)
        lo = ମଧ୍ୟ + 1;
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ true;
    }
  ଫେରନ୍ତୁ false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
sort_found_occurs (ଶୂନ୍ୟ)
{

  /* Only one language for the time being.  */
  ଯଦି (ସଂଖ୍ୟା_of_occurs[0])
    qsort (occurs_table[0], ସଂଖ୍ୟା_of_occurs[0], ଆକାର **occurs_table,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
digest_break_file (ଅକ୍ଷର ଅଚଳ *file_name)
{
  BLOCK file_contents;		/* to receive a copy of the file */

  swallow_file_in_memory (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (word_fastmap, 1, CHAR_SET_SIZE);
  ପାଇଁ (ଅକ୍ଷର *cursor = file_contents.start; cursor < file_contents.end;
       cursor++)
    word_fastmap[to_uchar (*cursor)] = 0;

  ଯଦି (!gnu_extensions)
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

  ମୁକ୍ତ (file_contents.start);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
digest_word_file (ଅକ୍ଷର ଅଚଳ *file_name, WORD_TABLE *table)
{
  BLOCK file_contents;		/* to receive a copy of the file */
  ଅକ୍ଷର *cursor;			/* cursor in file copy */
  ଅକ୍ଷର *word_start;		/* start of the current word */

  swallow_file_in_memory (file_name, &file_contents);

  table->start = ଖାଲି;
  table->alloc = 0;
  table->length = 0;

  /* Read the whole file.  */

  cursor = file_contents.start;
  ଯେତେବେଳେ (cursor < file_contents.end)
    {

      /* Read one line, and save the word in contains.  */

      word_start = cursor;
      ଯେତେବେଳେ (cursor < file_contents.end && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      ଯଦି (cursor > word_start)
        {
          ଯଦି (table->length == table->alloc)
            table->start = xpalloc (table->start, &table->alloc, 1, -1,
                                    ଆକାର *table->start);
          idx_t ଅଚଳ table_idx = table->length++;
          table->start[table_idx].start = word_start;
          table->start[table_idx].size = cursor - word_start;
        }

      /* This test allows for an incomplete line at end of file.  */

      ଯଦି (cursor < file_contents.end)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (table->start, table->length, ଆକାର table->start[0], compare_ଶବ୍ଦଗୁଡ଼ିକ);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
find_occurs_in_text (ପୂର୍ଣ୍ଣସଂଖ୍ୟା file_index)
{
  ଅକ୍ଷର *scan;			/* for scanning the source text */
  ଅକ୍ଷର *ଧାଡ଼ି_start;		/* start of the current input line */
  ଅକ୍ଷର *ଧାଡ଼ି_scan;		/* newlines scanned until this point */
  idx_t reference_length;	/* length of reference in input mode */
  WORD possible_ଚାବି;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  ଅକ୍ଷର *context_start;		/* start of left context */
  ଅକ୍ଷର *context_end;		/* end of right context */
  ଅକ୍ଷର *word_start;		/* start of word */
  ଅକ୍ଷର *word_end;		/* end of word */
  ଅକ୍ଷର *next_context_start;	/* next start of left context */

  ଅଚଳ BLOCK *text_buffer = &text_buffers[file_index];

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

  ଧାଡ଼ି_start = text_buffer->start;
  ଧାଡ଼ି_scan = ଧାଡ଼ି_start;
  ଯଦି (ନିବେଶ_reference)
    {
      SKIP_NON_WHITE (ଧାଡ଼ି_scan, text_buffer->end);
      reference_length = ଧାଡ଼ି_scan - ଧାଡ଼ି_start;
      SKIP_WHITE (ଧାଡ଼ି_scan, text_buffer->end);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  ପାଇଁ (ଅକ୍ଷର *cursor = text_buffer->start;
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
      ଯଦି (context_regex.string)
        ବାଛ (re_ସନ୍ଧାନ (&context_regex.pattern, cursor,
                           text_buffer->end - cursor,
                           0, text_buffer->end - cursor, &context_regs))
          {
          ସ୍ଥିତି -2:
            matcher_error ();

          ସ୍ଥିତି -1:
            ଭାଙ୍ଗ;

          ସ୍ଥିତି 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            next_context_start = cursor + context_regs.end[0];
            ଭାଙ୍ଗ;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_end = next_context_start;
      SKIP_WHITE_BACKWARDS (context_end, context_start);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      ଯେତେବେଳେ (true)
        {
          ଯଦି (word_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_ସନ୍ଧାନ (&word_regex.pattern, cursor,
                                      context_end - cursor,
                                      0, context_end - cursor, &word_regs);
              ଯଦି (r == -2)
                matcher_error ();
              ଯଦି (r == -1)
                ଭାଙ୍ଗ;
              word_start = cursor + word_regs.start[0];
              word_end = cursor + word_regs.end[0];
            }
          ଅନ୍ୟଥା

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              scan = cursor;
              ଯେତେବେଳେ (scan < context_end
                     && !word_fastmap[to_uchar (*scan)])
                scan++;

              ଯଦି (scan == context_end)
                ଭାଙ୍ଗ;

              word_start = scan;

              ଯେତେବେଳେ (scan < context_end
                     && word_fastmap[to_uchar (*scan)])
                scan++;

              word_end = scan;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = word_start;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          ଯଦି (word_end == word_start)
            {
              cursor++;
              ଜାରି;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_ଚାବି.start = cursor;
          possible_ଚାବି.size = word_end - word_start;
          cursor += possible_ଚାବି.size;

          ଯଦି (possible_ଚାବି.size > maximum_word_length)
            maximum_word_length = possible_ଚାବି.size;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          ଯଦି (ନିବେଶ_reference)
            {
              ଯେତେବେଳେ (ଧାଡ଼ି_scan < possible_ଚାବି.start)
                ଯଦି (*ଧାଡ଼ି_scan == '\n')
                  {
                    total_ଧାଡ଼ି_ଗଣନା++;
                    ଧାଡ଼ି_scan++;
                    ଧାଡ଼ି_start = ଧାଡ଼ି_scan;
                    SKIP_NON_WHITE (ଧାଡ଼ି_scan, text_buffer->end);
                    reference_length = ଧାଡ଼ି_scan - ଧାଡ଼ି_start;
                  }
                ଅନ୍ୟଥା
                  ଧାଡ଼ି_scan++;
              ଯଦି (ଧାଡ଼ି_scan > possible_ଚାବି.start)
                ଜାରି;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          ଯଦି (ignore_file && ସନ୍ଧାନ_table (&possible_ଚାବି, &ignore_table))
            ଜାରି;
          ଯଦି (only_file && !ସନ୍ଧାନ_table (&possible_ଚାବି, &only_table))
            ଜାରି;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          ଯଦି (ସଂଖ୍ୟା_of_occurs[0] == occurs_alloc[0])
            occurs_table[0] = xpalloc (occurs_table[0], &occurs_alloc[0],
                                       1, -1, ଆକାର *occurs_table[0]);
          occurs_cursor = occurs_table[0] + ସଂଖ୍ୟା_of_occurs[0];

          /* Define the reference field, if any.  */

          ଯଦି (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              ଯେତେବେଳେ (ଧାଡ଼ି_scan < possible_ଚାବି.start)
                ଯଦି (*ଧାଡ଼ି_scan == '\n')
                  {
                    total_ଧାଡ଼ି_ଗଣନା++;
                    ଧାଡ଼ି_scan++;
                    ଧାଡ଼ି_start = ଧାଡ଼ି_scan;
                    SKIP_NON_WHITE (ଧାଡ଼ି_scan, text_buffer->end);
                  }
                ଅନ୍ୟଥା
                  ଧାଡ଼ି_scan++;

              occurs_cursor->reference = total_ଧାଡ଼ି_ଗଣନା;
            }
          ଅନ୍ୟଥା ଯଦି (ନିବେଶ_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = ଧାଡ଼ି_start - possible_ଚାବି.start;
              ଯଦି (reference_length > reference_max_width)
                reference_max_width = reference_length;
            }

          /* Exclude the reference from the context in simple cases.  */

          ଯଦି (ନିବେଶ_reference && ଧାଡ଼ି_start == context_start)
            {
              SKIP_NON_WHITE (context_start, context_end);
              SKIP_WHITE (context_start, context_end);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->ଚାବି = possible_ଚାବି;
          occurs_cursor->ବାମ = context_start - possible_ଚାବି.start;
          occurs_cursor->ଡାହାଣ = context_end - possible_ଚାବି.start;
          occurs_cursor->file_index = file_index;

          ସଂଖ୍ୟା_of_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_spaces (ptrdiff_t ସଂଖ୍ୟା)
{
  ପାଇଁ (ptrdiff_t counter = ସଂଖ୍ୟା; counter > 0; counter--)
    putchar (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_field (BLOCK field)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  ପାଇଁ (ଅକ୍ଷର *cursor = field.start; cursor < field.end; cursor++)
    {
      unsigned ଅକ୍ଷର character = *cursor;
      ଯଦି (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          ବାଛ (character)
            {
            ସ୍ଥିତି '"':
              /* In roff output format, double any quote.  */
              putchar ('"');
              putchar ('"');
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '$':
            ସ୍ଥିତି '%':
            ସ୍ଥିତି '&':
            ସ୍ଥିତି '#':
            ସ୍ଥିତି '_':
              /* In TeX output format, precede these with a backslash.  */
              putchar ('\\');
              putchar (character);
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '{':
            ସ୍ଥିତି '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              printf ("$\\%c$", character);
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              ଭାଙ୍ଗ;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              /* Any other flagged character produces a single space.  */
              putchar (' ');
            }
        }
      ଅନ୍ୟଥା
        putchar (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
fix_output_parameters (ଶୂନ୍ୟ)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  ଯଦି (auto_reference)
    {
      reference_max_width = 0;
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା file_index = 0; file_index < ସଂଖ୍ୟା_ନିବେଶ_files; file_index++)
        {
          intmax_t ଧାଡ଼ି_ordinal = file_ଧାଡ଼ି_ଗଣନା[file_index] + 1;
          ଯଦି (file_index > 0)
            ଧାଡ଼ି_ordinal -= file_ଧାଡ଼ି_ଗଣନା[file_index - 1];
          ଅକ୍ଷର ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_width = sprintf (ordinal_string, "%jd", ଧାଡ଼ି_ordinal);
          ଯଦି (ନିବେଶ_file_name[file_index])
            reference_width += strlen (ନିବେଶ_file_name[file_index]);
          ଯଦି (reference_width > reference_max_width)
            reference_max_width = reference_width;
        }
      reference_max_width++;
      reference.start = xmalloc (reference_max_width + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  ଯଦି ((auto_reference || ନିବେଶ_reference) && !ଡାହାଣ_reference)
    ଧାଡ଼ି_width = MAX (0, ଧାଡ଼ି_width - (reference_max_width + gap_size));

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

  half_ଧାଡ଼ି_width = ଧାଡ଼ି_width >> 1;
  before_max_width = half_ଧାଡ଼ି_width - gap_size;
  keyafter_max_width = half_ଧାଡ଼ି_width;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  ଯଦି (truncation_string && *truncation_string)
    truncation_string_length = strlen (truncation_string);
  ଅନ୍ୟଥା
    truncation_string = ଖାଲି;

  ଯଦି (gnu_extensions)
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
  ଅନ୍ୟଥା
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_width -= 2 * truncation_string_length + 1;
    }

  ଯଦି (before_max_width < 0)
    before_max_width = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା character = 0; character < CHAR_SET_SIZE; character++)
    edited_flag[character] = !! isspace (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  ବାଛ (output_format)
    {
    ସ୍ଥିତି UNKNOWN_FORMAT:
      /* Should never happen.  */

    ସ୍ଥିତି DUMB_FORMAT:
      ଭାଙ୍ଗ;

    ସ୍ଥିତି ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି TEX_FORMAT:

      /* Various characters need special processing.  */

      ପାଇଁ (ଅକ୍ଷର ଅଚଳ *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[to_uchar (*cursor)] = 1;

      ଭାଙ୍ଗ;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t tail_max_width;	/* allowable width of tail field */
  ptrdiff_t head_max_width;	/* allowable width of head field */
  ଅକ୍ଷର *cursor;			/* running cursor in source text */
  ଅକ୍ଷର *ବାମ_context_start;	/* start of left context */
  ଅକ୍ଷର *ଡାହାଣ_context_end;	/* end of right context */
  ଅକ୍ଷର *ବାମ_field_start;	/* conservative start for 'head'/'before' */
  ଅକ୍ଷର ଅଚଳ *file_name;	/* file name for reference */
  intmax_t ଧାଡ଼ି_ordinal;	/* line ordinal for reference */
  ଅକ୍ଷର ଅଚଳ *buffer_start;	/* start of buffered file for this occurs */
  ଅକ୍ଷର ଅଚଳ *buffer_end;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.start = occurs->ଚାବି.start;
  keyafter.end = keyafter.start + occurs->ଚାବି.size;
  ବାମ_context_start = keyafter.start + occurs->ବାମ;
  ଡାହାଣ_context_end = keyafter.start + occurs->ଡାହାଣ;

  buffer_start = text_buffers[occurs->file_index].start;
  buffer_end = text_buffers[occurs->file_index].end;

  cursor = keyafter.end;
  ଯେତେବେଳେ (cursor < ଡାହାଣ_context_end
         && cursor <= keyafter.start + keyafter_max_width)
    {
      keyafter.end = cursor;
      SKIP_SOMETHING (cursor, ଡାହାଣ_context_end);
    }
  ଯଦି (cursor <= keyafter.start + keyafter_max_width)
    keyafter.end = cursor;

  keyafter_truncation = truncation_string && keyafter.end < ଡାହାଣ_context_end;

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

  ଯଦି (-occurs->ବାମ > half_ଧାଡ଼ି_width + maximum_word_length)
    {
      ବାମ_field_start
        = keyafter.start - (half_ଧାଡ଼ି_width + maximum_word_length);
      SKIP_SOMETHING (ବାମ_field_start, keyafter.start);
    }
  ଅନ୍ୟଥା
    ବାମ_field_start = keyafter.start + occurs->ବାମ;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.start = ବାମ_field_start;
  before.end = keyafter.start;
  SKIP_WHITE_BACKWARDS (before.end, before.start);

  ଯେତେବେଳେ (before.start + before_max_width < before.end)
    SKIP_SOMETHING (before.start, before.end);

  ଯଦି (truncation_string)
    {
      cursor = before.start;
      SKIP_WHITE_BACKWARDS (cursor, buffer_start);
      before_truncation = cursor > ବାମ_context_start;
    }
  ଅନ୍ୟଥା
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

  ଯଦି (tail_max_width > 0)
    {
      tail.start = keyafter.end;
      SKIP_WHITE (tail.start, buffer_end);

      tail.end = tail.start;
      cursor = tail.end;
      ଯେତେବେଳେ (cursor < ଡାହାଣ_context_end
             && cursor < tail.start + tail_max_width)
        {
          tail.end = cursor;
          SKIP_SOMETHING (cursor, ଡାହାଣ_context_end);
        }

      ଯଦି (cursor < tail.start + tail_max_width)
        tail.end = cursor;

      ଯଦି (tail.end > tail.start)
        {
          keyafter_truncation = false;
          tail_truncation = truncation_string && tail.end < ଡାହାଣ_context_end;
        }
      ଅନ୍ୟଥା
        tail_truncation = false;

      SKIP_WHITE_BACKWARDS (tail.end, tail.start);
    }
  ଅନ୍ୟଥା
    {

      /* No place left for a tail field.  */

      tail.start = ଖାଲି;
      tail.end = ଖାଲି;
      tail_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  head_max_width
    = keyafter_max_width - (keyafter.end - keyafter.start) - gap_size;

  ଯଦି (head_max_width > 0)
    {
      head.end = before.start;
      SKIP_WHITE_BACKWARDS (head.end, buffer_start);

      head.start = ବାମ_field_start;
      ଯେତେବେଳେ (head.start + head_max_width < head.end)
        SKIP_SOMETHING (head.start, head.end);

      ଯଦି (head.end > head.start)
        {
          before_truncation = false;
          head_truncation = (truncation_string
                             && head.start > ବାମ_context_start);
        }
      ଅନ୍ୟଥା
        head_truncation = false;

      SKIP_WHITE (head.start, head.end);
    }
  ଅନ୍ୟଥା
    {

      /* No place left for a head field.  */

      head.start = ଖାଲି;
      head.end = ଖାଲି;
      head_truncation = false;
    }

  ଯଦି (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = ନିବେଶ_file_name[occurs->file_index];
      ଯଦି (!file_name)
        file_name = "";

      ଧାଡ଼ି_ordinal = occurs->reference + 1;
      ଯଦି (occurs->file_index > 0)
        ଧାଡ଼ି_ordinal -= file_ଧାଡ଼ି_ଗଣନା[occurs->file_index - 1];

      ଅକ୍ଷର *file_end = stpcpy (reference.start, file_name);
      reference.end = file_end + sprintf (file_end, ":%jd", ଧାଡ଼ି_ordinal);
    }
  ଅନ୍ୟଥା ଯଦି (ନିବେଶ_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.start = keyafter.start + occurs->reference;
      reference.end = reference.start;
      SKIP_NON_WHITE (reference.end, ଡାହାଣ_context_end);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
output_one_roff_ଧାଡ଼ି (ଶୂନ୍ୟ)
{
  /* Output the 'tail' field.  */

  printf (".%s \"", macro_name);
  print_field (tail);
  ଯଦି (tail_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  ଯଦି (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);
  putchar ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_field (keyafter);
  ଯଦି (keyafter_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  ଯଦି (head_truncation)
    fputs (truncation_string, stdout);
  print_field (head);
  putchar ('"');

  /* Conditionally output the 'reference' field.  */

  ଯଦି (auto_reference || ନିବେଶ_reference)
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

ସ୍ଥିର ଶୂନ୍ୟ
output_one_tex_ଧାଡ଼ି (ଶୂନ୍ୟ)
{
  BLOCK ଚାବି;			/* key field, isolated */
  BLOCK after;			/* after field, isolated */
  ଅକ୍ଷର *cursor;			/* running cursor in source text */

  printf ("\\%s ", macro_name);
  putchar ('{');
  print_field (tail);
  fputs ("}{", stdout);
  print_field (before);
  fputs ("}{", stdout);
  ଚାବି.start = keyafter.start;
  after.end = keyafter.end;
  cursor = keyafter.start;
  SKIP_SOMETHING (cursor, keyafter.end);
  ଚାବି.end = cursor;
  after.start = cursor;
  print_field (ଚାବି);
  fputs ("}{", stdout);
  print_field (after);
  fputs ("}{", stdout);
  print_field (head);
  putchar ('}');
  ଯଦି (auto_reference || ନିବେଶ_reference)
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

ସ୍ଥିର ଶୂନ୍ୟ
output_one_dumb_ଧାଡ଼ି (ଶୂନ୍ୟ)
{
  ଯଦି (!ଡାହାଣ_reference)
    {
      ଯଦି (auto_reference)
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
      ଅନ୍ୟଥା
        {

          /* Output the 'reference' field and its following gap.  */

          print_field (reference);
          print_spaces (reference_max_width
                        + gap_size
                        - (reference.end - reference.start));
        }
    }

  ଯଦି (tail.start < tail.end)
    {
      /* Output the 'tail' field.  */

      print_field (tail);
      ଯଦି (tail_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_ଧାଡ଼ି_width - gap_size
                    - (before.end - before.start)
                    - (before_truncation ? truncation_string_length : 0)
                    - (tail.end - tail.start)
                    - (tail_truncation ? truncation_string_length : 0));
    }
  ଅନ୍ୟଥା
    print_spaces (half_ଧାଡ଼ି_width - gap_size
                  - (before.end - before.start)
                  - (before_truncation ? truncation_string_length : 0));

  /* Output the 'before' field.  */

  ଯଦି (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);

  print_spaces (gap_size);

  /* Output the 'keyafter' field.  */

  print_field (keyafter);
  ଯଦି (keyafter_truncation)
    fputs (truncation_string, stdout);

  ଯଦି (head.start < head.end)
    {
      /* Output the 'head' field.  */

      print_spaces (half_ଧାଡ଼ି_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0)
                    - (head.end - head.start)
                    - (head_truncation ? truncation_string_length : 0));
      ଯଦି (head_truncation)
        fputs (truncation_string, stdout);
      print_field (head);
    }
  ଅନ୍ୟଥା

    ଯଦି ((auto_reference || ନିବେଶ_reference) && ଡାହାଣ_reference)
      print_spaces (half_ଧାଡ଼ି_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0));

  ଯଦି ((auto_reference || ନିବେଶ_reference) && ଡାହାଣ_reference)
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

ସ୍ଥିର ଶୂନ୍ୟ
generate_all_output (ଶୂନ୍ୟ)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  tail.start = ଖାଲି;
  tail.end = ଖାଲି;
  tail_truncation = false;

  head.start = ଖାଲି;
  head.end = ଖାଲି;
  head_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_table[0];

  ପାଇଁ (idx_t occurs_index = 0; occurs_index < ସଂଖ୍ୟା_of_occurs[0];
       occurs_index++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      ବାଛ (output_format)
        {
        ସ୍ଥିତି UNKNOWN_FORMAT:
          /* Should never happen.  */

        ସ୍ଥିତି DUMB_FORMAT:
          output_one_dumb_ଧାଡ଼ି ();
          ଭାଙ୍ଗ;

        ସ୍ଥିତି ROFF_FORMAT:
          output_one_roff_ଧାଡ଼ି ();
          ଭାଙ୍ଗ;

        ସ୍ଥିତି TEX_FORMAT:
          output_one_tex_ଧାଡ଼ି ();
          ଭାଙ୍ଗ;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [ନିବେଶ_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [ନିବେଶ_2 [OUTPUT]]\n"),
              program_name, program_name);
      fputs (_("\
Output a permuted index, including context, of the ଶବ୍ଦଗୁଡ଼ିକ in the ନିବେଶ files.\n\
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
  -F, --flag-truncation=STRING   use STRING ପାଇଁ flagging ଧାଡ଼ି truncations.\n\
                                 The ପୂର୍ବନିର୍ଦ୍ଧାରିତ is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name to use instead of 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --ଡାହାଣ-side-refs          put references at ଡାହାଣ, not counted in -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   ପାଇଁ end of ଧାଡ଼ିଗୁଡ଼ିକ or end of sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --word-regexp=REGEXP       use REGEXP to match each keyword\n\
"));
      oputs (_("\
  -b, --ଭାଙ୍ଗ-file=FILE          word ଭାଙ୍ଗ characters in this FILE\n\
"));
      oputs (_("\
  -f, --ignore-ସ୍ଥିତି              fold lower ସ୍ଥିତି to upper ସ୍ଥିତି ପାଇଁ sorting\n\
"));
      oputs (_("\
  -g, --gap-size=ସଂଖ୍ୟା_2          gap size in columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         ପଠ ignore word list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           ପଠ only word list from this FILE\n\
"));
      oputs (_("\
  -r, --references               first field of each ଧାଡ଼ି is a reference\n\
"));
      oputs (_("\
  -t, --typeset-mode             change the ପୂର୍ବନିର୍ଦ୍ଧାରିତ width from 72 to 100\n\
"));
      oputs (_("\
  -w, --width=ସଂଖ୍ୟା_2             output width in columns, reference excluded\n\
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
ଗଣନା
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"auto-reference", no_argument, ଖାଲି, 'A'},
  {"break-file", required_argument, ଖାଲି, 'b'},
  {"flag-truncation", required_argument, ଖାଲି, 'F'},
  {"ignore-case", no_argument, ଖାଲି, 'f'},
  {"gap-size", required_argument, ଖାଲି, 'g'},
  {"ignore-file", required_argument, ଖାଲି, 'i'},
  {"macro-name", required_argument, ଖାଲି, 'M'},
  {"only-file", required_argument, ଖାଲି, 'o'},
  {"references", no_argument, ଖାଲି, 'r'},
  {"right-side-refs", no_argument, ଖାଲି, 'R'},
  {"format", required_argument, ଖାଲି, FORMAT_OPTION},
  {"sentence-regexp", required_argument, ଖାଲି, 'S'},
  {"traditional", no_argument, ଖାଲି, 'G'},
  {"typeset-mode", no_argument, ଖାଲି, 't'},
  {"width", required_argument, ଖାଲି, 'w'},
  {"word-regexp", required_argument, ଖାଲି, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0},
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ format_args[] =
{
  "roff", "tex", ଖାଲି
};

ସ୍ଥିର ଗଣନା Format ଅଚଳ format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optchar;			/* argument character */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା file_index;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, ଖାଲି),
         optchar != EOF)
    {
      ବାଛ (optchar)
        {
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);

        ସ୍ଥିତି 'G':
          gnu_extensions = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'b':
          break_file = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'f':
          ignore_case = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'g':
          {
            intmax_t tmp;
            ଯଦି (! (xstrtoimax (optarg, ଖାଲି, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_size = tmp;
            ଭାଙ୍ଗ;
          }

        ସ୍ଥିତି 'i':
          ignore_file = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'o':
          only_file = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'r':
          ନିବେଶ_reference = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 't':
          ଯଦି (ଧାଡ଼ି_width < 0)
            ଧାଡ଼ି_width = 100;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'w':
          {
            intmax_t tmp;
            ଯଦି (! (xstrtoimax (optarg, ଖାଲି, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            ଧାଡ଼ି_width = tmp;
            ଭାଙ୍ଗ;
          }

        ସ୍ଥିତି 'A':
          auto_reference = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'M':
          macro_name = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'O':
          output_format = ROFF_FORMAT;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'R':
          ଡାହାଣ_reference = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'T':
          output_format = TEX_FORMAT;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'W':
          word_regex.string = optarg;
          unescape_string (optarg);
          ଯଦି (!*word_regex.string)
            word_regex.string = ଖାଲି;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_args, format_vals);
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        }
    }

  ଯଦି (ଧାଡ଼ି_width < 0)
    ଧାଡ଼ି_width = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  ଯଦି (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      ନିବେଶ_file_name = xmalloc (ଆକାର *ନିବେଶ_file_name);
      file_ଧାଡ଼ି_ଗଣନା = xmalloc (ଆକାର *file_ଧାଡ଼ି_ଗଣନା);
      text_buffers =    xmalloc (ଆକାର *text_buffers);
      ସଂଖ୍ୟା_ନିବେଶ_files = 1;
      ନିବେଶ_file_name[0] = ଖାଲି;
    }
  ଅନ୍ୟଥା ଯଦି (gnu_extensions)
    {
      ସଂଖ୍ୟା_ନିବେଶ_files = argc - optind;
      ନିବେଶ_file_name = xnmalloc (ସଂଖ୍ୟା_ନିବେଶ_files, ଆକାର *ନିବେଶ_file_name);
      file_ଧାଡ଼ି_ଗଣନା = xnmalloc (ସଂଖ୍ୟା_ନିବେଶ_files, ଆକାର *file_ଧାଡ଼ି_ଗଣନା);
      text_buffers    = xnmalloc (ସଂଖ୍ୟା_ନିବେଶ_files, ଆକାର *text_buffers);

      ପାଇଁ (file_index = 0; file_index < ସଂଖ୍ୟା_ନିବେଶ_files; file_index++)
        {
          ଯଦି (!*argv[optind] || streq (argv[optind], "-"))
            ନିବେଶ_file_name[file_index] = ଖାଲି;
          ଅନ୍ୟଥା
            ନିବେଶ_file_name[file_index] = argv[optind];
          optind++;
        }
    }
  ଅନ୍ୟଥା
    {

      /* There is one necessary input file.  */

      ସଂଖ୍ୟା_ନିବେଶ_files = 1;
      ନିବେଶ_file_name = xmalloc (ଆକାର *ନିବେଶ_file_name);
      file_ଧାଡ଼ି_ଗଣନା = xmalloc (ଆକାର *file_ଧାଡ଼ି_ଗଣନା);
      text_buffers    = xmalloc (ଆକାର *text_buffers);
      ଯଦି (!*argv[optind] || streq (argv[optind], "-"))
        ନିବେଶ_file_name[0] = ଖାଲି;
      ଅନ୍ୟଥା
        ନିବେଶ_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      ଯଦି (optind < argc)
        {
          ଯଦି (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      ଯଦି (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  ଯଦି (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  ଯଦି (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  ଯଦି (ignore_file)
    {
      digest_word_file (ignore_file, &ignore_table);
      ଯଦି (ignore_table.length == 0)
        ignore_file = ଖାଲି;
    }

  ଯଦି (only_file)
    {
      digest_word_file (only_file, &only_table);
      ଯଦି (only_table.length == 0)
        only_file = ଖାଲି;
    }

  /* Prepare to study all the input files.  */

  ସଂଖ୍ୟା_of_occurs[0] = 0;
  total_ଧାଡ଼ି_ଗଣନା = 0;
  maximum_word_length = 0;
  reference_max_width = 0;

  ପାଇଁ (file_index = 0; file_index < ସଂଖ୍ୟା_ନିବେଶ_files; file_index++)
    {
      BLOCK *text_buffer = text_buffers + file_index;

      /* Read the file contents into memory, then study it.  */

      swallow_file_in_memory (ନିବେଶ_file_name[file_index], text_buffer);
      find_occurs_in_text (file_index);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      total_ଧାଡ଼ି_ଗଣନା++;
      file_ଧାଡ଼ି_ଗଣନା[file_index] = total_ଧାଡ଼ି_ଗଣନା;
    }

  /* Do the output process phase.  */

  sort_found_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
