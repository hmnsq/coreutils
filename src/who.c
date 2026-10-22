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

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "canon-host.h"
#अन्तर्भाव "readutmp.h"
#अन्तर्भाव "hard-locale.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xvasprintf.h"

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
#अन्यथा
# define UT_TYPE_RUN_LVL(U) false
#endif

#ifdef INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#अन्यथा
# define UT_TYPE_INIT_PROCESS(U) false
#endif

#ifdef DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#अन्यथा
# define UT_TYPE_DEAD_PROCESS(U) false
#endif

#ifdef NEW_TIME
# define UT_TYPE_NEW_TIME(U) ((U)->ut_type == NEW_TIME)
#अन्यथा
# define UT_TYPE_NEW_TIME(U) false
#endif

#define IDLESTR_LEN 6

#जर HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  अक्षर Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  प्र_लिखो (Var, "%ld", (दीर्घ पूर्णांक) (Utmp_ent->ut_pid))
#अन्यथा
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  अक्षर स्थिरांक *Var = ""
#endif

#जर HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#अन्यथा
# define UT_ID(U) "??"
#endif

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
स्थिर bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
स्थिर bool short_list;

/* If true, display only name, line, and time fields.  */
स्थिर bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
स्थिर bool include_idle;

/* If true, display a line at the top describing each field.  */
स्थिर bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
स्थिर bool include_mesg;

/* If true, display process termination & exit status.  */
स्थिर bool include_exit;

/* If true, display the last boot time.  */
स्थिर bool need_boottime;

/* If true, display dead processes.  */
स्थिर bool need_deadprocs;

/* If true, display processes waiting for user login.  */
स्थिर bool need_login;

/* If true, display processes started by init.  */
स्थिर bool need_initspawn;

/* If true, display the last clock change.  */
स्थिर bool need_clockchange;

/* If true, display the current runlevel.  */
स्थिर bool need_runlevel;

/* If true, display user processes.  */
स्थिर bool need_users;

/* If true, display info only for the controlling tty.  */
स्थिर bool my_ओळ_only;

/* The strftime format to use for login times, and its expected
   output width.  */
स्थिर अक्षर स्थिरांक *time_format;
स्थिर पूर्णांक time_format_width;

/* for long options with no corresponding short option, use enum */
गणना
{
  LOOKUP_OPTION = अक्षर_उच्च + 1
};

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"all", no_argument, रिक्त, 'a'},
  {"boot", no_argument, रिक्त, 'b'},
  {"count", no_argument, रिक्त, 'q'},
  {"dead", no_argument, रिक्त, 'd'},
  {"heading", no_argument, रिक्त, 'H'},
  {"login", no_argument, रिक्त, 'l'},
  {"lookup", no_argument, रिक्त, LOOKUP_OPTION},
  {"message", no_argument, रिक्त, 'T'},
  {"mesg", no_argument, रिक्त, 'T'},
  {"process", no_argument, रिक्त, 'p'},
  {"runlevel", no_argument, रिक्त, 'r'},
  {"short", no_argument, रिक्त, 's'},
  {"time", no_argument, रिक्त, 't'},
  {"users", no_argument, रिक्त, 'u'},
  {"writable", no_argument, रिक्त, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
स्थिर अक्षर स्थिरांक *
idle_string (समय_प्रकार when, समय_प्रकार boottime)
{
  स्थिर समय_प्रकार now = TYPE_MINIMUM (समय_प्रकार);

  जर (now == TYPE_MINIMUM (समय_प्रकार))
    समय (&now);

  पूर्णांक seconds_idle;
  जर (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      जर (seconds_idle < 60)
        परत "  .  ";
      अन्यथा
        {
          स्थिर अक्षर idle_hhmm[IDLESTR_LEN];
          प्र_लिखो (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          परत idle_hhmm;
        }
    }

  परत _(" old ");
}

/* Return a time string.  */
स्थिर अक्षर स्थिरांक *
time_string (STRUCT_UTMP स्थिरांक *utmp_ent)
{
  स्थिर अक्षर buf[INT_STRLEN_BOUND (intmax_t) + आकार "-%m-%d %H:%M"];
  संरचना पंचांग *tmp = स_स्थानीय (&utmp_ent->ut_ts.tv_sec);

  जर (tmp)
    {
      स_माला (buf, आकार buf, time_format, tmp);
      परत buf;
    }
  अन्यथा
    परत timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
स्थिर शून्य
print_ओळ (अक्षर स्थिरांक *user, स्थिरांक अक्षर state,
            अक्षर स्थिरांक *ओळ,
            अक्षर स्थिरांक *time_str, अक्षर स्थिरांक *idle, अक्षर स्थिरांक *pid,
            अक्षर स्थिरांक *comment, अक्षर स्थिरांक *exitstr)
{
  स्थिर अक्षर mesg[3] = { ' ', 'x', '\0' };
  अक्षर *buf;
  अक्षर x_idle[1 + IDLESTR_LEN + 1];
  अक्षर x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  अक्षर *x_exitstr;

  mesg[1] = state;

  जर (include_idle && !short_output && म_माप (idle) < आकार x_idle - 1)
    प्र_लिखो (x_idle, " %-6s", idle);
  अन्यथा
    *x_idle = '\0';

  जर (!short_output && म_माप (pid) < आकार x_pid - 1)
    प्र_लिखो (x_pid, " %10s", pid);
  अन्यथा
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, म_माप (exitstr)) + 1 : 1);
  जर (include_exit)
    प्र_लिखो (x_exitstr, " %-12s", exitstr);
  अन्यथा
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
                   ओळ,
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
    अक्षर *p = buf + म_माप (buf);
    पर्यंत (*--p == ' ')
      /* empty */;
    *(p + 1) = '\0';
  }

  माला_दो (buf);
  मुक्त (buf);
  मुक्त (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
स्थिर bool
is_tty_writable (संरचना stat स्थिरांक *pstat)
{
#ifdef TTY_GROUP_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  संरचना group *ttygr = getgrnam (TTY_GROUP_NAME);
  जर (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    परत false;
#endif

  परत pstat->st_mode & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
स्थिर शून्य
print_user (STRUCT_UTMP स्थिरांक *utmp_ent, समय_प्रकार boottime)
{
  संरचना stat stats;
  समय_प्रकार last_change;
  अक्षर mesg;
  अक्षर idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  स्थिर अक्षर *hoststr;
#जर HAVE_STRUCT_XTMP_UT_HOST
  स्थिर idx_t hostlen;
#endif

  /* If ut_line contains a space, the device name starts after the space.  */
  अक्षर *ओळ = utmp_ent->ut_ओळ;
  अक्षर *space = म_अक्षर (ओळ, ' ');
  ओळ = space ? space + 1 : ओळ;

  पूर्णांक dirfd;
  जर (IS_ABSOLUTE_FILE_NAME (ओळ))
    dirfd = AT_FDCWD;
  अन्यथा
    {
      स्थिर पूर्णांक dev_dirfd;
      जर (!dev_dirfd)
        {
          dev_dirfd = खोलो ("/dev", O_PATHSEARCH | O_DIRECTORY);
          जर (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  जर (AT_FDCWD <= dirfd && fstatat (dirfd, ओळ, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      last_change = stats.st_atime;
    }
  अन्यथा
    {
      mesg = '?';
      last_change = 0;
    }

  जर (last_change)
    प्र_लिखो (idlestr, "%.*s", IDLESTR_LEN, idle_string (last_change, boottime));
  अन्यथा
    प्र_लिखो (idlestr, "  ?");

#जर HAVE_STRUCT_XTMP_UT_HOST
  जर (utmp_ent->ut_host[0])
    {
      अक्षर *host = रिक्त;
      अक्षर *display = रिक्त;
      अक्षर *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = म_अक्षर (ut_host, ':');
      जर (display)
        *display++ = '\0';

      जर (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      जर (! host)
        host = ut_host;

      जर (display)
        {
          idx_t needed = म_माप (host) + म_माप (display) + 4;
          जर (hostlen < needed)
            {
              मुक्त (hoststr);
              hoststr = xpalloc (रिक्त, &hostlen, needed - hostlen, -1, 1);
            }
          अक्षर *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          म_नकल (stpcpy (p, display), ")");
        }
      अन्यथा
        {
          idx_t needed = म_माप (host) + 3;
          जर (hostlen < needed)
            {
              मुक्त (hoststr);
              hoststr = xpalloc (रिक्त, &hostlen, needed - hostlen, -1, 1);
            }
          अक्षर *p = hoststr;
          *p++ = '(';
          म_नकल (stpcpy (p, host), ")");
        }

      जर (host != ut_host)
        मुक्त (host);
    }
  अन्यथा
    {
      जर (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#endif

  print_ओळ (utmp_ent->ut_user, mesg,
              utmp_ent->ut_ओळ,
              time_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

स्थिर शून्य
print_boottime (STRUCT_UTMP स्थिरांक *utmp_ent)
{
  print_ओळ ("", ' ', _("system boot"),
              time_string (utmp_ent), "", "", "", "");
}

स्थिर अक्षर *
make_id_equals_comment (STRUCT_UTMP स्थिरांक *utmp_ent)
{
  अक्षर स्थिरांक *id = UT_ID (utmp_ent);
  idx_t idlen = म_माप (id);
  अक्षर स्थिरांक *prefix = _("id=");
  idx_t prefixlen = म_माप (prefix);
  अक्षर *comment = xmalloc (prefixlen + idlen + 1);
  अक्षर *p = mempcpy (comment, prefix, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  परत comment;
}

स्थिर शून्य
print_deadprocs (STRUCT_UTMP स्थिरांक *utmp_ent)
{
  स्थिर अक्षर *exitstr;
  अक्षर *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  जर (!exitstr)
    exitstr = xmalloc (म_माप (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + म_माप (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  प्र_लिखो (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_ओळ ("", ' ', utmp_ent->ut_ओळ,
              time_string (utmp_ent), "", pidstr, comment, exitstr);
  मुक्त (comment);
}

स्थिर शून्य
print_login (STRUCT_UTMP स्थिरांक *utmp_ent)
{
  अक्षर *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_ओळ (_("LOGIN"), ' ', utmp_ent->ut_ओळ,
              time_string (utmp_ent), "", pidstr, comment, "");
  मुक्त (comment);
}

स्थिर शून्य
print_initspawn (STRUCT_UTMP स्थिरांक *utmp_ent)
{
  अक्षर *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_ओळ ("", ' ', utmp_ent->ut_ओळ,
              time_string (utmp_ent), "", pidstr, comment, "");
  मुक्त (comment);
}

स्थिर शून्य
print_clockchange (STRUCT_UTMP स्थिरांक *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_ओळ ("", ' ', _("clock change"),
              time_string (utmp_ent), "", "", "", "");
}

स्थिर शून्य
print_runlevel (STRUCT_UTMP स्थिरांक *utmp_ent)
{
  स्थिर अक्षर *runlevline, *comment;
  अचिन्हित अक्षर last = utmp_ent->ut_pid / 256;
  अचिन्हित अक्षर curr = utmp_ent->ut_pid % 256;

  जर (!runlevline)
    runlevline = xmalloc (म_माप (_("run-level")) + 3);
  प्र_लिखो (runlevline, "%s %c", _("run-level"), curr);

  जर (!comment)
    comment = xmalloc (म_माप (_("last=")) + 2);
  प्र_लिखो (comment, "%s%c", _("last="), (last == 'N') ? 'S' : last);

  print_ओळ ("", ' ', runlevline, time_string (utmp_ent),
              "", "", c_isprint (last) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
स्थिर शून्य
list_entries_who (idx_t n, STRUCT_UTMP स्थिरांक *utmp_buf)
{
  idx_t entries = 0;

  पर्यंत (n--)
    {
      जर (IS_USER_PROCESS (utmp_buf))
        {
          अक्षर *trimmed_name;

          trimmed_name = extract_trimmed_name (utmp_buf);

          जर (entries)
            अक्षर_दो (' ');
          ख_माला_दो (trimmed_name, मानक_निकास);
          मुक्त (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  म_लिखो (_("\n# users=%td\n"), entries);
}

स्थिर शून्य
print_heading (शून्य)
{
  print_ओळ (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
स्थिर शून्य
scan_entries (idx_t n, STRUCT_UTMP स्थिरांक *utmp_buf)
{
  अक्षर *ttyname_b IF_LINT ( = रिक्त);
  समय_प्रकार boottime = TYPE_MINIMUM (समय_प्रकार);

  जर (include_heading)
    print_heading ();

  जर (my_ओळ_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      जर (!ttyname_b)
        परत;
      जर (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += आकार "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  पर्यंत (n--)
    {
      जर (!my_ओळ_only
          || str_endswith (utmp_buf->ut_ओळ, ttyname_b))
        {
          जर (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          अन्यथा जर (need_runlevel && UT_TYPE_RUN_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          अन्यथा जर (need_boottime && UT_TYPE_BOOT_TIME (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          अन्यथा जर (need_clockchange && UT_TYPE_NEW_TIME (utmp_buf))
            print_clockchange (utmp_buf);
          अन्यथा जर (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          अन्यथा जर (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          अन्यथा जर (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      जर (UT_TYPE_BOOT_TIME (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
स्थिर शून्य
who (अक्षर स्थिरांक *filename, पूर्णांक options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  जर (short_list)
    options |= READ_UTMP_USER_PROCESS;
  जर (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (filename));

  जर (short_list)
    list_entries_who (n_users, utmp_buf);
  अन्यथा
    scan_entries (n_users, utmp_buf);

  मुक्त (utmp_buf);
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [ FILE | ARG1 ARG2 ]\n"), program_name);
      ख_माला_दो (_("\
Print information about users who are currently logged in.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
"), मानक_निकास);
      oputs (_("\
  -a, --all         same as -b -d --login -p -r -t -T -u\n\
"));
      oputs (_("\
  -b, --boot        समय of last प्रणाली boot\n\
"));
      oputs (_("\
  -d, --dead        print dead processes\n\
"));
      oputs (_("\
  -H, --heading     print ओळ of column headings\n\
"));
      oputs (_("\
  -l, --login       print प्रणाली login processes\n\
"));
      oputs (_("\
      --lookup      attempt to canonicalize hostnames via DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard आदान\n\
"));
      oputs (_("\
  -p, --process     print active processes spawned by init\n\
"));
      oputs (_("\
  -q, --मोजणी       all login names and संख्या of users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --लघु       print only name, ओळ, and समय (पूर्वनिर्धारित)\n\
"));
      oputs (_("\
  -t, --समय        print last प्रणाली घड़ी change\n\
"));
      oputs (_("\
  -T, -w, --mesg    add user's message status as +, - or ?\n\
"));
      oputs (_("\
  -u, --users       list users logged in, including idle समय\n\
"));
      oputs (_("\
      --message     same as -T\n\
"));
      oputs (_("\
      --writable    same as -T\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      म_लिखो (_("\
\n\
If खाता is not specified, use %s.  %s as खाता is common.\n\
If ARG1 ARG2 given, -m presumed: 'am i' or 'mom likes' are usual.\n\
"), UTMP_FILE, WTMP_FILE);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, रिक्त))
         != -1)
    {
      निवड (optc)
        {
        स्थिती 'a':
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
          थांबा;

        स्थिती 'b':
          need_boottime = true;
          assumptions = false;
          थांबा;

        स्थिती 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          थांबा;

        स्थिती 'H':
          include_heading = true;
          थांबा;

        स्थिती 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          थांबा;

        स्थिती 'm':
          my_ओळ_only = true;
          थांबा;

        स्थिती 'p':
          need_initspawn = true;
          assumptions = false;
          थांबा;

        स्थिती 'q':
          short_list = true;
          थांबा;

        स्थिती 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          थांबा;

        स्थिती 's':
          short_output = true;
          थांबा;

        स्थिती 't':
          need_clockchange = true;
          assumptions = false;
          थांबा;

        स्थिती 'T':
        स्थिती 'w':
          include_mesg = true;
          थांबा;

        स्थिती 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          थांबा;

        स्थिती LOOKUP_OPTION:
          do_lookup = true;
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  जर (include_exit)
    {
      short_output = false;
    }

  जर (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  अन्यथा
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  निवड (argc - optind)
    {
    स्थिती 2:			/* who <blurf> <glop> */
      my_ओळ_only = true;
      FALLTHROUGH;
    स्थिती -1:
    स्थिती 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      थांबा;

    स्थिती 1:			/* who <utmp file> */
      who (argv[optind], 0);
      थांबा;

    पूर्वनिर्धारित:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (निकास_त्रुटि);
    }

  परत निकास_सफल;
}
