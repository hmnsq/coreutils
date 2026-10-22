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

#समावेश <config.h>

/* poll(2) is needed on AIX (where 'select' gives a readable
   event immediately) and Solaris (where 'select' never gave
   a readable event).  Also use poll(2) on systems we know work
   and/or are already using poll (linux).  On macOS we know poll()
   doesn't work on 10.5, while it does work on >= 11 at least. */
#अगर defined _AIX || defined __sun \
    || defined __linux__ || defined __ANDROID__ \
    || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 110000
# define IOPOLL_USES_POLL 1
  /* Check we've not enabled gnulib's poll module
     as that will emulate poll() in a way not
     currently compatible with our usage.  */
# अगर defined HAVE_POLL
#  error "gnulib's poll() replacement is currently incompatible"
# endif
#पूर्ण_अगर

#अगर IOPOLL_USES_POLL
# include <poll.h>
#अन्यथा
# include <sys/select.h>
#पूर्ण_अगर

#समावेश "system.h"
#समावेश "assure.h"
#समावेश "iopoll.h"
#समावेश "isapipe.h"


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

अटल पूर्णांक
iopoll_internal (पूर्णांक fdin, पूर्णांक fdout, bool block, bool broken_output)
{
  affirm (fdin != -1 || fdout != -1);

#अगर IOPOLL_USES_POLL
  काष्ठा pollfd pfds[2] = {  /* POLLRDBAND needed for illumos, macOS.  */
    { .fd = fdin,  .events = POLLIN | POLLRDBAND, .revents = 0 },
    { .fd = fdout, .events = POLLRDBAND, .revents = 0 },
  };
  पूर्णांक check_out_events = POLLERR | POLLHUP | POLLNVAL;
  पूर्णांक ret = 0;

  अगर (! broken_output)
    {
      pfds[0].events = pfds[1].events = POLLOUT;
      check_out_events = POLLOUT;
    }

  जबतक (0 <= ret || त्रुटि_सं == EINTR)
    {
      ret = poll (pfds, 2, block ? -1 : 0);

      अगर (ret < 0)
        जारी;
      अगर (ret == 0 && ! block)
        वापस 0;
      affirm (0 < ret);
      अगर (pfds[0].revents) /* input available or pipe closed indicating EOF; */
        वापस 0;          /* should now be able to read() without blocking  */
      अगर (pfds[1].revents & check_out_events)
        वापस broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#अन्यथा  /* fall back to select()-based implementation */

  पूर्णांक nfds = (fdin > fdout ? fdin : fdout) + 1;
  पूर्णांक ret = 0;

  अगर (FD_SETSIZE < nfds)
    {
      त्रुटि_सं = EINVAL;
      ret = -1;
    }

  /* If fdout has an error condition (like a broken pipe) it will be seen
     as ready for reading.  Assumes fdout is not actually readable.  */
  जबतक (0 <= ret || त्रुटि_सं == EINTR)
    {
      fd_set fds;
      FD_ZERO (&fds);
      अगर (0 <= fdin)
        FD_SET (fdin, &fds);
      अगर (0 <= fdout)
        FD_SET (fdout, &fds);

      काष्ठा timeval delay = {0};
      ret = select (nfds,
                    broken_output ? &fds : शून्य,
                    broken_output ? शून्य : &fds,
                    शून्य, block ? शून्य : &delay);

      अगर (ret < 0)
        जारी;
      अगर (ret == 0 && ! block)
        वापस 0;
      affirm (0 < ret);
      अगर (0 <= fdin && FD_ISSET (fdin, &fds))    /* input available or EOF; */
        वापस 0;          /* should now be able to read() without blocking */
      अगर (0 <= fdout && FD_ISSET (fdout, &fds))  /* equiv to POLLERR        */
        वापस broken_output ? IOPOLL_BROKEN_OUTPUT : 0;
    }

#पूर्ण_अगर
  वापस IOPOLL_ERROR;
}

बाह्य पूर्णांक
iopoll (पूर्णांक fdin, पूर्णांक fdout, bool block)
{
  वापस iopoll_internal (fdin, fdout, block, true);
}



/* Return true if fdin is relevant for iopoll().
   An fd is not relevant for iopoll() if it is always ready for reading,
   which is the case for a regular file or block device.  */

बाह्य bool
iopoll_निवेश_ok (पूर्णांक fdin)
{
  काष्ठा stat st;
  bool always_ready = ख_स्थिति (fdin, &st) == 0
                      && (S_ISREG (st.st_mode)
                          || S_ISBLK (st.st_mode));
  वापस ! always_ready;
}

/* Return true if fdout is suitable for iopoll().
   Namely, fdout refers to a pipe.  */

बाह्य bool
iopoll_output_ok (पूर्णांक fdout)
{
  वापस isapipe (fdout) > 0;
}

#अगर_घोषित EWOULDBLOCK
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN || (errcode) == EWOULDBLOCK)
#अन्यथा
# define IS_EAGAIN(errcode) ((errcode) == EAGAIN)
#पूर्ण_अगर

/* Inspect the errno of the previous syscall.
   On EAGAIN, wait for the underlying file descriptor to become writable.
   Return true, if EAGAIN has been successfully handled. */

अटल bool
wait_for_nonblocking_write (पूर्णांक fd)
{
  अगर (! IS_EAGAIN (त्रुटि_सं))
    /* non-recoverable write error */
    वापस false;

  /* wait for the file descriptor to become writable */
  अगर (iopoll_internal (-1, fd, true, false) != 0)
    {
      त्रुटि_सं = EAGAIN;
      वापस false;
    }

  /* successfully waited for the descriptor to become writable */
  वापस true;
}

/* wrapper for write() that also waits for FD if non blocking.  */

बाह्य bool
write_wait (पूर्णांक fd, व्योम स्थिर *बफर, माप_प्रकार आकार)
{
  अचिन्हित अक्षर स्थिर *buf = बफर;

  करो
    {
      स्थिर ssize_t written = ग_लिखो (fd, buf, आकार);
      /* POSIX says that calling write with SIZE of zero may detect and
         return errors.  If no error occurs, or write makes no attempt
         to detect errors, then write returns zero with no other
         results.  write_fail will return successfully in this case.  */
      अगर (written == 0)
        {
          अगर (आकार == 0)
            वापस true;
          अन्यथा
            {
              /* If SIZE is greater than zero and write returns zero,
                 treat it as an error.  Some buggy drivers behave this
                 way.  See src/dd.c and Gnulib's lib/full-write.c for
                 more details.  */
              त्रुटि_सं = ENOSPC;
              वापस false;
            }
        }

      अगर (written < 0)
        {
          /* Return an error if write detected one with a SIZE of zero.
             Otherwise, if SIZE is greater than zero, fail if it does
             not become writable.  */
          अगर (आकार == 0 || ! wait_for_nonblocking_write (fd))
            वापस false;
        }
      अन्यथा
        {
          buf += written;
          आकार -= written;
        }
    }
  जबतक (0 < आकार);

  वापस true;
}
