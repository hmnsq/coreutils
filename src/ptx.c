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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ <regex.h>
#അന്തര്ഭാവ "argmatch.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "octhexdigits.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "read-file.h"
#അന്തര്ഭാവ "stdio--.h"
#അന്തര്ഭാവ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#define AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#define CHAR_SET_SIZE 256

/* Debugging the memory allocator.  */

#എങ്കിൽ WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#endif

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

എണ്ണൽ Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

സ്ഥിതം bool gnu_extensions = true;	/* trigger all GNU extensions */
സ്ഥിതം bool auto_reference = false;	/* refs are 'file_name:line_number:' */
സ്ഥിതം bool നിവേശം_reference = false;	/* refs at beginning of input lines */
സ്ഥിതം bool വലത്_reference = false;	/* output refs after right context  */
സ്ഥിതം idx_t വരി_width = -1;		/* output line width in characters */
സ്ഥിതം idx_t gap_size = 3;	/* number of spaces between output fields */
സ്ഥിതം അക്ഷരം സ്ഥിരം *truncation_string = "/";
                                /* string used to mark line truncations */
സ്ഥിതം അക്ഷരം സ്ഥിരം *macro_name = "xx";	/* macro name for roff or TeX output */
സ്ഥിതം എണ്ണൽ Format output_format = UNKNOWN_FORMAT;
                                /* output format */

സ്ഥിതം bool ignore_case = false;	/* fold lower to upper for sorting */
സ്ഥിതം അക്ഷരം സ്ഥിരം *break_file = ശൂന്യ; /* name of the 'Break chars' file */
സ്ഥിതം അക്ഷരം സ്ഥിരം *only_file = ശൂന്യ;	/* name of the 'Only words' file */
സ്ഥിതം അക്ഷരം സ്ഥിരം *ignore_file = ശൂന്യ; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
ഘടന regex_data
{
  /* The original regular expression, as a string.  */
  അക്ഷരം സ്ഥിരം *string;

  /* The compiled regular expression, and its fastmap.  */
  ഘടന re_pattern_buffer pattern;
  അക്ഷരം fastmap[UCHAR_MAX + 1];
};

സ്ഥിതം ഘടന regex_data context_regex;	/* end of context */
സ്ഥിതം ഘടന regex_data word_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

തരനിർവചനം ഘടന
  {
    അക്ഷരം *start;		/* pointer to beginning of region */
    അക്ഷരം *end;			/* pointer to end + 1 of region */
  }
BLOCK;

തരനിർവചനം ഘടന
  {
    അക്ഷരം *start;		/* pointer to beginning of region */
    idx_t size;			/* length of the region */
  }
WORD;

തരനിർവചനം ഘടന
  {
    idx_t alloc;		/* allocated length */
    idx_t length;		/* number of used entries */
    WORD *start COUNTED_BY (length);	/* array of WORDs */
  }
WORD_TABLE;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
സ്ഥിതം unsigned അക്ഷരം folded_chars[CHAR_SET_SIZE];

/* End of context pattern register indices.  */
സ്ഥിതം ഘടന re_registers context_regs;

/* Keyword pattern register indices.  */
സ്ഥിതം ഘടന re_registers word_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
സ്ഥിതം അക്ഷരം word_fastmap[CHAR_SET_SIZE];

/* Maximum length of any word read.  */
സ്ഥിതം idx_t maximum_word_length;

/* Maximum width of any reference used.  */
സ്ഥിതം idx_t reference_max_width;

/* Ignore and Only word tables.  */

സ്ഥിതം WORD_TABLE ignore_table;	/* table of words to ignore */
സ്ഥിതം WORD_TABLE only_table;		/* table of words to select */

/* Source text table, and scanning macros.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ സംഖ്യ_നിവേശം_files;	/* number of text input files */
സ്ഥിതം intmax_t total_വരി_എണ്ണം;	/* total number of lines seen so far */
സ്ഥിതം അക്ഷരം സ്ഥിരം **നിവേശം_file_name;	/* array of text input file names */
സ്ഥിതം intmax_t *file_വരി_എണ്ണം;	/* array of line count values at end */

സ്ഥിതം BLOCK *text_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#define SKIP_NON_WHITE(cursor, limit) \
  ആകുമ്പോൾ (cursor < limit && ! isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE(cursor, limit) \
  ആകുമ്പോൾ (cursor < limit && isspace (to_uchar (*cursor)))		\
    cursor++

#define SKIP_WHITE_BACKWARDS(cursor, start) \
  ആകുമ്പോൾ (cursor > start && isspace (to_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#define SKIP_SOMETHING(cursor, limit) \
  എങ്കിൽ (word_regex.string)						\
    {									\
      regoff_t എണ്ണം;							\
      എണ്ണം = re_match (&word_regex.pattern, cursor, limit - cursor,	\
                        0, ശൂന്യ);					\
      എങ്കിൽ (എണ്ണം == -2)							\
        matcher_error ();						\
      cursor += എണ്ണം <= 0 ? 1 : എണ്ണം;					\
    }									\
  അല്ലെങ്കിൽ എങ്കിൽ (word_fastmap[to_uchar (*cursor)])				\
    ആകുമ്പോൾ (cursor < limit && word_fastmap[to_uchar (*cursor)])		\
      cursor++;								\
  അല്ലെങ്കിൽ									\
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

തരനിർവചനം ഘടന
  {
    WORD താക്കോൽ;			/* description of the keyword */
    ptrdiff_t ഇടത്;		/* distance to left context start */
    ptrdiff_t വലത്;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    പൂർണ്ണസംഖ്യ file_index;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

സ്ഥിതം OCCURS *occurs_table[1];	/* all words retained from the read text */
സ്ഥിതം idx_t occurs_alloc[1];	/* allocated size of occurs_table */
സ്ഥിതം idx_t സംഖ്യ_of_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
സ്ഥിതം അക്ഷരം edited_flag[CHAR_SET_SIZE];

/* Half of line width, reference excluded.  */
സ്ഥിതം idx_t half_വരി_width;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
സ്ഥിതം ptrdiff_t before_max_width;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
സ്ഥിതം ptrdiff_t keyafter_max_width;

/* Length of string that flags truncation.  */
സ്ഥിതം idx_t truncation_string_length;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

സ്ഥിതം BLOCK tail;		/* tail field */
സ്ഥിതം bool tail_truncation;	/* flag truncation after the tail field */

സ്ഥിതം BLOCK before;		/* before field */
സ്ഥിതം bool before_truncation;	/* flag truncation before the before field */

സ്ഥിതം BLOCK keyafter;		/* keyword-and-after field */
സ്ഥിതം bool keyafter_truncation; /* flag truncation after the keyafter field */

സ്ഥിതം BLOCK head;		/* head field */
സ്ഥിതം bool head_truncation;	/* flag truncation before the head field */

സ്ഥിതം BLOCK reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

സ്ഥിതം _Noreturn ശൂന്യം
matcher_error (ശൂന്യം)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

സ്ഥിതം ശൂന്യം
unescape_string (അക്ഷരം *string)
{
  അക്ഷരം *cursor;			/* cursor in result */
  പൂർണ്ണസംഖ്യ മൂല്യം;			/* value of \nnn escape */
  പൂർണ്ണസംഖ്യ length;			/* length of \nnn escape */

  cursor = string;

  ആകുമ്പോൾ (*string)
    {
      എങ്കിൽ (*string == '\\')
        {
          string++;
          തിരഞ്ഞെടുപ്പ് (*string)
            {
            അവസ്ഥ 'x':		/* \xhhh escape, 3 chars maximum */
              മൂല്യം = 0;
              വേണ്ടി (length = 0, string++;
                   length < 3 && c_isxdigit (*string);
                   length++, string++)
                മൂല്യം = മൂല്യം * 16 + fromhex (*string);
              എങ്കിൽ (length == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              അല്ലെങ്കിൽ
                *cursor++ = മൂല്യം;
              നിർത്തുക;

            അവസ്ഥ '0':		/* \0ooo escape, 3 chars maximum */
              മൂല്യം = 0;
              വേണ്ടി (length = 0, string++;
                   length < 3 && isoct (*string);
                   length++, string++)
                മൂല്യം = മൂല്യം * 8 + fromoct (*string);
              *cursor++ = മൂല്യം;
              നിർത്തുക;

            അവസ്ഥ 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              നിർത്തുക;

            അവസ്ഥ 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              നിർത്തുക;

            അവസ്ഥ 'c':		/* cancel the rest of the output */
              ആകുമ്പോൾ (*string)
                string++;
              നിർത്തുക;

            അവസ്ഥ 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              നിർത്തുക;

            അവസ്ഥ 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              നിർത്തുക;

            അവസ്ഥ 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              നിർത്തുക;

            അവസ്ഥ 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              നിർത്തുക;

            അവസ്ഥ 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              നിർത്തുക;

            അവസ്ഥ '\0':		/* lone backslash at end of string */
              /* ignore it */
              നിർത്തുക;

            സ്ഥിരസ്ഥിതി:
              *cursor++ = '\\';
              *cursor++ = *string++;
              നിർത്തുക;
            }
        }
      അല്ലെങ്കിൽ
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
compile_regex (ഘടന regex_data *regex)
{
  ഘടന re_pattern_buffer *pattern = &regex->pattern;
  അക്ഷരം സ്ഥിരം *string = regex->string;
  അക്ഷരം സ്ഥിരം *message;

  pattern->buffer = ശൂന്യ;
  pattern->allocated = 0;
  pattern->fastmap = regex->fastmap;
  pattern->translate = ignore_case ? folded_chars : ശൂന്യ;

  message = re_compile_pattern (string, strlen (string), pattern);
  എങ്കിൽ (message)
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

സ്ഥിതം ശൂന്യം
initialize_regex (ശൂന്യം)
{
  /* Initialize the case folding table.  */

  എങ്കിൽ (ignore_case)
    വേണ്ടി (പൂർണ്ണസംഖ്യ character = 0; character < CHAR_SET_SIZE; character++)
      folded_chars[character] = toupper (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  എങ്കിൽ (context_regex.string)
    {
      എങ്കിൽ (!*context_regex.string)
        context_regex.string = ശൂന്യ;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (gnu_extensions && !നിവേശം_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  അല്ലെങ്കിൽ
    context_regex.string = "\n";

  എങ്കിൽ (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  എങ്കിൽ (word_regex.string)
    compile_regex (&word_regex);
  അല്ലെങ്കിൽ എങ്കിൽ (!break_file)
    {
      എങ്കിൽ (gnu_extensions)
        {

          /* Simulate \w+.  */

          വേണ്ടി (പൂർണ്ണസംഖ്യ character = 0; character < CHAR_SET_SIZE; character++)
            word_fastmap[character] = !! isalpha (character);
        }
      അല്ലെങ്കിൽ
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

സ്ഥിതം ശൂന്യം
swallow_file_in_memory (അക്ഷരം സ്ഥിരം *file_name, BLOCK *block)
{
  size_t used_length;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  എങ്കിൽ (using_stdin)
    block->start = fread_file (stdin, 0, &used_length);
  അല്ലെങ്കിൽ
    block->start = read_file (file_name, 0, &used_length);

  എങ്കിൽ (!block->start)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  എങ്കിൽ (using_stdin)
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

സ്ഥിതം പൂർണ്ണസംഖ്യ
compare_വാക്കുകൾ (സ്ഥിരം ശൂന്യം *void_first, സ്ഥിരം ശൂന്യം *void_second)
{
  WORD സ്ഥിരം *first = void_first;
  WORD സ്ഥിരം *second = void_second;
  idx_t length = MIN (first->size, second->size);

  എങ്കിൽ (ignore_case)
    {
      വേണ്ടി (idx_t counter = 0; counter < length; counter++)
        {
          പൂർണ്ണസംഖ്യ മൂല്യം = (folded_chars[to_uchar (first->start[counter])]
                       - folded_chars[to_uchar (second->start[counter])]);
          എങ്കിൽ (മൂല്യം != 0)
            തിരികെ മൂല്യം;
        }
    }
  അല്ലെങ്കിൽ
    {
      വേണ്ടി (idx_t counter = 0; counter < length; counter++)
        {
          പൂർണ്ണസംഖ്യ മൂല്യം = (to_uchar (first->start[counter])
                       - to_uchar (second->start[counter]));
          എങ്കിൽ (മൂല്യം != 0)
            തിരികെ മൂല്യം;
        }
    }

  തിരികെ _GL_CMP (first->size, second->size);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

സ്ഥിതം പൂർണ്ണസംഖ്യ
compare_occurs (സ്ഥിരം ശൂന്യം *void_first, സ്ഥിരം ശൂന്യം *void_second)
{
#define first ((സ്ഥിരം OCCURS *) void_first)
#define second ((സ്ഥിരം OCCURS *) void_second)
  പൂർണ്ണസംഖ്യ മൂല്യം;

  മൂല്യം = compare_വാക്കുകൾ (&first->താക്കോൽ, &second->താക്കോൽ);
  തിരികെ (മൂല്യം ? മൂല്യം
          : _GL_CMP (first->താക്കോൽ.start, second->താക്കോൽ.start));
#undef first
#undef second
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_PURE
സ്ഥിതം bool
തിരയൽ_table (WORD *word, WORD_TABLE *table)
{
  idx_t lo = 0;
  idx_t hi = table->length;
  ആകുമ്പോൾ (lo < hi)
    {
      idx_t നടുവ് = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      പൂർണ്ണസംഖ്യ മൂല്യം = compare_വാക്കുകൾ (word, table->start + നടുവ്);
      എങ്കിൽ (മൂല്യം < 0)
        hi = നടുവ്;
      അല്ലെങ്കിൽ എങ്കിൽ (മൂല്യം > 0)
        lo = നടുവ് + 1;
      അല്ലെങ്കിൽ
        തിരികെ true;
    }
  തിരികെ false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
sort_found_occurs (ശൂന്യം)
{

  /* Only one language for the time being.  */
  എങ്കിൽ (സംഖ്യ_of_occurs[0])
    qsort (occurs_table[0], സംഖ്യ_of_occurs[0], വലുപ്പം **occurs_table,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
digest_break_file (അക്ഷരം സ്ഥിരം *file_name)
{
  BLOCK file_contents;		/* to receive a copy of the file */

  swallow_file_in_memory (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (word_fastmap, 1, CHAR_SET_SIZE);
  വേണ്ടി (അക്ഷരം *cursor = file_contents.start; cursor < file_contents.end;
       cursor++)
    word_fastmap[to_uchar (*cursor)] = 0;

  എങ്കിൽ (!gnu_extensions)
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

  സ്വതന്ത്രം (file_contents.start);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
digest_word_file (അക്ഷരം സ്ഥിരം *file_name, WORD_TABLE *table)
{
  BLOCK file_contents;		/* to receive a copy of the file */
  അക്ഷരം *cursor;			/* cursor in file copy */
  അക്ഷരം *word_start;		/* start of the current word */

  swallow_file_in_memory (file_name, &file_contents);

  table->start = ശൂന്യ;
  table->alloc = 0;
  table->length = 0;

  /* Read the whole file.  */

  cursor = file_contents.start;
  ആകുമ്പോൾ (cursor < file_contents.end)
    {

      /* Read one line, and save the word in contains.  */

      word_start = cursor;
      ആകുമ്പോൾ (cursor < file_contents.end && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      എങ്കിൽ (cursor > word_start)
        {
          എങ്കിൽ (table->length == table->alloc)
            table->start = xpalloc (table->start, &table->alloc, 1, -1,
                                    വലുപ്പം *table->start);
          idx_t സ്ഥിരം table_idx = table->length++;
          table->start[table_idx].start = word_start;
          table->start[table_idx].size = cursor - word_start;
        }

      /* This test allows for an incomplete line at end of file.  */

      എങ്കിൽ (cursor < file_contents.end)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (table->start, table->length, വലുപ്പം table->start[0], compare_വാക്കുകൾ);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
find_occurs_in_text (പൂർണ്ണസംഖ്യ file_index)
{
  അക്ഷരം *scan;			/* for scanning the source text */
  അക്ഷരം *വരി_start;		/* start of the current input line */
  അക്ഷരം *വരി_scan;		/* newlines scanned until this point */
  idx_t reference_length;	/* length of reference in input mode */
  WORD possible_താക്കോൽ;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  അക്ഷരം *context_start;		/* start of left context */
  അക്ഷരം *context_end;		/* end of right context */
  അക്ഷരം *word_start;		/* start of word */
  അക്ഷരം *word_end;		/* end of word */
  അക്ഷരം *next_context_start;	/* next start of left context */

  സ്ഥിരം BLOCK *text_buffer = &text_buffers[file_index];

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

  വരി_start = text_buffer->start;
  വരി_scan = വരി_start;
  എങ്കിൽ (നിവേശം_reference)
    {
      SKIP_NON_WHITE (വരി_scan, text_buffer->end);
      reference_length = വരി_scan - വരി_start;
      SKIP_WHITE (വരി_scan, text_buffer->end);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  വേണ്ടി (അക്ഷരം *cursor = text_buffer->start;
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
      എങ്കിൽ (context_regex.string)
        തിരഞ്ഞെടുപ്പ് (re_തിരയൽ (&context_regex.pattern, cursor,
                           text_buffer->end - cursor,
                           0, text_buffer->end - cursor, &context_regs))
          {
          അവസ്ഥ -2:
            matcher_error ();

          അവസ്ഥ -1:
            നിർത്തുക;

          അവസ്ഥ 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          സ്ഥിരസ്ഥിതി:
            next_context_start = cursor + context_regs.end[0];
            നിർത്തുക;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_end = next_context_start;
      SKIP_WHITE_BACKWARDS (context_end, context_start);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      ആകുമ്പോൾ (true)
        {
          എങ്കിൽ (word_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_തിരയൽ (&word_regex.pattern, cursor,
                                      context_end - cursor,
                                      0, context_end - cursor, &word_regs);
              എങ്കിൽ (r == -2)
                matcher_error ();
              എങ്കിൽ (r == -1)
                നിർത്തുക;
              word_start = cursor + word_regs.start[0];
              word_end = cursor + word_regs.end[0];
            }
          അല്ലെങ്കിൽ

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              scan = cursor;
              ആകുമ്പോൾ (scan < context_end
                     && !word_fastmap[to_uchar (*scan)])
                scan++;

              എങ്കിൽ (scan == context_end)
                നിർത്തുക;

              word_start = scan;

              ആകുമ്പോൾ (scan < context_end
                     && word_fastmap[to_uchar (*scan)])
                scan++;

              word_end = scan;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = word_start;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          എങ്കിൽ (word_end == word_start)
            {
              cursor++;
              തുടരുക;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_താക്കോൽ.start = cursor;
          possible_താക്കോൽ.size = word_end - word_start;
          cursor += possible_താക്കോൽ.size;

          എങ്കിൽ (possible_താക്കോൽ.size > maximum_word_length)
            maximum_word_length = possible_താക്കോൽ.size;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          എങ്കിൽ (നിവേശം_reference)
            {
              ആകുമ്പോൾ (വരി_scan < possible_താക്കോൽ.start)
                എങ്കിൽ (*വരി_scan == '\n')
                  {
                    total_വരി_എണ്ണം++;
                    വരി_scan++;
                    വരി_start = വരി_scan;
                    SKIP_NON_WHITE (വരി_scan, text_buffer->end);
                    reference_length = വരി_scan - വരി_start;
                  }
                അല്ലെങ്കിൽ
                  വരി_scan++;
              എങ്കിൽ (വരി_scan > possible_താക്കോൽ.start)
                തുടരുക;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          എങ്കിൽ (ignore_file && തിരയൽ_table (&possible_താക്കോൽ, &ignore_table))
            തുടരുക;
          എങ്കിൽ (only_file && !തിരയൽ_table (&possible_താക്കോൽ, &only_table))
            തുടരുക;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          എങ്കിൽ (സംഖ്യ_of_occurs[0] == occurs_alloc[0])
            occurs_table[0] = xpalloc (occurs_table[0], &occurs_alloc[0],
                                       1, -1, വലുപ്പം *occurs_table[0]);
          occurs_cursor = occurs_table[0] + സംഖ്യ_of_occurs[0];

          /* Define the reference field, if any.  */

          എങ്കിൽ (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              ആകുമ്പോൾ (വരി_scan < possible_താക്കോൽ.start)
                എങ്കിൽ (*വരി_scan == '\n')
                  {
                    total_വരി_എണ്ണം++;
                    വരി_scan++;
                    വരി_start = വരി_scan;
                    SKIP_NON_WHITE (വരി_scan, text_buffer->end);
                  }
                അല്ലെങ്കിൽ
                  വരി_scan++;

              occurs_cursor->reference = total_വരി_എണ്ണം;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (നിവേശം_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = വരി_start - possible_താക്കോൽ.start;
              എങ്കിൽ (reference_length > reference_max_width)
                reference_max_width = reference_length;
            }

          /* Exclude the reference from the context in simple cases.  */

          എങ്കിൽ (നിവേശം_reference && വരി_start == context_start)
            {
              SKIP_NON_WHITE (context_start, context_end);
              SKIP_WHITE (context_start, context_end);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->താക്കോൽ = possible_താക്കോൽ;
          occurs_cursor->ഇടത് = context_start - possible_താക്കോൽ.start;
          occurs_cursor->വലത് = context_end - possible_താക്കോൽ.start;
          occurs_cursor->file_index = file_index;

          സംഖ്യ_of_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

സ്ഥിതം ശൂന്യം
print_spaces (ptrdiff_t സംഖ്യ)
{
  വേണ്ടി (ptrdiff_t counter = സംഖ്യ; counter > 0; counter--)
    putchar (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

സ്ഥിതം ശൂന്യം
print_field (BLOCK field)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  വേണ്ടി (അക്ഷരം *cursor = field.start; cursor < field.end; cursor++)
    {
      unsigned അക്ഷരം character = *cursor;
      എങ്കിൽ (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          തിരഞ്ഞെടുപ്പ് (character)
            {
            അവസ്ഥ '"':
              /* In roff output format, double any quote.  */
              putchar ('"');
              putchar ('"');
              നിർത്തുക;

            അവസ്ഥ '$':
            അവസ്ഥ '%':
            അവസ്ഥ '&':
            അവസ്ഥ '#':
            അവസ്ഥ '_':
              /* In TeX output format, precede these with a backslash.  */
              putchar ('\\');
              putchar (character);
              നിർത്തുക;

            അവസ്ഥ '{':
            അവസ്ഥ '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              printf ("$\\%c$", character);
              നിർത്തുക;

            അവസ്ഥ '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              നിർത്തുക;

            സ്ഥിരസ്ഥിതി:
              /* Any other flagged character produces a single space.  */
              putchar (' ');
            }
        }
      അല്ലെങ്കിൽ
        putchar (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
fix_output_parameters (ശൂന്യം)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  എങ്കിൽ (auto_reference)
    {
      reference_max_width = 0;
      വേണ്ടി (പൂർണ്ണസംഖ്യ file_index = 0; file_index < സംഖ്യ_നിവേശം_files; file_index++)
        {
          intmax_t വരി_ordinal = file_വരി_എണ്ണം[file_index] + 1;
          എങ്കിൽ (file_index > 0)
            വരി_ordinal -= file_വരി_എണ്ണം[file_index - 1];
          അക്ഷരം ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_width = sprintf (ordinal_string, "%jd", വരി_ordinal);
          എങ്കിൽ (നിവേശം_file_name[file_index])
            reference_width += strlen (നിവേശം_file_name[file_index]);
          എങ്കിൽ (reference_width > reference_max_width)
            reference_max_width = reference_width;
        }
      reference_max_width++;
      reference.start = xmalloc (reference_max_width + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  എങ്കിൽ ((auto_reference || നിവേശം_reference) && !വലത്_reference)
    വരി_width = MAX (0, വരി_width - (reference_max_width + gap_size));

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

  half_വരി_width = വരി_width >> 1;
  before_max_width = half_വരി_width - gap_size;
  keyafter_max_width = half_വരി_width;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  എങ്കിൽ (truncation_string && *truncation_string)
    truncation_string_length = strlen (truncation_string);
  അല്ലെങ്കിൽ
    truncation_string = ശൂന്യ;

  എങ്കിൽ (gnu_extensions)
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
  അല്ലെങ്കിൽ
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_width -= 2 * truncation_string_length + 1;
    }

  എങ്കിൽ (before_max_width < 0)
    before_max_width = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  വേണ്ടി (പൂർണ്ണസംഖ്യ character = 0; character < CHAR_SET_SIZE; character++)
    edited_flag[character] = !! isspace (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  തിരഞ്ഞെടുപ്പ് (output_format)
    {
    അവസ്ഥ UNKNOWN_FORMAT:
      /* Should never happen.  */

    അവസ്ഥ DUMB_FORMAT:
      നിർത്തുക;

    അവസ്ഥ ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      നിർത്തുക;

    അവസ്ഥ TEX_FORMAT:

      /* Various characters need special processing.  */

      വേണ്ടി (അക്ഷരം സ്ഥിരം *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[to_uchar (*cursor)] = 1;

      നിർത്തുക;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t tail_max_width;	/* allowable width of tail field */
  ptrdiff_t head_max_width;	/* allowable width of head field */
  അക്ഷരം *cursor;			/* running cursor in source text */
  അക്ഷരം *ഇടത്_context_start;	/* start of left context */
  അക്ഷരം *വലത്_context_end;	/* end of right context */
  അക്ഷരം *ഇടത്_field_start;	/* conservative start for 'head'/'before' */
  അക്ഷരം സ്ഥിരം *file_name;	/* file name for reference */
  intmax_t വരി_ordinal;	/* line ordinal for reference */
  അക്ഷരം സ്ഥിരം *buffer_start;	/* start of buffered file for this occurs */
  അക്ഷരം സ്ഥിരം *buffer_end;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.start = occurs->താക്കോൽ.start;
  keyafter.end = keyafter.start + occurs->താക്കോൽ.size;
  ഇടത്_context_start = keyafter.start + occurs->ഇടത്;
  വലത്_context_end = keyafter.start + occurs->വലത്;

  buffer_start = text_buffers[occurs->file_index].start;
  buffer_end = text_buffers[occurs->file_index].end;

  cursor = keyafter.end;
  ആകുമ്പോൾ (cursor < വലത്_context_end
         && cursor <= keyafter.start + keyafter_max_width)
    {
      keyafter.end = cursor;
      SKIP_SOMETHING (cursor, വലത്_context_end);
    }
  എങ്കിൽ (cursor <= keyafter.start + keyafter_max_width)
    keyafter.end = cursor;

  keyafter_truncation = truncation_string && keyafter.end < വലത്_context_end;

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

  എങ്കിൽ (-occurs->ഇടത് > half_വരി_width + maximum_word_length)
    {
      ഇടത്_field_start
        = keyafter.start - (half_വരി_width + maximum_word_length);
      SKIP_SOMETHING (ഇടത്_field_start, keyafter.start);
    }
  അല്ലെങ്കിൽ
    ഇടത്_field_start = keyafter.start + occurs->ഇടത്;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.start = ഇടത്_field_start;
  before.end = keyafter.start;
  SKIP_WHITE_BACKWARDS (before.end, before.start);

  ആകുമ്പോൾ (before.start + before_max_width < before.end)
    SKIP_SOMETHING (before.start, before.end);

  എങ്കിൽ (truncation_string)
    {
      cursor = before.start;
      SKIP_WHITE_BACKWARDS (cursor, buffer_start);
      before_truncation = cursor > ഇടത്_context_start;
    }
  അല്ലെങ്കിൽ
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

  എങ്കിൽ (tail_max_width > 0)
    {
      tail.start = keyafter.end;
      SKIP_WHITE (tail.start, buffer_end);

      tail.end = tail.start;
      cursor = tail.end;
      ആകുമ്പോൾ (cursor < വലത്_context_end
             && cursor < tail.start + tail_max_width)
        {
          tail.end = cursor;
          SKIP_SOMETHING (cursor, വലത്_context_end);
        }

      എങ്കിൽ (cursor < tail.start + tail_max_width)
        tail.end = cursor;

      എങ്കിൽ (tail.end > tail.start)
        {
          keyafter_truncation = false;
          tail_truncation = truncation_string && tail.end < വലത്_context_end;
        }
      അല്ലെങ്കിൽ
        tail_truncation = false;

      SKIP_WHITE_BACKWARDS (tail.end, tail.start);
    }
  അല്ലെങ്കിൽ
    {

      /* No place left for a tail field.  */

      tail.start = ശൂന്യ;
      tail.end = ശൂന്യ;
      tail_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  head_max_width
    = keyafter_max_width - (keyafter.end - keyafter.start) - gap_size;

  എങ്കിൽ (head_max_width > 0)
    {
      head.end = before.start;
      SKIP_WHITE_BACKWARDS (head.end, buffer_start);

      head.start = ഇടത്_field_start;
      ആകുമ്പോൾ (head.start + head_max_width < head.end)
        SKIP_SOMETHING (head.start, head.end);

      എങ്കിൽ (head.end > head.start)
        {
          before_truncation = false;
          head_truncation = (truncation_string
                             && head.start > ഇടത്_context_start);
        }
      അല്ലെങ്കിൽ
        head_truncation = false;

      SKIP_WHITE (head.start, head.end);
    }
  അല്ലെങ്കിൽ
    {

      /* No place left for a head field.  */

      head.start = ശൂന്യ;
      head.end = ശൂന്യ;
      head_truncation = false;
    }

  എങ്കിൽ (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = നിവേശം_file_name[occurs->file_index];
      എങ്കിൽ (!file_name)
        file_name = "";

      വരി_ordinal = occurs->reference + 1;
      എങ്കിൽ (occurs->file_index > 0)
        വരി_ordinal -= file_വരി_എണ്ണം[occurs->file_index - 1];

      അക്ഷരം *file_end = stpcpy (reference.start, file_name);
      reference.end = file_end + sprintf (file_end, ":%jd", വരി_ordinal);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (നിവേശം_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.start = keyafter.start + occurs->reference;
      reference.end = reference.start;
      SKIP_NON_WHITE (reference.end, വലത്_context_end);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
output_one_roff_വരി (ശൂന്യം)
{
  /* Output the 'tail' field.  */

  printf (".%s \"", macro_name);
  print_field (tail);
  എങ്കിൽ (tail_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  എങ്കിൽ (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);
  putchar ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_field (keyafter);
  എങ്കിൽ (keyafter_truncation)
    fputs (truncation_string, stdout);
  putchar ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  എങ്കിൽ (head_truncation)
    fputs (truncation_string, stdout);
  print_field (head);
  putchar ('"');

  /* Conditionally output the 'reference' field.  */

  എങ്കിൽ (auto_reference || നിവേശം_reference)
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

സ്ഥിതം ശൂന്യം
output_one_tex_വരി (ശൂന്യം)
{
  BLOCK താക്കോൽ;			/* key field, isolated */
  BLOCK after;			/* after field, isolated */
  അക്ഷരം *cursor;			/* running cursor in source text */

  printf ("\\%s ", macro_name);
  putchar ('{');
  print_field (tail);
  fputs ("}{", stdout);
  print_field (before);
  fputs ("}{", stdout);
  താക്കോൽ.start = keyafter.start;
  after.end = keyafter.end;
  cursor = keyafter.start;
  SKIP_SOMETHING (cursor, keyafter.end);
  താക്കോൽ.end = cursor;
  after.start = cursor;
  print_field (താക്കോൽ);
  fputs ("}{", stdout);
  print_field (after);
  fputs ("}{", stdout);
  print_field (head);
  putchar ('}');
  എങ്കിൽ (auto_reference || നിവേശം_reference)
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

സ്ഥിതം ശൂന്യം
output_one_dumb_വരി (ശൂന്യം)
{
  എങ്കിൽ (!വലത്_reference)
    {
      എങ്കിൽ (auto_reference)
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
      അല്ലെങ്കിൽ
        {

          /* Output the 'reference' field and its following gap.  */

          print_field (reference);
          print_spaces (reference_max_width
                        + gap_size
                        - (reference.end - reference.start));
        }
    }

  എങ്കിൽ (tail.start < tail.end)
    {
      /* Output the 'tail' field.  */

      print_field (tail);
      എങ്കിൽ (tail_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_വരി_width - gap_size
                    - (before.end - before.start)
                    - (before_truncation ? truncation_string_length : 0)
                    - (tail.end - tail.start)
                    - (tail_truncation ? truncation_string_length : 0));
    }
  അല്ലെങ്കിൽ
    print_spaces (half_വരി_width - gap_size
                  - (before.end - before.start)
                  - (before_truncation ? truncation_string_length : 0));

  /* Output the 'before' field.  */

  എങ്കിൽ (before_truncation)
    fputs (truncation_string, stdout);
  print_field (before);

  print_spaces (gap_size);

  /* Output the 'keyafter' field.  */

  print_field (keyafter);
  എങ്കിൽ (keyafter_truncation)
    fputs (truncation_string, stdout);

  എങ്കിൽ (head.start < head.end)
    {
      /* Output the 'head' field.  */

      print_spaces (half_വരി_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0)
                    - (head.end - head.start)
                    - (head_truncation ? truncation_string_length : 0));
      എങ്കിൽ (head_truncation)
        fputs (truncation_string, stdout);
      print_field (head);
    }
  അല്ലെങ്കിൽ

    എങ്കിൽ ((auto_reference || നിവേശം_reference) && വലത്_reference)
      print_spaces (half_വരി_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_length : 0));

  എങ്കിൽ ((auto_reference || നിവേശം_reference) && വലത്_reference)
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

സ്ഥിതം ശൂന്യം
generate_all_output (ശൂന്യം)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  tail.start = ശൂന്യ;
  tail.end = ശൂന്യ;
  tail_truncation = false;

  head.start = ശൂന്യ;
  head.end = ശൂന്യ;
  head_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_table[0];

  വേണ്ടി (idx_t occurs_index = 0; occurs_index < സംഖ്യ_of_occurs[0];
       occurs_index++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      തിരഞ്ഞെടുപ്പ് (output_format)
        {
        അവസ്ഥ UNKNOWN_FORMAT:
          /* Should never happen.  */

        അവസ്ഥ DUMB_FORMAT:
          output_one_dumb_വരി ();
          നിർത്തുക;

        അവസ്ഥ ROFF_FORMAT:
          output_one_roff_വരി ();
          നിർത്തുക;

        അവസ്ഥ TEX_FORMAT:
          output_one_tex_വരി ();
          നിർത്തുക;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [നിവേശം_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [നിവേശം_2 [OUTPUT]]\n"),
              program_name, program_name);
      fputs (_("\
Output a permuted index, including context, of the വാക്കുകൾ in the നിവേശം files.\n\
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
  -F, --flag-truncation=STRING   use STRING വേണ്ടി flagging വരി truncations.\n\
                                 The സ്ഥിരസ്ഥിതി is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name to use instead of 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --വലത്-side-refs          put references at വലത്, not counted in -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   വേണ്ടി end of വരികൾ or end of sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --word-regexp=REGEXP       use REGEXP to match each keyword\n\
"));
      oputs (_("\
  -b, --നിർത്തുക-file=FILE          word നിർത്തുക characters in this FILE\n\
"));
      oputs (_("\
  -f, --ignore-അവസ്ഥ              fold lower അവസ്ഥ to upper അവസ്ഥ വേണ്ടി sorting\n\
"));
      oputs (_("\
  -g, --gap-size=സംഖ്യ_2          gap size in columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         പഠ ignore word list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           പഠ only word list from this FILE\n\
"));
      oputs (_("\
  -r, --references               first field of each വരി is a reference\n\
"));
      oputs (_("\
  -t, --typeset-mode             change the സ്ഥിരസ്ഥിതി width from 72 to 100\n\
"));
      oputs (_("\
  -w, --width=സംഖ്യ_2             output width in columns, reference excluded\n\
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
എണ്ണൽ
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"auto-reference", no_argument, ശൂന്യ, 'A'},
  {"break-file", required_argument, ശൂന്യ, 'b'},
  {"flag-truncation", required_argument, ശൂന്യ, 'F'},
  {"ignore-case", no_argument, ശൂന്യ, 'f'},
  {"gap-size", required_argument, ശൂന്യ, 'g'},
  {"ignore-file", required_argument, ശൂന്യ, 'i'},
  {"macro-name", required_argument, ശൂന്യ, 'M'},
  {"only-file", required_argument, ശൂന്യ, 'o'},
  {"references", no_argument, ശൂന്യ, 'r'},
  {"right-side-refs", no_argument, ശൂന്യ, 'R'},
  {"format", required_argument, ശൂന്യ, FORMAT_OPTION},
  {"sentence-regexp", required_argument, ശൂന്യ, 'S'},
  {"traditional", no_argument, ശൂന്യ, 'G'},
  {"typeset-mode", no_argument, ശൂന്യ, 't'},
  {"width", required_argument, ശൂന്യ, 'w'},
  {"word-regexp", required_argument, ശൂന്യ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0},
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം format_args[] =
{
  "roff", "tex", ശൂന്യ
};

സ്ഥിതം എണ്ണൽ Format സ്ഥിരം format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ optchar;			/* argument character */
  പൂർണ്ണസംഖ്യ file_index;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, ശൂന്യ),
         optchar != EOF)
    {
      തിരഞ്ഞെടുപ്പ് (optchar)
        {
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);

        അവസ്ഥ 'G':
          gnu_extensions = false;
          നിർത്തുക;

        അവസ്ഥ 'b':
          break_file = optarg;
          നിർത്തുക;

        അവസ്ഥ 'f':
          ignore_case = true;
          നിർത്തുക;

        അവസ്ഥ 'g':
          {
            intmax_t tmp;
            എങ്കിൽ (! (xstrtoimax (optarg, ശൂന്യ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_size = tmp;
            നിർത്തുക;
          }

        അവസ്ഥ 'i':
          ignore_file = optarg;
          നിർത്തുക;

        അവസ്ഥ 'o':
          only_file = optarg;
          നിർത്തുക;

        അവസ്ഥ 'r':
          നിവേശം_reference = true;
          നിർത്തുക;

        അവസ്ഥ 't':
          എങ്കിൽ (വരി_width < 0)
            വരി_width = 100;
          നിർത്തുക;

        അവസ്ഥ 'w':
          {
            intmax_t tmp;
            എങ്കിൽ (! (xstrtoimax (optarg, ശൂന്യ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            വരി_width = tmp;
            നിർത്തുക;
          }

        അവസ്ഥ 'A':
          auto_reference = true;
          നിർത്തുക;

        അവസ്ഥ 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          നിർത്തുക;

        അവസ്ഥ 'M':
          macro_name = optarg;
          നിർത്തുക;

        അവസ്ഥ 'O':
          output_format = ROFF_FORMAT;
          നിർത്തുക;

        അവസ്ഥ 'R':
          വലത്_reference = true;
          നിർത്തുക;

        അവസ്ഥ 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          നിർത്തുക;

        അവസ്ഥ 'T':
          output_format = TEX_FORMAT;
          നിർത്തുക;

        അവസ്ഥ 'W':
          word_regex.string = optarg;
          unescape_string (optarg);
          എങ്കിൽ (!*word_regex.string)
            word_regex.string = ശൂന്യ;
          നിർത്തുക;

        അവസ്ഥ FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_args, format_vals);
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        }
    }

  എങ്കിൽ (വരി_width < 0)
    വരി_width = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  എങ്കിൽ (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      നിവേശം_file_name = xmalloc (വലുപ്പം *നിവേശം_file_name);
      file_വരി_എണ്ണം = xmalloc (വലുപ്പം *file_വരി_എണ്ണം);
      text_buffers =    xmalloc (വലുപ്പം *text_buffers);
      സംഖ്യ_നിവേശം_files = 1;
      നിവേശം_file_name[0] = ശൂന്യ;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (gnu_extensions)
    {
      സംഖ്യ_നിവേശം_files = argc - optind;
      നിവേശം_file_name = xnmalloc (സംഖ്യ_നിവേശം_files, വലുപ്പം *നിവേശം_file_name);
      file_വരി_എണ്ണം = xnmalloc (സംഖ്യ_നിവേശം_files, വലുപ്പം *file_വരി_എണ്ണം);
      text_buffers    = xnmalloc (സംഖ്യ_നിവേശം_files, വലുപ്പം *text_buffers);

      വേണ്ടി (file_index = 0; file_index < സംഖ്യ_നിവേശം_files; file_index++)
        {
          എങ്കിൽ (!*argv[optind] || streq (argv[optind], "-"))
            നിവേശം_file_name[file_index] = ശൂന്യ;
          അല്ലെങ്കിൽ
            നിവേശം_file_name[file_index] = argv[optind];
          optind++;
        }
    }
  അല്ലെങ്കിൽ
    {

      /* There is one necessary input file.  */

      സംഖ്യ_നിവേശം_files = 1;
      നിവേശം_file_name = xmalloc (വലുപ്പം *നിവേശം_file_name);
      file_വരി_എണ്ണം = xmalloc (വലുപ്പം *file_വരി_എണ്ണം);
      text_buffers    = xmalloc (വലുപ്പം *text_buffers);
      എങ്കിൽ (!*argv[optind] || streq (argv[optind], "-"))
        നിവേശം_file_name[0] = ശൂന്യ;
      അല്ലെങ്കിൽ
        നിവേശം_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      എങ്കിൽ (optind < argc)
        {
          എങ്കിൽ (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      എങ്കിൽ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  എങ്കിൽ (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  എങ്കിൽ (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  എങ്കിൽ (ignore_file)
    {
      digest_word_file (ignore_file, &ignore_table);
      എങ്കിൽ (ignore_table.length == 0)
        ignore_file = ശൂന്യ;
    }

  എങ്കിൽ (only_file)
    {
      digest_word_file (only_file, &only_table);
      എങ്കിൽ (only_table.length == 0)
        only_file = ശൂന്യ;
    }

  /* Prepare to study all the input files.  */

  സംഖ്യ_of_occurs[0] = 0;
  total_വരി_എണ്ണം = 0;
  maximum_word_length = 0;
  reference_max_width = 0;

  വേണ്ടി (file_index = 0; file_index < സംഖ്യ_നിവേശം_files; file_index++)
    {
      BLOCK *text_buffer = text_buffers + file_index;

      /* Read the file contents into memory, then study it.  */

      swallow_file_in_memory (നിവേശം_file_name[file_index], text_buffer);
      find_occurs_in_text (file_index);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      total_വരി_എണ്ണം++;
      file_വരി_എണ്ണം[file_index] = total_വരി_എണ്ണം;
    }

  /* Do the output process phase.  */

  sort_found_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  തിരികെ EXIT_SUCCESS;
}
