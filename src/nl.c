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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <regex.h>

#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "linebuffer.h"
#ଅନ୍ତର୍ଭାଵ "mcel.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nl"

#define AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ FORMAT_ଡାହାଣ_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ FORMAT_ଡାହାଣ_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ FORMAT_ବାମ[] = "%-*jd%s";

/* Default section delimiter characters.  */
ସ୍ଥିର ଅକ୍ଷର DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
ଗଣନା section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *body_type = "t";

/* Format of header lines (-h).  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *header_type = "n";

/* Format of footer lines (-f).  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *current_type;

/* Regex for body lines to number (-bp).  */
ସ୍ଥିର ସଂରଚନା re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
ସ୍ଥିର ସଂରଚନା re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
ସ୍ଥିର ସଂରଚନା re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
ସ୍ଥିର ଅକ୍ଷର body_fastmap[UCHAR_MAX + 1];
ସ୍ଥିର ଅକ୍ଷର header_fastmap[UCHAR_MAX + 1];
ସ୍ଥିର ଅକ୍ଷର footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
ସ୍ଥିର ସଂରଚନା re_pattern_buffer *current_regex = ଖାଲି;

/* Separator string to print after line number (-s).  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *separator_str = "\t";

/* Input section delimiter string (-d).  */
ସ୍ଥିର ଅକ୍ଷର *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
ସ୍ଥିର size_t section_del_len;

/* Header delimiter string.  */
ସ୍ଥିର ଅକ୍ଷର *header_del = ଖାଲି;

/* Header section delimiter length.  */
ସ୍ଥିର size_t header_del_len;

/* Body delimiter string.  */
ସ୍ଥିର ଅକ୍ଷର *body_del = ଖାଲି;

/* Body section delimiter length.  */
ସ୍ଥିର size_t body_del_len;

/* Footer delimiter string.  */
ସ୍ଥିର ଅକ୍ଷର *footer_del = ଖାଲି;

/* Footer section delimiter length.  */
ସ୍ଥିର size_t footer_del_len;

/* Input buffer.  */
ସ୍ଥିର ସଂରଚନା linebuffer ଧାଡ଼ି_buf;

/* printf format string for unnumbered lines.  */
ସ୍ଥିର ଅକ୍ଷର *print_no_ଧାଡ଼ି_fmt = ଖାଲି;

/* Starting line number on each page (-v).  */
ସ୍ଥିର intmax_t starting_ଧାଡ଼ି_ସଂଖ୍ୟା = 1;

/* Line number increment (-i).  */
ସ୍ଥିର intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
ସ୍ଥିର bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
ସ୍ଥିର intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା lineno_width = 6;

/* Line number format (-n).  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *lineno_format = FORMAT_ଡାହାଣ_NOLZ;

/* Current print line number.  */
ସ୍ଥିର intmax_t ଧାଡ଼ି_no;

/* Whether the current line number has incremented past limits.  */
ସ୍ଥିର bool ଧାଡ଼ି_no_overflow;

/* True if we have ever read standard input.  */
ସ୍ଥିର bool have_read_stdin;

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"header-numbering", required_argument, ଖାଲି, 'h'},
  {"body-numbering", required_argument, ଖାଲି, 'b'},
  {"footer-numbering", required_argument, ଖାଲି, 'f'},
  {"starting-line-number", required_argument, ଖାଲି, 'v'},
  {"line-increment", required_argument, ଖାଲି, 'i'},
  {"no-renumber", no_argument, ଖାଲି, 'p'},
  {"join-blank-lines", required_argument, ଖାଲି, 'l'},
  {"number-separator", required_argument, ଖାଲି, 's'},
  {"number-width", required_argument, ଖାଲି, 'w'},
  {"number-format", required_argument, ଖାଲି, 'n'},
  {"section-delimiter", required_argument, ଖାଲି, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Print a usage message and quit. */

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, with ଧାଡ଼ି numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE ପାଇଁ numbering body ଧାଡ଼ିଗୁଡ଼ିକ\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC ପାଇଁ logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE ପାଇଁ numbering footer ଧାଡ଼ିଗୁଡ଼ିକ\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE ପାଇଁ numbering header ଧାଡ଼ିଗୁଡ଼ିକ\n\
"));
      oputs (_("\
  -i, --ଧାଡ଼ି-increment=ସଂଖ୍ୟା_2     ଧାଡ଼ି ସଂଖ୍ୟା increment at each ଧାଡ଼ି\n\
"));
      oputs (_("\
  -l, --join-blank-ଧାଡ଼ିଗୁଡ଼ିକ=ସଂଖ୍ୟା_2   group of ସଂଖ୍ୟା_2 empty ଧାଡ଼ିଗୁଡ଼ିକ counted as one\n\
"));
      oputs (_("\
  -n, --ସଂଖ୍ୟା-format=FORMAT      ଭର୍ତ୍ତି ଧାଡ଼ି numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               କର not reset ଧାଡ଼ି numbers ପାଇଁ each section\n\
"));
      oputs (_("\
  -s, --ସଂଖ୍ୟା-separator=STRING   add STRING after (possible) ଧାଡ଼ି ସଂଖ୍ୟା\n\
"));
      oputs (_("\
  -v, --starting-ଧାଡ଼ି-ସଂଖ୍ୟା=ସଂଖ୍ୟା_2  first ଧାଡ଼ି ସଂଖ୍ୟା ପାଇଁ each section\n\
"));
      oputs (_("\
  -w, --ସଂଖ୍ୟା-width=ସଂଖ୍ୟା_2       use ସଂଖ୍ୟା_2 columns ପାଇଁ ଧାଡ଼ି numbers\n\
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
  a      ସଂଖ୍ୟା all ଧାଡ଼ିଗୁଡ଼ିକ\n\
  t      ସଂଖ୍ୟା only nonempty ଧାଡ଼ିଗୁଡ଼ିକ\n\
  n      ସଂଖ୍ୟା no ଧାଡ଼ିଗୁଡ଼ିକ\n\
  pBRE   ସଂଖ୍ୟା only ଧାଡ଼ିଗୁଡ଼ିକ that contain a match ପାଇଁ the basic regular\n\
         expression, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     ବାମ justified, no leading zeros\n\
  rn     ଡାହାଣ justified, no leading zeros\n\
  rz     ଡାହାଣ justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

ସ୍ଥିର bool
build_type_arg (ଅକ୍ଷର ଅଚଳ **typep,
                ସଂରଚନା re_pattern_buffer *regexp, ଅକ୍ଷର *fastmap)
{
  ଅକ୍ଷର ଅଚଳ *errmsg;
  bool rval = true;

  ବାଛ (*optarg)
    {
    ସ୍ଥିତି 'a':
    ସ୍ଥିତି 't':
    ସ୍ଥିତି 'n':
      *typep = optarg;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'p':
      *typep = optarg++;
      regexp->buffer = ଖାଲି;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = ଖାଲି;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, strlen (optarg), regexp);
      ଯଦି (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      rval = false;
      ଭାଙ୍ଗ;
    }
  ଫେରନ୍ତୁ rval;
}

/* Print the line number and separator; increment the line number. */

ସ୍ଥିର ଶୂନ୍ୟ
print_lineno (ଶୂନ୍ୟ)
{
  ଯଦି (ଧାଡ଼ି_no_overflow)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  printf (lineno_format, lineno_width, ଧାଡ଼ି_no, separator_str);

  ଯଦି (ckd_add (&ଧାଡ଼ି_no, ଧାଡ଼ି_no, page_incr))
    ଧାଡ଼ି_no_overflow = true;
}

ସ୍ଥିର ଶୂନ୍ୟ
reset_lineno (ଶୂନ୍ୟ)
{
  ଯଦି (reset_numbers)
    {
      ଧାଡ଼ି_no = starting_ଧାଡ଼ି_ସଂଖ୍ୟା;
      ଧାଡ଼ି_no_overflow = false;
    }
}

/* Switch to a header section. */

ସ୍ଥିର ଶୂନ୍ୟ
proc_header (ଶୂନ୍ୟ)
{
  current_type = header_type;
  current_regex = &header_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a body section. */

ସ୍ଥିର ଶୂନ୍ୟ
proc_body (ଶୂନ୍ୟ)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a footer section. */

ସ୍ଥିର ଶୂନ୍ୟ
proc_footer (ଶୂନ୍ୟ)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Process a regular text line in 'line_buf'. */

ସ୍ଥିର ଶୂନ୍ୟ
proc_text (ଶୂନ୍ୟ)
{
  ସ୍ଥିର intmax_t blank_ଧାଡ଼ିଗୁଡ଼ିକ = 0;	/* Consecutive blank lines so far. */

  ବାଛ (*current_type)
    {
    ସ୍ଥିତି 'a':
      ଯଦି (blank_join > 1)
        {
          ଯଦି (1 < ଧାଡ଼ି_buf.length || ++blank_ଧାଡ଼ିଗୁଡ଼ିକ == blank_join)
            {
              print_lineno ();
              blank_ଧାଡ଼ିଗୁଡ଼ିକ = 0;
            }
          ଅନ୍ୟଥା
            fputs (print_no_ଧାଡ଼ି_fmt, stdout);
        }
      ଅନ୍ୟଥା
        print_lineno ();
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 't':
      ଯଦି (1 < ଧାଡ଼ି_buf.length)
        print_lineno ();
      ଅନ୍ୟଥା
        fputs (print_no_ଧାଡ଼ି_fmt, stdout);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'n':
      fputs (print_no_ଧାଡ଼ି_fmt, stdout);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'p':
      ବାଛ (re_ସନ୍ଧାନ (current_regex, ଧାଡ଼ି_buf.buffer, ଧାଡ଼ି_buf.length - 1,
                         0, ଧାଡ଼ି_buf.length - 1, ଖାଲି))
        {
        ସ୍ଥିତି -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        ସ୍ଥିତି -1:
          fputs (print_no_ଧାଡ଼ି_fmt, stdout);
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          print_lineno ();
          ଭାଙ୍ଗ;
        }
    }
  fwrite (ଧାଡ଼ି_buf.buffer, ଆକାର (ଅକ୍ଷର), ଧାଡ଼ି_buf.length, stdout);
}

/* Return the type of line in 'line_buf'. */

ସ୍ଥିର ଗଣନା section
check_section (ଶୂନ୍ୟ)
{
  size_t len = ଧାଡ଼ି_buf.length - 1;

  ଯଦି (len < section_del_len || footer_del_len < section_del_len
      || !memeq (ଧାଡ଼ି_buf.buffer, section_del, section_del_len))
    ଫେରନ୍ତୁ Text;
  ଯଦି (len == header_del_len
      && memeq (ଧାଡ଼ି_buf.buffer, header_del, header_del_len))
    ଫେରନ୍ତୁ Header;
  ଯଦି (len == body_del_len
      && memeq (ଧାଡ଼ି_buf.buffer, body_del, body_del_len))
    ଫେରନ୍ତୁ Body;
  ଯଦି (len == footer_del_len
      && memeq (ଧାଡ଼ି_buf.buffer, footer_del, footer_del_len))
    ଫେରନ୍ତୁ Footer;
  ଫେରନ୍ତୁ Text;
}

/* Read and process the file pointed to by FP. */

ସ୍ଥିର ଶୂନ୍ୟ
process_file (FILE *fp)
{
  ଯେତେବେଳେ (readlinebuffer (&ଧାଡ଼ି_buf, fp))
    {
      ବାଛ (check_section ())
        {
        ସ୍ଥିତି Header:
          proc_header ();
          ଭାଙ୍ଗ;
        ସ୍ଥିତି Body:
          proc_body ();
          ଭାଙ୍ଗ;
        ସ୍ଥିତି Footer:
          proc_footer ();
          ଭାଙ୍ଗ;
        ସ୍ଥିତି Text:
          proc_text ();
          ଭାଙ୍ଗ;
        }

      ଯଦି (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

ସ୍ଥିର bool
nl_file (ଅକ୍ଷର ଅଚଳ *file)
{
  FILE *stream;

  ଯଦି (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  ଅନ୍ୟଥା
    {
      stream = fopen (file, "r");
      ଯଦି (stream == ଖାଲି)
        {
          error (0, errno, "%s", quotef (file));
          ଫେରନ୍ତୁ false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = errno;
  ଯଦି (!ferror (stream))
    err = 0;
  ଯଦି (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF. */
  ଅନ୍ୟଥା ଯଦି (fclose (stream) != 0 && !err)
    err = errno;
  ଯଦି (err)
    {
      error (0, err, "%s", quotef (file));
      ଫେରନ୍ତୁ false;
    }
  ଫେରନ୍ତୁ true;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           ଖାଲି))
         != -1)
    {
      ବାଛ (c)
        {
        ସ୍ଥିତି 'h':
          ଯଦି (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'b':
          ଯଦି (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'f':
          ଯଦି (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'v':
          starting_ଧାଡ଼ି_ସଂଖ୍ୟା = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'p':
          reset_numbers = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 's':
          separator_str = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, INT_MAX, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'n':
          ଯଦି (streq (optarg, "ln"))
            lineno_format = FORMAT_ବାମ;
          ଅନ୍ୟଥା ଯଦି (streq (optarg, "rn"))
            lineno_format = FORMAT_ଡାହାଣ_NOLZ;
          ଅନ୍ୟଥା ଯଦି (streq (optarg, "rz"))
            lineno_format = FORMAT_ଡାହାଣ_LZ;
          ଅନ୍ୟଥା
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'd':
          len = strlen (optarg);
          ଯଦି (1 < MB_CUR_MAX)
            {
              ଅକ୍ଷର ଅଚଳ *p = optarg;
              ଅକ୍ଷର ଅଚଳ *lim = p + len;
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_chars = 0;
              ପାଇଁ (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              ଯଦି (n_chars == 1)
                memcpy (mempcpy (section_del, optarg, len),  ":", ଆକାର ":");
              ଅନ୍ୟଥା
                section_del = optarg;
            }
          ଅନ୍ୟଥା
            {
              ଯଦି (len == 1)
                *section_del = *optarg;
              ଅନ୍ୟଥା
                section_del = optarg;
            }
          ଭାଙ୍ଗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          ok = false;
          ଭାଙ୍ଗ;
        }
    }

  ଯଦି (!ok)
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
  initbuffer (&ଧାଡ଼ି_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = strlen (separator_str);
  print_no_ଧାଡ଼ି_fmt = xmalloc (lineno_width + len + 1);
  memset (print_no_ଧାଡ଼ି_fmt, ' ', lineno_width + len);
  print_no_ଧାଡ଼ି_fmt[lineno_width + len] = '\0';

  ଧାଡ଼ି_no = starting_ଧାଡ଼ି_ସଂଖ୍ୟା;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  ଯଦି (optind == argc)
    ok = nl_file ("-");
  ଅନ୍ୟଥା
    ପାଇଁ (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  ଯଦି (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
