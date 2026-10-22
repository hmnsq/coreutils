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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <getopt.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "argmatch.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "fcntl--.h"
#അന്തര്ഭാവ "iopoll.h"
#അന്തര്ഭാവ "xbinary-io.h"
#അന്തര്ഭാവ "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tee"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

സ്ഥിതം bool tee_files (പൂർണ്ണസംഖ്യ nfiles, അക്ഷരം **files, bool);

/* If true, append to output files rather than truncating them. */
സ്ഥിതം bool append;

/* If true, ignore interrupts. */
സ്ഥിതം bool ignore_interrupts;

എണ്ണൽ output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

സ്ഥിതം എണ്ണൽ output_error output_error;

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"append", no_argument, ശൂന്യ, 'a'},
  {"ignore-interrupts", no_argument, ശൂന്യ, 'i'},
  {"output-error", optional_argument, ശൂന്യ, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം output_error_args[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", ശൂന്യ
};
സ്ഥിതം എണ്ണൽ output_error സ്ഥിരം output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_args, output_error_types);

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Copy standard നിവേശം to each FILE, and also to standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append to the given FILEs, ചെയ്യുക not overwrite\n\
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
         set behavior on ലിഖ error.  See MODE below\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
MODE determines behavior with ലിഖ errors on the outputs:\n\
  warn           diagnose errors writing to any output\n\
  warn-nopipe    diagnose errors writing to any output not a pipe\n\
  exit           exit on error writing to any output\n\
  exit-nopipe    exit on error writing to any output not a pipe\n\
The സ്ഥിരസ്ഥിതി MODE വേണ്ടി the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, exit immediately എങ്കിൽ all outputs become broken pipes.\n\
The സ്ഥിരസ്ഥിതി operation when --output-error is not specified, is to\n\
exit immediately on error writing to a pipe, and diagnose errors\n\
writing to non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  പൂർണ്ണസംഖ്യ optc;
  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "aip", long_options, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'a':
          append = true;
          നിർത്തുക;

        അവസ്ഥ 'i':
          ignore_interrupts = true;
          നിർത്തുക;

        അവസ്ഥ 'p':
          എങ്കിൽ (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_args, output_error_types);
          അല്ലെങ്കിൽ
            output_error = output_error_warn_nopipe;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (ignore_interrupts)
    സൂചന (SIGINT, SIG_IGN);

  എങ്കിൽ (output_error != output_error_sigpipe)
    സൂചന (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_നിവേശം_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  എങ്കിൽ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_PURE
സ്ഥിതം പൂർണ്ണസംഖ്യ
get_next_out (പൂർണ്ണസംഖ്യ *descriptors, പൂർണ്ണസംഖ്യ nfiles, പൂർണ്ണസംഖ്യ idx)
{
  വേണ്ടി (idx++; idx <= nfiles; idx++)
    എങ്കിൽ (0 <= descriptors[idx])
      തിരികെ idx;
  തിരികെ -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

സ്ഥിതം bool
fail_output (പൂർണ്ണസംഖ്യ *descriptors, അക്ഷരം **files, പൂർണ്ണസംഖ്യ i)
{
  പൂർണ്ണസംഖ്യ w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  എങ്കിൽ (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  തിരികെ fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

സ്ഥിതം bool
tee_files (പൂർണ്ണസംഖ്യ nfiles, അക്ഷരം **files, bool pipe_check)
{
  size_t n_outputs = 0;
  പൂർണ്ണസംഖ്യ *descriptors;
  bool *out_pollable IF_LINT ( = ശൂന്യ);
  അക്ഷരം buffer[BUFSIZ];
  ssize_t bytes_read = 0;
  പൂർണ്ണസംഖ്യ first_out = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  പൂർണ്ണസംഖ്യ flags = O_WRONLY | O_CREAT | O_ദ്വിമാന | (append ? O_APPEND : O_TRUNC);

  xset_ദ്വിമാന_mode (STDIN_FILENO, O_ദ്വിമാന);
  xset_ദ്വിമാന_mode (STDOUT_FILENO, O_ദ്വിമാന);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, വലുപ്പം *descriptors);
  എങ്കിൽ (pipe_check)
    out_pollable = xnmalloc (nfiles + 1, വലുപ്പം *out_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  എങ്കിൽ (pipe_check)
    out_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, MODE_RW_UGO);
      എങ്കിൽ (descriptors[i] < 0)
        {
          എങ്കിൽ (pipe_check)
            out_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (pipe_check)
            out_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  ആകുമ്പോൾ (n_outputs)
    {
      എങ്കിൽ (pipe_check && out_pollable[first_out])
        {
          /* Monitor for input, or errors on first valid output.  */
          പൂർണ്ണസംഖ്യ err = iopoll (STDIN_FILENO, descriptors[first_out], true);

          /* Close the output if it became a broken pipe.  */
          എങ്കിൽ (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              എങ്കിൽ (fail_output (descriptors, files, first_out))
                ok = false;
              n_outputs--;
              first_out = get_next_out (descriptors, nfiles, first_out);
              തുടരുക;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      bytes_read = പഠ (STDIN_FILENO, buffer, വലുപ്പം buffer);
      എങ്കിൽ (bytes_read < 0 && errno == EINTR)
        തുടരുക;
      എങ്കിൽ (bytes_read <= 0)
        നിർത്തുക;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i <= nfiles; i++)
        എങ്കിൽ (0 <= descriptors[i]
            && ! write_wait (descriptors[i], buffer, bytes_read))
          {
            എങ്കിൽ (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            എങ്കിൽ (i == first_out)
              first_out = get_next_out (descriptors, nfiles, first_out);
          }
    }

  എങ്കിൽ (bytes_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i <= nfiles; i++)
    എങ്കിൽ (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  സ്വതന്ത്രം (descriptors);
  എങ്കിൽ (pipe_check)
    സ്വതന്ത്രം (out_pollable);

  തിരികെ ok;
}
