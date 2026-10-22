#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "fprintftime.h"
#అన్తర్భావ "parse-datetime.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "show-date.h"
#అన్తర్భావ "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

బాహ్య bool
show_date (అక్షరం స్థిరరాశి *format, నిర్మాణం timespec when, timezone_t tz)
{
  నిర్మాణం tm tm;

  ఒకవేళ (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      అక్షరం buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      తిరిగి false;
    }

  ఒకవేళ (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      ఒకవేళ (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      తిరిగి false;
    }

  తిరిగి true;
}
