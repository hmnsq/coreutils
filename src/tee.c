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
#وصف_ڪريو پروگرام_NAME "tee"

#وصف_ڪريو AUTHORS \
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
  {"append", no_دليل, عدم, 'a'},
  {"ignore-interrupts", no_دليل, عدم, 'i'},
  {"output-error", optional_دليل, عدم, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن حرف ثابت *ثابت output_error_دليلون[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", عدم
};
ساكن تعداد output_error ثابت output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_دليلون, output_error_types);

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("Usage: %s [OPTION]... [FILE]...\n"), پروگرام_name);
      fputs (_("\
Copy standard داخلا منزل each FILE, and also منزل standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append منزل the given FILEs, افعل not overwrite\n\
"));
      oputs (_("\
  -i, --ignore-interrupts\n\
         ignore مداخلت سگنل\n\
"));
      oputs (_("\
  -p\n\
         هلايو اندريون a وڌيڪ appropriate گهڻائي with pipes\n\
"));
      oputs (_("\
      --output-error[=گهڻائي]\n\
         سيٽ_2 behavior on اكتب error.  See گهڻائي هيٺ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\
\n\
گهڻائي determines behavior with اكتب errors on the outputs:\n\
  warn           diagnose errors writing منزل any output\n\
  warn-nopipe    diagnose errors writing منزل any output not a pipe\n\
  ٻاهر_نڪرو           ٻاهر_نڪرو on error writing منزل any output\n\
  ٻاهر_نڪرو-nopipe    ٻاهر_نڪرو on error writing منزل any output not a pipe\n\
The افتراضي گهڻائي لكل the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, ٻاهر_نڪرو immediately إذا all outputs become broken pipes.\n\
The افتراضي operation when --output-error is not specified, is منزل\n\
ٻاهر_نڪرو immediately on error writing منزل a pipe, and diagnose errors\n\
writing منزل non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
          إذا (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_دليلون, output_error_types);
          وإلا
            output_error = output_error_warn_nopipe;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (ignore_interrupts)
    إشارة (SIGINT, SIG_IGN);

  إذا (output_error != output_error_sigpipe)
    إشارة (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_داخلا_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  إذا (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_خالص
ساكن صحيح
get_ايندڙ_ٻاهريون (صحيح *descriptors, صحيح nfiles, صحيح idx)
{
  لكل (idx++; idx <= nfiles; idx++)
    إذا (0 <= descriptors[idx])
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
  إذا (fail)
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
  bool *ٻاهريون_pollable IF_LINT ( = عدم);
  حرف بفر[BUFSIZ];
  ssize_t بائيٽون_read = 0;
  صحيح پهريون_ٻاهريون = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  صحيح flags = O_WRONLY | O_CREAT | O_ٻٽو | (append ? O_APPEND : O_TRUNC);

  xset_ٻٽو_گهڻائي (STDIN_FILENO, O_ٻٽو);
  xset_ٻٽو_گهڻائي (STDOUT_FILENO, O_ٻٽو);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, حجم *descriptors);
  إذا (pipe_check)
    ٻاهريون_pollable = xnmalloc (nfiles + 1, حجم *ٻاهريون_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  إذا (pipe_check)
    ٻاهريون_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  لكل (صحيح i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, گهڻائي_RW_UGO);
      إذا (descriptors[i] < 0)
        {
          إذا (pipe_check)
            ٻاهريون_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      وإلا
        {
          إذا (pipe_check)
            ٻاهريون_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  طالما (n_outputs)
    {
      إذا (pipe_check && ٻاهريون_pollable[پهريون_ٻاهريون])
        {
          /* Monitor for input, or errors on first valid output.  */
          صحيح err = iopoll (STDIN_FILENO, descriptors[پهريون_ٻاهريون], true);

          /* Close the output if it became a broken pipe.  */
          إذا (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              إذا (fail_output (descriptors, files, پهريون_ٻاهريون))
                ok = false;
              n_outputs--;
              پهريون_ٻاهريون = get_ايندڙ_ٻاهريون (descriptors, nfiles, پهريون_ٻاهريون);
              استمر;
            }
          وإلا إذا (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      بائيٽون_read = اقرأ (STDIN_FILENO, بفر, حجم بفر);
      إذا (بائيٽون_read < 0 && errno == EINTR)
        استمر;
      إذا (بائيٽون_read <= 0)
        اكسر;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      لكل (صحيح i = 0; i <= nfiles; i++)
        إذا (0 <= descriptors[i]
            && ! write_wait (descriptors[i], بفر, بائيٽون_read))
          {
            إذا (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            إذا (i == پهريون_ٻاهريون)
              پهريون_ٻاهريون = get_ايندڙ_ٻاهريون (descriptors, nfiles, پهريون_ٻاهريون);
          }
    }

  إذا (بائيٽون_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  لكل (صحيح i = 1; i <= nfiles; i++)
    إذا (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  حرر (descriptors);
  إذا (pipe_check)
    حرر (ٻاهريون_pollable);

  أرجع ok;
}
