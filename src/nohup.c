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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "filenamecat.h"
#ଅନ୍ତର୍ଭାଵ "fd-reopen.h"
#ଅନ୍ତର୍ଭାଵ "long-options.h"
#ଅନ୍ତର୍ଭାଵ "unistd--.h"

#define PROGRAM_NAME "nohup"

#define AUTHORS proper_name ("Jim Meyering")

/* Exit statuses.  */
ଗଣନା
  {
    /* 'nohup' itself failed.  */
    POSIX_NOHUP_FAILURE = 127
  };

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s COMMAND [ARG]...\n\
  or:  %s OPTION\n\
"),
              program_name, program_name);

      fputs (_("\
Run COMMAND, ignoring hangup signals.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\n\
If standard ନିବେଶ is a terminal, redirect it from an unreadable file.\n\
If standard output is a terminal, append output to 'nohup.out' ଯଦି possible,\n\
'$HOME/nohup.out' otherwise.\n\
If standard error is a terminal, redirect it to standard output.\n\
To save output to FILE, use '%s COMMAND > FILE'.\n"),
              program_name);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* GCC 13 gets confused by the dup2 calls
   <https://gcc.gnu.org/PR109839>.  */
#ଯଦି 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-fd-leak"
#endif

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* POSIX 2008 requires that internal failure give status 127; unlike
     for env, exec, nice, time, and xargs where it requires internal
     failure give something in the range 1-125.  For consistency with
     other tools, fail with EXIT_CANCELED unless POSIXLY_CORRECT.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_internal_failure = (getenv ("POSIXLY_CORRECT")
                               ? POSIX_NOHUP_FAILURE : EXIT_CANCELED);
  initialize_exit_failure (exit_internal_failure);
  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, false, usage, AUTHORS,
                                   (ଅକ୍ଷର ଅଚଳ *) ଖାଲି);

  ଯଦି (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (exit_internal_failure);
    }

  bool ignoring_ନିବେଶ = isatty (STDIN_FILENO);
  bool redirecting_stdout = isatty (STDOUT_FILENO);
  bool stdout_is_closed = (!redirecting_stdout && errno == EBADF);
  bool redirecting_stderr = isatty (STDERR_FILENO);

  /* If standard input is a tty, replace it with /dev/null if possible.
     Note that it is deliberately opened for *writing*,
     to ensure any read evokes an error.  */
  ଯଦି (ignoring_ନିବେଶ)
    {
      ଯଦି (fd_reopen (STDIN_FILENO, "/dev/null", O_WRONLY, 0) < 0)
        error (exit_internal_failure, errno,
               _("failed to render standard input unusable"));
      ଯଦି (!redirecting_stdout && !redirecting_stderr)
        error (0, 0, _("ignoring input"));
    }

  /* If standard output is a tty, redirect it (appending) to a file.
     First try nohup.out, then $HOME/nohup.out.  If standard error is
     a tty and standard output is closed, open nohup.out or
     $HOME/nohup.out without redirecting anything.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_fd = STDOUT_FILENO;
  ଯଦି (redirecting_stdout || (redirecting_stderr && stdout_is_closed))
    {
      ଅକ୍ଷର *in_home = ଖାଲି;
      ଅକ୍ଷର ଅଚଳ *file = "nohup.out";
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = O_CREAT | O_WRONLY | O_APPEND;
      mode_t mode = S_IRUSR | S_IWUSR;
      mode_t umask_ମୂଲ୍ୟ = umask (0);
      out_fd = (redirecting_stdout
                ? fd_reopen (STDOUT_FILENO, file, flags, mode)
                : open (file, flags, mode));

      ଯଦି (out_fd < 0)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
          ଅକ୍ଷର ଅଚଳ *home = getenv ("HOME");
          ଯଦି (home)
            {
              in_home = file_name_concat (home, file, ଖାଲି);
              out_fd = (redirecting_stdout
                        ? fd_reopen (STDOUT_FILENO, in_home, flags, mode)
                        : open (in_home, flags, mode));
            }
          ଯଦି (out_fd < 0)
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno2 = errno;
              error (0, saved_errno, _("failed to open %s"), quoteaf (file));
              ଯଦି (in_home)
                error (0, saved_errno2, _("failed to open %s"),
                       quoteaf (in_home));
              ଫେରନ୍ତୁ exit_internal_failure;
            }
          file = in_home;
        }

      umask (umask_ମୂଲ୍ୟ);
      error (0, 0,
             _(ignoring_ନିବେଶ
               ? N_("ignoring input and appending output to %s")
               : N_("appending output to %s")),
             quoteaf (file));
      ମୁକ୍ତ (in_home);
    }

  /* If standard error is a tty, redirect it.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_stderr_fd = STDERR_FILENO;
  ଯଦି (redirecting_stderr)
    {
      /* Save a copy of stderr before redirecting, so we can use the original
         if execve fails.  It's no big deal if this dup fails.  It might
         not change anything, and at worst, it'll lead to suppression of
         the post-failed-execve diagnostic.  */
      saved_stderr_fd = fcntl (STDERR_FILENO, F_DUPFD_CLOEXEC,
                               STDERR_FILENO + 1);

      ଯଦି (!redirecting_stdout)
        error (0, 0,
               _(ignoring_ନିବେଶ
                 ? N_("ignoring input and redirecting standard error "
                      "to standard output")
                 : N_("redirecting standard error to standard output")));

      ଯଦି (dup2 (out_fd, STDERR_FILENO) < 0)
        error (exit_internal_failure, errno,
               _("failed to redirect standard error"));

      ଯଦି (stdout_is_closed)
        close (out_fd);
    }

  /* error() flushes stderr, but does not check for write failure.
     Normally, we would catch this via our atexit() hook of
     close_stdout, but execvp() gets in the way.  If stderr
     encountered a write failure, there is no need to try calling
     error() again, particularly since we may have just changed the
     underlying fd out from under stderr.  */
  ଯଦି (ferror (stderr))
    ଫେରନ୍ତୁ exit_internal_failure;

  /* Windows does not support SIGHUP.  */
#ifdef SIGHUP
  ସଙ୍କେତ (SIGHUP, SIG_IGN);
#endif

  ଅକ୍ଷର **cmd = argv + optind;
  execvp (*cmd, cmd);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;

  /* The execve failed.  Output a diagnostic to stderr only if:
     - stderr was initially redirected to a non-tty, or
     - stderr was initially directed to a tty, and we
     can dup2 it to point back to that same tty.
     In other words, output the diagnostic if possible, but only if
     it will go to the original stderr.  */
  ଯଦି (dup2 (saved_stderr_fd, STDERR_FILENO) == STDERR_FILENO)
    error (0, saved_errno, _("failed to run command %s"), quoteaf (*cmd));

  ଫେରନ୍ତୁ exit_status;
}
