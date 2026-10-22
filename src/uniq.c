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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "argmatch.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "linebuffer.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "mcel.h"
#ଅନ୍ତର୍ଭାଵ "posixver.h"
#ଅନ୍ତର୍ଭାଵ "skipchars.h"
#ଅନ୍ତର୍ଭାଵ "stdio--.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"
#ଅନ୍ତର୍ଭାଵ "memcasecmp.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "uniq"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ସ୍ଥିର ଶୂନ୍ୟ
swap_ଧାଡ଼ିଗୁଡ଼ିକ (ସଂରଚନା linebuffer **a, ସଂରଚନା linebuffer **b)
{
  ସଂରଚନା linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
ସ୍ଥିର idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
ସ୍ଥିର idx_t skip_chars = false;

/* Number of chars to compare. */
ସ୍ଥିର idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
ସ୍ଥିର bool ଗଣନା_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
ସ୍ଥିର bool output_unique = true;
ସ୍ଥିର bool output_first_repeated = true;
ସ୍ଥିର bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
ସ୍ଥିର bool ignore_case;

ଗଣନା delimit_method
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ delimit_method_string[] =
{
  "none", "prepend", "separate", ଖାଲି
};

ସ୍ଥିର ଗଣନା delimit_method ଅଚଳ delimit_method_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
ସ୍ଥିର ଗଣନା delimit_method delimit_groups = DM_NONE;

ଗଣନା grouping_method
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

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ grouping_method_string[] =
{
  "prepend", "append", "separate", "both", ଖାଲି
};

ସ୍ଥିର ଗଣନା grouping_method ଅଚଳ grouping_method_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

ସ୍ଥିର ଗଣନା grouping_method grouping = GM_NONE;

ଗଣନା
{
  GROUP_OPTION = CHAR_MAX + 1
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"count", no_argument, ଖାଲି, 'c'},
  {"repeated", no_argument, ଖାଲି, 'd'},
  {"all-repeated", optional_argument, ଖାଲି, 'D'},
  {"group", optional_argument, ଖାଲି, GROUP_OPTION},
  {"ignore-case", no_argument, ଖାଲି, 'i'},
  {"unique", no_argument, ଖାଲି, 'u'},
  {"skip-fields", required_argument, ଖାଲି, 'f'},
  {"skip-chars", required_argument, ଖାଲି, 's'},
  {"check-chars", required_argument, ଖାଲି, 'w'},
  {"zero-terminated", no_argument, ଖାଲି, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [ନିବେଶ_2 [OUTPUT]]\n\
"),
              program_name);
      fputs (_("\
Filter adjacent matching ଧାଡ଼ିଗୁଡ଼ିକ from ନିବେଶ_2 (or standard ନିବେଶ),\n\
writing to OUTPUT (or standard output).\n\
\n\
With no options, matching ଧାଡ଼ିଗୁଡ଼ିକ are merged to the first occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --ଗଣନା_2\n\
         prefix ଧାଡ଼ିଗୁଡ଼ିକ by the ସଂଖ୍ୟା of occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate ଧାଡ଼ିଗୁଡ଼ିକ, one ପାଇଁ each group\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate ଧାଡ଼ିଗୁଡ଼ିକ\n\
"));
     oputs (_("\
      --all-repeated[=METHOD]\n\
         like -D, but allow separating groups with an empty ଧାଡ଼ି;\n\
         METHOD={none(ପୂର୍ବନିର୍ଦ୍ଧାରିତ),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the first N fields\n\
"));
     oputs (_("\
      --group[=METHOD]\n\
         show all ବସ୍ତୁଗୁଡ଼ିକ, separating groups with an empty ଧାଡ଼ି;\n\
         METHOD={separate(ପୂର୍ବନିର୍ଦ୍ଧାରିତ),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-ସ୍ଥିତି\n\
         ignore differences in ସ୍ଥିତି when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the first N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique ଧାଡ଼ିଗୁଡ଼ିକ\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         ଧାଡ଼ି delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no more than N characters in ଧାଡ଼ିଗୁଡ଼ିକ\n\
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
'uniq' does not detect repeated ଧାଡ଼ିଗୁଡ଼ିକ unless they are adjacent.\n\
You may want to sort the ନିବେଶ first, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ସ୍ଥିର bool
strict_posix2 (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା posix_ver = posix2_version ();
  ଫେରନ୍ତୁ 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

ସ୍ଥିର idx_t
size_opt (ଅକ୍ଷର ଅଚଳ *opt, ଅକ୍ଷର ଅଚଳ *msgid)
{
  intmax_t size;
  ଯଦି (LONGINT_OVERFLOW < xstrtoimax (opt, ଖାଲି, 10, &size, "")
      || size < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  ଫେରନ୍ତୁ MIN (size, IDX_MAX);
}

ସ୍ଥିର bool
newline_or_blank (mcel_t g)
{
  ଫେରନ୍ତୁ g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

ସ୍ଥିର ଅକ୍ଷର *
find_field (ସଂରଚନା linebuffer ଅଚଳ *ଧାଡ଼ି, idx_t *plen)
{
  ଅକ୍ଷର *lp = ଧାଡ଼ି->buffer;
  ଅକ୍ଷର ଅଚଳ *lim = lp + ଧାଡ଼ି->length - 1;

  ପାଇଁ (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  ପାଇଁ (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_scan (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  ଯଦି (lim - lp <= check_chars)
    len = lim - lp;
  ଅନ୍ୟଥା ଯଦି (MB_CUR_MAX <= 1)
    len = check_chars;
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର *ep = lp;
      ପାଇଁ (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_scan (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  ଫେରନ୍ତୁ lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

ସ୍ଥିର bool
different (ଅକ୍ଷର *old, ଅକ୍ଷର *new, idx_t oldlen, idx_t newlen)
{
  ଯଦି (ignore_case)
    ଫେରନ୍ତୁ oldlen != newlen || memcasecmp (old, new, oldlen);
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

ସ୍ଥିର ଶୂନ୍ୟ
writeline (ସଂରଚନା linebuffer ଅଚଳ *ଧାଡ଼ି,
           bool match, intmax_t linecount)
{
  ଯଦି (! (linecount == 0 ? output_unique
         : !match ? output_first_repeated
         : output_later_repeated))
    ଫେରନ୍ତୁ;

  ଯଦି (ଗଣନା_occurrences)
    {
      ଅକ୍ଷର buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      ଅକ୍ଷର *end = buf + ଆକାର buf;
      ଅକ୍ଷର *p = end;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      କର
        *--p = '0' + i % 10;
      ଯେତେବେଳେ ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      ଯେତେବେଳେ (end - p < 8)
        *--p = ' ';
      ଅଚଳ idx_t nbytes = end - p;
      ଯଦି (fwrite (p, ଆକାର (ଅକ୍ଷର), nbytes, stdout) != nbytes)
        write_error ();
    }

  ଯଦି (fwrite (ଧାଡ଼ି->buffer, ଆକାର (ଅକ୍ଷର), ଧାଡ଼ି->length, stdout)
      != ଧାଡ଼ି->length)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

ସ୍ଥିର ଶୂନ୍ୟ
check_file (ଅକ୍ଷର ଅଚଳ *infile, ଅକ୍ଷର ଅଚଳ *outfile, ଅକ୍ଷର delimiter)
{
  ସଂରଚନା linebuffer lb1, lb2;
  ସଂରଚନା linebuffer *thisline, *prevline;

  ଯଦି (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  ଯଦି (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
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
  ଯଦି (output_unique && output_first_repeated && !ଗଣନା_occurrences)
    {
      ଅକ୍ଷର *prevfield = ଖାଲି;
      idx_t prevlen;
      bool first_group_printed = false;

      ଯେତେବେଳେ (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          ଅକ୍ଷର *thisfield = find_field (thisline, &thislen);
          bool new_group = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          ଯଦି (new_group && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (first_group_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            putchar (delimiter);

          ଯଦି (new_group || grouping != GM_NONE)
            {
              ଯଦି (fwrite (thisline->buffer, ଆକାର (ଅକ୍ଷର), thisline->length,
                  stdout) != thisline->length)
                write_error ();

              swap_ଧାଡ଼ିଗୁଡ଼ିକ (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              first_group_printed = true;
            }
        }
      ଯଦି ((grouping == GM_BOTH || grouping == GM_APPEND) && first_group_printed)
        putchar (delimiter);
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (!readlinebuffer_delim (prevline, stdin, delimiter))
        ଯାଅ closefiles;

      idx_t prevlen;
      ଅକ୍ଷର *prevfield = find_field (prevline, &prevlen);
      intmax_t match_ଗଣନା = 0;
      bool first_delimiter = true;

      ଯେତେବେଳେ (!feof (stdin))
        {
          ଯଦି (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              ଯଦି (ferror (stdin))
                ଯାଅ closefiles;
              ଭାଙ୍ଗ;
            }
          idx_t thislen;
          ଅକ୍ଷର *thisfield = find_field (thisline, &thislen);
          bool match = !different (thisfield, prevfield, thislen, prevlen);
          match_ଗଣନା += match;

          ଯଦି (match_ଗଣନା == INTMAX_MAX)
            {
              ଯଦି (ଗଣନା_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              match_ଗଣନା--;
            }

          ଯଦି (delimit_groups != DM_NONE)
            {
              ଯଦି (!match)
                {
                  ଯଦି (match_ଗଣନା) /* a previous match */
                    first_delimiter = false; /* Only used when DM_SEPARATE */
                }
              ଅନ୍ୟଥା ଯଦି (match_ଗଣନା == 1)
                {
                  ଯଦି ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !first_delimiter))
                    putchar (delimiter);
                }
            }

          ଯଦି (!match || output_later_repeated)
            {
              writeline (prevline, match, match_ଗଣନା);
              swap_ଧାଡ଼ିଗୁଡ଼ିକ (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              ଯଦି (!match)
                match_ଗଣନା = 0;
            }
        }

      writeline (prevline, false, match_ଗଣନା);
    }

 closefiles:
  ଯଦି (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  ମୁକ୍ତ (lb1.buffer);
  ମୁକ୍ତ (lb2.buffer);
}

ଗଣନା Skip_field_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ଖାଲି);
  ଗଣନା Skip_field_option_type skip_field_option_type = SFO_NONE;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfiles = 0;
  ଅକ୍ଷର ଅଚଳ *file[2];
  ଅକ୍ଷର delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_used = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      ଯଦି (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, ଖାଲି))
              == -1))
        {
          ଯଦି (argc <= optind)
            ଭାଙ୍ଗ;
          ଯଦି (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      ଅନ୍ୟଥା ବାଛ (optc)
        {
        ସ୍ଥିତି 1:
          {
            intmax_t size;
            ଯଦି (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, ଖାଲି, 10, &size, "")
                    <= LONGINT_OVERFLOW))
              skip_chars = MIN (size, IDX_MAX);
            ଅନ୍ୟଥା ଯଦି (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            ଅନ୍ୟଥା
              file[nfiles++] = optarg;
          }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '0':
        ସ୍ଥିତି '1':
        ସ୍ଥିତି '2':
        ସ୍ଥିତି '3':
        ସ୍ଥିତି '4':
        ସ୍ଥିତି '5':
        ସ୍ଥିତି '6':
        ସ୍ଥିତି '7':
        ସ୍ଥିତି '8':
        ସ୍ଥିତି '9':
          {
            ଯଦି (skip_field_option_type == SFO_NEW)
              skip_fields = 0;

            ଯଦି (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_field_option_type = SFO_OBSOLETE;
          }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'c':
          ଗଣନା_occurrences = true;
          output_option_used = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'd':
          output_unique = false;
          output_option_used = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'D':
          output_unique = false;
          output_later_repeated = true;
          ଯଦି (optarg == ଖାଲି)
            delimit_groups = DM_NONE;
          ଅନ୍ୟଥା
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_method_string,
                                        delimit_method_map);
          output_option_used = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି GROUP_OPTION:
          ଯଦି (optarg == ଖାଲି)
            grouping = GM_SEPARATE;
          ଅନ୍ୟଥା
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_method_string,
                                  grouping_method_map);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'f':
          skip_field_option_type = SFO_NEW;
          skip_fields = size_opt (optarg,
                                  N_("invalid number of fields to skip"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'i':
          ignore_case = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 's':
          skip_chars = size_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'u':
          output_first_repeated = false;
          output_option_used = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'w':
          check_chars = size_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'z':
          delimiter = '\0';
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  ଯଦି (grouping != GM_NONE && output_option_used)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (grouping != GM_NONE && ଗଣନା_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (ଗଣନା_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
