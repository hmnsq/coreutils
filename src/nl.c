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
#تعریف_کن برنامه_NAME "nl"

#تعریف_کن AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
ساكن حرف ثابت FORMAT_راست_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
ساكن حرف ثابت FORMAT_راست_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
ساكن حرف ثابت FORMAT_چپ[] = "%-*jd%s";

/* Default section delimiter characters.  */
ساكن حرف DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
تعداد section
{
  سرآیند_2, Body, Footer, متن_2
};

/* Format of body lines (-b).  */
ساكن حرف ثابت *body_type = "t";

/* Format of header lines (-h).  */
ساكن حرف ثابت *سرآیند_type = "n";

/* Format of footer lines (-f).  */
ساكن حرف ثابت *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
ساكن حرف ثابت *current_type;

/* Regex for body lines to number (-bp).  */
ساكن بنية re_الگو_میانگیر body_regex;

/* Regex for header lines to number (-hp).  */
ساكن بنية re_الگو_میانگیر سرآیند_regex;

/* Regex for footer lines to number (-fp).  */
ساكن بنية re_الگو_میانگیر footer_regex;

/* Fastmaps for the above.  */
ساكن حرف body_fastmap[UCHAR_MAX + 1];
ساكن حرف سرآیند_fastmap[UCHAR_MAX + 1];
ساكن حرف footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
ساكن بنية re_الگو_میانگیر *current_regex = عدم;

/* Separator string to print after line number (-s).  */
ساكن حرف ثابت *separator_str = "\t";

/* Input section delimiter string (-d).  */
ساكن حرف *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
ساكن size_t section_del_len;

/* Header delimiter string.  */
ساكن حرف *سرآیند_del = عدم;

/* Header section delimiter length.  */
ساكن size_t سرآیند_del_len;

/* Body delimiter string.  */
ساكن حرف *body_del = عدم;

/* Body section delimiter length.  */
ساكن size_t body_del_len;

/* Footer delimiter string.  */
ساكن حرف *footer_del = عدم;

/* Footer section delimiter length.  */
ساكن size_t footer_del_len;

/* Input buffer.  */
ساكن بنية linebuffer سطر_buf;

/* printf format string for unnumbered lines.  */
ساكن حرف *print_no_سطر_fmt = عدم;

/* Starting line number on each page (-v).  */
ساكن intmax_t starting_سطر_عدد = 1;

/* Line number increment (-i).  */
ساكن intmax_t صفحه_incr = 1;

/* If true, reset line number at start of each page (-p).  */
ساكن bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
ساكن intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
ساكن صحيح lineno_عرض = 6;

/* Line number format (-n).  */
ساكن حرف ثابت *lineno_format = FORMAT_راست_NOLZ;

/* Current print line number.  */
ساكن intmax_t سطر_no;

/* Whether the current line number has incremented past limits.  */
ساكن bool سطر_no_سرریز;

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin;

ساكن بنية option ثابت longopts[] =
{
  {"header-numbering", required_آرگومان, عدم, 'h'},
  {"body-numbering", required_آرگومان, عدم, 'b'},
  {"footer-numbering", required_آرگومان, عدم, 'f'},
  {"starting-line-number", required_آرگومان, عدم, 'v'},
  {"line-increment", required_آرگومان, عدم, 'i'},
  {"no-renumber", no_آرگومان, عدم, 'p'},
  {"join-blank-lines", required_آرگومان, عدم, 'l'},
  {"number-separator", required_آرگومان, عدم, 's'},
  {"number-width", required_آرگومان, عدم, 'w'},
  {"number-format", required_آرگومان, عدم, 'n'},
  {"section-delimiter", required_آرگومان, عدم, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Print a usage message and quit. */

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              برنامه_name);
      fputs (_("\
Write each FILE مقصد standard output, with سطر numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE لكل numbering body سطرها\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC لكل logical صفحه delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE لكل numbering footer سطرها\n\
"));
      oputs (_("\
  -h, --سرآیند-numbering=STYLE    use STYLE لكل numbering سرآیند سطرها\n\
"));
      oputs (_("\
  -i, --سطر-increment=عدد_2     سطر عدد increment at each سطر\n\
"));
      oputs (_("\
  -l, --join-blank-سطرها=عدد_2   گروه از عدد_2 خالی سطرها counted as one\n\
"));
      oputs (_("\
  -n, --عدد-format=FORMAT      درج سطر numbers according مقصد FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               افعل not reset سطر numbers لكل each section\n\
"));
      oputs (_("\
  -s, --عدد-separator=STRING   جمع_کن STRING after (possible) سطر عدد\n\
"));
      oputs (_("\
  -v, --starting-سطر-عدد=عدد_2  اولی سطر عدد لكل each section\n\
"));
      oputs (_("\
  -w, --عدد-عرض=عدد_2       use عدد_2 columns لكل سطر numbers\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Default options are: -bt -d'\\:' -fn -hn -i1 -l1 -n'rn' -s<TAB> -v1 -w6\n\
\n\
CC are two delimiter characters استفاده_شده مقصد construct logical صفحه delimiters;\n\
a missing دومی character نتیجه_میدهد ':'.  As a GNU extension one can specify\n\
بیشتر than two characters, and also specifying the خالی string (-d '')\n\
disables section matching.\n\
"), stdout);
      fputs (_("\
\n\
STYLE is one از:\n\
\n\
  a      عدد all سطرها\n\
  t      عدد only nonempty سطرها\n\
  n      عدد no سطرها\n\
  pBRE   عدد only سطرها that contain a تطابق لكل the basic regular\n\
         عبارت_2, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one از:\n\
\n\
  ln     چپ justified, no leading zeros\n\
  rn     راست justified, no leading zeros\n\
  rz     راست justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

ساكن bool
build_type_arg (حرف ثابت **typep,
                بنية re_الگو_میانگیر *regexp, حرف *fastmap)
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
      regexp->میانگیر = عدم;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = عدم;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_خالی_RANGES;
      errmsg = re_compile_الگو (optarg, طول_رشته (optarg), regexp);
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
  إذا (سطر_no_سرریز)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  چاپ_کن (lineno_format, lineno_عرض, سطر_no, separator_str);

  إذا (ckd_جمع_کن (&سطر_no, سطر_no, صفحه_incr))
    سطر_no_سرریز = true;
}

ساكن فراغ
reset_lineno (فراغ)
{
  إذا (reset_numbers)
    {
      سطر_no = starting_سطر_عدد;
      سطر_no_سرریز = false;
    }
}

/* Switch to a header section. */

ساكن فراغ
proc_سرآیند (فراغ)
{
  current_type = سرآیند_type;
  current_regex = &سرآیند_regex;
  reset_lineno ();
  حرف_چاپ_کن ('\n');
}

/* Switch to a body section. */

ساكن فراغ
proc_body (فراغ)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  حرف_چاپ_کن ('\n');
}

/* Switch to a footer section. */

ساكن فراغ
proc_footer (فراغ)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  حرف_چاپ_کن ('\n');
}

/* Process a regular text line in 'line_buf'. */

ساكن فراغ
proc_متن (فراغ)
{
  ساكن intmax_t blank_سطرها = 0;	/* Consecutive blank lines so far. */

  بدل (*current_type)
    {
    حالة 'a':
      إذا (blank_join > 1)
        {
          إذا (1 < سطر_buf.درازی || ++blank_سطرها == blank_join)
            {
              print_lineno ();
              blank_سطرها = 0;
            }
          وإلا
            fputs (print_no_سطر_fmt, stdout);
        }
      وإلا
        print_lineno ();
      اكسر;
    حالة 't':
      إذا (1 < سطر_buf.درازی)
        print_lineno ();
      وإلا
        fputs (print_no_سطر_fmt, stdout);
      اكسر;
    حالة 'n':
      fputs (print_no_سطر_fmt, stdout);
      اكسر;
    حالة 'p':
      بدل (re_جستجو (current_regex, سطر_buf.میانگیر, سطر_buf.درازی - 1,
                         0, سطر_buf.درازی - 1, عدم))
        {
        حالة -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        حالة -1:
          fputs (print_no_سطر_fmt, stdout);
          اكسر;

        افتراضي:
          print_lineno ();
          اكسر;
        }
    }
  fwrite (سطر_buf.میانگیر, حجم (حرف), سطر_buf.درازی, stdout);
}

/* Return the type of line in 'line_buf'. */

ساكن تعداد section
check_section (فراغ)
{
  size_t len = سطر_buf.درازی - 1;

  إذا (len < section_del_len || footer_del_len < section_del_len
      || !memeq (سطر_buf.میانگیر, section_del, section_del_len))
    أرجع متن_2;
  إذا (len == سرآیند_del_len
      && memeq (سطر_buf.میانگیر, سرآیند_del, سرآیند_del_len))
    أرجع سرآیند_2;
  إذا (len == body_del_len
      && memeq (سطر_buf.میانگیر, body_del, body_del_len))
    أرجع Body;
  إذا (len == footer_del_len
      && memeq (سطر_buf.میانگیر, footer_del, footer_del_len))
    أرجع Footer;
  أرجع متن_2;
}

/* Read and process the file pointed to by FP. */

ساكن فراغ
process_file (FILE *fp)
{
  طالما (readlinebuffer (&سطر_buf, fp))
    {
      بدل (check_section ())
        {
        حالة سرآیند_2:
          proc_سرآیند ();
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
  FILE *جریان;

  إذا (streq (file, "-"))
    {
      have_read_stdin = true;
      جریان = stdin;
      assume (جریان);  /* Pacify GCC bug#109613.  */
    }
  وإلا
    {
      جریان = fopen (file, "r");
      إذا (جریان == عدم)
        {
          error (0, errno, "%s", quotef (file));
          أرجع false;
        }
    }

  fadvise (جریان, FADVISE_SEQUENTIAL);

  process_file (جریان);

  صحيح err = errno;
  إذا (!ferror (جریان))
    err = 0;
  إذا (streq (file, "-"))
    clearerr (جریان);		/* Also clear EOF. */
  وإلا إذا (fclose (جریان) != 0 && !err)
    err = errno;
  إذا (err)
    {
      error (0, err, "%s", quotef (file));
      أرجع false;
    }
  أرجع true;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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
          إذا (! build_type_arg (&سرآیند_type, &سرآیند_regex, سرآیند_fastmap))
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
          starting_سطر_عدد = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          اكسر;
        حالة 'i':
          صفحه_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
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
          lineno_عرض = xnumtoimax (optarg, 10, 1, بیشترین_صحیح, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          اكسر;
        حالة 'n':
          إذا (streq (optarg, "ln"))
            lineno_format = FORMAT_چپ;
          وإلا إذا (streq (optarg, "rn"))
            lineno_format = FORMAT_راست_NOLZ;
          وإلا إذا (streq (optarg, "rz"))
            lineno_format = FORMAT_راست_LZ;
          وإلا
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          اكسر;
        حالة 'd':
          len = طول_رشته (optarg);
          إذا (1 < MB_CUR_MAX)
            {
              حرف ثابت *p = optarg;
              حرف ثابت *lim = p + len;
              صحيح n_chars = 0;
              لكل (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_پیمایش (p, lim).len;
              إذا (n_chars == 1)
                کپی_حافظه (mempcpy (section_del, optarg, len),  ":", حجم ":");
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
        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);
        افتراضي:
          ok = false;
          اكسر;
        }
    }

  إذا (!ok)
    usage (EXIT_FAILURE);

  /* Initialize the section delimiters.  */
  section_del_len = len = طول_رشته (section_del);

  سرآیند_del_len = len * 3;
  سرآیند_del = xmalloc (سرآیند_del_len + 1);
  stpcpy (stpcpy (stpcpy (سرآیند_del, section_del), section_del), section_del);

  body_del_len = len * 2;
  body_del = سرآیند_del + len;

  footer_del_len = len;
  footer_del = body_del + len;

  /* Initialize the input buffer.  */
  initbuffer (&سطر_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = طول_رشته (separator_str);
  print_no_سطر_fmt = xmalloc (lineno_عرض + len + 1);
  memset (print_no_سطر_fmt, ' ', lineno_عرض + len);
  print_no_سطر_fmt[lineno_عرض + len] = '\0';

  سطر_no = starting_سطر_عدد;
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
