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
#define PROGRAM_NAME "uniq"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ایستا تهی
swap_سطرها (ساختار linebuffer **a, ساختار linebuffer **b)
{
  ساختار linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
ایستا idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
ایستا idx_t skip_chars = false;

/* Number of chars to compare. */
ایستا idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
ایستا bool شمار_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
ایستا bool output_unique = true;
ایستا bool output_first_repeated = true;
ایستا bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
ایستا bool ignore_case;

شمارش delimit_method
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

ایستا نویسه ثابت *ثابت delimit_method_string[] =
{
  "none", "prepend", "separate", پوچ
};

ایستا شمارش delimit_method ثابت delimit_method_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
ایستا شمارش delimit_method delimit_groups = DM_NONE;

شمارش grouping_method
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

ایستا نویسه ثابت *ثابت grouping_method_string[] =
{
  "prepend", "append", "separate", "both", پوچ
};

ایستا شمارش grouping_method ثابت grouping_method_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

ایستا شمارش grouping_method grouping = GM_NONE;

شمارش
{
  GROUP_OPTION = CHAR_MAX + 1
};

ایستا ساختار option ثابت longopts[] =
{
  {"count", no_argument, پوچ, 'c'},
  {"repeated", no_argument, پوچ, 'd'},
  {"all-repeated", optional_argument, پوچ, 'D'},
  {"group", optional_argument, پوچ, GROUP_OPTION},
  {"ignore-case", no_argument, پوچ, 'i'},
  {"unique", no_argument, پوچ, 'u'},
  {"skip-fields", required_argument, پوچ, 'f'},
  {"skip-chars", required_argument, پوچ, 's'},
  {"check-chars", required_argument, پوچ, 'w'},
  {"zero-terminated", no_argument, پوچ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [درون_داد_2 [OUTPUT]]\n\
"),
              program_name);
      fputs (_("\
Filter adjacent matching سطرها from درون_داد_2 (or standard درون_داد),\n\
writing to OUTPUT (or standard output).\n\
\n\
With no options, matching سطرها are merged to the first occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --شمار\n\
         prefix سطرها by the عدد of occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate سطرها, one برای each group\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate سطرها\n\
"));
     oputs (_("\
      --all-repeated[=METHOD]\n\
         like -D, but allow separating groups with an empty سطر;\n\
         METHOD={none(پیشفرض),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the first N fields\n\
"));
     oputs (_("\
      --group[=METHOD]\n\
         show all اقلام, separating groups with an empty سطر;\n\
         METHOD={separate(پیشفرض),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-حالت\n\
         ignore differences in حالت when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the first N characters\n\
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
         compare no more than N characters in سطرها\n\
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
'uniq' does not detect repeated سطرها unless they are adjacent.\n\
You may want to sort the درون_داد first, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ایستا bool
strict_posix2 (تهی)
{
  صحیح posix_ver = posix2_version ();
  بازگشت 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

ایستا idx_t
size_opt (نویسه ثابت *opt, نویسه ثابت *msgid)
{
  intmax_t size;
  اگر (LONGINT_OVERFLOW < xstrtoimax (opt, پوچ, 10, &size, "")
      || size < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  بازگشت MIN (size, IDX_MAX);
}

ایستا bool
newline_or_blank (mcel_t g)
{
  بازگشت g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

ایستا نویسه *
find_field (ساختار linebuffer ثابت *سطر, idx_t *plen)
{
  نویسه *lp = سطر->buffer;
  نویسه ثابت *lim = lp + سطر->length - 1;

  برای (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  برای (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_scan (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  اگر (lim - lp <= check_chars)
    len = lim - lp;
  وگرنه اگر (MB_CUR_MAX <= 1)
    len = check_chars;
  وگرنه
    {
      نویسه *ep = lp;
      برای (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_scan (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  بازگشت lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

ایستا bool
different (نویسه *old, نویسه *new, idx_t oldlen, idx_t newlen)
{
  اگر (ignore_case)
    بازگشت oldlen != newlen || memcasecmp (old, new, oldlen);
  وگرنه
    بازگشت oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

ایستا تهی
writeline (ساختار linebuffer ثابت *سطر,
           bool match, intmax_t linecount)
{
  اگر (! (linecount == 0 ? output_unique
         : !match ? output_first_repeated
         : output_later_repeated))
    بازگشت;

  اگر (شمار_occurrences)
    {
      نویسه buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      نویسه *end = buf + اندازه buf;
      نویسه *p = end;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      انجامبده
        *--p = '0' + i % 10;
      وقتی ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      وقتی (end - p < 8)
        *--p = ' ';
      ثابت idx_t nbytes = end - p;
      اگر (fwrite (p, اندازه (نویسه), nbytes, stdout) != nbytes)
        write_error ();
    }

  اگر (fwrite (سطر->buffer, اندازه (نویسه), سطر->length, stdout)
      != سطر->length)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

ایستا تهی
check_file (نویسه ثابت *infile, نویسه ثابت *outfile, نویسه delimiter)
{
  ساختار linebuffer lb1, lb2;
  ساختار linebuffer *thisline, *prevline;

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
  اگر (output_unique && output_first_repeated && !شمار_occurrences)
    {
      نویسه *prevfield = پوچ;
      idx_t prevlen;
      bool first_group_printed = false;

      وقتی (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          نویسه *thisfield = find_field (thisline, &thislen);
          bool new_group = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          اگر (new_group && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (first_group_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            putchar (delimiter);

          اگر (new_group || grouping != GM_NONE)
            {
              اگر (fwrite (thisline->buffer, اندازه (نویسه), thisline->length,
                  stdout) != thisline->length)
                write_error ();

              swap_سطرها (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              first_group_printed = true;
            }
        }
      اگر ((grouping == GM_BOTH || grouping == GM_APPEND) && first_group_printed)
        putchar (delimiter);
    }
  وگرنه
    {
      اگر (!readlinebuffer_delim (prevline, stdin, delimiter))
        برو closefiles;

      idx_t prevlen;
      نویسه *prevfield = find_field (prevline, &prevlen);
      intmax_t match_شمار = 0;
      bool first_delimiter = true;

      وقتی (!feof (stdin))
        {
          اگر (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              اگر (ferror (stdin))
                برو closefiles;
              بشکن;
            }
          idx_t thislen;
          نویسه *thisfield = find_field (thisline, &thislen);
          bool match = !different (thisfield, prevfield, thislen, prevlen);
          match_شمار += match;

          اگر (match_شمار == INTMAX_MAX)
            {
              اگر (شمار_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              match_شمار--;
            }

          اگر (delimit_groups != DM_NONE)
            {
              اگر (!match)
                {
                  اگر (match_شمار) /* a previous match */
                    first_delimiter = false; /* Only used when DM_SEPARATE */
                }
              وگرنه اگر (match_شمار == 1)
                {
                  اگر ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !first_delimiter))
                    putchar (delimiter);
                }
            }

          اگر (!match || output_later_repeated)
            {
              writeline (prevline, match, match_شمار);
              swap_سطرها (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              اگر (!match)
                match_شمار = 0;
            }
        }

      writeline (prevline, false, match_شمار);
    }

 closefiles:
  اگر (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  آزاد (lb1.buffer);
  آزاد (lb2.buffer);
}

شمارش Skip_field_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != پوچ);
  شمارش Skip_field_option_type skip_field_option_type = SFO_NONE;
  صحیح nfiles = 0;
  نویسه ثابت *file[2];
  نویسه delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_used = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      اگر (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, پوچ))
              == -1))
        {
          اگر (argc <= optind)
            بشکن;
          اگر (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      وگرنه انتخاب (optc)
        {
        حالت 1:
          {
            intmax_t size;
            اگر (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, پوچ, 10, &size, "")
                    <= LONGINT_OVERFLOW))
              skip_chars = MIN (size, IDX_MAX);
            وگرنه اگر (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            وگرنه
              file[nfiles++] = optarg;
          }
          بشکن;

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
            اگر (skip_field_option_type == SFO_NEW)
              skip_fields = 0;

            اگر (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_field_option_type = SFO_OBSOLETE;
          }
          بشکن;

        حالت 'c':
          شمار_occurrences = true;
          output_option_used = true;
          بشکن;

        حالت 'd':
          output_unique = false;
          output_option_used = true;
          بشکن;

        حالت 'D':
          output_unique = false;
          output_later_repeated = true;
          اگر (optarg == پوچ)
            delimit_groups = DM_NONE;
          وگرنه
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_method_string,
                                        delimit_method_map);
          output_option_used = true;
          بشکن;

        حالت GROUP_OPTION:
          اگر (optarg == پوچ)
            grouping = GM_SEPARATE;
          وگرنه
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_method_string,
                                  grouping_method_map);
          بشکن;

        حالت 'f':
          skip_field_option_type = SFO_NEW;
          skip_fields = size_opt (optarg,
                                  N_("invalid number of fields to skip"));
          بشکن;

        حالت 'i':
          ignore_case = true;
          بشکن;

        حالت 's':
          skip_chars = size_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          بشکن;

        حالت 'u':
          output_first_repeated = false;
          output_option_used = true;
          بشکن;

        حالت 'w':
          check_chars = size_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          بشکن;

        حالت 'z':
          delimiter = '\0';
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  اگر (grouping != GM_NONE && output_option_used)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  اگر (grouping != GM_NONE && شمار_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  اگر (شمار_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  بازگشت EXIT_SUCCESS;
}
