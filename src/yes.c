/* yes - output a string repeatedly until killed
   Copyright (C) 1991-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* David MacKenzie <djm@gnu.ai.mit.edu> */

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/uio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "alignalloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "full-write.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "isapipe.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "long-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "splice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unistd--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "yes"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [STRING]...\n\
  or:  %s OPTION\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);

      fputs (_("\
Repeatedly output a ਸਤਰ with all specified STRING(s), or 'y'.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Fill DEST[0..BUFSIZE-1] with repeated copies of SRC[0..SRCSIZE-1],
   doubling the copy size each iteration.  DEST may equal SRC.  */

ਸਥਿਰ ਖਾਲੀ
repeat_ਨਮੂਨਾ (ਅੱਖਰ *dest, ਅੱਖਰ ਅਚਲ *src, idx_t srcsize, idx_t bufsize)
{
  ਜੇ (dest != src)
    ਸਮ੍ਰਿਤੀ_ਨਕਲ (dest, src, srcsize);
  ਲਈ (idx_t filled = srcsize; filled < bufsize; )
    {
      idx_t chunk = MIN (filled, bufsize - filled);
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (dest + filled, dest, chunk);
      filled += chunk;
    }
}

#ਜੇ HAVE_SPLICE

/* Enlarge a pipe towards SPLICE_PIPE_SIZE and return the actual
   capacity as a quarter of the pipe size (the empirical sweet spot
   for vmsplice throughput), rounded down to a multiple of COPYSIZE.
   Return 0 if the result would be smaller than COPYSIZE.  */

ਸਥਿਰ idx_t
pipe_splice_ਮਾਪ (ਪੂਰਨ fd, idx_t copysize)
{
  size_t buf_cap = increase_pipe_ਮਾਪ (fd) / 4;
  ਵਾਪਸ buf_cap / copysize * copysize;
}

#ਜੇ_ਅੰਤ

/* Repeatedly write the COPYSIZE-byte pattern in BUF to standard output
   using vmsplice/splice zero-copy I/O.  Since the data never varies,
   SPLICE_F_GIFT tells the kernel the pages will not be modified.

   Return TRUE if splice I/O was used (caller should check errno and
   report any error).  Return FALSE if splice could not be used.  */

ਸਥਿਰ bool
splice_write (MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ ਅਚਲ *buf, MAYBE_ਅਣਵਰਤਿਆ idx_t copysize)
{
  bool output_started = false;
#ਜੇ HAVE_SPLICE
  idx_t ਪੰਨਾ_ਮਾਪ = getpagesize ();

  bool stdout_is_pipe = isapipe (STDOUT_FILENO) > 0;

  /* Determine buffer size: enlarge the target pipe,
     then use 1/4 of actual capacity as the transfer size.  */
  ਪੂਰਨ pipefd[2] = { -1, -1 };
  idx_t splice_bufsize;
  ਅੱਖਰ *splice_buf = ਨਿਰਾ;

  ਜੇ (stdout_is_pipe)
    splice_bufsize = pipe_splice_ਮਾਪ (STDOUT_FILENO, copysize);
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (pipe2 (pipefd, 0) < 0)
        ਵਾਪਸ false;
      splice_bufsize = pipe_splice_ਮਾਪ (pipefd[0], copysize);
    }

  ਜੇ (splice_bufsize == 0)
    ਜਾਓ ਪੂਰਾ;

  /* Allocate page-aligned buffer for vmsplice.
     Needed with SPLICE_F_GIFT, but generally good for performance.  */
  ਜੇ (! (splice_buf = alignalloc (ਪੰਨਾ_ਮਾਪ, splice_bufsize)))
    ਜਾਓ ਪੂਰਾ;

  repeat_ਨਮੂਨਾ (splice_buf, buf, copysize, splice_bufsize);

  /* For the pipe case, vmsplice directly to stdout.
     For the non-pipe case, vmsplice into the intermediate pipe
     and then splice from it to stdout.  */
  ਪੂਰਨ vmsplice_fd = stdout_is_pipe ? STDOUT_FILENO : pipefd[1];

  ਲਈ (;;)
    {
      ਬਣਤਰ iovec iov = { .iov_ਅਧਾਰ = splice_buf,
                           .iov_len = splice_bufsize };

      ਜਦੋਂ (iov.iov_len > 0)
        {
          /* Use SPLICE_F_{GIFT,MOVE} to allow the kernel to take references
             to the pages.  I.e., we're indicating we won't make changes.
             SPLICE_F_GIFT is only appropriate for full pages.  */
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = iov.iov_len % ਪੰਨਾ_ਮਾਪ ? 0 : SPLICE_F_GIFT;
          ssize_t n = vmsplice (vmsplice_fd, &iov, 1, flags);
          ਜੇ (n <= 0)
            ਜਾਓ ਪੂਰਾ;

          ਜੇ (stdout_is_pipe)
            output_started = true;
          ਨਹੀਂਤਾਂ
            {
              idx_t ਬਚਿਆ = n;
              ਜਦੋਂ (ਬਚਿਆ > 0)
                {
                  ssize_t s = splice (pipefd[0], ਨਿਰਾ, STDOUT_FILENO, ਨਿਰਾ,
                                      ਬਚਿਆ, SPLICE_F_MOVE);
                  ਜੇ (s <= 0)
                    ਜਾਓ ਪੂਰਾ;
                  output_started = true;
                  ਬਚਿਆ -= s;
                }
            }

          iov.iov_ਅਧਾਰ = (ਅੱਖਰ *) iov.iov_ਅਧਾਰ + n;
          iov.iov_len -= n;
        }
    }

ਪੂਰਾ:
  ਜੇ (pipefd[0] >= 0)
    {
      ਪੂਰਨ saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
    }
  alignfree (splice_buf);
#ਜੇ_ਅੰਤ
  ਵਾਪਸ output_started;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE_NAME,
                                   ਸੰਸਕਰਣ, true, usage, AUTHORS,
                                   (ਅੱਖਰ ਅਚਲ *) ਨਿਰਾ);

  ਅੱਖਰ **operands = argv + optind;
  ਅੱਖਰ **ਸੰਕਾਰਜ_lim = argv + argc;
  ਜੇ (optind == argc)
    *ਸੰਕਾਰਜ_lim++ = bad_cast ("y");

  /* Buffer data locally once, rather than having the
     large overhead of stdio buffering each item.  */
  size_t bufalloc = 0;
  bool reuse_ਸੰਕਾਰਜ_ਲੜੀਆਂ = true;
  ਅੱਖਰ **operandp = operands;
  ਕਰੋ
    {
      size_t ਸੰਕਾਰਜ_len = ਲੜੀ_ਲੰਬਾਈ (*operandp);
      bufalloc += ਸੰਕਾਰਜ_len + 1;
      ਜੇ (operandp + 1 < ਸੰਕਾਰਜ_lim
          && *operandp + ਸੰਕਾਰਜ_len + 1 != operandp[1])
        reuse_ਸੰਕਾਰਜ_ਲੜੀਆਂ = false;
    }
  ਜਦੋਂ (++operandp < ਸੰਕਾਰਜ_lim);

  /* Improve performance by using a buffer size greater than BUFSIZ / 2.  */
  ਜੇ (bufalloc <= BUFSIZ / 2)
    {
      bufalloc = BUFSIZ;
      reuse_ਸੰਕਾਰਜ_ਲੜੀਆਂ = false;
    }

#ਜੇ defined __CHERI__
  /* Cheri capability bounds do not allow for this.  */
  reuse_ਸੰਕਾਰਜ_ਲੜੀਆਂ = false;
#ਜੇ_ਅੰਤ

  /* Fill the buffer with one copy of the output.  If possible, reuse
     the operands strings; this wins when the buffer would be large.  */
  ਅੱਖਰ *buf = reuse_ਸੰਕਾਰਜ_ਲੜੀਆਂ ? *operands : xmalloc (bufalloc);
  size_t bufused = 0;
  operandp = operands;
  ਕਰੋ
    {
      size_t ਸੰਕਾਰਜ_len = ਲੜੀ_ਲੰਬਾਈ (*operandp);
      ਜੇ (! reuse_ਸੰਕਾਰਜ_ਲੜੀਆਂ)
        ਸਮ੍ਰਿਤੀ_ਨਕਲ (buf + bufused, *operandp, ਸੰਕਾਰਜ_len);
      bufused += ਸੰਕਾਰਜ_len;
      buf[bufused++] = ' ';
    }
  ਜਦੋਂ (++operandp < ਸੰਕਾਰਜ_lim);
  buf[bufused - 1] = '\n';

  idx_t copysize = bufused;

  /* Repeatedly output the buffer until there is a write error; then fail.
     Do a minimal write first to check output with minimal set up cost.
     If successful then set up for efficient repetition.  */
  ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, buf, copysize) == copysize
      && splice_write (buf, copysize) == 0)
    {
      /* If a larger buffer was allocated, fill it by repeated copies.  */
      bufused = bufalloc / copysize * copysize;
      ਜੇ (bufused > copysize)
        repeat_ਨਮੂਨਾ (buf, buf, copysize, bufused);
      ਜਦੋਂ (ਭਰਿਆ_write (STDOUT_FILENO, buf, bufused) == bufused)
        ਜਾਰੀ;
    }

  error (0, errno, _("standard output"));
  main_exit (EXIT_FAILURE);
}
