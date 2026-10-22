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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਜੇ HAVE_PRCTL
# include <sys/prctl.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/wait.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cl-strtod.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dtimespec-bound.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sig2str.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "operand2sig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "term-sig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

#ਜੇ HAVE_SETRLIMIT
/* FreeBSD 5.0 at least needs <sys/types.h> and <sys/time.h> included
   before <sys/resource.h>.  Currently "system.h" includes <sys/time.h>.  */
# include <sys/resource.h>
#ਜੇ_ਅੰਤ

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SA_RESTART
# define SA_RESTART 0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "timeout"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ਸਥਿਰ ਪੂਰਨ timed_ਬਾਹਰਲਾ;
ਸਥਿਰ ਪੂਰਨ ਪਦ_signal = SIGTERM;  /* same default as kill command.  */
ਸਥਿਰ pid_t monitored_pid;
ਸਥਿਰ ਦੁੱਗਣਾ kill_after;
ਸਥਿਰ bool foreground;      /* whether to use another program group.  */
ਸਥਿਰ bool preserve_ਸਥਿਤੀ; /* whether to use a timeout status or not.  */
ਸਥਿਰ bool verbose;         /* whether to diagnose timeouts or not.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *command;

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"foreground", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"kill-after", required_ਦਲੀਲ, ਨਿਰਾ, 'k'},
  {"preserve-status", no_ਦਲੀਲ, ਨਿਰਾ, 'p'},
  {"signal", required_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Start the timeout after which we'll receive a SIGALRM.
   Round DURATION up to the next representable value.
   Treat out-of-range values as if they were maximal,
   as that's more useful in practice than reporting an error.
   '0' means don't timeout.  */
ਸਥਿਰ ਖਾਲੀ
settimeout (ਦੁੱਗਣਾ duration, bool warn)
{

#ਜੇ HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  ਬਣਤਰ timespec ts = dtotimespec (duration);
  ਬਣਤਰ itimerspec its = {.it_interval = {0}, .it_ਮੁੱਲ = ts};
  timer_t timerid;
  ਜੇ (timer_create (CLOCK_REALTIME, ਨਿਰਾ, &timerid) == 0)
    {
      ਜੇ (timer_settime (timerid, 0, &its, ਨਿਰਾ) == 0)
        ਵਾਪਸ;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (warn)
            error (0, errno, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (warn && errno != ENOSYS)
    error (0, errno, _("warning: timer_create"));

#ਜਾਂਜੇ HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  ਬਣਤਰ timeval tv;
  ਬਣਤਰ timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  ਜੇ (tv.tv_usec == 1000 * 1000)
    {
      ਜੇ (tv.tv_sec != TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (time_t))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      ਨਹੀਂਤਾਂ
        tv.tv_usec--;
    }
  ਬਣਤਰ itimerval it = {.it_interval = {0}, .it_ਮੁੱਲ = tv };
  ਜੇ (setitimer (ITIMER_REAL, &it, ਨਿਰਾ) == 0)
    ਵਾਪਸ;
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (warn && errno != ENOSYS)
        error (0, errno, _("warning: setitimer"));
    }
#ਜੇ_ਅੰਤ

  /* fallback to single second resolution provided by alarm().  */

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ timeint;
  ਜੇ (UINT_MAX <= duration)
    timeint = UINT_MAX;
  ਨਹੀਂਤਾਂ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

ਸਥਿਰ ਪੂਰਨ
send_sig (pid_t where, ਪੂਰਨ sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  ਜੇ (where == 0)
    ਸੰਕੇਤ (sig, SIG_IGN);
  ਵਾਪਸ kill (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
ਸਥਿਰ ਖਾਲੀ
chld (MAYBE_ਅਣਵਰਤਿਆ ਪੂਰਨ sig)
{
}

ਸਥਿਰ ਖਾਲੀ
cleanup (ਪੂਰਨ sig)
{
  ਜੇ (sig == SIGALRM)
    {
      timed_ਬਾਹਰਲਾ = 1;
      /* In case there is an issue with close_stdout,
         update to a more accurate default exit status.
         For example we might get failed writes with -v with:
           timeout -v 1 sleep 10 2>&1 | :
      */
      initialize_exit_failure (EXIT_TIMEDOUT);
      sig = ਪਦ_signal;
    }
  ਜੇ (0 < monitored_pid)
    {
      ਜੇ (kill_after)
        {
          ਪੂਰਨ saved_errno = errno; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          ਪਦ_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          errno = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      ਜੇ (verbose)
        {
          ਅੱਖਰ signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (ਪੂਰਨ))];
          ਜੇ (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, ਆਕਾਰ signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      ਜੇ (!foreground)
        {
          send_sig (0, sig);
          ਜੇ (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  ਨਹੀਂਤਾਂ /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), ਪ੍ਰੋਗਰਾਮ_name);

      fputs (_("\
ਸ਼ੁਰੂ_2 COMMAND, and kill it ਜੇ still running after DURATION.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --foreground\n\
         when not running timeout directly from a shell prompt,\n\
         allow COMMAND ਮੰਜ਼ਿਲ ਪਠ from the TTY and get TTY ਸੰਕੇਤ_2;\n\
         ਵਿੱਚ this ਬਹੁਲਕ_2, children ਦਾ COMMAND will not be timed ਬਾਹਰਲਾ\n\
"));
      oputs (_("\
  -k, --kill-after=DURATION\n\
         also send a KILL ਸੰਕੇਤ ਜੇ COMMAND is still running\n\
         this ਲੰਮਾ after the initial ਸੰਕੇਤ was ਭੇਜਿਆ\n\
"));
      oputs (_("\
  -p, --preserve-ਸਥਿਤੀ\n\
         ਬਾਹਰ_ਨਿਕਲੋ with the same ਸਥਿਤੀ as COMMAND,\n\
         even when the command times ਬਾਹਰਲਾ\n\
"));
      oputs (_("\
  -s, --ਸੰਕੇਤ=SIGNAL\n\
         specify the ਸੰਕੇਤ ਮੰਜ਼ਿਲ be ਭੇਜਿਆ on timeout;\n\
         SIGNAL may be a name like 'HUP' or a ਸੰਖਿਆ;\n\
         see 'kill -l' ਲਈ a list ਦਾ ਸੰਕੇਤ_2\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose ਮੰਜ਼ਿਲ standard error any ਸੰਕੇਤ ਭੇਜਿਆ upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);

      fputs (_("\n\
DURATION is a floating ਬਿੰਦੂ ਸੰਖਿਆ with an optional suffix:\n\
's' ਲਈ seconds (the ਮੂਲ), 'm' ਲਈ minutes, 'h' ਲਈ hours or \
'd' ਲਈ days.\nA duration ਦਾ 0 disables the associated timeout.\n"), stdout);

      fputs (_("\n\
Upon timeout, send the ਪਦ_2 ਸੰਕੇਤ ਮੰਜ਼ਿਲ COMMAND, ਜੇ no other SIGNAL specified.\n\
The ਪਦ_2 ਸੰਕੇਤ kills any process that does not ਖੰਡ or catch that ਸੰਕੇਤ.\n\
It may be necessary ਮੰਜ਼ਿਲ use the KILL ਸੰਕੇਤ, since this ਸੰਕੇਤ can't be caught.\
\n"), stdout);

      fputs (_("\n\
Exit ਸਥਿਤੀ:\n\
  124  ਜੇ COMMAND times ਬਾਹਰਲਾ, and --preserve-ਸਥਿਤੀ is not specified\n\
  125  ਜੇ the timeout command itself fails\n\
  126  ਜੇ COMMAND is ਮਿਲਿਆ but cannot be invoked\n\
  127  ਜੇ COMMAND cannot be ਮਿਲਿਆ\n\
  137  ਜੇ COMMAND (or timeout itself) is ਭੇਜਿਆ the KILL (9) ਸੰਕੇਤ (128+9)\n\
  -    the ਬਾਹਰ_ਨਿਕਲੋ ਸਥਿਤੀ ਦਾ COMMAND otherwise\n\
"), stdout);

      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

ਸਥਿਰ bool
apply_ਸਮਾਂ_suffix (ਦੁੱਗਣਾ *x, ਅੱਖਰ suffix_char)
{
  ਪੂਰਨ multiplier;

  ਚੋਣ (suffix_char)
    {
    ਹਾਲਤ 0:
    ਹਾਲਤ 's':
      multiplier = 1;
      ਰੋਕੋ;
    ਹਾਲਤ 'm':
      multiplier = 60;
      ਰੋਕੋ;
    ਹਾਲਤ 'h':
      multiplier = 60 * 60;
      ਰੋਕੋ;
    ਹਾਲਤ 'd':
      multiplier = 60 * 60 * 24;
      ਰੋਕੋ;
    ਮੂਲ:
      ਵਾਪਸ false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  ਵਾਪਸ true;
}

ਸਥਿਰ ਦੁੱਗਣਾ
parse_duration (ਅੱਖਰ ਅਚਲ *str)
{
  ਅੱਖਰ *ep;
  errno = 0;
  ਦੁੱਗਣਾ duration = cl_strtod (str, &ep);
  ਦੁੱਗਣਾ s = dtimespec_bound (duration, errno);

  ਜੇ (ep == str
      /* Nonnegative interval.  */
      || ! (0 <= s)
      /* No extra chars after the number and an optional s,m,h,d char.  */
      || (*ep && *(ep + 1))
      /* Check any suffix char and update timeout based on the suffix.  */
      || !apply_ਸਮਾਂ_suffix (&s, *ep))
    {
      error (0, 0, _("invalid time interval %s"), quote (str));
      usage (EXIT_CANCELED);
    }

  ਵਾਪਸ s;
}

ਸਥਿਰ ਖਾਲੀ
unblock_signal (ਪੂਰਨ sig)
{
  sigset_t unblock_ਸੈੱਟ;
  sigemptyset (&unblock_ਸੈੱਟ);
  sigaddset (&unblock_ਸੈੱਟ, sig);
  ਜੇ (sigprocmask (SIG_UNBLOCK, &unblock_ਸੈੱਟ, ਨਿਰਾ) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

ਸਥਿਰ ਖਾਲੀ
install_sigchld (ਖਾਲੀ)
{
  ਬਣਤਰ sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = chld;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  sigaction (SIGCHLD, &sa, ਨਿਰਾ);

  /* We inherit the signal mask from our parent process,
     so ensure SIGCHLD is not blocked. */
  unblock_signal (SIGCHLD);
}

/* Filter out signals that were ignored.  */

ਸਥਿਰ bool
sig_needs_handling (ਪੂਰਨ sig, ਪੂਰਨ sigterm)
{
  ਜੇ (sig == SIGALRM || sig == sigterm)
    ਵਾਪਸ true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  ਬਣਤਰ sigaction old_sa;
  sigaction (sig, ਨਿਰਾ, &old_sa);
  bool ret = old_sa.sa_handler != SIG_IGN;
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ
install_cleanup (ਪੂਰਨ sigterm)
{
  ਬਣਤਰ sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  ਲਈ (ਪੂਰਨ i = 0; i < countof (ਪਦ_sig); i++)
    ਜੇ (sig_needs_handling (ਪਦ_sig[i], sigterm))
      sigaction (ਪਦ_sig[i], &sa, ਨਿਰਾ);

  /* Real Time signals also terminate by default.  */
  ਲਈ (ਪੂਰਨ s = SIGRTMIN; s <= SIGRTMAX; s++)
    ਜੇ (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, ਨਿਰਾ);

  sigaction (sigterm, &sa, ਨਿਰਾ); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
ਸਥਿਰ ਖਾਲੀ
ਖੰਡ_cleanup_and_chld (ਪੂਰਨ sigterm, sigset_t *old_ਸੈੱਟ)
{
  sigset_t ਖੰਡ_ਸੈੱਟ;
  sigemptyset (&ਖੰਡ_ਸੈੱਟ);

  ਲਈ (ਪੂਰਨ i = 0; i < countof (ਪਦ_sig); i++)
    ਜੇ (sig_needs_handling (ਪਦ_sig[i], sigterm))
      sigaddset (&ਖੰਡ_ਸੈੱਟ, ਪਦ_sig[i]);

  ਲਈ (ਪੂਰਨ s = SIGRTMIN; s <= SIGRTMAX; s++)
    ਜੇ (sig_needs_handling (s, sigterm))
      sigaddset (&ਖੰਡ_ਸੈੱਟ, s);

  sigaddset (&ਖੰਡ_ਸੈੱਟ, sigterm);

  sigaddset (&ਖੰਡ_ਸੈੱਟ, SIGCHLD);

  ਜੇ (sigprocmask (SIG_BLOCK, &ਖੰਡ_ਸੈੱਟ, old_ਸੈੱਟ) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
ਸਥਿਰ bool
disable_core_dumps (ਖਾਲੀ)
{
#ਜੇ HAVE_PRCTL && defined PR_ਸੈੱਟ_DUMPABLE
  ਜੇ (prctl (PR_ਸੈੱਟ_DUMPABLE, 0) == 0)
    ਵਾਪਸ true;

#ਜਾਂਜੇ HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  ਜੇ (setrlimit (RLIMIT_CORE, &(ਬਣਤਰ rlimit) {0}) == 0)
    ਵਾਪਸ true;

#ਨਹੀਂਤਾਂ
  ਵਾਪਸ false;
#ਜੇ_ਅੰਤ

  error (0, errno, _("warning: disabling core dumps failed"));
  ਵਾਪਸ false;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਦੁੱਗਣਾ timeout;
  ਪੂਰਨ c;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ਜਦੋਂ ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'f':
          foreground = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'k':
          kill_after = parse_duration (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'p':
          preserve_ਸਥਿਤੀ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          ਪਦ_signal = operand2sig (optarg);
          ਜੇ (ਪਦ_signal == -1)
            usage (EXIT_CANCELED);
          ਰੋਕੋ;

        ਹਾਲਤ 'v':
          verbose = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_CANCELED);
          ਰੋਕੋ;
        }
    }

  ਜੇ (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  ਜੇ (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (ਪਦ_signal);
  ਸੰਕੇਤ (SIGTTIN, SIG_IGN);   /* Don't stop if background child needs tty.  */
  ਸੰਕੇਤ (SIGTTOU, SIG_IGN);   /* Don't stop if background child needs tty.  */
  install_sigchld ();          /* Interrupt sigsuspend() when child exits.   */

  /* We configure timers so that SIGALRM is sent on expiry.
     Therefore ensure we don't inherit a mask blocking SIGALRM.  */
  unblock_signal (SIGALRM);

  /* Block signals now, so monitored_pid is deterministic in cleanup().  */
  sigset_t orig_ਸੈੱਟ;
  ਖੰਡ_cleanup_and_chld (ਪਦ_signal, &orig_ਸੈੱਟ);

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
  ਜੇ (monitored_pid == -1)
    {
      error (0, errno, _("fork system call failed"));
      ਵਾਪਸ EXIT_CANCELED;
    }
  ਨਹੀਂਤਾਂ ਜੇ (monitored_pid == 0)  /*ਬੱਚਾ*/
    {
#ਜੇ HAVE_PRCTL && defined PR_ਸੈੱਟ_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_ਸੈੱਟ_PDEATHSIG, ਪਦ_signal);
#ਜੇ_ਅੰਤ
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      ਜੇ (getppid () != timeout_pid)
        ਵਾਪਸ EXIT_CANCELED;

      /* Restore signal mask for child.  */
      ਜੇ (sigprocmask (SIG_SETMASK, &orig_ਸੈੱਟ, ਨਿਰਾ) != 0)
        {
          error (0, errno, _("child failed to reset signal mask"));
          ਵਾਪਸ EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      ਸੰਕੇਤ (SIGTTIN, SIG_DFL);
      ਸੰਕੇਤ (SIGTTOU, SIG_DFL);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      ਪੂਰਨ exit_ਸਥਿਤੀ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, errno, _("failed to run command %s"), quote (command));
      ਵਾਪਸ exit_ਸਥਿਤੀ;
    }
  ਨਹੀਂਤਾਂ
    {
      pid_t wait_ਨਤੀਜਾ;
      ਪੂਰਨ ਸਥਿਤੀ;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      ਜਦੋਂ ((wait_ਨਤੀਜਾ = waitpid (monitored_pid, &ਸਥਿਤੀ, WNOHANG)) == 0)
        sigsuspend (&orig_ਸੈੱਟ);  /* Wait with cleanup signals unblocked.  */

      ਜੇ (wait_ਨਤੀਜਾ < 0)
        {
          /* shouldn't happen.  */
          error (0, errno, _("error waiting for command"));
          ਸਥਿਤੀ = EXIT_CANCELED;
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (WIFEXITED (ਸਥਿਤੀ))
            ਸਥਿਤੀ = WEXITSTATUS (ਸਥਿਤੀ);
          ਨਹੀਂਤਾਂ ਜੇ (WIFSIGNALED (ਸਥਿਤੀ))
            {
              ਪੂਰਨ sig = WTERMSIG (ਸਥਿਤੀ);
              ਜੇ (WCOREDUMP (ਸਥਿਤੀ))
                error (0, 0, _("the monitored command dumped core"));
              ਜੇ (!timed_ਬਾਹਰਲਾ && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  ਸੰਕੇਤ (sig, SIG_DFL);
                  unblock_signal (sig);
                  ਉਠਾਓ (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              ਜੇ (timed_ਬਾਹਰਲਾ && sig == SIGKILL)
                preserve_ਸਥਿਤੀ = true;
              ਸਥਿਤੀ = sig + 128; /* what sh returns for signaled processes.  */
            }
          ਨਹੀਂਤਾਂ
            {
              /* shouldn't happen.  */
              error (0, 0, _("unknown status from command (%d)"), ਸਥਿਤੀ);
              ਸਥਿਤੀ = EXIT_FAILURE;
            }
        }

      ਜੇ (timed_ਬਾਹਰਲਾ && !preserve_ਸਥਿਤੀ)
        ਸਥਿਤੀ = EXIT_TIMEDOUT;
      ਵਾਪਸ ਸਥਿਤੀ;
    }
}
