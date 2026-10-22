/* cat -- concatenate files and print on the standard output.
   Copyright (C) 1988-2026 Free Software Foundation, Inc.

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

/* Differences from the Unix cat:
   * Always unbuffered, -u is ignored.
   * Usually much faster than other versions of cat, the difference
   is especially apparent when using the -v option.

   By tege@sics.se, Torbjörn Granlund, advised by rms, Richard Stallman.  */

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#ఒకవేళ HAVE_STROPTS_H
# include <stropts.h>
#అయితే_ముగింపు
#అన్తర్భావ <sys/ioctl.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "alignalloc.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "ioblksize.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "full-write.h"
#అన్తర్భావ "isapipe.h"
#అన్తర్భావ "splice.h"
#అన్తర్భావ "unistd--.h"
#అన్తర్భావ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "cat"

#నిర్వచించు AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
స్థిర అక్షరం స్థిరరాశి *infile;

/* Descriptor on which input file is open.  */
స్థిర పూర్ణాంకం ఆదానం_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#నిర్వచించు పంక్తి_COUNTER_BUF_LEN 20
స్థిర అక్షరం పంక్తి_buf[పంక్తి_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
స్థిర అక్షరం *పంక్తి_అంకె_print = పంక్తి_buf + పంక్తి_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
స్థిర అక్షరం *పంక్తి_అంకె_ప్రారంభం = పంక్తి_buf + పంక్తి_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
స్థిర అక్షరం *పంక్తి_అంకె_చివర = పంక్తి_buf + పంక్తి_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
స్థిర పూర్ణాంకం newlines2 = 0;

/* Whether there is a pending CR to process.  */
స్థిర bool pending_cr = false;

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Concatenate FILE(s) గమ్యం standard output.\n\
"), stdout);

      emit_stdin_note ();

      oputs (_("\
  -A, --show-all           equivalent గమ్యం -vET\n\
"));
      oputs (_("\
  -b, --సంఖ్య-nonblank    సంఖ్య nonempty output పంక్తులు, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent గమ్యం -vE\n\
"));
      oputs (_("\
  -E, --show-చివర్లు          display $ or ^M$ at చివర యొక్క each పంక్తి\n\
"));
      oputs (_("\
  -n, --సంఖ్య             సంఖ్య all output పంక్తులు\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated ఖాళీలు output పంక్తులు\n\
"));
      oputs (_("\
  -t                       equivalent గమ్యం -vT\n\
"));
      oputs (_("\
  -T, --show-tabs          display TAB characters as ^I\n\
"));
      oputs (_("\
  -u                       (ignored)\n\
"));
      oputs (_("\
  -v, --show-nonprinting   use ^ and M- notation, except కొరకు LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      రాయి (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard ఆదానం గమ్యం standard output.\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Compute the next line number.  */

స్థిర శూన్య
తదుపరి_పంక్తి_అంకె (శూన్య)
{
  అక్షరం *endp = పంక్తి_అంకె_చివర;
  చేయి
    {
      ఒకవేళ ((*endp)++ < '9')
        తిరిగి;
      *endp-- = '0';
    }
  అయితే (endp >= పంక్తి_అంకె_ప్రారంభం);

  ఒకవేళ (పంక్తి_అంకె_ప్రారంభం > పంక్తి_buf)
    *--పంక్తి_అంకె_ప్రారంభం = '1';
  లేకపోతే
    *పంక్తి_buf = '>';
  ఒకవేళ (పంక్తి_అంకె_ప్రారంభం < పంక్తి_అంకె_print)
    పంక్తి_అంకె_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

స్థిర bool
simple_cat (అక్షరం *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  అయితే (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = పఠ (ఆదానం_desc, buf, bufsize);
      ఒకవేళ (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          తిరిగి false;
        }

      /* End of this file?  */

      ఒకవేళ (n_read == 0)
        తిరిగి true;

      /* Write this block out.  */

      ఒకవేళ (నిండిన_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

స్థిర అంతఃవరుస శూన్య
write_pending (అక్షరం *outbuf, అక్షరం **bpout)
{
  idx_t n_write = *bpout - outbuf;
  ఒకవేళ (0 < n_write)
    {
      ఒకవేళ (నిండిన_write (STDOUT_FILENO, outbuf, n_write) != n_write)
        write_error ();
      *bpout = outbuf;
    }
}

/* Copy the file behind 'input_desc' to STDOUT_FILENO.
   Use INBUF and read INSIZE with each call,
   and OUTBUF and write OUTSIZE with each call.
   (The buffers are a bit larger than the I/O sizes.)
   The remaining boolean args say what 'cat' options to use.

   Return true if successful.
   Called if any option more than -u was specified.

   A newline character is always put at the end of the buffer, to make
   an explicit test for buffer end unnecessary.  */

స్థిర bool
cat (అక్షరం *inbuf, idx_t insize, అక్షరం *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool సంఖ్య, bool సంఖ్య_nonblank,
     bool show_చివర్లు, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  గుర్తులేని అక్షరం ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  పూర్ణాంకం newlines = newlines2;

#నిర్వచితమైతే FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#అయితే_ముగింపు

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  అక్షరం *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  అక్షరం *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  అక్షరం *bpout = outbuf;

  అయితే (true)
    {
      చేయి
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          ఒకవేళ (outbuf + outsize <= bpout)
            {
              అక్షరం *wp = outbuf;
              idx_t మిగిలిన_బైట్లు;
              చేయి
                {
                  ఒకవేళ (నిండిన_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  మిగిలిన_బైట్లు = bpout - wp;
                }
              అయితే (outsize <= మిగిలిన_బైట్లు);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, మిగిలిన_బైట్లు);
              bpout = outbuf + మిగిలిన_బైట్లు;
            }

          /* Is INBUF empty?  */

          ఒకవేళ (bpin > eob)
            {
              bool ఆదానం_pending = false;
#నిర్వచితమైతే FIONREAD
              పూర్ణాంకం n_గమ్యం_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              ఒకవేళ (use_fionread
                  && ioctl (ఆదానం_desc, FIONREAD, &n_గమ్యం_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  ఒకవేళ (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  లేకపోతే
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      తిరిగి false;
                    }
                }
              ఒకవేళ (n_గమ్యం_read != 0)
                ఆదానం_pending = true;
#అయితే_ముగింపు

              ఒకవేళ (!ఆదానం_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = పఠ (ఆదానం_desc, inbuf, insize);
              ఒకవేళ (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  తిరిగి false;
                }
              ఒకవేళ (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  తిరిగి true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          లేకపోతే
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              ఒకవేళ (++newlines > 0)
                {
                  ఒకవేళ (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      ఒకవేళ (squeeze_blank)
                        {
                          ch = *bpin++;
                          కొనసాగు;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  ఒకవేళ (సంఖ్య && !సంఖ్య_nonblank)
                    {
                      తదుపరి_పంక్తి_అంకె ();
                      bpout = stpcpy (bpout, పంక్తి_అంకె_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              ఒకవేళ (show_చివర్లు)
                {
                  ఒకవేళ (pending_cr)
                    {
                      *bpout++ = '^';
                      *bpout++ = 'M';
                      pending_cr = false;
                    }
                  *bpout++ = '$';
                }

              /* Output the newline.  */

              *bpout++ = '\n';
            }
          ch = *bpin++;
        }
      అయితే (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      ఒకవేళ (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      ఒకవేళ (newlines >= 0 && సంఖ్య)
        {
          తదుపరి_పంక్తి_అంకె ();
          bpout = stpcpy (bpout, పంక్తి_అంకె_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      ఒకవేళ (show_nonprinting)
        {
          అయితే (true)
            {
              ఒకవేళ (ch >= 32)
                {
                  ఒకవేళ (ch < 127)
                    *bpout++ = ch;
                  లేకపోతే ఒకవేళ (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  లేకపోతే
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      ఒకవేళ (ch >= 128 + 32)
                        {
                          ఒకవేళ (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          లేకపోతే
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      లేకపోతే
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              లేకపోతే ఒకవేళ (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              లేకపోతే ఒకవేళ (ch == '\n')
                {
                  newlines = -1;
                  ఆపు;
                }
              లేకపోతే
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      లేకపోతే
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          అయితే (true)
            {
              ఒకవేళ (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              లేకపోతే ఒకవేళ (ch != '\n')
                {
                  ఒకవేళ (ch == '\r' && *bpin == '\n' && show_చివర్లు)
                    {
                      ఒకవేళ (bpin == eob)
                        pending_cr = true;
                      లేకపోతే
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  లేకపోతే
                    *bpout++ = ch;
                }
              లేకపోతే
                {
                  newlines = -1;
                  ఆపు;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

స్థిర పూర్ణాంకం
copy_cat (శూన్య)
{
  /* Copy at most COPY_MAX bytes at a time; this is min
     (SSIZE_MAX, SIZE_MAX) truncated to a value that is
     surely aligned well.  */
  ssize_t copy_max = MIN (SSIZE_MAX, SIZE_MAX) >> 30 << 30;

  /* copy_file_range does not support some cases, and it
     incorrectly returns 0 when reading from the proc file
     system on the Linux kernel through at least 5.6.19 (2020),
     so fall back on read+write if the copy_file_range is
     unsupported or the input file seems empty.  */

  కొరకు (bool some_copied = false; ; some_copied = true)
    ఎంపిక (copy_file_range (ఆదానం_desc, ఖాళీ, STDOUT_FILENO, ఖాళీ,
                             copy_max, 0))
      {
      స్థితి 0:
        తిరిగి some_copied;

      స్థితి -1:
        ఒకవేళ (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          తిరిగి 0;
        error (0, errno, "%s", quotef (infile));
        తిరిగి -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

స్థిర పూర్ణాంకం
splice_cat (శూన్య)
{
  bool some_copied = false;
  bool లోపల_ok = true;
  bool బహిర్గతం_ok = true;

#ఒకవేళ HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  స్థిర పూర్ణాంకం pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  స్థిర idx_t pipefd_pipe_కొలత = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  ఒకవేళ (pipefd[0] < 0)
    {
      ఒకవేళ (pipe (pipefd) < 0)
        తిరిగి false;
      pipefd_pipe_కొలత = increase_pipe_కొలత (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  స్థిర పూర్ణాంకం stdout_is_pipe = -1;
  స్థిర idx_t stdout_pipe_కొలత = 0;
  ఒకవేళ (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      ఒకవేళ (stdout_is_pipe)
        stdout_pipe_కొలత = increase_pipe_కొలత (STDOUT_FILENO);
    }

  idx_t pipe_కొలత = MAX (pipefd_pipe_కొలత, stdout_pipe_కొలత);

  అయితే (true)
    {
      ssize_t బైట్లు_read = splice (ఆదానం_desc, ఖాళీ, pipefd[1], ఖాళీ,
                                   pipe_కొలత, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      లోపల_ok = 0 <= బైట్లు_read || ! some_copied;
      ఒకవేళ (బైట్లు_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      ఒకవేళ (బైట్లు_read <= 0)
        వెళ్ళు పూర్తి;
      /* We need to drain the intermediate pipe to standard output.  */
      అయితే (0 < బైట్లు_read)
        {
          ssize_t బైట్లు_written = splice (pipefd[0], ఖాళీ, STDOUT_FILENO, ఖాళీ,
                                          pipe_కొలత, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          ఒకవేళ (బైట్లు_written < 0)
            {
              ఒకవేళ (some_copied)
                బహిర్గతం_ok = false;
              లేకపోతే
                {
                  అక్షరం buf[BUFSIZ];
                  అయితే (0 < బైట్లు_read)
                    {
                      ssize_t లెక్క = MIN (బైట్లు_read, పరిమాణం buf);
                      ssize_t n_read = పఠ (pipefd[0], buf, లెక్క);
                      /* Failure not associated with in or out.  */
                      లోపల_ok = బహిర్గతం_ok = 0 <= n_read;
                      ఒకవేళ (n_read <= 0)
                        వెళ్ళు పూర్తి;
                      ఒకవేళ (నిండిన_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      బైట్లు_read -= n_read;
                    }
                }
            }
          ఒకవేళ (బైట్లు_written <= 0)
            వెళ్ళు పూర్తి;
          some_copied = true;
          బైట్లు_read -= బైట్లు_written;
        }
    }

 పూర్తి:
  ఒకవేళ (! లోపల_ok && ! బహిర్గతం_ok)
    {
      /* Recreate the pipe on internal error.  */
      పూర్ణాంకం saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_కొలత = 0;
      error (0, errno, "%s", _("splice error"));
    }
  లేకపోతే ఒకవేళ (! లోపల_ok)
    error (0, errno, "%s", quotef (infile));
  లేకపోతే ఒకవేళ (! బహిర్గతం_ok)
    write_error ();
#అయితే_ముగింపు

  తిరిగి (లోపల_ok && బహిర్గతం_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

స్థిర అక్షరం *
ensure_buf_కొలత (అక్షరం *buf, idx_t *buf_alloc, idx_t alignment, idx_t కొలత)
{
  affirm (buf != ఖాళీ || *buf_alloc < కొలత);

  ఒకవేళ (*buf_alloc < కొలత)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, కొలత);
      *buf_alloc = కొలత;
    }

  తిరిగి buf;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  నిర్మాణం stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool సంఖ్య = false;
  bool సంఖ్య_nonblank = false;
  bool squeeze_blank = false;
  bool show_చివర్లు = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  పూర్ణాంకం file_open_బహుళకం = O_RDONLY;

  స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
  {
    {"number-nonblank", no_వాదం, ఖాళీ, 'b'},
    {"number", no_వాదం, ఖాళీ, 'n'},
    {"squeeze-blank", no_వాదం, ఖాళీ, 's'},
    {"show-nonprinting", no_వాదం, ఖాళీ, 'v'},
    {"show-ends", no_వాదం, ఖాళీ, 'E'},
    {"show-tabs", no_వాదం, ఖాళీ, 'T'},
    {"show-all", no_వాదం, ఖాళీ, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_సంస్కరణ_OPTION_DECL},
    {ఖాళీ, 0, ఖాళీ, 0}
  };

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if we exit via the
     case_GETOPT_HELP_CHAR or case_GETOPT_VERSION_CHAR code.
     Normally STDOUT_FILENO is used rather than stdout, so
     close_stdout does nothing.  */
  atexit (close_stdout);

  /* Parse command line options.  */

  పూర్ణాంకం c;
  అయితే ((c = getopt_long (argc, argv, "benstuvAET", long_options, ఖాళీ))
         != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'b':
          సంఖ్య = true;
          సంఖ్య_nonblank = true;
          ఆపు;

        స్థితి 'e':
          show_చివర్లు = true;
          show_nonprinting = true;
          ఆపు;

        స్థితి 'n':
          సంఖ్య = true;
          ఆపు;

        స్థితి 's':
          squeeze_blank = true;
          ఆపు;

        స్థితి 't':
          show_tabs = true;
          show_nonprinting = true;
          ఆపు;

        స్థితి 'u':
          /* We provide the -u feature unconditionally.  */
          ఆపు;

        స్థితి 'v':
          show_nonprinting = true;
          ఆపు;

        స్థితి 'A':
          show_nonprinting = true;
          show_చివర్లు = true;
          show_tabs = true;
          ఆపు;

        స్థితి 'E':
          show_చివర్లు = true;
          ఆపు;

        స్థితి 'T':
          show_tabs = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  ఒకవేళ (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  పూర్ణాంకం బహిర్గతం_flags = -2;

  /* True if the output is a regular file.  */
  bool బహిర్గతం_isreg = S_ISREG (ostat_buf.st_బహుళకం) != 0;

  ఒకవేళ (! (సంఖ్య || show_చివర్లు || squeeze_blank))
    {
      file_open_బహుళకం |= O_ద్విమాన;
      xset_ద్విమాన_బహుళకం (STDOUT_FILENO, O_ద్విమాన);
    }

  /* Main loop.  */

  infile = "-";
  పూర్ణాంకం argind = optind;
  bool ok = true;
  idx_t పుట_కొలత = getpagesize ();
  అక్షరం *inbuf = ఖాళీ;
  అక్షరం *outbuf = ఖాళీ;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  చేయి
    {
      ఒకవేళ (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      ఒకవేళ (reading_stdin)
        {
          have_read_stdin = true;
          ఆదానం_desc = STDIN_FILENO;
          ఒకవేళ (file_open_బహుళకం & O_ద్విమాన)
            xset_ద్విమాన_బహుళకం (STDIN_FILENO, O_ద్విమాన);
        }
      లేకపోతే
        {
          ఆదానం_desc = open (infile, file_open_బహుళకం);
          ఒకవేళ (ఆదానం_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              కొనసాగు;
            }
        }

      నిర్మాణం stat istat_buf;
      ఒకవేళ (fstat (ఆదానం_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          వెళ్ళు contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (ఆదానం_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      ఒకవేళ (! (S_ISFIFO (istat_buf.st_బహుళకం) || S_ISSOCK (istat_buf.st_బహుళకం)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t లోపల_pos = lseek (ఆదానం_desc, 0, SEEK_CUR);
          ఒకవేళ (0 <= లోపల_pos)
            {
              ఒకవేళ (బహిర్గతం_flags < -1)
                బహిర్గతం_flags = fcntl (STDOUT_FILENO, F_GETFL);
              పూర్ణాంకం whence = (0 <= బహిర్గతం_flags && బహిర్గతం_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              ఒకవేళ (లోపల_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  వెళ్ళు contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      ఒకవేళ (! (సంఖ్య || show_చివర్లు || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          పూర్ణాంకం copy_cat_స్థితి =
            బహిర్గతం_isreg && S_ISREG (istat_buf.st_బహుళకం) ? copy_cat () : 0;
          ఒకవేళ (copy_cat_స్థితి != 0)
            ok &= 0 < copy_cat_స్థితి;
          లేకపోతే
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              పూర్ణాంకం splice_cat_స్థితి = ((usable_st_కొలత (&istat_buf)
                                        && istat_buf.st_కొలత <= 32768)
                                       ? 0 : splice_cat ());
              ఒకవేళ (splice_cat_స్థితి != 0)
                ok &= 0 < splice_cat_స్థితి;
              లేకపోతే
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_కొలత (inbuf, &inbuf_alloc,
                                           పుట_కొలత, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      లేకపోతే
        {
          /* Allocate, with an extra byte for a newline sentinel.  */
          inbuf = ensure_buf_కొలత (inbuf, &inbuf_alloc,
                                   పుట_కొలత, insize + 1);

          /* Why are
             (OUTSIZE - 1 + INSIZE * 4 + LINE_COUNTER_BUF_LEN)
             bytes allocated for the output buffer?

             A test whether output needs to be written is done when the input
             buffer empties or when a newline appears in the input.  After
             output is written, at most (OUTSIZE - 1) bytes will remain in the
             buffer.  Now INSIZE bytes of input is read.  Each input character
             may grow by a factor of 4 (by the prepending of M-^).  If all
             characters do, and no newlines appear in this block of input, we
             will have at most (OUTSIZE - 1 + INSIZE * 4) bytes in the buffer.
             If the last character in the preceding block of input was a
             newline, a line number may be written (according to the given
             options) as the first thing in the output buffer. (Done after the
             new input is read, but before processing of the input begins.)
             A line number requires seldom more than LINE_COUNTER_BUF_LEN
             positions.

             Align the output buffer to a page size boundary, for efficiency
             on some paging implementations.  */

          idx_t bufsize;
          ఒకవేళ (ckd_mul (&bufsize, insize, 4)
              || ckd_కూడు (&bufsize, bufsize, outsize)
              || ckd_కూడు (&bufsize, bufsize, పంక్తి_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_కొలత (outbuf, &outbuf_alloc,
                                    పుట_కొలత, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, సంఖ్య, సంఖ్య_nonblank, show_చివర్లు,
                     squeeze_blank);
        }

    contin:
      ఒకవేళ (!reading_stdin && close (ఆదానం_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  అయితే (++argind < argc);

  ఒకవేళ (pending_cr)
    {
      ఒకవేళ (నిండిన_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#నిర్వచితమైతే lint
  alignfree (outbuf);
  alignfree (inbuf);
#అయితే_ముగింపు

  ఒకవేళ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
