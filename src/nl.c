/* nl -- number lines of files
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Written by Scott Bartram (nancy!scott@uunet.uu.net)
   Revised by David MacKenzie (djm@gnu.ai.mit.edu) */

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>

#అన్తర్భావ "system.h"

#అన్తర్భావ <regex.h>

#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "linebuffer.h"
#అన్తర్భావ "mcel.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "nl"

#నిర్వచించు AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
స్థిర అక్షరం స్థిరరాశి FORMAT_కుడి_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
స్థిర అక్షరం స్థిరరాశి FORMAT_కుడి_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
స్థిర అక్షరం స్థిరరాశి FORMAT_ఎడమ[] = "%-*jd%s";

/* Default section delimiter characters.  */
స్థిర అక్షరం DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
గణన section
{
  శీర్షిక_2, Body, Footer, పాఠ్యం_2
};

/* Format of body lines (-b).  */
స్థిర అక్షరం స్థిరరాశి *body_type = "t";

/* Format of header lines (-h).  */
స్థిర అక్షరం స్థిరరాశి *శీర్షిక_type = "n";

/* Format of footer lines (-f).  */
స్థిర అక్షరం స్థిరరాశి *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
స్థిర అక్షరం స్థిరరాశి *current_type;

/* Regex for body lines to number (-bp).  */
స్థిర నిర్మాణం re_నమూనా_బఫరు body_regex;

/* Regex for header lines to number (-hp).  */
స్థిర నిర్మాణం re_నమూనా_బఫరు శీర్షిక_regex;

/* Regex for footer lines to number (-fp).  */
స్థిర నిర్మాణం re_నమూనా_బఫరు footer_regex;

/* Fastmaps for the above.  */
స్థిర అక్షరం body_fastmap[UCHAR_MAX + 1];
స్థిర అక్షరం శీర్షిక_fastmap[UCHAR_MAX + 1];
స్థిర అక్షరం footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
స్థిర నిర్మాణం re_నమూనా_బఫరు *current_regex = ఖాళీ;

/* Separator string to print after line number (-s).  */
స్థిర అక్షరం స్థిరరాశి *separator_str = "\t";

/* Input section delimiter string (-d).  */
స్థిర అక్షరం *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
స్థిర size_t section_del_len;

/* Header delimiter string.  */
స్థిర అక్షరం *శీర్షిక_del = ఖాళీ;

/* Header section delimiter length.  */
స్థిర size_t శీర్షిక_del_len;

/* Body delimiter string.  */
స్థిర అక్షరం *body_del = ఖాళీ;

/* Body section delimiter length.  */
స్థిర size_t body_del_len;

/* Footer delimiter string.  */
స్థిర అక్షరం *footer_del = ఖాళీ;

/* Footer section delimiter length.  */
స్థిర size_t footer_del_len;

/* Input buffer.  */
స్థిర నిర్మాణం linebuffer పంక్తి_buf;

/* printf format string for unnumbered lines.  */
స్థిర అక్షరం *print_no_పంక్తి_fmt = ఖాళీ;

/* Starting line number on each page (-v).  */
స్థిర intmax_t starting_పంక్తి_సంఖ్య = 1;

/* Line number increment (-i).  */
స్థిర intmax_t పుట_incr = 1;

/* If true, reset line number at start of each page (-p).  */
స్థిర bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
స్థిర intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
స్థిర పూర్ణాంకం lineno_వెడల్పు = 6;

/* Line number format (-n).  */
స్థిర అక్షరం స్థిరరాశి *lineno_format = FORMAT_కుడి_NOLZ;

/* Current print line number.  */
స్థిర intmax_t పంక్తి_no;

/* Whether the current line number has incremented past limits.  */
స్థిర bool పంక్తి_no_అధికప్రవాహం;

/* True if we have ever read standard input.  */
స్థిర bool have_read_stdin;

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"header-numbering", required_వాదం, ఖాళీ, 'h'},
  {"body-numbering", required_వాదం, ఖాళీ, 'b'},
  {"footer-numbering", required_వాదం, ఖాళీ, 'f'},
  {"starting-line-number", required_వాదం, ఖాళీ, 'v'},
  {"line-increment", required_వాదం, ఖాళీ, 'i'},
  {"no-renumber", no_వాదం, ఖాళీ, 'p'},
  {"join-blank-lines", required_వాదం, ఖాళీ, 'l'},
  {"number-separator", required_వాదం, ఖాళీ, 's'},
  {"number-width", required_వాదం, ఖాళీ, 'w'},
  {"number-format", required_వాదం, ఖాళీ, 'n'},
  {"section-delimiter", required_వాదం, ఖాళీ, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Print a usage message and quit. */

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Write each FILE గమ్యం standard output, with పంక్తి numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE కొరకు numbering body పంక్తులు\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC కొరకు logical పుట delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE కొరకు numbering footer పంక్తులు\n\
"));
      oputs (_("\
  -h, --శీర్షిక-numbering=STYLE    use STYLE కొరకు numbering శీర్షిక పంక్తులు\n\
"));
      oputs (_("\
  -i, --పంక్తి-increment=సంఖ్య_2     పంక్తి సంఖ్య increment at each పంక్తి\n\
"));
      oputs (_("\
  -l, --join-blank-పంక్తులు=సంఖ్య_2   సమూహం యొక్క సంఖ్య_2 ఖాళీలు పంక్తులు counted as one\n\
"));
      oputs (_("\
  -n, --సంఖ్య-format=FORMAT      చొప్పించు పంక్తి numbers according గమ్యం FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               చేయి not reset పంక్తి numbers కొరకు each section\n\
"));
      oputs (_("\
  -s, --సంఖ్య-separator=STRING   కూడు STRING after (possible) పంక్తి సంఖ్య\n\
"));
      oputs (_("\
  -v, --starting-పంక్తి-సంఖ్య=సంఖ్య_2  మొదటి పంక్తి సంఖ్య కొరకు each section\n\
"));
      oputs (_("\
  -w, --సంఖ్య-వెడల్పు=సంఖ్య_2       use సంఖ్య_2 columns కొరకు పంక్తి numbers\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Default options are: -bt -d'\\:' -fn -hn -i1 -l1 -n'rn' -s<TAB> -v1 -w6\n\
\n\
CC are two delimiter characters వాడిన గమ్యం construct logical పుట delimiters;\n\
a missing రెండవ character సూచిస్తుంది ':'.  As a GNU extension one can specify\n\
మరిన్ని than two characters, and also specifying the ఖాళీలు string (-d '')\n\
disables section matching.\n\
"), stdout);
      fputs (_("\
\n\
STYLE is one యొక్క:\n\
\n\
  a      సంఖ్య all పంక్తులు\n\
  t      సంఖ్య only nonempty పంక్తులు\n\
  n      సంఖ్య no పంక్తులు\n\
  pBRE   సంఖ్య only పంక్తులు that contain a సరిపోలిక కొరకు the basic regular\n\
         వ్యక్తీకరణ_2, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one యొక్క:\n\
\n\
  ln     ఎడమ justified, no leading zeros\n\
  rn     కుడి justified, no leading zeros\n\
  rz     కుడి justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

స్థిర bool
build_type_arg (అక్షరం స్థిరరాశి **typep,
                నిర్మాణం re_నమూనా_బఫరు *regexp, అక్షరం *fastmap)
{
  అక్షరం స్థిరరాశి *errmsg;
  bool rval = true;

  ఎంపిక (*optarg)
    {
    స్థితి 'a':
    స్థితి 't':
    స్థితి 'n':
      *typep = optarg;
      ఆపు;
    స్థితి 'p':
      *typep = optarg++;
      regexp->బఫరు = ఖాళీ;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = ఖాళీ;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_ఖాళీలు_RANGES;
      errmsg = re_compile_నమూనా (optarg, అక్షరశ్రేణి_పొడవు (optarg), regexp);
      ఒకవేళ (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      ఆపు;
    అప్రమేయ:
      rval = false;
      ఆపు;
    }
  తిరిగి rval;
}

/* Print the line number and separator; increment the line number. */

స్థిర శూన్య
print_lineno (శూన్య)
{
  ఒకవేళ (పంక్తి_no_అధికప్రవాహం)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  రాయి (lineno_format, lineno_వెడల్పు, పంక్తి_no, separator_str);

  ఒకవేళ (ckd_కూడు (&పంక్తి_no, పంక్తి_no, పుట_incr))
    పంక్తి_no_అధికప్రవాహం = true;
}

స్థిర శూన్య
reset_lineno (శూన్య)
{
  ఒకవేళ (reset_numbers)
    {
      పంక్తి_no = starting_పంక్తి_సంఖ్య;
      పంక్తి_no_అధికప్రవాహం = false;
    }
}

/* Switch to a header section. */

స్థిర శూన్య
proc_శీర్షిక (శూన్య)
{
  current_type = శీర్షిక_type;
  current_regex = &శీర్షిక_regex;
  reset_lineno ();
  అక్షరం_రాయి ('\n');
}

/* Switch to a body section. */

స్థిర శూన్య
proc_body (శూన్య)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  అక్షరం_రాయి ('\n');
}

/* Switch to a footer section. */

స్థిర శూన్య
proc_footer (శూన్య)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  అక్షరం_రాయి ('\n');
}

/* Process a regular text line in 'line_buf'. */

స్థిర శూన్య
proc_పాఠ్యం (శూన్య)
{
  స్థిర intmax_t blank_పంక్తులు = 0;	/* Consecutive blank lines so far. */

  ఎంపిక (*current_type)
    {
    స్థితి 'a':
      ఒకవేళ (blank_join > 1)
        {
          ఒకవేళ (1 < పంక్తి_buf.నిడివి || ++blank_పంక్తులు == blank_join)
            {
              print_lineno ();
              blank_పంక్తులు = 0;
            }
          లేకపోతే
            fputs (print_no_పంక్తి_fmt, stdout);
        }
      లేకపోతే
        print_lineno ();
      ఆపు;
    స్థితి 't':
      ఒకవేళ (1 < పంక్తి_buf.నిడివి)
        print_lineno ();
      లేకపోతే
        fputs (print_no_పంక్తి_fmt, stdout);
      ఆపు;
    స్థితి 'n':
      fputs (print_no_పంక్తి_fmt, stdout);
      ఆపు;
    స్థితి 'p':
      ఎంపిక (re_శోధన (current_regex, పంక్తి_buf.బఫరు, పంక్తి_buf.నిడివి - 1,
                         0, పంక్తి_buf.నిడివి - 1, ఖాళీ))
        {
        స్థితి -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        స్థితి -1:
          fputs (print_no_పంక్తి_fmt, stdout);
          ఆపు;

        అప్రమేయ:
          print_lineno ();
          ఆపు;
        }
    }
  fwrite (పంక్తి_buf.బఫరు, పరిమాణం (అక్షరం), పంక్తి_buf.నిడివి, stdout);
}

/* Return the type of line in 'line_buf'. */

స్థిర గణన section
check_section (శూన్య)
{
  size_t len = పంక్తి_buf.నిడివి - 1;

  ఒకవేళ (len < section_del_len || footer_del_len < section_del_len
      || !memeq (పంక్తి_buf.బఫరు, section_del, section_del_len))
    తిరిగి పాఠ్యం_2;
  ఒకవేళ (len == శీర్షిక_del_len
      && memeq (పంక్తి_buf.బఫరు, శీర్షిక_del, శీర్షిక_del_len))
    తిరిగి శీర్షిక_2;
  ఒకవేళ (len == body_del_len
      && memeq (పంక్తి_buf.బఫరు, body_del, body_del_len))
    తిరిగి Body;
  ఒకవేళ (len == footer_del_len
      && memeq (పంక్తి_buf.బఫరు, footer_del, footer_del_len))
    తిరిగి Footer;
  తిరిగి పాఠ్యం_2;
}

/* Read and process the file pointed to by FP. */

స్థిర శూన్య
process_file (FILE *fp)
{
  అయితే (readlinebuffer (&పంక్తి_buf, fp))
    {
      ఎంపిక (check_section ())
        {
        స్థితి శీర్షిక_2:
          proc_శీర్షిక ();
          ఆపు;
        స్థితి Body:
          proc_body ();
          ఆపు;
        స్థితి Footer:
          proc_footer ();
          ఆపు;
        స్థితి పాఠ్యం_2:
          proc_పాఠ్యం ();
          ఆపు;
        }

      ఒకవేళ (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

స్థిర bool
nl_file (అక్షరం స్థిరరాశి *file)
{
  FILE *ప్రవాహం;

  ఒకవేళ (streq (file, "-"))
    {
      have_read_stdin = true;
      ప్రవాహం = stdin;
      assume (ప్రవాహం);  /* Pacify GCC bug#109613.  */
    }
  లేకపోతే
    {
      ప్రవాహం = fopen (file, "r");
      ఒకవేళ (ప్రవాహం == ఖాళీ)
        {
          error (0, errno, "%s", quotef (file));
          తిరిగి false;
        }
    }

  fadvise (ప్రవాహం, FADVISE_SEQUENTIAL);

  process_file (ప్రవాహం);

  పూర్ణాంకం err = errno;
  ఒకవేళ (!ferror (ప్రవాహం))
    err = 0;
  ఒకవేళ (streq (file, "-"))
    clearerr (ప్రవాహం);		/* Also clear EOF. */
  లేకపోతే ఒకవేళ (fclose (ప్రవాహం) != 0 && !err)
    err = errno;
  ఒకవేళ (err)
    {
      error (0, err, "%s", quotef (file));
      తిరిగి false;
    }
  తిరిగి true;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           ఖాళీ))
         != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'h':
          ఒకవేళ (! build_type_arg (&శీర్షిక_type, &శీర్షిక_regex, శీర్షిక_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ఆపు;
        స్థితి 'b':
          ఒకవేళ (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ఆపు;
        స్థితి 'f':
          ఒకవేళ (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ఆపు;
        స్థితి 'v':
          starting_పంక్తి_సంఖ్య = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          ఆపు;
        స్థితి 'i':
          పుట_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          ఆపు;
        స్థితి 'p':
          reset_numbers = false;
          ఆపు;
        స్థితి 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          ఆపు;
        స్థితి 's':
          separator_str = optarg;
          ఆపు;
        స్థితి 'w':
          lineno_వెడల్పు = xnumtoimax (optarg, 10, 1, గరిష్ఠ_పూర్ణాంకం, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          ఆపు;
        స్థితి 'n':
          ఒకవేళ (streq (optarg, "ln"))
            lineno_format = FORMAT_ఎడమ;
          లేకపోతే ఒకవేళ (streq (optarg, "rn"))
            lineno_format = FORMAT_కుడి_NOLZ;
          లేకపోతే ఒకవేళ (streq (optarg, "rz"))
            lineno_format = FORMAT_కుడి_LZ;
          లేకపోతే
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          ఆపు;
        స్థితి 'd':
          len = అక్షరశ్రేణి_పొడవు (optarg);
          ఒకవేళ (1 < MB_CUR_MAX)
            {
              అక్షరం స్థిరరాశి *p = optarg;
              అక్షరం స్థిరరాశి *lim = p + len;
              పూర్ణాంకం n_chars = 0;
              కొరకు (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_పరిశీలన (p, lim).len;
              ఒకవేళ (n_chars == 1)
                జ్ఞాపకం_నకలు (mempcpy (section_del, optarg, len),  ":", పరిమాణం ":");
              లేకపోతే
                section_del = optarg;
            }
          లేకపోతే
            {
              ఒకవేళ (len == 1)
                *section_del = *optarg;
              లేకపోతే
                section_del = optarg;
            }
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          ok = false;
          ఆపు;
        }
    }

  ఒకవేళ (!ok)
    usage (EXIT_FAILURE);

  /* Initialize the section delimiters.  */
  section_del_len = len = అక్షరశ్రేణి_పొడవు (section_del);

  శీర్షిక_del_len = len * 3;
  శీర్షిక_del = xmalloc (శీర్షిక_del_len + 1);
  stpcpy (stpcpy (stpcpy (శీర్షిక_del, section_del), section_del), section_del);

  body_del_len = len * 2;
  body_del = శీర్షిక_del + len;

  footer_del_len = len;
  footer_del = body_del + len;

  /* Initialize the input buffer.  */
  initbuffer (&పంక్తి_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = అక్షరశ్రేణి_పొడవు (separator_str);
  print_no_పంక్తి_fmt = xmalloc (lineno_వెడల్పు + len + 1);
  memset (print_no_పంక్తి_fmt, ' ', lineno_వెడల్పు + len);
  print_no_పంక్తి_fmt[lineno_వెడల్పు + len] = '\0';

  పంక్తి_no = starting_పంక్తి_సంఖ్య;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  ఒకవేళ (optind == argc)
    ok = nl_file ("-");
  లేకపోతే
    కొరకు (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  ఒకవేళ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
