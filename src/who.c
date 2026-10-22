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

#समावेश <config.h>
#समावेश <getopt.h>
#समावेश <stdio.h>

#समावेश <sys/types.h>
#समावेश "system.h"

#समावेश "c-ctype.h"
#समावेश "canon-host.h"
#समावेश "readutmp.h"
#समावेश "hard-locale.h"
#समावेश "quote.h"
#समावेश "xvasprintf.h"

#अगर_घोषित TTY_GROUP_NAME
# include <grp.h>
#पूर्ण_अगर

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "who"

#घोषणा AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Michael Stone")

#अगर_घोषित RUN_LVL
# define UT_TYPE_RUN_LVL(U) ((U)->ut_type == RUN_LVL)
#अन्यथा
# define UT_TYPE_RUN_LVL(U) false
#पूर्ण_अगर

#अगर_घोषित INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#अन्यथा
# define UT_TYPE_INIT_PROCESS(U) false
#पूर्ण_अगर

#अगर_घोषित DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#अन्यथा
# define UT_TYPE_DEAD_PROCESS(U) false
#पूर्ण_अगर

#अगर_घोषित NEW_TIME
# define UT_TYPE_NEW_TIME(U) ((U)->ut_type == NEW_TIME)
#अन्यथा
# define UT_TYPE_NEW_TIME(U) false
#पूर्ण_अगर

#घोषणा IDLESTR_LEN 6

#अगर HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  अक्षर Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  प्र_लिखो (Var, "%ld", (दीर्घ पूर्णांक) (Utmp_ent->ut_pid))
#अन्यथा
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  अक्षर स्थिर *Var = ""
#पूर्ण_अगर

#अगर HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#अन्यथा
# define UT_ID(U) "??"
#पूर्ण_अगर

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
अटल bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
अटल bool short_list;

/* If true, display only name, line, and time fields.  */
अटल bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
अटल bool include_idle;

/* If true, display a line at the top describing each field.  */
अटल bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
अटल bool include_mesg;

/* If true, display process termination & exit status.  */
अटल bool include_exit;

/* If true, display the last boot time.  */
अटल bool need_boottime;

/* If true, display dead processes.  */
अटल bool need_deadprocs;

/* If true, display processes waiting for user login.  */
अटल bool need_login;

/* If true, display processes started by init.  */
अटल bool need_initspawn;

/* If true, display the last clock change.  */
अटल bool need_clockchange;

/* If true, display the current runlevel.  */
अटल bool need_runlevel;

/* If true, display user processes.  */
अटल bool need_users;

/* If true, display info only for the controlling tty.  */
अटल bool my_पंक्ति_only;

/* The strftime format to use for login times, and its expected
   output width.  */
अटल अक्षर स्थिर *time_format;
अटल पूर्णांक time_format_width;

/* for long options with no corresponding short option, use enum */
क्रमागत
{
  LOOKUP_OPTION = अक्षर_उच्च + 1
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"all", no_argument, शून्य, 'a'},
  {"boot", no_argument, शून्य, 'b'},
  {"count", no_argument, शून्य, 'q'},
  {"dead", no_argument, शून्य, 'd'},
  {"heading", no_argument, शून्य, 'H'},
  {"login", no_argument, शून्य, 'l'},
  {"lookup", no_argument, शून्य, LOOKUP_OPTION},
  {"message", no_argument, शून्य, 'T'},
  {"mesg", no_argument, शून्य, 'T'},
  {"process", no_argument, शून्य, 'p'},
  {"runlevel", no_argument, शून्य, 'r'},
  {"short", no_argument, शून्य, 's'},
  {"time", no_argument, शून्य, 't'},
  {"users", no_argument, शून्य, 'u'},
  {"writable", no_argument, शून्य, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
अटल अक्षर स्थिर *
idle_string (समय_प्रकार when, समय_प्रकार boottime)
{
  अटल समय_प्रकार now = TYPE_MINIMUM (समय_प्रकार);

  अगर (now == TYPE_MINIMUM (समय_प्रकार))
    समय (&now);

  पूर्णांक seconds_idle;
  अगर (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      अगर (seconds_idle < 60)
        वापस "  .  ";
      अन्यथा
        {
          अटल अक्षर idle_hhmm[IDLESTR_LEN];
          प्र_लिखो (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          वापस idle_hhmm;
        }
    }

  वापस _(" old ");
}

/* Return a time string.  */
अटल अक्षर स्थिर *
time_string (STRUCT_UTMP स्थिर *utmp_ent)
{
  अटल अक्षर buf[INT_STRLEN_BOUND (intmax_t) + माप "-%m-%d %H:%M"];
  काष्ठा पंचांग *tmp = स_स्थानीय (&utmp_ent->ut_ts.tv_sec);

  अगर (tmp)
    {
      स_माला (buf, माप buf, time_format, tmp);
      वापस buf;
    }
  अन्यथा
    वापस timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
अटल व्योम
print_पंक्ति (अक्षर स्थिर *user, स्थिर अक्षर state,
            अक्षर स्थिर *पंक्ति,
            अक्षर स्थिर *time_str, अक्षर स्थिर *idle, अक्षर स्थिर *pid,
            अक्षर स्थिर *comment, अक्षर स्थिर *exitstr)
{
  अटल अक्षर mesg[3] = { ' ', 'x', '\0' };
  अक्षर *buf;
  अक्षर x_idle[1 + IDLESTR_LEN + 1];
  अक्षर x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  अक्षर *x_exitstr;

  mesg[1] = state;

  अगर (include_idle && !short_output && म_माप (idle) < माप x_idle - 1)
    प्र_लिखो (x_idle, " %-6s", idle);
  अन्यथा
    *x_idle = '\0';

  अगर (!short_output && म_माप (pid) < माप x_pid - 1)
    प्र_लिखो (x_pid, " %10s", pid);
  अन्यथा
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, म_माप (exitstr)) + 1 : 1);
  अगर (include_exit)
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
                   पंक्ति,
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
    जबतक (*--p == ' ')
      /* empty */;
    *(p + 1) = '\0';
  }

  माला_दो (buf);
  मुक्त (buf);
  मुक्त (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
अटल bool
is_tty_writable (काष्ठा stat स्थिर *pstat)
{
#अगर_घोषित TTY_GROUP_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  काष्ठा group *ttygr = getgrnam (TTY_GROUP_NAME);
  अगर (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    वापस false;
#पूर्ण_अगर

  वापस pstat->st_mode & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
अटल व्योम
print_user (STRUCT_UTMP स्थिर *utmp_ent, समय_प्रकार boottime)
{
  काष्ठा stat stats;
  समय_प्रकार last_change;
  अक्षर mesg;
  अक्षर idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  अटल अक्षर *hoststr;
#अगर HAVE_STRUCT_XTMP_UT_HOST
  अटल idx_t hostlen;
#पूर्ण_अगर

  /* If ut_line contains a space, the device name starts after the space.  */
  अक्षर *पंक्ति = utmp_ent->ut_पंक्ति;
  अक्षर *space = म_अक्षर (पंक्ति, ' ');
  पंक्ति = space ? space + 1 : पंक्ति;

  पूर्णांक dirfd;
  अगर (IS_ABSOLUTE_FILE_NAME (पंक्ति))
    dirfd = AT_FDCWD;
  अन्यथा
    {
      अटल पूर्णांक dev_dirfd;
      अगर (!dev_dirfd)
        {
          dev_dirfd = खोलो ("/dev", O_PATHSEARCH | O_DIRECTORY);
          अगर (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  अगर (AT_FDCWD <= dirfd && fstatat (dirfd, पंक्ति, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      last_change = stats.st_atime;
    }
  अन्यथा
    {
      mesg = '?';
      last_change = 0;
    }

  अगर (last_change)
    प्र_लिखो (idlestr, "%.*s", IDLESTR_LEN, idle_string (last_change, boottime));
  अन्यथा
    प्र_लिखो (idlestr, "  ?");

#अगर HAVE_STRUCT_XTMP_UT_HOST
  अगर (utmp_ent->ut_host[0])
    {
      अक्षर *host = शून्य;
      अक्षर *display = शून्य;
      अक्षर *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = म_अक्षर (ut_host, ':');
      अगर (display)
        *display++ = '\0';

      अगर (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      अगर (! host)
        host = ut_host;

      अगर (display)
        {
          idx_t needed = म_माप (host) + म_माप (display) + 4;
          अगर (hostlen < needed)
            {
              मुक्त (hoststr);
              hoststr = xpalloc (शून्य, &hostlen, needed - hostlen, -1, 1);
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
          अगर (hostlen < needed)
            {
              मुक्त (hoststr);
              hoststr = xpalloc (शून्य, &hostlen, needed - hostlen, -1, 1);
            }
          अक्षर *p = hoststr;
          *p++ = '(';
          म_नकल (stpcpy (p, host), ")");
        }

      अगर (host != ut_host)
        मुक्त (host);
    }
  अन्यथा
    {
      अगर (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#पूर्ण_अगर

  print_पंक्ति (utmp_ent->ut_user, mesg,
              utmp_ent->ut_पंक्ति,
              time_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

अटल व्योम
print_boottime (STRUCT_UTMP स्थिर *utmp_ent)
{
  print_पंक्ति ("", ' ', _("system boot"),
              time_string (utmp_ent), "", "", "", "");
}

अटल अक्षर *
make_id_equals_comment (STRUCT_UTMP स्थिर *utmp_ent)
{
  अक्षर स्थिर *id = UT_ID (utmp_ent);
  idx_t idlen = म_माप (id);
  अक्षर स्थिर *prefix = _("id=");
  idx_t prefixlen = म_माप (prefix);
  अक्षर *comment = xmalloc (prefixlen + idlen + 1);
  अक्षर *p = mempcpy (comment, prefix, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  वापस comment;
}

अटल व्योम
print_deadprocs (STRUCT_UTMP स्थिर *utmp_ent)
{
  अटल अक्षर *exitstr;
  अक्षर *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  अगर (!exitstr)
    exitstr = xmalloc (म_माप (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + म_माप (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  प्र_लिखो (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_पंक्ति ("", ' ', utmp_ent->ut_पंक्ति,
              time_string (utmp_ent), "", pidstr, comment, exitstr);
  मुक्त (comment);
}

अटल व्योम
print_login (STRUCT_UTMP स्थिर *utmp_ent)
{
  अक्षर *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_पंक्ति (_("LOGIN"), ' ', utmp_ent->ut_पंक्ति,
              time_string (utmp_ent), "", pidstr, comment, "");
  मुक्त (comment);
}

अटल व्योम
print_initspawn (STRUCT_UTMP स्थिर *utmp_ent)
{
  अक्षर *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_पंक्ति ("", ' ', utmp_ent->ut_पंक्ति,
              time_string (utmp_ent), "", pidstr, comment, "");
  मुक्त (comment);
}

अटल व्योम
print_clockchange (STRUCT_UTMP स्थिर *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_पंक्ति ("", ' ', _("clock change"),
              time_string (utmp_ent), "", "", "", "");
}

अटल व्योम
print_runlevel (STRUCT_UTMP स्थिर *utmp_ent)
{
  अटल अक्षर *runlevline, *comment;
  अचिन्हित अक्षर last = utmp_ent->ut_pid / 256;
  अचिन्हित अक्षर curr = utmp_ent->ut_pid % 256;

  अगर (!runlevline)
    runlevline = xmalloc (म_माप (_("run-level")) + 3);
  प्र_लिखो (runlevline, "%s %c", _("run-level"), curr);

  अगर (!comment)
    comment = xmalloc (म_माप (_("last=")) + 2);
  प्र_लिखो (comment, "%s%c", _("last="), (last == 'N') ? 'S' : last);

  print_पंक्ति ("", ' ', runlevline, time_string (utmp_ent),
              "", "", c_isprint (last) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
अटल व्योम
list_entries_who (idx_t n, STRUCT_UTMP स्थिर *utmp_buf)
{
  idx_t entries = 0;

  जबतक (n--)
    {
      अगर (IS_USER_PROCESS (utmp_buf))
        {
          अक्षर *trimmed_name;

          trimmed_name = extract_trimmed_name (utmp_buf);

          अगर (entries)
            अक्षर_दो (' ');
          ख_माला_दो (trimmed_name, मानक_निकास);
          मुक्त (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  म_लिखो (_("\n# users=%td\n"), entries);
}

अटल व्योम
print_heading (व्योम)
{
  print_पंक्ति (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
अटल व्योम
scan_entries (idx_t n, STRUCT_UTMP स्थिर *utmp_buf)
{
  अक्षर *ttyname_b IF_LINT ( = शून्य);
  समय_प्रकार boottime = TYPE_MINIMUM (समय_प्रकार);

  अगर (include_heading)
    print_heading ();

  अगर (my_पंक्ति_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      अगर (!ttyname_b)
        वापस;
      अगर (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += माप "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  जबतक (n--)
    {
      अगर (!my_पंक्ति_only
          || str_endswith (utmp_buf->ut_पंक्ति, ttyname_b))
        {
          अगर (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          अन्यथा अगर (need_runlevel && UT_TYPE_RUN_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          अन्यथा अगर (need_boottime && UT_TYPE_BOOT_TIME (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          अन्यथा अगर (need_clockchange && UT_TYPE_NEW_TIME (utmp_buf))
            print_clockchange (utmp_buf);
          अन्यथा अगर (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          अन्यथा अगर (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          अन्यथा अगर (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      अगर (UT_TYPE_BOOT_TIME (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
अटल व्योम
who (अक्षर स्थिर *filename, पूर्णांक options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  अगर (short_list)
    options |= READ_UTMP_USER_PROCESS;
  अगर (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (filename));

  अगर (short_list)
    list_entries_who (n_users, utmp_buf);
  अन्यथा
    scan_entries (n_users, utmp_buf);

  मुक्त (utmp_buf);
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
  -H, --heading     print पंक्ति of column headings\n\
"));
      oputs (_("\
  -l, --login       print प्रणाली login processes\n\
"));
      oputs (_("\
      --lookup      attempt to canonicalize hostnames via DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard निवेश\n\
"));
      oputs (_("\
  -p, --process     print active processes spawned by init\n\
"));
      oputs (_("\
  -q, --गिनती       all login names and संख्या of users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --लघु       print only name, पंक्ति, and समय (शेष)\n\
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
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, शून्य))
         != -1)
    {
      चयन (optc)
        {
        हाल 'a':
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
          अवरोध;

        हाल 'b':
          need_boottime = true;
          assumptions = false;
          अवरोध;

        हाल 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          अवरोध;

        हाल 'H':
          include_heading = true;
          अवरोध;

        हाल 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          अवरोध;

        हाल 'm':
          my_पंक्ति_only = true;
          अवरोध;

        हाल 'p':
          need_initspawn = true;
          assumptions = false;
          अवरोध;

        हाल 'q':
          short_list = true;
          अवरोध;

        हाल 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          अवरोध;

        हाल 's':
          short_output = true;
          अवरोध;

        हाल 't':
          need_clockchange = true;
          assumptions = false;
          अवरोध;

        हाल 'T':
        हाल 'w':
          include_mesg = true;
          अवरोध;

        हाल 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          अवरोध;

        हाल LOOKUP_OPTION:
          do_lookup = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  अगर (include_exit)
    {
      short_output = false;
    }

  अगर (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  अन्यथा
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  चयन (argc - optind)
    {
    हाल 2:			/* who <blurf> <glop> */
      my_पंक्ति_only = true;
      FALLTHROUGH;
    हाल -1:
    हाल 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      अवरोध;

    हाल 1:			/* who <utmp file> */
      who (argv[optind], 0);
      अवरोध;

    शेष:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (निकास_त्रुटि);
    }

  वापस निकास_सफल;
}
