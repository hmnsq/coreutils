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
#تعریف_کن برنامه_NAME "comm"

#تعریف_کن AUTHORS \
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
ساكن bool دیده_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ساكن bool issued_disorder_warning[2];

/* line delimiter.  */
ساكن بیعلامت حرف delim = '\n';

/* If true, print a summary.  */
ساكن bool مجموع_option;

/* If nonzero, check that the input is correctly ordered. */
ساكن تعداد
  {
    CHECK_ترتیب_DEFAULT,
    CHECK_ترتیب_ENABLED,
    CHECK_ترتیب_DISABLED
  } check_درون_داد_ترتیب;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
ساكن حرف ثابت *ستون_sep = "\t";
ساكن size_t ستون_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  CHECK_ترتیب_OPTION = CHAR_MAX + 1,
  NOCHECK_ترتیب_OPTION,
  OUTPUT_DELIMITER_OPTION,
  مجموع_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"check-order", no_آرگومان, عدم, CHECK_ترتیب_OPTION},
  {"nocheck-order", no_آرگومان, عدم, NOCHECK_ترتیب_OPTION},
  {"output-delimiter", required_آرگومان, عدم, OUTPUT_DELIMITER_OPTION},
  {"total", no_آرگومان, عدم, مجموع_OPTION},
  {"zero-terminated", no_آرگومان, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};


فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              برنامه_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 سطر by سطر.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, اقرأ standard درون_داد.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-ستونی output.  ستونی_2 one شامل_است\n\
سطرها unique مقصد FILE1, ستونی two شامل_است سطرها unique مقصد FILE2,\n\
and ستونی three شامل_است سطرها common مقصد both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress ستونی 1 (سطرها unique مقصد FILE1)\n\
"));
      oputs (_("\
  -2     suppress ستونی 2 (سطرها unique مقصد FILE2)\n\
"));
      oputs (_("\
  -3     suppress ستونی 3 (سطرها that appear درون both files)\n\
"));
      oputs (_("\
      --check-ترتیب\n\
         check that the درون_داد is correctly sorted,\n\
         even إذا all درون_داد سطرها are pairable\n\
"));
      oputs (_("\
      --nocheck-ترتیب\n\
         افعل not check that the درون_داد is correctly sorted\n\
"));
      oputs (_("\
      --output-delimiter=STR\n\
         separate columns with STR\n\
"));
      oputs (_("\
      --مجموع\n\
         output a summary\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Comparisons honor the قواعد specified by 'LC_COLLATE'.\n\
"), stdout);
      چاپ_کن (_("\
\n\
Examples:\n\
  %s -12 file1 file2  Print only سطرها present درون both file1 and file2.\n\
  %s -3 file1 file2  Print سطرها درون file1 not درون file2, and vice versa.\n\
"),
              برنامه_name, برنامه_name);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* Output the line in linebuffer LINE to stdout
   provided the switches say it should be output.
   CLASS is 1 for a line found only in file 1,
   2 for a line only in file 2, 3 for a line in both. */

ساكن فراغ
writeline (بنية linebuffer ثابت *سطر, صحيح class)
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
        fwrite (ستون_sep, 1, ستون_sep_len, stdout);
      اكسر;

    حالة 3:
      إذا (!both)
        أرجع;
      إذا (only_file_1)
        fwrite (ستون_sep, 1, ستون_sep_len, stdout);
      إذا (only_file_2)
        fwrite (ستون_sep, 1, ستون_sep_len, stdout);
      اكسر;
    }

  fwrite (سطر->میانگیر, حجم (حرف), سطر->درازی, stdout);

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
check_ترتیب (بنية linebuffer ثابت *prev,
             بنية linebuffer ثابت *current,
             صحيح whatfile)
{

  إذا (check_درون_داد_ترتیب != CHECK_ترتیب_DISABLED
      && ((check_درون_داد_ترتیب == CHECK_ترتیب_ENABLED) || دیده_unpairable))
    {
      إذا (!issued_disorder_warning[whatfile - 1])
        {
          صحيح ترتیب;

          إذا (hard_LC_COLLATE)
            ترتیب = xmemcoll (prev->میانگیر, prev->درازی - 1,
                              current->میانگیر, current->درازی - 1);
          وإلا
            ترتیب = memcmp2 (prev->میانگیر, prev->درازی - 1,
                             current->میانگیر, current->درازی - 1);

          إذا (0 < ترتیب)
            {
              error ((check_درون_داد_ترتیب == CHECK_ترتیب_ENABLED
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
  بنية linebuffer *all_سطر[2][4];

  /* This is used to rotate through the buffers for each input file. */
  صحيح alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t مجموع[] = {0, 0, 0};

  /* Initialize the storage. */
  لكل (صحيح i = 0; i < 2; i++)
    {
      لكل (صحيح j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_سطر[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      إذا (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_سطر[i][alt[i][0]], streams[i],
                                          delim);
      إذا (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  طالما (thisline[0] || thisline[1])
    {
      صحيح ترتیب;
      bool fill_به_بالا[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      إذا (!thisline[0])
        ترتیب = 1;
      وإلا إذا (!thisline[1])
        ترتیب = -1;
      وإلا
        {
          إذا (hard_LC_COLLATE)
            ترتیب = xmemcoll (thisline[0]->میانگیر, thisline[0]->درازی - 1,
                              thisline[1]->میانگیر, thisline[1]->درازی - 1);
          وإلا
            {
              size_t len = MIN (thisline[0]->درازی, thisline[1]->درازی) - 1;
              ترتیب = memcmp (thisline[0]->میانگیر, thisline[1]->میانگیر, len);
              إذا (ترتیب == 0)
                ترتیب = _GL_CMP (thisline[0]->درازی, thisline[1]->درازی);
            }
        }

      /* Output the line that is lesser. */
      إذا (ترتیب == 0)
        {
          /* Line is seen in both files.  */
          مجموع[2]++;
          writeline (thisline[1], 3);
        }
      وإلا
        {
          دیده_unpairable = true;
          إذا (ترتیب <= 0)
            {
              /* Line is seen in file 1 only.  */
              مجموع[0]++;
              writeline (thisline[0], 1);
            }
          وإلا
            {
              /* Line is seen in file 2 only.  */
              مجموع[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      إذا (0 <= ترتیب)
        fill_به_بالا[1] = true;
      إذا (ترتیب <= 0)
        fill_به_بالا[0] = true;

      لكل (صحيح i = 0; i < 2; i++)
        إذا (fill_به_بالا[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_سطر[i][alt[i][0]],
                                                streams[i], delim);

            إذا (thisline[i])
              check_ترتیب (all_سطر[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            وإلا إذا (all_سطر[i][alt[i][2]]->میانگیر)
              check_ترتیب (all_سطر[i][alt[i][2]],
                           all_سطر[i][alt[i][1]], i + 1);

            إذا (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_به_بالا[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  ثابت صحيح n_streams = 2 - (streams[0] == streams[1]);
  لكل (صحيح i = 0; i < n_streams; i++)
    إذا (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  إذا (مجموع_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      إذا (ستون_sep_len == 1)
        { /* Separate to handle NUL char.  */
          چاپ_کن ("%ju%c%ju%c%ju%c%s%c",
                  مجموع[0], *ستون_sep,
                  مجموع[1], *ستون_sep,
                  مجموع[2], *ستون_sep,
                  _("total"), delim);
        }
      وإلا
        {
          چاپ_کن ("%ju%s%ju%s%ju%s%s%c",
                  مجموع[0], ستون_sep,
                  مجموع[1], ستون_sep,
                  مجموع[2], ستون_sep,
                  _("total"), delim);
        }
    }

  إذا (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  خروج (EXIT_SUCCESS);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح c;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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

      حالة NOCHECK_ترتیب_OPTION:
        check_درون_داد_ترتیب = CHECK_ترتیب_DISABLED;
        اكسر;

      حالة CHECK_ترتیب_OPTION:
        check_درون_داد_ترتیب = CHECK_ترتیب_ENABLED;
        اكسر;

      حالة OUTPUT_DELIMITER_OPTION:
        إذا (ستون_sep_len && !streq (ستون_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        ستون_sep = optarg;
        ستون_sep_len = *optarg ? طول_رشته (optarg) : 1;
        اكسر;

      حالة مجموع_OPTION:
        مجموع_option = true;
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

      افتراضي:
        usage (EXIT_FAILURE);
      }

  إذا (! ستون_sep_len)
    ستون_sep_len = 1;

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
