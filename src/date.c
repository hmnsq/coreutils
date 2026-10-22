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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "parse-datetime.h"
#ಅನ್ತರ್ಭಾವ "posixtm.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "show-date.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "xsetenv.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "date"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

ಎಣಿಕೆ ಸಮಯ_spec
{
  /* Display only the date.  */
  ಸಮಯ_SPEC_DATE,
  /* Display date, hours, minutes, and seconds.  */
  ಸಮಯ_SPEC_SECONDS,
  /* Similar, but display nanoseconds. */
  ಸಮಯ_SPEC_NS,

  /* Put these last, since they aren't valid for --rfc-3339.  */

  /* Display date and hour.  */
  ಸಮಯ_SPEC_HOURS,
  /* Display date, hours, and minutes.  */
  ಸಮಯ_SPEC_MINUTES
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ಸಮಯ_spec_string[] =
{
  /* Put "hours" and "minutes" first, since they aren't valid for
     --rfc-3339.  */
  "hours", "minutes",
  "date", "seconds", "ns", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ ಸಮಯ_spec ಸ್ಥಿರಾಂಕ ಸಮಯ_spec_2[] =
{
  ಸಮಯ_SPEC_HOURS, ಸಮಯ_SPEC_MINUTES,
  ಸಮಯ_SPEC_DATE, ಸಮಯ_SPEC_SECONDS, ಸಮಯ_SPEC_NS
};
ARGMATCH_VERIFY (ಸಮಯ_spec_string, ಸಮಯ_spec_2);

/* A format suitable for Internet RFCs 5322, 2822, and 822.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ rfc_email_format[] = "%a, %d %b %Y %H:%M:%S %z";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  DEBUG_DATE_PARSING_OPTION = CHAR_MAX + 1,
  RESOLUTION_OPTION,
  RFC_3339_OPTION
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ short_options[] = "d:f:I::r:Rs:u";

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"date", required_ವಾದ, ಖಾಲಿ, 'd'},
  {"debug", no_ವಾದ, ಖಾಲಿ, DEBUG_DATE_PARSING_OPTION},
  {"file", required_ವಾದ, ಖಾಲಿ, 'f'},
  {"iso-8601", optional_ವಾದ, ಖಾಲಿ, 'I'},
  {"reference", required_ವಾದ, ಖಾಲಿ, 'r'},
  {"resolution", no_ವಾದ, ಖಾಲಿ, RESOLUTION_OPTION},
  {"rfc-email", no_ವಾದ, ಖಾಲಿ, 'R'},
  {"rfc-822", no_ವಾದ, ಖಾಲಿ, 'R'},  /* Deprecated.  */
  {"rfc-2822", no_ವಾದ, ಖಾಲಿ, 'R'},  /* Deprecated.  */
  {"rfc-3339", required_ವಾದ, ಖಾಲಿ, RFC_3339_OPTION},
  {"set", required_ವಾದ, ಖಾಲಿ, 's'},
  {"uct", no_ವಾದ, ಖಾಲಿ, 'u'},  /* Deprecated.  */
  {"utc", no_ವಾದ, ಖಾಲಿ, 'u'},
  {"universal", no_ವಾದ, ಖಾಲಿ, 'u'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* flags for parse_datetime2 */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ parse_datetime_flags;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _DATE_FMT
# define DATE_FMT_LANGINFO() nl_langinfo (_DATE_FMT)
#ಇಲ್ಲದಿದ್ದರೆ
# define DATE_FMT_LANGINFO() ""
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [+FORMAT]\n\
  or:  %s [OPTION]... MMDDhhmm[[CC]YY][.ss]\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Display date and ಸಮಯ ಒಳ the given FORMAT.\n\
With -s, or with MMDDhhmm[[CC]YY][.ss], ಗಣ_2 the date and ಸಮಯ ಮೊದಲ.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --date=STRING\n\
         display ಸಮಯ described by STRING, not 'now'\n\
"));
      oputs (_("\
      --debug\n\
         annotate the parsed date,\n\
         and warn about questionable usage ಗಮ್ಯ standard error\n\
"));
      oputs (_("\
  -f, --file=DATEFILE\n\
         like --date; once ಗಾಗಿ each ಸಾಲು ದ DATEFILE;\n\
         ಆದರೆ DATEFILE is -, ಪಠ ಹೆಸರುಗಳು from standard ಒಳಹರಿವು\n\
"));
      oputs (_("\
  -I[FMT], --iso-8601[=FMT]\n\
         output date/ಸಮಯ ಒಳ ISO 8601 format.\n\
         FMT='date' (ಪೂರ್ವನಿಯೋಜಿತ), 'hours', 'minutes', 'seconds', or 'ns'\n\
         ಗಾಗಿ date and ಸಮಯ ಗಮ್ಯ the indicated precision.\n\
         Example: 2006-08-14T02:34:56-06:00\n\
"));
      oputs (_("\
      --resolution\n\
         output the ಲಭ್ಯ resolution ದ timestamps.\n\
         Example: 0.000000001\n\
"));
      oputs (_("\
  -R, --rfc-email\n\
         output date and ಸಮಯ ಒಳ RFC 5322 format.\n\
         Example: Mon, 14 Aug 2006 02:34:56 +0000\n\
"));
      oputs (_("\
      --rfc-3339=FMT\n\
         output date/ಸಮಯ ಒಳ RFC 3339 format.\n\
         FMT='date', 'seconds', or 'ns'\n\
         ಗಾಗಿ date and ಸಮಯ ಗಮ್ಯ the indicated precision.\n\
         Example: 2006-08-14 02:34:56-06:00\n\
"));
      oputs (_("\
  -r, --reference=FILE\n\
         display the ಕೊನೆಯ modification ಸಮಯ ದ FILE\n\
"));
      oputs (_("\
  -s, --ಗಣ_2=STRING\n\
         ಗಣ_2 ಸಮಯ described by STRING\n\
"));
      oputs (_("\
  -u, --utc, --universal\n\
         print or ಗಣ_2 Coordinated Universal ಸಮಯ_2 (UTC)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
All options that specify the date ಗಮ್ಯ display are mutually exclusive.\n\
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
  %C    20          century; like %Y, except omit ಕೊನೆಯ two digits\n\
"), stdout);
      fputs (_("\
  %y    99          year (ಕೊನೆಯ two digits; 00..99)\n\
"), stdout);
      fputs (_("\
  %Y    1999        year\n\
"), stdout);
      fputs (_("\
  %g    99          year ದ ISO week ಸಂಖ್ಯೆ (ಕೊನೆಯ 2 digits; 00-99); see %G\n\
"), stdout);
      fputs (_("\
  %G    1999        year ದ ISO week ಸಂಖ್ಯೆ; normally useful only with %V\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %a    Sun         locale's abbreviated weekday name\n\
"), stdout);
      fputs (_("\
  %A    Sunday      locale's ತುಂಬಿದ weekday name\n\
"), stdout);
      fputs (_("\
  %b    Mar         locale's abbreviated month name\n\
"), stdout);
      fputs (_("\
  %h    Mar         same as %b\n\
"), stdout);
      fputs (_("\
  %B    March       locale's ತುಂಬಿದ month name\n\
"), stdout);
      fputs (_("\
  %m    12          month (01..12)\n\
"), stdout);
      fputs (_("\
  %d    31          day ದ month (01..31)\n\
"), stdout);
      fputs (_("\
  %e     1          day ದ month, space padded; same as %_d\n\
"), stdout);
      fputs (_("\
  %D    12/31/99    date (ambiguous); same as %m/%d/%y\n\
"), stdout);
      fputs (_("\
  %F    1999-12-31  ತುಂಬಿದ date; like %+4Y-%m-%d\n\
"), stdout);
      fputs (_("\
  %x    12/31/99    locale's date (can be ambiguous)\n\
"), stdout);
      fputs (_("\
  %c    ...         locale's date and ಸಮಯ (e.g., Thu Mar  3 23:05 2005)\n\
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
  %S    60          ಎರಡನೇ (00..60)\n\
"), stdout);
      fputs (_("\
  %N    123456789   nanoseconds (000000000..999999999)\n\
"), stdout);
      fputs (_("\
  %s    1970010100  seconds since the Epoch (1970-01-01 00:00 UTC)\n\
"), stdout);
      fputs (_("\
  %p    PM          locale's equivalent ದ AM or PM; blank ಆದರೆ not ತಿಳಿದ\n\
"), stdout);
      fputs (_("\
  %P    am          like %p, but ಅಧೋ ಸ್ಥಿತಿ\n\
"), stdout);
      fputs (_("\
  %r    1:11:04 PM  locale's 12-hour clock ಸಮಯ\n\
"), stdout);
      fputs (_("\
  %R    23:59       24-hour hour and minute; same as %H:%M\n\
"), stdout);
      fputs (_("\
  %T    23:59:59    ಸಮಯ; same as %H:%M:%S\n\
"), stdout);
      fputs (_("\
  %X    23:59:59    locale's ಸಮಯ representation\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %j    365         day ದ year (001..366)\n\
"), stdout);
      fputs (_("\
  %q    4           quarter ದ year (1..4)\n\
"), stdout);
      fputs (_("\
  %u    7           day ದ week (1..7); 1 is Monday\n\
"), stdout);
      fputs (_("\
  %w    6           day ದ week (0..6); 0 is Sunday\n\
"), stdout);
      fputs (_("\
  %U    52          week ದ year; Sunday as ಮೊದಲ day ದ week (00..53)\n\
"), stdout);
      fputs (_("\
  %V    52          ISO week ಸಂಖ್ಯೆ; Monday as ಮೊದಲ day ದ week (01..53)\n\
"), stdout);
      fputs (_("\
  %W    52          week ದ year; Monday as ಮೊದಲ day ದ week (00..53)\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %z     +0400      +hhmm numeric ಸಮಯ zone\n\
"), stdout);
      fputs (_("\
  %:z    +04:00     +hh:mm numeric ಸಮಯ zone\n\
"), stdout);
      fputs (_("\
  %::z   +04:00:00  +hh:mm:ss numeric ಸಮಯ zone\n\
"), stdout);
      fputs (_("\
  %:::z  +04        numeric ಸಮಯ zone ಗಮ್ಯ necessary precision; with :\n\
"), stdout);
      fputs (_("\
  %Z     EDT        alphabetic ಸಮಯ zone abbreviation\n\
"), stdout);
      fputs (_("\
\n\
By ಪೂರ್ವನಿಯೋಜಿತ, date pads numeric fields with zeroes.\n\
"), stdout);
      fputs (_("\
The following optional flags may follow '%':\n\
\n\
  -  (hyphen) ಮಾಡು not pad the ಕ್ಷೇತ್ರ\n\
  _  (underscore) pad with spaces\n\
  0  (zero) pad with zeros\n\
  +  pad with zeros, and put '+' before future years with >4 digits\n\
  ^  use ಊರ್ಧ್ವ ಸ್ಥಿತಿ ಆದರೆ possible\n\
  #  use opposite ಸ್ಥಿತಿ ಆದರೆ possible\n\
"), stdout);
      fputs (_("\
\n\
After any flags comes an optional ಕ್ಷೇತ್ರ ಅಗಲ, as a decimal ಸಂಖ್ಯೆ;\n\
then an optional modifier, which is either\n\
E ಗಮ್ಯ use the locale's alternate representations ಆದರೆ ಲಭ್ಯ, or\n\
O ಗಮ್ಯ use the locale's alternate numeric symbols ಆದರೆ ಲಭ್ಯ.\n\
"), stdout);
      fputs (_("\
\n\
Examples:\n\
Convert seconds since the Epoch (1970-01-01 UTC) ಗಮ್ಯ a date\n\
  $ date --date='@2147483647'\n\
\n\
Show the ಸಮಯ on the west coast ದ the US (use tzselect(1) ಗಮ್ಯ ಕಂಡುಹಿಡಿ TZ)\n\
  $ TZ='America/Los_Angeles' date\n\
\n\
Show the local ಸಮಯ ಗಾಗಿ 9AM ಮುಂದಿನ Friday on the west coast ದ the US\n\
  $ date --date='TZ=\"America/Los_Angeles\" 09:00 next Fri'\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Yield the number of decimal digits needed to output a time with the
   nanosecond resolution RES, without losing information.  */

ATTRIBUTE_CONST
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
res_ಅಗಲ (ದೀರ್ಘ ಪೂರ್ಣಾಂಕ res)
{
  ಪೂರ್ಣಾಂಕ i = 9;
  ಗಾಗಿ (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ r = 1; (r *= 10) <= res; )
    i--;
  ಹಿಂತಿರುಗಿ i;
}

/* Return a newly allocated copy of FORMAT with each "%-N" adjusted to
   be "%9N", "%6N", or whatever other resolution is appropriate for
   the current platform.  If no "%-N" appears, return NULL.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
adjust_resolution (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format)
{
  ಅಕ್ಷರ *copy = ಖಾಲಿ;

  ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *f = format; *f; f++)
    ಆದರೆ (f[0] == '%')
      {
        ಆದರೆ (f[1] == '-' && f[2] == 'N')
          {
            ಆದರೆ (!copy)
              copy = xstrdup (format);
            copy[f + 1 - format] = '0' + res_ಅಗಲ (gettime_res ());
            f += 2;
          }
        ಇಲ್ಲದಿದ್ದರೆ
          f += f[1] == '%';
      }

  ಹಿಂತಿರುಗಿ copy;
}

/* Set the LC_TIME category of the current locale.
   Return the previous value of the LC_TIME category, as a freshly allocated
   string or null.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
ಗಣ_LC_ಸಮಯ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *locale)
{
  /* It is not sufficient to do
       setlocale (LC_TIME, locale);
     because show_date relies on fprintftime, that uses the Gnulib module
     'localename-unsafe', that looks at the values of the environment variables
     (in order to distinguish the default locale from the C locale on platforms
     like macOS).  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *all = getenv ("LC_ALL");
  ಆದರೆ (all != ಖಾಲಿ && *all != '\0')
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
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ = getenv ("LC_TIME");
  ಅಕ್ಷರ *ret = (ಮೌಲ್ಯ == ಖಾಲಿ || *ಮೌಲ್ಯ == '\0' ? ಖಾಲಿ : xstrdup (ಮೌಲ್ಯ));
  ಆದರೆ (locale != ಖಾಲಿ)
    xsetenv ("LC_TIME", locale, 1);
  ಇಲ್ಲದಿದ್ದರೆ
    unsetenv ("LC_TIME");

  /* Update the current locale accordingly.  */
  setlocale (LC_ಸಮಯ, "");

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ bool
show_date_helper (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format, bool use_c_locale,
                  ರಚನೆ timespec when, timezone_t tz)
{
  ಆದರೆ (parse_datetime_flags & PARSE_DATETIME_DEBUG)
    error (0, 0, _("output format: %s"), quote (format));

  bool ok;
  ಆದರೆ (use_c_locale)
    {
      ಅಕ್ಷರ *old_locale_category = ಗಣ_LC_ಸಮಯ ("C");
      ok = show_date (format, when, tz);
      ಅಕ್ಷರ *new_locale_category = ಗಣ_LC_ಸಮಯ (old_locale_category);
      ಬಿಡುಗಡೆ (new_locale_category);
      ಬಿಡುಗಡೆ (old_locale_category);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ok = show_date (format, when, tz);

  ಅಕ್ಷರ_ಬರೆ ('\n');
  ಹಿಂತಿರುಗಿ ok;
}

/* Parse each line in INPUT_FILENAME as with --date and display each
   resulting time and date.  If the file cannot be opened, tell why
   then exit.  Issue a diagnostic for any lines that cannot be parsed.
   Return true if successful.  */

ಸ್ಥಿರ bool
batch_convert (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಒಳಹರಿವು_filename,
               ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format, bool format_ಒಳ_c_locale,
               timezone_t tz, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *tzstring)
{
  FILE *ಒಳ_ಹರಿವು;

  ಆದರೆ (streq (ಒಳಹರಿವು_filename, "-"))
    {
      ಒಳಹರಿವು_filename = _("standard input");
      ಒಳ_ಹರಿವು = stdin;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಒಳ_ಹರಿವು = fopen (ಒಳಹರಿವು_filename, "r");
      ಆದರೆ (ಒಳ_ಹರಿವು == ಖಾಲಿ)
        error (EXIT_FAILURE, errno, "%s", quotef (ಒಳಹರಿವು_filename));
    }

  ಅಕ್ಷರ *ಸಾಲು = ಖಾಲಿ;
  size_t buflen = 0;
  bool ok = true;
  ಆಗ (true)
    {
      ssize_t ಸಾಲು_ಉದ್ದಳತೆ = getline (&ಸಾಲು, &buflen, ಒಳ_ಹರಿವು);
      ಆದರೆ (ಸಾಲು_ಉದ್ದಳತೆ < 0)
        {
          ಆದರೆ (ferror (ಒಳ_ಹರಿವು))
            error (EXIT_FAILURE, errno, _("%s: read error"),
                   quotef (ಒಳಹರಿವು_filename));
          ನಿಲ್ಲಿಸು;
        }

      ರಚನೆ timespec when;
      ಆದರೆ (! parse_datetime2 (&when, ಸಾಲು, ಖಾಲಿ,
                             parse_datetime_flags, tz, tzstring))
        {
          ಆದರೆ (ಸಾಲು[ಸಾಲು_ಉದ್ದಳತೆ - 1] == '\n')
            ಸಾಲು[ಸಾಲು_ಉದ್ದಳತೆ - 1] = '\0';
          error (0, 0, _("invalid date %s"), quote (ಸಾಲು));
          ok = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ok &= show_date_helper (format, format_ಒಳ_c_locale, when, tz);
        }

      ಆದರೆ (ferror (stdout))
        write_error ();
    }

  ಆದರೆ (fclose (ಒಳ_ಹರಿವು) == EOF)
    error (EXIT_FAILURE, errno, "%s", quotef (ಒಳಹರಿವು_filename));

  ಬಿಡುಗಡೆ (ಸಾಲು);

  ಹಿಂತಿರುಗಿ ok;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *datestr = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗಣ_datestr = ಖಾಲಿ;
  bool ಗಣ_date = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format = ಖಾಲಿ;
  bool format_ಒಳ_c_locale = false;
  bool get_resolution = false;
  ಅಕ್ಷರ *batch_file = ಖಾಲಿ;
  ಅಕ್ಷರ *reference = ಖಾಲಿ;
  bool discarded_datestr = false;
  bool discarded_ಗಣ_datestr = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *tzstring = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಂಕ optc;
  ಆಗ ((optc = getopt_long (argc, argv, short_options, long_options, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'd':
          ಆದರೆ (datestr)
            discarded_datestr = true;
          datestr = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ DEBUG_DATE_PARSING_OPTION:
          parse_datetime_flags |= PARSE_DATETIME_DEBUG;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'f':
          batch_file = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ RESOLUTION_OPTION:
          get_resolution = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ RFC_3339_OPTION:
          {
            ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ rfc_3339_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%d %H:%M:%S%:z",
                "%Y-%m-%d %H:%M:%S.%N%:z"
              };
            ಎಣಿಕೆ ಸಮಯ_spec i =
              XARGMATCH ("--rfc-3339", optarg,
                         ಸಮಯ_spec_string + 2, ಸಮಯ_spec_2 + 2);
            format = rfc_3339_format[i];
            format_ಒಳ_c_locale = true;
            ನಿಲ್ಲಿಸು;
          }
        ಸ್ಥಿತಿ 'I':
          {
            ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ iso_8601_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%dT%H:%M:%S%:z",
                "%Y-%m-%dT%H:%M:%S,%N%:z",
                "%Y-%m-%dT%H%:z",
                "%Y-%m-%dT%H:%M%:z"
              };
            ಎಣಿಕೆ ಸಮಯ_spec i =
              (optarg
               ? XARGMATCH ("--iso-8601", optarg, ಸಮಯ_spec_string, ಸಮಯ_spec_2)
               : ಸಮಯ_SPEC_DATE);
            format = iso_8601_format[i];
            format_ಒಳ_c_locale = true;
            ನಿಲ್ಲಿಸು;
          }
        ಸ್ಥಿತಿ 'r':
          reference = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'R':
          format = rfc_email_format;
          format_ಒಳ_c_locale = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 's':
          ಆದರೆ (ಗಣ_datestr)
            discarded_ಗಣ_datestr = true;
          ಗಣ_datestr = optarg;
          ಗಣ_date = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'u':
          tzstring = "UTC0";
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಪೂರ್ಣಾಂಕ option_specified_date = (!!datestr + !!batch_file + !!reference
                               + get_resolution);

  ಆದರೆ (option_specified_date > 1)
    {
      error (0, 0,
        _("the options to specify dates for printing are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (ಗಣ_date && option_specified_date)
    {
      error (0, 0,
          _("the options to print and set the time may not be used together"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (discarded_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -d options"));

  ಆದರೆ (discarded_ಗಣ_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -s options"));

  ಆದರೆ (optind < argc)
    {
      ಆದರೆ (optind + 1 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          usage (EXIT_FAILURE);
        }

      ಆದರೆ (argv[optind][0] == '+')
        {
          ಆದರೆ (format)
            error (EXIT_FAILURE, 0, _("multiple output formats specified"));
          format = argv[optind++] + 1;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಣ_date || option_specified_date)
        {
          error (0, 0,
                 _("the argument %s lacks a leading '+';\n"
                   "when using an option to specify date(s), any non-option\n"
                   "argument must be a format string beginning with '+'"),
                 quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (!format)
    {
      ಆದರೆ (get_resolution)
        format = "%s.%N";
      ಇಲ್ಲದಿದ್ದರೆ
        {
          format = DATE_FMT_LANGINFO ();

          /* Do not wrap the following literal format string with _(...).
             For example, suppose LC_ALL is unset, LC_TIME=POSIX,
             and LANG="ko_KR".  In that case, POSIX says that LC_TIME
             determines the format and contents of date and time strings
             written by date, which means "date" must generate output
             using the POSIX locale; but adding _() would cause "date"
             to use a Korean translation of the format.  */
          ಆದರೆ (! *format)
            format = "%a %b %e %H:%M:%S %Z %Y";
        }
    }

  ಅಕ್ಷರ *format_copy = adjust_resolution (format);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format_res = format_copy ? format_copy : format;
  ಆದರೆ (!tzstring)
    tzstring = getenv ("TZ");
  timezone_t tz = tzalloc (tzstring);

  bool ok = true;
  ಆದರೆ (batch_file != ಖಾಲಿ)
    ok = batch_convert (batch_file, format_res, format_ಒಳ_c_locale,
                        tz, tzstring);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      bool valid_date = true;
      ರಚನೆ timespec when;

      ಆದರೆ (!option_specified_date && !ಗಣ_date)
        {
          ಆದರೆ (optind < argc)
            {
              /* Prepare to set system clock to the specified date/time
                 given in the POSIX-format.  */
              ಗಣ_date = true;
              datestr = argv[optind];
              valid_date = posixtime (&when.tv_sec,
                                      datestr,
                                      (PDS_TRAILING_YEAR
                                       | PDS_CENTURY | PDS_SECONDS));
              when.tv_nsec = 0; /* FIXME: posixtime should set this.  */
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Prepare to print the current date/time.  */
              gettime (&when);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* (option_specified_date || set_date) */
          ಆದರೆ (reference != ಖಾಲಿ)
            {
              ರಚನೆ stat refstats;
              ಆದರೆ (stat (reference, &refstats) != 0)
                error (EXIT_FAILURE, errno, "%s", quotef (reference));
              when = get_stat_mtime (&refstats);
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (get_resolution)
            {
              ದೀರ್ಘ ಪೂರ್ಣಾಂಕ res = gettime_res ();
              when.tv_sec = res / TIMESPEC_HZ;
              when.tv_nsec = res % TIMESPEC_HZ;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಆದರೆ (ಗಣ_datestr)
                datestr = ಗಣ_datestr;
              valid_date = parse_datetime2 (&when, datestr, ಖಾಲಿ,
                                            parse_datetime_flags,
                                            tz, tzstring);
            }
        }

      ಆದರೆ (! valid_date)
        error (EXIT_FAILURE, 0, _("invalid date %s"), quote (datestr));

      ಆದರೆ (ಗಣ_date)
        {
          /* Set the system clock to the specified date, then regardless of
             the success of that operation, format and print that date.  */
          ಆದರೆ (settime (&when) != 0)
            {
              error (0, errno, _("cannot set date"));
              ok = false;
            }
        }

      ok &= show_date_helper (format_res, format_ಒಳ_c_locale, when, tz);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
