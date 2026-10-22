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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <getopt.h>

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <regex.h>

#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "linebuffer.h"
#અન્તર્ભાવ "mcel.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nl"

#define AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
સ્થિર અક્ષર અચલ FORMAT_જમણું_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
સ્થિર અક્ષર અચલ FORMAT_જમણું_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
સ્થિર અક્ષર અચલ FORMAT_ડાબું[] = "%-*jd%s";

/* Default section delimiter characters.  */
સ્થિર અક્ષર DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
ગણના section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
સ્થિર અક્ષર અચલ *body_type = "t";

/* Format of header lines (-h).  */
સ્થિર અક્ષર અચલ *header_type = "n";

/* Format of footer lines (-f).  */
સ્થિર અક્ષર અચલ *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
સ્થિર અક્ષર અચલ *current_type;

/* Regex for body lines to number (-bp).  */
સ્થિર સંરચના re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
સ્થિર સંરચના re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
સ્થિર સંરચના re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
સ્થિર અક્ષર body_fastmap[UCHAR_MAX + 1];
સ્થિર અક્ષર header_fastmap[UCHAR_MAX + 1];
સ્થિર અક્ષર footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
સ્થિર સંરચના re_pattern_buffer *current_regex = ખાલી;

/* Separator string to print after line number (-s).  */
સ્થિર અક્ષર અચલ *separator_str = "\t";

/* Input section delimiter string (-d).  */
સ્થિર અક્ષર *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
સ્થિર size_t section_del_len;

/* Header delimiter string.  */
સ્થિર અક્ષર *header_del = ખાલી;

/* Header section delimiter length.  */
સ્થિર size_t header_del_len;

/* Body delimiter string.  */
સ્થિર અક્ષર *body_del = ખાલી;

/* Body section delimiter length.  */
સ્થિર size_t body_del_len;

/* Footer delimiter string.  */
સ્થિર અક્ષર *footer_del = ખાલી;

/* Footer section delimiter length.  */
સ્થિર size_t footer_del_len;

/* Input buffer.  */
સ્થિર સંરચના linebuffer લીટી_buf;

/* printf format string for unnumbered lines.  */
સ્થિર અક્ષર *print_no_લીટી_fmt = ખાલી;

/* Starting line number on each page (-v).  */
સ્થિર intmax_t starting_લીટી_સંખ્યા = 1;

/* Line number increment (-i).  */
સ્થિર intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
સ્થિર bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
સ્થિર intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
સ્થિર પૂર્ણાંક lineno_width = 6;

/* Line number format (-n).  */
સ્થિર અક્ષર અચલ *lineno_format = FORMAT_જમણું_NOLZ;

/* Current print line number.  */
સ્થિર intmax_t લીટી_no;

/* Whether the current line number has incremented past limits.  */
સ્થિર bool લીટી_no_overflow;

/* True if we have ever read standard input.  */
સ્થિર bool have_read_stdin;

સ્થિર સંરચના option અચલ longopts[] =
{
  {"header-numbering", required_argument, ખાલી, 'h'},
  {"body-numbering", required_argument, ખાલી, 'b'},
  {"footer-numbering", required_argument, ખાલી, 'f'},
  {"starting-line-number", required_argument, ખાલી, 'v'},
  {"line-increment", required_argument, ખાલી, 'i'},
  {"no-renumber", no_argument, ખાલી, 'p'},
  {"join-blank-lines", required_argument, ખાલી, 'l'},
  {"number-separator", required_argument, ખાલી, 's'},
  {"number-width", required_argument, ખાલી, 'w'},
  {"number-format", required_argument, ખાલી, 'n'},
  {"section-delimiter", required_argument, ખાલી, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Print a usage message and quit. */

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, with લીટી numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE માટે numbering body લીટીઓ\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC માટે logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE માટે numbering footer લીટીઓ\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE માટે numbering header લીટીઓ\n\
"));
      oputs (_("\
  -i, --લીટી-increment=સંખ્યા_2     લીટી સંખ્યા increment at each લીટી\n\
"));
      oputs (_("\
  -l, --join-blank-લીટીઓ=સંખ્યા_2   group of સંખ્યા_2 empty લીટીઓ counted as one\n\
"));
      oputs (_("\
  -n, --સંખ્યા-format=FORMAT      ઉમેરો લીટી numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               કરો not reset લીટી numbers માટે each section\n\
"));
      oputs (_("\
  -s, --સંખ્યા-separator=STRING   add STRING after (possible) લીટી સંખ્યા\n\
"));
      oputs (_("\
  -v, --starting-લીટી-સંખ્યા=સંખ્યા_2  first લીટી સંખ્યા માટે each section\n\
"));
      oputs (_("\
  -w, --સંખ્યા-width=સંખ્યા_2       use સંખ્યા_2 columns માટે લીટી numbers\n\
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
  a      સંખ્યા all લીટીઓ\n\
  t      સંખ્યા only nonempty લીટીઓ\n\
  n      સંખ્યા no લીટીઓ\n\
  pBRE   સંખ્યા only લીટીઓ that contain a match માટે the basic regular\n\
         expression, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     ડાબું justified, no leading zeros\n\
  rn     જમણું justified, no leading zeros\n\
  rz     જમણું justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

સ્થિર bool
build_type_arg (અક્ષર અચલ **typep,
                સંરચના re_pattern_buffer *regexp, અક્ષર *fastmap)
{
  અક્ષર અચલ *errmsg;
  bool rval = true;

  પસંદગી (*optarg)
    {
    સ્થિતિ 'a':
    સ્થિતિ 't':
    સ્થિતિ 'n':
      *typep = optarg;
      તોડો;
    સ્થિતિ 'p':
      *typep = optarg++;
      regexp->buffer = ખાલી;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = ખાલી;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, strlen (optarg), regexp);
      જો (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      તોડો;
    મૂળભૂત:
      rval = false;
      તોડો;
    }
  પાછા rval;
}

/* Print the line number and separator; increment the line number. */

સ્થિર શૂન્ય
print_lineno (શૂન્ય)
{
  જો (લીટી_no_overflow)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  printf (lineno_format, lineno_width, લીટી_no, separator_str);

  જો (ckd_add (&લીટી_no, લીટી_no, page_incr))
    લીટી_no_overflow = true;
}

સ્થિર શૂન્ય
reset_lineno (શૂન્ય)
{
  જો (reset_numbers)
    {
      લીટી_no = starting_લીટી_સંખ્યા;
      લીટી_no_overflow = false;
    }
}

/* Switch to a header section. */

સ્થિર શૂન્ય
proc_header (શૂન્ય)
{
  current_type = header_type;
  current_regex = &header_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a body section. */

સ્થિર શૂન્ય
proc_body (શૂન્ય)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a footer section. */

સ્થિર શૂન્ય
proc_footer (શૂન્ય)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Process a regular text line in 'line_buf'. */

સ્થિર શૂન્ય
proc_text (શૂન્ય)
{
  સ્થિર intmax_t blank_લીટીઓ = 0;	/* Consecutive blank lines so far. */

  પસંદગી (*current_type)
    {
    સ્થિતિ 'a':
      જો (blank_join > 1)
        {
          જો (1 < લીટી_buf.length || ++blank_લીટીઓ == blank_join)
            {
              print_lineno ();
              blank_લીટીઓ = 0;
            }
          નહીંતર
            fputs (print_no_લીટી_fmt, stdout);
        }
      નહીંતર
        print_lineno ();
      તોડો;
    સ્થિતિ 't':
      જો (1 < લીટી_buf.length)
        print_lineno ();
      નહીંતર
        fputs (print_no_લીટી_fmt, stdout);
      તોડો;
    સ્થિતિ 'n':
      fputs (print_no_લીટી_fmt, stdout);
      તોડો;
    સ્થિતિ 'p':
      પસંદગી (re_શોધ (current_regex, લીટી_buf.buffer, લીટી_buf.length - 1,
                         0, લીટી_buf.length - 1, ખાલી))
        {
        સ્થિતિ -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        સ્થિતિ -1:
          fputs (print_no_લીટી_fmt, stdout);
          તોડો;

        મૂળભૂત:
          print_lineno ();
          તોડો;
        }
    }
  fwrite (લીટી_buf.buffer, કદ (અક્ષર), લીટી_buf.length, stdout);
}

/* Return the type of line in 'line_buf'. */

સ્થિર ગણના section
check_section (શૂન્ય)
{
  size_t len = લીટી_buf.length - 1;

  જો (len < section_del_len || footer_del_len < section_del_len
      || !memeq (લીટી_buf.buffer, section_del, section_del_len))
    પાછા Text;
  જો (len == header_del_len
      && memeq (લીટી_buf.buffer, header_del, header_del_len))
    પાછા Header;
  જો (len == body_del_len
      && memeq (લીટી_buf.buffer, body_del, body_del_len))
    પાછા Body;
  જો (len == footer_del_len
      && memeq (લીટી_buf.buffer, footer_del, footer_del_len))
    પાછા Footer;
  પાછા Text;
}

/* Read and process the file pointed to by FP. */

સ્થિર શૂન્ય
process_file (FILE *fp)
{
  જ્યારે (readlinebuffer (&લીટી_buf, fp))
    {
      પસંદગી (check_section ())
        {
        સ્થિતિ Header:
          proc_header ();
          તોડો;
        સ્થિતિ Body:
          proc_body ();
          તોડો;
        સ્થિતિ Footer:
          proc_footer ();
          તોડો;
        સ્થિતિ Text:
          proc_text ();
          તોડો;
        }

      જો (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

સ્થિર bool
nl_file (અક્ષર અચલ *file)
{
  FILE *stream;

  જો (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  નહીંતર
    {
      stream = fopen (file, "r");
      જો (stream == ખાલી)
        {
          error (0, errno, "%s", quotef (file));
          પાછા false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  પૂર્ણાંક err = errno;
  જો (!ferror (stream))
    err = 0;
  જો (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF. */
  નહીંતર જો (fclose (stream) != 0 && !err)
    err = errno;
  જો (err)
    {
      error (0, err, "%s", quotef (file));
      પાછા false;
    }
  પાછા true;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           ખાલી))
         != -1)
    {
      પસંદગી (c)
        {
        સ્થિતિ 'h':
          જો (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          તોડો;
        સ્થિતિ 'b':
          જો (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          તોડો;
        સ્થિતિ 'f':
          જો (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          તોડો;
        સ્થિતિ 'v':
          starting_લીટી_સંખ્યા = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          તોડો;
        સ્થિતિ 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          તોડો;
        સ્થિતિ 'p':
          reset_numbers = false;
          તોડો;
        સ્થિતિ 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          તોડો;
        સ્થિતિ 's':
          separator_str = optarg;
          તોડો;
        સ્થિતિ 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, INT_MAX, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          તોડો;
        સ્થિતિ 'n':
          જો (streq (optarg, "ln"))
            lineno_format = FORMAT_ડાબું;
          નહીંતર જો (streq (optarg, "rn"))
            lineno_format = FORMAT_જમણું_NOLZ;
          નહીંતર જો (streq (optarg, "rz"))
            lineno_format = FORMAT_જમણું_LZ;
          નહીંતર
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          તોડો;
        સ્થિતિ 'd':
          len = strlen (optarg);
          જો (1 < MB_CUR_MAX)
            {
              અક્ષર અચલ *p = optarg;
              અક્ષર અચલ *lim = p + len;
              પૂર્ણાંક n_chars = 0;
              માટે (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              જો (n_chars == 1)
                memcpy (mempcpy (section_del, optarg, len),  ":", કદ ":");
              નહીંતર
                section_del = optarg;
            }
          નહીંતર
            {
              જો (len == 1)
                *section_del = *optarg;
              નહીંતર
                section_del = optarg;
            }
          તોડો;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          ok = false;
          તોડો;
        }
    }

  જો (!ok)
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
  initbuffer (&લીટી_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = strlen (separator_str);
  print_no_લીટી_fmt = xmalloc (lineno_width + len + 1);
  memset (print_no_લીટી_fmt, ' ', lineno_width + len);
  print_no_લીટી_fmt[lineno_width + len] = '\0';

  લીટી_no = starting_લીટી_સંખ્યા;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  જો (optind == argc)
    ok = nl_file ("-");
  નહીંતર
    માટે (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  જો (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
