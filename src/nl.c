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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <getopt.h>

#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ <regex.h>

#অন্তর্ভাৱ "fadvise.h"
#অন্তর্ভাৱ "linebuffer.h"
#অন্তর্ভাৱ "mcel.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nl"

#define AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
স্থির ৱর্ণ ধ্রুৱ FORMAT_ডান_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
স্থির ৱর্ণ ধ্রুৱ FORMAT_ডান_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
স্থির ৱর্ণ ধ্রুৱ FORMAT_বাম[] = "%-*jd%s";

/* Default section delimiter characters.  */
স্থির ৱর্ণ DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
গণনা section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
স্থির ৱর্ণ ধ্রুৱ *body_type = "t";

/* Format of header lines (-h).  */
স্থির ৱর্ণ ধ্রুৱ *header_type = "n";

/* Format of footer lines (-f).  */
স্থির ৱর্ণ ধ্রুৱ *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
স্থির ৱর্ণ ধ্রুৱ *current_type;

/* Regex for body lines to number (-bp).  */
স্থির সংরচনা re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
স্থির সংরচনা re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
স্থির সংরচনা re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
স্থির ৱর্ণ body_fastmap[UCHAR_MAX + 1];
স্থির ৱর্ণ header_fastmap[UCHAR_MAX + 1];
স্থির ৱর্ণ footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
স্থির সংরচনা re_pattern_buffer *current_regex = রিক্ত;

/* Separator string to print after line number (-s).  */
স্থির ৱর্ণ ধ্রুৱ *separator_str = "\t";

/* Input section delimiter string (-d).  */
স্থির ৱর্ণ *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
স্থির size_t section_del_len;

/* Header delimiter string.  */
স্থির ৱর্ণ *header_del = রিক্ত;

/* Header section delimiter length.  */
স্থির size_t header_del_len;

/* Body delimiter string.  */
স্থির ৱর্ণ *body_del = রিক্ত;

/* Body section delimiter length.  */
স্থির size_t body_del_len;

/* Footer delimiter string.  */
স্থির ৱর্ণ *footer_del = রিক্ত;

/* Footer section delimiter length.  */
স্থির size_t footer_del_len;

/* Input buffer.  */
স্থির সংরচনা linebuffer সাৰি_buf;

/* printf format string for unnumbered lines.  */
স্থির ৱর্ণ *print_no_সাৰি_fmt = রিক্ত;

/* Starting line number on each page (-v).  */
স্থির intmax_t starting_সাৰি_সংখ্যা = 1;

/* Line number increment (-i).  */
স্থির intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
স্থির bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
স্থির intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
স্থির পূর্ণাঙ্ক lineno_width = 6;

/* Line number format (-n).  */
স্থির ৱর্ণ ধ্রুৱ *lineno_format = FORMAT_ডান_NOLZ;

/* Current print line number.  */
স্থির intmax_t সাৰি_no;

/* Whether the current line number has incremented past limits.  */
স্থির bool সাৰি_no_overflow;

/* True if we have ever read standard input.  */
স্থির bool have_read_stdin;

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {"header-numbering", required_argument, রিক্ত, 'h'},
  {"body-numbering", required_argument, রিক্ত, 'b'},
  {"footer-numbering", required_argument, রিক্ত, 'f'},
  {"starting-line-number", required_argument, রিক্ত, 'v'},
  {"line-increment", required_argument, রিক্ত, 'i'},
  {"no-renumber", no_argument, রিক্ত, 'p'},
  {"join-blank-lines", required_argument, রিক্ত, 'l'},
  {"number-separator", required_argument, রিক্ত, 's'},
  {"number-width", required_argument, রিক্ত, 'w'},
  {"number-format", required_argument, রিক্ত, 'n'},
  {"section-delimiter", required_argument, রিক্ত, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Print a usage message and quit. */

শূন্য
usage (পূর্ণাঙ্ক status)
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
Write each FILE to standard output, with সাৰি numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE বাবে numbering body সাৰিগুলি\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC বাবে logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE বাবে numbering footer সাৰিগুলি\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE বাবে numbering header সাৰিগুলি\n\
"));
      oputs (_("\
  -i, --সাৰি-increment=সংখ্যা_2     সাৰি সংখ্যা increment at each সাৰি\n\
"));
      oputs (_("\
  -l, --join-blank-সাৰিগুলি=সংখ্যা_2   group of সংখ্যা_2 empty সাৰিগুলি counted as one\n\
"));
      oputs (_("\
  -n, --সংখ্যা-format=FORMAT      ঢোকাও সাৰি numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               কৰা not reset সাৰি numbers বাবে each section\n\
"));
      oputs (_("\
  -s, --সংখ্যা-separator=STRING   add STRING after (possible) সাৰি সংখ্যা\n\
"));
      oputs (_("\
  -v, --starting-সাৰি-সংখ্যা=সংখ্যা_2  first সাৰি সংখ্যা বাবে each section\n\
"));
      oputs (_("\
  -w, --সংখ্যা-width=সংখ্যা_2       use সংখ্যা_2 columns বাবে সাৰি numbers\n\
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
  a      সংখ্যা all সাৰিগুলি\n\
  t      সংখ্যা only nonempty সাৰিগুলি\n\
  n      সংখ্যা no সাৰিগুলি\n\
  pBRE   সংখ্যা only সাৰিগুলি that contain a match বাবে the basic regular\n\
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
build_type_arg (ৱর্ণ ধ্রুৱ **typep,
                সংরচনা re_pattern_buffer *regexp, ৱর্ণ *fastmap)
{
  ৱর্ণ ধ্রুৱ *errmsg;
  bool rval = true;

  ৱিকল্প (*optarg)
    {
    পক্ষ 'a':
    পক্ষ 't':
    পক্ষ 'n':
      *typep = optarg;
      ভঙ্গ;
    পক্ষ 'p':
      *typep = optarg++;
      regexp->buffer = রিক্ত;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = রিক্ত;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, strlen (optarg), regexp);
      যদি (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      ভঙ্গ;
    স্ৱভাৱ:
      rval = false;
      ভঙ্গ;
    }
  ঘূৰাই rval;
}

/* Print the line number and separator; increment the line number. */

স্থির শূন্য
print_lineno (শূন্য)
{
  যদি (সাৰি_no_overflow)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  printf (lineno_format, lineno_width, সাৰি_no, separator_str);

  যদি (ckd_add (&সাৰি_no, সাৰি_no, page_incr))
    সাৰি_no_overflow = true;
}

স্থির শূন্য
reset_lineno (শূন্য)
{
  যদি (reset_numbers)
    {
      সাৰি_no = starting_সাৰি_সংখ্যা;
      সাৰি_no_overflow = false;
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
  স্থির intmax_t blank_সাৰিগুলি = 0;	/* Consecutive blank lines so far. */

  ৱিকল্প (*current_type)
    {
    পক্ষ 'a':
      যদি (blank_join > 1)
        {
          যদি (1 < সাৰি_buf.length || ++blank_সাৰিগুলি == blank_join)
            {
              print_lineno ();
              blank_সাৰিগুলি = 0;
            }
          অন্যথা
            fputs (print_no_সাৰি_fmt, stdout);
        }
      অন্যথা
        print_lineno ();
      ভঙ্গ;
    পক্ষ 't':
      যদি (1 < সাৰি_buf.length)
        print_lineno ();
      অন্যথা
        fputs (print_no_সাৰি_fmt, stdout);
      ভঙ্গ;
    পক্ষ 'n':
      fputs (print_no_সাৰি_fmt, stdout);
      ভঙ্গ;
    পক্ষ 'p':
      ৱিকল্প (re_অনুসন্ধান (current_regex, সাৰি_buf.buffer, সাৰি_buf.length - 1,
                         0, সাৰি_buf.length - 1, রিক্ত))
        {
        পক্ষ -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        পক্ষ -1:
          fputs (print_no_সাৰি_fmt, stdout);
          ভঙ্গ;

        স্ৱভাৱ:
          print_lineno ();
          ভঙ্গ;
        }
    }
  fwrite (সাৰি_buf.buffer, পরিমাণ (ৱর্ণ), সাৰি_buf.length, stdout);
}

/* Return the type of line in 'line_buf'. */

স্থির গণনা section
check_section (শূন্য)
{
  size_t len = সাৰি_buf.length - 1;

  যদি (len < section_del_len || footer_del_len < section_del_len
      || !memeq (সাৰি_buf.buffer, section_del, section_del_len))
    ঘূৰাই Text;
  যদি (len == header_del_len
      && memeq (সাৰি_buf.buffer, header_del, header_del_len))
    ঘূৰাই Header;
  যদি (len == body_del_len
      && memeq (সাৰি_buf.buffer, body_del, body_del_len))
    ঘূৰাই Body;
  যদি (len == footer_del_len
      && memeq (সাৰি_buf.buffer, footer_del, footer_del_len))
    ঘূৰাই Footer;
  ঘূৰাই Text;
}

/* Read and process the file pointed to by FP. */

স্থির শূন্য
process_file (FILE *fp)
{
  যেতিয়ালৈ (readlinebuffer (&সাৰি_buf, fp))
    {
      ৱিকল্প (check_section ())
        {
        পক্ষ Header:
          proc_header ();
          ভঙ্গ;
        পক্ষ Body:
          proc_body ();
          ভঙ্গ;
        পক্ষ Footer:
          proc_footer ();
          ভঙ্গ;
        পক্ষ Text:
          proc_text ();
          ভঙ্গ;
        }

      যদি (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

স্থির bool
nl_file (ৱর্ণ ধ্রুৱ *file)
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
      যদি (stream == রিক্ত)
        {
          error (0, errno, "%s", quotef (file));
          ঘূৰাই false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  পূর্ণাঙ্ক err = errno;
  যদি (!ferror (stream))
    err = 0;
  যদি (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF. */
  অন্যথা যদি (fclose (stream) != 0 && !err)
    err = errno;
  যদি (err)
    {
      error (0, err, "%s", quotef (file));
      ঘূৰাই false;
    }
  ঘূৰাই true;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  পূর্ণাঙ্ক c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           রিক্ত))
         != -1)
    {
      ৱিকল্প (c)
        {
        পক্ষ 'h':
          যদি (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ভঙ্গ;
        পক্ষ 'b':
          যদি (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ভঙ্গ;
        পক্ষ 'f':
          যদি (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ভঙ্গ;
        পক্ষ 'v':
          starting_সাৰি_সংখ্যা = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          ভঙ্গ;
        পক্ষ 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          ভঙ্গ;
        পক্ষ 'p':
          reset_numbers = false;
          ভঙ্গ;
        পক্ষ 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          ভঙ্গ;
        পক্ষ 's':
          separator_str = optarg;
          ভঙ্গ;
        পক্ষ 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, INT_MAX, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          ভঙ্গ;
        পক্ষ 'n':
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
          ভঙ্গ;
        পক্ষ 'd':
          len = strlen (optarg);
          যদি (1 < MB_CUR_MAX)
            {
              ৱর্ণ ধ্রুৱ *p = optarg;
              ৱর্ণ ধ্রুৱ *lim = p + len;
              পূর্ণাঙ্ক n_chars = 0;
              বাবে (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              যদি (n_chars == 1)
                memcpy (mempcpy (section_del, optarg, len),  ":", পরিমাণ ":");
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
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
          ok = false;
          ভঙ্গ;
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
  initbuffer (&সাৰি_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = strlen (separator_str);
  print_no_সাৰি_fmt = xmalloc (lineno_width + len + 1);
  memset (print_no_সাৰি_fmt, ' ', lineno_width + len);
  print_no_সাৰি_fmt[lineno_width + len] = '\0';

  সাৰি_no = starting_সাৰি_সংখ্যা;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  যদি (optind == argc)
    ok = nl_file ("-");
  অন্যথা
    বাবে (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  যদি (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ঘূৰাই ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
