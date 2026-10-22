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

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <pwd.h>
#అన్తర్భావ <stdio.h>

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"

#అన్తర్భావ "canon-host.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "full-write.h"
#అన్తర్భావ "hard-locale.h"
#అన్తర్భావ "ioblksize.h"
#అన్తర్భావ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "pinky"

#నిర్వచించు AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
స్థిర bool include_idle = true;

/* If true, display a line at the top describing each field. */
స్థిర bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
స్థిర bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
స్థిర bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
స్థిర bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
స్థిర bool include_home_and_shell = true;

/* if true, use the "short" output format. */
స్థిర bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
స్థిర bool do_lookup;

/* if true, display the ut_host field. */
#ఒకవేళ HAVE_STRUCT_XTMP_UT_HOST
స్థిర bool include_where = true;
#అయితే_ముగింపు

/* The strftime format to use for login times, and its expected
   output width.  */
స్థిర అక్షరం స్థిరరాశి *సమయం_format;
స్థిర పూర్ణాంకం సమయం_format_వెడల్పు;

/* for long options with no corresponding short option, use enum */
గణన
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"lookup", no_వాదం, ఖాళీ, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_శుద్ధ
స్థిర idx_t
లెక్క_ampersands (అక్షరం స్థిరరాశి *str)
{
  idx_t లెక్క = 0;
  కొరకు (; *str; str++)
    లెక్క += *str == '&';
  తిరిగి లెక్క;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

స్థిర అక్షరం *
create_fullname (అక్షరం స్థిరరాశి *gecos_name, అక్షరం స్థిరరాశి *user_name)
{
  idx_t rsize = అక్షరశ్రేణి_పొడవు (gecos_name) + 1;
  idx_t ampersands = లెక్క_ampersands (gecos_name);

  ఒకవేళ (ampersands != 0)
    {
      idx_t ulen = అక్షరశ్రేణి_పొడవు (user_name);
      ptrdiff_t లబ్ధం;
      ఒకవేళ (ckd_mul (&లబ్ధం, ulen - 1, ampersands)
          || ckd_కూడు (&rsize, rsize, లబ్ధం))
        xalloc_die ();
    }

  అక్షరం *ఫలితం = xmalloc (rsize);
  అక్షరం *r = ఫలితం;

  అయితే (*gecos_name)
    {
      ఒకవేళ (*gecos_name == '&')
        {
          అక్షరం స్థిరరాశి *uname = user_name;
          ఒకవేళ (islower (గమ్యం_uchar (*uname)))
            *r++ = పెద్ద_అక్షరం (గమ్యం_uchar (*uname++));
          అయితే (*uname)
            *r++ = *uname++;
        }
      లేకపోతే
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  తిరిగి ఫలితం;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

స్థిర అక్షరం స్థిరరాశి *
idle_string (time_t when)
{
  స్థిర time_t now = 0;
  స్థిర అక్షరం buf[INT_STRLEN_BOUND (intmax_t) + పరిమాణం "d"];

  ఒకవేళ (now == 0)
    సమయం (&now);

  time_t seconds_idle = now - when;
  ఒకవేళ (seconds_idle < 60)	/* One minute. */
    తిరిగి "     ";
  ఒకవేళ (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      పూర్ణాంకం hours = seconds_idle / (60 * 60);
      పూర్ణాంకం minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  లేకపోతే
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  తిరిగి buf;
}

/* Return a time string.  */
స్థిర అక్షరం స్థిరరాశి *
సమయం_string (STRUCT_UTMP స్థిరరాశి *utmp_ent)
{
  స్థిర అక్షరం buf[INT_STRLEN_BOUND (intmax_t) + పరిమాణం "-%m-%d %H:%M"];
  నిర్మాణం tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  ఒకవేళ (tmp)
    {
      strftime (buf, పరిమాణం buf, సమయం_format, tmp);
      తిరిగి buf;
    }
  లేకపోతే
    తిరిగి timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

స్థిర శూన్య
print_నమోదు (STRUCT_UTMP స్థిరరాశి *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  అక్షరం *పంక్తి = utmp_ent->ut_పంక్తి;
  అక్షరం *space = strchr (పంక్తి, ' ');
  పంక్తి = space ? space + 1 : పంక్తి;

  పూర్ణాంకం dirfd;
  ఒకవేళ (IS_ABSOLUTE_FILE_NAME (పంక్తి))
    dirfd = AT_FDCWD;
  లేకపోతే
    {
      స్థిర పూర్ణాంకం dev_dirfd;
      ఒకవేళ (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          ఒకవేళ (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  నిర్మాణం stat stats;
  time_t చివరి_change;
  అక్షరం mesg;
  ఒకవేళ (AT_FDCWD <= dirfd && fstatat (dirfd, పంక్తి, &stats, 0) == 0)
    {
      mesg = (stats.st_బహుళకం & S_IWGRP) ? ' ' : '*';
      చివరి_change = stats.st_atime;
    }
  లేకపోతే
    {
      mesg = '?';
      చివరి_change = 0;
    }

  అక్షరం *ut_user = utmp_ent->ut_user;
  ఒకవేళ (strnlen (ut_user, 8) < 8)
    రాయి ("%-8s", ut_user);
  లేకపోతే
    fputs (ut_user, stdout);

  ఒకవేళ (include_fullname)
    {
      నిర్మాణం passwd *pw = getpwnam (ut_user);
      ఒకవేళ (pw == ఖాళీ)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        రాయి (" %19s", _("        ???"));
      లేకపోతే
        {
          అక్షరం *స్థిరరాశి comma = strchr (pw->pw_gecos, ',');

          ఒకవేళ (comma)
            *comma = '\0';

          అక్షరం *ఫలితం = create_fullname (pw->pw_gecos, pw->pw_name);
          రాయి (" %-19.19s", ఫలితం);
          విడుదల (ఫలితం);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  ఒకవేళ (strnlen (utmp_ent->ut_పంక్తి, 8) < 8)
    రాయి ("%-8s", utmp_ent->ut_పంక్తి);
  లేకపోతే
    fputs (utmp_ent->ut_పంక్తి, stdout);

  ఒకవేళ (include_idle)
    {
      ఒకవేళ (చివరి_change)
        రాయి (" %-6s", idle_string (చివరి_change));
      లేకపోతే
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        రాయి (" %-6s", _("?????"));
    }

  రాయి (" %s", సమయం_string (utmp_ent));

#నిర్వచితమైతే HAVE_STRUCT_XTMP_UT_HOST
  ఒకవేళ (include_where && utmp_ent->ut_host[0])
    {
      అక్షరం *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      అక్షరం *display = strchr (ut_host, ':');
      ఒకవేళ (display)
        *display++ = '\0';

      అక్షరం *host = ఖాళీ;
      ఒకవేళ (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      ఒకవేళ ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      ఒకవేళ (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      ఒకవేళ (host != ut_host)
        విడుదల (host);
    }
#అయితే_ముగింపు

  అక్షరం_రాయి ('\n');

  ఒకవేళ (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

స్థిర శూన్య
cat_file (అక్షరం స్థిరరాశి *శీర్షిక, అక్షరం స్థిరరాశి *home, అక్షరం స్థిరరాశి *file)
{
  అక్షరం *నిండిన_name = file_name_concat (home, file, ఖాళీ);
  FILE *fp = fopen (నిండిన_name, "r");

  ఒకవేళ (fp)
    {
      fputs (శీర్షిక, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      అక్షరం buf[BUFSIZ];
      కొరకు (size_t బైట్లు_read;
           0 < (బైట్లు_read = fread (buf, 1, పరిమాణం buf, fp));)
        ఒకవేళ (fwrite (buf, 1, బైట్లు_read, stdout) != బైట్లు_read)
          write_error ();

      fclose (fp);
    }

  విడుదల (నిండిన_name);
}

/* Display a verbose line of information about UTMP_ENT. */

స్థిర శూన్య
print_long_నమోదు (స్థిరరాశి అక్షరం name[])
{
  నిర్మాణం passwd *pw = getpwnam (name);

  రాయి (_("Login name: "));
  రాయి ("%-28s", name);

  రాయి (_("In real life: "));
  ఒకవేళ (pw == ఖాళీ)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      రాయి (" %s", _("???\n"));
      తిరిగి;
    }
  లేకపోతే
    {
      అక్షరం *స్థిరరాశి comma = strchr (pw->pw_gecos, ',');

      ఒకవేళ (comma)
        *comma = '\0';

      అక్షరం *ఫలితం = create_fullname (pw->pw_gecos, pw->pw_name);
      రాయి (" %s", ఫలితం);
      విడుదల (ఫలితం);
    }

  అక్షరం_రాయి ('\n');

  ఒకవేళ (include_home_and_shell)
    {
      రాయి (_("Directory: "));
      రాయి ("%-29s", pw->pw_dir);
      రాయి (_("Shell: "));
      రాయి (" %s", pw->pw_shell);
      అక్షరం_రాయి ('\n');
    }

  ఒకవేళ (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  ఒకవేళ (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  అక్షరం_రాయి ('\n');

  ఒకవేళ (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

స్థిర శూన్య
print_heading (శూన్య)
{
  రాయి ("%-8s", _("Login"));
  ఒకవేళ (include_fullname)
    రాయి (" %-19s", _("Name"));
  రాయి (" %-9s", _(" TTY"));
  ఒకవేళ (include_idle)
    రాయి (" %-6s", _("Idle"));
  రాయి (" %-*s", సమయం_format_వెడల్పు, _("When"));
#నిర్వచితమైతే HAVE_STRUCT_XTMP_UT_HOST
  ఒకవేళ (include_where)
    రాయి (" %s", _("Where"));
#అయితే_ముగింపు
  అక్షరం_రాయి ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

స్థిర శూన్య
పరిశీలన_entries (idx_t n, STRUCT_UTMP స్థిరరాశి *utmp_buf,
              స్థిరరాశి పూర్ణాంకం argc_పేర్లు, అక్షరం *స్థిరరాశి argv_పేర్లు[])
{
  ఒకవేళ (hard_locale (LC_సమయం))
    {
      సమయం_format = "%Y-%m-%d %H:%M";
      సమయం_format_వెడల్పు = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  లేకపోతే
    {
      సమయం_format = "%b %e %H:%M";
      సమయం_format_వెడల్పు = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  ఒకవేళ (include_heading)
    print_heading ();

  అయితే (n--)
    {
      ఒకవేళ (IS_USER_PROCESS (utmp_buf))
        {
          ఒకవేళ (argc_పేర్లు)
            {
              కొరకు (పూర్ణాంకం i = 0; i < argc_పేర్లు; i++)
                ఒకవేళ (streq (utmp_buf->ut_user, argv_పేర్లు[i]))
                  {
                    print_నమోదు (utmp_buf);
                    ఆపు;
                  }
            }
          లేకపోతే
            print_నమోదు (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

స్థిర శూన్య
short_pinky (అక్షరం స్థిరరాశి *filename,
             స్థిరరాశి పూర్ణాంకం argc_పేర్లు, అక్షరం *స్థిరరాశి argv_పేర్లు[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ఒకవేళ (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  పరిశీలన_entries (n_users, utmp_buf, argc_పేర్లు, argv_పేర్లు);
  నిష్క్రమించు (EXIT_SUCCESS);
}

స్థిర శూన్య
long_pinky (స్థిరరాశి పూర్ణాంకం argc_పేర్లు, అక్షరం *స్థిరరాశి argv_పేర్లు[])
{
  కొరకు (పూర్ణాంకం i = 0; i < argc_పేర్లు; i++)
    print_long_నమోదు (argv_పేర్లు[i]);
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... [USER]...\n"), కార్యక్రమం_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce దీర్ఘ format output కొరకు the specified USERs\n\
"));
      oputs (_("\
  -b     omit the user's home directory and shell లోపల దీర్ఘ format\n\
"));
      oputs (_("\
  -h     omit the user's project file లోపల దీర్ఘ format\n\
"));
      oputs (_("\
  -p     omit the user's plan file లోపల దీర్ఘ format\n\
"));
      oputs (_("\
  -s     చేయి పొట్టి format output, this is the అప్రమేయ\n\
"));
      oputs (_("\
  -f     omit the పంక్తి యొక్క స్తంభం headings లోపల పొట్టి format\n\
"));
      oputs (_("\
  -w     omit the user's నిండిన name లోపల పొట్టి format\n\
"));
      oputs (_("\
  -i     omit the user's నిండిన name and remote host లోపల పొట్టి format\n\
"));
      oputs (_("\
  -q     omit the user's నిండిన name, remote host and idle సమయం లోపల పొట్టి format\n\
"));
      oputs (_("\
      --lookup\n\
         attempt గమ్యం canonicalize hostnames ద్వారా DNS\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      రాయి (_("\
\n\
A lightweight 'finger' కార్యక్రమం;  print user information.\n\
The utmp file will be %s.\n\
"), UTMP_FILE);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  పూర్ణాంకం optc;
  అయితే ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, ఖాళీ))
         != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 's':
          do_short_format = true;
          ఆపు;

        స్థితి 'l':
          do_short_format = false;
          ఆపు;

        స్థితి 'f':
          include_heading = false;
          ఆపు;

        స్థితి 'w':
          include_fullname = false;
          ఆపు;

        స్థితి 'i':
          include_fullname = false;
#నిర్వచితమైతే HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#అయితే_ముగింపు
          ఆపు;

        స్థితి 'q':
          include_fullname = false;
#నిర్వచితమైతే HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#అయితే_ముగింపు
          include_idle = false;
          ఆపు;

        స్థితి 'h':
          include_project = false;
          ఆపు;

        స్థితి 'p':
          include_plan = false;
          ఆపు;

        స్థితి 'b':
          include_home_and_shell = false;
          ఆపు;

        స్థితి LOOKUP_OPTION:
          do_lookup = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  పూర్ణాంకం n_users = argc - optind;

  ఒకవేళ (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at కనిష్ఠ one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  లేకపోతే
    long_pinky (n_users, argv + optind);

  తిరిగి EXIT_SUCCESS;
}
