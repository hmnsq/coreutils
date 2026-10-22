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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"
#அந்தர்பாவ <regex.h>
#அந்தர்பாவ "argmatch.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "octhexdigits.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "read-file.h"
#அந்தர்பாவ "stdio--.h"
#அந்தர்பாவ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#define AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#define CHAR_SET_SIZE 256

/* Debugging the memory allocator.  */

#எனில் WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#endif

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

எண்ணிக்கை Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

நிலையான bool gnu_extensions = true;	/* trigger all GNU extensions */
நிலையான bool auto_reference = false;	/* refs are 'file_name:line_number:' */
நிலையான bool உள்ளீடு_reference = false;	/* refs at beginning of input lines */
நிலையான bool வலது_reference = false;	/* output refs after right context  */
நிலையான idx_t வரி_width = -1;		/* output line width in characters */
நிலையான idx_t gap_size = 3;	/* number of spaces between output fields */
நிலையான எழுத்து மாறிலி *truncation_string = "/";
                                /* string used to mark line truncations */
நிலையான எழுத்து மாறிலி *macro_name = "xx";	/* macro name for roff or TeX output */
நிலையான எண்ணிக்கை Format output_format = UNKNOWN_FORMAT;
                                /* output format */

நிலையான bool ignore_case = false;	/* fold lower to upper for sorting */
நிலையான எழுத்து மாறிலி *break_file = வெற்று; /* name of the 'Break chars' file */
நிலையான எழுத்து மாறிலி *only_file = வெற்று;	/* name of the 'Only words' file */
நிலையான எழுத்து மாறிலி *ignore_file = வெற்று; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
கட்டமைப்பு regex_data
{
  /* The original regular expression, as a string.  */
  எழுத்து மாறிலி *string;

  /* The compiled regular expression, and its fastmap.  */
  கட்டமைப்பு re_pattern_buffer pattern;
  எழுத்து fastmap[UCHAR_MAX + 1];
};

நிலையான கட்டமைப்பு regex_data context_regex;	/* end of context */
நிலையான கட்டமைப்பு regex_data word_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

வகைவரை கட்டமைப்பு
  {
    எழுத்து *start;		/* pointer to beginning of region */
    எழுத்து *end;			/* pointer to end + 1 of region */
  }
BLOCK;

வகைவரை கட்டமைப்பு
  {
    எழுத்து *start;		/* pointer to beginning of region */
    idx_t size;			/* length of the region */
  }
WORD;

வகைவரை கட்டமைப்பு
  {
    idx_t alloc;		/* allocated length */
    idx_t length;		/* number of used entries */
    WORD *start COUNTED_BY (length);	/* array of WORDs */
  }
WORD_TABLE;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
நிலையான unsigned எழுத்து folded_chars[CHAR_SET_SIZE];

/* End of context pattern register indices.  */
நிலையான கட்டமைப்பு re_registers context_regs;

/* Keyword pattern register indices.  */
நிலையான கட்டமைப்பு re_registers word_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
நிலையான எழுத்து word_fastmap[CHAR_SET_SIZE];

/* Maximum length of any word read.  */
நிலையான idx_t maximum_word_length;

/* Maximum width of any reference used.  */
நிலையான idx_t reference_max_width;

/* Ignore and Only word tables.  */

நிலையான WORD_TABLE ignore_table;	/* table of words to ignore */
நிலையான WORD_TABLE only_table;		/* table of words to select */

/* Source text table, and scanning macros.  */

நிலையான முழு எண்_உள்ளீடு_files;	/* number of text input files */
நிலையான intmax_t total_வரி_மொத்தம்;	/* total number of lines seen so far */
நிலையான எழுத்து மாறிலி **உள்ளீடு_file_name;	/* array of text input file names */
நிலையான intmax_t *file_வரி_மொத்தம்;	/* array of line count values at end */

நிலையான BLOCK *text_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#define SKIP_NON_WHITE(cursor, limit) \
  போது (cursor < limit && ! isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE(cursor, limit) \
  போது (cursor < limit && isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE_BACKWARDS(cursor, start) \
  போது (cursor > start && isspace (to_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#define SKIP_SOMETHING(cursor, limit) \
  எனில் (word_regex.string)						\
    {									\
      regoff_t மொத்தம்;							\
      மொத்தம் = re_match (&word_regex.pattern, cursor, limit - cursor,	\
                        0, வெற்று);					\
      எனில் (மொத்தம் == -2)							\
        matcher_error ();						\
      cursor += மொத்தம் <= 0 ? 1 : மொத்தம்;					\
    }									\
  இல்லையெனில் எனில் (word_fastmap[to_uchar (*cursor)])				\
    போது (cursor < limit && word_fastmap[to_uchar (*cursor)])		\
      cursor++;								\
  இல்லையெனில்									\
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

வகைவரை கட்டமைப்பு
  {
    WORD சாவி;			/* description of the keyword */
    ptrdiff_t இடது;		/* distance to left context start */
    ptrdiff_t வலது;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    முழு file_index;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

நிலையான OCCURS *occurs_table[1];	/* all words retained from the read text */
நிலையான idx_t occurs_alloc[1];	/* allocated size of occurs_table */
நிலையான idx_t எண்_of_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
நிலையான எழுத்து edited_flag[CHAR_SET_SIZE];

/* Half of line width, reference excluded.  */
நிலையான idx_t half_வரி_width;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
நிலையான ptrdiff_t before_max_width;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
நிலையான ptrdiff_t keyafter_max_width;

/* Length of string that flags truncation.  */
நிலையான idx_t truncation_string_length;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

நிலையான BLOCK tail;		/* tail field */
நிலையான bool tail_truncation;	/* flag truncation after the tail field */

நிலையான BLOCK before;		/* before field */
நிலையான bool before_truncation;	/* flag truncation before the before field */

நிலையான BLOCK keyafter;		/* keyword-and-after field */
நிலையான bool keyafter_truncation; /* flag truncation after the keyafter field */

நிலையான BLOCK head;		/* head field */
நிலையான bool head_truncation;	/* flag truncation before the head field */

நிலையான BLOCK reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

நிலையான _Noreturn வெறுமை
matcher_error (வெறுமை)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

நிலையான வெறுமை
unescape_string (எழுத்து *string)
{
  எழுத்து *cursor;			/* cursor in result */
  முழு மதிப்பு;			/* value of \nnn escape */
  முழு length;			/* length of \nnn escape */

  cursor = string;

  போது (*string)
    {
      எனில் (*string == '\\')
        {
          string++;
          தேர்வு (*string)
            {
            நிலை 'x':		/* \xhhh escape, 3 chars maximum */
              மதிப்பு = 0;
              க்கு (length = 0, string++;
                   length < 3 && c_isxdigit (*string);
                   length++, string++)
                மதிப்பு = மதிப்பு * 16 + fromhex (*string);
              எனில் (length == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              இல்லையெனில்
                *cursor++ = மதிப்பு;
              நிறுத்து;

            நிலை '0':		/* \0ooo escape, 3 chars maximum */
              மதிப்பு = 0;
              க்கு (length = 0, string++;
                   length < 3 && isoct (*string);
                   length++, string++)
                மதிப்பு = மதிப்பு * 8 + fromoct (*string);
              *cursor++ = மதிப்பு;
              நிறுத்து;

            நிலை 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              நிறுத்து;

            நிலை 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              நிறுத்து;

            நிலை 'c':		/* cancel the rest of the output */
              போது (*string)
                string++;
              நிறுத்து;

            நிலை 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              நிறுத்து;

            நிலை 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              நிறுத்து;

            நிலை 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              நிறுத்து;

            நிலை 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              நிறுத்து;

            நிலை 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              நிறுத்து;

            நிலை '\0':		/* lone backslash at end of string */
              /* ignore it */
              நிறுத்து;

            இயல்பு:
              *cursor++ = '\\';
              *cursor++ = *string++;
              நிறுத்து;
            }
        }
      இல்லையெனில்
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

நிலையான வெறுமை
compile_regex (கட்டமைப்பு regex_data *regex)
{
  கட்டமைப்பு re_pattern_buffer *pattern = &regex->pattern;
  எழுத்து மாறிலி *string = regex->string;
  எழுத்து மாறிலி *message;

  pattern->buffer = வெற்று;
  pattern->allocated = 0;
  pattern->fastmap = regex->fastmap;
  pattern->translate = ignore_case ? folded_chars : வெற்று;

  message = re_compile_pattern (string, strlen (string), pattern);
  எனில் (message)
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

நிலையான வெறுமை
initialize_regex (வெறுமை)
{
  /* Initialize the case folding table.  */

  எனில் (ignore_case)
    க்கு (முழு character = 0; character < CHAR_SET_SIZE; character++)
      folded_chars[character] = toupper (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  எனில் (context_regex.string)
    {
      எனில் (!*context_regex.string)
        context_regex.string = வெற்று;
    }
  இல்லையெனில் எனில் (gnu_extensions && !உள்ளீடு_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  இல்லையெனில்
    context_regex.string = "\n";

  எனில் (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  எனில் (word_regex.string)
    compile_regex (&word_regex);
  இல்லையெனில் எனில் (!break_file)
    {
      எனில் (gnu_extensions)
        {

          /* Simulate \w+.  */

          க்கு (முழு character = 0; character < CHAR_SET_SIZE; character++)
            word_fastmap[character] = !! isalpha (character);
        }
      இல்லையெனில்
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

நிலையான வெறுமை
swallow_file_in_memory (எழுத்து மாறிலி *file_name, BLOCK *block)
{
  size_t used_length;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  எனில் (using_stdin)
    block->start = fread_file (stdin, 0, &used_length);
  இல்லையெனில்
    block->start = read_file (file_name, 0, &used_length);

  எனில் (!block->start)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  எனில் (using_stdin)
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

நிலையான முழு
compare_சொற்கள் (மாறிலி வெறுமை *void_first, மாறிலி வெறுமை *void_second)
{
  WORD மாறிலி *first = void_first;
  WORD மாறிலி *second = void_second;
  idx_t length = MIN (first->size, second->size);

  எனில் (ignore_case)
    {
      க்கு (idx_t counter = 0; counter < length; counter++)
        {
          முழு மதிப்பு = (folded_chars[to_uchar (first->start[counter])]
                       - folded_chars[to_uchar (second->start[counter])]);
          எனில் (மதிப்பு != 0)
            திரும்பு மதிப்பு;
        }
    }
  இல்லையெனில்
    {
      க்கு (idx_t counter = 0; counter < length; counter++)
        {
          முழு மதிப்பு = (to_uchar (first->start[counter])
                       - to_uchar (second->start[counter]));
          எனில் (மதிப்பு != 0)
            திரும்பு மதிப்பு;
        }
    }

  திரும்பு _GL_CMP (first->size, second->size);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

நிலையான முழு
compare_occurs (மாறிலி வெறுமை *void_first, மாறிலி வெறுமை *void_second)
{
#define first ((மாறிலி OCCURS *) void_first)
#define second ((மாறிலி OCCURS *) void_second)
  முழு மதிப்பு;

  மதிப்பு = compare_சொற்கள் (&first->சாவி, &second->சாவி);
  திரும்பு (மதிப்பு ? மதிப்பு
          : _GL_CMP (first->சாவி.start, second->சாவி.start));
#undef first
#undef second
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_PURE
நிலையான bool
தேடல்_table (WORD *word, WORD_TABLE *table)
{
  idx_t lo = 0;
  idx_t hi = table->length;
  போது (lo < hi)
    {
      idx_t நடு = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      முழு மதிப்பு = compare_சொற்கள் (word, table->start + நடு);
      எனில் (மதிப்பு < 0)
        hi = நடு;
      இல்லையெனில் எனில் (மதிப்பு > 0)
        lo = நடு + 1;
      இல்லையெனில்
        திரும்பு true;
    }
  திரும்பு false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

நிலையான வெறுமை
sort_found_occurs (வெறுமை)
{

  /* Only one language for the time being.  */
  எனில் (எண்_of_occurs[0])
    qsort (occurs_table[0], எண்_of_occurs[0], அளவு **occurs_table,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

நிலையான வெறுமை
digest_break_file (எழுத்து மாறிலி *file_name)
{
  BLOCK file_contents;		/* to receive a copy of the file */

  swallow_file_in_memory (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (word_fastmap, 1, CHAR_SET_SIZE);
  க்கு (எழுத்து *cursor = file_contents.start; cursor < file_contents.end;
       cursor++)
    word_fastmap[to_uchar (*cursor)] = 0;

  எனில் (!gnu_extensions)
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

  விடுவி (file_contents.start);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

நிலையான வெறுமை
digest_word_file (எழுத்து மாறிலி *file_name, WORD_TABLE *table)
{
  BLOCK file_contents;		/* to receive a copy of the file */
  எழுத்து *cursor;			/* cursor in file copy */
  எழுத்து *word_start;		/* start of the current word */

  swallow_file_in_memory (file_name, &file_contents);

  table->start = வெற்று;
  table->alloc = 0;
  table->length = 0;

  /* Read the whole file.  */

  cursor = file_contents.start;
  போது (cursor < file_contents.end)
    {

      /* Read one line, and save the word in contains.  */

      word_start = cursor;
      போது (cursor < file_contents.end && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      எனில் (cursor > word_start)
        {
          எனில் (table->length == table->alloc)
            table->start = xpalloc (table->start, &table->alloc, 1, -1,
                                    அளவு *table->start);
          idx_t மாறிலி table_idx = table->length++;
          table->start[table_idx].start = word_start;
          table->start[table_idx].size = cursor - word_start;
        }

      /* This test allows for an incomplete line at end of file.  */

      எனில் (cursor < file_contents.end)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (table->start, table->length, அளவு table->start[0], compare_சொற்கள்);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

நிலையான வெறுமை
find_occurs_in_text (முழு file_index)
{
  எழுத்து *scan;			/* for scanning the source text */
  எழுத்து *வரி_start;		/* start of the current input line */
  எழுத்து *வரி_scan;		/* newlines scanned until this point */
  idx_t reference_length;	/* length of reference in input mode */
  WORD possible_சாவி;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  எழுத்து *context_start;		/* start of left context */
  எழுத்து *context_end;		/* end of right context */
  எழுத்து *word_start;		/* start of word */
  எழுத்து *word_end;		/* end of word */
  எழுத்து *next_context_start;	/* next start of left context */

  மாறிலி BLOCK *text_buffer = &text_buffers[file_index];

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

  வரி_start = text_buffer->start;
  வரி_scan = வரி_start;
  எனில் (உள்ளீடு_reference)
    {
      SKIP_NON_WHITE (வரி_scan, text_buffer->end);
      reference_length = வரி_scan - வரி_start;
      SKIP_WHITE (வரி_scan, text_buffer->end);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  க்கு (எழுத்து *cursor = text_buffer->start;
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
      எனில் (context_regex.string)
        தேர்வு (re_தேடல் (&context_regex.pattern, cursor,
                           text_buffer->end - cursor,
                           0, text_buffer->end - cursor, &context_regs))
          {
          நிலை -2:
            matcher_error ();

          நிலை -1:
            நிறுத்து;

          நிலை 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          இயல்பு:
            next_context_start = cursor + context_regs.end[0];
            நிறுத்து;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_end = next_context_start;
      SKIP_WHITE_BACKWARDS (context_end, context_start);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      போது (true)
        {
          எனில் (word_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_தேடல் (&word_regex.pattern, cursor,
                                      context_end - cursor,
                                      0, context_end - cursor, &word_regs);
              எனில் (r == -2)
                matcher_error ();
              எனில் (r == -1)
                நிறுத்து;
              word_start = cursor + word_regs.start[0];
              word_end = cursor + word_regs.end[0];
            }
          இல்லையெனில்

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              scan = cursor;
              போது (scan < context_end
                     && !word_fastmap[to_uchar (*scan)])
                scan++;

              எனில் (scan == context_end)
                நிறுத்து;

              word_start = scan;

              போது (scan < context_end
                     && word_fastmap[to_uchar (*scan)])
                scan++;

              word_end = scan;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = word_start;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          எனில் (word_end == word_start)
            {
              cursor++;
              தொடர்;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_சாவி.start = cursor;
          possible_சாவி.size = word_end - word_start;
          cursor += possible_சாவி.size;

          எனில் (possible_சாவி.size > maximum_word_length)
            maximum_word_length = possible_சாவி.size;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          எனில் (உள்ளீடு_reference)
            {
              போது (வரி_scan < possible_சாவி.start)
                எனில் (*வரி_scan == '\n')
                  {
                    total_வரி_மொத்தம்++;
                    வரி_scan++;
                    வரி_start = வரி_scan;
                    SKIP_NON_WHITE (வரி_scan, text_buffer->end);
                    reference_length = வரி_scan - வரி_start;
                  }
                இல்லையெனில்
                  வரி_scan++;
              எனில் (வரி_scan > possible_சாவி.start)
                தொடர்;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          எனில் (ignore_file && தேடல்_table (&possible_சாவி, &ignore_table))
            தொடர்;
          எனில் (only_file && !தேடல்_table (&possible_சாவி, &only_table))
            தொடர்;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          எனில் (எண்_of_occurs[0] == occurs_alloc[0])
            occurs_table[0] = xpalloc (occurs_table[0], &occurs_alloc[0],
                                       1, -1, அளவு *occurs_table[0]);
          occurs_cursor = occurs_table[0] + எண்_of_occurs[0];

          /* Define the reference field, if any.  */

          எனில் (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              போது (வரி_scan < possible_சாவி.start)
                எனில் (*வரி_scan == '\n')
                  {
                    total_வரி_மொத்தம்++;
                    வரி_scan++;
                    வரி_start = வரி_scan;
                    SKIP_NON_WHITE (வரி_scan, text_buffer->end);
                  }
                இல்லையெனில்
                  வரி_scan++;

              occurs_cursor->reference = total_வரி_மொத்தம்;
            }
          இல்லையெனில் எனில் (உள்ளீடு_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = வரி_start - possible_சாவி.start;
              எனில் (reference_length > reference_max_width)
                reference_max_width = reference_length;
            }

          /* Exclude the reference from the context in simple cases.  */

          எனில் (உள்ளீடு_reference && வரி_start == context_start)
            {
              SKIP_NON_WHITE (context_start, context_end);
              SKIP_WHITE (context_start, context_end);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->சாவி = possible_சாவி;
          occurs_cursor->இடது = context_start - possible_சாவி.start;
          occurs_cursor->வலது = context_end - possible_சாவி.start;
          occurs_cursor->file_index = file_index;

          எண்_of_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

நிலையான வெறுமை
print_spaces (ptrdiff_t எண்)
{
  க்கு (ptrdiff_t counter = எண்; counter > 0; counter--)
    putchar (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

நிலையான வெறுமை
print_field (BLOCK field)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  க்கு (எழுத்து *cursor = field.start; cursor < field.end; cursor++)
    {
      unsigned எழுத்து character = *cursor;
      எனில் (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          தேர்வு (character)
            {
            நிலை '"':
              /* In roff output format, double any quote.  */
              putchar ('"');
              putchar ('"');
              நிறுத்து;

            நிலை '$':
            நிலை '%':
            நிலை '&':
            நிலை '#':
            நிலை '_':
              /* In TeX output format, precede these with a backslash.  */
              putchar ('\\');
              putchar (character);
              நிறுத்து;

            நிலை '{':
            நிலை '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              printf ("$\\%c$", character);
              நிறுத்து;

            நிலை '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              நிறுத்து;

            இயல்பு:
              /* Any other flagged character produces a single space.  */
              putchar (' ');
            }
        }
      இல்லையெனில்
        putchar (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

நிலையான வெறுமை
fix_output_parameters (வெறுமை)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  எனில் (auto_reference)
    {
      reference_max_width = 0;
      க்கு (முழு file_index = 0; file_index < எண்_உள்ளீடு_files; file_index++)
        {
          intmax_t வரி_ordinal = file_வரி_மொத்தம்[file_index] + 1;
          எனில் (file_index > 0)
            வரி_ordinal -= file_வரி_மொத்தம்[file_index - 1];
          எழுத்து ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_width = sprintf (ordinal_string, "%jd", வரி_ordinal);
          எனில் (உள்ளீடு_file_name[file_index])
            reference_width += strlen (உள்ளீடு_file_name[file_index]);
          எனில் (reference_width > reference_max_width)
            reference_max_width = reference_width;
        }
      reference_max_width++;
      reference.start = xmalloc (reference_max_width + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  எனில் ((auto_reference || உள்ளீடு_reference) && !வலது_reference)
    வரி_width = MAX (0, வரி_width - (reference_max_width + gap_size));

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

  half_வரி_width = வரி_width >> 1;
  before_max_width = half_வரி_width - gap_size;
  keyafter_max_width = half_வரி_width;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  எனில் (truncation_string && *truncation_string)
    truncation_string_length = strlen (truncation_string);
  இல்லையெனில்
    truncation_string = வெற்று;

  எனில் (gnu_extensions)
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
  இல்லையெனில்
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_width -= 2 * truncation_string_length + 1;
    }

  எனில் (before_max_width < 0)
    before_max_width = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  க்கு (முழு character = 0; character < CHAR_SET_SIZE; character++)
    edited_flag[character] = !! isspace (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  தேர்வு (output_format)
    {
    நிலை UNKNOWN_FORMAT:
      /* Should never happen.  */

    நிலை DUMB_FORMAT:
      நிறுத்து;

    நிலை ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      நிறுத்து;

    நிலை TEX_FORMAT:

      /* Various characters need special processing.  */

      க்கு (எழுத்து மாறிலி *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[to_uchar (*cursor)] = 1;

      நிறுத்து;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

நிலையான வெறுமை
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t tail_max_width;	/* allowable width of tail field */
  ptrdiff_t head_max_width;	/* allowable width of head field */
  எழுத்து *cursor;			/* running cursor in source text */
  எழுத்து *இடது_context_start;	/* start of left context */
  எழுத்து *வலது_context_end;	/* end of right context */
  எழுத்து *இடது_field_start;	/* conservative start for 'head'/'before' */
  எழுத்து மாறிலி *file_name;	/* file name for reference */
  intmax_t வரி_ordinal;	/* line ordinal for reference */
  எழுத்து மாறிலி *buffer_start;	/* start of buffered file for this occurs */
  எழுத்து மாறிலி *buffer_end;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.start = occurs->சாவி.start;
  keyafter.end = keyafter.start + occurs->சாவி.size;
  இடது_context_start = keyafter.start + occurs->இடது;
  வலது_context_end = keyafter.start + occurs->வலது;

  buffer_start = text_buffers[occurs->file_index].start;
  buffer_end = text_buffers[occurs->file_index].end;

  cursor = keyafter.end;
  போது (cursor < வலது_context_end
         && cursor <= keyafter.start + keyafter_max_width)
    {
      keyafter.end = cursor;
      SKIP_SOMETHING (cursor, வலது_context_end);
    }
  எனில் (cursor <= keyafter.start + keyafter_max_width)
    keyafter.end = cursor;

  keyafter_truncation = truncation_string && keyafter.end < வலது_context_end;

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

  எனில் (-occurs->இடது > half_வரி_width + maximum_word_length)
    {
      இடது_field_start
        = keyafter.start - (half_வரி_width + maximum_word_length);
      SKIP_SOMETHING (இடது_field_start, keyafter.start);
    }
  இல்லையெனில்
    இடது_field_start = keyafter.start + occurs->இடது;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.start = இடது_field_start;
  before.end = keyafter.start;
  SKIP_WHITE_BACKWARDS (before.end, before.start);

  போது (before.start + before_max_width < before.end)
    SKIP_SOMETHING (before.start, before.end);

  எனில் (truncation_string)
    {
      cursor = before.start;
      SKIP_WHITE_BACKWARDS (cursor, buffer_start);
      before_truncation = cursor > இடது_context_start;
    }
  இல்லையெனில்
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

  எனில் (tail_max_width > 0)
    {
      tail.start = keyafter.end;
      SKIP_WHITE (tail.start, buffer_end);

      tail.end = tail.start;
      cursor = tail.end;
      போது (cursor < வலது_context_end
             && cursor < tail.start + tail_max_width)
        {
          tail.end = cursor;
          SKIP_SOMETHING (cursor, வலது_context_end);
        }

      எனில் (cursor < tail.start + tail_max_width)
        tail.end = cursor;

      எனில் (tail.end > tail.start)
        {
          keyafter_truncation = false;
          tail_truncation = truncation_string && tail.end < வலது_context_end;
        }
      இல்லையெனில்
        tail_truncation = false;

      SKIP_WHITE_BACKWARDS (tail.end, tail.start);
    }
  இல்லையெனில்
    {

      /* No place left for a tail field.  */

      tail.start = வெற்று;
      tail.end = வெற்று;
      tail_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  head_max_width
    = keyafter_max_width - (keyafter.end - keyafter.start) - gap_size;

  எனில் (head_max_width > 0)
    {
      head.end = before.start;
      SKIP_WHITE_BACKWARDS (head.end, buffer_start);

      head.start = இடது_field_start;
      போது (head.start + head_max_width < head.end)
        SKIP_SOMETHING (head.start, head.end);

      எனில் (head.end > head.start)
        {
          before_truncation = false;
          head_truncation = (truncation_string
                             && head.start > இடது_context_start);
        }
      இல்லையெனில்
        head_truncation = false;

      SKIP_WHITE (head.start, head.end);
    }
  இல்லையெனில்
    {

      /* No place left for a head field.  */

      head.start = வெற்று;
      head.end = வெற்று;
      head_truncation = false;
    }

  எனில் (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = உள்ளீடு_file_name[occurs->file_index];
      எனில் (!file_name)
        file_name = "";

      வரி_ordinal = occurs->reference + 1;
      எனில் (occurs->file_index > 0)
        வரி_ordinal -= file_வரி_மொத்தம்[occurs->file_index - 1];

      எழுத்து *file_end = stpcpy (reference.start, file_name);
      reference.end = file_end + sprintf (file_end, ":%jd", வரி_ordinal);
    }
  இல்லையெனில் எனில் (உள்ளீடு_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.start = keyafter.start + occurs->reference;
      reference.end = reference.start;
      SKIP_NON_WHITE (reference.end, வலது_context_end);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

நிலையான வெறுமை
output_one_roff_வரி (வெறுமை)
{
  /* Output the 'tail' field.  */

  printf (".%s \"", macro_name);
  print_field (tail);
  எனில் (tail_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  எனில் (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);
  putchar ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_field (keyafter);
  எனில் (keyafter_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  எனில் (head_truncation)
    fputs (truncation_string, stdout);
  print_field (head);
  putchar ('"');

  /* Conditionally output the 'reference' field.  */

  எனில் (auto_reference || உள்ளீடு_reference)
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

நிலையான வெறுமை
output_one_tex_வரி (வெறுமை)
{
  BLOCK சாவி;			/* key field, isolated */
  BLOCK after;			/* after field, isolated */
  எழுத்து *cursor;			/* running cursor in source text */

  printf ("\\%s ", macro_name);
  putchar ('{');
  print_field (tail);
  fputs ("}{", stdout);
  print_field (before);
  fputs ("}{", stdout);
  சாவி.start = keyafter.start;
  after.end = keyafter.end;
  cursor = keyafter.start;
  SKIP_SOMETHING (cursor, keyafter.end);
  சாவி.end = cursor;
  after.start = cursor;
  print_field (சாவி);
  fputs ("}{", stdout);
  print_field (after);
  fputs ("}{", stdout);
  print_field (head);
  putchar ('}');
  எனில் (auto_reference || உள்ளீடு_reference)
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

நிலையான வெறுமை
output_one_dumb_வரி (வெறுமை)
{
  எனில் (!வலது_reference)
    {
      எனில் (auto_reference)
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
      இல்லையெனில்
        {

          /* Output the 'reference' field and its following gap.  */

          print_field (reference);
          print_spaces (reference_max_width
                        + gap_size
                        - (reference.end - reference.start));
        }
    }

  எனில் (tail.start < tail.end)
    {
      /* Output the 'tail' field.  */

      print_field (tail);
      எனில் (tail_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_வரி_width - gap_size
                    - (before.end - before.start)
                    - (before_truncation ? truncation_string_length : 0)
                    - (tail.end - tail.start)
                    - (tail_truncation ? truncation_string_length : 0));
    }
  இல்லையெனில்
    print_spaces (half_வரி_width - gap_size
                  - (before.end - before.start)
                  - (before_truncation ? truncation_string_length : 0));

  /* Output the 'before' field.  */

  எனில் (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);

  print_spaces (gap_size);

  /* Output the 'keyafter' field.  */

  print_field (keyafter);
  எனில் (keyafter_truncation)
    fputs (truncation_string, stdout);

  எனில் (head.start < head.end)
    {
      /* Output the 'head' field.  */

      print_spaces (half_வரி_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0)
                    - (head.end - head.start)
                    - (head_truncation ? truncation_string_length : 0));
      எனில் (head_truncation)
        fputs (truncation_string, stdout);
      print_field (head);
    }
  இல்லையெனில்

    எனில் ((auto_reference || உள்ளீடு_reference) && வலது_reference)
      print_spaces (half_வரி_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0));

  எனில் ((auto_reference || உள்ளீடு_reference) && வலது_reference)
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

நிலையான வெறுமை
generate_all_output (வெறுமை)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  tail.start = வெற்று;
  tail.end = வெற்று;
  tail_truncation = false;

  head.start = வெற்று;
  head.end = வெற்று;
  head_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_table[0];

  க்கு (idx_t occurs_index = 0; occurs_index < எண்_of_occurs[0];
       occurs_index++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      தேர்வு (output_format)
        {
        நிலை UNKNOWN_FORMAT:
          /* Should never happen.  */

        நிலை DUMB_FORMAT:
          output_one_dumb_வரி ();
          நிறுத்து;

        நிலை ROFF_FORMAT:
          output_one_roff_வரி ();
          நிறுத்து;

        நிலை TEX_FORMAT:
          output_one_tex_வரி ();
          நிறுத்து;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [உள்ளீடு_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [உள்ளீடு_2 [OUTPUT]]\n"),
              program_name, program_name);
      fputs (_("\
Output a permuted index, including context, of the சொற்கள் in the உள்ளீடு files.\n\
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
  -F, --flag-truncation=STRING   use STRING க்கு flagging வரி truncations.\n\
                                 The இயல்பு is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name to use instead of 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --வலது-side-refs          put references at வலது, not counted in -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   க்கு end of வரிகள் or end of sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --word-regexp=REGEXP       use REGEXP to match each keyword\n\
"));
      oputs (_("\
  -b, --நிறுத்து-file=FILE          word நிறுத்து characters in this FILE\n\
"));
      oputs (_("\
  -f, --ignore-நிலை              fold lower நிலை to upper நிலை க்கு sorting\n\
"));
      oputs (_("\
  -g, --gap-size=எண்_2          gap size in columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         பட ignore word list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           பட only word list from this FILE\n\
"));
      oputs (_("\
  -r, --references               first field of each வரி is a reference\n\
"));
      oputs (_("\
  -t, --typeset-mode             change the இயல்பு width from 72 to 100\n\
"));
      oputs (_("\
  -w, --width=எண்_2             output width in columns, reference excluded\n\
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
எண்ணிக்கை
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"auto-reference", no_argument, வெற்று, 'A'},
  {"break-file", required_argument, வெற்று, 'b'},
  {"flag-truncation", required_argument, வெற்று, 'F'},
  {"ignore-case", no_argument, வெற்று, 'f'},
  {"gap-size", required_argument, வெற்று, 'g'},
  {"ignore-file", required_argument, வெற்று, 'i'},
  {"macro-name", required_argument, வெற்று, 'M'},
  {"only-file", required_argument, வெற்று, 'o'},
  {"references", no_argument, வெற்று, 'r'},
  {"right-side-refs", no_argument, வெற்று, 'R'},
  {"format", required_argument, வெற்று, FORMAT_OPTION},
  {"sentence-regexp", required_argument, வெற்று, 'S'},
  {"traditional", no_argument, வெற்று, 'G'},
  {"typeset-mode", no_argument, வெற்று, 't'},
  {"width", required_argument, வெற்று, 'w'},
  {"word-regexp", required_argument, வெற்று, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0},
};

நிலையான எழுத்து மாறிலி *மாறிலி format_args[] =
{
  "roff", "tex", வெற்று
};

நிலையான எண்ணிக்கை Format மாறிலி format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு optchar;			/* argument character */
  முழு file_index;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, வெற்று),
         optchar != EOF)
    {
      தேர்வு (optchar)
        {
        இயல்பு:
          usage (EXIT_FAILURE);

        நிலை 'G':
          gnu_extensions = false;
          நிறுத்து;

        நிலை 'b':
          break_file = optarg;
          நிறுத்து;

        நிலை 'f':
          ignore_case = true;
          நிறுத்து;

        நிலை 'g':
          {
            intmax_t tmp;
            எனில் (! (xstrtoimax (optarg, வெற்று, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_size = tmp;
            நிறுத்து;
          }

        நிலை 'i':
          ignore_file = optarg;
          நிறுத்து;

        நிலை 'o':
          only_file = optarg;
          நிறுத்து;

        நிலை 'r':
          உள்ளீடு_reference = true;
          நிறுத்து;

        நிலை 't':
          எனில் (வரி_width < 0)
            வரி_width = 100;
          நிறுத்து;

        நிலை 'w':
          {
            intmax_t tmp;
            எனில் (! (xstrtoimax (optarg, வெற்று, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            வரி_width = tmp;
            நிறுத்து;
          }

        நிலை 'A':
          auto_reference = true;
          நிறுத்து;

        நிலை 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          நிறுத்து;

        நிலை 'M':
          macro_name = optarg;
          நிறுத்து;

        நிலை 'O':
          output_format = ROFF_FORMAT;
          நிறுத்து;

        நிலை 'R':
          வலது_reference = true;
          நிறுத்து;

        நிலை 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          நிறுத்து;

        நிலை 'T':
          output_format = TEX_FORMAT;
          நிறுத்து;

        நிலை 'W':
          word_regex.string = optarg;
          unescape_string (optarg);
          எனில் (!*word_regex.string)
            word_regex.string = வெற்று;
          நிறுத்து;

        நிலை FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_args, format_vals);
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        }
    }

  எனில் (வரி_width < 0)
    வரி_width = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  எனில் (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      உள்ளீடு_file_name = xmalloc (அளவு *உள்ளீடு_file_name);
      file_வரி_மொத்தம் = xmalloc (அளவு *file_வரி_மொத்தம்);
      text_buffers =    xmalloc (அளவு *text_buffers);
      எண்_உள்ளீடு_files = 1;
      உள்ளீடு_file_name[0] = வெற்று;
    }
  இல்லையெனில் எனில் (gnu_extensions)
    {
      எண்_உள்ளீடு_files = argc - optind;
      உள்ளீடு_file_name = xnmalloc (எண்_உள்ளீடு_files, அளவு *உள்ளீடு_file_name);
      file_வரி_மொத்தம் = xnmalloc (எண்_உள்ளீடு_files, அளவு *file_வரி_மொத்தம்);
      text_buffers    = xnmalloc (எண்_உள்ளீடு_files, அளவு *text_buffers);

      க்கு (file_index = 0; file_index < எண்_உள்ளீடு_files; file_index++)
        {
          எனில் (!*argv[optind] || streq (argv[optind], "-"))
            உள்ளீடு_file_name[file_index] = வெற்று;
          இல்லையெனில்
            உள்ளீடு_file_name[file_index] = argv[optind];
          optind++;
        }
    }
  இல்லையெனில்
    {

      /* There is one necessary input file.  */

      எண்_உள்ளீடு_files = 1;
      உள்ளீடு_file_name = xmalloc (அளவு *உள்ளீடு_file_name);
      file_வரி_மொத்தம் = xmalloc (அளவு *file_வரி_மொத்தம்);
      text_buffers    = xmalloc (அளவு *text_buffers);
      எனில் (!*argv[optind] || streq (argv[optind], "-"))
        உள்ளீடு_file_name[0] = வெற்று;
      இல்லையெனில்
        உள்ளீடு_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      எனில் (optind < argc)
        {
          எனில் (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      எனில் (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  எனில் (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  எனில் (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  எனில் (ignore_file)
    {
      digest_word_file (ignore_file, &ignore_table);
      எனில் (ignore_table.length == 0)
        ignore_file = வெற்று;
    }

  எனில் (only_file)
    {
      digest_word_file (only_file, &only_table);
      எனில் (only_table.length == 0)
        only_file = வெற்று;
    }

  /* Prepare to study all the input files.  */

  எண்_of_occurs[0] = 0;
  total_வரி_மொத்தம் = 0;
  maximum_word_length = 0;
  reference_max_width = 0;

  க்கு (file_index = 0; file_index < எண்_உள்ளீடு_files; file_index++)
    {
      BLOCK *text_buffer = text_buffers + file_index;

      /* Read the file contents into memory, then study it.  */

      swallow_file_in_memory (உள்ளீடு_file_name[file_index], text_buffer);
      find_occurs_in_text (file_index);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      total_வரி_மொத்தம்++;
      file_வரி_மொத்தம்[file_index] = total_வரி_மொத்தம்;
    }

  /* Do the output process phase.  */

  sort_found_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  திரும்பு EXIT_SUCCESS;
}
