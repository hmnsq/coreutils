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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

/* Redefine.  Otherwise, systems (Unicos for one) with headers that define
   it to be a type get syntax errors for the variable declaration below.  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಶಬ್ದ_2 ಬಳಸದ_ಶಬ್ದ_type

#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "fmt"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Ross Paterson")

/* The following parameters represent the program's idea of what is
   "best".  Adjust to taste, subject to the caveats given.  */

/* Default longest permitted line length (max_width).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಅಗಲ_2	75

/* Prefer lines to be LEEWAY % shorter than the maximum width, giving
   room for optimization.  */
#ವ್ಯಾಖ್ಯಾನಿಸು LEEWAY	7

/* The default secondary indent of tagged paragraph used for unindented
   one-line paragraphs not preceded by any multi-line paragraphs.  */
#ವ್ಯಾಖ್ಯಾನಿಸು DEF_INDENT 3

/* Costs and bonuses are expressed as the equivalent departure from the
   optimal line length, multiplied by 10.  e.g. assigning something a
   cost of 50 means that it is as bad as a line 5 characters too short
   or too long.  The definition of SHORT_COST(n) should not be changed.
   However, EQUIV(n) may need tuning.  */

/* FIXME: "fmt" misbehaves given large inputs or options.  One
   possible workaround for part of the problem is to change COST to be
   a floating-point type.  There are other problems besides COST,
   though; see MAXWORDS below.  */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ ವೆಚ್ಚ;

#ವ್ಯಾಖ್ಯಾನಿಸು MAXCOST	TYPE_ಗರಿಷ್ಠ (ವೆಚ್ಚ)

#ವ್ಯಾಖ್ಯಾನಿಸು SQR(n)		((n) * (n))
#ವ್ಯಾಖ್ಯಾನಿಸು EQUIV(n)	SQR ((ವೆಚ್ಚ) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#ವ್ಯಾಖ್ಯಾನಿಸು SHORT_ವೆಚ್ಚ(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#ವ್ಯಾಖ್ಯಾನಿಸು RAGGED_ವೆಚ್ಚ(n)	(SHORT_ವೆಚ್ಚ (n) / 2)

/* Basic cost per line.  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸಾಲು_ವೆಚ್ಚ	EQUIV (70)

/* Cost of breaking a line after the first word of a sentence, where
   the length of the word is N.  */
#ವ್ಯಾಖ್ಯಾನಿಸು WIDOW_ವೆಚ್ಚ(n)	(EQUIV (200) / ((n) + 2))

/* Cost of breaking a line before the last word of a sentence, where
   the length of the word is N.  */
#ವ್ಯಾಖ್ಯಾನಿಸು ORPHAN_ವೆಚ್ಚ(n)	(EQUIV (150) / ((n) + 2))

/* Bonus for breaking a line at the end of a sentence.  */
#ವ್ಯಾಖ್ಯಾನಿಸು SENTENCE_BONUS	EQUIV (50)

/* Cost of breaking a line after a period not marking end of a sentence.
   With the definition of sentence we are using (borrowed from emacs, see
   get_line()) such a break would then look like a sentence break.  Hence
   we assign a very high cost -- it should be avoided unless things are
   really bad.  */
#ವ್ಯಾಖ್ಯಾನಿಸು NOBREAK_ವೆಚ್ಚ	EQUIV (600)

/* Bonus for breaking a line before open parenthesis.  */
#ವ್ಯಾಖ್ಯಾನಿಸು PAREN_BONUS	EQUIV (40)

/* Bonus for breaking a line after other punctuation.  */
#ವ್ಯಾಖ್ಯಾನಿಸು PUNCT_BONUS	EQUIV(40)

/* Credit for breaking a long paragraph one line later.  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸಾಲು_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#ವ್ಯಾಖ್ಯಾನಿಸು MAXWORDS	1000
#ವ್ಯಾಖ್ಯಾನಿಸು MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#ವ್ಯಾಖ್ಯಾನಿಸು isopen(c)	(strchr ("(['`\"", c) != ಖಾಲಿ)
#ವ್ಯಾಖ್ಯಾನಿಸು isclose(c)	(strchr (")]'\"", c) != ಖಾಲಿ)
#ವ್ಯಾಖ್ಯಾನಿಸು isperiod(c)	(strchr (".?!", c) != ಖಾಲಿ)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#ವ್ಯಾಖ್ಯಾನಿಸು TABWIDTH	8

/* Word descriptor structure.  */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ಶಬ್ದ_3 ಶಬ್ದ;

ರಚನೆ ಶಬ್ದ_3
  {

    /* Static attributes determined during input.  */

    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪಠ್ಯ;		/* the text of the word */
    ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ;			/* length of this word */
    ಪೂರ್ಣಾಂಕ space;			/* the size of the following space */
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ paren:1;	/* starts with open paren */
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ period:1;	/* ends in [.?!])* */
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ punct:1;	/* ends in punctuation */
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    ಪೂರ್ಣಾಂಕ ಸಾಲು_ಉದ್ದಳತೆ;		/* length of the best line starting here */
    ವೆಚ್ಚ ಉತ್ತಮ_ವೆಚ್ಚ;		/* cost of best paragraph starting here */
    ಶಬ್ದ *ಮುಂದಿನ_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ಪೂರ್ವಪ್ರತ್ಯಯ (ಅಕ್ಷರ *p);
ಸ್ಥಿರ bool fmt (FILE *f, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *);
ಸ್ಥಿರ bool get_paragraph (FILE *f);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ಸಾಲು (FILE *f, ಪೂರ್ಣಾಂಕ c);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ಪೂರ್ವಪ್ರತ್ಯಯ (FILE *f);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_space (FILE *f, ಪೂರ್ಣಾಂಕ c);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ copy_rest (FILE *f, ಪೂರ್ಣಾಂಕ c);
ಸ್ಥಿರ bool same_para (ಪೂರ್ಣಾಂಕ c);
ಸ್ಥಿರ ಶೂನ್ಯ flush_paragraph (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ fmt_paragraph (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ check_punctuation (ಶಬ್ದ *w);
ಸ್ಥಿರ ವೆಚ್ಚ ತಳ_ವೆಚ್ಚ (ಶಬ್ದ *this);
ಸ್ಥಿರ ವೆಚ್ಚ ಸಾಲು_ವೆಚ್ಚ_2 (ಶಬ್ದ *ಮುಂದಿನ, ಪೂರ್ಣಾಂಕ len);
ಸ್ಥಿರ ಶೂನ್ಯ put_paragraph (ಶಬ್ದ *ಮುಕ್ತಾಯ);
ಸ್ಥಿರ ಶೂನ್ಯ put_ಸಾಲು (ಶಬ್ದ *w, ಪೂರ್ಣಾಂಕ indent);
ಸ್ಥಿರ ಶೂನ್ಯ put_ಶಬ್ದ (ಶಬ್ದ *w);
ಸ್ಥಿರ ಶೂನ್ಯ put_space (ಪೂರ್ಣಾಂಕ space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
ಸ್ಥಿರ bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
ಸ್ಥಿರ bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
ಸ್ಥಿರ bool ವಿಭಜಿಸು;

/* If true, don't preserve inter-word spacing (default false).  */
ಸ್ಥಿರ bool ಏಕರೂಪ;

/* Prefix minus leading and trailing spaces (default "").  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪೂರ್ವಪ್ರತ್ಯಯ = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ max_ಅಗಲ = ಅಗಲ_2;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_ತುಂಬಿದ_ಉದ್ದಳತೆ;

/* The length of the leading space trimmed from the prefix.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_lead_space;

/* The length of prefix minus leading and trailing space.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ goal_ಅಗಲ;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಒಳ_ಸ್ತಂಭ;

/* Start column of the next character to be written to stdout.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_ಸ್ತಂಭ;

/* Space for the paragraph text -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ಸ್ಥಿರ ಅಕ್ಷರ parabuf[MAXCHARS];

/* A pointer into parabuf, indicating the first unused character position.  */
ಸ್ಥಿರ ಅಕ್ಷರ *wptr;

/* The words of a paragraph -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
ಸ್ಥಿರ ಶಬ್ದ ಶಬ್ದ_2[MAXWORDS];

/* A pointer into the above word array, indicating the first position
   after the last complete word.  Sometimes it will point at an incomplete
   word.  */
ಸ್ಥಿರ ಶಬ್ದ *ಶಬ್ದ_ಎಲ್ಲೆ;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
ಸ್ಥಿರ bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_indent;

/* Indentation of the first line of the current paragraph.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮೊದಲ_indent;

/* Indentation of other lines of the current paragraph */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮುಂದಿನ_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮುಂದಿನ_ಪೂರ್ವಪ್ರತ್ಯಯ_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_ಸಾಲು_ಉದ್ದಳತೆ;

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [-WIDTH] [OPTION]... [FILE]...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Reformat each paragraph ಒಳ the FILE(s), writing ಗಮ್ಯ standard output.\n\
The option -ಅಗಲ_2 is an abbreviated form ದ --ಅಗಲ=DIGITS.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --crown-margin\n\
         preserve indentation ದ ಮೊದಲ two ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -p, --ಪೂರ್ವಪ್ರತ್ಯಯ=STRING\n\
         reformat only ಸಾಲುಗಳು beginning with STRING,\n\
         reattaching the ಪೂರ್ವಪ್ರತ್ಯಯ ಗಮ್ಯ reformatted ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -s, --ವಿಭಜಿಸು-only\n\
         ವಿಭಜಿಸು ದೀರ್ಘ ಸಾಲುಗಳು, but ಮಾಡು not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation ದ ಮೊದಲ ಸಾಲು different from ಎರಡನೇ\n\
"));
      oputs (_("\
  -u, --ಏಕರೂಪ-spacing\n\
         one space between ಪದಗಳು, two after sentences\n\
"));
      oputs (_("\
  -w, --ಅಗಲ=ಅಗಲ_2\n\
         ಗರಿಷ್ಠ ಸಾಲು ಅಗಲ (ಪೂರ್ವನಿಯೋಜಿತ ದ 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=ಅಗಲ_2\n\
         goal ಅಗಲ (ಪೂರ್ವನಿಯೋಜಿತ ದ 93% ದ ಅಗಲ)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Decode options and launch execution.  */

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"crown-margin", no_ವಾದ, ಖಾಲಿ, 'c'},
  {"prefix", required_ವಾದ, ಖಾಲಿ, 'p'},
  {"split-only", no_ವಾದ, ಖಾಲಿ, 's'},
  {"tagged-paragraph", no_ವಾದ, ಖಾಲಿ, 't'},
  {"uniform-spacing", no_ವಾದ, ಖಾಲಿ, 'u'},
  {"width", required_ವಾದ, ಖಾಲಿ, 'w'},
  {"goal", required_ವಾದ, ಖಾಲಿ, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0},
};

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ optchar;
  bool ok = true;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *max_ಅಗಲ_option = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *goal_ಅಗಲ_option = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆದರೆ (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_ಅಗಲ_option = argv[1] + 1;

      /* Make the option we just parsed invisible to getopt.  */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  ಆಗ ((optchar = getopt_long (argc, argv, "0123456789cstuw:p:g:",
                                 long_options, ಖಾಲಿ))
         != -1)
    ಆಯ್ಕೆ (optchar)
      {
      ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (c_isdigit (optchar))
          error (0, 0, _("invalid option -- %c; -ಅಗಲ_2 is recognized\
 only when it is the ಮೊದಲ\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      ಸ್ಥಿತಿ 'c':
        crown = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 's':
        ವಿಭಜಿಸು = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 't':
        tagged = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'u':
        ಏಕರೂಪ = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'w':
        max_ಅಗಲ_option = optarg;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'g':
        goal_ಅಗಲ_option = optarg;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'p':
        ಗಣ_ಪೂರ್ವಪ್ರತ್ಯಯ (optarg);
        ನಿಲ್ಲಿಸು;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

      }

  ಆದರೆ (max_ಅಗಲ_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_ಅಗಲ = xnumtoumax (max_ಅಗಲ_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  ಆದರೆ (goal_ಅಗಲ_option)
    {
      /* Limit goal_width to max_width.  */
      goal_ಅಗಲ = xdectoumax (goal_ಅಗಲ_option, 0, max_ಅಗಲ, "",
                               _("invalid goal"), 0);
      ಆದರೆ (max_ಅಗಲ_option == ಖಾಲಿ)
        max_ಅಗಲ = goal_ಅಗಲ + 10;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      goal_ಅಗಲ = max_ಅಗಲ * (2 * (100 - LEEWAY) + 1) / 200;
    }

  bool have_read_stdin = false;

  ಆದರೆ (optind == argc)
    {
      have_read_stdin = true;
      ok = fmt (stdin, "-");
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಗಾಗಿ (; optind < argc; optind++)
        {
          ಅಕ್ಷರ *file = argv[optind];
          ಆದರೆ (streq (file, "-"))
            {
              ok &= fmt (stdin, file);
              have_read_stdin = true;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              FILE *ಒಳ_ಹರಿವು;
              ಒಳ_ಹರಿವು = fopen (file, "r");
              ಆದರೆ (ಒಳ_ಹರಿವು != ಖಾಲಿ)
                ok &= fmt (ಒಳ_ಹರಿವು, file);
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  error (0, errno, _("cannot open %s for reading"),
                         quoteaf (file));
                  ok = false;
                }
            }
        }
    }

  ಆದರೆ (have_read_stdin && fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s", _("closing standard input"));

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Trim space from the front and back of the string P, yielding the prefix,
   and record the lengths of the prefix and the space trimmed.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_ಪೂರ್ವಪ್ರತ್ಯಯ (ಅಕ್ಷರ *p)
{
  ಅಕ್ಷರ *s;

  ಪೂರ್ವಪ್ರತ್ಯಯ_lead_space = 0;
  ಆಗ (*p == ' ')
    {
      ಪೂರ್ವಪ್ರತ್ಯಯ_lead_space++;
      p++;
    }
  ಪೂರ್ವಪ್ರತ್ಯಯ = p;
  ಪೂರ್ವಪ್ರತ್ಯಯ_ತುಂಬಿದ_ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (p);
  s = p + ಪೂರ್ವಪ್ರತ್ಯಯ_ತುಂಬಿದ_ಉದ್ದಳತೆ;
  ಆಗ (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ = s - p;
}

/* Read F and send formatted output to stdout.
   Close F when done, unless F is stdin.  Diagnose input errors, using FILE.
   If !F, assume F resulted from an fopen failure and diagnose that.
   Return true if successful.  */

ಸ್ಥಿರ bool
fmt (FILE *f, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  fadvise (f, FADVISE_SEQUENTIAL);
  tabs = false;
  other_indent = 0;
  ಮುಂದಿನ_char = get_ಪೂರ್ವಪ್ರತ್ಯಯ (f);
  ಆಗ (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (ಶಬ್ದ_ಎಲ್ಲೆ);
    }

  ಪೂರ್ಣಾಂಕ err = ferror (f) ? 0 : -1;
  /* err < 0 means success.  */
  ಆದರೆ (f == stdin)
    clearerr (f);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose (f) != 0 && err < 0)
    err = errno;
  ಆದರೆ (0 <= err)
    {
      ಆದರೆ (f == stdin)
        error (0, errno, _("read error"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, errno, _("error reading %s"), quoteaf (file));
    }
  ಹಿಂತಿರುಗಿ err < 0;
}

/* Set the global variable 'other_indent' according to SAME_PARAGRAPH
   and other global variables.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_other_indent (bool same_paragraph)
{
  ಆದರೆ (ವಿಭಜಿಸು)
    other_indent = ಮೊದಲ_indent;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (crown)
    {
      other_indent = (same_paragraph ? ಒಳ_ಸ್ತಂಭ : ಮೊದಲ_indent);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (tagged)
    {
      ಆದರೆ (same_paragraph && ಒಳ_ಸ್ತಂಭ != ಮೊದಲ_indent)
        {
          other_indent = ಒಳ_ಸ್ತಂಭ;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (other_indent == ಮೊದಲ_indent)
        other_indent = ಮೊದಲ_indent == 0 ? DEF_INDENT : 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      other_indent = ಮೊದಲ_indent;
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

ಸ್ಥಿರ bool
get_paragraph (FILE *f)
{
  ಪೂರ್ಣಾಂಕ c;

  ಕೊನೆಯ_ಸಾಲು_ಉದ್ದಳತೆ = 0;
  c = ಮುಂದಿನ_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  ಆಗ (c == '\n' || c == EOF
         || ಮುಂದಿನ_ಪೂರ್ವಪ್ರತ್ಯಯ_indent < ಪೂರ್ವಪ್ರತ್ಯಯ_lead_space
         || ಒಳ_ಸ್ತಂಭ < ಮುಂದಿನ_ಪೂರ್ವಪ್ರತ್ಯಯ_indent + ಪೂರ್ವಪ್ರತ್ಯಯ_ತುಂಬಿದ_ಉದ್ದಳತೆ)
    {
      c = copy_rest (f, c);
      ಆದರೆ (c == EOF)
        {
          ಮುಂದಿನ_char = EOF;
          ಹಿಂತಿರುಗಿ false;
        }
      ಅಕ್ಷರ_ಬರೆ ('\n');
      c = get_ಪೂರ್ವಪ್ರತ್ಯಯ (f);
    }

  /* Got a suitable first line for a paragraph.  */

  ಪೂರ್ವಪ್ರತ್ಯಯ_indent = ಮುಂದಿನ_ಪೂರ್ವಪ್ರತ್ಯಯ_indent;
  ಮೊದಲ_indent = ಒಳ_ಸ್ತಂಭ;
  wptr = parabuf;
  ಶಬ್ದ_ಎಲ್ಲೆ = ಶಬ್ದ_2;
  c = get_ಸಾಲು (f, c);
  ಗಣ_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  ಆದರೆ (ವಿಭಜಿಸು)
    {
      /*ಖಾಲಿಗಳು*/
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (crown)
    {
      ಆದರೆ (same_para (c))
        {
          ಮಾಡು
            {			/* for each line till the end of the para */
              c = get_ಸಾಲು (f, c);
            }
          ಆಗ (same_para (c) && ಒಳ_ಸ್ತಂಭ == other_indent);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (tagged)
    {
      ಆದರೆ (same_para (c) && ಒಳ_ಸ್ತಂಭ != ಮೊದಲ_indent)
        {
          ಮಾಡು
            {			/* for each line till the end of the para */
              c = get_ಸಾಲು (f, c);
            }
          ಆಗ (same_para (c) && ಒಳ_ಸ್ತಂಭ == other_indent);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆಗ (same_para (c) && ಒಳ_ಸ್ತಂಭ == other_indent)
        c = get_ಸಾಲು (f, c);
    }

  (ಶಬ್ದ_ಎಲ್ಲೆ - 1)->period = (ಶಬ್ದ_ಎಲ್ಲೆ - 1)->final = true;
  ಮುಂದಿನ_char = c;
  ಹಿಂತಿರುಗಿ true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
copy_rest (FILE *f, ಪೂರ್ಣಾಂಕ c)
{
  ಹೊರಹೋಗು_ಸ್ತಂಭ = 0;
  ಆದರೆ (ಒಳ_ಸ್ತಂಭ > ಮುಂದಿನ_ಪೂರ್ವಪ್ರತ್ಯಯ_indent || (c != '\n' && c != EOF))
    {
      put_space (ಮುಂದಿನ_ಪೂರ್ವಪ್ರತ್ಯಯ_indent);
      ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s = ಪೂರ್ವಪ್ರತ್ಯಯ; ಹೊರಹೋಗು_ಸ್ತಂಭ != ಒಳ_ಸ್ತಂಭ && *s; ಹೊರಹೋಗು_ಸ್ತಂಭ++)
        ಅಕ್ಷರ_ಬರೆ (*s++);
      ಆದರೆ (c != EOF && c != '\n')
        put_space (ಒಳ_ಸ್ತಂಭ - ಹೊರಹೋಗು_ಸ್ತಂಭ);
      ಆದರೆ (c == EOF && ಒಳ_ಸ್ತಂಭ >= ಮುಂದಿನ_ಪೂರ್ವಪ್ರತ್ಯಯ_indent + ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ)
        ಅಕ್ಷರ_ಬರೆ ('\n');
    }
  ಆಗ (c != '\n' && c != EOF)
    {
      ಅಕ್ಷರ_ಬರೆ (c);
      c = getc (f);
    }
  ಹಿಂತಿರುಗಿ c;
}

/* Return true if a line whose first non-blank character after the
   prefix (if any) is C could belong to the current paragraph,
   otherwise false.  */

ಸ್ಥಿರ bool
same_para (ಪೂರ್ಣಾಂಕ c)
{
  ಹಿಂತಿರುಗಿ (ಮುಂದಿನ_ಪೂರ್ವಪ್ರತ್ಯಯ_indent == ಪೂರ್ವಪ್ರತ್ಯಯ_indent
          && ಒಳ_ಸ್ತಂಭ >= ಮುಂದಿನ_ಪೂರ್ವಪ್ರತ್ಯಯ_indent + ಪೂರ್ವಪ್ರತ್ಯಯ_ತುಂಬಿದ_ಉದ್ದಳತೆ
          && c != '\n' && c != EOF);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_ಸಾಲು (FILE *f, ಪೂರ್ಣಾಂಕ c)
{
  ಪೂರ್ಣಾಂಕ ಆರಂಭ;
  ಅಕ್ಷರ *ಕೊನೆ_ದ_parabuf;
  ಶಬ್ದ *ಕೊನೆ_ದ_ಶಬ್ದ;

  ಕೊನೆ_ದ_parabuf = &parabuf[MAXCHARS];
  ಕೊನೆ_ದ_ಶಬ್ದ = &ಶಬ್ದ_2[MAXWORDS - 2];

  ಮಾಡು
    {				/* for each word in a line */

      /* Scan word.  */

      ಶಬ್ದ_ಎಲ್ಲೆ->ಪಠ್ಯ = wptr;
      ಮಾಡು
        {
          ಆದರೆ (wptr == ಕೊನೆ_ದ_parabuf)
            {
              ಗಣ_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      ಆಗ (c != EOF && !c_isspace (c));
      ಒಳ_ಸ್ತಂಭ += ಶಬ್ದ_ಎಲ್ಲೆ->ಉದ್ದಳತೆ = wptr - ಶಬ್ದ_ಎಲ್ಲೆ->ಪಠ್ಯ;
      check_punctuation (ಶಬ್ದ_ಎಲ್ಲೆ);

      /* Scan inter-word space.  */

      ಆರಂಭ = ಒಳ_ಸ್ತಂಭ;
      c = get_space (f, c);
      ಶಬ್ದ_ಎಲ್ಲೆ->space = ಒಳ_ಸ್ತಂಭ - ಆರಂಭ;
      ಶಬ್ದ_ಎಲ್ಲೆ->final = (c == EOF
                           || (ಶಬ್ದ_ಎಲ್ಲೆ->period
                               && (c == '\n' || ಶಬ್ದ_ಎಲ್ಲೆ->space > 1)));
      ಆದರೆ (c == '\n' || c == EOF || ಏಕರೂಪ)
        ಶಬ್ದ_ಎಲ್ಲೆ->space = ಶಬ್ದ_ಎಲ್ಲೆ->final ? 2 : 1;
      ಆದರೆ (ಶಬ್ದ_ಎಲ್ಲೆ == ಕೊನೆ_ದ_ಶಬ್ದ)
        {
          ಗಣ_other_indent (true);
          flush_paragraph ();
        }
      ಶಬ್ದ_ಎಲ್ಲೆ++;
    }
  ಆಗ (c != '\n' && c != EOF);
  ಹಿಂತಿರುಗಿ get_ಪೂರ್ವಪ್ರತ್ಯಯ (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_ಪೂರ್ವಪ್ರತ್ಯಯ (FILE *f)
{
  ಪೂರ್ಣಾಂಕ c;

  ಒಳ_ಸ್ತಂಭ = 0;
  c = get_space (f, getc (f));
  ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ == 0)
    ಮುಂದಿನ_ಪೂರ್ವಪ್ರತ್ಯಯ_indent = ಪೂರ್ವಪ್ರತ್ಯಯ_lead_space < ಒಳ_ಸ್ತಂಭ ?
      ಪೂರ್ವಪ್ರತ್ಯಯ_lead_space : ಒಳ_ಸ್ತಂಭ;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಮುಂದಿನ_ಪೂರ್ವಪ್ರತ್ಯಯ_indent = ಒಳ_ಸ್ತಂಭ;
      ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = ಪೂರ್ವಪ್ರತ್ಯಯ; *p != '\0'; p++)
        {
          ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ pc = *p;
          ಆದರೆ (c != pc)
            ಹಿಂತಿರುಗಿ c;
          ಒಳ_ಸ್ತಂಭ++;
          c = getc (f);
        }
      c = get_space (f, c);
    }
  ಹಿಂತಿರುಗಿ c;
}

/* Read blank characters from input file F, starting with C, and keeping
   in_column up-to-date.  Return first non-blank character.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_space (FILE *f, ಪೂರ್ಣಾಂಕ c)
{
  ಆಗ (true)
    {
      ಆದರೆ (c == ' ')
        ಒಳ_ಸ್ತಂಭ++;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == '\t')
        {
          tabs = true;
          ಒಳ_ಸ್ತಂಭ = (ಒಳ_ಸ್ತಂಭ / TABWIDTH + 1) * TABWIDTH;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ c;
      c = getc (f);
    }
}

/* Set extra fields in word W describing any attached punctuation.  */

ಸ್ಥಿರ ಶೂನ್ಯ
check_punctuation (ಶಬ್ದ *w)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಆರಂಭ = w->ಪಠ್ಯ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮುಕ್ತಾಯ = ಆರಂಭ + (w->ಉದ್ದಳತೆ - 1);
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ fin = *ಮುಕ್ತಾಯ;

  w->paren = isopen (*ಆರಂಭ);
  w->punct = !! ispunct (fin);
  ಆಗ (ಆರಂಭ < ಮುಕ್ತಾಯ && isclose (*ಮುಕ್ತಾಯ))
    ಮುಕ್ತಾಯ--;
  w->period = isperiod (*ಮುಕ್ತಾಯ);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

ಸ್ಥಿರ ಶೂನ್ಯ
flush_paragraph (ಶೂನ್ಯ)
{
  ಶಬ್ದ *ವಿಭಜಿಸು_ಬಿಂದು;
  ಶಬ್ದ *w;
  ಪೂರ್ಣಾಂಕ ಸರಿಸು;
  ವೆಚ್ಚ ಉತ್ತಮ_break;

  /* In the special case where it's all one word, just flush it.  */

  ಆದರೆ (ಶಬ್ದ_ಎಲ್ಲೆ == ಶಬ್ದ_2)
    {
      size_t ಗಮ್ಯ_write = wptr - parabuf;
      ಆದರೆ (fwrite (parabuf, 1, ಗಮ್ಯ_write, stdout) != ಗಮ್ಯ_write)
        write_error ();

      wptr = parabuf;
      ಹಿಂತಿರುಗಿ;
    }

  /* Otherwise:
     - format what you have so far as a paragraph,
     - find a low-cost line break near the end,
     - output to there,
     - make that the start of the paragraph.  */

  fmt_paragraph ();

  /* Choose a good split point.  */

  ವಿಭಜಿಸು_ಬಿಂದು = ಶಬ್ದ_ಎಲ್ಲೆ;
  ಉತ್ತಮ_break = MAXCOST;
  ಗಾಗಿ (w = ಶಬ್ದ_2->ಮುಂದಿನ_break; w != ಶಬ್ದ_ಎಲ್ಲೆ; w = w->ಮುಂದಿನ_break)
    {
      ಆದರೆ (w->ಉತ್ತಮ_ವೆಚ್ಚ - w->ಮುಂದಿನ_break->ಉತ್ತಮ_ವೆಚ್ಚ < ಉತ್ತಮ_break)
        {
          ವಿಭಜಿಸು_ಬಿಂದು = w;
          ಉತ್ತಮ_break = w->ಉತ್ತಮ_ವೆಚ್ಚ - w->ಮುಂದಿನ_break->ಉತ್ತಮ_ವೆಚ್ಚ;
        }
      ಆದರೆ (ಉತ್ತಮ_break <= MAXCOST - ಸಾಲು_CREDIT)
        ಉತ್ತಮ_break += ಸಾಲು_CREDIT;
    }
  put_paragraph (ವಿಭಜಿಸು_ಬಿಂದು);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, ವಿಭಜಿಸು_ಬಿಂದು->ಪಠ್ಯ, wptr - ವಿಭಜಿಸು_ಬಿಂದು->ಪಠ್ಯ);
  ಸರಿಸು = ವಿಭಜಿಸು_ಬಿಂದು->ಪಠ್ಯ - parabuf;
  wptr -= ಸರಿಸು;

  /* Adjust text pointers.  */

  ಗಾಗಿ (w = ವಿಭಜಿಸು_ಬಿಂದು; w <= ಶಬ್ದ_ಎಲ್ಲೆ; w++)
    w->ಪಠ್ಯ -= ಸರಿಸು;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (ಶಬ್ದ_2, ವಿಭಜಿಸು_ಬಿಂದು, (ಶಬ್ದ_ಎಲ್ಲೆ - ವಿಭಜಿಸು_ಬಿಂದು + 1) * ಗಾತ್ರ *ಶಬ್ದ_2);
  ಶಬ್ದ_ಎಲ್ಲೆ -= ವಿಭಜಿಸು_ಬಿಂದು - ಶಬ್ದ_2;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

ಸ್ಥಿರ ಶೂನ್ಯ
fmt_paragraph (ಶೂನ್ಯ)
{
  ಶಬ್ದ *w;
  ಪೂರ್ಣಾಂಕ len;
  ವೆಚ್ಚ wcost, ಉತ್ತಮ;
  ಪೂರ್ಣಾಂಕ saved_ಉದ್ದಳತೆ;

  ಶಬ್ದ_ಎಲ್ಲೆ->ಉತ್ತಮ_ವೆಚ್ಚ = 0;
  saved_ಉದ್ದಳತೆ = ಶಬ್ದ_ಎಲ್ಲೆ->ಉದ್ದಳತೆ;
  ಶಬ್ದ_ಎಲ್ಲೆ->ಉದ್ದಳತೆ = max_ಅಗಲ;	/* sentinel */

  ಗಾಗಿ (ಶಬ್ದ *ಆರಂಭ = ಶಬ್ದ_ಎಲ್ಲೆ - 1; ಆರಂಭ >= ಶಬ್ದ_2; ಆರಂಭ--)
    {
      ಉತ್ತಮ = MAXCOST;
      len = ಆರಂಭ == ಶಬ್ದ_2 ? ಮೊದಲ_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = ಆರಂಭ;
      len += w->ಉದ್ದಳತೆ;
      ಮಾಡು
        {
          w++;

          /* Consider breaking before w.  */

          wcost = ಸಾಲು_ವೆಚ್ಚ_2 (w, len) + w->ಉತ್ತಮ_ವೆಚ್ಚ;
          ಆದರೆ (ಆರಂಭ == ಶಬ್ದ_2 && ಕೊನೆಯ_ಸಾಲು_ಉದ್ದಳತೆ > 0)
            wcost += RAGGED_ವೆಚ್ಚ (len - ಕೊನೆಯ_ಸಾಲು_ಉದ್ದಳತೆ);
          ಆದರೆ (wcost < ಉತ್ತಮ)
            {
              ಉತ್ತಮ = wcost;
              ಆರಂಭ->ಮುಂದಿನ_break = w;
              ಆರಂಭ->ಸಾಲು_ಉದ್ದಳತೆ = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          ಆದರೆ (w == ಶಬ್ದ_ಎಲ್ಲೆ)
            ನಿಲ್ಲಿಸು;

          len += (w - 1)->space + w->ಉದ್ದಳತೆ;	/* w > start >= word */
        }
      ಆಗ (len <= max_ಅಗಲ);
      ಆರಂಭ->ಉತ್ತಮ_ವೆಚ್ಚ = ಉತ್ತಮ + ತಳ_ವೆಚ್ಚ (ಆರಂಭ);
    }

  ಶಬ್ದ_ಎಲ್ಲೆ->ಉದ್ದಳತೆ = saved_ಉದ್ದಳತೆ;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#ಆದರೆ __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#ಆದರೆ_ಅಂತ್ಯ

/* Return the constant component of the cost of breaking before the
   word THIS.  */

ಸ್ಥಿರ ವೆಚ್ಚ
ತಳ_ವೆಚ್ಚ (ಶಬ್ದ *this)
{
  ವೆಚ್ಚ ವೆಚ್ಚ_2;

  ವೆಚ್ಚ_2 = ಸಾಲು_ವೆಚ್ಚ;

  ಆದರೆ (this > ಶಬ್ದ_2)
    {
      ಆದರೆ ((this - 1)->period)
        {
          ಆದರೆ ((this - 1)->final)
            ವೆಚ್ಚ_2 -= SENTENCE_BONUS;
          ಇಲ್ಲದಿದ್ದರೆ
            ವೆಚ್ಚ_2 += NOBREAK_ವೆಚ್ಚ;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((this - 1)->punct)
        ವೆಚ್ಚ_2 -= PUNCT_BONUS;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (this > ಶಬ್ದ_2 + 1 && (this - 2)->final)
        ವೆಚ್ಚ_2 += WIDOW_ವೆಚ್ಚ ((this - 1)->ಉದ್ದಳತೆ);
    }

  ಆದರೆ (this->paren)
    ವೆಚ್ಚ_2 -= PAREN_BONUS;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (this->final)
    ವೆಚ್ಚ_2 += ORPHAN_ವೆಚ್ಚ (this->ಉದ್ದಳತೆ);

  ಹಿಂತಿರುಗಿ ವೆಚ್ಚ_2;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

ಸ್ಥಿರ ವೆಚ್ಚ
ಸಾಲು_ವೆಚ್ಚ_2 (ಶಬ್ದ *ಮುಂದಿನ, ಪೂರ್ಣಾಂಕ len)
{
  ಪೂರ್ಣಾಂಕ n;
  ವೆಚ್ಚ ವೆಚ್ಚ_2;

  ಆದರೆ (ಮುಂದಿನ == ಶಬ್ದ_ಎಲ್ಲೆ)
    ಹಿಂತಿರುಗಿ 0;
  n = goal_ಅಗಲ - len;
  ವೆಚ್ಚ_2 = SHORT_ವೆಚ್ಚ (n);
  ಆದರೆ (ಮುಂದಿನ->ಮುಂದಿನ_break != ಶಬ್ದ_ಎಲ್ಲೆ)
    {
      n = len - ಮುಂದಿನ->ಸಾಲು_ಉದ್ದಳತೆ;
      ವೆಚ್ಚ_2 += RAGGED_ವೆಚ್ಚ (n);
    }
  ಹಿಂತಿರುಗಿ ವೆಚ್ಚ_2;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

ಸ್ಥಿರ ಶೂನ್ಯ
put_paragraph (ಶಬ್ದ *ಮುಕ್ತಾಯ)
{
  put_ಸಾಲು (ಶಬ್ದ_2, ಮೊದಲ_indent);
  ಗಾಗಿ (ಶಬ್ದ *w = ಶಬ್ದ_2->ಮುಂದಿನ_break; w != ಮುಕ್ತಾಯ; w = w->ಮುಂದಿನ_break)
    put_ಸಾಲು (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

ಸ್ಥಿರ ಶೂನ್ಯ
put_ಸಾಲು (ಶಬ್ದ *w, ಪೂರ್ಣಾಂಕ indent)
{
  ಶಬ್ದ *endline;

  ಹೊರಹೋಗು_ಸ್ತಂಭ = 0;
  put_space (ಪೂರ್ವಪ್ರತ್ಯಯ_indent);
  fputs (ಪೂರ್ವಪ್ರತ್ಯಯ, stdout);
  ಹೊರಹೋಗು_ಸ್ತಂಭ += ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ;
  put_space (indent - ಹೊರಹೋಗು_ಸ್ತಂಭ);

  endline = w->ಮುಂದಿನ_break - 1;
  ಗಾಗಿ (; w != endline; w++)
    {
      put_ಶಬ್ದ (w);
      put_space (w->space);
    }
  put_ಶಬ್ದ (w);
  ಕೊನೆಯ_ಸಾಲು_ಉದ್ದಳತೆ = ಹೊರಹೋಗು_ಸ್ತಂಭ;
  ಅಕ್ಷರ_ಬರೆ ('\n');

  ಆದರೆ (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

ಸ್ಥಿರ ಶೂನ್ಯ
put_ಶಬ್ದ (ಶಬ್ದ *w)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s = w->ಪಠ್ಯ;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ n = w->ಉದ್ದಳತೆ; n != 0; n--)
    ಅಕ್ಷರ_ಬರೆ (*s++);
  ಹೊರಹೋಗು_ಸ್ತಂಭ += w->ಉದ್ದಳತೆ;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

ಸ್ಥಿರ ಶೂನ್ಯ
put_space (ಪೂರ್ಣಾಂಕ space)
{
  ಪೂರ್ಣಾಂಕ space_ಗುರಿ, tab_ಗುರಿ;

  space_ಗುರಿ = ಹೊರಹೋಗು_ಸ್ತಂಭ + space;
  ಆದರೆ (tabs)
    {
      tab_ಗುರಿ = space_ಗುರಿ / TABWIDTH * TABWIDTH;
      ಆದರೆ (ಹೊರಹೋಗು_ಸ್ತಂಭ + 1 < tab_ಗುರಿ)
        ಆಗ (ಹೊರಹೋಗು_ಸ್ತಂಭ < tab_ಗುರಿ)
          {
            ಅಕ್ಷರ_ಬರೆ ('\t');
            ಹೊರಹೋಗು_ಸ್ತಂಭ = (ಹೊರಹೋಗು_ಸ್ತಂಭ / TABWIDTH + 1) * TABWIDTH;
          }
    }
  ಆಗ (ಹೊರಹೋಗು_ಸ್ತಂಭ < space_ಗುರಿ)
    {
      ಅಕ್ಷರ_ಬರೆ (' ');
      ಹೊರಹೋಗು_ಸ್ತಂಭ++;
    }
}
