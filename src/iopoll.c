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

#شامل <config.h>

/* poll(2) is needed on AIX (where 'select' gives a readable
   event immediately) and Solaris (where 'select' never gave
   a readable event).  Also use poll(2) on systems we know work
   and/or are already using poll (linux).  On macOS we know poll()
   doesn't work on 10.5, while it does work on >= 11 at least. */
#اگر defined _AIX || defined __sun \
    || defined __linux__ || defined __ANDROID__ \
    || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 110000
# define IOPOLL_USES_POLL 1
  /* Check we've not enabled gnulib's poll module
     as that will emulate poll() in a way not
     currently compatible with our usage.  */
# اگر defined HAVE_POLL
#  error "gnulib's poll() replacement is currently incompatible"
# endif
#endif

#اگر IOPOLL_USES_POLL
# include <poll.h>
#وگرنه
# include <sys/select.h>
#endif

#شامل "system.h"
#شامل "assure.h"
#شامل "iopoll.h"
#شامل "isapipe.h"


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

ایستا صحیح
iopoll_internal (صحیح fdin, صحیح fdout, bool block, bool broken_output)
{
  affirm (fdin != -1 || fdout != -1);

#اگر IOPOLL_USES_POLL
  ساختار pollfd pfds[2] = {  /* POLLRDBAND needed for illumos, macOS.  */
    { .fd = fdin,  .events = POLLIN | POLLRDBAND, .revents = 0 },
    { .fd = fdout, .events = POLLRDBAND, .revents = 0 },
  };
  صحیح check_out_events = POLLERR | POLLHUP | POLLNVAL;
  صحیح ret = 0;

  اگر (! broken_output)
    {
      pfds[0].events = pfds[1].events = POLLOUT;
      check_out_events = POLLOUT;
    }

  وقتی (0 <= ret || errno == EINTR)
    {
      ret = poll (pfds, 2, block ? -1 : 0);

      اگر (ret < 0)
        ادامه;
      اگر (ret == 0 && ! block)
        بازگشت 0;
      affirm (0 < ret);
      اگر (pfds[0].revents) /* input available or pipe closed indicating EOF; */
        بازگشت 0;          /* should now be able to read() without blocking  */
      اگر (pfds[1].revents & check_out_events)
        بازگشت broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#وگرنه  /* fall back to select()-based implementation */

  صحیح nfds = (fdin > fdout ? fdin : fdout) + 1;
  صحیح ret = 0;

  اگر (FD_SETSIZE < nfds)
    {
      errno = EINVAL;
      ret = -1;
    }

  /* If fdout has an error condition (like a broken pipe) it will be seen
     as ready for reading.  Assumes fdout is not actually readable.  */
  وقتی (0 <= ret || errno == EINTR)
    {
      fd_set fds;
      FD_ZERO (&fds);
      اگر (0 <= fdin)
        FD_SET (fdin, &fds);
      اگر (0 <= fdout)
        FD_SET (fdout, &fds);

      ساختار timeval delay = {0};
      ret = select (nfds,
                    broken_output ? &fds : پوچ,
                    broken_output ? پوچ : &fds,
                    پوچ, block ? پوچ : &delay);

      اگر (ret < 0)
        ادامه;
      اگر (ret == 0 && ! block)
        بازگشت 0;
      affirm (0 < ret);
      اگر (0 <= fdin && FD_ISSET (fdin, &fds))    /* input available or EOF; */
        بازگشت 0;          /* should now be able to read() without blocking */
      اگر (0 <= fdout && FD_ISSET (fdout, &fds))  /* equiv to POLLERR        */
        بازگشت broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#endif
  بازگشت IOPOLL_ERROR;
}

بیرونی صحیح
iopoll (صحیح fdin, صحیح fdout, bool block)
{
  بازگشت iopoll_internal (fdin, fdout, block, true);
}



/* Return true if fdin is relevant for iopoll().
   An fd is not relevant for iopoll() if it is always ready for reading,
   which is the case for a regular file or block device.  */

بیرونی bool
iopoll_درون_داد_ok (صحیح fdin)
{
  ساختار stat st;
  bool always_ready = fstat (fdin, &st) == 0
                      && (S_ISREG (st.st_mode)
                          || S_ISBLK (st.st_mode));
  بازگشت ! always_ready;
}

/* Return true if fdout is suitable for iopoll().
   Namely, fdout refers to a pipe.  */

بیرونی bool
iopoll_output_ok (صحیح fdout)
{
  بازگشت isapipe (fdout) > 0;
}

#ifdef EWOULDBLOCK
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN || (errcode) == EWOULDBLOCK)
#وگرنه
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN)
#endif

/* Inspect the errno of the previous syscall.
   On EAGAIN, wait for the underlying file descriptor to become writable.
   Return true, if EAGAIN has been successfully handled. */

ایستا bool
wait_for_nonblocking_write (صحیح fd)
{
  اگر (! IS_EAGAIN (errno))
    /* non-recoverable write error */
    بازگشت false;

  /* wait for the file descriptor to become writable */
  اگر (iopoll_internal (-1, fd, true, false) != 0)
    {
      errno = EAGAIN;
      بازگشت false;
    }

  /* successfully waited for the descriptor to become writable */
  بازگشت true;
}

/* wrapper for write() that also waits for FD if non blocking.  */

بیرونی bool
write_wait (صحیح fd, تهی ثابت *buffer, size_t size)
{
  unsigned نویسه ثابت *buf = buffer;

  انجامبده
    {
      ثابت ssize_t written = اكتب (fd, buf, size);
      /* POSIX says that calling write with SIZE of zero may detect and
         return errors.  If no error occurs, or write makes no attempt
         to detect errors, then write returns zero with no other
         results.  write_fail will return successfully in this case.  */
      اگر (written == 0)
        {
          اگر (size == 0)
            بازگشت true;
          وگرنه
            {
              /* If SIZE is greater than zero and write returns zero,
                 treat it as an error.  Some buggy drivers behave this
                 way.  See src/dd.c and Gnulib's lib/full-write.c for
                 more details.  */
              errno = ENOSPC;
              بازگشت false;
            }
        }

      اگر (written < 0)
        {
          /* Return an error if write detected one with a SIZE of zero.
             Otherwise, if SIZE is greater than zero, fail if it does
             not become writable.  */
          اگر (size == 0 || ! wait_for_nonblocking_write (fd))
            بازگشت false;
        }
      وگرنه
        {
          buf += written;
          size -= written;
        }
    }
  وقتی (0 < size);

  بازگشت true;
}
