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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "linebuffer.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "hard-locale.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "stdio--.h"
#അന്തര്ഭാവ "memcmp2.h"
#അന്തര്ഭാവ "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "comm"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
സ്ഥിതം bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
സ്ഥിതം bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
സ്ഥിതം bool only_file_2 = true;

/* If true, print lines that are found in both files. */
സ്ഥിതം bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
സ്ഥിതം bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
സ്ഥിതം bool issued_disorder_warning[2];

/* line delimiter.  */
സ്ഥിതം unsigned അക്ഷരം delim = '\n';

/* If true, print a summary.  */
സ്ഥിതം bool total_option;

/* If nonzero, check that the input is correctly ordered. */
സ്ഥിതം എണ്ണൽ
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_നിവേശം_order;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *col_sep = "\t";
സ്ഥിതം size_t col_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  OUTPUT_DELIMITER_OPTION,
  TOTAL_OPTION
};

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"check-order", no_argument, ശൂന്യ, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, ശൂന്യ, NOCHECK_ORDER_OPTION},
  {"output-delimiter", required_argument, ശൂന്യ, OUTPUT_DELIMITER_OPTION},
  {"total", no_argument, ശൂന്യ, TOTAL_OPTION},
  {"zero-terminated", no_argument, ശൂന്യ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};


ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 വരി by വരി.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, പഠ standard നിവേശം.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-column output.  Column one contains\n\
വരികൾ unique to FILE1, column two contains വരികൾ unique to FILE2,\n\
and column three contains വരികൾ common to both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress column 1 (വരികൾ unique to FILE1)\n\
"));
      oputs (_("\
  -2     suppress column 2 (വരികൾ unique to FILE2)\n\
"));
      oputs (_("\
  -3     suppress column 3 (വരികൾ that appear in both files)\n\
"));
      oputs (_("\
      --check-order\n\
         check that the നിവേശം is correctly sorted,\n\
         even എങ്കിൽ all നിവേശം വരികൾ are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         ചെയ്യുക not check that the നിവേശം is correctly sorted\n\
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
         വരി delimiter is NUL, not newline\n\
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
  %s -12 file1 file2  Print only വരികൾ present in both file1 and file2.\n\
  %s -3 file1 file2  Print വരികൾ in file1 not in file2, and vice versa.\n\
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

സ്ഥിതം ശൂന്യം
writeline (ഘടന linebuffer സ്ഥിരം *വരി, പൂർണ്ണസംഖ്യ class)
{
  തിരഞ്ഞെടുപ്പ് (class)
    {
    അവസ്ഥ 1:
      എങ്കിൽ (!only_file_1)
        തിരികെ;
      നിർത്തുക;

    അവസ്ഥ 2:
      എങ്കിൽ (!only_file_2)
        തിരികെ;
      എങ്കിൽ (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      നിർത്തുക;

    അവസ്ഥ 3:
      എങ്കിൽ (!both)
        തിരികെ;
      എങ്കിൽ (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      എങ്കിൽ (only_file_2)
        fwrite (col_sep, 1, col_sep_len, stdout);
      നിർത്തുക;
    }

  fwrite (വരി->buffer, വലുപ്പം (അക്ഷരം), വരി->length, stdout);

  എങ്കിൽ (ferror (stdout))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

സ്ഥിതം ശൂന്യം
check_order (ഘടന linebuffer സ്ഥിരം *prev,
             ഘടന linebuffer സ്ഥിരം *current,
             പൂർണ്ണസംഖ്യ whatfile)
{

  എങ്കിൽ (check_നിവേശം_order != CHECK_ORDER_DISABLED
      && ((check_നിവേശം_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      എങ്കിൽ (!issued_disorder_warning[whatfile - 1])
        {
          പൂർണ്ണസംഖ്യ order;

          എങ്കിൽ (hard_LC_COLLATE)
            order = xmemcoll (prev->buffer, prev->length - 1,
                              current->buffer, current->length - 1);
          അല്ലെങ്കിൽ
            order = memcmp2 (prev->buffer, prev->length - 1,
                             current->buffer, current->length - 1);

          എങ്കിൽ (0 < order)
            {
              error ((check_നിവേശം_order == CHECK_ORDER_ENABLED
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

സ്ഥിതം _Noreturn ശൂന്യം
compare_files (അക്ഷരം **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  ഘടന linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  ഘടന linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  ഘടന linebuffer *all_വരി[2][4];

  /* This is used to rotate through the buffers for each input file. */
  പൂർണ്ണസംഖ്യ alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t total[] = {0, 0, 0};

  /* Initialize the storage. */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 2; i++)
    {
      വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_വരി[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      എങ്കിൽ (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_വരി[i][alt[i][0]], streams[i],
                                          delim);
      എങ്കിൽ (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  ആകുമ്പോൾ (thisline[0] || thisline[1])
    {
      പൂർണ്ണസംഖ്യ order;
      bool fill_up[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      എങ്കിൽ (!thisline[0])
        order = 1;
      അല്ലെങ്കിൽ എങ്കിൽ (!thisline[1])
        order = -1;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (hard_LC_COLLATE)
            order = xmemcoll (thisline[0]->buffer, thisline[0]->length - 1,
                              thisline[1]->buffer, thisline[1]->length - 1);
          അല്ലെങ്കിൽ
            {
              size_t len = MIN (thisline[0]->length, thisline[1]->length) - 1;
              order = memcmp (thisline[0]->buffer, thisline[1]->buffer, len);
              എങ്കിൽ (order == 0)
                order = _GL_CMP (thisline[0]->length, thisline[1]->length);
            }
        }

      /* Output the line that is lesser. */
      എങ്കിൽ (order == 0)
        {
          /* Line is seen in both files.  */
          total[2]++;
          writeline (thisline[1], 3);
        }
      അല്ലെങ്കിൽ
        {
          seen_unpairable = true;
          എങ്കിൽ (order <= 0)
            {
              /* Line is seen in file 1 only.  */
              total[0]++;
              writeline (thisline[0], 1);
            }
          അല്ലെങ്കിൽ
            {
              /* Line is seen in file 2 only.  */
              total[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      എങ്കിൽ (0 <= order)
        fill_up[1] = true;
      എങ്കിൽ (order <= 0)
        fill_up[0] = true;

      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 2; i++)
        എങ്കിൽ (fill_up[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_വരി[i][alt[i][0]],
                                                streams[i], delim);

            എങ്കിൽ (thisline[i])
              check_order (all_വരി[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            അല്ലെങ്കിൽ എങ്കിൽ (all_വരി[i][alt[i][2]]->buffer)
              check_order (all_വരി[i][alt[i][2]],
                           all_വരി[i][alt[i][1]], i + 1);

            എങ്കിൽ (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_up[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  സ്ഥിരം പൂർണ്ണസംഖ്യ n_streams = 2 - (streams[0] == streams[1]);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_streams; i++)
    എങ്കിൽ (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  എങ്കിൽ (total_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      എങ്കിൽ (col_sep_len == 1)
        { /* Separate to handle NUL char.  */
          printf ("%ju%c%ju%c%ju%c%s%c",
                  total[0], *col_sep,
                  total[1], *col_sep,
                  total[2], *col_sep,
                  _("total"), delim);
        }
      അല്ലെങ്കിൽ
        {
          printf ("%ju%s%ju%s%ju%s%s%c",
                  total[0], col_sep,
                  total[1], col_sep,
                  total[2], col_sep,
                  _("total"), delim);
        }
    }

  എങ്കിൽ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  exit (EXIT_SUCCESS);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "123z", long_options, ശൂന്യ)) != -1)
    തിരഞ്ഞെടുപ്പ് (c)
      {
      അവസ്ഥ '1':
        only_file_1 = false;
        നിർത്തുക;

      അവസ്ഥ '2':
        only_file_2 = false;
        നിർത്തുക;

      അവസ്ഥ '3':
        both = false;
        നിർത്തുക;

      അവസ്ഥ 'z':
        delim = '\0';
        നിർത്തുക;

      അവസ്ഥ NOCHECK_ORDER_OPTION:
        check_നിവേശം_order = CHECK_ORDER_DISABLED;
        നിർത്തുക;

      അവസ്ഥ CHECK_ORDER_OPTION:
        check_നിവേശം_order = CHECK_ORDER_ENABLED;
        നിർത്തുക;

      അവസ്ഥ OUTPUT_DELIMITER_OPTION:
        എങ്കിൽ (col_sep_len && !streq (col_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        col_sep = optarg;
        col_sep_len = *optarg ? strlen (optarg) : 1;
        നിർത്തുക;

      അവസ്ഥ TOTAL_OPTION:
        total_option = true;
        നിർത്തുക;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      സ്ഥിരസ്ഥിതി:
        usage (EXIT_FAILURE);
      }

  എങ്കിൽ (! col_sep_len)
    col_sep_len = 1;

  എങ്കിൽ (argc - optind < 2)
    {
      എങ്കിൽ (argc <= optind)
        error (0, 0, _("missing operand"));
      അല്ലെങ്കിൽ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
