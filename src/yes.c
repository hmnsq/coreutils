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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/uio.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "alignalloc.h"
#ಅನ್ತರ್ಭಾವ "full-write.h"
#ಅನ್ತರ್ಭಾವ "isapipe.h"
#ಅನ್ತರ್ಭಾವ "long-options.h"
#ಅನ್ತರ್ಭಾವ "splice.h"
#ಅನ್ತರ್ಭಾವ "unistd--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "yes"

#define AUTHORS proper_name ("David MacKenzie")

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [STRING]...\n\
  or:  %s OPTION\n\
"),
              program_name, program_name);

      fputs (_("\
Repeatedly output a ಸಾಲು with all specified STRING(s), or 'y'.\n\
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

ಸ್ಥಿರ ಶೂನ್ಯ
repeat_pattern (ವರ್ಣ *dest, ವರ್ಣ ಧ್ರುವ *src, idx_t srcsize, idx_t bufsize)
{
  ಯದಿ (dest != src)
    memcpy (dest, src, srcsize);
  ಪ್ರತಿ (idx_t filled = srcsize; filled < bufsize; )
    {
      idx_t chunk = MIN (filled, bufsize - filled);
      memcpy (dest + filled, dest, chunk);
      filled += chunk;
    }
}

#ಯದಿ HAVE_SPLICE

/* Enlarge a pipe towards SPLICE_PIPE_SIZE and return the actual
   capacity as a quarter of the pipe size (the empirical sweet spot
   for vmsplice throughput), rounded down to a multiple of COPYSIZE.
   Return 0 if the result would be smaller than COPYSIZE.  */

ಸ್ಥಿರ idx_t
pipe_splice_size (ಪೂರ್ಣಾಙ್ಕ fd, idx_t copysize)
{
  size_t buf_cap = increase_pipe_size (fd) / 4;
  ಪ್ರತ್ಯಾಗಮ buf_cap / copysize * copysize;
}

#endif

/* Repeatedly write the COPYSIZE-byte pattern in BUF to standard output
   using vmsplice/splice zero-copy I/O.  Since the data never varies,
   SPLICE_F_GIFT tells the kernel the pages will not be modified.

   Return TRUE if splice I/O was used (caller should check errno and
   report any error).  Return FALSE if splice could not be used.  */

ಸ್ಥಿರ bool
splice_write (MAYBE_UNUSED ವರ್ಣ ಧ್ರುವ *buf, MAYBE_UNUSED idx_t copysize)
{
  bool output_started = false;
#ಯದಿ HAVE_SPLICE
  idx_t page_size = getpagesize ();

  bool stdout_is_pipe = isapipe (STDOUT_FILENO) > 0;

  /* Determine buffer size: enlarge the target pipe,
     then use 1/4 of actual capacity as the transfer size.  */
  ಪೂರ್ಣಾಙ್ಕ pipefd[2] = { -1, -1 };
  idx_t splice_bufsize;
  ವರ್ಣ *splice_buf = ರಿಕ್ತ;

  ಯದಿ (stdout_is_pipe)
    splice_bufsize = pipe_splice_size (STDOUT_FILENO, copysize);
  ಅನ್ಯಥಾ
    {
      ಯದಿ (pipe2 (pipefd, 0) < 0)
        ಪ್ರತ್ಯಾಗಮ false;
      splice_bufsize = pipe_splice_size (pipefd[0], copysize);
    }

  ಯದಿ (splice_bufsize == 0)
    ಗಚ್ಛ done;

  /* Allocate page-aligned buffer for vmsplice.
     Needed with SPLICE_F_GIFT, but generally good for performance.  */
  ಯದಿ (! (splice_buf = alignalloc (page_size, splice_bufsize)))
    ಗಚ್ಛ done;

  repeat_pattern (splice_buf, buf, copysize, splice_bufsize);

  /* For the pipe case, vmsplice directly to stdout.
     For the non-pipe case, vmsplice into the intermediate pipe
     and then splice from it to stdout.  */
  ಪೂರ್ಣಾಙ್ಕ vmsplice_fd = stdout_is_pipe ? STDOUT_FILENO : pipefd[1];

  ಪ್ರತಿ (;;)
    {
      ಸಂರಚನಾ iovec iov = { .iov_base = splice_buf,
                           .iov_len = splice_bufsize };

      ಯಾವತ್ (iov.iov_len > 0)
        {
          /* Use SPLICE_F_{GIFT,MOVE} to allow the kernel to take references
             to the pages.  I.e., we're indicating we won't make changes.
             SPLICE_F_GIFT is only appropriate for full pages.  */
          unsigned ಪೂರ್ಣಾಙ್ಕ flags = iov.iov_len % page_size ? 0 : SPLICE_F_GIFT;
          ssize_t n = vmsplice (vmsplice_fd, &iov, 1, flags);
          ಯದಿ (n <= 0)
            ಗಚ್ಛ done;

          ಯದಿ (stdout_is_pipe)
            output_started = true;
          ಅನ್ಯಥಾ
            {
              idx_t remaining = n;
              ಯಾವತ್ (remaining > 0)
                {
                  ssize_t s = splice (pipefd[0], ರಿಕ್ತ, STDOUT_FILENO, ರಿಕ್ತ,
                                      remaining, SPLICE_F_MOVE);
                  ಯದಿ (s <= 0)
                    ಗಚ್ಛ done;
                  output_started = true;
                  remaining -= s;
                }
            }

          iov.iov_base = (ವರ್ಣ *) iov.iov_base + n;
          iov.iov_len -= n;
        }
    }

done:
  ಯದಿ (pipefd[0] >= 0)
    {
      ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
    }
  alignfree (splice_buf);
#endif
  ಪ್ರತ್ಯಾಗಮ output_started;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (ವರ್ಣ ಧ್ರುವ *) ರಿಕ್ತ);

  ವರ್ಣ **operands = argv + optind;
  ವರ್ಣ **operand_lim = argv + argc;
  ಯದಿ (optind == argc)
    *operand_lim++ = bad_cast ("y");

  /* Buffer data locally once, rather than having the
     large overhead of stdio buffering each item.  */
  size_t bufalloc = 0;
  bool reuse_operand_strings = true;
  ವರ್ಣ **operandp = operands;
  ಕುರು
    {
      size_t operand_len = strlen (*operandp);
      bufalloc += operand_len + 1;
      ಯದಿ (operandp + 1 < operand_lim
          && *operandp + operand_len + 1 != operandp[1])
        reuse_operand_strings = false;
    }
  ಯಾವತ್ (++operandp < operand_lim);

  /* Improve performance by using a buffer size greater than BUFSIZ / 2.  */
  ಯದಿ (bufalloc <= BUFSIZ / 2)
    {
      bufalloc = BUFSIZ;
      reuse_operand_strings = false;
    }

#ಯದಿ defined __CHERI__
  /* Cheri capability bounds do not allow for this.  */
  reuse_operand_strings = false;
#endif

  /* Fill the buffer with one copy of the output.  If possible, reuse
     the operands strings; this wins when the buffer would be large.  */
  ವರ್ಣ *buf = reuse_operand_strings ? *operands : xmalloc (bufalloc);
  size_t bufused = 0;
  operandp = operands;
  ಕುರು
    {
      size_t operand_len = strlen (*operandp);
      ಯದಿ (! reuse_operand_strings)
        memcpy (buf + bufused, *operandp, operand_len);
      bufused += operand_len;
      buf[bufused++] = ' ';
    }
  ಯಾವತ್ (++operandp < operand_lim);
  buf[bufused - 1] = '\n';

  idx_t copysize = bufused;

  /* Repeatedly output the buffer until there is a write error; then fail.
     Do a minimal write first to check output with minimal set up cost.
     If successful then set up for efficient repetition.  */
  ಯದಿ (full_write (STDOUT_FILENO, buf, copysize) == copysize
      && splice_write (buf, copysize) == 0)
    {
      /* If a larger buffer was allocated, fill it by repeated copies.  */
      bufused = bufalloc / copysize * copysize;
      ಯದಿ (bufused > copysize)
        repeat_pattern (buf, buf, copysize, bufused);
      ಯಾವತ್ (full_write (STDOUT_FILENO, buf, bufused) == bufused)
        ಅನುವರ್ತ;
    }

  error (0, errno, _("standard output"));
  main_exit (EXIT_FAILURE);
}
