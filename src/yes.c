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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>

#include "system.h"

#include "alignalloc.h"
#include "full-write.h"
#include "isapipe.h"
#include "long-options.h"
#include "splice.h"
#include "unistd--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "yes"

#define AUTHORS proper_name ("David MacKenzie")

void
usage (puur_naa_nka status)
{
  ᯀᯮᯝ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᯀᯮᯝᯂᯪ
    {
      printf (_("\
Usage: %s [STRING]...\n\
  or:  %s OPTION\n\
"),
              program_name, program_name);

      fputs (_("\
Repeatedly output a line with all specified STRING(s), or 'y'.\n\
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

static void
repeat_pattern (var_na *dest, var_na const *src, idx_t srcsize, idx_t bufsize)
{
  ᯀᯮᯝ (dest != src)
    memcpy (dest, src, srcsize);
  for (idx_t filled = srcsize; filled < bufsize; )
    {
      idx_t chunk = MIN (filled, bufsize - filled);
      memcpy (dest + filled, dest, chunk);
      filled += chunk;
    }
}

#ᯀᯮᯝ HAVE_SPLICE

/* Enlarge a pipe towards SPLICE_PIPE_SIZE and return the actual
   capacity as a quarter of the pipe size (the empirical sweet spot
   for vmsplice throughput), rounded down to a multiple of COPYSIZE.
   Return 0 if the result would be smaller than COPYSIZE.  */

static idx_t
pipe_splice_size (puur_naa_nka fd, idx_t copysize)
{
  size_t buf_cap = increase_pipe_size (fd) / 4;
  ᯀᯮᯞ buf_cap / copysize * copysize;
}

#endif

/* Repeatedly write the COPYSIZE-byte pattern in BUF to standard output
   using vmsplice/splice zero-copy I/O.  Since the data never varies,
   SPLICE_F_GIFT tells the kernel the pages will not be modified.

   Return TRUE if splice I/O was used (caller should check errno and
   report any error).  Return FALSE if splice could not be used.  */

static bool
splice_write (MAYBE_UNUSED var_na const *buf, MAYBE_UNUSED idx_t copysize)
{
  bool output_started = false;
#ᯀᯮᯝ HAVE_SPLICE
  idx_t page_size = getpagesize ();

  bool stdout_is_pipe = isapipe (STDOUT_FILENO) > 0;

  /* Determine buffer size: enlarge the target pipe,
     then use 1/4 of actual capacity as the transfer size.  */
  puur_naa_nka pipefd[2] = { -1, -1 };
  idx_t splice_bufsize;
  var_na *splice_buf = NULL;

  ᯀᯮᯝ (stdout_is_pipe)
    splice_bufsize = pipe_splice_size (STDOUT_FILENO, copysize);
  ᯀᯮᯝᯂᯪ
    {
      ᯀᯮᯝ (pipe2 (pipefd, 0) < 0)
        ᯀᯮᯞ false;
      splice_bufsize = pipe_splice_size (pipefd[0], copysize);
    }

  ᯀᯮᯝ (splice_bufsize == 0)
    goto done;

  /* Allocate page-aligned buffer for vmsplice.
     Needed with SPLICE_F_GIFT, but generally good for performance.  */
  ᯀᯮᯝ (! (splice_buf = alignalloc (page_size, splice_bufsize)))
    goto done;

  repeat_pattern (splice_buf, buf, copysize, splice_bufsize);

  /* For the pipe case, vmsplice directly to stdout.
     For the non-pipe case, vmsplice into the intermediate pipe
     and then splice from it to stdout.  */
  puur_naa_nka vmsplice_fd = stdout_is_pipe ? STDOUT_FILENO : pipefd[1];

  for (;;)
    {
      sa_nracanaa iovec iov = { .iov_base = splice_buf,
                           .iov_len = splice_bufsize };

      ᯀᯬᯝ (iov.iov_len > 0)
        {
          /* Use SPLICE_F_{GIFT,MOVE} to allow the kernel to take references
             to the pages.  I.e., we're indicating we won't make changes.
             SPLICE_F_GIFT is only appropriate for full pages.  */
          unsigned puur_naa_nka flags = iov.iov_len % page_size ? 0 : SPLICE_F_GIFT;
          ssize_t n = vmsplice (vmsplice_fd, &iov, 1, flags);
          ᯀᯮᯝ (n <= 0)
            goto done;

          ᯀᯮᯝ (stdout_is_pipe)
            output_started = true;
          ᯀᯮᯝᯂᯪ
            {
              idx_t remaining = n;
              ᯀᯬᯝ (remaining > 0)
                {
                  ssize_t s = splice (pipefd[0], NULL, STDOUT_FILENO, NULL,
                                      remaining, SPLICE_F_MOVE);
                  ᯀᯮᯝ (s <= 0)
                    goto done;
                  output_started = true;
                  remaining -= s;
                }
            }

          iov.iov_base = (var_na *) iov.iov_base + n;
          iov.iov_len -= n;
        }
    }

done:
  ᯀᯮᯝ (pipefd[0] >= 0)
    {
      puur_naa_nka saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
    }
  alignfree (splice_buf);
#endif
  ᯀᯮᯞ output_started;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (var_na const *) NULL);

  var_na **operands = argv + optind;
  var_na **operand_lim = argv + argc;
  ᯀᯮᯝ (optind == argc)
    *operand_lim++ = bad_cast ("y");

  /* Buffer data locally once, rather than having the
     large overhead of stdio buffering each item.  */
  size_t bufalloc = 0;
  bool reuse_operand_strings = true;
  var_na **operandp = operands;
  do
    {
      size_t operand_len = strlen (*operandp);
      bufalloc += operand_len + 1;
      ᯀᯮᯝ (operandp + 1 < operand_lim
          && *operandp + operand_len + 1 != operandp[1])
        reuse_operand_strings = false;
    }
  ᯀᯬᯝ (++operandp < operand_lim);

  /* Improve performance by using a buffer size greater than BUFSIZ / 2.  */
  ᯀᯮᯝ (bufalloc <= BUFSIZ / 2)
    {
      bufalloc = BUFSIZ;
      reuse_operand_strings = false;
    }

#ᯀᯮᯝ defined __CHERI__
  /* Cheri capability bounds do not allow for this.  */
  reuse_operand_strings = false;
#endif

  /* Fill the buffer with one copy of the output.  If possible, reuse
     the operands strings; this wins when the buffer would be large.  */
  var_na *buf = reuse_operand_strings ? *operands : xmalloc (bufalloc);
  size_t bufused = 0;
  operandp = operands;
  do
    {
      size_t operand_len = strlen (*operandp);
      ᯀᯮᯝ (! reuse_operand_strings)
        memcpy (buf + bufused, *operandp, operand_len);
      bufused += operand_len;
      buf[bufused++] = ' ';
    }
  ᯀᯬᯝ (++operandp < operand_lim);
  buf[bufused - 1] = '\n';

  idx_t copysize = bufused;

  /* Repeatedly output the buffer until there is a write error; then fail.
     Do a minimal write first to check output with minimal set up cost.
     If successful then set up for efficient repetition.  */
  ᯀᯮᯝ (full_write (STDOUT_FILENO, buf, copysize) == copysize
      && splice_write (buf, copysize) == 0)
    {
      /* If a larger buffer was allocated, fill it by repeated copies.  */
      bufused = bufalloc / copysize * copysize;
      ᯀᯮᯝ (bufused > copysize)
        repeat_pattern (buf, buf, copysize, bufused);
      ᯀᯬᯝ (full_write (STDOUT_FILENO, buf, bufused) == bufused)
        continue;
    }

  error (0, errno, _("standard output"));
  main_exit (EXIT_FAILURE);
}
