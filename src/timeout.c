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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <signal.h>
#જો HAVE_PRCTL
# include <sys/prctl.h>
#endif
#અન્તર્ભાવ <sys/wait.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "cl-strtod.h"
#અન્તર્ભાવ "dtimespec-bound.h"
#અન્તર્ભાવ "sig2str.h"
#અન્તર્ભાવ "operand2sig.h"
#અન્તર્ભાવ "term-sig.h"
#અન્તર્ભાવ "quote.h"

#જો HAVE_SETRLIMIT
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

સ્થિર પૂર્ણાંક timed_out;
સ્થિર પૂર્ણાંક term_signal = SIGTERM;  /* same default as kill command.  */
સ્થિર pid_t monitored_pid;
સ્થિર દ્વિગુણ kill_after;
સ્થિર bool foreground;      /* whether to use another program group.  */
સ્થિર bool preserve_status; /* whether to use a timeout status or not.  */
સ્થિર bool verbose;         /* whether to diagnose timeouts or not.  */
સ્થિર અક્ષર અચલ *command;

સ્થિર સંરચના option અચલ long_options[] =
{
  {"foreground", no_argument, ખાલી, 'f'},
  {"kill-after", required_argument, ખાલી, 'k'},
  {"preserve-status", no_argument, ખાલી, 'p'},
  {"signal", required_argument, ખાલી, 's'},
  {"verbose", no_argument, ખાલી, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Start the timeout after which we'll receive a SIGALRM.
   Round DURATION up to the next representable value.
   Treat out-of-range values as if they were maximal,
   as that's more useful in practice than reporting an error.
   '0' means don't timeout.  */
સ્થિર શૂન્ય
settimeout (દ્વિગુણ duration, bool warn)
{

#જો HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  સંરચના timespec ts = dtotimespec (duration);
  સંરચના itimerspec its = {.it_interval = {0}, .it_કિંમત = ts};
  timer_t timerid;
  જો (timer_create (CLOCK_REALTIME, ખાલી, &timerid) == 0)
    {
      જો (timer_settime (timerid, 0, &its, ખાલી) == 0)
        પાછા;
      નહીંતર
        {
          જો (warn)
            error (0, errno, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  નહીંતર જો (warn && errno != ENOSYS)
    error (0, errno, _("warning: timer_create"));

#અથવાજો HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  સંરચના timeval tv;
  સંરચના timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  જો (tv.tv_usec == 1000 * 1000)
    {
      જો (tv.tv_sec != TYPE_MAXIMUM (time_t))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      નહીંતર
        tv.tv_usec--;
    }
  સંરચના itimerval it = {.it_interval = {0}, .it_કિંમત = tv };
  જો (setitimer (ITIMER_REAL, &it, ખાલી) == 0)
    પાછા;
  નહીંતર
    {
      જો (warn && errno != ENOSYS)
        error (0, errno, _("warning: setitimer"));
    }
#endif

  /* fallback to single second resolution provided by alarm().  */

  unsigned પૂર્ણાંક timeint;
  જો (UINT_MAX <= duration)
    timeint = UINT_MAX;
  નહીંતર
    {
      unsigned પૂર્ણાંક duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

સ્થિર પૂર્ણાંક
send_sig (pid_t where, પૂર્ણાંક sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  જો (where == 0)
    સંકેત (sig, SIG_IGN);
  પાછા kill (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
સ્થિર શૂન્ય
chld (MAYBE_UNUSED પૂર્ણાંક sig)
{
}

સ્થિર શૂન્ય
cleanup (પૂર્ણાંક sig)
{
  જો (sig == SIGALRM)
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
  જો (0 < monitored_pid)
    {
      જો (kill_after)
        {
          પૂર્ણાંક saved_errno = errno; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          term_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          errno = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      જો (verbose)
        {
          અક્ષર signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (પૂર્ણાંક))];
          જો (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, કદ signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      જો (!foreground)
        {
          send_sig (0, sig);
          જો (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  નહીંતર જો (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  નહીંતર /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), program_name);

      fputs (_("\
Start COMMAND, and kill it જો still running after DURATION.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --foreground\n\
         when not running timeout directly from a shell prompt,\n\
         allow COMMAND to પઠ from the TTY and get TTY signals;\n\
         in this mode, children of COMMAND will not be timed out\n\
"));
      oputs (_("\
  -k, --kill-after=DURATION\n\
         also send a KILL સંકેત જો COMMAND is still running\n\
         this long after the initial સંકેત was sent\n\
"));
      oputs (_("\
  -p, --preserve-status\n\
         exit with the same status as COMMAND,\n\
         even when the command times out\n\
"));
      oputs (_("\
  -s, --સંકેત=SIGNAL\n\
         specify the સંકેત to be sent on timeout;\n\
         SIGNAL may be a name like 'HUP' or a સંખ્યા;\n\
         see 'kill -l' માટે a list of signals\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose to standard error any સંકેત sent upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
DURATION is a floating point સંખ્યા with an optional suffix:\n\
's' માટે seconds (the મૂળભૂત), 'm' માટે minutes, 'h' માટે hours or \
'd' માટે days.\nA duration of 0 disables the associated timeout.\n"), stdout);

      fputs (_("\n\
Upon timeout, send the TERM સંકેત to COMMAND, જો no other SIGNAL specified.\n\
The TERM સંકેત kills any process that does not block or catch that સંકેત.\n\
It may be necessary to use the KILL સંકેત, since this સંકેત can't be caught.\
\n"), stdout);

      fputs (_("\n\
Exit status:\n\
  124  જો COMMAND times out, and --preserve-status is not specified\n\
  125  જો the timeout command itself fails\n\
  126  જો COMMAND is found but cannot be invoked\n\
  127  જો COMMAND cannot be found\n\
  137  જો COMMAND (or timeout itself) is sent the KILL (9) સંકેત (128+9)\n\
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

સ્થિર bool
apply_time_suffix (દ્વિગુણ *x, અક્ષર suffix_char)
{
  પૂર્ણાંક multiplier;

  પસંદગી (suffix_char)
    {
    સ્થિતિ 0:
    સ્થિતિ 's':
      multiplier = 1;
      તોડો;
    સ્થિતિ 'm':
      multiplier = 60;
      તોડો;
    સ્થિતિ 'h':
      multiplier = 60 * 60;
      તોડો;
    સ્થિતિ 'd':
      multiplier = 60 * 60 * 24;
      તોડો;
    મૂળભૂત:
      પાછા false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  પાછા true;
}

સ્થિર દ્વિગુણ
parse_duration (અક્ષર અચલ *str)
{
  અક્ષર *ep;
  errno = 0;
  દ્વિગુણ duration = cl_strtod (str, &ep);
  દ્વિગુણ s = dtimespec_bound (duration, errno);

  જો (ep == str
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

  પાછા s;
}

સ્થિર શૂન્ય
unblock_signal (પૂર્ણાંક sig)
{
  sigset_t unblock_set;
  sigemptyset (&unblock_set);
  sigaddset (&unblock_set, sig);
  જો (sigprocmask (SIG_UNBLOCK, &unblock_set, ખાલી) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

સ્થિર શૂન્ય
install_sigchld (શૂન્ય)
{
  સંરચના sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = chld;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  sigaction (SIGCHLD, &sa, ખાલી);

  /* We inherit the signal mask from our parent process,
     so ensure SIGCHLD is not blocked. */
  unblock_signal (SIGCHLD);
}

/* Filter out signals that were ignored.  */

સ્થિર bool
sig_needs_handling (પૂર્ણાંક sig, પૂર્ણાંક sigterm)
{
  જો (sig == SIGALRM || sig == sigterm)
    પાછા true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  સંરચના sigaction old_sa;
  sigaction (sig, ખાલી, &old_sa);
  bool ret = old_sa.sa_handler != SIG_IGN;
  પાછા ret;
}

સ્થિર શૂન્ય
install_cleanup (પૂર્ણાંક sigterm)
{
  સંરચના sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  માટે (પૂર્ણાંક i = 0; i < countof (term_sig); i++)
    જો (sig_needs_handling (term_sig[i], sigterm))
      sigaction (term_sig[i], &sa, ખાલી);

  /* Real Time signals also terminate by default.  */
  માટે (પૂર્ણાંક s = SIGRTMIN; s <= SIGRTMAX; s++)
    જો (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, ખાલી);

  sigaction (sigterm, &sa, ખાલી); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
સ્થિર શૂન્ય
block_cleanup_and_chld (પૂર્ણાંક sigterm, sigset_t *old_set)
{
  sigset_t block_set;
  sigemptyset (&block_set);

  માટે (પૂર્ણાંક i = 0; i < countof (term_sig); i++)
    જો (sig_needs_handling (term_sig[i], sigterm))
      sigaddset (&block_set, term_sig[i]);

  માટે (પૂર્ણાંક s = SIGRTMIN; s <= SIGRTMAX; s++)
    જો (sig_needs_handling (s, sigterm))
      sigaddset (&block_set, s);

  sigaddset (&block_set, sigterm);

  sigaddset (&block_set, SIGCHLD);

  જો (sigprocmask (SIG_BLOCK, &block_set, old_set) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
સ્થિર bool
disable_core_dumps (શૂન્ય)
{
#જો HAVE_PRCTL && defined PR_SET_DUMPABLE
  જો (prctl (PR_SET_DUMPABLE, 0) == 0)
    પાછા true;

#અથવાજો HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  જો (setrlimit (RLIMIT_CORE, &(સંરચના rlimit) {0}) == 0)
    પાછા true;

#નહીંતર
  પાછા false;
#endif

  error (0, errno, _("warning: disabling core dumps failed"));
  પાછા false;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  દ્વિગુણ timeout;
  પૂર્ણાંક c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  જ્યારે ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, ખાલી))
         != -1)
    {
      પસંદગી (c)
        {
        સ્થિતિ 'f':
          foreground = true;
          તોડો;

        સ્થિતિ 'k':
          kill_after = parse_duration (optarg);
          તોડો;

        સ્થિતિ 'p':
          preserve_status = true;
          તોડો;

        સ્થિતિ 's':
          term_signal = operand2sig (optarg);
          જો (term_signal == -1)
            usage (EXIT_CANCELED);
          તોડો;

        સ્થિતિ 'v':
          verbose = true;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_CANCELED);
          તોડો;
        }
    }

  જો (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  જો (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (term_signal);
  સંકેત (SIGTTIN, SIG_IGN);   /* Don't stop if background child needs tty.  */
  સંકેત (SIGTTOU, SIG_IGN);   /* Don't stop if background child needs tty.  */
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
  જો (monitored_pid == -1)
    {
      error (0, errno, _("fork system call failed"));
      પાછા EXIT_CANCELED;
    }
  નહીંતર જો (monitored_pid == 0)  /* child */
    {
#જો HAVE_PRCTL && defined PR_SET_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_SET_PDEATHSIG, term_signal);
#endif
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      જો (getppid () != timeout_pid)
        પાછા EXIT_CANCELED;

      /* Restore signal mask for child.  */
      જો (sigprocmask (SIG_SETMASK, &orig_set, ખાલી) != 0)
        {
          error (0, errno, _("child failed to reset signal mask"));
          પાછા EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      સંકેત (SIGTTIN, SIG_DFL);
      સંકેત (SIGTTOU, SIG_DFL);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      પૂર્ણાંક exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, errno, _("failed to run command %s"), quote (command));
      પાછા exit_status;
    }
  નહીંતર
    {
      pid_t wait_result;
      પૂર્ણાંક status;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      જ્યારે ((wait_result = waitpid (monitored_pid, &status, WNOHANG)) == 0)
        sigsuspend (&orig_set);  /* Wait with cleanup signals unblocked.  */

      જો (wait_result < 0)
        {
          /* shouldn't happen.  */
          error (0, errno, _("error waiting for command"));
          status = EXIT_CANCELED;
        }
      નહીંતર
        {
          જો (WIFEXITED (status))
            status = WEXITSTATUS (status);
          નહીંતર જો (WIFSIGNALED (status))
            {
              પૂર્ણાંક sig = WTERMSIG (status);
              જો (WCOREDUMP (status))
                error (0, 0, _("the monitored command dumped core"));
              જો (!timed_out && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  સંકેત (sig, SIG_DFL);
                  unblock_signal (sig);
                  ઉઠાવો (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              જો (timed_out && sig == SIGKILL)
                preserve_status = true;
              status = sig + 128; /* what sh returns for signaled processes.  */
            }
          નહીંતર
            {
              /* shouldn't happen.  */
              error (0, 0, _("unknown status from command (%d)"), status);
              status = EXIT_FAILURE;
            }
        }

      જો (timed_out && !preserve_status)
        status = EXIT_TIMEDOUT;
      પાછા status;
    }
}
