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

#ಅನ್ತರ್ಭಾವ <config.h>

/* poll(2) is needed on AIX (where 'select' gives a readable
   event immediately) and Solaris (where 'select' never gave
   a readable event).  Also use poll(2) on systems we know work
   and/or are already using poll (linux).  On macOS we know poll()
   doesn't work on 10.5, while it does work on >= 11 at least. */
#ಯದಿ defined _AIX || defined __sun \
    || defined __linux__ || defined __ANDROID__ \
    || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 110000
# define IOPOLL_USES_POLL 1
  /* Check we've not enabled gnulib's poll module
     as that will emulate poll() in a way not
     currently compatible with our usage.  */
# ಯದಿ defined HAVE_POLL
#  error "gnulib's poll() replacement is currently incompatible"
# endif
#endif

#ಯದಿ IOPOLL_USES_POLL
# include <poll.h>
#ಅನ್ಯಥಾ
# include <sys/select.h>
#endif

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "iopoll.h"
#ಅನ್ತರ್ಭಾವ "isapipe.h"


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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
iopoll_internal (ಪೂರ್ಣಾಙ್ಕ fdin, ಪೂರ್ಣಾಙ್ಕ fdout, bool block, bool broken_output)
{
  affirm (fdin != -1 || fdout != -1);

#ಯದಿ IOPOLL_USES_POLL
  ಸಂರಚನಾ pollfd pfds[2] = {  /* POLLRDBAND needed for illumos, macOS.  */
    { .fd = fdin,  .events = POLLIN | POLLRDBAND, .revents = 0 },
    { .fd = fdout, .events = POLLRDBAND, .revents = 0 },
  };
  ಪೂರ್ಣಾಙ್ಕ check_out_events = POLLERR | POLLHUP | POLLNVAL;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;

  ಯದಿ (! broken_output)
    {
      pfds[0].events = pfds[1].events = POLLOUT;
      check_out_events = POLLOUT;
    }

  ಯಾವತ್ (0 <= ret || errno == EINTR)
    {
      ret = poll (pfds, 2, block ? -1 : 0);

      ಯದಿ (ret < 0)
        ಅನುವರ್ತ;
      ಯದಿ (ret == 0 && ! block)
        ಪ್ರತ್ಯಾಗಮ 0;
      affirm (0 < ret);
      ಯದಿ (pfds[0].revents) /* input available or pipe closed indicating EOF; */
        ಪ್ರತ್ಯಾಗಮ 0;          /* should now be able to read() without blocking  */
      ಯದಿ (pfds[1].revents & check_out_events)
        ಪ್ರತ್ಯಾಗಮ broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#ಅನ್ಯಥಾ  /* fall back to select()-based implementation */

  ಪೂರ್ಣಾಙ್ಕ nfds = (fdin > fdout ? fdin : fdout) + 1;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;

  ಯದಿ (FD_SETSIZE < nfds)
    {
      errno = EINVAL;
      ret = -1;
    }

  /* If fdout has an error condition (like a broken pipe) it will be seen
     as ready for reading.  Assumes fdout is not actually readable.  */
  ಯಾವತ್ (0 <= ret || errno == EINTR)
    {
      fd_set fds;
      FD_ZERO (&fds);
      ಯದಿ (0 <= fdin)
        FD_SET (fdin, &fds);
      ಯದಿ (0 <= fdout)
        FD_SET (fdout, &fds);

      ಸಂರಚನಾ timeval delay = {0};
      ret = select (nfds,
                    broken_output ? &fds : ರಿಕ್ತ,
                    broken_output ? ರಿಕ್ತ : &fds,
                    ರಿಕ್ತ, block ? ರಿಕ್ತ : &delay);

      ಯದಿ (ret < 0)
        ಅನುವರ್ತ;
      ಯದಿ (ret == 0 && ! block)
        ಪ್ರತ್ಯಾಗಮ 0;
      affirm (0 < ret);
      ಯದಿ (0 <= fdin && FD_ISSET (fdin, &fds))    /* input available or EOF; */
        ಪ್ರತ್ಯಾಗಮ 0;          /* should now be able to read() without blocking */
      ಯದಿ (0 <= fdout && FD_ISSET (fdout, &fds))  /* equiv to POLLERR        */
        ಪ್ರತ್ಯಾಗಮ broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#endif
  ಪ್ರತ್ಯಾಗಮ IOPOLL_ERROR;
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ
iopoll (ಪೂರ್ಣಾಙ್ಕ fdin, ಪೂರ್ಣಾಙ್ಕ fdout, bool block)
{
  ಪ್ರತ್ಯಾಗಮ iopoll_internal (fdin, fdout, block, true);
}



/* Return true if fdin is relevant for iopoll().
   An fd is not relevant for iopoll() if it is always ready for reading,
   which is the case for a regular file or block device.  */

ಬಾಹ್ಯ bool
iopoll_ಒಳಹರಿವು_ok (ಪೂರ್ಣಾಙ್ಕ fdin)
{
  ಸಂರಚನಾ stat st;
  bool always_ready = fstat (fdin, &st) == 0
                      && (S_ISREG (st.st_mode)
                          || S_ISBLK (st.st_mode));
  ಪ್ರತ್ಯಾಗಮ ! always_ready;
}

/* Return true if fdout is suitable for iopoll().
   Namely, fdout refers to a pipe.  */

ಬಾಹ್ಯ bool
iopoll_output_ok (ಪೂರ್ಣಾಙ್ಕ fdout)
{
  ಪ್ರತ್ಯಾಗಮ isapipe (fdout) > 0;
}

#ifdef EWOULDBLOCK
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN || (errcode) == EWOULDBLOCK)
#ಅನ್ಯಥಾ
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN)
#endif

/* Inspect the errno of the previous syscall.
   On EAGAIN, wait for the underlying file descriptor to become writable.
   Return true, if EAGAIN has been successfully handled. */

ಸ್ಥಿರ bool
wait_for_nonblocking_write (ಪೂರ್ಣಾಙ್ಕ fd)
{
  ಯದಿ (! IS_EAGAIN (errno))
    /* non-recoverable write error */
    ಪ್ರತ್ಯಾಗಮ false;

  /* wait for the file descriptor to become writable */
  ಯದಿ (iopoll_internal (-1, fd, true, false) != 0)
    {
      errno = EAGAIN;
      ಪ್ರತ್ಯಾಗಮ false;
    }

  /* successfully waited for the descriptor to become writable */
  ಪ್ರತ್ಯಾಗಮ true;
}

/* wrapper for write() that also waits for FD if non blocking.  */

ಬಾಹ್ಯ bool
write_wait (ಪೂರ್ಣಾಙ್ಕ fd, ಶೂನ್ಯ ಧ್ರುವ *buffer, size_t size)
{
  unsigned ವರ್ಣ ಧ್ರುವ *buf = buffer;

  ಕುರು
    {
      ಧ್ರುವ ssize_t written = ಲಿಖ (fd, buf, size);
      /* POSIX says that calling write with SIZE of zero may detect and
         return errors.  If no error occurs, or write makes no attempt
         to detect errors, then write returns zero with no other
         results.  write_fail will return successfully in this case.  */
      ಯದಿ (written == 0)
        {
          ಯದಿ (size == 0)
            ಪ್ರತ್ಯಾಗಮ true;
          ಅನ್ಯಥಾ
            {
              /* If SIZE is greater than zero and write returns zero,
                 treat it as an error.  Some buggy drivers behave this
                 way.  See src/dd.c and Gnulib's lib/full-write.c for
                 more details.  */
              errno = ENOSPC;
              ಪ್ರತ್ಯಾಗಮ false;
            }
        }

      ಯದಿ (written < 0)
        {
          /* Return an error if write detected one with a SIZE of zero.
             Otherwise, if SIZE is greater than zero, fail if it does
             not become writable.  */
          ಯದಿ (size == 0 || ! wait_for_nonblocking_write (fd))
            ಪ್ರತ್ಯಾಗಮ false;
        }
      ಅನ್ಯಥಾ
        {
          buf += written;
          size -= written;
        }
    }
  ಯಾವತ್ (0 < size);

  ಪ್ರತ್ಯಾಗಮ true;
}
