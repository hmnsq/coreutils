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

#అన్తర్భావ <config.h>

/* poll(2) is needed on AIX (where 'select' gives a readable
   event immediately) and Solaris (where 'select' never gave
   a readable event).  Also use poll(2) on systems we know work
   and/or are already using poll (linux).  On macOS we know poll()
   doesn't work on 10.5, while it does work on >= 11 at least. */
#ఒకవేళ defined _AIX || defined __sun \
    || defined __linux__ || defined __ANDROID__ \
    || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 110000
# define IOPOLL_ఉపయోగిస్తుంది_POLL 1
  /* Check we've not enabled gnulib's poll module
     as that will emulate poll() in a way not
     currently compatible with our usage.  */
# ఒకవేళ defined HAVE_POLL
#  error "gnulib's poll() replacement is currently incompatible"
# endif
#అయితే_ముగింపు

#ఒకవేళ IOPOLL_ఉపయోగిస్తుంది_POLL
# include <poll.h>
#లేకపోతే
# include <sys/select.h>
#అయితే_ముగింపు

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "iopoll.h"
#అన్తర్భావ "isapipe.h"


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

స్థిర పూర్ణాంకం
iopoll_అంతర్గత (పూర్ణాంకం fdin, పూర్ణాంకం fdout, bool ఖండం, bool broken_output)
{
  affirm (fdin != -1 || fdout != -1);

#ఒకవేళ IOPOLL_ఉపయోగిస్తుంది_POLL
  నిర్మాణం pollfd pfds[2] = {  /* POLLRDBAND needed for illumos, macOS.  */
    { .fd = fdin,  .events = POLLIN | POLLRDBAND, .revents = 0 },
    { .fd = fdout, .events = POLLRDBAND, .revents = 0 },
  };
  పూర్ణాంకం check_బహిర్గతం_events = POLLERR | POLLHUP | POLLNVAL;
  పూర్ణాంకం ret = 0;

  ఒకవేళ (! broken_output)
    {
      pfds[0].events = pfds[1].events = POLLOUT;
      check_బహిర్గతం_events = POLLOUT;
    }

  అయితే (0 <= ret || errno == EINTR)
    {
      ret = poll (pfds, 2, ఖండం ? -1 : 0);

      ఒకవేళ (ret < 0)
        కొనసాగు;
      ఒకవేళ (ret == 0 && ! ఖండం)
        తిరిగి 0;
      affirm (0 < ret);
      ఒకవేళ (pfds[0].revents) /* input available or pipe closed indicating EOF; */
        తిరిగి 0;          /* should now be able to read() without blocking  */
      ఒకవేళ (pfds[1].revents & check_బహిర్గతం_events)
        తిరిగి broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#లేకపోతే  /* fall back to select()-based implementation */

  పూర్ణాంకం nfds = (fdin > fdout ? fdin : fdout) + 1;
  పూర్ణాంకం ret = 0;

  ఒకవేళ (FD_SETSIZE < nfds)
    {
      errno = EINVAL;
      ret = -1;
    }

  /* If fdout has an error condition (like a broken pipe) it will be seen
     as ready for reading.  Assumes fdout is not actually readable.  */
  అయితే (0 <= ret || errno == EINTR)
    {
      fd_set fds;
      FD_ZERO (&fds);
      ఒకవేళ (0 <= fdin)
        FD_SET (fdin, &fds);
      ఒకవేళ (0 <= fdout)
        FD_SET (fdout, &fds);

      నిర్మాణం timeval delay = {0};
      ret = select (nfds,
                    broken_output ? &fds : ఖాళీ,
                    broken_output ? ఖాళీ : &fds,
                    ఖాళీ, ఖండం ? ఖాళీ : &delay);

      ఒకవేళ (ret < 0)
        కొనసాగు;
      ఒకవేళ (ret == 0 && ! ఖండం)
        తిరిగి 0;
      affirm (0 < ret);
      ఒకవేళ (0 <= fdin && FD_ISSET (fdin, &fds))    /* input available or EOF; */
        తిరిగి 0;          /* should now be able to read() without blocking */
      ఒకవేళ (0 <= fdout && FD_ISSET (fdout, &fds))  /* equiv to POLLERR        */
        తిరిగి broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#అయితే_ముగింపు
  తిరిగి IOPOLL_ERROR;
}

బాహ్య పూర్ణాంకం
iopoll (పూర్ణాంకం fdin, పూర్ణాంకం fdout, bool ఖండం)
{
  తిరిగి iopoll_అంతర్గత (fdin, fdout, ఖండం, true);
}



/* Return true if fdin is relevant for iopoll().
   An fd is not relevant for iopoll() if it is always ready for reading,
   which is the case for a regular file or block device.  */

బాహ్య bool
iopoll_ఆదానం_ok (పూర్ణాంకం fdin)
{
  నిర్మాణం stat st;
  bool always_ready = fstat (fdin, &st) == 0
                      && (S_ISREG (st.st_బహుళకం)
                          || S_ISBLK (st.st_బహుళకం));
  తిరిగి ! always_ready;
}

/* Return true if fdout is suitable for iopoll().
   Namely, fdout refers to a pipe.  */

బాహ్య bool
iopoll_output_ok (పూర్ణాంకం fdout)
{
  తిరిగి isapipe (fdout) > 0;
}

#నిర్వచితమైతే EWOULDBLOCK
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN || (errcode) == EWOULDBLOCK)
#లేకపోతే
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN)
#అయితే_ముగింపు

/* Inspect the errno of the previous syscall.
   On EAGAIN, wait for the underlying file descriptor to become writable.
   Return true, if EAGAIN has been successfully handled. */

స్థిర bool
wait_for_nonblocking_write (పూర్ణాంకం fd)
{
  ఒకవేళ (! IS_EAGAIN (errno))
    /* non-recoverable write error */
    తిరిగి false;

  /* wait for the file descriptor to become writable */
  ఒకవేళ (iopoll_అంతర్గత (-1, fd, true, false) != 0)
    {
      errno = EAGAIN;
      తిరిగి false;
    }

  /* successfully waited for the descriptor to become writable */
  తిరిగి true;
}

/* wrapper for write() that also waits for FD if non blocking.  */

బాహ్య bool
write_wait (పూర్ణాంకం fd, శూన్య స్థిరరాశి *బఫరు, size_t కొలత)
{
  గుర్తులేని అక్షరం స్థిరరాశి *buf = బఫరు;

  చేయి
    {
      స్థిరరాశి ssize_t written = లిఖ (fd, buf, కొలత);
      /* POSIX says that calling write with SIZE of zero may detect and
         return errors.  If no error occurs, or write makes no attempt
         to detect errors, then write returns zero with no other
         results.  write_fail will return successfully in this case.  */
      ఒకవేళ (written == 0)
        {
          ఒకవేళ (కొలత == 0)
            తిరిగి true;
          లేకపోతే
            {
              /* If SIZE is greater than zero and write returns zero,
                 treat it as an error.  Some buggy drivers behave this
                 way.  See src/dd.c and Gnulib's lib/full-write.c for
                 more details.  */
              errno = ENOSPC;
              తిరిగి false;
            }
        }

      ఒకవేళ (written < 0)
        {
          /* Return an error if write detected one with a SIZE of zero.
             Otherwise, if SIZE is greater than zero, fail if it does
             not become writable.  */
          ఒకవేళ (కొలత == 0 || ! wait_for_nonblocking_write (fd))
            తిరిగి false;
        }
      లేకపోతే
        {
          buf += written;
          కొలత -= written;
        }
    }
  అయితే (0 < కొలత);

  తిరిగి true;
}
