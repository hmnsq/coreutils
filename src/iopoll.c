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

#include <config.h>

/* poll(2) is needed on AIX (where 'select' gives a readable
   event immediately) and Solaris (where 'select' never gave
   a readable event).  Also use poll(2) on systems we know work
   and/or are already using poll (linux).  On macOS we know poll()
   doesn't work on 10.5, while it does work on >= 11 at least. */
#якщо defined _AIX || defined __sun \
    || defined __linux__ || defined __ANDROID__ \
    || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 110000
# define IOPOLL_USES_POLL 1
  /* Check we've not enabled gnulib's poll module
     as that will emulate poll() in a way not
     currently compatible with our usage.  */
# якщо defined HAVE_POLL
#  error "gnulib's poll() replacement is currently incompatible"
# endif
#endif

#якщо IOPOLL_USES_POLL
# include <poll.h>
#інакше
# include <sys/select.h>
#endif

#include "system.h"
#include "assure.h"
#include "iopoll.h"
#include "isapipe.h"


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

static ціле
iopoll_internal (ціле fdin, ціле fdout, bool block, bool broken_output)
{
  affirm (fdin != -1 || fdout != -1);

#якщо IOPOLL_USES_POLL
  struct pollfd pfds[2] = {  /* POLLRDBAND needed for illumos, macOS.  */
    { .fd = fdin,  .events = POLLIN | POLLRDBAND, .revents = 0 },
    { .fd = fdout, .events = POLLRDBAND, .revents = 0 },
  };
  ціле check_out_events = POLLERR | POLLHUP | POLLNVAL;
  ціле ret = 0;

  якщо (! broken_output)
    {
      pfds[0].events = pfds[1].events = POLLOUT;
      check_out_events = POLLOUT;
    }

  поки (0 <= ret || errno == EINTR)
    {
      ret = poll (pfds, 2, block ? -1 : 0);

      якщо (ret < 0)
        continue;
      якщо (ret == 0 && ! block)
        повернути 0;
      affirm (0 < ret);
      якщо (pfds[0].revents) /* input available or pipe closed indicating EOF; */
        повернути 0;          /* should now be able to read() without blocking  */
      якщо (pfds[1].revents & check_out_events)
        повернути broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#інакше  /* fall back to select()-based implementation */

  ціле nfds = (fdin > fdout ? fdin : fdout) + 1;
  ціле ret = 0;

  якщо (FD_SETSIZE < nfds)
    {
      errno = EINVAL;
      ret = -1;
    }

  /* If fdout has an error condition (like a broken pipe) it will be seen
     as ready for reading.  Assumes fdout is not actually readable.  */
  поки (0 <= ret || errno == EINTR)
    {
      fd_set fds;
      FD_ZERO (&fds);
      якщо (0 <= fdin)
        FD_SET (fdin, &fds);
      якщо (0 <= fdout)
        FD_SET (fdout, &fds);

      struct timeval delay = {0};
      ret = select (nfds,
                    broken_output ? &fds : NULL,
                    broken_output ? NULL : &fds,
                    NULL, block ? NULL : &delay);

      якщо (ret < 0)
        continue;
      якщо (ret == 0 && ! block)
        повернути 0;
      affirm (0 < ret);
      якщо (0 <= fdin && FD_ISSET (fdin, &fds))    /* input available or EOF; */
        повернути 0;          /* should now be able to read() without blocking */
      якщо (0 <= fdout && FD_ISSET (fdout, &fds))  /* equiv to POLLERR        */
        повернути broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#endif
  повернути IOPOLL_ERROR;
}

extern ціле
iopoll (ціле fdin, ціле fdout, bool block)
{
  повернути iopoll_internal (fdin, fdout, block, true);
}



/* Return true if fdin is relevant for iopoll().
   An fd is not relevant for iopoll() if it is always ready for reading,
   which is the case for a regular file or block device.  */

extern bool
iopoll_input_ok (ціле fdin)
{
  struct stat st;
  bool always_ready = fstat (fdin, &st) == 0
                      && (S_ISREG (st.st_mode)
                          || S_ISBLK (st.st_mode));
  повернути ! always_ready;
}

/* Return true if fdout is suitable for iopoll().
   Namely, fdout refers to a pipe.  */

extern bool
iopoll_output_ok (ціле fdout)
{
  повернути isapipe (fdout) > 0;
}

#ifdef EWOULDBLOCK
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN || (errcode) == EWOULDBLOCK)
#інакше
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN)
#endif

/* Inspect the errno of the previous syscall.
   On EAGAIN, wait for the underlying file descriptor to become writable.
   Return true, if EAGAIN has been successfully handled. */

static bool
wait_for_nonblocking_write (ціле fd)
{
  якщо (! IS_EAGAIN (errno))
    /* non-recoverable write error */
    повернути false;

  /* wait for the file descriptor to become writable */
  якщо (iopoll_internal (-1, fd, true, false) != 0)
    {
      errno = EAGAIN;
      повернути false;
    }

  /* successfully waited for the descriptor to become writable */
  повернути true;
}

/* wrapper for write() that also waits for FD if non blocking.  */

extern bool
write_wait (ціле fd, void const *buffer, size_t size)
{
  unsigned char const *buf = buffer;

  do
    {
      const ssize_t written = write (fd, buf, size);
      /* POSIX says that calling write with SIZE of zero may detect and
         return errors.  If no error occurs, or write makes no attempt
         to detect errors, then write returns zero with no other
         results.  write_fail will return successfully in this case.  */
      якщо (written == 0)
        {
          якщо (size == 0)
            повернути true;
          інакше
            {
              /* If SIZE is greater than zero and write returns zero,
                 treat it as an error.  Some buggy drivers behave this
                 way.  See src/dd.c and Gnulib's lib/full-write.c for
                 more details.  */
              errno = ENOSPC;
              повернути false;
            }
        }

      якщо (written < 0)
        {
          /* Return an error if write detected one with a SIZE of zero.
             Otherwise, if SIZE is greater than zero, fail if it does
             not become writable.  */
          якщо (size == 0 || ! wait_for_nonblocking_write (fd))
            повернути false;
        }
      інакше
        {
          buf += written;
          size -= written;
        }
    }
  поки (0 < size);

  повернути true;
}
