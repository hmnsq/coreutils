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
#परिभाषय क्रमादेशः_NAME "comm"

#परिभाषय AUTHORS \
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
स्थिर bool आलोकितम्_unpairable;

/* If nonzero, we have warned about disorder in that file. */
स्थिर bool issued_disorder_warning[2];

/* line delimiter.  */
स्थिर अचिह्नित वर्ण delim = '\n';

/* If true, print a summary.  */
स्थिर bool योग_option;

/* If nonzero, check that the input is correctly ordered. */
स्थिर गणना
  {
    CHECK_क्रमः_DEFAULT,
    CHECK_क्रमः_ENABLED,
    CHECK_क्रमः_DISABLED
  } check_निवेशः_क्रमः;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
स्थिर वर्ण ध्रुव *col_sep = "\t";
स्थिर size_t col_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  CHECK_क्रमः_OPTION = CHAR_MAX + 1,
  NOCHECK_क्रमः_OPTION,
  OUTPUT_DELIMITER_OPTION,
  योग_OPTION
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"check-order", no_तर्कः, रिक्त, CHECK_क्रमः_OPTION},
  {"nocheck-order", no_तर्कः, रिक्त, NOCHECK_क्रमः_OPTION},
  {"output-delimiter", required_तर्कः, रिक्त, OUTPUT_DELIMITER_OPTION},
  {"total", no_तर्कः, रिक्त, योग_OPTION},
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
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              क्रमादेशः_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 पङ्क्तिः by पङ्क्तिः.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, पठ standard निवेशः.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-column output.  Column one अन्तर्भवति\n\
पङ्क्तयः unique गन्तव्यम् FILE1, column two अन्तर्भवति पङ्क्तयः unique गन्तव्यम् FILE2,\n\
and column three अन्तर्भवति पङ्क्तयः common गन्तव्यम् both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress column 1 (पङ्क्तयः unique गन्तव्यम् FILE1)\n\
"));
      oputs (_("\
  -2     suppress column 2 (पङ्क्तयः unique गन्तव्यम् FILE2)\n\
"));
      oputs (_("\
  -3     suppress column 3 (पङ्क्तयः that appear अन्तर् both files)\n\
"));
      oputs (_("\
      --check-क्रमः\n\
         check that the निवेशः is correctly sorted,\n\
         even यदि all निवेशः पङ्क्तयः are pairable\n\
"));
      oputs (_("\
      --nocheck-क्रमः\n\
         कुरु not check that the निवेशः is correctly sorted\n\
"));
      oputs (_("\
      --output-delimiter=STR\n\
         separate columns with STR\n\
"));
      oputs (_("\
      --योग_2\n\
         output a summary\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Comparisons honor the नियमाः specified by 'LC_COLLATE'.\n\
"), stdout);
      प्रारूप_लिख (_("\
\n\
Examples:\n\
  %s -12 file1 file2  Print only पङ्क्तयः present अन्तर् both file1 and file2.\n\
  %s -3 file1 file2  Print पङ्क्तयः अन्तर् file1 not अन्तर् file2, and vice versa.\n\
"),
              क्रमादेशः_name, क्रमादेशः_name);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Output the line in linebuffer LINE to stdout
   provided the switches say it should be output.
   CLASS is 1 for a line found only in file 1,
   2 for a line only in file 2, 3 for a line in both. */

स्थिर शून्य
writeline (संरचना linebuffer ध्रुव *पङ्क्तिः, पूर्णाङ्क class)
{
  विकल्प (class)
    {
    पक्ष 1:
      यदि (!only_file_1)
        प्रत्यागम;
      भङ्ग;

    पक्ष 2:
      यदि (!only_file_2)
        प्रत्यागम;
      यदि (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      भङ्ग;

    पक्ष 3:
      यदि (!both)
        प्रत्यागम;
      यदि (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      यदि (only_file_2)
        fwrite (col_sep, 1, col_sep_len, stdout);
      भङ्ग;
    }

  fwrite (पङ्क्तिः->बफर, परिमाण (वर्ण), पङ्क्तिः->दीर्घता, stdout);

  यदि (ferror (stdout))
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
check_क्रमः (संरचना linebuffer ध्रुव *पूर्व,
             संरचना linebuffer ध्रुव *current,
             पूर्णाङ्क whatfile)
{

  यदि (check_निवेशः_क्रमः != CHECK_क्रमः_DISABLED
      && ((check_निवेशः_क्रमः == CHECK_क्रमः_ENABLED) || आलोकितम्_unpairable))
    {
      यदि (!issued_disorder_warning[whatfile - 1])
        {
          पूर्णाङ्क क्रमः;

          यदि (hard_LC_COLLATE)
            क्रमः = xmemcoll (पूर्व->बफर, पूर्व->दीर्घता - 1,
                              current->बफर, current->दीर्घता - 1);
          अन्यथा
            क्रमः = memcmp2 (पूर्व->बफर, पूर्व->दीर्घता - 1,
                             current->बफर, current->दीर्घता - 1);

          यदि (0 < क्रमः)
            {
              error ((check_निवेशः_क्रमः == CHECK_क्रमः_ENABLED
                      ? EXIT_FAILURE : 0),
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
  संरचना linebuffer *all_पङ्क्तिः[2][4];

  /* This is used to rotate through the buffers for each input file. */
  पूर्णाङ्क alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t योग_2[] = {0, 0, 0};

  /* Initialize the storage. */
  प्रति (पूर्णाङ्क i = 0; i < 2; i++)
    {
      प्रति (पूर्णाङ्क j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_पङ्क्तिः[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      यदि (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_पङ्क्तिः[i][alt[i][0]], streams[i],
                                          delim);
      यदि (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  यावत् (thisline[0] || thisline[1])
    {
      पूर्णाङ्क क्रमः;
      bool fill_ऊर्ध्वम्[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      यदि (!thisline[0])
        क्रमः = 1;
      अन्यथा यदि (!thisline[1])
        क्रमः = -1;
      अन्यथा
        {
          यदि (hard_LC_COLLATE)
            क्रमः = xmemcoll (thisline[0]->बफर, thisline[0]->दीर्घता - 1,
                              thisline[1]->बफर, thisline[1]->दीर्घता - 1);
          अन्यथा
            {
              size_t len = MIN (thisline[0]->दीर्घता, thisline[1]->दीर्घता) - 1;
              क्रमः = memcmp (thisline[0]->बफर, thisline[1]->बफर, len);
              यदि (क्रमः == 0)
                क्रमः = _GL_CMP (thisline[0]->दीर्घता, thisline[1]->दीर्घता);
            }
        }

      /* Output the line that is lesser. */
      यदि (क्रमः == 0)
        {
          /* Line is seen in both files.  */
          योग_2[2]++;
          writeline (thisline[1], 3);
        }
      अन्यथा
        {
          आलोकितम्_unpairable = true;
          यदि (क्रमः <= 0)
            {
              /* Line is seen in file 1 only.  */
              योग_2[0]++;
              writeline (thisline[0], 1);
            }
          अन्यथा
            {
              /* Line is seen in file 2 only.  */
              योग_2[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      यदि (0 <= क्रमः)
        fill_ऊर्ध्वम्[1] = true;
      यदि (क्रमः <= 0)
        fill_ऊर्ध्वम्[0] = true;

      प्रति (पूर्णाङ्क i = 0; i < 2; i++)
        यदि (fill_ऊर्ध्वम्[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_पङ्क्तिः[i][alt[i][0]],
                                                streams[i], delim);

            यदि (thisline[i])
              check_क्रमः (all_पङ्क्तिः[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            अन्यथा यदि (all_पङ्क्तिः[i][alt[i][2]]->बफर)
              check_क्रमः (all_पङ्क्तिः[i][alt[i][2]],
                           all_पङ्क्तिः[i][alt[i][1]], i + 1);

            यदि (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_ऊर्ध्वम्[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  ध्रुव पूर्णाङ्क n_streams = 2 - (streams[0] == streams[1]);
  प्रति (पूर्णाङ्क i = 0; i < n_streams; i++)
    यदि (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  यदि (योग_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      यदि (col_sep_len == 1)
        { /* Separate to handle NUL char.  */
          प्रारूप_लिख ("%ju%c%ju%c%ju%c%s%c",
                  योग_2[0], *col_sep,
                  योग_2[1], *col_sep,
                  योग_2[2], *col_sep,
                  _("total"), delim);
        }
      अन्यथा
        {
          प्रारूप_लिख ("%ju%s%ju%s%ju%s%s%c",
                  योग_2[0], col_sep,
                  योग_2[1], col_sep,
                  योग_2[2], col_sep,
                  _("total"), delim);
        }
    }

  यदि (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  निर्गच्छ (EXIT_SUCCESS);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  यावत् ((c = getopt_long (argc, argv, "123z", long_options, रिक्त)) != -1)
    विकल्प (c)
      {
      पक्ष '1':
        only_file_1 = false;
        भङ्ग;

      पक्ष '2':
        only_file_2 = false;
        भङ्ग;

      पक्ष '3':
        both = false;
        भङ्ग;

      पक्ष 'z':
        delim = '\0';
        भङ्ग;

      पक्ष NOCHECK_क्रमः_OPTION:
        check_निवेशः_क्रमः = CHECK_क्रमः_DISABLED;
        भङ्ग;

      पक्ष CHECK_क्रमः_OPTION:
        check_निवेशः_क्रमः = CHECK_क्रमः_ENABLED;
        भङ्ग;

      पक्ष OUTPUT_DELIMITER_OPTION:
        यदि (col_sep_len && !streq (col_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        col_sep = optarg;
        col_sep_len = *optarg ? वाक्यदैर्घ्यम् (optarg) : 1;
        भङ्ग;

      पक्ष योग_OPTION:
        योग_option = true;
        भङ्ग;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

      स्वभाव:
        usage (EXIT_FAILURE);
      }

  यदि (! col_sep_len)
    col_sep_len = 1;

  यदि (argc - optind < 2)
    {
      यदि (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  यदि (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
