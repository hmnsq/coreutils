#समावेश <config.h>
#समावेश <stdio.h>

#समावेश "system.h"
#समावेश "fprintftime.h"
#समावेश "parse-datetime.h"
#समावेश "quote.h"
#समावेश "show-date.h"
#समावेश "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

बाह्य bool
show_date (अक्षर स्थिर *format, काष्ठा timespec when, timezone_t tz)
{
  काष्ठा पंचांग पंचांग;

  अगर (!localtime_rz (tz, &when.tv_sec, &पंचांग))
    {
      अक्षर buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      वापस false;
    }

  अगर (fprintftime (मानक_निकास, format, &पंचांग, tz, when.tv_nsec) < 0)
    {
      अगर (! ख_त्रुटि (मानक_निकास))  /* otherwise it will be diagnosed later.  */
        error (0, त्रुटि_सं, _("fprintftime error"));
      वापस false;
    }

  वापस true;
}
