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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "canon-host.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "full-write.h"
#ಅನ್ತರ್ಭಾವ "hard-locale.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pinky"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
ಸ್ಥಿರ bool include_idle = true;

/* If true, display a line at the top describing each field. */
ಸ್ಥಿರ bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
ಸ್ಥಿರ bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
ಸ್ಥಿರ bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
ಸ್ಥಿರ bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
ಸ್ಥಿರ bool include_home_and_shell = true;

/* if true, use the "short" output format. */
ಸ್ಥಿರ bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ಸ್ಥಿರ bool do_lookup;

/* if true, display the ut_host field. */
#ಯದಿ HAVE_STRUCT_XTMP_UT_HOST
ಸ್ಥಿರ bool include_where = true;
#endif

/* The strftime format to use for login times, and its expected
   output width.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *time_format;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ time_format_width;

/* for long options with no corresponding short option, use enum */
ಗಣನಾ
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"lookup", no_argument, ರಿಕ್ತ, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_PURE
ಸ್ಥಿರ idx_t
ಎಣಿಕೆ_ampersands (ವರ್ಣ ಧ್ರುವ *str)
{
  idx_t ಎಣಿಕೆ = 0;
  ಪ್ರತಿ (; *str; str++)
    ಎಣಿಕೆ += *str == '&';
  ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

ಸ್ಥಿರ ವರ್ಣ *
create_fullname (ವರ್ಣ ಧ್ರುವ *gecos_name, ವರ್ಣ ಧ್ರುವ *user_name)
{
  idx_t rsize = strlen (gecos_name) + 1;
  idx_t ampersands = ಎಣಿಕೆ_ampersands (gecos_name);

  ಯದಿ (ampersands != 0)
    {
      idx_t ulen = strlen (user_name);
      ptrdiff_t product;
      ಯದಿ (ckd_mul (&product, ulen - 1, ampersands)
          || ckd_add (&rsize, rsize, product))
        xalloc_die ();
    }

  ವರ್ಣ *result = xmalloc (rsize);
  ವರ್ಣ *r = result;

  ಯಾವತ್ (*gecos_name)
    {
      ಯದಿ (*gecos_name == '&')
        {
          ವರ್ಣ ಧ್ರುವ *uname = user_name;
          ಯದಿ (islower (to_uchar (*uname)))
            *r++ = toupper (to_uchar (*uname++));
          ಯಾವತ್ (*uname)
            *r++ = *uname++;
        }
      ಅನ್ಯಥಾ
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  ಪ್ರತ್ಯಾಗಮ result;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
idle_string (time_t when)
{
  ಸ್ಥಿರ time_t now = 0;
  ಸ್ಥಿರ ವರ್ಣ buf[INT_STRLEN_BOUND (intmax_t) + ಪರಿಮಾಣ "d"];

  ಯದಿ (now == 0)
    time (&now);

  time_t seconds_idle = now - when;
  ಯದಿ (seconds_idle < 60)	/* One minute. */
    ಪ್ರತ್ಯಾಗಮ "     ";
  ಯದಿ (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      ಪೂರ್ಣಾಙ್ಕ hours = seconds_idle / (60 * 60);
      ಪೂರ್ಣಾಙ್ಕ minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  ಅನ್ಯಥಾ
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  ಪ್ರತ್ಯಾಗಮ buf;
}

/* Return a time string.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
time_string (STRUCT_UTMP ಧ್ರುವ *utmp_ent)
{
  ಸ್ಥಿರ ವರ್ಣ buf[INT_STRLEN_BOUND (intmax_t) + ಪರಿಮಾಣ "-%m-%d %H:%M"];
  ಸಂರಚನಾ tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ಯದಿ (tmp)
    {
      strftime (buf, ಪರಿಮಾಣ buf, time_format, tmp);
      ಪ್ರತ್ಯಾಗಮ buf;
    }
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_entry (STRUCT_UTMP ಧ್ರುವ *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  ವರ್ಣ *ಸಾಲು = utmp_ent->ut_ಸಾಲು;
  ವರ್ಣ *space = strchr (ಸಾಲು, ' ');
  ಸಾಲು = space ? space + 1 : ಸಾಲು;

  ಪೂರ್ಣಾಙ್ಕ dirfd;
  ಯದಿ (IS_ABSOLUTE_FILE_NAME (ಸಾಲು))
    dirfd = AT_FDCWD;
  ಅನ್ಯಥಾ
    {
      ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ dev_dirfd;
      ಯದಿ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ಯದಿ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ಸಂರಚನಾ stat stats;
  time_t last_change;
  ವರ್ಣ mesg;
  ಯದಿ (AT_FDCWD <= dirfd && fstatat (dirfd, ಸಾಲು, &stats, 0) == 0)
    {
      mesg = (stats.st_mode & S_IWGRP) ? ' ' : '*';
      last_change = stats.st_atime;
    }
  ಅನ್ಯಥಾ
    {
      mesg = '?';
      last_change = 0;
    }

  ವರ್ಣ *ut_user = utmp_ent->ut_user;
  ಯದಿ (strnlen (ut_user, 8) < 8)
    printf ("%-8s", ut_user);
  ಅನ್ಯಥಾ
    fputs (ut_user, stdout);

  ಯದಿ (include_fullname)
    {
      ಸಂರಚನಾ passwd *pw = getpwnam (ut_user);
      ಯದಿ (pw == ರಿಕ್ತ)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        printf (" %19s", _("        ???"));
      ಅನ್ಯಥಾ
        {
          ವರ್ಣ *ಧ್ರುವ comma = strchr (pw->pw_gecos, ',');

          ಯದಿ (comma)
            *comma = '\0';

          ವರ್ಣ *result = create_fullname (pw->pw_gecos, pw->pw_name);
          printf (" %-19.19s", result);
          ಮುಕ್ತ (result);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  ಯದಿ (strnlen (utmp_ent->ut_ಸಾಲು, 8) < 8)
    printf ("%-8s", utmp_ent->ut_ಸಾಲು);
  ಅನ್ಯಥಾ
    fputs (utmp_ent->ut_ಸಾಲು, stdout);

  ಯದಿ (include_idle)
    {
      ಯದಿ (last_change)
        printf (" %-6s", idle_string (last_change));
      ಅನ್ಯಥಾ
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        printf (" %-6s", _("?????"));
    }

  printf (" %s", time_string (utmp_ent));

#ifdef HAVE_STRUCT_XTMP_UT_HOST
  ಯದಿ (include_where && utmp_ent->ut_host[0])
    {
      ವರ್ಣ *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      ವರ್ಣ *display = strchr (ut_host, ':');
      ಯದಿ (display)
        *display++ = '\0';

      ವರ್ಣ *host = ರಿಕ್ತ;
      ಯದಿ (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      ಯದಿ ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      ಯದಿ (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      ಯದಿ (host != ut_host)
        ಮುಕ್ತ (host);
    }
#endif

  putchar ('\n');

  ಯದಿ (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

ಸ್ಥಿರ ಶೂನ್ಯ
cat_file (ವರ್ಣ ಧ್ರುವ *header, ವರ್ಣ ಧ್ರುವ *home, ವರ್ಣ ಧ್ರುವ *file)
{
  ವರ್ಣ *full_name = file_name_concat (home, file, ರಿಕ್ತ);
  FILE *fp = fopen (full_name, "r");

  ಯದಿ (fp)
    {
      fputs (header, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      ವರ್ಣ buf[BUFSIZ];
      ಪ್ರತಿ (size_t bytes_read;
           0 < (bytes_read = fread (buf, 1, ಪರಿಮಾಣ buf, fp));)
        ಯದಿ (fwrite (buf, 1, bytes_read, stdout) != bytes_read)
          write_error ();

      fclose (fp);
    }

  ಮುಕ್ತ (full_name);
}

/* Display a verbose line of information about UTMP_ENT. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_long_entry (ಧ್ರುವ ವರ್ಣ name[])
{
  ಸಂರಚನಾ passwd *pw = getpwnam (name);

  printf (_("Login name: "));
  printf ("%-28s", name);

  printf (_("In real life: "));
  ಯದಿ (pw == ರಿಕ್ತ)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      printf (" %s", _("???\n"));
      ಪ್ರತ್ಯಾಗಮ;
    }
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ *ಧ್ರುವ comma = strchr (pw->pw_gecos, ',');

      ಯದಿ (comma)
        *comma = '\0';

      ವರ್ಣ *result = create_fullname (pw->pw_gecos, pw->pw_name);
      printf (" %s", result);
      ಮುಕ್ತ (result);
    }

  putchar ('\n');

  ಯದಿ (include_home_and_shell)
    {
      printf (_("Directory: "));
      printf ("%-29s", pw->pw_dir);
      printf (_("Shell: "));
      printf (" %s", pw->pw_shell);
      putchar ('\n');
    }

  ಯದಿ (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  ಯದಿ (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  putchar ('\n');

  ಯದಿ (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_heading (ಶೂನ್ಯ)
{
  printf ("%-8s", _("Login"));
  ಯದಿ (include_fullname)
    printf (" %-19s", _("Name"));
  printf (" %-9s", _(" TTY"));
  ಯದಿ (include_idle)
    printf (" %-6s", _("Idle"));
  printf (" %-*s", time_format_width, _("When"));
#ifdef HAVE_STRUCT_XTMP_UT_HOST
  ಯದಿ (include_where)
    printf (" %s", _("Where"));
#endif
  putchar ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

ಸ್ಥಿರ ಶೂನ್ಯ
scan_entries (idx_t n, STRUCT_UTMP ಧ್ರುವ *utmp_buf,
              ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ argc_names, ವರ್ಣ *ಧ್ರುವ argv_names[])
{
  ಯದಿ (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ಅನ್ಯಥಾ
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ಯದಿ (include_heading)
    print_heading ();

  ಯಾವತ್ (n--)
    {
      ಯದಿ (IS_USER_PROCESS (utmp_buf))
        {
          ಯದಿ (argc_names)
            {
              ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < argc_names; i++)
                ಯದಿ (streq (utmp_buf->ut_user, argv_names[i]))
                  {
                    print_entry (utmp_buf);
                    ಭಙ್ಗ;
                  }
            }
          ಅನ್ಯಥಾ
            print_entry (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

ಸ್ಥಿರ ಶೂನ್ಯ
short_pinky (ವರ್ಣ ಧ್ರುವ *filename,
             ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ argc_names, ವರ್ಣ *ಧ್ರುವ argv_names[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ಯದಿ (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  scan_entries (n_users, utmp_buf, argc_names, argv_names);
  exit (EXIT_SUCCESS);
}

ಸ್ಥಿರ ಶೂನ್ಯ
long_pinky (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ argc_names, ವರ್ಣ *ಧ್ರುವ argv_names[])
{
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < argc_names; i++)
    print_long_entry (argv_names[i]);
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce long format output ಪ್ರತಿ the specified USERs\n\
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
  -s     ಕುರು short format output, this is the ಸ್ವಭಾವ\n\
"));
      oputs (_("\
  -f     omit the ಸಾಲು of column headings in short format\n\
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

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಙ್ಕ optc;
  ಯಾವತ್ ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 's':
          do_short_format = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'l':
          do_short_format = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'f':
          include_heading = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'w':
          include_fullname = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'i':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          ಭಙ್ಗ;

        ಪಕ್ಷ 'q':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          include_idle = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'h':
          include_project = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'p':
          include_plan = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'b':
          include_home_and_shell = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ LOOKUP_OPTION:
          do_lookup = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಪೂರ್ಣಾಙ್ಕ n_users = argc - optind;

  ಯದಿ (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at least one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  ಅನ್ಯಥಾ
    long_pinky (n_users, argv + optind);

  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}
