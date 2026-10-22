/* timeout -- run a command with bounded time
   Copyright (C) 2008-2026 Free Software Foundation, Inc.

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


/* timeout - Start a command, and kill it if the specified timeout expires

   We try to behave like a shell starting a single (foreground) job,
   and will kill the job if we receive the alarm signal we setup.
   The exit status of the job is returned, or one of these errors:
     EXIT_TIMEDOUT      124      job timed out
     EXIT_CANCELED      125      internal error
     EXIT_CANNOT_INVOKE 126      error executing job
     EXIT_ENOENT        127      couldn't find job to exec

   Caveats:
     If user specifies the KILL (9) signal is to be sent on timeout,
     the monitor is killed and so exits with 128+9 rather than 124.

     If you start a command in the background, which reads from the tty
     and so is immediately sent SIGTTIN to stop, then the timeout
     process will ignore this so it can timeout the command as expected.
     This can be seen with 'timeout 10 dd&' for example.
     However if one brings this group to the foreground with the 'fg'
     command before the timer expires, the command will remain
     in the stop state as the shell doesn't send a SIGCONT
     because the timeout process (group leader) is already running.
     To get the command running again one can Ctrl-Z, and do fg again.
     Note one can Ctrl-C the whole job when in this state.
     I think this could be fixed but I'm not sure the extra
     complication is justified for this scenario.

   Written by Pádraig Brady.  */

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <signal.h>
#যদি HAVE_PRCTL
# include <sys/prctl.h>
#endif
#অন্তর্ভাৱ <sys/wait.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "cl-strtod.h"
#অন্তর্ভাৱ "dtimespec-bound.h"
#অন্তর্ভাৱ "sig2str.h"
#অন্তর্ভাৱ "operand2sig.h"
#অন্তর্ভাৱ "term-sig.h"
#অন্তর্ভাৱ "quote.h"

#যদি HAVE_SETRLIMIT
/* FreeBSD 5.0 at least needs <sys/types.h> and <sys/time.h> included
   before <sys/resource.h>.  Currently "system.h" includes <sys/time.h>.  */
# include <sys/resource.h>
#endif

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt.  */
#ifndef SA_RESTART
# define SA_RESTART 0
#endif

#ifndef SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#endif
#ifndef SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#endif

#define PROGRAM_NAME "timeout"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

স্থির পূর্ণাঙ্ক timed_out;
স্থির পূর্ণাঙ্ক term_signal = SIGTERM;  /* same default as kill command.  */
স্থির pid_t monitored_pid;
স্থির দ্ৱিগুণ kill_after;
স্থির bool foreground;      /* whether to use another program group.  */
স্থির bool preserve_status; /* whether to use a timeout status or not.  */
স্থির bool verbose;         /* whether to diagnose timeouts or not.  */
স্থির ৱর্ণ ধ্রুৱ *command;

স্থির সংরচনা option ধ্রুৱ long_options[] =
{
  {"foreground", no_argument, রিক্ত, 'f'},
  {"kill-after", required_argument, রিক্ত, 'k'},
  {"preserve-status", no_argument, রিক্ত, 'p'},
  {"signal", required_argument, রিক্ত, 's'},
  {"verbose", no_argument, রিক্ত, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Start the timeout after which we'll receive a SIGALRM.
   Round DURATION up to the next representable value.
   Treat out-of-range values as if they were maximal,
   as that's more useful in practice than reporting an error.
   '0' means don't timeout.  */
স্থির শূন্য
settimeout (দ্ৱিগুণ duration, bool warn)
{

#যদি HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  সংরচনা timespec ts = dtotimespec (duration);
  সংরচনা itimerspec its = {.it_interval = {0}, .it_মান = ts};
  timer_t timerid;
  যদি (timer_create (CLOCK_REALTIME, রিক্ত, &timerid) == 0)
    {
      যদি (timer_settime (timerid, 0, &its, রিক্ত) == 0)
        ঘূৰাই;
      অন্যথা
        {
          যদি (warn)
            error (0, errno, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  অন্যথা যদি (warn && errno != ENOSYS)
    error (0, errno, _("warning: timer_create"));

#নোচেদ্যদি HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  সংরচনা timeval tv;
  সংরচনা timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  যদি (tv.tv_usec == 1000 * 1000)
    {
      যদি (tv.tv_sec != TYPE_MAXIMUM (time_t))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      অন্যথা
        tv.tv_usec--;
    }
  সংরচনা itimerval it = {.it_interval = {0}, .it_মান = tv };
  যদি (setitimer (ITIMER_REAL, &it, রিক্ত) == 0)
    ঘূৰাই;
  অন্যথা
    {
      যদি (warn && errno != ENOSYS)
        error (0, errno, _("warning: setitimer"));
    }
#endif

  /* fallback to single second resolution provided by alarm().  */

  unsigned পূর্ণাঙ্ক timeint;
  যদি (UINT_MAX <= duration)
    timeint = UINT_MAX;
  অন্যথা
    {
      unsigned পূর্ণাঙ্ক duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

স্থির পূর্ণাঙ্ক
send_sig (pid_t where, পূর্ণাঙ্ক sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  যদি (where == 0)
    সঙ্কেত (sig, SIG_IGN);
  ঘূৰাই kill (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
স্থির শূন্য
chld (MAYBE_UNUSED পূর্ণাঙ্ক sig)
{
}

স্থির শূন্য
cleanup (পূর্ণাঙ্ক sig)
{
  যদি (sig == SIGALRM)
    {
      timed_out = 1;
      /* In case there is an issue with close_stdout,
         update to a more accurate default exit status.
         For example we might get failed writes with -v with:
           timeout -v 1 sleep 10 2>&1 | :
      */
      initialize_exit_failure (EXIT_TIMEDOUT);
      sig = term_signal;
    }
  যদি (0 < monitored_pid)
    {
      যদি (kill_after)
        {
          পূর্ণাঙ্ক saved_errno = errno; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          term_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          errno = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      যদি (verbose)
        {
          ৱর্ণ signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (পূর্ণাঙ্ক))];
          যদি (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, পরিমাণ signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      যদি (!foreground)
        {
          send_sig (0, sig);
          যদি (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  অন্যথা যদি (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  অন্যথা /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), program_name);

      fputs (_("\
Start COMMAND, and kill it যদি still running after DURATION.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --foreground\n\
         when not running timeout directly from a shell prompt,\n\
         allow COMMAND to পঠ from the TTY and get TTY signals;\n\
         in this mode, children of COMMAND will not be timed out\n\
"));
      oputs (_("\
  -k, --kill-after=DURATION\n\
         also send a KILL সঙ্কেত যদি COMMAND is still running\n\
         this long after the initial সঙ্কেত was sent\n\
"));
      oputs (_("\
  -p, --preserve-status\n\
         exit with the same status as COMMAND,\n\
         even when the command times out\n\
"));
      oputs (_("\
  -s, --সঙ্কেত=SIGNAL\n\
         specify the সঙ্কেত to be sent on timeout;\n\
         SIGNAL may be a name like 'HUP' or a সংখ্যা;\n\
         see 'kill -l' বাবে a list of signals\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose to standard error any সঙ্কেত sent upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
DURATION is a floating point সংখ্যা with an optional suffix:\n\
's' বাবে seconds (the স্ৱভাৱ), 'm' বাবে minutes, 'h' বাবে hours or \
'd' বাবে days.\nA duration of 0 disables the associated timeout.\n"), stdout);

      fputs (_("\n\
Upon timeout, send the TERM সঙ্কেত to COMMAND, যদি no other SIGNAL specified.\n\
The TERM সঙ্কেত kills any process that does not block or catch that সঙ্কেত.\n\
It may be necessary to use the KILL সঙ্কেত, since this সঙ্কেত can't be caught.\
\n"), stdout);

      fputs (_("\n\
Exit status:\n\
  124  যদি COMMAND times out, and --preserve-status is not specified\n\
  125  যদি the timeout command itself fails\n\
  126  যদি COMMAND is found but cannot be invoked\n\
  127  যদি COMMAND cannot be found\n\
  137  যদি COMMAND (or timeout itself) is sent the KILL (9) সঙ্কেত (128+9)\n\
  -    the exit status of COMMAND otherwise\n\
"), stdout);

      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

স্থির bool
apply_time_suffix (দ্ৱিগুণ *x, ৱর্ণ suffix_char)
{
  পূর্ণাঙ্ক multiplier;

  ৱিকল্প (suffix_char)
    {
    পক্ষ 0:
    পক্ষ 's':
      multiplier = 1;
      ভঙ্গ;
    পক্ষ 'm':
      multiplier = 60;
      ভঙ্গ;
    পক্ষ 'h':
      multiplier = 60 * 60;
      ভঙ্গ;
    পক্ষ 'd':
      multiplier = 60 * 60 * 24;
      ভঙ্গ;
    স্ৱভাৱ:
      ঘূৰাই false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  ঘূৰাই true;
}

স্থির দ্ৱিগুণ
parse_duration (ৱর্ণ ধ্রুৱ *str)
{
  ৱর্ণ *ep;
  errno = 0;
  দ্ৱিগুণ duration = cl_strtod (str, &ep);
  দ্ৱিগুণ s = dtimespec_bound (duration, errno);

  যদি (ep == str
      /* Nonnegative interval.  */
      || ! (0 <= s)
      /* No extra chars after the number and an optional s,m,h,d char.  */
      || (*ep && *(ep + 1))
      /* Check any suffix char and update timeout based on the suffix.  */
      || !apply_time_suffix (&s, *ep))
    {
      error (0, 0, _("invalid time interval %s"), quote (str));
      usage (EXIT_CANCELED);
    }

  ঘূৰাই s;
}

স্থির শূন্য
unblock_signal (পূর্ণাঙ্ক sig)
{
  sigset_t unblock_set;
  sigemptyset (&unblock_set);
  sigaddset (&unblock_set, sig);
  যদি (sigprocmask (SIG_UNBLOCK, &unblock_set, রিক্ত) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

স্থির শূন্য
install_sigchld (শূন্য)
{
  সংরচনা sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = chld;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  sigaction (SIGCHLD, &sa, রিক্ত);

  /* We inherit the signal mask from our parent process,
     so ensure SIGCHLD is not blocked. */
  unblock_signal (SIGCHLD);
}

/* Filter out signals that were ignored.  */

স্থির bool
sig_needs_handling (পূর্ণাঙ্ক sig, পূর্ণাঙ্ক sigterm)
{
  যদি (sig == SIGALRM || sig == sigterm)
    ঘূৰাই true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  সংরচনা sigaction old_sa;
  sigaction (sig, রিক্ত, &old_sa);
  bool ret = old_sa.sa_handler != SIG_IGN;
  ঘূৰাই ret;
}

স্থির শূন্য
install_cleanup (পূর্ণাঙ্ক sigterm)
{
  সংরচনা sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  বাবে (পূর্ণাঙ্ক i = 0; i < countof (term_sig); i++)
    যদি (sig_needs_handling (term_sig[i], sigterm))
      sigaction (term_sig[i], &sa, রিক্ত);

  /* Real Time signals also terminate by default.  */
  বাবে (পূর্ণাঙ্ক s = SIGRTMIN; s <= SIGRTMAX; s++)
    যদি (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, রিক্ত);

  sigaction (sigterm, &sa, রিক্ত); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
স্থির শূন্য
block_cleanup_and_chld (পূর্ণাঙ্ক sigterm, sigset_t *old_set)
{
  sigset_t block_set;
  sigemptyset (&block_set);

  বাবে (পূর্ণাঙ্ক i = 0; i < countof (term_sig); i++)
    যদি (sig_needs_handling (term_sig[i], sigterm))
      sigaddset (&block_set, term_sig[i]);

  বাবে (পূর্ণাঙ্ক s = SIGRTMIN; s <= SIGRTMAX; s++)
    যদি (sig_needs_handling (s, sigterm))
      sigaddset (&block_set, s);

  sigaddset (&block_set, sigterm);

  sigaddset (&block_set, SIGCHLD);

  যদি (sigprocmask (SIG_BLOCK, &block_set, old_set) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
স্থির bool
disable_core_dumps (শূন্য)
{
#যদি HAVE_PRCTL && defined PR_SET_DUMPABLE
  যদি (prctl (PR_SET_DUMPABLE, 0) == 0)
    ঘূৰাই true;

#নোচেদ্যদি HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  যদি (setrlimit (RLIMIT_CORE, &(সংরচনা rlimit) {0}) == 0)
    ঘূৰাই true;

#অন্যথা
  ঘূৰাই false;
#endif

  error (0, errno, _("warning: disabling core dumps failed"));
  ঘূৰাই false;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  দ্ৱিগুণ timeout;
  পূর্ণাঙ্ক c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  যেতিয়ালৈ ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, রিক্ত))
         != -1)
    {
      ৱিকল্প (c)
        {
        পক্ষ 'f':
          foreground = true;
          ভঙ্গ;

        পক্ষ 'k':
          kill_after = parse_duration (optarg);
          ভঙ্গ;

        পক্ষ 'p':
          preserve_status = true;
          ভঙ্গ;

        পক্ষ 's':
          term_signal = operand2sig (optarg);
          যদি (term_signal == -1)
            usage (EXIT_CANCELED);
          ভঙ্গ;

        পক্ষ 'v':
          verbose = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্ৱভাৱ:
          usage (EXIT_CANCELED);
          ভঙ্গ;
        }
    }

  যদি (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  যদি (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (term_signal);
  সঙ্কেত (SIGTTIN, SIG_IGN);   /* Don't stop if background child needs tty.  */
  সঙ্কেত (SIGTTOU, SIG_IGN);   /* Don't stop if background child needs tty.  */
  install_sigchld ();          /* Interrupt sigsuspend() when child exits.   */

  /* We configure timers so that SIGALRM is sent on expiry.
     Therefore ensure we don't inherit a mask blocking SIGALRM.  */
  unblock_signal (SIGALRM);

  /* Block signals now, so monitored_pid is deterministic in cleanup().  */
  sigset_t orig_set;
  block_cleanup_and_chld (term_signal, &orig_set);

  /* Get the parent process ID so we can check if we are reparented later.
     Traditionally processes would be reparented to init.  On Linux a process
     can be come a subreaper using PR_SET_CHILD_SUBREAPER to fulfill the role
     of init for child processes, as is done by systemd(1).  */
  pid_t timeout_pid = getpid ();

  /* We cannot use posix_spawn here since the child will have an exit status of
     127 for any failure.  If implemented through fork and exec, posix_spawn
     will return successfully and 'timeout' will have no way to determine if it
     should exit with EXIT_CANNOT_INVOKE or EXIT_ENOENT upon checking the exit
     status of the child.  */
  monitored_pid = fork ();
  যদি (monitored_pid == -1)
    {
      error (0, errno, _("fork system call failed"));
      ঘূৰাই EXIT_CANCELED;
    }
  অন্যথা যদি (monitored_pid == 0)  /* child */
    {
#যদি HAVE_PRCTL && defined PR_SET_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_SET_PDEATHSIG, term_signal);
#endif
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      যদি (getppid () != timeout_pid)
        ঘূৰাই EXIT_CANCELED;

      /* Restore signal mask for child.  */
      যদি (sigprocmask (SIG_SETMASK, &orig_set, রিক্ত) != 0)
        {
          error (0, errno, _("child failed to reset signal mask"));
          ঘূৰাই EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      সঙ্কেত (SIGTTIN, SIG_DFL);
      সঙ্কেত (SIGTTOU, SIG_DFL);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      পূর্ণাঙ্ক exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, errno, _("failed to run command %s"), quote (command));
      ঘূৰাই exit_status;
    }
  অন্যথা
    {
      pid_t wait_result;
      পূর্ণাঙ্ক status;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      যেতিয়ালৈ ((wait_result = waitpid (monitored_pid, &status, WNOHANG)) == 0)
        sigsuspend (&orig_set);  /* Wait with cleanup signals unblocked.  */

      যদি (wait_result < 0)
        {
          /* shouldn't happen.  */
          error (0, errno, _("error waiting for command"));
          status = EXIT_CANCELED;
        }
      অন্যথা
        {
          যদি (WIFEXITED (status))
            status = WEXITSTATUS (status);
          অন্যথা যদি (WIFSIGNALED (status))
            {
              পূর্ণাঙ্ক sig = WTERMSIG (status);
              যদি (WCOREDUMP (status))
                error (0, 0, _("the monitored command dumped core"));
              যদি (!timed_out && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  সঙ্কেত (sig, SIG_DFL);
                  unblock_signal (sig);
                  উত্থাপয (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              যদি (timed_out && sig == SIGKILL)
                preserve_status = true;
              status = sig + 128; /* what sh returns for signaled processes.  */
            }
          অন্যথা
            {
              /* shouldn't happen.  */
              error (0, 0, _("unknown status from command (%d)"), status);
              status = EXIT_FAILURE;
            }
        }

      যদি (timed_out && !preserve_status)
        status = EXIT_TIMEDOUT;
      ঘূৰাই status;
    }
}
