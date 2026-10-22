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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "argmatch.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "fcntl--.h"
#ଅନ୍ତର୍ଭାଵ "iopoll.h"
#ଅନ୍ତର୍ଭାଵ "xbinary-io.h"
#ଅନ୍ତର୍ଭାଵ "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tee"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ସ୍ଥିର bool tee_files (ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfiles, ଅକ୍ଷର **files, bool);

/* If true, append to output files rather than truncating them. */
ସ୍ଥିର bool append;

/* If true, ignore interrupts. */
ସ୍ଥିର bool ignore_interrupts;

ଗଣନା output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

ସ୍ଥିର ଗଣନା output_error output_error;

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"append", no_argument, ଖାଲି, 'a'},
  {"ignore-interrupts", no_argument, ଖାଲି, 'i'},
  {"output-error", optional_argument, ଖାଲି, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ output_error_args[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", ଖାଲି
};
ସ୍ଥିର ଗଣନା output_error ଅଚଳ output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_args, output_error_types);

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Copy standard ନିବେଶ to each FILE, and also to standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append to the given FILEs, କର not overwrite\n\
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
         set behavior on ଲିଖ error.  See MODE below\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
MODE determines behavior with ଲିଖ errors on the outputs:\n\
  warn           diagnose errors writing to any output\n\
  warn-nopipe    diagnose errors writing to any output not a pipe\n\
  exit           exit on error writing to any output\n\
  exit-nopipe    exit on error writing to any output not a pipe\n\
The ପୂର୍ବନିର୍ଦ୍ଧାରିତ MODE ପାଇଁ the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, exit immediately ଯଦି all outputs become broken pipes.\n\
The ପୂର୍ବନିର୍ଦ୍ଧାରିତ operation when --output-error is not specified, is to\n\
exit immediately on error writing to a pipe, and diagnose errors\n\
writing to non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "aip", long_options, ଖାଲି)) != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'a':
          append = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'i':
          ignore_interrupts = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'p':
          ଯଦି (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_args, output_error_types);
          ଅନ୍ୟଥା
            output_error = output_error_warn_nopipe;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (ignore_interrupts)
    ସଙ୍କେତ (SIGINT, SIG_IGN);

  ଯଦି (output_error != output_error_sigpipe)
    ସଙ୍କେତ (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_ନିବେଶ_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  ଯଦି (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_PURE
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_next_out (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *descriptors, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfiles, ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx)
{
  ପାଇଁ (idx++; idx <= nfiles; idx++)
    ଯଦି (0 <= descriptors[idx])
      ଫେରନ୍ତୁ idx;
  ଫେରନ୍ତୁ -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

ସ୍ଥିର bool
fail_output (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *descriptors, ଅକ୍ଷର **files, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  ଯଦି (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  ଫେରନ୍ତୁ fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

ସ୍ଥିର bool
tee_files (ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfiles, ଅକ୍ଷର **files, bool pipe_check)
{
  size_t n_outputs = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *descriptors;
  bool *out_pollable IF_LINT ( = ଖାଲି);
  ଅକ୍ଷର buffer[BUFSIZ];
  ssize_t bytes_read = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_out = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = O_WRONLY | O_CREAT | O_ଦ୍ୱିମିକ | (append ? O_APPEND : O_TRUNC);

  xset_ଦ୍ୱିମିକ_mode (STDIN_FILENO, O_ଦ୍ୱିମିକ);
  xset_ଦ୍ୱିମିକ_mode (STDOUT_FILENO, O_ଦ୍ୱିମିକ);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, ଆକାର *descriptors);
  ଯଦି (pipe_check)
    out_pollable = xnmalloc (nfiles + 1, ଆକାର *out_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  ଯଦି (pipe_check)
    out_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, MODE_RW_UGO);
      ଯଦି (descriptors[i] < 0)
        {
          ଯଦି (pipe_check)
            out_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (pipe_check)
            out_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  ଯେତେବେଳେ (n_outputs)
    {
      ଯଦି (pipe_check && out_pollable[first_out])
        {
          /* Monitor for input, or errors on first valid output.  */
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = iopoll (STDIN_FILENO, descriptors[first_out], true);

          /* Close the output if it became a broken pipe.  */
          ଯଦି (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              ଯଦି (fail_output (descriptors, files, first_out))
                ok = false;
              n_outputs--;
              first_out = get_next_out (descriptors, nfiles, first_out);
              ଜାରି;
            }
          ଅନ୍ୟଥା ଯଦି (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      bytes_read = ପଠ (STDIN_FILENO, buffer, ଆକାର buffer);
      ଯଦି (bytes_read < 0 && errno == EINTR)
        ଜାରି;
      ଯଦି (bytes_read <= 0)
        ଭାଙ୍ଗ;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i <= nfiles; i++)
        ଯଦି (0 <= descriptors[i]
            && ! write_wait (descriptors[i], buffer, bytes_read))
          {
            ଯଦି (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            ଯଦି (i == first_out)
              first_out = get_next_out (descriptors, nfiles, first_out);
          }
    }

  ଯଦି (bytes_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i <= nfiles; i++)
    ଯଦି (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  ମୁକ୍ତ (descriptors);
  ଯଦି (pipe_check)
    ମୁକ୍ତ (out_pollable);

  ଫେରନ୍ତୁ ok;
}
