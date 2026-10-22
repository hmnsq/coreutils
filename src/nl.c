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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "system.h"

#अन्तर्भाव <regex.h>

#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "linebuffer.h"
#अन्तर्भाव "mcel.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nl"

#define AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
स्थिर अक्षर स्थिरांक FORMAT_उजवा_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
स्थिर अक्षर स्थिरांक FORMAT_उजवा_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
स्थिर अक्षर स्थिरांक FORMAT_डावा[] = "%-*jd%s";

/* Default section delimiter characters.  */
स्थिर अक्षर DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
गणना section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
स्थिर अक्षर स्थिरांक *body_type = "t";

/* Format of header lines (-h).  */
स्थिर अक्षर स्थिरांक *header_type = "n";

/* Format of footer lines (-f).  */
स्थिर अक्षर स्थिरांक *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
स्थिर अक्षर स्थिरांक *current_type;

/* Regex for body lines to number (-bp).  */
स्थिर संरचना re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
स्थिर संरचना re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
स्थिर संरचना re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
स्थिर अक्षर body_fastmap[अच_अक्षर_उच्च + 1];
स्थिर अक्षर header_fastmap[अच_अक्षर_उच्च + 1];
स्थिर अक्षर footer_fastmap[अच_अक्षर_उच्च + 1];

/* Pointer to current regex, if any.  */
स्थिर संरचना re_pattern_buffer *current_regex = रिक्त;

/* Separator string to print after line number (-s).  */
स्थिर अक्षर स्थिरांक *separator_str = "\t";

/* Input section delimiter string (-d).  */
स्थिर अक्षर *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
स्थिर माप_प्रकार section_del_len;

/* Header delimiter string.  */
स्थिर अक्षर *header_del = रिक्त;

/* Header section delimiter length.  */
स्थिर माप_प्रकार header_del_len;

/* Body delimiter string.  */
स्थिर अक्षर *body_del = रिक्त;

/* Body section delimiter length.  */
स्थिर माप_प्रकार body_del_len;

/* Footer delimiter string.  */
स्थिर अक्षर *footer_del = रिक्त;

/* Footer section delimiter length.  */
स्थिर माप_प्रकार footer_del_len;

/* Input buffer.  */
स्थिर संरचना linebuffer ओळ_buf;

/* printf format string for unnumbered lines.  */
स्थिर अक्षर *print_no_ओळ_fmt = रिक्त;

/* Starting line number on each page (-v).  */
स्थिर intmax_t starting_ओळ_संख्या = 1;

/* Line number increment (-i).  */
स्थिर intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
स्थिर bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
स्थिर intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
स्थिर पूर्णांक lineno_width = 6;

/* Line number format (-n).  */
स्थिर अक्षर स्थिरांक *lineno_format = FORMAT_उजवा_NOLZ;

/* Current print line number.  */
स्थिर intmax_t ओळ_no;

/* Whether the current line number has incremented past limits.  */
स्थिर bool ओळ_no_overflow;

/* True if we have ever read standard input.  */
स्थिर bool have_read_stdin;

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"header-numbering", required_argument, रिक्त, 'h'},
  {"body-numbering", required_argument, रिक्त, 'b'},
  {"footer-numbering", required_argument, रिक्त, 'f'},
  {"starting-line-number", required_argument, रिक्त, 'v'},
  {"line-increment", required_argument, रिक्त, 'i'},
  {"no-renumber", no_argument, रिक्त, 'p'},
  {"join-blank-lines", required_argument, रिक्त, 'l'},
  {"number-separator", required_argument, रिक्त, 's'},
  {"number-width", required_argument, रिक्त, 'w'},
  {"number-format", required_argument, रिक्त, 'n'},
  {"section-delimiter", required_argument, रिक्त, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Print a usage message and quit. */

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      ख_माला_दो (_("\
Write each खाता to standard output, with ओळ numbers added.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE साठी numbering body ओळी\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC साठी logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE साठी numbering footer ओळी\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE साठी numbering header ओळी\n\
"));
      oputs (_("\
  -i, --ओळ-increment=संख्या_2     ओळ संख्या increment at each ओळ\n\
"));
      oputs (_("\
  -l, --join-blank-ओळी=संख्या_2   group of संख्या_2 empty ओळी counted as one\n\
"));
      oputs (_("\
  -n, --संख्या-format=FORMAT      घाला ओळ numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               करा not reset ओळ numbers साठी each section\n\
"));
      oputs (_("\
  -s, --संख्या-separator=STRING   add STRING after (possible) ओळ संख्या\n\
"));
      oputs (_("\
  -v, --starting-ओळ-संख्या=संख्या_2  first ओळ संख्या साठी each section\n\
"));
      oputs (_("\
  -w, --संख्या-width=संख्या_2       use संख्या_2 columns साठी ओळ numbers\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Default options are: -bt -d'\\:' -fn -hn -i1 -l1 -n'rn' -s<TAB> -v1 -w6\n\
\n\
CC are two delimiter characters used to construct logical page delimiters;\n\
a missing second character implies ':'.  As a GNU extension one can specify\n\
more than two characters, and also specifying the empty string (-d '')\n\
disables section matching.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
STYLE is one of:\n\
\n\
  a      संख्या all ओळी\n\
  t      संख्या only nonempty ओळी\n\
  n      संख्या no ओळी\n\
  pBRE   संख्या only ओळी that contain a match साठी the basic regular\n\
         expression, BRE\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     डावा justified, no leading zeros\n\
  rn     उजवा justified, no leading zeros\n\
  rz     उजवा justified, leading zeros\n\
\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

स्थिर bool
build_type_arg (अक्षर स्थिरांक **typep,
                संरचना re_pattern_buffer *regexp, अक्षर *fastmap)
{
  अक्षर स्थिरांक *errmsg;
  bool rval = true;

  निवड (*optarg)
    {
    स्थिती 'a':
    स्थिती 't':
    स्थिती 'n':
      *typep = optarg;
      थांबा;
    स्थिती 'p':
      *typep = optarg++;
      regexp->buffer = रिक्त;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = रिक्त;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, म_माप (optarg), regexp);
      जर (errmsg)
        error (निकास_त्रुटि, 0, "%s", (errmsg));
      थांबा;
    पूर्वनिर्धारित:
      rval = false;
      थांबा;
    }
  परत rval;
}

/* Print the line number and separator; increment the line number. */

स्थिर शून्य
print_lineno (शून्य)
{
  जर (ओळ_no_overflow)
    error (निकास_त्रुटि, 0, _("line number overflow"));

  म_लिखो (lineno_format, lineno_width, ओळ_no, separator_str);

  जर (ckd_add (&ओळ_no, ओळ_no, page_incr))
    ओळ_no_overflow = true;
}

स्थिर शून्य
reset_lineno (शून्य)
{
  जर (reset_numbers)
    {
      ओळ_no = starting_ओळ_संख्या;
      ओळ_no_overflow = false;
    }
}

/* Switch to a header section. */

स्थिर शून्य
proc_header (शून्य)
{
  current_type = header_type;
  current_regex = &header_regex;
  reset_lineno ();
  अक्षर_दो ('\n');
}

/* Switch to a body section. */

स्थिर शून्य
proc_body (शून्य)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  अक्षर_दो ('\n');
}

/* Switch to a footer section. */

स्थिर शून्य
proc_footer (शून्य)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  अक्षर_दो ('\n');
}

/* Process a regular text line in 'line_buf'. */

स्थिर शून्य
proc_text (शून्य)
{
  स्थिर intmax_t blank_ओळी = 0;	/* Consecutive blank lines so far. */

  निवड (*current_type)
    {
    स्थिती 'a':
      जर (blank_join > 1)
        {
          जर (1 < ओळ_buf.length || ++blank_ओळी == blank_join)
            {
              print_lineno ();
              blank_ओळी = 0;
            }
          अन्यथा
            ख_माला_दो (print_no_ओळ_fmt, मानक_निकास);
        }
      अन्यथा
        print_lineno ();
      थांबा;
    स्थिती 't':
      जर (1 < ओळ_buf.length)
        print_lineno ();
      अन्यथा
        ख_माला_दो (print_no_ओळ_fmt, मानक_निकास);
      थांबा;
    स्थिती 'n':
      ख_माला_दो (print_no_ओळ_fmt, मानक_निकास);
      थांबा;
    स्थिती 'p':
      निवड (re_शोध (current_regex, ओळ_buf.buffer, ओळ_buf.length - 1,
                         0, ओळ_buf.length - 1, रिक्त))
        {
        स्थिती -2:
          error (निकास_त्रुटि, त्रुटि_सं, _("error in regular expression search"));

        स्थिती -1:
          ख_माला_दो (print_no_ओळ_fmt, मानक_निकास);
          थांबा;

        पूर्वनिर्धारित:
          print_lineno ();
          थांबा;
        }
    }
  ख_डालो (ओळ_buf.buffer, आकार (अक्षर), ओळ_buf.length, मानक_निकास);
}

/* Return the type of line in 'line_buf'. */

स्थिर गणना section
check_section (शून्य)
{
  माप_प्रकार len = ओळ_buf.length - 1;

  जर (len < section_del_len || footer_del_len < section_del_len
      || !memeq (ओळ_buf.buffer, section_del, section_del_len))
    परत Text;
  जर (len == header_del_len
      && memeq (ओळ_buf.buffer, header_del, header_del_len))
    परत Header;
  जर (len == body_del_len
      && memeq (ओळ_buf.buffer, body_del, body_del_len))
    परत Body;
  जर (len == footer_del_len
      && memeq (ओळ_buf.buffer, footer_del, footer_del_len))
    परत Footer;
  परत Text;
}

/* Read and process the file pointed to by FP. */

स्थिर शून्य
process_file (खाता *fp)
{
  पर्यंत (readlinebuffer (&ओळ_buf, fp))
    {
      निवड (check_section ())
        {
        स्थिती Header:
          proc_header ();
          थांबा;
        स्थिती Body:
          proc_body ();
          थांबा;
        स्थिती Footer:
          proc_footer ();
          थांबा;
        स्थिती Text:
          proc_text ();
          थांबा;
        }

      जर (ख_त्रुटि (मानक_निकास))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

स्थिर bool
nl_file (अक्षर स्थिरांक *file)
{
  खाता *stream;

  जर (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = मानक_निवेश;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  अन्यथा
    {
      stream = ख_खोलो (file, "r");
      जर (stream == रिक्त)
        {
          error (0, त्रुटि_सं, "%s", quotef (file));
          परत false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  पूर्णांक err = त्रुटि_सं;
  जर (!ख_त्रुटि (stream))
    err = 0;
  जर (streq (file, "-"))
    साफ_त्रुटि (stream);		/* Also clear EOF. */
  अन्यथा जर (ख_बंद (stream) != 0 && !err)
    err = त्रुटि_सं;
  जर (err)
    {
      error (0, err, "%s", quotef (file));
      परत false;
    }
  परत true;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक c;
  माप_प्रकार len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           रिक्त))
         != -1)
    {
      निवड (c)
        {
        स्थिती 'h':
          जर (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          थांबा;
        स्थिती 'b':
          जर (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          थांबा;
        स्थिती 'f':
          जर (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          थांबा;
        स्थिती 'v':
          starting_ओळ_संख्या = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          थांबा;
        स्थिती 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          थांबा;
        स्थिती 'p':
          reset_numbers = false;
          थांबा;
        स्थिती 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          थांबा;
        स्थिती 's':
          separator_str = optarg;
          थांबा;
        स्थिती 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, पूर्णांक_उच्च, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          थांबा;
        स्थिती 'n':
          जर (streq (optarg, "ln"))
            lineno_format = FORMAT_डावा;
          अन्यथा जर (streq (optarg, "rn"))
            lineno_format = FORMAT_उजवा_NOLZ;
          अन्यथा जर (streq (optarg, "rz"))
            lineno_format = FORMAT_उजवा_LZ;
          अन्यथा
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          थांबा;
        स्थिती 'd':
          len = म_माप (optarg);
          जर (1 < MB_CUR_MAX)
            {
              अक्षर स्थिरांक *p = optarg;
              अक्षर स्थिरांक *lim = p + len;
              पूर्णांक n_chars = 0;
              साठी (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              जर (n_chars == 1)
                स_नकल (mempcpy (section_del, optarg, len),  ":", आकार ":");
              अन्यथा
                section_del = optarg;
            }
          अन्यथा
            {
              जर (len == 1)
                *section_del = *optarg;
              अन्यथा
                section_del = optarg;
            }
          थांबा;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          ok = false;
          थांबा;
        }
    }

  जर (!ok)
    usage (निकास_त्रुटि);

  /* Initialize the section delimiters.  */
  section_del_len = len = म_माप (section_del);

  header_del_len = len * 3;
  header_del = xmalloc (header_del_len + 1);
  stpcpy (stpcpy (stpcpy (header_del, section_del), section_del), section_del);

  body_del_len = len * 2;
  body_del = header_del + len;

  footer_del_len = len;
  footer_del = body_del + len;

  /* Initialize the input buffer.  */
  initbuffer (&ओळ_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = म_माप (separator_str);
  print_no_ओळ_fmt = xmalloc (lineno_width + len + 1);
  स_रखो (print_no_ओळ_fmt, ' ', lineno_width + len);
  print_no_ओळ_fmt[lineno_width + len] = '\0';

  ओळ_no = starting_ओळ_संख्या;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  जर (optind == argc)
    ok = nl_file ("-");
  अन्यथा
    साठी (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  जर (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    error (निकास_त्रुटि, त्रुटि_सं, "-");

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
