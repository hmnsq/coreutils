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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "gettext.h"
#define _(msgid) gettext (msgid)

/* Deactivate config.h's "rpl_"-prefixed definitions, since we don't
   link gnulib here, and the replacements aren't needed.  */
#undef fprintf
#undef ಮುಕ್ತ
#undef malloc
#undef strtoul

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

ಸ್ಥಿರ ಶೂನ್ಯ
apply_mode (FILE *stream, ವರ್ಣ ಧ್ರುವ *stream_name, ವರ್ಣ ಧ್ರುವ *envvar)
{
  ವರ್ಣ *buf = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ setvbuf_mode;
  unsigned long ಪೂರ್ಣಾಙ್ಕ size = 0;

  ವರ್ಣ ಧ್ರುವ *mode = getenv (envvar);
  ಯದಿ (!mode)
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (*mode == '0')
    setvbuf_mode = _IONBF;
  ಅನ್ಯಥಾ ಯದಿ (*mode == 'L')
    setvbuf_mode = _IOLBF;      /* FIXME: should we allow 1ML  */
  ಅನ್ಯಥಾ
    {
      setvbuf_mode = _IOFBF;
      ವರ್ಣ *mode_end;
      size = strtoul (mode, &mode_end, 10);
      ಯದಿ (size == 0 || *mode_end)
        {
          fprintf (stderr, _("invalid buffering mode %s for %s\n"),
                   mode, stream_name);
          ಪ್ರತ್ಯಾಗಮ;
        }

      /* If strtoul might have overflowed or if the size is more than
         half of size_t range, treat it as an allocation failure.
         Huge sizes can cause problems with some stdio implementations.  */
      buf = (size <= ((unsigned long ಪೂರ್ಣಾಙ್ಕ) -2 < (size_t) -1 / 2
                      ? (unsigned long ಪೂರ್ಣಾಙ್ಕ) -2 : (size_t) -1 / 2)
             ? malloc (size) : ರಿಕ್ತ);
      ಯದಿ (!buf)
        {
          /* We could defer the allocation to libc, however since
             glibc currently ignores the combination of null buffer
             with non zero size, we'll fail here.  */
          fprintf (stderr,
                   _("failed to allocate a %lu byte stdio buffer\n"),
                   size);
          ಪ್ರತ್ಯಾಗಮ;
        }
      /* buf will be freed by fclose.  */
    }

  ಯದಿ (setvbuf (stream, buf, setvbuf_mode, size) != 0)
    {
      fprintf (stderr, _("could not set buffering of %s to mode %s\n"),
               stream_name, mode);
      ಮುಕ್ತ (buf);
    }
}

/* Use __attribute to avoid elision of __attribute__ on SUNPRO_C etc.  */
ಸ್ಥಿರ ಶೂನ್ಯ __attribute ((constructor))
stdbuf (ಶೂನ್ಯ)
{
  /* Do first so can write errors to stderr.  */
  apply_mode (stderr, "stderr", "_STDBUF_E");

  apply_mode (stdin, "stdin", "_STDBUF_I");
  apply_mode (stdout, "stdout", "_STDBUF_O");
}
