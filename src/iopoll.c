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

#അന്തര്ഭാവ <config.h>

/* poll(2) is needed on AIX (where 'select' gives a readable
   event immediately) and Solaris (where 'select' never gave
   a readable event).  Also use poll(2) on systems we know work
   and/or are already using poll (linux).  On macOS we know poll()
   doesn't work on 10.5, while it does work on >= 11 at least. */
#എങ്കിൽ defined _AIX || defined __sun \
    || defined __linux__ || defined __ANDROID__ \
    || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 110000
# define IOPOLL_USES_POLL 1
  /* Check we've not enabled gnulib's poll module
     as that will emulate poll() in a way not
     currently compatible with our usage.  */
# എങ്കിൽ defined HAVE_POLL
#  error "gnulib's poll() replacement is currently incompatible"
# endif
#endif

#എങ്കിൽ IOPOLL_USES_POLL
# include <poll.h>
#അല്ലെങ്കിൽ
# include <sys/select.h>
#endif

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "iopoll.h"
#അന്തര്ഭാവ "isapipe.h"


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

സ്ഥിതം പൂർണ്ണസംഖ്യ
iopoll_internal (പൂർണ്ണസംഖ്യ fdin, പൂർണ്ണസംഖ്യ fdout, bool block, bool broken_output)
{
  affirm (fdin != -1 || fdout != -1);

#എങ്കിൽ IOPOLL_USES_POLL
  ഘടന pollfd pfds[2] = {  /* POLLRDBAND needed for illumos, macOS.  */
    { .fd = fdin,  .events = POLLIN | POLLRDBAND, .revents = 0 },
    { .fd = fdout, .events = POLLRDBAND, .revents = 0 },
  };
  പൂർണ്ണസംഖ്യ check_out_events = POLLERR | POLLHUP | POLLNVAL;
  പൂർണ്ണസംഖ്യ ret = 0;

  എങ്കിൽ (! broken_output)
    {
      pfds[0].events = pfds[1].events = POLLOUT;
      check_out_events = POLLOUT;
    }

  ആകുമ്പോൾ (0 <= ret || errno == EINTR)
    {
      ret = poll (pfds, 2, block ? -1 : 0);

      എങ്കിൽ (ret < 0)
        തുടരുക;
      എങ്കിൽ (ret == 0 && ! block)
        തിരികെ 0;
      affirm (0 < ret);
      എങ്കിൽ (pfds[0].revents) /* input available or pipe closed indicating EOF; */
        തിരികെ 0;          /* should now be able to read() without blocking  */
      എങ്കിൽ (pfds[1].revents & check_out_events)
        തിരികെ broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#അല്ലെങ്കിൽ  /* fall back to select()-based implementation */

  പൂർണ്ണസംഖ്യ nfds = (fdin > fdout ? fdin : fdout) + 1;
  പൂർണ്ണസംഖ്യ ret = 0;

  എങ്കിൽ (FD_SETSIZE < nfds)
    {
      errno = EINVAL;
      ret = -1;
    }

  /* If fdout has an error condition (like a broken pipe) it will be seen
     as ready for reading.  Assumes fdout is not actually readable.  */
  ആകുമ്പോൾ (0 <= ret || errno == EINTR)
    {
      fd_set fds;
      FD_ZERO (&fds);
      എങ്കിൽ (0 <= fdin)
        FD_SET (fdin, &fds);
      എങ്കിൽ (0 <= fdout)
        FD_SET (fdout, &fds);

      ഘടന timeval delay = {0};
      ret = select (nfds,
                    broken_output ? &fds : ശൂന്യ,
                    broken_output ? ശൂന്യ : &fds,
                    ശൂന്യ, block ? ശൂന്യ : &delay);

      എങ്കിൽ (ret < 0)
        തുടരുക;
      എങ്കിൽ (ret == 0 && ! block)
        തിരികെ 0;
      affirm (0 < ret);
      എങ്കിൽ (0 <= fdin && FD_ISSET (fdin, &fds))    /* input available or EOF; */
        തിരികെ 0;          /* should now be able to read() without blocking */
      എങ്കിൽ (0 <= fdout && FD_ISSET (fdout, &fds))  /* equiv to POLLERR        */
        തിരികെ broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#endif
  തിരികെ IOPOLL_ERROR;
}

ബാഹ്യ പൂർണ്ണസംഖ്യ
iopoll (പൂർണ്ണസംഖ്യ fdin, പൂർണ്ണസംഖ്യ fdout, bool block)
{
  തിരികെ iopoll_internal (fdin, fdout, block, true);
}



/* Return true if fdin is relevant for iopoll().
   An fd is not relevant for iopoll() if it is always ready for reading,
   which is the case for a regular file or block device.  */

ബാഹ്യ bool
iopoll_നിവേശം_ok (പൂർണ്ണസംഖ്യ fdin)
{
  ഘടന stat st;
  bool always_ready = fstat (fdin, &st) == 0
                      && (S_ISREG (st.st_mode)
                          || S_ISBLK (st.st_mode));
  തിരികെ ! always_ready;
}

/* Return true if fdout is suitable for iopoll().
   Namely, fdout refers to a pipe.  */

ബാഹ്യ bool
iopoll_output_ok (പൂർണ്ണസംഖ്യ fdout)
{
  തിരികെ isapipe (fdout) > 0;
}

#ifdef EWOULDBLOCK
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN || (errcode) == EWOULDBLOCK)
#അല്ലെങ്കിൽ
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN)
#endif

/* Inspect the errno of the previous syscall.
   On EAGAIN, wait for the underlying file descriptor to become writable.
   Return true, if EAGAIN has been successfully handled. */

സ്ഥിതം bool
wait_for_nonblocking_write (പൂർണ്ണസംഖ്യ fd)
{
  എങ്കിൽ (! IS_EAGAIN (errno))
    /* non-recoverable write error */
    തിരികെ false;

  /* wait for the file descriptor to become writable */
  എങ്കിൽ (iopoll_internal (-1, fd, true, false) != 0)
    {
      errno = EAGAIN;
      തിരികെ false;
    }

  /* successfully waited for the descriptor to become writable */
  തിരികെ true;
}

/* wrapper for write() that also waits for FD if non blocking.  */

ബാഹ്യ bool
write_wait (പൂർണ്ണസംഖ്യ fd, ശൂന്യം സ്ഥിരം *buffer, size_t size)
{
  unsigned അക്ഷരം സ്ഥിരം *buf = buffer;

  ചെയ്യുക
    {
      സ്ഥിരം ssize_t written = ലിഖ (fd, buf, size);
      /* POSIX says that calling write with SIZE of zero may detect and
         return errors.  If no error occurs, or write makes no attempt
         to detect errors, then write returns zero with no other
         results.  write_fail will return successfully in this case.  */
      എങ്കിൽ (written == 0)
        {
          എങ്കിൽ (size == 0)
            തിരികെ true;
          അല്ലെങ്കിൽ
            {
              /* If SIZE is greater than zero and write returns zero,
                 treat it as an error.  Some buggy drivers behave this
                 way.  See src/dd.c and Gnulib's lib/full-write.c for
                 more details.  */
              errno = ENOSPC;
              തിരികെ false;
            }
        }

      എങ്കിൽ (written < 0)
        {
          /* Return an error if write detected one with a SIZE of zero.
             Otherwise, if SIZE is greater than zero, fail if it does
             not become writable.  */
          എങ്കിൽ (size == 0 || ! wait_for_nonblocking_write (fd))
            തിരികെ false;
        }
      അല്ലെങ്കിൽ
        {
          buf += written;
          size -= written;
        }
    }
  ആകുമ്പോൾ (0 < size);

  തിരികെ true;
}
