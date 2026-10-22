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

#شامل <config.h>
#شامل <getopt.h>
#شامل <pwd.h>
#شامل <stdio.h>

#شامل <sys/types.h>
#شامل "system.h"

#شامل "canon-host.h"
#شامل "fadvise.h"
#شامل "filenamecat.h"
#شامل "full-write.h"
#شامل "hard-locale.h"
#شامل "ioblksize.h"
#شامل "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pinky"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
ایستا bool include_idle = true;

/* If true, display a line at the top describing each field. */
ایستا bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
ایستا bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
ایستا bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
ایستا bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
ایستا bool include_home_and_shell = true;

/* if true, use the "short" output format. */
ایستا bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ایستا bool do_lookup;

/* if true, display the ut_host field. */
#اگر HAVE_STRUCT_XTMP_UT_HOST
ایستا bool include_where = true;
#endif

/* The strftime format to use for login times, and its expected
   output width.  */
ایستا نویسه ثابت *time_format;
ایستا صحیح time_format_width;

/* for long options with no corresponding short option, use enum */
شمارش
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ایستا ساختار option ثابت longopts[] =
{
  {"lookup", no_argument, پوچ, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_PURE
ایستا idx_t
شمار_ampersands (نویسه ثابت *str)
{
  idx_t شمار = 0;
  برای (; *str; str++)
    شمار += *str == '&';
  بازگشت شمار;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

ایستا نویسه *
create_fullname (نویسه ثابت *gecos_name, نویسه ثابت *user_name)
{
  idx_t rsize = strlen (gecos_name) + 1;
  idx_t ampersands = شمار_ampersands (gecos_name);

  اگر (ampersands != 0)
    {
      idx_t ulen = strlen (user_name);
      ptrdiff_t product;
      اگر (ckd_mul (&product, ulen - 1, ampersands)
          || ckd_add (&rsize, rsize, product))
        xalloc_die ();
    }

  نویسه *result = xmalloc (rsize);
  نویسه *r = result;

  وقتی (*gecos_name)
    {
      اگر (*gecos_name == '&')
        {
          نویسه ثابت *uname = user_name;
          اگر (islower (to_uchar (*uname)))
            *r++ = toupper (to_uchar (*uname++));
          وقتی (*uname)
            *r++ = *uname++;
        }
      وگرنه
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  بازگشت result;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

ایستا نویسه ثابت *
idle_string (time_t when)
{
  ایستا time_t now = 0;
  ایستا نویسه buf[INT_STRLEN_BOUND (intmax_t) + اندازه "d"];

  اگر (now == 0)
    time (&now);

  time_t seconds_idle = now - when;
  اگر (seconds_idle < 60)	/* One minute. */
    بازگشت "     ";
  اگر (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      صحیح hours = seconds_idle / (60 * 60);
      صحیح minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  وگرنه
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  بازگشت buf;
}

/* Return a time string.  */
ایستا نویسه ثابت *
time_string (STRUCT_UTMP ثابت *utmp_ent)
{
  ایستا نویسه buf[INT_STRLEN_BOUND (intmax_t) + اندازه "-%m-%d %H:%M"];
  ساختار tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  اگر (tmp)
    {
      strftime (buf, اندازه buf, time_format, tmp);
      بازگشت buf;
    }
  وگرنه
    بازگشت timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

ایستا تهی
print_entry (STRUCT_UTMP ثابت *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  نویسه *سطر = utmp_ent->ut_سطر;
  نویسه *space = strchr (سطر, ' ');
  سطر = space ? space + 1 : سطر;

  صحیح dirfd;
  اگر (IS_ABSOLUTE_FILE_NAME (سطر))
    dirfd = AT_FDCWD;
  وگرنه
    {
      ایستا صحیح dev_dirfd;
      اگر (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          اگر (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ساختار stat stats;
  time_t last_change;
  نویسه mesg;
  اگر (AT_FDCWD <= dirfd && fstatat (dirfd, سطر, &stats, 0) == 0)
    {
      mesg = (stats.st_mode & S_IWGRP) ? ' ' : '*';
      last_change = stats.st_atime;
    }
  وگرنه
    {
      mesg = '?';
      last_change = 0;
    }

  نویسه *ut_user = utmp_ent->ut_user;
  اگر (strnlen (ut_user, 8) < 8)
    printf ("%-8s", ut_user);
  وگرنه
    fputs (ut_user, stdout);

  اگر (include_fullname)
    {
      ساختار passwd *pw = getpwnam (ut_user);
      اگر (pw == پوچ)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        printf (" %19s", _("        ???"));
      وگرنه
        {
          نویسه *ثابت comma = strchr (pw->pw_gecos, ',');

          اگر (comma)
            *comma = '\0';

          نویسه *result = create_fullname (pw->pw_gecos, pw->pw_name);
          printf (" %-19.19s", result);
          آزاد (result);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  اگر (strnlen (utmp_ent->ut_سطر, 8) < 8)
    printf ("%-8s", utmp_ent->ut_سطر);
  وگرنه
    fputs (utmp_ent->ut_سطر, stdout);

  اگر (include_idle)
    {
      اگر (last_change)
        printf (" %-6s", idle_string (last_change));
      وگرنه
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        printf (" %-6s", _("?????"));
    }

  printf (" %s", time_string (utmp_ent));

#ifdef HAVE_STRUCT_XTMP_UT_HOST
  اگر (include_where && utmp_ent->ut_host[0])
    {
      نویسه *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      نویسه *display = strchr (ut_host, ':');
      اگر (display)
        *display++ = '\0';

      نویسه *host = پوچ;
      اگر (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      اگر ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      اگر (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      اگر (host != ut_host)
        آزاد (host);
    }
#endif

  putchar ('\n');

  اگر (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

ایستا تهی
cat_file (نویسه ثابت *header, نویسه ثابت *home, نویسه ثابت *file)
{
  نویسه *full_name = file_name_concat (home, file, پوچ);
  FILE *fp = fopen (full_name, "r");

  اگر (fp)
    {
      fputs (header, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      نویسه buf[BUFSIZ];
      برای (size_t bytes_read;
           0 < (bytes_read = fread (buf, 1, اندازه buf, fp));)
        اگر (fwrite (buf, 1, bytes_read, stdout) != bytes_read)
          write_error ();

      fclose (fp);
    }

  آزاد (full_name);
}

/* Display a verbose line of information about UTMP_ENT. */

ایستا تهی
print_long_entry (ثابت نویسه name[])
{
  ساختار passwd *pw = getpwnam (name);

  printf (_("Login name: "));
  printf ("%-28s", name);

  printf (_("In real life: "));
  اگر (pw == پوچ)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      printf (" %s", _("???\n"));
      بازگشت;
    }
  وگرنه
    {
      نویسه *ثابت comma = strchr (pw->pw_gecos, ',');

      اگر (comma)
        *comma = '\0';

      نویسه *result = create_fullname (pw->pw_gecos, pw->pw_name);
      printf (" %s", result);
      آزاد (result);
    }

  putchar ('\n');

  اگر (include_home_and_shell)
    {
      printf (_("Directory: "));
      printf ("%-29s", pw->pw_dir);
      printf (_("Shell: "));
      printf (" %s", pw->pw_shell);
      putchar ('\n');
    }

  اگر (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  اگر (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  putchar ('\n');

  اگر (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

ایستا تهی
print_heading (تهی)
{
  printf ("%-8s", _("Login"));
  اگر (include_fullname)
    printf (" %-19s", _("Name"));
  printf (" %-9s", _(" TTY"));
  اگر (include_idle)
    printf (" %-6s", _("Idle"));
  printf (" %-*s", time_format_width, _("When"));
#ifdef HAVE_STRUCT_XTMP_UT_HOST
  اگر (include_where)
    printf (" %s", _("Where"));
#endif
  putchar ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

ایستا تهی
scan_entries (idx_t n, STRUCT_UTMP ثابت *utmp_buf,
              ثابت صحیح argc_names, نویسه *ثابت argv_names[])
{
  اگر (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  وگرنه
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  اگر (include_heading)
    print_heading ();

  وقتی (n--)
    {
      اگر (IS_USER_PROCESS (utmp_buf))
        {
          اگر (argc_names)
            {
              برای (صحیح i = 0; i < argc_names; i++)
                اگر (streq (utmp_buf->ut_user, argv_names[i]))
                  {
                    print_entry (utmp_buf);
                    بشکن;
                  }
            }
          وگرنه
            print_entry (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

ایستا تهی
short_pinky (نویسه ثابت *filename,
             ثابت صحیح argc_names, نویسه *ثابت argv_names[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  اگر (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  scan_entries (n_users, utmp_buf, argc_names, argv_names);
  exit (EXIT_SUCCESS);
}

ایستا تهی
long_pinky (ثابت صحیح argc_names, نویسه *ثابت argv_names[])
{
  برای (صحیح i = 0; i < argc_names; i++)
    print_long_entry (argv_names[i]);
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce long format output برای the specified USERs\n\
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
  -s     انجامبده short format output, this is the پیشفرض\n\
"));
      oputs (_("\
  -f     omit the سطر of column headings in short format\n\
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

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحیح optc;
  وقتی ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, پوچ))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 's':
          do_short_format = true;
          بشکن;

        حالت 'l':
          do_short_format = false;
          بشکن;

        حالت 'f':
          include_heading = false;
          بشکن;

        حالت 'w':
          include_fullname = false;
          بشکن;

        حالت 'i':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          بشکن;

        حالت 'q':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          include_idle = false;
          بشکن;

        حالت 'h':
          include_project = false;
          بشکن;

        حالت 'p':
          include_plan = false;
          بشکن;

        حالت 'b':
          include_home_and_shell = false;
          بشکن;

        حالت LOOKUP_OPTION:
          do_lookup = true;
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  صحیح n_users = argc - optind;

  اگر (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at least one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  اگر (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  وگرنه
    long_pinky (n_users, argv + optind);

  بازگشت EXIT_SUCCESS;
}
