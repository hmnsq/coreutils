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
#تعریف_کن برنامه_NAME "uniq"

#تعریف_کن AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ساكن فراغ
جابجا_سطرها (بنية linebuffer **a, بنية linebuffer **b)
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
ساكن bool شمار_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
ساكن bool output_unique = true;
ساكن bool output_اولی_repeated = true;
ساكن bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
ساكن bool ignore_case;

تعداد delimit_روش
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

ساكن حرف ثابت *ثابت delimit_روش_string[] =
{
  "none", "prepend", "separate", عدم
};

ساكن تعداد delimit_روش ثابت delimit_روش_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
ساكن تعداد delimit_روش delimit_groups = DM_NONE;

تعداد grouping_روش
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

ساكن حرف ثابت *ثابت grouping_روش_string[] =
{
  "prepend", "append", "separate", "both", عدم
};

ساكن تعداد grouping_روش ثابت grouping_روش_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

ساكن تعداد grouping_روش grouping = GM_NONE;

تعداد
{
  گروه_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"count", no_آرگومان, عدم, 'c'},
  {"repeated", no_آرگومان, عدم, 'd'},
  {"all-repeated", optional_آرگومان, عدم, 'D'},
  {"group", optional_آرگومان, عدم, گروه_OPTION},
  {"ignore-case", no_آرگومان, عدم, 'i'},
  {"unique", no_آرگومان, عدم, 'u'},
  {"skip-fields", required_آرگومان, عدم, 'f'},
  {"skip-chars", required_آرگومان, عدم, 's'},
  {"check-chars", required_آرگومان, عدم, 'w'},
  {"zero-terminated", no_آرگومان, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [درون_داد_2 [OUTPUT]]\n\
"),
              برنامه_name);
      fputs (_("\
Filter adjacent matching سطرها from درون_داد_2 (or standard درون_داد),\n\
writing مقصد OUTPUT (or standard output).\n\
\n\
With no options, matching سطرها are merged مقصد the اولی occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --شمار\n\
         پیشوند سطرها by the عدد از occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate سطرها, one لكل each گروه\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate سطرها\n\
"));
     oputs (_("\
      --all-repeated[=روش]\n\
         like -D, but allow separating groups with an خالی سطر;\n\
         روش={none(افتراضي),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the اولی N fields\n\
"));
     oputs (_("\
      --گروه[=روش]\n\
         show all اقلام, separating groups with an خالی سطر;\n\
         روش={separate(افتراضي),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-حالة\n\
         ignore differences درون حالة when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the اولی N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique سطرها\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no بیشتر than N characters درون سطرها\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (نسخه_OPTION_DESCRIPTION);
     fputs (_("\
\n\
A فیلد is a پیاپی_2 از blanks (usually spaces and/or TABs), then non-blank\n\
characters.  Fields are skipped before chars.\n\
"), stdout);
     fputs (_("\
\n\
'uniq' does not detect repeated سطرها unless they are adjacent.\n\
You may want مقصد مرتب_سازی the درون_داد اولی, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

ساكن bool
strict_posix2 (فراغ)
{
  صحيح posix_ver = posix2_نسخه ();
  أرجع 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

ساكن idx_t
اندازه_opt (حرف ثابت *opt, حرف ثابت *msgid)
{
  intmax_t اندازه;
  إذا (LONGINT_سرریز < xstrtoimax (opt, عدم, 10, &اندازه, "")
      || اندازه < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  أرجع MIN (اندازه, IDX_MAX);
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
پیدا_کن_فیلد (بنية linebuffer ثابت *سطر, idx_t *plen)
{
  حرف *lp = سطر->میانگیر;
  حرف ثابت *lim = lp + سطر->درازی - 1;

  لكل (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  لكل (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_پیمایش (lp, lim).len;

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
        ep += mcel_پیمایش (ep, lim).len;
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
           bool تطابق, intmax_t linecount)
{
  إذا (! (linecount == 0 ? output_unique
         : !تطابق ? output_اولی_repeated
         : output_later_repeated))
    أرجع;

  إذا (شمار_occurrences)
    {
      حرف buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      حرف *انتها = buf + حجم buf;
      حرف *p = انتها;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      افعل
        *--p = '0' + i % 10;
      طالما ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      طالما (انتها - p < 8)
        *--p = ' ';
      ثابت idx_t nbytes = انتها - p;
      إذا (fwrite (p, حجم (حرف), nbytes, stdout) != nbytes)
        write_error ();
    }

  إذا (fwrite (سطر->میانگیر, حجم (حرف), سطر->درازی, stdout)
      != سطر->درازی)
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
  إذا (output_unique && output_اولی_repeated && !شمار_occurrences)
    {
      حرف *prevfield = عدم;
      idx_t prevlen;
      bool اولی_گروه_printed = false;

      طالما (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          حرف *thisfield = پیدا_کن_فیلد (thisline, &thislen);
          bool new_گروه = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          إذا (new_گروه && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (اولی_گروه_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            حرف_چاپ_کن (delimiter);

          إذا (new_گروه || grouping != GM_NONE)
            {
              إذا (fwrite (thisline->میانگیر, حجم (حرف), thisline->درازی,
                  stdout) != thisline->درازی)
                write_error ();

              جابجا_سطرها (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              اولی_گروه_printed = true;
            }
        }
      إذا ((grouping == GM_BOTH || grouping == GM_APPEND) && اولی_گروه_printed)
        حرف_چاپ_کن (delimiter);
    }
  وإلا
    {
      إذا (!readlinebuffer_delim (prevline, stdin, delimiter))
        اذهب closefiles;

      idx_t prevlen;
      حرف *prevfield = پیدا_کن_فیلد (prevline, &prevlen);
      intmax_t تطابق_شمار = 0;
      bool اولی_delimiter = true;

      طالما (!feof (stdin))
        {
          إذا (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              إذا (ferror (stdin))
                اذهب closefiles;
              اكسر;
            }
          idx_t thislen;
          حرف *thisfield = پیدا_کن_فیلد (thisline, &thislen);
          bool تطابق = !different (thisfield, prevfield, thislen, prevlen);
          تطابق_شمار += تطابق;

          إذا (تطابق_شمار == INTMAX_MAX)
            {
              إذا (شمار_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              تطابق_شمار--;
            }

          إذا (delimit_groups != DM_NONE)
            {
              إذا (!تطابق)
                {
                  إذا (تطابق_شمار) /* a previous match */
                    اولی_delimiter = false; /* Only used when DM_SEPARATE */
                }
              وإلا إذا (تطابق_شمار == 1)
                {
                  إذا ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !اولی_delimiter))
                    حرف_چاپ_کن (delimiter);
                }
            }

          إذا (!تطابق || output_later_repeated)
            {
              writeline (prevline, تطابق, تطابق_شمار);
              جابجا_سطرها (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              إذا (!تطابق)
                تطابق_شمار = 0;
            }
        }

      writeline (prevline, false, تطابق_شمار);
    }

 closefiles:
  إذا (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  حرر (lb1.میانگیر);
  حرر (lb2.میانگیر);
}

تعداد Skip_فیلد_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);
  تعداد Skip_فیلد_option_type skip_فیلد_option_type = SFO_NONE;
  صحيح nfiles = 0;
  حرف ثابت *file[2];
  حرف delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_استفاده_شده = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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
            intmax_t اندازه;
            إذا (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, عدم, 10, &اندازه, "")
                    <= LONGINT_سرریز))
              skip_chars = MIN (اندازه, IDX_MAX);
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
            إذا (skip_فیلد_option_type == SFO_NEW)
              skip_fields = 0;

            إذا (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_فیلد_option_type = SFO_OBSOLETE;
          }
          اكسر;

        حالة 'c':
          شمار_occurrences = true;
          output_option_استفاده_شده = true;
          اكسر;

        حالة 'd':
          output_unique = false;
          output_option_استفاده_شده = true;
          اكسر;

        حالة 'D':
          output_unique = false;
          output_later_repeated = true;
          إذا (optarg == عدم)
            delimit_groups = DM_NONE;
          وإلا
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_روش_string,
                                        delimit_روش_map);
          output_option_استفاده_شده = true;
          اكسر;

        حالة گروه_OPTION:
          إذا (optarg == عدم)
            grouping = GM_SEPARATE;
          وإلا
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_روش_string,
                                  grouping_روش_map);
          اكسر;

        حالة 'f':
          skip_فیلد_option_type = SFO_NEW;
          skip_fields = اندازه_opt (optarg,
                                  N_("invalid number of fields to skip"));
          اكسر;

        حالة 'i':
          ignore_case = true;
          اكسر;

        حالة 's':
          skip_chars = اندازه_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          اكسر;

        حالة 'u':
          output_اولی_repeated = false;
          output_option_استفاده_شده = true;
          اكسر;

        حالة 'w':
          check_chars = اندازه_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          اكسر;

        حالة 'z':
          delimiter = '\0';
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  إذا (grouping != GM_NONE && output_option_استفاده_شده)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  إذا (grouping != GM_NONE && شمار_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  إذا (شمار_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  أرجع EXIT_SUCCESS;
}
