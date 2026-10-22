/* GNU fmt -- simple text formatter.
   Copyright (C) 1994-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Written by Ross Paterson <rap@doc.ic.ac.uk>.  */

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <getopt.h>

/* Redefine.  Otherwise, systems (Unicos for one) with headers that define
   it to be a type get syntax errors for the variable declaration below.  */
#تعریف_کن کلمه_2 استفادهنشده_کلمه_type

#ضمن "c-ctype.h"
#ضمن "system.h"
#ضمن "fadvise.h"
#ضمن "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "fmt"

#تعریف_کن AUTHORS proper_name ("Ross Paterson")

/* The following parameters represent the program's idea of what is
   "best".  Adjust to taste, subject to the caveats given.  */

/* Default longest permitted line length (max_width).  */
#تعریف_کن عرض_2	75

/* Prefer lines to be LEEWAY % shorter than the maximum width, giving
   room for optimization.  */
#تعریف_کن LEEWAY	7

/* The default secondary indent of tagged paragraph used for unindented
   one-line paragraphs not preceded by any multi-line paragraphs.  */
#تعریف_کن DEF_INDENT 3

/* Costs and bonuses are expressed as the equivalent departure from the
   optimal line length, multiplied by 10.  e.g. assigning something a
   cost of 50 means that it is as bad as a line 5 characters too short
   or too long.  The definition of SHORT_COST(n) should not be changed.
   However, EQUIV(n) may need tuning.  */

/* FIXME: "fmt" misbehaves given large inputs or options.  One
   possible workaround for part of the problem is to change COST to be
   a floating-point type.  There are other problems besides COST,
   though; see MAXWORDS below.  */

تعريفنوع دراز صحيح هزینه;

#تعریف_کن MAXCOST	TYPE_بیشینه (هزینه)

#تعریف_کن SQR(n)		((n) * (n))
#تعریف_کن EQUIV(n)	SQR ((هزینه) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#تعریف_کن SHORT_هزینه(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#تعریف_کن RAGGED_هزینه(n)	(SHORT_هزینه (n) / 2)

/* Basic cost per line.  */
#تعریف_کن سطر_هزینه	EQUIV (70)

/* Cost of breaking a line after the first word of a sentence, where
   the length of the word is N.  */
#تعریف_کن WIDOW_هزینه(n)	(EQUIV (200) / ((n) + 2))

/* Cost of breaking a line before the last word of a sentence, where
   the length of the word is N.  */
#تعریف_کن ORPHAN_هزینه(n)	(EQUIV (150) / ((n) + 2))

/* Bonus for breaking a line at the end of a sentence.  */
#تعریف_کن SENTENCE_BONUS	EQUIV (50)

/* Cost of breaking a line after a period not marking end of a sentence.
   With the definition of sentence we are using (borrowed from emacs, see
   get_line()) such a break would then look like a sentence break.  Hence
   we assign a very high cost -- it should be avoided unless things are
   really bad.  */
#تعریف_کن NOBREAK_هزینه	EQUIV (600)

/* Bonus for breaking a line before open parenthesis.  */
#تعریف_کن PAREN_BONUS	EQUIV (40)

/* Bonus for breaking a line after other punctuation.  */
#تعریف_کن PUNCT_BONUS	EQUIV(40)

/* Credit for breaking a long paragraph one line later.  */
#تعریف_کن سطر_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#تعریف_کن MAXWORDS	1000
#تعریف_کن MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#تعریف_کن isopen(c)	(strchr ("(['`\"", c) != عدم)
#تعریف_کن isclose(c)	(strchr (")]'\"", c) != عدم)
#تعریف_کن isperiod(c)	(strchr (".?!", c) != عدم)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#تعریف_کن TABWIDTH	8

/* Word descriptor structure.  */

تعريفنوع بنية کلمه_3 کلمه;

بنية کلمه_3
  {

    /* Static attributes determined during input.  */

    حرف ثابت *متن;		/* the text of the word */
    صحيح درازی;			/* length of this word */
    صحيح space;			/* the size of the following space */
    بیعلامت صحيح paren:1;	/* starts with open paren */
    بیعلامت صحيح period:1;	/* ends in [.?!])* */
    بیعلامت صحيح punct:1;	/* ends in punctuation */
    بیعلامت صحيح final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    صحيح سطر_درازی;		/* length of the best line starting here */
    هزینه بهترین_هزینه;		/* cost of best paragraph starting here */
    کلمه *بعدی_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

ساكن فراغ مجموعه_پیشوند (حرف *p);
ساكن bool fmt (FILE *f, حرف ثابت *);
ساكن bool get_paragraph (FILE *f);
ساكن صحيح get_سطر (FILE *f, صحيح c);
ساكن صحيح get_پیشوند (FILE *f);
ساكن صحيح get_space (FILE *f, صحيح c);
ساكن صحيح copy_rest (FILE *f, صحيح c);
ساكن bool same_para (صحيح c);
ساكن فراغ flush_paragraph (فراغ);
ساكن فراغ fmt_paragraph (فراغ);
ساكن فراغ check_punctuation (کلمه *w);
ساكن هزینه پایه_هزینه (کلمه *this);
ساكن هزینه سطر_هزینه_2 (کلمه *بعدی, صحيح len);
ساكن فراغ put_paragraph (کلمه *پایان);
ساكن فراغ put_سطر (کلمه *w, صحيح indent);
ساكن فراغ put_کلمه (کلمه *w);
ساكن فراغ put_space (صحيح space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
ساكن bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
ساكن bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
ساكن bool تقسیم;

/* If true, don't preserve inter-word spacing (default false).  */
ساكن bool یکنواخت;

/* Prefix minus leading and trailing spaces (default "").  */
ساكن حرف ثابت *پیشوند = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
ساكن صحيح max_عرض = عرض_2;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
ساكن صحيح پیشوند_پر_درازی;

/* The length of the leading space trimmed from the prefix.  */
ساكن صحيح پیشوند_lead_space;

/* The length of prefix minus leading and trailing space.  */
ساكن صحيح پیشوند_درازی;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
ساكن صحيح goal_عرض;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
ساكن صحيح درون_ستونی;

/* Start column of the next character to be written to stdout.  */
ساكن صحيح بیرون_ستونی;

/* Space for the paragraph text -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ساكن حرف parabuf[MAXCHARS];

/* A pointer into parabuf, indicating the first unused character position.  */
ساكن حرف *wptr;

/* The words of a paragraph -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ساكن کلمه کلمه_2[MAXWORDS];

/* A pointer into the above word array, indicating the first position
   after the last complete word.  Sometimes it will point at an incomplete
   word.  */
ساكن کلمه *کلمه_حد;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
ساكن bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
ساكن صحيح پیشوند_indent;

/* Indentation of the first line of the current paragraph.  */
ساكن صحيح اولی_indent;

/* Indentation of other lines of the current paragraph */
ساكن صحيح other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
ساكن صحيح بعدی_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
ساكن صحيح بعدی_پیشوند_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
ساكن صحيح آخرین_سطر_درازی;

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("Usage: %s [-WIDTH] [OPTION]... [FILE]...\n"), برنامه_name);
      fputs (_("\
Reformat each paragraph درون the FILE(s), writing مقصد standard output.\n\
The option -عرض_2 is an abbreviated form از --عرض=DIGITS.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --crown-margin\n\
         preserve indentation از اولی two سطرها\n\
"));
      oputs (_("\
  -p, --پیشوند=STRING\n\
         reformat only سطرها beginning with STRING,\n\
         reattaching the پیشوند مقصد reformatted سطرها\n\
"));
      oputs (_("\
  -s, --تقسیم-only\n\
         تقسیم دراز سطرها, but افعل not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation از اولی سطر different from دومی\n\
"));
      oputs (_("\
  -u, --یکنواخت-spacing\n\
         one space between واژگان, two after sentences\n\
"));
      oputs (_("\
  -w, --عرض=عرض_2\n\
         بیشینه سطر عرض (افتراضي از 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=عرض_2\n\
         goal عرض (افتراضي از 93% از عرض)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* Decode options and launch execution.  */

ساكن بنية option ثابت long_options[] =
{
  {"crown-margin", no_آرگومان, عدم, 'c'},
  {"prefix", required_آرگومان, عدم, 'p'},
  {"split-only", no_آرگومان, عدم, 's'},
  {"tagged-paragraph", no_آرگومان, عدم, 't'},
  {"uniform-spacing", no_آرگومان, عدم, 'u'},
  {"width", required_آرگومان, عدم, 'w'},
  {"goal", required_آرگومان, عدم, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح optchar;
  bool ok = true;
  حرف ثابت *max_عرض_option = عدم;
  حرف ثابت *goal_عرض_option = عدم;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  إذا (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_عرض_option = argv[1] + 1;

      /* Make the option we just parsed invisible to getopt.  */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  طالما ((optchar = getopt_long (argc, argv, "0123456789cstuw:p:g:",
                                 long_options, عدم))
         != -1)
    بدل (optchar)
      {
      افتراضي:
        إذا (c_isdigit (optchar))
          error (0, 0, _("invalid option -- %c; -عرض_2 is recognized\
 only when it is the اولی\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      حالة 'c':
        crown = true;
        اكسر;

      حالة 's':
        تقسیم = true;
        اكسر;

      حالة 't':
        tagged = true;
        اكسر;

      حالة 'u':
        یکنواخت = true;
        اكسر;

      حالة 'w':
        max_عرض_option = optarg;
        اكسر;

      حالة 'g':
        goal_عرض_option = optarg;
        اكسر;

      حالة 'p':
        مجموعه_پیشوند (optarg);
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

      }

  إذا (max_عرض_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_عرض = xnumtoumax (max_عرض_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  إذا (goal_عرض_option)
    {
      /* Limit goal_width to max_width.  */
      goal_عرض = xdectoumax (goal_عرض_option, 0, max_عرض, "",
                               _("invalid goal"), 0);
      إذا (max_عرض_option == عدم)
        max_عرض = goal_عرض + 10;
    }
  وإلا
    {
      goal_عرض = max_عرض * (2 * (100 - LEEWAY) + 1) / 200;
    }

  bool have_read_stdin = false;

  إذا (optind == argc)
    {
      have_read_stdin = true;
      ok = fmt (stdin, "-");
    }
  وإلا
    {
      لكل (; optind < argc; optind++)
        {
          حرف *file = argv[optind];
          إذا (streq (file, "-"))
            {
              ok &= fmt (stdin, file);
              have_read_stdin = true;
            }
          وإلا
            {
              FILE *درون_جریان;
              درون_جریان = fopen (file, "r");
              إذا (درون_جریان != عدم)
                ok &= fmt (درون_جریان, file);
              وإلا
                {
                  error (0, errno, _("cannot open %s for reading"),
                         quoteaf (file));
                  ok = false;
                }
            }
        }
    }

  إذا (have_read_stdin && fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s", _("closing standard input"));

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Trim space from the front and back of the string P, yielding the prefix,
   and record the lengths of the prefix and the space trimmed.  */

ساكن فراغ
مجموعه_پیشوند (حرف *p)
{
  حرف *s;

  پیشوند_lead_space = 0;
  طالما (*p == ' ')
    {
      پیشوند_lead_space++;
      p++;
    }
  پیشوند = p;
  پیشوند_پر_درازی = طول_رشته (p);
  s = p + پیشوند_پر_درازی;
  طالما (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  پیشوند_درازی = s - p;
}

/* Read F and send formatted output to stdout.
   Close F when done, unless F is stdin.  Diagnose input errors, using FILE.
   If !F, assume F resulted from an fopen failure and diagnose that.
   Return true if successful.  */

ساكن bool
fmt (FILE *f, حرف ثابت *file)
{
  fadvise (f, FADVISE_SEQUENTIAL);
  tabs = false;
  other_indent = 0;
  بعدی_char = get_پیشوند (f);
  طالما (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (کلمه_حد);
    }

  صحيح err = ferror (f) ? 0 : -1;
  /* err < 0 means success.  */
  إذا (f == stdin)
    clearerr (f);
  وإلا إذا (fclose (f) != 0 && err < 0)
    err = errno;
  إذا (0 <= err)
    {
      إذا (f == stdin)
        error (0, errno, _("read error"));
      وإلا
        error (0, errno, _("error reading %s"), quoteaf (file));
    }
  أرجع err < 0;
}

/* Set the global variable 'other_indent' according to SAME_PARAGRAPH
   and other global variables.  */

ساكن فراغ
مجموعه_other_indent (bool same_paragraph)
{
  إذا (تقسیم)
    other_indent = اولی_indent;
  وإلا إذا (crown)
    {
      other_indent = (same_paragraph ? درون_ستونی : اولی_indent);
    }
  وإلا إذا (tagged)
    {
      إذا (same_paragraph && درون_ستونی != اولی_indent)
        {
          other_indent = درون_ستونی;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      وإلا إذا (other_indent == اولی_indent)
        other_indent = اولی_indent == 0 ? DEF_INDENT : 0;
    }
  وإلا
    {
      other_indent = اولی_indent;
    }
}

/* Read a paragraph from input file F.  A paragraph consists of a
   maximal number of non-blank (excluding any prefix) lines subject to:
   * In split mode, a paragraph is a single non-blank line.
   * In crown mode, the second and subsequent lines must have the
   same indentation, but possibly different from the indent of the
   first line.
   * Tagged mode is similar, but the first and second lines must have
   different indentations.
   * Otherwise, all lines of a paragraph must have the same indent.
   If a prefix is in effect, it must be present at the same indent for
   each line in the paragraph.

   Return false if end-of-file was encountered before the start of a
   paragraph, else true.  */

ساكن bool
get_paragraph (FILE *f)
{
  صحيح c;

  آخرین_سطر_درازی = 0;
  c = بعدی_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  طالما (c == '\n' || c == EOF
         || بعدی_پیشوند_indent < پیشوند_lead_space
         || درون_ستونی < بعدی_پیشوند_indent + پیشوند_پر_درازی)
    {
      c = copy_rest (f, c);
      إذا (c == EOF)
        {
          بعدی_char = EOF;
          أرجع false;
        }
      حرف_چاپ_کن ('\n');
      c = get_پیشوند (f);
    }

  /* Got a suitable first line for a paragraph.  */

  پیشوند_indent = بعدی_پیشوند_indent;
  اولی_indent = درون_ستونی;
  wptr = parabuf;
  کلمه_حد = کلمه_2;
  c = get_سطر (f, c);
  مجموعه_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  إذا (تقسیم)
    {
      /*خالی*/
    }
  وإلا إذا (crown)
    {
      إذا (same_para (c))
        {
          افعل
            {			/* for each line till the end of the para */
              c = get_سطر (f, c);
            }
          طالما (same_para (c) && درون_ستونی == other_indent);
        }
    }
  وإلا إذا (tagged)
    {
      إذا (same_para (c) && درون_ستونی != اولی_indent)
        {
          افعل
            {			/* for each line till the end of the para */
              c = get_سطر (f, c);
            }
          طالما (same_para (c) && درون_ستونی == other_indent);
        }
    }
  وإلا
    {
      طالما (same_para (c) && درون_ستونی == other_indent)
        c = get_سطر (f, c);
    }

  (کلمه_حد - 1)->period = (کلمه_حد - 1)->final = true;
  بعدی_char = c;
  أرجع true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

ساكن صحيح
copy_rest (FILE *f, صحيح c)
{
  بیرون_ستونی = 0;
  إذا (درون_ستونی > بعدی_پیشوند_indent || (c != '\n' && c != EOF))
    {
      put_space (بعدی_پیشوند_indent);
      لكل (حرف ثابت *s = پیشوند; بیرون_ستونی != درون_ستونی && *s; بیرون_ستونی++)
        حرف_چاپ_کن (*s++);
      إذا (c != EOF && c != '\n')
        put_space (درون_ستونی - بیرون_ستونی);
      إذا (c == EOF && درون_ستونی >= بعدی_پیشوند_indent + پیشوند_درازی)
        حرف_چاپ_کن ('\n');
    }
  طالما (c != '\n' && c != EOF)
    {
      حرف_چاپ_کن (c);
      c = getc (f);
    }
  أرجع c;
}

/* Return true if a line whose first non-blank character after the
   prefix (if any) is C could belong to the current paragraph,
   otherwise false.  */

ساكن bool
same_para (صحيح c)
{
  أرجع (بعدی_پیشوند_indent == پیشوند_indent
          && درون_ستونی >= بعدی_پیشوند_indent + پیشوند_پر_درازی
          && c != '\n' && c != EOF);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

ساكن صحيح
get_سطر (FILE *f, صحيح c)
{
  صحيح شروع;
  حرف *انتها_از_parabuf;
  کلمه *انتها_از_کلمه;

  انتها_از_parabuf = &parabuf[MAXCHARS];
  انتها_از_کلمه = &کلمه_2[MAXWORDS - 2];

  افعل
    {				/* for each word in a line */

      /* Scan word.  */

      کلمه_حد->متن = wptr;
      افعل
        {
          إذا (wptr == انتها_از_parabuf)
            {
              مجموعه_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      طالما (c != EOF && !c_isspace (c));
      درون_ستونی += کلمه_حد->درازی = wptr - کلمه_حد->متن;
      check_punctuation (کلمه_حد);

      /* Scan inter-word space.  */

      شروع = درون_ستونی;
      c = get_space (f, c);
      کلمه_حد->space = درون_ستونی - شروع;
      کلمه_حد->final = (c == EOF
                           || (کلمه_حد->period
                               && (c == '\n' || کلمه_حد->space > 1)));
      إذا (c == '\n' || c == EOF || یکنواخت)
        کلمه_حد->space = کلمه_حد->final ? 2 : 1;
      إذا (کلمه_حد == انتها_از_کلمه)
        {
          مجموعه_other_indent (true);
          flush_paragraph ();
        }
      کلمه_حد++;
    }
  طالما (c != '\n' && c != EOF);
  أرجع get_پیشوند (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

ساكن صحيح
get_پیشوند (FILE *f)
{
  صحيح c;

  درون_ستونی = 0;
  c = get_space (f, getc (f));
  إذا (پیشوند_درازی == 0)
    بعدی_پیشوند_indent = پیشوند_lead_space < درون_ستونی ?
      پیشوند_lead_space : درون_ستونی;
  وإلا
    {
      بعدی_پیشوند_indent = درون_ستونی;
      لكل (حرف ثابت *p = پیشوند; *p != '\0'; p++)
        {
          بیعلامت حرف pc = *p;
          إذا (c != pc)
            أرجع c;
          درون_ستونی++;
          c = getc (f);
        }
      c = get_space (f, c);
    }
  أرجع c;
}

/* Read blank characters from input file F, starting with C, and keeping
   in_column up-to-date.  Return first non-blank character.  */

ساكن صحيح
get_space (FILE *f, صحيح c)
{
  طالما (true)
    {
      إذا (c == ' ')
        درون_ستونی++;
      وإلا إذا (c == '\t')
        {
          tabs = true;
          درون_ستونی = (درون_ستونی / TABWIDTH + 1) * TABWIDTH;
        }
      وإلا
        أرجع c;
      c = getc (f);
    }
}

/* Set extra fields in word W describing any attached punctuation.  */

ساكن فراغ
check_punctuation (کلمه *w)
{
  حرف ثابت *شروع = w->متن;
  حرف ثابت *پایان = شروع + (w->درازی - 1);
  بیعلامت حرف fin = *پایان;

  w->paren = isopen (*شروع);
  w->punct = !! ispunct (fin);
  طالما (شروع < پایان && isclose (*پایان))
    پایان--;
  w->period = isperiod (*پایان);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

ساكن فراغ
flush_paragraph (فراغ)
{
  کلمه *تقسیم_نقطه;
  کلمه *w;
  صحيح لغزش;
  هزینه بهترین_break;

  /* In the special case where it's all one word, just flush it.  */

  إذا (کلمه_حد == کلمه_2)
    {
      size_t مقصد_write = wptr - parabuf;
      إذا (fwrite (parabuf, 1, مقصد_write, stdout) != مقصد_write)
        write_error ();

      wptr = parabuf;
      أرجع;
    }

  /* Otherwise:
     - format what you have so far as a paragraph,
     - find a low-cost line break near the end,
     - output to there,
     - make that the start of the paragraph.  */

  fmt_paragraph ();

  /* Choose a good split point.  */

  تقسیم_نقطه = کلمه_حد;
  بهترین_break = MAXCOST;
  لكل (w = کلمه_2->بعدی_break; w != کلمه_حد; w = w->بعدی_break)
    {
      إذا (w->بهترین_هزینه - w->بعدی_break->بهترین_هزینه < بهترین_break)
        {
          تقسیم_نقطه = w;
          بهترین_break = w->بهترین_هزینه - w->بعدی_break->بهترین_هزینه;
        }
      إذا (بهترین_break <= MAXCOST - سطر_CREDIT)
        بهترین_break += سطر_CREDIT;
    }
  put_paragraph (تقسیم_نقطه);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, تقسیم_نقطه->متن, wptr - تقسیم_نقطه->متن);
  لغزش = تقسیم_نقطه->متن - parabuf;
  wptr -= لغزش;

  /* Adjust text pointers.  */

  لكل (w = تقسیم_نقطه; w <= کلمه_حد; w++)
    w->متن -= لغزش;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (کلمه_2, تقسیم_نقطه, (کلمه_حد - تقسیم_نقطه + 1) * حجم *کلمه_2);
  کلمه_حد -= تقسیم_نقطه - کلمه_2;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

ساكن فراغ
fmt_paragraph (فراغ)
{
  کلمه *w;
  صحيح len;
  هزینه wcost, بهترین;
  صحيح saved_درازی;

  کلمه_حد->بهترین_هزینه = 0;
  saved_درازی = کلمه_حد->درازی;
  کلمه_حد->درازی = max_عرض;	/* sentinel */

  لكل (کلمه *شروع = کلمه_حد - 1; شروع >= کلمه_2; شروع--)
    {
      بهترین = MAXCOST;
      len = شروع == کلمه_2 ? اولی_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = شروع;
      len += w->درازی;
      افعل
        {
          w++;

          /* Consider breaking before w.  */

          wcost = سطر_هزینه_2 (w, len) + w->بهترین_هزینه;
          إذا (شروع == کلمه_2 && آخرین_سطر_درازی > 0)
            wcost += RAGGED_هزینه (len - آخرین_سطر_درازی);
          إذا (wcost < بهترین)
            {
              بهترین = wcost;
              شروع->بعدی_break = w;
              شروع->سطر_درازی = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          إذا (w == کلمه_حد)
            اكسر;

          len += (w - 1)->space + w->درازی;	/* w > start >= word */
        }
      طالما (len <= max_عرض);
      شروع->بهترین_هزینه = بهترین + پایه_هزینه (شروع);
    }

  کلمه_حد->درازی = saved_درازی;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#إذا __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#پایان_اگر

/* Return the constant component of the cost of breaking before the
   word THIS.  */

ساكن هزینه
پایه_هزینه (کلمه *this)
{
  هزینه هزینه_2;

  هزینه_2 = سطر_هزینه;

  إذا (this > کلمه_2)
    {
      إذا ((this - 1)->period)
        {
          إذا ((this - 1)->final)
            هزینه_2 -= SENTENCE_BONUS;
          وإلا
            هزینه_2 += NOBREAK_هزینه;
        }
      وإلا إذا ((this - 1)->punct)
        هزینه_2 -= PUNCT_BONUS;
      وإلا إذا (this > کلمه_2 + 1 && (this - 2)->final)
        هزینه_2 += WIDOW_هزینه ((this - 1)->درازی);
    }

  إذا (this->paren)
    هزینه_2 -= PAREN_BONUS;
  وإلا إذا (this->final)
    هزینه_2 += ORPHAN_هزینه (this->درازی);

  أرجع هزینه_2;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

ساكن هزینه
سطر_هزینه_2 (کلمه *بعدی, صحيح len)
{
  صحيح n;
  هزینه هزینه_2;

  إذا (بعدی == کلمه_حد)
    أرجع 0;
  n = goal_عرض - len;
  هزینه_2 = SHORT_هزینه (n);
  إذا (بعدی->بعدی_break != کلمه_حد)
    {
      n = len - بعدی->سطر_درازی;
      هزینه_2 += RAGGED_هزینه (n);
    }
  أرجع هزینه_2;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

ساكن فراغ
put_paragraph (کلمه *پایان)
{
  put_سطر (کلمه_2, اولی_indent);
  لكل (کلمه *w = کلمه_2->بعدی_break; w != پایان; w = w->بعدی_break)
    put_سطر (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

ساكن فراغ
put_سطر (کلمه *w, صحيح indent)
{
  کلمه *endline;

  بیرون_ستونی = 0;
  put_space (پیشوند_indent);
  fputs (پیشوند, stdout);
  بیرون_ستونی += پیشوند_درازی;
  put_space (indent - بیرون_ستونی);

  endline = w->بعدی_break - 1;
  لكل (; w != endline; w++)
    {
      put_کلمه (w);
      put_space (w->space);
    }
  put_کلمه (w);
  آخرین_سطر_درازی = بیرون_ستونی;
  حرف_چاپ_کن ('\n');

  إذا (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

ساكن فراغ
put_کلمه (کلمه *w)
{
  حرف ثابت *s = w->متن;
  لكل (صحيح n = w->درازی; n != 0; n--)
    حرف_چاپ_کن (*s++);
  بیرون_ستونی += w->درازی;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

ساكن فراغ
put_space (صحيح space)
{
  صحيح space_هدف, tab_هدف;

  space_هدف = بیرون_ستونی + space;
  إذا (tabs)
    {
      tab_هدف = space_هدف / TABWIDTH * TABWIDTH;
      إذا (بیرون_ستونی + 1 < tab_هدف)
        طالما (بیرون_ستونی < tab_هدف)
          {
            حرف_چاپ_کن ('\t');
            بیرون_ستونی = (بیرون_ستونی / TABWIDTH + 1) * TABWIDTH;
          }
    }
  طالما (بیرون_ستونی < space_هدف)
    {
      حرف_چاپ_کن (' ');
      بیرون_ستونی++;
    }
}
