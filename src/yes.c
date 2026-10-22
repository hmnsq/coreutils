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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "yes"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [STRING]...\n\
  or:  %s OPTION\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);

      fputs (_("\
Repeatedly output a ಸಾಲು with all specified STRING(s), or 'y'.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Fill DEST[0..BUFSIZE-1] with repeated copies of SRC[0..SRCSIZE-1],
   doubling the copy size each iteration.  DEST may equal SRC.  */

ಸ್ಥಿರ ಶೂನ್ಯ
repeat_ನಮೂನೆ (ಅಕ್ಷರ *dest, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src, idx_t srcsize, idx_t bufsize)
{
  ಆದರೆ (dest != src)
    ಸ್ಮರಣೆ_ನಕಲು (dest, src, srcsize);
  ಗಾಗಿ (idx_t filled = srcsize; filled < bufsize; )
    {
      idx_t chunk = MIN (filled, bufsize - filled);
      ಸ್ಮರಣೆ_ನಕಲು (dest + filled, dest, chunk);
      filled += chunk;
    }
}

#ಆದರೆ HAVE_SPLICE

/* Enlarge a pipe towards SPLICE_PIPE_SIZE and return the actual
   capacity as a quarter of the pipe size (the empirical sweet spot
   for vmsplice throughput), rounded down to a multiple of COPYSIZE.
   Return 0 if the result would be smaller than COPYSIZE.  */

ಸ್ಥಿರ idx_t
pipe_splice_ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ fd, idx_t copysize)
{
  size_t buf_cap = increase_pipe_ಗಾತ್ರ (fd) / 4;
  ಹಿಂತಿರುಗಿ buf_cap / copysize * copysize;
}

#ಆದರೆ_ಅಂತ್ಯ

/* Repeatedly write the COPYSIZE-byte pattern in BUF to standard output
   using vmsplice/splice zero-copy I/O.  Since the data never varies,
   SPLICE_F_GIFT tells the kernel the pages will not be modified.

   Return TRUE if splice I/O was used (caller should check errno and
   report any error).  Return FALSE if splice could not be used.  */

ಸ್ಥಿರ bool
splice_write (MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *buf, MAYBE_ಬಳಸದ idx_t copysize)
{
  bool output_started = false;
#ಆದರೆ HAVE_SPLICE
  idx_t ಪುಟ_ಗಾತ್ರ = getpagesize ();

  bool stdout_is_pipe = isapipe (STDOUT_FILENO) > 0;

  /* Determine buffer size: enlarge the target pipe,
     then use 1/4 of actual capacity as the transfer size.  */
  ಪೂರ್ಣಾಂಕ pipefd[2] = { -1, -1 };
  idx_t splice_bufsize;
  ಅಕ್ಷರ *splice_buf = ಖಾಲಿ;

  ಆದರೆ (stdout_is_pipe)
    splice_bufsize = pipe_splice_ಗಾತ್ರ (STDOUT_FILENO, copysize);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (pipe2 (pipefd, 0) < 0)
        ಹಿಂತಿರುಗಿ false;
      splice_bufsize = pipe_splice_ಗಾತ್ರ (pipefd[0], copysize);
    }

  ಆದರೆ (splice_bufsize == 0)
    ಹೋಗು ಮುಗಿದ;

  /* Allocate page-aligned buffer for vmsplice.
     Needed with SPLICE_F_GIFT, but generally good for performance.  */
  ಆದರೆ (! (splice_buf = alignalloc (ಪುಟ_ಗಾತ್ರ, splice_bufsize)))
    ಹೋಗು ಮುಗಿದ;

  repeat_ನಮೂನೆ (splice_buf, buf, copysize, splice_bufsize);

  /* For the pipe case, vmsplice directly to stdout.
     For the non-pipe case, vmsplice into the intermediate pipe
     and then splice from it to stdout.  */
  ಪೂರ್ಣಾಂಕ vmsplice_fd = stdout_is_pipe ? STDOUT_FILENO : pipefd[1];

  ಗಾಗಿ (;;)
    {
      ರಚನೆ iovec iov = { .iov_ತಳ = splice_buf,
                           .iov_len = splice_bufsize };

      ಆಗ (iov.iov_len > 0)
        {
          /* Use SPLICE_F_{GIFT,MOVE} to allow the kernel to take references
             to the pages.  I.e., we're indicating we won't make changes.
             SPLICE_F_GIFT is only appropriate for full pages.  */
          ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = iov.iov_len % ಪುಟ_ಗಾತ್ರ ? 0 : SPLICE_F_GIFT;
          ssize_t n = vmsplice (vmsplice_fd, &iov, 1, flags);
          ಆದರೆ (n <= 0)
            ಹೋಗು ಮುಗಿದ;

          ಆದರೆ (stdout_is_pipe)
            output_started = true;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              idx_t ಉಳಿದ = n;
              ಆಗ (ಉಳಿದ > 0)
                {
                  ssize_t s = splice (pipefd[0], ಖಾಲಿ, STDOUT_FILENO, ಖಾಲಿ,
                                      ಉಳಿದ, SPLICE_F_MOVE);
                  ಆದರೆ (s <= 0)
                    ಹೋಗು ಮುಗಿದ;
                  output_started = true;
                  ಉಳಿದ -= s;
                }
            }

          iov.iov_ತಳ = (ಅಕ್ಷರ *) iov.iov_ತಳ + n;
          iov.iov_len -= n;
        }
    }

ಮುಗಿದ:
  ಆದರೆ (pipefd[0] >= 0)
    {
      ಪೂರ್ಣಾಂಕ saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
    }
  alignfree (splice_buf);
#ಆದರೆ_ಅಂತ್ಯ
  ಹಿಂತಿರುಗಿ output_started;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE_NAME,
                                   ಆವೃತ್ತಿ, true, usage, AUTHORS,
                                   (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) ಖಾಲಿ);

  ಅಕ್ಷರ **operands = argv + optind;
  ಅಕ್ಷರ **ಕಾರ್ಯಾಂಶ_lim = argv + argc;
  ಆದರೆ (optind == argc)
    *ಕಾರ್ಯಾಂಶ_lim++ = bad_cast ("y");

  /* Buffer data locally once, rather than having the
     large overhead of stdio buffering each item.  */
  size_t bufalloc = 0;
  bool reuse_ಕಾರ್ಯಾಂಶ_ಅಕ್ಷರಸರಣಿಗಳು = true;
  ಅಕ್ಷರ **operandp = operands;
  ಮಾಡು
    {
      size_t ಕಾರ್ಯಾಂಶ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (*operandp);
      bufalloc += ಕಾರ್ಯಾಂಶ_len + 1;
      ಆದರೆ (operandp + 1 < ಕಾರ್ಯಾಂಶ_lim
          && *operandp + ಕಾರ್ಯಾಂಶ_len + 1 != operandp[1])
        reuse_ಕಾರ್ಯಾಂಶ_ಅಕ್ಷರಸರಣಿಗಳು = false;
    }
  ಆಗ (++operandp < ಕಾರ್ಯಾಂಶ_lim);

  /* Improve performance by using a buffer size greater than BUFSIZ / 2.  */
  ಆದರೆ (bufalloc <= BUFSIZ / 2)
    {
      bufalloc = BUFSIZ;
      reuse_ಕಾರ್ಯಾಂಶ_ಅಕ್ಷರಸರಣಿಗಳು = false;
    }

#ಆದರೆ defined __CHERI__
  /* Cheri capability bounds do not allow for this.  */
  reuse_ಕಾರ್ಯಾಂಶ_ಅಕ್ಷರಸರಣಿಗಳು = false;
#ಆದರೆ_ಅಂತ್ಯ

  /* Fill the buffer with one copy of the output.  If possible, reuse
     the operands strings; this wins when the buffer would be large.  */
  ಅಕ್ಷರ *buf = reuse_ಕಾರ್ಯಾಂಶ_ಅಕ್ಷರಸರಣಿಗಳು ? *operands : xmalloc (bufalloc);
  size_t bufused = 0;
  operandp = operands;
  ಮಾಡು
    {
      size_t ಕಾರ್ಯಾಂಶ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (*operandp);
      ಆದರೆ (! reuse_ಕಾರ್ಯಾಂಶ_ಅಕ್ಷರಸರಣಿಗಳು)
        ಸ್ಮರಣೆ_ನಕಲು (buf + bufused, *operandp, ಕಾರ್ಯಾಂಶ_len);
      bufused += ಕಾರ್ಯಾಂಶ_len;
      buf[bufused++] = ' ';
    }
  ಆಗ (++operandp < ಕಾರ್ಯಾಂಶ_lim);
  buf[bufused - 1] = '\n';

  idx_t copysize = bufused;

  /* Repeatedly output the buffer until there is a write error; then fail.
     Do a minimal write first to check output with minimal set up cost.
     If successful then set up for efficient repetition.  */
  ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, buf, copysize) == copysize
      && splice_write (buf, copysize) == 0)
    {
      /* If a larger buffer was allocated, fill it by repeated copies.  */
      bufused = bufalloc / copysize * copysize;
      ಆದರೆ (bufused > copysize)
        repeat_ನಮೂನೆ (buf, buf, copysize, bufused);
      ಆಗ (ತುಂಬಿದ_write (STDOUT_FILENO, buf, bufused) == bufused)
        ಮುಂದುವರಿ;
    }

  error (0, errno, _("standard output"));
  main_exit (EXIT_FAILURE);
}
