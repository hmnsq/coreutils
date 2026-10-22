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

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>
#ఒకవేళ HAVE_PRCTL
# include <sys/prctl.h>
#అయితే_ముగింపు
#అన్తర్భావ <sys/wait.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "cl-strtod.h"
#అన్తర్భావ "dtimespec-bound.h"
#అన్తర్భావ "sig2str.h"
#అన్తర్భావ "operand2sig.h"
#అన్తర్భావ "term-sig.h"
#అన్తర్భావ "quote.h"

#ఒకవేళ HAVE_SETRLIMIT
/* FreeBSD 5.0 at least needs <sys/types.h> and <sys/time.h> included
   before <sys/resource.h>.  Currently "system.h" includes <sys/time.h>.  */
# include <sys/resource.h>
#అయితే_ముగింపు

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt.  */
#నిర్వచితం_కాకపోతే SA_RESTART
# define SA_RESTART 0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#అయితే_ముగింపు

#నిర్వచించు కార్యక్రమం_NAME "timeout"

#నిర్వచించు AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

స్థిర పూర్ణాంకం timed_బహిర్గతం;
స్థిర పూర్ణాంకం పదం_signal = SIGTERM;  /* same default as kill command.  */
స్థిర pid_t monitored_pid;
స్థిర ద్విగుణ kill_after;
స్థిర bool foreground;      /* whether to use another program group.  */
స్థిర bool preserve_స్థితి; /* whether to use a timeout status or not.  */
స్థిర bool verbose;         /* whether to diagnose timeouts or not.  */
స్థిర అక్షరం స్థిరరాశి *command;

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"foreground", no_వాదం, ఖాళీ, 'f'},
  {"kill-after", required_వాదం, ఖాళీ, 'k'},
  {"preserve-status", no_వాదం, ఖాళీ, 'p'},
  {"signal", required_వాదం, ఖాళీ, 's'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Start the timeout after which we'll receive a SIGALRM.
   Round DURATION up to the next representable value.
   Treat out-of-range values as if they were maximal,
   as that's more useful in practice than reporting an error.
   '0' means don't timeout.  */
స్థిర శూన్య
settimeout (ద్విగుణ duration, bool warn)
{

#ఒకవేళ HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  నిర్మాణం timespec ts = dtotimespec (duration);
  నిర్మాణం itimerspec its = {.it_interval = {0}, .it_విలువ = ts};
  timer_t timerid;
  ఒకవేళ (timer_create (CLOCK_REALTIME, ఖాళీ, &timerid) == 0)
    {
      ఒకవేళ (timer_settime (timerid, 0, &its, ఖాళీ) == 0)
        తిరిగి;
      లేకపోతే
        {
          ఒకవేళ (warn)
            error (0, errno, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  లేకపోతే ఒకవేళ (warn && errno != ENOSYS)
    error (0, errno, _("warning: timer_create"));

#లేదావేళ HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  నిర్మాణం timeval tv;
  నిర్మాణం timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  ఒకవేళ (tv.tv_usec == 1000 * 1000)
    {
      ఒకవేళ (tv.tv_sec != TYPE_గరిష్ఠ (time_t))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      లేకపోతే
        tv.tv_usec--;
    }
  నిర్మాణం itimerval it = {.it_interval = {0}, .it_విలువ = tv };
  ఒకవేళ (setitimer (ITIMER_REAL, &it, ఖాళీ) == 0)
    తిరిగి;
  లేకపోతే
    {
      ఒకవేళ (warn && errno != ENOSYS)
        error (0, errno, _("warning: setitimer"));
    }
#అయితే_ముగింపు

  /* fallback to single second resolution provided by alarm().  */

  గుర్తులేని పూర్ణాంకం timeint;
  ఒకవేళ (UINT_MAX <= duration)
    timeint = UINT_MAX;
  లేకపోతే
    {
      గుర్తులేని పూర్ణాంకం duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

స్థిర పూర్ణాంకం
send_sig (pid_t where, పూర్ణాంకం sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  ఒకవేళ (where == 0)
    సంకేతం (sig, SIG_IGN);
  తిరిగి kill (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
స్థిర శూన్య
chld (MAYBE_వాడని పూర్ణాంకం sig)
{
}

స్థిర శూన్య
cleanup (పూర్ణాంకం sig)
{
  ఒకవేళ (sig == SIGALRM)
    {
      timed_బహిర్గతం = 1;
      /* In case there is an issue with close_stdout,
         update to a more accurate default exit status.
         For example we might get failed writes with -v with:
           timeout -v 1 sleep 10 2>&1 | :
      */
      initialize_exit_failure (EXIT_TIMEDOUT);
      sig = పదం_signal;
    }
  ఒకవేళ (0 < monitored_pid)
    {
      ఒకవేళ (kill_after)
        {
          పూర్ణాంకం saved_errno = errno; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          పదం_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          errno = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      ఒకవేళ (verbose)
        {
          అక్షరం signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (పూర్ణాంకం))];
          ఒకవేళ (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, పరిమాణం signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      ఒకవేళ (!foreground)
        {
          send_sig (0, sig);
          ఒకవేళ (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  లేకపోతే ఒకవేళ (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  లేకపోతే /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), కార్యక్రమం_name);

      fputs (_("\
ప్రారంభం_2 COMMAND, and kill it ఒకవేళ still running after DURATION.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --foreground\n\
         when not running timeout directly from a shell prompt,\n\
         allow COMMAND గమ్యం పఠ from the TTY and get TTY సంకేతాలు;\n\
         లోపల this బహుళకం_2, children యొక్క COMMAND will not be timed బహిర్గతం\n\
"));
      oputs (_("\
  -k, --kill-after=DURATION\n\
         also send a KILL సంకేతం ఒకవేళ COMMAND is still running\n\
         this దీర్ఘ after the initial సంకేతం was పంపిన\n\
"));
      oputs (_("\
  -p, --preserve-స్థితి_2\n\
         నిష్క్రమించు with the same స్థితి_2 as COMMAND,\n\
         even when the command times బహిర్గతం\n\
"));
      oputs (_("\
  -s, --సంకేతం=SIGNAL\n\
         specify the సంకేతం గమ్యం be పంపిన on timeout;\n\
         SIGNAL may be a name like 'HUP' or a సంఖ్య;\n\
         see 'kill -l' కొరకు a list యొక్క సంకేతాలు\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose గమ్యం standard error any సంకేతం పంపిన upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);

      fputs (_("\n\
DURATION is a floating బిందువు సంఖ్య with an optional suffix:\n\
's' కొరకు seconds (the అప్రమేయ), 'm' కొరకు minutes, 'h' కొరకు hours or \
'd' కొరకు days.\nA duration యొక్క 0 disables the associated timeout.\n"), stdout);

      fputs (_("\n\
Upon timeout, send the పదం_2 సంకేతం గమ్యం COMMAND, ఒకవేళ no other SIGNAL specified.\n\
The పదం_2 సంకేతం kills any process that does not ఖండం or catch that సంకేతం.\n\
It may be necessary గమ్యం use the KILL సంకేతం, since this సంకేతం can't be caught.\
\n"), stdout);

      fputs (_("\n\
Exit స్థితి_2:\n\
  124  ఒకవేళ COMMAND times బహిర్గతం, and --preserve-స్థితి_2 is not specified\n\
  125  ఒకవేళ the timeout command itself fails\n\
  126  ఒకవేళ COMMAND is దొరికింది but cannot be invoked\n\
  127  ఒకవేళ COMMAND cannot be దొరికింది\n\
  137  ఒకవేళ COMMAND (or timeout itself) is పంపిన the KILL (9) సంకేతం (128+9)\n\
  -    the నిష్క్రమించు స్థితి_2 యొక్క COMMAND otherwise\n\
"), stdout);

      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

స్థిర bool
apply_సమయం_suffix (ద్విగుణ *x, అక్షరం suffix_char)
{
  పూర్ణాంకం multiplier;

  ఎంపిక (suffix_char)
    {
    స్థితి 0:
    స్థితి 's':
      multiplier = 1;
      ఆపు;
    స్థితి 'm':
      multiplier = 60;
      ఆపు;
    స్థితి 'h':
      multiplier = 60 * 60;
      ఆపు;
    స్థితి 'd':
      multiplier = 60 * 60 * 24;
      ఆపు;
    అప్రమేయ:
      తిరిగి false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  తిరిగి true;
}

స్థిర ద్విగుణ
parse_duration (అక్షరం స్థిరరాశి *str)
{
  అక్షరం *ep;
  errno = 0;
  ద్విగుణ duration = cl_strtod (str, &ep);
  ద్విగుణ s = dtimespec_bound (duration, errno);

  ఒకవేళ (ep == str
      /* Nonnegative interval.  */
      || ! (0 <= s)
      /* No extra chars after the number and an optional s,m,h,d char.  */
      || (*ep && *(ep + 1))
      /* Check any suffix char and update timeout based on the suffix.  */
      || !apply_సమయం_suffix (&s, *ep))
    {
      error (0, 0, _("invalid time interval %s"), quote (str));
      usage (EXIT_CANCELED);
    }

  తిరిగి s;
}

స్థిర శూన్య
unblock_signal (పూర్ణాంకం sig)
{
  sigset_t unblock_సమితి;
  sigemptyset (&unblock_సమితి);
  sigaddset (&unblock_సమితి, sig);
  ఒకవేళ (sigprocmask (SIG_UNBLOCK, &unblock_సమితి, ఖాళీ) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

స్థిర శూన్య
install_sigchld (శూన్య)
{
  నిర్మాణం sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = chld;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  sigaction (SIGCHLD, &sa, ఖాళీ);

  /* We inherit the signal mask from our parent process,
     so ensure SIGCHLD is not blocked. */
  unblock_signal (SIGCHLD);
}

/* Filter out signals that were ignored.  */

స్థిర bool
sig_needs_handling (పూర్ణాంకం sig, పూర్ణాంకం sigterm)
{
  ఒకవేళ (sig == SIGALRM || sig == sigterm)
    తిరిగి true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  నిర్మాణం sigaction old_sa;
  sigaction (sig, ఖాళీ, &old_sa);
  bool ret = old_sa.sa_handler != SIG_IGN;
  తిరిగి ret;
}

స్థిర శూన్య
install_cleanup (పూర్ణాంకం sigterm)
{
  నిర్మాణం sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  కొరకు (పూర్ణాంకం i = 0; i < countof (పదం_sig); i++)
    ఒకవేళ (sig_needs_handling (పదం_sig[i], sigterm))
      sigaction (పదం_sig[i], &sa, ఖాళీ);

  /* Real Time signals also terminate by default.  */
  కొరకు (పూర్ణాంకం s = SIGRTMIN; s <= SIGRTMAX; s++)
    ఒకవేళ (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, ఖాళీ);

  sigaction (sigterm, &sa, ఖాళీ); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
స్థిర శూన్య
ఖండం_cleanup_and_chld (పూర్ణాంకం sigterm, sigset_t *old_సమితి)
{
  sigset_t ఖండం_సమితి;
  sigemptyset (&ఖండం_సమితి);

  కొరకు (పూర్ణాంకం i = 0; i < countof (పదం_sig); i++)
    ఒకవేళ (sig_needs_handling (పదం_sig[i], sigterm))
      sigaddset (&ఖండం_సమితి, పదం_sig[i]);

  కొరకు (పూర్ణాంకం s = SIGRTMIN; s <= SIGRTMAX; s++)
    ఒకవేళ (sig_needs_handling (s, sigterm))
      sigaddset (&ఖండం_సమితి, s);

  sigaddset (&ఖండం_సమితి, sigterm);

  sigaddset (&ఖండం_సమితి, SIGCHLD);

  ఒకవేళ (sigprocmask (SIG_BLOCK, &ఖండం_సమితి, old_సమితి) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
స్థిర bool
disable_core_dumps (శూన్య)
{
#ఒకవేళ HAVE_PRCTL && defined PR_సమితి_DUMPABLE
  ఒకవేళ (prctl (PR_సమితి_DUMPABLE, 0) == 0)
    తిరిగి true;

#లేదావేళ HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  ఒకవేళ (setrlimit (RLIMIT_CORE, &(నిర్మాణం rlimit) {0}) == 0)
    తిరిగి true;

#లేకపోతే
  తిరిగి false;
#అయితే_ముగింపు

  error (0, errno, _("warning: disabling core dumps failed"));
  తిరిగి false;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  ద్విగుణ timeout;
  పూర్ణాంకం c;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  అయితే ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, ఖాళీ))
         != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'f':
          foreground = true;
          ఆపు;

        స్థితి 'k':
          kill_after = parse_duration (optarg);
          ఆపు;

        స్థితి 'p':
          preserve_స్థితి = true;
          ఆపు;

        స్థితి 's':
          పదం_signal = operand2sig (optarg);
          ఒకవేళ (పదం_signal == -1)
            usage (EXIT_CANCELED);
          ఆపు;

        స్థితి 'v':
          verbose = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_CANCELED);
          ఆపు;
        }
    }

  ఒకవేళ (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  ఒకవేళ (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (పదం_signal);
  సంకేతం (SIGTTIN, SIG_IGN);   /* Don't stop if background child needs tty.  */
  సంకేతం (SIGTTOU, SIG_IGN);   /* Don't stop if background child needs tty.  */
  install_sigchld ();          /* Interrupt sigsuspend() when child exits.   */

  /* We configure timers so that SIGALRM is sent on expiry.
     Therefore ensure we don't inherit a mask blocking SIGALRM.  */
  unblock_signal (SIGALRM);

  /* Block signals now, so monitored_pid is deterministic in cleanup().  */
  sigset_t orig_సమితి;
  ఖండం_cleanup_and_chld (పదం_signal, &orig_సమితి);

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
  ఒకవేళ (monitored_pid == -1)
    {
      error (0, errno, _("fork system call failed"));
      తిరిగి EXIT_CANCELED;
    }
  లేకపోతే ఒకవేళ (monitored_pid == 0)  /*బిడ్డ*/
    {
#ఒకవేళ HAVE_PRCTL && defined PR_సమితి_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_సమితి_PDEATHSIG, పదం_signal);
#అయితే_ముగింపు
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      ఒకవేళ (getppid () != timeout_pid)
        తిరిగి EXIT_CANCELED;

      /* Restore signal mask for child.  */
      ఒకవేళ (sigprocmask (SIG_SETMASK, &orig_సమితి, ఖాళీ) != 0)
        {
          error (0, errno, _("child failed to reset signal mask"));
          తిరిగి EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      సంకేతం (SIGTTIN, SIG_DFL);
      సంకేతం (SIGTTOU, SIG_DFL);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      పూర్ణాంకం exit_స్థితి = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, errno, _("failed to run command %s"), quote (command));
      తిరిగి exit_స్థితి;
    }
  లేకపోతే
    {
      pid_t wait_ఫలితం;
      పూర్ణాంకం స్థితి_2;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      అయితే ((wait_ఫలితం = waitpid (monitored_pid, &స్థితి_2, WNOHANG)) == 0)
        sigsuspend (&orig_సమితి);  /* Wait with cleanup signals unblocked.  */

      ఒకవేళ (wait_ఫలితం < 0)
        {
          /* shouldn't happen.  */
          error (0, errno, _("error waiting for command"));
          స్థితి_2 = EXIT_CANCELED;
        }
      లేకపోతే
        {
          ఒకవేళ (WIFEXITED (స్థితి_2))
            స్థితి_2 = WEXITSTATUS (స్థితి_2);
          లేకపోతే ఒకవేళ (WIFSIGNALED (స్థితి_2))
            {
              పూర్ణాంకం sig = WTERMSIG (స్థితి_2);
              ఒకవేళ (WCOREDUMP (స్థితి_2))
                error (0, 0, _("the monitored command dumped core"));
              ఒకవేళ (!timed_బహిర్గతం && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  సంకేతం (sig, SIG_DFL);
                  unblock_signal (sig);
                  లేపు (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              ఒకవేళ (timed_బహిర్గతం && sig == SIGKILL)
                preserve_స్థితి = true;
              స్థితి_2 = sig + 128; /* what sh returns for signaled processes.  */
            }
          లేకపోతే
            {
              /* shouldn't happen.  */
              error (0, 0, _("unknown status from command (%d)"), స్థితి_2);
              స్థితి_2 = EXIT_FAILURE;
            }
        }

      ఒకవేళ (timed_బహిర్గతం && !preserve_స్థితి)
        స్థితి_2 = EXIT_TIMEDOUT;
      తిరిగి స్థితి_2;
    }
}
