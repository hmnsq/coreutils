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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

/* poll(2) is needed on AIX (where 'select' gives a readable
   event immediately) and Solaris (where 'select' never gave
   a readable event).  Also use poll(2) on systems we know work
   and/or are already using poll (linux).  On macOS we know poll()
   doesn't work on 10.5, while it does work on >= 11 at least. */
#ਜੇ defined _AIX || defined __sun \
    || defined __linux__ || defined __ANDROID__ \
    || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 110000
# define IOPOLL_ਵਰਤਦਾ_POLL 1
  /* Check we've not enabled gnulib's poll module
     as that will emulate poll() in a way not
     currently compatible with our usage.  */
# ਜੇ defined HAVE_POLL
#  error "gnulib's poll() replacement is currently incompatible"
# endif
#ਜੇ_ਅੰਤ

#ਜੇ IOPOLL_ਵਰਤਦਾ_POLL
# include <poll.h>
#ਨਹੀਂਤਾਂ
# include <sys/select.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "iopoll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "isapipe.h"


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

ਸਥਿਰ ਪੂਰਨ
iopoll_ਅੰਦਰੂਨੀ (ਪੂਰਨ fdin, ਪੂਰਨ fdout, bool ਖੰਡ, bool broken_output)
{
  affirm (fdin != -1 || fdout != -1);

#ਜੇ IOPOLL_ਵਰਤਦਾ_POLL
  ਬਣਤਰ pollfd pfds[2] = {  /* POLLRDBAND needed for illumos, macOS.  */
    { .fd = fdin,  .events = POLLIN | POLLRDBAND, .revents = 0 },
    { .fd = fdout, .events = POLLRDBAND, .revents = 0 },
  };
  ਪੂਰਨ check_ਬਾਹਰਲਾ_events = POLLERR | POLLHUP | POLLNVAL;
  ਪੂਰਨ ret = 0;

  ਜੇ (! broken_output)
    {
      pfds[0].events = pfds[1].events = POLLOUT;
      check_ਬਾਹਰਲਾ_events = POLLOUT;
    }

  ਜਦੋਂ (0 <= ret || errno == EINTR)
    {
      ret = poll (pfds, 2, ਖੰਡ ? -1 : 0);

      ਜੇ (ret < 0)
        ਜਾਰੀ;
      ਜੇ (ret == 0 && ! ਖੰਡ)
        ਵਾਪਸ 0;
      affirm (0 < ret);
      ਜੇ (pfds[0].revents) /* input available or pipe closed indicating EOF; */
        ਵਾਪਸ 0;          /* should now be able to read() without blocking  */
      ਜੇ (pfds[1].revents & check_ਬਾਹਰਲਾ_events)
        ਵਾਪਸ broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#ਨਹੀਂਤਾਂ  /* fall back to select()-based implementation */

  ਪੂਰਨ nfds = (fdin > fdout ? fdin : fdout) + 1;
  ਪੂਰਨ ret = 0;

  ਜੇ (FD_SETSIZE < nfds)
    {
      errno = EINVAL;
      ret = -1;
    }

  /* If fdout has an error condition (like a broken pipe) it will be seen
     as ready for reading.  Assumes fdout is not actually readable.  */
  ਜਦੋਂ (0 <= ret || errno == EINTR)
    {
      fd_set fds;
      FD_ZERO (&fds);
      ਜੇ (0 <= fdin)
        FD_SET (fdin, &fds);
      ਜੇ (0 <= fdout)
        FD_SET (fdout, &fds);

      ਬਣਤਰ timeval delay = {0};
      ret = select (nfds,
                    broken_output ? &fds : ਨਿਰਾ,
                    broken_output ? ਨਿਰਾ : &fds,
                    ਨਿਰਾ, ਖੰਡ ? ਨਿਰਾ : &delay);

      ਜੇ (ret < 0)
        ਜਾਰੀ;
      ਜੇ (ret == 0 && ! ਖੰਡ)
        ਵਾਪਸ 0;
      affirm (0 < ret);
      ਜੇ (0 <= fdin && FD_ISSET (fdin, &fds))    /* input available or EOF; */
        ਵਾਪਸ 0;          /* should now be able to read() without blocking */
      ਜੇ (0 <= fdout && FD_ISSET (fdout, &fds))  /* equiv to POLLERR        */
        ਵਾਪਸ broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#ਜੇ_ਅੰਤ
  ਵਾਪਸ IOPOLL_ERROR;
}

ਬਾਹਰੀ ਪੂਰਨ
iopoll (ਪੂਰਨ fdin, ਪੂਰਨ fdout, bool ਖੰਡ)
{
  ਵਾਪਸ iopoll_ਅੰਦਰੂਨੀ (fdin, fdout, ਖੰਡ, true);
}



/* Return true if fdin is relevant for iopoll().
   An fd is not relevant for iopoll() if it is always ready for reading,
   which is the case for a regular file or block device.  */

ਬਾਹਰੀ bool
iopoll_ਨਿਵੇਸ਼_ok (ਪੂਰਨ fdin)
{
  ਬਣਤਰ stat st;
  bool always_ready = fstat (fdin, &st) == 0
                      && (S_ISREG (st.st_ਬਹੁਲਕ)
                          || S_ISBLK (st.st_ਬਹੁਲਕ));
  ਵਾਪਸ ! always_ready;
}

/* Return true if fdout is suitable for iopoll().
   Namely, fdout refers to a pipe.  */

ਬਾਹਰੀ bool
iopoll_output_ok (ਪੂਰਨ fdout)
{
  ਵਾਪਸ isapipe (fdout) > 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EWOULDBLOCK
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN || (errcode) == EWOULDBLOCK)
#ਨਹੀਂਤਾਂ
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN)
#ਜੇ_ਅੰਤ

/* Inspect the errno of the previous syscall.
   On EAGAIN, wait for the underlying file descriptor to become writable.
   Return true, if EAGAIN has been successfully handled. */

ਸਥਿਰ bool
wait_for_nonblocking_write (ਪੂਰਨ fd)
{
  ਜੇ (! IS_EAGAIN (errno))
    /* non-recoverable write error */
    ਵਾਪਸ false;

  /* wait for the file descriptor to become writable */
  ਜੇ (iopoll_ਅੰਦਰੂਨੀ (-1, fd, true, false) != 0)
    {
      errno = EAGAIN;
      ਵਾਪਸ false;
    }

  /* successfully waited for the descriptor to become writable */
  ਵਾਪਸ true;
}

/* wrapper for write() that also waits for FD if non blocking.  */

ਬਾਹਰੀ bool
write_wait (ਪੂਰਨ fd, ਖਾਲੀ ਅਚਲ *ਬਫਰ, size_t ਮਾਪ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *buf = ਬਫਰ;

  ਕਰੋ
    {
      ਅਚਲ ssize_t written = ਲਿਖ (fd, buf, ਮਾਪ);
      /* POSIX says that calling write with SIZE of zero may detect and
         return errors.  If no error occurs, or write makes no attempt
         to detect errors, then write returns zero with no other
         results.  write_fail will return successfully in this case.  */
      ਜੇ (written == 0)
        {
          ਜੇ (ਮਾਪ == 0)
            ਵਾਪਸ true;
          ਨਹੀਂਤਾਂ
            {
              /* If SIZE is greater than zero and write returns zero,
                 treat it as an error.  Some buggy drivers behave this
                 way.  See src/dd.c and Gnulib's lib/full-write.c for
                 more details.  */
              errno = ENOSPC;
              ਵਾਪਸ false;
            }
        }

      ਜੇ (written < 0)
        {
          /* Return an error if write detected one with a SIZE of zero.
             Otherwise, if SIZE is greater than zero, fail if it does
             not become writable.  */
          ਜੇ (ਮਾਪ == 0 || ! wait_for_nonblocking_write (fd))
            ਵਾਪਸ false;
        }
      ਨਹੀਂਤਾਂ
        {
          buf += written;
          ਮਾਪ -= written;
        }
    }
  ਜਦੋਂ (0 < ਮਾਪ);

  ਵਾਪਸ true;
}
