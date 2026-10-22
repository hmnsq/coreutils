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
#تعریف_کړه پروګرام_NAME "nl"

#تعریف_کړه AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
ساكن حرف ثابت FORMAT_ښی_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
ساكن حرف ثابت FORMAT_ښی_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
ساكن حرف ثابت FORMAT_کیڼ[] = "%-*jd%s";

/* Default section delimiter characters.  */
ساكن حرف DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
تعداد section
{
  سرلیک_2, Body, Footer, متن_2
};

/* Format of body lines (-b).  */
ساكن حرف ثابت *body_type = "t";

/* Format of header lines (-h).  */
ساكن حرف ثابت *سرلیک_type = "n";

/* Format of footer lines (-f).  */
ساكن حرف ثابت *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
ساكن حرف ثابت *current_type;

/* Regex for body lines to number (-bp).  */
ساكن بنية re_نمونه_بفر body_regex;

/* Regex for header lines to number (-hp).  */
ساكن بنية re_نمونه_بفر سرلیک_regex;

/* Regex for footer lines to number (-fp).  */
ساكن بنية re_نمونه_بفر footer_regex;

/* Fastmaps for the above.  */
ساكن حرف body_fastmap[UCHAR_MAX + 1];
ساكن حرف سرلیک_fastmap[UCHAR_MAX + 1];
ساكن حرف footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
ساكن بنية re_نمونه_بفر *current_regex = عدم;

/* Separator string to print after line number (-s).  */
ساكن حرف ثابت *separator_str = "\t";

/* Input section delimiter string (-d).  */
ساكن حرف *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
ساكن size_t section_del_len;

/* Header delimiter string.  */
ساكن حرف *سرلیک_del = عدم;

/* Header section delimiter length.  */
ساكن size_t سرلیک_del_len;

/* Body delimiter string.  */
ساكن حرف *body_del = عدم;

/* Body section delimiter length.  */
ساكن size_t body_del_len;

/* Footer delimiter string.  */
ساكن حرف *footer_del = عدم;

/* Footer section delimiter length.  */
ساكن size_t footer_del_len;

/* Input buffer.  */
ساكن بنية linebuffer کرښه_buf;

/* printf format string for unnumbered lines.  */
ساكن حرف *print_no_کرښه_fmt = عدم;

/* Starting line number on each page (-v).  */
ساكن intmax_t starting_کرښه_شمېره = 1;

/* Line number increment (-i).  */
ساكن intmax_t پاڼه_incr = 1;

/* If true, reset line number at start of each page (-p).  */
ساكن bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
ساكن intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
ساكن صحيح lineno_پلنوالی = 6;

/* Line number format (-n).  */
ساكن حرف ثابت *lineno_format = FORMAT_ښی_NOLZ;

/* Current print line number.  */
ساكن intmax_t کرښه_no;

/* Whether the current line number has incremented past limits.  */
ساكن bool کرښه_no_ډکېدنه;

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin;

ساكن بنية option ثابت longopts[] =
{
  {"header-numbering", required_دلیل, عدم, 'h'},
  {"body-numbering", required_دلیل, عدم, 'b'},
  {"footer-numbering", required_دلیل, عدم, 'f'},
  {"starting-line-number", required_دلیل, عدم, 'v'},
  {"line-increment", required_دلیل, عدم, 'i'},
  {"no-renumber", no_دلیل, عدم, 'p'},
  {"join-blank-lines", required_دلیل, عدم, 'l'},
  {"number-separator", required_دلیل, عدم, 's'},
  {"number-width", required_دلیل, عدم, 'w'},
  {"number-format", required_دلیل, عدم, 'n'},
  {"section-delimiter", required_دلیل, عدم, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Print a usage message and quit. */

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروګرام_name);
      fputs (_("\
Write each FILE منزل standard output, with کرښه numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE لكل numbering body کرښې\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC لكل logical پاڼه delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE لكل numbering footer کرښې\n\
"));
      oputs (_("\
  -h, --سرلیک-numbering=STYLE    use STYLE لكل numbering سرلیک کرښې\n\
"));
      oputs (_("\
  -i, --کرښه-increment=شمېره_2     کرښه شمېره increment at each کرښه\n\
"));
      oputs (_("\
  -l, --join-blank-کرښې=شمېره_2   ډله د شمېره_2 تشه کرښې counted as one\n\
"));
      oputs (_("\
  -n, --شمېره-format=FORMAT      ورننباسه کرښه numbers according منزل FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               افعل not reset کرښه numbers لكل each section\n\
"));
      oputs (_("\
  -s, --شمېره-separator=STRING   جمع_کړه STRING after (possible) کرښه شمېره\n\
"));
      oputs (_("\
  -v, --starting-کرښه-شمېره=شمېره_2  لومړی کرښه شمېره لكل each section\n\
"));
      oputs (_("\
  -w, --شمېره-پلنوالی=شمېره_2       use شمېره_2 columns لكل کرښه numbers\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Default options are: -bt -d'\\:' -fn -hn -i1 -l1 -n'rn' -s<TAB> -v1 -w6\n\
\n\
CC are two delimiter characters کارول_شوی منزل construct logical پاڼه delimiters;\n\
a missing دوهم character پایله_ورکوي ':'.  As a GNU extension one can specify\n\
نور than two characters, and also specifying the تشه string (-d '')\n\
disables section matching.\n\
"), stdout);
      fputs (_("\
\n\
STYLE is one د:\n\
\n\
  a      شمېره all کرښې\n\
  t      شمېره only nonempty کرښې\n\
  n      شمېره no کرښې\n\
  pBRE   شمېره only کرښې that contain a سمون لكل the basic regular\n\
         افاده_2, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one د:\n\
\n\
  ln     کیڼ justified, no leading zeros\n\
  rn     ښی justified, no leading zeros\n\
  rz     ښی justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

ساكن bool
build_type_arg (حرف ثابت **typep,
                بنية re_نمونه_بفر *regexp, حرف *fastmap)
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
      regexp->بفر = عدم;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = عدم;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_تشه_RANGES;
      errmsg = re_compile_نمونه (optarg, مزي_اوږدوالی (optarg), regexp);
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
  إذا (کرښه_no_ډکېدنه)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  چاپ_کړه (lineno_format, lineno_پلنوالی, کرښه_no, separator_str);

  إذا (ckd_جمع_کړه (&کرښه_no, کرښه_no, پاڼه_incr))
    کرښه_no_ډکېدنه = true;
}

ساكن فراغ
reset_lineno (فراغ)
{
  إذا (reset_numbers)
    {
      کرښه_no = starting_کرښه_شمېره;
      کرښه_no_ډکېدنه = false;
    }
}

/* Switch to a header section. */

ساكن فراغ
proc_سرلیک (فراغ)
{
  current_type = سرلیک_type;
  current_regex = &سرلیک_regex;
  reset_lineno ();
  توری_چاپ_کړه ('\n');
}

/* Switch to a body section. */

ساكن فراغ
proc_body (فراغ)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  توری_چاپ_کړه ('\n');
}

/* Switch to a footer section. */

ساكن فراغ
proc_footer (فراغ)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  توری_چاپ_کړه ('\n');
}

/* Process a regular text line in 'line_buf'. */

ساكن فراغ
proc_متن (فراغ)
{
  ساكن intmax_t blank_کرښې = 0;	/* Consecutive blank lines so far. */

  بدل (*current_type)
    {
    حالة 'a':
      إذا (blank_join > 1)
        {
          إذا (1 < کرښه_buf.اوږدتیا || ++blank_کرښې == blank_join)
            {
              print_lineno ();
              blank_کرښې = 0;
            }
          وإلا
            fputs (print_no_کرښه_fmt, stdout);
        }
      وإلا
        print_lineno ();
      اكسر;
    حالة 't':
      إذا (1 < کرښه_buf.اوږدتیا)
        print_lineno ();
      وإلا
        fputs (print_no_کرښه_fmt, stdout);
      اكسر;
    حالة 'n':
      fputs (print_no_کرښه_fmt, stdout);
      اكسر;
    حالة 'p':
      بدل (re_پلټنه (current_regex, کرښه_buf.بفر, کرښه_buf.اوږدتیا - 1,
                         0, کرښه_buf.اوږدتیا - 1, عدم))
        {
        حالة -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        حالة -1:
          fputs (print_no_کرښه_fmt, stdout);
          اكسر;

        افتراضي:
          print_lineno ();
          اكسر;
        }
    }
  fwrite (کرښه_buf.بفر, حجم (حرف), کرښه_buf.اوږدتیا, stdout);
}

/* Return the type of line in 'line_buf'. */

ساكن تعداد section
check_section (فراغ)
{
  size_t len = کرښه_buf.اوږدتیا - 1;

  إذا (len < section_del_len || footer_del_len < section_del_len
      || !memeq (کرښه_buf.بفر, section_del, section_del_len))
    أرجع متن_2;
  إذا (len == سرلیک_del_len
      && memeq (کرښه_buf.بفر, سرلیک_del, سرلیک_del_len))
    أرجع سرلیک_2;
  إذا (len == body_del_len
      && memeq (کرښه_buf.بفر, body_del, body_del_len))
    أرجع Body;
  إذا (len == footer_del_len
      && memeq (کرښه_buf.بفر, footer_del, footer_del_len))
    أرجع Footer;
  أرجع متن_2;
}

/* Read and process the file pointed to by FP. */

ساكن فراغ
process_file (FILE *fp)
{
  طالما (readlinebuffer (&کرښه_buf, fp))
    {
      بدل (check_section ())
        {
        حالة سرلیک_2:
          proc_سرلیک ();
          اكسر;
        حالة Body:
          proc_body ();
          اكسر;
        حالة Footer:
          proc_footer ();
          اكسر;
        حالة متن_2:
          proc_متن ();
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
  FILE *بهیر;

  إذا (streq (file, "-"))
    {
      have_read_stdin = true;
      بهیر = stdin;
      assume (بهیر);  /* Pacify GCC bug#109613.  */
    }
  وإلا
    {
      بهیر = fopen (file, "r");
      إذا (بهیر == عدم)
        {
          error (0, errno, "%s", quotef (file));
          أرجع false;
        }
    }

  fadvise (بهیر, FADVISE_SEQUENTIAL);

  process_file (بهیر);

  صحيح err = errno;
  إذا (!ferror (بهیر))
    err = 0;
  إذا (streq (file, "-"))
    clearerr (بهیر);		/* Also clear EOF. */
  وإلا إذا (fclose (بهیر) != 0 && !err)
    err = errno;
  إذا (err)
    {
      error (0, err, "%s", quotef (file));
      أرجع false;
    }
  أرجع true;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  صحيح c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
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
          إذا (! build_type_arg (&سرلیک_type, &سرلیک_regex, سرلیک_fastmap))
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
          starting_کرښه_شمېره = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          اكسر;
        حالة 'i':
          پاڼه_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
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
          lineno_پلنوالی = xnumtoimax (optarg, 10, 1, لوی_عدد, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          اكسر;
        حالة 'n':
          إذا (streq (optarg, "ln"))
            lineno_format = FORMAT_کیڼ;
          وإلا إذا (streq (optarg, "rn"))
            lineno_format = FORMAT_ښی_NOLZ;
          وإلا إذا (streq (optarg, "rz"))
            lineno_format = FORMAT_ښی_LZ;
          وإلا
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          اكسر;
        حالة 'd':
          len = مزي_اوږدوالی (optarg);
          إذا (1 < MB_CUR_MAX)
            {
              حرف ثابت *p = optarg;
              حرف ثابت *lim = p + len;
              صحيح n_chars = 0;
              لكل (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_لټون (p, lim).len;
              إذا (n_chars == 1)
                حافظه_نقل (mempcpy (section_del, optarg, len),  ":", حجم ":");
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
        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);
        افتراضي:
          ok = false;
          اكسر;
        }
    }

  إذا (!ok)
    usage (EXIT_FAILURE);

  /* Initialize the section delimiters.  */
  section_del_len = len = مزي_اوږدوالی (section_del);

  سرلیک_del_len = len * 3;
  سرلیک_del = xmalloc (سرلیک_del_len + 1);
  stpcpy (stpcpy (stpcpy (سرلیک_del, section_del), section_del), section_del);

  body_del_len = len * 2;
  body_del = سرلیک_del + len;

  footer_del_len = len;
  footer_del = body_del + len;

  /* Initialize the input buffer.  */
  initbuffer (&کرښه_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = مزي_اوږدوالی (separator_str);
  print_no_کرښه_fmt = xmalloc (lineno_پلنوالی + len + 1);
  memset (print_no_کرښه_fmt, ' ', lineno_پلنوالی + len);
  print_no_کرښه_fmt[lineno_پلنوالی + len] = '\0';

  کرښه_no = starting_کرښه_شمېره;
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
