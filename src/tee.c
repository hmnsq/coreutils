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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <getopt.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "argmatch.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "fcntl--.h"
#அந்தர்பாவ "iopoll.h"
#அந்தர்பாவ "xbinary-io.h"
#அந்தர்பாவ "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tee"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

நிலையான bool tee_files (முழு nfiles, எழுத்து **files, bool);

/* If true, append to output files rather than truncating them. */
நிலையான bool append;

/* If true, ignore interrupts. */
நிலையான bool ignore_interrupts;

எண்ணிக்கை output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

நிலையான எண்ணிக்கை output_error output_error;

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"append", no_argument, வெற்று, 'a'},
  {"ignore-interrupts", no_argument, வெற்று, 'i'},
  {"output-error", optional_argument, வெற்று, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

நிலையான எழுத்து மாறிலி *மாறிலி output_error_args[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", வெற்று
};
நிலையான எண்ணிக்கை output_error மாறிலி output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_args, output_error_types);

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Copy standard உள்ளீடு to each FILE, and also to standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append to the given FILEs, செய் not overwrite\n\
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
         set behavior on லிக error.  See MODE below\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
MODE determines behavior with லிக errors on the outputs:\n\
  warn           diagnose errors writing to any output\n\
  warn-nopipe    diagnose errors writing to any output not a pipe\n\
  exit           exit on error writing to any output\n\
  exit-nopipe    exit on error writing to any output not a pipe\n\
The இயல்பு MODE க்கு the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, exit immediately எனில் all outputs become broken pipes.\n\
The இயல்பு operation when --output-error is not specified, is to\n\
exit immediately on error writing to a pipe, and diagnose errors\n\
writing to non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  முழு optc;
  போது ((optc = getopt_long (argc, argv, "aip", long_options, வெற்று)) != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'a':
          append = true;
          நிறுத்து;

        நிலை 'i':
          ignore_interrupts = true;
          நிறுத்து;

        நிலை 'p':
          எனில் (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_args, output_error_types);
          இல்லையெனில்
            output_error = output_error_warn_nopipe;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (ignore_interrupts)
    சமிக்ஞை (SIGINT, SIG_IGN);

  எனில் (output_error != output_error_sigpipe)
    சமிக்ஞை (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_உள்ளீடு_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  எனில் (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_PURE
நிலையான முழு
get_next_out (முழு *descriptors, முழு nfiles, முழு idx)
{
  க்கு (idx++; idx <= nfiles; idx++)
    எனில் (0 <= descriptors[idx])
      திரும்பு idx;
  திரும்பு -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

நிலையான bool
fail_output (முழு *descriptors, எழுத்து **files, முழு i)
{
  முழு w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  எனில் (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  திரும்பு fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

நிலையான bool
tee_files (முழு nfiles, எழுத்து **files, bool pipe_check)
{
  size_t n_outputs = 0;
  முழு *descriptors;
  bool *out_pollable IF_LINT ( = வெற்று);
  எழுத்து buffer[BUFSIZ];
  ssize_t bytes_read = 0;
  முழு first_out = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  முழு flags = O_WRONLY | O_CREAT | O_இருமம் | (append ? O_APPEND : O_TRUNC);

  xset_இருமம்_mode (STDIN_FILENO, O_இருமம்);
  xset_இருமம்_mode (STDOUT_FILENO, O_இருமம்);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, அளவு *descriptors);
  எனில் (pipe_check)
    out_pollable = xnmalloc (nfiles + 1, அளவு *out_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  எனில் (pipe_check)
    out_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  க்கு (முழு i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, MODE_RW_UGO);
      எனில் (descriptors[i] < 0)
        {
          எனில் (pipe_check)
            out_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      இல்லையெனில்
        {
          எனில் (pipe_check)
            out_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  போது (n_outputs)
    {
      எனில் (pipe_check && out_pollable[first_out])
        {
          /* Monitor for input, or errors on first valid output.  */
          முழு err = iopoll (STDIN_FILENO, descriptors[first_out], true);

          /* Close the output if it became a broken pipe.  */
          எனில் (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              எனில் (fail_output (descriptors, files, first_out))
                ok = false;
              n_outputs--;
              first_out = get_next_out (descriptors, nfiles, first_out);
              தொடர்;
            }
          இல்லையெனில் எனில் (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      bytes_read = பட (STDIN_FILENO, buffer, அளவு buffer);
      எனில் (bytes_read < 0 && errno == EINTR)
        தொடர்;
      எனில் (bytes_read <= 0)
        நிறுத்து;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      க்கு (முழு i = 0; i <= nfiles; i++)
        எனில் (0 <= descriptors[i]
            && ! write_wait (descriptors[i], buffer, bytes_read))
          {
            எனில் (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            எனில் (i == first_out)
              first_out = get_next_out (descriptors, nfiles, first_out);
          }
    }

  எனில் (bytes_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  க்கு (முழு i = 1; i <= nfiles; i++)
    எனில் (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  விடுவி (descriptors);
  எனில் (pipe_check)
    விடுவி (out_pollable);

  திரும்பு ok;
}
