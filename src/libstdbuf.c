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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _(msgid) gettext (msgid)

/* Deactivate config.h's "rpl_"-prefixed definitions, since we don't
   link gnulib here, and the replacements aren't needed.  */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ fprintf
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਮੁਕਤ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਸਮ੍ਰਿਤੀ_ਦਿਓ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ strtoul

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

ਸਥਿਰ ਖਾਲੀ
apply_ਬਹੁਲਕ (FILE *ਧਾਰਾ, ਅੱਖਰ ਅਚਲ *ਧਾਰਾ_name, ਅੱਖਰ ਅਚਲ *envvar)
{
  ਅੱਖਰ *buf = ਨਿਰਾ;
  ਪੂਰਨ setvbuf_ਬਹੁਲਕ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ ਮਾਪ = 0;

  ਅੱਖਰ ਅਚਲ *ਬਹੁਲਕ_2 = getenv (envvar);
  ਜੇ (!ਬਹੁਲਕ_2)
    ਵਾਪਸ;

  ਜੇ (*ਬਹੁਲਕ_2 == '0')
    setvbuf_ਬਹੁਲਕ = _IONBF;
  ਨਹੀਂਤਾਂ ਜੇ (*ਬਹੁਲਕ_2 == 'L')
    setvbuf_ਬਹੁਲਕ = _IOLBF;      /* FIXME: should we allow 1ML  */
  ਨਹੀਂਤਾਂ
    {
      setvbuf_ਬਹੁਲਕ = _IOFBF;
      ਅੱਖਰ *ਬਹੁਲਕ_ਸਿਰਾ;
      ਮਾਪ = strtoul (ਬਹੁਲਕ_2, &ਬਹੁਲਕ_ਸਿਰਾ, 10);
      ਜੇ (ਮਾਪ == 0 || *ਬਹੁਲਕ_ਸਿਰਾ)
        {
          fprintf (stderr, _("invalid buffering mode %s for %s\n"),
                   ਬਹੁਲਕ_2, ਧਾਰਾ_name);
          ਵਾਪਸ;
        }

      /* If strtoul might have overflowed or if the size is more than
         half of size_t range, treat it as an allocation failure.
         Huge sizes can cause problems with some stdio implementations.  */
      buf = (ਮਾਪ <= ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) -2 < (size_t) -1 / 2
                      ? (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) -2 : (size_t) -1 / 2)
             ? ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਮਾਪ) : ਨਿਰਾ);
      ਜੇ (!buf)
        {
          /* We could defer the allocation to libc, however since
             glibc currently ignores the combination of null buffer
             with non zero size, we'll fail here.  */
          fprintf (stderr,
                   _("failed to allocate a %lu byte stdio buffer\n"),
                   ਮਾਪ);
          ਵਾਪਸ;
        }
      /* buf will be freed by fclose.  */
    }

  ਜੇ (setvbuf (ਧਾਰਾ, buf, setvbuf_ਬਹੁਲਕ, ਮਾਪ) != 0)
    {
      fprintf (stderr, _("could not set buffering of %s to mode %s\n"),
               ਧਾਰਾ_name, ਬਹੁਲਕ_2);
      ਮੁਕਤ (buf);
    }
}

/* Use __attribute to avoid elision of __attribute__ on SUNPRO_C etc.  */
ਸਥਿਰ ਖਾਲੀ __attribute ((constructor))
stdbuf (ਖਾਲੀ)
{
  /* Do first so can write errors to stderr.  */
  apply_ਬਹੁਲਕ (stderr, "stderr", "_STDBUF_E");

  apply_ਬਹੁਲਕ (stdin, "stdin", "_STDBUF_I");
  apply_ਬਹੁਲਕ (stdout, "stdout", "_STDBUF_O");
}
