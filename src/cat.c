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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಯದಿ HAVE_STROPTS_H
# include <stropts.h>
#endif
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "alignalloc.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "full-write.h"
#ಅನ್ತರ್ಭಾವ "isapipe.h"
#ಅನ್ತರ್ಭಾವ "splice.h"
#ಅನ್ತರ್ಭಾವ "unistd--.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cat"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *infile;

/* Descriptor on which input file is open.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#define ಸಾಲು_COUNTER_BUF_LEN 20
ಸ್ಥಿರ ವರ್ಣ ಸಾಲು_buf[ಸಾಲು_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
ಸ್ಥಿರ ವರ್ಣ *ಸಾಲು_ಅಂಕೆ_print = ಸಾಲು_buf + ಸಾಲು_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
ಸ್ಥಿರ ವರ್ಣ *ಸಾಲು_ಅಂಕೆ_start = ಸಾಲು_buf + ಸಾಲು_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
ಸ್ಥಿರ ವರ್ಣ *ಸಾಲು_ಅಂಕೆ_end = ಸಾಲು_buf + ಸಾಲು_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ newlines2 = 0;

/* Whether there is a pending CR to process.  */
ಸ್ಥಿರ bool pending_cr = false;

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Concatenate FILE(s) to standard output.\n\
"), stdout);

      emit_stdin_note ();

      oputs (_("\
  -A, --show-all           equivalent to -vET\n\
"));
      oputs (_("\
  -b, --ಸಂಖ್ಯೆ-nonblank    ಸಂಖ್ಯೆ nonempty output ಸಾಲುಗಳು, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent to -vE\n\
"));
      oputs (_("\
  -E, --show-ends          display $ or ^M$ at end of each ಸಾಲು\n\
"));
      oputs (_("\
  -n, --ಸಂಖ್ಯೆ             ಸಂಖ್ಯೆ all output ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated empty output ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -t                       equivalent to -vT\n\
"));
      oputs (_("\
  -T, --show-tabs          display TAB characters as ^I\n\
"));
      oputs (_("\
  -u                       (ignored)\n\
"));
      oputs (_("\
  -v, --show-nonprinting   use ^ and M- notation, except ಪ್ರತಿ LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard ಒಳಹರಿವು to standard output.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Compute the next line number.  */

ಸ್ಥಿರ ಶೂನ್ಯ
next_ಸಾಲು_ಅಂಕೆ (ಶೂನ್ಯ)
{
  ವರ್ಣ *endp = ಸಾಲು_ಅಂಕೆ_end;
  ಕುರು
    {
      ಯದಿ ((*endp)++ < '9')
        ಪ್ರತ್ಯಾಗಮ;
      *endp-- = '0';
    }
  ಯಾವತ್ (endp >= ಸಾಲು_ಅಂಕೆ_start);

  ಯದಿ (ಸಾಲು_ಅಂಕೆ_start > ಸಾಲು_buf)
    *--ಸಾಲು_ಅಂಕೆ_start = '1';
  ಅನ್ಯಥಾ
    *ಸಾಲು_buf = '>';
  ಯದಿ (ಸಾಲು_ಅಂಕೆ_start < ಸಾಲು_ಅಂಕೆ_print)
    ಸಾಲು_ಅಂಕೆ_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

ಸ್ಥಿರ bool
simple_cat (ವರ್ಣ *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  ಯಾವತ್ (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = ಪಠ (ಒಳಹರಿವು_desc, buf, bufsize);
      ಯದಿ (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ಪ್ರತ್ಯಾಗಮ false;
        }

      /* End of this file?  */

      ಯದಿ (n_read == 0)
        ಪ್ರತ್ಯಾಗಮ true;

      /* Write this block out.  */

      ಯದಿ (full_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
write_pending (ವರ್ಣ *outbuf, ವರ್ಣ **bpout)
{
  idx_t n_write = *bpout - outbuf;
  ಯದಿ (0 < n_write)
    {
      ಯದಿ (full_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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

ಸ್ಥಿರ bool
cat (ವರ್ಣ *inbuf, idx_t insize, ವರ್ಣ *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool ಸಂಖ್ಯೆ, bool ಸಂಖ್ಯೆ_nonblank,
     bool show_ends, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  unsigned ವರ್ಣ ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  ಪೂರ್ಣಾಙ್ಕ newlines = newlines2;

#ifdef FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#endif

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  ವರ್ಣ *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  ವರ್ಣ *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  ವರ್ಣ *bpout = outbuf;

  ಯಾವತ್ (true)
    {
      ಕುರು
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          ಯದಿ (outbuf + outsize <= bpout)
            {
              ವರ್ಣ *wp = outbuf;
              idx_t remaining_bytes;
              ಕುರು
                {
                  ಯದಿ (full_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  remaining_bytes = bpout - wp;
                }
              ಯಾವತ್ (outsize <= remaining_bytes);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, remaining_bytes);
              bpout = outbuf + remaining_bytes;
            }

          /* Is INBUF empty?  */

          ಯದಿ (bpin > eob)
            {
              bool ಒಳಹರಿವು_pending = false;
#ifdef FIONREAD
              ಪೂರ್ಣಾಙ್ಕ n_to_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              ಯದಿ (use_fionread
                  && ioctl (ಒಳಹರಿವು_desc, FIONREAD, &n_to_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  ಯದಿ (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  ಅನ್ಯಥಾ
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      ಪ್ರತ್ಯಾಗಮ false;
                    }
                }
              ಯದಿ (n_to_read != 0)
                ಒಳಹರಿವು_pending = true;
#endif

              ಯದಿ (!ಒಳಹರಿವು_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = ಪಠ (ಒಳಹರಿವು_desc, inbuf, insize);
              ಯದಿ (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  ಪ್ರತ್ಯಾಗಮ false;
                }
              ಯದಿ (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  ಪ್ರತ್ಯಾಗಮ true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          ಅನ್ಯಥಾ
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              ಯದಿ (++newlines > 0)
                {
                  ಯದಿ (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      ಯದಿ (squeeze_blank)
                        {
                          ch = *bpin++;
                          ಅನುವರ್ತ;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  ಯದಿ (ಸಂಖ್ಯೆ && !ಸಂಖ್ಯೆ_nonblank)
                    {
                      next_ಸಾಲು_ಅಂಕೆ ();
                      bpout = stpcpy (bpout, ಸಾಲು_ಅಂಕೆ_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              ಯದಿ (show_ends)
                {
                  ಯದಿ (pending_cr)
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
      ಯಾವತ್ (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      ಯದಿ (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      ಯದಿ (newlines >= 0 && ಸಂಖ್ಯೆ)
        {
          next_ಸಾಲು_ಅಂಕೆ ();
          bpout = stpcpy (bpout, ಸಾಲು_ಅಂಕೆ_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      ಯದಿ (show_nonprinting)
        {
          ಯಾವತ್ (true)
            {
              ಯದಿ (ch >= 32)
                {
                  ಯದಿ (ch < 127)
                    *bpout++ = ch;
                  ಅನ್ಯಥಾ ಯದಿ (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  ಅನ್ಯಥಾ
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      ಯದಿ (ch >= 128 + 32)
                        {
                          ಯದಿ (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          ಅನ್ಯಥಾ
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      ಅನ್ಯಥಾ
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              ಅನ್ಯಥಾ ಯದಿ (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              ಅನ್ಯಥಾ ಯದಿ (ch == '\n')
                {
                  newlines = -1;
                  ಭಙ್ಗ;
                }
              ಅನ್ಯಥಾ
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      ಅನ್ಯಥಾ
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          ಯಾವತ್ (true)
            {
              ಯದಿ (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              ಅನ್ಯಥಾ ಯದಿ (ch != '\n')
                {
                  ಯದಿ (ch == '\r' && *bpin == '\n' && show_ends)
                    {
                      ಯದಿ (bpin == eob)
                        pending_cr = true;
                      ಅನ್ಯಥಾ
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  ಅನ್ಯಥಾ
                    *bpout++ = ch;
                }
              ಅನ್ಯಥಾ
                {
                  newlines = -1;
                  ಭಙ್ಗ;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
copy_cat (ಶೂನ್ಯ)
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

  ಪ್ರತಿ (bool some_copied = false; ; some_copied = true)
    ವಿಕಲ್ಪ (copy_file_range (ಒಳಹರಿವು_desc, ರಿಕ್ತ, STDOUT_FILENO, ರಿಕ್ತ,
                             copy_max, 0))
      {
      ಪಕ್ಷ 0:
        ಪ್ರತ್ಯಾಗಮ some_copied;

      ಪಕ್ಷ -1:
        ಯದಿ (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          ಪ್ರತ್ಯಾಗಮ 0;
        error (0, errno, "%s", quotef (infile));
        ಪ್ರತ್ಯಾಗಮ -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
splice_cat (ಶೂನ್ಯ)
{
  bool some_copied = false;
  bool in_ok = true;
  bool out_ok = true;

#ಯದಿ HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  ಸ್ಥಿರ idx_t pipefd_pipe_size = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  ಯದಿ (pipefd[0] < 0)
    {
      ಯದಿ (pipe (pipefd) < 0)
        ಪ್ರತ್ಯಾಗಮ false;
      pipefd_pipe_size = increase_pipe_size (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stdout_is_pipe = -1;
  ಸ್ಥಿರ idx_t stdout_pipe_size = 0;
  ಯದಿ (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      ಯದಿ (stdout_is_pipe)
        stdout_pipe_size = increase_pipe_size (STDOUT_FILENO);
    }

  idx_t pipe_size = MAX (pipefd_pipe_size, stdout_pipe_size);

  ಯಾವತ್ (true)
    {
      ssize_t bytes_read = splice (ಒಳಹರಿವು_desc, ರಿಕ್ತ, pipefd[1], ರಿಕ್ತ,
                                   pipe_size, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      in_ok = 0 <= bytes_read || ! some_copied;
      ಯದಿ (bytes_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      ಯದಿ (bytes_read <= 0)
        ಗಚ್ಛ done;
      /* We need to drain the intermediate pipe to standard output.  */
      ಯಾವತ್ (0 < bytes_read)
        {
          ssize_t bytes_written = splice (pipefd[0], ರಿಕ್ತ, STDOUT_FILENO, ರಿಕ್ತ,
                                          pipe_size, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          ಯದಿ (bytes_written < 0)
            {
              ಯದಿ (some_copied)
                out_ok = false;
              ಅನ್ಯಥಾ
                {
                  ವರ್ಣ buf[BUFSIZ];
                  ಯಾವತ್ (0 < bytes_read)
                    {
                      ssize_t ಎಣಿಕೆ = MIN (bytes_read, ಪರಿಮಾಣ buf);
                      ssize_t n_read = ಪಠ (pipefd[0], buf, ಎಣಿಕೆ);
                      /* Failure not associated with in or out.  */
                      in_ok = out_ok = 0 <= n_read;
                      ಯದಿ (n_read <= 0)
                        ಗಚ್ಛ done;
                      ಯದಿ (full_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      bytes_read -= n_read;
                    }
                }
            }
          ಯದಿ (bytes_written <= 0)
            ಗಚ್ಛ done;
          some_copied = true;
          bytes_read -= bytes_written;
        }
    }

 done:
  ಯದಿ (! in_ok && ! out_ok)
    {
      /* Recreate the pipe on internal error.  */
      ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_size = 0;
      error (0, errno, "%s", _("splice error"));
    }
  ಅನ್ಯಥಾ ಯದಿ (! in_ok)
    error (0, errno, "%s", quotef (infile));
  ಅನ್ಯಥಾ ಯದಿ (! out_ok)
    write_error ();
#endif

  ಪ್ರತ್ಯಾಗಮ (in_ok && out_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

ಸ್ಥಿರ ವರ್ಣ *
ensure_buf_size (ವರ್ಣ *buf, idx_t *buf_alloc, idx_t alignment, idx_t size)
{
  affirm (buf != ರಿಕ್ತ || *buf_alloc < size);

  ಯದಿ (*buf_alloc < size)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, size);
      *buf_alloc = size;
    }

  ಪ್ರತ್ಯಾಗಮ buf;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  ಸಂರಚನಾ stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool ಸಂಖ್ಯೆ = false;
  bool ಸಂಖ್ಯೆ_nonblank = false;
  bool squeeze_blank = false;
  bool show_ends = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  ಪೂರ್ಣಾಙ್ಕ file_open_mode = O_RDONLY;

  ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
  {
    {"number-nonblank", no_argument, ರಿಕ್ತ, 'b'},
    {"number", no_argument, ರಿಕ್ತ, 'n'},
    {"squeeze-blank", no_argument, ರಿಕ್ತ, 's'},
    {"show-nonprinting", no_argument, ರಿಕ್ತ, 'v'},
    {"show-ends", no_argument, ರಿಕ್ತ, 'E'},
    {"show-tabs", no_argument, ರಿಕ್ತ, 'T'},
    {"show-all", no_argument, ರಿಕ್ತ, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
  };

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if we exit via the
     case_GETOPT_HELP_CHAR or case_GETOPT_VERSION_CHAR code.
     Normally STDOUT_FILENO is used rather than stdout, so
     close_stdout does nothing.  */
  atexit (close_stdout);

  /* Parse command line options.  */

  ಪೂರ್ಣಾಙ್ಕ c;
  ಯಾವತ್ ((c = getopt_long (argc, argv, "benstuvAET", long_options, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'b':
          ಸಂಖ್ಯೆ = true;
          ಸಂಖ್ಯೆ_nonblank = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'e':
          show_ends = true;
          show_nonprinting = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'n':
          ಸಂಖ್ಯೆ = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 's':
          squeeze_blank = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 't':
          show_tabs = true;
          show_nonprinting = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'u':
          /* We provide the -u feature unconditionally.  */
          ಭಙ್ಗ;

        ಪಕ್ಷ 'v':
          show_nonprinting = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'A':
          show_nonprinting = true;
          show_ends = true;
          show_tabs = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'E':
          show_ends = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'T':
          show_tabs = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  ಯದಿ (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  ಪೂರ್ಣಾಙ್ಕ out_flags = -2;

  /* True if the output is a regular file.  */
  bool out_isreg = S_ISREG (ostat_buf.st_mode) != 0;

  ಯದಿ (! (ಸಂಖ್ಯೆ || show_ends || squeeze_blank))
    {
      file_open_mode |= O_ದ್ವಿಮಾನ;
      xset_ದ್ವಿಮಾನ_mode (STDOUT_FILENO, O_ದ್ವಿಮಾನ);
    }

  /* Main loop.  */

  infile = "-";
  ಪೂರ್ಣಾಙ್ಕ argind = optind;
  bool ok = true;
  idx_t page_size = getpagesize ();
  ವರ್ಣ *inbuf = ರಿಕ್ತ;
  ವರ್ಣ *outbuf = ರಿಕ್ತ;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  ಕುರು
    {
      ಯದಿ (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      ಯದಿ (reading_stdin)
        {
          have_read_stdin = true;
          ಒಳಹರಿವು_desc = STDIN_FILENO;
          ಯದಿ (file_open_mode & O_ದ್ವಿಮಾನ)
            xset_ದ್ವಿಮಾನ_mode (STDIN_FILENO, O_ದ್ವಿಮಾನ);
        }
      ಅನ್ಯಥಾ
        {
          ಒಳಹರಿವು_desc = open (infile, file_open_mode);
          ಯದಿ (ಒಳಹರಿವು_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              ಅನುವರ್ತ;
            }
        }

      ಸಂರಚನಾ stat istat_buf;
      ಯದಿ (fstat (ಒಳಹರಿವು_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          ಗಚ್ಛ contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (ಒಳಹರಿವು_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      ಯದಿ (! (S_ISFIFO (istat_buf.st_mode) || S_ISSOCK (istat_buf.st_mode)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t in_pos = lseek (ಒಳಹರಿವು_desc, 0, SEEK_CUR);
          ಯದಿ (0 <= in_pos)
            {
              ಯದಿ (out_flags < -1)
                out_flags = fcntl (STDOUT_FILENO, F_GETFL);
              ಪೂರ್ಣಾಙ್ಕ whence = (0 <= out_flags && out_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              ಯದಿ (in_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  ಗಚ್ಛ contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      ಯದಿ (! (ಸಂಖ್ಯೆ || show_ends || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          ಪೂರ್ಣಾಙ್ಕ copy_cat_status =
            out_isreg && S_ISREG (istat_buf.st_mode) ? copy_cat () : 0;
          ಯದಿ (copy_cat_status != 0)
            ok &= 0 < copy_cat_status;
          ಅನ್ಯಥಾ
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              ಪೂರ್ಣಾಙ್ಕ splice_cat_status = ((usable_st_size (&istat_buf)
                                        && istat_buf.st_size <= 32768)
                                       ? 0 : splice_cat ());
              ಯದಿ (splice_cat_status != 0)
                ok &= 0 < splice_cat_status;
              ಅನ್ಯಥಾ
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_size (inbuf, &inbuf_alloc,
                                           page_size, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      ಅನ್ಯಥಾ
        {
          /* Allocate, with an extra byte for a newline sentinel.  */
          inbuf = ensure_buf_size (inbuf, &inbuf_alloc,
                                   page_size, insize + 1);

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
          ಯದಿ (ckd_mul (&bufsize, insize, 4)
              || ckd_add (&bufsize, bufsize, outsize)
              || ckd_add (&bufsize, bufsize, ಸಾಲು_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_size (outbuf, &outbuf_alloc,
                                    page_size, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, ಸಂಖ್ಯೆ, ಸಂಖ್ಯೆ_nonblank, show_ends,
                     squeeze_blank);
        }

    contin:
      ಯದಿ (!reading_stdin && close (ಒಳಹರಿವು_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  ಯಾವತ್ (++argind < argc);

  ಯದಿ (pending_cr)
    {
      ಯದಿ (full_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#ifdef lint
  alignfree (outbuf);
  alignfree (inbuf);
#endif

  ಯದಿ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
