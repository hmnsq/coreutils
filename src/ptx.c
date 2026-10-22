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

#అన్తర్భావ <config.h>

#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"
#అన్తర్భావ <regex.h>
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "octhexdigits.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "read-file.h"
#అన్తర్భావ "stdio--.h"
#అన్తర్భావ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#నిర్వచించు AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#నిర్వచించు CHAR_సమితి_కొలత 256

/* Debugging the memory allocator.  */

#ఒకవేళ WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#అయితే_ముగింపు

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

గణన Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

స్థిర bool gnu_extensions = true;	/* trigger all GNU extensions */
స్థిర bool auto_reference = false;	/* refs are 'file_name:line_number:' */
స్థిర bool ఆదానం_reference = false;	/* refs at beginning of input lines */
స్థిర bool కుడి_reference = false;	/* output refs after right context  */
స్థిర idx_t పంక్తి_వెడల్పు = -1;		/* output line width in characters */
స్థిర idx_t gap_కొలత = 3;	/* number of spaces between output fields */
స్థిర అక్షరం స్థిరరాశి *truncation_string = "/";
                                /* string used to mark line truncations */
స్థిర అక్షరం స్థిరరాశి *macro_name = "xx";	/* macro name for roff or TeX output */
స్థిర గణన Format output_format = UNKNOWN_FORMAT;
                                /* output format */

స్థిర bool ignore_case = false;	/* fold lower to upper for sorting */
స్థిర అక్షరం స్థిరరాశి *break_file = ఖాళీ; /* name of the 'Break chars' file */
స్థిర అక్షరం స్థిరరాశి *only_file = ఖాళీ;	/* name of the 'Only words' file */
స్థిర అక్షరం స్థిరరాశి *ignore_file = ఖాళీ; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
నిర్మాణం regex_దత్తాంశం
{
  /* The original regular expression, as a string.  */
  అక్షరం స్థిరరాశి *string;

  /* The compiled regular expression, and its fastmap.  */
  నిర్మాణం re_నమూనా_బఫరు నమూనా_2;
  అక్షరం fastmap[UCHAR_MAX + 1];
};

స్థిర నిర్మాణం regex_దత్తాంశం context_regex;	/* end of context */
స్థిర నిర్మాణం regex_దత్తాంశం మాట_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

రకనిర్వచనం నిర్మాణం
  {
    అక్షరం *ప్రారంభం;		/* pointer to beginning of region */
    అక్షరం *చివర;			/* pointer to end + 1 of region */
  }
ఖండం_2;

రకనిర్వచనం నిర్మాణం
  {
    అక్షరం *ప్రారంభం;		/* pointer to beginning of region */
    idx_t కొలత;			/* length of the region */
  }
మాట;

రకనిర్వచనం నిర్మాణం
  {
    idx_t alloc;		/* allocated length */
    idx_t నిడివి;		/* number of used entries */
    మాట *ప్రారంభం COUNTED_BY (నిడివి);	/* array of WORDs */
  }
మాట_పట్టిక;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
స్థిర గుర్తులేని అక్షరం folded_chars[CHAR_సమితి_కొలత];

/* End of context pattern register indices.  */
స్థిర నిర్మాణం re_registers context_regs;

/* Keyword pattern register indices.  */
స్థిర నిర్మాణం re_registers మాట_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
స్థిర అక్షరం మాట_fastmap[CHAR_సమితి_కొలత];

/* Maximum length of any word read.  */
స్థిర idx_t గరిష్ఠ_మాట_నిడివి;

/* Maximum width of any reference used.  */
స్థిర idx_t reference_max_వెడల్పు;

/* Ignore and Only word tables.  */

స్థిర మాట_పట్టిక ignore_పట్టిక;	/* table of words to ignore */
స్థిర మాట_పట్టిక only_పట్టిక;		/* table of words to select */

/* Source text table, and scanning macros.  */

స్థిర పూర్ణాంకం సంఖ్య_ఆదానం_files;	/* number of text input files */
స్థిర intmax_t మొత్తం_పంక్తి_లెక్క;	/* total number of lines seen so far */
స్థిర అక్షరం స్థిరరాశి **ఆదానం_file_name;	/* array of text input file names */
స్థిర intmax_t *file_పంక్తి_లెక్క;	/* array of line count values at end */

స్థిర ఖండం_2 *పాఠ్యం_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#నిర్వచించు SKIP_NON_WHITE(cursor, హద్దు) \
  అయితే (cursor < హద్దు && ! ఖాళీనా (గమ్యం_uchar (*cursor)))		\
    cursor++

#నిర్వచించు SKIP_WHITE(cursor, హద్దు) \
  అయితే (cursor < హద్దు && ఖాళీనా (గమ్యం_uchar (*cursor)))		\
    cursor++

#నిర్వచించు SKIP_WHITE_BACKWARDS(cursor, ప్రారంభం) \
  అయితే (cursor > ప్రారంభం && ఖాళీనా (గమ్యం_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#నిర్వచించు SKIP_SOMETHING(cursor, హద్దు) \
  ఒకవేళ (మాట_regex.string)						\
    {									\
      regoff_t లెక్క;							\
      లెక్క = re_సరిపోలిక (&మాట_regex.నమూనా_2, cursor, హద్దు - cursor,	\
                        0, ఖాళీ);					\
      ఒకవేళ (లెక్క == -2)							\
        matcher_error ();						\
      cursor += లెక్క <= 0 ? 1 : లెక్క;					\
    }									\
  లేకపోతే ఒకవేళ (మాట_fastmap[గమ్యం_uchar (*cursor)])				\
    అయితే (cursor < హద్దు && మాట_fastmap[గమ్యం_uchar (*cursor)])		\
      cursor++;								\
  లేకపోతే									\
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

రకనిర్వచనం నిర్మాణం
  {
    మాట కీలకం;			/* description of the keyword */
    ptrdiff_t ఎడమ;		/* distance to left context start */
    ptrdiff_t కుడి;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    పూర్ణాంకం file_సూచీ;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

స్థిర OCCURS *occurs_పట్టిక[1];	/* all words retained from the read text */
స్థిర idx_t occurs_alloc[1];	/* allocated size of occurs_table */
స్థిర idx_t సంఖ్య_యొక్క_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
స్థిర అక్షరం edited_flag[CHAR_సమితి_కొలత];

/* Half of line width, reference excluded.  */
స్థిర idx_t half_పంక్తి_వెడల్పు;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
స్థిర ptrdiff_t before_max_వెడల్పు;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
స్థిర ptrdiff_t keyafter_max_వెడల్పు;

/* Length of string that flags truncation.  */
స్థిర idx_t truncation_string_నిడివి;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

స్థిర ఖండం_2 తోక;		/* tail field */
స్థిర bool తోక_truncation;	/* flag truncation after the tail field */

స్థిర ఖండం_2 before;		/* before field */
స్థిర bool before_truncation;	/* flag truncation before the before field */

స్థిర ఖండం_2 keyafter;		/* keyword-and-after field */
స్థిర bool keyafter_truncation; /* flag truncation after the keyafter field */

స్థిర ఖండం_2 తల;		/* head field */
స్థిర bool తల_truncation;	/* flag truncation before the head field */

స్థిర ఖండం_2 reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

స్థిర _Noreturn శూన్య
matcher_error (శూన్య)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

స్థిర శూన్య
unescape_string (అక్షరం *string)
{
  అక్షరం *cursor;			/* cursor in result */
  పూర్ణాంకం విలువ;			/* value of \nnn escape */
  పూర్ణాంకం నిడివి;			/* length of \nnn escape */

  cursor = string;

  అయితే (*string)
    {
      ఒకవేళ (*string == '\\')
        {
          string++;
          ఎంపిక (*string)
            {
            స్థితి 'x':		/* \xhhh escape, 3 chars maximum */
              విలువ = 0;
              కొరకు (నిడివి = 0, string++;
                   నిడివి < 3 && c_isxdigit (*string);
                   నిడివి++, string++)
                విలువ = విలువ * 16 + fromhex (*string);
              ఒకవేళ (నిడివి == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              లేకపోతే
                *cursor++ = విలువ;
              ఆపు;

            స్థితి '0':		/* \0ooo escape, 3 chars maximum */
              విలువ = 0;
              కొరకు (నిడివి = 0, string++;
                   నిడివి < 3 && isoct (*string);
                   నిడివి++, string++)
                విలువ = విలువ * 8 + fromoct (*string);
              *cursor++ = విలువ;
              ఆపు;

            స్థితి 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              ఆపు;

            స్థితి 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              ఆపు;

            స్థితి 'c':		/* cancel the rest of the output */
              అయితే (*string)
                string++;
              ఆపు;

            స్థితి 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              ఆపు;

            స్థితి 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              ఆపు;

            స్థితి 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              ఆపు;

            స్థితి 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              ఆపు;

            స్థితి 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              ఆపు;

            స్థితి '\0':		/* lone backslash at end of string */
              /* ignore it */
              ఆపు;

            అప్రమేయ:
              *cursor++ = '\\';
              *cursor++ = *string++;
              ఆపు;
            }
        }
      లేకపోతే
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

స్థిర శూన్య
compile_regex (నిర్మాణం regex_దత్తాంశం *regex)
{
  నిర్మాణం re_నమూనా_బఫరు *నమూనా_2 = &regex->నమూనా_2;
  అక్షరం స్థిరరాశి *string = regex->string;
  అక్షరం స్థిరరాశి *సందేశం_2;

  నమూనా_2->బఫరు = ఖాళీ;
  నమూనా_2->allocated = 0;
  నమూనా_2->fastmap = regex->fastmap;
  నమూనా_2->translate = ignore_case ? folded_chars : ఖాళీ;

  సందేశం_2 = re_compile_నమూనా (string, అక్షరశ్రేణి_పొడవు (string), నమూనా_2);
  ఒకవేళ (సందేశం_2)
    error (EXIT_FAILURE, 0, _("%s (for regexp %s)"), సందేశం_2, quote (string));

  /* The fastmap should be compiled before 're_match'.  The following
     call is not mandatory, because 're_search' is always called sooner,
     and it compiles the fastmap if this has not been done yet.  */

  re_compile_fastmap (నమూనా_2);
}

/*------------------------------------------------------------------------.
| This will initialize various tables for pattern match and compiles some |
| regexps.								  |
`------------------------------------------------------------------------*/

స్థిర శూన్య
initialize_regex (శూన్య)
{
  /* Initialize the case folding table.  */

  ఒకవేళ (ignore_case)
    కొరకు (పూర్ణాంకం character = 0; character < CHAR_సమితి_కొలత; character++)
      folded_chars[character] = పెద్ద_అక్షరం (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  ఒకవేళ (context_regex.string)
    {
      ఒకవేళ (!*context_regex.string)
        context_regex.string = ఖాళీ;
    }
  లేకపోతే ఒకవేళ (gnu_extensions && !ఆదానం_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  లేకపోతే
    context_regex.string = "\n";

  ఒకవేళ (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  ఒకవేళ (మాట_regex.string)
    compile_regex (&మాట_regex);
  లేకపోతే ఒకవేళ (!break_file)
    {
      ఒకవేళ (gnu_extensions)
        {

          /* Simulate \w+.  */

          కొరకు (పూర్ణాంకం character = 0; character < CHAR_సమితి_కొలత; character++)
            మాట_fastmap[character] = !! అక్షరమా (character);
        }
      లేకపోతే
        {

          /* Simulate [^ \t\n]+.  */

          memset (మాట_fastmap, 1, CHAR_సమితి_కొలత);
          మాట_fastmap[' '] = 0;
          మాట_fastmap['\t'] = 0;
          మాట_fastmap['\n'] = 0;
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

స్థిర శూన్య
swallow_file_లోపల_జ్ఞాపకం (అక్షరం స్థిరరాశి *file_name, ఖండం_2 *ఖండం)
{
  size_t వాడిన_నిడివి;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  ఒకవేళ (using_stdin)
    ఖండం->ప్రారంభం = fread_file (stdin, 0, &వాడిన_నిడివి);
  లేకపోతే
    ఖండం->ప్రారంభం = read_file (file_name, 0, &వాడిన_నిడివి);

  ఒకవేళ (!ఖండం->ప్రారంభం)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  ఒకవేళ (using_stdin)
    clearerr (stdin);

  ఖండం->చివర = ఖండం->ప్రారంభం + వాడిన_నిడివి;
}

/* Sort and search routines.  */

/*--------------------------------------------------------------------------.
| Compare two words, FIRST and SECOND, and return 0 if they are identical.  |
| Return less than 0 if the first word goes before the second; return	    |
| greater than 0 if the first word goes after the second.		    |
|									    |
| If a word is indeed a prefix of the other, the shorter should go first.   |
`--------------------------------------------------------------------------*/

స్థిర పూర్ణాంకం
compare_పదాలు (స్థిరరాశి శూన్య *void_మొదటి, స్థిరరాశి శూన్య *void_రెండవ)
{
  మాట స్థిరరాశి *మొదటి = void_మొదటి;
  మాట స్థిరరాశి *రెండవ = void_రెండవ;
  idx_t నిడివి = MIN (మొదటి->కొలత, రెండవ->కొలత);

  ఒకవేళ (ignore_case)
    {
      కొరకు (idx_t counter = 0; counter < నిడివి; counter++)
        {
          పూర్ణాంకం విలువ = (folded_chars[గమ్యం_uchar (మొదటి->ప్రారంభం[counter])]
                       - folded_chars[గమ్యం_uchar (రెండవ->ప్రారంభం[counter])]);
          ఒకవేళ (విలువ != 0)
            తిరిగి విలువ;
        }
    }
  లేకపోతే
    {
      కొరకు (idx_t counter = 0; counter < నిడివి; counter++)
        {
          పూర్ణాంకం విలువ = (గమ్యం_uchar (మొదటి->ప్రారంభం[counter])
                       - గమ్యం_uchar (రెండవ->ప్రారంభం[counter]));
          ఒకవేళ (విలువ != 0)
            తిరిగి విలువ;
        }
    }

  తిరిగి _GL_CMP (మొదటి->కొలత, రెండవ->కొలత);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

స్థిర పూర్ణాంకం
compare_occurs (స్థిరరాశి శూన్య *void_మొదటి, స్థిరరాశి శూన్య *void_రెండవ)
{
#నిర్వచించు మొదటి ((స్థిరరాశి OCCURS *) void_మొదటి)
#నిర్వచించు రెండవ ((స్థిరరాశి OCCURS *) void_రెండవ)
  పూర్ణాంకం విలువ;

  విలువ = compare_పదాలు (&మొదటి->కీలకం, &రెండవ->కీలకం);
  తిరిగి (విలువ ? విలువ
          : _GL_CMP (మొదటి->కీలకం.ప్రారంభం, రెండవ->కీలకం.ప్రారంభం));
#నిర్వచనం_రద్దు మొదటి
#నిర్వచనం_రద్దు రెండవ
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_శుద్ధ
స్థిర bool
శోధన_పట్టిక (మాట *మాట_2, మాట_పట్టిక *పట్టిక)
{
  idx_t lo = 0;
  idx_t hi = పట్టిక->నిడివి;
  అయితే (lo < hi)
    {
      idx_t మధ్య = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      పూర్ణాంకం విలువ = compare_పదాలు (మాట_2, పట్టిక->ప్రారంభం + మధ్య);
      ఒకవేళ (విలువ < 0)
        hi = మధ్య;
      లేకపోతే ఒకవేళ (విలువ > 0)
        lo = మధ్య + 1;
      లేకపోతే
        తిరిగి true;
    }
  తిరిగి false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

స్థిర శూన్య
క్రమీకరించు_దొరికింది_occurs (శూన్య)
{

  /* Only one language for the time being.  */
  ఒకవేళ (సంఖ్య_యొక్క_occurs[0])
    qsort (occurs_పట్టిక[0], సంఖ్య_యొక్క_occurs[0], పరిమాణం **occurs_పట్టిక,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

స్థిర శూన్య
digest_break_file (అక్షరం స్థిరరాశి *file_name)
{
  ఖండం_2 file_contents;		/* to receive a copy of the file */

  swallow_file_లోపల_జ్ఞాపకం (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (మాట_fastmap, 1, CHAR_సమితి_కొలత);
  కొరకు (అక్షరం *cursor = file_contents.ప్రారంభం; cursor < file_contents.చివర;
       cursor++)
    మాట_fastmap[గమ్యం_uchar (*cursor)] = 0;

  ఒకవేళ (!gnu_extensions)
    {

      /* If GNU extensions are enabled, the only way to avoid newline as
         a break character is to write all the break characters in the
         file with no newline at all, not even at the end of the file.
         If disabled, spaces, tabs and newlines are always considered as
         break characters even if not included in the break file.  */

      మాట_fastmap[' '] = 0;
      మాట_fastmap['\t'] = 0;
      మాట_fastmap['\n'] = 0;
    }

  /* Return the space of the file, which is no more required.  */

  విడుదల (file_contents.ప్రారంభం);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

స్థిర శూన్య
digest_మాట_file (అక్షరం స్థిరరాశి *file_name, మాట_పట్టిక *పట్టిక)
{
  ఖండం_2 file_contents;		/* to receive a copy of the file */
  అక్షరం *cursor;			/* cursor in file copy */
  అక్షరం *మాట_ప్రారంభం;		/* start of the current word */

  swallow_file_లోపల_జ్ఞాపకం (file_name, &file_contents);

  పట్టిక->ప్రారంభం = ఖాళీ;
  పట్టిక->alloc = 0;
  పట్టిక->నిడివి = 0;

  /* Read the whole file.  */

  cursor = file_contents.ప్రారంభం;
  అయితే (cursor < file_contents.చివర)
    {

      /* Read one line, and save the word in contains.  */

      మాట_ప్రారంభం = cursor;
      అయితే (cursor < file_contents.చివర && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      ఒకవేళ (cursor > మాట_ప్రారంభం)
        {
          ఒకవేళ (పట్టిక->నిడివి == పట్టిక->alloc)
            పట్టిక->ప్రారంభం = xpalloc (పట్టిక->ప్రారంభం, &పట్టిక->alloc, 1, -1,
                                    పరిమాణం *పట్టిక->ప్రారంభం);
          idx_t స్థిరరాశి పట్టిక_idx = పట్టిక->నిడివి++;
          పట్టిక->ప్రారంభం[పట్టిక_idx].ప్రారంభం = మాట_ప్రారంభం;
          పట్టిక->ప్రారంభం[పట్టిక_idx].కొలత = cursor - మాట_ప్రారంభం;
        }

      /* This test allows for an incomplete line at end of file.  */

      ఒకవేళ (cursor < file_contents.చివర)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (పట్టిక->ప్రారంభం, పట్టిక->నిడివి, పరిమాణం పట్టిక->ప్రారంభం[0], compare_పదాలు);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

స్థిర శూన్య
కనుగొను_occurs_లోపల_పాఠ్యం (పూర్ణాంకం file_సూచీ)
{
  అక్షరం *పరిశీలన;			/* for scanning the source text */
  అక్షరం *పంక్తి_ప్రారంభం;		/* start of the current input line */
  అక్షరం *పంక్తి_పరిశీలన;		/* newlines scanned until this point */
  idx_t reference_నిడివి;	/* length of reference in input mode */
  మాట possible_కీలకం;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  అక్షరం *context_ప్రారంభం;		/* start of left context */
  అక్షరం *context_చివర;		/* end of right context */
  అక్షరం *మాట_ప్రారంభం;		/* start of word */
  అక్షరం *మాట_చివర;		/* end of word */
  అక్షరం *తదుపరి_context_ప్రారంభం;	/* next start of left context */

  స్థిరరాశి ఖండం_2 *పాఠ్యం_బఫరు = &పాఠ్యం_buffers[file_సూచీ];

  /* reference_length is always used within 'if (input_reference)'.
     However, GNU C diagnoses that it may be used uninitialized.  The
     following assignment is merely to shut it up.  */

  reference_నిడివి = 0;

  /* Tracking where lines start is helpful for reference processing.  In
     auto reference mode, this allows counting lines.  In input reference
     mode, this permits finding the beginning of the references.

     The first line begins with the file, skip immediately this very first
     reference in input reference mode, to help further rejection any word
     found inside it.  Also, unconditionally assigning these variable has
     the happy effect of shutting up lint.  */

  పంక్తి_ప్రారంభం = పాఠ్యం_బఫరు->ప్రారంభం;
  పంక్తి_పరిశీలన = పంక్తి_ప్రారంభం;
  ఒకవేళ (ఆదానం_reference)
    {
      SKIP_NON_WHITE (పంక్తి_పరిశీలన, పాఠ్యం_బఫరు->చివర);
      reference_నిడివి = పంక్తి_పరిశీలన - పంక్తి_ప్రారంభం;
      SKIP_WHITE (పంక్తి_పరిశీలన, పాఠ్యం_బఫరు->చివర);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  కొరకు (అక్షరం *cursor = పాఠ్యం_బఫరు->ప్రారంభం;
       cursor < పాఠ్యం_బఫరు->చివర;
       cursor = తదుపరి_context_ప్రారంభం)
    {

      /* 'context_start' gets initialized before the processing of each
         line, or once for the whole buffer if no end of line or sentence
         sequence separator.  */

      context_ప్రారంభం = cursor;

      /* If an end of line or end of sentence sequence is defined and
         non-empty, 'next_context_start' will be recomputed to be the end of
         each line or sentence, before each one is processed.  If no such
         sequence, then 'next_context_start' is set at the end of the whole
         buffer, which is then considered to be a single line or sentence.
         This test also accounts for the case of an incomplete line or
         sentence at the end of the buffer.  */

      తదుపరి_context_ప్రారంభం = పాఠ్యం_బఫరు->చివర;
      ఒకవేళ (context_regex.string)
        ఎంపిక (re_శోధన (&context_regex.నమూనా_2, cursor,
                           పాఠ్యం_బఫరు->చివర - cursor,
                           0, పాఠ్యం_బఫరు->చివర - cursor, &context_regs))
          {
          స్థితి -2:
            matcher_error ();

          స్థితి -1:
            ఆపు;

          స్థితి 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          అప్రమేయ:
            తదుపరి_context_ప్రారంభం = cursor + context_regs.చివర[0];
            ఆపు;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_చివర = తదుపరి_context_ప్రారంభం;
      SKIP_WHITE_BACKWARDS (context_చివర, context_ప్రారంభం);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      అయితే (true)
        {
          ఒకవేళ (మాట_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_శోధన (&మాట_regex.నమూనా_2, cursor,
                                      context_చివర - cursor,
                                      0, context_చివర - cursor, &మాట_regs);
              ఒకవేళ (r == -2)
                matcher_error ();
              ఒకవేళ (r == -1)
                ఆపు;
              మాట_ప్రారంభం = cursor + మాట_regs.ప్రారంభం[0];
              మాట_చివర = cursor + మాట_regs.చివర[0];
            }
          లేకపోతే

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              పరిశీలన = cursor;
              అయితే (పరిశీలన < context_చివర
                     && !మాట_fastmap[గమ్యం_uchar (*పరిశీలన)])
                పరిశీలన++;

              ఒకవేళ (పరిశీలన == context_చివర)
                ఆపు;

              మాట_ప్రారంభం = పరిశీలన;

              అయితే (పరిశీలన < context_చివర
                     && మాట_fastmap[గమ్యం_uchar (*పరిశీలన)])
                పరిశీలన++;

              మాట_చివర = పరిశీలన;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = మాట_ప్రారంభం;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          ఒకవేళ (మాట_చివర == మాట_ప్రారంభం)
            {
              cursor++;
              కొనసాగు;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_కీలకం.ప్రారంభం = cursor;
          possible_కీలకం.కొలత = మాట_చివర - మాట_ప్రారంభం;
          cursor += possible_కీలకం.కొలత;

          ఒకవేళ (possible_కీలకం.కొలత > గరిష్ఠ_మాట_నిడివి)
            గరిష్ఠ_మాట_నిడివి = possible_కీలకం.కొలత;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          ఒకవేళ (ఆదానం_reference)
            {
              అయితే (పంక్తి_పరిశీలన < possible_కీలకం.ప్రారంభం)
                ఒకవేళ (*పంక్తి_పరిశీలన == '\n')
                  {
                    మొత్తం_పంక్తి_లెక్క++;
                    పంక్తి_పరిశీలన++;
                    పంక్తి_ప్రారంభం = పంక్తి_పరిశీలన;
                    SKIP_NON_WHITE (పంక్తి_పరిశీలన, పాఠ్యం_బఫరు->చివర);
                    reference_నిడివి = పంక్తి_పరిశీలన - పంక్తి_ప్రారంభం;
                  }
                లేకపోతే
                  పంక్తి_పరిశీలన++;
              ఒకవేళ (పంక్తి_పరిశీలన > possible_కీలకం.ప్రారంభం)
                కొనసాగు;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          ఒకవేళ (ignore_file && శోధన_పట్టిక (&possible_కీలకం, &ignore_పట్టిక))
            కొనసాగు;
          ఒకవేళ (only_file && !శోధన_పట్టిక (&possible_కీలకం, &only_పట్టిక))
            కొనసాగు;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          ఒకవేళ (సంఖ్య_యొక్క_occurs[0] == occurs_alloc[0])
            occurs_పట్టిక[0] = xpalloc (occurs_పట్టిక[0], &occurs_alloc[0],
                                       1, -1, పరిమాణం *occurs_పట్టిక[0]);
          occurs_cursor = occurs_పట్టిక[0] + సంఖ్య_యొక్క_occurs[0];

          /* Define the reference field, if any.  */

          ఒకవేళ (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              అయితే (పంక్తి_పరిశీలన < possible_కీలకం.ప్రారంభం)
                ఒకవేళ (*పంక్తి_పరిశీలన == '\n')
                  {
                    మొత్తం_పంక్తి_లెక్క++;
                    పంక్తి_పరిశీలన++;
                    పంక్తి_ప్రారంభం = పంక్తి_పరిశీలన;
                    SKIP_NON_WHITE (పంక్తి_పరిశీలన, పాఠ్యం_బఫరు->చివర);
                  }
                లేకపోతే
                  పంక్తి_పరిశీలన++;

              occurs_cursor->reference = మొత్తం_పంక్తి_లెక్క;
            }
          లేకపోతే ఒకవేళ (ఆదానం_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = పంక్తి_ప్రారంభం - possible_కీలకం.ప్రారంభం;
              ఒకవేళ (reference_నిడివి > reference_max_వెడల్పు)
                reference_max_వెడల్పు = reference_నిడివి;
            }

          /* Exclude the reference from the context in simple cases.  */

          ఒకవేళ (ఆదానం_reference && పంక్తి_ప్రారంభం == context_ప్రారంభం)
            {
              SKIP_NON_WHITE (context_ప్రారంభం, context_చివర);
              SKIP_WHITE (context_ప్రారంభం, context_చివర);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->కీలకం = possible_కీలకం;
          occurs_cursor->ఎడమ = context_ప్రారంభం - possible_కీలకం.ప్రారంభం;
          occurs_cursor->కుడి = context_చివర - possible_కీలకం.ప్రారంభం;
          occurs_cursor->file_సూచీ = file_సూచీ;

          సంఖ్య_యొక్క_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

స్థిర శూన్య
print_spaces (ptrdiff_t సంఖ్య)
{
  కొరకు (ptrdiff_t counter = సంఖ్య; counter > 0; counter--)
    అక్షరం_రాయి (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

స్థిర శూన్య
print_క్షేత్రం (ఖండం_2 క్షేత్రం)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  కొరకు (అక్షరం *cursor = క్షేత్రం.ప్రారంభం; cursor < క్షేత్రం.చివర; cursor++)
    {
      గుర్తులేని అక్షరం character = *cursor;
      ఒకవేళ (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          ఎంపిక (character)
            {
            స్థితి '"':
              /* In roff output format, double any quote.  */
              అక్షరం_రాయి ('"');
              అక్షరం_రాయి ('"');
              ఆపు;

            స్థితి '$':
            స్థితి '%':
            స్థితి '&':
            స్థితి '#':
            స్థితి '_':
              /* In TeX output format, precede these with a backslash.  */
              అక్షరం_రాయి ('\\');
              అక్షరం_రాయి (character);
              ఆపు;

            స్థితి '{':
            స్థితి '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              రాయి ("$\\%c$", character);
              ఆపు;

            స్థితి '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              ఆపు;

            అప్రమేయ:
              /* Any other flagged character produces a single space.  */
              అక్షరం_రాయి (' ');
            }
        }
      లేకపోతే
        అక్షరం_రాయి (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

స్థిర శూన్య
fix_output_parameters (శూన్య)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  ఒకవేళ (auto_reference)
    {
      reference_max_వెడల్పు = 0;
      కొరకు (పూర్ణాంకం file_సూచీ = 0; file_సూచీ < సంఖ్య_ఆదానం_files; file_సూచీ++)
        {
          intmax_t పంక్తి_ordinal = file_పంక్తి_లెక్క[file_సూచీ] + 1;
          ఒకవేళ (file_సూచీ > 0)
            పంక్తి_ordinal -= file_పంక్తి_లెక్క[file_సూచీ - 1];
          అక్షరం ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_వెడల్పు = sprintf (ordinal_string, "%jd", పంక్తి_ordinal);
          ఒకవేళ (ఆదానం_file_name[file_సూచీ])
            reference_వెడల్పు += అక్షరశ్రేణి_పొడవు (ఆదానం_file_name[file_సూచీ]);
          ఒకవేళ (reference_వెడల్పు > reference_max_వెడల్పు)
            reference_max_వెడల్పు = reference_వెడల్పు;
        }
      reference_max_వెడల్పు++;
      reference.ప్రారంభం = xmalloc (reference_max_వెడల్పు + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  ఒకవేళ ((auto_reference || ఆదానం_reference) && !కుడి_reference)
    పంక్తి_వెడల్పు = MAX (0, పంక్తి_వెడల్పు - (reference_max_వెడల్పు + gap_కొలత));

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

  half_పంక్తి_వెడల్పు = పంక్తి_వెడల్పు >> 1;
  before_max_వెడల్పు = half_పంక్తి_వెడల్పు - gap_కొలత;
  keyafter_max_వెడల్పు = half_పంక్తి_వెడల్పు;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  ఒకవేళ (truncation_string && *truncation_string)
    truncation_string_నిడివి = అక్షరశ్రేణి_పొడవు (truncation_string);
  లేకపోతే
    truncation_string = ఖాళీ;

  ఒకవేళ (gnu_extensions)
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

      before_max_వెడల్పు -= 2 * truncation_string_నిడివి;
      keyafter_max_వెడల్పు -= 2 * truncation_string_నిడివి;
    }
  లేకపోతే
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_వెడల్పు -= 2 * truncation_string_నిడివి + 1;
    }

  ఒకవేళ (before_max_వెడల్పు < 0)
    before_max_వెడల్పు = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  కొరకు (పూర్ణాంకం character = 0; character < CHAR_సమితి_కొలత; character++)
    edited_flag[character] = !! ఖాళీనా (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  ఎంపిక (output_format)
    {
    స్థితి UNKNOWN_FORMAT:
      /* Should never happen.  */

    స్థితి DUMB_FORMAT:
      ఆపు;

    స్థితి ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      ఆపు;

    స్థితి TEX_FORMAT:

      /* Various characters need special processing.  */

      కొరకు (అక్షరం స్థిరరాశి *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[గమ్యం_uchar (*cursor)] = 1;

      ఆపు;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

స్థిర శూన్య
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t తోక_max_వెడల్పు;	/* allowable width of tail field */
  ptrdiff_t తల_max_వెడల్పు;	/* allowable width of head field */
  అక్షరం *cursor;			/* running cursor in source text */
  అక్షరం *ఎడమ_context_ప్రారంభం;	/* start of left context */
  అక్షరం *కుడి_context_చివర;	/* end of right context */
  అక్షరం *ఎడమ_క్షేత్రం_ప్రారంభం;	/* conservative start for 'head'/'before' */
  అక్షరం స్థిరరాశి *file_name;	/* file name for reference */
  intmax_t పంక్తి_ordinal;	/* line ordinal for reference */
  అక్షరం స్థిరరాశి *బఫరు_ప్రారంభం;	/* start of buffered file for this occurs */
  అక్షరం స్థిరరాశి *బఫరు_చివర;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.ప్రారంభం = occurs->కీలకం.ప్రారంభం;
  keyafter.చివర = keyafter.ప్రారంభం + occurs->కీలకం.కొలత;
  ఎడమ_context_ప్రారంభం = keyafter.ప్రారంభం + occurs->ఎడమ;
  కుడి_context_చివర = keyafter.ప్రారంభం + occurs->కుడి;

  బఫరు_ప్రారంభం = పాఠ్యం_buffers[occurs->file_సూచీ].ప్రారంభం;
  బఫరు_చివర = పాఠ్యం_buffers[occurs->file_సూచీ].చివర;

  cursor = keyafter.చివర;
  అయితే (cursor < కుడి_context_చివర
         && cursor <= keyafter.ప్రారంభం + keyafter_max_వెడల్పు)
    {
      keyafter.చివర = cursor;
      SKIP_SOMETHING (cursor, కుడి_context_చివర);
    }
  ఒకవేళ (cursor <= keyafter.ప్రారంభం + keyafter_max_వెడల్పు)
    keyafter.చివర = cursor;

  keyafter_truncation = truncation_string && keyafter.చివర < కుడి_context_చివర;

  SKIP_WHITE_BACKWARDS (keyafter.చివర, keyafter.ప్రారంభం);

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

  ఒకవేళ (-occurs->ఎడమ > half_పంక్తి_వెడల్పు + గరిష్ఠ_మాట_నిడివి)
    {
      ఎడమ_క్షేత్రం_ప్రారంభం
        = keyafter.ప్రారంభం - (half_పంక్తి_వెడల్పు + గరిష్ఠ_మాట_నిడివి);
      SKIP_SOMETHING (ఎడమ_క్షేత్రం_ప్రారంభం, keyafter.ప్రారంభం);
    }
  లేకపోతే
    ఎడమ_క్షేత్రం_ప్రారంభం = keyafter.ప్రారంభం + occurs->ఎడమ;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.ప్రారంభం = ఎడమ_క్షేత్రం_ప్రారంభం;
  before.చివర = keyafter.ప్రారంభం;
  SKIP_WHITE_BACKWARDS (before.చివర, before.ప్రారంభం);

  అయితే (before.ప్రారంభం + before_max_వెడల్పు < before.చివర)
    SKIP_SOMETHING (before.ప్రారంభం, before.చివర);

  ఒకవేళ (truncation_string)
    {
      cursor = before.ప్రారంభం;
      SKIP_WHITE_BACKWARDS (cursor, బఫరు_ప్రారంభం);
      before_truncation = cursor > ఎడమ_context_ప్రారంభం;
    }
  లేకపోతే
    before_truncation = false;

  SKIP_WHITE (before.ప్రారంభం, బఫరు_చివర);

  /* The tail could not take more columns than what has been left in the
     left context field, and a gap is mandatory.  It starts after the
     right context, and does not contain prefixed spaces.  It ends at
     the end of line, the end of buffer or when the tail field is full,
     whichever comes first.  It cannot contain only part of a word, and
     has no suffixed spaces.  */

  తోక_max_వెడల్పు
    = before_max_వెడల్పు - (before.చివర - before.ప్రారంభం) - gap_కొలత;

  ఒకవేళ (తోక_max_వెడల్పు > 0)
    {
      తోక.ప్రారంభం = keyafter.చివర;
      SKIP_WHITE (తోక.ప్రారంభం, బఫరు_చివర);

      తోక.చివర = తోక.ప్రారంభం;
      cursor = తోక.చివర;
      అయితే (cursor < కుడి_context_చివర
             && cursor < తోక.ప్రారంభం + తోక_max_వెడల్పు)
        {
          తోక.చివర = cursor;
          SKIP_SOMETHING (cursor, కుడి_context_చివర);
        }

      ఒకవేళ (cursor < తోక.ప్రారంభం + తోక_max_వెడల్పు)
        తోక.చివర = cursor;

      ఒకవేళ (తోక.చివర > తోక.ప్రారంభం)
        {
          keyafter_truncation = false;
          తోక_truncation = truncation_string && తోక.చివర < కుడి_context_చివర;
        }
      లేకపోతే
        తోక_truncation = false;

      SKIP_WHITE_BACKWARDS (తోక.చివర, తోక.ప్రారంభం);
    }
  లేకపోతే
    {

      /* No place left for a tail field.  */

      తోక.ప్రారంభం = ఖాళీ;
      తోక.చివర = ఖాళీ;
      తోక_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  తల_max_వెడల్పు
    = keyafter_max_వెడల్పు - (keyafter.చివర - keyafter.ప్రారంభం) - gap_కొలత;

  ఒకవేళ (తల_max_వెడల్పు > 0)
    {
      తల.చివర = before.ప్రారంభం;
      SKIP_WHITE_BACKWARDS (తల.చివర, బఫరు_ప్రారంభం);

      తల.ప్రారంభం = ఎడమ_క్షేత్రం_ప్రారంభం;
      అయితే (తల.ప్రారంభం + తల_max_వెడల్పు < తల.చివర)
        SKIP_SOMETHING (తల.ప్రారంభం, తల.చివర);

      ఒకవేళ (తల.చివర > తల.ప్రారంభం)
        {
          before_truncation = false;
          తల_truncation = (truncation_string
                             && తల.ప్రారంభం > ఎడమ_context_ప్రారంభం);
        }
      లేకపోతే
        తల_truncation = false;

      SKIP_WHITE (తల.ప్రారంభం, తల.చివర);
    }
  లేకపోతే
    {

      /* No place left for a head field.  */

      తల.ప్రారంభం = ఖాళీ;
      తల.చివర = ఖాళీ;
      తల_truncation = false;
    }

  ఒకవేళ (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = ఆదానం_file_name[occurs->file_సూచీ];
      ఒకవేళ (!file_name)
        file_name = "";

      పంక్తి_ordinal = occurs->reference + 1;
      ఒకవేళ (occurs->file_సూచీ > 0)
        పంక్తి_ordinal -= file_పంక్తి_లెక్క[occurs->file_సూచీ - 1];

      అక్షరం *file_చివర = stpcpy (reference.ప్రారంభం, file_name);
      reference.చివర = file_చివర + sprintf (file_చివర, ":%jd", పంక్తి_ordinal);
    }
  లేకపోతే ఒకవేళ (ఆదానం_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.ప్రారంభం = keyafter.ప్రారంభం + occurs->reference;
      reference.చివర = reference.ప్రారంభం;
      SKIP_NON_WHITE (reference.చివర, కుడి_context_చివర);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

స్థిర శూన్య
output_one_roff_పంక్తి (శూన్య)
{
  /* Output the 'tail' field.  */

  రాయి (".%s \"", macro_name);
  print_క్షేత్రం (తోక);
  ఒకవేళ (తోక_truncation)
    fputs (truncation_string, stdout);
  అక్షరం_రాయి ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  ఒకవేళ (before_truncation)
    fputs (truncation_string, stdout);
  print_క్షేత్రం (before);
  అక్షరం_రాయి ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_క్షేత్రం (keyafter);
  ఒకవేళ (keyafter_truncation)
    fputs (truncation_string, stdout);
  అక్షరం_రాయి ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  ఒకవేళ (తల_truncation)
    fputs (truncation_string, stdout);
  print_క్షేత్రం (తల);
  అక్షరం_రాయి ('"');

  /* Conditionally output the 'reference' field.  */

  ఒకవేళ (auto_reference || ఆదానం_reference)
    {
      fputs (" \"", stdout);
      print_క్షేత్రం (reference);
      అక్షరం_రాయి ('"');
    }

  అక్షరం_రాయి ('\n');
}

/*---------------------------------------------------------.
| Output the current output fields as one line for 'TeX'.  |
`---------------------------------------------------------*/

స్థిర శూన్య
output_one_tex_పంక్తి (శూన్య)
{
  ఖండం_2 కీలకం;			/* key field, isolated */
  ఖండం_2 after;			/* after field, isolated */
  అక్షరం *cursor;			/* running cursor in source text */

  రాయి ("\\%s ", macro_name);
  అక్షరం_రాయి ('{');
  print_క్షేత్రం (తోక);
  fputs ("}{", stdout);
  print_క్షేత్రం (before);
  fputs ("}{", stdout);
  కీలకం.ప్రారంభం = keyafter.ప్రారంభం;
  after.చివర = keyafter.చివర;
  cursor = keyafter.ప్రారంభం;
  SKIP_SOMETHING (cursor, keyafter.చివర);
  కీలకం.చివర = cursor;
  after.ప్రారంభం = cursor;
  print_క్షేత్రం (కీలకం);
  fputs ("}{", stdout);
  print_క్షేత్రం (after);
  fputs ("}{", stdout);
  print_క్షేత్రం (తల);
  అక్షరం_రాయి ('}');
  ఒకవేళ (auto_reference || ఆదానం_reference)
    {
      అక్షరం_రాయి ('{');
      print_క్షేత్రం (reference);
      అక్షరం_రాయి ('}');
    }
  అక్షరం_రాయి ('\n');
}

/*-------------------------------------------------------------------.
| Output the current output fields as one line for a dumb terminal.  |
`-------------------------------------------------------------------*/

స్థిర శూన్య
output_one_dumb_పంక్తి (శూన్య)
{
  ఒకవేళ (!కుడి_reference)
    {
      ఒకవేళ (auto_reference)
        {

          /* Output the 'reference' field, in such a way that GNU emacs
             next-error will handle it.  The ending colon is taken from the
             gap which follows.  */

          print_క్షేత్రం (reference);
          అక్షరం_రాయి (':');
          print_spaces (reference_max_వెడల్పు
                        + gap_కొలత
                        - (reference.చివర - reference.ప్రారంభం)
                        - 1);
        }
      లేకపోతే
        {

          /* Output the 'reference' field and its following gap.  */

          print_క్షేత్రం (reference);
          print_spaces (reference_max_వెడల్పు
                        + gap_కొలత
                        - (reference.చివర - reference.ప్రారంభం));
        }
    }

  ఒకవేళ (తోక.ప్రారంభం < తోక.చివర)
    {
      /* Output the 'tail' field.  */

      print_క్షేత్రం (తోక);
      ఒకవేళ (తోక_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_పంక్తి_వెడల్పు - gap_కొలత
                    - (before.చివర - before.ప్రారంభం)
                    - (before_truncation ? truncation_string_నిడివి : 0)
                    - (తోక.చివర - తోక.ప్రారంభం)
                    - (తోక_truncation ? truncation_string_నిడివి : 0));
    }
  లేకపోతే
    print_spaces (half_పంక్తి_వెడల్పు - gap_కొలత
                  - (before.చివర - before.ప్రారంభం)
                  - (before_truncation ? truncation_string_నిడివి : 0));

  /* Output the 'before' field.  */

  ఒకవేళ (before_truncation)
    fputs (truncation_string, stdout);
  print_క్షేత్రం (before);

  print_spaces (gap_కొలత);

  /* Output the 'keyafter' field.  */

  print_క్షేత్రం (keyafter);
  ఒకవేళ (keyafter_truncation)
    fputs (truncation_string, stdout);

  ఒకవేళ (తల.ప్రారంభం < తల.చివర)
    {
      /* Output the 'head' field.  */

      print_spaces (half_పంక్తి_వెడల్పు
                    - (keyafter.చివర - keyafter.ప్రారంభం)
                    - (keyafter_truncation ? truncation_string_నిడివి : 0)
                    - (తల.చివర - తల.ప్రారంభం)
                    - (తల_truncation ? truncation_string_నిడివి : 0));
      ఒకవేళ (తల_truncation)
        fputs (truncation_string, stdout);
      print_క్షేత్రం (తల);
    }
  లేకపోతే

    ఒకవేళ ((auto_reference || ఆదానం_reference) && కుడి_reference)
      print_spaces (half_పంక్తి_వెడల్పు
                    - (keyafter.చివర - keyafter.ప్రారంభం)
                    - (keyafter_truncation ? truncation_string_నిడివి : 0));

  ఒకవేళ ((auto_reference || ఆదానం_reference) && కుడి_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_కొలత);
      print_క్షేత్రం (reference);
    }

  అక్షరం_రాయి ('\n');
}

/*------------------------------------------------------------------------.
| Scan the whole occurs table and, for each entry, output one line in the |
| appropriate format.							  |
`------------------------------------------------------------------------*/

స్థిర శూన్య
generate_all_output (శూన్య)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  తోక.ప్రారంభం = ఖాళీ;
  తోక.చివర = ఖాళీ;
  తోక_truncation = false;

  తల.ప్రారంభం = ఖాళీ;
  తల.చివర = ఖాళీ;
  తల_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_పట్టిక[0];

  కొరకు (idx_t occurs_సూచీ = 0; occurs_సూచీ < సంఖ్య_యొక్క_occurs[0];
       occurs_సూచీ++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      ఎంపిక (output_format)
        {
        స్థితి UNKNOWN_FORMAT:
          /* Should never happen.  */

        స్థితి DUMB_FORMAT:
          output_one_dumb_పంక్తి ();
          ఆపు;

        స్థితి ROFF_FORMAT:
          output_one_roff_పంక్తి ();
          ఆపు;

        స్థితి TEX_FORMAT:
          output_one_tex_పంక్తి ();
          ఆపు;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [ఆదానం_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [ఆదానం_2 [OUTPUT]]\n"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Output a permuted index, including context, యొక్క the పదాలు లోపల the ఆదానం files.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --స్వయంచాలక-reference           output automatically generated references\n\
"));
      oputs (_("\
  -G, --traditional              behave మరిన్ని like System V 'ptx'\n\
"));
      oputs (_("\
  -F, --flag-truncation=STRING   use STRING కొరకు flagging పంక్తి truncations.\n\
                                 The అప్రమేయ is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name గమ్యం use instead యొక్క 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --కుడి-side-refs          put references at కుడి, not counted లోపల -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   కొరకు చివర యొక్క పంక్తులు or చివర యొక్క sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --మాట_2-regexp=REGEXP       use REGEXP గమ్యం సరిపోలిక each keyword\n\
"));
      oputs (_("\
  -b, --ఆపు-file=FILE          మాట_2 ఆపు characters లోపల this FILE\n\
"));
      oputs (_("\
  -f, --ignore-స్థితి              fold క్రింది స్థితి గమ్యం పై స్థితి కొరకు క్రమీకరణ\n\
"));
      oputs (_("\
  -g, --gap-కొలత=సంఖ్య_2          gap కొలత లోపల columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         పఠ ignore మాట_2 list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           పఠ only మాట_2 list from this FILE\n\
"));
      oputs (_("\
  -r, --references               మొదటి క్షేత్రం యొక్క each పంక్తి is a reference\n\
"));
      oputs (_("\
  -t, --typeset-బహుళకం_2             change the అప్రమేయ వెడల్పు from 72 గమ్యం 100\n\
"));
      oputs (_("\
  -w, --వెడల్పు=సంఖ్య_2             output వెడల్పు లోపల columns, reference excluded\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/*----------------------------------------------------------------------.
| Main program.  Decode ARGC arguments passed through the ARGV array of |
| strings, then launch execution.				        |
`----------------------------------------------------------------------*/

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"auto-reference", no_వాదం, ఖాళీ, 'A'},
  {"break-file", required_వాదం, ఖాళీ, 'b'},
  {"flag-truncation", required_వాదం, ఖాళీ, 'F'},
  {"ignore-case", no_వాదం, ఖాళీ, 'f'},
  {"gap-size", required_వాదం, ఖాళీ, 'g'},
  {"ignore-file", required_వాదం, ఖాళీ, 'i'},
  {"macro-name", required_వాదం, ఖాళీ, 'M'},
  {"only-file", required_వాదం, ఖాళీ, 'o'},
  {"references", no_వాదం, ఖాళీ, 'r'},
  {"right-side-refs", no_వాదం, ఖాళీ, 'R'},
  {"format", required_వాదం, ఖాళీ, FORMAT_OPTION},
  {"sentence-regexp", required_వాదం, ఖాళీ, 'S'},
  {"traditional", no_వాదం, ఖాళీ, 'G'},
  {"typeset-mode", no_వాదం, ఖాళీ, 't'},
  {"width", required_వాదం, ఖాళీ, 'w'},
  {"word-regexp", required_వాదం, ఖాళీ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0},
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి format_వాదనలు[] =
{
  "roff", "tex", ఖాళీ
};

స్థిర గణన Format స్థిరరాశి format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optchar;			/* argument character */
  పూర్ణాంకం file_సూచీ;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, ఖాళీ),
         optchar != EOF)
    {
      ఎంపిక (optchar)
        {
        అప్రమేయ:
          usage (EXIT_FAILURE);

        స్థితి 'G':
          gnu_extensions = false;
          ఆపు;

        స్థితి 'b':
          break_file = optarg;
          ఆపు;

        స్థితి 'f':
          ignore_case = true;
          ఆపు;

        స్థితి 'g':
          {
            intmax_t tmp;
            ఒకవేళ (! (xstrtoimax (optarg, ఖాళీ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_కొలత = tmp;
            ఆపు;
          }

        స్థితి 'i':
          ignore_file = optarg;
          ఆపు;

        స్థితి 'o':
          only_file = optarg;
          ఆపు;

        స్థితి 'r':
          ఆదానం_reference = true;
          ఆపు;

        స్థితి 't':
          ఒకవేళ (పంక్తి_వెడల్పు < 0)
            పంక్తి_వెడల్పు = 100;
          ఆపు;

        స్థితి 'w':
          {
            intmax_t tmp;
            ఒకవేళ (! (xstrtoimax (optarg, ఖాళీ, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            పంక్తి_వెడల్పు = tmp;
            ఆపు;
          }

        స్థితి 'A':
          auto_reference = true;
          ఆపు;

        స్థితి 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          ఆపు;

        స్థితి 'M':
          macro_name = optarg;
          ఆపు;

        స్థితి 'O':
          output_format = ROFF_FORMAT;
          ఆపు;

        స్థితి 'R':
          కుడి_reference = true;
          ఆపు;

        స్థితి 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          ఆపు;

        స్థితి 'T':
          output_format = TEX_FORMAT;
          ఆపు;

        స్థితి 'W':
          మాట_regex.string = optarg;
          unescape_string (optarg);
          ఒకవేళ (!*మాట_regex.string)
            మాట_regex.string = ఖాళీ;
          ఆపు;

        స్థితి FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_వాదనలు, format_vals);
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        }
    }

  ఒకవేళ (పంక్తి_వెడల్పు < 0)
    పంక్తి_వెడల్పు = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  ఒకవేళ (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      ఆదానం_file_name = xmalloc (పరిమాణం *ఆదానం_file_name);
      file_పంక్తి_లెక్క = xmalloc (పరిమాణం *file_పంక్తి_లెక్క);
      పాఠ్యం_buffers =    xmalloc (పరిమాణం *పాఠ్యం_buffers);
      సంఖ్య_ఆదానం_files = 1;
      ఆదానం_file_name[0] = ఖాళీ;
    }
  లేకపోతే ఒకవేళ (gnu_extensions)
    {
      సంఖ్య_ఆదానం_files = argc - optind;
      ఆదానం_file_name = xnmalloc (సంఖ్య_ఆదానం_files, పరిమాణం *ఆదానం_file_name);
      file_పంక్తి_లెక్క = xnmalloc (సంఖ్య_ఆదానం_files, పరిమాణం *file_పంక్తి_లెక్క);
      పాఠ్యం_buffers    = xnmalloc (సంఖ్య_ఆదానం_files, పరిమాణం *పాఠ్యం_buffers);

      కొరకు (file_సూచీ = 0; file_సూచీ < సంఖ్య_ఆదానం_files; file_సూచీ++)
        {
          ఒకవేళ (!*argv[optind] || streq (argv[optind], "-"))
            ఆదానం_file_name[file_సూచీ] = ఖాళీ;
          లేకపోతే
            ఆదానం_file_name[file_సూచీ] = argv[optind];
          optind++;
        }
    }
  లేకపోతే
    {

      /* There is one necessary input file.  */

      సంఖ్య_ఆదానం_files = 1;
      ఆదానం_file_name = xmalloc (పరిమాణం *ఆదానం_file_name);
      file_పంక్తి_లెక్క = xmalloc (పరిమాణం *file_పంక్తి_లెక్క);
      పాఠ్యం_buffers    = xmalloc (పరిమాణం *పాఠ్యం_buffers);
      ఒకవేళ (!*argv[optind] || streq (argv[optind], "-"))
        ఆదానం_file_name[0] = ఖాళీ;
      లేకపోతే
        ఆదానం_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      ఒకవేళ (optind < argc)
        {
          ఒకవేళ (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      ఒకవేళ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  ఒకవేళ (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  ఒకవేళ (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  ఒకవేళ (ignore_file)
    {
      digest_మాట_file (ignore_file, &ignore_పట్టిక);
      ఒకవేళ (ignore_పట్టిక.నిడివి == 0)
        ignore_file = ఖాళీ;
    }

  ఒకవేళ (only_file)
    {
      digest_మాట_file (only_file, &only_పట్టిక);
      ఒకవేళ (only_పట్టిక.నిడివి == 0)
        only_file = ఖాళీ;
    }

  /* Prepare to study all the input files.  */

  సంఖ్య_యొక్క_occurs[0] = 0;
  మొత్తం_పంక్తి_లెక్క = 0;
  గరిష్ఠ_మాట_నిడివి = 0;
  reference_max_వెడల్పు = 0;

  కొరకు (file_సూచీ = 0; file_సూచీ < సంఖ్య_ఆదానం_files; file_సూచీ++)
    {
      ఖండం_2 *పాఠ్యం_బఫరు = పాఠ్యం_buffers + file_సూచీ;

      /* Read the file contents into memory, then study it.  */

      swallow_file_లోపల_జ్ఞాపకం (ఆదానం_file_name[file_సూచీ], పాఠ్యం_బఫరు);
      కనుగొను_occurs_లోపల_పాఠ్యం (file_సూచీ);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      మొత్తం_పంక్తి_లెక్క++;
      file_పంక్తి_లెక్క[file_సూచీ] = మొత్తం_పంక్తి_లెక్క;
    }

  /* Do the output process phase.  */

  క్రమీకరించు_దొరికింది_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  తిరిగి EXIT_SUCCESS;
}
