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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/uio.h>

#అన్తర్భావ "system.h"

#అన్తర్భావ "alignalloc.h"
#అన్తర్భావ "full-write.h"
#అన్తర్భావ "isapipe.h"
#అన్తర్భావ "long-options.h"
#అన్తర్భావ "splice.h"
#అన్తర్భావ "unistd--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "yes"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [STRING]...\n\
  or:  %s OPTION\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);

      fputs (_("\
Repeatedly output a పంక్తి with all specified STRING(s), or 'y'.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Fill DEST[0..BUFSIZE-1] with repeated copies of SRC[0..SRCSIZE-1],
   doubling the copy size each iteration.  DEST may equal SRC.  */

స్థిర శూన్య
repeat_నమూనా (అక్షరం *dest, అక్షరం స్థిరరాశి *src, idx_t srcsize, idx_t bufsize)
{
  ఒకవేళ (dest != src)
    జ్ఞాపకం_నకలు (dest, src, srcsize);
  కొరకు (idx_t filled = srcsize; filled < bufsize; )
    {
      idx_t chunk = MIN (filled, bufsize - filled);
      జ్ఞాపకం_నకలు (dest + filled, dest, chunk);
      filled += chunk;
    }
}

#ఒకవేళ HAVE_SPLICE

/* Enlarge a pipe towards SPLICE_PIPE_SIZE and return the actual
   capacity as a quarter of the pipe size (the empirical sweet spot
   for vmsplice throughput), rounded down to a multiple of COPYSIZE.
   Return 0 if the result would be smaller than COPYSIZE.  */

స్థిర idx_t
pipe_splice_కొలత (పూర్ణాంకం fd, idx_t copysize)
{
  size_t buf_cap = increase_pipe_కొలత (fd) / 4;
  తిరిగి buf_cap / copysize * copysize;
}

#అయితే_ముగింపు

/* Repeatedly write the COPYSIZE-byte pattern in BUF to standard output
   using vmsplice/splice zero-copy I/O.  Since the data never varies,
   SPLICE_F_GIFT tells the kernel the pages will not be modified.

   Return TRUE if splice I/O was used (caller should check errno and
   report any error).  Return FALSE if splice could not be used.  */

స్థిర bool
splice_write (MAYBE_వాడని అక్షరం స్థిరరాశి *buf, MAYBE_వాడని idx_t copysize)
{
  bool output_started = false;
#ఒకవేళ HAVE_SPLICE
  idx_t పుట_కొలత = getpagesize ();

  bool stdout_is_pipe = isapipe (STDOUT_FILENO) > 0;

  /* Determine buffer size: enlarge the target pipe,
     then use 1/4 of actual capacity as the transfer size.  */
  పూర్ణాంకం pipefd[2] = { -1, -1 };
  idx_t splice_bufsize;
  అక్షరం *splice_buf = ఖాళీ;

  ఒకవేళ (stdout_is_pipe)
    splice_bufsize = pipe_splice_కొలత (STDOUT_FILENO, copysize);
  లేకపోతే
    {
      ఒకవేళ (pipe2 (pipefd, 0) < 0)
        తిరిగి false;
      splice_bufsize = pipe_splice_కొలత (pipefd[0], copysize);
    }

  ఒకవేళ (splice_bufsize == 0)
    వెళ్ళు పూర్తి;

  /* Allocate page-aligned buffer for vmsplice.
     Needed with SPLICE_F_GIFT, but generally good for performance.  */
  ఒకవేళ (! (splice_buf = alignalloc (పుట_కొలత, splice_bufsize)))
    వెళ్ళు పూర్తి;

  repeat_నమూనా (splice_buf, buf, copysize, splice_bufsize);

  /* For the pipe case, vmsplice directly to stdout.
     For the non-pipe case, vmsplice into the intermediate pipe
     and then splice from it to stdout.  */
  పూర్ణాంకం vmsplice_fd = stdout_is_pipe ? STDOUT_FILENO : pipefd[1];

  కొరకు (;;)
    {
      నిర్మాణం iovec iov = { .iov_పునాది = splice_buf,
                           .iov_len = splice_bufsize };

      అయితే (iov.iov_len > 0)
        {
          /* Use SPLICE_F_{GIFT,MOVE} to allow the kernel to take references
             to the pages.  I.e., we're indicating we won't make changes.
             SPLICE_F_GIFT is only appropriate for full pages.  */
          గుర్తులేని పూర్ణాంకం flags = iov.iov_len % పుట_కొలత ? 0 : SPLICE_F_GIFT;
          ssize_t n = vmsplice (vmsplice_fd, &iov, 1, flags);
          ఒకవేళ (n <= 0)
            వెళ్ళు పూర్తి;

          ఒకవేళ (stdout_is_pipe)
            output_started = true;
          లేకపోతే
            {
              idx_t మిగిలిన = n;
              అయితే (మిగిలిన > 0)
                {
                  ssize_t s = splice (pipefd[0], ఖాళీ, STDOUT_FILENO, ఖాళీ,
                                      మిగిలిన, SPLICE_F_MOVE);
                  ఒకవేళ (s <= 0)
                    వెళ్ళు పూర్తి;
                  output_started = true;
                  మిగిలిన -= s;
                }
            }

          iov.iov_పునాది = (అక్షరం *) iov.iov_పునాది + n;
          iov.iov_len -= n;
        }
    }

పూర్తి:
  ఒకవేళ (pipefd[0] >= 0)
    {
      పూర్ణాంకం saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
    }
  alignfree (splice_buf);
#అయితే_ముగింపు
  తిరిగి output_started;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, కార్యక్రమం_NAME, PACKAGE_NAME,
                                   సంస్కరణ, true, usage, AUTHORS,
                                   (అక్షరం స్థిరరాశి *) ఖాళీ);

  అక్షరం **operands = argv + optind;
  అక్షరం **నిర్వాహ్యం_lim = argv + argc;
  ఒకవేళ (optind == argc)
    *నిర్వాహ్యం_lim++ = bad_cast ("y");

  /* Buffer data locally once, rather than having the
     large overhead of stdio buffering each item.  */
  size_t bufalloc = 0;
  bool reuse_నిర్వాహ్యం_అక్షరశ్రేణులు = true;
  అక్షరం **operandp = operands;
  చేయి
    {
      size_t నిర్వాహ్యం_len = అక్షరశ్రేణి_పొడవు (*operandp);
      bufalloc += నిర్వాహ్యం_len + 1;
      ఒకవేళ (operandp + 1 < నిర్వాహ్యం_lim
          && *operandp + నిర్వాహ్యం_len + 1 != operandp[1])
        reuse_నిర్వాహ్యం_అక్షరశ్రేణులు = false;
    }
  అయితే (++operandp < నిర్వాహ్యం_lim);

  /* Improve performance by using a buffer size greater than BUFSIZ / 2.  */
  ఒకవేళ (bufalloc <= BUFSIZ / 2)
    {
      bufalloc = BUFSIZ;
      reuse_నిర్వాహ్యం_అక్షరశ్రేణులు = false;
    }

#ఒకవేళ defined __CHERI__
  /* Cheri capability bounds do not allow for this.  */
  reuse_నిర్వాహ్యం_అక్షరశ్రేణులు = false;
#అయితే_ముగింపు

  /* Fill the buffer with one copy of the output.  If possible, reuse
     the operands strings; this wins when the buffer would be large.  */
  అక్షరం *buf = reuse_నిర్వాహ్యం_అక్షరశ్రేణులు ? *operands : xmalloc (bufalloc);
  size_t bufused = 0;
  operandp = operands;
  చేయి
    {
      size_t నిర్వాహ్యం_len = అక్షరశ్రేణి_పొడవు (*operandp);
      ఒకవేళ (! reuse_నిర్వాహ్యం_అక్షరశ్రేణులు)
        జ్ఞాపకం_నకలు (buf + bufused, *operandp, నిర్వాహ్యం_len);
      bufused += నిర్వాహ్యం_len;
      buf[bufused++] = ' ';
    }
  అయితే (++operandp < నిర్వాహ్యం_lim);
  buf[bufused - 1] = '\n';

  idx_t copysize = bufused;

  /* Repeatedly output the buffer until there is a write error; then fail.
     Do a minimal write first to check output with minimal set up cost.
     If successful then set up for efficient repetition.  */
  ఒకవేళ (నిండిన_write (STDOUT_FILENO, buf, copysize) == copysize
      && splice_write (buf, copysize) == 0)
    {
      /* If a larger buffer was allocated, fill it by repeated copies.  */
      bufused = bufalloc / copysize * copysize;
      ఒకవేళ (bufused > copysize)
        repeat_నమూనా (buf, buf, copysize, bufused);
      అయితే (నిండిన_write (STDOUT_FILENO, buf, bufused) == bufused)
        కొనసాగు;
    }

  error (0, errno, _("standard output"));
  main_exit (EXIT_FAILURE);
}
