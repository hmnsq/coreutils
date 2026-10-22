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

#اگر_تعریف_شدہ TTY_گروہ_NAME
# include <grp.h>
#اگر_ختم

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "who"

#تعریف_کرو AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Michael Stone")

#اگر_تعریف_شدہ تسلسل_LVL
# define UT_TYPE_تسلسل_LVL(U) ((U)->ut_type == تسلسل_LVL)
#ورنہ
# define UT_TYPE_تسلسل_LVL(U) false
#اگر_ختم

#اگر_تعریف_شدہ INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#ورنہ
# define UT_TYPE_INIT_PROCESS(U) false
#اگر_ختم

#اگر_تعریف_شدہ DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#ورنہ
# define UT_TYPE_DEAD_PROCESS(U) false
#اگر_ختم

#اگر_تعریف_شدہ NEW_وقت
# define UT_TYPE_NEW_وقت(U) ((U)->ut_type == NEW_وقت)
#ورنہ
# define UT_TYPE_NEW_وقت(U) false
#اگر_ختم

#تعریف_کرو IDLESTR_LEN 6

#اگر HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  حرف Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (طویل صحیح) (Utmp_ent->ut_pid))
#ورنہ
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  حرف ثابت *Var = ""
#اگر_ختم

#اگر HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#ورنہ
# define UT_ID(U) "??"
#اگر_ختم

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ساکن bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
ساکن bool short_list;

/* If true, display only name, line, and time fields.  */
ساکن bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
ساکن bool include_idle;

/* If true, display a line at the top describing each field.  */
ساکن bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
ساکن bool include_mesg;

/* If true, display process termination & exit status.  */
ساکن bool include_exit;

/* If true, display the last boot time.  */
ساکن bool need_boottime;

/* If true, display dead processes.  */
ساکن bool need_deadprocs;

/* If true, display processes waiting for user login.  */
ساکن bool need_login;

/* If true, display processes started by init.  */
ساکن bool need_initspawn;

/* If true, display the last clock change.  */
ساکن bool need_clockchange;

/* If true, display the current runlevel.  */
ساکن bool need_runlevel;

/* If true, display user processes.  */
ساکن bool need_users;

/* If true, display info only for the controlling tty.  */
ساکن bool my_سطر_only;

/* The strftime format to use for login times, and its expected
   output width.  */
ساکن حرف ثابت *وقت_format;
ساکن صحیح وقت_format_چوڑائی;

/* for long options with no corresponding short option, use enum */
شمار
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت longopts[] =
{
  {"all", no_دلیل, تہی, 'a'},
  {"boot", no_دلیل, تہی, 'b'},
  {"count", no_دلیل, تہی, 'q'},
  {"dead", no_دلیل, تہی, 'd'},
  {"heading", no_دلیل, تہی, 'H'},
  {"login", no_دلیل, تہی, 'l'},
  {"lookup", no_دلیل, تہی, LOOKUP_OPTION},
  {"message", no_دلیل, تہی, 'T'},
  {"mesg", no_دلیل, تہی, 'T'},
  {"process", no_دلیل, تہی, 'p'},
  {"runlevel", no_دلیل, تہی, 'r'},
  {"short", no_دلیل, تہی, 's'},
  {"time", no_دلیل, تہی, 't'},
  {"users", no_دلیل, تہی, 'u'},
  {"writable", no_دلیل, تہی, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
ساکن حرف ثابت *
idle_string (time_t when, time_t boottime)
{
  ساکن time_t now = TYPE_MINIMUM (time_t);

  اگر (now == TYPE_MINIMUM (time_t))
    وقت (&now);

  صحیح seconds_idle;
  اگر (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      اگر (seconds_idle < 60)
        واپس "  .  ";
      ورنہ
        {
          ساکن حرف idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          واپس idle_hhmm;
        }
    }

  واپس _(" old ");
}

/* Return a time string.  */
ساکن حرف ثابت *
وقت_string (STRUCT_UTMP ثابت *utmp_ent)
{
  ساکن حرف buf[INT_STRLEN_BOUND (intmax_t) + حجم "-%m-%d %H:%M"];
  ساخت tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  اگر (tmp)
    {
      strftime (buf, حجم buf, وقت_format, tmp);
      واپس buf;
    }
  ورنہ
    واپس timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
ساکن خالی
print_سطر (حرف ثابت *user, ثابت حرف state,
            حرف ثابت *سطر,
            حرف ثابت *وقت_str, حرف ثابت *idle, حرف ثابت *pid,
            حرف ثابت *comment, حرف ثابت *exitstr)
{
  ساکن حرف mesg[3] = { ' ', 'x', '\0' };
  حرف *buf;
  حرف x_idle[1 + IDLESTR_LEN + 1];
  حرف x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  حرف *x_exitstr;

  mesg[1] = state;

  اگر (include_idle && !short_output && لڑی_لمبائی (idle) < حجم x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  ورنہ
    *x_idle = '\0';

  اگر (!short_output && لڑی_لمبائی (pid) < حجم x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  ورنہ
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, لڑی_لمبائی (exitstr)) + 1 : 1);
  اگر (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  ورنہ
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
                   وقت_format_چوڑائی,
                   وقت_str,
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
    حرف *p = buf + لڑی_لمبائی (buf);
    جب (*--p == ' ')
      /*خالی_خانے*/;
    *(p + 1) = '\0';
  }

  puts (buf);
  آزاد (buf);
  آزاد (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
ساکن bool
is_tty_writable (ساخت stat ثابت *pstat)
{
#اگر_تعریف_شدہ TTY_گروہ_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  ساخت گروہ *ttygr = getgrnam (TTY_گروہ_NAME);
  اگر (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    واپس false;
#اگر_ختم

  واپس pstat->st_منوال & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
ساکن خالی
print_user (STRUCT_UTMP ثابت *utmp_ent, time_t boottime)
{
  ساخت stat stats;
  time_t آخری_change;
  حرف mesg;
  حرف idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  ساکن حرف *hoststr;
#اگر HAVE_STRUCT_XTMP_UT_HOST
  ساکن idx_t hostlen;
#اگر_ختم

  /* If ut_line contains a space, the device name starts after the space.  */
  حرف *سطر = utmp_ent->ut_سطر;
  حرف *space = strchr (سطر, ' ');
  سطر = space ? space + 1 : سطر;

  صحیح dirfd;
  اگر (IS_ABSOLUTE_FILE_NAME (سطر))
    dirfd = AT_FDCWD;
  ورنہ
    {
      ساکن صحیح dev_dirfd;
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
      آخری_change = stats.st_atime;
    }
  ورنہ
    {
      mesg = '?';
      آخری_change = 0;
    }

  اگر (آخری_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (آخری_change, boottime));
  ورنہ
    sprintf (idlestr, "  ?");

#اگر HAVE_STRUCT_XTMP_UT_HOST
  اگر (utmp_ent->ut_host[0])
    {
      حرف *host = تہی;
      حرف *display = تہی;
      حرف *ut_host = utmp_ent->ut_host;

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
          idx_t needed = لڑی_لمبائی (host) + لڑی_لمبائی (display) + 4;
          اگر (hostlen < needed)
            {
              آزاد (hoststr);
              hoststr = xpalloc (تہی, &hostlen, needed - hostlen, -1, 1);
            }
          حرف *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      ورنہ
        {
          idx_t needed = لڑی_لمبائی (host) + 3;
          اگر (hostlen < needed)
            {
              آزاد (hoststr);
              hoststr = xpalloc (تہی, &hostlen, needed - hostlen, -1, 1);
            }
          حرف *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      اگر (host != ut_host)
        آزاد (host);
    }
  ورنہ
    {
      اگر (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#اگر_ختم

  print_سطر (utmp_ent->ut_user, mesg,
              utmp_ent->ut_سطر,
              وقت_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

ساکن خالی
print_boottime (STRUCT_UTMP ثابت *utmp_ent)
{
  print_سطر ("", ' ', _("system boot"),
              وقت_string (utmp_ent), "", "", "", "");
}

ساکن حرف *
make_id_equals_comment (STRUCT_UTMP ثابت *utmp_ent)
{
  حرف ثابت *id = UT_ID (utmp_ent);
  idx_t idlen = لڑی_لمبائی (id);
  حرف ثابت *سابقہ = _("id=");
  idx_t prefixlen = لڑی_لمبائی (سابقہ);
  حرف *comment = xmalloc (prefixlen + idlen + 1);
  حرف *p = mempcpy (comment, سابقہ, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  واپس comment;
}

ساکن خالی
print_deadprocs (STRUCT_UTMP ثابت *utmp_ent)
{
  ساکن حرف *exitstr;
  حرف *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  اگر (!exitstr)
    exitstr = xmalloc (لڑی_لمبائی (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + لڑی_لمبائی (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_سطر ("", ' ', utmp_ent->ut_سطر,
              وقت_string (utmp_ent), "", pidstr, comment, exitstr);
  آزاد (comment);
}

ساکن خالی
print_login (STRUCT_UTMP ثابت *utmp_ent)
{
  حرف *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_سطر (_("LOGIN"), ' ', utmp_ent->ut_سطر,
              وقت_string (utmp_ent), "", pidstr, comment, "");
  آزاد (comment);
}

ساکن خالی
print_initspawn (STRUCT_UTMP ثابت *utmp_ent)
{
  حرف *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_سطر ("", ' ', utmp_ent->ut_سطر,
              وقت_string (utmp_ent), "", pidstr, comment, "");
  آزاد (comment);
}

ساکن خالی
print_clockchange (STRUCT_UTMP ثابت *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_سطر ("", ' ', _("clock change"),
              وقت_string (utmp_ent), "", "", "", "");
}

ساکن خالی
print_runlevel (STRUCT_UTMP ثابت *utmp_ent)
{
  ساکن حرف *runlevline, *comment;
  غیرعلامتی حرف آخری = utmp_ent->ut_pid / 256;
  غیرعلامتی حرف curr = utmp_ent->ut_pid % 256;

  اگر (!runlevline)
    runlevline = xmalloc (لڑی_لمبائی (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  اگر (!comment)
    comment = xmalloc (لڑی_لمبائی (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (آخری == 'N') ? 'S' : آخری);

  print_سطر ("", ' ', runlevline, وقت_string (utmp_ent),
              "", "", c_isprint (آخری) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
ساکن خالی
list_entries_who (idx_t n, STRUCT_UTMP ثابت *utmp_buf)
{
  idx_t entries = 0;

  جب (n--)
    {
      اگر (IS_USER_PROCESS (utmp_buf))
        {
          حرف *trimmed_name;

          trimmed_name = نکالو_trimmed_name (utmp_buf);

          اگر (entries)
            حرف_لکھو (' ');
          fputs (trimmed_name, stdout);
          آزاد (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  لکھو (_("\n# users=%td\n"), entries);
}

ساکن خالی
print_heading (خالی)
{
  print_سطر (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
ساکن خالی
جائزہ_entries (idx_t n, STRUCT_UTMP ثابت *utmp_buf)
{
  حرف *ttyname_b IF_LINT ( = تہی);
  time_t boottime = TYPE_MINIMUM (time_t);

  اگر (include_heading)
    print_heading ();

  اگر (my_سطر_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      اگر (!ttyname_b)
        واپس;
      اگر (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += حجم "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  جب (n--)
    {
      اگر (!my_سطر_only
          || str_endswith (utmp_buf->ut_سطر, ttyname_b))
        {
          اگر (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          ورنہ اگر (need_runlevel && UT_TYPE_تسلسل_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          ورنہ اگر (need_boottime && UT_TYPE_BOOT_وقت (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          ورنہ اگر (need_clockchange && UT_TYPE_NEW_وقت (utmp_buf))
            print_clockchange (utmp_buf);
          ورنہ اگر (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          ورنہ اگر (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          ورنہ اگر (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      اگر (UT_TYPE_BOOT_وقت (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
ساکن خالی
who (حرف ثابت *filename, صحیح options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  اگر (short_list)
    options |= READ_UTMP_USER_PROCESS;
  اگر (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  اگر (short_list)
    list_entries_who (n_users, utmp_buf);
  ورنہ
    جائزہ_entries (n_users, utmp_buf);

  آزاد (utmp_buf);
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [ FILE | ARG1 ARG2 ]\n"), پروگرام_name);
      fputs (_("\
Print information about users who are currently logged اندر.\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -a, --all         same as -b -d --login -p -r -t -T -u\n\
"));
      oputs (_("\
  -b, --boot        وقت کا آخری system boot\n\
"));
      oputs (_("\
  -d, --dead        print dead processes\n\
"));
      oputs (_("\
  -H, --heading     print سطر کا عمود headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt مقصد canonicalize hostnames بذریعہ DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard داخلہ\n\
"));
      oputs (_("\
  -p, --process     print فعال processes spawned by init\n\
"));
      oputs (_("\
  -q, --گنتی       all login نام and نمبر کا users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --مختصر       print only name, سطر, and وقت (طےشدہ)\n\
"));
      oputs (_("\
  -t, --وقت        print آخری system clock change\n\
"));
      oputs (_("\
  -T, -w, --mesg    جوڑو user's پیغام_2 حالت_2 as +, - or ?\n\
"));
      oputs (_("\
  -u, --users       list users logged اندر, including idle وقت\n\
"));
      oputs (_("\
      --پیغام_2     same as -T\n\
"));
      oputs (_("\
      --writable    same as -T\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      لکھو (_("\
\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
If ARG1 ARG2 given, -m presumed: 'am i' or 'mom likes' are usual.\n\
"), UTMP_FILE, WTMP_FILE);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, تہی))
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
          توڑو;

        حالت 'b':
          need_boottime = true;
          assumptions = false;
          توڑو;

        حالت 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          توڑو;

        حالت 'H':
          include_heading = true;
          توڑو;

        حالت 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          توڑو;

        حالت 'm':
          my_سطر_only = true;
          توڑو;

        حالت 'p':
          need_initspawn = true;
          assumptions = false;
          توڑو;

        حالت 'q':
          short_list = true;
          توڑو;

        حالت 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          توڑو;

        حالت 's':
          short_output = true;
          توڑو;

        حالت 't':
          need_clockchange = true;
          assumptions = false;
          توڑو;

        حالت 'T':
        حالت 'w':
          include_mesg = true;
          توڑو;

        حالت 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          توڑو;

        حالت LOOKUP_OPTION:
          do_lookup = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
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

  اگر (hard_locale (LC_وقت))
    {
      وقت_format = "%Y-%m-%d %H:%M";
      وقت_format_چوڑائی = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ورنہ
    {
      وقت_format = "%b %e %H:%M";
      وقت_format_چوڑائی = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  انتخاب (argc - optind)
    {
    حالت 2:			/* who <blurf> <glop> */
      my_سطر_only = true;
      FALLTHROUGH;
    حالت -1:
    حالت 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      توڑو;

    حالت 1:			/* who <utmp file> */
      who (argv[optind], 0);
      توڑو;

    طےشدہ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  واپس EXIT_SUCCESS;
}
