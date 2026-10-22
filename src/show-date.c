#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "fprintftime.h"
#अन्तर्भाव "parse-datetime.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "show-date.h"
#अन्तर्भाव "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

बाह्य bool
show_date (अक्षर स्थिरांक *format, संरचना timespec when, timezone_t tz)
{
  संरचना पंचांग पंचांग;

  जर (!localtime_rz (tz, &when.tv_sec, &पंचांग))
    {
      अक्षर buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      परत false;
    }

  जर (fprintftime (मानक_निकास, format, &पंचांग, tz, when.tv_nsec) < 0)
    {
      जर (! ख_त्रुटि (मानक_निकास))  /* otherwise it will be diagnosed later.  */
        error (0, त्रुटि_सं, _("fprintftime error"));
      परत false;
    }

  परत true;
}
