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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <pwd.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>

#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "canon-host.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "filenamecat.h"
#ଅନ୍ତର୍ଭାଵ "full-write.h"
#ଅନ୍ତର୍ଭାଵ "hard-locale.h"
#ଅନ୍ତର୍ଭାଵ "ioblksize.h"
#ଅନ୍ତର୍ଭାଵ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pinky"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
ସ୍ଥିର bool include_idle = true;

/* If true, display a line at the top describing each field. */
ସ୍ଥିର bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
ସ୍ଥିର bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
ସ୍ଥିର bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
ସ୍ଥିର bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
ସ୍ଥିର bool include_home_and_shell = true;

/* if true, use the "short" output format. */
ସ୍ଥିର bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ସ୍ଥିର bool do_lookup;

/* if true, display the ut_host field. */
#ଯଦି HAVE_STRUCT_XTMP_UT_HOST
ସ୍ଥିର bool include_where = true;
#endif

/* The strftime format to use for login times, and its expected
   output width.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *time_format;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା time_format_width;

/* for long options with no corresponding short option, use enum */
ଗଣନା
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"lookup", no_argument, ଖାଲି, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_PURE
ସ୍ଥିର idx_t
ଗଣନା_ampersands (ଅକ୍ଷର ଅଚଳ *str)
{
  idx_t ଗଣନା_2 = 0;
  ପାଇଁ (; *str; str++)
    ଗଣନା_2 += *str == '&';
  ଫେରନ୍ତୁ ଗଣନା_2;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

ସ୍ଥିର ଅକ୍ଷର *
create_fullname (ଅକ୍ଷର ଅଚଳ *gecos_name, ଅକ୍ଷର ଅଚଳ *user_name)
{
  idx_t rsize = strlen (gecos_name) + 1;
  idx_t ampersands = ଗଣନା_ampersands (gecos_name);

  ଯଦି (ampersands != 0)
    {
      idx_t ulen = strlen (user_name);
      ptrdiff_t product;
      ଯଦି (ckd_mul (&product, ulen - 1, ampersands)
          || ckd_add (&rsize, rsize, product))
        xalloc_die ();
    }

  ଅକ୍ଷର *result = xmalloc (rsize);
  ଅକ୍ଷର *r = result;

  ଯେତେବେଳେ (*gecos_name)
    {
      ଯଦି (*gecos_name == '&')
        {
          ଅକ୍ଷର ଅଚଳ *uname = user_name;
          ଯଦି (islower (to_uchar (*uname)))
            *r++ = toupper (to_uchar (*uname++));
          ଯେତେବେଳେ (*uname)
            *r++ = *uname++;
        }
      ଅନ୍ୟଥା
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  ଫେରନ୍ତୁ result;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
idle_string (time_t when)
{
  ସ୍ଥିର time_t now = 0;
  ସ୍ଥିର ଅକ୍ଷର buf[INT_STRLEN_BOUND (intmax_t) + ଆକାର "d"];

  ଯଦି (now == 0)
    time (&now);

  time_t seconds_idle = now - when;
  ଯଦି (seconds_idle < 60)	/* One minute. */
    ଫେରନ୍ତୁ "     ";
  ଯଦି (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା hours = seconds_idle / (60 * 60);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  ଅନ୍ୟଥା
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  ଫେରନ୍ତୁ buf;
}

/* Return a time string.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
time_string (STRUCT_UTMP ଅଚଳ *utmp_ent)
{
  ସ୍ଥିର ଅକ୍ଷର buf[INT_STRLEN_BOUND (intmax_t) + ଆକାର "-%m-%d %H:%M"];
  ସଂରଚନା tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ଯଦି (tmp)
    {
      strftime (buf, ଆକାର buf, time_format, tmp);
      ଫେରନ୍ତୁ buf;
    }
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

ସ୍ଥିର ଶୂନ୍ୟ
print_entry (STRUCT_UTMP ଅଚଳ *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  ଅକ୍ଷର *ଧାଡ଼ି = utmp_ent->ut_ଧାଡ଼ି;
  ଅକ୍ଷର *space = strchr (ଧାଡ଼ି, ' ');
  ଧାଡ଼ି = space ? space + 1 : ଧାଡ଼ି;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd;
  ଯଦି (IS_ABSOLUTE_FILE_NAME (ଧାଡ଼ି))
    dirfd = AT_FDCWD;
  ଅନ୍ୟଥା
    {
      ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା dev_dirfd;
      ଯଦି (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ଯଦି (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ସଂରଚନା stat stats;
  time_t last_change;
  ଅକ୍ଷର mesg;
  ଯଦି (AT_FDCWD <= dirfd && fstatat (dirfd, ଧାଡ଼ି, &stats, 0) == 0)
    {
      mesg = (stats.st_mode & S_IWGRP) ? ' ' : '*';
      last_change = stats.st_atime;
    }
  ଅନ୍ୟଥା
    {
      mesg = '?';
      last_change = 0;
    }

  ଅକ୍ଷର *ut_user = utmp_ent->ut_user;
  ଯଦି (strnlen (ut_user, 8) < 8)
    printf ("%-8s", ut_user);
  ଅନ୍ୟଥା
    fputs (ut_user, stdout);

  ଯଦି (include_fullname)
    {
      ସଂରଚନା passwd *pw = getpwnam (ut_user);
      ଯଦି (pw == ଖାଲି)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        printf (" %19s", _("        ???"));
      ଅନ୍ୟଥା
        {
          ଅକ୍ଷର *ଅଚଳ comma = strchr (pw->pw_gecos, ',');

          ଯଦି (comma)
            *comma = '\0';

          ଅକ୍ଷର *result = create_fullname (pw->pw_gecos, pw->pw_name);
          printf (" %-19.19s", result);
          ମୁକ୍ତ (result);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  ଯଦି (strnlen (utmp_ent->ut_ଧାଡ଼ି, 8) < 8)
    printf ("%-8s", utmp_ent->ut_ଧାଡ଼ି);
  ଅନ୍ୟଥା
    fputs (utmp_ent->ut_ଧାଡ଼ି, stdout);

  ଯଦି (include_idle)
    {
      ଯଦି (last_change)
        printf (" %-6s", idle_string (last_change));
      ଅନ୍ୟଥା
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        printf (" %-6s", _("?????"));
    }

  printf (" %s", time_string (utmp_ent));

#ifdef HAVE_STRUCT_XTMP_UT_HOST
  ଯଦି (include_where && utmp_ent->ut_host[0])
    {
      ଅକ୍ଷର *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      ଅକ୍ଷର *display = strchr (ut_host, ':');
      ଯଦି (display)
        *display++ = '\0';

      ଅକ୍ଷର *host = ଖାଲି;
      ଯଦି (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      ଯଦି ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      ଯଦି (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      ଯଦି (host != ut_host)
        ମୁକ୍ତ (host);
    }
#endif

  putchar ('\n');

  ଯଦି (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

ସ୍ଥିର ଶୂନ୍ୟ
cat_file (ଅକ୍ଷର ଅଚଳ *header, ଅକ୍ଷର ଅଚଳ *home, ଅକ୍ଷର ଅଚଳ *file)
{
  ଅକ୍ଷର *full_name = file_name_concat (home, file, ଖାଲି);
  FILE *fp = fopen (full_name, "r");

  ଯଦି (fp)
    {
      fputs (header, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      ଅକ୍ଷର buf[BUFSIZ];
      ପାଇଁ (size_t bytes_read;
           0 < (bytes_read = fread (buf, 1, ଆକାର buf, fp));)
        ଯଦି (fwrite (buf, 1, bytes_read, stdout) != bytes_read)
          write_error ();

      fclose (fp);
    }

  ମୁକ୍ତ (full_name);
}

/* Display a verbose line of information about UTMP_ENT. */

ସ୍ଥିର ଶୂନ୍ୟ
print_long_entry (ଅଚଳ ଅକ୍ଷର name[])
{
  ସଂରଚନା passwd *pw = getpwnam (name);

  printf (_("Login name: "));
  printf ("%-28s", name);

  printf (_("In real life: "));
  ଯଦି (pw == ଖାଲି)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      printf (" %s", _("???\n"));
      ଫେରନ୍ତୁ;
    }
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର *ଅଚଳ comma = strchr (pw->pw_gecos, ',');

      ଯଦି (comma)
        *comma = '\0';

      ଅକ୍ଷର *result = create_fullname (pw->pw_gecos, pw->pw_name);
      printf (" %s", result);
      ମୁକ୍ତ (result);
    }

  putchar ('\n');

  ଯଦି (include_home_and_shell)
    {
      printf (_("Directory: "));
      printf ("%-29s", pw->pw_dir);
      printf (_("Shell: "));
      printf (" %s", pw->pw_shell);
      putchar ('\n');
    }

  ଯଦି (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  ଯଦି (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  putchar ('\n');

  ଯଦି (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

ସ୍ଥିର ଶୂନ୍ୟ
print_heading (ଶୂନ୍ୟ)
{
  printf ("%-8s", _("Login"));
  ଯଦି (include_fullname)
    printf (" %-19s", _("Name"));
  printf (" %-9s", _(" TTY"));
  ଯଦି (include_idle)
    printf (" %-6s", _("Idle"));
  printf (" %-*s", time_format_width, _("When"));
#ifdef HAVE_STRUCT_XTMP_UT_HOST
  ଯଦି (include_where)
    printf (" %s", _("Where"));
#endif
  putchar ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

ସ୍ଥିର ଶୂନ୍ୟ
scan_entries (idx_t n, STRUCT_UTMP ଅଚଳ *utmp_buf,
              ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc_names, ଅକ୍ଷର *ଅଚଳ argv_names[])
{
  ଯଦି (hard_locale (LC_TIME))
    {
      time_format = "%Y-%m-%d %H:%M";
      time_format_width = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ଅନ୍ୟଥା
    {
      time_format = "%b %e %H:%M";
      time_format_width = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ଯଦି (include_heading)
    print_heading ();

  ଯେତେବେଳେ (n--)
    {
      ଯଦି (IS_USER_PROCESS (utmp_buf))
        {
          ଯଦି (argc_names)
            {
              ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < argc_names; i++)
                ଯଦି (streq (utmp_buf->ut_user, argv_names[i]))
                  {
                    print_entry (utmp_buf);
                    ଭାଙ୍ଗ;
                  }
            }
          ଅନ୍ୟଥା
            print_entry (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

ସ୍ଥିର ଶୂନ୍ୟ
short_pinky (ଅକ୍ଷର ଅଚଳ *filename,
             ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc_names, ଅକ୍ଷର *ଅଚଳ argv_names[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ଯଦି (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  scan_entries (n_users, utmp_buf, argc_names, argv_names);
  exit (EXIT_SUCCESS);
}

ସ୍ଥିର ଶୂନ୍ୟ
long_pinky (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc_names, ଅକ୍ଷର *ଅଚଳ argv_names[])
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < argc_names; i++)
    print_long_entry (argv_names[i]);
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce long format output ପାଇଁ the specified USERs\n\
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
  -s     କର short format output, this is the ପୂର୍ବନିର୍ଦ୍ଧାରିତ\n\
"));
      oputs (_("\
  -f     omit the ଧାଡ଼ି of column headings in short format\n\
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, ଖାଲି))
         != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 's':
          do_short_format = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'l':
          do_short_format = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'f':
          include_heading = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'w':
          include_fullname = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'i':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'q':
          include_fullname = false;
#ifdef HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#endif
          include_idle = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'h':
          include_project = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'p':
          include_plan = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'b':
          include_home_and_shell = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି LOOKUP_OPTION:
          do_lookup = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_users = argc - optind;

  ଯଦି (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at least one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  ଅନ୍ୟଥା
    long_pinky (n_users, argv + optind);

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
