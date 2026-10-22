#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "fprintftime.h"
#ଅନ୍ତର୍ଭାଵ "parse-datetime.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "show-date.h"
#ଅନ୍ତର୍ଭାଵ "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

ବାହ୍ୟ bool
show_date (ଅକ୍ଷର ଅଚଳ *format, ସଂରଚନା timespec when, timezone_t tz)
{
  ସଂରଚନା tm tm;

  ଯଦି (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      ଅକ୍ଷର buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      ଫେରନ୍ତୁ false;
    }

  ଯଦି (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      ଯଦି (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      ଫେରନ୍ତୁ false;
    }

  ଫେରନ୍ତୁ true;
}
