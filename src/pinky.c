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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <pwd.h>
#அந்தர்பாவ <stdio.h>

#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "canon-host.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "filenamecat.h"
#அந்தர்பாவ "full-write.h"
#அந்தர்பாவ "hard-locale.h"
#அந்தர்பாவ "ioblksize.h"
#அந்தர்பாவ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pinky"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
நிலையான bool include_idle = true;

/* If true, display a line at the top describing each field. */
நிலையான bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
நிலையான bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
நிலையான bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
நிலையான bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
நிலையான bool include_home_and_shell = true;

/* if true, use the "short" output format. */
நிலையான bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
நிலையான bool do_lookup;

/* if true, display the ut_host field. */
#எனில் HAVE_STRUCT_XTMP_UT_HOST
நிலையான bool include_where = true;
#endif

/* The strftime format to use for login times, and its expected
   output width.  */
நிலையான எழுத்து மாறிலி *time_format;
நிலையான முழு time_format_width;

/* for long options with no corresponding short option, use enum */
எண்ணிக்கை
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"lookup", no_argument, வெற்று, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_PURE
நிலையான idx_t
மொத்தம்_ampersands (எழுத்து மாறிலி *str)
{
  idx_t மொத்தம் = 0;
  க்கு (; *str; str++)
    மொத்தம் += *str == '&';
  திரும்பு மொத்தம்;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

நிலையான எழுத்து *
create_fullname (எழுத்து மாறிலி *gecos_name, எழுத்து மாறிலி *user_name)
{
  idx_t rsize = strlen (gecos_name) + 1;
  idx_t ampersands = மொத்தம்_ampersands (gecos_name);

  எனில் (ampersands != 0)
    {
      idx_t ulen = strlen (user_name);
      ptrdiff_t product;
      எனில் (ckd_mul (&product, ulen - 1, ampersands)
          || ckd_add (&rsize, rsize, product))
        xalloc_die ();
    }

  எழுத்து *result = xmalloc (rsize);
  எழுத்து *r = result;

  போது (*gecos_name)
    {
      எனில் (*gecos_name == '&')
        {
          எழுத்து மாறிலி *uname = user_name;
          எனில் (islower (to_uchar (*uname)))
            *r++ = toupper (to_uchar (*uname++));
          போது (*uname)
            *r++ = *uname++;
        }
      இல்லையெனில்
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  திரும்பு result;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

நிலையான எழுத்து மாறிலி *
idle_string (time_t when)
{
  நிலையான time_t now = 0;
  நிலையான எழுத்து buf[INT_STRLEN_BOUND (intmax_t) + அளவு "d"];

  எனில் (now == 0)
    time (&now);

  time_t seconds_idle = now - when;
  எனில் (seconds_idle < 60)	/* One minute. */
    திரும்பு "     ";
  எனில் (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      முழு hours = seconds_idle / (60 * 60);
      முழு minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  இல்லையெனில்
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  திரும்பு buf;
}

/* Return a time string.  */
நிலையான எழுத்து மாறிலி *
time_string (STRUCT_UTMP மாறிலி *utmp_ent)
{
  நிலையான எழுத்து buf[INT_STRLEN_BOUND (intmax_t) + அளவு "-%m-%d %H:%M"];
  கட்டமைப்பு tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  எனில் (tmp)
    {
      strftime (buf, அளவு buf, time_format, tmp);
      திரும்பு buf;
    }
  இல்லையெனில்
    திரும்பு timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

நிலையான வெறுமை
print_entry (STRUCT_UTMP மாறிலி *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  எழுத்து *வரி = utmp_ent->ut_வரி;
  எழுத்து *space = strchr (வரி, ' ');
  வரி = space ? space + 1 : வரி;

  முழு dirfd;
  எனில் (IS_ABSOLUTE_FILE_NAME (வரி))
    dirfd = AT_FDCWD;
  இல்லையெனில்
    {
      நிலையான முழு dev_dirfd;
      எனில் (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          எனில் (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  கட்டமைப்பு stat stats;
  time_t last_change;
  எழுத்து mesg;
  எனில் (AT_FDCWD <= dirfd && fstatat (dirfd, வரி, &stats, 0) == 0)
    {
      mesg = (stats.st_mode & S_IWGRP) ? ' ' : '*';
      last_change = stats.st_atime;
    }
  இல்லையெனில்
    {
      mesg = '?';
      last_change = 0;
    }

  எழுத்து *ut_user = utmp_ent->ut_user;
  எனில் (strnlen (ut_user, 8) < 8)
    printf ("%-8s", ut_user);
  இல்லையெனில்
    fputs (ut_user, stdout);

  எனில் (include_fullname)
    {
      கட்டமைப்பு passwd *pw = getpwnam (ut_user);
      எனில் (pw == வெற்று)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        printf (" %19s", _("        ???"));
      இல்லையெனில்
        {
          எழுத்து *மாறிலி comma = strchr (pw->pw_gecos, ',');

          எனில் (comma)
            *comma = '\0';

          எழுத்து *result = create_fullname (pw->pw_gecos, pw->pw_name);
          printf (" %-19.19s", result);
          விடுவி (result);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  எனில் (strnlen (utmp_ent->ut_வரி, 8) < 8)
    printf ("%-8s", utmp_ent->ut_வரி);
  இல்லையெனில்
    fputs (utmp_ent->ut_வரி, stdout);

  எனில் (include_idle)
    {
      எனில் (last_change)
        printf (" %-6s", idle_string (last_change));
      இல்லையெனில்
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        printf (" %-6s", _("?????"));
    }

  printf (" %s", time_string (utmp_ent));

#ifdef HAVE_STRUCT_XTMP_UT_HOST
  எனில் (include_where && utmp_ent->ut_host[0])
    {
      எழுத்து *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      எழுத்து *display = strchr (ut_host, ':');
      எனில் (display)
        *display++ = '\0';

      எழுத்து *host = வெற்று;
      எனில் (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      எனில் ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      எனில் (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      எனில் (host != ut_host)
        விடுவி (host);
    }
#endif

  putchar ('\n');

  எனில் (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

நிலையான வெறுமை
cat_file (எழுத்து மாறிலி *header, எழுத்து மாறிலி *home, எழுத்து மாறிலி *file)
{
  எழுத்து *full_name = file_name_concat (home, file, வெற்று);
  FILE *fp = fopen (full_name, "r");

  எனில் (fp)
    {
      fputs (header, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      எழுத்து buf[BUFSIZ];
      க்கு (size_t bytes_read;
           0 < (bytes_read = fread (buf, 1, அளவு buf, fp));)
        எனில் (fwrite (buf, 1, bytes_read, stdout) != bytes_read)
          write_error ();

      fclose (fp);
    }

  விடுவி (full_name);
}

/* Display a verbose line of information about UTMP_ENT. */

நிலையான வெறுமை
print_long_entry (மாறிலி எழுத்து name[])
{
  கட்டமைப்பு passwd *pw = getpwnam (name);

  printf (_("Login name: "));
  printf ("%-28s", name);

  printf (_("In real life: "));
  எனில் (pw == வெற்று)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      printf (" %s", _("???\n"));
      திரும்பு;
    }
  இல்லையெனில்
    {
      எழுத்து *மாறிலி comma = strchr (pw->pw_gecos, ',');

      எனில் (comma)
        *comma = '\0';

      எழுத்து *result = create_fullname (pw->pw_gecos, pw->pw_name);
      printf (" %s", result);
      விடுவி (result);
    }

  putchar ('\n');

  எனில் (include_home_and_shell)
    {
      printf (_("Directory: "));
      printf ("%-29s", pw->pw_dir);
      printf (_("Shell: "));
      printf (" %s", pw->pw_shell);
      putchar ('\n');
    }

  எனில் (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  எனில் (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  putchar ('\n');

  எனில் (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

நிலையான வெறுமை
print_heading (வெறுமை)
{
  printf ("%-8s", _("Login"));
  எனில் (include_fullname)
    printf (" %-19s", _("Name"));
  printf (" %-9s", _(" TTY"));
  எனில் (include_idle)
    printf (" %-6s", _("Idle"));
  printf (" %-*s", time_format_width, _("When"));
#ifdef HAVE_STRUCT_XTMP_UT_HOST
  எனில் (include_where)
    printf (" %s", _("Where"));
#endif
  putchar ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

நிலையான வெறுமை
scan_entries (idx_t n, STRUCT_UTMP மாறிலி *utmp_buf,
              மாறிலி முழு argc_names, எழுத்து *மாறிலி argv_names[])
{
  எனில் (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  இல்லையெனில்
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  எனில் (include_heading)
    print_heading ();

  போது (n--)
    {
      எனில் (IS_USER_PROCESS (utmp_buf))
        {
          எனில் (argc_names)
            {
              க்கு (முழு i = 0; i < argc_names; i++)
                எனில் (streq (utmp_buf->ut_user, argv_names[i]))
                  {
                    print_entry (utmp_buf);
                    நிறுத்து;
                  }
            }
          இல்லையெனில்
            print_entry (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

நிலையான வெறுமை
short_pinky (எழுத்து மாறிலி *filename,
             மாறிலி முழு argc_names, எழுத்து *மாறிலி argv_names[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  எனில் (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  scan_entries (n_users, utmp_buf, argc_names, argv_names);
  exit (EXIT_SUCCESS);
}

நிலையான வெறுமை
long_pinky (மாறிலி முழு argc_names, எழுத்து *மாறிலி argv_names[])
{
  க்கு (முழு i = 0; i < argc_names; i++)
    print_long_entry (argv_names[i]);
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce long format output க்கு the specified USERs\n\
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
  -s     செய் short format output, this is the இயல்பு\n\
"));
      oputs (_("\
  -f     omit the வரி of column headings in short format\n\
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

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  முழு optc;
  போது ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, வெற்று))
         != -1)
    {
      தேர்வு (optc)
        {
        நிலை 's':
          do_short_format = true;
          நிறுத்து;

        நிலை 'l':
          do_short_format = false;
          நிறுத்து;

        நிலை 'f':
          include_heading = false;
          நிறுத்து;

        நிலை 'w':
          include_fullname = false;
          நிறுத்து;

        நிலை 'i':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          நிறுத்து;

        நிலை 'q':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          include_idle = false;
          நிறுத்து;

        நிலை 'h':
          include_project = false;
          நிறுத்து;

        நிலை 'p':
          include_plan = false;
          நிறுத்து;

        நிலை 'b':
          include_home_and_shell = false;
          நிறுத்து;

        நிலை LOOKUP_OPTION:
          do_lookup = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  முழு n_users = argc - optind;

  எனில் (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at least one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  எனில் (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  இல்லையெனில்
    long_pinky (n_users, argv + optind);

  திரும்பு EXIT_SUCCESS;
}
