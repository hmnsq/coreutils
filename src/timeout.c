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
#ಆದರೆ HAVE_PRCTL
# include <sys/prctl.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <sys/wait.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "cl-strtod.h"
#ಅನ್ತರ್ಭಾವ "dtimespec-bound.h"
#ಅನ್ತರ್ಭಾವ "sig2str.h"
#ಅನ್ತರ್ಭಾವ "operand2sig.h"
#ಅನ್ತರ್ಭಾವ "term-sig.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

#ಆದರೆ HAVE_SETRLIMIT
/* FreeBSD 5.0 at least needs <sys/types.h> and <sys/time.h> included
   before <sys/resource.h>.  Currently "system.h" includes <sys/time.h>.  */
# include <sys/resource.h>
#ಆದರೆ_ಅಂತ್ಯ

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SA_RESTART
# define SA_RESTART 0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "timeout"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ timed_ಹೊರಹೋಗು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪದ_signal = SIGTERM;  /* same default as kill command.  */
ಸ್ಥಿರ pid_t monitored_pid;
ಸ್ಥಿರ ದ್ವಿಗುಣ kill_after;
ಸ್ಥಿರ bool foreground;      /* whether to use another program group.  */
ಸ್ಥಿರ bool preserve_ಸ್ಥಿತಿ; /* whether to use a timeout status or not.  */
ಸ್ಥಿರ bool verbose;         /* whether to diagnose timeouts or not.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *command;

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"foreground", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"kill-after", required_ವಾದ, ಖಾಲಿ, 'k'},
  {"preserve-status", no_ವಾದ, ಖಾಲಿ, 'p'},
  {"signal", required_ವಾದ, ಖಾಲಿ, 's'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Start the timeout after which we'll receive a SIGALRM.
   Round DURATION up to the next representable value.
   Treat out-of-range values as if they were maximal,
   as that's more useful in practice than reporting an error.
   '0' means don't timeout.  */
ಸ್ಥಿರ ಶೂನ್ಯ
settimeout (ದ್ವಿಗುಣ duration, bool warn)
{

#ಆದರೆ HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  ರಚನೆ timespec ts = dtotimespec (duration);
  ರಚನೆ itimerspec its = {.it_interval = {0}, .it_ಮೌಲ್ಯ = ts};
  timer_t timerid;
  ಆದರೆ (timer_create (CLOCK_REALTIME, ಖಾಲಿ, &timerid) == 0)
    {
      ಆದರೆ (timer_settime (timerid, 0, &its, ಖಾಲಿ) == 0)
        ಹಿಂತಿರುಗಿ;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (warn)
            error (0, errno, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (warn && errno != ENOSYS)
    error (0, errno, _("warning: timer_create"));

#ಅಥವಾಆದರೆ HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  ರಚನೆ timeval tv;
  ರಚನೆ timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  ಆದರೆ (tv.tv_usec == 1000 * 1000)
    {
      ಆದರೆ (tv.tv_sec != TYPE_ಗರಿಷ್ಠ (time_t))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        tv.tv_usec--;
    }
  ರಚನೆ itimerval it = {.it_interval = {0}, .it_ಮೌಲ್ಯ = tv };
  ಆದರೆ (setitimer (ITIMER_REAL, &it, ಖಾಲಿ) == 0)
    ಹಿಂತಿರುಗಿ;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (warn && errno != ENOSYS)
        error (0, errno, _("warning: setitimer"));
    }
#ಆದರೆ_ಅಂತ್ಯ

  /* fallback to single second resolution provided by alarm().  */

  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ timeint;
  ಆದರೆ (UINT_MAX <= duration)
    timeint = UINT_MAX;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
send_sig (pid_t where, ಪೂರ್ಣಾಂಕ sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  ಆದರೆ (where == 0)
    ಸಂಕೇತ (sig, SIG_IGN);
  ಹಿಂತಿರುಗಿ kill (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
ಸ್ಥಿರ ಶೂನ್ಯ
chld (MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ sig)
{
}

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup (ಪೂರ್ಣಾಂಕ sig)
{
  ಆದರೆ (sig == SIGALRM)
    {
      timed_ಹೊರಹೋಗು = 1;
      /* In case there is an issue with close_stdout,
         update to a more accurate default exit status.
         For example we might get failed writes with -v with:
           timeout -v 1 sleep 10 2>&1 | :
      */
      initialize_exit_failure (EXIT_TIMEDOUT);
      sig = ಪದ_signal;
    }
  ಆದರೆ (0 < monitored_pid)
    {
      ಆದರೆ (kill_after)
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          ಪದ_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          errno = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      ಆದರೆ (verbose)
        {
          ಅಕ್ಷರ signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (ಪೂರ್ಣಾಂಕ))];
          ಆದರೆ (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, ಗಾತ್ರ signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      ಆದರೆ (!foreground)
        {
          send_sig (0, sig);
          ಆದರೆ (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  ಇಲ್ಲದಿದ್ದರೆ /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), ಕಾರ್ಯಕ್ರಮ_name);

      fputs (_("\
ಆರಂಭ_2 COMMAND, and kill it ಆದರೆ still running after DURATION.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --foreground\n\
         when not running timeout directly from a shell prompt,\n\
         allow COMMAND ಗಮ್ಯ ಪಠ from the TTY and get TTY ಸಂಕೇತಗಳು;\n\
         ಒಳ this ಬಹುಲಕ_2, children ದ COMMAND will not be timed ಹೊರಹೋಗು\n\
"));
      oputs (_("\
  -k, --kill-after=DURATION\n\
         also send a KILL ಸಂಕೇತ ಆದರೆ COMMAND is still running\n\
         this ದೀರ್ಘ after the initial ಸಂಕೇತ was ಕಳುಹಿಸಿದ\n\
"));
      oputs (_("\
  -p, --preserve-ಸ್ಥಿತಿ_2\n\
         ನಿರ್ಗಮಿಸು with the same ಸ್ಥಿತಿ_2 as COMMAND,\n\
         even when the command times ಹೊರಹೋಗು\n\
"));
      oputs (_("\
  -s, --ಸಂಕೇತ=SIGNAL\n\
         specify the ಸಂಕೇತ ಗಮ್ಯ be ಕಳುಹಿಸಿದ on timeout;\n\
         SIGNAL may be a name like 'HUP' or a ಸಂಖ್ಯೆ;\n\
         see 'kill -l' ಗಾಗಿ a list ದ ಸಂಕೇತಗಳು\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose ಗಮ್ಯ standard error any ಸಂಕೇತ ಕಳುಹಿಸಿದ upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);

      fputs (_("\n\
DURATION is a floating ಬಿಂದು ಸಂಖ್ಯೆ with an optional suffix:\n\
's' ಗಾಗಿ seconds (the ಪೂರ್ವನಿಯೋಜಿತ), 'm' ಗಾಗಿ minutes, 'h' ಗಾಗಿ hours or \
'd' ಗಾಗಿ days.\nA duration ದ 0 disables the associated timeout.\n"), stdout);

      fputs (_("\n\
Upon timeout, send the ಪದ_2 ಸಂಕೇತ ಗಮ್ಯ COMMAND, ಆದರೆ no other SIGNAL specified.\n\
The ಪದ_2 ಸಂಕೇತ kills any process that does not ಖಂಡ or catch that ಸಂಕೇತ.\n\
It may be necessary ಗಮ್ಯ use the KILL ಸಂಕೇತ, since this ಸಂಕೇತ can't be caught.\
\n"), stdout);

      fputs (_("\n\
Exit ಸ್ಥಿತಿ_2:\n\
  124  ಆದರೆ COMMAND times ಹೊರಹೋಗು, and --preserve-ಸ್ಥಿತಿ_2 is not specified\n\
  125  ಆದರೆ the timeout command itself fails\n\
  126  ಆದರೆ COMMAND is ಸಿಕ್ಕಿತು but cannot be invoked\n\
  127  ಆದರೆ COMMAND cannot be ಸಿಕ್ಕಿತು\n\
  137  ಆದರೆ COMMAND (or timeout itself) is ಕಳುಹಿಸಿದ the KILL (9) ಸಂಕೇತ (128+9)\n\
  -    the ನಿರ್ಗಮಿಸು ಸ್ಥಿತಿ_2 ದ COMMAND otherwise\n\
"), stdout);

      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

ಸ್ಥಿರ bool
apply_ಸಮಯ_suffix (ದ್ವಿಗುಣ *x, ಅಕ್ಷರ suffix_char)
{
  ಪೂರ್ಣಾಂಕ multiplier;

  ಆಯ್ಕೆ (suffix_char)
    {
    ಸ್ಥಿತಿ 0:
    ಸ್ಥಿತಿ 's':
      multiplier = 1;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'm':
      multiplier = 60;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'h':
      multiplier = 60 * 60;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'd':
      multiplier = 60 * 60 * 24;
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ದ್ವಿಗುಣ
parse_duration (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  ಅಕ್ಷರ *ep;
  errno = 0;
  ದ್ವಿಗುಣ duration = cl_strtod (str, &ep);
  ದ್ವಿಗುಣ s = dtimespec_bound (duration, errno);

  ಆದರೆ (ep == str
      /* Nonnegative interval.  */
      || ! (0 <= s)
      /* No extra chars after the number and an optional s,m,h,d char.  */
      || (*ep && *(ep + 1))
      /* Check any suffix char and update timeout based on the suffix.  */
      || !apply_ಸಮಯ_suffix (&s, *ep))
    {
      error (0, 0, _("invalid time interval %s"), quote (str));
      usage (EXIT_CANCELED);
    }

  ಹಿಂತಿರುಗಿ s;
}

ಸ್ಥಿರ ಶೂನ್ಯ
unblock_signal (ಪೂರ್ಣಾಂಕ sig)
{
  sigset_t unblock_ಗಣ;
  sigemptyset (&unblock_ಗಣ);
  sigaddset (&unblock_ಗಣ, sig);
  ಆದರೆ (sigprocmask (SIG_UNBLOCK, &unblock_ಗಣ, ಖಾಲಿ) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

ಸ್ಥಿರ ಶೂನ್ಯ
install_sigchld (ಶೂನ್ಯ)
{
  ರಚನೆ sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = chld;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  sigaction (SIGCHLD, &sa, ಖಾಲಿ);

  /* We inherit the signal mask from our parent process,
     so ensure SIGCHLD is not blocked. */
  unblock_signal (SIGCHLD);
}

/* Filter out signals that were ignored.  */

ಸ್ಥಿರ bool
sig_needs_handling (ಪೂರ್ಣಾಂಕ sig, ಪೂರ್ಣಾಂಕ sigterm)
{
  ಆದರೆ (sig == SIGALRM || sig == sigterm)
    ಹಿಂತಿರುಗಿ true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  ರಚನೆ sigaction old_sa;
  sigaction (sig, ಖಾಲಿ, &old_sa);
  bool ret = old_sa.sa_handler != SIG_IGN;
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
install_cleanup (ಪೂರ್ಣಾಂಕ sigterm)
{
  ರಚನೆ sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < countof (ಪದ_sig); i++)
    ಆದರೆ (sig_needs_handling (ಪದ_sig[i], sigterm))
      sigaction (ಪದ_sig[i], &sa, ಖಾಲಿ);

  /* Real Time signals also terminate by default.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = SIGRTMIN; s <= SIGRTMAX; s++)
    ಆದರೆ (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, ಖಾಲಿ);

  sigaction (sigterm, &sa, ಖಾಲಿ); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಖಂಡ_cleanup_and_chld (ಪೂರ್ಣಾಂಕ sigterm, sigset_t *old_ಗಣ)
{
  sigset_t ಖಂಡ_ಗಣ;
  sigemptyset (&ಖಂಡ_ಗಣ);

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < countof (ಪದ_sig); i++)
    ಆದರೆ (sig_needs_handling (ಪದ_sig[i], sigterm))
      sigaddset (&ಖಂಡ_ಗಣ, ಪದ_sig[i]);

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = SIGRTMIN; s <= SIGRTMAX; s++)
    ಆದರೆ (sig_needs_handling (s, sigterm))
      sigaddset (&ಖಂಡ_ಗಣ, s);

  sigaddset (&ಖಂಡ_ಗಣ, sigterm);

  sigaddset (&ಖಂಡ_ಗಣ, SIGCHLD);

  ಆದರೆ (sigprocmask (SIG_BLOCK, &ಖಂಡ_ಗಣ, old_ಗಣ) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
ಸ್ಥಿರ bool
disable_core_dumps (ಶೂನ್ಯ)
{
#ಆದರೆ HAVE_PRCTL && defined PR_ಗಣ_DUMPABLE
  ಆದರೆ (prctl (PR_ಗಣ_DUMPABLE, 0) == 0)
    ಹಿಂತಿರುಗಿ true;

#ಅಥವಾಆದರೆ HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  ಆದರೆ (setrlimit (RLIMIT_CORE, &(ರಚನೆ rlimit) {0}) == 0)
    ಹಿಂತಿರುಗಿ true;

#ಇಲ್ಲದಿದ್ದರೆ
  ಹಿಂತಿರುಗಿ false;
#ಆದರೆ_ಅಂತ್ಯ

  error (0, errno, _("warning: disabling core dumps failed"));
  ಹಿಂತಿರುಗಿ false;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ದ್ವಿಗುಣ timeout;
  ಪೂರ್ಣಾಂಕ c;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ಆಗ ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'f':
          foreground = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'k':
          kill_after = parse_duration (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'p':
          preserve_ಸ್ಥಿತಿ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          ಪದ_signal = operand2sig (optarg);
          ಆದರೆ (ಪದ_signal == -1)
            usage (EXIT_CANCELED);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'v':
          verbose = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_CANCELED);
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  ಆದರೆ (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (ಪದ_signal);
  ಸಂಕೇತ (SIGTTIN, SIG_IGN);   /* Don't stop if background child needs tty.  */
  ಸಂಕೇತ (SIGTTOU, SIG_IGN);   /* Don't stop if background child needs tty.  */
  install_sigchld ();          /* Interrupt sigsuspend() when child exits.   */

  /* We configure timers so that SIGALRM is sent on expiry.
     Therefore ensure we don't inherit a mask blocking SIGALRM.  */
  unblock_signal (SIGALRM);

  /* Block signals now, so monitored_pid is deterministic in cleanup().  */
  sigset_t orig_ಗಣ;
  ಖಂಡ_cleanup_and_chld (ಪದ_signal, &orig_ಗಣ);

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
  ಆದರೆ (monitored_pid == -1)
    {
      error (0, errno, _("fork system call failed"));
      ಹಿಂತಿರುಗಿ EXIT_CANCELED;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (monitored_pid == 0)  /*ಮಗು*/
    {
#ಆದರೆ HAVE_PRCTL && defined PR_ಗಣ_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_ಗಣ_PDEATHSIG, ಪದ_signal);
#ಆದರೆ_ಅಂತ್ಯ
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      ಆದರೆ (getppid () != timeout_pid)
        ಹಿಂತಿರುಗಿ EXIT_CANCELED;

      /* Restore signal mask for child.  */
      ಆದರೆ (sigprocmask (SIG_SETMASK, &orig_ಗಣ, ಖಾಲಿ) != 0)
        {
          error (0, errno, _("child failed to reset signal mask"));
          ಹಿಂತಿರುಗಿ EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      ಸಂಕೇತ (SIGTTIN, SIG_DFL);
      ಸಂಕೇತ (SIGTTOU, SIG_DFL);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, errno, _("failed to run command %s"), quote (command));
      ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      pid_t wait_ಫಲಿತಾಂಶ;
      ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      ಆಗ ((wait_ಫಲಿತಾಂಶ = waitpid (monitored_pid, &ಸ್ಥಿತಿ_2, WNOHANG)) == 0)
        sigsuspend (&orig_ಗಣ);  /* Wait with cleanup signals unblocked.  */

      ಆದರೆ (wait_ಫಲಿತಾಂಶ < 0)
        {
          /* shouldn't happen.  */
          error (0, errno, _("error waiting for command"));
          ಸ್ಥಿತಿ_2 = EXIT_CANCELED;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (WIFEXITED (ಸ್ಥಿತಿ_2))
            ಸ್ಥಿತಿ_2 = WEXITSTATUS (ಸ್ಥಿತಿ_2);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (WIFSIGNALED (ಸ್ಥಿತಿ_2))
            {
              ಪೂರ್ಣಾಂಕ sig = WTERMSIG (ಸ್ಥಿತಿ_2);
              ಆದರೆ (WCOREDUMP (ಸ್ಥಿತಿ_2))
                error (0, 0, _("the monitored command dumped core"));
              ಆದರೆ (!timed_ಹೊರಹೋಗು && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  ಸಂಕೇತ (sig, SIG_DFL);
                  unblock_signal (sig);
                  ಎತ್ತು (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              ಆದರೆ (timed_ಹೊರಹೋಗು && sig == SIGKILL)
                preserve_ಸ್ಥಿತಿ = true;
              ಸ್ಥಿತಿ_2 = sig + 128; /* what sh returns for signaled processes.  */
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* shouldn't happen.  */
              error (0, 0, _("unknown status from command (%d)"), ಸ್ಥಿತಿ_2);
              ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
            }
        }

      ಆದರೆ (timed_ಹೊರಹೋಗು && !preserve_ಸ್ಥಿತಿ)
        ಸ್ಥಿತಿ_2 = EXIT_TIMEDOUT;
      ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
    }
}
