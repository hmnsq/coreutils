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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>

#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "canon-host.h"
#ଅନ୍ତର୍ଭାଵ "readutmp.h"
#ଅନ୍ତର୍ଭାଵ "hard-locale.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "xvasprintf.h"

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
#ଅନ୍ୟଥା
# define UT_TYPE_RUN_LVL(U) false
#endif

#ifdef INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#ଅନ୍ୟଥା
# define UT_TYPE_INIT_PROCESS(U) false
#endif

#ifdef DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#ଅନ୍ୟଥା
# define UT_TYPE_DEAD_PROCESS(U) false
#endif

#ifdef NEW_TIME
# define UT_TYPE_NEW_TIME(U) ((U)->ut_type == NEW_TIME)
#ଅନ୍ୟଥା
# define UT_TYPE_NEW_TIME(U) false
#endif

#define IDLESTR_LEN 6

#ଯଦି HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  ଅକ୍ଷର Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (Utmp_ent->ut_pid))
#ଅନ୍ୟଥା
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  ଅକ୍ଷର ଅଚଳ *Var = ""
#endif

#ଯଦି HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#ଅନ୍ୟଥା
# define UT_ID(U) "??"
#endif

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ସ୍ଥିର bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
ସ୍ଥିର bool short_list;

/* If true, display only name, line, and time fields.  */
ସ୍ଥିର bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
ସ୍ଥିର bool include_idle;

/* If true, display a line at the top describing each field.  */
ସ୍ଥିର bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
ସ୍ଥିର bool include_mesg;

/* If true, display process termination & exit status.  */
ସ୍ଥିର bool include_exit;

/* If true, display the last boot time.  */
ସ୍ଥିର bool need_boottime;

/* If true, display dead processes.  */
ସ୍ଥିର bool need_deadprocs;

/* If true, display processes waiting for user login.  */
ସ୍ଥିର bool need_login;

/* If true, display processes started by init.  */
ସ୍ଥିର bool need_initspawn;

/* If true, display the last clock change.  */
ସ୍ଥିର bool need_clockchange;

/* If true, display the current runlevel.  */
ସ୍ଥିର bool need_runlevel;

/* If true, display user processes.  */
ସ୍ଥିର bool need_users;

/* If true, display info only for the controlling tty.  */
ସ୍ଥିର bool my_ଧାଡ଼ି_only;

/* The strftime format to use for login times, and its expected
   output width.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *time_format;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା time_format_width;

/* for long options with no corresponding short option, use enum */
ଗଣନା
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"all", no_argument, ଖାଲି, 'a'},
  {"boot", no_argument, ଖାଲି, 'b'},
  {"count", no_argument, ଖାଲି, 'q'},
  {"dead", no_argument, ଖାଲି, 'd'},
  {"heading", no_argument, ଖାଲି, 'H'},
  {"login", no_argument, ଖାଲି, 'l'},
  {"lookup", no_argument, ଖାଲି, LOOKUP_OPTION},
  {"message", no_argument, ଖାଲି, 'T'},
  {"mesg", no_argument, ଖାଲି, 'T'},
  {"process", no_argument, ଖାଲି, 'p'},
  {"runlevel", no_argument, ଖାଲି, 'r'},
  {"short", no_argument, ଖାଲି, 's'},
  {"time", no_argument, ଖାଲି, 't'},
  {"users", no_argument, ଖାଲି, 'u'},
  {"writable", no_argument, ଖାଲି, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
idle_string (time_t when, time_t boottime)
{
  ସ୍ଥିର time_t now = TYPE_MINIMUM (time_t);

  ଯଦି (now == TYPE_MINIMUM (time_t))
    time (&now);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା seconds_idle;
  ଯଦି (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      ଯଦି (seconds_idle < 60)
        ଫେରନ୍ତୁ "  .  ";
      ଅନ୍ୟଥା
        {
          ସ୍ଥିର ଅକ୍ଷର idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          ଫେରନ୍ତୁ idle_hhmm;
        }
    }

  ଫେରନ୍ତୁ _(" old ");
}

/* Return a time string.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
time_string (STRUCT_UTMP ଅଚଳ *utmp_ent)
{
  ସ୍ଥିର ଅକ୍ଷର buf[INT_STRLEN_BOUND (intmax_t) + ଆକାର "-%m-%d %H:%M"];
  ସଂରଚନା tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ଯଦି (tmp)
    {
      strftime (buf, ଆକାର buf, time_format, tmp);
      ଫେରନ୍ତୁ buf;
    }
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
ସ୍ଥିର ଶୂନ୍ୟ
print_ଧାଡ଼ି (ଅକ୍ଷର ଅଚଳ *user, ଅଚଳ ଅକ୍ଷର state,
            ଅକ୍ଷର ଅଚଳ *ଧାଡ଼ି,
            ଅକ୍ଷର ଅଚଳ *time_str, ଅକ୍ଷର ଅଚଳ *idle, ଅକ୍ଷର ଅଚଳ *pid,
            ଅକ୍ଷର ଅଚଳ *comment, ଅକ୍ଷର ଅଚଳ *exitstr)
{
  ସ୍ଥିର ଅକ୍ଷର mesg[3] = { ' ', 'x', '\0' };
  ଅକ୍ଷର *buf;
  ଅକ୍ଷର x_idle[1 + IDLESTR_LEN + 1];
  ଅକ୍ଷର x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  ଅକ୍ଷର *x_exitstr;

  mesg[1] = state;

  ଯଦି (include_idle && !short_output && strlen (idle) < ଆକାର x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  ଅନ୍ୟଥା
    *x_idle = '\0';

  ଯଦି (!short_output && strlen (pid) < ଆକାର x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  ଅନ୍ୟଥା
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, strlen (exitstr)) + 1 : 1);
  ଯଦି (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  ଅନ୍ୟଥା
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
                   ଧାଡ଼ି,
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
    ଅକ୍ଷର *p = buf + strlen (buf);
    ଯେତେବେଳେ (*--p == ' ')
      /* empty */;
    *(p + 1) = '\0';
  }

  puts (buf);
  ମୁକ୍ତ (buf);
  ମୁକ୍ତ (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
ସ୍ଥିର bool
is_tty_writable (ସଂରଚନା stat ଅଚଳ *pstat)
{
#ifdef TTY_GROUP_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  ସଂରଚନା group *ttygr = getgrnam (TTY_GROUP_NAME);
  ଯଦି (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    ଫେରନ୍ତୁ false;
#endif

  ଫେରନ୍ତୁ pstat->st_mode & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
ସ୍ଥିର ଶୂନ୍ୟ
print_user (STRUCT_UTMP ଅଚଳ *utmp_ent, time_t boottime)
{
  ସଂରଚନା stat stats;
  time_t last_change;
  ଅକ୍ଷର mesg;
  ଅକ୍ଷର idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  ସ୍ଥିର ଅକ୍ଷର *hoststr;
#ଯଦି HAVE_STRUCT_XTMP_UT_HOST
  ସ୍ଥିର idx_t hostlen;
#endif

  /* If ut_line contains a space, the device name starts after the space.  */
  ଅକ୍ଷର *ଧାଡ଼ି = utmp_ent->ut_ଧାଡ଼ି;
  ଅକ୍ଷର *space = strchr (ଧାଡ଼ି, ' ');
  ଧାଡ଼ି = space ? space + 1 : ଧାଡ଼ି;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd;
  ଯଦି (IS_ABSOLUTE_FILE_NAME (ଧାଡ଼ି))
    dirfd = AT_FDCWD;
  ଅନ୍ୟଥା
    {
      ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା dev_dirfd;
      ଯଦି (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ଯଦି (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ଯଦି (AT_FDCWD <= dirfd && fstatat (dirfd, ଧାଡ଼ି, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      last_change = stats.st_atime;
    }
  ଅନ୍ୟଥା
    {
      mesg = '?';
      last_change = 0;
    }

  ଯଦି (last_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (last_change, boottime));
  ଅନ୍ୟଥା
    sprintf (idlestr, "  ?");

#ଯଦି HAVE_STRUCT_XTMP_UT_HOST
  ଯଦି (utmp_ent->ut_host[0])
    {
      ଅକ୍ଷର *host = ଖାଲି;
      ଅକ୍ଷର *display = ଖାଲି;
      ଅକ୍ଷର *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      ଯଦି (display)
        *display++ = '\0';

      ଯଦି (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      ଯଦି (! host)
        host = ut_host;

      ଯଦି (display)
        {
          idx_t needed = strlen (host) + strlen (display) + 4;
          ଯଦି (hostlen < needed)
            {
              ମୁକ୍ତ (hoststr);
              hoststr = xpalloc (ଖାଲି, &hostlen, needed - hostlen, -1, 1);
            }
          ଅକ୍ଷର *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      ଅନ୍ୟଥା
        {
          idx_t needed = strlen (host) + 3;
          ଯଦି (hostlen < needed)
            {
              ମୁକ୍ତ (hoststr);
              hoststr = xpalloc (ଖାଲି, &hostlen, needed - hostlen, -1, 1);
            }
          ଅକ୍ଷର *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      ଯଦି (host != ut_host)
        ମୁକ୍ତ (host);
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#endif

  print_ଧାଡ଼ି (utmp_ent->ut_user, mesg,
              utmp_ent->ut_ଧାଡ଼ି,
              time_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

ସ୍ଥିର ଶୂନ୍ୟ
print_boottime (STRUCT_UTMP ଅଚଳ *utmp_ent)
{
  print_ଧାଡ଼ି ("", ' ', _("system boot"),
              time_string (utmp_ent), "", "", "", "");
}

ସ୍ଥିର ଅକ୍ଷର *
make_id_equals_comment (STRUCT_UTMP ଅଚଳ *utmp_ent)
{
  ଅକ୍ଷର ଅଚଳ *id = UT_ID (utmp_ent);
  idx_t idlen = strlen (id);
  ଅକ୍ଷର ଅଚଳ *prefix = _("id=");
  idx_t prefixlen = strlen (prefix);
  ଅକ୍ଷର *comment = xmalloc (prefixlen + idlen + 1);
  ଅକ୍ଷର *p = mempcpy (comment, prefix, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  ଫେରନ୍ତୁ comment;
}

ସ୍ଥିର ଶୂନ୍ୟ
print_deadprocs (STRUCT_UTMP ଅଚଳ *utmp_ent)
{
  ସ୍ଥିର ଅକ୍ଷର *exitstr;
  ଅକ୍ଷର *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  ଯଦି (!exitstr)
    exitstr = xmalloc (strlen (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + strlen (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_ଧାଡ଼ି ("", ' ', utmp_ent->ut_ଧାଡ଼ି,
              time_string (utmp_ent), "", pidstr, comment, exitstr);
  ମୁକ୍ତ (comment);
}

ସ୍ଥିର ଶୂନ୍ୟ
print_login (STRUCT_UTMP ଅଚଳ *utmp_ent)
{
  ଅକ୍ଷର *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_ଧାଡ଼ି (_("LOGIN"), ' ', utmp_ent->ut_ଧାଡ଼ି,
              time_string (utmp_ent), "", pidstr, comment, "");
  ମୁକ୍ତ (comment);
}

ସ୍ଥିର ଶୂନ୍ୟ
print_initspawn (STRUCT_UTMP ଅଚଳ *utmp_ent)
{
  ଅକ୍ଷର *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_ଧାଡ଼ି ("", ' ', utmp_ent->ut_ଧାଡ଼ି,
              time_string (utmp_ent), "", pidstr, comment, "");
  ମୁକ୍ତ (comment);
}

ସ୍ଥିର ଶୂନ୍ୟ
print_clockchange (STRUCT_UTMP ଅଚଳ *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_ଧାଡ଼ି ("", ' ', _("clock change"),
              time_string (utmp_ent), "", "", "", "");
}

ସ୍ଥିର ଶୂନ୍ୟ
print_runlevel (STRUCT_UTMP ଅଚଳ *utmp_ent)
{
  ସ୍ଥିର ଅକ୍ଷର *runlevline, *comment;
  unsigned ଅକ୍ଷର last = utmp_ent->ut_pid / 256;
  unsigned ଅକ୍ଷର curr = utmp_ent->ut_pid % 256;

  ଯଦି (!runlevline)
    runlevline = xmalloc (strlen (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  ଯଦି (!comment)
    comment = xmalloc (strlen (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (last == 'N') ? 'S' : last);

  print_ଧାଡ଼ି ("", ' ', runlevline, time_string (utmp_ent),
              "", "", c_isprint (last) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
ସ୍ଥିର ଶୂନ୍ୟ
list_entries_who (idx_t n, STRUCT_UTMP ଅଚଳ *utmp_buf)
{
  idx_t entries = 0;

  ଯେତେବେଳେ (n--)
    {
      ଯଦି (IS_USER_PROCESS (utmp_buf))
        {
          ଅକ୍ଷର *trimmed_name;

          trimmed_name = extract_trimmed_name (utmp_buf);

          ଯଦି (entries)
            putchar (' ');
          fputs (trimmed_name, stdout);
          ମୁକ୍ତ (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  printf (_("\n# users=%td\n"), entries);
}

ସ୍ଥିର ଶୂନ୍ୟ
print_heading (ଶୂନ୍ୟ)
{
  print_ଧାଡ଼ି (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
ସ୍ଥିର ଶୂନ୍ୟ
scan_entries (idx_t n, STRUCT_UTMP ଅଚଳ *utmp_buf)
{
  ଅକ୍ଷର *ttyname_b IF_LINT ( = ଖାଲି);
  time_t boottime = TYPE_MINIMUM (time_t);

  ଯଦି (include_heading)
    print_heading ();

  ଯଦି (my_ଧାଡ଼ି_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      ଯଦି (!ttyname_b)
        ଫେରନ୍ତୁ;
      ଯଦି (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += ଆକାର "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  ଯେତେବେଳେ (n--)
    {
      ଯଦି (!my_ଧାଡ଼ି_only
          || str_endswith (utmp_buf->ut_ଧାଡ଼ି, ttyname_b))
        {
          ଯଦି (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          ଅନ୍ୟଥା ଯଦି (need_runlevel && UT_TYPE_RUN_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          ଅନ୍ୟଥା ଯଦି (need_boottime && UT_TYPE_BOOT_TIME (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          ଅନ୍ୟଥା ଯଦି (need_clockchange && UT_TYPE_NEW_TIME (utmp_buf))
            print_clockchange (utmp_buf);
          ଅନ୍ୟଥା ଯଦି (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          ଅନ୍ୟଥା ଯଦି (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          ଅନ୍ୟଥା ଯଦି (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      ଯଦି (UT_TYPE_BOOT_TIME (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
ସ୍ଥିର ଶୂନ୍ୟ
who (ଅକ୍ଷର ଅଚଳ *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ଯଦି (short_list)
    options |= READ_UTMP_USER_PROCESS;
  ଯଦି (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  ଯଦି (short_list)
    list_entries_who (n_users, utmp_buf);
  ଅନ୍ୟଥା
    scan_entries (n_users, utmp_buf);

  ମୁକ୍ତ (utmp_buf);
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
  -H, --heading     print ଧାଡ଼ି of column headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt to canonicalize hostnames via DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard ନିବେଶ\n\
"));
      oputs (_("\
  -p, --process     print active processes spawned by init\n\
"));
      oputs (_("\
  -q, --ଗଣନା_2       all login names and ସଂଖ୍ୟା of users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --short       print only name, ଧାଡ଼ି, and time (ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\n\
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, ଖାଲି))
         != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'a':
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
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'b':
          need_boottime = true;
          assumptions = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'H':
          include_heading = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'm':
          my_ଧାଡ଼ି_only = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'p':
          need_initspawn = true;
          assumptions = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'q':
          short_list = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 's':
          short_output = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 't':
          need_clockchange = true;
          assumptions = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'T':
        ସ୍ଥିତି 'w':
          include_mesg = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି LOOKUP_OPTION:
          do_lookup = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  ଯଦି (include_exit)
    {
      short_output = false;
    }

  ଯଦି (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ଅନ୍ୟଥା
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ବାଛ (argc - optind)
    {
    ସ୍ଥିତି 2:			/* who <blurf> <glop> */
      my_ଧାଡ଼ି_only = true;
      FALLTHROUGH;
    ସ୍ଥିତି -1:
    ସ୍ଥିତି 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 1:			/* who <utmp file> */
      who (argv[optind], 0);
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
