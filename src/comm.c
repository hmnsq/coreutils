/* comm -- compare two sorted files line by line.
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

/* Written by Richard Stallman and David MacKenzie. */

#अन्तर्भाव <config.h>

#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव "linebuffer.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "hard-locale.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "memcmp2.h"
#अन्तर्भाव "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "comm"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
स्थिर bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
स्थिर bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
स्थिर bool only_file_2 = true;

/* If true, print lines that are found in both files. */
स्थिर bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
स्थिर bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
स्थिर bool issued_disorder_warning[2];

/* line delimiter.  */
स्थिर अचिन्हित वर्ण delim = '\n';

/* If true, print a summary.  */
स्थिर bool total_option;

/* If nonzero, check that the input is correctly ordered. */
स्थिर गणना
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_आगत_order;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
स्थिर वर्ण अचर *col_sep = "\t";
स्थिर माप_प्रकार col_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  CHECK_ORDER_OPTION = अक्षर_उच्च + 1,
  NOCHECK_ORDER_OPTION,
  OUTPUT_DELIMITER_OPTION,
  TOTAL_OPTION
};

स्थिर संरचना option अचर long_options[] =
{
  {"check-order", no_argument, रिक्त, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, रिक्त, NOCHECK_ORDER_OPTION},
  {"output-delimiter", required_argument, रिक्त, OUTPUT_DELIMITER_OPTION},
  {"total", no_argument, रिक्त, TOTAL_OPTION},
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
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      ख_माला_दो (_("\
Compare sorted files FILE1 and FILE2 पङ्क्ति by पङ्क्ति.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
When FILE1 or FILE2 is -, पठ standard आगत.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
With no options, produce three-column output.  Column one contains\n\
पङ्क्तिहरू unique to FILE1, column two contains पङ्क्तिहरू unique to FILE2,\n\
and column three contains पङ्क्तिहरू common to both files.\n\
"), मानक_निकास);
      oputs (_("\
  -1     suppress column 1 (पङ्क्तिहरू unique to FILE1)\n\
"));
      oputs (_("\
  -2     suppress column 2 (पङ्क्तिहरू unique to FILE2)\n\
"));
      oputs (_("\
  -3     suppress column 3 (पङ्क्तिहरू that appear in both files)\n\
"));
      oputs (_("\
      --check-order\n\
         check that the आगत is correctly sorted,\n\
         even यदि all आगत पङ्क्तिहरू are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         गर not check that the आगत is correctly sorted\n\
"));
      oputs (_("\
      --output-delimiter=STR\n\
         separate columns with STR\n\
"));
      oputs (_("\
      --total\n\
         output a summary\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्ति delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
"), मानक_निकास);
      म_लिखो (_("\
\n\
Examples:\n\
  %s -12 file1 file2  Print only पङ्क्तिहरू present in both file1 and file2.\n\
  %s -3 file1 file2  Print पङ्क्तिहरू in file1 not in file2, and vice versa.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Output the line in linebuffer LINE to stdout
   provided the switches say it should be output.
   CLASS is 1 for a line found only in file 1,
   2 for a line only in file 2, 3 for a line in both. */

स्थिर शून्य
writeline (संरचना linebuffer अचर *पङ्क्ति, पूर्णाङ्क class)
{
  विकल्प (class)
    {
    पक्ष 1:
      यदि (!only_file_1)
        फर्काऊ;
      रोक;

    पक्ष 2:
      यदि (!only_file_2)
        फर्काऊ;
      यदि (only_file_1)
        ख_डालो (col_sep, 1, col_sep_len, मानक_निकास);
      रोक;

    पक्ष 3:
      यदि (!both)
        फर्काऊ;
      यदि (only_file_1)
        ख_डालो (col_sep, 1, col_sep_len, मानक_निकास);
      यदि (only_file_2)
        ख_डालो (col_sep, 1, col_sep_len, मानक_निकास);
      रोक;
    }

  ख_डालो (पङ्क्ति->buffer, परिमाण (वर्ण), पङ्क्ति->length, मानक_निकास);

  यदि (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

स्थिर शून्य
check_order (संरचना linebuffer अचर *prev,
             संरचना linebuffer अचर *current,
             पूर्णाङ्क whatfile)
{

  यदि (check_आगत_order != CHECK_ORDER_DISABLED
      && ((check_आगत_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      यदि (!issued_disorder_warning[whatfile - 1])
        {
          पूर्णाङ्क order;

          यदि (hard_LC_COLLATE)
            order = xmemcoll (prev->buffer, prev->length - 1,
                              current->buffer, current->length - 1);
          अन्यथा
            order = memcmp2 (prev->buffer, prev->length - 1,
                             current->buffer, current->length - 1);

          यदि (0 < order)
            {
              error ((check_आगत_order == CHECK_ORDER_ENABLED
                      ? निकास_त्रुटि : 0),
                     0, _("file %d is not in sorted order"), whatfile);

              /* If we get to here, the message was just a warning, but we
                 want only to issue it once. */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

/* Compare INFILES[0] and INFILES[1].
   If either is "-", use the standard input for that file.
   Assume that each input file is sorted;
   merge them and output the result.
   Exit the program when done.  */

स्थिर _Noreturn शून्य
compare_files (वर्ण **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  संरचना linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  संरचना linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  संरचना linebuffer *all_पङ्क्ति[2][4];

  /* This is used to rotate through the buffers for each input file. */
  पूर्णाङ्क alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  खाता *streams[2];

  /* Counters for the summary.  */
  uintmax_t total[] = {0, 0, 0};

  /* Initialize the storage. */
  लागि (पूर्णाङ्क i = 0; i < 2; i++)
    {
      लागि (पूर्णाङ्क j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_पङ्क्ति[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? मानक_निवेश : ख_खोलो (infiles[i], "r"));
      यदि (!streams[i])
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_पङ्क्ति[i][alt[i][0]], streams[i],
                                          delim);
      यदि (ख_त्रुटि (streams[i]))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infiles[i]));
    }

  जबसम्म (thisline[0] || thisline[1])
    {
      पूर्णाङ्क order;
      bool fill_up[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      यदि (!thisline[0])
        order = 1;
      अन्यथा यदि (!thisline[1])
        order = -1;
      अन्यथा
        {
          यदि (hard_LC_COLLATE)
            order = xmemcoll (thisline[0]->buffer, thisline[0]->length - 1,
                              thisline[1]->buffer, thisline[1]->length - 1);
          अन्यथा
            {
              माप_प्रकार len = MIN (thisline[0]->length, thisline[1]->length) - 1;
              order = स_भेद (thisline[0]->buffer, thisline[1]->buffer, len);
              यदि (order == 0)
                order = _GL_CMP (thisline[0]->length, thisline[1]->length);
            }
        }

      /* Output the line that is lesser. */
      यदि (order == 0)
        {
          /* Line is seen in both files.  */
          total[2]++;
          writeline (thisline[1], 3);
        }
      अन्यथा
        {
          seen_unpairable = true;
          यदि (order <= 0)
            {
              /* Line is seen in file 1 only.  */
              total[0]++;
              writeline (thisline[0], 1);
            }
          अन्यथा
            {
              /* Line is seen in file 2 only.  */
              total[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      यदि (0 <= order)
        fill_up[1] = true;
      यदि (order <= 0)
        fill_up[0] = true;

      लागि (पूर्णाङ्क i = 0; i < 2; i++)
        यदि (fill_up[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_पङ्क्ति[i][alt[i][0]],
                                                streams[i], delim);

            यदि (thisline[i])
              check_order (all_पङ्क्ति[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            अन्यथा यदि (all_पङ्क्ति[i][alt[i][2]]->buffer)
              check_order (all_पङ्क्ति[i][alt[i][2]],
                           all_पङ्क्ति[i][alt[i][1]], i + 1);

            यदि (ख_त्रुटि (streams[i]))
              error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infiles[i]));

            fill_up[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  अचर पूर्णाङ्क n_streams = 2 - (streams[0] == streams[1]);
  लागि (पूर्णाङ्क i = 0; i < n_streams; i++)
    यदि (ख_बंद (streams[i]) != 0)
      error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infiles[i]));

  यदि (total_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      यदि (col_sep_len == 1)
        { /* Separate to handle NUL char.  */
          म_लिखो ("%ju%c%ju%c%ju%c%s%c",
                  total[0], *col_sep,
                  total[1], *col_sep,
                  total[2], *col_sep,
                  _("total"), delim);
        }
      अन्यथा
        {
          म_लिखो ("%ju%s%ju%s%ju%s%s%c",
                  total[0], col_sep,
                  total[1], col_sep,
                  total[2], col_sep,
                  _("total"), delim);
        }
    }

  यदि (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (निकास_त्रुटि, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  निकास (निकास_सफल);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  निकास_पर (close_stdout);

  जबसम्म ((c = getopt_long (argc, argv, "123z", long_options, रिक्त)) != -1)
    विकल्प (c)
      {
      पक्ष '1':
        only_file_1 = false;
        रोक;

      पक्ष '2':
        only_file_2 = false;
        रोक;

      पक्ष '3':
        both = false;
        रोक;

      पक्ष 'z':
        delim = '\0';
        रोक;

      पक्ष NOCHECK_ORDER_OPTION:
        check_आगत_order = CHECK_ORDER_DISABLED;
        रोक;

      पक्ष CHECK_ORDER_OPTION:
        check_आगत_order = CHECK_ORDER_ENABLED;
        रोक;

      पक्ष OUTPUT_DELIMITER_OPTION:
        यदि (col_sep_len && !streq (col_sep, optarg))
          error (निकास_त्रुटि, 0, _("multiple output delimiters specified"));
        col_sep = optarg;
        col_sep_len = *optarg ? म_माप (optarg) : 1;
        रोक;

      पक्ष TOTAL_OPTION:
        total_option = true;
        रोक;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      स्वभाव:
        usage (निकास_त्रुटि);
      }

  यदि (! col_sep_len)
    col_sep_len = 1;

  यदि (argc - optind < 2)
    {
      यदि (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  यदि (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (निकास_त्रुटि);
    }

  compare_files (argv + optind);
}
