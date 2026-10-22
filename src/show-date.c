#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "fprintftime.h"
#অন্তর্ভাব "parse-datetime.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "show-date.h"
#অন্তর্ভাব "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

বাহ্য bool
show_date (বর্ণ ধ্রুব *format, সংরচনা timespec when, timezone_t tz)
{
  সংরচনা tm tm;

  যদি (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      বর্ণ buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      প্রত্যাগম false;
    }

  যদি (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      যদি (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      প্রত্যাগম false;
    }

  প্রত্যাগম true;
}
