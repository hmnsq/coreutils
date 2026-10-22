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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "argmatch.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "linebuffer.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "mcel.h"
#അന്തര്ഭാവ "posixver.h"
#അന്തര്ഭാവ "skipchars.h"
#അന്തര്ഭാവ "stdio--.h"
#അന്തര്ഭാവ "xstrtol.h"
#അന്തര്ഭാവ "memcasecmp.h"
#അന്തര്ഭാവ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "uniq"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

സ്ഥിതം ശൂന്യം
swap_വരികൾ (ഘടന linebuffer **a, ഘടന linebuffer **b)
{
  ഘടന linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
സ്ഥിതം idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
സ്ഥിതം idx_t skip_chars = false;

/* Number of chars to compare. */
സ്ഥിതം idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
സ്ഥിതം bool എണ്ണം_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
സ്ഥിതം bool output_unique = true;
സ്ഥിതം bool output_first_repeated = true;
സ്ഥിതം bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
സ്ഥിതം bool ignore_case;

എണ്ണൽ delimit_method
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം delimit_method_string[] =
{
  "none", "prepend", "separate", ശൂന്യ
};

സ്ഥിതം എണ്ണൽ delimit_method സ്ഥിരം delimit_method_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
സ്ഥിതം എണ്ണൽ delimit_method delimit_groups = DM_NONE;

എണ്ണൽ grouping_method
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

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം grouping_method_string[] =
{
  "prepend", "append", "separate", "both", ശൂന്യ
};

സ്ഥിതം എണ്ണൽ grouping_method സ്ഥിരം grouping_method_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

സ്ഥിതം എണ്ണൽ grouping_method grouping = GM_NONE;

എണ്ണൽ
{
  GROUP_OPTION = CHAR_MAX + 1
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"count", no_argument, ശൂന്യ, 'c'},
  {"repeated", no_argument, ശൂന്യ, 'd'},
  {"all-repeated", optional_argument, ശൂന്യ, 'D'},
  {"group", optional_argument, ശൂന്യ, GROUP_OPTION},
  {"ignore-case", no_argument, ശൂന്യ, 'i'},
  {"unique", no_argument, ശൂന്യ, 'u'},
  {"skip-fields", required_argument, ശൂന്യ, 'f'},
  {"skip-chars", required_argument, ശൂന്യ, 's'},
  {"check-chars", required_argument, ശൂന്യ, 'w'},
  {"zero-terminated", no_argument, ശൂന്യ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [നിവേശം_2 [OUTPUT]]\n\
"),
              program_name);
      fputs (_("\
Filter adjacent matching വരികൾ from നിവേശം_2 (or standard നിവേശം),\n\
writing to OUTPUT (or standard output).\n\
\n\
With no options, matching വരികൾ are merged to the first occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --എണ്ണം\n\
         prefix വരികൾ by the സംഖ്യ of occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate വരികൾ, one വേണ്ടി each group\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate വരികൾ\n\
"));
     oputs (_("\
      --all-repeated[=METHOD]\n\
         like -D, but allow separating groups with an empty വരി;\n\
         METHOD={none(സ്ഥിരസ്ഥിതി),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the first N fields\n\
"));
     oputs (_("\
      --group[=METHOD]\n\
         show all ഇനങ്ങൾ, separating groups with an empty വരി;\n\
         METHOD={separate(സ്ഥിരസ്ഥിതി),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-അവസ്ഥ\n\
         ignore differences in അവസ്ഥ when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the first N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique വരികൾ\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         വരി delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no more than N characters in വരികൾ\n\
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
'uniq' does not detect repeated വരികൾ unless they are adjacent.\n\
You may want to sort the നിവേശം first, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

സ്ഥിതം bool
strict_posix2 (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ posix_ver = posix2_version ();
  തിരികെ 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

സ്ഥിതം idx_t
size_opt (അക്ഷരം സ്ഥിരം *opt, അക്ഷരം സ്ഥിരം *msgid)
{
  intmax_t size;
  എങ്കിൽ (LONGINT_OVERFLOW < xstrtoimax (opt, ശൂന്യ, 10, &size, "")
      || size < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  തിരികെ MIN (size, IDX_MAX);
}

സ്ഥിതം bool
newline_or_blank (mcel_t g)
{
  തിരികെ g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

സ്ഥിതം അക്ഷരം *
find_field (ഘടന linebuffer സ്ഥിരം *വരി, idx_t *plen)
{
  അക്ഷരം *lp = വരി->buffer;
  അക്ഷരം സ്ഥിരം *lim = lp + വരി->length - 1;

  വേണ്ടി (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  വേണ്ടി (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_scan (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  എങ്കിൽ (lim - lp <= check_chars)
    len = lim - lp;
  അല്ലെങ്കിൽ എങ്കിൽ (MB_CUR_MAX <= 1)
    len = check_chars;
  അല്ലെങ്കിൽ
    {
      അക്ഷരം *ep = lp;
      വേണ്ടി (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_scan (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  തിരികെ lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

സ്ഥിതം bool
different (അക്ഷരം *old, അക്ഷരം *new, idx_t oldlen, idx_t newlen)
{
  എങ്കിൽ (ignore_case)
    തിരികെ oldlen != newlen || memcasecmp (old, new, oldlen);
  അല്ലെങ്കിൽ
    തിരികെ oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

സ്ഥിതം ശൂന്യം
writeline (ഘടന linebuffer സ്ഥിരം *വരി,
           bool match, intmax_t linecount)
{
  എങ്കിൽ (! (linecount == 0 ? output_unique
         : !match ? output_first_repeated
         : output_later_repeated))
    തിരികെ;

  എങ്കിൽ (എണ്ണം_occurrences)
    {
      അക്ഷരം buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      അക്ഷരം *end = buf + വലുപ്പം buf;
      അക്ഷരം *p = end;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      ചെയ്യുക
        *--p = '0' + i % 10;
      ആകുമ്പോൾ ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      ആകുമ്പോൾ (end - p < 8)
        *--p = ' ';
      സ്ഥിരം idx_t nbytes = end - p;
      എങ്കിൽ (fwrite (p, വലുപ്പം (അക്ഷരം), nbytes, stdout) != nbytes)
        write_error ();
    }

  എങ്കിൽ (fwrite (വരി->buffer, വലുപ്പം (അക്ഷരം), വരി->length, stdout)
      != വരി->length)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

സ്ഥിതം ശൂന്യം
check_file (അക്ഷരം സ്ഥിരം *infile, അക്ഷരം സ്ഥിരം *outfile, അക്ഷരം delimiter)
{
  ഘടന linebuffer lb1, lb2;
  ഘടന linebuffer *thisline, *prevline;

  എങ്കിൽ (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  എങ്കിൽ (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
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
  എങ്കിൽ (output_unique && output_first_repeated && !എണ്ണം_occurrences)
    {
      അക്ഷരം *prevfield = ശൂന്യ;
      idx_t prevlen;
      bool first_group_printed = false;

      ആകുമ്പോൾ (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          അക്ഷരം *thisfield = find_field (thisline, &thislen);
          bool new_group = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          എങ്കിൽ (new_group && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (first_group_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            putchar (delimiter);

          എങ്കിൽ (new_group || grouping != GM_NONE)
            {
              എങ്കിൽ (fwrite (thisline->buffer, വലുപ്പം (അക്ഷരം), thisline->length,
                  stdout) != thisline->length)
                write_error ();

              swap_വരികൾ (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              first_group_printed = true;
            }
        }
      എങ്കിൽ ((grouping == GM_BOTH || grouping == GM_APPEND) && first_group_printed)
        putchar (delimiter);
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (!readlinebuffer_delim (prevline, stdin, delimiter))
        പോകുക closefiles;

      idx_t prevlen;
      അക്ഷരം *prevfield = find_field (prevline, &prevlen);
      intmax_t match_എണ്ണം = 0;
      bool first_delimiter = true;

      ആകുമ്പോൾ (!feof (stdin))
        {
          എങ്കിൽ (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              എങ്കിൽ (ferror (stdin))
                പോകുക closefiles;
              നിർത്തുക;
            }
          idx_t thislen;
          അക്ഷരം *thisfield = find_field (thisline, &thislen);
          bool match = !different (thisfield, prevfield, thislen, prevlen);
          match_എണ്ണം += match;

          എങ്കിൽ (match_എണ്ണം == INTMAX_MAX)
            {
              എങ്കിൽ (എണ്ണം_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              match_എണ്ണം--;
            }

          എങ്കിൽ (delimit_groups != DM_NONE)
            {
              എങ്കിൽ (!match)
                {
                  എങ്കിൽ (match_എണ്ണം) /* a previous match */
                    first_delimiter = false; /* Only used when DM_SEPARATE */
                }
              അല്ലെങ്കിൽ എങ്കിൽ (match_എണ്ണം == 1)
                {
                  എങ്കിൽ ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !first_delimiter))
                    putchar (delimiter);
                }
            }

          എങ്കിൽ (!match || output_later_repeated)
            {
              writeline (prevline, match, match_എണ്ണം);
              swap_വരികൾ (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              എങ്കിൽ (!match)
                match_എണ്ണം = 0;
            }
        }

      writeline (prevline, false, match_എണ്ണം);
    }

 closefiles:
  എങ്കിൽ (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  സ്വതന്ത്രം (lb1.buffer);
  സ്വതന്ത്രം (lb2.buffer);
}

എണ്ണൽ Skip_field_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ശൂന്യ);
  എണ്ണൽ Skip_field_option_type skip_field_option_type = SFO_NONE;
  പൂർണ്ണസംഖ്യ nfiles = 0;
  അക്ഷരം സ്ഥിരം *file[2];
  അക്ഷരം delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_used = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      എങ്കിൽ (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, ശൂന്യ))
              == -1))
        {
          എങ്കിൽ (argc <= optind)
            നിർത്തുക;
          എങ്കിൽ (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      അല്ലെങ്കിൽ തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 1:
          {
            intmax_t size;
            എങ്കിൽ (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, ശൂന്യ, 10, &size, "")
                    <= LONGINT_OVERFLOW))
              skip_chars = MIN (size, IDX_MAX);
            അല്ലെങ്കിൽ എങ്കിൽ (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            അല്ലെങ്കിൽ
              file[nfiles++] = optarg;
          }
          നിർത്തുക;

        അവസ്ഥ '0':
        അവസ്ഥ '1':
        അവസ്ഥ '2':
        അവസ്ഥ '3':
        അവസ്ഥ '4':
        അവസ്ഥ '5':
        അവസ്ഥ '6':
        അവസ്ഥ '7':
        അവസ്ഥ '8':
        അവസ്ഥ '9':
          {
            എങ്കിൽ (skip_field_option_type == SFO_NEW)
              skip_fields = 0;

            എങ്കിൽ (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_field_option_type = SFO_OBSOLETE;
          }
          നിർത്തുക;

        അവസ്ഥ 'c':
          എണ്ണം_occurrences = true;
          output_option_used = true;
          നിർത്തുക;

        അവസ്ഥ 'd':
          output_unique = false;
          output_option_used = true;
          നിർത്തുക;

        അവസ്ഥ 'D':
          output_unique = false;
          output_later_repeated = true;
          എങ്കിൽ (optarg == ശൂന്യ)
            delimit_groups = DM_NONE;
          അല്ലെങ്കിൽ
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_method_string,
                                        delimit_method_map);
          output_option_used = true;
          നിർത്തുക;

        അവസ്ഥ GROUP_OPTION:
          എങ്കിൽ (optarg == ശൂന്യ)
            grouping = GM_SEPARATE;
          അല്ലെങ്കിൽ
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_method_string,
                                  grouping_method_map);
          നിർത്തുക;

        അവസ്ഥ 'f':
          skip_field_option_type = SFO_NEW;
          skip_fields = size_opt (optarg,
                                  N_("invalid number of fields to skip"));
          നിർത്തുക;

        അവസ്ഥ 'i':
          ignore_case = true;
          നിർത്തുക;

        അവസ്ഥ 's':
          skip_chars = size_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          നിർത്തുക;

        അവസ്ഥ 'u':
          output_first_repeated = false;
          output_option_used = true;
          നിർത്തുക;

        അവസ്ഥ 'w':
          check_chars = size_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          നിർത്തുക;

        അവസ്ഥ 'z':
          delimiter = '\0';
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  എങ്കിൽ (grouping != GM_NONE && output_option_used)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (grouping != GM_NONE && എണ്ണം_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (എണ്ണം_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  തിരികെ EXIT_SUCCESS;
}
