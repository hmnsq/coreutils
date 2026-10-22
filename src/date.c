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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#ఒకవేళ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#అయితే_ముగింపు

#అన్తర్భావ "system.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "parse-datetime.h"
#అన్తర్భావ "posixtm.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "show-date.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "xsetenv.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "date"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

గణన సమయం_spec
{
  /* Display only the date.  */
  సమయం_SPEC_DATE,
  /* Display date, hours, minutes, and seconds.  */
  సమయం_SPEC_SECONDS,
  /* Similar, but display nanoseconds. */
  సమయం_SPEC_NS,

  /* Put these last, since they aren't valid for --rfc-3339.  */

  /* Display date and hour.  */
  సమయం_SPEC_HOURS,
  /* Display date, hours, and minutes.  */
  సమయం_SPEC_MINUTES
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి సమయం_spec_string[] =
{
  /* Put "hours" and "minutes" first, since they aren't valid for
     --rfc-3339.  */
  "hours", "minutes",
  "date", "seconds", "ns", ఖాళీ
};
స్థిర గణన సమయం_spec స్థిరరాశి సమయం_spec_2[] =
{
  సమయం_SPEC_HOURS, సమయం_SPEC_MINUTES,
  సమయం_SPEC_DATE, సమయం_SPEC_SECONDS, సమయం_SPEC_NS
};
ARGMATCH_VERIFY (సమయం_spec_string, సమయం_spec_2);

/* A format suitable for Internet RFCs 5322, 2822, and 822.  */
స్థిర అక్షరం స్థిరరాశి rfc_email_format[] = "%a, %d %b %Y %H:%M:%S %z";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  DEBUG_DATE_PARSING_OPTION = CHAR_MAX + 1,
  RESOLUTION_OPTION,
  RFC_3339_OPTION
};

స్థిర అక్షరం స్థిరరాశి short_options[] = "d:f:I::r:Rs:u";

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"date", required_వాదం, ఖాళీ, 'd'},
  {"debug", no_వాదం, ఖాళీ, DEBUG_DATE_PARSING_OPTION},
  {"file", required_వాదం, ఖాళీ, 'f'},
  {"iso-8601", optional_వాదం, ఖాళీ, 'I'},
  {"reference", required_వాదం, ఖాళీ, 'r'},
  {"resolution", no_వాదం, ఖాళీ, RESOLUTION_OPTION},
  {"rfc-email", no_వాదం, ఖాళీ, 'R'},
  {"rfc-822", no_వాదం, ఖాళీ, 'R'},  /* Deprecated.  */
  {"rfc-2822", no_వాదం, ఖాళీ, 'R'},  /* Deprecated.  */
  {"rfc-3339", required_వాదం, ఖాళీ, RFC_3339_OPTION},
  {"set", required_వాదం, ఖాళీ, 's'},
  {"uct", no_వాదం, ఖాళీ, 'u'},  /* Deprecated.  */
  {"utc", no_వాదం, ఖాళీ, 'u'},
  {"universal", no_వాదం, ఖాళీ, 'u'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* flags for parse_datetime2 */
స్థిర గుర్తులేని పూర్ణాంకం parse_datetime_flags;

#నిర్వచితమైతే _DATE_FMT
# define DATE_FMT_LANGINFO() nl_langinfo (_DATE_FMT)
#లేకపోతే
# define DATE_FMT_LANGINFO() ""
#అయితే_ముగింపు

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [+FORMAT]\n\
  or:  %s [OPTION]... MMDDhhmm[[CC]YY][.ss]\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Display date and సమయం లోపల the given FORMAT.\n\
With -s, or with MMDDhhmm[[CC]YY][.ss], సమితి_2 the date and సమయం మొదటి.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --date=STRING\n\
         display సమయం described by STRING, not 'now'\n\
"));
      oputs (_("\
      --debug\n\
         annotate the parsed date,\n\
         and warn about questionable usage గమ్యం standard error\n\
"));
      oputs (_("\
  -f, --file=DATEFILE\n\
         like --date; once కొరకు each పంక్తి యొక్క DATEFILE;\n\
         ఒకవేళ DATEFILE is -, పఠ పేర్లు from standard ఆదానం\n\
"));
      oputs (_("\
  -I[FMT], --iso-8601[=FMT]\n\
         output date/సమయం లోపల ISO 8601 format.\n\
         FMT='date' (అప్రమేయ), 'hours', 'minutes', 'seconds', or 'ns'\n\
         కొరకు date and సమయం గమ్యం the indicated precision.\n\
         Example: 2006-08-14T02:34:56-06:00\n\
"));
      oputs (_("\
      --resolution\n\
         output the అందుబాటు resolution యొక్క timestamps.\n\
         Example: 0.000000001\n\
"));
      oputs (_("\
  -R, --rfc-email\n\
         output date and సమయం లోపల RFC 5322 format.\n\
         Example: Mon, 14 Aug 2006 02:34:56 +0000\n\
"));
      oputs (_("\
      --rfc-3339=FMT\n\
         output date/సమయం లోపల RFC 3339 format.\n\
         FMT='date', 'seconds', or 'ns'\n\
         కొరకు date and సమయం గమ్యం the indicated precision.\n\
         Example: 2006-08-14 02:34:56-06:00\n\
"));
      oputs (_("\
  -r, --reference=FILE\n\
         display the చివరి modification సమయం యొక్క FILE\n\
"));
      oputs (_("\
  -s, --సమితి_2=STRING\n\
         సమితి_2 సమయం described by STRING\n\
"));
      oputs (_("\
  -u, --utc, --universal\n\
         print or సమితి_2 Coordinated Universal సమయం_2 (UTC)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
All options that specify the date గమ్యం display are mutually exclusive.\n\
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
  %C    20          century; like %Y, except omit చివరి two digits\n\
"), stdout);
      fputs (_("\
  %y    99          year (చివరి two digits; 00..99)\n\
"), stdout);
      fputs (_("\
  %Y    1999        year\n\
"), stdout);
      fputs (_("\
  %g    99          year యొక్క ISO week సంఖ్య (చివరి 2 digits; 00-99); see %G\n\
"), stdout);
      fputs (_("\
  %G    1999        year యొక్క ISO week సంఖ్య; normally useful only with %V\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %a    Sun         locale's abbreviated weekday name\n\
"), stdout);
      fputs (_("\
  %A    Sunday      locale's నిండిన weekday name\n\
"), stdout);
      fputs (_("\
  %b    Mar         locale's abbreviated month name\n\
"), stdout);
      fputs (_("\
  %h    Mar         same as %b\n\
"), stdout);
      fputs (_("\
  %B    March       locale's నిండిన month name\n\
"), stdout);
      fputs (_("\
  %m    12          month (01..12)\n\
"), stdout);
      fputs (_("\
  %d    31          day యొక్క month (01..31)\n\
"), stdout);
      fputs (_("\
  %e     1          day యొక్క month, space padded; same as %_d\n\
"), stdout);
      fputs (_("\
  %D    12/31/99    date (ambiguous); same as %m/%d/%y\n\
"), stdout);
      fputs (_("\
  %F    1999-12-31  నిండిన date; like %+4Y-%m-%d\n\
"), stdout);
      fputs (_("\
  %x    12/31/99    locale's date (can be ambiguous)\n\
"), stdout);
      fputs (_("\
  %c    ...         locale's date and సమయం (e.g., Thu Mar  3 23:05 2005)\n\
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
  %S    60          రెండవ (00..60)\n\
"), stdout);
      fputs (_("\
  %N    123456789   nanoseconds (000000000..999999999)\n\
"), stdout);
      fputs (_("\
  %s    1970010100  seconds since the Epoch (1970-01-01 00:00 UTC)\n\
"), stdout);
      fputs (_("\
  %p    PM          locale's equivalent యొక్క AM or PM; blank ఒకవేళ not తెలిసిన\n\
"), stdout);
      fputs (_("\
  %P    am          like %p, but క్రింది స్థితి\n\
"), stdout);
      fputs (_("\
  %r    1:11:04 PM  locale's 12-hour clock సమయం\n\
"), stdout);
      fputs (_("\
  %R    23:59       24-hour hour and minute; same as %H:%M\n\
"), stdout);
      fputs (_("\
  %T    23:59:59    సమయం; same as %H:%M:%S\n\
"), stdout);
      fputs (_("\
  %X    23:59:59    locale's సమయం representation\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %j    365         day యొక్క year (001..366)\n\
"), stdout);
      fputs (_("\
  %q    4           quarter యొక్క year (1..4)\n\
"), stdout);
      fputs (_("\
  %u    7           day యొక్క week (1..7); 1 is Monday\n\
"), stdout);
      fputs (_("\
  %w    6           day యొక్క week (0..6); 0 is Sunday\n\
"), stdout);
      fputs (_("\
  %U    52          week యొక్క year; Sunday as మొదటి day యొక్క week (00..53)\n\
"), stdout);
      fputs (_("\
  %V    52          ISO week సంఖ్య; Monday as మొదటి day యొక్క week (01..53)\n\
"), stdout);
      fputs (_("\
  %W    52          week యొక్క year; Monday as మొదటి day యొక్క week (00..53)\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %z     +0400      +hhmm numeric సమయం zone\n\
"), stdout);
      fputs (_("\
  %:z    +04:00     +hh:mm numeric సమయం zone\n\
"), stdout);
      fputs (_("\
  %::z   +04:00:00  +hh:mm:ss numeric సమయం zone\n\
"), stdout);
      fputs (_("\
  %:::z  +04        numeric సమయం zone గమ్యం necessary precision; with :\n\
"), stdout);
      fputs (_("\
  %Z     EDT        alphabetic సమయం zone abbreviation\n\
"), stdout);
      fputs (_("\
\n\
By అప్రమేయ, date pads numeric fields with zeroes.\n\
"), stdout);
      fputs (_("\
The following optional flags may follow '%':\n\
\n\
  -  (hyphen) చేయి not pad the క్షేత్రం\n\
  _  (underscore) pad with spaces\n\
  0  (zero) pad with zeros\n\
  +  pad with zeros, and put '+' before future years with >4 digits\n\
  ^  use పై స్థితి ఒకవేళ possible\n\
  #  use opposite స్థితి ఒకవేళ possible\n\
"), stdout);
      fputs (_("\
\n\
After any flags comes an optional క్షేత్రం వెడల్పు, as a decimal సంఖ్య;\n\
then an optional modifier, which is either\n\
E గమ్యం use the locale's alternate representations ఒకవేళ అందుబాటు, or\n\
O గమ్యం use the locale's alternate numeric symbols ఒకవేళ అందుబాటు.\n\
"), stdout);
      fputs (_("\
\n\
Examples:\n\
Convert seconds since the Epoch (1970-01-01 UTC) గమ్యం a date\n\
  $ date --date='@2147483647'\n\
\n\
Show the సమయం on the west coast యొక్క the US (use tzselect(1) గమ్యం కనుగొను TZ)\n\
  $ TZ='America/Los_Angeles' date\n\
\n\
Show the local సమయం కొరకు 9AM తదుపరి Friday on the west coast యొక్క the US\n\
  $ date --date='TZ=\"America/Los_Angeles\" 09:00 next Fri'\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Yield the number of decimal digits needed to output a time with the
   nanosecond resolution RES, without losing information.  */

ATTRIBUTE_CONST
స్థిర పూర్ణాంకం
res_వెడల్పు (దీర్ఘ పూర్ణాంకం res)
{
  పూర్ణాంకం i = 9;
  కొరకు (దీర్ఘ దీర్ఘ పూర్ణాంకం r = 1; (r *= 10) <= res; )
    i--;
  తిరిగి i;
}

/* Return a newly allocated copy of FORMAT with each "%-N" adjusted to
   be "%9N", "%6N", or whatever other resolution is appropriate for
   the current platform.  If no "%-N" appears, return NULL.  */

స్థిర అక్షరం *
adjust_resolution (అక్షరం స్థిరరాశి *format)
{
  అక్షరం *copy = ఖాళీ;

  కొరకు (అక్షరం స్థిరరాశి *f = format; *f; f++)
    ఒకవేళ (f[0] == '%')
      {
        ఒకవేళ (f[1] == '-' && f[2] == 'N')
          {
            ఒకవేళ (!copy)
              copy = xstrdup (format);
            copy[f + 1 - format] = '0' + res_వెడల్పు (gettime_res ());
            f += 2;
          }
        లేకపోతే
          f += f[1] == '%';
      }

  తిరిగి copy;
}

/* Set the LC_TIME category of the current locale.
   Return the previous value of the LC_TIME category, as a freshly allocated
   string or null.  */

స్థిర అక్షరం *
సమితి_LC_సమయం (అక్షరం స్థిరరాశి *locale)
{
  /* It is not sufficient to do
       setlocale (LC_TIME, locale);
     because show_date relies on fprintftime, that uses the Gnulib module
     'localename-unsafe', that looks at the values of the environment variables
     (in order to distinguish the default locale from the C locale on platforms
     like macOS).  */
  అక్షరం స్థిరరాశి *all = getenv ("LC_ALL");
  ఒకవేళ (all != ఖాళీ && *all != '\0')
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
  అక్షరం స్థిరరాశి *విలువ = getenv ("LC_TIME");
  అక్షరం *ret = (విలువ == ఖాళీ || *విలువ == '\0' ? ఖాళీ : xstrdup (విలువ));
  ఒకవేళ (locale != ఖాళీ)
    xsetenv ("LC_TIME", locale, 1);
  లేకపోతే
    unsetenv ("LC_TIME");

  /* Update the current locale accordingly.  */
  setlocale (LC_సమయం, "");

  తిరిగి ret;
}

స్థిర bool
show_date_helper (అక్షరం స్థిరరాశి *format, bool use_c_locale,
                  నిర్మాణం timespec when, timezone_t tz)
{
  ఒకవేళ (parse_datetime_flags & PARSE_DATETIME_DEBUG)
    error (0, 0, _("output format: %s"), quote (format));

  bool ok;
  ఒకవేళ (use_c_locale)
    {
      అక్షరం *old_locale_category = సమితి_LC_సమయం ("C");
      ok = show_date (format, when, tz);
      అక్షరం *new_locale_category = సమితి_LC_సమయం (old_locale_category);
      విడుదల (new_locale_category);
      విడుదల (old_locale_category);
    }
  లేకపోతే
    ok = show_date (format, when, tz);

  అక్షరం_రాయి ('\n');
  తిరిగి ok;
}

/* Parse each line in INPUT_FILENAME as with --date and display each
   resulting time and date.  If the file cannot be opened, tell why
   then exit.  Issue a diagnostic for any lines that cannot be parsed.
   Return true if successful.  */

స్థిర bool
batch_convert (అక్షరం స్థిరరాశి *ఆదానం_filename,
               అక్షరం స్థిరరాశి *format, bool format_లోపల_c_locale,
               timezone_t tz, అక్షరం స్థిరరాశి *tzstring)
{
  FILE *లోపల_ప్రవాహం;

  ఒకవేళ (streq (ఆదానం_filename, "-"))
    {
      ఆదానం_filename = _("standard input");
      లోపల_ప్రవాహం = stdin;
    }
  లేకపోతే
    {
      లోపల_ప్రవాహం = fopen (ఆదానం_filename, "r");
      ఒకవేళ (లోపల_ప్రవాహం == ఖాళీ)
        error (EXIT_FAILURE, errno, "%s", quotef (ఆదానం_filename));
    }

  అక్షరం *పంక్తి = ఖాళీ;
  size_t buflen = 0;
  bool ok = true;
  అయితే (true)
    {
      ssize_t పంక్తి_నిడివి = getline (&పంక్తి, &buflen, లోపల_ప్రవాహం);
      ఒకవేళ (పంక్తి_నిడివి < 0)
        {
          ఒకవేళ (ferror (లోపల_ప్రవాహం))
            error (EXIT_FAILURE, errno, _("%s: read error"),
                   quotef (ఆదానం_filename));
          ఆపు;
        }

      నిర్మాణం timespec when;
      ఒకవేళ (! parse_datetime2 (&when, పంక్తి, ఖాళీ,
                             parse_datetime_flags, tz, tzstring))
        {
          ఒకవేళ (పంక్తి[పంక్తి_నిడివి - 1] == '\n')
            పంక్తి[పంక్తి_నిడివి - 1] = '\0';
          error (0, 0, _("invalid date %s"), quote (పంక్తి));
          ok = false;
        }
      లేకపోతే
        {
          ok &= show_date_helper (format, format_లోపల_c_locale, when, tz);
        }

      ఒకవేళ (ferror (stdout))
        write_error ();
    }

  ఒకవేళ (fclose (లోపల_ప్రవాహం) == EOF)
    error (EXIT_FAILURE, errno, "%s", quotef (ఆదానం_filename));

  విడుదల (పంక్తి);

  తిరిగి ok;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం స్థిరరాశి *datestr = ఖాళీ;
  అక్షరం స్థిరరాశి *సమితి_datestr = ఖాళీ;
  bool సమితి_date = false;
  అక్షరం స్థిరరాశి *format = ఖాళీ;
  bool format_లోపల_c_locale = false;
  bool get_resolution = false;
  అక్షరం *batch_file = ఖాళీ;
  అక్షరం *reference = ఖాళీ;
  bool discarded_datestr = false;
  bool discarded_సమితి_datestr = false;
  అక్షరం స్థిరరాశి *tzstring = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  పూర్ణాంకం optc;
  అయితే ((optc = getopt_long (argc, argv, short_options, long_options, ఖాళీ))
         != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'd':
          ఒకవేళ (datestr)
            discarded_datestr = true;
          datestr = optarg;
          ఆపు;
        స్థితి DEBUG_DATE_PARSING_OPTION:
          parse_datetime_flags |= PARSE_DATETIME_DEBUG;
          ఆపు;
        స్థితి 'f':
          batch_file = optarg;
          ఆపు;
        స్థితి RESOLUTION_OPTION:
          get_resolution = true;
          ఆపు;
        స్థితి RFC_3339_OPTION:
          {
            స్థిర అక్షరం స్థిరరాశి rfc_3339_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%d %H:%M:%S%:z",
                "%Y-%m-%d %H:%M:%S.%N%:z"
              };
            గణన సమయం_spec i =
              XARGMATCH ("--rfc-3339", optarg,
                         సమయం_spec_string + 2, సమయం_spec_2 + 2);
            format = rfc_3339_format[i];
            format_లోపల_c_locale = true;
            ఆపు;
          }
        స్థితి 'I':
          {
            స్థిర అక్షరం స్థిరరాశి iso_8601_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%dT%H:%M:%S%:z",
                "%Y-%m-%dT%H:%M:%S,%N%:z",
                "%Y-%m-%dT%H%:z",
                "%Y-%m-%dT%H:%M%:z"
              };
            గణన సమయం_spec i =
              (optarg
               ? XARGMATCH ("--iso-8601", optarg, సమయం_spec_string, సమయం_spec_2)
               : సమయం_SPEC_DATE);
            format = iso_8601_format[i];
            format_లోపల_c_locale = true;
            ఆపు;
          }
        స్థితి 'r':
          reference = optarg;
          ఆపు;
        స్థితి 'R':
          format = rfc_email_format;
          format_లోపల_c_locale = true;
          ఆపు;
        స్థితి 's':
          ఒకవేళ (సమితి_datestr)
            discarded_సమితి_datestr = true;
          సమితి_datestr = optarg;
          సమితి_date = true;
          ఆపు;
        స్థితి 'u':
          tzstring = "UTC0";
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  పూర్ణాంకం option_specified_date = (!!datestr + !!batch_file + !!reference
                               + get_resolution);

  ఒకవేళ (option_specified_date > 1)
    {
      error (0, 0,
        _("the options to specify dates for printing are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (సమితి_date && option_specified_date)
    {
      error (0, 0,
          _("the options to print and set the time may not be used together"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (discarded_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -d options"));

  ఒకవేళ (discarded_సమితి_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -s options"));

  ఒకవేళ (optind < argc)
    {
      ఒకవేళ (optind + 1 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          usage (EXIT_FAILURE);
        }

      ఒకవేళ (argv[optind][0] == '+')
        {
          ఒకవేళ (format)
            error (EXIT_FAILURE, 0, _("multiple output formats specified"));
          format = argv[optind++] + 1;
        }
      లేకపోతే ఒకవేళ (సమితి_date || option_specified_date)
        {
          error (0, 0,
                 _("the argument %s lacks a leading '+';\n"
                   "when using an option to specify date(s), any non-option\n"
                   "argument must be a format string beginning with '+'"),
                 quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (!format)
    {
      ఒకవేళ (get_resolution)
        format = "%s.%N";
      లేకపోతే
        {
          format = DATE_FMT_LANGINFO ();

          /* Do not wrap the following literal format string with _(...).
             For example, suppose LC_ALL is unset, LC_TIME=POSIX,
             and LANG="ko_KR".  In that case, POSIX says that LC_TIME
             determines the format and contents of date and time strings
             written by date, which means "date" must generate output
             using the POSIX locale; but adding _() would cause "date"
             to use a Korean translation of the format.  */
          ఒకవేళ (! *format)
            format = "%a %b %e %H:%M:%S %Z %Y";
        }
    }

  అక్షరం *format_copy = adjust_resolution (format);
  అక్షరం స్థిరరాశి *format_res = format_copy ? format_copy : format;
  ఒకవేళ (!tzstring)
    tzstring = getenv ("TZ");
  timezone_t tz = tzalloc (tzstring);

  bool ok = true;
  ఒకవేళ (batch_file != ఖాళీ)
    ok = batch_convert (batch_file, format_res, format_లోపల_c_locale,
                        tz, tzstring);
  లేకపోతే
    {
      bool valid_date = true;
      నిర్మాణం timespec when;

      ఒకవేళ (!option_specified_date && !సమితి_date)
        {
          ఒకవేళ (optind < argc)
            {
              /* Prepare to set system clock to the specified date/time
                 given in the POSIX-format.  */
              సమితి_date = true;
              datestr = argv[optind];
              valid_date = posixtime (&when.tv_sec,
                                      datestr,
                                      (PDS_TRAILING_YEAR
                                       | PDS_CENTURY | PDS_SECONDS));
              when.tv_nsec = 0; /* FIXME: posixtime should set this.  */
            }
          లేకపోతే
            {
              /* Prepare to print the current date/time.  */
              gettime (&when);
            }
        }
      లేకపోతే
        {
          /* (option_specified_date || set_date) */
          ఒకవేళ (reference != ఖాళీ)
            {
              నిర్మాణం stat refstats;
              ఒకవేళ (stat (reference, &refstats) != 0)
                error (EXIT_FAILURE, errno, "%s", quotef (reference));
              when = get_stat_mtime (&refstats);
            }
          లేకపోతే ఒకవేళ (get_resolution)
            {
              దీర్ఘ పూర్ణాంకం res = gettime_res ();
              when.tv_sec = res / TIMESPEC_HZ;
              when.tv_nsec = res % TIMESPEC_HZ;
            }
          లేకపోతే
            {
              ఒకవేళ (సమితి_datestr)
                datestr = సమితి_datestr;
              valid_date = parse_datetime2 (&when, datestr, ఖాళీ,
                                            parse_datetime_flags,
                                            tz, tzstring);
            }
        }

      ఒకవేళ (! valid_date)
        error (EXIT_FAILURE, 0, _("invalid date %s"), quote (datestr));

      ఒకవేళ (సమితి_date)
        {
          /* Set the system clock to the specified date, then regardless of
             the success of that operation, format and print that date.  */
          ఒకవేళ (settime (&when) != 0)
            {
              error (0, errno, _("cannot set date"));
              ok = false;
            }
        }

      ok &= show_date_helper (format_res, format_లోపల_c_locale, when, tz);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
