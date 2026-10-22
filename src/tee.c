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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "tee"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ಸ್ಥಿರ bool tee_files (ಪೂರ್ಣಾಂಕ nfiles, ಅಕ್ಷರ **files, bool);

/* If true, append to output files rather than truncating them. */
ಸ್ಥಿರ bool append;

/* If true, ignore interrupts. */
ಸ್ಥಿರ bool ignore_interrupts;

ಎಣಿಕೆ output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

ಸ್ಥಿರ ಎಣಿಕೆ output_error output_error;

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"append", no_ವಾದ, ಖಾಲಿ, 'a'},
  {"ignore-interrupts", no_ವಾದ, ಖಾಲಿ, 'i'},
  {"output-error", optional_ವಾದ, ಖಾಲಿ, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ output_error_ವಾದಗಳು[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ output_error ಸ್ಥಿರಾಂಕ output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_ವಾದಗಳು, output_error_types);

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... [FILE]...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Copy standard ಒಳಹರಿವು ಗಮ್ಯ each FILE, and also ಗಮ್ಯ standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append ಗಮ್ಯ the given FILEs, ಮಾಡು not overwrite\n\
"));
      oputs (_("\
  -i, --ignore-interrupts\n\
         ignore ಅಡಚಣೆ ಸಂಕೇತಗಳು\n\
"));
      oputs (_("\
  -p\n\
         ನಿರ್ವಹಿಸು ಒಳ a ಇನ್ನಷ್ಟು appropriate ಬಹುಲಕ with pipes\n\
"));
      oputs (_("\
      --output-error[=ಬಹುಲಕ]\n\
         ಗಣ_2 behavior on ಲಿಖ error.  See ಬಹುಲಕ ಕೆಳಗೆ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ಬಹುಲಕ determines behavior with ಲಿಖ errors on the outputs:\n\
  warn           diagnose errors writing ಗಮ್ಯ any output\n\
  warn-nopipe    diagnose errors writing ಗಮ್ಯ any output not a pipe\n\
  ನಿರ್ಗಮಿಸು           ನಿರ್ಗಮಿಸು on error writing ಗಮ್ಯ any output\n\
  ನಿರ್ಗಮಿಸು-nopipe    ನಿರ್ಗಮಿಸು on error writing ಗಮ್ಯ any output not a pipe\n\
The ಪೂರ್ವನಿಯೋಜಿತ ಬಹುಲಕ ಗಾಗಿ the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, ನಿರ್ಗಮಿಸು immediately ಆದರೆ all outputs become broken pipes.\n\
The ಪೂರ್ವನಿಯೋಜಿತ operation when --output-error is not specified, is ಗಮ್ಯ\n\
ನಿರ್ಗಮಿಸು immediately on error writing ಗಮ್ಯ a pipe, and diagnose errors\n\
writing ಗಮ್ಯ non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಂಕ optc;
  ಆಗ ((optc = getopt_long (argc, argv, "aip", long_options, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'a':
          append = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'i':
          ignore_interrupts = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'p':
          ಆದರೆ (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_ವಾದಗಳು, output_error_types);
          ಇಲ್ಲದಿದ್ದರೆ
            output_error = output_error_warn_nopipe;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (ignore_interrupts)
    ಸಂಕೇತ (SIGINT, SIG_IGN);

  ಆದರೆ (output_error != output_error_sigpipe)
    ಸಂಕೇತ (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_ಒಳಹರಿವು_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  ಆದರೆ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_ಮುಂದಿನ_ಹೊರಹೋಗು (ಪೂರ್ಣಾಂಕ *descriptors, ಪೂರ್ಣಾಂಕ nfiles, ಪೂರ್ಣಾಂಕ idx)
{
  ಗಾಗಿ (idx++; idx <= nfiles; idx++)
    ಆದರೆ (0 <= descriptors[idx])
      ಹಿಂತಿರುಗಿ idx;
  ಹಿಂತಿರುಗಿ -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

ಸ್ಥಿರ bool
fail_output (ಪೂರ್ಣಾಂಕ *descriptors, ಅಕ್ಷರ **files, ಪೂರ್ಣಾಂಕ i)
{
  ಪೂರ್ಣಾಂಕ w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  ಆದರೆ (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  ಹಿಂತಿರುಗಿ fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

ಸ್ಥಿರ bool
tee_files (ಪೂರ್ಣಾಂಕ nfiles, ಅಕ್ಷರ **files, bool pipe_check)
{
  size_t n_outputs = 0;
  ಪೂರ್ಣಾಂಕ *descriptors;
  bool *ಹೊರಹೋಗು_pollable IF_LINT ( = ಖಾಲಿ);
  ಅಕ್ಷರ ಬಫರ್[BUFSIZ];
  ssize_t ಬೈಟುಗಳು_read = 0;
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_ಹೊರಹೋಗು = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  ಪೂರ್ಣಾಂಕ flags = O_WRONLY | O_CREAT | O_ದ್ವಿಮಾನ | (append ? O_APPEND : O_TRUNC);

  xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDIN_FILENO, O_ದ್ವಿಮಾನ);
  xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDOUT_FILENO, O_ದ್ವಿಮಾನ);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, ಗಾತ್ರ *descriptors);
  ಆದರೆ (pipe_check)
    ಹೊರಹೋಗು_pollable = xnmalloc (nfiles + 1, ಗಾತ್ರ *ಹೊರಹೋಗು_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  ಆದರೆ (pipe_check)
    ಹೊರಹೋಗು_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, ಬಹುಲಕ_RW_UGO);
      ಆದರೆ (descriptors[i] < 0)
        {
          ಆದರೆ (pipe_check)
            ಹೊರಹೋಗು_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (pipe_check)
            ಹೊರಹೋಗು_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  ಆಗ (n_outputs)
    {
      ಆದರೆ (pipe_check && ಹೊರಹೋಗು_pollable[ಮೊದಲ_ಹೊರಹೋಗು])
        {
          /* Monitor for input, or errors on first valid output.  */
          ಪೂರ್ಣಾಂಕ err = iopoll (STDIN_FILENO, descriptors[ಮೊದಲ_ಹೊರಹೋಗು], true);

          /* Close the output if it became a broken pipe.  */
          ಆದರೆ (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              ಆದರೆ (fail_output (descriptors, files, ಮೊದಲ_ಹೊರಹೋಗು))
                ok = false;
              n_outputs--;
              ಮೊದಲ_ಹೊರಹೋಗು = get_ಮುಂದಿನ_ಹೊರಹೋಗು (descriptors, nfiles, ಮೊದಲ_ಹೊರಹೋಗು);
              ಮುಂದುವರಿ;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      ಬೈಟುಗಳು_read = ಪಠ (STDIN_FILENO, ಬಫರ್, ಗಾತ್ರ ಬಫರ್);
      ಆದರೆ (ಬೈಟುಗಳು_read < 0 && errno == EINTR)
        ಮುಂದುವರಿ;
      ಆದರೆ (ಬೈಟುಗಳು_read <= 0)
        ನಿಲ್ಲಿಸು;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i <= nfiles; i++)
        ಆದರೆ (0 <= descriptors[i]
            && ! write_wait (descriptors[i], ಬಫರ್, ಬೈಟುಗಳು_read))
          {
            ಆದರೆ (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            ಆದರೆ (i == ಮೊದಲ_ಹೊರಹೋಗು)
              ಮೊದಲ_ಹೊರಹೋಗು = get_ಮುಂದಿನ_ಹೊರಹೋಗು (descriptors, nfiles, ಮೊದಲ_ಹೊರಹೋಗು);
          }
    }

  ಆದರೆ (ಬೈಟುಗಳು_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i <= nfiles; i++)
    ಆದರೆ (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  ಬಿಡುಗಡೆ (descriptors);
  ಆದರೆ (pipe_check)
    ಬಿಡುಗಡೆ (ಹೊರಹೋಗು_pollable);

  ಹಿಂತಿರುಗಿ ok;
}
