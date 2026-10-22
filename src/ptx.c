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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "system.h"
#অন্তর্ভাব <regex.h>
#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "octhexdigits.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "read-file.h"
#অন্তর্ভাব "stdio--.h"
#অন্তর্ভাব "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#define AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#define CHAR_SET_আকার 256

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
স্থির bool প্রবেশ_reference = false;	/* refs at beginning of input lines */
স্থির bool ডান_reference = false;	/* output refs after right context  */
স্থির idx_t সারি_width = -1;		/* output line width in characters */
স্থির idx_t gap_আকার = 3;	/* number of spaces between output fields */
স্থির অক্ষর ধ্রুবক *truncation_string = "/";
                                /* string used to mark line truncations */
স্থির অক্ষর ধ্রুবক *macro_name = "xx";	/* macro name for roff or TeX output */
স্থির গণনা Format output_format = UNKNOWN_FORMAT;
                                /* output format */

স্থির bool ignore_case = false;	/* fold lower to upper for sorting */
স্থির অক্ষর ধ্রুবক *break_file = ফাঁকা; /* name of the 'Break chars' file */
স্থির অক্ষর ধ্রুবক *only_file = ফাঁকা;	/* name of the 'Only words' file */
স্থির অক্ষর ধ্রুবক *ignore_file = ফাঁকা; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
কাঠামো regex_তথ্য
{
  /* The original regular expression, as a string.  */
  অক্ষর ধ্রুবক *string;

  /* The compiled regular expression, and its fastmap.  */
  কাঠামো re_pattern_বাফার pattern;
  অক্ষর fastmap[UCHAR_MAX + 1];
};

স্থির কাঠামো regex_তথ্য context_regex;	/* end of context */
স্থির কাঠামো regex_তথ্য word_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

প্রকারলেখ কাঠামো
  {
    অক্ষর *start;		/* pointer to beginning of region */
    অক্ষর *end;			/* pointer to end + 1 of region */
  }
BLOCK;

প্রকারলেখ কাঠামো
  {
    অক্ষর *start;		/* pointer to beginning of region */
    idx_t আকার_2;			/* length of the region */
  }
WORD;

প্রকারলেখ কাঠামো
  {
    idx_t alloc;		/* allocated length */
    idx_t দৈর্ঘ্য;		/* number of used entries */
    WORD *start COUNTED_BY (দৈর্ঘ্য);	/* array of WORDs */
  }
WORD_TABLE;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
স্থির unsigned অক্ষর folded_chars[CHAR_SET_আকার];

/* End of context pattern register indices.  */
স্থির কাঠামো re_registers context_regs;

/* Keyword pattern register indices.  */
স্থির কাঠামো re_registers word_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
স্থির অক্ষর word_fastmap[CHAR_SET_আকার];

/* Maximum length of any word read.  */
স্থির idx_t maximum_word_দৈর্ঘ্য;

/* Maximum width of any reference used.  */
স্থির idx_t reference_max_width;

/* Ignore and Only word tables.  */

স্থির WORD_TABLE ignore_table;	/* table of words to ignore */
স্থির WORD_TABLE only_table;		/* table of words to select */

/* Source text table, and scanning macros.  */

স্থির পূর্ণসংখ্যা সংখ্যা_প্রবেশ_files;	/* number of text input files */
স্থির intmax_t যোগ_সারি_গুনতি;	/* total number of lines seen so far */
স্থির অক্ষর ধ্রুবক **প্রবেশ_file_name;	/* array of text input file names */
স্থির intmax_t *file_সারি_গুনতি;	/* array of line count values at end */

স্থির BLOCK *text_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#define SKIP_NON_WHITE(cursor, limit) \
  যখন (cursor < limit && ! isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE(cursor, limit) \
  যখন (cursor < limit && isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE_BACKWARDS(cursor, start) \
  যখন (cursor > start && isspace (to_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#define SKIP_SOMETHING(cursor, limit) \
  যদি (word_regex.string)						\
    {									\
      regoff_t গুনতি;							\
      গুনতি = re_match (&word_regex.pattern, cursor, limit - cursor,	\
                        0, ফাঁকা);					\
      যদি (গুনতি == -2)							\
        matcher_error ();						\
      cursor += গুনতি <= 0 ? 1 : গুনতি;					\
    }									\
  অন্যথা যদি (word_fastmap[to_uchar (*cursor)])				\
    যখন (cursor < limit && word_fastmap[to_uchar (*cursor)])		\
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

প্রকারলেখ কাঠামো
  {
    WORD চাবি;			/* description of the keyword */
    ptrdiff_t বাম;		/* distance to left context start */
    ptrdiff_t ডান;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    পূর্ণসংখ্যা file_সূচক;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

স্থির OCCURS *occurs_table[1];	/* all words retained from the read text */
স্থির idx_t occurs_alloc[1];	/* allocated size of occurs_table */
স্থির idx_t সংখ্যা_of_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
স্থির অক্ষর edited_flag[CHAR_SET_আকার];

/* Half of line width, reference excluded.  */
স্থির idx_t half_সারি_width;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
স্থির ptrdiff_t before_max_width;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
স্থির ptrdiff_t keyafter_max_width;

/* Length of string that flags truncation.  */
স্থির idx_t truncation_string_দৈর্ঘ্য;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

স্থির BLOCK লেজ;		/* tail field */
স্থির bool লেজ_truncation;	/* flag truncation after the tail field */

স্থির BLOCK before;		/* before field */
স্থির bool before_truncation;	/* flag truncation before the before field */

স্থির BLOCK keyafter;		/* keyword-and-after field */
স্থির bool keyafter_truncation; /* flag truncation after the keyafter field */

স্থির BLOCK শীর্ষ;		/* head field */
স্থির bool শীর্ষ_truncation;	/* flag truncation before the head field */

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
unescape_string (অক্ষর *string)
{
  অক্ষর *cursor;			/* cursor in result */
  পূর্ণসংখ্যা মান;			/* value of \nnn escape */
  পূর্ণসংখ্যা দৈর্ঘ্য;			/* length of \nnn escape */

  cursor = string;

  যখন (*string)
    {
      যদি (*string == '\\')
        {
          string++;
          নির্বাচন (*string)
            {
            ক্ষেত্র 'x':		/* \xhhh escape, 3 chars maximum */
              মান = 0;
              জন্য (দৈর্ঘ্য = 0, string++;
                   দৈর্ঘ্য < 3 && c_isxdigit (*string);
                   দৈর্ঘ্য++, string++)
                মান = মান * 16 + fromhex (*string);
              যদি (দৈর্ঘ্য == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              অন্যথা
                *cursor++ = মান;
              বিরতি;

            ক্ষেত্র '0':		/* \0ooo escape, 3 chars maximum */
              মান = 0;
              জন্য (দৈর্ঘ্য = 0, string++;
                   দৈর্ঘ্য < 3 && isoct (*string);
                   দৈর্ঘ্য++, string++)
                মান = মান * 8 + fromoct (*string);
              *cursor++ = মান;
              বিরতি;

            ক্ষেত্র 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              বিরতি;

            ক্ষেত্র 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              বিরতি;

            ক্ষেত্র 'c':		/* cancel the rest of the output */
              যখন (*string)
                string++;
              বিরতি;

            ক্ষেত্র 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              বিরতি;

            ক্ষেত্র 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              বিরতি;

            ক্ষেত্র 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              বিরতি;

            ক্ষেত্র 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              বিরতি;

            ক্ষেত্র 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              বিরতি;

            ক্ষেত্র '\0':		/* lone backslash at end of string */
              /* ignore it */
              বিরতি;

            পূর্বনির্ধারিত:
              *cursor++ = '\\';
              *cursor++ = *string++;
              বিরতি;
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
compile_regex (কাঠামো regex_তথ্য *regex)
{
  কাঠামো re_pattern_বাফার *pattern = &regex->pattern;
  অক্ষর ধ্রুবক *string = regex->string;
  অক্ষর ধ্রুবক *message;

  pattern->বাফার = ফাঁকা;
  pattern->allocated = 0;
  pattern->fastmap = regex->fastmap;
  pattern->translate = ignore_case ? folded_chars : ফাঁকা;

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
    জন্য (পূর্ণসংখ্যা character = 0; character < CHAR_SET_আকার; character++)
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
        context_regex.string = ফাঁকা;
    }
  অন্যথা যদি (gnu_extensions && !প্রবেশ_reference)
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

          জন্য (পূর্ণসংখ্যা character = 0; character < CHAR_SET_আকার; character++)
            word_fastmap[character] = !! isalpha (character);
        }
      অন্যথা
        {

          /* Simulate [^ \t\n]+.  */

          memset (word_fastmap, 1, CHAR_SET_আকার);
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
swallow_file_in_memory (অক্ষর ধ্রুবক *file_name, BLOCK *block)
{
  size_t used_দৈর্ঘ্য;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  যদি (using_stdin)
    block->start = fread_file (stdin, 0, &used_দৈর্ঘ্য);
  অন্যথা
    block->start = read_file (file_name, 0, &used_দৈর্ঘ্য);

  যদি (!block->start)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  যদি (using_stdin)
    clearerr (stdin);

  block->end = block->start + used_দৈর্ঘ্য;
}

/* Sort and search routines.  */

/*--------------------------------------------------------------------------.
| Compare two words, FIRST and SECOND, and return 0 if they are identical.  |
| Return less than 0 if the first word goes before the second; return	    |
| greater than 0 if the first word goes after the second.		    |
|									    |
| If a word is indeed a prefix of the other, the shorter should go first.   |
`--------------------------------------------------------------------------*/

স্থির পূর্ণসংখ্যা
compare_শব্দগুলি (ধ্রুবক শূন্য *void_first, ধ্রুবক শূন্য *void_second)
{
  WORD ধ্রুবক *first = void_first;
  WORD ধ্রুবক *second = void_second;
  idx_t দৈর্ঘ্য = MIN (first->আকার_2, second->আকার_2);

  যদি (ignore_case)
    {
      জন্য (idx_t গণক = 0; গণক < দৈর্ঘ্য; গণক++)
        {
          পূর্ণসংখ্যা মান = (folded_chars[to_uchar (first->start[গণক])]
                       - folded_chars[to_uchar (second->start[গণক])]);
          যদি (মান != 0)
            ফেরত মান;
        }
    }
  অন্যথা
    {
      জন্য (idx_t গণক = 0; গণক < দৈর্ঘ্য; গণক++)
        {
          পূর্ণসংখ্যা মান = (to_uchar (first->start[গণক])
                       - to_uchar (second->start[গণক]));
          যদি (মান != 0)
            ফেরত মান;
        }
    }

  ফেরত _GL_CMP (first->আকার_2, second->আকার_2);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

স্থির পূর্ণসংখ্যা
compare_occurs (ধ্রুবক শূন্য *void_first, ধ্রুবক শূন্য *void_second)
{
#define first ((ধ্রুবক OCCURS *) void_first)
#define second ((ধ্রুবক OCCURS *) void_second)
  পূর্ণসংখ্যা মান;

  মান = compare_শব্দগুলি (&first->চাবি, &second->চাবি);
  ফেরত (মান ? মান
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
  idx_t hi = table->দৈর্ঘ্য;
  যখন (lo < hi)
    {
      idx_t মধ্য = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      পূর্ণসংখ্যা মান = compare_শব্দগুলি (word, table->start + মধ্য);
      যদি (মান < 0)
        hi = মধ্য;
      অন্যথা যদি (মান > 0)
        lo = মধ্য + 1;
      অন্যথা
        ফেরত true;
    }
  ফেরত false;
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
    qsort (occurs_table[0], সংখ্যা_of_occurs[0], আকার **occurs_table,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

স্থির শূন্য
digest_break_file (অক্ষর ধ্রুবক *file_name)
{
  BLOCK file_contents;		/* to receive a copy of the file */

  swallow_file_in_memory (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (word_fastmap, 1, CHAR_SET_আকার);
  জন্য (অক্ষর *cursor = file_contents.start; cursor < file_contents.end;
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
digest_word_file (অক্ষর ধ্রুবক *file_name, WORD_TABLE *table)
{
  BLOCK file_contents;		/* to receive a copy of the file */
  অক্ষর *cursor;			/* cursor in file copy */
  অক্ষর *word_start;		/* start of the current word */

  swallow_file_in_memory (file_name, &file_contents);

  table->start = ফাঁকা;
  table->alloc = 0;
  table->দৈর্ঘ্য = 0;

  /* Read the whole file.  */

  cursor = file_contents.start;
  যখন (cursor < file_contents.end)
    {

      /* Read one line, and save the word in contains.  */

      word_start = cursor;
      যখন (cursor < file_contents.end && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      যদি (cursor > word_start)
        {
          যদি (table->দৈর্ঘ্য == table->alloc)
            table->start = xpalloc (table->start, &table->alloc, 1, -1,
                                    আকার *table->start);
          idx_t ধ্রুবক table_idx = table->দৈর্ঘ্য++;
          table->start[table_idx].start = word_start;
          table->start[table_idx].আকার_2 = cursor - word_start;
        }

      /* This test allows for an incomplete line at end of file.  */

      যদি (cursor < file_contents.end)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (table->start, table->দৈর্ঘ্য, আকার table->start[0], compare_শব্দগুলি);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

স্থির শূন্য
find_occurs_in_text (পূর্ণসংখ্যা file_সূচক)
{
  অক্ষর *scan;			/* for scanning the source text */
  অক্ষর *সারি_start;		/* start of the current input line */
  অক্ষর *সারি_scan;		/* newlines scanned until this point */
  idx_t reference_দৈর্ঘ্য;	/* length of reference in input mode */
  WORD possible_চাবি;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  অক্ষর *context_start;		/* start of left context */
  অক্ষর *context_end;		/* end of right context */
  অক্ষর *word_start;		/* start of word */
  অক্ষর *word_end;		/* end of word */
  অক্ষর *পরবর্তী_context_start;	/* next start of left context */

  ধ্রুবক BLOCK *text_বাফার = &text_buffers[file_সূচক];

  /* reference_length is always used within 'if (input_reference)'.
     However, GNU C diagnoses that it may be used uninitialized.  The
     following assignment is merely to shut it up.  */

  reference_দৈর্ঘ্য = 0;

  /* Tracking where lines start is helpful for reference processing.  In
     auto reference mode, this allows counting lines.  In input reference
     mode, this permits finding the beginning of the references.

     The first line begins with the file, skip immediately this very first
     reference in input reference mode, to help further rejection any word
     found inside it.  Also, unconditionally assigning these variable has
     the happy effect of shutting up lint.  */

  সারি_start = text_বাফার->start;
  সারি_scan = সারি_start;
  যদি (প্রবেশ_reference)
    {
      SKIP_NON_WHITE (সারি_scan, text_বাফার->end);
      reference_দৈর্ঘ্য = সারি_scan - সারি_start;
      SKIP_WHITE (সারি_scan, text_বাফার->end);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  জন্য (অক্ষর *cursor = text_বাফার->start;
       cursor < text_বাফার->end;
       cursor = পরবর্তী_context_start)
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

      পরবর্তী_context_start = text_বাফার->end;
      যদি (context_regex.string)
        নির্বাচন (re_অনুসন্ধান (&context_regex.pattern, cursor,
                           text_বাফার->end - cursor,
                           0, text_বাফার->end - cursor, &context_regs))
          {
          ক্ষেত্র -2:
            matcher_error ();

          ক্ষেত্র -1:
            বিরতি;

          ক্ষেত্র 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          পূর্বনির্ধারিত:
            পরবর্তী_context_start = cursor + context_regs.end[0];
            বিরতি;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_end = পরবর্তী_context_start;
      SKIP_WHITE_BACKWARDS (context_end, context_start);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      যখন (true)
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
                বিরতি;
              word_start = cursor + word_regs.start[0];
              word_end = cursor + word_regs.end[0];
            }
          অন্যথা

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              scan = cursor;
              যখন (scan < context_end
                     && !word_fastmap[to_uchar (*scan)])
                scan++;

              যদি (scan == context_end)
                বিরতি;

              word_start = scan;

              যখন (scan < context_end
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
              চালু;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_চাবি.start = cursor;
          possible_চাবি.আকার_2 = word_end - word_start;
          cursor += possible_চাবি.আকার_2;

          যদি (possible_চাবি.আকার_2 > maximum_word_দৈর্ঘ্য)
            maximum_word_দৈর্ঘ্য = possible_চাবি.আকার_2;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          যদি (প্রবেশ_reference)
            {
              যখন (সারি_scan < possible_চাবি.start)
                যদি (*সারি_scan == '\n')
                  {
                    যোগ_সারি_গুনতি++;
                    সারি_scan++;
                    সারি_start = সারি_scan;
                    SKIP_NON_WHITE (সারি_scan, text_বাফার->end);
                    reference_দৈর্ঘ্য = সারি_scan - সারি_start;
                  }
                অন্যথা
                  সারি_scan++;
              যদি (সারি_scan > possible_চাবি.start)
                চালু;
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
            চালু;
          যদি (only_file && !অনুসন্ধান_table (&possible_চাবি, &only_table))
            চালু;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          যদি (সংখ্যা_of_occurs[0] == occurs_alloc[0])
            occurs_table[0] = xpalloc (occurs_table[0], &occurs_alloc[0],
                                       1, -1, আকার *occurs_table[0]);
          occurs_cursor = occurs_table[0] + সংখ্যা_of_occurs[0];

          /* Define the reference field, if any.  */

          যদি (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              যখন (সারি_scan < possible_চাবি.start)
                যদি (*সারি_scan == '\n')
                  {
                    যোগ_সারি_গুনতি++;
                    সারি_scan++;
                    সারি_start = সারি_scan;
                    SKIP_NON_WHITE (সারি_scan, text_বাফার->end);
                  }
                অন্যথা
                  সারি_scan++;

              occurs_cursor->reference = যোগ_সারি_গুনতি;
            }
          অন্যথা যদি (প্রবেশ_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = সারি_start - possible_চাবি.start;
              যদি (reference_দৈর্ঘ্য > reference_max_width)
                reference_max_width = reference_দৈর্ঘ্য;
            }

          /* Exclude the reference from the context in simple cases.  */

          যদি (প্রবেশ_reference && সারি_start == context_start)
            {
              SKIP_NON_WHITE (context_start, context_end);
              SKIP_WHITE (context_start, context_end);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->চাবি = possible_চাবি;
          occurs_cursor->বাম = context_start - possible_চাবি.start;
          occurs_cursor->ডান = context_end - possible_চাবি.start;
          occurs_cursor->file_সূচক = file_সূচক;

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
  জন্য (ptrdiff_t গণক = সংখ্যা; গণক > 0; গণক--)
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

  জন্য (অক্ষর *cursor = field.start; cursor < field.end; cursor++)
    {
      unsigned অক্ষর character = *cursor;
      যদি (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          নির্বাচন (character)
            {
            ক্ষেত্র '"':
              /* In roff output format, double any quote.  */
              putchar ('"');
              putchar ('"');
              বিরতি;

            ক্ষেত্র '$':
            ক্ষেত্র '%':
            ক্ষেত্র '&':
            ক্ষেত্র '#':
            ক্ষেত্র '_':
              /* In TeX output format, precede these with a backslash.  */
              putchar ('\\');
              putchar (character);
              বিরতি;

            ক্ষেত্র '{':
            ক্ষেত্র '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              printf ("$\\%c$", character);
              বিরতি;

            ক্ষেত্র '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              বিরতি;

            পূর্বনির্ধারিত:
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
      জন্য (পূর্ণসংখ্যা file_সূচক = 0; file_সূচক < সংখ্যা_প্রবেশ_files; file_সূচক++)
        {
          intmax_t সারি_ordinal = file_সারি_গুনতি[file_সূচক] + 1;
          যদি (file_সূচক > 0)
            সারি_ordinal -= file_সারি_গুনতি[file_সূচক - 1];
          অক্ষর ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_width = sprintf (ordinal_string, "%jd", সারি_ordinal);
          যদি (প্রবেশ_file_name[file_সূচক])
            reference_width += strlen (প্রবেশ_file_name[file_সূচক]);
          যদি (reference_width > reference_max_width)
            reference_max_width = reference_width;
        }
      reference_max_width++;
      reference.start = xmalloc (reference_max_width + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  যদি ((auto_reference || প্রবেশ_reference) && !ডান_reference)
    সারি_width = MAX (0, সারি_width - (reference_max_width + gap_আকার));

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

  half_সারি_width = সারি_width >> 1;
  before_max_width = half_সারি_width - gap_আকার;
  keyafter_max_width = half_সারি_width;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  যদি (truncation_string && *truncation_string)
    truncation_string_দৈর্ঘ্য = strlen (truncation_string);
  অন্যথা
    truncation_string = ফাঁকা;

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

      before_max_width -= 2 * truncation_string_দৈর্ঘ্য;
      keyafter_max_width -= 2 * truncation_string_দৈর্ঘ্য;
    }
  অন্যথা
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_width -= 2 * truncation_string_দৈর্ঘ্য + 1;
    }

  যদি (before_max_width < 0)
    before_max_width = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  জন্য (পূর্ণসংখ্যা character = 0; character < CHAR_SET_আকার; character++)
    edited_flag[character] = !! isspace (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  নির্বাচন (output_format)
    {
    ক্ষেত্র UNKNOWN_FORMAT:
      /* Should never happen.  */

    ক্ষেত্র DUMB_FORMAT:
      বিরতি;

    ক্ষেত্র ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      বিরতি;

    ক্ষেত্র TEX_FORMAT:

      /* Various characters need special processing.  */

      জন্য (অক্ষর ধ্রুবক *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[to_uchar (*cursor)] = 1;

      বিরতি;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

স্থির শূন্য
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t লেজ_max_width;	/* allowable width of tail field */
  ptrdiff_t শীর্ষ_max_width;	/* allowable width of head field */
  অক্ষর *cursor;			/* running cursor in source text */
  অক্ষর *বাম_context_start;	/* start of left context */
  অক্ষর *ডান_context_end;	/* end of right context */
  অক্ষর *বাম_field_start;	/* conservative start for 'head'/'before' */
  অক্ষর ধ্রুবক *file_name;	/* file name for reference */
  intmax_t সারি_ordinal;	/* line ordinal for reference */
  অক্ষর ধ্রুবক *বাফার_start;	/* start of buffered file for this occurs */
  অক্ষর ধ্রুবক *বাফার_end;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.start = occurs->চাবি.start;
  keyafter.end = keyafter.start + occurs->চাবি.আকার_2;
  বাম_context_start = keyafter.start + occurs->বাম;
  ডান_context_end = keyafter.start + occurs->ডান;

  বাফার_start = text_buffers[occurs->file_সূচক].start;
  বাফার_end = text_buffers[occurs->file_সূচক].end;

  cursor = keyafter.end;
  যখন (cursor < ডান_context_end
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

  যদি (-occurs->বাম > half_সারি_width + maximum_word_দৈর্ঘ্য)
    {
      বাম_field_start
        = keyafter.start - (half_সারি_width + maximum_word_দৈর্ঘ্য);
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

  যখন (before.start + before_max_width < before.end)
    SKIP_SOMETHING (before.start, before.end);

  যদি (truncation_string)
    {
      cursor = before.start;
      SKIP_WHITE_BACKWARDS (cursor, বাফার_start);
      before_truncation = cursor > বাম_context_start;
    }
  অন্যথা
    before_truncation = false;

  SKIP_WHITE (before.start, বাফার_end);

  /* The tail could not take more columns than what has been left in the
     left context field, and a gap is mandatory.  It starts after the
     right context, and does not contain prefixed spaces.  It ends at
     the end of line, the end of buffer or when the tail field is full,
     whichever comes first.  It cannot contain only part of a word, and
     has no suffixed spaces.  */

  লেজ_max_width
    = before_max_width - (before.end - before.start) - gap_আকার;

  যদি (লেজ_max_width > 0)
    {
      লেজ.start = keyafter.end;
      SKIP_WHITE (লেজ.start, বাফার_end);

      লেজ.end = লেজ.start;
      cursor = লেজ.end;
      যখন (cursor < ডান_context_end
             && cursor < লেজ.start + লেজ_max_width)
        {
          লেজ.end = cursor;
          SKIP_SOMETHING (cursor, ডান_context_end);
        }

      যদি (cursor < লেজ.start + লেজ_max_width)
        লেজ.end = cursor;

      যদি (লেজ.end > লেজ.start)
        {
          keyafter_truncation = false;
          লেজ_truncation = truncation_string && লেজ.end < ডান_context_end;
        }
      অন্যথা
        লেজ_truncation = false;

      SKIP_WHITE_BACKWARDS (লেজ.end, লেজ.start);
    }
  অন্যথা
    {

      /* No place left for a tail field.  */

      লেজ.start = ফাঁকা;
      লেজ.end = ফাঁকা;
      লেজ_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  শীর্ষ_max_width
    = keyafter_max_width - (keyafter.end - keyafter.start) - gap_আকার;

  যদি (শীর্ষ_max_width > 0)
    {
      শীর্ষ.end = before.start;
      SKIP_WHITE_BACKWARDS (শীর্ষ.end, বাফার_start);

      শীর্ষ.start = বাম_field_start;
      যখন (শীর্ষ.start + শীর্ষ_max_width < শীর্ষ.end)
        SKIP_SOMETHING (শীর্ষ.start, শীর্ষ.end);

      যদি (শীর্ষ.end > শীর্ষ.start)
        {
          before_truncation = false;
          শীর্ষ_truncation = (truncation_string
                             && শীর্ষ.start > বাম_context_start);
        }
      অন্যথা
        শীর্ষ_truncation = false;

      SKIP_WHITE (শীর্ষ.start, শীর্ষ.end);
    }
  অন্যথা
    {

      /* No place left for a head field.  */

      শীর্ষ.start = ফাঁকা;
      শীর্ষ.end = ফাঁকা;
      শীর্ষ_truncation = false;
    }

  যদি (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = প্রবেশ_file_name[occurs->file_সূচক];
      যদি (!file_name)
        file_name = "";

      সারি_ordinal = occurs->reference + 1;
      যদি (occurs->file_সূচক > 0)
        সারি_ordinal -= file_সারি_গুনতি[occurs->file_সূচক - 1];

      অক্ষর *file_end = stpcpy (reference.start, file_name);
      reference.end = file_end + sprintf (file_end, ":%jd", সারি_ordinal);
    }
  অন্যথা যদি (প্রবেশ_reference)
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
output_one_roff_সারি (শূন্য)
{
  /* Output the 'tail' field.  */

  printf (".%s \"", macro_name);
  print_field (লেজ);
  যদি (লেজ_truncation)
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
  যদি (শীর্ষ_truncation)
    fputs (truncation_string, stdout);
  print_field (শীর্ষ);
  putchar ('"');

  /* Conditionally output the 'reference' field.  */

  যদি (auto_reference || প্রবেশ_reference)
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
output_one_tex_সারি (শূন্য)
{
  BLOCK চাবি;			/* key field, isolated */
  BLOCK after;			/* after field, isolated */
  অক্ষর *cursor;			/* running cursor in source text */

  printf ("\\%s ", macro_name);
  putchar ('{');
  print_field (লেজ);
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
  print_field (শীর্ষ);
  putchar ('}');
  যদি (auto_reference || প্রবেশ_reference)
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
output_one_dumb_সারি (শূন্য)
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
                        + gap_আকার
                        - (reference.end - reference.start)
                        - 1);
        }
      অন্যথা
        {

          /* Output the 'reference' field and its following gap.  */

          print_field (reference);
          print_spaces (reference_max_width
                        + gap_আকার
                        - (reference.end - reference.start));
        }
    }

  যদি (লেজ.start < লেজ.end)
    {
      /* Output the 'tail' field.  */

      print_field (লেজ);
      যদি (লেজ_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_সারি_width - gap_আকার
                    - (before.end - before.start)
                    - (before_truncation ? truncation_string_দৈর্ঘ্য : 0)
                    - (লেজ.end - লেজ.start)
                    - (লেজ_truncation ? truncation_string_দৈর্ঘ্য : 0));
    }
  অন্যথা
    print_spaces (half_সারি_width - gap_আকার
                  - (before.end - before.start)
                  - (before_truncation ? truncation_string_দৈর্ঘ্য : 0));

  /* Output the 'before' field.  */

  যদি (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);

  print_spaces (gap_আকার);

  /* Output the 'keyafter' field.  */

  print_field (keyafter);
  যদি (keyafter_truncation)
    fputs (truncation_string, stdout);

  যদি (শীর্ষ.start < শীর্ষ.end)
    {
      /* Output the 'head' field.  */

      print_spaces (half_সারি_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_দৈর্ঘ্য : 0)
                    - (শীর্ষ.end - শীর্ষ.start)
                    - (শীর্ষ_truncation ? truncation_string_দৈর্ঘ্য : 0));
      যদি (শীর্ষ_truncation)
        fputs (truncation_string, stdout);
      print_field (শীর্ষ);
    }
  অন্যথা

    যদি ((auto_reference || প্রবেশ_reference) && ডান_reference)
      print_spaces (half_সারি_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_দৈর্ঘ্য : 0));

  যদি ((auto_reference || প্রবেশ_reference) && ডান_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_আকার);
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

  লেজ.start = ফাঁকা;
  লেজ.end = ফাঁকা;
  লেজ_truncation = false;

  শীর্ষ.start = ফাঁকা;
  শীর্ষ.end = ফাঁকা;
  শীর্ষ_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_table[0];

  জন্য (idx_t occurs_সূচক = 0; occurs_সূচক < সংখ্যা_of_occurs[0];
       occurs_সূচক++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      নির্বাচন (output_format)
        {
        ক্ষেত্র UNKNOWN_FORMAT:
          /* Should never happen.  */

        ক্ষেত্র DUMB_FORMAT:
          output_one_dumb_সারি ();
          বিরতি;

        ক্ষেত্র ROFF_FORMAT:
          output_one_roff_সারি ();
          বিরতি;

        ক্ষেত্র TEX_FORMAT:
          output_one_tex_সারি ();
          বিরতি;
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
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [প্রবেশ_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [প্রবেশ_2 [OUTPUT]]\n"),
              program_name, program_name);
      fputs (_("\
Output a permuted index, including context, of the শব্দগুলি in the প্রবেশ files.\n\
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
  -F, --flag-truncation=STRING   use STRING জন্য flagging সারি truncations.\n\
                                 The পূর্বনির্ধারিত is '/'\n\
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
  -S, --sentence-regexp=REGEXP   জন্য end of সারিগুলি or end of sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --word-regexp=REGEXP       use REGEXP to match each keyword\n\
"));
      oputs (_("\
  -b, --বিরতি-file=FILE          word বিরতি characters in this FILE\n\
"));
      oputs (_("\
  -f, --ignore-ক্ষেত্র              fold lower ক্ষেত্র to upper ক্ষেত্র জন্য sorting\n\
"));
      oputs (_("\
  -g, --gap-আকার_2=সংখ্যা_2          gap আকার_2 in columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         পঠ ignore word list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           পঠ only word list from this FILE\n\
"));
      oputs (_("\
  -r, --references               first field of each সারি is a reference\n\
"));
      oputs (_("\
  -t, --typeset-mode             change the পূর্বনির্ধারিত width from 72 to 100\n\
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
স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"auto-reference", no_argument, ফাঁকা, 'A'},
  {"break-file", required_argument, ফাঁকা, 'b'},
  {"flag-truncation", required_argument, ফাঁকা, 'F'},
  {"ignore-case", no_argument, ফাঁকা, 'f'},
  {"gap-size", required_argument, ফাঁকা, 'g'},
  {"ignore-file", required_argument, ফাঁকা, 'i'},
  {"macro-name", required_argument, ফাঁকা, 'M'},
  {"only-file", required_argument, ফাঁকা, 'o'},
  {"references", no_argument, ফাঁকা, 'r'},
  {"right-side-refs", no_argument, ফাঁকা, 'R'},
  {"format", required_argument, ফাঁকা, FORMAT_OPTION},
  {"sentence-regexp", required_argument, ফাঁকা, 'S'},
  {"traditional", no_argument, ফাঁকা, 'G'},
  {"typeset-mode", no_argument, ফাঁকা, 't'},
  {"width", required_argument, ফাঁকা, 'w'},
  {"word-regexp", required_argument, ফাঁকা, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0},
};

স্থির অক্ষর ধ্রুবক *ধ্রুবক format_args[] =
{
  "roff", "tex", ফাঁকা
};

স্থির গণনা Format ধ্রুবক format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা optchar;			/* argument character */
  পূর্ণসংখ্যা file_সূচক;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, ফাঁকা),
         optchar != EOF)
    {
      নির্বাচন (optchar)
        {
        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);

        ক্ষেত্র 'G':
          gnu_extensions = false;
          বিরতি;

        ক্ষেত্র 'b':
          break_file = optarg;
          বিরতি;

        ক্ষেত্র 'f':
          ignore_case = true;
          বিরতি;

        ক্ষেত্র 'g':
          {
            intmax_t tmp;
            যদি (! (xstrtoimax (optarg, ফাঁকা, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_আকার = tmp;
            বিরতি;
          }

        ক্ষেত্র 'i':
          ignore_file = optarg;
          বিরতি;

        ক্ষেত্র 'o':
          only_file = optarg;
          বিরতি;

        ক্ষেত্র 'r':
          প্রবেশ_reference = true;
          বিরতি;

        ক্ষেত্র 't':
          যদি (সারি_width < 0)
            সারি_width = 100;
          বিরতি;

        ক্ষেত্র 'w':
          {
            intmax_t tmp;
            যদি (! (xstrtoimax (optarg, ফাঁকা, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            সারি_width = tmp;
            বিরতি;
          }

        ক্ষেত্র 'A':
          auto_reference = true;
          বিরতি;

        ক্ষেত্র 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          বিরতি;

        ক্ষেত্র 'M':
          macro_name = optarg;
          বিরতি;

        ক্ষেত্র 'O':
          output_format = ROFF_FORMAT;
          বিরতি;

        ক্ষেত্র 'R':
          ডান_reference = true;
          বিরতি;

        ক্ষেত্র 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          বিরতি;

        ক্ষেত্র 'T':
          output_format = TEX_FORMAT;
          বিরতি;

        ক্ষেত্র 'W':
          word_regex.string = optarg;
          unescape_string (optarg);
          যদি (!*word_regex.string)
            word_regex.string = ফাঁকা;
          বিরতি;

        ক্ষেত্র FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_args, format_vals);
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        }
    }

  যদি (সারি_width < 0)
    সারি_width = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  যদি (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      প্রবেশ_file_name = xmalloc (আকার *প্রবেশ_file_name);
      file_সারি_গুনতি = xmalloc (আকার *file_সারি_গুনতি);
      text_buffers =    xmalloc (আকার *text_buffers);
      সংখ্যা_প্রবেশ_files = 1;
      প্রবেশ_file_name[0] = ফাঁকা;
    }
  অন্যথা যদি (gnu_extensions)
    {
      সংখ্যা_প্রবেশ_files = argc - optind;
      প্রবেশ_file_name = xnmalloc (সংখ্যা_প্রবেশ_files, আকার *প্রবেশ_file_name);
      file_সারি_গুনতি = xnmalloc (সংখ্যা_প্রবেশ_files, আকার *file_সারি_গুনতি);
      text_buffers    = xnmalloc (সংখ্যা_প্রবেশ_files, আকার *text_buffers);

      জন্য (file_সূচক = 0; file_সূচক < সংখ্যা_প্রবেশ_files; file_সূচক++)
        {
          যদি (!*argv[optind] || streq (argv[optind], "-"))
            প্রবেশ_file_name[file_সূচক] = ফাঁকা;
          অন্যথা
            প্রবেশ_file_name[file_সূচক] = argv[optind];
          optind++;
        }
    }
  অন্যথা
    {

      /* There is one necessary input file.  */

      সংখ্যা_প্রবেশ_files = 1;
      প্রবেশ_file_name = xmalloc (আকার *প্রবেশ_file_name);
      file_সারি_গুনতি = xmalloc (আকার *file_সারি_গুনতি);
      text_buffers    = xmalloc (আকার *text_buffers);
      যদি (!*argv[optind] || streq (argv[optind], "-"))
        প্রবেশ_file_name[0] = ফাঁকা;
      অন্যথা
        প্রবেশ_file_name[0] = argv[optind];
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
      যদি (ignore_table.দৈর্ঘ্য == 0)
        ignore_file = ফাঁকা;
    }

  যদি (only_file)
    {
      digest_word_file (only_file, &only_table);
      যদি (only_table.দৈর্ঘ্য == 0)
        only_file = ফাঁকা;
    }

  /* Prepare to study all the input files.  */

  সংখ্যা_of_occurs[0] = 0;
  যোগ_সারি_গুনতি = 0;
  maximum_word_দৈর্ঘ্য = 0;
  reference_max_width = 0;

  জন্য (file_সূচক = 0; file_সূচক < সংখ্যা_প্রবেশ_files; file_সূচক++)
    {
      BLOCK *text_বাফার = text_buffers + file_সূচক;

      /* Read the file contents into memory, then study it.  */

      swallow_file_in_memory (প্রবেশ_file_name[file_সূচক], text_বাফার);
      find_occurs_in_text (file_সূচক);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      যোগ_সারি_গুনতি++;
      file_সারি_গুনতি[file_সূচক] = যোগ_সারি_গুনতি;
    }

  /* Do the output process phase.  */

  sort_found_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  ফেরত EXIT_SUCCESS;
}
