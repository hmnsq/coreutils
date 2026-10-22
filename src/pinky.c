/* GNU's pinky.
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

/* Created by hacking who.c by Kaveh Ghazi ghazi@caip.rutgers.edu */

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <stdio.h>

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "canon-host.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "filenamecat.h"
#अन्तर्भाव "full-write.h"
#अन्तर्भाव "hard-locale.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pinky"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
स्थिर bool include_idle = true;

/* If true, display a line at the top describing each field. */
स्थिर bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
स्थिर bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
स्थिर bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
स्थिर bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
स्थिर bool include_home_and_shell = true;

/* if true, use the "short" output format. */
स्थिर bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
स्थिर bool do_lookup;

/* if true, display the ut_host field. */
#जर HAVE_STRUCT_XTMP_UT_HOST
स्थिर bool include_where = true;
#endif

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
  {"lookup", no_argument, रिक्त, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_PURE
स्थिर idx_t
मोजणी_ampersands (अक्षर स्थिरांक *str)
{
  idx_t मोजणी = 0;
  साठी (; *str; str++)
    मोजणी += *str == '&';
  परत मोजणी;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

स्थिर अक्षर *
create_fullname (अक्षर स्थिरांक *gecos_name, अक्षर स्थिरांक *user_name)
{
  idx_t rsize = म_माप (gecos_name) + 1;
  idx_t ampersands = मोजणी_ampersands (gecos_name);

  जर (ampersands != 0)
    {
      idx_t ulen = म_माप (user_name);
      सूचक_भेद_प्रकार product;
      जर (ckd_mul (&product, ulen - 1, ampersands)
          || ckd_add (&rsize, rsize, product))
        xalloc_die ();
    }

  अक्षर *result = xmalloc (rsize);
  अक्षर *r = result;

  पर्यंत (*gecos_name)
    {
      जर (*gecos_name == '&')
        {
          अक्षर स्थिरांक *uname = user_name;
          जर (है_छोटा (to_uchar (*uname)))
            *r++ = बड़े (to_uchar (*uname++));
          पर्यंत (*uname)
            *r++ = *uname++;
        }
      अन्यथा
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  परत result;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

स्थिर अक्षर स्थिरांक *
idle_string (समय_प्रकार when)
{
  स्थिर समय_प्रकार now = 0;
  स्थिर अक्षर buf[INT_STRLEN_BOUND (intmax_t) + आकार "d"];

  जर (now == 0)
    समय (&now);

  समय_प्रकार seconds_idle = now - when;
  जर (seconds_idle < 60)	/* One minute. */
    परत "     ";
  जर (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      पूर्णांक hours = seconds_idle / (60 * 60);
      पूर्णांक minutes = (seconds_idle % (60 * 60)) / 60;
      प्र_लिखो (buf, "%02d:%02d", hours, minutes);
    }
  अन्यथा
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      प्र_लिखो (buf, "%jdd", days);
    }
  परत buf;
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

/* Display a line of information about UTMP_ENT. */

स्थिर शून्य
print_entry (STRUCT_UTMP स्थिरांक *utmp_ent)
{
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

  संरचना stat stats;
  समय_प्रकार last_change;
  अक्षर mesg;
  जर (AT_FDCWD <= dirfd && fstatat (dirfd, ओळ, &stats, 0) == 0)
    {
      mesg = (stats.st_mode & S_IWGRP) ? ' ' : '*';
      last_change = stats.st_atime;
    }
  अन्यथा
    {
      mesg = '?';
      last_change = 0;
    }

  अक्षर *ut_user = utmp_ent->ut_user;
  जर (strnlen (ut_user, 8) < 8)
    म_लिखो ("%-8s", ut_user);
  अन्यथा
    ख_माला_दो (ut_user, मानक_निकास);

  जर (include_fullname)
    {
      संरचना passwd *pw = getpwnam (ut_user);
      जर (pw == रिक्त)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        म_लिखो (" %19s", _("        ???"));
      अन्यथा
        {
          अक्षर *स्थिरांक comma = म_अक्षर (pw->pw_gecos, ',');

          जर (comma)
            *comma = '\0';

          अक्षर *result = create_fullname (pw->pw_gecos, pw->pw_name);
          म_लिखो (" %-19.19s", result);
          मुक्त (result);
        }
    }

  ख_अक्षर_दो (' ', मानक_निकास);
  ख_अक्षर_दो (mesg, मानक_निकास);
  जर (strnlen (utmp_ent->ut_ओळ, 8) < 8)
    म_लिखो ("%-8s", utmp_ent->ut_ओळ);
  अन्यथा
    ख_माला_दो (utmp_ent->ut_ओळ, मानक_निकास);

  जर (include_idle)
    {
      जर (last_change)
        म_लिखो (" %-6s", idle_string (last_change));
      अन्यथा
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        म_लिखो (" %-6s", _("?????"));
    }

  म_लिखो (" %s", time_string (utmp_ent));

#ifdef HAVE_STRUCT_XTMP_UT_HOST
  जर (include_where && utmp_ent->ut_host[0])
    {
      अक्षर *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      अक्षर *display = म_अक्षर (ut_host, ':');
      जर (display)
        *display++ = '\0';

      अक्षर *host = रिक्त;
      जर (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      जर ( ! host)
        host = ut_host;

      ख_अक्षर_दो (' ', मानक_निकास);
      ख_माला_दो (host, मानक_निकास);
      जर (display)
        {
          ख_अक्षर_दो (':', मानक_निकास);
          ख_माला_दो (display, मानक_निकास);
        }

      जर (host != ut_host)
        मुक्त (host);
    }
#endif

  अक्षर_दो ('\n');

  जर (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

स्थिर शून्य
cat_file (अक्षर स्थिरांक *header, अक्षर स्थिरांक *home, अक्षर स्थिरांक *file)
{
  अक्षर *full_name = file_name_concat (home, file, रिक्त);
  खाता *fp = ख_खोलो (full_name, "r");

  जर (fp)
    {
      ख_माला_दो (header, मानक_निकास);

      fadvise (fp, FADVISE_SEQUENTIAL);

      अक्षर buf[बफ_मान];
      साठी (माप_प्रकार bytes_read;
           0 < (bytes_read = ख_पढ़ो (buf, 1, आकार buf, fp));)
        जर (ख_डालो (buf, 1, bytes_read, मानक_निकास) != bytes_read)
          write_error ();

      ख_बंद (fp);
    }

  मुक्त (full_name);
}

/* Display a verbose line of information about UTMP_ENT. */

स्थिर शून्य
print_long_entry (स्थिरांक अक्षर name[])
{
  संरचना passwd *pw = getpwnam (name);

  म_लिखो (_("Login name: "));
  म_लिखो ("%-28s", name);

  म_लिखो (_("In real life: "));
  जर (pw == रिक्त)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      म_लिखो (" %s", _("???\n"));
      परत;
    }
  अन्यथा
    {
      अक्षर *स्थिरांक comma = म_अक्षर (pw->pw_gecos, ',');

      जर (comma)
        *comma = '\0';

      अक्षर *result = create_fullname (pw->pw_gecos, pw->pw_name);
      म_लिखो (" %s", result);
      मुक्त (result);
    }

  अक्षर_दो ('\n');

  जर (include_home_and_shell)
    {
      म_लिखो (_("Directory: "));
      म_लिखो ("%-29s", pw->pw_dir);
      म_लिखो (_("Shell: "));
      म_लिखो (" %s", pw->pw_shell);
      अक्षर_दो ('\n');
    }

  जर (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  जर (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  अक्षर_दो ('\n');

  जर (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

स्थिर शून्य
print_heading (शून्य)
{
  म_लिखो ("%-8s", _("Login"));
  जर (include_fullname)
    म_लिखो (" %-19s", _("Name"));
  म_लिखो (" %-9s", _(" TTY"));
  जर (include_idle)
    म_लिखो (" %-6s", _("Idle"));
  म_लिखो (" %-*s", time_format_width, _("When"));
#ifdef HAVE_STRUCT_XTMP_UT_HOST
  जर (include_where)
    म_लिखो (" %s", _("Where"));
#endif
  अक्षर_दो ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

स्थिर शून्य
scan_entries (idx_t n, STRUCT_UTMP स्थिरांक *utmp_buf,
              स्थिरांक पूर्णांक argc_names, अक्षर *स्थिरांक argv_names[])
{
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

  जर (include_heading)
    print_heading ();

  पर्यंत (n--)
    {
      जर (IS_USER_PROCESS (utmp_buf))
        {
          जर (argc_names)
            {
              साठी (पूर्णांक i = 0; i < argc_names; i++)
                जर (streq (utmp_buf->ut_user, argv_names[i]))
                  {
                    print_entry (utmp_buf);
                    थांबा;
                  }
            }
          अन्यथा
            print_entry (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

स्थिर शून्य
short_pinky (अक्षर स्थिरांक *filename,
             स्थिरांक पूर्णांक argc_names, अक्षर *स्थिरांक argv_names[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  जर (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (filename));

  scan_entries (n_users, utmp_buf, argc_names, argv_names);
  निकास (निकास_सफल);
}

स्थिर शून्य
long_pinky (स्थिरांक पूर्णांक argc_names, अक्षर *स्थिरांक argv_names[])
{
  साठी (पूर्णांक i = 0; i < argc_names; i++)
    print_long_entry (argv_names[i]);
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      ख_माला_दो (_("\
\n\
"), मानक_निकास);
      oputs (_("\
  -l     produce दीर्घ format output साठी the specified USERs\n\
"));
      oputs (_("\
  -b     omit the user's home directory and shell in दीर्घ format\n\
"));
      oputs (_("\
  -h     omit the user's project file in दीर्घ format\n\
"));
      oputs (_("\
  -p     omit the user's plan file in दीर्घ format\n\
"));
      oputs (_("\
  -s     करा लघु format output, this is the पूर्वनिर्धारित\n\
"));
      oputs (_("\
  -f     omit the ओळ of column headings in लघु format\n\
"));
      oputs (_("\
  -w     omit the user's full name in लघु format\n\
"));
      oputs (_("\
  -i     omit the user's full name and remote host in लघु format\n\
"));
      oputs (_("\
  -q     omit the user's full name, remote host and idle समय in लघु format\n\
"));
      oputs (_("\
      --lookup\n\
         attempt to canonicalize hostnames via DNS\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      म_लिखो (_("\
\n\
A lightweight 'finger' program;  print user information.\n\
The utmp file will be %s.\n\
"), UTMP_FILE);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णांक optc;
  पर्यंत ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, रिक्त))
         != -1)
    {
      निवड (optc)
        {
        स्थिती 's':
          do_short_format = true;
          थांबा;

        स्थिती 'l':
          do_short_format = false;
          थांबा;

        स्थिती 'f':
          include_heading = false;
          थांबा;

        स्थिती 'w':
          include_fullname = false;
          थांबा;

        स्थिती 'i':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          थांबा;

        स्थिती 'q':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          include_idle = false;
          थांबा;

        स्थिती 'h':
          include_project = false;
          थांबा;

        स्थिती 'p':
          include_plan = false;
          थांबा;

        स्थिती 'b':
          include_home_and_shell = false;
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

  पूर्णांक n_users = argc - optind;

  जर (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at least one must be\
 specified when using -l"));
      usage (निकास_त्रुटि);
    }

  जर (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  अन्यथा
    long_pinky (n_users, argv + optind);

  परत निकास_सफल;
}
