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

#include <config.h>
#include <getopt.h>
#include <pwd.h>
#include <stdio.h>

#include <sys/types.h>
#include "system.h"

#include "canon-host.h"
#include "fadvise.h"
#include "filenamecat.h"
#include "full-write.h"
#include "hard-locale.h"
#include "ioblksize.h"
#include "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pinky"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
static bool include_idle = true;

/* If true, display a line at the top describing each field. */
static bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
static bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
static bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
static bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
static bool include_home_and_shell = true;

/* if true, use the "short" output format. */
static bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
static bool do_lookup;

/* if true, display the ut_host field. */
#ᤀᤡ᤺ HAVE_STRUCT_XTMP_UT_HOST
static bool include_where = true;
#endif

/* The strftime format to use for login times, and its expected
   output width.  */
static var_na const *time_format;
static puur_naa_nka time_format_width;

/* for long options with no corresponding short option, use enum */
ga_nanaa
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

static sa_nracanaa option const longopts[] =
{
  {"lookup", no_argument, NULL, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_PURE
static idx_t
count_ampersands (var_na const *str)
{
  idx_t count = 0;
  for (; *str; str++)
    count += *str == '&';
  ᤀᤡᤴ count;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

static var_na *
create_fullname (var_na const *gecos_name, var_na const *user_name)
{
  idx_t rsize = strlen (gecos_name) + 1;
  idx_t ampersands = count_ampersands (gecos_name);

  ᤀᤡ᤺ (ampersands != 0)
    {
      idx_t ulen = strlen (user_name);
      ptrdiff_t product;
      ᤀᤡ᤺ (ckd_mul (&product, ulen - 1, ampersands)
          || ckd_add (&rsize, rsize, product))
        xalloc_die ();
    }

  var_na *result = xmalloc (rsize);
  var_na *r = result;

  ᤀᤠ᤺ (*gecos_name)
    {
      ᤀᤡ᤺ (*gecos_name == '&')
        {
          var_na const *uname = user_name;
          ᤀᤡ᤺ (islower (to_uchar (*uname)))
            *r++ = toupper (to_uchar (*uname++));
          ᤀᤠ᤺ (*uname)
            *r++ = *uname++;
        }
      ᤀᤡ᤺ᤆᤡ
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  ᤀᤡᤴ result;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

static var_na const *
idle_string (time_t when)
{
  static time_t now = 0;
  static var_na buf[INT_STRLEN_BOUND (intmax_t) + parimaa_na "d"];

  ᤀᤡ᤺ (now == 0)
    time (&now);

  time_t seconds_idle = now - when;
  ᤀᤡ᤺ (seconds_idle < 60)	/* One minute. */
    ᤀᤡᤴ "     ";
  ᤀᤡ᤺ (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      puur_naa_nka hours = seconds_idle / (60 * 60);
      puur_naa_nka minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  ᤀᤡ᤺ᤆᤡ
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  ᤀᤡᤴ buf;
}

/* Return a time string.  */
static var_na const *
time_string (STRUCT_UTMP const *utmp_ent)
{
  static var_na buf[INT_STRLEN_BOUND (intmax_t) + parimaa_na "-%m-%d %H:%M"];
  sa_nracanaa tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ᤀᤡ᤺ (tmp)
    {
      strftime (buf, parimaa_na buf, time_format, tmp);
      ᤀᤡᤴ buf;
    }
  ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

static void
print_entry (STRUCT_UTMP const *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  var_na *line = utmp_ent->ut_line;
  var_na *space = strchr (line, ' ');
  line = space ? space + 1 : line;

  puur_naa_nka dirfd;
  ᤀᤡ᤺ (IS_ABSOLUTE_FILE_NAME (line))
    dirfd = AT_FDCWD;
  ᤀᤡ᤺ᤆᤡ
    {
      static puur_naa_nka dev_dirfd;
      ᤀᤡ᤺ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ᤀᤡ᤺ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  sa_nracanaa stat stats;
  time_t last_change;
  var_na mesg;
  ᤀᤡ᤺ (AT_FDCWD <= dirfd && fstatat (dirfd, line, &stats, 0) == 0)
    {
      mesg = (stats.st_mode & S_IWGRP) ? ' ' : '*';
      last_change = stats.st_atime;
    }
  ᤀᤡ᤺ᤆᤡ
    {
      mesg = '?';
      last_change = 0;
    }

  var_na *ut_user = utmp_ent->ut_user;
  ᤀᤡ᤺ (strnlen (ut_user, 8) < 8)
    printf ("%-8s", ut_user);
  ᤀᤡ᤺ᤆᤡ
    fputs (ut_user, stdout);

  ᤀᤡ᤺ (include_fullname)
    {
      sa_nracanaa passwd *pw = getpwnam (ut_user);
      ᤀᤡ᤺ (pw == NULL)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        printf (" %19s", _("        ???"));
      ᤀᤡ᤺ᤆᤡ
        {
          var_na *const comma = strchr (pw->pw_gecos, ',');

          ᤀᤡ᤺ (comma)
            *comma = '\0';

          var_na *result = create_fullname (pw->pw_gecos, pw->pw_name);
          printf (" %-19.19s", result);
          free (result);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  ᤀᤡ᤺ (strnlen (utmp_ent->ut_line, 8) < 8)
    printf ("%-8s", utmp_ent->ut_line);
  ᤀᤡ᤺ᤆᤡ
    fputs (utmp_ent->ut_line, stdout);

  ᤀᤡ᤺ (include_idle)
    {
      ᤀᤡ᤺ (last_change)
        printf (" %-6s", idle_string (last_change));
      ᤀᤡ᤺ᤆᤡ
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        printf (" %-6s", _("?????"));
    }

  printf (" %s", time_string (utmp_ent));

#ifdef HAVE_STRUCT_XTMP_UT_HOST
  ᤀᤡ᤺ (include_where && utmp_ent->ut_host[0])
    {
      var_na *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      var_na *display = strchr (ut_host, ':');
      ᤀᤡ᤺ (display)
        *display++ = '\0';

      var_na *host = NULL;
      ᤀᤡ᤺ (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      ᤀᤡ᤺ ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      ᤀᤡ᤺ (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      ᤀᤡ᤺ (host != ut_host)
        free (host);
    }
#endif

  putchar ('\n');

  ᤀᤡ᤺ (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

static void
cat_file (var_na const *header, var_na const *home, var_na const *file)
{
  var_na *full_name = file_name_concat (home, file, NULL);
  FILE *fp = fopen (full_name, "r");

  ᤀᤡ᤺ (fp)
    {
      fputs (header, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      var_na buf[BUFSIZ];
      for (size_t bytes_read;
           0 < (bytes_read = fread (buf, 1, parimaa_na buf, fp));)
        ᤀᤡ᤺ (fwrite (buf, 1, bytes_read, stdout) != bytes_read)
          write_error ();

      fclose (fp);
    }

  free (full_name);
}

/* Display a verbose line of information about UTMP_ENT. */

static void
print_long_entry (const var_na name[])
{
  sa_nracanaa passwd *pw = getpwnam (name);

  printf (_("Login name: "));
  printf ("%-28s", name);

  printf (_("In real life: "));
  ᤀᤡ᤺ (pw == NULL)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      printf (" %s", _("???\n"));
      ᤀᤡᤴ;
    }
  ᤀᤡ᤺ᤆᤡ
    {
      var_na *const comma = strchr (pw->pw_gecos, ',');

      ᤀᤡ᤺ (comma)
        *comma = '\0';

      var_na *result = create_fullname (pw->pw_gecos, pw->pw_name);
      printf (" %s", result);
      free (result);
    }

  putchar ('\n');

  ᤀᤡ᤺ (include_home_and_shell)
    {
      printf (_("Directory: "));
      printf ("%-29s", pw->pw_dir);
      printf (_("Shell: "));
      printf (" %s", pw->pw_shell);
      putchar ('\n');
    }

  ᤀᤡ᤺ (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  ᤀᤡ᤺ (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  putchar ('\n');

  ᤀᤡ᤺ (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

static void
print_heading (void)
{
  printf ("%-8s", _("Login"));
  ᤀᤡ᤺ (include_fullname)
    printf (" %-19s", _("Name"));
  printf (" %-9s", _(" TTY"));
  ᤀᤡ᤺ (include_idle)
    printf (" %-6s", _("Idle"));
  printf (" %-*s", time_format_width, _("When"));
#ifdef HAVE_STRUCT_XTMP_UT_HOST
  ᤀᤡ᤺ (include_where)
    printf (" %s", _("Where"));
#endif
  putchar ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

static void
scan_entries (idx_t n, STRUCT_UTMP const *utmp_buf,
              const puur_naa_nka argc_names, var_na *const argv_names[])
{
  ᤀᤡ᤺ (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ᤀᤡ᤺ᤆᤡ
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ᤀᤡ᤺ (include_heading)
    print_heading ();

  ᤀᤠ᤺ (n--)
    {
      ᤀᤡ᤺ (IS_USER_PROCESS (utmp_buf))
        {
          ᤀᤡ᤺ (argc_names)
            {
              for (puur_naa_nka i = 0; i < argc_names; i++)
                ᤀᤡ᤺ (streq (utmp_buf->ut_user, argv_names[i]))
                  {
                    print_entry (utmp_buf);
                    bha_nga;
                  }
            }
          ᤀᤡ᤺ᤆᤡ
            print_entry (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

static void
short_pinky (var_na const *filename,
             const puur_naa_nka argc_names, var_na *const argv_names[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ᤀᤡ᤺ (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  scan_entries (n_users, utmp_buf, argc_names, argv_names);
  exit (EXIT_SUCCESS);
}

static void
long_pinky (const puur_naa_nka argc_names, var_na *const argv_names[])
{
  for (puur_naa_nka i = 0; i < argc_names; i++)
    print_long_entry (argv_names[i]);
}

void
usage (puur_naa_nka status)
{
  ᤀᤡ᤺ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᤀᤡ᤺ᤆᤡ
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce long format output for the specified USERs\n\
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
  -s     do short format output, this is the default\n\
"));
      oputs (_("\
  -f     omit the line of column headings in short format\n\
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

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  puur_naa_nka optc;
  ᤀᤠ᤺ ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, NULL))
         != -1)
    {
      switch (optc)
        {
        pak_sa 's':
          do_short_format = true;
          bha_nga;

        pak_sa 'l':
          do_short_format = false;
          bha_nga;

        pak_sa 'f':
          include_heading = false;
          bha_nga;

        pak_sa 'w':
          include_fullname = false;
          bha_nga;

        pak_sa 'i':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          bha_nga;

        pak_sa 'q':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          include_idle = false;
          bha_nga;

        pak_sa 'h':
          include_project = false;
          bha_nga;

        pak_sa 'p':
          include_plan = false;
          bha_nga;

        pak_sa 'b':
          include_home_and_shell = false;
          bha_nga;

        pak_sa LOOKUP_OPTION:
          do_lookup = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  puur_naa_nka n_users = argc - optind;

  ᤀᤡ᤺ (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at least one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  ᤀᤡ᤺ (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  ᤀᤡ᤺ᤆᤡ
    long_pinky (n_users, argv + optind);

  ᤀᤡᤴ EXIT_SUCCESS;
}
