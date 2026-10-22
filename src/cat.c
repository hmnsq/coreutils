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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#जइ HAVE_STROPTS_H
# include <stropts.h>
#endif
#अन्तर्भाव <sys/ioctl.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "alignalloc.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "full-write.h"
#अन्तर्भाव "isapipe.h"
#अन्तर्भाव "splice.h"
#अन्तर्भाव "unistd--.h"
#अन्तर्भाव "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cat"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
स्थिर वर्ण ध्रुव *infile;

/* Descriptor on which input file is open.  */
स्थिर पूर्णाङ्क निवेशः_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#define पङ्क्तिः_COUNTER_BUF_LEN 20
स्थिर वर्ण पङ्क्तिः_buf[पङ्क्तिः_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
स्थिर वर्ण *पङ्क्तिः_अङ्कः_print = पङ्क्तिः_buf + पङ्क्तिः_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
स्थिर वर्ण *पङ्क्तिः_अङ्कः_start = पङ्क्तिः_buf + पङ्क्तिः_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
स्थिर वर्ण *पङ्क्तिः_अङ्कः_end = पङ्क्तिः_buf + पङ्क्तिः_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
स्थिर पूर्णाङ्क newlines2 = 0;

/* Whether there is a pending CR to process.  */
स्थिर bool pending_cr = false;

शून्य
usage (पूर्णाङ्क status)
{
  जइ (status != EXIT_SUCCESS)
    emit_try_help ();
  अण्णहा
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
  -b, --सङ्ख्या-nonblank    सङ्ख्या nonempty output पङ्क्तयः, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent to -vE\n\
"));
      oputs (_("\
  -E, --show-ends          display $ or ^M$ at end of each पङ्क्तिः\n\
"));
      oputs (_("\
  -n, --सङ्ख्या             सङ्ख्या all output पङ्क्तयः\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated empty output पङ्क्तयः\n\
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
  -v, --show-nonprinting   use ^ and M- notation, except प्रति LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard निवेशः to standard output.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Compute the next line number.  */

स्थिर शून्य
next_पङ्क्तिः_अङ्कः (शून्य)
{
  वर्ण *endp = पङ्क्तिः_अङ्कः_end;
  करेहि
    {
      जइ ((*endp)++ < '9')
        प्रत्यागम;
      *endp-- = '0';
    }
  जाव (endp >= पङ्क्तिः_अङ्कः_start);

  जइ (पङ्क्तिः_अङ्कः_start > पङ्क्तिः_buf)
    *--पङ्क्तिः_अङ्कः_start = '1';
  अण्णहा
    *पङ्क्तिः_buf = '>';
  जइ (पङ्क्तिः_अङ्कः_start < पङ्क्तिः_अङ्कः_print)
    पङ्क्तिः_अङ्कः_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

स्थिर bool
simple_cat (वर्ण *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  जाव (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = पठ (निवेशः_desc, buf, bufsize);
      जइ (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          प्रत्यागम false;
        }

      /* End of this file?  */

      जइ (n_read == 0)
        प्रत्यागम true;

      /* Write this block out.  */

      जइ (full_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

स्थिर अन्तःस्थ शून्य
write_pending (वर्ण *outbuf, वर्ण **bpout)
{
  idx_t n_write = *bpout - outbuf;
  जइ (0 < n_write)
    {
      जइ (full_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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

स्थिर bool
cat (वर्ण *inbuf, idx_t insize, वर्ण *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool सङ्ख्या, bool सङ्ख्या_nonblank,
     bool show_ends, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  unsigned वर्ण ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  पूर्णाङ्क newlines = newlines2;

#ifdef FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#endif

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  वर्ण *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  वर्ण *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  वर्ण *bpout = outbuf;

  जाव (true)
    {
      करेहि
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          जइ (outbuf + outsize <= bpout)
            {
              वर्ण *wp = outbuf;
              idx_t remaining_bytes;
              करेहि
                {
                  जइ (full_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  remaining_bytes = bpout - wp;
                }
              जाव (outsize <= remaining_bytes);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, remaining_bytes);
              bpout = outbuf + remaining_bytes;
            }

          /* Is INBUF empty?  */

          जइ (bpin > eob)
            {
              bool निवेशः_pending = false;
#ifdef FIONREAD
              पूर्णाङ्क n_to_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              जइ (use_fionread
                  && ioctl (निवेशः_desc, FIONREAD, &n_to_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  जइ (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  अण्णहा
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      प्रत्यागम false;
                    }
                }
              जइ (n_to_read != 0)
                निवेशः_pending = true;
#endif

              जइ (!निवेशः_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = पठ (निवेशः_desc, inbuf, insize);
              जइ (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  प्रत्यागम false;
                }
              जइ (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  प्रत्यागम true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          अण्णहा
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              जइ (++newlines > 0)
                {
                  जइ (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      जइ (squeeze_blank)
                        {
                          ch = *bpin++;
                          अनुवर्त;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  जइ (सङ्ख्या && !सङ्ख्या_nonblank)
                    {
                      next_पङ्क्तिः_अङ्कः ();
                      bpout = stpcpy (bpout, पङ्क्तिः_अङ्कः_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              जइ (show_ends)
                {
                  जइ (pending_cr)
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
      जाव (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      जइ (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      जइ (newlines >= 0 && सङ्ख्या)
        {
          next_पङ्क्तिः_अङ्कः ();
          bpout = stpcpy (bpout, पङ्क्तिः_अङ्कः_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      जइ (show_nonprinting)
        {
          जाव (true)
            {
              जइ (ch >= 32)
                {
                  जइ (ch < 127)
                    *bpout++ = ch;
                  अण्णहा जइ (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  अण्णहा
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      जइ (ch >= 128 + 32)
                        {
                          जइ (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          अण्णहा
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      अण्णहा
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              अण्णहा जइ (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              अण्णहा जइ (ch == '\n')
                {
                  newlines = -1;
                  भङ्ग;
                }
              अण्णहा
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      अण्णहा
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          जाव (true)
            {
              जइ (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              अण्णहा जइ (ch != '\n')
                {
                  जइ (ch == '\r' && *bpin == '\n' && show_ends)
                    {
                      जइ (bpin == eob)
                        pending_cr = true;
                      अण्णहा
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  अण्णहा
                    *bpout++ = ch;
                }
              अण्णहा
                {
                  newlines = -1;
                  भङ्ग;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

स्थिर पूर्णाङ्क
copy_cat (शून्य)
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

  प्रति (bool some_copied = false; ; some_copied = true)
    विकल्प (copy_file_range (निवेशः_desc, रिक्त, STDOUT_FILENO, रिक्त,
                             copy_max, 0))
      {
      पक्ष 0:
        प्रत्यागम some_copied;

      पक्ष -1:
        जइ (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          प्रत्यागम 0;
        error (0, errno, "%s", quotef (infile));
        प्रत्यागम -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

स्थिर पूर्णाङ्क
splice_cat (शून्य)
{
  bool some_copied = false;
  bool in_ok = true;
  bool out_ok = true;

#जइ HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  स्थिर पूर्णाङ्क pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  स्थिर idx_t pipefd_pipe_size = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  जइ (pipefd[0] < 0)
    {
      जइ (pipe (pipefd) < 0)
        प्रत्यागम false;
      pipefd_pipe_size = increase_pipe_size (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  स्थिर पूर्णाङ्क stdout_is_pipe = -1;
  स्थिर idx_t stdout_pipe_size = 0;
  जइ (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      जइ (stdout_is_pipe)
        stdout_pipe_size = increase_pipe_size (STDOUT_FILENO);
    }

  idx_t pipe_size = MAX (pipefd_pipe_size, stdout_pipe_size);

  जाव (true)
    {
      ssize_t bytes_read = splice (निवेशः_desc, रिक्त, pipefd[1], रिक्त,
                                   pipe_size, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      in_ok = 0 <= bytes_read || ! some_copied;
      जइ (bytes_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      जइ (bytes_read <= 0)
        गच्छ done;
      /* We need to drain the intermediate pipe to standard output.  */
      जाव (0 < bytes_read)
        {
          ssize_t bytes_written = splice (pipefd[0], रिक्त, STDOUT_FILENO, रिक्त,
                                          pipe_size, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          जइ (bytes_written < 0)
            {
              जइ (some_copied)
                out_ok = false;
              अण्णहा
                {
                  वर्ण buf[BUFSIZ];
                  जाव (0 < bytes_read)
                    {
                      ssize_t गणनम् = MIN (bytes_read, परिमाण buf);
                      ssize_t n_read = पठ (pipefd[0], buf, गणनम्);
                      /* Failure not associated with in or out.  */
                      in_ok = out_ok = 0 <= n_read;
                      जइ (n_read <= 0)
                        गच्छ done;
                      जइ (full_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      bytes_read -= n_read;
                    }
                }
            }
          जइ (bytes_written <= 0)
            गच्छ done;
          some_copied = true;
          bytes_read -= bytes_written;
        }
    }

 done:
  जइ (! in_ok && ! out_ok)
    {
      /* Recreate the pipe on internal error.  */
      पूर्णाङ्क saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_size = 0;
      error (0, errno, "%s", _("splice error"));
    }
  अण्णहा जइ (! in_ok)
    error (0, errno, "%s", quotef (infile));
  अण्णहा जइ (! out_ok)
    write_error ();
#endif

  प्रत्यागम (in_ok && out_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

स्थिर वर्ण *
ensure_buf_size (वर्ण *buf, idx_t *buf_alloc, idx_t alignment, idx_t size)
{
  affirm (buf != रिक्त || *buf_alloc < size);

  जइ (*buf_alloc < size)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, size);
      *buf_alloc = size;
    }

  प्रत्यागम buf;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  संरचना stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool सङ्ख्या = false;
  bool सङ्ख्या_nonblank = false;
  bool squeeze_blank = false;
  bool show_ends = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  पूर्णाङ्क file_open_mode = O_RDONLY;

  स्थिर संरचना option ध्रुव long_options[] =
  {
    {"number-nonblank", no_argument, रिक्त, 'b'},
    {"number", no_argument, रिक्त, 'n'},
    {"squeeze-blank", no_argument, रिक्त, 's'},
    {"show-nonprinting", no_argument, रिक्त, 'v'},
    {"show-ends", no_argument, रिक्त, 'E'},
    {"show-tabs", no_argument, रिक्त, 'T'},
    {"show-all", no_argument, रिक्त, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {रिक्त, 0, रिक्त, 0}
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

  पूर्णाङ्क c;
  जाव ((c = getopt_long (argc, argv, "benstuvAET", long_options, रिक्त))
         != -1)
    {
      विकल्प (c)
        {
        पक्ष 'b':
          सङ्ख्या = true;
          सङ्ख्या_nonblank = true;
          भङ्ग;

        पक्ष 'e':
          show_ends = true;
          show_nonprinting = true;
          भङ्ग;

        पक्ष 'n':
          सङ्ख्या = true;
          भङ्ग;

        पक्ष 's':
          squeeze_blank = true;
          भङ्ग;

        पक्ष 't':
          show_tabs = true;
          show_nonprinting = true;
          भङ्ग;

        पक्ष 'u':
          /* We provide the -u feature unconditionally.  */
          भङ्ग;

        पक्ष 'v':
          show_nonprinting = true;
          भङ्ग;

        पक्ष 'A':
          show_nonprinting = true;
          show_ends = true;
          show_tabs = true;
          भङ्ग;

        पक्ष 'E':
          show_ends = true;
          भङ्ग;

        पक्ष 'T':
          show_tabs = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  जइ (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  पूर्णाङ्क out_flags = -2;

  /* True if the output is a regular file.  */
  bool out_isreg = S_ISREG (ostat_buf.st_mode) != 0;

  जइ (! (सङ्ख्या || show_ends || squeeze_blank))
    {
      file_open_mode |= O_द्विमान;
      xset_द्विमान_mode (STDOUT_FILENO, O_द्विमान);
    }

  /* Main loop.  */

  infile = "-";
  पूर्णाङ्क argind = optind;
  bool ok = true;
  idx_t page_size = getpagesize ();
  वर्ण *inbuf = रिक्त;
  वर्ण *outbuf = रिक्त;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  करेहि
    {
      जइ (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      जइ (reading_stdin)
        {
          have_read_stdin = true;
          निवेशः_desc = STDIN_FILENO;
          जइ (file_open_mode & O_द्विमान)
            xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
        }
      अण्णहा
        {
          निवेशः_desc = open (infile, file_open_mode);
          जइ (निवेशः_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              अनुवर्त;
            }
        }

      संरचना stat istat_buf;
      जइ (fstat (निवेशः_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          गच्छ contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (निवेशः_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      जइ (! (S_ISFIFO (istat_buf.st_mode) || S_ISSOCK (istat_buf.st_mode)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t in_pos = lseek (निवेशः_desc, 0, SEEK_CUR);
          जइ (0 <= in_pos)
            {
              जइ (out_flags < -1)
                out_flags = fcntl (STDOUT_FILENO, F_GETFL);
              पूर्णाङ्क whence = (0 <= out_flags && out_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              जइ (in_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  गच्छ contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      जइ (! (सङ्ख्या || show_ends || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          पूर्णाङ्क copy_cat_status =
            out_isreg && S_ISREG (istat_buf.st_mode) ? copy_cat () : 0;
          जइ (copy_cat_status != 0)
            ok &= 0 < copy_cat_status;
          अण्णहा
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              पूर्णाङ्क splice_cat_status = ((usable_st_size (&istat_buf)
                                        && istat_buf.st_size <= 32768)
                                       ? 0 : splice_cat ());
              जइ (splice_cat_status != 0)
                ok &= 0 < splice_cat_status;
              अण्णहा
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_size (inbuf, &inbuf_alloc,
                                           page_size, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      अण्णहा
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
          जइ (ckd_mul (&bufsize, insize, 4)
              || ckd_add (&bufsize, bufsize, outsize)
              || ckd_add (&bufsize, bufsize, पङ्क्तिः_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_size (outbuf, &outbuf_alloc,
                                    page_size, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, सङ्ख्या, सङ्ख्या_nonblank, show_ends,
                     squeeze_blank);
        }

    contin:
      जइ (!reading_stdin && close (निवेशः_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  जाव (++argind < argc);

  जइ (pending_cr)
    {
      जइ (full_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#ifdef lint
  alignfree (outbuf);
  alignfree (inbuf);
#endif

  जइ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
