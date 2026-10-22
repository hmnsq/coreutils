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

#అన్తర్భావ <config.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <getopt.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "fcntl--.h"
#అన్తర్భావ "iopoll.h"
#అన్తర్భావ "xbinary-io.h"
#అన్తర్భావ "iopoll.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "tee"

#నిర్వచించు AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

స్థిర bool tee_files (పూర్ణాంకం nfiles, అక్షరం **files, bool);

/* If true, append to output files rather than truncating them. */
స్థిర bool append;

/* If true, ignore interrupts. */
స్థిర bool ignore_interrupts;

గణన output_error
  {
    output_error_sigpipe,      /* traditional behavior, sigpipe enabled.  */
    output_error_warn,         /* warn on EPIPE, but continue.  */
    output_error_warn_nopipe,  /* ignore EPIPE, continue.  */
    output_error_exit,         /* exit on any output error.  */
    output_error_exit_nopipe   /* exit on any output error except EPIPE.  */
  };

స్థిర గణన output_error output_error;

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"append", no_వాదం, ఖాళీ, 'a'},
  {"ignore-interrupts", no_వాదం, ఖాళీ, 'i'},
  {"output-error", optional_వాదం, ఖాళీ, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి output_error_వాదనలు[] =
{
  "warn", "warn-nopipe", "exit", "exit-nopipe", ఖాళీ
};
స్థిర గణన output_error స్థిరరాశి output_error_types[] =
{
  output_error_warn, output_error_warn_nopipe,
  output_error_exit, output_error_exit_nopipe
};
ARGMATCH_VERIFY (output_error_వాదనలు, output_error_types);

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... [FILE]...\n"), కార్యక్రమం_name);
      fputs (_("\
Copy standard ఆదానం గమ్యం each FILE, and also గమ్యం standard output.\n\
\n\
"), stdout);
      oputs (_("\
  -a, --append\n\
         append గమ్యం the given FILEs, చేయి not overwrite\n\
"));
      oputs (_("\
  -i, --ignore-interrupts\n\
         ignore అంతరాయం సంకేతాలు\n\
"));
      oputs (_("\
  -p\n\
         నిర్వహించు లోపల a మరిన్ని appropriate బహుళకం with pipes\n\
"));
      oputs (_("\
      --output-error[=బహుళకం]\n\
         సమితి_2 behavior on లిఖ error.  See బహుళకం క్రింద\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
బహుళకం determines behavior with లిఖ errors on the outputs:\n\
  warn           diagnose errors writing గమ్యం any output\n\
  warn-nopipe    diagnose errors writing గమ్యం any output not a pipe\n\
  నిష్క్రమించు           నిష్క్రమించు on error writing గమ్యం any output\n\
  నిష్క్రమించు-nopipe    నిష్క్రమించు on error writing గమ్యం any output not a pipe\n\
The అప్రమేయ బహుళకం కొరకు the -p option is 'warn-nopipe'.\n\
With \"nopipe\" MODEs, నిష్క్రమించు immediately ఒకవేళ all outputs become broken pipes.\n\
The అప్రమేయ operation when --output-error is not specified, is గమ్యం\n\
నిష్క్రమించు immediately on error writing గమ్యం a pipe, and diagnose errors\n\
writing గమ్యం non pipe outputs.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  పూర్ణాంకం optc;
  అయితే ((optc = getopt_long (argc, argv, "aip", long_options, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'a':
          append = true;
          ఆపు;

        స్థితి 'i':
          ignore_interrupts = true;
          ఆపు;

        స్థితి 'p':
          ఒకవేళ (optarg)
            output_error = XARGMATCH ("--output-error", optarg,
                                      output_error_వాదనలు, output_error_types);
          లేకపోతే
            output_error = output_error_warn_nopipe;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (ignore_interrupts)
    సంకేతం (SIGINT, SIG_IGN);

  ఒకవేళ (output_error != output_error_sigpipe)
    సంకేతం (SIGPIPE, SIG_IGN);

  /* Whether to detect and close a broken pipe output.
     There is no need if the input is always ready for reading.  */
  bool pipe_check = ((output_error == output_error_warn_nopipe
                      || output_error == output_error_exit_nopipe)
                     && iopoll_ఆదానం_ok (STDIN_FILENO));

  /* Do *not* warn if tee is given no file arguments.
     POSIX requires that it work when given no arguments.  */

  bool ok = tee_files (argc - optind, &argv[optind], pipe_check);
  ఒకవేళ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", _("standard input"));

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* Return the index of the first non-null descriptor after idx,
   or -1 if all are null.  */

ATTRIBUTE_శుద్ధ
స్థిర పూర్ణాంకం
get_తదుపరి_బహిర్గతం (పూర్ణాంకం *descriptors, పూర్ణాంకం nfiles, పూర్ణాంకం idx)
{
  కొరకు (idx++; idx <= nfiles; idx++)
    ఒకవేళ (0 <= descriptors[idx])
      తిరిగి idx;
  తిరిగి -1;  /* no outputs remaining */
}

/* Remove descriptors[i] due to write failure or broken pipe.
   Return true if this indicates a reportable error.  */

స్థిర bool
fail_output (పూర్ణాంకం *descriptors, అక్షరం **files, పూర్ణాంకం i)
{
  పూర్ణాంకం w_errno = errno;
  bool fail = errno != EPIPE
              || output_error == output_error_exit
              || output_error == output_error_warn;
  ఒకవేళ (fail)
    {
      error (output_error == output_error_exit
             || output_error == output_error_exit_nopipe,
             w_errno, "%s", quotef (files[i]));
    }
  descriptors[i] = -1;
  తిరిగి fail;
}


/* Copy the standard input into each of the NFILES files in FILES
   and into the standard output.  As a side effect, modify FILES[-1].
   Return true if successful.  */

స్థిర bool
tee_files (పూర్ణాంకం nfiles, అక్షరం **files, bool pipe_check)
{
  size_t n_outputs = 0;
  పూర్ణాంకం *descriptors;
  bool *బహిర్గతం_pollable IF_LINT ( = ఖాళీ);
  అక్షరం బఫరు[BUFSIZ];
  ssize_t బైట్లు_read = 0;
  పూర్ణాంకం మొదటి_బహిర్గతం = 0;  /* idx of first non-null output in descriptors */
  bool ok = true;
  పూర్ణాంకం flags = O_WRONLY | O_CREAT | O_ద్విమాన | (append ? O_APPEND : O_TRUNC);

  xset_ద్విమాన_బహుళకం (STDIN_FILENO, O_ద్విమాన);
  xset_ద్విమాన_బహుళకం (STDOUT_FILENO, O_ద్విమాన);
  fadvise (stdin, FADVISE_SEQUENTIAL);

  /* Set up FILES[0 .. NFILES] and DESCRIPTORS[0 .. NFILES].
     In both arrays, entry 0 corresponds to standard output.  */

  descriptors = xnmalloc (nfiles + 1, పరిమాణం *descriptors);
  ఒకవేళ (pipe_check)
    బహిర్గతం_pollable = xnmalloc (nfiles + 1, పరిమాణం *బహిర్గతం_pollable);
  files--;
  descriptors[0] = STDOUT_FILENO;
  ఒకవేళ (pipe_check)
    బహిర్గతం_pollable[0] = iopoll_output_ok (descriptors[0]);
  files[0] = bad_cast (_("standard output"));
  n_outputs++;

  కొరకు (పూర్ణాంకం i = 1; i <= nfiles; i++)
    {
      /* Do not treat "-" specially - as mandated by POSIX.  */
      descriptors[i] = open (files[i], flags, బహుళకం_RW_UGO);
      ఒకవేళ (descriptors[i] < 0)
        {
          ఒకవేళ (pipe_check)
            బహిర్గతం_pollable[i] = false;
          error (output_error == output_error_exit
                 || output_error == output_error_exit_nopipe,
                 errno, "%s", quotef (files[i]));
          ok = false;
        }
      లేకపోతే
        {
          ఒకవేళ (pipe_check)
            బహిర్గతం_pollable[i] = iopoll_output_ok (descriptors[i]);
          n_outputs++;
        }
    }

  అయితే (n_outputs)
    {
      ఒకవేళ (pipe_check && బహిర్గతం_pollable[మొదటి_బహిర్గతం])
        {
          /* Monitor for input, or errors on first valid output.  */
          పూర్ణాంకం err = iopoll (STDIN_FILENO, descriptors[మొదటి_బహిర్గతం], true);

          /* Close the output if it became a broken pipe.  */
          ఒకవేళ (err == IOPOLL_BROKEN_OUTPUT)
            {
              errno = EPIPE;  /* behave like write produced EPIPE */
              ఒకవేళ (fail_output (descriptors, files, మొదటి_బహిర్గతం))
                ok = false;
              n_outputs--;
              మొదటి_బహిర్గతం = get_తదుపరి_బహిర్గతం (descriptors, nfiles, మొదటి_బహిర్గతం);
              కొనసాగు;
            }
          లేకపోతే ఒకవేళ (err == IOPOLL_ERROR)
            {
              error (0, errno, _("iopoll error"));
              ok = false;
            }
        }

      బైట్లు_read = పఠ (STDIN_FILENO, బఫరు, పరిమాణం బఫరు);
      ఒకవేళ (బైట్లు_read < 0 && errno == EINTR)
        కొనసాగు;
      ఒకవేళ (బైట్లు_read <= 0)
        ఆపు;

      /* Write to all NFILES + 1 descriptors.
         Standard output is the first one.  */
      కొరకు (పూర్ణాంకం i = 0; i <= nfiles; i++)
        ఒకవేళ (0 <= descriptors[i]
            && ! write_wait (descriptors[i], బఫరు, బైట్లు_read))
          {
            ఒకవేళ (fail_output (descriptors, files, i))
              ok = false;
            n_outputs--;
            ఒకవేళ (i == మొదటి_బహిర్గతం)
              మొదటి_బహిర్గతం = get_తదుపరి_బహిర్గతం (descriptors, nfiles, మొదటి_బహిర్గతం);
          }
    }

  ఒకవేళ (బైట్లు_read == -1)
    {
      error (0, errno, _("read error"));
      ok = false;
    }

  /* Close the files, but not standard output.  */
  కొరకు (పూర్ణాంకం i = 1; i <= nfiles; i++)
    ఒకవేళ (0 <= descriptors[i] && close (descriptors[i]) < 0)
      {
        error (0, errno, "%s", quotef (files[i]));
        ok = false;
      }

  విడుదల (descriptors);
  ఒకవేళ (pipe_check)
    విడుదల (బహిర్గతం_pollable);

  తిరిగి ok;
}
