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

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>
#जर HAVE_PRCTL
# include <sys/prctl.h>
#endif
#अन्तर्भाव <sys/wait.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "cl-strtod.h"
#अन्तर्भाव "dtimespec-bound.h"
#अन्तर्भाव "sig2str.h"
#अन्तर्भाव "operand2sig.h"
#अन्तर्भाव "term-sig.h"
#अन्तर्भाव "quote.h"

#जर HAVE_SETRLIMIT
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

स्थिर पूर्णांक timed_out;
स्थिर पूर्णांक term_signal = संक_इति;  /* same default as kill command.  */
स्थिर pid_t monitored_pid;
स्थिर द्विगुण kill_after;
स्थिर bool foreground;      /* whether to use another program group.  */
स्थिर bool preserve_status; /* whether to use a timeout status or not.  */
स्थिर bool verbose;         /* whether to diagnose timeouts or not.  */
स्थिर अक्षर स्थिरांक *command;

स्थिर संरचना option स्थिरांक long_options[] =
{
  {"foreground", no_argument, रिक्त, 'f'},
  {"kill-after", required_argument, रिक्त, 'k'},
  {"preserve-status", no_argument, रिक्त, 'p'},
  {"signal", required_argument, रिक्त, 's'},
  {"verbose", no_argument, रिक्त, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Start the timeout after which we'll receive a SIGALRM.
   Round DURATION up to the next representable value.
   Treat out-of-range values as if they were maximal,
   as that's more useful in practice than reporting an error.
   '0' means don't timeout.  */
स्थिर शून्य
settimeout (द्विगुण duration, bool warn)
{

#जर HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  संरचना timespec ts = dtotimespec (duration);
  संरचना itimerspec its = {.it_interval = {0}, .it_मूल्य = ts};
  timer_t timerid;
  जर (timer_create (CLOCK_REALTIME, रिक्त, &timerid) == 0)
    {
      जर (timer_settime (timerid, 0, &its, रिक्त) == 0)
        परत;
      अन्यथा
        {
          जर (warn)
            error (0, त्रुटि_सं, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  अन्यथा जर (warn && त्रुटि_सं != ENOSYS)
    error (0, त्रुटि_सं, _("warning: timer_create"));

#किंवाजर HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  संरचना timeval tv;
  संरचना timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  जर (tv.tv_usec == 1000 * 1000)
    {
      जर (tv.tv_sec != TYPE_MAXIMUM (समय_प्रकार))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      अन्यथा
        tv.tv_usec--;
    }
  संरचना itimerval it = {.it_interval = {0}, .it_मूल्य = tv };
  जर (setitimer (ITIMER_REAL, &it, रिक्त) == 0)
    परत;
  अन्यथा
    {
      जर (warn && त्रुटि_सं != ENOSYS)
        error (0, त्रुटि_सं, _("warning: setitimer"));
    }
#endif

  /* fallback to single second resolution provided by alarm().  */

  अचिन्हित पूर्णांक timeint;
  जर (अच_पूर्णांक_उच्च <= duration)
    timeint = अच_पूर्णांक_उच्च;
  अन्यथा
    {
      अचिन्हित पूर्णांक duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

स्थिर पूर्णांक
send_sig (pid_t where, पूर्णांक sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  जर (where == 0)
    संकेत (sig, संक_छोड़ो);
  परत समाप्त (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
स्थिर शून्य
chld (MAYBE_UNUSED पूर्णांक sig)
{
}

स्थिर शून्य
cleanup (पूर्णांक sig)
{
  जर (sig == SIGALRM)
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
  जर (0 < monitored_pid)
    {
      जर (kill_after)
        {
          पूर्णांक saved_errno = त्रुटि_सं; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          term_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          त्रुटि_सं = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      जर (verbose)
        {
          अक्षर signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (पूर्णांक))];
          जर (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, आकार signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      जर (!foreground)
        {
          send_sig (0, sig);
          जर (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  अन्यथा जर (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  अन्यथा /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), program_name);

      ख_माला_दो (_("\
Start COMMAND, and समाप्त it जर still running after DURATION.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --foreground\n\
         when not running timeout directly from a shell prompt,\n\
         allow COMMAND to पठ from the TTY and get TTY signals;\n\
         in this mode, children of COMMAND will not be timed out\n\
"));
      oputs (_("\
  -k, --समाप्त-after=DURATION\n\
         also send a KILL संकेत जर COMMAND is still running\n\
         this दीर्घ after the initial संकेत was sent\n\
"));
      oputs (_("\
  -p, --preserve-status\n\
         निकास with the same status as COMMAND,\n\
         even when the command बार out\n\
"));
      oputs (_("\
  -s, --संकेत=SIGNAL\n\
         specify the संकेत to be sent on timeout;\n\
         SIGNAL may be a name like 'HUP' or a संख्या;\n\
         see 'kill -l' साठी a list of signals\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose to standard error any संकेत sent upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      ख_माला_दो (_("\n\
DURATION is a floating point संख्या with an optional suffix:\n\
's' साठी seconds (the पूर्वनिर्धारित), 'm' साठी minutes, 'h' साठी hours or \
'd' साठी days.\nA duration of 0 disables the associated timeout.\n"), मानक_निकास);

      ख_माला_दो (_("\n\
Upon timeout, send the TERM संकेत to COMMAND, जर no other SIGNAL specified.\n\
The TERM संकेत kills any process that does not block or catch that संकेत.\n\
It may be necessary to use the KILL संकेत, since this संकेत can't be caught.\
\n"), मानक_निकास);

      ख_माला_दो (_("\n\
Exit status:\n\
  124  जर COMMAND बार out, and --preserve-status is not specified\n\
  125  जर the timeout command itself fails\n\
  126  जर COMMAND is found but cannot be invoked\n\
  127  जर COMMAND cannot be found\n\
  137  जर COMMAND (or timeout itself) is sent the KILL (9) संकेत (128+9)\n\
  -    the निकास status of COMMAND otherwise\n\
"), मानक_निकास);

      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

स्थिर bool
apply_time_suffix (द्विगुण *x, अक्षर suffix_char)
{
  पूर्णांक multiplier;

  निवड (suffix_char)
    {
    स्थिती 0:
    स्थिती 's':
      multiplier = 1;
      थांबा;
    स्थिती 'm':
      multiplier = 60;
      थांबा;
    स्थिती 'h':
      multiplier = 60 * 60;
      थांबा;
    स्थिती 'd':
      multiplier = 60 * 60 * 24;
      थांबा;
    पूर्वनिर्धारित:
      परत false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  परत true;
}

स्थिर द्विगुण
parse_duration (अक्षर स्थिरांक *str)
{
  अक्षर *ep;
  त्रुटि_सं = 0;
  द्विगुण duration = cl_strtod (str, &ep);
  द्विगुण s = dtimespec_bound (duration, त्रुटि_सं);

  जर (ep == str
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

  परत s;
}

स्थिर शून्य
unblock_signal (पूर्णांक sig)
{
  sigset_t unblock_set;
  sigemptyset (&unblock_set);
  sigaddset (&unblock_set, sig);
  जर (sigprocmask (SIG_UNBLOCK, &unblock_set, रिक्त) != 0)
    error (0, त्रुटि_सं, _("warning: sigprocmask"));
}

स्थिर शून्य
install_sigchld (शून्य)
{
  संरचना sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = chld;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  sigaction (SIGCHLD, &sa, रिक्त);

  /* We inherit the signal mask from our parent process,
     so ensure SIGCHLD is not blocked. */
  unblock_signal (SIGCHLD);
}

/* Filter out signals that were ignored.  */

स्थिर bool
sig_needs_handling (पूर्णांक sig, पूर्णांक sigterm)
{
  जर (sig == SIGALRM || sig == sigterm)
    परत true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  संरचना sigaction old_sa;
  sigaction (sig, रिक्त, &old_sa);
  bool ret = old_sa.sa_handler != संक_छोड़ो;
  परत ret;
}

स्थिर शून्य
install_cleanup (पूर्णांक sigterm)
{
  संरचना sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  साठी (पूर्णांक i = 0; i < countof (term_sig); i++)
    जर (sig_needs_handling (term_sig[i], sigterm))
      sigaction (term_sig[i], &sa, रिक्त);

  /* Real Time signals also terminate by default.  */
  साठी (पूर्णांक s = SIGRTMIN; s <= SIGRTMAX; s++)
    जर (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, रिक्त);

  sigaction (sigterm, &sa, रिक्त); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
स्थिर शून्य
block_cleanup_and_chld (पूर्णांक sigterm, sigset_t *old_set)
{
  sigset_t block_set;
  sigemptyset (&block_set);

  साठी (पूर्णांक i = 0; i < countof (term_sig); i++)
    जर (sig_needs_handling (term_sig[i], sigterm))
      sigaddset (&block_set, term_sig[i]);

  साठी (पूर्णांक s = SIGRTMIN; s <= SIGRTMAX; s++)
    जर (sig_needs_handling (s, sigterm))
      sigaddset (&block_set, s);

  sigaddset (&block_set, sigterm);

  sigaddset (&block_set, SIGCHLD);

  जर (sigprocmask (SIG_BLOCK, &block_set, old_set) != 0)
    error (0, त्रुटि_सं, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
स्थिर bool
disable_core_dumps (शून्य)
{
#जर HAVE_PRCTL && defined PR_SET_DUMPABLE
  जर (prctl (PR_SET_DUMPABLE, 0) == 0)
    परत true;

#किंवाजर HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  जर (setrlimit (RLIMIT_CORE, &(संरचना rlimit) {0}) == 0)
    परत true;

#अन्यथा
  परत false;
#endif

  error (0, त्रुटि_सं, _("warning: disabling core dumps failed"));
  परत false;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  द्विगुण timeout;
  पूर्णांक c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  निकास_पर (close_stdout);

  पर्यंत ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, रिक्त))
         != -1)
    {
      निवड (c)
        {
        स्थिती 'f':
          foreground = true;
          थांबा;

        स्थिती 'k':
          kill_after = parse_duration (optarg);
          थांबा;

        स्थिती 'p':
          preserve_status = true;
          थांबा;

        स्थिती 's':
          term_signal = operand2sig (optarg);
          जर (term_signal == -1)
            usage (EXIT_CANCELED);
          थांबा;

        स्थिती 'v':
          verbose = true;
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (EXIT_CANCELED);
          थांबा;
        }
    }

  जर (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  जर (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (term_signal);
  संकेत (SIGTTIN, संक_छोड़ो);   /* Don't stop if background child needs tty.  */
  संकेत (SIGTTOU, संक_छोड़ो);   /* Don't stop if background child needs tty.  */
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
  monitored_pid = विभाजन ();
  जर (monitored_pid == -1)
    {
      error (0, त्रुटि_सं, _("fork system call failed"));
      परत EXIT_CANCELED;
    }
  अन्यथा जर (monitored_pid == 0)  /* child */
    {
#जर HAVE_PRCTL && defined PR_SET_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_SET_PDEATHSIG, term_signal);
#endif
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      जर (getppid () != timeout_pid)
        परत EXIT_CANCELED;

      /* Restore signal mask for child.  */
      जर (sigprocmask (SIG_SETMASK, &orig_set, रिक्त) != 0)
        {
          error (0, त्रुटि_सं, _("child failed to reset signal mask"));
          परत EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      संकेत (SIGTTIN, संक_पूर्व);
      संकेत (SIGTTOU, संक_पूर्व);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      पूर्णांक exit_status = त्रुटि_सं == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, त्रुटि_सं, _("failed to run command %s"), quote (command));
      परत exit_status;
    }
  अन्यथा
    {
      pid_t wait_result;
      पूर्णांक status;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      पर्यंत ((wait_result = waitpid (monitored_pid, &status, WNOHANG)) == 0)
        संक_रोको (&orig_set);  /* Wait with cleanup signals unblocked.  */

      जर (wait_result < 0)
        {
          /* shouldn't happen.  */
          error (0, त्रुटि_सं, _("error waiting for command"));
          status = EXIT_CANCELED;
        }
      अन्यथा
        {
          जर (WIFEXITED (status))
            status = WEXITSTATUS (status);
          अन्यथा जर (WIFSIGNALED (status))
            {
              पूर्णांक sig = WTERMSIG (status);
              जर (WCOREDUMP (status))
                error (0, 0, _("the monitored command dumped core"));
              जर (!timed_out && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  संकेत (sig, संक_पूर्व);
                  unblock_signal (sig);
                  उठवा (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              जर (timed_out && sig == SIGKILL)
                preserve_status = true;
              status = sig + 128; /* what sh returns for signaled processes.  */
            }
          अन्यथा
            {
              /* shouldn't happen.  */
              error (0, 0, _("unknown status from command (%d)"), status);
              status = निकास_त्रुटि;
            }
        }

      जर (timed_out && !preserve_status)
        status = EXIT_TIMEDOUT;
      परत status;
    }
}
