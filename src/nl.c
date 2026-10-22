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
#وصف_ڪريو پروگرام_NAME "nl"

#وصف_ڪريو AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
ساكن حرف ثابت FORMAT_ساڄو_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
ساكن حرف ثابت FORMAT_ساڄو_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
ساكن حرف ثابت FORMAT_کاٻو[] = "%-*jd%s";

/* Default section delimiter characters.  */
ساكن حرف DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
تعداد section
{
  سرو_2, Body, Footer, متن_2
};

/* Format of body lines (-b).  */
ساكن حرف ثابت *body_type = "t";

/* Format of header lines (-h).  */
ساكن حرف ثابت *سرو_type = "n";

/* Format of footer lines (-f).  */
ساكن حرف ثابت *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
ساكن حرف ثابت *current_type;

/* Regex for body lines to number (-bp).  */
ساكن بنية re_نمونو_بفر body_regex;

/* Regex for header lines to number (-hp).  */
ساكن بنية re_نمونو_بفر سرو_regex;

/* Regex for footer lines to number (-fp).  */
ساكن بنية re_نمونو_بفر footer_regex;

/* Fastmaps for the above.  */
ساكن حرف body_fastmap[UCHAR_MAX + 1];
ساكن حرف سرو_fastmap[UCHAR_MAX + 1];
ساكن حرف footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
ساكن بنية re_نمونو_بفر *current_regex = عدم;

/* Separator string to print after line number (-s).  */
ساكن حرف ثابت *separator_str = "\t";

/* Input section delimiter string (-d).  */
ساكن حرف *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
ساكن size_t section_del_len;

/* Header delimiter string.  */
ساكن حرف *سرو_del = عدم;

/* Header section delimiter length.  */
ساكن size_t سرو_del_len;

/* Body delimiter string.  */
ساكن حرف *body_del = عدم;

/* Body section delimiter length.  */
ساكن size_t body_del_len;

/* Footer delimiter string.  */
ساكن حرف *footer_del = عدم;

/* Footer section delimiter length.  */
ساكن size_t footer_del_len;

/* Input buffer.  */
ساكن بنية linebuffer سٽ_buf;

/* printf format string for unnumbered lines.  */
ساكن حرف *print_no_سٽ_fmt = عدم;

/* Starting line number on each page (-v).  */
ساكن intmax_t starting_سٽ_عدد = 1;

/* Line number increment (-i).  */
ساكن intmax_t صفحو_incr = 1;

/* If true, reset line number at start of each page (-p).  */
ساكن bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
ساكن intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
ساكن صحيح lineno_ويڪر = 6;

/* Line number format (-n).  */
ساكن حرف ثابت *lineno_format = FORMAT_ساڄو_NOLZ;

/* Current print line number.  */
ساكن intmax_t سٽ_no;

/* Whether the current line number has incremented past limits.  */
ساكن bool سٽ_no_حد_کان_مٿي;

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin;

ساكن بنية option ثابت longopts[] =
{
  {"header-numbering", required_دليل, عدم, 'h'},
  {"body-numbering", required_دليل, عدم, 'b'},
  {"footer-numbering", required_دليل, عدم, 'f'},
  {"starting-line-number", required_دليل, عدم, 'v'},
  {"line-increment", required_دليل, عدم, 'i'},
  {"no-renumber", no_دليل, عدم, 'p'},
  {"join-blank-lines", required_دليل, عدم, 'l'},
  {"number-separator", required_دليل, عدم, 's'},
  {"number-width", required_دليل, عدم, 'w'},
  {"number-format", required_دليل, عدم, 'n'},
  {"section-delimiter", required_دليل, عدم, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
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
      لکو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Write each FILE منزل standard output, with سٽ numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE لكل numbering body سٽون\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC لكل logical صفحو delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE لكل numbering footer سٽون\n\
"));
      oputs (_("\
  -h, --سرو-numbering=STYLE    use STYLE لكل numbering سرو سٽون\n\
"));
      oputs (_("\
  -i, --سٽ-increment=عدد_2     سٽ عدد increment at each سٽ\n\
"));
      oputs (_("\
  -l, --join-blank-سٽون=عدد_2   گروهه جو عدد_2 خالي_جايون سٽون counted as one\n\
"));
      oputs (_("\
  -n, --عدد-format=FORMAT      وجهو سٽ numbers according منزل FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               افعل not reset سٽ numbers لكل each section\n\
"));
      oputs (_("\
  -s, --عدد-separator=STRING   جوڙيو STRING after (possible) سٽ عدد\n\
"));
      oputs (_("\
  -v, --starting-سٽ-عدد=عدد_2  پهريون سٽ عدد لكل each section\n\
"));
      oputs (_("\
  -w, --عدد-ويڪر=عدد_2       use عدد_2 columns لكل سٽ numbers\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Default options are: -bt -d'\\:' -fn -hn -i1 -l1 -n'rn' -s<TAB> -v1 -w6\n\
\n\
CC are two delimiter characters استعمال_ٿيل منزل construct logical صفحو delimiters;\n\
a missing ٻيو character نتيجو_ڏئي ':'.  As a GNU extension one can specify\n\
وڌيڪ than two characters, and also specifying the خالي_جايون string (-d '')\n\
disables section matching.\n\
"), stdout);
      fputs (_("\
\n\
STYLE is one جو:\n\
\n\
  a      عدد all سٽون\n\
  t      عدد only nonempty سٽون\n\
  n      عدد no سٽون\n\
  pBRE   عدد only سٽون that contain a ميلاپ لكل the basic regular\n\
         اظهار_2, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one جو:\n\
\n\
  ln     کاٻو justified, no leading zeros\n\
  rn     ساڄو justified, no leading zeros\n\
  rz     ساڄو justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

ساكن bool
build_type_arg (حرف ثابت **typep,
                بنية re_نمونو_بفر *regexp, حرف *fastmap)
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
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_خالي_جايون_RANGES;
      errmsg = re_compile_نمونو (optarg, لڙي_ڊيگهه (optarg), regexp);
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
  إذا (سٽ_no_حد_کان_مٿي)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  لکو (lineno_format, lineno_ويڪر, سٽ_no, separator_str);

  إذا (ckd_جوڙيو (&سٽ_no, سٽ_no, صفحو_incr))
    سٽ_no_حد_کان_مٿي = true;
}

ساكن فراغ
reset_lineno (فراغ)
{
  إذا (reset_numbers)
    {
      سٽ_no = starting_سٽ_عدد;
      سٽ_no_حد_کان_مٿي = false;
    }
}

/* Switch to a header section. */

ساكن فراغ
proc_سرو (فراغ)
{
  current_type = سرو_type;
  current_regex = &سرو_regex;
  reset_lineno ();
  اکر_لکو ('\n');
}

/* Switch to a body section. */

ساكن فراغ
proc_body (فراغ)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  اکر_لکو ('\n');
}

/* Switch to a footer section. */

ساكن فراغ
proc_footer (فراغ)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  اکر_لکو ('\n');
}

/* Process a regular text line in 'line_buf'. */

ساكن فراغ
proc_متن (فراغ)
{
  ساكن intmax_t blank_سٽون = 0;	/* Consecutive blank lines so far. */

  بدل (*current_type)
    {
    حالة 'a':
      إذا (blank_join > 1)
        {
          إذا (1 < سٽ_buf.ڊيگهائي || ++blank_سٽون == blank_join)
            {
              print_lineno ();
              blank_سٽون = 0;
            }
          وإلا
            fputs (print_no_سٽ_fmt, stdout);
        }
      وإلا
        print_lineno ();
      اكسر;
    حالة 't':
      إذا (1 < سٽ_buf.ڊيگهائي)
        print_lineno ();
      وإلا
        fputs (print_no_سٽ_fmt, stdout);
      اكسر;
    حالة 'n':
      fputs (print_no_سٽ_fmt, stdout);
      اكسر;
    حالة 'p':
      بدل (re_ڳولا (current_regex, سٽ_buf.بفر, سٽ_buf.ڊيگهائي - 1,
                         0, سٽ_buf.ڊيگهائي - 1, عدم))
        {
        حالة -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        حالة -1:
          fputs (print_no_سٽ_fmt, stdout);
          اكسر;

        افتراضي:
          print_lineno ();
          اكسر;
        }
    }
  fwrite (سٽ_buf.بفر, حجم (حرف), سٽ_buf.ڊيگهائي, stdout);
}

/* Return the type of line in 'line_buf'. */

ساكن تعداد section
check_section (فراغ)
{
  size_t len = سٽ_buf.ڊيگهائي - 1;

  إذا (len < section_del_len || footer_del_len < section_del_len
      || !memeq (سٽ_buf.بفر, section_del, section_del_len))
    أرجع متن_2;
  إذا (len == سرو_del_len
      && memeq (سٽ_buf.بفر, سرو_del, سرو_del_len))
    أرجع سرو_2;
  إذا (len == body_del_len
      && memeq (سٽ_buf.بفر, body_del, body_del_len))
    أرجع Body;
  إذا (len == footer_del_len
      && memeq (سٽ_buf.بفر, footer_del, footer_del_len))
    أرجع Footer;
  أرجع متن_2;
}

/* Read and process the file pointed to by FP. */

ساكن فراغ
process_file (FILE *fp)
{
  طالما (readlinebuffer (&سٽ_buf, fp))
    {
      بدل (check_section ())
        {
        حالة سرو_2:
          proc_سرو ();
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
  FILE *وهڪرو;

  إذا (streq (file, "-"))
    {
      have_read_stdin = true;
      وهڪرو = stdin;
      assume (وهڪرو);  /* Pacify GCC bug#109613.  */
    }
  وإلا
    {
      وهڪرو = fopen (file, "r");
      إذا (وهڪرو == عدم)
        {
          error (0, errno, "%s", quotef (file));
          أرجع false;
        }
    }

  fadvise (وهڪرو, FADVISE_SEQUENTIAL);

  process_file (وهڪرو);

  صحيح err = errno;
  إذا (!ferror (وهڪرو))
    err = 0;
  إذا (streq (file, "-"))
    clearerr (وهڪرو);		/* Also clear EOF. */
  وإلا إذا (fclose (وهڪرو) != 0 && !err)
    err = errno;
  إذا (err)
    {
      error (0, err, "%s", quotef (file));
      أرجع false;
    }
  أرجع true;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
          إذا (! build_type_arg (&سرو_type, &سرو_regex, سرو_fastmap))
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
          starting_سٽ_عدد = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          اكسر;
        حالة 'i':
          صفحو_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
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
          lineno_ويڪر = xnumtoimax (optarg, 10, 1, وڌ_کان_وڌ_عدد, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          اكسر;
        حالة 'n':
          إذا (streq (optarg, "ln"))
            lineno_format = FORMAT_کاٻو;
          وإلا إذا (streq (optarg, "rn"))
            lineno_format = FORMAT_ساڄو_NOLZ;
          وإلا إذا (streq (optarg, "rz"))
            lineno_format = FORMAT_ساڄو_LZ;
          وإلا
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          اكسر;
        حالة 'd':
          len = لڙي_ڊيگهه (optarg);
          إذا (1 < MB_CUR_MAX)
            {
              حرف ثابت *p = optarg;
              حرف ثابت *lim = p + len;
              صحيح n_chars = 0;
              لكل (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_جاچ (p, lim).len;
              إذا (n_chars == 1)
                يادگيري_نقل (mempcpy (section_del, optarg, len),  ":", حجم ":");
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
        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          ok = false;
          اكسر;
        }
    }

  إذا (!ok)
    usage (EXIT_FAILURE);

  /* Initialize the section delimiters.  */
  section_del_len = len = لڙي_ڊيگهه (section_del);

  سرو_del_len = len * 3;
  سرو_del = xmalloc (سرو_del_len + 1);
  stpcpy (stpcpy (stpcpy (سرو_del, section_del), section_del), section_del);

  body_del_len = len * 2;
  body_del = سرو_del + len;

  footer_del_len = len;
  footer_del = body_del + len;

  /* Initialize the input buffer.  */
  initbuffer (&سٽ_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = لڙي_ڊيگهه (separator_str);
  print_no_سٽ_fmt = xmalloc (lineno_ويڪر + len + 1);
  memset (print_no_سٽ_fmt, ' ', lineno_ويڪر + len);
  print_no_سٽ_fmt[lineno_ويڪر + len] = '\0';

  سٽ_no = starting_سٽ_عدد;
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
