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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-datetime.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "posixtm.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "show-date.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xsetenv.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "date"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

ਗਿਣਤੀ ਸਮਾਂ_spec
{
  /* Display only the date.  */
  ਸਮਾਂ_SPEC_DATE,
  /* Display date, hours, minutes, and seconds.  */
  ਸਮਾਂ_SPEC_SECONDS,
  /* Similar, but display nanoseconds. */
  ਸਮਾਂ_SPEC_NS,

  /* Put these last, since they aren't valid for --rfc-3339.  */

  /* Display date and hour.  */
  ਸਮਾਂ_SPEC_HOURS,
  /* Display date, hours, and minutes.  */
  ਸਮਾਂ_SPEC_MINUTES
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ ਸਮਾਂ_spec_string[] =
{
  /* Put "hours" and "minutes" first, since they aren't valid for
     --rfc-3339.  */
  "hours", "minutes",
  "date", "seconds", "ns", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ ਸਮਾਂ_spec ਅਚਲ ਸਮਾਂ_spec_2[] =
{
  ਸਮਾਂ_SPEC_HOURS, ਸਮਾਂ_SPEC_MINUTES,
  ਸਮਾਂ_SPEC_DATE, ਸਮਾਂ_SPEC_SECONDS, ਸਮਾਂ_SPEC_NS
};
ARGMATCH_VERIFY (ਸਮਾਂ_spec_string, ਸਮਾਂ_spec_2);

/* A format suitable for Internet RFCs 5322, 2822, and 822.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ rfc_email_format[] = "%a, %d %b %Y %H:%M:%S %z";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  DEBUG_DATE_PARSING_OPTION = CHAR_MAX + 1,
  RESOLUTION_OPTION,
  RFC_3339_OPTION
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ short_options[] = "d:f:I::r:Rs:u";

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"date", required_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"debug", no_ਦਲੀਲ, ਨਿਰਾ, DEBUG_DATE_PARSING_OPTION},
  {"file", required_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"iso-8601", optional_ਦਲੀਲ, ਨਿਰਾ, 'I'},
  {"reference", required_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"resolution", no_ਦਲੀਲ, ਨਿਰਾ, RESOLUTION_OPTION},
  {"rfc-email", no_ਦਲੀਲ, ਨਿਰਾ, 'R'},
  {"rfc-822", no_ਦਲੀਲ, ਨਿਰਾ, 'R'},  /* Deprecated.  */
  {"rfc-2822", no_ਦਲੀਲ, ਨਿਰਾ, 'R'},  /* Deprecated.  */
  {"rfc-3339", required_ਦਲੀਲ, ਨਿਰਾ, RFC_3339_OPTION},
  {"set", required_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"uct", no_ਦਲੀਲ, ਨਿਰਾ, 'u'},  /* Deprecated.  */
  {"utc", no_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"universal", no_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* flags for parse_datetime2 */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ parse_datetime_flags;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _DATE_FMT
# define DATE_FMT_LANGINFO() nl_langinfo (_DATE_FMT)
#ਨਹੀਂਤਾਂ
# define DATE_FMT_LANGINFO() ""
#ਜੇ_ਅੰਤ

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [+FORMAT]\n\
  or:  %s [OPTION]... MMDDhhmm[[CC]YY][.ss]\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Display date and ਸਮਾਂ ਵਿੱਚ the given FORMAT.\n\
With -s, or with MMDDhhmm[[CC]YY][.ss], ਸੈੱਟ_2 the date and ਸਮਾਂ ਪਹਿਲਾ.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --date=STRING\n\
         display ਸਮਾਂ described by STRING, not 'now'\n\
"));
      oputs (_("\
      --debug\n\
         annotate the parsed date,\n\
         and warn about questionable usage ਮੰਜ਼ਿਲ standard error\n\
"));
      oputs (_("\
  -f, --file=DATEFILE\n\
         like --date; once ਲਈ each ਸਤਰ ਦਾ DATEFILE;\n\
         ਜੇ DATEFILE is -, ਪਠ ਨਾਂ from standard ਨਿਵੇਸ਼\n\
"));
      oputs (_("\
  -I[FMT], --iso-8601[=FMT]\n\
         output date/ਸਮਾਂ ਵਿੱਚ ISO 8601 format.\n\
         FMT='date' (ਮੂਲ), 'hours', 'minutes', 'seconds', or 'ns'\n\
         ਲਈ date and ਸਮਾਂ ਮੰਜ਼ਿਲ the indicated precision.\n\
         Example: 2006-08-14T02:34:56-06:00\n\
"));
      oputs (_("\
      --resolution\n\
         output the ਉਪਲਬਧ resolution ਦਾ timestamps.\n\
         Example: 0.000000001\n\
"));
      oputs (_("\
  -R, --rfc-email\n\
         output date and ਸਮਾਂ ਵਿੱਚ RFC 5322 format.\n\
         Example: Mon, 14 Aug 2006 02:34:56 +0000\n\
"));
      oputs (_("\
      --rfc-3339=FMT\n\
         output date/ਸਮਾਂ ਵਿੱਚ RFC 3339 format.\n\
         FMT='date', 'seconds', or 'ns'\n\
         ਲਈ date and ਸਮਾਂ ਮੰਜ਼ਿਲ the indicated precision.\n\
         Example: 2006-08-14 02:34:56-06:00\n\
"));
      oputs (_("\
  -r, --reference=FILE\n\
         display the ਆਖਰੀ modification ਸਮਾਂ ਦਾ FILE\n\
"));
      oputs (_("\
  -s, --ਸੈੱਟ_2=STRING\n\
         ਸੈੱਟ_2 ਸਮਾਂ described by STRING\n\
"));
      oputs (_("\
  -u, --utc, --universal\n\
         print or ਸੈੱਟ_2 Coordinated Universal ਸਮਾਂ_2 (UTC)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
All options that specify the date ਮੰਜ਼ਿਲ display are mutually exclusive.\n\
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
  %C    20          century; like %Y, except omit ਆਖਰੀ two digits\n\
"), stdout);
      fputs (_("\
  %y    99          year (ਆਖਰੀ two digits; 00..99)\n\
"), stdout);
      fputs (_("\
  %Y    1999        year\n\
"), stdout);
      fputs (_("\
  %g    99          year ਦਾ ISO week ਸੰਖਿਆ (ਆਖਰੀ 2 digits; 00-99); see %G\n\
"), stdout);
      fputs (_("\
  %G    1999        year ਦਾ ISO week ਸੰਖਿਆ; normally useful only with %V\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %a    Sun         locale's abbreviated weekday name\n\
"), stdout);
      fputs (_("\
  %A    Sunday      locale's ਭਰਿਆ weekday name\n\
"), stdout);
      fputs (_("\
  %b    Mar         locale's abbreviated month name\n\
"), stdout);
      fputs (_("\
  %h    Mar         same as %b\n\
"), stdout);
      fputs (_("\
  %B    March       locale's ਭਰਿਆ month name\n\
"), stdout);
      fputs (_("\
  %m    12          month (01..12)\n\
"), stdout);
      fputs (_("\
  %d    31          day ਦਾ month (01..31)\n\
"), stdout);
      fputs (_("\
  %e     1          day ਦਾ month, space padded; same as %_d\n\
"), stdout);
      fputs (_("\
  %D    12/31/99    date (ambiguous); same as %m/%d/%y\n\
"), stdout);
      fputs (_("\
  %F    1999-12-31  ਭਰਿਆ date; like %+4Y-%m-%d\n\
"), stdout);
      fputs (_("\
  %x    12/31/99    locale's date (can be ambiguous)\n\
"), stdout);
      fputs (_("\
  %c    ...         locale's date and ਸਮਾਂ (e.g., Thu Mar  3 23:05 2005)\n\
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
  %S    60          ਦੂਜਾ (00..60)\n\
"), stdout);
      fputs (_("\
  %N    123456789   nanoseconds (000000000..999999999)\n\
"), stdout);
      fputs (_("\
  %s    1970010100  seconds since the Epoch (1970-01-01 00:00 UTC)\n\
"), stdout);
      fputs (_("\
  %p    PM          locale's equivalent ਦਾ AM or PM; blank ਜੇ not ਗਿਆਤ\n\
"), stdout);
      fputs (_("\
  %P    am          like %p, but ਹੇਠਲਾ ਹਾਲਤ\n\
"), stdout);
      fputs (_("\
  %r    1:11:04 PM  locale's 12-hour clock ਸਮਾਂ\n\
"), stdout);
      fputs (_("\
  %R    23:59       24-hour hour and minute; same as %H:%M\n\
"), stdout);
      fputs (_("\
  %T    23:59:59    ਸਮਾਂ; same as %H:%M:%S\n\
"), stdout);
      fputs (_("\
  %X    23:59:59    locale's ਸਮਾਂ representation\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %j    365         day ਦਾ year (001..366)\n\
"), stdout);
      fputs (_("\
  %q    4           quarter ਦਾ year (1..4)\n\
"), stdout);
      fputs (_("\
  %u    7           day ਦਾ week (1..7); 1 is Monday\n\
"), stdout);
      fputs (_("\
  %w    6           day ਦਾ week (0..6); 0 is Sunday\n\
"), stdout);
      fputs (_("\
  %U    52          week ਦਾ year; Sunday as ਪਹਿਲਾ day ਦਾ week (00..53)\n\
"), stdout);
      fputs (_("\
  %V    52          ISO week ਸੰਖਿਆ; Monday as ਪਹਿਲਾ day ਦਾ week (01..53)\n\
"), stdout);
      fputs (_("\
  %W    52          week ਦਾ year; Monday as ਪਹਿਲਾ day ਦਾ week (00..53)\n\
"), stdout);

      fputs ("\n", stdout);

      fputs (_("\
  %z     +0400      +hhmm numeric ਸਮਾਂ zone\n\
"), stdout);
      fputs (_("\
  %:z    +04:00     +hh:mm numeric ਸਮਾਂ zone\n\
"), stdout);
      fputs (_("\
  %::z   +04:00:00  +hh:mm:ss numeric ਸਮਾਂ zone\n\
"), stdout);
      fputs (_("\
  %:::z  +04        numeric ਸਮਾਂ zone ਮੰਜ਼ਿਲ necessary precision; with :\n\
"), stdout);
      fputs (_("\
  %Z     EDT        alphabetic ਸਮਾਂ zone abbreviation\n\
"), stdout);
      fputs (_("\
\n\
By ਮੂਲ, date pads numeric fields with zeroes.\n\
"), stdout);
      fputs (_("\
The following optional flags may follow '%':\n\
\n\
  -  (hyphen) ਕਰੋ not pad the ਖੇਤਰ\n\
  _  (underscore) pad with spaces\n\
  0  (zero) pad with zeros\n\
  +  pad with zeros, and put '+' before future years with >4 digits\n\
  ^  use ਉੱਤਲਾ ਹਾਲਤ ਜੇ possible\n\
  #  use opposite ਹਾਲਤ ਜੇ possible\n\
"), stdout);
      fputs (_("\
\n\
After any flags comes an optional ਖੇਤਰ ਚੌੜਾਈ, as a decimal ਸੰਖਿਆ;\n\
then an optional modifier, which is either\n\
E ਮੰਜ਼ਿਲ use the locale's alternate representations ਜੇ ਉਪਲਬਧ, or\n\
O ਮੰਜ਼ਿਲ use the locale's alternate numeric symbols ਜੇ ਉਪਲਬਧ.\n\
"), stdout);
      fputs (_("\
\n\
Examples:\n\
Convert seconds since the Epoch (1970-01-01 UTC) ਮੰਜ਼ਿਲ a date\n\
  $ date --date='@2147483647'\n\
\n\
Show the ਸਮਾਂ on the west coast ਦਾ the US (use tzselect(1) ਮੰਜ਼ਿਲ ਲੱਭੋ TZ)\n\
  $ TZ='America/Los_Angeles' date\n\
\n\
Show the local ਸਮਾਂ ਲਈ 9AM ਅਗਲਾ Friday on the west coast ਦਾ the US\n\
  $ date --date='TZ=\"America/Los_Angeles\" 09:00 next Fri'\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Yield the number of decimal digits needed to output a time with the
   nanosecond resolution RES, without losing information.  */

ATTRIBUTE_CONST
ਸਥਿਰ ਪੂਰਨ
res_ਚੌੜਾਈ (ਲੰਮਾ ਪੂਰਨ res)
{
  ਪੂਰਨ i = 9;
  ਲਈ (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ r = 1; (r *= 10) <= res; )
    i--;
  ਵਾਪਸ i;
}

/* Return a newly allocated copy of FORMAT with each "%-N" adjusted to
   be "%9N", "%6N", or whatever other resolution is appropriate for
   the current platform.  If no "%-N" appears, return NULL.  */

ਸਥਿਰ ਅੱਖਰ *
adjust_resolution (ਅੱਖਰ ਅਚਲ *format)
{
  ਅੱਖਰ *copy = ਨਿਰਾ;

  ਲਈ (ਅੱਖਰ ਅਚਲ *f = format; *f; f++)
    ਜੇ (f[0] == '%')
      {
        ਜੇ (f[1] == '-' && f[2] == 'N')
          {
            ਜੇ (!copy)
              copy = xstrdup (format);
            copy[f + 1 - format] = '0' + res_ਚੌੜਾਈ (gettime_res ());
            f += 2;
          }
        ਨਹੀਂਤਾਂ
          f += f[1] == '%';
      }

  ਵਾਪਸ copy;
}

/* Set the LC_TIME category of the current locale.
   Return the previous value of the LC_TIME category, as a freshly allocated
   string or null.  */

ਸਥਿਰ ਅੱਖਰ *
ਸੈੱਟ_LC_ਸਮਾਂ (ਅੱਖਰ ਅਚਲ *locale)
{
  /* It is not sufficient to do
       setlocale (LC_TIME, locale);
     because show_date relies on fprintftime, that uses the Gnulib module
     'localename-unsafe', that looks at the values of the environment variables
     (in order to distinguish the default locale from the C locale on platforms
     like macOS).  */
  ਅੱਖਰ ਅਚਲ *all = getenv ("LC_ALL");
  ਜੇ (all != ਨਿਰਾ && *all != '\0')
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
  ਅੱਖਰ ਅਚਲ *ਮੁੱਲ = getenv ("LC_TIME");
  ਅੱਖਰ *ret = (ਮੁੱਲ == ਨਿਰਾ || *ਮੁੱਲ == '\0' ? ਨਿਰਾ : xstrdup (ਮੁੱਲ));
  ਜੇ (locale != ਨਿਰਾ)
    xsetenv ("LC_TIME", locale, 1);
  ਨਹੀਂਤਾਂ
    unsetenv ("LC_TIME");

  /* Update the current locale accordingly.  */
  setlocale (LC_ਸਮਾਂ, "");

  ਵਾਪਸ ret;
}

ਸਥਿਰ bool
show_date_helper (ਅੱਖਰ ਅਚਲ *format, bool use_c_locale,
                  ਬਣਤਰ timespec when, timezone_t tz)
{
  ਜੇ (parse_datetime_flags & PARSE_DATETIME_DEBUG)
    error (0, 0, _("output format: %s"), quote (format));

  bool ok;
  ਜੇ (use_c_locale)
    {
      ਅੱਖਰ *old_locale_category = ਸੈੱਟ_LC_ਸਮਾਂ ("C");
      ok = show_date (format, when, tz);
      ਅੱਖਰ *new_locale_category = ਸੈੱਟ_LC_ਸਮਾਂ (old_locale_category);
      ਮੁਕਤ (new_locale_category);
      ਮੁਕਤ (old_locale_category);
    }
  ਨਹੀਂਤਾਂ
    ok = show_date (format, when, tz);

  ਅੱਖਰ_ਲਿਖੋ ('\n');
  ਵਾਪਸ ok;
}

/* Parse each line in INPUT_FILENAME as with --date and display each
   resulting time and date.  If the file cannot be opened, tell why
   then exit.  Issue a diagnostic for any lines that cannot be parsed.
   Return true if successful.  */

ਸਥਿਰ bool
batch_convert (ਅੱਖਰ ਅਚਲ *ਨਿਵੇਸ਼_filename,
               ਅੱਖਰ ਅਚਲ *format, bool format_ਵਿੱਚ_c_locale,
               timezone_t tz, ਅੱਖਰ ਅਚਲ *tzstring)
{
  FILE *ਵਿੱਚ_ਧਾਰਾ;

  ਜੇ (streq (ਨਿਵੇਸ਼_filename, "-"))
    {
      ਨਿਵੇਸ਼_filename = _("standard input");
      ਵਿੱਚ_ਧਾਰਾ = stdin;
    }
  ਨਹੀਂਤਾਂ
    {
      ਵਿੱਚ_ਧਾਰਾ = fopen (ਨਿਵੇਸ਼_filename, "r");
      ਜੇ (ਵਿੱਚ_ਧਾਰਾ == ਨਿਰਾ)
        error (EXIT_FAILURE, errno, "%s", quotef (ਨਿਵੇਸ਼_filename));
    }

  ਅੱਖਰ *ਸਤਰ = ਨਿਰਾ;
  size_t buflen = 0;
  bool ok = true;
  ਜਦੋਂ (true)
    {
      ssize_t ਸਤਰ_ਲੰਮਾਈ = getline (&ਸਤਰ, &buflen, ਵਿੱਚ_ਧਾਰਾ);
      ਜੇ (ਸਤਰ_ਲੰਮਾਈ < 0)
        {
          ਜੇ (ferror (ਵਿੱਚ_ਧਾਰਾ))
            error (EXIT_FAILURE, errno, _("%s: read error"),
                   quotef (ਨਿਵੇਸ਼_filename));
          ਰੋਕੋ;
        }

      ਬਣਤਰ timespec when;
      ਜੇ (! parse_datetime2 (&when, ਸਤਰ, ਨਿਰਾ,
                             parse_datetime_flags, tz, tzstring))
        {
          ਜੇ (ਸਤਰ[ਸਤਰ_ਲੰਮਾਈ - 1] == '\n')
            ਸਤਰ[ਸਤਰ_ਲੰਮਾਈ - 1] = '\0';
          error (0, 0, _("invalid date %s"), quote (ਸਤਰ));
          ok = false;
        }
      ਨਹੀਂਤਾਂ
        {
          ok &= show_date_helper (format, format_ਵਿੱਚ_c_locale, when, tz);
        }

      ਜੇ (ferror (stdout))
        write_error ();
    }

  ਜੇ (fclose (ਵਿੱਚ_ਧਾਰਾ) == EOF)
    error (EXIT_FAILURE, errno, "%s", quotef (ਨਿਵੇਸ਼_filename));

  ਮੁਕਤ (ਸਤਰ);

  ਵਾਪਸ ok;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ ਅਚਲ *datestr = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *ਸੈੱਟ_datestr = ਨਿਰਾ;
  bool ਸੈੱਟ_date = false;
  ਅੱਖਰ ਅਚਲ *format = ਨਿਰਾ;
  bool format_ਵਿੱਚ_c_locale = false;
  bool get_resolution = false;
  ਅੱਖਰ *batch_file = ਨਿਰਾ;
  ਅੱਖਰ *reference = ਨਿਰਾ;
  bool discarded_datestr = false;
  bool discarded_ਸੈੱਟ_datestr = false;
  ਅੱਖਰ ਅਚਲ *tzstring = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਪੂਰਨ optc;
  ਜਦੋਂ ((optc = getopt_long (argc, argv, short_options, long_options, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'd':
          ਜੇ (datestr)
            discarded_datestr = true;
          datestr = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ DEBUG_DATE_PARSING_OPTION:
          parse_datetime_flags |= PARSE_DATETIME_DEBUG;
          ਰੋਕੋ;
        ਹਾਲਤ 'f':
          batch_file = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ RESOLUTION_OPTION:
          get_resolution = true;
          ਰੋਕੋ;
        ਹਾਲਤ RFC_3339_OPTION:
          {
            ਸਥਿਰ ਅੱਖਰ ਅਚਲ rfc_3339_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%d %H:%M:%S%:z",
                "%Y-%m-%d %H:%M:%S.%N%:z"
              };
            ਗਿਣਤੀ ਸਮਾਂ_spec i =
              XARGMATCH ("--rfc-3339", optarg,
                         ਸਮਾਂ_spec_string + 2, ਸਮਾਂ_spec_2 + 2);
            format = rfc_3339_format[i];
            format_ਵਿੱਚ_c_locale = true;
            ਰੋਕੋ;
          }
        ਹਾਲਤ 'I':
          {
            ਸਥਿਰ ਅੱਖਰ ਅਚਲ iso_8601_format[][32] =
              {
                "%Y-%m-%d",
                "%Y-%m-%dT%H:%M:%S%:z",
                "%Y-%m-%dT%H:%M:%S,%N%:z",
                "%Y-%m-%dT%H%:z",
                "%Y-%m-%dT%H:%M%:z"
              };
            ਗਿਣਤੀ ਸਮਾਂ_spec i =
              (optarg
               ? XARGMATCH ("--iso-8601", optarg, ਸਮਾਂ_spec_string, ਸਮਾਂ_spec_2)
               : ਸਮਾਂ_SPEC_DATE);
            format = iso_8601_format[i];
            format_ਵਿੱਚ_c_locale = true;
            ਰੋਕੋ;
          }
        ਹਾਲਤ 'r':
          reference = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'R':
          format = rfc_email_format;
          format_ਵਿੱਚ_c_locale = true;
          ਰੋਕੋ;
        ਹਾਲਤ 's':
          ਜੇ (ਸੈੱਟ_datestr)
            discarded_ਸੈੱਟ_datestr = true;
          ਸੈੱਟ_datestr = optarg;
          ਸੈੱਟ_date = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'u':
          tzstring = "UTC0";
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਪੂਰਨ option_specified_date = (!!datestr + !!batch_file + !!reference
                               + get_resolution);

  ਜੇ (option_specified_date > 1)
    {
      error (0, 0,
        _("the options to specify dates for printing are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (ਸੈੱਟ_date && option_specified_date)
    {
      error (0, 0,
          _("the options to print and set the time may not be used together"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (discarded_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -d options"));

  ਜੇ (discarded_ਸੈੱਟ_datestr && (parse_datetime_flags & PARSE_DATETIME_DEBUG))
    error (0, 0, _("only using last of multiple -s options"));

  ਜੇ (optind < argc)
    {
      ਜੇ (optind + 1 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          usage (EXIT_FAILURE);
        }

      ਜੇ (argv[optind][0] == '+')
        {
          ਜੇ (format)
            error (EXIT_FAILURE, 0, _("multiple output formats specified"));
          format = argv[optind++] + 1;
        }
      ਨਹੀਂਤਾਂ ਜੇ (ਸੈੱਟ_date || option_specified_date)
        {
          error (0, 0,
                 _("the argument %s lacks a leading '+';\n"
                   "when using an option to specify date(s), any non-option\n"
                   "argument must be a format string beginning with '+'"),
                 quote (argv[optind]));
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (!format)
    {
      ਜੇ (get_resolution)
        format = "%s.%N";
      ਨਹੀਂਤਾਂ
        {
          format = DATE_FMT_LANGINFO ();

          /* Do not wrap the following literal format string with _(...).
             For example, suppose LC_ALL is unset, LC_TIME=POSIX,
             and LANG="ko_KR".  In that case, POSIX says that LC_TIME
             determines the format and contents of date and time strings
             written by date, which means "date" must generate output
             using the POSIX locale; but adding _() would cause "date"
             to use a Korean translation of the format.  */
          ਜੇ (! *format)
            format = "%a %b %e %H:%M:%S %Z %Y";
        }
    }

  ਅੱਖਰ *format_copy = adjust_resolution (format);
  ਅੱਖਰ ਅਚਲ *format_res = format_copy ? format_copy : format;
  ਜੇ (!tzstring)
    tzstring = getenv ("TZ");
  timezone_t tz = tzalloc (tzstring);

  bool ok = true;
  ਜੇ (batch_file != ਨਿਰਾ)
    ok = batch_convert (batch_file, format_res, format_ਵਿੱਚ_c_locale,
                        tz, tzstring);
  ਨਹੀਂਤਾਂ
    {
      bool valid_date = true;
      ਬਣਤਰ timespec when;

      ਜੇ (!option_specified_date && !ਸੈੱਟ_date)
        {
          ਜੇ (optind < argc)
            {
              /* Prepare to set system clock to the specified date/time
                 given in the POSIX-format.  */
              ਸੈੱਟ_date = true;
              datestr = argv[optind];
              valid_date = posixtime (&when.tv_sec,
                                      datestr,
                                      (PDS_TRAILING_YEAR
                                       | PDS_CENTURY | PDS_SECONDS));
              when.tv_nsec = 0; /* FIXME: posixtime should set this.  */
            }
          ਨਹੀਂਤਾਂ
            {
              /* Prepare to print the current date/time.  */
              gettime (&when);
            }
        }
      ਨਹੀਂਤਾਂ
        {
          /* (option_specified_date || set_date) */
          ਜੇ (reference != ਨਿਰਾ)
            {
              ਬਣਤਰ stat refstats;
              ਜੇ (stat (reference, &refstats) != 0)
                error (EXIT_FAILURE, errno, "%s", quotef (reference));
              when = get_stat_mtime (&refstats);
            }
          ਨਹੀਂਤਾਂ ਜੇ (get_resolution)
            {
              ਲੰਮਾ ਪੂਰਨ res = gettime_res ();
              when.tv_sec = res / TIMESPEC_HZ;
              when.tv_nsec = res % TIMESPEC_HZ;
            }
          ਨਹੀਂਤਾਂ
            {
              ਜੇ (ਸੈੱਟ_datestr)
                datestr = ਸੈੱਟ_datestr;
              valid_date = parse_datetime2 (&when, datestr, ਨਿਰਾ,
                                            parse_datetime_flags,
                                            tz, tzstring);
            }
        }

      ਜੇ (! valid_date)
        error (EXIT_FAILURE, 0, _("invalid date %s"), quote (datestr));

      ਜੇ (ਸੈੱਟ_date)
        {
          /* Set the system clock to the specified date, then regardless of
             the success of that operation, format and print that date.  */
          ਜੇ (settime (&when) != 0)
            {
              error (0, errno, _("cannot set date"));
              ok = false;
            }
        }

      ok &= show_date_helper (format_res, format_ਵਿੱਚ_c_locale, when, tz);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
