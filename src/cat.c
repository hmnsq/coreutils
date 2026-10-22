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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#જો HAVE_STROPTS_H
# include <stropts.h>
#endif
#અન્તર્ભાવ <sys/ioctl.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "alignalloc.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "ioblksize.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "full-write.h"
#અન્તર્ભાવ "isapipe.h"
#અન્તર્ભાવ "splice.h"
#અન્તર્ભાવ "unistd--.h"
#અન્તર્ભાવ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cat"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
સ્થિર અક્ષર અચલ *infile;

/* Descriptor on which input file is open.  */
સ્થિર પૂર્ણાંક આગત_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#define લીટી_COUNTER_BUF_LEN 20
સ્થિર અક્ષર લીટી_buf[લીટી_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
સ્થિર અક્ષર *લીટી_અંક_print = લીટી_buf + લીટી_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
સ્થિર અક્ષર *લીટી_અંક_start = લીટી_buf + લીટી_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
સ્થિર અક્ષર *લીટી_અંક_end = લીટી_buf + લીટી_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
સ્થિર પૂર્ણાંક newlines2 = 0;

/* Whether there is a pending CR to process.  */
સ્થિર bool pending_cr = false;

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
  -b, --સંખ્યા-nonblank    સંખ્યા nonempty output લીટીઓ, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent to -vE\n\
"));
      oputs (_("\
  -E, --show-ends          display $ or ^M$ at end of each લીટી\n\
"));
      oputs (_("\
  -n, --સંખ્યા             સંખ્યા all output લીટીઓ\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated empty output લીટીઓ\n\
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
  -v, --show-nonprinting   use ^ and M- notation, except માટે LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard આગત to standard output.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Compute the next line number.  */

સ્થિર શૂન્ય
next_લીટી_અંક (શૂન્ય)
{
  અક્ષર *endp = લીટી_અંક_end;
  કરો
    {
      જો ((*endp)++ < '9')
        પાછા;
      *endp-- = '0';
    }
  જ્યારે (endp >= લીટી_અંક_start);

  જો (લીટી_અંક_start > લીટી_buf)
    *--લીટી_અંક_start = '1';
  નહીંતર
    *લીટી_buf = '>';
  જો (લીટી_અંક_start < લીટી_અંક_print)
    લીટી_અંક_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

સ્થિર bool
simple_cat (અક્ષર *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  જ્યારે (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = પઠ (આગત_desc, buf, bufsize);
      જો (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          પાછા false;
        }

      /* End of this file?  */

      જો (n_read == 0)
        પાછા true;

      /* Write this block out.  */

      જો (full_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

સ્થિર અંતઃરેખ શૂન્ય
write_pending (અક્ષર *outbuf, અક્ષર **bpout)
{
  idx_t n_write = *bpout - outbuf;
  જો (0 < n_write)
    {
      જો (full_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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

સ્થિર bool
cat (અક્ષર *inbuf, idx_t insize, અક્ષર *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool સંખ્યા, bool સંખ્યા_nonblank,
     bool show_ends, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  unsigned અક્ષર ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  પૂર્ણાંક newlines = newlines2;

#ifdef FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#endif

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  અક્ષર *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  અક્ષર *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  અક્ષર *bpout = outbuf;

  જ્યારે (true)
    {
      કરો
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          જો (outbuf + outsize <= bpout)
            {
              અક્ષર *wp = outbuf;
              idx_t remaining_bytes;
              કરો
                {
                  જો (full_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  remaining_bytes = bpout - wp;
                }
              જ્યારે (outsize <= remaining_bytes);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, remaining_bytes);
              bpout = outbuf + remaining_bytes;
            }

          /* Is INBUF empty?  */

          જો (bpin > eob)
            {
              bool આગત_pending = false;
#ifdef FIONREAD
              પૂર્ણાંક n_to_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              જો (use_fionread
                  && ioctl (આગત_desc, FIONREAD, &n_to_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  જો (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  નહીંતર
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      પાછા false;
                    }
                }
              જો (n_to_read != 0)
                આગત_pending = true;
#endif

              જો (!આગત_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = પઠ (આગત_desc, inbuf, insize);
              જો (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  પાછા false;
                }
              જો (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  પાછા true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          નહીંતર
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              જો (++newlines > 0)
                {
                  જો (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      જો (squeeze_blank)
                        {
                          ch = *bpin++;
                          ચાલુ;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  જો (સંખ્યા && !સંખ્યા_nonblank)
                    {
                      next_લીટી_અંક ();
                      bpout = stpcpy (bpout, લીટી_અંક_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              જો (show_ends)
                {
                  જો (pending_cr)
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
      જ્યારે (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      જો (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      જો (newlines >= 0 && સંખ્યા)
        {
          next_લીટી_અંક ();
          bpout = stpcpy (bpout, લીટી_અંક_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      જો (show_nonprinting)
        {
          જ્યારે (true)
            {
              જો (ch >= 32)
                {
                  જો (ch < 127)
                    *bpout++ = ch;
                  નહીંતર જો (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  નહીંતર
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      જો (ch >= 128 + 32)
                        {
                          જો (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          નહીંતર
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      નહીંતર
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              નહીંતર જો (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              નહીંતર જો (ch == '\n')
                {
                  newlines = -1;
                  તોડો;
                }
              નહીંતર
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      નહીંતર
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          જ્યારે (true)
            {
              જો (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              નહીંતર જો (ch != '\n')
                {
                  જો (ch == '\r' && *bpin == '\n' && show_ends)
                    {
                      જો (bpin == eob)
                        pending_cr = true;
                      નહીંતર
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  નહીંતર
                    *bpout++ = ch;
                }
              નહીંતર
                {
                  newlines = -1;
                  તોડો;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

સ્થિર પૂર્ણાંક
copy_cat (શૂન્ય)
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

  માટે (bool some_copied = false; ; some_copied = true)
    પસંદગી (copy_file_range (આગત_desc, ખાલી, STDOUT_FILENO, ખાલી,
                             copy_max, 0))
      {
      સ્થિતિ 0:
        પાછા some_copied;

      સ્થિતિ -1:
        જો (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          પાછા 0;
        error (0, errno, "%s", quotef (infile));
        પાછા -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

સ્થિર પૂર્ણાંક
splice_cat (શૂન્ય)
{
  bool some_copied = false;
  bool in_ok = true;
  bool out_ok = true;

#જો HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  સ્થિર પૂર્ણાંક pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  સ્થિર idx_t pipefd_pipe_size = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  જો (pipefd[0] < 0)
    {
      જો (pipe (pipefd) < 0)
        પાછા false;
      pipefd_pipe_size = increase_pipe_size (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  સ્થિર પૂર્ણાંક stdout_is_pipe = -1;
  સ્થિર idx_t stdout_pipe_size = 0;
  જો (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      જો (stdout_is_pipe)
        stdout_pipe_size = increase_pipe_size (STDOUT_FILENO);
    }

  idx_t pipe_size = MAX (pipefd_pipe_size, stdout_pipe_size);

  જ્યારે (true)
    {
      ssize_t bytes_read = splice (આગત_desc, ખાલી, pipefd[1], ખાલી,
                                   pipe_size, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      in_ok = 0 <= bytes_read || ! some_copied;
      જો (bytes_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      જો (bytes_read <= 0)
        જાઓ done;
      /* We need to drain the intermediate pipe to standard output.  */
      જ્યારે (0 < bytes_read)
        {
          ssize_t bytes_written = splice (pipefd[0], ખાલી, STDOUT_FILENO, ખાલી,
                                          pipe_size, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          જો (bytes_written < 0)
            {
              જો (some_copied)
                out_ok = false;
              નહીંતર
                {
                  અક્ષર buf[BUFSIZ];
                  જ્યારે (0 < bytes_read)
                    {
                      ssize_t ગણતરી = MIN (bytes_read, કદ buf);
                      ssize_t n_read = પઠ (pipefd[0], buf, ગણતરી);
                      /* Failure not associated with in or out.  */
                      in_ok = out_ok = 0 <= n_read;
                      જો (n_read <= 0)
                        જાઓ done;
                      જો (full_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      bytes_read -= n_read;
                    }
                }
            }
          જો (bytes_written <= 0)
            જાઓ done;
          some_copied = true;
          bytes_read -= bytes_written;
        }
    }

 done:
  જો (! in_ok && ! out_ok)
    {
      /* Recreate the pipe on internal error.  */
      પૂર્ણાંક saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_size = 0;
      error (0, errno, "%s", _("splice error"));
    }
  નહીંતર જો (! in_ok)
    error (0, errno, "%s", quotef (infile));
  નહીંતર જો (! out_ok)
    write_error ();
#endif

  પાછા (in_ok && out_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

સ્થિર અક્ષર *
ensure_buf_size (અક્ષર *buf, idx_t *buf_alloc, idx_t alignment, idx_t size)
{
  affirm (buf != ખાલી || *buf_alloc < size);

  જો (*buf_alloc < size)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, size);
      *buf_alloc = size;
    }

  પાછા buf;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  સંરચના stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool સંખ્યા = false;
  bool સંખ્યા_nonblank = false;
  bool squeeze_blank = false;
  bool show_ends = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  પૂર્ણાંક file_open_mode = O_RDONLY;

  સ્થિર સંરચના option અચલ long_options[] =
  {
    {"number-nonblank", no_argument, ખાલી, 'b'},
    {"number", no_argument, ખાલી, 'n'},
    {"squeeze-blank", no_argument, ખાલી, 's'},
    {"show-nonprinting", no_argument, ખાલી, 'v'},
    {"show-ends", no_argument, ખાલી, 'E'},
    {"show-tabs", no_argument, ખાલી, 'T'},
    {"show-all", no_argument, ખાલી, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {ખાલી, 0, ખાલી, 0}
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

  પૂર્ણાંક c;
  જ્યારે ((c = getopt_long (argc, argv, "benstuvAET", long_options, ખાલી))
         != -1)
    {
      પસંદગી (c)
        {
        સ્થિતિ 'b':
          સંખ્યા = true;
          સંખ્યા_nonblank = true;
          તોડો;

        સ્થિતિ 'e':
          show_ends = true;
          show_nonprinting = true;
          તોડો;

        સ્થિતિ 'n':
          સંખ્યા = true;
          તોડો;

        સ્થિતિ 's':
          squeeze_blank = true;
          તોડો;

        સ્થિતિ 't':
          show_tabs = true;
          show_nonprinting = true;
          તોડો;

        સ્થિતિ 'u':
          /* We provide the -u feature unconditionally.  */
          તોડો;

        સ્થિતિ 'v':
          show_nonprinting = true;
          તોડો;

        સ્થિતિ 'A':
          show_nonprinting = true;
          show_ends = true;
          show_tabs = true;
          તોડો;

        સ્થિતિ 'E':
          show_ends = true;
          તોડો;

        સ્થિતિ 'T':
          show_tabs = true;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  જો (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  પૂર્ણાંક out_flags = -2;

  /* True if the output is a regular file.  */
  bool out_isreg = S_ISREG (ostat_buf.st_mode) != 0;

  જો (! (સંખ્યા || show_ends || squeeze_blank))
    {
      file_open_mode |= O_દ્વિઅંકી;
      xset_દ્વિઅંકી_mode (STDOUT_FILENO, O_દ્વિઅંકી);
    }

  /* Main loop.  */

  infile = "-";
  પૂર્ણાંક argind = optind;
  bool ok = true;
  idx_t page_size = getpagesize ();
  અક્ષર *inbuf = ખાલી;
  અક્ષર *outbuf = ખાલી;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  કરો
    {
      જો (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      જો (reading_stdin)
        {
          have_read_stdin = true;
          આગત_desc = STDIN_FILENO;
          જો (file_open_mode & O_દ્વિઅંકી)
            xset_દ્વિઅંકી_mode (STDIN_FILENO, O_દ્વિઅંકી);
        }
      નહીંતર
        {
          આગત_desc = open (infile, file_open_mode);
          જો (આગત_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              ચાલુ;
            }
        }

      સંરચના stat istat_buf;
      જો (fstat (આગત_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          જાઓ contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (આગત_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      જો (! (S_ISFIFO (istat_buf.st_mode) || S_ISSOCK (istat_buf.st_mode)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t in_pos = lseek (આગત_desc, 0, SEEK_CUR);
          જો (0 <= in_pos)
            {
              જો (out_flags < -1)
                out_flags = fcntl (STDOUT_FILENO, F_GETFL);
              પૂર્ણાંક whence = (0 <= out_flags && out_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              જો (in_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  જાઓ contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      જો (! (સંખ્યા || show_ends || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          પૂર્ણાંક copy_cat_status =
            out_isreg && S_ISREG (istat_buf.st_mode) ? copy_cat () : 0;
          જો (copy_cat_status != 0)
            ok &= 0 < copy_cat_status;
          નહીંતર
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              પૂર્ણાંક splice_cat_status = ((usable_st_size (&istat_buf)
                                        && istat_buf.st_size <= 32768)
                                       ? 0 : splice_cat ());
              જો (splice_cat_status != 0)
                ok &= 0 < splice_cat_status;
              નહીંતર
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_size (inbuf, &inbuf_alloc,
                                           page_size, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      નહીંતર
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
          જો (ckd_mul (&bufsize, insize, 4)
              || ckd_add (&bufsize, bufsize, outsize)
              || ckd_add (&bufsize, bufsize, લીટી_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_size (outbuf, &outbuf_alloc,
                                    page_size, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, સંખ્યા, સંખ્યા_nonblank, show_ends,
                     squeeze_blank);
        }

    contin:
      જો (!reading_stdin && close (આગત_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  જ્યારે (++argind < argc);

  જો (pending_cr)
    {
      જો (full_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#ifdef lint
  alignfree (outbuf);
  alignfree (inbuf);
#endif

  જો (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
