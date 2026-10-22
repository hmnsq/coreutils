/* GNU's who.
   Copyright (C) 1992-2026 Free Software Foundation, Inc.

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

/* Written by jla; revised by djm; revised again by mstone */

/* Output format:
   name [state] line time [activity] [pid] [comment] [exit]
   state: -T
   name, line, time: not -q
   idle: -u
*/

#شامل <config.h>
#شامل <getopt.h>
#شامل <stdio.h>

#شامل <sys/types.h>
#شامل "system.h"

#شامل "c-ctype.h"
#شامل "canon-host.h"
#شامل "readutmp.h"
#شامل "hard-locale.h"
#شامل "quote.h"
#شامل "xvasprintf.h"

#ifdef TTY_GROUP_NAME
# include <grp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "who"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Michael Stone")

#ifdef RUN_LVL
# define UT_TYPE_RUN_LVL(U) ((U)->ut_type == RUN_LVL)
#وگرنه
# define UT_TYPE_RUN_LVL(U) false
#endif

#ifdef INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#وگرنه
# define UT_TYPE_INIT_PROCESS(U) false
#endif

#ifdef DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#وگرنه
# define UT_TYPE_DEAD_PROCESS(U) false
#endif

#ifdef NEW_TIME
# define UT_TYPE_NEW_TIME(U) ((U)->ut_type == NEW_TIME)
#وگرنه
# define UT_TYPE_NEW_TIME(U) false
#endif

#define IDLESTR_LEN 6

#اگر HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  نویسه Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (long صحیح) (Utmp_ent->ut_pid))
#وگرنه
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  نویسه ثابت *Var = ""
#endif

#اگر HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#وگرنه
# define UT_ID(U) "??"
#endif

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ایستا bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
ایستا bool short_list;

/* If true, display only name, line, and time fields.  */
ایستا bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
ایستا bool include_idle;

/* If true, display a line at the top describing each field.  */
ایستا bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
ایستا bool include_mesg;

/* If true, display process termination & exit status.  */
ایستا bool include_exit;

/* If true, display the last boot time.  */
ایستا bool need_boottime;

/* If true, display dead processes.  */
ایستا bool need_deadprocs;

/* If true, display processes waiting for user login.  */
ایستا bool need_login;

/* If true, display processes started by init.  */
ایستا bool need_initspawn;

/* If true, display the last clock change.  */
ایستا bool need_clockchange;

/* If true, display the current runlevel.  */
ایستا bool need_runlevel;

/* If true, display user processes.  */
ایستا bool need_users;

/* If true, display info only for the controlling tty.  */
ایستا bool my_سطر_only;

/* The strftime format to use for login times, and its expected
   output width.  */
ایستا نویسه ثابت *time_format;
ایستا صحیح time_format_width;

/* for long options with no corresponding short option, use enum */
شمارش
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ایستا ساختار option ثابت longopts[] =
{
  {"all", no_argument, پوچ, 'a'},
  {"boot", no_argument, پوچ, 'b'},
  {"count", no_argument, پوچ, 'q'},
  {"dead", no_argument, پوچ, 'd'},
  {"heading", no_argument, پوچ, 'H'},
  {"login", no_argument, پوچ, 'l'},
  {"lookup", no_argument, پوچ, LOOKUP_OPTION},
  {"message", no_argument, پوچ, 'T'},
  {"mesg", no_argument, پوچ, 'T'},
  {"process", no_argument, پوچ, 'p'},
  {"runlevel", no_argument, پوچ, 'r'},
  {"short", no_argument, پوچ, 's'},
  {"time", no_argument, پوچ, 't'},
  {"users", no_argument, پوچ, 'u'},
  {"writable", no_argument, پوچ, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
ایستا نویسه ثابت *
idle_string (time_t when, time_t boottime)
{
  ایستا time_t now = TYPE_MINIMUM (time_t);

  اگر (now == TYPE_MINIMUM (time_t))
    time (&now);

  صحیح seconds_idle;
  اگر (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      اگر (seconds_idle < 60)
        بازگشت "  .  ";
      وگرنه
        {
          ایستا نویسه idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          بازگشت idle_hhmm;
        }
    }

  بازگشت _(" old ");
}

/* Return a time string.  */
ایستا نویسه ثابت *
time_string (STRUCT_UTMP ثابت *utmp_ent)
{
  ایستا نویسه buf[INT_STRLEN_BOUND (intmax_t) + اندازه "-%m-%d %H:%M"];
  ساختار tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  اگر (tmp)
    {
      strftime (buf, اندازه buf, time_format, tmp);
      بازگشت buf;
    }
  وگرنه
    بازگشت timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
ایستا تهی
print_سطر (نویسه ثابت *user, ثابت نویسه state,
            نویسه ثابت *سطر,
            نویسه ثابت *time_str, نویسه ثابت *idle, نویسه ثابت *pid,
            نویسه ثابت *comment, نویسه ثابت *exitstr)
{
  ایستا نویسه mesg[3] = { ' ', 'x', '\0' };
  نویسه *buf;
  نویسه x_idle[1 + IDLESTR_LEN + 1];
  نویسه x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  نویسه *x_exitstr;

  mesg[1] = state;

  اگر (include_idle && !short_output && strlen (idle) < اندازه x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  وگرنه
    *x_idle = '\0';

  اگر (!short_output && strlen (pid) < اندازه x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  وگرنه
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, strlen (exitstr)) + 1 : 1);
  اگر (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  وگرنه
    *x_exitstr = '\0';

  buf = xasprintf ("%-8s"
                   "%s"
                   " %-12s"
                   " %-*s"
                   "%s"
                   "%s"
                   " %-8s"
                   "%s"
                   ,
                   user ? user : "   .",
                   include_mesg ? mesg : "",
                   سطر,
                   time_format_width,
                   time_str,
                   x_idle,
                   x_pid,
                   /* FIXME: it's not really clear whether the following
                      field should be in the short_output.  A strict reading
                      of SUSv2 would suggest not, but I haven't seen any
                      implementations that actually work that way... */
                   comment,
                   x_exitstr
                  );

  {
    /* Remove any trailing spaces.  */
    نویسه *p = buf + strlen (buf);
    وقتی (*--p == ' ')
      /* empty */;
    *(p + 1) = '\0';
  }

  puts (buf);
  آزاد (buf);
  آزاد (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
ایستا bool
is_tty_writable (ساختار stat ثابت *pstat)
{
#ifdef TTY_GROUP_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  ساختار group *ttygr = getgrnam (TTY_GROUP_NAME);
  اگر (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    بازگشت false;
#endif

  بازگشت pstat->st_mode & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
ایستا تهی
print_user (STRUCT_UTMP ثابت *utmp_ent, time_t boottime)
{
  ساختار stat stats;
  time_t last_change;
  نویسه mesg;
  نویسه idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  ایستا نویسه *hoststr;
#اگر HAVE_STRUCT_XTMP_UT_HOST
  ایستا idx_t hostlen;
#endif

  /* If ut_line contains a space, the device name starts after the space.  */
  نویسه *سطر = utmp_ent->ut_سطر;
  نویسه *space = strchr (سطر, ' ');
  سطر = space ? space + 1 : سطر;

  صحیح dirfd;
  اگر (IS_ABSOLUTE_FILE_NAME (سطر))
    dirfd = AT_FDCWD;
  وگرنه
    {
      ایستا صحیح dev_dirfd;
      اگر (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          اگر (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  اگر (AT_FDCWD <= dirfd && fstatat (dirfd, سطر, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      last_change = stats.st_atime;
    }
  وگرنه
    {
      mesg = '?';
      last_change = 0;
    }

  اگر (last_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (last_change, boottime));
  وگرنه
    sprintf (idlestr, "  ?");

#اگر HAVE_STRUCT_XTMP_UT_HOST
  اگر (utmp_ent->ut_host[0])
    {
      نویسه *host = پوچ;
      نویسه *display = پوچ;
      نویسه *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      اگر (display)
        *display++ = '\0';

      اگر (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      اگر (! host)
        host = ut_host;

      اگر (display)
        {
          idx_t needed = strlen (host) + strlen (display) + 4;
          اگر (hostlen < needed)
            {
              آزاد (hoststr);
              hoststr = xpalloc (پوچ, &hostlen, needed - hostlen, -1, 1);
            }
          نویسه *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      وگرنه
        {
          idx_t needed = strlen (host) + 3;
          اگر (hostlen < needed)
            {
              آزاد (hoststr);
              hoststr = xpalloc (پوچ, &hostlen, needed - hostlen, -1, 1);
            }
          نویسه *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      اگر (host != ut_host)
        آزاد (host);
    }
  وگرنه
    {
      اگر (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#endif

  print_سطر (utmp_ent->ut_user, mesg,
              utmp_ent->ut_سطر,
              time_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

ایستا تهی
print_boottime (STRUCT_UTMP ثابت *utmp_ent)
{
  print_سطر ("", ' ', _("system boot"),
              time_string (utmp_ent), "", "", "", "");
}

ایستا نویسه *
make_id_equals_comment (STRUCT_UTMP ثابت *utmp_ent)
{
  نویسه ثابت *id = UT_ID (utmp_ent);
  idx_t idlen = strlen (id);
  نویسه ثابت *prefix = _("id=");
  idx_t prefixlen = strlen (prefix);
  نویسه *comment = xmalloc (prefixlen + idlen + 1);
  نویسه *p = mempcpy (comment, prefix, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  بازگشت comment;
}

ایستا تهی
print_deadprocs (STRUCT_UTMP ثابت *utmp_ent)
{
  ایستا نویسه *exitstr;
  نویسه *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  اگر (!exitstr)
    exitstr = xmalloc (strlen (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + strlen (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_سطر ("", ' ', utmp_ent->ut_سطر,
              time_string (utmp_ent), "", pidstr, comment, exitstr);
  آزاد (comment);
}

ایستا تهی
print_login (STRUCT_UTMP ثابت *utmp_ent)
{
  نویسه *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_سطر (_("LOGIN"), ' ', utmp_ent->ut_سطر,
              time_string (utmp_ent), "", pidstr, comment, "");
  آزاد (comment);
}

ایستا تهی
print_initspawn (STRUCT_UTMP ثابت *utmp_ent)
{
  نویسه *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_سطر ("", ' ', utmp_ent->ut_سطر,
              time_string (utmp_ent), "", pidstr, comment, "");
  آزاد (comment);
}

ایستا تهی
print_clockchange (STRUCT_UTMP ثابت *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_سطر ("", ' ', _("clock change"),
              time_string (utmp_ent), "", "", "", "");
}

ایستا تهی
print_runlevel (STRUCT_UTMP ثابت *utmp_ent)
{
  ایستا نویسه *runlevline, *comment;
  unsigned نویسه last = utmp_ent->ut_pid / 256;
  unsigned نویسه curr = utmp_ent->ut_pid % 256;

  اگر (!runlevline)
    runlevline = xmalloc (strlen (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  اگر (!comment)
    comment = xmalloc (strlen (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (last == 'N') ? 'S' : last);

  print_سطر ("", ' ', runlevline, time_string (utmp_ent),
              "", "", c_isprint (last) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
ایستا تهی
list_entries_who (idx_t n, STRUCT_UTMP ثابت *utmp_buf)
{
  idx_t entries = 0;

  وقتی (n--)
    {
      اگر (IS_USER_PROCESS (utmp_buf))
        {
          نویسه *trimmed_name;

          trimmed_name = extract_trimmed_name (utmp_buf);

          اگر (entries)
            putchar (' ');
          fputs (trimmed_name, stdout);
          آزاد (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  printf (_("\n# users=%td\n"), entries);
}

ایستا تهی
print_heading (تهی)
{
  print_سطر (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
ایستا تهی
scan_entries (idx_t n, STRUCT_UTMP ثابت *utmp_buf)
{
  نویسه *ttyname_b IF_LINT ( = پوچ);
  time_t boottime = TYPE_MINIMUM (time_t);

  اگر (include_heading)
    print_heading ();

  اگر (my_سطر_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      اگر (!ttyname_b)
        بازگشت;
      اگر (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += اندازه "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  وقتی (n--)
    {
      اگر (!my_سطر_only
          || str_endswith (utmp_buf->ut_سطر, ttyname_b))
        {
          اگر (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          وگرنه اگر (need_runlevel && UT_TYPE_RUN_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          وگرنه اگر (need_boottime && UT_TYPE_BOOT_TIME (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          وگرنه اگر (need_clockchange && UT_TYPE_NEW_TIME (utmp_buf))
            print_clockchange (utmp_buf);
          وگرنه اگر (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          وگرنه اگر (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          وگرنه اگر (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      اگر (UT_TYPE_BOOT_TIME (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
ایستا تهی
who (نویسه ثابت *filename, صحیح options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  اگر (short_list)
    options |= READ_UTMP_USER_PROCESS;
  اگر (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  اگر (short_list)
    list_entries_who (n_users, utmp_buf);
  وگرنه
    scan_entries (n_users, utmp_buf);

  آزاد (utmp_buf);
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("Usage: %s [OPTION]... [ FILE | ARG1 ARG2 ]\n"), program_name);
      fputs (_("\
Print information about users who are currently logged in.\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -a, --all         same as -b -d --login -p -r -t -T -u\n\
"));
      oputs (_("\
  -b, --boot        time of last system boot\n\
"));
      oputs (_("\
  -d, --dead        print dead processes\n\
"));
      oputs (_("\
  -H, --heading     print سطر of column headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt to canonicalize hostnames via DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard درون_داد\n\
"));
      oputs (_("\
  -p, --process     print active processes spawned by init\n\
"));
      oputs (_("\
  -q, --شمار       all login names and عدد of users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --short       print only name, سطر, and time (پیشفرض)\n\
"));
      oputs (_("\
  -t, --time        print last system clock change\n\
"));
      oputs (_("\
  -T, -w, --mesg    add user's message status as +, - or ?\n\
"));
      oputs (_("\
  -u, --users       list users logged in, including idle time\n\
"));
      oputs (_("\
      --message     same as -T\n\
"));
      oputs (_("\
      --writable    same as -T\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
If ARG1 ARG2 given, -m presumed: 'am i' or 'mom likes' are usual.\n\
"), UTMP_FILE, WTMP_FILE);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, پوچ))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 'a':
          need_boottime = true;
          need_deadprocs = true;
          need_login = true;
          need_initspawn = true;
          need_runlevel = true;
          need_clockchange = true;
          need_users = true;
          include_mesg = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          بشکن;

        حالت 'b':
          need_boottime = true;
          assumptions = false;
          بشکن;

        حالت 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          بشکن;

        حالت 'H':
          include_heading = true;
          بشکن;

        حالت 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          بشکن;

        حالت 'm':
          my_سطر_only = true;
          بشکن;

        حالت 'p':
          need_initspawn = true;
          assumptions = false;
          بشکن;

        حالت 'q':
          short_list = true;
          بشکن;

        حالت 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          بشکن;

        حالت 's':
          short_output = true;
          بشکن;

        حالت 't':
          need_clockchange = true;
          assumptions = false;
          بشکن;

        حالت 'T':
        حالت 'w':
          include_mesg = true;
          بشکن;

        حالت 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          بشکن;

        حالت LOOKUP_OPTION:
          do_lookup = true;
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  اگر (include_exit)
    {
      short_output = false;
    }

  اگر (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  وگرنه
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  انتخاب (argc - optind)
    {
    حالت 2:			/* who <blurf> <glop> */
      my_سطر_only = true;
      FALLTHROUGH;
    حالت -1:
    حالت 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      بشکن;

    حالت 1:			/* who <utmp file> */
      who (argv[optind], 0);
      بشکن;

    پیشفرض:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  بازگشت EXIT_SUCCESS;
}
