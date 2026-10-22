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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

/* Redefine.  Otherwise, systems (Unicos for one) with headers that define
   it to be a type get syntax errors for the variable declaration below.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸ਼ਬਦ_2 ਅਣਵਰਤਿਆ_ਸ਼ਬਦ_type

#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "fmt"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Ross Paterson")

/* The following parameters represent the program's idea of what is
   "best".  Adjust to taste, subject to the caveats given.  */

/* Default longest permitted line length (max_width).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਚੌੜਾਈ_2	75

/* Prefer lines to be LEEWAY % shorter than the maximum width, giving
   room for optimization.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LEEWAY	7

/* The default secondary indent of tagged paragraph used for unindented
   one-line paragraphs not preceded by any multi-line paragraphs.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_INDENT 3

/* Costs and bonuses are expressed as the equivalent departure from the
   optimal line length, multiplied by 10.  e.g. assigning something a
   cost of 50 means that it is as bad as a line 5 characters too short
   or too long.  The definition of SHORT_COST(n) should not be changed.
   However, EQUIV(n) may need tuning.  */

/* FIXME: "fmt" misbehaves given large inputs or options.  One
   possible workaround for part of the problem is to change COST to be
   a floating-point type.  There are other problems besides COST,
   though; see MAXWORDS below.  */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਲੰਮਾ ਪੂਰਨ ਲਾਗਤ;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAXCOST	TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (ਲਾਗਤ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SQR(n)		((n) * (n))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EQUIV(n)	SQR ((ਲਾਗਤ) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SHORT_ਲਾਗਤ(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RAGGED_ਲਾਗਤ(n)	(SHORT_ਲਾਗਤ (n) / 2)

/* Basic cost per line.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਤਰ_ਲਾਗਤ	EQUIV (70)

/* Cost of breaking a line after the first word of a sentence, where
   the length of the word is N.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WIDOW_ਲਾਗਤ(n)	(EQUIV (200) / ((n) + 2))

/* Cost of breaking a line before the last word of a sentence, where
   the length of the word is N.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ORPHAN_ਲਾਗਤ(n)	(EQUIV (150) / ((n) + 2))

/* Bonus for breaking a line at the end of a sentence.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SENTENCE_BONUS	EQUIV (50)

/* Cost of breaking a line after a period not marking end of a sentence.
   With the definition of sentence we are using (borrowed from emacs, see
   get_line()) such a break would then look like a sentence break.  Hence
   we assign a very high cost -- it should be avoided unless things are
   really bad.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOBREAK_ਲਾਗਤ	EQUIV (600)

/* Bonus for breaking a line before open parenthesis.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PAREN_BONUS	EQUIV (40)

/* Bonus for breaking a line after other punctuation.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PUNCT_BONUS	EQUIV(40)

/* Credit for breaking a long paragraph one line later.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਤਰ_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAXWORDS	1000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ isopen(c)	(strchr ("(['`\"", c) != ਨਿਰਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ isclose(c)	(strchr (")]'\"", c) != ਨਿਰਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ isperiod(c)	(strchr (".?!", c) != ਨਿਰਾ)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TABWIDTH	8

/* Word descriptor structure.  */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਸ਼ਬਦ_3 ਸ਼ਬਦ;

ਬਣਤਰ ਸ਼ਬਦ_3
  {

    /* Static attributes determined during input.  */

    ਅੱਖਰ ਅਚਲ *ਪਾਠ;		/* the text of the word */
    ਪੂਰਨ ਲੰਮਾਈ;			/* length of this word */
    ਪੂਰਨ space;			/* the size of the following space */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ paren:1;	/* starts with open paren */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ period:1;	/* ends in [.?!])* */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ punct:1;	/* ends in punctuation */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    ਪੂਰਨ ਸਤਰ_ਲੰਮਾਈ;		/* length of the best line starting here */
    ਲਾਗਤ ਉੱਤਮ_ਲਾਗਤ;		/* cost of best paragraph starting here */
    ਸ਼ਬਦ *ਅਗਲਾ_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਅਗੇਤਰ (ਅੱਖਰ *p);
ਸਥਿਰ bool fmt (FILE *f, ਅੱਖਰ ਅਚਲ *);
ਸਥਿਰ bool get_paragraph (FILE *f);
ਸਥਿਰ ਪੂਰਨ get_ਸਤਰ (FILE *f, ਪੂਰਨ c);
ਸਥਿਰ ਪੂਰਨ get_ਅਗੇਤਰ (FILE *f);
ਸਥਿਰ ਪੂਰਨ get_space (FILE *f, ਪੂਰਨ c);
ਸਥਿਰ ਪੂਰਨ copy_rest (FILE *f, ਪੂਰਨ c);
ਸਥਿਰ bool same_para (ਪੂਰਨ c);
ਸਥਿਰ ਖਾਲੀ flush_paragraph (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ fmt_paragraph (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ check_punctuation (ਸ਼ਬਦ *w);
ਸਥਿਰ ਲਾਗਤ ਅਧਾਰ_ਲਾਗਤ (ਸ਼ਬਦ *this);
ਸਥਿਰ ਲਾਗਤ ਸਤਰ_ਲਾਗਤ_2 (ਸ਼ਬਦ *ਅਗਲਾ, ਪੂਰਨ len);
ਸਥਿਰ ਖਾਲੀ put_paragraph (ਸ਼ਬਦ *ਅੰਤ);
ਸਥਿਰ ਖਾਲੀ put_ਸਤਰ (ਸ਼ਬਦ *w, ਪੂਰਨ indent);
ਸਥਿਰ ਖਾਲੀ put_ਸ਼ਬਦ (ਸ਼ਬਦ *w);
ਸਥਿਰ ਖਾਲੀ put_space (ਪੂਰਨ space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
ਸਥਿਰ bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
ਸਥਿਰ bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
ਸਥਿਰ bool ਵੰਡੋ;

/* If true, don't preserve inter-word spacing (default false).  */
ਸਥਿਰ bool ਇਕਸਾਰ;

/* Prefix minus leading and trailing spaces (default "").  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਗੇਤਰ = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
ਸਥਿਰ ਪੂਰਨ max_ਚੌੜਾਈ = ਚੌੜਾਈ_2;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
ਸਥਿਰ ਪੂਰਨ ਅਗੇਤਰ_ਭਰਿਆ_ਲੰਮਾਈ;

/* The length of the leading space trimmed from the prefix.  */
ਸਥਿਰ ਪੂਰਨ ਅਗੇਤਰ_lead_space;

/* The length of prefix minus leading and trailing space.  */
ਸਥਿਰ ਪੂਰਨ ਅਗੇਤਰ_ਲੰਮਾਈ;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
ਸਥਿਰ ਪੂਰਨ goal_ਚੌੜਾਈ;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
ਸਥਿਰ ਪੂਰਨ ਵਿੱਚ_ਕਾਲਮ;

/* Start column of the next character to be written to stdout.  */
ਸਥਿਰ ਪੂਰਨ ਬਾਹਰਲਾ_ਕਾਲਮ;

/* Space for the paragraph text -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ਸਥਿਰ ਅੱਖਰ parabuf[MAXCHARS];

/* A pointer into parabuf, indicating the first unused character position.  */
ਸਥਿਰ ਅੱਖਰ *wptr;

/* The words of a paragraph -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ਸਥਿਰ ਸ਼ਬਦ ਸ਼ਬਦ_2[MAXWORDS];

/* A pointer into the above word array, indicating the first position
   after the last complete word.  Sometimes it will point at an incomplete
   word.  */
ਸਥਿਰ ਸ਼ਬਦ *ਸ਼ਬਦ_ਹੱਦਬੰਦੀ;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
ਸਥਿਰ bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
ਸਥਿਰ ਪੂਰਨ ਅਗੇਤਰ_indent;

/* Indentation of the first line of the current paragraph.  */
ਸਥਿਰ ਪੂਰਨ ਪਹਿਲਾ_indent;

/* Indentation of other lines of the current paragraph */
ਸਥਿਰ ਪੂਰਨ other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
ਸਥਿਰ ਪੂਰਨ ਅਗਲਾ_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
ਸਥਿਰ ਪੂਰਨ ਅਗਲਾ_ਅਗੇਤਰ_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
ਸਥਿਰ ਪੂਰਨ ਆਖਰੀ_ਸਤਰ_ਲੰਮਾਈ;

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [-WIDTH] [OPTION]... [FILE]...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Reformat each paragraph ਵਿੱਚ the FILE(s), writing ਮੰਜ਼ਿਲ standard output.\n\
The option -ਚੌੜਾਈ_2 is an abbreviated form ਦਾ --ਚੌੜਾਈ=DIGITS.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --crown-margin\n\
         preserve indentation ਦਾ ਪਹਿਲਾ two ਸਤਰਾਂ\n\
"));
      oputs (_("\
  -p, --ਅਗੇਤਰ=STRING\n\
         reformat only ਸਤਰਾਂ beginning with STRING,\n\
         reattaching the ਅਗੇਤਰ ਮੰਜ਼ਿਲ reformatted ਸਤਰਾਂ\n\
"));
      oputs (_("\
  -s, --ਵੰਡੋ-only\n\
         ਵੰਡੋ ਲੰਮਾ ਸਤਰਾਂ, but ਕਰੋ not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation ਦਾ ਪਹਿਲਾ ਸਤਰ different from ਦੂਜਾ\n\
"));
      oputs (_("\
  -u, --ਇਕਸਾਰ-spacing\n\
         one space between ਸ਼ਬਦ_4, two after sentences\n\
"));
      oputs (_("\
  -w, --ਚੌੜਾਈ=ਚੌੜਾਈ_2\n\
         ਵੱਧ_ਤੋਂ_ਵੱਧ ਸਤਰ ਚੌੜਾਈ (ਮੂਲ ਦਾ 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=ਚੌੜਾਈ_2\n\
         goal ਚੌੜਾਈ (ਮੂਲ ਦਾ 93% ਦਾ ਚੌੜਾਈ)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Decode options and launch execution.  */

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"crown-margin", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"prefix", required_ਦਲੀਲ, ਨਿਰਾ, 'p'},
  {"split-only", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"tagged-paragraph", no_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"uniform-spacing", no_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"width", required_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  {"goal", required_ਦਲੀਲ, ਨਿਰਾ, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0},
};

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ optchar;
  bool ok = true;
  ਅੱਖਰ ਅਚਲ *max_ਚੌੜਾਈ_option = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *goal_ਚੌੜਾਈ_option = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜੇ (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_ਚੌੜਾਈ_option = argv[1] + 1;

      /* Make the option we just parsed invisible to getopt.  */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  ਜਦੋਂ ((optchar = getopt_long (argc, argv, "0123456789cstuw:p:g:",
                                 long_options, ਨਿਰਾ))
         != -1)
    ਚੋਣ (optchar)
      {
      ਮੂਲ:
        ਜੇ (c_isdigit (optchar))
          error (0, 0, _("invalid option -- %c; -ਚੌੜਾਈ_2 is recognized\
 only when it is the ਪਹਿਲਾ\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      ਹਾਲਤ 'c':
        crown = true;
        ਰੋਕੋ;

      ਹਾਲਤ 's':
        ਵੰਡੋ = true;
        ਰੋਕੋ;

      ਹਾਲਤ 't':
        tagged = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'u':
        ਇਕਸਾਰ = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'w':
        max_ਚੌੜਾਈ_option = optarg;
        ਰੋਕੋ;

      ਹਾਲਤ 'g':
        goal_ਚੌੜਾਈ_option = optarg;
        ਰੋਕੋ;

      ਹਾਲਤ 'p':
        ਸੈੱਟ_ਅਗੇਤਰ (optarg);
        ਰੋਕੋ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

      }

  ਜੇ (max_ਚੌੜਾਈ_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_ਚੌੜਾਈ = xnumtoumax (max_ਚੌੜਾਈ_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  ਜੇ (goal_ਚੌੜਾਈ_option)
    {
      /* Limit goal_width to max_width.  */
      goal_ਚੌੜਾਈ = xdectoumax (goal_ਚੌੜਾਈ_option, 0, max_ਚੌੜਾਈ, "",
                               _("invalid goal"), 0);
      ਜੇ (max_ਚੌੜਾਈ_option == ਨਿਰਾ)
        max_ਚੌੜਾਈ = goal_ਚੌੜਾਈ + 10;
    }
  ਨਹੀਂਤਾਂ
    {
      goal_ਚੌੜਾਈ = max_ਚੌੜਾਈ * (2 * (100 - LEEWAY) + 1) / 200;
    }

  bool have_read_stdin = false;

  ਜੇ (optind == argc)
    {
      have_read_stdin = true;
      ok = fmt (stdin, "-");
    }
  ਨਹੀਂਤਾਂ
    {
      ਲਈ (; optind < argc; optind++)
        {
          ਅੱਖਰ *file = argv[optind];
          ਜੇ (streq (file, "-"))
            {
              ok &= fmt (stdin, file);
              have_read_stdin = true;
            }
          ਨਹੀਂਤਾਂ
            {
              FILE *ਵਿੱਚ_ਧਾਰਾ;
              ਵਿੱਚ_ਧਾਰਾ = fopen (file, "r");
              ਜੇ (ਵਿੱਚ_ਧਾਰਾ != ਨਿਰਾ)
                ok &= fmt (ਵਿੱਚ_ਧਾਰਾ, file);
              ਨਹੀਂਤਾਂ
                {
                  error (0, errno, _("cannot open %s for reading"),
                         quoteaf (file));
                  ok = false;
                }
            }
        }
    }

  ਜੇ (have_read_stdin && fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s", _("closing standard input"));

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Trim space from the front and back of the string P, yielding the prefix,
   and record the lengths of the prefix and the space trimmed.  */

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_ਅਗੇਤਰ (ਅੱਖਰ *p)
{
  ਅੱਖਰ *s;

  ਅਗੇਤਰ_lead_space = 0;
  ਜਦੋਂ (*p == ' ')
    {
      ਅਗੇਤਰ_lead_space++;
      p++;
    }
  ਅਗੇਤਰ = p;
  ਅਗੇਤਰ_ਭਰਿਆ_ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ (p);
  s = p + ਅਗੇਤਰ_ਭਰਿਆ_ਲੰਮਾਈ;
  ਜਦੋਂ (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  ਅਗੇਤਰ_ਲੰਮਾਈ = s - p;
}

/* Read F and send formatted output to stdout.
   Close F when done, unless F is stdin.  Diagnose input errors, using FILE.
   If !F, assume F resulted from an fopen failure and diagnose that.
   Return true if successful.  */

ਸਥਿਰ bool
fmt (FILE *f, ਅੱਖਰ ਅਚਲ *file)
{
  fadvise (f, FADVISE_SEQUENTIAL);
  tabs = false;
  other_indent = 0;
  ਅਗਲਾ_char = get_ਅਗੇਤਰ (f);
  ਜਦੋਂ (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (ਸ਼ਬਦ_ਹੱਦਬੰਦੀ);
    }

  ਪੂਰਨ err = ferror (f) ? 0 : -1;
  /* err < 0 means success.  */
  ਜੇ (f == stdin)
    clearerr (f);
  ਨਹੀਂਤਾਂ ਜੇ (fclose (f) != 0 && err < 0)
    err = errno;
  ਜੇ (0 <= err)
    {
      ਜੇ (f == stdin)
        error (0, errno, _("read error"));
      ਨਹੀਂਤਾਂ
        error (0, errno, _("error reading %s"), quoteaf (file));
    }
  ਵਾਪਸ err < 0;
}

/* Set the global variable 'other_indent' according to SAME_PARAGRAPH
   and other global variables.  */

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_other_indent (bool same_paragraph)
{
  ਜੇ (ਵੰਡੋ)
    other_indent = ਪਹਿਲਾ_indent;
  ਨਹੀਂਤਾਂ ਜੇ (crown)
    {
      other_indent = (same_paragraph ? ਵਿੱਚ_ਕਾਲਮ : ਪਹਿਲਾ_indent);
    }
  ਨਹੀਂਤਾਂ ਜੇ (tagged)
    {
      ਜੇ (same_paragraph && ਵਿੱਚ_ਕਾਲਮ != ਪਹਿਲਾ_indent)
        {
          other_indent = ਵਿੱਚ_ਕਾਲਮ;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      ਨਹੀਂਤਾਂ ਜੇ (other_indent == ਪਹਿਲਾ_indent)
        other_indent = ਪਹਿਲਾ_indent == 0 ? DEF_INDENT : 0;
    }
  ਨਹੀਂਤਾਂ
    {
      other_indent = ਪਹਿਲਾ_indent;
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

ਸਥਿਰ bool
get_paragraph (FILE *f)
{
  ਪੂਰਨ c;

  ਆਖਰੀ_ਸਤਰ_ਲੰਮਾਈ = 0;
  c = ਅਗਲਾ_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  ਜਦੋਂ (c == '\n' || c == EOF
         || ਅਗਲਾ_ਅਗੇਤਰ_indent < ਅਗੇਤਰ_lead_space
         || ਵਿੱਚ_ਕਾਲਮ < ਅਗਲਾ_ਅਗੇਤਰ_indent + ਅਗੇਤਰ_ਭਰਿਆ_ਲੰਮਾਈ)
    {
      c = copy_rest (f, c);
      ਜੇ (c == EOF)
        {
          ਅਗਲਾ_char = EOF;
          ਵਾਪਸ false;
        }
      ਅੱਖਰ_ਲਿਖੋ ('\n');
      c = get_ਅਗੇਤਰ (f);
    }

  /* Got a suitable first line for a paragraph.  */

  ਅਗੇਤਰ_indent = ਅਗਲਾ_ਅਗੇਤਰ_indent;
  ਪਹਿਲਾ_indent = ਵਿੱਚ_ਕਾਲਮ;
  wptr = parabuf;
  ਸ਼ਬਦ_ਹੱਦਬੰਦੀ = ਸ਼ਬਦ_2;
  c = get_ਸਤਰ (f, c);
  ਸੈੱਟ_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  ਜੇ (ਵੰਡੋ)
    {
      /*ਖਾਲੀ_ਥਾਵਾਂ*/
    }
  ਨਹੀਂਤਾਂ ਜੇ (crown)
    {
      ਜੇ (same_para (c))
        {
          ਕਰੋ
            {			/* for each line till the end of the para */
              c = get_ਸਤਰ (f, c);
            }
          ਜਦੋਂ (same_para (c) && ਵਿੱਚ_ਕਾਲਮ == other_indent);
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (tagged)
    {
      ਜੇ (same_para (c) && ਵਿੱਚ_ਕਾਲਮ != ਪਹਿਲਾ_indent)
        {
          ਕਰੋ
            {			/* for each line till the end of the para */
              c = get_ਸਤਰ (f, c);
            }
          ਜਦੋਂ (same_para (c) && ਵਿੱਚ_ਕਾਲਮ == other_indent);
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਜਦੋਂ (same_para (c) && ਵਿੱਚ_ਕਾਲਮ == other_indent)
        c = get_ਸਤਰ (f, c);
    }

  (ਸ਼ਬਦ_ਹੱਦਬੰਦੀ - 1)->period = (ਸ਼ਬਦ_ਹੱਦਬੰਦੀ - 1)->final = true;
  ਅਗਲਾ_char = c;
  ਵਾਪਸ true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

ਸਥਿਰ ਪੂਰਨ
copy_rest (FILE *f, ਪੂਰਨ c)
{
  ਬਾਹਰਲਾ_ਕਾਲਮ = 0;
  ਜੇ (ਵਿੱਚ_ਕਾਲਮ > ਅਗਲਾ_ਅਗੇਤਰ_indent || (c != '\n' && c != EOF))
    {
      put_space (ਅਗਲਾ_ਅਗੇਤਰ_indent);
      ਲਈ (ਅੱਖਰ ਅਚਲ *s = ਅਗੇਤਰ; ਬਾਹਰਲਾ_ਕਾਲਮ != ਵਿੱਚ_ਕਾਲਮ && *s; ਬਾਹਰਲਾ_ਕਾਲਮ++)
        ਅੱਖਰ_ਲਿਖੋ (*s++);
      ਜੇ (c != EOF && c != '\n')
        put_space (ਵਿੱਚ_ਕਾਲਮ - ਬਾਹਰਲਾ_ਕਾਲਮ);
      ਜੇ (c == EOF && ਵਿੱਚ_ਕਾਲਮ >= ਅਗਲਾ_ਅਗੇਤਰ_indent + ਅਗੇਤਰ_ਲੰਮਾਈ)
        ਅੱਖਰ_ਲਿਖੋ ('\n');
    }
  ਜਦੋਂ (c != '\n' && c != EOF)
    {
      ਅੱਖਰ_ਲਿਖੋ (c);
      c = getc (f);
    }
  ਵਾਪਸ c;
}

/* Return true if a line whose first non-blank character after the
   prefix (if any) is C could belong to the current paragraph,
   otherwise false.  */

ਸਥਿਰ bool
same_para (ਪੂਰਨ c)
{
  ਵਾਪਸ (ਅਗਲਾ_ਅਗੇਤਰ_indent == ਅਗੇਤਰ_indent
          && ਵਿੱਚ_ਕਾਲਮ >= ਅਗਲਾ_ਅਗੇਤਰ_indent + ਅਗੇਤਰ_ਭਰਿਆ_ਲੰਮਾਈ
          && c != '\n' && c != EOF);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

ਸਥਿਰ ਪੂਰਨ
get_ਸਤਰ (FILE *f, ਪੂਰਨ c)
{
  ਪੂਰਨ ਸ਼ੁਰੂ;
  ਅੱਖਰ *ਸਿਰਾ_ਦਾ_parabuf;
  ਸ਼ਬਦ *ਸਿਰਾ_ਦਾ_ਸ਼ਬਦ;

  ਸਿਰਾ_ਦਾ_parabuf = &parabuf[MAXCHARS];
  ਸਿਰਾ_ਦਾ_ਸ਼ਬਦ = &ਸ਼ਬਦ_2[MAXWORDS - 2];

  ਕਰੋ
    {				/* for each word in a line */

      /* Scan word.  */

      ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->ਪਾਠ = wptr;
      ਕਰੋ
        {
          ਜੇ (wptr == ਸਿਰਾ_ਦਾ_parabuf)
            {
              ਸੈੱਟ_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      ਜਦੋਂ (c != EOF && !c_isspace (c));
      ਵਿੱਚ_ਕਾਲਮ += ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->ਲੰਮਾਈ = wptr - ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->ਪਾਠ;
      check_punctuation (ਸ਼ਬਦ_ਹੱਦਬੰਦੀ);

      /* Scan inter-word space.  */

      ਸ਼ੁਰੂ = ਵਿੱਚ_ਕਾਲਮ;
      c = get_space (f, c);
      ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->space = ਵਿੱਚ_ਕਾਲਮ - ਸ਼ੁਰੂ;
      ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->final = (c == EOF
                           || (ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->period
                               && (c == '\n' || ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->space > 1)));
      ਜੇ (c == '\n' || c == EOF || ਇਕਸਾਰ)
        ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->space = ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->final ? 2 : 1;
      ਜੇ (ਸ਼ਬਦ_ਹੱਦਬੰਦੀ == ਸਿਰਾ_ਦਾ_ਸ਼ਬਦ)
        {
          ਸੈੱਟ_other_indent (true);
          flush_paragraph ();
        }
      ਸ਼ਬਦ_ਹੱਦਬੰਦੀ++;
    }
  ਜਦੋਂ (c != '\n' && c != EOF);
  ਵਾਪਸ get_ਅਗੇਤਰ (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

ਸਥਿਰ ਪੂਰਨ
get_ਅਗੇਤਰ (FILE *f)
{
  ਪੂਰਨ c;

  ਵਿੱਚ_ਕਾਲਮ = 0;
  c = get_space (f, getc (f));
  ਜੇ (ਅਗੇਤਰ_ਲੰਮਾਈ == 0)
    ਅਗਲਾ_ਅਗੇਤਰ_indent = ਅਗੇਤਰ_lead_space < ਵਿੱਚ_ਕਾਲਮ ?
      ਅਗੇਤਰ_lead_space : ਵਿੱਚ_ਕਾਲਮ;
  ਨਹੀਂਤਾਂ
    {
      ਅਗਲਾ_ਅਗੇਤਰ_indent = ਵਿੱਚ_ਕਾਲਮ;
      ਲਈ (ਅੱਖਰ ਅਚਲ *p = ਅਗੇਤਰ; *p != '\0'; p++)
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ pc = *p;
          ਜੇ (c != pc)
            ਵਾਪਸ c;
          ਵਿੱਚ_ਕਾਲਮ++;
          c = getc (f);
        }
      c = get_space (f, c);
    }
  ਵਾਪਸ c;
}

/* Read blank characters from input file F, starting with C, and keeping
   in_column up-to-date.  Return first non-blank character.  */

ਸਥਿਰ ਪੂਰਨ
get_space (FILE *f, ਪੂਰਨ c)
{
  ਜਦੋਂ (true)
    {
      ਜੇ (c == ' ')
        ਵਿੱਚ_ਕਾਲਮ++;
      ਨਹੀਂਤਾਂ ਜੇ (c == '\t')
        {
          tabs = true;
          ਵਿੱਚ_ਕਾਲਮ = (ਵਿੱਚ_ਕਾਲਮ / TABWIDTH + 1) * TABWIDTH;
        }
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ c;
      c = getc (f);
    }
}

/* Set extra fields in word W describing any attached punctuation.  */

ਸਥਿਰ ਖਾਲੀ
check_punctuation (ਸ਼ਬਦ *w)
{
  ਅੱਖਰ ਅਚਲ *ਸ਼ੁਰੂ = w->ਪਾਠ;
  ਅੱਖਰ ਅਚਲ *ਅੰਤ = ਸ਼ੁਰੂ + (w->ਲੰਮਾਈ - 1);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ fin = *ਅੰਤ;

  w->paren = isopen (*ਸ਼ੁਰੂ);
  w->punct = !! ispunct (fin);
  ਜਦੋਂ (ਸ਼ੁਰੂ < ਅੰਤ && isclose (*ਅੰਤ))
    ਅੰਤ--;
  w->period = isperiod (*ਅੰਤ);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

ਸਥਿਰ ਖਾਲੀ
flush_paragraph (ਖਾਲੀ)
{
  ਸ਼ਬਦ *ਵੰਡੋ_ਬਿੰਦੂ;
  ਸ਼ਬਦ *w;
  ਪੂਰਨ ਖਿਸਕਾਓ;
  ਲਾਗਤ ਉੱਤਮ_break;

  /* In the special case where it's all one word, just flush it.  */

  ਜੇ (ਸ਼ਬਦ_ਹੱਦਬੰਦੀ == ਸ਼ਬਦ_2)
    {
      size_t ਮੰਜ਼ਿਲ_write = wptr - parabuf;
      ਜੇ (fwrite (parabuf, 1, ਮੰਜ਼ਿਲ_write, stdout) != ਮੰਜ਼ਿਲ_write)
        write_error ();

      wptr = parabuf;
      ਵਾਪਸ;
    }

  /* Otherwise:
     - format what you have so far as a paragraph,
     - find a low-cost line break near the end,
     - output to there,
     - make that the start of the paragraph.  */

  fmt_paragraph ();

  /* Choose a good split point.  */

  ਵੰਡੋ_ਬਿੰਦੂ = ਸ਼ਬਦ_ਹੱਦਬੰਦੀ;
  ਉੱਤਮ_break = MAXCOST;
  ਲਈ (w = ਸ਼ਬਦ_2->ਅਗਲਾ_break; w != ਸ਼ਬਦ_ਹੱਦਬੰਦੀ; w = w->ਅਗਲਾ_break)
    {
      ਜੇ (w->ਉੱਤਮ_ਲਾਗਤ - w->ਅਗਲਾ_break->ਉੱਤਮ_ਲਾਗਤ < ਉੱਤਮ_break)
        {
          ਵੰਡੋ_ਬਿੰਦੂ = w;
          ਉੱਤਮ_break = w->ਉੱਤਮ_ਲਾਗਤ - w->ਅਗਲਾ_break->ਉੱਤਮ_ਲਾਗਤ;
        }
      ਜੇ (ਉੱਤਮ_break <= MAXCOST - ਸਤਰ_CREDIT)
        ਉੱਤਮ_break += ਸਤਰ_CREDIT;
    }
  put_paragraph (ਵੰਡੋ_ਬਿੰਦੂ);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, ਵੰਡੋ_ਬਿੰਦੂ->ਪਾਠ, wptr - ਵੰਡੋ_ਬਿੰਦੂ->ਪਾਠ);
  ਖਿਸਕਾਓ = ਵੰਡੋ_ਬਿੰਦੂ->ਪਾਠ - parabuf;
  wptr -= ਖਿਸਕਾਓ;

  /* Adjust text pointers.  */

  ਲਈ (w = ਵੰਡੋ_ਬਿੰਦੂ; w <= ਸ਼ਬਦ_ਹੱਦਬੰਦੀ; w++)
    w->ਪਾਠ -= ਖਿਸਕਾਓ;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (ਸ਼ਬਦ_2, ਵੰਡੋ_ਬਿੰਦੂ, (ਸ਼ਬਦ_ਹੱਦਬੰਦੀ - ਵੰਡੋ_ਬਿੰਦੂ + 1) * ਆਕਾਰ *ਸ਼ਬਦ_2);
  ਸ਼ਬਦ_ਹੱਦਬੰਦੀ -= ਵੰਡੋ_ਬਿੰਦੂ - ਸ਼ਬਦ_2;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

ਸਥਿਰ ਖਾਲੀ
fmt_paragraph (ਖਾਲੀ)
{
  ਸ਼ਬਦ *w;
  ਪੂਰਨ len;
  ਲਾਗਤ wcost, ਉੱਤਮ;
  ਪੂਰਨ saved_ਲੰਮਾਈ;

  ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->ਉੱਤਮ_ਲਾਗਤ = 0;
  saved_ਲੰਮਾਈ = ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->ਲੰਮਾਈ;
  ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->ਲੰਮਾਈ = max_ਚੌੜਾਈ;	/* sentinel */

  ਲਈ (ਸ਼ਬਦ *ਸ਼ੁਰੂ = ਸ਼ਬਦ_ਹੱਦਬੰਦੀ - 1; ਸ਼ੁਰੂ >= ਸ਼ਬਦ_2; ਸ਼ੁਰੂ--)
    {
      ਉੱਤਮ = MAXCOST;
      len = ਸ਼ੁਰੂ == ਸ਼ਬਦ_2 ? ਪਹਿਲਾ_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = ਸ਼ੁਰੂ;
      len += w->ਲੰਮਾਈ;
      ਕਰੋ
        {
          w++;

          /* Consider breaking before w.  */

          wcost = ਸਤਰ_ਲਾਗਤ_2 (w, len) + w->ਉੱਤਮ_ਲਾਗਤ;
          ਜੇ (ਸ਼ੁਰੂ == ਸ਼ਬਦ_2 && ਆਖਰੀ_ਸਤਰ_ਲੰਮਾਈ > 0)
            wcost += RAGGED_ਲਾਗਤ (len - ਆਖਰੀ_ਸਤਰ_ਲੰਮਾਈ);
          ਜੇ (wcost < ਉੱਤਮ)
            {
              ਉੱਤਮ = wcost;
              ਸ਼ੁਰੂ->ਅਗਲਾ_break = w;
              ਸ਼ੁਰੂ->ਸਤਰ_ਲੰਮਾਈ = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          ਜੇ (w == ਸ਼ਬਦ_ਹੱਦਬੰਦੀ)
            ਰੋਕੋ;

          len += (w - 1)->space + w->ਲੰਮਾਈ;	/* w > start >= word */
        }
      ਜਦੋਂ (len <= max_ਚੌੜਾਈ);
      ਸ਼ੁਰੂ->ਉੱਤਮ_ਲਾਗਤ = ਉੱਤਮ + ਅਧਾਰ_ਲਾਗਤ (ਸ਼ੁਰੂ);
    }

  ਸ਼ਬਦ_ਹੱਦਬੰਦੀ->ਲੰਮਾਈ = saved_ਲੰਮਾਈ;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#ਜੇ __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#ਜੇ_ਅੰਤ

/* Return the constant component of the cost of breaking before the
   word THIS.  */

ਸਥਿਰ ਲਾਗਤ
ਅਧਾਰ_ਲਾਗਤ (ਸ਼ਬਦ *this)
{
  ਲਾਗਤ ਲਾਗਤ_2;

  ਲਾਗਤ_2 = ਸਤਰ_ਲਾਗਤ;

  ਜੇ (this > ਸ਼ਬਦ_2)
    {
      ਜੇ ((this - 1)->period)
        {
          ਜੇ ((this - 1)->final)
            ਲਾਗਤ_2 -= SENTENCE_BONUS;
          ਨਹੀਂਤਾਂ
            ਲਾਗਤ_2 += NOBREAK_ਲਾਗਤ;
        }
      ਨਹੀਂਤਾਂ ਜੇ ((this - 1)->punct)
        ਲਾਗਤ_2 -= PUNCT_BONUS;
      ਨਹੀਂਤਾਂ ਜੇ (this > ਸ਼ਬਦ_2 + 1 && (this - 2)->final)
        ਲਾਗਤ_2 += WIDOW_ਲਾਗਤ ((this - 1)->ਲੰਮਾਈ);
    }

  ਜੇ (this->paren)
    ਲਾਗਤ_2 -= PAREN_BONUS;
  ਨਹੀਂਤਾਂ ਜੇ (this->final)
    ਲਾਗਤ_2 += ORPHAN_ਲਾਗਤ (this->ਲੰਮਾਈ);

  ਵਾਪਸ ਲਾਗਤ_2;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

ਸਥਿਰ ਲਾਗਤ
ਸਤਰ_ਲਾਗਤ_2 (ਸ਼ਬਦ *ਅਗਲਾ, ਪੂਰਨ len)
{
  ਪੂਰਨ n;
  ਲਾਗਤ ਲਾਗਤ_2;

  ਜੇ (ਅਗਲਾ == ਸ਼ਬਦ_ਹੱਦਬੰਦੀ)
    ਵਾਪਸ 0;
  n = goal_ਚੌੜਾਈ - len;
  ਲਾਗਤ_2 = SHORT_ਲਾਗਤ (n);
  ਜੇ (ਅਗਲਾ->ਅਗਲਾ_break != ਸ਼ਬਦ_ਹੱਦਬੰਦੀ)
    {
      n = len - ਅਗਲਾ->ਸਤਰ_ਲੰਮਾਈ;
      ਲਾਗਤ_2 += RAGGED_ਲਾਗਤ (n);
    }
  ਵਾਪਸ ਲਾਗਤ_2;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

ਸਥਿਰ ਖਾਲੀ
put_paragraph (ਸ਼ਬਦ *ਅੰਤ)
{
  put_ਸਤਰ (ਸ਼ਬਦ_2, ਪਹਿਲਾ_indent);
  ਲਈ (ਸ਼ਬਦ *w = ਸ਼ਬਦ_2->ਅਗਲਾ_break; w != ਅੰਤ; w = w->ਅਗਲਾ_break)
    put_ਸਤਰ (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

ਸਥਿਰ ਖਾਲੀ
put_ਸਤਰ (ਸ਼ਬਦ *w, ਪੂਰਨ indent)
{
  ਸ਼ਬਦ *endline;

  ਬਾਹਰਲਾ_ਕਾਲਮ = 0;
  put_space (ਅਗੇਤਰ_indent);
  fputs (ਅਗੇਤਰ, stdout);
  ਬਾਹਰਲਾ_ਕਾਲਮ += ਅਗੇਤਰ_ਲੰਮਾਈ;
  put_space (indent - ਬਾਹਰਲਾ_ਕਾਲਮ);

  endline = w->ਅਗਲਾ_break - 1;
  ਲਈ (; w != endline; w++)
    {
      put_ਸ਼ਬਦ (w);
      put_space (w->space);
    }
  put_ਸ਼ਬਦ (w);
  ਆਖਰੀ_ਸਤਰ_ਲੰਮਾਈ = ਬਾਹਰਲਾ_ਕਾਲਮ;
  ਅੱਖਰ_ਲਿਖੋ ('\n');

  ਜੇ (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

ਸਥਿਰ ਖਾਲੀ
put_ਸ਼ਬਦ (ਸ਼ਬਦ *w)
{
  ਅੱਖਰ ਅਚਲ *s = w->ਪਾਠ;
  ਲਈ (ਪੂਰਨ n = w->ਲੰਮਾਈ; n != 0; n--)
    ਅੱਖਰ_ਲਿਖੋ (*s++);
  ਬਾਹਰਲਾ_ਕਾਲਮ += w->ਲੰਮਾਈ;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

ਸਥਿਰ ਖਾਲੀ
put_space (ਪੂਰਨ space)
{
  ਪੂਰਨ space_ਨਿਸ਼ਾਨਾ, tab_ਨਿਸ਼ਾਨਾ;

  space_ਨਿਸ਼ਾਨਾ = ਬਾਹਰਲਾ_ਕਾਲਮ + space;
  ਜੇ (tabs)
    {
      tab_ਨਿਸ਼ਾਨਾ = space_ਨਿਸ਼ਾਨਾ / TABWIDTH * TABWIDTH;
      ਜੇ (ਬਾਹਰਲਾ_ਕਾਲਮ + 1 < tab_ਨਿਸ਼ਾਨਾ)
        ਜਦੋਂ (ਬਾਹਰਲਾ_ਕਾਲਮ < tab_ਨਿਸ਼ਾਨਾ)
          {
            ਅੱਖਰ_ਲਿਖੋ ('\t');
            ਬਾਹਰਲਾ_ਕਾਲਮ = (ਬਾਹਰਲਾ_ਕਾਲਮ / TABWIDTH + 1) * TABWIDTH;
          }
    }
  ਜਦੋਂ (ਬਾਹਰਲਾ_ਕਾਲਮ < space_ਨਿਸ਼ਾਨਾ)
    {
      ਅੱਖਰ_ਲਿਖੋ (' ');
      ਬਾਹਰਲਾ_ਕਾਲਮ++;
    }
}
