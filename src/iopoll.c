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

#அந்தர்பாவ <config.h>

/* poll(2) is needed on AIX (where 'select' gives a readable
   event immediately) and Solaris (where 'select' never gave
   a readable event).  Also use poll(2) on systems we know work
   and/or are already using poll (linux).  On macOS we know poll()
   doesn't work on 10.5, while it does work on >= 11 at least. */
#எனில் defined _AIX || defined __sun \
    || defined __linux__ || defined __ANDROID__ \
    || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 110000
# define IOPOLL_USES_POLL 1
  /* Check we've not enabled gnulib's poll module
     as that will emulate poll() in a way not
     currently compatible with our usage.  */
# எனில் defined HAVE_POLL
#  error "gnulib's poll() replacement is currently incompatible"
# endif
#endif

#எனில் IOPOLL_USES_POLL
# include <poll.h>
#இல்லையெனில்
# include <sys/select.h>
#endif

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "iopoll.h"
#அந்தர்பாவ "isapipe.h"


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

நிலையான முழு
iopoll_internal (முழு fdin, முழு fdout, bool block, bool broken_output)
{
  affirm (fdin != -1 || fdout != -1);

#எனில் IOPOLL_USES_POLL
  கட்டமைப்பு pollfd pfds[2] = {  /* POLLRDBAND needed for illumos, macOS.  */
    { .fd = fdin,  .events = POLLIN | POLLRDBAND, .revents = 0 },
    { .fd = fdout, .events = POLLRDBAND, .revents = 0 },
  };
  முழு check_out_events = POLLERR | POLLHUP | POLLNVAL;
  முழு ret = 0;

  எனில் (! broken_output)
    {
      pfds[0].events = pfds[1].events = POLLOUT;
      check_out_events = POLLOUT;
    }

  போது (0 <= ret || errno == EINTR)
    {
      ret = poll (pfds, 2, block ? -1 : 0);

      எனில் (ret < 0)
        தொடர்;
      எனில் (ret == 0 && ! block)
        திரும்பு 0;
      affirm (0 < ret);
      எனில் (pfds[0].revents) /* input available or pipe closed indicating EOF; */
        திரும்பு 0;          /* should now be able to read() without blocking  */
      எனில் (pfds[1].revents & check_out_events)
        திரும்பு broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#இல்லையெனில்  /* fall back to select()-based implementation */

  முழு nfds = (fdin > fdout ? fdin : fdout) + 1;
  முழு ret = 0;

  எனில் (FD_SETSIZE < nfds)
    {
      errno = EINVAL;
      ret = -1;
    }

  /* If fdout has an error condition (like a broken pipe) it will be seen
     as ready for reading.  Assumes fdout is not actually readable.  */
  போது (0 <= ret || errno == EINTR)
    {
      fd_set fds;
      FD_ZERO (&fds);
      எனில் (0 <= fdin)
        FD_SET (fdin, &fds);
      எனில் (0 <= fdout)
        FD_SET (fdout, &fds);

      கட்டமைப்பு timeval delay = {0};
      ret = select (nfds,
                    broken_output ? &fds : வெற்று,
                    broken_output ? வெற்று : &fds,
                    வெற்று, block ? வெற்று : &delay);

      எனில் (ret < 0)
        தொடர்;
      எனில் (ret == 0 && ! block)
        திரும்பு 0;
      affirm (0 < ret);
      எனில் (0 <= fdin && FD_ISSET (fdin, &fds))    /* input available or EOF; */
        திரும்பு 0;          /* should now be able to read() without blocking */
      எனில் (0 <= fdout && FD_ISSET (fdout, &fds))  /* equiv to POLLERR        */
        திரும்பு broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#endif
  திரும்பு IOPOLL_ERROR;
}

வெளி முழு
iopoll (முழு fdin, முழு fdout, bool block)
{
  திரும்பு iopoll_internal (fdin, fdout, block, true);
}



/* Return true if fdin is relevant for iopoll().
   An fd is not relevant for iopoll() if it is always ready for reading,
   which is the case for a regular file or block device.  */

வெளி bool
iopoll_உள்ளீடு_ok (முழு fdin)
{
  கட்டமைப்பு stat st;
  bool always_ready = fstat (fdin, &st) == 0
                      && (S_ISREG (st.st_mode)
                          || S_ISBLK (st.st_mode));
  திரும்பு ! always_ready;
}

/* Return true if fdout is suitable for iopoll().
   Namely, fdout refers to a pipe.  */

வெளி bool
iopoll_output_ok (முழு fdout)
{
  திரும்பு isapipe (fdout) > 0;
}

#ifdef EWOULDBLOCK
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN || (errcode) == EWOULDBLOCK)
#இல்லையெனில்
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN)
#endif

/* Inspect the errno of the previous syscall.
   On EAGAIN, wait for the underlying file descriptor to become writable.
   Return true, if EAGAIN has been successfully handled. */

நிலையான bool
wait_for_nonblocking_write (முழு fd)
{
  எனில் (! IS_EAGAIN (errno))
    /* non-recoverable write error */
    திரும்பு false;

  /* wait for the file descriptor to become writable */
  எனில் (iopoll_internal (-1, fd, true, false) != 0)
    {
      errno = EAGAIN;
      திரும்பு false;
    }

  /* successfully waited for the descriptor to become writable */
  திரும்பு true;
}

/* wrapper for write() that also waits for FD if non blocking.  */

வெளி bool
write_wait (முழு fd, வெறுமை மாறிலி *buffer, size_t size)
{
  unsigned எழுத்து மாறிலி *buf = buffer;

  செய்
    {
      மாறிலி ssize_t written = லிக (fd, buf, size);
      /* POSIX says that calling write with SIZE of zero may detect and
         return errors.  If no error occurs, or write makes no attempt
         to detect errors, then write returns zero with no other
         results.  write_fail will return successfully in this case.  */
      எனில் (written == 0)
        {
          எனில் (size == 0)
            திரும்பு true;
          இல்லையெனில்
            {
              /* If SIZE is greater than zero and write returns zero,
                 treat it as an error.  Some buggy drivers behave this
                 way.  See src/dd.c and Gnulib's lib/full-write.c for
                 more details.  */
              errno = ENOSPC;
              திரும்பு false;
            }
        }

      எனில் (written < 0)
        {
          /* Return an error if write detected one with a SIZE of zero.
             Otherwise, if SIZE is greater than zero, fail if it does
             not become writable.  */
          எனில் (size == 0 || ! wait_for_nonblocking_write (fd))
            திரும்பு false;
        }
      இல்லையெனில்
        {
          buf += written;
          size -= written;
        }
    }
  போது (0 < size);

  திரும்பு true;
}
