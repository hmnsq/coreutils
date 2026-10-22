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

#অন্তর্ভাব <config.h>

/* poll(2) is needed on AIX (where 'select' gives a readable
   event immediately) and Solaris (where 'select' never gave
   a readable event).  Also use poll(2) on systems we know work
   and/or are already using poll (linux).  On macOS we know poll()
   doesn't work on 10.5, while it does work on >= 11 at least. */
#যদি defined _AIX || defined __sun \
    || defined __linux__ || defined __ANDROID__ \
    || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 110000
# define IOPOLL_USES_POLL 1
  /* Check we've not enabled gnulib's poll module
     as that will emulate poll() in a way not
     currently compatible with our usage.  */
# যদি defined HAVE_POLL
#  error "gnulib's poll() replacement is currently incompatible"
# endif
#endif

#যদি IOPOLL_USES_POLL
# include <poll.h>
#অন্যথা
# include <sys/select.h>
#endif

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "iopoll.h"
#অন্তর্ভাব "isapipe.h"


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

স্থির পূর্ণসংখ্যা
iopoll_internal (পূর্ণসংখ্যা fdin, পূর্ণসংখ্যা fdout, bool block, bool broken_output)
{
  affirm (fdin != -1 || fdout != -1);

#যদি IOPOLL_USES_POLL
  কাঠামো pollfd pfds[2] = {  /* POLLRDBAND needed for illumos, macOS.  */
    { .fd = fdin,  .events = POLLIN | POLLRDBAND, .revents = 0 },
    { .fd = fdout, .events = POLLRDBAND, .revents = 0 },
  };
  পূর্ণসংখ্যা check_out_events = POLLERR | POLLHUP | POLLNVAL;
  পূর্ণসংখ্যা ret = 0;

  যদি (! broken_output)
    {
      pfds[0].events = pfds[1].events = POLLOUT;
      check_out_events = POLLOUT;
    }

  যখন (0 <= ret || errno == EINTR)
    {
      ret = poll (pfds, 2, block ? -1 : 0);

      যদি (ret < 0)
        চালু;
      যদি (ret == 0 && ! block)
        ফেরত 0;
      affirm (0 < ret);
      যদি (pfds[0].revents) /* input available or pipe closed indicating EOF; */
        ফেরত 0;          /* should now be able to read() without blocking  */
      যদি (pfds[1].revents & check_out_events)
        ফেরত broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#অন্যথা  /* fall back to select()-based implementation */

  পূর্ণসংখ্যা nfds = (fdin > fdout ? fdin : fdout) + 1;
  পূর্ণসংখ্যা ret = 0;

  যদি (FD_SETSIZE < nfds)
    {
      errno = EINVAL;
      ret = -1;
    }

  /* If fdout has an error condition (like a broken pipe) it will be seen
     as ready for reading.  Assumes fdout is not actually readable.  */
  যখন (0 <= ret || errno == EINTR)
    {
      fd_set fds;
      FD_ZERO (&fds);
      যদি (0 <= fdin)
        FD_SET (fdin, &fds);
      যদি (0 <= fdout)
        FD_SET (fdout, &fds);

      কাঠামো timeval delay = {0};
      ret = select (nfds,
                    broken_output ? &fds : ফাঁকা,
                    broken_output ? ফাঁকা : &fds,
                    ফাঁকা, block ? ফাঁকা : &delay);

      যদি (ret < 0)
        চালু;
      যদি (ret == 0 && ! block)
        ফেরত 0;
      affirm (0 < ret);
      যদি (0 <= fdin && FD_ISSET (fdin, &fds))    /* input available or EOF; */
        ফেরত 0;          /* should now be able to read() without blocking */
      যদি (0 <= fdout && FD_ISSET (fdout, &fds))  /* equiv to POLLERR        */
        ফেরত broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#endif
  ফেরত IOPOLL_ERROR;
}

বাহ্যিক পূর্ণসংখ্যা
iopoll (পূর্ণসংখ্যা fdin, পূর্ণসংখ্যা fdout, bool block)
{
  ফেরত iopoll_internal (fdin, fdout, block, true);
}



/* Return true if fdin is relevant for iopoll().
   An fd is not relevant for iopoll() if it is always ready for reading,
   which is the case for a regular file or block device.  */

বাহ্যিক bool
iopoll_প্রবেশ_ok (পূর্ণসংখ্যা fdin)
{
  কাঠামো stat st;
  bool always_ready = fstat (fdin, &st) == 0
                      && (S_ISREG (st.st_mode)
                          || S_ISBLK (st.st_mode));
  ফেরত ! always_ready;
}

/* Return true if fdout is suitable for iopoll().
   Namely, fdout refers to a pipe.  */

বাহ্যিক bool
iopoll_output_ok (পূর্ণসংখ্যা fdout)
{
  ফেরত isapipe (fdout) > 0;
}

#ifdef EWOULDBLOCK
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN || (errcode) == EWOULDBLOCK)
#অন্যথা
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN)
#endif

/* Inspect the errno of the previous syscall.
   On EAGAIN, wait for the underlying file descriptor to become writable.
   Return true, if EAGAIN has been successfully handled. */

স্থির bool
wait_for_nonblocking_write (পূর্ণসংখ্যা fd)
{
  যদি (! IS_EAGAIN (errno))
    /* non-recoverable write error */
    ফেরত false;

  /* wait for the file descriptor to become writable */
  যদি (iopoll_internal (-1, fd, true, false) != 0)
    {
      errno = EAGAIN;
      ফেরত false;
    }

  /* successfully waited for the descriptor to become writable */
  ফেরত true;
}

/* wrapper for write() that also waits for FD if non blocking.  */

বাহ্যিক bool
write_wait (পূর্ণসংখ্যা fd, শূন্য ধ্রুবক *বাফার, size_t আকার_2)
{
  unsigned অক্ষর ধ্রুবক *buf = বাফার;

  করো
    {
      ধ্রুবক ssize_t written = লিখ (fd, buf, আকার_2);
      /* POSIX says that calling write with SIZE of zero may detect and
         return errors.  If no error occurs, or write makes no attempt
         to detect errors, then write returns zero with no other
         results.  write_fail will return successfully in this case.  */
      যদি (written == 0)
        {
          যদি (আকার_2 == 0)
            ফেরত true;
          অন্যথা
            {
              /* If SIZE is greater than zero and write returns zero,
                 treat it as an error.  Some buggy drivers behave this
                 way.  See src/dd.c and Gnulib's lib/full-write.c for
                 more details.  */
              errno = ENOSPC;
              ফেরত false;
            }
        }

      যদি (written < 0)
        {
          /* Return an error if write detected one with a SIZE of zero.
             Otherwise, if SIZE is greater than zero, fail if it does
             not become writable.  */
          যদি (আকার_2 == 0 || ! wait_for_nonblocking_write (fd))
            ফেরত false;
        }
      অন্যথা
        {
          buf += written;
          আকার_2 -= written;
        }
    }
  যখন (0 < আকার_2);

  ফেরত true;
}
