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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <regex.h>

#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "linebuffer.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nl"

#define AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ FORMAT_ಬಲ_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ FORMAT_ಬಲ_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ FORMAT_ಎಡ[] = "%-*jd%s";

/* Default section delimiter characters.  */
ಸ್ಥಿರ ವರ್ಣ DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
ಗಣನಾ section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *body_type = "t";

/* Format of header lines (-h).  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *header_type = "n";

/* Format of footer lines (-f).  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *current_type;

/* Regex for body lines to number (-bp).  */
ಸ್ಥಿರ ಸಂರಚನಾ re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
ಸ್ಥಿರ ಸಂರಚನಾ re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
ಸ್ಥಿರ ಸಂರಚನಾ re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
ಸ್ಥಿರ ವರ್ಣ body_fastmap[UCHAR_MAX + 1];
ಸ್ಥಿರ ವರ್ಣ header_fastmap[UCHAR_MAX + 1];
ಸ್ಥಿರ ವರ್ಣ footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
ಸ್ಥಿರ ಸಂರಚನಾ re_pattern_buffer *current_regex = ರಿಕ್ತ;

/* Separator string to print after line number (-s).  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *separator_str = "\t";

/* Input section delimiter string (-d).  */
ಸ್ಥಿರ ವರ್ಣ *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
ಸ್ಥಿರ size_t section_del_len;

/* Header delimiter string.  */
ಸ್ಥಿರ ವರ್ಣ *header_del = ರಿಕ್ತ;

/* Header section delimiter length.  */
ಸ್ಥಿರ size_t header_del_len;

/* Body delimiter string.  */
ಸ್ಥಿರ ವರ್ಣ *body_del = ರಿಕ್ತ;

/* Body section delimiter length.  */
ಸ್ಥಿರ size_t body_del_len;

/* Footer delimiter string.  */
ಸ್ಥಿರ ವರ್ಣ *footer_del = ರಿಕ್ತ;

/* Footer section delimiter length.  */
ಸ್ಥಿರ size_t footer_del_len;

/* Input buffer.  */
ಸ್ಥಿರ ಸಂರಚನಾ linebuffer ಸಾಲು_buf;

/* printf format string for unnumbered lines.  */
ಸ್ಥಿರ ವರ್ಣ *print_no_ಸಾಲು_fmt = ರಿಕ್ತ;

/* Starting line number on each page (-v).  */
ಸ್ಥಿರ intmax_t starting_ಸಾಲು_ಸಂಖ್ಯೆ = 1;

/* Line number increment (-i).  */
ಸ್ಥಿರ intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
ಸ್ಥಿರ bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
ಸ್ಥಿರ intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ lineno_width = 6;

/* Line number format (-n).  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *lineno_format = FORMAT_ಬಲ_NOLZ;

/* Current print line number.  */
ಸ್ಥಿರ intmax_t ಸಾಲು_no;

/* Whether the current line number has incremented past limits.  */
ಸ್ಥಿರ bool ಸಾಲು_no_overflow;

/* True if we have ever read standard input.  */
ಸ್ಥಿರ bool have_read_stdin;

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"header-numbering", required_argument, ರಿಕ್ತ, 'h'},
  {"body-numbering", required_argument, ರಿಕ್ತ, 'b'},
  {"footer-numbering", required_argument, ರಿಕ್ತ, 'f'},
  {"starting-line-number", required_argument, ರಿಕ್ತ, 'v'},
  {"line-increment", required_argument, ರಿಕ್ತ, 'i'},
  {"no-renumber", no_argument, ರಿಕ್ತ, 'p'},
  {"join-blank-lines", required_argument, ರಿಕ್ತ, 'l'},
  {"number-separator", required_argument, ರಿಕ್ತ, 's'},
  {"number-width", required_argument, ರಿಕ್ತ, 'w'},
  {"number-format", required_argument, ರಿಕ್ತ, 'n'},
  {"section-delimiter", required_argument, ರಿಕ್ತ, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Print a usage message and quit. */

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, with ಸಾಲು numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE ಪ್ರತಿ numbering body ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC ಪ್ರತಿ logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE ಪ್ರತಿ numbering footer ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE ಪ್ರತಿ numbering header ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -i, --ಸಾಲು-increment=ಸಂಖ್ಯೆ_2     ಸಾಲು ಸಂಖ್ಯೆ increment at each ಸಾಲು\n\
"));
      oputs (_("\
  -l, --join-blank-ಸಾಲುಗಳು=ಸಂಖ್ಯೆ_2   group of ಸಂಖ್ಯೆ_2 empty ಸಾಲುಗಳು counted as one\n\
"));
      oputs (_("\
  -n, --ಸಂಖ್ಯೆ-format=FORMAT      ಸೇರಿಸು ಸಾಲು numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               ಕುರು not reset ಸಾಲು numbers ಪ್ರತಿ each section\n\
"));
      oputs (_("\
  -s, --ಸಂಖ್ಯೆ-separator=STRING   add STRING after (possible) ಸಾಲು ಸಂಖ್ಯೆ\n\
"));
      oputs (_("\
  -v, --starting-ಸಾಲು-ಸಂಖ್ಯೆ=ಸಂಖ್ಯೆ_2  first ಸಾಲು ಸಂಖ್ಯೆ ಪ್ರತಿ each section\n\
"));
      oputs (_("\
  -w, --ಸಂಖ್ಯೆ-width=ಸಂಖ್ಯೆ_2       use ಸಂಖ್ಯೆ_2 columns ಪ್ರತಿ ಸಾಲು numbers\n\
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
  a      ಸಂಖ್ಯೆ all ಸಾಲುಗಳು\n\
  t      ಸಂಖ್ಯೆ only nonempty ಸಾಲುಗಳು\n\
  n      ಸಂಖ್ಯೆ no ಸಾಲುಗಳು\n\
  pBRE   ಸಂಖ್ಯೆ only ಸಾಲುಗಳು that contain a match ಪ್ರತಿ the basic regular\n\
         expression, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     ಎಡ justified, no leading zeros\n\
  rn     ಬಲ justified, no leading zeros\n\
  rz     ಬಲ justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

ಸ್ಥಿರ bool
build_type_arg (ವರ್ಣ ಧ್ರುವ **typep,
                ಸಂರಚನಾ re_pattern_buffer *regexp, ವರ್ಣ *fastmap)
{
  ವರ್ಣ ಧ್ರುವ *errmsg;
  bool rval = true;

  ವಿಕಲ್ಪ (*optarg)
    {
    ಪಕ್ಷ 'a':
    ಪಕ್ಷ 't':
    ಪಕ್ಷ 'n':
      *typep = optarg;
      ಭಙ್ಗ;
    ಪಕ್ಷ 'p':
      *typep = optarg++;
      regexp->buffer = ರಿಕ್ತ;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = ರಿಕ್ತ;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, strlen (optarg), regexp);
      ಯದಿ (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      rval = false;
      ಭಙ್ಗ;
    }
  ಪ್ರತ್ಯಾಗಮ rval;
}

/* Print the line number and separator; increment the line number. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_lineno (ಶೂನ್ಯ)
{
  ಯದಿ (ಸಾಲು_no_overflow)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  printf (lineno_format, lineno_width, ಸಾಲು_no, separator_str);

  ಯದಿ (ckd_add (&ಸಾಲು_no, ಸಾಲು_no, page_incr))
    ಸಾಲು_no_overflow = true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
reset_lineno (ಶೂನ್ಯ)
{
  ಯದಿ (reset_numbers)
    {
      ಸಾಲು_no = starting_ಸಾಲು_ಸಂಖ್ಯೆ;
      ಸಾಲು_no_overflow = false;
    }
}

/* Switch to a header section. */

ಸ್ಥಿರ ಶೂನ್ಯ
proc_header (ಶೂನ್ಯ)
{
  current_type = header_type;
  current_regex = &header_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a body section. */

ಸ್ಥಿರ ಶೂನ್ಯ
proc_body (ಶೂನ್ಯ)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a footer section. */

ಸ್ಥಿರ ಶೂನ್ಯ
proc_footer (ಶೂನ್ಯ)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Process a regular text line in 'line_buf'. */

ಸ್ಥಿರ ಶೂನ್ಯ
proc_text (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ intmax_t blank_ಸಾಲುಗಳು = 0;	/* Consecutive blank lines so far. */

  ವಿಕಲ್ಪ (*current_type)
    {
    ಪಕ್ಷ 'a':
      ಯದಿ (blank_join > 1)
        {
          ಯದಿ (1 < ಸಾಲು_buf.length || ++blank_ಸಾಲುಗಳು == blank_join)
            {
              print_lineno ();
              blank_ಸಾಲುಗಳು = 0;
            }
          ಅನ್ಯಥಾ
            fputs (print_no_ಸಾಲು_fmt, stdout);
        }
      ಅನ್ಯಥಾ
        print_lineno ();
      ಭಙ್ಗ;
    ಪಕ್ಷ 't':
      ಯದಿ (1 < ಸಾಲು_buf.length)
        print_lineno ();
      ಅನ್ಯಥಾ
        fputs (print_no_ಸಾಲು_fmt, stdout);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'n':
      fputs (print_no_ಸಾಲು_fmt, stdout);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'p':
      ವಿಕಲ್ಪ (re_ಹುಡುಕು (current_regex, ಸಾಲು_buf.buffer, ಸಾಲು_buf.length - 1,
                         0, ಸಾಲು_buf.length - 1, ರಿಕ್ತ))
        {
        ಪಕ್ಷ -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        ಪಕ್ಷ -1:
          fputs (print_no_ಸಾಲು_fmt, stdout);
          ಭಙ್ಗ;

        ಸ್ವಭಾವ:
          print_lineno ();
          ಭಙ್ಗ;
        }
    }
  fwrite (ಸಾಲು_buf.buffer, ಪರಿಮಾಣ (ವರ್ಣ), ಸಾಲು_buf.length, stdout);
}

/* Return the type of line in 'line_buf'. */

ಸ್ಥಿರ ಗಣನಾ section
check_section (ಶೂನ್ಯ)
{
  size_t len = ಸಾಲು_buf.length - 1;

  ಯದಿ (len < section_del_len || footer_del_len < section_del_len
      || !memeq (ಸಾಲು_buf.buffer, section_del, section_del_len))
    ಪ್ರತ್ಯಾಗಮ Text;
  ಯದಿ (len == header_del_len
      && memeq (ಸಾಲು_buf.buffer, header_del, header_del_len))
    ಪ್ರತ್ಯಾಗಮ Header;
  ಯದಿ (len == body_del_len
      && memeq (ಸಾಲು_buf.buffer, body_del, body_del_len))
    ಪ್ರತ್ಯಾಗಮ Body;
  ಯದಿ (len == footer_del_len
      && memeq (ಸಾಲು_buf.buffer, footer_del, footer_del_len))
    ಪ್ರತ್ಯಾಗಮ Footer;
  ಪ್ರತ್ಯಾಗಮ Text;
}

/* Read and process the file pointed to by FP. */

ಸ್ಥಿರ ಶೂನ್ಯ
process_file (FILE *fp)
{
  ಯಾವತ್ (readlinebuffer (&ಸಾಲು_buf, fp))
    {
      ವಿಕಲ್ಪ (check_section ())
        {
        ಪಕ್ಷ Header:
          proc_header ();
          ಭಙ್ಗ;
        ಪಕ್ಷ Body:
          proc_body ();
          ಭಙ್ಗ;
        ಪಕ್ಷ Footer:
          proc_footer ();
          ಭಙ್ಗ;
        ಪಕ್ಷ Text:
          proc_text ();
          ಭಙ್ಗ;
        }

      ಯದಿ (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

ಸ್ಥಿರ bool
nl_file (ವರ್ಣ ಧ್ರುವ *file)
{
  FILE *stream;

  ಯದಿ (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  ಅನ್ಯಥಾ
    {
      stream = fopen (file, "r");
      ಯದಿ (stream == ರಿಕ್ತ)
        {
          error (0, errno, "%s", quotef (file));
          ಪ್ರತ್ಯಾಗಮ false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  ಪೂರ್ಣಾಙ್ಕ err = errno;
  ಯದಿ (!ferror (stream))
    err = 0;
  ಯದಿ (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF. */
  ಅನ್ಯಥಾ ಯದಿ (fclose (stream) != 0 && !err)
    err = errno;
  ಯದಿ (err)
    {
      error (0, err, "%s", quotef (file));
      ಪ್ರತ್ಯಾಗಮ false;
    }
  ಪ್ರತ್ಯಾಗಮ true;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'h':
          ಯದಿ (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ಭಙ್ಗ;
        ಪಕ್ಷ 'b':
          ಯದಿ (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ಭಙ್ಗ;
        ಪಕ್ಷ 'f':
          ಯದಿ (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ಭಙ್ಗ;
        ಪಕ್ಷ 'v':
          starting_ಸಾಲು_ಸಂಖ್ಯೆ = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          ಭಙ್ಗ;
        ಪಕ್ಷ 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          ಭಙ್ಗ;
        ಪಕ್ಷ 'p':
          reset_numbers = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          ಭಙ್ಗ;
        ಪಕ್ಷ 's':
          separator_str = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, INT_MAX, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          ಭಙ್ಗ;
        ಪಕ್ಷ 'n':
          ಯದಿ (streq (optarg, "ln"))
            lineno_format = FORMAT_ಎಡ;
          ಅನ್ಯಥಾ ಯದಿ (streq (optarg, "rn"))
            lineno_format = FORMAT_ಬಲ_NOLZ;
          ಅನ್ಯಥಾ ಯದಿ (streq (optarg, "rz"))
            lineno_format = FORMAT_ಬಲ_LZ;
          ಅನ್ಯಥಾ
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          ಭಙ್ಗ;
        ಪಕ್ಷ 'd':
          len = strlen (optarg);
          ಯದಿ (1 < MB_CUR_MAX)
            {
              ವರ್ಣ ಧ್ರುವ *p = optarg;
              ವರ್ಣ ಧ್ರುವ *lim = p + len;
              ಪೂರ್ಣಾಙ್ಕ n_chars = 0;
              ಪ್ರತಿ (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              ಯದಿ (n_chars == 1)
                memcpy (mempcpy (section_del, optarg, len),  ":", ಪರಿಮಾಣ ":");
              ಅನ್ಯಥಾ
                section_del = optarg;
            }
          ಅನ್ಯಥಾ
            {
              ಯದಿ (len == 1)
                *section_del = *optarg;
              ಅನ್ಯಥಾ
                section_del = optarg;
            }
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          ok = false;
          ಭಙ್ಗ;
        }
    }

  ಯದಿ (!ok)
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
  initbuffer (&ಸಾಲು_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = strlen (separator_str);
  print_no_ಸಾಲು_fmt = xmalloc (lineno_width + len + 1);
  memset (print_no_ಸಾಲು_fmt, ' ', lineno_width + len);
  print_no_ಸಾಲು_fmt[lineno_width + len] = '\0';

  ಸಾಲು_no = starting_ಸಾಲು_ಸಂಖ್ಯೆ;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  ಯದಿ (optind == argc)
    ok = nl_file ("-");
  ಅನ್ಯಥಾ
    ಪ್ರತಿ (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  ಯದಿ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
