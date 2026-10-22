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

#अन्तर्भाव <config.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "fcntl--.h"
#अन्तर्भाव "iopoll.h"
#अन्तर्भाव "xbinary-io.h"
#अन्तर्भाव "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tee"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

स्थिर bool tee_files (पूर्णांक nfiles, अक्षर **files, bool);

/* If true, append to output files rather than truncating them. */
स्थिर bool append;

/* If true, ignore interrupts. */
स्थिर bool ignore_interrupts;

गणना output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

स्थिर गणना output_error output_error;

स्थिर संरचना option स्थिरांक long_options[] =
{
  {"append", no_argument, रिक्त, 'a'},
  {"ignore-interrupts", no_argument, रिक्त, 'i'},
  {"output-error", optional_argument, रिक्त, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

स्थिर अक्षर स्थिरांक *स्थिरांक output_error_args[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", रिक्त
};
स्थिर गणना output_error स्थिरांक output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_args, output_error_types);

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      ख_माला_दो (_("\
Copy standard आदान to each खाता, and also to standard output.\n\
\n\
"), मानक_निकास);
      oputs (_("\
  -a, --append\n\
         append to the given FILEs, करा not overwrite\n\
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
         set behavior on लिख error.  See MODE below\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
MODE determines behavior with लिख errors on the outputs:\n\
  warn           diagnose errors writing to any output\n\
  warn-nopipe    diagnose errors writing to any output not a pipe\n\
  निकास           निकास on error writing to any output\n\
  निकास-nopipe    निकास on error writing to any output not a pipe\n\
The पूर्वनिर्धारित MODE साठी the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, निकास immediately जर all outputs become broken pipes.\n\
The पूर्वनिर्धारित operation when --output-error is not specified, is to\n\
निकास immediately on error writing to a pipe, and diagnose errors\n\
writing to non pipe outputs.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णांक optc;
  पर्यंत ((optc = getopt_long (argc, argv, "aip", long_options, रिक्त)) != -1)
    {
      निवड (optc)
        {
        स्थिती 'a':
          append = true;
          थांबा;

        स्थिती 'i':
          ignore_interrupts = true;
          थांबा;

        स्थिती 'p':
          जर (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_args, output_error_types);
          अन्यथा
            output_error = output_error_warn_nopipe;
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (ignore_interrupts)
    संकेत (संक_विघ्न, संक_छोड़ो);

  जर (output_error != output_error_sigpipe)
    संकेत (SIGPIPE, संक_छोड़ो);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_आदान_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  जर (बंद (STDIN_FILENO) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", _("standard input"));

  परत ok ? निकास_सफल : निकास_त्रुटि;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_PURE
स्थिर पूर्णांक
get_next_out (पूर्णांक *descriptors, पूर्णांक nfiles, पूर्णांक idx)
{
  साठी (idx++; idx <= nfiles; idx++)
    जर (0 <= descriptors[idx])
      परत idx;
  परत -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

स्थिर bool
fail_output (पूर्णांक *descriptors, अक्षर **files, पूर्णांक i)
{
  पूर्णांक w_errno = त्रुटि_सं;
  bool fail = त्रुटि_सं != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  जर (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  परत fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

स्थिर bool
tee_files (पूर्णांक nfiles, अक्षर **files, bool pipe_check)
{
  माप_प्रकार n_outputs = 0;
  पूर्णांक *descriptors;
  bool *out_pollable IF_LINT ( = रिक्त);
  अक्षर buffer[बफ_मान];
  ssize_t bytes_read = 0;
  पूर्णांक first_out = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  पूर्णांक flags = O_WRONLY | O_CREAT | O_द्विमान | (append ? O_APPEND : O_TRUNC);

  xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
  xset_द्विमान_mode (STDOUT_FILENO, O_द्विमान);
  fadvise (मानक_निवेश, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, आकार *descriptors);
  जर (pipe_check)
    out_pollable = xnmalloc (nfiles + 1, आकार *out_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  जर (pipe_check)
    out_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  साठी (पूर्णांक i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = खोलो (files[i], flags, MODE_RW_UGO);
      जर (descriptors[i] < 0)
        {
          जर (pipe_check)
            out_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 त्रुटि_सं, "%s", quotef (files[i]));
          ok = false;
        }
      अन्यथा
        {
          जर (pipe_check)
            out_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  पर्यंत (n_outputs)
    {
      जर (pipe_check && out_pollable[first_out])
        {
          /* Monitor for input, or errors on first valid output.  */
          पूर्णांक err = iopoll (STDIN_FILENO, descriptors[first_out], true);

          /* Close the output if it became a broken pipe.  */
          जर (err == IOPOLL_BROKEN_OUTPUT)
            {
              त्रुटि_सं = EPIPE;  /* behave like write produced EPIPE */
              जर (fail_output (descriptors, files, first_out))
                ok = false;
              n_outputs--;
              first_out = get_next_out (descriptors, nfiles, first_out);
              सुरूठेवा;
            }
          अन्यथा जर (err == IOPOLL_ERROR)
            {
              error (0, त्रुटि_सं, _("iopoll error"));
              ok = false;
            }
        }

      bytes_read = पठ (STDIN_FILENO, buffer, आकार buffer);
      जर (bytes_read < 0 && त्रुटि_सं == EINTR)
        सुरूठेवा;
      जर (bytes_read <= 0)
        थांबा;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      साठी (पूर्णांक i = 0; i <= nfiles; i++)
        जर (0 <= descriptors[i]
            && ! write_wait (descriptors[i], buffer, bytes_read))
          {
            जर (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            जर (i == first_out)
              first_out = get_next_out (descriptors, nfiles, first_out);
          }
    }

  जर (bytes_read == -1)
    {
      error (0, त्रुटि_सं, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  साठी (पूर्णांक i = 1; i <= nfiles; i++)
    जर (0 <= descriptors[i] && बंद (descriptors[i]) < 0)
      {
        error (0, त्रुटि_सं, "%s", quotef (files[i]));
        ok = false;
      }

  मुक्त (descriptors);
  जर (pipe_check)
    मुक्त (out_pollable);

  परत ok;
}
