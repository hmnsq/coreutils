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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <signal.h>

#ضمن "system.h"

#ضمن "filenamecat.h"
#ضمن "fd-reopen.h"
#ضمن "long-options.h"
#ضمن "unistd--.h"

#تعریف_کرو پروگرام_NAME "nohup"

#تعریف_کرو AUTHORS proper_name ("Jim Meyering")

/* Exit statuses.  */
تعداد
  {
    /* 'nohup' itself failed.  */
    POSIX_NOHUP_FAILURE = 127
  };

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s COMMAND [ARG]...\n\
  or:  %s OPTION\n\
"),
              پروگرام_name, پروگرام_name);

      fputs (_("\
لگاتار COMMAND, ignoring hangup اشارے.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      لکھو (_("\n\
If standard داخلہ is a terminal, redirect it from an unreadable file.\n\
If standard output is a terminal, append output منزل 'nohup.out' إذا possible,\n\
'$HOME/nohup.out' otherwise.\n\
If standard error is a terminal, redirect it منزل standard output.\n\
منزل_2 save output منزل FILE, use '%s COMMAND > FILE'.\n"),
              پروگرام_name);
      لکھو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
      emit_exec_حالت (پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* GCC 13 gets confused by the dup2 calls
   <https://gcc.gnu.org/PR109839>.  */
#إذا 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-fd-leak"
#جے_انت

صحيح
مکھ (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* POSIX 2008 requires that internal failure give status 127; unlike
     for env, exec, nice, time, and xargs where it requires internal
     failure give something in the range 1-125.  For consistency with
     other tools, fail with EXIT_CANCELED unless POSIXLY_CORRECT.  */
  صحيح exit_اندرونی_failure = (getenv ("POSIXLY_CORRECT")
                               ? POSIX_NOHUP_FAILURE : EXIT_CANCELED);
  initialize_exit_failure (exit_اندرونی_failure);
  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخہ, false, usage, AUTHORS,
                                   (حرف ثابت *) عدم);

  إذا (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (exit_اندرونی_failure);
    }

  bool ignoring_داخلہ = isatty (STDIN_FILENO);
  bool redirecting_stdout = isatty (STDOUT_FILENO);
  bool stdout_is_closed = (!redirecting_stdout && errno == EBADF);
  bool redirecting_stderr = isatty (STDERR_FILENO);

  /* If standard input is a tty, replace it with /dev/null if possible.
     Note that it is deliberately opened for *writing*,
     to ensure any read evokes an error.  */
  إذا (ignoring_داخلہ)
    {
      إذا (fd_reopen (STDIN_FILENO, "/dev/null", O_WRONLY, 0) < 0)
        error (exit_اندرونی_failure, errno,
               _("failed to render standard input unusable"));
      إذا (!redirecting_stdout && !redirecting_stderr)
        error (0, 0, _("ignoring input"));
    }

  /* If standard output is a tty, redirect it (appending) to a file.
     First try nohup.out, then $HOME/nohup.out.  If standard error is
     a tty and standard output is closed, open nohup.out or
     $HOME/nohup.out without redirecting anything.  */
  صحيح باہرلا_fd = STDOUT_FILENO;
  إذا (redirecting_stdout || (redirecting_stderr && stdout_is_closed))
    {
      حرف *وچ_home = عدم;
      حرف ثابت *file = "nohup.out";
      صحيح flags = O_CREAT | O_WRONLY | O_APPEND;
      mode_t منوال_2 = S_IRUSR | S_IWUSR;
      mode_t umask_قدر = umask (0);
      باہرلا_fd = (redirecting_stdout
                ? fd_reopen (STDOUT_FILENO, file, flags, منوال_2)
                : open (file, flags, منوال_2));

      إذا (باہرلا_fd < 0)
        {
          صحيح saved_errno = errno;
          حرف ثابت *home = getenv ("HOME");
          إذا (home)
            {
              وچ_home = file_name_concat (home, file, عدم);
              باہرلا_fd = (redirecting_stdout
                        ? fd_reopen (STDOUT_FILENO, وچ_home, flags, منوال_2)
                        : open (وچ_home, flags, منوال_2));
            }
          إذا (باہرلا_fd < 0)
            {
              صحيح saved_errno2 = errno;
              error (0, saved_errno, _("failed to open %s"), quoteaf (file));
              إذا (وچ_home)
                error (0, saved_errno2, _("failed to open %s"),
                       quoteaf (وچ_home));
              أرجع exit_اندرونی_failure;
            }
          file = وچ_home;
        }

      umask (umask_قدر);
      error (0, 0,
             _(ignoring_داخلہ
               ? N_("ignoring input and appending output to %s")
               : N_("appending output to %s")),
             quoteaf (file));
      حرر (وچ_home);
    }

  /* If standard error is a tty, redirect it.  */
  صحيح saved_stderr_fd = STDERR_FILENO;
  إذا (redirecting_stderr)
    {
      /* Save a copy of stderr before redirecting, so we can use the original
         if execve fails.  It's no big deal if this dup fails.  It might
         not change anything, and at worst, it'll lead to suppression of
         the post-failed-execve diagnostic.  */
      saved_stderr_fd = fcntl (STDERR_FILENO, F_DUPFD_CLOEXEC,
                               STDERR_FILENO + 1);

      إذا (!redirecting_stdout)
        error (0, 0,
               _(ignoring_داخلہ
                 ? N_("ignoring input and redirecting standard error "
                      "to standard output")
                 : N_("redirecting standard error to standard output")));

      إذا (dup2 (باہرلا_fd, STDERR_FILENO) < 0)
        error (exit_اندرونی_failure, errno,
               _("failed to redirect standard error"));

      إذا (stdout_is_closed)
        close (باہرلا_fd);
    }

  /* error() flushes stderr, but does not check for write failure.
     Normally, we would catch this via our atexit() hook of
     close_stdout, but execvp() gets in the way.  If stderr
     encountered a write failure, there is no need to try calling
     error() again, particularly since we may have just changed the
     underlying fd out from under stderr.  */
  إذا (ferror (stderr))
    أرجع exit_اندرونی_failure;

  /* Windows does not support SIGHUP.  */
#جے_تعریف_شدہ SIGHUP
  إشارة (SIGHUP, SIG_IGN);
#جے_انت

  حرف **cmd = argv + optind;
  execvp (*cmd, cmd);
  صحيح exit_حالت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  صحيح saved_errno = errno;

  /* The execve failed.  Output a diagnostic to stderr only if:
     - stderr was initially redirected to a non-tty, or
     - stderr was initially directed to a tty, and we
     can dup2 it to point back to that same tty.
     In other words, output the diagnostic if possible, but only if
     it will go to the original stderr.  */
  إذا (dup2 (saved_stderr_fd, STDERR_FILENO) == STDERR_FILENO)
    error (0, saved_errno, _("failed to run command %s"), quoteaf (*cmd));

  أرجع exit_حالت;
}
