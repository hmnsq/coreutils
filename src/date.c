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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>
#যদি HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "argmatch.h"
#অন্তর্ভাৱ "parse-datetime.h"
#অন্তর্ভাৱ "posixtm.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "show-date.h"
#অন্তর্ভাৱ "stat-time.h"
#অন্তর্ভাৱ "xsetenv.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "date"

#define AUTHORS proper_name ("David MacKenzie")

গণনা Time_spec
{
  /* Display only the date.  */
  TIME_SPEC_DATE,
  /* Display date, hours, minutes, and seconds.  */
  TIME_SPEC_SECONDS,
  /* Similar, but display nanoseconds. */
  TIME_SPEC_NS,

  /* Put these last, since they aren't valid for --rfc-3339.  */

  /* Display date and hour.  */
  TIME_SPEC_HOURS,
  /* Display date, hours, and minutes.  */
  TIME_SPEC_MINUTES
};

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ time_spec_string[] =
{
  /* Put "hours" and "minutes" first, since they aren't valid for
     --rfc-3339.  */
  "hours", "minutes",
  "date", "seconds", "ns", রিক্ত
};
স্থির গণনা Time_spec ধ্রুৱ time_spec[] =
{
  TIME_SPEC_HOURS, TIME_SPEC_MINUTES,
  TIME_SPEC_DATE, TIME_SPEC_SECONDS, TIME_SPEC_NS
};
ARGMATCH_VERIFY (time_spec_string, time_spec);

/* A format suitable for Internet RFCs 5322, 2822, and 822.  */
স্থির ৱর্ণ ধ্রুৱ rfc_email_format[] = "%a, %d %b %Y %H:%M:%S %z";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  DEBUG_DATE_PARSING_OPTION = CHAR_MAX + 1,
  RESOLUTION_OPTION,
  RFC_3339_OPTION
};

স্থির ৱর্ণ ধ্রুৱ short_options[] = "d:f:I::r:Rs:u";

স্থির সংরচনা option ধ্রুৱ long_options[] =
{
  {"date", required_argument, রিক্ত, 'd'},
  {"debug", no_argument, রিক্ত, DEBUG_DATE_PARSING_OPTION},
  {"file", required_argument, রিক্ত, 'f'},
  {"iso-8601", optional_argument, রিক্ত, 'I'},
  {"reference", required_argument, রিক্ত, 'r'},
  {"resolution", no_argument, রিক্ত, RESOLUTION_OPTION},
  {"rfc-email", no_argument, রিক্ত, 'R'},
  {"rfc-822", no_argument, রিক্ত, 'R'},  /* Deprecated.  */
  {"rfc-2822", no_argument, রিক্ত, 'R'},  /* Deprecated.  */
  {"rfc-3339", required_argument, রিক্ত, RFC_3339_OPTION},
  {"set", required_argument, রিক্ত, 's'},
  {"uct", no_argument, রিক্ত, 'u'},  /* Deprecated.  */
  {"utc", no_argument, রিক্ত, 'u'},
  {"universal", no_argument, রিক্ত, 'u'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* flags for parse_datetime2 */
স্থির unsigned পূর্ণাঙ্ক parse_datetime_flags;

#ifdef _DATE_FMT
# define DATE_FMT_LANGINFO() nl_langinfo (_DATE_FMT)
#অন্যথা
# define DATE_FMT_LANGINFO() ""
#endif

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [+FORMAT]\n\
  or:  %s [OPTION]... MMDDhhmm[[CC]YY][.ss]\n\
"),
              program_name, program_name);
      fputs (_("\
Display date and time in the given FORMAT.\n\
With -s, or with MMDDhhmm[[CC]YY][.ss], set the date and time first.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --date=STRING\n\
         display time described by STRING, not 'now'\n\
"));
      oputs (_("\
      --debug\n\
         annotate the parsed date,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
  -f, --file=DATEFILE\n\
         like --date; once বাবে each সাৰি of DATEFILE;\n\
         যদি DATEFILE is -, পঠ names from standard প্ৰবেশ\n\
"));
      oputs (_("\
  -I[FMT], --iso-8601[=FMT]\n\
         output date/time in ISO 8601 format.\n\
         FMT='date' (স্ৱভাৱ), 'hours', 'minutes', 'seconds', or 'ns'\n\
         বাবে date and time to the indicated precision.\n\
         Example: 2006-08-14T02:34:56-06:00\n\
"));
      oputs (_("\
      --resolution\n\
         output the available resolution of timestamps.\n\
         Example: 0.000000001\n\
"));
      oputs (_("\
  -R, --rfc-email\n\
         output date and time in RFC 5322 format.\n\
         Example: Mon, 14 Aug 2006 02:34:56 +0000\n\
"));
      oputs (_("\
      --rfc-3339=FMT\n\
         output date/time in RFC 3339 format.\n\
         FMT='date', 'seconds', or 'ns'\n\
         বাবে date and time to the indicated precision.\n\
         Example: 2006-08-14 02:34:56-06:00\n\
"));
      oputs (_("\
  -r, --reference=FILE\n\
         display the last modification time of FILE\n\
"));
      oputs (_("\
  -s, --set=STRING\n\
         set time described by STRING\n\
"));
      oputs (_("\
  -u, --utc, --universal\n\
         print or set Coordinated Universal Time (UTC)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
All options that specify the date to display are mutually exclusive.\n\
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
  %C    20          century; like %Y, except omit last two digits\n\
"), stdout);
      fputs (_("\
  %y    99          year (last two digits; 00..99)\n\
"), stdout);
      fputs (_("\
  %Y    1999        year\n\
"), stdout);
      fputs (_("\
  %g    99          year of ISO week সংখ্যা (last 2 digits; 00-99); see %G\n\
"), stdout);
      fputs (_("\
  %G    1999        year of ISO week সংখ্যা; normally useful only with %V\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %a    Sun         locale's abbreviated weekday name\n\
"), stdout);
      fputs (_("\
  %A    Sunday      locale's full weekday name\n\
"), stdout);
      fputs (_("\
  %b    Mar         locale's abbreviated month name\n\
"), stdout);
      fputs (_("\
  %h    Mar         same as %b\n\
"), stdout);
      fputs (_("\
  %B    March       locale's full month name\n\
"), stdout);
      fputs (_("\
  %m    12          month (01..12)\n\
"), stdout);
      fputs (_("\
  %d    31          day of month (01..31)\n\
"), stdout);
      fputs (_("\
  %e     1          day of month, space padded; same as %_d\n\
"), stdout);
      fputs (_("\
  %D    12/31/99    date (ambiguous); same as %m/%d/%y\n\
"), stdout);
      fputs (_("\
  %F    1999-12-31  full date; like %+4Y-%m-%d\n\
"), stdout);
      fputs (_("\
  %x    12/31/99    locale's date (can be ambiguous)\n\
"), stdout);
      fputs (_("\
  %c    ...         locale's date and time (e.g., Thu Mar  3 23:05 2005)\n\
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
  %S    60          second (00..60)\n\
"), stdout);
      fputs (_("\
  %N    123456789   nanoseconds (000000000..999999999)\n\
"), stdout);
      fputs (_("\
  %s    1970010100  seconds since the Epoch (1970-01-01 00:00 UTC)\n\
"), stdout);
      fputs (_("\
  %p    PM          locale's equivalent of AM or PM; blank যদি not known\n\
"), stdout);
      fputs (_("\
  %P    am          like %p, but lower পক্ষ\n\
"), stdout);
      fputs (_("\
  %r    1:11:04 PM  locale's 12-hour clock time\n\
"), stdout);
      fputs (_("\
  %R    23:59       24-hour hour and minute; same as %H:%M\n\
"), stdout);
      fputs (_("\
  %T    23:59:59    time; same as %H:%M:%S\n\
"), stdout);
      fputs (_("\
  %X    23:59:59    locale's time representation\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %j    365         day of year (001..366)\n\
"), stdout);
      fputs (_("\
  %q    4           quarter of year (1..4)\n\
"), stdout);
      fputs (_("\
  %u    7           day of week (1..7); 1 is Monday\n\
"), stdout);
      fputs (_("\
  %w    6           day of week (0..6); 0 is Sunday\n\
"), stdout);
      fputs (_("\
  %U    52          week of year; Sunday as first day of week (00..53)\n\
"), stdout);
      fputs (_("\
  %V    52          ISO week সংখ্যা; Monday as first day of week (01..53)\n\
"), stdout);
      fputs (_("\
  %W    52          week of year; Monday as first day of week (00..53)\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %z     +0400      +hhmm numeric time zone\n\
"), stdout);
      fputs (_("\
  %:z    +04:00     +hh:mm numeric time zone\n\
"), stdout);
      fputs (_("\
  %::z   +04:00:00  +hh:mm:ss numeric time zone\n\
"), stdout);
      fputs (_("\
  %:::z  +04        numeric time zone to necessary precision; with :\n\
"), stdout);
      fputs (_("\
  %Z     EDT        alphabetic time zone abbreviation\n\
"), stdout);
      fputs (_("\
\n\
By স্ৱভাৱ, date pads numeric fields with zeroes.\n\
"), stdout);
      fputs (_("\
The following optional flags may follow '%':\n\
\n\
  -  (hyphen) কৰা not pad the field\n\
  _  (underscore) pad with spaces\n\
  0  (zero) pad with zeros\n\
  +  pad with zeros, and put '+' before future years with >4 digits\n\
  ^  use upper পক্ষ যদি possible\n\
  #  use opposite পক্ষ যদি possible\n\
"), stdout);
      fputs (_("\
\n\
After any flags comes an optional field width, as a decimal সংখ্যা;\n\
then an optional modifier, which is either\n\
E to use the locale's alternate representations যদি available, or\n\
O to use the locale's alternate numeric symbols যদি available.\n\
"), stdout);
      fputs (_("\
\n\
Examples:\n\
Convert seconds since the Epoch (1970-01-01 UTC) to a date\n\
  $ date --date='@2147483647'\n\
\n\
Show the time on the west coast of the US (use tzselect(1) to find TZ)\n\
  $ TZ='America/Los_Angeles' date\n\
\n\
Show the local time বাবে 9AM next Friday on the west coast of the US\n\
  $ date --date='TZ=\"America/Los_Angeles\" 09:00 next Fri'\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Yield the number of decimal digits needed to output a time with the
   nanosecond resolution RES, without losing information.  */

ATTRIBUTE_CONST
স্থির পূর্ণাঙ্ক
res_width (long পূর্ণাঙ্ক res)
{
  পূর্ণাঙ্ক i = 9;
  বাবে (long long পূর্ণাঙ্ক r = 1; (r *= 10) <= res; )
    i--;
  ঘূৰাই i;
}

/* Return a newly allocated copy of FORMAT with each "%-N" adjusted to
   be "%9N", "%6N", or whatever other resolution is appropriate for
   the current platform.  If no "%-N" appears, return NULL.  */

স্থির ৱর্ণ *
adjust_resolution (ৱর্ণ ধ্রুৱ *format)
{
  ৱর্ণ *copy = রিক্ত;

  বাবে (ৱর্ণ ধ্রুৱ *f = format; *f; f++)
    যদি (f[0] == '%')
      {
        যদি (f[1] == '-' && f[2] == 'N')
          {
            যদি (!copy)
              copy = xstrdup (format);
            copy[f + 1 - format] = '0' + res_width (gettime_res ());
            f += 2;
          }
        অন্যথা
          f += f[1] == '%';
      }

  ঘূৰাই copy;
}

/* Set the LC_TIME category of the current locale.
   Return the previous value of the LC_TIME category, as a freshly allocated
   string or null.  */

স্থির ৱর্ণ *
set_LC_TIME (ৱর্ণ ধ্রুৱ *locale)
{
  /* It is not sufficient to do
       setlocale (LC_TIME, locale);
     because show_date relies on fprintftime, that uses the Gnulib module
     'localename-unsafe', that looks at the values of the environment variables
     (in order to distinguish the default locale from the C locale on platforms
     like macOS).  */
  ৱর্ণ ধ্রুৱ *all = getenv ("LC_ALL");
  যদি (all != রিক্ত && *all != '\0')
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
  ৱর্ণ ধ্রুৱ *মান = getenv ("LC_TIME");
  ৱর্ণ *ret = (মান == রিক্ত || *মান == '\0' ? রিক্ত : xstrdup (মান));
  যদি (locale != রিক্ত)
    xsetenv ("LC_TIME", locale, 1);
  অন্যথা
    unsetenv ("LC_TIME");

  /* Update the current locale accordingly.  */
  setlocale (LC_TIME, "");

  ঘূৰাই ret;
}

স্থির bool
show_date_helper (ৱর্ণ ধ্রুৱ *format, bool use_c_locale,
                  সংরচনা timespec when, timezone_t tz)
{
  যদি (parse_datetime_flags & PARSE_DATETIME_DEBUG)
    error (0, 0, _("output format: %s"), quote (format));

  bool ok;
  যদি (use_c_locale)
    {
      ৱর্ণ *old_locale_category = set_LC_TIME ("C");
      ok = show_date (format, when, tz);
      ৱর্ণ *new_locale_category = set_LC_TIME (old_locale_category);
      মুক্ত (new_locale_category);
      মুক্ত (old_locale_category);
    }
  অন্যথা
    ok = show_date (format, when, tz);

  putchar ('\n');
  ঘূৰাই ok;
}

/* Parse each line in INPUT_FILENAME as with --date and display each
   resulting time and date.  If the file cannot be opened, tell why
   then exit.  Issue a diagnostic for any lines that cannot be parsed.
   Return true if successful.  */

স্থির bool
batch_convert (ৱর্ণ ধ্রুৱ *প্ৰবেশ_filename,
               ৱর্ণ ধ্রুৱ *format, bool format_in_c_locale,
               timezone_t tz, ৱর্ণ ধ্রুৱ *tzstring)
{
  FILE *in_stream;

  যদি (streq (প্ৰবেশ_filename, "-"))
    {
      প্ৰবেশ_filename = _("standard input");
      in_stream = stdin;
    }
  অন্যথা
    {
      in_stream = fopen (প্ৰবেশ_filename, "r");
      যদি (in_stream == রিক্ত)
        error (EXIT_FAILURE, errno, "%s", quotef (প্ৰবেশ_filename));
    }

  ৱর্ণ *সাৰি = রিক্ত;
  size_t buflen = 0;
  bool ok = true;
  যেতিয়ালৈ (true)
    {
      ssize_t সাৰি_length = getline (&সাৰি, &buflen, in_stream);
      যদি (সাৰি_length < 0)
        {
          যদি (ferror (in_stream))
            error (EXIT_FAILURE, errno, _("%s: read error"),
                   quotef (প্ৰবেশ_filename));
          ভঙ্গ;
        }

      সংরচনা timespec when;
      যদি (! parse_datetime2 (&when, সাৰি, রিক্ত,
                             parse_datetime_flags, tz, tzstring))
        {
          যদি (সাৰি[সাৰি_length - 1] == '\n')
            সাৰি[সাৰি_length - 1] = '\0';
          error (0, 0, _("invalid date %s"), quote (সাৰি));
          ok = false;
        }
      অন্যথা
        {
          ok &= show_date_helper (format, format_in_c_locale, when, tz);
        }

      যদি (ferror (stdout))
        write_error ();
    }

  যদি (fclose (in_stream) == EOF)
    error (EXIT_FAILURE, errno, "%s", quotef (প্ৰবেশ_filename));

  মুক্ত (সাৰি);

  ঘূৰাই ok;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  ৱর্ণ ধ্রুৱ *datestr = রিক্ত;
  ৱর্ণ ধ্রুৱ *set_datestr = রিক্ত;
  bool set_date = false;
  ৱর্ণ ধ্রুৱ *format = রিক্ত;
  bool format_in_c_locale = false;
  bool get_resolution = false;
  ৱর্ণ *batch_file = রিক্ত;
  ৱর্ণ *reference = রিক্ত;
  bool discarded_datestr = false;
  bool discarded_set_datestr = false;
  ৱর্ণ ধ্রুৱ *tzstring = রিক্ত;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  পূর্ণাঙ্ক optc;
  যেতিয়ালৈ ((optc = getopt_long (argc, argv, short_options, long_options, রিক্ত))
         != -1)
    {
      ৱিকল্প (optc)
        {
        পক্ষ 'd':
          যদি (datestr)
            discarded_datestr = true;
          datestr = optarg;
          ভঙ্গ;
        পক্ষ DEBUG_DATE_PARSING_OPTION:
          parse_datetime_flags |= PARSE_DATETIME_DEBUG;
          ভঙ্গ;
        পক্ষ 'f':
          batch_file = optarg;
          ভঙ্গ;
        পক্ষ RESOLUTION_OPTION:
          get_resolution = true;
          ভঙ্গ;
        পক্ষ RFC_3339_OPTION:
          {
            স্থির ৱর্ণ ধ্রুৱ rfc_3339_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%d %H:%M:%S%:z",
                "%Y-%m-%d %H:%M:%S.%N%:z"
              };
            গণনা Time_spec i =
              XARGMATCH ("--rfc-3339", optarg,
                         time_spec_string + 2, time_spec + 2);
            format = rfc_3339_format[i];
            format_in_c_locale = true;
            ভঙ্গ;
          }
        পক্ষ 'I':
          {
            স্থির ৱর্ণ ধ্রুৱ iso_8601_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%dT%H:%M:%S%:z",
                "%Y-%m-%dT%H:%M:%S,%N%:z",
                "%Y-%m-%dT%H%:z",
                "%Y-%m-%dT%H:%M%:z"
              };
            গণনা Time_spec i =
              (optarg
               ? XARGMATCH ("--iso-8601", optarg, time_spec_string, time_spec)
               : TIME_SPEC_DATE);
            format = iso_8601_format[i];
            format_in_c_locale = true;
            ভঙ্গ;
          }
        পক্ষ 'r':
          reference = optarg;
          ভঙ্গ;
        পক্ষ 'R':
          format = rfc_email_format;
          format_in_c_locale = true;
          ভঙ্গ;
        পক্ষ 's':
          যদি (set_datestr)
            discarded_set_datestr = true;
          set_datestr = optarg;
          set_date = true;
          ভঙ্গ;
        পক্ষ 'u':
          tzstring = "UTC0";
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  পূর্ণাঙ্ক option_specified_date = (!!datestr + !!batch_file + !!reference
                               + get_resolution);

  যদি (option_specified_date > 1)
    {
      error (0, 0,
        _("the options to specify dates for printing are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  যদি (set_date && option_specified_date)
    {
      error (0, 0,
          _("the options to print and set the time may not be used together"));
      usage (EXIT_FAILURE);
    }

  যদি (discarded_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -d options"));

  যদি (discarded_set_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -s options"));

  যদি (optind < argc)
    {
      যদি (optind + 1 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          usage (EXIT_FAILURE);
        }

      যদি (argv[optind][0] == '+')
        {
          যদি (format)
            error (EXIT_FAILURE, 0, _("multiple output formats specified"));
          format = argv[optind++] + 1;
        }
      অন্যথা যদি (set_date || option_specified_date)
        {
          error (0, 0,
                 _("the argument %s lacks a leading '+';\n"
                   "when using an option to specify date(s), any non-option\n"
                   "argument must be a format string beginning with '+'"),
                 quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  যদি (!format)
    {
      যদি (get_resolution)
        format = "%s.%N";
      অন্যথা
        {
          format = DATE_FMT_LANGINFO ();

          /* Do not wrap the following literal format string with _(...).
             For example, suppose LC_ALL is unset, LC_TIME=POSIX,
             and LANG="ko_KR".  In that case, POSIX says that LC_TIME
             determines the format and contents of date and time strings
             written by date, which means "date" must generate output
             using the POSIX locale; but adding _() would cause "date"
             to use a Korean translation of the format.  */
          যদি (! *format)
            format = "%a %b %e %H:%M:%S %Z %Y";
        }
    }

  ৱর্ণ *format_copy = adjust_resolution (format);
  ৱর্ণ ধ্রুৱ *format_res = format_copy ? format_copy : format;
  যদি (!tzstring)
    tzstring = getenv ("TZ");
  timezone_t tz = tzalloc (tzstring);

  bool ok = true;
  যদি (batch_file != রিক্ত)
    ok = batch_convert (batch_file, format_res, format_in_c_locale,
                        tz, tzstring);
  অন্যথা
    {
      bool valid_date = true;
      সংরচনা timespec when;

      যদি (!option_specified_date && !set_date)
        {
          যদি (optind < argc)
            {
              /* Prepare to set system clock to the specified date/time
                 given in the POSIX-format.  */
              set_date = true;
              datestr = argv[optind];
              valid_date = posixtime (&when.tv_sec,
                                      datestr,
                                      (PDS_TRAILING_YEAR
                                       | PDS_CENTURY | PDS_SECONDS));
              when.tv_nsec = 0; /* FIXME: posixtime should set this.  */
            }
          অন্যথা
            {
              /* Prepare to print the current date/time.  */
              gettime (&when);
            }
        }
      অন্যথা
        {
          /* (option_specified_date || set_date) */
          যদি (reference != রিক্ত)
            {
              সংরচনা stat refstats;
              যদি (stat (reference, &refstats) != 0)
                error (EXIT_FAILURE, errno, "%s", quotef (reference));
              when = get_stat_mtime (&refstats);
            }
          অন্যথা যদি (get_resolution)
            {
              long পূর্ণাঙ্ক res = gettime_res ();
              when.tv_sec = res / TIMESPEC_HZ;
              when.tv_nsec = res % TIMESPEC_HZ;
            }
          অন্যথা
            {
              যদি (set_datestr)
                datestr = set_datestr;
              valid_date = parse_datetime2 (&when, datestr, রিক্ত,
                                            parse_datetime_flags,
                                            tz, tzstring);
            }
        }

      যদি (! valid_date)
        error (EXIT_FAILURE, 0, _("invalid date %s"), quote (datestr));

      যদি (set_date)
        {
          /* Set the system clock to the specified date, then regardless of
             the success of that operation, format and print that date.  */
          যদি (settime (&when) != 0)
            {
              error (0, errno, _("cannot set date"));
              ok = false;
            }
        }

      ok &= show_date_helper (format_res, format_in_c_locale, when, tz);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
