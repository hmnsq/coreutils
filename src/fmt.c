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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>

/* Redefine.  Otherwise, systems (Unicos for one) with headers that define
   it to be a type get syntax errors for the variable declaration below.  */
#परिभाषय word अप्रयुक्तम्_word_type

#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "system.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "fmt"

#परिभाषय AUTHORS proper_name ("Ross Paterson")

/* The following parameters represent the program's idea of what is
   "best".  Adjust to taste, subject to the caveats given.  */

/* Default longest permitted line length (max_width).  */
#परिभाषय विस्तारः_2	75

/* Prefer lines to be LEEWAY % shorter than the maximum width, giving
   room for optimization.  */
#परिभाषय LEEWAY	7

/* The default secondary indent of tagged paragraph used for unindented
   one-line paragraphs not preceded by any multi-line paragraphs.  */
#परिभाषय DEF_INDENT 3

/* Costs and bonuses are expressed as the equivalent departure from the
   optimal line length, multiplied by 10.  e.g. assigning something a
   cost of 50 means that it is as bad as a line 5 characters too short
   or too long.  The definition of SHORT_COST(n) should not be changed.
   However, EQUIV(n) may need tuning.  */

/* FIXME: "fmt" misbehaves given large inputs or options.  One
   possible workaround for part of the problem is to change COST to be
   a floating-point type.  There are other problems besides COST,
   though; see MAXWORDS below.  */

प्रकारनिर्देश दीर्घ पूर्णाङ्क COST;

#परिभाषय MAXCOST	TYPE_अधिकतमम् (COST)

#परिभाषय SQR(n)		((n) * (n))
#परिभाषय EQUIV(n)	SQR ((COST) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#परिभाषय SHORT_COST(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#परिभाषय RAGGED_COST(n)	(SHORT_COST (n) / 2)

/* Basic cost per line.  */
#परिभाषय पङ्क्तिः_COST	EQUIV (70)

/* Cost of breaking a line after the first word of a sentence, where
   the length of the word is N.  */
#परिभाषय WIDOW_COST(n)	(EQUIV (200) / ((n) + 2))

/* Cost of breaking a line before the last word of a sentence, where
   the length of the word is N.  */
#परिभाषय ORPHAN_COST(n)	(EQUIV (150) / ((n) + 2))

/* Bonus for breaking a line at the end of a sentence.  */
#परिभाषय SENTENCE_BONUS	EQUIV (50)

/* Cost of breaking a line after a period not marking end of a sentence.
   With the definition of sentence we are using (borrowed from emacs, see
   get_line()) such a break would then look like a sentence break.  Hence
   we assign a very high cost -- it should be avoided unless things are
   really bad.  */
#परिभाषय NOBREAK_COST	EQUIV (600)

/* Bonus for breaking a line before open parenthesis.  */
#परिभाषय PAREN_BONUS	EQUIV (40)

/* Bonus for breaking a line after other punctuation.  */
#परिभाषय PUNCT_BONUS	EQUIV(40)

/* Credit for breaking a long paragraph one line later.  */
#परिभाषय पङ्क्तिः_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#परिभाषय MAXWORDS	1000
#परिभाषय MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#परिभाषय isopen(c)	(strchr ("(['`\"", c) != रिक्त)
#परिभाषय isclose(c)	(strchr (")]'\"", c) != रिक्त)
#परिभाषय isperiod(c)	(strchr (".?!", c) != रिक्त)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#परिभाषय TABWIDTH	8

/* Word descriptor structure.  */

प्रकारनिर्देश संरचना Word WORD;

संरचना Word
  {

    /* Static attributes determined during input.  */

    वर्ण ध्रुव *text;		/* the text of the word */
    पूर्णाङ्क दीर्घता;			/* length of this word */
    पूर्णाङ्क space;			/* the size of the following space */
    अचिह्नित पूर्णाङ्क paren:1;	/* starts with open paren */
    अचिह्नित पूर्णाङ्क period:1;	/* ends in [.?!])* */
    अचिह्नित पूर्णाङ्क punct:1;	/* ends in punctuation */
    अचिह्नित पूर्णाङ्क final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    पूर्णाङ्क पङ्क्तिः_दीर्घता;		/* length of the best line starting here */
    COST best_cost;		/* cost of best paragraph starting here */
    WORD *अग्र_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

स्थिर शून्य समुच्चयः_उपसर्गः (वर्ण *p);
स्थिर bool fmt (FILE *f, वर्ण ध्रुव *);
स्थिर bool get_paragraph (FILE *f);
स्थिर पूर्णाङ्क get_पङ्क्तिः (FILE *f, पूर्णाङ्क c);
स्थिर पूर्णाङ्क get_उपसर्गः (FILE *f);
स्थिर पूर्णाङ्क get_space (FILE *f, पूर्णाङ्क c);
स्थिर पूर्णाङ्क copy_rest (FILE *f, पूर्णाङ्क c);
स्थिर bool same_para (पूर्णाङ्क c);
स्थिर शून्य flush_paragraph (शून्य);
स्थिर शून्य fmt_paragraph (शून्य);
स्थिर शून्य check_punctuation (WORD *w);
स्थिर COST आधारः_cost (WORD *this);
स्थिर COST पङ्क्तिः_cost (WORD *अग्र, पूर्णाङ्क len);
स्थिर शून्य put_paragraph (WORD *finish);
स्थिर शून्य put_पङ्क्तिः (WORD *w, पूर्णाङ्क indent);
स्थिर शून्य put_word (WORD *w);
स्थिर शून्य put_space (पूर्णाङ्क space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
स्थिर bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
स्थिर bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
स्थिर bool विभज;

/* If true, don't preserve inter-word spacing (default false).  */
स्थिर bool एकरूपम्;

/* Prefix minus leading and trailing spaces (default "").  */
स्थिर वर्ण ध्रुव *उपसर्गः = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
स्थिर पूर्णाङ्क max_विस्तारः = विस्तारः_2;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
स्थिर पूर्णाङ्क उपसर्गः_पूर्णम्_दीर्घता;

/* The length of the leading space trimmed from the prefix.  */
स्थिर पूर्णाङ्क उपसर्गः_lead_space;

/* The length of prefix minus leading and trailing space.  */
स्थिर पूर्णाङ्क उपसर्गः_दीर्घता;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
स्थिर पूर्णाङ्क goal_विस्तारः;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
स्थिर पूर्णाङ्क अन्तर्_column;

/* Start column of the next character to be written to stdout.  */
स्थिर पूर्णाङ्क बहिर्गमनम्_column;

/* Space for the paragraph text -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
स्थिर वर्ण parabuf[MAXCHARS];

/* A pointer into parabuf, indicating the first unused character position.  */
स्थिर वर्ण *wptr;

/* The words of a paragraph -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
स्थिर WORD word[MAXWORDS];

/* A pointer into the above word array, indicating the first position
   after the last complete word.  Sometimes it will point at an incomplete
   word.  */
स्थिर WORD *word_सीमा;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
स्थिर bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
स्थिर पूर्णाङ्क उपसर्गः_indent;

/* Indentation of the first line of the current paragraph.  */
स्थिर पूर्णाङ्क first_indent;

/* Indentation of other lines of the current paragraph */
स्थिर पूर्णाङ्क other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
स्थिर पूर्णाङ्क अग्र_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
स्थिर पूर्णाङ्क अग्र_उपसर्गः_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
स्थिर पूर्णाङ्क चरमः_पङ्क्तिः_दीर्घता;

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [-WIDTH] [OPTION]... [FILE]...\n"), क्रमादेशः_name);
      fputs (_("\
Reformat each paragraph अन्तर् the FILE(s), writing गन्तव्यम् standard output.\n\
The option -विस्तारः_2 is an abbreviated form सम्बन्धि --विस्तारः=DIGITS.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --crown-margin\n\
         preserve indentation सम्बन्धि first two पङ्क्तयः\n\
"));
      oputs (_("\
  -p, --उपसर्गः=STRING\n\
         reformat only पङ्क्तयः beginning with STRING,\n\
         reattaching the उपसर्गः गन्तव्यम् reformatted पङ्क्तयः\n\
"));
      oputs (_("\
  -s, --विभज-only\n\
         विभज दीर्घ पङ्क्तयः, but कुरु not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation सम्बन्धि first पङ्क्तिः different from second\n\
"));
      oputs (_("\
  -u, --एकरूपम्-spacing\n\
         one space between शब्दाः, two after sentences\n\
"));
      oputs (_("\
  -w, --विस्तारः=विस्तारः_2\n\
         अधिकतमम् पङ्क्तिः विस्तारः (स्वभाव सम्बन्धि 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=विस्तारः_2\n\
         goal विस्तारः (स्वभाव सम्बन्धि 93% सम्बन्धि विस्तारः)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Decode options and launch execution.  */

स्थिर संरचना option ध्रुव long_options[] =
{
  {"crown-margin", no_तर्कः, रिक्त, 'c'},
  {"prefix", required_तर्कः, रिक्त, 'p'},
  {"split-only", no_तर्कः, रिक्त, 's'},
  {"tagged-paragraph", no_तर्कः, रिक्त, 't'},
  {"uniform-spacing", no_तर्कः, रिक्त, 'u'},
  {"width", required_तर्कः, रिक्त, 'w'},
  {"goal", required_तर्कः, रिक्त, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0},
};

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optchar;
  bool ok = true;
  वर्ण ध्रुव *max_विस्तारः_option = रिक्त;
  वर्ण ध्रुव *goal_विस्तारः_option = रिक्त;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यदि (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_विस्तारः_option = argv[1] + 1;

      /* Make the option we just parsed invisible to getopt.  */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  यावत् ((optchar = getopt_long (argc, argv, "0123456789cstuw:p:g:",
                                 long_options, रिक्त))
         != -1)
    विकल्प (optchar)
      {
      स्वभाव:
        यदि (c_isdigit (optchar))
          error (0, 0, _("invalid option -- %c; -विस्तारः_2 is recognized\
 only when it is the first\noption; use -w N instead"),
                 optchar);
        usage (EXIT_FAILURE);

      पक्ष 'c':
        crown = true;
        भङ्ग;

      पक्ष 's':
        विभज = true;
        भङ्ग;

      पक्ष 't':
        tagged = true;
        भङ्ग;

      पक्ष 'u':
        एकरूपम् = true;
        भङ्ग;

      पक्ष 'w':
        max_विस्तारः_option = optarg;
        भङ्ग;

      पक्ष 'g':
        goal_विस्तारः_option = optarg;
        भङ्ग;

      पक्ष 'p':
        समुच्चयः_उपसर्गः (optarg);
        भङ्ग;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

      }

  यदि (max_विस्तारः_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_विस्तारः = xnumtoumax (max_विस्तारः_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  यदि (goal_विस्तारः_option)
    {
      /* Limit goal_width to max_width.  */
      goal_विस्तारः = xdectoumax (goal_विस्तारः_option, 0, max_विस्तारः, "",
                               _("invalid goal"), 0);
      यदि (max_विस्तारः_option == रिक्त)
        max_विस्तारः = goal_विस्तारः + 10;
    }
  अन्यथा
    {
      goal_विस्तारः = max_विस्तारः * (2 * (100 - LEEWAY) + 1) / 200;
    }

  bool have_read_stdin = false;

  यदि (optind == argc)
    {
      have_read_stdin = true;
      ok = fmt (stdin, "-");
    }
  अन्यथा
    {
      प्रति (; optind < argc; optind++)
        {
          वर्ण *file = argv[optind];
          यदि (streq (file, "-"))
            {
              ok &= fmt (stdin, file);
              have_read_stdin = true;
            }
          अन्यथा
            {
              FILE *अन्तर्_प्रवाहः;
              अन्तर्_प्रवाहः = fopen (file, "r");
              यदि (अन्तर्_प्रवाहः != रिक्त)
                ok &= fmt (अन्तर्_प्रवाहः, file);
              अन्यथा
                {
                  error (0, errno, _("cannot open %s for reading"),
                         quoteaf (file));
                  ok = false;
                }
            }
        }
    }

  यदि (have_read_stdin && fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s", _("closing standard input"));

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Trim space from the front and back of the string P, yielding the prefix,
   and record the lengths of the prefix and the space trimmed.  */

स्थिर शून्य
समुच्चयः_उपसर्गः (वर्ण *p)
{
  वर्ण *s;

  उपसर्गः_lead_space = 0;
  यावत् (*p == ' ')
    {
      उपसर्गः_lead_space++;
      p++;
    }
  उपसर्गः = p;
  उपसर्गः_पूर्णम्_दीर्घता = वाक्यदैर्घ्यम् (p);
  s = p + उपसर्गः_पूर्णम्_दीर्घता;
  यावत् (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  उपसर्गः_दीर्घता = s - p;
}

/* Read F and send formatted output to stdout.
   Close F when done, unless F is stdin.  Diagnose input errors, using FILE.
   If !F, assume F resulted from an fopen failure and diagnose that.
   Return true if successful.  */

स्थिर bool
fmt (FILE *f, वर्ण ध्रुव *file)
{
  fadvise (f, FADVISE_SEQUENTIAL);
  tabs = false;
  other_indent = 0;
  अग्र_char = get_उपसर्गः (f);
  यावत् (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (word_सीमा);
    }

  पूर्णाङ्क err = ferror (f) ? 0 : -1;
  /* err < 0 means success.  */
  यदि (f == stdin)
    clearerr (f);
  अन्यथा यदि (fclose (f) != 0 && err < 0)
    err = errno;
  यदि (0 <= err)
    {
      यदि (f == stdin)
        error (0, errno, _("read error"));
      अन्यथा
        error (0, errno, _("error reading %s"), quoteaf (file));
    }
  प्रत्यागम err < 0;
}

/* Set the global variable 'other_indent' according to SAME_PARAGRAPH
   and other global variables.  */

स्थिर शून्य
समुच्चयः_other_indent (bool same_paragraph)
{
  यदि (विभज)
    other_indent = first_indent;
  अन्यथा यदि (crown)
    {
      other_indent = (same_paragraph ? अन्तर्_column : first_indent);
    }
  अन्यथा यदि (tagged)
    {
      यदि (same_paragraph && अन्तर्_column != first_indent)
        {
          other_indent = अन्तर्_column;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      अन्यथा यदि (other_indent == first_indent)
        other_indent = first_indent == 0 ? DEF_INDENT : 0;
    }
  अन्यथा
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

स्थिर bool
get_paragraph (FILE *f)
{
  पूर्णाङ्क c;

  चरमः_पङ्क्तिः_दीर्घता = 0;
  c = अग्र_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  यावत् (c == '\n' || c == EOF
         || अग्र_उपसर्गः_indent < उपसर्गः_lead_space
         || अन्तर्_column < अग्र_उपसर्गः_indent + उपसर्गः_पूर्णम्_दीर्घता)
    {
      c = copy_rest (f, c);
      यदि (c == EOF)
        {
          अग्र_char = EOF;
          प्रत्यागम false;
        }
      अक्षरं_लिख ('\n');
      c = get_उपसर्गः (f);
    }

  /* Got a suitable first line for a paragraph.  */

  उपसर्गः_indent = अग्र_उपसर्गः_indent;
  first_indent = अन्तर्_column;
  wptr = parabuf;
  word_सीमा = word;
  c = get_पङ्क्तिः (f, c);
  समुच्चयः_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  यदि (विभज)
    {
      /*रिक्तानि*/
    }
  अन्यथा यदि (crown)
    {
      यदि (same_para (c))
        {
          कुरु
            {			/* for each line till the end of the para */
              c = get_पङ्क्तिः (f, c);
            }
          यावत् (same_para (c) && अन्तर्_column == other_indent);
        }
    }
  अन्यथा यदि (tagged)
    {
      यदि (same_para (c) && अन्तर्_column != first_indent)
        {
          कुरु
            {			/* for each line till the end of the para */
              c = get_पङ्क्तिः (f, c);
            }
          यावत् (same_para (c) && अन्तर्_column == other_indent);
        }
    }
  अन्यथा
    {
      यावत् (same_para (c) && अन्तर्_column == other_indent)
        c = get_पङ्क्तिः (f, c);
    }

  (word_सीमा - 1)->period = (word_सीमा - 1)->final = true;
  अग्र_char = c;
  प्रत्यागम true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

स्थिर पूर्णाङ्क
copy_rest (FILE *f, पूर्णाङ्क c)
{
  बहिर्गमनम्_column = 0;
  यदि (अन्तर्_column > अग्र_उपसर्गः_indent || (c != '\n' && c != EOF))
    {
      put_space (अग्र_उपसर्गः_indent);
      प्रति (वर्ण ध्रुव *s = उपसर्गः; बहिर्गमनम्_column != अन्तर्_column && *s; बहिर्गमनम्_column++)
        अक्षरं_लिख (*s++);
      यदि (c != EOF && c != '\n')
        put_space (अन्तर्_column - बहिर्गमनम्_column);
      यदि (c == EOF && अन्तर्_column >= अग्र_उपसर्गः_indent + उपसर्गः_दीर्घता)
        अक्षरं_लिख ('\n');
    }
  यावत् (c != '\n' && c != EOF)
    {
      अक्षरं_लिख (c);
      c = getc (f);
    }
  प्रत्यागम c;
}

/* Return true if a line whose first non-blank character after the
   prefix (if any) is C could belong to the current paragraph,
   otherwise false.  */

स्थिर bool
same_para (पूर्णाङ्क c)
{
  प्रत्यागम (अग्र_उपसर्गः_indent == उपसर्गः_indent
          && अन्तर्_column >= अग्र_उपसर्गः_indent + उपसर्गः_पूर्णम्_दीर्घता
          && c != '\n' && c != EOF);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

स्थिर पूर्णाङ्क
get_पङ्क्तिः (FILE *f, पूर्णाङ्क c)
{
  पूर्णाङ्क start;
  वर्ण *अन्तः_सम्बन्धि_parabuf;
  WORD *अन्तः_सम्बन्धि_word;

  अन्तः_सम्बन्धि_parabuf = &parabuf[MAXCHARS];
  अन्तः_सम्बन्धि_word = &word[MAXWORDS - 2];

  कुरु
    {				/* for each word in a line */

      /* Scan word.  */

      word_सीमा->text = wptr;
      कुरु
        {
          यदि (wptr == अन्तः_सम्बन्धि_parabuf)
            {
              समुच्चयः_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = getc (f);
        }
      यावत् (c != EOF && !c_isspace (c));
      अन्तर्_column += word_सीमा->दीर्घता = wptr - word_सीमा->text;
      check_punctuation (word_सीमा);

      /* Scan inter-word space.  */

      start = अन्तर्_column;
      c = get_space (f, c);
      word_सीमा->space = अन्तर्_column - start;
      word_सीमा->final = (c == EOF
                           || (word_सीमा->period
                               && (c == '\n' || word_सीमा->space > 1)));
      यदि (c == '\n' || c == EOF || एकरूपम्)
        word_सीमा->space = word_सीमा->final ? 2 : 1;
      यदि (word_सीमा == अन्तः_सम्बन्धि_word)
        {
          समुच्चयः_other_indent (true);
          flush_paragraph ();
        }
      word_सीमा++;
    }
  यावत् (c != '\n' && c != EOF);
  प्रत्यागम get_उपसर्गः (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

स्थिर पूर्णाङ्क
get_उपसर्गः (FILE *f)
{
  पूर्णाङ्क c;

  अन्तर्_column = 0;
  c = get_space (f, getc (f));
  यदि (उपसर्गः_दीर्घता == 0)
    अग्र_उपसर्गः_indent = उपसर्गः_lead_space < अन्तर्_column ?
      उपसर्गः_lead_space : अन्तर्_column;
  अन्यथा
    {
      अग्र_उपसर्गः_indent = अन्तर्_column;
      प्रति (वर्ण ध्रुव *p = उपसर्गः; *p != '\0'; p++)
        {
          अचिह्नित वर्ण pc = *p;
          यदि (c != pc)
            प्रत्यागम c;
          अन्तर्_column++;
          c = getc (f);
        }
      c = get_space (f, c);
    }
  प्रत्यागम c;
}

/* Read blank characters from input file F, starting with C, and keeping
   in_column up-to-date.  Return first non-blank character.  */

स्थिर पूर्णाङ्क
get_space (FILE *f, पूर्णाङ्क c)
{
  यावत् (true)
    {
      यदि (c == ' ')
        अन्तर्_column++;
      अन्यथा यदि (c == '\t')
        {
          tabs = true;
          अन्तर्_column = (अन्तर्_column / TABWIDTH + 1) * TABWIDTH;
        }
      अन्यथा
        प्रत्यागम c;
      c = getc (f);
    }
}

/* Set extra fields in word W describing any attached punctuation.  */

स्थिर शून्य
check_punctuation (WORD *w)
{
  वर्ण ध्रुव *start = w->text;
  वर्ण ध्रुव *finish = start + (w->दीर्घता - 1);
  अचिह्नित वर्ण fin = *finish;

  w->paren = isopen (*start);
  w->punct = !! ispunct (fin);
  यावत् (start < finish && isclose (*finish))
    finish--;
  w->period = isperiod (*finish);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

स्थिर शून्य
flush_paragraph (शून्य)
{
  WORD *विभज_point;
  WORD *w;
  पूर्णाङ्क सरणम्;
  COST best_break;

  /* In the special case where it's all one word, just flush it.  */

  यदि (word_सीमा == word)
    {
      size_t गन्तव्यम्_write = wptr - parabuf;
      यदि (fwrite (parabuf, 1, गन्तव्यम्_write, stdout) != गन्तव्यम्_write)
        write_error ();

      wptr = parabuf;
      प्रत्यागम;
    }

  /* Otherwise:
     - format what you have so far as a paragraph,
     - find a low-cost line break near the end,
     - output to there,
     - make that the start of the paragraph.  */

  fmt_paragraph ();

  /* Choose a good split point.  */

  विभज_point = word_सीमा;
  best_break = MAXCOST;
  प्रति (w = word->अग्र_break; w != word_सीमा; w = w->अग्र_break)
    {
      यदि (w->best_cost - w->अग्र_break->best_cost < best_break)
        {
          विभज_point = w;
          best_break = w->best_cost - w->अग्र_break->best_cost;
        }
      यदि (best_break <= MAXCOST - पङ्क्तिः_CREDIT)
        best_break += पङ्क्तिः_CREDIT;
    }
  put_paragraph (विभज_point);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  memmove (parabuf, विभज_point->text, wptr - विभज_point->text);
  सरणम् = विभज_point->text - parabuf;
  wptr -= सरणम्;

  /* Adjust text pointers.  */

  प्रति (w = विभज_point; w <= word_सीमा; w++)
    w->text -= सरणम्;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  memmove (word, विभज_point, (word_सीमा - विभज_point + 1) * परिमाण *word);
  word_सीमा -= विभज_point - word;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

स्थिर शून्य
fmt_paragraph (शून्य)
{
  WORD *w;
  पूर्णाङ्क len;
  COST wcost, best;
  पूर्णाङ्क saved_दीर्घता;

  word_सीमा->best_cost = 0;
  saved_दीर्घता = word_सीमा->दीर्घता;
  word_सीमा->दीर्घता = max_विस्तारः;	/* sentinel */

  प्रति (WORD *start = word_सीमा - 1; start >= word; start--)
    {
      best = MAXCOST;
      len = start == word ? first_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = start;
      len += w->दीर्घता;
      कुरु
        {
          w++;

          /* Consider breaking before w.  */

          wcost = पङ्क्तिः_cost (w, len) + w->best_cost;
          यदि (start == word && चरमः_पङ्क्तिः_दीर्घता > 0)
            wcost += RAGGED_COST (len - चरमः_पङ्क्तिः_दीर्घता);
          यदि (wcost < best)
            {
              best = wcost;
              start->अग्र_break = w;
              start->पङ्क्तिः_दीर्घता = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          यदि (w == word_सीमा)
            भङ्ग;

          len += (w - 1)->space + w->दीर्घता;	/* w > start >= word */
        }
      यावत् (len <= max_विस्तारः);
      start->best_cost = best + आधारः_cost (start);
    }

  word_सीमा->दीर्घता = saved_दीर्घता;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#यदि __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#यदि_अन्तः

/* Return the constant component of the cost of breaking before the
   word THIS.  */

स्थिर COST
आधारः_cost (WORD *this)
{
  COST cost;

  cost = पङ्क्तिः_COST;

  यदि (this > word)
    {
      यदि ((this - 1)->period)
        {
          यदि ((this - 1)->final)
            cost -= SENTENCE_BONUS;
          अन्यथा
            cost += NOBREAK_COST;
        }
      अन्यथा यदि ((this - 1)->punct)
        cost -= PUNCT_BONUS;
      अन्यथा यदि (this > word + 1 && (this - 2)->final)
        cost += WIDOW_COST ((this - 1)->दीर्घता);
    }

  यदि (this->paren)
    cost -= PAREN_BONUS;
  अन्यथा यदि (this->final)
    cost += ORPHAN_COST (this->दीर्घता);

  प्रत्यागम cost;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

स्थिर COST
पङ्क्तिः_cost (WORD *अग्र, पूर्णाङ्क len)
{
  पूर्णाङ्क n;
  COST cost;

  यदि (अग्र == word_सीमा)
    प्रत्यागम 0;
  n = goal_विस्तारः - len;
  cost = SHORT_COST (n);
  यदि (अग्र->अग्र_break != word_सीमा)
    {
      n = len - अग्र->पङ्क्तिः_दीर्घता;
      cost += RAGGED_COST (n);
    }
  प्रत्यागम cost;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

स्थिर शून्य
put_paragraph (WORD *finish)
{
  put_पङ्क्तिः (word, first_indent);
  प्रति (WORD *w = word->अग्र_break; w != finish; w = w->अग्र_break)
    put_पङ्क्तिः (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

स्थिर शून्य
put_पङ्क्तिः (WORD *w, पूर्णाङ्क indent)
{
  WORD *endline;

  बहिर्गमनम्_column = 0;
  put_space (उपसर्गः_indent);
  fputs (उपसर्गः, stdout);
  बहिर्गमनम्_column += उपसर्गः_दीर्घता;
  put_space (indent - बहिर्गमनम्_column);

  endline = w->अग्र_break - 1;
  प्रति (; w != endline; w++)
    {
      put_word (w);
      put_space (w->space);
    }
  put_word (w);
  चरमः_पङ्क्तिः_दीर्घता = बहिर्गमनम्_column;
  अक्षरं_लिख ('\n');

  यदि (ferror (stdout))
    write_error ();
}

/* Output to stdout the word W.  */

स्थिर शून्य
put_word (WORD *w)
{
  वर्ण ध्रुव *s = w->text;
  प्रति (पूर्णाङ्क n = w->दीर्घता; n != 0; n--)
    अक्षरं_लिख (*s++);
  बहिर्गमनम्_column += w->दीर्घता;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

स्थिर शून्य
put_space (पूर्णाङ्क space)
{
  पूर्णाङ्क space_target, tab_target;

  space_target = बहिर्गमनम्_column + space;
  यदि (tabs)
    {
      tab_target = space_target / TABWIDTH * TABWIDTH;
      यदि (बहिर्गमनम्_column + 1 < tab_target)
        यावत् (बहिर्गमनम्_column < tab_target)
          {
            अक्षरं_लिख ('\t');
            बहिर्गमनम्_column = (बहिर्गमनम्_column / TABWIDTH + 1) * TABWIDTH;
          }
    }
  यावत् (बहिर्गमनम्_column < space_target)
    {
      अक्षरं_लिख (' ');
      बहिर्गमनम्_column++;
    }
}
