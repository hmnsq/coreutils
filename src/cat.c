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

#ಆದರೆ HAVE_STROPTS_H
# include <stropts.h>
#ಆದರೆ_ಅಂತ್ಯ
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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "cat"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *infile;

/* Descriptor on which input file is open.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸಾಲು_COUNTER_BUF_LEN 20
ಸ್ಥಿರ ಅಕ್ಷರ ಸಾಲು_buf[ಸಾಲು_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
ಸ್ಥಿರ ಅಕ್ಷರ *ಸಾಲು_ಅಂಕೆ_print = ಸಾಲು_buf + ಸಾಲು_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
ಸ್ಥಿರ ಅಕ್ಷರ *ಸಾಲು_ಅಂಕೆ_ಆರಂಭ = ಸಾಲು_buf + ಸಾಲು_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
ಸ್ಥಿರ ಅಕ್ಷರ *ಸಾಲು_ಅಂಕೆ_ಕೊನೆ = ಸಾಲು_buf + ಸಾಲು_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ newlines2 = 0;

/* Whether there is a pending CR to process.  */
ಸ್ಥಿರ bool pending_cr = false;

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Concatenate FILE(s) ಗಮ್ಯ standard output.\n\
"), stdout);

      emit_stdin_note ();

      oputs (_("\
  -A, --show-all           equivalent ಗಮ್ಯ -vET\n\
"));
      oputs (_("\
  -b, --ಸಂಖ್ಯೆ-nonblank    ಸಂಖ್ಯೆ nonempty output ಸಾಲುಗಳು, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent ಗಮ್ಯ -vE\n\
"));
      oputs (_("\
  -E, --show-ತುದಿಗಳು          display $ or ^M$ at ಕೊನೆ ದ each ಸಾಲು\n\
"));
      oputs (_("\
  -n, --ಸಂಖ್ಯೆ             ಸಂಖ್ಯೆ all output ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated ಖಾಲಿಗಳು output ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -t                       equivalent ಗಮ್ಯ -vT\n\
"));
      oputs (_("\
  -T, --show-tabs          display TAB characters as ^I\n\
"));
      oputs (_("\
  -u                       (ignored)\n\
"));
      oputs (_("\
  -v, --show-nonprinting   use ^ and M- notation, except ಗಾಗಿ LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      ಬರೆ (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard ಒಳಹರಿವು ಗಮ್ಯ standard output.\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Compute the next line number.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಮುಂದಿನ_ಸಾಲು_ಅಂಕೆ (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ *endp = ಸಾಲು_ಅಂಕೆ_ಕೊನೆ;
  ಮಾಡು
    {
      ಆದರೆ ((*endp)++ < '9')
        ಹಿಂತಿರುಗಿ;
      *endp-- = '0';
    }
  ಆಗ (endp >= ಸಾಲು_ಅಂಕೆ_ಆರಂಭ);

  ಆದರೆ (ಸಾಲು_ಅಂಕೆ_ಆರಂಭ > ಸಾಲು_buf)
    *--ಸಾಲು_ಅಂಕೆ_ಆರಂಭ = '1';
  ಇಲ್ಲದಿದ್ದರೆ
    *ಸಾಲು_buf = '>';
  ಆದರೆ (ಸಾಲು_ಅಂಕೆ_ಆರಂಭ < ಸಾಲು_ಅಂಕೆ_print)
    ಸಾಲು_ಅಂಕೆ_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

ಸ್ಥಿರ bool
simple_cat (ಅಕ್ಷರ *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  ಆಗ (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = ಪಠ (ಒಳಹರಿವು_desc, buf, bufsize);
      ಆದರೆ (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ಹಿಂತಿರುಗಿ false;
        }

      /* End of this file?  */

      ಆದರೆ (n_read == 0)
        ಹಿಂತಿರುಗಿ true;

      /* Write this block out.  */

      ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
write_pending (ಅಕ್ಷರ *outbuf, ಅಕ್ಷರ **bpout)
{
  idx_t n_write = *bpout - outbuf;
  ಆದರೆ (0 < n_write)
    {
      ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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
cat (ಅಕ್ಷರ *inbuf, idx_t insize, ಅಕ್ಷರ *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool ಸಂಖ್ಯೆ, bool ಸಂಖ್ಯೆ_nonblank,
     bool show_ತುದಿಗಳು, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  ಪೂರ್ಣಾಂಕ newlines = newlines2;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#ಆದರೆ_ಅಂತ್ಯ

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  ಅಕ್ಷರ *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  ಅಕ್ಷರ *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  ಅಕ್ಷರ *bpout = outbuf;

  ಆಗ (true)
    {
      ಮಾಡು
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          ಆದರೆ (outbuf + outsize <= bpout)
            {
              ಅಕ್ಷರ *wp = outbuf;
              idx_t ಉಳಿದ_ಬೈಟುಗಳು;
              ಮಾಡು
                {
                  ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  ಉಳಿದ_ಬೈಟುಗಳು = bpout - wp;
                }
              ಆಗ (outsize <= ಉಳಿದ_ಬೈಟುಗಳು);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, ಉಳಿದ_ಬೈಟುಗಳು);
              bpout = outbuf + ಉಳಿದ_ಬೈಟುಗಳು;
            }

          /* Is INBUF empty?  */

          ಆದರೆ (bpin > eob)
            {
              bool ಒಳಹರಿವು_pending = false;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FIONREAD
              ಪೂರ್ಣಾಂಕ n_ಗಮ್ಯ_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              ಆದರೆ (use_fionread
                  && ioctl (ಒಳಹರಿವು_desc, FIONREAD, &n_ಗಮ್ಯ_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  ಆದರೆ (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  ಇಲ್ಲದಿದ್ದರೆ
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      ಹಿಂತಿರುಗಿ false;
                    }
                }
              ಆದರೆ (n_ಗಮ್ಯ_read != 0)
                ಒಳಹರಿವು_pending = true;
#ಆದರೆ_ಅಂತ್ಯ

              ಆದರೆ (!ಒಳಹರಿವು_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = ಪಠ (ಒಳಹರಿವು_desc, inbuf, insize);
              ಆದರೆ (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  ಹಿಂತಿರುಗಿ false;
                }
              ಆದರೆ (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  ಹಿಂತಿರುಗಿ true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              ಆದರೆ (++newlines > 0)
                {
                  ಆದರೆ (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      ಆದರೆ (squeeze_blank)
                        {
                          ch = *bpin++;
                          ಮುಂದುವರಿ;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  ಆದರೆ (ಸಂಖ್ಯೆ && !ಸಂಖ್ಯೆ_nonblank)
                    {
                      ಮುಂದಿನ_ಸಾಲು_ಅಂಕೆ ();
                      bpout = stpcpy (bpout, ಸಾಲು_ಅಂಕೆ_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              ಆದರೆ (show_ತುದಿಗಳು)
                {
                  ಆದರೆ (pending_cr)
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
      ಆಗ (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      ಆದರೆ (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      ಆದರೆ (newlines >= 0 && ಸಂಖ್ಯೆ)
        {
          ಮುಂದಿನ_ಸಾಲು_ಅಂಕೆ ();
          bpout = stpcpy (bpout, ಸಾಲು_ಅಂಕೆ_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      ಆದರೆ (show_nonprinting)
        {
          ಆಗ (true)
            {
              ಆದರೆ (ch >= 32)
                {
                  ಆದರೆ (ch < 127)
                    *bpout++ = ch;
                  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  ಇಲ್ಲದಿದ್ದರೆ
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      ಆದರೆ (ch >= 128 + 32)
                        {
                          ಆದರೆ (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          ಇಲ್ಲದಿದ್ದರೆ
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      ಇಲ್ಲದಿದ್ದರೆ
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ch == '\n')
                {
                  newlines = -1;
                  ನಿಲ್ಲಿಸು;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          ಆಗ (true)
            {
              ಆದರೆ (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ch != '\n')
                {
                  ಆದರೆ (ch == '\r' && *bpin == '\n' && show_ತುದಿಗಳು)
                    {
                      ಆದರೆ (bpin == eob)
                        pending_cr = true;
                      ಇಲ್ಲದಿದ್ದರೆ
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  ಇಲ್ಲದಿದ್ದರೆ
                    *bpout++ = ch;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  newlines = -1;
                  ನಿಲ್ಲಿಸು;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
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

  ಗಾಗಿ (bool some_copied = false; ; some_copied = true)
    ಆಯ್ಕೆ (copy_file_range (ಒಳಹರಿವು_desc, ಖಾಲಿ, STDOUT_FILENO, ಖಾಲಿ,
                             copy_max, 0))
      {
      ಸ್ಥಿತಿ 0:
        ಹಿಂತಿರುಗಿ some_copied;

      ಸ್ಥಿತಿ -1:
        ಆದರೆ (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          ಹಿಂತಿರುಗಿ 0;
        error (0, errno, "%s", quotef (infile));
        ಹಿಂತಿರುಗಿ -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
splice_cat (ಶೂನ್ಯ)
{
  bool some_copied = false;
  bool ಒಳ_ok = true;
  bool ಹೊರಹೋಗು_ok = true;

#ಆದರೆ HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  ಸ್ಥಿರ idx_t pipefd_pipe_ಗಾತ್ರ = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  ಆದರೆ (pipefd[0] < 0)
    {
      ಆದರೆ (pipe (pipefd) < 0)
        ಹಿಂತಿರುಗಿ false;
      pipefd_pipe_ಗಾತ್ರ = increase_pipe_ಗಾತ್ರ (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stdout_is_pipe = -1;
  ಸ್ಥಿರ idx_t stdout_pipe_ಗಾತ್ರ = 0;
  ಆದರೆ (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      ಆದರೆ (stdout_is_pipe)
        stdout_pipe_ಗಾತ್ರ = increase_pipe_ಗಾತ್ರ (STDOUT_FILENO);
    }

  idx_t pipe_ಗಾತ್ರ = MAX (pipefd_pipe_ಗಾತ್ರ, stdout_pipe_ಗಾತ್ರ);

  ಆಗ (true)
    {
      ssize_t ಬೈಟುಗಳು_read = splice (ಒಳಹರಿವು_desc, ಖಾಲಿ, pipefd[1], ಖಾಲಿ,
                                   pipe_ಗಾತ್ರ, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      ಒಳ_ok = 0 <= ಬೈಟುಗಳು_read || ! some_copied;
      ಆದರೆ (ಬೈಟುಗಳು_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      ಆದರೆ (ಬೈಟುಗಳು_read <= 0)
        ಹೋಗು ಮುಗಿದ;
      /* We need to drain the intermediate pipe to standard output.  */
      ಆಗ (0 < ಬೈಟುಗಳು_read)
        {
          ssize_t ಬೈಟುಗಳು_written = splice (pipefd[0], ಖಾಲಿ, STDOUT_FILENO, ಖಾಲಿ,
                                          pipe_ಗಾತ್ರ, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          ಆದರೆ (ಬೈಟುಗಳು_written < 0)
            {
              ಆದರೆ (some_copied)
                ಹೊರಹೋಗು_ok = false;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಅಕ್ಷರ buf[BUFSIZ];
                  ಆಗ (0 < ಬೈಟುಗಳು_read)
                    {
                      ssize_t ಎಣಿಕೆ_2 = MIN (ಬೈಟುಗಳು_read, ಗಾತ್ರ buf);
                      ssize_t n_read = ಪಠ (pipefd[0], buf, ಎಣಿಕೆ_2);
                      /* Failure not associated with in or out.  */
                      ಒಳ_ok = ಹೊರಹೋಗು_ok = 0 <= n_read;
                      ಆದರೆ (n_read <= 0)
                        ಹೋಗು ಮುಗಿದ;
                      ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      ಬೈಟುಗಳು_read -= n_read;
                    }
                }
            }
          ಆದರೆ (ಬೈಟುಗಳು_written <= 0)
            ಹೋಗು ಮುಗಿದ;
          some_copied = true;
          ಬೈಟುಗಳು_read -= ಬೈಟುಗಳು_written;
        }
    }

 ಮುಗಿದ:
  ಆದರೆ (! ಒಳ_ok && ! ಹೊರಹೋಗು_ok)
    {
      /* Recreate the pipe on internal error.  */
      ಪೂರ್ಣಾಂಕ saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_ಗಾತ್ರ = 0;
      error (0, errno, "%s", _("splice error"));
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! ಒಳ_ok)
    error (0, errno, "%s", quotef (infile));
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! ಹೊರಹೋಗು_ok)
    write_error ();
#ಆದರೆ_ಅಂತ್ಯ

  ಹಿಂತಿರುಗಿ (ಒಳ_ok && ಹೊರಹೋಗು_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
ensure_buf_ಗಾತ್ರ (ಅಕ್ಷರ *buf, idx_t *buf_alloc, idx_t alignment, idx_t ಗಾತ್ರ_2)
{
  affirm (buf != ಖಾಲಿ || *buf_alloc < ಗಾತ್ರ_2);

  ಆದರೆ (*buf_alloc < ಗಾತ್ರ_2)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, ಗಾತ್ರ_2);
      *buf_alloc = ಗಾತ್ರ_2;
    }

  ಹಿಂತಿರುಗಿ buf;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  ರಚನೆ stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool ಸಂಖ್ಯೆ = false;
  bool ಸಂಖ್ಯೆ_nonblank = false;
  bool squeeze_blank = false;
  bool show_ತುದಿಗಳು = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  ಪೂರ್ಣಾಂಕ file_open_ಬಹುಲಕ = O_RDONLY;

  ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
  {
    {"number-nonblank", no_ವಾದ, ಖಾಲಿ, 'b'},
    {"number", no_ವಾದ, ಖಾಲಿ, 'n'},
    {"squeeze-blank", no_ವಾದ, ಖಾಲಿ, 's'},
    {"show-nonprinting", no_ವಾದ, ಖಾಲಿ, 'v'},
    {"show-ends", no_ವಾದ, ಖಾಲಿ, 'E'},
    {"show-tabs", no_ವಾದ, ಖಾಲಿ, 'T'},
    {"show-all", no_ವಾದ, ಖಾಲಿ, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
    {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
  };

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if we exit via the
     case_GETOPT_HELP_CHAR or case_GETOPT_VERSION_CHAR code.
     Normally STDOUT_FILENO is used rather than stdout, so
     close_stdout does nothing.  */
  atexit (close_stdout);

  /* Parse command line options.  */

  ಪೂರ್ಣಾಂಕ c;
  ಆಗ ((c = getopt_long (argc, argv, "benstuvAET", long_options, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'b':
          ಸಂಖ್ಯೆ = true;
          ಸಂಖ್ಯೆ_nonblank = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'e':
          show_ತುದಿಗಳು = true;
          show_nonprinting = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'n':
          ಸಂಖ್ಯೆ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          squeeze_blank = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          show_tabs = true;
          show_nonprinting = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'u':
          /* We provide the -u feature unconditionally.  */
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'v':
          show_nonprinting = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'A':
          show_nonprinting = true;
          show_ತುದಿಗಳು = true;
          show_tabs = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'E':
          show_ತುದಿಗಳು = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'T':
          show_tabs = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  ಆದರೆ (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_flags = -2;

  /* True if the output is a regular file.  */
  bool ಹೊರಹೋಗು_isreg = S_ISREG (ostat_buf.st_ಬಹುಲಕ) != 0;

  ಆದರೆ (! (ಸಂಖ್ಯೆ || show_ತುದಿಗಳು || squeeze_blank))
    {
      file_open_ಬಹುಲಕ |= O_ದ್ವಿಮಾನ;
      xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDOUT_FILENO, O_ದ್ವಿಮಾನ);
    }

  /* Main loop.  */

  infile = "-";
  ಪೂರ್ಣಾಂಕ argind = optind;
  bool ok = true;
  idx_t ಪುಟ_ಗಾತ್ರ = getpagesize ();
  ಅಕ್ಷರ *inbuf = ಖಾಲಿ;
  ಅಕ್ಷರ *outbuf = ಖಾಲಿ;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  ಮಾಡು
    {
      ಆದರೆ (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      ಆದರೆ (reading_stdin)
        {
          have_read_stdin = true;
          ಒಳಹರಿವು_desc = STDIN_FILENO;
          ಆದರೆ (file_open_ಬಹುಲಕ & O_ದ್ವಿಮಾನ)
            xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDIN_FILENO, O_ದ್ವಿಮಾನ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಒಳಹರಿವು_desc = open (infile, file_open_ಬಹುಲಕ);
          ಆದರೆ (ಒಳಹರಿವು_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              ಮುಂದುವರಿ;
            }
        }

      ರಚನೆ stat istat_buf;
      ಆದರೆ (fstat (ಒಳಹರಿವು_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          ಹೋಗು contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (ಒಳಹರಿವು_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      ಆದರೆ (! (S_ISFIFO (istat_buf.st_ಬಹುಲಕ) || S_ISSOCK (istat_buf.st_ಬಹುಲಕ)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t ಒಳ_pos = lseek (ಒಳಹರಿವು_desc, 0, SEEK_CUR);
          ಆದರೆ (0 <= ಒಳ_pos)
            {
              ಆದರೆ (ಹೊರಹೋಗು_flags < -1)
                ಹೊರಹೋಗು_flags = fcntl (STDOUT_FILENO, F_GETFL);
              ಪೂರ್ಣಾಂಕ whence = (0 <= ಹೊರಹೋಗು_flags && ಹೊರಹೋಗು_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              ಆದರೆ (ಒಳ_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  ಹೋಗು contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      ಆದರೆ (! (ಸಂಖ್ಯೆ || show_ತುದಿಗಳು || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          ಪೂರ್ಣಾಂಕ copy_cat_ಸ್ಥಿತಿ =
            ಹೊರಹೋಗು_isreg && S_ISREG (istat_buf.st_ಬಹುಲಕ) ? copy_cat () : 0;
          ಆದರೆ (copy_cat_ಸ್ಥಿತಿ != 0)
            ok &= 0 < copy_cat_ಸ್ಥಿತಿ;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              ಪೂರ್ಣಾಂಕ splice_cat_ಸ್ಥಿತಿ = ((usable_st_ಗಾತ್ರ (&istat_buf)
                                        && istat_buf.st_ಗಾತ್ರ <= 32768)
                                       ? 0 : splice_cat ());
              ಆದರೆ (splice_cat_ಸ್ಥಿತಿ != 0)
                ok &= 0 < splice_cat_ಸ್ಥಿತಿ;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_ಗಾತ್ರ (inbuf, &inbuf_alloc,
                                           ಪುಟ_ಗಾತ್ರ, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Allocate, with an extra byte for a newline sentinel.  */
          inbuf = ensure_buf_ಗಾತ್ರ (inbuf, &inbuf_alloc,
                                   ಪುಟ_ಗಾತ್ರ, insize + 1);

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
          ಆದರೆ (ckd_mul (&bufsize, insize, 4)
              || ckd_ಕೂಡಿಸು (&bufsize, bufsize, outsize)
              || ckd_ಕೂಡಿಸು (&bufsize, bufsize, ಸಾಲು_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_ಗಾತ್ರ (outbuf, &outbuf_alloc,
                                    ಪುಟ_ಗಾತ್ರ, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, ಸಂಖ್ಯೆ, ಸಂಖ್ಯೆ_nonblank, show_ತುದಿಗಳು,
                     squeeze_blank);
        }

    contin:
      ಆದರೆ (!reading_stdin && close (ಒಳಹರಿವು_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  ಆಗ (++argind < argc);

  ಆದರೆ (pending_cr)
    {
      ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ lint
  alignfree (outbuf);
  alignfree (inbuf);
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
