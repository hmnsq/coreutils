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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <pwd.h>
#অন্তর্ভাব <stdio.h>

#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "canon-host.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "filenamecat.h"
#অন্তর্ভাব "full-write.h"
#অন্তর্ভাব "hard-locale.h"
#অন্তর্ভাব "ioblksize.h"
#অন্তর্ভাব "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pinky"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
স্থির bool include_idle = true;

/* If true, display a line at the top describing each field. */
স্থির bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
স্থির bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
স্থির bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
স্থির bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
স্থির bool include_home_and_shell = true;

/* if true, use the "short" output format. */
স্থির bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
স্থির bool do_lookup;

/* if true, display the ut_host field. */
#যদি HAVE_STRUCT_XTMP_UT_HOST
স্থির bool include_where = true;
#endif

/* The strftime format to use for login times, and its expected
   output width.  */
স্থির বর্ণ ধ্রুব *time_format;
স্থির পূর্ণাঙ্ক time_format_width;

/* for long options with no corresponding short option, use enum */
গণনা
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

স্থির সংরচনা option ধ্রুব longopts[] =
{
  {"lookup", no_argument, রিক্ত, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_PURE
স্থির idx_t
গুনতি_ampersands (বর্ণ ধ্রুব *str)
{
  idx_t গুনতি = 0;
  প্রতি (; *str; str++)
    গুনতি += *str == '&';
  প্রত্যাগম গুনতি;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

স্থির বর্ণ *
create_fullname (বর্ণ ধ্রুব *gecos_name, বর্ণ ধ্রুব *user_name)
{
  idx_t rsize = strlen (gecos_name) + 1;
  idx_t ampersands = গুনতি_ampersands (gecos_name);

  যদি (ampersands != 0)
    {
      idx_t ulen = strlen (user_name);
      ptrdiff_t product;
      যদি (ckd_mul (&product, ulen - 1, ampersands)
          || ckd_add (&rsize, rsize, product))
        xalloc_die ();
    }

  বর্ণ *result = xmalloc (rsize);
  বর্ণ *r = result;

  যাবত্ (*gecos_name)
    {
      যদি (*gecos_name == '&')
        {
          বর্ণ ধ্রুব *uname = user_name;
          যদি (islower (to_uchar (*uname)))
            *r++ = toupper (to_uchar (*uname++));
          যাবত্ (*uname)
            *r++ = *uname++;
        }
      অন্যথা
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  প্রত্যাগম result;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

স্থির বর্ণ ধ্রুব *
idle_string (time_t when)
{
  স্থির time_t now = 0;
  স্থির বর্ণ buf[INT_STRLEN_BOUND (intmax_t) + পরিমাণ "d"];

  যদি (now == 0)
    time (&now);

  time_t seconds_idle = now - when;
  যদি (seconds_idle < 60)	/* One minute. */
    প্রত্যাগম "     ";
  যদি (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      পূর্ণাঙ্ক hours = seconds_idle / (60 * 60);
      পূর্ণাঙ্ক minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  অন্যথা
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  প্রত্যাগম buf;
}

/* Return a time string.  */
স্থির বর্ণ ধ্রুব *
time_string (STRUCT_UTMP ধ্রুব *utmp_ent)
{
  স্থির বর্ণ buf[INT_STRLEN_BOUND (intmax_t) + পরিমাণ "-%m-%d %H:%M"];
  সংরচনা tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  যদি (tmp)
    {
      strftime (buf, পরিমাণ buf, time_format, tmp);
      প্রত্যাগম buf;
    }
  অন্যথা
    প্রত্যাগম timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

স্থির শূন্য
print_entry (STRUCT_UTMP ধ্রুব *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  বর্ণ *সারি = utmp_ent->ut_সারি;
  বর্ণ *space = strchr (সারি, ' ');
  সারি = space ? space + 1 : সারি;

  পূর্ণাঙ্ক dirfd;
  যদি (IS_ABSOLUTE_FILE_NAME (সারি))
    dirfd = AT_FDCWD;
  অন্যথা
    {
      স্থির পূর্ণাঙ্ক dev_dirfd;
      যদি (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          যদি (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  সংরচনা stat stats;
  time_t last_change;
  বর্ণ mesg;
  যদি (AT_FDCWD <= dirfd && fstatat (dirfd, সারি, &stats, 0) == 0)
    {
      mesg = (stats.st_mode & S_IWGRP) ? ' ' : '*';
      last_change = stats.st_atime;
    }
  অন্যথা
    {
      mesg = '?';
      last_change = 0;
    }

  বর্ণ *ut_user = utmp_ent->ut_user;
  যদি (strnlen (ut_user, 8) < 8)
    printf ("%-8s", ut_user);
  অন্যথা
    fputs (ut_user, stdout);

  যদি (include_fullname)
    {
      সংরচনা passwd *pw = getpwnam (ut_user);
      যদি (pw == রিক্ত)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        printf (" %19s", _("        ???"));
      অন্যথা
        {
          বর্ণ *ধ্রুব comma = strchr (pw->pw_gecos, ',');

          যদি (comma)
            *comma = '\0';

          বর্ণ *result = create_fullname (pw->pw_gecos, pw->pw_name);
          printf (" %-19.19s", result);
          মুক্ত (result);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  যদি (strnlen (utmp_ent->ut_সারি, 8) < 8)
    printf ("%-8s", utmp_ent->ut_সারি);
  অন্যথা
    fputs (utmp_ent->ut_সারি, stdout);

  যদি (include_idle)
    {
      যদি (last_change)
        printf (" %-6s", idle_string (last_change));
      অন্যথা
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        printf (" %-6s", _("?????"));
    }

  printf (" %s", time_string (utmp_ent));

#ifdef HAVE_STRUCT_XTMP_UT_HOST
  যদি (include_where && utmp_ent->ut_host[0])
    {
      বর্ণ *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      বর্ণ *display = strchr (ut_host, ':');
      যদি (display)
        *display++ = '\0';

      বর্ণ *host = রিক্ত;
      যদি (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      যদি ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      যদি (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      যদি (host != ut_host)
        মুক্ত (host);
    }
#endif

  putchar ('\n');

  যদি (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

স্থির শূন্য
cat_file (বর্ণ ধ্রুব *header, বর্ণ ধ্রুব *home, বর্ণ ধ্রুব *file)
{
  বর্ণ *full_name = file_name_concat (home, file, রিক্ত);
  FILE *fp = fopen (full_name, "r");

  যদি (fp)
    {
      fputs (header, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      বর্ণ buf[BUFSIZ];
      প্রতি (size_t bytes_read;
           0 < (bytes_read = fread (buf, 1, পরিমাণ buf, fp));)
        যদি (fwrite (buf, 1, bytes_read, stdout) != bytes_read)
          write_error ();

      fclose (fp);
    }

  মুক্ত (full_name);
}

/* Display a verbose line of information about UTMP_ENT. */

স্থির শূন্য
print_long_entry (ধ্রুব বর্ণ name[])
{
  সংরচনা passwd *pw = getpwnam (name);

  printf (_("Login name: "));
  printf ("%-28s", name);

  printf (_("In real life: "));
  যদি (pw == রিক্ত)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      printf (" %s", _("???\n"));
      প্রত্যাগম;
    }
  অন্যথা
    {
      বর্ণ *ধ্রুব comma = strchr (pw->pw_gecos, ',');

      যদি (comma)
        *comma = '\0';

      বর্ণ *result = create_fullname (pw->pw_gecos, pw->pw_name);
      printf (" %s", result);
      মুক্ত (result);
    }

  putchar ('\n');

  যদি (include_home_and_shell)
    {
      printf (_("Directory: "));
      printf ("%-29s", pw->pw_dir);
      printf (_("Shell: "));
      printf (" %s", pw->pw_shell);
      putchar ('\n');
    }

  যদি (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  যদি (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  putchar ('\n');

  যদি (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

স্থির শূন্য
print_heading (শূন্য)
{
  printf ("%-8s", _("Login"));
  যদি (include_fullname)
    printf (" %-19s", _("Name"));
  printf (" %-9s", _(" TTY"));
  যদি (include_idle)
    printf (" %-6s", _("Idle"));
  printf (" %-*s", time_format_width, _("When"));
#ifdef HAVE_STRUCT_XTMP_UT_HOST
  যদি (include_where)
    printf (" %s", _("Where"));
#endif
  putchar ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

স্থির শূন্য
scan_entries (idx_t n, STRUCT_UTMP ধ্রুব *utmp_buf,
              ধ্রুব পূর্ণাঙ্ক argc_names, বর্ণ *ধ্রুব argv_names[])
{
  যদি (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  অন্যথা
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  যদি (include_heading)
    print_heading ();

  যাবত্ (n--)
    {
      যদি (IS_USER_PROCESS (utmp_buf))
        {
          যদি (argc_names)
            {
              প্রতি (পূর্ণাঙ্ক i = 0; i < argc_names; i++)
                যদি (streq (utmp_buf->ut_user, argv_names[i]))
                  {
                    print_entry (utmp_buf);
                    ভঙ্গ;
                  }
            }
          অন্যথা
            print_entry (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

স্থির শূন্য
short_pinky (বর্ণ ধ্রুব *filename,
             ধ্রুব পূর্ণাঙ্ক argc_names, বর্ণ *ধ্রুব argv_names[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  যদি (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  scan_entries (n_users, utmp_buf, argc_names, argv_names);
  exit (EXIT_SUCCESS);
}

স্থির শূন্য
long_pinky (ধ্রুব পূর্ণাঙ্ক argc_names, বর্ণ *ধ্রুব argv_names[])
{
  প্রতি (পূর্ণাঙ্ক i = 0; i < argc_names; i++)
    print_long_entry (argv_names[i]);
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce long format output প্রতি the specified USERs\n\
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
  -s     কুরু short format output, this is the স্বভাব\n\
"));
      oputs (_("\
  -f     omit the সারি of column headings in short format\n\
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

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  পূর্ণাঙ্ক optc;
  যাবত্ ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, রিক্ত))
         != -1)
    {
      বিকল্প (optc)
        {
        পক্ষ 's':
          do_short_format = true;
          ভঙ্গ;

        পক্ষ 'l':
          do_short_format = false;
          ভঙ্গ;

        পক্ষ 'f':
          include_heading = false;
          ভঙ্গ;

        পক্ষ 'w':
          include_fullname = false;
          ভঙ্গ;

        পক্ষ 'i':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          ভঙ্গ;

        পক্ষ 'q':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          include_idle = false;
          ভঙ্গ;

        পক্ষ 'h':
          include_project = false;
          ভঙ্গ;

        পক্ষ 'p':
          include_plan = false;
          ভঙ্গ;

        পক্ষ 'b':
          include_home_and_shell = false;
          ভঙ্গ;

        পক্ষ LOOKUP_OPTION:
          do_lookup = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্বভাব:
          usage (EXIT_FAILURE);
        }
    }

  পূর্ণাঙ্ক n_users = argc - optind;

  যদি (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at least one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  যদি (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  অন্যথা
    long_pinky (n_users, argv + optind);

  প্রত্যাগম EXIT_SUCCESS;
}
