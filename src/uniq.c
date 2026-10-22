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

#شامل <config.h>

#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "argmatch.h"
#شامل "assure.h"
#شامل "linebuffer.h"
#شامل "fadvise.h"
#شامل "mcel.h"
#شامل "posixver.h"
#شامل "skipchars.h"
#شامل "stdio--.h"
#شامل "xstrtol.h"
#شامل "memcasecmp.h"
#شامل "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "uniq"

#تعریف_کرو AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ساکن خالی
تبادلہ_سطریں (ساخت linebuffer **a, ساخت linebuffer **b)
{
  ساخت linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
ساکن idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
ساکن idx_t skip_chars = false;

/* Number of chars to compare. */
ساکن idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
ساکن bool گنتی_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
ساکن bool output_unique = true;
ساکن bool output_پہلا_repeated = true;
ساکن bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
ساکن bool ignore_case;

شمار delimit_طریقہ
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

ساکن حرف ثابت *ثابت delimit_طریقہ_string[] =
{
  "none", "prepend", "separate", تہی
};

ساکن شمار delimit_طریقہ ثابت delimit_طریقہ_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
ساکن شمار delimit_طریقہ delimit_groups = DM_NONE;

شمار grouping_طریقہ
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

ساکن حرف ثابت *ثابت grouping_طریقہ_string[] =
{
  "prepend", "append", "separate", "both", تہی
};

ساکن شمار grouping_طریقہ ثابت grouping_طریقہ_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

ساکن شمار grouping_طریقہ grouping = GM_NONE;

شمار
{
  گروہ_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت longopts[] =
{
  {"count", no_دلیل, تہی, 'c'},
  {"repeated", no_دلیل, تہی, 'd'},
  {"all-repeated", optional_دلیل, تہی, 'D'},
  {"group", optional_دلیل, تہی, گروہ_OPTION},
  {"ignore-case", no_دلیل, تہی, 'i'},
  {"unique", no_دلیل, تہی, 'u'},
  {"skip-fields", required_دلیل, تہی, 'f'},
  {"skip-chars", required_دلیل, تہی, 's'},
  {"check-chars", required_دلیل, تہی, 'w'},
  {"zero-terminated", no_دلیل, تہی, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [داخلہ_2 [OUTPUT]]\n\
"),
              پروگرام_name);
      fputs (_("\
Filter adjacent matching سطریں from داخلہ_2 (or standard داخلہ),\n\
writing مقصد OUTPUT (or standard output).\n\
\n\
With no options, matching سطریں are merged مقصد the پہلا occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --گنتی\n\
         سابقہ سطریں by the نمبر کا occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate سطریں, one برائے each گروہ\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate سطریں\n\
"));
     oputs (_("\
      --all-repeated[=طریقہ]\n\
         like -D, but allow separating groups with an خالی_خانے سطر;\n\
         طریقہ={none(طےشدہ),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the پہلا N fields\n\
"));
     oputs (_("\
      --گروہ[=طریقہ]\n\
         show all اشیاء, separating groups with an خالی_خانے سطر;\n\
         طریقہ={separate(طےشدہ),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-حالت\n\
         ignore differences اندر حالت when comparing\n\
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
         compare no مزید than N characters اندر سطریں\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (نسخہ_OPTION_DESCRIPTION);
     fputs (_("\
\n\
A میدان is a تسلسل_2 کا blanks (usually spaces and/or TABs), then non-blank\n\
characters.  Fields are skipped before chars.\n\
"), stdout);
     fputs (_("\
\n\
'uniq' does not detect repeated سطریں unless they are adjacent.\n\
You may want مقصد ترتیب_دو the داخلہ پہلا, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

ساکن bool
strict_posix2 (خالی)
{
  صحیح posix_ver = posix2_نسخہ ();
  واپس 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

ساکن idx_t
سائز_opt (حرف ثابت *opt, حرف ثابت *msgid)
{
  intmax_t سائز;
  اگر (LONGINT_حد_سے_تجاوز < xstrtoimax (opt, تہی, 10, &سائز, "")
      || سائز < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  واپس MIN (سائز, IDX_MAX);
}

ساکن bool
newline_or_blank (mcel_t g)
{
  واپس g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

ساکن حرف *
ڈھونڈو_میدان (ساخت linebuffer ثابت *سطر, idx_t *plen)
{
  حرف *lp = سطر->بفر;
  حرف ثابت *lim = lp + سطر->لمبائی - 1;

  برائے (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  برائے (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_جائزہ (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  اگر (lim - lp <= check_chars)
    len = lim - lp;
  ورنہ اگر (MB_CUR_MAX <= 1)
    len = check_chars;
  ورنہ
    {
      حرف *ep = lp;
      برائے (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_جائزہ (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  واپس lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

ساکن bool
different (حرف *old, حرف *new, idx_t oldlen, idx_t newlen)
{
  اگر (ignore_case)
    واپس oldlen != newlen || memcasecmp (old, new, oldlen);
  ورنہ
    واپس oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

ساکن خالی
writeline (ساخت linebuffer ثابت *سطر,
           bool مطابقت, intmax_t linecount)
{
  اگر (! (linecount == 0 ? output_unique
         : !مطابقت ? output_پہلا_repeated
         : output_later_repeated))
    واپس;

  اگر (گنتی_occurrences)
    {
      حرف buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      حرف *انتہا = buf + حجم buf;
      حرف *p = انتہا;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      کرو
        *--p = '0' + i % 10;
      جب ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      جب (انتہا - p < 8)
        *--p = ' ';
      ثابت idx_t nbytes = انتہا - p;
      اگر (fwrite (p, حجم (حرف), nbytes, stdout) != nbytes)
        write_error ();
    }

  اگر (fwrite (سطر->بفر, حجم (حرف), سطر->لمبائی, stdout)
      != سطر->لمبائی)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

ساکن خالی
check_file (حرف ثابت *infile, حرف ثابت *outfile, حرف delimiter)
{
  ساخت linebuffer lb1, lb2;
  ساخت linebuffer *thisline, *prevline;

  اگر (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  اگر (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
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
  اگر (output_unique && output_پہلا_repeated && !گنتی_occurrences)
    {
      حرف *prevfield = تہی;
      idx_t prevlen;
      bool پہلا_گروہ_printed = false;

      جب (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          حرف *thisfield = ڈھونڈو_میدان (thisline, &thislen);
          bool new_گروہ = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          اگر (new_گروہ && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (پہلا_گروہ_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            حرف_لکھو (delimiter);

          اگر (new_گروہ || grouping != GM_NONE)
            {
              اگر (fwrite (thisline->بفر, حجم (حرف), thisline->لمبائی,
                  stdout) != thisline->لمبائی)
                write_error ();

              تبادلہ_سطریں (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              پہلا_گروہ_printed = true;
            }
        }
      اگر ((grouping == GM_BOTH || grouping == GM_APPEND) && پہلا_گروہ_printed)
        حرف_لکھو (delimiter);
    }
  ورنہ
    {
      اگر (!readlinebuffer_delim (prevline, stdin, delimiter))
        جاؤ closefiles;

      idx_t prevlen;
      حرف *prevfield = ڈھونڈو_میدان (prevline, &prevlen);
      intmax_t مطابقت_گنتی = 0;
      bool پہلا_delimiter = true;

      جب (!feof (stdin))
        {
          اگر (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              اگر (ferror (stdin))
                جاؤ closefiles;
              توڑو;
            }
          idx_t thislen;
          حرف *thisfield = ڈھونڈو_میدان (thisline, &thislen);
          bool مطابقت = !different (thisfield, prevfield, thislen, prevlen);
          مطابقت_گنتی += مطابقت;

          اگر (مطابقت_گنتی == INTMAX_MAX)
            {
              اگر (گنتی_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              مطابقت_گنتی--;
            }

          اگر (delimit_groups != DM_NONE)
            {
              اگر (!مطابقت)
                {
                  اگر (مطابقت_گنتی) /* a previous match */
                    پہلا_delimiter = false; /* Only used when DM_SEPARATE */
                }
              ورنہ اگر (مطابقت_گنتی == 1)
                {
                  اگر ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !پہلا_delimiter))
                    حرف_لکھو (delimiter);
                }
            }

          اگر (!مطابقت || output_later_repeated)
            {
              writeline (prevline, مطابقت, مطابقت_گنتی);
              تبادلہ_سطریں (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              اگر (!مطابقت)
                مطابقت_گنتی = 0;
            }
        }

      writeline (prevline, false, مطابقت_گنتی);
    }

 closefiles:
  اگر (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  آزاد (lb1.بفر);
  آزاد (lb2.بفر);
}

شمار Skip_میدان_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != تہی);
  شمار Skip_میدان_option_type skip_میدان_option_type = SFO_NONE;
  صحیح nfiles = 0;
  حرف ثابت *file[2];
  حرف delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_مستعمل = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      اگر (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, تہی))
              == -1))
        {
          اگر (argc <= optind)
            توڑو;
          اگر (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      ورنہ انتخاب (optc)
        {
        حالت 1:
          {
            intmax_t سائز;
            اگر (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, تہی, 10, &سائز, "")
                    <= LONGINT_حد_سے_تجاوز))
              skip_chars = MIN (سائز, IDX_MAX);
            ورنہ اگر (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            ورنہ
              file[nfiles++] = optarg;
          }
          توڑو;

        حالت '0':
        حالت '1':
        حالت '2':
        حالت '3':
        حالت '4':
        حالت '5':
        حالت '6':
        حالت '7':
        حالت '8':
        حالت '9':
          {
            اگر (skip_میدان_option_type == SFO_NEW)
              skip_fields = 0;

            اگر (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_میدان_option_type = SFO_OBSOLETE;
          }
          توڑو;

        حالت 'c':
          گنتی_occurrences = true;
          output_option_مستعمل = true;
          توڑو;

        حالت 'd':
          output_unique = false;
          output_option_مستعمل = true;
          توڑو;

        حالت 'D':
          output_unique = false;
          output_later_repeated = true;
          اگر (optarg == تہی)
            delimit_groups = DM_NONE;
          ورنہ
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_طریقہ_string,
                                        delimit_طریقہ_map);
          output_option_مستعمل = true;
          توڑو;

        حالت گروہ_OPTION:
          اگر (optarg == تہی)
            grouping = GM_SEPARATE;
          ورنہ
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_طریقہ_string,
                                  grouping_طریقہ_map);
          توڑو;

        حالت 'f':
          skip_میدان_option_type = SFO_NEW;
          skip_fields = سائز_opt (optarg,
                                  N_("invalid number of fields to skip"));
          توڑو;

        حالت 'i':
          ignore_case = true;
          توڑو;

        حالت 's':
          skip_chars = سائز_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          توڑو;

        حالت 'u':
          output_پہلا_repeated = false;
          output_option_مستعمل = true;
          توڑو;

        حالت 'w':
          check_chars = سائز_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          توڑو;

        حالت 'z':
          delimiter = '\0';
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  اگر (grouping != GM_NONE && output_option_مستعمل)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  اگر (grouping != GM_NONE && گنتی_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  اگر (گنتی_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  واپس EXIT_SUCCESS;
}
