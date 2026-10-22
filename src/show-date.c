#شامل <config.h>
#شامل <stdio.h>

#شامل "system.h"
#شامل "fprintftime.h"
#شامل "parse-datetime.h"
#شامل "quote.h"
#شامل "show-date.h"
#شامل "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

بیرونی bool
show_date (نویسه ثابت *format, ساختار timespec when, timezone_t tz)
{
  ساختار tm tm;

  اگر (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      نویسه buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      بازگشت false;
    }

  اگر (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      اگر (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      بازگشت false;
    }

  بازگشت true;
}
