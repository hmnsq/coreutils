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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "linebuffer.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "hard-locale.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "memcmp2.h"
#ಅನ್ತರ್ಭಾವ "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "comm"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
ಸ್ಥಿರ bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
ಸ್ಥಿರ bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
ಸ್ಥಿರ bool only_file_2 = true;

/* If true, print lines that are found in both files. */
ಸ್ಥಿರ bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
ಸ್ಥಿರ bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ಸ್ಥಿರ bool issued_disorder_warning[2];

/* line delimiter.  */
ಸ್ಥಿರ unsigned ವರ್ಣ delim = '\n';

/* If true, print a summary.  */
ಸ್ಥಿರ bool total_option;

/* If nonzero, check that the input is correctly ordered. */
ಸ್ಥಿರ ಗಣನಾ
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_ಒಳಹರಿವು_order;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *col_sep = "\t";
ಸ್ಥಿರ size_t col_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  OUTPUT_DELIMITER_OPTION,
  TOTAL_OPTION
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"check-order", no_argument, ರಿಕ್ತ, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, ರಿಕ್ತ, NOCHECK_ORDER_OPTION},
  {"output-delimiter", required_argument, ರಿಕ್ತ, OUTPUT_DELIMITER_OPTION},
  {"total", no_argument, ರಿಕ್ತ, TOTAL_OPTION},
  {"zero-terminated", no_argument, ರಿಕ್ತ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};


ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 ಸಾಲು by ಸಾಲು.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, ಪಠ standard ಒಳಹರಿವು.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-column output.  Column one contains\n\
ಸಾಲುಗಳು unique to FILE1, column two contains ಸಾಲುಗಳು unique to FILE2,\n\
and column three contains ಸಾಲುಗಳು common to both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress column 1 (ಸಾಲುಗಳು unique to FILE1)\n\
"));
      oputs (_("\
  -2     suppress column 2 (ಸಾಲುಗಳು unique to FILE2)\n\
"));
      oputs (_("\
  -3     suppress column 3 (ಸಾಲುಗಳು that appear in both files)\n\
"));
      oputs (_("\
      --check-order\n\
         check that the ಒಳಹರಿವು is correctly sorted,\n\
         even ಯದಿ all ಒಳಹರಿವು ಸಾಲುಗಳು are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         ಕುರು not check that the ಒಳಹರಿವು is correctly sorted\n\
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
         ಸಾಲು delimiter is NUL, not newline\n\
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
  %s -12 file1 file2  Print only ಸಾಲುಗಳು present in both file1 and file2.\n\
  %s -3 file1 file2  Print ಸಾಲುಗಳು in file1 not in file2, and vice versa.\n\
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

ಸ್ಥಿರ ಶೂನ್ಯ
writeline (ಸಂರಚನಾ linebuffer ಧ್ರುವ *ಸಾಲು, ಪೂರ್ಣಾಙ್ಕ class)
{
  ವಿಕಲ್ಪ (class)
    {
    ಪಕ್ಷ 1:
      ಯದಿ (!only_file_1)
        ಪ್ರತ್ಯಾಗಮ;
      ಭಙ್ಗ;

    ಪಕ್ಷ 2:
      ಯದಿ (!only_file_2)
        ಪ್ರತ್ಯಾಗಮ;
      ಯದಿ (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      ಭಙ್ಗ;

    ಪಕ್ಷ 3:
      ಯದಿ (!both)
        ಪ್ರತ್ಯಾಗಮ;
      ಯದಿ (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      ಯದಿ (only_file_2)
        fwrite (col_sep, 1, col_sep_len, stdout);
      ಭಙ್ಗ;
    }

  fwrite (ಸಾಲು->buffer, ಪರಿಮಾಣ (ವರ್ಣ), ಸಾಲು->length, stdout);

  ಯದಿ (ferror (stdout))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

ಸ್ಥಿರ ಶೂನ್ಯ
check_order (ಸಂರಚನಾ linebuffer ಧ್ರುವ *prev,
             ಸಂರಚನಾ linebuffer ಧ್ರುವ *current,
             ಪೂರ್ಣಾಙ್ಕ whatfile)
{

  ಯದಿ (check_ಒಳಹರಿವು_order != CHECK_ORDER_DISABLED
      && ((check_ಒಳಹರಿವು_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      ಯದಿ (!issued_disorder_warning[whatfile - 1])
        {
          ಪೂರ್ಣಾಙ್ಕ order;

          ಯದಿ (hard_LC_COLLATE)
            order = xmemcoll (prev->buffer, prev->length - 1,
                              current->buffer, current->length - 1);
          ಅನ್ಯಥಾ
            order = memcmp2 (prev->buffer, prev->length - 1,
                             current->buffer, current->length - 1);

          ಯದಿ (0 < order)
            {
              error ((check_ಒಳಹರಿವು_order == CHECK_ORDER_ENABLED
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

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
compare_files (ವರ್ಣ **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  ಸಂರಚನಾ linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  ಸಂರಚನಾ linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  ಸಂರಚನಾ linebuffer *all_ಸಾಲು[2][4];

  /* This is used to rotate through the buffers for each input file. */
  ಪೂರ್ಣಾಙ್ಕ alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t total[] = {0, 0, 0};

  /* Initialize the storage. */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 2; i++)
    {
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_ಸಾಲು[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      ಯದಿ (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_ಸಾಲು[i][alt[i][0]], streams[i],
                                          delim);
      ಯದಿ (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  ಯಾವತ್ (thisline[0] || thisline[1])
    {
      ಪೂರ್ಣಾಙ್ಕ order;
      bool fill_up[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      ಯದಿ (!thisline[0])
        order = 1;
      ಅನ್ಯಥಾ ಯದಿ (!thisline[1])
        order = -1;
      ಅನ್ಯಥಾ
        {
          ಯದಿ (hard_LC_COLLATE)
            order = xmemcoll (thisline[0]->buffer, thisline[0]->length - 1,
                              thisline[1]->buffer, thisline[1]->length - 1);
          ಅನ್ಯಥಾ
            {
              size_t len = MIN (thisline[0]->length, thisline[1]->length) - 1;
              order = memcmp (thisline[0]->buffer, thisline[1]->buffer, len);
              ಯದಿ (order == 0)
                order = _GL_CMP (thisline[0]->length, thisline[1]->length);
            }
        }

      /* Output the line that is lesser. */
      ಯದಿ (order == 0)
        {
          /* Line is seen in both files.  */
          total[2]++;
          writeline (thisline[1], 3);
        }
      ಅನ್ಯಥಾ
        {
          seen_unpairable = true;
          ಯದಿ (order <= 0)
            {
              /* Line is seen in file 1 only.  */
              total[0]++;
              writeline (thisline[0], 1);
            }
          ಅನ್ಯಥಾ
            {
              /* Line is seen in file 2 only.  */
              total[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      ಯದಿ (0 <= order)
        fill_up[1] = true;
      ಯದಿ (order <= 0)
        fill_up[0] = true;

      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 2; i++)
        ಯದಿ (fill_up[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_ಸಾಲು[i][alt[i][0]],
                                                streams[i], delim);

            ಯದಿ (thisline[i])
              check_order (all_ಸಾಲು[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            ಅನ್ಯಥಾ ಯದಿ (all_ಸಾಲು[i][alt[i][2]]->buffer)
              check_order (all_ಸಾಲು[i][alt[i][2]],
                           all_ಸಾಲು[i][alt[i][1]], i + 1);

            ಯದಿ (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_up[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ n_streams = 2 - (streams[0] == streams[1]);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n_streams; i++)
    ಯದಿ (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  ಯದಿ (total_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      ಯದಿ (col_sep_len == 1)
        { /* Separate to handle NUL char.  */
          printf ("%ju%c%ju%c%ju%c%s%c",
                  total[0], *col_sep,
                  total[1], *col_sep,
                  total[2], *col_sep,
                  _("total"), delim);
        }
      ಅನ್ಯಥಾ
        {
          printf ("%ju%s%ju%s%ju%s%s%c",
                  total[0], col_sep,
                  total[1], col_sep,
                  total[2], col_sep,
                  _("total"), delim);
        }
    }

  ಯದಿ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  exit (EXIT_SUCCESS);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  ಯಾವತ್ ((c = getopt_long (argc, argv, "123z", long_options, ರಿಕ್ತ)) != -1)
    ವಿಕಲ್ಪ (c)
      {
      ಪಕ್ಷ '1':
        only_file_1 = false;
        ಭಙ್ಗ;

      ಪಕ್ಷ '2':
        only_file_2 = false;
        ಭಙ್ಗ;

      ಪಕ್ಷ '3':
        both = false;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'z':
        delim = '\0';
        ಭಙ್ಗ;

      ಪಕ್ಷ NOCHECK_ORDER_OPTION:
        check_ಒಳಹರಿವು_order = CHECK_ORDER_DISABLED;
        ಭಙ್ಗ;

      ಪಕ್ಷ CHECK_ORDER_OPTION:
        check_ಒಳಹರಿವು_order = CHECK_ORDER_ENABLED;
        ಭಙ್ಗ;

      ಪಕ್ಷ OUTPUT_DELIMITER_OPTION:
        ಯದಿ (col_sep_len && !streq (col_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        col_sep = optarg;
        col_sep_len = *optarg ? strlen (optarg) : 1;
        ಭಙ್ಗ;

      ಪಕ್ಷ TOTAL_OPTION:
        total_option = true;
        ಭಙ್ಗ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      ಸ್ವಭಾವ:
        usage (EXIT_FAILURE);
      }

  ಯದಿ (! col_sep_len)
    col_sep_len = 1;

  ಯದಿ (argc - optind < 2)
    {
      ಯದಿ (argc <= optind)
        error (0, 0, _("missing operand"));
      ಅನ್ಯಥಾ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
