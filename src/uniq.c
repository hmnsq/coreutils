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

#समावेश <config.h>

#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "argmatch.h"
#समावेश "assure.h"
#समावेश "linebuffer.h"
#समावेश "fadvise.h"
#समावेश "mcel.h"
#समावेश "posixver.h"
#समावेश "skipchars.h"
#समावेश "stdio--.h"
#समावेश "xstrtol.h"
#समावेश "memcasecmp.h"
#समावेश "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "uniq"

#घोषणा AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

अटल व्योम
swap_पंक्तियाँ (काष्ठा linebuffer **a, काष्ठा linebuffer **b)
{
  काष्ठा linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
अटल idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
अटल idx_t skip_chars = false;

/* Number of chars to compare. */
अटल idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
अटल bool गिनती_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
अटल bool output_unique = true;
अटल bool output_first_repeated = true;
अटल bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
अटल bool ignore_case;

क्रमागत delimit_method
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

अटल अक्षर स्थिर *स्थिर delimit_method_string[] =
{
  "none", "prepend", "separate", शून्य
};

अटल क्रमागत delimit_method स्थिर delimit_method_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
अटल क्रमागत delimit_method delimit_groups = DM_NONE;

क्रमागत grouping_method
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

अटल अक्षर स्थिर *स्थिर grouping_method_string[] =
{
  "prepend", "append", "separate", "both", शून्य
};

अटल क्रमागत grouping_method स्थिर grouping_method_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

अटल क्रमागत grouping_method grouping = GM_NONE;

क्रमागत
{
  GROUP_OPTION = अक्षर_उच्च + 1
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"count", no_argument, शून्य, 'c'},
  {"repeated", no_argument, शून्य, 'd'},
  {"all-repeated", optional_argument, शून्य, 'D'},
  {"group", optional_argument, शून्य, GROUP_OPTION},
  {"ignore-case", no_argument, शून्य, 'i'},
  {"unique", no_argument, शून्य, 'u'},
  {"skip-fields", required_argument, शून्य, 'f'},
  {"skip-chars", required_argument, शून्य, 's'},
  {"check-chars", required_argument, शून्य, 'w'},
  {"zero-terminated", no_argument, शून्य, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [निवेश_2 [OUTPUT]]\n\
"),
              program_name);
      ख_माला_दो (_("\
Filter adjacent matching पंक्तियाँ from निवेश_2 (or standard निवेश),\n\
writing to OUTPUT (or standard output).\n\
\n\
With no options, matching पंक्तियाँ are merged to the first occurrence.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --गिनती\n\
         prefix पंक्तियाँ by the संख्या of occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate पंक्तियाँ, one क्रम each group\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate पंक्तियाँ\n\
"));
     oputs (_("\
      --all-repeated[=METHOD]\n\
         like -D, but allow separating groups with an empty पंक्ति;\n\
         METHOD={none(शेष),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the first N fields\n\
"));
     oputs (_("\
      --group[=METHOD]\n\
         show all वस्तुएँ, separating groups with an empty पंक्ति;\n\
         METHOD={separate(शेष),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-हाल\n\
         ignore differences in हाल when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the first N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique पंक्तियाँ\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         पंक्ति delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no more than N characters in पंक्तियाँ\n\
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
'uniq' does not detect repeated पंक्तियाँ unless they are adjacent.\n\
You may want to sort the निवेश first, or use 'sort -u' without 'uniq'.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

अटल bool
strict_posix2 (व्योम)
{
  पूर्णांक posix_ver = posix2_version ();
  वापस 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

अटल idx_t
आकार_opt (अक्षर स्थिर *opt, अक्षर स्थिर *msgid)
{
  intmax_t आकार;
  अगर (LONGINT_OVERFLOW < xstrtoimax (opt, शून्य, 10, &आकार, "")
      || आकार < 0)
    error (निकास_त्रुटि, 0, "%s: %s", opt, _(msgid));
  वापस MIN (आकार, IDX_MAX);
}

अटल bool
newline_or_blank (mcel_t g)
{
  वापस g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

अटल अक्षर *
find_field (काष्ठा linebuffer स्थिर *पंक्ति, idx_t *plen)
{
  अक्षर *lp = पंक्ति->बफर;
  अक्षर स्थिर *lim = lp + पंक्ति->लंबाई - 1;

  क्रम (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  क्रम (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_scan (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  अगर (lim - lp <= check_chars)
    len = lim - lp;
  अन्यथा अगर (MB_CUR_MAX <= 1)
    len = check_chars;
  अन्यथा
    {
      अक्षर *ep = lp;
      क्रम (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_scan (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  वापस lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

अटल bool
different (अक्षर *old, अक्षर *new, idx_t oldlen, idx_t newlen)
{
  अगर (ignore_case)
    वापस oldlen != newlen || memcasecmp (old, new, oldlen);
  अन्यथा
    वापस oldlen != newlen || स_भेद (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

अटल व्योम
writeline (काष्ठा linebuffer स्थिर *पंक्ति,
           bool match, intmax_t linecount)
{
  अगर (! (linecount == 0 ? output_unique
         : !match ? output_first_repeated
         : output_later_repeated))
    वापस;

  अगर (गिनती_occurrences)
    {
      अक्षर buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      अक्षर *end = buf + माप buf;
      अक्षर *p = end;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      करो
        *--p = '0' + i % 10;
      जबतक ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      जबतक (end - p < 8)
        *--p = ' ';
      स्थिर idx_t nbytes = end - p;
      अगर (ख_डालो (p, माप (अक्षर), nbytes, मानक_निकास) != nbytes)
        write_error ();
    }

  अगर (ख_डालो (पंक्ति->बफर, माप (अक्षर), पंक्ति->लंबाई, मानक_निकास)
      != पंक्ति->लंबाई)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

अटल व्योम
check_file (अक्षर स्थिर *infile, अक्षर स्थिर *outfile, अक्षर delimiter)
{
  काष्ठा linebuffer lb1, lb2;
  काष्ठा linebuffer *thisline, *prevline;

  अगर (! (streq (infile, "-") || ख_व_खोलो (infile, "r", मानक_निवेश)))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
  अगर (! (streq (outfile, "-") || ख_व_खोलो (outfile, "w", मानक_निकास)))
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
  अगर (output_unique && output_first_repeated && !गिनती_occurrences)
    {
      अक्षर *prevfield = शून्य;
      idx_t prevlen;
      bool first_group_printed = false;

      जबतक (!ख_पूर्ण (मानक_निवेश)
             && readlinebuffer_delim (thisline, मानक_निवेश, delimiter))
        {
          idx_t thislen;
          अक्षर *thisfield = find_field (thisline, &thislen);
          bool new_group = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          अगर (new_group && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (first_group_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            अक्षर_दो (delimiter);

          अगर (new_group || grouping != GM_NONE)
            {
              अगर (ख_डालो (thisline->बफर, माप (अक्षर), thisline->लंबाई,
                  मानक_निकास) != thisline->लंबाई)
                write_error ();

              swap_पंक्तियाँ (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              first_group_printed = true;
            }
        }
      अगर ((grouping == GM_BOTH || grouping == GM_APPEND) && first_group_printed)
        अक्षर_दो (delimiter);
    }
  अन्यथा
    {
      अगर (!readlinebuffer_delim (prevline, मानक_निवेश, delimiter))
        जाओ closefiles;

      idx_t prevlen;
      अक्षर *prevfield = find_field (prevline, &prevlen);
      intmax_t match_गिनती = 0;
      bool first_delimiter = true;

      जबतक (!ख_पूर्ण (मानक_निवेश))
        {
          अगर (!readlinebuffer_delim (thisline, मानक_निवेश, delimiter))
            {
              अगर (ख_त्रुटि (मानक_निवेश))
                जाओ closefiles;
              अवरोध;
            }
          idx_t thislen;
          अक्षर *thisfield = find_field (thisline, &thislen);
          bool match = !different (thisfield, prevfield, thislen, prevlen);
          match_गिनती += match;

          अगर (match_गिनती == INTMAX_MAX)
            {
              अगर (गिनती_occurrences)
                error (निकास_त्रुटि, 0, _("too many repeated lines"));
              match_गिनती--;
            }

          अगर (delimit_groups != DM_NONE)
            {
              अगर (!match)
                {
                  अगर (match_गिनती) /* a previous match */
                    first_delimiter = false; /* Only used when DM_SEPARATE */
                }
              अन्यथा अगर (match_गिनती == 1)
                {
                  अगर ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !first_delimiter))
                    अक्षर_दो (delimiter);
                }
            }

          अगर (!match || output_later_repeated)
            {
              writeline (prevline, match, match_गिनती);
              swap_पंक्तियाँ (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              अगर (!match)
                match_गिनती = 0;
            }
        }

      writeline (prevline, false, match_गिनती);
    }

 closefiles:
  अगर (ख_त्रुटि (मानक_निवेश) || ख_बंद (मानक_निवेश) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  मुक्त (lb1.बफर);
  मुक्त (lb2.बफर);
}

क्रमागत Skip_field_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc = 0;
  bool posixly_correct = (दो_पर्या ("POSIXLY_CORRECT") != शून्य);
  क्रमागत Skip_field_option_type skip_field_option_type = SFO_NONE;
  पूर्णांक nfiles = 0;
  अक्षर स्थिर *file[2];
  अक्षर delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_used = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      अगर (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, शून्य))
              == -1))
        {
          अगर (argc <= optind)
            अवरोध;
          अगर (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (निकास_त्रुटि);
            }
          file[nfiles++] = argv[optind++];
        }
      अन्यथा चयन (optc)
        {
        हाल 1:
          {
            intmax_t आकार;
            अगर (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, शून्य, 10, &आकार, "")
                    <= LONGINT_OVERFLOW))
              skip_chars = MIN (आकार, IDX_MAX);
            अन्यथा अगर (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (निकास_त्रुटि);
              }
            अन्यथा
              file[nfiles++] = optarg;
          }
          अवरोध;

        हाल '0':
        हाल '1':
        हाल '2':
        हाल '3':
        हाल '4':
        हाल '5':
        हाल '6':
        हाल '7':
        हाल '8':
        हाल '9':
          {
            अगर (skip_field_option_type == SFO_NEW)
              skip_fields = 0;

            अगर (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_field_option_type = SFO_OBSOLETE;
          }
          अवरोध;

        हाल 'c':
          गिनती_occurrences = true;
          output_option_used = true;
          अवरोध;

        हाल 'd':
          output_unique = false;
          output_option_used = true;
          अवरोध;

        हाल 'D':
          output_unique = false;
          output_later_repeated = true;
          अगर (optarg == शून्य)
            delimit_groups = DM_NONE;
          अन्यथा
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_method_string,
                                        delimit_method_map);
          output_option_used = true;
          अवरोध;

        हाल GROUP_OPTION:
          अगर (optarg == शून्य)
            grouping = GM_SEPARATE;
          अन्यथा
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_method_string,
                                  grouping_method_map);
          अवरोध;

        हाल 'f':
          skip_field_option_type = SFO_NEW;
          skip_fields = आकार_opt (optarg,
                                  N_("invalid number of fields to skip"));
          अवरोध;

        हाल 'i':
          ignore_case = true;
          अवरोध;

        हाल 's':
          skip_chars = आकार_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          अवरोध;

        हाल 'u':
          output_first_repeated = false;
          output_option_used = true;
          अवरोध;

        हाल 'w':
          check_chars = आकार_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          अवरोध;

        हाल 'z':
          delimiter = '\0';
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  अगर (grouping != GM_NONE && output_option_used)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (निकास_त्रुटि);
    }

  अगर (grouping != GM_NONE && गिनती_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (निकास_त्रुटि);
    }

  अगर (गिनती_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (निकास_त्रुटि);
    }

  check_file (file[0], file[1], delimiter);

  वापस निकास_सफल;
}
