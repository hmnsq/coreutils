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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "canon-host.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "readutmp.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hard-locale.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xvasprintf.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TTY_ਸਮੂਹ_NAME
# include <grp.h>
#ਜੇ_ਅੰਤ

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "who"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Michael Stone")

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਲੜੀਵਾਰ_LVL
# define UT_TYPE_ਲੜੀਵਾਰ_LVL(U) ((U)->ut_type == ਲੜੀਵਾਰ_LVL)
#ਨਹੀਂਤਾਂ
# define UT_TYPE_ਲੜੀਵਾਰ_LVL(U) false
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#ਨਹੀਂਤਾਂ
# define UT_TYPE_INIT_PROCESS(U) false
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#ਨਹੀਂਤਾਂ
# define UT_TYPE_DEAD_PROCESS(U) false
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NEW_ਸਮਾਂ
# define UT_TYPE_NEW_ਸਮਾਂ(U) ((U)->ut_type == NEW_ਸਮਾਂ)
#ਨਹੀਂਤਾਂ
# define UT_TYPE_NEW_ਸਮਾਂ(U) false
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDLESTR_LEN 6

#ਜੇ HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  ਅੱਖਰ Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (ਲੰਮਾ ਪੂਰਨ) (Utmp_ent->ut_pid))
#ਨਹੀਂਤਾਂ
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  ਅੱਖਰ ਅਚਲ *Var = ""
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#ਨਹੀਂਤਾਂ
# define UT_ID(U) "??"
#ਜੇ_ਅੰਤ

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ਸਥਿਰ bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
ਸਥਿਰ bool short_list;

/* If true, display only name, line, and time fields.  */
ਸਥਿਰ bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
ਸਥਿਰ bool include_idle;

/* If true, display a line at the top describing each field.  */
ਸਥਿਰ bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
ਸਥਿਰ bool include_mesg;

/* If true, display process termination & exit status.  */
ਸਥਿਰ bool include_exit;

/* If true, display the last boot time.  */
ਸਥਿਰ bool need_boottime;

/* If true, display dead processes.  */
ਸਥਿਰ bool need_deadprocs;

/* If true, display processes waiting for user login.  */
ਸਥਿਰ bool need_login;

/* If true, display processes started by init.  */
ਸਥਿਰ bool need_initspawn;

/* If true, display the last clock change.  */
ਸਥਿਰ bool need_clockchange;

/* If true, display the current runlevel.  */
ਸਥਿਰ bool need_runlevel;

/* If true, display user processes.  */
ਸਥਿਰ bool need_users;

/* If true, display info only for the controlling tty.  */
ਸਥਿਰ bool my_ਸਤਰ_only;

/* The strftime format to use for login times, and its expected
   output width.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਸਮਾਂ_format;
ਸਥਿਰ ਪੂਰਨ ਸਮਾਂ_format_ਚੌੜਾਈ;

/* for long options with no corresponding short option, use enum */
ਗਿਣਤੀ
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"all", no_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"boot", no_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"count", no_ਦਲੀਲ, ਨਿਰਾ, 'q'},
  {"dead", no_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"heading", no_ਦਲੀਲ, ਨਿਰਾ, 'H'},
  {"login", no_ਦਲੀਲ, ਨਿਰਾ, 'l'},
  {"lookup", no_ਦਲੀਲ, ਨਿਰਾ, LOOKUP_OPTION},
  {"message", no_ਦਲੀਲ, ਨਿਰਾ, 'T'},
  {"mesg", no_ਦਲੀਲ, ਨਿਰਾ, 'T'},
  {"process", no_ਦਲੀਲ, ਨਿਰਾ, 'p'},
  {"runlevel", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"short", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"time", no_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"users", no_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"writable", no_ਦਲੀਲ, ਨਿਰਾ, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
idle_string (time_t when, time_t boottime)
{
  ਸਥਿਰ time_t now = TYPE_MINIMUM (time_t);

  ਜੇ (now == TYPE_MINIMUM (time_t))
    ਸਮਾਂ (&now);

  ਪੂਰਨ seconds_idle;
  ਜੇ (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      ਜੇ (seconds_idle < 60)
        ਵਾਪਸ "  .  ";
      ਨਹੀਂਤਾਂ
        {
          ਸਥਿਰ ਅੱਖਰ idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          ਵਾਪਸ idle_hhmm;
        }
    }

  ਵਾਪਸ _(" old ");
}

/* Return a time string.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
ਸਮਾਂ_string (STRUCT_UTMP ਅਚਲ *utmp_ent)
{
  ਸਥਿਰ ਅੱਖਰ buf[INT_STRLEN_BOUND (intmax_t) + ਆਕਾਰ "-%m-%d %H:%M"];
  ਬਣਤਰ tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ਜੇ (tmp)
    {
      strftime (buf, ਆਕਾਰ buf, ਸਮਾਂ_format, tmp);
      ਵਾਪਸ buf;
    }
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
ਸਥਿਰ ਖਾਲੀ
print_ਸਤਰ (ਅੱਖਰ ਅਚਲ *user, ਅਚਲ ਅੱਖਰ state,
            ਅੱਖਰ ਅਚਲ *ਸਤਰ,
            ਅੱਖਰ ਅਚਲ *ਸਮਾਂ_str, ਅੱਖਰ ਅਚਲ *idle, ਅੱਖਰ ਅਚਲ *pid,
            ਅੱਖਰ ਅਚਲ *comment, ਅੱਖਰ ਅਚਲ *exitstr)
{
  ਸਥਿਰ ਅੱਖਰ mesg[3] = { ' ', 'x', '\0' };
  ਅੱਖਰ *buf;
  ਅੱਖਰ x_idle[1 + IDLESTR_LEN + 1];
  ਅੱਖਰ x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  ਅੱਖਰ *x_exitstr;

  mesg[1] = state;

  ਜੇ (include_idle && !short_output && ਲੜੀ_ਲੰਬਾਈ (idle) < ਆਕਾਰ x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  ਨਹੀਂਤਾਂ
    *x_idle = '\0';

  ਜੇ (!short_output && ਲੜੀ_ਲੰਬਾਈ (pid) < ਆਕਾਰ x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  ਨਹੀਂਤਾਂ
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, ਲੜੀ_ਲੰਬਾਈ (exitstr)) + 1 : 1);
  ਜੇ (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  ਨਹੀਂਤਾਂ
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
                   ਸਤਰ,
                   ਸਮਾਂ_format_ਚੌੜਾਈ,
                   ਸਮਾਂ_str,
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
    ਅੱਖਰ *p = buf + ਲੜੀ_ਲੰਬਾਈ (buf);
    ਜਦੋਂ (*--p == ' ')
      /*ਖਾਲੀ_ਥਾਵਾਂ*/;
    *(p + 1) = '\0';
  }

  puts (buf);
  ਮੁਕਤ (buf);
  ਮੁਕਤ (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
ਸਥਿਰ bool
is_tty_writable (ਬਣਤਰ stat ਅਚਲ *pstat)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TTY_ਸਮੂਹ_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  ਬਣਤਰ ਸਮੂਹ *ttygr = getgrnam (TTY_ਸਮੂਹ_NAME);
  ਜੇ (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    ਵਾਪਸ false;
#ਜੇ_ਅੰਤ

  ਵਾਪਸ pstat->st_ਬਹੁਲਕ & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
ਸਥਿਰ ਖਾਲੀ
print_user (STRUCT_UTMP ਅਚਲ *utmp_ent, time_t boottime)
{
  ਬਣਤਰ stat stats;
  time_t ਆਖਰੀ_change;
  ਅੱਖਰ mesg;
  ਅੱਖਰ idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  ਸਥਿਰ ਅੱਖਰ *hoststr;
#ਜੇ HAVE_STRUCT_XTMP_UT_HOST
  ਸਥਿਰ idx_t hostlen;
#ਜੇ_ਅੰਤ

  /* If ut_line contains a space, the device name starts after the space.  */
  ਅੱਖਰ *ਸਤਰ = utmp_ent->ut_ਸਤਰ;
  ਅੱਖਰ *space = strchr (ਸਤਰ, ' ');
  ਸਤਰ = space ? space + 1 : ਸਤਰ;

  ਪੂਰਨ dirfd;
  ਜੇ (IS_ABSOLUTE_FILE_NAME (ਸਤਰ))
    dirfd = AT_FDCWD;
  ਨਹੀਂਤਾਂ
    {
      ਸਥਿਰ ਪੂਰਨ dev_dirfd;
      ਜੇ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ਜੇ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ਜੇ (AT_FDCWD <= dirfd && fstatat (dirfd, ਸਤਰ, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      ਆਖਰੀ_change = stats.st_atime;
    }
  ਨਹੀਂਤਾਂ
    {
      mesg = '?';
      ਆਖਰੀ_change = 0;
    }

  ਜੇ (ਆਖਰੀ_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (ਆਖਰੀ_change, boottime));
  ਨਹੀਂਤਾਂ
    sprintf (idlestr, "  ?");

#ਜੇ HAVE_STRUCT_XTMP_UT_HOST
  ਜੇ (utmp_ent->ut_host[0])
    {
      ਅੱਖਰ *host = ਨਿਰਾ;
      ਅੱਖਰ *display = ਨਿਰਾ;
      ਅੱਖਰ *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      ਜੇ (display)
        *display++ = '\0';

      ਜੇ (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      ਜੇ (! host)
        host = ut_host;

      ਜੇ (display)
        {
          idx_t needed = ਲੜੀ_ਲੰਬਾਈ (host) + ਲੜੀ_ਲੰਬਾਈ (display) + 4;
          ਜੇ (hostlen < needed)
            {
              ਮੁਕਤ (hoststr);
              hoststr = xpalloc (ਨਿਰਾ, &hostlen, needed - hostlen, -1, 1);
            }
          ਅੱਖਰ *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      ਨਹੀਂਤਾਂ
        {
          idx_t needed = ਲੜੀ_ਲੰਬਾਈ (host) + 3;
          ਜੇ (hostlen < needed)
            {
              ਮੁਕਤ (hoststr);
              hoststr = xpalloc (ਨਿਰਾ, &hostlen, needed - hostlen, -1, 1);
            }
          ਅੱਖਰ *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      ਜੇ (host != ut_host)
        ਮੁਕਤ (host);
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#ਜੇ_ਅੰਤ

  print_ਸਤਰ (utmp_ent->ut_user, mesg,
              utmp_ent->ut_ਸਤਰ,
              ਸਮਾਂ_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

ਸਥਿਰ ਖਾਲੀ
print_boottime (STRUCT_UTMP ਅਚਲ *utmp_ent)
{
  print_ਸਤਰ ("", ' ', _("system boot"),
              ਸਮਾਂ_string (utmp_ent), "", "", "", "");
}

ਸਥਿਰ ਅੱਖਰ *
make_id_equals_comment (STRUCT_UTMP ਅਚਲ *utmp_ent)
{
  ਅੱਖਰ ਅਚਲ *id = UT_ID (utmp_ent);
  idx_t idlen = ਲੜੀ_ਲੰਬਾਈ (id);
  ਅੱਖਰ ਅਚਲ *ਅਗੇਤਰ = _("id=");
  idx_t prefixlen = ਲੜੀ_ਲੰਬਾਈ (ਅਗੇਤਰ);
  ਅੱਖਰ *comment = xmalloc (prefixlen + idlen + 1);
  ਅੱਖਰ *p = mempcpy (comment, ਅਗੇਤਰ, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  ਵਾਪਸ comment;
}

ਸਥਿਰ ਖਾਲੀ
print_deadprocs (STRUCT_UTMP ਅਚਲ *utmp_ent)
{
  ਸਥਿਰ ਅੱਖਰ *exitstr;
  ਅੱਖਰ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  ਜੇ (!exitstr)
    exitstr = xmalloc (ਲੜੀ_ਲੰਬਾਈ (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + ਲੜੀ_ਲੰਬਾਈ (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_ਸਤਰ ("", ' ', utmp_ent->ut_ਸਤਰ,
              ਸਮਾਂ_string (utmp_ent), "", pidstr, comment, exitstr);
  ਮੁਕਤ (comment);
}

ਸਥਿਰ ਖਾਲੀ
print_login (STRUCT_UTMP ਅਚਲ *utmp_ent)
{
  ਅੱਖਰ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_ਸਤਰ (_("LOGIN"), ' ', utmp_ent->ut_ਸਤਰ,
              ਸਮਾਂ_string (utmp_ent), "", pidstr, comment, "");
  ਮੁਕਤ (comment);
}

ਸਥਿਰ ਖਾਲੀ
print_initspawn (STRUCT_UTMP ਅਚਲ *utmp_ent)
{
  ਅੱਖਰ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_ਸਤਰ ("", ' ', utmp_ent->ut_ਸਤਰ,
              ਸਮਾਂ_string (utmp_ent), "", pidstr, comment, "");
  ਮੁਕਤ (comment);
}

ਸਥਿਰ ਖਾਲੀ
print_clockchange (STRUCT_UTMP ਅਚਲ *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_ਸਤਰ ("", ' ', _("clock change"),
              ਸਮਾਂ_string (utmp_ent), "", "", "", "");
}

ਸਥਿਰ ਖਾਲੀ
print_runlevel (STRUCT_UTMP ਅਚਲ *utmp_ent)
{
  ਸਥਿਰ ਅੱਖਰ *runlevline, *comment;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਆਖਰੀ = utmp_ent->ut_pid / 256;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ curr = utmp_ent->ut_pid % 256;

  ਜੇ (!runlevline)
    runlevline = xmalloc (ਲੜੀ_ਲੰਬਾਈ (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  ਜੇ (!comment)
    comment = xmalloc (ਲੜੀ_ਲੰਬਾਈ (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (ਆਖਰੀ == 'N') ? 'S' : ਆਖਰੀ);

  print_ਸਤਰ ("", ' ', runlevline, ਸਮਾਂ_string (utmp_ent),
              "", "", c_isprint (ਆਖਰੀ) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
ਸਥਿਰ ਖਾਲੀ
list_entries_who (idx_t n, STRUCT_UTMP ਅਚਲ *utmp_buf)
{
  idx_t entries = 0;

  ਜਦੋਂ (n--)
    {
      ਜੇ (IS_USER_PROCESS (utmp_buf))
        {
          ਅੱਖਰ *trimmed_name;

          trimmed_name = ਕੱਢੋ_trimmed_name (utmp_buf);

          ਜੇ (entries)
            ਅੱਖਰ_ਲਿਖੋ (' ');
          fputs (trimmed_name, stdout);
          ਮੁਕਤ (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  ਲਿਖੋ (_("\n# users=%td\n"), entries);
}

ਸਥਿਰ ਖਾਲੀ
print_heading (ਖਾਲੀ)
{
  print_ਸਤਰ (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
ਸਥਿਰ ਖਾਲੀ
ਜਾਂਚ_entries (idx_t n, STRUCT_UTMP ਅਚਲ *utmp_buf)
{
  ਅੱਖਰ *ttyname_b IF_LINT ( = ਨਿਰਾ);
  time_t boottime = TYPE_MINIMUM (time_t);

  ਜੇ (include_heading)
    print_heading ();

  ਜੇ (my_ਸਤਰ_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      ਜੇ (!ttyname_b)
        ਵਾਪਸ;
      ਜੇ (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += ਆਕਾਰ "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  ਜਦੋਂ (n--)
    {
      ਜੇ (!my_ਸਤਰ_only
          || str_endswith (utmp_buf->ut_ਸਤਰ, ttyname_b))
        {
          ਜੇ (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          ਨਹੀਂਤਾਂ ਜੇ (need_runlevel && UT_TYPE_ਲੜੀਵਾਰ_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          ਨਹੀਂਤਾਂ ਜੇ (need_boottime && UT_TYPE_BOOT_ਸਮਾਂ (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          ਨਹੀਂਤਾਂ ਜੇ (need_clockchange && UT_TYPE_NEW_ਸਮਾਂ (utmp_buf))
            print_clockchange (utmp_buf);
          ਨਹੀਂਤਾਂ ਜੇ (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          ਨਹੀਂਤਾਂ ਜੇ (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          ਨਹੀਂਤਾਂ ਜੇ (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      ਜੇ (UT_TYPE_BOOT_ਸਮਾਂ (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
ਸਥਿਰ ਖਾਲੀ
who (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ਜੇ (short_list)
    options |= READ_UTMP_USER_PROCESS;
  ਜੇ (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  ਜੇ (short_list)
    list_entries_who (n_users, utmp_buf);
  ਨਹੀਂਤਾਂ
    ਜਾਂਚ_entries (n_users, utmp_buf);

  ਮੁਕਤ (utmp_buf);
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... [ FILE | ARG1 ARG2 ]\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Print information about users who are currently logged ਵਿੱਚ.\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -a, --all         same as -b -d --login -p -r -t -T -u\n\
"));
      oputs (_("\
  -b, --boot        ਸਮਾਂ ਦਾ ਆਖਰੀ system boot\n\
"));
      oputs (_("\
  -d, --dead        print dead processes\n\
"));
      oputs (_("\
  -H, --heading     print ਸਤਰ ਦਾ ਕਾਲਮ headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt ਮੰਜ਼ਿਲ canonicalize hostnames ਰਾਹੀਂ DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard ਨਿਵੇਸ਼\n\
"));
      oputs (_("\
  -p, --process     print ਸਰਗਰਮ processes spawned by init\n\
"));
      oputs (_("\
  -q, --ਗਿਣਤੀ_2       all login ਨਾਂ and ਸੰਖਿਆ ਦਾ users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --ਛੋਟਾ       print only name, ਸਤਰ, and ਸਮਾਂ (ਮੂਲ)\n\
"));
      oputs (_("\
  -t, --ਸਮਾਂ        print ਆਖਰੀ system clock change\n\
"));
      oputs (_("\
  -T, -w, --mesg    ਜੋੜੋ user's ਸੁਨੇਹਾ_2 ਸਥਿਤੀ as +, - or ?\n\
"));
      oputs (_("\
  -u, --users       list users logged ਵਿੱਚ, including idle ਸਮਾਂ\n\
"));
      oputs (_("\
      --ਸੁਨੇਹਾ_2     same as -T\n\
"));
      oputs (_("\
      --writable    same as -T\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      ਲਿਖੋ (_("\
\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
If ARG1 ARG2 given, -m presumed: 'am i' or 'mom likes' are usual.\n\
"), UTMP_FILE, WTMP_FILE);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'a':
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
          ਰੋਕੋ;

        ਹਾਲਤ 'b':
          need_boottime = true;
          assumptions = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'H':
          include_heading = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'm':
          my_ਸਤਰ_only = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'p':
          need_initspawn = true;
          assumptions = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'q':
          short_list = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          short_output = true;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          need_clockchange = true;
          assumptions = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'T':
        ਹਾਲਤ 'w':
          include_mesg = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          ਰੋਕੋ;

        ਹਾਲਤ LOOKUP_OPTION:
          do_lookup = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  ਜੇ (include_exit)
    {
      short_output = false;
    }

  ਜੇ (hard_locale (LC_ਸਮਾਂ))
    {
      ਸਮਾਂ_format = "%Y-%m-%d %H:%M";
      ਸਮਾਂ_format_ਚੌੜਾਈ = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ਨਹੀਂਤਾਂ
    {
      ਸਮਾਂ_format = "%b %e %H:%M";
      ਸਮਾਂ_format_ਚੌੜਾਈ = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ਚੋਣ (argc - optind)
    {
    ਹਾਲਤ 2:			/* who <blurf> <glop> */
      my_ਸਤਰ_only = true;
      FALLTHROUGH;
    ਹਾਲਤ -1:
    ਹਾਲਤ 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ਰੋਕੋ;

    ਹਾਲਤ 1:			/* who <utmp file> */
      who (argv[optind], 0);
      ਰੋਕੋ;

    ਮੂਲ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  ਵਾਪਸ EXIT_SUCCESS;
}
