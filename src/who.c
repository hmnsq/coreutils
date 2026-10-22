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

#यदि_परिभाषितम् TTY_गणः_NAME
# include <grp.h>
#यदि_अन्तः

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "who"

#परिभाषय AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Michael Stone")

#यदि_परिभाषितम् सततम्_LVL
# define UT_TYPE_सततम्_LVL(U) ((U)->ut_type == सततम्_LVL)
#अन्यथा
# define UT_TYPE_सततम्_LVL(U) false
#यदि_अन्तः

#यदि_परिभाषितम् INIT_PROCESS
# define UT_TYPE_INIT_PROCESS(U) ((U)->ut_type == INIT_PROCESS)
#अन्यथा
# define UT_TYPE_INIT_PROCESS(U) false
#यदि_अन्तः

#यदि_परिभाषितम् DEAD_PROCESS
# define UT_TYPE_DEAD_PROCESS(U) ((U)->ut_type == DEAD_PROCESS)
#अन्यथा
# define UT_TYPE_DEAD_PROCESS(U) false
#यदि_अन्तः

#यदि_परिभाषितम् NEW_TIME
# define UT_TYPE_NEW_TIME(U) ((U)->ut_type == NEW_TIME)
#अन्यथा
# define UT_TYPE_NEW_TIME(U) false
#यदि_अन्तः

#परिभाषय IDLESTR_LEN 6

#यदि HAVE_STRUCT_XTMP_UT_PID
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  वर्ण Var[INT_STRLEN_BOUND (Utmp_ent->ut_pid) + 1]; \
  sprintf (Var, "%ld", (दीर्घ पूर्णाङ्क) (Utmp_ent->ut_pid))
#अन्यथा
# define PIDSTR_DECL_AND_INIT(Var, Utmp_ent) \
  वर्ण ध्रुव *Var = ""
#यदि_अन्तः

#यदि HAVE_STRUCT_XTMP_UT_ID
# define UT_ID(U) ((U)->ut_id)
#अन्यथा
# define UT_ID(U) "??"
#यदि_अन्तः

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
स्थिर bool my_पङ्क्तिः_only;

/* The strftime format to use for login times, and its expected
   output width.  */
स्थिर वर्ण ध्रुव *time_format;
स्थिर पूर्णाङ्क time_format_विस्तारः;

/* for long options with no corresponding short option, use enum */
गणना
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"all", no_तर्कः, रिक्त, 'a'},
  {"boot", no_तर्कः, रिक्त, 'b'},
  {"count", no_तर्कः, रिक्त, 'q'},
  {"dead", no_तर्कः, रिक्त, 'd'},
  {"heading", no_तर्कः, रिक्त, 'H'},
  {"login", no_तर्कः, रिक्त, 'l'},
  {"lookup", no_तर्कः, रिक्त, LOOKUP_OPTION},
  {"message", no_तर्कः, रिक्त, 'T'},
  {"mesg", no_तर्कः, रिक्त, 'T'},
  {"process", no_तर्कः, रिक्त, 'p'},
  {"runlevel", no_तर्कः, रिक्त, 'r'},
  {"short", no_तर्कः, रिक्त, 's'},
  {"time", no_तर्कः, रिक्त, 't'},
  {"users", no_तर्कः, रिक्त, 'u'},
  {"writable", no_तर्कः, रिक्त, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Return a string representing the time between WHEN and now.
   BOOTTIME is the time of last reboot.
   FIXME: locale? */
स्थिर वर्ण ध्रुव *
idle_string (time_t when, time_t boottime)
{
  स्थिर time_t now = TYPE_MINIMUM (time_t);

  यदि (now == TYPE_MINIMUM (time_t))
    time (&now);

  पूर्णाङ्क seconds_idle;
  यदि (boottime < when && when <= now
      && ! ckd_sub (&seconds_idle, now, when)
      && seconds_idle < 24 * 60 * 60)
    {
      यदि (seconds_idle < 60)
        प्रत्यागम "  .  ";
      अन्यथा
        {
          स्थिर वर्ण idle_hhmm[IDLESTR_LEN];
          sprintf (idle_hhmm, "%02d:%02d",
                   seconds_idle / (60 * 60),
                   (seconds_idle % (60 * 60)) / 60);
          प्रत्यागम idle_hhmm;
        }
    }

  प्रत्यागम _(" old ");
}

/* Return a time string.  */
स्थिर वर्ण ध्रुव *
time_string (STRUCT_UTMP ध्रुव *utmp_ent)
{
  स्थिर वर्ण buf[INT_STRLEN_BOUND (intmax_t) + परिमाण "-%m-%d %H:%M"];
  संरचना tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  यदि (tmp)
    {
      strftime (buf, परिमाण buf, time_format, tmp);
      प्रत्यागम buf;
    }
  अन्यथा
    प्रत्यागम timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Print formatted output line. Uses mostly arbitrary field sizes, probably
   will need tweaking if any of the localization stuff is done, or for 64 bit
   pids, etc. */
स्थिर शून्य
print_पङ्क्तिः (वर्ण ध्रुव *user, ध्रुव वर्ण state,
            वर्ण ध्रुव *पङ्क्तिः,
            वर्ण ध्रुव *time_str, वर्ण ध्रुव *idle, वर्ण ध्रुव *pid,
            वर्ण ध्रुव *comment, वर्ण ध्रुव *exitstr)
{
  स्थिर वर्ण mesg[3] = { ' ', 'x', '\0' };
  वर्ण *buf;
  वर्ण x_idle[1 + IDLESTR_LEN + 1];
  वर्ण x_pid[1 + INT_STRLEN_BOUND (pid_t) + 1];
  वर्ण *x_exitstr;

  mesg[1] = state;

  यदि (include_idle && !short_output && वाक्यदैर्घ्यम् (idle) < परिमाण x_idle - 1)
    sprintf (x_idle, " %-6s", idle);
  अन्यथा
    *x_idle = '\0';

  यदि (!short_output && वाक्यदैर्घ्यम् (pid) < परिमाण x_pid - 1)
    sprintf (x_pid, " %10s", pid);
  अन्यथा
    *x_pid = '\0';

  x_exitstr = xmalloc (include_exit ? 1 + MAX (12, वाक्यदैर्घ्यम् (exitstr)) + 1 : 1);
  यदि (include_exit)
    sprintf (x_exitstr, " %-12s", exitstr);
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
                   पङ्क्तिः,
                   time_format_विस्तारः,
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
    वर्ण *p = buf + वाक्यदैर्घ्यम् (buf);
    यावत् (*--p == ' ')
      /*रिक्तानि*/;
    *(p + 1) = '\0';
  }

  पङ्क्तिं_लिख (buf);
  मुक्त (buf);
  मुक्त (x_exitstr);
}

/* Return true if a terminal device given as PSTAT allows other users
   to send messages to; false otherwise */
स्थिर bool
is_tty_writable (संरचना stat ध्रुव *pstat)
{
#यदि_परिभाषितम् TTY_गणः_NAME
  /* Ensure the group of the TTY device matches TTY_GROUP_NAME, more info at
     https://bugzilla.redhat.com/454261 */
  संरचना गणः *ttygr = getgrnam (TTY_गणः_NAME);
  यदि (!ttygr || (pstat->st_gid != ttygr->gr_gid))
    प्रत्यागम false;
#यदि_अन्तः

  प्रत्यागम pstat->st_बहुलकम् & S_IWGRP;
}

/* Send properly parsed USER_PROCESS info to print_line.  The most
   recent boot time is BOOTTIME. */
स्थिर शून्य
print_user (STRUCT_UTMP ध्रुव *utmp_ent, time_t boottime)
{
  संरचना stat stats;
  time_t चरमः_change;
  वर्ण mesg;
  वर्ण idlestr[IDLESTR_LEN + 1];
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);
  स्थिर वर्ण *hoststr;
#यदि HAVE_STRUCT_XTMP_UT_HOST
  स्थिर idx_t hostlen;
#यदि_अन्तः

  /* If ut_line contains a space, the device name starts after the space.  */
  वर्ण *पङ्क्तिः = utmp_ent->ut_पङ्क्तिः;
  वर्ण *space = strchr (पङ्क्तिः, ' ');
  पङ्क्तिः = space ? space + 1 : पङ्क्तिः;

  पूर्णाङ्क dirfd;
  यदि (IS_ABSOLUTE_FILE_NAME (पङ्क्तिः))
    dirfd = AT_FDCWD;
  अन्यथा
    {
      स्थिर पूर्णाङ्क dev_dirfd;
      यदि (!dev_dirfd)
        {
          dev_dirfd = उद्घाटय ("/dev", O_PATHSEARCH | O_DIRECTORY);
          यदि (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  यदि (AT_FDCWD <= dirfd && fstatat (dirfd, पङ्क्तिः, &stats, 0) == 0)
    {
      mesg = is_tty_writable (&stats) ? '+' : '-';
      चरमः_change = stats.st_atime;
    }
  अन्यथा
    {
      mesg = '?';
      चरमः_change = 0;
    }

  यदि (चरमः_change)
    sprintf (idlestr, "%.*s", IDLESTR_LEN, idle_string (चरमः_change, boottime));
  अन्यथा
    sprintf (idlestr, "  ?");

#यदि HAVE_STRUCT_XTMP_UT_HOST
  यदि (utmp_ent->ut_host[0])
    {
      वर्ण *host = रिक्त;
      वर्ण *display = रिक्त;
      वर्ण *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      display = strchr (ut_host, ':');
      यदि (display)
        *display++ = '\0';

      यदि (*ut_host && do_lookup)
        {
          /* See if we can canonicalize it.  */
          host = canon_host (ut_host);
        }

      यदि (! host)
        host = ut_host;

      यदि (display)
        {
          idx_t needed = वाक्यदैर्घ्यम् (host) + वाक्यदैर्घ्यम् (display) + 4;
          यदि (hostlen < needed)
            {
              मुक्त (hoststr);
              hoststr = xpalloc (रिक्त, &hostlen, needed - hostlen, -1, 1);
            }
          वर्ण *p = hoststr;
          *p++ = '(';
          p = stpcpy (p, host);
          *p++ = ':';
          वाक्यं_प्रतिलिख (stpcpy (p, display), ")");
        }
      अन्यथा
        {
          idx_t needed = वाक्यदैर्घ्यम् (host) + 3;
          यदि (hostlen < needed)
            {
              मुक्त (hoststr);
              hoststr = xpalloc (रिक्त, &hostlen, needed - hostlen, -1, 1);
            }
          वर्ण *p = hoststr;
          *p++ = '(';
          वाक्यं_प्रतिलिख (stpcpy (p, host), ")");
        }

      यदि (host != ut_host)
        मुक्त (host);
    }
  अन्यथा
    {
      यदि (hostlen < 1)
        hoststr = xpalloc (hoststr, &hostlen, 1, -1, 1);
      *hoststr = '\0';
    }
#यदि_अन्तः

  print_पङ्क्तिः (utmp_ent->ut_user, mesg,
              utmp_ent->ut_पङ्क्तिः,
              time_string (utmp_ent), idlestr, pidstr,
              hoststr ? hoststr : "", "");
}

स्थिर शून्य
print_boottime (STRUCT_UTMP ध्रुव *utmp_ent)
{
  print_पङ्क्तिः ("", ' ', _("system boot"),
              time_string (utmp_ent), "", "", "", "");
}

स्थिर वर्ण *
make_id_equals_comment (STRUCT_UTMP ध्रुव *utmp_ent)
{
  वर्ण ध्रुव *id = UT_ID (utmp_ent);
  idx_t idlen = वाक्यदैर्घ्यम् (id);
  वर्ण ध्रुव *उपसर्गः = _("id=");
  idx_t prefixlen = वाक्यदैर्घ्यम् (उपसर्गः);
  वर्ण *comment = xmalloc (prefixlen + idlen + 1);
  वर्ण *p = mempcpy (comment, उपसर्गः, prefixlen);
  p = mempcpy (p, id, idlen);
  *p = '\0';
  प्रत्यागम comment;
}

स्थिर शून्य
print_deadprocs (STRUCT_UTMP ध्रुव *utmp_ent)
{
  स्थिर वर्ण *exitstr;
  वर्ण *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  यदि (!exitstr)
    exitstr = xmalloc (वाक्यदैर्घ्यम् (_("term="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_termination) + 1
                       + वाक्यदैर्घ्यम् (_("exit="))
                       + INT_STRLEN_BOUND (utmp_ent->ut_exit.e_exit)
                       + 1);
  sprintf (exitstr, "%s%d %s%d", _("term="), utmp_ent->ut_exit.e_termination,
           _("exit="), utmp_ent->ut_exit.e_exit);

  /* FIXME: add idle time? */

  print_पङ्क्तिः ("", ' ', utmp_ent->ut_पङ्क्तिः,
              time_string (utmp_ent), "", pidstr, comment, exitstr);
  मुक्त (comment);
}

स्थिर शून्य
print_login (STRUCT_UTMP ध्रुव *utmp_ent)
{
  वर्ण *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  /* FIXME: add idle time? */

  print_पङ्क्तिः (_("LOGIN"), ' ', utmp_ent->ut_पङ्क्तिः,
              time_string (utmp_ent), "", pidstr, comment, "");
  मुक्त (comment);
}

स्थिर शून्य
print_initspawn (STRUCT_UTMP ध्रुव *utmp_ent)
{
  वर्ण *comment = make_id_equals_comment (utmp_ent);
  PIDSTR_DECL_AND_INIT (pidstr, utmp_ent);

  print_पङ्क्तिः ("", ' ', utmp_ent->ut_पङ्क्तिः,
              time_string (utmp_ent), "", pidstr, comment, "");
  मुक्त (comment);
}

स्थिर शून्य
print_clockchange (STRUCT_UTMP ध्रुव *utmp_ent)
{
  /* FIXME: handle NEW_TIME & OLD_TIME both */
  print_पङ्क्तिः ("", ' ', _("clock change"),
              time_string (utmp_ent), "", "", "", "");
}

स्थिर शून्य
print_runlevel (STRUCT_UTMP ध्रुव *utmp_ent)
{
  स्थिर वर्ण *runlevline, *comment;
  अचिह्नित वर्ण चरमः = utmp_ent->ut_pid / 256;
  अचिह्नित वर्ण curr = utmp_ent->ut_pid % 256;

  यदि (!runlevline)
    runlevline = xmalloc (वाक्यदैर्घ्यम् (_("run-level")) + 3);
  sprintf (runlevline, "%s %c", _("run-level"), curr);

  यदि (!comment)
    comment = xmalloc (वाक्यदैर्घ्यम् (_("last=")) + 2);
  sprintf (comment, "%s%c", _("last="), (चरमः == 'N') ? 'S' : चरमः);

  print_पङ्क्तिः ("", ' ', runlevline, time_string (utmp_ent),
              "", "", c_isprint (चरमः) ? comment : "", "");
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */
स्थिर शून्य
list_entries_who (idx_t n, STRUCT_UTMP ध्रुव *utmp_buf)
{
  idx_t entries = 0;

  यावत् (n--)
    {
      यदि (IS_USER_PROCESS (utmp_buf))
        {
          वर्ण *trimmed_name;

          trimmed_name = extract_trimmed_name (utmp_buf);

          यदि (entries)
            अक्षरं_लिख (' ');
          fputs (trimmed_name, stdout);
          मुक्त (trimmed_name);
          entries++;
        }
      utmp_buf++;
    }
  प्रारूप_लिख (_("\n# users=%td\n"), entries);
}

स्थिर शून्य
print_heading (शून्य)
{
  print_पङ्क्तिः (_("NAME"), ' ', _("LINE"), _("TIME"), _("IDLE"),
              _("PID"), _("COMMENT"), _("EXIT"));
}

/* Display UTMP_BUF, which should have N entries. */
स्थिर शून्य
अवलोकनम्_entries (idx_t n, STRUCT_UTMP ध्रुव *utmp_buf)
{
  वर्ण *ttyname_b IF_LINT ( = रिक्त);
  time_t boottime = TYPE_MINIMUM (time_t);

  यदि (include_heading)
    print_heading ();

  यदि (my_पङ्क्तिः_only)
    {
      ttyname_b = ttyname (STDIN_FILENO);
      यदि (!ttyname_b)
        प्रत्यागम;
      यदि (STRNCMP_LIT (ttyname_b, "/dev/") == 0)
        ttyname_b += परिमाण "/dev/" - 1;	/* Discard /dev/ prefix.  */
    }

  यावत् (n--)
    {
      यदि (!my_पङ्क्तिः_only
          || str_endswith (utmp_buf->ut_पङ्क्तिः, ttyname_b))
        {
          यदि (need_users && IS_USER_PROCESS (utmp_buf))
            print_user (utmp_buf, boottime);
          अन्यथा यदि (need_runlevel && UT_TYPE_सततम्_LVL (utmp_buf))
            print_runlevel (utmp_buf);
          अन्यथा यदि (need_boottime && UT_TYPE_BOOT_TIME (utmp_buf))
            print_boottime (utmp_buf);
          /* I've never seen one of these, so I don't know what it should
             look like :^)
             FIXME: handle OLD_TIME also, perhaps show the delta? */
          अन्यथा यदि (need_clockchange && UT_TYPE_NEW_TIME (utmp_buf))
            print_clockchange (utmp_buf);
          अन्यथा यदि (need_initspawn && UT_TYPE_INIT_PROCESS (utmp_buf))
            print_initspawn (utmp_buf);
          अन्यथा यदि (need_login && UT_TYPE_LOGIN_PROCESS (utmp_buf))
            print_login (utmp_buf);
          अन्यथा यदि (need_deadprocs && UT_TYPE_DEAD_PROCESS (utmp_buf))
            print_deadprocs (utmp_buf);
        }

      यदि (UT_TYPE_BOOT_TIME (utmp_buf))
        boottime = utmp_buf->ut_ts.tv_sec;

      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read the file.  */
स्थिर शून्य
who (वर्ण ध्रुव *filename, पूर्णाङ्क options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  यदि (short_list)
    options |= READ_UTMP_USER_PROCESS;
  यदि (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  यदि (short_list)
    list_entries_who (n_users, utmp_buf);
  अन्यथा
    अवलोकनम्_entries (n_users, utmp_buf);

  मुक्त (utmp_buf);
}

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]... [ FILE | ARG1 ARG2 ]\n"), क्रमादेशः_name);
      fputs (_("\
Print information about users who are currently logged अन्तर्.\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -a, --all         same as -b -d --login -p -r -t -T -u\n\
"));
      oputs (_("\
  -b, --boot        time सम्बन्धि चरमः system boot\n\
"));
      oputs (_("\
  -d, --dead        print dead processes\n\
"));
      oputs (_("\
  -H, --heading     print पङ्क्तिः सम्बन्धि column headings\n\
"));
      oputs (_("\
  -l, --login       print system login processes\n\
"));
      oputs (_("\
      --lookup      attempt गन्तव्यम् canonicalize hostnames द्वारा DNS\n\
"));
      oputs (_("\
  -m                only hostname and user associated with standard निवेशः\n\
"));
      oputs (_("\
  -p, --process     print सक्रिय processes spawned by init\n\
"));
      oputs (_("\
  -q, --गणनम्       all login नामानि and सङ्ख्या सम्बन्धि users logged on\n\
"));
      oputs (_("\
  -r, --runlevel    print current runlevel\n\
"));
      oputs (_("\
  -s, --ह्रस्व       print only name, पङ्क्तिः, and time (स्वभाव)\n\
"));
      oputs (_("\
  -t, --time        print चरमः system clock change\n\
"));
      oputs (_("\
  -T, -w, --mesg    सङ्कलय user's सन्देशः_2 स्थितिः as +, - or ?\n\
"));
      oputs (_("\
  -u, --users       list users logged अन्तर्, including idle time\n\
"));
      oputs (_("\
      --सन्देशः_2     same as -T\n\
"));
      oputs (_("\
      --writable    same as -T\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      प्रारूप_लिख (_("\
\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
If ARG1 ARG2 given, -m presumed: 'am i' or 'mom likes' are usual.\n\
"), UTMP_FILE, WTMP_FILE);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc;
  bool assumptions = true;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((optc = getopt_long (argc, argv, "abdlmpqrstuwHT", longopts, रिक्त))
         != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'a':
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
          भङ्ग;

        पक्ष 'b':
          need_boottime = true;
          assumptions = false;
          भङ्ग;

        पक्ष 'd':
          need_deadprocs = true;
          include_idle = true;
          include_exit = true;
          assumptions = false;
          भङ्ग;

        पक्ष 'H':
          include_heading = true;
          भङ्ग;

        पक्ष 'l':
          need_login = true;
          include_idle = true;
          assumptions = false;
          भङ्ग;

        पक्ष 'm':
          my_पङ्क्तिः_only = true;
          भङ्ग;

        पक्ष 'p':
          need_initspawn = true;
          assumptions = false;
          भङ्ग;

        पक्ष 'q':
          short_list = true;
          भङ्ग;

        पक्ष 'r':
          need_runlevel = true;
          include_idle = true;
          assumptions = false;
          भङ्ग;

        पक्ष 's':
          short_output = true;
          भङ्ग;

        पक्ष 't':
          need_clockchange = true;
          assumptions = false;
          भङ्ग;

        पक्ष 'T':
        पक्ष 'w':
          include_mesg = true;
          भङ्ग;

        पक्ष 'u':
          need_users = true;
          include_idle = true;
          assumptions = false;
          भङ्ग;

        पक्ष LOOKUP_OPTION:
          do_lookup = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (assumptions)
    {
      need_users = true;
      short_output = true;
    }

  यदि (include_exit)
    {
      short_output = false;
    }

  यदि (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_विस्तारः = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  अन्यथा
    {
      time_format = "%b %e %H:%M";
      time_format_विस्तारः = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  विकल्प (argc - optind)
    {
    पक्ष 2:			/* who <blurf> <glop> */
      my_पङ्क्तिः_only = true;
      FALLTHROUGH;
    पक्ष -1:
    पक्ष 0:			/* who */
      who (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      भङ्ग;

    पक्ष 1:			/* who <utmp file> */
      who (argv[optind], 0);
      भङ्ग;

    स्वभाव:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  प्रत्यागम EXIT_SUCCESS;
}
