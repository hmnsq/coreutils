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

#ضمن <config.h>
#ضمن <stddef.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>

#ضمن "gettext.h"
#تعریف_کرو _(msgid) gettext (msgid)

/* Deactivate config.h's "rpl_"-prefixed definitions, since we don't
   link gnulib here, and the replacements aren't needed.  */
#تعریف_رد fprintf
#تعریف_رد حرر
#تعریف_رد حافظہ_دیو
#تعریف_رد strtoul

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

ساكن فراغ
apply_منوال (FILE *دھارا, حرف ثابت *دھارا_name, حرف ثابت *envvar)
{
  حرف *buf = عدم;
  صحيح setvbuf_منوال;
  بنانشانی لما صحيح ناپ = 0;

  حرف ثابت *منوال_2 = getenv (envvar);
  إذا (!منوال_2)
    أرجع;

  إذا (*منوال_2 == '0')
    setvbuf_منوال = _IONBF;
  وإلا إذا (*منوال_2 == 'L')
    setvbuf_منوال = _IOLBF;      /* FIXME: should we allow 1ML  */
  وإلا
    {
      setvbuf_منوال = _IOFBF;
      حرف *منوال_سرا;
      ناپ = strtoul (منوال_2, &منوال_سرا, 10);
      إذا (ناپ == 0 || *منوال_سرا)
        {
          fprintf (stderr, _("invalid buffering mode %s for %s\n"),
                   منوال_2, دھارا_name);
          أرجع;
        }

      /* If strtoul might have overflowed or if the size is more than
         half of size_t range, treat it as an allocation failure.
         Huge sizes can cause problems with some stdio implementations.  */
      buf = (ناپ <= ((بنانشانی لما صحيح) -2 < (size_t) -1 / 2
                      ? (بنانشانی لما صحيح) -2 : (size_t) -1 / 2)
             ? حافظہ_دیو (ناپ) : عدم);
      إذا (!buf)
        {
          /* We could defer the allocation to libc, however since
             glibc currently ignores the combination of null buffer
             with non zero size, we'll fail here.  */
          fprintf (stderr,
                   _("failed to allocate a %lu byte stdio buffer\n"),
                   ناپ);
          أرجع;
        }
      /* buf will be freed by fclose.  */
    }

  إذا (setvbuf (دھارا, buf, setvbuf_منوال, ناپ) != 0)
    {
      fprintf (stderr, _("could not set buffering of %s to mode %s\n"),
               دھارا_name, منوال_2);
      حرر (buf);
    }
}

/* Use __attribute to avoid elision of __attribute__ on SUNPRO_C etc.  */
ساكن فراغ __attribute ((constructor))
stdbuf (فراغ)
{
  /* Do first so can write errors to stderr.  */
  apply_منوال (stderr, "stderr", "_STDBUF_E");

  apply_منوال (stdin, "stdin", "_STDBUF_I");
  apply_منوال (stdout, "stdout", "_STDBUF_O");
}
