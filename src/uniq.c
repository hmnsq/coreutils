/* uniq -- remove duplicate lines from a sorted file
   Copyright (C) 1986-2026 Free Software Foundation, Inc.

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

/* Written by Richard M. Stallman and David MacKenzie. */

#ضمن <config.h>

#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "linebuffer.h"
#ضمن "fadvise.h"
#ضمن "mcel.h"
#ضمن "posixver.h"
#ضمن "skipchars.h"
#ضمن "stdio--.h"
#ضمن "xstrtol.h"
#ضمن "memcasecmp.h"
#ضمن "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "uniq"

#تعریف_کرو AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ساكن فراغ
وٹاندرا_سطریں (بنية linebuffer **a, بنية linebuffer **b)
{
  بنية linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
ساكن idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
ساكن idx_t skip_chars = false;

/* Number of chars to compare. */
ساكن idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
ساكن bool گنتی_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
ساكن bool output_unique = true;
ساكن bool output_پہلا_repeated = true;
ساكن bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
ساكن bool ignore_case;

تعداد delimit_طریقہ
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

ساكن حرف ثابت *ثابت delimit_طریقہ_string[] =
{
  "none", "prepend", "separate", عدم
};

ساكن تعداد delimit_طریقہ ثابت delimit_طریقہ_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
ساكن تعداد delimit_طریقہ delimit_groups = DM_NONE;

تعداد grouping_طریقہ
{
  /* No grouping, when "--group" isn't used */
  GM_NONE,

  /* Delimiter precedes all groups.  --group=prepend */
  GM_PREPEND,

  /* Delimiter follows all groups.   --group=append */
  GM_APPEND,

  /* Delimiter between groups.    --group[=separate] */
  GM_SEPARATE,

  /* Delimiter before and after each group. --group=both */
  GM_BOTH
};

ساكن حرف ثابت *ثابت grouping_طریقہ_string[] =
{
  "prepend", "append", "separate", "both", عدم
};

ساكن تعداد grouping_طریقہ ثابت grouping_طریقہ_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

ساكن تعداد grouping_طریقہ grouping = GM_NONE;

تعداد
{
  گروہ_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"count", no_دلیل, عدم, 'c'},
  {"repeated", no_دلیل, عدم, 'd'},
  {"all-repeated", optional_دلیل, عدم, 'D'},
  {"group", optional_دلیل, عدم, گروہ_OPTION},
  {"ignore-case", no_دلیل, عدم, 'i'},
  {"unique", no_دلیل, عدم, 'u'},
  {"skip-fields", required_دلیل, عدم, 'f'},
  {"skip-chars", required_دلیل, عدم, 's'},
  {"check-chars", required_دلیل, عدم, 'w'},
  {"zero-terminated", no_دلیل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s [OPTION]... [داخلہ_2 [OUTPUT]]\n\
"),
              پروگرام_name);
      fputs (_("\
Filter adjacent matching سطریں from داخلہ_2 (or standard داخلہ),\n\
writing منزل OUTPUT (or standard output).\n\
\n\
With no options, matching سطریں are merged منزل the پہلا occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --گنتی\n\
         اگیتر سطریں by the نمبر دا occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate سطریں, one لكل each گروہ\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate سطریں\n\
"));
     oputs (_("\
      --all-repeated[=طریقہ]\n\
         like -D, but allow separating groups with an خالی_تھاواں سطر;\n\
         طریقہ={none(افتراضي),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the پہلا N fields\n\
"));
     oputs (_("\
      --گروہ[=طریقہ]\n\
         show all اشیاء, separating groups with an خالی_تھاواں سطر;\n\
         طریقہ={separate(افتراضي),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-حالة\n\
         ignore differences وچ حالة when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the پہلا N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique سطریں\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no ہور than N characters وچ سطریں\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (نسخہ_OPTION_DESCRIPTION);
     fputs (_("\
\n\
A میدان is a لگاتار_2 دا blanks (usually spaces and/or TABs), then non-blank\n\
characters.  Fields are skipped before chars.\n\
"), stdout);
     fputs (_("\
\n\
'uniq' does not detect repeated سطریں unless they are adjacent.\n\
You may want منزل ترتیب_دیو the داخلہ پہلا, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

ساكن bool
strict_posix2 (فراغ)
{
  صحيح posix_ver = posix2_نسخہ ();
  أرجع 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

ساكن idx_t
ناپ_opt (حرف ثابت *opt, حرف ثابت *msgid)
{
  intmax_t ناپ;
  إذا (LONGINT_حد_توں_ودھ < xstrtoimax (opt, عدم, 10, &ناپ, "")
      || ناپ < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  أرجع MIN (ناپ, IDX_MAX);
}

ساكن bool
newline_or_blank (mcel_t g)
{
  أرجع g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

ساكن حرف *
لبھو_میدان (بنية linebuffer ثابت *سطر, idx_t *plen)
{
  حرف *lp = سطر->بفر;
  حرف ثابت *lim = lp + سطر->لمائی - 1;

  لكل (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  لكل (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_جانچ (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  إذا (lim - lp <= check_chars)
    len = lim - lp;
  وإلا إذا (MB_CUR_MAX <= 1)
    len = check_chars;
  وإلا
    {
      حرف *ep = lp;
      لكل (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_جانچ (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  أرجع lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

ساكن bool
different (حرف *old, حرف *new, idx_t oldlen, idx_t newlen)
{
  إذا (ignore_case)
    أرجع oldlen != newlen || memcasecmp (old, new, oldlen);
  وإلا
    أرجع oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

ساكن فراغ
writeline (بنية linebuffer ثابت *سطر,
           bool میل, intmax_t linecount)
{
  إذا (! (linecount == 0 ? output_unique
         : !میل ? output_پہلا_repeated
         : output_later_repeated))
    أرجع;

  إذا (گنتی_occurrences)
    {
      حرف buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      حرف *سرا = buf + حجم buf;
      حرف *p = سرا;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      افعل
        *--p = '0' + i % 10;
      طالما ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      طالما (سرا - p < 8)
        *--p = ' ';
      ثابت idx_t nbytes = سرا - p;
      إذا (fwrite (p, حجم (حرف), nbytes, stdout) != nbytes)
        write_error ();
    }

  إذا (fwrite (سطر->بفر, حجم (حرف), سطر->لمائی, stdout)
      != سطر->لمائی)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

ساكن فراغ
check_file (حرف ثابت *infile, حرف ثابت *outfile, حرف delimiter)
{
  بنية linebuffer lb1, lb2;
  بنية linebuffer *thisline, *prevline;

  إذا (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  إذا (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  thisline = &lb1;
  prevline = &lb2;

  initbuffer (thisline);
  initbuffer (prevline);

  /* The duplication in the following 'if' and 'else' blocks is an
     optimization to distinguish between when we can print input
     lines immediately (1. & 2.) or not.

     1. --group => all input lines are printed.
        checking for unique/duplicated lines is used only for printing
        group separators.

     2. The default case in which none of these options has been specified:
          --count, --repeated,  --all-repeated, --unique
        In the default case, this optimization lets uniq output each different
        line right away, without waiting to see if the next one is different.

     3. All other cases.
  */
  إذا (output_unique && output_پہلا_repeated && !گنتی_occurrences)
    {
      حرف *prevfield = عدم;
      idx_t prevlen;
      bool پہلا_گروہ_printed = false;

      طالما (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          حرف *thisfield = لبھو_میدان (thisline, &thislen);
          bool new_گروہ = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          إذا (new_گروہ && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (پہلا_گروہ_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            حرف_لکھو (delimiter);

          إذا (new_گروہ || grouping != GM_NONE)
            {
              إذا (fwrite (thisline->بفر, حجم (حرف), thisline->لمائی,
                  stdout) != thisline->لمائی)
                write_error ();

              وٹاندرا_سطریں (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              پہلا_گروہ_printed = true;
            }
        }
      إذا ((grouping == GM_BOTH || grouping == GM_APPEND) && پہلا_گروہ_printed)
        حرف_لکھو (delimiter);
    }
  وإلا
    {
      إذا (!readlinebuffer_delim (prevline, stdin, delimiter))
        اذهب closefiles;

      idx_t prevlen;
      حرف *prevfield = لبھو_میدان (prevline, &prevlen);
      intmax_t میل_گنتی = 0;
      bool پہلا_delimiter = true;

      طالما (!feof (stdin))
        {
          إذا (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              إذا (ferror (stdin))
                اذهب closefiles;
              اكسر;
            }
          idx_t thislen;
          حرف *thisfield = لبھو_میدان (thisline, &thislen);
          bool میل = !different (thisfield, prevfield, thislen, prevlen);
          میل_گنتی += میل;

          إذا (میل_گنتی == INTMAX_MAX)
            {
              إذا (گنتی_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              میل_گنتی--;
            }

          إذا (delimit_groups != DM_NONE)
            {
              إذا (!میل)
                {
                  إذا (میل_گنتی) /* a previous match */
                    پہلا_delimiter = false; /* Only used when DM_SEPARATE */
                }
              وإلا إذا (میل_گنتی == 1)
                {
                  إذا ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !پہلا_delimiter))
                    حرف_لکھو (delimiter);
                }
            }

          إذا (!میل || output_later_repeated)
            {
              writeline (prevline, میل, میل_گنتی);
              وٹاندرا_سطریں (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              إذا (!میل)
                میل_گنتی = 0;
            }
        }

      writeline (prevline, false, میل_گنتی);
    }

 closefiles:
  إذا (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  حرر (lb1.بفر);
  حرر (lb2.بفر);
}

تعداد Skip_میدان_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

صحيح
مکھ (صحيح argc, حرف **argv)
{
  صحيح optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);
  تعداد Skip_میدان_option_type skip_میدان_option_type = SFO_NONE;
  صحيح nfiles = 0;
  حرف ثابت *file[2];
  حرف delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_ورتیا = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      إذا (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, عدم))
              == -1))
        {
          إذا (argc <= optind)
            اكسر;
          إذا (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      وإلا بدل (optc)
        {
        حالة 1:
          {
            intmax_t ناپ;
            إذا (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, عدم, 10, &ناپ, "")
                    <= LONGINT_حد_توں_ودھ))
              skip_chars = MIN (ناپ, IDX_MAX);
            وإلا إذا (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            وإلا
              file[nfiles++] = optarg;
          }
          اكسر;

        حالة '0':
        حالة '1':
        حالة '2':
        حالة '3':
        حالة '4':
        حالة '5':
        حالة '6':
        حالة '7':
        حالة '8':
        حالة '9':
          {
            إذا (skip_میدان_option_type == SFO_NEW)
              skip_fields = 0;

            إذا (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_میدان_option_type = SFO_OBSOLETE;
          }
          اكسر;

        حالة 'c':
          گنتی_occurrences = true;
          output_option_ورتیا = true;
          اكسر;

        حالة 'd':
          output_unique = false;
          output_option_ورتیا = true;
          اكسر;

        حالة 'D':
          output_unique = false;
          output_later_repeated = true;
          إذا (optarg == عدم)
            delimit_groups = DM_NONE;
          وإلا
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_طریقہ_string,
                                        delimit_طریقہ_map);
          output_option_ورتیا = true;
          اكسر;

        حالة گروہ_OPTION:
          إذا (optarg == عدم)
            grouping = GM_SEPARATE;
          وإلا
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_طریقہ_string,
                                  grouping_طریقہ_map);
          اكسر;

        حالة 'f':
          skip_میدان_option_type = SFO_NEW;
          skip_fields = ناپ_opt (optarg,
                                  N_("invalid number of fields to skip"));
          اكسر;

        حالة 'i':
          ignore_case = true;
          اكسر;

        حالة 's':
          skip_chars = ناپ_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          اكسر;

        حالة 'u':
          output_پہلا_repeated = false;
          output_option_ورتیا = true;
          اكسر;

        حالة 'w':
          check_chars = ناپ_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          اكسر;

        حالة 'z':
          delimiter = '\0';
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  إذا (grouping != GM_NONE && output_option_ورتیا)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  إذا (grouping != GM_NONE && گنتی_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  إذا (گنتی_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  أرجع EXIT_SUCCESS;
}
