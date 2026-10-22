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
#تعریف_کرو پروگرام_NAME "comm"

#تعریف_کرو AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
ساکن bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
ساکن bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
ساکن bool only_file_2 = true;

/* If true, print lines that are found in both files. */
ساکن bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
ساکن bool نظر_آیا_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ساکن bool issued_disorder_warning[2];

/* line delimiter.  */
ساکن غیرعلامتی حرف delim = '\n';

/* If true, print a summary.  */
ساکن bool کل_option;

/* If nonzero, check that the input is correctly ordered. */
ساکن شمار
  {
    CHECK_ترتیب_DEFAULT,
    CHECK_ترتیب_ENABLED,
    CHECK_ترتیب_DISABLED
  } check_داخلہ_ترتیب;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
ساکن حرف ثابت *ستون_sep = "\t";
ساکن size_t ستون_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  CHECK_ترتیب_OPTION = CHAR_MAX + 1,
  NOCHECK_ترتیب_OPTION,
  OUTPUT_DELIMITER_OPTION,
  کل_OPTION
};

ساکن ساخت option ثابت long_options[] =
{
  {"check-order", no_دلیل, تہی, CHECK_ترتیب_OPTION},
  {"nocheck-order", no_دلیل, تہی, NOCHECK_ترتیب_OPTION},
  {"output-delimiter", required_دلیل, تہی, OUTPUT_DELIMITER_OPTION},
  {"total", no_دلیل, تہی, کل_OPTION},
  {"zero-terminated", no_دلیل, تہی, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};


خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              پروگرام_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 سطر by سطر.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, اقرأ standard داخلہ.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-عمود output.  عمود_2 one شامل_ہے\n\
سطریں unique مقصد FILE1, عمود two شامل_ہے سطریں unique مقصد FILE2,\n\
and عمود three شامل_ہے سطریں common مقصد both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress عمود 1 (سطریں unique مقصد FILE1)\n\
"));
      oputs (_("\
  -2     suppress عمود 2 (سطریں unique مقصد FILE2)\n\
"));
      oputs (_("\
  -3     suppress عمود 3 (سطریں that appear اندر both files)\n\
"));
      oputs (_("\
      --check-ترتیب\n\
         check that the داخلہ is correctly sorted,\n\
         even اگر all داخلہ سطریں are pairable\n\
"));
      oputs (_("\
      --nocheck-ترتیب\n\
         کرو not check that the داخلہ is correctly sorted\n\
"));
      oputs (_("\
      --output-delimiter=STR\n\
         separate columns with STR\n\
"));
      oputs (_("\
      --کل\n\
         output a summary\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Comparisons honor the قواعد specified by 'LC_COLLATE'.\n\
"), stdout);
      لکھو (_("\
\n\
Examples:\n\
  %s -12 file1 file2  Print only سطریں present اندر both file1 and file2.\n\
  %s -3 file1 file2  Print سطریں اندر file1 not اندر file2, and vice versa.\n\
"),
              پروگرام_name, پروگرام_name);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Output the line in linebuffer LINE to stdout
   provided the switches say it should be output.
   CLASS is 1 for a line found only in file 1,
   2 for a line only in file 2, 3 for a line in both. */

ساکن خالی
writeline (ساخت linebuffer ثابت *سطر, صحیح class)
{
  انتخاب (class)
    {
    حالت 1:
      اگر (!only_file_1)
        واپس;
      توڑو;

    حالت 2:
      اگر (!only_file_2)
        واپس;
      اگر (only_file_1)
        fwrite (ستون_sep, 1, ستون_sep_len, stdout);
      توڑو;

    حالت 3:
      اگر (!both)
        واپس;
      اگر (only_file_1)
        fwrite (ستون_sep, 1, ستون_sep_len, stdout);
      اگر (only_file_2)
        fwrite (ستون_sep, 1, ستون_sep_len, stdout);
      توڑو;
    }

  fwrite (سطر->بفر, حجم (حرف), سطر->لمبائی, stdout);

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

ساکن خالی
check_ترتیب (ساخت linebuffer ثابت *پچھلا,
             ساخت linebuffer ثابت *current,
             صحیح whatfile)
{

  اگر (check_داخلہ_ترتیب != CHECK_ترتیب_DISABLED
      && ((check_داخلہ_ترتیب == CHECK_ترتیب_ENABLED) || نظر_آیا_unpairable))
    {
      اگر (!issued_disorder_warning[whatfile - 1])
        {
          صحیح ترتیب;

          اگر (hard_LC_COLLATE)
            ترتیب = xmemcoll (پچھلا->بفر, پچھلا->لمبائی - 1,
                              current->بفر, current->لمبائی - 1);
          ورنہ
            ترتیب = memcmp2 (پچھلا->بفر, پچھلا->لمبائی - 1,
                             current->بفر, current->لمبائی - 1);

          اگر (0 < ترتیب)
            {
              error ((check_داخلہ_ترتیب == CHECK_ترتیب_ENABLED
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

ساکن _Noreturn خالی
compare_files (حرف **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  ساخت linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  ساخت linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  ساخت linebuffer *all_سطر[2][4];

  /* This is used to rotate through the buffers for each input file. */
  صحیح alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t کل[] = {0, 0, 0};

  /* Initialize the storage. */
  برائے (صحیح i = 0; i < 2; i++)
    {
      برائے (صحیح j = 0; j < 4; j++)
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

  جب (thisline[0] || thisline[1])
    {
      صحیح ترتیب;
      bool fill_اوپر[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      اگر (!thisline[0])
        ترتیب = 1;
      ورنہ اگر (!thisline[1])
        ترتیب = -1;
      ورنہ
        {
          اگر (hard_LC_COLLATE)
            ترتیب = xmemcoll (thisline[0]->بفر, thisline[0]->لمبائی - 1,
                              thisline[1]->بفر, thisline[1]->لمبائی - 1);
          ورنہ
            {
              size_t len = MIN (thisline[0]->لمبائی, thisline[1]->لمبائی) - 1;
              ترتیب = memcmp (thisline[0]->بفر, thisline[1]->بفر, len);
              اگر (ترتیب == 0)
                ترتیب = _GL_CMP (thisline[0]->لمبائی, thisline[1]->لمبائی);
            }
        }

      /* Output the line that is lesser. */
      اگر (ترتیب == 0)
        {
          /* Line is seen in both files.  */
          کل[2]++;
          writeline (thisline[1], 3);
        }
      ورنہ
        {
          نظر_آیا_unpairable = true;
          اگر (ترتیب <= 0)
            {
              /* Line is seen in file 1 only.  */
              کل[0]++;
              writeline (thisline[0], 1);
            }
          ورنہ
            {
              /* Line is seen in file 2 only.  */
              کل[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      اگر (0 <= ترتیب)
        fill_اوپر[1] = true;
      اگر (ترتیب <= 0)
        fill_اوپر[0] = true;

      برائے (صحیح i = 0; i < 2; i++)
        اگر (fill_اوپر[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_سطر[i][alt[i][0]],
                                                streams[i], delim);

            اگر (thisline[i])
              check_ترتیب (all_سطر[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            ورنہ اگر (all_سطر[i][alt[i][2]]->بفر)
              check_ترتیب (all_سطر[i][alt[i][2]],
                           all_سطر[i][alt[i][1]], i + 1);

            اگر (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_اوپر[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  ثابت صحیح n_streams = 2 - (streams[0] == streams[1]);
  برائے (صحیح i = 0; i < n_streams; i++)
    اگر (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  اگر (کل_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      اگر (ستون_sep_len == 1)
        { /* Separate to handle NUL char.  */
          لکھو ("%ju%c%ju%c%ju%c%s%c",
                  کل[0], *ستون_sep,
                  کل[1], *ستون_sep,
                  کل[2], *ستون_sep,
                  _("total"), delim);
        }
      ورنہ
        {
          لکھو ("%ju%s%ju%s%ju%s%s%c",
                  کل[0], ستون_sep,
                  کل[1], ستون_sep,
                  کل[2], ستون_sep,
                  _("total"), delim);
        }
    }

  اگر (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  باہر_نکلو (EXIT_SUCCESS);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح c;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  جب ((c = getopt_long (argc, argv, "123z", long_options, تہی)) != -1)
    انتخاب (c)
      {
      حالت '1':
        only_file_1 = false;
        توڑو;

      حالت '2':
        only_file_2 = false;
        توڑو;

      حالت '3':
        both = false;
        توڑو;

      حالت 'z':
        delim = '\0';
        توڑو;

      حالت NOCHECK_ترتیب_OPTION:
        check_داخلہ_ترتیب = CHECK_ترتیب_DISABLED;
        توڑو;

      حالت CHECK_ترتیب_OPTION:
        check_داخلہ_ترتیب = CHECK_ترتیب_ENABLED;
        توڑو;

      حالت OUTPUT_DELIMITER_OPTION:
        اگر (ستون_sep_len && !streq (ستون_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        ستون_sep = optarg;
        ستون_sep_len = *optarg ? لڑی_لمبائی (optarg) : 1;
        توڑو;

      حالت کل_OPTION:
        کل_option = true;
        توڑو;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

      طےشدہ:
        usage (EXIT_FAILURE);
      }

  اگر (! ستون_sep_len)
    ستون_sep_len = 1;

  اگر (argc - optind < 2)
    {
      اگر (argc <= optind)
        error (0, 0, _("missing operand"));
      ورنہ
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
