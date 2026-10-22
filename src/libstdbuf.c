/* libstdbuf -- a shared lib to preload to setup stdio buffering for a command
   Copyright (C) 2009-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Written by Pádraig Brady.  LD_PRELOAD idea from Brian Dessent.  */

#అన్తర్భావ <config.h>
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>

#అన్తర్భావ "gettext.h"
#నిర్వచించు _(msgid) gettext (msgid)

/* Deactivate config.h's "rpl_"-prefixed definitions, since we don't
   link gnulib here, and the replacements aren't needed.  */
#నిర్వచనం_రద్దు fprintf
#నిర్వచనం_రద్దు విడుదల
#నిర్వచనం_రద్దు జ్ఞాపకం_ఇవ్వు
#నిర్వచనం_రద్దు strtoul

/* Note currently for glibc (2.3.5) the following call does not change
   the buffer size, and more problematically does not give any indication
   that the new size request was ignored:

       setvbuf (stdout, NULL, _IOFBF, 8192);

   The ISO C99 standard section 7.19.5.6 on the setvbuf function says:

   ... If buf is not a null pointer, the array it points to _may_ be used
   instead of a buffer allocated by the setvbuf function and the argument
   size specifies the size of the array; otherwise, size _may_ determine
   the size of a buffer allocated by the setvbuf function. ...

   Obviously some interpret the above to mean setvbuf (..., size)
   is only a hint from the application which I don't agree with.

   FreeBSD's libc seems more sensible in this regard. From the man page:

   The size argument may be given as zero to obtain deferred optimal-size
   buffer allocation as usual.  If it is not zero, then except for
   unbuffered files, the buf argument should point to a buffer at least size
   bytes long; this buffer will be used instead of the current buffer.  (If
   the size argument is not zero but buf is null, a buffer of the given size
   will be allocated immediately, and released on close.  This is an extension
   to ANSI C; portable code should use a size of 0 with any null buffer.)
   --------------------
   Another issue is that on glibc-2.7 the following doesn't buffer
   the first write if it's greater than 1 byte.

       setvbuf (stdout, buf, _IOFBF, 127);

   Now the POSIX standard says that "allocating a buffer of size bytes does
   not necessarily imply that all of size bytes are used for the buffer area".
   However I think it's just a buggy implementation due to the various
   inconsistencies with write sizes and subsequent writes.  */

స్థిర శూన్య
apply_బహుళకం (FILE *ప్రవాహం, అక్షరం స్థిరరాశి *ప్రవాహం_name, అక్షరం స్థిరరాశి *envvar)
{
  అక్షరం *buf = ఖాళీ;
  పూర్ణాంకం setvbuf_బహుళకం;
  గుర్తులేని దీర్ఘ పూర్ణాంకం కొలత = 0;

  అక్షరం స్థిరరాశి *బహుళకం_2 = getenv (envvar);
  ఒకవేళ (!బహుళకం_2)
    తిరిగి;

  ఒకవేళ (*బహుళకం_2 == '0')
    setvbuf_బహుళకం = _IONBF;
  లేకపోతే ఒకవేళ (*బహుళకం_2 == 'L')
    setvbuf_బహుళకం = _IOLBF;      /* FIXME: should we allow 1ML  */
  లేకపోతే
    {
      setvbuf_బహుళకం = _IOFBF;
      అక్షరం *బహుళకం_చివర;
      కొలత = strtoul (బహుళకం_2, &బహుళకం_చివర, 10);
      ఒకవేళ (కొలత == 0 || *బహుళకం_చివర)
        {
          fprintf (stderr, _("invalid buffering mode %s for %s\n"),
                   బహుళకం_2, ప్రవాహం_name);
          తిరిగి;
        }

      /* If strtoul might have overflowed or if the size is more than
         half of size_t range, treat it as an allocation failure.
         Huge sizes can cause problems with some stdio implementations.  */
      buf = (కొలత <= ((గుర్తులేని దీర్ఘ పూర్ణాంకం) -2 < (size_t) -1 / 2
                      ? (గుర్తులేని దీర్ఘ పూర్ణాంకం) -2 : (size_t) -1 / 2)
             ? జ్ఞాపకం_ఇవ్వు (కొలత) : ఖాళీ);
      ఒకవేళ (!buf)
        {
          /* We could defer the allocation to libc, however since
             glibc currently ignores the combination of null buffer
             with non zero size, we'll fail here.  */
          fprintf (stderr,
                   _("failed to allocate a %lu byte stdio buffer\n"),
                   కొలత);
          తిరిగి;
        }
      /* buf will be freed by fclose.  */
    }

  ఒకవేళ (setvbuf (ప్రవాహం, buf, setvbuf_బహుళకం, కొలత) != 0)
    {
      fprintf (stderr, _("could not set buffering of %s to mode %s\n"),
               ప్రవాహం_name, బహుళకం_2);
      విడుదల (buf);
    }
}

/* Use __attribute to avoid elision of __attribute__ on SUNPRO_C etc.  */
స్థిర శూన్య __attribute ((constructor))
stdbuf (శూన్య)
{
  /* Do first so can write errors to stderr.  */
  apply_బహుళకం (stderr, "stderr", "_STDBUF_E");

  apply_బహుళకం (stdin, "stdin", "_STDBUF_I");
  apply_బహుళకం (stdout, "stdout", "_STDBUF_O");
}
