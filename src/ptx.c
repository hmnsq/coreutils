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

#شامل <config.h>

#شامل <getopt.h>
#شامل <sys/types.h>
#شامل "system.h"
#شامل <regex.h>
#شامل "argmatch.h"
#شامل "c-ctype.h"
#شامل "fadvise.h"
#شامل "octhexdigits.h"
#شامل "quote.h"
#شامل "read-file.h"
#شامل "stdio--.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#define AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#define CHAR_SET_SIZE 256

/* Debugging the memory allocator.  */

#اگر WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#endif

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

شمارش Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

ایستا bool gnu_extensions = true;	/* trigger all GNU extensions */
ایستا bool auto_reference = false;	/* refs are 'file_name:line_number:' */
ایستا bool درون_داد_reference = false;	/* refs at beginning of input lines */
ایستا bool راست_reference = false;	/* output refs after right context  */
ایستا idx_t سطر_width = -1;		/* output line width in characters */
ایستا idx_t gap_size = 3;	/* number of spaces between output fields */
ایستا نویسه ثابت *truncation_string = "/";
                                /* string used to mark line truncations */
ایستا نویسه ثابت *macro_name = "xx";	/* macro name for roff or TeX output */
ایستا شمارش Format output_format = UNKNOWN_FORMAT;
                                /* output format */

ایستا bool ignore_case = false;	/* fold lower to upper for sorting */
ایستا نویسه ثابت *break_file = پوچ; /* name of the 'Break chars' file */
ایستا نویسه ثابت *only_file = پوچ;	/* name of the 'Only words' file */
ایستا نویسه ثابت *ignore_file = پوچ; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
ساختار regex_data
{
  /* The original regular expression, as a string.  */
  نویسه ثابت *string;

  /* The compiled regular expression, and its fastmap.  */
  ساختار re_pattern_buffer pattern;
  نویسه fastmap[UCHAR_MAX + 1];
};

ایستا ساختار regex_data context_regex;	/* end of context */
ایستا ساختار regex_data word_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

تعریفنوع ساختار
  {
    نویسه *start;		/* pointer to beginning of region */
    نویسه *end;			/* pointer to end + 1 of region */
  }
BLOCK;

تعریفنوع ساختار
  {
    نویسه *start;		/* pointer to beginning of region */
    idx_t size;			/* length of the region */
  }
WORD;

تعریفنوع ساختار
  {
    idx_t alloc;		/* allocated length */
    idx_t length;		/* number of used entries */
    WORD *start COUNTED_BY (length);	/* array of WORDs */
  }
WORD_TABLE;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
ایستا unsigned نویسه folded_chars[CHAR_SET_SIZE];

/* End of context pattern register indices.  */
ایستا ساختار re_registers context_regs;

/* Keyword pattern register indices.  */
ایستا ساختار re_registers word_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
ایستا نویسه word_fastmap[CHAR_SET_SIZE];

/* Maximum length of any word read.  */
ایستا idx_t maximum_word_length;

/* Maximum width of any reference used.  */
ایستا idx_t reference_max_width;

/* Ignore and Only word tables.  */

ایستا WORD_TABLE ignore_table;	/* table of words to ignore */
ایستا WORD_TABLE only_table;		/* table of words to select */

/* Source text table, and scanning macros.  */

ایستا صحیح عدد_درون_داد_files;	/* number of text input files */
ایستا intmax_t total_سطر_شمار;	/* total number of lines seen so far */
ایستا نویسه ثابت **درون_داد_file_name;	/* array of text input file names */
ایستا intmax_t *file_سطر_شمار;	/* array of line count values at end */

ایستا BLOCK *text_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#define SKIP_NON_WHITE(cursor, limit) \
  وقتی (cursor < limit && ! isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE(cursor, limit) \
  وقتی (cursor < limit && isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE_BACKWARDS(cursor, start) \
  وقتی (cursor > start && isspace (to_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#define SKIP_SOMETHING(cursor, limit) \
  اگر (word_regex.string)						\
    {									\
      regoff_t شمار;							\
      شمار = re_match (&word_regex.pattern, cursor, limit - cursor,	\
                        0, پوچ);					\
      اگر (شمار == -2)							\
        matcher_error ();						\
      cursor += شمار <= 0 ? 1 : شمار;					\
    }									\
  وگرنه اگر (word_fastmap[to_uchar (*cursor)])				\
    وقتی (cursor < limit && word_fastmap[to_uchar (*cursor)])		\
      cursor++;								\
  وگرنه									\
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

تعریفنوع ساختار
  {
    WORD کلید;			/* description of the keyword */
    ptrdiff_t چپ;		/* distance to left context start */
    ptrdiff_t راست;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    صحیح file_index;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

ایستا OCCURS *occurs_table[1];	/* all words retained from the read text */
ایستا idx_t occurs_alloc[1];	/* allocated size of occurs_table */
ایستا idx_t عدد_of_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
ایستا نویسه edited_flag[CHAR_SET_SIZE];

/* Half of line width, reference excluded.  */
ایستا idx_t half_سطر_width;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ایستا ptrdiff_t before_max_width;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ایستا ptrdiff_t keyafter_max_width;

/* Length of string that flags truncation.  */
ایستا idx_t truncation_string_length;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

ایستا BLOCK tail;		/* tail field */
ایستا bool tail_truncation;	/* flag truncation after the tail field */

ایستا BLOCK before;		/* before field */
ایستا bool before_truncation;	/* flag truncation before the before field */

ایستا BLOCK keyafter;		/* keyword-and-after field */
ایستا bool keyafter_truncation; /* flag truncation after the keyafter field */

ایستا BLOCK head;		/* head field */
ایستا bool head_truncation;	/* flag truncation before the head field */

ایستا BLOCK reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

ایستا _Noreturn تهی
matcher_error (تهی)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

ایستا تهی
unescape_string (نویسه *string)
{
  نویسه *cursor;			/* cursor in result */
  صحیح مقدار;			/* value of \nnn escape */
  صحیح length;			/* length of \nnn escape */

  cursor = string;

  وقتی (*string)
    {
      اگر (*string == '\\')
        {
          string++;
          انتخاب (*string)
            {
            حالت 'x':		/* \xhhh escape, 3 chars maximum */
              مقدار = 0;
              برای (length = 0, string++;
                   length < 3 && c_isxdigit (*string);
                   length++, string++)
                مقدار = مقدار * 16 + fromhex (*string);
              اگر (length == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              وگرنه
                *cursor++ = مقدار;
              بشکن;

            حالت '0':		/* \0ooo escape, 3 chars maximum */
              مقدار = 0;
              برای (length = 0, string++;
                   length < 3 && isoct (*string);
                   length++, string++)
                مقدار = مقدار * 8 + fromoct (*string);
              *cursor++ = مقدار;
              بشکن;

            حالت 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              بشکن;

            حالت 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              بشکن;

            حالت 'c':		/* cancel the rest of the output */
              وقتی (*string)
                string++;
              بشکن;

            حالت 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              بشکن;

            حالت 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              بشکن;

            حالت 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              بشکن;

            حالت 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              بشکن;

            حالت 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              بشکن;

            حالت '\0':		/* lone backslash at end of string */
              /* ignore it */
              بشکن;

            پیشفرض:
              *cursor++ = '\\';
              *cursor++ = *string++;
              بشکن;
            }
        }
      وگرنه
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

ایستا تهی
compile_regex (ساختار regex_data *regex)
{
  ساختار re_pattern_buffer *pattern = &regex->pattern;
  نویسه ثابت *string = regex->string;
  نویسه ثابت *message;

  pattern->buffer = پوچ;
  pattern->allocated = 0;
  pattern->fastmap = regex->fastmap;
  pattern->translate = ignore_case ? folded_chars : پوچ;

  message = re_compile_pattern (string, strlen (string), pattern);
  اگر (message)
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

ایستا تهی
initialize_regex (تهی)
{
  /* Initialize the case folding table.  */

  اگر (ignore_case)
    برای (صحیح character = 0; character < CHAR_SET_SIZE; character++)
      folded_chars[character] = toupper (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  اگر (context_regex.string)
    {
      اگر (!*context_regex.string)
        context_regex.string = پوچ;
    }
  وگرنه اگر (gnu_extensions && !درون_داد_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  وگرنه
    context_regex.string = "\n";

  اگر (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  اگر (word_regex.string)
    compile_regex (&word_regex);
  وگرنه اگر (!break_file)
    {
      اگر (gnu_extensions)
        {

          /* Simulate \w+.  */

          برای (صحیح character = 0; character < CHAR_SET_SIZE; character++)
            word_fastmap[character] = !! isalpha (character);
        }
      وگرنه
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

ایستا تهی
swallow_file_in_memory (نویسه ثابت *file_name, BLOCK *block)
{
  size_t used_length;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  اگر (using_stdin)
    block->start = fread_file (stdin, 0, &used_length);
  وگرنه
    block->start = read_file (file_name, 0, &used_length);

  اگر (!block->start)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  اگر (using_stdin)
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

ایستا صحیح
compare_واژگان (ثابت تهی *void_first, ثابت تهی *void_second)
{
  WORD ثابت *first = void_first;
  WORD ثابت *second = void_second;
  idx_t length = MIN (first->size, second->size);

  اگر (ignore_case)
    {
      برای (idx_t counter = 0; counter < length; counter++)
        {
          صحیح مقدار = (folded_chars[to_uchar (first->start[counter])]
                       - folded_chars[to_uchar (second->start[counter])]);
          اگر (مقدار != 0)
            بازگشت مقدار;
        }
    }
  وگرنه
    {
      برای (idx_t counter = 0; counter < length; counter++)
        {
          صحیح مقدار = (to_uchar (first->start[counter])
                       - to_uchar (second->start[counter]));
          اگر (مقدار != 0)
            بازگشت مقدار;
        }
    }

  بازگشت _GL_CMP (first->size, second->size);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

ایستا صحیح
compare_occurs (ثابت تهی *void_first, ثابت تهی *void_second)
{
#define first ((ثابت OCCURS *) void_first)
#define second ((ثابت OCCURS *) void_second)
  صحیح مقدار;

  مقدار = compare_واژگان (&first->کلید, &second->کلید);
  بازگشت (مقدار ? مقدار
          : _GL_CMP (first->کلید.start, second->کلید.start));
#undef first
#undef second
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_PURE
ایستا bool
جستجو_table (WORD *word, WORD_TABLE *table)
{
  idx_t lo = 0;
  idx_t hi = table->length;
  وقتی (lo < hi)
    {
      idx_t میانه = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      صحیح مقدار = compare_واژگان (word, table->start + میانه);
      اگر (مقدار < 0)
        hi = میانه;
      وگرنه اگر (مقدار > 0)
        lo = میانه + 1;
      وگرنه
        بازگشت true;
    }
  بازگشت false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

ایستا تهی
sort_found_occurs (تهی)
{

  /* Only one language for the time being.  */
  اگر (عدد_of_occurs[0])
    qsort (occurs_table[0], عدد_of_occurs[0], اندازه **occurs_table,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

ایستا تهی
digest_break_file (نویسه ثابت *file_name)
{
  BLOCK file_contents;		/* to receive a copy of the file */

  swallow_file_in_memory (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (word_fastmap, 1, CHAR_SET_SIZE);
  برای (نویسه *cursor = file_contents.start; cursor < file_contents.end;
       cursor++)
    word_fastmap[to_uchar (*cursor)] = 0;

  اگر (!gnu_extensions)
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

  آزاد (file_contents.start);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

ایستا تهی
digest_word_file (نویسه ثابت *file_name, WORD_TABLE *table)
{
  BLOCK file_contents;		/* to receive a copy of the file */
  نویسه *cursor;			/* cursor in file copy */
  نویسه *word_start;		/* start of the current word */

  swallow_file_in_memory (file_name, &file_contents);

  table->start = پوچ;
  table->alloc = 0;
  table->length = 0;

  /* Read the whole file.  */

  cursor = file_contents.start;
  وقتی (cursor < file_contents.end)
    {

      /* Read one line, and save the word in contains.  */

      word_start = cursor;
      وقتی (cursor < file_contents.end && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      اگر (cursor > word_start)
        {
          اگر (table->length == table->alloc)
            table->start = xpalloc (table->start, &table->alloc, 1, -1,
                                    اندازه *table->start);
          idx_t ثابت table_idx = table->length++;
          table->start[table_idx].start = word_start;
          table->start[table_idx].size = cursor - word_start;
        }

      /* This test allows for an incomplete line at end of file.  */

      اگر (cursor < file_contents.end)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (table->start, table->length, اندازه table->start[0], compare_واژگان);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

ایستا تهی
find_occurs_in_text (صحیح file_index)
{
  نویسه *scan;			/* for scanning the source text */
  نویسه *سطر_start;		/* start of the current input line */
  نویسه *سطر_scan;		/* newlines scanned until this point */
  idx_t reference_length;	/* length of reference in input mode */
  WORD possible_کلید;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  نویسه *context_start;		/* start of left context */
  نویسه *context_end;		/* end of right context */
  نویسه *word_start;		/* start of word */
  نویسه *word_end;		/* end of word */
  نویسه *next_context_start;	/* next start of left context */

  ثابت BLOCK *text_buffer = &text_buffers[file_index];

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

  سطر_start = text_buffer->start;
  سطر_scan = سطر_start;
  اگر (درون_داد_reference)
    {
      SKIP_NON_WHITE (سطر_scan, text_buffer->end);
      reference_length = سطر_scan - سطر_start;
      SKIP_WHITE (سطر_scan, text_buffer->end);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  برای (نویسه *cursor = text_buffer->start;
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
      اگر (context_regex.string)
        انتخاب (re_جستجو (&context_regex.pattern, cursor,
                           text_buffer->end - cursor,
                           0, text_buffer->end - cursor, &context_regs))
          {
          حالت -2:
            matcher_error ();

          حالت -1:
            بشکن;

          حالت 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          پیشفرض:
            next_context_start = cursor + context_regs.end[0];
            بشکن;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_end = next_context_start;
      SKIP_WHITE_BACKWARDS (context_end, context_start);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      وقتی (true)
        {
          اگر (word_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_جستجو (&word_regex.pattern, cursor,
                                      context_end - cursor,
                                      0, context_end - cursor, &word_regs);
              اگر (r == -2)
                matcher_error ();
              اگر (r == -1)
                بشکن;
              word_start = cursor + word_regs.start[0];
              word_end = cursor + word_regs.end[0];
            }
          وگرنه

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              scan = cursor;
              وقتی (scan < context_end
                     && !word_fastmap[to_uchar (*scan)])
                scan++;

              اگر (scan == context_end)
                بشکن;

              word_start = scan;

              وقتی (scan < context_end
                     && word_fastmap[to_uchar (*scan)])
                scan++;

              word_end = scan;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = word_start;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          اگر (word_end == word_start)
            {
              cursor++;
              ادامه;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_کلید.start = cursor;
          possible_کلید.size = word_end - word_start;
          cursor += possible_کلید.size;

          اگر (possible_کلید.size > maximum_word_length)
            maximum_word_length = possible_کلید.size;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          اگر (درون_داد_reference)
            {
              وقتی (سطر_scan < possible_کلید.start)
                اگر (*سطر_scan == '\n')
                  {
                    total_سطر_شمار++;
                    سطر_scan++;
                    سطر_start = سطر_scan;
                    SKIP_NON_WHITE (سطر_scan, text_buffer->end);
                    reference_length = سطر_scan - سطر_start;
                  }
                وگرنه
                  سطر_scan++;
              اگر (سطر_scan > possible_کلید.start)
                ادامه;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          اگر (ignore_file && جستجو_table (&possible_کلید, &ignore_table))
            ادامه;
          اگر (only_file && !جستجو_table (&possible_کلید, &only_table))
            ادامه;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          اگر (عدد_of_occurs[0] == occurs_alloc[0])
            occurs_table[0] = xpalloc (occurs_table[0], &occurs_alloc[0],
                                       1, -1, اندازه *occurs_table[0]);
          occurs_cursor = occurs_table[0] + عدد_of_occurs[0];

          /* Define the reference field, if any.  */

          اگر (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              وقتی (سطر_scan < possible_کلید.start)
                اگر (*سطر_scan == '\n')
                  {
                    total_سطر_شمار++;
                    سطر_scan++;
                    سطر_start = سطر_scan;
                    SKIP_NON_WHITE (سطر_scan, text_buffer->end);
                  }
                وگرنه
                  سطر_scan++;

              occurs_cursor->reference = total_سطر_شمار;
            }
          وگرنه اگر (درون_داد_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = سطر_start - possible_کلید.start;
              اگر (reference_length > reference_max_width)
                reference_max_width = reference_length;
            }

          /* Exclude the reference from the context in simple cases.  */

          اگر (درون_داد_reference && سطر_start == context_start)
            {
              SKIP_NON_WHITE (context_start, context_end);
              SKIP_WHITE (context_start, context_end);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->کلید = possible_کلید;
          occurs_cursor->چپ = context_start - possible_کلید.start;
          occurs_cursor->راست = context_end - possible_کلید.start;
          occurs_cursor->file_index = file_index;

          عدد_of_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

ایستا تهی
print_spaces (ptrdiff_t عدد)
{
  برای (ptrdiff_t counter = عدد; counter > 0; counter--)
    putchar (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

ایستا تهی
print_field (BLOCK field)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  برای (نویسه *cursor = field.start; cursor < field.end; cursor++)
    {
      unsigned نویسه character = *cursor;
      اگر (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          انتخاب (character)
            {
            حالت '"':
              /* In roff output format, double any quote.  */
              putchar ('"');
              putchar ('"');
              بشکن;

            حالت '$':
            حالت '%':
            حالت '&':
            حالت '#':
            حالت '_':
              /* In TeX output format, precede these with a backslash.  */
              putchar ('\\');
              putchar (character);
              بشکن;

            حالت '{':
            حالت '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              printf ("$\\%c$", character);
              بشکن;

            حالت '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              بشکن;

            پیشفرض:
              /* Any other flagged character produces a single space.  */
              putchar (' ');
            }
        }
      وگرنه
        putchar (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

ایستا تهی
fix_output_parameters (تهی)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  اگر (auto_reference)
    {
      reference_max_width = 0;
      برای (صحیح file_index = 0; file_index < عدد_درون_داد_files; file_index++)
        {
          intmax_t سطر_ordinal = file_سطر_شمار[file_index] + 1;
          اگر (file_index > 0)
            سطر_ordinal -= file_سطر_شمار[file_index - 1];
          نویسه ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_width = sprintf (ordinal_string, "%jd", سطر_ordinal);
          اگر (درون_داد_file_name[file_index])
            reference_width += strlen (درون_داد_file_name[file_index]);
          اگر (reference_width > reference_max_width)
            reference_max_width = reference_width;
        }
      reference_max_width++;
      reference.start = xmalloc (reference_max_width + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  اگر ((auto_reference || درون_داد_reference) && !راست_reference)
    سطر_width = MAX (0, سطر_width - (reference_max_width + gap_size));

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

  half_سطر_width = سطر_width >> 1;
  before_max_width = half_سطر_width - gap_size;
  keyafter_max_width = half_سطر_width;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  اگر (truncation_string && *truncation_string)
    truncation_string_length = strlen (truncation_string);
  وگرنه
    truncation_string = پوچ;

  اگر (gnu_extensions)
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
  وگرنه
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_width -= 2 * truncation_string_length + 1;
    }

  اگر (before_max_width < 0)
    before_max_width = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  برای (صحیح character = 0; character < CHAR_SET_SIZE; character++)
    edited_flag[character] = !! isspace (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  انتخاب (output_format)
    {
    حالت UNKNOWN_FORMAT:
      /* Should never happen.  */

    حالت DUMB_FORMAT:
      بشکن;

    حالت ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      بشکن;

    حالت TEX_FORMAT:

      /* Various characters need special processing.  */

      برای (نویسه ثابت *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[to_uchar (*cursor)] = 1;

      بشکن;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

ایستا تهی
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t tail_max_width;	/* allowable width of tail field */
  ptrdiff_t head_max_width;	/* allowable width of head field */
  نویسه *cursor;			/* running cursor in source text */
  نویسه *چپ_context_start;	/* start of left context */
  نویسه *راست_context_end;	/* end of right context */
  نویسه *چپ_field_start;	/* conservative start for 'head'/'before' */
  نویسه ثابت *file_name;	/* file name for reference */
  intmax_t سطر_ordinal;	/* line ordinal for reference */
  نویسه ثابت *buffer_start;	/* start of buffered file for this occurs */
  نویسه ثابت *buffer_end;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.start = occurs->کلید.start;
  keyafter.end = keyafter.start + occurs->کلید.size;
  چپ_context_start = keyafter.start + occurs->چپ;
  راست_context_end = keyafter.start + occurs->راست;

  buffer_start = text_buffers[occurs->file_index].start;
  buffer_end = text_buffers[occurs->file_index].end;

  cursor = keyafter.end;
  وقتی (cursor < راست_context_end
         && cursor <= keyafter.start + keyafter_max_width)
    {
      keyafter.end = cursor;
      SKIP_SOMETHING (cursor, راست_context_end);
    }
  اگر (cursor <= keyafter.start + keyafter_max_width)
    keyafter.end = cursor;

  keyafter_truncation = truncation_string && keyafter.end < راست_context_end;

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

  اگر (-occurs->چپ > half_سطر_width + maximum_word_length)
    {
      چپ_field_start
        = keyafter.start - (half_سطر_width + maximum_word_length);
      SKIP_SOMETHING (چپ_field_start, keyafter.start);
    }
  وگرنه
    چپ_field_start = keyafter.start + occurs->چپ;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.start = چپ_field_start;
  before.end = keyafter.start;
  SKIP_WHITE_BACKWARDS (before.end, before.start);

  وقتی (before.start + before_max_width < before.end)
    SKIP_SOMETHING (before.start, before.end);

  اگر (truncation_string)
    {
      cursor = before.start;
      SKIP_WHITE_BACKWARDS (cursor, buffer_start);
      before_truncation = cursor > چپ_context_start;
    }
  وگرنه
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

  اگر (tail_max_width > 0)
    {
      tail.start = keyafter.end;
      SKIP_WHITE (tail.start, buffer_end);

      tail.end = tail.start;
      cursor = tail.end;
      وقتی (cursor < راست_context_end
             && cursor < tail.start + tail_max_width)
        {
          tail.end = cursor;
          SKIP_SOMETHING (cursor, راست_context_end);
        }

      اگر (cursor < tail.start + tail_max_width)
        tail.end = cursor;

      اگر (tail.end > tail.start)
        {
          keyafter_truncation = false;
          tail_truncation = truncation_string && tail.end < راست_context_end;
        }
      وگرنه
        tail_truncation = false;

      SKIP_WHITE_BACKWARDS (tail.end, tail.start);
    }
  وگرنه
    {

      /* No place left for a tail field.  */

      tail.start = پوچ;
      tail.end = پوچ;
      tail_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  head_max_width
    = keyafter_max_width - (keyafter.end - keyafter.start) - gap_size;

  اگر (head_max_width > 0)
    {
      head.end = before.start;
      SKIP_WHITE_BACKWARDS (head.end, buffer_start);

      head.start = چپ_field_start;
      وقتی (head.start + head_max_width < head.end)
        SKIP_SOMETHING (head.start, head.end);

      اگر (head.end > head.start)
        {
          before_truncation = false;
          head_truncation = (truncation_string
                             && head.start > چپ_context_start);
        }
      وگرنه
        head_truncation = false;

      SKIP_WHITE (head.start, head.end);
    }
  وگرنه
    {

      /* No place left for a head field.  */

      head.start = پوچ;
      head.end = پوچ;
      head_truncation = false;
    }

  اگر (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = درون_داد_file_name[occurs->file_index];
      اگر (!file_name)
        file_name = "";

      سطر_ordinal = occurs->reference + 1;
      اگر (occurs->file_index > 0)
        سطر_ordinal -= file_سطر_شمار[occurs->file_index - 1];

      نویسه *file_end = stpcpy (reference.start, file_name);
      reference.end = file_end + sprintf (file_end, ":%jd", سطر_ordinal);
    }
  وگرنه اگر (درون_داد_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.start = keyafter.start + occurs->reference;
      reference.end = reference.start;
      SKIP_NON_WHITE (reference.end, راست_context_end);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

ایستا تهی
output_one_roff_سطر (تهی)
{
  /* Output the 'tail' field.  */

  printf (".%s \"", macro_name);
  print_field (tail);
  اگر (tail_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  اگر (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);
  putchar ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_field (keyafter);
  اگر (keyafter_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  اگر (head_truncation)
    fputs (truncation_string, stdout);
  print_field (head);
  putchar ('"');

  /* Conditionally output the 'reference' field.  */

  اگر (auto_reference || درون_داد_reference)
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

ایستا تهی
output_one_tex_سطر (تهی)
{
  BLOCK کلید;			/* key field, isolated */
  BLOCK after;			/* after field, isolated */
  نویسه *cursor;			/* running cursor in source text */

  printf ("\\%s ", macro_name);
  putchar ('{');
  print_field (tail);
  fputs ("}{", stdout);
  print_field (before);
  fputs ("}{", stdout);
  کلید.start = keyafter.start;
  after.end = keyafter.end;
  cursor = keyafter.start;
  SKIP_SOMETHING (cursor, keyafter.end);
  کلید.end = cursor;
  after.start = cursor;
  print_field (کلید);
  fputs ("}{", stdout);
  print_field (after);
  fputs ("}{", stdout);
  print_field (head);
  putchar ('}');
  اگر (auto_reference || درون_داد_reference)
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

ایستا تهی
output_one_dumb_سطر (تهی)
{
  اگر (!راست_reference)
    {
      اگر (auto_reference)
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
      وگرنه
        {

          /* Output the 'reference' field and its following gap.  */

          print_field (reference);
          print_spaces (reference_max_width
                        + gap_size
                        - (reference.end - reference.start));
        }
    }

  اگر (tail.start < tail.end)
    {
      /* Output the 'tail' field.  */

      print_field (tail);
      اگر (tail_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_سطر_width - gap_size
                    - (before.end - before.start)
                    - (before_truncation ? truncation_string_length : 0)
                    - (tail.end - tail.start)
                    - (tail_truncation ? truncation_string_length : 0));
    }
  وگرنه
    print_spaces (half_سطر_width - gap_size
                  - (before.end - before.start)
                  - (before_truncation ? truncation_string_length : 0));

  /* Output the 'before' field.  */

  اگر (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);

  print_spaces (gap_size);

  /* Output the 'keyafter' field.  */

  print_field (keyafter);
  اگر (keyafter_truncation)
    fputs (truncation_string, stdout);

  اگر (head.start < head.end)
    {
      /* Output the 'head' field.  */

      print_spaces (half_سطر_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0)
                    - (head.end - head.start)
                    - (head_truncation ? truncation_string_length : 0));
      اگر (head_truncation)
        fputs (truncation_string, stdout);
      print_field (head);
    }
  وگرنه

    اگر ((auto_reference || درون_داد_reference) && راست_reference)
      print_spaces (half_سطر_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0));

  اگر ((auto_reference || درون_داد_reference) && راست_reference)
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

ایستا تهی
generate_all_output (تهی)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  tail.start = پوچ;
  tail.end = پوچ;
  tail_truncation = false;

  head.start = پوچ;
  head.end = پوچ;
  head_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_table[0];

  برای (idx_t occurs_index = 0; occurs_index < عدد_of_occurs[0];
       occurs_index++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      انتخاب (output_format)
        {
        حالت UNKNOWN_FORMAT:
          /* Should never happen.  */

        حالت DUMB_FORMAT:
          output_one_dumb_سطر ();
          بشکن;

        حالت ROFF_FORMAT:
          output_one_roff_سطر ();
          بشکن;

        حالت TEX_FORMAT:
          output_one_tex_سطر ();
          بشکن;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [درون_داد_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [درون_داد_2 [OUTPUT]]\n"),
              program_name, program_name);
      fputs (_("\
Output a permuted index, including context, of the واژگان in the درون_داد files.\n\
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
  -F, --flag-truncation=STRING   use STRING برای flagging سطر truncations.\n\
                                 The پیشفرض is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name to use instead of 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --راست-side-refs          put references at راست, not counted in -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   برای end of سطرها or end of sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --word-regexp=REGEXP       use REGEXP to match each keyword\n\
"));
      oputs (_("\
  -b, --بشکن-file=FILE          word بشکن characters in this FILE\n\
"));
      oputs (_("\
  -f, --ignore-حالت              fold lower حالت to upper حالت برای sorting\n\
"));
      oputs (_("\
  -g, --gap-size=عدد_2          gap size in columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         اقرأ ignore word list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           اقرأ only word list from this FILE\n\
"));
      oputs (_("\
  -r, --references               first field of each سطر is a reference\n\
"));
      oputs (_("\
  -t, --typeset-mode             change the پیشفرض width from 72 to 100\n\
"));
      oputs (_("\
  -w, --width=عدد_2             output width in columns, reference excluded\n\
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
شمارش
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
ایستا ساختار option ثابت long_options[] =
{
  {"auto-reference", no_argument, پوچ, 'A'},
  {"break-file", required_argument, پوچ, 'b'},
  {"flag-truncation", required_argument, پوچ, 'F'},
  {"ignore-case", no_argument, پوچ, 'f'},
  {"gap-size", required_argument, پوچ, 'g'},
  {"ignore-file", required_argument, پوچ, 'i'},
  {"macro-name", required_argument, پوچ, 'M'},
  {"only-file", required_argument, پوچ, 'o'},
  {"references", no_argument, پوچ, 'r'},
  {"right-side-refs", no_argument, پوچ, 'R'},
  {"format", required_argument, پوچ, FORMAT_OPTION},
  {"sentence-regexp", required_argument, پوچ, 'S'},
  {"traditional", no_argument, پوچ, 'G'},
  {"typeset-mode", no_argument, پوچ, 't'},
  {"width", required_argument, پوچ, 'w'},
  {"word-regexp", required_argument, پوچ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0},
};

ایستا نویسه ثابت *ثابت format_args[] =
{
  "roff", "tex", پوچ
};

ایستا شمارش Format ثابت format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح optchar;			/* argument character */
  صحیح file_index;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, پوچ),
         optchar != EOF)
    {
      انتخاب (optchar)
        {
        پیشفرض:
          usage (EXIT_FAILURE);

        حالت 'G':
          gnu_extensions = false;
          بشکن;

        حالت 'b':
          break_file = optarg;
          بشکن;

        حالت 'f':
          ignore_case = true;
          بشکن;

        حالت 'g':
          {
            intmax_t tmp;
            اگر (! (xstrtoimax (optarg, پوچ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_size = tmp;
            بشکن;
          }

        حالت 'i':
          ignore_file = optarg;
          بشکن;

        حالت 'o':
          only_file = optarg;
          بشکن;

        حالت 'r':
          درون_داد_reference = true;
          بشکن;

        حالت 't':
          اگر (سطر_width < 0)
            سطر_width = 100;
          بشکن;

        حالت 'w':
          {
            intmax_t tmp;
            اگر (! (xstrtoimax (optarg, پوچ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            سطر_width = tmp;
            بشکن;
          }

        حالت 'A':
          auto_reference = true;
          بشکن;

        حالت 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          بشکن;

        حالت 'M':
          macro_name = optarg;
          بشکن;

        حالت 'O':
          output_format = ROFF_FORMAT;
          بشکن;

        حالت 'R':
          راست_reference = true;
          بشکن;

        حالت 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          بشکن;

        حالت 'T':
          output_format = TEX_FORMAT;
          بشکن;

        حالت 'W':
          word_regex.string = optarg;
          unescape_string (optarg);
          اگر (!*word_regex.string)
            word_regex.string = پوچ;
          بشکن;

        حالت FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_args, format_vals);
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        }
    }

  اگر (سطر_width < 0)
    سطر_width = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  اگر (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      درون_داد_file_name = xmalloc (اندازه *درون_داد_file_name);
      file_سطر_شمار = xmalloc (اندازه *file_سطر_شمار);
      text_buffers =    xmalloc (اندازه *text_buffers);
      عدد_درون_داد_files = 1;
      درون_داد_file_name[0] = پوچ;
    }
  وگرنه اگر (gnu_extensions)
    {
      عدد_درون_داد_files = argc - optind;
      درون_داد_file_name = xnmalloc (عدد_درون_داد_files, اندازه *درون_داد_file_name);
      file_سطر_شمار = xnmalloc (عدد_درون_داد_files, اندازه *file_سطر_شمار);
      text_buffers    = xnmalloc (عدد_درون_داد_files, اندازه *text_buffers);

      برای (file_index = 0; file_index < عدد_درون_داد_files; file_index++)
        {
          اگر (!*argv[optind] || streq (argv[optind], "-"))
            درون_داد_file_name[file_index] = پوچ;
          وگرنه
            درون_داد_file_name[file_index] = argv[optind];
          optind++;
        }
    }
  وگرنه
    {

      /* There is one necessary input file.  */

      عدد_درون_داد_files = 1;
      درون_داد_file_name = xmalloc (اندازه *درون_داد_file_name);
      file_سطر_شمار = xmalloc (اندازه *file_سطر_شمار);
      text_buffers    = xmalloc (اندازه *text_buffers);
      اگر (!*argv[optind] || streq (argv[optind], "-"))
        درون_داد_file_name[0] = پوچ;
      وگرنه
        درون_داد_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      اگر (optind < argc)
        {
          اگر (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      اگر (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  اگر (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  اگر (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  اگر (ignore_file)
    {
      digest_word_file (ignore_file, &ignore_table);
      اگر (ignore_table.length == 0)
        ignore_file = پوچ;
    }

  اگر (only_file)
    {
      digest_word_file (only_file, &only_table);
      اگر (only_table.length == 0)
        only_file = پوچ;
    }

  /* Prepare to study all the input files.  */

  عدد_of_occurs[0] = 0;
  total_سطر_شمار = 0;
  maximum_word_length = 0;
  reference_max_width = 0;

  برای (file_index = 0; file_index < عدد_درون_داد_files; file_index++)
    {
      BLOCK *text_buffer = text_buffers + file_index;

      /* Read the file contents into memory, then study it.  */

      swallow_file_in_memory (درون_داد_file_name[file_index], text_buffer);
      find_occurs_in_text (file_index);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      total_سطر_شمار++;
      file_سطر_شمار[file_index] = total_سطر_شمار;
    }

  /* Do the output process phase.  */

  sort_found_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  بازگشت EXIT_SUCCESS;
}
