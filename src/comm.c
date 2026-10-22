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

#ضمن <config.h>

#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن "system.h"
#ضمن "linebuffer.h"
#ضمن "fadvise.h"
#ضمن "hard-locale.h"
#ضمن "quote.h"
#ضمن "stdio--.h"
#ضمن "memcmp2.h"
#ضمن "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "comm"

#وصف_ڪريو AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
ساكن bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
ساكن bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
ساكن bool only_file_2 = true;

/* If true, print lines that are found in both files. */
ساكن bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
ساكن bool ڏٺل_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ساكن bool issued_disorder_warning[2];

/* line delimiter.  */
ساكن بنانشان حرف delim = '\n';

/* If true, print a summary.  */
ساكن bool ڪل_option;

/* If nonzero, check that the input is correctly ordered. */
ساكن تعداد
  {
    CHECK_ترتيب_DEFAULT,
    CHECK_ترتيب_ENABLED,
    CHECK_ترتيب_DISABLED
  } check_داخلا_ترتيب;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
ساكن حرف ثابت *کانو_sep = "\t";
ساكن size_t کانو_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  CHECK_ترتيب_OPTION = CHAR_MAX + 1,
  NOCHECK_ترتيب_OPTION,
  OUTPUT_DELIMITER_OPTION,
  ڪل_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"check-order", no_دليل, عدم, CHECK_ترتيب_OPTION},
  {"nocheck-order", no_دليل, عدم, NOCHECK_ترتيب_OPTION},
  {"output-delimiter", required_دليل, عدم, OUTPUT_DELIMITER_OPTION},
  {"total", no_دليل, عدم, ڪل_OPTION},
  {"zero-terminated", no_دليل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};


فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              پروگرام_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 سٽ by سٽ.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, اقرأ standard داخلا.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-ڪالم output.  ڪالم_2 one شامل_آهي\n\
سٽون unique منزل FILE1, ڪالم two شامل_آهي سٽون unique منزل FILE2,\n\
and ڪالم three شامل_آهي سٽون common منزل both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress ڪالم 1 (سٽون unique منزل FILE1)\n\
"));
      oputs (_("\
  -2     suppress ڪالم 2 (سٽون unique منزل FILE2)\n\
"));
      oputs (_("\
  -3     suppress ڪالم 3 (سٽون that appear اندريون both files)\n\
"));
      oputs (_("\
      --check-ترتيب\n\
         check that the داخلا is correctly sorted,\n\
         even إذا all داخلا سٽون are pairable\n\
"));
      oputs (_("\
      --nocheck-ترتيب\n\
         افعل not check that the داخلا is correctly sorted\n\
"));
      oputs (_("\
      --output-delimiter=STR\n\
         separate columns with STR\n\
"));
      oputs (_("\
      --ڪل\n\
         output a summary\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سٽ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Comparisons honor the قاعدا specified by 'LC_COLLATE'.\n\
"), stdout);
      لکو (_("\
\n\
Examples:\n\
  %s -12 file1 file2  Print only سٽون present اندريون both file1 and file2.\n\
  %s -3 file1 file2  Print سٽون اندريون file1 not اندريون file2, and vice versa.\n\
"),
              پروگرام_name, پروگرام_name);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Output the line in linebuffer LINE to stdout
   provided the switches say it should be output.
   CLASS is 1 for a line found only in file 1,
   2 for a line only in file 2, 3 for a line in both. */

ساكن فراغ
writeline (بنية linebuffer ثابت *سٽ, صحيح class)
{
  بدل (class)
    {
    حالة 1:
      إذا (!only_file_1)
        أرجع;
      اكسر;

    حالة 2:
      إذا (!only_file_2)
        أرجع;
      إذا (only_file_1)
        fwrite (کانو_sep, 1, کانو_sep_len, stdout);
      اكسر;

    حالة 3:
      إذا (!both)
        أرجع;
      إذا (only_file_1)
        fwrite (کانو_sep, 1, کانو_sep_len, stdout);
      إذا (only_file_2)
        fwrite (کانو_sep, 1, کانو_sep_len, stdout);
      اكسر;
    }

  fwrite (سٽ->بفر, حجم (حرف), سٽ->ڊيگهائي, stdout);

  إذا (ferror (stdout))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

ساكن فراغ
check_ترتيب (بنية linebuffer ثابت *prev,
             بنية linebuffer ثابت *current,
             صحيح whatfile)
{

  إذا (check_داخلا_ترتيب != CHECK_ترتيب_DISABLED
      && ((check_داخلا_ترتيب == CHECK_ترتيب_ENABLED) || ڏٺل_unpairable))
    {
      إذا (!issued_disorder_warning[whatfile - 1])
        {
          صحيح ترتيب;

          إذا (hard_LC_COLLATE)
            ترتيب = xmemcoll (prev->بفر, prev->ڊيگهائي - 1,
                              current->بفر, current->ڊيگهائي - 1);
          وإلا
            ترتيب = memcmp2 (prev->بفر, prev->ڊيگهائي - 1,
                             current->بفر, current->ڊيگهائي - 1);

          إذا (0 < ترتيب)
            {
              error ((check_داخلا_ترتيب == CHECK_ترتيب_ENABLED
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

ساكن _Noreturn فراغ
compare_files (حرف **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  بنية linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  بنية linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  بنية linebuffer *all_سٽ[2][4];

  /* This is used to rotate through the buffers for each input file. */
  صحيح alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t ڪل[] = {0, 0, 0};

  /* Initialize the storage. */
  لكل (صحيح i = 0; i < 2; i++)
    {
      لكل (صحيح j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_سٽ[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      إذا (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_سٽ[i][alt[i][0]], streams[i],
                                          delim);
      إذا (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  طالما (thisline[0] || thisline[1])
    {
      صحيح ترتيب;
      bool fill_مٿي[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      إذا (!thisline[0])
        ترتيب = 1;
      وإلا إذا (!thisline[1])
        ترتيب = -1;
      وإلا
        {
          إذا (hard_LC_COLLATE)
            ترتيب = xmemcoll (thisline[0]->بفر, thisline[0]->ڊيگهائي - 1,
                              thisline[1]->بفر, thisline[1]->ڊيگهائي - 1);
          وإلا
            {
              size_t len = MIN (thisline[0]->ڊيگهائي, thisline[1]->ڊيگهائي) - 1;
              ترتيب = memcmp (thisline[0]->بفر, thisline[1]->بفر, len);
              إذا (ترتيب == 0)
                ترتيب = _GL_CMP (thisline[0]->ڊيگهائي, thisline[1]->ڊيگهائي);
            }
        }

      /* Output the line that is lesser. */
      إذا (ترتيب == 0)
        {
          /* Line is seen in both files.  */
          ڪل[2]++;
          writeline (thisline[1], 3);
        }
      وإلا
        {
          ڏٺل_unpairable = true;
          إذا (ترتيب <= 0)
            {
              /* Line is seen in file 1 only.  */
              ڪل[0]++;
              writeline (thisline[0], 1);
            }
          وإلا
            {
              /* Line is seen in file 2 only.  */
              ڪل[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      إذا (0 <= ترتيب)
        fill_مٿي[1] = true;
      إذا (ترتيب <= 0)
        fill_مٿي[0] = true;

      لكل (صحيح i = 0; i < 2; i++)
        إذا (fill_مٿي[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_سٽ[i][alt[i][0]],
                                                streams[i], delim);

            إذا (thisline[i])
              check_ترتيب (all_سٽ[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            وإلا إذا (all_سٽ[i][alt[i][2]]->بفر)
              check_ترتيب (all_سٽ[i][alt[i][2]],
                           all_سٽ[i][alt[i][1]], i + 1);

            إذا (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_مٿي[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  ثابت صحيح n_streams = 2 - (streams[0] == streams[1]);
  لكل (صحيح i = 0; i < n_streams; i++)
    إذا (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  إذا (ڪل_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      إذا (کانو_sep_len == 1)
        { /* Separate to handle NUL char.  */
          لکو ("%ju%c%ju%c%ju%c%s%c",
                  ڪل[0], *کانو_sep,
                  ڪل[1], *کانو_sep,
                  ڪل[2], *کانو_sep,
                  _("total"), delim);
        }
      وإلا
        {
          لکو ("%ju%s%ju%s%ju%s%s%c",
                  ڪل[0], کانو_sep,
                  ڪل[1], کانو_sep,
                  ڪل[2], کانو_sep,
                  _("total"), delim);
        }
    }

  إذا (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  ٻاهر_نڪرو (EXIT_SUCCESS);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح c;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  طالما ((c = getopt_long (argc, argv, "123z", long_options, عدم)) != -1)
    بدل (c)
      {
      حالة '1':
        only_file_1 = false;
        اكسر;

      حالة '2':
        only_file_2 = false;
        اكسر;

      حالة '3':
        both = false;
        اكسر;

      حالة 'z':
        delim = '\0';
        اكسر;

      حالة NOCHECK_ترتيب_OPTION:
        check_داخلا_ترتيب = CHECK_ترتيب_DISABLED;
        اكسر;

      حالة CHECK_ترتيب_OPTION:
        check_داخلا_ترتيب = CHECK_ترتيب_ENABLED;
        اكسر;

      حالة OUTPUT_DELIMITER_OPTION:
        إذا (کانو_sep_len && !streq (کانو_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        کانو_sep = optarg;
        کانو_sep_len = *optarg ? لڙي_ڊيگهه (optarg) : 1;
        اكسر;

      حالة ڪل_OPTION:
        ڪل_option = true;
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

      افتراضي:
        usage (EXIT_FAILURE);
      }

  إذا (! کانو_sep_len)
    کانو_sep_len = 1;

  إذا (argc - optind < 2)
    {
      إذا (argc <= optind)
        error (0, 0, _("missing operand"));
      وإلا
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  إذا (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
