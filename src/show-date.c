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
show_date (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format, ರಚನೆ timespec when, timezone_t tz)
{
  ರಚನೆ tm tm;

  ಆದರೆ (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      ಅಕ್ಷರ buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      ಹಿಂತಿರುಗಿ false;
    }

  ಆದರೆ (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      ಆದರೆ (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      ಹಿಂತಿರುಗಿ false;
    }

  ಹಿಂತಿರುಗಿ true;
}
