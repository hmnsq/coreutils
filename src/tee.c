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

#شامل <config.h>
#شامل <sys/types.h>
#شامل <signal.h>
#شامل <getopt.h>

#شامل "system.h"
#شامل "argmatch.h"
#شامل "fadvise.h"
#شامل "fcntl--.h"
#شامل "iopoll.h"
#شامل "xbinary-io.h"
#شامل "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "tee"

#تعریف_کرو AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ساکن bool tee_files (صحیح nfiles, حرف **files, bool);

/* If true, append to output files rather than truncating them. */
ساکن bool append;

/* If true, ignore interrupts. */
ساکن bool ignore_interrupts;

شمار output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

ساکن شمار output_error output_error;

ساکن ساخت option ثابت long_options[] =
{
  {"append", no_دلیل, تہی, 'a'},
  {"ignore-interrupts", no_دلیل, تہی, 'i'},
  {"output-error", optional_دلیل, تہی, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

ساکن حرف ثابت *ثابت output_error_دلائل[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", تہی
};
ساکن شمار output_error ثابت output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_دلائل, output_error_types);

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [FILE]...\n"), پروگرام_name);
      fputs (_("\
Copy standard داخلہ مقصد each FILE, and also مقصد standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append مقصد the given FILEs, کرو not overwrite\n\
"));
      oputs (_("\
  -i, --ignore-interrupts\n\
         ignore مداخلت اشارات\n\
"));
      oputs (_("\
  -p\n\
         عمل_کرو اندر a مزید appropriate منوال with pipes\n\
"));
      oputs (_("\
      --output-error[=منوال]\n\
         سیٹ_2 behavior on اكتب error.  See منوال نیچے\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
منوال determines behavior with اكتب errors on the outputs:\n\
  warn           diagnose errors writing مقصد any output\n\
  warn-nopipe    diagnose errors writing مقصد any output not a pipe\n\
  باہر_نکلو           باہر_نکلو on error writing مقصد any output\n\
  باہر_نکلو-nopipe    باہر_نکلو on error writing مقصد any output not a pipe\n\
The طےشدہ منوال برائے the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, باہر_نکلو immediately اگر all outputs become broken pipes.\n\
The طےشدہ operation when --output-error is not specified, is مقصد\n\
باہر_نکلو immediately on error writing مقصد a pipe, and diagnose errors\n\
writing مقصد non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحیح optc;
  جب ((optc = getopt_long (argc, argv, "aip", long_options, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'a':
          append = true;
          توڑو;

        حالت 'i':
          ignore_interrupts = true;
          توڑو;

        حالت 'p':
          اگر (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_دلائل, output_error_types);
          ورنہ
            output_error = output_error_warn_nopipe;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (ignore_interrupts)
    اشارہ (SIGINT, SIG_IGN);

  اگر (output_error != output_error_sigpipe)
    اشارہ (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_داخلہ_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  اگر (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_خالص
ساکن صحیح
get_اگلا_برآمد (صحیح *descriptors, صحیح nfiles, صحیح idx)
{
  برائے (idx++; idx <= nfiles; idx++)
    اگر (0 <= descriptors[idx])
      واپس idx;
  واپس -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

ساکن bool
fail_output (صحیح *descriptors, حرف **files, صحیح i)
{
  صحیح w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  اگر (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  واپس fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

ساکن bool
tee_files (صحیح nfiles, حرف **files, bool pipe_check)
{
  size_t n_outputs = 0;
  صحیح *descriptors;
  bool *برآمد_pollable IF_LINT ( = تہی);
  حرف بفر[BUFSIZ];
  ssize_t بائٹیں_read = 0;
  صحیح پہلا_برآمد = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  صحیح flags = O_WRONLY | O_CREAT | O_ثنائی | (append ? O_APPEND : O_TRUNC);

  xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
  xset_ثنائی_منوال (STDOUT_FILENO, O_ثنائی);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, حجم *descriptors);
  اگر (pipe_check)
    برآمد_pollable = xnmalloc (nfiles + 1, حجم *برآمد_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  اگر (pipe_check)
    برآمد_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  برائے (صحیح i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, منوال_RW_UGO);
      اگر (descriptors[i] < 0)
        {
          اگر (pipe_check)
            برآمد_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      ورنہ
        {
          اگر (pipe_check)
            برآمد_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  جب (n_outputs)
    {
      اگر (pipe_check && برآمد_pollable[پہلا_برآمد])
        {
          /* Monitor for input, or errors on first valid output.  */
          صحیح err = iopoll (STDIN_FILENO, descriptors[پہلا_برآمد], true);

          /* Close the output if it became a broken pipe.  */
          اگر (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              اگر (fail_output (descriptors, files, پہلا_برآمد))
                ok = false;
              n_outputs--;
              پہلا_برآمد = get_اگلا_برآمد (descriptors, nfiles, پہلا_برآمد);
              جاری;
            }
          ورنہ اگر (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      بائٹیں_read = اقرأ (STDIN_FILENO, بفر, حجم بفر);
      اگر (بائٹیں_read < 0 && errno == EINTR)
        جاری;
      اگر (بائٹیں_read <= 0)
        توڑو;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      برائے (صحیح i = 0; i <= nfiles; i++)
        اگر (0 <= descriptors[i]
            && ! write_wait (descriptors[i], بفر, بائٹیں_read))
          {
            اگر (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            اگر (i == پہلا_برآمد)
              پہلا_برآمد = get_اگلا_برآمد (descriptors, nfiles, پہلا_برآمد);
          }
    }

  اگر (بائٹیں_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  برائے (صحیح i = 1; i <= nfiles; i++)
    اگر (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  آزاد (descriptors);
  اگر (pipe_check)
    آزاد (برآمد_pollable);

  واپس ok;
}
