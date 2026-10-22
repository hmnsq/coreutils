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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "linebuffer.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "hard-locale.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "stdio--.h"
#ଅନ୍ତର୍ଭାଵ "memcmp2.h"
#ଅନ୍ତର୍ଭାଵ "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "comm"

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
ସ୍ଥିର bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
ସ୍ଥିର bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
ସ୍ଥିର bool only_file_2 = true;

/* If true, print lines that are found in both files. */
ସ୍ଥିର bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
ସ୍ଥିର bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ସ୍ଥିର bool issued_disorder_warning[2];

/* line delimiter.  */
ସ୍ଥିର unsigned ଅକ୍ଷର delim = '\n';

/* If true, print a summary.  */
ସ୍ଥିର bool total_option;

/* If nonzero, check that the input is correctly ordered. */
ସ୍ଥିର ଗଣନା
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_ନିବେଶ_order;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *col_sep = "\t";
ସ୍ଥିର size_t col_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  OUTPUT_DELIMITER_OPTION,
  TOTAL_OPTION
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"check-order", no_argument, ଖାଲି, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, ଖାଲି, NOCHECK_ORDER_OPTION},
  {"output-delimiter", required_argument, ଖାଲି, OUTPUT_DELIMITER_OPTION},
  {"total", no_argument, ଖାଲି, TOTAL_OPTION},
  {"zero-terminated", no_argument, ଖାଲି, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};


ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 ଧାଡ଼ି by ଧାଡ଼ି.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, ପଠ standard ନିବେଶ.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-column output.  Column one contains\n\
ଧାଡ଼ିଗୁଡ଼ିକ unique to FILE1, column two contains ଧାଡ଼ିଗୁଡ଼ିକ unique to FILE2,\n\
and column three contains ଧାଡ଼ିଗୁଡ଼ିକ common to both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress column 1 (ଧାଡ଼ିଗୁଡ଼ିକ unique to FILE1)\n\
"));
      oputs (_("\
  -2     suppress column 2 (ଧାଡ଼ିଗୁଡ଼ିକ unique to FILE2)\n\
"));
      oputs (_("\
  -3     suppress column 3 (ଧାଡ଼ିଗୁଡ଼ିକ that appear in both files)\n\
"));
      oputs (_("\
      --check-order\n\
         check that the ନିବେଶ is correctly sorted,\n\
         even ଯଦି all ନିବେଶ ଧାଡ଼ିଗୁଡ଼ିକ are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         କର not check that the ନିବେଶ is correctly sorted\n\
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
         ଧାଡ଼ି delimiter is NUL, not newline\n\
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
  %s -12 file1 file2  Print only ଧାଡ଼ିଗୁଡ଼ିକ present in both file1 and file2.\n\
  %s -3 file1 file2  Print ଧାଡ଼ିଗୁଡ଼ିକ in file1 not in file2, and vice versa.\n\
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

ସ୍ଥିର ଶୂନ୍ୟ
writeline (ସଂରଚନା linebuffer ଅଚଳ *ଧାଡ଼ି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା class)
{
  ବାଛ (class)
    {
    ସ୍ଥିତି 1:
      ଯଦି (!only_file_1)
        ଫେରନ୍ତୁ;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 2:
      ଯଦି (!only_file_2)
        ଫେରନ୍ତୁ;
      ଯଦି (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 3:
      ଯଦି (!both)
        ଫେରନ୍ତୁ;
      ଯଦି (only_file_1)
        fwrite (col_sep, 1, col_sep_len, stdout);
      ଯଦି (only_file_2)
        fwrite (col_sep, 1, col_sep_len, stdout);
      ଭାଙ୍ଗ;
    }

  fwrite (ଧାଡ଼ି->buffer, ଆକାର (ଅକ୍ଷର), ଧାଡ଼ି->length, stdout);

  ଯଦି (ferror (stdout))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

ସ୍ଥିର ଶୂନ୍ୟ
check_order (ସଂରଚନା linebuffer ଅଚଳ *prev,
             ସଂରଚନା linebuffer ଅଚଳ *current,
             ପୂର୍ଣ୍ଣସଂଖ୍ୟା whatfile)
{

  ଯଦି (check_ନିବେଶ_order != CHECK_ORDER_DISABLED
      && ((check_ନିବେଶ_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      ଯଦି (!issued_disorder_warning[whatfile - 1])
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା order;

          ଯଦି (hard_LC_COLLATE)
            order = xmemcoll (prev->buffer, prev->length - 1,
                              current->buffer, current->length - 1);
          ଅନ୍ୟଥା
            order = memcmp2 (prev->buffer, prev->length - 1,
                             current->buffer, current->length - 1);

          ଯଦି (0 < order)
            {
              error ((check_ନିବେଶ_order == CHECK_ORDER_ENABLED
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

ସ୍ଥିର _Noreturn ଶୂନ୍ୟ
compare_files (ଅକ୍ଷର **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  ସଂରଚନା linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  ସଂରଚନା linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  ସଂରଚନା linebuffer *all_ଧାଡ଼ି[2][4];

  /* This is used to rotate through the buffers for each input file. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t total[] = {0, 0, 0};

  /* Initialize the storage. */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 2; i++)
    {
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_ଧାଡ଼ି[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      ଯଦି (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_ଧାଡ଼ି[i][alt[i][0]], streams[i],
                                          delim);
      ଯଦି (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  ଯେତେବେଳେ (thisline[0] || thisline[1])
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା order;
      bool fill_up[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      ଯଦି (!thisline[0])
        order = 1;
      ଅନ୍ୟଥା ଯଦି (!thisline[1])
        order = -1;
      ଅନ୍ୟଥା
        {
          ଯଦି (hard_LC_COLLATE)
            order = xmemcoll (thisline[0]->buffer, thisline[0]->length - 1,
                              thisline[1]->buffer, thisline[1]->length - 1);
          ଅନ୍ୟଥା
            {
              size_t len = MIN (thisline[0]->length, thisline[1]->length) - 1;
              order = memcmp (thisline[0]->buffer, thisline[1]->buffer, len);
              ଯଦି (order == 0)
                order = _GL_CMP (thisline[0]->length, thisline[1]->length);
            }
        }

      /* Output the line that is lesser. */
      ଯଦି (order == 0)
        {
          /* Line is seen in both files.  */
          total[2]++;
          writeline (thisline[1], 3);
        }
      ଅନ୍ୟଥା
        {
          seen_unpairable = true;
          ଯଦି (order <= 0)
            {
              /* Line is seen in file 1 only.  */
              total[0]++;
              writeline (thisline[0], 1);
            }
          ଅନ୍ୟଥା
            {
              /* Line is seen in file 2 only.  */
              total[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      ଯଦି (0 <= order)
        fill_up[1] = true;
      ଯଦି (order <= 0)
        fill_up[0] = true;

      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 2; i++)
        ଯଦି (fill_up[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_ଧାଡ଼ି[i][alt[i][0]],
                                                streams[i], delim);

            ଯଦି (thisline[i])
              check_order (all_ଧାଡ଼ି[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            ଅନ୍ୟଥା ଯଦି (all_ଧାଡ଼ି[i][alt[i][2]]->buffer)
              check_order (all_ଧାଡ଼ି[i][alt[i][2]],
                           all_ଧାଡ଼ି[i][alt[i][1]], i + 1);

            ଯଦି (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_up[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_streams = 2 - (streams[0] == streams[1]);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_streams; i++)
    ଯଦି (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  ଯଦି (total_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      ଯଦି (col_sep_len == 1)
        { /* Separate to handle NUL char.  */
          printf ("%ju%c%ju%c%ju%c%s%c",
                  total[0], *col_sep,
                  total[1], *col_sep,
                  total[2], *col_sep,
                  _("total"), delim);
        }
      ଅନ୍ୟଥା
        {
          printf ("%ju%s%ju%s%ju%s%s%c",
                  total[0], col_sep,
                  total[1], col_sep,
                  total[2], col_sep,
                  _("total"), delim);
        }
    }

  ଯଦି (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  exit (EXIT_SUCCESS);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "123z", long_options, ଖାଲି)) != -1)
    ବାଛ (c)
      {
      ସ୍ଥିତି '1':
        only_file_1 = false;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି '2':
        only_file_2 = false;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି '3':
        both = false;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'z':
        delim = '\0';
        ଭାଙ୍ଗ;

      ସ୍ଥିତି NOCHECK_ORDER_OPTION:
        check_ନିବେଶ_order = CHECK_ORDER_DISABLED;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି CHECK_ORDER_OPTION:
        check_ନିବେଶ_order = CHECK_ORDER_ENABLED;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି OUTPUT_DELIMITER_OPTION:
        ଯଦି (col_sep_len && !streq (col_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        col_sep = optarg;
        col_sep_len = *optarg ? strlen (optarg) : 1;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି TOTAL_OPTION:
        total_option = true;
        ଭାଙ୍ଗ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        usage (EXIT_FAILURE);
      }

  ଯଦି (! col_sep_len)
    col_sep_len = 1;

  ଯଦି (argc - optind < 2)
    {
      ଯଦି (argc <= optind)
        error (0, 0, _("missing operand"));
      ଅନ୍ୟଥା
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ଯଦି (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
