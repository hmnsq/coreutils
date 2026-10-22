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

#include <config.h>

#include <stdio.h>
#include <sys/types.h>
#include <getopt.h>

#include "system.h"

#include <regex.h>

#include "fadvise.h"
#include "linebuffer.h"
#include "mcel.h"
#include "quote.h"
#include "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nl"

#define AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
static var_na const FORMAT_RIGHT_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
static var_na const FORMAT_RIGHT_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
static var_na const FORMAT_LEFT[] = "%-*jd%s";

/* Default section delimiter characters.  */
static var_na DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
ga_nanaa section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
static var_na const *body_type = "t";

/* Format of header lines (-h).  */
static var_na const *header_type = "n";

/* Format of footer lines (-f).  */
static var_na const *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
static var_na const *current_type;

/* Regex for body lines to number (-bp).  */
static sa_nracanaa re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
static sa_nracanaa re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
static sa_nracanaa re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
static var_na body_fastmap[UCHAR_MAX + 1];
static var_na header_fastmap[UCHAR_MAX + 1];
static var_na footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
static sa_nracanaa re_pattern_buffer *current_regex = NULL;

/* Separator string to print after line number (-s).  */
static var_na const *separator_str = "\t";

/* Input section delimiter string (-d).  */
static var_na *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
static size_t section_del_len;

/* Header delimiter string.  */
static var_na *header_del = NULL;

/* Header section delimiter length.  */
static size_t header_del_len;

/* Body delimiter string.  */
static var_na *body_del = NULL;

/* Body section delimiter length.  */
static size_t body_del_len;

/* Footer delimiter string.  */
static var_na *footer_del = NULL;

/* Footer section delimiter length.  */
static size_t footer_del_len;

/* Input buffer.  */
static sa_nracanaa linebuffer line_buf;

/* printf format string for unnumbered lines.  */
static var_na *print_no_line_fmt = NULL;

/* Starting line number on each page (-v).  */
static intmax_t starting_line_number = 1;

/* Line number increment (-i).  */
static intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
static bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
static intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
static puur_naa_nka lineno_width = 6;

/* Line number format (-n).  */
static var_na const *lineno_format = FORMAT_RIGHT_NOLZ;

/* Current print line number.  */
static intmax_t line_no;

/* Whether the current line number has incremented past limits.  */
static bool line_no_overflow;

/* True if we have ever read standard input.  */
static bool have_read_stdin;

static sa_nracanaa option const longopts[] =
{
  {"header-numbering", required_argument, NULL, 'h'},
  {"body-numbering", required_argument, NULL, 'b'},
  {"footer-numbering", required_argument, NULL, 'f'},
  {"starting-line-number", required_argument, NULL, 'v'},
  {"line-increment", required_argument, NULL, 'i'},
  {"no-renumber", no_argument, NULL, 'p'},
  {"join-blank-lines", required_argument, NULL, 'l'},
  {"number-separator", required_argument, NULL, 's'},
  {"number-width", required_argument, NULL, 'w'},
  {"number-format", required_argument, NULL, 'n'},
  {"section-delimiter", required_argument, NULL, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Print a usage message and quit. */

void
usage (puur_naa_nka status)
{
  ᨀᨘ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᨀᨘᨁ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, with line numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE for numbering body lines\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC for logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE for numbering footer lines\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE for numbering header lines\n\
"));
      oputs (_("\
  -i, --line-increment=NUMBER     line number increment at each line\n\
"));
      oputs (_("\
  -l, --join-blank-lines=NUMBER   group of NUMBER empty lines counted as one\n\
"));
      oputs (_("\
  -n, --number-format=FORMAT      insert line numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               do not reset line numbers for each section\n\
"));
      oputs (_("\
  -s, --number-separator=STRING   add STRING after (possible) line number\n\
"));
      oputs (_("\
  -v, --starting-line-number=NUMBER  first line number for each section\n\
"));
      oputs (_("\
  -w, --number-width=NUMBER       use NUMBER columns for line numbers\n\
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
  a      number all lines\n\
  t      number only nonempty lines\n\
  n      number no lines\n\
  pBRE   number only lines that contain a match for the basic regular\n\
         expression, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     left justified, no leading zeros\n\
  rn     right justified, no leading zeros\n\
  rz     right justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

static bool
build_type_arg (var_na const **typep,
                sa_nracanaa re_pattern_buffer *regexp, var_na *fastmap)
{
  var_na const *errmsg;
  bool rval = true;

  switch (*optarg)
    {
    pak_sa 'a':
    pak_sa 't':
    pak_sa 'n':
      *typep = optarg;
      bha_nga;
    pak_sa 'p':
      *typep = optarg++;
      regexp->buffer = NULL;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = NULL;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, strlen (optarg), regexp);
      ᨀᨘ (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      bha_nga;
    default:
      rval = false;
      bha_nga;
    }
  ᨀᨘᨒ rval;
}

/* Print the line number and separator; increment the line number. */

static void
print_lineno (void)
{
  ᨀᨘ (line_no_overflow)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  printf (lineno_format, lineno_width, line_no, separator_str);

  ᨀᨘ (ckd_add (&line_no, line_no, page_incr))
    line_no_overflow = true;
}

static void
reset_lineno (void)
{
  ᨀᨘ (reset_numbers)
    {
      line_no = starting_line_number;
      line_no_overflow = false;
    }
}

/* Switch to a header section. */

static void
proc_header (void)
{
  current_type = header_type;
  current_regex = &header_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a body section. */

static void
proc_body (void)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a footer section. */

static void
proc_footer (void)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Process a regular text line in 'line_buf'. */

static void
proc_text (void)
{
  static intmax_t blank_lines = 0;	/* Consecutive blank lines so far. */

  switch (*current_type)
    {
    pak_sa 'a':
      ᨀᨘ (blank_join > 1)
        {
          ᨀᨘ (1 < line_buf.length || ++blank_lines == blank_join)
            {
              print_lineno ();
              blank_lines = 0;
            }
          ᨀᨘᨁ
            fputs (print_no_line_fmt, stdout);
        }
      ᨀᨘᨁ
        print_lineno ();
      bha_nga;
    pak_sa 't':
      ᨀᨘ (1 < line_buf.length)
        print_lineno ();
      ᨀᨘᨁ
        fputs (print_no_line_fmt, stdout);
      bha_nga;
    pak_sa 'n':
      fputs (print_no_line_fmt, stdout);
      bha_nga;
    pak_sa 'p':
      switch (re_search (current_regex, line_buf.buffer, line_buf.length - 1,
                         0, line_buf.length - 1, NULL))
        {
        pak_sa -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        pak_sa -1:
          fputs (print_no_line_fmt, stdout);
          bha_nga;

        default:
          print_lineno ();
          bha_nga;
        }
    }
  fwrite (line_buf.buffer, parimaa_na (var_na), line_buf.length, stdout);
}

/* Return the type of line in 'line_buf'. */

static ga_nanaa section
check_section (void)
{
  size_t len = line_buf.length - 1;

  ᨀᨘ (len < section_del_len || footer_del_len < section_del_len
      || !memeq (line_buf.buffer, section_del, section_del_len))
    ᨀᨘᨒ Text;
  ᨀᨘ (len == header_del_len
      && memeq (line_buf.buffer, header_del, header_del_len))
    ᨀᨘᨒ Header;
  ᨀᨘ (len == body_del_len
      && memeq (line_buf.buffer, body_del, body_del_len))
    ᨀᨘᨒ Body;
  ᨀᨘ (len == footer_del_len
      && memeq (line_buf.buffer, footer_del, footer_del_len))
    ᨀᨘᨒ Footer;
  ᨀᨘᨒ Text;
}

/* Read and process the file pointed to by FP. */

static void
process_file (FILE *fp)
{
  ᨀᨛ (readlinebuffer (&line_buf, fp))
    {
      switch (check_section ())
        {
        pak_sa Header:
          proc_header ();
          bha_nga;
        pak_sa Body:
          proc_body ();
          bha_nga;
        pak_sa Footer:
          proc_footer ();
          bha_nga;
        pak_sa Text:
          proc_text ();
          bha_nga;
        }

      ᨀᨘ (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

static bool
nl_file (var_na const *file)
{
  FILE *stream;

  ᨀᨘ (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  ᨀᨘᨁ
    {
      stream = fopen (file, "r");
      ᨀᨘ (stream == NULL)
        {
          error (0, errno, "%s", quotef (file));
          ᨀᨘᨒ false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  puur_naa_nka err = errno;
  ᨀᨘ (!ferror (stream))
    err = 0;
  ᨀᨘ (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF. */
  ᨀᨘᨁ ᨀᨘ (fclose (stream) != 0 && !err)
    err = errno;
  ᨀᨘ (err)
    {
      error (0, err, "%s", quotef (file));
      ᨀᨘᨒ false;
    }
  ᨀᨘᨒ true;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  puur_naa_nka c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ᨀᨛ ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           NULL))
         != -1)
    {
      switch (c)
        {
        pak_sa 'h':
          ᨀᨘ (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          bha_nga;
        pak_sa 'b':
          ᨀᨘ (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          bha_nga;
        pak_sa 'f':
          ᨀᨘ (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          bha_nga;
        pak_sa 'v':
          starting_line_number = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          bha_nga;
        pak_sa 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          bha_nga;
        pak_sa 'p':
          reset_numbers = false;
          bha_nga;
        pak_sa 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          bha_nga;
        pak_sa 's':
          separator_str = optarg;
          bha_nga;
        pak_sa 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, INT_MAX, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          bha_nga;
        pak_sa 'n':
          ᨀᨘ (streq (optarg, "ln"))
            lineno_format = FORMAT_LEFT;
          ᨀᨘᨁ ᨀᨘ (streq (optarg, "rn"))
            lineno_format = FORMAT_RIGHT_NOLZ;
          ᨀᨘᨁ ᨀᨘ (streq (optarg, "rz"))
            lineno_format = FORMAT_RIGHT_LZ;
          ᨀᨘᨁ
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          bha_nga;
        pak_sa 'd':
          len = strlen (optarg);
          ᨀᨘ (1 < MB_CUR_MAX)
            {
              var_na const *p = optarg;
              var_na const *lim = p + len;
              puur_naa_nka n_chars = 0;
              for (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              ᨀᨘ (n_chars == 1)
                memcpy (mempcpy (section_del, optarg, len),  ":", parimaa_na ":");
              ᨀᨘᨁ
                section_del = optarg;
            }
          ᨀᨘᨁ
            {
              ᨀᨘ (len == 1)
                *section_del = *optarg;
              ᨀᨘᨁ
                section_del = optarg;
            }
          bha_nga;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          ok = false;
          bha_nga;
        }
    }

  ᨀᨘ (!ok)
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
  initbuffer (&line_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = strlen (separator_str);
  print_no_line_fmt = xmalloc (lineno_width + len + 1);
  memset (print_no_line_fmt, ' ', lineno_width + len);
  print_no_line_fmt[lineno_width + len] = '\0';

  line_no = starting_line_number;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  ᨀᨘ (optind == argc)
    ok = nl_file ("-");
  ᨀᨘᨁ
    for (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  ᨀᨘ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ᨀᨘᨒ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
