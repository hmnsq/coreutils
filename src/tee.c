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

#include <config.h>
#include <sys/types.h>
#include <signal.h>
#include <getopt.h>

#include "system.h"
#include "argmatch.h"
#include "fadvise.h"
#include "fcntl--.h"
#include "iopoll.h"
#include "xbinary-io.h"
#include "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tee"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

static bool tee_files (puur_naa_nka nfiles, var_na **files, bool);

/* If true, append to output files rather than truncating them. */
static bool append;

/* If true, ignore interrupts. */
static bool ignore_interrupts;

ga_nanaa output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

static ga_nanaa output_error output_error;

static sa_nracanaa option const long_options[] =
{
  {"append", no_argument, NULL, 'a'},
  {"ignore-interrupts", no_argument, NULL, 'i'},
  {"output-error", optional_argument, NULL, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

static var_na const *const output_error_args[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", NULL
};
static ga_nanaa output_error const output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_args, output_error_types);

void
usage (puur_naa_nka status)
{
  ᨀᨘ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᨀᨘᨁ
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Copy standard input to each FILE, and also to standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append to the given FILEs, do not overwrite\n\
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
         set behavior on write error.  See MODE below\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
MODE determines behavior with write errors on the outputs:\n\
  warn           diagnose errors writing to any output\n\
  warn-nopipe    diagnose errors writing to any output not a pipe\n\
  exit           exit on error writing to any output\n\
  exit-nopipe    exit on error writing to any output not a pipe\n\
The default MODE for the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, exit immediately ᨀᨘ all outputs become broken pipes.\n\
The default operation when --output-error is not specified, is to\n\
exit immediately on error writing to a pipe, and diagnose errors\n\
writing to non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  puur_naa_nka optc;
  ᨀᨛ ((optc = getopt_long (argc, argv, "aip", long_options, NULL)) != -1)
    {
      switch (optc)
        {
        pak_sa 'a':
          append = true;
          bha_nga;

        pak_sa 'i':
          ignore_interrupts = true;
          bha_nga;

        pak_sa 'p':
          ᨀᨘ (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_args, output_error_types);
          ᨀᨘᨁ
            output_error = output_error_warn_nopipe;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  ᨀᨘ (ignore_interrupts)
    sa_nkeeta (SIGINT, SIG_IGN);

  ᨀᨘ (output_error != output_error_sigpipe)
    sa_nkeeta (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_input_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  ᨀᨘ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  ᨀᨘᨒ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_PURE
static puur_naa_nka
get_next_out (puur_naa_nka *descriptors, puur_naa_nka nfiles, puur_naa_nka idx)
{
  for (idx++; idx <= nfiles; idx++)
    ᨀᨘ (0 <= descriptors[idx])
      ᨀᨘᨒ idx;
  ᨀᨘᨒ -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

static bool
fail_output (puur_naa_nka *descriptors, var_na **files, puur_naa_nka i)
{
  puur_naa_nka w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  ᨀᨘ (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  ᨀᨘᨒ fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

static bool
tee_files (puur_naa_nka nfiles, var_na **files, bool pipe_check)
{
  size_t n_outputs = 0;
  puur_naa_nka *descriptors;
  bool *out_pollable IF_LINT ( = NULL);
  var_na buffer[BUFSIZ];
  ssize_t bytes_read = 0;
  puur_naa_nka first_out = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  puur_naa_nka flags = O_WRONLY | O_CREAT | O_BINARY | (append ? O_APPEND : O_TRUNC);

  xset_binary_mode (STDIN_FILENO, O_BINARY);
  xset_binary_mode (STDOUT_FILENO, O_BINARY);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, parimaa_na *descriptors);
  ᨀᨘ (pipe_check)
    out_pollable = xnmalloc (nfiles + 1, parimaa_na *out_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  ᨀᨘ (pipe_check)
    out_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  for (puur_naa_nka i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, MODE_RW_UGO);
      ᨀᨘ (descriptors[i] < 0)
        {
          ᨀᨘ (pipe_check)
            out_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      ᨀᨘᨁ
        {
          ᨀᨘ (pipe_check)
            out_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  ᨀᨛ (n_outputs)
    {
      ᨀᨘ (pipe_check && out_pollable[first_out])
        {
          /* Monitor for input, or errors on first valid output.  */
          puur_naa_nka err = iopoll (STDIN_FILENO, descriptors[first_out], true);

          /* Close the output if it became a broken pipe.  */
          ᨀᨘ (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              ᨀᨘ (fail_output (descriptors, files, first_out))
                ok = false;
              n_outputs--;
              first_out = get_next_out (descriptors, nfiles, first_out);
              continue;
            }
          ᨀᨘᨁ ᨀᨘ (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      bytes_read = read (STDIN_FILENO, buffer, parimaa_na buffer);
      ᨀᨘ (bytes_read < 0 && errno == EINTR)
        continue;
      ᨀᨘ (bytes_read <= 0)
        bha_nga;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      for (puur_naa_nka i = 0; i <= nfiles; i++)
        ᨀᨘ (0 <= descriptors[i]
            && ! write_wait (descriptors[i], buffer, bytes_read))
          {
            ᨀᨘ (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            ᨀᨘ (i == first_out)
              first_out = get_next_out (descriptors, nfiles, first_out);
          }
    }

  ᨀᨘ (bytes_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  for (puur_naa_nka i = 1; i <= nfiles; i++)
    ᨀᨘ (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  free (descriptors);
  ᨀᨘ (pipe_check)
    free (out_pollable);

  ᨀᨘᨒ ok;
}
