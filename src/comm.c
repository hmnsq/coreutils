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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "comm"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
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
ಸ್ಥಿರ bool ನೋಡಿದ_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ಸ್ಥಿರ bool issued_disorder_warning[2];

/* line delimiter.  */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ delim = '\n';

/* If true, print a summary.  */
ಸ್ಥಿರ bool ಒಟ್ಟು_option;

/* If nonzero, check that the input is correctly ordered. */
ಸ್ಥಿರ ಎಣಿಕೆ
  {
    CHECK_ಕ್ರಮ_DEFAULT,
    CHECK_ಕ್ರಮ_ENABLED,
    CHECK_ಕ್ರಮ_DISABLED
  } check_ಒಳಹರಿವು_ಕ್ರಮ;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಲಂಬಸಾಲು_sep = "\t";
ಸ್ಥಿರ size_t ಲಂಬಸಾಲು_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  CHECK_ಕ್ರಮ_OPTION = CHAR_MAX + 1,
  NOCHECK_ಕ್ರಮ_OPTION,
  OUTPUT_DELIMITER_OPTION,
  ಒಟ್ಟು_OPTION
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"check-order", no_ವಾದ, ಖಾಲಿ, CHECK_ಕ್ರಮ_OPTION},
  {"nocheck-order", no_ವಾದ, ಖಾಲಿ, NOCHECK_ಕ್ರಮ_OPTION},
  {"output-delimiter", required_ವಾದ, ಖಾಲಿ, OUTPUT_DELIMITER_OPTION},
  {"total", no_ವಾದ, ಖಾಲಿ, ಒಟ್ಟು_OPTION},
  {"zero-terminated", no_ವಾದ, ಖಾಲಿ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};


ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 ಸಾಲು by ಸಾಲು.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, ಪಠ standard ಒಳಹರಿವು.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-ಸ್ತಂಭ output.  ಸ್ತಂಭ_2 one ಒಳಗೊಂಡಿದೆ\n\
ಸಾಲುಗಳು unique ಗಮ್ಯ FILE1, ಸ್ತಂಭ two ಒಳಗೊಂಡಿದೆ ಸಾಲುಗಳು unique ಗಮ್ಯ FILE2,\n\
and ಸ್ತಂಭ three ಒಳಗೊಂಡಿದೆ ಸಾಲುಗಳು common ಗಮ್ಯ both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress ಸ್ತಂಭ 1 (ಸಾಲುಗಳು unique ಗಮ್ಯ FILE1)\n\
"));
      oputs (_("\
  -2     suppress ಸ್ತಂಭ 2 (ಸಾಲುಗಳು unique ಗಮ್ಯ FILE2)\n\
"));
      oputs (_("\
  -3     suppress ಸ್ತಂಭ 3 (ಸಾಲುಗಳು that appear ಒಳ both files)\n\
"));
      oputs (_("\
      --check-ಕ್ರಮ\n\
         check that the ಒಳಹರಿವು is correctly sorted,\n\
         even ಆದರೆ all ಒಳಹರಿವು ಸಾಲುಗಳು are pairable\n\
"));
      oputs (_("\
      --nocheck-ಕ್ರಮ\n\
         ಮಾಡು not check that the ಒಳಹರಿವು is correctly sorted\n\
"));
      oputs (_("\
      --output-delimiter=STR\n\
         separate columns with STR\n\
"));
      oputs (_("\
      --ಒಟ್ಟು\n\
         output a summary\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Comparisons honor the ನಿಯಮಗಳು specified by 'LC_COLLATE'.\n\
"), stdout);
      ಬರೆ (_("\
\n\
Examples:\n\
  %s -12 file1 file2  Print only ಸಾಲುಗಳು present ಒಳ both file1 and file2.\n\
  %s -3 file1 file2  Print ಸಾಲುಗಳು ಒಳ file1 not ಒಳ file2, and vice versa.\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Output the line in linebuffer LINE to stdout
   provided the switches say it should be output.
   CLASS is 1 for a line found only in file 1,
   2 for a line only in file 2, 3 for a line in both. */

ಸ್ಥಿರ ಶೂನ್ಯ
writeline (ರಚನೆ linebuffer ಸ್ಥಿರಾಂಕ *ಸಾಲು, ಪೂರ್ಣಾಂಕ class)
{
  ಆಯ್ಕೆ (class)
    {
    ಸ್ಥಿತಿ 1:
      ಆದರೆ (!only_file_1)
        ಹಿಂತಿರುಗಿ;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 2:
      ಆದರೆ (!only_file_2)
        ಹಿಂತಿರುಗಿ;
      ಆದರೆ (only_file_1)
        fwrite (ಲಂಬಸಾಲು_sep, 1, ಲಂಬಸಾಲು_sep_len, stdout);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 3:
      ಆದರೆ (!both)
        ಹಿಂತಿರುಗಿ;
      ಆದರೆ (only_file_1)
        fwrite (ಲಂಬಸಾಲು_sep, 1, ಲಂಬಸಾಲು_sep_len, stdout);
      ಆದರೆ (only_file_2)
        fwrite (ಲಂಬಸಾಲು_sep, 1, ಲಂಬಸಾಲು_sep_len, stdout);
      ನಿಲ್ಲಿಸು;
    }

  fwrite (ಸಾಲು->ಬಫರ್, ಗಾತ್ರ (ಅಕ್ಷರ), ಸಾಲು->ಉದ್ದಳತೆ, stdout);

  ಆದರೆ (ferror (stdout))
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
check_ಕ್ರಮ (ರಚನೆ linebuffer ಸ್ಥಿರಾಂಕ *prev,
             ರಚನೆ linebuffer ಸ್ಥಿರಾಂಕ *current,
             ಪೂರ್ಣಾಂಕ whatfile)
{

  ಆದರೆ (check_ಒಳಹರಿವು_ಕ್ರಮ != CHECK_ಕ್ರಮ_DISABLED
      && ((check_ಒಳಹರಿವು_ಕ್ರಮ == CHECK_ಕ್ರಮ_ENABLED) || ನೋಡಿದ_unpairable))
    {
      ಆದರೆ (!issued_disorder_warning[whatfile - 1])
        {
          ಪೂರ್ಣಾಂಕ ಕ್ರಮ;

          ಆದರೆ (hard_LC_COLLATE)
            ಕ್ರಮ = xmemcoll (prev->ಬಫರ್, prev->ಉದ್ದಳತೆ - 1,
                              current->ಬಫರ್, current->ಉದ್ದಳತೆ - 1);
          ಇಲ್ಲದಿದ್ದರೆ
            ಕ್ರಮ = memcmp2 (prev->ಬಫರ್, prev->ಉದ್ದಳತೆ - 1,
                             current->ಬಫರ್, current->ಉದ್ದಳತೆ - 1);

          ಆದರೆ (0 < ಕ್ರಮ)
            {
              error ((check_ಒಳಹರಿವು_ಕ್ರಮ == CHECK_ಕ್ರಮ_ENABLED
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
compare_files (ಅಕ್ಷರ **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  ರಚನೆ linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  ರಚನೆ linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  ರಚನೆ linebuffer *all_ಸಾಲು[2][4];

  /* This is used to rotate through the buffers for each input file. */
  ಪೂರ್ಣಾಂಕ alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t ಒಟ್ಟು[] = {0, 0, 0};

  /* Initialize the storage. */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2; i++)
    {
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_ಸಾಲು[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      ಆದರೆ (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_ಸಾಲು[i][alt[i][0]], streams[i],
                                          delim);
      ಆದರೆ (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  ಆಗ (thisline[0] || thisline[1])
    {
      ಪೂರ್ಣಾಂಕ ಕ್ರಮ;
      bool fill_ಮೇಲೆ[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      ಆದರೆ (!thisline[0])
        ಕ್ರಮ = 1;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!thisline[1])
        ಕ್ರಮ = -1;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (hard_LC_COLLATE)
            ಕ್ರಮ = xmemcoll (thisline[0]->ಬಫರ್, thisline[0]->ಉದ್ದಳತೆ - 1,
                              thisline[1]->ಬಫರ್, thisline[1]->ಉದ್ದಳತೆ - 1);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              size_t len = MIN (thisline[0]->ಉದ್ದಳತೆ, thisline[1]->ಉದ್ದಳತೆ) - 1;
              ಕ್ರಮ = memcmp (thisline[0]->ಬಫರ್, thisline[1]->ಬಫರ್, len);
              ಆದರೆ (ಕ್ರಮ == 0)
                ಕ್ರಮ = _GL_CMP (thisline[0]->ಉದ್ದಳತೆ, thisline[1]->ಉದ್ದಳತೆ);
            }
        }

      /* Output the line that is lesser. */
      ಆದರೆ (ಕ್ರಮ == 0)
        {
          /* Line is seen in both files.  */
          ಒಟ್ಟು[2]++;
          writeline (thisline[1], 3);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ನೋಡಿದ_unpairable = true;
          ಆದರೆ (ಕ್ರಮ <= 0)
            {
              /* Line is seen in file 1 only.  */
              ಒಟ್ಟು[0]++;
              writeline (thisline[0], 1);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Line is seen in file 2 only.  */
              ಒಟ್ಟು[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      ಆದರೆ (0 <= ಕ್ರಮ)
        fill_ಮೇಲೆ[1] = true;
      ಆದರೆ (ಕ್ರಮ <= 0)
        fill_ಮೇಲೆ[0] = true;

      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2; i++)
        ಆದರೆ (fill_ಮೇಲೆ[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_ಸಾಲು[i][alt[i][0]],
                                                streams[i], delim);

            ಆದರೆ (thisline[i])
              check_ಕ್ರಮ (all_ಸಾಲು[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (all_ಸಾಲು[i][alt[i][2]]->ಬಫರ್)
              check_ಕ್ರಮ (all_ಸಾಲು[i][alt[i][2]],
                           all_ಸಾಲು[i][alt[i][1]], i + 1);

            ಆದರೆ (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_ಮೇಲೆ[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n_streams = 2 - (streams[0] == streams[1]);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_streams; i++)
    ಆದರೆ (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  ಆದರೆ (ಒಟ್ಟು_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      ಆದರೆ (ಲಂಬಸಾಲು_sep_len == 1)
        { /* Separate to handle NUL char.  */
          ಬರೆ ("%ju%c%ju%c%ju%c%s%c",
                  ಒಟ್ಟು[0], *ಲಂಬಸಾಲು_sep,
                  ಒಟ್ಟು[1], *ಲಂಬಸಾಲು_sep,
                  ಒಟ್ಟು[2], *ಲಂಬಸಾಲು_sep,
                  _("total"), delim);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಬರೆ ("%ju%s%ju%s%ju%s%s%c",
                  ಒಟ್ಟು[0], ಲಂಬಸಾಲು_sep,
                  ಒಟ್ಟು[1], ಲಂಬಸಾಲು_sep,
                  ಒಟ್ಟು[2], ಲಂಬಸಾಲು_sep,
                  _("total"), delim);
        }
    }

  ಆದರೆ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ c;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  ಆಗ ((c = getopt_long (argc, argv, "123z", long_options, ಖಾಲಿ)) != -1)
    ಆಯ್ಕೆ (c)
      {
      ಸ್ಥಿತಿ '1':
        only_file_1 = false;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ '2':
        only_file_2 = false;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ '3':
        both = false;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'z':
        delim = '\0';
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ NOCHECK_ಕ್ರಮ_OPTION:
        check_ಒಳಹರಿವು_ಕ್ರಮ = CHECK_ಕ್ರಮ_DISABLED;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ CHECK_ಕ್ರಮ_OPTION:
        check_ಒಳಹರಿವು_ಕ್ರಮ = CHECK_ಕ್ರಮ_ENABLED;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ OUTPUT_DELIMITER_OPTION:
        ಆದರೆ (ಲಂಬಸಾಲು_sep_len && !streq (ಲಂಬಸಾಲು_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        ಲಂಬಸಾಲು_sep = optarg;
        ಲಂಬಸಾಲು_sep_len = *optarg ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (optarg) : 1;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ ಒಟ್ಟು_OPTION:
        ಒಟ್ಟು_option = true;
        ನಿಲ್ಲಿಸು;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

      ಪೂರ್ವನಿಯೋಜಿತ:
        usage (EXIT_FAILURE);
      }

  ಆದರೆ (! ಲಂಬಸಾಲು_sep_len)
    ಲಂಬಸಾಲು_sep_len = 1;

  ಆದರೆ (argc - optind < 2)
    {
      ಆದರೆ (argc <= optind)
        error (0, 0, _("missing operand"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
