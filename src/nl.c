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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <getopt.h>

#அந்தர்பாவ "system.h"

#அந்தர்பாவ <regex.h>

#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "linebuffer.h"
#அந்தர்பாவ "mcel.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nl"

#define AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
நிலையான எழுத்து மாறிலி FORMAT_வலது_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
நிலையான எழுத்து மாறிலி FORMAT_வலது_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
நிலையான எழுத்து மாறிலி FORMAT_இடது[] = "%-*jd%s";

/* Default section delimiter characters.  */
நிலையான எழுத்து DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
எண்ணிக்கை section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
நிலையான எழுத்து மாறிலி *body_type = "t";

/* Format of header lines (-h).  */
நிலையான எழுத்து மாறிலி *header_type = "n";

/* Format of footer lines (-f).  */
நிலையான எழுத்து மாறிலி *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
நிலையான எழுத்து மாறிலி *current_type;

/* Regex for body lines to number (-bp).  */
நிலையான கட்டமைப்பு re_pattern_buffer body_regex;

/* Regex for header lines to number (-hp).  */
நிலையான கட்டமைப்பு re_pattern_buffer header_regex;

/* Regex for footer lines to number (-fp).  */
நிலையான கட்டமைப்பு re_pattern_buffer footer_regex;

/* Fastmaps for the above.  */
நிலையான எழுத்து body_fastmap[UCHAR_MAX + 1];
நிலையான எழுத்து header_fastmap[UCHAR_MAX + 1];
நிலையான எழுத்து footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
நிலையான கட்டமைப்பு re_pattern_buffer *current_regex = வெற்று;

/* Separator string to print after line number (-s).  */
நிலையான எழுத்து மாறிலி *separator_str = "\t";

/* Input section delimiter string (-d).  */
நிலையான எழுத்து *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
நிலையான size_t section_del_len;

/* Header delimiter string.  */
நிலையான எழுத்து *header_del = வெற்று;

/* Header section delimiter length.  */
நிலையான size_t header_del_len;

/* Body delimiter string.  */
நிலையான எழுத்து *body_del = வெற்று;

/* Body section delimiter length.  */
நிலையான size_t body_del_len;

/* Footer delimiter string.  */
நிலையான எழுத்து *footer_del = வெற்று;

/* Footer section delimiter length.  */
நிலையான size_t footer_del_len;

/* Input buffer.  */
நிலையான கட்டமைப்பு linebuffer வரி_buf;

/* printf format string for unnumbered lines.  */
நிலையான எழுத்து *print_no_வரி_fmt = வெற்று;

/* Starting line number on each page (-v).  */
நிலையான intmax_t starting_வரி_எண் = 1;

/* Line number increment (-i).  */
நிலையான intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
நிலையான bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
நிலையான intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
நிலையான முழு lineno_width = 6;

/* Line number format (-n).  */
நிலையான எழுத்து மாறிலி *lineno_format = FORMAT_வலது_NOLZ;

/* Current print line number.  */
நிலையான intmax_t வரி_no;

/* Whether the current line number has incremented past limits.  */
நிலையான bool வரி_no_overflow;

/* True if we have ever read standard input.  */
நிலையான bool have_read_stdin;

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"header-numbering", required_argument, வெற்று, 'h'},
  {"body-numbering", required_argument, வெற்று, 'b'},
  {"footer-numbering", required_argument, வெற்று, 'f'},
  {"starting-line-number", required_argument, வெற்று, 'v'},
  {"line-increment", required_argument, வெற்று, 'i'},
  {"no-renumber", no_argument, வெற்று, 'p'},
  {"join-blank-lines", required_argument, வெற்று, 'l'},
  {"number-separator", required_argument, வெற்று, 's'},
  {"number-width", required_argument, வெற்று, 'w'},
  {"number-format", required_argument, வெற்று, 'n'},
  {"section-delimiter", required_argument, வெற்று, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Print a usage message and quit. */

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, with வரி numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE க்கு numbering body வரிகள்\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC க்கு logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE க்கு numbering footer வரிகள்\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE க்கு numbering header வரிகள்\n\
"));
      oputs (_("\
  -i, --வரி-increment=எண்_2     வரி எண் increment at each வரி\n\
"));
      oputs (_("\
  -l, --join-blank-வரிகள்=எண்_2   group of எண்_2 empty வரிகள் counted as one\n\
"));
      oputs (_("\
  -n, --எண்-format=FORMAT      செருகு வரி numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               செய் not reset வரி numbers க்கு each section\n\
"));
      oputs (_("\
  -s, --எண்-separator=STRING   add STRING after (possible) வரி எண்\n\
"));
      oputs (_("\
  -v, --starting-வரி-எண்=எண்_2  first வரி எண் க்கு each section\n\
"));
      oputs (_("\
  -w, --எண்-width=எண்_2       use எண்_2 columns க்கு வரி numbers\n\
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
  a      எண் all வரிகள்\n\
  t      எண் only nonempty வரிகள்\n\
  n      எண் no வரிகள்\n\
  pBRE   எண் only வரிகள் that contain a match க்கு the basic regular\n\
         expression, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     இடது justified, no leading zeros\n\
  rn     வலது justified, no leading zeros\n\
  rz     வலது justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

நிலையான bool
build_type_arg (எழுத்து மாறிலி **typep,
                கட்டமைப்பு re_pattern_buffer *regexp, எழுத்து *fastmap)
{
  எழுத்து மாறிலி *errmsg;
  bool rval = true;

  தேர்வு (*optarg)
    {
    நிலை 'a':
    நிலை 't':
    நிலை 'n':
      *typep = optarg;
      நிறுத்து;
    நிலை 'p':
      *typep = optarg++;
      regexp->buffer = வெற்று;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = வெற்று;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, strlen (optarg), regexp);
      எனில் (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      நிறுத்து;
    இயல்பு:
      rval = false;
      நிறுத்து;
    }
  திரும்பு rval;
}

/* Print the line number and separator; increment the line number. */

நிலையான வெறுமை
print_lineno (வெறுமை)
{
  எனில் (வரி_no_overflow)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  printf (lineno_format, lineno_width, வரி_no, separator_str);

  எனில் (ckd_add (&வரி_no, வரி_no, page_incr))
    வரி_no_overflow = true;
}

நிலையான வெறுமை
reset_lineno (வெறுமை)
{
  எனில் (reset_numbers)
    {
      வரி_no = starting_வரி_எண்;
      வரி_no_overflow = false;
    }
}

/* Switch to a header section. */

நிலையான வெறுமை
proc_header (வெறுமை)
{
  current_type = header_type;
  current_regex = &header_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a body section. */

நிலையான வெறுமை
proc_body (வெறுமை)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Switch to a footer section. */

நிலையான வெறுமை
proc_footer (வெறுமை)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  putchar ('\n');
}

/* Process a regular text line in 'line_buf'. */

நிலையான வெறுமை
proc_text (வெறுமை)
{
  நிலையான intmax_t blank_வரிகள் = 0;	/* Consecutive blank lines so far. */

  தேர்வு (*current_type)
    {
    நிலை 'a':
      எனில் (blank_join > 1)
        {
          எனில் (1 < வரி_buf.length || ++blank_வரிகள் == blank_join)
            {
              print_lineno ();
              blank_வரிகள் = 0;
            }
          இல்லையெனில்
            fputs (print_no_வரி_fmt, stdout);
        }
      இல்லையெனில்
        print_lineno ();
      நிறுத்து;
    நிலை 't':
      எனில் (1 < வரி_buf.length)
        print_lineno ();
      இல்லையெனில்
        fputs (print_no_வரி_fmt, stdout);
      நிறுத்து;
    நிலை 'n':
      fputs (print_no_வரி_fmt, stdout);
      நிறுத்து;
    நிலை 'p':
      தேர்வு (re_தேடல் (current_regex, வரி_buf.buffer, வரி_buf.length - 1,
                         0, வரி_buf.length - 1, வெற்று))
        {
        நிலை -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        நிலை -1:
          fputs (print_no_வரி_fmt, stdout);
          நிறுத்து;

        இயல்பு:
          print_lineno ();
          நிறுத்து;
        }
    }
  fwrite (வரி_buf.buffer, அளவு (எழுத்து), வரி_buf.length, stdout);
}

/* Return the type of line in 'line_buf'. */

நிலையான எண்ணிக்கை section
check_section (வெறுமை)
{
  size_t len = வரி_buf.length - 1;

  எனில் (len < section_del_len || footer_del_len < section_del_len
      || !memeq (வரி_buf.buffer, section_del, section_del_len))
    திரும்பு Text;
  எனில் (len == header_del_len
      && memeq (வரி_buf.buffer, header_del, header_del_len))
    திரும்பு Header;
  எனில் (len == body_del_len
      && memeq (வரி_buf.buffer, body_del, body_del_len))
    திரும்பு Body;
  எனில் (len == footer_del_len
      && memeq (வரி_buf.buffer, footer_del, footer_del_len))
    திரும்பு Footer;
  திரும்பு Text;
}

/* Read and process the file pointed to by FP. */

நிலையான வெறுமை
process_file (FILE *fp)
{
  போது (readlinebuffer (&வரி_buf, fp))
    {
      தேர்வு (check_section ())
        {
        நிலை Header:
          proc_header ();
          நிறுத்து;
        நிலை Body:
          proc_body ();
          நிறுத்து;
        நிலை Footer:
          proc_footer ();
          நிறுத்து;
        நிலை Text:
          proc_text ();
          நிறுத்து;
        }

      எனில் (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

நிலையான bool
nl_file (எழுத்து மாறிலி *file)
{
  FILE *stream;

  எனில் (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  இல்லையெனில்
    {
      stream = fopen (file, "r");
      எனில் (stream == வெற்று)
        {
          error (0, errno, "%s", quotef (file));
          திரும்பு false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  முழு err = errno;
  எனில் (!ferror (stream))
    err = 0;
  எனில் (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF. */
  இல்லையெனில் எனில் (fclose (stream) != 0 && !err)
    err = errno;
  எனில் (err)
    {
      error (0, err, "%s", quotef (file));
      திரும்பு false;
    }
  திரும்பு true;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           வெற்று))
         != -1)
    {
      தேர்வு (c)
        {
        நிலை 'h':
          எனில் (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          நிறுத்து;
        நிலை 'b':
          எனில் (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          நிறுத்து;
        நிலை 'f':
          எனில் (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          நிறுத்து;
        நிலை 'v':
          starting_வரி_எண் = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          நிறுத்து;
        நிலை 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          நிறுத்து;
        நிலை 'p':
          reset_numbers = false;
          நிறுத்து;
        நிலை 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          நிறுத்து;
        நிலை 's':
          separator_str = optarg;
          நிறுத்து;
        நிலை 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, INT_MAX, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          நிறுத்து;
        நிலை 'n':
          எனில் (streq (optarg, "ln"))
            lineno_format = FORMAT_இடது;
          இல்லையெனில் எனில் (streq (optarg, "rn"))
            lineno_format = FORMAT_வலது_NOLZ;
          இல்லையெனில் எனில் (streq (optarg, "rz"))
            lineno_format = FORMAT_வலது_LZ;
          இல்லையெனில்
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          நிறுத்து;
        நிலை 'd':
          len = strlen (optarg);
          எனில் (1 < MB_CUR_MAX)
            {
              எழுத்து மாறிலி *p = optarg;
              எழுத்து மாறிலி *lim = p + len;
              முழு n_chars = 0;
              க்கு (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              எனில் (n_chars == 1)
                memcpy (mempcpy (section_del, optarg, len),  ":", அளவு ":");
              இல்லையெனில்
                section_del = optarg;
            }
          இல்லையெனில்
            {
              எனில் (len == 1)
                *section_del = *optarg;
              இல்லையெனில்
                section_del = optarg;
            }
          நிறுத்து;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          ok = false;
          நிறுத்து;
        }
    }

  எனில் (!ok)
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
  initbuffer (&வரி_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = strlen (separator_str);
  print_no_வரி_fmt = xmalloc (lineno_width + len + 1);
  memset (print_no_வரி_fmt, ' ', lineno_width + len);
  print_no_வரி_fmt[lineno_width + len] = '\0';

  வரி_no = starting_வரி_எண்;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  எனில் (optind == argc)
    ok = nl_file ("-");
  இல்லையெனில்
    க்கு (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  எனில் (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
