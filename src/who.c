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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <stdio.h>

#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "canon-host.h"
#അന്തര്ഭാവ "readutmp.h"
#അന്തര്ഭാവ "hard-locale.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "xvasprintf.h"

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
#അല്ലെങ്കിൽ
# define UT_TYPE_RUN_LVL(U) false
#endif

#ifdef INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#അല്ലെങ്കിൽ
# define UT_TYPE_INIT_PROCESS(U) false
#endif

#ifdef DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#അല്ലെങ്കിൽ
# define UT_TYPE_DEAD_PROCESS(U) false
#endif

#ifdef NEW_TIME
# define UT_TYPE_NEW_TIME(U) ((U)->ut_type == NEW_TIME)
#അല്ലെങ്കിൽ
# define UT_TYPE_NEW_TIME(U) false
#endif

#define IDLESTR_LEN 6

#എങ്കിൽ HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  അക്ഷരം Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (long പൂർണ്ണസംഖ്യ) (Utmp_ent->ut_pid))
#അല്ലെങ്കിൽ
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  അക്ഷരം സ്ഥിരം *Var = ""
#endif

#എങ്കിൽ HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#അല്ലെങ്കിൽ
# define UT_ID(U) "??"
#endif

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
സ്ഥിതം bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
സ്ഥിതം bool short_list;

/* If true, display only name, line, and time fields.  */
സ്ഥിതം bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
സ്ഥിതം bool include_idle;

/* If true, display a line at the top describing each field.  */
സ്ഥിതം bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
സ്ഥിതം bool include_mesg;

/* If true, display process termination & exit status.  */
സ്ഥിതം bool include_exit;

/* If true, display the last boot time.  */
സ്ഥിതം bool need_boottime;

/* If true, display dead processes.  */
സ്ഥിതം bool need_deadprocs;

/* If true, display processes waiting for user login.  */
സ്ഥിതം bool need_login;

/* If true, display processes started by init.  */
സ്ഥിതം bool need_initspawn;

/* If true, display the last clock change.  */
സ്ഥിതം bool need_clockchange;

/* If true, display the current runlevel.  */
സ്ഥിതം bool need_runlevel;

/* If true, display user processes.  */
സ്ഥിതം bool need_users;

/* If true, display info only for the controlling tty.  */
സ്ഥിതം bool my_വരി_only;

/* The strftime format to use for login times, and its expected
   output width.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *time_format;
സ്ഥിതം പൂർണ്ണസംഖ്യ time_format_width;

/* for long options with no corresponding short option, use enum */
എണ്ണൽ
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"all", no_argument, ശൂന്യ, 'a'},
  {"boot", no_argument, ശൂന്യ, 'b'},
  {"count", no_argument, ശൂന്യ, 'q'},
  {"dead", no_argument, ശൂന്യ, 'd'},
  {"heading", no_argument, ശൂന്യ, 'H'},
  {"login", no_argument, ശൂന്യ, 'l'},
  {"lookup", no_argument, ശൂന്യ, LOOKUP_OPTION},
  {"message", no_argument, ശൂന്യ, 'T'},
  {"mesg", no_argument, ശൂന്യ, 'T'},
  {"process", no_argument, ശൂന്യ, 'p'},
  {"runlevel", no_argument, ശൂന്യ, 'r'},
  {"short", no_argument, ശൂന്യ, 's'},
  {"time", no_argument, ശൂന്യ, 't'},
  {"users", no_argument, ശൂന്യ, 'u'},
  {"writable", no_argument, ശൂന്യ, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
സ്ഥിതം അക്ഷരം സ്ഥിരം *
idle_string (time_t when, time_t boottime)
{
  സ്ഥിതം time_t now = TYPE_MINIMUM (time_t);

  എങ്കിൽ (now == TYPE_MINIMUM (time_t))
    time (&now);

  പൂർണ്ണസംഖ്യ seconds_idle;
  എങ്കിൽ (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      എങ്കിൽ (seconds_idle < 60)
        തിരികെ "  .  ";
      അല്ലെങ്കിൽ
        {
          സ്ഥിതം അക്ഷരം idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          തിരികെ idle_hhmm;
        }
    }

  തിരികെ _(" old ");
}

/* Return a time string.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *
time_string (STRUCT_UTMP സ്ഥിരം *utmp_ent)
{
  സ്ഥിതം അക്ഷരം buf[INT_STRLEN_BOUND (intmax_t) + വലുപ്പം "-%m-%d %H:%M"];
  ഘടന tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  എങ്കിൽ (tmp)
    {
      strftime (buf, വലുപ്പം buf, time_format, tmp);
      തിരികെ buf;
    }
  അല്ലെങ്കിൽ
    തിരികെ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
സ്ഥിതം ശൂന്യം
print_വരി (അക്ഷരം സ്ഥിരം *user, സ്ഥിരം അക്ഷരം state,
            അക്ഷരം സ്ഥിരം *വരി,
            അക്ഷരം സ്ഥിരം *time_str, അക്ഷരം സ്ഥിരം *idle, അക്ഷരം സ്ഥിരം *pid,
            അക്ഷരം സ്ഥിരം *comment, അക്ഷരം സ്ഥിരം *exitstr)
{
  സ്ഥിതം അക്ഷരം mesg[3] = { ' ', 'x', '\0' };
  അക്ഷരം *buf;
  അക്ഷരം x_idle[1 + IDLESTR_LEN + 1];
  അക്ഷരം x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  അക്ഷരം *x_exitstr;

  mesg[1] = state;

  എങ്കിൽ (include_idle && !short_output && strlen (idle) < വലുപ്പം x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  അല്ലെങ്കിൽ
    *x_idle = '\0';

  എങ്കിൽ (!short_output && strlen (pid) < വലുപ്പം x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  അല്ലെങ്കിൽ
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, strlen (exitstr)) + 1 : 1);
  എങ്കിൽ (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  അല്ലെങ്കിൽ
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
                   വരി,
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
    അക്ഷരം *p = buf + strlen (buf);
    ആകുമ്പോൾ (*--p == ' ')
      /* empty */;
    *(p + 1) = '\0';
  }

  puts (buf);
  സ്വതന്ത്രം (buf);
  സ്വതന്ത്രം (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
സ്ഥിതം bool
is_tty_writable (ഘടന stat സ്ഥിരം *pstat)
{
#ifdef TTY_GROUP_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  ഘടന group *ttygr = getgrnam (TTY_GROUP_NAME);
  എങ്കിൽ (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    തിരികെ false;
#endif

  തിരികെ pstat->st_mode & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
സ്ഥിതം ശൂന്യം
print_user (STRUCT_UTMP സ്ഥിരം *utmp_ent, time_t boottime)
{
  ഘടന stat stats;
  time_t last_change;
  അക്ഷരം mesg;
  അക്ഷരം idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  സ്ഥിതം അക്ഷരം *hoststr;
#എങ്കിൽ HAVE_STRUCT_XTMP_UT_HOST
  സ്ഥിതം idx_t hostlen;
#endif

  /* If ut_line contains a space, the device name starts after the space.  */
  അക്ഷരം *വരി = utmp_ent->ut_വരി;
  അക്ഷരം *space = strchr (വരി, ' ');
  വരി = space ? space + 1 : വരി;

  പൂർണ്ണസംഖ്യ dirfd;
  എങ്കിൽ (IS_ABSOLUTE_FILE_NAME (വരി))
    dirfd = AT_FDCWD;
  അല്ലെങ്കിൽ
    {
      സ്ഥിതം പൂർണ്ണസംഖ്യ dev_dirfd;
      എങ്കിൽ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          എങ്കിൽ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  എങ്കിൽ (AT_FDCWD <= dirfd && fstatat (dirfd, വരി, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      last_change = stats.st_atime;
    }
  അല്ലെങ്കിൽ
    {
      mesg = '?';
      last_change = 0;
    }

  എങ്കിൽ (last_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (last_change, boottime));
  അല്ലെങ്കിൽ
    sprintf (idlestr, "  ?");

#എങ്കിൽ HAVE_STRUCT_XTMP_UT_HOST
  എങ്കിൽ (utmp_ent->ut_host[0])
    {
      അക്ഷരം *host = ശൂന്യ;
      അക്ഷരം *display = ശൂന്യ;
      അക്ഷരം *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      എങ്കിൽ (display)
        *display++ = '\0';

      എങ്കിൽ (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      എങ്കിൽ (! host)
        host = ut_host;

      എങ്കിൽ (display)
        {
          idx_t needed = strlen (host) + strlen (display) + 4;
          എങ്കിൽ (hostlen < needed)
            {
              സ്വതന്ത്രം (hoststr);
              hoststr = xpalloc (ശൂന്യ, &hostlen, needed - hostlen, -1, 1);
            }
          അക്ഷരം *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      അല്ലെങ്കിൽ
        {
          idx_t needed = strlen (host) + 3;
          എങ്കിൽ (hostlen < needed)
            {
              സ്വതന്ത്രം (hoststr);
              hoststr = xpalloc (ശൂന്യ, &hostlen, needed - hostlen, -1, 1);
            }
          അക്ഷരം *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      എങ്കിൽ (host != ut_host)
        സ്വതന്ത്രം (host);
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#endif

  print_വരി (utmp_ent->ut_user, mesg,
              utmp_ent->ut_വരി,
              time_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

സ്ഥിതം ശൂന്യം
print_boottime (STRUCT_UTMP സ്ഥിരം *utmp_ent)
{
  print_വരി ("", ' ', _("system boot"),
              time_string (utmp_ent), "", "", "", "");
}

സ്ഥിതം അക്ഷരം *
make_id_equals_comment (STRUCT_UTMP സ്ഥിരം *utmp_ent)
{
  അക്ഷരം സ്ഥിരം *id = UT_ID (utmp_ent);
  idx_t idlen = strlen (id);
  അക്ഷരം സ്ഥിരം *prefix = _("id=");
  idx_t prefixlen = strlen (prefix);
  അക്ഷരം *comment = xmalloc (prefixlen + idlen + 1);
  അക്ഷരം *p = mempcpy (comment, prefix, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  തിരികെ comment;
}

സ്ഥിതം ശൂന്യം
print_deadprocs (STRUCT_UTMP സ്ഥിരം *utmp_ent)
{
  സ്ഥിതം അക്ഷരം *exitstr;
  അക്ഷരം *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  എങ്കിൽ (!exitstr)
    exitstr = xmalloc (strlen (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + strlen (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_വരി ("", ' ', utmp_ent->ut_വരി,
              time_string (utmp_ent), "", pidstr, comment, exitstr);
  സ്വതന്ത്രം (comment);
}

സ്ഥിതം ശൂന്യം
print_login (STRUCT_UTMP സ്ഥിരം *utmp_ent)
{
  അക്ഷരം *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_വരി (_("LOGIN"), ' ', utmp_ent->ut_വരി,
              time_string (utmp_ent), "", pidstr, comment, "");
  സ്വതന്ത്രം (comment);
}

സ്ഥിതം ശൂന്യം
print_initspawn (STRUCT_UTMP സ്ഥിരം *utmp_ent)
{
  അക്ഷരം *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_വരി ("", ' ', utmp_ent->ut_വരി,
              time_string (utmp_ent), "", pidstr, comment, "");
  സ്വതന്ത്രം (comment);
}

സ്ഥിതം ശൂന്യം
print_clockchange (STRUCT_UTMP സ്ഥിരം *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_വരി ("", ' ', _("clock change"),
              time_string (utmp_ent), "", "", "", "");
}

സ്ഥിതം ശൂന്യം
print_runlevel (STRUCT_UTMP സ്ഥിരം *utmp_ent)
{
  സ്ഥിതം അക്ഷരം *runlevline, *comment;
  unsigned അക്ഷരം last = utmp_ent->ut_pid / 256;
  unsigned അക്ഷരം curr = utmp_ent->ut_pid % 256;

  എങ്കിൽ (!runlevline)
    runlevline = xmalloc (strlen (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  എങ്കിൽ (!comment)
    comment = xmalloc (strlen (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (last == 'N') ? 'S' : last);

  print_വരി ("", ' ', runlevline, time_string (utmp_ent),
              "", "", c_isprint (last) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
സ്ഥിതം ശൂന്യം
list_entries_who (idx_t n, STRUCT_UTMP സ്ഥിരം *utmp_buf)
{
  idx_t entries = 0;

  ആകുമ്പോൾ (n--)
    {
      എങ്കിൽ (IS_USER_PROCESS (utmp_buf))
        {
          അക്ഷരം *trimmed_name;

          trimmed_name = extract_trimmed_name (utmp_buf);

          എങ്കിൽ (entries)
            putchar (' ');
          fputs (trimmed_name, stdout);
          സ്വതന്ത്രം (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  printf (_("\n# users=%td\n"), entries);
}

സ്ഥിതം ശൂന്യം
print_heading (ശൂന്യം)
{
  print_വരി (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
സ്ഥിതം ശൂന്യം
scan_entries (idx_t n, STRUCT_UTMP സ്ഥിരം *utmp_buf)
{
  അക്ഷരം *ttyname_b IF_LINT ( = ശൂന്യ);
  time_t boottime = TYPE_MINIMUM (time_t);

  എങ്കിൽ (include_heading)
    print_heading ();

  എങ്കിൽ (my_വരി_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      എങ്കിൽ (!ttyname_b)
        തിരികെ;
      എങ്കിൽ (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += വലുപ്പം "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  ആകുമ്പോൾ (n--)
    {
      എങ്കിൽ (!my_വരി_only
          || str_endswith (utmp_buf->ut_വരി, ttyname_b))
        {
          എങ്കിൽ (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          അല്ലെങ്കിൽ എങ്കിൽ (need_runlevel && UT_TYPE_RUN_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          അല്ലെങ്കിൽ എങ്കിൽ (need_boottime && UT_TYPE_BOOT_TIME (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          അല്ലെങ്കിൽ എങ്കിൽ (need_clockchange && UT_TYPE_NEW_TIME (utmp_buf))
            print_clockchange (utmp_buf);
          അല്ലെങ്കിൽ എങ്കിൽ (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          അല്ലെങ്കിൽ എങ്കിൽ (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          അല്ലെങ്കിൽ എങ്കിൽ (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      എങ്കിൽ (UT_TYPE_BOOT_TIME (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
സ്ഥിതം ശൂന്യം
who (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  എങ്കിൽ (short_list)
    options |= READ_UTMP_USER_PROCESS;
  എങ്കിൽ (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  എങ്കിൽ (short_list)
    list_entries_who (n_users, utmp_buf);
  അല്ലെങ്കിൽ
    scan_entries (n_users, utmp_buf);

  സ്വതന്ത്രം (utmp_buf);
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
  -H, --heading     print വരി of column headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt to canonicalize hostnames via DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard നിവേശം\n\
"));
      oputs (_("\
  -p, --process     print active processes spawned by init\n\
"));
      oputs (_("\
  -q, --എണ്ണം       all login names and സംഖ്യ of users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --short       print only name, വരി, and time (സ്ഥിരസ്ഥിതി)\n\
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'a':
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
          നിർത്തുക;

        അവസ്ഥ 'b':
          need_boottime = true;
          assumptions = false;
          നിർത്തുക;

        അവസ്ഥ 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          നിർത്തുക;

        അവസ്ഥ 'H':
          include_heading = true;
          നിർത്തുക;

        അവസ്ഥ 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          നിർത്തുക;

        അവസ്ഥ 'm':
          my_വരി_only = true;
          നിർത്തുക;

        അവസ്ഥ 'p':
          need_initspawn = true;
          assumptions = false;
          നിർത്തുക;

        അവസ്ഥ 'q':
          short_list = true;
          നിർത്തുക;

        അവസ്ഥ 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          നിർത്തുക;

        അവസ്ഥ 's':
          short_output = true;
          നിർത്തുക;

        അവസ്ഥ 't':
          need_clockchange = true;
          assumptions = false;
          നിർത്തുക;

        അവസ്ഥ 'T':
        അവസ്ഥ 'w':
          include_mesg = true;
          നിർത്തുക;

        അവസ്ഥ 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          നിർത്തുക;

        അവസ്ഥ LOOKUP_OPTION:
          do_lookup = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  എങ്കിൽ (include_exit)
    {
      short_output = false;
    }

  എങ്കിൽ (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  അല്ലെങ്കിൽ
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  തിരഞ്ഞെടുപ്പ് (argc - optind)
    {
    അവസ്ഥ 2:			/* who <blurf> <glop> */
      my_വരി_only = true;
      FALLTHROUGH;
    അവസ്ഥ -1:
    അവസ്ഥ 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      നിർത്തുക;

    അവസ്ഥ 1:			/* who <utmp file> */
      who (argv[optind], 0);
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  തിരികെ EXIT_SUCCESS;
}
