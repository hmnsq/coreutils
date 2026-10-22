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
#وصف_ڪريو لفظ_2 اڻ_استعمال_ٿيل_لفظ_type

#ضمن "c-ctype.h"
#ضمن "system.h"
#ضمن "fadvise.h"
#ضمن "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "fmt"

#وصف_ڪريو AUTHORS proper_name ("Ross Paterson")

/* The following parameters represent the program's idea of what is
   "best".  Adjust to taste, subject to the caveats given.  */

/* Default longest permitted line length (max_width).  */
#وصف_ڪريو ويڪر_2	75

/* Prefer lines to be LEEWAY % shorter than the maximum width, giving
   room for optimization.  */
#وصف_ڪريو LEEWAY	7

/* The default secondary indent of tagged paragraph used for unindented
   one-line paragraphs not preceded by any multi-line paragraphs.  */
#وصف_ڪريو DEF_INDENT 3

/* Costs and bonuses are expressed as the equivalent departure from the
   optimal line length, multiplied by 10.  e.g. assigning something a
   cost of 50 means that it is as bad as a line 5 characters too short
   or too long.  The definition of SHORT_COST(n) should not be changed.
   However, EQUIV(n) may need tuning.  */

/* FIXME: "fmt" misbehaves given large inputs or options.  One
   possible workaround for part of the problem is to change COST to be
   a floating-point type.  There are other problems besides COST,
   though; see MAXWORDS below.  */

تعريفنوع ڊگهو صحيح لاڳت;

#وصف_ڪريو MAXCOST	TYPE_وڌ_کان_وڌ (لاڳت)

#وصف_ڪريو SQR(n)		((n) * (n))
#وصف_ڪريو EQUIV(n)	SQR ((لاڳت) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#وصف_ڪريو SHORT_لاڳت(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#وصف_ڪريو RAGGED_لاڳت(n)	(SHORT_لاڳت (n) / 2)

/* Basic cost per line.  */
#وصف_ڪريو سٽ_لاڳت	EQUIV (70)

/* Cost of breaking a line after the first word of a sentence, where
   the length of the word is N.  */
#وصف_ڪريو WIDOW_لاڳت(n)	(EQUIV (200) / ((n) + 2))

/* Cost of breaking a line before the last word of a sentence, where
   the length of the word is N.  */
#وصف_ڪريو ORPHAN_لاڳت(n)	(EQUIV (150) / ((n) + 2))

/* Bonus for breaking a line at the end of a sentence.  */
#وصف_ڪريو SENTENCE_BONUS	EQUIV (50)

/* Cost of breaking a line after a period not marking end of a sentence.
   With the definition of sentence we are using (borrowed from emacs, see
   get_line()) such a break would then look like a sentence break.  Hence
   we assign a very high cost -- it should be avoided unless things are
   really bad.  */
#وصف_ڪريو NOBREAK_لاڳت	EQUIV (600)

/* Bonus for breaking a line before open parenthesis.  */
#وصف_ڪريو PAREN_BONUS	EQUIV (40)

/* Bonus for breaking a line after other punctuation.  */
#وصف_ڪريو PUNCT_BONUS	EQUIV(40)

/* Credit for breaking a long paragraph one line later.  */
#وصف_ڪريو سٽ_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#وصف_ڪريو MAXWORDS	1000
#وصف_ڪريو MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#وصف_ڪريو isopen(c)	(strchr ("(['`\"", c) != عدم)
#وصف_ڪريو isclose(c)	(strchr (")]'\"", c) != عدم)
#وصف_ڪريو isperiod(c)	(strchr (".?!", c) != عدم)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#وصف_ڪريو TABWIDTH	8

/* Word descriptor structure.  */

تعريفنوع بنية لفظ_3 لفظ;

بنية لفظ_3
  {

    /* Static attributes determined during input.  */

    حرف ثابت *متن;		/* the text of the word */
    صحيح ڊيگهائي;			/* length of this word */
    صحيح space;			/* the size of the following space */
    بنانشان صحيح paren:1;	/* starts with open paren */
    بنانشان صحيح period:1;	/* ends in [.?!])* */
    بنانشان صحيح punct:1;	/* ends in punctuation */
    بنانشان صحيح final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    صحيح سٽ_ڊيگهائي;		/* length of the best line starting here */
    لاڳت بهترين_لاڳت;		/* cost of best paragraph starting here */
    لفظ *ايندڙ_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

ساكن فراغ سيٽ_اڳياڙي (حرف *p);
ساكن bool fmt (FILE *f, حرف ثابت *);
ساكن bool get_paragraph (FILE *f);
ساكن صحيح get_سٽ (FILE *f, صحيح c);
ساكن صحيح get_اڳياڙي (FILE *f);
ساكن صحيح get_space (FILE *f, صحيح c);
ساكن صحيح copy_rest (FILE *f, صحيح c);
ساكن bool same_para (صحيح c);
ساكن فراغ flush_paragraph (فراغ);
ساكن فراغ fmt_paragraph (فراغ);
ساكن فراغ check_punctuation (لفظ *w);
ساكن لاڳت بنياد_لاڳت (لفظ *this);
ساكن لاڳت سٽ_لاڳت_2 (لفظ *ايندڙ, صحيح len);
ساكن فراغ put_paragraph (لفظ *پڄاڻي);
ساكن فراغ put_سٽ (لفظ *w, صحيح indent);
ساكن فراغ put_لفظ (لفظ *w);
ساكن فراغ put_space (صحيح space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
ساكن bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
ساكن bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
ساكن bool ورهايو;

/* If true, don't preserve inter-word spacing (default false).  */
ساكن bool هڪجهڙو;

/* Prefix minus leading and trailing spaces (default "").  */
ساكن حرف ثابت *اڳياڙي = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
ساكن صحيح max_ويڪر = ويڪر_2;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
ساكن صحيح اڳياڙي_ڀريل_ڊيگهائي;

/* The length of the leading space trimmed from the prefix.  */
ساكن صحيح اڳياڙي_lead_space;

/* The length of prefix minus leading and trailing space.  */
ساكن صحيح اڳياڙي_ڊيگهائي;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
ساكن صحيح goal_ويڪر;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
ساكن صحيح اندريون_ڪالم;

/* Start column of the next character to be written to stdout.  */
ساكن صحيح ٻاهريون_ڪالم;

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
ساكن لفظ *لفظ_حد_بندي;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
ساكن bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
ساكن صحيح اڳياڙي_indent;

/* Indentation of the first line of the current paragraph.  */
ساكن صحيح پهريون_indent;

/* Indentation of other lines of the current paragraph */
ساكن صحيح other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
ساكن صحيح ايندڙ_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
ساكن صحيح ايندڙ_اڳياڙي_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
ساكن صحيح آخري_سٽ_ڊيگهائي;

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("Usage: %s [-WIDTH] [OPTION]... [FILE]...\n"), پروگرام_name);
      fputs (_("\
Reformat each paragraph اندريون the FILE(s), writing منزل standard output.\n\
The option -ويڪر_2 is an abbreviated form جو --ويڪر=DIGITS.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --crown-margin\n\
         preserve indentation جو پهريون two سٽون\n\
"));
      oputs (_("\
  -p, --اڳياڙي=STRING\n\
         reformat only سٽون beginning with STRING,\n\
         reattaching the اڳياڙي منزل reformatted سٽون\n\
"));
      oputs (_("\
  -s, --ورهايو-only\n\
         ورهايو ڊگهو سٽون, but افعل not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation جو پهريون سٽ different from ٻيو\n\
"));
      oputs (_("\
  -u, --هڪجهڙو-spacing\n\
         one space between لفظ_4, two after sentences\n\
"));
      oputs (_("\
  -w, --ويڪر=ويڪر_2\n\
         وڌ_کان_وڌ سٽ ويڪر (افتراضي جو 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=ويڪر_2\n\
         goal ويڪر (افتراضي جو 93% جو ويڪر)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Decode options and launch execution.  */

ساكن بنية option ثابت long_options[] =
{
  {"crown-margin", no_دليل, عدم, 'c'},
  {"prefix", required_دليل, عدم, 'p'},
  {"split-only", no_دليل, عدم, 's'},
  {"tagged-paragraph", no_دليل, عدم, 't'},
  {"uniform-spacing", no_دليل, عدم, 'u'},
  {"width", required_دليل, عدم, 'w'},
  {"goal", required_دليل, عدم, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح optchar;
  bool ok = true;
  حرف ثابت *max_ويڪر_option = عدم;
  حرف ثابت *goal_ويڪر_option = عدم;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  إذا (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_ويڪر_option = argv[1] + 1;

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
          error (0, 0, _("invalid option -- %c; -ويڪر_2 is recognized\
 only when it is the پهريون\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      حالة 'c':
        crown = true;
        اكسر;

      حالة 's':
        ورهايو = true;
        اكسر;

      حالة 't':
        tagged = true;
        اكسر;

      حالة 'u':
        هڪجهڙو = true;
        اكسر;

      حالة 'w':
        max_ويڪر_option = optarg;
        اكسر;

      حالة 'g':
        goal_ويڪر_option = optarg;
        اكسر;

      حالة 'p':
        سيٽ_اڳياڙي (optarg);
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

      }

  إذا (max_ويڪر_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_ويڪر = xnumtoumax (max_ويڪر_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  إذا (goal_ويڪر_option)
    {
      /* Limit goal_width to max_width.  */
      goal_ويڪر = xdectoumax (goal_ويڪر_option, 0, max_ويڪر, "",
                               _("invalid goal"), 0);
      إذا (max_ويڪر_option == عدم)
        max_ويڪر = goal_ويڪر + 10;
    }
  وإلا
    {
      goal_ويڪر = max_ويڪر * (2 * (100 - LEEWAY) + 1) / 200;
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
              FILE *اندريون_وهڪرو;
              اندريون_وهڪرو = fopen (file, "r");
              إذا (اندريون_وهڪرو != عدم)
                ok &= fmt (اندريون_وهڪرو, file);
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
سيٽ_اڳياڙي (حرف *p)
{
  حرف *s;

  اڳياڙي_lead_space = 0;
  طالما (*p == ' ')
    {
      اڳياڙي_lead_space++;
      p++;
    }
  اڳياڙي = p;
  اڳياڙي_ڀريل_ڊيگهائي = لڙي_ڊيگهه (p);
  s = p + اڳياڙي_ڀريل_ڊيگهائي;
  طالما (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  اڳياڙي_ڊيگهائي = s - p;
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
  ايندڙ_char = get_اڳياڙي (f);
  طالما (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (لفظ_حد_بندي);
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
سيٽ_other_indent (bool same_paragraph)
{
  إذا (ورهايو)
    other_indent = پهريون_indent;
  وإلا إذا (crown)
    {
      other_indent = (same_paragraph ? اندريون_ڪالم : پهريون_indent);
    }
  وإلا إذا (tagged)
    {
      إذا (same_paragraph && اندريون_ڪالم != پهريون_indent)
        {
          other_indent = اندريون_ڪالم;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      وإلا إذا (other_indent == پهريون_indent)
        other_indent = پهريون_indent == 0 ? DEF_INDENT : 0;
    }
  وإلا
    {
      other_indent = پهريون_indent;
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

  آخري_سٽ_ڊيگهائي = 0;
  c = ايندڙ_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  طالما (c == '\n' || c == EOF
         || ايندڙ_اڳياڙي_indent < اڳياڙي_lead_space
         || اندريون_ڪالم < ايندڙ_اڳياڙي_indent + اڳياڙي_ڀريل_ڊيگهائي)
    {
      c = copy_rest (f, c);
      إذا (c == EOF)
        {
          ايندڙ_char = EOF;
          أرجع false;
        }
      اکر_لکو ('\n');
      c = get_اڳياڙي (f);
    }

  /* Got a suitable first line for a paragraph.  */

  اڳياڙي_indent = ايندڙ_اڳياڙي_indent;
  پهريون_indent = اندريون_ڪالم;
  wptr = parabuf;
  لفظ_حد_بندي = لفظ_2;
  c = get_سٽ (f, c);
  سيٽ_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  إذا (ورهايو)
    {
      /*خالي_جايون*/
    }
  وإلا إذا (crown)
    {
      إذا (same_para (c))
        {
          افعل
            {			/* for each line till the end of the para */
              c = get_سٽ (f, c);
            }
          طالما (same_para (c) && اندريون_ڪالم == other_indent);
        }
    }
  وإلا إذا (tagged)
    {
      إذا (same_para (c) && اندريون_ڪالم != پهريون_indent)
        {
          افعل
            {			/* for each line till the end of the para */
              c = get_سٽ (f, c);
            }
          طالما (same_para (c) && اندريون_ڪالم == other_indent);
        }
    }
  وإلا
    {
      طالما (same_para (c) && اندريون_ڪالم == other_indent)
        c = get_سٽ (f, c);
    }

  (لفظ_حد_بندي - 1)->period = (لفظ_حد_بندي - 1)->final = true;
  ايندڙ_char = c;
  أرجع true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

ساكن صحيح
copy_rest (FILE *f, صحيح c)
{
  ٻاهريون_ڪالم = 0;
  إذا (اندريون_ڪالم > ايندڙ_اڳياڙي_indent || (c != '\n' && c != EOF))
    {
      put_space (ايندڙ_اڳياڙي_indent);
      لكل (حرف ثابت *s = اڳياڙي; ٻاهريون_ڪالم != اندريون_ڪالم && *s; ٻاهريون_ڪالم++)
        اکر_لکو (*s++);
      إذا (c != EOF && c != '\n')
        put_space (اندريون_ڪالم - ٻاهريون_ڪالم);
      إذا (c == EOF && اندريون_ڪالم >= ايندڙ_اڳياڙي_indent + اڳياڙي_ڊيگهائي)
        اکر_لکو ('\n');
    }
  طالما (c != '\n' && c != EOF)
    {
      اکر_لکو (c);
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
  أرجع (ايندڙ_اڳياڙي_indent == اڳياڙي_indent
          && اندريون_ڪالم >= ايندڙ_اڳياڙي_indent + اڳياڙي_ڀريل_ڊيگهائي
          && c != '\n' && c != EOF);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

ساكن صحيح
get_سٽ (FILE *f, صحيح c)
{
  صحيح شروعات;
  حرف *پڇاڙي_جو_parabuf;
  لفظ *پڇاڙي_جو_لفظ;

  پڇاڙي_جو_parabuf = &parabuf[MAXCHARS];
  پڇاڙي_جو_لفظ = &لفظ_2[MAXWORDS - 2];

  افعل
    {				/* for each word in a line */

      /* Scan word.  */

      لفظ_حد_بندي->متن = wptr;
      افعل
        {
          إذا (wptr == پڇاڙي_جو_parabuf)
            {
              سيٽ_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      طالما (c != EOF && !c_isspace (c));
      اندريون_ڪالم += لفظ_حد_بندي->ڊيگهائي = wptr - لفظ_حد_بندي->متن;
      check_punctuation (لفظ_حد_بندي);

      /* Scan inter-word space.  */

      شروعات = اندريون_ڪالم;
      c = get_space (f, c);
      لفظ_حد_بندي->space = اندريون_ڪالم - شروعات;
      لفظ_حد_بندي->final = (c == EOF
                           || (لفظ_حد_بندي->period
                               && (c == '\n' || لفظ_حد_بندي->space > 1)));
      إذا (c == '\n' || c == EOF || هڪجهڙو)
        لفظ_حد_بندي->space = لفظ_حد_بندي->final ? 2 : 1;
      إذا (لفظ_حد_بندي == پڇاڙي_جو_لفظ)
        {
          سيٽ_other_indent (true);
          flush_paragraph ();
        }
      لفظ_حد_بندي++;
    }
  طالما (c != '\n' && c != EOF);
  أرجع get_اڳياڙي (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

ساكن صحيح
get_اڳياڙي (FILE *f)
{
  صحيح c;

  اندريون_ڪالم = 0;
  c = get_space (f, getc (f));
  إذا (اڳياڙي_ڊيگهائي == 0)
    ايندڙ_اڳياڙي_indent = اڳياڙي_lead_space < اندريون_ڪالم ?
      اڳياڙي_lead_space : اندريون_ڪالم;
  وإلا
    {
      ايندڙ_اڳياڙي_indent = اندريون_ڪالم;
      لكل (حرف ثابت *p = اڳياڙي; *p != '\0'; p++)
        {
          بنانشان حرف pc = *p;
          إذا (c != pc)
            أرجع c;
          اندريون_ڪالم++;
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
        اندريون_ڪالم++;
      وإلا إذا (c == '\t')
        {
          tabs = true;
          اندريون_ڪالم = (اندريون_ڪالم / TABWIDTH + 1) * TABWIDTH;
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
  حرف ثابت *شروعات = w->متن;
  حرف ثابت *پڄاڻي = شروعات + (w->ڊيگهائي - 1);
  بنانشان حرف fin = *پڄاڻي;

  w->paren = isopen (*شروعات);
  w->punct = !! ispunct (fin);
  طالما (شروعات < پڄاڻي && isclose (*پڄاڻي))
    پڄاڻي--;
  w->period = isperiod (*پڄاڻي);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

ساكن فراغ
flush_paragraph (فراغ)
{
  لفظ *ورهايو_نقطو;
  لفظ *w;
  صحيح سرڪايو;
  لاڳت بهترين_break;

  /* In the special case where it's all one word, just flush it.  */

  إذا (لفظ_حد_بندي == لفظ_2)
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

  ورهايو_نقطو = لفظ_حد_بندي;
  بهترين_break = MAXCOST;
  لكل (w = لفظ_2->ايندڙ_break; w != لفظ_حد_بندي; w = w->ايندڙ_break)
    {
      إذا (w->بهترين_لاڳت - w->ايندڙ_break->بهترين_لاڳت < بهترين_break)
        {
          ورهايو_نقطو = w;
          بهترين_break = w->بهترين_لاڳت - w->ايندڙ_break->بهترين_لاڳت;
        }
      إذا (بهترين_break <= MAXCOST - سٽ_CREDIT)
        بهترين_break += سٽ_CREDIT;
    }
  put_paragraph (ورهايو_نقطو);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, ورهايو_نقطو->متن, wptr - ورهايو_نقطو->متن);
  سرڪايو = ورهايو_نقطو->متن - parabuf;
  wptr -= سرڪايو;

  /* Adjust text pointers.  */

  لكل (w = ورهايو_نقطو; w <= لفظ_حد_بندي; w++)
    w->متن -= سرڪايو;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (لفظ_2, ورهايو_نقطو, (لفظ_حد_بندي - ورهايو_نقطو + 1) * حجم *لفظ_2);
  لفظ_حد_بندي -= ورهايو_نقطو - لفظ_2;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

ساكن فراغ
fmt_paragraph (فراغ)
{
  لفظ *w;
  صحيح len;
  لاڳت wcost, بهترين;
  صحيح saved_ڊيگهائي;

  لفظ_حد_بندي->بهترين_لاڳت = 0;
  saved_ڊيگهائي = لفظ_حد_بندي->ڊيگهائي;
  لفظ_حد_بندي->ڊيگهائي = max_ويڪر;	/* sentinel */

  لكل (لفظ *شروعات = لفظ_حد_بندي - 1; شروعات >= لفظ_2; شروعات--)
    {
      بهترين = MAXCOST;
      len = شروعات == لفظ_2 ? پهريون_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = شروعات;
      len += w->ڊيگهائي;
      افعل
        {
          w++;

          /* Consider breaking before w.  */

          wcost = سٽ_لاڳت_2 (w, len) + w->بهترين_لاڳت;
          إذا (شروعات == لفظ_2 && آخري_سٽ_ڊيگهائي > 0)
            wcost += RAGGED_لاڳت (len - آخري_سٽ_ڊيگهائي);
          إذا (wcost < بهترين)
            {
              بهترين = wcost;
              شروعات->ايندڙ_break = w;
              شروعات->سٽ_ڊيگهائي = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          إذا (w == لفظ_حد_بندي)
            اكسر;

          len += (w - 1)->space + w->ڊيگهائي;	/* w > start >= word */
        }
      طالما (len <= max_ويڪر);
      شروعات->بهترين_لاڳت = بهترين + بنياد_لاڳت (شروعات);
    }

  لفظ_حد_بندي->ڊيگهائي = saved_ڊيگهائي;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#إذا __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#جيڪڏهن_پورو

/* Return the constant component of the cost of breaking before the
   word THIS.  */

ساكن لاڳت
بنياد_لاڳت (لفظ *this)
{
  لاڳت لاڳت_2;

  لاڳت_2 = سٽ_لاڳت;

  إذا (this > لفظ_2)
    {
      إذا ((this - 1)->period)
        {
          إذا ((this - 1)->final)
            لاڳت_2 -= SENTENCE_BONUS;
          وإلا
            لاڳت_2 += NOBREAK_لاڳت;
        }
      وإلا إذا ((this - 1)->punct)
        لاڳت_2 -= PUNCT_BONUS;
      وإلا إذا (this > لفظ_2 + 1 && (this - 2)->final)
        لاڳت_2 += WIDOW_لاڳت ((this - 1)->ڊيگهائي);
    }

  إذا (this->paren)
    لاڳت_2 -= PAREN_BONUS;
  وإلا إذا (this->final)
    لاڳت_2 += ORPHAN_لاڳت (this->ڊيگهائي);

  أرجع لاڳت_2;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

ساكن لاڳت
سٽ_لاڳت_2 (لفظ *ايندڙ, صحيح len)
{
  صحيح n;
  لاڳت لاڳت_2;

  إذا (ايندڙ == لفظ_حد_بندي)
    أرجع 0;
  n = goal_ويڪر - len;
  لاڳت_2 = SHORT_لاڳت (n);
  إذا (ايندڙ->ايندڙ_break != لفظ_حد_بندي)
    {
      n = len - ايندڙ->سٽ_ڊيگهائي;
      لاڳت_2 += RAGGED_لاڳت (n);
    }
  أرجع لاڳت_2;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

ساكن فراغ
put_paragraph (لفظ *پڄاڻي)
{
  put_سٽ (لفظ_2, پهريون_indent);
  لكل (لفظ *w = لفظ_2->ايندڙ_break; w != پڄاڻي; w = w->ايندڙ_break)
    put_سٽ (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

ساكن فراغ
put_سٽ (لفظ *w, صحيح indent)
{
  لفظ *endline;

  ٻاهريون_ڪالم = 0;
  put_space (اڳياڙي_indent);
  fputs (اڳياڙي, stdout);
  ٻاهريون_ڪالم += اڳياڙي_ڊيگهائي;
  put_space (indent - ٻاهريون_ڪالم);

  endline = w->ايندڙ_break - 1;
  لكل (; w != endline; w++)
    {
      put_لفظ (w);
      put_space (w->space);
    }
  put_لفظ (w);
  آخري_سٽ_ڊيگهائي = ٻاهريون_ڪالم;
  اکر_لکو ('\n');

  إذا (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

ساكن فراغ
put_لفظ (لفظ *w)
{
  حرف ثابت *s = w->متن;
  لكل (صحيح n = w->ڊيگهائي; n != 0; n--)
    اکر_لکو (*s++);
  ٻاهريون_ڪالم += w->ڊيگهائي;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

ساكن فراغ
put_space (صحيح space)
{
  صحيح space_هدف, tab_هدف;

  space_هدف = ٻاهريون_ڪالم + space;
  إذا (tabs)
    {
      tab_هدف = space_هدف / TABWIDTH * TABWIDTH;
      إذا (ٻاهريون_ڪالم + 1 < tab_هدف)
        طالما (ٻاهريون_ڪالم < tab_هدف)
          {
            اکر_لکو ('\t');
            ٻاهريون_ڪالم = (ٻاهريون_ڪالم / TABWIDTH + 1) * TABWIDTH;
          }
    }
  طالما (ٻاهريون_ڪالم < space_هدف)
    {
      اکر_لکو (' ');
      ٻاهريون_ڪالم++;
    }
}
