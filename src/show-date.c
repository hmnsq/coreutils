#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "fprintftime.h"
#அந்தர்பாவ "parse-datetime.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "show-date.h"
#அந்தர்பாவ "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

வெளி bool
show_date (எழுத்து மாறிலி *format, கட்டமைப்பு timespec when, timezone_t tz)
{
  கட்டமைப்பு tm tm;

  எனில் (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      எழுத்து buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      திரும்பு false;
    }

  எனில் (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      எனில் (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      திரும்பு false;
    }

  திரும்பு true;
}
