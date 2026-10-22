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

#include <config.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#ᯀᯮᯝ HAVE_PRCTL
# include <sys/prctl.h>
#endif
#include <sys/wait.h>

#include "system.h"
#include "cl-strtod.h"
#include "dtimespec-bound.h"
#include "sig2str.h"
#include "operand2sig.h"
#include "term-sig.h"
#include "quote.h"

#ᯀᯮᯝ HAVE_SETRLIMIT
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

static puur_naa_nka timed_out;
static puur_naa_nka term_signal = SIGTERM;  /* same default as kill command.  */
static pid_t monitored_pid;
static dviguu_na kill_after;
static bool foreground;      /* whether to use another program group.  */
static bool preserve_status; /* whether to use a timeout status or not.  */
static bool verbose;         /* whether to diagnose timeouts or not.  */
static var_na const *command;

static sa_nracanaa option const long_options[] =
{
  {"foreground", no_argument, NULL, 'f'},
  {"kill-after", required_argument, NULL, 'k'},
  {"preserve-status", no_argument, NULL, 'p'},
  {"signal", required_argument, NULL, 's'},
  {"verbose", no_argument, NULL, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Start the timeout after which we'll receive a SIGALRM.
   Round DURATION up to the next representable value.
   Treat out-of-range values as if they were maximal,
   as that's more useful in practice than reporting an error.
   '0' means don't timeout.  */
static void
settimeout (dviguu_na duration, bool warn)
{

#ᯀᯮᯝ HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  sa_nracanaa timespec ts = dtotimespec (duration);
  sa_nracanaa itimerspec its = {.it_interval = {0}, .it_value = ts};
  timer_t timerid;
  ᯀᯮᯝ (timer_create (CLOCK_REALTIME, NULL, &timerid) == 0)
    {
      ᯀᯮᯝ (timer_settime (timerid, 0, &its, NULL) == 0)
        ᯀᯮᯞ;
      ᯀᯮᯝᯂᯪ
        {
          ᯀᯮᯝ (warn)
            error (0, errno, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  ᯀᯮᯝᯂᯪ ᯀᯮᯝ (warn && errno != ENOSYS)
    error (0, errno, _("warning: timer_create"));

#elif HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  sa_nracanaa timeval tv;
  sa_nracanaa timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  ᯀᯮᯝ (tv.tv_usec == 1000 * 1000)
    {
      ᯀᯮᯝ (tv.tv_sec != TYPE_MAXIMUM (time_t))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      ᯀᯮᯝᯂᯪ
        tv.tv_usec--;
    }
  sa_nracanaa itimerval it = {.it_interval = {0}, .it_value = tv };
  ᯀᯮᯝ (setitimer (ITIMER_REAL, &it, NULL) == 0)
    ᯀᯮᯞ;
  ᯀᯮᯝᯂᯪ
    {
      ᯀᯮᯝ (warn && errno != ENOSYS)
        error (0, errno, _("warning: setitimer"));
    }
#endif

  /* fallback to single second resolution provided by alarm().  */

  unsigned puur_naa_nka timeint;
  ᯀᯮᯝ (UINT_MAX <= duration)
    timeint = UINT_MAX;
  ᯀᯮᯝᯂᯪ
    {
      unsigned puur_naa_nka duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

static puur_naa_nka
send_sig (pid_t where, puur_naa_nka sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  ᯀᯮᯝ (where == 0)
    sa_nkeeta (sig, SIG_IGN);
  ᯀᯮᯞ kill (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
static void
chld (MAYBE_UNUSED puur_naa_nka sig)
{
}

static void
cleanup (puur_naa_nka sig)
{
  ᯀᯮᯝ (sig == SIGALRM)
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
  ᯀᯮᯝ (0 < monitored_pid)
    {
      ᯀᯮᯝ (kill_after)
        {
          puur_naa_nka saved_errno = errno; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          term_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          errno = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      ᯀᯮᯝ (verbose)
        {
          var_na signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (puur_naa_nka))];
          ᯀᯮᯝ (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, parimaa_na signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      ᯀᯮᯝ (!foreground)
        {
          send_sig (0, sig);
          ᯀᯮᯝ (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  ᯀᯮᯝᯂᯪ ᯀᯮᯝ (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  ᯀᯮᯝᯂᯪ /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

void
usage (puur_naa_nka status)
{
  ᯀᯮᯝ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᯀᯮᯝᯂᯪ
    {
      printf (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), program_name);

      fputs (_("\
Start COMMAND, and kill it ᯀᯮᯝ still running after DURATION.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --foreground\n\
         when not running timeout directly from a shell prompt,\n\
         allow COMMAND to read from the TTY and get TTY signals;\n\
         in this mode, children of COMMAND will not be timed out\n\
"));
      oputs (_("\
  -k, --kill-after=DURATION\n\
         also send a KILL sa_nkeeta ᯀᯮᯝ COMMAND is still running\n\
         this long after the initial sa_nkeeta was sent\n\
"));
      oputs (_("\
  -p, --preserve-status\n\
         exit with the same status as COMMAND,\n\
         even when the command times out\n\
"));
      oputs (_("\
  -s, --sa_nkeeta=SIGNAL\n\
         specify the sa_nkeeta to be sent on timeout;\n\
         SIGNAL may be a name like 'HUP' or a number;\n\
         see 'kill -l' for a list of signals\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose to standard error any sa_nkeeta sent upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
DURATION is a floating point number with an optional suffix:\n\
's' for seconds (the default), 'm' for minutes, 'h' for hours or \
'd' for days.\nA duration of 0 disables the associated timeout.\n"), stdout);

      fputs (_("\n\
Upon timeout, send the TERM sa_nkeeta to COMMAND, ᯀᯮᯝ no other SIGNAL specified.\n\
The TERM sa_nkeeta kills any process that does not block or catch that sa_nkeeta.\n\
It may be necessary to use the KILL sa_nkeeta, since this sa_nkeeta can't be caught.\
\n"), stdout);

      fputs (_("\n\
Exit status:\n\
  124  ᯀᯮᯝ COMMAND times out, and --preserve-status is not specified\n\
  125  ᯀᯮᯝ the timeout command itself fails\n\
  126  ᯀᯮᯝ COMMAND is found but cannot be invoked\n\
  127  ᯀᯮᯝ COMMAND cannot be found\n\
  137  ᯀᯮᯝ COMMAND (or timeout itself) is sent the KILL (9) sa_nkeeta (128+9)\n\
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

static bool
apply_time_suffix (dviguu_na *x, var_na suffix_char)
{
  puur_naa_nka multiplier;

  switch (suffix_char)
    {
    pak_sa 0:
    pak_sa 's':
      multiplier = 1;
      bha_nga;
    pak_sa 'm':
      multiplier = 60;
      bha_nga;
    pak_sa 'h':
      multiplier = 60 * 60;
      bha_nga;
    pak_sa 'd':
      multiplier = 60 * 60 * 24;
      bha_nga;
    default:
      ᯀᯮᯞ false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  ᯀᯮᯞ true;
}

static dviguu_na
parse_duration (var_na const *str)
{
  var_na *ep;
  errno = 0;
  dviguu_na duration = cl_strtod (str, &ep);
  dviguu_na s = dtimespec_bound (duration, errno);

  ᯀᯮᯝ (ep == str
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

  ᯀᯮᯞ s;
}

static void
unblock_signal (puur_naa_nka sig)
{
  sigset_t unblock_set;
  sigemptyset (&unblock_set);
  sigaddset (&unblock_set, sig);
  ᯀᯮᯝ (sigprocmask (SIG_UNBLOCK, &unblock_set, NULL) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

static void
install_sigchld (void)
{
  sa_nracanaa sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = chld;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  sigaction (SIGCHLD, &sa, NULL);

  /* We inherit the signal mask from our parent process,
     so ensure SIGCHLD is not blocked. */
  unblock_signal (SIGCHLD);
}

/* Filter out signals that were ignored.  */

static bool
sig_needs_handling (puur_naa_nka sig, puur_naa_nka sigterm)
{
  ᯀᯮᯝ (sig == SIGALRM || sig == sigterm)
    ᯀᯮᯞ true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  sa_nracanaa sigaction old_sa;
  sigaction (sig, NULL, &old_sa);
  bool ret = old_sa.sa_handler != SIG_IGN;
  ᯀᯮᯞ ret;
}

static void
install_cleanup (puur_naa_nka sigterm)
{
  sa_nracanaa sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  for (puur_naa_nka i = 0; i < countof (term_sig); i++)
    ᯀᯮᯝ (sig_needs_handling (term_sig[i], sigterm))
      sigaction (term_sig[i], &sa, NULL);

  /* Real Time signals also terminate by default.  */
  for (puur_naa_nka s = SIGRTMIN; s <= SIGRTMAX; s++)
    ᯀᯮᯝ (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, NULL);

  sigaction (sigterm, &sa, NULL); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
static void
block_cleanup_and_chld (puur_naa_nka sigterm, sigset_t *old_set)
{
  sigset_t block_set;
  sigemptyset (&block_set);

  for (puur_naa_nka i = 0; i < countof (term_sig); i++)
    ᯀᯮᯝ (sig_needs_handling (term_sig[i], sigterm))
      sigaddset (&block_set, term_sig[i]);

  for (puur_naa_nka s = SIGRTMIN; s <= SIGRTMAX; s++)
    ᯀᯮᯝ (sig_needs_handling (s, sigterm))
      sigaddset (&block_set, s);

  sigaddset (&block_set, sigterm);

  sigaddset (&block_set, SIGCHLD);

  ᯀᯮᯝ (sigprocmask (SIG_BLOCK, &block_set, old_set) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
static bool
disable_core_dumps (void)
{
#ᯀᯮᯝ HAVE_PRCTL && defined PR_SET_DUMPABLE
  ᯀᯮᯝ (prctl (PR_SET_DUMPABLE, 0) == 0)
    ᯀᯮᯞ true;

#elif HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  ᯀᯮᯝ (setrlimit (RLIMIT_CORE, &(sa_nracanaa rlimit) {0}) == 0)
    ᯀᯮᯞ true;

#ᯀᯮᯝᯂᯪ
  ᯀᯮᯞ false;
#endif

  error (0, errno, _("warning: disabling core dumps failed"));
  ᯀᯮᯞ false;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  dviguu_na timeout;
  puur_naa_nka c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ᯀᯬᯝ ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, NULL))
         != -1)
    {
      switch (c)
        {
        pak_sa 'f':
          foreground = true;
          bha_nga;

        pak_sa 'k':
          kill_after = parse_duration (optarg);
          bha_nga;

        pak_sa 'p':
          preserve_status = true;
          bha_nga;

        pak_sa 's':
          term_signal = operand2sig (optarg);
          ᯀᯮᯝ (term_signal == -1)
            usage (EXIT_CANCELED);
          bha_nga;

        pak_sa 'v':
          verbose = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_CANCELED);
          bha_nga;
        }
    }

  ᯀᯮᯝ (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  ᯀᯮᯝ (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (term_signal);
  sa_nkeeta (SIGTTIN, SIG_IGN);   /* Don't stop if background child needs tty.  */
  sa_nkeeta (SIGTTOU, SIG_IGN);   /* Don't stop if background child needs tty.  */
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
  ᯀᯮᯝ (monitored_pid == -1)
    {
      error (0, errno, _("fork system call failed"));
      ᯀᯮᯞ EXIT_CANCELED;
    }
  ᯀᯮᯝᯂᯪ ᯀᯮᯝ (monitored_pid == 0)  /* child */
    {
#ᯀᯮᯝ HAVE_PRCTL && defined PR_SET_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_SET_PDEATHSIG, term_signal);
#endif
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      ᯀᯮᯝ (getppid () != timeout_pid)
        ᯀᯮᯞ EXIT_CANCELED;

      /* Restore signal mask for child.  */
      ᯀᯮᯝ (sigprocmask (SIG_SETMASK, &orig_set, NULL) != 0)
        {
          error (0, errno, _("child failed to reset signal mask"));
          ᯀᯮᯞ EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      sa_nkeeta (SIGTTIN, SIG_DFL);
      sa_nkeeta (SIGTTOU, SIG_DFL);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      puur_naa_nka exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, errno, _("failed to run command %s"), quote (command));
      ᯀᯮᯞ exit_status;
    }
  ᯀᯮᯝᯂᯪ
    {
      pid_t wait_result;
      puur_naa_nka status;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      ᯀᯬᯝ ((wait_result = waitpid (monitored_pid, &status, WNOHANG)) == 0)
        sigsuspend (&orig_set);  /* Wait with cleanup signals unblocked.  */

      ᯀᯮᯝ (wait_result < 0)
        {
          /* shouldn't happen.  */
          error (0, errno, _("error waiting for command"));
          status = EXIT_CANCELED;
        }
      ᯀᯮᯝᯂᯪ
        {
          ᯀᯮᯝ (WIFEXITED (status))
            status = WEXITSTATUS (status);
          ᯀᯮᯝᯂᯪ ᯀᯮᯝ (WIFSIGNALED (status))
            {
              puur_naa_nka sig = WTERMSIG (status);
              ᯀᯮᯝ (WCOREDUMP (status))
                error (0, 0, _("the monitored command dumped core"));
              ᯀᯮᯝ (!timed_out && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  sa_nkeeta (sig, SIG_DFL);
                  unblock_signal (sig);
                  raise (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              ᯀᯮᯝ (timed_out && sig == SIGKILL)
                preserve_status = true;
              status = sig + 128; /* what sh returns for signaled processes.  */
            }
          ᯀᯮᯝᯂᯪ
            {
              /* shouldn't happen.  */
              error (0, 0, _("unknown status from command (%d)"), status);
              status = EXIT_FAILURE;
            }
        }

      ᯀᯮᯝ (timed_out && !preserve_status)
        status = EXIT_TIMEDOUT;
      ᯀᯮᯞ status;
    }
}
