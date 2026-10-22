#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fprintftime.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-datetime.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "show-date.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

ਬਾਹਰੀ bool
show_date (ਅੱਖਰ ਅਚਲ *format, ਬਣਤਰ timespec when, timezone_t tz)
{
  ਬਣਤਰ tm tm;

  ਜੇ (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      ਅੱਖਰ buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      ਵਾਪਸ false;
    }

  ਜੇ (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      ਜੇ (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      ਵਾਪਸ false;
    }

  ਵਾਪਸ true;
}
