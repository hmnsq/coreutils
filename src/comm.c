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

#అన్తర్భావ <config.h>

#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"
#అన్తర్భావ "linebuffer.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "hard-locale.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "stdio--.h"
#అన్తర్భావ "memcmp2.h"
#అన్తర్భావ "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "comm"

#నిర్వచించు AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
స్థిర bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
స్థిర bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
స్థిర bool only_file_2 = true;

/* If true, print lines that are found in both files. */
స్థిర bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
స్థిర bool చూసిన_unpairable;

/* If nonzero, we have warned about disorder in that file. */
స్థిర bool issued_disorder_warning[2];

/* line delimiter.  */
స్థిర గుర్తులేని అక్షరం delim = '\n';

/* If true, print a summary.  */
స్థిర bool మొత్తం_option;

/* If nonzero, check that the input is correctly ordered. */
స్థిర గణన
  {
    CHECK_క్రమం_DEFAULT,
    CHECK_క్రమం_ENABLED,
    CHECK_క్రమం_DISABLED
  } check_ఆదానం_క్రమం;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
స్థిర అక్షరం స్థిరరాశి *నిలువు_sep = "\t";
స్థిర size_t నిలువు_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  CHECK_క్రమం_OPTION = CHAR_MAX + 1,
  NOCHECK_క్రమం_OPTION,
  OUTPUT_DELIMITER_OPTION,
  మొత్తం_OPTION
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"check-order", no_వాదం, ఖాళీ, CHECK_క్రమం_OPTION},
  {"nocheck-order", no_వాదం, ఖాళీ, NOCHECK_క్రమం_OPTION},
  {"output-delimiter", required_వాదం, ఖాళీ, OUTPUT_DELIMITER_OPTION},
  {"total", no_వాదం, ఖాళీ, మొత్తం_OPTION},
  {"zero-terminated", no_వాదం, ఖాళీ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};


శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 పంక్తి by పంక్తి.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, పఠ standard ఆదానం.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-స్తంభం output.  స్తంభం_2 one కలిగియుంది\n\
పంక్తులు unique గమ్యం FILE1, స్తంభం two కలిగియుంది పంక్తులు unique గమ్యం FILE2,\n\
and స్తంభం three కలిగియుంది పంక్తులు common గమ్యం both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress స్తంభం 1 (పంక్తులు unique గమ్యం FILE1)\n\
"));
      oputs (_("\
  -2     suppress స్తంభం 2 (పంక్తులు unique గమ్యం FILE2)\n\
"));
      oputs (_("\
  -3     suppress స్తంభం 3 (పంక్తులు that appear లోపల both files)\n\
"));
      oputs (_("\
      --check-క్రమం\n\
         check that the ఆదానం is correctly sorted,\n\
         even ఒకవేళ all ఆదానం పంక్తులు are pairable\n\
"));
      oputs (_("\
      --nocheck-క్రమం\n\
         చేయి not check that the ఆదానం is correctly sorted\n\
"));
      oputs (_("\
      --output-delimiter=STR\n\
         separate columns with STR\n\
"));
      oputs (_("\
      --మొత్తం\n\
         output a summary\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         పంక్తి delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Comparisons honor the నియమాలు specified by 'LC_COLLATE'.\n\
"), stdout);
      రాయి (_("\
\n\
Examples:\n\
  %s -12 file1 file2  Print only పంక్తులు present లోపల both file1 and file2.\n\
  %s -3 file1 file2  Print పంక్తులు లోపల file1 not లోపల file2, and vice versa.\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Output the line in linebuffer LINE to stdout
   provided the switches say it should be output.
   CLASS is 1 for a line found only in file 1,
   2 for a line only in file 2, 3 for a line in both. */

స్థిర శూన్య
writeline (నిర్మాణం linebuffer స్థిరరాశి *పంక్తి, పూర్ణాంకం class)
{
  ఎంపిక (class)
    {
    స్థితి 1:
      ఒకవేళ (!only_file_1)
        తిరిగి;
      ఆపు;

    స్థితి 2:
      ఒకవేళ (!only_file_2)
        తిరిగి;
      ఒకవేళ (only_file_1)
        fwrite (నిలువు_sep, 1, నిలువు_sep_len, stdout);
      ఆపు;

    స్థితి 3:
      ఒకవేళ (!both)
        తిరిగి;
      ఒకవేళ (only_file_1)
        fwrite (నిలువు_sep, 1, నిలువు_sep_len, stdout);
      ఒకవేళ (only_file_2)
        fwrite (నిలువు_sep, 1, నిలువు_sep_len, stdout);
      ఆపు;
    }

  fwrite (పంక్తి->బఫరు, పరిమాణం (అక్షరం), పంక్తి->నిడివి, stdout);

  ఒకవేళ (ferror (stdout))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

స్థిర శూన్య
check_క్రమం (నిర్మాణం linebuffer స్థిరరాశి *prev,
             నిర్మాణం linebuffer స్థిరరాశి *current,
             పూర్ణాంకం whatfile)
{

  ఒకవేళ (check_ఆదానం_క్రమం != CHECK_క్రమం_DISABLED
      && ((check_ఆదానం_క్రమం == CHECK_క్రమం_ENABLED) || చూసిన_unpairable))
    {
      ఒకవేళ (!issued_disorder_warning[whatfile - 1])
        {
          పూర్ణాంకం క్రమం;

          ఒకవేళ (hard_LC_COLLATE)
            క్రమం = xmemcoll (prev->బఫరు, prev->నిడివి - 1,
                              current->బఫరు, current->నిడివి - 1);
          లేకపోతే
            క్రమం = memcmp2 (prev->బఫరు, prev->నిడివి - 1,
                             current->బఫరు, current->నిడివి - 1);

          ఒకవేళ (0 < క్రమం)
            {
              error ((check_ఆదానం_క్రమం == CHECK_క్రమం_ENABLED
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

స్థిర _Noreturn శూన్య
compare_files (అక్షరం **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  నిర్మాణం linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  నిర్మాణం linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  నిర్మాణం linebuffer *all_పంక్తి[2][4];

  /* This is used to rotate through the buffers for each input file. */
  పూర్ణాంకం alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t మొత్తం[] = {0, 0, 0};

  /* Initialize the storage. */
  కొరకు (పూర్ణాంకం i = 0; i < 2; i++)
    {
      కొరకు (పూర్ణాంకం j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_పంక్తి[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      ఒకవేళ (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_పంక్తి[i][alt[i][0]], streams[i],
                                          delim);
      ఒకవేళ (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  అయితే (thisline[0] || thisline[1])
    {
      పూర్ణాంకం క్రమం;
      bool fill_పైకి[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      ఒకవేళ (!thisline[0])
        క్రమం = 1;
      లేకపోతే ఒకవేళ (!thisline[1])
        క్రమం = -1;
      లేకపోతే
        {
          ఒకవేళ (hard_LC_COLLATE)
            క్రమం = xmemcoll (thisline[0]->బఫరు, thisline[0]->నిడివి - 1,
                              thisline[1]->బఫరు, thisline[1]->నిడివి - 1);
          లేకపోతే
            {
              size_t len = MIN (thisline[0]->నిడివి, thisline[1]->నిడివి) - 1;
              క్రమం = memcmp (thisline[0]->బఫరు, thisline[1]->బఫరు, len);
              ఒకవేళ (క్రమం == 0)
                క్రమం = _GL_CMP (thisline[0]->నిడివి, thisline[1]->నిడివి);
            }
        }

      /* Output the line that is lesser. */
      ఒకవేళ (క్రమం == 0)
        {
          /* Line is seen in both files.  */
          మొత్తం[2]++;
          writeline (thisline[1], 3);
        }
      లేకపోతే
        {
          చూసిన_unpairable = true;
          ఒకవేళ (క్రమం <= 0)
            {
              /* Line is seen in file 1 only.  */
              మొత్తం[0]++;
              writeline (thisline[0], 1);
            }
          లేకపోతే
            {
              /* Line is seen in file 2 only.  */
              మొత్తం[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      ఒకవేళ (0 <= క్రమం)
        fill_పైకి[1] = true;
      ఒకవేళ (క్రమం <= 0)
        fill_పైకి[0] = true;

      కొరకు (పూర్ణాంకం i = 0; i < 2; i++)
        ఒకవేళ (fill_పైకి[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_పంక్తి[i][alt[i][0]],
                                                streams[i], delim);

            ఒకవేళ (thisline[i])
              check_క్రమం (all_పంక్తి[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            లేకపోతే ఒకవేళ (all_పంక్తి[i][alt[i][2]]->బఫరు)
              check_క్రమం (all_పంక్తి[i][alt[i][2]],
                           all_పంక్తి[i][alt[i][1]], i + 1);

            ఒకవేళ (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_పైకి[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  స్థిరరాశి పూర్ణాంకం n_streams = 2 - (streams[0] == streams[1]);
  కొరకు (పూర్ణాంకం i = 0; i < n_streams; i++)
    ఒకవేళ (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  ఒకవేళ (మొత్తం_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      ఒకవేళ (నిలువు_sep_len == 1)
        { /* Separate to handle NUL char.  */
          రాయి ("%ju%c%ju%c%ju%c%s%c",
                  మొత్తం[0], *నిలువు_sep,
                  మొత్తం[1], *నిలువు_sep,
                  మొత్తం[2], *నిలువు_sep,
                  _("total"), delim);
        }
      లేకపోతే
        {
          రాయి ("%ju%s%ju%s%ju%s%s%c",
                  మొత్తం[0], నిలువు_sep,
                  మొత్తం[1], నిలువు_sep,
                  మొత్తం[2], నిలువు_sep,
                  _("total"), delim);
        }
    }

  ఒకవేళ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  నిష్క్రమించు (EXIT_SUCCESS);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం c;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  అయితే ((c = getopt_long (argc, argv, "123z", long_options, ఖాళీ)) != -1)
    ఎంపిక (c)
      {
      స్థితి '1':
        only_file_1 = false;
        ఆపు;

      స్థితి '2':
        only_file_2 = false;
        ఆపు;

      స్థితి '3':
        both = false;
        ఆపు;

      స్థితి 'z':
        delim = '\0';
        ఆపు;

      స్థితి NOCHECK_క్రమం_OPTION:
        check_ఆదానం_క్రమం = CHECK_క్రమం_DISABLED;
        ఆపు;

      స్థితి CHECK_క్రమం_OPTION:
        check_ఆదానం_క్రమం = CHECK_క్రమం_ENABLED;
        ఆపు;

      స్థితి OUTPUT_DELIMITER_OPTION:
        ఒకవేళ (నిలువు_sep_len && !streq (నిలువు_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        నిలువు_sep = optarg;
        నిలువు_sep_len = *optarg ? అక్షరశ్రేణి_పొడవు (optarg) : 1;
        ఆపు;

      స్థితి మొత్తం_OPTION:
        మొత్తం_option = true;
        ఆపు;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

      అప్రమేయ:
        usage (EXIT_FAILURE);
      }

  ఒకవేళ (! నిలువు_sep_len)
    నిలువు_sep_len = 1;

  ఒకవేళ (argc - optind < 2)
    {
      ఒకవేళ (argc <= optind)
        error (0, 0, _("missing operand"));
      లేకపోతే
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
