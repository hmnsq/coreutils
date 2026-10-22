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

#تعریف_کن برنامه_NAME "nohup"

#تعریف_کن AUTHORS proper_name ("Jim Meyering")

/* Exit statuses.  */
تعداد
  {
    /* 'nohup' itself failed.  */
    POSIX_NOHUP_FAILURE = 127
  };

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s COMMAND [ARG]...\n\
  or:  %s OPTION\n\
"),
              برنامه_name, برنامه_name);

      fputs (_("\
پیاپی COMMAND, ignoring hangup سیگنالها.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      چاپ_کن (_("\n\
If standard درون_داد is a terminal, redirect it from an unreadable file.\n\
If standard output is a terminal, append output مقصد 'nohup.out' إذا possible,\n\
'$HOME/nohup.out' otherwise.\n\
If standard error is a terminal, redirect it مقصد standard output.\n\
مقصد_2 save output مقصد FILE, use '%s COMMAND > FILE'.\n"),
              برنامه_name);
      چاپ_کن (USAGE_BUILTIN_WARNING, برنامه_NAME);
      emit_exec_وضعیت (برنامه_NAME);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* GCC 13 gets confused by the dup2 calls
   <https://gcc.gnu.org/PR109839>.  */
#إذا 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-fd-leak"
#پایان_اگر

صحيح
اصلی (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* POSIX 2008 requires that internal failure give status 127; unlike
     for env, exec, nice, time, and xargs where it requires internal
     failure give something in the range 1-125.  For consistency with
     other tools, fail with EXIT_CANCELED unless POSIXLY_CORRECT.  */
  صحيح exit_داخلی_failure = (getenv ("POSIXLY_CORRECT")
                               ? POSIX_NOHUP_FAILURE : EXIT_CANCELED);
  initialize_exit_failure (exit_داخلی_failure);
  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, برنامه_NAME, PACKAGE_NAME,
                                   نسخه, false, usage, AUTHORS,
                                   (حرف ثابت *) عدم);

  إذا (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (exit_داخلی_failure);
    }

  bool ignoring_درون_داد = isatty (STDIN_FILENO);
  bool redirecting_stdout = isatty (STDOUT_FILENO);
  bool stdout_is_closed = (!redirecting_stdout && errno == EBADF);
  bool redirecting_stderr = isatty (STDERR_FILENO);

  /* If standard input is a tty, replace it with /dev/null if possible.
     Note that it is deliberately opened for *writing*,
     to ensure any read evokes an error.  */
  إذا (ignoring_درون_داد)
    {
      إذا (fd_reopen (STDIN_FILENO, "/dev/null", O_WRONLY, 0) < 0)
        error (exit_داخلی_failure, errno,
               _("failed to render standard input unusable"));
      إذا (!redirecting_stdout && !redirecting_stderr)
        error (0, 0, _("ignoring input"));
    }

  /* If standard output is a tty, redirect it (appending) to a file.
     First try nohup.out, then $HOME/nohup.out.  If standard error is
     a tty and standard output is closed, open nohup.out or
     $HOME/nohup.out without redirecting anything.  */
  صحيح بیرون_fd = STDOUT_FILENO;
  إذا (redirecting_stdout || (redirecting_stderr && stdout_is_closed))
    {
      حرف *درون_home = عدم;
      حرف ثابت *file = "nohup.out";
      صحيح flags = O_CREAT | O_WRONLY | O_APPEND;
      mode_t نما_2 = S_IRUSR | S_IWUSR;
      mode_t umask_مقدار = umask (0);
      بیرون_fd = (redirecting_stdout
                ? fd_reopen (STDOUT_FILENO, file, flags, نما_2)
                : open (file, flags, نما_2));

      إذا (بیرون_fd < 0)
        {
          صحيح saved_errno = errno;
          حرف ثابت *home = getenv ("HOME");
          إذا (home)
            {
              درون_home = file_name_concat (home, file, عدم);
              بیرون_fd = (redirecting_stdout
                        ? fd_reopen (STDOUT_FILENO, درون_home, flags, نما_2)
                        : open (درون_home, flags, نما_2));
            }
          إذا (بیرون_fd < 0)
            {
              صحيح saved_errno2 = errno;
              error (0, saved_errno, _("failed to open %s"), quoteaf (file));
              إذا (درون_home)
                error (0, saved_errno2, _("failed to open %s"),
                       quoteaf (درون_home));
              أرجع exit_داخلی_failure;
            }
          file = درون_home;
        }

      umask (umask_مقدار);
      error (0, 0,
             _(ignoring_درون_داد
               ? N_("ignoring input and appending output to %s")
               : N_("appending output to %s")),
             quoteaf (file));
      حرر (درون_home);
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
               _(ignoring_درون_داد
                 ? N_("ignoring input and redirecting standard error "
                      "to standard output")
                 : N_("redirecting standard error to standard output")));

      إذا (dup2 (بیرون_fd, STDERR_FILENO) < 0)
        error (exit_داخلی_failure, errno,
               _("failed to redirect standard error"));

      إذا (stdout_is_closed)
        close (بیرون_fd);
    }

  /* error() flushes stderr, but does not check for write failure.
     Normally, we would catch this via our atexit() hook of
     close_stdout, but execvp() gets in the way.  If stderr
     encountered a write failure, there is no need to try calling
     error() again, particularly since we may have just changed the
     underlying fd out from under stderr.  */
  إذا (ferror (stderr))
    أرجع exit_داخلی_failure;

  /* Windows does not support SIGHUP.  */
#اگر_تعریف_شده SIGHUP
  إشارة (SIGHUP, SIG_IGN);
#پایان_اگر

  حرف **cmd = argv + optind;
  execvp (*cmd, cmd);
  صحيح exit_وضعیت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  صحيح saved_errno = errno;

  /* The execve failed.  Output a diagnostic to stderr only if:
     - stderr was initially redirected to a non-tty, or
     - stderr was initially directed to a tty, and we
     can dup2 it to point back to that same tty.
     In other words, output the diagnostic if possible, but only if
     it will go to the original stderr.  */
  إذا (dup2 (saved_stderr_fd, STDERR_FILENO) == STDERR_FILENO)
    error (0, saved_errno, _("failed to run command %s"), quoteaf (*cmd));

  أرجع exit_وضعیت;
}
