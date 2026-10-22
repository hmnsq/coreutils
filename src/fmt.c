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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <getopt.h>

/* Redefine.  Otherwise, systems (Unicos for one) with headers that define
   it to be a type get syntax errors for the variable declaration below.  */
#define word unused_word_type

#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "fmt"

#define AUTHORS proper_name ("Ross Paterson")

/* The following parameters represent the program's idea of what is
   "best".  Adjust to taste, subject to the caveats given.  */

/* Default longest permitted line length (max_width).  */
#define WIDTH	75

/* Prefer lines to be LEEWAY % shorter than the maximum width, giving
   room for optimization.  */
#define LEEWAY	7

/* The default secondary indent of tagged paragraph used for unindented
   one-line paragraphs not preceded by any multi-line paragraphs.  */
#define DEF_INDENT 3

/* Costs and bonuses are expressed as the equivalent departure from the
   optimal line length, multiplied by 10.  e.g. assigning something a
   cost of 50 means that it is as bad as a line 5 characters too short
   or too long.  The definition of SHORT_COST(n) should not be changed.
   However, EQUIV(n) may need tuning.  */

/* FIXME: "fmt" misbehaves given large inputs or options.  One
   possible workaround for part of the problem is to change COST to be
   a floating-point type.  There are other problems besides COST,
   though; see MAXWORDS below.  */

প্রকারলেখ long পূর্ণসংখ্যা COST;

#define MAXCOST	TYPE_MAXIMUM (COST)

#define SQR(n)		((n) * (n))
#define EQUIV(n)	SQR ((COST) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#define SHORT_COST(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#define RAGGED_COST(n)	(SHORT_COST (n) / 2)

/* Basic cost per line.  */
#define সারি_COST	EQUIV (70)

/* Cost of breaking a line after the first word of a sentence, where
   the length of the word is N.  */
#define WIDOW_COST(n)	(EQUIV (200) / ((n) + 2))

/* Cost of breaking a line before the last word of a sentence, where
   the length of the word is N.  */
#define ORPHAN_COST(n)	(EQUIV (150) / ((n) + 2))

/* Bonus for breaking a line at the end of a sentence.  */
#define SENTENCE_BONUS	EQUIV (50)

/* Cost of breaking a line after a period not marking end of a sentence.
   With the definition of sentence we are using (borrowed from emacs, see
   get_line()) such a break would then look like a sentence break.  Hence
   we assign a very high cost -- it should be avoided unless things are
   really bad.  */
#define NOBREAK_COST	EQUIV (600)

/* Bonus for breaking a line before open parenthesis.  */
#define PAREN_BONUS	EQUIV (40)

/* Bonus for breaking a line after other punctuation.  */
#define PUNCT_BONUS	EQUIV(40)

/* Credit for breaking a long paragraph one line later.  */
#define সারি_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#define MAXWORDS	1000
#define MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#define isopen(c)	(strchr ("(['`\"", c) != ফাঁকা)
#define isclose(c)	(strchr (")]'\"", c) != ফাঁকা)
#define isperiod(c)	(strchr (".?!", c) != ফাঁকা)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#define TABWIDTH	8

/* Word descriptor structure.  */

প্রকারলেখ কাঠামো Word WORD;

কাঠামো Word
  {

    /* Static attributes determined during input.  */

    অক্ষর ধ্রুবক *text;		/* the text of the word */
    পূর্ণসংখ্যা দৈর্ঘ্য;			/* length of this word */
    পূর্ণসংখ্যা space;			/* the size of the following space */
    unsigned পূর্ণসংখ্যা paren:1;	/* starts with open paren */
    unsigned পূর্ণসংখ্যা period:1;	/* ends in [.?!])* */
    unsigned পূর্ণসংখ্যা punct:1;	/* ends in punctuation */
    unsigned পূর্ণসংখ্যা final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    পূর্ণসংখ্যা সারি_দৈর্ঘ্য;		/* length of the best line starting here */
    COST best_cost;		/* cost of best paragraph starting here */
    WORD *পরবর্তী_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

স্থির শূন্য set_prefix (অক্ষর *p);
স্থির bool fmt (FILE *f, অক্ষর ধ্রুবক *);
স্থির bool get_paragraph (FILE *f);
স্থির পূর্ণসংখ্যা get_সারি (FILE *f, পূর্ণসংখ্যা c);
স্থির পূর্ণসংখ্যা get_prefix (FILE *f);
স্থির পূর্ণসংখ্যা get_space (FILE *f, পূর্ণসংখ্যা c);
স্থির পূর্ণসংখ্যা copy_rest (FILE *f, পূর্ণসংখ্যা c);
স্থির bool same_para (পূর্ণসংখ্যা c);
স্থির শূন্য flush_paragraph (শূন্য);
স্থির শূন্য fmt_paragraph (শূন্য);
স্থির শূন্য check_punctuation (WORD *w);
স্থির COST base_cost (WORD *this);
স্থির COST সারি_cost (WORD *পরবর্তী, পূর্ণসংখ্যা len);
স্থির শূন্য put_paragraph (WORD *finish);
স্থির শূন্য put_সারি (WORD *w, পূর্ণসংখ্যা indent);
স্থির শূন্য put_word (WORD *w);
স্থির শূন্য put_space (পূর্ণসংখ্যা space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
স্থির bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
স্থির bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
স্থির bool split;

/* If true, don't preserve inter-word spacing (default false).  */
স্থির bool uniform;

/* Prefix minus leading and trailing spaces (default "").  */
স্থির অক্ষর ধ্রুবক *prefix = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
স্থির পূর্ণসংখ্যা max_width = WIDTH;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
স্থির পূর্ণসংখ্যা prefix_full_দৈর্ঘ্য;

/* The length of the leading space trimmed from the prefix.  */
স্থির পূর্ণসংখ্যা prefix_lead_space;

/* The length of prefix minus leading and trailing space.  */
স্থির পূর্ণসংখ্যা prefix_দৈর্ঘ্য;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
স্থির পূর্ণসংখ্যা goal_width;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
স্থির পূর্ণসংখ্যা in_column;

/* Start column of the next character to be written to stdout.  */
স্থির পূর্ণসংখ্যা out_column;

/* Space for the paragraph text -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
স্থির অক্ষর parabuf[MAXCHARS];

/* A pointer into parabuf, indicating the first unused character position.  */
স্থির অক্ষর *wptr;

/* The words of a paragraph -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
স্থির WORD word[MAXWORDS];

/* A pointer into the above word array, indicating the first position
   after the last complete word.  Sometimes it will point at an incomplete
   word.  */
স্থির WORD *word_limit;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
স্থির bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
স্থির পূর্ণসংখ্যা prefix_indent;

/* Indentation of the first line of the current paragraph.  */
স্থির পূর্ণসংখ্যা first_indent;

/* Indentation of other lines of the current paragraph */
স্থির পূর্ণসংখ্যা other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
স্থির পূর্ণসংখ্যা পরবর্তী_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
স্থির পূর্ণসংখ্যা পরবর্তী_prefix_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
স্থির পূর্ণসংখ্যা last_সারি_দৈর্ঘ্য;

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [-WIDTH] [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Reformat each paragraph in the FILE(s), writing to standard output.\n\
The option -WIDTH is an abbreviated form of --width=DIGITS.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --crown-margin\n\
         preserve indentation of first two সারিগুলি\n\
"));
      oputs (_("\
  -p, --prefix=STRING\n\
         reformat only সারিগুলি beginning with STRING,\n\
         reattaching the prefix to reformatted সারিগুলি\n\
"));
      oputs (_("\
  -s, --split-only\n\
         split long সারিগুলি, but করো not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation of first সারি different from second\n\
"));
      oputs (_("\
  -u, --uniform-spacing\n\
         one space between শব্দগুলি, two after sentences\n\
"));
      oputs (_("\
  -w, --width=WIDTH\n\
         maximum সারি width (পূর্বনির্ধারিত of 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=WIDTH\n\
         goal width (পূর্বনির্ধারিত of 93% of width)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Decode options and launch execution.  */

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"crown-margin", no_argument, ফাঁকা, 'c'},
  {"prefix", required_argument, ফাঁকা, 'p'},
  {"split-only", no_argument, ফাঁকা, 's'},
  {"tagged-paragraph", no_argument, ফাঁকা, 't'},
  {"uniform-spacing", no_argument, ফাঁকা, 'u'},
  {"width", required_argument, ফাঁকা, 'w'},
  {"goal", required_argument, ফাঁকা, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0},
};

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা optchar;
  bool ok = true;
  অক্ষর ধ্রুবক *max_width_option = ফাঁকা;
  অক্ষর ধ্রুবক *goal_width_option = ফাঁকা;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যদি (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_width_option = argv[1] + 1;

      /* Make the option we just parsed invisible to getopt.  */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  যখন ((optchar = getopt_long (argc, argv, "0123456789cstuw:p:g:",
                                 long_options, ফাঁকা))
         != -1)
    নির্বাচন (optchar)
      {
      পূর্বনির্ধারিত:
        যদি (c_isdigit (optchar))
          error (0, 0, _("invalid option -- %c; -WIDTH is recognized\
 only when it is the first\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      ক্ষেত্র 'c':
        crown = true;
        বিরতি;

      ক্ষেত্র 's':
        split = true;
        বিরতি;

      ক্ষেত্র 't':
        tagged = true;
        বিরতি;

      ক্ষেত্র 'u':
        uniform = true;
        বিরতি;

      ক্ষেত্র 'w':
        max_width_option = optarg;
        বিরতি;

      ক্ষেত্র 'g':
        goal_width_option = optarg;
        বিরতি;

      ক্ষেত্র 'p':
        set_prefix (optarg);
        বিরতি;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      }

  যদি (max_width_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_width = xnumtoumax (max_width_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  যদি (goal_width_option)
    {
      /* Limit goal_width to max_width.  */
      goal_width = xdectoumax (goal_width_option, 0, max_width, "",
                               _("invalid goal"), 0);
      যদি (max_width_option == ফাঁকা)
        max_width = goal_width + 10;
    }
  অন্যথা
    {
      goal_width = max_width * (2 * (100 - LEEWAY) + 1) / 200;
    }

  bool have_read_stdin = false;

  যদি (optind == argc)
    {
      have_read_stdin = true;
      ok = fmt (stdin, "-");
    }
  অন্যথা
    {
      জন্য (; optind < argc; optind++)
        {
          অক্ষর *file = argv[optind];
          যদি (streq (file, "-"))
            {
              ok &= fmt (stdin, file);
              have_read_stdin = true;
            }
          অন্যথা
            {
              FILE *in_stream;
              in_stream = fopen (file, "r");
              যদি (in_stream != ফাঁকা)
                ok &= fmt (in_stream, file);
              অন্যথা
                {
                  error (0, errno, _("cannot open %s for reading"),
                         quoteaf (file));
                  ok = false;
                }
            }
        }
    }

  যদি (have_read_stdin && fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s", _("closing standard input"));

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Trim space from the front and back of the string P, yielding the prefix,
   and record the lengths of the prefix and the space trimmed.  */

স্থির শূন্য
set_prefix (অক্ষর *p)
{
  অক্ষর *s;

  prefix_lead_space = 0;
  যখন (*p == ' ')
    {
      prefix_lead_space++;
      p++;
    }
  prefix = p;
  prefix_full_দৈর্ঘ্য = strlen (p);
  s = p + prefix_full_দৈর্ঘ্য;
  যখন (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  prefix_দৈর্ঘ্য = s - p;
}

/* Read F and send formatted output to stdout.
   Close F when done, unless F is stdin.  Diagnose input errors, using FILE.
   If !F, assume F resulted from an fopen failure and diagnose that.
   Return true if successful.  */

স্থির bool
fmt (FILE *f, অক্ষর ধ্রুবক *file)
{
  fadvise (f, FADVISE_SEQUENTIAL);
  tabs = false;
  other_indent = 0;
  পরবর্তী_char = get_prefix (f);
  যখন (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (word_limit);
    }

  পূর্ণসংখ্যা err = ferror (f) ? 0 : -1;
  /* err < 0 means success.  */
  যদি (f == stdin)
    clearerr (f);
  অন্যথা যদি (fclose (f) != 0 && err < 0)
    err = errno;
  যদি (0 <= err)
    {
      যদি (f == stdin)
        error (0, errno, _("read error"));
      অন্যথা
        error (0, errno, _("error reading %s"), quoteaf (file));
    }
  ফেরত err < 0;
}

/* Set the global variable 'other_indent' according to SAME_PARAGRAPH
   and other global variables.  */

স্থির শূন্য
set_other_indent (bool same_paragraph)
{
  যদি (split)
    other_indent = first_indent;
  অন্যথা যদি (crown)
    {
      other_indent = (same_paragraph ? in_column : first_indent);
    }
  অন্যথা যদি (tagged)
    {
      যদি (same_paragraph && in_column != first_indent)
        {
          other_indent = in_column;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      অন্যথা যদি (other_indent == first_indent)
        other_indent = first_indent == 0 ? DEF_INDENT : 0;
    }
  অন্যথা
    {
      other_indent = first_indent;
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

স্থির bool
get_paragraph (FILE *f)
{
  পূর্ণসংখ্যা c;

  last_সারি_দৈর্ঘ্য = 0;
  c = পরবর্তী_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  যখন (c == '\n' || c == EOF
         || পরবর্তী_prefix_indent < prefix_lead_space
         || in_column < পরবর্তী_prefix_indent + prefix_full_দৈর্ঘ্য)
    {
      c = copy_rest (f, c);
      যদি (c == EOF)
        {
          পরবর্তী_char = EOF;
          ফেরত false;
        }
      putchar ('\n');
      c = get_prefix (f);
    }

  /* Got a suitable first line for a paragraph.  */

  prefix_indent = পরবর্তী_prefix_indent;
  first_indent = in_column;
  wptr = parabuf;
  word_limit = word;
  c = get_সারি (f, c);
  set_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  যদি (split)
    {
      /* empty */
    }
  অন্যথা যদি (crown)
    {
      যদি (same_para (c))
        {
          করো
            {			/* for each line till the end of the para */
              c = get_সারি (f, c);
            }
          যখন (same_para (c) && in_column == other_indent);
        }
    }
  অন্যথা যদি (tagged)
    {
      যদি (same_para (c) && in_column != first_indent)
        {
          করো
            {			/* for each line till the end of the para */
              c = get_সারি (f, c);
            }
          যখন (same_para (c) && in_column == other_indent);
        }
    }
  অন্যথা
    {
      যখন (same_para (c) && in_column == other_indent)
        c = get_সারি (f, c);
    }

  (word_limit - 1)->period = (word_limit - 1)->final = true;
  পরবর্তী_char = c;
  ফেরত true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

স্থির পূর্ণসংখ্যা
copy_rest (FILE *f, পূর্ণসংখ্যা c)
{
  out_column = 0;
  যদি (in_column > পরবর্তী_prefix_indent || (c != '\n' && c != EOF))
    {
      put_space (পরবর্তী_prefix_indent);
      জন্য (অক্ষর ধ্রুবক *s = prefix; out_column != in_column && *s; out_column++)
        putchar (*s++);
      যদি (c != EOF && c != '\n')
        put_space (in_column - out_column);
      যদি (c == EOF && in_column >= পরবর্তী_prefix_indent + prefix_দৈর্ঘ্য)
        putchar ('\n');
    }
  যখন (c != '\n' && c != EOF)
    {
      putchar (c);
      c = getc (f);
    }
  ফেরত c;
}

/* Return true if a line whose first non-blank character after the
   prefix (if any) is C could belong to the current paragraph,
   otherwise false.  */

স্থির bool
same_para (পূর্ণসংখ্যা c)
{
  ফেরত (পরবর্তী_prefix_indent == prefix_indent
          && in_column >= পরবর্তী_prefix_indent + prefix_full_দৈর্ঘ্য
          && c != '\n' && c != EOF);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

স্থির পূর্ণসংখ্যা
get_সারি (FILE *f, পূর্ণসংখ্যা c)
{
  পূর্ণসংখ্যা start;
  অক্ষর *end_of_parabuf;
  WORD *end_of_word;

  end_of_parabuf = &parabuf[MAXCHARS];
  end_of_word = &word[MAXWORDS - 2];

  করো
    {				/* for each word in a line */

      /* Scan word.  */

      word_limit->text = wptr;
      করো
        {
          যদি (wptr == end_of_parabuf)
            {
              set_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      যখন (c != EOF && !c_isspace (c));
      in_column += word_limit->দৈর্ঘ্য = wptr - word_limit->text;
      check_punctuation (word_limit);

      /* Scan inter-word space.  */

      start = in_column;
      c = get_space (f, c);
      word_limit->space = in_column - start;
      word_limit->final = (c == EOF
                           || (word_limit->period
                               && (c == '\n' || word_limit->space > 1)));
      যদি (c == '\n' || c == EOF || uniform)
        word_limit->space = word_limit->final ? 2 : 1;
      যদি (word_limit == end_of_word)
        {
          set_other_indent (true);
          flush_paragraph ();
        }
      word_limit++;
    }
  যখন (c != '\n' && c != EOF);
  ফেরত get_prefix (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

স্থির পূর্ণসংখ্যা
get_prefix (FILE *f)
{
  পূর্ণসংখ্যা c;

  in_column = 0;
  c = get_space (f, getc (f));
  যদি (prefix_দৈর্ঘ্য == 0)
    পরবর্তী_prefix_indent = prefix_lead_space < in_column ?
      prefix_lead_space : in_column;
  অন্যথা
    {
      পরবর্তী_prefix_indent = in_column;
      জন্য (অক্ষর ধ্রুবক *p = prefix; *p != '\0'; p++)
        {
          unsigned অক্ষর pc = *p;
          যদি (c != pc)
            ফেরত c;
          in_column++;
          c = getc (f);
        }
      c = get_space (f, c);
    }
  ফেরত c;
}

/* Read blank characters from input file F, starting with C, and keeping
   in_column up-to-date.  Return first non-blank character.  */

স্থির পূর্ণসংখ্যা
get_space (FILE *f, পূর্ণসংখ্যা c)
{
  যখন (true)
    {
      যদি (c == ' ')
        in_column++;
      অন্যথা যদি (c == '\t')
        {
          tabs = true;
          in_column = (in_column / TABWIDTH + 1) * TABWIDTH;
        }
      অন্যথা
        ফেরত c;
      c = getc (f);
    }
}

/* Set extra fields in word W describing any attached punctuation.  */

স্থির শূন্য
check_punctuation (WORD *w)
{
  অক্ষর ধ্রুবক *start = w->text;
  অক্ষর ধ্রুবক *finish = start + (w->দৈর্ঘ্য - 1);
  unsigned অক্ষর fin = *finish;

  w->paren = isopen (*start);
  w->punct = !! ispunct (fin);
  যখন (start < finish && isclose (*finish))
    finish--;
  w->period = isperiod (*finish);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

স্থির শূন্য
flush_paragraph (শূন্য)
{
  WORD *split_point;
  WORD *w;
  পূর্ণসংখ্যা shift;
  COST best_break;

  /* In the special case where it's all one word, just flush it.  */

  যদি (word_limit == word)
    {
      size_t to_write = wptr - parabuf;
      যদি (fwrite (parabuf, 1, to_write, stdout) != to_write)
        write_error ();

      wptr = parabuf;
      ফেরত;
    }

  /* Otherwise:
     - format what you have so far as a paragraph,
     - find a low-cost line break near the end,
     - output to there,
     - make that the start of the paragraph.  */

  fmt_paragraph ();

  /* Choose a good split point.  */

  split_point = word_limit;
  best_break = MAXCOST;
  জন্য (w = word->পরবর্তী_break; w != word_limit; w = w->পরবর্তী_break)
    {
      যদি (w->best_cost - w->পরবর্তী_break->best_cost < best_break)
        {
          split_point = w;
          best_break = w->best_cost - w->পরবর্তী_break->best_cost;
        }
      যদি (best_break <= MAXCOST - সারি_CREDIT)
        best_break += সারি_CREDIT;
    }
  put_paragraph (split_point);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, split_point->text, wptr - split_point->text);
  shift = split_point->text - parabuf;
  wptr -= shift;

  /* Adjust text pointers.  */

  জন্য (w = split_point; w <= word_limit; w++)
    w->text -= shift;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (word, split_point, (word_limit - split_point + 1) * আকার *word);
  word_limit -= split_point - word;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

স্থির শূন্য
fmt_paragraph (শূন্য)
{
  WORD *w;
  পূর্ণসংখ্যা len;
  COST wcost, best;
  পূর্ণসংখ্যা saved_দৈর্ঘ্য;

  word_limit->best_cost = 0;
  saved_দৈর্ঘ্য = word_limit->দৈর্ঘ্য;
  word_limit->দৈর্ঘ্য = max_width;	/* sentinel */

  জন্য (WORD *start = word_limit - 1; start >= word; start--)
    {
      best = MAXCOST;
      len = start == word ? first_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = start;
      len += w->দৈর্ঘ্য;
      করো
        {
          w++;

          /* Consider breaking before w.  */

          wcost = সারি_cost (w, len) + w->best_cost;
          যদি (start == word && last_সারি_দৈর্ঘ্য > 0)
            wcost += RAGGED_COST (len - last_সারি_দৈর্ঘ্য);
          যদি (wcost < best)
            {
              best = wcost;
              start->পরবর্তী_break = w;
              start->সারি_দৈর্ঘ্য = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          যদি (w == word_limit)
            বিরতি;

          len += (w - 1)->space + w->দৈর্ঘ্য;	/* w > start >= word */
        }
      যখন (len <= max_width);
      start->best_cost = best + base_cost (start);
    }

  word_limit->দৈর্ঘ্য = saved_দৈর্ঘ্য;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#যদি __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

/* Return the constant component of the cost of breaking before the
   word THIS.  */

স্থির COST
base_cost (WORD *this)
{
  COST cost;

  cost = সারি_COST;

  যদি (this > word)
    {
      যদি ((this - 1)->period)
        {
          যদি ((this - 1)->final)
            cost -= SENTENCE_BONUS;
          অন্যথা
            cost += NOBREAK_COST;
        }
      অন্যথা যদি ((this - 1)->punct)
        cost -= PUNCT_BONUS;
      অন্যথা যদি (this > word + 1 && (this - 2)->final)
        cost += WIDOW_COST ((this - 1)->দৈর্ঘ্য);
    }

  যদি (this->paren)
    cost -= PAREN_BONUS;
  অন্যথা যদি (this->final)
    cost += ORPHAN_COST (this->দৈর্ঘ্য);

  ফেরত cost;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

স্থির COST
সারি_cost (WORD *পরবর্তী, পূর্ণসংখ্যা len)
{
  পূর্ণসংখ্যা n;
  COST cost;

  যদি (পরবর্তী == word_limit)
    ফেরত 0;
  n = goal_width - len;
  cost = SHORT_COST (n);
  যদি (পরবর্তী->পরবর্তী_break != word_limit)
    {
      n = len - পরবর্তী->সারি_দৈর্ঘ্য;
      cost += RAGGED_COST (n);
    }
  ফেরত cost;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

স্থির শূন্য
put_paragraph (WORD *finish)
{
  put_সারি (word, first_indent);
  জন্য (WORD *w = word->পরবর্তী_break; w != finish; w = w->পরবর্তী_break)
    put_সারি (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

স্থির শূন্য
put_সারি (WORD *w, পূর্ণসংখ্যা indent)
{
  WORD *endline;

  out_column = 0;
  put_space (prefix_indent);
  fputs (prefix, stdout);
  out_column += prefix_দৈর্ঘ্য;
  put_space (indent - out_column);

  endline = w->পরবর্তী_break - 1;
  জন্য (; w != endline; w++)
    {
      put_word (w);
      put_space (w->space);
    }
  put_word (w);
  last_সারি_দৈর্ঘ্য = out_column;
  putchar ('\n');

  যদি (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

স্থির শূন্য
put_word (WORD *w)
{
  অক্ষর ধ্রুবক *s = w->text;
  জন্য (পূর্ণসংখ্যা n = w->দৈর্ঘ্য; n != 0; n--)
    putchar (*s++);
  out_column += w->দৈর্ঘ্য;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

স্থির শূন্য
put_space (পূর্ণসংখ্যা space)
{
  পূর্ণসংখ্যা space_target, tab_target;

  space_target = out_column + space;
  যদি (tabs)
    {
      tab_target = space_target / TABWIDTH * TABWIDTH;
      যদি (out_column + 1 < tab_target)
        যখন (out_column < tab_target)
          {
            putchar ('\t');
            out_column = (out_column / TABWIDTH + 1) * TABWIDTH;
          }
    }
  যখন (out_column < space_target)
    {
      putchar (' ');
      out_column++;
    }
}
