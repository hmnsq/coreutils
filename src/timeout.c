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

#شامل <config.h>
#شامل <getopt.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <signal.h>
#اگر HAVE_PRCTL
# include <sys/prctl.h>
#اگر_ختم
#شامل <sys/wait.h>

#شامل "system.h"
#شامل "cl-strtod.h"
#شامل "dtimespec-bound.h"
#شامل "sig2str.h"
#شامل "operand2sig.h"
#شامل "term-sig.h"
#شامل "quote.h"

#اگر HAVE_SETRLIMIT
/* FreeBSD 5.0 at least needs <sys/types.h> and <sys/time.h> included
   before <sys/resource.h>.  Currently "system.h" includes <sys/time.h>.  */
# include <sys/resource.h>
#اگر_ختم

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt.  */
#اگر_غیر_تعریف_شدہ SA_RESTART
# define SA_RESTART 0
#اگر_ختم

#اگر_غیر_تعریف_شدہ SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#اگر_ختم
#اگر_غیر_تعریف_شدہ SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#اگر_ختم

#تعریف_کرو پروگرام_NAME "timeout"

#تعریف_کرو AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ساکن صحیح timed_برآمد;
ساکن صحیح رکن_signal = SIGTERM;  /* same default as kill command.  */
ساکن pid_t monitored_pid;
ساکن دوہرا kill_after;
ساکن bool foreground;      /* whether to use another program group.  */
ساکن bool preserve_حالت; /* whether to use a timeout status or not.  */
ساکن bool verbose;         /* whether to diagnose timeouts or not.  */
ساکن حرف ثابت *command;

ساکن ساخت option ثابت long_options[] =
{
  {"foreground", no_دلیل, تہی, 'f'},
  {"kill-after", required_دلیل, تہی, 'k'},
  {"preserve-status", no_دلیل, تہی, 'p'},
  {"signal", required_دلیل, تہی, 's'},
  {"verbose", no_دلیل, تہی, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Start the timeout after which we'll receive a SIGALRM.
   Round DURATION up to the next representable value.
   Treat out-of-range values as if they were maximal,
   as that's more useful in practice than reporting an error.
   '0' means don't timeout.  */
ساکن خالی
settimeout (دوہرا duration, bool warn)
{

#اگر HAVE_TIMER_SETTIME
  /* timer_settime() provides potentially nanosecond resolution.  */

  ساخت timespec ts = dtotimespec (duration);
  ساخت itimerspec its = {.it_interval = {0}, .it_قدر = ts};
  timer_t timerid;
  اگر (timer_create (CLOCK_REALTIME, تہی, &timerid) == 0)
    {
      اگر (timer_settime (timerid, 0, &its, تہی) == 0)
        واپس;
      ورنہ
        {
          اگر (warn)
            error (0, errno, _("warning: timer_settime"));
          timer_delete (timerid);
        }
    }
  ورنہ اگر (warn && errno != ENOSYS)
    error (0, errno, _("warning: timer_create"));

#وگرنہ_اگر HAVE_SETITIMER
  /* setitimer() is more portable (to Darwin for example),
     but only provides microsecond resolution.  */

  ساخت timeval tv;
  ساخت timespec ts = dtotimespec (duration);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = (ts.tv_nsec + 999) / 1000;
  اگر (tv.tv_usec == 1000 * 1000)
    {
      اگر (tv.tv_sec != TYPE_زیادہ_سے_زیادہ (time_t))
        {
          tv.tv_sec++;
          tv.tv_usec = 0;
        }
      ورنہ
        tv.tv_usec--;
    }
  ساخت itimerval it = {.it_interval = {0}, .it_قدر = tv };
  اگر (setitimer (ITIMER_REAL, &it, تہی) == 0)
    واپس;
  ورنہ
    {
      اگر (warn && errno != ENOSYS)
        error (0, errno, _("warning: setitimer"));
    }
#اگر_ختم

  /* fallback to single second resolution provided by alarm().  */

  غیرعلامتی صحیح timeint;
  اگر (UINT_MAX <= duration)
    timeint = UINT_MAX;
  ورنہ
    {
      غیرعلامتی صحیح duration_floor = duration;
      timeint = duration_floor + (duration_floor < duration);
    }
  alarm (timeint);
}

/* send SIG avoiding the current process.  */

ساکن صحیح
send_sig (pid_t where, صحیح sig)
{
  /* If sending to the group, then ignore the signal,
     so we don't go into a signal loop.  Note that this will ignore any of the
     signals registered in install_cleanup(), that are sent after we
     propagate the first one, which hopefully won't be an issue.  Note this
     process can be implicitly multithreaded due to some timer_settime()
     implementations, therefore a signal sent to the group, can be sent
     multiple times to this process.  */
  اگر (where == 0)
    اشارہ (sig, SIG_IGN);
  واپس kill (where, sig);
}

/* Signal handler which is required for sigsuspend() to be interrupted
   whenever SIGCHLD is received.  */
ساکن خالی
chld (MAYBE_غیر_مستعمل صحیح sig)
{
}

ساکن خالی
cleanup (صحیح sig)
{
  اگر (sig == SIGALRM)
    {
      timed_برآمد = 1;
      /* In case there is an issue with close_stdout,
         update to a more accurate default exit status.
         For example we might get failed writes with -v with:
           timeout -v 1 sleep 10 2>&1 | :
      */
      initialize_exit_failure (EXIT_TIMEDOUT);
      sig = رکن_signal;
    }
  اگر (0 < monitored_pid)
    {
      اگر (kill_after)
        {
          صحیح saved_errno = errno; /* settimeout may reset.  */
          /* Start a new timeout after which we'll send SIGKILL.  */
          رکن_signal = SIGKILL;
          settimeout (kill_after, false);
          kill_after = 0; /* Don't let later signals reset kill alarm.  */
          errno = saved_errno;
        }

      /* Send the signal directly to the monitored child,
         in case it has itself become group leader,
         or is not running in a separate group.  */
      اگر (verbose)
        {
          حرف signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (صحیح))];
          اگر (sig == 0 || sig2str (sig, signame) != 0)
            snprintf (signame, حجم signame, "%d", sig);
          error (0, 0, _("sending signal %s to command %s"),
                 signame, quote (command));
        }
      send_sig (monitored_pid, sig);

      /* The normal case is the job has remained in our
         newly created process group, so send to all processes in that.  */
      اگر (!foreground)
        {
          send_sig (0, sig);
          اگر (sig != SIGKILL && sig != SIGCONT)
            {
              send_sig (monitored_pid, SIGCONT);
              send_sig (0, SIGCONT);
            }
        }
    }
  ورنہ اگر (monitored_pid == -1)
    { /* were in the parent, so let it continue to exit below.  */
    }
  ورنہ /* monitored_pid == 0  */
    { /* parent hasn't forked yet, or child has not exec'd yet.  */
      _exit (128 + sig);
    }
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... DURATION COMMAND [ARG]...\n"), پروگرام_name);

      fputs (_("\
آغاز_2 COMMAND, and kill it اگر still running after DURATION.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --foreground\n\
         when not running timeout directly from a shell prompt,\n\
         allow COMMAND مقصد اقرأ from the TTY and get TTY اشارات;\n\
         اندر this منوال_2, children کا COMMAND will not be timed برآمد\n\
"));
      oputs (_("\
  -k, --kill-after=DURATION\n\
         also send a KILL اشارہ اگر COMMAND is still running\n\
         this طویل after the initial اشارہ was بھیجا\n\
"));
      oputs (_("\
  -p, --preserve-حالت_2\n\
         باہر_نکلو with the same حالت_2 as COMMAND,\n\
         even when the command times برآمد\n\
"));
      oputs (_("\
  -s, --اشارہ=SIGNAL\n\
         specify the اشارہ مقصد be بھیجا on timeout;\n\
         SIGNAL may be a name like 'HUP' or a نمبر;\n\
         see 'kill -l' برائے a list کا اشارات\n\
"));
      oputs (_("\
  -v, --verbose\n\
         diagnose مقصد standard error any اشارہ بھیجا upon timeout\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);

      fputs (_("\n\
DURATION is a floating نقطہ نمبر with an optional suffix:\n\
's' برائے seconds (the طےشدہ), 'm' برائے minutes, 'h' برائے hours or \
'd' برائے days.\nA duration کا 0 disables the associated timeout.\n"), stdout);

      fputs (_("\n\
Upon timeout, send the رکن_2 اشارہ مقصد COMMAND, اگر no other SIGNAL specified.\n\
The رکن_2 اشارہ kills any process that does not بلاک or catch that اشارہ.\n\
It may be necessary مقصد use the KILL اشارہ, since this اشارہ can't be caught.\
\n"), stdout);

      fputs (_("\n\
Exit حالت_2:\n\
  124  اگر COMMAND times برآمد, and --preserve-حالت_2 is not specified\n\
  125  اگر the timeout command itself fails\n\
  126  اگر COMMAND is ملا but cannot be invoked\n\
  127  اگر COMMAND cannot be ملا\n\
  137  اگر COMMAND (or timeout itself) is بھیجا the KILL (9) اشارہ (128+9)\n\
  -    the باہر_نکلو حالت_2 کا COMMAND otherwise\n\
"), stdout);

      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

ساکن bool
apply_وقت_suffix (دوہرا *x, حرف suffix_char)
{
  صحیح multiplier;

  انتخاب (suffix_char)
    {
    حالت 0:
    حالت 's':
      multiplier = 1;
      توڑو;
    حالت 'm':
      multiplier = 60;
      توڑو;
    حالت 'h':
      multiplier = 60 * 60;
      توڑو;
    حالت 'd':
      multiplier = 60 * 60 * 24;
      توڑو;
    طےشدہ:
      واپس false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  واپس true;
}

ساکن دوہرا
parse_duration (حرف ثابت *str)
{
  حرف *ep;
  errno = 0;
  دوہرا duration = cl_strtod (str, &ep);
  دوہرا s = dtimespec_bound (duration, errno);

  اگر (ep == str
      /* Nonnegative interval.  */
      || ! (0 <= s)
      /* No extra chars after the number and an optional s,m,h,d char.  */
      || (*ep && *(ep + 1))
      /* Check any suffix char and update timeout based on the suffix.  */
      || !apply_وقت_suffix (&s, *ep))
    {
      error (0, 0, _("invalid time interval %s"), quote (str));
      usage (EXIT_CANCELED);
    }

  واپس s;
}

ساکن خالی
unblock_signal (صحیح sig)
{
  sigset_t unblock_سیٹ;
  sigemptyset (&unblock_سیٹ);
  sigaddset (&unblock_سیٹ, sig);
  اگر (sigprocmask (SIG_UNBLOCK, &unblock_سیٹ, تہی) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

ساکن خالی
install_sigchld (خالی)
{
  ساخت sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = chld;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  sigaction (SIGCHLD, &sa, تہی);

  /* We inherit the signal mask from our parent process,
     so ensure SIGCHLD is not blocked. */
  unblock_signal (SIGCHLD);
}

/* Filter out signals that were ignored.  */

ساکن bool
sig_needs_handling (صحیح sig, صحیح sigterm)
{
  اگر (sig == SIGALRM || sig == sigterm)
    واپس true;  /* We can't ignore these.  */

  /* Note background jobs in shells have SIGINT and SIGQUIT
     set to SIG_IGN by default.  I.e., those signals will
     not be propagated through background timeout jobs.  */
  ساخت sigaction old_sa;
  sigaction (sig, تہی, &old_sa);
  bool ret = old_sa.sa_handler != SIG_IGN;
  واپس ret;
}

ساکن خالی
install_cleanup (صحیح sigterm)
{
  ساخت sigaction sa;
  sigemptyset (&sa.sa_mask);  /* Allow concurrent calls to handler */
  sa.sa_handler = cleanup;
  sa.sa_flags = SA_RESTART;   /* Restart syscalls if possible, as that's
                                 more likely to work cleanly.  */

  برائے (صحیح i = 0; i < countof (رکن_sig); i++)
    اگر (sig_needs_handling (رکن_sig[i], sigterm))
      sigaction (رکن_sig[i], &sa, تہی);

  /* Real Time signals also terminate by default.  */
  برائے (صحیح s = SIGRTMIN; s <= SIGRTMAX; s++)
    اگر (sig_needs_handling (s, sigterm))
      sigaction (s, &sa, تہی);

  sigaction (sigterm, &sa, تہی); /* user specified termination signal.  */
}

/* Block all signals which were registered with cleanup() as the signal
   handler, so we never kill processes after waitpid() returns.
   Also block SIGCHLD to ensure it doesn't fire between
   waitpid() polling and sigsuspend() waiting for a signal.
   Return original mask in OLD_SET.  */
ساکن خالی
بلاک_cleanup_and_chld (صحیح sigterm, sigset_t *old_سیٹ)
{
  sigset_t بلاک_سیٹ;
  sigemptyset (&بلاک_سیٹ);

  برائے (صحیح i = 0; i < countof (رکن_sig); i++)
    اگر (sig_needs_handling (رکن_sig[i], sigterm))
      sigaddset (&بلاک_سیٹ, رکن_sig[i]);

  برائے (صحیح s = SIGRTMIN; s <= SIGRTMAX; s++)
    اگر (sig_needs_handling (s, sigterm))
      sigaddset (&بلاک_سیٹ, s);

  sigaddset (&بلاک_سیٹ, sigterm);

  sigaddset (&بلاک_سیٹ, SIGCHLD);

  اگر (sigprocmask (SIG_BLOCK, &بلاک_سیٹ, old_سیٹ) != 0)
    error (0, errno, _("warning: sigprocmask"));
}

/* Try to disable core dumps for this process.
   Return TRUE if successful, FALSE otherwise.  */
ساکن bool
disable_core_dumps (خالی)
{
#اگر HAVE_PRCTL && defined PR_سیٹ_DUMPABLE
  اگر (prctl (PR_سیٹ_DUMPABLE, 0) == 0)
    واپس true;

#وگرنہ_اگر HAVE_SETRLIMIT && defined RLIMIT_CORE
  /* Note this doesn't disable processing by a filter in
     /proc/sys/kernel/core_pattern on Linux.  */
  اگر (setrlimit (RLIMIT_CORE, &(ساخت rlimit) {0}) == 0)
    واپس true;

#ورنہ
  واپس false;
#اگر_ختم

  error (0, errno, _("warning: disabling core dumps failed"));
  واپس false;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  دوہرا timeout;
  صحیح c;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  جب ((c = getopt_long (argc, argv, "+fk:ps:v", long_options, تہی))
         != -1)
    {
      انتخاب (c)
        {
        حالت 'f':
          foreground = true;
          توڑو;

        حالت 'k':
          kill_after = parse_duration (optarg);
          توڑو;

        حالت 'p':
          preserve_حالت = true;
          توڑو;

        حالت 's':
          رکن_signal = operand2sig (optarg);
          اگر (رکن_signal == -1)
            usage (EXIT_CANCELED);
          توڑو;

        حالت 'v':
          verbose = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_CANCELED);
          توڑو;
        }
    }

  اگر (argc - optind < 2)
    usage (EXIT_CANCELED);

  timeout = parse_duration (argv[optind++]);

  argv += optind;
  command = argv[0];

  /* Ensure we're in our own group so all subprocesses can be killed.
     Note we don't just put the child in a separate group as
     then we would need to worry about foreground and background groups
     and propagating signals between them.  */
  اگر (!foreground)
    setpgid (0, 0);

  /* Setup handlers before fork() so that we
     handle any signals caused by child, without races.  */
  install_cleanup (رکن_signal);
  اشارہ (SIGTTIN, SIG_IGN);   /* Don't stop if background child needs tty.  */
  اشارہ (SIGTTOU, SIG_IGN);   /* Don't stop if background child needs tty.  */
  install_sigchld ();          /* Interrupt sigsuspend() when child exits.   */

  /* We configure timers so that SIGALRM is sent on expiry.
     Therefore ensure we don't inherit a mask blocking SIGALRM.  */
  unblock_signal (SIGALRM);

  /* Block signals now, so monitored_pid is deterministic in cleanup().  */
  sigset_t orig_سیٹ;
  بلاک_cleanup_and_chld (رکن_signal, &orig_سیٹ);

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
  اگر (monitored_pid == -1)
    {
      error (0, errno, _("fork system call failed"));
      واپس EXIT_CANCELED;
    }
  ورنہ اگر (monitored_pid == 0)  /*بچہ*/
    {
#اگر HAVE_PRCTL && defined PR_سیٹ_PDEATHSIG
      /* Add protection if the parent dies without signaling child.  */
      prctl (PR_سیٹ_PDEATHSIG, رکن_signal);
#اگر_ختم
      /* If we're already reparented to init, don't proceed.  Be aware that
         'timeout' may actually be started by the init process.  E.g., when
         the shell is the entrypoint to a container.  */
      اگر (getppid () != timeout_pid)
        واپس EXIT_CANCELED;

      /* Restore signal mask for child.  */
      اگر (sigprocmask (SIG_SETMASK, &orig_سیٹ, تہی) != 0)
        {
          error (0, errno, _("child failed to reset signal mask"));
          واپس EXIT_CANCELED;
        }

      /* exec doesn't reset SIG_IGN -> SIG_DFL.  */
      اشارہ (SIGTTIN, SIG_DFL);
      اشارہ (SIGTTOU, SIG_DFL);

      execvp (argv[0], argv);

      /* exit like sh, env, nohup, ...  */
      صحیح exit_حالت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
      error (0, errno, _("failed to run command %s"), quote (command));
      واپس exit_حالت;
    }
  ورنہ
    {
      pid_t wait_نتیجہ;
      صحیح حالت_2;

      settimeout (timeout, true);

      /* Note signals remain blocked in parent here, to ensure
         we don't cleanup() after waitpid() reaps the child,
         to avoid sending signals to a possibly different process.  */

      جب ((wait_نتیجہ = waitpid (monitored_pid, &حالت_2, WNOHANG)) == 0)
        sigsuspend (&orig_سیٹ);  /* Wait with cleanup signals unblocked.  */

      اگر (wait_نتیجہ < 0)
        {
          /* shouldn't happen.  */
          error (0, errno, _("error waiting for command"));
          حالت_2 = EXIT_CANCELED;
        }
      ورنہ
        {
          اگر (WIFEXITED (حالت_2))
            حالت_2 = WEXITSTATUS (حالت_2);
          ورنہ اگر (WIFSIGNALED (حالت_2))
            {
              صحیح sig = WTERMSIG (حالت_2);
              اگر (WCOREDUMP (حالت_2))
                error (0, 0, _("the monitored command dumped core"));
              اگر (!timed_برآمد && disable_core_dumps ())
                {
                  /* exit with the signal flag set.  */
                  اشارہ (sig, SIG_DFL);
                  unblock_signal (sig);
                  اٹھاؤ (sig);
                }
              /* Allow users to distinguish if command was forcibly killed.
                 Needed with --foreground where we don't send SIGKILL to
                 the timeout process itself.  */
              اگر (timed_برآمد && sig == SIGKILL)
                preserve_حالت = true;
              حالت_2 = sig + 128; /* what sh returns for signaled processes.  */
            }
          ورنہ
            {
              /* shouldn't happen.  */
              error (0, 0, _("unknown status from command (%d)"), حالت_2);
              حالت_2 = EXIT_FAILURE;
            }
        }

      اگر (timed_برآمد && !preserve_حالت)
        حالت_2 = EXIT_TIMEDOUT;
      واپس حالت_2;
    }
}
