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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <sys/uio.h>

#شامل "system.h"

#شامل "alignalloc.h"
#شامل "full-write.h"
#شامل "isapipe.h"
#شامل "long-options.h"
#شامل "splice.h"
#شامل "unistd--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "yes"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [STRING]...\n\
  or:  %s OPTION\n\
"),
              پروگرام_name, پروگرام_name);

      fputs (_("\
Repeatedly output a سطر with all specified STRING(s), or 'y'.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Fill DEST[0..BUFSIZE-1] with repeated copies of SRC[0..SRCSIZE-1],
   doubling the copy size each iteration.  DEST may equal SRC.  */

ساکن خالی
repeat_نمونہ (حرف *dest, حرف ثابت *src, idx_t srcsize, idx_t bufsize)
{
  اگر (dest != src)
    حافظہ_نقل (dest, src, srcsize);
  برائے (idx_t filled = srcsize; filled < bufsize; )
    {
      idx_t chunk = MIN (filled, bufsize - filled);
      حافظہ_نقل (dest + filled, dest, chunk);
      filled += chunk;
    }
}

#اگر HAVE_SPLICE

/* Enlarge a pipe towards SPLICE_PIPE_SIZE and return the actual
   capacity as a quarter of the pipe size (the empirical sweet spot
   for vmsplice throughput), rounded down to a multiple of COPYSIZE.
   Return 0 if the result would be smaller than COPYSIZE.  */

ساکن idx_t
pipe_splice_سائز (صحیح fd, idx_t copysize)
{
  size_t buf_cap = increase_pipe_سائز (fd) / 4;
  واپس buf_cap / copysize * copysize;
}

#اگر_ختم

/* Repeatedly write the COPYSIZE-byte pattern in BUF to standard output
   using vmsplice/splice zero-copy I/O.  Since the data never varies,
   SPLICE_F_GIFT tells the kernel the pages will not be modified.

   Return TRUE if splice I/O was used (caller should check errno and
   report any error).  Return FALSE if splice could not be used.  */

ساکن bool
splice_write (MAYBE_غیر_مستعمل حرف ثابت *buf, MAYBE_غیر_مستعمل idx_t copysize)
{
  bool output_started = false;
#اگر HAVE_SPLICE
  idx_t صفحہ_سائز = getpagesize ();

  bool stdout_is_pipe = isapipe (STDOUT_FILENO) > 0;

  /* Determine buffer size: enlarge the target pipe,
     then use 1/4 of actual capacity as the transfer size.  */
  صحیح pipefd[2] = { -1, -1 };
  idx_t splice_bufsize;
  حرف *splice_buf = تہی;

  اگر (stdout_is_pipe)
    splice_bufsize = pipe_splice_سائز (STDOUT_FILENO, copysize);
  ورنہ
    {
      اگر (pipe2 (pipefd, 0) < 0)
        واپس false;
      splice_bufsize = pipe_splice_سائز (pipefd[0], copysize);
    }

  اگر (splice_bufsize == 0)
    جاؤ مکمل;

  /* Allocate page-aligned buffer for vmsplice.
     Needed with SPLICE_F_GIFT, but generally good for performance.  */
  اگر (! (splice_buf = alignalloc (صفحہ_سائز, splice_bufsize)))
    جاؤ مکمل;

  repeat_نمونہ (splice_buf, buf, copysize, splice_bufsize);

  /* For the pipe case, vmsplice directly to stdout.
     For the non-pipe case, vmsplice into the intermediate pipe
     and then splice from it to stdout.  */
  صحیح vmsplice_fd = stdout_is_pipe ? STDOUT_FILENO : pipefd[1];

  برائے (;;)
    {
      ساخت iovec iov = { .iov_بنیاد = splice_buf,
                           .iov_len = splice_bufsize };

      جب (iov.iov_len > 0)
        {
          /* Use SPLICE_F_{GIFT,MOVE} to allow the kernel to take references
             to the pages.  I.e., we're indicating we won't make changes.
             SPLICE_F_GIFT is only appropriate for full pages.  */
          غیرعلامتی صحیح flags = iov.iov_len % صفحہ_سائز ? 0 : SPLICE_F_GIFT;
          ssize_t n = vmsplice (vmsplice_fd, &iov, 1, flags);
          اگر (n <= 0)
            جاؤ مکمل;

          اگر (stdout_is_pipe)
            output_started = true;
          ورنہ
            {
              idx_t بقایا = n;
              جب (بقایا > 0)
                {
                  ssize_t s = splice (pipefd[0], تہی, STDOUT_FILENO, تہی,
                                      بقایا, SPLICE_F_MOVE);
                  اگر (s <= 0)
                    جاؤ مکمل;
                  output_started = true;
                  بقایا -= s;
                }
            }

          iov.iov_بنیاد = (حرف *) iov.iov_بنیاد + n;
          iov.iov_len -= n;
        }
    }

مکمل:
  اگر (pipefd[0] >= 0)
    {
      صحیح saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
    }
  alignfree (splice_buf);
#اگر_ختم
  واپس output_started;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخہ, true, usage, AUTHORS,
                                   (حرف ثابت *) تہی);

  حرف **operands = argv + optind;
  حرف **عملیہ_lim = argv + argc;
  اگر (optind == argc)
    *عملیہ_lim++ = bad_cast ("y");

  /* Buffer data locally once, rather than having the
     large overhead of stdio buffering each item.  */
  size_t bufalloc = 0;
  bool reuse_عملیہ_لڑیاں = true;
  حرف **operandp = operands;
  کرو
    {
      size_t عملیہ_len = لڑی_لمبائی (*operandp);
      bufalloc += عملیہ_len + 1;
      اگر (operandp + 1 < عملیہ_lim
          && *operandp + عملیہ_len + 1 != operandp[1])
        reuse_عملیہ_لڑیاں = false;
    }
  جب (++operandp < عملیہ_lim);

  /* Improve performance by using a buffer size greater than BUFSIZ / 2.  */
  اگر (bufalloc <= BUFSIZ / 2)
    {
      bufalloc = BUFSIZ;
      reuse_عملیہ_لڑیاں = false;
    }

#اگر defined __CHERI__
  /* Cheri capability bounds do not allow for this.  */
  reuse_عملیہ_لڑیاں = false;
#اگر_ختم

  /* Fill the buffer with one copy of the output.  If possible, reuse
     the operands strings; this wins when the buffer would be large.  */
  حرف *buf = reuse_عملیہ_لڑیاں ? *operands : xmalloc (bufalloc);
  size_t bufused = 0;
  operandp = operands;
  کرو
    {
      size_t عملیہ_len = لڑی_لمبائی (*operandp);
      اگر (! reuse_عملیہ_لڑیاں)
        حافظہ_نقل (buf + bufused, *operandp, عملیہ_len);
      bufused += عملیہ_len;
      buf[bufused++] = ' ';
    }
  جب (++operandp < عملیہ_lim);
  buf[bufused - 1] = '\n';

  idx_t copysize = bufused;

  /* Repeatedly output the buffer until there is a write error; then fail.
     Do a minimal write first to check output with minimal set up cost.
     If successful then set up for efficient repetition.  */
  اگر (بھرا_write (STDOUT_FILENO, buf, copysize) == copysize
      && splice_write (buf, copysize) == 0)
    {
      /* If a larger buffer was allocated, fill it by repeated copies.  */
      bufused = bufalloc / copysize * copysize;
      اگر (bufused > copysize)
        repeat_نمونہ (buf, buf, copysize, bufused);
      جب (بھرا_write (STDOUT_FILENO, buf, bufused) == bufused)
        جاری;
    }

  error (0, errno, _("standard output"));
  main_exit (EXIT_FAILURE);
}
