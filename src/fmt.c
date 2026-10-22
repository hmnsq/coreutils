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
#define word unused_word_type

#شامل "c-ctype.h"
#شامل "system.h"
#شامل "fadvise.h"
#شامل "xdectoint.h"

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

تعریفنوع long صحیح COST;

#define MAXCOST	TYPE_MAXIMUM (COST)

#define SQR(n)		((n) * (n))
#define EQUIV(n)	SQR ((COST) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#define SHORT_COST(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#define RAGGED_COST(n)	(SHORT_COST (n) / 2)

/* Basic cost per line.  */
#define سطر_COST	EQUIV (70)

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
#define سطر_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#define MAXWORDS	1000
#define MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#define isopen(c)	(strchr ("(['`\"", c) != پوچ)
#define isclose(c)	(strchr (")]'\"", c) != پوچ)
#define isperiod(c)	(strchr (".?!", c) != پوچ)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#define TABWIDTH	8

/* Word descriptor structure.  */

تعریفنوع ساختار Word WORD;

ساختار Word
  {

    /* Static attributes determined during input.  */

    نویسه ثابت *text;		/* the text of the word */
    صحیح length;			/* length of this word */
    صحیح space;			/* the size of the following space */
    unsigned صحیح paren:1;	/* starts with open paren */
    unsigned صحیح period:1;	/* ends in [.?!])* */
    unsigned صحیح punct:1;	/* ends in punctuation */
    unsigned صحیح final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    صحیح سطر_length;		/* length of the best line starting here */
    COST best_cost;		/* cost of best paragraph starting here */
    WORD *next_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

ایستا تهی set_prefix (نویسه *p);
ایستا bool fmt (FILE *f, نویسه ثابت *);
ایستا bool get_paragraph (FILE *f);
ایستا صحیح get_سطر (FILE *f, صحیح c);
ایستا صحیح get_prefix (FILE *f);
ایستا صحیح get_space (FILE *f, صحیح c);
ایستا صحیح copy_rest (FILE *f, صحیح c);
ایستا bool same_para (صحیح c);
ایستا تهی flush_paragraph (تهی);
ایستا تهی fmt_paragraph (تهی);
ایستا تهی check_punctuation (WORD *w);
ایستا COST base_cost (WORD *this);
ایستا COST سطر_cost (WORD *next, صحیح len);
ایستا تهی put_paragraph (WORD *finish);
ایستا تهی put_سطر (WORD *w, صحیح indent);
ایستا تهی put_word (WORD *w);
ایستا تهی put_space (صحیح space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
ایستا bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
ایستا bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
ایستا bool split;

/* If true, don't preserve inter-word spacing (default false).  */
ایستا bool uniform;

/* Prefix minus leading and trailing spaces (default "").  */
ایستا نویسه ثابت *prefix = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
ایستا صحیح max_width = WIDTH;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
ایستا صحیح prefix_full_length;

/* The length of the leading space trimmed from the prefix.  */
ایستا صحیح prefix_lead_space;

/* The length of prefix minus leading and trailing space.  */
ایستا صحیح prefix_length;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
ایستا صحیح goal_width;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
ایستا صحیح in_column;

/* Start column of the next character to be written to stdout.  */
ایستا صحیح out_column;

/* Space for the paragraph text -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ایستا نویسه parabuf[MAXCHARS];

/* A pointer into parabuf, indicating the first unused character position.  */
ایستا نویسه *wptr;

/* The words of a paragraph -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ایستا WORD word[MAXWORDS];

/* A pointer into the above word array, indicating the first position
   after the last complete word.  Sometimes it will point at an incomplete
   word.  */
ایستا WORD *word_limit;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
ایستا bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
ایستا صحیح prefix_indent;

/* Indentation of the first line of the current paragraph.  */
ایستا صحیح first_indent;

/* Indentation of other lines of the current paragraph */
ایستا صحیح other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
ایستا صحیح next_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
ایستا صحیح next_prefix_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
ایستا صحیح last_سطر_length;

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
         preserve indentation of first two سطرها\n\
"));
      oputs (_("\
  -p, --prefix=STRING\n\
         reformat only سطرها beginning with STRING,\n\
         reattaching the prefix to reformatted سطرها\n\
"));
      oputs (_("\
  -s, --split-only\n\
         split long سطرها, but انجامبده not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation of first سطر different from second\n\
"));
      oputs (_("\
  -u, --uniform-spacing\n\
         one space between واژگان, two after sentences\n\
"));
      oputs (_("\
  -w, --width=WIDTH\n\
         maximum سطر width (پیشفرض of 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=WIDTH\n\
         goal width (پیشفرض of 93% of width)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Decode options and launch execution.  */

ایستا ساختار option ثابت long_options[] =
{
  {"crown-margin", no_argument, پوچ, 'c'},
  {"prefix", required_argument, پوچ, 'p'},
  {"split-only", no_argument, پوچ, 's'},
  {"tagged-paragraph", no_argument, پوچ, 't'},
  {"uniform-spacing", no_argument, پوچ, 'u'},
  {"width", required_argument, پوچ, 'w'},
  {"goal", required_argument, پوچ, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0},
};

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح optchar;
  bool ok = true;
  نویسه ثابت *max_width_option = پوچ;
  نویسه ثابت *goal_width_option = پوچ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  اگر (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_width_option = argv[1] + 1;

      /* Make the option we just parsed invisible to getopt.  */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  وقتی ((optchar = getopt_long (argc, argv, "0123456789cstuw:p:g:",
                                 long_options, پوچ))
         != -1)
    انتخاب (optchar)
      {
      پیشفرض:
        اگر (c_isdigit (optchar))
          error (0, 0, _("invalid option -- %c; -WIDTH is recognized\
 only when it is the first\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      حالت 'c':
        crown = true;
        بشکن;

      حالت 's':
        split = true;
        بشکن;

      حالت 't':
        tagged = true;
        بشکن;

      حالت 'u':
        uniform = true;
        بشکن;

      حالت 'w':
        max_width_option = optarg;
        بشکن;

      حالت 'g':
        goal_width_option = optarg;
        بشکن;

      حالت 'p':
        set_prefix (optarg);
        بشکن;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      }

  اگر (max_width_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_width = xnumtoumax (max_width_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  اگر (goal_width_option)
    {
      /* Limit goal_width to max_width.  */
      goal_width = xdectoumax (goal_width_option, 0, max_width, "",
                               _("invalid goal"), 0);
      اگر (max_width_option == پوچ)
        max_width = goal_width + 10;
    }
  وگرنه
    {
      goal_width = max_width * (2 * (100 - LEEWAY) + 1) / 200;
    }

  bool have_read_stdin = false;

  اگر (optind == argc)
    {
      have_read_stdin = true;
      ok = fmt (stdin, "-");
    }
  وگرنه
    {
      برای (; optind < argc; optind++)
        {
          نویسه *file = argv[optind];
          اگر (streq (file, "-"))
            {
              ok &= fmt (stdin, file);
              have_read_stdin = true;
            }
          وگرنه
            {
              FILE *in_stream;
              in_stream = fopen (file, "r");
              اگر (in_stream != پوچ)
                ok &= fmt (in_stream, file);
              وگرنه
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

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Trim space from the front and back of the string P, yielding the prefix,
   and record the lengths of the prefix and the space trimmed.  */

ایستا تهی
set_prefix (نویسه *p)
{
  نویسه *s;

  prefix_lead_space = 0;
  وقتی (*p == ' ')
    {
      prefix_lead_space++;
      p++;
    }
  prefix = p;
  prefix_full_length = strlen (p);
  s = p + prefix_full_length;
  وقتی (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  prefix_length = s - p;
}

/* Read F and send formatted output to stdout.
   Close F when done, unless F is stdin.  Diagnose input errors, using FILE.
   If !F, assume F resulted from an fopen failure and diagnose that.
   Return true if successful.  */

ایستا bool
fmt (FILE *f, نویسه ثابت *file)
{
  fadvise (f, FADVISE_SEQUENTIAL);
  tabs = false;
  other_indent = 0;
  next_char = get_prefix (f);
  وقتی (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (word_limit);
    }

  صحیح err = ferror (f) ? 0 : -1;
  /* err < 0 means success.  */
  اگر (f == stdin)
    clearerr (f);
  وگرنه اگر (fclose (f) != 0 && err < 0)
    err = errno;
  اگر (0 <= err)
    {
      اگر (f == stdin)
        error (0, errno, _("read error"));
      وگرنه
        error (0, errno, _("error reading %s"), quoteaf (file));
    }
  بازگشت err < 0;
}

/* Set the global variable 'other_indent' according to SAME_PARAGRAPH
   and other global variables.  */

ایستا تهی
set_other_indent (bool same_paragraph)
{
  اگر (split)
    other_indent = first_indent;
  وگرنه اگر (crown)
    {
      other_indent = (same_paragraph ? in_column : first_indent);
    }
  وگرنه اگر (tagged)
    {
      اگر (same_paragraph && in_column != first_indent)
        {
          other_indent = in_column;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      وگرنه اگر (other_indent == first_indent)
        other_indent = first_indent == 0 ? DEF_INDENT : 0;
    }
  وگرنه
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

ایستا bool
get_paragraph (FILE *f)
{
  صحیح c;

  last_سطر_length = 0;
  c = next_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  وقتی (c == '\n' || c == EOF
         || next_prefix_indent < prefix_lead_space
         || in_column < next_prefix_indent + prefix_full_length)
    {
      c = copy_rest (f, c);
      اگر (c == EOF)
        {
          next_char = EOF;
          بازگشت false;
        }
      putchar ('\n');
      c = get_prefix (f);
    }

  /* Got a suitable first line for a paragraph.  */

  prefix_indent = next_prefix_indent;
  first_indent = in_column;
  wptr = parabuf;
  word_limit = word;
  c = get_سطر (f, c);
  set_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  اگر (split)
    {
      /* empty */
    }
  وگرنه اگر (crown)
    {
      اگر (same_para (c))
        {
          انجامبده
            {			/* for each line till the end of the para */
              c = get_سطر (f, c);
            }
          وقتی (same_para (c) && in_column == other_indent);
        }
    }
  وگرنه اگر (tagged)
    {
      اگر (same_para (c) && in_column != first_indent)
        {
          انجامبده
            {			/* for each line till the end of the para */
              c = get_سطر (f, c);
            }
          وقتی (same_para (c) && in_column == other_indent);
        }
    }
  وگرنه
    {
      وقتی (same_para (c) && in_column == other_indent)
        c = get_سطر (f, c);
    }

  (word_limit - 1)->period = (word_limit - 1)->final = true;
  next_char = c;
  بازگشت true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

ایستا صحیح
copy_rest (FILE *f, صحیح c)
{
  out_column = 0;
  اگر (in_column > next_prefix_indent || (c != '\n' && c != EOF))
    {
      put_space (next_prefix_indent);
      برای (نویسه ثابت *s = prefix; out_column != in_column && *s; out_column++)
        putchar (*s++);
      اگر (c != EOF && c != '\n')
        put_space (in_column - out_column);
      اگر (c == EOF && in_column >= next_prefix_indent + prefix_length)
        putchar ('\n');
    }
  وقتی (c != '\n' && c != EOF)
    {
      putchar (c);
      c = getc (f);
    }
  بازگشت c;
}

/* Return true if a line whose first non-blank character after the
   prefix (if any) is C could belong to the current paragraph,
   otherwise false.  */

ایستا bool
same_para (صحیح c)
{
  بازگشت (next_prefix_indent == prefix_indent
          && in_column >= next_prefix_indent + prefix_full_length
          && c != '\n' && c != EOF);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

ایستا صحیح
get_سطر (FILE *f, صحیح c)
{
  صحیح start;
  نویسه *end_of_parabuf;
  WORD *end_of_word;

  end_of_parabuf = &parabuf[MAXCHARS];
  end_of_word = &word[MAXWORDS - 2];

  انجامبده
    {				/* for each word in a line */

      /* Scan word.  */

      word_limit->text = wptr;
      انجامبده
        {
          اگر (wptr == end_of_parabuf)
            {
              set_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      وقتی (c != EOF && !c_isspace (c));
      in_column += word_limit->length = wptr - word_limit->text;
      check_punctuation (word_limit);

      /* Scan inter-word space.  */

      start = in_column;
      c = get_space (f, c);
      word_limit->space = in_column - start;
      word_limit->final = (c == EOF
                           || (word_limit->period
                               && (c == '\n' || word_limit->space > 1)));
      اگر (c == '\n' || c == EOF || uniform)
        word_limit->space = word_limit->final ? 2 : 1;
      اگر (word_limit == end_of_word)
        {
          set_other_indent (true);
          flush_paragraph ();
        }
      word_limit++;
    }
  وقتی (c != '\n' && c != EOF);
  بازگشت get_prefix (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

ایستا صحیح
get_prefix (FILE *f)
{
  صحیح c;

  in_column = 0;
  c = get_space (f, getc (f));
  اگر (prefix_length == 0)
    next_prefix_indent = prefix_lead_space < in_column ?
      prefix_lead_space : in_column;
  وگرنه
    {
      next_prefix_indent = in_column;
      برای (نویسه ثابت *p = prefix; *p != '\0'; p++)
        {
          unsigned نویسه pc = *p;
          اگر (c != pc)
            بازگشت c;
          in_column++;
          c = getc (f);
        }
      c = get_space (f, c);
    }
  بازگشت c;
}

/* Read blank characters from input file F, starting with C, and keeping
   in_column up-to-date.  Return first non-blank character.  */

ایستا صحیح
get_space (FILE *f, صحیح c)
{
  وقتی (true)
    {
      اگر (c == ' ')
        in_column++;
      وگرنه اگر (c == '\t')
        {
          tabs = true;
          in_column = (in_column / TABWIDTH + 1) * TABWIDTH;
        }
      وگرنه
        بازگشت c;
      c = getc (f);
    }
}

/* Set extra fields in word W describing any attached punctuation.  */

ایستا تهی
check_punctuation (WORD *w)
{
  نویسه ثابت *start = w->text;
  نویسه ثابت *finish = start + (w->length - 1);
  unsigned نویسه fin = *finish;

  w->paren = isopen (*start);
  w->punct = !! ispunct (fin);
  وقتی (start < finish && isclose (*finish))
    finish--;
  w->period = isperiod (*finish);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

ایستا تهی
flush_paragraph (تهی)
{
  WORD *split_point;
  WORD *w;
  صحیح shift;
  COST best_break;

  /* In the special case where it's all one word, just flush it.  */

  اگر (word_limit == word)
    {
      size_t to_write = wptr - parabuf;
      اگر (fwrite (parabuf, 1, to_write, stdout) != to_write)
        write_error ();

      wptr = parabuf;
      بازگشت;
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
  برای (w = word->next_break; w != word_limit; w = w->next_break)
    {
      اگر (w->best_cost - w->next_break->best_cost < best_break)
        {
          split_point = w;
          best_break = w->best_cost - w->next_break->best_cost;
        }
      اگر (best_break <= MAXCOST - سطر_CREDIT)
        best_break += سطر_CREDIT;
    }
  put_paragraph (split_point);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, split_point->text, wptr - split_point->text);
  shift = split_point->text - parabuf;
  wptr -= shift;

  /* Adjust text pointers.  */

  برای (w = split_point; w <= word_limit; w++)
    w->text -= shift;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (word, split_point, (word_limit - split_point + 1) * اندازه *word);
  word_limit -= split_point - word;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

ایستا تهی
fmt_paragraph (تهی)
{
  WORD *w;
  صحیح len;
  COST wcost, best;
  صحیح saved_length;

  word_limit->best_cost = 0;
  saved_length = word_limit->length;
  word_limit->length = max_width;	/* sentinel */

  برای (WORD *start = word_limit - 1; start >= word; start--)
    {
      best = MAXCOST;
      len = start == word ? first_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = start;
      len += w->length;
      انجامبده
        {
          w++;

          /* Consider breaking before w.  */

          wcost = سطر_cost (w, len) + w->best_cost;
          اگر (start == word && last_سطر_length > 0)
            wcost += RAGGED_COST (len - last_سطر_length);
          اگر (wcost < best)
            {
              best = wcost;
              start->next_break = w;
              start->سطر_length = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          اگر (w == word_limit)
            بشکن;

          len += (w - 1)->space + w->length;	/* w > start >= word */
        }
      وقتی (len <= max_width);
      start->best_cost = best + base_cost (start);
    }

  word_limit->length = saved_length;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#اگر __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

/* Return the constant component of the cost of breaking before the
   word THIS.  */

ایستا COST
base_cost (WORD *this)
{
  COST cost;

  cost = سطر_COST;

  اگر (this > word)
    {
      اگر ((this - 1)->period)
        {
          اگر ((this - 1)->final)
            cost -= SENTENCE_BONUS;
          وگرنه
            cost += NOBREAK_COST;
        }
      وگرنه اگر ((this - 1)->punct)
        cost -= PUNCT_BONUS;
      وگرنه اگر (this > word + 1 && (this - 2)->final)
        cost += WIDOW_COST ((this - 1)->length);
    }

  اگر (this->paren)
    cost -= PAREN_BONUS;
  وگرنه اگر (this->final)
    cost += ORPHAN_COST (this->length);

  بازگشت cost;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

ایستا COST
سطر_cost (WORD *next, صحیح len)
{
  صحیح n;
  COST cost;

  اگر (next == word_limit)
    بازگشت 0;
  n = goal_width - len;
  cost = SHORT_COST (n);
  اگر (next->next_break != word_limit)
    {
      n = len - next->سطر_length;
      cost += RAGGED_COST (n);
    }
  بازگشت cost;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

ایستا تهی
put_paragraph (WORD *finish)
{
  put_سطر (word, first_indent);
  برای (WORD *w = word->next_break; w != finish; w = w->next_break)
    put_سطر (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

ایستا تهی
put_سطر (WORD *w, صحیح indent)
{
  WORD *endline;

  out_column = 0;
  put_space (prefix_indent);
  fputs (prefix, stdout);
  out_column += prefix_length;
  put_space (indent - out_column);

  endline = w->next_break - 1;
  برای (; w != endline; w++)
    {
      put_word (w);
      put_space (w->space);
    }
  put_word (w);
  last_سطر_length = out_column;
  putchar ('\n');

  اگر (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

ایستا تهی
put_word (WORD *w)
{
  نویسه ثابت *s = w->text;
  برای (صحیح n = w->length; n != 0; n--)
    putchar (*s++);
  out_column += w->length;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

ایستا تهی
put_space (صحیح space)
{
  صحیح space_target, tab_target;

  space_target = out_column + space;
  اگر (tabs)
    {
      tab_target = space_target / TABWIDTH * TABWIDTH;
      اگر (out_column + 1 < tab_target)
        وقتی (out_column < tab_target)
          {
            putchar ('\t');
            out_column = (out_column / TABWIDTH + 1) * TABWIDTH;
          }
    }
  وقتی (out_column < space_target)
    {
      putchar (' ');
      out_column++;
    }
}
