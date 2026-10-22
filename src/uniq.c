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

#अन्तर्भाव <config.h>

#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "linebuffer.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "mcel.h"
#अन्तर्भाव "posixver.h"
#अन्तर्भाव "skipchars.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "xstrtol.h"
#अन्तर्भाव "memcasecmp.h"
#अन्तर्भाव "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "uniq"

#परिभाषय AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

स्थिर शून्य
swap_पङ्क्तयः (संरचना linebuffer **a, संरचना linebuffer **b)
{
  संरचना linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
स्थिर idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
स्थिर idx_t skip_chars = false;

/* Number of chars to compare. */
स्थिर idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
स्थिर bool गणनम्_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
स्थिर bool output_unique = true;
स्थिर bool output_first_repeated = true;
स्थिर bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
स्थिर bool ignore_case;

गणना delimit_विधानम्
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

स्थिर वर्ण ध्रुव *ध्रुव delimit_विधानम्_string[] =
{
  "none", "prepend", "separate", रिक्त
};

स्थिर गणना delimit_विधानम् ध्रुव delimit_विधानम्_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
स्थिर गणना delimit_विधानम् delimit_groups = DM_NONE;

गणना grouping_विधानम्
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

स्थिर वर्ण ध्रुव *ध्रुव grouping_विधानम्_string[] =
{
  "prepend", "append", "separate", "both", रिक्त
};

स्थिर गणना grouping_विधानम् ध्रुव grouping_विधानम्_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

स्थिर गणना grouping_विधानम् grouping = GM_NONE;

गणना
{
  गणः_OPTION = CHAR_MAX + 1
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"count", no_तर्कः, रिक्त, 'c'},
  {"repeated", no_तर्कः, रिक्त, 'd'},
  {"all-repeated", optional_तर्कः, रिक्त, 'D'},
  {"group", optional_तर्कः, रिक्त, गणः_OPTION},
  {"ignore-case", no_तर्कः, रिक्त, 'i'},
  {"unique", no_तर्कः, रिक्त, 'u'},
  {"skip-fields", required_तर्कः, रिक्त, 'f'},
  {"skip-chars", required_तर्कः, रिक्त, 's'},
  {"check-chars", required_तर्कः, रिक्त, 'w'},
  {"zero-terminated", no_तर्कः, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [निवेशः_2 [OUTPUT]]\n\
"),
              क्रमादेशः_name);
      fputs (_("\
Filter adjacent matching पङ्क्तयः from निवेशः_2 (or standard निवेशः),\n\
writing गन्तव्यम् OUTPUT (or standard output).\n\
\n\
With no options, matching पङ्क्तयः are merged गन्तव्यम् the first occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --गणनम्\n\
         उपसर्गः पङ्क्तयः by the सङ्ख्या सम्बन्धि occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate पङ्क्तयः, one प्रति each गणः\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate पङ्क्तयः\n\
"));
     oputs (_("\
      --all-repeated[=विधानम्]\n\
         like -D, but allow separating groups with an रिक्तानि पङ्क्तिः;\n\
         विधानम्={none(स्वभाव),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the first N fields\n\
"));
     oputs (_("\
      --गणः[=विधानम्]\n\
         show all वस्तूनि, separating groups with an रिक्तानि पङ्क्तिः;\n\
         विधानम्={separate(स्वभाव),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-पक्ष\n\
         ignore differences अन्तर् पक्ष when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the first N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique पङ्क्तयः\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्तिः delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no अधिकम् than N characters अन्तर् पङ्क्तयः\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (संस्करणम्_OPTION_DESCRIPTION);
     fputs (_("\
\n\
A क्षेत्रम् is a सततम्_2 सम्बन्धि blanks (usually spaces and/or TABs), then non-blank\n\
characters.  Fields are skipped before chars.\n\
"), stdout);
     fputs (_("\
\n\
'uniq' does not detect repeated पङ्क्तयः unless they are adjacent.\n\
You may want गन्तव्यम् क्रमय the निवेशः first, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

स्थिर bool
strict_posix2 (शून्य)
{
  पूर्णाङ्क posix_ver = posix2_संस्करणम् ();
  प्रत्यागम 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

स्थिर idx_t
आकार_opt (वर्ण ध्रुव *opt, वर्ण ध्रुव *msgid)
{
  intmax_t आकार;
  यदि (LONGINT_अतिप्रवाहः < xstrtoimax (opt, रिक्त, 10, &आकार, "")
      || आकार < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  प्रत्यागम MIN (आकार, IDX_MAX);
}

स्थिर bool
newline_or_blank (mcel_t g)
{
  प्रत्यागम g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

स्थिर वर्ण *
find_क्षेत्रम् (संरचना linebuffer ध्रुव *पङ्क्तिः, idx_t *plen)
{
  वर्ण *lp = पङ्क्तिः->बफर;
  वर्ण ध्रुव *lim = lp + पङ्क्तिः->दीर्घता - 1;

  प्रति (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  प्रति (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_अवलोकनम् (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  यदि (lim - lp <= check_chars)
    len = lim - lp;
  अन्यथा यदि (MB_CUR_MAX <= 1)
    len = check_chars;
  अन्यथा
    {
      वर्ण *ep = lp;
      प्रति (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_अवलोकनम् (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  प्रत्यागम lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

स्थिर bool
different (वर्ण *old, वर्ण *new, idx_t oldlen, idx_t newlen)
{
  यदि (ignore_case)
    प्रत्यागम oldlen != newlen || memcasecmp (old, new, oldlen);
  अन्यथा
    प्रत्यागम oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

स्थिर शून्य
writeline (संरचना linebuffer ध्रुव *पङ्क्तिः,
           bool मेलनम्, intmax_t linecount)
{
  यदि (! (linecount == 0 ? output_unique
         : !मेलनम् ? output_first_repeated
         : output_later_repeated))
    प्रत्यागम;

  यदि (गणनम्_occurrences)
    {
      वर्ण buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      वर्ण *अन्तः = buf + परिमाण buf;
      वर्ण *p = अन्तः;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      कुरु
        *--p = '0' + i % 10;
      यावत् ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      यावत् (अन्तः - p < 8)
        *--p = ' ';
      ध्रुव idx_t nbytes = अन्तः - p;
      यदि (fwrite (p, परिमाण (वर्ण), nbytes, stdout) != nbytes)
        write_error ();
    }

  यदि (fwrite (पङ्क्तिः->बफर, परिमाण (वर्ण), पङ्क्तिः->दीर्घता, stdout)
      != पङ्क्तिः->दीर्घता)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

स्थिर शून्य
check_file (वर्ण ध्रुव *infile, वर्ण ध्रुव *outfile, वर्ण delimiter)
{
  संरचना linebuffer lb1, lb2;
  संरचना linebuffer *thisline, *prevline;

  यदि (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  यदि (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
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
  यदि (output_unique && output_first_repeated && !गणनम्_occurrences)
    {
      वर्ण *prevfield = रिक्त;
      idx_t prevlen;
      bool first_गणः_printed = false;

      यावत् (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          वर्ण *thisfield = find_क्षेत्रम् (thisline, &thislen);
          bool new_गणः = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          यदि (new_गणः && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (first_गणः_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            अक्षरं_लिख (delimiter);

          यदि (new_गणः || grouping != GM_NONE)
            {
              यदि (fwrite (thisline->बफर, परिमाण (वर्ण), thisline->दीर्घता,
                  stdout) != thisline->दीर्घता)
                write_error ();

              swap_पङ्क्तयः (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              first_गणः_printed = true;
            }
        }
      यदि ((grouping == GM_BOTH || grouping == GM_APPEND) && first_गणः_printed)
        अक्षरं_लिख (delimiter);
    }
  अन्यथा
    {
      यदि (!readlinebuffer_delim (prevline, stdin, delimiter))
        गच्छ closefiles;

      idx_t prevlen;
      वर्ण *prevfield = find_क्षेत्रम् (prevline, &prevlen);
      intmax_t मेलनम्_गणनम् = 0;
      bool first_delimiter = true;

      यावत् (!feof (stdin))
        {
          यदि (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              यदि (ferror (stdin))
                गच्छ closefiles;
              भङ्ग;
            }
          idx_t thislen;
          वर्ण *thisfield = find_क्षेत्रम् (thisline, &thislen);
          bool मेलनम् = !different (thisfield, prevfield, thislen, prevlen);
          मेलनम्_गणनम् += मेलनम्;

          यदि (मेलनम्_गणनम् == INTMAX_MAX)
            {
              यदि (गणनम्_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              मेलनम्_गणनम्--;
            }

          यदि (delimit_groups != DM_NONE)
            {
              यदि (!मेलनम्)
                {
                  यदि (मेलनम्_गणनम्) /* a previous match */
                    first_delimiter = false; /* Only used when DM_SEPARATE */
                }
              अन्यथा यदि (मेलनम्_गणनम् == 1)
                {
                  यदि ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !first_delimiter))
                    अक्षरं_लिख (delimiter);
                }
            }

          यदि (!मेलनम् || output_later_repeated)
            {
              writeline (prevline, मेलनम्, मेलनम्_गणनम्);
              swap_पङ्क्तयः (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              यदि (!मेलनम्)
                मेलनम्_गणनम् = 0;
            }
        }

      writeline (prevline, false, मेलनम्_गणनम्);
    }

 closefiles:
  यदि (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  मुक्त (lb1.बफर);
  मुक्त (lb2.बफर);
}

गणना Skip_क्षेत्रम्_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != रिक्त);
  गणना Skip_क्षेत्रम्_option_type skip_क्षेत्रम्_option_type = SFO_NONE;
  पूर्णाङ्क nfiles = 0;
  वर्ण ध्रुव *file[2];
  वर्ण delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_प्रयुक्तम् = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      यदि (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, रिक्त))
              == -1))
        {
          यदि (argc <= optind)
            भङ्ग;
          यदि (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      अन्यथा विकल्प (optc)
        {
        पक्ष 1:
          {
            intmax_t आकार;
            यदि (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, रिक्त, 10, &आकार, "")
                    <= LONGINT_अतिप्रवाहः))
              skip_chars = MIN (आकार, IDX_MAX);
            अन्यथा यदि (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            अन्यथा
              file[nfiles++] = optarg;
          }
          भङ्ग;

        पक्ष '0':
        पक्ष '1':
        पक्ष '2':
        पक्ष '3':
        पक्ष '4':
        पक्ष '5':
        पक्ष '6':
        पक्ष '7':
        पक्ष '8':
        पक्ष '9':
          {
            यदि (skip_क्षेत्रम्_option_type == SFO_NEW)
              skip_fields = 0;

            यदि (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_क्षेत्रम्_option_type = SFO_OBSOLETE;
          }
          भङ्ग;

        पक्ष 'c':
          गणनम्_occurrences = true;
          output_option_प्रयुक्तम् = true;
          भङ्ग;

        पक्ष 'd':
          output_unique = false;
          output_option_प्रयुक्तम् = true;
          भङ्ग;

        पक्ष 'D':
          output_unique = false;
          output_later_repeated = true;
          यदि (optarg == रिक्त)
            delimit_groups = DM_NONE;
          अन्यथा
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_विधानम्_string,
                                        delimit_विधानम्_map);
          output_option_प्रयुक्तम् = true;
          भङ्ग;

        पक्ष गणः_OPTION:
          यदि (optarg == रिक्त)
            grouping = GM_SEPARATE;
          अन्यथा
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_विधानम्_string,
                                  grouping_विधानम्_map);
          भङ्ग;

        पक्ष 'f':
          skip_क्षेत्रम्_option_type = SFO_NEW;
          skip_fields = आकार_opt (optarg,
                                  N_("invalid number of fields to skip"));
          भङ्ग;

        पक्ष 'i':
          ignore_case = true;
          भङ्ग;

        पक्ष 's':
          skip_chars = आकार_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          भङ्ग;

        पक्ष 'u':
          output_first_repeated = false;
          output_option_प्रयुक्तम् = true;
          भङ्ग;

        पक्ष 'w':
          check_chars = आकार_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          भङ्ग;

        पक्ष 'z':
          delimiter = '\0';
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  यदि (grouping != GM_NONE && output_option_प्रयुक्तम्)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  यदि (grouping != GM_NONE && गणनम्_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  यदि (गणनम्_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  प्रत्यागम EXIT_SUCCESS;
}
