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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "fd-reopen.h"
#ಅನ್ತರ್ಭಾವ "long-options.h"
#ಅನ್ತರ್ಭಾವ "unistd--.h"

#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "nohup"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Jim Meyering")

/* Exit statuses.  */
ಎಣಿಕೆ
  {
    /* 'nohup' itself failed.  */
    POSIX_NOHUP_FAILURE = 127
  };

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s COMMAND [ARG]...\n\
  or:  %s OPTION\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);

      fputs (_("\
ಸತತ COMMAND, ignoring hangup ಸಂಕೇತಗಳು.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      ಬರೆ (_("\n\
If standard ಒಳಹರಿವು is a terminal, redirect it from an unreadable file.\n\
If standard output is a terminal, append output ಗಮ್ಯ 'nohup.out' ಆದರೆ possible,\n\
'$HOME/nohup.out' otherwise.\n\
If standard error is a terminal, redirect it ಗಮ್ಯ standard output.\n\
ಗಮ್ಯ_2 save output ಗಮ್ಯ FILE, use '%s COMMAND > FILE'.\n"),
              ಕಾರ್ಯಕ್ರಮ_name);
      ಬರೆ (USAGE_BUILTIN_WARNING, ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_exec_ಸ್ಥಿತಿ (ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* GCC 13 gets confused by the dup2 calls
   <https://gcc.gnu.org/PR109839>.  */
#ಆದರೆ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-fd-leak"
#ಆದರೆ_ಅಂತ್ಯ

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* POSIX 2008 requires that internal failure give status 127; unlike
     for env, exec, nice, time, and xargs where it requires internal
     failure give something in the range 1-125.  For consistency with
     other tools, fail with EXIT_CANCELED unless POSIXLY_CORRECT.  */
  ಪೂರ್ಣಾಂಕ exit_ಆಂತರಿಕ_failure = (getenv ("POSIXLY_CORRECT")
                               ? POSIX_NOHUP_FAILURE : EXIT_CANCELED);
  initialize_exit_failure (exit_ಆಂತರಿಕ_failure);
  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE_NAME,
                                   ಆವೃತ್ತಿ, false, usage, AUTHORS,
                                   (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) ಖಾಲಿ);

  ಆದರೆ (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (exit_ಆಂತರಿಕ_failure);
    }

  bool ignoring_ಒಳಹರಿವು = isatty (STDIN_FILENO);
  bool redirecting_stdout = isatty (STDOUT_FILENO);
  bool stdout_is_closed = (!redirecting_stdout && errno == EBADF);
  bool redirecting_stderr = isatty (STDERR_FILENO);

  /* If standard input is a tty, replace it with /dev/null if possible.
     Note that it is deliberately opened for *writing*,
     to ensure any read evokes an error.  */
  ಆದರೆ (ignoring_ಒಳಹರಿವು)
    {
      ಆದರೆ (fd_reopen (STDIN_FILENO, "/dev/null", O_WRONLY, 0) < 0)
        error (exit_ಆಂತರಿಕ_failure, errno,
               _("failed to render standard input unusable"));
      ಆದರೆ (!redirecting_stdout && !redirecting_stderr)
        error (0, 0, _("ignoring input"));
    }

  /* If standard output is a tty, redirect it (appending) to a file.
     First try nohup.out, then $HOME/nohup.out.  If standard error is
     a tty and standard output is closed, open nohup.out or
     $HOME/nohup.out without redirecting anything.  */
  ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_fd = STDOUT_FILENO;
  ಆದರೆ (redirecting_stdout || (redirecting_stderr && stdout_is_closed))
    {
      ಅಕ್ಷರ *ಒಳ_home = ಖಾಲಿ;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file = "nohup.out";
      ಪೂರ್ಣಾಂಕ flags = O_CREAT | O_WRONLY | O_APPEND;
      mode_t ಬಹುಲಕ_2 = S_IRUSR | S_IWUSR;
      mode_t umask_ಮೌಲ್ಯ = umask (0);
      ಹೊರಹೋಗು_fd = (redirecting_stdout
                ? fd_reopen (STDOUT_FILENO, file, flags, ಬಹುಲಕ_2)
                : open (file, flags, ಬಹುಲಕ_2));

      ಆದರೆ (ಹೊರಹೋಗು_fd < 0)
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *home = getenv ("HOME");
          ಆದರೆ (home)
            {
              ಒಳ_home = file_name_concat (home, file, ಖಾಲಿ);
              ಹೊರಹೋಗು_fd = (redirecting_stdout
                        ? fd_reopen (STDOUT_FILENO, ಒಳ_home, flags, ಬಹುಲಕ_2)
                        : open (ಒಳ_home, flags, ಬಹುಲಕ_2));
            }
          ಆದರೆ (ಹೊರಹೋಗು_fd < 0)
            {
              ಪೂರ್ಣಾಂಕ saved_errno2 = errno;
              error (0, saved_errno, _("failed to open %s"), quoteaf (file));
              ಆದರೆ (ಒಳ_home)
                error (0, saved_errno2, _("failed to open %s"),
                       quoteaf (ಒಳ_home));
              ಹಿಂತಿರುಗಿ exit_ಆಂತರಿಕ_failure;
            }
          file = ಒಳ_home;
        }

      umask (umask_ಮೌಲ್ಯ);
      error (0, 0,
             _(ignoring_ಒಳಹರಿವು
               ? N_("ignoring input and appending output to %s")
               : N_("appending output to %s")),
             quoteaf (file));
      ಬಿಡುಗಡೆ (ಒಳ_home);
    }

  /* If standard error is a tty, redirect it.  */
  ಪೂರ್ಣಾಂಕ saved_stderr_fd = STDERR_FILENO;
  ಆದರೆ (redirecting_stderr)
    {
      /* Save a copy of stderr before redirecting, so we can use the original
         if execve fails.  It's no big deal if this dup fails.  It might
         not change anything, and at worst, it'll lead to suppression of
         the post-failed-execve diagnostic.  */
      saved_stderr_fd = fcntl (STDERR_FILENO, F_DUPFD_CLOEXEC,
                               STDERR_FILENO + 1);

      ಆದರೆ (!redirecting_stdout)
        error (0, 0,
               _(ignoring_ಒಳಹರಿವು
                 ? N_("ignoring input and redirecting standard error "
                      "to standard output")
                 : N_("redirecting standard error to standard output")));

      ಆದರೆ (dup2 (ಹೊರಹೋಗು_fd, STDERR_FILENO) < 0)
        error (exit_ಆಂತರಿಕ_failure, errno,
               _("failed to redirect standard error"));

      ಆದರೆ (stdout_is_closed)
        close (ಹೊರಹೋಗು_fd);
    }

  /* error() flushes stderr, but does not check for write failure.
     Normally, we would catch this via our atexit() hook of
     close_stdout, but execvp() gets in the way.  If stderr
     encountered a write failure, there is no need to try calling
     error() again, particularly since we may have just changed the
     underlying fd out from under stderr.  */
  ಆದರೆ (ferror (stderr))
    ಹಿಂತಿರುಗಿ exit_ಆಂತರಿಕ_failure;

  /* Windows does not support SIGHUP.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SIGHUP
  ಸಂಕೇತ (SIGHUP, SIG_IGN);
#ಆದರೆ_ಅಂತ್ಯ

  ಅಕ್ಷರ **cmd = argv + optind;
  execvp (*cmd, cmd);
  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  ಪೂರ್ಣಾಂಕ saved_errno = errno;

  /* The execve failed.  Output a diagnostic to stderr only if:
     - stderr was initially redirected to a non-tty, or
     - stderr was initially directed to a tty, and we
     can dup2 it to point back to that same tty.
     In other words, output the diagnostic if possible, but only if
     it will go to the original stderr.  */
  ಆದರೆ (dup2 (saved_stderr_fd, STDERR_FILENO) == STDERR_FILENO)
    error (0, saved_errno, _("failed to run command %s"), quoteaf (*cmd));

  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}
