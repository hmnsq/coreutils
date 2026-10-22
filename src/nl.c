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
स्थिर वर्ण ध्रुव FORMAT_उजवा_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
स्थिर वर्ण ध्रुव FORMAT_उजवा_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
स्थिर वर्ण ध्रुव FORMAT_डावा[] = "%-*jd%s";

/* Default section delimiter characters.  */
स्थिर वर्ण DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
गणना section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
स्थिर वर्ण ध्रुव *body_type = "t";

/* Format of header lines (-h).  */
स्थिर वर्ण ध्रुव *header_type = "n";

/* Format of footer lines (-f).  */
स्थिर वर्ण ध्रुव *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
स्थिर वर्ण ध्रुव *current_type;

/* Regex for body lines to number (-bp).  */
स्थिर संरचना re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
स्थिर संरचना re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
स्थिर संरचना re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
स्थिर वर्ण body_fastmap[UCHAR_MAX + 1];
स्थिर वर्ण header_fastmap[UCHAR_MAX + 1];
स्थिर वर्ण footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
स्थिर संरचना re_pattern_buffer *current_regex = रिक्त;

/* Separator string to print after line number (-s).  */
स्थिर वर्ण ध्रुव *separator_str = "\t";

/* Input section delimiter string (-d).  */
स्थिर वर्ण *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
स्थिर size_t section_del_len;

/* Header delimiter string.  */
स्थिर वर्ण *header_del = रिक्त;

/* Header section delimiter length.  */
स्थिर size_t header_del_len;

/* Body delimiter string.  */
स्थिर वर्ण *body_del = रिक्त;

/* Body section delimiter length.  */
स्थिर size_t body_del_len;

/* Footer delimiter string.  */
स्थिर वर्ण *footer_del = रिक्त;

/* Footer section delimiter length.  */
स्थिर size_t footer_del_len;

/* Input buffer.  */
स्थिर संरचना linebuffer ओळ_buf;

/* printf format string for unnumbered lines.  */
स्थिर वर्ण *print_no_ओळ_fmt = रिक्त;

/* Starting line number on each page (-v).  */
स्थिर intmax_t starting_ओळ_संख्या = 1;

/* Line number increment (-i).  */
स्थिर intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
स्थिर bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
स्थिर intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
स्थिर पूर्णाङ्क lineno_width = 6;

/* Line number format (-n).  */
स्थिर वर्ण ध्रुव *lineno_format = FORMAT_उजवा_NOLZ;

/* Current print line number.  */
स्थिर intmax_t ओळ_no;

/* Whether the current line number has incremented past limits.  */
स्थिर bool ओळ_no_overflow;

/* True if we have ever read standard input.  */
स्थिर bool have_read_stdin;

स्थिर संरचना option ध्रुव longopts[] =
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
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, with ओळ numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE प्रति numbering body ओळी\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC प्रति logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE प्रति numbering footer ओळी\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE प्रति numbering header ओळी\n\
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
  -p, --no-renumber               कुरु not reset ओळ numbers प्रति each section\n\
"));
      oputs (_("\
  -s, --संख्या-separator=STRING   add STRING after (possible) ओळ संख्या\n\
"));
      oputs (_("\
  -v, --starting-ओळ-संख्या=संख्या_2  first ओळ संख्या प्रति each section\n\
"));
      oputs (_("\
  -w, --संख्या-width=संख्या_2       use संख्या_2 columns प्रति ओळ numbers\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Default options are: -bt -d'\\:' -fn -hn -i1 -l1 -n'rn' -s<TAB> -v1 -w6\n\
\n\
CC are two delimiter characters used to construct logical page delimiters;\n\
a missing second character implies ':'.  As a GNU extension one can specify\n\
more than two characters, and also specifying the empty string (-d '')\n\
disables section matching.\n\
"), stdout);
      fputs (_("\
\n\
STYLE is one of:\n\
\n\
  a      संख्या all ओळी\n\
  t      संख्या only nonempty ओळी\n\
  n      संख्या no ओळी\n\
  pBRE   संख्या only ओळी that contain a match प्रति the basic regular\n\
         expression, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     डावा justified, no leading zeros\n\
  rn     उजवा justified, no leading zeros\n\
  rz     उजवा justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

स्थिर bool
build_type_arg (वर्ण ध्रुव **typep,
                संरचना re_pattern_buffer *regexp, वर्ण *fastmap)
{
  वर्ण ध्रुव *errmsg;
  bool rval = true;

  विकल्प (*optarg)
    {
    पक्ष 'a':
    पक्ष 't':
    पक्ष 'n':
      *typep = optarg;
      भङ्ग;
    पक्ष 'p':
      *typep = optarg++;
      regexp->buffer = रिक्त;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = रिक्त;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, strlen (optarg), regexp);
      यदि (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      भङ्ग;
    स्वभाव:
      rval = false;
      भङ्ग;
    }
  प्रत्यागम rval;
}

/* Print the line number and separator; increment the line number. */

स्थिर शून्य
print_lineno (शून्य)
{
  यदि (ओळ_no_overflow)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  printf (lineno_format, lineno_width, ओळ_no, separator_str);

  यदि (ckd_add (&ओळ_no, ओळ_no, page_incr))
    ओळ_no_overflow = true;
}

स्थिर शून्य
reset_lineno (शून्य)
{
  यदि (reset_numbers)
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
  putchar ('\n');
}

/* Switch to a body section. */

स्थिर शून्य
proc_body (शून्य)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a footer section. */

स्थिर शून्य
proc_footer (शून्य)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Process a regular text line in 'line_buf'. */

स्थिर शून्य
proc_text (शून्य)
{
  स्थिर intmax_t blank_ओळी = 0;	/* Consecutive blank lines so far. */

  विकल्प (*current_type)
    {
    पक्ष 'a':
      यदि (blank_join > 1)
        {
          यदि (1 < ओळ_buf.length || ++blank_ओळी == blank_join)
            {
              print_lineno ();
              blank_ओळी = 0;
            }
          अन्यथा
            fputs (print_no_ओळ_fmt, stdout);
        }
      अन्यथा
        print_lineno ();
      भङ्ग;
    पक्ष 't':
      यदि (1 < ओळ_buf.length)
        print_lineno ();
      अन्यथा
        fputs (print_no_ओळ_fmt, stdout);
      भङ्ग;
    पक्ष 'n':
      fputs (print_no_ओळ_fmt, stdout);
      भङ्ग;
    पक्ष 'p':
      विकल्प (re_शोध (current_regex, ओळ_buf.buffer, ओळ_buf.length - 1,
                         0, ओळ_buf.length - 1, रिक्त))
        {
        पक्ष -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        पक्ष -1:
          fputs (print_no_ओळ_fmt, stdout);
          भङ्ग;

        स्वभाव:
          print_lineno ();
          भङ्ग;
        }
    }
  fwrite (ओळ_buf.buffer, परिमाण (वर्ण), ओळ_buf.length, stdout);
}

/* Return the type of line in 'line_buf'. */

स्थिर गणना section
check_section (शून्य)
{
  size_t len = ओळ_buf.length - 1;

  यदि (len < section_del_len || footer_del_len < section_del_len
      || !memeq (ओळ_buf.buffer, section_del, section_del_len))
    प्रत्यागम Text;
  यदि (len == header_del_len
      && memeq (ओळ_buf.buffer, header_del, header_del_len))
    प्रत्यागम Header;
  यदि (len == body_del_len
      && memeq (ओळ_buf.buffer, body_del, body_del_len))
    प्रत्यागम Body;
  यदि (len == footer_del_len
      && memeq (ओळ_buf.buffer, footer_del, footer_del_len))
    प्रत्यागम Footer;
  प्रत्यागम Text;
}

/* Read and process the file pointed to by FP. */

स्थिर शून्य
process_file (FILE *fp)
{
  यावत् (readlinebuffer (&ओळ_buf, fp))
    {
      विकल्प (check_section ())
        {
        पक्ष Header:
          proc_header ();
          भङ्ग;
        पक्ष Body:
          proc_body ();
          भङ्ग;
        पक्ष Footer:
          proc_footer ();
          भङ्ग;
        पक्ष Text:
          proc_text ();
          भङ्ग;
        }

      यदि (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

स्थिर bool
nl_file (वर्ण ध्रुव *file)
{
  FILE *stream;

  यदि (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  अन्यथा
    {
      stream = fopen (file, "r");
      यदि (stream == रिक्त)
        {
          error (0, errno, "%s", quotef (file));
          प्रत्यागम false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  पूर्णाङ्क err = errno;
  यदि (!ferror (stream))
    err = 0;
  यदि (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF. */
  अन्यथा यदि (fclose (stream) != 0 && !err)
    err = errno;
  यदि (err)
    {
      error (0, err, "%s", quotef (file));
      प्रत्यागम false;
    }
  प्रत्यागम true;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
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
          भङ्ग;
        पक्ष 'b':
          यदि (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          भङ्ग;
        पक्ष 'f':
          यदि (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          भङ्ग;
        पक्ष 'v':
          starting_ओळ_संख्या = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          भङ्ग;
        पक्ष 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          भङ्ग;
        पक्ष 'p':
          reset_numbers = false;
          भङ्ग;
        पक्ष 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          भङ्ग;
        पक्ष 's':
          separator_str = optarg;
          भङ्ग;
        पक्ष 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, INT_MAX, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          भङ्ग;
        पक्ष 'n':
          यदि (streq (optarg, "ln"))
            lineno_format = FORMAT_डावा;
          अन्यथा यदि (streq (optarg, "rn"))
            lineno_format = FORMAT_उजवा_NOLZ;
          अन्यथा यदि (streq (optarg, "rz"))
            lineno_format = FORMAT_उजवा_LZ;
          अन्यथा
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          भङ्ग;
        पक्ष 'd':
          len = strlen (optarg);
          यदि (1 < MB_CUR_MAX)
            {
              वर्ण ध्रुव *p = optarg;
              वर्ण ध्रुव *lim = p + len;
              पूर्णाङ्क n_chars = 0;
              प्रति (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              यदि (n_chars == 1)
                memcpy (mempcpy (section_del, optarg, len),  ":", परिमाण ":");
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
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          ok = false;
          भङ्ग;
        }
    }

  यदि (!ok)
    usage (EXIT_FAILURE);

  /* Initialize the section delimiters.  */
  section_del_len = len = strlen (section_del);

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
  len = strlen (separator_str);
  print_no_ओळ_fmt = xmalloc (lineno_width + len + 1);
  memset (print_no_ओळ_fmt, ' ', lineno_width + len);
  print_no_ओळ_fmt[lineno_width + len] = '\0';

  ओळ_no = starting_ओळ_संख्या;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  यदि (optind == argc)
    ok = nl_file ("-");
  अन्यथा
    प्रति (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  यदि (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
