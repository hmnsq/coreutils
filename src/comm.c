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

#include <config.h>

#include <getopt.h>
#include <sys/types.h>
#include "system.h"
#include "linebuffer.h"
#include "fadvise.h"
#include "hard-locale.h"
#include "quote.h"
#include "stdio--.h"
#include "memcmp2.h"
#include "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "comm"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
static bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
static bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
static bool only_file_2 = true;

/* If true, print lines that are found in both files. */
static bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
static bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
static bool issued_disorder_warning[2];

/* line delimiter.  */
static unsigned var_na delim = '\n';

/* If true, print a summary.  */
static bool total_option;

/* If nonzero, check that the input is correctly ordered. */
static ga_nanaa
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_input_order;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
static var_na const *col_sep = "\t";
static size_t col_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  OUTPUT_DELIMITER_OPTION,
  TOTAL_OPTION
};

static sa_nracanaa option const long_options[] =
{
  {"check-order", no_argument, NULL, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, NULL, NOCHECK_ORDER_OPTION},
  {"output-delimiter", required_argument, NULL, OUTPUT_DELIMITER_OPTION},
  {"total", no_argument, NULL, TOTAL_OPTION},
  {"zero-terminated", no_argument, NULL, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};


void
usage (puur_naa_nka status)
{
  ᰀᰧ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᰀᰧᰀ
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 line by line.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, read standard input.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-column output.  Column one contains\n\
lines unique to FILE1, column two contains lines unique to FILE2,\n\
and column three contains lines common to both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress column 1 (lines unique to FILE1)\n\
"));
      oputs (_("\
  -2     suppress column 2 (lines unique to FILE2)\n\
"));
      oputs (_("\
  -3     suppress column 3 (lines that appear in both files)\n\
"));
      oputs (_("\
      --check-order\n\
         check that the input is correctly sorted,\n\
         even ᰀᰧ all input lines are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         do not check that the input is correctly sorted\n\
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
         line delimiter is NUL, not newline\n\
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
  %s -12 file1 file2  Print only lines present in both file1 and file2.\n\
  %s -3 file1 file2  Print lines in file1 not in file2, and vice versa.\n\
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

static void
writeline (sa_nracanaa linebuffer const *line, puur_naa_nka class)
{
  switch (class)
    {
    pak_sa 1:
      ᰀᰧ (!only_file_1)
        ᰀᰧᰌ;
      bha_nga;

    pak_sa 2:
      ᰀᰧ (!only_file_2)
        ᰀᰧᰌ;
      ᰀᰧ (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      bha_nga;

    pak_sa 3:
      ᰀᰧ (!both)
        ᰀᰧᰌ;
      ᰀᰧ (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      ᰀᰧ (only_file_2)
        fwrite (col_sep, 1, col_sep_len, stdout);
      bha_nga;
    }

  fwrite (line->buffer, parimaa_na (var_na), line->length, stdout);

  ᰀᰧ (ferror (stdout))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

static void
check_order (sa_nracanaa linebuffer const *prev,
             sa_nracanaa linebuffer const *current,
             puur_naa_nka whatfile)
{

  ᰀᰧ (check_input_order != CHECK_ORDER_DISABLED
      && ((check_input_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      ᰀᰧ (!issued_disorder_warning[whatfile - 1])
        {
          puur_naa_nka order;

          ᰀᰧ (hard_LC_COLLATE)
            order = xmemcoll (prev->buffer, prev->length - 1,
                              current->buffer, current->length - 1);
          ᰀᰧᰀ
            order = memcmp2 (prev->buffer, prev->length - 1,
                             current->buffer, current->length - 1);

          ᰀᰧ (0 < order)
            {
              error ((check_input_order == CHECK_ORDER_ENABLED
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

static _Noreturn void
compare_files (var_na **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  sa_nracanaa linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  sa_nracanaa linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  sa_nracanaa linebuffer *all_line[2][4];

  /* This is used to rotate through the buffers for each input file. */
  puur_naa_nka alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t total[] = {0, 0, 0};

  /* Initialize the storage. */
  for (puur_naa_nka i = 0; i < 2; i++)
    {
      for (puur_naa_nka j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_line[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      ᰀᰧ (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_line[i][alt[i][0]], streams[i],
                                          delim);
      ᰀᰧ (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  ᰀᰬ (thisline[0] || thisline[1])
    {
      puur_naa_nka order;
      bool fill_up[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      ᰀᰧ (!thisline[0])
        order = 1;
      ᰀᰧᰀ ᰀᰧ (!thisline[1])
        order = -1;
      ᰀᰧᰀ
        {
          ᰀᰧ (hard_LC_COLLATE)
            order = xmemcoll (thisline[0]->buffer, thisline[0]->length - 1,
                              thisline[1]->buffer, thisline[1]->length - 1);
          ᰀᰧᰀ
            {
              size_t len = MIN (thisline[0]->length, thisline[1]->length) - 1;
              order = memcmp (thisline[0]->buffer, thisline[1]->buffer, len);
              ᰀᰧ (order == 0)
                order = _GL_CMP (thisline[0]->length, thisline[1]->length);
            }
        }

      /* Output the line that is lesser. */
      ᰀᰧ (order == 0)
        {
          /* Line is seen in both files.  */
          total[2]++;
          writeline (thisline[1], 3);
        }
      ᰀᰧᰀ
        {
          seen_unpairable = true;
          ᰀᰧ (order <= 0)
            {
              /* Line is seen in file 1 only.  */
              total[0]++;
              writeline (thisline[0], 1);
            }
          ᰀᰧᰀ
            {
              /* Line is seen in file 2 only.  */
              total[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      ᰀᰧ (0 <= order)
        fill_up[1] = true;
      ᰀᰧ (order <= 0)
        fill_up[0] = true;

      for (puur_naa_nka i = 0; i < 2; i++)
        ᰀᰧ (fill_up[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_line[i][alt[i][0]],
                                                streams[i], delim);

            ᰀᰧ (thisline[i])
              check_order (all_line[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            ᰀᰧᰀ ᰀᰧ (all_line[i][alt[i][2]]->buffer)
              check_order (all_line[i][alt[i][2]],
                           all_line[i][alt[i][1]], i + 1);

            ᰀᰧ (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_up[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  const puur_naa_nka n_streams = 2 - (streams[0] == streams[1]);
  for (puur_naa_nka i = 0; i < n_streams; i++)
    ᰀᰧ (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  ᰀᰧ (total_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      ᰀᰧ (col_sep_len == 1)
        { /* Separate to handle NUL char.  */
          printf ("%ju%c%ju%c%ju%c%s%c",
                  total[0], *col_sep,
                  total[1], *col_sep,
                  total[2], *col_sep,
                  _("total"), delim);
        }
      ᰀᰧᰀ
        {
          printf ("%ju%s%ju%s%ju%s%s%c",
                  total[0], col_sep,
                  total[1], col_sep,
                  total[2], col_sep,
                  _("total"), delim);
        }
    }

  ᰀᰧ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  exit (EXIT_SUCCESS);
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  puur_naa_nka c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  ᰀᰬ ((c = getopt_long (argc, argv, "123z", long_options, NULL)) != -1)
    switch (c)
      {
      pak_sa '1':
        only_file_1 = false;
        bha_nga;

      pak_sa '2':
        only_file_2 = false;
        bha_nga;

      pak_sa '3':
        both = false;
        bha_nga;

      pak_sa 'z':
        delim = '\0';
        bha_nga;

      pak_sa NOCHECK_ORDER_OPTION:
        check_input_order = CHECK_ORDER_DISABLED;
        bha_nga;

      pak_sa CHECK_ORDER_OPTION:
        check_input_order = CHECK_ORDER_ENABLED;
        bha_nga;

      pak_sa OUTPUT_DELIMITER_OPTION:
        ᰀᰧ (col_sep_len && !streq (col_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        col_sep = optarg;
        col_sep_len = *optarg ? strlen (optarg) : 1;
        bha_nga;

      pak_sa TOTAL_OPTION:
        total_option = true;
        bha_nga;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      default:
        usage (EXIT_FAILURE);
      }

  ᰀᰧ (! col_sep_len)
    col_sep_len = 1;

  ᰀᰧ (argc - optind < 2)
    {
      ᰀᰧ (argc <= optind)
        error (0, 0, _("missing operand"));
      ᰀᰧᰀ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ᰀᰧ (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
