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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <stdio.h>

#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ "c-ctype.h"
#অন্তর্ভাৱ "canon-host.h"
#অন্তর্ভাৱ "readutmp.h"
#অন্তর্ভাৱ "hard-locale.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "xvasprintf.h"

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
#অন্যথা
# define UT_TYPE_RUN_LVL(U) false
#endif

#ifdef INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#অন্যথা
# define UT_TYPE_INIT_PROCESS(U) false
#endif

#ifdef DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#অন্যথা
# define UT_TYPE_DEAD_PROCESS(U) false
#endif

#ifdef NEW_TIME
# define UT_TYPE_NEW_TIME(U) ((U)->ut_type == NEW_TIME)
#অন্যথা
# define UT_TYPE_NEW_TIME(U) false
#endif

#define IDLESTR_LEN 6

#যদি HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  ৱর্ণ Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (long পূর্ণাঙ্ক) (Utmp_ent->ut_pid))
#অন্যথা
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  ৱর্ণ ধ্রুৱ *Var = ""
#endif

#যদি HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#অন্যথা
# define UT_ID(U) "??"
#endif

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
স্থির bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
স্থির bool short_list;

/* If true, display only name, line, and time fields.  */
স্থির bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
স্থির bool include_idle;

/* If true, display a line at the top describing each field.  */
স্থির bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
স্থির bool include_mesg;

/* If true, display process termination & exit status.  */
স্থির bool include_exit;

/* If true, display the last boot time.  */
স্থির bool need_boottime;

/* If true, display dead processes.  */
স্থির bool need_deadprocs;

/* If true, display processes waiting for user login.  */
স্থির bool need_login;

/* If true, display processes started by init.  */
স্থির bool need_initspawn;

/* If true, display the last clock change.  */
স্থির bool need_clockchange;

/* If true, display the current runlevel.  */
স্থির bool need_runlevel;

/* If true, display user processes.  */
স্থির bool need_users;

/* If true, display info only for the controlling tty.  */
স্থির bool my_সাৰি_only;

/* The strftime format to use for login times, and its expected
   output width.  */
স্থির ৱর্ণ ধ্রুৱ *time_format;
স্থির পূর্ণাঙ্ক time_format_width;

/* for long options with no corresponding short option, use enum */
গণনা
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {"all", no_argument, রিক্ত, 'a'},
  {"boot", no_argument, রিক্ত, 'b'},
  {"count", no_argument, রিক্ত, 'q'},
  {"dead", no_argument, রিক্ত, 'd'},
  {"heading", no_argument, রিক্ত, 'H'},
  {"login", no_argument, রিক্ত, 'l'},
  {"lookup", no_argument, রিক্ত, LOOKUP_OPTION},
  {"message", no_argument, রিক্ত, 'T'},
  {"mesg", no_argument, রিক্ত, 'T'},
  {"process", no_argument, রিক্ত, 'p'},
  {"runlevel", no_argument, রিক্ত, 'r'},
  {"short", no_argument, রিক্ত, 's'},
  {"time", no_argument, রিক্ত, 't'},
  {"users", no_argument, রিক্ত, 'u'},
  {"writable", no_argument, রিক্ত, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
স্থির ৱর্ণ ধ্রুৱ *
idle_string (time_t when, time_t boottime)
{
  স্থির time_t now = TYPE_MINIMUM (time_t);

  যদি (now == TYPE_MINIMUM (time_t))
    time (&now);

  পূর্ণাঙ্ক seconds_idle;
  যদি (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      যদি (seconds_idle < 60)
        ঘূৰাই "  .  ";
      অন্যথা
        {
          স্থির ৱর্ণ idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          ঘূৰাই idle_hhmm;
        }
    }

  ঘূৰাই _(" old ");
}

/* Return a time string.  */
স্থির ৱর্ণ ধ্রুৱ *
time_string (STRUCT_UTMP ধ্রুৱ *utmp_ent)
{
  স্থির ৱর্ণ buf[INT_STRLEN_BOUND (intmax_t) + পরিমাণ "-%m-%d %H:%M"];
  সংরচনা tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  যদি (tmp)
    {
      strftime (buf, পরিমাণ buf, time_format, tmp);
      ঘূৰাই buf;
    }
  অন্যথা
    ঘূৰাই timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
স্থির শূন্য
print_সাৰি (ৱর্ণ ধ্রুৱ *user, ধ্রুৱ ৱর্ণ state,
            ৱর্ণ ধ্রুৱ *সাৰি,
            ৱর্ণ ধ্রুৱ *time_str, ৱর্ণ ধ্রুৱ *idle, ৱর্ণ ধ্রুৱ *pid,
            ৱর্ণ ধ্রুৱ *comment, ৱর্ণ ধ্রুৱ *exitstr)
{
  স্থির ৱর্ণ mesg[3] = { ' ', 'x', '\0' };
  ৱর্ণ *buf;
  ৱর্ণ x_idle[1 + IDLESTR_LEN + 1];
  ৱর্ণ x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  ৱর্ণ *x_exitstr;

  mesg[1] = state;

  যদি (include_idle && !short_output && strlen (idle) < পরিমাণ x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  অন্যথা
    *x_idle = '\0';

  যদি (!short_output && strlen (pid) < পরিমাণ x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  অন্যথা
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, strlen (exitstr)) + 1 : 1);
  যদি (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  অন্যথা
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
                   সাৰি,
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
    ৱর্ণ *p = buf + strlen (buf);
    যেতিয়ালৈ (*--p == ' ')
      /* empty */;
    *(p + 1) = '\0';
  }

  puts (buf);
  মুক্ত (buf);
  মুক্ত (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
স্থির bool
is_tty_writable (সংরচনা stat ধ্রুৱ *pstat)
{
#ifdef TTY_GROUP_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  সংরচনা group *ttygr = getgrnam (TTY_GROUP_NAME);
  যদি (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    ঘূৰাই false;
#endif

  ঘূৰাই pstat->st_mode & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
স্থির শূন্য
print_user (STRUCT_UTMP ধ্রুৱ *utmp_ent, time_t boottime)
{
  সংরচনা stat stats;
  time_t last_change;
  ৱর্ণ mesg;
  ৱর্ণ idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  স্থির ৱর্ণ *hoststr;
#যদি HAVE_STRUCT_XTMP_UT_HOST
  স্থির idx_t hostlen;
#endif

  /* If ut_line contains a space, the device name starts after the space.  */
  ৱর্ণ *সাৰি = utmp_ent->ut_সাৰি;
  ৱর্ণ *space = strchr (সাৰি, ' ');
  সাৰি = space ? space + 1 : সাৰি;

  পূর্ণাঙ্ক dirfd;
  যদি (IS_ABSOLUTE_FILE_NAME (সাৰি))
    dirfd = AT_FDCWD;
  অন্যথা
    {
      স্থির পূর্ণাঙ্ক dev_dirfd;
      যদি (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          যদি (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  যদি (AT_FDCWD <= dirfd && fstatat (dirfd, সাৰি, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      last_change = stats.st_atime;
    }
  অন্যথা
    {
      mesg = '?';
      last_change = 0;
    }

  যদি (last_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (last_change, boottime));
  অন্যথা
    sprintf (idlestr, "  ?");

#যদি HAVE_STRUCT_XTMP_UT_HOST
  যদি (utmp_ent->ut_host[0])
    {
      ৱর্ণ *host = রিক্ত;
      ৱর্ণ *display = রিক্ত;
      ৱর্ণ *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      যদি (display)
        *display++ = '\0';

      যদি (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      যদি (! host)
        host = ut_host;

      যদি (display)
        {
          idx_t needed = strlen (host) + strlen (display) + 4;
          যদি (hostlen < needed)
            {
              মুক্ত (hoststr);
              hoststr = xpalloc (রিক্ত, &hostlen, needed - hostlen, -1, 1);
            }
          ৱর্ণ *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      অন্যথা
        {
          idx_t needed = strlen (host) + 3;
          যদি (hostlen < needed)
            {
              মুক্ত (hoststr);
              hoststr = xpalloc (রিক্ত, &hostlen, needed - hostlen, -1, 1);
            }
          ৱর্ণ *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      যদি (host != ut_host)
        মুক্ত (host);
    }
  অন্যথা
    {
      যদি (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#endif

  print_সাৰি (utmp_ent->ut_user, mesg,
              utmp_ent->ut_সাৰি,
              time_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

স্থির শূন্য
print_boottime (STRUCT_UTMP ধ্রুৱ *utmp_ent)
{
  print_সাৰি ("", ' ', _("system boot"),
              time_string (utmp_ent), "", "", "", "");
}

স্থির ৱর্ণ *
make_id_equals_comment (STRUCT_UTMP ধ্রুৱ *utmp_ent)
{
  ৱর্ণ ধ্রুৱ *id = UT_ID (utmp_ent);
  idx_t idlen = strlen (id);
  ৱর্ণ ধ্রুৱ *prefix = _("id=");
  idx_t prefixlen = strlen (prefix);
  ৱর্ণ *comment = xmalloc (prefixlen + idlen + 1);
  ৱর্ণ *p = mempcpy (comment, prefix, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  ঘূৰাই comment;
}

স্থির শূন্য
print_deadprocs (STRUCT_UTMP ধ্রুৱ *utmp_ent)
{
  স্থির ৱর্ণ *exitstr;
  ৱর্ণ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  যদি (!exitstr)
    exitstr = xmalloc (strlen (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + strlen (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_সাৰি ("", ' ', utmp_ent->ut_সাৰি,
              time_string (utmp_ent), "", pidstr, comment, exitstr);
  মুক্ত (comment);
}

স্থির শূন্য
print_login (STRUCT_UTMP ধ্রুৱ *utmp_ent)
{
  ৱর্ণ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_সাৰি (_("LOGIN"), ' ', utmp_ent->ut_সাৰি,
              time_string (utmp_ent), "", pidstr, comment, "");
  মুক্ত (comment);
}

স্থির শূন্য
print_initspawn (STRUCT_UTMP ধ্রুৱ *utmp_ent)
{
  ৱর্ণ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_সাৰি ("", ' ', utmp_ent->ut_সাৰি,
              time_string (utmp_ent), "", pidstr, comment, "");
  মুক্ত (comment);
}

স্থির শূন্য
print_clockchange (STRUCT_UTMP ধ্রুৱ *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_সাৰি ("", ' ', _("clock change"),
              time_string (utmp_ent), "", "", "", "");
}

স্থির শূন্য
print_runlevel (STRUCT_UTMP ধ্রুৱ *utmp_ent)
{
  স্থির ৱর্ণ *runlevline, *comment;
  unsigned ৱর্ণ last = utmp_ent->ut_pid / 256;
  unsigned ৱর্ণ curr = utmp_ent->ut_pid % 256;

  যদি (!runlevline)
    runlevline = xmalloc (strlen (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  যদি (!comment)
    comment = xmalloc (strlen (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (last == 'N') ? 'S' : last);

  print_সাৰি ("", ' ', runlevline, time_string (utmp_ent),
              "", "", c_isprint (last) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
স্থির শূন্য
list_entries_who (idx_t n, STRUCT_UTMP ধ্রুৱ *utmp_buf)
{
  idx_t entries = 0;

  যেতিয়ালৈ (n--)
    {
      যদি (IS_USER_PROCESS (utmp_buf))
        {
          ৱর্ণ *trimmed_name;

          trimmed_name = extract_trimmed_name (utmp_buf);

          যদি (entries)
            putchar (' ');
          fputs (trimmed_name, stdout);
          মুক্ত (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  printf (_("\n# users=%td\n"), entries);
}

স্থির শূন্য
print_heading (শূন্য)
{
  print_সাৰি (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
স্থির শূন্য
scan_entries (idx_t n, STRUCT_UTMP ধ্রুৱ *utmp_buf)
{
  ৱর্ণ *ttyname_b IF_LINT ( = রিক্ত);
  time_t boottime = TYPE_MINIMUM (time_t);

  যদি (include_heading)
    print_heading ();

  যদি (my_সাৰি_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      যদি (!ttyname_b)
        ঘূৰাই;
      যদি (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += পরিমাণ "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  যেতিয়ালৈ (n--)
    {
      যদি (!my_সাৰি_only
          || str_endswith (utmp_buf->ut_সাৰি, ttyname_b))
        {
          যদি (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          অন্যথা যদি (need_runlevel && UT_TYPE_RUN_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          অন্যথা যদি (need_boottime && UT_TYPE_BOOT_TIME (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          অন্যথা যদি (need_clockchange && UT_TYPE_NEW_TIME (utmp_buf))
            print_clockchange (utmp_buf);
          অন্যথা যদি (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          অন্যথা যদি (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          অন্যথা যদি (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      যদি (UT_TYPE_BOOT_TIME (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
স্থির শূন্য
who (ৱর্ণ ধ্রুৱ *filename, পূর্ণাঙ্ক options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  যদি (short_list)
    options |= READ_UTMP_USER_PROCESS;
  যদি (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  যদি (short_list)
    list_entries_who (n_users, utmp_buf);
  অন্যথা
    scan_entries (n_users, utmp_buf);

  মুক্ত (utmp_buf);
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
  -H, --heading     print সাৰি of column headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt to canonicalize hostnames via DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard প্ৰবেশ\n\
"));
      oputs (_("\
  -p, --process     print active processes spawned by init\n\
"));
      oputs (_("\
  -q, --গুনতি       all login names and সংখ্যা of users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --short       print only name, সাৰি, and time (স্ৱভাৱ)\n\
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

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  পূর্ণাঙ্ক optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, রিক্ত))
         != -1)
    {
      ৱিকল্প (optc)
        {
        পক্ষ 'a':
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
          ভঙ্গ;

        পক্ষ 'b':
          need_boottime = true;
          assumptions = false;
          ভঙ্গ;

        পক্ষ 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          ভঙ্গ;

        পক্ষ 'H':
          include_heading = true;
          ভঙ্গ;

        পক্ষ 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          ভঙ্গ;

        পক্ষ 'm':
          my_সাৰি_only = true;
          ভঙ্গ;

        পক্ষ 'p':
          need_initspawn = true;
          assumptions = false;
          ভঙ্গ;

        পক্ষ 'q':
          short_list = true;
          ভঙ্গ;

        পক্ষ 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          ভঙ্গ;

        পক্ষ 's':
          short_output = true;
          ভঙ্গ;

        পক্ষ 't':
          need_clockchange = true;
          assumptions = false;
          ভঙ্গ;

        পক্ষ 'T':
        পক্ষ 'w':
          include_mesg = true;
          ভঙ্গ;

        পক্ষ 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          ভঙ্গ;

        পক্ষ LOOKUP_OPTION:
          do_lookup = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  যদি (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  যদি (include_exit)
    {
      short_output = false;
    }

  যদি (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  অন্যথা
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ৱিকল্প (argc - optind)
    {
    পক্ষ 2:			/* who <blurf> <glop> */
      my_সাৰি_only = true;
      FALLTHROUGH;
    পক্ষ -1:
    পক্ষ 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ভঙ্গ;

    পক্ষ 1:			/* who <utmp file> */
      who (argv[optind], 0);
      ভঙ্গ;

    স্ৱভাৱ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  ঘূৰাই EXIT_SUCCESS;
}
