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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <getopt.h>

#ضمن "system.h"

#ضمن <regex.h>

#ضمن "fadvise.h"
#ضمن "linebuffer.h"
#ضمن "mcel.h"
#ضمن "quote.h"
#ضمن "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nl"

#define AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
ساكن حرف ثابت FORMAT_ئوڭ_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
ساكن حرف ثابت FORMAT_ئوڭ_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
ساكن حرف ثابت FORMAT_سول[] = "%-*jd%s";

/* Default section delimiter characters.  */
ساكن حرف DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
تعداد section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
ساكن حرف ثابت *body_type = "t";

/* Format of header lines (-h).  */
ساكن حرف ثابت *header_type = "n";

/* Format of footer lines (-f).  */
ساكن حرف ثابت *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
ساكن حرف ثابت *current_type;

/* Regex for body lines to number (-bp).  */
ساكن بنية re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
ساكن بنية re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
ساكن بنية re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
ساكن حرف body_fastmap[UCHAR_MAX + 1];
ساكن حرف header_fastmap[UCHAR_MAX + 1];
ساكن حرف footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
ساكن بنية re_pattern_buffer *current_regex = عدم;

/* Separator string to print after line number (-s).  */
ساكن حرف ثابت *separator_str = "\t";

/* Input section delimiter string (-d).  */
ساكن حرف *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
ساكن size_t section_del_len;

/* Header delimiter string.  */
ساكن حرف *header_del = عدم;

/* Header section delimiter length.  */
ساكن size_t header_del_len;

/* Body delimiter string.  */
ساكن حرف *body_del = عدم;

/* Body section delimiter length.  */
ساكن size_t body_del_len;

/* Footer delimiter string.  */
ساكن حرف *footer_del = عدم;

/* Footer section delimiter length.  */
ساكن size_t footer_del_len;

/* Input buffer.  */
ساكن بنية linebuffer قۇر_buf;

/* printf format string for unnumbered lines.  */
ساكن حرف *print_no_قۇر_fmt = عدم;

/* Starting line number on each page (-v).  */
ساكن intmax_t starting_قۇر_سان = 1;

/* Line number increment (-i).  */
ساكن intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
ساكن bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
ساكن intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
ساكن صحيح lineno_width = 6;

/* Line number format (-n).  */
ساكن حرف ثابت *lineno_format = FORMAT_ئوڭ_NOLZ;

/* Current print line number.  */
ساكن intmax_t قۇر_no;

/* Whether the current line number has incremented past limits.  */
ساكن bool قۇر_no_overflow;

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin;

ساكن بنية option ثابت longopts[] =
{
  {"header-numbering", required_argument, عدم, 'h'},
  {"body-numbering", required_argument, عدم, 'b'},
  {"footer-numbering", required_argument, عدم, 'f'},
  {"starting-line-number", required_argument, عدم, 'v'},
  {"line-increment", required_argument, عدم, 'i'},
  {"no-renumber", no_argument, عدم, 'p'},
  {"join-blank-lines", required_argument, عدم, 'l'},
  {"number-separator", required_argument, عدم, 's'},
  {"number-width", required_argument, عدم, 'w'},
  {"number-format", required_argument, عدم, 'n'},
  {"section-delimiter", required_argument, عدم, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Print a usage message and quit. */

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, with قۇر numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE لكل numbering body قۇرلار\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC لكل logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE لكل numbering footer قۇرلار\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE لكل numbering header قۇرلار\n\
"));
      oputs (_("\
  -i, --قۇر-increment=سان_2     قۇر سان increment at each قۇر\n\
"));
      oputs (_("\
  -l, --join-blank-قۇرلار=سان_2   group of سان_2 empty قۇرلار counted as one\n\
"));
      oputs (_("\
  -n, --سان-format=FORMAT      قىستۇرۇش قۇر numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               افعل not reset قۇر numbers لكل each section\n\
"));
      oputs (_("\
  -s, --سان-separator=STRING   add STRING after (possible) قۇر سان\n\
"));
      oputs (_("\
  -v, --starting-قۇر-سان=سان_2  first قۇر سان لكل each section\n\
"));
      oputs (_("\
  -w, --سان-width=سان_2       use سان_2 columns لكل قۇر numbers\n\
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
  a      سان all قۇرلار\n\
  t      سان only nonempty قۇرلار\n\
  n      سان no قۇرلار\n\
  pBRE   سان only قۇرلار that contain a match لكل the basic regular\n\
         expression, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     سول justified, no leading zeros\n\
  rn     ئوڭ justified, no leading zeros\n\
  rz     ئوڭ justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

ساكن bool
build_type_arg (حرف ثابت **typep,
                بنية re_pattern_buffer *regexp, حرف *fastmap)
{
  حرف ثابت *errmsg;
  bool rval = true;

  بدل (*optarg)
    {
    حالة 'a':
    حالة 't':
    حالة 'n':
      *typep = optarg;
      اكسر;
    حالة 'p':
      *typep = optarg++;
      regexp->buffer = عدم;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = عدم;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, strlen (optarg), regexp);
      إذا (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      اكسر;
    افتراضي:
      rval = false;
      اكسر;
    }
  أرجع rval;
}

/* Print the line number and separator; increment the line number. */

ساكن فراغ
print_lineno (فراغ)
{
  إذا (قۇر_no_overflow)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  printf (lineno_format, lineno_width, قۇر_no, separator_str);

  إذا (ckd_add (&قۇر_no, قۇر_no, page_incr))
    قۇر_no_overflow = true;
}

ساكن فراغ
reset_lineno (فراغ)
{
  إذا (reset_numbers)
    {
      قۇر_no = starting_قۇر_سان;
      قۇر_no_overflow = false;
    }
}

/* Switch to a header section. */

ساكن فراغ
proc_header (فراغ)
{
  current_type = header_type;
  current_regex = &header_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a body section. */

ساكن فراغ
proc_body (فراغ)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a footer section. */

ساكن فراغ
proc_footer (فراغ)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Process a regular text line in 'line_buf'. */

ساكن فراغ
proc_text (فراغ)
{
  ساكن intmax_t blank_قۇرلار = 0;	/* Consecutive blank lines so far. */

  بدل (*current_type)
    {
    حالة 'a':
      إذا (blank_join > 1)
        {
          إذا (1 < قۇر_buf.length || ++blank_قۇرلار == blank_join)
            {
              print_lineno ();
              blank_قۇرلار = 0;
            }
          وإلا
            fputs (print_no_قۇر_fmt, stdout);
        }
      وإلا
        print_lineno ();
      اكسر;
    حالة 't':
      إذا (1 < قۇر_buf.length)
        print_lineno ();
      وإلا
        fputs (print_no_قۇر_fmt, stdout);
      اكسر;
    حالة 'n':
      fputs (print_no_قۇر_fmt, stdout);
      اكسر;
    حالة 'p':
      بدل (re_ئىزدەش (current_regex, قۇر_buf.buffer, قۇر_buf.length - 1,
                         0, قۇر_buf.length - 1, عدم))
        {
        حالة -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        حالة -1:
          fputs (print_no_قۇر_fmt, stdout);
          اكسر;

        افتراضي:
          print_lineno ();
          اكسر;
        }
    }
  fwrite (قۇر_buf.buffer, حجم (حرف), قۇر_buf.length, stdout);
}

/* Return the type of line in 'line_buf'. */

ساكن تعداد section
check_section (فراغ)
{
  size_t len = قۇر_buf.length - 1;

  إذا (len < section_del_len || footer_del_len < section_del_len
      || !memeq (قۇر_buf.buffer, section_del, section_del_len))
    أرجع Text;
  إذا (len == header_del_len
      && memeq (قۇر_buf.buffer, header_del, header_del_len))
    أرجع Header;
  إذا (len == body_del_len
      && memeq (قۇر_buf.buffer, body_del, body_del_len))
    أرجع Body;
  إذا (len == footer_del_len
      && memeq (قۇر_buf.buffer, footer_del, footer_del_len))
    أرجع Footer;
  أرجع Text;
}

/* Read and process the file pointed to by FP. */

ساكن فراغ
process_file (FILE *fp)
{
  طالما (readlinebuffer (&قۇر_buf, fp))
    {
      بدل (check_section ())
        {
        حالة Header:
          proc_header ();
          اكسر;
        حالة Body:
          proc_body ();
          اكسر;
        حالة Footer:
          proc_footer ();
          اكسر;
        حالة Text:
          proc_text ();
          اكسر;
        }

      إذا (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

ساكن bool
nl_file (حرف ثابت *file)
{
  FILE *stream;

  إذا (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  وإلا
    {
      stream = fopen (file, "r");
      إذا (stream == عدم)
        {
          error (0, errno, "%s", quotef (file));
          أرجع false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  صحيح err = errno;
  إذا (!ferror (stream))
    err = 0;
  إذا (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF. */
  وإلا إذا (fclose (stream) != 0 && !err)
    err = errno;
  إذا (err)
    {
      error (0, err, "%s", quotef (file));
      أرجع false;
    }
  أرجع true;
}

صحيح
main (صحيح argc, حرف **argv)
{
  صحيح c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           عدم))
         != -1)
    {
      بدل (c)
        {
        حالة 'h':
          إذا (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          اكسر;
        حالة 'b':
          إذا (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          اكسر;
        حالة 'f':
          إذا (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          اكسر;
        حالة 'v':
          starting_قۇر_سان = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          اكسر;
        حالة 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          اكسر;
        حالة 'p':
          reset_numbers = false;
          اكسر;
        حالة 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          اكسر;
        حالة 's':
          separator_str = optarg;
          اكسر;
        حالة 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, INT_MAX, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          اكسر;
        حالة 'n':
          إذا (streq (optarg, "ln"))
            lineno_format = FORMAT_سول;
          وإلا إذا (streq (optarg, "rn"))
            lineno_format = FORMAT_ئوڭ_NOLZ;
          وإلا إذا (streq (optarg, "rz"))
            lineno_format = FORMAT_ئوڭ_LZ;
          وإلا
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          اكسر;
        حالة 'd':
          len = strlen (optarg);
          إذا (1 < MB_CUR_MAX)
            {
              حرف ثابت *p = optarg;
              حرف ثابت *lim = p + len;
              صحيح n_chars = 0;
              لكل (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              إذا (n_chars == 1)
                memcpy (mempcpy (section_del, optarg, len),  ":", حجم ":");
              وإلا
                section_del = optarg;
            }
          وإلا
            {
              إذا (len == 1)
                *section_del = *optarg;
              وإلا
                section_del = optarg;
            }
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          ok = false;
          اكسر;
        }
    }

  إذا (!ok)
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
  initbuffer (&قۇر_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = strlen (separator_str);
  print_no_قۇر_fmt = xmalloc (lineno_width + len + 1);
  memset (print_no_قۇر_fmt, ' ', lineno_width + len);
  print_no_قۇر_fmt[lineno_width + len] = '\0';

  قۇر_no = starting_قۇر_سان;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  إذا (optind == argc)
    ok = nl_file ("-");
  وإلا
    لكل (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  إذا (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
