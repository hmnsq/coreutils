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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "fcntl--.h"
#ಅನ್ತರ್ಭಾವ "iopoll.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"
#ಅನ್ತರ್ಭಾವ "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tee"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ಸ್ಥಿರ bool tee_files (ಪೂರ್ಣಾಙ್ಕ nfiles, ವರ್ಣ **files, bool);

/* If true, append to output files rather than truncating them. */
ಸ್ಥಿರ bool append;

/* If true, ignore interrupts. */
ಸ್ಥಿರ bool ignore_interrupts;

ಗಣನಾ output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

ಸ್ಥಿರ ಗಣನಾ output_error output_error;

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"append", no_argument, ರಿಕ್ತ, 'a'},
  {"ignore-interrupts", no_argument, ರಿಕ್ತ, 'i'},
  {"output-error", optional_argument, ರಿಕ್ತ, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ output_error_args[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ output_error ಧ್ರುವ output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_args, output_error_types);

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Copy standard ಒಳಹರಿವು to each FILE, and also to standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append to the given FILEs, ಕುರು not overwrite\n\
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
         set behavior on ಲಿಖ error.  See MODE below\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
MODE determines behavior with ಲಿಖ errors on the outputs:\n\
  warn           diagnose errors writing to any output\n\
  warn-nopipe    diagnose errors writing to any output not a pipe\n\
  exit           exit on error writing to any output\n\
  exit-nopipe    exit on error writing to any output not a pipe\n\
The ಸ್ವಭಾವ MODE ಪ್ರತಿ the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, exit immediately ಯದಿ all outputs become broken pipes.\n\
The ಸ್ವಭಾವ operation when --output-error is not specified, is to\n\
exit immediately on error writing to a pipe, and diagnose errors\n\
writing to non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಙ್ಕ optc;
  ಯಾವತ್ ((optc = getopt_long (argc, argv, "aip", long_options, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'a':
          append = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'i':
          ignore_interrupts = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'p':
          ಯದಿ (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_args, output_error_types);
          ಅನ್ಯಥಾ
            output_error = output_error_warn_nopipe;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (ignore_interrupts)
    ಸಙ್ಕೇತ (SIGINT, SIG_IGN);

  ಯದಿ (output_error != output_error_sigpipe)
    ಸಙ್ಕೇತ (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_ಒಳಹರಿವು_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  ಯದಿ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_PURE
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_next_out (ಪೂರ್ಣಾಙ್ಕ *descriptors, ಪೂರ್ಣಾಙ್ಕ nfiles, ಪೂರ್ಣಾಙ್ಕ idx)
{
  ಪ್ರತಿ (idx++; idx <= nfiles; idx++)
    ಯದಿ (0 <= descriptors[idx])
      ಪ್ರತ್ಯಾಗಮ idx;
  ಪ್ರತ್ಯಾಗಮ -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

ಸ್ಥಿರ bool
fail_output (ಪೂರ್ಣಾಙ್ಕ *descriptors, ವರ್ಣ **files, ಪೂರ್ಣಾಙ್ಕ i)
{
  ಪೂರ್ಣಾಙ್ಕ w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  ಯದಿ (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  ಪ್ರತ್ಯಾಗಮ fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

ಸ್ಥಿರ bool
tee_files (ಪೂರ್ಣಾಙ್ಕ nfiles, ವರ್ಣ **files, bool pipe_check)
{
  size_t n_outputs = 0;
  ಪೂರ್ಣಾಙ್ಕ *descriptors;
  bool *out_pollable IF_LINT ( = ರಿಕ್ತ);
  ವರ್ಣ buffer[BUFSIZ];
  ssize_t bytes_read = 0;
  ಪೂರ್ಣಾಙ್ಕ first_out = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  ಪೂರ್ಣಾಙ್ಕ flags = O_WRONLY | O_CREAT | O_ದ್ವಿಮಾನ | (append ? O_APPEND : O_TRUNC);

  xset_ದ್ವಿಮಾನ_mode (STDIN_FILENO, O_ದ್ವಿಮಾನ);
  xset_ದ್ವಿಮಾನ_mode (STDOUT_FILENO, O_ದ್ವಿಮಾನ);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, ಪರಿಮಾಣ *descriptors);
  ಯದಿ (pipe_check)
    out_pollable = xnmalloc (nfiles + 1, ಪರಿಮಾಣ *out_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  ಯದಿ (pipe_check)
    out_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, MODE_RW_UGO);
      ಯದಿ (descriptors[i] < 0)
        {
          ಯದಿ (pipe_check)
            out_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (pipe_check)
            out_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  ಯಾವತ್ (n_outputs)
    {
      ಯದಿ (pipe_check && out_pollable[first_out])
        {
          /* Monitor for input, or errors on first valid output.  */
          ಪೂರ್ಣಾಙ್ಕ err = iopoll (STDIN_FILENO, descriptors[first_out], true);

          /* Close the output if it became a broken pipe.  */
          ಯದಿ (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              ಯದಿ (fail_output (descriptors, files, first_out))
                ok = false;
              n_outputs--;
              first_out = get_next_out (descriptors, nfiles, first_out);
              ಅನುವರ್ತ;
            }
          ಅನ್ಯಥಾ ಯದಿ (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      bytes_read = ಪಠ (STDIN_FILENO, buffer, ಪರಿಮಾಣ buffer);
      ಯದಿ (bytes_read < 0 && errno == EINTR)
        ಅನುವರ್ತ;
      ಯದಿ (bytes_read <= 0)
        ಭಙ್ಗ;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i <= nfiles; i++)
        ಯದಿ (0 <= descriptors[i]
            && ! write_wait (descriptors[i], buffer, bytes_read))
          {
            ಯದಿ (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            ಯದಿ (i == first_out)
              first_out = get_next_out (descriptors, nfiles, first_out);
          }
    }

  ಯದಿ (bytes_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i <= nfiles; i++)
    ಯದಿ (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  ಮುಕ್ತ (descriptors);
  ಯದಿ (pipe_check)
    ಮುಕ್ತ (out_pollable);

  ಪ್ರತ್ಯಾಗಮ ok;
}
