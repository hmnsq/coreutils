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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "argmatch.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "linebuffer.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "mcel.h"
#அந்தர்பாவ "posixver.h"
#அந்தர்பாவ "skipchars.h"
#அந்தர்பாவ "stdio--.h"
#அந்தர்பாவ "xstrtol.h"
#அந்தர்பாவ "memcasecmp.h"
#அந்தர்பாவ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "uniq"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

நிலையான வெறுமை
swap_வரிகள் (கட்டமைப்பு linebuffer **a, கட்டமைப்பு linebuffer **b)
{
  கட்டமைப்பு linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
நிலையான idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
நிலையான idx_t skip_chars = false;

/* Number of chars to compare. */
நிலையான idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
நிலையான bool மொத்தம்_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
நிலையான bool output_unique = true;
நிலையான bool output_first_repeated = true;
நிலையான bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
நிலையான bool ignore_case;

எண்ணிக்கை delimit_method
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

நிலையான எழுத்து மாறிலி *மாறிலி delimit_method_string[] =
{
  "none", "prepend", "separate", வெற்று
};

நிலையான எண்ணிக்கை delimit_method மாறிலி delimit_method_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
நிலையான எண்ணிக்கை delimit_method delimit_groups = DM_NONE;

எண்ணிக்கை grouping_method
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

நிலையான எழுத்து மாறிலி *மாறிலி grouping_method_string[] =
{
  "prepend", "append", "separate", "both", வெற்று
};

நிலையான எண்ணிக்கை grouping_method மாறிலி grouping_method_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

நிலையான எண்ணிக்கை grouping_method grouping = GM_NONE;

எண்ணிக்கை
{
  GROUP_OPTION = CHAR_MAX + 1
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"count", no_argument, வெற்று, 'c'},
  {"repeated", no_argument, வெற்று, 'd'},
  {"all-repeated", optional_argument, வெற்று, 'D'},
  {"group", optional_argument, வெற்று, GROUP_OPTION},
  {"ignore-case", no_argument, வெற்று, 'i'},
  {"unique", no_argument, வெற்று, 'u'},
  {"skip-fields", required_argument, வெற்று, 'f'},
  {"skip-chars", required_argument, வெற்று, 's'},
  {"check-chars", required_argument, வெற்று, 'w'},
  {"zero-terminated", no_argument, வெற்று, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [உள்ளீடு_2 [OUTPUT]]\n\
"),
              program_name);
      fputs (_("\
Filter adjacent matching வரிகள் from உள்ளீடு_2 (or standard உள்ளீடு),\n\
writing to OUTPUT (or standard output).\n\
\n\
With no options, matching வரிகள் are merged to the first occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --மொத்தம்\n\
         prefix வரிகள் by the எண் of occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate வரிகள், one க்கு each group\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate வரிகள்\n\
"));
     oputs (_("\
      --all-repeated[=METHOD]\n\
         like -D, but allow separating groups with an empty வரி;\n\
         METHOD={none(இயல்பு),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the first N fields\n\
"));
     oputs (_("\
      --group[=METHOD]\n\
         show all பொருட்கள், separating groups with an empty வரி;\n\
         METHOD={separate(இயல்பு),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-நிலை\n\
         ignore differences in நிலை when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the first N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique வரிகள்\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         வரி delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no more than N characters in வரிகள்\n\
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
'uniq' does not detect repeated வரிகள் unless they are adjacent.\n\
You may want to sort the உள்ளீடு first, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

நிலையான bool
strict_posix2 (வெறுமை)
{
  முழு posix_ver = posix2_version ();
  திரும்பு 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

நிலையான idx_t
size_opt (எழுத்து மாறிலி *opt, எழுத்து மாறிலி *msgid)
{
  intmax_t size;
  எனில் (LONGINT_OVERFLOW < xstrtoimax (opt, வெற்று, 10, &size, "")
      || size < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  திரும்பு MIN (size, IDX_MAX);
}

நிலையான bool
newline_or_blank (mcel_t g)
{
  திரும்பு g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

நிலையான எழுத்து *
find_field (கட்டமைப்பு linebuffer மாறிலி *வரி, idx_t *plen)
{
  எழுத்து *lp = வரி->buffer;
  எழுத்து மாறிலி *lim = lp + வரி->length - 1;

  க்கு (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  க்கு (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_scan (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  எனில் (lim - lp <= check_chars)
    len = lim - lp;
  இல்லையெனில் எனில் (MB_CUR_MAX <= 1)
    len = check_chars;
  இல்லையெனில்
    {
      எழுத்து *ep = lp;
      க்கு (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_scan (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  திரும்பு lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

நிலையான bool
different (எழுத்து *old, எழுத்து *new, idx_t oldlen, idx_t newlen)
{
  எனில் (ignore_case)
    திரும்பு oldlen != newlen || memcasecmp (old, new, oldlen);
  இல்லையெனில்
    திரும்பு oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

நிலையான வெறுமை
writeline (கட்டமைப்பு linebuffer மாறிலி *வரி,
           bool match, intmax_t linecount)
{
  எனில் (! (linecount == 0 ? output_unique
         : !match ? output_first_repeated
         : output_later_repeated))
    திரும்பு;

  எனில் (மொத்தம்_occurrences)
    {
      எழுத்து buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      எழுத்து *end = buf + அளவு buf;
      எழுத்து *p = end;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      செய்
        *--p = '0' + i % 10;
      போது ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      போது (end - p < 8)
        *--p = ' ';
      மாறிலி idx_t nbytes = end - p;
      எனில் (fwrite (p, அளவு (எழுத்து), nbytes, stdout) != nbytes)
        write_error ();
    }

  எனில் (fwrite (வரி->buffer, அளவு (எழுத்து), வரி->length, stdout)
      != வரி->length)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

நிலையான வெறுமை
check_file (எழுத்து மாறிலி *infile, எழுத்து மாறிலி *outfile, எழுத்து delimiter)
{
  கட்டமைப்பு linebuffer lb1, lb2;
  கட்டமைப்பு linebuffer *thisline, *prevline;

  எனில் (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  எனில் (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
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
  எனில் (output_unique && output_first_repeated && !மொத்தம்_occurrences)
    {
      எழுத்து *prevfield = வெற்று;
      idx_t prevlen;
      bool first_group_printed = false;

      போது (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          எழுத்து *thisfield = find_field (thisline, &thislen);
          bool new_group = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          எனில் (new_group && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (first_group_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            putchar (delimiter);

          எனில் (new_group || grouping != GM_NONE)
            {
              எனில் (fwrite (thisline->buffer, அளவு (எழுத்து), thisline->length,
                  stdout) != thisline->length)
                write_error ();

              swap_வரிகள் (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              first_group_printed = true;
            }
        }
      எனில் ((grouping == GM_BOTH || grouping == GM_APPEND) && first_group_printed)
        putchar (delimiter);
    }
  இல்லையெனில்
    {
      எனில் (!readlinebuffer_delim (prevline, stdin, delimiter))
        செல் closefiles;

      idx_t prevlen;
      எழுத்து *prevfield = find_field (prevline, &prevlen);
      intmax_t match_மொத்தம் = 0;
      bool first_delimiter = true;

      போது (!feof (stdin))
        {
          எனில் (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              எனில் (ferror (stdin))
                செல் closefiles;
              நிறுத்து;
            }
          idx_t thislen;
          எழுத்து *thisfield = find_field (thisline, &thislen);
          bool match = !different (thisfield, prevfield, thislen, prevlen);
          match_மொத்தம் += match;

          எனில் (match_மொத்தம் == INTMAX_MAX)
            {
              எனில் (மொத்தம்_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              match_மொத்தம்--;
            }

          எனில் (delimit_groups != DM_NONE)
            {
              எனில் (!match)
                {
                  எனில் (match_மொத்தம்) /* a previous match */
                    first_delimiter = false; /* Only used when DM_SEPARATE */
                }
              இல்லையெனில் எனில் (match_மொத்தம் == 1)
                {
                  எனில் ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !first_delimiter))
                    putchar (delimiter);
                }
            }

          எனில் (!match || output_later_repeated)
            {
              writeline (prevline, match, match_மொத்தம்);
              swap_வரிகள் (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              எனில் (!match)
                match_மொத்தம் = 0;
            }
        }

      writeline (prevline, false, match_மொத்தம்);
    }

 closefiles:
  எனில் (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  விடுவி (lb1.buffer);
  விடுவி (lb2.buffer);
}

எண்ணிக்கை Skip_field_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != வெற்று);
  எண்ணிக்கை Skip_field_option_type skip_field_option_type = SFO_NONE;
  முழு nfiles = 0;
  எழுத்து மாறிலி *file[2];
  எழுத்து delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_used = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      எனில் (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, வெற்று))
              == -1))
        {
          எனில் (argc <= optind)
            நிறுத்து;
          எனில் (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      இல்லையெனில் தேர்வு (optc)
        {
        நிலை 1:
          {
            intmax_t size;
            எனில் (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, வெற்று, 10, &size, "")
                    <= LONGINT_OVERFLOW))
              skip_chars = MIN (size, IDX_MAX);
            இல்லையெனில் எனில் (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            இல்லையெனில்
              file[nfiles++] = optarg;
          }
          நிறுத்து;

        நிலை '0':
        நிலை '1':
        நிலை '2':
        நிலை '3':
        நிலை '4':
        நிலை '5':
        நிலை '6':
        நிலை '7':
        நிலை '8':
        நிலை '9':
          {
            எனில் (skip_field_option_type == SFO_NEW)
              skip_fields = 0;

            எனில் (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_field_option_type = SFO_OBSOLETE;
          }
          நிறுத்து;

        நிலை 'c':
          மொத்தம்_occurrences = true;
          output_option_used = true;
          நிறுத்து;

        நிலை 'd':
          output_unique = false;
          output_option_used = true;
          நிறுத்து;

        நிலை 'D':
          output_unique = false;
          output_later_repeated = true;
          எனில் (optarg == வெற்று)
            delimit_groups = DM_NONE;
          இல்லையெனில்
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_method_string,
                                        delimit_method_map);
          output_option_used = true;
          நிறுத்து;

        நிலை GROUP_OPTION:
          எனில் (optarg == வெற்று)
            grouping = GM_SEPARATE;
          இல்லையெனில்
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_method_string,
                                  grouping_method_map);
          நிறுத்து;

        நிலை 'f':
          skip_field_option_type = SFO_NEW;
          skip_fields = size_opt (optarg,
                                  N_("invalid number of fields to skip"));
          நிறுத்து;

        நிலை 'i':
          ignore_case = true;
          நிறுத்து;

        நிலை 's':
          skip_chars = size_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          நிறுத்து;

        நிலை 'u':
          output_first_repeated = false;
          output_option_used = true;
          நிறுத்து;

        நிலை 'w':
          check_chars = size_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          நிறுத்து;

        நிலை 'z':
          delimiter = '\0';
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  எனில் (grouping != GM_NONE && output_option_used)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  எனில் (grouping != GM_NONE && மொத்தம்_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  எனில் (மொத்தம்_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  திரும்பு EXIT_SUCCESS;
}
