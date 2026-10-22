/* date - print or set the system date and time
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

   David MacKenzie <djm@gnu.ai.mit.edu> */

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#إذا HAVE_LANGINFO_CODESET
# include <langinfo.h>
#جے_انت

#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "parse-datetime.h"
#ضمن "posixtm.h"
#ضمن "quote.h"
#ضمن "show-date.h"
#ضمن "stat-time.h"
#ضمن "xsetenv.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "date"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

تعداد ویلا_spec
{
  /* Display only the date.  */
  ویلا_SPEC_DATE,
  /* Display date, hours, minutes, and seconds.  */
  ویلا_SPEC_SECONDS,
  /* Similar, but display nanoseconds. */
  ویلا_SPEC_NS,

  /* Put these last, since they aren't valid for --rfc-3339.  */

  /* Display date and hour.  */
  ویلا_SPEC_HOURS,
  /* Display date, hours, and minutes.  */
  ویلا_SPEC_MINUTES
};

ساكن حرف ثابت *ثابت ویلا_spec_string[] =
{
  /* Put "hours" and "minutes" first, since they aren't valid for
     --rfc-3339.  */
  "hours", "minutes",
  "date", "seconds", "ns", عدم
};
ساكن تعداد ویلا_spec ثابت ویلا_spec_2[] =
{
  ویلا_SPEC_HOURS, ویلا_SPEC_MINUTES,
  ویلا_SPEC_DATE, ویلا_SPEC_SECONDS, ویلا_SPEC_NS
};
ARGMATCH_VERIFY (ویلا_spec_string, ویلا_spec_2);

/* A format suitable for Internet RFCs 5322, 2822, and 822.  */
ساكن حرف ثابت rfc_email_format[] = "%a, %d %b %Y %H:%M:%S %z";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEBUG_DATE_PARSING_OPTION = CHAR_MAX + 1,
  RESOLUTION_OPTION,
  RFC_3339_OPTION
};

ساكن حرف ثابت short_options[] = "d:f:I::r:Rs:u";

ساكن بنية option ثابت long_options[] =
{
  {"date", required_دلیل, عدم, 'd'},
  {"debug", no_دلیل, عدم, DEBUG_DATE_PARSING_OPTION},
  {"file", required_دلیل, عدم, 'f'},
  {"iso-8601", optional_دلیل, عدم, 'I'},
  {"reference", required_دلیل, عدم, 'r'},
  {"resolution", no_دلیل, عدم, RESOLUTION_OPTION},
  {"rfc-email", no_دلیل, عدم, 'R'},
  {"rfc-822", no_دلیل, عدم, 'R'},  /* Deprecated.  */
  {"rfc-2822", no_دلیل, عدم, 'R'},  /* Deprecated.  */
  {"rfc-3339", required_دلیل, عدم, RFC_3339_OPTION},
  {"set", required_دلیل, عدم, 's'},
  {"uct", no_دلیل, عدم, 'u'},  /* Deprecated.  */
  {"utc", no_دلیل, عدم, 'u'},
  {"universal", no_دلیل, عدم, 'u'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* flags for parse_datetime2 */
ساكن بنانشانی صحيح parse_datetime_flags;

#جے_تعریف_شدہ _DATE_FMT
# define DATE_FMT_LANGINFO() nl_langinfo (_DATE_FMT)
#وإلا
# define DATE_FMT_LANGINFO() ""
#جے_انت

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s [OPTION]... [+FORMAT]\n\
  or:  %s [OPTION]... MMDDhhmm[[CC]YY][.ss]\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Display date and ویلا وچ the given FORMAT.\n\
With -s, or with MMDDhhmm[[CC]YY][.ss], سیٹ_2 the date and ویلا پہلا.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --date=STRING\n\
         display ویلا described by STRING, not 'now'\n\
"));
      oputs (_("\
      --debug\n\
         annotate the parsed date,\n\
         and warn about questionable usage منزل standard error\n\
"));
      oputs (_("\
  -f, --file=DATEFILE\n\
         like --date; once لكل each سطر دا DATEFILE;\n\
         إذا DATEFILE is -, اقرأ ناں from standard داخلہ\n\
"));
      oputs (_("\
  -I[FMT], --iso-8601[=FMT]\n\
         output date/ویلا وچ ISO 8601 format.\n\
         FMT='date' (افتراضي), 'hours', 'minutes', 'seconds', or 'ns'\n\
         لكل date and ویلا منزل the indicated precision.\n\
         Example: 2006-08-14T02:34:56-06:00\n\
"));
      oputs (_("\
      --resolution\n\
         output the دستیاب resolution دا timestamps.\n\
         Example: 0.000000001\n\
"));
      oputs (_("\
  -R, --rfc-email\n\
         output date and ویلا وچ RFC 5322 format.\n\
         Example: Mon, 14 Aug 2006 02:34:56 +0000\n\
"));
      oputs (_("\
      --rfc-3339=FMT\n\
         output date/ویلا وچ RFC 3339 format.\n\
         FMT='date', 'seconds', or 'ns'\n\
         لكل date and ویلا منزل the indicated precision.\n\
         Example: 2006-08-14 02:34:56-06:00\n\
"));
      oputs (_("\
  -r, --reference=FILE\n\
         display the آخری modification ویلا دا FILE\n\
"));
      oputs (_("\
  -s, --سیٹ_2=STRING\n\
         سیٹ_2 ویلا described by STRING\n\
"));
      oputs (_("\
  -u, --utc, --universal\n\
         print or سیٹ_2 Coordinated Universal ویلا_2 (UTC)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
All options that specify the date منزل display are mutually exclusive.\n\
I.e.: --date, --file, --reference, --resolution.\n\
"), stdout);
      fputs (_("\
\n\
FORMAT controls the output.  Interpreted sequences are:\n\
\n\
"), stdout);
      fputs (_("\
FORMAT  Example    Description\n\
\n\
"), stdout);
      fputs (_("\
  %%    %           a literal %\n\
"), stdout);
      fputs (_("\
  %n    \\n          a newline\n\
"), stdout);
      fputs (_("\
  %t    \\t          a tab\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %C    20          century; like %Y, except omit آخری two digits\n\
"), stdout);
      fputs (_("\
  %y    99          year (آخری two digits; 00..99)\n\
"), stdout);
      fputs (_("\
  %Y    1999        year\n\
"), stdout);
      fputs (_("\
  %g    99          year دا ISO week نمبر (آخری 2 digits; 00-99); see %G\n\
"), stdout);
      fputs (_("\
  %G    1999        year دا ISO week نمبر; normally useful only with %V\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %a    Sun         locale's abbreviated weekday name\n\
"), stdout);
      fputs (_("\
  %A    Sunday      locale's بھریا weekday name\n\
"), stdout);
      fputs (_("\
  %b    Mar         locale's abbreviated month name\n\
"), stdout);
      fputs (_("\
  %h    Mar         same as %b\n\
"), stdout);
      fputs (_("\
  %B    March       locale's بھریا month name\n\
"), stdout);
      fputs (_("\
  %m    12          month (01..12)\n\
"), stdout);
      fputs (_("\
  %d    31          day دا month (01..31)\n\
"), stdout);
      fputs (_("\
  %e     1          day دا month, space padded; same as %_d\n\
"), stdout);
      fputs (_("\
  %D    12/31/99    date (ambiguous); same as %m/%d/%y\n\
"), stdout);
      fputs (_("\
  %F    1999-12-31  بھریا date; like %+4Y-%m-%d\n\
"), stdout);
      fputs (_("\
  %x    12/31/99    locale's date (can be ambiguous)\n\
"), stdout);
      fputs (_("\
  %c    ...         locale's date and ویلا (e.g., Thu Mar  3 23:05 2005)\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %H    23          hour (00..23)\n\
"), stdout);
      fputs (_("\
  %I    01          hour (01..12)\n\
"), stdout);
      fputs (_("\
  %k     8          hour, space padded ( 0..23); same as %_H\n\
"), stdout);
      fputs (_("\
  %l     9          hour, space padded ( 1..12); same as %_I\n\
"), stdout);
      fputs (_("\
  %M    59          minute (00..59)\n\
"), stdout);
      fputs (_("\
  %S    60          دوجا (00..60)\n\
"), stdout);
      fputs (_("\
  %N    123456789   nanoseconds (000000000..999999999)\n\
"), stdout);
      fputs (_("\
  %s    1970010100  seconds since the Epoch (1970-01-01 00:00 UTC)\n\
"), stdout);
      fputs (_("\
  %p    PM          locale's equivalent دا AM or PM; blank إذا not معلوم\n\
"), stdout);
      fputs (_("\
  %P    am          like %p, but ہیٹھلا حالة\n\
"), stdout);
      fputs (_("\
  %r    1:11:04 PM  locale's 12-hour clock ویلا\n\
"), stdout);
      fputs (_("\
  %R    23:59       24-hour hour and minute; same as %H:%M\n\
"), stdout);
      fputs (_("\
  %T    23:59:59    ویلا; same as %H:%M:%S\n\
"), stdout);
      fputs (_("\
  %X    23:59:59    locale's ویلا representation\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %j    365         day دا year (001..366)\n\
"), stdout);
      fputs (_("\
  %q    4           quarter دا year (1..4)\n\
"), stdout);
      fputs (_("\
  %u    7           day دا week (1..7); 1 is Monday\n\
"), stdout);
      fputs (_("\
  %w    6           day دا week (0..6); 0 is Sunday\n\
"), stdout);
      fputs (_("\
  %U    52          week دا year; Sunday as پہلا day دا week (00..53)\n\
"), stdout);
      fputs (_("\
  %V    52          ISO week نمبر; Monday as پہلا day دا week (01..53)\n\
"), stdout);
      fputs (_("\
  %W    52          week دا year; Monday as پہلا day دا week (00..53)\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %z     +0400      +hhmm numeric ویلا zone\n\
"), stdout);
      fputs (_("\
  %:z    +04:00     +hh:mm numeric ویلا zone\n\
"), stdout);
      fputs (_("\
  %::z   +04:00:00  +hh:mm:ss numeric ویلا zone\n\
"), stdout);
      fputs (_("\
  %:::z  +04        numeric ویلا zone منزل necessary precision; with :\n\
"), stdout);
      fputs (_("\
  %Z     EDT        alphabetic ویلا zone abbreviation\n\
"), stdout);
      fputs (_("\
\n\
By افتراضي, date pads numeric fields with zeroes.\n\
"), stdout);
      fputs (_("\
The following optional flags may follow '%':\n\
\n\
  -  (hyphen) افعل not pad the میدان\n\
  _  (underscore) pad with spaces\n\
  0  (zero) pad with zeros\n\
  +  pad with zeros, and put '+' before future years with >4 digits\n\
  ^  use اتلا حالة إذا possible\n\
  #  use opposite حالة إذا possible\n\
"), stdout);
      fputs (_("\
\n\
After any flags comes an optional میدان چوڑائی, as a decimal نمبر;\n\
then an optional modifier, which is either\n\
E منزل use the locale's alternate representations إذا دستیاب, or\n\
O منزل use the locale's alternate numeric symbols إذا دستیاب.\n\
"), stdout);
      fputs (_("\
\n\
Examples:\n\
Convert seconds since the Epoch (1970-01-01 UTC) منزل a date\n\
  $ date --date='@2147483647'\n\
\n\
Show the ویلا on the west coast دا the US (use tzselect(1) منزل لبھو TZ)\n\
  $ TZ='America/Los_Angeles' date\n\
\n\
Show the local ویلا لكل 9AM اگلا Friday on the west coast دا the US\n\
  $ date --date='TZ=\"America/Los_Angeles\" 09:00 next Fri'\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Yield the number of decimal digits needed to output a time with the
   nanosecond resolution RES, without losing information.  */

ATTRIBUTE_CONST
ساكن صحيح
res_چوڑائی (لما صحيح res)
{
  صحيح i = 9;
  لكل (لما لما صحيح r = 1; (r *= 10) <= res; )
    i--;
  أرجع i;
}

/* Return a newly allocated copy of FORMAT with each "%-N" adjusted to
   be "%9N", "%6N", or whatever other resolution is appropriate for
   the current platform.  If no "%-N" appears, return NULL.  */

ساكن حرف *
adjust_resolution (حرف ثابت *format)
{
  حرف *copy = عدم;

  لكل (حرف ثابت *f = format; *f; f++)
    إذا (f[0] == '%')
      {
        إذا (f[1] == '-' && f[2] == 'N')
          {
            إذا (!copy)
              copy = xstrdup (format);
            copy[f + 1 - format] = '0' + res_چوڑائی (gettime_res ());
            f += 2;
          }
        وإلا
          f += f[1] == '%';
      }

  أرجع copy;
}

/* Set the LC_TIME category of the current locale.
   Return the previous value of the LC_TIME category, as a freshly allocated
   string or null.  */

ساكن حرف *
سیٹ_LC_ویلا (حرف ثابت *locale)
{
  /* It is not sufficient to do
       setlocale (LC_TIME, locale);
     because show_date relies on fprintftime, that uses the Gnulib module
     'localename-unsafe', that looks at the values of the environment variables
     (in order to distinguish the default locale from the C locale on platforms
     like macOS).  */
  حرف ثابت *all = getenv ("LC_ALL");
  إذا (all != عدم && *all != '\0')
    {
      /* Setting LC_TIME when LC_ALL is set would have no effect.  Therefore we
         have to unset LC_ALL and sets its value to all locale categories that
         are relevant for this program.  */
      xsetenv ("LC_CTYPE", all, 1);          /* definitely needed */
      xsetenv ("LC_TIME", all, 1);           /* definitely needed */
      xsetenv ("LC_MESSAGES", all, 1);       /* definitely needed */
      xsetenv ("LC_NUMERIC", all, 1);        /* possibly needed */
      /* xsetenv ("LC_COLLATE", all, 1); */  /* not needed */
      /* xsetenv ("LC_MONETARY", all, 1); */ /* not needed */
      unsetenv ("LC_ALL");
    }

  /* Set LC_TIME as an environment variable.  */
  حرف ثابت *قدر = getenv ("LC_TIME");
  حرف *ret = (قدر == عدم || *قدر == '\0' ? عدم : xstrdup (قدر));
  إذا (locale != عدم)
    xsetenv ("LC_TIME", locale, 1);
  وإلا
    unsetenv ("LC_TIME");

  /* Update the current locale accordingly.  */
  setlocale (LC_ویلا, "");

  أرجع ret;
}

ساكن bool
show_date_helper (حرف ثابت *format, bool use_c_locale,
                  بنية timespec when, timezone_t tz)
{
  إذا (parse_datetime_flags & PARSE_DATETIME_DEBUG)
    error (0, 0, _("output format: %s"), quote (format));

  bool ok;
  إذا (use_c_locale)
    {
      حرف *old_locale_category = سیٹ_LC_ویلا ("C");
      ok = show_date (format, when, tz);
      حرف *new_locale_category = سیٹ_LC_ویلا (old_locale_category);
      حرر (new_locale_category);
      حرر (old_locale_category);
    }
  وإلا
    ok = show_date (format, when, tz);

  حرف_لکھو ('\n');
  أرجع ok;
}

/* Parse each line in INPUT_FILENAME as with --date and display each
   resulting time and date.  If the file cannot be opened, tell why
   then exit.  Issue a diagnostic for any lines that cannot be parsed.
   Return true if successful.  */

ساكن bool
batch_convert (حرف ثابت *داخلہ_filename,
               حرف ثابت *format, bool format_وچ_c_locale,
               timezone_t tz, حرف ثابت *tzstring)
{
  FILE *وچ_دھارا;

  إذا (streq (داخلہ_filename, "-"))
    {
      داخلہ_filename = _("standard input");
      وچ_دھارا = stdin;
    }
  وإلا
    {
      وچ_دھارا = fopen (داخلہ_filename, "r");
      إذا (وچ_دھارا == عدم)
        error (EXIT_FAILURE, errno, "%s", quotef (داخلہ_filename));
    }

  حرف *سطر = عدم;
  size_t buflen = 0;
  bool ok = true;
  طالما (true)
    {
      ssize_t سطر_لمائی = getline (&سطر, &buflen, وچ_دھارا);
      إذا (سطر_لمائی < 0)
        {
          إذا (ferror (وچ_دھارا))
            error (EXIT_FAILURE, errno, _("%s: read error"),
                   quotef (داخلہ_filename));
          اكسر;
        }

      بنية timespec when;
      إذا (! parse_datetime2 (&when, سطر, عدم,
                             parse_datetime_flags, tz, tzstring))
        {
          إذا (سطر[سطر_لمائی - 1] == '\n')
            سطر[سطر_لمائی - 1] = '\0';
          error (0, 0, _("invalid date %s"), quote (سطر));
          ok = false;
        }
      وإلا
        {
          ok &= show_date_helper (format, format_وچ_c_locale, when, tz);
        }

      إذا (ferror (stdout))
        write_error ();
    }

  إذا (fclose (وچ_دھارا) == EOF)
    error (EXIT_FAILURE, errno, "%s", quotef (داخلہ_filename));

  حرر (سطر);

  أرجع ok;
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  حرف ثابت *datestr = عدم;
  حرف ثابت *سیٹ_datestr = عدم;
  bool سیٹ_date = false;
  حرف ثابت *format = عدم;
  bool format_وچ_c_locale = false;
  bool get_resolution = false;
  حرف *batch_file = عدم;
  حرف *reference = عدم;
  bool discarded_datestr = false;
  bool discarded_سیٹ_datestr = false;
  حرف ثابت *tzstring = عدم;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحيح optc;
  طالما ((optc = getopt_long (argc, argv, short_options, long_options, عدم))
         != -1)
    {
      بدل (optc)
        {
        حالة 'd':
          إذا (datestr)
            discarded_datestr = true;
          datestr = optarg;
          اكسر;
        حالة DEBUG_DATE_PARSING_OPTION:
          parse_datetime_flags |= PARSE_DATETIME_DEBUG;
          اكسر;
        حالة 'f':
          batch_file = optarg;
          اكسر;
        حالة RESOLUTION_OPTION:
          get_resolution = true;
          اكسر;
        حالة RFC_3339_OPTION:
          {
            ساكن حرف ثابت rfc_3339_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%d %H:%M:%S%:z",
                "%Y-%m-%d %H:%M:%S.%N%:z"
              };
            تعداد ویلا_spec i =
              XARGMATCH ("--rfc-3339", optarg,
                         ویلا_spec_string + 2, ویلا_spec_2 + 2);
            format = rfc_3339_format[i];
            format_وچ_c_locale = true;
            اكسر;
          }
        حالة 'I':
          {
            ساكن حرف ثابت iso_8601_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%dT%H:%M:%S%:z",
                "%Y-%m-%dT%H:%M:%S,%N%:z",
                "%Y-%m-%dT%H%:z",
                "%Y-%m-%dT%H:%M%:z"
              };
            تعداد ویلا_spec i =
              (optarg
               ? XARGMATCH ("--iso-8601", optarg, ویلا_spec_string, ویلا_spec_2)
               : ویلا_SPEC_DATE);
            format = iso_8601_format[i];
            format_وچ_c_locale = true;
            اكسر;
          }
        حالة 'r':
          reference = optarg;
          اكسر;
        حالة 'R':
          format = rfc_email_format;
          format_وچ_c_locale = true;
          اكسر;
        حالة 's':
          إذا (سیٹ_datestr)
            discarded_سیٹ_datestr = true;
          سیٹ_datestr = optarg;
          سیٹ_date = true;
          اكسر;
        حالة 'u':
          tzstring = "UTC0";
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  صحيح option_specified_date = (!!datestr + !!batch_file + !!reference
                               + get_resolution);

  إذا (option_specified_date > 1)
    {
      error (0, 0,
        _("the options to specify dates for printing are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  إذا (سیٹ_date && option_specified_date)
    {
      error (0, 0,
          _("the options to print and set the time may not be used together"));
      usage (EXIT_FAILURE);
    }

  إذا (discarded_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -d options"));

  إذا (discarded_سیٹ_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -s options"));

  إذا (optind < argc)
    {
      إذا (optind + 1 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          usage (EXIT_FAILURE);
        }

      إذا (argv[optind][0] == '+')
        {
          إذا (format)
            error (EXIT_FAILURE, 0, _("multiple output formats specified"));
          format = argv[optind++] + 1;
        }
      وإلا إذا (سیٹ_date || option_specified_date)
        {
          error (0, 0,
                 _("the argument %s lacks a leading '+';\n"
                   "when using an option to specify date(s), any non-option\n"
                   "argument must be a format string beginning with '+'"),
                 quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  إذا (!format)
    {
      إذا (get_resolution)
        format = "%s.%N";
      وإلا
        {
          format = DATE_FMT_LANGINFO ();

          /* Do not wrap the following literal format string with _(...).
             For example, suppose LC_ALL is unset, LC_TIME=POSIX,
             and LANG="ko_KR".  In that case, POSIX says that LC_TIME
             determines the format and contents of date and time strings
             written by date, which means "date" must generate output
             using the POSIX locale; but adding _() would cause "date"
             to use a Korean translation of the format.  */
          إذا (! *format)
            format = "%a %b %e %H:%M:%S %Z %Y";
        }
    }

  حرف *format_copy = adjust_resolution (format);
  حرف ثابت *format_res = format_copy ? format_copy : format;
  إذا (!tzstring)
    tzstring = getenv ("TZ");
  timezone_t tz = tzalloc (tzstring);

  bool ok = true;
  إذا (batch_file != عدم)
    ok = batch_convert (batch_file, format_res, format_وچ_c_locale,
                        tz, tzstring);
  وإلا
    {
      bool valid_date = true;
      بنية timespec when;

      إذا (!option_specified_date && !سیٹ_date)
        {
          إذا (optind < argc)
            {
              /* Prepare to set system clock to the specified date/time
                 given in the POSIX-format.  */
              سیٹ_date = true;
              datestr = argv[optind];
              valid_date = posixtime (&when.tv_sec,
                                      datestr,
                                      (PDS_TRAILING_YEAR
                                       | PDS_CENTURY | PDS_SECONDS));
              when.tv_nsec = 0; /* FIXME: posixtime should set this.  */
            }
          وإلا
            {
              /* Prepare to print the current date/time.  */
              gettime (&when);
            }
        }
      وإلا
        {
          /* (option_specified_date || set_date) */
          إذا (reference != عدم)
            {
              بنية stat refstats;
              إذا (stat (reference, &refstats) != 0)
                error (EXIT_FAILURE, errno, "%s", quotef (reference));
              when = get_stat_mtime (&refstats);
            }
          وإلا إذا (get_resolution)
            {
              لما صحيح res = gettime_res ();
              when.tv_sec = res / TIMESPEC_HZ;
              when.tv_nsec = res % TIMESPEC_HZ;
            }
          وإلا
            {
              إذا (سیٹ_datestr)
                datestr = سیٹ_datestr;
              valid_date = parse_datetime2 (&when, datestr, عدم,
                                            parse_datetime_flags,
                                            tz, tzstring);
            }
        }

      إذا (! valid_date)
        error (EXIT_FAILURE, 0, _("invalid date %s"), quote (datestr));

      إذا (سیٹ_date)
        {
          /* Set the system clock to the specified date, then regardless of
             the success of that operation, format and print that date.  */
          إذا (settime (&when) != 0)
            {
              error (0, errno, _("cannot set date"));
              ok = false;
            }
        }

      ok &= show_date_helper (format_res, format_وچ_c_locale, when, tz);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
