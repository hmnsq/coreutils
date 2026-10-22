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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "linebuffer.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "mcel.h"
#অন্তর্ভাব "posixver.h"
#অন্তর্ভাব "skipchars.h"
#অন্তর্ভাব "stdio--.h"
#অন্তর্ভাব "xstrtol.h"
#অন্তর্ভাব "memcasecmp.h"
#অন্তর্ভাব "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "uniq"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

স্থির শূন্য
swap_সারিগুলি (কাঠামো linebuffer **a, কাঠামো linebuffer **b)
{
  কাঠামো linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
স্থির idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
স্থির idx_t skip_chars = false;

/* Number of chars to compare. */
স্থির idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
স্থির bool গুনতি_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
স্থির bool output_unique = true;
স্থির bool output_first_repeated = true;
স্থির bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
স্থির bool ignore_case;

গণনা delimit_method
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

স্থির অক্ষর ধ্রুবক *ধ্রুবক delimit_method_string[] =
{
  "none", "prepend", "separate", ফাঁকা
};

স্থির গণনা delimit_method ধ্রুবক delimit_method_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
স্থির গণনা delimit_method delimit_groups = DM_NONE;

গণনা grouping_method
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

স্থির অক্ষর ধ্রুবক *ধ্রুবক grouping_method_string[] =
{
  "prepend", "append", "separate", "both", ফাঁকা
};

স্থির গণনা grouping_method ধ্রুবক grouping_method_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

স্থির গণনা grouping_method grouping = GM_NONE;

গণনা
{
  GROUP_OPTION = CHAR_MAX + 1
};

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"count", no_argument, ফাঁকা, 'c'},
  {"repeated", no_argument, ফাঁকা, 'd'},
  {"all-repeated", optional_argument, ফাঁকা, 'D'},
  {"group", optional_argument, ফাঁকা, GROUP_OPTION},
  {"ignore-case", no_argument, ফাঁকা, 'i'},
  {"unique", no_argument, ফাঁকা, 'u'},
  {"skip-fields", required_argument, ফাঁকা, 'f'},
  {"skip-chars", required_argument, ফাঁকা, 's'},
  {"check-chars", required_argument, ফাঁকা, 'w'},
  {"zero-terminated", no_argument, ফাঁকা, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [প্রবেশ_2 [OUTPUT]]\n\
"),
              program_name);
      fputs (_("\
Filter adjacent matching সারিগুলি from প্রবেশ_2 (or standard প্রবেশ),\n\
writing to OUTPUT (or standard output).\n\
\n\
With no options, matching সারিগুলি are merged to the first occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --গুনতি\n\
         prefix সারিগুলি by the সংখ্যা of occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate সারিগুলি, one জন্য each group\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate সারিগুলি\n\
"));
     oputs (_("\
      --all-repeated[=METHOD]\n\
         like -D, but allow separating groups with an empty সারি;\n\
         METHOD={none(পূর্বনির্ধারিত),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the first N fields\n\
"));
     oputs (_("\
      --group[=METHOD]\n\
         show all বস্তুসমূহ, separating groups with an empty সারি;\n\
         METHOD={separate(পূর্বনির্ধারিত),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-ক্ষেত্র\n\
         ignore differences in ক্ষেত্র when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the first N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique সারিগুলি\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         সারি delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no more than N characters in সারিগুলি\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (VERSION_OPTION_DESCRIPTION);
     fputs (_("\
\n\
A field is a run of blanks (usually spaces and/or TABs), then non-blank\n\
characters.  Fields are skipped before chars.\n\
"), stdout);
     fputs (_("\
\n\
'uniq' does not detect repeated সারিগুলি unless they are adjacent.\n\
You may want to sort the প্রবেশ first, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

স্থির bool
strict_posix2 (শূন্য)
{
  পূর্ণসংখ্যা posix_ver = posix2_version ();
  ফেরত 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

স্থির idx_t
আকার_opt (অক্ষর ধ্রুবক *opt, অক্ষর ধ্রুবক *msgid)
{
  intmax_t আকার_2;
  যদি (LONGINT_OVERFLOW < xstrtoimax (opt, ফাঁকা, 10, &আকার_2, "")
      || আকার_2 < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  ফেরত MIN (আকার_2, IDX_MAX);
}

স্থির bool
newline_or_blank (mcel_t g)
{
  ফেরত g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

স্থির অক্ষর *
find_field (কাঠামো linebuffer ধ্রুবক *সারি, idx_t *plen)
{
  অক্ষর *lp = সারি->বাফার;
  অক্ষর ধ্রুবক *lim = lp + সারি->দৈর্ঘ্য - 1;

  জন্য (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  জন্য (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_scan (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  যদি (lim - lp <= check_chars)
    len = lim - lp;
  অন্যথা যদি (MB_CUR_MAX <= 1)
    len = check_chars;
  অন্যথা
    {
      অক্ষর *ep = lp;
      জন্য (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_scan (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  ফেরত lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

স্থির bool
different (অক্ষর *old, অক্ষর *new, idx_t oldlen, idx_t newlen)
{
  যদি (ignore_case)
    ফেরত oldlen != newlen || memcasecmp (old, new, oldlen);
  অন্যথা
    ফেরত oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

স্থির শূন্য
writeline (কাঠামো linebuffer ধ্রুবক *সারি,
           bool match, intmax_t linecount)
{
  যদি (! (linecount == 0 ? output_unique
         : !match ? output_first_repeated
         : output_later_repeated))
    ফেরত;

  যদি (গুনতি_occurrences)
    {
      অক্ষর buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      অক্ষর *end = buf + আকার buf;
      অক্ষর *p = end;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      করো
        *--p = '0' + i % 10;
      যখন ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      যখন (end - p < 8)
        *--p = ' ';
      ধ্রুবক idx_t nbytes = end - p;
      যদি (fwrite (p, আকার (অক্ষর), nbytes, stdout) != nbytes)
        write_error ();
    }

  যদি (fwrite (সারি->বাফার, আকার (অক্ষর), সারি->দৈর্ঘ্য, stdout)
      != সারি->দৈর্ঘ্য)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

স্থির শূন্য
check_file (অক্ষর ধ্রুবক *infile, অক্ষর ধ্রুবক *outfile, অক্ষর delimiter)
{
  কাঠামো linebuffer lb1, lb2;
  কাঠামো linebuffer *thisline, *prevline;

  যদি (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  যদি (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
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
  যদি (output_unique && output_first_repeated && !গুনতি_occurrences)
    {
      অক্ষর *prevfield = ফাঁকা;
      idx_t prevlen;
      bool first_group_printed = false;

      যখন (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          অক্ষর *thisfield = find_field (thisline, &thislen);
          bool new_group = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          যদি (new_group && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (first_group_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            putchar (delimiter);

          যদি (new_group || grouping != GM_NONE)
            {
              যদি (fwrite (thisline->বাফার, আকার (অক্ষর), thisline->দৈর্ঘ্য,
                  stdout) != thisline->দৈর্ঘ্য)
                write_error ();

              swap_সারিগুলি (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              first_group_printed = true;
            }
        }
      যদি ((grouping == GM_BOTH || grouping == GM_APPEND) && first_group_printed)
        putchar (delimiter);
    }
  অন্যথা
    {
      যদি (!readlinebuffer_delim (prevline, stdin, delimiter))
        যাও closefiles;

      idx_t prevlen;
      অক্ষর *prevfield = find_field (prevline, &prevlen);
      intmax_t match_গুনতি = 0;
      bool first_delimiter = true;

      যখন (!feof (stdin))
        {
          যদি (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              যদি (ferror (stdin))
                যাও closefiles;
              বিরতি;
            }
          idx_t thislen;
          অক্ষর *thisfield = find_field (thisline, &thislen);
          bool match = !different (thisfield, prevfield, thislen, prevlen);
          match_গুনতি += match;

          যদি (match_গুনতি == INTMAX_MAX)
            {
              যদি (গুনতি_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              match_গুনতি--;
            }

          যদি (delimit_groups != DM_NONE)
            {
              যদি (!match)
                {
                  যদি (match_গুনতি) /* a previous match */
                    first_delimiter = false; /* Only used when DM_SEPARATE */
                }
              অন্যথা যদি (match_গুনতি == 1)
                {
                  যদি ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !first_delimiter))
                    putchar (delimiter);
                }
            }

          যদি (!match || output_later_repeated)
            {
              writeline (prevline, match, match_গুনতি);
              swap_সারিগুলি (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              যদি (!match)
                match_গুনতি = 0;
            }
        }

      writeline (prevline, false, match_গুনতি);
    }

 closefiles:
  যদি (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  মুক্ত (lb1.বাফার);
  মুক্ত (lb2.বাফার);
}

গণনা Skip_field_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ফাঁকা);
  গণনা Skip_field_option_type skip_field_option_type = SFO_NONE;
  পূর্ণসংখ্যা nfiles = 0;
  অক্ষর ধ্রুবক *file[2];
  অক্ষর delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_used = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      যদি (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, ফাঁকা))
              == -1))
        {
          যদি (argc <= optind)
            বিরতি;
          যদি (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      অন্যথা নির্বাচন (optc)
        {
        ক্ষেত্র 1:
          {
            intmax_t আকার_2;
            যদি (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, ফাঁকা, 10, &আকার_2, "")
                    <= LONGINT_OVERFLOW))
              skip_chars = MIN (আকার_2, IDX_MAX);
            অন্যথা যদি (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            অন্যথা
              file[nfiles++] = optarg;
          }
          বিরতি;

        ক্ষেত্র '0':
        ক্ষেত্র '1':
        ক্ষেত্র '2':
        ক্ষেত্র '3':
        ক্ষেত্র '4':
        ক্ষেত্র '5':
        ক্ষেত্র '6':
        ক্ষেত্র '7':
        ক্ষেত্র '8':
        ক্ষেত্র '9':
          {
            যদি (skip_field_option_type == SFO_NEW)
              skip_fields = 0;

            যদি (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_field_option_type = SFO_OBSOLETE;
          }
          বিরতি;

        ক্ষেত্র 'c':
          গুনতি_occurrences = true;
          output_option_used = true;
          বিরতি;

        ক্ষেত্র 'd':
          output_unique = false;
          output_option_used = true;
          বিরতি;

        ক্ষেত্র 'D':
          output_unique = false;
          output_later_repeated = true;
          যদি (optarg == ফাঁকা)
            delimit_groups = DM_NONE;
          অন্যথা
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_method_string,
                                        delimit_method_map);
          output_option_used = true;
          বিরতি;

        ক্ষেত্র GROUP_OPTION:
          যদি (optarg == ফাঁকা)
            grouping = GM_SEPARATE;
          অন্যথা
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_method_string,
                                  grouping_method_map);
          বিরতি;

        ক্ষেত্র 'f':
          skip_field_option_type = SFO_NEW;
          skip_fields = আকার_opt (optarg,
                                  N_("invalid number of fields to skip"));
          বিরতি;

        ক্ষেত্র 'i':
          ignore_case = true;
          বিরতি;

        ক্ষেত্র 's':
          skip_chars = আকার_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          বিরতি;

        ক্ষেত্র 'u':
          output_first_repeated = false;
          output_option_used = true;
          বিরতি;

        ক্ষেত্র 'w':
          check_chars = আকার_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          বিরতি;

        ক্ষেত্র 'z':
          delimiter = '\0';
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  যদি (grouping != GM_NONE && output_option_used)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  যদি (grouping != GM_NONE && গুনতি_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  যদি (গুনতি_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  ফেরত EXIT_SUCCESS;
}
