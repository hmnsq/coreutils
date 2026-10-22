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

#شامل <config.h>

#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <getopt.h>

#شامل "system.h"

#شامل <regex.h>

#شامل "fadvise.h"
#شامل "linebuffer.h"
#شامل "mcel.h"
#شامل "quote.h"
#شامل "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nl"

#define AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
ایستا نویسه ثابت FORMAT_راست_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
ایستا نویسه ثابت FORMAT_راست_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
ایستا نویسه ثابت FORMAT_چپ[] = "%-*jd%s";

/* Default section delimiter characters.  */
ایستا نویسه DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
شمارش section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
ایستا نویسه ثابت *body_type = "t";

/* Format of header lines (-h).  */
ایستا نویسه ثابت *header_type = "n";

/* Format of footer lines (-f).  */
ایستا نویسه ثابت *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
ایستا نویسه ثابت *current_type;

/* Regex for body lines to number (-bp).  */
ایستا ساختار re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
ایستا ساختار re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
ایستا ساختار re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
ایستا نویسه body_fastmap[UCHAR_MAX + 1];
ایستا نویسه header_fastmap[UCHAR_MAX + 1];
ایستا نویسه footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
ایستا ساختار re_pattern_buffer *current_regex = پوچ;

/* Separator string to print after line number (-s).  */
ایستا نویسه ثابت *separator_str = "\t";

/* Input section delimiter string (-d).  */
ایستا نویسه *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
ایستا size_t section_del_len;

/* Header delimiter string.  */
ایستا نویسه *header_del = پوچ;

/* Header section delimiter length.  */
ایستا size_t header_del_len;

/* Body delimiter string.  */
ایستا نویسه *body_del = پوچ;

/* Body section delimiter length.  */
ایستا size_t body_del_len;

/* Footer delimiter string.  */
ایستا نویسه *footer_del = پوچ;

/* Footer section delimiter length.  */
ایستا size_t footer_del_len;

/* Input buffer.  */
ایستا ساختار linebuffer سطر_buf;

/* printf format string for unnumbered lines.  */
ایستا نویسه *print_no_سطر_fmt = پوچ;

/* Starting line number on each page (-v).  */
ایستا intmax_t starting_سطر_عدد = 1;

/* Line number increment (-i).  */
ایستا intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
ایستا bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
ایستا intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
ایستا صحیح lineno_width = 6;

/* Line number format (-n).  */
ایستا نویسه ثابت *lineno_format = FORMAT_راست_NOLZ;

/* Current print line number.  */
ایستا intmax_t سطر_no;

/* Whether the current line number has incremented past limits.  */
ایستا bool سطر_no_overflow;

/* True if we have ever read standard input.  */
ایستا bool have_read_stdin;

ایستا ساختار option ثابت longopts[] =
{
  {"header-numbering", required_argument, پوچ, 'h'},
  {"body-numbering", required_argument, پوچ, 'b'},
  {"footer-numbering", required_argument, پوچ, 'f'},
  {"starting-line-number", required_argument, پوچ, 'v'},
  {"line-increment", required_argument, پوچ, 'i'},
  {"no-renumber", no_argument, پوچ, 'p'},
  {"join-blank-lines", required_argument, پوچ, 'l'},
  {"number-separator", required_argument, پوچ, 's'},
  {"number-width", required_argument, پوچ, 'w'},
  {"number-format", required_argument, پوچ, 'n'},
  {"section-delimiter", required_argument, پوچ, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Print a usage message and quit. */

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, with سطر numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE برای numbering body سطرها\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC برای logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE برای numbering footer سطرها\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE برای numbering header سطرها\n\
"));
      oputs (_("\
  -i, --سطر-increment=عدد_2     سطر عدد increment at each سطر\n\
"));
      oputs (_("\
  -l, --join-blank-سطرها=عدد_2   group of عدد_2 empty سطرها counted as one\n\
"));
      oputs (_("\
  -n, --عدد-format=FORMAT      درج سطر numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               انجامبده not reset سطر numbers برای each section\n\
"));
      oputs (_("\
  -s, --عدد-separator=STRING   add STRING after (possible) سطر عدد\n\
"));
      oputs (_("\
  -v, --starting-سطر-عدد=عدد_2  first سطر عدد برای each section\n\
"));
      oputs (_("\
  -w, --عدد-width=عدد_2       use عدد_2 columns برای سطر numbers\n\
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
  a      عدد all سطرها\n\
  t      عدد only nonempty سطرها\n\
  n      عدد no سطرها\n\
  pBRE   عدد only سطرها that contain a match برای the basic regular\n\
         expression, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     چپ justified, no leading zeros\n\
  rn     راست justified, no leading zeros\n\
  rz     راست justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

ایستا bool
build_type_arg (نویسه ثابت **typep,
                ساختار re_pattern_buffer *regexp, نویسه *fastmap)
{
  نویسه ثابت *errmsg;
  bool rval = true;

  انتخاب (*optarg)
    {
    حالت 'a':
    حالت 't':
    حالت 'n':
      *typep = optarg;
      بشکن;
    حالت 'p':
      *typep = optarg++;
      regexp->buffer = پوچ;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = پوچ;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, strlen (optarg), regexp);
      اگر (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      بشکن;
    پیشفرض:
      rval = false;
      بشکن;
    }
  بازگشت rval;
}

/* Print the line number and separator; increment the line number. */

ایستا تهی
print_lineno (تهی)
{
  اگر (سطر_no_overflow)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  printf (lineno_format, lineno_width, سطر_no, separator_str);

  اگر (ckd_add (&سطر_no, سطر_no, page_incr))
    سطر_no_overflow = true;
}

ایستا تهی
reset_lineno (تهی)
{
  اگر (reset_numbers)
    {
      سطر_no = starting_سطر_عدد;
      سطر_no_overflow = false;
    }
}

/* Switch to a header section. */

ایستا تهی
proc_header (تهی)
{
  current_type = header_type;
  current_regex = &header_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a body section. */

ایستا تهی
proc_body (تهی)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a footer section. */

ایستا تهی
proc_footer (تهی)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Process a regular text line in 'line_buf'. */

ایستا تهی
proc_text (تهی)
{
  ایستا intmax_t blank_سطرها = 0;	/* Consecutive blank lines so far. */

  انتخاب (*current_type)
    {
    حالت 'a':
      اگر (blank_join > 1)
        {
          اگر (1 < سطر_buf.length || ++blank_سطرها == blank_join)
            {
              print_lineno ();
              blank_سطرها = 0;
            }
          وگرنه
            fputs (print_no_سطر_fmt, stdout);
        }
      وگرنه
        print_lineno ();
      بشکن;
    حالت 't':
      اگر (1 < سطر_buf.length)
        print_lineno ();
      وگرنه
        fputs (print_no_سطر_fmt, stdout);
      بشکن;
    حالت 'n':
      fputs (print_no_سطر_fmt, stdout);
      بشکن;
    حالت 'p':
      انتخاب (re_جستجو (current_regex, سطر_buf.buffer, سطر_buf.length - 1,
                         0, سطر_buf.length - 1, پوچ))
        {
        حالت -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        حالت -1:
          fputs (print_no_سطر_fmt, stdout);
          بشکن;

        پیشفرض:
          print_lineno ();
          بشکن;
        }
    }
  fwrite (سطر_buf.buffer, اندازه (نویسه), سطر_buf.length, stdout);
}

/* Return the type of line in 'line_buf'. */

ایستا شمارش section
check_section (تهی)
{
  size_t len = سطر_buf.length - 1;

  اگر (len < section_del_len || footer_del_len < section_del_len
      || !memeq (سطر_buf.buffer, section_del, section_del_len))
    بازگشت Text;
  اگر (len == header_del_len
      && memeq (سطر_buf.buffer, header_del, header_del_len))
    بازگشت Header;
  اگر (len == body_del_len
      && memeq (سطر_buf.buffer, body_del, body_del_len))
    بازگشت Body;
  اگر (len == footer_del_len
      && memeq (سطر_buf.buffer, footer_del, footer_del_len))
    بازگشت Footer;
  بازگشت Text;
}

/* Read and process the file pointed to by FP. */

ایستا تهی
process_file (FILE *fp)
{
  وقتی (readlinebuffer (&سطر_buf, fp))
    {
      انتخاب (check_section ())
        {
        حالت Header:
          proc_header ();
          بشکن;
        حالت Body:
          proc_body ();
          بشکن;
        حالت Footer:
          proc_footer ();
          بشکن;
        حالت Text:
          proc_text ();
          بشکن;
        }

      اگر (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

ایستا bool
nl_file (نویسه ثابت *file)
{
  FILE *stream;

  اگر (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  وگرنه
    {
      stream = fopen (file, "r");
      اگر (stream == پوچ)
        {
          error (0, errno, "%s", quotef (file));
          بازگشت false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  صحیح err = errno;
  اگر (!ferror (stream))
    err = 0;
  اگر (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF. */
  وگرنه اگر (fclose (stream) != 0 && !err)
    err = errno;
  اگر (err)
    {
      error (0, err, "%s", quotef (file));
      بازگشت false;
    }
  بازگشت true;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           پوچ))
         != -1)
    {
      انتخاب (c)
        {
        حالت 'h':
          اگر (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          بشکن;
        حالت 'b':
          اگر (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          بشکن;
        حالت 'f':
          اگر (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          بشکن;
        حالت 'v':
          starting_سطر_عدد = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          بشکن;
        حالت 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          بشکن;
        حالت 'p':
          reset_numbers = false;
          بشکن;
        حالت 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          بشکن;
        حالت 's':
          separator_str = optarg;
          بشکن;
        حالت 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, INT_MAX, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          بشکن;
        حالت 'n':
          اگر (streq (optarg, "ln"))
            lineno_format = FORMAT_چپ;
          وگرنه اگر (streq (optarg, "rn"))
            lineno_format = FORMAT_راست_NOLZ;
          وگرنه اگر (streq (optarg, "rz"))
            lineno_format = FORMAT_راست_LZ;
          وگرنه
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          بشکن;
        حالت 'd':
          len = strlen (optarg);
          اگر (1 < MB_CUR_MAX)
            {
              نویسه ثابت *p = optarg;
              نویسه ثابت *lim = p + len;
              صحیح n_chars = 0;
              برای (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              اگر (n_chars == 1)
                memcpy (mempcpy (section_del, optarg, len),  ":", اندازه ":");
              وگرنه
                section_del = optarg;
            }
          وگرنه
            {
              اگر (len == 1)
                *section_del = *optarg;
              وگرنه
                section_del = optarg;
            }
          بشکن;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          ok = false;
          بشکن;
        }
    }

  اگر (!ok)
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
  initbuffer (&سطر_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = strlen (separator_str);
  print_no_سطر_fmt = xmalloc (lineno_width + len + 1);
  memset (print_no_سطر_fmt, ' ', lineno_width + len);
  print_no_سطر_fmt[lineno_width + len] = '\0';

  سطر_no = starting_سطر_عدد;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  اگر (optind == argc)
    ok = nl_file ("-");
  وگرنه
    برای (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  اگر (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
