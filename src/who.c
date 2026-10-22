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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TTY_ಗುಂಪು_NAME
# include <grp.h>
#ಆದರೆ_ಅಂತ್ಯ

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "who"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Michael Stone")

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಸತತ_LVL
# define UT_TYPE_ಸತತ_LVL(U) ((U)->ut_type == ಸತತ_LVL)
#ಇಲ್ಲದಿದ್ದರೆ
# define UT_TYPE_ಸತತ_LVL(U) false
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#ಇಲ್ಲದಿದ್ದರೆ
# define UT_TYPE_INIT_PROCESS(U) false
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#ಇಲ್ಲದಿದ್ದರೆ
# define UT_TYPE_DEAD_PROCESS(U) false
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NEW_ಸಮಯ
# define UT_TYPE_NEW_ಸಮಯ(U) ((U)->ut_type == NEW_ಸಮಯ)
#ಇಲ್ಲದಿದ್ದರೆ
# define UT_TYPE_NEW_ಸಮಯ(U) false
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು IDLESTR_LEN 6

#ಆದರೆ HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  ಅಕ್ಷರ Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) (Utmp_ent->ut_pid))
#ಇಲ್ಲದಿದ್ದರೆ
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *Var = ""
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#ಇಲ್ಲದಿದ್ದರೆ
# define UT_ID(U) "??"
#ಆದರೆ_ಅಂತ್ಯ

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
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಮಯ_format;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಮಯ_format_ಅಗಲ;

/* for long options with no corresponding short option, use enum */
ಎಣಿಕೆ
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"all", no_ವಾದ, ಖಾಲಿ, 'a'},
  {"boot", no_ವಾದ, ಖಾಲಿ, 'b'},
  {"count", no_ವಾದ, ಖಾಲಿ, 'q'},
  {"dead", no_ವಾದ, ಖಾಲಿ, 'd'},
  {"heading", no_ವಾದ, ಖಾಲಿ, 'H'},
  {"login", no_ವಾದ, ಖಾಲಿ, 'l'},
  {"lookup", no_ವಾದ, ಖಾಲಿ, LOOKUP_OPTION},
  {"message", no_ವಾದ, ಖಾಲಿ, 'T'},
  {"mesg", no_ವಾದ, ಖಾಲಿ, 'T'},
  {"process", no_ವಾದ, ಖಾಲಿ, 'p'},
  {"runlevel", no_ವಾದ, ಖಾಲಿ, 'r'},
  {"short", no_ವಾದ, ಖಾಲಿ, 's'},
  {"time", no_ವಾದ, ಖಾಲಿ, 't'},
  {"users", no_ವಾದ, ಖಾಲಿ, 'u'},
  {"writable", no_ವಾದ, ಖಾಲಿ, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
idle_string (time_t when, time_t boottime)
{
  ಸ್ಥಿರ time_t now = TYPE_MINIMUM (time_t);

  ಆದರೆ (now == TYPE_MINIMUM (time_t))
    ಸಮಯ (&now);

  ಪೂರ್ಣಾಂಕ seconds_idle;
  ಆದರೆ (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      ಆದರೆ (seconds_idle < 60)
        ಹಿಂತಿರುಗಿ "  .  ";
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಸ್ಥಿರ ಅಕ್ಷರ idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          ಹಿಂತಿರುಗಿ idle_hhmm;
        }
    }

  ಹಿಂತಿರುಗಿ _(" old ");
}

/* Return a time string.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
ಸಮಯ_string (STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_ent)
{
  ಸ್ಥಿರ ಅಕ್ಷರ buf[INT_STRLEN_BOUND (intmax_t) + ಗಾತ್ರ "-%m-%d %H:%M"];
  ರಚನೆ tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ಆದರೆ (tmp)
    {
      strftime (buf, ಗಾತ್ರ buf, ಸಮಯ_format, tmp);
      ಹಿಂತಿರುಗಿ buf;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
ಸ್ಥಿರ ಶೂನ್ಯ
print_ಸಾಲು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *user, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ state,
            ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಾಲು,
            ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಮಯ_str, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *idle, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *pid,
            ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *comment, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *exitstr)
{
  ಸ್ಥಿರ ಅಕ್ಷರ mesg[3] = { ' ', 'x', '\0' };
  ಅಕ್ಷರ *buf;
  ಅಕ್ಷರ x_idle[1 + IDLESTR_LEN + 1];
  ಅಕ್ಷರ x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  ಅಕ್ಷರ *x_exitstr;

  mesg[1] = state;

  ಆದರೆ (include_idle && !short_output && ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (idle) < ಗಾತ್ರ x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  ಇಲ್ಲದಿದ್ದರೆ
    *x_idle = '\0';

  ಆದರೆ (!short_output && ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (pid) < ಗಾತ್ರ x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  ಇಲ್ಲದಿದ್ದರೆ
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (exitstr)) + 1 : 1);
  ಆದರೆ (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  ಇಲ್ಲದಿದ್ದರೆ
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
                   ಸಮಯ_format_ಅಗಲ,
                   ಸಮಯ_str,
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
    ಅಕ್ಷರ *p = buf + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (buf);
    ಆಗ (*--p == ' ')
      /*ಖಾಲಿಗಳು*/;
    *(p + 1) = '\0';
  }

  puts (buf);
  ಬಿಡುಗಡೆ (buf);
  ಬಿಡುಗಡೆ (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
ಸ್ಥಿರ bool
is_tty_writable (ರಚನೆ stat ಸ್ಥಿರಾಂಕ *pstat)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TTY_ಗುಂಪು_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  ರಚನೆ ಗುಂಪು *ttygr = getgrnam (TTY_ಗುಂಪು_NAME);
  ಆದರೆ (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    ಹಿಂತಿರುಗಿ false;
#ಆದರೆ_ಅಂತ್ಯ

  ಹಿಂತಿರುಗಿ pstat->st_ಬಹುಲಕ & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
ಸ್ಥಿರ ಶೂನ್ಯ
print_user (STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_ent, time_t boottime)
{
  ರಚನೆ stat stats;
  time_t ಕೊನೆಯ_change;
  ಅಕ್ಷರ mesg;
  ಅಕ್ಷರ idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  ಸ್ಥಿರ ಅಕ್ಷರ *hoststr;
#ಆದರೆ HAVE_STRUCT_XTMP_UT_HOST
  ಸ್ಥಿರ idx_t hostlen;
#ಆದರೆ_ಅಂತ್ಯ

  /* If ut_line contains a space, the device name starts after the space.  */
  ಅಕ್ಷರ *ಸಾಲು = utmp_ent->ut_ಸಾಲು;
  ಅಕ್ಷರ *space = strchr (ಸಾಲು, ' ');
  ಸಾಲು = space ? space + 1 : ಸಾಲು;

  ಪೂರ್ಣಾಂಕ dirfd;
  ಆದರೆ (IS_ABSOLUTE_FILE_NAME (ಸಾಲು))
    dirfd = AT_FDCWD;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dev_dirfd;
      ಆದರೆ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ಆದರೆ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ಆದರೆ (AT_FDCWD <= dirfd && fstatat (dirfd, ಸಾಲು, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      ಕೊನೆಯ_change = stats.st_atime;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      mesg = '?';
      ಕೊನೆಯ_change = 0;
    }

  ಆದರೆ (ಕೊನೆಯ_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (ಕೊನೆಯ_change, boottime));
  ಇಲ್ಲದಿದ್ದರೆ
    sprintf (idlestr, "  ?");

#ಆದರೆ HAVE_STRUCT_XTMP_UT_HOST
  ಆದರೆ (utmp_ent->ut_host[0])
    {
      ಅಕ್ಷರ *host = ಖಾಲಿ;
      ಅಕ್ಷರ *display = ಖಾಲಿ;
      ಅಕ್ಷರ *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      ಆದರೆ (display)
        *display++ = '\0';

      ಆದರೆ (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      ಆದರೆ (! host)
        host = ut_host;

      ಆದರೆ (display)
        {
          idx_t needed = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (host) + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (display) + 4;
          ಆದರೆ (hostlen < needed)
            {
              ಬಿಡುಗಡೆ (hoststr);
              hoststr = xpalloc (ಖಾಲಿ, &hostlen, needed - hostlen, -1, 1);
            }
          ಅಕ್ಷರ *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          idx_t needed = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (host) + 3;
          ಆದರೆ (hostlen < needed)
            {
              ಬಿಡುಗಡೆ (hoststr);
              hoststr = xpalloc (ಖಾಲಿ, &hostlen, needed - hostlen, -1, 1);
            }
          ಅಕ್ಷರ *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      ಆದರೆ (host != ut_host)
        ಬಿಡುಗಡೆ (host);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#ಆದರೆ_ಅಂತ್ಯ

  print_ಸಾಲು (utmp_ent->ut_user, mesg,
              utmp_ent->ut_ಸಾಲು,
              ಸಮಯ_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_boottime (STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_ent)
{
  print_ಸಾಲು ("", ' ', _("system boot"),
              ಸಮಯ_string (utmp_ent), "", "", "", "");
}

ಸ್ಥಿರ ಅಕ್ಷರ *
make_id_equals_comment (STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_ent)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *id = UT_ID (utmp_ent);
  idx_t idlen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (id);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪೂರ್ವಪ್ರತ್ಯಯ = _("id=");
  idx_t prefixlen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಪೂರ್ವಪ್ರತ್ಯಯ);
  ಅಕ್ಷರ *comment = xmalloc (prefixlen + idlen + 1);
  ಅಕ್ಷರ *p = mempcpy (comment, ಪೂರ್ವಪ್ರತ್ಯಯ, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  ಹಿಂತಿರುಗಿ comment;
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_deadprocs (STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_ent)
{
  ಸ್ಥಿರ ಅಕ್ಷರ *exitstr;
  ಅಕ್ಷರ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  ಆದರೆ (!exitstr)
    exitstr = xmalloc (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_ಸಾಲು ("", ' ', utmp_ent->ut_ಸಾಲು,
              ಸಮಯ_string (utmp_ent), "", pidstr, comment, exitstr);
  ಬಿಡುಗಡೆ (comment);
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_login (STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_ent)
{
  ಅಕ್ಷರ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_ಸಾಲು (_("LOGIN"), ' ', utmp_ent->ut_ಸಾಲು,
              ಸಮಯ_string (utmp_ent), "", pidstr, comment, "");
  ಬಿಡುಗಡೆ (comment);
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_initspawn (STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_ent)
{
  ಅಕ್ಷರ *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_ಸಾಲು ("", ' ', utmp_ent->ut_ಸಾಲು,
              ಸಮಯ_string (utmp_ent), "", pidstr, comment, "");
  ಬಿಡುಗಡೆ (comment);
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_clockchange (STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_ಸಾಲು ("", ' ', _("clock change"),
              ಸಮಯ_string (utmp_ent), "", "", "", "");
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_runlevel (STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_ent)
{
  ಸ್ಥಿರ ಅಕ್ಷರ *runlevline, *comment;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಕೊನೆಯ = utmp_ent->ut_pid / 256;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ curr = utmp_ent->ut_pid % 256;

  ಆದರೆ (!runlevline)
    runlevline = xmalloc (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  ಆದರೆ (!comment)
    comment = xmalloc (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (ಕೊನೆಯ == 'N') ? 'S' : ಕೊನೆಯ);

  print_ಸಾಲು ("", ' ', runlevline, ಸಮಯ_string (utmp_ent),
              "", "", c_isprint (ಕೊನೆಯ) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
ಸ್ಥಿರ ಶೂನ್ಯ
list_entries_who (idx_t n, STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_buf)
{
  idx_t entries = 0;

  ಆಗ (n--)
    {
      ಆದರೆ (IS_USER_PROCESS (utmp_buf))
        {
          ಅಕ್ಷರ *trimmed_name;

          trimmed_name = ಹೊರತೆಗೆ_trimmed_name (utmp_buf);

          ಆದರೆ (entries)
            ಅಕ್ಷರ_ಬರೆ (' ');
          fputs (trimmed_name, stdout);
          ಬಿಡುಗಡೆ (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  ಬರೆ (_("\n# users=%td\n"), entries);
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_heading (ಶೂನ್ಯ)
{
  print_ಸಾಲು (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
ಸ್ಥಿರ ಶೂನ್ಯ
ಪರಿಶೀಲನೆ_entries (idx_t n, STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_buf)
{
  ಅಕ್ಷರ *ttyname_b IF_LINT ( = ಖಾಲಿ);
  time_t boottime = TYPE_MINIMUM (time_t);

  ಆದರೆ (include_heading)
    print_heading ();

  ಆದರೆ (my_ಸಾಲು_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      ಆದರೆ (!ttyname_b)
        ಹಿಂತಿರುಗಿ;
      ಆದರೆ (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += ಗಾತ್ರ "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  ಆಗ (n--)
    {
      ಆದರೆ (!my_ಸಾಲು_only
          || str_endswith (utmp_buf->ut_ಸಾಲು, ttyname_b))
        {
          ಆದರೆ (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (need_runlevel && UT_TYPE_ಸತತ_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (need_boottime && UT_TYPE_BOOT_ಸಮಯ (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (need_clockchange && UT_TYPE_NEW_ಸಮಯ (utmp_buf))
            print_clockchange (utmp_buf);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      ಆದರೆ (UT_TYPE_BOOT_ಸಮಯ (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
ಸ್ಥಿರ ಶೂನ್ಯ
who (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ಆದರೆ (short_list)
    options |= READ_UTMP_USER_PROCESS;
  ಆದರೆ (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  ಆದರೆ (short_list)
    list_entries_who (n_users, utmp_buf);
  ಇಲ್ಲದಿದ್ದರೆ
    ಪರಿಶೀಲನೆ_entries (n_users, utmp_buf);

  ಬಿಡುಗಡೆ (utmp_buf);
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... [ FILE | ARG1 ARG2 ]\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print information about users who are currently logged ಒಳ.\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -a, --all         same as -b -d --login -p -r -t -T -u\n\
"));
      oputs (_("\
  -b, --boot        ಸಮಯ ದ ಕೊನೆಯ system boot\n\
"));
      oputs (_("\
  -d, --dead        print dead processes\n\
"));
      oputs (_("\
  -H, --heading     print ಸಾಲು ದ ಸ್ತಂಭ headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt ಗಮ್ಯ canonicalize hostnames ಮೂಲಕ DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard ಒಳಹರಿವು\n\
"));
      oputs (_("\
  -p, --process     print ಸಕ್ರಿಯ processes spawned by init\n\
"));
      oputs (_("\
  -q, --ಎಣಿಕೆ_2       all login ಹೆಸರುಗಳು and ಸಂಖ್ಯೆ ದ users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --ಗಿಡ್ಡ       print only name, ಸಾಲು, and ಸಮಯ (ಪೂರ್ವನಿಯೋಜಿತ)\n\
"));
      oputs (_("\
  -t, --ಸಮಯ        print ಕೊನೆಯ system clock change\n\
"));
      oputs (_("\
  -T, -w, --mesg    ಕೂಡಿಸು user's ಸಂದೇಶ_2 ಸ್ಥಿತಿ_2 as +, - or ?\n\
"));
      oputs (_("\
  -u, --users       list users logged ಒಳ, including idle ಸಮಯ\n\
"));
      oputs (_("\
      --ಸಂದೇಶ_2     same as -T\n\
"));
      oputs (_("\
      --writable    same as -T\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      ಬರೆ (_("\
\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
If ARG1 ARG2 given, -m presumed: 'am i' or 'mom likes' are usual.\n\
"), UTMP_FILE, WTMP_FILE);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'a':
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
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'b':
          need_boottime = true;
          assumptions = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'H':
          include_heading = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'm':
          my_ಸಾಲು_only = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'p':
          need_initspawn = true;
          assumptions = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'q':
          short_list = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          short_output = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          need_clockchange = true;
          assumptions = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'T':
        ಸ್ಥಿತಿ 'w':
          include_mesg = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LOOKUP_OPTION:
          do_lookup = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  ಆದರೆ (include_exit)
    {
      short_output = false;
    }

  ಆದರೆ (hard_locale (LC_ಸಮಯ))
    {
      ಸಮಯ_format = "%Y-%m-%d %H:%M";
      ಸಮಯ_format_ಅಗಲ = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸಮಯ_format = "%b %e %H:%M";
      ಸಮಯ_format_ಅಗಲ = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ಆಯ್ಕೆ (argc - optind)
    {
    ಸ್ಥಿತಿ 2:			/* who <blurf> <glop> */
      my_ಸಾಲು_only = true;
      FALLTHROUGH;
    ಸ್ಥಿತಿ -1:
    ಸ್ಥಿತಿ 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 1:			/* who <utmp file> */
      who (argv[optind], 0);
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
