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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <pwd.h>
#અન્તર્ભાવ <stdio.h>

#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "canon-host.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "filenamecat.h"
#અન્તર્ભાવ "full-write.h"
#અન્તર્ભાવ "hard-locale.h"
#અન્તર્ભાવ "ioblksize.h"
#અન્તર્ભાવ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pinky"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
સ્થિર bool include_idle = true;

/* If true, display a line at the top describing each field. */
સ્થિર bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
સ્થિર bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
સ્થિર bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
સ્થિર bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
સ્થિર bool include_home_and_shell = true;

/* if true, use the "short" output format. */
સ્થિર bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
સ્થિર bool do_lookup;

/* if true, display the ut_host field. */
#જો HAVE_STRUCT_XTMP_UT_HOST
સ્થિર bool include_where = true;
#endif

/* The strftime format to use for login times, and its expected
   output width.  */
સ્થિર અક્ષર અચલ *time_format;
સ્થિર પૂર્ણાંક time_format_width;

/* for long options with no corresponding short option, use enum */
ગણના
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"lookup", no_argument, ખાલી, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_PURE
સ્થિર idx_t
ગણતરી_ampersands (અક્ષર અચલ *str)
{
  idx_t ગણતરી = 0;
  માટે (; *str; str++)
    ગણતરી += *str == '&';
  પાછા ગણતરી;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

સ્થિર અક્ષર *
create_fullname (અક્ષર અચલ *gecos_name, અક્ષર અચલ *user_name)
{
  idx_t rsize = strlen (gecos_name) + 1;
  idx_t ampersands = ગણતરી_ampersands (gecos_name);

  જો (ampersands != 0)
    {
      idx_t ulen = strlen (user_name);
      ptrdiff_t product;
      જો (ckd_mul (&product, ulen - 1, ampersands)
          || ckd_add (&rsize, rsize, product))
        xalloc_die ();
    }

  અક્ષર *result = xmalloc (rsize);
  અક્ષર *r = result;

  જ્યારે (*gecos_name)
    {
      જો (*gecos_name == '&')
        {
          અક્ષર અચલ *uname = user_name;
          જો (islower (to_uchar (*uname)))
            *r++ = toupper (to_uchar (*uname++));
          જ્યારે (*uname)
            *r++ = *uname++;
        }
      નહીંતર
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  પાછા result;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

સ્થિર અક્ષર અચલ *
idle_string (time_t when)
{
  સ્થિર time_t now = 0;
  સ્થિર અક્ષર buf[INT_STRLEN_BOUND (intmax_t) + કદ "d"];

  જો (now == 0)
    time (&now);

  time_t seconds_idle = now - when;
  જો (seconds_idle < 60)	/* One minute. */
    પાછા "     ";
  જો (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      પૂર્ણાંક hours = seconds_idle / (60 * 60);
      પૂર્ણાંક minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  નહીંતર
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  પાછા buf;
}

/* Return a time string.  */
સ્થિર અક્ષર અચલ *
time_string (STRUCT_UTMP અચલ *utmp_ent)
{
  સ્થિર અક્ષર buf[INT_STRLEN_BOUND (intmax_t) + કદ "-%m-%d %H:%M"];
  સંરચના tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  જો (tmp)
    {
      strftime (buf, કદ buf, time_format, tmp);
      પાછા buf;
    }
  નહીંતર
    પાછા timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

સ્થિર શૂન્ય
print_entry (STRUCT_UTMP અચલ *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  અક્ષર *લીટી = utmp_ent->ut_લીટી;
  અક્ષર *space = strchr (લીટી, ' ');
  લીટી = space ? space + 1 : લીટી;

  પૂર્ણાંક dirfd;
  જો (IS_ABSOLUTE_FILE_NAME (લીટી))
    dirfd = AT_FDCWD;
  નહીંતર
    {
      સ્થિર પૂર્ણાંક dev_dirfd;
      જો (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          જો (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  સંરચના stat stats;
  time_t last_change;
  અક્ષર mesg;
  જો (AT_FDCWD <= dirfd && fstatat (dirfd, લીટી, &stats, 0) == 0)
    {
      mesg = (stats.st_mode & S_IWGRP) ? ' ' : '*';
      last_change = stats.st_atime;
    }
  નહીંતર
    {
      mesg = '?';
      last_change = 0;
    }

  અક્ષર *ut_user = utmp_ent->ut_user;
  જો (strnlen (ut_user, 8) < 8)
    printf ("%-8s", ut_user);
  નહીંતર
    fputs (ut_user, stdout);

  જો (include_fullname)
    {
      સંરચના passwd *pw = getpwnam (ut_user);
      જો (pw == ખાલી)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        printf (" %19s", _("        ???"));
      નહીંતર
        {
          અક્ષર *અચલ comma = strchr (pw->pw_gecos, ',');

          જો (comma)
            *comma = '\0';

          અક્ષર *result = create_fullname (pw->pw_gecos, pw->pw_name);
          printf (" %-19.19s", result);
          મુક્ત (result);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  જો (strnlen (utmp_ent->ut_લીટી, 8) < 8)
    printf ("%-8s", utmp_ent->ut_લીટી);
  નહીંતર
    fputs (utmp_ent->ut_લીટી, stdout);

  જો (include_idle)
    {
      જો (last_change)
        printf (" %-6s", idle_string (last_change));
      નહીંતર
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        printf (" %-6s", _("?????"));
    }

  printf (" %s", time_string (utmp_ent));

#ifdef HAVE_STRUCT_XTMP_UT_HOST
  જો (include_where && utmp_ent->ut_host[0])
    {
      અક્ષર *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      અક્ષર *display = strchr (ut_host, ':');
      જો (display)
        *display++ = '\0';

      અક્ષર *host = ખાલી;
      જો (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      જો ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      જો (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      જો (host != ut_host)
        મુક્ત (host);
    }
#endif

  putchar ('\n');

  જો (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

સ્થિર શૂન્ય
cat_file (અક્ષર અચલ *header, અક્ષર અચલ *home, અક્ષર અચલ *file)
{
  અક્ષર *full_name = file_name_concat (home, file, ખાલી);
  FILE *fp = fopen (full_name, "r");

  જો (fp)
    {
      fputs (header, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      અક્ષર buf[BUFSIZ];
      માટે (size_t bytes_read;
           0 < (bytes_read = fread (buf, 1, કદ buf, fp));)
        જો (fwrite (buf, 1, bytes_read, stdout) != bytes_read)
          write_error ();

      fclose (fp);
    }

  મુક્ત (full_name);
}

/* Display a verbose line of information about UTMP_ENT. */

સ્થિર શૂન્ય
print_long_entry (અચલ અક્ષર name[])
{
  સંરચના passwd *pw = getpwnam (name);

  printf (_("Login name: "));
  printf ("%-28s", name);

  printf (_("In real life: "));
  જો (pw == ખાલી)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      printf (" %s", _("???\n"));
      પાછા;
    }
  નહીંતર
    {
      અક્ષર *અચલ comma = strchr (pw->pw_gecos, ',');

      જો (comma)
        *comma = '\0';

      અક્ષર *result = create_fullname (pw->pw_gecos, pw->pw_name);
      printf (" %s", result);
      મુક્ત (result);
    }

  putchar ('\n');

  જો (include_home_and_shell)
    {
      printf (_("Directory: "));
      printf ("%-29s", pw->pw_dir);
      printf (_("Shell: "));
      printf (" %s", pw->pw_shell);
      putchar ('\n');
    }

  જો (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  જો (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  putchar ('\n');

  જો (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

સ્થિર શૂન્ય
print_heading (શૂન્ય)
{
  printf ("%-8s", _("Login"));
  જો (include_fullname)
    printf (" %-19s", _("Name"));
  printf (" %-9s", _(" TTY"));
  જો (include_idle)
    printf (" %-6s", _("Idle"));
  printf (" %-*s", time_format_width, _("When"));
#ifdef HAVE_STRUCT_XTMP_UT_HOST
  જો (include_where)
    printf (" %s", _("Where"));
#endif
  putchar ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

સ્થિર શૂન્ય
scan_entries (idx_t n, STRUCT_UTMP અચલ *utmp_buf,
              અચલ પૂર્ણાંક argc_names, અક્ષર *અચલ argv_names[])
{
  જો (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  નહીંતર
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  જો (include_heading)
    print_heading ();

  જ્યારે (n--)
    {
      જો (IS_USER_PROCESS (utmp_buf))
        {
          જો (argc_names)
            {
              માટે (પૂર્ણાંક i = 0; i < argc_names; i++)
                જો (streq (utmp_buf->ut_user, argv_names[i]))
                  {
                    print_entry (utmp_buf);
                    તોડો;
                  }
            }
          નહીંતર
            print_entry (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

સ્થિર શૂન્ય
short_pinky (અક્ષર અચલ *filename,
             અચલ પૂર્ણાંક argc_names, અક્ષર *અચલ argv_names[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  જો (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  scan_entries (n_users, utmp_buf, argc_names, argv_names);
  exit (EXIT_SUCCESS);
}

સ્થિર શૂન્ય
long_pinky (અચલ પૂર્ણાંક argc_names, અક્ષર *અચલ argv_names[])
{
  માટે (પૂર્ણાંક i = 0; i < argc_names; i++)
    print_long_entry (argv_names[i]);
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce long format output માટે the specified USERs\n\
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
  -s     કરો short format output, this is the મૂળભૂત\n\
"));
      oputs (_("\
  -f     omit the લીટી of column headings in short format\n\
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

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  પૂર્ણાંક optc;
  જ્યારે ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, ખાલી))
         != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 's':
          do_short_format = true;
          તોડો;

        સ્થિતિ 'l':
          do_short_format = false;
          તોડો;

        સ્થિતિ 'f':
          include_heading = false;
          તોડો;

        સ્થિતિ 'w':
          include_fullname = false;
          તોડો;

        સ્થિતિ 'i':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          તોડો;

        સ્થિતિ 'q':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          include_idle = false;
          તોડો;

        સ્થિતિ 'h':
          include_project = false;
          તોડો;

        સ્થિતિ 'p':
          include_plan = false;
          તોડો;

        સ્થિતિ 'b':
          include_home_and_shell = false;
          તોડો;

        સ્થિતિ LOOKUP_OPTION:
          do_lookup = true;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  પૂર્ણાંક n_users = argc - optind;

  જો (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at least one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  જો (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  નહીંતર
    long_pinky (n_users, argv + optind);

  પાછા EXIT_SUCCESS;
}
