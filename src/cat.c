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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਜੇ HAVE_STROPTS_H
# include <stropts.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/ioctl.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "alignalloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "full-write.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "isapipe.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "splice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unistd--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "cat"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *infile;

/* Descriptor on which input file is open.  */
ਸਥਿਰ ਪੂਰਨ ਨਿਵੇਸ਼_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਤਰ_COUNTER_BUF_LEN 20
ਸਥਿਰ ਅੱਖਰ ਸਤਰ_buf[ਸਤਰ_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
ਸਥਿਰ ਅੱਖਰ *ਸਤਰ_ਅੰਕ_print = ਸਤਰ_buf + ਸਤਰ_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
ਸਥਿਰ ਅੱਖਰ *ਸਤਰ_ਅੰਕ_ਸ਼ੁਰੂ = ਸਤਰ_buf + ਸਤਰ_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
ਸਥਿਰ ਅੱਖਰ *ਸਤਰ_ਅੰਕ_ਸਿਰਾ = ਸਤਰ_buf + ਸਤਰ_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
ਸਥਿਰ ਪੂਰਨ newlines2 = 0;

/* Whether there is a pending CR to process.  */
ਸਥਿਰ bool pending_cr = false;

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Concatenate FILE(s) ਮੰਜ਼ਿਲ standard output.\n\
"), stdout);

      emit_stdin_note ();

      oputs (_("\
  -A, --show-all           equivalent ਮੰਜ਼ਿਲ -vET\n\
"));
      oputs (_("\
  -b, --ਸੰਖਿਆ-nonblank    ਸੰਖਿਆ nonempty output ਸਤਰਾਂ, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent ਮੰਜ਼ਿਲ -vE\n\
"));
      oputs (_("\
  -E, --show-ਸਿਰੇ          display $ or ^M$ at ਸਿਰਾ ਦਾ each ਸਤਰ\n\
"));
      oputs (_("\
  -n, --ਸੰਖਿਆ             ਸੰਖਿਆ all output ਸਤਰਾਂ\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated ਖਾਲੀ_ਥਾਵਾਂ output ਸਤਰਾਂ\n\
"));
      oputs (_("\
  -t                       equivalent ਮੰਜ਼ਿਲ -vT\n\
"));
      oputs (_("\
  -T, --show-tabs          display TAB characters as ^I\n\
"));
      oputs (_("\
  -u                       (ignored)\n\
"));
      oputs (_("\
  -v, --show-nonprinting   use ^ and M- notation, except ਲਈ LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      ਲਿਖੋ (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard ਨਿਵੇਸ਼ ਮੰਜ਼ਿਲ standard output.\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Compute the next line number.  */

ਸਥਿਰ ਖਾਲੀ
ਅਗਲਾ_ਸਤਰ_ਅੰਕ (ਖਾਲੀ)
{
  ਅੱਖਰ *endp = ਸਤਰ_ਅੰਕ_ਸਿਰਾ;
  ਕਰੋ
    {
      ਜੇ ((*endp)++ < '9')
        ਵਾਪਸ;
      *endp-- = '0';
    }
  ਜਦੋਂ (endp >= ਸਤਰ_ਅੰਕ_ਸ਼ੁਰੂ);

  ਜੇ (ਸਤਰ_ਅੰਕ_ਸ਼ੁਰੂ > ਸਤਰ_buf)
    *--ਸਤਰ_ਅੰਕ_ਸ਼ੁਰੂ = '1';
  ਨਹੀਂਤਾਂ
    *ਸਤਰ_buf = '>';
  ਜੇ (ਸਤਰ_ਅੰਕ_ਸ਼ੁਰੂ < ਸਤਰ_ਅੰਕ_print)
    ਸਤਰ_ਅੰਕ_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

ਸਥਿਰ bool
simple_cat (ਅੱਖਰ *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  ਜਦੋਂ (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = ਪਠ (ਨਿਵੇਸ਼_desc, buf, bufsize);
      ਜੇ (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ਵਾਪਸ false;
        }

      /* End of this file?  */

      ਜੇ (n_read == 0)
        ਵਾਪਸ true;

      /* Write this block out.  */

      ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
write_pending (ਅੱਖਰ *outbuf, ਅੱਖਰ **bpout)
{
  idx_t n_write = *bpout - outbuf;
  ਜੇ (0 < n_write)
    {
      ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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

ਸਥਿਰ bool
cat (ਅੱਖਰ *inbuf, idx_t insize, ਅੱਖਰ *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool ਸੰਖਿਆ, bool ਸੰਖਿਆ_nonblank,
     bool show_ਸਿਰੇ, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  ਪੂਰਨ newlines = newlines2;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#ਜੇ_ਅੰਤ

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  ਅੱਖਰ *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  ਅੱਖਰ *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  ਅੱਖਰ *bpout = outbuf;

  ਜਦੋਂ (true)
    {
      ਕਰੋ
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          ਜੇ (outbuf + outsize <= bpout)
            {
              ਅੱਖਰ *wp = outbuf;
              idx_t ਬਚਿਆ_ਬਾਈਟਾਂ;
              ਕਰੋ
                {
                  ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  ਬਚਿਆ_ਬਾਈਟਾਂ = bpout - wp;
                }
              ਜਦੋਂ (outsize <= ਬਚਿਆ_ਬਾਈਟਾਂ);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, ਬਚਿਆ_ਬਾਈਟਾਂ);
              bpout = outbuf + ਬਚਿਆ_ਬਾਈਟਾਂ;
            }

          /* Is INBUF empty?  */

          ਜੇ (bpin > eob)
            {
              bool ਨਿਵੇਸ਼_pending = false;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FIONREAD
              ਪੂਰਨ n_ਮੰਜ਼ਿਲ_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              ਜੇ (use_fionread
                  && ioctl (ਨਿਵੇਸ਼_desc, FIONREAD, &n_ਮੰਜ਼ਿਲ_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  ਜੇ (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  ਨਹੀਂਤਾਂ
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      ਵਾਪਸ false;
                    }
                }
              ਜੇ (n_ਮੰਜ਼ਿਲ_read != 0)
                ਨਿਵੇਸ਼_pending = true;
#ਜੇ_ਅੰਤ

              ਜੇ (!ਨਿਵੇਸ਼_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = ਪਠ (ਨਿਵੇਸ਼_desc, inbuf, insize);
              ਜੇ (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  ਵਾਪਸ false;
                }
              ਜੇ (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  ਵਾਪਸ true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          ਨਹੀਂਤਾਂ
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              ਜੇ (++newlines > 0)
                {
                  ਜੇ (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      ਜੇ (squeeze_blank)
                        {
                          ch = *bpin++;
                          ਜਾਰੀ;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  ਜੇ (ਸੰਖਿਆ && !ਸੰਖਿਆ_nonblank)
                    {
                      ਅਗਲਾ_ਸਤਰ_ਅੰਕ ();
                      bpout = stpcpy (bpout, ਸਤਰ_ਅੰਕ_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              ਜੇ (show_ਸਿਰੇ)
                {
                  ਜੇ (pending_cr)
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
      ਜਦੋਂ (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      ਜੇ (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      ਜੇ (newlines >= 0 && ਸੰਖਿਆ)
        {
          ਅਗਲਾ_ਸਤਰ_ਅੰਕ ();
          bpout = stpcpy (bpout, ਸਤਰ_ਅੰਕ_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      ਜੇ (show_nonprinting)
        {
          ਜਦੋਂ (true)
            {
              ਜੇ (ch >= 32)
                {
                  ਜੇ (ch < 127)
                    *bpout++ = ch;
                  ਨਹੀਂਤਾਂ ਜੇ (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  ਨਹੀਂਤਾਂ
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      ਜੇ (ch >= 128 + 32)
                        {
                          ਜੇ (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          ਨਹੀਂਤਾਂ
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      ਨਹੀਂਤਾਂ
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              ਨਹੀਂਤਾਂ ਜੇ (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              ਨਹੀਂਤਾਂ ਜੇ (ch == '\n')
                {
                  newlines = -1;
                  ਰੋਕੋ;
                }
              ਨਹੀਂਤਾਂ
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      ਨਹੀਂਤਾਂ
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          ਜਦੋਂ (true)
            {
              ਜੇ (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              ਨਹੀਂਤਾਂ ਜੇ (ch != '\n')
                {
                  ਜੇ (ch == '\r' && *bpin == '\n' && show_ਸਿਰੇ)
                    {
                      ਜੇ (bpin == eob)
                        pending_cr = true;
                      ਨਹੀਂਤਾਂ
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  ਨਹੀਂਤਾਂ
                    *bpout++ = ch;
                }
              ਨਹੀਂਤਾਂ
                {
                  newlines = -1;
                  ਰੋਕੋ;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ਸਥਿਰ ਪੂਰਨ
copy_cat (ਖਾਲੀ)
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

  ਲਈ (bool some_copied = false; ; some_copied = true)
    ਚੋਣ (copy_file_range (ਨਿਵੇਸ਼_desc, ਨਿਰਾ, STDOUT_FILENO, ਨਿਰਾ,
                             copy_max, 0))
      {
      ਹਾਲਤ 0:
        ਵਾਪਸ some_copied;

      ਹਾਲਤ -1:
        ਜੇ (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          ਵਾਪਸ 0;
        error (0, errno, "%s", quotef (infile));
        ਵਾਪਸ -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ਸਥਿਰ ਪੂਰਨ
splice_cat (ਖਾਲੀ)
{
  bool some_copied = false;
  bool ਵਿੱਚ_ok = true;
  bool ਬਾਹਰਲਾ_ok = true;

#ਜੇ HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  ਸਥਿਰ ਪੂਰਨ pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  ਸਥਿਰ idx_t pipefd_pipe_ਮਾਪ = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  ਜੇ (pipefd[0] < 0)
    {
      ਜੇ (pipe (pipefd) < 0)
        ਵਾਪਸ false;
      pipefd_pipe_ਮਾਪ = increase_pipe_ਮਾਪ (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  ਸਥਿਰ ਪੂਰਨ stdout_is_pipe = -1;
  ਸਥਿਰ idx_t stdout_pipe_ਮਾਪ = 0;
  ਜੇ (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      ਜੇ (stdout_is_pipe)
        stdout_pipe_ਮਾਪ = increase_pipe_ਮਾਪ (STDOUT_FILENO);
    }

  idx_t pipe_ਮਾਪ = MAX (pipefd_pipe_ਮਾਪ, stdout_pipe_ਮਾਪ);

  ਜਦੋਂ (true)
    {
      ssize_t ਬਾਈਟਾਂ_read = splice (ਨਿਵੇਸ਼_desc, ਨਿਰਾ, pipefd[1], ਨਿਰਾ,
                                   pipe_ਮਾਪ, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      ਵਿੱਚ_ok = 0 <= ਬਾਈਟਾਂ_read || ! some_copied;
      ਜੇ (ਬਾਈਟਾਂ_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      ਜੇ (ਬਾਈਟਾਂ_read <= 0)
        ਜਾਓ ਪੂਰਾ;
      /* We need to drain the intermediate pipe to standard output.  */
      ਜਦੋਂ (0 < ਬਾਈਟਾਂ_read)
        {
          ssize_t ਬਾਈਟਾਂ_written = splice (pipefd[0], ਨਿਰਾ, STDOUT_FILENO, ਨਿਰਾ,
                                          pipe_ਮਾਪ, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          ਜੇ (ਬਾਈਟਾਂ_written < 0)
            {
              ਜੇ (some_copied)
                ਬਾਹਰਲਾ_ok = false;
              ਨਹੀਂਤਾਂ
                {
                  ਅੱਖਰ buf[BUFSIZ];
                  ਜਦੋਂ (0 < ਬਾਈਟਾਂ_read)
                    {
                      ssize_t ਗਿਣਤੀ_2 = MIN (ਬਾਈਟਾਂ_read, ਆਕਾਰ buf);
                      ssize_t n_read = ਪਠ (pipefd[0], buf, ਗਿਣਤੀ_2);
                      /* Failure not associated with in or out.  */
                      ਵਿੱਚ_ok = ਬਾਹਰਲਾ_ok = 0 <= n_read;
                      ਜੇ (n_read <= 0)
                        ਜਾਓ ਪੂਰਾ;
                      ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      ਬਾਈਟਾਂ_read -= n_read;
                    }
                }
            }
          ਜੇ (ਬਾਈਟਾਂ_written <= 0)
            ਜਾਓ ਪੂਰਾ;
          some_copied = true;
          ਬਾਈਟਾਂ_read -= ਬਾਈਟਾਂ_written;
        }
    }

 ਪੂਰਾ:
  ਜੇ (! ਵਿੱਚ_ok && ! ਬਾਹਰਲਾ_ok)
    {
      /* Recreate the pipe on internal error.  */
      ਪੂਰਨ saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_ਮਾਪ = 0;
      error (0, errno, "%s", _("splice error"));
    }
  ਨਹੀਂਤਾਂ ਜੇ (! ਵਿੱਚ_ok)
    error (0, errno, "%s", quotef (infile));
  ਨਹੀਂਤਾਂ ਜੇ (! ਬਾਹਰਲਾ_ok)
    write_error ();
#ਜੇ_ਅੰਤ

  ਵਾਪਸ (ਵਿੱਚ_ok && ਬਾਹਰਲਾ_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

ਸਥਿਰ ਅੱਖਰ *
ensure_buf_ਮਾਪ (ਅੱਖਰ *buf, idx_t *buf_alloc, idx_t alignment, idx_t ਮਾਪ)
{
  affirm (buf != ਨਿਰਾ || *buf_alloc < ਮਾਪ);

  ਜੇ (*buf_alloc < ਮਾਪ)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, ਮਾਪ);
      *buf_alloc = ਮਾਪ;
    }

  ਵਾਪਸ buf;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  ਬਣਤਰ stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool ਸੰਖਿਆ = false;
  bool ਸੰਖਿਆ_nonblank = false;
  bool squeeze_blank = false;
  bool show_ਸਿਰੇ = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  ਪੂਰਨ file_open_ਬਹੁਲਕ = O_RDONLY;

  ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
  {
    {"number-nonblank", no_ਦਲੀਲ, ਨਿਰਾ, 'b'},
    {"number", no_ਦਲੀਲ, ਨਿਰਾ, 'n'},
    {"squeeze-blank", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
    {"show-nonprinting", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
    {"show-ends", no_ਦਲੀਲ, ਨਿਰਾ, 'E'},
    {"show-tabs", no_ਦਲੀਲ, ਨਿਰਾ, 'T'},
    {"show-all", no_ਦਲੀਲ, ਨਿਰਾ, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
    {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
  };

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if we exit via the
     case_GETOPT_HELP_CHAR or case_GETOPT_VERSION_CHAR code.
     Normally STDOUT_FILENO is used rather than stdout, so
     close_stdout does nothing.  */
  atexit (close_stdout);

  /* Parse command line options.  */

  ਪੂਰਨ c;
  ਜਦੋਂ ((c = getopt_long (argc, argv, "benstuvAET", long_options, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'b':
          ਸੰਖਿਆ = true;
          ਸੰਖਿਆ_nonblank = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'e':
          show_ਸਿਰੇ = true;
          show_nonprinting = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'n':
          ਸੰਖਿਆ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          squeeze_blank = true;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          show_tabs = true;
          show_nonprinting = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'u':
          /* We provide the -u feature unconditionally.  */
          ਰੋਕੋ;

        ਹਾਲਤ 'v':
          show_nonprinting = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'A':
          show_nonprinting = true;
          show_ਸਿਰੇ = true;
          show_tabs = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'E':
          show_ਸਿਰੇ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'T':
          show_tabs = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  ਜੇ (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  ਪੂਰਨ ਬਾਹਰਲਾ_flags = -2;

  /* True if the output is a regular file.  */
  bool ਬਾਹਰਲਾ_isreg = S_ISREG (ostat_buf.st_ਬਹੁਲਕ) != 0;

  ਜੇ (! (ਸੰਖਿਆ || show_ਸਿਰੇ || squeeze_blank))
    {
      file_open_ਬਹੁਲਕ |= O_ਦੁਆਧਾਰੀ;
      xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDOUT_FILENO, O_ਦੁਆਧਾਰੀ);
    }

  /* Main loop.  */

  infile = "-";
  ਪੂਰਨ argind = optind;
  bool ok = true;
  idx_t ਪੰਨਾ_ਮਾਪ = getpagesize ();
  ਅੱਖਰ *inbuf = ਨਿਰਾ;
  ਅੱਖਰ *outbuf = ਨਿਰਾ;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  ਕਰੋ
    {
      ਜੇ (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      ਜੇ (reading_stdin)
        {
          have_read_stdin = true;
          ਨਿਵੇਸ਼_desc = STDIN_FILENO;
          ਜੇ (file_open_ਬਹੁਲਕ & O_ਦੁਆਧਾਰੀ)
            xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDIN_FILENO, O_ਦੁਆਧਾਰੀ);
        }
      ਨਹੀਂਤਾਂ
        {
          ਨਿਵੇਸ਼_desc = open (infile, file_open_ਬਹੁਲਕ);
          ਜੇ (ਨਿਵੇਸ਼_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              ਜਾਰੀ;
            }
        }

      ਬਣਤਰ stat istat_buf;
      ਜੇ (fstat (ਨਿਵੇਸ਼_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          ਜਾਓ contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (ਨਿਵੇਸ਼_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      ਜੇ (! (S_ISFIFO (istat_buf.st_ਬਹੁਲਕ) || S_ISSOCK (istat_buf.st_ਬਹੁਲਕ)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t ਵਿੱਚ_pos = lseek (ਨਿਵੇਸ਼_desc, 0, SEEK_CUR);
          ਜੇ (0 <= ਵਿੱਚ_pos)
            {
              ਜੇ (ਬਾਹਰਲਾ_flags < -1)
                ਬਾਹਰਲਾ_flags = fcntl (STDOUT_FILENO, F_GETFL);
              ਪੂਰਨ whence = (0 <= ਬਾਹਰਲਾ_flags && ਬਾਹਰਲਾ_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              ਜੇ (ਵਿੱਚ_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  ਜਾਓ contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      ਜੇ (! (ਸੰਖਿਆ || show_ਸਿਰੇ || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          ਪੂਰਨ copy_cat_ਸਥਿਤੀ =
            ਬਾਹਰਲਾ_isreg && S_ISREG (istat_buf.st_ਬਹੁਲਕ) ? copy_cat () : 0;
          ਜੇ (copy_cat_ਸਥਿਤੀ != 0)
            ok &= 0 < copy_cat_ਸਥਿਤੀ;
          ਨਹੀਂਤਾਂ
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              ਪੂਰਨ splice_cat_ਸਥਿਤੀ = ((usable_st_ਮਾਪ (&istat_buf)
                                        && istat_buf.st_ਮਾਪ <= 32768)
                                       ? 0 : splice_cat ());
              ਜੇ (splice_cat_ਸਥਿਤੀ != 0)
                ok &= 0 < splice_cat_ਸਥਿਤੀ;
              ਨਹੀਂਤਾਂ
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_ਮਾਪ (inbuf, &inbuf_alloc,
                                           ਪੰਨਾ_ਮਾਪ, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      ਨਹੀਂਤਾਂ
        {
          /* Allocate, with an extra byte for a newline sentinel.  */
          inbuf = ensure_buf_ਮਾਪ (inbuf, &inbuf_alloc,
                                   ਪੰਨਾ_ਮਾਪ, insize + 1);

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
          ਜੇ (ckd_mul (&bufsize, insize, 4)
              || ckd_ਜੋੜੋ (&bufsize, bufsize, outsize)
              || ckd_ਜੋੜੋ (&bufsize, bufsize, ਸਤਰ_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_ਮਾਪ (outbuf, &outbuf_alloc,
                                    ਪੰਨਾ_ਮਾਪ, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, ਸੰਖਿਆ, ਸੰਖਿਆ_nonblank, show_ਸਿਰੇ,
                     squeeze_blank);
        }

    contin:
      ਜੇ (!reading_stdin && close (ਨਿਵੇਸ਼_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  ਜਦੋਂ (++argind < argc);

  ਜੇ (pending_cr)
    {
      ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ lint
  alignfree (outbuf);
  alignfree (inbuf);
#ਜੇ_ਅੰਤ

  ਜੇ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
