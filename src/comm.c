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

#شامل <config.h>

#شامل <getopt.h>
#شامل <sys/types.h>
#شامل "system.h"
#شامل "linebuffer.h"
#شامل "fadvise.h"
#شامل "hard-locale.h"
#شامل "quote.h"
#شامل "stdio--.h"
#شامل "memcmp2.h"
#شامل "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "comm"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
ایستا bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
ایستا bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
ایستا bool only_file_2 = true;

/* If true, print lines that are found in both files. */
ایستا bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
ایستا bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ایستا bool issued_disorder_warning[2];

/* line delimiter.  */
ایستا unsigned نویسه delim = '\n';

/* If true, print a summary.  */
ایستا bool total_option;

/* If nonzero, check that the input is correctly ordered. */
ایستا شمارش
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_درون_داد_order;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
ایستا نویسه ثابت *col_sep = "\t";
ایستا size_t col_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  OUTPUT_DELIMITER_OPTION,
  TOTAL_OPTION
};

ایستا ساختار option ثابت long_options[] =
{
  {"check-order", no_argument, پوچ, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, پوچ, NOCHECK_ORDER_OPTION},
  {"output-delimiter", required_argument, پوچ, OUTPUT_DELIMITER_OPTION},
  {"total", no_argument, پوچ, TOTAL_OPTION},
  {"zero-terminated", no_argument, پوچ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};


تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 سطر by سطر.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, اقرأ standard درون_داد.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-column output.  Column one contains\n\
سطرها unique to FILE1, column two contains سطرها unique to FILE2,\n\
and column three contains سطرها common to both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress column 1 (سطرها unique to FILE1)\n\
"));
      oputs (_("\
  -2     suppress column 2 (سطرها unique to FILE2)\n\
"));
      oputs (_("\
  -3     suppress column 3 (سطرها that appear in both files)\n\
"));
      oputs (_("\
      --check-order\n\
         check that the درون_داد is correctly sorted,\n\
         even اگر all درون_داد سطرها are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         انجامبده not check that the درون_داد is correctly sorted\n\
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
         سطر delimiter is NUL, not newline\n\
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
  %s -12 file1 file2  Print only سطرها present in both file1 and file2.\n\
  %s -3 file1 file2  Print سطرها in file1 not in file2, and vice versa.\n\
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

ایستا تهی
writeline (ساختار linebuffer ثابت *سطر, صحیح class)
{
  انتخاب (class)
    {
    حالت 1:
      اگر (!only_file_1)
        بازگشت;
      بشکن;

    حالت 2:
      اگر (!only_file_2)
        بازگشت;
      اگر (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      بشکن;

    حالت 3:
      اگر (!both)
        بازگشت;
      اگر (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      اگر (only_file_2)
        fwrite (col_sep, 1, col_sep_len, stdout);
      بشکن;
    }

  fwrite (سطر->buffer, اندازه (نویسه), سطر->length, stdout);

  اگر (ferror (stdout))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

ایستا تهی
check_order (ساختار linebuffer ثابت *prev,
             ساختار linebuffer ثابت *current,
             صحیح whatfile)
{

  اگر (check_درون_داد_order != CHECK_ORDER_DISABLED
      && ((check_درون_داد_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      اگر (!issued_disorder_warning[whatfile - 1])
        {
          صحیح order;

          اگر (hard_LC_COLLATE)
            order = xmemcoll (prev->buffer, prev->length - 1,
                              current->buffer, current->length - 1);
          وگرنه
            order = memcmp2 (prev->buffer, prev->length - 1,
                             current->buffer, current->length - 1);

          اگر (0 < order)
            {
              error ((check_درون_داد_order == CHECK_ORDER_ENABLED
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

ایستا _Noreturn تهی
compare_files (نویسه **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  ساختار linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  ساختار linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  ساختار linebuffer *all_سطر[2][4];

  /* This is used to rotate through the buffers for each input file. */
  صحیح alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t total[] = {0, 0, 0};

  /* Initialize the storage. */
  برای (صحیح i = 0; i < 2; i++)
    {
      برای (صحیح j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_سطر[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      اگر (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_سطر[i][alt[i][0]], streams[i],
                                          delim);
      اگر (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  وقتی (thisline[0] || thisline[1])
    {
      صحیح order;
      bool fill_up[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      اگر (!thisline[0])
        order = 1;
      وگرنه اگر (!thisline[1])
        order = -1;
      وگرنه
        {
          اگر (hard_LC_COLLATE)
            order = xmemcoll (thisline[0]->buffer, thisline[0]->length - 1,
                              thisline[1]->buffer, thisline[1]->length - 1);
          وگرنه
            {
              size_t len = MIN (thisline[0]->length, thisline[1]->length) - 1;
              order = memcmp (thisline[0]->buffer, thisline[1]->buffer, len);
              اگر (order == 0)
                order = _GL_CMP (thisline[0]->length, thisline[1]->length);
            }
        }

      /* Output the line that is lesser. */
      اگر (order == 0)
        {
          /* Line is seen in both files.  */
          total[2]++;
          writeline (thisline[1], 3);
        }
      وگرنه
        {
          seen_unpairable = true;
          اگر (order <= 0)
            {
              /* Line is seen in file 1 only.  */
              total[0]++;
              writeline (thisline[0], 1);
            }
          وگرنه
            {
              /* Line is seen in file 2 only.  */
              total[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      اگر (0 <= order)
        fill_up[1] = true;
      اگر (order <= 0)
        fill_up[0] = true;

      برای (صحیح i = 0; i < 2; i++)
        اگر (fill_up[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_سطر[i][alt[i][0]],
                                                streams[i], delim);

            اگر (thisline[i])
              check_order (all_سطر[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            وگرنه اگر (all_سطر[i][alt[i][2]]->buffer)
              check_order (all_سطر[i][alt[i][2]],
                           all_سطر[i][alt[i][1]], i + 1);

            اگر (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_up[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  ثابت صحیح n_streams = 2 - (streams[0] == streams[1]);
  برای (صحیح i = 0; i < n_streams; i++)
    اگر (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  اگر (total_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      اگر (col_sep_len == 1)
        { /* Separate to handle NUL char.  */
          printf ("%ju%c%ju%c%ju%c%s%c",
                  total[0], *col_sep,
                  total[1], *col_sep,
                  total[2], *col_sep,
                  _("total"), delim);
        }
      وگرنه
        {
          printf ("%ju%s%ju%s%ju%s%s%c",
                  total[0], col_sep,
                  total[1], col_sep,
                  total[2], col_sep,
                  _("total"), delim);
        }
    }

  اگر (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  exit (EXIT_SUCCESS);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  وقتی ((c = getopt_long (argc, argv, "123z", long_options, پوچ)) != -1)
    انتخاب (c)
      {
      حالت '1':
        only_file_1 = false;
        بشکن;

      حالت '2':
        only_file_2 = false;
        بشکن;

      حالت '3':
        both = false;
        بشکن;

      حالت 'z':
        delim = '\0';
        بشکن;

      حالت NOCHECK_ORDER_OPTION:
        check_درون_داد_order = CHECK_ORDER_DISABLED;
        بشکن;

      حالت CHECK_ORDER_OPTION:
        check_درون_داد_order = CHECK_ORDER_ENABLED;
        بشکن;

      حالت OUTPUT_DELIMITER_OPTION:
        اگر (col_sep_len && !streq (col_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        col_sep = optarg;
        col_sep_len = *optarg ? strlen (optarg) : 1;
        بشکن;

      حالت TOTAL_OPTION:
        total_option = true;
        بشکن;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      پیشفرض:
        usage (EXIT_FAILURE);
      }

  اگر (! col_sep_len)
    col_sep_len = 1;

  اگر (argc - optind < 2)
    {
      اگر (argc <= optind)
        error (0, 0, _("missing operand"));
      وگرنه
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  اگر (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
