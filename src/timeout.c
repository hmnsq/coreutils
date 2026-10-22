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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಯದಿ HAVE_PRCTL
# include <sys/prctl.h>
#endif
#ಅನ್ತರ್ಭಾವ <sys/wait.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "cl-strtod.h"
#ಅನ್ತರ್ಭಾವ "dtimespec-bound.h"
#ಅನ್ತರ್ಭಾವ "sig2str.h"
#ಅನ್ತರ್ಭಾವ "operand2sig.h"
#ಅನ್ತರ್ಭಾವ "term-sig.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

#ಯದಿ HAVE_SETRLIMIT
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ timed_out;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ term_signal = SIGTERM;  /* same default as kill command.  */
ಸ್ಥಿರ pid_t monitored_pid;
ಸ್ಥಿರ ದ್ವಿಗುಣ kill_after;
ಸ್ಥಿರ bool foreground;      /* whether to use another program group.  */
ಸ್ಥಿರ bool preserve_status; /* whether to use a timeout status or not.  */
ಸ್ಥಿರ bool verbose;         /* whether to diagnose timeouts or not.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *command;

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"foreground", no_argument, ರಿಕ್ತ, 'f'},
  {"kill-after", required_argument, ರಿಕ್ತ, 'k'},
  {"preserve-status", no_argument, ರಿಕ್ತ, 'p'},
  {"signal", required_argument, ರಿಕ್ತ, 's'},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Start the timeout after which we'll receive a SIGALRM.
   Round DURATION up to the next representable value.
   Treat out-of-range values as if they were maximal,
   as that's more useful in practice than reporting an error.
   '0' means don't timeout.  */
ಸ್ಥಿರ ಶೂನ್ಯ
settimeout (ದ್ವಿಗುಣ duration, bool warn)
{

#ಯದಿ HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  ಸಂರಚನಾ timespec ts = dtotimespec (duration);
  ಸಂರಚನಾ itimerspec its = {.it_interval = {0}, .it_ಮೌಲ್ಯ = ts};
  timer_t timerid;
  ಯದಿ (timer_create (CLOCK_REALTIME, ರಿಕ್ತ, &timerid) == 0)
    {
      ಯದಿ (timer_settime (timerid, 0, &its, ರಿಕ್ತ) == 0)
        ಪ್ರತ್ಯಾಗಮ;
      ಅನ್ಯಥಾ
        {
          ಯದಿ (warn)
            error (0, errno, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (warn && errno != ENOSYS)
    error (0, errno, _("warning: timer_create"));

#ನೋಚೇದ್ಯದಿ HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  ಸಂರಚನಾ timeval tv;
  ಸಂರಚನಾ timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  ಯದಿ (tv.tv_usec == 1000 * 1000)
    {
      ಯದಿ (tv.tv_sec != TYPE_MAXIMUM (time_t))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      ಅನ್ಯಥಾ
        tv.tv_usec--;
    }
  ಸಂರಚನಾ itimerval it = {.it_interval = {0}, .it_ಮೌಲ್ಯ = tv };
  ಯದಿ (setitimer (ITIMER_REAL, &it, ರಿಕ್ತ) == 0)
    ಪ್ರತ್ಯಾಗಮ;
  ಅನ್ಯಥಾ
    {
      ಯದಿ (warn && errno != ENOSYS)
        error (0, errno, _("warning: setitimer"));
    }
#endif

  /* fallback to single second resolution provided by alarm().  */

  unsigned ಪೂರ್ಣಾಙ್ಕ timeint;
  ಯದಿ (UINT_MAX <= duration)
    timeint = UINT_MAX;
  ಅನ್ಯಥಾ
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
send_sig (pid_t where, ಪೂರ್ಣಾಙ್ಕ sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  ಯದಿ (where == 0)
    ಸಙ್ಕೇತ (sig, SIG_IGN);
  ಪ್ರತ್ಯಾಗಮ kill (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
ಸ್ಥಿರ ಶೂನ್ಯ
chld (MAYBE_UNUSED ಪೂರ್ಣಾಙ್ಕ sig)
{
}

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup (ಪೂರ್ಣಾಙ್ಕ sig)
{
  ಯದಿ (sig == SIGALRM)
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
  ಯದಿ (0 < monitored_pid)
    {
      ಯದಿ (kill_after)
        {
          ಪೂರ್ಣಾಙ್ಕ saved_errno = errno; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          term_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          errno = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      ಯದಿ (verbose)
        {
          ವರ್ಣ signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (ಪೂರ್ಣಾಙ್ಕ))];
          ಯದಿ (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, ಪರಿಮಾಣ signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      ಯದಿ (!foreground)
        {
          send_sig (0, sig);
          ಯದಿ (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  ಅನ್ಯಥಾ /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), program_name);

      fputs (_("\
Start COMMAND, and kill it ಯದಿ still running after DURATION.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --foreground\n\
         when not running timeout directly from a shell prompt,\n\
         allow COMMAND to ಪಠ from the TTY and get TTY signals;\n\
         in this mode, children of COMMAND will not be timed out\n\
"));
      oputs (_("\
  -k, --kill-after=DURATION\n\
         also send a KILL ಸಙ್ಕೇತ ಯದಿ COMMAND is still running\n\
         this long after the initial ಸಙ್ಕೇತ was sent\n\
"));
      oputs (_("\
  -p, --preserve-status\n\
         exit with the same status as COMMAND,\n\
         even when the command times out\n\
"));
      oputs (_("\
  -s, --ಸಙ್ಕೇತ=SIGNAL\n\
         specify the ಸಙ್ಕೇತ to be sent on timeout;\n\
         SIGNAL may be a name like 'HUP' or a ಸಂಖ್ಯೆ;\n\
         see 'kill -l' ಪ್ರತಿ a list of signals\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose to standard error any ಸಙ್ಕೇತ sent upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
DURATION is a floating point ಸಂಖ್ಯೆ with an optional suffix:\n\
's' ಪ್ರತಿ seconds (the ಸ್ವಭಾವ), 'm' ಪ್ರತಿ minutes, 'h' ಪ್ರತಿ hours or \
'd' ಪ್ರತಿ days.\nA duration of 0 disables the associated timeout.\n"), stdout);

      fputs (_("\n\
Upon timeout, send the TERM ಸಙ್ಕೇತ to COMMAND, ಯದಿ no other SIGNAL specified.\n\
The TERM ಸಙ್ಕೇತ kills any process that does not block or catch that ಸಙ್ಕೇತ.\n\
It may be necessary to use the KILL ಸಙ್ಕೇತ, since this ಸಙ್ಕೇತ can't be caught.\
\n"), stdout);

      fputs (_("\n\
Exit status:\n\
  124  ಯದಿ COMMAND times out, and --preserve-status is not specified\n\
  125  ಯದಿ the timeout command itself fails\n\
  126  ಯದಿ COMMAND is found but cannot be invoked\n\
  127  ಯದಿ COMMAND cannot be found\n\
  137  ಯದಿ COMMAND (or timeout itself) is sent the KILL (9) ಸಙ್ಕೇತ (128+9)\n\
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

ಸ್ಥಿರ bool
apply_time_suffix (ದ್ವಿಗುಣ *x, ವರ್ಣ suffix_char)
{
  ಪೂರ್ಣಾಙ್ಕ multiplier;

  ವಿಕಲ್ಪ (suffix_char)
    {
    ಪಕ್ಷ 0:
    ಪಕ್ಷ 's':
      multiplier = 1;
      ಭಙ್ಗ;
    ಪಕ್ಷ 'm':
      multiplier = 60;
      ಭಙ್ಗ;
    ಪಕ್ಷ 'h':
      multiplier = 60 * 60;
      ಭಙ್ಗ;
    ಪಕ್ಷ 'd':
      multiplier = 60 * 60 * 24;
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ದ್ವಿಗುಣ
parse_duration (ವರ್ಣ ಧ್ರುವ *str)
{
  ವರ್ಣ *ep;
  errno = 0;
  ದ್ವಿಗುಣ duration = cl_strtod (str, &ep);
  ದ್ವಿಗುಣ s = dtimespec_bound (duration, errno);

  ಯದಿ (ep == str
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

  ಪ್ರತ್ಯಾಗಮ s;
}

ಸ್ಥಿರ ಶೂನ್ಯ
unblock_signal (ಪೂರ್ಣಾಙ್ಕ sig)
{
  sigset_t unblock_set;
  sigemptyset (&unblock_set);
  sigaddset (&unblock_set, sig);
  ಯದಿ (sigprocmask (SIG_UNBLOCK, &unblock_set, ರಿಕ್ತ) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

ಸ್ಥಿರ ಶೂನ್ಯ
install_sigchld (ಶೂನ್ಯ)
{
  ಸಂರಚನಾ sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = chld;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  sigaction (SIGCHLD, &sa, ರಿಕ್ತ);

  /* We inherit the signal mask from our parent process,
     so ensure SIGCHLD is not blocked. */
  unblock_signal (SIGCHLD);
}

/* Filter out signals that were ignored.  */

ಸ್ಥಿರ bool
sig_needs_handling (ಪೂರ್ಣಾಙ್ಕ sig, ಪೂರ್ಣಾಙ್ಕ sigterm)
{
  ಯದಿ (sig == SIGALRM || sig == sigterm)
    ಪ್ರತ್ಯಾಗಮ true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  ಸಂರಚನಾ sigaction old_sa;
  sigaction (sig, ರಿಕ್ತ, &old_sa);
  bool ret = old_sa.sa_handler != SIG_IGN;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
install_cleanup (ಪೂರ್ಣಾಙ್ಕ sigterm)
{
  ಸಂರಚನಾ sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < countof (term_sig); i++)
    ಯದಿ (sig_needs_handling (term_sig[i], sigterm))
      sigaction (term_sig[i], &sa, ರಿಕ್ತ);

  /* Real Time signals also terminate by default.  */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = SIGRTMIN; s <= SIGRTMAX; s++)
    ಯದಿ (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, ರಿಕ್ತ);

  sigaction (sigterm, &sa, ರಿಕ್ತ); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
ಸ್ಥಿರ ಶೂನ್ಯ
block_cleanup_and_chld (ಪೂರ್ಣಾಙ್ಕ sigterm, sigset_t *old_set)
{
  sigset_t block_set;
  sigemptyset (&block_set);

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < countof (term_sig); i++)
    ಯದಿ (sig_needs_handling (term_sig[i], sigterm))
      sigaddset (&block_set, term_sig[i]);

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = SIGRTMIN; s <= SIGRTMAX; s++)
    ಯದಿ (sig_needs_handling (s, sigterm))
      sigaddset (&block_set, s);

  sigaddset (&block_set, sigterm);

  sigaddset (&block_set, SIGCHLD);

  ಯದಿ (sigprocmask (SIG_BLOCK, &block_set, old_set) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
ಸ್ಥಿರ bool
disable_core_dumps (ಶೂನ್ಯ)
{
#ಯದಿ HAVE_PRCTL && defined PR_SET_DUMPABLE
  ಯದಿ (prctl (PR_SET_DUMPABLE, 0) == 0)
    ಪ್ರತ್ಯಾಗಮ true;

#ನೋಚೇದ್ಯದಿ HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  ಯದಿ (setrlimit (RLIMIT_CORE, &(ಸಂರಚನಾ rlimit) {0}) == 0)
    ಪ್ರತ್ಯಾಗಮ true;

#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ false;
#endif

  error (0, errno, _("warning: disabling core dumps failed"));
  ಪ್ರತ್ಯಾಗಮ false;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ದ್ವಿಗುಣ timeout;
  ಪೂರ್ಣಾಙ್ಕ c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ಯಾವತ್ ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'f':
          foreground = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'k':
          kill_after = parse_duration (optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'p':
          preserve_status = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 's':
          term_signal = operand2sig (optarg);
          ಯದಿ (term_signal == -1)
            usage (EXIT_CANCELED);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'v':
          verbose = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_CANCELED);
          ಭಙ್ಗ;
        }
    }

  ಯದಿ (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  ಯದಿ (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (term_signal);
  ಸಙ್ಕೇತ (SIGTTIN, SIG_IGN);   /* Don't stop if background child needs tty.  */
  ಸಙ್ಕೇತ (SIGTTOU, SIG_IGN);   /* Don't stop if background child needs tty.  */
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
  ಯದಿ (monitored_pid == -1)
    {
      error (0, errno, _("fork system call failed"));
      ಪ್ರತ್ಯಾಗಮ EXIT_CANCELED;
    }
  ಅನ್ಯಥಾ ಯದಿ (monitored_pid == 0)  /* child */
    {
#ಯದಿ HAVE_PRCTL && defined PR_SET_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_SET_PDEATHSIG, term_signal);
#endif
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      ಯದಿ (getppid () != timeout_pid)
        ಪ್ರತ್ಯಾಗಮ EXIT_CANCELED;

      /* Restore signal mask for child.  */
      ಯದಿ (sigprocmask (SIG_SETMASK, &orig_set, ರಿಕ್ತ) != 0)
        {
          error (0, errno, _("child failed to reset signal mask"));
          ಪ್ರತ್ಯಾಗಮ EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      ಸಙ್ಕೇತ (SIGTTIN, SIG_DFL);
      ಸಙ್ಕೇತ (SIGTTOU, SIG_DFL);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      ಪೂರ್ಣಾಙ್ಕ exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, errno, _("failed to run command %s"), quote (command));
      ಪ್ರತ್ಯಾಗಮ exit_status;
    }
  ಅನ್ಯಥಾ
    {
      pid_t wait_result;
      ಪೂರ್ಣಾಙ್ಕ status;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      ಯಾವತ್ ((wait_result = waitpid (monitored_pid, &status, WNOHANG)) == 0)
        sigsuspend (&orig_set);  /* Wait with cleanup signals unblocked.  */

      ಯದಿ (wait_result < 0)
        {
          /* shouldn't happen.  */
          error (0, errno, _("error waiting for command"));
          status = EXIT_CANCELED;
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (WIFEXITED (status))
            status = WEXITSTATUS (status);
          ಅನ್ಯಥಾ ಯದಿ (WIFSIGNALED (status))
            {
              ಪೂರ್ಣಾಙ್ಕ sig = WTERMSIG (status);
              ಯದಿ (WCOREDUMP (status))
                error (0, 0, _("the monitored command dumped core"));
              ಯದಿ (!timed_out && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  ಸಙ್ಕೇತ (sig, SIG_DFL);
                  unblock_signal (sig);
                  ಉತ್ಥಾಪಯ (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              ಯದಿ (timed_out && sig == SIGKILL)
                preserve_status = true;
              status = sig + 128; /* what sh returns for signaled processes.  */
            }
          ಅನ್ಯಥಾ
            {
              /* shouldn't happen.  */
              error (0, 0, _("unknown status from command (%d)"), status);
              status = EXIT_FAILURE;
            }
        }

      ಯದಿ (timed_out && !preserve_status)
        status = EXIT_TIMEDOUT;
      ಪ್ರತ್ಯಾಗಮ status;
    }
}
