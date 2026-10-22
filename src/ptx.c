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
#تعریف_کن برنامه_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#تعریف_کن AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#تعریف_کن CHAR_مجموعه_اندازه 256

/* Debugging the memory allocator.  */

#إذا WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#پایان_اگر

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
ساكن bool درون_داد_reference = false;	/* refs at beginning of input lines */
ساكن bool راست_reference = false;	/* output refs after right context  */
ساكن idx_t سطر_عرض = -1;		/* output line width in characters */
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
بنية regex_داده
{
  /* The original regular expression, as a string.  */
  حرف ثابت *string;

  /* The compiled regular expression, and its fastmap.  */
  بنية re_الگو_میانگیر الگو_2;
  حرف fastmap[UCHAR_MAX + 1];
};

ساكن بنية regex_داده context_regex;	/* end of context */
ساكن بنية regex_داده کلمه_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

تعريفنوع بنية
  {
    حرف *شروع;		/* pointer to beginning of region */
    حرف *انتها;			/* pointer to end + 1 of region */
  }
بلوک_2;

تعريفنوع بنية
  {
    حرف *شروع;		/* pointer to beginning of region */
    idx_t اندازه;			/* length of the region */
  }
کلمه;

تعريفنوع بنية
  {
    idx_t alloc;		/* allocated length */
    idx_t درازی;		/* number of used entries */
    کلمه *شروع COUNTED_BY (درازی);	/* array of WORDs */
  }
کلمه_جدول;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
ساكن بیعلامت حرف folded_chars[CHAR_مجموعه_اندازه];

/* End of context pattern register indices.  */
ساكن بنية re_registers context_regs;

/* Keyword pattern register indices.  */
ساكن بنية re_registers کلمه_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
ساكن حرف کلمه_fastmap[CHAR_مجموعه_اندازه];

/* Maximum length of any word read.  */
ساكن idx_t بیشینه_کلمه_درازی;

/* Maximum width of any reference used.  */
ساكن idx_t reference_max_عرض;

/* Ignore and Only word tables.  */

ساكن کلمه_جدول ignore_جدول;	/* table of words to ignore */
ساكن کلمه_جدول only_جدول;		/* table of words to select */

/* Source text table, and scanning macros.  */

ساكن صحيح عدد_درون_داد_files;	/* number of text input files */
ساكن intmax_t مجموع_سطر_شمار;	/* total number of lines seen so far */
ساكن حرف ثابت **درون_داد_file_name;	/* array of text input file names */
ساكن intmax_t *file_سطر_شمار;	/* array of line count values at end */

ساكن بلوک_2 *متن_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#تعریف_کن SKIP_NON_WHITE(cursor, حد) \
  طالما (cursor < حد && ! آیا_فاصله (مقصد_uchar (*cursor)))		\
    cursor++

#تعریف_کن SKIP_WHITE(cursor, حد) \
  طالما (cursor < حد && آیا_فاصله (مقصد_uchar (*cursor)))		\
    cursor++

#تعریف_کن SKIP_WHITE_BACKWARDS(cursor, شروع) \
  طالما (cursor > شروع && آیا_فاصله (مقصد_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#تعریف_کن SKIP_SOMETHING(cursor, حد) \
  إذا (کلمه_regex.string)						\
    {									\
      regoff_t شمار;							\
      شمار = re_تطابق (&کلمه_regex.الگو_2, cursor, حد - cursor,	\
                        0, عدم);					\
      إذا (شمار == -2)							\
        matcher_error ();						\
      cursor += شمار <= 0 ? 1 : شمار;					\
    }									\
  وإلا إذا (کلمه_fastmap[مقصد_uchar (*cursor)])				\
    طالما (cursor < حد && کلمه_fastmap[مقصد_uchar (*cursor)])		\
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
    کلمه کلید;			/* description of the keyword */
    ptrdiff_t چپ;		/* distance to left context start */
    ptrdiff_t راست;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    صحيح file_شاخص;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

ساكن OCCURS *occurs_جدول[1];	/* all words retained from the read text */
ساكن idx_t occurs_alloc[1];	/* allocated size of occurs_table */
ساكن idx_t عدد_از_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
ساكن حرف edited_flag[CHAR_مجموعه_اندازه];

/* Half of line width, reference excluded.  */
ساكن idx_t half_سطر_عرض;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ساكن ptrdiff_t before_max_عرض;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ساكن ptrdiff_t keyafter_max_عرض;

/* Length of string that flags truncation.  */
ساكن idx_t truncation_string_درازی;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

ساكن بلوک_2 دم;		/* tail field */
ساكن bool دم_truncation;	/* flag truncation after the tail field */

ساكن بلوک_2 before;		/* before field */
ساكن bool before_truncation;	/* flag truncation before the before field */

ساكن بلوک_2 keyafter;		/* keyword-and-after field */
ساكن bool keyafter_truncation; /* flag truncation after the keyafter field */

ساكن بلوک_2 سر;		/* head field */
ساكن bool سر_truncation;	/* flag truncation before the head field */

ساكن بلوک_2 reference;		/* reference field for input reference mode */

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
  صحيح مقدار;			/* value of \nnn escape */
  صحيح درازی;			/* length of \nnn escape */

  cursor = string;

  طالما (*string)
    {
      إذا (*string == '\\')
        {
          string++;
          بدل (*string)
            {
            حالة 'x':		/* \xhhh escape, 3 chars maximum */
              مقدار = 0;
              لكل (درازی = 0, string++;
                   درازی < 3 && c_isxdigit (*string);
                   درازی++, string++)
                مقدار = مقدار * 16 + fromhex (*string);
              إذا (درازی == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              وإلا
                *cursor++ = مقدار;
              اكسر;

            حالة '0':		/* \0ooo escape, 3 chars maximum */
              مقدار = 0;
              لكل (درازی = 0, string++;
                   درازی < 3 && isoct (*string);
                   درازی++, string++)
                مقدار = مقدار * 8 + fromoct (*string);
              *cursor++ = مقدار;
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
compile_regex (بنية regex_داده *regex)
{
  بنية re_الگو_میانگیر *الگو_2 = &regex->الگو_2;
  حرف ثابت *string = regex->string;
  حرف ثابت *پیام_2;

  الگو_2->میانگیر = عدم;
  الگو_2->allocated = 0;
  الگو_2->fastmap = regex->fastmap;
  الگو_2->translate = ignore_case ? folded_chars : عدم;

  پیام_2 = re_compile_الگو (string, طول_رشته (string), الگو_2);
  إذا (پیام_2)
    error (EXIT_FAILURE, 0, _("%s (for regexp %s)"), پیام_2, quote (string));

  /* The fastmap should be compiled before 're_match'.  The following
     call is not mandatory, because 're_search' is always called sooner,
     and it compiles the fastmap if this has not been done yet.  */

  re_compile_fastmap (الگو_2);
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
    لكل (صحيح character = 0; character < CHAR_مجموعه_اندازه; character++)
      folded_chars[character] = به_بزرگ (character);

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
  وإلا إذا (gnu_extensions && !درون_داد_reference)
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

          لكل (صحيح character = 0; character < CHAR_مجموعه_اندازه; character++)
            کلمه_fastmap[character] = !! آیا_حرف (character);
        }
      وإلا
        {

          /* Simulate [^ \t\n]+.  */

          memset (کلمه_fastmap, 1, CHAR_مجموعه_اندازه);
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
swallow_file_درون_حافظه (حرف ثابت *file_name, بلوک_2 *بلوک)
{
  size_t استفاده_شده_درازی;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  إذا (using_stdin)
    بلوک->شروع = fread_file (stdin, 0, &استفاده_شده_درازی);
  وإلا
    بلوک->شروع = read_file (file_name, 0, &استفاده_شده_درازی);

  إذا (!بلوک->شروع)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  إذا (using_stdin)
    clearerr (stdin);

  بلوک->انتها = بلوک->شروع + استفاده_شده_درازی;
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
compare_واژگان (ثابت فراغ *void_اولی, ثابت فراغ *void_دومی)
{
  کلمه ثابت *اولی = void_اولی;
  کلمه ثابت *دومی = void_دومی;
  idx_t درازی = MIN (اولی->اندازه, دومی->اندازه);

  إذا (ignore_case)
    {
      لكل (idx_t counter = 0; counter < درازی; counter++)
        {
          صحيح مقدار = (folded_chars[مقصد_uchar (اولی->شروع[counter])]
                       - folded_chars[مقصد_uchar (دومی->شروع[counter])]);
          إذا (مقدار != 0)
            أرجع مقدار;
        }
    }
  وإلا
    {
      لكل (idx_t counter = 0; counter < درازی; counter++)
        {
          صحيح مقدار = (مقصد_uchar (اولی->شروع[counter])
                       - مقصد_uchar (دومی->شروع[counter]));
          إذا (مقدار != 0)
            أرجع مقدار;
        }
    }

  أرجع _GL_CMP (اولی->اندازه, دومی->اندازه);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

ساكن صحيح
compare_occurs (ثابت فراغ *void_اولی, ثابت فراغ *void_دومی)
{
#تعریف_کن اولی ((ثابت OCCURS *) void_اولی)
#تعریف_کن دومی ((ثابت OCCURS *) void_دومی)
  صحيح مقدار;

  مقدار = compare_واژگان (&اولی->کلید, &دومی->کلید);
  أرجع (مقدار ? مقدار
          : _GL_CMP (اولی->کلید.شروع, دومی->کلید.شروع));
#لغو_تعریف اولی
#لغو_تعریف دومی
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_خالص
ساكن bool
جستجو_جدول (کلمه *کلمه_2, کلمه_جدول *جدول)
{
  idx_t lo = 0;
  idx_t hi = جدول->درازی;
  طالما (lo < hi)
    {
      idx_t میانه = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      صحيح مقدار = compare_واژگان (کلمه_2, جدول->شروع + میانه);
      إذا (مقدار < 0)
        hi = میانه;
      وإلا إذا (مقدار > 0)
        lo = میانه + 1;
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
مرتب_سازی_یافت_occurs (فراغ)
{

  /* Only one language for the time being.  */
  إذا (عدد_از_occurs[0])
    qsort (occurs_جدول[0], عدد_از_occurs[0], حجم **occurs_جدول,
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
  بلوک_2 file_contents;		/* to receive a copy of the file */

  swallow_file_درون_حافظه (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (کلمه_fastmap, 1, CHAR_مجموعه_اندازه);
  لكل (حرف *cursor = file_contents.شروع; cursor < file_contents.انتها;
       cursor++)
    کلمه_fastmap[مقصد_uchar (*cursor)] = 0;

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

  حرر (file_contents.شروع);
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
  بلوک_2 file_contents;		/* to receive a copy of the file */
  حرف *cursor;			/* cursor in file copy */
  حرف *کلمه_شروع;		/* start of the current word */

  swallow_file_درون_حافظه (file_name, &file_contents);

  جدول->شروع = عدم;
  جدول->alloc = 0;
  جدول->درازی = 0;

  /* Read the whole file.  */

  cursor = file_contents.شروع;
  طالما (cursor < file_contents.انتها)
    {

      /* Read one line, and save the word in contains.  */

      کلمه_شروع = cursor;
      طالما (cursor < file_contents.انتها && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      إذا (cursor > کلمه_شروع)
        {
          إذا (جدول->درازی == جدول->alloc)
            جدول->شروع = xpalloc (جدول->شروع, &جدول->alloc, 1, -1,
                                    حجم *جدول->شروع);
          idx_t ثابت جدول_idx = جدول->درازی++;
          جدول->شروع[جدول_idx].شروع = کلمه_شروع;
          جدول->شروع[جدول_idx].اندازه = cursor - کلمه_شروع;
        }

      /* This test allows for an incomplete line at end of file.  */

      إذا (cursor < file_contents.انتها)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (جدول->شروع, جدول->درازی, حجم جدول->شروع[0], compare_واژگان);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

ساكن فراغ
پیدا_کن_occurs_درون_متن (صحيح file_شاخص)
{
  حرف *پیمایش;			/* for scanning the source text */
  حرف *سطر_شروع;		/* start of the current input line */
  حرف *سطر_پیمایش;		/* newlines scanned until this point */
  idx_t reference_درازی;	/* length of reference in input mode */
  کلمه possible_کلید;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  حرف *context_شروع;		/* start of left context */
  حرف *context_انتها;		/* end of right context */
  حرف *کلمه_شروع;		/* start of word */
  حرف *کلمه_انتها;		/* end of word */
  حرف *بعدی_context_شروع;	/* next start of left context */

  ثابت بلوک_2 *متن_میانگیر = &متن_buffers[file_شاخص];

  /* reference_length is always used within 'if (input_reference)'.
     However, GNU C diagnoses that it may be used uninitialized.  The
     following assignment is merely to shut it up.  */

  reference_درازی = 0;

  /* Tracking where lines start is helpful for reference processing.  In
     auto reference mode, this allows counting lines.  In input reference
     mode, this permits finding the beginning of the references.

     The first line begins with the file, skip immediately this very first
     reference in input reference mode, to help further rejection any word
     found inside it.  Also, unconditionally assigning these variable has
     the happy effect of shutting up lint.  */

  سطر_شروع = متن_میانگیر->شروع;
  سطر_پیمایش = سطر_شروع;
  إذا (درون_داد_reference)
    {
      SKIP_NON_WHITE (سطر_پیمایش, متن_میانگیر->انتها);
      reference_درازی = سطر_پیمایش - سطر_شروع;
      SKIP_WHITE (سطر_پیمایش, متن_میانگیر->انتها);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  لكل (حرف *cursor = متن_میانگیر->شروع;
       cursor < متن_میانگیر->انتها;
       cursor = بعدی_context_شروع)
    {

      /* 'context_start' gets initialized before the processing of each
         line, or once for the whole buffer if no end of line or sentence
         sequence separator.  */

      context_شروع = cursor;

      /* If an end of line or end of sentence sequence is defined and
         non-empty, 'next_context_start' will be recomputed to be the end of
         each line or sentence, before each one is processed.  If no such
         sequence, then 'next_context_start' is set at the end of the whole
         buffer, which is then considered to be a single line or sentence.
         This test also accounts for the case of an incomplete line or
         sentence at the end of the buffer.  */

      بعدی_context_شروع = متن_میانگیر->انتها;
      إذا (context_regex.string)
        بدل (re_جستجو (&context_regex.الگو_2, cursor,
                           متن_میانگیر->انتها - cursor,
                           0, متن_میانگیر->انتها - cursor, &context_regs))
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
            بعدی_context_شروع = cursor + context_regs.انتها[0];
            اكسر;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_انتها = بعدی_context_شروع;
      SKIP_WHITE_BACKWARDS (context_انتها, context_شروع);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      طالما (true)
        {
          إذا (کلمه_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_جستجو (&کلمه_regex.الگو_2, cursor,
                                      context_انتها - cursor,
                                      0, context_انتها - cursor, &کلمه_regs);
              إذا (r == -2)
                matcher_error ();
              إذا (r == -1)
                اكسر;
              کلمه_شروع = cursor + کلمه_regs.شروع[0];
              کلمه_انتها = cursor + کلمه_regs.انتها[0];
            }
          وإلا

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              پیمایش = cursor;
              طالما (پیمایش < context_انتها
                     && !کلمه_fastmap[مقصد_uchar (*پیمایش)])
                پیمایش++;

              إذا (پیمایش == context_انتها)
                اكسر;

              کلمه_شروع = پیمایش;

              طالما (پیمایش < context_انتها
                     && کلمه_fastmap[مقصد_uchar (*پیمایش)])
                پیمایش++;

              کلمه_انتها = پیمایش;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = کلمه_شروع;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          إذا (کلمه_انتها == کلمه_شروع)
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

          possible_کلید.شروع = cursor;
          possible_کلید.اندازه = کلمه_انتها - کلمه_شروع;
          cursor += possible_کلید.اندازه;

          إذا (possible_کلید.اندازه > بیشینه_کلمه_درازی)
            بیشینه_کلمه_درازی = possible_کلید.اندازه;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          إذا (درون_داد_reference)
            {
              طالما (سطر_پیمایش < possible_کلید.شروع)
                إذا (*سطر_پیمایش == '\n')
                  {
                    مجموع_سطر_شمار++;
                    سطر_پیمایش++;
                    سطر_شروع = سطر_پیمایش;
                    SKIP_NON_WHITE (سطر_پیمایش, متن_میانگیر->انتها);
                    reference_درازی = سطر_پیمایش - سطر_شروع;
                  }
                وإلا
                  سطر_پیمایش++;
              إذا (سطر_پیمایش > possible_کلید.شروع)
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

          إذا (ignore_file && جستجو_جدول (&possible_کلید, &ignore_جدول))
            استمر;
          إذا (only_file && !جستجو_جدول (&possible_کلید, &only_جدول))
            استمر;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          إذا (عدد_از_occurs[0] == occurs_alloc[0])
            occurs_جدول[0] = xpalloc (occurs_جدول[0], &occurs_alloc[0],
                                       1, -1, حجم *occurs_جدول[0]);
          occurs_cursor = occurs_جدول[0] + عدد_از_occurs[0];

          /* Define the reference field, if any.  */

          إذا (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              طالما (سطر_پیمایش < possible_کلید.شروع)
                إذا (*سطر_پیمایش == '\n')
                  {
                    مجموع_سطر_شمار++;
                    سطر_پیمایش++;
                    سطر_شروع = سطر_پیمایش;
                    SKIP_NON_WHITE (سطر_پیمایش, متن_میانگیر->انتها);
                  }
                وإلا
                  سطر_پیمایش++;

              occurs_cursor->reference = مجموع_سطر_شمار;
            }
          وإلا إذا (درون_داد_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = سطر_شروع - possible_کلید.شروع;
              إذا (reference_درازی > reference_max_عرض)
                reference_max_عرض = reference_درازی;
            }

          /* Exclude the reference from the context in simple cases.  */

          إذا (درون_داد_reference && سطر_شروع == context_شروع)
            {
              SKIP_NON_WHITE (context_شروع, context_انتها);
              SKIP_WHITE (context_شروع, context_انتها);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->کلید = possible_کلید;
          occurs_cursor->چپ = context_شروع - possible_کلید.شروع;
          occurs_cursor->راست = context_انتها - possible_کلید.شروع;
          occurs_cursor->file_شاخص = file_شاخص;

          عدد_از_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

ساكن فراغ
print_spaces (ptrdiff_t عدد)
{
  لكل (ptrdiff_t counter = عدد; counter > 0; counter--)
    حرف_چاپ_کن (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

ساكن فراغ
print_فیلد (بلوک_2 فیلد)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  لكل (حرف *cursor = فیلد.شروع; cursor < فیلد.انتها; cursor++)
    {
      بیعلامت حرف character = *cursor;
      إذا (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          بدل (character)
            {
            حالة '"':
              /* In roff output format, double any quote.  */
              حرف_چاپ_کن ('"');
              حرف_چاپ_کن ('"');
              اكسر;

            حالة '$':
            حالة '%':
            حالة '&':
            حالة '#':
            حالة '_':
              /* In TeX output format, precede these with a backslash.  */
              حرف_چاپ_کن ('\\');
              حرف_چاپ_کن (character);
              اكسر;

            حالة '{':
            حالة '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              چاپ_کن ("$\\%c$", character);
              اكسر;

            حالة '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              اكسر;

            افتراضي:
              /* Any other flagged character produces a single space.  */
              حرف_چاپ_کن (' ');
            }
        }
      وإلا
        حرف_چاپ_کن (*cursor);
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
      reference_max_عرض = 0;
      لكل (صحيح file_شاخص = 0; file_شاخص < عدد_درون_داد_files; file_شاخص++)
        {
          intmax_t سطر_ordinal = file_سطر_شمار[file_شاخص] + 1;
          إذا (file_شاخص > 0)
            سطر_ordinal -= file_سطر_شمار[file_شاخص - 1];
          حرف ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_عرض = sprintf (ordinal_string, "%jd", سطر_ordinal);
          إذا (درون_داد_file_name[file_شاخص])
            reference_عرض += طول_رشته (درون_داد_file_name[file_شاخص]);
          إذا (reference_عرض > reference_max_عرض)
            reference_max_عرض = reference_عرض;
        }
      reference_max_عرض++;
      reference.شروع = xmalloc (reference_max_عرض + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  إذا ((auto_reference || درون_داد_reference) && !راست_reference)
    سطر_عرض = MAX (0, سطر_عرض - (reference_max_عرض + gap_اندازه));

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

  half_سطر_عرض = سطر_عرض >> 1;
  before_max_عرض = half_سطر_عرض - gap_اندازه;
  keyafter_max_عرض = half_سطر_عرض;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  إذا (truncation_string && *truncation_string)
    truncation_string_درازی = طول_رشته (truncation_string);
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

      before_max_عرض -= 2 * truncation_string_درازی;
      keyafter_max_عرض -= 2 * truncation_string_درازی;
    }
  وإلا
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_عرض -= 2 * truncation_string_درازی + 1;
    }

  إذا (before_max_عرض < 0)
    before_max_عرض = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  لكل (صحيح character = 0; character < CHAR_مجموعه_اندازه; character++)
    edited_flag[character] = !! آیا_فاصله (character);
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
        edited_flag[مقصد_uchar (*cursor)] = 1;

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
  ptrdiff_t دم_max_عرض;	/* allowable width of tail field */
  ptrdiff_t سر_max_عرض;	/* allowable width of head field */
  حرف *cursor;			/* running cursor in source text */
  حرف *چپ_context_شروع;	/* start of left context */
  حرف *راست_context_انتها;	/* end of right context */
  حرف *چپ_فیلد_شروع;	/* conservative start for 'head'/'before' */
  حرف ثابت *file_name;	/* file name for reference */
  intmax_t سطر_ordinal;	/* line ordinal for reference */
  حرف ثابت *میانگیر_شروع;	/* start of buffered file for this occurs */
  حرف ثابت *میانگیر_انتها;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.شروع = occurs->کلید.شروع;
  keyafter.انتها = keyafter.شروع + occurs->کلید.اندازه;
  چپ_context_شروع = keyafter.شروع + occurs->چپ;
  راست_context_انتها = keyafter.شروع + occurs->راست;

  میانگیر_شروع = متن_buffers[occurs->file_شاخص].شروع;
  میانگیر_انتها = متن_buffers[occurs->file_شاخص].انتها;

  cursor = keyafter.انتها;
  طالما (cursor < راست_context_انتها
         && cursor <= keyafter.شروع + keyafter_max_عرض)
    {
      keyafter.انتها = cursor;
      SKIP_SOMETHING (cursor, راست_context_انتها);
    }
  إذا (cursor <= keyafter.شروع + keyafter_max_عرض)
    keyafter.انتها = cursor;

  keyafter_truncation = truncation_string && keyafter.انتها < راست_context_انتها;

  SKIP_WHITE_BACKWARDS (keyafter.انتها, keyafter.شروع);

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

  إذا (-occurs->چپ > half_سطر_عرض + بیشینه_کلمه_درازی)
    {
      چپ_فیلد_شروع
        = keyafter.شروع - (half_سطر_عرض + بیشینه_کلمه_درازی);
      SKIP_SOMETHING (چپ_فیلد_شروع, keyafter.شروع);
    }
  وإلا
    چپ_فیلد_شروع = keyafter.شروع + occurs->چپ;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.شروع = چپ_فیلد_شروع;
  before.انتها = keyafter.شروع;
  SKIP_WHITE_BACKWARDS (before.انتها, before.شروع);

  طالما (before.شروع + before_max_عرض < before.انتها)
    SKIP_SOMETHING (before.شروع, before.انتها);

  إذا (truncation_string)
    {
      cursor = before.شروع;
      SKIP_WHITE_BACKWARDS (cursor, میانگیر_شروع);
      before_truncation = cursor > چپ_context_شروع;
    }
  وإلا
    before_truncation = false;

  SKIP_WHITE (before.شروع, میانگیر_انتها);

  /* The tail could not take more columns than what has been left in the
     left context field, and a gap is mandatory.  It starts after the
     right context, and does not contain prefixed spaces.  It ends at
     the end of line, the end of buffer or when the tail field is full,
     whichever comes first.  It cannot contain only part of a word, and
     has no suffixed spaces.  */

  دم_max_عرض
    = before_max_عرض - (before.انتها - before.شروع) - gap_اندازه;

  إذا (دم_max_عرض > 0)
    {
      دم.شروع = keyafter.انتها;
      SKIP_WHITE (دم.شروع, میانگیر_انتها);

      دم.انتها = دم.شروع;
      cursor = دم.انتها;
      طالما (cursor < راست_context_انتها
             && cursor < دم.شروع + دم_max_عرض)
        {
          دم.انتها = cursor;
          SKIP_SOMETHING (cursor, راست_context_انتها);
        }

      إذا (cursor < دم.شروع + دم_max_عرض)
        دم.انتها = cursor;

      إذا (دم.انتها > دم.شروع)
        {
          keyafter_truncation = false;
          دم_truncation = truncation_string && دم.انتها < راست_context_انتها;
        }
      وإلا
        دم_truncation = false;

      SKIP_WHITE_BACKWARDS (دم.انتها, دم.شروع);
    }
  وإلا
    {

      /* No place left for a tail field.  */

      دم.شروع = عدم;
      دم.انتها = عدم;
      دم_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  سر_max_عرض
    = keyafter_max_عرض - (keyafter.انتها - keyafter.شروع) - gap_اندازه;

  إذا (سر_max_عرض > 0)
    {
      سر.انتها = before.شروع;
      SKIP_WHITE_BACKWARDS (سر.انتها, میانگیر_شروع);

      سر.شروع = چپ_فیلد_شروع;
      طالما (سر.شروع + سر_max_عرض < سر.انتها)
        SKIP_SOMETHING (سر.شروع, سر.انتها);

      إذا (سر.انتها > سر.شروع)
        {
          before_truncation = false;
          سر_truncation = (truncation_string
                             && سر.شروع > چپ_context_شروع);
        }
      وإلا
        سر_truncation = false;

      SKIP_WHITE (سر.شروع, سر.انتها);
    }
  وإلا
    {

      /* No place left for a head field.  */

      سر.شروع = عدم;
      سر.انتها = عدم;
      سر_truncation = false;
    }

  إذا (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = درون_داد_file_name[occurs->file_شاخص];
      إذا (!file_name)
        file_name = "";

      سطر_ordinal = occurs->reference + 1;
      إذا (occurs->file_شاخص > 0)
        سطر_ordinal -= file_سطر_شمار[occurs->file_شاخص - 1];

      حرف *file_انتها = stpcpy (reference.شروع, file_name);
      reference.انتها = file_انتها + sprintf (file_انتها, ":%jd", سطر_ordinal);
    }
  وإلا إذا (درون_داد_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.شروع = keyafter.شروع + occurs->reference;
      reference.انتها = reference.شروع;
      SKIP_NON_WHITE (reference.انتها, راست_context_انتها);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

ساكن فراغ
output_one_roff_سطر (فراغ)
{
  /* Output the 'tail' field.  */

  چاپ_کن (".%s \"", macro_name);
  print_فیلد (دم);
  إذا (دم_truncation)
    fputs (truncation_string, stdout);
  حرف_چاپ_کن ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  إذا (before_truncation)
    fputs (truncation_string, stdout);
  print_فیلد (before);
  حرف_چاپ_کن ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_فیلد (keyafter);
  إذا (keyafter_truncation)
    fputs (truncation_string, stdout);
  حرف_چاپ_کن ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  إذا (سر_truncation)
    fputs (truncation_string, stdout);
  print_فیلد (سر);
  حرف_چاپ_کن ('"');

  /* Conditionally output the 'reference' field.  */

  إذا (auto_reference || درون_داد_reference)
    {
      fputs (" \"", stdout);
      print_فیلد (reference);
      حرف_چاپ_کن ('"');
    }

  حرف_چاپ_کن ('\n');
}

/*---------------------------------------------------------.
| Output the current output fields as one line for 'TeX'.  |
`---------------------------------------------------------*/

ساكن فراغ
output_one_tex_سطر (فراغ)
{
  بلوک_2 کلید;			/* key field, isolated */
  بلوک_2 after;			/* after field, isolated */
  حرف *cursor;			/* running cursor in source text */

  چاپ_کن ("\\%s ", macro_name);
  حرف_چاپ_کن ('{');
  print_فیلد (دم);
  fputs ("}{", stdout);
  print_فیلد (before);
  fputs ("}{", stdout);
  کلید.شروع = keyafter.شروع;
  after.انتها = keyafter.انتها;
  cursor = keyafter.شروع;
  SKIP_SOMETHING (cursor, keyafter.انتها);
  کلید.انتها = cursor;
  after.شروع = cursor;
  print_فیلد (کلید);
  fputs ("}{", stdout);
  print_فیلد (after);
  fputs ("}{", stdout);
  print_فیلد (سر);
  حرف_چاپ_کن ('}');
  إذا (auto_reference || درون_داد_reference)
    {
      حرف_چاپ_کن ('{');
      print_فیلد (reference);
      حرف_چاپ_کن ('}');
    }
  حرف_چاپ_کن ('\n');
}

/*-------------------------------------------------------------------.
| Output the current output fields as one line for a dumb terminal.  |
`-------------------------------------------------------------------*/

ساكن فراغ
output_one_dumb_سطر (فراغ)
{
  إذا (!راست_reference)
    {
      إذا (auto_reference)
        {

          /* Output the 'reference' field, in such a way that GNU emacs
             next-error will handle it.  The ending colon is taken from the
             gap which follows.  */

          print_فیلد (reference);
          حرف_چاپ_کن (':');
          print_spaces (reference_max_عرض
                        + gap_اندازه
                        - (reference.انتها - reference.شروع)
                        - 1);
        }
      وإلا
        {

          /* Output the 'reference' field and its following gap.  */

          print_فیلد (reference);
          print_spaces (reference_max_عرض
                        + gap_اندازه
                        - (reference.انتها - reference.شروع));
        }
    }

  إذا (دم.شروع < دم.انتها)
    {
      /* Output the 'tail' field.  */

      print_فیلد (دم);
      إذا (دم_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_سطر_عرض - gap_اندازه
                    - (before.انتها - before.شروع)
                    - (before_truncation ? truncation_string_درازی : 0)
                    - (دم.انتها - دم.شروع)
                    - (دم_truncation ? truncation_string_درازی : 0));
    }
  وإلا
    print_spaces (half_سطر_عرض - gap_اندازه
                  - (before.انتها - before.شروع)
                  - (before_truncation ? truncation_string_درازی : 0));

  /* Output the 'before' field.  */

  إذا (before_truncation)
    fputs (truncation_string, stdout);
  print_فیلد (before);

  print_spaces (gap_اندازه);

  /* Output the 'keyafter' field.  */

  print_فیلد (keyafter);
  إذا (keyafter_truncation)
    fputs (truncation_string, stdout);

  إذا (سر.شروع < سر.انتها)
    {
      /* Output the 'head' field.  */

      print_spaces (half_سطر_عرض
                    - (keyafter.انتها - keyafter.شروع)
                    - (keyafter_truncation ? truncation_string_درازی : 0)
                    - (سر.انتها - سر.شروع)
                    - (سر_truncation ? truncation_string_درازی : 0));
      إذا (سر_truncation)
        fputs (truncation_string, stdout);
      print_فیلد (سر);
    }
  وإلا

    إذا ((auto_reference || درون_داد_reference) && راست_reference)
      print_spaces (half_سطر_عرض
                    - (keyafter.انتها - keyafter.شروع)
                    - (keyafter_truncation ? truncation_string_درازی : 0));

  إذا ((auto_reference || درون_داد_reference) && راست_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_اندازه);
      print_فیلد (reference);
    }

  حرف_چاپ_کن ('\n');
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

  دم.شروع = عدم;
  دم.انتها = عدم;
  دم_truncation = false;

  سر.شروع = عدم;
  سر.انتها = عدم;
  سر_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_جدول[0];

  لكل (idx_t occurs_شاخص = 0; occurs_شاخص < عدد_از_occurs[0];
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
          output_one_dumb_سطر ();
          اكسر;

        حالة ROFF_FORMAT:
          output_one_roff_سطر ();
          اكسر;

        حالة TEX_FORMAT:
          output_one_tex_سطر ();
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
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [درون_داد_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [درون_داد_2 [OUTPUT]]\n"),
              برنامه_name, برنامه_name);
      fputs (_("\
Output a permuted index, including context, از the واژگان درون the درون_داد files.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --خودکار-reference           output automatically generated references\n\
"));
      oputs (_("\
  -G, --traditional              behave بیشتر like System V 'ptx'\n\
"));
      oputs (_("\
  -F, --flag-truncation=STRING   use STRING لكل flagging سطر truncations.\n\
                                 The افتراضي is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name مقصد use instead از 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --راست-side-refs          put references at راست, not counted درون -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   لكل انتها از سطرها or انتها از sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --کلمه_2-regexp=REGEXP       use REGEXP مقصد تطابق each keyword\n\
"));
      oputs (_("\
  -b, --اكسر-file=FILE          کلمه_2 اكسر characters درون this FILE\n\
"));
      oputs (_("\
  -f, --ignore-حالة              fold پایینی حالة مقصد فوقانی حالة لكل ترتیبدهی\n\
"));
      oputs (_("\
  -g, --gap-اندازه=عدد_2          gap اندازه درون columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         اقرأ ignore کلمه_2 list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           اقرأ only کلمه_2 list from this FILE\n\
"));
      oputs (_("\
  -r, --references               اولی فیلد از each سطر is a reference\n\
"));
      oputs (_("\
  -t, --typeset-نما_2             change the افتراضي عرض from 72 مقصد 100\n\
"));
      oputs (_("\
  -w, --عرض=عدد_2             output عرض درون columns, reference excluded\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
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
  {"auto-reference", no_آرگومان, عدم, 'A'},
  {"break-file", required_آرگومان, عدم, 'b'},
  {"flag-truncation", required_آرگومان, عدم, 'F'},
  {"ignore-case", no_آرگومان, عدم, 'f'},
  {"gap-size", required_آرگومان, عدم, 'g'},
  {"ignore-file", required_آرگومان, عدم, 'i'},
  {"macro-name", required_آرگومان, عدم, 'M'},
  {"only-file", required_آرگومان, عدم, 'o'},
  {"references", no_آرگومان, عدم, 'r'},
  {"right-side-refs", no_آرگومان, عدم, 'R'},
  {"format", required_آرگومان, عدم, FORMAT_OPTION},
  {"sentence-regexp", required_آرگومان, عدم, 'S'},
  {"traditional", no_آرگومان, عدم, 'G'},
  {"typeset-mode", no_آرگومان, عدم, 't'},
  {"width", required_آرگومان, عدم, 'w'},
  {"word-regexp", required_آرگومان, عدم, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

ساكن حرف ثابت *ثابت format_آرگومانها[] =
{
  "roff", "tex", عدم
};

ساكن تعداد Format ثابت format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح optchar;			/* argument character */
  صحيح file_شاخص;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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
          درون_داد_reference = true;
          اكسر;

        حالة 't':
          إذا (سطر_عرض < 0)
            سطر_عرض = 100;
          اكسر;

        حالة 'w':
          {
            intmax_t tmp;
            إذا (! (xstrtoimax (optarg, عدم, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            سطر_عرض = tmp;
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
          راست_reference = true;
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
                                     format_آرگومانها, format_vals);
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);
        }
    }

  إذا (سطر_عرض < 0)
    سطر_عرض = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  إذا (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      درون_داد_file_name = xmalloc (حجم *درون_داد_file_name);
      file_سطر_شمار = xmalloc (حجم *file_سطر_شمار);
      متن_buffers =    xmalloc (حجم *متن_buffers);
      عدد_درون_داد_files = 1;
      درون_داد_file_name[0] = عدم;
    }
  وإلا إذا (gnu_extensions)
    {
      عدد_درون_داد_files = argc - optind;
      درون_داد_file_name = xnmalloc (عدد_درون_داد_files, حجم *درون_داد_file_name);
      file_سطر_شمار = xnmalloc (عدد_درون_داد_files, حجم *file_سطر_شمار);
      متن_buffers    = xnmalloc (عدد_درون_داد_files, حجم *متن_buffers);

      لكل (file_شاخص = 0; file_شاخص < عدد_درون_داد_files; file_شاخص++)
        {
          إذا (!*argv[optind] || streq (argv[optind], "-"))
            درون_داد_file_name[file_شاخص] = عدم;
          وإلا
            درون_داد_file_name[file_شاخص] = argv[optind];
          optind++;
        }
    }
  وإلا
    {

      /* There is one necessary input file.  */

      عدد_درون_داد_files = 1;
      درون_داد_file_name = xmalloc (حجم *درون_داد_file_name);
      file_سطر_شمار = xmalloc (حجم *file_سطر_شمار);
      متن_buffers    = xmalloc (حجم *متن_buffers);
      إذا (!*argv[optind] || streq (argv[optind], "-"))
        درون_داد_file_name[0] = عدم;
      وإلا
        درون_داد_file_name[0] = argv[optind];
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
      إذا (ignore_جدول.درازی == 0)
        ignore_file = عدم;
    }

  إذا (only_file)
    {
      digest_کلمه_file (only_file, &only_جدول);
      إذا (only_جدول.درازی == 0)
        only_file = عدم;
    }

  /* Prepare to study all the input files.  */

  عدد_از_occurs[0] = 0;
  مجموع_سطر_شمار = 0;
  بیشینه_کلمه_درازی = 0;
  reference_max_عرض = 0;

  لكل (file_شاخص = 0; file_شاخص < عدد_درون_داد_files; file_شاخص++)
    {
      بلوک_2 *متن_میانگیر = متن_buffers + file_شاخص;

      /* Read the file contents into memory, then study it.  */

      swallow_file_درون_حافظه (درون_داد_file_name[file_شاخص], متن_میانگیر);
      پیدا_کن_occurs_درون_متن (file_شاخص);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      مجموع_سطر_شمار++;
      file_سطر_شمار[file_شاخص] = مجموع_سطر_شمار;
    }

  /* Do the output process phase.  */

  مرتب_سازی_یافت_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  أرجع EXIT_SUCCESS;
}
