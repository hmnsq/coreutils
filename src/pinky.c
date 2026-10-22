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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "pinky"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
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
#ಆದರೆ HAVE_STRUCT_XTMP_UT_HOST
ಸ್ಥಿರ bool include_where = true;
#ಆದರೆ_ಅಂತ್ಯ

/* The strftime format to use for login times, and its expected
   output width.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಮಯ_format;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಮಯ_format_ಅಗಲ;

/* for long options with no corresponding short option, use enum */
ಎಣಿಕೆ
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"lookup", no_ವಾದ, ಖಾಲಿ, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ idx_t
ಎಣಿಕೆ_ampersands (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  idx_t ಎಣಿಕೆ_2 = 0;
  ಗಾಗಿ (; *str; str++)
    ಎಣಿಕೆ_2 += *str == '&';
  ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
create_fullname (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *gecos_name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *user_name)
{
  idx_t rsize = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (gecos_name) + 1;
  idx_t ampersands = ಎಣಿಕೆ_ampersands (gecos_name);

  ಆದರೆ (ampersands != 0)
    {
      idx_t ulen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (user_name);
      ptrdiff_t ಗುಣಲಬ್ಧ;
      ಆದರೆ (ckd_mul (&ಗುಣಲಬ್ಧ, ulen - 1, ampersands)
          || ckd_ಕೂಡಿಸು (&rsize, rsize, ಗುಣಲಬ್ಧ))
        xalloc_die ();
    }

  ಅಕ್ಷರ *ಫಲಿತಾಂಶ = xmalloc (rsize);
  ಅಕ್ಷರ *r = ಫಲಿತಾಂಶ;

  ಆಗ (*gecos_name)
    {
      ಆದರೆ (*gecos_name == '&')
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *uname = user_name;
          ಆದರೆ (islower (ಗಮ್ಯ_uchar (*uname)))
            *r++ = ದೊಡ್ಡ_ಅಕ್ಷರ (ಗಮ್ಯ_uchar (*uname++));
          ಆಗ (*uname)
            *r++ = *uname++;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
idle_string (time_t when)
{
  ಸ್ಥಿರ time_t now = 0;
  ಸ್ಥಿರ ಅಕ್ಷರ buf[INT_STRLEN_BOUND (intmax_t) + ಗಾತ್ರ "d"];

  ಆದರೆ (now == 0)
    ಸಮಯ (&now);

  time_t seconds_idle = now - when;
  ಆದರೆ (seconds_idle < 60)	/* One minute. */
    ಹಿಂತಿರುಗಿ "     ";
  ಆದರೆ (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      ಪೂರ್ಣಾಂಕ hours = seconds_idle / (60 * 60);
      ಪೂರ್ಣಾಂಕ minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  ಹಿಂತಿರುಗಿ buf;
}

/* Return a time string.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
ಸಮಯ_string (STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_ent)
{
  ಸ್ಥಿರ ಅಕ್ಷರ buf[INT_STRLEN_BOUND (intmax_t) + ಗಾತ್ರ "-%m-%d %H:%M"];
  ರಚನೆ tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ಆದರೆ (tmp)
    {
      strftime (buf, ಗಾತ್ರ buf, ಸಮಯ_format, tmp);
      ಹಿಂತಿರುಗಿ buf;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_ನಮೂದು (STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  ಅಕ್ಷರ *ಸಾಲು = utmp_ent->ut_ಸಾಲು;
  ಅಕ್ಷರ *space = strchr (ಸಾಲು, ' ');
  ಸಾಲು = space ? space + 1 : ಸಾಲು;

  ಪೂರ್ಣಾಂಕ dirfd;
  ಆದರೆ (IS_ABSOLUTE_FILE_NAME (ಸಾಲು))
    dirfd = AT_FDCWD;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dev_dirfd;
      ಆದರೆ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ಆದರೆ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ರಚನೆ stat stats;
  time_t ಕೊನೆಯ_change;
  ಅಕ್ಷರ mesg;
  ಆದರೆ (AT_FDCWD <= dirfd && fstatat (dirfd, ಸಾಲು, &stats, 0) == 0)
    {
      mesg = (stats.st_ಬಹುಲಕ & S_IWGRP) ? ' ' : '*';
      ಕೊನೆಯ_change = stats.st_atime;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      mesg = '?';
      ಕೊನೆಯ_change = 0;
    }

  ಅಕ್ಷರ *ut_user = utmp_ent->ut_user;
  ಆದರೆ (strnlen (ut_user, 8) < 8)
    ಬರೆ ("%-8s", ut_user);
  ಇಲ್ಲದಿದ್ದರೆ
    fputs (ut_user, stdout);

  ಆದರೆ (include_fullname)
    {
      ರಚನೆ passwd *pw = getpwnam (ut_user);
      ಆದರೆ (pw == ಖಾಲಿ)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        ಬರೆ (" %19s", _("        ???"));
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ comma = strchr (pw->pw_gecos, ',');

          ಆದರೆ (comma)
            *comma = '\0';

          ಅಕ್ಷರ *ಫಲಿತಾಂಶ = create_fullname (pw->pw_gecos, pw->pw_name);
          ಬರೆ (" %-19.19s", ಫಲಿತಾಂಶ);
          ಬಿಡುಗಡೆ (ಫಲಿತಾಂಶ);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  ಆದರೆ (strnlen (utmp_ent->ut_ಸಾಲು, 8) < 8)
    ಬರೆ ("%-8s", utmp_ent->ut_ಸಾಲು);
  ಇಲ್ಲದಿದ್ದರೆ
    fputs (utmp_ent->ut_ಸಾಲು, stdout);

  ಆದರೆ (include_idle)
    {
      ಆದರೆ (ಕೊನೆಯ_change)
        ಬರೆ (" %-6s", idle_string (ಕೊನೆಯ_change));
      ಇಲ್ಲದಿದ್ದರೆ
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        ಬರೆ (" %-6s", _("?????"));
    }

  ಬರೆ (" %s", ಸಮಯ_string (utmp_ent));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_STRUCT_XTMP_UT_HOST
  ಆದರೆ (include_where && utmp_ent->ut_host[0])
    {
      ಅಕ್ಷರ *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      ಅಕ್ಷರ *display = strchr (ut_host, ':');
      ಆದರೆ (display)
        *display++ = '\0';

      ಅಕ್ಷರ *host = ಖಾಲಿ;
      ಆದರೆ (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      ಆದರೆ ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      ಆದರೆ (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      ಆದರೆ (host != ut_host)
        ಬಿಡುಗಡೆ (host);
    }
#ಆದರೆ_ಅಂತ್ಯ

  ಅಕ್ಷರ_ಬರೆ ('\n');

  ಆದರೆ (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

ಸ್ಥಿರ ಶೂನ್ಯ
cat_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಶೀರ್ಷಿಕೆ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *home, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  ಅಕ್ಷರ *ತುಂಬಿದ_name = file_name_concat (home, file, ಖಾಲಿ);
  FILE *fp = fopen (ತುಂಬಿದ_name, "r");

  ಆದರೆ (fp)
    {
      fputs (ಶೀರ್ಷಿಕೆ, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      ಅಕ್ಷರ buf[BUFSIZ];
      ಗಾಗಿ (size_t ಬೈಟುಗಳು_read;
           0 < (ಬೈಟುಗಳು_read = fread (buf, 1, ಗಾತ್ರ buf, fp));)
        ಆದರೆ (fwrite (buf, 1, ಬೈಟುಗಳು_read, stdout) != ಬೈಟುಗಳು_read)
          write_error ();

      fclose (fp);
    }

  ಬಿಡುಗಡೆ (ತುಂಬಿದ_name);
}

/* Display a verbose line of information about UTMP_ENT. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_long_ನಮೂದು (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ name[])
{
  ರಚನೆ passwd *pw = getpwnam (name);

  ಬರೆ (_("Login name: "));
  ಬರೆ ("%-28s", name);

  ಬರೆ (_("In real life: "));
  ಆದರೆ (pw == ಖಾಲಿ)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      ಬರೆ (" %s", _("???\n"));
      ಹಿಂತಿರುಗಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ comma = strchr (pw->pw_gecos, ',');

      ಆದರೆ (comma)
        *comma = '\0';

      ಅಕ್ಷರ *ಫಲಿತಾಂಶ = create_fullname (pw->pw_gecos, pw->pw_name);
      ಬರೆ (" %s", ಫಲಿತಾಂಶ);
      ಬಿಡುಗಡೆ (ಫಲಿತಾಂಶ);
    }

  ಅಕ್ಷರ_ಬರೆ ('\n');

  ಆದರೆ (include_home_and_shell)
    {
      ಬರೆ (_("Directory: "));
      ಬರೆ ("%-29s", pw->pw_dir);
      ಬರೆ (_("Shell: "));
      ಬರೆ (" %s", pw->pw_shell);
      ಅಕ್ಷರ_ಬರೆ ('\n');
    }

  ಆದರೆ (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  ಆದರೆ (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  ಅಕ್ಷರ_ಬರೆ ('\n');

  ಆದರೆ (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_heading (ಶೂನ್ಯ)
{
  ಬರೆ ("%-8s", _("Login"));
  ಆದರೆ (include_fullname)
    ಬರೆ (" %-19s", _("Name"));
  ಬರೆ (" %-9s", _(" TTY"));
  ಆದರೆ (include_idle)
    ಬರೆ (" %-6s", _("Idle"));
  ಬರೆ (" %-*s", ಸಮಯ_format_ಅಗಲ, _("When"));
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_STRUCT_XTMP_UT_HOST
  ಆದರೆ (include_where)
    ಬರೆ (" %s", _("Where"));
#ಆದರೆ_ಅಂತ್ಯ
  ಅಕ್ಷರ_ಬರೆ ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

ಸ್ಥಿರ ಶೂನ್ಯ
ಪರಿಶೀಲನೆ_entries (idx_t n, STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_buf,
              ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ argc_ಹೆಸರುಗಳು, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ argv_ಹೆಸರುಗಳು[])
{
  ಆದರೆ (hard_locale (LC_ಸಮಯ))
    {
      ಸಮಯ_format = "%Y-%m-%d %H:%M";
      ಸಮಯ_format_ಅಗಲ = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸಮಯ_format = "%b %e %H:%M";
      ಸಮಯ_format_ಅಗಲ = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ಆದರೆ (include_heading)
    print_heading ();

  ಆಗ (n--)
    {
      ಆದರೆ (IS_USER_PROCESS (utmp_buf))
        {
          ಆದರೆ (argc_ಹೆಸರುಗಳು)
            {
              ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < argc_ಹೆಸರುಗಳು; i++)
                ಆದರೆ (streq (utmp_buf->ut_user, argv_ಹೆಸರುಗಳು[i]))
                  {
                    print_ನಮೂದು (utmp_buf);
                    ನಿಲ್ಲಿಸು;
                  }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            print_ನಮೂದು (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

ಸ್ಥಿರ ಶೂನ್ಯ
short_pinky (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename,
             ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ argc_ಹೆಸರುಗಳು, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ argv_ಹೆಸರುಗಳು[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ಆದರೆ (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  ಪರಿಶೀಲನೆ_entries (n_users, utmp_buf, argc_ಹೆಸರುಗಳು, argv_ಹೆಸರುಗಳು);
  ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
}

ಸ್ಥಿರ ಶೂನ್ಯ
long_pinky (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ argc_ಹೆಸರುಗಳು, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ argv_ಹೆಸರುಗಳು[])
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < argc_ಹೆಸರುಗಳು; i++)
    print_long_ನಮೂದು (argv_ಹೆಸರುಗಳು[i]);
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... [USER]...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce ದೀರ್ಘ format output ಗಾಗಿ the specified USERs\n\
"));
      oputs (_("\
  -b     omit the user's home directory and shell ಒಳ ದೀರ್ಘ format\n\
"));
      oputs (_("\
  -h     omit the user's project file ಒಳ ದೀರ್ಘ format\n\
"));
      oputs (_("\
  -p     omit the user's plan file ಒಳ ದೀರ್ಘ format\n\
"));
      oputs (_("\
  -s     ಮಾಡು ಗಿಡ್ಡ format output, this is the ಪೂರ್ವನಿಯೋಜಿತ\n\
"));
      oputs (_("\
  -f     omit the ಸಾಲು ದ ಸ್ತಂಭ headings ಒಳ ಗಿಡ್ಡ format\n\
"));
      oputs (_("\
  -w     omit the user's ತುಂಬಿದ name ಒಳ ಗಿಡ್ಡ format\n\
"));
      oputs (_("\
  -i     omit the user's ತುಂಬಿದ name and remote host ಒಳ ಗಿಡ್ಡ format\n\
"));
      oputs (_("\
  -q     omit the user's ತುಂಬಿದ name, remote host and idle ಸಮಯ ಒಳ ಗಿಡ್ಡ format\n\
"));
      oputs (_("\
      --lookup\n\
         attempt ಗಮ್ಯ canonicalize hostnames ಮೂಲಕ DNS\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      ಬರೆ (_("\
\n\
A lightweight 'finger' ಕಾರ್ಯಕ್ರಮ;  print user information.\n\
The utmp file will be %s.\n\
"), UTMP_FILE);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಂಕ optc;
  ಆಗ ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 's':
          do_short_format = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'l':
          do_short_format = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
          include_heading = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'w':
          include_fullname = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'i':
          include_fullname = false;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#ಆದರೆ_ಅಂತ್ಯ
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'q':
          include_fullname = false;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#ಆದರೆ_ಅಂತ್ಯ
          include_idle = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'h':
          include_project = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'p':
          include_plan = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'b':
          include_home_and_shell = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LOOKUP_OPTION:
          do_lookup = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಪೂರ್ಣಾಂಕ n_users = argc - optind;

  ಆದರೆ (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at ಕನಿಷ್ಠ one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  ಇಲ್ಲದಿದ್ದರೆ
    long_pinky (n_users, argv + optind);

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
