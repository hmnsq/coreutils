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
#यदि HAVE_STRUCT_XTMP_UT_HOST
स्थिर bool include_where = true;
#endif

/* The strftime format to use for login times, and its expected
   output width.  */
स्थिर वर्ण ध्रुव *time_format;
स्थिर पूर्णाङ्क time_format_width;

/* for long options with no corresponding short option, use enum */
गणना
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"lookup", no_argument, रिक्त, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_PURE
स्थिर idx_t
गिनती_ampersands (वर्ण ध्रुव *str)
{
  idx_t गिनती = 0;
  प्रति (; *str; str++)
    गिनती += *str == '&';
  प्रत्यागम गिनती;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

स्थिर वर्ण *
create_fullname (वर्ण ध्रुव *gecos_name, वर्ण ध्रुव *user_name)
{
  idx_t rsize = strlen (gecos_name) + 1;
  idx_t ampersands = गिनती_ampersands (gecos_name);

  यदि (ampersands != 0)
    {
      idx_t ulen = strlen (user_name);
      ptrdiff_t product;
      यदि (ckd_mul (&product, ulen - 1, ampersands)
          || ckd_add (&rsize, rsize, product))
        xalloc_die ();
    }

  वर्ण *result = xmalloc (rsize);
  वर्ण *r = result;

  यावत् (*gecos_name)
    {
      यदि (*gecos_name == '&')
        {
          वर्ण ध्रुव *uname = user_name;
          यदि (islower (to_uchar (*uname)))
            *r++ = toupper (to_uchar (*uname++));
          यावत् (*uname)
            *r++ = *uname++;
        }
      अन्यथा
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  प्रत्यागम result;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

स्थिर वर्ण ध्रुव *
idle_string (time_t when)
{
  स्थिर time_t now = 0;
  स्थिर वर्ण buf[INT_STRLEN_BOUND (intmax_t) + परिमाण "d"];

  यदि (now == 0)
    time (&now);

  time_t seconds_idle = now - when;
  यदि (seconds_idle < 60)	/* One minute. */
    प्रत्यागम "     ";
  यदि (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      पूर्णाङ्क hours = seconds_idle / (60 * 60);
      पूर्णाङ्क minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  अन्यथा
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  प्रत्यागम buf;
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

/* Display a line of information about UTMP_ENT. */

स्थिर शून्य
print_entry (STRUCT_UTMP ध्रुव *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  वर्ण *पंक्ति = utmp_ent->ut_पंक्ति;
  वर्ण *space = strchr (पंक्ति, ' ');
  पंक्ति = space ? space + 1 : पंक्ति;

  पूर्णाङ्क dirfd;
  यदि (IS_ABSOLUTE_FILE_NAME (पंक्ति))
    dirfd = AT_FDCWD;
  अन्यथा
    {
      स्थिर पूर्णाङ्क dev_dirfd;
      यदि (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          यदि (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  संरचना stat stats;
  time_t last_change;
  वर्ण mesg;
  यदि (AT_FDCWD <= dirfd && fstatat (dirfd, पंक्ति, &stats, 0) == 0)
    {
      mesg = (stats.st_mode & S_IWGRP) ? ' ' : '*';
      last_change = stats.st_atime;
    }
  अन्यथा
    {
      mesg = '?';
      last_change = 0;
    }

  वर्ण *ut_user = utmp_ent->ut_user;
  यदि (strnlen (ut_user, 8) < 8)
    printf ("%-8s", ut_user);
  अन्यथा
    fputs (ut_user, stdout);

  यदि (include_fullname)
    {
      संरचना passwd *pw = getpwnam (ut_user);
      यदि (pw == रिक्त)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        printf (" %19s", _("        ???"));
      अन्यथा
        {
          वर्ण *ध्रुव comma = strchr (pw->pw_gecos, ',');

          यदि (comma)
            *comma = '\0';

          वर्ण *result = create_fullname (pw->pw_gecos, pw->pw_name);
          printf (" %-19.19s", result);
          मुक्त (result);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  यदि (strnlen (utmp_ent->ut_पंक्ति, 8) < 8)
    printf ("%-8s", utmp_ent->ut_पंक्ति);
  अन्यथा
    fputs (utmp_ent->ut_पंक्ति, stdout);

  यदि (include_idle)
    {
      यदि (last_change)
        printf (" %-6s", idle_string (last_change));
      अन्यथा
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        printf (" %-6s", _("?????"));
    }

  printf (" %s", time_string (utmp_ent));

#ifdef HAVE_STRUCT_XTMP_UT_HOST
  यदि (include_where && utmp_ent->ut_host[0])
    {
      वर्ण *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      वर्ण *display = strchr (ut_host, ':');
      यदि (display)
        *display++ = '\0';

      वर्ण *host = रिक्त;
      यदि (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      यदि ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      यदि (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      यदि (host != ut_host)
        मुक्त (host);
    }
#endif

  putchar ('\n');

  यदि (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

स्थिर शून्य
cat_file (वर्ण ध्रुव *header, वर्ण ध्रुव *home, वर्ण ध्रुव *file)
{
  वर्ण *full_name = file_name_concat (home, file, रिक्त);
  FILE *fp = fopen (full_name, "r");

  यदि (fp)
    {
      fputs (header, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      वर्ण buf[BUFSIZ];
      प्रति (size_t bytes_read;
           0 < (bytes_read = fread (buf, 1, परिमाण buf, fp));)
        यदि (fwrite (buf, 1, bytes_read, stdout) != bytes_read)
          write_error ();

      fclose (fp);
    }

  मुक्त (full_name);
}

/* Display a verbose line of information about UTMP_ENT. */

स्थिर शून्य
print_long_entry (ध्रुव वर्ण name[])
{
  संरचना passwd *pw = getpwnam (name);

  printf (_("Login name: "));
  printf ("%-28s", name);

  printf (_("In real life: "));
  यदि (pw == रिक्त)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      printf (" %s", _("???\n"));
      प्रत्यागम;
    }
  अन्यथा
    {
      वर्ण *ध्रुव comma = strchr (pw->pw_gecos, ',');

      यदि (comma)
        *comma = '\0';

      वर्ण *result = create_fullname (pw->pw_gecos, pw->pw_name);
      printf (" %s", result);
      मुक्त (result);
    }

  putchar ('\n');

  यदि (include_home_and_shell)
    {
      printf (_("Directory: "));
      printf ("%-29s", pw->pw_dir);
      printf (_("Shell: "));
      printf (" %s", pw->pw_shell);
      putchar ('\n');
    }

  यदि (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  यदि (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  putchar ('\n');

  यदि (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

स्थिर शून्य
print_heading (शून्य)
{
  printf ("%-8s", _("Login"));
  यदि (include_fullname)
    printf (" %-19s", _("Name"));
  printf (" %-9s", _(" TTY"));
  यदि (include_idle)
    printf (" %-6s", _("Idle"));
  printf (" %-*s", time_format_width, _("When"));
#ifdef HAVE_STRUCT_XTMP_UT_HOST
  यदि (include_where)
    printf (" %s", _("Where"));
#endif
  putchar ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

स्थिर शून्य
scan_entries (idx_t n, STRUCT_UTMP ध्रुव *utmp_buf,
              ध्रुव पूर्णाङ्क argc_names, वर्ण *ध्रुव argv_names[])
{
  यदि (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  अन्यथा
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  यदि (include_heading)
    print_heading ();

  यावत् (n--)
    {
      यदि (IS_USER_PROCESS (utmp_buf))
        {
          यदि (argc_names)
            {
              प्रति (पूर्णाङ्क i = 0; i < argc_names; i++)
                यदि (streq (utmp_buf->ut_user, argv_names[i]))
                  {
                    print_entry (utmp_buf);
                    भङ्ग;
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
short_pinky (वर्ण ध्रुव *filename,
             ध्रुव पूर्णाङ्क argc_names, वर्ण *ध्रुव argv_names[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  यदि (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  scan_entries (n_users, utmp_buf, argc_names, argv_names);
  exit (EXIT_SUCCESS);
}

स्थिर शून्य
long_pinky (ध्रुव पूर्णाङ्क argc_names, वर्ण *ध्रुव argv_names[])
{
  प्रति (पूर्णाङ्क i = 0; i < argc_names; i++)
    print_long_entry (argv_names[i]);
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce long format output प्रति the specified USERs\n\
"));
      oputs (_("\
  -b     omit the user's home directory and shell in long format\n\
"));
      oputs (_("\
  -h     omit the user's project file in long format\n\
"));
      oputs (_("\
  -p     omit the user's plan file in long format\n\
"));
      oputs (_("\
  -s     कुरु short format output, this is the स्वभाव\n\
"));
      oputs (_("\
  -f     omit the पंक्ति of column headings in short format\n\
"));
      oputs (_("\
  -w     omit the user's full name in short format\n\
"));
      oputs (_("\
  -i     omit the user's full name and remote host in short format\n\
"));
      oputs (_("\
  -q     omit the user's full name, remote host and idle time in short format\n\
"));
      oputs (_("\
      --lookup\n\
         attempt to canonicalize hostnames via DNS\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
A lightweight 'finger' program;  print user information.\n\
The utmp file will be %s.\n\
"), UTMP_FILE);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  पूर्णाङ्क optc;
  यावत् ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, रिक्त))
         != -1)
    {
      विकल्प (optc)
        {
        पक्ष 's':
          do_short_format = true;
          भङ्ग;

        पक्ष 'l':
          do_short_format = false;
          भङ्ग;

        पक्ष 'f':
          include_heading = false;
          भङ्ग;

        पक्ष 'w':
          include_fullname = false;
          भङ्ग;

        पक्ष 'i':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          भङ्ग;

        पक्ष 'q':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          include_idle = false;
          भङ्ग;

        पक्ष 'h':
          include_project = false;
          भङ्ग;

        पक्ष 'p':
          include_plan = false;
          भङ्ग;

        पक्ष 'b':
          include_home_and_shell = false;
          भङ्ग;

        पक्ष LOOKUP_OPTION:
          do_lookup = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  पूर्णाङ्क n_users = argc - optind;

  यदि (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at least one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  यदि (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  अन्यथा
    long_pinky (n_users, argv + optind);

  प्रत्यागम EXIT_SUCCESS;
}
