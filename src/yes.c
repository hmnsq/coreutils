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
#परिभाषय क्रमादेशः_NAME "yes"

#परिभाषय AUTHORS proper_name ("David MacKenzie")

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [STRING]...\n\
  or:  %s OPTION\n\
"),
              क्रमादेशः_name, क्रमादेशः_name);

      fputs (_("\
Repeatedly output a पङ्क्तिः with all specified STRING(s), or 'y'.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Fill DEST[0..BUFSIZE-1] with repeated copies of SRC[0..SRCSIZE-1],
   doubling the copy size each iteration.  DEST may equal SRC.  */

स्थिर शून्य
repeat_आदर्शः (वर्ण *dest, वर्ण ध्रुव *src, idx_t srcsize, idx_t bufsize)
{
  यदि (dest != src)
    स्मृतिं_प्रतिलिख (dest, src, srcsize);
  प्रति (idx_t filled = srcsize; filled < bufsize; )
    {
      idx_t chunk = MIN (filled, bufsize - filled);
      स्मृतिं_प्रतिलिख (dest + filled, dest, chunk);
      filled += chunk;
    }
}

#यदि HAVE_SPLICE

/* Enlarge a pipe towards SPLICE_PIPE_SIZE and return the actual
   capacity as a quarter of the pipe size (the empirical sweet spot
   for vmsplice throughput), rounded down to a multiple of COPYSIZE.
   Return 0 if the result would be smaller than COPYSIZE.  */

स्थिर idx_t
pipe_splice_आकार (पूर्णाङ्क fd, idx_t copysize)
{
  size_t buf_cap = increase_pipe_आकार (fd) / 4;
  प्रत्यागम buf_cap / copysize * copysize;
}

#यदि_अन्तः

/* Repeatedly write the COPYSIZE-byte pattern in BUF to standard output
   using vmsplice/splice zero-copy I/O.  Since the data never varies,
   SPLICE_F_GIFT tells the kernel the pages will not be modified.

   Return TRUE if splice I/O was used (caller should check errno and
   report any error).  Return FALSE if splice could not be used.  */

स्थिर bool
splice_write (MAYBE_अप्रयुक्तम् वर्ण ध्रुव *buf, MAYBE_अप्रयुक्तम् idx_t copysize)
{
  bool output_started = false;
#यदि HAVE_SPLICE
  idx_t page_आकार = getpagesize ();

  bool stdout_is_pipe = isapipe (STDOUT_FILENO) > 0;

  /* Determine buffer size: enlarge the target pipe,
     then use 1/4 of actual capacity as the transfer size.  */
  पूर्णाङ्क pipefd[2] = { -1, -1 };
  idx_t splice_bufsize;
  वर्ण *splice_buf = रिक्त;

  यदि (stdout_is_pipe)
    splice_bufsize = pipe_splice_आकार (STDOUT_FILENO, copysize);
  अन्यथा
    {
      यदि (pipe2 (pipefd, 0) < 0)
        प्रत्यागम false;
      splice_bufsize = pipe_splice_आकार (pipefd[0], copysize);
    }

  यदि (splice_bufsize == 0)
    गच्छ समाप्तम्;

  /* Allocate page-aligned buffer for vmsplice.
     Needed with SPLICE_F_GIFT, but generally good for performance.  */
  यदि (! (splice_buf = alignalloc (page_आकार, splice_bufsize)))
    गच्छ समाप्तम्;

  repeat_आदर्शः (splice_buf, buf, copysize, splice_bufsize);

  /* For the pipe case, vmsplice directly to stdout.
     For the non-pipe case, vmsplice into the intermediate pipe
     and then splice from it to stdout.  */
  पूर्णाङ्क vmsplice_fd = stdout_is_pipe ? STDOUT_FILENO : pipefd[1];

  प्रति (;;)
    {
      संरचना iovec iov = { .iov_आधारः = splice_buf,
                           .iov_len = splice_bufsize };

      यावत् (iov.iov_len > 0)
        {
          /* Use SPLICE_F_{GIFT,MOVE} to allow the kernel to take references
             to the pages.  I.e., we're indicating we won't make changes.
             SPLICE_F_GIFT is only appropriate for full pages.  */
          अचिह्नित पूर्णाङ्क flags = iov.iov_len % page_आकार ? 0 : SPLICE_F_GIFT;
          ssize_t n = vmsplice (vmsplice_fd, &iov, 1, flags);
          यदि (n <= 0)
            गच्छ समाप्तम्;

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
                    गच्छ समाप्तम्;
                  output_started = true;
                  remaining -= s;
                }
            }

          iov.iov_आधारः = (वर्ण *) iov.iov_आधारः + n;
          iov.iov_len -= n;
        }
    }

समाप्तम्:
  यदि (pipefd[0] >= 0)
    {
      पूर्णाङ्क saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
    }
  alignfree (splice_buf);
#यदि_अन्तः
  प्रत्यागम output_started;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, क्रमादेशः_NAME, PACKAGE_NAME,
                                   संस्करणम्, true, usage, AUTHORS,
                                   (वर्ण ध्रुव *) रिक्त);

  वर्ण **operands = argv + optind;
  वर्ण **सङ्कार्यम्_lim = argv + argc;
  यदि (optind == argc)
    *सङ्कार्यम्_lim++ = bad_cast ("y");

  /* Buffer data locally once, rather than having the
     large overhead of stdio buffering each item.  */
  size_t bufalloc = 0;
  bool reuse_सङ्कार्यम्_वाक्यानि = true;
  वर्ण **operandp = operands;
  कुरु
    {
      size_t सङ्कार्यम्_len = वाक्यदैर्घ्यम् (*operandp);
      bufalloc += सङ्कार्यम्_len + 1;
      यदि (operandp + 1 < सङ्कार्यम्_lim
          && *operandp + सङ्कार्यम्_len + 1 != operandp[1])
        reuse_सङ्कार्यम्_वाक्यानि = false;
    }
  यावत् (++operandp < सङ्कार्यम्_lim);

  /* Improve performance by using a buffer size greater than BUFSIZ / 2.  */
  यदि (bufalloc <= BUFSIZ / 2)
    {
      bufalloc = BUFSIZ;
      reuse_सङ्कार्यम्_वाक्यानि = false;
    }

#यदि defined __CHERI__
  /* Cheri capability bounds do not allow for this.  */
  reuse_सङ्कार्यम्_वाक्यानि = false;
#यदि_अन्तः

  /* Fill the buffer with one copy of the output.  If possible, reuse
     the operands strings; this wins when the buffer would be large.  */
  वर्ण *buf = reuse_सङ्कार्यम्_वाक्यानि ? *operands : xmalloc (bufalloc);
  size_t bufused = 0;
  operandp = operands;
  कुरु
    {
      size_t सङ्कार्यम्_len = वाक्यदैर्घ्यम् (*operandp);
      यदि (! reuse_सङ्कार्यम्_वाक्यानि)
        स्मृतिं_प्रतिलिख (buf + bufused, *operandp, सङ्कार्यम्_len);
      bufused += सङ्कार्यम्_len;
      buf[bufused++] = ' ';
    }
  यावत् (++operandp < सङ्कार्यम्_lim);
  buf[bufused - 1] = '\n';

  idx_t copysize = bufused;

  /* Repeatedly output the buffer until there is a write error; then fail.
     Do a minimal write first to check output with minimal set up cost.
     If successful then set up for efficient repetition.  */
  यदि (पूर्णम्_write (STDOUT_FILENO, buf, copysize) == copysize
      && splice_write (buf, copysize) == 0)
    {
      /* If a larger buffer was allocated, fill it by repeated copies.  */
      bufused = bufalloc / copysize * copysize;
      यदि (bufused > copysize)
        repeat_आदर्शः (buf, buf, copysize, bufused);
      यावत् (पूर्णम्_write (STDOUT_FILENO, buf, bufused) == bufused)
        अनुवर्त;
    }

  error (0, errno, _("standard output"));
  main_exit (EXIT_FAILURE);
}
