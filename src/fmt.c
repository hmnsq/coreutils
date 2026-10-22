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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <getopt.h>

/* Redefine.  Otherwise, systems (Unicos for one) with headers that define
   it to be a type get syntax errors for the variable declaration below.  */
#تعریف_کرو لفظ_2 غیر_مستعمل_لفظ_type

#شامل "c-ctype.h"
#شامل "system.h"
#شامل "fadvise.h"
#شامل "xdectoint.h"

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

قسمتعریف طویل صحیح لاگت;

#تعریف_کرو MAXCOST	TYPE_زیادہ_سے_زیادہ (لاگت)

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

#تعریف_کرو isopen(c)	(strchr ("(['`\"", c) != تہی)
#تعریف_کرو isclose(c)	(strchr (")]'\"", c) != تہی)
#تعریف_کرو isperiod(c)	(strchr (".?!", c) != تہی)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#تعریف_کرو TABWIDTH	8

/* Word descriptor structure.  */

قسمتعریف ساخت لفظ_3 لفظ;

ساخت لفظ_3
  {

    /* Static attributes determined during input.  */

    حرف ثابت *متن;		/* the text of the word */
    صحیح لمبائی;			/* length of this word */
    صحیح space;			/* the size of the following space */
    غیرعلامتی صحیح paren:1;	/* starts with open paren */
    غیرعلامتی صحیح period:1;	/* ends in [.?!])* */
    غیرعلامتی صحیح punct:1;	/* ends in punctuation */
    غیرعلامتی صحیح final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    صحیح سطر_لمبائی;		/* length of the best line starting here */
    لاگت بہترین_لاگت;		/* cost of best paragraph starting here */
    لفظ *اگلا_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

ساکن خالی سیٹ_سابقہ (حرف *p);
ساکن bool fmt (FILE *f, حرف ثابت *);
ساکن bool get_paragraph (FILE *f);
ساکن صحیح get_سطر (FILE *f, صحیح c);
ساکن صحیح get_سابقہ (FILE *f);
ساکن صحیح get_space (FILE *f, صحیح c);
ساکن صحیح copy_rest (FILE *f, صحیح c);
ساکن bool same_para (صحیح c);
ساکن خالی flush_paragraph (خالی);
ساکن خالی fmt_paragraph (خالی);
ساکن خالی check_punctuation (لفظ *w);
ساکن لاگت بنیاد_لاگت (لفظ *this);
ساکن لاگت سطر_لاگت_2 (لفظ *اگلا, صحیح len);
ساکن خالی put_paragraph (لفظ *اختتام);
ساکن خالی put_سطر (لفظ *w, صحیح indent);
ساکن خالی put_لفظ (لفظ *w);
ساکن خالی put_space (صحیح space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
ساکن bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
ساکن bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
ساکن bool بانٹو;

/* If true, don't preserve inter-word spacing (default false).  */
ساکن bool یکساں;

/* Prefix minus leading and trailing spaces (default "").  */
ساکن حرف ثابت *سابقہ = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
ساکن صحیح max_چوڑائی = چوڑائی_2;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
ساکن صحیح سابقہ_بھرا_لمبائی;

/* The length of the leading space trimmed from the prefix.  */
ساکن صحیح سابقہ_lead_space;

/* The length of prefix minus leading and trailing space.  */
ساکن صحیح سابقہ_لمبائی;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
ساکن صحیح goal_چوڑائی;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
ساکن صحیح اندر_عمود;

/* Start column of the next character to be written to stdout.  */
ساکن صحیح برآمد_عمود;

/* Space for the paragraph text -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ساکن حرف parabuf[MAXCHARS];

/* A pointer into parabuf, indicating the first unused character position.  */
ساکن حرف *wptr;

/* The words of a paragraph -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ساکن لفظ لفظ_2[MAXWORDS];

/* A pointer into the above word array, indicating the first position
   after the last complete word.  Sometimes it will point at an incomplete
   word.  */
ساکن لفظ *لفظ_حد_بندی;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
ساکن bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
ساکن صحیح سابقہ_indent;

/* Indentation of the first line of the current paragraph.  */
ساکن صحیح پہلا_indent;

/* Indentation of other lines of the current paragraph */
ساکن صحیح other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
ساکن صحیح اگلا_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
ساکن صحیح اگلا_سابقہ_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
ساکن صحیح آخری_سطر_لمبائی;

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [-WIDTH] [OPTION]... [FILE]...\n"), پروگرام_name);
      fputs (_("\
Reformat each paragraph اندر the FILE(s), writing مقصد standard output.\n\
The option -چوڑائی_2 is an abbreviated form کا --چوڑائی=DIGITS.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --crown-margin\n\
         preserve indentation کا پہلا two سطریں\n\
"));
      oputs (_("\
  -p, --سابقہ=STRING\n\
         reformat only سطریں beginning with STRING,\n\
         reattaching the سابقہ مقصد reformatted سطریں\n\
"));
      oputs (_("\
  -s, --بانٹو-only\n\
         بانٹو طویل سطریں, but کرو not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation کا پہلا سطر different from دوسرا\n\
"));
      oputs (_("\
  -u, --یکساں-spacing\n\
         one space between الفاظ, two after sentences\n\
"));
      oputs (_("\
  -w, --چوڑائی=چوڑائی_2\n\
         زیادہ_سے_زیادہ سطر چوڑائی (طےشدہ کا 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=چوڑائی_2\n\
         goal چوڑائی (طےشدہ کا 93% کا چوڑائی)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Decode options and launch execution.  */

ساکن ساخت option ثابت long_options[] =
{
  {"crown-margin", no_دلیل, تہی, 'c'},
  {"prefix", required_دلیل, تہی, 'p'},
  {"split-only", no_دلیل, تہی, 's'},
  {"tagged-paragraph", no_دلیل, تہی, 't'},
  {"uniform-spacing", no_دلیل, تہی, 'u'},
  {"width", required_دلیل, تہی, 'w'},
  {"goal", required_دلیل, تہی, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0},
};

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optchar;
  bool ok = true;
  حرف ثابت *max_چوڑائی_option = تہی;
  حرف ثابت *goal_چوڑائی_option = تہی;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  اگر (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_چوڑائی_option = argv[1] + 1;

      /* Make the option we just parsed invisible to getopt.  */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  جب ((optchar = getopt_long (argc, argv, "0123456789cstuw:p:g:",
                                 long_options, تہی))
         != -1)
    انتخاب (optchar)
      {
      طےشدہ:
        اگر (c_isdigit (optchar))
          error (0, 0, _("invalid option -- %c; -چوڑائی_2 is recognized\
 only when it is the پہلا\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      حالت 'c':
        crown = true;
        توڑو;

      حالت 's':
        بانٹو = true;
        توڑو;

      حالت 't':
        tagged = true;
        توڑو;

      حالت 'u':
        یکساں = true;
        توڑو;

      حالت 'w':
        max_چوڑائی_option = optarg;
        توڑو;

      حالت 'g':
        goal_چوڑائی_option = optarg;
        توڑو;

      حالت 'p':
        سیٹ_سابقہ (optarg);
        توڑو;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

      }

  اگر (max_چوڑائی_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_چوڑائی = xnumtoumax (max_چوڑائی_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  اگر (goal_چوڑائی_option)
    {
      /* Limit goal_width to max_width.  */
      goal_چوڑائی = xdectoumax (goal_چوڑائی_option, 0, max_چوڑائی, "",
                               _("invalid goal"), 0);
      اگر (max_چوڑائی_option == تہی)
        max_چوڑائی = goal_چوڑائی + 10;
    }
  ورنہ
    {
      goal_چوڑائی = max_چوڑائی * (2 * (100 - LEEWAY) + 1) / 200;
    }

  bool have_read_stdin = false;

  اگر (optind == argc)
    {
      have_read_stdin = true;
      ok = fmt (stdin, "-");
    }
  ورنہ
    {
      برائے (; optind < argc; optind++)
        {
          حرف *file = argv[optind];
          اگر (streq (file, "-"))
            {
              ok &= fmt (stdin, file);
              have_read_stdin = true;
            }
          ورنہ
            {
              FILE *اندر_دھارا;
              اندر_دھارا = fopen (file, "r");
              اگر (اندر_دھارا != تہی)
                ok &= fmt (اندر_دھارا, file);
              ورنہ
                {
                  error (0, errno, _("cannot open %s for reading"),
                         quoteaf (file));
                  ok = false;
                }
            }
        }
    }

  اگر (have_read_stdin && fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s", _("closing standard input"));

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Trim space from the front and back of the string P, yielding the prefix,
   and record the lengths of the prefix and the space trimmed.  */

ساکن خالی
سیٹ_سابقہ (حرف *p)
{
  حرف *s;

  سابقہ_lead_space = 0;
  جب (*p == ' ')
    {
      سابقہ_lead_space++;
      p++;
    }
  سابقہ = p;
  سابقہ_بھرا_لمبائی = لڑی_لمبائی (p);
  s = p + سابقہ_بھرا_لمبائی;
  جب (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  سابقہ_لمبائی = s - p;
}

/* Read F and send formatted output to stdout.
   Close F when done, unless F is stdin.  Diagnose input errors, using FILE.
   If !F, assume F resulted from an fopen failure and diagnose that.
   Return true if successful.  */

ساکن bool
fmt (FILE *f, حرف ثابت *file)
{
  fadvise (f, FADVISE_SEQUENTIAL);
  tabs = false;
  other_indent = 0;
  اگلا_char = get_سابقہ (f);
  جب (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (لفظ_حد_بندی);
    }

  صحیح err = ferror (f) ? 0 : -1;
  /* err < 0 means success.  */
  اگر (f == stdin)
    clearerr (f);
  ورنہ اگر (fclose (f) != 0 && err < 0)
    err = errno;
  اگر (0 <= err)
    {
      اگر (f == stdin)
        error (0, errno, _("read error"));
      ورنہ
        error (0, errno, _("error reading %s"), quoteaf (file));
    }
  واپس err < 0;
}

/* Set the global variable 'other_indent' according to SAME_PARAGRAPH
   and other global variables.  */

ساکن خالی
سیٹ_other_indent (bool same_paragraph)
{
  اگر (بانٹو)
    other_indent = پہلا_indent;
  ورنہ اگر (crown)
    {
      other_indent = (same_paragraph ? اندر_عمود : پہلا_indent);
    }
  ورنہ اگر (tagged)
    {
      اگر (same_paragraph && اندر_عمود != پہلا_indent)
        {
          other_indent = اندر_عمود;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      ورنہ اگر (other_indent == پہلا_indent)
        other_indent = پہلا_indent == 0 ? DEF_INDENT : 0;
    }
  ورنہ
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

ساکن bool
get_paragraph (FILE *f)
{
  صحیح c;

  آخری_سطر_لمبائی = 0;
  c = اگلا_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  جب (c == '\n' || c == EOF
         || اگلا_سابقہ_indent < سابقہ_lead_space
         || اندر_عمود < اگلا_سابقہ_indent + سابقہ_بھرا_لمبائی)
    {
      c = copy_rest (f, c);
      اگر (c == EOF)
        {
          اگلا_char = EOF;
          واپس false;
        }
      حرف_لکھو ('\n');
      c = get_سابقہ (f);
    }

  /* Got a suitable first line for a paragraph.  */

  سابقہ_indent = اگلا_سابقہ_indent;
  پہلا_indent = اندر_عمود;
  wptr = parabuf;
  لفظ_حد_بندی = لفظ_2;
  c = get_سطر (f, c);
  سیٹ_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  اگر (بانٹو)
    {
      /*خالی_خانے*/
    }
  ورنہ اگر (crown)
    {
      اگر (same_para (c))
        {
          کرو
            {			/* for each line till the end of the para */
              c = get_سطر (f, c);
            }
          جب (same_para (c) && اندر_عمود == other_indent);
        }
    }
  ورنہ اگر (tagged)
    {
      اگر (same_para (c) && اندر_عمود != پہلا_indent)
        {
          کرو
            {			/* for each line till the end of the para */
              c = get_سطر (f, c);
            }
          جب (same_para (c) && اندر_عمود == other_indent);
        }
    }
  ورنہ
    {
      جب (same_para (c) && اندر_عمود == other_indent)
        c = get_سطر (f, c);
    }

  (لفظ_حد_بندی - 1)->period = (لفظ_حد_بندی - 1)->final = true;
  اگلا_char = c;
  واپس true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

ساکن صحیح
copy_rest (FILE *f, صحیح c)
{
  برآمد_عمود = 0;
  اگر (اندر_عمود > اگلا_سابقہ_indent || (c != '\n' && c != EOF))
    {
      put_space (اگلا_سابقہ_indent);
      برائے (حرف ثابت *s = سابقہ; برآمد_عمود != اندر_عمود && *s; برآمد_عمود++)
        حرف_لکھو (*s++);
      اگر (c != EOF && c != '\n')
        put_space (اندر_عمود - برآمد_عمود);
      اگر (c == EOF && اندر_عمود >= اگلا_سابقہ_indent + سابقہ_لمبائی)
        حرف_لکھو ('\n');
    }
  جب (c != '\n' && c != EOF)
    {
      حرف_لکھو (c);
      c = getc (f);
    }
  واپس c;
}

/* Return true if a line whose first non-blank character after the
   prefix (if any) is C could belong to the current paragraph,
   otherwise false.  */

ساکن bool
same_para (صحیح c)
{
  واپس (اگلا_سابقہ_indent == سابقہ_indent
          && اندر_عمود >= اگلا_سابقہ_indent + سابقہ_بھرا_لمبائی
          && c != '\n' && c != EOF);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

ساکن صحیح
get_سطر (FILE *f, صحیح c)
{
  صحیح آغاز;
  حرف *انتہا_کا_parabuf;
  لفظ *انتہا_کا_لفظ;

  انتہا_کا_parabuf = &parabuf[MAXCHARS];
  انتہا_کا_لفظ = &لفظ_2[MAXWORDS - 2];

  کرو
    {				/* for each word in a line */

      /* Scan word.  */

      لفظ_حد_بندی->متن = wptr;
      کرو
        {
          اگر (wptr == انتہا_کا_parabuf)
            {
              سیٹ_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      جب (c != EOF && !c_isspace (c));
      اندر_عمود += لفظ_حد_بندی->لمبائی = wptr - لفظ_حد_بندی->متن;
      check_punctuation (لفظ_حد_بندی);

      /* Scan inter-word space.  */

      آغاز = اندر_عمود;
      c = get_space (f, c);
      لفظ_حد_بندی->space = اندر_عمود - آغاز;
      لفظ_حد_بندی->final = (c == EOF
                           || (لفظ_حد_بندی->period
                               && (c == '\n' || لفظ_حد_بندی->space > 1)));
      اگر (c == '\n' || c == EOF || یکساں)
        لفظ_حد_بندی->space = لفظ_حد_بندی->final ? 2 : 1;
      اگر (لفظ_حد_بندی == انتہا_کا_لفظ)
        {
          سیٹ_other_indent (true);
          flush_paragraph ();
        }
      لفظ_حد_بندی++;
    }
  جب (c != '\n' && c != EOF);
  واپس get_سابقہ (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

ساکن صحیح
get_سابقہ (FILE *f)
{
  صحیح c;

  اندر_عمود = 0;
  c = get_space (f, getc (f));
  اگر (سابقہ_لمبائی == 0)
    اگلا_سابقہ_indent = سابقہ_lead_space < اندر_عمود ?
      سابقہ_lead_space : اندر_عمود;
  ورنہ
    {
      اگلا_سابقہ_indent = اندر_عمود;
      برائے (حرف ثابت *p = سابقہ; *p != '\0'; p++)
        {
          غیرعلامتی حرف pc = *p;
          اگر (c != pc)
            واپس c;
          اندر_عمود++;
          c = getc (f);
        }
      c = get_space (f, c);
    }
  واپس c;
}

/* Read blank characters from input file F, starting with C, and keeping
   in_column up-to-date.  Return first non-blank character.  */

ساکن صحیح
get_space (FILE *f, صحیح c)
{
  جب (true)
    {
      اگر (c == ' ')
        اندر_عمود++;
      ورنہ اگر (c == '\t')
        {
          tabs = true;
          اندر_عمود = (اندر_عمود / TABWIDTH + 1) * TABWIDTH;
        }
      ورنہ
        واپس c;
      c = getc (f);
    }
}

/* Set extra fields in word W describing any attached punctuation.  */

ساکن خالی
check_punctuation (لفظ *w)
{
  حرف ثابت *آغاز = w->متن;
  حرف ثابت *اختتام = آغاز + (w->لمبائی - 1);
  غیرعلامتی حرف fin = *اختتام;

  w->paren = isopen (*آغاز);
  w->punct = !! ispunct (fin);
  جب (آغاز < اختتام && isclose (*اختتام))
    اختتام--;
  w->period = isperiod (*اختتام);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

ساکن خالی
flush_paragraph (خالی)
{
  لفظ *بانٹو_نقطہ;
  لفظ *w;
  صحیح سرکاؤ;
  لاگت بہترین_break;

  /* In the special case where it's all one word, just flush it.  */

  اگر (لفظ_حد_بندی == لفظ_2)
    {
      size_t مقصد_write = wptr - parabuf;
      اگر (fwrite (parabuf, 1, مقصد_write, stdout) != مقصد_write)
        write_error ();

      wptr = parabuf;
      واپس;
    }

  /* Otherwise:
     - format what you have so far as a paragraph,
     - find a low-cost line break near the end,
     - output to there,
     - make that the start of the paragraph.  */

  fmt_paragraph ();

  /* Choose a good split point.  */

  بانٹو_نقطہ = لفظ_حد_بندی;
  بہترین_break = MAXCOST;
  برائے (w = لفظ_2->اگلا_break; w != لفظ_حد_بندی; w = w->اگلا_break)
    {
      اگر (w->بہترین_لاگت - w->اگلا_break->بہترین_لاگت < بہترین_break)
        {
          بانٹو_نقطہ = w;
          بہترین_break = w->بہترین_لاگت - w->اگلا_break->بہترین_لاگت;
        }
      اگر (بہترین_break <= MAXCOST - سطر_CREDIT)
        بہترین_break += سطر_CREDIT;
    }
  put_paragraph (بانٹو_نقطہ);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, بانٹو_نقطہ->متن, wptr - بانٹو_نقطہ->متن);
  سرکاؤ = بانٹو_نقطہ->متن - parabuf;
  wptr -= سرکاؤ;

  /* Adjust text pointers.  */

  برائے (w = بانٹو_نقطہ; w <= لفظ_حد_بندی; w++)
    w->متن -= سرکاؤ;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (لفظ_2, بانٹو_نقطہ, (لفظ_حد_بندی - بانٹو_نقطہ + 1) * حجم *لفظ_2);
  لفظ_حد_بندی -= بانٹو_نقطہ - لفظ_2;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

ساکن خالی
fmt_paragraph (خالی)
{
  لفظ *w;
  صحیح len;
  لاگت wcost, بہترین;
  صحیح saved_لمبائی;

  لفظ_حد_بندی->بہترین_لاگت = 0;
  saved_لمبائی = لفظ_حد_بندی->لمبائی;
  لفظ_حد_بندی->لمبائی = max_چوڑائی;	/* sentinel */

  برائے (لفظ *آغاز = لفظ_حد_بندی - 1; آغاز >= لفظ_2; آغاز--)
    {
      بہترین = MAXCOST;
      len = آغاز == لفظ_2 ? پہلا_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = آغاز;
      len += w->لمبائی;
      کرو
        {
          w++;

          /* Consider breaking before w.  */

          wcost = سطر_لاگت_2 (w, len) + w->بہترین_لاگت;
          اگر (آغاز == لفظ_2 && آخری_سطر_لمبائی > 0)
            wcost += RAGGED_لاگت (len - آخری_سطر_لمبائی);
          اگر (wcost < بہترین)
            {
              بہترین = wcost;
              آغاز->اگلا_break = w;
              آغاز->سطر_لمبائی = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          اگر (w == لفظ_حد_بندی)
            توڑو;

          len += (w - 1)->space + w->لمبائی;	/* w > start >= word */
        }
      جب (len <= max_چوڑائی);
      آغاز->بہترین_لاگت = بہترین + بنیاد_لاگت (آغاز);
    }

  لفظ_حد_بندی->لمبائی = saved_لمبائی;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#اگر __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#اگر_ختم

/* Return the constant component of the cost of breaking before the
   word THIS.  */

ساکن لاگت
بنیاد_لاگت (لفظ *this)
{
  لاگت لاگت_2;

  لاگت_2 = سطر_لاگت;

  اگر (this > لفظ_2)
    {
      اگر ((this - 1)->period)
        {
          اگر ((this - 1)->final)
            لاگت_2 -= SENTENCE_BONUS;
          ورنہ
            لاگت_2 += NOBREAK_لاگت;
        }
      ورنہ اگر ((this - 1)->punct)
        لاگت_2 -= PUNCT_BONUS;
      ورنہ اگر (this > لفظ_2 + 1 && (this - 2)->final)
        لاگت_2 += WIDOW_لاگت ((this - 1)->لمبائی);
    }

  اگر (this->paren)
    لاگت_2 -= PAREN_BONUS;
  ورنہ اگر (this->final)
    لاگت_2 += ORPHAN_لاگت (this->لمبائی);

  واپس لاگت_2;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

ساکن لاگت
سطر_لاگت_2 (لفظ *اگلا, صحیح len)
{
  صحیح n;
  لاگت لاگت_2;

  اگر (اگلا == لفظ_حد_بندی)
    واپس 0;
  n = goal_چوڑائی - len;
  لاگت_2 = SHORT_لاگت (n);
  اگر (اگلا->اگلا_break != لفظ_حد_بندی)
    {
      n = len - اگلا->سطر_لمبائی;
      لاگت_2 += RAGGED_لاگت (n);
    }
  واپس لاگت_2;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

ساکن خالی
put_paragraph (لفظ *اختتام)
{
  put_سطر (لفظ_2, پہلا_indent);
  برائے (لفظ *w = لفظ_2->اگلا_break; w != اختتام; w = w->اگلا_break)
    put_سطر (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

ساکن خالی
put_سطر (لفظ *w, صحیح indent)
{
  لفظ *endline;

  برآمد_عمود = 0;
  put_space (سابقہ_indent);
  fputs (سابقہ, stdout);
  برآمد_عمود += سابقہ_لمبائی;
  put_space (indent - برآمد_عمود);

  endline = w->اگلا_break - 1;
  برائے (; w != endline; w++)
    {
      put_لفظ (w);
      put_space (w->space);
    }
  put_لفظ (w);
  آخری_سطر_لمبائی = برآمد_عمود;
  حرف_لکھو ('\n');

  اگر (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

ساکن خالی
put_لفظ (لفظ *w)
{
  حرف ثابت *s = w->متن;
  برائے (صحیح n = w->لمبائی; n != 0; n--)
    حرف_لکھو (*s++);
  برآمد_عمود += w->لمبائی;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

ساکن خالی
put_space (صحیح space)
{
  صحیح space_ہدف, tab_ہدف;

  space_ہدف = برآمد_عمود + space;
  اگر (tabs)
    {
      tab_ہدف = space_ہدف / TABWIDTH * TABWIDTH;
      اگر (برآمد_عمود + 1 < tab_ہدف)
        جب (برآمد_عمود < tab_ہدف)
          {
            حرف_لکھو ('\t');
            برآمد_عمود = (برآمد_عمود / TABWIDTH + 1) * TABWIDTH;
          }
    }
  جب (برآمد_عمود < space_ہدف)
    {
      حرف_لکھو (' ');
      برآمد_عمود++;
    }
}
