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

#ضمن <config.h>

#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن "system.h"
#ضمن <regex.h>
#ضمن "argmatch.h"
#ضمن "c-ctype.h"
#ضمن "fadvise.h"
#ضمن "octhexdigits.h"
#ضمن "quote.h"
#ضمن "read-file.h"
#ضمن "stdio--.h"
#ضمن "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کړه پروګرام_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#تعریف_کړه AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#تعریف_کړه CHAR_سېټ_اندازه 256

/* Debugging the memory allocator.  */

#إذا WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#د_که_پای

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

تعداد Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

ساكن bool gnu_extensions = true;	/* trigger all GNU extensions */
ساكن bool auto_reference = false;	/* refs are 'file_name:line_number:' */
ساكن bool ننوت_reference = false;	/* refs at beginning of input lines */
ساكن bool ښی_reference = false;	/* output refs after right context  */
ساكن idx_t کرښه_پلنوالی = -1;		/* output line width in characters */
ساكن idx_t gap_اندازه = 3;	/* number of spaces between output fields */
ساكن حرف ثابت *truncation_string = "/";
                                /* string used to mark line truncations */
ساكن حرف ثابت *macro_name = "xx";	/* macro name for roff or TeX output */
ساكن تعداد Format output_format = UNKNOWN_FORMAT;
                                /* output format */

ساكن bool ignore_case = false;	/* fold lower to upper for sorting */
ساكن حرف ثابت *break_file = عدم; /* name of the 'Break chars' file */
ساكن حرف ثابت *only_file = عدم;	/* name of the 'Only words' file */
ساكن حرف ثابت *ignore_file = عدم; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
بنية regex_معلومات
{
  /* The original regular expression, as a string.  */
  حرف ثابت *string;

  /* The compiled regular expression, and its fastmap.  */
  بنية re_نمونه_بفر نمونه_2;
  حرف fastmap[UCHAR_MAX + 1];
};

ساكن بنية regex_معلومات context_regex;	/* end of context */
ساكن بنية regex_معلومات کلمه_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

تعريفنوع بنية
  {
    حرف *پیل;		/* pointer to beginning of region */
    حرف *پای_ټکی;			/* pointer to end + 1 of region */
  }
بلاک_2;

تعريفنوع بنية
  {
    حرف *پیل;		/* pointer to beginning of region */
    idx_t اندازه;			/* length of the region */
  }
کلمه;

تعريفنوع بنية
  {
    idx_t alloc;		/* allocated length */
    idx_t اوږدتیا;		/* number of used entries */
    کلمه *پیل COUNTED_BY (اوږدتیا);	/* array of WORDs */
  }
کلمه_جدول;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
ساكن بینښه حرف folded_chars[CHAR_سېټ_اندازه];

/* End of context pattern register indices.  */
ساكن بنية re_registers context_regs;

/* Keyword pattern register indices.  */
ساكن بنية re_registers کلمه_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
ساكن حرف کلمه_fastmap[CHAR_سېټ_اندازه];

/* Maximum length of any word read.  */
ساكن idx_t اعظمي_کلمه_اوږدتیا;

/* Maximum width of any reference used.  */
ساكن idx_t reference_max_پلنوالی;

/* Ignore and Only word tables.  */

ساكن کلمه_جدول ignore_جدول;	/* table of words to ignore */
ساكن کلمه_جدول only_جدول;		/* table of words to select */

/* Source text table, and scanning macros.  */

ساكن صحيح شمېره_ننوت_files;	/* number of text input files */
ساكن intmax_t ټول_کرښه_شمېر;	/* total number of lines seen so far */
ساكن حرف ثابت **ننوت_file_name;	/* array of text input file names */
ساكن intmax_t *file_کرښه_شمېر;	/* array of line count values at end */

ساكن بلاک_2 *متن_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#تعریف_کړه SKIP_NON_WHITE(cursor, حد) \
  طالما (cursor < حد && ! که_تشه (منزل_uchar (*cursor)))		\
    cursor++

#تعریف_کړه SKIP_WHITE(cursor, حد) \
  طالما (cursor < حد && که_تشه (منزل_uchar (*cursor)))		\
    cursor++

#تعریف_کړه SKIP_WHITE_BACKWARDS(cursor, پیل) \
  طالما (cursor > پیل && که_تشه (منزل_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#تعریف_کړه SKIP_SOMETHING(cursor, حد) \
  إذا (کلمه_regex.string)						\
    {									\
      regoff_t شمېر;							\
      شمېر = re_سمون (&کلمه_regex.نمونه_2, cursor, حد - cursor,	\
                        0, عدم);					\
      إذا (شمېر == -2)							\
        matcher_error ();						\
      cursor += شمېر <= 0 ? 1 : شمېر;					\
    }									\
  وإلا إذا (کلمه_fastmap[منزل_uchar (*cursor)])				\
    طالما (cursor < حد && کلمه_fastmap[منزل_uchar (*cursor)])		\
      cursor++;								\
  وإلا									\
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

تعريفنوع بنية
  {
    کلمه کلی;			/* description of the keyword */
    ptrdiff_t کیڼ;		/* distance to left context start */
    ptrdiff_t ښی;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    صحيح file_شاخص;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

ساكن OCCURS *occurs_جدول[1];	/* all words retained from the read text */
ساكن idx_t occurs_alloc[1];	/* allocated size of occurs_table */
ساكن idx_t شمېره_د_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
ساكن حرف edited_flag[CHAR_سېټ_اندازه];

/* Half of line width, reference excluded.  */
ساكن idx_t half_کرښه_پلنوالی;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ساكن ptrdiff_t before_max_پلنوالی;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ساكن ptrdiff_t keyafter_max_پلنوالی;

/* Length of string that flags truncation.  */
ساكن idx_t truncation_string_اوږدتیا;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

ساكن بلاک_2 لکۍ;		/* tail field */
ساكن bool لکۍ_truncation;	/* flag truncation after the tail field */

ساكن بلاک_2 before;		/* before field */
ساكن bool before_truncation;	/* flag truncation before the before field */

ساكن بلاک_2 keyafter;		/* keyword-and-after field */
ساكن bool keyafter_truncation; /* flag truncation after the keyafter field */

ساكن بلاک_2 سر;		/* head field */
ساكن bool سر_truncation;	/* flag truncation before the head field */

ساكن بلاک_2 reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

ساكن _Noreturn فراغ
matcher_error (فراغ)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

ساكن فراغ
unescape_string (حرف *string)
{
  حرف *cursor;			/* cursor in result */
  صحيح ارزښت;			/* value of \nnn escape */
  صحيح اوږدتیا;			/* length of \nnn escape */

  cursor = string;

  طالما (*string)
    {
      إذا (*string == '\\')
        {
          string++;
          بدل (*string)
            {
            حالة 'x':		/* \xhhh escape, 3 chars maximum */
              ارزښت = 0;
              لكل (اوږدتیا = 0, string++;
                   اوږدتیا < 3 && c_isxdigit (*string);
                   اوږدتیا++, string++)
                ارزښت = ارزښت * 16 + fromhex (*string);
              إذا (اوږدتیا == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              وإلا
                *cursor++ = ارزښت;
              اكسر;

            حالة '0':		/* \0ooo escape, 3 chars maximum */
              ارزښت = 0;
              لكل (اوږدتیا = 0, string++;
                   اوږدتیا < 3 && isoct (*string);
                   اوږدتیا++, string++)
                ارزښت = ارزښت * 8 + fromoct (*string);
              *cursor++ = ارزښت;
              اكسر;

            حالة 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              اكسر;

            حالة 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              اكسر;

            حالة 'c':		/* cancel the rest of the output */
              طالما (*string)
                string++;
              اكسر;

            حالة 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              اكسر;

            حالة 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              اكسر;

            حالة 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              اكسر;

            حالة 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              اكسر;

            حالة 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              اكسر;

            حالة '\0':		/* lone backslash at end of string */
              /* ignore it */
              اكسر;

            افتراضي:
              *cursor++ = '\\';
              *cursor++ = *string++;
              اكسر;
            }
        }
      وإلا
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

ساكن فراغ
compile_regex (بنية regex_معلومات *regex)
{
  بنية re_نمونه_بفر *نمونه_2 = &regex->نمونه_2;
  حرف ثابت *string = regex->string;
  حرف ثابت *پیغام_2;

  نمونه_2->بفر = عدم;
  نمونه_2->allocated = 0;
  نمونه_2->fastmap = regex->fastmap;
  نمونه_2->translate = ignore_case ? folded_chars : عدم;

  پیغام_2 = re_compile_نمونه (string, مزي_اوږدوالی (string), نمونه_2);
  إذا (پیغام_2)
    error (EXIT_FAILURE, 0, _("%s (for regexp %s)"), پیغام_2, quote (string));

  /* The fastmap should be compiled before 're_match'.  The following
     call is not mandatory, because 're_search' is always called sooner,
     and it compiles the fastmap if this has not been done yet.  */

  re_compile_fastmap (نمونه_2);
}

/*------------------------------------------------------------------------.
| This will initialize various tables for pattern match and compiles some |
| regexps.								  |
`------------------------------------------------------------------------*/

ساكن فراغ
initialize_regex (فراغ)
{
  /* Initialize the case folding table.  */

  إذا (ignore_case)
    لكل (صحيح character = 0; character < CHAR_سېټ_اندازه; character++)
      folded_chars[character] = لوی_توری (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  إذا (context_regex.string)
    {
      إذا (!*context_regex.string)
        context_regex.string = عدم;
    }
  وإلا إذا (gnu_extensions && !ننوت_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  وإلا
    context_regex.string = "\n";

  إذا (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  إذا (کلمه_regex.string)
    compile_regex (&کلمه_regex);
  وإلا إذا (!break_file)
    {
      إذا (gnu_extensions)
        {

          /* Simulate \w+.  */

          لكل (صحيح character = 0; character < CHAR_سېټ_اندازه; character++)
            کلمه_fastmap[character] = !! که_توری (character);
        }
      وإلا
        {

          /* Simulate [^ \t\n]+.  */

          memset (کلمه_fastmap, 1, CHAR_سېټ_اندازه);
          کلمه_fastmap[' '] = 0;
          کلمه_fastmap['\t'] = 0;
          کلمه_fastmap['\n'] = 0;
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

ساكن فراغ
swallow_file_دننه_حافظه (حرف ثابت *file_name, بلاک_2 *بلاک)
{
  size_t کارول_شوی_اوږدتیا;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  إذا (using_stdin)
    بلاک->پیل = fread_file (stdin, 0, &کارول_شوی_اوږدتیا);
  وإلا
    بلاک->پیل = read_file (file_name, 0, &کارول_شوی_اوږدتیا);

  إذا (!بلاک->پیل)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  إذا (using_stdin)
    clearerr (stdin);

  بلاک->پای_ټکی = بلاک->پیل + کارول_شوی_اوږدتیا;
}

/* Sort and search routines.  */

/*--------------------------------------------------------------------------.
| Compare two words, FIRST and SECOND, and return 0 if they are identical.  |
| Return less than 0 if the first word goes before the second; return	    |
| greater than 0 if the first word goes after the second.		    |
|									    |
| If a word is indeed a prefix of the other, the shorter should go first.   |
`--------------------------------------------------------------------------*/

ساكن صحيح
compare_ټکي (ثابت فراغ *void_لومړی, ثابت فراغ *void_دوهم)
{
  کلمه ثابت *لومړی = void_لومړی;
  کلمه ثابت *دوهم = void_دوهم;
  idx_t اوږدتیا = MIN (لومړی->اندازه, دوهم->اندازه);

  إذا (ignore_case)
    {
      لكل (idx_t counter = 0; counter < اوږدتیا; counter++)
        {
          صحيح ارزښت = (folded_chars[منزل_uchar (لومړی->پیل[counter])]
                       - folded_chars[منزل_uchar (دوهم->پیل[counter])]);
          إذا (ارزښت != 0)
            أرجع ارزښت;
        }
    }
  وإلا
    {
      لكل (idx_t counter = 0; counter < اوږدتیا; counter++)
        {
          صحيح ارزښت = (منزل_uchar (لومړی->پیل[counter])
                       - منزل_uchar (دوهم->پیل[counter]));
          إذا (ارزښت != 0)
            أرجع ارزښت;
        }
    }

  أرجع _GL_CMP (لومړی->اندازه, دوهم->اندازه);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

ساكن صحيح
compare_occurs (ثابت فراغ *void_لومړی, ثابت فراغ *void_دوهم)
{
#تعریف_کړه لومړی ((ثابت OCCURS *) void_لومړی)
#تعریف_کړه دوهم ((ثابت OCCURS *) void_دوهم)
  صحيح ارزښت;

  ارزښت = compare_ټکي (&لومړی->کلی, &دوهم->کلی);
  أرجع (ارزښت ? ارزښت
          : _GL_CMP (لومړی->کلی.پیل, دوهم->کلی.پیل));
#تعریف_لغوه لومړی
#تعریف_لغوه دوهم
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_خالص
ساكن bool
پلټنه_جدول (کلمه *کلمه_2, کلمه_جدول *جدول)
{
  idx_t lo = 0;
  idx_t hi = جدول->اوږدتیا;
  طالما (lo < hi)
    {
      idx_t منځ = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      صحيح ارزښت = compare_ټکي (کلمه_2, جدول->پیل + منځ);
      إذا (ارزښت < 0)
        hi = منځ;
      وإلا إذا (ارزښت > 0)
        lo = منځ + 1;
      وإلا
        أرجع true;
    }
  أرجع false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

ساكن فراغ
ترتیبول_وموندل_occurs (فراغ)
{

  /* Only one language for the time being.  */
  إذا (شمېره_د_occurs[0])
    qsort (occurs_جدول[0], شمېره_د_occurs[0], حجم **occurs_جدول,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

ساكن فراغ
digest_break_file (حرف ثابت *file_name)
{
  بلاک_2 file_contents;		/* to receive a copy of the file */

  swallow_file_دننه_حافظه (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (کلمه_fastmap, 1, CHAR_سېټ_اندازه);
  لكل (حرف *cursor = file_contents.پیل; cursor < file_contents.پای_ټکی;
       cursor++)
    کلمه_fastmap[منزل_uchar (*cursor)] = 0;

  إذا (!gnu_extensions)
    {

      /* If GNU extensions are enabled, the only way to avoid newline as
         a break character is to write all the break characters in the
         file with no newline at all, not even at the end of the file.
         If disabled, spaces, tabs and newlines are always considered as
         break characters even if not included in the break file.  */

      کلمه_fastmap[' '] = 0;
      کلمه_fastmap['\t'] = 0;
      کلمه_fastmap['\n'] = 0;
    }

  /* Return the space of the file, which is no more required.  */

  حرر (file_contents.پیل);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

ساكن فراغ
digest_کلمه_file (حرف ثابت *file_name, کلمه_جدول *جدول)
{
  بلاک_2 file_contents;		/* to receive a copy of the file */
  حرف *cursor;			/* cursor in file copy */
  حرف *کلمه_پیل;		/* start of the current word */

  swallow_file_دننه_حافظه (file_name, &file_contents);

  جدول->پیل = عدم;
  جدول->alloc = 0;
  جدول->اوږدتیا = 0;

  /* Read the whole file.  */

  cursor = file_contents.پیل;
  طالما (cursor < file_contents.پای_ټکی)
    {

      /* Read one line, and save the word in contains.  */

      کلمه_پیل = cursor;
      طالما (cursor < file_contents.پای_ټکی && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      إذا (cursor > کلمه_پیل)
        {
          إذا (جدول->اوږدتیا == جدول->alloc)
            جدول->پیل = xpalloc (جدول->پیل, &جدول->alloc, 1, -1,
                                    حجم *جدول->پیل);
          idx_t ثابت جدول_idx = جدول->اوږدتیا++;
          جدول->پیل[جدول_idx].پیل = کلمه_پیل;
          جدول->پیل[جدول_idx].اندازه = cursor - کلمه_پیل;
        }

      /* This test allows for an incomplete line at end of file.  */

      إذا (cursor < file_contents.پای_ټکی)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (جدول->پیل, جدول->اوږدتیا, حجم جدول->پیل[0], compare_ټکي);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

ساكن فراغ
پیدا_کړه_occurs_دننه_متن (صحيح file_شاخص)
{
  حرف *لټون;			/* for scanning the source text */
  حرف *کرښه_پیل;		/* start of the current input line */
  حرف *کرښه_لټون;		/* newlines scanned until this point */
  idx_t reference_اوږدتیا;	/* length of reference in input mode */
  کلمه possible_کلی;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  حرف *context_پیل;		/* start of left context */
  حرف *context_پای_ټکی;		/* end of right context */
  حرف *کلمه_پیل;		/* start of word */
  حرف *کلمه_پای_ټکی;		/* end of word */
  حرف *بل_context_پیل;	/* next start of left context */

  ثابت بلاک_2 *متن_بفر = &متن_buffers[file_شاخص];

  /* reference_length is always used within 'if (input_reference)'.
     However, GNU C diagnoses that it may be used uninitialized.  The
     following assignment is merely to shut it up.  */

  reference_اوږدتیا = 0;

  /* Tracking where lines start is helpful for reference processing.  In
     auto reference mode, this allows counting lines.  In input reference
     mode, this permits finding the beginning of the references.

     The first line begins with the file, skip immediately this very first
     reference in input reference mode, to help further rejection any word
     found inside it.  Also, unconditionally assigning these variable has
     the happy effect of shutting up lint.  */

  کرښه_پیل = متن_بفر->پیل;
  کرښه_لټون = کرښه_پیل;
  إذا (ننوت_reference)
    {
      SKIP_NON_WHITE (کرښه_لټون, متن_بفر->پای_ټکی);
      reference_اوږدتیا = کرښه_لټون - کرښه_پیل;
      SKIP_WHITE (کرښه_لټون, متن_بفر->پای_ټکی);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  لكل (حرف *cursor = متن_بفر->پیل;
       cursor < متن_بفر->پای_ټکی;
       cursor = بل_context_پیل)
    {

      /* 'context_start' gets initialized before the processing of each
         line, or once for the whole buffer if no end of line or sentence
         sequence separator.  */

      context_پیل = cursor;

      /* If an end of line or end of sentence sequence is defined and
         non-empty, 'next_context_start' will be recomputed to be the end of
         each line or sentence, before each one is processed.  If no such
         sequence, then 'next_context_start' is set at the end of the whole
         buffer, which is then considered to be a single line or sentence.
         This test also accounts for the case of an incomplete line or
         sentence at the end of the buffer.  */

      بل_context_پیل = متن_بفر->پای_ټکی;
      إذا (context_regex.string)
        بدل (re_پلټنه (&context_regex.نمونه_2, cursor,
                           متن_بفر->پای_ټکی - cursor,
                           0, متن_بفر->پای_ټکی - cursor, &context_regs))
          {
          حالة -2:
            matcher_error ();

          حالة -1:
            اكسر;

          حالة 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          افتراضي:
            بل_context_پیل = cursor + context_regs.پای_ټکی[0];
            اكسر;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_پای_ټکی = بل_context_پیل;
      SKIP_WHITE_BACKWARDS (context_پای_ټکی, context_پیل);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      طالما (true)
        {
          إذا (کلمه_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_پلټنه (&کلمه_regex.نمونه_2, cursor,
                                      context_پای_ټکی - cursor,
                                      0, context_پای_ټکی - cursor, &کلمه_regs);
              إذا (r == -2)
                matcher_error ();
              إذا (r == -1)
                اكسر;
              کلمه_پیل = cursor + کلمه_regs.پیل[0];
              کلمه_پای_ټکی = cursor + کلمه_regs.پای_ټکی[0];
            }
          وإلا

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              لټون = cursor;
              طالما (لټون < context_پای_ټکی
                     && !کلمه_fastmap[منزل_uchar (*لټون)])
                لټون++;

              إذا (لټون == context_پای_ټکی)
                اكسر;

              کلمه_پیل = لټون;

              طالما (لټون < context_پای_ټکی
                     && کلمه_fastmap[منزل_uchar (*لټون)])
                لټون++;

              کلمه_پای_ټکی = لټون;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = کلمه_پیل;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          إذا (کلمه_پای_ټکی == کلمه_پیل)
            {
              cursor++;
              استمر;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_کلی.پیل = cursor;
          possible_کلی.اندازه = کلمه_پای_ټکی - کلمه_پیل;
          cursor += possible_کلی.اندازه;

          إذا (possible_کلی.اندازه > اعظمي_کلمه_اوږدتیا)
            اعظمي_کلمه_اوږدتیا = possible_کلی.اندازه;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          إذا (ننوت_reference)
            {
              طالما (کرښه_لټون < possible_کلی.پیل)
                إذا (*کرښه_لټون == '\n')
                  {
                    ټول_کرښه_شمېر++;
                    کرښه_لټون++;
                    کرښه_پیل = کرښه_لټون;
                    SKIP_NON_WHITE (کرښه_لټون, متن_بفر->پای_ټکی);
                    reference_اوږدتیا = کرښه_لټون - کرښه_پیل;
                  }
                وإلا
                  کرښه_لټون++;
              إذا (کرښه_لټون > possible_کلی.پیل)
                استمر;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          إذا (ignore_file && پلټنه_جدول (&possible_کلی, &ignore_جدول))
            استمر;
          إذا (only_file && !پلټنه_جدول (&possible_کلی, &only_جدول))
            استمر;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          إذا (شمېره_د_occurs[0] == occurs_alloc[0])
            occurs_جدول[0] = xpalloc (occurs_جدول[0], &occurs_alloc[0],
                                       1, -1, حجم *occurs_جدول[0]);
          occurs_cursor = occurs_جدول[0] + شمېره_د_occurs[0];

          /* Define the reference field, if any.  */

          إذا (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              طالما (کرښه_لټون < possible_کلی.پیل)
                إذا (*کرښه_لټون == '\n')
                  {
                    ټول_کرښه_شمېر++;
                    کرښه_لټون++;
                    کرښه_پیل = کرښه_لټون;
                    SKIP_NON_WHITE (کرښه_لټون, متن_بفر->پای_ټکی);
                  }
                وإلا
                  کرښه_لټون++;

              occurs_cursor->reference = ټول_کرښه_شمېر;
            }
          وإلا إذا (ننوت_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = کرښه_پیل - possible_کلی.پیل;
              إذا (reference_اوږدتیا > reference_max_پلنوالی)
                reference_max_پلنوالی = reference_اوږدتیا;
            }

          /* Exclude the reference from the context in simple cases.  */

          إذا (ننوت_reference && کرښه_پیل == context_پیل)
            {
              SKIP_NON_WHITE (context_پیل, context_پای_ټکی);
              SKIP_WHITE (context_پیل, context_پای_ټکی);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->کلی = possible_کلی;
          occurs_cursor->کیڼ = context_پیل - possible_کلی.پیل;
          occurs_cursor->ښی = context_پای_ټکی - possible_کلی.پیل;
          occurs_cursor->file_شاخص = file_شاخص;

          شمېره_د_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

ساكن فراغ
print_spaces (ptrdiff_t شمېره)
{
  لكل (ptrdiff_t counter = شمېره; counter > 0; counter--)
    توری_چاپ_کړه (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

ساكن فراغ
print_ډګر (بلاک_2 ډګر)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  لكل (حرف *cursor = ډګر.پیل; cursor < ډګر.پای_ټکی; cursor++)
    {
      بینښه حرف character = *cursor;
      إذا (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          بدل (character)
            {
            حالة '"':
              /* In roff output format, double any quote.  */
              توری_چاپ_کړه ('"');
              توری_چاپ_کړه ('"');
              اكسر;

            حالة '$':
            حالة '%':
            حالة '&':
            حالة '#':
            حالة '_':
              /* In TeX output format, precede these with a backslash.  */
              توری_چاپ_کړه ('\\');
              توری_چاپ_کړه (character);
              اكسر;

            حالة '{':
            حالة '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              چاپ_کړه ("$\\%c$", character);
              اكسر;

            حالة '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              اكسر;

            افتراضي:
              /* Any other flagged character produces a single space.  */
              توری_چاپ_کړه (' ');
            }
        }
      وإلا
        توری_چاپ_کړه (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

ساكن فراغ
fix_output_parameters (فراغ)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  إذا (auto_reference)
    {
      reference_max_پلنوالی = 0;
      لكل (صحيح file_شاخص = 0; file_شاخص < شمېره_ننوت_files; file_شاخص++)
        {
          intmax_t کرښه_ordinal = file_کرښه_شمېر[file_شاخص] + 1;
          إذا (file_شاخص > 0)
            کرښه_ordinal -= file_کرښه_شمېر[file_شاخص - 1];
          حرف ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_پلنوالی = sprintf (ordinal_string, "%jd", کرښه_ordinal);
          إذا (ننوت_file_name[file_شاخص])
            reference_پلنوالی += مزي_اوږدوالی (ننوت_file_name[file_شاخص]);
          إذا (reference_پلنوالی > reference_max_پلنوالی)
            reference_max_پلنوالی = reference_پلنوالی;
        }
      reference_max_پلنوالی++;
      reference.پیل = xmalloc (reference_max_پلنوالی + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  إذا ((auto_reference || ننوت_reference) && !ښی_reference)
    کرښه_پلنوالی = MAX (0, کرښه_پلنوالی - (reference_max_پلنوالی + gap_اندازه));

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

  half_کرښه_پلنوالی = کرښه_پلنوالی >> 1;
  before_max_پلنوالی = half_کرښه_پلنوالی - gap_اندازه;
  keyafter_max_پلنوالی = half_کرښه_پلنوالی;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  إذا (truncation_string && *truncation_string)
    truncation_string_اوږدتیا = مزي_اوږدوالی (truncation_string);
  وإلا
    truncation_string = عدم;

  إذا (gnu_extensions)
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

      before_max_پلنوالی -= 2 * truncation_string_اوږدتیا;
      keyafter_max_پلنوالی -= 2 * truncation_string_اوږدتیا;
    }
  وإلا
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_پلنوالی -= 2 * truncation_string_اوږدتیا + 1;
    }

  إذا (before_max_پلنوالی < 0)
    before_max_پلنوالی = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  لكل (صحيح character = 0; character < CHAR_سېټ_اندازه; character++)
    edited_flag[character] = !! که_تشه (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  بدل (output_format)
    {
    حالة UNKNOWN_FORMAT:
      /* Should never happen.  */

    حالة DUMB_FORMAT:
      اكسر;

    حالة ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      اكسر;

    حالة TEX_FORMAT:

      /* Various characters need special processing.  */

      لكل (حرف ثابت *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[منزل_uchar (*cursor)] = 1;

      اكسر;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

ساكن فراغ
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t لکۍ_max_پلنوالی;	/* allowable width of tail field */
  ptrdiff_t سر_max_پلنوالی;	/* allowable width of head field */
  حرف *cursor;			/* running cursor in source text */
  حرف *کیڼ_context_پیل;	/* start of left context */
  حرف *ښی_context_پای_ټکی;	/* end of right context */
  حرف *کیڼ_ډګر_پیل;	/* conservative start for 'head'/'before' */
  حرف ثابت *file_name;	/* file name for reference */
  intmax_t کرښه_ordinal;	/* line ordinal for reference */
  حرف ثابت *بفر_پیل;	/* start of buffered file for this occurs */
  حرف ثابت *بفر_پای_ټکی;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.پیل = occurs->کلی.پیل;
  keyafter.پای_ټکی = keyafter.پیل + occurs->کلی.اندازه;
  کیڼ_context_پیل = keyafter.پیل + occurs->کیڼ;
  ښی_context_پای_ټکی = keyafter.پیل + occurs->ښی;

  بفر_پیل = متن_buffers[occurs->file_شاخص].پیل;
  بفر_پای_ټکی = متن_buffers[occurs->file_شاخص].پای_ټکی;

  cursor = keyafter.پای_ټکی;
  طالما (cursor < ښی_context_پای_ټکی
         && cursor <= keyafter.پیل + keyafter_max_پلنوالی)
    {
      keyafter.پای_ټکی = cursor;
      SKIP_SOMETHING (cursor, ښی_context_پای_ټکی);
    }
  إذا (cursor <= keyafter.پیل + keyafter_max_پلنوالی)
    keyafter.پای_ټکی = cursor;

  keyafter_truncation = truncation_string && keyafter.پای_ټکی < ښی_context_پای_ټکی;

  SKIP_WHITE_BACKWARDS (keyafter.پای_ټکی, keyafter.پیل);

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

  إذا (-occurs->کیڼ > half_کرښه_پلنوالی + اعظمي_کلمه_اوږدتیا)
    {
      کیڼ_ډګر_پیل
        = keyafter.پیل - (half_کرښه_پلنوالی + اعظمي_کلمه_اوږدتیا);
      SKIP_SOMETHING (کیڼ_ډګر_پیل, keyafter.پیل);
    }
  وإلا
    کیڼ_ډګر_پیل = keyafter.پیل + occurs->کیڼ;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.پیل = کیڼ_ډګر_پیل;
  before.پای_ټکی = keyafter.پیل;
  SKIP_WHITE_BACKWARDS (before.پای_ټکی, before.پیل);

  طالما (before.پیل + before_max_پلنوالی < before.پای_ټکی)
    SKIP_SOMETHING (before.پیل, before.پای_ټکی);

  إذا (truncation_string)
    {
      cursor = before.پیل;
      SKIP_WHITE_BACKWARDS (cursor, بفر_پیل);
      before_truncation = cursor > کیڼ_context_پیل;
    }
  وإلا
    before_truncation = false;

  SKIP_WHITE (before.پیل, بفر_پای_ټکی);

  /* The tail could not take more columns than what has been left in the
     left context field, and a gap is mandatory.  It starts after the
     right context, and does not contain prefixed spaces.  It ends at
     the end of line, the end of buffer or when the tail field is full,
     whichever comes first.  It cannot contain only part of a word, and
     has no suffixed spaces.  */

  لکۍ_max_پلنوالی
    = before_max_پلنوالی - (before.پای_ټکی - before.پیل) - gap_اندازه;

  إذا (لکۍ_max_پلنوالی > 0)
    {
      لکۍ.پیل = keyafter.پای_ټکی;
      SKIP_WHITE (لکۍ.پیل, بفر_پای_ټکی);

      لکۍ.پای_ټکی = لکۍ.پیل;
      cursor = لکۍ.پای_ټکی;
      طالما (cursor < ښی_context_پای_ټکی
             && cursor < لکۍ.پیل + لکۍ_max_پلنوالی)
        {
          لکۍ.پای_ټکی = cursor;
          SKIP_SOMETHING (cursor, ښی_context_پای_ټکی);
        }

      إذا (cursor < لکۍ.پیل + لکۍ_max_پلنوالی)
        لکۍ.پای_ټکی = cursor;

      إذا (لکۍ.پای_ټکی > لکۍ.پیل)
        {
          keyafter_truncation = false;
          لکۍ_truncation = truncation_string && لکۍ.پای_ټکی < ښی_context_پای_ټکی;
        }
      وإلا
        لکۍ_truncation = false;

      SKIP_WHITE_BACKWARDS (لکۍ.پای_ټکی, لکۍ.پیل);
    }
  وإلا
    {

      /* No place left for a tail field.  */

      لکۍ.پیل = عدم;
      لکۍ.پای_ټکی = عدم;
      لکۍ_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  سر_max_پلنوالی
    = keyafter_max_پلنوالی - (keyafter.پای_ټکی - keyafter.پیل) - gap_اندازه;

  إذا (سر_max_پلنوالی > 0)
    {
      سر.پای_ټکی = before.پیل;
      SKIP_WHITE_BACKWARDS (سر.پای_ټکی, بفر_پیل);

      سر.پیل = کیڼ_ډګر_پیل;
      طالما (سر.پیل + سر_max_پلنوالی < سر.پای_ټکی)
        SKIP_SOMETHING (سر.پیل, سر.پای_ټکی);

      إذا (سر.پای_ټکی > سر.پیل)
        {
          before_truncation = false;
          سر_truncation = (truncation_string
                             && سر.پیل > کیڼ_context_پیل);
        }
      وإلا
        سر_truncation = false;

      SKIP_WHITE (سر.پیل, سر.پای_ټکی);
    }
  وإلا
    {

      /* No place left for a head field.  */

      سر.پیل = عدم;
      سر.پای_ټکی = عدم;
      سر_truncation = false;
    }

  إذا (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = ننوت_file_name[occurs->file_شاخص];
      إذا (!file_name)
        file_name = "";

      کرښه_ordinal = occurs->reference + 1;
      إذا (occurs->file_شاخص > 0)
        کرښه_ordinal -= file_کرښه_شمېر[occurs->file_شاخص - 1];

      حرف *file_پای_ټکی = stpcpy (reference.پیل, file_name);
      reference.پای_ټکی = file_پای_ټکی + sprintf (file_پای_ټکی, ":%jd", کرښه_ordinal);
    }
  وإلا إذا (ننوت_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.پیل = keyafter.پیل + occurs->reference;
      reference.پای_ټکی = reference.پیل;
      SKIP_NON_WHITE (reference.پای_ټکی, ښی_context_پای_ټکی);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

ساكن فراغ
output_one_roff_کرښه (فراغ)
{
  /* Output the 'tail' field.  */

  چاپ_کړه (".%s \"", macro_name);
  print_ډګر (لکۍ);
  إذا (لکۍ_truncation)
    fputs (truncation_string, stdout);
  توری_چاپ_کړه ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  إذا (before_truncation)
    fputs (truncation_string, stdout);
  print_ډګر (before);
  توری_چاپ_کړه ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_ډګر (keyafter);
  إذا (keyafter_truncation)
    fputs (truncation_string, stdout);
  توری_چاپ_کړه ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  إذا (سر_truncation)
    fputs (truncation_string, stdout);
  print_ډګر (سر);
  توری_چاپ_کړه ('"');

  /* Conditionally output the 'reference' field.  */

  إذا (auto_reference || ننوت_reference)
    {
      fputs (" \"", stdout);
      print_ډګر (reference);
      توری_چاپ_کړه ('"');
    }

  توری_چاپ_کړه ('\n');
}

/*---------------------------------------------------------.
| Output the current output fields as one line for 'TeX'.  |
`---------------------------------------------------------*/

ساكن فراغ
output_one_tex_کرښه (فراغ)
{
  بلاک_2 کلی;			/* key field, isolated */
  بلاک_2 after;			/* after field, isolated */
  حرف *cursor;			/* running cursor in source text */

  چاپ_کړه ("\\%s ", macro_name);
  توری_چاپ_کړه ('{');
  print_ډګر (لکۍ);
  fputs ("}{", stdout);
  print_ډګر (before);
  fputs ("}{", stdout);
  کلی.پیل = keyafter.پیل;
  after.پای_ټکی = keyafter.پای_ټکی;
  cursor = keyafter.پیل;
  SKIP_SOMETHING (cursor, keyafter.پای_ټکی);
  کلی.پای_ټکی = cursor;
  after.پیل = cursor;
  print_ډګر (کلی);
  fputs ("}{", stdout);
  print_ډګر (after);
  fputs ("}{", stdout);
  print_ډګر (سر);
  توری_چاپ_کړه ('}');
  إذا (auto_reference || ننوت_reference)
    {
      توری_چاپ_کړه ('{');
      print_ډګر (reference);
      توری_چاپ_کړه ('}');
    }
  توری_چاپ_کړه ('\n');
}

/*-------------------------------------------------------------------.
| Output the current output fields as one line for a dumb terminal.  |
`-------------------------------------------------------------------*/

ساكن فراغ
output_one_dumb_کرښه (فراغ)
{
  إذا (!ښی_reference)
    {
      إذا (auto_reference)
        {

          /* Output the 'reference' field, in such a way that GNU emacs
             next-error will handle it.  The ending colon is taken from the
             gap which follows.  */

          print_ډګر (reference);
          توری_چاپ_کړه (':');
          print_spaces (reference_max_پلنوالی
                        + gap_اندازه
                        - (reference.پای_ټکی - reference.پیل)
                        - 1);
        }
      وإلا
        {

          /* Output the 'reference' field and its following gap.  */

          print_ډګر (reference);
          print_spaces (reference_max_پلنوالی
                        + gap_اندازه
                        - (reference.پای_ټکی - reference.پیل));
        }
    }

  إذا (لکۍ.پیل < لکۍ.پای_ټکی)
    {
      /* Output the 'tail' field.  */

      print_ډګر (لکۍ);
      إذا (لکۍ_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_کرښه_پلنوالی - gap_اندازه
                    - (before.پای_ټکی - before.پیل)
                    - (before_truncation ? truncation_string_اوږدتیا : 0)
                    - (لکۍ.پای_ټکی - لکۍ.پیل)
                    - (لکۍ_truncation ? truncation_string_اوږدتیا : 0));
    }
  وإلا
    print_spaces (half_کرښه_پلنوالی - gap_اندازه
                  - (before.پای_ټکی - before.پیل)
                  - (before_truncation ? truncation_string_اوږدتیا : 0));

  /* Output the 'before' field.  */

  إذا (before_truncation)
    fputs (truncation_string, stdout);
  print_ډګر (before);

  print_spaces (gap_اندازه);

  /* Output the 'keyafter' field.  */

  print_ډګر (keyafter);
  إذا (keyafter_truncation)
    fputs (truncation_string, stdout);

  إذا (سر.پیل < سر.پای_ټکی)
    {
      /* Output the 'head' field.  */

      print_spaces (half_کرښه_پلنوالی
                    - (keyafter.پای_ټکی - keyafter.پیل)
                    - (keyafter_truncation ? truncation_string_اوږدتیا : 0)
                    - (سر.پای_ټکی - سر.پیل)
                    - (سر_truncation ? truncation_string_اوږدتیا : 0));
      إذا (سر_truncation)
        fputs (truncation_string, stdout);
      print_ډګر (سر);
    }
  وإلا

    إذا ((auto_reference || ننوت_reference) && ښی_reference)
      print_spaces (half_کرښه_پلنوالی
                    - (keyafter.پای_ټکی - keyafter.پیل)
                    - (keyafter_truncation ? truncation_string_اوږدتیا : 0));

  إذا ((auto_reference || ننوت_reference) && ښی_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_اندازه);
      print_ډګر (reference);
    }

  توری_چاپ_کړه ('\n');
}

/*------------------------------------------------------------------------.
| Scan the whole occurs table and, for each entry, output one line in the |
| appropriate format.							  |
`------------------------------------------------------------------------*/

ساكن فراغ
generate_all_output (فراغ)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  لکۍ.پیل = عدم;
  لکۍ.پای_ټکی = عدم;
  لکۍ_truncation = false;

  سر.پیل = عدم;
  سر.پای_ټکی = عدم;
  سر_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_جدول[0];

  لكل (idx_t occurs_شاخص = 0; occurs_شاخص < شمېره_د_occurs[0];
       occurs_شاخص++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      بدل (output_format)
        {
        حالة UNKNOWN_FORMAT:
          /* Should never happen.  */

        حالة DUMB_FORMAT:
          output_one_dumb_کرښه ();
          اكسر;

        حالة ROFF_FORMAT:
          output_one_roff_کرښه ();
          اكسر;

        حالة TEX_FORMAT:
          output_one_tex_کرښه ();
          اكسر;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION]... [ننوت_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [ننوت_2 [OUTPUT]]\n"),
              پروګرام_name, پروګرام_name);
      fputs (_("\
Output a permuted index, including context, د the ټکي دننه the ننوت files.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --خپلکاری-reference           output automatically generated references\n\
"));
      oputs (_("\
  -G, --traditional              behave نور like System V 'ptx'\n\
"));
      oputs (_("\
  -F, --flag-truncation=STRING   use STRING لكل flagging کرښه truncations.\n\
                                 The افتراضي is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name منزل use instead د 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --ښی-side-refs          put references at ښی, not counted دننه -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   لكل پای_ټکی د کرښې or پای_ټکی د sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --کلمه_2-regexp=REGEXP       use REGEXP منزل سمون each keyword\n\
"));
      oputs (_("\
  -b, --اكسر-file=FILE          کلمه_2 اكسر characters دننه this FILE\n\
"));
      oputs (_("\
  -f, --ignore-حالة              fold ښکتنی حالة منزل پاسنی حالة لكل ترتیبونه\n\
"));
      oputs (_("\
  -g, --gap-اندازه=شمېره_2          gap اندازه دننه columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         اقرأ ignore کلمه_2 list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           اقرأ only کلمه_2 list from this FILE\n\
"));
      oputs (_("\
  -r, --references               لومړی ډګر د each کرښه is a reference\n\
"));
      oputs (_("\
  -t, --typeset-ډېرپېښ_2             change the افتراضي پلنوالی from 72 منزل 100\n\
"));
      oputs (_("\
  -w, --پلنوالی=شمېره_2             output پلنوالی دننه columns, reference excluded\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

/*----------------------------------------------------------------------.
| Main program.  Decode ARGC arguments passed through the ARGV array of |
| strings, then launch execution.				        |
`----------------------------------------------------------------------*/

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
ساكن بنية option ثابت long_options[] =
{
  {"auto-reference", no_دلیل, عدم, 'A'},
  {"break-file", required_دلیل, عدم, 'b'},
  {"flag-truncation", required_دلیل, عدم, 'F'},
  {"ignore-case", no_دلیل, عدم, 'f'},
  {"gap-size", required_دلیل, عدم, 'g'},
  {"ignore-file", required_دلیل, عدم, 'i'},
  {"macro-name", required_دلیل, عدم, 'M'},
  {"only-file", required_دلیل, عدم, 'o'},
  {"references", no_دلیل, عدم, 'r'},
  {"right-side-refs", no_دلیل, عدم, 'R'},
  {"format", required_دلیل, عدم, FORMAT_OPTION},
  {"sentence-regexp", required_دلیل, عدم, 'S'},
  {"traditional", no_دلیل, عدم, 'G'},
  {"typeset-mode", no_دلیل, عدم, 't'},
  {"width", required_دلیل, عدم, 'w'},
  {"word-regexp", required_دلیل, عدم, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

ساكن حرف ثابت *ثابت format_دلیلونه[] =
{
  "roff", "tex", عدم
};

ساكن تعداد Format ثابت format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

صحيح
اصلي (صحيح argc, حرف **argv)
{
  صحيح optchar;			/* argument character */
  صحيح file_شاخص;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, عدم),
         optchar != EOF)
    {
      بدل (optchar)
        {
        افتراضي:
          usage (EXIT_FAILURE);

        حالة 'G':
          gnu_extensions = false;
          اكسر;

        حالة 'b':
          break_file = optarg;
          اكسر;

        حالة 'f':
          ignore_case = true;
          اكسر;

        حالة 'g':
          {
            intmax_t tmp;
            إذا (! (xstrtoimax (optarg, عدم, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_اندازه = tmp;
            اكسر;
          }

        حالة 'i':
          ignore_file = optarg;
          اكسر;

        حالة 'o':
          only_file = optarg;
          اكسر;

        حالة 'r':
          ننوت_reference = true;
          اكسر;

        حالة 't':
          إذا (کرښه_پلنوالی < 0)
            کرښه_پلنوالی = 100;
          اكسر;

        حالة 'w':
          {
            intmax_t tmp;
            إذا (! (xstrtoimax (optarg, عدم, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            کرښه_پلنوالی = tmp;
            اكسر;
          }

        حالة 'A':
          auto_reference = true;
          اكسر;

        حالة 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          اكسر;

        حالة 'M':
          macro_name = optarg;
          اكسر;

        حالة 'O':
          output_format = ROFF_FORMAT;
          اكسر;

        حالة 'R':
          ښی_reference = true;
          اكسر;

        حالة 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          اكسر;

        حالة 'T':
          output_format = TEX_FORMAT;
          اكسر;

        حالة 'W':
          کلمه_regex.string = optarg;
          unescape_string (optarg);
          إذا (!*کلمه_regex.string)
            کلمه_regex.string = عدم;
          اكسر;

        حالة FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_دلیلونه, format_vals);
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);
        }
    }

  إذا (کرښه_پلنوالی < 0)
    کرښه_پلنوالی = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  إذا (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      ننوت_file_name = xmalloc (حجم *ننوت_file_name);
      file_کرښه_شمېر = xmalloc (حجم *file_کرښه_شمېر);
      متن_buffers =    xmalloc (حجم *متن_buffers);
      شمېره_ننوت_files = 1;
      ننوت_file_name[0] = عدم;
    }
  وإلا إذا (gnu_extensions)
    {
      شمېره_ننوت_files = argc - optind;
      ننوت_file_name = xnmalloc (شمېره_ننوت_files, حجم *ننوت_file_name);
      file_کرښه_شمېر = xnmalloc (شمېره_ننوت_files, حجم *file_کرښه_شمېر);
      متن_buffers    = xnmalloc (شمېره_ننوت_files, حجم *متن_buffers);

      لكل (file_شاخص = 0; file_شاخص < شمېره_ننوت_files; file_شاخص++)
        {
          إذا (!*argv[optind] || streq (argv[optind], "-"))
            ننوت_file_name[file_شاخص] = عدم;
          وإلا
            ننوت_file_name[file_شاخص] = argv[optind];
          optind++;
        }
    }
  وإلا
    {

      /* There is one necessary input file.  */

      شمېره_ننوت_files = 1;
      ننوت_file_name = xmalloc (حجم *ننوت_file_name);
      file_کرښه_شمېر = xmalloc (حجم *file_کرښه_شمېر);
      متن_buffers    = xmalloc (حجم *متن_buffers);
      إذا (!*argv[optind] || streq (argv[optind], "-"))
        ننوت_file_name[0] = عدم;
      وإلا
        ننوت_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      إذا (optind < argc)
        {
          إذا (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      إذا (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  إذا (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  إذا (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  إذا (ignore_file)
    {
      digest_کلمه_file (ignore_file, &ignore_جدول);
      إذا (ignore_جدول.اوږدتیا == 0)
        ignore_file = عدم;
    }

  إذا (only_file)
    {
      digest_کلمه_file (only_file, &only_جدول);
      إذا (only_جدول.اوږدتیا == 0)
        only_file = عدم;
    }

  /* Prepare to study all the input files.  */

  شمېره_د_occurs[0] = 0;
  ټول_کرښه_شمېر = 0;
  اعظمي_کلمه_اوږدتیا = 0;
  reference_max_پلنوالی = 0;

  لكل (file_شاخص = 0; file_شاخص < شمېره_ننوت_files; file_شاخص++)
    {
      بلاک_2 *متن_بفر = متن_buffers + file_شاخص;

      /* Read the file contents into memory, then study it.  */

      swallow_file_دننه_حافظه (ننوت_file_name[file_شاخص], متن_بفر);
      پیدا_کړه_occurs_دننه_متن (file_شاخص);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      ټول_کرښه_شمېر++;
      file_کرښه_شمېر[file_شاخص] = ټول_کرښه_شمېر;
    }

  /* Do the output process phase.  */

  ترتیبول_وموندل_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  أرجع EXIT_SUCCESS;
}
