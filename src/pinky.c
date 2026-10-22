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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pwd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "canon-host.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filenamecat.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "full-write.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hard-locale.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "pinky"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
ਸਥਿਰ bool include_idle = true;

/* If true, display a line at the top describing each field. */
ਸਥਿਰ bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
ਸਥਿਰ bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
ਸਥਿਰ bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
ਸਥਿਰ bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
ਸਥਿਰ bool include_home_and_shell = true;

/* if true, use the "short" output format. */
ਸਥਿਰ bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ਸਥਿਰ bool do_lookup;

/* if true, display the ut_host field. */
#ਜੇ HAVE_STRUCT_XTMP_UT_HOST
ਸਥਿਰ bool include_where = true;
#ਜੇ_ਅੰਤ

/* The strftime format to use for login times, and its expected
   output width.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਸਮਾਂ_format;
ਸਥਿਰ ਪੂਰਨ ਸਮਾਂ_format_ਚੌੜਾਈ;

/* for long options with no corresponding short option, use enum */
ਗਿਣਤੀ
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"lookup", no_ਦਲੀਲ, ਨਿਰਾ, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ idx_t
ਗਿਣਤੀ_ampersands (ਅੱਖਰ ਅਚਲ *str)
{
  idx_t ਗਿਣਤੀ_2 = 0;
  ਲਈ (; *str; str++)
    ਗਿਣਤੀ_2 += *str == '&';
  ਵਾਪਸ ਗਿਣਤੀ_2;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

ਸਥਿਰ ਅੱਖਰ *
create_fullname (ਅੱਖਰ ਅਚਲ *gecos_name, ਅੱਖਰ ਅਚਲ *user_name)
{
  idx_t rsize = ਲੜੀ_ਲੰਬਾਈ (gecos_name) + 1;
  idx_t ampersands = ਗਿਣਤੀ_ampersands (gecos_name);

  ਜੇ (ampersands != 0)
    {
      idx_t ulen = ਲੜੀ_ਲੰਬਾਈ (user_name);
      ptrdiff_t ਗੁਣਨਫਲ;
      ਜੇ (ckd_mul (&ਗੁਣਨਫਲ, ulen - 1, ampersands)
          || ckd_ਜੋੜੋ (&rsize, rsize, ਗੁਣਨਫਲ))
        xalloc_die ();
    }

  ਅੱਖਰ *ਨਤੀਜਾ = xmalloc (rsize);
  ਅੱਖਰ *r = ਨਤੀਜਾ;

  ਜਦੋਂ (*gecos_name)
    {
      ਜੇ (*gecos_name == '&')
        {
          ਅੱਖਰ ਅਚਲ *uname = user_name;
          ਜੇ (islower (ਮੰਜ਼ਿਲ_uchar (*uname)))
            *r++ = ਵੱਡਾ_ਅੱਖਰ (ਮੰਜ਼ਿਲ_uchar (*uname++));
          ਜਦੋਂ (*uname)
            *r++ = *uname++;
        }
      ਨਹੀਂਤਾਂ
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  ਵਾਪਸ ਨਤੀਜਾ;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
idle_string (time_t when)
{
  ਸਥਿਰ time_t now = 0;
  ਸਥਿਰ ਅੱਖਰ buf[INT_STRLEN_BOUND (intmax_t) + ਆਕਾਰ "d"];

  ਜੇ (now == 0)
    ਸਮਾਂ (&now);

  time_t seconds_idle = now - when;
  ਜੇ (seconds_idle < 60)	/* One minute. */
    ਵਾਪਸ "     ";
  ਜੇ (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      ਪੂਰਨ hours = seconds_idle / (60 * 60);
      ਪੂਰਨ minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  ਨਹੀਂਤਾਂ
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  ਵਾਪਸ buf;
}

/* Return a time string.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
ਸਮਾਂ_string (STRUCT_UTMP ਅਚਲ *utmp_ent)
{
  ਸਥਿਰ ਅੱਖਰ buf[INT_STRLEN_BOUND (intmax_t) + ਆਕਾਰ "-%m-%d %H:%M"];
  ਬਣਤਰ tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ਜੇ (tmp)
    {
      strftime (buf, ਆਕਾਰ buf, ਸਮਾਂ_format, tmp);
      ਵਾਪਸ buf;
    }
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

ਸਥਿਰ ਖਾਲੀ
print_ਇੰਦਰਾਜ (STRUCT_UTMP ਅਚਲ *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  ਅੱਖਰ *ਸਤਰ = utmp_ent->ut_ਸਤਰ;
  ਅੱਖਰ *space = strchr (ਸਤਰ, ' ');
  ਸਤਰ = space ? space + 1 : ਸਤਰ;

  ਪੂਰਨ dirfd;
  ਜੇ (IS_ABSOLUTE_FILE_NAME (ਸਤਰ))
    dirfd = AT_FDCWD;
  ਨਹੀਂਤਾਂ
    {
      ਸਥਿਰ ਪੂਰਨ dev_dirfd;
      ਜੇ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ਜੇ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ਬਣਤਰ stat stats;
  time_t ਆਖਰੀ_change;
  ਅੱਖਰ mesg;
  ਜੇ (AT_FDCWD <= dirfd && fstatat (dirfd, ਸਤਰ, &stats, 0) == 0)
    {
      mesg = (stats.st_ਬਹੁਲਕ & S_IWGRP) ? ' ' : '*';
      ਆਖਰੀ_change = stats.st_atime;
    }
  ਨਹੀਂਤਾਂ
    {
      mesg = '?';
      ਆਖਰੀ_change = 0;
    }

  ਅੱਖਰ *ut_user = utmp_ent->ut_user;
  ਜੇ (strnlen (ut_user, 8) < 8)
    ਲਿਖੋ ("%-8s", ut_user);
  ਨਹੀਂਤਾਂ
    fputs (ut_user, stdout);

  ਜੇ (include_fullname)
    {
      ਬਣਤਰ passwd *pw = getpwnam (ut_user);
      ਜੇ (pw == ਨਿਰਾ)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        ਲਿਖੋ (" %19s", _("        ???"));
      ਨਹੀਂਤਾਂ
        {
          ਅੱਖਰ *ਅਚਲ comma = strchr (pw->pw_gecos, ',');

          ਜੇ (comma)
            *comma = '\0';

          ਅੱਖਰ *ਨਤੀਜਾ = create_fullname (pw->pw_gecos, pw->pw_name);
          ਲਿਖੋ (" %-19.19s", ਨਤੀਜਾ);
          ਮੁਕਤ (ਨਤੀਜਾ);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  ਜੇ (strnlen (utmp_ent->ut_ਸਤਰ, 8) < 8)
    ਲਿਖੋ ("%-8s", utmp_ent->ut_ਸਤਰ);
  ਨਹੀਂਤਾਂ
    fputs (utmp_ent->ut_ਸਤਰ, stdout);

  ਜੇ (include_idle)
    {
      ਜੇ (ਆਖਰੀ_change)
        ਲਿਖੋ (" %-6s", idle_string (ਆਖਰੀ_change));
      ਨਹੀਂਤਾਂ
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        ਲਿਖੋ (" %-6s", _("?????"));
    }

  ਲਿਖੋ (" %s", ਸਮਾਂ_string (utmp_ent));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_STRUCT_XTMP_UT_HOST
  ਜੇ (include_where && utmp_ent->ut_host[0])
    {
      ਅੱਖਰ *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      ਅੱਖਰ *display = strchr (ut_host, ':');
      ਜੇ (display)
        *display++ = '\0';

      ਅੱਖਰ *host = ਨਿਰਾ;
      ਜੇ (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      ਜੇ ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      ਜੇ (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      ਜੇ (host != ut_host)
        ਮੁਕਤ (host);
    }
#ਜੇ_ਅੰਤ

  ਅੱਖਰ_ਲਿਖੋ ('\n');

  ਜੇ (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

ਸਥਿਰ ਖਾਲੀ
cat_file (ਅੱਖਰ ਅਚਲ *ਸਿਰਲੇਖ, ਅੱਖਰ ਅਚਲ *home, ਅੱਖਰ ਅਚਲ *file)
{
  ਅੱਖਰ *ਭਰਿਆ_name = file_name_concat (home, file, ਨਿਰਾ);
  FILE *fp = fopen (ਭਰਿਆ_name, "r");

  ਜੇ (fp)
    {
      fputs (ਸਿਰਲੇਖ, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      ਅੱਖਰ buf[BUFSIZ];
      ਲਈ (size_t ਬਾਈਟਾਂ_read;
           0 < (ਬਾਈਟਾਂ_read = fread (buf, 1, ਆਕਾਰ buf, fp));)
        ਜੇ (fwrite (buf, 1, ਬਾਈਟਾਂ_read, stdout) != ਬਾਈਟਾਂ_read)
          write_error ();

      fclose (fp);
    }

  ਮੁਕਤ (ਭਰਿਆ_name);
}

/* Display a verbose line of information about UTMP_ENT. */

ਸਥਿਰ ਖਾਲੀ
print_long_ਇੰਦਰਾਜ (ਅਚਲ ਅੱਖਰ name[])
{
  ਬਣਤਰ passwd *pw = getpwnam (name);

  ਲਿਖੋ (_("Login name: "));
  ਲਿਖੋ ("%-28s", name);

  ਲਿਖੋ (_("In real life: "));
  ਜੇ (pw == ਨਿਰਾ)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      ਲਿਖੋ (" %s", _("???\n"));
      ਵਾਪਸ;
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ *ਅਚਲ comma = strchr (pw->pw_gecos, ',');

      ਜੇ (comma)
        *comma = '\0';

      ਅੱਖਰ *ਨਤੀਜਾ = create_fullname (pw->pw_gecos, pw->pw_name);
      ਲਿਖੋ (" %s", ਨਤੀਜਾ);
      ਮੁਕਤ (ਨਤੀਜਾ);
    }

  ਅੱਖਰ_ਲਿਖੋ ('\n');

  ਜੇ (include_home_and_shell)
    {
      ਲਿਖੋ (_("Directory: "));
      ਲਿਖੋ ("%-29s", pw->pw_dir);
      ਲਿਖੋ (_("Shell: "));
      ਲਿਖੋ (" %s", pw->pw_shell);
      ਅੱਖਰ_ਲਿਖੋ ('\n');
    }

  ਜੇ (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  ਜੇ (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  ਅੱਖਰ_ਲਿਖੋ ('\n');

  ਜੇ (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

ਸਥਿਰ ਖਾਲੀ
print_heading (ਖਾਲੀ)
{
  ਲਿਖੋ ("%-8s", _("Login"));
  ਜੇ (include_fullname)
    ਲਿਖੋ (" %-19s", _("Name"));
  ਲਿਖੋ (" %-9s", _(" TTY"));
  ਜੇ (include_idle)
    ਲਿਖੋ (" %-6s", _("Idle"));
  ਲਿਖੋ (" %-*s", ਸਮਾਂ_format_ਚੌੜਾਈ, _("When"));
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_STRUCT_XTMP_UT_HOST
  ਜੇ (include_where)
    ਲਿਖੋ (" %s", _("Where"));
#ਜੇ_ਅੰਤ
  ਅੱਖਰ_ਲਿਖੋ ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

ਸਥਿਰ ਖਾਲੀ
ਜਾਂਚ_entries (idx_t n, STRUCT_UTMP ਅਚਲ *utmp_buf,
              ਅਚਲ ਪੂਰਨ argc_ਨਾਂ, ਅੱਖਰ *ਅਚਲ argv_ਨਾਂ[])
{
  ਜੇ (hard_locale (LC_ਸਮਾਂ))
    {
      ਸਮਾਂ_format = "%Y-%m-%d %H:%M";
      ਸਮਾਂ_format_ਚੌੜਾਈ = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ਨਹੀਂਤਾਂ
    {
      ਸਮਾਂ_format = "%b %e %H:%M";
      ਸਮਾਂ_format_ਚੌੜਾਈ = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ਜੇ (include_heading)
    print_heading ();

  ਜਦੋਂ (n--)
    {
      ਜੇ (IS_USER_PROCESS (utmp_buf))
        {
          ਜੇ (argc_ਨਾਂ)
            {
              ਲਈ (ਪੂਰਨ i = 0; i < argc_ਨਾਂ; i++)
                ਜੇ (streq (utmp_buf->ut_user, argv_ਨਾਂ[i]))
                  {
                    print_ਇੰਦਰਾਜ (utmp_buf);
                    ਰੋਕੋ;
                  }
            }
          ਨਹੀਂਤਾਂ
            print_ਇੰਦਰਾਜ (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

ਸਥਿਰ ਖਾਲੀ
short_pinky (ਅੱਖਰ ਅਚਲ *filename,
             ਅਚਲ ਪੂਰਨ argc_ਨਾਂ, ਅੱਖਰ *ਅਚਲ argv_ਨਾਂ[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ਜੇ (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  ਜਾਂਚ_entries (n_users, utmp_buf, argc_ਨਾਂ, argv_ਨਾਂ);
  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
}

ਸਥਿਰ ਖਾਲੀ
long_pinky (ਅਚਲ ਪੂਰਨ argc_ਨਾਂ, ਅੱਖਰ *ਅਚਲ argv_ਨਾਂ[])
{
  ਲਈ (ਪੂਰਨ i = 0; i < argc_ਨਾਂ; i++)
    print_long_ਇੰਦਰਾਜ (argv_ਨਾਂ[i]);
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... [USER]...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce ਲੰਮਾ format output ਲਈ the specified USERs\n\
"));
      oputs (_("\
  -b     omit the user's home directory and shell ਵਿੱਚ ਲੰਮਾ format\n\
"));
      oputs (_("\
  -h     omit the user's project file ਵਿੱਚ ਲੰਮਾ format\n\
"));
      oputs (_("\
  -p     omit the user's plan file ਵਿੱਚ ਲੰਮਾ format\n\
"));
      oputs (_("\
  -s     ਕਰੋ ਛੋਟਾ format output, this is the ਮੂਲ\n\
"));
      oputs (_("\
  -f     omit the ਸਤਰ ਦਾ ਕਾਲਮ headings ਵਿੱਚ ਛੋਟਾ format\n\
"));
      oputs (_("\
  -w     omit the user's ਭਰਿਆ name ਵਿੱਚ ਛੋਟਾ format\n\
"));
      oputs (_("\
  -i     omit the user's ਭਰਿਆ name and remote host ਵਿੱਚ ਛੋਟਾ format\n\
"));
      oputs (_("\
  -q     omit the user's ਭਰਿਆ name, remote host and idle ਸਮਾਂ ਵਿੱਚ ਛੋਟਾ format\n\
"));
      oputs (_("\
      --lookup\n\
         attempt ਮੰਜ਼ਿਲ canonicalize hostnames ਰਾਹੀਂ DNS\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      ਲਿਖੋ (_("\
\n\
A lightweight 'finger' ਪ੍ਰੋਗਰਾਮ;  print user information.\n\
The utmp file will be %s.\n\
"), UTMP_FILE);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਪੂਰਨ optc;
  ਜਦੋਂ ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 's':
          do_short_format = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'l':
          do_short_format = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
          include_heading = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'w':
          include_fullname = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'i':
          include_fullname = false;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#ਜੇ_ਅੰਤ
          ਰੋਕੋ;

        ਹਾਲਤ 'q':
          include_fullname = false;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#ਜੇ_ਅੰਤ
          include_idle = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'h':
          include_project = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'p':
          include_plan = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'b':
          include_home_and_shell = false;
          ਰੋਕੋ;

        ਹਾਲਤ LOOKUP_OPTION:
          do_lookup = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਪੂਰਨ n_users = argc - optind;

  ਜੇ (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at ਘੱਟ_ਤੋਂ_ਘੱਟ one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  ਨਹੀਂਤਾਂ
    long_pinky (n_users, argv + optind);

  ਵਾਪਸ EXIT_SUCCESS;
}
