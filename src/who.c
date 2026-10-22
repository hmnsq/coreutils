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

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"

#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "canon-host.h"
#అన్తర్భావ "readutmp.h"
#అన్తర్భావ "hard-locale.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xvasprintf.h"

#నిర్వచితమైతే TTY_సమూహం_NAME
# include <grp.h>
#అయితే_ముగింపు

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "who"

#నిర్వచించు AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Michael Stone")

#నిర్వచితమైతే నిరంతరం_LVL
# define UT_TYPE_నిరంతరం_LVL(U) ((U)->ut_type == నిరంతరం_LVL)
#లేకపోతే
# define UT_TYPE_నిరంతరం_LVL(U) false
#అయితే_ముగింపు

#నిర్వచితమైతే INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#లేకపోతే
# define UT_TYPE_INIT_PROCESS(U) false
#అయితే_ముగింపు

#నిర్వచితమైతే DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#లేకపోతే
# define UT_TYPE_DEAD_PROCESS(U) false
#అయితే_ముగింపు

#నిర్వచితమైతే NEW_సమయం
# define UT_TYPE_NEW_సమయం(U) ((U)->ut_type == NEW_సమయం)
#లేకపోతే
# define UT_TYPE_NEW_సమయం(U) false
#అయితే_ముగింపు

#నిర్వచించు IDLESTR_LEN 6

#ఒకవేళ HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  అక్షరం Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (దీర్ఘ పూర్ణాంకం) (Utmp_ent->ut_pid))
#లేకపోతే
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  అక్షరం స్థిరరాశి *Var = ""
#అయితే_ముగింపు

#ఒకవేళ HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#లేకపోతే
# define UT_ID(U) "??"
#అయితే_ముగింపు

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
స్థిర bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
స్థిర bool short_list;

/* If true, display only name, line, and time fields.  */
స్థిర bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
స్థిర bool include_idle;

/* If true, display a line at the top describing each field.  */
స్థిర bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
స్థిర bool include_mesg;

/* If true, display process termination & exit status.  */
స్థిర bool include_exit;

/* If true, display the last boot time.  */
స్థిర bool need_boottime;

/* If true, display dead processes.  */
స్థిర bool need_deadprocs;

/* If true, display processes waiting for user login.  */
స్థిర bool need_login;

/* If true, display processes started by init.  */
స్థిర bool need_initspawn;

/* If true, display the last clock change.  */
స్థిర bool need_clockchange;

/* If true, display the current runlevel.  */
స్థిర bool need_runlevel;

/* If true, display user processes.  */
స్థిర bool need_users;

/* If true, display info only for the controlling tty.  */
స్థిర bool my_పంక్తి_only;

/* The strftime format to use for login times, and its expected
   output width.  */
స్థిర అక్షరం స్థిరరాశి *సమయం_format;
స్థిర పూర్ణాంకం సమయం_format_వెడల్పు;

/* for long options with no corresponding short option, use enum */
గణన
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"all", no_వాదం, ఖాళీ, 'a'},
  {"boot", no_వాదం, ఖాళీ, 'b'},
  {"count", no_వాదం, ఖాళీ, 'q'},
  {"dead", no_వాదం, ఖాళీ, 'd'},
  {"heading", no_వాదం, ఖాళీ, 'H'},
  {"login", no_వాదం, ఖాళీ, 'l'},
  {"lookup", no_వాదం, ఖాళీ, LOOKUP_OPTION},
  {"message", no_వాదం, ఖాళీ, 'T'},
  {"mesg", no_వాదం, ఖాళీ, 'T'},
  {"process", no_వాదం, ఖాళీ, 'p'},
  {"runlevel", no_వాదం, ఖాళీ, 'r'},
  {"short", no_వాదం, ఖాళీ, 's'},
  {"time", no_వాదం, ఖాళీ, 't'},
  {"users", no_వాదం, ఖాళీ, 'u'},
  {"writable", no_వాదం, ఖాళీ, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
స్థిర అక్షరం స్థిరరాశి *
idle_string (time_t when, time_t boottime)
{
  స్థిర time_t now = TYPE_MINIMUM (time_t);

  ఒకవేళ (now == TYPE_MINIMUM (time_t))
    సమయం (&now);

  పూర్ణాంకం seconds_idle;
  ఒకవేళ (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      ఒకవేళ (seconds_idle < 60)
        తిరిగి "  .  ";
      లేకపోతే
        {
          స్థిర అక్షరం idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          తిరిగి idle_hhmm;
        }
    }

  తిరిగి _(" old ");
}

/* Return a time string.  */
స్థిర అక్షరం స్థిరరాశి *
సమయం_string (STRUCT_UTMP స్థిరరాశి *utmp_ent)
{
  స్థిర అక్షరం buf[INT_STRLEN_BOUND (intmax_t) + పరిమాణం "-%m-%d %H:%M"];
  నిర్మాణం tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ఒకవేళ (tmp)
    {
      strftime (buf, పరిమాణం buf, సమయం_format, tmp);
      తిరిగి buf;
    }
  లేకపోతే
    తిరిగి timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
స్థిర శూన్య
print_పంక్తి (అక్షరం స్థిరరాశి *user, స్థిరరాశి అక్షరం state,
            అక్షరం స్థిరరాశి *పంక్తి,
            అక్షరం స్థిరరాశి *సమయం_str, అక్షరం స్థిరరాశి *idle, అక్షరం స్థిరరాశి *pid,
            అక్షరం స్థిరరాశి *comment, అక్షరం స్థిరరాశి *exitstr)
{
  స్థిర అక్షరం mesg[3] = { ' ', 'x', '\0' };
  అక్షరం *buf;
  అక్షరం x_idle[1 + IDLESTR_LEN + 1];
  అక్షరం x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  అక్షరం *x_exitstr;

  mesg[1] = state;

  ఒకవేళ (include_idle && !short_output && అక్షరశ్రేణి_పొడవు (idle) < పరిమాణం x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  లేకపోతే
    *x_idle = '\0';

  ఒకవేళ (!short_output && అక్షరశ్రేణి_పొడవు (pid) < పరిమాణం x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  లేకపోతే
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, అక్షరశ్రేణి_పొడవు (exitstr)) + 1 : 1);
  ఒకవేళ (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  లేకపోతే
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
                   పంక్తి,
                   సమయం_format_వెడల్పు,
                   సమయం_str,
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
    అక్షరం *p = buf + అక్షరశ్రేణి_పొడవు (buf);
    అయితే (*--p == ' ')
      /*ఖాళీలు*/;
    *(p + 1) = '\0';
  }

  puts (buf);
  విడుదల (buf);
  విడుదల (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
స్థిర bool
is_tty_writable (నిర్మాణం stat స్థిరరాశి *pstat)
{
#నిర్వచితమైతే TTY_సమూహం_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  నిర్మాణం సమూహం *ttygr = getgrnam (TTY_సమూహం_NAME);
  ఒకవేళ (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    తిరిగి false;
#అయితే_ముగింపు

  తిరిగి pstat->st_బహుళకం & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
స్థిర శూన్య
print_user (STRUCT_UTMP స్థిరరాశి *utmp_ent, time_t boottime)
{
  నిర్మాణం stat stats;
  time_t చివరి_change;
  అక్షరం mesg;
  అక్షరం idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  స్థిర అక్షరం *hoststr;
#ఒకవేళ HAVE_STRUCT_XTMP_UT_HOST
  స్థిర idx_t hostlen;
#అయితే_ముగింపు

  /* If ut_line contains a space, the device name starts after the space.  */
  అక్షరం *పంక్తి = utmp_ent->ut_పంక్తి;
  అక్షరం *space = strchr (పంక్తి, ' ');
  పంక్తి = space ? space + 1 : పంక్తి;

  పూర్ణాంకం dirfd;
  ఒకవేళ (IS_ABSOLUTE_FILE_NAME (పంక్తి))
    dirfd = AT_FDCWD;
  లేకపోతే
    {
      స్థిర పూర్ణాంకం dev_dirfd;
      ఒకవేళ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ఒకవేళ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ఒకవేళ (AT_FDCWD <= dirfd && fstatat (dirfd, పంక్తి, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      చివరి_change = stats.st_atime;
    }
  లేకపోతే
    {
      mesg = '?';
      చివరి_change = 0;
    }

  ఒకవేళ (చివరి_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (చివరి_change, boottime));
  లేకపోతే
    sprintf (idlestr, "  ?");

#ఒకవేళ HAVE_STRUCT_XTMP_UT_HOST
  ఒకవేళ (utmp_ent->ut_host[0])
    {
      అక్షరం *host = ఖాళీ;
      అక్షరం *display = ఖాళీ;
      అక్షరం *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      ఒకవేళ (display)
        *display++ = '\0';

      ఒకవేళ (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      ఒకవేళ (! host)
        host = ut_host;

      ఒకవేళ (display)
        {
          idx_t needed = అక్షరశ్రేణి_పొడవు (host) + అక్షరశ్రేణి_పొడవు (display) + 4;
          ఒకవేళ (hostlen < needed)
            {
              విడుదల (hoststr);
              hoststr = xpalloc (ఖాళీ, &hostlen, needed - hostlen, -1, 1);
            }
          అక్షరం *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      లేకపోతే
        {
          idx_t needed = అక్షరశ్రేణి_పొడవు (host) + 3;
          ఒకవేళ (hostlen < needed)
            {
              విడుదల (hoststr);
              hoststr = xpalloc (ఖాళీ, &hostlen, needed - hostlen, -1, 1);
            }
          అక్షరం *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      ఒకవేళ (host != ut_host)
        విడుదల (host);
    }
  లేకపోతే
    {
      ఒకవేళ (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#అయితే_ముగింపు

  print_పంక్తి (utmp_ent->ut_user, mesg,
              utmp_ent->ut_పంక్తి,
              సమయం_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

స్థిర శూన్య
print_boottime (STRUCT_UTMP స్థిరరాశి *utmp_ent)
{
  print_పంక్తి ("", ' ', _("system boot"),
              సమయం_string (utmp_ent), "", "", "", "");
}

స్థిర అక్షరం *
make_id_equals_comment (STRUCT_UTMP స్థిరరాశి *utmp_ent)
{
  అక్షరం స్థిరరాశి *id = UT_ID (utmp_ent);
  idx_t idlen = అక్షరశ్రేణి_పొడవు (id);
  అక్షరం స్థిరరాశి *ఉపసర్గ = _("id=");
  idx_t prefixlen = అక్షరశ్రేణి_పొడవు (ఉపసర్గ);
  అక్షరం *comment = xmalloc (prefixlen + idlen + 1);
  అక్షరం *p = mempcpy (comment, ఉపసర్గ, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  తిరిగి comment;
}

స్థిర శూన్య
print_deadprocs (STRUCT_UTMP స్థిరరాశి *utmp_ent)
{
  స్థిర అక్షరం *exitstr;
  అక్షరం *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  ఒకవేళ (!exitstr)
    exitstr = xmalloc (అక్షరశ్రేణి_పొడవు (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + అక్షరశ్రేణి_పొడవు (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_పంక్తి ("", ' ', utmp_ent->ut_పంక్తి,
              సమయం_string (utmp_ent), "", pidstr, comment, exitstr);
  విడుదల (comment);
}

స్థిర శూన్య
print_login (STRUCT_UTMP స్థిరరాశి *utmp_ent)
{
  అక్షరం *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_పంక్తి (_("LOGIN"), ' ', utmp_ent->ut_పంక్తి,
              సమయం_string (utmp_ent), "", pidstr, comment, "");
  విడుదల (comment);
}

స్థిర శూన్య
print_initspawn (STRUCT_UTMP స్థిరరాశి *utmp_ent)
{
  అక్షరం *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_పంక్తి ("", ' ', utmp_ent->ut_పంక్తి,
              సమయం_string (utmp_ent), "", pidstr, comment, "");
  విడుదల (comment);
}

స్థిర శూన్య
print_clockchange (STRUCT_UTMP స్థిరరాశి *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_పంక్తి ("", ' ', _("clock change"),
              సమయం_string (utmp_ent), "", "", "", "");
}

స్థిర శూన్య
print_runlevel (STRUCT_UTMP స్థిరరాశి *utmp_ent)
{
  స్థిర అక్షరం *runlevline, *comment;
  గుర్తులేని అక్షరం చివరి = utmp_ent->ut_pid / 256;
  గుర్తులేని అక్షరం curr = utmp_ent->ut_pid % 256;

  ఒకవేళ (!runlevline)
    runlevline = xmalloc (అక్షరశ్రేణి_పొడవు (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  ఒకవేళ (!comment)
    comment = xmalloc (అక్షరశ్రేణి_పొడవు (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (చివరి == 'N') ? 'S' : చివరి);

  print_పంక్తి ("", ' ', runlevline, సమయం_string (utmp_ent),
              "", "", c_isprint (చివరి) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
స్థిర శూన్య
list_entries_who (idx_t n, STRUCT_UTMP స్థిరరాశి *utmp_buf)
{
  idx_t entries = 0;

  అయితే (n--)
    {
      ఒకవేళ (IS_USER_PROCESS (utmp_buf))
        {
          అక్షరం *trimmed_name;

          trimmed_name = వెలికితీయి_trimmed_name (utmp_buf);

          ఒకవేళ (entries)
            అక్షరం_రాయి (' ');
          fputs (trimmed_name, stdout);
          విడుదల (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  రాయి (_("\n# users=%td\n"), entries);
}

స్థిర శూన్య
print_heading (శూన్య)
{
  print_పంక్తి (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
స్థిర శూన్య
పరిశీలన_entries (idx_t n, STRUCT_UTMP స్థిరరాశి *utmp_buf)
{
  అక్షరం *ttyname_b IF_LINT ( = ఖాళీ);
  time_t boottime = TYPE_MINIMUM (time_t);

  ఒకవేళ (include_heading)
    print_heading ();

  ఒకవేళ (my_పంక్తి_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      ఒకవేళ (!ttyname_b)
        తిరిగి;
      ఒకవేళ (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += పరిమాణం "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  అయితే (n--)
    {
      ఒకవేళ (!my_పంక్తి_only
          || str_endswith (utmp_buf->ut_పంక్తి, ttyname_b))
        {
          ఒకవేళ (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          లేకపోతే ఒకవేళ (need_runlevel && UT_TYPE_నిరంతరం_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          లేకపోతే ఒకవేళ (need_boottime && UT_TYPE_BOOT_సమయం (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          లేకపోతే ఒకవేళ (need_clockchange && UT_TYPE_NEW_సమయం (utmp_buf))
            print_clockchange (utmp_buf);
          లేకపోతే ఒకవేళ (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          లేకపోతే ఒకవేళ (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          లేకపోతే ఒకవేళ (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      ఒకవేళ (UT_TYPE_BOOT_సమయం (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
స్థిర శూన్య
who (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ఒకవేళ (short_list)
    options |= READ_UTMP_USER_PROCESS;
  ఒకవేళ (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  ఒకవేళ (short_list)
    list_entries_who (n_users, utmp_buf);
  లేకపోతే
    పరిశీలన_entries (n_users, utmp_buf);

  విడుదల (utmp_buf);
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... [ FILE | ARG1 ARG2 ]\n"), కార్యక్రమం_name);
      fputs (_("\
Print information about users who are currently logged లోపల.\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -a, --all         same as -b -d --login -p -r -t -T -u\n\
"));
      oputs (_("\
  -b, --boot        సమయం యొక్క చివరి system boot\n\
"));
      oputs (_("\
  -d, --dead        print dead processes\n\
"));
      oputs (_("\
  -H, --heading     print పంక్తి యొక్క స్తంభం headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt గమ్యం canonicalize hostnames ద్వారా DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard ఆదానం\n\
"));
      oputs (_("\
  -p, --process     print చురుకైన processes spawned by init\n\
"));
      oputs (_("\
  -q, --లెక్క       all login పేర్లు and సంఖ్య యొక్క users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --పొట్టి       print only name, పంక్తి, and సమయం (అప్రమేయ)\n\
"));
      oputs (_("\
  -t, --సమయం        print చివరి system clock change\n\
"));
      oputs (_("\
  -T, -w, --mesg    కూడు user's సందేశం_2 స్థితి_2 as +, - or ?\n\
"));
      oputs (_("\
  -u, --users       list users logged లోపల, including idle సమయం\n\
"));
      oputs (_("\
      --సందేశం_2     same as -T\n\
"));
      oputs (_("\
      --writable    same as -T\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      రాయి (_("\
\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
If ARG1 ARG2 given, -m presumed: 'am i' or 'mom likes' are usual.\n\
"), UTMP_FILE, WTMP_FILE);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, ఖాళీ))
         != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'a':
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
          ఆపు;

        స్థితి 'b':
          need_boottime = true;
          assumptions = false;
          ఆపు;

        స్థితి 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          ఆపు;

        స్థితి 'H':
          include_heading = true;
          ఆపు;

        స్థితి 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          ఆపు;

        స్థితి 'm':
          my_పంక్తి_only = true;
          ఆపు;

        స్థితి 'p':
          need_initspawn = true;
          assumptions = false;
          ఆపు;

        స్థితి 'q':
          short_list = true;
          ఆపు;

        స్థితి 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          ఆపు;

        స్థితి 's':
          short_output = true;
          ఆపు;

        స్థితి 't':
          need_clockchange = true;
          assumptions = false;
          ఆపు;

        స్థితి 'T':
        స్థితి 'w':
          include_mesg = true;
          ఆపు;

        స్థితి 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          ఆపు;

        స్థితి LOOKUP_OPTION:
          do_lookup = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  ఒకవేళ (include_exit)
    {
      short_output = false;
    }

  ఒకవేళ (hard_locale (LC_సమయం))
    {
      సమయం_format = "%Y-%m-%d %H:%M";
      సమయం_format_వెడల్పు = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  లేకపోతే
    {
      సమయం_format = "%b %e %H:%M";
      సమయం_format_వెడల్పు = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ఎంపిక (argc - optind)
    {
    స్థితి 2:			/* who <blurf> <glop> */
      my_పంక్తి_only = true;
      FALLTHROUGH;
    స్థితి -1:
    స్థితి 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ఆపు;

    స్థితి 1:			/* who <utmp file> */
      who (argv[optind], 0);
      ఆపు;

    అప్రమేయ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  తిరిగి EXIT_SUCCESS;
}
