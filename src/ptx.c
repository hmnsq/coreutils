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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ <regex.h>
#অন্তর্ভাৱ "argmatch.h"
#অন্তর্ভাৱ "c-ctype.h"
#অন্তর্ভাৱ "fadvise.h"
#অন্তর্ভাৱ "octhexdigits.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "read-file.h"
#অন্তর্ভাৱ "stdio--.h"
#অন্তর্ভাৱ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#define AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#define CHAR_SET_SIZE 256

/* Debugging the memory allocator.  */

#যদি WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#endif

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

গণনা Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

স্থির bool gnu_extensions = true;	/* trigger all GNU extensions */
স্থির bool auto_reference = false;	/* refs are 'file_name:line_number:' */
স্থির bool প্ৰবেশ_reference = false;	/* refs at beginning of input lines */
স্থির bool ডান_reference = false;	/* output refs after right context  */
স্থির idx_t সাৰি_width = -1;		/* output line width in characters */
স্থির idx_t gap_size = 3;	/* number of spaces between output fields */
স্থির ৱর্ণ ধ্রুৱ *truncation_string = "/";
                                /* string used to mark line truncations */
স্থির ৱর্ণ ধ্রুৱ *macro_name = "xx";	/* macro name for roff or TeX output */
স্থির গণনা Format output_format = UNKNOWN_FORMAT;
                                /* output format */

স্থির bool ignore_case = false;	/* fold lower to upper for sorting */
স্থির ৱর্ণ ধ্রুৱ *break_file = রিক্ত; /* name of the 'Break chars' file */
স্থির ৱর্ণ ধ্রুৱ *only_file = রিক্ত;	/* name of the 'Only words' file */
স্থির ৱর্ণ ধ্রুৱ *ignore_file = রিক্ত; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
সংরচনা regex_data
{
  /* The original regular expression, as a string.  */
  ৱর্ণ ধ্রুৱ *string;

  /* The compiled regular expression, and its fastmap.  */
  সংরচনা re_pattern_buffer pattern;
  ৱর্ণ fastmap[UCHAR_MAX + 1];
};

স্থির সংরচনা regex_data context_regex;	/* end of context */
স্থির সংরচনা regex_data word_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

প্রকারনির্দেশ সংরচনা
  {
    ৱর্ণ *start;		/* pointer to beginning of region */
    ৱর্ণ *end;			/* pointer to end + 1 of region */
  }
BLOCK;

প্রকারনির্দেশ সংরচনা
  {
    ৱর্ণ *start;		/* pointer to beginning of region */
    idx_t size;			/* length of the region */
  }
WORD;

প্রকারনির্দেশ সংরচনা
  {
    idx_t alloc;		/* allocated length */
    idx_t length;		/* number of used entries */
    WORD *start COUNTED_BY (length);	/* array of WORDs */
  }
WORD_TABLE;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
স্থির unsigned ৱর্ণ folded_chars[CHAR_SET_SIZE];

/* End of context pattern register indices.  */
স্থির সংরচনা re_registers context_regs;

/* Keyword pattern register indices.  */
স্থির সংরচনা re_registers word_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
স্থির ৱর্ণ word_fastmap[CHAR_SET_SIZE];

/* Maximum length of any word read.  */
স্থির idx_t maximum_word_length;

/* Maximum width of any reference used.  */
স্থির idx_t reference_max_width;

/* Ignore and Only word tables.  */

স্থির WORD_TABLE ignore_table;	/* table of words to ignore */
স্থির WORD_TABLE only_table;		/* table of words to select */

/* Source text table, and scanning macros.  */

স্থির পূর্ণাঙ্ক সংখ্যা_প্ৰবেশ_files;	/* number of text input files */
স্থির intmax_t total_সাৰি_গুনতি;	/* total number of lines seen so far */
স্থির ৱর্ণ ধ্রুৱ **প্ৰবেশ_file_name;	/* array of text input file names */
স্থির intmax_t *file_সাৰি_গুনতি;	/* array of line count values at end */

স্থির BLOCK *text_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#define SKIP_NON_WHITE(cursor, limit) \
  যেতিয়ালৈ (cursor < limit && ! isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE(cursor, limit) \
  যেতিয়ালৈ (cursor < limit && isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE_BACKWARDS(cursor, start) \
  যেতিয়ালৈ (cursor > start && isspace (to_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#define SKIP_SOMETHING(cursor, limit) \
  যদি (word_regex.string)						\
    {									\
      regoff_t গুনতি;							\
      গুনতি = re_match (&word_regex.pattern, cursor, limit - cursor,	\
                        0, রিক্ত);					\
      যদি (গুনতি == -2)							\
        matcher_error ();						\
      cursor += গুনতি <= 0 ? 1 : গুনতি;					\
    }									\
  অন্যথা যদি (word_fastmap[to_uchar (*cursor)])				\
    যেতিয়ালৈ (cursor < limit && word_fastmap[to_uchar (*cursor)])		\
      cursor++;								\
  অন্যথা									\
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

প্রকারনির্দেশ সংরচনা
  {
    WORD চাবি;			/* description of the keyword */
    ptrdiff_t বাম;		/* distance to left context start */
    ptrdiff_t ডান;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    পূর্ণাঙ্ক file_index;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

স্থির OCCURS *occurs_table[1];	/* all words retained from the read text */
স্থির idx_t occurs_alloc[1];	/* allocated size of occurs_table */
স্থির idx_t সংখ্যা_of_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
স্থির ৱর্ণ edited_flag[CHAR_SET_SIZE];

/* Half of line width, reference excluded.  */
স্থির idx_t half_সাৰি_width;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
স্থির ptrdiff_t before_max_width;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
স্থির ptrdiff_t keyafter_max_width;

/* Length of string that flags truncation.  */
স্থির idx_t truncation_string_length;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

স্থির BLOCK tail;		/* tail field */
স্থির bool tail_truncation;	/* flag truncation after the tail field */

স্থির BLOCK before;		/* before field */
স্থির bool before_truncation;	/* flag truncation before the before field */

স্থির BLOCK keyafter;		/* keyword-and-after field */
স্থির bool keyafter_truncation; /* flag truncation after the keyafter field */

স্থির BLOCK head;		/* head field */
স্থির bool head_truncation;	/* flag truncation before the head field */

স্থির BLOCK reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

স্থির _Noreturn শূন্য
matcher_error (শূন্য)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

স্থির শূন্য
unescape_string (ৱর্ণ *string)
{
  ৱর্ণ *cursor;			/* cursor in result */
  পূর্ণাঙ্ক মান;			/* value of \nnn escape */
  পূর্ণাঙ্ক length;			/* length of \nnn escape */

  cursor = string;

  যেতিয়ালৈ (*string)
    {
      যদি (*string == '\\')
        {
          string++;
          ৱিকল্প (*string)
            {
            পক্ষ 'x':		/* \xhhh escape, 3 chars maximum */
              মান = 0;
              বাবে (length = 0, string++;
                   length < 3 && c_isxdigit (*string);
                   length++, string++)
                মান = মান * 16 + fromhex (*string);
              যদি (length == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              অন্যথা
                *cursor++ = মান;
              ভঙ্গ;

            পক্ষ '0':		/* \0ooo escape, 3 chars maximum */
              মান = 0;
              বাবে (length = 0, string++;
                   length < 3 && isoct (*string);
                   length++, string++)
                মান = মান * 8 + fromoct (*string);
              *cursor++ = মান;
              ভঙ্গ;

            পক্ষ 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              ভঙ্গ;

            পক্ষ 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              ভঙ্গ;

            পক্ষ 'c':		/* cancel the rest of the output */
              যেতিয়ালৈ (*string)
                string++;
              ভঙ্গ;

            পক্ষ 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              ভঙ্গ;

            পক্ষ 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              ভঙ্গ;

            পক্ষ 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              ভঙ্গ;

            পক্ষ 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              ভঙ্গ;

            পক্ষ 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              ভঙ্গ;

            পক্ষ '\0':		/* lone backslash at end of string */
              /* ignore it */
              ভঙ্গ;

            স্ৱভাৱ:
              *cursor++ = '\\';
              *cursor++ = *string++;
              ভঙ্গ;
            }
        }
      অন্যথা
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

স্থির শূন্য
compile_regex (সংরচনা regex_data *regex)
{
  সংরচনা re_pattern_buffer *pattern = &regex->pattern;
  ৱর্ণ ধ্রুৱ *string = regex->string;
  ৱর্ণ ধ্রুৱ *message;

  pattern->buffer = রিক্ত;
  pattern->allocated = 0;
  pattern->fastmap = regex->fastmap;
  pattern->translate = ignore_case ? folded_chars : রিক্ত;

  message = re_compile_pattern (string, strlen (string), pattern);
  যদি (message)
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

স্থির শূন্য
initialize_regex (শূন্য)
{
  /* Initialize the case folding table.  */

  যদি (ignore_case)
    বাবে (পূর্ণাঙ্ক character = 0; character < CHAR_SET_SIZE; character++)
      folded_chars[character] = toupper (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  যদি (context_regex.string)
    {
      যদি (!*context_regex.string)
        context_regex.string = রিক্ত;
    }
  অন্যথা যদি (gnu_extensions && !প্ৰবেশ_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  অন্যথা
    context_regex.string = "\n";

  যদি (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  যদি (word_regex.string)
    compile_regex (&word_regex);
  অন্যথা যদি (!break_file)
    {
      যদি (gnu_extensions)
        {

          /* Simulate \w+.  */

          বাবে (পূর্ণাঙ্ক character = 0; character < CHAR_SET_SIZE; character++)
            word_fastmap[character] = !! isalpha (character);
        }
      অন্যথা
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

স্থির শূন্য
swallow_file_in_memory (ৱর্ণ ধ্রুৱ *file_name, BLOCK *block)
{
  size_t used_length;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  যদি (using_stdin)
    block->start = fread_file (stdin, 0, &used_length);
  অন্যথা
    block->start = read_file (file_name, 0, &used_length);

  যদি (!block->start)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  যদি (using_stdin)
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

স্থির পূর্ণাঙ্ক
compare_শব্দগুলি (ধ্রুৱ শূন্য *void_first, ধ্রুৱ শূন্য *void_second)
{
  WORD ধ্রুৱ *first = void_first;
  WORD ধ্রুৱ *second = void_second;
  idx_t length = MIN (first->size, second->size);

  যদি (ignore_case)
    {
      বাবে (idx_t counter = 0; counter < length; counter++)
        {
          পূর্ণাঙ্ক মান = (folded_chars[to_uchar (first->start[counter])]
                       - folded_chars[to_uchar (second->start[counter])]);
          যদি (মান != 0)
            ঘূৰাই মান;
        }
    }
  অন্যথা
    {
      বাবে (idx_t counter = 0; counter < length; counter++)
        {
          পূর্ণাঙ্ক মান = (to_uchar (first->start[counter])
                       - to_uchar (second->start[counter]));
          যদি (মান != 0)
            ঘূৰাই মান;
        }
    }

  ঘূৰাই _GL_CMP (first->size, second->size);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

স্থির পূর্ণাঙ্ক
compare_occurs (ধ্রুৱ শূন্য *void_first, ধ্রুৱ শূন্য *void_second)
{
#define first ((ধ্রুৱ OCCURS *) void_first)
#define second ((ধ্রুৱ OCCURS *) void_second)
  পূর্ণাঙ্ক মান;

  মান = compare_শব্দগুলি (&first->চাবি, &second->চাবি);
  ঘূৰাই (মান ? মান
          : _GL_CMP (first->চাবি.start, second->চাবি.start));
#undef first
#undef second
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_PURE
স্থির bool
অনুসন্ধান_table (WORD *word, WORD_TABLE *table)
{
  idx_t lo = 0;
  idx_t hi = table->length;
  যেতিয়ালৈ (lo < hi)
    {
      idx_t মধ্য = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      পূর্ণাঙ্ক মান = compare_শব্দগুলি (word, table->start + মধ্য);
      যদি (মান < 0)
        hi = মধ্য;
      অন্যথা যদি (মান > 0)
        lo = মধ্য + 1;
      অন্যথা
        ঘূৰাই true;
    }
  ঘূৰাই false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

স্থির শূন্য
sort_found_occurs (শূন্য)
{

  /* Only one language for the time being.  */
  যদি (সংখ্যা_of_occurs[0])
    qsort (occurs_table[0], সংখ্যা_of_occurs[0], পরিমাণ **occurs_table,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

স্থির শূন্য
digest_break_file (ৱর্ণ ধ্রুৱ *file_name)
{
  BLOCK file_contents;		/* to receive a copy of the file */

  swallow_file_in_memory (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (word_fastmap, 1, CHAR_SET_SIZE);
  বাবে (ৱর্ণ *cursor = file_contents.start; cursor < file_contents.end;
       cursor++)
    word_fastmap[to_uchar (*cursor)] = 0;

  যদি (!gnu_extensions)
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

  মুক্ত (file_contents.start);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

স্থির শূন্য
digest_word_file (ৱর্ণ ধ্রুৱ *file_name, WORD_TABLE *table)
{
  BLOCK file_contents;		/* to receive a copy of the file */
  ৱর্ণ *cursor;			/* cursor in file copy */
  ৱর্ণ *word_start;		/* start of the current word */

  swallow_file_in_memory (file_name, &file_contents);

  table->start = রিক্ত;
  table->alloc = 0;
  table->length = 0;

  /* Read the whole file.  */

  cursor = file_contents.start;
  যেতিয়ালৈ (cursor < file_contents.end)
    {

      /* Read one line, and save the word in contains.  */

      word_start = cursor;
      যেতিয়ালৈ (cursor < file_contents.end && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      যদি (cursor > word_start)
        {
          যদি (table->length == table->alloc)
            table->start = xpalloc (table->start, &table->alloc, 1, -1,
                                    পরিমাণ *table->start);
          idx_t ধ্রুৱ table_idx = table->length++;
          table->start[table_idx].start = word_start;
          table->start[table_idx].size = cursor - word_start;
        }

      /* This test allows for an incomplete line at end of file.  */

      যদি (cursor < file_contents.end)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (table->start, table->length, পরিমাণ table->start[0], compare_শব্দগুলি);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

স্থির শূন্য
find_occurs_in_text (পূর্ণাঙ্ক file_index)
{
  ৱর্ণ *scan;			/* for scanning the source text */
  ৱর্ণ *সাৰি_start;		/* start of the current input line */
  ৱর্ণ *সাৰি_scan;		/* newlines scanned until this point */
  idx_t reference_length;	/* length of reference in input mode */
  WORD possible_চাবি;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  ৱর্ণ *context_start;		/* start of left context */
  ৱর্ণ *context_end;		/* end of right context */
  ৱর্ণ *word_start;		/* start of word */
  ৱর্ণ *word_end;		/* end of word */
  ৱর্ণ *next_context_start;	/* next start of left context */

  ধ্রুৱ BLOCK *text_buffer = &text_buffers[file_index];

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

  সাৰি_start = text_buffer->start;
  সাৰি_scan = সাৰি_start;
  যদি (প্ৰবেশ_reference)
    {
      SKIP_NON_WHITE (সাৰি_scan, text_buffer->end);
      reference_length = সাৰি_scan - সাৰি_start;
      SKIP_WHITE (সাৰি_scan, text_buffer->end);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  বাবে (ৱর্ণ *cursor = text_buffer->start;
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
      যদি (context_regex.string)
        ৱিকল্প (re_অনুসন্ধান (&context_regex.pattern, cursor,
                           text_buffer->end - cursor,
                           0, text_buffer->end - cursor, &context_regs))
          {
          পক্ষ -2:
            matcher_error ();

          পক্ষ -1:
            ভঙ্গ;

          পক্ষ 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          স্ৱভাৱ:
            next_context_start = cursor + context_regs.end[0];
            ভঙ্গ;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_end = next_context_start;
      SKIP_WHITE_BACKWARDS (context_end, context_start);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      যেতিয়ালৈ (true)
        {
          যদি (word_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_অনুসন্ধান (&word_regex.pattern, cursor,
                                      context_end - cursor,
                                      0, context_end - cursor, &word_regs);
              যদি (r == -2)
                matcher_error ();
              যদি (r == -1)
                ভঙ্গ;
              word_start = cursor + word_regs.start[0];
              word_end = cursor + word_regs.end[0];
            }
          অন্যথা

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              scan = cursor;
              যেতিয়ালৈ (scan < context_end
                     && !word_fastmap[to_uchar (*scan)])
                scan++;

              যদি (scan == context_end)
                ভঙ্গ;

              word_start = scan;

              যেতিয়ালৈ (scan < context_end
                     && word_fastmap[to_uchar (*scan)])
                scan++;

              word_end = scan;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = word_start;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          যদি (word_end == word_start)
            {
              cursor++;
              অনুৱর্ত;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_চাবি.start = cursor;
          possible_চাবি.size = word_end - word_start;
          cursor += possible_চাবি.size;

          যদি (possible_চাবি.size > maximum_word_length)
            maximum_word_length = possible_চাবি.size;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          যদি (প্ৰবেশ_reference)
            {
              যেতিয়ালৈ (সাৰি_scan < possible_চাবি.start)
                যদি (*সাৰি_scan == '\n')
                  {
                    total_সাৰি_গুনতি++;
                    সাৰি_scan++;
                    সাৰি_start = সাৰি_scan;
                    SKIP_NON_WHITE (সাৰি_scan, text_buffer->end);
                    reference_length = সাৰি_scan - সাৰি_start;
                  }
                অন্যথা
                  সাৰি_scan++;
              যদি (সাৰি_scan > possible_চাবি.start)
                অনুৱর্ত;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          যদি (ignore_file && অনুসন্ধান_table (&possible_চাবি, &ignore_table))
            অনুৱর্ত;
          যদি (only_file && !অনুসন্ধান_table (&possible_চাবি, &only_table))
            অনুৱর্ত;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          যদি (সংখ্যা_of_occurs[0] == occurs_alloc[0])
            occurs_table[0] = xpalloc (occurs_table[0], &occurs_alloc[0],
                                       1, -1, পরিমাণ *occurs_table[0]);
          occurs_cursor = occurs_table[0] + সংখ্যা_of_occurs[0];

          /* Define the reference field, if any.  */

          যদি (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              যেতিয়ালৈ (সাৰি_scan < possible_চাবি.start)
                যদি (*সাৰি_scan == '\n')
                  {
                    total_সাৰি_গুনতি++;
                    সাৰি_scan++;
                    সাৰি_start = সাৰি_scan;
                    SKIP_NON_WHITE (সাৰি_scan, text_buffer->end);
                  }
                অন্যথা
                  সাৰি_scan++;

              occurs_cursor->reference = total_সাৰি_গুনতি;
            }
          অন্যথা যদি (প্ৰবেশ_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = সাৰি_start - possible_চাবি.start;
              যদি (reference_length > reference_max_width)
                reference_max_width = reference_length;
            }

          /* Exclude the reference from the context in simple cases.  */

          যদি (প্ৰবেশ_reference && সাৰি_start == context_start)
            {
              SKIP_NON_WHITE (context_start, context_end);
              SKIP_WHITE (context_start, context_end);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->চাবি = possible_চাবি;
          occurs_cursor->বাম = context_start - possible_চাবি.start;
          occurs_cursor->ডান = context_end - possible_চাবি.start;
          occurs_cursor->file_index = file_index;

          সংখ্যা_of_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

স্থির শূন্য
print_spaces (ptrdiff_t সংখ্যা)
{
  বাবে (ptrdiff_t counter = সংখ্যা; counter > 0; counter--)
    putchar (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

স্থির শূন্য
print_field (BLOCK field)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  বাবে (ৱর্ণ *cursor = field.start; cursor < field.end; cursor++)
    {
      unsigned ৱর্ণ character = *cursor;
      যদি (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          ৱিকল্প (character)
            {
            পক্ষ '"':
              /* In roff output format, double any quote.  */
              putchar ('"');
              putchar ('"');
              ভঙ্গ;

            পক্ষ '$':
            পক্ষ '%':
            পক্ষ '&':
            পক্ষ '#':
            পক্ষ '_':
              /* In TeX output format, precede these with a backslash.  */
              putchar ('\\');
              putchar (character);
              ভঙ্গ;

            পক্ষ '{':
            পক্ষ '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              printf ("$\\%c$", character);
              ভঙ্গ;

            পক্ষ '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              ভঙ্গ;

            স্ৱভাৱ:
              /* Any other flagged character produces a single space.  */
              putchar (' ');
            }
        }
      অন্যথা
        putchar (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

স্থির শূন্য
fix_output_parameters (শূন্য)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  যদি (auto_reference)
    {
      reference_max_width = 0;
      বাবে (পূর্ণাঙ্ক file_index = 0; file_index < সংখ্যা_প্ৰবেশ_files; file_index++)
        {
          intmax_t সাৰি_ordinal = file_সাৰি_গুনতি[file_index] + 1;
          যদি (file_index > 0)
            সাৰি_ordinal -= file_সাৰি_গুনতি[file_index - 1];
          ৱর্ণ ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_width = sprintf (ordinal_string, "%jd", সাৰি_ordinal);
          যদি (প্ৰবেশ_file_name[file_index])
            reference_width += strlen (প্ৰবেশ_file_name[file_index]);
          যদি (reference_width > reference_max_width)
            reference_max_width = reference_width;
        }
      reference_max_width++;
      reference.start = xmalloc (reference_max_width + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  যদি ((auto_reference || প্ৰবেশ_reference) && !ডান_reference)
    সাৰি_width = MAX (0, সাৰি_width - (reference_max_width + gap_size));

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

  half_সাৰি_width = সাৰি_width >> 1;
  before_max_width = half_সাৰি_width - gap_size;
  keyafter_max_width = half_সাৰি_width;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  যদি (truncation_string && *truncation_string)
    truncation_string_length = strlen (truncation_string);
  অন্যথা
    truncation_string = রিক্ত;

  যদি (gnu_extensions)
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
  অন্যথা
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_width -= 2 * truncation_string_length + 1;
    }

  যদি (before_max_width < 0)
    before_max_width = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  বাবে (পূর্ণাঙ্ক character = 0; character < CHAR_SET_SIZE; character++)
    edited_flag[character] = !! isspace (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  ৱিকল্প (output_format)
    {
    পক্ষ UNKNOWN_FORMAT:
      /* Should never happen.  */

    পক্ষ DUMB_FORMAT:
      ভঙ্গ;

    পক্ষ ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      ভঙ্গ;

    পক্ষ TEX_FORMAT:

      /* Various characters need special processing.  */

      বাবে (ৱর্ণ ধ্রুৱ *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[to_uchar (*cursor)] = 1;

      ভঙ্গ;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

স্থির শূন্য
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t tail_max_width;	/* allowable width of tail field */
  ptrdiff_t head_max_width;	/* allowable width of head field */
  ৱর্ণ *cursor;			/* running cursor in source text */
  ৱর্ণ *বাম_context_start;	/* start of left context */
  ৱর্ণ *ডান_context_end;	/* end of right context */
  ৱর্ণ *বাম_field_start;	/* conservative start for 'head'/'before' */
  ৱর্ণ ধ্রুৱ *file_name;	/* file name for reference */
  intmax_t সাৰি_ordinal;	/* line ordinal for reference */
  ৱর্ণ ধ্রুৱ *buffer_start;	/* start of buffered file for this occurs */
  ৱর্ণ ধ্রুৱ *buffer_end;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.start = occurs->চাবি.start;
  keyafter.end = keyafter.start + occurs->চাবি.size;
  বাম_context_start = keyafter.start + occurs->বাম;
  ডান_context_end = keyafter.start + occurs->ডান;

  buffer_start = text_buffers[occurs->file_index].start;
  buffer_end = text_buffers[occurs->file_index].end;

  cursor = keyafter.end;
  যেতিয়ালৈ (cursor < ডান_context_end
         && cursor <= keyafter.start + keyafter_max_width)
    {
      keyafter.end = cursor;
      SKIP_SOMETHING (cursor, ডান_context_end);
    }
  যদি (cursor <= keyafter.start + keyafter_max_width)
    keyafter.end = cursor;

  keyafter_truncation = truncation_string && keyafter.end < ডান_context_end;

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

  যদি (-occurs->বাম > half_সাৰি_width + maximum_word_length)
    {
      বাম_field_start
        = keyafter.start - (half_সাৰি_width + maximum_word_length);
      SKIP_SOMETHING (বাম_field_start, keyafter.start);
    }
  অন্যথা
    বাম_field_start = keyafter.start + occurs->বাম;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.start = বাম_field_start;
  before.end = keyafter.start;
  SKIP_WHITE_BACKWARDS (before.end, before.start);

  যেতিয়ালৈ (before.start + before_max_width < before.end)
    SKIP_SOMETHING (before.start, before.end);

  যদি (truncation_string)
    {
      cursor = before.start;
      SKIP_WHITE_BACKWARDS (cursor, buffer_start);
      before_truncation = cursor > বাম_context_start;
    }
  অন্যথা
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

  যদি (tail_max_width > 0)
    {
      tail.start = keyafter.end;
      SKIP_WHITE (tail.start, buffer_end);

      tail.end = tail.start;
      cursor = tail.end;
      যেতিয়ালৈ (cursor < ডান_context_end
             && cursor < tail.start + tail_max_width)
        {
          tail.end = cursor;
          SKIP_SOMETHING (cursor, ডান_context_end);
        }

      যদি (cursor < tail.start + tail_max_width)
        tail.end = cursor;

      যদি (tail.end > tail.start)
        {
          keyafter_truncation = false;
          tail_truncation = truncation_string && tail.end < ডান_context_end;
        }
      অন্যথা
        tail_truncation = false;

      SKIP_WHITE_BACKWARDS (tail.end, tail.start);
    }
  অন্যথা
    {

      /* No place left for a tail field.  */

      tail.start = রিক্ত;
      tail.end = রিক্ত;
      tail_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  head_max_width
    = keyafter_max_width - (keyafter.end - keyafter.start) - gap_size;

  যদি (head_max_width > 0)
    {
      head.end = before.start;
      SKIP_WHITE_BACKWARDS (head.end, buffer_start);

      head.start = বাম_field_start;
      যেতিয়ালৈ (head.start + head_max_width < head.end)
        SKIP_SOMETHING (head.start, head.end);

      যদি (head.end > head.start)
        {
          before_truncation = false;
          head_truncation = (truncation_string
                             && head.start > বাম_context_start);
        }
      অন্যথা
        head_truncation = false;

      SKIP_WHITE (head.start, head.end);
    }
  অন্যথা
    {

      /* No place left for a head field.  */

      head.start = রিক্ত;
      head.end = রিক্ত;
      head_truncation = false;
    }

  যদি (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = প্ৰবেশ_file_name[occurs->file_index];
      যদি (!file_name)
        file_name = "";

      সাৰি_ordinal = occurs->reference + 1;
      যদি (occurs->file_index > 0)
        সাৰি_ordinal -= file_সাৰি_গুনতি[occurs->file_index - 1];

      ৱর্ণ *file_end = stpcpy (reference.start, file_name);
      reference.end = file_end + sprintf (file_end, ":%jd", সাৰি_ordinal);
    }
  অন্যথা যদি (প্ৰবেশ_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.start = keyafter.start + occurs->reference;
      reference.end = reference.start;
      SKIP_NON_WHITE (reference.end, ডান_context_end);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

স্থির শূন্য
output_one_roff_সাৰি (শূন্য)
{
  /* Output the 'tail' field.  */

  printf (".%s \"", macro_name);
  print_field (tail);
  যদি (tail_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  যদি (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);
  putchar ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_field (keyafter);
  যদি (keyafter_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  যদি (head_truncation)
    fputs (truncation_string, stdout);
  print_field (head);
  putchar ('"');

  /* Conditionally output the 'reference' field.  */

  যদি (auto_reference || প্ৰবেশ_reference)
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

স্থির শূন্য
output_one_tex_সাৰি (শূন্য)
{
  BLOCK চাবি;			/* key field, isolated */
  BLOCK after;			/* after field, isolated */
  ৱর্ণ *cursor;			/* running cursor in source text */

  printf ("\\%s ", macro_name);
  putchar ('{');
  print_field (tail);
  fputs ("}{", stdout);
  print_field (before);
  fputs ("}{", stdout);
  চাবি.start = keyafter.start;
  after.end = keyafter.end;
  cursor = keyafter.start;
  SKIP_SOMETHING (cursor, keyafter.end);
  চাবি.end = cursor;
  after.start = cursor;
  print_field (চাবি);
  fputs ("}{", stdout);
  print_field (after);
  fputs ("}{", stdout);
  print_field (head);
  putchar ('}');
  যদি (auto_reference || প্ৰবেশ_reference)
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

স্থির শূন্য
output_one_dumb_সাৰি (শূন্য)
{
  যদি (!ডান_reference)
    {
      যদি (auto_reference)
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
      অন্যথা
        {

          /* Output the 'reference' field and its following gap.  */

          print_field (reference);
          print_spaces (reference_max_width
                        + gap_size
                        - (reference.end - reference.start));
        }
    }

  যদি (tail.start < tail.end)
    {
      /* Output the 'tail' field.  */

      print_field (tail);
      যদি (tail_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_সাৰি_width - gap_size
                    - (before.end - before.start)
                    - (before_truncation ? truncation_string_length : 0)
                    - (tail.end - tail.start)
                    - (tail_truncation ? truncation_string_length : 0));
    }
  অন্যথা
    print_spaces (half_সাৰি_width - gap_size
                  - (before.end - before.start)
                  - (before_truncation ? truncation_string_length : 0));

  /* Output the 'before' field.  */

  যদি (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);

  print_spaces (gap_size);

  /* Output the 'keyafter' field.  */

  print_field (keyafter);
  যদি (keyafter_truncation)
    fputs (truncation_string, stdout);

  যদি (head.start < head.end)
    {
      /* Output the 'head' field.  */

      print_spaces (half_সাৰি_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0)
                    - (head.end - head.start)
                    - (head_truncation ? truncation_string_length : 0));
      যদি (head_truncation)
        fputs (truncation_string, stdout);
      print_field (head);
    }
  অন্যথা

    যদি ((auto_reference || প্ৰবেশ_reference) && ডান_reference)
      print_spaces (half_সাৰি_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0));

  যদি ((auto_reference || প্ৰবেশ_reference) && ডান_reference)
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

স্থির শূন্য
generate_all_output (শূন্য)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  tail.start = রিক্ত;
  tail.end = রিক্ত;
  tail_truncation = false;

  head.start = রিক্ত;
  head.end = রিক্ত;
  head_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_table[0];

  বাবে (idx_t occurs_index = 0; occurs_index < সংখ্যা_of_occurs[0];
       occurs_index++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      ৱিকল্প (output_format)
        {
        পক্ষ UNKNOWN_FORMAT:
          /* Should never happen.  */

        পক্ষ DUMB_FORMAT:
          output_one_dumb_সাৰি ();
          ভঙ্গ;

        পক্ষ ROFF_FORMAT:
          output_one_roff_সাৰি ();
          ভঙ্গ;

        পক্ষ TEX_FORMAT:
          output_one_tex_সাৰি ();
          ভঙ্গ;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [প্ৰবেশ_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [প্ৰবেশ_2 [OUTPUT]]\n"),
              program_name, program_name);
      fputs (_("\
Output a permuted index, including context, of the শব্দগুলি in the প্ৰবেশ files.\n\
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
  -F, --flag-truncation=STRING   use STRING বাবে flagging সাৰি truncations.\n\
                                 The স্ৱভাৱ is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name to use instead of 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --ডান-side-refs          put references at ডান, not counted in -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   বাবে end of সাৰিগুলি or end of sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --word-regexp=REGEXP       use REGEXP to match each keyword\n\
"));
      oputs (_("\
  -b, --ভঙ্গ-file=FILE          word ভঙ্গ characters in this FILE\n\
"));
      oputs (_("\
  -f, --ignore-পক্ষ              fold lower পক্ষ to upper পক্ষ বাবে sorting\n\
"));
      oputs (_("\
  -g, --gap-size=সংখ্যা_2          gap size in columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         পঠ ignore word list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           পঠ only word list from this FILE\n\
"));
      oputs (_("\
  -r, --references               first field of each সাৰি is a reference\n\
"));
      oputs (_("\
  -t, --typeset-mode             change the স্ৱভাৱ width from 72 to 100\n\
"));
      oputs (_("\
  -w, --width=সংখ্যা_2             output width in columns, reference excluded\n\
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
গণনা
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
স্থির সংরচনা option ধ্রুৱ long_options[] =
{
  {"auto-reference", no_argument, রিক্ত, 'A'},
  {"break-file", required_argument, রিক্ত, 'b'},
  {"flag-truncation", required_argument, রিক্ত, 'F'},
  {"ignore-case", no_argument, রিক্ত, 'f'},
  {"gap-size", required_argument, রিক্ত, 'g'},
  {"ignore-file", required_argument, রিক্ত, 'i'},
  {"macro-name", required_argument, রিক্ত, 'M'},
  {"only-file", required_argument, রিক্ত, 'o'},
  {"references", no_argument, রিক্ত, 'r'},
  {"right-side-refs", no_argument, রিক্ত, 'R'},
  {"format", required_argument, রিক্ত, FORMAT_OPTION},
  {"sentence-regexp", required_argument, রিক্ত, 'S'},
  {"traditional", no_argument, রিক্ত, 'G'},
  {"typeset-mode", no_argument, রিক্ত, 't'},
  {"width", required_argument, রিক্ত, 'w'},
  {"word-regexp", required_argument, রিক্ত, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0},
};

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ format_args[] =
{
  "roff", "tex", রিক্ত
};

স্থির গণনা Format ধ্রুৱ format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  পূর্ণাঙ্ক optchar;			/* argument character */
  পূর্ণাঙ্ক file_index;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, রিক্ত),
         optchar != EOF)
    {
      ৱিকল্প (optchar)
        {
        স্ৱভাৱ:
          usage (EXIT_FAILURE);

        পক্ষ 'G':
          gnu_extensions = false;
          ভঙ্গ;

        পক্ষ 'b':
          break_file = optarg;
          ভঙ্গ;

        পক্ষ 'f':
          ignore_case = true;
          ভঙ্গ;

        পক্ষ 'g':
          {
            intmax_t tmp;
            যদি (! (xstrtoimax (optarg, রিক্ত, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_size = tmp;
            ভঙ্গ;
          }

        পক্ষ 'i':
          ignore_file = optarg;
          ভঙ্গ;

        পক্ষ 'o':
          only_file = optarg;
          ভঙ্গ;

        পক্ষ 'r':
          প্ৰবেশ_reference = true;
          ভঙ্গ;

        পক্ষ 't':
          যদি (সাৰি_width < 0)
            সাৰি_width = 100;
          ভঙ্গ;

        পক্ষ 'w':
          {
            intmax_t tmp;
            যদি (! (xstrtoimax (optarg, রিক্ত, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            সাৰি_width = tmp;
            ভঙ্গ;
          }

        পক্ষ 'A':
          auto_reference = true;
          ভঙ্গ;

        পক্ষ 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          ভঙ্গ;

        পক্ষ 'M':
          macro_name = optarg;
          ভঙ্গ;

        পক্ষ 'O':
          output_format = ROFF_FORMAT;
          ভঙ্গ;

        পক্ষ 'R':
          ডান_reference = true;
          ভঙ্গ;

        পক্ষ 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          ভঙ্গ;

        পক্ষ 'T':
          output_format = TEX_FORMAT;
          ভঙ্গ;

        পক্ষ 'W':
          word_regex.string = optarg;
          unescape_string (optarg);
          যদি (!*word_regex.string)
            word_regex.string = রিক্ত;
          ভঙ্গ;

        পক্ষ FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_args, format_vals);
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        }
    }

  যদি (সাৰি_width < 0)
    সাৰি_width = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  যদি (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      প্ৰবেশ_file_name = xmalloc (পরিমাণ *প্ৰবেশ_file_name);
      file_সাৰি_গুনতি = xmalloc (পরিমাণ *file_সাৰি_গুনতি);
      text_buffers =    xmalloc (পরিমাণ *text_buffers);
      সংখ্যা_প্ৰবেশ_files = 1;
      প্ৰবেশ_file_name[0] = রিক্ত;
    }
  অন্যথা যদি (gnu_extensions)
    {
      সংখ্যা_প্ৰবেশ_files = argc - optind;
      প্ৰবেশ_file_name = xnmalloc (সংখ্যা_প্ৰবেশ_files, পরিমাণ *প্ৰবেশ_file_name);
      file_সাৰি_গুনতি = xnmalloc (সংখ্যা_প্ৰবেশ_files, পরিমাণ *file_সাৰি_গুনতি);
      text_buffers    = xnmalloc (সংখ্যা_প্ৰবেশ_files, পরিমাণ *text_buffers);

      বাবে (file_index = 0; file_index < সংখ্যা_প্ৰবেশ_files; file_index++)
        {
          যদি (!*argv[optind] || streq (argv[optind], "-"))
            প্ৰবেশ_file_name[file_index] = রিক্ত;
          অন্যথা
            প্ৰবেশ_file_name[file_index] = argv[optind];
          optind++;
        }
    }
  অন্যথা
    {

      /* There is one necessary input file.  */

      সংখ্যা_প্ৰবেশ_files = 1;
      প্ৰবেশ_file_name = xmalloc (পরিমাণ *প্ৰবেশ_file_name);
      file_সাৰি_গুনতি = xmalloc (পরিমাণ *file_সাৰি_গুনতি);
      text_buffers    = xmalloc (পরিমাণ *text_buffers);
      যদি (!*argv[optind] || streq (argv[optind], "-"))
        প্ৰবেশ_file_name[0] = রিক্ত;
      অন্যথা
        প্ৰবেশ_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      যদি (optind < argc)
        {
          যদি (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      যদি (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  যদি (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  যদি (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  যদি (ignore_file)
    {
      digest_word_file (ignore_file, &ignore_table);
      যদি (ignore_table.length == 0)
        ignore_file = রিক্ত;
    }

  যদি (only_file)
    {
      digest_word_file (only_file, &only_table);
      যদি (only_table.length == 0)
        only_file = রিক্ত;
    }

  /* Prepare to study all the input files.  */

  সংখ্যা_of_occurs[0] = 0;
  total_সাৰি_গুনতি = 0;
  maximum_word_length = 0;
  reference_max_width = 0;

  বাবে (file_index = 0; file_index < সংখ্যা_প্ৰবেশ_files; file_index++)
    {
      BLOCK *text_buffer = text_buffers + file_index;

      /* Read the file contents into memory, then study it.  */

      swallow_file_in_memory (প্ৰবেশ_file_name[file_index], text_buffer);
      find_occurs_in_text (file_index);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      total_সাৰি_গুনতি++;
      file_সাৰি_গুনতি[file_index] = total_সাৰি_গুনতি;
    }

  /* Do the output process phase.  */

  sort_found_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  ঘূৰাই EXIT_SUCCESS;
}
