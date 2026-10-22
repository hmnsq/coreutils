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

#ضمن <config.h>
#ضمن <getopt.h>
#ضمن <stdio.h>

#ضمن <sys/types.h>
#ضمن "system.h"

#ضمن "c-ctype.h"
#ضمن "canon-host.h"
#ضمن "readutmp.h"
#ضمن "hard-locale.h"
#ضمن "quote.h"
#ضمن "xvasprintf.h"

#جيڪڏهن_وصف_ٿيل TTY_گروهه_NAME
# include <grp.h>
#جيڪڏهن_پورو

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "who"

#وصف_ڪريو AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Michael Stone")

#جيڪڏهن_وصف_ٿيل لاڳيتو_LVL
# define UT_TYPE_لاڳيتو_LVL(U) ((U)->ut_type == لاڳيتو_LVL)
#وإلا
# define UT_TYPE_لاڳيتو_LVL(U) false
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#وإلا
# define UT_TYPE_INIT_PROCESS(U) false
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#وإلا
# define UT_TYPE_DEAD_PROCESS(U) false
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل NEW_وقت
# define UT_TYPE_NEW_وقت(U) ((U)->ut_type == NEW_وقت)
#وإلا
# define UT_TYPE_NEW_وقت(U) false
#جيڪڏهن_پورو

#وصف_ڪريو IDLESTR_LEN 6

#إذا HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  حرف Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (ڊگهو صحيح) (Utmp_ent->ut_pid))
#وإلا
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  حرف ثابت *Var = ""
#جيڪڏهن_پورو

#إذا HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#وإلا
# define UT_ID(U) "??"
#جيڪڏهن_پورو

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ساكن bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
ساكن bool short_list;

/* If true, display only name, line, and time fields.  */
ساكن bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
ساكن bool include_idle;

/* If true, display a line at the top describing each field.  */
ساكن bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
ساكن bool include_mesg;

/* If true, display process termination & exit status.  */
ساكن bool include_exit;

/* If true, display the last boot time.  */
ساكن bool need_boottime;

/* If true, display dead processes.  */
ساكن bool need_deadprocs;

/* If true, display processes waiting for user login.  */
ساكن bool need_login;

/* If true, display processes started by init.  */
ساكن bool need_initspawn;

/* If true, display the last clock change.  */
ساكن bool need_clockchange;

/* If true, display the current runlevel.  */
ساكن bool need_runlevel;

/* If true, display user processes.  */
ساكن bool need_users;

/* If true, display info only for the controlling tty.  */
ساكن bool my_سٽ_only;

/* The strftime format to use for login times, and its expected
   output width.  */
ساكن حرف ثابت *وقت_format;
ساكن صحيح وقت_format_ويڪر;

/* for long options with no corresponding short option, use enum */
تعداد
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"all", no_دليل, عدم, 'a'},
  {"boot", no_دليل, عدم, 'b'},
  {"count", no_دليل, عدم, 'q'},
  {"dead", no_دليل, عدم, 'd'},
  {"heading", no_دليل, عدم, 'H'},
  {"login", no_دليل, عدم, 'l'},
  {"lookup", no_دليل, عدم, LOOKUP_OPTION},
  {"message", no_دليل, عدم, 'T'},
  {"mesg", no_دليل, عدم, 'T'},
  {"process", no_دليل, عدم, 'p'},
  {"runlevel", no_دليل, عدم, 'r'},
  {"short", no_دليل, عدم, 's'},
  {"time", no_دليل, عدم, 't'},
  {"users", no_دليل, عدم, 'u'},
  {"writable", no_دليل, عدم, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
ساكن حرف ثابت *
idle_string (time_t when, time_t boottime)
{
  ساكن time_t now = TYPE_MINIMUM (time_t);

  إذا (now == TYPE_MINIMUM (time_t))
    وقت (&now);

  صحيح seconds_idle;
  إذا (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      إذا (seconds_idle < 60)
        أرجع "  .  ";
      وإلا
        {
          ساكن حرف idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          أرجع idle_hhmm;
        }
    }

  أرجع _(" old ");
}

/* Return a time string.  */
ساكن حرف ثابت *
وقت_string (STRUCT_UTMP ثابت *utmp_ent)
{
  ساكن حرف buf[INT_STRLEN_BOUND (intmax_t) + حجم "-%m-%d %H:%M"];
  بنية tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  إذا (tmp)
    {
      strftime (buf, حجم buf, وقت_format, tmp);
      أرجع buf;
    }
  وإلا
    أرجع timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
ساكن فراغ
print_سٽ (حرف ثابت *user, ثابت حرف state,
            حرف ثابت *سٽ,
            حرف ثابت *وقت_str, حرف ثابت *idle, حرف ثابت *pid,
            حرف ثابت *comment, حرف ثابت *exitstr)
{
  ساكن حرف mesg[3] = { ' ', 'x', '\0' };
  حرف *buf;
  حرف x_idle[1 + IDLESTR_LEN + 1];
  حرف x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  حرف *x_exitstr;

  mesg[1] = state;

  إذا (include_idle && !short_output && لڙي_ڊيگهه (idle) < حجم x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  وإلا
    *x_idle = '\0';

  إذا (!short_output && لڙي_ڊيگهه (pid) < حجم x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  وإلا
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, لڙي_ڊيگهه (exitstr)) + 1 : 1);
  إذا (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  وإلا
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
                   سٽ,
                   وقت_format_ويڪر,
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
    حرف *p = buf + لڙي_ڊيگهه (buf);
    طالما (*--p == ' ')
      /*خالي_جايون*/;
    *(p + 1) = '\0';
  }

  puts (buf);
  حرر (buf);
  حرر (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
ساكن bool
is_tty_writable (بنية stat ثابت *pstat)
{
#جيڪڏهن_وصف_ٿيل TTY_گروهه_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  بنية گروهه *ttygr = getgrnam (TTY_گروهه_NAME);
  إذا (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    أرجع false;
#جيڪڏهن_پورو

  أرجع pstat->st_گهڻائي & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
ساكن فراغ
print_user (STRUCT_UTMP ثابت *utmp_ent, time_t boottime)
{
  بنية stat stats;
  time_t آخري_change;
  حرف mesg;
  حرف idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  ساكن حرف *hoststr;
#إذا HAVE_STRUCT_XTMP_UT_HOST
  ساكن idx_t hostlen;
#جيڪڏهن_پورو

  /* If ut_line contains a space, the device name starts after the space.  */
  حرف *سٽ = utmp_ent->ut_سٽ;
  حرف *space = strchr (سٽ, ' ');
  سٽ = space ? space + 1 : سٽ;

  صحيح dirfd;
  إذا (IS_ABSOLUTE_FILE_NAME (سٽ))
    dirfd = AT_FDCWD;
  وإلا
    {
      ساكن صحيح dev_dirfd;
      إذا (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          إذا (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  إذا (AT_FDCWD <= dirfd && fstatat (dirfd, سٽ, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      آخري_change = stats.st_atime;
    }
  وإلا
    {
      mesg = '?';
      آخري_change = 0;
    }

  إذا (آخري_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (آخري_change, boottime));
  وإلا
    sprintf (idlestr, "  ?");

#إذا HAVE_STRUCT_XTMP_UT_HOST
  إذا (utmp_ent->ut_host[0])
    {
      حرف *host = عدم;
      حرف *display = عدم;
      حرف *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      إذا (display)
        *display++ = '\0';

      إذا (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      إذا (! host)
        host = ut_host;

      إذا (display)
        {
          idx_t needed = لڙي_ڊيگهه (host) + لڙي_ڊيگهه (display) + 4;
          إذا (hostlen < needed)
            {
              حرر (hoststr);
              hoststr = xpalloc (عدم, &hostlen, needed - hostlen, -1, 1);
            }
          حرف *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      وإلا
        {
          idx_t needed = لڙي_ڊيگهه (host) + 3;
          إذا (hostlen < needed)
            {
              حرر (hoststr);
              hoststr = xpalloc (عدم, &hostlen, needed - hostlen, -1, 1);
            }
          حرف *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      إذا (host != ut_host)
        حرر (host);
    }
  وإلا
    {
      إذا (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#جيڪڏهن_پورو

  print_سٽ (utmp_ent->ut_user, mesg,
              utmp_ent->ut_سٽ,
              وقت_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

ساكن فراغ
print_boottime (STRUCT_UTMP ثابت *utmp_ent)
{
  print_سٽ ("", ' ', _("system boot"),
              وقت_string (utmp_ent), "", "", "", "");
}

ساكن حرف *
make_id_equals_comment (STRUCT_UTMP ثابت *utmp_ent)
{
  حرف ثابت *id = UT_ID (utmp_ent);
  idx_t idlen = لڙي_ڊيگهه (id);
  حرف ثابت *اڳياڙي = _("id=");
  idx_t prefixlen = لڙي_ڊيگهه (اڳياڙي);
  حرف *comment = xmalloc (prefixlen + idlen + 1);
  حرف *p = mempcpy (comment, اڳياڙي, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  أرجع comment;
}

ساكن فراغ
print_deadprocs (STRUCT_UTMP ثابت *utmp_ent)
{
  ساكن حرف *exitstr;
  حرف *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  إذا (!exitstr)
    exitstr = xmalloc (لڙي_ڊيگهه (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + لڙي_ڊيگهه (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_سٽ ("", ' ', utmp_ent->ut_سٽ,
              وقت_string (utmp_ent), "", pidstr, comment, exitstr);
  حرر (comment);
}

ساكن فراغ
print_login (STRUCT_UTMP ثابت *utmp_ent)
{
  حرف *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_سٽ (_("LOGIN"), ' ', utmp_ent->ut_سٽ,
              وقت_string (utmp_ent), "", pidstr, comment, "");
  حرر (comment);
}

ساكن فراغ
print_initspawn (STRUCT_UTMP ثابت *utmp_ent)
{
  حرف *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_سٽ ("", ' ', utmp_ent->ut_سٽ,
              وقت_string (utmp_ent), "", pidstr, comment, "");
  حرر (comment);
}

ساكن فراغ
print_clockchange (STRUCT_UTMP ثابت *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_سٽ ("", ' ', _("clock change"),
              وقت_string (utmp_ent), "", "", "", "");
}

ساكن فراغ
print_runlevel (STRUCT_UTMP ثابت *utmp_ent)
{
  ساكن حرف *runlevline, *comment;
  بنانشان حرف آخري = utmp_ent->ut_pid / 256;
  بنانشان حرف curr = utmp_ent->ut_pid % 256;

  إذا (!runlevline)
    runlevline = xmalloc (لڙي_ڊيگهه (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  إذا (!comment)
    comment = xmalloc (لڙي_ڊيگهه (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (آخري == 'N') ? 'S' : آخري);

  print_سٽ ("", ' ', runlevline, وقت_string (utmp_ent),
              "", "", c_isprint (آخري) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
ساكن فراغ
list_entries_who (idx_t n, STRUCT_UTMP ثابت *utmp_buf)
{
  idx_t entries = 0;

  طالما (n--)
    {
      إذا (IS_USER_PROCESS (utmp_buf))
        {
          حرف *trimmed_name;

          trimmed_name = ڪڍو_trimmed_name (utmp_buf);

          إذا (entries)
            اکر_لکو (' ');
          fputs (trimmed_name, stdout);
          حرر (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  لکو (_("\n# users=%td\n"), entries);
}

ساكن فراغ
print_heading (فراغ)
{
  print_سٽ (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
ساكن فراغ
جاچ_entries (idx_t n, STRUCT_UTMP ثابت *utmp_buf)
{
  حرف *ttyname_b IF_LINT ( = عدم);
  time_t boottime = TYPE_MINIMUM (time_t);

  إذا (include_heading)
    print_heading ();

  إذا (my_سٽ_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      إذا (!ttyname_b)
        أرجع;
      إذا (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += حجم "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  طالما (n--)
    {
      إذا (!my_سٽ_only
          || str_endswith (utmp_buf->ut_سٽ, ttyname_b))
        {
          إذا (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          وإلا إذا (need_runlevel && UT_TYPE_لاڳيتو_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          وإلا إذا (need_boottime && UT_TYPE_BOOT_وقت (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          وإلا إذا (need_clockchange && UT_TYPE_NEW_وقت (utmp_buf))
            print_clockchange (utmp_buf);
          وإلا إذا (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          وإلا إذا (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          وإلا إذا (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      إذا (UT_TYPE_BOOT_وقت (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
ساكن فراغ
who (حرف ثابت *filename, صحيح options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  إذا (short_list)
    options |= READ_UTMP_USER_PROCESS;
  إذا (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  إذا (short_list)
    list_entries_who (n_users, utmp_buf);
  وإلا
    جاچ_entries (n_users, utmp_buf);

  حرر (utmp_buf);
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("Usage: %s [OPTION]... [ FILE | ARG1 ARG2 ]\n"), پروگرام_name);
      fputs (_("\
Print information about users who are currently logged اندريون.\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -a, --all         same as -b -d --login -p -r -t -T -u\n\
"));
      oputs (_("\
  -b, --boot        وقت جو آخري system boot\n\
"));
      oputs (_("\
  -d, --dead        print dead processes\n\
"));
      oputs (_("\
  -H, --heading     print سٽ جو ڪالم headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt منزل canonicalize hostnames ذريعي DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard داخلا\n\
"));
      oputs (_("\
  -p, --process     print سرگرم processes spawned by init\n\
"));
      oputs (_("\
  -q, --ڳڻپ       all login نالا and عدد جو users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --ننڍو       print only name, سٽ, and وقت (افتراضي)\n\
"));
      oputs (_("\
  -t, --وقت        print آخري system clock change\n\
"));
      oputs (_("\
  -T, -w, --mesg    جوڙيو user's پيغام_2 حالت as +, - or ?\n\
"));
      oputs (_("\
  -u, --users       list users logged اندريون, including idle وقت\n\
"));
      oputs (_("\
      --پيغام_2     same as -T\n\
"));
      oputs (_("\
      --writable    same as -T\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      لکو (_("\
\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
If ARG1 ARG2 given, -m presumed: 'am i' or 'mom likes' are usual.\n\
"), UTMP_FILE, WTMP_FILE);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, عدم))
         != -1)
    {
      بدل (optc)
        {
        حالة 'a':
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
          اكسر;

        حالة 'b':
          need_boottime = true;
          assumptions = false;
          اكسر;

        حالة 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          اكسر;

        حالة 'H':
          include_heading = true;
          اكسر;

        حالة 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          اكسر;

        حالة 'm':
          my_سٽ_only = true;
          اكسر;

        حالة 'p':
          need_initspawn = true;
          assumptions = false;
          اكسر;

        حالة 'q':
          short_list = true;
          اكسر;

        حالة 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          اكسر;

        حالة 's':
          short_output = true;
          اكسر;

        حالة 't':
          need_clockchange = true;
          assumptions = false;
          اكسر;

        حالة 'T':
        حالة 'w':
          include_mesg = true;
          اكسر;

        حالة 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          اكسر;

        حالة LOOKUP_OPTION:
          do_lookup = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  إذا (include_exit)
    {
      short_output = false;
    }

  إذا (hard_locale (LC_وقت))
    {
      وقت_format = "%Y-%m-%d %H:%M";
      وقت_format_ويڪر = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  وإلا
    {
      وقت_format = "%b %e %H:%M";
      وقت_format_ويڪر = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  بدل (argc - optind)
    {
    حالة 2:			/* who <blurf> <glop> */
      my_سٽ_only = true;
      FALLTHROUGH;
    حالة -1:
    حالة 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      اكسر;

    حالة 1:			/* who <utmp file> */
      who (argv[optind], 0);
      اكسر;

    افتراضي:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  أرجع EXIT_SUCCESS;
}
