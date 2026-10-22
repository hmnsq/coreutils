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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "linebuffer.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "hard-locale.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "stdio--.h"
#અન્તર્ભાવ "memcmp2.h"
#અન્તર્ભાવ "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "comm"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
સ્થિર bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
સ્થિર bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
સ્થિર bool only_file_2 = true;

/* If true, print lines that are found in both files. */
સ્થિર bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
સ્થિર bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
સ્થિર bool issued_disorder_warning[2];

/* line delimiter.  */
સ્થિર unsigned અક્ષર delim = '\n';

/* If true, print a summary.  */
સ્થિર bool total_option;

/* If nonzero, check that the input is correctly ordered. */
સ્થિર ગણના
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_આગત_order;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
સ્થિર અક્ષર અચલ *col_sep = "\t";
સ્થિર size_t col_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  OUTPUT_DELIMITER_OPTION,
  TOTAL_OPTION
};

સ્થિર સંરચના option અચલ long_options[] =
{
  {"check-order", no_argument, ખાલી, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, ખાલી, NOCHECK_ORDER_OPTION},
  {"output-delimiter", required_argument, ખાલી, OUTPUT_DELIMITER_OPTION},
  {"total", no_argument, ખાલી, TOTAL_OPTION},
  {"zero-terminated", no_argument, ખાલી, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};


શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 લીટી by લીટી.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, પઠ standard આગત.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-column output.  Column one contains\n\
લીટીઓ unique to FILE1, column two contains લીટીઓ unique to FILE2,\n\
and column three contains લીટીઓ common to both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress column 1 (લીટીઓ unique to FILE1)\n\
"));
      oputs (_("\
  -2     suppress column 2 (લીટીઓ unique to FILE2)\n\
"));
      oputs (_("\
  -3     suppress column 3 (લીટીઓ that appear in both files)\n\
"));
      oputs (_("\
      --check-order\n\
         check that the આગત is correctly sorted,\n\
         even જો all આગત લીટીઓ are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         કરો not check that the આગત is correctly sorted\n\
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
         લીટી delimiter is NUL, not newline\n\
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
  %s -12 file1 file2  Print only લીટીઓ present in both file1 and file2.\n\
  %s -3 file1 file2  Print લીટીઓ in file1 not in file2, and vice versa.\n\
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

સ્થિર શૂન્ય
writeline (સંરચના linebuffer અચલ *લીટી, પૂર્ણાંક class)
{
  પસંદગી (class)
    {
    સ્થિતિ 1:
      જો (!only_file_1)
        પાછા;
      તોડો;

    સ્થિતિ 2:
      જો (!only_file_2)
        પાછા;
      જો (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      તોડો;

    સ્થિતિ 3:
      જો (!both)
        પાછા;
      જો (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      જો (only_file_2)
        fwrite (col_sep, 1, col_sep_len, stdout);
      તોડો;
    }

  fwrite (લીટી->buffer, કદ (અક્ષર), લીટી->length, stdout);

  જો (ferror (stdout))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

સ્થિર શૂન્ય
check_order (સંરચના linebuffer અચલ *prev,
             સંરચના linebuffer અચલ *current,
             પૂર્ણાંક whatfile)
{

  જો (check_આગત_order != CHECK_ORDER_DISABLED
      && ((check_આગત_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      જો (!issued_disorder_warning[whatfile - 1])
        {
          પૂર્ણાંક order;

          જો (hard_LC_COLLATE)
            order = xmemcoll (prev->buffer, prev->length - 1,
                              current->buffer, current->length - 1);
          નહીંતર
            order = memcmp2 (prev->buffer, prev->length - 1,
                             current->buffer, current->length - 1);

          જો (0 < order)
            {
              error ((check_આગત_order == CHECK_ORDER_ENABLED
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

સ્થિર _Noreturn શૂન્ય
compare_files (અક્ષર **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  સંરચના linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  સંરચના linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  સંરચના linebuffer *all_લીટી[2][4];

  /* This is used to rotate through the buffers for each input file. */
  પૂર્ણાંક alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t total[] = {0, 0, 0};

  /* Initialize the storage. */
  માટે (પૂર્ણાંક i = 0; i < 2; i++)
    {
      માટે (પૂર્ણાંક j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_લીટી[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      જો (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_લીટી[i][alt[i][0]], streams[i],
                                          delim);
      જો (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  જ્યારે (thisline[0] || thisline[1])
    {
      પૂર્ણાંક order;
      bool fill_up[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      જો (!thisline[0])
        order = 1;
      નહીંતર જો (!thisline[1])
        order = -1;
      નહીંતર
        {
          જો (hard_LC_COLLATE)
            order = xmemcoll (thisline[0]->buffer, thisline[0]->length - 1,
                              thisline[1]->buffer, thisline[1]->length - 1);
          નહીંતર
            {
              size_t len = MIN (thisline[0]->length, thisline[1]->length) - 1;
              order = memcmp (thisline[0]->buffer, thisline[1]->buffer, len);
              જો (order == 0)
                order = _GL_CMP (thisline[0]->length, thisline[1]->length);
            }
        }

      /* Output the line that is lesser. */
      જો (order == 0)
        {
          /* Line is seen in both files.  */
          total[2]++;
          writeline (thisline[1], 3);
        }
      નહીંતર
        {
          seen_unpairable = true;
          જો (order <= 0)
            {
              /* Line is seen in file 1 only.  */
              total[0]++;
              writeline (thisline[0], 1);
            }
          નહીંતર
            {
              /* Line is seen in file 2 only.  */
              total[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      જો (0 <= order)
        fill_up[1] = true;
      જો (order <= 0)
        fill_up[0] = true;

      માટે (પૂર્ણાંક i = 0; i < 2; i++)
        જો (fill_up[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_લીટી[i][alt[i][0]],
                                                streams[i], delim);

            જો (thisline[i])
              check_order (all_લીટી[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            નહીંતર જો (all_લીટી[i][alt[i][2]]->buffer)
              check_order (all_લીટી[i][alt[i][2]],
                           all_લીટી[i][alt[i][1]], i + 1);

            જો (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_up[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  અચલ પૂર્ણાંક n_streams = 2 - (streams[0] == streams[1]);
  માટે (પૂર્ણાંક i = 0; i < n_streams; i++)
    જો (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  જો (total_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      જો (col_sep_len == 1)
        { /* Separate to handle NUL char.  */
          printf ("%ju%c%ju%c%ju%c%s%c",
                  total[0], *col_sep,
                  total[1], *col_sep,
                  total[2], *col_sep,
                  _("total"), delim);
        }
      નહીંતર
        {
          printf ("%ju%s%ju%s%ju%s%s%c",
                  total[0], col_sep,
                  total[1], col_sep,
                  total[2], col_sep,
                  _("total"), delim);
        }
    }

  જો (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  exit (EXIT_SUCCESS);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  જ્યારે ((c = getopt_long (argc, argv, "123z", long_options, ખાલી)) != -1)
    પસંદગી (c)
      {
      સ્થિતિ '1':
        only_file_1 = false;
        તોડો;

      સ્થિતિ '2':
        only_file_2 = false;
        તોડો;

      સ્થિતિ '3':
        both = false;
        તોડો;

      સ્થિતિ 'z':
        delim = '\0';
        તોડો;

      સ્થિતિ NOCHECK_ORDER_OPTION:
        check_આગત_order = CHECK_ORDER_DISABLED;
        તોડો;

      સ્થિતિ CHECK_ORDER_OPTION:
        check_આગત_order = CHECK_ORDER_ENABLED;
        તોડો;

      સ્થિતિ OUTPUT_DELIMITER_OPTION:
        જો (col_sep_len && !streq (col_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        col_sep = optarg;
        col_sep_len = *optarg ? strlen (optarg) : 1;
        તોડો;

      સ્થિતિ TOTAL_OPTION:
        total_option = true;
        તોડો;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      મૂળભૂત:
        usage (EXIT_FAILURE);
      }

  જો (! col_sep_len)
    col_sep_len = 1;

  જો (argc - optind < 2)
    {
      જો (argc <= optind)
        error (0, 0, _("missing operand"));
      નહીંતર
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  જો (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
