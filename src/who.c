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

#include <config.h>
#include <getopt.h>
#include <stdio.h>

#include <sys/types.h>
#include "system.h"

#include "c-ctype.h"
#include "canon-host.h"
#include "readutmp.h"
#include "hard-locale.h"
#include "quote.h"
#include "xvasprintf.h"

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
#ᯀᯮᯝᯂᯪ
# define UT_TYPE_RUN_LVL(U) false
#endif

#ifdef INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#ᯀᯮᯝᯂᯪ
# define UT_TYPE_INIT_PROCESS(U) false
#endif

#ifdef DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#ᯀᯮᯝᯂᯪ
# define UT_TYPE_DEAD_PROCESS(U) false
#endif

#ifdef NEW_TIME
# define UT_TYPE_NEW_TIME(U) ((U)->ut_type == NEW_TIME)
#ᯀᯮᯝᯂᯪ
# define UT_TYPE_NEW_TIME(U) false
#endif

#define IDLESTR_LEN 6

#ᯀᯮᯝ HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  var_na Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (long puur_naa_nka) (Utmp_ent->ut_pid))
#ᯀᯮᯝᯂᯪ
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  var_na const *Var = ""
#endif

#ᯀᯮᯝ HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#ᯀᯮᯝᯂᯪ
# define UT_ID(U) "??"
#endif

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
static bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
static bool short_list;

/* If true, display only name, line, and time fields.  */
static bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
static bool include_idle;

/* If true, display a line at the top describing each field.  */
static bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
static bool include_mesg;

/* If true, display process termination & exit status.  */
static bool include_exit;

/* If true, display the last boot time.  */
static bool need_boottime;

/* If true, display dead processes.  */
static bool need_deadprocs;

/* If true, display processes waiting for user login.  */
static bool need_login;

/* If true, display processes started by init.  */
static bool need_initspawn;

/* If true, display the last clock change.  */
static bool need_clockchange;

/* If true, display the current runlevel.  */
static bool need_runlevel;

/* If true, display user processes.  */
static bool need_users;

/* If true, display info only for the controlling tty.  */
static bool my_line_only;

/* The strftime format to use for login times, and its expected
   output width.  */
static var_na const *time_format;
static puur_naa_nka time_format_width;

/* for long options with no corresponding short option, use enum */
ga_nanaa
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

static sa_nracanaa option const longopts[] =
{
  {"all", no_argument, NULL, 'a'},
  {"boot", no_argument, NULL, 'b'},
  {"count", no_argument, NULL, 'q'},
  {"dead", no_argument, NULL, 'd'},
  {"heading", no_argument, NULL, 'H'},
  {"login", no_argument, NULL, 'l'},
  {"lookup", no_argument, NULL, LOOKUP_OPTION},
  {"message", no_argument, NULL, 'T'},
  {"mesg", no_argument, NULL, 'T'},
  {"process", no_argument, NULL, 'p'},
  {"runlevel", no_argument, NULL, 'r'},
  {"short", no_argument, NULL, 's'},
  {"time", no_argument, NULL, 't'},
  {"users", no_argument, NULL, 'u'},
  {"writable", no_argument, NULL, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
static var_na const *
idle_string (time_t when, time_t boottime)
{
  static time_t now = TYPE_MINIMUM (time_t);

  ᯀᯮᯝ (now == TYPE_MINIMUM (time_t))
    time (&now);

  puur_naa_nka seconds_idle;
  ᯀᯮᯝ (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      ᯀᯮᯝ (seconds_idle < 60)
        ᯀᯮᯞ "  .  ";
      ᯀᯮᯝᯂᯪ
        {
          static var_na idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          ᯀᯮᯞ idle_hhmm;
        }
    }

  ᯀᯮᯞ _(" old ");
}

/* Return a time string.  */
static var_na const *
time_string (STRUCT_UTMP const *utmp_ent)
{
  static var_na buf[INT_STRLEN_BOUND (intmax_t) + parimaa_na "-%m-%d %H:%M"];
  sa_nracanaa tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ᯀᯮᯝ (tmp)
    {
      strftime (buf, parimaa_na buf, time_format, tmp);
      ᯀᯮᯞ buf;
    }
  ᯀᯮᯝᯂᯪ
    ᯀᯮᯞ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
static void
print_line (var_na const *user, const var_na state,
            var_na const *line,
            var_na const *time_str, var_na const *idle, var_na const *pid,
            var_na const *comment, var_na const *exitstr)
{
  static var_na mesg[3] = { ' ', 'x', '\0' };
  var_na *buf;
  var_na x_idle[1 + IDLESTR_LEN + 1];
  var_na x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  var_na *x_exitstr;

  mesg[1] = state;

  ᯀᯮᯝ (include_idle && !short_output && strlen (idle) < parimaa_na x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  ᯀᯮᯝᯂᯪ
    *x_idle = '\0';

  ᯀᯮᯝ (!short_output && strlen (pid) < parimaa_na x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  ᯀᯮᯝᯂᯪ
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, strlen (exitstr)) + 1 : 1);
  ᯀᯮᯝ (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  ᯀᯮᯝᯂᯪ
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
                   line,
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
    var_na *p = buf + strlen (buf);
    ᯀᯬᯝ (*--p == ' ')
      /* empty */;
    *(p + 1) = '\0';
  }

  puts (buf);
  free (buf);
  free (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
static bool
is_tty_writable (sa_nracanaa stat const *pstat)
{
#ifdef TTY_GROUP_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  sa_nracanaa group *ttygr = getgrnam (TTY_GROUP_NAME);
  ᯀᯮᯝ (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    ᯀᯮᯞ false;
#endif

  ᯀᯮᯞ pstat->st_mode & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
static void
print_user (STRUCT_UTMP const *utmp_ent, time_t boottime)
{
  sa_nracanaa stat stats;
  time_t last_change;
  var_na mesg;
  var_na idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  static var_na *hoststr;
#ᯀᯮᯝ HAVE_STRUCT_XTMP_UT_HOST
  static idx_t hostlen;
#endif

  /* If ut_line contains a space, the device name starts after the space.  */
  var_na *line = utmp_ent->ut_line;
  var_na *space = strchr (line, ' ');
  line = space ? space + 1 : line;

  puur_naa_nka dirfd;
  ᯀᯮᯝ (IS_ABSOLUTE_FILE_NAME (line))
    dirfd = AT_FDCWD;
  ᯀᯮᯝᯂᯪ
    {
      static puur_naa_nka dev_dirfd;
      ᯀᯮᯝ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ᯀᯮᯝ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ᯀᯮᯝ (AT_FDCWD <= dirfd && fstatat (dirfd, line, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      last_change = stats.st_atime;
    }
  ᯀᯮᯝᯂᯪ
    {
      mesg = '?';
      last_change = 0;
    }

  ᯀᯮᯝ (last_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (last_change, boottime));
  ᯀᯮᯝᯂᯪ
    sprintf (idlestr, "  ?");

#ᯀᯮᯝ HAVE_STRUCT_XTMP_UT_HOST
  ᯀᯮᯝ (utmp_ent->ut_host[0])
    {
      var_na *host = NULL;
      var_na *display = NULL;
      var_na *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      ᯀᯮᯝ (display)
        *display++ = '\0';

      ᯀᯮᯝ (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      ᯀᯮᯝ (! host)
        host = ut_host;

      ᯀᯮᯝ (display)
        {
          idx_t needed = strlen (host) + strlen (display) + 4;
          ᯀᯮᯝ (hostlen < needed)
            {
              free (hoststr);
              hoststr = xpalloc (NULL, &hostlen, needed - hostlen, -1, 1);
            }
          var_na *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      ᯀᯮᯝᯂᯪ
        {
          idx_t needed = strlen (host) + 3;
          ᯀᯮᯝ (hostlen < needed)
            {
              free (hoststr);
              hoststr = xpalloc (NULL, &hostlen, needed - hostlen, -1, 1);
            }
          var_na *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      ᯀᯮᯝ (host != ut_host)
        free (host);
    }
  ᯀᯮᯝᯂᯪ
    {
      ᯀᯮᯝ (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#endif

  print_line (utmp_ent->ut_user, mesg,
              utmp_ent->ut_line,
              time_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

static void
print_boottime (STRUCT_UTMP const *utmp_ent)
{
  print_line ("", ' ', _("system boot"),
              time_string (utmp_ent), "", "", "", "");
}

static var_na *
make_id_equals_comment (STRUCT_UTMP const *utmp_ent)
{
  var_na const *id = UT_ID (utmp_ent);
  idx_t idlen = strlen (id);
  var_na const *prefix = _("id=");
  idx_t prefixlen = strlen (prefix);
  var_na *comment = xmalloc (prefixlen + idlen + 1);
  var_na *p = mempcpy (comment, prefix, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  ᯀᯮᯞ comment;
}

static void
print_deadprocs (STRUCT_UTMP const *utmp_ent)
{
  static var_na *exitstr;
  var_na *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  ᯀᯮᯝ (!exitstr)
    exitstr = xmalloc (strlen (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + strlen (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_line ("", ' ', utmp_ent->ut_line,
              time_string (utmp_ent), "", pidstr, comment, exitstr);
  free (comment);
}

static void
print_login (STRUCT_UTMP const *utmp_ent)
{
  var_na *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_line (_("LOGIN"), ' ', utmp_ent->ut_line,
              time_string (utmp_ent), "", pidstr, comment, "");
  free (comment);
}

static void
print_initspawn (STRUCT_UTMP const *utmp_ent)
{
  var_na *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_line ("", ' ', utmp_ent->ut_line,
              time_string (utmp_ent), "", pidstr, comment, "");
  free (comment);
}

static void
print_clockchange (STRUCT_UTMP const *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_line ("", ' ', _("clock change"),
              time_string (utmp_ent), "", "", "", "");
}

static void
print_runlevel (STRUCT_UTMP const *utmp_ent)
{
  static var_na *runlevline, *comment;
  unsigned var_na last = utmp_ent->ut_pid / 256;
  unsigned var_na curr = utmp_ent->ut_pid % 256;

  ᯀᯮᯝ (!runlevline)
    runlevline = xmalloc (strlen (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  ᯀᯮᯝ (!comment)
    comment = xmalloc (strlen (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (last == 'N') ? 'S' : last);

  print_line ("", ' ', runlevline, time_string (utmp_ent),
              "", "", c_isprint (last) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
static void
list_entries_who (idx_t n, STRUCT_UTMP const *utmp_buf)
{
  idx_t entries = 0;

  ᯀᯬᯝ (n--)
    {
      ᯀᯮᯝ (IS_USER_PROCESS (utmp_buf))
        {
          var_na *trimmed_name;

          trimmed_name = extract_trimmed_name (utmp_buf);

          ᯀᯮᯝ (entries)
            putchar (' ');
          fputs (trimmed_name, stdout);
          free (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  printf (_("\n# users=%td\n"), entries);
}

static void
print_heading (void)
{
  print_line (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
static void
scan_entries (idx_t n, STRUCT_UTMP const *utmp_buf)
{
  var_na *ttyname_b IF_LINT ( = NULL);
  time_t boottime = TYPE_MINIMUM (time_t);

  ᯀᯮᯝ (include_heading)
    print_heading ();

  ᯀᯮᯝ (my_line_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      ᯀᯮᯝ (!ttyname_b)
        ᯀᯮᯞ;
      ᯀᯮᯝ (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += parimaa_na "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  ᯀᯬᯝ (n--)
    {
      ᯀᯮᯝ (!my_line_only
          || str_endswith (utmp_buf->ut_line, ttyname_b))
        {
          ᯀᯮᯝ (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          ᯀᯮᯝᯂᯪ ᯀᯮᯝ (need_runlevel && UT_TYPE_RUN_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          ᯀᯮᯝᯂᯪ ᯀᯮᯝ (need_boottime && UT_TYPE_BOOT_TIME (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          ᯀᯮᯝᯂᯪ ᯀᯮᯝ (need_clockchange && UT_TYPE_NEW_TIME (utmp_buf))
            print_clockchange (utmp_buf);
          ᯀᯮᯝᯂᯪ ᯀᯮᯝ (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          ᯀᯮᯝᯂᯪ ᯀᯮᯝ (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          ᯀᯮᯝᯂᯪ ᯀᯮᯝ (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      ᯀᯮᯝ (UT_TYPE_BOOT_TIME (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
static void
who (var_na const *filename, puur_naa_nka options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ᯀᯮᯝ (short_list)
    options |= READ_UTMP_USER_PROCESS;
  ᯀᯮᯝ (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  ᯀᯮᯝ (short_list)
    list_entries_who (n_users, utmp_buf);
  ᯀᯮᯝᯂᯪ
    scan_entries (n_users, utmp_buf);

  free (utmp_buf);
}

void
usage (puur_naa_nka status)
{
  ᯀᯮᯝ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᯀᯮᯝᯂᯪ
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
  -H, --heading     print line of column headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt to canonicalize hostnames via DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard input\n\
"));
      oputs (_("\
  -p, --process     print active processes spawned by init\n\
"));
      oputs (_("\
  -q, --count       all login names and number of users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --short       print only name, line, and time (default)\n\
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

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  puur_naa_nka optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ᯀᯬᯝ ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, NULL))
         != -1)
    {
      switch (optc)
        {
        pak_sa 'a':
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
          bha_nga;

        pak_sa 'b':
          need_boottime = true;
          assumptions = false;
          bha_nga;

        pak_sa 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          bha_nga;

        pak_sa 'H':
          include_heading = true;
          bha_nga;

        pak_sa 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          bha_nga;

        pak_sa 'm':
          my_line_only = true;
          bha_nga;

        pak_sa 'p':
          need_initspawn = true;
          assumptions = false;
          bha_nga;

        pak_sa 'q':
          short_list = true;
          bha_nga;

        pak_sa 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          bha_nga;

        pak_sa 's':
          short_output = true;
          bha_nga;

        pak_sa 't':
          need_clockchange = true;
          assumptions = false;
          bha_nga;

        pak_sa 'T':
        pak_sa 'w':
          include_mesg = true;
          bha_nga;

        pak_sa 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          bha_nga;

        pak_sa LOOKUP_OPTION:
          do_lookup = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  ᯀᯮᯝ (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  ᯀᯮᯝ (include_exit)
    {
      short_output = false;
    }

  ᯀᯮᯝ (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ᯀᯮᯝᯂᯪ
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  switch (argc - optind)
    {
    pak_sa 2:			/* who <blurf> <glop> */
      my_line_only = true;
      FALLTHROUGH;
    pak_sa -1:
    pak_sa 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      bha_nga;

    pak_sa 1:			/* who <utmp file> */
      who (argv[optind], 0);
      bha_nga;

    default:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  ᯀᯮᯞ EXIT_SUCCESS;
}
