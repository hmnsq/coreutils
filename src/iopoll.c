/* iopoll.c -- broken pipe detection / non blocking output handling
   Copyright (C) 2022-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

   Written by Carl Edquist in collaboration with Arsen Arsenović.  */

#અન્તર્ભાવ <config.h>

/* poll(2) is needed on AIX (where 'select' gives a readable
   event immediately) and Solaris (where 'select' never gave
   a readable event).  Also use poll(2) on systems we know work
   and/or are already using poll (linux).  On macOS we know poll()
   doesn't work on 10.5, while it does work on >= 11 at least. */
#જો defined _AIX || defined __sun \
    || defined __linux__ || defined __ANDROID__ \
    || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 110000
# define IOPOLL_USES_POLL 1
  /* Check we've not enabled gnulib's poll module
     as that will emulate poll() in a way not
     currently compatible with our usage.  */
# જો defined HAVE_POLL
#  error "gnulib's poll() replacement is currently incompatible"
# endif
#endif

#જો IOPOLL_USES_POLL
# include <poll.h>
#નહીંતર
# include <sys/select.h>
#endif

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "iopoll.h"
#અન્તર્ભાવ "isapipe.h"


/* BROKEN_OUTPUT selects the mode of operation of this function.
   If BROKEN_OUTPUT, wait for FDIN to become ready for reading
   or FDOUT to become a broken pipe.
   If !BROKEN_OUTPUT, wait for FDIN or FDOUT to become ready for writing.
   If either of those are -1, then they're not checked.  Set BLOCK to true
   to wait for an event, otherwise return the status immediately.
   Return 0 if not BLOCKing and there is no event on the requested descriptors.
   Return 0 if FDIN can be read() without blocking, or IOPOLL_BROKEN_OUTPUT if
   FDOUT becomes a broken pipe. If !BROKEN_OUTPUT return 0 if FDOUT writable.
   Otherwise return IOPOLL_ERROR if there is a poll() or select() error.  */

સ્થિર પૂર્ણાંક
iopoll_internal (પૂર્ણાંક fdin, પૂર્ણાંક fdout, bool block, bool broken_output)
{
  affirm (fdin != -1 || fdout != -1);

#જો IOPOLL_USES_POLL
  સંરચના pollfd pfds[2] = {  /* POLLRDBAND needed for illumos, macOS.  */
    { .fd = fdin,  .events = POLLIN | POLLRDBAND, .revents = 0 },
    { .fd = fdout, .events = POLLRDBAND, .revents = 0 },
  };
  પૂર્ણાંક check_out_events = POLLERR | POLLHUP | POLLNVAL;
  પૂર્ણાંક ret = 0;

  જો (! broken_output)
    {
      pfds[0].events = pfds[1].events = POLLOUT;
      check_out_events = POLLOUT;
    }

  જ્યારે (0 <= ret || errno == EINTR)
    {
      ret = poll (pfds, 2, block ? -1 : 0);

      જો (ret < 0)
        ચાલુ;
      જો (ret == 0 && ! block)
        પાછા 0;
      affirm (0 < ret);
      જો (pfds[0].revents) /* input available or pipe closed indicating EOF; */
        પાછા 0;          /* should now be able to read() without blocking  */
      જો (pfds[1].revents & check_out_events)
        પાછા broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#નહીંતર  /* fall back to select()-based implementation */

  પૂર્ણાંક nfds = (fdin > fdout ? fdin : fdout) + 1;
  પૂર્ણાંક ret = 0;

  જો (FD_SETSIZE < nfds)
    {
      errno = EINVAL;
      ret = -1;
    }

  /* If fdout has an error condition (like a broken pipe) it will be seen
     as ready for reading.  Assumes fdout is not actually readable.  */
  જ્યારે (0 <= ret || errno == EINTR)
    {
      fd_set fds;
      FD_ZERO (&fds);
      જો (0 <= fdin)
        FD_SET (fdin, &fds);
      જો (0 <= fdout)
        FD_SET (fdout, &fds);

      સંરચના timeval delay = {0};
      ret = select (nfds,
                    broken_output ? &fds : ખાલી,
                    broken_output ? ખાલી : &fds,
                    ખાલી, block ? ખાલી : &delay);

      જો (ret < 0)
        ચાલુ;
      જો (ret == 0 && ! block)
        પાછા 0;
      affirm (0 < ret);
      જો (0 <= fdin && FD_ISSET (fdin, &fds))    /* input available or EOF; */
        પાછા 0;          /* should now be able to read() without blocking */
      જો (0 <= fdout && FD_ISSET (fdout, &fds))  /* equiv to POLLERR        */
        પાછા broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#endif
  પાછા IOPOLL_ERROR;
}

બાહ્ય પૂર્ણાંક
iopoll (પૂર્ણાંક fdin, પૂર્ણાંક fdout, bool block)
{
  પાછા iopoll_internal (fdin, fdout, block, true);
}



/* Return true if fdin is relevant for iopoll().
   An fd is not relevant for iopoll() if it is always ready for reading,
   which is the case for a regular file or block device.  */

બાહ્ય bool
iopoll_આગત_ok (પૂર્ણાંક fdin)
{
  સંરચના stat st;
  bool always_ready = fstat (fdin, &st) == 0
                      && (S_ISREG (st.st_mode)
                          || S_ISBLK (st.st_mode));
  પાછા ! always_ready;
}

/* Return true if fdout is suitable for iopoll().
   Namely, fdout refers to a pipe.  */

બાહ્ય bool
iopoll_output_ok (પૂર્ણાંક fdout)
{
  પાછા isapipe (fdout) > 0;
}

#ifdef EWOULDBLOCK
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN || (errcode) == EWOULDBLOCK)
#નહીંતર
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN)
#endif

/* Inspect the errno of the previous syscall.
   On EAGAIN, wait for the underlying file descriptor to become writable.
   Return true, if EAGAIN has been successfully handled. */

સ્થિર bool
wait_for_nonblocking_write (પૂર્ણાંક fd)
{
  જો (! IS_EAGAIN (errno))
    /* non-recoverable write error */
    પાછા false;

  /* wait for the file descriptor to become writable */
  જો (iopoll_internal (-1, fd, true, false) != 0)
    {
      errno = EAGAIN;
      પાછા false;
    }

  /* successfully waited for the descriptor to become writable */
  પાછા true;
}

/* wrapper for write() that also waits for FD if non blocking.  */

બાહ્ય bool
write_wait (પૂર્ણાંક fd, શૂન્ય અચલ *buffer, size_t size)
{
  unsigned અક્ષર અચલ *buf = buffer;

  કરો
    {
      અચલ ssize_t written = લિખ (fd, buf, size);
      /* POSIX says that calling write with SIZE of zero may detect and
         return errors.  If no error occurs, or write makes no attempt
         to detect errors, then write returns zero with no other
         results.  write_fail will return successfully in this case.  */
      જો (written == 0)
        {
          જો (size == 0)
            પાછા true;
          નહીંતર
            {
              /* If SIZE is greater than zero and write returns zero,
                 treat it as an error.  Some buggy drivers behave this
                 way.  See src/dd.c and Gnulib's lib/full-write.c for
                 more details.  */
              errno = ENOSPC;
              પાછા false;
            }
        }

      જો (written < 0)
        {
          /* Return an error if write detected one with a SIZE of zero.
             Otherwise, if SIZE is greater than zero, fail if it does
             not become writable.  */
          જો (size == 0 || ! wait_for_nonblocking_write (fd))
            પાછા false;
        }
      નહીંતર
        {
          buf += written;
          size -= written;
        }
    }
  જ્યારે (0 < size);

  પાછા true;
}
