#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "fprintftime.h"
#അന്തര്ഭാവ "parse-datetime.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "show-date.h"
#അന്തര്ഭാവ "stat-time.h"

/* Display the date and/or time in WHEN according to the format specified
   in FORMAT, followed by a newline.

   If successful, return true.
   If unsuccessful, prints an error message to STDERR and returns false.
   If unsuccessful and ON_ERROR_PRINT_UNFORMATTED, also prints WHEN.TV_SEC
   to STDOUT.  */

ബാഹ്യ bool
show_date (അക്ഷരം സ്ഥിരം *format, ഘടന timespec when, timezone_t tz)
{
  ഘടന tm tm;

  എങ്കിൽ (!localtime_rz (tz, &when.tv_sec, &tm))
    {
      അക്ഷരം buf[INT_BUFSIZE_BOUND (intmax_t)];
      error (0, 0, _("time %s is out of range"),
             quote (timetostr (when.tv_sec, buf)));
      തിരികെ false;
    }

  എങ്കിൽ (fprintftime (stdout, format, &tm, tz, when.tv_nsec) < 0)
    {
      എങ്കിൽ (! ferror (stdout))  /* otherwise it will be diagnosed later.  */
        error (0, errno, _("fprintftime error"));
      തിരികെ false;
    }

  തിരികെ true;
}
