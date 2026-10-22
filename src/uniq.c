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
#define PROGRAM_NAME "uniq"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ساكن فراغ
swap_ليوك (بنية linebuffer **a, بنية linebuffer **b)
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
ساكن bool كدي_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
ساكن bool output_unique = true;
ساكن bool output_first_repeated = true;
ساكن bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
ساكن bool ignore_case;

تعداد delimit_method
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

ساكن حرف ثابت *ثابت delimit_method_string[] =
{
  "none", "prepend", "separate", عدم
};

ساكن تعداد delimit_method ثابت delimit_method_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
ساكن تعداد delimit_method delimit_groups = DM_NONE;

تعداد grouping_method
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

ساكن حرف ثابت *ثابت grouping_method_string[] =
{
  "prepend", "append", "separate", "both", عدم
};

ساكن تعداد grouping_method ثابت grouping_method_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

ساكن تعداد grouping_method grouping = GM_NONE;

تعداد
{
  GROUP_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"count", no_argument, عدم, 'c'},
  {"repeated", no_argument, عدم, 'd'},
  {"all-repeated", optional_argument, عدم, 'D'},
  {"group", optional_argument, عدم, GROUP_OPTION},
  {"ignore-case", no_argument, عدم, 'i'},
  {"unique", no_argument, عدم, 'u'},
  {"skip-fields", required_argument, عدم, 'f'},
  {"skip-chars", required_argument, عدم, 's'},
  {"check-chars", required_argument, عدم, 'w'},
  {"zero-terminated", no_argument, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... [شغرو_2 [OUTPUT]]\n\
"),
              program_name);
      fputs (_("\
Filter adjacent matching ليوك from شغرو_2 (or standard شغرو),\n\
writing to OUTPUT (or standard output).\n\
\n\
With no options, matching ليوك are merged to the first occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --كدي\n\
         prefix ليوك by the لمب_2 of occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only بغ_2 duplicate ليوك, one لكل each group\n\
"));
     oputs (_("\
  -D\n\
         بغ_2 all duplicate ليوك\n\
"));
     oputs (_("\
      --all-repeated[=METHOD]\n\
         like -D, but allow separating groups with an empty لي;\n\
         METHOD={none(افتراضي),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the first N fields\n\
"));
     oputs (_("\
      --group[=METHOD]\n\
         show all ابوبو, separating groups with an empty لي;\n\
         METHOD={separate(افتراضي),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-حالة\n\
         ignore differences in حالة when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the first N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only بغ_2 unique ليوك\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         لي delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no more than N characters in ليوك\n\
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
'uniq' does not detect repeated ليوك unless they are adjacent.\n\
You may want to sort the شغرو first, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ساكن bool
strict_posix2 (فراغ)
{
  صحيح posix_ver = posix2_version ();
  أرجع 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

ساكن idx_t
size_opt (حرف ثابت *opt, حرف ثابت *msgid)
{
  intmax_t size;
  إذا (LONGINT_OVERFLOW < xstrtoimax (opt, عدم, 10, &size, "")
      || size < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  أرجع MIN (size, IDX_MAX);
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
find_field (بنية linebuffer ثابت *لي, idx_t *plen)
{
  حرف *lp = لي->buffer;
  حرف ثابت *lim = lp + لي->length - 1;

  لكل (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  لكل (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_scan (lp, lim).len;

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
        ep += mcel_scan (ep, lim).len;
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
writeline (بنية linebuffer ثابت *لي,
           bool match, intmax_t linecount)
{
  إذا (! (linecount == 0 ? output_unique
         : !match ? output_first_repeated
         : output_later_repeated))
    أرجع;

  إذا (كدي_occurrences)
    {
      حرف buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      حرف *end = buf + حجم buf;
      حرف *p = end;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      افعل
        *--p = '0' + i % 10;
      طالما ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      طالما (end - p < 8)
        *--p = ' ';
      ثابت idx_t nbytes = end - p;
      إذا (fwrite (p, حجم (حرف), nbytes, stdout) != nbytes)
        write_error ();
    }

  إذا (fwrite (لي->buffer, حجم (حرف), لي->length, stdout)
      != لي->length)
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
  إذا (output_unique && output_first_repeated && !كدي_occurrences)
    {
      حرف *prevfield = عدم;
      idx_t prevlen;
      bool first_group_printed = false;

      طالما (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          حرف *thisfield = find_field (thisline, &thislen);
          bool new_group = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          إذا (new_group && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (first_group_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            putchar (delimiter);

          إذا (new_group || grouping != GM_NONE)
            {
              إذا (fwrite (thisline->buffer, حجم (حرف), thisline->length,
                  stdout) != thisline->length)
                write_error ();

              swap_ليوك (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              first_group_printed = true;
            }
        }
      إذا ((grouping == GM_BOTH || grouping == GM_APPEND) && first_group_printed)
        putchar (delimiter);
    }
  وإلا
    {
      إذا (!readlinebuffer_delim (prevline, stdin, delimiter))
        اذهب closefiles;

      idx_t prevlen;
      حرف *prevfield = find_field (prevline, &prevlen);
      intmax_t match_كدي = 0;
      bool first_delimiter = true;

      طالما (!feof (stdin))
        {
          إذا (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              إذا (ferror (stdin))
                اذهب closefiles;
              اكسر;
            }
          idx_t thislen;
          حرف *thisfield = find_field (thisline, &thislen);
          bool match = !different (thisfield, prevfield, thislen, prevlen);
          match_كدي += match;

          إذا (match_كدي == INTMAX_MAX)
            {
              إذا (كدي_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              match_كدي--;
            }

          إذا (delimit_groups != DM_NONE)
            {
              إذا (!match)
                {
                  إذا (match_كدي) /* a previous match */
                    first_delimiter = false; /* Only used when DM_SEPARATE */
                }
              وإلا إذا (match_كدي == 1)
                {
                  إذا ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !first_delimiter))
                    putchar (delimiter);
                }
            }

          إذا (!match || output_later_repeated)
            {
              writeline (prevline, match, match_كدي);
              swap_ليوك (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              إذا (!match)
                match_كدي = 0;
            }
        }

      writeline (prevline, false, match_كدي);
    }

 closefiles:
  إذا (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  حرر (lb1.buffer);
  حرر (lb2.buffer);
}

تعداد Skip_field_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

صحيح
main (صحيح argc, حرف **argv)
{
  صحيح optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);
  تعداد Skip_field_option_type skip_field_option_type = SFO_NONE;
  صحيح nfiles = 0;
  حرف ثابت *file[2];
  حرف delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_used = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
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
            intmax_t size;
            إذا (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, عدم, 10, &size, "")
                    <= LONGINT_OVERFLOW))
              skip_chars = MIN (size, IDX_MAX);
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
            إذا (skip_field_option_type == SFO_NEW)
              skip_fields = 0;

            إذا (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_field_option_type = SFO_OBSOLETE;
          }
          اكسر;

        حالة 'c':
          كدي_occurrences = true;
          output_option_used = true;
          اكسر;

        حالة 'd':
          output_unique = false;
          output_option_used = true;
          اكسر;

        حالة 'D':
          output_unique = false;
          output_later_repeated = true;
          إذا (optarg == عدم)
            delimit_groups = DM_NONE;
          وإلا
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_method_string,
                                        delimit_method_map);
          output_option_used = true;
          اكسر;

        حالة GROUP_OPTION:
          إذا (optarg == عدم)
            grouping = GM_SEPARATE;
          وإلا
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_method_string,
                                  grouping_method_map);
          اكسر;

        حالة 'f':
          skip_field_option_type = SFO_NEW;
          skip_fields = size_opt (optarg,
                                  N_("invalid number of fields to skip"));
          اكسر;

        حالة 'i':
          ignore_case = true;
          اكسر;

        حالة 's':
          skip_chars = size_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          اكسر;

        حالة 'u':
          output_first_repeated = false;
          output_option_used = true;
          اكسر;

        حالة 'w':
          check_chars = size_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          اكسر;

        حالة 'z':
          delimiter = '\0';
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  إذا (grouping != GM_NONE && output_option_used)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  إذا (grouping != GM_NONE && كدي_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  إذا (كدي_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  أرجع EXIT_SUCCESS;
}
