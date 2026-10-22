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
#تعریف_کرو پروگرام_NAME "pinky"

#تعریف_کرو AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

/* If true, display the hours:minutes since each user has touched
   the keyboard, or blank if within the last minute, or days followed
   by a 'd' if not within the last day. */
ساکن bool include_idle = true;

/* If true, display a line at the top describing each field. */
ساکن bool include_heading = true;

/* if true, display the user's full name from pw_gecos. */
ساکن bool include_fullname = true;

/* if true, display the user's ~/.project file when doing long format. */
ساکن bool include_project = true;

/* if true, display the user's ~/.plan file when doing long format. */
ساکن bool include_plan = true;

/* if true, display the user's home directory and shell
   when doing long format. */
ساکن bool include_home_and_shell = true;

/* if true, use the "short" output format. */
ساکن bool do_short_format = true;

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
ساکن bool do_lookup;

/* if true, display the ut_host field. */
#اگر HAVE_STRUCT_XTMP_UT_HOST
ساکن bool include_where = true;
#اگر_ختم

/* The strftime format to use for login times, and its expected
   output width.  */
ساکن حرف ثابت *وقت_format;
ساکن صحیح وقت_format_چوڑائی;

/* for long options with no corresponding short option, use enum */
شمار
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت longopts[] =
{
  {"lookup", no_دلیل, تہی, LOOKUP_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Count and return the number of ampersands in STR.  */

ATTRIBUTE_خالص
ساکن idx_t
گنتی_ampersands (حرف ثابت *str)
{
  idx_t گنتی = 0;
  برائے (; *str; str++)
    گنتی += *str == '&';
  واپس گنتی;
}

/* Create a string (via xmalloc) which contains a full name by substituting
   for each ampersand in GECOS_NAME the USER_NAME string with its first
   character capitalized.  The caller must ensure that GECOS_NAME contains
   no ','s.  The caller also is responsible for free'ing the return value of
   this function.  */

ساکن حرف *
create_fullname (حرف ثابت *gecos_name, حرف ثابت *user_name)
{
  idx_t rsize = لڑی_لمبائی (gecos_name) + 1;
  idx_t ampersands = گنتی_ampersands (gecos_name);

  اگر (ampersands != 0)
    {
      idx_t ulen = لڑی_لمبائی (user_name);
      ptrdiff_t حاصل_ضرب;
      اگر (ckd_mul (&حاصل_ضرب, ulen - 1, ampersands)
          || ckd_جوڑو (&rsize, rsize, حاصل_ضرب))
        xalloc_die ();
    }

  حرف *نتیجہ = xmalloc (rsize);
  حرف *r = نتیجہ;

  جب (*gecos_name)
    {
      اگر (*gecos_name == '&')
        {
          حرف ثابت *uname = user_name;
          اگر (islower (مقصد_uchar (*uname)))
            *r++ = بڑا_حرف (مقصد_uchar (*uname++));
          جب (*uname)
            *r++ = *uname++;
        }
      ورنہ
        {
          *r++ = *gecos_name;
        }

      gecos_name++;
    }
  *r = 0;

  واپس نتیجہ;
}

/* Return a string representing the time between WHEN and the time
   that this function is first run. */

ساکن حرف ثابت *
idle_string (time_t when)
{
  ساکن time_t now = 0;
  ساکن حرف buf[INT_STRLEN_BOUND (intmax_t) + حجم "d"];

  اگر (now == 0)
    وقت (&now);

  time_t seconds_idle = now - when;
  اگر (seconds_idle < 60)	/* One minute. */
    واپس "     ";
  اگر (seconds_idle < (24 * 60 * 60))	/* One day. */
    {
      صحیح hours = seconds_idle / (60 * 60);
      صحیح minutes = (seconds_idle % (60 * 60)) / 60;
      sprintf (buf, "%02d:%02d", hours, minutes);
    }
  ورنہ
    {
      intmax_t days = seconds_idle / (24 * 60 * 60);
      sprintf (buf, "%jdd", days);
    }
  واپس buf;
}

/* Return a time string.  */
ساکن حرف ثابت *
وقت_string (STRUCT_UTMP ثابت *utmp_ent)
{
  ساکن حرف buf[INT_STRLEN_BOUND (intmax_t) + حجم "-%m-%d %H:%M"];
  ساخت tm *tmp = localtime (&utmp_ent->ut_ts.tv_sec);

  اگر (tmp)
    {
      strftime (buf, حجم buf, وقت_format, tmp);
      واپس buf;
    }
  ورنہ
    واپس timetostr (utmp_ent->ut_ts.tv_sec, buf);
}

/* Display a line of information about UTMP_ENT. */

ساکن خالی
print_اندراج (STRUCT_UTMP ثابت *utmp_ent)
{
  /* If ut_line contains a space, the device name starts after the space.  */
  حرف *سطر = utmp_ent->ut_سطر;
  حرف *space = strchr (سطر, ' ');
  سطر = space ? space + 1 : سطر;

  صحیح dirfd;
  اگر (IS_ABSOLUTE_FILE_NAME (سطر))
    dirfd = AT_FDCWD;
  ورنہ
    {
      ساکن صحیح dev_dirfd;
      اگر (!dev_dirfd)
        {
          dev_dirfd = open ("/dev", O_PATHSEARCH | O_DIRECTORY);
          اگر (dev_dirfd < 0)
            dev_dirfd = AT_FDCWD - 1;
        }
      dirfd = dev_dirfd;
    }

  ساخت stat stats;
  time_t آخری_change;
  حرف mesg;
  اگر (AT_FDCWD <= dirfd && fstatat (dirfd, سطر, &stats, 0) == 0)
    {
      mesg = (stats.st_منوال & S_IWGRP) ? ' ' : '*';
      آخری_change = stats.st_atime;
    }
  ورنہ
    {
      mesg = '?';
      آخری_change = 0;
    }

  حرف *ut_user = utmp_ent->ut_user;
  اگر (strnlen (ut_user, 8) < 8)
    لکھو ("%-8s", ut_user);
  ورنہ
    fputs (ut_user, stdout);

  اگر (include_fullname)
    {
      ساخت passwd *pw = getpwnam (ut_user);
      اگر (pw == تہی)
        /* TRANSLATORS: Real name is unknown; at most 19 characters. */
        لکھو (" %19s", _("        ???"));
      ورنہ
        {
          حرف *ثابت comma = strchr (pw->pw_gecos, ',');

          اگر (comma)
            *comma = '\0';

          حرف *نتیجہ = create_fullname (pw->pw_gecos, pw->pw_name);
          لکھو (" %-19.19s", نتیجہ);
          آزاد (نتیجہ);
        }
    }

  fputc (' ', stdout);
  fputc (mesg, stdout);
  اگر (strnlen (utmp_ent->ut_سطر, 8) < 8)
    لکھو ("%-8s", utmp_ent->ut_سطر);
  ورنہ
    fputs (utmp_ent->ut_سطر, stdout);

  اگر (include_idle)
    {
      اگر (آخری_change)
        لکھو (" %-6s", idle_string (آخری_change));
      ورنہ
        /* TRANSLATORS: Idle time is unknown; at most 5 characters. */
        لکھو (" %-6s", _("?????"));
    }

  لکھو (" %s", وقت_string (utmp_ent));

#اگر_تعریف_شدہ HAVE_STRUCT_XTMP_UT_HOST
  اگر (include_where && utmp_ent->ut_host[0])
    {
      حرف *ut_host = utmp_ent->ut_host;

      /* Look for an X display.  */
      حرف *display = strchr (ut_host, ':');
      اگر (display)
        *display++ = '\0';

      حرف *host = تہی;
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
#اگر_ختم

  حرف_لکھو ('\n');

  اگر (ferror (stdout))
    write_error ();
}

/* If FILE exists in HOME, print it to standard output, preceded by HEADER. */

ساکن خالی
cat_file (حرف ثابت *سرخی, حرف ثابت *home, حرف ثابت *file)
{
  حرف *بھرا_name = file_name_concat (home, file, تہی);
  FILE *fp = fopen (بھرا_name, "r");

  اگر (fp)
    {
      fputs (سرخی, stdout);

      fadvise (fp, FADVISE_SEQUENTIAL);

      حرف buf[BUFSIZ];
      برائے (size_t بائٹیں_read;
           0 < (بائٹیں_read = fread (buf, 1, حجم buf, fp));)
        اگر (fwrite (buf, 1, بائٹیں_read, stdout) != بائٹیں_read)
          write_error ();

      fclose (fp);
    }

  آزاد (بھرا_name);
}

/* Display a verbose line of information about UTMP_ENT. */

ساکن خالی
print_long_اندراج (ثابت حرف name[])
{
  ساخت passwd *pw = getpwnam (name);

  لکھو (_("Login name: "));
  لکھو ("%-28s", name);

  لکھو (_("In real life: "));
  اگر (pw == تہی)
    {
      /* TRANSLATORS: Real name is unknown; no hard limit. */
      لکھو (" %s", _("???\n"));
      واپس;
    }
  ورنہ
    {
      حرف *ثابت comma = strchr (pw->pw_gecos, ',');

      اگر (comma)
        *comma = '\0';

      حرف *نتیجہ = create_fullname (pw->pw_gecos, pw->pw_name);
      لکھو (" %s", نتیجہ);
      آزاد (نتیجہ);
    }

  حرف_لکھو ('\n');

  اگر (include_home_and_shell)
    {
      لکھو (_("Directory: "));
      لکھو ("%-29s", pw->pw_dir);
      لکھو (_("Shell: "));
      لکھو (" %s", pw->pw_shell);
      حرف_لکھو ('\n');
    }

  اگر (include_project)
    cat_file (_("Project: "), pw->pw_dir, ".project");

  اگر (include_plan)
    cat_file (_("Plan:\n"), pw->pw_dir, ".plan");

  حرف_لکھو ('\n');

  اگر (ferror (stdout))
    write_error ();
}

/* Print the username of each valid entry and the number of valid entries
   in UTMP_BUF, which should have N elements. */

ساکن خالی
print_heading (خالی)
{
  لکھو ("%-8s", _("Login"));
  اگر (include_fullname)
    لکھو (" %-19s", _("Name"));
  لکھو (" %-9s", _(" TTY"));
  اگر (include_idle)
    لکھو (" %-6s", _("Idle"));
  لکھو (" %-*s", وقت_format_چوڑائی, _("When"));
#اگر_تعریف_شدہ HAVE_STRUCT_XTMP_UT_HOST
  اگر (include_where)
    لکھو (" %s", _("Where"));
#اگر_ختم
  حرف_لکھو ('\n');
}

/* Display UTMP_BUF, which should have N entries. */

ساکن خالی
جائزہ_entries (idx_t n, STRUCT_UTMP ثابت *utmp_buf,
              ثابت صحیح argc_نام, حرف *ثابت argv_نام[])
{
  اگر (hard_locale (LC_وقت))
    {
      وقت_format = "%Y-%m-%d %H:%M";
      وقت_format_چوڑائی = 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2;
    }
  ورنہ
    {
      وقت_format = "%b %e %H:%M";
      وقت_format_چوڑائی = 3 + 1 + 2 + 1 + 2 + 1 + 2;
    }

  اگر (include_heading)
    print_heading ();

  جب (n--)
    {
      اگر (IS_USER_PROCESS (utmp_buf))
        {
          اگر (argc_نام)
            {
              برائے (صحیح i = 0; i < argc_نام; i++)
                اگر (streq (utmp_buf->ut_user, argv_نام[i]))
                  {
                    print_اندراج (utmp_buf);
                    توڑو;
                  }
            }
          ورنہ
            print_اندراج (utmp_buf);
        }
      utmp_buf++;
    }
}

/* Display a list of who is on the system, according to utmp file FILENAME. */

ساکن خالی
short_pinky (حرف ثابت *filename,
             ثابت صحیح argc_نام, حرف *ثابت argv_نام[])
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  اگر (read_utmp (filename, &n_users, &utmp_buf, READ_UTMP_USER_PROCESS) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  جائزہ_entries (n_users, utmp_buf, argc_نام, argv_نام);
  باہر_نکلو (EXIT_SUCCESS);
}

ساکن خالی
long_pinky (ثابت صحیح argc_نام, حرف *ثابت argv_نام[])
{
  برائے (صحیح i = 0; i < argc_نام; i++)
    print_long_اندراج (argv_نام[i]);
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [USER]...\n"), پروگرام_name);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -l     produce طویل format output برائے the specified USERs\n\
"));
      oputs (_("\
  -b     omit the user's home directory and shell اندر طویل format\n\
"));
      oputs (_("\
  -h     omit the user's project file اندر طویل format\n\
"));
      oputs (_("\
  -p     omit the user's plan file اندر طویل format\n\
"));
      oputs (_("\
  -s     کرو مختصر format output, this is the طےشدہ\n\
"));
      oputs (_("\
  -f     omit the سطر کا عمود headings اندر مختصر format\n\
"));
      oputs (_("\
  -w     omit the user's بھرا name اندر مختصر format\n\
"));
      oputs (_("\
  -i     omit the user's بھرا name and remote host اندر مختصر format\n\
"));
      oputs (_("\
  -q     omit the user's بھرا name, remote host and idle وقت اندر مختصر format\n\
"));
      oputs (_("\
      --lookup\n\
         attempt مقصد canonicalize hostnames بذریعہ DNS\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      لکھو (_("\
\n\
A lightweight 'finger' پروگرام;  print user information.\n\
The utmp file will be %s.\n\
"), UTMP_FILE);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحیح optc;
  جب ((optc = getopt_long (argc, argv, "sfwiqbhlp", longopts, تہی))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 's':
          do_short_format = true;
          توڑو;

        حالت 'l':
          do_short_format = false;
          توڑو;

        حالت 'f':
          include_heading = false;
          توڑو;

        حالت 'w':
          include_fullname = false;
          توڑو;

        حالت 'i':
          include_fullname = false;
#اگر_تعریف_شدہ HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#اگر_ختم
          توڑو;

        حالت 'q':
          include_fullname = false;
#اگر_تعریف_شدہ HAVE_STRUCT_XTMP_UT_HOST
          include_where = false;
#اگر_ختم
          include_idle = false;
          توڑو;

        حالت 'h':
          include_project = false;
          توڑو;

        حالت 'p':
          include_plan = false;
          توڑو;

        حالت 'b':
          include_home_and_shell = false;
          توڑو;

        حالت LOOKUP_OPTION:
          do_lookup = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  صحیح n_users = argc - optind;

  اگر (!do_short_format && n_users == 0)
    {
      error (0, 0, _("no username specified; at کم_سے_کم one must be\
 specified when using -l"));
      usage (EXIT_FAILURE);
    }

  اگر (do_short_format)
    short_pinky (UTMP_FILE, n_users, argv + optind);
  ورنہ
    long_pinky (n_users, argv + optind);

  واپس EXIT_SUCCESS;
}
