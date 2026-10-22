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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <getopt.h>

/* Redefine.  Otherwise, systems (Unicos for one) with headers that define
   it to be a type get syntax errors for the variable declaration below.  */
#define word unused_word_type

#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "xdectoint.h"

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

வகைவரை long முழு COST;

#define MAXCOST	TYPE_MAXIMUM (COST)

#define SQR(n)		((n) * (n))
#define EQUIV(n)	SQR ((COST) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#define SHORT_COST(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#define RAGGED_COST(n)	(SHORT_COST (n) / 2)

/* Basic cost per line.  */
#define வரி_COST	EQUIV (70)

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
#define வரி_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#define MAXWORDS	1000
#define MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#define isopen(c)	(strchr ("(['`\"", c) != வெற்று)
#define isclose(c)	(strchr (")]'\"", c) != வெற்று)
#define isperiod(c)	(strchr (".?!", c) != வெற்று)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#define TABWIDTH	8

/* Word descriptor structure.  */

வகைவரை கட்டமைப்பு Word WORD;

கட்டமைப்பு Word
  {

    /* Static attributes determined during input.  */

    எழுத்து மாறிலி *text;		/* the text of the word */
    முழு length;			/* length of this word */
    முழு space;			/* the size of the following space */
    unsigned முழு paren:1;	/* starts with open paren */
    unsigned முழு period:1;	/* ends in [.?!])* */
    unsigned முழு punct:1;	/* ends in punctuation */
    unsigned முழு final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    முழு வரி_length;		/* length of the best line starting here */
    COST best_cost;		/* cost of best paragraph starting here */
    WORD *next_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

நிலையான வெறுமை set_prefix (எழுத்து *p);
நிலையான bool fmt (FILE *f, எழுத்து மாறிலி *);
நிலையான bool get_paragraph (FILE *f);
நிலையான முழு get_வரி (FILE *f, முழு c);
நிலையான முழு get_prefix (FILE *f);
நிலையான முழு get_space (FILE *f, முழு c);
நிலையான முழு copy_rest (FILE *f, முழு c);
நிலையான bool same_para (முழு c);
நிலையான வெறுமை flush_paragraph (வெறுமை);
நிலையான வெறுமை fmt_paragraph (வெறுமை);
நிலையான வெறுமை check_punctuation (WORD *w);
நிலையான COST base_cost (WORD *this);
நிலையான COST வரி_cost (WORD *next, முழு len);
நிலையான வெறுமை put_paragraph (WORD *finish);
நிலையான வெறுமை put_வரி (WORD *w, முழு indent);
நிலையான வெறுமை put_word (WORD *w);
நிலையான வெறுமை put_space (முழு space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
நிலையான bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
நிலையான bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
நிலையான bool split;

/* If true, don't preserve inter-word spacing (default false).  */
நிலையான bool uniform;

/* Prefix minus leading and trailing spaces (default "").  */
நிலையான எழுத்து மாறிலி *prefix = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
நிலையான முழு max_width = WIDTH;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
நிலையான முழு prefix_full_length;

/* The length of the leading space trimmed from the prefix.  */
நிலையான முழு prefix_lead_space;

/* The length of prefix minus leading and trailing space.  */
நிலையான முழு prefix_length;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
நிலையான முழு goal_width;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
நிலையான முழு in_column;

/* Start column of the next character to be written to stdout.  */
நிலையான முழு out_column;

/* Space for the paragraph text -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
நிலையான எழுத்து parabuf[MAXCHARS];

/* A pointer into parabuf, indicating the first unused character position.  */
நிலையான எழுத்து *wptr;

/* The words of a paragraph -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
நிலையான WORD word[MAXWORDS];

/* A pointer into the above word array, indicating the first position
   after the last complete word.  Sometimes it will point at an incomplete
   word.  */
நிலையான WORD *word_limit;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
நிலையான bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
நிலையான முழு prefix_indent;

/* Indentation of the first line of the current paragraph.  */
நிலையான முழு first_indent;

/* Indentation of other lines of the current paragraph */
நிலையான முழு other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
நிலையான முழு next_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
நிலையான முழு next_prefix_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
நிலையான முழு last_வரி_length;

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
         preserve indentation of first two வரிகள்\n\
"));
      oputs (_("\
  -p, --prefix=STRING\n\
         reformat only வரிகள் beginning with STRING,\n\
         reattaching the prefix to reformatted வரிகள்\n\
"));
      oputs (_("\
  -s, --split-only\n\
         split long வரிகள், but செய் not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation of first வரி different from second\n\
"));
      oputs (_("\
  -u, --uniform-spacing\n\
         one space between சொற்கள், two after sentences\n\
"));
      oputs (_("\
  -w, --width=WIDTH\n\
         maximum வரி width (இயல்பு of 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=WIDTH\n\
         goal width (இயல்பு of 93% of width)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Decode options and launch execution.  */

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"crown-margin", no_argument, வெற்று, 'c'},
  {"prefix", required_argument, வெற்று, 'p'},
  {"split-only", no_argument, வெற்று, 's'},
  {"tagged-paragraph", no_argument, வெற்று, 't'},
  {"uniform-spacing", no_argument, வெற்று, 'u'},
  {"width", required_argument, வெற்று, 'w'},
  {"goal", required_argument, வெற்று, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0},
};

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு optchar;
  bool ok = true;
  எழுத்து மாறிலி *max_width_option = வெற்று;
  எழுத்து மாறிலி *goal_width_option = வெற்று;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  எனில் (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_width_option = argv[1] + 1;

      /* Make the option we just parsed invisible to getopt.  */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  போது ((optchar = getopt_long (argc, argv, "0123456789cstuw:p:g:",
                                 long_options, வெற்று))
         != -1)
    தேர்வு (optchar)
      {
      இயல்பு:
        எனில் (c_isdigit (optchar))
          error (0, 0, _("invalid option -- %c; -WIDTH is recognized\
 only when it is the first\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      நிலை 'c':
        crown = true;
        நிறுத்து;

      நிலை 's':
        split = true;
        நிறுத்து;

      நிலை 't':
        tagged = true;
        நிறுத்து;

      நிலை 'u':
        uniform = true;
        நிறுத்து;

      நிலை 'w':
        max_width_option = optarg;
        நிறுத்து;

      நிலை 'g':
        goal_width_option = optarg;
        நிறுத்து;

      நிலை 'p':
        set_prefix (optarg);
        நிறுத்து;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      }

  எனில் (max_width_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_width = xnumtoumax (max_width_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  எனில் (goal_width_option)
    {
      /* Limit goal_width to max_width.  */
      goal_width = xdectoumax (goal_width_option, 0, max_width, "",
                               _("invalid goal"), 0);
      எனில் (max_width_option == வெற்று)
        max_width = goal_width + 10;
    }
  இல்லையெனில்
    {
      goal_width = max_width * (2 * (100 - LEEWAY) + 1) / 200;
    }

  bool have_read_stdin = false;

  எனில் (optind == argc)
    {
      have_read_stdin = true;
      ok = fmt (stdin, "-");
    }
  இல்லையெனில்
    {
      க்கு (; optind < argc; optind++)
        {
          எழுத்து *file = argv[optind];
          எனில் (streq (file, "-"))
            {
              ok &= fmt (stdin, file);
              have_read_stdin = true;
            }
          இல்லையெனில்
            {
              FILE *in_stream;
              in_stream = fopen (file, "r");
              எனில் (in_stream != வெற்று)
                ok &= fmt (in_stream, file);
              இல்லையெனில்
                {
                  error (0, errno, _("cannot open %s for reading"),
                         quoteaf (file));
                  ok = false;
                }
            }
        }
    }

  எனில் (have_read_stdin && fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s", _("closing standard input"));

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Trim space from the front and back of the string P, yielding the prefix,
   and record the lengths of the prefix and the space trimmed.  */

நிலையான வெறுமை
set_prefix (எழுத்து *p)
{
  எழுத்து *s;

  prefix_lead_space = 0;
  போது (*p == ' ')
    {
      prefix_lead_space++;
      p++;
    }
  prefix = p;
  prefix_full_length = strlen (p);
  s = p + prefix_full_length;
  போது (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  prefix_length = s - p;
}

/* Read F and send formatted output to stdout.
   Close F when done, unless F is stdin.  Diagnose input errors, using FILE.
   If !F, assume F resulted from an fopen failure and diagnose that.
   Return true if successful.  */

நிலையான bool
fmt (FILE *f, எழுத்து மாறிலி *file)
{
  fadvise (f, FADVISE_SEQUENTIAL);
  tabs = false;
  other_indent = 0;
  next_char = get_prefix (f);
  போது (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (word_limit);
    }

  முழு err = ferror (f) ? 0 : -1;
  /* err < 0 means success.  */
  எனில் (f == stdin)
    clearerr (f);
  இல்லையெனில் எனில் (fclose (f) != 0 && err < 0)
    err = errno;
  எனில் (0 <= err)
    {
      எனில் (f == stdin)
        error (0, errno, _("read error"));
      இல்லையெனில்
        error (0, errno, _("error reading %s"), quoteaf (file));
    }
  திரும்பு err < 0;
}

/* Set the global variable 'other_indent' according to SAME_PARAGRAPH
   and other global variables.  */

நிலையான வெறுமை
set_other_indent (bool same_paragraph)
{
  எனில் (split)
    other_indent = first_indent;
  இல்லையெனில் எனில் (crown)
    {
      other_indent = (same_paragraph ? in_column : first_indent);
    }
  இல்லையெனில் எனில் (tagged)
    {
      எனில் (same_paragraph && in_column != first_indent)
        {
          other_indent = in_column;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      இல்லையெனில் எனில் (other_indent == first_indent)
        other_indent = first_indent == 0 ? DEF_INDENT : 0;
    }
  இல்லையெனில்
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

நிலையான bool
get_paragraph (FILE *f)
{
  முழு c;

  last_வரி_length = 0;
  c = next_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  போது (c == '\n' || c == EOF
         || next_prefix_indent < prefix_lead_space
         || in_column < next_prefix_indent + prefix_full_length)
    {
      c = copy_rest (f, c);
      எனில் (c == EOF)
        {
          next_char = EOF;
          திரும்பு false;
        }
      putchar ('\n');
      c = get_prefix (f);
    }

  /* Got a suitable first line for a paragraph.  */

  prefix_indent = next_prefix_indent;
  first_indent = in_column;
  wptr = parabuf;
  word_limit = word;
  c = get_வரி (f, c);
  set_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  எனில் (split)
    {
      /* empty */
    }
  இல்லையெனில் எனில் (crown)
    {
      எனில் (same_para (c))
        {
          செய்
            {			/* for each line till the end of the para */
              c = get_வரி (f, c);
            }
          போது (same_para (c) && in_column == other_indent);
        }
    }
  இல்லையெனில் எனில் (tagged)
    {
      எனில் (same_para (c) && in_column != first_indent)
        {
          செய்
            {			/* for each line till the end of the para */
              c = get_வரி (f, c);
            }
          போது (same_para (c) && in_column == other_indent);
        }
    }
  இல்லையெனில்
    {
      போது (same_para (c) && in_column == other_indent)
        c = get_வரி (f, c);
    }

  (word_limit - 1)->period = (word_limit - 1)->final = true;
  next_char = c;
  திரும்பு true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

நிலையான முழு
copy_rest (FILE *f, முழு c)
{
  out_column = 0;
  எனில் (in_column > next_prefix_indent || (c != '\n' && c != EOF))
    {
      put_space (next_prefix_indent);
      க்கு (எழுத்து மாறிலி *s = prefix; out_column != in_column && *s; out_column++)
        putchar (*s++);
      எனில் (c != EOF && c != '\n')
        put_space (in_column - out_column);
      எனில் (c == EOF && in_column >= next_prefix_indent + prefix_length)
        putchar ('\n');
    }
  போது (c != '\n' && c != EOF)
    {
      putchar (c);
      c = getc (f);
    }
  திரும்பு c;
}

/* Return true if a line whose first non-blank character after the
   prefix (if any) is C could belong to the current paragraph,
   otherwise false.  */

நிலையான bool
same_para (முழு c)
{
  திரும்பு (next_prefix_indent == prefix_indent
          && in_column >= next_prefix_indent + prefix_full_length
          && c != '\n' && c != EOF);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

நிலையான முழு
get_வரி (FILE *f, முழு c)
{
  முழு start;
  எழுத்து *end_of_parabuf;
  WORD *end_of_word;

  end_of_parabuf = &parabuf[MAXCHARS];
  end_of_word = &word[MAXWORDS - 2];

  செய்
    {				/* for each word in a line */

      /* Scan word.  */

      word_limit->text = wptr;
      செய்
        {
          எனில் (wptr == end_of_parabuf)
            {
              set_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      போது (c != EOF && !c_isspace (c));
      in_column += word_limit->length = wptr - word_limit->text;
      check_punctuation (word_limit);

      /* Scan inter-word space.  */

      start = in_column;
      c = get_space (f, c);
      word_limit->space = in_column - start;
      word_limit->final = (c == EOF
                           || (word_limit->period
                               && (c == '\n' || word_limit->space > 1)));
      எனில் (c == '\n' || c == EOF || uniform)
        word_limit->space = word_limit->final ? 2 : 1;
      எனில் (word_limit == end_of_word)
        {
          set_other_indent (true);
          flush_paragraph ();
        }
      word_limit++;
    }
  போது (c != '\n' && c != EOF);
  திரும்பு get_prefix (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

நிலையான முழு
get_prefix (FILE *f)
{
  முழு c;

  in_column = 0;
  c = get_space (f, getc (f));
  எனில் (prefix_length == 0)
    next_prefix_indent = prefix_lead_space < in_column ?
      prefix_lead_space : in_column;
  இல்லையெனில்
    {
      next_prefix_indent = in_column;
      க்கு (எழுத்து மாறிலி *p = prefix; *p != '\0'; p++)
        {
          unsigned எழுத்து pc = *p;
          எனில் (c != pc)
            திரும்பு c;
          in_column++;
          c = getc (f);
        }
      c = get_space (f, c);
    }
  திரும்பு c;
}

/* Read blank characters from input file F, starting with C, and keeping
   in_column up-to-date.  Return first non-blank character.  */

நிலையான முழு
get_space (FILE *f, முழு c)
{
  போது (true)
    {
      எனில் (c == ' ')
        in_column++;
      இல்லையெனில் எனில் (c == '\t')
        {
          tabs = true;
          in_column = (in_column / TABWIDTH + 1) * TABWIDTH;
        }
      இல்லையெனில்
        திரும்பு c;
      c = getc (f);
    }
}

/* Set extra fields in word W describing any attached punctuation.  */

நிலையான வெறுமை
check_punctuation (WORD *w)
{
  எழுத்து மாறிலி *start = w->text;
  எழுத்து மாறிலி *finish = start + (w->length - 1);
  unsigned எழுத்து fin = *finish;

  w->paren = isopen (*start);
  w->punct = !! ispunct (fin);
  போது (start < finish && isclose (*finish))
    finish--;
  w->period = isperiod (*finish);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

நிலையான வெறுமை
flush_paragraph (வெறுமை)
{
  WORD *split_point;
  WORD *w;
  முழு shift;
  COST best_break;

  /* In the special case where it's all one word, just flush it.  */

  எனில் (word_limit == word)
    {
      size_t to_write = wptr - parabuf;
      எனில் (fwrite (parabuf, 1, to_write, stdout) != to_write)
        write_error ();

      wptr = parabuf;
      திரும்பு;
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
  க்கு (w = word->next_break; w != word_limit; w = w->next_break)
    {
      எனில் (w->best_cost - w->next_break->best_cost < best_break)
        {
          split_point = w;
          best_break = w->best_cost - w->next_break->best_cost;
        }
      எனில் (best_break <= MAXCOST - வரி_CREDIT)
        best_break += வரி_CREDIT;
    }
  put_paragraph (split_point);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, split_point->text, wptr - split_point->text);
  shift = split_point->text - parabuf;
  wptr -= shift;

  /* Adjust text pointers.  */

  க்கு (w = split_point; w <= word_limit; w++)
    w->text -= shift;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (word, split_point, (word_limit - split_point + 1) * அளவு *word);
  word_limit -= split_point - word;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

நிலையான வெறுமை
fmt_paragraph (வெறுமை)
{
  WORD *w;
  முழு len;
  COST wcost, best;
  முழு saved_length;

  word_limit->best_cost = 0;
  saved_length = word_limit->length;
  word_limit->length = max_width;	/* sentinel */

  க்கு (WORD *start = word_limit - 1; start >= word; start--)
    {
      best = MAXCOST;
      len = start == word ? first_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = start;
      len += w->length;
      செய்
        {
          w++;

          /* Consider breaking before w.  */

          wcost = வரி_cost (w, len) + w->best_cost;
          எனில் (start == word && last_வரி_length > 0)
            wcost += RAGGED_COST (len - last_வரி_length);
          எனில் (wcost < best)
            {
              best = wcost;
              start->next_break = w;
              start->வரி_length = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          எனில் (w == word_limit)
            நிறுத்து;

          len += (w - 1)->space + w->length;	/* w > start >= word */
        }
      போது (len <= max_width);
      start->best_cost = best + base_cost (start);
    }

  word_limit->length = saved_length;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#எனில் __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

/* Return the constant component of the cost of breaking before the
   word THIS.  */

நிலையான COST
base_cost (WORD *this)
{
  COST cost;

  cost = வரி_COST;

  எனில் (this > word)
    {
      எனில் ((this - 1)->period)
        {
          எனில் ((this - 1)->final)
            cost -= SENTENCE_BONUS;
          இல்லையெனில்
            cost += NOBREAK_COST;
        }
      இல்லையெனில் எனில் ((this - 1)->punct)
        cost -= PUNCT_BONUS;
      இல்லையெனில் எனில் (this > word + 1 && (this - 2)->final)
        cost += WIDOW_COST ((this - 1)->length);
    }

  எனில் (this->paren)
    cost -= PAREN_BONUS;
  இல்லையெனில் எனில் (this->final)
    cost += ORPHAN_COST (this->length);

  திரும்பு cost;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

நிலையான COST
வரி_cost (WORD *next, முழு len)
{
  முழு n;
  COST cost;

  எனில் (next == word_limit)
    திரும்பு 0;
  n = goal_width - len;
  cost = SHORT_COST (n);
  எனில் (next->next_break != word_limit)
    {
      n = len - next->வரி_length;
      cost += RAGGED_COST (n);
    }
  திரும்பு cost;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

நிலையான வெறுமை
put_paragraph (WORD *finish)
{
  put_வரி (word, first_indent);
  க்கு (WORD *w = word->next_break; w != finish; w = w->next_break)
    put_வரி (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

நிலையான வெறுமை
put_வரி (WORD *w, முழு indent)
{
  WORD *endline;

  out_column = 0;
  put_space (prefix_indent);
  fputs (prefix, stdout);
  out_column += prefix_length;
  put_space (indent - out_column);

  endline = w->next_break - 1;
  க்கு (; w != endline; w++)
    {
      put_word (w);
      put_space (w->space);
    }
  put_word (w);
  last_வரி_length = out_column;
  putchar ('\n');

  எனில் (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

நிலையான வெறுமை
put_word (WORD *w)
{
  எழுத்து மாறிலி *s = w->text;
  க்கு (முழு n = w->length; n != 0; n--)
    putchar (*s++);
  out_column += w->length;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

நிலையான வெறுமை
put_space (முழு space)
{
  முழு space_target, tab_target;

  space_target = out_column + space;
  எனில் (tabs)
    {
      tab_target = space_target / TABWIDTH * TABWIDTH;
      எனில் (out_column + 1 < tab_target)
        போது (out_column < tab_target)
          {
            putchar ('\t');
            out_column = (out_column / TABWIDTH + 1) * TABWIDTH;
          }
    }
  போது (out_column < space_target)
    {
      putchar (' ');
      out_column++;
    }
}
