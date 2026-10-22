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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>
#എങ്കിൽ HAVE_PRCTL
# include <sys/prctl.h>
#endif
#അന്തര്ഭാവ <sys/wait.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "cl-strtod.h"
#അന്തര്ഭാവ "dtimespec-bound.h"
#അന്തര്ഭാവ "sig2str.h"
#അന്തര്ഭാവ "operand2sig.h"
#അന്തര്ഭാവ "term-sig.h"
#അന്തര്ഭാവ "quote.h"

#എങ്കിൽ HAVE_SETRLIMIT
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

സ്ഥിതം പൂർണ്ണസംഖ്യ timed_out;
സ്ഥിതം പൂർണ്ണസംഖ്യ term_signal = SIGTERM;  /* same default as kill command.  */
സ്ഥിതം pid_t monitored_pid;
സ്ഥിതം ഇരട്ട kill_after;
സ്ഥിതം bool foreground;      /* whether to use another program group.  */
സ്ഥിതം bool preserve_status; /* whether to use a timeout status or not.  */
സ്ഥിതം bool verbose;         /* whether to diagnose timeouts or not.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *command;

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"foreground", no_argument, ശൂന്യ, 'f'},
  {"kill-after", required_argument, ശൂന്യ, 'k'},
  {"preserve-status", no_argument, ശൂന്യ, 'p'},
  {"signal", required_argument, ശൂന്യ, 's'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Start the timeout after which we'll receive a SIGALRM.
   Round DURATION up to the next representable value.
   Treat out-of-range values as if they were maximal,
   as that's more useful in practice than reporting an error.
   '0' means don't timeout.  */
സ്ഥിതം ശൂന്യം
settimeout (ഇരട്ട duration, bool warn)
{

#എങ്കിൽ HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  ഘടന timespec ts = dtotimespec (duration);
  ഘടന itimerspec its = {.it_interval = {0}, .it_മൂല്യം = ts};
  timer_t timerid;
  എങ്കിൽ (timer_create (CLOCK_REALTIME, ശൂന്യ, &timerid) == 0)
    {
      എങ്കിൽ (timer_settime (timerid, 0, &its, ശൂന്യ) == 0)
        തിരികെ;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (warn)
            error (0, errno, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (warn && errno != ENOSYS)
    error (0, errno, _("warning: timer_create"));

#അല്ലെങ്കിലെങ്കിൽ HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  ഘടന timeval tv;
  ഘടന timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  എങ്കിൽ (tv.tv_usec == 1000 * 1000)
    {
      എങ്കിൽ (tv.tv_sec != TYPE_MAXIMUM (time_t))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      അല്ലെങ്കിൽ
        tv.tv_usec--;
    }
  ഘടന itimerval it = {.it_interval = {0}, .it_മൂല്യം = tv };
  എങ്കിൽ (setitimer (ITIMER_REAL, &it, ശൂന്യ) == 0)
    തിരികെ;
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (warn && errno != ENOSYS)
        error (0, errno, _("warning: setitimer"));
    }
#endif

  /* fallback to single second resolution provided by alarm().  */

  unsigned പൂർണ്ണസംഖ്യ timeint;
  എങ്കിൽ (UINT_MAX <= duration)
    timeint = UINT_MAX;
  അല്ലെങ്കിൽ
    {
      unsigned പൂർണ്ണസംഖ്യ duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
send_sig (pid_t where, പൂർണ്ണസംഖ്യ sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  എങ്കിൽ (where == 0)
    സൂചന (sig, SIG_IGN);
  തിരികെ kill (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
സ്ഥിതം ശൂന്യം
chld (MAYBE_UNUSED പൂർണ്ണസംഖ്യ sig)
{
}

സ്ഥിതം ശൂന്യം
cleanup (പൂർണ്ണസംഖ്യ sig)
{
  എങ്കിൽ (sig == SIGALRM)
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
  എങ്കിൽ (0 < monitored_pid)
    {
      എങ്കിൽ (kill_after)
        {
          പൂർണ്ണസംഖ്യ saved_errno = errno; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          term_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          errno = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      എങ്കിൽ (verbose)
        {
          അക്ഷരം signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (പൂർണ്ണസംഖ്യ))];
          എങ്കിൽ (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, വലുപ്പം signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      എങ്കിൽ (!foreground)
        {
          send_sig (0, sig);
          എങ്കിൽ (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  അല്ലെങ്കിൽ /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), program_name);

      fputs (_("\
Start COMMAND, and kill it എങ്കിൽ still running after DURATION.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --foreground\n\
         when not running timeout directly from a shell prompt,\n\
         allow COMMAND to പഠ from the TTY and get TTY signals;\n\
         in this mode, children of COMMAND will not be timed out\n\
"));
      oputs (_("\
  -k, --kill-after=DURATION\n\
         also send a KILL സൂചന എങ്കിൽ COMMAND is still running\n\
         this long after the initial സൂചന was sent\n\
"));
      oputs (_("\
  -p, --preserve-status\n\
         exit with the same status as COMMAND,\n\
         even when the command times out\n\
"));
      oputs (_("\
  -s, --സൂചന=SIGNAL\n\
         specify the സൂചന to be sent on timeout;\n\
         SIGNAL may be a name like 'HUP' or a സംഖ്യ;\n\
         see 'kill -l' വേണ്ടി a list of signals\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose to standard error any സൂചന sent upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
DURATION is a floating point സംഖ്യ with an optional suffix:\n\
's' വേണ്ടി seconds (the സ്ഥിരസ്ഥിതി), 'm' വേണ്ടി minutes, 'h' വേണ്ടി hours or \
'd' വേണ്ടി days.\nA duration of 0 disables the associated timeout.\n"), stdout);

      fputs (_("\n\
Upon timeout, send the TERM സൂചന to COMMAND, എങ്കിൽ no other SIGNAL specified.\n\
The TERM സൂചന kills any process that does not block or catch that സൂചന.\n\
It may be necessary to use the KILL സൂചന, since this സൂചന can't be caught.\
\n"), stdout);

      fputs (_("\n\
Exit status:\n\
  124  എങ്കിൽ COMMAND times out, and --preserve-status is not specified\n\
  125  എങ്കിൽ the timeout command itself fails\n\
  126  എങ്കിൽ COMMAND is found but cannot be invoked\n\
  127  എങ്കിൽ COMMAND cannot be found\n\
  137  എങ്കിൽ COMMAND (or timeout itself) is sent the KILL (9) സൂചന (128+9)\n\
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

സ്ഥിതം bool
apply_time_suffix (ഇരട്ട *x, അക്ഷരം suffix_char)
{
  പൂർണ്ണസംഖ്യ multiplier;

  തിരഞ്ഞെടുപ്പ് (suffix_char)
    {
    അവസ്ഥ 0:
    അവസ്ഥ 's':
      multiplier = 1;
      നിർത്തുക;
    അവസ്ഥ 'm':
      multiplier = 60;
      നിർത്തുക;
    അവസ്ഥ 'h':
      multiplier = 60 * 60;
      നിർത്തുക;
    അവസ്ഥ 'd':
      multiplier = 60 * 60 * 24;
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      തിരികെ false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  തിരികെ true;
}

സ്ഥിതം ഇരട്ട
parse_duration (അക്ഷരം സ്ഥിരം *str)
{
  അക്ഷരം *ep;
  errno = 0;
  ഇരട്ട duration = cl_strtod (str, &ep);
  ഇരട്ട s = dtimespec_bound (duration, errno);

  എങ്കിൽ (ep == str
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

  തിരികെ s;
}

സ്ഥിതം ശൂന്യം
unblock_signal (പൂർണ്ണസംഖ്യ sig)
{
  sigset_t unblock_set;
  sigemptyset (&unblock_set);
  sigaddset (&unblock_set, sig);
  എങ്കിൽ (sigprocmask (SIG_UNBLOCK, &unblock_set, ശൂന്യ) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

സ്ഥിതം ശൂന്യം
install_sigchld (ശൂന്യം)
{
  ഘടന sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = chld;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  sigaction (SIGCHLD, &sa, ശൂന്യ);

  /* We inherit the signal mask from our parent process,
     so ensure SIGCHLD is not blocked. */
  unblock_signal (SIGCHLD);
}

/* Filter out signals that were ignored.  */

സ്ഥിതം bool
sig_needs_handling (പൂർണ്ണസംഖ്യ sig, പൂർണ്ണസംഖ്യ sigterm)
{
  എങ്കിൽ (sig == SIGALRM || sig == sigterm)
    തിരികെ true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  ഘടന sigaction old_sa;
  sigaction (sig, ശൂന്യ, &old_sa);
  bool ret = old_sa.sa_handler != SIG_IGN;
  തിരികെ ret;
}

സ്ഥിതം ശൂന്യം
install_cleanup (പൂർണ്ണസംഖ്യ sigterm)
{
  ഘടന sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < countof (term_sig); i++)
    എങ്കിൽ (sig_needs_handling (term_sig[i], sigterm))
      sigaction (term_sig[i], &sa, ശൂന്യ);

  /* Real Time signals also terminate by default.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ s = SIGRTMIN; s <= SIGRTMAX; s++)
    എങ്കിൽ (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, ശൂന്യ);

  sigaction (sigterm, &sa, ശൂന്യ); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
സ്ഥിതം ശൂന്യം
block_cleanup_and_chld (പൂർണ്ണസംഖ്യ sigterm, sigset_t *old_set)
{
  sigset_t block_set;
  sigemptyset (&block_set);

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < countof (term_sig); i++)
    എങ്കിൽ (sig_needs_handling (term_sig[i], sigterm))
      sigaddset (&block_set, term_sig[i]);

  വേണ്ടി (പൂർണ്ണസംഖ്യ s = SIGRTMIN; s <= SIGRTMAX; s++)
    എങ്കിൽ (sig_needs_handling (s, sigterm))
      sigaddset (&block_set, s);

  sigaddset (&block_set, sigterm);

  sigaddset (&block_set, SIGCHLD);

  എങ്കിൽ (sigprocmask (SIG_BLOCK, &block_set, old_set) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
സ്ഥിതം bool
disable_core_dumps (ശൂന്യം)
{
#എങ്കിൽ HAVE_PRCTL && defined PR_SET_DUMPABLE
  എങ്കിൽ (prctl (PR_SET_DUMPABLE, 0) == 0)
    തിരികെ true;

#അല്ലെങ്കിലെങ്കിൽ HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  എങ്കിൽ (setrlimit (RLIMIT_CORE, &(ഘടന rlimit) {0}) == 0)
    തിരികെ true;

#അല്ലെങ്കിൽ
  തിരികെ false;
#endif

  error (0, errno, _("warning: disabling core dumps failed"));
  തിരികെ false;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  ഇരട്ട timeout;
  പൂർണ്ണസംഖ്യ c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'f':
          foreground = true;
          നിർത്തുക;

        അവസ്ഥ 'k':
          kill_after = parse_duration (optarg);
          നിർത്തുക;

        അവസ്ഥ 'p':
          preserve_status = true;
          നിർത്തുക;

        അവസ്ഥ 's':
          term_signal = operand2sig (optarg);
          എങ്കിൽ (term_signal == -1)
            usage (EXIT_CANCELED);
          നിർത്തുക;

        അവസ്ഥ 'v':
          verbose = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_CANCELED);
          നിർത്തുക;
        }
    }

  എങ്കിൽ (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  എങ്കിൽ (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (term_signal);
  സൂചന (SIGTTIN, SIG_IGN);   /* Don't stop if background child needs tty.  */
  സൂചന (SIGTTOU, SIG_IGN);   /* Don't stop if background child needs tty.  */
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
  എങ്കിൽ (monitored_pid == -1)
    {
      error (0, errno, _("fork system call failed"));
      തിരികെ EXIT_CANCELED;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (monitored_pid == 0)  /* child */
    {
#എങ്കിൽ HAVE_PRCTL && defined PR_SET_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_SET_PDEATHSIG, term_signal);
#endif
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      എങ്കിൽ (getppid () != timeout_pid)
        തിരികെ EXIT_CANCELED;

      /* Restore signal mask for child.  */
      എങ്കിൽ (sigprocmask (SIG_SETMASK, &orig_set, ശൂന്യ) != 0)
        {
          error (0, errno, _("child failed to reset signal mask"));
          തിരികെ EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      സൂചന (SIGTTIN, SIG_DFL);
      സൂചന (SIGTTOU, SIG_DFL);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      പൂർണ്ണസംഖ്യ exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, errno, _("failed to run command %s"), quote (command));
      തിരികെ exit_status;
    }
  അല്ലെങ്കിൽ
    {
      pid_t wait_result;
      പൂർണ്ണസംഖ്യ status;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      ആകുമ്പോൾ ((wait_result = waitpid (monitored_pid, &status, WNOHANG)) == 0)
        sigsuspend (&orig_set);  /* Wait with cleanup signals unblocked.  */

      എങ്കിൽ (wait_result < 0)
        {
          /* shouldn't happen.  */
          error (0, errno, _("error waiting for command"));
          status = EXIT_CANCELED;
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (WIFEXITED (status))
            status = WEXITSTATUS (status);
          അല്ലെങ്കിൽ എങ്കിൽ (WIFSIGNALED (status))
            {
              പൂർണ്ണസംഖ്യ sig = WTERMSIG (status);
              എങ്കിൽ (WCOREDUMP (status))
                error (0, 0, _("the monitored command dumped core"));
              എങ്കിൽ (!timed_out && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  സൂചന (sig, SIG_DFL);
                  unblock_signal (sig);
                  ഉയർത്തുക (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              എങ്കിൽ (timed_out && sig == SIGKILL)
                preserve_status = true;
              status = sig + 128; /* what sh returns for signaled processes.  */
            }
          അല്ലെങ്കിൽ
            {
              /* shouldn't happen.  */
              error (0, 0, _("unknown status from command (%d)"), status);
              status = EXIT_FAILURE;
            }
        }

      എങ്കിൽ (timed_out && !preserve_status)
        status = EXIT_TIMEDOUT;
      തിരികെ status;
    }
}
