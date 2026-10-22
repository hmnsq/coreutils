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
#परिभाषय क्रमादेशः_NAME "nl"

#परिभाषय AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
स्थिर वर्ण ध्रुव FORMAT_दक्षिणः_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
स्थिर वर्ण ध्रुव FORMAT_दक्षिणः_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
स्थिर वर्ण ध्रुव FORMAT_वामः[] = "%-*jd%s";

/* Default section delimiter characters.  */
स्थिर वर्ण DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
गणना section
{
  शीर्षकम्_2, Body, Footer, Text
};

/* Format of body lines (-b).  */
स्थिर वर्ण ध्रुव *body_type = "t";

/* Format of header lines (-h).  */
स्थिर वर्ण ध्रुव *शीर्षकम्_type = "n";

/* Format of footer lines (-f).  */
स्थिर वर्ण ध्रुव *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
स्थिर वर्ण ध्रुव *current_type;

/* Regex for body lines to number (-bp).  */
स्थिर संरचना re_आदर्शः_बफर body_regex;

/* Regex for header lines to number (-hp).  */
स्थिर संरचना re_आदर्शः_बफर शीर्षकम्_regex;

/* Regex for footer lines to number (-fp).  */
स्थिर संरचना re_आदर्शः_बफर footer_regex;

/* Fastmaps for the above.  */
स्थिर वर्ण body_fastmap[UCHAR_MAX + 1];
स्थिर वर्ण शीर्षकम्_fastmap[UCHAR_MAX + 1];
स्थिर वर्ण footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
स्थिर संरचना re_आदर्शः_बफर *current_regex = रिक्त;

/* Separator string to print after line number (-s).  */
स्थिर वर्ण ध्रुव *separator_str = "\t";

/* Input section delimiter string (-d).  */
स्थिर वर्ण *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
स्थिर size_t section_del_len;

/* Header delimiter string.  */
स्थिर वर्ण *शीर्षकम्_del = रिक्त;

/* Header section delimiter length.  */
स्थिर size_t शीर्षकम्_del_len;

/* Body delimiter string.  */
स्थिर वर्ण *body_del = रिक्त;

/* Body section delimiter length.  */
स्थिर size_t body_del_len;

/* Footer delimiter string.  */
स्थिर वर्ण *footer_del = रिक्त;

/* Footer section delimiter length.  */
स्थिर size_t footer_del_len;

/* Input buffer.  */
स्थिर संरचना linebuffer पङ्क्तिः_buf;

/* printf format string for unnumbered lines.  */
स्थिर वर्ण *print_no_पङ्क्तिः_fmt = रिक्त;

/* Starting line number on each page (-v).  */
स्थिर intmax_t starting_पङ्क्तिः_सङ्ख्या = 1;

/* Line number increment (-i).  */
स्थिर intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
स्थिर bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
स्थिर intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
स्थिर पूर्णाङ्क lineno_विस्तारः = 6;

/* Line number format (-n).  */
स्थिर वर्ण ध्रुव *lineno_format = FORMAT_दक्षिणः_NOLZ;

/* Current print line number.  */
स्थिर intmax_t पङ्क्तिः_no;

/* Whether the current line number has incremented past limits.  */
स्थिर bool पङ्क्तिः_no_अतिप्रवाहः;

/* True if we have ever read standard input.  */
स्थिर bool have_read_stdin;

स्थिर संरचना option ध्रुव longopts[] =
{
  {"header-numbering", required_तर्कः, रिक्त, 'h'},
  {"body-numbering", required_तर्कः, रिक्त, 'b'},
  {"footer-numbering", required_तर्कः, रिक्त, 'f'},
  {"starting-line-number", required_तर्कः, रिक्त, 'v'},
  {"line-increment", required_तर्कः, रिक्त, 'i'},
  {"no-renumber", no_तर्कः, रिक्त, 'p'},
  {"join-blank-lines", required_तर्कः, रिक्त, 'l'},
  {"number-separator", required_तर्कः, रिक्त, 's'},
  {"number-width", required_तर्कः, रिक्त, 'w'},
  {"number-format", required_तर्कः, रिक्त, 'n'},
  {"section-delimiter", required_तर्कः, रिक्त, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Print a usage message and quit. */

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              क्रमादेशः_name);
      fputs (_("\
Write each FILE गन्तव्यम् standard output, with पङ्क्तिः numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE प्रति numbering body पङ्क्तयः\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC प्रति logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE प्रति numbering footer पङ्क्तयः\n\
"));
      oputs (_("\
  -h, --शीर्षकम्-numbering=STYLE    use STYLE प्रति numbering शीर्षकम् पङ्क्तयः\n\
"));
      oputs (_("\
  -i, --पङ्क्तिः-increment=सङ्ख्या_2     पङ्क्तिः सङ्ख्या increment at each पङ्क्तिः\n\
"));
      oputs (_("\
  -l, --join-blank-पङ्क्तयः=सङ्ख्या_2   गणः सम्बन्धि सङ्ख्या_2 रिक्तानि पङ्क्तयः counted as one\n\
"));
      oputs (_("\
  -n, --सङ्ख्या-format=FORMAT      निवेशय पङ्क्तिः numbers according गन्तव्यम् FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               कुरु not reset पङ्क्तिः numbers प्रति each section\n\
"));
      oputs (_("\
  -s, --सङ्ख्या-separator=STRING   सङ्कलय STRING after (possible) पङ्क्तिः सङ्ख्या\n\
"));
      oputs (_("\
  -v, --starting-पङ्क्तिः-सङ्ख्या=सङ्ख्या_2  first पङ्क्तिः सङ्ख्या प्रति each section\n\
"));
      oputs (_("\
  -w, --सङ्ख्या-विस्तारः=सङ्ख्या_2       use सङ्ख्या_2 columns प्रति पङ्क्तिः numbers\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Default options are: -bt -d'\\:' -fn -hn -i1 -l1 -n'rn' -s<TAB> -v1 -w6\n\
\n\
CC are two delimiter characters प्रयुक्तम् गन्तव्यम् construct logical page delimiters;\n\
a missing second character उपपादयति ':'.  As a GNU extension one can specify\n\
अधिकम् than two characters, and also specifying the रिक्तानि string (-d '')\n\
disables section matching.\n\
"), stdout);
      fputs (_("\
\n\
STYLE is one सम्बन्धि:\n\
\n\
  a      सङ्ख्या all पङ्क्तयः\n\
  t      सङ्ख्या only nonempty पङ्क्तयः\n\
  n      सङ्ख्या no पङ्क्तयः\n\
  pBRE   सङ्ख्या only पङ्क्तयः that contain a मेलनम् प्रति the basic regular\n\
         expression, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one सम्बन्धि:\n\
\n\
  ln     वामः justified, no leading zeros\n\
  rn     दक्षिणः justified, no leading zeros\n\
  rz     दक्षिणः justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

स्थिर bool
build_type_arg (वर्ण ध्रुव **typep,
                संरचना re_आदर्शः_बफर *regexp, वर्ण *fastmap)
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
      regexp->बफर = रिक्त;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = रिक्त;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_रिक्तानि_RANGES;
      errmsg = re_compile_आदर्शः (optarg, वाक्यदैर्घ्यम् (optarg), regexp);
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
  यदि (पङ्क्तिः_no_अतिप्रवाहः)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  प्रारूप_लिख (lineno_format, lineno_विस्तारः, पङ्क्तिः_no, separator_str);

  यदि (ckd_सङ्कलय (&पङ्क्तिः_no, पङ्क्तिः_no, page_incr))
    पङ्क्तिः_no_अतिप्रवाहः = true;
}

स्थिर शून्य
reset_lineno (शून्य)
{
  यदि (reset_numbers)
    {
      पङ्क्तिः_no = starting_पङ्क्तिः_सङ्ख्या;
      पङ्क्तिः_no_अतिप्रवाहः = false;
    }
}

/* Switch to a header section. */

स्थिर शून्य
proc_शीर्षकम् (शून्य)
{
  current_type = शीर्षकम्_type;
  current_regex = &शीर्षकम्_regex;
  reset_lineno ();
  अक्षरं_लिख ('\n');
}

/* Switch to a body section. */

स्थिर शून्य
proc_body (शून्य)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  अक्षरं_लिख ('\n');
}

/* Switch to a footer section. */

स्थिर शून्य
proc_footer (शून्य)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  अक्षरं_लिख ('\n');
}

/* Process a regular text line in 'line_buf'. */

स्थिर शून्य
proc_text (शून्य)
{
  स्थिर intmax_t blank_पङ्क्तयः = 0;	/* Consecutive blank lines so far. */

  विकल्प (*current_type)
    {
    पक्ष 'a':
      यदि (blank_join > 1)
        {
          यदि (1 < पङ्क्तिः_buf.दीर्घता || ++blank_पङ्क्तयः == blank_join)
            {
              print_lineno ();
              blank_पङ्क्तयः = 0;
            }
          अन्यथा
            fputs (print_no_पङ्क्तिः_fmt, stdout);
        }
      अन्यथा
        print_lineno ();
      भङ्ग;
    पक्ष 't':
      यदि (1 < पङ्क्तिः_buf.दीर्घता)
        print_lineno ();
      अन्यथा
        fputs (print_no_पङ्क्तिः_fmt, stdout);
      भङ्ग;
    पक्ष 'n':
      fputs (print_no_पङ्क्तिः_fmt, stdout);
      भङ्ग;
    पक्ष 'p':
      विकल्प (re_अन्वेषणम् (current_regex, पङ्क्तिः_buf.बफर, पङ्क्तिः_buf.दीर्घता - 1,
                         0, पङ्क्तिः_buf.दीर्घता - 1, रिक्त))
        {
        पक्ष -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        पक्ष -1:
          fputs (print_no_पङ्क्तिः_fmt, stdout);
          भङ्ग;

        स्वभाव:
          print_lineno ();
          भङ्ग;
        }
    }
  fwrite (पङ्क्तिः_buf.बफर, परिमाण (वर्ण), पङ्क्तिः_buf.दीर्घता, stdout);
}

/* Return the type of line in 'line_buf'. */

स्थिर गणना section
check_section (शून्य)
{
  size_t len = पङ्क्तिः_buf.दीर्घता - 1;

  यदि (len < section_del_len || footer_del_len < section_del_len
      || !memeq (पङ्क्तिः_buf.बफर, section_del, section_del_len))
    प्रत्यागम Text;
  यदि (len == शीर्षकम्_del_len
      && memeq (पङ्क्तिः_buf.बफर, शीर्षकम्_del, शीर्षकम्_del_len))
    प्रत्यागम शीर्षकम्_2;
  यदि (len == body_del_len
      && memeq (पङ्क्तिः_buf.बफर, body_del, body_del_len))
    प्रत्यागम Body;
  यदि (len == footer_del_len
      && memeq (पङ्क्तिः_buf.बफर, footer_del, footer_del_len))
    प्रत्यागम Footer;
  प्रत्यागम Text;
}

/* Read and process the file pointed to by FP. */

स्थिर शून्य
process_file (FILE *fp)
{
  यावत् (readlinebuffer (&पङ्क्तिः_buf, fp))
    {
      विकल्प (check_section ())
        {
        पक्ष शीर्षकम्_2:
          proc_शीर्षकम् ();
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
  FILE *प्रवाहः;

  यदि (streq (file, "-"))
    {
      have_read_stdin = true;
      प्रवाहः = stdin;
      assume (प्रवाहः);  /* Pacify GCC bug#109613.  */
    }
  अन्यथा
    {
      प्रवाहः = fopen (file, "r");
      यदि (प्रवाहः == रिक्त)
        {
          error (0, errno, "%s", quotef (file));
          प्रत्यागम false;
        }
    }

  fadvise (प्रवाहः, FADVISE_SEQUENTIAL);

  process_file (प्रवाहः);

  पूर्णाङ्क err = errno;
  यदि (!ferror (प्रवाहः))
    err = 0;
  यदि (streq (file, "-"))
    clearerr (प्रवाहः);		/* Also clear EOF. */
  अन्यथा यदि (fclose (प्रवाहः) != 0 && !err)
    err = errno;
  यदि (err)
    {
      error (0, err, "%s", quotef (file));
      प्रत्यागम false;
    }
  प्रत्यागम true;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
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
          यदि (! build_type_arg (&शीर्षकम्_type, &शीर्षकम्_regex, शीर्षकम्_fastmap))
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
          starting_पङ्क्तिः_सङ्ख्या = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
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
          lineno_विस्तारः = xnumtoimax (optarg, 10, 1, अधिकतम_पूर्णाङ्कः, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          भङ्ग;
        पक्ष 'n':
          यदि (streq (optarg, "ln"))
            lineno_format = FORMAT_वामः;
          अन्यथा यदि (streq (optarg, "rn"))
            lineno_format = FORMAT_दक्षिणः_NOLZ;
          अन्यथा यदि (streq (optarg, "rz"))
            lineno_format = FORMAT_दक्षिणः_LZ;
          अन्यथा
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          भङ्ग;
        पक्ष 'd':
          len = वाक्यदैर्घ्यम् (optarg);
          यदि (1 < MB_CUR_MAX)
            {
              वर्ण ध्रुव *p = optarg;
              वर्ण ध्रुव *lim = p + len;
              पूर्णाङ्क n_chars = 0;
              प्रति (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_अवलोकनम् (p, lim).len;
              यदि (n_chars == 1)
                स्मृतिं_प्रतिलिख (mempcpy (section_del, optarg, len),  ":", परिमाण ":");
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
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          ok = false;
          भङ्ग;
        }
    }

  यदि (!ok)
    usage (EXIT_FAILURE);

  /* Initialize the section delimiters.  */
  section_del_len = len = वाक्यदैर्घ्यम् (section_del);

  शीर्षकम्_del_len = len * 3;
  शीर्षकम्_del = xmalloc (शीर्षकम्_del_len + 1);
  stpcpy (stpcpy (stpcpy (शीर्षकम्_del, section_del), section_del), section_del);

  body_del_len = len * 2;
  body_del = शीर्षकम्_del + len;

  footer_del_len = len;
  footer_del = body_del + len;

  /* Initialize the input buffer.  */
  initbuffer (&पङ्क्तिः_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = वाक्यदैर्घ्यम् (separator_str);
  print_no_पङ्क्तिः_fmt = xmalloc (lineno_विस्तारः + len + 1);
  स्मृतिं_पूरय (print_no_पङ्क्तिः_fmt, ' ', lineno_विस्तारः + len);
  print_no_पङ्क्तिः_fmt[lineno_विस्तारः + len] = '\0';

  पङ्क्तिः_no = starting_पङ्क्तिः_सङ्ख्या;
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
