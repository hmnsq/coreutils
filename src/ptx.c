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

#अन्तर्भाव <config.h>

#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव <regex.h>
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "octhexdigits.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "read-file.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#define AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#define CHAR_SET_SIZE 256

/* Debugging the memory allocator.  */

#यदि WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#endif

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

गणना Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

स्थिर bool gnu_extensions = true;	/* trigger all GNU extensions */
स्थिर bool auto_reference = false;	/* refs are 'file_name:line_number:' */
स्थिर bool आगत_reference = false;	/* refs at beginning of input lines */
स्थिर bool दायाँ_reference = false;	/* output refs after right context  */
स्थिर idx_t पङ्क्ति_width = -1;		/* output line width in characters */
स्थिर idx_t gap_size = 3;	/* number of spaces between output fields */
स्थिर वर्ण अचर *truncation_string = "/";
                                /* string used to mark line truncations */
स्थिर वर्ण अचर *macro_name = "xx";	/* macro name for roff or TeX output */
स्थिर गणना Format output_format = UNKNOWN_FORMAT;
                                /* output format */

स्थिर bool ignore_case = false;	/* fold lower to upper for sorting */
स्थिर वर्ण अचर *break_file = रिक्त; /* name of the 'Break chars' file */
स्थिर वर्ण अचर *only_file = रिक्त;	/* name of the 'Only words' file */
स्थिर वर्ण अचर *ignore_file = रिक्त; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
संरचना regex_data
{
  /* The original regular expression, as a string.  */
  वर्ण अचर *string;

  /* The compiled regular expression, and its fastmap.  */
  संरचना re_pattern_buffer pattern;
  वर्ण fastmap[अच_अक्षर_उच्च + 1];
};

स्थिर संरचना regex_data context_regex;	/* end of context */
स्थिर संरचना regex_data word_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

प्रकारनिर्देश संरचना
  {
    वर्ण *start;		/* pointer to beginning of region */
    वर्ण *end;			/* pointer to end + 1 of region */
  }
BLOCK;

प्रकारनिर्देश संरचना
  {
    वर्ण *start;		/* pointer to beginning of region */
    idx_t size;			/* length of the region */
  }
WORD;

प्रकारनिर्देश संरचना
  {
    idx_t alloc;		/* allocated length */
    idx_t length;		/* number of used entries */
    WORD *start COUNTED_BY (length);	/* array of WORDs */
  }
WORD_TABLE;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
स्थिर अचिन्हित वर्ण folded_chars[CHAR_SET_SIZE];

/* End of context pattern register indices.  */
स्थिर संरचना re_registers context_regs;

/* Keyword pattern register indices.  */
स्थिर संरचना re_registers word_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
स्थिर वर्ण word_fastmap[CHAR_SET_SIZE];

/* Maximum length of any word read.  */
स्थिर idx_t maximum_word_length;

/* Maximum width of any reference used.  */
स्थिर idx_t reference_max_width;

/* Ignore and Only word tables.  */

स्थिर WORD_TABLE ignore_table;	/* table of words to ignore */
स्थिर WORD_TABLE only_table;		/* table of words to select */

/* Source text table, and scanning macros.  */

स्थिर पूर्णाङ्क सङ्ख्या_आगत_files;	/* number of text input files */
स्थिर intmax_t total_पङ्क्ति_गन्ती;	/* total number of lines seen so far */
स्थिर वर्ण अचर **आगत_file_name;	/* array of text input file names */
स्थिर intmax_t *file_पङ्क्ति_गन्ती;	/* array of line count values at end */

स्थिर BLOCK *text_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#define SKIP_NON_WHITE(cursor, limit) \
  जबसम्म (cursor < limit && ! है_खाली (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE(cursor, limit) \
  जबसम्म (cursor < limit && है_खाली (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE_BACKWARDS(cursor, start) \
  जबसम्म (cursor > start && है_खाली (to_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#define SKIP_SOMETHING(cursor, limit) \
  यदि (word_regex.string)						\
    {									\
      regoff_t गन्ती;							\
      गन्ती = re_match (&word_regex.pattern, cursor, limit - cursor,	\
                        0, रिक्त);					\
      यदि (गन्ती == -2)							\
        matcher_error ();						\
      cursor += गन्ती <= 0 ? 1 : गन्ती;					\
    }									\
  अन्यथा यदि (word_fastmap[to_uchar (*cursor)])				\
    जबसम्म (cursor < limit && word_fastmap[to_uchar (*cursor)])		\
      cursor++;								\
  अन्यथा									\
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

प्रकारनिर्देश संरचना
  {
    WORD साँचो;			/* description of the keyword */
    सूचक_भेद_प्रकार बायाँ;		/* distance to left context start */
    सूचक_भेद_प्रकार दायाँ;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    पूर्णाङ्क file_index;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

स्थिर OCCURS *occurs_table[1];	/* all words retained from the read text */
स्थिर idx_t occurs_alloc[1];	/* allocated size of occurs_table */
स्थिर idx_t सङ्ख्या_of_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
स्थिर वर्ण edited_flag[CHAR_SET_SIZE];

/* Half of line width, reference excluded.  */
स्थिर idx_t half_पङ्क्ति_width;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
स्थिर सूचक_भेद_प्रकार before_max_width;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
स्थिर सूचक_भेद_प्रकार keyafter_max_width;

/* Length of string that flags truncation.  */
स्थिर idx_t truncation_string_length;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

स्थिर BLOCK tail;		/* tail field */
स्थिर bool tail_truncation;	/* flag truncation after the tail field */

स्थिर BLOCK before;		/* before field */
स्थिर bool before_truncation;	/* flag truncation before the before field */

स्थिर BLOCK keyafter;		/* keyword-and-after field */
स्थिर bool keyafter_truncation; /* flag truncation after the keyafter field */

स्थिर BLOCK head;		/* head field */
स्थिर bool head_truncation;	/* flag truncation before the head field */

स्थिर BLOCK reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

स्थिर _Noreturn शून्य
matcher_error (शून्य)
{
  error (निकास_त्रुटि, त्रुटि_सं, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

स्थिर शून्य
unescape_string (वर्ण *string)
{
  वर्ण *cursor;			/* cursor in result */
  पूर्णाङ्क मान;			/* value of \nnn escape */
  पूर्णाङ्क length;			/* length of \nnn escape */

  cursor = string;

  जबसम्म (*string)
    {
      यदि (*string == '\\')
        {
          string++;
          विकल्प (*string)
            {
            पक्ष 'x':		/* \xhhh escape, 3 chars maximum */
              मान = 0;
              लागि (length = 0, string++;
                   length < 3 && c_isxdigit (*string);
                   length++, string++)
                मान = मान * 16 + fromhex (*string);
              यदि (length == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              अन्यथा
                *cursor++ = मान;
              रोक;

            पक्ष '0':		/* \0ooo escape, 3 chars maximum */
              मान = 0;
              लागि (length = 0, string++;
                   length < 3 && isoct (*string);
                   length++, string++)
                मान = मान * 8 + fromoct (*string);
              *cursor++ = मान;
              रोक;

            पक्ष 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              रोक;

            पक्ष 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              रोक;

            पक्ष 'c':		/* cancel the rest of the output */
              जबसम्म (*string)
                string++;
              रोक;

            पक्ष 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              रोक;

            पक्ष 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              रोक;

            पक्ष 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              रोक;

            पक्ष 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              रोक;

            पक्ष 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              रोक;

            पक्ष '\0':		/* lone backslash at end of string */
              /* ignore it */
              रोक;

            स्वभाव:
              *cursor++ = '\\';
              *cursor++ = *string++;
              रोक;
            }
        }
      अन्यथा
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

स्थिर शून्य
compile_regex (संरचना regex_data *regex)
{
  संरचना re_pattern_buffer *pattern = &regex->pattern;
  वर्ण अचर *string = regex->string;
  वर्ण अचर *message;

  pattern->buffer = रिक्त;
  pattern->allocated = 0;
  pattern->fastmap = regex->fastmap;
  pattern->translate = ignore_case ? folded_chars : रिक्त;

  message = re_compile_pattern (string, म_माप (string), pattern);
  यदि (message)
    error (निकास_त्रुटि, 0, _("%s (for regexp %s)"), message, quote (string));

  /* The fastmap should be compiled before 're_match'.  The following
     call is not mandatory, because 're_search' is always called sooner,
     and it compiles the fastmap if this has not been done yet.  */

  re_compile_fastmap (pattern);
}

/*------------------------------------------------------------------------.
| This will initialize various tables for pattern match and compiles some |
| regexps.								  |
`------------------------------------------------------------------------*/

स्थिर शून्य
initialize_regex (शून्य)
{
  /* Initialize the case folding table.  */

  यदि (ignore_case)
    लागि (पूर्णाङ्क character = 0; character < CHAR_SET_SIZE; character++)
      folded_chars[character] = बड़े (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  यदि (context_regex.string)
    {
      यदि (!*context_regex.string)
        context_regex.string = रिक्त;
    }
  अन्यथा यदि (gnu_extensions && !आगत_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  अन्यथा
    context_regex.string = "\n";

  यदि (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  यदि (word_regex.string)
    compile_regex (&word_regex);
  अन्यथा यदि (!break_file)
    {
      यदि (gnu_extensions)
        {

          /* Simulate \w+.  */

          लागि (पूर्णाङ्क character = 0; character < CHAR_SET_SIZE; character++)
            word_fastmap[character] = !! है_अक्षर (character);
        }
      अन्यथा
        {

          /* Simulate [^ \t\n]+.  */

          स_रखो (word_fastmap, 1, CHAR_SET_SIZE);
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

स्थिर शून्य
swallow_file_in_memory (वर्ण अचर *file_name, BLOCK *block)
{
  माप_प्रकार used_length;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  यदि (using_stdin)
    block->start = fread_file (मानक_निवेश, 0, &used_length);
  अन्यथा
    block->start = read_file (file_name, 0, &used_length);

  यदि (!block->start)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (using_stdin ? "-" : file_name));

  यदि (using_stdin)
    साफ_त्रुटि (मानक_निवेश);

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

स्थिर पूर्णाङ्क
compare_शब्दहरू (अचर शून्य *void_first, अचर शून्य *void_second)
{
  WORD अचर *first = void_first;
  WORD अचर *second = void_second;
  idx_t length = MIN (first->size, second->size);

  यदि (ignore_case)
    {
      लागि (idx_t counter = 0; counter < length; counter++)
        {
          पूर्णाङ्क मान = (folded_chars[to_uchar (first->start[counter])]
                       - folded_chars[to_uchar (second->start[counter])]);
          यदि (मान != 0)
            फर्काऊ मान;
        }
    }
  अन्यथा
    {
      लागि (idx_t counter = 0; counter < length; counter++)
        {
          पूर्णाङ्क मान = (to_uchar (first->start[counter])
                       - to_uchar (second->start[counter]));
          यदि (मान != 0)
            फर्काऊ मान;
        }
    }

  फर्काऊ _GL_CMP (first->size, second->size);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

स्थिर पूर्णाङ्क
compare_occurs (अचर शून्य *void_first, अचर शून्य *void_second)
{
#define first ((अचर OCCURS *) void_first)
#define second ((अचर OCCURS *) void_second)
  पूर्णाङ्क मान;

  मान = compare_शब्दहरू (&first->साँचो, &second->साँचो);
  फर्काऊ (मान ? मान
          : _GL_CMP (first->साँचो.start, second->साँचो.start));
#undef first
#undef second
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_PURE
स्थिर bool
खोज_table (WORD *word, WORD_TABLE *table)
{
  idx_t lo = 0;
  idx_t hi = table->length;
  जबसम्म (lo < hi)
    {
      idx_t बीच = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      पूर्णाङ्क मान = compare_शब्दहरू (word, table->start + बीच);
      यदि (मान < 0)
        hi = बीच;
      अन्यथा यदि (मान > 0)
        lo = बीच + 1;
      अन्यथा
        फर्काऊ true;
    }
  फर्काऊ false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

स्थिर शून्य
sort_found_occurs (शून्य)
{

  /* Only one language for the time being.  */
  यदि (सङ्ख्या_of_occurs[0])
    क्विक (occurs_table[0], सङ्ख्या_of_occurs[0], परिमाण **occurs_table,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

स्थिर शून्य
digest_break_file (वर्ण अचर *file_name)
{
  BLOCK file_contents;		/* to receive a copy of the file */

  swallow_file_in_memory (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  स_रखो (word_fastmap, 1, CHAR_SET_SIZE);
  लागि (वर्ण *cursor = file_contents.start; cursor < file_contents.end;
       cursor++)
    word_fastmap[to_uchar (*cursor)] = 0;

  यदि (!gnu_extensions)
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

  मुक्त (file_contents.start);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

स्थिर शून्य
digest_word_file (वर्ण अचर *file_name, WORD_TABLE *table)
{
  BLOCK file_contents;		/* to receive a copy of the file */
  वर्ण *cursor;			/* cursor in file copy */
  वर्ण *word_start;		/* start of the current word */

  swallow_file_in_memory (file_name, &file_contents);

  table->start = रिक्त;
  table->alloc = 0;
  table->length = 0;

  /* Read the whole file.  */

  cursor = file_contents.start;
  जबसम्म (cursor < file_contents.end)
    {

      /* Read one line, and save the word in contains.  */

      word_start = cursor;
      जबसम्म (cursor < file_contents.end && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      यदि (cursor > word_start)
        {
          यदि (table->length == table->alloc)
            table->start = xpalloc (table->start, &table->alloc, 1, -1,
                                    परिमाण *table->start);
          idx_t अचर table_idx = table->length++;
          table->start[table_idx].start = word_start;
          table->start[table_idx].size = cursor - word_start;
        }

      /* This test allows for an incomplete line at end of file.  */

      यदि (cursor < file_contents.end)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  क्विक (table->start, table->length, परिमाण table->start[0], compare_शब्दहरू);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

स्थिर शून्य
find_occurs_in_text (पूर्णाङ्क file_index)
{
  वर्ण *scan;			/* for scanning the source text */
  वर्ण *पङ्क्ति_start;		/* start of the current input line */
  वर्ण *पङ्क्ति_scan;		/* newlines scanned until this point */
  idx_t reference_length;	/* length of reference in input mode */
  WORD possible_साँचो;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  वर्ण *context_start;		/* start of left context */
  वर्ण *context_end;		/* end of right context */
  वर्ण *word_start;		/* start of word */
  वर्ण *word_end;		/* end of word */
  वर्ण *next_context_start;	/* next start of left context */

  अचर BLOCK *text_buffer = &text_buffers[file_index];

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

  पङ्क्ति_start = text_buffer->start;
  पङ्क्ति_scan = पङ्क्ति_start;
  यदि (आगत_reference)
    {
      SKIP_NON_WHITE (पङ्क्ति_scan, text_buffer->end);
      reference_length = पङ्क्ति_scan - पङ्क्ति_start;
      SKIP_WHITE (पङ्क्ति_scan, text_buffer->end);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  लागि (वर्ण *cursor = text_buffer->start;
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
      यदि (context_regex.string)
        विकल्प (re_खोज (&context_regex.pattern, cursor,
                           text_buffer->end - cursor,
                           0, text_buffer->end - cursor, &context_regs))
          {
          पक्ष -2:
            matcher_error ();

          पक्ष -1:
            रोक;

          पक्ष 0:
            error (निकास_त्रुटि, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          स्वभाव:
            next_context_start = cursor + context_regs.end[0];
            रोक;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_end = next_context_start;
      SKIP_WHITE_BACKWARDS (context_end, context_start);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      जबसम्म (true)
        {
          यदि (word_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_खोज (&word_regex.pattern, cursor,
                                      context_end - cursor,
                                      0, context_end - cursor, &word_regs);
              यदि (r == -2)
                matcher_error ();
              यदि (r == -1)
                रोक;
              word_start = cursor + word_regs.start[0];
              word_end = cursor + word_regs.end[0];
            }
          अन्यथा

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              scan = cursor;
              जबसम्म (scan < context_end
                     && !word_fastmap[to_uchar (*scan)])
                scan++;

              यदि (scan == context_end)
                रोक;

              word_start = scan;

              जबसम्म (scan < context_end
                     && word_fastmap[to_uchar (*scan)])
                scan++;

              word_end = scan;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = word_start;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          यदि (word_end == word_start)
            {
              cursor++;
              जारी;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_साँचो.start = cursor;
          possible_साँचो.size = word_end - word_start;
          cursor += possible_साँचो.size;

          यदि (possible_साँचो.size > maximum_word_length)
            maximum_word_length = possible_साँचो.size;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          यदि (आगत_reference)
            {
              जबसम्म (पङ्क्ति_scan < possible_साँचो.start)
                यदि (*पङ्क्ति_scan == '\n')
                  {
                    total_पङ्क्ति_गन्ती++;
                    पङ्क्ति_scan++;
                    पङ्क्ति_start = पङ्क्ति_scan;
                    SKIP_NON_WHITE (पङ्क्ति_scan, text_buffer->end);
                    reference_length = पङ्क्ति_scan - पङ्क्ति_start;
                  }
                अन्यथा
                  पङ्क्ति_scan++;
              यदि (पङ्क्ति_scan > possible_साँचो.start)
                जारी;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          यदि (ignore_file && खोज_table (&possible_साँचो, &ignore_table))
            जारी;
          यदि (only_file && !खोज_table (&possible_साँचो, &only_table))
            जारी;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          यदि (सङ्ख्या_of_occurs[0] == occurs_alloc[0])
            occurs_table[0] = xpalloc (occurs_table[0], &occurs_alloc[0],
                                       1, -1, परिमाण *occurs_table[0]);
          occurs_cursor = occurs_table[0] + सङ्ख्या_of_occurs[0];

          /* Define the reference field, if any.  */

          यदि (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              जबसम्म (पङ्क्ति_scan < possible_साँचो.start)
                यदि (*पङ्क्ति_scan == '\n')
                  {
                    total_पङ्क्ति_गन्ती++;
                    पङ्क्ति_scan++;
                    पङ्क्ति_start = पङ्क्ति_scan;
                    SKIP_NON_WHITE (पङ्क्ति_scan, text_buffer->end);
                  }
                अन्यथा
                  पङ्क्ति_scan++;

              occurs_cursor->reference = total_पङ्क्ति_गन्ती;
            }
          अन्यथा यदि (आगत_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = पङ्क्ति_start - possible_साँचो.start;
              यदि (reference_length > reference_max_width)
                reference_max_width = reference_length;
            }

          /* Exclude the reference from the context in simple cases.  */

          यदि (आगत_reference && पङ्क्ति_start == context_start)
            {
              SKIP_NON_WHITE (context_start, context_end);
              SKIP_WHITE (context_start, context_end);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->साँचो = possible_साँचो;
          occurs_cursor->बायाँ = context_start - possible_साँचो.start;
          occurs_cursor->दायाँ = context_end - possible_साँचो.start;
          occurs_cursor->file_index = file_index;

          सङ्ख्या_of_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

स्थिर शून्य
print_spaces (सूचक_भेद_प्रकार सङ्ख्या)
{
  लागि (सूचक_भेद_प्रकार counter = सङ्ख्या; counter > 0; counter--)
    अक्षर_दो (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

स्थिर शून्य
print_field (BLOCK field)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  लागि (वर्ण *cursor = field.start; cursor < field.end; cursor++)
    {
      अचिन्हित वर्ण character = *cursor;
      यदि (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          विकल्प (character)
            {
            पक्ष '"':
              /* In roff output format, double any quote.  */
              अक्षर_दो ('"');
              अक्षर_दो ('"');
              रोक;

            पक्ष '$':
            पक्ष '%':
            पक्ष '&':
            पक्ष '#':
            पक्ष '_':
              /* In TeX output format, precede these with a backslash.  */
              अक्षर_दो ('\\');
              अक्षर_दो (character);
              रोक;

            पक्ष '{':
            पक्ष '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              म_लिखो ("$\\%c$", character);
              रोक;

            पक्ष '\\':
              /* In TeX output mode, request production of a backslash.  */
              ख_माला_दो ("\\backslash{}", मानक_निकास);
              रोक;

            स्वभाव:
              /* Any other flagged character produces a single space.  */
              अक्षर_दो (' ');
            }
        }
      अन्यथा
        अक्षर_दो (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

स्थिर शून्य
fix_output_parameters (शून्य)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  यदि (auto_reference)
    {
      reference_max_width = 0;
      लागि (पूर्णाङ्क file_index = 0; file_index < सङ्ख्या_आगत_files; file_index++)
        {
          intmax_t पङ्क्ति_ordinal = file_पङ्क्ति_गन्ती[file_index] + 1;
          यदि (file_index > 0)
            पङ्क्ति_ordinal -= file_पङ्क्ति_गन्ती[file_index - 1];
          वर्ण ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_width = प्र_लिखो (ordinal_string, "%jd", पङ्क्ति_ordinal);
          यदि (आगत_file_name[file_index])
            reference_width += म_माप (आगत_file_name[file_index]);
          यदि (reference_width > reference_max_width)
            reference_max_width = reference_width;
        }
      reference_max_width++;
      reference.start = xmalloc (reference_max_width + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  यदि ((auto_reference || आगत_reference) && !दायाँ_reference)
    पङ्क्ति_width = MAX (0, पङ्क्ति_width - (reference_max_width + gap_size));

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

  half_पङ्क्ति_width = पङ्क्ति_width >> 1;
  before_max_width = half_पङ्क्ति_width - gap_size;
  keyafter_max_width = half_पङ्क्ति_width;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  यदि (truncation_string && *truncation_string)
    truncation_string_length = म_माप (truncation_string);
  अन्यथा
    truncation_string = रिक्त;

  यदि (gnu_extensions)
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
  अन्यथा
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_width -= 2 * truncation_string_length + 1;
    }

  यदि (before_max_width < 0)
    before_max_width = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  लागि (पूर्णाङ्क character = 0; character < CHAR_SET_SIZE; character++)
    edited_flag[character] = !! है_खाली (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  विकल्प (output_format)
    {
    पक्ष UNKNOWN_FORMAT:
      /* Should never happen.  */

    पक्ष DUMB_FORMAT:
      रोक;

    पक्ष ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      रोक;

    पक्ष TEX_FORMAT:

      /* Various characters need special processing.  */

      लागि (वर्ण अचर *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[to_uchar (*cursor)] = 1;

      रोक;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

स्थिर शून्य
define_all_fields (OCCURS *occurs)
{
  सूचक_भेद_प्रकार tail_max_width;	/* allowable width of tail field */
  सूचक_भेद_प्रकार head_max_width;	/* allowable width of head field */
  वर्ण *cursor;			/* running cursor in source text */
  वर्ण *बायाँ_context_start;	/* start of left context */
  वर्ण *दायाँ_context_end;	/* end of right context */
  वर्ण *बायाँ_field_start;	/* conservative start for 'head'/'before' */
  वर्ण अचर *file_name;	/* file name for reference */
  intmax_t पङ्क्ति_ordinal;	/* line ordinal for reference */
  वर्ण अचर *buffer_start;	/* start of buffered file for this occurs */
  वर्ण अचर *buffer_end;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.start = occurs->साँचो.start;
  keyafter.end = keyafter.start + occurs->साँचो.size;
  बायाँ_context_start = keyafter.start + occurs->बायाँ;
  दायाँ_context_end = keyafter.start + occurs->दायाँ;

  buffer_start = text_buffers[occurs->file_index].start;
  buffer_end = text_buffers[occurs->file_index].end;

  cursor = keyafter.end;
  जबसम्म (cursor < दायाँ_context_end
         && cursor <= keyafter.start + keyafter_max_width)
    {
      keyafter.end = cursor;
      SKIP_SOMETHING (cursor, दायाँ_context_end);
    }
  यदि (cursor <= keyafter.start + keyafter_max_width)
    keyafter.end = cursor;

  keyafter_truncation = truncation_string && keyafter.end < दायाँ_context_end;

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

  यदि (-occurs->बायाँ > half_पङ्क्ति_width + maximum_word_length)
    {
      बायाँ_field_start
        = keyafter.start - (half_पङ्क्ति_width + maximum_word_length);
      SKIP_SOMETHING (बायाँ_field_start, keyafter.start);
    }
  अन्यथा
    बायाँ_field_start = keyafter.start + occurs->बायाँ;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.start = बायाँ_field_start;
  before.end = keyafter.start;
  SKIP_WHITE_BACKWARDS (before.end, before.start);

  जबसम्म (before.start + before_max_width < before.end)
    SKIP_SOMETHING (before.start, before.end);

  यदि (truncation_string)
    {
      cursor = before.start;
      SKIP_WHITE_BACKWARDS (cursor, buffer_start);
      before_truncation = cursor > बायाँ_context_start;
    }
  अन्यथा
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

  यदि (tail_max_width > 0)
    {
      tail.start = keyafter.end;
      SKIP_WHITE (tail.start, buffer_end);

      tail.end = tail.start;
      cursor = tail.end;
      जबसम्म (cursor < दायाँ_context_end
             && cursor < tail.start + tail_max_width)
        {
          tail.end = cursor;
          SKIP_SOMETHING (cursor, दायाँ_context_end);
        }

      यदि (cursor < tail.start + tail_max_width)
        tail.end = cursor;

      यदि (tail.end > tail.start)
        {
          keyafter_truncation = false;
          tail_truncation = truncation_string && tail.end < दायाँ_context_end;
        }
      अन्यथा
        tail_truncation = false;

      SKIP_WHITE_BACKWARDS (tail.end, tail.start);
    }
  अन्यथा
    {

      /* No place left for a tail field.  */

      tail.start = रिक्त;
      tail.end = रिक्त;
      tail_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  head_max_width
    = keyafter_max_width - (keyafter.end - keyafter.start) - gap_size;

  यदि (head_max_width > 0)
    {
      head.end = before.start;
      SKIP_WHITE_BACKWARDS (head.end, buffer_start);

      head.start = बायाँ_field_start;
      जबसम्म (head.start + head_max_width < head.end)
        SKIP_SOMETHING (head.start, head.end);

      यदि (head.end > head.start)
        {
          before_truncation = false;
          head_truncation = (truncation_string
                             && head.start > बायाँ_context_start);
        }
      अन्यथा
        head_truncation = false;

      SKIP_WHITE (head.start, head.end);
    }
  अन्यथा
    {

      /* No place left for a head field.  */

      head.start = रिक्त;
      head.end = रिक्त;
      head_truncation = false;
    }

  यदि (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = आगत_file_name[occurs->file_index];
      यदि (!file_name)
        file_name = "";

      पङ्क्ति_ordinal = occurs->reference + 1;
      यदि (occurs->file_index > 0)
        पङ्क्ति_ordinal -= file_पङ्क्ति_गन्ती[occurs->file_index - 1];

      वर्ण *file_end = stpcpy (reference.start, file_name);
      reference.end = file_end + प्र_लिखो (file_end, ":%jd", पङ्क्ति_ordinal);
    }
  अन्यथा यदि (आगत_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.start = keyafter.start + occurs->reference;
      reference.end = reference.start;
      SKIP_NON_WHITE (reference.end, दायाँ_context_end);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

स्थिर शून्य
output_one_roff_पङ्क्ति (शून्य)
{
  /* Output the 'tail' field.  */

  म_लिखो (".%s \"", macro_name);
  print_field (tail);
  यदि (tail_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);
  अक्षर_दो ('"');

  /* Output the 'before' field.  */

  ख_माला_दो (" \"", मानक_निकास);
  यदि (before_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);
  print_field (before);
  अक्षर_दो ('"');

  /* Output the 'keyafter' field.  */

  ख_माला_दो (" \"", मानक_निकास);
  print_field (keyafter);
  यदि (keyafter_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);
  अक्षर_दो ('"');

  /* Output the 'head' field.  */

  ख_माला_दो (" \"", मानक_निकास);
  यदि (head_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);
  print_field (head);
  अक्षर_दो ('"');

  /* Conditionally output the 'reference' field.  */

  यदि (auto_reference || आगत_reference)
    {
      ख_माला_दो (" \"", मानक_निकास);
      print_field (reference);
      अक्षर_दो ('"');
    }

  अक्षर_दो ('\n');
}

/*---------------------------------------------------------.
| Output the current output fields as one line for 'TeX'.  |
`---------------------------------------------------------*/

स्थिर शून्य
output_one_tex_पङ्क्ति (शून्य)
{
  BLOCK साँचो;			/* key field, isolated */
  BLOCK after;			/* after field, isolated */
  वर्ण *cursor;			/* running cursor in source text */

  म_लिखो ("\\%s ", macro_name);
  अक्षर_दो ('{');
  print_field (tail);
  ख_माला_दो ("}{", मानक_निकास);
  print_field (before);
  ख_माला_दो ("}{", मानक_निकास);
  साँचो.start = keyafter.start;
  after.end = keyafter.end;
  cursor = keyafter.start;
  SKIP_SOMETHING (cursor, keyafter.end);
  साँचो.end = cursor;
  after.start = cursor;
  print_field (साँचो);
  ख_माला_दो ("}{", मानक_निकास);
  print_field (after);
  ख_माला_दो ("}{", मानक_निकास);
  print_field (head);
  अक्षर_दो ('}');
  यदि (auto_reference || आगत_reference)
    {
      अक्षर_दो ('{');
      print_field (reference);
      अक्षर_दो ('}');
    }
  अक्षर_दो ('\n');
}

/*-------------------------------------------------------------------.
| Output the current output fields as one line for a dumb terminal.  |
`-------------------------------------------------------------------*/

स्थिर शून्य
output_one_dumb_पङ्क्ति (शून्य)
{
  यदि (!दायाँ_reference)
    {
      यदि (auto_reference)
        {

          /* Output the 'reference' field, in such a way that GNU emacs
             next-error will handle it.  The ending colon is taken from the
             gap which follows.  */

          print_field (reference);
          अक्षर_दो (':');
          print_spaces (reference_max_width
                        + gap_size
                        - (reference.end - reference.start)
                        - 1);
        }
      अन्यथा
        {

          /* Output the 'reference' field and its following gap.  */

          print_field (reference);
          print_spaces (reference_max_width
                        + gap_size
                        - (reference.end - reference.start));
        }
    }

  यदि (tail.start < tail.end)
    {
      /* Output the 'tail' field.  */

      print_field (tail);
      यदि (tail_truncation)
        ख_माला_दो (truncation_string, मानक_निकास);

      print_spaces (half_पङ्क्ति_width - gap_size
                    - (before.end - before.start)
                    - (before_truncation ? truncation_string_length : 0)
                    - (tail.end - tail.start)
                    - (tail_truncation ? truncation_string_length : 0));
    }
  अन्यथा
    print_spaces (half_पङ्क्ति_width - gap_size
                  - (before.end - before.start)
                  - (before_truncation ? truncation_string_length : 0));

  /* Output the 'before' field.  */

  यदि (before_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);
  print_field (before);

  print_spaces (gap_size);

  /* Output the 'keyafter' field.  */

  print_field (keyafter);
  यदि (keyafter_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);

  यदि (head.start < head.end)
    {
      /* Output the 'head' field.  */

      print_spaces (half_पङ्क्ति_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0)
                    - (head.end - head.start)
                    - (head_truncation ? truncation_string_length : 0));
      यदि (head_truncation)
        ख_माला_दो (truncation_string, मानक_निकास);
      print_field (head);
    }
  अन्यथा

    यदि ((auto_reference || आगत_reference) && दायाँ_reference)
      print_spaces (half_पङ्क्ति_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0));

  यदि ((auto_reference || आगत_reference) && दायाँ_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_size);
      print_field (reference);
    }

  अक्षर_दो ('\n');
}

/*------------------------------------------------------------------------.
| Scan the whole occurs table and, for each entry, output one line in the |
| appropriate format.							  |
`------------------------------------------------------------------------*/

स्थिर शून्य
generate_all_output (शून्य)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  tail.start = रिक्त;
  tail.end = रिक्त;
  tail_truncation = false;

  head.start = रिक्त;
  head.end = रिक्त;
  head_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_table[0];

  लागि (idx_t occurs_index = 0; occurs_index < सङ्ख्या_of_occurs[0];
       occurs_index++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      विकल्प (output_format)
        {
        पक्ष UNKNOWN_FORMAT:
          /* Should never happen.  */

        पक्ष DUMB_FORMAT:
          output_one_dumb_पङ्क्ति ();
          रोक;

        पक्ष ROFF_FORMAT:
          output_one_roff_पङ्क्ति ();
          रोक;

        पक्ष TEX_FORMAT:
          output_one_tex_पङ्क्ति ();
          रोक;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [आगत_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [आगत_2 [OUTPUT]]\n"),
              program_name, program_name);
      ख_माला_दो (_("\
Output a permuted index, including context, of the शब्दहरू in the आगत files.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --स्वतः-reference           output automatically generated references\n\
"));
      oputs (_("\
  -G, --traditional              behave more like System V 'ptx'\n\
"));
      oputs (_("\
  -F, --flag-truncation=STRING   use STRING लागि flagging पङ्क्ति truncations.\n\
                                 The स्वभाव is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name to use instead of 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --दायाँ-side-refs          put references at दायाँ, not counted in -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   लागि end of पङ्क्तिहरू or end of sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --word-regexp=REGEXP       use REGEXP to match each keyword\n\
"));
      oputs (_("\
  -b, --रोक-file=खाता          word रोक characters in this खाता\n\
"));
      oputs (_("\
  -f, --ignore-पक्ष              fold lower पक्ष to upper पक्ष लागि sorting\n\
"));
      oputs (_("\
  -g, --gap-size=सङ्ख्या_2          gap size in columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=खाता         पठ ignore word list from खाता\n\
"));
      oputs (_("\
  -o, --only-file=खाता           पठ only word list from this खाता\n\
"));
      oputs (_("\
  -r, --references               first field of each पङ्क्ति is a reference\n\
"));
      oputs (_("\
  -t, --typeset-mode             change the स्वभाव width from 72 to 100\n\
"));
      oputs (_("\
  -w, --width=सङ्ख्या_2             output width in columns, reference excluded\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/*----------------------------------------------------------------------.
| Main program.  Decode ARGC arguments passed through the ARGV array of |
| strings, then launch execution.				        |
`----------------------------------------------------------------------*/

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  FORMAT_OPTION = अक्षर_उच्च + 1
};

/* Long options equivalences.  */
स्थिर संरचना option अचर long_options[] =
{
  {"auto-reference", no_argument, रिक्त, 'A'},
  {"break-file", required_argument, रिक्त, 'b'},
  {"flag-truncation", required_argument, रिक्त, 'F'},
  {"ignore-case", no_argument, रिक्त, 'f'},
  {"gap-size", required_argument, रिक्त, 'g'},
  {"ignore-file", required_argument, रिक्त, 'i'},
  {"macro-name", required_argument, रिक्त, 'M'},
  {"only-file", required_argument, रिक्त, 'o'},
  {"references", no_argument, रिक्त, 'r'},
  {"right-side-refs", no_argument, रिक्त, 'R'},
  {"format", required_argument, रिक्त, FORMAT_OPTION},
  {"sentence-regexp", required_argument, रिक्त, 'S'},
  {"traditional", no_argument, रिक्त, 'G'},
  {"typeset-mode", no_argument, रिक्त, 't'},
  {"width", required_argument, रिक्त, 'w'},
  {"word-regexp", required_argument, रिक्त, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0},
};

स्थिर वर्ण अचर *अचर format_args[] =
{
  "roff", "tex", रिक्त
};

स्थिर गणना Format अचर format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optchar;			/* argument character */
  पूर्णाङ्क file_index;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबसम्म (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, रिक्त),
         optchar != खातापूर्ण)
    {
      विकल्प (optchar)
        {
        स्वभाव:
          usage (निकास_त्रुटि);

        पक्ष 'G':
          gnu_extensions = false;
          रोक;

        पक्ष 'b':
          break_file = optarg;
          रोक;

        पक्ष 'f':
          ignore_case = true;
          रोक;

        पक्ष 'g':
          {
            intmax_t tmp;
            यदि (! (xstrtoimax (optarg, रिक्त, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (निकास_त्रुटि, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_size = tmp;
            रोक;
          }

        पक्ष 'i':
          ignore_file = optarg;
          रोक;

        पक्ष 'o':
          only_file = optarg;
          रोक;

        पक्ष 'r':
          आगत_reference = true;
          रोक;

        पक्ष 't':
          यदि (पङ्क्ति_width < 0)
            पङ्क्ति_width = 100;
          रोक;

        पक्ष 'w':
          {
            intmax_t tmp;
            यदि (! (xstrtoimax (optarg, रिक्त, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (निकास_त्रुटि, 0, _("invalid line width: %s"),
                     quote (optarg));
            पङ्क्ति_width = tmp;
            रोक;
          }

        पक्ष 'A':
          auto_reference = true;
          रोक;

        पक्ष 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          रोक;

        पक्ष 'M':
          macro_name = optarg;
          रोक;

        पक्ष 'O':
          output_format = ROFF_FORMAT;
          रोक;

        पक्ष 'R':
          दायाँ_reference = true;
          रोक;

        पक्ष 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          रोक;

        पक्ष 'T':
          output_format = TEX_FORMAT;
          रोक;

        पक्ष 'W':
          word_regex.string = optarg;
          unescape_string (optarg);
          यदि (!*word_regex.string)
            word_regex.string = रिक्त;
          रोक;

        पक्ष FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_args, format_vals);
          रोक;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        }
    }

  यदि (पङ्क्ति_width < 0)
    पङ्क्ति_width = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  यदि (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      आगत_file_name = xmalloc (परिमाण *आगत_file_name);
      file_पङ्क्ति_गन्ती = xmalloc (परिमाण *file_पङ्क्ति_गन्ती);
      text_buffers =    xmalloc (परिमाण *text_buffers);
      सङ्ख्या_आगत_files = 1;
      आगत_file_name[0] = रिक्त;
    }
  अन्यथा यदि (gnu_extensions)
    {
      सङ्ख्या_आगत_files = argc - optind;
      आगत_file_name = xnmalloc (सङ्ख्या_आगत_files, परिमाण *आगत_file_name);
      file_पङ्क्ति_गन्ती = xnmalloc (सङ्ख्या_आगत_files, परिमाण *file_पङ्क्ति_गन्ती);
      text_buffers    = xnmalloc (सङ्ख्या_आगत_files, परिमाण *text_buffers);

      लागि (file_index = 0; file_index < सङ्ख्या_आगत_files; file_index++)
        {
          यदि (!*argv[optind] || streq (argv[optind], "-"))
            आगत_file_name[file_index] = रिक्त;
          अन्यथा
            आगत_file_name[file_index] = argv[optind];
          optind++;
        }
    }
  अन्यथा
    {

      /* There is one necessary input file.  */

      सङ्ख्या_आगत_files = 1;
      आगत_file_name = xmalloc (परिमाण *आगत_file_name);
      file_पङ्क्ति_गन्ती = xmalloc (परिमाण *file_पङ्क्ति_गन्ती);
      text_buffers    = xmalloc (परिमाण *text_buffers);
      यदि (!*argv[optind] || streq (argv[optind], "-"))
        आगत_file_name[0] = रिक्त;
      अन्यथा
        आगत_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      यदि (optind < argc)
        {
          यदि (! ख_व_खोलो (argv[optind], "w", मानक_निकास))
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      यदि (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (निकास_त्रुटि);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  यदि (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  यदि (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  यदि (ignore_file)
    {
      digest_word_file (ignore_file, &ignore_table);
      यदि (ignore_table.length == 0)
        ignore_file = रिक्त;
    }

  यदि (only_file)
    {
      digest_word_file (only_file, &only_table);
      यदि (only_table.length == 0)
        only_file = रिक्त;
    }

  /* Prepare to study all the input files.  */

  सङ्ख्या_of_occurs[0] = 0;
  total_पङ्क्ति_गन्ती = 0;
  maximum_word_length = 0;
  reference_max_width = 0;

  लागि (file_index = 0; file_index < सङ्ख्या_आगत_files; file_index++)
    {
      BLOCK *text_buffer = text_buffers + file_index;

      /* Read the file contents into memory, then study it.  */

      swallow_file_in_memory (आगत_file_name[file_index], text_buffer);
      find_occurs_in_text (file_index);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      total_पङ्क्ति_गन्ती++;
      file_पङ्क्ति_गन्ती[file_index] = total_पङ्क्ति_गन्ती;
    }

  /* Do the output process phase.  */

  sort_found_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  फर्काऊ निकास_सफल;
}
