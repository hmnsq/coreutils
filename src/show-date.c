#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "fprintftime.h"
#ಅನ್ತರ್ಭಾವ "parse-datetime.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "show-date.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

ಬಾಹ್ಯ bool
show_date (ವರ್ಣ ಧ್ರುವ *format, ಸಂರಚನಾ timespec when, timezone_t tz)
{
  ಸಂರಚನಾ tm tm;

  ಯದಿ (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      ವರ್ಣ buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಯದಿ (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      ಯದಿ (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಪ್ರತ್ಯಾಗಮ true;
}
