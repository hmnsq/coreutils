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
স্থির unsigned বর্ণ delim = '\n';

/* If true, print a summary.  */
স্থির bool total_option;

/* If nonzero, check that the input is correctly ordered. */
স্থির গণনা
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_প্রবেশ_order;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
স্থির বর্ণ ধ্রুব *col_sep = "\t";
স্থির size_t col_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  OUTPUT_DELIMITER_OPTION,
  TOTAL_OPTION
};

স্থির সংরচনা option ধ্রুব long_options[] =
{
  {"check-order", no_argument, রিক্ত, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, রিক্ত, NOCHECK_ORDER_OPTION},
  {"output-delimiter", required_argument, রিক্ত, OUTPUT_DELIMITER_OPTION},
  {"total", no_argument, রিক্ত, TOTAL_OPTION},
  {"zero-terminated", no_argument, রিক্ত, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};


শূন্য
usage (পূর্ণাঙ্ক status)
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
         কুরু not check that the প্রবেশ is correctly sorted\n\
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
writeline (সংরচনা linebuffer ধ্রুব *সারি, পূর্ণাঙ্ক class)
{
  বিকল্প (class)
    {
    পক্ষ 1:
      যদি (!only_file_1)
        প্রত্যাগম;
      ভঙ্গ;

    পক্ষ 2:
      যদি (!only_file_2)
        প্রত্যাগম;
      যদি (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      ভঙ্গ;

    পক্ষ 3:
      যদি (!both)
        প্রত্যাগম;
      যদি (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      যদি (only_file_2)
        fwrite (col_sep, 1, col_sep_len, stdout);
      ভঙ্গ;
    }

  fwrite (সারি->buffer, পরিমাণ (বর্ণ), সারি->length, stdout);

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
check_order (সংরচনা linebuffer ধ্রুব *prev,
             সংরচনা linebuffer ধ্রুব *current,
             পূর্ণাঙ্ক whatfile)
{

  যদি (check_প্রবেশ_order != CHECK_ORDER_DISABLED
      && ((check_প্রবেশ_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      যদি (!issued_disorder_warning[whatfile - 1])
        {
          পূর্ণাঙ্ক order;

          যদি (hard_LC_COLLATE)
            order = xmemcoll (prev->buffer, prev->length - 1,
                              current->buffer, current->length - 1);
          অন্যথা
            order = memcmp2 (prev->buffer, prev->length - 1,
                             current->buffer, current->length - 1);

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
compare_files (বর্ণ **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  সংরচনা linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  সংরচনা linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  সংরচনা linebuffer *all_সারি[2][4];

  /* This is used to rotate through the buffers for each input file. */
  পূর্ণাঙ্ক alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t total[] = {0, 0, 0};

  /* Initialize the storage. */
  প্রতি (পূর্ণাঙ্ক i = 0; i < 2; i++)
    {
      প্রতি (পূর্ণাঙ্ক j = 0; j < 4; j++)
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

  যাবত্ (thisline[0] || thisline[1])
    {
      পূর্ণাঙ্ক order;
      bool fill_up[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      যদি (!thisline[0])
        order = 1;
      অন্যথা যদি (!thisline[1])
        order = -1;
      অন্যথা
        {
          যদি (hard_LC_COLLATE)
            order = xmemcoll (thisline[0]->buffer, thisline[0]->length - 1,
                              thisline[1]->buffer, thisline[1]->length - 1);
          অন্যথা
            {
              size_t len = MIN (thisline[0]->length, thisline[1]->length) - 1;
              order = memcmp (thisline[0]->buffer, thisline[1]->buffer, len);
              যদি (order == 0)
                order = _GL_CMP (thisline[0]->length, thisline[1]->length);
            }
        }

      /* Output the line that is lesser. */
      যদি (order == 0)
        {
          /* Line is seen in both files.  */
          total[2]++;
          writeline (thisline[1], 3);
        }
      অন্যথা
        {
          seen_unpairable = true;
          যদি (order <= 0)
            {
              /* Line is seen in file 1 only.  */
              total[0]++;
              writeline (thisline[0], 1);
            }
          অন্যথা
            {
              /* Line is seen in file 2 only.  */
              total[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      যদি (0 <= order)
        fill_up[1] = true;
      যদি (order <= 0)
        fill_up[0] = true;

      প্রতি (পূর্ণাঙ্ক i = 0; i < 2; i++)
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
            অন্যথা যদি (all_সারি[i][alt[i][2]]->buffer)
              check_order (all_সারি[i][alt[i][2]],
                           all_সারি[i][alt[i][1]], i + 1);

            যদি (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_up[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  ধ্রুব পূর্ণাঙ্ক n_streams = 2 - (streams[0] == streams[1]);
  প্রতি (পূর্ণাঙ্ক i = 0; i < n_streams; i++)
    যদি (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  যদি (total_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      যদি (col_sep_len == 1)
        { /* Separate to handle NUL char.  */
          printf ("%ju%c%ju%c%ju%c%s%c",
                  total[0], *col_sep,
                  total[1], *col_sep,
                  total[2], *col_sep,
                  _("total"), delim);
        }
      অন্যথা
        {
          printf ("%ju%s%ju%s%ju%s%s%c",
                  total[0], col_sep,
                  total[1], col_sep,
                  total[2], col_sep,
                  _("total"), delim);
        }
    }

  যদি (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  exit (EXIT_SUCCESS);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  পূর্ণাঙ্ক c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  যাবত্ ((c = getopt_long (argc, argv, "123z", long_options, রিক্ত)) != -1)
    বিকল্প (c)
      {
      পক্ষ '1':
        only_file_1 = false;
        ভঙ্গ;

      পক্ষ '2':
        only_file_2 = false;
        ভঙ্গ;

      পক্ষ '3':
        both = false;
        ভঙ্গ;

      পক্ষ 'z':
        delim = '\0';
        ভঙ্গ;

      পক্ষ NOCHECK_ORDER_OPTION:
        check_প্রবেশ_order = CHECK_ORDER_DISABLED;
        ভঙ্গ;

      পক্ষ CHECK_ORDER_OPTION:
        check_প্রবেশ_order = CHECK_ORDER_ENABLED;
        ভঙ্গ;

      পক্ষ OUTPUT_DELIMITER_OPTION:
        যদি (col_sep_len && !streq (col_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        col_sep = optarg;
        col_sep_len = *optarg ? strlen (optarg) : 1;
        ভঙ্গ;

      পক্ষ TOTAL_OPTION:
        total_option = true;
        ভঙ্গ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      স্বভাব:
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
