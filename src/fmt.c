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
#تعریف_کړه کلمه_2 ناکارول_شوی_کلمه_type

#ضمن "c-ctype.h"
#ضمن "system.h"
#ضمن "fadvise.h"
#ضمن "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کړه پروګرام_NAME "fmt"

#تعریف_کړه AUTHORS proper_name ("Ross Paterson")

/* The following parameters represent the program's idea of what is
   "best".  Adjust to taste, subject to the caveats given.  */

/* Default longest permitted line length (max_width).  */
#تعریف_کړه پلنوالی_2	75

/* Prefer lines to be LEEWAY % shorter than the maximum width, giving
   room for optimization.  */
#تعریف_کړه LEEWAY	7

/* The default secondary indent of tagged paragraph used for unindented
   one-line paragraphs not preceded by any multi-line paragraphs.  */
#تعریف_کړه DEF_INDENT 3

/* Costs and bonuses are expressed as the equivalent departure from the
   optimal line length, multiplied by 10.  e.g. assigning something a
   cost of 50 means that it is as bad as a line 5 characters too short
   or too long.  The definition of SHORT_COST(n) should not be changed.
   However, EQUIV(n) may need tuning.  */

/* FIXME: "fmt" misbehaves given large inputs or options.  One
   possible workaround for part of the problem is to change COST to be
   a floating-point type.  There are other problems besides COST,
   though; see MAXWORDS below.  */

تعريفنوع اوږد صحيح لګښت;

#تعریف_کړه MAXCOST	TYPE_اعظمي (لګښت)

#تعریف_کړه SQR(n)		((n) * (n))
#تعریف_کړه EQUIV(n)	SQR ((لګښت) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#تعریف_کړه SHORT_لګښت(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#تعریف_کړه RAGGED_لګښت(n)	(SHORT_لګښت (n) / 2)

/* Basic cost per line.  */
#تعریف_کړه کرښه_لګښت	EQUIV (70)

/* Cost of breaking a line after the first word of a sentence, where
   the length of the word is N.  */
#تعریف_کړه WIDOW_لګښت(n)	(EQUIV (200) / ((n) + 2))

/* Cost of breaking a line before the last word of a sentence, where
   the length of the word is N.  */
#تعریف_کړه ORPHAN_لګښت(n)	(EQUIV (150) / ((n) + 2))

/* Bonus for breaking a line at the end of a sentence.  */
#تعریف_کړه SENTENCE_BONUS	EQUIV (50)

/* Cost of breaking a line after a period not marking end of a sentence.
   With the definition of sentence we are using (borrowed from emacs, see
   get_line()) such a break would then look like a sentence break.  Hence
   we assign a very high cost -- it should be avoided unless things are
   really bad.  */
#تعریف_کړه NOBREAK_لګښت	EQUIV (600)

/* Bonus for breaking a line before open parenthesis.  */
#تعریف_کړه PAREN_BONUS	EQUIV (40)

/* Bonus for breaking a line after other punctuation.  */
#تعریف_کړه PUNCT_BONUS	EQUIV(40)

/* Credit for breaking a long paragraph one line later.  */
#تعریف_کړه کرښه_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#تعریف_کړه MAXWORDS	1000
#تعریف_کړه MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#تعریف_کړه isopen(c)	(strchr ("(['`\"", c) != عدم)
#تعریف_کړه isclose(c)	(strchr (")]'\"", c) != عدم)
#تعریف_کړه isperiod(c)	(strchr (".?!", c) != عدم)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#تعریف_کړه TABWIDTH	8

/* Word descriptor structure.  */

تعريفنوع بنية کلمه_3 کلمه;

بنية کلمه_3
  {

    /* Static attributes determined during input.  */

    حرف ثابت *متن;		/* the text of the word */
    صحيح اوږدتیا;			/* length of this word */
    صحيح space;			/* the size of the following space */
    بینښه صحيح paren:1;	/* starts with open paren */
    بینښه صحيح period:1;	/* ends in [.?!])* */
    بینښه صحيح punct:1;	/* ends in punctuation */
    بینښه صحيح final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    صحيح کرښه_اوږدتیا;		/* length of the best line starting here */
    لګښت غوره_لګښت;		/* cost of best paragraph starting here */
    کلمه *بل_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

ساكن فراغ سېټ_مختاړی (حرف *p);
ساكن bool fmt (FILE *f, حرف ثابت *);
ساكن bool get_paragraph (FILE *f);
ساكن صحيح get_کرښه (FILE *f, صحيح c);
ساكن صحيح get_مختاړی (FILE *f);
ساكن صحيح get_space (FILE *f, صحيح c);
ساكن صحيح copy_rest (FILE *f, صحيح c);
ساكن bool same_para (صحيح c);
ساكن فراغ flush_paragraph (فراغ);
ساكن فراغ fmt_paragraph (فراغ);
ساكن فراغ check_punctuation (کلمه *w);
ساكن لګښت بنسټ_لګښت (کلمه *this);
ساكن لګښت کرښه_لګښت_2 (کلمه *بل, صحيح len);
ساكن فراغ put_paragraph (کلمه *پای);
ساكن فراغ put_کرښه (کلمه *w, صحيح indent);
ساكن فراغ put_کلمه (کلمه *w);
ساكن فراغ put_space (صحيح space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
ساكن bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
ساكن bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
ساكن bool وېشل;

/* If true, don't preserve inter-word spacing (default false).  */
ساكن bool یوشان;

/* Prefix minus leading and trailing spaces (default "").  */
ساكن حرف ثابت *مختاړی = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
ساكن صحيح max_پلنوالی = پلنوالی_2;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
ساكن صحيح مختاړی_ډک_اوږدتیا;

/* The length of the leading space trimmed from the prefix.  */
ساكن صحيح مختاړی_lead_space;

/* The length of prefix minus leading and trailing space.  */
ساكن صحيح مختاړی_اوږدتیا;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
ساكن صحيح goal_پلنوالی;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
ساكن صحيح دننه_ستنه;

/* Start column of the next character to be written to stdout.  */
ساكن صحيح بهر_ستنه;

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
ساكن صحيح مختاړی_indent;

/* Indentation of the first line of the current paragraph.  */
ساكن صحيح لومړی_indent;

/* Indentation of other lines of the current paragraph */
ساكن صحيح other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
ساكن صحيح بل_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
ساكن صحيح بل_مختاړی_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
ساكن صحيح وروستی_کرښه_اوږدتیا;

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("Usage: %s [-WIDTH] [OPTION]... [FILE]...\n"), پروګرام_name);
      fputs (_("\
Reformat each paragraph دننه the FILE(s), writing منزل standard output.\n\
The option -پلنوالی_2 is an abbreviated form د --پلنوالی=DIGITS.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --crown-margin\n\
         preserve indentation د لومړی two کرښې\n\
"));
      oputs (_("\
  -p, --مختاړی=STRING\n\
         reformat only کرښې beginning with STRING,\n\
         reattaching the مختاړی منزل reformatted کرښې\n\
"));
      oputs (_("\
  -s, --وېشل-only\n\
         وېشل اوږد کرښې, but افعل not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation د لومړی کرښه different from دوهم\n\
"));
      oputs (_("\
  -u, --یوشان-spacing\n\
         one space between ټکي, two after sentences\n\
"));
      oputs (_("\
  -w, --پلنوالی=پلنوالی_2\n\
         اعظمي کرښه پلنوالی (افتراضي د 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=پلنوالی_2\n\
         goal پلنوالی (افتراضي د 93% د پلنوالی)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
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
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

صحيح
اصلي (صحيح argc, حرف **argv)
{
  صحيح optchar;
  bool ok = true;
  حرف ثابت *max_پلنوالی_option = عدم;
  حرف ثابت *goal_پلنوالی_option = عدم;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  إذا (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_پلنوالی_option = argv[1] + 1;

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
          error (0, 0, _("invalid option -- %c; -پلنوالی_2 is recognized\
 only when it is the لومړی\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      حالة 'c':
        crown = true;
        اكسر;

      حالة 's':
        وېشل = true;
        اكسر;

      حالة 't':
        tagged = true;
        اكسر;

      حالة 'u':
        یوشان = true;
        اكسر;

      حالة 'w':
        max_پلنوالی_option = optarg;
        اكسر;

      حالة 'g':
        goal_پلنوالی_option = optarg;
        اكسر;

      حالة 'p':
        سېټ_مختاړی (optarg);
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

      }

  إذا (max_پلنوالی_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_پلنوالی = xnumtoumax (max_پلنوالی_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  إذا (goal_پلنوالی_option)
    {
      /* Limit goal_width to max_width.  */
      goal_پلنوالی = xdectoumax (goal_پلنوالی_option, 0, max_پلنوالی, "",
                               _("invalid goal"), 0);
      إذا (max_پلنوالی_option == عدم)
        max_پلنوالی = goal_پلنوالی + 10;
    }
  وإلا
    {
      goal_پلنوالی = max_پلنوالی * (2 * (100 - LEEWAY) + 1) / 200;
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
              FILE *دننه_بهیر;
              دننه_بهیر = fopen (file, "r");
              إذا (دننه_بهیر != عدم)
                ok &= fmt (دننه_بهیر, file);
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
سېټ_مختاړی (حرف *p)
{
  حرف *s;

  مختاړی_lead_space = 0;
  طالما (*p == ' ')
    {
      مختاړی_lead_space++;
      p++;
    }
  مختاړی = p;
  مختاړی_ډک_اوږدتیا = مزي_اوږدوالی (p);
  s = p + مختاړی_ډک_اوږدتیا;
  طالما (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  مختاړی_اوږدتیا = s - p;
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
  بل_char = get_مختاړی (f);
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
سېټ_other_indent (bool same_paragraph)
{
  إذا (وېشل)
    other_indent = لومړی_indent;
  وإلا إذا (crown)
    {
      other_indent = (same_paragraph ? دننه_ستنه : لومړی_indent);
    }
  وإلا إذا (tagged)
    {
      إذا (same_paragraph && دننه_ستنه != لومړی_indent)
        {
          other_indent = دننه_ستنه;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      وإلا إذا (other_indent == لومړی_indent)
        other_indent = لومړی_indent == 0 ? DEF_INDENT : 0;
    }
  وإلا
    {
      other_indent = لومړی_indent;
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

  وروستی_کرښه_اوږدتیا = 0;
  c = بل_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  طالما (c == '\n' || c == EOF
         || بل_مختاړی_indent < مختاړی_lead_space
         || دننه_ستنه < بل_مختاړی_indent + مختاړی_ډک_اوږدتیا)
    {
      c = copy_rest (f, c);
      إذا (c == EOF)
        {
          بل_char = EOF;
          أرجع false;
        }
      توری_چاپ_کړه ('\n');
      c = get_مختاړی (f);
    }

  /* Got a suitable first line for a paragraph.  */

  مختاړی_indent = بل_مختاړی_indent;
  لومړی_indent = دننه_ستنه;
  wptr = parabuf;
  کلمه_حد = کلمه_2;
  c = get_کرښه (f, c);
  سېټ_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  إذا (وېشل)
    {
      /*تشه*/
    }
  وإلا إذا (crown)
    {
      إذا (same_para (c))
        {
          افعل
            {			/* for each line till the end of the para */
              c = get_کرښه (f, c);
            }
          طالما (same_para (c) && دننه_ستنه == other_indent);
        }
    }
  وإلا إذا (tagged)
    {
      إذا (same_para (c) && دننه_ستنه != لومړی_indent)
        {
          افعل
            {			/* for each line till the end of the para */
              c = get_کرښه (f, c);
            }
          طالما (same_para (c) && دننه_ستنه == other_indent);
        }
    }
  وإلا
    {
      طالما (same_para (c) && دننه_ستنه == other_indent)
        c = get_کرښه (f, c);
    }

  (کلمه_حد - 1)->period = (کلمه_حد - 1)->final = true;
  بل_char = c;
  أرجع true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

ساكن صحيح
copy_rest (FILE *f, صحيح c)
{
  بهر_ستنه = 0;
  إذا (دننه_ستنه > بل_مختاړی_indent || (c != '\n' && c != EOF))
    {
      put_space (بل_مختاړی_indent);
      لكل (حرف ثابت *s = مختاړی; بهر_ستنه != دننه_ستنه && *s; بهر_ستنه++)
        توری_چاپ_کړه (*s++);
      إذا (c != EOF && c != '\n')
        put_space (دننه_ستنه - بهر_ستنه);
      إذا (c == EOF && دننه_ستنه >= بل_مختاړی_indent + مختاړی_اوږدتیا)
        توری_چاپ_کړه ('\n');
    }
  طالما (c != '\n' && c != EOF)
    {
      توری_چاپ_کړه (c);
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
  أرجع (بل_مختاړی_indent == مختاړی_indent
          && دننه_ستنه >= بل_مختاړی_indent + مختاړی_ډک_اوږدتیا
          && c != '\n' && c != EOF);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

ساكن صحيح
get_کرښه (FILE *f, صحيح c)
{
  صحيح پیل;
  حرف *پای_ټکی_د_parabuf;
  کلمه *پای_ټکی_د_کلمه;

  پای_ټکی_د_parabuf = &parabuf[MAXCHARS];
  پای_ټکی_د_کلمه = &کلمه_2[MAXWORDS - 2];

  افعل
    {				/* for each word in a line */

      /* Scan word.  */

      کلمه_حد->متن = wptr;
      افعل
        {
          إذا (wptr == پای_ټکی_د_parabuf)
            {
              سېټ_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      طالما (c != EOF && !c_isspace (c));
      دننه_ستنه += کلمه_حد->اوږدتیا = wptr - کلمه_حد->متن;
      check_punctuation (کلمه_حد);

      /* Scan inter-word space.  */

      پیل = دننه_ستنه;
      c = get_space (f, c);
      کلمه_حد->space = دننه_ستنه - پیل;
      کلمه_حد->final = (c == EOF
                           || (کلمه_حد->period
                               && (c == '\n' || کلمه_حد->space > 1)));
      إذا (c == '\n' || c == EOF || یوشان)
        کلمه_حد->space = کلمه_حد->final ? 2 : 1;
      إذا (کلمه_حد == پای_ټکی_د_کلمه)
        {
          سېټ_other_indent (true);
          flush_paragraph ();
        }
      کلمه_حد++;
    }
  طالما (c != '\n' && c != EOF);
  أرجع get_مختاړی (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

ساكن صحيح
get_مختاړی (FILE *f)
{
  صحيح c;

  دننه_ستنه = 0;
  c = get_space (f, getc (f));
  إذا (مختاړی_اوږدتیا == 0)
    بل_مختاړی_indent = مختاړی_lead_space < دننه_ستنه ?
      مختاړی_lead_space : دننه_ستنه;
  وإلا
    {
      بل_مختاړی_indent = دننه_ستنه;
      لكل (حرف ثابت *p = مختاړی; *p != '\0'; p++)
        {
          بینښه حرف pc = *p;
          إذا (c != pc)
            أرجع c;
          دننه_ستنه++;
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
        دننه_ستنه++;
      وإلا إذا (c == '\t')
        {
          tabs = true;
          دننه_ستنه = (دننه_ستنه / TABWIDTH + 1) * TABWIDTH;
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
  حرف ثابت *پیل = w->متن;
  حرف ثابت *پای = پیل + (w->اوږدتیا - 1);
  بینښه حرف fin = *پای;

  w->paren = isopen (*پیل);
  w->punct = !! ispunct (fin);
  طالما (پیل < پای && isclose (*پای))
    پای--;
  w->period = isperiod (*پای);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

ساكن فراغ
flush_paragraph (فراغ)
{
  کلمه *وېشل_ټکی;
  کلمه *w;
  صحيح ښويدنه;
  لګښت غوره_break;

  /* In the special case where it's all one word, just flush it.  */

  إذا (کلمه_حد == کلمه_2)
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

  وېشل_ټکی = کلمه_حد;
  غوره_break = MAXCOST;
  لكل (w = کلمه_2->بل_break; w != کلمه_حد; w = w->بل_break)
    {
      إذا (w->غوره_لګښت - w->بل_break->غوره_لګښت < غوره_break)
        {
          وېشل_ټکی = w;
          غوره_break = w->غوره_لګښت - w->بل_break->غوره_لګښت;
        }
      إذا (غوره_break <= MAXCOST - کرښه_CREDIT)
        غوره_break += کرښه_CREDIT;
    }
  put_paragraph (وېشل_ټکی);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, وېشل_ټکی->متن, wptr - وېشل_ټکی->متن);
  ښويدنه = وېشل_ټکی->متن - parabuf;
  wptr -= ښويدنه;

  /* Adjust text pointers.  */

  لكل (w = وېشل_ټکی; w <= کلمه_حد; w++)
    w->متن -= ښويدنه;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (کلمه_2, وېشل_ټکی, (کلمه_حد - وېشل_ټکی + 1) * حجم *کلمه_2);
  کلمه_حد -= وېشل_ټکی - کلمه_2;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

ساكن فراغ
fmt_paragraph (فراغ)
{
  کلمه *w;
  صحيح len;
  لګښت wcost, غوره;
  صحيح saved_اوږدتیا;

  کلمه_حد->غوره_لګښت = 0;
  saved_اوږدتیا = کلمه_حد->اوږدتیا;
  کلمه_حد->اوږدتیا = max_پلنوالی;	/* sentinel */

  لكل (کلمه *پیل = کلمه_حد - 1; پیل >= کلمه_2; پیل--)
    {
      غوره = MAXCOST;
      len = پیل == کلمه_2 ? لومړی_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = پیل;
      len += w->اوږدتیا;
      افعل
        {
          w++;

          /* Consider breaking before w.  */

          wcost = کرښه_لګښت_2 (w, len) + w->غوره_لګښت;
          إذا (پیل == کلمه_2 && وروستی_کرښه_اوږدتیا > 0)
            wcost += RAGGED_لګښت (len - وروستی_کرښه_اوږدتیا);
          إذا (wcost < غوره)
            {
              غوره = wcost;
              پیل->بل_break = w;
              پیل->کرښه_اوږدتیا = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          إذا (w == کلمه_حد)
            اكسر;

          len += (w - 1)->space + w->اوږدتیا;	/* w > start >= word */
        }
      طالما (len <= max_پلنوالی);
      پیل->غوره_لګښت = غوره + بنسټ_لګښت (پیل);
    }

  کلمه_حد->اوږدتیا = saved_اوږدتیا;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#إذا __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#د_که_پای

/* Return the constant component of the cost of breaking before the
   word THIS.  */

ساكن لګښت
بنسټ_لګښت (کلمه *this)
{
  لګښت لګښت_2;

  لګښت_2 = کرښه_لګښت;

  إذا (this > کلمه_2)
    {
      إذا ((this - 1)->period)
        {
          إذا ((this - 1)->final)
            لګښت_2 -= SENTENCE_BONUS;
          وإلا
            لګښت_2 += NOBREAK_لګښت;
        }
      وإلا إذا ((this - 1)->punct)
        لګښت_2 -= PUNCT_BONUS;
      وإلا إذا (this > کلمه_2 + 1 && (this - 2)->final)
        لګښت_2 += WIDOW_لګښت ((this - 1)->اوږدتیا);
    }

  إذا (this->paren)
    لګښت_2 -= PAREN_BONUS;
  وإلا إذا (this->final)
    لګښت_2 += ORPHAN_لګښت (this->اوږدتیا);

  أرجع لګښت_2;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

ساكن لګښت
کرښه_لګښت_2 (کلمه *بل, صحيح len)
{
  صحيح n;
  لګښت لګښت_2;

  إذا (بل == کلمه_حد)
    أرجع 0;
  n = goal_پلنوالی - len;
  لګښت_2 = SHORT_لګښت (n);
  إذا (بل->بل_break != کلمه_حد)
    {
      n = len - بل->کرښه_اوږدتیا;
      لګښت_2 += RAGGED_لګښت (n);
    }
  أرجع لګښت_2;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

ساكن فراغ
put_paragraph (کلمه *پای)
{
  put_کرښه (کلمه_2, لومړی_indent);
  لكل (کلمه *w = کلمه_2->بل_break; w != پای; w = w->بل_break)
    put_کرښه (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

ساكن فراغ
put_کرښه (کلمه *w, صحيح indent)
{
  کلمه *endline;

  بهر_ستنه = 0;
  put_space (مختاړی_indent);
  fputs (مختاړی, stdout);
  بهر_ستنه += مختاړی_اوږدتیا;
  put_space (indent - بهر_ستنه);

  endline = w->بل_break - 1;
  لكل (; w != endline; w++)
    {
      put_کلمه (w);
      put_space (w->space);
    }
  put_کلمه (w);
  وروستی_کرښه_اوږدتیا = بهر_ستنه;
  توری_چاپ_کړه ('\n');

  إذا (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

ساكن فراغ
put_کلمه (کلمه *w)
{
  حرف ثابت *s = w->متن;
  لكل (صحيح n = w->اوږدتیا; n != 0; n--)
    توری_چاپ_کړه (*s++);
  بهر_ستنه += w->اوږدتیا;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

ساكن فراغ
put_space (صحيح space)
{
  صحيح space_موخه, tab_موخه;

  space_موخه = بهر_ستنه + space;
  إذا (tabs)
    {
      tab_موخه = space_موخه / TABWIDTH * TABWIDTH;
      إذا (بهر_ستنه + 1 < tab_موخه)
        طالما (بهر_ستنه < tab_موخه)
          {
            توری_چاپ_کړه ('\t');
            بهر_ستنه = (بهر_ستنه / TABWIDTH + 1) * TABWIDTH;
          }
    }
  طالما (بهر_ستنه < space_موخه)
    {
      توری_چاپ_کړه (' ');
      بهر_ستنه++;
    }
}
