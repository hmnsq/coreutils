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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>

/* Redefine.  Otherwise, systems (Unicos for one) with headers that define
   it to be a type get syntax errors for the variable declaration below.  */
#నిర్వచించు మాట_2 వాడని_మాట_type

#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "system.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "fmt"

#నిర్వచించు AUTHORS proper_name ("Ross Paterson")

/* The following parameters represent the program's idea of what is
   "best".  Adjust to taste, subject to the caveats given.  */

/* Default longest permitted line length (max_width).  */
#నిర్వచించు వెడల్పు_2	75

/* Prefer lines to be LEEWAY % shorter than the maximum width, giving
   room for optimization.  */
#నిర్వచించు LEEWAY	7

/* The default secondary indent of tagged paragraph used for unindented
   one-line paragraphs not preceded by any multi-line paragraphs.  */
#నిర్వచించు DEF_INDENT 3

/* Costs and bonuses are expressed as the equivalent departure from the
   optimal line length, multiplied by 10.  e.g. assigning something a
   cost of 50 means that it is as bad as a line 5 characters too short
   or too long.  The definition of SHORT_COST(n) should not be changed.
   However, EQUIV(n) may need tuning.  */

/* FIXME: "fmt" misbehaves given large inputs or options.  One
   possible workaround for part of the problem is to change COST to be
   a floating-point type.  There are other problems besides COST,
   though; see MAXWORDS below.  */

రకనిర్వచనం దీర్ఘ పూర్ణాంకం ఖర్చు;

#నిర్వచించు MAXCOST	TYPE_గరిష్ఠ (ఖర్చు)

#నిర్వచించు SQR(n)		((n) * (n))
#నిర్వచించు EQUIV(n)	SQR ((ఖర్చు) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#నిర్వచించు SHORT_ఖర్చు(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#నిర్వచించు RAGGED_ఖర్చు(n)	(SHORT_ఖర్చు (n) / 2)

/* Basic cost per line.  */
#నిర్వచించు పంక్తి_ఖర్చు	EQUIV (70)

/* Cost of breaking a line after the first word of a sentence, where
   the length of the word is N.  */
#నిర్వచించు WIDOW_ఖర్చు(n)	(EQUIV (200) / ((n) + 2))

/* Cost of breaking a line before the last word of a sentence, where
   the length of the word is N.  */
#నిర్వచించు ORPHAN_ఖర్చు(n)	(EQUIV (150) / ((n) + 2))

/* Bonus for breaking a line at the end of a sentence.  */
#నిర్వచించు SENTENCE_BONUS	EQUIV (50)

/* Cost of breaking a line after a period not marking end of a sentence.
   With the definition of sentence we are using (borrowed from emacs, see
   get_line()) such a break would then look like a sentence break.  Hence
   we assign a very high cost -- it should be avoided unless things are
   really bad.  */
#నిర్వచించు NOBREAK_ఖర్చు	EQUIV (600)

/* Bonus for breaking a line before open parenthesis.  */
#నిర్వచించు PAREN_BONUS	EQUIV (40)

/* Bonus for breaking a line after other punctuation.  */
#నిర్వచించు PUNCT_BONUS	EQUIV(40)

/* Credit for breaking a long paragraph one line later.  */
#నిర్వచించు పంక్తి_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#నిర్వచించు MAXWORDS	1000
#నిర్వచించు MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#నిర్వచించు isopen(c)	(strchr ("(['`\"", c) != ఖాళీ)
#నిర్వచించు isclose(c)	(strchr (")]'\"", c) != ఖాళీ)
#నిర్వచించు isperiod(c)	(strchr (".?!", c) != ఖాళీ)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#నిర్వచించు TABWIDTH	8

/* Word descriptor structure.  */

రకనిర్వచనం నిర్మాణం మాట_3 మాట;

నిర్మాణం మాట_3
  {

    /* Static attributes determined during input.  */

    అక్షరం స్థిరరాశి *పాఠ్యం;		/* the text of the word */
    పూర్ణాంకం నిడివి;			/* length of this word */
    పూర్ణాంకం space;			/* the size of the following space */
    గుర్తులేని పూర్ణాంకం paren:1;	/* starts with open paren */
    గుర్తులేని పూర్ణాంకం period:1;	/* ends in [.?!])* */
    గుర్తులేని పూర్ణాంకం punct:1;	/* ends in punctuation */
    గుర్తులేని పూర్ణాంకం final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    పూర్ణాంకం పంక్తి_నిడివి;		/* length of the best line starting here */
    ఖర్చు ఉత్తమ_ఖర్చు;		/* cost of best paragraph starting here */
    మాట *తదుపరి_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

స్థిర శూన్య సమితి_ఉపసర్గ (అక్షరం *p);
స్థిర bool fmt (FILE *f, అక్షరం స్థిరరాశి *);
స్థిర bool get_paragraph (FILE *f);
స్థిర పూర్ణాంకం get_పంక్తి (FILE *f, పూర్ణాంకం c);
స్థిర పూర్ణాంకం get_ఉపసర్గ (FILE *f);
స్థిర పూర్ణాంకం get_space (FILE *f, పూర్ణాంకం c);
స్థిర పూర్ణాంకం copy_rest (FILE *f, పూర్ణాంకం c);
స్థిర bool same_para (పూర్ణాంకం c);
స్థిర శూన్య flush_paragraph (శూన్య);
స్థిర శూన్య fmt_paragraph (శూన్య);
స్థిర శూన్య check_punctuation (మాట *w);
స్థిర ఖర్చు పునాది_ఖర్చు (మాట *this);
స్థిర ఖర్చు పంక్తి_ఖర్చు_2 (మాట *తదుపరి, పూర్ణాంకం len);
స్థిర శూన్య put_paragraph (మాట *ముగింపు);
స్థిర శూన్య put_పంక్తి (మాట *w, పూర్ణాంకం indent);
స్థిర శూన్య put_మాట (మాట *w);
స్థిర శూన్య put_space (పూర్ణాంకం space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
స్థిర bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
స్థిర bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
స్థిర bool విభజించు;

/* If true, don't preserve inter-word spacing (default false).  */
స్థిర bool ఏకరీతి;

/* Prefix minus leading and trailing spaces (default "").  */
స్థిర అక్షరం స్థిరరాశి *ఉపసర్గ = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
స్థిర పూర్ణాంకం max_వెడల్పు = వెడల్పు_2;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
స్థిర పూర్ణాంకం ఉపసర్గ_నిండిన_నిడివి;

/* The length of the leading space trimmed from the prefix.  */
స్థిర పూర్ణాంకం ఉపసర్గ_lead_space;

/* The length of prefix minus leading and trailing space.  */
స్థిర పూర్ణాంకం ఉపసర్గ_నిడివి;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
స్థిర పూర్ణాంకం goal_వెడల్పు;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
స్థిర పూర్ణాంకం లోపల_స్తంభం;

/* Start column of the next character to be written to stdout.  */
స్థిర పూర్ణాంకం బహిర్గతం_స్తంభం;

/* Space for the paragraph text -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
స్థిర అక్షరం parabuf[MAXCHARS];

/* A pointer into parabuf, indicating the first unused character position.  */
స్థిర అక్షరం *wptr;

/* The words of a paragraph -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
స్థిర మాట మాట_2[MAXWORDS];

/* A pointer into the above word array, indicating the first position
   after the last complete word.  Sometimes it will point at an incomplete
   word.  */
స్థిర మాట *మాట_హద్దు;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
స్థిర bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
స్థిర పూర్ణాంకం ఉపసర్గ_indent;

/* Indentation of the first line of the current paragraph.  */
స్థిర పూర్ణాంకం మొదటి_indent;

/* Indentation of other lines of the current paragraph */
స్థిర పూర్ణాంకం other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
స్థిర పూర్ణాంకం తదుపరి_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
స్థిర పూర్ణాంకం తదుపరి_ఉపసర్గ_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
స్థిర పూర్ణాంకం చివరి_పంక్తి_నిడివి;

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [-WIDTH] [OPTION]... [FILE]...\n"), కార్యక్రమం_name);
      fputs (_("\
Reformat each paragraph లోపల the FILE(s), writing గమ్యం standard output.\n\
The option -వెడల్పు_2 is an abbreviated form యొక్క --వెడల్పు=DIGITS.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --crown-margin\n\
         preserve indentation యొక్క మొదటి two పంక్తులు\n\
"));
      oputs (_("\
  -p, --ఉపసర్గ=STRING\n\
         reformat only పంక్తులు beginning with STRING,\n\
         reattaching the ఉపసర్గ గమ్యం reformatted పంక్తులు\n\
"));
      oputs (_("\
  -s, --విభజించు-only\n\
         విభజించు దీర్ఘ పంక్తులు, but చేయి not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation యొక్క మొదటి పంక్తి different from రెండవ\n\
"));
      oputs (_("\
  -u, --ఏకరీతి-spacing\n\
         one space between పదాలు, two after sentences\n\
"));
      oputs (_("\
  -w, --వెడల్పు=వెడల్పు_2\n\
         గరిష్ఠ పంక్తి వెడల్పు (అప్రమేయ యొక్క 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=వెడల్పు_2\n\
         goal వెడల్పు (అప్రమేయ యొక్క 93% యొక్క వెడల్పు)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Decode options and launch execution.  */

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"crown-margin", no_వాదం, ఖాళీ, 'c'},
  {"prefix", required_వాదం, ఖాళీ, 'p'},
  {"split-only", no_వాదం, ఖాళీ, 's'},
  {"tagged-paragraph", no_వాదం, ఖాళీ, 't'},
  {"uniform-spacing", no_వాదం, ఖాళీ, 'u'},
  {"width", required_వాదం, ఖాళీ, 'w'},
  {"goal", required_వాదం, ఖాళీ, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0},
};

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optchar;
  bool ok = true;
  అక్షరం స్థిరరాశి *max_వెడల్పు_option = ఖాళీ;
  అక్షరం స్థిరరాశి *goal_వెడల్పు_option = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ఒకవేళ (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_వెడల్పు_option = argv[1] + 1;

      /* Make the option we just parsed invisible to getopt.  */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  అయితే ((optchar = getopt_long (argc, argv, "0123456789cstuw:p:g:",
                                 long_options, ఖాళీ))
         != -1)
    ఎంపిక (optchar)
      {
      అప్రమేయ:
        ఒకవేళ (c_isdigit (optchar))
          error (0, 0, _("invalid option -- %c; -వెడల్పు_2 is recognized\
 only when it is the మొదటి\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      స్థితి 'c':
        crown = true;
        ఆపు;

      స్థితి 's':
        విభజించు = true;
        ఆపు;

      స్థితి 't':
        tagged = true;
        ఆపు;

      స్థితి 'u':
        ఏకరీతి = true;
        ఆపు;

      స్థితి 'w':
        max_వెడల్పు_option = optarg;
        ఆపు;

      స్థితి 'g':
        goal_వెడల్పు_option = optarg;
        ఆపు;

      స్థితి 'p':
        సమితి_ఉపసర్గ (optarg);
        ఆపు;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

      }

  ఒకవేళ (max_వెడల్పు_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_వెడల్పు = xnumtoumax (max_వెడల్పు_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  ఒకవేళ (goal_వెడల్పు_option)
    {
      /* Limit goal_width to max_width.  */
      goal_వెడల్పు = xdectoumax (goal_వెడల్పు_option, 0, max_వెడల్పు, "",
                               _("invalid goal"), 0);
      ఒకవేళ (max_వెడల్పు_option == ఖాళీ)
        max_వెడల్పు = goal_వెడల్పు + 10;
    }
  లేకపోతే
    {
      goal_వెడల్పు = max_వెడల్పు * (2 * (100 - LEEWAY) + 1) / 200;
    }

  bool have_read_stdin = false;

  ఒకవేళ (optind == argc)
    {
      have_read_stdin = true;
      ok = fmt (stdin, "-");
    }
  లేకపోతే
    {
      కొరకు (; optind < argc; optind++)
        {
          అక్షరం *file = argv[optind];
          ఒకవేళ (streq (file, "-"))
            {
              ok &= fmt (stdin, file);
              have_read_stdin = true;
            }
          లేకపోతే
            {
              FILE *లోపల_ప్రవాహం;
              లోపల_ప్రవాహం = fopen (file, "r");
              ఒకవేళ (లోపల_ప్రవాహం != ఖాళీ)
                ok &= fmt (లోపల_ప్రవాహం, file);
              లేకపోతే
                {
                  error (0, errno, _("cannot open %s for reading"),
                         quoteaf (file));
                  ok = false;
                }
            }
        }
    }

  ఒకవేళ (have_read_stdin && fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s", _("closing standard input"));

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Trim space from the front and back of the string P, yielding the prefix,
   and record the lengths of the prefix and the space trimmed.  */

స్థిర శూన్య
సమితి_ఉపసర్గ (అక్షరం *p)
{
  అక్షరం *s;

  ఉపసర్గ_lead_space = 0;
  అయితే (*p == ' ')
    {
      ఉపసర్గ_lead_space++;
      p++;
    }
  ఉపసర్గ = p;
  ఉపసర్గ_నిండిన_నిడివి = అక్షరశ్రేణి_పొడవు (p);
  s = p + ఉపసర్గ_నిండిన_నిడివి;
  అయితే (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  ఉపసర్గ_నిడివి = s - p;
}

/* Read F and send formatted output to stdout.
   Close F when done, unless F is stdin.  Diagnose input errors, using FILE.
   If !F, assume F resulted from an fopen failure and diagnose that.
   Return true if successful.  */

స్థిర bool
fmt (FILE *f, అక్షరం స్థిరరాశి *file)
{
  fadvise (f, FADVISE_SEQUENTIAL);
  tabs = false;
  other_indent = 0;
  తదుపరి_char = get_ఉపసర్గ (f);
  అయితే (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (మాట_హద్దు);
    }

  పూర్ణాంకం err = ferror (f) ? 0 : -1;
  /* err < 0 means success.  */
  ఒకవేళ (f == stdin)
    clearerr (f);
  లేకపోతే ఒకవేళ (fclose (f) != 0 && err < 0)
    err = errno;
  ఒకవేళ (0 <= err)
    {
      ఒకవేళ (f == stdin)
        error (0, errno, _("read error"));
      లేకపోతే
        error (0, errno, _("error reading %s"), quoteaf (file));
    }
  తిరిగి err < 0;
}

/* Set the global variable 'other_indent' according to SAME_PARAGRAPH
   and other global variables.  */

స్థిర శూన్య
సమితి_other_indent (bool same_paragraph)
{
  ఒకవేళ (విభజించు)
    other_indent = మొదటి_indent;
  లేకపోతే ఒకవేళ (crown)
    {
      other_indent = (same_paragraph ? లోపల_స్తంభం : మొదటి_indent);
    }
  లేకపోతే ఒకవేళ (tagged)
    {
      ఒకవేళ (same_paragraph && లోపల_స్తంభం != మొదటి_indent)
        {
          other_indent = లోపల_స్తంభం;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      లేకపోతే ఒకవేళ (other_indent == మొదటి_indent)
        other_indent = మొదటి_indent == 0 ? DEF_INDENT : 0;
    }
  లేకపోతే
    {
      other_indent = మొదటి_indent;
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

స్థిర bool
get_paragraph (FILE *f)
{
  పూర్ణాంకం c;

  చివరి_పంక్తి_నిడివి = 0;
  c = తదుపరి_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  అయితే (c == '\n' || c == EOF
         || తదుపరి_ఉపసర్గ_indent < ఉపసర్గ_lead_space
         || లోపల_స్తంభం < తదుపరి_ఉపసర్గ_indent + ఉపసర్గ_నిండిన_నిడివి)
    {
      c = copy_rest (f, c);
      ఒకవేళ (c == EOF)
        {
          తదుపరి_char = EOF;
          తిరిగి false;
        }
      అక్షరం_రాయి ('\n');
      c = get_ఉపసర్గ (f);
    }

  /* Got a suitable first line for a paragraph.  */

  ఉపసర్గ_indent = తదుపరి_ఉపసర్గ_indent;
  మొదటి_indent = లోపల_స్తంభం;
  wptr = parabuf;
  మాట_హద్దు = మాట_2;
  c = get_పంక్తి (f, c);
  సమితి_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  ఒకవేళ (విభజించు)
    {
      /*ఖాళీలు*/
    }
  లేకపోతే ఒకవేళ (crown)
    {
      ఒకవేళ (same_para (c))
        {
          చేయి
            {			/* for each line till the end of the para */
              c = get_పంక్తి (f, c);
            }
          అయితే (same_para (c) && లోపల_స్తంభం == other_indent);
        }
    }
  లేకపోతే ఒకవేళ (tagged)
    {
      ఒకవేళ (same_para (c) && లోపల_స్తంభం != మొదటి_indent)
        {
          చేయి
            {			/* for each line till the end of the para */
              c = get_పంక్తి (f, c);
            }
          అయితే (same_para (c) && లోపల_స్తంభం == other_indent);
        }
    }
  లేకపోతే
    {
      అయితే (same_para (c) && లోపల_స్తంభం == other_indent)
        c = get_పంక్తి (f, c);
    }

  (మాట_హద్దు - 1)->period = (మాట_హద్దు - 1)->final = true;
  తదుపరి_char = c;
  తిరిగి true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

స్థిర పూర్ణాంకం
copy_rest (FILE *f, పూర్ణాంకం c)
{
  బహిర్గతం_స్తంభం = 0;
  ఒకవేళ (లోపల_స్తంభం > తదుపరి_ఉపసర్గ_indent || (c != '\n' && c != EOF))
    {
      put_space (తదుపరి_ఉపసర్గ_indent);
      కొరకు (అక్షరం స్థిరరాశి *s = ఉపసర్గ; బహిర్గతం_స్తంభం != లోపల_స్తంభం && *s; బహిర్గతం_స్తంభం++)
        అక్షరం_రాయి (*s++);
      ఒకవేళ (c != EOF && c != '\n')
        put_space (లోపల_స్తంభం - బహిర్గతం_స్తంభం);
      ఒకవేళ (c == EOF && లోపల_స్తంభం >= తదుపరి_ఉపసర్గ_indent + ఉపసర్గ_నిడివి)
        అక్షరం_రాయి ('\n');
    }
  అయితే (c != '\n' && c != EOF)
    {
      అక్షరం_రాయి (c);
      c = getc (f);
    }
  తిరిగి c;
}

/* Return true if a line whose first non-blank character after the
   prefix (if any) is C could belong to the current paragraph,
   otherwise false.  */

స్థిర bool
same_para (పూర్ణాంకం c)
{
  తిరిగి (తదుపరి_ఉపసర్గ_indent == ఉపసర్గ_indent
          && లోపల_స్తంభం >= తదుపరి_ఉపసర్గ_indent + ఉపసర్గ_నిండిన_నిడివి
          && c != '\n' && c != EOF);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

స్థిర పూర్ణాంకం
get_పంక్తి (FILE *f, పూర్ణాంకం c)
{
  పూర్ణాంకం ప్రారంభం;
  అక్షరం *చివర_యొక్క_parabuf;
  మాట *చివర_యొక్క_మాట;

  చివర_యొక్క_parabuf = &parabuf[MAXCHARS];
  చివర_యొక్క_మాట = &మాట_2[MAXWORDS - 2];

  చేయి
    {				/* for each word in a line */

      /* Scan word.  */

      మాట_హద్దు->పాఠ్యం = wptr;
      చేయి
        {
          ఒకవేళ (wptr == చివర_యొక్క_parabuf)
            {
              సమితి_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      అయితే (c != EOF && !c_isspace (c));
      లోపల_స్తంభం += మాట_హద్దు->నిడివి = wptr - మాట_హద్దు->పాఠ్యం;
      check_punctuation (మాట_హద్దు);

      /* Scan inter-word space.  */

      ప్రారంభం = లోపల_స్తంభం;
      c = get_space (f, c);
      మాట_హద్దు->space = లోపల_స్తంభం - ప్రారంభం;
      మాట_హద్దు->final = (c == EOF
                           || (మాట_హద్దు->period
                               && (c == '\n' || మాట_హద్దు->space > 1)));
      ఒకవేళ (c == '\n' || c == EOF || ఏకరీతి)
        మాట_హద్దు->space = మాట_హద్దు->final ? 2 : 1;
      ఒకవేళ (మాట_హద్దు == చివర_యొక్క_మాట)
        {
          సమితి_other_indent (true);
          flush_paragraph ();
        }
      మాట_హద్దు++;
    }
  అయితే (c != '\n' && c != EOF);
  తిరిగి get_ఉపసర్గ (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

స్థిర పూర్ణాంకం
get_ఉపసర్గ (FILE *f)
{
  పూర్ణాంకం c;

  లోపల_స్తంభం = 0;
  c = get_space (f, getc (f));
  ఒకవేళ (ఉపసర్గ_నిడివి == 0)
    తదుపరి_ఉపసర్గ_indent = ఉపసర్గ_lead_space < లోపల_స్తంభం ?
      ఉపసర్గ_lead_space : లోపల_స్తంభం;
  లేకపోతే
    {
      తదుపరి_ఉపసర్గ_indent = లోపల_స్తంభం;
      కొరకు (అక్షరం స్థిరరాశి *p = ఉపసర్గ; *p != '\0'; p++)
        {
          గుర్తులేని అక్షరం pc = *p;
          ఒకవేళ (c != pc)
            తిరిగి c;
          లోపల_స్తంభం++;
          c = getc (f);
        }
      c = get_space (f, c);
    }
  తిరిగి c;
}

/* Read blank characters from input file F, starting with C, and keeping
   in_column up-to-date.  Return first non-blank character.  */

స్థిర పూర్ణాంకం
get_space (FILE *f, పూర్ణాంకం c)
{
  అయితే (true)
    {
      ఒకవేళ (c == ' ')
        లోపల_స్తంభం++;
      లేకపోతే ఒకవేళ (c == '\t')
        {
          tabs = true;
          లోపల_స్తంభం = (లోపల_స్తంభం / TABWIDTH + 1) * TABWIDTH;
        }
      లేకపోతే
        తిరిగి c;
      c = getc (f);
    }
}

/* Set extra fields in word W describing any attached punctuation.  */

స్థిర శూన్య
check_punctuation (మాట *w)
{
  అక్షరం స్థిరరాశి *ప్రారంభం = w->పాఠ్యం;
  అక్షరం స్థిరరాశి *ముగింపు = ప్రారంభం + (w->నిడివి - 1);
  గుర్తులేని అక్షరం fin = *ముగింపు;

  w->paren = isopen (*ప్రారంభం);
  w->punct = !! ispunct (fin);
  అయితే (ప్రారంభం < ముగింపు && isclose (*ముగింపు))
    ముగింపు--;
  w->period = isperiod (*ముగింపు);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

స్థిర శూన్య
flush_paragraph (శూన్య)
{
  మాట *విభజించు_బిందువు;
  మాట *w;
  పూర్ణాంకం జరుపు;
  ఖర్చు ఉత్తమ_break;

  /* In the special case where it's all one word, just flush it.  */

  ఒకవేళ (మాట_హద్దు == మాట_2)
    {
      size_t గమ్యం_write = wptr - parabuf;
      ఒకవేళ (fwrite (parabuf, 1, గమ్యం_write, stdout) != గమ్యం_write)
        write_error ();

      wptr = parabuf;
      తిరిగి;
    }

  /* Otherwise:
     - format what you have so far as a paragraph,
     - find a low-cost line break near the end,
     - output to there,
     - make that the start of the paragraph.  */

  fmt_paragraph ();

  /* Choose a good split point.  */

  విభజించు_బిందువు = మాట_హద్దు;
  ఉత్తమ_break = MAXCOST;
  కొరకు (w = మాట_2->తదుపరి_break; w != మాట_హద్దు; w = w->తదుపరి_break)
    {
      ఒకవేళ (w->ఉత్తమ_ఖర్చు - w->తదుపరి_break->ఉత్తమ_ఖర్చు < ఉత్తమ_break)
        {
          విభజించు_బిందువు = w;
          ఉత్తమ_break = w->ఉత్తమ_ఖర్చు - w->తదుపరి_break->ఉత్తమ_ఖర్చు;
        }
      ఒకవేళ (ఉత్తమ_break <= MAXCOST - పంక్తి_CREDIT)
        ఉత్తమ_break += పంక్తి_CREDIT;
    }
  put_paragraph (విభజించు_బిందువు);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, విభజించు_బిందువు->పాఠ్యం, wptr - విభజించు_బిందువు->పాఠ్యం);
  జరుపు = విభజించు_బిందువు->పాఠ్యం - parabuf;
  wptr -= జరుపు;

  /* Adjust text pointers.  */

  కొరకు (w = విభజించు_బిందువు; w <= మాట_హద్దు; w++)
    w->పాఠ్యం -= జరుపు;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (మాట_2, విభజించు_బిందువు, (మాట_హద్దు - విభజించు_బిందువు + 1) * పరిమాణం *మాట_2);
  మాట_హద్దు -= విభజించు_బిందువు - మాట_2;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

స్థిర శూన్య
fmt_paragraph (శూన్య)
{
  మాట *w;
  పూర్ణాంకం len;
  ఖర్చు wcost, ఉత్తమ;
  పూర్ణాంకం saved_నిడివి;

  మాట_హద్దు->ఉత్తమ_ఖర్చు = 0;
  saved_నిడివి = మాట_హద్దు->నిడివి;
  మాట_హద్దు->నిడివి = max_వెడల్పు;	/* sentinel */

  కొరకు (మాట *ప్రారంభం = మాట_హద్దు - 1; ప్రారంభం >= మాట_2; ప్రారంభం--)
    {
      ఉత్తమ = MAXCOST;
      len = ప్రారంభం == మాట_2 ? మొదటి_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = ప్రారంభం;
      len += w->నిడివి;
      చేయి
        {
          w++;

          /* Consider breaking before w.  */

          wcost = పంక్తి_ఖర్చు_2 (w, len) + w->ఉత్తమ_ఖర్చు;
          ఒకవేళ (ప్రారంభం == మాట_2 && చివరి_పంక్తి_నిడివి > 0)
            wcost += RAGGED_ఖర్చు (len - చివరి_పంక్తి_నిడివి);
          ఒకవేళ (wcost < ఉత్తమ)
            {
              ఉత్తమ = wcost;
              ప్రారంభం->తదుపరి_break = w;
              ప్రారంభం->పంక్తి_నిడివి = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          ఒకవేళ (w == మాట_హద్దు)
            ఆపు;

          len += (w - 1)->space + w->నిడివి;	/* w > start >= word */
        }
      అయితే (len <= max_వెడల్పు);
      ప్రారంభం->ఉత్తమ_ఖర్చు = ఉత్తమ + పునాది_ఖర్చు (ప్రారంభం);
    }

  మాట_హద్దు->నిడివి = saved_నిడివి;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#ఒకవేళ __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#అయితే_ముగింపు

/* Return the constant component of the cost of breaking before the
   word THIS.  */

స్థిర ఖర్చు
పునాది_ఖర్చు (మాట *this)
{
  ఖర్చు ఖర్చు_2;

  ఖర్చు_2 = పంక్తి_ఖర్చు;

  ఒకవేళ (this > మాట_2)
    {
      ఒకవేళ ((this - 1)->period)
        {
          ఒకవేళ ((this - 1)->final)
            ఖర్చు_2 -= SENTENCE_BONUS;
          లేకపోతే
            ఖర్చు_2 += NOBREAK_ఖర్చు;
        }
      లేకపోతే ఒకవేళ ((this - 1)->punct)
        ఖర్చు_2 -= PUNCT_BONUS;
      లేకపోతే ఒకవేళ (this > మాట_2 + 1 && (this - 2)->final)
        ఖర్చు_2 += WIDOW_ఖర్చు ((this - 1)->నిడివి);
    }

  ఒకవేళ (this->paren)
    ఖర్చు_2 -= PAREN_BONUS;
  లేకపోతే ఒకవేళ (this->final)
    ఖర్చు_2 += ORPHAN_ఖర్చు (this->నిడివి);

  తిరిగి ఖర్చు_2;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

స్థిర ఖర్చు
పంక్తి_ఖర్చు_2 (మాట *తదుపరి, పూర్ణాంకం len)
{
  పూర్ణాంకం n;
  ఖర్చు ఖర్చు_2;

  ఒకవేళ (తదుపరి == మాట_హద్దు)
    తిరిగి 0;
  n = goal_వెడల్పు - len;
  ఖర్చు_2 = SHORT_ఖర్చు (n);
  ఒకవేళ (తదుపరి->తదుపరి_break != మాట_హద్దు)
    {
      n = len - తదుపరి->పంక్తి_నిడివి;
      ఖర్చు_2 += RAGGED_ఖర్చు (n);
    }
  తిరిగి ఖర్చు_2;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

స్థిర శూన్య
put_paragraph (మాట *ముగింపు)
{
  put_పంక్తి (మాట_2, మొదటి_indent);
  కొరకు (మాట *w = మాట_2->తదుపరి_break; w != ముగింపు; w = w->తదుపరి_break)
    put_పంక్తి (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

స్థిర శూన్య
put_పంక్తి (మాట *w, పూర్ణాంకం indent)
{
  మాట *endline;

  బహిర్గతం_స్తంభం = 0;
  put_space (ఉపసర్గ_indent);
  fputs (ఉపసర్గ, stdout);
  బహిర్గతం_స్తంభం += ఉపసర్గ_నిడివి;
  put_space (indent - బహిర్గతం_స్తంభం);

  endline = w->తదుపరి_break - 1;
  కొరకు (; w != endline; w++)
    {
      put_మాట (w);
      put_space (w->space);
    }
  put_మాట (w);
  చివరి_పంక్తి_నిడివి = బహిర్గతం_స్తంభం;
  అక్షరం_రాయి ('\n');

  ఒకవేళ (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

స్థిర శూన్య
put_మాట (మాట *w)
{
  అక్షరం స్థిరరాశి *s = w->పాఠ్యం;
  కొరకు (పూర్ణాంకం n = w->నిడివి; n != 0; n--)
    అక్షరం_రాయి (*s++);
  బహిర్గతం_స్తంభం += w->నిడివి;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

స్థిర శూన్య
put_space (పూర్ణాంకం space)
{
  పూర్ణాంకం space_లక్ష్యం, tab_లక్ష్యం;

  space_లక్ష్యం = బహిర్గతం_స్తంభం + space;
  ఒకవేళ (tabs)
    {
      tab_లక్ష్యం = space_లక్ష్యం / TABWIDTH * TABWIDTH;
      ఒకవేళ (బహిర్గతం_స్తంభం + 1 < tab_లక్ష్యం)
        అయితే (బహిర్గతం_స్తంభం < tab_లక్ష్యం)
          {
            అక్షరం_రాయి ('\t');
            బహిర్గతం_స్తంభం = (బహిర్గతం_స్తంభం / TABWIDTH + 1) * TABWIDTH;
          }
    }
  అయితే (బహిర్గతం_స్తంభం < space_లక్ష్యం)
    {
      అక్షరం_రాయి (' ');
      బహిర్గతం_స్తంభం++;
    }
}
