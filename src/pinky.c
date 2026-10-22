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

#ضمن <config.h>
#ضمن <getopt.h>
#ضمن <pwd.h>
#ضمن <stdio.h>

#ضمن <sys/types.h>
#ضمن "system.h"

#ضمن "canon-host.h"
#ضمن "fadvise.h"
#ضمن "filenamecat.h"
#ضمن "full-write.h"
#ضمن "hard-locale.h"
#ضمن "ioblksize.h"
#ضمن "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "pinky"

#تعریف_کن AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
ساكن bool include_idle = true;

/* If true, display a line at the top describing each field. */
ساكن bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
ساكن bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
ساكن bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
ساكن bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
ساكن bool include_home_and_shell = true;

/* if true, use the "short" output format. */
ساكن bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ساكن bool do_lookup;

/* if true, display the ut_host field. */
#إذا HAVE_STRUCT_XTMP_UT_HOST
ساكن bool include_where = true;
#پایان_اگر

/* The strftime format to use for login times, and its expected
   output width.  */
ساكن حرف ثابت *زمان_format;
ساكن صحيح زمان_format_عرض;

/* for long options with no corresponding short option, use enum */
تعداد
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"lookup", no_آرگومان, عدم, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_خالص
ساكن idx_t
شمار_ampersands (حرف ثابت *str)
{
  idx_t شمار = 0;
  لكل (; *str; str++)
    شمار += *str == '&';
  أرجع شمار;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

ساكن حرف *
create_fullname (حرف ثابت *gecos_name, حرف ثابت *user_name)
{
  idx_t rsize = طول_رشته (gecos_name) + 1;
  idx_t ampersands = شمار_ampersands (gecos_name);

  إذا (ampersands != 0)
    {
      idx_t ulen = طول_رشته (user_name);
      ptrdiff_t حاصلضرب;
      إذا (ckd_mul (&حاصلضرب, ulen - 1, ampersands)
          || ckd_جمع_کن (&rsize, rsize, حاصلضرب))
        xalloc_die ();
    }

  حرف *نتیجه = xmalloc (rsize);
  حرف *r = نتیجه;

  طالما (*gecos_name)
    {
      إذا (*gecos_name == '&')
        {
          حرف ثابت *uname = user_name;
          إذا (islower (مقصد_uchar (*uname)))
            *r++ = به_بزرگ (مقصد_uchar (*uname++));
          طالما (*uname)
            *r++ = *uname++;
        }
      وإلا
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  أرجع نتیجه;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

ساكن حرف ثابت *
idle_string (time_t when)
{
  ساكن time_t now = 0;
  ساكن حرف buf[INT_STRLEN_BOUND (intmax_t) + حجم "d"];

  إذا (now == 0)
    زمان (&now);

  time_t seconds_idle = now - when;
  إذا (seconds_idle < 60)	/* One minute. */
    أرجع "     ";
  إذا (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      صحيح hours = seconds_idle / (60 * 60);
      صحيح minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  وإلا
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  أرجع buf;
}

/* Return a time string.  */
ساكن حرف ثابت *
زمان_string (STRUCT_UTMP ثابت *utmp_ent)
{
  ساكن حرف buf[INT_STRLEN_BOUND (intmax_t) + حجم "-%m-%d %H:%M"];
  بنية tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  إذا (tmp)
    {
      strftime (buf, حجم buf, زمان_format, tmp);
      أرجع buf;
    }
  وإلا
    أرجع timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

ساكن فراغ
print_مدخل (STRUCT_UTMP ثابت *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  حرف *سطر = utmp_ent->ut_سطر;
  حرف *space = strchr (سطر, ' ');
  سطر = space ? space + 1 : سطر;

  صحيح dirfd;
  إذا (IS_ABSOLUTE_FILE_NAME (سطر))
    dirfd = AT_FDCWD;
  وإلا
    {
      ساكن صحيح dev_dirfd;
      إذا (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          إذا (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  بنية stat stats;
  time_t آخرین_change;
  حرف mesg;
  إذا (AT_FDCWD <= dirfd && fstatat (dirfd, سطر, &stats, 0) == 0)
    {
      mesg = (stats.st_نما & S_IWGRP) ? ' ' : '*';
      آخرین_change = stats.st_atime;
    }
  وإلا
    {
      mesg = '?';
      آخرین_change = 0;
    }

  حرف *ut_user = utmp_ent->ut_user;
  إذا (strnlen (ut_user, 8) < 8)
    چاپ_کن ("%-8s", ut_user);
  وإلا
    fputs (ut_user, stdout);

  إذا (include_fullname)
    {
      بنية passwd *pw = getpwnam (ut_user);
      إذا (pw == عدم)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        چاپ_کن (" %19s", _("        ???"));
      وإلا
        {
          حرف *ثابت comma = strchr (pw->pw_gecos, ',');

          إذا (comma)
            *comma = '\0';

          حرف *نتیجه = create_fullname (pw->pw_gecos, pw->pw_name);
          چاپ_کن (" %-19.19s", نتیجه);
          حرر (نتیجه);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  إذا (strnlen (utmp_ent->ut_سطر, 8) < 8)
    چاپ_کن ("%-8s", utmp_ent->ut_سطر);
  وإلا
    fputs (utmp_ent->ut_سطر, stdout);

  إذا (include_idle)
    {
      إذا (آخرین_change)
        چاپ_کن (" %-6s", idle_string (آخرین_change));
      وإلا
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        چاپ_کن (" %-6s", _("?????"));
    }

  چاپ_کن (" %s", زمان_string (utmp_ent));

#اگر_تعریف_شده HAVE_STRUCT_XTMP_UT_HOST
  إذا (include_where && utmp_ent->ut_host[0])
    {
      حرف *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      حرف *display = strchr (ut_host, ':');
      إذا (display)
        *display++ = '\0';

      حرف *host = عدم;
      إذا (*ut_host && do_lookup)
        /* See if we can canonicalize it.  */
        host = canon_host (ut_host);
      إذا ( ! host)
        host = ut_host;

      fputc (' ', stdout);
      fputs (host, stdout);
      إذا (display)
        {
          fputc (':', stdout);
          fputs (display, stdout);
        }

      إذا (host != ut_host)
        حرر (host);
    }
#پایان_اگر

  حرف_چاپ_کن ('\n');

  إذا (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

ساكن فراغ
cat_file (حرف ثابت *سرآیند, حرف ثابت *home, حرف ثابت *file)
{
  حرف *پر_name = file_name_concat (home, file, عدم);
  FILE *fp = fopen (پر_name, "r");

  إذا (fp)
    {
      fputs (سرآیند, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      حرف buf[BUFSIZ];
      لكل (size_t بایتها_read;
           0 < (بایتها_read = fread (buf, 1, حجم buf, fp));)
        إذا (fwrite (buf, 1, بایتها_read, stdout) != بایتها_read)
          write_error ();

      fclose (fp);
    }

  حرر (پر_name);
}

/* Display a verbose line of information about UTMP_ENT. */

ساكن فراغ
print_long_مدخل (ثابت حرف name[])
{
  بنية passwd *pw = getpwnam (name);

  چاپ_کن (_("Login name: "));
  چاپ_کن ("%-28s", name);

  چاپ_کن (_("In real life: "));
  إذا (pw == عدم)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      چاپ_کن (" %s", _("???\n"));
      أرجع;
    }
  وإلا
    {
      حرف *ثابت comma = strchr (pw->pw_gecos, ',');

      إذا (comma)
        *comma = '\0';

      حرف *نتیجه = create_fullname (pw->pw_gecos, pw->pw_name);
      چاپ_کن (" %s", نتیجه);
      حرر (نتیجه);
    }

  حرف_چاپ_کن ('\n');

  إذا (include_home_and_shell)
    {
      چاپ_کن (_("Directory: "));
      چاپ_کن ("%-29s", pw->pw_dir);
      چاپ_کن (_("Shell: "));
      چاپ_کن (" %s", pw->pw_shell);
      حرف_چاپ_کن ('\n');
    }

  إذا (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  إذا (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  حرف_چاپ_کن ('\n');

  إذا (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

ساكن فراغ
print_heading (فراغ)
{
  چاپ_کن ("%-8s", _("Login"));
  إذا (include_fullname)
    چاپ_کن (" %-19s", _("Name"));
  چاپ_کن (" %-9s", _(" TTY"));
  إذا (include_idle)
    چاپ_کن (" %-6s", _("Idle"));
  چاپ_کن (" %-*s", زمان_format_عرض, _("When"));
#اگر_تعریف_شده HAVE_STRUCT_XTMP_UT_HOST
  إذا (include_where)
    چاپ_کن (" %s", _("Where"));
#پایان_اگر
  حرف_چاپ_کن ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

ساكن فراغ
پیمایش_entries (idx_t n, STRUCT_UTMP ثابت *utmp_buf,
              ثابت صحيح argc_نامها, حرف *ثابت argv_نامها[])
{
  إذا (hard_locale (LC_زمان))
    {
      زمان_format = "%Y-%m-%d %H:%M";
      زمان_format_عرض = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  وإلا
    {
      زمان_format = "%b %e %H:%M";
      زمان_format_عرض = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  إذا (include_heading)
    print_heading ();

  طالما (n--)
    {
      إذا (IS_USER_PROCESS (utmp_buf))
        {
          إذا (argc_نامها)
            {
              لكل (صحيح i = 0; i < argc_نامها; i++)
                إذا (streq (utmp_buf->ut_user, argv_نامها[i]))
                  {
                    print_مدخل (utmp_buf);
                    اكسر;
                  }
            }
          وإلا
            print_مدخل (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

ساكن فراغ
short_pinky (حرف ثابت *filename,
             ثابت صحيح argc_نامها, حرف *ثابت argv_نامها[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  إذا (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  پیمایش_entries (n_users, utmp_buf, argc_نامها, argv_نامها);
  خروج (EXIT_SUCCESS);
}

ساكن فراغ
long_pinky (ثابت صحيح argc_نامها, حرف *ثابت argv_نامها[])
{
  لكل (صحيح i = 0; i < argc_نامها; i++)
    print_long_مدخل (argv_نامها[i]);
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("Usage: %s [OPTION]... [USER]...\n"), برنامه_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce دراز format output لكل the specified USERs\n\
"));
      oputs (_("\
  -b     omit the user's home directory and shell درون دراز format\n\
"));
      oputs (_("\
  -h     omit the user's project file درون دراز format\n\
"));
      oputs (_("\
  -p     omit the user's plan file درون دراز format\n\
"));
      oputs (_("\
  -s     افعل کوتاه format output, this is the افتراضي\n\
"));
      oputs (_("\
  -f     omit the سطر از ستونی headings درون کوتاه format\n\
"));
      oputs (_("\
  -w     omit the user's پر name درون کوتاه format\n\
"));
      oputs (_("\
  -i     omit the user's پر name and remote host درون کوتاه format\n\
"));
      oputs (_("\
  -q     omit the user's پر name, remote host and idle زمان درون کوتاه format\n\
"));
      oputs (_("\
      --lookup\n\
         attempt مقصد canonicalize hostnames از_طریق DNS\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      چاپ_کن (_("\
\n\
A lightweight 'finger' برنامه;  print user information.\n\
The utmp file will be %s.\n\
"), UTMP_FILE);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحيح optc;
  طالما ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, عدم))
         != -1)
    {
      بدل (optc)
        {
        حالة 's':
          do_short_format = true;
          اكسر;

        حالة 'l':
          do_short_format = false;
          اكسر;

        حالة 'f':
          include_heading = false;
          اكسر;

        حالة 'w':
          include_fullname = false;
          اكسر;

        حالة 'i':
          include_fullname = false;
#اگر_تعریف_شده HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#پایان_اگر
          اكسر;

        حالة 'q':
          include_fullname = false;
#اگر_تعریف_شده HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#پایان_اگر
          include_idle = false;
          اكسر;

        حالة 'h':
          include_project = false;
          اكسر;

        حالة 'p':
          include_plan = false;
          اكسر;

        حالة 'b':
          include_home_and_shell = false;
          اكسر;

        حالة LOOKUP_OPTION:
          do_lookup = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  صحيح n_users = argc - optind;

  إذا (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at کمترین one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  إذا (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  وإلا
    long_pinky (n_users, argv + optind);

  أرجع EXIT_SUCCESS;
}
