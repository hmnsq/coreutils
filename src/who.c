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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "canon-host.h"
#ಅನ್ತರ್ಭಾವ "readutmp.h"
#ಅನ್ತರ್ಭಾವ "hard-locale.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xvasprintf.h"

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
#ಅನ್ಯಥಾ
# define UT_TYPE_RUN_LVL(U) false
#endif

#ifdef INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#ಅನ್ಯಥಾ
# define UT_TYPE_INIT_PROCESS(U) false
#endif

#ifdef DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#ಅನ್ಯಥಾ
# define UT_TYPE_DEAD_PROCESS(U) false
#endif

#ifdef NEW_TIME
# define UT_TYPE_NEW_TIME(U) ((U)->ut_type == NEW_TIME)
#ಅನ್ಯಥಾ
# define UT_TYPE_NEW_TIME(U) false
#endif

#define IDLESTR_LEN 6

#ಯದಿ HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  ವರ್ಣ Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (long ಪೂರ್ಣಾಙ್ಕ) (Utmp_ent->ut_pid))
#ಅನ್ಯಥಾ
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  ವರ್ಣ ಧ್ರುವ *Var = ""
#endif

#ಯದಿ HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#ಅನ್ಯಥಾ
# define UT_ID(U) "??"
#endif

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ಸ್ಥಿರ bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
ಸ್ಥಿರ bool short_list;

/* If true, display only name, line, and time fields.  */
ಸ್ಥಿರ bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
ಸ್ಥಿರ bool include_idle;

/* If true, display a line at the top describing each field.  */
ಸ್ಥಿರ bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
ಸ್ಥಿರ bool include_mesg;

/* If true, display process termination & exit status.  */
ಸ್ಥಿರ bool include_exit;

/* If true, display the last boot time.  */
ಸ್ಥಿರ bool need_boottime;

/* If true, display dead processes.  */
ಸ್ಥಿರ bool need_deadprocs;

/* If true, display processes waiting for user login.  */
ಸ್ಥಿರ bool need_login;

/* If true, display processes started by init.  */
ಸ್ಥಿರ bool need_initspawn;

/* If true, display the last clock change.  */
ಸ್ಥಿರ bool need_clockchange;

/* If true, display the current runlevel.  */
ಸ್ಥಿರ bool need_runlevel;

/* If true, display user processes.  */
ಸ್ಥಿರ bool need_users;

/* If true, display info only for the controlling tty.  */
ಸ್ಥಿರ bool my_ಸಾಲು_only;

/* The strftime format to use for login times, and its expected
   output width.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *time_format;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ time_format_width;

/* for long options with no corresponding short option, use enum */
ಗಣನಾ
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"all", no_argument, ರಿಕ್ತ, 'a'},
  {"boot", no_argument, ರಿಕ್ತ, 'b'},
  {"count", no_argument, ರಿಕ್ತ, 'q'},
  {"dead", no_argument, ರಿಕ್ತ, 'd'},
  {"heading", no_argument, ರಿಕ್ತ, 'H'},
  {"login", no_argument, ರಿಕ್ತ, 'l'},
  {"lookup", no_argument, ರಿಕ್ತ, LOOKUP_OPTION},
  {"message", no_argument, ರಿಕ್ತ, 'T'},
  {"mesg", no_argument, ರಿಕ್ತ, 'T'},
  {"process", no_argument, ರಿಕ್ತ, 'p'},
  {"runlevel", no_argument, ರಿಕ್ತ, 'r'},
  {"short", no_argument, ರಿಕ್ತ, 's'},
  {"time", no_argument, ರಿಕ್ತ, 't'},
  {"users", no_argument, ರಿಕ್ತ, 'u'},
  {"writable", no_argument, ರಿಕ್ತ, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
idle_string (time_t when, time_t boottime)
{
  ಸ್ಥಿರ time_t now = TYPE_MINIMUM (time_t);

  ಯದಿ (now == TYPE_MINIMUM (time_t))
    time (&now);

  ಪೂರ್ಣಾಙ್ಕ seconds_idle;
  ಯದಿ (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      ಯದಿ (seconds_idle < 60)
        ಪ್ರತ್ಯಾಗಮ "  .  ";
      ಅನ್ಯಥಾ
        {
          ಸ್ಥಿರ ವರ್ಣ idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          ಪ್ರತ್ಯಾಗಮ idle_hhmm;
        }
    }

  ಪ್ರತ್ಯಾಗಮ _(" old ");
}

/* Return a time string.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
time_string (STRUCT_UTMP ಧ್ರುವ *utmp_ent)
{
  ಸ್ಥಿರ ವರ್ಣ buf[INT_STRLEN_BOUND (intmax_t) + ಪರಿಮಾಣ "-%m-%d %H:%M"];
  ಸಂರಚನಾ tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ಯದಿ (tmp)
    {
      strftime (buf, ಪರಿಮಾಣ buf, time_format, tmp);
      ಪ್ರತ್ಯಾಗಮ buf;
    }
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
ಸ್ಥಿರ ಶೂನ್ಯ
print_ಸಾಲು (ವರ್ಣ ಧ್ರುವ *user, ಧ್ರುವ ವರ್ಣ state,
            ವರ್ಣ ಧ್ರುವ *ಸಾಲು,
            ವರ್ಣ ಧ್ರುವ *time_str, ವರ್ಣ ಧ್ರುವ *idle, ವರ್ಣ ಧ್ರುವ *pid,
            ವರ್ಣ ಧ್ರುವ *comment, ವರ್ಣ ಧ್ರುವ *exitstr)
{
  ಸ್ಥಿರ ವರ್ಣ mesg[3] = { ' ', 'x', '\0' };
  ವರ್ಣ *buf;
  ವರ್ಣ x_idle[1 + IDLESTR_LEN + 1];
  ವರ್ಣ x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  ವರ್ಣ *x_exitstr;

  mesg[1] = state;

  ಯದಿ (include_idle && !short_output && strlen (idle) < ಪರಿಮಾಣ x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  ಅನ್ಯಥಾ
    *x_idle = '\0';

  ಯದಿ (!short_output && strlen (pid) < ಪರಿಮಾಣ x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  ಅನ್ಯಥಾ
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, strlen (exitstr)) + 1 : 1);
  ಯದಿ (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  ಅನ್ಯಥಾ
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
                   ಸಾಲು,
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
    ವರ್ಣ *p = buf + strlen (buf);
    ಯಾವತ್ (*--p == ' ')
      /* empty */;
    *(p + 1) = '\0';
  }

  puts (buf);
  ಮುಕ್ತ (buf);
  ಮುಕ್ತ (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
ಸ್ಥಿರ bool
is_tty_writable (ಸಂರಚನಾ stat ಧ್ರುವ *pstat)
{
#ifdef TTY_GROUP_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  ಸಂರಚನಾ group *ttygr = getgrnam (TTY_GROUP_NAME);
  ಯದಿ (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    ಪ್ರತ್ಯಾಗಮ false;
#endif

  ಪ್ರತ್ಯಾಗಮ pstat->st_mode & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
ಸ್ಥಿರ ಶೂನ್ಯ
print_user (STRUCT_UTMP ಧ್ರುವ *utmp_ent, time_t boottime)
{
  ಸಂರಚನಾ stat stats;
  time_t last_change;
  ವರ್ಣ mesg;
  ವರ್ಣ idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  ಸ್ಥಿರ ವರ್ಣ *hoststr;
#ಯದಿ HAVE_STRUCT_XTMP_UT_HOST
  ಸ್ಥಿರ idx_t hostlen;
#endif

  /* If ut_line contains a space, the device name starts after the space.  */
  ವರ್ಣ *ಸಾಲು = utmp_ent->ut_ಸಾಲು;
  ವರ್ಣ *space = strchr (ಸಾಲು, ' ');
  ಸಾಲು = space ? space + 1 : ಸಾಲು;

  ಪೂರ್ಣಾಙ್ಕ dirfd;
  ಯದಿ (IS_ABSOLUTE_FILE_NAME (ಸಾಲು))
    dirfd = AT_FDCWD;
  ಅನ್ಯಥಾ
    {
      ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ dev_dirfd;
      ಯದಿ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ಯದಿ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ಯದಿ (AT_FDCWD <= dirfd && fstatat (dirfd, ಸಾಲು, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      last_change = stats.st_atime;
    }
  ಅನ್ಯಥಾ
    {
      mesg = '?';
      last_change = 0;
    }

  ಯದಿ (last_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (last_change, boottime));
  ಅನ್ಯಥಾ
    sprintf (idlestr, "  ?");

#ಯದಿ HAVE_STRUCT_XTMP_UT_HOST
  ಯದಿ (utmp_ent->ut_host[0])
    {
      ವರ್ಣ *host = ರಿಕ್ತ;
      ವರ್ಣ *display = ರಿಕ್ತ;
      ವರ್ಣ *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      ಯದಿ (display)
        *display++ = '\0';

      ಯದಿ (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      ಯದಿ (! host)
        host = ut_host;

      ಯದಿ (display)
        {
          idx_t needed = strlen (host) + strlen (display) + 4;
          ಯದಿ (hostlen < needed)
            {
              ಮುಕ್ತ (hoststr);
              hoststr = xpalloc (ರಿಕ್ತ, &hostlen, needed - hostlen, -1, 1);
            }
          ವರ್ಣ *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      ಅನ್ಯಥಾ
        {
          idx_t needed = strlen (host) + 3;
          ಯದಿ (hostlen < needed)
            {
              ಮುಕ್ತ (hoststr);
              hoststr = xpalloc (ರಿಕ್ತ, &hostlen, needed - hostlen, -1, 1);
            }
          ವರ್ಣ *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      ಯದಿ (host != ut_host)
        ಮುಕ್ತ (host);
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#endif

  print_ಸಾಲು (utmp_ent->ut_user, mesg,
              utmp_ent->ut_ಸಾಲು,
              time_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_boottime (STRUCT_UTMP ಧ್ರುವ *utmp_ent)
{
  print_ಸಾಲು ("", ' ', _("system boot"),
              time_string (utmp_ent), "", "", "", "");
}

ಸ್ಥಿರ ವರ್ಣ *
make_id_equals_comment (STRUCT_UTMP ಧ್ರುವ *utmp_ent)
{
  ವರ್ಣ ಧ್ರುವ *id = UT_ID (utmp_ent);
  idx_t idlen = strlen (id);
  ವರ್ಣ ಧ್ರುವ *prefix = _("id=");
  idx_t prefixlen = strlen (prefix);
  ವರ್ಣ *comment = xmalloc (prefixlen + idlen + 1);
  ವರ್ಣ *p = mempcpy (comment, prefix, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  ಪ್ರತ್ಯಾಗಮ comment;
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_deadprocs (STRUCT_UTMP ಧ್ರುವ *utmp_ent)
{
  ಸ್ಥಿರ ವರ್ಣ *exitstr;
  ವರ್ಣ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  ಯದಿ (!exitstr)
    exitstr = xmalloc (strlen (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + strlen (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_ಸಾಲು ("", ' ', utmp_ent->ut_ಸಾಲು,
              time_string (utmp_ent), "", pidstr, comment, exitstr);
  ಮುಕ್ತ (comment);
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_login (STRUCT_UTMP ಧ್ರುವ *utmp_ent)
{
  ವರ್ಣ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_ಸಾಲು (_("LOGIN"), ' ', utmp_ent->ut_ಸಾಲು,
              time_string (utmp_ent), "", pidstr, comment, "");
  ಮುಕ್ತ (comment);
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_initspawn (STRUCT_UTMP ಧ್ರುವ *utmp_ent)
{
  ವರ್ಣ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_ಸಾಲು ("", ' ', utmp_ent->ut_ಸಾಲು,
              time_string (utmp_ent), "", pidstr, comment, "");
  ಮುಕ್ತ (comment);
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_clockchange (STRUCT_UTMP ಧ್ರುವ *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_ಸಾಲು ("", ' ', _("clock change"),
              time_string (utmp_ent), "", "", "", "");
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_runlevel (STRUCT_UTMP ಧ್ರುವ *utmp_ent)
{
  ಸ್ಥಿರ ವರ್ಣ *runlevline, *comment;
  unsigned ವರ್ಣ last = utmp_ent->ut_pid / 256;
  unsigned ವರ್ಣ curr = utmp_ent->ut_pid % 256;

  ಯದಿ (!runlevline)
    runlevline = xmalloc (strlen (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  ಯದಿ (!comment)
    comment = xmalloc (strlen (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (last == 'N') ? 'S' : last);

  print_ಸಾಲು ("", ' ', runlevline, time_string (utmp_ent),
              "", "", c_isprint (last) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
ಸ್ಥಿರ ಶೂನ್ಯ
list_entries_who (idx_t n, STRUCT_UTMP ಧ್ರುವ *utmp_buf)
{
  idx_t entries = 0;

  ಯಾವತ್ (n--)
    {
      ಯದಿ (IS_USER_PROCESS (utmp_buf))
        {
          ವರ್ಣ *trimmed_name;

          trimmed_name = extract_trimmed_name (utmp_buf);

          ಯದಿ (entries)
            putchar (' ');
          fputs (trimmed_name, stdout);
          ಮುಕ್ತ (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  printf (_("\n# users=%td\n"), entries);
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_heading (ಶೂನ್ಯ)
{
  print_ಸಾಲು (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
ಸ್ಥಿರ ಶೂನ್ಯ
scan_entries (idx_t n, STRUCT_UTMP ಧ್ರುವ *utmp_buf)
{
  ವರ್ಣ *ttyname_b IF_LINT ( = ರಿಕ್ತ);
  time_t boottime = TYPE_MINIMUM (time_t);

  ಯದಿ (include_heading)
    print_heading ();

  ಯದಿ (my_ಸಾಲು_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      ಯದಿ (!ttyname_b)
        ಪ್ರತ್ಯಾಗಮ;
      ಯದಿ (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += ಪರಿಮಾಣ "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  ಯಾವತ್ (n--)
    {
      ಯದಿ (!my_ಸಾಲು_only
          || str_endswith (utmp_buf->ut_ಸಾಲು, ttyname_b))
        {
          ಯದಿ (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          ಅನ್ಯಥಾ ಯದಿ (need_runlevel && UT_TYPE_RUN_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          ಅನ್ಯಥಾ ಯದಿ (need_boottime && UT_TYPE_BOOT_TIME (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          ಅನ್ಯಥಾ ಯದಿ (need_clockchange && UT_TYPE_NEW_TIME (utmp_buf))
            print_clockchange (utmp_buf);
          ಅನ್ಯಥಾ ಯದಿ (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          ಅನ್ಯಥಾ ಯದಿ (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          ಅನ್ಯಥಾ ಯದಿ (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      ಯದಿ (UT_TYPE_BOOT_TIME (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
ಸ್ಥಿರ ಶೂನ್ಯ
who (ವರ್ಣ ಧ್ರುವ *filename, ಪೂರ್ಣಾಙ್ಕ options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ಯದಿ (short_list)
    options |= READ_UTMP_USER_PROCESS;
  ಯದಿ (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  ಯದಿ (short_list)
    list_entries_who (n_users, utmp_buf);
  ಅನ್ಯಥಾ
    scan_entries (n_users, utmp_buf);

  ಮುಕ್ತ (utmp_buf);
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
  -H, --heading     print ಸಾಲು of column headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt to canonicalize hostnames via DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard ಒಳಹರಿವು\n\
"));
      oputs (_("\
  -p, --process     print active processes spawned by init\n\
"));
      oputs (_("\
  -q, --ಎಣಿಕೆ       all login names and ಸಂಖ್ಯೆ of users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --short       print only name, ಸಾಲು, and time (ಸ್ವಭಾವ)\n\
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

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'a':
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
          ಭಙ್ಗ;

        ಪಕ್ಷ 'b':
          need_boottime = true;
          assumptions = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'H':
          include_heading = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'm':
          my_ಸಾಲು_only = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'p':
          need_initspawn = true;
          assumptions = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'q':
          short_list = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 's':
          short_output = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 't':
          need_clockchange = true;
          assumptions = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'T':
        ಪಕ್ಷ 'w':
          include_mesg = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ LOOKUP_OPTION:
          do_lookup = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  ಯದಿ (include_exit)
    {
      short_output = false;
    }

  ಯದಿ (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ಅನ್ಯಥಾ
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ವಿಕಲ್ಪ (argc - optind)
    {
    ಪಕ್ಷ 2:			/* who <blurf> <glop> */
      my_ಸಾಲು_only = true;
      FALLTHROUGH;
    ಪಕ್ಷ -1:
    ಪಕ್ಷ 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ಭಙ್ಗ;

    ಪಕ್ಷ 1:			/* who <utmp file> */
      who (argv[optind], 0);
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}
