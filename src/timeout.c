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
#यदि HAVE_PRCTL
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

#यदि HAVE_SETRLIMIT
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

स्थिर पूर्णाङ्क timed_out;
स्थिर पूर्णाङ्क term_signal = संक_इति;  /* same default as kill command.  */
स्थिर pid_t monitored_pid;
स्थिर द्विगुण kill_after;
स्थिर bool foreground;      /* whether to use another program group.  */
स्थिर bool preserve_status; /* whether to use a timeout status or not.  */
स्थिर bool verbose;         /* whether to diagnose timeouts or not.  */
स्थिर वर्ण ध्रुव *command;

स्थिर संरचना option ध्रुव long_options[] =
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

#यदि HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  संरचना timespec ts = dtotimespec (duration);
  संरचना itimerspec its = {.it_interval = {0}, .it_मूल्य = ts};
  timer_t timerid;
  यदि (timer_create (CLOCK_REALTIME, रिक्त, &timerid) == 0)
    {
      यदि (timer_settime (timerid, 0, &its, रिक्त) == 0)
        प्रत्यागम;
      अन्यथा
        {
          यदि (warn)
            error (0, त्रुटि_सं, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  अन्यथा यदि (warn && त्रुटि_सं != ENOSYS)
    error (0, त्रुटि_सं, _("warning: timer_create"));

#नोचेद्यदि HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  संरचना timeval tv;
  संरचना timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  यदि (tv.tv_usec == 1000 * 1000)
    {
      यदि (tv.tv_sec != TYPE_MAXIMUM (समय_प्रकार))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      अन्यथा
        tv.tv_usec--;
    }
  संरचना itimerval it = {.it_interval = {0}, .it_मूल्य = tv };
  यदि (setitimer (ITIMER_REAL, &it, रिक्त) == 0)
    प्रत्यागम;
  अन्यथा
    {
      यदि (warn && त्रुटि_सं != ENOSYS)
        error (0, त्रुटि_सं, _("warning: setitimer"));
    }
#endif

  /* fallback to single second resolution provided by alarm().  */

  अचिन्हित पूर्णाङ्क timeint;
  यदि (अच_पूर्णांक_उच्च <= duration)
    timeint = अच_पूर्णांक_उच्च;
  अन्यथा
    {
      अचिन्हित पूर्णाङ्क duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

स्थिर पूर्णाङ्क
send_sig (pid_t where, पूर्णाङ्क sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  यदि (where == 0)
    सङ्केत (sig, संक_छोड़ो);
  प्रत्यागम समाप्त (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
स्थिर शून्य
chld (MAYBE_UNUSED पूर्णाङ्क sig)
{
}

स्थिर शून्य
cleanup (पूर्णाङ्क sig)
{
  यदि (sig == SIGALRM)
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
  यदि (0 < monitored_pid)
    {
      यदि (kill_after)
        {
          पूर्णाङ्क saved_errno = त्रुटि_सं; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          term_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          त्रुटि_सं = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      यदि (verbose)
        {
          वर्ण signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (पूर्णाङ्क))];
          यदि (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, परिमाण signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      यदि (!foreground)
        {
          send_sig (0, sig);
          यदि (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  अन्यथा यदि (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  अन्यथा /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), program_name);

      ख_माला_दो (_("\
Start COMMAND, and समाप्त it यदि still running after DURATION.\n\
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
         also send a KILL सङ्केत यदि COMMAND is still running\n\
         this दीर्घ after the initial सङ्केत was sent\n\
"));
      oputs (_("\
  -p, --preserve-status\n\
         निकास with the same status as COMMAND,\n\
         even when the command बार out\n\
"));
      oputs (_("\
  -s, --सङ्केत=SIGNAL\n\
         specify the सङ्केत to be sent on timeout;\n\
         SIGNAL may be a name like 'HUP' or a संख्या;\n\
         see 'kill -l' प्रति a list of signals\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose to standard error any सङ्केत sent upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      ख_माला_दो (_("\n\
DURATION is a floating point संख्या with an optional suffix:\n\
's' प्रति seconds (the स्वभाव), 'm' प्रति minutes, 'h' प्रति hours or \
'd' प्रति days.\nA duration of 0 disables the associated timeout.\n"), मानक_निकास);

      ख_माला_दो (_("\n\
Upon timeout, send the TERM सङ्केत to COMMAND, यदि no other SIGNAL specified.\n\
The TERM सङ्केत kills any process that does not block or catch that सङ्केत.\n\
It may be necessary to use the KILL सङ्केत, since this सङ्केत can't be caught.\
\n"), मानक_निकास);

      ख_माला_दो (_("\n\
Exit status:\n\
  124  यदि COMMAND बार out, and --preserve-status is not specified\n\
  125  यदि the timeout command itself fails\n\
  126  यदि COMMAND is found but cannot be invoked\n\
  127  यदि COMMAND cannot be found\n\
  137  यदि COMMAND (or timeout itself) is sent the KILL (9) सङ्केत (128+9)\n\
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
apply_time_suffix (द्विगुण *x, वर्ण suffix_char)
{
  पूर्णाङ्क multiplier;

  विकल्प (suffix_char)
    {
    पक्ष 0:
    पक्ष 's':
      multiplier = 1;
      भङ्ग;
    पक्ष 'm':
      multiplier = 60;
      भङ्ग;
    पक्ष 'h':
      multiplier = 60 * 60;
      भङ्ग;
    पक्ष 'd':
      multiplier = 60 * 60 * 24;
      भङ्ग;
    स्वभाव:
      प्रत्यागम false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  प्रत्यागम true;
}

स्थिर द्विगुण
parse_duration (वर्ण ध्रुव *str)
{
  वर्ण *ep;
  त्रुटि_सं = 0;
  द्विगुण duration = cl_strtod (str, &ep);
  द्विगुण s = dtimespec_bound (duration, त्रुटि_सं);

  यदि (ep == str
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

  प्रत्यागम s;
}

स्थिर शून्य
unblock_signal (पूर्णाङ्क sig)
{
  sigset_t unblock_set;
  sigemptyset (&unblock_set);
  sigaddset (&unblock_set, sig);
  यदि (sigprocmask (SIG_UNBLOCK, &unblock_set, रिक्त) != 0)
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
sig_needs_handling (पूर्णाङ्क sig, पूर्णाङ्क sigterm)
{
  यदि (sig == SIGALRM || sig == sigterm)
    प्रत्यागम true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  संरचना sigaction old_sa;
  sigaction (sig, रिक्त, &old_sa);
  bool ret = old_sa.sa_handler != संक_छोड़ो;
  प्रत्यागम ret;
}

स्थिर शून्य
install_cleanup (पूर्णाङ्क sigterm)
{
  संरचना sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  प्रति (पूर्णाङ्क i = 0; i < countof (term_sig); i++)
    यदि (sig_needs_handling (term_sig[i], sigterm))
      sigaction (term_sig[i], &sa, रिक्त);

  /* Real Time signals also terminate by default.  */
  प्रति (पूर्णाङ्क s = SIGRTMIN; s <= SIGRTMAX; s++)
    यदि (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, रिक्त);

  sigaction (sigterm, &sa, रिक्त); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
स्थिर शून्य
block_cleanup_and_chld (पूर्णाङ्क sigterm, sigset_t *old_set)
{
  sigset_t block_set;
  sigemptyset (&block_set);

  प्रति (पूर्णाङ्क i = 0; i < countof (term_sig); i++)
    यदि (sig_needs_handling (term_sig[i], sigterm))
      sigaddset (&block_set, term_sig[i]);

  प्रति (पूर्णाङ्क s = SIGRTMIN; s <= SIGRTMAX; s++)
    यदि (sig_needs_handling (s, sigterm))
      sigaddset (&block_set, s);

  sigaddset (&block_set, sigterm);

  sigaddset (&block_set, SIGCHLD);

  यदि (sigprocmask (SIG_BLOCK, &block_set, old_set) != 0)
    error (0, त्रुटि_सं, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
स्थिर bool
disable_core_dumps (शून्य)
{
#यदि HAVE_PRCTL && defined PR_SET_DUMPABLE
  यदि (prctl (PR_SET_DUMPABLE, 0) == 0)
    प्रत्यागम true;

#नोचेद्यदि HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  यदि (setrlimit (RLIMIT_CORE, &(संरचना rlimit) {0}) == 0)
    प्रत्यागम true;

#अन्यथा
  प्रत्यागम false;
#endif

  error (0, त्रुटि_सं, _("warning: disabling core dumps failed"));
  प्रत्यागम false;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  द्विगुण timeout;
  पूर्णाङ्क c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  निकास_पर (close_stdout);

  यावत् ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, रिक्त))
         != -1)
    {
      विकल्प (c)
        {
        पक्ष 'f':
          foreground = true;
          भङ्ग;

        पक्ष 'k':
          kill_after = parse_duration (optarg);
          भङ्ग;

        पक्ष 'p':
          preserve_status = true;
          भङ्ग;

        पक्ष 's':
          term_signal = operand2sig (optarg);
          यदि (term_signal == -1)
            usage (EXIT_CANCELED);
          भङ्ग;

        पक्ष 'v':
          verbose = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_CANCELED);
          भङ्ग;
        }
    }

  यदि (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  यदि (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (term_signal);
  सङ्केत (SIGTTIN, संक_छोड़ो);   /* Don't stop if background child needs tty.  */
  सङ्केत (SIGTTOU, संक_छोड़ो);   /* Don't stop if background child needs tty.  */
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
  यदि (monitored_pid == -1)
    {
      error (0, त्रुटि_सं, _("fork system call failed"));
      प्रत्यागम EXIT_CANCELED;
    }
  अन्यथा यदि (monitored_pid == 0)  /* child */
    {
#यदि HAVE_PRCTL && defined PR_SET_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_SET_PDEATHSIG, term_signal);
#endif
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      यदि (getppid () != timeout_pid)
        प्रत्यागम EXIT_CANCELED;

      /* Restore signal mask for child.  */
      यदि (sigprocmask (SIG_SETMASK, &orig_set, रिक्त) != 0)
        {
          error (0, त्रुटि_सं, _("child failed to reset signal mask"));
          प्रत्यागम EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      सङ्केत (SIGTTIN, संक_पूर्व);
      सङ्केत (SIGTTOU, संक_पूर्व);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      पूर्णाङ्क exit_status = त्रुटि_सं == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, त्रुटि_सं, _("failed to run command %s"), quote (command));
      प्रत्यागम exit_status;
    }
  अन्यथा
    {
      pid_t wait_result;
      पूर्णाङ्क status;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      यावत् ((wait_result = waitpid (monitored_pid, &status, WNOHANG)) == 0)
        संक_रोको (&orig_set);  /* Wait with cleanup signals unblocked.  */

      यदि (wait_result < 0)
        {
          /* shouldn't happen.  */
          error (0, त्रुटि_सं, _("error waiting for command"));
          status = EXIT_CANCELED;
        }
      अन्यथा
        {
          यदि (WIFEXITED (status))
            status = WEXITSTATUS (status);
          अन्यथा यदि (WIFSIGNALED (status))
            {
              पूर्णाङ्क sig = WTERMSIG (status);
              यदि (WCOREDUMP (status))
                error (0, 0, _("the monitored command dumped core"));
              यदि (!timed_out && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  सङ्केत (sig, संक_पूर्व);
                  unblock_signal (sig);
                  उत्थापय (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              यदि (timed_out && sig == SIGKILL)
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

      यदि (timed_out && !preserve_status)
        status = EXIT_TIMEDOUT;
      प्रत्यागम status;
    }
}
