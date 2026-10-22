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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <stdio.h>

#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "canon-host.h"
#அந்தர்பாவ "readutmp.h"
#அந்தர்பாவ "hard-locale.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "xvasprintf.h"

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
#இல்லையெனில்
# define UT_TYPE_RUN_LVL(U) false
#endif

#ifdef INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#இல்லையெனில்
# define UT_TYPE_INIT_PROCESS(U) false
#endif

#ifdef DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#இல்லையெனில்
# define UT_TYPE_DEAD_PROCESS(U) false
#endif

#ifdef NEW_TIME
# define UT_TYPE_NEW_TIME(U) ((U)->ut_type == NEW_TIME)
#இல்லையெனில்
# define UT_TYPE_NEW_TIME(U) false
#endif

#define IDLESTR_LEN 6

#எனில் HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  எழுத்து Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (long முழு) (Utmp_ent->ut_pid))
#இல்லையெனில்
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  எழுத்து மாறிலி *Var = ""
#endif

#எனில் HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#இல்லையெனில்
# define UT_ID(U) "??"
#endif

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
நிலையான bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
நிலையான bool short_list;

/* If true, display only name, line, and time fields.  */
நிலையான bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
நிலையான bool include_idle;

/* If true, display a line at the top describing each field.  */
நிலையான bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
நிலையான bool include_mesg;

/* If true, display process termination & exit status.  */
நிலையான bool include_exit;

/* If true, display the last boot time.  */
நிலையான bool need_boottime;

/* If true, display dead processes.  */
நிலையான bool need_deadprocs;

/* If true, display processes waiting for user login.  */
நிலையான bool need_login;

/* If true, display processes started by init.  */
நிலையான bool need_initspawn;

/* If true, display the last clock change.  */
நிலையான bool need_clockchange;

/* If true, display the current runlevel.  */
நிலையான bool need_runlevel;

/* If true, display user processes.  */
நிலையான bool need_users;

/* If true, display info only for the controlling tty.  */
நிலையான bool my_வரி_only;

/* The strftime format to use for login times, and its expected
   output width.  */
நிலையான எழுத்து மாறிலி *time_format;
நிலையான முழு time_format_width;

/* for long options with no corresponding short option, use enum */
எண்ணிக்கை
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"all", no_argument, வெற்று, 'a'},
  {"boot", no_argument, வெற்று, 'b'},
  {"count", no_argument, வெற்று, 'q'},
  {"dead", no_argument, வெற்று, 'd'},
  {"heading", no_argument, வெற்று, 'H'},
  {"login", no_argument, வெற்று, 'l'},
  {"lookup", no_argument, வெற்று, LOOKUP_OPTION},
  {"message", no_argument, வெற்று, 'T'},
  {"mesg", no_argument, வெற்று, 'T'},
  {"process", no_argument, வெற்று, 'p'},
  {"runlevel", no_argument, வெற்று, 'r'},
  {"short", no_argument, வெற்று, 's'},
  {"time", no_argument, வெற்று, 't'},
  {"users", no_argument, வெற்று, 'u'},
  {"writable", no_argument, வெற்று, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
நிலையான எழுத்து மாறிலி *
idle_string (time_t when, time_t boottime)
{
  நிலையான time_t now = TYPE_MINIMUM (time_t);

  எனில் (now == TYPE_MINIMUM (time_t))
    time (&now);

  முழு seconds_idle;
  எனில் (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      எனில் (seconds_idle < 60)
        திரும்பு "  .  ";
      இல்லையெனில்
        {
          நிலையான எழுத்து idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          திரும்பு idle_hhmm;
        }
    }

  திரும்பு _(" old ");
}

/* Return a time string.  */
நிலையான எழுத்து மாறிலி *
time_string (STRUCT_UTMP மாறிலி *utmp_ent)
{
  நிலையான எழுத்து buf[INT_STRLEN_BOUND (intmax_t) + அளவு "-%m-%d %H:%M"];
  கட்டமைப்பு tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  எனில் (tmp)
    {
      strftime (buf, அளவு buf, time_format, tmp);
      திரும்பு buf;
    }
  இல்லையெனில்
    திரும்பு timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
நிலையான வெறுமை
print_வரி (எழுத்து மாறிலி *user, மாறிலி எழுத்து state,
            எழுத்து மாறிலி *வரி,
            எழுத்து மாறிலி *time_str, எழுத்து மாறிலி *idle, எழுத்து மாறிலி *pid,
            எழுத்து மாறிலி *comment, எழுத்து மாறிலி *exitstr)
{
  நிலையான எழுத்து mesg[3] = { ' ', 'x', '\0' };
  எழுத்து *buf;
  எழுத்து x_idle[1 + IDLESTR_LEN + 1];
  எழுத்து x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  எழுத்து *x_exitstr;

  mesg[1] = state;

  எனில் (include_idle && !short_output && strlen (idle) < அளவு x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  இல்லையெனில்
    *x_idle = '\0';

  எனில் (!short_output && strlen (pid) < அளவு x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  இல்லையெனில்
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, strlen (exitstr)) + 1 : 1);
  எனில் (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
  இல்லையெனில்
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
                   வரி,
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
    எழுத்து *p = buf + strlen (buf);
    போது (*--p == ' ')
      /* empty */;
    *(p + 1) = '\0';
  }

  puts (buf);
  விடுவி (buf);
  விடுவி (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
நிலையான bool
is_tty_writable (கட்டமைப்பு stat மாறிலி *pstat)
{
#ifdef TTY_GROUP_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  கட்டமைப்பு group *ttygr = getgrnam (TTY_GROUP_NAME);
  எனில் (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    திரும்பு false;
#endif

  திரும்பு pstat->st_mode & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
நிலையான வெறுமை
print_user (STRUCT_UTMP மாறிலி *utmp_ent, time_t boottime)
{
  கட்டமைப்பு stat stats;
  time_t last_change;
  எழுத்து mesg;
  எழுத்து idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  நிலையான எழுத்து *hoststr;
#எனில் HAVE_STRUCT_XTMP_UT_HOST
  நிலையான idx_t hostlen;
#endif

  /* If ut_line contains a space, the device name starts after the space.  */
  எழுத்து *வரி = utmp_ent->ut_வரி;
  எழுத்து *space = strchr (வரி, ' ');
  வரி = space ? space + 1 : வரி;

  முழு dirfd;
  எனில் (IS_ABSOLUTE_FILE_NAME (வரி))
    dirfd = AT_FDCWD;
  இல்லையெனில்
    {
      நிலையான முழு dev_dirfd;
      எனில் (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          எனில் (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  எனில் (AT_FDCWD <= dirfd && fstatat (dirfd, வரி, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      last_change = stats.st_atime;
    }
  இல்லையெனில்
    {
      mesg = '?';
      last_change = 0;
    }

  எனில் (last_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (last_change, boottime));
  இல்லையெனில்
    sprintf (idlestr, "  ?");

#எனில் HAVE_STRUCT_XTMP_UT_HOST
  எனில் (utmp_ent->ut_host[0])
    {
      எழுத்து *host = வெற்று;
      எழுத்து *display = வெற்று;
      எழுத்து *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      எனில் (display)
        *display++ = '\0';

      எனில் (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      எனில் (! host)
        host = ut_host;

      எனில் (display)
        {
          idx_t needed = strlen (host) + strlen (display) + 4;
          எனில் (hostlen < needed)
            {
              விடுவி (hoststr);
              hoststr = xpalloc (வெற்று, &hostlen, needed - hostlen, -1, 1);
            }
          எழுத்து *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          strcpy (stpcpy (p, display), ")");
        }
      இல்லையெனில்
        {
          idx_t needed = strlen (host) + 3;
          எனில் (hostlen < needed)
            {
              விடுவி (hoststr);
              hoststr = xpalloc (வெற்று, &hostlen, needed - hostlen, -1, 1);
            }
          எழுத்து *p = hoststr;
          *p++ = '(';
          strcpy (stpcpy (p, host), ")");
        }

      எனில் (host != ut_host)
        விடுவி (host);
    }
  இல்லையெனில்
    {
      எனில் (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#endif

  print_வரி (utmp_ent->ut_user, mesg,
              utmp_ent->ut_வரி,
              time_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

நிலையான வெறுமை
print_boottime (STRUCT_UTMP மாறிலி *utmp_ent)
{
  print_வரி ("", ' ', _("system boot"),
              time_string (utmp_ent), "", "", "", "");
}

நிலையான எழுத்து *
make_id_equals_comment (STRUCT_UTMP மாறிலி *utmp_ent)
{
  எழுத்து மாறிலி *id = UT_ID (utmp_ent);
  idx_t idlen = strlen (id);
  எழுத்து மாறிலி *prefix = _("id=");
  idx_t prefixlen = strlen (prefix);
  எழுத்து *comment = xmalloc (prefixlen + idlen + 1);
  எழுத்து *p = mempcpy (comment, prefix, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  திரும்பு comment;
}

நிலையான வெறுமை
print_deadprocs (STRUCT_UTMP மாறிலி *utmp_ent)
{
  நிலையான எழுத்து *exitstr;
  எழுத்து *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  எனில் (!exitstr)
    exitstr = xmalloc (strlen (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + strlen (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_வரி ("", ' ', utmp_ent->ut_வரி,
              time_string (utmp_ent), "", pidstr, comment, exitstr);
  விடுவி (comment);
}

நிலையான வெறுமை
print_login (STRUCT_UTMP மாறிலி *utmp_ent)
{
  எழுத்து *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_வரி (_("LOGIN"), ' ', utmp_ent->ut_வரி,
              time_string (utmp_ent), "", pidstr, comment, "");
  விடுவி (comment);
}

நிலையான வெறுமை
print_initspawn (STRUCT_UTMP மாறிலி *utmp_ent)
{
  எழுத்து *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_வரி ("", ' ', utmp_ent->ut_வரி,
              time_string (utmp_ent), "", pidstr, comment, "");
  விடுவி (comment);
}

நிலையான வெறுமை
print_clockchange (STRUCT_UTMP மாறிலி *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_வரி ("", ' ', _("clock change"),
              time_string (utmp_ent), "", "", "", "");
}

நிலையான வெறுமை
print_runlevel (STRUCT_UTMP மாறிலி *utmp_ent)
{
  நிலையான எழுத்து *runlevline, *comment;
  unsigned எழுத்து last = utmp_ent->ut_pid / 256;
  unsigned எழுத்து curr = utmp_ent->ut_pid % 256;

  எனில் (!runlevline)
    runlevline = xmalloc (strlen (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  எனில் (!comment)
    comment = xmalloc (strlen (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (last == 'N') ? 'S' : last);

  print_வரி ("", ' ', runlevline, time_string (utmp_ent),
              "", "", c_isprint (last) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
நிலையான வெறுமை
list_entries_who (idx_t n, STRUCT_UTMP மாறிலி *utmp_buf)
{
  idx_t entries = 0;

  போது (n--)
    {
      எனில் (IS_USER_PROCESS (utmp_buf))
        {
          எழுத்து *trimmed_name;

          trimmed_name = extract_trimmed_name (utmp_buf);

          எனில் (entries)
            putchar (' ');
          fputs (trimmed_name, stdout);
          விடுவி (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  printf (_("\n# users=%td\n"), entries);
}

நிலையான வெறுமை
print_heading (வெறுமை)
{
  print_வரி (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
நிலையான வெறுமை
scan_entries (idx_t n, STRUCT_UTMP மாறிலி *utmp_buf)
{
  எழுத்து *ttyname_b IF_LINT ( = வெற்று);
  time_t boottime = TYPE_MINIMUM (time_t);

  எனில் (include_heading)
    print_heading ();

  எனில் (my_வரி_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      எனில் (!ttyname_b)
        திரும்பு;
      எனில் (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += அளவு "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  போது (n--)
    {
      எனில் (!my_வரி_only
          || str_endswith (utmp_buf->ut_வரி, ttyname_b))
        {
          எனில் (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          இல்லையெனில் எனில் (need_runlevel && UT_TYPE_RUN_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          இல்லையெனில் எனில் (need_boottime && UT_TYPE_BOOT_TIME (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          இல்லையெனில் எனில் (need_clockchange && UT_TYPE_NEW_TIME (utmp_buf))
            print_clockchange (utmp_buf);
          இல்லையெனில் எனில் (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          இல்லையெனில் எனில் (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          இல்லையெனில் எனில் (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      எனில் (UT_TYPE_BOOT_TIME (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
நிலையான வெறுமை
who (எழுத்து மாறிலி *filename, முழு options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  எனில் (short_list)
    options |= READ_UTMP_USER_PROCESS;
  எனில் (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  எனில் (short_list)
    list_entries_who (n_users, utmp_buf);
  இல்லையெனில்
    scan_entries (n_users, utmp_buf);

  விடுவி (utmp_buf);
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
  -H, --heading     print வரி of column headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt to canonicalize hostnames via DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard உள்ளீடு\n\
"));
      oputs (_("\
  -p, --process     print active processes spawned by init\n\
"));
      oputs (_("\
  -q, --மொத்தம்       all login names and எண் of users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --short       print only name, வரி, and time (இயல்பு)\n\
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

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, வெற்று))
         != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'a':
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
          நிறுத்து;

        நிலை 'b':
          need_boottime = true;
          assumptions = false;
          நிறுத்து;

        நிலை 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          நிறுத்து;

        நிலை 'H':
          include_heading = true;
          நிறுத்து;

        நிலை 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          நிறுத்து;

        நிலை 'm':
          my_வரி_only = true;
          நிறுத்து;

        நிலை 'p':
          need_initspawn = true;
          assumptions = false;
          நிறுத்து;

        நிலை 'q':
          short_list = true;
          நிறுத்து;

        நிலை 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          நிறுத்து;

        நிலை 's':
          short_output = true;
          நிறுத்து;

        நிலை 't':
          need_clockchange = true;
          assumptions = false;
          நிறுத்து;

        நிலை 'T':
        நிலை 'w':
          include_mesg = true;
          நிறுத்து;

        நிலை 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          நிறுத்து;

        நிலை LOOKUP_OPTION:
          do_lookup = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  எனில் (include_exit)
    {
      short_output = false;
    }

  எனில் (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  இல்லையெனில்
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  தேர்வு (argc - optind)
    {
    நிலை 2:			/* who <blurf> <glop> */
      my_வரி_only = true;
      FALLTHROUGH;
    நிலை -1:
    நிலை 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      நிறுத்து;

    நிலை 1:			/* who <utmp file> */
      who (argv[optind], 0);
      நிறுத்து;

    இயல்பு:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  திரும்பு EXIT_SUCCESS;
}
