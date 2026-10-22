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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <pwd.h>
#അന്തര്ഭാവ <stdio.h>

#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "canon-host.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "filenamecat.h"
#അന്തര്ഭാവ "full-write.h"
#അന്തര്ഭാവ "hard-locale.h"
#അന്തര്ഭാവ "ioblksize.h"
#അന്തര്ഭാവ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pinky"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
സ്ഥിതം bool include_idle = true;

/* If true, display a line at the top describing each field. */
സ്ഥിതം bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
സ്ഥിതം bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
സ്ഥിതം bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
സ്ഥിതം bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
സ്ഥിതം bool include_home_and_shell = true;

/* if true, use the "short" output format. */
സ്ഥിതം bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
സ്ഥിതം bool do_lookup;

/* if true, display the ut_host field. */
#എങ്കിൽ HAVE_STRUCT_XTMP_UT_HOST
സ്ഥിതം bool include_where = true;
#endif

/* The strftime format to use for login times, and its expected
   output width.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *time_format;
സ്ഥിതം പൂർണ്ണസംഖ്യ time_format_width;

/* for long options with no corresponding short option, use enum */
എണ്ണൽ
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"lookup", no_argument, ശൂന്യ, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_PURE
സ്ഥിതം idx_t
എണ്ണം_ampersands (അക്ഷരം സ്ഥിരം *str)
{
  idx_t എണ്ണം = 0;
  വേണ്ടി (; *str; str++)
    എണ്ണം += *str == '&';
  തിരികെ എണ്ണം;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

സ്ഥിതം അക്ഷരം *
create_fullname (അക്ഷരം സ്ഥിരം *gecos_name, അക്ഷരം സ്ഥിരം *user_name)
{
  idx_t rsize = strlen (gecos_name) + 1;
  idx_t ampersands = എണ്ണം_ampersands (gecos_name);

  എങ്കിൽ (ampersands != 0)
    {
      idx_t ulen = strlen (user_name);
      ptrdiff_t product;
      എങ്കിൽ (ckd_mul (&product, ulen - 1, ampersands)
          || ckd_add (&rsize, rsize, product))
        xalloc_die ();
    }

  അക്ഷരം *result = xmalloc (rsize);
  അക്ഷരം *r = result;

  ആകുമ്പോൾ (*gecos_name)
    {
      എങ്കിൽ (*gecos_name == '&')
        {
          അക്ഷരം സ്ഥിരം *uname = user_name;
          എങ്കിൽ (islower (to_uchar (*uname)))
            *r++ = toupper (to_uchar (*uname++));
          ആകുമ്പോൾ (*uname)
            *r++ = *uname++;
        }
      അല്ലെങ്കിൽ
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  തിരികെ result;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

സ്ഥിതം അക്ഷരം സ്ഥിരം *
idle_string (time_t when)
{
  സ്ഥിതം time_t now = 0;
  സ്ഥിതം അക്ഷരം buf[INT_STRLEN_BOUND (intmax_t) + വലുപ്പം "d"];

  എങ്കിൽ (now == 0)
    time (&now);

  time_t seconds_idle = now - when;
  എങ്കിൽ (seconds_idle < 60)	/* One minute. */
    തിരികെ "     ";
  എങ്കിൽ (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      പൂർണ്ണസംഖ്യ hours = seconds_idle / (60 * 60);
      പൂർണ്ണസംഖ്യ minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  അല്ലെങ്കിൽ
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  തിരികെ buf;
}

/* Return a time string.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *
time_string (STRUCT_UTMP സ്ഥിരം *utmp_ent)
{
  സ്ഥിതം അക്ഷരം buf[INT_STRLEN_BOUND (intmax_t) + വലുപ്പം "-%m-%d %H:%M"];
  ഘടന tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  എങ്കിൽ (tmp)
    {
      strftime (buf, വലുപ്പം buf, time_format, tmp);
      തിരികെ buf;
    }
  അല്ലെങ്കിൽ
    തിരികെ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

സ്ഥിതം ശൂന്യം
print_entry (STRUCT_UTMP സ്ഥിരം *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  അക്ഷരം *വരി = utmp_ent->ut_വരി;
  അക്ഷരം *space = strchr (വരി, ' ');
  വരി = space ? space + 1 : വരി;

  പൂർണ്ണസംഖ്യ dirfd;
  എങ്കിൽ (IS_ABSOLUTE_FILE_NAME (വരി))
    dirfd = AT_FDCWD;
  അല്ലെങ്കിൽ
    {
      സ്ഥിതം പൂർണ്ണസംഖ്യ dev_dirfd;
      എങ്കിൽ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          എങ്കിൽ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ഘടന stat stats;
  time_t last_change;
  അക്ഷരം mesg;
  എങ്കിൽ (AT_FDCWD <= dirfd && fstatat (dirfd, വരി, &stats, 0) == 0)
    {
      mesg = (stats.st_mode & S_IWGRP) ? ' ' : '*';
      last_change = stats.st_atime;
    }
  അല്ലെങ്കിൽ
    {
      mesg = '?';
      last_change = 0;
    }

  അക്ഷരം *ut_user = utmp_ent->ut_user;
  എങ്കിൽ (strnlen (ut_user, 8) < 8)
    printf ("%-8s", ut_user);
  അല്ലെങ്കിൽ
    fputs (ut_user, stdout);

  എങ്കിൽ (include_fullname)
    {
      ഘടന passwd *pw = getpwnam (ut_user);
      എങ്കിൽ (pw == ശൂന്യ)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        printf (" %19s", _("        ???"));
      അല്ലെങ്കിൽ
        {
          അക്ഷരം *സ്ഥിരം comma = strchr (pw->pw_gecos, ',');

          എങ്കിൽ (comma)
            *comma = '\0';

          അക്ഷരം *result = create_fullname (pw->pw_gecos, pw->pw_name);
          printf (" %-19.19s", result);
          സ്വതന്ത്രം (result);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  എങ്കിൽ (strnlen (utmp_ent->ut_വരി, 8) < 8)
    printf ("%-8s", utmp_ent->ut_വരി);
  അല്ലെങ്കിൽ
    fputs (utmp_ent->ut_വരി, stdout);

  എങ്കിൽ (include_idle)
    {
      എങ്കിൽ (last_change)
        printf (" %-6s", idle_string (last_change));
      അല്ലെങ്കിൽ
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        printf (" %-6s", _("?????"));
    }

  printf (" %s", time_string (utmp_ent));

#ifdef HAVE_STRUCT_XTMP_UT_HOST
  എങ്കിൽ (include_where && utmp_ent->ut_host[0])
    {
      അക്ഷരം *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      അക്ഷരം *display = strchr (ut_host, ':');
      എങ്കിൽ (display)
        *display++ = '\0';

      അക്ഷരം *host = ശൂന്യ;
      എങ്കിൽ (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      എങ്കിൽ ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      എങ്കിൽ (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      എങ്കിൽ (host != ut_host)
        സ്വതന്ത്രം (host);
    }
#endif

  putchar ('\n');

  എങ്കിൽ (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

സ്ഥിതം ശൂന്യം
cat_file (അക്ഷരം സ്ഥിരം *header, അക്ഷരം സ്ഥിരം *home, അക്ഷരം സ്ഥിരം *file)
{
  അക്ഷരം *full_name = file_name_concat (home, file, ശൂന്യ);
  FILE *fp = fopen (full_name, "r");

  എങ്കിൽ (fp)
    {
      fputs (header, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      അക്ഷരം buf[BUFSIZ];
      വേണ്ടി (size_t bytes_read;
           0 < (bytes_read = fread (buf, 1, വലുപ്പം buf, fp));)
        എങ്കിൽ (fwrite (buf, 1, bytes_read, stdout) != bytes_read)
          write_error ();

      fclose (fp);
    }

  സ്വതന്ത്രം (full_name);
}

/* Display a verbose line of information about UTMP_ENT. */

സ്ഥിതം ശൂന്യം
print_long_entry (സ്ഥിരം അക്ഷരം name[])
{
  ഘടന passwd *pw = getpwnam (name);

  printf (_("Login name: "));
  printf ("%-28s", name);

  printf (_("In real life: "));
  എങ്കിൽ (pw == ശൂന്യ)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      printf (" %s", _("???\n"));
      തിരികെ;
    }
  അല്ലെങ്കിൽ
    {
      അക്ഷരം *സ്ഥിരം comma = strchr (pw->pw_gecos, ',');

      എങ്കിൽ (comma)
        *comma = '\0';

      അക്ഷരം *result = create_fullname (pw->pw_gecos, pw->pw_name);
      printf (" %s", result);
      സ്വതന്ത്രം (result);
    }

  putchar ('\n');

  എങ്കിൽ (include_home_and_shell)
    {
      printf (_("Directory: "));
      printf ("%-29s", pw->pw_dir);
      printf (_("Shell: "));
      printf (" %s", pw->pw_shell);
      putchar ('\n');
    }

  എങ്കിൽ (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  എങ്കിൽ (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  putchar ('\n');

  എങ്കിൽ (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

സ്ഥിതം ശൂന്യം
print_heading (ശൂന്യം)
{
  printf ("%-8s", _("Login"));
  എങ്കിൽ (include_fullname)
    printf (" %-19s", _("Name"));
  printf (" %-9s", _(" TTY"));
  എങ്കിൽ (include_idle)
    printf (" %-6s", _("Idle"));
  printf (" %-*s", time_format_width, _("When"));
#ifdef HAVE_STRUCT_XTMP_UT_HOST
  എങ്കിൽ (include_where)
    printf (" %s", _("Where"));
#endif
  putchar ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

സ്ഥിതം ശൂന്യം
scan_entries (idx_t n, STRUCT_UTMP സ്ഥിരം *utmp_buf,
              സ്ഥിരം പൂർണ്ണസംഖ്യ argc_names, അക്ഷരം *സ്ഥിരം argv_names[])
{
  എങ്കിൽ (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  അല്ലെങ്കിൽ
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  എങ്കിൽ (include_heading)
    print_heading ();

  ആകുമ്പോൾ (n--)
    {
      എങ്കിൽ (IS_USER_PROCESS (utmp_buf))
        {
          എങ്കിൽ (argc_names)
            {
              വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < argc_names; i++)
                എങ്കിൽ (streq (utmp_buf->ut_user, argv_names[i]))
                  {
                    print_entry (utmp_buf);
                    നിർത്തുക;
                  }
            }
          അല്ലെങ്കിൽ
            print_entry (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

സ്ഥിതം ശൂന്യം
short_pinky (അക്ഷരം സ്ഥിരം *filename,
             സ്ഥിരം പൂർണ്ണസംഖ്യ argc_names, അക്ഷരം *സ്ഥിരം argv_names[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  എങ്കിൽ (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  scan_entries (n_users, utmp_buf, argc_names, argv_names);
  exit (EXIT_SUCCESS);
}

സ്ഥിതം ശൂന്യം
long_pinky (സ്ഥിരം പൂർണ്ണസംഖ്യ argc_names, അക്ഷരം *സ്ഥിരം argv_names[])
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < argc_names; i++)
    print_long_entry (argv_names[i]);
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce long format output വേണ്ടി the specified USERs\n\
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
  -s     ചെയ്യുക short format output, this is the സ്ഥിരസ്ഥിതി\n\
"));
      oputs (_("\
  -f     omit the വരി of column headings in short format\n\
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  പൂർണ്ണസംഖ്യ optc;
  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 's':
          do_short_format = true;
          നിർത്തുക;

        അവസ്ഥ 'l':
          do_short_format = false;
          നിർത്തുക;

        അവസ്ഥ 'f':
          include_heading = false;
          നിർത്തുക;

        അവസ്ഥ 'w':
          include_fullname = false;
          നിർത്തുക;

        അവസ്ഥ 'i':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          നിർത്തുക;

        അവസ്ഥ 'q':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          include_idle = false;
          നിർത്തുക;

        അവസ്ഥ 'h':
          include_project = false;
          നിർത്തുക;

        അവസ്ഥ 'p':
          include_plan = false;
          നിർത്തുക;

        അവസ്ഥ 'b':
          include_home_and_shell = false;
          നിർത്തുക;

        അവസ്ഥ LOOKUP_OPTION:
          do_lookup = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  പൂർണ്ണസംഖ്യ n_users = argc - optind;

  എങ്കിൽ (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at least one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  അല്ലെങ്കിൽ
    long_pinky (n_users, argv + optind);

  തിരികെ EXIT_SUCCESS;
}
