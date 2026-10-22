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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ <regex.h>
#અન્તર્ભાવ "argmatch.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "octhexdigits.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "read-file.h"
#અન્તર્ભાવ "stdio--.h"
#અન્તર્ભાવ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#define AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#define CHAR_SET_SIZE 256

/* Debugging the memory allocator.  */

#જો WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#endif

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

ગણના Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

સ્થિર bool gnu_extensions = true;	/* trigger all GNU extensions */
સ્થિર bool auto_reference = false;	/* refs are 'file_name:line_number:' */
સ્થિર bool આગત_reference = false;	/* refs at beginning of input lines */
સ્થિર bool જમણું_reference = false;	/* output refs after right context  */
સ્થિર idx_t લીટી_width = -1;		/* output line width in characters */
સ્થિર idx_t gap_size = 3;	/* number of spaces between output fields */
સ્થિર અક્ષર અચલ *truncation_string = "/";
                                /* string used to mark line truncations */
સ્થિર અક્ષર અચલ *macro_name = "xx";	/* macro name for roff or TeX output */
સ્થિર ગણના Format output_format = UNKNOWN_FORMAT;
                                /* output format */

સ્થિર bool ignore_case = false;	/* fold lower to upper for sorting */
સ્થિર અક્ષર અચલ *break_file = ખાલી; /* name of the 'Break chars' file */
સ્થિર અક્ષર અચલ *only_file = ખાલી;	/* name of the 'Only words' file */
સ્થિર અક્ષર અચલ *ignore_file = ખાલી; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
સંરચના regex_data
{
  /* The original regular expression, as a string.  */
  અક્ષર અચલ *string;

  /* The compiled regular expression, and its fastmap.  */
  સંરચના re_pattern_buffer pattern;
  અક્ષર fastmap[UCHAR_MAX + 1];
};

સ્થિર સંરચના regex_data context_regex;	/* end of context */
સ્થિર સંરચના regex_data word_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

પ્રકારવ્યાખ્યા સંરચના
  {
    અક્ષર *start;		/* pointer to beginning of region */
    અક્ષર *end;			/* pointer to end + 1 of region */
  }
BLOCK;

પ્રકારવ્યાખ્યા સંરચના
  {
    અક્ષર *start;		/* pointer to beginning of region */
    idx_t size;			/* length of the region */
  }
WORD;

પ્રકારવ્યાખ્યા સંરચના
  {
    idx_t alloc;		/* allocated length */
    idx_t length;		/* number of used entries */
    WORD *start COUNTED_BY (length);	/* array of WORDs */
  }
WORD_TABLE;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
સ્થિર unsigned અક્ષર folded_chars[CHAR_SET_SIZE];

/* End of context pattern register indices.  */
સ્થિર સંરચના re_registers context_regs;

/* Keyword pattern register indices.  */
સ્થિર સંરચના re_registers word_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
સ્થિર અક્ષર word_fastmap[CHAR_SET_SIZE];

/* Maximum length of any word read.  */
સ્થિર idx_t maximum_word_length;

/* Maximum width of any reference used.  */
સ્થિર idx_t reference_max_width;

/* Ignore and Only word tables.  */

સ્થિર WORD_TABLE ignore_table;	/* table of words to ignore */
સ્થિર WORD_TABLE only_table;		/* table of words to select */

/* Source text table, and scanning macros.  */

સ્થિર પૂર્ણાંક સંખ્યા_આગત_files;	/* number of text input files */
સ્થિર intmax_t total_લીટી_ગણતરી;	/* total number of lines seen so far */
સ્થિર અક્ષર અચલ **આગત_file_name;	/* array of text input file names */
સ્થિર intmax_t *file_લીટી_ગણતરી;	/* array of line count values at end */

સ્થિર BLOCK *text_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#define SKIP_NON_WHITE(cursor, limit) \
  જ્યારે (cursor < limit && ! isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE(cursor, limit) \
  જ્યારે (cursor < limit && isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE_BACKWARDS(cursor, start) \
  જ્યારે (cursor > start && isspace (to_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#define SKIP_SOMETHING(cursor, limit) \
  જો (word_regex.string)						\
    {									\
      regoff_t ગણતરી;							\
      ગણતરી = re_match (&word_regex.pattern, cursor, limit - cursor,	\
                        0, ખાલી);					\
      જો (ગણતરી == -2)							\
        matcher_error ();						\
      cursor += ગણતરી <= 0 ? 1 : ગણતરી;					\
    }									\
  નહીંતર જો (word_fastmap[to_uchar (*cursor)])				\
    જ્યારે (cursor < limit && word_fastmap[to_uchar (*cursor)])		\
      cursor++;								\
  નહીંતર									\
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

પ્રકારવ્યાખ્યા સંરચના
  {
    WORD ચાવી;			/* description of the keyword */
    ptrdiff_t ડાબું;		/* distance to left context start */
    ptrdiff_t જમણું;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    પૂર્ણાંક file_index;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

સ્થિર OCCURS *occurs_table[1];	/* all words retained from the read text */
સ્થિર idx_t occurs_alloc[1];	/* allocated size of occurs_table */
સ્થિર idx_t સંખ્યા_of_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
સ્થિર અક્ષર edited_flag[CHAR_SET_SIZE];

/* Half of line width, reference excluded.  */
સ્થિર idx_t half_લીટી_width;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
સ્થિર ptrdiff_t before_max_width;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
સ્થિર ptrdiff_t keyafter_max_width;

/* Length of string that flags truncation.  */
સ્થિર idx_t truncation_string_length;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

સ્થિર BLOCK tail;		/* tail field */
સ્થિર bool tail_truncation;	/* flag truncation after the tail field */

સ્થિર BLOCK before;		/* before field */
સ્થિર bool before_truncation;	/* flag truncation before the before field */

સ્થિર BLOCK keyafter;		/* keyword-and-after field */
સ્થિર bool keyafter_truncation; /* flag truncation after the keyafter field */

સ્થિર BLOCK head;		/* head field */
સ્થિર bool head_truncation;	/* flag truncation before the head field */

સ્થિર BLOCK reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

સ્થિર _Noreturn શૂન્ય
matcher_error (શૂન્ય)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

સ્થિર શૂન્ય
unescape_string (અક્ષર *string)
{
  અક્ષર *cursor;			/* cursor in result */
  પૂર્ણાંક કિંમત;			/* value of \nnn escape */
  પૂર્ણાંક length;			/* length of \nnn escape */

  cursor = string;

  જ્યારે (*string)
    {
      જો (*string == '\\')
        {
          string++;
          પસંદગી (*string)
            {
            સ્થિતિ 'x':		/* \xhhh escape, 3 chars maximum */
              કિંમત = 0;
              માટે (length = 0, string++;
                   length < 3 && c_isxdigit (*string);
                   length++, string++)
                કિંમત = કિંમત * 16 + fromhex (*string);
              જો (length == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              નહીંતર
                *cursor++ = કિંમત;
              તોડો;

            સ્થિતિ '0':		/* \0ooo escape, 3 chars maximum */
              કિંમત = 0;
              માટે (length = 0, string++;
                   length < 3 && isoct (*string);
                   length++, string++)
                કિંમત = કિંમત * 8 + fromoct (*string);
              *cursor++ = કિંમત;
              તોડો;

            સ્થિતિ 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              તોડો;

            સ્થિતિ 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              તોડો;

            સ્થિતિ 'c':		/* cancel the rest of the output */
              જ્યારે (*string)
                string++;
              તોડો;

            સ્થિતિ 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              તોડો;

            સ્થિતિ 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              તોડો;

            સ્થિતિ 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              તોડો;

            સ્થિતિ 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              તોડો;

            સ્થિતિ 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              તોડો;

            સ્થિતિ '\0':		/* lone backslash at end of string */
              /* ignore it */
              તોડો;

            મૂળભૂત:
              *cursor++ = '\\';
              *cursor++ = *string++;
              તોડો;
            }
        }
      નહીંતર
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

સ્થિર શૂન્ય
compile_regex (સંરચના regex_data *regex)
{
  સંરચના re_pattern_buffer *pattern = &regex->pattern;
  અક્ષર અચલ *string = regex->string;
  અક્ષર અચલ *message;

  pattern->buffer = ખાલી;
  pattern->allocated = 0;
  pattern->fastmap = regex->fastmap;
  pattern->translate = ignore_case ? folded_chars : ખાલી;

  message = re_compile_pattern (string, strlen (string), pattern);
  જો (message)
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

સ્થિર શૂન્ય
initialize_regex (શૂન્ય)
{
  /* Initialize the case folding table.  */

  જો (ignore_case)
    માટે (પૂર્ણાંક character = 0; character < CHAR_SET_SIZE; character++)
      folded_chars[character] = toupper (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  જો (context_regex.string)
    {
      જો (!*context_regex.string)
        context_regex.string = ખાલી;
    }
  નહીંતર જો (gnu_extensions && !આગત_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  નહીંતર
    context_regex.string = "\n";

  જો (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  જો (word_regex.string)
    compile_regex (&word_regex);
  નહીંતર જો (!break_file)
    {
      જો (gnu_extensions)
        {

          /* Simulate \w+.  */

          માટે (પૂર્ણાંક character = 0; character < CHAR_SET_SIZE; character++)
            word_fastmap[character] = !! isalpha (character);
        }
      નહીંતર
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

સ્થિર શૂન્ય
swallow_file_in_memory (અક્ષર અચલ *file_name, BLOCK *block)
{
  size_t used_length;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  જો (using_stdin)
    block->start = fread_file (stdin, 0, &used_length);
  નહીંતર
    block->start = read_file (file_name, 0, &used_length);

  જો (!block->start)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  જો (using_stdin)
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

સ્થિર પૂર્ણાંક
compare_શબ્દો (અચલ શૂન્ય *void_first, અચલ શૂન્ય *void_second)
{
  WORD અચલ *first = void_first;
  WORD અચલ *second = void_second;
  idx_t length = MIN (first->size, second->size);

  જો (ignore_case)
    {
      માટે (idx_t counter = 0; counter < length; counter++)
        {
          પૂર્ણાંક કિંમત = (folded_chars[to_uchar (first->start[counter])]
                       - folded_chars[to_uchar (second->start[counter])]);
          જો (કિંમત != 0)
            પાછા કિંમત;
        }
    }
  નહીંતર
    {
      માટે (idx_t counter = 0; counter < length; counter++)
        {
          પૂર્ણાંક કિંમત = (to_uchar (first->start[counter])
                       - to_uchar (second->start[counter]));
          જો (કિંમત != 0)
            પાછા કિંમત;
        }
    }

  પાછા _GL_CMP (first->size, second->size);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

સ્થિર પૂર્ણાંક
compare_occurs (અચલ શૂન્ય *void_first, અચલ શૂન્ય *void_second)
{
#define first ((અચલ OCCURS *) void_first)
#define second ((અચલ OCCURS *) void_second)
  પૂર્ણાંક કિંમત;

  કિંમત = compare_શબ્દો (&first->ચાવી, &second->ચાવી);
  પાછા (કિંમત ? કિંમત
          : _GL_CMP (first->ચાવી.start, second->ચાવી.start));
#undef first
#undef second
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_PURE
સ્થિર bool
શોધ_table (WORD *word, WORD_TABLE *table)
{
  idx_t lo = 0;
  idx_t hi = table->length;
  જ્યારે (lo < hi)
    {
      idx_t મધ્ય = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      પૂર્ણાંક કિંમત = compare_શબ્દો (word, table->start + મધ્ય);
      જો (કિંમત < 0)
        hi = મધ્ય;
      નહીંતર જો (કિંમત > 0)
        lo = મધ્ય + 1;
      નહીંતર
        પાછા true;
    }
  પાછા false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

સ્થિર શૂન્ય
sort_found_occurs (શૂન્ય)
{

  /* Only one language for the time being.  */
  જો (સંખ્યા_of_occurs[0])
    qsort (occurs_table[0], સંખ્યા_of_occurs[0], કદ **occurs_table,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

સ્થિર શૂન્ય
digest_break_file (અક્ષર અચલ *file_name)
{
  BLOCK file_contents;		/* to receive a copy of the file */

  swallow_file_in_memory (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (word_fastmap, 1, CHAR_SET_SIZE);
  માટે (અક્ષર *cursor = file_contents.start; cursor < file_contents.end;
       cursor++)
    word_fastmap[to_uchar (*cursor)] = 0;

  જો (!gnu_extensions)
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

  મુક્ત (file_contents.start);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

સ્થિર શૂન્ય
digest_word_file (અક્ષર અચલ *file_name, WORD_TABLE *table)
{
  BLOCK file_contents;		/* to receive a copy of the file */
  અક્ષર *cursor;			/* cursor in file copy */
  અક્ષર *word_start;		/* start of the current word */

  swallow_file_in_memory (file_name, &file_contents);

  table->start = ખાલી;
  table->alloc = 0;
  table->length = 0;

  /* Read the whole file.  */

  cursor = file_contents.start;
  જ્યારે (cursor < file_contents.end)
    {

      /* Read one line, and save the word in contains.  */

      word_start = cursor;
      જ્યારે (cursor < file_contents.end && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      જો (cursor > word_start)
        {
          જો (table->length == table->alloc)
            table->start = xpalloc (table->start, &table->alloc, 1, -1,
                                    કદ *table->start);
          idx_t અચલ table_idx = table->length++;
          table->start[table_idx].start = word_start;
          table->start[table_idx].size = cursor - word_start;
        }

      /* This test allows for an incomplete line at end of file.  */

      જો (cursor < file_contents.end)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (table->start, table->length, કદ table->start[0], compare_શબ્દો);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

સ્થિર શૂન્ય
find_occurs_in_text (પૂર્ણાંક file_index)
{
  અક્ષર *scan;			/* for scanning the source text */
  અક્ષર *લીટી_start;		/* start of the current input line */
  અક્ષર *લીટી_scan;		/* newlines scanned until this point */
  idx_t reference_length;	/* length of reference in input mode */
  WORD possible_ચાવી;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  અક્ષર *context_start;		/* start of left context */
  અક્ષર *context_end;		/* end of right context */
  અક્ષર *word_start;		/* start of word */
  અક્ષર *word_end;		/* end of word */
  અક્ષર *next_context_start;	/* next start of left context */

  અચલ BLOCK *text_buffer = &text_buffers[file_index];

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

  લીટી_start = text_buffer->start;
  લીટી_scan = લીટી_start;
  જો (આગત_reference)
    {
      SKIP_NON_WHITE (લીટી_scan, text_buffer->end);
      reference_length = લીટી_scan - લીટી_start;
      SKIP_WHITE (લીટી_scan, text_buffer->end);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  માટે (અક્ષર *cursor = text_buffer->start;
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
      જો (context_regex.string)
        પસંદગી (re_શોધ (&context_regex.pattern, cursor,
                           text_buffer->end - cursor,
                           0, text_buffer->end - cursor, &context_regs))
          {
          સ્થિતિ -2:
            matcher_error ();

          સ્થિતિ -1:
            તોડો;

          સ્થિતિ 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          મૂળભૂત:
            next_context_start = cursor + context_regs.end[0];
            તોડો;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_end = next_context_start;
      SKIP_WHITE_BACKWARDS (context_end, context_start);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      જ્યારે (true)
        {
          જો (word_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_શોધ (&word_regex.pattern, cursor,
                                      context_end - cursor,
                                      0, context_end - cursor, &word_regs);
              જો (r == -2)
                matcher_error ();
              જો (r == -1)
                તોડો;
              word_start = cursor + word_regs.start[0];
              word_end = cursor + word_regs.end[0];
            }
          નહીંતર

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              scan = cursor;
              જ્યારે (scan < context_end
                     && !word_fastmap[to_uchar (*scan)])
                scan++;

              જો (scan == context_end)
                તોડો;

              word_start = scan;

              જ્યારે (scan < context_end
                     && word_fastmap[to_uchar (*scan)])
                scan++;

              word_end = scan;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = word_start;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          જો (word_end == word_start)
            {
              cursor++;
              ચાલુ;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_ચાવી.start = cursor;
          possible_ચાવી.size = word_end - word_start;
          cursor += possible_ચાવી.size;

          જો (possible_ચાવી.size > maximum_word_length)
            maximum_word_length = possible_ચાવી.size;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          જો (આગત_reference)
            {
              જ્યારે (લીટી_scan < possible_ચાવી.start)
                જો (*લીટી_scan == '\n')
                  {
                    total_લીટી_ગણતરી++;
                    લીટી_scan++;
                    લીટી_start = લીટી_scan;
                    SKIP_NON_WHITE (લીટી_scan, text_buffer->end);
                    reference_length = લીટી_scan - લીટી_start;
                  }
                નહીંતર
                  લીટી_scan++;
              જો (લીટી_scan > possible_ચાવી.start)
                ચાલુ;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          જો (ignore_file && શોધ_table (&possible_ચાવી, &ignore_table))
            ચાલુ;
          જો (only_file && !શોધ_table (&possible_ચાવી, &only_table))
            ચાલુ;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          જો (સંખ્યા_of_occurs[0] == occurs_alloc[0])
            occurs_table[0] = xpalloc (occurs_table[0], &occurs_alloc[0],
                                       1, -1, કદ *occurs_table[0]);
          occurs_cursor = occurs_table[0] + સંખ્યા_of_occurs[0];

          /* Define the reference field, if any.  */

          જો (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              જ્યારે (લીટી_scan < possible_ચાવી.start)
                જો (*લીટી_scan == '\n')
                  {
                    total_લીટી_ગણતરી++;
                    લીટી_scan++;
                    લીટી_start = લીટી_scan;
                    SKIP_NON_WHITE (લીટી_scan, text_buffer->end);
                  }
                નહીંતર
                  લીટી_scan++;

              occurs_cursor->reference = total_લીટી_ગણતરી;
            }
          નહીંતર જો (આગત_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = લીટી_start - possible_ચાવી.start;
              જો (reference_length > reference_max_width)
                reference_max_width = reference_length;
            }

          /* Exclude the reference from the context in simple cases.  */

          જો (આગત_reference && લીટી_start == context_start)
            {
              SKIP_NON_WHITE (context_start, context_end);
              SKIP_WHITE (context_start, context_end);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->ચાવી = possible_ચાવી;
          occurs_cursor->ડાબું = context_start - possible_ચાવી.start;
          occurs_cursor->જમણું = context_end - possible_ચાવી.start;
          occurs_cursor->file_index = file_index;

          સંખ્યા_of_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

સ્થિર શૂન્ય
print_spaces (ptrdiff_t સંખ્યા)
{
  માટે (ptrdiff_t counter = સંખ્યા; counter > 0; counter--)
    putchar (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

સ્થિર શૂન્ય
print_field (BLOCK field)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  માટે (અક્ષર *cursor = field.start; cursor < field.end; cursor++)
    {
      unsigned અક્ષર character = *cursor;
      જો (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          પસંદગી (character)
            {
            સ્થિતિ '"':
              /* In roff output format, double any quote.  */
              putchar ('"');
              putchar ('"');
              તોડો;

            સ્થિતિ '$':
            સ્થિતિ '%':
            સ્થિતિ '&':
            સ્થિતિ '#':
            સ્થિતિ '_':
              /* In TeX output format, precede these with a backslash.  */
              putchar ('\\');
              putchar (character);
              તોડો;

            સ્થિતિ '{':
            સ્થિતિ '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              printf ("$\\%c$", character);
              તોડો;

            સ્થિતિ '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              તોડો;

            મૂળભૂત:
              /* Any other flagged character produces a single space.  */
              putchar (' ');
            }
        }
      નહીંતર
        putchar (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

સ્થિર શૂન્ય
fix_output_parameters (શૂન્ય)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  જો (auto_reference)
    {
      reference_max_width = 0;
      માટે (પૂર્ણાંક file_index = 0; file_index < સંખ્યા_આગત_files; file_index++)
        {
          intmax_t લીટી_ordinal = file_લીટી_ગણતરી[file_index] + 1;
          જો (file_index > 0)
            લીટી_ordinal -= file_લીટી_ગણતરી[file_index - 1];
          અક્ષર ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_width = sprintf (ordinal_string, "%jd", લીટી_ordinal);
          જો (આગત_file_name[file_index])
            reference_width += strlen (આગત_file_name[file_index]);
          જો (reference_width > reference_max_width)
            reference_max_width = reference_width;
        }
      reference_max_width++;
      reference.start = xmalloc (reference_max_width + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  જો ((auto_reference || આગત_reference) && !જમણું_reference)
    લીટી_width = MAX (0, લીટી_width - (reference_max_width + gap_size));

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

  half_લીટી_width = લીટી_width >> 1;
  before_max_width = half_લીટી_width - gap_size;
  keyafter_max_width = half_લીટી_width;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  જો (truncation_string && *truncation_string)
    truncation_string_length = strlen (truncation_string);
  નહીંતર
    truncation_string = ખાલી;

  જો (gnu_extensions)
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
  નહીંતર
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_width -= 2 * truncation_string_length + 1;
    }

  જો (before_max_width < 0)
    before_max_width = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  માટે (પૂર્ણાંક character = 0; character < CHAR_SET_SIZE; character++)
    edited_flag[character] = !! isspace (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  પસંદગી (output_format)
    {
    સ્થિતિ UNKNOWN_FORMAT:
      /* Should never happen.  */

    સ્થિતિ DUMB_FORMAT:
      તોડો;

    સ્થિતિ ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      તોડો;

    સ્થિતિ TEX_FORMAT:

      /* Various characters need special processing.  */

      માટે (અક્ષર અચલ *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[to_uchar (*cursor)] = 1;

      તોડો;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

સ્થિર શૂન્ય
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t tail_max_width;	/* allowable width of tail field */
  ptrdiff_t head_max_width;	/* allowable width of head field */
  અક્ષર *cursor;			/* running cursor in source text */
  અક્ષર *ડાબું_context_start;	/* start of left context */
  અક્ષર *જમણું_context_end;	/* end of right context */
  અક્ષર *ડાબું_field_start;	/* conservative start for 'head'/'before' */
  અક્ષર અચલ *file_name;	/* file name for reference */
  intmax_t લીટી_ordinal;	/* line ordinal for reference */
  અક્ષર અચલ *buffer_start;	/* start of buffered file for this occurs */
  અક્ષર અચલ *buffer_end;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.start = occurs->ચાવી.start;
  keyafter.end = keyafter.start + occurs->ચાવી.size;
  ડાબું_context_start = keyafter.start + occurs->ડાબું;
  જમણું_context_end = keyafter.start + occurs->જમણું;

  buffer_start = text_buffers[occurs->file_index].start;
  buffer_end = text_buffers[occurs->file_index].end;

  cursor = keyafter.end;
  જ્યારે (cursor < જમણું_context_end
         && cursor <= keyafter.start + keyafter_max_width)
    {
      keyafter.end = cursor;
      SKIP_SOMETHING (cursor, જમણું_context_end);
    }
  જો (cursor <= keyafter.start + keyafter_max_width)
    keyafter.end = cursor;

  keyafter_truncation = truncation_string && keyafter.end < જમણું_context_end;

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

  જો (-occurs->ડાબું > half_લીટી_width + maximum_word_length)
    {
      ડાબું_field_start
        = keyafter.start - (half_લીટી_width + maximum_word_length);
      SKIP_SOMETHING (ડાબું_field_start, keyafter.start);
    }
  નહીંતર
    ડાબું_field_start = keyafter.start + occurs->ડાબું;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.start = ડાબું_field_start;
  before.end = keyafter.start;
  SKIP_WHITE_BACKWARDS (before.end, before.start);

  જ્યારે (before.start + before_max_width < before.end)
    SKIP_SOMETHING (before.start, before.end);

  જો (truncation_string)
    {
      cursor = before.start;
      SKIP_WHITE_BACKWARDS (cursor, buffer_start);
      before_truncation = cursor > ડાબું_context_start;
    }
  નહીંતર
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

  જો (tail_max_width > 0)
    {
      tail.start = keyafter.end;
      SKIP_WHITE (tail.start, buffer_end);

      tail.end = tail.start;
      cursor = tail.end;
      જ્યારે (cursor < જમણું_context_end
             && cursor < tail.start + tail_max_width)
        {
          tail.end = cursor;
          SKIP_SOMETHING (cursor, જમણું_context_end);
        }

      જો (cursor < tail.start + tail_max_width)
        tail.end = cursor;

      જો (tail.end > tail.start)
        {
          keyafter_truncation = false;
          tail_truncation = truncation_string && tail.end < જમણું_context_end;
        }
      નહીંતર
        tail_truncation = false;

      SKIP_WHITE_BACKWARDS (tail.end, tail.start);
    }
  નહીંતર
    {

      /* No place left for a tail field.  */

      tail.start = ખાલી;
      tail.end = ખાલી;
      tail_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  head_max_width
    = keyafter_max_width - (keyafter.end - keyafter.start) - gap_size;

  જો (head_max_width > 0)
    {
      head.end = before.start;
      SKIP_WHITE_BACKWARDS (head.end, buffer_start);

      head.start = ડાબું_field_start;
      જ્યારે (head.start + head_max_width < head.end)
        SKIP_SOMETHING (head.start, head.end);

      જો (head.end > head.start)
        {
          before_truncation = false;
          head_truncation = (truncation_string
                             && head.start > ડાબું_context_start);
        }
      નહીંતર
        head_truncation = false;

      SKIP_WHITE (head.start, head.end);
    }
  નહીંતર
    {

      /* No place left for a head field.  */

      head.start = ખાલી;
      head.end = ખાલી;
      head_truncation = false;
    }

  જો (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = આગત_file_name[occurs->file_index];
      જો (!file_name)
        file_name = "";

      લીટી_ordinal = occurs->reference + 1;
      જો (occurs->file_index > 0)
        લીટી_ordinal -= file_લીટી_ગણતરી[occurs->file_index - 1];

      અક્ષર *file_end = stpcpy (reference.start, file_name);
      reference.end = file_end + sprintf (file_end, ":%jd", લીટી_ordinal);
    }
  નહીંતર જો (આગત_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.start = keyafter.start + occurs->reference;
      reference.end = reference.start;
      SKIP_NON_WHITE (reference.end, જમણું_context_end);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

સ્થિર શૂન્ય
output_one_roff_લીટી (શૂન્ય)
{
  /* Output the 'tail' field.  */

  printf (".%s \"", macro_name);
  print_field (tail);
  જો (tail_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  જો (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);
  putchar ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_field (keyafter);
  જો (keyafter_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  જો (head_truncation)
    fputs (truncation_string, stdout);
  print_field (head);
  putchar ('"');

  /* Conditionally output the 'reference' field.  */

  જો (auto_reference || આગત_reference)
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

સ્થિર શૂન્ય
output_one_tex_લીટી (શૂન્ય)
{
  BLOCK ચાવી;			/* key field, isolated */
  BLOCK after;			/* after field, isolated */
  અક્ષર *cursor;			/* running cursor in source text */

  printf ("\\%s ", macro_name);
  putchar ('{');
  print_field (tail);
  fputs ("}{", stdout);
  print_field (before);
  fputs ("}{", stdout);
  ચાવી.start = keyafter.start;
  after.end = keyafter.end;
  cursor = keyafter.start;
  SKIP_SOMETHING (cursor, keyafter.end);
  ચાવી.end = cursor;
  after.start = cursor;
  print_field (ચાવી);
  fputs ("}{", stdout);
  print_field (after);
  fputs ("}{", stdout);
  print_field (head);
  putchar ('}');
  જો (auto_reference || આગત_reference)
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

સ્થિર શૂન્ય
output_one_dumb_લીટી (શૂન્ય)
{
  જો (!જમણું_reference)
    {
      જો (auto_reference)
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
      નહીંતર
        {

          /* Output the 'reference' field and its following gap.  */

          print_field (reference);
          print_spaces (reference_max_width
                        + gap_size
                        - (reference.end - reference.start));
        }
    }

  જો (tail.start < tail.end)
    {
      /* Output the 'tail' field.  */

      print_field (tail);
      જો (tail_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_લીટી_width - gap_size
                    - (before.end - before.start)
                    - (before_truncation ? truncation_string_length : 0)
                    - (tail.end - tail.start)
                    - (tail_truncation ? truncation_string_length : 0));
    }
  નહીંતર
    print_spaces (half_લીટી_width - gap_size
                  - (before.end - before.start)
                  - (before_truncation ? truncation_string_length : 0));

  /* Output the 'before' field.  */

  જો (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);

  print_spaces (gap_size);

  /* Output the 'keyafter' field.  */

  print_field (keyafter);
  જો (keyafter_truncation)
    fputs (truncation_string, stdout);

  જો (head.start < head.end)
    {
      /* Output the 'head' field.  */

      print_spaces (half_લીટી_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0)
                    - (head.end - head.start)
                    - (head_truncation ? truncation_string_length : 0));
      જો (head_truncation)
        fputs (truncation_string, stdout);
      print_field (head);
    }
  નહીંતર

    જો ((auto_reference || આગત_reference) && જમણું_reference)
      print_spaces (half_લીટી_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0));

  જો ((auto_reference || આગત_reference) && જમણું_reference)
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

સ્થિર શૂન્ય
generate_all_output (શૂન્ય)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  tail.start = ખાલી;
  tail.end = ખાલી;
  tail_truncation = false;

  head.start = ખાલી;
  head.end = ખાલી;
  head_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_table[0];

  માટે (idx_t occurs_index = 0; occurs_index < સંખ્યા_of_occurs[0];
       occurs_index++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      પસંદગી (output_format)
        {
        સ્થિતિ UNKNOWN_FORMAT:
          /* Should never happen.  */

        સ્થિતિ DUMB_FORMAT:
          output_one_dumb_લીટી ();
          તોડો;

        સ્થિતિ ROFF_FORMAT:
          output_one_roff_લીટી ();
          તોડો;

        સ્થિતિ TEX_FORMAT:
          output_one_tex_લીટી ();
          તોડો;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [આગત_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [આગત_2 [OUTPUT]]\n"),
              program_name, program_name);
      fputs (_("\
Output a permuted index, including context, of the શબ્દો in the આગત files.\n\
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
  -F, --flag-truncation=STRING   use STRING માટે flagging લીટી truncations.\n\
                                 The મૂળભૂત is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name to use instead of 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --જમણું-side-refs          put references at જમણું, not counted in -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   માટે end of લીટીઓ or end of sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --word-regexp=REGEXP       use REGEXP to match each keyword\n\
"));
      oputs (_("\
  -b, --તોડો-file=FILE          word તોડો characters in this FILE\n\
"));
      oputs (_("\
  -f, --ignore-સ્થિતિ              fold lower સ્થિતિ to upper સ્થિતિ માટે sorting\n\
"));
      oputs (_("\
  -g, --gap-size=સંખ્યા_2          gap size in columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         પઠ ignore word list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           પઠ only word list from this FILE\n\
"));
      oputs (_("\
  -r, --references               first field of each લીટી is a reference\n\
"));
      oputs (_("\
  -t, --typeset-mode             change the મૂળભૂત width from 72 to 100\n\
"));
      oputs (_("\
  -w, --width=સંખ્યા_2             output width in columns, reference excluded\n\
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
ગણના
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
સ્થિર સંરચના option અચલ long_options[] =
{
  {"auto-reference", no_argument, ખાલી, 'A'},
  {"break-file", required_argument, ખાલી, 'b'},
  {"flag-truncation", required_argument, ખાલી, 'F'},
  {"ignore-case", no_argument, ખાલી, 'f'},
  {"gap-size", required_argument, ખાલી, 'g'},
  {"ignore-file", required_argument, ખાલી, 'i'},
  {"macro-name", required_argument, ખાલી, 'M'},
  {"only-file", required_argument, ખાલી, 'o'},
  {"references", no_argument, ખાલી, 'r'},
  {"right-side-refs", no_argument, ખાલી, 'R'},
  {"format", required_argument, ખાલી, FORMAT_OPTION},
  {"sentence-regexp", required_argument, ખાલી, 'S'},
  {"traditional", no_argument, ખાલી, 'G'},
  {"typeset-mode", no_argument, ખાલી, 't'},
  {"width", required_argument, ખાલી, 'w'},
  {"word-regexp", required_argument, ખાલી, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0},
};

સ્થિર અક્ષર અચલ *અચલ format_args[] =
{
  "roff", "tex", ખાલી
};

સ્થિર ગણના Format અચલ format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક optchar;			/* argument character */
  પૂર્ણાંક file_index;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, ખાલી),
         optchar != EOF)
    {
      પસંદગી (optchar)
        {
        મૂળભૂત:
          usage (EXIT_FAILURE);

        સ્થિતિ 'G':
          gnu_extensions = false;
          તોડો;

        સ્થિતિ 'b':
          break_file = optarg;
          તોડો;

        સ્થિતિ 'f':
          ignore_case = true;
          તોડો;

        સ્થિતિ 'g':
          {
            intmax_t tmp;
            જો (! (xstrtoimax (optarg, ખાલી, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_size = tmp;
            તોડો;
          }

        સ્થિતિ 'i':
          ignore_file = optarg;
          તોડો;

        સ્થિતિ 'o':
          only_file = optarg;
          તોડો;

        સ્થિતિ 'r':
          આગત_reference = true;
          તોડો;

        સ્થિતિ 't':
          જો (લીટી_width < 0)
            લીટી_width = 100;
          તોડો;

        સ્થિતિ 'w':
          {
            intmax_t tmp;
            જો (! (xstrtoimax (optarg, ખાલી, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            લીટી_width = tmp;
            તોડો;
          }

        સ્થિતિ 'A':
          auto_reference = true;
          તોડો;

        સ્થિતિ 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          તોડો;

        સ્થિતિ 'M':
          macro_name = optarg;
          તોડો;

        સ્થિતિ 'O':
          output_format = ROFF_FORMAT;
          તોડો;

        સ્થિતિ 'R':
          જમણું_reference = true;
          તોડો;

        સ્થિતિ 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          તોડો;

        સ્થિતિ 'T':
          output_format = TEX_FORMAT;
          તોડો;

        સ્થિતિ 'W':
          word_regex.string = optarg;
          unescape_string (optarg);
          જો (!*word_regex.string)
            word_regex.string = ખાલી;
          તોડો;

        સ્થિતિ FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_args, format_vals);
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        }
    }

  જો (લીટી_width < 0)
    લીટી_width = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  જો (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      આગત_file_name = xmalloc (કદ *આગત_file_name);
      file_લીટી_ગણતરી = xmalloc (કદ *file_લીટી_ગણતરી);
      text_buffers =    xmalloc (કદ *text_buffers);
      સંખ્યા_આગત_files = 1;
      આગત_file_name[0] = ખાલી;
    }
  નહીંતર જો (gnu_extensions)
    {
      સંખ્યા_આગત_files = argc - optind;
      આગત_file_name = xnmalloc (સંખ્યા_આગત_files, કદ *આગત_file_name);
      file_લીટી_ગણતરી = xnmalloc (સંખ્યા_આગત_files, કદ *file_લીટી_ગણતરી);
      text_buffers    = xnmalloc (સંખ્યા_આગત_files, કદ *text_buffers);

      માટે (file_index = 0; file_index < સંખ્યા_આગત_files; file_index++)
        {
          જો (!*argv[optind] || streq (argv[optind], "-"))
            આગત_file_name[file_index] = ખાલી;
          નહીંતર
            આગત_file_name[file_index] = argv[optind];
          optind++;
        }
    }
  નહીંતર
    {

      /* There is one necessary input file.  */

      સંખ્યા_આગત_files = 1;
      આગત_file_name = xmalloc (કદ *આગત_file_name);
      file_લીટી_ગણતરી = xmalloc (કદ *file_લીટી_ગણતરી);
      text_buffers    = xmalloc (કદ *text_buffers);
      જો (!*argv[optind] || streq (argv[optind], "-"))
        આગત_file_name[0] = ખાલી;
      નહીંતર
        આગત_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      જો (optind < argc)
        {
          જો (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      જો (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  જો (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  જો (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  જો (ignore_file)
    {
      digest_word_file (ignore_file, &ignore_table);
      જો (ignore_table.length == 0)
        ignore_file = ખાલી;
    }

  જો (only_file)
    {
      digest_word_file (only_file, &only_table);
      જો (only_table.length == 0)
        only_file = ખાલી;
    }

  /* Prepare to study all the input files.  */

  સંખ્યા_of_occurs[0] = 0;
  total_લીટી_ગણતરી = 0;
  maximum_word_length = 0;
  reference_max_width = 0;

  માટે (file_index = 0; file_index < સંખ્યા_આગત_files; file_index++)
    {
      BLOCK *text_buffer = text_buffers + file_index;

      /* Read the file contents into memory, then study it.  */

      swallow_file_in_memory (આગત_file_name[file_index], text_buffer);
      find_occurs_in_text (file_index);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      total_લીટી_ગણતરી++;
      file_લીટી_ગણતરી[file_index] = total_લીટી_ગણતરી;
    }

  /* Do the output process phase.  */

  sort_found_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  પાછા EXIT_SUCCESS;
}
