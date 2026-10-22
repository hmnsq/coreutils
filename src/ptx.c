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

#समावेश <config.h>

#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश "system.h"
#समावेश <regex.h>
#समावेश "argmatch.h"
#समावेश "c-ctype.h"
#समावेश "fadvise.h"
#समावेश "octhexdigits.h"
#समावेश "quote.h"
#समावेश "read-file.h"
#समावेश "stdio--.h"
#समावेश "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#घोषणा AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#घोषणा CHAR_SET_आकार 256

/* Debugging the memory allocator.  */

#अगर WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#पूर्ण_अगर

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

क्रमागत Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

अटल bool gnu_extensions = true;	/* trigger all GNU extensions */
अटल bool auto_reference = false;	/* refs are 'file_name:line_number:' */
अटल bool निवेश_reference = false;	/* refs at beginning of input lines */
अटल bool दायाँ_reference = false;	/* output refs after right context  */
अटल idx_t पंक्ति_width = -1;		/* output line width in characters */
अटल idx_t gap_आकार = 3;	/* number of spaces between output fields */
अटल अक्षर स्थिर *truncation_string = "/";
                                /* string used to mark line truncations */
अटल अक्षर स्थिर *macro_name = "xx";	/* macro name for roff or TeX output */
अटल क्रमागत Format output_format = UNKNOWN_FORMAT;
                                /* output format */

अटल bool ignore_case = false;	/* fold lower to upper for sorting */
अटल अक्षर स्थिर *break_file = शून्य; /* name of the 'Break chars' file */
अटल अक्षर स्थिर *only_file = शून्य;	/* name of the 'Only words' file */
अटल अक्षर स्थिर *ignore_file = शून्य; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
काष्ठा regex_तथ्य
{
  /* The original regular expression, as a string.  */
  अक्षर स्थिर *string;

  /* The compiled regular expression, and its fastmap.  */
  काष्ठा re_pattern_बफर pattern;
  अक्षर fastmap[अच_अक्षर_उच्च + 1];
};

अटल काष्ठा regex_तथ्य context_regex;	/* end of context */
अटल काष्ठा regex_तथ्य word_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

प्रकार काष्ठा
  {
    अक्षर *start;		/* pointer to beginning of region */
    अक्षर *end;			/* pointer to end + 1 of region */
  }
BLOCK;

प्रकार काष्ठा
  {
    अक्षर *start;		/* pointer to beginning of region */
    idx_t आकार;			/* length of the region */
  }
WORD;

प्रकार काष्ठा
  {
    idx_t alloc;		/* allocated length */
    idx_t लंबाई;		/* number of used entries */
    WORD *start COUNTED_BY (लंबाई);	/* array of WORDs */
  }
WORD_TABLE;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
अटल अचिन्हित अक्षर folded_chars[CHAR_SET_आकार];

/* End of context pattern register indices.  */
अटल काष्ठा re_registers context_regs;

/* Keyword pattern register indices.  */
अटल काष्ठा re_registers word_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
अटल अक्षर word_fastmap[CHAR_SET_आकार];

/* Maximum length of any word read.  */
अटल idx_t maximum_word_लंबाई;

/* Maximum width of any reference used.  */
अटल idx_t reference_max_width;

/* Ignore and Only word tables.  */

अटल WORD_TABLE ignore_table;	/* table of words to ignore */
अटल WORD_TABLE only_table;		/* table of words to select */

/* Source text table, and scanning macros.  */

अटल पूर्णांक संख्या_निवेश_files;	/* number of text input files */
अटल intmax_t योग_पंक्ति_गिनती;	/* total number of lines seen so far */
अटल अक्षर स्थिर **निवेश_file_name;	/* array of text input file names */
अटल intmax_t *file_पंक्ति_गिनती;	/* array of line count values at end */

अटल BLOCK *text_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#घोषणा SKIP_NON_WHITE(cursor, limit) \
  जबतक (cursor < limit && ! है_खाली (to_uchar (*cursor)))		\
    cursor++

#घोषणा SKIP_WHITE(cursor, limit) \
  जबतक (cursor < limit && है_खाली (to_uchar (*cursor)))		\
    cursor++

#घोषणा SKIP_WHITE_BACKWARDS(cursor, start) \
  जबतक (cursor > start && है_खाली (to_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#घोषणा SKIP_SOMETHING(cursor, limit) \
  अगर (word_regex.string)						\
    {									\
      regoff_t गिनती;							\
      गिनती = re_match (&word_regex.pattern, cursor, limit - cursor,	\
                        0, शून्य);					\
      अगर (गिनती == -2)							\
        matcher_error ();						\
      cursor += गिनती <= 0 ? 1 : गिनती;					\
    }									\
  अन्यथा अगर (word_fastmap[to_uchar (*cursor)])				\
    जबतक (cursor < limit && word_fastmap[to_uchar (*cursor)])		\
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

प्रकार काष्ठा
  {
    WORD कुंजी;			/* description of the keyword */
    सूचक_भेद_प्रकार बायाँ;		/* distance to left context start */
    सूचक_भेद_प्रकार दायाँ;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    पूर्णांक file_सूचकांक;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

अटल OCCURS *occurs_table[1];	/* all words retained from the read text */
अटल idx_t occurs_alloc[1];	/* allocated size of occurs_table */
अटल idx_t संख्या_of_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
अटल अक्षर edited_flag[CHAR_SET_आकार];

/* Half of line width, reference excluded.  */
अटल idx_t half_पंक्ति_width;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
अटल सूचक_भेद_प्रकार before_max_width;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
अटल सूचक_भेद_प्रकार keyafter_max_width;

/* Length of string that flags truncation.  */
अटल idx_t truncation_string_लंबाई;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

अटल BLOCK पुच्छ;		/* tail field */
अटल bool पुच्छ_truncation;	/* flag truncation after the tail field */

अटल BLOCK before;		/* before field */
अटल bool before_truncation;	/* flag truncation before the before field */

अटल BLOCK keyafter;		/* keyword-and-after field */
अटल bool keyafter_truncation; /* flag truncation after the keyafter field */

अटल BLOCK शीर्ष;		/* head field */
अटल bool शीर्ष_truncation;	/* flag truncation before the head field */

अटल BLOCK reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

अटल _Noreturn व्योम
matcher_error (व्योम)
{
  error (निकास_त्रुटि, त्रुटि_सं, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

अटल व्योम
unescape_string (अक्षर *string)
{
  अक्षर *cursor;			/* cursor in result */
  पूर्णांक मूल्य;			/* value of \nnn escape */
  पूर्णांक लंबाई;			/* length of \nnn escape */

  cursor = string;

  जबतक (*string)
    {
      अगर (*string == '\\')
        {
          string++;
          चयन (*string)
            {
            हाल 'x':		/* \xhhh escape, 3 chars maximum */
              मूल्य = 0;
              क्रम (लंबाई = 0, string++;
                   लंबाई < 3 && c_isxdigit (*string);
                   लंबाई++, string++)
                मूल्य = मूल्य * 16 + fromhex (*string);
              अगर (लंबाई == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              अन्यथा
                *cursor++ = मूल्य;
              अवरोध;

            हाल '0':		/* \0ooo escape, 3 chars maximum */
              मूल्य = 0;
              क्रम (लंबाई = 0, string++;
                   लंबाई < 3 && isoct (*string);
                   लंबाई++, string++)
                मूल्य = मूल्य * 8 + fromoct (*string);
              *cursor++ = मूल्य;
              अवरोध;

            हाल 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              अवरोध;

            हाल 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              अवरोध;

            हाल 'c':		/* cancel the rest of the output */
              जबतक (*string)
                string++;
              अवरोध;

            हाल 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              अवरोध;

            हाल 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              अवरोध;

            हाल 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              अवरोध;

            हाल 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              अवरोध;

            हाल 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              अवरोध;

            हाल '\0':		/* lone backslash at end of string */
              /* ignore it */
              अवरोध;

            शेष:
              *cursor++ = '\\';
              *cursor++ = *string++;
              अवरोध;
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

अटल व्योम
compile_regex (काष्ठा regex_तथ्य *regex)
{
  काष्ठा re_pattern_बफर *pattern = &regex->pattern;
  अक्षर स्थिर *string = regex->string;
  अक्षर स्थिर *message;

  pattern->बफर = शून्य;
  pattern->allocated = 0;
  pattern->fastmap = regex->fastmap;
  pattern->translate = ignore_case ? folded_chars : शून्य;

  message = re_compile_pattern (string, म_माप (string), pattern);
  अगर (message)
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

अटल व्योम
initialize_regex (व्योम)
{
  /* Initialize the case folding table.  */

  अगर (ignore_case)
    क्रम (पूर्णांक character = 0; character < CHAR_SET_आकार; character++)
      folded_chars[character] = बड़े (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  अगर (context_regex.string)
    {
      अगर (!*context_regex.string)
        context_regex.string = शून्य;
    }
  अन्यथा अगर (gnu_extensions && !निवेश_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  अन्यथा
    context_regex.string = "\n";

  अगर (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  अगर (word_regex.string)
    compile_regex (&word_regex);
  अन्यथा अगर (!break_file)
    {
      अगर (gnu_extensions)
        {

          /* Simulate \w+.  */

          क्रम (पूर्णांक character = 0; character < CHAR_SET_आकार; character++)
            word_fastmap[character] = !! है_अक्षर (character);
        }
      अन्यथा
        {

          /* Simulate [^ \t\n]+.  */

          स_रखो (word_fastmap, 1, CHAR_SET_आकार);
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

अटल व्योम
swallow_file_in_memory (अक्षर स्थिर *file_name, BLOCK *block)
{
  माप_प्रकार used_लंबाई;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  अगर (using_stdin)
    block->start = fread_file (मानक_निवेश, 0, &used_लंबाई);
  अन्यथा
    block->start = read_file (file_name, 0, &used_लंबाई);

  अगर (!block->start)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (using_stdin ? "-" : file_name));

  अगर (using_stdin)
    साफ_त्रुटि (मानक_निवेश);

  block->end = block->start + used_लंबाई;
}

/* Sort and search routines.  */

/*--------------------------------------------------------------------------.
| Compare two words, FIRST and SECOND, and return 0 if they are identical.  |
| Return less than 0 if the first word goes before the second; return	    |
| greater than 0 if the first word goes after the second.		    |
|									    |
| If a word is indeed a prefix of the other, the shorter should go first.   |
`--------------------------------------------------------------------------*/

अटल पूर्णांक
compare_शब्दगण (स्थिर व्योम *void_first, स्थिर व्योम *void_second)
{
  WORD स्थिर *first = void_first;
  WORD स्थिर *second = void_second;
  idx_t लंबाई = MIN (first->आकार, second->आकार);

  अगर (ignore_case)
    {
      क्रम (idx_t गणक = 0; गणक < लंबाई; गणक++)
        {
          पूर्णांक मूल्य = (folded_chars[to_uchar (first->start[गणक])]
                       - folded_chars[to_uchar (second->start[गणक])]);
          अगर (मूल्य != 0)
            वापस मूल्य;
        }
    }
  अन्यथा
    {
      क्रम (idx_t गणक = 0; गणक < लंबाई; गणक++)
        {
          पूर्णांक मूल्य = (to_uchar (first->start[गणक])
                       - to_uchar (second->start[गणक]));
          अगर (मूल्य != 0)
            वापस मूल्य;
        }
    }

  वापस _GL_CMP (first->आकार, second->आकार);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

अटल पूर्णांक
compare_occurs (स्थिर व्योम *void_first, स्थिर व्योम *void_second)
{
#घोषणा first ((स्थिर OCCURS *) void_first)
#घोषणा second ((स्थिर OCCURS *) void_second)
  पूर्णांक मूल्य;

  मूल्य = compare_शब्दगण (&first->कुंजी, &second->कुंजी);
  वापस (मूल्य ? मूल्य
          : _GL_CMP (first->कुंजी.start, second->कुंजी.start));
#अघोषित first
#अघोषित second
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_PURE
अटल bool
खोज_table (WORD *word, WORD_TABLE *table)
{
  idx_t lo = 0;
  idx_t hi = table->लंबाई;
  जबतक (lo < hi)
    {
      idx_t मध्य = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      पूर्णांक मूल्य = compare_शब्दगण (word, table->start + मध्य);
      अगर (मूल्य < 0)
        hi = मध्य;
      अन्यथा अगर (मूल्य > 0)
        lo = मध्य + 1;
      अन्यथा
        वापस true;
    }
  वापस false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

अटल व्योम
sort_found_occurs (व्योम)
{

  /* Only one language for the time being.  */
  अगर (संख्या_of_occurs[0])
    क्विक (occurs_table[0], संख्या_of_occurs[0], माप **occurs_table,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

अटल व्योम
digest_break_file (अक्षर स्थिर *file_name)
{
  BLOCK file_contents;		/* to receive a copy of the file */

  swallow_file_in_memory (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  स_रखो (word_fastmap, 1, CHAR_SET_आकार);
  क्रम (अक्षर *cursor = file_contents.start; cursor < file_contents.end;
       cursor++)
    word_fastmap[to_uchar (*cursor)] = 0;

  अगर (!gnu_extensions)
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

अटल व्योम
digest_word_file (अक्षर स्थिर *file_name, WORD_TABLE *table)
{
  BLOCK file_contents;		/* to receive a copy of the file */
  अक्षर *cursor;			/* cursor in file copy */
  अक्षर *word_start;		/* start of the current word */

  swallow_file_in_memory (file_name, &file_contents);

  table->start = शून्य;
  table->alloc = 0;
  table->लंबाई = 0;

  /* Read the whole file.  */

  cursor = file_contents.start;
  जबतक (cursor < file_contents.end)
    {

      /* Read one line, and save the word in contains.  */

      word_start = cursor;
      जबतक (cursor < file_contents.end && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      अगर (cursor > word_start)
        {
          अगर (table->लंबाई == table->alloc)
            table->start = xpalloc (table->start, &table->alloc, 1, -1,
                                    माप *table->start);
          idx_t स्थिर table_idx = table->लंबाई++;
          table->start[table_idx].start = word_start;
          table->start[table_idx].आकार = cursor - word_start;
        }

      /* This test allows for an incomplete line at end of file.  */

      अगर (cursor < file_contents.end)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  क्विक (table->start, table->लंबाई, माप table->start[0], compare_शब्दगण);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

अटल व्योम
find_occurs_in_text (पूर्णांक file_सूचकांक)
{
  अक्षर *scan;			/* for scanning the source text */
  अक्षर *पंक्ति_start;		/* start of the current input line */
  अक्षर *पंक्ति_scan;		/* newlines scanned until this point */
  idx_t reference_लंबाई;	/* length of reference in input mode */
  WORD possible_कुंजी;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  अक्षर *context_start;		/* start of left context */
  अक्षर *context_end;		/* end of right context */
  अक्षर *word_start;		/* start of word */
  अक्षर *word_end;		/* end of word */
  अक्षर *अग्र_context_start;	/* next start of left context */

  स्थिर BLOCK *text_बफर = &text_buffers[file_सूचकांक];

  /* reference_length is always used within 'if (input_reference)'.
     However, GNU C diagnoses that it may be used uninitialized.  The
     following assignment is merely to shut it up.  */

  reference_लंबाई = 0;

  /* Tracking where lines start is helpful for reference processing.  In
     auto reference mode, this allows counting lines.  In input reference
     mode, this permits finding the beginning of the references.

     The first line begins with the file, skip immediately this very first
     reference in input reference mode, to help further rejection any word
     found inside it.  Also, unconditionally assigning these variable has
     the happy effect of shutting up lint.  */

  पंक्ति_start = text_बफर->start;
  पंक्ति_scan = पंक्ति_start;
  अगर (निवेश_reference)
    {
      SKIP_NON_WHITE (पंक्ति_scan, text_बफर->end);
      reference_लंबाई = पंक्ति_scan - पंक्ति_start;
      SKIP_WHITE (पंक्ति_scan, text_बफर->end);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  क्रम (अक्षर *cursor = text_बफर->start;
       cursor < text_बफर->end;
       cursor = अग्र_context_start)
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

      अग्र_context_start = text_बफर->end;
      अगर (context_regex.string)
        चयन (re_खोज (&context_regex.pattern, cursor,
                           text_बफर->end - cursor,
                           0, text_बफर->end - cursor, &context_regs))
          {
          हाल -2:
            matcher_error ();

          हाल -1:
            अवरोध;

          हाल 0:
            error (निकास_त्रुटि, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          शेष:
            अग्र_context_start = cursor + context_regs.end[0];
            अवरोध;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_end = अग्र_context_start;
      SKIP_WHITE_BACKWARDS (context_end, context_start);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      जबतक (true)
        {
          अगर (word_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_खोज (&word_regex.pattern, cursor,
                                      context_end - cursor,
                                      0, context_end - cursor, &word_regs);
              अगर (r == -2)
                matcher_error ();
              अगर (r == -1)
                अवरोध;
              word_start = cursor + word_regs.start[0];
              word_end = cursor + word_regs.end[0];
            }
          अन्यथा

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              scan = cursor;
              जबतक (scan < context_end
                     && !word_fastmap[to_uchar (*scan)])
                scan++;

              अगर (scan == context_end)
                अवरोध;

              word_start = scan;

              जबतक (scan < context_end
                     && word_fastmap[to_uchar (*scan)])
                scan++;

              word_end = scan;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = word_start;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          अगर (word_end == word_start)
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

          possible_कुंजी.start = cursor;
          possible_कुंजी.आकार = word_end - word_start;
          cursor += possible_कुंजी.आकार;

          अगर (possible_कुंजी.आकार > maximum_word_लंबाई)
            maximum_word_लंबाई = possible_कुंजी.आकार;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          अगर (निवेश_reference)
            {
              जबतक (पंक्ति_scan < possible_कुंजी.start)
                अगर (*पंक्ति_scan == '\n')
                  {
                    योग_पंक्ति_गिनती++;
                    पंक्ति_scan++;
                    पंक्ति_start = पंक्ति_scan;
                    SKIP_NON_WHITE (पंक्ति_scan, text_बफर->end);
                    reference_लंबाई = पंक्ति_scan - पंक्ति_start;
                  }
                अन्यथा
                  पंक्ति_scan++;
              अगर (पंक्ति_scan > possible_कुंजी.start)
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

          अगर (ignore_file && खोज_table (&possible_कुंजी, &ignore_table))
            जारी;
          अगर (only_file && !खोज_table (&possible_कुंजी, &only_table))
            जारी;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          अगर (संख्या_of_occurs[0] == occurs_alloc[0])
            occurs_table[0] = xpalloc (occurs_table[0], &occurs_alloc[0],
                                       1, -1, माप *occurs_table[0]);
          occurs_cursor = occurs_table[0] + संख्या_of_occurs[0];

          /* Define the reference field, if any.  */

          अगर (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              जबतक (पंक्ति_scan < possible_कुंजी.start)
                अगर (*पंक्ति_scan == '\n')
                  {
                    योग_पंक्ति_गिनती++;
                    पंक्ति_scan++;
                    पंक्ति_start = पंक्ति_scan;
                    SKIP_NON_WHITE (पंक्ति_scan, text_बफर->end);
                  }
                अन्यथा
                  पंक्ति_scan++;

              occurs_cursor->reference = योग_पंक्ति_गिनती;
            }
          अन्यथा अगर (निवेश_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = पंक्ति_start - possible_कुंजी.start;
              अगर (reference_लंबाई > reference_max_width)
                reference_max_width = reference_लंबाई;
            }

          /* Exclude the reference from the context in simple cases.  */

          अगर (निवेश_reference && पंक्ति_start == context_start)
            {
              SKIP_NON_WHITE (context_start, context_end);
              SKIP_WHITE (context_start, context_end);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->कुंजी = possible_कुंजी;
          occurs_cursor->बायाँ = context_start - possible_कुंजी.start;
          occurs_cursor->दायाँ = context_end - possible_कुंजी.start;
          occurs_cursor->file_सूचकांक = file_सूचकांक;

          संख्या_of_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

अटल व्योम
print_spaces (सूचक_भेद_प्रकार संख्या)
{
  क्रम (सूचक_भेद_प्रकार गणक = संख्या; गणक > 0; गणक--)
    अक्षर_दो (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

अटल व्योम
print_field (BLOCK field)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  क्रम (अक्षर *cursor = field.start; cursor < field.end; cursor++)
    {
      अचिन्हित अक्षर character = *cursor;
      अगर (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          चयन (character)
            {
            हाल '"':
              /* In roff output format, double any quote.  */
              अक्षर_दो ('"');
              अक्षर_दो ('"');
              अवरोध;

            हाल '$':
            हाल '%':
            हाल '&':
            हाल '#':
            हाल '_':
              /* In TeX output format, precede these with a backslash.  */
              अक्षर_दो ('\\');
              अक्षर_दो (character);
              अवरोध;

            हाल '{':
            हाल '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              म_लिखो ("$\\%c$", character);
              अवरोध;

            हाल '\\':
              /* In TeX output mode, request production of a backslash.  */
              ख_माला_दो ("\\backslash{}", मानक_निकास);
              अवरोध;

            शेष:
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

अटल व्योम
fix_output_parameters (व्योम)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  अगर (auto_reference)
    {
      reference_max_width = 0;
      क्रम (पूर्णांक file_सूचकांक = 0; file_सूचकांक < संख्या_निवेश_files; file_सूचकांक++)
        {
          intmax_t पंक्ति_ordinal = file_पंक्ति_गिनती[file_सूचकांक] + 1;
          अगर (file_सूचकांक > 0)
            पंक्ति_ordinal -= file_पंक्ति_गिनती[file_सूचकांक - 1];
          अक्षर ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_width = प्र_लिखो (ordinal_string, "%jd", पंक्ति_ordinal);
          अगर (निवेश_file_name[file_सूचकांक])
            reference_width += म_माप (निवेश_file_name[file_सूचकांक]);
          अगर (reference_width > reference_max_width)
            reference_max_width = reference_width;
        }
      reference_max_width++;
      reference.start = xmalloc (reference_max_width + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  अगर ((auto_reference || निवेश_reference) && !दायाँ_reference)
    पंक्ति_width = MAX (0, पंक्ति_width - (reference_max_width + gap_आकार));

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

  half_पंक्ति_width = पंक्ति_width >> 1;
  before_max_width = half_पंक्ति_width - gap_आकार;
  keyafter_max_width = half_पंक्ति_width;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  अगर (truncation_string && *truncation_string)
    truncation_string_लंबाई = म_माप (truncation_string);
  अन्यथा
    truncation_string = शून्य;

  अगर (gnu_extensions)
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

      before_max_width -= 2 * truncation_string_लंबाई;
      keyafter_max_width -= 2 * truncation_string_लंबाई;
    }
  अन्यथा
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_width -= 2 * truncation_string_लंबाई + 1;
    }

  अगर (before_max_width < 0)
    before_max_width = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  क्रम (पूर्णांक character = 0; character < CHAR_SET_आकार; character++)
    edited_flag[character] = !! है_खाली (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  चयन (output_format)
    {
    हाल UNKNOWN_FORMAT:
      /* Should never happen.  */

    हाल DUMB_FORMAT:
      अवरोध;

    हाल ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      अवरोध;

    हाल TEX_FORMAT:

      /* Various characters need special processing.  */

      क्रम (अक्षर स्थिर *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[to_uchar (*cursor)] = 1;

      अवरोध;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

अटल व्योम
define_all_fields (OCCURS *occurs)
{
  सूचक_भेद_प्रकार पुच्छ_max_width;	/* allowable width of tail field */
  सूचक_भेद_प्रकार शीर्ष_max_width;	/* allowable width of head field */
  अक्षर *cursor;			/* running cursor in source text */
  अक्षर *बायाँ_context_start;	/* start of left context */
  अक्षर *दायाँ_context_end;	/* end of right context */
  अक्षर *बायाँ_field_start;	/* conservative start for 'head'/'before' */
  अक्षर स्थिर *file_name;	/* file name for reference */
  intmax_t पंक्ति_ordinal;	/* line ordinal for reference */
  अक्षर स्थिर *बफर_start;	/* start of buffered file for this occurs */
  अक्षर स्थिर *बफर_end;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.start = occurs->कुंजी.start;
  keyafter.end = keyafter.start + occurs->कुंजी.आकार;
  बायाँ_context_start = keyafter.start + occurs->बायाँ;
  दायाँ_context_end = keyafter.start + occurs->दायाँ;

  बफर_start = text_buffers[occurs->file_सूचकांक].start;
  बफर_end = text_buffers[occurs->file_सूचकांक].end;

  cursor = keyafter.end;
  जबतक (cursor < दायाँ_context_end
         && cursor <= keyafter.start + keyafter_max_width)
    {
      keyafter.end = cursor;
      SKIP_SOMETHING (cursor, दायाँ_context_end);
    }
  अगर (cursor <= keyafter.start + keyafter_max_width)
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

  अगर (-occurs->बायाँ > half_पंक्ति_width + maximum_word_लंबाई)
    {
      बायाँ_field_start
        = keyafter.start - (half_पंक्ति_width + maximum_word_लंबाई);
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

  जबतक (before.start + before_max_width < before.end)
    SKIP_SOMETHING (before.start, before.end);

  अगर (truncation_string)
    {
      cursor = before.start;
      SKIP_WHITE_BACKWARDS (cursor, बफर_start);
      before_truncation = cursor > बायाँ_context_start;
    }
  अन्यथा
    before_truncation = false;

  SKIP_WHITE (before.start, बफर_end);

  /* The tail could not take more columns than what has been left in the
     left context field, and a gap is mandatory.  It starts after the
     right context, and does not contain prefixed spaces.  It ends at
     the end of line, the end of buffer or when the tail field is full,
     whichever comes first.  It cannot contain only part of a word, and
     has no suffixed spaces.  */

  पुच्छ_max_width
    = before_max_width - (before.end - before.start) - gap_आकार;

  अगर (पुच्छ_max_width > 0)
    {
      पुच्छ.start = keyafter.end;
      SKIP_WHITE (पुच्छ.start, बफर_end);

      पुच्छ.end = पुच्छ.start;
      cursor = पुच्छ.end;
      जबतक (cursor < दायाँ_context_end
             && cursor < पुच्छ.start + पुच्छ_max_width)
        {
          पुच्छ.end = cursor;
          SKIP_SOMETHING (cursor, दायाँ_context_end);
        }

      अगर (cursor < पुच्छ.start + पुच्छ_max_width)
        पुच्छ.end = cursor;

      अगर (पुच्छ.end > पुच्छ.start)
        {
          keyafter_truncation = false;
          पुच्छ_truncation = truncation_string && पुच्छ.end < दायाँ_context_end;
        }
      अन्यथा
        पुच्छ_truncation = false;

      SKIP_WHITE_BACKWARDS (पुच्छ.end, पुच्छ.start);
    }
  अन्यथा
    {

      /* No place left for a tail field.  */

      पुच्छ.start = शून्य;
      पुच्छ.end = शून्य;
      पुच्छ_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  शीर्ष_max_width
    = keyafter_max_width - (keyafter.end - keyafter.start) - gap_आकार;

  अगर (शीर्ष_max_width > 0)
    {
      शीर्ष.end = before.start;
      SKIP_WHITE_BACKWARDS (शीर्ष.end, बफर_start);

      शीर्ष.start = बायाँ_field_start;
      जबतक (शीर्ष.start + शीर्ष_max_width < शीर्ष.end)
        SKIP_SOMETHING (शीर्ष.start, शीर्ष.end);

      अगर (शीर्ष.end > शीर्ष.start)
        {
          before_truncation = false;
          शीर्ष_truncation = (truncation_string
                             && शीर्ष.start > बायाँ_context_start);
        }
      अन्यथा
        शीर्ष_truncation = false;

      SKIP_WHITE (शीर्ष.start, शीर्ष.end);
    }
  अन्यथा
    {

      /* No place left for a head field.  */

      शीर्ष.start = शून्य;
      शीर्ष.end = शून्य;
      शीर्ष_truncation = false;
    }

  अगर (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = निवेश_file_name[occurs->file_सूचकांक];
      अगर (!file_name)
        file_name = "";

      पंक्ति_ordinal = occurs->reference + 1;
      अगर (occurs->file_सूचकांक > 0)
        पंक्ति_ordinal -= file_पंक्ति_गिनती[occurs->file_सूचकांक - 1];

      अक्षर *file_end = stpcpy (reference.start, file_name);
      reference.end = file_end + प्र_लिखो (file_end, ":%jd", पंक्ति_ordinal);
    }
  अन्यथा अगर (निवेश_reference)
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

अटल व्योम
output_one_roff_पंक्ति (व्योम)
{
  /* Output the 'tail' field.  */

  म_लिखो (".%s \"", macro_name);
  print_field (पुच्छ);
  अगर (पुच्छ_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);
  अक्षर_दो ('"');

  /* Output the 'before' field.  */

  ख_माला_दो (" \"", मानक_निकास);
  अगर (before_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);
  print_field (before);
  अक्षर_दो ('"');

  /* Output the 'keyafter' field.  */

  ख_माला_दो (" \"", मानक_निकास);
  print_field (keyafter);
  अगर (keyafter_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);
  अक्षर_दो ('"');

  /* Output the 'head' field.  */

  ख_माला_दो (" \"", मानक_निकास);
  अगर (शीर्ष_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);
  print_field (शीर्ष);
  अक्षर_दो ('"');

  /* Conditionally output the 'reference' field.  */

  अगर (auto_reference || निवेश_reference)
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

अटल व्योम
output_one_tex_पंक्ति (व्योम)
{
  BLOCK कुंजी;			/* key field, isolated */
  BLOCK after;			/* after field, isolated */
  अक्षर *cursor;			/* running cursor in source text */

  म_लिखो ("\\%s ", macro_name);
  अक्षर_दो ('{');
  print_field (पुच्छ);
  ख_माला_दो ("}{", मानक_निकास);
  print_field (before);
  ख_माला_दो ("}{", मानक_निकास);
  कुंजी.start = keyafter.start;
  after.end = keyafter.end;
  cursor = keyafter.start;
  SKIP_SOMETHING (cursor, keyafter.end);
  कुंजी.end = cursor;
  after.start = cursor;
  print_field (कुंजी);
  ख_माला_दो ("}{", मानक_निकास);
  print_field (after);
  ख_माला_दो ("}{", मानक_निकास);
  print_field (शीर्ष);
  अक्षर_दो ('}');
  अगर (auto_reference || निवेश_reference)
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

अटल व्योम
output_one_dumb_पंक्ति (व्योम)
{
  अगर (!दायाँ_reference)
    {
      अगर (auto_reference)
        {

          /* Output the 'reference' field, in such a way that GNU emacs
             next-error will handle it.  The ending colon is taken from the
             gap which follows.  */

          print_field (reference);
          अक्षर_दो (':');
          print_spaces (reference_max_width
                        + gap_आकार
                        - (reference.end - reference.start)
                        - 1);
        }
      अन्यथा
        {

          /* Output the 'reference' field and its following gap.  */

          print_field (reference);
          print_spaces (reference_max_width
                        + gap_आकार
                        - (reference.end - reference.start));
        }
    }

  अगर (पुच्छ.start < पुच्छ.end)
    {
      /* Output the 'tail' field.  */

      print_field (पुच्छ);
      अगर (पुच्छ_truncation)
        ख_माला_दो (truncation_string, मानक_निकास);

      print_spaces (half_पंक्ति_width - gap_आकार
                    - (before.end - before.start)
                    - (before_truncation ? truncation_string_लंबाई : 0)
                    - (पुच्छ.end - पुच्छ.start)
                    - (पुच्छ_truncation ? truncation_string_लंबाई : 0));
    }
  अन्यथा
    print_spaces (half_पंक्ति_width - gap_आकार
                  - (before.end - before.start)
                  - (before_truncation ? truncation_string_लंबाई : 0));

  /* Output the 'before' field.  */

  अगर (before_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);
  print_field (before);

  print_spaces (gap_आकार);

  /* Output the 'keyafter' field.  */

  print_field (keyafter);
  अगर (keyafter_truncation)
    ख_माला_दो (truncation_string, मानक_निकास);

  अगर (शीर्ष.start < शीर्ष.end)
    {
      /* Output the 'head' field.  */

      print_spaces (half_पंक्ति_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_लंबाई : 0)
                    - (शीर्ष.end - शीर्ष.start)
                    - (शीर्ष_truncation ? truncation_string_लंबाई : 0));
      अगर (शीर्ष_truncation)
        ख_माला_दो (truncation_string, मानक_निकास);
      print_field (शीर्ष);
    }
  अन्यथा

    अगर ((auto_reference || निवेश_reference) && दायाँ_reference)
      print_spaces (half_पंक्ति_width
                    - (keyafter.end - keyafter.start)
                    - (keyafter_truncation ? truncation_string_लंबाई : 0));

  अगर ((auto_reference || निवेश_reference) && दायाँ_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_आकार);
      print_field (reference);
    }

  अक्षर_दो ('\n');
}

/*------------------------------------------------------------------------.
| Scan the whole occurs table and, for each entry, output one line in the |
| appropriate format.							  |
`------------------------------------------------------------------------*/

अटल व्योम
generate_all_output (व्योम)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  पुच्छ.start = शून्य;
  पुच्छ.end = शून्य;
  पुच्छ_truncation = false;

  शीर्ष.start = शून्य;
  शीर्ष.end = शून्य;
  शीर्ष_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_table[0];

  क्रम (idx_t occurs_सूचकांक = 0; occurs_सूचकांक < संख्या_of_occurs[0];
       occurs_सूचकांक++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      चयन (output_format)
        {
        हाल UNKNOWN_FORMAT:
          /* Should never happen.  */

        हाल DUMB_FORMAT:
          output_one_dumb_पंक्ति ();
          अवरोध;

        हाल ROFF_FORMAT:
          output_one_roff_पंक्ति ();
          अवरोध;

        हाल TEX_FORMAT:
          output_one_tex_पंक्ति ();
          अवरोध;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [निवेश_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [निवेश_2 [OUTPUT]]\n"),
              program_name, program_name);
      ख_माला_दो (_("\
Output a permuted index, including context, of the शब्दगण in the निवेश files.\n\
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
  -F, --flag-truncation=STRING   use STRING क्रम flagging पंक्ति truncations.\n\
                                 The शेष is '/'\n\
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
  -S, --sentence-regexp=REGEXP   क्रम end of पंक्तियाँ or end of sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --word-regexp=REGEXP       use REGEXP to match each keyword\n\
"));
      oputs (_("\
  -b, --अवरोध-file=खाता          word अवरोध characters in this खाता\n\
"));
      oputs (_("\
  -f, --ignore-हाल              fold lower हाल to upper हाल क्रम sorting\n\
"));
      oputs (_("\
  -g, --gap-आकार=संख्या_2          gap आकार in columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=खाता         पढ़ो ignore word list from खाता\n\
"));
      oputs (_("\
  -o, --only-file=खाता           पढ़ो only word list from this खाता\n\
"));
      oputs (_("\
  -r, --references               first field of each पंक्ति is a reference\n\
"));
      oputs (_("\
  -t, --typeset-mode             change the शेष width from 72 to 100\n\
"));
      oputs (_("\
  -w, --width=संख्या_2             output width in columns, reference excluded\n\
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
क्रमागत
{
  FORMAT_OPTION = अक्षर_उच्च + 1
};

/* Long options equivalences.  */
अटल काष्ठा option स्थिर long_options[] =
{
  {"auto-reference", no_argument, शून्य, 'A'},
  {"break-file", required_argument, शून्य, 'b'},
  {"flag-truncation", required_argument, शून्य, 'F'},
  {"ignore-case", no_argument, शून्य, 'f'},
  {"gap-size", required_argument, शून्य, 'g'},
  {"ignore-file", required_argument, शून्य, 'i'},
  {"macro-name", required_argument, शून्य, 'M'},
  {"only-file", required_argument, शून्य, 'o'},
  {"references", no_argument, शून्य, 'r'},
  {"right-side-refs", no_argument, शून्य, 'R'},
  {"format", required_argument, शून्य, FORMAT_OPTION},
  {"sentence-regexp", required_argument, शून्य, 'S'},
  {"traditional", no_argument, शून्य, 'G'},
  {"typeset-mode", no_argument, शून्य, 't'},
  {"width", required_argument, शून्य, 'w'},
  {"word-regexp", required_argument, शून्य, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0},
};

अटल अक्षर स्थिर *स्थिर format_args[] =
{
  "roff", "tex", शून्य
};

अटल क्रमागत Format स्थिर format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optchar;			/* argument character */
  पूर्णांक file_सूचकांक;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, शून्य),
         optchar != खातापूर्ण)
    {
      चयन (optchar)
        {
        शेष:
          usage (निकास_त्रुटि);

        हाल 'G':
          gnu_extensions = false;
          अवरोध;

        हाल 'b':
          break_file = optarg;
          अवरोध;

        हाल 'f':
          ignore_case = true;
          अवरोध;

        हाल 'g':
          {
            intmax_t tmp;
            अगर (! (xstrtoimax (optarg, शून्य, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (निकास_त्रुटि, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_आकार = tmp;
            अवरोध;
          }

        हाल 'i':
          ignore_file = optarg;
          अवरोध;

        हाल 'o':
          only_file = optarg;
          अवरोध;

        हाल 'r':
          निवेश_reference = true;
          अवरोध;

        हाल 't':
          अगर (पंक्ति_width < 0)
            पंक्ति_width = 100;
          अवरोध;

        हाल 'w':
          {
            intmax_t tmp;
            अगर (! (xstrtoimax (optarg, शून्य, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (निकास_त्रुटि, 0, _("invalid line width: %s"),
                     quote (optarg));
            पंक्ति_width = tmp;
            अवरोध;
          }

        हाल 'A':
          auto_reference = true;
          अवरोध;

        हाल 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          अवरोध;

        हाल 'M':
          macro_name = optarg;
          अवरोध;

        हाल 'O':
          output_format = ROFF_FORMAT;
          अवरोध;

        हाल 'R':
          दायाँ_reference = true;
          अवरोध;

        हाल 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          अवरोध;

        हाल 'T':
          output_format = TEX_FORMAT;
          अवरोध;

        हाल 'W':
          word_regex.string = optarg;
          unescape_string (optarg);
          अगर (!*word_regex.string)
            word_regex.string = शून्य;
          अवरोध;

        हाल FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_args, format_vals);
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        }
    }

  अगर (पंक्ति_width < 0)
    पंक्ति_width = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  अगर (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      निवेश_file_name = xmalloc (माप *निवेश_file_name);
      file_पंक्ति_गिनती = xmalloc (माप *file_पंक्ति_गिनती);
      text_buffers =    xmalloc (माप *text_buffers);
      संख्या_निवेश_files = 1;
      निवेश_file_name[0] = शून्य;
    }
  अन्यथा अगर (gnu_extensions)
    {
      संख्या_निवेश_files = argc - optind;
      निवेश_file_name = xnmalloc (संख्या_निवेश_files, माप *निवेश_file_name);
      file_पंक्ति_गिनती = xnmalloc (संख्या_निवेश_files, माप *file_पंक्ति_गिनती);
      text_buffers    = xnmalloc (संख्या_निवेश_files, माप *text_buffers);

      क्रम (file_सूचकांक = 0; file_सूचकांक < संख्या_निवेश_files; file_सूचकांक++)
        {
          अगर (!*argv[optind] || streq (argv[optind], "-"))
            निवेश_file_name[file_सूचकांक] = शून्य;
          अन्यथा
            निवेश_file_name[file_सूचकांक] = argv[optind];
          optind++;
        }
    }
  अन्यथा
    {

      /* There is one necessary input file.  */

      संख्या_निवेश_files = 1;
      निवेश_file_name = xmalloc (माप *निवेश_file_name);
      file_पंक्ति_गिनती = xmalloc (माप *file_पंक्ति_गिनती);
      text_buffers    = xmalloc (माप *text_buffers);
      अगर (!*argv[optind] || streq (argv[optind], "-"))
        निवेश_file_name[0] = शून्य;
      अन्यथा
        निवेश_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      अगर (optind < argc)
        {
          अगर (! ख_व_खोलो (argv[optind], "w", मानक_निकास))
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      अगर (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (निकास_त्रुटि);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  अगर (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  अगर (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  अगर (ignore_file)
    {
      digest_word_file (ignore_file, &ignore_table);
      अगर (ignore_table.लंबाई == 0)
        ignore_file = शून्य;
    }

  अगर (only_file)
    {
      digest_word_file (only_file, &only_table);
      अगर (only_table.लंबाई == 0)
        only_file = शून्य;
    }

  /* Prepare to study all the input files.  */

  संख्या_of_occurs[0] = 0;
  योग_पंक्ति_गिनती = 0;
  maximum_word_लंबाई = 0;
  reference_max_width = 0;

  क्रम (file_सूचकांक = 0; file_सूचकांक < संख्या_निवेश_files; file_सूचकांक++)
    {
      BLOCK *text_बफर = text_buffers + file_सूचकांक;

      /* Read the file contents into memory, then study it.  */

      swallow_file_in_memory (निवेश_file_name[file_सूचकांक], text_बफर);
      find_occurs_in_text (file_सूचकांक);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      योग_पंक्ति_गिनती++;
      file_पंक्ति_गिनती[file_सूचकांक] = योग_पंक्ति_गिनती;
    }

  /* Do the output process phase.  */

  sort_found_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  वापस निकास_सफल;
}
