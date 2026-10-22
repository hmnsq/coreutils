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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଯଦି HAVE_PRCTL
# include <sys/prctl.h>
#endif
#ଅନ୍ତର୍ଭାଵ <sys/wait.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "cl-strtod.h"
#ଅନ୍ତର୍ଭାଵ "dtimespec-bound.h"
#ଅନ୍ତର୍ଭାଵ "sig2str.h"
#ଅନ୍ତର୍ଭାଵ "operand2sig.h"
#ଅନ୍ତର୍ଭାଵ "term-sig.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"

#ଯଦି HAVE_SETRLIMIT
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା timed_out;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା term_signal = SIGTERM;  /* same default as kill command.  */
ସ୍ଥିର pid_t monitored_pid;
ସ୍ଥିର ଦ୍ୱିଗୁଣ kill_after;
ସ୍ଥିର bool foreground;      /* whether to use another program group.  */
ସ୍ଥିର bool preserve_status; /* whether to use a timeout status or not.  */
ସ୍ଥିର bool verbose;         /* whether to diagnose timeouts or not.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *command;

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"foreground", no_argument, ଖାଲି, 'f'},
  {"kill-after", required_argument, ଖାଲି, 'k'},
  {"preserve-status", no_argument, ଖାଲି, 'p'},
  {"signal", required_argument, ଖାଲି, 's'},
  {"verbose", no_argument, ଖାଲି, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Start the timeout after which we'll receive a SIGALRM.
   Round DURATION up to the next representable value.
   Treat out-of-range values as if they were maximal,
   as that's more useful in practice than reporting an error.
   '0' means don't timeout.  */
ସ୍ଥିର ଶୂନ୍ୟ
settimeout (ଦ୍ୱିଗୁଣ duration, bool warn)
{

#ଯଦି HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  ସଂରଚନା timespec ts = dtotimespec (duration);
  ସଂରଚନା itimerspec its = {.it_interval = {0}, .it_ମୂଲ୍ୟ = ts};
  timer_t timerid;
  ଯଦି (timer_create (CLOCK_REALTIME, ଖାଲି, &timerid) == 0)
    {
      ଯଦି (timer_settime (timerid, 0, &its, ଖାଲି) == 0)
        ଫେରନ୍ତୁ;
      ଅନ୍ୟଥା
        {
          ଯଦି (warn)
            error (0, errno, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  ଅନ୍ୟଥା ଯଦି (warn && errno != ENOSYS)
    error (0, errno, _("warning: timer_create"));

#ନଚେତଯଦି HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  ସଂରଚନା timeval tv;
  ସଂରଚନା timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  ଯଦି (tv.tv_usec == 1000 * 1000)
    {
      ଯଦି (tv.tv_sec != TYPE_MAXIMUM (time_t))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      ଅନ୍ୟଥା
        tv.tv_usec--;
    }
  ସଂରଚନା itimerval it = {.it_interval = {0}, .it_ମୂଲ୍ୟ = tv };
  ଯଦି (setitimer (ITIMER_REAL, &it, ଖାଲି) == 0)
    ଫେରନ୍ତୁ;
  ଅନ୍ୟଥା
    {
      ଯଦି (warn && errno != ENOSYS)
        error (0, errno, _("warning: setitimer"));
    }
#endif

  /* fallback to single second resolution provided by alarm().  */

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା timeint;
  ଯଦି (UINT_MAX <= duration)
    timeint = UINT_MAX;
  ଅନ୍ୟଥା
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
send_sig (pid_t where, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  ଯଦି (where == 0)
    ସଙ୍କେତ (sig, SIG_IGN);
  ଫେରନ୍ତୁ kill (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
ସ୍ଥିର ଶୂନ୍ୟ
chld (MAYBE_UNUSED ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig)
{
}

ସ୍ଥିର ଶୂନ୍ୟ
cleanup (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig)
{
  ଯଦି (sig == SIGALRM)
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
  ଯଦି (0 < monitored_pid)
    {
      ଯଦି (kill_after)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          term_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          errno = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      ଯଦି (verbose)
        {
          ଅକ୍ଷର signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (ପୂର୍ଣ୍ଣସଂଖ୍ୟା))];
          ଯଦି (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, ଆକାର signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      ଯଦି (!foreground)
        {
          send_sig (0, sig);
          ଯଦି (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  ଅନ୍ୟଥା ଯଦି (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  ଅନ୍ୟଥା /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), program_name);

      fputs (_("\
Start COMMAND, and kill it ଯଦି still running after DURATION.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --foreground\n\
         when not running timeout directly from a shell prompt,\n\
         allow COMMAND to ପଠ from the TTY and get TTY signals;\n\
         in this mode, children of COMMAND will not be timed out\n\
"));
      oputs (_("\
  -k, --kill-after=DURATION\n\
         also send a KILL ସଙ୍କେତ ଯଦି COMMAND is still running\n\
         this long after the initial ସଙ୍କେତ was sent\n\
"));
      oputs (_("\
  -p, --preserve-status\n\
         exit with the same status as COMMAND,\n\
         even when the command times out\n\
"));
      oputs (_("\
  -s, --ସଙ୍କେତ=SIGNAL\n\
         specify the ସଙ୍କେତ to be sent on timeout;\n\
         SIGNAL may be a name like 'HUP' or a ସଂଖ୍ୟା;\n\
         see 'kill -l' ପାଇଁ a list of signals\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose to standard error any ସଙ୍କେତ sent upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
DURATION is a floating point ସଂଖ୍ୟା with an optional suffix:\n\
's' ପାଇଁ seconds (the ପୂର୍ବନିର୍ଦ୍ଧାରିତ), 'm' ପାଇଁ minutes, 'h' ପାଇଁ hours or \
'd' ପାଇଁ days.\nA duration of 0 disables the associated timeout.\n"), stdout);

      fputs (_("\n\
Upon timeout, send the TERM ସଙ୍କେତ to COMMAND, ଯଦି no other SIGNAL specified.\n\
The TERM ସଙ୍କେତ kills any process that does not block or catch that ସଙ୍କେତ.\n\
It may be necessary to use the KILL ସଙ୍କେତ, since this ସଙ୍କେତ can't be caught.\
\n"), stdout);

      fputs (_("\n\
Exit status:\n\
  124  ଯଦି COMMAND times out, and --preserve-status is not specified\n\
  125  ଯଦି the timeout command itself fails\n\
  126  ଯଦି COMMAND is found but cannot be invoked\n\
  127  ଯଦି COMMAND cannot be found\n\
  137  ଯଦି COMMAND (or timeout itself) is sent the KILL (9) ସଙ୍କେତ (128+9)\n\
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

ସ୍ଥିର bool
apply_time_suffix (ଦ୍ୱିଗୁଣ *x, ଅକ୍ଷର suffix_char)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା multiplier;

  ବାଛ (suffix_char)
    {
    ସ୍ଥିତି 0:
    ସ୍ଥିତି 's':
      multiplier = 1;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'm':
      multiplier = 60;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'h':
      multiplier = 60 * 60;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'd':
      multiplier = 60 * 60 * 24;
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଦ୍ୱିଗୁଣ
parse_duration (ଅକ୍ଷର ଅଚଳ *str)
{
  ଅକ୍ଷର *ep;
  errno = 0;
  ଦ୍ୱିଗୁଣ duration = cl_strtod (str, &ep);
  ଦ୍ୱିଗୁଣ s = dtimespec_bound (duration, errno);

  ଯଦି (ep == str
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

  ଫେରନ୍ତୁ s;
}

ସ୍ଥିର ଶୂନ୍ୟ
unblock_signal (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig)
{
  sigset_t unblock_set;
  sigemptyset (&unblock_set);
  sigaddset (&unblock_set, sig);
  ଯଦି (sigprocmask (SIG_UNBLOCK, &unblock_set, ଖାଲି) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

ସ୍ଥିର ଶୂନ୍ୟ
install_sigchld (ଶୂନ୍ୟ)
{
  ସଂରଚନା sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = chld;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  sigaction (SIGCHLD, &sa, ଖାଲି);

  /* We inherit the signal mask from our parent process,
     so ensure SIGCHLD is not blocked. */
  unblock_signal (SIGCHLD);
}

/* Filter out signals that were ignored.  */

ସ୍ଥିର bool
sig_needs_handling (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sigterm)
{
  ଯଦି (sig == SIGALRM || sig == sigterm)
    ଫେରନ୍ତୁ true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  ସଂରଚନା sigaction old_sa;
  sigaction (sig, ଖାଲି, &old_sa);
  bool ret = old_sa.sa_handler != SIG_IGN;
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ
install_cleanup (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sigterm)
{
  ସଂରଚନା sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < countof (term_sig); i++)
    ଯଦି (sig_needs_handling (term_sig[i], sigterm))
      sigaction (term_sig[i], &sa, ଖାଲି);

  /* Real Time signals also terminate by default.  */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = SIGRTMIN; s <= SIGRTMAX; s++)
    ଯଦି (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, ଖାଲି);

  sigaction (sigterm, &sa, ଖାଲି); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
ସ୍ଥିର ଶୂନ୍ୟ
block_cleanup_and_chld (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sigterm, sigset_t *old_set)
{
  sigset_t block_set;
  sigemptyset (&block_set);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < countof (term_sig); i++)
    ଯଦି (sig_needs_handling (term_sig[i], sigterm))
      sigaddset (&block_set, term_sig[i]);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = SIGRTMIN; s <= SIGRTMAX; s++)
    ଯଦି (sig_needs_handling (s, sigterm))
      sigaddset (&block_set, s);

  sigaddset (&block_set, sigterm);

  sigaddset (&block_set, SIGCHLD);

  ଯଦି (sigprocmask (SIG_BLOCK, &block_set, old_set) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
ସ୍ଥିର bool
disable_core_dumps (ଶୂନ୍ୟ)
{
#ଯଦି HAVE_PRCTL && defined PR_SET_DUMPABLE
  ଯଦି (prctl (PR_SET_DUMPABLE, 0) == 0)
    ଫେରନ୍ତୁ true;

#ନଚେତଯଦି HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  ଯଦି (setrlimit (RLIMIT_CORE, &(ସଂରଚନା rlimit) {0}) == 0)
    ଫେରନ୍ତୁ true;

#ଅନ୍ୟଥା
  ଫେରନ୍ତୁ false;
#endif

  error (0, errno, _("warning: disabling core dumps failed"));
  ଫେରନ୍ତୁ false;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଦ୍ୱିଗୁଣ timeout;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, ଖାଲି))
         != -1)
    {
      ବାଛ (c)
        {
        ସ୍ଥିତି 'f':
          foreground = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'k':
          kill_after = parse_duration (optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'p':
          preserve_status = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 's':
          term_signal = operand2sig (optarg);
          ଯଦି (term_signal == -1)
            usage (EXIT_CANCELED);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'v':
          verbose = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_CANCELED);
          ଭାଙ୍ଗ;
        }
    }

  ଯଦି (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  ଯଦି (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (term_signal);
  ସଙ୍କେତ (SIGTTIN, SIG_IGN);   /* Don't stop if background child needs tty.  */
  ସଙ୍କେତ (SIGTTOU, SIG_IGN);   /* Don't stop if background child needs tty.  */
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
  ଯଦି (monitored_pid == -1)
    {
      error (0, errno, _("fork system call failed"));
      ଫେରନ୍ତୁ EXIT_CANCELED;
    }
  ଅନ୍ୟଥା ଯଦି (monitored_pid == 0)  /* child */
    {
#ଯଦି HAVE_PRCTL && defined PR_SET_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_SET_PDEATHSIG, term_signal);
#endif
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      ଯଦି (getppid () != timeout_pid)
        ଫେରନ୍ତୁ EXIT_CANCELED;

      /* Restore signal mask for child.  */
      ଯଦି (sigprocmask (SIG_SETMASK, &orig_set, ଖାଲି) != 0)
        {
          error (0, errno, _("child failed to reset signal mask"));
          ଫେରନ୍ତୁ EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      ସଙ୍କେତ (SIGTTIN, SIG_DFL);
      ସଙ୍କେତ (SIGTTOU, SIG_DFL);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, errno, _("failed to run command %s"), quote (command));
      ଫେରନ୍ତୁ exit_status;
    }
  ଅନ୍ୟଥା
    {
      pid_t wait_result;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      ଯେତେବେଳେ ((wait_result = waitpid (monitored_pid, &status, WNOHANG)) == 0)
        sigsuspend (&orig_set);  /* Wait with cleanup signals unblocked.  */

      ଯଦି (wait_result < 0)
        {
          /* shouldn't happen.  */
          error (0, errno, _("error waiting for command"));
          status = EXIT_CANCELED;
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (WIFEXITED (status))
            status = WEXITSTATUS (status);
          ଅନ୍ୟଥା ଯଦି (WIFSIGNALED (status))
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig = WTERMSIG (status);
              ଯଦି (WCOREDUMP (status))
                error (0, 0, _("the monitored command dumped core"));
              ଯଦି (!timed_out && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  ସଙ୍କେତ (sig, SIG_DFL);
                  unblock_signal (sig);
                  ଉଠାଅ (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              ଯଦି (timed_out && sig == SIGKILL)
                preserve_status = true;
              status = sig + 128; /* what sh returns for signaled processes.  */
            }
          ଅନ୍ୟଥା
            {
              /* shouldn't happen.  */
              error (0, 0, _("unknown status from command (%d)"), status);
              status = EXIT_FAILURE;
            }
        }

      ଯଦି (timed_out && !preserve_status)
        status = EXIT_TIMEDOUT;
      ଫେରନ୍ତୁ status;
    }
}
