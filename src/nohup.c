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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "filenamecat.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fd-reopen.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "long-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unistd--.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "nohup"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Jim Meyering")

/* Exit statuses.  */
ਗਿਣਤੀ
  {
    /* 'nohup' itself failed.  */
    POSIX_NOHUP_FAILURE = 127
  };

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s COMMAND [ARG]...\n\
  or:  %s OPTION\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);

      fputs (_("\
ਲੜੀਵਾਰ COMMAND, ignoring hangup ਸੰਕੇਤ_2.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      ਲਿਖੋ (_("\n\
If standard ਨਿਵੇਸ਼ is a terminal, redirect it from an unreadable file.\n\
If standard output is a terminal, append output ਮੰਜ਼ਿਲ 'nohup.out' ਜੇ possible,\n\
'$HOME/nohup.out' otherwise.\n\
If standard error is a terminal, redirect it ਮੰਜ਼ਿਲ standard output.\n\
ਮੰਜ਼ਿਲ_2 save output ਮੰਜ਼ਿਲ FILE, use '%s COMMAND > FILE'.\n"),
              ਪ੍ਰੋਗਰਾਮ_name);
      ਲਿਖੋ (USAGE_BUILTIN_WARNING, ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_exec_ਸਥਿਤੀ (ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* GCC 13 gets confused by the dup2 calls
   <https://gcc.gnu.org/PR109839>.  */
#ਜੇ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-fd-leak"
#ਜੇ_ਅੰਤ

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* POSIX 2008 requires that internal failure give status 127; unlike
     for env, exec, nice, time, and xargs where it requires internal
     failure give something in the range 1-125.  For consistency with
     other tools, fail with EXIT_CANCELED unless POSIXLY_CORRECT.  */
  ਪੂਰਨ exit_ਅੰਦਰੂਨੀ_failure = (getenv ("POSIXLY_CORRECT")
                               ? POSIX_NOHUP_FAILURE : EXIT_CANCELED);
  initialize_exit_failure (exit_ਅੰਦਰੂਨੀ_failure);
  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE_NAME,
                                   ਸੰਸਕਰਣ, false, usage, AUTHORS,
                                   (ਅੱਖਰ ਅਚਲ *) ਨਿਰਾ);

  ਜੇ (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (exit_ਅੰਦਰੂਨੀ_failure);
    }

  bool ignoring_ਨਿਵੇਸ਼ = isatty (STDIN_FILENO);
  bool redirecting_stdout = isatty (STDOUT_FILENO);
  bool stdout_is_closed = (!redirecting_stdout && errno == EBADF);
  bool redirecting_stderr = isatty (STDERR_FILENO);

  /* If standard input is a tty, replace it with /dev/null if possible.
     Note that it is deliberately opened for *writing*,
     to ensure any read evokes an error.  */
  ਜੇ (ignoring_ਨਿਵੇਸ਼)
    {
      ਜੇ (fd_reopen (STDIN_FILENO, "/dev/null", O_WRONLY, 0) < 0)
        error (exit_ਅੰਦਰੂਨੀ_failure, errno,
               _("failed to render standard input unusable"));
      ਜੇ (!redirecting_stdout && !redirecting_stderr)
        error (0, 0, _("ignoring input"));
    }

  /* If standard output is a tty, redirect it (appending) to a file.
     First try nohup.out, then $HOME/nohup.out.  If standard error is
     a tty and standard output is closed, open nohup.out or
     $HOME/nohup.out without redirecting anything.  */
  ਪੂਰਨ ਬਾਹਰਲਾ_fd = STDOUT_FILENO;
  ਜੇ (redirecting_stdout || (redirecting_stderr && stdout_is_closed))
    {
      ਅੱਖਰ *ਵਿੱਚ_home = ਨਿਰਾ;
      ਅੱਖਰ ਅਚਲ *file = "nohup.out";
      ਪੂਰਨ flags = O_CREAT | O_WRONLY | O_APPEND;
      mode_t ਬਹੁਲਕ_2 = S_IRUSR | S_IWUSR;
      mode_t umask_ਮੁੱਲ = umask (0);
      ਬਾਹਰਲਾ_fd = (redirecting_stdout
                ? fd_reopen (STDOUT_FILENO, file, flags, ਬਹੁਲਕ_2)
                : open (file, flags, ਬਹੁਲਕ_2));

      ਜੇ (ਬਾਹਰਲਾ_fd < 0)
        {
          ਪੂਰਨ saved_errno = errno;
          ਅੱਖਰ ਅਚਲ *home = getenv ("HOME");
          ਜੇ (home)
            {
              ਵਿੱਚ_home = file_name_concat (home, file, ਨਿਰਾ);
              ਬਾਹਰਲਾ_fd = (redirecting_stdout
                        ? fd_reopen (STDOUT_FILENO, ਵਿੱਚ_home, flags, ਬਹੁਲਕ_2)
                        : open (ਵਿੱਚ_home, flags, ਬਹੁਲਕ_2));
            }
          ਜੇ (ਬਾਹਰਲਾ_fd < 0)
            {
              ਪੂਰਨ saved_errno2 = errno;
              error (0, saved_errno, _("failed to open %s"), quoteaf (file));
              ਜੇ (ਵਿੱਚ_home)
                error (0, saved_errno2, _("failed to open %s"),
                       quoteaf (ਵਿੱਚ_home));
              ਵਾਪਸ exit_ਅੰਦਰੂਨੀ_failure;
            }
          file = ਵਿੱਚ_home;
        }

      umask (umask_ਮੁੱਲ);
      error (0, 0,
             _(ignoring_ਨਿਵੇਸ਼
               ? N_("ignoring input and appending output to %s")
               : N_("appending output to %s")),
             quoteaf (file));
      ਮੁਕਤ (ਵਿੱਚ_home);
    }

  /* If standard error is a tty, redirect it.  */
  ਪੂਰਨ saved_stderr_fd = STDERR_FILENO;
  ਜੇ (redirecting_stderr)
    {
      /* Save a copy of stderr before redirecting, so we can use the original
         if execve fails.  It's no big deal if this dup fails.  It might
         not change anything, and at worst, it'll lead to suppression of
         the post-failed-execve diagnostic.  */
      saved_stderr_fd = fcntl (STDERR_FILENO, F_DUPFD_CLOEXEC,
                               STDERR_FILENO + 1);

      ਜੇ (!redirecting_stdout)
        error (0, 0,
               _(ignoring_ਨਿਵੇਸ਼
                 ? N_("ignoring input and redirecting standard error "
                      "to standard output")
                 : N_("redirecting standard error to standard output")));

      ਜੇ (dup2 (ਬਾਹਰਲਾ_fd, STDERR_FILENO) < 0)
        error (exit_ਅੰਦਰੂਨੀ_failure, errno,
               _("failed to redirect standard error"));

      ਜੇ (stdout_is_closed)
        close (ਬਾਹਰਲਾ_fd);
    }

  /* error() flushes stderr, but does not check for write failure.
     Normally, we would catch this via our atexit() hook of
     close_stdout, but execvp() gets in the way.  If stderr
     encountered a write failure, there is no need to try calling
     error() again, particularly since we may have just changed the
     underlying fd out from under stderr.  */
  ਜੇ (ferror (stderr))
    ਵਾਪਸ exit_ਅੰਦਰੂਨੀ_failure;

  /* Windows does not support SIGHUP.  */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SIGHUP
  ਸੰਕੇਤ (SIGHUP, SIG_IGN);
#ਜੇ_ਅੰਤ

  ਅੱਖਰ **cmd = argv + optind;
  execvp (*cmd, cmd);
  ਪੂਰਨ exit_ਸਥਿਤੀ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  ਪੂਰਨ saved_errno = errno;

  /* The execve failed.  Output a diagnostic to stderr only if:
     - stderr was initially redirected to a non-tty, or
     - stderr was initially directed to a tty, and we
     can dup2 it to point back to that same tty.
     In other words, output the diagnostic if possible, but only if
     it will go to the original stderr.  */
  ਜੇ (dup2 (saved_stderr_fd, STDERR_FILENO) == STDERR_FILENO)
    error (0, saved_errno, _("failed to run command %s"), quoteaf (*cmd));

  ਵਾਪਸ exit_ਸਥਿਤੀ;
}
