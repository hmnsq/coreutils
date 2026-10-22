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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/uio.h>

#அந்தர்பாவ "system.h"

#அந்தர்பாவ "alignalloc.h"
#அந்தர்பாவ "full-write.h"
#அந்தர்பாவ "isapipe.h"
#அந்தர்பாவ "long-options.h"
#அந்தர்பாவ "splice.h"
#அந்தர்பாவ "unistd--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "yes"

#define AUTHORS proper_name ("David MacKenzie")

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [STRING]...\n\
  or:  %s OPTION\n\
"),
              program_name, program_name);

      fputs (_("\
Repeatedly output a வரி with all specified STRING(s), or 'y'.\n\
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

நிலையான வெறுமை
repeat_pattern (எழுத்து *dest, எழுத்து மாறிலி *src, idx_t srcsize, idx_t bufsize)
{
  எனில் (dest != src)
    memcpy (dest, src, srcsize);
  க்கு (idx_t filled = srcsize; filled < bufsize; )
    {
      idx_t chunk = MIN (filled, bufsize - filled);
      memcpy (dest + filled, dest, chunk);
      filled += chunk;
    }
}

#எனில் HAVE_SPLICE

/* Enlarge a pipe towards SPLICE_PIPE_SIZE and return the actual
   capacity as a quarter of the pipe size (the empirical sweet spot
   for vmsplice throughput), rounded down to a multiple of COPYSIZE.
   Return 0 if the result would be smaller than COPYSIZE.  */

நிலையான idx_t
pipe_splice_size (முழு fd, idx_t copysize)
{
  size_t buf_cap = increase_pipe_size (fd) / 4;
  திரும்பு buf_cap / copysize * copysize;
}

#endif

/* Repeatedly write the COPYSIZE-byte pattern in BUF to standard output
   using vmsplice/splice zero-copy I/O.  Since the data never varies,
   SPLICE_F_GIFT tells the kernel the pages will not be modified.

   Return TRUE if splice I/O was used (caller should check errno and
   report any error).  Return FALSE if splice could not be used.  */

நிலையான bool
splice_write (MAYBE_UNUSED எழுத்து மாறிலி *buf, MAYBE_UNUSED idx_t copysize)
{
  bool output_started = false;
#எனில் HAVE_SPLICE
  idx_t page_size = getpagesize ();

  bool stdout_is_pipe = isapipe (STDOUT_FILENO) > 0;

  /* Determine buffer size: enlarge the target pipe,
     then use 1/4 of actual capacity as the transfer size.  */
  முழு pipefd[2] = { -1, -1 };
  idx_t splice_bufsize;
  எழுத்து *splice_buf = வெற்று;

  எனில் (stdout_is_pipe)
    splice_bufsize = pipe_splice_size (STDOUT_FILENO, copysize);
  இல்லையெனில்
    {
      எனில் (pipe2 (pipefd, 0) < 0)
        திரும்பு false;
      splice_bufsize = pipe_splice_size (pipefd[0], copysize);
    }

  எனில் (splice_bufsize == 0)
    செல் done;

  /* Allocate page-aligned buffer for vmsplice.
     Needed with SPLICE_F_GIFT, but generally good for performance.  */
  எனில் (! (splice_buf = alignalloc (page_size, splice_bufsize)))
    செல் done;

  repeat_pattern (splice_buf, buf, copysize, splice_bufsize);

  /* For the pipe case, vmsplice directly to stdout.
     For the non-pipe case, vmsplice into the intermediate pipe
     and then splice from it to stdout.  */
  முழு vmsplice_fd = stdout_is_pipe ? STDOUT_FILENO : pipefd[1];

  க்கு (;;)
    {
      கட்டமைப்பு iovec iov = { .iov_base = splice_buf,
                           .iov_len = splice_bufsize };

      போது (iov.iov_len > 0)
        {
          /* Use SPLICE_F_{GIFT,MOVE} to allow the kernel to take references
             to the pages.  I.e., we're indicating we won't make changes.
             SPLICE_F_GIFT is only appropriate for full pages.  */
          unsigned முழு flags = iov.iov_len % page_size ? 0 : SPLICE_F_GIFT;
          ssize_t n = vmsplice (vmsplice_fd, &iov, 1, flags);
          எனில் (n <= 0)
            செல் done;

          எனில் (stdout_is_pipe)
            output_started = true;
          இல்லையெனில்
            {
              idx_t remaining = n;
              போது (remaining > 0)
                {
                  ssize_t s = splice (pipefd[0], வெற்று, STDOUT_FILENO, வெற்று,
                                      remaining, SPLICE_F_MOVE);
                  எனில் (s <= 0)
                    செல் done;
                  output_started = true;
                  remaining -= s;
                }
            }

          iov.iov_base = (எழுத்து *) iov.iov_base + n;
          iov.iov_len -= n;
        }
    }

done:
  எனில் (pipefd[0] >= 0)
    {
      முழு saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
    }
  alignfree (splice_buf);
#endif
  திரும்பு output_started;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (எழுத்து மாறிலி *) வெற்று);

  எழுத்து **operands = argv + optind;
  எழுத்து **operand_lim = argv + argc;
  எனில் (optind == argc)
    *operand_lim++ = bad_cast ("y");

  /* Buffer data locally once, rather than having the
     large overhead of stdio buffering each item.  */
  size_t bufalloc = 0;
  bool reuse_operand_strings = true;
  எழுத்து **operandp = operands;
  செய்
    {
      size_t operand_len = strlen (*operandp);
      bufalloc += operand_len + 1;
      எனில் (operandp + 1 < operand_lim
          && *operandp + operand_len + 1 != operandp[1])
        reuse_operand_strings = false;
    }
  போது (++operandp < operand_lim);

  /* Improve performance by using a buffer size greater than BUFSIZ / 2.  */
  எனில் (bufalloc <= BUFSIZ / 2)
    {
      bufalloc = BUFSIZ;
      reuse_operand_strings = false;
    }

#எனில் defined __CHERI__
  /* Cheri capability bounds do not allow for this.  */
  reuse_operand_strings = false;
#endif

  /* Fill the buffer with one copy of the output.  If possible, reuse
     the operands strings; this wins when the buffer would be large.  */
  எழுத்து *buf = reuse_operand_strings ? *operands : xmalloc (bufalloc);
  size_t bufused = 0;
  operandp = operands;
  செய்
    {
      size_t operand_len = strlen (*operandp);
      எனில் (! reuse_operand_strings)
        memcpy (buf + bufused, *operandp, operand_len);
      bufused += operand_len;
      buf[bufused++] = ' ';
    }
  போது (++operandp < operand_lim);
  buf[bufused - 1] = '\n';

  idx_t copysize = bufused;

  /* Repeatedly output the buffer until there is a write error; then fail.
     Do a minimal write first to check output with minimal set up cost.
     If successful then set up for efficient repetition.  */
  எனில் (full_write (STDOUT_FILENO, buf, copysize) == copysize
      && splice_write (buf, copysize) == 0)
    {
      /* If a larger buffer was allocated, fill it by repeated copies.  */
      bufused = bufalloc / copysize * copysize;
      எனில் (bufused > copysize)
        repeat_pattern (buf, buf, copysize, bufused);
      போது (full_write (STDOUT_FILENO, buf, bufused) == bufused)
        தொடர்;
    }

  error (0, errno, _("standard output"));
  main_exit (EXIT_FAILURE);
}
