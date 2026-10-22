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
#define PROGRAM_NAME "uniq"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

स्थिर शून्य
swap_पङ्क्तिहरू (संरचना linebuffer **a, संरचना linebuffer **b)
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
स्थिर bool गन्ती_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
स्थिर bool output_unique = true;
स्थिर bool output_first_repeated = true;
स्थिर bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
स्थिर bool ignore_case;

गणना delimit_method
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

स्थिर वर्ण अचर *अचर delimit_method_string[] =
{
  "none", "prepend", "separate", रिक्त
};

स्थिर गणना delimit_method अचर delimit_method_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
स्थिर गणना delimit_method delimit_groups = DM_NONE;

गणना grouping_method
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

स्थिर वर्ण अचर *अचर grouping_method_string[] =
{
  "prepend", "append", "separate", "both", रिक्त
};

स्थिर गणना grouping_method अचर grouping_method_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

स्थिर गणना grouping_method grouping = GM_NONE;

गणना
{
  GROUP_OPTION = अक्षर_उच्च + 1
};

स्थिर संरचना option अचर longopts[] =
{
  {"count", no_argument, रिक्त, 'c'},
  {"repeated", no_argument, रिक्त, 'd'},
  {"all-repeated", optional_argument, रिक्त, 'D'},
  {"group", optional_argument, रिक्त, GROUP_OPTION},
  {"ignore-case", no_argument, रिक्त, 'i'},
  {"unique", no_argument, रिक्त, 'u'},
  {"skip-fields", required_argument, रिक्त, 'f'},
  {"skip-chars", required_argument, रिक्त, 's'},
  {"check-chars", required_argument, रिक्त, 'w'},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [आगत_2 [OUTPUT]]\n\
"),
              program_name);
      ख_माला_दो (_("\
Filter adjacent matching पङ्क्तिहरू from आगत_2 (or standard आगत),\n\
writing to OUTPUT (or standard output).\n\
\n\
With no options, matching पङ्क्तिहरू are merged to the first occurrence.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --गन्ती\n\
         prefix पङ्क्तिहरू by the सङ्ख्या of occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate पङ्क्तिहरू, one लागि each group\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate पङ्क्तिहरू\n\
"));
     oputs (_("\
      --all-repeated[=METHOD]\n\
         like -D, but allow separating groups with an empty पङ्क्ति;\n\
         METHOD={none(स्वभाव),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the first N fields\n\
"));
     oputs (_("\
      --group[=METHOD]\n\
         show all वस्तुहरू, separating groups with an empty पङ्क्ति;\n\
         METHOD={separate(स्वभाव),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-पक्ष\n\
         ignore differences in पक्ष when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the first N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique पङ्क्तिहरू\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्ति delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no more than N characters in पङ्क्तिहरू\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (VERSION_OPTION_DESCRIPTION);
     ख_माला_दो (_("\
\n\
A field is a run of blanks (usually spaces and/or TABs), then non-blank\n\
characters.  Fields are skipped before chars.\n\
"), मानक_निकास);
     ख_माला_दो (_("\
\n\
'uniq' does not detect repeated पङ्क्तिहरू unless they are adjacent.\n\
You may want to sort the आगत first, or use 'sort -u' without 'uniq'.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

स्थिर bool
strict_posix2 (शून्य)
{
  पूर्णाङ्क posix_ver = posix2_version ();
  फर्काऊ 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

स्थिर idx_t
size_opt (वर्ण अचर *opt, वर्ण अचर *msgid)
{
  intmax_t size;
  यदि (LONGINT_OVERFLOW < xstrtoimax (opt, रिक्त, 10, &size, "")
      || size < 0)
    error (निकास_त्रुटि, 0, "%s: %s", opt, _(msgid));
  फर्काऊ MIN (size, IDX_MAX);
}

स्थिर bool
newline_or_blank (mcel_t g)
{
  फर्काऊ g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

स्थिर वर्ण *
find_field (संरचना linebuffer अचर *पङ्क्ति, idx_t *plen)
{
  वर्ण *lp = पङ्क्ति->buffer;
  वर्ण अचर *lim = lp + पङ्क्ति->length - 1;

  लागि (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  लागि (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_scan (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  यदि (lim - lp <= check_chars)
    len = lim - lp;
  अन्यथा यदि (MB_CUR_MAX <= 1)
    len = check_chars;
  अन्यथा
    {
      वर्ण *ep = lp;
      लागि (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_scan (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  फर्काऊ lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

स्थिर bool
different (वर्ण *old, वर्ण *new, idx_t oldlen, idx_t newlen)
{
  यदि (ignore_case)
    फर्काऊ oldlen != newlen || memcasecmp (old, new, oldlen);
  अन्यथा
    फर्काऊ oldlen != newlen || स_भेद (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

स्थिर शून्य
writeline (संरचना linebuffer अचर *पङ्क्ति,
           bool match, intmax_t linecount)
{
  यदि (! (linecount == 0 ? output_unique
         : !match ? output_first_repeated
         : output_later_repeated))
    फर्काऊ;

  यदि (गन्ती_occurrences)
    {
      वर्ण buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      वर्ण *end = buf + परिमाण buf;
      वर्ण *p = end;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      गर
        *--p = '0' + i % 10;
      जबसम्म ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      जबसम्म (end - p < 8)
        *--p = ' ';
      अचर idx_t nbytes = end - p;
      यदि (ख_डालो (p, परिमाण (वर्ण), nbytes, मानक_निकास) != nbytes)
        write_error ();
    }

  यदि (ख_डालो (पङ्क्ति->buffer, परिमाण (वर्ण), पङ्क्ति->length, मानक_निकास)
      != पङ्क्ति->length)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

स्थिर शून्य
check_file (वर्ण अचर *infile, वर्ण अचर *outfile, वर्ण delimiter)
{
  संरचना linebuffer lb1, lb2;
  संरचना linebuffer *thisline, *prevline;

  यदि (! (streq (infile, "-") || ख_व_खोलो (infile, "r", मानक_निवेश)))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
  यदि (! (streq (outfile, "-") || ख_व_खोलो (outfile, "w", मानक_निकास)))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (outfile));

  fadvise (मानक_निवेश, FADVISE_SEQUENTIAL);

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
  यदि (output_unique && output_first_repeated && !गन्ती_occurrences)
    {
      वर्ण *prevfield = रिक्त;
      idx_t prevlen;
      bool first_group_printed = false;

      जबसम्म (!ख_पूर्ण (मानक_निवेश)
             && readlinebuffer_delim (thisline, मानक_निवेश, delimiter))
        {
          idx_t thislen;
          वर्ण *thisfield = find_field (thisline, &thislen);
          bool new_group = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          यदि (new_group && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (first_group_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            अक्षर_दो (delimiter);

          यदि (new_group || grouping != GM_NONE)
            {
              यदि (ख_डालो (thisline->buffer, परिमाण (वर्ण), thisline->length,
                  मानक_निकास) != thisline->length)
                write_error ();

              swap_पङ्क्तिहरू (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              first_group_printed = true;
            }
        }
      यदि ((grouping == GM_BOTH || grouping == GM_APPEND) && first_group_printed)
        अक्षर_दो (delimiter);
    }
  अन्यथा
    {
      यदि (!readlinebuffer_delim (prevline, मानक_निवेश, delimiter))
        गच्छ closefiles;

      idx_t prevlen;
      वर्ण *prevfield = find_field (prevline, &prevlen);
      intmax_t match_गन्ती = 0;
      bool first_delimiter = true;

      जबसम्म (!ख_पूर्ण (मानक_निवेश))
        {
          यदि (!readlinebuffer_delim (thisline, मानक_निवेश, delimiter))
            {
              यदि (ख_त्रुटि (मानक_निवेश))
                गच्छ closefiles;
              रोक;
            }
          idx_t thislen;
          वर्ण *thisfield = find_field (thisline, &thislen);
          bool match = !different (thisfield, prevfield, thislen, prevlen);
          match_गन्ती += match;

          यदि (match_गन्ती == INTMAX_MAX)
            {
              यदि (गन्ती_occurrences)
                error (निकास_त्रुटि, 0, _("too many repeated lines"));
              match_गन्ती--;
            }

          यदि (delimit_groups != DM_NONE)
            {
              यदि (!match)
                {
                  यदि (match_गन्ती) /* a previous match */
                    first_delimiter = false; /* Only used when DM_SEPARATE */
                }
              अन्यथा यदि (match_गन्ती == 1)
                {
                  यदि ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !first_delimiter))
                    अक्षर_दो (delimiter);
                }
            }

          यदि (!match || output_later_repeated)
            {
              writeline (prevline, match, match_गन्ती);
              swap_पङ्क्तिहरू (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              यदि (!match)
                match_गन्ती = 0;
            }
        }

      writeline (prevline, false, match_गन्ती);
    }

 closefiles:
  यदि (ख_त्रुटि (मानक_निवेश) || ख_बंद (मानक_निवेश) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  मुक्त (lb1.buffer);
  मुक्त (lb2.buffer);
}

गणना Skip_field_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc = 0;
  bool posixly_correct = (दो_पर्या ("POSIXLY_CORRECT") != रिक्त);
  गणना Skip_field_option_type skip_field_option_type = SFO_NONE;
  पूर्णाङ्क nfiles = 0;
  वर्ण अचर *file[2];
  वर्ण delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_used = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबसम्म (true)
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
            रोक;
          यदि (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (निकास_त्रुटि);
            }
          file[nfiles++] = argv[optind++];
        }
      अन्यथा विकल्प (optc)
        {
        पक्ष 1:
          {
            intmax_t size;
            यदि (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, रिक्त, 10, &size, "")
                    <= LONGINT_OVERFLOW))
              skip_chars = MIN (size, IDX_MAX);
            अन्यथा यदि (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (निकास_त्रुटि);
              }
            अन्यथा
              file[nfiles++] = optarg;
          }
          रोक;

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
            यदि (skip_field_option_type == SFO_NEW)
              skip_fields = 0;

            यदि (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_field_option_type = SFO_OBSOLETE;
          }
          रोक;

        पक्ष 'c':
          गन्ती_occurrences = true;
          output_option_used = true;
          रोक;

        पक्ष 'd':
          output_unique = false;
          output_option_used = true;
          रोक;

        पक्ष 'D':
          output_unique = false;
          output_later_repeated = true;
          यदि (optarg == रिक्त)
            delimit_groups = DM_NONE;
          अन्यथा
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_method_string,
                                        delimit_method_map);
          output_option_used = true;
          रोक;

        पक्ष GROUP_OPTION:
          यदि (optarg == रिक्त)
            grouping = GM_SEPARATE;
          अन्यथा
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_method_string,
                                  grouping_method_map);
          रोक;

        पक्ष 'f':
          skip_field_option_type = SFO_NEW;
          skip_fields = size_opt (optarg,
                                  N_("invalid number of fields to skip"));
          रोक;

        पक्ष 'i':
          ignore_case = true;
          रोक;

        पक्ष 's':
          skip_chars = size_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          रोक;

        पक्ष 'u':
          output_first_repeated = false;
          output_option_used = true;
          रोक;

        पक्ष 'w':
          check_chars = size_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          रोक;

        पक्ष 'z':
          delimiter = '\0';
          रोक;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (निकास_त्रुटि);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  यदि (grouping != GM_NONE && output_option_used)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (निकास_त्रुटि);
    }

  यदि (grouping != GM_NONE && गन्ती_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (निकास_त्रुटि);
    }

  यदि (गन्ती_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (निकास_त्रुटि);
    }

  check_file (file[0], file[1], delimiter);

  फर्काऊ निकास_सफल;
}
