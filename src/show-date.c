#ضمن <config.h>
#ضمن <stdio.h>

#ضمن "system.h"
#ضمن "fprintftime.h"
#ضمن "parse-datetime.h"
#ضمن "quote.h"
#ضمن "show-date.h"
#ضمن "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

خارجي bool
show_date (حرف ثابت *format, بنية timespec when, timezone_t tz)
{
  بنية tm tm;

  𐡄𐡍 (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      حرف buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      أرجع false;
    }

  𐡄𐡍 (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      𐡄𐡍 (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      أرجع false;
    }

  أرجع true;
}
