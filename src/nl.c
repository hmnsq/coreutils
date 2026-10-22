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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <getopt.h>

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <regex.h>

#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "linebuffer.h"
#അന്തര്ഭാവ "mcel.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nl"

#define AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം FORMAT_വലത്_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം FORMAT_വലത്_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം FORMAT_ഇടത്[] = "%-*jd%s";

/* Default section delimiter characters.  */
സ്ഥിതം അക്ഷരം DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
എണ്ണൽ section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *body_type = "t";

/* Format of header lines (-h).  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *header_type = "n";

/* Format of footer lines (-f).  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *current_type;

/* Regex for body lines to number (-bp).  */
സ്ഥിതം ഘടന re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
സ്ഥിതം ഘടന re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
സ്ഥിതം ഘടന re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
സ്ഥിതം അക്ഷരം body_fastmap[UCHAR_MAX + 1];
സ്ഥിതം അക്ഷരം header_fastmap[UCHAR_MAX + 1];
സ്ഥിതം അക്ഷരം footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
സ്ഥിതം ഘടന re_pattern_buffer *current_regex = ശൂന്യ;

/* Separator string to print after line number (-s).  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *separator_str = "\t";

/* Input section delimiter string (-d).  */
സ്ഥിതം അക്ഷരം *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
സ്ഥിതം size_t section_del_len;

/* Header delimiter string.  */
സ്ഥിതം അക്ഷരം *header_del = ശൂന്യ;

/* Header section delimiter length.  */
സ്ഥിതം size_t header_del_len;

/* Body delimiter string.  */
സ്ഥിതം അക്ഷരം *body_del = ശൂന്യ;

/* Body section delimiter length.  */
സ്ഥിതം size_t body_del_len;

/* Footer delimiter string.  */
സ്ഥിതം അക്ഷരം *footer_del = ശൂന്യ;

/* Footer section delimiter length.  */
സ്ഥിതം size_t footer_del_len;

/* Input buffer.  */
സ്ഥിതം ഘടന linebuffer വരി_buf;

/* printf format string for unnumbered lines.  */
സ്ഥിതം അക്ഷരം *print_no_വരി_fmt = ശൂന്യ;

/* Starting line number on each page (-v).  */
സ്ഥിതം intmax_t starting_വരി_സംഖ്യ = 1;

/* Line number increment (-i).  */
സ്ഥിതം intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
സ്ഥിതം bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
സ്ഥിതം intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
സ്ഥിതം പൂർണ്ണസംഖ്യ lineno_width = 6;

/* Line number format (-n).  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *lineno_format = FORMAT_വലത്_NOLZ;

/* Current print line number.  */
സ്ഥിതം intmax_t വരി_no;

/* Whether the current line number has incremented past limits.  */
സ്ഥിതം bool വരി_no_overflow;

/* True if we have ever read standard input.  */
സ്ഥിതം bool have_read_stdin;

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"header-numbering", required_argument, ശൂന്യ, 'h'},
  {"body-numbering", required_argument, ശൂന്യ, 'b'},
  {"footer-numbering", required_argument, ശൂന്യ, 'f'},
  {"starting-line-number", required_argument, ശൂന്യ, 'v'},
  {"line-increment", required_argument, ശൂന്യ, 'i'},
  {"no-renumber", no_argument, ശൂന്യ, 'p'},
  {"join-blank-lines", required_argument, ശൂന്യ, 'l'},
  {"number-separator", required_argument, ശൂന്യ, 's'},
  {"number-width", required_argument, ശൂന്യ, 'w'},
  {"number-format", required_argument, ശൂന്യ, 'n'},
  {"section-delimiter", required_argument, ശൂന്യ, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Print a usage message and quit. */

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, with വരി numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE വേണ്ടി numbering body വരികൾ\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC വേണ്ടി logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE വേണ്ടി numbering footer വരികൾ\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE വേണ്ടി numbering header വരികൾ\n\
"));
      oputs (_("\
  -i, --വരി-increment=സംഖ്യ_2     വരി സംഖ്യ increment at each വരി\n\
"));
      oputs (_("\
  -l, --join-blank-വരികൾ=സംഖ്യ_2   group of സംഖ്യ_2 empty വരികൾ counted as one\n\
"));
      oputs (_("\
  -n, --സംഖ്യ-format=FORMAT      തിരുകുക വരി numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               ചെയ്യുക not reset വരി numbers വേണ്ടി each section\n\
"));
      oputs (_("\
  -s, --സംഖ്യ-separator=STRING   add STRING after (possible) വരി സംഖ്യ\n\
"));
      oputs (_("\
  -v, --starting-വരി-സംഖ്യ=സംഖ്യ_2  first വരി സംഖ്യ വേണ്ടി each section\n\
"));
      oputs (_("\
  -w, --സംഖ്യ-width=സംഖ്യ_2       use സംഖ്യ_2 columns വേണ്ടി വരി numbers\n\
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
  a      സംഖ്യ all വരികൾ\n\
  t      സംഖ്യ only nonempty വരികൾ\n\
  n      സംഖ്യ no വരികൾ\n\
  pBRE   സംഖ്യ only വരികൾ that contain a match വേണ്ടി the basic regular\n\
         expression, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     ഇടത് justified, no leading zeros\n\
  rn     വലത് justified, no leading zeros\n\
  rz     വലത് justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

സ്ഥിതം bool
build_type_arg (അക്ഷരം സ്ഥിരം **typep,
                ഘടന re_pattern_buffer *regexp, അക്ഷരം *fastmap)
{
  അക്ഷരം സ്ഥിരം *errmsg;
  bool rval = true;

  തിരഞ്ഞെടുപ്പ് (*optarg)
    {
    അവസ്ഥ 'a':
    അവസ്ഥ 't':
    അവസ്ഥ 'n':
      *typep = optarg;
      നിർത്തുക;
    അവസ്ഥ 'p':
      *typep = optarg++;
      regexp->buffer = ശൂന്യ;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = ശൂന്യ;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, strlen (optarg), regexp);
      എങ്കിൽ (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      rval = false;
      നിർത്തുക;
    }
  തിരികെ rval;
}

/* Print the line number and separator; increment the line number. */

സ്ഥിതം ശൂന്യം
print_lineno (ശൂന്യം)
{
  എങ്കിൽ (വരി_no_overflow)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  printf (lineno_format, lineno_width, വരി_no, separator_str);

  എങ്കിൽ (ckd_add (&വരി_no, വരി_no, page_incr))
    വരി_no_overflow = true;
}

സ്ഥിതം ശൂന്യം
reset_lineno (ശൂന്യം)
{
  എങ്കിൽ (reset_numbers)
    {
      വരി_no = starting_വരി_സംഖ്യ;
      വരി_no_overflow = false;
    }
}

/* Switch to a header section. */

സ്ഥിതം ശൂന്യം
proc_header (ശൂന്യം)
{
  current_type = header_type;
  current_regex = &header_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a body section. */

സ്ഥിതം ശൂന്യം
proc_body (ശൂന്യം)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a footer section. */

സ്ഥിതം ശൂന്യം
proc_footer (ശൂന്യം)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Process a regular text line in 'line_buf'. */

സ്ഥിതം ശൂന്യം
proc_text (ശൂന്യം)
{
  സ്ഥിതം intmax_t blank_വരികൾ = 0;	/* Consecutive blank lines so far. */

  തിരഞ്ഞെടുപ്പ് (*current_type)
    {
    അവസ്ഥ 'a':
      എങ്കിൽ (blank_join > 1)
        {
          എങ്കിൽ (1 < വരി_buf.length || ++blank_വരികൾ == blank_join)
            {
              print_lineno ();
              blank_വരികൾ = 0;
            }
          അല്ലെങ്കിൽ
            fputs (print_no_വരി_fmt, stdout);
        }
      അല്ലെങ്കിൽ
        print_lineno ();
      നിർത്തുക;
    അവസ്ഥ 't':
      എങ്കിൽ (1 < വരി_buf.length)
        print_lineno ();
      അല്ലെങ്കിൽ
        fputs (print_no_വരി_fmt, stdout);
      നിർത്തുക;
    അവസ്ഥ 'n':
      fputs (print_no_വരി_fmt, stdout);
      നിർത്തുക;
    അവസ്ഥ 'p':
      തിരഞ്ഞെടുപ്പ് (re_തിരയൽ (current_regex, വരി_buf.buffer, വരി_buf.length - 1,
                         0, വരി_buf.length - 1, ശൂന്യ))
        {
        അവസ്ഥ -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        അവസ്ഥ -1:
          fputs (print_no_വരി_fmt, stdout);
          നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
          print_lineno ();
          നിർത്തുക;
        }
    }
  fwrite (വരി_buf.buffer, വലുപ്പം (അക്ഷരം), വരി_buf.length, stdout);
}

/* Return the type of line in 'line_buf'. */

സ്ഥിതം എണ്ണൽ section
check_section (ശൂന്യം)
{
  size_t len = വരി_buf.length - 1;

  എങ്കിൽ (len < section_del_len || footer_del_len < section_del_len
      || !memeq (വരി_buf.buffer, section_del, section_del_len))
    തിരികെ Text;
  എങ്കിൽ (len == header_del_len
      && memeq (വരി_buf.buffer, header_del, header_del_len))
    തിരികെ Header;
  എങ്കിൽ (len == body_del_len
      && memeq (വരി_buf.buffer, body_del, body_del_len))
    തിരികെ Body;
  എങ്കിൽ (len == footer_del_len
      && memeq (വരി_buf.buffer, footer_del, footer_del_len))
    തിരികെ Footer;
  തിരികെ Text;
}

/* Read and process the file pointed to by FP. */

സ്ഥിതം ശൂന്യം
process_file (FILE *fp)
{
  ആകുമ്പോൾ (readlinebuffer (&വരി_buf, fp))
    {
      തിരഞ്ഞെടുപ്പ് (check_section ())
        {
        അവസ്ഥ Header:
          proc_header ();
          നിർത്തുക;
        അവസ്ഥ Body:
          proc_body ();
          നിർത്തുക;
        അവസ്ഥ Footer:
          proc_footer ();
          നിർത്തുക;
        അവസ്ഥ Text:
          proc_text ();
          നിർത്തുക;
        }

      എങ്കിൽ (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

സ്ഥിതം bool
nl_file (അക്ഷരം സ്ഥിരം *file)
{
  FILE *stream;

  എങ്കിൽ (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  അല്ലെങ്കിൽ
    {
      stream = fopen (file, "r");
      എങ്കിൽ (stream == ശൂന്യ)
        {
          error (0, errno, "%s", quotef (file));
          തിരികെ false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  പൂർണ്ണസംഖ്യ err = errno;
  എങ്കിൽ (!ferror (stream))
    err = 0;
  എങ്കിൽ (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF. */
  അല്ലെങ്കിൽ എങ്കിൽ (fclose (stream) != 0 && !err)
    err = errno;
  എങ്കിൽ (err)
    {
      error (0, err, "%s", quotef (file));
      തിരികെ false;
    }
  തിരികെ true;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'h':
          എങ്കിൽ (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          നിർത്തുക;
        അവസ്ഥ 'b':
          എങ്കിൽ (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          നിർത്തുക;
        അവസ്ഥ 'f':
          എങ്കിൽ (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          നിർത്തുക;
        അവസ്ഥ 'v':
          starting_വരി_സംഖ്യ = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          നിർത്തുക;
        അവസ്ഥ 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          നിർത്തുക;
        അവസ്ഥ 'p':
          reset_numbers = false;
          നിർത്തുക;
        അവസ്ഥ 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          നിർത്തുക;
        അവസ്ഥ 's':
          separator_str = optarg;
          നിർത്തുക;
        അവസ്ഥ 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, INT_MAX, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          നിർത്തുക;
        അവസ്ഥ 'n':
          എങ്കിൽ (streq (optarg, "ln"))
            lineno_format = FORMAT_ഇടത്;
          അല്ലെങ്കിൽ എങ്കിൽ (streq (optarg, "rn"))
            lineno_format = FORMAT_വലത്_NOLZ;
          അല്ലെങ്കിൽ എങ്കിൽ (streq (optarg, "rz"))
            lineno_format = FORMAT_വലത്_LZ;
          അല്ലെങ്കിൽ
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          നിർത്തുക;
        അവസ്ഥ 'd':
          len = strlen (optarg);
          എങ്കിൽ (1 < MB_CUR_MAX)
            {
              അക്ഷരം സ്ഥിരം *p = optarg;
              അക്ഷരം സ്ഥിരം *lim = p + len;
              പൂർണ്ണസംഖ്യ n_chars = 0;
              വേണ്ടി (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              എങ്കിൽ (n_chars == 1)
                memcpy (mempcpy (section_del, optarg, len),  ":", വലുപ്പം ":");
              അല്ലെങ്കിൽ
                section_del = optarg;
            }
          അല്ലെങ്കിൽ
            {
              എങ്കിൽ (len == 1)
                *section_del = *optarg;
              അല്ലെങ്കിൽ
                section_del = optarg;
            }
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          ok = false;
          നിർത്തുക;
        }
    }

  എങ്കിൽ (!ok)
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
  initbuffer (&വരി_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = strlen (separator_str);
  print_no_വരി_fmt = xmalloc (lineno_width + len + 1);
  memset (print_no_വരി_fmt, ' ', lineno_width + len);
  print_no_വരി_fmt[lineno_width + len] = '\0';

  വരി_no = starting_വരി_സംഖ്യ;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  എങ്കിൽ (optind == argc)
    ok = nl_file ("-");
  അല്ലെങ്കിൽ
    വേണ്ടി (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  എങ്കിൽ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
