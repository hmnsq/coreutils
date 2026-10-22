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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "linebuffer.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "hard-locale.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "stdio--.h"
#অন্তর্ভাব "memcmp2.h"
#অন্তর্ভাব "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "comm"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
স্থির bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
স্থির bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
স্থির bool only_file_2 = true;

/* If true, print lines that are found in both files. */
স্থির bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
স্থির bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
স্থির bool issued_disorder_warning[2];

/* line delimiter.  */
স্থির unsigned অক্ষর delim = '\n';

/* If true, print a summary.  */
স্থির bool যোগ_option;

/* If nonzero, check that the input is correctly ordered. */
স্থির গণনা
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_প্রবেশ_order;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
স্থির অক্ষর ধ্রুবক *col_sep = "\t";
স্থির size_t col_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  OUTPUT_DELIMITER_OPTION,
  যোগ_OPTION
};

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"check-order", no_argument, ফাঁকা, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, ফাঁকা, NOCHECK_ORDER_OPTION},
  {"output-delimiter", required_argument, ফাঁকা, OUTPUT_DELIMITER_OPTION},
  {"total", no_argument, ফাঁকা, যোগ_OPTION},
  {"zero-terminated", no_argument, ফাঁকা, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};


শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 সারি by সারি.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, পঠ standard প্রবেশ.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-column output.  Column one contains\n\
সারিগুলি unique to FILE1, column two contains সারিগুলি unique to FILE2,\n\
and column three contains সারিগুলি common to both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress column 1 (সারিগুলি unique to FILE1)\n\
"));
      oputs (_("\
  -2     suppress column 2 (সারিগুলি unique to FILE2)\n\
"));
      oputs (_("\
  -3     suppress column 3 (সারিগুলি that appear in both files)\n\
"));
      oputs (_("\
      --check-order\n\
         check that the প্রবেশ is correctly sorted,\n\
         even যদি all প্রবেশ সারিগুলি are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         করো not check that the প্রবেশ is correctly sorted\n\
"));
      oputs (_("\
      --output-delimiter=STR\n\
         separate columns with STR\n\
"));
      oputs (_("\
      --যোগ_2\n\
         output a summary\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         সারি delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
"), stdout);
      printf (_("\
\n\
Examples:\n\
  %s -12 file1 file2  Print only সারিগুলি present in both file1 and file2.\n\
  %s -3 file1 file2  Print সারিগুলি in file1 not in file2, and vice versa.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Output the line in linebuffer LINE to stdout
   provided the switches say it should be output.
   CLASS is 1 for a line found only in file 1,
   2 for a line only in file 2, 3 for a line in both. */

স্থির শূন্য
writeline (কাঠামো linebuffer ধ্রুবক *সারি, পূর্ণসংখ্যা class)
{
  নির্বাচন (class)
    {
    ক্ষেত্র 1:
      যদি (!only_file_1)
        ফেরত;
      বিরতি;

    ক্ষেত্র 2:
      যদি (!only_file_2)
        ফেরত;
      যদি (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      বিরতি;

    ক্ষেত্র 3:
      যদি (!both)
        ফেরত;
      যদি (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      যদি (only_file_2)
        fwrite (col_sep, 1, col_sep_len, stdout);
      বিরতি;
    }

  fwrite (সারি->বাফার, আকার (অক্ষর), সারি->দৈর্ঘ্য, stdout);

  যদি (ferror (stdout))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

স্থির শূন্য
check_order (কাঠামো linebuffer ধ্রুবক *পূর্ব,
             কাঠামো linebuffer ধ্রুবক *current,
             পূর্ণসংখ্যা whatfile)
{

  যদি (check_প্রবেশ_order != CHECK_ORDER_DISABLED
      && ((check_প্রবেশ_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      যদি (!issued_disorder_warning[whatfile - 1])
        {
          পূর্ণসংখ্যা order;

          যদি (hard_LC_COLLATE)
            order = xmemcoll (পূর্ব->বাফার, পূর্ব->দৈর্ঘ্য - 1,
                              current->বাফার, current->দৈর্ঘ্য - 1);
          অন্যথা
            order = memcmp2 (পূর্ব->বাফার, পূর্ব->দৈর্ঘ্য - 1,
                             current->বাফার, current->দৈর্ঘ্য - 1);

          যদি (0 < order)
            {
              error ((check_প্রবেশ_order == CHECK_ORDER_ENABLED
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

স্থির _Noreturn শূন্য
compare_files (অক্ষর **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  কাঠামো linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  কাঠামো linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  কাঠামো linebuffer *all_সারি[2][4];

  /* This is used to rotate through the buffers for each input file. */
  পূর্ণসংখ্যা alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t যোগ_2[] = {0, 0, 0};

  /* Initialize the storage. */
  জন্য (পূর্ণসংখ্যা i = 0; i < 2; i++)
    {
      জন্য (পূর্ণসংখ্যা j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_সারি[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      যদি (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_সারি[i][alt[i][0]], streams[i],
                                          delim);
      যদি (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  যখন (thisline[0] || thisline[1])
    {
      পূর্ণসংখ্যা order;
      bool fill_up[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      যদি (!thisline[0])
        order = 1;
      অন্যথা যদি (!thisline[1])
        order = -1;
      অন্যথা
        {
          যদি (hard_LC_COLLATE)
            order = xmemcoll (thisline[0]->বাফার, thisline[0]->দৈর্ঘ্য - 1,
                              thisline[1]->বাফার, thisline[1]->দৈর্ঘ্য - 1);
          অন্যথা
            {
              size_t len = MIN (thisline[0]->দৈর্ঘ্য, thisline[1]->দৈর্ঘ্য) - 1;
              order = memcmp (thisline[0]->বাফার, thisline[1]->বাফার, len);
              যদি (order == 0)
                order = _GL_CMP (thisline[0]->দৈর্ঘ্য, thisline[1]->দৈর্ঘ্য);
            }
        }

      /* Output the line that is lesser. */
      যদি (order == 0)
        {
          /* Line is seen in both files.  */
          যোগ_2[2]++;
          writeline (thisline[1], 3);
        }
      অন্যথা
        {
          seen_unpairable = true;
          যদি (order <= 0)
            {
              /* Line is seen in file 1 only.  */
              যোগ_2[0]++;
              writeline (thisline[0], 1);
            }
          অন্যথা
            {
              /* Line is seen in file 2 only.  */
              যোগ_2[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      যদি (0 <= order)
        fill_up[1] = true;
      যদি (order <= 0)
        fill_up[0] = true;

      জন্য (পূর্ণসংখ্যা i = 0; i < 2; i++)
        যদি (fill_up[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_সারি[i][alt[i][0]],
                                                streams[i], delim);

            যদি (thisline[i])
              check_order (all_সারি[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            অন্যথা যদি (all_সারি[i][alt[i][2]]->বাফার)
              check_order (all_সারি[i][alt[i][2]],
                           all_সারি[i][alt[i][1]], i + 1);

            যদি (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_up[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  ধ্রুবক পূর্ণসংখ্যা n_streams = 2 - (streams[0] == streams[1]);
  জন্য (পূর্ণসংখ্যা i = 0; i < n_streams; i++)
    যদি (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  যদি (যোগ_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      যদি (col_sep_len == 1)
        { /* Separate to handle NUL char.  */
          printf ("%ju%c%ju%c%ju%c%s%c",
                  যোগ_2[0], *col_sep,
                  যোগ_2[1], *col_sep,
                  যোগ_2[2], *col_sep,
                  _("total"), delim);
        }
      অন্যথা
        {
          printf ("%ju%s%ju%s%ju%s%s%c",
                  যোগ_2[0], col_sep,
                  যোগ_2[1], col_sep,
                  যোগ_2[2], col_sep,
                  _("total"), delim);
        }
    }

  যদি (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  exit (EXIT_SUCCESS);
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  যখন ((c = getopt_long (argc, argv, "123z", long_options, ফাঁকা)) != -1)
    নির্বাচন (c)
      {
      ক্ষেত্র '1':
        only_file_1 = false;
        বিরতি;

      ক্ষেত্র '2':
        only_file_2 = false;
        বিরতি;

      ক্ষেত্র '3':
        both = false;
        বিরতি;

      ক্ষেত্র 'z':
        delim = '\0';
        বিরতি;

      ক্ষেত্র NOCHECK_ORDER_OPTION:
        check_প্রবেশ_order = CHECK_ORDER_DISABLED;
        বিরতি;

      ক্ষেত্র CHECK_ORDER_OPTION:
        check_প্রবেশ_order = CHECK_ORDER_ENABLED;
        বিরতি;

      ক্ষেত্র OUTPUT_DELIMITER_OPTION:
        যদি (col_sep_len && !streq (col_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        col_sep = optarg;
        col_sep_len = *optarg ? strlen (optarg) : 1;
        বিরতি;

      ক্ষেত্র যোগ_OPTION:
        যোগ_option = true;
        বিরতি;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      পূর্বনির্ধারিত:
        usage (EXIT_FAILURE);
      }

  যদি (! col_sep_len)
    col_sep_len = 1;

  যদি (argc - optind < 2)
    {
      যদি (argc <= optind)
        error (0, 0, _("missing operand"));
      অন্যথা
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  যদি (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
