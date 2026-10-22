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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <getopt.h>

#অন্তর্ভাব "system.h"

#অন্তর্ভাব <regex.h>

#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "linebuffer.h"
#অন্তর্ভাব "mcel.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nl"

#define AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
স্থির অক্ষর ধ্রুবক FORMAT_ডান_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
স্থির অক্ষর ধ্রুবক FORMAT_ডান_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
স্থির অক্ষর ধ্রুবক FORMAT_বাম[] = "%-*jd%s";

/* Default section delimiter characters.  */
স্থির অক্ষর DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
গণনা section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
স্থির অক্ষর ধ্রুবক *body_type = "t";

/* Format of header lines (-h).  */
স্থির অক্ষর ধ্রুবক *header_type = "n";

/* Format of footer lines (-f).  */
স্থির অক্ষর ধ্রুবক *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
স্থির অক্ষর ধ্রুবক *current_type;

/* Regex for body lines to number (-bp).  */
স্থির কাঠামো re_pattern_বাফার body_regex;

/* Regex for header lines to number (-hp).  */
স্থির কাঠামো re_pattern_বাফার header_regex;

/* Regex for footer lines to number (-fp).  */
স্থির কাঠামো re_pattern_বাফার footer_regex;

/* Fastmaps for the above.  */
স্থির অক্ষর body_fastmap[UCHAR_MAX + 1];
স্থির অক্ষর header_fastmap[UCHAR_MAX + 1];
স্থির অক্ষর footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
স্থির কাঠামো re_pattern_বাফার *current_regex = ফাঁকা;

/* Separator string to print after line number (-s).  */
স্থির অক্ষর ধ্রুবক *separator_str = "\t";

/* Input section delimiter string (-d).  */
স্থির অক্ষর *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
স্থির size_t section_del_len;

/* Header delimiter string.  */
স্থির অক্ষর *header_del = ফাঁকা;

/* Header section delimiter length.  */
স্থির size_t header_del_len;

/* Body delimiter string.  */
স্থির অক্ষর *body_del = ফাঁকা;

/* Body section delimiter length.  */
স্থির size_t body_del_len;

/* Footer delimiter string.  */
স্থির অক্ষর *footer_del = ফাঁকা;

/* Footer section delimiter length.  */
স্থির size_t footer_del_len;

/* Input buffer.  */
স্থির কাঠামো linebuffer সারি_buf;

/* printf format string for unnumbered lines.  */
স্থির অক্ষর *print_no_সারি_fmt = ফাঁকা;

/* Starting line number on each page (-v).  */
স্থির intmax_t starting_সারি_সংখ্যা = 1;

/* Line number increment (-i).  */
স্থির intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
স্থির bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
স্থির intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
স্থির পূর্ণসংখ্যা lineno_width = 6;

/* Line number format (-n).  */
স্থির অক্ষর ধ্রুবক *lineno_format = FORMAT_ডান_NOLZ;

/* Current print line number.  */
স্থির intmax_t সারি_no;

/* Whether the current line number has incremented past limits.  */
স্থির bool সারি_no_overflow;

/* True if we have ever read standard input.  */
স্থির bool have_read_stdin;

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"header-numbering", required_argument, ফাঁকা, 'h'},
  {"body-numbering", required_argument, ফাঁকা, 'b'},
  {"footer-numbering", required_argument, ফাঁকা, 'f'},
  {"starting-line-number", required_argument, ফাঁকা, 'v'},
  {"line-increment", required_argument, ফাঁকা, 'i'},
  {"no-renumber", no_argument, ফাঁকা, 'p'},
  {"join-blank-lines", required_argument, ফাঁকা, 'l'},
  {"number-separator", required_argument, ফাঁকা, 's'},
  {"number-width", required_argument, ফাঁকা, 'w'},
  {"number-format", required_argument, ফাঁকা, 'n'},
  {"section-delimiter", required_argument, ফাঁকা, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

/* Print a usage message and quit. */

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, with সারি numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE জন্য numbering body সারিগুলি\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC জন্য logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE জন্য numbering footer সারিগুলি\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE জন্য numbering header সারিগুলি\n\
"));
      oputs (_("\
  -i, --সারি-increment=সংখ্যা_2     সারি সংখ্যা increment at each সারি\n\
"));
      oputs (_("\
  -l, --join-blank-সারিগুলি=সংখ্যা_2   group of সংখ্যা_2 empty সারিগুলি counted as one\n\
"));
      oputs (_("\
  -n, --সংখ্যা-format=FORMAT      ঢোকাও সারি numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               করো not reset সারি numbers জন্য each section\n\
"));
      oputs (_("\
  -s, --সংখ্যা-separator=STRING   add STRING after (possible) সারি সংখ্যা\n\
"));
      oputs (_("\
  -v, --starting-সারি-সংখ্যা=সংখ্যা_2  first সারি সংখ্যা জন্য each section\n\
"));
      oputs (_("\
  -w, --সংখ্যা-width=সংখ্যা_2       use সংখ্যা_2 columns জন্য সারি numbers\n\
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
  a      সংখ্যা all সারিগুলি\n\
  t      সংখ্যা only nonempty সারিগুলি\n\
  n      সংখ্যা no সারিগুলি\n\
  pBRE   সংখ্যা only সারিগুলি that contain a match জন্য the basic regular\n\
         expression, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     বাম justified, no leading zeros\n\
  rn     ডান justified, no leading zeros\n\
  rz     ডান justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

স্থির bool
build_type_arg (অক্ষর ধ্রুবক **typep,
                কাঠামো re_pattern_বাফার *regexp, অক্ষর *fastmap)
{
  অক্ষর ধ্রুবক *errmsg;
  bool rval = true;

  নির্বাচন (*optarg)
    {
    ক্ষেত্র 'a':
    ক্ষেত্র 't':
    ক্ষেত্র 'n':
      *typep = optarg;
      বিরতি;
    ক্ষেত্র 'p':
      *typep = optarg++;
      regexp->বাফার = ফাঁকা;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = ফাঁকা;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, strlen (optarg), regexp);
      যদি (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      বিরতি;
    পূর্বনির্ধারিত:
      rval = false;
      বিরতি;
    }
  ফেরত rval;
}

/* Print the line number and separator; increment the line number. */

স্থির শূন্য
print_lineno (শূন্য)
{
  যদি (সারি_no_overflow)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  printf (lineno_format, lineno_width, সারি_no, separator_str);

  যদি (ckd_add (&সারি_no, সারি_no, page_incr))
    সারি_no_overflow = true;
}

স্থির শূন্য
reset_lineno (শূন্য)
{
  যদি (reset_numbers)
    {
      সারি_no = starting_সারি_সংখ্যা;
      সারি_no_overflow = false;
    }
}

/* Switch to a header section. */

স্থির শূন্য
proc_header (শূন্য)
{
  current_type = header_type;
  current_regex = &header_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a body section. */

স্থির শূন্য
proc_body (শূন্য)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a footer section. */

স্থির শূন্য
proc_footer (শূন্য)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Process a regular text line in 'line_buf'. */

স্থির শূন্য
proc_text (শূন্য)
{
  স্থির intmax_t blank_সারিগুলি = 0;	/* Consecutive blank lines so far. */

  নির্বাচন (*current_type)
    {
    ক্ষেত্র 'a':
      যদি (blank_join > 1)
        {
          যদি (1 < সারি_buf.দৈর্ঘ্য || ++blank_সারিগুলি == blank_join)
            {
              print_lineno ();
              blank_সারিগুলি = 0;
            }
          অন্যথা
            fputs (print_no_সারি_fmt, stdout);
        }
      অন্যথা
        print_lineno ();
      বিরতি;
    ক্ষেত্র 't':
      যদি (1 < সারি_buf.দৈর্ঘ্য)
        print_lineno ();
      অন্যথা
        fputs (print_no_সারি_fmt, stdout);
      বিরতি;
    ক্ষেত্র 'n':
      fputs (print_no_সারি_fmt, stdout);
      বিরতি;
    ক্ষেত্র 'p':
      নির্বাচন (re_অনুসন্ধান (current_regex, সারি_buf.বাফার, সারি_buf.দৈর্ঘ্য - 1,
                         0, সারি_buf.দৈর্ঘ্য - 1, ফাঁকা))
        {
        ক্ষেত্র -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        ক্ষেত্র -1:
          fputs (print_no_সারি_fmt, stdout);
          বিরতি;

        পূর্বনির্ধারিত:
          print_lineno ();
          বিরতি;
        }
    }
  fwrite (সারি_buf.বাফার, আকার (অক্ষর), সারি_buf.দৈর্ঘ্য, stdout);
}

/* Return the type of line in 'line_buf'. */

স্থির গণনা section
check_section (শূন্য)
{
  size_t len = সারি_buf.দৈর্ঘ্য - 1;

  যদি (len < section_del_len || footer_del_len < section_del_len
      || !memeq (সারি_buf.বাফার, section_del, section_del_len))
    ফেরত Text;
  যদি (len == header_del_len
      && memeq (সারি_buf.বাফার, header_del, header_del_len))
    ফেরত Header;
  যদি (len == body_del_len
      && memeq (সারি_buf.বাফার, body_del, body_del_len))
    ফেরত Body;
  যদি (len == footer_del_len
      && memeq (সারি_buf.বাফার, footer_del, footer_del_len))
    ফেরত Footer;
  ফেরত Text;
}

/* Read and process the file pointed to by FP. */

স্থির শূন্য
process_file (FILE *fp)
{
  যখন (readlinebuffer (&সারি_buf, fp))
    {
      নির্বাচন (check_section ())
        {
        ক্ষেত্র Header:
          proc_header ();
          বিরতি;
        ক্ষেত্র Body:
          proc_body ();
          বিরতি;
        ক্ষেত্র Footer:
          proc_footer ();
          বিরতি;
        ক্ষেত্র Text:
          proc_text ();
          বিরতি;
        }

      যদি (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

স্থির bool
nl_file (অক্ষর ধ্রুবক *file)
{
  FILE *stream;

  যদি (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  অন্যথা
    {
      stream = fopen (file, "r");
      যদি (stream == ফাঁকা)
        {
          error (0, errno, "%s", quotef (file));
          ফেরত false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  পূর্ণসংখ্যা err = errno;
  যদি (!ferror (stream))
    err = 0;
  যদি (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF. */
  অন্যথা যদি (fclose (stream) != 0 && !err)
    err = errno;
  যদি (err)
    {
      error (0, err, "%s", quotef (file));
      ফেরত false;
    }
  ফেরত true;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           ফাঁকা))
         != -1)
    {
      নির্বাচন (c)
        {
        ক্ষেত্র 'h':
          যদি (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          বিরতি;
        ক্ষেত্র 'b':
          যদি (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          বিরতি;
        ক্ষেত্র 'f':
          যদি (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          বিরতি;
        ক্ষেত্র 'v':
          starting_সারি_সংখ্যা = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          বিরতি;
        ক্ষেত্র 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          বিরতি;
        ক্ষেত্র 'p':
          reset_numbers = false;
          বিরতি;
        ক্ষেত্র 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          বিরতি;
        ক্ষেত্র 's':
          separator_str = optarg;
          বিরতি;
        ক্ষেত্র 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, INT_MAX, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          বিরতি;
        ক্ষেত্র 'n':
          যদি (streq (optarg, "ln"))
            lineno_format = FORMAT_বাম;
          অন্যথা যদি (streq (optarg, "rn"))
            lineno_format = FORMAT_ডান_NOLZ;
          অন্যথা যদি (streq (optarg, "rz"))
            lineno_format = FORMAT_ডান_LZ;
          অন্যথা
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          বিরতি;
        ক্ষেত্র 'd':
          len = strlen (optarg);
          যদি (1 < MB_CUR_MAX)
            {
              অক্ষর ধ্রুবক *p = optarg;
              অক্ষর ধ্রুবক *lim = p + len;
              পূর্ণসংখ্যা n_chars = 0;
              জন্য (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              যদি (n_chars == 1)
                memcpy (mempcpy (section_del, optarg, len),  ":", আকার ":");
              অন্যথা
                section_del = optarg;
            }
          অন্যথা
            {
              যদি (len == 1)
                *section_del = *optarg;
              অন্যথা
                section_del = optarg;
            }
          বিরতি;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        পূর্বনির্ধারিত:
          ok = false;
          বিরতি;
        }
    }

  যদি (!ok)
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
  initbuffer (&সারি_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = strlen (separator_str);
  print_no_সারি_fmt = xmalloc (lineno_width + len + 1);
  memset (print_no_সারি_fmt, ' ', lineno_width + len);
  print_no_সারি_fmt[lineno_width + len] = '\0';

  সারি_no = starting_সারি_সংখ্যা;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  যদি (optind == argc)
    ok = nl_file ("-");
  অন্যথা
    জন্য (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  যদি (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
