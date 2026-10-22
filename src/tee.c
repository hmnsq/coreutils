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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fcntl--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "iopoll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "tee"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ਸਥਿਰ bool tee_files (ਪੂਰਨ nfiles, ਅੱਖਰ **files, bool);

/* If true, append to output files rather than truncating them. */
ਸਥਿਰ bool append;

/* If true, ignore interrupts. */
ਸਥਿਰ bool ignore_interrupts;

ਗਿਣਤੀ output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

ਸਥਿਰ ਗਿਣਤੀ output_error output_error;

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"append", no_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"ignore-interrupts", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"output-error", optional_ਦਲੀਲ, ਨਿਰਾ, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ output_error_ਦਲੀਲਾਂ[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ output_error ਅਚਲ output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_ਦਲੀਲਾਂ, output_error_types);

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... [FILE]...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Copy standard ਨਿਵੇਸ਼ ਮੰਜ਼ਿਲ each FILE, and also ਮੰਜ਼ਿਲ standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append ਮੰਜ਼ਿਲ the given FILEs, ਕਰੋ not overwrite\n\
"));
      oputs (_("\
  -i, --ignore-interrupts\n\
         ignore ਰੁਕਾਵਟ ਸੰਕੇਤ_2\n\
"));
      oputs (_("\
  -p\n\
         ਚਲਾਓ ਵਿੱਚ a ਹੋਰ appropriate ਬਹੁਲਕ with pipes\n\
"));
      oputs (_("\
      --output-error[=ਬਹੁਲਕ]\n\
         ਸੈੱਟ_2 behavior on ਲਿਖ error.  See ਬਹੁਲਕ ਹੇਠਾਂ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ਬਹੁਲਕ determines behavior with ਲਿਖ errors on the outputs:\n\
  warn           diagnose errors writing ਮੰਜ਼ਿਲ any output\n\
  warn-nopipe    diagnose errors writing ਮੰਜ਼ਿਲ any output not a pipe\n\
  ਬਾਹਰ_ਨਿਕਲੋ           ਬਾਹਰ_ਨਿਕਲੋ on error writing ਮੰਜ਼ਿਲ any output\n\
  ਬਾਹਰ_ਨਿਕਲੋ-nopipe    ਬਾਹਰ_ਨਿਕਲੋ on error writing ਮੰਜ਼ਿਲ any output not a pipe\n\
The ਮੂਲ ਬਹੁਲਕ ਲਈ the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, ਬਾਹਰ_ਨਿਕਲੋ immediately ਜੇ all outputs become broken pipes.\n\
The ਮੂਲ operation when --output-error is not specified, is ਮੰਜ਼ਿਲ\n\
ਬਾਹਰ_ਨਿਕਲੋ immediately on error writing ਮੰਜ਼ਿਲ a pipe, and diagnose errors\n\
writing ਮੰਜ਼ਿਲ non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਪੂਰਨ optc;
  ਜਦੋਂ ((optc = getopt_long (argc, argv, "aip", long_options, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'a':
          append = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'i':
          ignore_interrupts = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'p':
          ਜੇ (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_ਦਲੀਲਾਂ, output_error_types);
          ਨਹੀਂਤਾਂ
            output_error = output_error_warn_nopipe;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (ignore_interrupts)
    ਸੰਕੇਤ (SIGINT, SIG_IGN);

  ਜੇ (output_error != output_error_sigpipe)
    ਸੰਕੇਤ (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_ਨਿਵੇਸ਼_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  ਜੇ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਪੂਰਨ
get_ਅਗਲਾ_ਬਾਹਰਲਾ (ਪੂਰਨ *descriptors, ਪੂਰਨ nfiles, ਪੂਰਨ idx)
{
  ਲਈ (idx++; idx <= nfiles; idx++)
    ਜੇ (0 <= descriptors[idx])
      ਵਾਪਸ idx;
  ਵਾਪਸ -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

ਸਥਿਰ bool
fail_output (ਪੂਰਨ *descriptors, ਅੱਖਰ **files, ਪੂਰਨ i)
{
  ਪੂਰਨ w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  ਜੇ (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  ਵਾਪਸ fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

ਸਥਿਰ bool
tee_files (ਪੂਰਨ nfiles, ਅੱਖਰ **files, bool pipe_check)
{
  size_t n_outputs = 0;
  ਪੂਰਨ *descriptors;
  bool *ਬਾਹਰਲਾ_pollable IF_LINT ( = ਨਿਰਾ);
  ਅੱਖਰ ਬਫਰ[BUFSIZ];
  ssize_t ਬਾਈਟਾਂ_read = 0;
  ਪੂਰਨ ਪਹਿਲਾ_ਬਾਹਰਲਾ = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  ਪੂਰਨ flags = O_WRONLY | O_CREAT | O_ਦੁਆਧਾਰੀ | (append ? O_APPEND : O_TRUNC);

  xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDIN_FILENO, O_ਦੁਆਧਾਰੀ);
  xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDOUT_FILENO, O_ਦੁਆਧਾਰੀ);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, ਆਕਾਰ *descriptors);
  ਜੇ (pipe_check)
    ਬਾਹਰਲਾ_pollable = xnmalloc (nfiles + 1, ਆਕਾਰ *ਬਾਹਰਲਾ_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  ਜੇ (pipe_check)
    ਬਾਹਰਲਾ_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  ਲਈ (ਪੂਰਨ i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, ਬਹੁਲਕ_RW_UGO);
      ਜੇ (descriptors[i] < 0)
        {
          ਜੇ (pipe_check)
            ਬਾਹਰਲਾ_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (pipe_check)
            ਬਾਹਰਲਾ_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  ਜਦੋਂ (n_outputs)
    {
      ਜੇ (pipe_check && ਬਾਹਰਲਾ_pollable[ਪਹਿਲਾ_ਬਾਹਰਲਾ])
        {
          /* Monitor for input, or errors on first valid output.  */
          ਪੂਰਨ err = iopoll (STDIN_FILENO, descriptors[ਪਹਿਲਾ_ਬਾਹਰਲਾ], true);

          /* Close the output if it became a broken pipe.  */
          ਜੇ (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              ਜੇ (fail_output (descriptors, files, ਪਹਿਲਾ_ਬਾਹਰਲਾ))
                ok = false;
              n_outputs--;
              ਪਹਿਲਾ_ਬਾਹਰਲਾ = get_ਅਗਲਾ_ਬਾਹਰਲਾ (descriptors, nfiles, ਪਹਿਲਾ_ਬਾਹਰਲਾ);
              ਜਾਰੀ;
            }
          ਨਹੀਂਤਾਂ ਜੇ (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      ਬਾਈਟਾਂ_read = ਪਠ (STDIN_FILENO, ਬਫਰ, ਆਕਾਰ ਬਫਰ);
      ਜੇ (ਬਾਈਟਾਂ_read < 0 && errno == EINTR)
        ਜਾਰੀ;
      ਜੇ (ਬਾਈਟਾਂ_read <= 0)
        ਰੋਕੋ;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      ਲਈ (ਪੂਰਨ i = 0; i <= nfiles; i++)
        ਜੇ (0 <= descriptors[i]
            && ! write_wait (descriptors[i], ਬਫਰ, ਬਾਈਟਾਂ_read))
          {
            ਜੇ (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            ਜੇ (i == ਪਹਿਲਾ_ਬਾਹਰਲਾ)
              ਪਹਿਲਾ_ਬਾਹਰਲਾ = get_ਅਗਲਾ_ਬਾਹਰਲਾ (descriptors, nfiles, ਪਹਿਲਾ_ਬਾਹਰਲਾ);
          }
    }

  ਜੇ (ਬਾਈਟਾਂ_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  ਲਈ (ਪੂਰਨ i = 1; i <= nfiles; i++)
    ਜੇ (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  ਮੁਕਤ (descriptors);
  ਜੇ (pipe_check)
    ਮੁਕਤ (ਬਾਹਰਲਾ_pollable);

  ਵਾਪਸ ok;
}
