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
स्थिर वर्ण अचर FORMAT_दायाँ_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
स्थिर वर्ण अचर FORMAT_दायाँ_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
स्थिर वर्ण अचर FORMAT_बायाँ[] = "%-*jd%s";

/* Default section delimiter characters.  */
स्थिर वर्ण DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
गणना section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
स्थिर वर्ण अचर *body_type = "t";

/* Format of header lines (-h).  */
स्थिर वर्ण अचर *header_type = "n";

/* Format of footer lines (-f).  */
स्थिर वर्ण अचर *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
स्थिर वर्ण अचर *current_type;

/* Regex for body lines to number (-bp).  */
स्थिर संरचना re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
स्थिर संरचना re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
स्थिर संरचना re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
स्थिर वर्ण body_fastmap[अच_अक्षर_उच्च + 1];
स्थिर वर्ण header_fastmap[अच_अक्षर_उच्च + 1];
स्थिर वर्ण footer_fastmap[अच_अक्षर_उच्च + 1];

/* Pointer to current regex, if any.  */
स्थिर संरचना re_pattern_buffer *current_regex = रिक्त;

/* Separator string to print after line number (-s).  */
स्थिर वर्ण अचर *separator_str = "\t";

/* Input section delimiter string (-d).  */
स्थिर वर्ण *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
स्थिर माप_प्रकार section_del_len;

/* Header delimiter string.  */
स्थिर वर्ण *header_del = रिक्त;

/* Header section delimiter length.  */
स्थिर माप_प्रकार header_del_len;

/* Body delimiter string.  */
स्थिर वर्ण *body_del = रिक्त;

/* Body section delimiter length.  */
स्थिर माप_प्रकार body_del_len;

/* Footer delimiter string.  */
स्थिर वर्ण *footer_del = रिक्त;

/* Footer section delimiter length.  */
स्थिर माप_प्रकार footer_del_len;

/* Input buffer.  */
स्थिर संरचना linebuffer पङ्क्ति_buf;

/* printf format string for unnumbered lines.  */
स्थिर वर्ण *print_no_पङ्क्ति_fmt = रिक्त;

/* Starting line number on each page (-v).  */
स्थिर intmax_t starting_पङ्क्ति_सङ्ख्या = 1;

/* Line number increment (-i).  */
स्थिर intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
स्थिर bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
स्थिर intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
स्थिर पूर्णाङ्क lineno_width = 6;

/* Line number format (-n).  */
स्थिर वर्ण अचर *lineno_format = FORMAT_दायाँ_NOLZ;

/* Current print line number.  */
स्थिर intmax_t पङ्क्ति_no;

/* Whether the current line number has incremented past limits.  */
स्थिर bool पङ्क्ति_no_overflow;

/* True if we have ever read standard input.  */
स्थिर bool have_read_stdin;

स्थिर संरचना option अचर longopts[] =
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
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      ख_माला_दो (_("\
Write each खाता to standard output, with पङ्क्ति numbers added.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE लागि numbering body पङ्क्तिहरू\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC लागि logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE लागि numbering footer पङ्क्तिहरू\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE लागि numbering header पङ्क्तिहरू\n\
"));
      oputs (_("\
  -i, --पङ्क्ति-increment=सङ्ख्या_2     पङ्क्ति सङ्ख्या increment at each पङ्क्ति\n\
"));
      oputs (_("\
  -l, --join-blank-पङ्क्तिहरू=सङ्ख्या_2   group of सङ्ख्या_2 empty पङ्क्तिहरू counted as one\n\
"));
      oputs (_("\
  -n, --सङ्ख्या-format=FORMAT      घुसाउ पङ्क्ति numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               गर not reset पङ्क्ति numbers लागि each section\n\
"));
      oputs (_("\
  -s, --सङ्ख्या-separator=STRING   add STRING after (possible) पङ्क्ति सङ्ख्या\n\
"));
      oputs (_("\
  -v, --starting-पङ्क्ति-सङ्ख्या=सङ्ख्या_2  first पङ्क्ति सङ्ख्या लागि each section\n\
"));
      oputs (_("\
  -w, --सङ्ख्या-width=सङ्ख्या_2       use सङ्ख्या_2 columns लागि पङ्क्ति numbers\n\
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
  a      सङ्ख्या all पङ्क्तिहरू\n\
  t      सङ्ख्या only nonempty पङ्क्तिहरू\n\
  n      सङ्ख्या no पङ्क्तिहरू\n\
  pBRE   सङ्ख्या only पङ्क्तिहरू that contain a match लागि the basic regular\n\
         expression, BRE\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     बायाँ justified, no leading zeros\n\
  rn     दायाँ justified, no leading zeros\n\
  rz     दायाँ justified, leading zeros\n\
\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

स्थिर bool
build_type_arg (वर्ण अचर **typep,
                संरचना re_pattern_buffer *regexp, वर्ण *fastmap)
{
  वर्ण अचर *errmsg;
  bool rval = true;

  विकल्प (*optarg)
    {
    पक्ष 'a':
    पक्ष 't':
    पक्ष 'n':
      *typep = optarg;
      रोक;
    पक्ष 'p':
      *typep = optarg++;
      regexp->buffer = रिक्त;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = रिक्त;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, म_माप (optarg), regexp);
      यदि (errmsg)
        error (निकास_त्रुटि, 0, "%s", (errmsg));
      रोक;
    स्वभाव:
      rval = false;
      रोक;
    }
  फर्काऊ rval;
}

/* Print the line number and separator; increment the line number. */

स्थिर शून्य
print_lineno (शून्य)
{
  यदि (पङ्क्ति_no_overflow)
    error (निकास_त्रुटि, 0, _("line number overflow"));

  म_लिखो (lineno_format, lineno_width, पङ्क्ति_no, separator_str);

  यदि (ckd_add (&पङ्क्ति_no, पङ्क्ति_no, page_incr))
    पङ्क्ति_no_overflow = true;
}

स्थिर शून्य
reset_lineno (शून्य)
{
  यदि (reset_numbers)
    {
      पङ्क्ति_no = starting_पङ्क्ति_सङ्ख्या;
      पङ्क्ति_no_overflow = false;
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
  स्थिर intmax_t blank_पङ्क्तिहरू = 0;	/* Consecutive blank lines so far. */

  विकल्प (*current_type)
    {
    पक्ष 'a':
      यदि (blank_join > 1)
        {
          यदि (1 < पङ्क्ति_buf.length || ++blank_पङ्क्तिहरू == blank_join)
            {
              print_lineno ();
              blank_पङ्क्तिहरू = 0;
            }
          अन्यथा
            ख_माला_दो (print_no_पङ्क्ति_fmt, मानक_निकास);
        }
      अन्यथा
        print_lineno ();
      रोक;
    पक्ष 't':
      यदि (1 < पङ्क्ति_buf.length)
        print_lineno ();
      अन्यथा
        ख_माला_दो (print_no_पङ्क्ति_fmt, मानक_निकास);
      रोक;
    पक्ष 'n':
      ख_माला_दो (print_no_पङ्क्ति_fmt, मानक_निकास);
      रोक;
    पक्ष 'p':
      विकल्प (re_खोज (current_regex, पङ्क्ति_buf.buffer, पङ्क्ति_buf.length - 1,
                         0, पङ्क्ति_buf.length - 1, रिक्त))
        {
        पक्ष -2:
          error (निकास_त्रुटि, त्रुटि_सं, _("error in regular expression search"));

        पक्ष -1:
          ख_माला_दो (print_no_पङ्क्ति_fmt, मानक_निकास);
          रोक;

        स्वभाव:
          print_lineno ();
          रोक;
        }
    }
  ख_डालो (पङ्क्ति_buf.buffer, परिमाण (वर्ण), पङ्क्ति_buf.length, मानक_निकास);
}

/* Return the type of line in 'line_buf'. */

स्थिर गणना section
check_section (शून्य)
{
  माप_प्रकार len = पङ्क्ति_buf.length - 1;

  यदि (len < section_del_len || footer_del_len < section_del_len
      || !memeq (पङ्क्ति_buf.buffer, section_del, section_del_len))
    फर्काऊ Text;
  यदि (len == header_del_len
      && memeq (पङ्क्ति_buf.buffer, header_del, header_del_len))
    फर्काऊ Header;
  यदि (len == body_del_len
      && memeq (पङ्क्ति_buf.buffer, body_del, body_del_len))
    फर्काऊ Body;
  यदि (len == footer_del_len
      && memeq (पङ्क्ति_buf.buffer, footer_del, footer_del_len))
    फर्काऊ Footer;
  फर्काऊ Text;
}

/* Read and process the file pointed to by FP. */

स्थिर शून्य
process_file (खाता *fp)
{
  जबसम्म (readlinebuffer (&पङ्क्ति_buf, fp))
    {
      विकल्प (check_section ())
        {
        पक्ष Header:
          proc_header ();
          रोक;
        पक्ष Body:
          proc_body ();
          रोक;
        पक्ष Footer:
          proc_footer ();
          रोक;
        पक्ष Text:
          proc_text ();
          रोक;
        }

      यदि (ख_त्रुटि (मानक_निकास))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

स्थिर bool
nl_file (वर्ण अचर *file)
{
  खाता *stream;

  यदि (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = मानक_निवेश;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  अन्यथा
    {
      stream = ख_खोलो (file, "r");
      यदि (stream == रिक्त)
        {
          error (0, त्रुटि_सं, "%s", quotef (file));
          फर्काऊ false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  पूर्णाङ्क err = त्रुटि_सं;
  यदि (!ख_त्रुटि (stream))
    err = 0;
  यदि (streq (file, "-"))
    साफ_त्रुटि (stream);		/* Also clear EOF. */
  अन्यथा यदि (ख_बंद (stream) != 0 && !err)
    err = त्रुटि_सं;
  यदि (err)
    {
      error (0, err, "%s", quotef (file));
      फर्काऊ false;
    }
  फर्काऊ true;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;
  माप_प्रकार len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबसम्म ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           रिक्त))
         != -1)
    {
      विकल्प (c)
        {
        पक्ष 'h':
          यदि (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          रोक;
        पक्ष 'b':
          यदि (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          रोक;
        पक्ष 'f':
          यदि (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          रोक;
        पक्ष 'v':
          starting_पङ्क्ति_सङ्ख्या = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          रोक;
        पक्ष 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          रोक;
        पक्ष 'p':
          reset_numbers = false;
          रोक;
        पक्ष 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          रोक;
        पक्ष 's':
          separator_str = optarg;
          रोक;
        पक्ष 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, पूर्णांक_उच्च, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          रोक;
        पक्ष 'n':
          यदि (streq (optarg, "ln"))
            lineno_format = FORMAT_बायाँ;
          अन्यथा यदि (streq (optarg, "rn"))
            lineno_format = FORMAT_दायाँ_NOLZ;
          अन्यथा यदि (streq (optarg, "rz"))
            lineno_format = FORMAT_दायाँ_LZ;
          अन्यथा
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          रोक;
        पक्ष 'd':
          len = म_माप (optarg);
          यदि (1 < MB_CUR_MAX)
            {
              वर्ण अचर *p = optarg;
              वर्ण अचर *lim = p + len;
              पूर्णाङ्क n_chars = 0;
              लागि (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              यदि (n_chars == 1)
                स_नकल (mempcpy (section_del, optarg, len),  ":", परिमाण ":");
              अन्यथा
                section_del = optarg;
            }
          अन्यथा
            {
              यदि (len == 1)
                *section_del = *optarg;
              अन्यथा
                section_del = optarg;
            }
          रोक;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          ok = false;
          रोक;
        }
    }

  यदि (!ok)
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
  initbuffer (&पङ्क्ति_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = म_माप (separator_str);
  print_no_पङ्क्ति_fmt = xmalloc (lineno_width + len + 1);
  स_रखो (print_no_पङ्क्ति_fmt, ' ', lineno_width + len);
  print_no_पङ्क्ति_fmt[lineno_width + len] = '\0';

  पङ्क्ति_no = starting_पङ्क्ति_सङ्ख्या;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  यदि (optind == argc)
    ok = nl_file ("-");
  अन्यथा
    लागि (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  यदि (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    error (निकास_त्रुटि, त्रुटि_सं, "-");

  फर्काऊ ok ? निकास_सफल : निकास_त्रुटि;
}
