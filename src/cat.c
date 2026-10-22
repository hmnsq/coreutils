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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#எனில் HAVE_STROPTS_H
# include <stropts.h>
#endif
#அந்தர்பாவ <sys/ioctl.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "alignalloc.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "ioblksize.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "full-write.h"
#அந்தர்பாவ "isapipe.h"
#அந்தர்பாவ "splice.h"
#அந்தர்பாவ "unistd--.h"
#அந்தர்பாவ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cat"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
நிலையான எழுத்து மாறிலி *infile;

/* Descriptor on which input file is open.  */
நிலையான முழு உள்ளீடு_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#define வரி_COUNTER_BUF_LEN 20
நிலையான எழுத்து வரி_buf[வரி_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
நிலையான எழுத்து *வரி_இலக்கம்_print = வரி_buf + வரி_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
நிலையான எழுத்து *வரி_இலக்கம்_start = வரி_buf + வரி_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
நிலையான எழுத்து *வரி_இலக்கம்_end = வரி_buf + வரி_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
நிலையான முழு newlines2 = 0;

/* Whether there is a pending CR to process.  */
நிலையான bool pending_cr = false;

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
  -b, --எண்-nonblank    எண் nonempty output வரிகள், overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent to -vE\n\
"));
      oputs (_("\
  -E, --show-ends          display $ or ^M$ at end of each வரி\n\
"));
      oputs (_("\
  -n, --எண்             எண் all output வரிகள்\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated empty output வரிகள்\n\
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
  -v, --show-nonprinting   use ^ and M- notation, except க்கு LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard உள்ளீடு to standard output.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Compute the next line number.  */

நிலையான வெறுமை
next_வரி_இலக்கம் (வெறுமை)
{
  எழுத்து *endp = வரி_இலக்கம்_end;
  செய்
    {
      எனில் ((*endp)++ < '9')
        திரும்பு;
      *endp-- = '0';
    }
  போது (endp >= வரி_இலக்கம்_start);

  எனில் (வரி_இலக்கம்_start > வரி_buf)
    *--வரி_இலக்கம்_start = '1';
  இல்லையெனில்
    *வரி_buf = '>';
  எனில் (வரி_இலக்கம்_start < வரி_இலக்கம்_print)
    வரி_இலக்கம்_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

நிலையான bool
simple_cat (எழுத்து *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  போது (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = பட (உள்ளீடு_desc, buf, bufsize);
      எனில் (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          திரும்பு false;
        }

      /* End of this file?  */

      எனில் (n_read == 0)
        திரும்பு true;

      /* Write this block out.  */

      எனில் (full_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

நிலையான உள்வரி வெறுமை
write_pending (எழுத்து *outbuf, எழுத்து **bpout)
{
  idx_t n_write = *bpout - outbuf;
  எனில் (0 < n_write)
    {
      எனில் (full_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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

நிலையான bool
cat (எழுத்து *inbuf, idx_t insize, எழுத்து *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool எண், bool எண்_nonblank,
     bool show_ends, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  unsigned எழுத்து ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  முழு newlines = newlines2;

#ifdef FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#endif

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  எழுத்து *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  எழுத்து *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  எழுத்து *bpout = outbuf;

  போது (true)
    {
      செய்
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          எனில் (outbuf + outsize <= bpout)
            {
              எழுத்து *wp = outbuf;
              idx_t remaining_bytes;
              செய்
                {
                  எனில் (full_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  remaining_bytes = bpout - wp;
                }
              போது (outsize <= remaining_bytes);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, remaining_bytes);
              bpout = outbuf + remaining_bytes;
            }

          /* Is INBUF empty?  */

          எனில் (bpin > eob)
            {
              bool உள்ளீடு_pending = false;
#ifdef FIONREAD
              முழு n_to_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              எனில் (use_fionread
                  && ioctl (உள்ளீடு_desc, FIONREAD, &n_to_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  எனில் (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  இல்லையெனில்
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      திரும்பு false;
                    }
                }
              எனில் (n_to_read != 0)
                உள்ளீடு_pending = true;
#endif

              எனில் (!உள்ளீடு_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = பட (உள்ளீடு_desc, inbuf, insize);
              எனில் (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  திரும்பு false;
                }
              எனில் (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  திரும்பு true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          இல்லையெனில்
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              எனில் (++newlines > 0)
                {
                  எனில் (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      எனில் (squeeze_blank)
                        {
                          ch = *bpin++;
                          தொடர்;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  எனில் (எண் && !எண்_nonblank)
                    {
                      next_வரி_இலக்கம் ();
                      bpout = stpcpy (bpout, வரி_இலக்கம்_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              எனில் (show_ends)
                {
                  எனில் (pending_cr)
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
      போது (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      எனில் (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      எனில் (newlines >= 0 && எண்)
        {
          next_வரி_இலக்கம் ();
          bpout = stpcpy (bpout, வரி_இலக்கம்_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      எனில் (show_nonprinting)
        {
          போது (true)
            {
              எனில் (ch >= 32)
                {
                  எனில் (ch < 127)
                    *bpout++ = ch;
                  இல்லையெனில் எனில் (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  இல்லையெனில்
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      எனில் (ch >= 128 + 32)
                        {
                          எனில் (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          இல்லையெனில்
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      இல்லையெனில்
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              இல்லையெனில் எனில் (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              இல்லையெனில் எனில் (ch == '\n')
                {
                  newlines = -1;
                  நிறுத்து;
                }
              இல்லையெனில்
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      இல்லையெனில்
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          போது (true)
            {
              எனில் (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              இல்லையெனில் எனில் (ch != '\n')
                {
                  எனில் (ch == '\r' && *bpin == '\n' && show_ends)
                    {
                      எனில் (bpin == eob)
                        pending_cr = true;
                      இல்லையெனில்
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  இல்லையெனில்
                    *bpout++ = ch;
                }
              இல்லையெனில்
                {
                  newlines = -1;
                  நிறுத்து;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

நிலையான முழு
copy_cat (வெறுமை)
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

  க்கு (bool some_copied = false; ; some_copied = true)
    தேர்வு (copy_file_range (உள்ளீடு_desc, வெற்று, STDOUT_FILENO, வெற்று,
                             copy_max, 0))
      {
      நிலை 0:
        திரும்பு some_copied;

      நிலை -1:
        எனில் (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          திரும்பு 0;
        error (0, errno, "%s", quotef (infile));
        திரும்பு -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

நிலையான முழு
splice_cat (வெறுமை)
{
  bool some_copied = false;
  bool in_ok = true;
  bool out_ok = true;

#எனில் HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  நிலையான முழு pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  நிலையான idx_t pipefd_pipe_size = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  எனில் (pipefd[0] < 0)
    {
      எனில் (pipe (pipefd) < 0)
        திரும்பு false;
      pipefd_pipe_size = increase_pipe_size (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  நிலையான முழு stdout_is_pipe = -1;
  நிலையான idx_t stdout_pipe_size = 0;
  எனில் (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      எனில் (stdout_is_pipe)
        stdout_pipe_size = increase_pipe_size (STDOUT_FILENO);
    }

  idx_t pipe_size = MAX (pipefd_pipe_size, stdout_pipe_size);

  போது (true)
    {
      ssize_t bytes_read = splice (உள்ளீடு_desc, வெற்று, pipefd[1], வெற்று,
                                   pipe_size, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      in_ok = 0 <= bytes_read || ! some_copied;
      எனில் (bytes_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      எனில் (bytes_read <= 0)
        செல் done;
      /* We need to drain the intermediate pipe to standard output.  */
      போது (0 < bytes_read)
        {
          ssize_t bytes_written = splice (pipefd[0], வெற்று, STDOUT_FILENO, வெற்று,
                                          pipe_size, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          எனில் (bytes_written < 0)
            {
              எனில் (some_copied)
                out_ok = false;
              இல்லையெனில்
                {
                  எழுத்து buf[BUFSIZ];
                  போது (0 < bytes_read)
                    {
                      ssize_t மொத்தம் = MIN (bytes_read, அளவு buf);
                      ssize_t n_read = பட (pipefd[0], buf, மொத்தம்);
                      /* Failure not associated with in or out.  */
                      in_ok = out_ok = 0 <= n_read;
                      எனில் (n_read <= 0)
                        செல் done;
                      எனில் (full_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      bytes_read -= n_read;
                    }
                }
            }
          எனில் (bytes_written <= 0)
            செல் done;
          some_copied = true;
          bytes_read -= bytes_written;
        }
    }

 done:
  எனில் (! in_ok && ! out_ok)
    {
      /* Recreate the pipe on internal error.  */
      முழு saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_size = 0;
      error (0, errno, "%s", _("splice error"));
    }
  இல்லையெனில் எனில் (! in_ok)
    error (0, errno, "%s", quotef (infile));
  இல்லையெனில் எனில் (! out_ok)
    write_error ();
#endif

  திரும்பு (in_ok && out_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

நிலையான எழுத்து *
ensure_buf_size (எழுத்து *buf, idx_t *buf_alloc, idx_t alignment, idx_t size)
{
  affirm (buf != வெற்று || *buf_alloc < size);

  எனில் (*buf_alloc < size)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, size);
      *buf_alloc = size;
    }

  திரும்பு buf;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  கட்டமைப்பு stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool எண் = false;
  bool எண்_nonblank = false;
  bool squeeze_blank = false;
  bool show_ends = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  முழு file_open_mode = O_RDONLY;

  நிலையான கட்டமைப்பு option மாறிலி long_options[] =
  {
    {"number-nonblank", no_argument, வெற்று, 'b'},
    {"number", no_argument, வெற்று, 'n'},
    {"squeeze-blank", no_argument, வெற்று, 's'},
    {"show-nonprinting", no_argument, வெற்று, 'v'},
    {"show-ends", no_argument, வெற்று, 'E'},
    {"show-tabs", no_argument, வெற்று, 'T'},
    {"show-all", no_argument, வெற்று, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {வெற்று, 0, வெற்று, 0}
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

  முழு c;
  போது ((c = getopt_long (argc, argv, "benstuvAET", long_options, வெற்று))
         != -1)
    {
      தேர்வு (c)
        {
        நிலை 'b':
          எண் = true;
          எண்_nonblank = true;
          நிறுத்து;

        நிலை 'e':
          show_ends = true;
          show_nonprinting = true;
          நிறுத்து;

        நிலை 'n':
          எண் = true;
          நிறுத்து;

        நிலை 's':
          squeeze_blank = true;
          நிறுத்து;

        நிலை 't':
          show_tabs = true;
          show_nonprinting = true;
          நிறுத்து;

        நிலை 'u':
          /* We provide the -u feature unconditionally.  */
          நிறுத்து;

        நிலை 'v':
          show_nonprinting = true;
          நிறுத்து;

        நிலை 'A':
          show_nonprinting = true;
          show_ends = true;
          show_tabs = true;
          நிறுத்து;

        நிலை 'E':
          show_ends = true;
          நிறுத்து;

        நிலை 'T':
          show_tabs = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  எனில் (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  முழு out_flags = -2;

  /* True if the output is a regular file.  */
  bool out_isreg = S_ISREG (ostat_buf.st_mode) != 0;

  எனில் (! (எண் || show_ends || squeeze_blank))
    {
      file_open_mode |= O_இருமம்;
      xset_இருமம்_mode (STDOUT_FILENO, O_இருமம்);
    }

  /* Main loop.  */

  infile = "-";
  முழு argind = optind;
  bool ok = true;
  idx_t page_size = getpagesize ();
  எழுத்து *inbuf = வெற்று;
  எழுத்து *outbuf = வெற்று;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  செய்
    {
      எனில் (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      எனில் (reading_stdin)
        {
          have_read_stdin = true;
          உள்ளீடு_desc = STDIN_FILENO;
          எனில் (file_open_mode & O_இருமம்)
            xset_இருமம்_mode (STDIN_FILENO, O_இருமம்);
        }
      இல்லையெனில்
        {
          உள்ளீடு_desc = open (infile, file_open_mode);
          எனில் (உள்ளீடு_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              தொடர்;
            }
        }

      கட்டமைப்பு stat istat_buf;
      எனில் (fstat (உள்ளீடு_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          செல் contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (உள்ளீடு_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      எனில் (! (S_ISFIFO (istat_buf.st_mode) || S_ISSOCK (istat_buf.st_mode)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t in_pos = lseek (உள்ளீடு_desc, 0, SEEK_CUR);
          எனில் (0 <= in_pos)
            {
              எனில் (out_flags < -1)
                out_flags = fcntl (STDOUT_FILENO, F_GETFL);
              முழு whence = (0 <= out_flags && out_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              எனில் (in_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  செல் contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      எனில் (! (எண் || show_ends || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          முழு copy_cat_status =
            out_isreg && S_ISREG (istat_buf.st_mode) ? copy_cat () : 0;
          எனில் (copy_cat_status != 0)
            ok &= 0 < copy_cat_status;
          இல்லையெனில்
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              முழு splice_cat_status = ((usable_st_size (&istat_buf)
                                        && istat_buf.st_size <= 32768)
                                       ? 0 : splice_cat ());
              எனில் (splice_cat_status != 0)
                ok &= 0 < splice_cat_status;
              இல்லையெனில்
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_size (inbuf, &inbuf_alloc,
                                           page_size, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      இல்லையெனில்
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
          எனில் (ckd_mul (&bufsize, insize, 4)
              || ckd_add (&bufsize, bufsize, outsize)
              || ckd_add (&bufsize, bufsize, வரி_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_size (outbuf, &outbuf_alloc,
                                    page_size, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, எண், எண்_nonblank, show_ends,
                     squeeze_blank);
        }

    contin:
      எனில் (!reading_stdin && close (உள்ளீடு_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  போது (++argind < argc);

  எனில் (pending_cr)
    {
      எனில் (full_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#ifdef lint
  alignfree (outbuf);
  alignfree (inbuf);
#endif

  எனில் (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
