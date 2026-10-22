/* nohup -- run a command immune to hangups, with output to a non-tty
   Copyright (C) 2003-2026 Free Software Foundation, Inc.

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

/* Written by Jim Meyering  */

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>

#అన్తర్భావ "system.h"

#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "fd-reopen.h"
#అన్తర్భావ "long-options.h"
#అన్తర్భావ "unistd--.h"

#నిర్వచించు కార్యక్రమం_NAME "nohup"

#నిర్వచించు AUTHORS proper_name ("Jim Meyering")

/* Exit statuses.  */
గణన
  {
    /* 'nohup' itself failed.  */
    POSIX_NOHUP_FAILURE = 127
  };

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s COMMAND [ARG]...\n\
  or:  %s OPTION\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);

      fputs (_("\
నిరంతరం COMMAND, ignoring hangup సంకేతాలు.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      రాయి (_("\n\
If standard ఆదానం is a terminal, redirect it from an unreadable file.\n\
If standard output is a terminal, append output గమ్యం 'nohup.out' ఒకవేళ possible,\n\
'$HOME/nohup.out' otherwise.\n\
If standard error is a terminal, redirect it గమ్యం standard output.\n\
గమ్యం_2 save output గమ్యం FILE, use '%s COMMAND > FILE'.\n"),
              కార్యక్రమం_name);
      రాయి (USAGE_BUILTIN_WARNING, కార్యక్రమం_NAME);
      emit_exec_స్థితి (కార్యక్రమం_NAME);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* GCC 13 gets confused by the dup2 calls
   <https://gcc.gnu.org/PR109839>.  */
#ఒకవేళ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-fd-leak"
#అయితే_ముగింపు

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* POSIX 2008 requires that internal failure give status 127; unlike
     for env, exec, nice, time, and xargs where it requires internal
     failure give something in the range 1-125.  For consistency with
     other tools, fail with EXIT_CANCELED unless POSIXLY_CORRECT.  */
  పూర్ణాంకం exit_అంతర్గత_failure = (getenv ("POSIXLY_CORRECT")
                               ? POSIX_NOHUP_FAILURE : EXIT_CANCELED);
  initialize_exit_failure (exit_అంతర్గత_failure);
  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, కార్యక్రమం_NAME, PACKAGE_NAME,
                                   సంస్కరణ, false, usage, AUTHORS,
                                   (అక్షరం స్థిరరాశి *) ఖాళీ);

  ఒకవేళ (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (exit_అంతర్గత_failure);
    }

  bool ignoring_ఆదానం = isatty (STDIN_FILENO);
  bool redirecting_stdout = isatty (STDOUT_FILENO);
  bool stdout_is_closed = (!redirecting_stdout && errno == EBADF);
  bool redirecting_stderr = isatty (STDERR_FILENO);

  /* If standard input is a tty, replace it with /dev/null if possible.
     Note that it is deliberately opened for *writing*,
     to ensure any read evokes an error.  */
  ఒకవేళ (ignoring_ఆదానం)
    {
      ఒకవేళ (fd_reopen (STDIN_FILENO, "/dev/null", O_WRONLY, 0) < 0)
        error (exit_అంతర్గత_failure, errno,
               _("failed to render standard input unusable"));
      ఒకవేళ (!redirecting_stdout && !redirecting_stderr)
        error (0, 0, _("ignoring input"));
    }

  /* If standard output is a tty, redirect it (appending) to a file.
     First try nohup.out, then $HOME/nohup.out.  If standard error is
     a tty and standard output is closed, open nohup.out or
     $HOME/nohup.out without redirecting anything.  */
  పూర్ణాంకం బహిర్గతం_fd = STDOUT_FILENO;
  ఒకవేళ (redirecting_stdout || (redirecting_stderr && stdout_is_closed))
    {
      అక్షరం *లోపల_home = ఖాళీ;
      అక్షరం స్థిరరాశి *file = "nohup.out";
      పూర్ణాంకం flags = O_CREAT | O_WRONLY | O_APPEND;
      mode_t బహుళకం_2 = S_IRUSR | S_IWUSR;
      mode_t umask_విలువ = umask (0);
      బహిర్గతం_fd = (redirecting_stdout
                ? fd_reopen (STDOUT_FILENO, file, flags, బహుళకం_2)
                : open (file, flags, బహుళకం_2));

      ఒకవేళ (బహిర్గతం_fd < 0)
        {
          పూర్ణాంకం saved_errno = errno;
          అక్షరం స్థిరరాశి *home = getenv ("HOME");
          ఒకవేళ (home)
            {
              లోపల_home = file_name_concat (home, file, ఖాళీ);
              బహిర్గతం_fd = (redirecting_stdout
                        ? fd_reopen (STDOUT_FILENO, లోపల_home, flags, బహుళకం_2)
                        : open (లోపల_home, flags, బహుళకం_2));
            }
          ఒకవేళ (బహిర్గతం_fd < 0)
            {
              పూర్ణాంకం saved_errno2 = errno;
              error (0, saved_errno, _("failed to open %s"), quoteaf (file));
              ఒకవేళ (లోపల_home)
                error (0, saved_errno2, _("failed to open %s"),
                       quoteaf (లోపల_home));
              తిరిగి exit_అంతర్గత_failure;
            }
          file = లోపల_home;
        }

      umask (umask_విలువ);
      error (0, 0,
             _(ignoring_ఆదానం
               ? N_("ignoring input and appending output to %s")
               : N_("appending output to %s")),
             quoteaf (file));
      విడుదల (లోపల_home);
    }

  /* If standard error is a tty, redirect it.  */
  పూర్ణాంకం saved_stderr_fd = STDERR_FILENO;
  ఒకవేళ (redirecting_stderr)
    {
      /* Save a copy of stderr before redirecting, so we can use the original
         if execve fails.  It's no big deal if this dup fails.  It might
         not change anything, and at worst, it'll lead to suppression of
         the post-failed-execve diagnostic.  */
      saved_stderr_fd = fcntl (STDERR_FILENO, F_DUPFD_CLOEXEC,
                               STDERR_FILENO + 1);

      ఒకవేళ (!redirecting_stdout)
        error (0, 0,
               _(ignoring_ఆదానం
                 ? N_("ignoring input and redirecting standard error "
                      "to standard output")
                 : N_("redirecting standard error to standard output")));

      ఒకవేళ (dup2 (బహిర్గతం_fd, STDERR_FILENO) < 0)
        error (exit_అంతర్గత_failure, errno,
               _("failed to redirect standard error"));

      ఒకవేళ (stdout_is_closed)
        close (బహిర్గతం_fd);
    }

  /* error() flushes stderr, but does not check for write failure.
     Normally, we would catch this via our atexit() hook of
     close_stdout, but execvp() gets in the way.  If stderr
     encountered a write failure, there is no need to try calling
     error() again, particularly since we may have just changed the
     underlying fd out from under stderr.  */
  ఒకవేళ (ferror (stderr))
    తిరిగి exit_అంతర్గత_failure;

  /* Windows does not support SIGHUP.  */
#నిర్వచితమైతే SIGHUP
  సంకేతం (SIGHUP, SIG_IGN);
#అయితే_ముగింపు

  అక్షరం **cmd = argv + optind;
  execvp (*cmd, cmd);
  పూర్ణాంకం exit_స్థితి = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  పూర్ణాంకం saved_errno = errno;

  /* The execve failed.  Output a diagnostic to stderr only if:
     - stderr was initially redirected to a non-tty, or
     - stderr was initially directed to a tty, and we
     can dup2 it to point back to that same tty.
     In other words, output the diagnostic if possible, but only if
     it will go to the original stderr.  */
  ఒకవేళ (dup2 (saved_stderr_fd, STDERR_FILENO) == STDERR_FILENO)
    error (0, saved_errno, _("failed to run command %s"), quoteaf (*cmd));

  తిరిగి exit_స్థితి;
}
