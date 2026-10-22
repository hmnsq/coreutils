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

#شامل <config.h>

#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <getopt.h>

#شامل "system.h"

#شامل <regex.h>

#شامل "fadvise.h"
#شامل "linebuffer.h"
#شامل "mcel.h"
#شامل "quote.h"
#شامل "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "nl"

#تعریف_کرو AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
ساکن حرف ثابت FORMAT_دایاں_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
ساکن حرف ثابت FORMAT_دایاں_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
ساکن حرف ثابت FORMAT_بایاں[] = "%-*jd%s";

/* Default section delimiter characters.  */
ساکن حرف DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
شمار section
{
  سرخی_2, Body, Footer, متن_2
};

/* Format of body lines (-b).  */
ساکن حرف ثابت *body_type = "t";

/* Format of header lines (-h).  */
ساکن حرف ثابت *سرخی_type = "n";

/* Format of footer lines (-f).  */
ساکن حرف ثابت *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
ساکن حرف ثابت *current_type;

/* Regex for body lines to number (-bp).  */
ساکن ساخت re_نمونہ_بفر body_regex;

/* Regex for header lines to number (-hp).  */
ساکن ساخت re_نمونہ_بفر سرخی_regex;

/* Regex for footer lines to number (-fp).  */
ساکن ساخت re_نمونہ_بفر footer_regex;

/* Fastmaps for the above.  */
ساکن حرف body_fastmap[UCHAR_MAX + 1];
ساکن حرف سرخی_fastmap[UCHAR_MAX + 1];
ساکن حرف footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
ساکن ساخت re_نمونہ_بفر *current_regex = تہی;

/* Separator string to print after line number (-s).  */
ساکن حرف ثابت *separator_str = "\t";

/* Input section delimiter string (-d).  */
ساکن حرف *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
ساکن size_t section_del_len;

/* Header delimiter string.  */
ساکن حرف *سرخی_del = تہی;

/* Header section delimiter length.  */
ساکن size_t سرخی_del_len;

/* Body delimiter string.  */
ساکن حرف *body_del = تہی;

/* Body section delimiter length.  */
ساکن size_t body_del_len;

/* Footer delimiter string.  */
ساکن حرف *footer_del = تہی;

/* Footer section delimiter length.  */
ساکن size_t footer_del_len;

/* Input buffer.  */
ساکن ساخت linebuffer سطر_buf;

/* printf format string for unnumbered lines.  */
ساکن حرف *print_no_سطر_fmt = تہی;

/* Starting line number on each page (-v).  */
ساکن intmax_t starting_سطر_نمبر = 1;

/* Line number increment (-i).  */
ساکن intmax_t صفحہ_incr = 1;

/* If true, reset line number at start of each page (-p).  */
ساکن bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
ساکن intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
ساکن صحیح lineno_چوڑائی = 6;

/* Line number format (-n).  */
ساکن حرف ثابت *lineno_format = FORMAT_دایاں_NOLZ;

/* Current print line number.  */
ساکن intmax_t سطر_no;

/* Whether the current line number has incremented past limits.  */
ساکن bool سطر_no_حد_سے_تجاوز;

/* True if we have ever read standard input.  */
ساکن bool have_read_stdin;

ساکن ساخت option ثابت longopts[] =
{
  {"header-numbering", required_دلیل, تہی, 'h'},
  {"body-numbering", required_دلیل, تہی, 'b'},
  {"footer-numbering", required_دلیل, تہی, 'f'},
  {"starting-line-number", required_دلیل, تہی, 'v'},
  {"line-increment", required_دلیل, تہی, 'i'},
  {"no-renumber", no_دلیل, تہی, 'p'},
  {"join-blank-lines", required_دلیل, تہی, 'l'},
  {"number-separator", required_دلیل, تہی, 's'},
  {"number-width", required_دلیل, تہی, 'w'},
  {"number-format", required_دلیل, تہی, 'n'},
  {"section-delimiter", required_دلیل, تہی, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Print a usage message and quit. */

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Write each FILE مقصد standard output, with سطر numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE برائے numbering body سطریں\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC برائے logical صفحہ delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE برائے numbering footer سطریں\n\
"));
      oputs (_("\
  -h, --سرخی-numbering=STYLE    use STYLE برائے numbering سرخی سطریں\n\
"));
      oputs (_("\
  -i, --سطر-increment=نمبر_2     سطر نمبر increment at each سطر\n\
"));
      oputs (_("\
  -l, --join-blank-سطریں=نمبر_2   گروہ کا نمبر_2 خالی_خانے سطریں counted as one\n\
"));
      oputs (_("\
  -n, --نمبر-format=FORMAT      داخل_کرو سطر numbers according مقصد FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               کرو not reset سطر numbers برائے each section\n\
"));
      oputs (_("\
  -s, --نمبر-separator=STRING   جوڑو STRING after (possible) سطر نمبر\n\
"));
      oputs (_("\
  -v, --starting-سطر-نمبر=نمبر_2  پہلا سطر نمبر برائے each section\n\
"));
      oputs (_("\
  -w, --نمبر-چوڑائی=نمبر_2       use نمبر_2 columns برائے سطر numbers\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Default options are: -bt -d'\\:' -fn -hn -i1 -l1 -n'rn' -s<TAB> -v1 -w6\n\
\n\
CC are two delimiter characters مستعمل مقصد construct logical صفحہ delimiters;\n\
a missing دوسرا character مستلزم ':'.  As a GNU extension one can specify\n\
مزید than two characters, and also specifying the خالی_خانے string (-d '')\n\
disables section matching.\n\
"), stdout);
      fputs (_("\
\n\
STYLE is one کا:\n\
\n\
  a      نمبر all سطریں\n\
  t      نمبر only nonempty سطریں\n\
  n      نمبر no سطریں\n\
  pBRE   نمبر only سطریں that contain a مطابقت برائے the basic regular\n\
         عبارت_2, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one کا:\n\
\n\
  ln     بایاں justified, no leading zeros\n\
  rn     دایاں justified, no leading zeros\n\
  rz     دایاں justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

ساکن bool
build_type_arg (حرف ثابت **typep,
                ساخت re_نمونہ_بفر *regexp, حرف *fastmap)
{
  حرف ثابت *errmsg;
  bool rval = true;

  انتخاب (*optarg)
    {
    حالت 'a':
    حالت 't':
    حالت 'n':
      *typep = optarg;
      توڑو;
    حالت 'p':
      *typep = optarg++;
      regexp->بفر = تہی;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = تہی;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_خالی_خانے_RANGES;
      errmsg = re_compile_نمونہ (optarg, لڑی_لمبائی (optarg), regexp);
      اگر (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      توڑو;
    طےشدہ:
      rval = false;
      توڑو;
    }
  واپس rval;
}

/* Print the line number and separator; increment the line number. */

ساکن خالی
print_lineno (خالی)
{
  اگر (سطر_no_حد_سے_تجاوز)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  لکھو (lineno_format, lineno_چوڑائی, سطر_no, separator_str);

  اگر (ckd_جوڑو (&سطر_no, سطر_no, صفحہ_incr))
    سطر_no_حد_سے_تجاوز = true;
}

ساکن خالی
reset_lineno (خالی)
{
  اگر (reset_numbers)
    {
      سطر_no = starting_سطر_نمبر;
      سطر_no_حد_سے_تجاوز = false;
    }
}

/* Switch to a header section. */

ساکن خالی
proc_سرخی (خالی)
{
  current_type = سرخی_type;
  current_regex = &سرخی_regex;
  reset_lineno ();
  حرف_لکھو ('\n');
}

/* Switch to a body section. */

ساکن خالی
proc_body (خالی)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  حرف_لکھو ('\n');
}

/* Switch to a footer section. */

ساکن خالی
proc_footer (خالی)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  حرف_لکھو ('\n');
}

/* Process a regular text line in 'line_buf'. */

ساکن خالی
proc_متن (خالی)
{
  ساکن intmax_t blank_سطریں = 0;	/* Consecutive blank lines so far. */

  انتخاب (*current_type)
    {
    حالت 'a':
      اگر (blank_join > 1)
        {
          اگر (1 < سطر_buf.لمبائی || ++blank_سطریں == blank_join)
            {
              print_lineno ();
              blank_سطریں = 0;
            }
          ورنہ
            fputs (print_no_سطر_fmt, stdout);
        }
      ورنہ
        print_lineno ();
      توڑو;
    حالت 't':
      اگر (1 < سطر_buf.لمبائی)
        print_lineno ();
      ورنہ
        fputs (print_no_سطر_fmt, stdout);
      توڑو;
    حالت 'n':
      fputs (print_no_سطر_fmt, stdout);
      توڑو;
    حالت 'p':
      انتخاب (re_تلاش (current_regex, سطر_buf.بفر, سطر_buf.لمبائی - 1,
                         0, سطر_buf.لمبائی - 1, تہی))
        {
        حالت -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        حالت -1:
          fputs (print_no_سطر_fmt, stdout);
          توڑو;

        طےشدہ:
          print_lineno ();
          توڑو;
        }
    }
  fwrite (سطر_buf.بفر, حجم (حرف), سطر_buf.لمبائی, stdout);
}

/* Return the type of line in 'line_buf'. */

ساکن شمار section
check_section (خالی)
{
  size_t len = سطر_buf.لمبائی - 1;

  اگر (len < section_del_len || footer_del_len < section_del_len
      || !memeq (سطر_buf.بفر, section_del, section_del_len))
    واپس متن_2;
  اگر (len == سرخی_del_len
      && memeq (سطر_buf.بفر, سرخی_del, سرخی_del_len))
    واپس سرخی_2;
  اگر (len == body_del_len
      && memeq (سطر_buf.بفر, body_del, body_del_len))
    واپس Body;
  اگر (len == footer_del_len
      && memeq (سطر_buf.بفر, footer_del, footer_del_len))
    واپس Footer;
  واپس متن_2;
}

/* Read and process the file pointed to by FP. */

ساکن خالی
process_file (FILE *fp)
{
  جب (readlinebuffer (&سطر_buf, fp))
    {
      انتخاب (check_section ())
        {
        حالت سرخی_2:
          proc_سرخی ();
          توڑو;
        حالت Body:
          proc_body ();
          توڑو;
        حالت Footer:
          proc_footer ();
          توڑو;
        حالت متن_2:
          proc_متن ();
          توڑو;
        }

      اگر (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

ساکن bool
nl_file (حرف ثابت *file)
{
  FILE *دھارا;

  اگر (streq (file, "-"))
    {
      have_read_stdin = true;
      دھارا = stdin;
      assume (دھارا);  /* Pacify GCC bug#109613.  */
    }
  ورنہ
    {
      دھارا = fopen (file, "r");
      اگر (دھارا == تہی)
        {
          error (0, errno, "%s", quotef (file));
          واپس false;
        }
    }

  fadvise (دھارا, FADVISE_SEQUENTIAL);

  process_file (دھارا);

  صحیح err = errno;
  اگر (!ferror (دھارا))
    err = 0;
  اگر (streq (file, "-"))
    clearerr (دھارا);		/* Also clear EOF. */
  ورنہ اگر (fclose (دھارا) != 0 && !err)
    err = errno;
  اگر (err)
    {
      error (0, err, "%s", quotef (file));
      واپس false;
    }
  واپس true;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           تہی))
         != -1)
    {
      انتخاب (c)
        {
        حالت 'h':
          اگر (! build_type_arg (&سرخی_type, &سرخی_regex, سرخی_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          توڑو;
        حالت 'b':
          اگر (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          توڑو;
        حالت 'f':
          اگر (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          توڑو;
        حالت 'v':
          starting_سطر_نمبر = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          توڑو;
        حالت 'i':
          صفحہ_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          توڑو;
        حالت 'p':
          reset_numbers = false;
          توڑو;
        حالت 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          توڑو;
        حالت 's':
          separator_str = optarg;
          توڑو;
        حالت 'w':
          lineno_چوڑائی = xnumtoimax (optarg, 10, 1, زیادہ_سے_زیادہ_عدد, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          توڑو;
        حالت 'n':
          اگر (streq (optarg, "ln"))
            lineno_format = FORMAT_بایاں;
          ورنہ اگر (streq (optarg, "rn"))
            lineno_format = FORMAT_دایاں_NOLZ;
          ورنہ اگر (streq (optarg, "rz"))
            lineno_format = FORMAT_دایاں_LZ;
          ورنہ
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          توڑو;
        حالت 'd':
          len = لڑی_لمبائی (optarg);
          اگر (1 < MB_CUR_MAX)
            {
              حرف ثابت *p = optarg;
              حرف ثابت *lim = p + len;
              صحیح n_chars = 0;
              برائے (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_جائزہ (p, lim).len;
              اگر (n_chars == 1)
                حافظہ_نقل (mempcpy (section_del, optarg, len),  ":", حجم ":");
              ورنہ
                section_del = optarg;
            }
          ورنہ
            {
              اگر (len == 1)
                *section_del = *optarg;
              ورنہ
                section_del = optarg;
            }
          توڑو;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          ok = false;
          توڑو;
        }
    }

  اگر (!ok)
    usage (EXIT_FAILURE);

  /* Initialize the section delimiters.  */
  section_del_len = len = لڑی_لمبائی (section_del);

  سرخی_del_len = len * 3;
  سرخی_del = xmalloc (سرخی_del_len + 1);
  stpcpy (stpcpy (stpcpy (سرخی_del, section_del), section_del), section_del);

  body_del_len = len * 2;
  body_del = سرخی_del + len;

  footer_del_len = len;
  footer_del = body_del + len;

  /* Initialize the input buffer.  */
  initbuffer (&سطر_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = لڑی_لمبائی (separator_str);
  print_no_سطر_fmt = xmalloc (lineno_چوڑائی + len + 1);
  memset (print_no_سطر_fmt, ' ', lineno_چوڑائی + len);
  print_no_سطر_fmt[lineno_چوڑائی + len] = '\0';

  سطر_no = starting_سطر_نمبر;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  اگر (optind == argc)
    ok = nl_file ("-");
  ورنہ
    برائے (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  اگر (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
