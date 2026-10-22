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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "linebuffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hard-locale.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdio--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "memcmp2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xmemcoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "comm"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

/* True if the LC_COLLATE locale is hard.  */
ਸਥਿਰ bool hard_LC_COLLATE;

/* If true, print lines that are found only in file 1. */
ਸਥਿਰ bool only_file_1 = true;

/* If true, print lines that are found only in file 2. */
ਸਥਿਰ bool only_file_2 = true;

/* If true, print lines that are found in both files. */
ਸਥਿਰ bool both = true;

/* If nonzero, we have seen at least one unpairable line. */
ਸਥਿਰ bool ਵੇਖਿਆ_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ਸਥਿਰ bool issued_disorder_warning[2];

/* line delimiter.  */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ delim = '\n';

/* If true, print a summary.  */
ਸਥਿਰ bool ਕੁੱਲ_option;

/* If nonzero, check that the input is correctly ordered. */
ਸਥਿਰ ਗਿਣਤੀ
  {
    CHECK_ਕ੍ਰਮ_DEFAULT,
    CHECK_ਕ੍ਰਮ_ENABLED,
    CHECK_ਕ੍ਰਮ_DISABLED
  } check_ਨਿਵੇਸ਼_ਕ੍ਰਮ;

/* Output columns will be delimited with this string, which may be set
   on the command-line with --output-delimiter=STR.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਖਾਨਾ_sep = "\t";
ਸਥਿਰ size_t ਖਾਨਾ_sep_len = 0;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  CHECK_ਕ੍ਰਮ_OPTION = CHAR_MAX + 1,
  NOCHECK_ਕ੍ਰਮ_OPTION,
  OUTPUT_DELIMITER_OPTION,
  ਕੁੱਲ_OPTION
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"check-order", no_ਦਲੀਲ, ਨਿਰਾ, CHECK_ਕ੍ਰਮ_OPTION},
  {"nocheck-order", no_ਦਲੀਲ, ਨਿਰਾ, NOCHECK_ਕ੍ਰਮ_OPTION},
  {"output-delimiter", required_ਦਲੀਲ, ਨਿਰਾ, OUTPUT_DELIMITER_OPTION},
  {"total", no_ਦਲੀਲ, ਨਿਰਾ, ਕੁੱਲ_OPTION},
  {"zero-terminated", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};


ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Compare sorted files FILE1 and FILE2 ਸਤਰ by ਸਤਰ.\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 is -, ਪਠ standard ਨਿਵੇਸ਼.\n\
"), stdout);
      fputs (_("\
\n\
With no options, produce three-ਕਾਲਮ output.  ਕਾਲਮ_2 one ਸ਼ਾਮਲ_ਹੈ\n\
ਸਤਰਾਂ unique ਮੰਜ਼ਿਲ FILE1, ਕਾਲਮ two ਸ਼ਾਮਲ_ਹੈ ਸਤਰਾਂ unique ਮੰਜ਼ਿਲ FILE2,\n\
and ਕਾਲਮ three ਸ਼ਾਮਲ_ਹੈ ਸਤਰਾਂ common ਮੰਜ਼ਿਲ both files.\n\
"), stdout);
      oputs (_("\
  -1     suppress ਕਾਲਮ 1 (ਸਤਰਾਂ unique ਮੰਜ਼ਿਲ FILE1)\n\
"));
      oputs (_("\
  -2     suppress ਕਾਲਮ 2 (ਸਤਰਾਂ unique ਮੰਜ਼ਿਲ FILE2)\n\
"));
      oputs (_("\
  -3     suppress ਕਾਲਮ 3 (ਸਤਰਾਂ that appear ਵਿੱਚ both files)\n\
"));
      oputs (_("\
      --check-ਕ੍ਰਮ\n\
         check that the ਨਿਵੇਸ਼ is correctly sorted,\n\
         even ਜੇ all ਨਿਵੇਸ਼ ਸਤਰਾਂ are pairable\n\
"));
      oputs (_("\
      --nocheck-ਕ੍ਰਮ\n\
         ਕਰੋ not check that the ਨਿਵੇਸ਼ is correctly sorted\n\
"));
      oputs (_("\
      --output-delimiter=STR\n\
         separate columns with STR\n\
"));
      oputs (_("\
      --ਕੁੱਲ\n\
         output a summary\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ਸਤਰ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Comparisons honor the ਨਿਯਮਾਂ specified by 'LC_COLLATE'.\n\
"), stdout);
      ਲਿਖੋ (_("\
\n\
Examples:\n\
  %s -12 file1 file2  Print only ਸਤਰਾਂ present ਵਿੱਚ both file1 and file2.\n\
  %s -3 file1 file2  Print ਸਤਰਾਂ ਵਿੱਚ file1 not ਵਿੱਚ file2, and vice versa.\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Output the line in linebuffer LINE to stdout
   provided the switches say it should be output.
   CLASS is 1 for a line found only in file 1,
   2 for a line only in file 2, 3 for a line in both. */

ਸਥਿਰ ਖਾਲੀ
writeline (ਬਣਤਰ linebuffer ਅਚਲ *ਸਤਰ, ਪੂਰਨ class)
{
  ਚੋਣ (class)
    {
    ਹਾਲਤ 1:
      ਜੇ (!only_file_1)
        ਵਾਪਸ;
      ਰੋਕੋ;

    ਹਾਲਤ 2:
      ਜੇ (!only_file_2)
        ਵਾਪਸ;
      ਜੇ (only_file_1)
        fwrite (ਖਾਨਾ_sep, 1, ਖਾਨਾ_sep_len, stdout);
      ਰੋਕੋ;

    ਹਾਲਤ 3:
      ਜੇ (!both)
        ਵਾਪਸ;
      ਜੇ (only_file_1)
        fwrite (ਖਾਨਾ_sep, 1, ਖਾਨਾ_sep_len, stdout);
      ਜੇ (only_file_2)
        fwrite (ਖਾਨਾ_sep, 1, ਖਾਨਾ_sep_len, stdout);
      ਰੋਕੋ;
    }

  fwrite (ਸਤਰ->ਬਫਰ, ਆਕਾਰ (ਅੱਖਰ), ਸਤਰ->ਲੰਮਾਈ, stdout);

  ਜੇ (ferror (stdout))
    write_error ();
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file.

   This function was copied (nearly) verbatim from 'src/join.c'. */

ਸਥਿਰ ਖਾਲੀ
check_ਕ੍ਰਮ (ਬਣਤਰ linebuffer ਅਚਲ *prev,
             ਬਣਤਰ linebuffer ਅਚਲ *current,
             ਪੂਰਨ whatfile)
{

  ਜੇ (check_ਨਿਵੇਸ਼_ਕ੍ਰਮ != CHECK_ਕ੍ਰਮ_DISABLED
      && ((check_ਨਿਵੇਸ਼_ਕ੍ਰਮ == CHECK_ਕ੍ਰਮ_ENABLED) || ਵੇਖਿਆ_unpairable))
    {
      ਜੇ (!issued_disorder_warning[whatfile - 1])
        {
          ਪੂਰਨ ਕ੍ਰਮ;

          ਜੇ (hard_LC_COLLATE)
            ਕ੍ਰਮ = xmemcoll (prev->ਬਫਰ, prev->ਲੰਮਾਈ - 1,
                              current->ਬਫਰ, current->ਲੰਮਾਈ - 1);
          ਨਹੀਂਤਾਂ
            ਕ੍ਰਮ = memcmp2 (prev->ਬਫਰ, prev->ਲੰਮਾਈ - 1,
                             current->ਬਫਰ, current->ਲੰਮਾਈ - 1);

          ਜੇ (0 < ਕ੍ਰਮ)
            {
              error ((check_ਨਿਵੇਸ਼_ਕ੍ਰਮ == CHECK_ਕ੍ਰਮ_ENABLED
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

ਸਥਿਰ _Noreturn ਖਾਲੀ
compare_files (ਅੱਖਰ **infiles)
{
  /* For each file, we have four linebuffers in lba. */
  ਬਣਤਰ linebuffer lba[2][4];

  /* thisline[i] points to the linebuffer holding the next available line
     in file i, or is null if there are no lines left in that file.  */
  ਬਣਤਰ linebuffer *thisline[2];

  /* all_line[i][alt[i][0]] also points to the linebuffer holding the
     current line in file i. We keep two buffers of history around so we
     can look two lines back when we get to the end of a file. */
  ਬਣਤਰ linebuffer *all_ਸਤਰ[2][4];

  /* This is used to rotate through the buffers for each input file. */
  ਪੂਰਨ alt[2][3];

  /* streams[i] holds the input stream for file i.  */
  FILE *streams[2];

  /* Counters for the summary.  */
  uintmax_t ਕੁੱਲ[] = {0, 0, 0};

  /* Initialize the storage. */
  ਲਈ (ਪੂਰਨ i = 0; i < 2; i++)
    {
      ਲਈ (ਪੂਰਨ j = 0; j < 4; j++)
        {
          initbuffer (&lba[i][j]);
          all_ਸਤਰ[i][j] = &lba[i][j];
        }
      alt[i][0] = 0;
      alt[i][1] = 0;
      alt[i][2] = 0;
      streams[i] = (streq (infiles[i], "-") ? stdin : fopen (infiles[i], "r"));
      ਜੇ (!streams[i])
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

      fadvise (streams[i], FADVISE_SEQUENTIAL);

      thisline[i] = readlinebuffer_delim (all_ਸਤਰ[i][alt[i][0]], streams[i],
                                          delim);
      ਜੇ (ferror (streams[i]))
        error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));
    }

  ਜਦੋਂ (thisline[0] || thisline[1])
    {
      ਪੂਰਨ ਕ੍ਰਮ;
      bool fill_ਉੱਪਰ[2] = { false, false };

      /* Compare the next available lines of the two files.  */

      ਜੇ (!thisline[0])
        ਕ੍ਰਮ = 1;
      ਨਹੀਂਤਾਂ ਜੇ (!thisline[1])
        ਕ੍ਰਮ = -1;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (hard_LC_COLLATE)
            ਕ੍ਰਮ = xmemcoll (thisline[0]->ਬਫਰ, thisline[0]->ਲੰਮਾਈ - 1,
                              thisline[1]->ਬਫਰ, thisline[1]->ਲੰਮਾਈ - 1);
          ਨਹੀਂਤਾਂ
            {
              size_t len = MIN (thisline[0]->ਲੰਮਾਈ, thisline[1]->ਲੰਮਾਈ) - 1;
              ਕ੍ਰਮ = memcmp (thisline[0]->ਬਫਰ, thisline[1]->ਬਫਰ, len);
              ਜੇ (ਕ੍ਰਮ == 0)
                ਕ੍ਰਮ = _GL_CMP (thisline[0]->ਲੰਮਾਈ, thisline[1]->ਲੰਮਾਈ);
            }
        }

      /* Output the line that is lesser. */
      ਜੇ (ਕ੍ਰਮ == 0)
        {
          /* Line is seen in both files.  */
          ਕੁੱਲ[2]++;
          writeline (thisline[1], 3);
        }
      ਨਹੀਂਤਾਂ
        {
          ਵੇਖਿਆ_unpairable = true;
          ਜੇ (ਕ੍ਰਮ <= 0)
            {
              /* Line is seen in file 1 only.  */
              ਕੁੱਲ[0]++;
              writeline (thisline[0], 1);
            }
          ਨਹੀਂਤਾਂ
            {
              /* Line is seen in file 2 only.  */
              ਕੁੱਲ[1]++;
              writeline (thisline[1], 2);
            }
        }

      /* Step the file the line came from.
         If the files match, step both files.  */
      ਜੇ (0 <= ਕ੍ਰਮ)
        fill_ਉੱਪਰ[1] = true;
      ਜੇ (ਕ੍ਰਮ <= 0)
        fill_ਉੱਪਰ[0] = true;

      ਲਈ (ਪੂਰਨ i = 0; i < 2; i++)
        ਜੇ (fill_ਉੱਪਰ[i])
          {
            /* Rotate the buffers for this file. */
            alt[i][2] = alt[i][1];
            alt[i][1] = alt[i][0];
            alt[i][0] = (alt[i][0] + 1) & 0x03;

            thisline[i] = readlinebuffer_delim (all_ਸਤਰ[i][alt[i][0]],
                                                streams[i], delim);

            ਜੇ (thisline[i])
              check_ਕ੍ਰਮ (all_ਸਤਰ[i][alt[i][1]], thisline[i], i + 1);

            /* If this is the end of the file we may need to re-check
               the order of the previous two lines, since we might have
               discovered an unpairable match since we checked before. */
            ਨਹੀਂਤਾਂ ਜੇ (all_ਸਤਰ[i][alt[i][2]]->ਬਫਰ)
              check_ਕ੍ਰਮ (all_ਸਤਰ[i][alt[i][2]],
                           all_ਸਤਰ[i][alt[i][1]], i + 1);

            ਜੇ (ferror (streams[i]))
              error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

            fill_ਉੱਪਰ[i] = false;
          }
    }

  /* Avoid closing standard input twice when invoking 'comm - -'.  */
  ਅਚਲ ਪੂਰਨ n_streams = 2 - (streams[0] == streams[1]);
  ਲਈ (ਪੂਰਨ i = 0; i < n_streams; i++)
    ਜੇ (fclose (streams[i]) != 0)
      error (EXIT_FAILURE, errno, "%s", quotef (infiles[i]));

  ਜੇ (ਕੁੱਲ_option)
    {
      /* Print the summary, minding the column and line delimiters.  */
      ਜੇ (ਖਾਨਾ_sep_len == 1)
        { /* Separate to handle NUL char.  */
          ਲਿਖੋ ("%ju%c%ju%c%ju%c%s%c",
                  ਕੁੱਲ[0], *ਖਾਨਾ_sep,
                  ਕੁੱਲ[1], *ਖਾਨਾ_sep,
                  ਕੁੱਲ[2], *ਖਾਨਾ_sep,
                  _("total"), delim);
        }
      ਨਹੀਂਤਾਂ
        {
          ਲਿਖੋ ("%ju%s%ju%s%ju%s%s%c",
                  ਕੁੱਲ[0], ਖਾਨਾ_sep,
                  ਕੁੱਲ[1], ਖਾਨਾ_sep,
                  ਕੁੱਲ[2], ਖਾਨਾ_sep,
                  _("total"), delim);
        }
    }

  ਜੇ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));

  /* Exit here to pacify gcc -fsanitizer=leak.  */
  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ c;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);

  ਜਦੋਂ ((c = getopt_long (argc, argv, "123z", long_options, ਨਿਰਾ)) != -1)
    ਚੋਣ (c)
      {
      ਹਾਲਤ '1':
        only_file_1 = false;
        ਰੋਕੋ;

      ਹਾਲਤ '2':
        only_file_2 = false;
        ਰੋਕੋ;

      ਹਾਲਤ '3':
        both = false;
        ਰੋਕੋ;

      ਹਾਲਤ 'z':
        delim = '\0';
        ਰੋਕੋ;

      ਹਾਲਤ NOCHECK_ਕ੍ਰਮ_OPTION:
        check_ਨਿਵੇਸ਼_ਕ੍ਰਮ = CHECK_ਕ੍ਰਮ_DISABLED;
        ਰੋਕੋ;

      ਹਾਲਤ CHECK_ਕ੍ਰਮ_OPTION:
        check_ਨਿਵੇਸ਼_ਕ੍ਰਮ = CHECK_ਕ੍ਰਮ_ENABLED;
        ਰੋਕੋ;

      ਹਾਲਤ OUTPUT_DELIMITER_OPTION:
        ਜੇ (ਖਾਨਾ_sep_len && !streq (ਖਾਨਾ_sep, optarg))
          error (EXIT_FAILURE, 0, _("multiple output delimiters specified"));
        ਖਾਨਾ_sep = optarg;
        ਖਾਨਾ_sep_len = *optarg ? ਲੜੀ_ਲੰਬਾਈ (optarg) : 1;
        ਰੋਕੋ;

      ਹਾਲਤ ਕੁੱਲ_OPTION:
        ਕੁੱਲ_option = true;
        ਰੋਕੋ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

      ਮੂਲ:
        usage (EXIT_FAILURE);
      }

  ਜੇ (! ਖਾਨਾ_sep_len)
    ਖਾਨਾ_sep_len = 1;

  ਜੇ (argc - optind < 2)
    {
      ਜੇ (argc <= optind)
        error (0, 0, _("missing operand"));
      ਨਹੀਂਤਾਂ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ਜੇ (2 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  compare_files (argv + optind);
}
