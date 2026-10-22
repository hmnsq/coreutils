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

#समावेश <config.h>
#समावेश <getopt.h>
#समावेश <pwd.h>
#समावेश <stdio.h>

#समावेश <sys/types.h>
#समावेश "system.h"

#समावेश "canon-host.h"
#समावेश "fadvise.h"
#समावेश "filenamecat.h"
#समावेश "full-write.h"
#समावेश "hard-locale.h"
#समावेश "ioblksize.h"
#समावेश "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "pinky"

#घोषणा AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
अटल bool include_idle = true;

/* If true, display a line at the top describing each field. */
अटल bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
अटल bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
अटल bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
अटल bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
अटल bool include_home_and_shell = true;

/* if true, use the "short" output format. */
अटल bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
अटल bool do_lookup;

/* if true, display the ut_host field. */
#अगर HAVE_STRUCT_XTMP_UT_HOST
अटल bool include_where = true;
#पूर्ण_अगर

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
  {"lookup", no_argument, शून्य, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_PURE
अटल idx_t
गिनती_ampersands (अक्षर स्थिर *str)
{
  idx_t गिनती = 0;
  क्रम (; *str; str++)
    गिनती += *str == '&';
  वापस गिनती;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

अटल अक्षर *
create_fullname (अक्षर स्थिर *gecos_name, अक्षर स्थिर *user_name)
{
  idx_t rsize = म_माप (gecos_name) + 1;
  idx_t ampersands = गिनती_ampersands (gecos_name);

  अगर (ampersands != 0)
    {
      idx_t ulen = म_माप (user_name);
      सूचक_भेद_प्रकार product;
      अगर (ckd_mul (&product, ulen - 1, ampersands)
          || ckd_add (&rsize, rsize, product))
        xalloc_die ();
    }

  अक्षर *परिणाम = xmalloc (rsize);
  अक्षर *r = परिणाम;

  जबतक (*gecos_name)
    {
      अगर (*gecos_name == '&')
        {
          अक्षर स्थिर *uname = user_name;
          अगर (है_छोटा (to_uchar (*uname)))
            *r++ = बड़े (to_uchar (*uname++));
          जबतक (*uname)
            *r++ = *uname++;
        }
      अन्यथा
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  वापस परिणाम;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

अटल अक्षर स्थिर *
idle_string (समय_प्रकार when)
{
  अटल समय_प्रकार now = 0;
  अटल अक्षर buf[INT_STRLEN_BOUND (intmax_t) + माप "d"];

  अगर (now == 0)
    समय (&now);

  समय_प्रकार seconds_idle = now - when;
  अगर (seconds_idle < 60)	/* One minute. */
    वापस "     ";
  अगर (seconds_idle < (24 * 60 * 60))	/* One day. */
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
  वापस buf;
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

/* Display a line of information about UTMP_ENT. */

अटल व्योम
print_entry (STRUCT_UTMP स्थिर *utmp_ent)
{
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

  काष्ठा stat stats;
  समय_प्रकार last_change;
  अक्षर mesg;
  अगर (AT_FDCWD <= dirfd && fstatat (dirfd, पंक्ति, &stats, 0) == 0)
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
  अगर (strnlen (ut_user, 8) < 8)
    म_लिखो ("%-8s", ut_user);
  अन्यथा
    ख_माला_दो (ut_user, मानक_निकास);

  अगर (include_fullname)
    {
      काष्ठा passwd *pw = getpwnam (ut_user);
      अगर (pw == शून्य)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        म_लिखो (" %19s", _("        ???"));
      अन्यथा
        {
          अक्षर *स्थिर comma = म_अक्षर (pw->pw_gecos, ',');

          अगर (comma)
            *comma = '\0';

          अक्षर *परिणाम = create_fullname (pw->pw_gecos, pw->pw_name);
          म_लिखो (" %-19.19s", परिणाम);
          मुक्त (परिणाम);
        }
    }

  ख_अक्षर_दो (' ', मानक_निकास);
  ख_अक्षर_दो (mesg, मानक_निकास);
  अगर (strnlen (utmp_ent->ut_पंक्ति, 8) < 8)
    म_लिखो ("%-8s", utmp_ent->ut_पंक्ति);
  अन्यथा
    ख_माला_दो (utmp_ent->ut_पंक्ति, मानक_निकास);

  अगर (include_idle)
    {
      अगर (last_change)
        म_लिखो (" %-6s", idle_string (last_change));
      अन्यथा
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        म_लिखो (" %-6s", _("?????"));
    }

  म_लिखो (" %s", time_string (utmp_ent));

#अगर_घोषित HAVE_STRUCT_XTMP_UT_HOST
  अगर (include_where && utmp_ent->ut_host[0])
    {
      अक्षर *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      अक्षर *display = म_अक्षर (ut_host, ':');
      अगर (display)
        *display++ = '\0';

      अक्षर *host = शून्य;
      अगर (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      अगर ( ! host)
        host = ut_host;

      ख_अक्षर_दो (' ', मानक_निकास);
      ख_माला_दो (host, मानक_निकास);
      अगर (display)
        {
          ख_अक्षर_दो (':', मानक_निकास);
          ख_माला_दो (display, मानक_निकास);
        }

      अगर (host != ut_host)
        मुक्त (host);
    }
#पूर्ण_अगर

  अक्षर_दो ('\n');

  अगर (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

अटल व्योम
cat_file (अक्षर स्थिर *header, अक्षर स्थिर *home, अक्षर स्थिर *file)
{
  अक्षर *full_name = file_name_concat (home, file, शून्य);
  खाता *fp = ख_खोलो (full_name, "r");

  अगर (fp)
    {
      ख_माला_दो (header, मानक_निकास);

      fadvise (fp, FADVISE_SEQUENTIAL);

      अक्षर buf[बफ_मान];
      क्रम (माप_प्रकार bytes_read;
           0 < (bytes_read = ख_पढ़ो (buf, 1, माप buf, fp));)
        अगर (ख_डालो (buf, 1, bytes_read, मानक_निकास) != bytes_read)
          write_error ();

      ख_बंद (fp);
    }

  मुक्त (full_name);
}

/* Display a verbose line of information about UTMP_ENT. */

अटल व्योम
print_long_entry (स्थिर अक्षर name[])
{
  काष्ठा passwd *pw = getpwnam (name);

  म_लिखो (_("Login name: "));
  म_लिखो ("%-28s", name);

  म_लिखो (_("In real life: "));
  अगर (pw == शून्य)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      म_लिखो (" %s", _("???\n"));
      वापस;
    }
  अन्यथा
    {
      अक्षर *स्थिर comma = म_अक्षर (pw->pw_gecos, ',');

      अगर (comma)
        *comma = '\0';

      अक्षर *परिणाम = create_fullname (pw->pw_gecos, pw->pw_name);
      म_लिखो (" %s", परिणाम);
      मुक्त (परिणाम);
    }

  अक्षर_दो ('\n');

  अगर (include_home_and_shell)
    {
      म_लिखो (_("Directory: "));
      म_लिखो ("%-29s", pw->pw_dir);
      म_लिखो (_("Shell: "));
      म_लिखो (" %s", pw->pw_shell);
      अक्षर_दो ('\n');
    }

  अगर (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  अगर (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  अक्षर_दो ('\n');

  अगर (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

अटल व्योम
print_heading (व्योम)
{
  म_लिखो ("%-8s", _("Login"));
  अगर (include_fullname)
    म_लिखो (" %-19s", _("Name"));
  म_लिखो (" %-9s", _(" TTY"));
  अगर (include_idle)
    म_लिखो (" %-6s", _("Idle"));
  म_लिखो (" %-*s", time_format_width, _("When"));
#अगर_घोषित HAVE_STRUCT_XTMP_UT_HOST
  अगर (include_where)
    म_लिखो (" %s", _("Where"));
#पूर्ण_अगर
  अक्षर_दो ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

अटल व्योम
scan_entries (idx_t n, STRUCT_UTMP स्थिर *utmp_buf,
              स्थिर पूर्णांक argc_names, अक्षर *स्थिर argv_names[])
{
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

  अगर (include_heading)
    print_heading ();

  जबतक (n--)
    {
      अगर (IS_USER_PROCESS (utmp_buf))
        {
          अगर (argc_names)
            {
              क्रम (पूर्णांक i = 0; i < argc_names; i++)
                अगर (streq (utmp_buf->ut_user, argv_names[i]))
                  {
                    print_entry (utmp_buf);
                    अवरोध;
                  }
            }
          अन्यथा
            print_entry (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

अटल व्योम
short_pinky (अक्षर स्थिर *filename,
             स्थिर पूर्णांक argc_names, अक्षर *स्थिर argv_names[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  अगर (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (filename));

  scan_entries (n_users, utmp_buf, argc_names, argv_names);
  निकास (निकास_सफल);
}

अटल व्योम
long_pinky (स्थिर पूर्णांक argc_names, अक्षर *स्थिर argv_names[])
{
  क्रम (पूर्णांक i = 0; i < argc_names; i++)
    print_long_entry (argv_names[i]);
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      ख_माला_दो (_("\
\n\
"), मानक_निकास);
      oputs (_("\
  -l     produce दीर्घ format output क्रम the specified USERs\n\
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
  -s     करो लघु format output, this is the शेष\n\
"));
      oputs (_("\
  -f     omit the पंक्ति of column headings in लघु format\n\
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
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णांक optc;
  जबतक ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, शून्य))
         != -1)
    {
      चयन (optc)
        {
        हाल 's':
          do_short_format = true;
          अवरोध;

        हाल 'l':
          do_short_format = false;
          अवरोध;

        हाल 'f':
          include_heading = false;
          अवरोध;

        हाल 'w':
          include_fullname = false;
          अवरोध;

        हाल 'i':
          include_fullname = false;
#अगर_घोषित HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#पूर्ण_अगर
          अवरोध;

        हाल 'q':
          include_fullname = false;
#अगर_घोषित HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#पूर्ण_अगर
          include_idle = false;
          अवरोध;

        हाल 'h':
          include_project = false;
          अवरोध;

        हाल 'p':
          include_plan = false;
          अवरोध;

        हाल 'b':
          include_home_and_shell = false;
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

  पूर्णांक n_users = argc - optind;

  अगर (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at least one must be\
 specified when using -l"));
      usage (निकास_त्रुटि);
    }

  अगर (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  अन्यथा
    long_pinky (n_users, argv + optind);

  वापस निकास_सफल;
}
