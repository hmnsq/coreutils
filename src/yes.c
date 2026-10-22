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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/uio.h>

#अन्तर्भाव "system.h"

#अन्तर्भाव "alignalloc.h"
#अन्तर्भाव "full-write.h"
#अन्तर्भाव "isapipe.h"
#अन्तर्भाव "long-options.h"
#अन्तर्भाव "splice.h"
#अन्तर्भाव "unistd--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "yes"

#define AUTHORS proper_name ("David MacKenzie")

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      printf (_("\
Usage: %s [STRING]...\n\
  or:  %s OPTION\n\
"),
              program_name, program_name);

      fputs (_("\
Repeatedly output a पंक्ति with all specified STRING(s), or 'y'.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Fill DEST[0..BUFSIZE-1] with repeated copies of SRC[0..SRCSIZE-1],
   doubling the copy size each iteration.  DEST may equal SRC.  */

स्थिर शून्य
repeat_pattern (वर्ण *dest, वर्ण ध्रुव *src, idx_t srcsize, idx_t bufsize)
{
  यदि (dest != src)
    memcpy (dest, src, srcsize);
  प्रति (idx_t filled = srcsize; filled < bufsize; )
    {
      idx_t chunk = MIN (filled, bufsize - filled);
      memcpy (dest + filled, dest, chunk);
      filled += chunk;
    }
}

#यदि HAVE_SPLICE

/* Enlarge a pipe towards SPLICE_PIPE_SIZE and return the actual
   capacity as a quarter of the pipe size (the empirical sweet spot
   for vmsplice throughput), rounded down to a multiple of COPYSIZE.
   Return 0 if the result would be smaller than COPYSIZE.  */

स्थिर idx_t
pipe_splice_size (पूर्णाङ्क fd, idx_t copysize)
{
  size_t buf_cap = increase_pipe_size (fd) / 4;
  प्रत्यागम buf_cap / copysize * copysize;
}

#endif

/* Repeatedly write the COPYSIZE-byte pattern in BUF to standard output
   using vmsplice/splice zero-copy I/O.  Since the data never varies,
   SPLICE_F_GIFT tells the kernel the pages will not be modified.

   Return TRUE if splice I/O was used (caller should check errno and
   report any error).  Return FALSE if splice could not be used.  */

स्थिर bool
splice_write (MAYBE_UNUSED वर्ण ध्रुव *buf, MAYBE_UNUSED idx_t copysize)
{
  bool output_started = false;
#यदि HAVE_SPLICE
  idx_t page_size = getpagesize ();

  bool stdout_is_pipe = isapipe (STDOUT_FILENO) > 0;

  /* Determine buffer size: enlarge the target pipe,
     then use 1/4 of actual capacity as the transfer size.  */
  पूर्णाङ्क pipefd[2] = { -1, -1 };
  idx_t splice_bufsize;
  वर्ण *splice_buf = रिक्त;

  यदि (stdout_is_pipe)
    splice_bufsize = pipe_splice_size (STDOUT_FILENO, copysize);
  अन्यथा
    {
      यदि (pipe2 (pipefd, 0) < 0)
        प्रत्यागम false;
      splice_bufsize = pipe_splice_size (pipefd[0], copysize);
    }

  यदि (splice_bufsize == 0)
    गच्छ done;

  /* Allocate page-aligned buffer for vmsplice.
     Needed with SPLICE_F_GIFT, but generally good for performance.  */
  यदि (! (splice_buf = alignalloc (page_size, splice_bufsize)))
    गच्छ done;

  repeat_pattern (splice_buf, buf, copysize, splice_bufsize);

  /* For the pipe case, vmsplice directly to stdout.
     For the non-pipe case, vmsplice into the intermediate pipe
     and then splice from it to stdout.  */
  पूर्णाङ्क vmsplice_fd = stdout_is_pipe ? STDOUT_FILENO : pipefd[1];

  प्रति (;;)
    {
      संरचना iovec iov = { .iov_base = splice_buf,
                           .iov_len = splice_bufsize };

      यावत् (iov.iov_len > 0)
        {
          /* Use SPLICE_F_{GIFT,MOVE} to allow the kernel to take references
             to the pages.  I.e., we're indicating we won't make changes.
             SPLICE_F_GIFT is only appropriate for full pages.  */
          unsigned पूर्णाङ्क flags = iov.iov_len % page_size ? 0 : SPLICE_F_GIFT;
          ssize_t n = vmsplice (vmsplice_fd, &iov, 1, flags);
          यदि (n <= 0)
            गच्छ done;

          यदि (stdout_is_pipe)
            output_started = true;
          अन्यथा
            {
              idx_t remaining = n;
              यावत् (remaining > 0)
                {
                  ssize_t s = splice (pipefd[0], रिक्त, STDOUT_FILENO, रिक्त,
                                      remaining, SPLICE_F_MOVE);
                  यदि (s <= 0)
                    गच्छ done;
                  output_started = true;
                  remaining -= s;
                }
            }

          iov.iov_base = (वर्ण *) iov.iov_base + n;
          iov.iov_len -= n;
        }
    }

done:
  यदि (pipefd[0] >= 0)
    {
      पूर्णाङ्क saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
    }
  alignfree (splice_buf);
#endif
  प्रत्यागम output_started;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (वर्ण ध्रुव *) रिक्त);

  वर्ण **operands = argv + optind;
  वर्ण **operand_lim = argv + argc;
  यदि (optind == argc)
    *operand_lim++ = bad_cast ("y");

  /* Buffer data locally once, rather than having the
     large overhead of stdio buffering each item.  */
  size_t bufalloc = 0;
  bool reuse_operand_strings = true;
  वर्ण **operandp = operands;
  कुरु
    {
      size_t operand_len = strlen (*operandp);
      bufalloc += operand_len + 1;
      यदि (operandp + 1 < operand_lim
          && *operandp + operand_len + 1 != operandp[1])
        reuse_operand_strings = false;
    }
  यावत् (++operandp < operand_lim);

  /* Improve performance by using a buffer size greater than BUFSIZ / 2.  */
  यदि (bufalloc <= BUFSIZ / 2)
    {
      bufalloc = BUFSIZ;
      reuse_operand_strings = false;
    }

#यदि defined __CHERI__
  /* Cheri capability bounds do not allow for this.  */
  reuse_operand_strings = false;
#endif

  /* Fill the buffer with one copy of the output.  If possible, reuse
     the operands strings; this wins when the buffer would be large.  */
  वर्ण *buf = reuse_operand_strings ? *operands : xmalloc (bufalloc);
  size_t bufused = 0;
  operandp = operands;
  कुरु
    {
      size_t operand_len = strlen (*operandp);
      यदि (! reuse_operand_strings)
        memcpy (buf + bufused, *operandp, operand_len);
      bufused += operand_len;
      buf[bufused++] = ' ';
    }
  यावत् (++operandp < operand_lim);
  buf[bufused - 1] = '\n';

  idx_t copysize = bufused;

  /* Repeatedly output the buffer until there is a write error; then fail.
     Do a minimal write first to check output with minimal set up cost.
     If successful then set up for efficient repetition.  */
  यदि (full_write (STDOUT_FILENO, buf, copysize) == copysize
      && splice_write (buf, copysize) == 0)
    {
      /* If a larger buffer was allocated, fill it by repeated copies.  */
      bufused = bufalloc / copysize * copysize;
      यदि (bufused > copysize)
        repeat_pattern (buf, buf, copysize, bufused);
      यावत् (full_write (STDOUT_FILENO, buf, bufused) == bufused)
        अनुवर्त;
    }

  error (0, errno, _("standard output"));
  main_exit (EXIT_FAILURE);
}
