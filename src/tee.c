/* tee - read from standard input and write to standard output and files.
   Copyright (C) 1985-2026 Free Software Foundation, Inc.

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

/* Mike Parker, Richard M. Stallman, and David MacKenzie */

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <getopt.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "fcntl--.h"
#অন্তর্ভাব "iopoll.h"
#অন্তর্ভাব "xbinary-io.h"
#অন্তর্ভাব "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tee"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

স্থির bool tee_files (পূর্ণসংখ্যা nfiles, অক্ষর **files, bool);

/* If true, append to output files rather than truncating them. */
স্থির bool append;

/* If true, ignore interrupts. */
স্থির bool ignore_interrupts;

গণনা output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

স্থির গণনা output_error output_error;

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"append", no_argument, ফাঁকা, 'a'},
  {"ignore-interrupts", no_argument, ফাঁকা, 'i'},
  {"output-error", optional_argument, ফাঁকা, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

স্থির অক্ষর ধ্রুবক *ধ্রুবক output_error_args[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", ফাঁকা
};
স্থির গণনা output_error ধ্রুবক output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_args, output_error_types);

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Copy standard প্রবেশ to each FILE, and also to standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append to the given FILEs, করো not overwrite\n\
"));
      oputs (_("\
  -i, --ignore-interrupts\n\
         ignore interrupt signals\n\
"));
      oputs (_("\
  -p\n\
         operate in a more appropriate MODE with pipes\n\
"));
      oputs (_("\
      --output-error[=MODE]\n\
         set behavior on লিখ error.  See MODE below\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
MODE determines behavior with লিখ errors on the outputs:\n\
  warn           diagnose errors writing to any output\n\
  warn-nopipe    diagnose errors writing to any output not a pipe\n\
  exit           exit on error writing to any output\n\
  exit-nopipe    exit on error writing to any output not a pipe\n\
The পূর্বনির্ধারিত MODE জন্য the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, exit immediately যদি all outputs become broken pipes.\n\
The পূর্বনির্ধারিত operation when --output-error is not specified, is to\n\
exit immediately on error writing to a pipe, and diagnose errors\n\
writing to non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  পূর্ণসংখ্যা optc;
  যখন ((optc = getopt_long (argc, argv, "aip", long_options, ফাঁকা)) != -1)
    {
      নির্বাচন (optc)
        {
        ক্ষেত্র 'a':
          append = true;
          বিরতি;

        ক্ষেত্র 'i':
          ignore_interrupts = true;
          বিরতি;

        ক্ষেত্র 'p':
          যদি (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_args, output_error_types);
          অন্যথা
            output_error = output_error_warn_nopipe;
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (ignore_interrupts)
    সংকেত (SIGINT, SIG_IGN);

  যদি (output_error != output_error_sigpipe)
    সংকেত (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_প্রবেশ_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  যদি (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_PURE
স্থির পূর্ণসংখ্যা
get_পরবর্তী_out (পূর্ণসংখ্যা *descriptors, পূর্ণসংখ্যা nfiles, পূর্ণসংখ্যা idx)
{
  জন্য (idx++; idx <= nfiles; idx++)
    যদি (0 <= descriptors[idx])
      ফেরত idx;
  ফেরত -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

স্থির bool
fail_output (পূর্ণসংখ্যা *descriptors, অক্ষর **files, পূর্ণসংখ্যা i)
{
  পূর্ণসংখ্যা w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  যদি (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  ফেরত fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

স্থির bool
tee_files (পূর্ণসংখ্যা nfiles, অক্ষর **files, bool pipe_check)
{
  size_t n_outputs = 0;
  পূর্ণসংখ্যা *descriptors;
  bool *out_pollable IF_LINT ( = ফাঁকা);
  অক্ষর বাফার[BUFSIZ];
  ssize_t bytes_read = 0;
  পূর্ণসংখ্যা first_out = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  পূর্ণসংখ্যা flags = O_WRONLY | O_CREAT | O_দ্বিমিক | (append ? O_APPEND : O_TRUNC);

  xset_দ্বিমিক_mode (STDIN_FILENO, O_দ্বিমিক);
  xset_দ্বিমিক_mode (STDOUT_FILENO, O_দ্বিমিক);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, আকার *descriptors);
  যদি (pipe_check)
    out_pollable = xnmalloc (nfiles + 1, আকার *out_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  যদি (pipe_check)
    out_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  জন্য (পূর্ণসংখ্যা i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, MODE_RW_UGO);
      যদি (descriptors[i] < 0)
        {
          যদি (pipe_check)
            out_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      অন্যথা
        {
          যদি (pipe_check)
            out_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  যখন (n_outputs)
    {
      যদি (pipe_check && out_pollable[first_out])
        {
          /* Monitor for input, or errors on first valid output.  */
          পূর্ণসংখ্যা err = iopoll (STDIN_FILENO, descriptors[first_out], true);

          /* Close the output if it became a broken pipe.  */
          যদি (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              যদি (fail_output (descriptors, files, first_out))
                ok = false;
              n_outputs--;
              first_out = get_পরবর্তী_out (descriptors, nfiles, first_out);
              চালু;
            }
          অন্যথা যদি (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      bytes_read = পঠ (STDIN_FILENO, বাফার, আকার বাফার);
      যদি (bytes_read < 0 && errno == EINTR)
        চালু;
      যদি (bytes_read <= 0)
        বিরতি;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      জন্য (পূর্ণসংখ্যা i = 0; i <= nfiles; i++)
        যদি (0 <= descriptors[i]
            && ! write_wait (descriptors[i], বাফার, bytes_read))
          {
            যদি (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            যদি (i == first_out)
              first_out = get_পরবর্তী_out (descriptors, nfiles, first_out);
          }
    }

  যদি (bytes_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  জন্য (পূর্ণসংখ্যা i = 1; i <= nfiles; i++)
    যদি (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  মুক্ত (descriptors);
  যদি (pipe_check)
    মুক্ত (out_pollable);

  ফেরত ok;
}
