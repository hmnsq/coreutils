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
#وصف_ڪريو پروگرام_NAME "ptx"

/* TRANSLATORS: Please translate "F. Pinard" to "François Pinard"
   if "ç" (c-with-cedilla) is available in the translation's character
   set and encoding.  */
#وصف_ڪريو AUTHORS proper_name_lite ("F. Pinard", "Fran\xc3\xa7ois Pinard")

/* Number of possible characters in a byte.  */
#وصف_ڪريو CHAR_سيٽ_سائيز 256

/* Debugging the memory allocator.  */

#إذا WITH_DMALLOC
# define MALLOC_FUNC_CHECK 1
# include <dmalloc.h>
#جيڪڏهن_پورو

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
ساكن bool داخلا_reference = false;	/* refs at beginning of input lines */
ساكن bool ساڄو_reference = false;	/* output refs after right context  */
ساكن idx_t سٽ_ويڪر = -1;		/* output line width in characters */
ساكن idx_t gap_سائيز = 3;	/* number of spaces between output fields */
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
  بنية re_نمونو_بفر نمونو_2;
  حرف fastmap[UCHAR_MAX + 1];
};

ساكن بنية regex_معلومات context_regex;	/* end of context */
ساكن بنية regex_معلومات لفظ_regex;	/* keyword */

/* A BLOCK delimit a region in memory of arbitrary size, like the copy of a
   whole file.  A WORD is similar, except it is intended for smaller regions.
   A WORD_TABLE may contain several WORDs.  */

تعريفنوع بنية
  {
    حرف *شروعات;		/* pointer to beginning of region */
    حرف *پڇاڙي;			/* pointer to end + 1 of region */
  }
بلاڪ_2;

تعريفنوع بنية
  {
    حرف *شروعات;		/* pointer to beginning of region */
    idx_t سائيز;			/* length of the region */
  }
لفظ;

تعريفنوع بنية
  {
    idx_t alloc;		/* allocated length */
    idx_t ڊيگهائي;		/* number of used entries */
    لفظ *شروعات COUNTED_BY (ڊيگهائي);	/* array of WORDs */
  }
لفظ_جدول;

/* Pattern description tables.  */

/* For each character, provide its folded equivalent.  */
ساكن بنانشان حرف folded_chars[CHAR_سيٽ_سائيز];

/* End of context pattern register indices.  */
ساكن بنية re_registers context_regs;

/* Keyword pattern register indices.  */
ساكن بنية re_registers لفظ_regs;

/* A word characters fastmap is used only when no word regexp has been
   provided.  A word is then made up of a sequence of one or more characters
   allowed by the fastmap.  Contains !0 if character allowed in word.  Not
   only this is faster in most cases, but it simplifies the implementation
   of the Break files.  */
ساكن حرف لفظ_fastmap[CHAR_سيٽ_سائيز];

/* Maximum length of any word read.  */
ساكن idx_t وڌ_کان_وڌ_لفظ_ڊيگهائي;

/* Maximum width of any reference used.  */
ساكن idx_t reference_max_ويڪر;

/* Ignore and Only word tables.  */

ساكن لفظ_جدول ignore_جدول;	/* table of words to ignore */
ساكن لفظ_جدول only_جدول;		/* table of words to select */

/* Source text table, and scanning macros.  */

ساكن صحيح عدد_داخلا_files;	/* number of text input files */
ساكن intmax_t ڪل_سٽ_ڳڻپ;	/* total number of lines seen so far */
ساكن حرف ثابت **داخلا_file_name;	/* array of text input file names */
ساكن intmax_t *file_سٽ_ڳڻپ;	/* array of line count values at end */

ساكن بلاڪ_2 *متن_buffers;	/* files to study */

/* SKIP_NON_WHITE used only for getting or skipping the reference.  */

#وصف_ڪريو SKIP_NON_WHITE(cursor, حد_بندي) \
  طالما (cursor < حد_بندي && ! خال_آهي (منزل_uchar (*cursor)))		\
    cursor++

#وصف_ڪريو SKIP_WHITE(cursor, حد_بندي) \
  طالما (cursor < حد_بندي && خال_آهي (منزل_uchar (*cursor)))		\
    cursor++

#وصف_ڪريو SKIP_WHITE_BACKWARDS(cursor, شروعات) \
  طالما (cursor > شروعات && خال_آهي (منزل_uchar (cursor[-1])))		\
    cursor--

/* Always advance by at least one byte, lest a nullable word regexp
   such as 'a*' match the empty string and loop forever.  */

#وصف_ڪريو SKIP_SOMETHING(cursor, حد_بندي) \
  إذا (لفظ_regex.string)						\
    {									\
      regoff_t ڳڻپ;							\
      ڳڻپ = re_ميلاپ (&لفظ_regex.نمونو_2, cursor, حد_بندي - cursor,	\
                        0, عدم);					\
      إذا (ڳڻپ == -2)							\
        matcher_error ();						\
      cursor += ڳڻپ <= 0 ? 1 : ڳڻپ;					\
    }									\
  وإلا إذا (لفظ_fastmap[منزل_uchar (*cursor)])				\
    طالما (cursor < حد_بندي && لفظ_fastmap[منزل_uchar (*cursor)])		\
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
    لفظ ڪنجي;			/* description of the keyword */
    ptrdiff_t کاٻو;		/* distance to left context start */
    ptrdiff_t ساڄو;		/* distance to right context end */
    intmax_t reference;		/* reference descriptor */
    صحيح file_اشاريو;		/* corresponding file  */
  }
OCCURS;

/* The various OCCURS tables are indexed by the language.  But the time
   being, there is no such multiple language support.  */

ساكن OCCURS *occurs_جدول[1];	/* all words retained from the read text */
ساكن idx_t occurs_alloc[1];	/* allocated size of occurs_table */
ساكن idx_t عدد_جو_occurs[1]; /* number of used slots in occurs_table */


/* Communication among output routines.  */

/* Indicate if special output processing is requested for each character.  */
ساكن حرف edited_flag[CHAR_سيٽ_سائيز];

/* Half of line width, reference excluded.  */
ساكن idx_t half_سٽ_ويڪر;

/* Maximum width of before field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ساكن ptrdiff_t before_max_ويڪر;

/* Maximum width of keyword-and-after field.
   FIXME: Is this nonnegative?  That is, should this be idx_t?  */
ساكن ptrdiff_t keyafter_max_ويڪر;

/* Length of string that flags truncation.  */
ساكن idx_t truncation_string_ڊيگهائي;

/* When context is limited by lines, wraparound may happen on final output:
   the 'head' pointer gives access to some supplementary left context which
   will be seen at the end of the output line, the 'tail' pointer gives
   access to some supplementary right context which will be seen at the
   beginning of the output line. */

ساكن بلاڪ_2 پڇ;		/* tail field */
ساكن bool پڇ_truncation;	/* flag truncation after the tail field */

ساكن بلاڪ_2 before;		/* before field */
ساكن bool before_truncation;	/* flag truncation before the before field */

ساكن بلاڪ_2 keyafter;		/* keyword-and-after field */
ساكن bool keyafter_truncation; /* flag truncation after the keyafter field */

ساكن بلاڪ_2 سر;		/* head field */
ساكن bool سر_truncation;	/* flag truncation before the head field */

ساكن بلاڪ_2 reference;		/* reference field for input reference mode */

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
  صحيح قدر;			/* value of \nnn escape */
  صحيح ڊيگهائي;			/* length of \nnn escape */

  cursor = string;

  طالما (*string)
    {
      إذا (*string == '\\')
        {
          string++;
          بدل (*string)
            {
            حالة 'x':		/* \xhhh escape, 3 chars maximum */
              قدر = 0;
              لكل (ڊيگهائي = 0, string++;
                   ڊيگهائي < 3 && c_isxdigit (*string);
                   ڊيگهائي++, string++)
                قدر = قدر * 16 + fromhex (*string);
              إذا (ڊيگهائي == 0)
                {
                  *cursor++ = '\\';
                  *cursor++ = 'x';
                }
              وإلا
                *cursor++ = قدر;
              اكسر;

            حالة '0':		/* \0ooo escape, 3 chars maximum */
              قدر = 0;
              لكل (ڊيگهائي = 0, string++;
                   ڊيگهائي < 3 && isoct (*string);
                   ڊيگهائي++, string++)
                قدر = قدر * 8 + fromoct (*string);
              *cursor++ = قدر;
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
  بنية re_نمونو_بفر *نمونو_2 = &regex->نمونو_2;
  حرف ثابت *string = regex->string;
  حرف ثابت *پيغام_2;

  نمونو_2->بفر = عدم;
  نمونو_2->allocated = 0;
  نمونو_2->fastmap = regex->fastmap;
  نمونو_2->translate = ignore_case ? folded_chars : عدم;

  پيغام_2 = re_compile_نمونو (string, لڙي_ڊيگهه (string), نمونو_2);
  إذا (پيغام_2)
    error (EXIT_FAILURE, 0, _("%s (for regexp %s)"), پيغام_2, quote (string));

  /* The fastmap should be compiled before 're_match'.  The following
     call is not mandatory, because 're_search' is always called sooner,
     and it compiles the fastmap if this has not been done yet.  */

  re_compile_fastmap (نمونو_2);
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
    لكل (صحيح character = 0; character < CHAR_سيٽ_سائيز; character++)
      folded_chars[character] = وڏو_اکر (character);

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
  وإلا إذا (gnu_extensions && !داخلا_reference)
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

  إذا (لفظ_regex.string)
    compile_regex (&لفظ_regex);
  وإلا إذا (!break_file)
    {
      إذا (gnu_extensions)
        {

          /* Simulate \w+.  */

          لكل (صحيح character = 0; character < CHAR_سيٽ_سائيز; character++)
            لفظ_fastmap[character] = !! اکر_آهي (character);
        }
      وإلا
        {

          /* Simulate [^ \t\n]+.  */

          memset (لفظ_fastmap, 1, CHAR_سيٽ_سائيز);
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

ساكن فراغ
swallow_file_اندريون_يادگيري (حرف ثابت *file_name, بلاڪ_2 *بلاڪ)
{
  size_t استعمال_ٿيل_ڊيگهائي;		/* used length in memory buffer */

  /* As special cases, a file name which is null or "-" indicates standard
     input, which is already opened.  In all other cases, open the file from
     its name.  */
  bool using_stdin = !file_name || !*file_name || streq (file_name, "-");
  إذا (using_stdin)
    بلاڪ->شروعات = fread_file (stdin, 0, &استعمال_ٿيل_ڊيگهائي);
  وإلا
    بلاڪ->شروعات = read_file (file_name, 0, &استعمال_ٿيل_ڊيگهائي);

  إذا (!بلاڪ->شروعات)
    error (EXIT_FAILURE, errno, "%s", quotef (using_stdin ? "-" : file_name));

  إذا (using_stdin)
    clearerr (stdin);

  بلاڪ->پڇاڙي = بلاڪ->شروعات + استعمال_ٿيل_ڊيگهائي;
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
compare_لفظ (ثابت فراغ *void_پهريون, ثابت فراغ *void_ٻيو)
{
  لفظ ثابت *پهريون = void_پهريون;
  لفظ ثابت *ٻيو = void_ٻيو;
  idx_t ڊيگهائي = MIN (پهريون->سائيز, ٻيو->سائيز);

  إذا (ignore_case)
    {
      لكل (idx_t counter = 0; counter < ڊيگهائي; counter++)
        {
          صحيح قدر = (folded_chars[منزل_uchar (پهريون->شروعات[counter])]
                       - folded_chars[منزل_uchar (ٻيو->شروعات[counter])]);
          إذا (قدر != 0)
            أرجع قدر;
        }
    }
  وإلا
    {
      لكل (idx_t counter = 0; counter < ڊيگهائي; counter++)
        {
          صحيح قدر = (منزل_uchar (پهريون->شروعات[counter])
                       - منزل_uchar (ٻيو->شروعات[counter]));
          إذا (قدر != 0)
            أرجع قدر;
        }
    }

  أرجع _GL_CMP (پهريون->سائيز, ٻيو->سائيز);
}

/*-----------------------------------------------------------------------.
| Decides which of two OCCURS, FIRST or SECOND, should lexicographically |
| go first.  In case of a tie, preserve the original order through a	 |
| pointer comparison.							 |
`-----------------------------------------------------------------------*/

ساكن صحيح
compare_occurs (ثابت فراغ *void_پهريون, ثابت فراغ *void_ٻيو)
{
#وصف_ڪريو پهريون ((ثابت OCCURS *) void_پهريون)
#وصف_ڪريو ٻيو ((ثابت OCCURS *) void_ٻيو)
  صحيح قدر;

  قدر = compare_لفظ (&پهريون->ڪنجي, &ٻيو->ڪنجي);
  أرجع (قدر ? قدر
          : _GL_CMP (پهريون->ڪنجي.شروعات, ٻيو->ڪنجي.شروعات));
#وصف_رد پهريون
#وصف_رد ٻيو
}

/* True if WORD appears in TABLE.  Uses a binary search.  */

ATTRIBUTE_خالص
ساكن bool
ڳولا_جدول (لفظ *لفظ_2, لفظ_جدول *جدول)
{
  idx_t lo = 0;
  idx_t hi = جدول->ڊيگهائي;
  طالما (lo < hi)
    {
      idx_t وچ = (lo >> 1) + (hi >> 1) + (lo & hi & 1);
      صحيح قدر = compare_لفظ (لفظ_2, جدول->شروعات + وچ);
      إذا (قدر < 0)
        hi = وچ;
      وإلا إذا (قدر > 0)
        lo = وچ + 1;
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
ترتيب_ڏيو_مليو_occurs (فراغ)
{

  /* Only one language for the time being.  */
  إذا (عدد_جو_occurs[0])
    qsort (occurs_جدول[0], عدد_جو_occurs[0], حجم **occurs_جدول,
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
  بلاڪ_2 file_contents;		/* to receive a copy of the file */

  swallow_file_اندريون_يادگيري (file_name, &file_contents);

  /* Make the fastmap and record the file contents in it.  */

  memset (لفظ_fastmap, 1, CHAR_سيٽ_سائيز);
  لكل (حرف *cursor = file_contents.شروعات; cursor < file_contents.پڇاڙي;
       cursor++)
    لفظ_fastmap[منزل_uchar (*cursor)] = 0;

  إذا (!gnu_extensions)
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

  حرر (file_contents.شروعات);
}

/*-----------------------------------------------------------------------.
| Read a file named FILE_NAME, containing one word per line, then	 |
| construct in TABLE a table of WORD descriptors for them.  The routine	 |
| swallows the whole file in memory; this is at the expense of space	 |
| needed for newlines, which are useless; however, the reading is fast.	 |
`-----------------------------------------------------------------------*/

ساكن فراغ
digest_لفظ_file (حرف ثابت *file_name, لفظ_جدول *جدول)
{
  بلاڪ_2 file_contents;		/* to receive a copy of the file */
  حرف *cursor;			/* cursor in file copy */
  حرف *لفظ_شروعات;		/* start of the current word */

  swallow_file_اندريون_يادگيري (file_name, &file_contents);

  جدول->شروعات = عدم;
  جدول->alloc = 0;
  جدول->ڊيگهائي = 0;

  /* Read the whole file.  */

  cursor = file_contents.شروعات;
  طالما (cursor < file_contents.پڇاڙي)
    {

      /* Read one line, and save the word in contains.  */

      لفظ_شروعات = cursor;
      طالما (cursor < file_contents.پڇاڙي && *cursor != '\n')
        cursor++;

      /* Record the word in table if it is not empty.  */

      إذا (cursor > لفظ_شروعات)
        {
          إذا (جدول->ڊيگهائي == جدول->alloc)
            جدول->شروعات = xpalloc (جدول->شروعات, &جدول->alloc, 1, -1,
                                    حجم *جدول->شروعات);
          idx_t ثابت جدول_idx = جدول->ڊيگهائي++;
          جدول->شروعات[جدول_idx].شروعات = لفظ_شروعات;
          جدول->شروعات[جدول_idx].سائيز = cursor - لفظ_شروعات;
        }

      /* This test allows for an incomplete line at end of file.  */

      إذا (cursor < file_contents.پڇاڙي)
        cursor++;
    }

  /* Finally, sort all the words read.  */

  qsort (جدول->شروعات, جدول->ڊيگهائي, حجم جدول->شروعات[0], compare_لفظ);
}

/* Keyword recognition and selection.  */

/*----------------------------------------------------------------------.
| For each keyword in the source text, constructs an OCCURS structure.  |
`----------------------------------------------------------------------*/

ساكن فراغ
ڳوليو_occurs_اندريون_متن (صحيح file_اشاريو)
{
  حرف *جاچ;			/* for scanning the source text */
  حرف *سٽ_شروعات;		/* start of the current input line */
  حرف *سٽ_جاچ;		/* newlines scanned until this point */
  idx_t reference_ڊيگهائي;	/* length of reference in input mode */
  لفظ possible_ڪنجي;		/* possible key, to ease searches */
  OCCURS *occurs_cursor;	/* current OCCURS under construction */

  حرف *context_شروعات;		/* start of left context */
  حرف *context_پڇاڙي;		/* end of right context */
  حرف *لفظ_شروعات;		/* start of word */
  حرف *لفظ_پڇاڙي;		/* end of word */
  حرف *ايندڙ_context_شروعات;	/* next start of left context */

  ثابت بلاڪ_2 *متن_بفر = &متن_buffers[file_اشاريو];

  /* reference_length is always used within 'if (input_reference)'.
     However, GNU C diagnoses that it may be used uninitialized.  The
     following assignment is merely to shut it up.  */

  reference_ڊيگهائي = 0;

  /* Tracking where lines start is helpful for reference processing.  In
     auto reference mode, this allows counting lines.  In input reference
     mode, this permits finding the beginning of the references.

     The first line begins with the file, skip immediately this very first
     reference in input reference mode, to help further rejection any word
     found inside it.  Also, unconditionally assigning these variable has
     the happy effect of shutting up lint.  */

  سٽ_شروعات = متن_بفر->شروعات;
  سٽ_جاچ = سٽ_شروعات;
  إذا (داخلا_reference)
    {
      SKIP_NON_WHITE (سٽ_جاچ, متن_بفر->پڇاڙي);
      reference_ڊيگهائي = سٽ_جاچ - سٽ_شروعات;
      SKIP_WHITE (سٽ_جاچ, متن_بفر->پڇاڙي);
    }

  /* Process the whole buffer, one line or one sentence at a time.  */

  لكل (حرف *cursor = متن_بفر->شروعات;
       cursor < متن_بفر->پڇاڙي;
       cursor = ايندڙ_context_شروعات)
    {

      /* 'context_start' gets initialized before the processing of each
         line, or once for the whole buffer if no end of line or sentence
         sequence separator.  */

      context_شروعات = cursor;

      /* If an end of line or end of sentence sequence is defined and
         non-empty, 'next_context_start' will be recomputed to be the end of
         each line or sentence, before each one is processed.  If no such
         sequence, then 'next_context_start' is set at the end of the whole
         buffer, which is then considered to be a single line or sentence.
         This test also accounts for the case of an incomplete line or
         sentence at the end of the buffer.  */

      ايندڙ_context_شروعات = متن_بفر->پڇاڙي;
      إذا (context_regex.string)
        بدل (re_ڳولا (&context_regex.نمونو_2, cursor,
                           متن_بفر->پڇاڙي - cursor,
                           0, متن_بفر->پڇاڙي - cursor, &context_regs))
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
            ايندڙ_context_شروعات = cursor + context_regs.پڇاڙي[0];
            اكسر;
          }

      /* Include the separator into the right context, but not any suffix
         white space in this separator; this insures it will be seen in
         output and will not take more space than necessary.  */

      context_پڇاڙي = ايندڙ_context_شروعات;
      SKIP_WHITE_BACKWARDS (context_پڇاڙي, context_شروعات);

      /* Read and process a single input line or sentence, one word at a
         time.  */

      طالما (true)
        {
          إذا (لفظ_regex.string)

            /* If a word regexp has been compiled, use it to skip at the
               beginning of the next word.  If there is no such word, exit
               the loop.  */

            {
              regoff_t r = re_ڳولا (&لفظ_regex.نمونو_2, cursor,
                                      context_پڇاڙي - cursor,
                                      0, context_پڇاڙي - cursor, &لفظ_regs);
              إذا (r == -2)
                matcher_error ();
              إذا (r == -1)
                اكسر;
              لفظ_شروعات = cursor + لفظ_regs.شروعات[0];
              لفظ_پڇاڙي = cursor + لفظ_regs.پڇاڙي[0];
            }
          وإلا

            /* Avoid re_search and use the fastmap to skip to the
               beginning of the next word.  If there is no more word in
               the buffer, exit the loop.  */

            {
              جاچ = cursor;
              طالما (جاچ < context_پڇاڙي
                     && !لفظ_fastmap[منزل_uchar (*جاچ)])
                جاچ++;

              إذا (جاچ == context_پڇاڙي)
                اكسر;

              لفظ_شروعات = جاچ;

              طالما (جاچ < context_پڇاڙي
                     && لفظ_fastmap[منزل_uchar (*جاچ)])
                جاچ++;

              لفظ_پڇاڙي = جاچ;
            }

          /* Skip right to the beginning of the found word.  */

          cursor = لفظ_شروعات;

          /* Skip any zero length word.  Just advance a single position,
             then go fetch the next word.  */

          إذا (لفظ_پڇاڙي == لفظ_شروعات)
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

          possible_ڪنجي.شروعات = cursor;
          possible_ڪنجي.سائيز = لفظ_پڇاڙي - لفظ_شروعات;
          cursor += possible_ڪنجي.سائيز;

          إذا (possible_ڪنجي.سائيز > وڌ_کان_وڌ_لفظ_ڊيگهائي)
            وڌ_کان_وڌ_لفظ_ڊيگهائي = possible_ڪنجي.سائيز;

          /* In input reference mode, update 'line_start' from its previous
             value.  Count the lines just in case auto reference mode is
             also selected. If it happens that the word just matched is
             indeed part of a reference; just ignore it.  */

          إذا (داخلا_reference)
            {
              طالما (سٽ_جاچ < possible_ڪنجي.شروعات)
                إذا (*سٽ_جاچ == '\n')
                  {
                    ڪل_سٽ_ڳڻپ++;
                    سٽ_جاچ++;
                    سٽ_شروعات = سٽ_جاچ;
                    SKIP_NON_WHITE (سٽ_جاچ, متن_بفر->پڇاڙي);
                    reference_ڊيگهائي = سٽ_جاچ - سٽ_شروعات;
                  }
                وإلا
                  سٽ_جاچ++;
              إذا (سٽ_جاچ > possible_ڪنجي.شروعات)
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

          إذا (ignore_file && ڳولا_جدول (&possible_ڪنجي, &ignore_جدول))
            استمر;
          إذا (only_file && !ڳولا_جدول (&possible_ڪنجي, &only_جدول))
            استمر;

          /* A non-empty word has been found.  First of all, insure
             proper allocation of the next OCCURS, and make a pointer to
             where it will be constructed.  */

          إذا (عدد_جو_occurs[0] == occurs_alloc[0])
            occurs_جدول[0] = xpalloc (occurs_جدول[0], &occurs_alloc[0],
                                       1, -1, حجم *occurs_جدول[0]);
          occurs_cursor = occurs_جدول[0] + عدد_جو_occurs[0];

          /* Define the reference field, if any.  */

          إذا (auto_reference)
            {

              /* While auto referencing, update 'line_start' from its
                 previous value, counting lines as we go.  If input
                 referencing at the same time, 'line_start' has been
                 advanced earlier, and the following loop is never really
                 executed.  */

              طالما (سٽ_جاچ < possible_ڪنجي.شروعات)
                إذا (*سٽ_جاچ == '\n')
                  {
                    ڪل_سٽ_ڳڻپ++;
                    سٽ_جاچ++;
                    سٽ_شروعات = سٽ_جاچ;
                    SKIP_NON_WHITE (سٽ_جاچ, متن_بفر->پڇاڙي);
                  }
                وإلا
                  سٽ_جاچ++;

              occurs_cursor->reference = ڪل_سٽ_ڳڻپ;
            }
          وإلا إذا (داخلا_reference)
            {

              /* If only input referencing, 'line_start' has been computed
                 earlier to detect the case the word matched would be part
                 of the reference.  The reference position is simply the
                 value of 'line_start'.  */

              occurs_cursor->reference = سٽ_شروعات - possible_ڪنجي.شروعات;
              إذا (reference_ڊيگهائي > reference_max_ويڪر)
                reference_max_ويڪر = reference_ڊيگهائي;
            }

          /* Exclude the reference from the context in simple cases.  */

          إذا (داخلا_reference && سٽ_شروعات == context_شروعات)
            {
              SKIP_NON_WHITE (context_شروعات, context_پڇاڙي);
              SKIP_WHITE (context_شروعات, context_پڇاڙي);
            }

          /* Completes the OCCURS structure.  */

          occurs_cursor->ڪنجي = possible_ڪنجي;
          occurs_cursor->کاٻو = context_شروعات - possible_ڪنجي.شروعات;
          occurs_cursor->ساڄو = context_پڇاڙي - possible_ڪنجي.شروعات;
          occurs_cursor->file_اشاريو = file_اشاريو;

          عدد_جو_occurs[0]++;
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
    اکر_لکو (' ');
}

/*-------------------------------------.
| Prints the field provided by FIELD.  |
`-------------------------------------*/

ساكن فراغ
print_ميدان (بلاڪ_2 ميدان)
{
  /* Whitespace is not really compressed.  Instead, each white space
     character (tab, vt, ht etc.) is printed as one single space.  */

  لكل (حرف *cursor = ميدان.شروعات; cursor < ميدان.پڇاڙي; cursor++)
    {
      بنانشان حرف character = *cursor;
      إذا (edited_flag[character])
        {
          /* Handle cases which are specific to 'roff' or TeX.  All
             white space processing is done as the default case of
             this switch.  */

          بدل (character)
            {
            حالة '"':
              /* In roff output format, double any quote.  */
              اکر_لکو ('"');
              اکر_لکو ('"');
              اكسر;

            حالة '$':
            حالة '%':
            حالة '&':
            حالة '#':
            حالة '_':
              /* In TeX output format, precede these with a backslash.  */
              اکر_لکو ('\\');
              اکر_لکو (character);
              اكسر;

            حالة '{':
            حالة '}':
              /* In TeX output format, precede these with a backslash and
                 force mathematical mode.  */
              لکو ("$\\%c$", character);
              اكسر;

            حالة '\\':
              /* In TeX output mode, request production of a backslash.  */
              fputs ("\\backslash{}", stdout);
              اكسر;

            افتراضي:
              /* Any other flagged character produces a single space.  */
              اکر_لکو (' ');
            }
        }
      وإلا
        اکر_لکو (*cursor);
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
      reference_max_ويڪر = 0;
      لكل (صحيح file_اشاريو = 0; file_اشاريو < عدد_داخلا_files; file_اشاريو++)
        {
          intmax_t سٽ_ordinal = file_سٽ_ڳڻپ[file_اشاريو] + 1;
          إذا (file_اشاريو > 0)
            سٽ_ordinal -= file_سٽ_ڳڻپ[file_اشاريو - 1];
          حرف ordinal_string[INT_BUFSIZE_BOUND (intmax_t)];
          idx_t reference_ويڪر = sprintf (ordinal_string, "%jd", سٽ_ordinal);
          إذا (داخلا_file_name[file_اشاريو])
            reference_ويڪر += لڙي_ڊيگهه (داخلا_file_name[file_اشاريو]);
          إذا (reference_ويڪر > reference_max_ويڪر)
            reference_max_ويڪر = reference_ويڪر;
        }
      reference_max_ويڪر++;
      reference.شروعات = xmalloc (reference_max_ويڪر + 1);
    }

  /* If the reference appears to the left of the output line, reserve some
     space for it right away, including one gap size.  */

  إذا ((auto_reference || داخلا_reference) && !ساڄو_reference)
    سٽ_ويڪر = MAX (0, سٽ_ويڪر - (reference_max_ويڪر + gap_سائيز));

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

  half_سٽ_ويڪر = سٽ_ويڪر >> 1;
  before_max_ويڪر = half_سٽ_ويڪر - gap_سائيز;
  keyafter_max_ويڪر = half_سٽ_ويڪر;

  /* If truncation_string is the empty string, make it null to speed up
     tests.  In this case, truncation_string_length will never get used, so
     there is no need to set it.  */

  إذا (truncation_string && *truncation_string)
    truncation_string_ڊيگهائي = لڙي_ڊيگهه (truncation_string);
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

      before_max_ويڪر -= 2 * truncation_string_ڊيگهائي;
      keyafter_max_ويڪر -= 2 * truncation_string_ڊيگهائي;
    }
  وإلا
    {

      /* I never figured out exactly how UNIX' ptx plans the output width
         of its various fields.  If GNU extensions are disabled, do not
         try computing the field widths correctly; instead, use the
         following formula, which does not completely imitate UNIX' ptx,
         but almost.  */

      keyafter_max_ويڪر -= 2 * truncation_string_ڊيگهائي + 1;
    }

  إذا (before_max_ويڪر < 0)
    before_max_ويڪر = 0;

  /* Compute which characters need special output processing.  Initialize
     by flagging any white space character.  Some systems do not consider
     form feed as a space character, but we do.  */

  لكل (صحيح character = 0; character < CHAR_سيٽ_سائيز; character++)
    edited_flag[character] = !! خال_آهي (character);
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
  ptrdiff_t پڇ_max_ويڪر;	/* allowable width of tail field */
  ptrdiff_t سر_max_ويڪر;	/* allowable width of head field */
  حرف *cursor;			/* running cursor in source text */
  حرف *کاٻو_context_شروعات;	/* start of left context */
  حرف *ساڄو_context_پڇاڙي;	/* end of right context */
  حرف *کاٻو_ميدان_شروعات;	/* conservative start for 'head'/'before' */
  حرف ثابت *file_name;	/* file name for reference */
  intmax_t سٽ_ordinal;	/* line ordinal for reference */
  حرف ثابت *بفر_شروعات;	/* start of buffered file for this occurs */
  حرف ثابت *بفر_پڇاڙي;	/* end of buffered file for this occurs */

  /* Define 'keyafter', start of left context and end of right context.
     'keyafter' starts at the saved position for keyword and extend to the
     right from the end of the keyword, eating separators or full words, but
     not beyond maximum allowed width for 'keyafter' field or limit for the
     right context.  Suffix spaces will be removed afterwards.  */

  keyafter.شروعات = occurs->ڪنجي.شروعات;
  keyafter.پڇاڙي = keyafter.شروعات + occurs->ڪنجي.سائيز;
  کاٻو_context_شروعات = keyafter.شروعات + occurs->کاٻو;
  ساڄو_context_پڇاڙي = keyafter.شروعات + occurs->ساڄو;

  بفر_شروعات = متن_buffers[occurs->file_اشاريو].شروعات;
  بفر_پڇاڙي = متن_buffers[occurs->file_اشاريو].پڇاڙي;

  cursor = keyafter.پڇاڙي;
  طالما (cursor < ساڄو_context_پڇاڙي
         && cursor <= keyafter.شروعات + keyafter_max_ويڪر)
    {
      keyafter.پڇاڙي = cursor;
      SKIP_SOMETHING (cursor, ساڄو_context_پڇاڙي);
    }
  إذا (cursor <= keyafter.شروعات + keyafter_max_ويڪر)
    keyafter.پڇاڙي = cursor;

  keyafter_truncation = truncation_string && keyafter.پڇاڙي < ساڄو_context_پڇاڙي;

  SKIP_WHITE_BACKWARDS (keyafter.پڇاڙي, keyafter.شروعات);

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

  إذا (-occurs->کاٻو > half_سٽ_ويڪر + وڌ_کان_وڌ_لفظ_ڊيگهائي)
    {
      کاٻو_ميدان_شروعات
        = keyafter.شروعات - (half_سٽ_ويڪر + وڌ_کان_وڌ_لفظ_ڊيگهائي);
      SKIP_SOMETHING (کاٻو_ميدان_شروعات, keyafter.شروعات);
    }
  وإلا
    کاٻو_ميدان_شروعات = keyafter.شروعات + occurs->کاٻو;

  /* 'before' certainly ends at the keyword, but not including separating
     spaces.  It starts after than the saved value for the left context, by
     advancing it until it falls inside the maximum allowed width for the
     before field.  There will be no prefix spaces either.  'before' only
     advances by skipping single separators or whole words. */

  before.شروعات = کاٻو_ميدان_شروعات;
  before.پڇاڙي = keyafter.شروعات;
  SKIP_WHITE_BACKWARDS (before.پڇاڙي, before.شروعات);

  طالما (before.شروعات + before_max_ويڪر < before.پڇاڙي)
    SKIP_SOMETHING (before.شروعات, before.پڇاڙي);

  إذا (truncation_string)
    {
      cursor = before.شروعات;
      SKIP_WHITE_BACKWARDS (cursor, بفر_شروعات);
      before_truncation = cursor > کاٻو_context_شروعات;
    }
  وإلا
    before_truncation = false;

  SKIP_WHITE (before.شروعات, بفر_پڇاڙي);

  /* The tail could not take more columns than what has been left in the
     left context field, and a gap is mandatory.  It starts after the
     right context, and does not contain prefixed spaces.  It ends at
     the end of line, the end of buffer or when the tail field is full,
     whichever comes first.  It cannot contain only part of a word, and
     has no suffixed spaces.  */

  پڇ_max_ويڪر
    = before_max_ويڪر - (before.پڇاڙي - before.شروعات) - gap_سائيز;

  إذا (پڇ_max_ويڪر > 0)
    {
      پڇ.شروعات = keyafter.پڇاڙي;
      SKIP_WHITE (پڇ.شروعات, بفر_پڇاڙي);

      پڇ.پڇاڙي = پڇ.شروعات;
      cursor = پڇ.پڇاڙي;
      طالما (cursor < ساڄو_context_پڇاڙي
             && cursor < پڇ.شروعات + پڇ_max_ويڪر)
        {
          پڇ.پڇاڙي = cursor;
          SKIP_SOMETHING (cursor, ساڄو_context_پڇاڙي);
        }

      إذا (cursor < پڇ.شروعات + پڇ_max_ويڪر)
        پڇ.پڇاڙي = cursor;

      إذا (پڇ.پڇاڙي > پڇ.شروعات)
        {
          keyafter_truncation = false;
          پڇ_truncation = truncation_string && پڇ.پڇاڙي < ساڄو_context_پڇاڙي;
        }
      وإلا
        پڇ_truncation = false;

      SKIP_WHITE_BACKWARDS (پڇ.پڇاڙي, پڇ.شروعات);
    }
  وإلا
    {

      /* No place left for a tail field.  */

      پڇ.شروعات = عدم;
      پڇ.پڇاڙي = عدم;
      پڇ_truncation = false;
    }

  /* 'head' could not take more columns than what has been left in the right
     context field, and a gap is mandatory.  It ends before the left
     context, and does not contain suffixed spaces.  Its pointer is advanced
     until the head field has shrunk to its allowed width.  It cannot
     contain only part of a word, and has no suffixed spaces.  */

  سر_max_ويڪر
    = keyafter_max_ويڪر - (keyafter.پڇاڙي - keyafter.شروعات) - gap_سائيز;

  إذا (سر_max_ويڪر > 0)
    {
      سر.پڇاڙي = before.شروعات;
      SKIP_WHITE_BACKWARDS (سر.پڇاڙي, بفر_شروعات);

      سر.شروعات = کاٻو_ميدان_شروعات;
      طالما (سر.شروعات + سر_max_ويڪر < سر.پڇاڙي)
        SKIP_SOMETHING (سر.شروعات, سر.پڇاڙي);

      إذا (سر.پڇاڙي > سر.شروعات)
        {
          before_truncation = false;
          سر_truncation = (truncation_string
                             && سر.شروعات > کاٻو_context_شروعات);
        }
      وإلا
        سر_truncation = false;

      SKIP_WHITE (سر.شروعات, سر.پڇاڙي);
    }
  وإلا
    {

      /* No place left for a head field.  */

      سر.شروعات = عدم;
      سر.پڇاڙي = عدم;
      سر_truncation = false;
    }

  إذا (auto_reference)
    {

      /* Construct the reference text in preallocated space from the file
         name and the line number.  Standard input yields an empty file name.
         Ensure line numbers are 1 based, even if they are computed 0 based.  */

      file_name = داخلا_file_name[occurs->file_اشاريو];
      إذا (!file_name)
        file_name = "";

      سٽ_ordinal = occurs->reference + 1;
      إذا (occurs->file_اشاريو > 0)
        سٽ_ordinal -= file_سٽ_ڳڻپ[occurs->file_اشاريو - 1];

      حرف *file_پڇاڙي = stpcpy (reference.شروعات, file_name);
      reference.پڇاڙي = file_پڇاڙي + sprintf (file_پڇاڙي, ":%jd", سٽ_ordinal);
    }
  وإلا إذا (داخلا_reference)
    {

      /* Reference starts at saved position for reference and extends right
         until some white space is met.  */

      reference.شروعات = keyafter.شروعات + occurs->reference;
      reference.پڇاڙي = reference.شروعات;
      SKIP_NON_WHITE (reference.پڇاڙي, ساڄو_context_پڇاڙي);
    }
}

/* Formatting and actual output - control routines.  */

/*----------------------------------------------------------------------.
| Output the current output fields as one line for 'troff' or 'nroff'.  |
`----------------------------------------------------------------------*/

ساكن فراغ
output_one_roff_سٽ (فراغ)
{
  /* Output the 'tail' field.  */

  لکو (".%s \"", macro_name);
  print_ميدان (پڇ);
  إذا (پڇ_truncation)
    fputs (truncation_string, stdout);
  اکر_لکو ('"');

  /* Output the 'before' field.  */

  fputs (" \"", stdout);
  إذا (before_truncation)
    fputs (truncation_string, stdout);
  print_ميدان (before);
  اکر_لکو ('"');

  /* Output the 'keyafter' field.  */

  fputs (" \"", stdout);
  print_ميدان (keyafter);
  إذا (keyafter_truncation)
    fputs (truncation_string, stdout);
  اکر_لکو ('"');

  /* Output the 'head' field.  */

  fputs (" \"", stdout);
  إذا (سر_truncation)
    fputs (truncation_string, stdout);
  print_ميدان (سر);
  اکر_لکو ('"');

  /* Conditionally output the 'reference' field.  */

  إذا (auto_reference || داخلا_reference)
    {
      fputs (" \"", stdout);
      print_ميدان (reference);
      اکر_لکو ('"');
    }

  اکر_لکو ('\n');
}

/*---------------------------------------------------------.
| Output the current output fields as one line for 'TeX'.  |
`---------------------------------------------------------*/

ساكن فراغ
output_one_tex_سٽ (فراغ)
{
  بلاڪ_2 ڪنجي;			/* key field, isolated */
  بلاڪ_2 after;			/* after field, isolated */
  حرف *cursor;			/* running cursor in source text */

  لکو ("\\%s ", macro_name);
  اکر_لکو ('{');
  print_ميدان (پڇ);
  fputs ("}{", stdout);
  print_ميدان (before);
  fputs ("}{", stdout);
  ڪنجي.شروعات = keyafter.شروعات;
  after.پڇاڙي = keyafter.پڇاڙي;
  cursor = keyafter.شروعات;
  SKIP_SOMETHING (cursor, keyafter.پڇاڙي);
  ڪنجي.پڇاڙي = cursor;
  after.شروعات = cursor;
  print_ميدان (ڪنجي);
  fputs ("}{", stdout);
  print_ميدان (after);
  fputs ("}{", stdout);
  print_ميدان (سر);
  اکر_لکو ('}');
  إذا (auto_reference || داخلا_reference)
    {
      اکر_لکو ('{');
      print_ميدان (reference);
      اکر_لکو ('}');
    }
  اکر_لکو ('\n');
}

/*-------------------------------------------------------------------.
| Output the current output fields as one line for a dumb terminal.  |
`-------------------------------------------------------------------*/

ساكن فراغ
output_one_dumb_سٽ (فراغ)
{
  إذا (!ساڄو_reference)
    {
      إذا (auto_reference)
        {

          /* Output the 'reference' field, in such a way that GNU emacs
             next-error will handle it.  The ending colon is taken from the
             gap which follows.  */

          print_ميدان (reference);
          اکر_لکو (':');
          print_spaces (reference_max_ويڪر
                        + gap_سائيز
                        - (reference.پڇاڙي - reference.شروعات)
                        - 1);
        }
      وإلا
        {

          /* Output the 'reference' field and its following gap.  */

          print_ميدان (reference);
          print_spaces (reference_max_ويڪر
                        + gap_سائيز
                        - (reference.پڇاڙي - reference.شروعات));
        }
    }

  إذا (پڇ.شروعات < پڇ.پڇاڙي)
    {
      /* Output the 'tail' field.  */

      print_ميدان (پڇ);
      إذا (پڇ_truncation)
        fputs (truncation_string, stdout);

      print_spaces (half_سٽ_ويڪر - gap_سائيز
                    - (before.پڇاڙي - before.شروعات)
                    - (before_truncation ? truncation_string_ڊيگهائي : 0)
                    - (پڇ.پڇاڙي - پڇ.شروعات)
                    - (پڇ_truncation ? truncation_string_ڊيگهائي : 0));
    }
  وإلا
    print_spaces (half_سٽ_ويڪر - gap_سائيز
                  - (before.پڇاڙي - before.شروعات)
                  - (before_truncation ? truncation_string_ڊيگهائي : 0));

  /* Output the 'before' field.  */

  إذا (before_truncation)
    fputs (truncation_string, stdout);
  print_ميدان (before);

  print_spaces (gap_سائيز);

  /* Output the 'keyafter' field.  */

  print_ميدان (keyafter);
  إذا (keyafter_truncation)
    fputs (truncation_string, stdout);

  إذا (سر.شروعات < سر.پڇاڙي)
    {
      /* Output the 'head' field.  */

      print_spaces (half_سٽ_ويڪر
                    - (keyafter.پڇاڙي - keyafter.شروعات)
                    - (keyafter_truncation ? truncation_string_ڊيگهائي : 0)
                    - (سر.پڇاڙي - سر.شروعات)
                    - (سر_truncation ? truncation_string_ڊيگهائي : 0));
      إذا (سر_truncation)
        fputs (truncation_string, stdout);
      print_ميدان (سر);
    }
  وإلا

    إذا ((auto_reference || داخلا_reference) && ساڄو_reference)
      print_spaces (half_سٽ_ويڪر
                    - (keyafter.پڇاڙي - keyafter.شروعات)
                    - (keyafter_truncation ? truncation_string_ڊيگهائي : 0));

  إذا ((auto_reference || داخلا_reference) && ساڄو_reference)
    {
      /* Output the 'reference' field.  */

      print_spaces (gap_سائيز);
      print_ميدان (reference);
    }

  اکر_لکو ('\n');
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

  پڇ.شروعات = عدم;
  پڇ.پڇاڙي = عدم;
  پڇ_truncation = false;

  سر.شروعات = عدم;
  سر.پڇاڙي = عدم;
  سر_truncation = false;

  /* Loop over all keyword occurrences.  */

  occurs_cursor = occurs_جدول[0];

  لكل (idx_t occurs_اشاريو = 0; occurs_اشاريو < عدد_جو_occurs[0];
       occurs_اشاريو++)
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
          output_one_dumb_سٽ ();
          اكسر;

        حالة ROFF_FORMAT:
          output_one_roff_سٽ ();
          اكسر;

        حالة TEX_FORMAT:
          output_one_tex_سٽ ();
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
      لکو (_("\
Usage: %s [OPTION]... [داخلا_3]...   (without -G)\n\
  or:  %s -G [OPTION]... [داخلا_3 [OUTPUT]]\n"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Output a permuted index, including context, جو the لفظ_4 اندريون the داخلا files.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --خودڪار-reference           output automatically generated references\n\
"));
      oputs (_("\
  -G, --traditional              behave وڌيڪ like System V 'ptx'\n\
"));
      oputs (_("\
  -F, --flag-truncation=STRING   use STRING لكل flagging سٽ truncations.\n\
                                 The افتراضي is '/'\n\
"));
      oputs (_("\
  -M, --macro-name=STRING        macro name منزل use instead جو 'xx'\n\
"));
      oputs (_("\
  -O, --format=roff              generate output as roff directives\n\
"));
      oputs (_("\
  -R, --ساڄو-side-refs          put references at ساڄو, not counted اندريون -w\n\
"));
      oputs (_("\
  -S, --sentence-regexp=REGEXP   لكل پڇاڙي جو سٽون or پڇاڙي جو sentences\n\
"));
      oputs (_("\
  -T, --format=tex               generate output as TeX directives\n\
"));
      oputs (_("\
  -W, --لفظ_2-regexp=REGEXP       use REGEXP منزل ميلاپ each keyword\n\
"));
      oputs (_("\
  -b, --اكسر-file=FILE          لفظ_2 اكسر characters اندريون this FILE\n\
"));
      oputs (_("\
  -f, --ignore-حالة              fold هيٺاهون حالة منزل مٿانهون حالة لكل ترتيب_ڏيڻ\n\
"));
      oputs (_("\
  -g, --gap-سائيز=عدد_2          gap سائيز اندريون columns between output fields\n\
"));
      oputs (_("\
  -i, --ignore-file=FILE         اقرأ ignore لفظ_2 list from FILE\n\
"));
      oputs (_("\
  -o, --only-file=FILE           اقرأ only لفظ_2 list from this FILE\n\
"));
      oputs (_("\
  -r, --references               پهريون ميدان جو each سٽ is a reference\n\
"));
      oputs (_("\
  -t, --typeset-گهڻائي_2             change the افتراضي ويڪر from 72 منزل 100\n\
"));
      oputs (_("\
  -w, --ويڪر=عدد_2             output ويڪر اندريون columns, reference excluded\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
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
  {"auto-reference", no_دليل, عدم, 'A'},
  {"break-file", required_دليل, عدم, 'b'},
  {"flag-truncation", required_دليل, عدم, 'F'},
  {"ignore-case", no_دليل, عدم, 'f'},
  {"gap-size", required_دليل, عدم, 'g'},
  {"ignore-file", required_دليل, عدم, 'i'},
  {"macro-name", required_دليل, عدم, 'M'},
  {"only-file", required_دليل, عدم, 'o'},
  {"references", no_دليل, عدم, 'r'},
  {"right-side-refs", no_دليل, عدم, 'R'},
  {"format", required_دليل, عدم, FORMAT_OPTION},
  {"sentence-regexp", required_دليل, عدم, 'S'},
  {"traditional", no_دليل, عدم, 'G'},
  {"typeset-mode", no_دليل, عدم, 't'},
  {"width", required_دليل, عدم, 'w'},
  {"word-regexp", required_دليل, عدم, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

ساكن حرف ثابت *ثابت format_دليلون[] =
{
  "roff", "tex", عدم
};

ساكن تعداد Format ثابت format_vals[] =
{
  ROFF_FORMAT, TEX_FORMAT
};

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح optchar;			/* argument character */
  صحيح file_اشاريو;		/* index in text input file arrays */

  /* Decode program options.  */

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
            gap_سائيز = tmp;
            اكسر;
          }

        حالة 'i':
          ignore_file = optarg;
          اكسر;

        حالة 'o':
          only_file = optarg;
          اكسر;

        حالة 'r':
          داخلا_reference = true;
          اكسر;

        حالة 't':
          إذا (سٽ_ويڪر < 0)
            سٽ_ويڪر = 100;
          اكسر;

        حالة 'w':
          {
            intmax_t tmp;
            إذا (! (xstrtoimax (optarg, عدم, 0, &tmp, "") == LONGINT_OK
                   && 0 < tmp && tmp <= IDX_MAX))
              error (EXIT_FAILURE, 0, _("invalid line width: %s"),
                     quote (optarg));
            سٽ_ويڪر = tmp;
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
          ساڄو_reference = true;
          اكسر;

        حالة 'S':
          context_regex.string = optarg;
          unescape_string (optarg);
          اكسر;

        حالة 'T':
          output_format = TEX_FORMAT;
          اكسر;

        حالة 'W':
          لفظ_regex.string = optarg;
          unescape_string (optarg);
          إذا (!*لفظ_regex.string)
            لفظ_regex.string = عدم;
          اكسر;

        حالة FORMAT_OPTION:
          output_format = XARGMATCH ("--format", optarg,
                                     format_دليلون, format_vals);
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
        }
    }

  إذا (سٽ_ويڪر < 0)
    سٽ_ويڪر = 72;

  /* Process remaining arguments.  If GNU extensions are enabled, process
     all arguments as input parameters.  If disabled, accept at most two
     arguments, the second of which is an output parameter.  */

  إذا (optind == argc)
    {

      /* No more argument simply means: read standard input.  */

      داخلا_file_name = xmalloc (حجم *داخلا_file_name);
      file_سٽ_ڳڻپ = xmalloc (حجم *file_سٽ_ڳڻپ);
      متن_buffers =    xmalloc (حجم *متن_buffers);
      عدد_داخلا_files = 1;
      داخلا_file_name[0] = عدم;
    }
  وإلا إذا (gnu_extensions)
    {
      عدد_داخلا_files = argc - optind;
      داخلا_file_name = xnmalloc (عدد_داخلا_files, حجم *داخلا_file_name);
      file_سٽ_ڳڻپ = xnmalloc (عدد_داخلا_files, حجم *file_سٽ_ڳڻپ);
      متن_buffers    = xnmalloc (عدد_داخلا_files, حجم *متن_buffers);

      لكل (file_اشاريو = 0; file_اشاريو < عدد_داخلا_files; file_اشاريو++)
        {
          إذا (!*argv[optind] || streq (argv[optind], "-"))
            داخلا_file_name[file_اشاريو] = عدم;
          وإلا
            داخلا_file_name[file_اشاريو] = argv[optind];
          optind++;
        }
    }
  وإلا
    {

      /* There is one necessary input file.  */

      عدد_داخلا_files = 1;
      داخلا_file_name = xmalloc (حجم *داخلا_file_name);
      file_سٽ_ڳڻپ = xmalloc (حجم *file_سٽ_ڳڻپ);
      متن_buffers    = xmalloc (حجم *متن_buffers);
      إذا (!*argv[optind] || streq (argv[optind], "-"))
        داخلا_file_name[0] = عدم;
      وإلا
        داخلا_file_name[0] = argv[optind];
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
      digest_لفظ_file (ignore_file, &ignore_جدول);
      إذا (ignore_جدول.ڊيگهائي == 0)
        ignore_file = عدم;
    }

  إذا (only_file)
    {
      digest_لفظ_file (only_file, &only_جدول);
      إذا (only_جدول.ڊيگهائي == 0)
        only_file = عدم;
    }

  /* Prepare to study all the input files.  */

  عدد_جو_occurs[0] = 0;
  ڪل_سٽ_ڳڻپ = 0;
  وڌ_کان_وڌ_لفظ_ڊيگهائي = 0;
  reference_max_ويڪر = 0;

  لكل (file_اشاريو = 0; file_اشاريو < عدد_داخلا_files; file_اشاريو++)
    {
      بلاڪ_2 *متن_بفر = متن_buffers + file_اشاريو;

      /* Read the file contents into memory, then study it.  */

      swallow_file_اندريون_يادگيري (داخلا_file_name[file_اشاريو], متن_بفر);
      ڳوليو_occurs_اندريون_متن (file_اشاريو);

      /* Maintain for each file how many lines has been read so far when its
         end is reached.  Incrementing the count first is a simple kludge to
         handle a possible incomplete line at end of file.  */

      ڪل_سٽ_ڳڻپ++;
      file_سٽ_ڳڻپ[file_اشاريو] = ڪل_سٽ_ڳڻپ;
    }

  /* Do the output process phase.  */

  ترتيب_ڏيو_مليو_occurs ();
  fix_output_parameters ();
  generate_all_output ();

  /* All done.  */

  أرجع EXIT_SUCCESS;
}
