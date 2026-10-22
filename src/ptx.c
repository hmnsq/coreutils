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

#شامل <config.h>

#شامل <getopt.h>
#شامل <sys/types.h>
#شامل "system.h"
#شامل <regex.h>
#شامل "argmatch.h"
#شامل "c-ctype.h"
#شامل "fadvise.h"
#شامل "octhexdigits.h"
#شامل "quote.h"
#شامل "read-file.h"
#شامل "stdio--.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#تعریف_کرو AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#تعریف_کرو CHAR_سیٹ_سائز 256

/* Debugging the memory allocator.  */

#اگر WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#اگر_ختم

/* Global definitions.  */

/* FIXME: There are many unchecked integer overflows in this file,
   and in theory they could cause this command to have undefined
   behavior given large inputs or options.  This command should
   diagnose any such overflow and exit.  */

/* Program options.  */

شمار Format
{
  UNKNOWN_FORMAT,		/* output format still unknown */
  DUMB_FORMAT,			/* output for a dumb terminal */
  ROFF_FORMAT,			/* output for 'troff' or 'nroff' */
  TEX_FORMAT			/* output for 'TeX' or 'LaTeX' */
};

ساکن bool gnu_extensions = true;	/* trigger all GNU extensions */
ساکن bool auto_reference = false;	/* refs are 'file_name:line_number:' */
ساکن bool داخلہ_reference = false;	/* refs at beginning of input lines */
ساکن bool دایاں_reference = false;	/* output refs after right context  */
ساکن idx_t سطر_چوڑائی = -1;		/* output line width in characters */
ساکن idx_t gap_سائز = 3;	/* number of spaces between output fields */
ساکن حرف ثابت *truncation_string = "/";
                                /* string used to mark line truncations */
ساکن حرف ثابت *macro_name = "xx";	/* macro name for roff or TeX output */
ساکن شمار Format output_format = UNKNOWN_FORMAT;
                                /* output format */

ساکن bool ignore_case = false;	/* fold lower to upper for sorting */
ساکن حرف ثابت *break_file = تہی; /* name of the 'Break chars' file */
ساکن حرف ثابت *only_file = تہی;	/* name of the 'Only words' file */
ساکن حرف ثابت *ignore_file = تہی; /* name of the 'Ignore words' file */

/* Options that use regular expressions.  */
ساخت regex_ڈیٹا
{
  /* The original regular expression, as a string.  */
  حرف ثابت *string;

  /* The compiled regular expression, and its fastmap.  */
  ساخت re_نمونہ_بفر نمونہ_2;
  حرف fastmap[UCHAR_MAX + 1];
};

ساکن ساخت regex_ڈیٹا context_regex;	/* end of context */
ساکن ساخت regex_ڈیٹا لفظ_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

قسمتعریف ساخت
  {
    حرف *آغاز;		/* pointer to beginning of region */
    حرف *انتہا;			/* pointer to end + 1 of region */
  }
بلاک_2;

قسمتعریف ساخت
  {
    حرف *آغاز;		/* pointer to beginning of region */
    idx_t سائز;			/* length of the region */
  }
لفظ;

قسمتعریف ساخت
  {
    idx_t alloc;		/* allocated length */
    idx_t لمبائی;		/* number of used entries */
    لفظ *آغاز COUNTED_BY (لمبائی);	/* array of WORDs */
  }
لفظ_جدول;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
ساکن غیرعلامتی حرف folded_chars[CHAR_سیٹ_سائز];

/* End of context pattern register indices.  */
ساکن ساخت re_registers context_regs;

/* Keyword pattern register indices.  */
ساکن ساخت re_registers لفظ_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
ساکن حرف لفظ_fastmap[CHAR_سیٹ_سائز];

/* Maximum length of any word read.  */
ساکن idx_t زیادہ_سے_زیادہ_لفظ_لمبائی;

/* Maximum width of any reference used.  */
ساکن idx_t reference_max_چوڑائی;

/* Ignore and Only word tables.  */

ساکن لفظ_جدول ignore_جدول;	/* table of words to ignore */
ساکن لفظ_جدول only_جدول;		/* table of words to select */

/* Source text table, and scanning macros.  */

ساکن صحیح نمبر_داخلہ_files;	/* number of text input files */
ساکن intmax_t کل_سطر_گنتی;	/* total number of lines seen so far */
ساکن حرف ثابت **داخلہ_file_name;	/* array of text input file names */
ساکن intmax_t *file_سطر_گنتی;	/* array of line count values at end */

ساکن بلاک_2 *متن_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#تعریف_کرو SKIP_NON_WHITE(cursor, حد_بندی) \
  جب (cursor < حد_بندی && ! خلا_ہے (مقصد_uchar (*cursor)))		\
    cursor++

#تعریف_کرو SKIP_WHITE(cursor, حد_بندی) \
  جب (cursor < حد_بندی && خلا_ہے (مقصد_uchar (*cursor)))		\
    cursor++

#تعریف_کرو SKIP_WHITE_BACKWARDS(cursor, آغاز) \
  جب (cursor > آغاز && خلا_ہے (مقصد_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#تعریف_کرو SKIP_SOMETHING(cursor, حد_بندی) \
  اگر (لفظ_regex.string)						\
    {									\
      regoff_t گنتی;							\
      گنتی = re_مطابقت (&لفظ_regex.نمونہ_2, cursor, حد_بندی - cursor,	\
                        0, تہی);					\
      اگر (گنتی == -2)							\
        matcher_error ();						\
      cursor += گنتی <= 0 ? 1 : گنتی;					\
    }									\
  ورنہ اگر (لفظ_fastmap[مقصد_uchar (*cursor)])				\
    جب (cursor < حد_بندی && لفظ_fastmap[مقصد_uchar (*cursor)])		\
      cursor++;								\
  ورنہ									\
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

قسمتعریف ساخت
  {
    لفظ کلید;			/* description of the keyword */
    ptrdiff_t بایاں;		/* distance to left context start */
    ptrdiff_t دایاں;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    صحیح file_اشاریہ;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

ساکن OCCURS *occurs_جدول[1];	/* all words retained from the read text */
ساکن idx_t occurs_alloc[1];	/* allocated size of occurs_table */
ساکن idx_t نمبر_کا_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
ساکن حرف edited_flag[CHAR_سیٹ_سائز];

/* Half of line width, reference excluded.  */
ساکن idx_t half_سطر_چوڑائی;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ساکن ptrdiff_t before_max_چوڑائی;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ساکن ptrdiff_t keyafter_max_چوڑائی;

/* Length of string that flags truncation.  */
ساکن idx_t truncation_string_لمبائی;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

ساکن بلاک_2 دم;		/* tail field */
ساکن bool دم_truncation;	/* flag truncation after the tail field */

ساکن بلاک_2 before;		/* before field */
ساکن bool before_truncation;	/* flag truncation before the before field */

ساکن بلاک_2 keyafter;		/* keyword-and-after field */
ساکن bool keyafter_truncation; /* flag truncation after the keyafter field */

ساکن بلاک_2 سر;		/* head field */
ساکن bool سر_truncation;	/* flag truncation before the head field */

ساکن بلاک_2 reference;		/* reference field for input reference mode */

/* Miscellaneous routines.  */

/* Diagnose an error in the regular expression matcher.  Then exit.  */

ساکن _Noreturn خالی
matcher_error (خالی)
{
  error (EXIT_FAILURE, errno, _("error in regular expression matcher"));
}

/* Unescape STRING in-place.  */

ساکن خالی
unescape_string (حرف *string)
{
  حرف *cursor;			/* cursor in result */
  صحیح قدر;			/* value of \nnn escape */
  صحیح لمبائی;			/* length of \nnn escape */

  cursor = string;

  جب (*string)
    {
      اگر (*string == '\\')
        {
          string++;
          انتخاب (*string)
            {
            حالت 'x':		/* \xhhh escape, 3 chars maximum */
              قدر = 0;
              برائے (لمبائی = 0, string++;
                   لمبائی < 3 && c_isxdigit (*string);
                   لمبائی++, string++)
                قدر = قدر * 16 + fromhex (*string);
              اگر (لمبائی == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              ورنہ
                *cursor++ = قدر;
              توڑو;

            حالت '0':		/* \0ooo escape, 3 chars maximum */
              قدر = 0;
              برائے (لمبائی = 0, string++;
                   لمبائی < 3 && isoct (*string);
                   لمبائی++, string++)
                قدر = قدر * 8 + fromoct (*string);
              *cursor++ = قدر;
              توڑو;

            حالت 'a':		/* alert */
              *cursor++ = '\a';
              string++;
              توڑو;

            حالت 'b':		/* backspace */
              *cursor++ = '\b';
              string++;
              توڑو;

            حالت 'c':		/* cancel the rest of the output */
              جب (*string)
                string++;
              توڑو;

            حالت 'f':		/* form feed */
              *cursor++ = '\f';
              string++;
              توڑو;

            حالت 'n':		/* new line */
              *cursor++ = '\n';
              string++;
              توڑو;

            حالت 'r':		/* carriage return */
              *cursor++ = '\r';
              string++;
              توڑو;

            حالت 't':		/* horizontal tab */
              *cursor++ = '\t';
              string++;
              توڑو;

            حالت 'v':		/* vertical tab */
              *cursor++ = '\v';
              string++;
              توڑو;

            حالت '\0':		/* lone backslash at end of string */
              /* ignore it */
              توڑو;

            طےشدہ:
              *cursor++ = '\\';
              *cursor++ = *string++;
              توڑو;
            }
        }
      ورنہ
        *cursor++ = *string++;
    }

  *cursor = '\0';
}

/*--------------------------------------------------------------------------.
| Compile the regex represented by REGEX, diagnose and abort if any error.  |
`--------------------------------------------------------------------------*/

ساکن خالی
compile_regex (ساخت regex_ڈیٹا *regex)
{
  ساخت re_نمونہ_بفر *نمونہ_2 = &regex->نمونہ_2;
  حرف ثابت *string = regex->string;
  حرف ثابت *پیغام_2;

  نمونہ_2->بفر = تہی;
  نمونہ_2->allocated = 0;
  نمونہ_2->fastmap = regex->fastmap;
  نمونہ_2->translate = ignore_case ? folded_chars : تہی;

  پیغام_2 = re_compile_نمونہ (string, لڑی_لمبائی (string), نمونہ_2);
  اگر (پیغام_2)
    error (EXIT_FAILURE, 0, _("%s (for regexp %s)"), پیغام_2, quote (string));

  /* The fastmap should be compiled before 're_match'.  The following
     call is not mandatory, because 're_search' is always called sooner,
     and it compiles the fastmap if this has not been done yet.  */

  re_compile_fastmap (نمونہ_2);
}

/*------------------------------------------------------------------------.
| This will initialize various tables for pattern match and compiles some |
| regexps.								  |
`------------------------------------------------------------------------*/

ساکن خالی
initialize_regex (خالی)
{
  /* Initialize the case folding table.  */

  اگر (ignore_case)
    برائے (صحیح character = 0; character < CHAR_سیٹ_سائز; character++)
      folded_chars[character] = بڑا_حرف (character);

  /* Unless the user already provided a description of the end of line or
     end of sentence sequence, select an end of line sequence to compile.
     If the user provided an empty definition, thus disabling end of line
     or sentence feature, make it null to speed up tests.  If GNU
     extensions are enabled, use end of sentence like in GNU emacs.  If
     disabled, use end of lines.  */

  اگر (context_regex.string)
    {
      اگر (!*context_regex.string)
        context_regex.string = تہی;
    }
  ورنہ اگر (gnu_extensions && !داخلہ_reference)
    context_regex.string = "[.?!][]\"')}]*\\($\\|\t\\|  \\)[ \t\n]*";
  ورنہ
    context_regex.string = "\n";

  اگر (context_regex.string)
    compile_regex (&context_regex);

  /* If the user has already provided a non-empty regexp to describe
     words, compile it.  Else, unless this has already been done through
     a user provided Break character file, construct a fastmap of
     characters that may appear in a word.  If GNU extensions enabled,
     include only letters of the underlying character set.  If disabled,
     include almost everything, even punctuation; stop only on white
     space.  */

  اگر (لفظ_regex.string)
    compile_regex (&لفظ_regex);
  ورنہ اگر (!break_file)
    {
      اگر (gnu_extensions)
        {

          /* Simulate \w+.  */

          برائے (صحیح character = 0; character < CHAR_سیٹ_سائز; character++)
            لفظ_fastmap[character] = !! حرف_ہے (character);
        }
      ورنہ
        {

          /* Simulate [^ \t\n]+.  */

          memset (لفظ_fastmap, 1, CHAR_سیٹ_سائز);
          لفظ_fastmap[' '] = 0;
          لفظ_fastmap['\t'] = 0;
          لفظ_fastmap['\n'] = 0;
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

ساکن خالی
swallow_file_اندر_حافظہ (حرف ثابت *file_name, بلاک_2 *بلاک)
{
  size_t مستعمل_لمبائی;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  اگر (using_stdin)
    بلاک->آغاز = fread_file (stdin, 0, &مستعمل_لمبائی);
  ورنہ
    بلاک->آغاز = read_file (file_name, 0, &مستعمل_لمبائی);

  اگر (!بلاک->آغاز)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  اگر (using_stdin)
    clearerr (stdin);

  بلاک->انتہا = بلاک->آغاز + مستعمل_لمبائی;
}

/* Sort and search routines.  */

/*--------------------------------------------------------------------------.
| Compare two words, FIRST and SECOND, and return 0 if they are identical.  |
| Return less than 0 if the first word goes before the second; return	    |
| greater than 0 if the first word goes after the second.		    |
|									    |
| If a word is indeed a prefix of the other, the shorter should go first.   |
`--------------------------------------------------------------------------*/

ساکن صحیح
compare_الفاظ (ثابت خالی *void_پہلا, ثابت خالی *void_دوسرا)
{
  لفظ ثابت *پہلا = void_پہلا;
  لفظ ثابت *دوسرا = void_دوسرا;
  idx_t لمبائی = MIN (پہلا->سائز, دوسرا->سائز);

  اگر (ignore_case)
    {
      برائے (idx_t شمار_2 = 0; شمار_2 < لمبائی; شمار_2++)
        {
          صحیح قدر = (folded_chars[مقصد_uchar (پہلا->آغاز[شمار_2])]
                       - folded_chars[مقصد_uchar (دوسرا->آغاز[شمار_2])]);
          اگر (قدر != 0)
            واپس قدر;
        }
    }
  ورنہ
    {
      برائے (idx_t شمار_2 = 0; شمار_2 < لمبائی; شمار_2++)
        {
          صحیح قدر = (مقصد_uchar (پہلا->آغاز[شمار_2])
                       - مقصد_uchar (دوسرا->آغاز[شمار_2]));
          اگر (قدر != 0)
            واپس قدر;
        }
    }

  واپس _GL_CMP (پہلا->سائز, دوسرا->سائز);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

ساکن صحیح
compare_occurs (ثابت خالی *void_پہلا, ثابت خالی *void_دوسرا)
{
#تعریف_کرو پہلا ((ثابت OCCURS *) void_پہلا)
#تعریف_کرو دوسرا ((ثابت OCCURS *) void_دوسرا)
  صحیح قدر;

  قدر = compare_الفاظ (&پہلا->کلید, &دوسرا->کلید);
  واپس (قدر ? قدر
          : _GL_CMP (پہلا->کلید.آغاز, دوسرا->کلید.آغاز));
#تعریف_منسوخ پہلا
#تعریف_منسوخ دوسرا
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_خالص
ساکن bool
تلاش_جدول (لفظ *لفظ_2, لفظ_جدول *جدول)
{
  idx_t lo = 0;
  idx_t hi = جدول->لمبائی;
  جب (lo < hi)
    {
      idx_t درمیان = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      صحیح قدر = compare_الفاظ (لفظ_2, جدول->آغاز + درمیان);
      اگر (قدر < 0)
        hi = درمیان;
      ورنہ اگر (قدر > 0)
        lo = درمیان + 1;
      ورنہ
        واپس true;
    }
  واپس false;
}

/*---------------------------------------------------------------------.
| Sort the whole occurs table in memory.  Presumably, 'qsort' does not |
| take intermediate copies or table elements, so the sort will be      |
| stabilized throughout the comparison routine.			       |
`---------------------------------------------------------------------*/

ساکن خالی
ترتیب_دو_ملا_occurs (خالی)
{

  /* Only one language for the time being.  */
  اگر (نمبر_کا_occurs[0])
    qsort (occurs_جدول[0], نمبر_کا_occurs[0], حجم **occurs_جدول,
           compare_occurs);
}

/* Parameter files reading routines.  */

/*----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing a set of break characters.    |
| Build a content to the array word_fastmap in which all characters are |
| allowed except those found in the file.  Characters may be repeated.  |
`----------------------------------------------------------------------*/

ساکن خالی
digest_break_file (حرف ثابت *file_name)
{
  بلاک_2 file_contents;		/* to receive a copy of the file */

  swallow_file_اندر_حافظہ (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (لفظ_fastmap, 1, CHAR_سیٹ_سائز);
  برائے (حرف *cursor = file_contents.آغاز; cursor < file_contents.انتہا;
       cursor++)
    لفظ_fastmap[مقصد_uchar (*cursor)] = 0;

  اگر (!gnu_extensions)
    {

      /* If GNU extensions are enabled, the only way to avoid newline as
         a break character is to write all the break characters in the
         file with no newline at all, not even at the end of the file.
         If disabled, spaces, tabs and newlines are always considered as
         break characters even if not included in the break file.  */

      لفظ_fastmap[' '] = 0;
      لفظ_fastmap['\t'] = 0;
      لفظ_fastmap['\n'] = 0;
    }

  /* Return the space of the file, which is no more required.  */

  آزاد (file_contents.آغاز);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

ساکن خالی
digest_لفظ_file (حرف ثابت *file_name, لفظ_جدول *جدول)
{
  بلاک_2 file_contents;		/* to receive a copy of the file */
  حرف *cursor;			/* cursor in file copy */
  حرف *لفظ_آغاز;		/* start of the current word */

  swallow_file_اندر_حافظہ (file_name, &file_contents);

  جدول->آغاز = تہی;
  جدول->alloc = 0;
  جدول->لمبائی = 0;

  /* Read the whole file.  */

  cursor = file_contents.آغاز;
  جب (cursor < file_contents.انتہا)
    {

      /* Read one line, and save the word in contains.  */

      لفظ_آغاز = cursor;
      جب (cursor < file_contents.انتہا && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      اگر (cursor > لفظ_آغاز)
        {
          اگر (جدول->لمبائی == جدول->alloc)
            جدول->آغاز = xpalloc (جدول->آغاز, &جدول->alloc, 1, -1,
                                    حجم *جدول->آغاز);
          idx_t ثابت جدول_idx = جدول->لمبائی++;
          جدول->آغاز[جدول_idx].آغاز = لفظ_آغاز;
          جدول->آغاز[جدول_idx].سائز = cursor - لفظ_آغاز;
        }

      /* This test allows for an incomplete line at end of file.  */

      اگر (cursor < file_contents.انتہا)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (جدول->آغاز, جدول->لمبائی, حجم جدول->آغاز[0], compare_الفاظ);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

ساکن خالی
ڈھونڈو_occurs_اندر_متن (صحیح file_اشاریہ)
{
  حرف *جائزہ;			/* for scanning the source text */
  حرف *سطر_آغاز;		/* start of the current input line */
  حرف *سطر_جائزہ;		/* newlines scanned until this point */
  idx_t reference_لمبائی;	/* length of reference in input mode */
  لفظ possible_کلید;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  حرف *context_آغاز;		/* start of left context */
  حرف *context_انتہا;		/* end of right context */
  حرف *لفظ_آغاز;		/* start of word */
  حرف *لفظ_انتہا;		/* end of word */
  حرف *اگلا_context_آغاز;	/* next start of left context */

  ثابت بلاک_2 *متن_بفر = &متن_buffers[file_اشاریہ];

  /* reference_length is always used within 'if (input_reference)'.
     However, GNU C diagnoses that it may be used uninitialized.  The
     following assignment is merely to shut it up.  */

  reference_لمبائی = 0;

  /* Tracking where lines start is helpful for reference processing.  In
     auto reference mode, this allows counting lines.  In input reference
     mode, this permits finding the beginning of the references.

     The first line begins with the file, skip immediately this very first
     reference in input reference mode, to help further rejection any word
     found inside it.  Also, unconditionally assigning these variable has
     the happy effect of shutting up lint.  */

  سطر_آغاز = متن_بفر->آغاز;
  سطر_جائزہ = سطر_آغاز;
  اگر (داخلہ_reference)
    {
      SKIP_NON_WHITE (سطر_جائزہ, متن_بفر->انتہا);
      reference_لمبائی = سطر_جائزہ - سطر_آغاز;
      SKIP_WHITE (سطر_جائزہ, متن_بفر->انتہا);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  برائے (حرف *cursor = متن_بفر->آغاز;
       cursor < متن_بفر->انتہا;
       cursor = اگلا_context_آغاز)
    {

      /* 'context_start' gets initialized before the processing of each
         line, or once for the whole buffer if no end of line or sentence
         sequence separator.  */

      context_آغاز = cursor;

      /* If an end of line or end of sentence sequence is defined and
         non-empty, 'next_context_start' will be recomputed to be the end of
         each line or sentence, before each one is processed.  If no such
         sequence, then 'next_context_start' is set at the end of the whole
         buffer, which is then considered to be a single line or sentence.
         This test also accounts for the case of an incomplete line or
         sentence at the end of the buffer.  */

      اگلا_context_آغاز = متن_بفر->انتہا;
      اگر (context_regex.string)
        انتخاب (re_تلاش (&context_regex.نمونہ_2, cursor,
                           متن_بفر->انتہا - cursor,
                           0, متن_بفر->انتہا - cursor, &context_regs))
          {
          حالت -2:
            matcher_error ();

          حالت -1:
            توڑو;

          حالت 0:
            error (EXIT_FAILURE, 0,
                   _("error: regular expression has a match of length zero:"
                     " %s"),
                   quote (context_regex.string));

          طےشدہ:
            اگلا_context_آغاز = cursor + context_regs.انتہا[0];
            توڑو;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_انتہا = اگلا_context_آغاز;
      SKIP_WHITE_BACKWARDS (context_انتہا, context_آغاز);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      جب (true)
        {
          اگر (لفظ_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_تلاش (&لفظ_regex.نمونہ_2, cursor,
                                      context_انتہا - cursor,
                                      0, context_انتہا - cursor, &لفظ_regs);
              اگر (r == -2)
                matcher_error ();
              اگر (r == -1)
                توڑو;
              لفظ_آغاز = cursor + لفظ_regs.آغاز[0];
              لفظ_انتہا = cursor + لفظ_regs.انتہا[0];
            }
          ورنہ

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              جائزہ = cursor;
              جب (جائزہ < context_انتہا
                     && !لفظ_fastmap[مقصد_uchar (*جائزہ)])
                جائزہ++;

              اگر (جائزہ == context_انتہا)
                توڑو;

              لفظ_آغاز = جائزہ;

              جب (جائزہ < context_انتہا
                     && لفظ_fastmap[مقصد_uchar (*جائزہ)])
                جائزہ++;

              لفظ_انتہا = جائزہ;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = لفظ_آغاز;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          اگر (لفظ_انتہا == لفظ_آغاز)
            {
              cursor++;
              جاری;
            }

          /* This is a genuine, non empty word, so save it as a possible
             key.  Then skip over it.  Also, maintain the maximum length of
             all words read so far.  It is mandatory to take the maximum
             length of all words in the file, without considering if they
             are actually kept or rejected, because backward jumps at output
             generation time may fall in *any* word.  */

          possible_کلید.آغاز = cursor;
          possible_کلید.سائز = لفظ_انتہا - لفظ_آغاز;
          cursor += possible_کلید.سائز;

          اگر (possible_کلید.سائز > زیادہ_سے_زیادہ_لفظ_لمبائی)
            زیادہ_سے_زیادہ_لفظ_لمبائی = possible_کلید.سائز;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          اگر (داخلہ_reference)
            {
              جب (سطر_جائزہ < possible_کلید.آغاز)
                اگر (*سطر_جائزہ == '\n')
                  {
                    کل_سطر_گنتی++;
                    سطر_جائزہ++;
                    سطر_آغاز = سطر_جائزہ;
                    SKIP_NON_WHITE (سطر_جائزہ, متن_بفر->انتہا);
                    reference_لمبائی = سطر_جائزہ - سطر_آغاز;
                  }
                ورنہ
                  سطر_جائزہ++;
              اگر (سطر_جائزہ > possible_کلید.آغاز)
                جاری;
            }

          /* Ignore the word if an 'Ignore words' table exists and if it is
             part of it.  Also ignore the word if an 'Only words' table and
             if it is *not* part of it.

             It is allowed that both tables be used at once, even if this
             may look strange for now.  Just ignore a word that would appear
             in both.  If regexps are eventually implemented for these
             tables, the Ignore table could then reject words that would
             have been previously accepted by the Only table.  */

          اگر (ignore_file && تلاش_جدول (&possible_کلید, &ignore_جدول))
            جاری;
          اگر (only_file && !تلاش_جدول (&possible_کلید, &only_جدول))
            جاری;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          اگر (نمبر_کا_occurs[0] == occurs_alloc[0])
            occurs_جدول[0] = xpalloc (occurs_جدول[0], &occurs_alloc[0],
                                       1, -1, حجم *occurs_جدول[0]);
          occurs_cursor = occurs_جدول[0] + نمبر_کا_occurs[0];

          /* Define the reference field, if any.  */

          اگر (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              جب (سطر_جائزہ < possible_کلید.آغاز)
                اگر (*سطر_جائزہ == '\n')
                  {
                    کل_سطر_گنتی++;
                    سطر_جائزہ++;
                    سطر_آغاز = سطر_جائزہ;
                    SKIP_NON_WHITE (سطر_جائزہ, متن_بفر->انتہا);
                  }
                ورنہ
                  سطر_جائزہ++;

              occurs_cursor->reference = کل_سطر_گنتی;
            }
          ورنہ اگر (داخلہ_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = سطر_آغاز - possible_کلید.آغاز;
              اگر (reference_لمبائی > reference_max_چوڑائی)
                reference_max_چوڑائی = reference_لمبائی;
            }

          /* Exclude the reference from the context in simple cases.  */

          اگر (داخلہ_reference && سطر_آغاز == context_آغاز)
            {
              SKIP_NON_WHITE (context_آغاز, context_انتہا);
              SKIP_WHITE (context_آغاز, context_انتہا);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->کلید = possible_کلید;
          occurs_cursor->بایاں = context_آغاز - possible_کلید.آغاز;
          occurs_cursor->دایاں = context_انتہا - possible_کلید.آغاز;
          occurs_cursor->file_اشاریہ = file_اشاریہ;

          نمبر_کا_occurs[0]++;
        }
    }
}

/* Formatting and actual output - service routines.  */

/*-----------------------------------------.
| Prints some NUMBER of spaces on stdout.  |
`-----------------------------------------*/

ساکن خالی
print_spaces (ptrdiff_t نمبر)
{
  برائے (ptrdiff_t شمار_2 = نمبر; شمار_2 > 0; شمار_2--)
    حرف_لکھو (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

ساکن خالی
print_میدان (بلاک_2 میدان)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  برائے (حرف *cursor = میدان.آغاز; cursor < میدان.انتہا; cursor++)
    {
      غیرعلامتی حرف character = *cursor;
      اگر (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          انتخاب (character)
            {
            حالت '"':
              /* In roff output format, double any quote.  */
              حرف_لکھو ('"');
              حرف_لکھو ('"');
              توڑو;

            حالت '$':
            حالت '%':
            حالت '&':
            حالت '#':
            حالت '_':
              /* In TeX output format, precede these with a backslash.  */
              حرف_لکھو ('\\');
              حرف_لکھو (character);
              توڑو;

            حالت '{':
            حالت '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              لکھو ("$\\%c$", character);
              توڑو;

            حالت '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              توڑو;

            طےشدہ:
              /* Any other flagged character produces a single space.  */
              حرف_لکھو (' ');
            }
        }
      ورنہ
        حرف_لکھو (*cursor);
    }
}

/* Formatting and actual output - planning routines.  */

/*--------------------------------------------------------------------.
| From information collected from command line options and input file |
| readings, compute and fix some output parameter values.	      |
`--------------------------------------------------------------------*/

ساکن خالی
fix_output_parameters (خالی)
{
  /* In auto reference mode, the maximum width of this field is
     precomputed and subtracted from the overall line width.  Add one for
     the column which separate the file name from the line number.  */

  اگر (auto_reference)
    {
      reference_max_چوڑائی = 0;
      برائے (صحیح file_اشاریہ = 0; file_اشاریہ < نمبر_داخلہ_files; file_اشاریہ++)
        {
          intmax_t سطر_ordinal = file_سطر_گنتی[file_اشاریہ] + 1;
          اگر (file_اشاریہ > 0)
            سطر_ordinal -= file_سطر_گنتی[file_اشاریہ - 1];
          حرف ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_چوڑائی = sprintf (ordinal_string, "%jd", سطر_ordinal);
          اگر (داخلہ_file_name[file_اشاریہ])
            reference_چوڑائی += لڑی_لمبائی (داخلہ_file_name[file_اشاریہ]);
          اگر (reference_چوڑائی > reference_max_چوڑائی)
            reference_max_چوڑائی = reference_چوڑائی;
        }
      reference_max_چوڑائی++;
      reference.آغاز = xmalloc (reference_max_چوڑائی + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  اگر ((auto_reference || داخلہ_reference) && !دایاں_reference)
    سطر_چوڑائی = MAX (0, سطر_چوڑائی - (reference_max_چوڑائی + gap_سائز));

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

  half_سطر_چوڑائی = سطر_چوڑائی >> 1;
  before_max_چوڑائی = half_سطر_چوڑائی - gap_سائز;
  keyafter_max_چوڑائی = half_سطر_چوڑائی;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  اگر (truncation_string && *truncation_string)
    truncation_string_لمبائی = لڑی_لمبائی (truncation_string);
  ورنہ
    truncation_string = تہی;

  اگر (gnu_extensions)
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

      before_max_چوڑائی -= 2 * truncation_string_لمبائی;
      keyafter_max_چوڑائی -= 2 * truncation_string_لمبائی;
    }
  ورنہ
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_چوڑائی -= 2 * truncation_string_لمبائی + 1;
    }

  اگر (before_max_چوڑائی < 0)
    before_max_چوڑائی = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  برائے (صحیح character = 0; character < CHAR_سیٹ_سائز; character++)
    edited_flag[character] = !! خلا_ہے (character);
  edited_flag['\f'] = 1;

  /* Complete the special character flagging according to selected output
     format.  */

  انتخاب (output_format)
    {
    حالت UNKNOWN_FORMAT:
      /* Should never happen.  */

    حالت DUMB_FORMAT:
      توڑو;

    حالت ROFF_FORMAT:

      /* 'Quote' characters should be doubled.  */

      edited_flag['"'] = 1;
      توڑو;

    حالت TEX_FORMAT:

      /* Various characters need special processing.  */

      برائے (حرف ثابت *cursor = "$%&#_{}\\"; *cursor; cursor++)
        edited_flag[مقصد_uchar (*cursor)] = 1;

      توڑو;
    }
}

/*------------------------------------------------------------------.
| Compute the position and length of all the output fields, given a |
| pointer to some OCCURS.					    |
`------------------------------------------------------------------*/

ساکن خالی
define_all_fields (OCCURS *occurs)
{
  ptrdiff_t دم_max_چوڑائی;	/* allowable width of tail field */
  ptrdiff_t سر_max_چوڑائی;	/* allowable width of head field */
  حرف *cursor;			/* running cursor in source text */
  حرف *بایاں_context_آغاز;	/* start of left context */
  حرف *دایاں_context_انتہا;	/* end of right context */
  حرف *بایاں_میدان_آغاز;	/* conservative start for 'head'/'before' */
  حرف ثابت *file_name;	/* file name for reference */
  intmax_t سطر_ordinal;	/* line ordinal for reference */
  حرف ثابت *بفر_آغاز;	/* start of buffered file for this occurs */
  حرف ثابت *بفر_انتہا;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.آغاز = occurs->کلید.آغاز;
  keyafter.انتہا = keyafter.آغاز + occurs->کلید.سائز;
  بایاں_context_آغاز = keyafter.آغاز + occurs->بایاں;
  دایاں_context_انتہا = keyafter.آغاز + occurs->دایاں;

  بفر_آغاز = متن_buffers[occurs->file_اشاریہ].آغاز;
  بفر_انتہا = متن_buffers[occurs->file_اشاریہ].انتہا;

  cursor = keyafter.انتہا;
  جب (cursor < دایاں_context_انتہا
         && cursor <= keyafter.آغاز + keyafter_max_چوڑائی)
    {
      keyafter.انتہا = cursor;
      SKIP_SOMETHING (cursor, دایاں_context_انتہا);
    }
  اگر (cursor <= keyafter.آغاز + keyafter_max_چوڑائی)
    keyafter.انتہا = cursor;

  keyafter_truncation = truncation_string && keyafter.انتہا < دایاں_context_انتہا;

  SKIP_WHITE_BACKWARDS (keyafter.انتہا, keyafter.آغاز);

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

  اگر (-occurs->بایاں > half_سطر_چوڑائی + زیادہ_سے_زیادہ_لفظ_لمبائی)
    {
      بایاں_میدان_آغاز
        = keyafter.آغاز - (half_سطر_چوڑائی + زیادہ_سے_زیادہ_لفظ_لمبائی);
      SKIP_SOMETHING (بایاں_میدان_آغاز, keyafter.آغاز);
    }
  ورنہ
    بایاں_میدان_آغاز = keyafter.آغاز + occurs->بایاں;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.آغاز = بایاں_میدان_آغاز;
  before.انتہا = keyafter.آغاز;
  SKIP_WHITE_BACKWARDS (before.انتہا, before.آغاز);

  جب (before.آغاز + before_max_چوڑائی < before.انتہا)
    SKIP_SOMETHING (before.آغاز, before.انتہا);

  اگر (truncation_string)
    {
      cursor = before.آغاز;
      SKIP_WHITE_BACKWARDS (cursor, بفر_آغاز);
      before_truncation = cursor > بایاں_context_آغاز;
    }
  ورنہ
    before_truncation = false;

  SKIP_WHITE (before.آغاز, بفر_انتہا);

  /* The tail could not take more columns than what has been left in the
     left context field, and a gap is mandatory.  It starts after the
     right context, and does not contain prefixed spaces.  It ends at
     the end of line, the end of buffer or when the tail field is full,
     whichever comes first.  It cannot contain only part of a word, and
     has no suffixed spaces.  */

  دم_max_چوڑائی
    = before_max_چوڑائی - (before.انتہا - before.آغاز) - gap_سائز;

  اگر (دم_max_چوڑائی > 0)
    {
      دم.آغاز = keyafter.انتہا;
      SKIP_WHITE (دم.آغاز, بفر_انتہا);

      دم.انتہا = دم.آغاز;
      cursor = دم.انتہا;
      جب (cursor < دایاں_context_انتہا
             && cursor < دم.آغاز + دم_max_چوڑائی)
        {
          دم.انتہا = cursor;
          SKIP_SOMETHING (cursor, دایاں_context_انتہا);
        }

      اگر (cursor < دم.آغاز + دم_max_چوڑائی)
        دم.انتہا = cursor;

      اگر (دم.انتہا > دم.آغاز)
        {
          keyafter_truncation = false;
          دم_truncation = truncation_string && دم.انتہا < دایاں_context_انتہا;
        }
      ورنہ
        دم_truncation = false;

      SKIP_WHITE_BACKWARDS (دم.انتہا, دم.آغاز);
    }
  ورنہ
    {

      /* No place left for a tail field.  */

      دم.آغاز = تہی;
      دم.انتہا = تہی;
      دم_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  سر_max_چوڑائی
    = keyafter_max_چوڑائی - (keyafter.انتہا - keyafter.آغاز) - gap_سائز;

  اگر (سر_max_چوڑائی > 0)
    {
      سر.انتہا = before.آغاز;
      SKIP_WHITE_BACKWARDS (سر.انتہا, بفر_آغاز);

      سر.آغاز = بایاں_میدان_آغاز;
      جب (سر.آغاز + سر_max_چوڑائی < سر.انتہا)
        SKIP_SOMETHING (سر.آغاز, سر.انتہا);

      اگر (سر.انتہا > سر.آغاز)
        {
          before_truncation = false;
          سر_truncation = (truncation_string
                             && سر.آغاز > بایاں_context_آغاز);
        }
      ورنہ
        سر_truncation = false;

      SKIP_WHITE (سر.آغاز, سر.انتہا);
    }
  ورنہ
    {

      /* No place left for a head field.  */

      سر.آغاز = تہی;
      سر.انتہا = تہی;
      سر_truncation = false;
    }

  اگر (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = داخلہ_file_name[occurs->file_اشاریہ];
      اگر (!file_name)
        file_name = "";

      سطر_ordinal = occurs->reference + 1;
      اگر (occurs->file_اشاریہ > 0)
        سطر_ordinal -= file_سطر_گنتی[occurs->file_اشاریہ - 1];

      حرف *file_انتہا = stpcpy (reference.آغاز, file_name);
      reference.انتہا = file_انتہا + sprintf (file_انتہا, ":%jd", سطر_ordinal);
    }
  ورنہ اگر (داخلہ_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.آغاز = keyafter.آغاز + occurs->reference;
      reference.انتہا = reference.آغاز;
      SKIP_NON_WHITE (reference.انتہا, دایاں_context_انتہا);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

ساکن خالی
output_one_roff_سطر (خالی)
{
  /* Output the 'tail' field.  */

  لکھو (".%s \"", macro_name);
  print_میدان (دم);
  اگر (دم_truncation)
    fputs (truncation_string, stdout);
  حرف_لکھو ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  اگر (before_truncation)
    fputs (truncation_string, stdout);
  print_میدان (before);
  حرف_لکھو ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_میدان (keyafter);
  اگر (keyafter_truncation)
    fputs (truncation_string, stdout);
  حرف_لکھو ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  اگر (سر_truncation)
    fputs (truncation_string, stdout);
  print_میدان (سر);
  حرف_لکھو ('"');

  /* Conditionally output the 'reference' field.  */

  اگر (auto_reference || داخلہ_reference)
    {
      fputs (" \"", stdout);
      print_میدان (reference);
      حرف_لکھو ('"');
    }

  حرف_لکھو ('\n');
}

/*---------------------------------------------------------.
| Output the current output fields as one line for 'TeX'.  |
`---------------------------------------------------------*/

ساکن خالی
output_one_tex_سطر (خالی)
{
  بلاک_2 کلید;			/* key field, isolated */
  بلاک_2 after;			/* after field, isolated */
  حرف *cursor;			/* running cursor in source text */

  لکھو ("\\%s ", macro_name);
  حرف_لکھو ('{');
  print_میدان (دم);
  fputs ("}{", stdout);
  print_میدان (before);
  fputs ("}{", stdout);
  کلید.آغاز = keyafter.آغاز;
  after.انتہا = keyafter.انتہا;
  cursor = keyafter.آغاز;
  SKIP_SOMETHING (cursor, keyafter.انتہا);
  کلید.انتہا = cursor;
  after.آغاز = cursor;
  print_میدان (کلید);
  fputs ("}{", stdout);
  print_میدان (after);
  fputs ("}{", stdout);
  print_میدان (سر);
  حرف_لکھو ('}');
  اگر (auto_reference || داخلہ_reference)
    {
      حرف_لکھو ('{');
      print_میدان (reference);
      حرف_لکھو ('}');
    }
  حرف_لکھو ('\n');
}

/*-------------------------------------------------------------------.
| Output the current output fields as one line for a dumb terminal.  |
`-------------------------------------------------------------------*/

ساکن خالی
output_one_dumb_سطر (خالی)
{
  اگر (!دایاں_reference)
    {
      اگر (auto_reference)
        {

          /* Output the 'reference' field, in such a way that GNU emacs
             next-error will handle it.  The ending colon is taken from the
             gap which follows.  */

          print_میدان (reference);
          حرف_لکھو (':');
          print_spaces (reference_max_چوڑائی
                        + gap_سائز
                        - (reference.انتہا - reference.آغاز)
                        - 1);
        }
      ورنہ
        {

          /* Output the 'reference' field and its following gap.  */

          print_میدان (reference);
          print_spaces (reference_max_چوڑائی
                        + gap_سائز
                        - (reference.انتہا - reference.آغاز));
        }
    }

  اگر (دم.آغاز < دم.انتہا)
    {
      /* Output the 'tail' field.  */

      print_میدان (دم);
      اگر (دم_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_سطر_چوڑائی - gap_سائز
                    - (before.انتہا - before.آغاز)
                    - (before_truncation ? truncation_string_لمبائی : 0)
                    - (دم.انتہا - دم.آغاز)
                    - (دم_truncation ? truncation_string_لمبائی : 0));
    }
  ورنہ
    print_spaces (half_سطر_چوڑائی - gap_سائز
                  - (before.انتہا - before.آغاز)
                  - (before_truncation ? truncation_string_لمبائی : 0));

  /* Output the 'before' field.  */

  اگر (before_truncation)
    fputs (truncation_string, stdout);
  print_میدان (before);

  print_spaces (gap_سائز);

  /* Output the 'keyafter' field.  */

  print_میدان (keyafter);
  اگر (keyafter_truncation)
    fputs (truncation_string, stdout);

  اگر (سر.آغاز < سر.انتہا)
    {
      /* Output the 'head' field.  */

      print_spaces (half_سطر_چوڑائی
                    - (keyafter.انتہا - keyafter.آغاز)
                    - (keyafter_truncation ? truncation_string_لمبائی : 0)
                    - (سر.انتہا - سر.آغاز)
                    - (سر_truncation ? truncation_string_لمبائی : 0));
      اگر (سر_truncation)
        fputs (truncation_string, stdout);
      print_میدان (سر);
    }
  ورنہ

    اگر ((auto_reference || داخلہ_reference) && دایاں_reference)
      print_spaces (half_سطر_چوڑائی
                    - (keyafter.انتہا - keyafter.آغاز)
                    - (keyafter_truncation ? truncation_string_لمبائی : 0));

  اگر ((auto_reference || داخلہ_reference) && دایاں_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_سائز);
      print_میدان (reference);
    }

  حرف_لکھو ('\n');
}

/*------------------------------------------------------------------------.
| Scan the whole occurs table and, for each entry, output one line in the |
| appropriate format.							  |
`------------------------------------------------------------------------*/

ساکن خالی
generate_all_output (خالی)
{
  OCCURS *occurs_cursor;	/* current keyword entry being processed */

  /* The following assignments are useful to provide default values in case
     line contexts or references are not used, in which case these variables
     would never be computed.  */

  دم.آغاز = تہی;
  دم.انتہا = تہی;
  دم_truncation = false;

  سر.آغاز = تہی;
  سر.انتہا = تہی;
  سر_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_جدول[0];

  برائے (idx_t occurs_اشاریہ = 0; occurs_اشاریہ < نمبر_کا_occurs[0];
       occurs_اشاریہ++)
    {
      /* Compute the exact size of every field and whenever truncation flags
         are present or not.  */

      define_all_fields (occurs_cursor);

      /* Produce one output line according to selected format.  */

      انتخاب (output_format)
        {
        حالت UNKNOWN_FORMAT:
          /* Should never happen.  */

        حالت DUMB_FORMAT:
          output_one_dumb_سطر ();
          توڑو;

        حالت ROFF_FORMAT:
          output_one_roff_سطر ();
          توڑو;

        حالت TEX_FORMAT:
          output_one_tex_سطر ();
          توڑو;
        }

      /* Advance the cursor into the occurs table.  */

      occurs_cursor++;
    }
}

/* Option decoding and main program.  */

/*------------------------------------------------------.
| Print program identification and options, then exit.  |
`------------------------------------------------------*/

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [داخلہ_2]...   (without -G)\n\
  or:  %s -G [OPTION]... [داخلہ_2 [OUTPUT]]\n"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Output a permuted index, including context, کا the الفاظ اندر the داخلہ files.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --خودکار-reference           output automatically generated references\n\
"));
      oputs (_("\
  -G, --traditional              behave مزید like System V 'ptx'\n\
"));
      oputs (_("\
  -F, --flag-truncation=STRING   use STRING برائے flagging سطر truncations.\n\
                                 The طےشدہ is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name مقصد use instead کا 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --دایاں-side-refs          put references at دایاں, not counted اندر -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   برائے انتہا کا سطریں or انتہا کا sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --لفظ_2-regexp=REGEXP       use REGEXP مقصد مطابقت each keyword\n\
"));
      oputs (_("\
  -b, --توڑو-file=FILE          لفظ_2 توڑو characters اندر this FILE\n\
"));
      oputs (_("\
  -f, --ignore-حالت              fold زیریں حالت مقصد بالائی حالت برائے ترتیب_کاری\n\
"));
      oputs (_("\
  -g, --gap-سائز=نمبر_2          gap سائز اندر columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         اقرأ ignore لفظ_2 list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           اقرأ only لفظ_2 list from this FILE\n\
"));
      oputs (_("\
  -r, --references               پہلا میدان کا each سطر is a reference\n\
"));
      oputs (_("\
  -t, --typeset-منوال_2             change the طےشدہ چوڑائی from 72 مقصد 100\n\
"));
      oputs (_("\
  -w, --چوڑائی=نمبر_2             output چوڑائی اندر columns, reference excluded\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/*----------------------------------------------------------------------.
| Main program.  Decode ARGC arguments passed through the ARGV array of |
| strings, then launch execution.				        |
`----------------------------------------------------------------------*/

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  FORMAT_OPTION = CHAR_MAX + 1
};

/* Long options equivalences.  */
ساکن ساخت option ثابت long_options[] =
{
  {"auto-reference", no_دلیل, تہی, 'A'},
  {"break-file", required_دلیل, تہی, 'b'},
  {"flag-truncation", required_دلیل, تہی, 'F'},
  {"ignore-case", no_دلیل, تہی, 'f'},
  {"gap-size", required_دلیل, تہی, 'g'},
  {"ignore-file", required_دلیل, تہی, 'i'},
  {"macro-name", required_دلیل, تہی, 'M'},
  {"only-file", required_دلیل, تہی, 'o'},
  {"references", no_دلیل, تہی, 'r'},
  {"right-side-refs", no_دلیل, تہی, 'R'},
  {"format", required_دلیل, تہی, FORMAT_OPTION},
  {"sentence-regexp", required_دلیل, تہی, 'S'},
  {"traditional", no_دلیل, تہی, 'G'},
  {"typeset-mode", no_دلیل, تہی, 't'},
  {"width", required_دلیل, تہی, 'w'},
  {"word-regexp", required_دلیل, تہی, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0},
};

ساکن حرف ثابت *ثابت format_دلائل[] =
{
  "roff", "tex", تہی
};

ساکن شمار Format ثابت format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optchar;			/* argument character */
  صحیح file_اشاریہ;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب (optchar = getopt_long (argc, argv, "AF:GM:ORS:TW:b:i:fg:o:trw:",
                                long_options, تہی),
         optchar != EOF)
    {
      انتخاب (optchar)
        {
        طےشدہ:
          usage (EXIT_FAILURE);

        حالت 'G':
          gnu_extensions = false;
          توڑو;

        حالت 'b':
          break_file = optarg;
          توڑو;

        حالت 'f':
          ignore_case = true;
          توڑو;

        حالت 'g':
          {
            intmax_t tmp;
            اگر (! (xstrtoimax (optarg, تہی, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid gap width: %s"),
                     quote (optarg));
            gap_سائز = tmp;
            توڑو;
          }

        حالت 'i':
          ignore_file = optarg;
          توڑو;

        حالت 'o':
          only_file = optarg;
          توڑو;

        حالت 'r':
          داخلہ_reference = true;
          توڑو;

        حالت 't':
          اگر (سطر_چوڑائی < 0)
            سطر_چوڑائی = 100;
          توڑو;

        حالت 'w':
          {
            intmax_t tmp;
            اگر (! (xstrtoimax (optarg, تہی, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            سطر_چوڑائی = tmp;
            توڑو;
          }

        حالت 'A':
          auto_reference = true;
          توڑو;

        حالت 'F':
          truncation_string = optarg;
          unescape_string (optarg);
          توڑو;

        حالت 'M':
          macro_name = optarg;
          توڑو;

        حالت 'O':
          output_format = ROFF_FORMAT;
          توڑو;

        حالت 'R':
          دایاں_reference = true;
          توڑو;

        حالت 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          توڑو;

        حالت 'T':
          output_format = TEX_FORMAT;
          توڑو;

        حالت 'W':
          لفظ_regex.string = optarg;
          unescape_string (optarg);
          اگر (!*لفظ_regex.string)
            لفظ_regex.string = تہی;
          توڑو;

        حالت FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_دلائل, format_vals);
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        }
    }

  اگر (سطر_چوڑائی < 0)
    سطر_چوڑائی = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  اگر (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      داخلہ_file_name = xmalloc (حجم *داخلہ_file_name);
      file_سطر_گنتی = xmalloc (حجم *file_سطر_گنتی);
      متن_buffers =    xmalloc (حجم *متن_buffers);
      نمبر_داخلہ_files = 1;
      داخلہ_file_name[0] = تہی;
    }
  ورنہ اگر (gnu_extensions)
    {
      نمبر_داخلہ_files = argc - optind;
      داخلہ_file_name = xnmalloc (نمبر_داخلہ_files, حجم *داخلہ_file_name);
      file_سطر_گنتی = xnmalloc (نمبر_داخلہ_files, حجم *file_سطر_گنتی);
      متن_buffers    = xnmalloc (نمبر_داخلہ_files, حجم *متن_buffers);

      برائے (file_اشاریہ = 0; file_اشاریہ < نمبر_داخلہ_files; file_اشاریہ++)
        {
          اگر (!*argv[optind] || streq (argv[optind], "-"))
            داخلہ_file_name[file_اشاریہ] = تہی;
          ورنہ
            داخلہ_file_name[file_اشاریہ] = argv[optind];
          optind++;
        }
    }
  ورنہ
    {

      /* There is one necessary input file.  */

      نمبر_داخلہ_files = 1;
      داخلہ_file_name = xmalloc (حجم *داخلہ_file_name);
      file_سطر_گنتی = xmalloc (حجم *file_سطر_گنتی);
      متن_buffers    = xmalloc (حجم *متن_buffers);
      اگر (!*argv[optind] || streq (argv[optind], "-"))
        داخلہ_file_name[0] = تہی;
      ورنہ
        داخلہ_file_name[0] = argv[optind];
      optind++;

      /* Redirect standard output, only if requested.  */

      اگر (optind < argc)
        {
          اگر (! freopen (argv[optind], "w", stdout))
            error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
          optind++;
        }

      /* Diagnose any other argument as an error.  */

      اگر (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  /* If the output format has not been explicitly selected, choose dumb
     terminal format if GNU extensions are enabled, else 'roff' format.  */

  اگر (output_format == UNKNOWN_FORMAT)
    output_format = gnu_extensions ? DUMB_FORMAT : ROFF_FORMAT;

  /* Initialize the main tables.  */

  initialize_regex ();

  /* Read 'Break character' file, if any.  */

  اگر (break_file)
    digest_break_file (break_file);

  /* Read 'Ignore words' file and 'Only words' files, if any.  If any of
     these files is empty, reset the name of the file to null, to avoid
     unnecessary calls to search_table. */

  اگر (ignore_file)
    {
      digest_لفظ_file (ignore_file, &ignore_جدول);
      اگر (ignore_جدول.لمبائی == 0)
        ignore_file = تہی;
    }

  اگر (only_file)
    {
      digest_لفظ_file (only_file, &only_جدول);
      اگر (only_جدول.لمبائی == 0)
        only_file = تہی;
    }

  /* Prepare to study all the input files.  */

  نمبر_کا_occurs[0] = 0;
  کل_سطر_گنتی = 0;
  زیادہ_سے_زیادہ_لفظ_لمبائی = 0;
  reference_max_چوڑائی = 0;

  برائے (file_اشاریہ = 0; file_اشاریہ < نمبر_داخلہ_files; file_اشاریہ++)
    {
      بلاک_2 *متن_بفر = متن_buffers + file_اشاریہ;

      /* Read the file contents into memory, then study it.  */

      swallow_file_اندر_حافظہ (داخلہ_file_name[file_اشاریہ], متن_بفر);
      ڈھونڈو_occurs_اندر_متن (file_اشاریہ);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      کل_سطر_گنتی++;
      file_سطر_گنتی[file_اشاریہ] = کل_سطر_گنتی;
    }

  /* Do the output process phase.  */

  ترتیب_دو_ملا_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  واپس EXIT_SUCCESS;
}
