#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "fprintftime.h"
#અન્તર્ભાવ "parse-datetime.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "show-date.h"
#અન્તર્ભાવ "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

બાહ્ય bool
show_date (અક્ષર અચલ *format, સંરચના timespec when, timezone_t tz)
{
  સંરચના tm tm;

  જો (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      અક્ષર buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      પાછા false;
    }

  જો (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      જો (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      પાછા false;
    }

  પાછા true;
}
