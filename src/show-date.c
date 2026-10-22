#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "fprintftime.h"
#অন্তর্ভাৱ "parse-datetime.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "show-date.h"
#অন্তর্ভাৱ "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

বাহ্য bool
show_date (ৱর্ণ ধ্রুৱ *format, সংরচনা timespec when, timezone_t tz)
{
  সংরচনা tm tm;

  যদি (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      ৱর্ণ buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      ঘূৰাই false;
    }

  যদি (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      যদি (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      ঘূৰাই false;
    }

  ঘূৰাই true;
}
