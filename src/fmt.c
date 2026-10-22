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
#تعریف_کرو لفظ_2 بن_ورتیا_لفظ_type

#ضمن "c-ctype.h"
#ضمن "system.h"
#ضمن "fadvise.h"
#ضمن "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "fmt"

#تعریف_کرو AUTHORS proper_name ("Ross Paterson")

/* The following parameters represent the program's idea of what is
   "best".  Adjust to taste, subject to the caveats given.  */

/* Default longest permitted line length (max_width).  */
#تعریف_کرو چوڑائی_2	75

/* Prefer lines to be LEEWAY % shorter than the maximum width, giving
   room for optimization.  */
#تعریف_کرو LEEWAY	7

/* The default secondary indent of tagged paragraph used for unindented
   one-line paragraphs not preceded by any multi-line paragraphs.  */
#تعریف_کرو DEF_INDENT 3

/* Costs and bonuses are expressed as the equivalent departure from the
   optimal line length, multiplied by 10.  e.g. assigning something a
   cost of 50 means that it is as bad as a line 5 characters too short
   or too long.  The definition of SHORT_COST(n) should not be changed.
   However, EQUIV(n) may need tuning.  */

/* FIXME: "fmt" misbehaves given large inputs or options.  One
   possible workaround for part of the problem is to change COST to be
   a floating-point type.  There are other problems besides COST,
   though; see MAXWORDS below.  */

تعريفنوع لما صحيح لاگت;

#تعریف_کرو MAXCOST	TYPE_ودھ_توں_ودھ (لاگت)

#تعریف_کرو SQR(n)		((n) * (n))
#تعریف_کرو EQUIV(n)	SQR ((لاگت) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#تعریف_کرو SHORT_لاگت(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#تعریف_کرو RAGGED_لاگت(n)	(SHORT_لاگت (n) / 2)

/* Basic cost per line.  */
#تعریف_کرو سطر_لاگت	EQUIV (70)

/* Cost of breaking a line after the first word of a sentence, where
   the length of the word is N.  */
#تعریف_کرو WIDOW_لاگت(n)	(EQUIV (200) / ((n) + 2))

/* Cost of breaking a line before the last word of a sentence, where
   the length of the word is N.  */
#تعریف_کرو ORPHAN_لاگت(n)	(EQUIV (150) / ((n) + 2))

/* Bonus for breaking a line at the end of a sentence.  */
#تعریف_کرو SENTENCE_BONUS	EQUIV (50)

/* Cost of breaking a line after a period not marking end of a sentence.
   With the definition of sentence we are using (borrowed from emacs, see
   get_line()) such a break would then look like a sentence break.  Hence
   we assign a very high cost -- it should be avoided unless things are
   really bad.  */
#تعریف_کرو NOBREAK_لاگت	EQUIV (600)

/* Bonus for breaking a line before open parenthesis.  */
#تعریف_کرو PAREN_BONUS	EQUIV (40)

/* Bonus for breaking a line after other punctuation.  */
#تعریف_کرو PUNCT_BONUS	EQUIV(40)

/* Credit for breaking a long paragraph one line later.  */
#تعریف_کرو سطر_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#تعریف_کرو MAXWORDS	1000
#تعریف_کرو MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#تعریف_کرو isopen(c)	(strchr ("(['`\"", c) != عدم)
#تعریف_کرو isclose(c)	(strchr (")]'\"", c) != عدم)
#تعریف_کرو isperiod(c)	(strchr (".?!", c) != عدم)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#تعریف_کرو TABWIDTH	8

/* Word descriptor structure.  */

تعريفنوع بنية لفظ_3 لفظ;

بنية لفظ_3
  {

    /* Static attributes determined during input.  */

    حرف ثابت *متن;		/* the text of the word */
    صحيح لمائی;			/* length of this word */
    صحيح space;			/* the size of the following space */
    بنانشانی صحيح paren:1;	/* starts with open paren */
    بنانشانی صحيح period:1;	/* ends in [.?!])* */
    بنانشانی صحيح punct:1;	/* ends in punctuation */
    بنانشانی صحيح final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    صحيح سطر_لمائی;		/* length of the best line starting here */
    لاگت بہترین_لاگت;		/* cost of best paragraph starting here */
    لفظ *اگلا_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

ساكن فراغ سیٹ_اگیتر (حرف *p);
ساكن bool fmt (FILE *f, حرف ثابت *);
ساكن bool get_paragraph (FILE *f);
ساكن صحيح get_سطر (FILE *f, صحيح c);
ساكن صحيح get_اگیتر (FILE *f);
ساكن صحيح get_space (FILE *f, صحيح c);
ساكن صحيح copy_rest (FILE *f, صحيح c);
ساكن bool same_para (صحيح c);
ساكن فراغ flush_paragraph (فراغ);
ساكن فراغ fmt_paragraph (فراغ);
ساكن فراغ check_punctuation (لفظ *w);
ساكن لاگت ادھار_لاگت (لفظ *this);
ساكن لاگت سطر_لاگت_2 (لفظ *اگلا, صحيح len);
ساكن فراغ put_paragraph (لفظ *انت);
ساكن فراغ put_سطر (لفظ *w, صحيح indent);
ساكن فراغ put_لفظ (لفظ *w);
ساكن فراغ put_space (صحيح space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
ساكن bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
ساكن bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
ساكن bool ونڈو;

/* If true, don't preserve inter-word spacing (default false).  */
ساكن bool اکسار;

/* Prefix minus leading and trailing spaces (default "").  */
ساكن حرف ثابت *اگیتر = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
ساكن صحيح max_چوڑائی = چوڑائی_2;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
ساكن صحيح اگیتر_بھریا_لمائی;

/* The length of the leading space trimmed from the prefix.  */
ساكن صحيح اگیتر_lead_space;

/* The length of prefix minus leading and trailing space.  */
ساكن صحيح اگیتر_لمائی;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
ساكن صحيح goal_چوڑائی;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
ساكن صحيح وچ_کالم;

/* Start column of the next character to be written to stdout.  */
ساكن صحيح باہرلا_کالم;

/* Space for the paragraph text -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ساكن حرف parabuf[MAXCHARS];

/* A pointer into parabuf, indicating the first unused character position.  */
ساكن حرف *wptr;

/* The words of a paragraph -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ساكن لفظ لفظ_2[MAXWORDS];

/* A pointer into the above word array, indicating the first position
   after the last complete word.  Sometimes it will point at an incomplete
   word.  */
ساكن لفظ *لفظ_حد_بندی;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
ساكن bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
ساكن صحيح اگیتر_indent;

/* Indentation of the first line of the current paragraph.  */
ساكن صحيح پہلا_indent;

/* Indentation of other lines of the current paragraph */
ساكن صحيح other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
ساكن صحيح اگلا_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
ساكن صحيح اگلا_اگیتر_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
ساكن صحيح آخری_سطر_لمائی;

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [-WIDTH] [OPTION]... [FILE]...\n"), پروگرام_name);
      fputs (_("\
Reformat each paragraph وچ the FILE(s), writing منزل standard output.\n\
The option -چوڑائی_2 is an abbreviated form دا --چوڑائی=DIGITS.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --crown-margin\n\
         preserve indentation دا پہلا two سطریں\n\
"));
      oputs (_("\
  -p, --اگیتر=STRING\n\
         reformat only سطریں beginning with STRING,\n\
         reattaching the اگیتر منزل reformatted سطریں\n\
"));
      oputs (_("\
  -s, --ونڈو-only\n\
         ونڈو لما سطریں, but افعل not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation دا پہلا سطر different from دوجا\n\
"));
      oputs (_("\
  -u, --اکسار-spacing\n\
         one space between الفاظ, two after sentences\n\
"));
      oputs (_("\
  -w, --چوڑائی=چوڑائی_2\n\
         ودھ_توں_ودھ سطر چوڑائی (افتراضي دا 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=چوڑائی_2\n\
         goal چوڑائی (افتراضي دا 93% دا چوڑائی)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Decode options and launch execution.  */

ساكن بنية option ثابت long_options[] =
{
  {"crown-margin", no_دلیل, عدم, 'c'},
  {"prefix", required_دلیل, عدم, 'p'},
  {"split-only", no_دلیل, عدم, 's'},
  {"tagged-paragraph", no_دلیل, عدم, 't'},
  {"uniform-spacing", no_دلیل, عدم, 'u'},
  {"width", required_دلیل, عدم, 'w'},
  {"goal", required_دلیل, عدم, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

صحيح
مکھ (صحيح argc, حرف **argv)
{
  صحيح optchar;
  bool ok = true;
  حرف ثابت *max_چوڑائی_option = عدم;
  حرف ثابت *goal_چوڑائی_option = عدم;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  إذا (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_چوڑائی_option = argv[1] + 1;

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
          error (0, 0, _("invalid option -- %c; -چوڑائی_2 is recognized\
 only when it is the پہلا\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      حالة 'c':
        crown = true;
        اكسر;

      حالة 's':
        ونڈو = true;
        اكسر;

      حالة 't':
        tagged = true;
        اكسر;

      حالة 'u':
        اکسار = true;
        اكسر;

      حالة 'w':
        max_چوڑائی_option = optarg;
        اكسر;

      حالة 'g':
        goal_چوڑائی_option = optarg;
        اكسر;

      حالة 'p':
        سیٹ_اگیتر (optarg);
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

      }

  إذا (max_چوڑائی_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_چوڑائی = xnumtoumax (max_چوڑائی_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  إذا (goal_چوڑائی_option)
    {
      /* Limit goal_width to max_width.  */
      goal_چوڑائی = xdectoumax (goal_چوڑائی_option, 0, max_چوڑائی, "",
                               _("invalid goal"), 0);
      إذا (max_چوڑائی_option == عدم)
        max_چوڑائی = goal_چوڑائی + 10;
    }
  وإلا
    {
      goal_چوڑائی = max_چوڑائی * (2 * (100 - LEEWAY) + 1) / 200;
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
              FILE *وچ_دھارا;
              وچ_دھارا = fopen (file, "r");
              إذا (وچ_دھارا != عدم)
                ok &= fmt (وچ_دھارا, file);
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
سیٹ_اگیتر (حرف *p)
{
  حرف *s;

  اگیتر_lead_space = 0;
  طالما (*p == ' ')
    {
      اگیتر_lead_space++;
      p++;
    }
  اگیتر = p;
  اگیتر_بھریا_لمائی = لڑی_لمبائی (p);
  s = p + اگیتر_بھریا_لمائی;
  طالما (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  اگیتر_لمائی = s - p;
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
  اگلا_char = get_اگیتر (f);
  طالما (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (لفظ_حد_بندی);
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
سیٹ_other_indent (bool same_paragraph)
{
  إذا (ونڈو)
    other_indent = پہلا_indent;
  وإلا إذا (crown)
    {
      other_indent = (same_paragraph ? وچ_کالم : پہلا_indent);
    }
  وإلا إذا (tagged)
    {
      إذا (same_paragraph && وچ_کالم != پہلا_indent)
        {
          other_indent = وچ_کالم;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      وإلا إذا (other_indent == پہلا_indent)
        other_indent = پہلا_indent == 0 ? DEF_INDENT : 0;
    }
  وإلا
    {
      other_indent = پہلا_indent;
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

  آخری_سطر_لمائی = 0;
  c = اگلا_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  طالما (c == '\n' || c == EOF
         || اگلا_اگیتر_indent < اگیتر_lead_space
         || وچ_کالم < اگلا_اگیتر_indent + اگیتر_بھریا_لمائی)
    {
      c = copy_rest (f, c);
      إذا (c == EOF)
        {
          اگلا_char = EOF;
          أرجع false;
        }
      حرف_لکھو ('\n');
      c = get_اگیتر (f);
    }

  /* Got a suitable first line for a paragraph.  */

  اگیتر_indent = اگلا_اگیتر_indent;
  پہلا_indent = وچ_کالم;
  wptr = parabuf;
  لفظ_حد_بندی = لفظ_2;
  c = get_سطر (f, c);
  سیٹ_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  إذا (ونڈو)
    {
      /*خالی_تھاواں*/
    }
  وإلا إذا (crown)
    {
      إذا (same_para (c))
        {
          افعل
            {			/* for each line till the end of the para */
              c = get_سطر (f, c);
            }
          طالما (same_para (c) && وچ_کالم == other_indent);
        }
    }
  وإلا إذا (tagged)
    {
      إذا (same_para (c) && وچ_کالم != پہلا_indent)
        {
          افعل
            {			/* for each line till the end of the para */
              c = get_سطر (f, c);
            }
          طالما (same_para (c) && وچ_کالم == other_indent);
        }
    }
  وإلا
    {
      طالما (same_para (c) && وچ_کالم == other_indent)
        c = get_سطر (f, c);
    }

  (لفظ_حد_بندی - 1)->period = (لفظ_حد_بندی - 1)->final = true;
  اگلا_char = c;
  أرجع true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

ساكن صحيح
copy_rest (FILE *f, صحيح c)
{
  باہرلا_کالم = 0;
  إذا (وچ_کالم > اگلا_اگیتر_indent || (c != '\n' && c != EOF))
    {
      put_space (اگلا_اگیتر_indent);
      لكل (حرف ثابت *s = اگیتر; باہرلا_کالم != وچ_کالم && *s; باہرلا_کالم++)
        حرف_لکھو (*s++);
      إذا (c != EOF && c != '\n')
        put_space (وچ_کالم - باہرلا_کالم);
      إذا (c == EOF && وچ_کالم >= اگلا_اگیتر_indent + اگیتر_لمائی)
        حرف_لکھو ('\n');
    }
  طالما (c != '\n' && c != EOF)
    {
      حرف_لکھو (c);
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
  أرجع (اگلا_اگیتر_indent == اگیتر_indent
          && وچ_کالم >= اگلا_اگیتر_indent + اگیتر_بھریا_لمائی
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
  حرف *سرا_دا_parabuf;
  لفظ *سرا_دا_لفظ;

  سرا_دا_parabuf = &parabuf[MAXCHARS];
  سرا_دا_لفظ = &لفظ_2[MAXWORDS - 2];

  افعل
    {				/* for each word in a line */

      /* Scan word.  */

      لفظ_حد_بندی->متن = wptr;
      افعل
        {
          إذا (wptr == سرا_دا_parabuf)
            {
              سیٹ_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      طالما (c != EOF && !c_isspace (c));
      وچ_کالم += لفظ_حد_بندی->لمائی = wptr - لفظ_حد_بندی->متن;
      check_punctuation (لفظ_حد_بندی);

      /* Scan inter-word space.  */

      شروع = وچ_کالم;
      c = get_space (f, c);
      لفظ_حد_بندی->space = وچ_کالم - شروع;
      لفظ_حد_بندی->final = (c == EOF
                           || (لفظ_حد_بندی->period
                               && (c == '\n' || لفظ_حد_بندی->space > 1)));
      إذا (c == '\n' || c == EOF || اکسار)
        لفظ_حد_بندی->space = لفظ_حد_بندی->final ? 2 : 1;
      إذا (لفظ_حد_بندی == سرا_دا_لفظ)
        {
          سیٹ_other_indent (true);
          flush_paragraph ();
        }
      لفظ_حد_بندی++;
    }
  طالما (c != '\n' && c != EOF);
  أرجع get_اگیتر (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

ساكن صحيح
get_اگیتر (FILE *f)
{
  صحيح c;

  وچ_کالم = 0;
  c = get_space (f, getc (f));
  إذا (اگیتر_لمائی == 0)
    اگلا_اگیتر_indent = اگیتر_lead_space < وچ_کالم ?
      اگیتر_lead_space : وچ_کالم;
  وإلا
    {
      اگلا_اگیتر_indent = وچ_کالم;
      لكل (حرف ثابت *p = اگیتر; *p != '\0'; p++)
        {
          بنانشانی حرف pc = *p;
          إذا (c != pc)
            أرجع c;
          وچ_کالم++;
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
        وچ_کالم++;
      وإلا إذا (c == '\t')
        {
          tabs = true;
          وچ_کالم = (وچ_کالم / TABWIDTH + 1) * TABWIDTH;
        }
      وإلا
        أرجع c;
      c = getc (f);
    }
}

/* Set extra fields in word W describing any attached punctuation.  */

ساكن فراغ
check_punctuation (لفظ *w)
{
  حرف ثابت *شروع = w->متن;
  حرف ثابت *انت = شروع + (w->لمائی - 1);
  بنانشانی حرف fin = *انت;

  w->paren = isopen (*شروع);
  w->punct = !! ispunct (fin);
  طالما (شروع < انت && isclose (*انت))
    انت--;
  w->period = isperiod (*انت);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

ساكن فراغ
flush_paragraph (فراغ)
{
  لفظ *ونڈو_نقطہ;
  لفظ *w;
  صحيح کھسکاؤ;
  لاگت بہترین_break;

  /* In the special case where it's all one word, just flush it.  */

  إذا (لفظ_حد_بندی == لفظ_2)
    {
      size_t منزل_write = wptr - parabuf;
      إذا (fwrite (parabuf, 1, منزل_write, stdout) != منزل_write)
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

  ونڈو_نقطہ = لفظ_حد_بندی;
  بہترین_break = MAXCOST;
  لكل (w = لفظ_2->اگلا_break; w != لفظ_حد_بندی; w = w->اگلا_break)
    {
      إذا (w->بہترین_لاگت - w->اگلا_break->بہترین_لاگت < بہترین_break)
        {
          ونڈو_نقطہ = w;
          بہترین_break = w->بہترین_لاگت - w->اگلا_break->بہترین_لاگت;
        }
      إذا (بہترین_break <= MAXCOST - سطر_CREDIT)
        بہترین_break += سطر_CREDIT;
    }
  put_paragraph (ونڈو_نقطہ);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, ونڈو_نقطہ->متن, wptr - ونڈو_نقطہ->متن);
  کھسکاؤ = ونڈو_نقطہ->متن - parabuf;
  wptr -= کھسکاؤ;

  /* Adjust text pointers.  */

  لكل (w = ونڈو_نقطہ; w <= لفظ_حد_بندی; w++)
    w->متن -= کھسکاؤ;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (لفظ_2, ونڈو_نقطہ, (لفظ_حد_بندی - ونڈو_نقطہ + 1) * حجم *لفظ_2);
  لفظ_حد_بندی -= ونڈو_نقطہ - لفظ_2;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

ساكن فراغ
fmt_paragraph (فراغ)
{
  لفظ *w;
  صحيح len;
  لاگت wcost, بہترین;
  صحيح saved_لمائی;

  لفظ_حد_بندی->بہترین_لاگت = 0;
  saved_لمائی = لفظ_حد_بندی->لمائی;
  لفظ_حد_بندی->لمائی = max_چوڑائی;	/* sentinel */

  لكل (لفظ *شروع = لفظ_حد_بندی - 1; شروع >= لفظ_2; شروع--)
    {
      بہترین = MAXCOST;
      len = شروع == لفظ_2 ? پہلا_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = شروع;
      len += w->لمائی;
      افعل
        {
          w++;

          /* Consider breaking before w.  */

          wcost = سطر_لاگت_2 (w, len) + w->بہترین_لاگت;
          إذا (شروع == لفظ_2 && آخری_سطر_لمائی > 0)
            wcost += RAGGED_لاگت (len - آخری_سطر_لمائی);
          إذا (wcost < بہترین)
            {
              بہترین = wcost;
              شروع->اگلا_break = w;
              شروع->سطر_لمائی = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          إذا (w == لفظ_حد_بندی)
            اكسر;

          len += (w - 1)->space + w->لمائی;	/* w > start >= word */
        }
      طالما (len <= max_چوڑائی);
      شروع->بہترین_لاگت = بہترین + ادھار_لاگت (شروع);
    }

  لفظ_حد_بندی->لمائی = saved_لمائی;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#إذا __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#جے_انت

/* Return the constant component of the cost of breaking before the
   word THIS.  */

ساكن لاگت
ادھار_لاگت (لفظ *this)
{
  لاگت لاگت_2;

  لاگت_2 = سطر_لاگت;

  إذا (this > لفظ_2)
    {
      إذا ((this - 1)->period)
        {
          إذا ((this - 1)->final)
            لاگت_2 -= SENTENCE_BONUS;
          وإلا
            لاگت_2 += NOBREAK_لاگت;
        }
      وإلا إذا ((this - 1)->punct)
        لاگت_2 -= PUNCT_BONUS;
      وإلا إذا (this > لفظ_2 + 1 && (this - 2)->final)
        لاگت_2 += WIDOW_لاگت ((this - 1)->لمائی);
    }

  إذا (this->paren)
    لاگت_2 -= PAREN_BONUS;
  وإلا إذا (this->final)
    لاگت_2 += ORPHAN_لاگت (this->لمائی);

  أرجع لاگت_2;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

ساكن لاگت
سطر_لاگت_2 (لفظ *اگلا, صحيح len)
{
  صحيح n;
  لاگت لاگت_2;

  إذا (اگلا == لفظ_حد_بندی)
    أرجع 0;
  n = goal_چوڑائی - len;
  لاگت_2 = SHORT_لاگت (n);
  إذا (اگلا->اگلا_break != لفظ_حد_بندی)
    {
      n = len - اگلا->سطر_لمائی;
      لاگت_2 += RAGGED_لاگت (n);
    }
  أرجع لاگت_2;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

ساكن فراغ
put_paragraph (لفظ *انت)
{
  put_سطر (لفظ_2, پہلا_indent);
  لكل (لفظ *w = لفظ_2->اگلا_break; w != انت; w = w->اگلا_break)
    put_سطر (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

ساكن فراغ
put_سطر (لفظ *w, صحيح indent)
{
  لفظ *endline;

  باہرلا_کالم = 0;
  put_space (اگیتر_indent);
  fputs (اگیتر, stdout);
  باہرلا_کالم += اگیتر_لمائی;
  put_space (indent - باہرلا_کالم);

  endline = w->اگلا_break - 1;
  لكل (; w != endline; w++)
    {
      put_لفظ (w);
      put_space (w->space);
    }
  put_لفظ (w);
  آخری_سطر_لمائی = باہرلا_کالم;
  حرف_لکھو ('\n');

  إذا (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

ساكن فراغ
put_لفظ (لفظ *w)
{
  حرف ثابت *s = w->متن;
  لكل (صحيح n = w->لمائی; n != 0; n--)
    حرف_لکھو (*s++);
  باہرلا_کالم += w->لمائی;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

ساكن فراغ
put_space (صحيح space)
{
  صحيح space_نشانہ, tab_نشانہ;

  space_نشانہ = باہرلا_کالم + space;
  إذا (tabs)
    {
      tab_نشانہ = space_نشانہ / TABWIDTH * TABWIDTH;
      إذا (باہرلا_کالم + 1 < tab_نشانہ)
        طالما (باہرلا_کالم < tab_نشانہ)
          {
            حرف_لکھو ('\t');
            باہرلا_کالم = (باہرلا_کالم / TABWIDTH + 1) * TABWIDTH;
          }
    }
  طالما (باہرلا_کالم < space_نشانہ)
    {
      حرف_لکھو (' ');
      باہرلا_کالم++;
    }
}
