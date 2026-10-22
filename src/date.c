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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#जर HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#अन्तर्भाव "system.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "parse-datetime.h"
#अन्तर्भाव "posixtm.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "show-date.h"
#अन्तर्भाव "stat-time.h"
#अन्तर्भाव "xsetenv.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "date"

#define AUTHORS proper_name ("David MacKenzie")

गणना Time_spec
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

स्थिर अक्षर स्थिरांक *स्थिरांक time_spec_string[] =
{
  /* Put "hours" and "minutes" first, since they aren't valid for
     --rfc-3339.  */
  "hours", "minutes",
  "date", "seconds", "ns", रिक्त
};
स्थिर गणना Time_spec स्थिरांक time_spec[] =
{
  TIME_SPEC_HOURS, TIME_SPEC_MINUTES,
  TIME_SPEC_DATE, TIME_SPEC_SECONDS, TIME_SPEC_NS
};
ARGMATCH_VERIFY (time_spec_string, time_spec);

/* A format suitable for Internet RFCs 5322, 2822, and 822.  */
स्थिर अक्षर स्थिरांक rfc_email_format[] = "%a, %d %b %Y %H:%M:%S %z";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEBUG_DATE_PARSING_OPTION = अक्षर_उच्च + 1,
  RESOLUTION_OPTION,
  RFC_3339_OPTION
};

स्थिर अक्षर स्थिरांक short_options[] = "d:f:I::r:Rs:u";

स्थिर संरचना option स्थिरांक long_options[] =
{
  {"date", required_argument, रिक्त, 'd'},
  {"debug", no_argument, रिक्त, DEBUG_DATE_PARSING_OPTION},
  {"file", required_argument, रिक्त, 'f'},
  {"iso-8601", optional_argument, रिक्त, 'I'},
  {"reference", required_argument, रिक्त, 'r'},
  {"resolution", no_argument, रिक्त, RESOLUTION_OPTION},
  {"rfc-email", no_argument, रिक्त, 'R'},
  {"rfc-822", no_argument, रिक्त, 'R'},  /* Deprecated.  */
  {"rfc-2822", no_argument, रिक्त, 'R'},  /* Deprecated.  */
  {"rfc-3339", required_argument, रिक्त, RFC_3339_OPTION},
  {"set", required_argument, रिक्त, 's'},
  {"uct", no_argument, रिक्त, 'u'},  /* Deprecated.  */
  {"utc", no_argument, रिक्त, 'u'},
  {"universal", no_argument, रिक्त, 'u'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* flags for parse_datetime2 */
स्थिर अचिन्हित पूर्णांक parse_datetime_flags;

#ifdef _DATE_FMT
# define DATE_FMT_LANGINFO() nl_langinfo (_DATE_FMT)
#अन्यथा
# define DATE_FMT_LANGINFO() ""
#endif

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [+FORMAT]\n\
  or:  %s [OPTION]... MMDDhhmm[[CC]YY][.ss]\n\
"),
              program_name, program_name);
      ख_माला_दो (_("\
Display date and समय in the given FORMAT.\n\
With -s, or with MMDDhhmm[[CC]YY][.ss], set the date and समय first.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --date=STRING\n\
         display समय described by STRING, not 'now'\n\
"));
      oputs (_("\
      --debug\n\
         annotate the parsed date,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
  -f, --file=DATEFILE\n\
         like --date; once साठी each ओळ of DATEFILE;\n\
         जर DATEFILE is -, पठ names from standard आदान\n\
"));
      oputs (_("\
  -I[FMT], --iso-8601[=FMT]\n\
         output date/समय in ISO 8601 format.\n\
         FMT='date' (पूर्वनिर्धारित), 'hours', 'minutes', 'seconds', or 'ns'\n\
         साठी date and समय to the indicated precision.\n\
         Example: 2006-08-14T02:34:56-06:00\n\
"));
      oputs (_("\
      --resolution\n\
         output the available resolution of timestamps.\n\
         Example: 0.000000001\n\
"));
      oputs (_("\
  -R, --rfc-email\n\
         output date and समय in RFC 5322 format.\n\
         Example: Mon, 14 Aug 2006 02:34:56 +0000\n\
"));
      oputs (_("\
      --rfc-3339=FMT\n\
         output date/समय in RFC 3339 format.\n\
         FMT='date', 'seconds', or 'ns'\n\
         साठी date and समय to the indicated precision.\n\
         Example: 2006-08-14 02:34:56-06:00\n\
"));
      oputs (_("\
  -r, --reference=खाता\n\
         display the last modification समय of खाता\n\
"));
      oputs (_("\
  -s, --set=STRING\n\
         set समय described by STRING\n\
"));
      oputs (_("\
  -u, --utc, --universal\n\
         print or set Coordinated Universal Time (UTC)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
All options that specify the date to display are mutually exclusive.\n\
I.e.: --date, --file, --reference, --resolution.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
FORMAT controls the output.  Interpreted sequences are:\n\
\n\
"), मानक_निकास);
      ख_माला_दो (_("\
FORMAT  Example    Description\n\
\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %%    %           a literal %\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %n    \\n          a newline\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %t    \\t          a tab\n\
"), मानक_निकास);

      ख_माला_दो ("\n", मानक_निकास);

      ख_माला_दो (_("\
  %C    20          century; like %Y, except omit last two digits\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %y    99          year (last two digits; 00..99)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %Y    1999        year\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %g    99          year of ISO week संख्या (last 2 digits; 00-99); see %G\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %G    1999        year of ISO week संख्या; normally useful only with %V\n\
"), मानक_निकास);

      ख_माला_दो ("\n", मानक_निकास);

      ख_माला_दो (_("\
  %a    Sun         locale's abbreviated weekday name\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %A    Sunday      locale's full weekday name\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %b    Mar         locale's abbreviated month name\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %h    Mar         same as %b\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %B    March       locale's full month name\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %m    12          month (01..12)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %d    31          day of month (01..31)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %e     1          day of month, space padded; same as %_d\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %D    12/31/99    date (ambiguous); same as %m/%d/%y\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %F    1999-12-31  full date; like %+4Y-%m-%d\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %x    12/31/99    locale's date (can be ambiguous)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %c    ...         locale's date and समय (e.g., Thu Mar  3 23:05 2005)\n\
"), मानक_निकास);

      ख_माला_दो ("\n", मानक_निकास);

      ख_माला_दो (_("\
  %H    23          hour (00..23)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %I    01          hour (01..12)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %k     8          hour, space padded ( 0..23); same as %_H\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %l     9          hour, space padded ( 1..12); same as %_I\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %M    59          minute (00..59)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %S    60          second (00..60)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %N    123456789   nanoseconds (000000000..999999999)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %s    1970010100  seconds since the Epoch (1970-01-01 00:00 UTC)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %p    PM          locale's equivalent of AM or PM; blank जर not known\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %P    am          like %p, but lower स्थिती\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %r    1:11:04 PM  locale's 12-hour घड़ी समय\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %R    23:59       24-hour hour and minute; same as %H:%M\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %T    23:59:59    समय; same as %H:%M:%S\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %X    23:59:59    locale's समय representation\n\
"), मानक_निकास);

      ख_माला_दो ("\n", मानक_निकास);

      ख_माला_दो (_("\
  %j    365         day of year (001..366)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %q    4           quarter of year (1..4)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %u    7           day of week (1..7); 1 is Monday\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %w    6           day of week (0..6); 0 is Sunday\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %U    52          week of year; Sunday as first day of week (00..53)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %V    52          ISO week संख्या; Monday as first day of week (01..53)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %W    52          week of year; Monday as first day of week (00..53)\n\
"), मानक_निकास);

      ख_माला_दो ("\n", मानक_निकास);

      ख_माला_दो (_("\
  %z     +0400      +hhmm numeric समय zone\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %:z    +04:00     +hh:mm numeric समय zone\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %::z   +04:00:00  +hh:mm:ss numeric समय zone\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %:::z  +04        numeric समय zone to necessary precision; with :\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %Z     EDT        alphabetic समय zone abbreviation\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
By पूर्वनिर्धारित, date pads numeric fields with zeroes.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
The following optional flags may follow '%':\n\
\n\
  -  (hyphen) करा not pad the field\n\
  _  (underscore) pad with spaces\n\
  0  (zero) pad with zeros\n\
  +  pad with zeros, and put '+' before future years with >4 digits\n\
  ^  use upper स्थिती जर possible\n\
  #  use opposite स्थिती जर possible\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
After any flags comes an optional field width, as a decimal संख्या;\n\
then an optional modifier, which is either\n\
E to use the locale's alternate representations जर available, or\n\
O to use the locale's alternate numeric symbols जर available.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Examples:\n\
Convert seconds since the Epoch (1970-01-01 UTC) to a date\n\
  $ date --date='@2147483647'\n\
\n\
Show the समय on the west coast of the US (use tzselect(1) to find TZ)\n\
  $ TZ='America/Los_Angeles' date\n\
\n\
Show the local समय साठी 9AM next Friday on the west coast of the US\n\
  $ date --date='TZ=\"America/Los_Angeles\" 09:00 next Fri'\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Yield the number of decimal digits needed to output a time with the
   nanosecond resolution RES, without losing information.  */

ATTRIBUTE_CONST
स्थिर पूर्णांक
res_width (दीर्घ पूर्णांक res)
{
  पूर्णांक i = 9;
  साठी (दीर्घ दीर्घ पूर्णांक r = 1; (r *= 10) <= res; )
    i--;
  परत i;
}

/* Return a newly allocated copy of FORMAT with each "%-N" adjusted to
   be "%9N", "%6N", or whatever other resolution is appropriate for
   the current platform.  If no "%-N" appears, return NULL.  */

स्थिर अक्षर *
adjust_resolution (अक्षर स्थिरांक *format)
{
  अक्षर *copy = रिक्त;

  साठी (अक्षर स्थिरांक *f = format; *f; f++)
    जर (f[0] == '%')
      {
        जर (f[1] == '-' && f[2] == 'N')
          {
            जर (!copy)
              copy = xstrdup (format);
            copy[f + 1 - format] = '0' + res_width (gettime_res ());
            f += 2;
          }
        अन्यथा
          f += f[1] == '%';
      }

  परत copy;
}

/* Set the LC_TIME category of the current locale.
   Return the previous value of the LC_TIME category, as a freshly allocated
   string or null.  */

स्थिर अक्षर *
set_LC_TIME (अक्षर स्थिरांक *locale)
{
  /* It is not sufficient to do
       setlocale (LC_TIME, locale);
     because show_date relies on fprintftime, that uses the Gnulib module
     'localename-unsafe', that looks at the values of the environment variables
     (in order to distinguish the default locale from the C locale on platforms
     like macOS).  */
  अक्षर स्थिरांक *all = दो_पर्या ("LC_ALL");
  जर (all != रिक्त && *all != '\0')
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
  अक्षर स्थिरांक *मूल्य = दो_पर्या ("LC_TIME");
  अक्षर *ret = (मूल्य == रिक्त || *मूल्य == '\0' ? रिक्त : xstrdup (मूल्य));
  जर (locale != रिक्त)
    xsetenv ("LC_TIME", locale, 1);
  अन्यथा
    unsetenv ("LC_TIME");

  /* Update the current locale accordingly.  */
  रखो_क्षेत्र (LC_TIME, "");

  परत ret;
}

स्थिर bool
show_date_helper (अक्षर स्थिरांक *format, bool use_c_locale,
                  संरचना timespec when, timezone_t tz)
{
  जर (parse_datetime_flags & PARSE_DATETIME_DEBUG)
    error (0, 0, _("output format: %s"), quote (format));

  bool ok;
  जर (use_c_locale)
    {
      अक्षर *old_locale_category = set_LC_TIME ("C");
      ok = show_date (format, when, tz);
      अक्षर *new_locale_category = set_LC_TIME (old_locale_category);
      मुक्त (new_locale_category);
      मुक्त (old_locale_category);
    }
  अन्यथा
    ok = show_date (format, when, tz);

  अक्षर_दो ('\n');
  परत ok;
}

/* Parse each line in INPUT_FILENAME as with --date and display each
   resulting time and date.  If the file cannot be opened, tell why
   then exit.  Issue a diagnostic for any lines that cannot be parsed.
   Return true if successful.  */

स्थिर bool
batch_convert (अक्षर स्थिरांक *आदान_filename,
               अक्षर स्थिरांक *format, bool format_in_c_locale,
               timezone_t tz, अक्षर स्थिरांक *tzstring)
{
  खाता *in_stream;

  जर (streq (आदान_filename, "-"))
    {
      आदान_filename = _("standard input");
      in_stream = मानक_निवेश;
    }
  अन्यथा
    {
      in_stream = ख_खोलो (आदान_filename, "r");
      जर (in_stream == रिक्त)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (आदान_filename));
    }

  अक्षर *ओळ = रिक्त;
  माप_प्रकार buflen = 0;
  bool ok = true;
  पर्यंत (true)
    {
      ssize_t ओळ_length = getline (&ओळ, &buflen, in_stream);
      जर (ओळ_length < 0)
        {
          जर (ख_त्रुटि (in_stream))
            error (निकास_त्रुटि, त्रुटि_सं, _("%s: read error"),
                   quotef (आदान_filename));
          थांबा;
        }

      संरचना timespec when;
      जर (! parse_datetime2 (&when, ओळ, रिक्त,
                             parse_datetime_flags, tz, tzstring))
        {
          जर (ओळ[ओळ_length - 1] == '\n')
            ओळ[ओळ_length - 1] = '\0';
          error (0, 0, _("invalid date %s"), quote (ओळ));
          ok = false;
        }
      अन्यथा
        {
          ok &= show_date_helper (format, format_in_c_locale, when, tz);
        }

      जर (ख_त्रुटि (मानक_निकास))
        write_error ();
    }

  जर (ख_बंद (in_stream) == खातापूर्ण)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (आदान_filename));

  मुक्त (ओळ);

  परत ok;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  अक्षर स्थिरांक *datestr = रिक्त;
  अक्षर स्थिरांक *set_datestr = रिक्त;
  bool set_date = false;
  अक्षर स्थिरांक *format = रिक्त;
  bool format_in_c_locale = false;
  bool get_resolution = false;
  अक्षर *batch_file = रिक्त;
  अक्षर *reference = रिक्त;
  bool discarded_datestr = false;
  bool discarded_set_datestr = false;
  अक्षर स्थिरांक *tzstring = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णांक optc;
  पर्यंत ((optc = getopt_long (argc, argv, short_options, long_options, रिक्त))
         != -1)
    {
      निवड (optc)
        {
        स्थिती 'd':
          जर (datestr)
            discarded_datestr = true;
          datestr = optarg;
          थांबा;
        स्थिती DEBUG_DATE_PARSING_OPTION:
          parse_datetime_flags |= PARSE_DATETIME_DEBUG;
          थांबा;
        स्थिती 'f':
          batch_file = optarg;
          थांबा;
        स्थिती RESOLUTION_OPTION:
          get_resolution = true;
          थांबा;
        स्थिती RFC_3339_OPTION:
          {
            स्थिर अक्षर स्थिरांक rfc_3339_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%d %H:%M:%S%:z",
                "%Y-%m-%d %H:%M:%S.%N%:z"
              };
            गणना Time_spec i =
              XARGMATCH ("--rfc-3339", optarg,
                         time_spec_string + 2, time_spec + 2);
            format = rfc_3339_format[i];
            format_in_c_locale = true;
            थांबा;
          }
        स्थिती 'I':
          {
            स्थिर अक्षर स्थिरांक iso_8601_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%dT%H:%M:%S%:z",
                "%Y-%m-%dT%H:%M:%S,%N%:z",
                "%Y-%m-%dT%H%:z",
                "%Y-%m-%dT%H:%M%:z"
              };
            गणना Time_spec i =
              (optarg
               ? XARGMATCH ("--iso-8601", optarg, time_spec_string, time_spec)
               : TIME_SPEC_DATE);
            format = iso_8601_format[i];
            format_in_c_locale = true;
            थांबा;
          }
        स्थिती 'r':
          reference = optarg;
          थांबा;
        स्थिती 'R':
          format = rfc_email_format;
          format_in_c_locale = true;
          थांबा;
        स्थिती 's':
          जर (set_datestr)
            discarded_set_datestr = true;
          set_datestr = optarg;
          set_date = true;
          थांबा;
        स्थिती 'u':
          tzstring = "UTC0";
          थांबा;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  पूर्णांक option_specified_date = (!!datestr + !!batch_file + !!reference
                               + get_resolution);

  जर (option_specified_date > 1)
    {
      error (0, 0,
        _("the options to specify dates for printing are mutually exclusive"));
      usage (निकास_त्रुटि);
    }

  जर (set_date && option_specified_date)
    {
      error (0, 0,
          _("the options to print and set the time may not be used together"));
      usage (निकास_त्रुटि);
    }

  जर (discarded_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -d options"));

  जर (discarded_set_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -s options"));

  जर (optind < argc)
    {
      जर (optind + 1 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          usage (निकास_त्रुटि);
        }

      जर (argv[optind][0] == '+')
        {
          जर (format)
            error (निकास_त्रुटि, 0, _("multiple output formats specified"));
          format = argv[optind++] + 1;
        }
      अन्यथा जर (set_date || option_specified_date)
        {
          error (0, 0,
                 _("the argument %s lacks a leading '+';\n"
                   "when using an option to specify date(s), any non-option\n"
                   "argument must be a format string beginning with '+'"),
                 quote (argv[optind]));
          usage (निकास_त्रुटि);
        }
    }

  जर (!format)
    {
      जर (get_resolution)
        format = "%s.%N";
      अन्यथा
        {
          format = DATE_FMT_LANGINFO ();

          /* Do not wrap the following literal format string with _(...).
             For example, suppose LC_ALL is unset, LC_TIME=POSIX,
             and LANG="ko_KR".  In that case, POSIX says that LC_TIME
             determines the format and contents of date and time strings
             written by date, which means "date" must generate output
             using the POSIX locale; but adding _() would cause "date"
             to use a Korean translation of the format.  */
          जर (! *format)
            format = "%a %b %e %H:%M:%S %Z %Y";
        }
    }

  अक्षर *format_copy = adjust_resolution (format);
  अक्षर स्थिरांक *format_res = format_copy ? format_copy : format;
  जर (!tzstring)
    tzstring = दो_पर्या ("TZ");
  timezone_t tz = tzalloc (tzstring);

  bool ok = true;
  जर (batch_file != रिक्त)
    ok = batch_convert (batch_file, format_res, format_in_c_locale,
                        tz, tzstring);
  अन्यथा
    {
      bool valid_date = true;
      संरचना timespec when;

      जर (!option_specified_date && !set_date)
        {
          जर (optind < argc)
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
          अन्यथा
            {
              /* Prepare to print the current date/time.  */
              समय_लो (&when);
            }
        }
      अन्यथा
        {
          /* (option_specified_date || set_date) */
          जर (reference != रिक्त)
            {
              संरचना stat refstats;
              जर (stat (reference, &refstats) != 0)
                error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (reference));
              when = get_stat_mtime (&refstats);
            }
          अन्यथा जर (get_resolution)
            {
              दीर्घ पूर्णांक res = gettime_res ();
              when.tv_sec = res / TIMESPEC_HZ;
              when.tv_nsec = res % TIMESPEC_HZ;
            }
          अन्यथा
            {
              जर (set_datestr)
                datestr = set_datestr;
              valid_date = parse_datetime2 (&when, datestr, रिक्त,
                                            parse_datetime_flags,
                                            tz, tzstring);
            }
        }

      जर (! valid_date)
        error (निकास_त्रुटि, 0, _("invalid date %s"), quote (datestr));

      जर (set_date)
        {
          /* Set the system clock to the specified date, then regardless of
             the success of that operation, format and print that date.  */
          जर (समय_रखो (&when) != 0)
            {
              error (0, त्रुटि_सं, _("cannot set date"));
              ok = false;
            }
        }

      ok &= show_date_helper (format_res, format_in_c_locale, when, tz);
    }

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
