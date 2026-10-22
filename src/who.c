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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdio.h>

#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "canon-host.h"
#અન્તર્ભાવ "readutmp.h"
#અન્તર્ભાવ "hard-locale.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "xvasprintf.h"

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
#નહીંતર
# define UT_TYPE_RUN_LVL(U) false
#endif

#ifdef INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#નહીંતર
# define UT_TYPE_INIT_PROCESS(U) false
#endif

#ifdef DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#નહીંતર
# define UT_TYPE_DEAD_PROCESS(U) false
#endif

#ifdef NEW_TIME
# define UT_TYPE_NEW_TIME(U) ((U)->ut_type == NEW_TIME)
#નહીંતર
# define UT_TYPE_NEW_TIME(U) false
#endif

#define IDLESTR_LEN 6

#જો HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  અક્ષર Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (long પૂર્ણાંક) (Utmp_ent->ut_pid))
#નહીંતર
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  અક્ષર અચલ *Var = ""
#endif

#જો HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#નહીંતર
# define UT_ID(U) "??"
#endif

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
સ્થિર bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
સ્થિર bool short_list;

/* If true, display only name, line, and time fields.  */
સ્થિર bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
સ્થિર bool include_idle;

/* If true, display a line at the top describing each field.  */
સ્થિર bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
સ્થિર bool include_mesg;

/* If true, display process termination & exit status.  */
સ્થિર bool include_exit;

/* If true, display the last boot time.  */
સ્થિર bool need_boottime;

/* If true, display dead processes.  */
સ્થિર bool need_deadprocs;

/* If true, display processes waiting for user login.  */
સ્થિર bool need_login;

/* If true, display processes started by init.  */
સ્થિર bool need_initspawn;

/* If true, display the last clock change.  */
સ્થિર bool need_clockchange;

/* If true, display the current runlevel.  */
સ્થિર bool need_runlevel;

/* If true, display user processes.  */
સ્થિર bool need_users;

/* If true, display info only for the controlling tty.  */
સ્થિર bool my_લીટી_only;

/* The strftime format to use for login times, and its expected
   output width.  */
સ્થિર અક્ષર અચલ *time_format;
સ્થિર પૂર્ણાંક time_format_width;

/* for long options with no corresponding short option, use enum */
ગણના
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"all", no_argument, ખાલી, 'a'},
  {"boot", no_argument, ખાલી, 'b'},
  {"count", no_argument, ખાલી, 'q'},
  {"dead", no_argument, ખાલી, 'd'},
  {"heading", no_argument, ખાલી, 'H'},
  {"login", no_argument, ખાલી, 'l'},
  {"lookup", no_argument, ખાલી, LOOKUP_OPTION},
  {"message", no_argument, ખાલી, 'T'},
  {"mesg", no_argument, ખાલી, 'T'},
  {"process", no_argument, ખાલી, 'p'},
  {"runlevel", no_argument, ખાલી, 'r'},
  {"short", no_argument, ખાલી, 's'},
  {"time", no_argument, ખાલી, 't'},
  {"users", no_argument, ખાલી, 'u'},
  {"writable", no_argument, ખાલી, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
સ્થિર અક્ષર અચલ *
idle_string (time_t when, time_t boottime)
{
  સ્થિર time_t now = TYPE_MINIMUM (time_t);

  જો (now == TYPE_MINIMUM (time_t))
    time (&now);

  પૂર્ણાંક seconds_idle;
  જો (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      જો (seconds_idle < 60)
        પાછા "  .  ";
      નહીંતર
        {
          સ્થિર અક્ષર idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          પાછા idle_hhmm;
        }
    }

  પાછા _(" old ");
}

/* Return a time string.  */
સ્થિર અક્ષર અચલ *
time_string (STRUCT_UTMP અચલ *utmp_ent)
{
  સ્થિર અક્ષર buf[INT_STRLEN_BOUND (intmax_t) + કદ "-%m-%d %H:%M"];
  સંરચના tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  જો (tmp)
    {
      strftime (buf, કદ buf, time_format, tmp);
      પાછા buf;
    }
  નહીંતર
    પાછા timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
સ્થિર શૂન્ય
print_લીટી (અક્ષર અચલ *user, અચલ અક્ષર state,
            અક્ષર અચલ *લીટી,
            અક્ષર અચલ *time_str, અક્ષર અચલ *idle, અક્ષર અચલ *pid,
            અક્ષર અચલ *comment, અક્ષર અચલ *exitstr)
{
  સ્થિર અક્ષર mesg[3] = { ' ', 'x', '\0' };
  અક્ષર *buf;
  અક્ષર x_idle[1 + IDLESTR_LEN + 1];
  અક્ષર x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  અક્ષર *x_exitstr;

  mesg[1] = state;

  જો (include_idle && !short_output && strlen (idle) < કદ x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  નહીંતર
    *x_idle = '\0';

  જો (!short_output && strlen (pid) < કદ x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  નહીંતર
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, strlen (exitstr)) + 1 : 1);
  જો (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  નહીંતર
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
                   લીટી,
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
    અક્ષર *p = buf + strlen (buf);
    જ્યારે (*--p == ' ')
      /* empty */;
    *(p + 1) = '\0';
  }

  puts (buf);
  મુક્ત (buf);
  મુક્ત (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
સ્થિર bool
is_tty_writable (સંરચના stat અચલ *pstat)
{
#ifdef TTY_GROUP_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  સંરચના group *ttygr = getgrnam (TTY_GROUP_NAME);
  જો (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    પાછા false;
#endif

  પાછા pstat->st_mode & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
સ્થિર શૂન્ય
print_user (STRUCT_UTMP અચલ *utmp_ent, time_t boottime)
{
  સંરચના stat stats;
  time_t last_change;
  અક્ષર mesg;
  અક્ષર idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  સ્થિર અક્ષર *hoststr;
#જો HAVE_STRUCT_XTMP_UT_HOST
  સ્થિર idx_t hostlen;
#endif

  /* If ut_line contains a space, the device name starts after the space.  */
  અક્ષર *લીટી = utmp_ent->ut_લીટી;
  અક્ષર *space = strchr (લીટી, ' ');
  લીટી = space ? space + 1 : લીટી;

  પૂર્ણાંક dirfd;
  જો (IS_ABSOLUTE_FILE_NAME (લીટી))
    dirfd = AT_FDCWD;
  નહીંતર
    {
      સ્થિર પૂર્ણાંક dev_dirfd;
      જો (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          જો (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  જો (AT_FDCWD <= dirfd && fstatat (dirfd, લીટી, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      last_change = stats.st_atime;
    }
  નહીંતર
    {
      mesg = '?';
      last_change = 0;
    }

  જો (last_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (last_change, boottime));
  નહીંતર
    sprintf (idlestr, "  ?");

#જો HAVE_STRUCT_XTMP_UT_HOST
  જો (utmp_ent->ut_host[0])
    {
      અક્ષર *host = ખાલી;
      અક્ષર *display = ખાલી;
      અક્ષર *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      જો (display)
        *display++ = '\0';

      જો (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      જો (! host)
        host = ut_host;

      જો (display)
        {
          idx_t needed = strlen (host) + strlen (display) + 4;
          જો (hostlen < needed)
            {
              મુક્ત (hoststr);
              hoststr = xpalloc (ખાલી, &hostlen, needed - hostlen, -1, 1);
            }
          અક્ષર *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      નહીંતર
        {
          idx_t needed = strlen (host) + 3;
          જો (hostlen < needed)
            {
              મુક્ત (hoststr);
              hoststr = xpalloc (ખાલી, &hostlen, needed - hostlen, -1, 1);
            }
          અક્ષર *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      જો (host != ut_host)
        મુક્ત (host);
    }
  નહીંતર
    {
      જો (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#endif

  print_લીટી (utmp_ent->ut_user, mesg,
              utmp_ent->ut_લીટી,
              time_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

સ્થિર શૂન્ય
print_boottime (STRUCT_UTMP અચલ *utmp_ent)
{
  print_લીટી ("", ' ', _("system boot"),
              time_string (utmp_ent), "", "", "", "");
}

સ્થિર અક્ષર *
make_id_equals_comment (STRUCT_UTMP અચલ *utmp_ent)
{
  અક્ષર અચલ *id = UT_ID (utmp_ent);
  idx_t idlen = strlen (id);
  અક્ષર અચલ *prefix = _("id=");
  idx_t prefixlen = strlen (prefix);
  અક્ષર *comment = xmalloc (prefixlen + idlen + 1);
  અક્ષર *p = mempcpy (comment, prefix, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  પાછા comment;
}

સ્થિર શૂન્ય
print_deadprocs (STRUCT_UTMP અચલ *utmp_ent)
{
  સ્થિર અક્ષર *exitstr;
  અક્ષર *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  જો (!exitstr)
    exitstr = xmalloc (strlen (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + strlen (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_લીટી ("", ' ', utmp_ent->ut_લીટી,
              time_string (utmp_ent), "", pidstr, comment, exitstr);
  મુક્ત (comment);
}

સ્થિર શૂન્ય
print_login (STRUCT_UTMP અચલ *utmp_ent)
{
  અક્ષર *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_લીટી (_("LOGIN"), ' ', utmp_ent->ut_લીટી,
              time_string (utmp_ent), "", pidstr, comment, "");
  મુક્ત (comment);
}

સ્થિર શૂન્ય
print_initspawn (STRUCT_UTMP અચલ *utmp_ent)
{
  અક્ષર *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_લીટી ("", ' ', utmp_ent->ut_લીટી,
              time_string (utmp_ent), "", pidstr, comment, "");
  મુક્ત (comment);
}

સ્થિર શૂન્ય
print_clockchange (STRUCT_UTMP અચલ *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_લીટી ("", ' ', _("clock change"),
              time_string (utmp_ent), "", "", "", "");
}

સ્થિર શૂન્ય
print_runlevel (STRUCT_UTMP અચલ *utmp_ent)
{
  સ્થિર અક્ષર *runlevline, *comment;
  unsigned અક્ષર last = utmp_ent->ut_pid / 256;
  unsigned અક્ષર curr = utmp_ent->ut_pid % 256;

  જો (!runlevline)
    runlevline = xmalloc (strlen (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  જો (!comment)
    comment = xmalloc (strlen (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (last == 'N') ? 'S' : last);

  print_લીટી ("", ' ', runlevline, time_string (utmp_ent),
              "", "", c_isprint (last) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
સ્થિર શૂન્ય
list_entries_who (idx_t n, STRUCT_UTMP અચલ *utmp_buf)
{
  idx_t entries = 0;

  જ્યારે (n--)
    {
      જો (IS_USER_PROCESS (utmp_buf))
        {
          અક્ષર *trimmed_name;

          trimmed_name = extract_trimmed_name (utmp_buf);

          જો (entries)
            putchar (' ');
          fputs (trimmed_name, stdout);
          મુક્ત (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  printf (_("\n# users=%td\n"), entries);
}

સ્થિર શૂન્ય
print_heading (શૂન્ય)
{
  print_લીટી (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
સ્થિર શૂન્ય
scan_entries (idx_t n, STRUCT_UTMP અચલ *utmp_buf)
{
  અક્ષર *ttyname_b IF_LINT ( = ખાલી);
  time_t boottime = TYPE_MINIMUM (time_t);

  જો (include_heading)
    print_heading ();

  જો (my_લીટી_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      જો (!ttyname_b)
        પાછા;
      જો (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += કદ "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  જ્યારે (n--)
    {
      જો (!my_લીટી_only
          || str_endswith (utmp_buf->ut_લીટી, ttyname_b))
        {
          જો (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          નહીંતર જો (need_runlevel && UT_TYPE_RUN_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          નહીંતર જો (need_boottime && UT_TYPE_BOOT_TIME (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          નહીંતર જો (need_clockchange && UT_TYPE_NEW_TIME (utmp_buf))
            print_clockchange (utmp_buf);
          નહીંતર જો (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          નહીંતર જો (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          નહીંતર જો (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      જો (UT_TYPE_BOOT_TIME (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
સ્થિર શૂન્ય
who (અક્ષર અચલ *filename, પૂર્ણાંક options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  જો (short_list)
    options |= READ_UTMP_USER_PROCESS;
  જો (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  જો (short_list)
    list_entries_who (n_users, utmp_buf);
  નહીંતર
    scan_entries (n_users, utmp_buf);

  મુક્ત (utmp_buf);
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
  -H, --heading     print લીટી of column headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt to canonicalize hostnames via DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard આગત\n\
"));
      oputs (_("\
  -p, --process     print active processes spawned by init\n\
"));
      oputs (_("\
  -q, --ગણતરી       all login names and સંખ્યા of users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --short       print only name, લીટી, and time (મૂળભૂત)\n\
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

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, ખાલી))
         != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'a':
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
          તોડો;

        સ્થિતિ 'b':
          need_boottime = true;
          assumptions = false;
          તોડો;

        સ્થિતિ 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          તોડો;

        સ્થિતિ 'H':
          include_heading = true;
          તોડો;

        સ્થિતિ 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          તોડો;

        સ્થિતિ 'm':
          my_લીટી_only = true;
          તોડો;

        સ્થિતિ 'p':
          need_initspawn = true;
          assumptions = false;
          તોડો;

        સ્થિતિ 'q':
          short_list = true;
          તોડો;

        સ્થિતિ 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          તોડો;

        સ્થિતિ 's':
          short_output = true;
          તોડો;

        સ્થિતિ 't':
          need_clockchange = true;
          assumptions = false;
          તોડો;

        સ્થિતિ 'T':
        સ્થિતિ 'w':
          include_mesg = true;
          તોડો;

        સ્થિતિ 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          તોડો;

        સ્થિતિ LOOKUP_OPTION:
          do_lookup = true;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  જો (include_exit)
    {
      short_output = false;
    }

  જો (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  નહીંતર
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  પસંદગી (argc - optind)
    {
    સ્થિતિ 2:			/* who <blurf> <glop> */
      my_લીટી_only = true;
      FALLTHROUGH;
    સ્થિતિ -1:
    સ્થિતિ 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      તોડો;

    સ્થિતિ 1:			/* who <utmp file> */
      who (argv[optind], 0);
      તોડો;

    મૂળભૂત:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  પાછા EXIT_SUCCESS;
}
