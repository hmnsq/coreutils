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

#అన్తర్భావ <config.h>

#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "linebuffer.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "mcel.h"
#అన్తర్భావ "posixver.h"
#అన్తర్భావ "skipchars.h"
#అన్తర్భావ "stdio--.h"
#అన్తర్భావ "xstrtol.h"
#అన్తర్భావ "memcasecmp.h"
#అన్తర్భావ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "uniq"

#నిర్వచించు AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

స్థిర శూన్య
మార్పిడి_పంక్తులు (నిర్మాణం linebuffer **a, నిర్మాణం linebuffer **b)
{
  నిర్మాణం linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
స్థిర idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
స్థిర idx_t skip_chars = false;

/* Number of chars to compare. */
స్థిర idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
స్థిర bool లెక్క_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
స్థిర bool output_unique = true;
స్థిర bool output_మొదటి_repeated = true;
స్థిర bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
స్థిర bool ignore_case;

గణన delimit_పద్ధతి
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి delimit_పద్ధతి_string[] =
{
  "none", "prepend", "separate", ఖాళీ
};

స్థిర గణన delimit_పద్ధతి స్థిరరాశి delimit_పద్ధతి_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
స్థిర గణన delimit_పద్ధతి delimit_groups = DM_NONE;

గణన grouping_పద్ధతి
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

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి grouping_పద్ధతి_string[] =
{
  "prepend", "append", "separate", "both", ఖాళీ
};

స్థిర గణన grouping_పద్ధతి స్థిరరాశి grouping_పద్ధతి_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

స్థిర గణన grouping_పద్ధతి grouping = GM_NONE;

గణన
{
  సమూహం_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"count", no_వాదం, ఖాళీ, 'c'},
  {"repeated", no_వాదం, ఖాళీ, 'd'},
  {"all-repeated", optional_వాదం, ఖాళీ, 'D'},
  {"group", optional_వాదం, ఖాళీ, సమూహం_OPTION},
  {"ignore-case", no_వాదం, ఖాళీ, 'i'},
  {"unique", no_వాదం, ఖాళీ, 'u'},
  {"skip-fields", required_వాదం, ఖాళీ, 'f'},
  {"skip-chars", required_వాదం, ఖాళీ, 's'},
  {"check-chars", required_వాదం, ఖాళీ, 'w'},
  {"zero-terminated", no_వాదం, ఖాళీ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [ఆదానం_2 [OUTPUT]]\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Filter adjacent matching పంక్తులు from ఆదానం_2 (or standard ఆదానం),\n\
writing గమ్యం OUTPUT (or standard output).\n\
\n\
With no options, matching పంక్తులు are merged గమ్యం the మొదటి occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --లెక్క\n\
         ఉపసర్గ పంక్తులు by the సంఖ్య యొక్క occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate పంక్తులు, one కొరకు each సమూహం\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate పంక్తులు\n\
"));
     oputs (_("\
      --all-repeated[=పద్ధతి]\n\
         like -D, but allow separating groups with an ఖాళీలు పంక్తి;\n\
         పద్ధతి={none(అప్రమేయ),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the మొదటి N fields\n\
"));
     oputs (_("\
      --సమూహం[=పద్ధతి]\n\
         show all వస్తువులు, separating groups with an ఖాళీలు పంక్తి;\n\
         పద్ధతి={separate(అప్రమేయ),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-స్థితి\n\
         ignore differences లోపల స్థితి when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the మొదటి N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique పంక్తులు\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         పంక్తి delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no మరిన్ని than N characters లోపల పంక్తులు\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (సంస్కరణ_OPTION_DESCRIPTION);
     fputs (_("\
\n\
A క్షేత్రం is a నిరంతరం_2 యొక్క blanks (usually spaces and/or TABs), then non-blank\n\
characters.  Fields are skipped before chars.\n\
"), stdout);
     fputs (_("\
\n\
'uniq' does not detect repeated పంక్తులు unless they are adjacent.\n\
You may want గమ్యం క్రమీకరించు the ఆదానం మొదటి, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

స్థిర bool
strict_posix2 (శూన్య)
{
  పూర్ణాంకం posix_ver = posix2_సంస్కరణ ();
  తిరిగి 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

స్థిర idx_t
కొలత_opt (అక్షరం స్థిరరాశి *opt, అక్షరం స్థిరరాశి *msgid)
{
  intmax_t కొలత;
  ఒకవేళ (LONGINT_అధికప్రవాహం < xstrtoimax (opt, ఖాళీ, 10, &కొలత, "")
      || కొలత < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  తిరిగి MIN (కొలత, IDX_MAX);
}

స్థిర bool
newline_or_blank (mcel_t g)
{
  తిరిగి g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

స్థిర అక్షరం *
కనుగొను_క్షేత్రం (నిర్మాణం linebuffer స్థిరరాశి *పంక్తి, idx_t *plen)
{
  అక్షరం *lp = పంక్తి->బఫరు;
  అక్షరం స్థిరరాశి *lim = lp + పంక్తి->నిడివి - 1;

  కొరకు (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  కొరకు (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_పరిశీలన (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  ఒకవేళ (lim - lp <= check_chars)
    len = lim - lp;
  లేకపోతే ఒకవేళ (MB_CUR_MAX <= 1)
    len = check_chars;
  లేకపోతే
    {
      అక్షరం *ep = lp;
      కొరకు (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_పరిశీలన (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  తిరిగి lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

స్థిర bool
different (అక్షరం *old, అక్షరం *new, idx_t oldlen, idx_t newlen)
{
  ఒకవేళ (ignore_case)
    తిరిగి oldlen != newlen || memcasecmp (old, new, oldlen);
  లేకపోతే
    తిరిగి oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

స్థిర శూన్య
writeline (నిర్మాణం linebuffer స్థిరరాశి *పంక్తి,
           bool సరిపోలిక, intmax_t linecount)
{
  ఒకవేళ (! (linecount == 0 ? output_unique
         : !సరిపోలిక ? output_మొదటి_repeated
         : output_later_repeated))
    తిరిగి;

  ఒకవేళ (లెక్క_occurrences)
    {
      అక్షరం buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      అక్షరం *చివర = buf + పరిమాణం buf;
      అక్షరం *p = చివర;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      చేయి
        *--p = '0' + i % 10;
      అయితే ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      అయితే (చివర - p < 8)
        *--p = ' ';
      స్థిరరాశి idx_t nbytes = చివర - p;
      ఒకవేళ (fwrite (p, పరిమాణం (అక్షరం), nbytes, stdout) != nbytes)
        write_error ();
    }

  ఒకవేళ (fwrite (పంక్తి->బఫరు, పరిమాణం (అక్షరం), పంక్తి->నిడివి, stdout)
      != పంక్తి->నిడివి)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

స్థిర శూన్య
check_file (అక్షరం స్థిరరాశి *infile, అక్షరం స్థిరరాశి *outfile, అక్షరం delimiter)
{
  నిర్మాణం linebuffer lb1, lb2;
  నిర్మాణం linebuffer *thisline, *prevline;

  ఒకవేళ (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  ఒకవేళ (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
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
  ఒకవేళ (output_unique && output_మొదటి_repeated && !లెక్క_occurrences)
    {
      అక్షరం *prevfield = ఖాళీ;
      idx_t prevlen;
      bool మొదటి_సమూహం_printed = false;

      అయితే (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          అక్షరం *thisfield = కనుగొను_క్షేత్రం (thisline, &thislen);
          bool new_సమూహం = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          ఒకవేళ (new_సమూహం && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (మొదటి_సమూహం_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            అక్షరం_రాయి (delimiter);

          ఒకవేళ (new_సమూహం || grouping != GM_NONE)
            {
              ఒకవేళ (fwrite (thisline->బఫరు, పరిమాణం (అక్షరం), thisline->నిడివి,
                  stdout) != thisline->నిడివి)
                write_error ();

              మార్పిడి_పంక్తులు (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              మొదటి_సమూహం_printed = true;
            }
        }
      ఒకవేళ ((grouping == GM_BOTH || grouping == GM_APPEND) && మొదటి_సమూహం_printed)
        అక్షరం_రాయి (delimiter);
    }
  లేకపోతే
    {
      ఒకవేళ (!readlinebuffer_delim (prevline, stdin, delimiter))
        వెళ్ళు closefiles;

      idx_t prevlen;
      అక్షరం *prevfield = కనుగొను_క్షేత్రం (prevline, &prevlen);
      intmax_t సరిపోలిక_లెక్క = 0;
      bool మొదటి_delimiter = true;

      అయితే (!feof (stdin))
        {
          ఒకవేళ (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              ఒకవేళ (ferror (stdin))
                వెళ్ళు closefiles;
              ఆపు;
            }
          idx_t thislen;
          అక్షరం *thisfield = కనుగొను_క్షేత్రం (thisline, &thislen);
          bool సరిపోలిక = !different (thisfield, prevfield, thislen, prevlen);
          సరిపోలిక_లెక్క += సరిపోలిక;

          ఒకవేళ (సరిపోలిక_లెక్క == INTMAX_MAX)
            {
              ఒకవేళ (లెక్క_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              సరిపోలిక_లెక్క--;
            }

          ఒకవేళ (delimit_groups != DM_NONE)
            {
              ఒకవేళ (!సరిపోలిక)
                {
                  ఒకవేళ (సరిపోలిక_లెక్క) /* a previous match */
                    మొదటి_delimiter = false; /* Only used when DM_SEPARATE */
                }
              లేకపోతే ఒకవేళ (సరిపోలిక_లెక్క == 1)
                {
                  ఒకవేళ ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !మొదటి_delimiter))
                    అక్షరం_రాయి (delimiter);
                }
            }

          ఒకవేళ (!సరిపోలిక || output_later_repeated)
            {
              writeline (prevline, సరిపోలిక, సరిపోలిక_లెక్క);
              మార్పిడి_పంక్తులు (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              ఒకవేళ (!సరిపోలిక)
                సరిపోలిక_లెక్క = 0;
            }
        }

      writeline (prevline, false, సరిపోలిక_లెక్క);
    }

 closefiles:
  ఒకవేళ (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  విడుదల (lb1.బఫరు);
  విడుదల (lb2.బఫరు);
}

గణన Skip_క్షేత్రం_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ఖాళీ);
  గణన Skip_క్షేత్రం_option_type skip_క్షేత్రం_option_type = SFO_NONE;
  పూర్ణాంకం nfiles = 0;
  అక్షరం స్థిరరాశి *file[2];
  అక్షరం delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_వాడిన = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      ఒకవేళ (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, ఖాళీ))
              == -1))
        {
          ఒకవేళ (argc <= optind)
            ఆపు;
          ఒకవేళ (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      లేకపోతే ఎంపిక (optc)
        {
        స్థితి 1:
          {
            intmax_t కొలత;
            ఒకవేళ (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, ఖాళీ, 10, &కొలత, "")
                    <= LONGINT_అధికప్రవాహం))
              skip_chars = MIN (కొలత, IDX_MAX);
            లేకపోతే ఒకవేళ (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            లేకపోతే
              file[nfiles++] = optarg;
          }
          ఆపు;

        స్థితి '0':
        స్థితి '1':
        స్థితి '2':
        స్థితి '3':
        స్థితి '4':
        స్థితి '5':
        స్థితి '6':
        స్థితి '7':
        స్థితి '8':
        స్థితి '9':
          {
            ఒకవేళ (skip_క్షేత్రం_option_type == SFO_NEW)
              skip_fields = 0;

            ఒకవేళ (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_క్షేత్రం_option_type = SFO_OBSOLETE;
          }
          ఆపు;

        స్థితి 'c':
          లెక్క_occurrences = true;
          output_option_వాడిన = true;
          ఆపు;

        స్థితి 'd':
          output_unique = false;
          output_option_వాడిన = true;
          ఆపు;

        స్థితి 'D':
          output_unique = false;
          output_later_repeated = true;
          ఒకవేళ (optarg == ఖాళీ)
            delimit_groups = DM_NONE;
          లేకపోతే
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_పద్ధతి_string,
                                        delimit_పద్ధతి_map);
          output_option_వాడిన = true;
          ఆపు;

        స్థితి సమూహం_OPTION:
          ఒకవేళ (optarg == ఖాళీ)
            grouping = GM_SEPARATE;
          లేకపోతే
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_పద్ధతి_string,
                                  grouping_పద్ధతి_map);
          ఆపు;

        స్థితి 'f':
          skip_క్షేత్రం_option_type = SFO_NEW;
          skip_fields = కొలత_opt (optarg,
                                  N_("invalid number of fields to skip"));
          ఆపు;

        స్థితి 'i':
          ignore_case = true;
          ఆపు;

        స్థితి 's':
          skip_chars = కొలత_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          ఆపు;

        స్థితి 'u':
          output_మొదటి_repeated = false;
          output_option_వాడిన = true;
          ఆపు;

        స్థితి 'w':
          check_chars = కొలత_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          ఆపు;

        స్థితి 'z':
          delimiter = '\0';
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  ఒకవేళ (grouping != GM_NONE && output_option_వాడిన)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (grouping != GM_NONE && లెక్క_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (లెక్క_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  తిరిగి EXIT_SUCCESS;
}
