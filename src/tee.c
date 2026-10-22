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
#परिभाषय क्रमादेशः_NAME "tee"

#परिभाषय AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

स्थिर bool tee_files (पूर्णाङ्क nfiles, वर्ण **files, bool);

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

स्थिर संरचना option ध्रुव long_options[] =
{
  {"append", no_तर्कः, रिक्त, 'a'},
  {"ignore-interrupts", no_तर्कः, रिक्त, 'i'},
  {"output-error", optional_तर्कः, रिक्त, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

स्थिर वर्ण ध्रुव *ध्रुव output_error_तर्काः[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", रिक्त
};
स्थिर गणना output_error ध्रुव output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_तर्काः, output_error_types);

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]... [FILE]...\n"), क्रमादेशः_name);
      fputs (_("\
Copy standard निवेशः गन्तव्यम् each FILE, and also गन्तव्यम् standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append गन्तव्यम् the given FILEs, कुरु not overwrite\n\
"));
      oputs (_("\
  -i, --ignore-interrupts\n\
         ignore व्यवधानम् सङ्केताः\n\
"));
      oputs (_("\
  -p\n\
         सञ्चालय अन्तर् a अधिकम् appropriate बहुलकम् with pipes\n\
"));
      oputs (_("\
      --output-error[=बहुलकम्]\n\
         समुच्चयः_2 behavior on लिख error.  See बहुलकम् अधः\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
बहुलकम् determines behavior with लिख errors on the outputs:\n\
  warn           diagnose errors writing गन्तव्यम् any output\n\
  warn-nopipe    diagnose errors writing गन्तव्यम् any output not a pipe\n\
  निर्गच्छ           निर्गच्छ on error writing गन्तव्यम् any output\n\
  निर्गच्छ-nopipe    निर्गच्छ on error writing गन्तव्यम् any output not a pipe\n\
The स्वभाव बहुलकम् प्रति the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, निर्गच्छ immediately यदि all outputs become broken pipes.\n\
The स्वभाव operation when --output-error is not specified, is गन्तव्यम्\n\
निर्गच्छ immediately on error writing गन्तव्यम् a pipe, and diagnose errors\n\
writing गन्तव्यम् non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  पूर्णाङ्क optc;
  यावत् ((optc = getopt_long (argc, argv, "aip", long_options, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'a':
          append = true;
          भङ्ग;

        पक्ष 'i':
          ignore_interrupts = true;
          भङ्ग;

        पक्ष 'p':
          यदि (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_तर्काः, output_error_types);
          अन्यथा
            output_error = output_error_warn_nopipe;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (ignore_interrupts)
    सङ्केत (SIGINT, SIG_IGN);

  यदि (output_error != output_error_sigpipe)
    सङ्केत (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_निवेशः_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  यदि (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_विशुद्धम्
स्थिर पूर्णाङ्क
get_अग्र_बहिर्गमनम् (पूर्णाङ्क *descriptors, पूर्णाङ्क nfiles, पूर्णाङ्क idx)
{
  प्रति (idx++; idx <= nfiles; idx++)
    यदि (0 <= descriptors[idx])
      प्रत्यागम idx;
  प्रत्यागम -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

स्थिर bool
fail_output (पूर्णाङ्क *descriptors, वर्ण **files, पूर्णाङ्क i)
{
  पूर्णाङ्क w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  यदि (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  प्रत्यागम fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

स्थिर bool
tee_files (पूर्णाङ्क nfiles, वर्ण **files, bool pipe_check)
{
  size_t n_outputs = 0;
  पूर्णाङ्क *descriptors;
  bool *बहिर्गमनम्_pollable IF_LINT ( = रिक्त);
  वर्ण बफर[BUFSIZ];
  ssize_t अष्टकानि_read = 0;
  पूर्णाङ्क first_बहिर्गमनम् = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  पूर्णाङ्क flags = O_WRONLY | O_CREAT | O_द्विमान | (append ? O_APPEND : O_TRUNC);

  xset_द्विमान_बहुलकम् (STDIN_FILENO, O_द्विमान);
  xset_द्विमान_बहुलकम् (STDOUT_FILENO, O_द्विमान);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, परिमाण *descriptors);
  यदि (pipe_check)
    बहिर्गमनम्_pollable = xnmalloc (nfiles + 1, परिमाण *बहिर्गमनम्_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  यदि (pipe_check)
    बहिर्गमनम्_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  प्रति (पूर्णाङ्क i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = उद्घाटय (files[i], flags, बहुलकम्_RW_UGO);
      यदि (descriptors[i] < 0)
        {
          यदि (pipe_check)
            बहिर्गमनम्_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      अन्यथा
        {
          यदि (pipe_check)
            बहिर्गमनम्_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  यावत् (n_outputs)
    {
      यदि (pipe_check && बहिर्गमनम्_pollable[first_बहिर्गमनम्])
        {
          /* Monitor for input, or errors on first valid output.  */
          पूर्णाङ्क err = iopoll (STDIN_FILENO, descriptors[first_बहिर्गमनम्], true);

          /* Close the output if it became a broken pipe.  */
          यदि (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              यदि (fail_output (descriptors, files, first_बहिर्गमनम्))
                ok = false;
              n_outputs--;
              first_बहिर्गमनम् = get_अग्र_बहिर्गमनम् (descriptors, nfiles, first_बहिर्गमनम्);
              अनुवर्त;
            }
          अन्यथा यदि (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      अष्टकानि_read = पठ (STDIN_FILENO, बफर, परिमाण बफर);
      यदि (अष्टकानि_read < 0 && errno == EINTR)
        अनुवर्त;
      यदि (अष्टकानि_read <= 0)
        भङ्ग;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      प्रति (पूर्णाङ्क i = 0; i <= nfiles; i++)
        यदि (0 <= descriptors[i]
            && ! write_wait (descriptors[i], बफर, अष्टकानि_read))
          {
            यदि (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            यदि (i == first_बहिर्गमनम्)
              first_बहिर्गमनम् = get_अग्र_बहिर्गमनम् (descriptors, nfiles, first_बहिर्गमनम्);
          }
    }

  यदि (अष्टकानि_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  प्रति (पूर्णाङ्क i = 1; i <= nfiles; i++)
    यदि (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  मुक्त (descriptors);
  यदि (pipe_check)
    मुक्त (बहिर्गमनम्_pollable);

  प्रत्यागम ok;
}
