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
#परिभाषय क्रमादेशः_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#परिभाषय AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#परिभाषय CHAR_समुच्चयः_आकार 256

/* Debugging the memory allocator.  */

#यदि WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#यदि_अन्तः

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
स्थिर bool निवेशः_reference = false;	/* refs at beginning of input lines */
स्थिर bool दक्षिणः_reference = false;	/* output refs after right context  */
स्थिर idx_t पङ्क्तिः_विस्तारः = -1;		/* output line width in characters */
स्थिर idx_t gap_आकार = 3;	/* number of spaces between output fields */
स्थिर वर्ण ध्रुव *truncation_string = "/";
                                /* string used to mark line truncations */
स्थिर वर्ण ध्रुव *macro_name = "xx";	/* macro name for roff or TeX output */
स्थिर गणना Format output_format = UNKNOWN_FORMAT;
                                /* output format */

स्थिर bool ignore_case = false;	/* fold lower to upper for sorting */
स्थिर वर्ण ध्रुव *break_file = रिक्त; /* name of the 'Break chars' file */
स्थिर वर्ण ध्रुव *only_file = रिक्त;	/* name of the 'Only words' file */
स्थिर वर्ण ध्रुव *ignore_file = रिक्त; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
संरचना regex_दत्त
{
  /* The original regular expression, as a string.  */
  वर्ण ध्रुव *string;

  /* The compiled regular expression, and its fastmap.  */
  संरचना re_आदर्शः_बफर आदर्शः_2;
  वर्ण fastmap[UCHAR_MAX + 1];
};

स्थिर संरचना regex_दत्त context_regex;	/* end of context */
स्थिर संरचना regex_दत्त word_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

प्रकारनिर्देश संरचना
  {
    वर्ण *start;		/* pointer to beginning of region */
    वर्ण *अन्तः;			/* pointer to end + 1 of region */
  }
खण्डम्_2;

प्रकारनिर्देश संरचना
  {
    वर्ण *start;		/* pointer to beginning of region */
    idx_t आकार;			/* length of the region */
  }
WORD;

प्रकारनिर्देश संरचना
  {
    idx_t alloc;		/* allocated length */
    idx_t दीर्घता;		/* number of used entries */
    WORD *start COUNTED_BY (दीर्घता);	/* array of WORDs */
  }
WORD_सारणी;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
स्थिर अचिह्नित वर्ण folded_chars[CHAR_समुच्चयः_आकार];

/* End of context pattern register indices.  */
स्थिर संरचना re_registers context_regs;

/* Keyword pattern register indices.  */
स्थिर संरचना re_registers word_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
स्थिर वर्ण word_fastmap[CHAR_समुच्चयः_आकार];

/* Maximum length of any word read.  */
स्थिर idx_t अधिकतमम्_word_दीर्घता;

/* Maximum width of any reference used.  */
स्थिर idx_t reference_max_विस्तारः;

/* Ignore and Only word tables.  */

स्थिर WORD_सारणी ignore_सारणी;	/* table of words to ignore */
स्थिर WORD_सारणी only_सारणी;		/* table of words to select */

/* Source text table, and scanning macros.  */

स्थिर पूर्णाङ्क सङ्ख्या_निवेशः_files;	/* number of text input files */
स्थिर intmax_t योग_पङ्क्तिः_गणनम्;	/* total number of lines seen so far */
स्थिर वर्ण ध्रुव **निवेशः_file_name;	/* array of text input file names */
स्थिर intmax_t *file_पङ्क्तिः_गणनम्;	/* array of line count values at end */

स्थिर खण्डम्_2 *text_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#परिभाषय SKIP_NON_WHITE(cursor, सीमा) \
  यावत् (cursor < सीमा && ! रिक्तम्_वा (गन्तव्यम्_uchar (*cursor)))		\
    cursor++

#परिभाषय SKIP_WHITE(cursor, सीमा) \
  यावत् (cursor < सीमा && रिक्तम्_वा (गन्तव्यम्_uchar (*cursor)))		\
    cursor++

#परिभाषय SKIP_WHITE_BACKWARDS(cursor, start) \
  यावत् (cursor > start && रिक्तम्_वा (गन्तव्यम्_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#परिभाषय SKIP_SOMETHING(cursor, सीमा) \
  यदि (word_regex.string)						\
    {									\
      regoff_t गणनम्;							\
      गणनम् = re_मेलनम् (&word_regex.आदर्शः_2, cursor, सीमा - cursor,	\
                        0, रिक्त);					\
      यदि (गणनम् == -2)							\
        matcher_error ();						\
      cursor += गणनम् <= 0 ? 1 : गणनम्;					\
    }									\
  अन्यथा यदि (word_fastmap[गन्तव्यम्_uchar (*cursor)])				\
    यावत् (cursor < सीमा && word_fastmap[गन्तव्यम्_uchar (*cursor)])		\
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
    WORD कुञ्चिका;			/* description of the keyword */
    ptrdiff_t वामः;		/* distance to left context start */
    ptrdiff_t दक्षिणः;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    पूर्णाङ्क file_सूचकांक;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

स्थिर OCCURS *occurs_सारणी[1];	/* all words retained from the read text */
स्थिर idx_t occurs_alloc[1];	/* allocated size of occurs_table */
स्थिर idx_t सङ्ख्या_सम्बन्धि_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
स्थिर वर्ण edited_flag[CHAR_समुच्चयः_आकार];

/* Half of line width, reference excluded.  */
स्थिर idx_t half_पङ्क्तिः_विस्तारः;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
स्थिर ptrdiff_t before_max_विस्तारः;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
स्थिर ptrdiff_t keyafter_max_विस्तारः;

/* Length of string that flags truncation.  */
स्थिर idx_t truncation_string_दीर्घता;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

स्थिर खण्डम्_2 पुच्छ;		/* tail field */
स्थिर bool पुच्छ_truncation;	/* flag truncation after the tail field */

स्थिर खण्डम्_2 before;		/* before field */
स्थिर bool before_truncation;	/* flag truncation before the before field */

स्थिर खण्डम्_2 keyafter;		/* keyword-and-after field */
स्थिर bool keyafter_truncation; /* flag truncation after the keyafter field */

स्थिर खण्डम्_2 शीर्ष;		/* head field */
स्थिर bool शीर्ष_truncation;	/* flag truncation before the head field */

स्थिर खण्डम्_2 reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

स्थिर _Noreturn शून्य
matcher_error (शून्य)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

स्थिर शून्य
unescape_string (वर्ण *string)
{
  वर्ण *cursor;			/* cursor in result */
  पूर्णाङ्क मानम्;			/* value of \nnn escape */
  पूर्णाङ्क दीर्घता;			/* length of \nnn escape */

  cursor = string;

  यावत् (*string)
    {
      यदि (*string == '\\')
        {
          string++;
          विकल्प (*string)
            {
            पक्ष 'x':		/* \xhhh escape, 3 chars maximum */
              मानम् = 0;
              प्रति (दीर्घता = 0, string++;
                   दीर्घता < 3 && c_isxdigit (*string);
                   दीर्घता++, string++)
                मानम् = मानम् * 16 + fromhex (*string);
              यदि (दीर्घता == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              अन्यथा
                *cursor++ = मानम्;
              भङ्ग;

            पक्ष '0':		/* \0ooo escape, 3 chars maximum */
              मानम् = 0;
              प्रति (दीर्घता = 0, string++;
                   दीर्घता < 3 && isoct (*string);
                   दीर्घता++, string++)
                मानम् = मानम् * 8 + fromoct (*string);
              *cursor++ = मानम्;
              भङ्ग;

            पक्ष 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              भङ्ग;

            पक्ष 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              भङ्ग;

            पक्ष 'c':		/* cancel the rest of the output */
              यावत् (*string)
                string++;
              भङ्ग;

            पक्ष 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              भङ्ग;

            पक्ष 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              भङ्ग;

            पक्ष 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              भङ्ग;

            पक्ष 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              भङ्ग;

            पक्ष 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              भङ्ग;

            पक्ष '\0':		/* lone backslash at end of string */
              /* ignore it */
              भङ्ग;

            स्वभाव:
              *cursor++ = '\\';
              *cursor++ = *string++;
              भङ्ग;
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
compile_regex (संरचना regex_दत्त *regex)
{
  संरचना re_आदर्शः_बफर *आदर्शः_2 = &regex->आदर्शः_2;
  वर्ण ध्रुव *string = regex->string;
  वर्ण ध्रुव *सन्देशः_2;

  आदर्शः_2->बफर = रिक्त;
  आदर्शः_2->allocated = 0;
  आदर्शः_2->fastmap = regex->fastmap;
  आदर्शः_2->translate = ignore_case ? folded_chars : रिक्त;

  सन्देशः_2 = re_compile_आदर्शः (string, वाक्यदैर्घ्यम् (string), आदर्शः_2);
  यदि (सन्देशः_2)
    error (EXIT_FAILURE, 0, _("%s (for regexp %s)"), सन्देशः_2, quote (string));

  /* The fastmap should be compiled before 're_match'.  The following
     call is not mandatory, because 're_search' is always called sooner,
     and it compiles the fastmap if this has not been done yet.  */

  re_compile_fastmap (आदर्शः_2);
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
    प्रति (पूर्णाङ्क character = 0; character < CHAR_समुच्चयः_आकार; character++)
      folded_chars[character] = महाक्षरम् (character);

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
  अन्यथा यदि (gnu_extensions && !निवेशः_reference)
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

          प्रति (पूर्णाङ्क character = 0; character < CHAR_समुच्चयः_आकार; character++)
            word_fastmap[character] = !! अक्षरम्_वा (character);
        }
      अन्यथा
        {

          /* Simulate [^ \t\n]+.  */

          स्मृतिं_पूरय (word_fastmap, 1, CHAR_समुच्चयः_आकार);
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
swallow_file_अन्तर्_स्मृतिः (वर्ण ध्रुव *file_name, खण्डम्_2 *खण्डम्)
{
  size_t प्रयुक्तम्_दीर्घता;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  यदि (using_stdin)
    खण्डम्->start = fread_file (stdin, 0, &प्रयुक्तम्_दीर्घता);
  अन्यथा
    खण्डम्->start = read_file (file_name, 0, &प्रयुक्तम्_दीर्घता);

  यदि (!खण्डम्->start)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  यदि (using_stdin)
    clearerr (stdin);

  खण्डम्->अन्तः = खण्डम्->start + प्रयुक्तम्_दीर्घता;
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
compare_शब्दाः (ध्रुव शून्य *void_first, ध्रुव शून्य *void_second)
{
  WORD ध्रुव *first = void_first;
  WORD ध्रुव *second = void_second;
  idx_t दीर्घता = MIN (first->आकार, second->आकार);

  यदि (ignore_case)
    {
      प्रति (idx_t गणक = 0; गणक < दीर्घता; गणक++)
        {
          पूर्णाङ्क मानम् = (folded_chars[गन्तव्यम्_uchar (first->start[गणक])]
                       - folded_chars[गन्तव्यम्_uchar (second->start[गणक])]);
          यदि (मानम् != 0)
            प्रत्यागम मानम्;
        }
    }
  अन्यथा
    {
      प्रति (idx_t गणक = 0; गणक < दीर्घता; गणक++)
        {
          पूर्णाङ्क मानम् = (गन्तव्यम्_uchar (first->start[गणक])
                       - गन्तव्यम्_uchar (second->start[गणक]));
          यदि (मानम् != 0)
            प्रत्यागम मानम्;
        }
    }

  प्रत्यागम _GL_CMP (first->आकार, second->आकार);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

स्थिर पूर्णाङ्क
compare_occurs (ध्रुव शून्य *void_first, ध्रुव शून्य *void_second)
{
#परिभाषय first ((ध्रुव OCCURS *) void_first)
#परिभाषय second ((ध्रुव OCCURS *) void_second)
  पूर्णाङ्क मानम्;

  मानम् = compare_शब्दाः (&first->कुञ्चिका, &second->कुञ्चिका);
  प्रत्यागम (मानम् ? मानम्
          : _GL_CMP (first->कुञ्चिका.start, second->कुञ्चिका.start));
#अपरिभाषय first
#अपरिभाषय second
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_विशुद्धम्
स्थिर bool
अन्वेषणम्_सारणी (WORD *word, WORD_सारणी *सारणी)
{
  idx_t lo = 0;
  idx_t hi = सारणी->दीर्घता;
  यावत् (lo < hi)
    {
      idx_t मध्यः = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      पूर्णाङ्क मानम् = compare_शब्दाः (word, सारणी->start + मध्यः);
      यदि (मानम् < 0)
        hi = मध्यः;
      अन्यथा यदि (मानम् > 0)
        lo = मध्यः + 1;
      अन्यथा
        प्रत्यागम true;
    }
  प्रत्यागम false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

स्थिर शून्य
क्रमय_प्राप्तम्_occurs (शून्य)
{

  /* Only one language for the time being.  */
  यदि (सङ्ख्या_सम्बन्धि_occurs[0])
    शीघ्रं_क्रमय (occurs_सारणी[0], सङ्ख्या_सम्बन्धि_occurs[0], परिमाण **occurs_सारणी,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

स्थिर शून्य
digest_break_file (वर्ण ध्रुव *file_name)
{
  खण्डम्_2 file_contents;		/* to receive a copy of the file */

  swallow_file_अन्तर्_स्मृतिः (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  स्मृतिं_पूरय (word_fastmap, 1, CHAR_समुच्चयः_आकार);
  प्रति (वर्ण *cursor = file_contents.start; cursor < file_contents.अन्तः;
       cursor++)
    word_fastmap[गन्तव्यम्_uchar (*cursor)] = 0;

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
digest_word_file (वर्ण ध्रुव *file_name, WORD_सारणी *सारणी)
{
  खण्डम्_2 file_contents;		/* to receive a copy of the file */
  वर्ण *cursor;			/* cursor in file copy */
  वर्ण *word_start;		/* start of the current word */

  swallow_file_अन्तर्_स्मृतिः (file_name, &file_contents);

  सारणी->start = रिक्त;
  सारणी->alloc = 0;
  सारणी->दीर्घता = 0;

  /* Read the whole file.  */

  cursor = file_contents.start;
  यावत् (cursor < file_contents.अन्तः)
    {

      /* Read one line, and save the word in contains.  */

      word_start = cursor;
      यावत् (cursor < file_contents.अन्तः && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      यदि (cursor > word_start)
        {
          यदि (सारणी->दीर्घता == सारणी->alloc)
            सारणी->start = xpalloc (सारणी->start, &सारणी->alloc, 1, -1,
                                    परिमाण *सारणी->start);
          idx_t ध्रुव सारणी_idx = सारणी->दीर्घता++;
          सारणी->start[सारणी_idx].start = word_start;
          सारणी->start[सारणी_idx].आकार = cursor - word_start;
        }

      /* This test allows for an incomplete line at end of file.  */

      यदि (cursor < file_contents.अन्तः)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  शीघ्रं_क्रमय (सारणी->start, सारणी->दीर्घता, परिमाण सारणी->start[0], compare_शब्दाः);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

स्थिर शून्य
find_occurs_अन्तर्_text (पूर्णाङ्क file_सूचकांक)
{
  वर्ण *अवलोकनम्;			/* for scanning the source text */
  वर्ण *पङ्क्तिः_start;		/* start of the current input line */
  वर्ण *पङ्क्तिः_अवलोकनम्;		/* newlines scanned until this point */
  idx_t reference_दीर्घता;	/* length of reference in input mode */
  WORD possible_कुञ्चिका;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  वर्ण *context_start;		/* start of left context */
  वर्ण *context_अन्तः;		/* end of right context */
  वर्ण *word_start;		/* start of word */
  वर्ण *word_अन्तः;		/* end of word */
  वर्ण *अग्र_context_start;	/* next start of left context */

  ध्रुव खण्डम्_2 *text_बफर = &text_buffers[file_सूचकांक];

  /* reference_length is always used within 'if (input_reference)'.
     However, GNU C diagnoses that it may be used uninitialized.  The
     following assignment is merely to shut it up.  */

  reference_दीर्घता = 0;

  /* Tracking where lines start is helpful for reference processing.  In
     auto reference mode, this allows counting lines.  In input reference
     mode, this permits finding the beginning of the references.

     The first line begins with the file, skip immediately this very first
     reference in input reference mode, to help further rejection any word
     found inside it.  Also, unconditionally assigning these variable has
     the happy effect of shutting up lint.  */

  पङ्क्तिः_start = text_बफर->start;
  पङ्क्तिः_अवलोकनम् = पङ्क्तिः_start;
  यदि (निवेशः_reference)
    {
      SKIP_NON_WHITE (पङ्क्तिः_अवलोकनम्, text_बफर->अन्तः);
      reference_दीर्घता = पङ्क्तिः_अवलोकनम् - पङ्क्तिः_start;
      SKIP_WHITE (पङ्क्तिः_अवलोकनम्, text_बफर->अन्तः);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  प्रति (वर्ण *cursor = text_बफर->start;
       cursor < text_बफर->अन्तः;
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

      अग्र_context_start = text_बफर->अन्तः;
      यदि (context_regex.string)
        विकल्प (re_अन्वेषणम् (&context_regex.आदर्शः_2, cursor,
                           text_बफर->अन्तः - cursor,
                           0, text_बफर->अन्तः - cursor, &context_regs))
          {
          पक्ष -2:
            matcher_error ();

          पक्ष -1:
            भङ्ग;

          पक्ष 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          स्वभाव:
            अग्र_context_start = cursor + context_regs.अन्तः[0];
            भङ्ग;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_अन्तः = अग्र_context_start;
      SKIP_WHITE_BACKWARDS (context_अन्तः, context_start);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      यावत् (true)
        {
          यदि (word_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_अन्वेषणम् (&word_regex.आदर्शः_2, cursor,
                                      context_अन्तः - cursor,
                                      0, context_अन्तः - cursor, &word_regs);
              यदि (r == -2)
                matcher_error ();
              यदि (r == -1)
                भङ्ग;
              word_start = cursor + word_regs.start[0];
              word_अन्तः = cursor + word_regs.अन्तः[0];
            }
          अन्यथा

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              अवलोकनम् = cursor;
              यावत् (अवलोकनम् < context_अन्तः
                     && !word_fastmap[गन्तव्यम्_uchar (*अवलोकनम्)])
                अवलोकनम्++;

              यदि (अवलोकनम् == context_अन्तः)
                भङ्ग;

              word_start = अवलोकनम्;

              यावत् (अवलोकनम् < context_अन्तः
                     && word_fastmap[गन्तव्यम्_uchar (*अवलोकनम्)])
                अवलोकनम्++;

              word_अन्तः = अवलोकनम्;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = word_start;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          यदि (word_अन्तः == word_start)
            {
              cursor++;
              अनुवर्त;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_कुञ्चिका.start = cursor;
          possible_कुञ्चिका.आकार = word_अन्तः - word_start;
          cursor += possible_कुञ्चिका.आकार;

          यदि (possible_कुञ्चिका.आकार > अधिकतमम्_word_दीर्घता)
            अधिकतमम्_word_दीर्घता = possible_कुञ्चिका.आकार;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          यदि (निवेशः_reference)
            {
              यावत् (पङ्क्तिः_अवलोकनम् < possible_कुञ्चिका.start)
                यदि (*पङ्क्तिः_अवलोकनम् == '\n')
                  {
                    योग_पङ्क्तिः_गणनम्++;
                    पङ्क्तिः_अवलोकनम्++;
                    पङ्क्तिः_start = पङ्क्तिः_अवलोकनम्;
                    SKIP_NON_WHITE (पङ्क्तिः_अवलोकनम्, text_बफर->अन्तः);
                    reference_दीर्घता = पङ्क्तिः_अवलोकनम् - पङ्क्तिः_start;
                  }
                अन्यथा
                  पङ्क्तिः_अवलोकनम्++;
              यदि (पङ्क्तिः_अवलोकनम् > possible_कुञ्चिका.start)
                अनुवर्त;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          यदि (ignore_file && अन्वेषणम्_सारणी (&possible_कुञ्चिका, &ignore_सारणी))
            अनुवर्त;
          यदि (only_file && !अन्वेषणम्_सारणी (&possible_कुञ्चिका, &only_सारणी))
            अनुवर्त;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          यदि (सङ्ख्या_सम्बन्धि_occurs[0] == occurs_alloc[0])
            occurs_सारणी[0] = xpalloc (occurs_सारणी[0], &occurs_alloc[0],
                                       1, -1, परिमाण *occurs_सारणी[0]);
          occurs_cursor = occurs_सारणी[0] + सङ्ख्या_सम्बन्धि_occurs[0];

          /* Define the reference field, if any.  */

          यदि (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              यावत् (पङ्क्तिः_अवलोकनम् < possible_कुञ्चिका.start)
                यदि (*पङ्क्तिः_अवलोकनम् == '\n')
                  {
                    योग_पङ्क्तिः_गणनम्++;
                    पङ्क्तिः_अवलोकनम्++;
                    पङ्क्तिः_start = पङ्क्तिः_अवलोकनम्;
                    SKIP_NON_WHITE (पङ्क्तिः_अवलोकनम्, text_बफर->अन्तः);
                  }
                अन्यथा
                  पङ्क्तिः_अवलोकनम्++;

              occurs_cursor->reference = योग_पङ्क्तिः_गणनम्;
            }
          अन्यथा यदि (निवेशः_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = पङ्क्तिः_start - possible_कुञ्चिका.start;
              यदि (reference_दीर्घता > reference_max_विस्तारः)
                reference_max_विस्तारः = reference_दीर्घता;
            }

          /* Exclude the reference from the context in simple cases.  */

          यदि (निवेशः_reference && पङ्क्तिः_start == context_start)
            {
              SKIP_NON_WHITE (context_start, context_अन्तः);
              SKIP_WHITE (context_start, context_अन्तः);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->कुञ्चिका = possible_कुञ्चिका;
          occurs_cursor->वामः = context_start - possible_कुञ्चिका.start;
          occurs_cursor->दक्षिणः = context_अन्तः - possible_कुञ्चिका.start;
          occurs_cursor->file_सूचकांक = file_सूचकांक;

          सङ्ख्या_सम्बन्धि_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

स्थिर शून्य
print_spaces (ptrdiff_t सङ्ख्या)
{
  प्रति (ptrdiff_t गणक = सङ्ख्या; गणक > 0; गणक--)
    अक्षरं_लिख (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

स्थिर शून्य
print_क्षेत्रम् (खण्डम्_2 क्षेत्रम्)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  प्रति (वर्ण *cursor = क्षेत्रम्.start; cursor < क्षेत्रम्.अन्तः; cursor++)
    {
      अचिह्नित वर्ण character = *cursor;
      यदि (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          विकल्प (character)
            {
            पक्ष '"':
              /* In roff output format, double any quote.  */
              अक्षरं_लिख ('"');
              अक्षरं_लिख ('"');
              भङ्ग;

            पक्ष '$':
            पक्ष '%':
            पक्ष '&':
            पक्ष '#':
            पक्ष '_':
              /* In TeX output format, precede these with a backslash.  */
              अक्षरं_लिख ('\\');
              अक्षरं_लिख (character);
              भङ्ग;

            पक्ष '{':
            पक्ष '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              प्रारूप_लिख ("$\\%c$", character);
              भङ्ग;

            पक्ष '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              भङ्ग;

            स्वभाव:
              /* Any other flagged character produces a single space.  */
              अक्षरं_लिख (' ');
            }
        }
      अन्यथा
        अक्षरं_लिख (*cursor);
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
      reference_max_विस्तारः = 0;
      प्रति (पूर्णाङ्क file_सूचकांक = 0; file_सूचकांक < सङ्ख्या_निवेशः_files; file_सूचकांक++)
        {
          intmax_t पङ्क्तिः_ordinal = file_पङ्क्तिः_गणनम्[file_सूचकांक] + 1;
          यदि (file_सूचकांक > 0)
            पङ्क्तिः_ordinal -= file_पङ्क्तिः_गणनम्[file_सूचकांक - 1];
          वर्ण ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_विस्तारः = sprintf (ordinal_string, "%jd", पङ्क्तिः_ordinal);
          यदि (निवेशः_file_name[file_सूचकांक])
            reference_विस्तारः += वाक्यदैर्घ्यम् (निवेशः_file_name[file_सूचकांक]);
          यदि (reference_विस्तारः > reference_max_विस्तारः)
            reference_max_विस्तारः = reference_विस्तारः;
        }
      reference_max_विस्तारः++;
      reference.start = xmalloc (reference_max_विस्तारः + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  यदि ((auto_reference || निवेशः_reference) && !दक्षिणः_reference)
    पङ्क्तिः_विस्तारः = MAX (0, पङ्क्तिः_विस्तारः - (reference_max_विस्तारः + gap_आकार));

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

  half_पङ्क्तिः_विस्तारः = पङ्क्तिः_विस्तारः >> 1;
  before_max_विस्तारः = half_पङ्क्तिः_विस्तारः - gap_आकार;
  keyafter_max_विस्तारः = half_पङ्क्तिः_विस्तारः;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  यदि (truncation_string && *truncation_string)
    truncation_string_दीर्घता = वाक्यदैर्घ्यम् (truncation_string);
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

      before_max_विस्तारः -= 2 * truncation_string_दीर्घता;
      keyafter_max_विस्तारः -= 2 * truncation_string_दीर्घता;
    }
  अन्यथा
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_विस्तारः -= 2 * truncation_string_दीर्घता + 1;
    }

  यदि (before_max_विस्तारः < 0)
    before_max_विस्तारः = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  प्रति (पूर्णाङ्क character = 0; character < CHAR_समुच्चयः_आकार; character++)
    edited_flag[character] = !! रिक्तम्_वा (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  विकल्प (output_format)
    {
    पक्ष UNKNOWN_FORMAT:
      /* Should never happen.  */

    पक्ष DUMB_FORMAT:
      भङ्ग;

    पक्ष ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      भङ्ग;

    पक्ष TEX_FORMAT:

      /* Various characters need special processing.  */

      प्रति (वर्ण ध्रुव *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[गन्तव्यम्_uchar (*cursor)] = 1;

      भङ्ग;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

स्थिर शून्य
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t पुच्छ_max_विस्तारः;	/* allowable width of tail field */
  ptrdiff_t शीर्ष_max_विस्तारः;	/* allowable width of head field */
  वर्ण *cursor;			/* running cursor in source text */
  वर्ण *वामः_context_start;	/* start of left context */
  वर्ण *दक्षिणः_context_अन्तः;	/* end of right context */
  वर्ण *वामः_क्षेत्रम्_start;	/* conservative start for 'head'/'before' */
  वर्ण ध्रुव *file_name;	/* file name for reference */
  intmax_t पङ्क्तिः_ordinal;	/* line ordinal for reference */
  वर्ण ध्रुव *बफर_start;	/* start of buffered file for this occurs */
  वर्ण ध्रुव *बफर_अन्तः;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.start = occurs->कुञ्चिका.start;
  keyafter.अन्तः = keyafter.start + occurs->कुञ्चिका.आकार;
  वामः_context_start = keyafter.start + occurs->वामः;
  दक्षिणः_context_अन्तः = keyafter.start + occurs->दक्षिणः;

  बफर_start = text_buffers[occurs->file_सूचकांक].start;
  बफर_अन्तः = text_buffers[occurs->file_सूचकांक].अन्तः;

  cursor = keyafter.अन्तः;
  यावत् (cursor < दक्षिणः_context_अन्तः
         && cursor <= keyafter.start + keyafter_max_विस्तारः)
    {
      keyafter.अन्तः = cursor;
      SKIP_SOMETHING (cursor, दक्षिणः_context_अन्तः);
    }
  यदि (cursor <= keyafter.start + keyafter_max_विस्तारः)
    keyafter.अन्तः = cursor;

  keyafter_truncation = truncation_string && keyafter.अन्तः < दक्षिणः_context_अन्तः;

  SKIP_WHITE_BACKWARDS (keyafter.अन्तः, keyafter.start);

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

  यदि (-occurs->वामः > half_पङ्क्तिः_विस्तारः + अधिकतमम्_word_दीर्घता)
    {
      वामः_क्षेत्रम्_start
        = keyafter.start - (half_पङ्क्तिः_विस्तारः + अधिकतमम्_word_दीर्घता);
      SKIP_SOMETHING (वामः_क्षेत्रम्_start, keyafter.start);
    }
  अन्यथा
    वामः_क्षेत्रम्_start = keyafter.start + occurs->वामः;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.start = वामः_क्षेत्रम्_start;
  before.अन्तः = keyafter.start;
  SKIP_WHITE_BACKWARDS (before.अन्तः, before.start);

  यावत् (before.start + before_max_विस्तारः < before.अन्तः)
    SKIP_SOMETHING (before.start, before.अन्तः);

  यदि (truncation_string)
    {
      cursor = before.start;
      SKIP_WHITE_BACKWARDS (cursor, बफर_start);
      before_truncation = cursor > वामः_context_start;
    }
  अन्यथा
    before_truncation = false;

  SKIP_WHITE (before.start, बफर_अन्तः);

  /* The tail could not take more columns than what has been left in the
     left context field, and a gap is mandatory.  It starts after the
     right context, and does not contain prefixed spaces.  It ends at
     the end of line, the end of buffer or when the tail field is full,
     whichever comes first.  It cannot contain only part of a word, and
     has no suffixed spaces.  */

  पुच्छ_max_विस्तारः
    = before_max_विस्तारः - (before.अन्तः - before.start) - gap_आकार;

  यदि (पुच्छ_max_विस्तारः > 0)
    {
      पुच्छ.start = keyafter.अन्तः;
      SKIP_WHITE (पुच्छ.start, बफर_अन्तः);

      पुच्छ.अन्तः = पुच्छ.start;
      cursor = पुच्छ.अन्तः;
      यावत् (cursor < दक्षिणः_context_अन्तः
             && cursor < पुच्छ.start + पुच्छ_max_विस्तारः)
        {
          पुच्छ.अन्तः = cursor;
          SKIP_SOMETHING (cursor, दक्षिणः_context_अन्तः);
        }

      यदि (cursor < पुच्छ.start + पुच्छ_max_विस्तारः)
        पुच्छ.अन्तः = cursor;

      यदि (पुच्छ.अन्तः > पुच्छ.start)
        {
          keyafter_truncation = false;
          पुच्छ_truncation = truncation_string && पुच्छ.अन्तः < दक्षिणः_context_अन्तः;
        }
      अन्यथा
        पुच्छ_truncation = false;

      SKIP_WHITE_BACKWARDS (पुच्छ.अन्तः, पुच्छ.start);
    }
  अन्यथा
    {

      /* No place left for a tail field.  */

      पुच्छ.start = रिक्त;
      पुच्छ.अन्तः = रिक्त;
      पुच्छ_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  शीर्ष_max_विस्तारः
    = keyafter_max_विस्तारः - (keyafter.अन्तः - keyafter.start) - gap_आकार;

  यदि (शीर्ष_max_विस्तारः > 0)
    {
      शीर्ष.अन्तः = before.start;
      SKIP_WHITE_BACKWARDS (शीर्ष.अन्तः, बफर_start);

      शीर्ष.start = वामः_क्षेत्रम्_start;
      यावत् (शीर्ष.start + शीर्ष_max_विस्तारः < शीर्ष.अन्तः)
        SKIP_SOMETHING (शीर्ष.start, शीर्ष.अन्तः);

      यदि (शीर्ष.अन्तः > शीर्ष.start)
        {
          before_truncation = false;
          शीर्ष_truncation = (truncation_string
                             && शीर्ष.start > वामः_context_start);
        }
      अन्यथा
        शीर्ष_truncation = false;

      SKIP_WHITE (शीर्ष.start, शीर्ष.अन्तः);
    }
  अन्यथा
    {

      /* No place left for a head field.  */

      शीर्ष.start = रिक्त;
      शीर्ष.अन्तः = रिक्त;
      शीर्ष_truncation = false;
    }

  यदि (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = निवेशः_file_name[occurs->file_सूचकांक];
      यदि (!file_name)
        file_name = "";

      पङ्क्तिः_ordinal = occurs->reference + 1;
      यदि (occurs->file_सूचकांक > 0)
        पङ्क्तिः_ordinal -= file_पङ्क्तिः_गणनम्[occurs->file_सूचकांक - 1];

      वर्ण *file_अन्तः = stpcpy (reference.start, file_name);
      reference.अन्तः = file_अन्तः + sprintf (file_अन्तः, ":%jd", पङ्क्तिः_ordinal);
    }
  अन्यथा यदि (निवेशः_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.start = keyafter.start + occurs->reference;
      reference.अन्तः = reference.start;
      SKIP_NON_WHITE (reference.अन्तः, दक्षिणः_context_अन्तः);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

स्थिर शून्य
output_one_roff_पङ्क्तिः (शून्य)
{
  /* Output the 'tail' field.  */

  प्रारूप_लिख (".%s \"", macro_name);
  print_क्षेत्रम् (पुच्छ);
  यदि (पुच्छ_truncation)
    fputs (truncation_string, stdout);
  अक्षरं_लिख ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  यदि (before_truncation)
    fputs (truncation_string, stdout);
  print_क्षेत्रम् (before);
  अक्षरं_लिख ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_क्षेत्रम् (keyafter);
  यदि (keyafter_truncation)
    fputs (truncation_string, stdout);
  अक्षरं_लिख ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  यदि (शीर्ष_truncation)
    fputs (truncation_string, stdout);
  print_क्षेत्रम् (शीर्ष);
  अक्षरं_लिख ('"');

  /* Conditionally output the 'reference' field.  */

  यदि (auto_reference || निवेशः_reference)
    {
      fputs (" \"", stdout);
      print_क्षेत्रम् (reference);
      अक्षरं_लिख ('"');
    }

  अक्षरं_लिख ('\n');
}

/*---------------------------------------------------------.
| Output the current output fields as one line for 'TeX'.  |
`---------------------------------------------------------*/

स्थिर शून्य
output_one_tex_पङ्क्तिः (शून्य)
{
  खण्डम्_2 कुञ्चिका;			/* key field, isolated */
  खण्डम्_2 after;			/* after field, isolated */
  वर्ण *cursor;			/* running cursor in source text */

  प्रारूप_लिख ("\\%s ", macro_name);
  अक्षरं_लिख ('{');
  print_क्षेत्रम् (पुच्छ);
  fputs ("}{", stdout);
  print_क्षेत्रम् (before);
  fputs ("}{", stdout);
  कुञ्चिका.start = keyafter.start;
  after.अन्तः = keyafter.अन्तः;
  cursor = keyafter.start;
  SKIP_SOMETHING (cursor, keyafter.अन्तः);
  कुञ्चिका.अन्तः = cursor;
  after.start = cursor;
  print_क्षेत्रम् (कुञ्चिका);
  fputs ("}{", stdout);
  print_क्षेत्रम् (after);
  fputs ("}{", stdout);
  print_क्षेत्रम् (शीर्ष);
  अक्षरं_लिख ('}');
  यदि (auto_reference || निवेशः_reference)
    {
      अक्षरं_लिख ('{');
      print_क्षेत्रम् (reference);
      अक्षरं_लिख ('}');
    }
  अक्षरं_लिख ('\n');
}

/*-------------------------------------------------------------------.
| Output the current output fields as one line for a dumb terminal.  |
`-------------------------------------------------------------------*/

स्थिर शून्य
output_one_dumb_पङ्क्तिः (शून्य)
{
  यदि (!दक्षिणः_reference)
    {
      यदि (auto_reference)
        {

          /* Output the 'reference' field, in such a way that GNU emacs
             next-error will handle it.  The ending colon is taken from the
             gap which follows.  */

          print_क्षेत्रम् (reference);
          अक्षरं_लिख (':');
          print_spaces (reference_max_विस्तारः
                        + gap_आकार
                        - (reference.अन्तः - reference.start)
                        - 1);
        }
      अन्यथा
        {

          /* Output the 'reference' field and its following gap.  */

          print_क्षेत्रम् (reference);
          print_spaces (reference_max_विस्तारः
                        + gap_आकार
                        - (reference.अन्तः - reference.start));
        }
    }

  यदि (पुच्छ.start < पुच्छ.अन्तः)
    {
      /* Output the 'tail' field.  */

      print_क्षेत्रम् (पुच्छ);
      यदि (पुच्छ_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_पङ्क्तिः_विस्तारः - gap_आकार
                    - (before.अन्तः - before.start)
                    - (before_truncation ? truncation_string_दीर्घता : 0)
                    - (पुच्छ.अन्तः - पुच्छ.start)
                    - (पुच्छ_truncation ? truncation_string_दीर्घता : 0));
    }
  अन्यथा
    print_spaces (half_पङ्क्तिः_विस्तारः - gap_आकार
                  - (before.अन्तः - before.start)
                  - (before_truncation ? truncation_string_दीर्घता : 0));

  /* Output the 'before' field.  */

  यदि (before_truncation)
    fputs (truncation_string, stdout);
  print_क्षेत्रम् (before);

  print_spaces (gap_आकार);

  /* Output the 'keyafter' field.  */

  print_क्षेत्रम् (keyafter);
  यदि (keyafter_truncation)
    fputs (truncation_string, stdout);

  यदि (शीर्ष.start < शीर्ष.अन्तः)
    {
      /* Output the 'head' field.  */

      print_spaces (half_पङ्क्तिः_विस्तारः
                    - (keyafter.अन्तः - keyafter.start)
                    - (keyafter_truncation ? truncation_string_दीर्घता : 0)
                    - (शीर्ष.अन्तः - शीर्ष.start)
                    - (शीर्ष_truncation ? truncation_string_दीर्घता : 0));
      यदि (शीर्ष_truncation)
        fputs (truncation_string, stdout);
      print_क्षेत्रम् (शीर्ष);
    }
  अन्यथा

    यदि ((auto_reference || निवेशः_reference) && दक्षिणः_reference)
      print_spaces (half_पङ्क्तिः_विस्तारः
                    - (keyafter.अन्तः - keyafter.start)
                    - (keyafter_truncation ? truncation_string_दीर्घता : 0));

  यदि ((auto_reference || निवेशः_reference) && दक्षिणः_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_आकार);
      print_क्षेत्रम् (reference);
    }

  अक्षरं_लिख ('\n');
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

  पुच्छ.start = रिक्त;
  पुच्छ.अन्तः = रिक्त;
  पुच्छ_truncation = false;

  शीर्ष.start = रिक्त;
  शीर्ष.अन्तः = रिक्त;
  शीर्ष_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_सारणी[0];

  प्रति (idx_t occurs_सूचकांक = 0; occurs_सूचकांक < सङ्ख्या_सम्बन्धि_occurs[0];
       occurs_सूचकांक++)
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
          output_one_dumb_पङ्क्तिः ();
          भङ्ग;

        पक्ष ROFF_FORMAT:
          output_one_roff_पङ्क्तिः ();
          भङ्ग;

        पक्ष TEX_FORMAT:
          output_one_tex_पङ्क्तिः ();
          भङ्ग;
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
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [निवेशः_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [निवेशः_2 [OUTPUT]]\n"),
              क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
Output a permuted index, including context, सम्बन्धि the शब्दाः अन्तर् the निवेशः files.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --स्वचालित-reference           output automatically generated references\n\
"));
      oputs (_("\
  -G, --traditional              behave अधिकम् like System V 'ptx'\n\
"));
      oputs (_("\
  -F, --flag-truncation=STRING   use STRING प्रति flagging पङ्क्तिः truncations.\n\
                                 The स्वभाव is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name गन्तव्यम् use instead सम्बन्धि 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --दक्षिणः-side-refs          put references at दक्षिणः, not counted अन्तर् -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   प्रति अन्तः सम्बन्धि पङ्क्तयः or अन्तः सम्बन्धि sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --word-regexp=REGEXP       use REGEXP गन्तव्यम् मेलनम् each keyword\n\
"));
      oputs (_("\
  -b, --भङ्ग-file=FILE          word भङ्ग characters अन्तर् this FILE\n\
"));
      oputs (_("\
  -f, --ignore-पक्ष              fold अधस्तनम् पक्ष गन्तव्यम् ऊर्ध्वस्थम् पक्ष प्रति क्रमणम्\n\
"));
      oputs (_("\
  -g, --gap-आकार=सङ्ख्या_2          gap आकार अन्तर् columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         पठ ignore word list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           पठ only word list from this FILE\n\
"));
      oputs (_("\
  -r, --references               first क्षेत्रम् सम्बन्धि each पङ्क्तिः is a reference\n\
"));
      oputs (_("\
  -t, --typeset-बहुलकम्_2             change the स्वभाव विस्तारः from 72 गन्तव्यम् 100\n\
"));
      oputs (_("\
  -w, --विस्तारः=सङ्ख्या_2             output विस्तारः अन्तर् columns, reference excluded\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/*----------------------------------------------------------------------.
| Main program.  Decode ARGC arguments passed through the ARGV array of |
| strings, then launch execution.				        |
`----------------------------------------------------------------------*/

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
स्थिर संरचना option ध्रुव long_options[] =
{
  {"auto-reference", no_तर्कः, रिक्त, 'A'},
  {"break-file", required_तर्कः, रिक्त, 'b'},
  {"flag-truncation", required_तर्कः, रिक्त, 'F'},
  {"ignore-case", no_तर्कः, रिक्त, 'f'},
  {"gap-size", required_तर्कः, रिक्त, 'g'},
  {"ignore-file", required_तर्कः, रिक्त, 'i'},
  {"macro-name", required_तर्कः, रिक्त, 'M'},
  {"only-file", required_तर्कः, रिक्त, 'o'},
  {"references", no_तर्कः, रिक्त, 'r'},
  {"right-side-refs", no_तर्कः, रिक्त, 'R'},
  {"format", required_तर्कः, रिक्त, FORMAT_OPTION},
  {"sentence-regexp", required_तर्कः, रिक्त, 'S'},
  {"traditional", no_तर्कः, रिक्त, 'G'},
  {"typeset-mode", no_तर्कः, रिक्त, 't'},
  {"width", required_तर्कः, रिक्त, 'w'},
  {"word-regexp", required_तर्कः, रिक्त, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0},
};

स्थिर वर्ण ध्रुव *ध्रुव format_तर्काः[] =
{
  "roff", "tex", रिक्त
};

स्थिर गणना Format ध्रुव format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optchar;			/* argument character */
  पूर्णाङ्क file_सूचकांक;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, रिक्त),
         optchar != EOF)
    {
      विकल्प (optchar)
        {
        स्वभाव:
          usage (EXIT_FAILURE);

        पक्ष 'G':
          gnu_extensions = false;
          भङ्ग;

        पक्ष 'b':
          break_file = optarg;
          भङ्ग;

        पक्ष 'f':
          ignore_case = true;
          भङ्ग;

        पक्ष 'g':
          {
            intmax_t tmp;
            यदि (! (xstrtoimax (optarg, रिक्त, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_आकार = tmp;
            भङ्ग;
          }

        पक्ष 'i':
          ignore_file = optarg;
          भङ्ग;

        पक्ष 'o':
          only_file = optarg;
          भङ्ग;

        पक्ष 'r':
          निवेशः_reference = true;
          भङ्ग;

        पक्ष 't':
          यदि (पङ्क्तिः_विस्तारः < 0)
            पङ्क्तिः_विस्तारः = 100;
          भङ्ग;

        पक्ष 'w':
          {
            intmax_t tmp;
            यदि (! (xstrtoimax (optarg, रिक्त, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            पङ्क्तिः_विस्तारः = tmp;
            भङ्ग;
          }

        पक्ष 'A':
          auto_reference = true;
          भङ्ग;

        पक्ष 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          भङ्ग;

        पक्ष 'M':
          macro_name = optarg;
          भङ्ग;

        पक्ष 'O':
          output_format = ROFF_FORMAT;
          भङ्ग;

        पक्ष 'R':
          दक्षिणः_reference = true;
          भङ्ग;

        पक्ष 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          भङ्ग;

        पक्ष 'T':
          output_format = TEX_FORMAT;
          भङ्ग;

        पक्ष 'W':
          word_regex.string = optarg;
          unescape_string (optarg);
          यदि (!*word_regex.string)
            word_regex.string = रिक्त;
          भङ्ग;

        पक्ष FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_तर्काः, format_vals);
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        }
    }

  यदि (पङ्क्तिः_विस्तारः < 0)
    पङ्क्तिः_विस्तारः = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  यदि (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      निवेशः_file_name = xmalloc (परिमाण *निवेशः_file_name);
      file_पङ्क्तिः_गणनम् = xmalloc (परिमाण *file_पङ्क्तिः_गणनम्);
      text_buffers =    xmalloc (परिमाण *text_buffers);
      सङ्ख्या_निवेशः_files = 1;
      निवेशः_file_name[0] = रिक्त;
    }
  अन्यथा यदि (gnu_extensions)
    {
      सङ्ख्या_निवेशः_files = argc - optind;
      निवेशः_file_name = xnmalloc (सङ्ख्या_निवेशः_files, परिमाण *निवेशः_file_name);
      file_पङ्क्तिः_गणनम् = xnmalloc (सङ्ख्या_निवेशः_files, परिमाण *file_पङ्क्तिः_गणनम्);
      text_buffers    = xnmalloc (सङ्ख्या_निवेशः_files, परिमाण *text_buffers);

      प्रति (file_सूचकांक = 0; file_सूचकांक < सङ्ख्या_निवेशः_files; file_सूचकांक++)
        {
          यदि (!*argv[optind] || streq (argv[optind], "-"))
            निवेशः_file_name[file_सूचकांक] = रिक्त;
          अन्यथा
            निवेशः_file_name[file_सूचकांक] = argv[optind];
          optind++;
        }
    }
  अन्यथा
    {

      /* There is one necessary input file.  */

      सङ्ख्या_निवेशः_files = 1;
      निवेशः_file_name = xmalloc (परिमाण *निवेशः_file_name);
      file_पङ्क्तिः_गणनम् = xmalloc (परिमाण *file_पङ्क्तिः_गणनम्);
      text_buffers    = xmalloc (परिमाण *text_buffers);
      यदि (!*argv[optind] || streq (argv[optind], "-"))
        निवेशः_file_name[0] = रिक्त;
      अन्यथा
        निवेशः_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      यदि (optind < argc)
        {
          यदि (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      यदि (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
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
      digest_word_file (ignore_file, &ignore_सारणी);
      यदि (ignore_सारणी.दीर्घता == 0)
        ignore_file = रिक्त;
    }

  यदि (only_file)
    {
      digest_word_file (only_file, &only_सारणी);
      यदि (only_सारणी.दीर्घता == 0)
        only_file = रिक्त;
    }

  /* Prepare to study all the input files.  */

  सङ्ख्या_सम्बन्धि_occurs[0] = 0;
  योग_पङ्क्तिः_गणनम् = 0;
  अधिकतमम्_word_दीर्घता = 0;
  reference_max_विस्तारः = 0;

  प्रति (file_सूचकांक = 0; file_सूचकांक < सङ्ख्या_निवेशः_files; file_सूचकांक++)
    {
      खण्डम्_2 *text_बफर = text_buffers + file_सूचकांक;

      /* Read the file contents into memory, then study it.  */

      swallow_file_अन्तर्_स्मृतिः (निवेशः_file_name[file_सूचकांक], text_बफर);
      find_occurs_अन्तर्_text (file_सूचकांक);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      योग_पङ्क्तिः_गणनम्++;
      file_पङ्क्तिः_गणनम्[file_सूचकांक] = योग_पङ्क्तिः_गणनम्;
    }

  /* Do the output process phase.  */

  क्रमय_प्राप्तम्_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  प्रत्यागम EXIT_SUCCESS;
}
