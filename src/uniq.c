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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "linebuffer.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"
#ಅನ್ತರ್ಭಾವ "posixver.h"
#ಅನ್ತರ್ಭಾವ "skipchars.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"
#ಅನ್ತರ್ಭಾವ "memcasecmp.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "uniq"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ಸ್ಥಿರ ಶೂನ್ಯ
swap_ಸಾಲುಗಳು (ಸಂರಚನಾ linebuffer **a, ಸಂರಚನಾ linebuffer **b)
{
  ಸಂರಚನಾ linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
ಸ್ಥಿರ idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
ಸ್ಥಿರ idx_t skip_chars = false;

/* Number of chars to compare. */
ಸ್ಥಿರ idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
ಸ್ಥಿರ bool ಎಣಿಕೆ_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
ಸ್ಥಿರ bool output_unique = true;
ಸ್ಥಿರ bool output_first_repeated = true;
ಸ್ಥಿರ bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
ಸ್ಥಿರ bool ignore_case;

ಗಣನಾ delimit_method
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ delimit_method_string[] =
{
  "none", "prepend", "separate", ರಿಕ್ತ
};

ಸ್ಥಿರ ಗಣನಾ delimit_method ಧ್ರುವ delimit_method_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
ಸ್ಥಿರ ಗಣನಾ delimit_method delimit_groups = DM_NONE;

ಗಣನಾ grouping_method
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

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ grouping_method_string[] =
{
  "prepend", "append", "separate", "both", ರಿಕ್ತ
};

ಸ್ಥಿರ ಗಣನಾ grouping_method ಧ್ರುವ grouping_method_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

ಸ್ಥಿರ ಗಣನಾ grouping_method grouping = GM_NONE;

ಗಣನಾ
{
  GROUP_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"count", no_argument, ರಿಕ್ತ, 'c'},
  {"repeated", no_argument, ರಿಕ್ತ, 'd'},
  {"all-repeated", optional_argument, ರಿಕ್ತ, 'D'},
  {"group", optional_argument, ರಿಕ್ತ, GROUP_OPTION},
  {"ignore-case", no_argument, ರಿಕ್ತ, 'i'},
  {"unique", no_argument, ರಿಕ್ತ, 'u'},
  {"skip-fields", required_argument, ರಿಕ್ತ, 'f'},
  {"skip-chars", required_argument, ರಿಕ್ತ, 's'},
  {"check-chars", required_argument, ರಿಕ್ತ, 'w'},
  {"zero-terminated", no_argument, ರಿಕ್ತ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [ಒಳಹರಿವು_2 [OUTPUT]]\n\
"),
              program_name);
      fputs (_("\
Filter adjacent matching ಸಾಲುಗಳು from ಒಳಹರಿವು_2 (or standard ಒಳಹರಿವು),\n\
writing to OUTPUT (or standard output).\n\
\n\
With no options, matching ಸಾಲುಗಳು are merged to the first occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --ಎಣಿಕೆ\n\
         prefix ಸಾಲುಗಳು by the ಸಂಖ್ಯೆ of occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate ಸಾಲುಗಳು, one ಪ್ರತಿ each group\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate ಸಾಲುಗಳು\n\
"));
     oputs (_("\
      --all-repeated[=METHOD]\n\
         like -D, but allow separating groups with an empty ಸಾಲು;\n\
         METHOD={none(ಸ್ವಭಾವ),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the first N fields\n\
"));
     oputs (_("\
      --group[=METHOD]\n\
         show all ವಸ್ತುಗಳು, separating groups with an empty ಸಾಲು;\n\
         METHOD={separate(ಸ್ವಭಾವ),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-ಪಕ್ಷ\n\
         ignore differences in ಪಕ್ಷ when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the first N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique ಸಾಲುಗಳು\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no more than N characters in ಸಾಲುಗಳು\n\
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
'uniq' does not detect repeated ಸಾಲುಗಳು unless they are adjacent.\n\
You may want to sort the ಒಳಹರಿವು first, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಸ್ಥಿರ bool
strict_posix2 (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಙ್ಕ posix_ver = posix2_version ();
  ಪ್ರತ್ಯಾಗಮ 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

ಸ್ಥಿರ idx_t
size_opt (ವರ್ಣ ಧ್ರುವ *opt, ವರ್ಣ ಧ್ರುವ *msgid)
{
  intmax_t size;
  ಯದಿ (LONGINT_OVERFLOW < xstrtoimax (opt, ರಿಕ್ತ, 10, &size, "")
      || size < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  ಪ್ರತ್ಯಾಗಮ MIN (size, IDX_MAX);
}

ಸ್ಥಿರ bool
newline_or_blank (mcel_t g)
{
  ಪ್ರತ್ಯಾಗಮ g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

ಸ್ಥಿರ ವರ್ಣ *
find_field (ಸಂರಚನಾ linebuffer ಧ್ರುವ *ಸಾಲು, idx_t *plen)
{
  ವರ್ಣ *lp = ಸಾಲು->buffer;
  ವರ್ಣ ಧ್ರುವ *lim = lp + ಸಾಲು->length - 1;

  ಪ್ರತಿ (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  ಪ್ರತಿ (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_scan (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  ಯದಿ (lim - lp <= check_chars)
    len = lim - lp;
  ಅನ್ಯಥಾ ಯದಿ (MB_CUR_MAX <= 1)
    len = check_chars;
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ *ep = lp;
      ಪ್ರತಿ (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_scan (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  ಪ್ರತ್ಯಾಗಮ lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

ಸ್ಥಿರ bool
different (ವರ್ಣ *old, ವರ್ಣ *new, idx_t oldlen, idx_t newlen)
{
  ಯದಿ (ignore_case)
    ಪ್ರತ್ಯಾಗಮ oldlen != newlen || memcasecmp (old, new, oldlen);
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

ಸ್ಥಿರ ಶೂನ್ಯ
writeline (ಸಂರಚನಾ linebuffer ಧ್ರುವ *ಸಾಲು,
           bool match, intmax_t linecount)
{
  ಯದಿ (! (linecount == 0 ? output_unique
         : !match ? output_first_repeated
         : output_later_repeated))
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (ಎಣಿಕೆ_occurrences)
    {
      ವರ್ಣ buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      ವರ್ಣ *end = buf + ಪರಿಮಾಣ buf;
      ವರ್ಣ *p = end;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      ಕುರು
        *--p = '0' + i % 10;
      ಯಾವತ್ ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      ಯಾವತ್ (end - p < 8)
        *--p = ' ';
      ಧ್ರುವ idx_t nbytes = end - p;
      ಯದಿ (fwrite (p, ಪರಿಮಾಣ (ವರ್ಣ), nbytes, stdout) != nbytes)
        write_error ();
    }

  ಯದಿ (fwrite (ಸಾಲು->buffer, ಪರಿಮಾಣ (ವರ್ಣ), ಸಾಲು->length, stdout)
      != ಸಾಲು->length)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

ಸ್ಥಿರ ಶೂನ್ಯ
check_file (ವರ್ಣ ಧ್ರುವ *infile, ವರ್ಣ ಧ್ರುವ *outfile, ವರ್ಣ delimiter)
{
  ಸಂರಚನಾ linebuffer lb1, lb2;
  ಸಂರಚನಾ linebuffer *thisline, *prevline;

  ಯದಿ (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  ಯದಿ (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
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
  ಯದಿ (output_unique && output_first_repeated && !ಎಣಿಕೆ_occurrences)
    {
      ವರ್ಣ *prevfield = ರಿಕ್ತ;
      idx_t prevlen;
      bool first_group_printed = false;

      ಯಾವತ್ (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          ವರ್ಣ *thisfield = find_field (thisline, &thislen);
          bool new_group = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          ಯದಿ (new_group && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (first_group_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            putchar (delimiter);

          ಯದಿ (new_group || grouping != GM_NONE)
            {
              ಯದಿ (fwrite (thisline->buffer, ಪರಿಮಾಣ (ವರ್ಣ), thisline->length,
                  stdout) != thisline->length)
                write_error ();

              swap_ಸಾಲುಗಳು (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              first_group_printed = true;
            }
        }
      ಯದಿ ((grouping == GM_BOTH || grouping == GM_APPEND) && first_group_printed)
        putchar (delimiter);
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (!readlinebuffer_delim (prevline, stdin, delimiter))
        ಗಚ್ಛ closefiles;

      idx_t prevlen;
      ವರ್ಣ *prevfield = find_field (prevline, &prevlen);
      intmax_t match_ಎಣಿಕೆ = 0;
      bool first_delimiter = true;

      ಯಾವತ್ (!feof (stdin))
        {
          ಯದಿ (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              ಯದಿ (ferror (stdin))
                ಗಚ್ಛ closefiles;
              ಭಙ್ಗ;
            }
          idx_t thislen;
          ವರ್ಣ *thisfield = find_field (thisline, &thislen);
          bool match = !different (thisfield, prevfield, thislen, prevlen);
          match_ಎಣಿಕೆ += match;

          ಯದಿ (match_ಎಣಿಕೆ == INTMAX_MAX)
            {
              ಯದಿ (ಎಣಿಕೆ_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              match_ಎಣಿಕೆ--;
            }

          ಯದಿ (delimit_groups != DM_NONE)
            {
              ಯದಿ (!match)
                {
                  ಯದಿ (match_ಎಣಿಕೆ) /* a previous match */
                    first_delimiter = false; /* Only used when DM_SEPARATE */
                }
              ಅನ್ಯಥಾ ಯದಿ (match_ಎಣಿಕೆ == 1)
                {
                  ಯದಿ ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !first_delimiter))
                    putchar (delimiter);
                }
            }

          ಯದಿ (!match || output_later_repeated)
            {
              writeline (prevline, match, match_ಎಣಿಕೆ);
              swap_ಸಾಲುಗಳು (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              ಯದಿ (!match)
                match_ಎಣಿಕೆ = 0;
            }
        }

      writeline (prevline, false, match_ಎಣಿಕೆ);
    }

 closefiles:
  ಯದಿ (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  ಮುಕ್ತ (lb1.buffer);
  ಮುಕ್ತ (lb2.buffer);
}

ಗಣನಾ Skip_field_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ರಿಕ್ತ);
  ಗಣನಾ Skip_field_option_type skip_field_option_type = SFO_NONE;
  ಪೂರ್ಣಾಙ್ಕ nfiles = 0;
  ವರ್ಣ ಧ್ರುವ *file[2];
  ವರ್ಣ delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_used = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      ಯದಿ (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, ರಿಕ್ತ))
              == -1))
        {
          ಯದಿ (argc <= optind)
            ಭಙ್ಗ;
          ಯದಿ (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      ಅನ್ಯಥಾ ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 1:
          {
            intmax_t size;
            ಯದಿ (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, ರಿಕ್ತ, 10, &size, "")
                    <= LONGINT_OVERFLOW))
              skip_chars = MIN (size, IDX_MAX);
            ಅನ್ಯಥಾ ಯದಿ (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            ಅನ್ಯಥಾ
              file[nfiles++] = optarg;
          }
          ಭಙ್ಗ;

        ಪಕ್ಷ '0':
        ಪಕ್ಷ '1':
        ಪಕ್ಷ '2':
        ಪಕ್ಷ '3':
        ಪಕ್ಷ '4':
        ಪಕ್ಷ '5':
        ಪಕ್ಷ '6':
        ಪಕ್ಷ '7':
        ಪಕ್ಷ '8':
        ಪಕ್ಷ '9':
          {
            ಯದಿ (skip_field_option_type == SFO_NEW)
              skip_fields = 0;

            ಯದಿ (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_field_option_type = SFO_OBSOLETE;
          }
          ಭಙ್ಗ;

        ಪಕ್ಷ 'c':
          ಎಣಿಕೆ_occurrences = true;
          output_option_used = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'd':
          output_unique = false;
          output_option_used = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'D':
          output_unique = false;
          output_later_repeated = true;
          ಯದಿ (optarg == ರಿಕ್ತ)
            delimit_groups = DM_NONE;
          ಅನ್ಯಥಾ
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_method_string,
                                        delimit_method_map);
          output_option_used = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ GROUP_OPTION:
          ಯದಿ (optarg == ರಿಕ್ತ)
            grouping = GM_SEPARATE;
          ಅನ್ಯಥಾ
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_method_string,
                                  grouping_method_map);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'f':
          skip_field_option_type = SFO_NEW;
          skip_fields = size_opt (optarg,
                                  N_("invalid number of fields to skip"));
          ಭಙ್ಗ;

        ಪಕ್ಷ 'i':
          ignore_case = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 's':
          skip_chars = size_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          ಭಙ್ಗ;

        ಪಕ್ಷ 'u':
          output_first_repeated = false;
          output_option_used = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'w':
          check_chars = size_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          ಭಙ್ಗ;

        ಪಕ್ಷ 'z':
          delimiter = '\0';
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  ಯದಿ (grouping != GM_NONE && output_option_used)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (grouping != GM_NONE && ಎಣಿಕೆ_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (ಎಣಿಕೆ_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}
