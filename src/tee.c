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
#تعریف_کړه پروګرام_NAME "tee"

#تعریف_کړه AUTHORS \
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
  {"append", no_دلیل, عدم, 'a'},
  {"ignore-interrupts", no_دلیل, عدم, 'i'},
  {"output-error", optional_دلیل, عدم, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن حرف ثابت *ثابت output_error_دلیلونه[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", عدم
};
ساكن تعداد output_error ثابت output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_دلیلونه, output_error_types);

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("Usage: %s [OPTION]... [FILE]...\n"), پروګرام_name);
      fputs (_("\
Copy standard ننوت منزل each FILE, and also منزل standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append منزل the given FILEs, افعل not overwrite\n\
"));
      oputs (_("\
  -i, --ignore-interrupts\n\
         ignore مداخله سیګنالونه\n\
"));
      oputs (_("\
  -p\n\
         عمل_کړه دننه a نور appropriate ډېرپېښ with pipes\n\
"));
      oputs (_("\
      --output-error[=ډېرپېښ]\n\
         سېټ_2 behavior on اكتب error.  See ډېرپېښ لاندې\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ډېرپېښ determines behavior with اكتب errors on the outputs:\n\
  warn           diagnose errors writing منزل any output\n\
  warn-nopipe    diagnose errors writing منزل any output not a pipe\n\
  وتل           وتل on error writing منزل any output\n\
  وتل-nopipe    وتل on error writing منزل any output not a pipe\n\
The افتراضي ډېرپېښ لكل the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, وتل immediately إذا all outputs become broken pipes.\n\
The افتراضي operation when --output-error is not specified, is منزل\n\
وتل immediately on error writing منزل a pipe, and diagnose errors\n\
writing منزل non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
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
                                      output_error_دلیلونه, output_error_types);
          وإلا
            output_error = output_error_warn_nopipe;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

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
                     && iopoll_ننوت_ok (STDIN_FILENO));

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
get_بل_بهر (صحيح *descriptors, صحيح nfiles, صحيح idx)
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
  bool *بهر_pollable IF_LINT ( = عدم);
  حرف بفر[BUFSIZ];
  ssize_t بایټونه_read = 0;
  صحيح لومړی_بهر = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  صحيح flags = O_WRONLY | O_CREAT | O_دوګونی | (append ? O_APPEND : O_TRUNC);

  xset_دوګونی_ډېرپېښ (STDIN_FILENO, O_دوګونی);
  xset_دوګونی_ډېرپېښ (STDOUT_FILENO, O_دوګونی);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, حجم *descriptors);
  إذا (pipe_check)
    بهر_pollable = xnmalloc (nfiles + 1, حجم *بهر_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  إذا (pipe_check)
    بهر_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  لكل (صحيح i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, ډېرپېښ_RW_UGO);
      إذا (descriptors[i] < 0)
        {
          إذا (pipe_check)
            بهر_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      وإلا
        {
          إذا (pipe_check)
            بهر_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  طالما (n_outputs)
    {
      إذا (pipe_check && بهر_pollable[لومړی_بهر])
        {
          /* Monitor for input, or errors on first valid output.  */
          صحيح err = iopoll (STDIN_FILENO, descriptors[لومړی_بهر], true);

          /* Close the output if it became a broken pipe.  */
          إذا (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              إذا (fail_output (descriptors, files, لومړی_بهر))
                ok = false;
              n_outputs--;
              لومړی_بهر = get_بل_بهر (descriptors, nfiles, لومړی_بهر);
              استمر;
            }
          وإلا إذا (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      بایټونه_read = اقرأ (STDIN_FILENO, بفر, حجم بفر);
      إذا (بایټونه_read < 0 && errno == EINTR)
        استمر;
      إذا (بایټونه_read <= 0)
        اكسر;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      لكل (صحيح i = 0; i <= nfiles; i++)
        إذا (0 <= descriptors[i]
            && ! write_wait (descriptors[i], بفر, بایټونه_read))
          {
            إذا (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            إذا (i == لومړی_بهر)
              لومړی_بهر = get_بل_بهر (descriptors, nfiles, لومړی_بهر);
          }
    }

  إذا (بایټونه_read == -1)
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
    حرر (بهر_pollable);

  أرجع ok;
}
