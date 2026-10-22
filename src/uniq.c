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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "argmatch.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "linebuffer.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "mcel.h"
#અન્તર્ભાવ "posixver.h"
#અન્તર્ભાવ "skipchars.h"
#અન્તર્ભાવ "stdio--.h"
#અન્તર્ભાવ "xstrtol.h"
#અન્તર્ભાવ "memcasecmp.h"
#અન્તર્ભાવ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "uniq"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

સ્થિર શૂન્ય
swap_લીટીઓ (સંરચના linebuffer **a, સંરચના linebuffer **b)
{
  સંરચના linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
સ્થિર idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
સ્થિર idx_t skip_chars = false;

/* Number of chars to compare. */
સ્થિર idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
સ્થિર bool ગણતરી_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
સ્થિર bool output_unique = true;
સ્થિર bool output_first_repeated = true;
સ્થિર bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
સ્થિર bool ignore_case;

ગણના delimit_method
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

સ્થિર અક્ષર અચલ *અચલ delimit_method_string[] =
{
  "none", "prepend", "separate", ખાલી
};

સ્થિર ગણના delimit_method અચલ delimit_method_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
સ્થિર ગણના delimit_method delimit_groups = DM_NONE;

ગણના grouping_method
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

સ્થિર અક્ષર અચલ *અચલ grouping_method_string[] =
{
  "prepend", "append", "separate", "both", ખાલી
};

સ્થિર ગણના grouping_method અચલ grouping_method_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

સ્થિર ગણના grouping_method grouping = GM_NONE;

ગણના
{
  GROUP_OPTION = CHAR_MAX + 1
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"count", no_argument, ખાલી, 'c'},
  {"repeated", no_argument, ખાલી, 'd'},
  {"all-repeated", optional_argument, ખાલી, 'D'},
  {"group", optional_argument, ખાલી, GROUP_OPTION},
  {"ignore-case", no_argument, ખાલી, 'i'},
  {"unique", no_argument, ખાલી, 'u'},
  {"skip-fields", required_argument, ખાલી, 'f'},
  {"skip-chars", required_argument, ખાલી, 's'},
  {"check-chars", required_argument, ખાલી, 'w'},
  {"zero-terminated", no_argument, ખાલી, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [આગત_2 [OUTPUT]]\n\
"),
              program_name);
      fputs (_("\
Filter adjacent matching લીટીઓ from આગત_2 (or standard આગત),\n\
writing to OUTPUT (or standard output).\n\
\n\
With no options, matching લીટીઓ are merged to the first occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --ગણતરી\n\
         prefix લીટીઓ by the સંખ્યા of occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate લીટીઓ, one માટે each group\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate લીટીઓ\n\
"));
     oputs (_("\
      --all-repeated[=METHOD]\n\
         like -D, but allow separating groups with an empty લીટી;\n\
         METHOD={none(મૂળભૂત),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the first N fields\n\
"));
     oputs (_("\
      --group[=METHOD]\n\
         show all વસ્તુઓ, separating groups with an empty લીટી;\n\
         METHOD={separate(મૂળભૂત),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-સ્થિતિ\n\
         ignore differences in સ્થિતિ when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the first N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique લીટીઓ\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         લીટી delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no more than N characters in લીટીઓ\n\
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
'uniq' does not detect repeated લીટીઓ unless they are adjacent.\n\
You may want to sort the આગત first, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

સ્થિર bool
strict_posix2 (શૂન્ય)
{
  પૂર્ણાંક posix_ver = posix2_version ();
  પાછા 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

સ્થિર idx_t
size_opt (અક્ષર અચલ *opt, અક્ષર અચલ *msgid)
{
  intmax_t size;
  જો (LONGINT_OVERFLOW < xstrtoimax (opt, ખાલી, 10, &size, "")
      || size < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  પાછા MIN (size, IDX_MAX);
}

સ્થિર bool
newline_or_blank (mcel_t g)
{
  પાછા g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

સ્થિર અક્ષર *
find_field (સંરચના linebuffer અચલ *લીટી, idx_t *plen)
{
  અક્ષર *lp = લીટી->buffer;
  અક્ષર અચલ *lim = lp + લીટી->length - 1;

  માટે (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  માટે (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_scan (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  જો (lim - lp <= check_chars)
    len = lim - lp;
  નહીંતર જો (MB_CUR_MAX <= 1)
    len = check_chars;
  નહીંતર
    {
      અક્ષર *ep = lp;
      માટે (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_scan (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  પાછા lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

સ્થિર bool
different (અક્ષર *old, અક્ષર *new, idx_t oldlen, idx_t newlen)
{
  જો (ignore_case)
    પાછા oldlen != newlen || memcasecmp (old, new, oldlen);
  નહીંતર
    પાછા oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

સ્થિર શૂન્ય
writeline (સંરચના linebuffer અચલ *લીટી,
           bool match, intmax_t linecount)
{
  જો (! (linecount == 0 ? output_unique
         : !match ? output_first_repeated
         : output_later_repeated))
    પાછા;

  જો (ગણતરી_occurrences)
    {
      અક્ષર buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      અક્ષર *end = buf + કદ buf;
      અક્ષર *p = end;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      કરો
        *--p = '0' + i % 10;
      જ્યારે ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      જ્યારે (end - p < 8)
        *--p = ' ';
      અચલ idx_t nbytes = end - p;
      જો (fwrite (p, કદ (અક્ષર), nbytes, stdout) != nbytes)
        write_error ();
    }

  જો (fwrite (લીટી->buffer, કદ (અક્ષર), લીટી->length, stdout)
      != લીટી->length)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

સ્થિર શૂન્ય
check_file (અક્ષર અચલ *infile, અક્ષર અચલ *outfile, અક્ષર delimiter)
{
  સંરચના linebuffer lb1, lb2;
  સંરચના linebuffer *thisline, *prevline;

  જો (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  જો (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
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
  જો (output_unique && output_first_repeated && !ગણતરી_occurrences)
    {
      અક્ષર *prevfield = ખાલી;
      idx_t prevlen;
      bool first_group_printed = false;

      જ્યારે (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          અક્ષર *thisfield = find_field (thisline, &thislen);
          bool new_group = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          જો (new_group && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (first_group_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            putchar (delimiter);

          જો (new_group || grouping != GM_NONE)
            {
              જો (fwrite (thisline->buffer, કદ (અક્ષર), thisline->length,
                  stdout) != thisline->length)
                write_error ();

              swap_લીટીઓ (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              first_group_printed = true;
            }
        }
      જો ((grouping == GM_BOTH || grouping == GM_APPEND) && first_group_printed)
        putchar (delimiter);
    }
  નહીંતર
    {
      જો (!readlinebuffer_delim (prevline, stdin, delimiter))
        જાઓ closefiles;

      idx_t prevlen;
      અક્ષર *prevfield = find_field (prevline, &prevlen);
      intmax_t match_ગણતરી = 0;
      bool first_delimiter = true;

      જ્યારે (!feof (stdin))
        {
          જો (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              જો (ferror (stdin))
                જાઓ closefiles;
              તોડો;
            }
          idx_t thislen;
          અક્ષર *thisfield = find_field (thisline, &thislen);
          bool match = !different (thisfield, prevfield, thislen, prevlen);
          match_ગણતરી += match;

          જો (match_ગણતરી == INTMAX_MAX)
            {
              જો (ગણતરી_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              match_ગણતરી--;
            }

          જો (delimit_groups != DM_NONE)
            {
              જો (!match)
                {
                  જો (match_ગણતરી) /* a previous match */
                    first_delimiter = false; /* Only used when DM_SEPARATE */
                }
              નહીંતર જો (match_ગણતરી == 1)
                {
                  જો ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !first_delimiter))
                    putchar (delimiter);
                }
            }

          જો (!match || output_later_repeated)
            {
              writeline (prevline, match, match_ગણતરી);
              swap_લીટીઓ (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              જો (!match)
                match_ગણતરી = 0;
            }
        }

      writeline (prevline, false, match_ગણતરી);
    }

 closefiles:
  જો (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  મુક્ત (lb1.buffer);
  મુક્ત (lb2.buffer);
}

ગણના Skip_field_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ખાલી);
  ગણના Skip_field_option_type skip_field_option_type = SFO_NONE;
  પૂર્ણાંક nfiles = 0;
  અક્ષર અચલ *file[2];
  અક્ષર delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_used = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      જો (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, ખાલી))
              == -1))
        {
          જો (argc <= optind)
            તોડો;
          જો (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      નહીંતર પસંદગી (optc)
        {
        સ્થિતિ 1:
          {
            intmax_t size;
            જો (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, ખાલી, 10, &size, "")
                    <= LONGINT_OVERFLOW))
              skip_chars = MIN (size, IDX_MAX);
            નહીંતર જો (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            નહીંતર
              file[nfiles++] = optarg;
          }
          તોડો;

        સ્થિતિ '0':
        સ્થિતિ '1':
        સ્થિતિ '2':
        સ્થિતિ '3':
        સ્થિતિ '4':
        સ્થિતિ '5':
        સ્થિતિ '6':
        સ્થિતિ '7':
        સ્થિતિ '8':
        સ્થિતિ '9':
          {
            જો (skip_field_option_type == SFO_NEW)
              skip_fields = 0;

            જો (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_field_option_type = SFO_OBSOLETE;
          }
          તોડો;

        સ્થિતિ 'c':
          ગણતરી_occurrences = true;
          output_option_used = true;
          તોડો;

        સ્થિતિ 'd':
          output_unique = false;
          output_option_used = true;
          તોડો;

        સ્થિતિ 'D':
          output_unique = false;
          output_later_repeated = true;
          જો (optarg == ખાલી)
            delimit_groups = DM_NONE;
          નહીંતર
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_method_string,
                                        delimit_method_map);
          output_option_used = true;
          તોડો;

        સ્થિતિ GROUP_OPTION:
          જો (optarg == ખાલી)
            grouping = GM_SEPARATE;
          નહીંતર
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_method_string,
                                  grouping_method_map);
          તોડો;

        સ્થિતિ 'f':
          skip_field_option_type = SFO_NEW;
          skip_fields = size_opt (optarg,
                                  N_("invalid number of fields to skip"));
          તોડો;

        સ્થિતિ 'i':
          ignore_case = true;
          તોડો;

        સ્થિતિ 's':
          skip_chars = size_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          તોડો;

        સ્થિતિ 'u':
          output_first_repeated = false;
          output_option_used = true;
          તોડો;

        સ્થિતિ 'w':
          check_chars = size_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          તોડો;

        સ્થિતિ 'z':
          delimiter = '\0';
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  જો (grouping != GM_NONE && output_option_used)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  જો (grouping != GM_NONE && ગણતરી_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  જો (ગણતરી_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  પાછા EXIT_SUCCESS;
}
