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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <getopt.h>

/* Redefine.  Otherwise, systems (Unicos for one) with headers that define
   it to be a type get syntax errors for the variable declaration below.  */
#घोषणा word unused_word_type

#समावेश "c-ctype.h"
#समावेश "system.h"
#समावेश "fadvise.h"
#समावेश "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "fmt"

#घोषणा AUTHORS proper_name ("Ross Paterson")

/* The following parameters represent the program's idea of what is
   "best".  Adjust to taste, subject to the caveats given.  */

/* Default longest permitted line length (max_width).  */
#घोषणा WIDTH	75

/* Prefer lines to be LEEWAY % shorter than the maximum width, giving
   room for optimization.  */
#घोषणा LEEWAY	7

/* The default secondary indent of tagged paragraph used for unindented
   one-line paragraphs not preceded by any multi-line paragraphs.  */
#घोषणा DEF_INDENT 3

/* Costs and bonuses are expressed as the equivalent departure from the
   optimal line length, multiplied by 10.  e.g. assigning something a
   cost of 50 means that it is as bad as a line 5 characters too short
   or too long.  The definition of SHORT_COST(n) should not be changed.
   However, EQUIV(n) may need tuning.  */

/* FIXME: "fmt" misbehaves given large inputs or options.  One
   possible workaround for part of the problem is to change COST to be
   a floating-point type.  There are other problems besides COST,
   though; see MAXWORDS below.  */

प्रकार दीर्घ पूर्णांक COST;

#घोषणा MAXCOST	TYPE_MAXIMUM (COST)

#घोषणा SQR(n)		((n) * (n))
#घोषणा EQUIV(n)	SQR ((COST) (n))

/* Cost of a filled line n chars longer or shorter than goal_width.  */
#घोषणा SHORT_COST(n)	EQUIV ((n) * 10)

/* Cost of the difference between adjacent filled lines.  */
#घोषणा RAGGED_COST(n)	(SHORT_COST (n) / 2)

/* Basic cost per line.  */
#घोषणा पंक्ति_COST	EQUIV (70)

/* Cost of breaking a line after the first word of a sentence, where
   the length of the word is N.  */
#घोषणा WIDOW_COST(n)	(EQUIV (200) / ((n) + 2))

/* Cost of breaking a line before the last word of a sentence, where
   the length of the word is N.  */
#घोषणा ORPHAN_COST(n)	(EQUIV (150) / ((n) + 2))

/* Bonus for breaking a line at the end of a sentence.  */
#घोषणा SENTENCE_BONUS	EQUIV (50)

/* Cost of breaking a line after a period not marking end of a sentence.
   With the definition of sentence we are using (borrowed from emacs, see
   get_line()) such a break would then look like a sentence break.  Hence
   we assign a very high cost -- it should be avoided unless things are
   really bad.  */
#घोषणा NOBREAK_COST	EQUIV (600)

/* Bonus for breaking a line before open parenthesis.  */
#घोषणा PAREN_BONUS	EQUIV (40)

/* Bonus for breaking a line after other punctuation.  */
#घोषणा PUNCT_BONUS	EQUIV(40)

/* Credit for breaking a long paragraph one line later.  */
#घोषणा पंक्ति_CREDIT	EQUIV(3)

/* Size of paragraph buffer, in words and characters.  Longer paragraphs
   are handled neatly (cf. flush_paragraph()), so long as these values
   are considerably greater than required by the width.  These values
   cannot be extended indefinitely: doing so would run into size limits
   and/or cause more overflows in cost calculations.  FIXME: Remove these
   arbitrary limits.  */

#घोषणा MAXWORDS	1000
#घोषणा MAXCHARS	5000

/* Extra ctype(3)-style macros.  */

#घोषणा isopen(c)	(म_अक्षर ("(['`\"", c) != शून्य)
#घोषणा isclose(c)	(म_अक्षर (")]'\"", c) != शून्य)
#घोषणा isperiod(c)	(म_अक्षर (".?!", c) != शून्य)

/* Size of a tab stop, for expansion on input and re-introduction on
   output.  */
#घोषणा TABWIDTH	8

/* Word descriptor structure.  */

प्रकार काष्ठा Word WORD;

काष्ठा Word
  {

    /* Static attributes determined during input.  */

    अक्षर स्थिर *text;		/* the text of the word */
    पूर्णांक लंबाई;			/* length of this word */
    पूर्णांक space;			/* the size of the following space */
    अचिन्हित पूर्णांक paren:1;	/* starts with open paren */
    अचिन्हित पूर्णांक period:1;	/* ends in [.?!])* */
    अचिन्हित पूर्णांक punct:1;	/* ends in punctuation */
    अचिन्हित पूर्णांक final:1;	/* end of sentence */

    /* The remaining fields are computed during the optimization.  */

    पूर्णांक पंक्ति_लंबाई;		/* length of the best line starting here */
    COST best_cost;		/* cost of best paragraph starting here */
    WORD *अग्र_break;		/* break which achieves best_cost */
  };

/* Forward declarations.  */

अटल व्योम set_prefix (अक्षर *p);
अटल bool fmt (खाता *f, अक्षर स्थिर *);
अटल bool get_paragraph (खाता *f);
अटल पूर्णांक get_पंक्ति (खाता *f, पूर्णांक c);
अटल पूर्णांक get_prefix (खाता *f);
अटल पूर्णांक get_space (खाता *f, पूर्णांक c);
अटल पूर्णांक copy_rest (खाता *f, पूर्णांक c);
अटल bool same_para (पूर्णांक c);
अटल व्योम flush_paragraph (व्योम);
अटल व्योम fmt_paragraph (व्योम);
अटल व्योम check_punctuation (WORD *w);
अटल COST base_cost (WORD *this);
अटल COST पंक्ति_cost (WORD *अग्र, पूर्णांक len);
अटल व्योम put_paragraph (WORD *finish);
अटल व्योम put_पंक्ति (WORD *w, पूर्णांक indent);
अटल व्योम put_word (WORD *w);
अटल व्योम put_space (पूर्णांक space);

/* Option values.  */

/* If true, first 2 lines may have different indent (default false).  */
अटल bool crown;

/* If true, first 2 lines _must_ have different indent (default false).  */
अटल bool tagged;

/* If true, each line is a paragraph on its own (default false).  */
अटल bool split;

/* If true, don't preserve inter-word spacing (default false).  */
अटल bool uniform;

/* Prefix minus leading and trailing spaces (default "").  */
अटल अक्षर स्थिर *prefix = "";

/* User-supplied maximum line width (default WIDTH).  The only output
   lines longer than this will each comprise a single word.  */
अटल पूर्णांक max_width = WIDTH;

/* Values derived from the option values.  */

/* The length of prefix minus leading space.  */
अटल पूर्णांक prefix_full_लंबाई;

/* The length of the leading space trimmed from the prefix.  */
अटल पूर्णांक prefix_lead_space;

/* The length of prefix minus leading and trailing space.  */
अटल पूर्णांक prefix_लंबाई;

/* The preferred width of text lines, set to LEEWAY % less than max_width.  */
अटल पूर्णांक goal_width;

/* Dynamic variables.  */

/* Start column of the character most recently read from the input file.  */
अटल पूर्णांक in_column;

/* Start column of the next character to be written to stdout.  */
अटल पूर्णांक out_column;

/* Space for the paragraph text -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
अटल अक्षर parabuf[MAXCHARS];

/* A pointer into parabuf, indicating the first unused character position.  */
अटल अक्षर *wptr;

/* The words of a paragraph -- longer paragraphs are handled neatly
   (cf. flush_paragraph()).  */
अटल WORD word[MAXWORDS];

/* A pointer into the above word array, indicating the first position
   after the last complete word.  Sometimes it will point at an incomplete
   word.  */
अटल WORD *word_limit;

/* If true, current input file contains tab characters, and so tabs can be
   used for white space on output.  */
अटल bool tabs;

/* Space before trimmed prefix on each line of the current paragraph.  */
अटल पूर्णांक prefix_indent;

/* Indentation of the first line of the current paragraph.  */
अटल पूर्णांक first_indent;

/* Indentation of other lines of the current paragraph */
अटल पूर्णांक other_indent;

/* To detect the end of a paragraph, we need to look ahead to the first
   non-blank character after the prefix on the next line, or the first
   character on the following line that failed to match the prefix.
   We can reconstruct the lookahead from that character (next_char), its
   position on the line (in_column) and the amount of space before the
   prefix (next_prefix_indent).  See get_paragraph() and copy_rest().  */

/* The last character read from the input file.  */
अटल पूर्णांक अग्र_char;

/* The space before the trimmed prefix (or part of it) on the next line
   after the current paragraph.  */
अटल पूर्णांक अग्र_prefix_indent;

/* If nonzero, the length of the last line output in the current
   paragraph, used to charge for raggedness at the split point for long
   paragraphs chosen by fmt_paragraph().  */
अटल पूर्णांक last_पंक्ति_लंबाई;

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [-WIDTH] [OPTION]... [FILE]...\n"), program_name);
      ख_माला_दो (_("\
Reformat each paragraph in the खाता(s), writing to standard output.\n\
The option -WIDTH is an abbreviated form of --width=DIGITS.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --crown-margin\n\
         preserve indentation of first two पंक्तियाँ\n\
"));
      oputs (_("\
  -p, --prefix=STRING\n\
         reformat only पंक्तियाँ beginning with STRING,\n\
         reattaching the prefix to reformatted पंक्तियाँ\n\
"));
      oputs (_("\
  -s, --split-only\n\
         split दीर्घ पंक्तियाँ, but करो not refill\n\
"));
      oputs (_("\
  -t, --tagged-paragraph\n\
         indentation of first पंक्ति different from second\n\
"));
      oputs (_("\
  -u, --uniform-spacing\n\
         one space between शब्दगण, two after sentences\n\
"));
      oputs (_("\
  -w, --width=WIDTH\n\
         maximum पंक्ति width (शेष of 75 columns)\n\
"));
      /* Tell xgettext that the "% o" below is not a printf-style
         format string:  xgettext:no-c-format */
      oputs (_("\
  -g, --goal=WIDTH\n\
         goal width (शेष of 93% of width)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Decode options and launch execution.  */

अटल काष्ठा option स्थिर long_options[] =
{
  {"crown-margin", no_argument, शून्य, 'c'},
  {"prefix", required_argument, शून्य, 'p'},
  {"split-only", no_argument, शून्य, 's'},
  {"tagged-paragraph", no_argument, शून्य, 't'},
  {"uniform-spacing", no_argument, शून्य, 'u'},
  {"width", required_argument, शून्य, 'w'},
  {"goal", required_argument, शून्य, 'g'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0},
};

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optchar;
  bool ok = true;
  अक्षर स्थिर *max_width_option = शून्य;
  अक्षर स्थिर *goal_width_option = शून्य;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  अगर (argc > 1 && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      /* Old option syntax; a dash followed by one or more digits.  */
      max_width_option = argv[1] + 1;

      /* Make the option we just parsed invisible to getopt.  */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  जबतक ((optchar = getopt_long (argc, argv, "0123456789cstuw:p:g:",
                                 long_options, शून्य))
         != -1)
    चयन (optchar)
      {
      शेष:
        अगर (c_isdigit (optchar))
          error (0, 0, _("invalid option -- %c; -WIDTH is recognized\
 only when it is the first\noption; use -w N instead"),
                 optchar);
        usage (निकास_त्रुटि);

      हाल 'c':
        crown = true;
        अवरोध;

      हाल 's':
        split = true;
        अवरोध;

      हाल 't':
        tagged = true;
        अवरोध;

      हाल 'u':
        uniform = true;
        अवरोध;

      हाल 'w':
        max_width_option = optarg;
        अवरोध;

      हाल 'g':
        goal_width_option = optarg;
        अवरोध;

      हाल 'p':
        set_prefix (optarg);
        अवरोध;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      }

  अगर (max_width_option)
    {
      /* Limit max_width to MAXCHARS / 2; otherwise, the resulting
         output can be quite ugly.  */
      max_width = xnumtoumax (max_width_option, 10, 0, MAXCHARS / 2, "",
                              _("invalid width"), 0, XTOINT_MAX_RANGE);
    }

  अगर (goal_width_option)
    {
      /* Limit goal_width to max_width.  */
      goal_width = xdectoumax (goal_width_option, 0, max_width, "",
                               _("invalid goal"), 0);
      अगर (max_width_option == शून्य)
        max_width = goal_width + 10;
    }
  अन्यथा
    {
      goal_width = max_width * (2 * (100 - LEEWAY) + 1) / 200;
    }

  bool have_read_stdin = false;

  अगर (optind == argc)
    {
      have_read_stdin = true;
      ok = fmt (मानक_निवेश, "-");
    }
  अन्यथा
    {
      क्रम (; optind < argc; optind++)
        {
          अक्षर *file = argv[optind];
          अगर (streq (file, "-"))
            {
              ok &= fmt (मानक_निवेश, file);
              have_read_stdin = true;
            }
          अन्यथा
            {
              खाता *in_stream;
              in_stream = ख_खोलो (file, "r");
              अगर (in_stream != शून्य)
                ok &= fmt (in_stream, file);
              अन्यथा
                {
                  error (0, त्रुटि_सं, _("cannot open %s for reading"),
                         quoteaf (file));
                  ok = false;
                }
            }
        }
    }

  अगर (have_read_stdin && ख_बंद (मानक_निवेश) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", _("closing standard input"));

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}

/* Trim space from the front and back of the string P, yielding the prefix,
   and record the lengths of the prefix and the space trimmed.  */

अटल व्योम
set_prefix (अक्षर *p)
{
  अक्षर *s;

  prefix_lead_space = 0;
  जबतक (*p == ' ')
    {
      prefix_lead_space++;
      p++;
    }
  prefix = p;
  prefix_full_लंबाई = म_माप (p);
  s = p + prefix_full_लंबाई;
  जबतक (s > p && s[-1] == ' ')
    s--;
  *s = '\0';
  prefix_लंबाई = s - p;
}

/* Read F and send formatted output to stdout.
   Close F when done, unless F is stdin.  Diagnose input errors, using FILE.
   If !F, assume F resulted from an fopen failure and diagnose that.
   Return true if successful.  */

अटल bool
fmt (खाता *f, अक्षर स्थिर *file)
{
  fadvise (f, FADVISE_SEQUENTIAL);
  tabs = false;
  other_indent = 0;
  अग्र_char = get_prefix (f);
  जबतक (get_paragraph (f))
    {
      fmt_paragraph ();
      put_paragraph (word_limit);
    }

  पूर्णांक err = ख_त्रुटि (f) ? 0 : -1;
  /* err < 0 means success.  */
  अगर (f == मानक_निवेश)
    साफ_त्रुटि (f);
  अन्यथा अगर (ख_बंद (f) != 0 && err < 0)
    err = त्रुटि_सं;
  अगर (0 <= err)
    {
      अगर (f == मानक_निवेश)
        error (0, त्रुटि_सं, _("read error"));
      अन्यथा
        error (0, त्रुटि_सं, _("error reading %s"), quoteaf (file));
    }
  वापस err < 0;
}

/* Set the global variable 'other_indent' according to SAME_PARAGRAPH
   and other global variables.  */

अटल व्योम
set_other_indent (bool same_paragraph)
{
  अगर (split)
    other_indent = first_indent;
  अन्यथा अगर (crown)
    {
      other_indent = (same_paragraph ? in_column : first_indent);
    }
  अन्यथा अगर (tagged)
    {
      अगर (same_paragraph && in_column != first_indent)
        {
          other_indent = in_column;
        }

      /* Only one line: use the secondary indent from last time if it
         splits, or 0 if there have been no multi-line paragraphs in the
         input so far.  But if these rules make the two indents the same,
         pick a new secondary indent.  */

      अन्यथा अगर (other_indent == first_indent)
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

अटल bool
get_paragraph (खाता *f)
{
  पूर्णांक c;

  last_पंक्ति_लंबाई = 0;
  c = अग्र_char;

  /* Scan (and copy) blank lines, and lines not introduced by the prefix.  */

  जबतक (c == '\n' || c == खातापूर्ण
         || अग्र_prefix_indent < prefix_lead_space
         || in_column < अग्र_prefix_indent + prefix_full_लंबाई)
    {
      c = copy_rest (f, c);
      अगर (c == खातापूर्ण)
        {
          अग्र_char = खातापूर्ण;
          वापस false;
        }
      अक्षर_दो ('\n');
      c = get_prefix (f);
    }

  /* Got a suitable first line for a paragraph.  */

  prefix_indent = अग्र_prefix_indent;
  first_indent = in_column;
  wptr = parabuf;
  word_limit = word;
  c = get_पंक्ति (f, c);
  set_other_indent (same_para (c));

  /* Read rest of paragraph (unless split is specified).  */

  अगर (split)
    {
      /* empty */
    }
  अन्यथा अगर (crown)
    {
      अगर (same_para (c))
        {
          करो
            {			/* for each line till the end of the para */
              c = get_पंक्ति (f, c);
            }
          जबतक (same_para (c) && in_column == other_indent);
        }
    }
  अन्यथा अगर (tagged)
    {
      अगर (same_para (c) && in_column != first_indent)
        {
          करो
            {			/* for each line till the end of the para */
              c = get_पंक्ति (f, c);
            }
          जबतक (same_para (c) && in_column == other_indent);
        }
    }
  अन्यथा
    {
      जबतक (same_para (c) && in_column == other_indent)
        c = get_पंक्ति (f, c);
    }

  (word_limit - 1)->period = (word_limit - 1)->final = true;
  अग्र_char = c;
  वापस true;
}

/* Copy to the output a line that failed to match the prefix, or that
   was blank after the prefix.  In the former case, C is the character
   that failed to match the prefix.  In the latter, C is \n or EOF.
   Return the character (\n or EOF) ending the line.  */

अटल पूर्णांक
copy_rest (खाता *f, पूर्णांक c)
{
  out_column = 0;
  अगर (in_column > अग्र_prefix_indent || (c != '\n' && c != खातापूर्ण))
    {
      put_space (अग्र_prefix_indent);
      क्रम (अक्षर स्थिर *s = prefix; out_column != in_column && *s; out_column++)
        अक्षर_दो (*s++);
      अगर (c != खातापूर्ण && c != '\n')
        put_space (in_column - out_column);
      अगर (c == खातापूर्ण && in_column >= अग्र_prefix_indent + prefix_लंबाई)
        अक्षर_दो ('\n');
    }
  जबतक (c != '\n' && c != खातापूर्ण)
    {
      अक्षर_दो (c);
      c = अ_लो (f);
    }
  वापस c;
}

/* Return true if a line whose first non-blank character after the
   prefix (if any) is C could belong to the current paragraph,
   otherwise false.  */

अटल bool
same_para (पूर्णांक c)
{
  वापस (अग्र_prefix_indent == prefix_indent
          && in_column >= अग्र_prefix_indent + prefix_full_लंबाई
          && c != '\n' && c != खातापूर्ण);
}

/* Read a line from input file F, given first non-blank character C
   after the prefix, and the following indent, and break it into words.
   A word is a maximal non-empty string of non-white characters.  A word
   ending in [.?!][])"']* and followed by end-of-line or at least two
   spaces ends a sentence, as in emacs.

   Return the first non-blank character of the next line.  */

अटल पूर्णांक
get_पंक्ति (खाता *f, पूर्णांक c)
{
  पूर्णांक start;
  अक्षर *end_of_parabuf;
  WORD *end_of_word;

  end_of_parabuf = &parabuf[MAXCHARS];
  end_of_word = &word[MAXWORDS - 2];

  करो
    {				/* for each word in a line */

      /* Scan word.  */

      word_limit->text = wptr;
      करो
        {
          अगर (wptr == end_of_parabuf)
            {
              set_other_indent (true);
              flush_paragraph ();
            }
          *wptr++ = c;
          c = अ_लो (f);
        }
      जबतक (c != खातापूर्ण && !c_isspace (c));
      in_column += word_limit->लंबाई = wptr - word_limit->text;
      check_punctuation (word_limit);

      /* Scan inter-word space.  */

      start = in_column;
      c = get_space (f, c);
      word_limit->space = in_column - start;
      word_limit->final = (c == खातापूर्ण
                           || (word_limit->period
                               && (c == '\n' || word_limit->space > 1)));
      अगर (c == '\n' || c == खातापूर्ण || uniform)
        word_limit->space = word_limit->final ? 2 : 1;
      अगर (word_limit == end_of_word)
        {
          set_other_indent (true);
          flush_paragraph ();
        }
      word_limit++;
    }
  जबतक (c != '\n' && c != खातापूर्ण);
  वापस get_prefix (f);
}

/* Read a prefix from input file F.  Return either first non-matching
   character, or first non-blank character after the prefix.  */

अटल पूर्णांक
get_prefix (खाता *f)
{
  पूर्णांक c;

  in_column = 0;
  c = get_space (f, अ_लो (f));
  अगर (prefix_लंबाई == 0)
    अग्र_prefix_indent = prefix_lead_space < in_column ?
      prefix_lead_space : in_column;
  अन्यथा
    {
      अग्र_prefix_indent = in_column;
      क्रम (अक्षर स्थिर *p = prefix; *p != '\0'; p++)
        {
          अचिन्हित अक्षर pc = *p;
          अगर (c != pc)
            वापस c;
          in_column++;
          c = अ_लो (f);
        }
      c = get_space (f, c);
    }
  वापस c;
}

/* Read blank characters from input file F, starting with C, and keeping
   in_column up-to-date.  Return first non-blank character.  */

अटल पूर्णांक
get_space (खाता *f, पूर्णांक c)
{
  जबतक (true)
    {
      अगर (c == ' ')
        in_column++;
      अन्यथा अगर (c == '\t')
        {
          tabs = true;
          in_column = (in_column / TABWIDTH + 1) * TABWIDTH;
        }
      अन्यथा
        वापस c;
      c = अ_लो (f);
    }
}

/* Set extra fields in word W describing any attached punctuation.  */

अटल व्योम
check_punctuation (WORD *w)
{
  अक्षर स्थिर *start = w->text;
  अक्षर स्थिर *finish = start + (w->लंबाई - 1);
  अचिन्हित अक्षर fin = *finish;

  w->paren = isopen (*start);
  w->punct = !! है_विराम (fin);
  जबतक (start < finish && isclose (*finish))
    finish--;
  w->period = isperiod (*finish);
}

/* Flush part of the paragraph to make room.  This function is called on
   hitting the limit on the number of words or characters.  */

अटल व्योम
flush_paragraph (व्योम)
{
  WORD *split_point;
  WORD *w;
  पूर्णांक shift;
  COST best_break;

  /* In the special case where it's all one word, just flush it.  */

  अगर (word_limit == word)
    {
      माप_प्रकार to_write = wptr - parabuf;
      अगर (ख_डालो (parabuf, 1, to_write, मानक_निकास) != to_write)
        write_error ();

      wptr = parabuf;
      वापस;
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
  क्रम (w = word->अग्र_break; w != word_limit; w = w->अग्र_break)
    {
      अगर (w->best_cost - w->अग्र_break->best_cost < best_break)
        {
          split_point = w;
          best_break = w->best_cost - w->अग्र_break->best_cost;
        }
      अगर (best_break <= MAXCOST - पंक्ति_CREDIT)
        best_break += पंक्ति_CREDIT;
    }
  put_paragraph (split_point);

  /* Copy text of words down to start of parabuf -- we use memmove because
     the source and target may overlap.  */

  स_हटाओ (parabuf, split_point->text, wptr - split_point->text);
  shift = split_point->text - parabuf;
  wptr -= shift;

  /* Adjust text pointers.  */

  क्रम (w = split_point; w <= word_limit; w++)
    w->text -= shift;

  /* Copy words from split_point down to word -- we use memmove because
     the source and target may overlap.  */

  स_हटाओ (word, split_point, (word_limit - split_point + 1) * माप *word);
  word_limit -= split_point - word;
}

/* Compute the optimal formatting for the whole paragraph by computing
   and remembering the optimal formatting for each suffix from the empty
   one to the whole paragraph.  */

अटल व्योम
fmt_paragraph (व्योम)
{
  WORD *w;
  पूर्णांक len;
  COST wcost, best;
  पूर्णांक saved_लंबाई;

  word_limit->best_cost = 0;
  saved_लंबाई = word_limit->लंबाई;
  word_limit->लंबाई = max_width;	/* sentinel */

  क्रम (WORD *start = word_limit - 1; start >= word; start--)
    {
      best = MAXCOST;
      len = start == word ? first_indent : other_indent;

      /* At least one word, however long, in the line.  */

      w = start;
      len += w->लंबाई;
      करो
        {
          w++;

          /* Consider breaking before w.  */

          wcost = पंक्ति_cost (w, len) + w->best_cost;
          अगर (start == word && last_पंक्ति_लंबाई > 0)
            wcost += RAGGED_COST (len - last_पंक्ति_लंबाई);
          अगर (wcost < best)
            {
              best = wcost;
              start->अग्र_break = w;
              start->पंक्ति_लंबाई = len;
            }

          /* This is a kludge to keep us from computing 'len' as the
             sum of the sentinel length and some non-zero number.
             Since the sentinel w->length may be INT_MAX, adding
             to that would give a negative result.  */
          अगर (w == word_limit)
            अवरोध;

          len += (w - 1)->space + w->लंबाई;	/* w > start >= word */
        }
      जबतक (len <= max_width);
      start->best_cost = best + base_cost (start);
    }

  word_limit->लंबाई = saved_लंबाई;
}

/* Work around <https://gcc.gnu.org/PR109628>.  */
#अगर __GNUC__ == 13
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#पूर्ण_अगर

/* Return the constant component of the cost of breaking before the
   word THIS.  */

अटल COST
base_cost (WORD *this)
{
  COST cost;

  cost = पंक्ति_COST;

  अगर (this > word)
    {
      अगर ((this - 1)->period)
        {
          अगर ((this - 1)->final)
            cost -= SENTENCE_BONUS;
          अन्यथा
            cost += NOBREAK_COST;
        }
      अन्यथा अगर ((this - 1)->punct)
        cost -= PUNCT_BONUS;
      अन्यथा अगर (this > word + 1 && (this - 2)->final)
        cost += WIDOW_COST ((this - 1)->लंबाई);
    }

  अगर (this->paren)
    cost -= PAREN_BONUS;
  अन्यथा अगर (this->final)
    cost += ORPHAN_COST (this->लंबाई);

  वापस cost;
}

/* Return the component of the cost of breaking before word NEXT that
   depends on LEN, the length of the line beginning there.  */

अटल COST
पंक्ति_cost (WORD *अग्र, पूर्णांक len)
{
  पूर्णांक n;
  COST cost;

  अगर (अग्र == word_limit)
    वापस 0;
  n = goal_width - len;
  cost = SHORT_COST (n);
  अगर (अग्र->अग्र_break != word_limit)
    {
      n = len - अग्र->पंक्ति_लंबाई;
      cost += RAGGED_COST (n);
    }
  वापस cost;
}

/* Output to stdout a paragraph from word up to (but not including)
   FINISH, which must be in the next_break chain from word.  */

अटल व्योम
put_paragraph (WORD *finish)
{
  put_पंक्ति (word, first_indent);
  क्रम (WORD *w = word->अग्र_break; w != finish; w = w->अग्र_break)
    put_पंक्ति (w, other_indent);
}

/* Output to stdout the line beginning with word W, beginning in column
   INDENT, including the prefix (if any).  */

अटल व्योम
put_पंक्ति (WORD *w, पूर्णांक indent)
{
  WORD *endline;

  out_column = 0;
  put_space (prefix_indent);
  ख_माला_दो (prefix, मानक_निकास);
  out_column += prefix_लंबाई;
  put_space (indent - out_column);

  endline = w->अग्र_break - 1;
  क्रम (; w != endline; w++)
    {
      put_word (w);
      put_space (w->space);
    }
  put_word (w);
  last_पंक्ति_लंबाई = out_column;
  अक्षर_दो ('\n');

  अगर (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Output to stdout the word W.  */

अटल व्योम
put_word (WORD *w)
{
  अक्षर स्थिर *s = w->text;
  क्रम (पूर्णांक n = w->लंबाई; n != 0; n--)
    अक्षर_दो (*s++);
  out_column += w->लंबाई;
}

/* Output to stdout SPACE spaces, or equivalent tabs.  */

अटल व्योम
put_space (पूर्णांक space)
{
  पूर्णांक space_target, tab_target;

  space_target = out_column + space;
  अगर (tabs)
    {
      tab_target = space_target / TABWIDTH * TABWIDTH;
      अगर (out_column + 1 < tab_target)
        जबतक (out_column < tab_target)
          {
            अक्षर_दो ('\t');
            out_column = (out_column / TABWIDTH + 1) * TABWIDTH;
          }
    }
  जबतक (out_column < space_target)
    {
      अक्षर_दो (' ');
      out_column++;
    }
}
