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

#ضمن <config.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <getopt.h>

#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "fadvise.h"
#ضمن "fcntl--.h"
#ضمن "iopoll.h"
#ضمن "xbinary-io.h"
#ضمن "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tee"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ساكن bool tee_files (صحيح nfiles, حرف **files, bool);

/* If true, append to output files rather than truncating them. */
ساكن bool append;

/* If true, ignore interrupts. */
ساكن bool ignore_interrupts;

تعداد output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

ساكن تعداد output_error output_error;

ساكن بنية option ثابت long_options[] =
{
  {"append", no_argument, عدم, 'a'},
  {"ignore-interrupts", no_argument, عدم, 'i'},
  {"output-error", optional_argument, عدم, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن حرف ثابت *ثابت output_error_args[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", عدم
};
ساكن تعداد output_error ثابت output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_args, output_error_types);

فراغ
usage (صحيح status)
{
  𐤀𐤌 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Copy standard input to each FILE, and also to standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append to the given FILEs, 𐤐𐤏𐤋 not overwrite\n\
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
         set behavior on اكتب error.  See MODE below\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
MODE determines behavior with اكتب errors on the outputs:\n\
  warn           diagnose errors writing to any output\n\
  warn-nopipe    diagnose errors writing to any output not a pipe\n\
  exit           exit on error writing to any output\n\
  exit-nopipe    exit on error writing to any output not a pipe\n\
The افتراضي MODE لكل the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, exit immediately 𐤀𐤌 all outputs become broken pipes.\n\
The افتراضي operation when --output-error is not specified, is to\n\
exit immediately on error writing to a pipe, and diagnose errors\n\
writing to non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحيح
main (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحيح optc;
  طالما ((optc = getopt_long (argc, argv, "aip", long_options, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'a':
          append = true;
          اكسر;

        حالة 'i':
          ignore_interrupts = true;
          اكسر;

        حالة 'p':
          𐤀𐤌 (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_args, output_error_types);
          وإلا
            output_error = output_error_warn_nopipe;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  𐤀𐤌 (ignore_interrupts)
    إشارة (SIGINT, SIG_IGN);

  𐤀𐤌 (output_error != output_error_sigpipe)
    إشارة (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_input_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  𐤀𐤌 (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_PURE
ساكن صحيح
get_next_out (صحيح *descriptors, صحيح nfiles, صحيح idx)
{
  لكل (idx++; idx <= nfiles; idx++)
    𐤀𐤌 (0 <= descriptors[idx])
      أرجع idx;
  أرجع -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

ساكن bool
fail_output (صحيح *descriptors, حرف **files, صحيح i)
{
  صحيح w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  𐤀𐤌 (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  أرجع fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

ساكن bool
tee_files (صحيح nfiles, حرف **files, bool pipe_check)
{
  size_t n_outputs = 0;
  صحيح *descriptors;
  bool *out_pollable IF_LINT ( = عدم);
  حرف buffer[BUFSIZ];
  ssize_t bytes_read = 0;
  صحيح first_out = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  صحيح flags = O_WRONLY | O_CREAT | O_BINARY | (append ? O_APPEND : O_TRUNC);

  xset_binary_mode (STDIN_FILENO, O_BINARY);
  xset_binary_mode (STDOUT_FILENO, O_BINARY);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, حجم *descriptors);
  𐤀𐤌 (pipe_check)
    out_pollable = xnmalloc (nfiles + 1, حجم *out_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  𐤀𐤌 (pipe_check)
    out_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  لكل (صحيح i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, MODE_RW_UGO);
      𐤀𐤌 (descriptors[i] < 0)
        {
          𐤀𐤌 (pipe_check)
            out_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      وإلا
        {
          𐤀𐤌 (pipe_check)
            out_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  طالما (n_outputs)
    {
      𐤀𐤌 (pipe_check && out_pollable[first_out])
        {
          /* Monitor for input, or errors on first valid output.  */
          صحيح err = iopoll (STDIN_FILENO, descriptors[first_out], true);

          /* Close the output if it became a broken pipe.  */
          𐤀𐤌 (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              𐤀𐤌 (fail_output (descriptors, files, first_out))
                ok = false;
              n_outputs--;
              first_out = get_next_out (descriptors, nfiles, first_out);
              استمر;
            }
          وإلا 𐤀𐤌 (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      bytes_read = اقرأ (STDIN_FILENO, buffer, حجم buffer);
      𐤀𐤌 (bytes_read < 0 && errno == EINTR)
        استمر;
      𐤀𐤌 (bytes_read <= 0)
        اكسر;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      لكل (صحيح i = 0; i <= nfiles; i++)
        𐤀𐤌 (0 <= descriptors[i]
            && ! write_wait (descriptors[i], buffer, bytes_read))
          {
            𐤀𐤌 (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            𐤀𐤌 (i == first_out)
              first_out = get_next_out (descriptors, nfiles, first_out);
          }
    }

  𐤀𐤌 (bytes_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  لكل (صحيح i = 1; i <= nfiles; i++)
    𐤀𐤌 (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  حرر (descriptors);
  𐤀𐤌 (pipe_check)
    حرر (out_pollable);

  أرجع ok;
}
