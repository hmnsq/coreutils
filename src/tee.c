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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <getopt.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "argmatch.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "fcntl--.h"
#અન્તર્ભાવ "iopoll.h"
#અન્તર્ભાવ "xbinary-io.h"
#અન્તર્ભાવ "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tee"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

સ્થિર bool tee_files (પૂર્ણાંક nfiles, અક્ષર **files, bool);

/* If true, append to output files rather than truncating them. */
સ્થિર bool append;

/* If true, ignore interrupts. */
સ્થિર bool ignore_interrupts;

ગણના output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

સ્થિર ગણના output_error output_error;

સ્થિર સંરચના option અચલ long_options[] =
{
  {"append", no_argument, ખાલી, 'a'},
  {"ignore-interrupts", no_argument, ખાલી, 'i'},
  {"output-error", optional_argument, ખાલી, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

સ્થિર અક્ષર અચલ *અચલ output_error_args[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", ખાલી
};
સ્થિર ગણના output_error અચલ output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_args, output_error_types);

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Copy standard આગત to each FILE, and also to standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append to the given FILEs, કરો not overwrite\n\
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
         set behavior on લિખ error.  See MODE below\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
MODE determines behavior with લિખ errors on the outputs:\n\
  warn           diagnose errors writing to any output\n\
  warn-nopipe    diagnose errors writing to any output not a pipe\n\
  exit           exit on error writing to any output\n\
  exit-nopipe    exit on error writing to any output not a pipe\n\
The મૂળભૂત MODE માટે the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, exit immediately જો all outputs become broken pipes.\n\
The મૂળભૂત operation when --output-error is not specified, is to\n\
exit immediately on error writing to a pipe, and diagnose errors\n\
writing to non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  પૂર્ણાંક optc;
  જ્યારે ((optc = getopt_long (argc, argv, "aip", long_options, ખાલી)) != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'a':
          append = true;
          તોડો;

        સ્થિતિ 'i':
          ignore_interrupts = true;
          તોડો;

        સ્થિતિ 'p':
          જો (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_args, output_error_types);
          નહીંતર
            output_error = output_error_warn_nopipe;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (ignore_interrupts)
    સંકેત (SIGINT, SIG_IGN);

  જો (output_error != output_error_sigpipe)
    સંકેત (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_આગત_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  જો (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_PURE
સ્થિર પૂર્ણાંક
get_next_out (પૂર્ણાંક *descriptors, પૂર્ણાંક nfiles, પૂર્ણાંક idx)
{
  માટે (idx++; idx <= nfiles; idx++)
    જો (0 <= descriptors[idx])
      પાછા idx;
  પાછા -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

સ્થિર bool
fail_output (પૂર્ણાંક *descriptors, અક્ષર **files, પૂર્ણાંક i)
{
  પૂર્ણાંક w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  જો (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  પાછા fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

સ્થિર bool
tee_files (પૂર્ણાંક nfiles, અક્ષર **files, bool pipe_check)
{
  size_t n_outputs = 0;
  પૂર્ણાંક *descriptors;
  bool *out_pollable IF_LINT ( = ખાલી);
  અક્ષર buffer[BUFSIZ];
  ssize_t bytes_read = 0;
  પૂર્ણાંક first_out = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  પૂર્ણાંક flags = O_WRONLY | O_CREAT | O_દ્વિઅંકી | (append ? O_APPEND : O_TRUNC);

  xset_દ્વિઅંકી_mode (STDIN_FILENO, O_દ્વિઅંકી);
  xset_દ્વિઅંકી_mode (STDOUT_FILENO, O_દ્વિઅંકી);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, કદ *descriptors);
  જો (pipe_check)
    out_pollable = xnmalloc (nfiles + 1, કદ *out_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  જો (pipe_check)
    out_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  માટે (પૂર્ણાંક i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, MODE_RW_UGO);
      જો (descriptors[i] < 0)
        {
          જો (pipe_check)
            out_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      નહીંતર
        {
          જો (pipe_check)
            out_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  જ્યારે (n_outputs)
    {
      જો (pipe_check && out_pollable[first_out])
        {
          /* Monitor for input, or errors on first valid output.  */
          પૂર્ણાંક err = iopoll (STDIN_FILENO, descriptors[first_out], true);

          /* Close the output if it became a broken pipe.  */
          જો (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              જો (fail_output (descriptors, files, first_out))
                ok = false;
              n_outputs--;
              first_out = get_next_out (descriptors, nfiles, first_out);
              ચાલુ;
            }
          નહીંતર જો (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      bytes_read = પઠ (STDIN_FILENO, buffer, કદ buffer);
      જો (bytes_read < 0 && errno == EINTR)
        ચાલુ;
      જો (bytes_read <= 0)
        તોડો;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      માટે (પૂર્ણાંક i = 0; i <= nfiles; i++)
        જો (0 <= descriptors[i]
            && ! write_wait (descriptors[i], buffer, bytes_read))
          {
            જો (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            જો (i == first_out)
              first_out = get_next_out (descriptors, nfiles, first_out);
          }
    }

  જો (bytes_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  માટે (પૂર્ણાંક i = 1; i <= nfiles; i++)
    જો (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  મુક્ત (descriptors);
  જો (pipe_check)
    મુક્ત (out_pollable);

  પાછા ok;
}
