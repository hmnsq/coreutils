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

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#اگر HAVE_STROPTS_H
# include <stropts.h>
#endif
#شامل <sys/ioctl.h>

#شامل "system.h"
#شامل "alignalloc.h"
#شامل "assure.h"
#شامل "ioblksize.h"
#شامل "fadvise.h"
#شامل "full-write.h"
#شامل "isapipe.h"
#شامل "splice.h"
#شامل "unistd--.h"
#شامل "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cat"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
ایستا نویسه ثابت *infile;

/* Descriptor on which input file is open.  */
ایستا صحیح درون_داد_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#define سطر_COUNTER_BUF_LEN 20
ایستا نویسه سطر_buf[سطر_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
ایستا نویسه *سطر_رقم_print = سطر_buf + سطر_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
ایستا نویسه *سطر_رقم_start = سطر_buf + سطر_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
ایستا نویسه *سطر_رقم_end = سطر_buf + سطر_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
ایستا صحیح newlines2 = 0;

/* Whether there is a pending CR to process.  */
ایستا bool pending_cr = false;

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
  -b, --عدد-nonblank    عدد nonempty output سطرها, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent to -vE\n\
"));
      oputs (_("\
  -E, --show-ends          display $ or ^M$ at end of each سطر\n\
"));
      oputs (_("\
  -n, --عدد             عدد all output سطرها\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated empty output سطرها\n\
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
  -v, --show-nonprinting   use ^ and M- notation, except برای LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard درون_داد to standard output.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Compute the next line number.  */

ایستا تهی
next_سطر_رقم (تهی)
{
  نویسه *endp = سطر_رقم_end;
  انجامبده
    {
      اگر ((*endp)++ < '9')
        بازگشت;
      *endp-- = '0';
    }
  وقتی (endp >= سطر_رقم_start);

  اگر (سطر_رقم_start > سطر_buf)
    *--سطر_رقم_start = '1';
  وگرنه
    *سطر_buf = '>';
  اگر (سطر_رقم_start < سطر_رقم_print)
    سطر_رقم_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

ایستا bool
simple_cat (نویسه *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  وقتی (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = اقرأ (درون_داد_desc, buf, bufsize);
      اگر (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          بازگشت false;
        }

      /* End of this file?  */

      اگر (n_read == 0)
        بازگشت true;

      /* Write this block out.  */

      اگر (full_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

ایستا درونخطی تهی
write_pending (نویسه *outbuf, نویسه **bpout)
{
  idx_t n_write = *bpout - outbuf;
  اگر (0 < n_write)
    {
      اگر (full_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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

ایستا bool
cat (نویسه *inbuf, idx_t insize, نویسه *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool عدد, bool عدد_nonblank,
     bool show_ends, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  unsigned نویسه ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  صحیح newlines = newlines2;

#ifdef FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#endif

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  نویسه *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  نویسه *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  نویسه *bpout = outbuf;

  وقتی (true)
    {
      انجامبده
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          اگر (outbuf + outsize <= bpout)
            {
              نویسه *wp = outbuf;
              idx_t remaining_bytes;
              انجامبده
                {
                  اگر (full_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  remaining_bytes = bpout - wp;
                }
              وقتی (outsize <= remaining_bytes);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, remaining_bytes);
              bpout = outbuf + remaining_bytes;
            }

          /* Is INBUF empty?  */

          اگر (bpin > eob)
            {
              bool درون_داد_pending = false;
#ifdef FIONREAD
              صحیح n_to_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              اگر (use_fionread
                  && ioctl (درون_داد_desc, FIONREAD, &n_to_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  اگر (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  وگرنه
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      بازگشت false;
                    }
                }
              اگر (n_to_read != 0)
                درون_داد_pending = true;
#endif

              اگر (!درون_داد_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = اقرأ (درون_داد_desc, inbuf, insize);
              اگر (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  بازگشت false;
                }
              اگر (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  بازگشت true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          وگرنه
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              اگر (++newlines > 0)
                {
                  اگر (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      اگر (squeeze_blank)
                        {
                          ch = *bpin++;
                          ادامه;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  اگر (عدد && !عدد_nonblank)
                    {
                      next_سطر_رقم ();
                      bpout = stpcpy (bpout, سطر_رقم_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              اگر (show_ends)
                {
                  اگر (pending_cr)
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
      وقتی (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      اگر (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      اگر (newlines >= 0 && عدد)
        {
          next_سطر_رقم ();
          bpout = stpcpy (bpout, سطر_رقم_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      اگر (show_nonprinting)
        {
          وقتی (true)
            {
              اگر (ch >= 32)
                {
                  اگر (ch < 127)
                    *bpout++ = ch;
                  وگرنه اگر (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  وگرنه
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      اگر (ch >= 128 + 32)
                        {
                          اگر (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          وگرنه
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      وگرنه
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              وگرنه اگر (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              وگرنه اگر (ch == '\n')
                {
                  newlines = -1;
                  بشکن;
                }
              وگرنه
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      وگرنه
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          وقتی (true)
            {
              اگر (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              وگرنه اگر (ch != '\n')
                {
                  اگر (ch == '\r' && *bpin == '\n' && show_ends)
                    {
                      اگر (bpin == eob)
                        pending_cr = true;
                      وگرنه
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  وگرنه
                    *bpout++ = ch;
                }
              وگرنه
                {
                  newlines = -1;
                  بشکن;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ایستا صحیح
copy_cat (تهی)
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

  برای (bool some_copied = false; ; some_copied = true)
    انتخاب (copy_file_range (درون_داد_desc, پوچ, STDOUT_FILENO, پوچ,
                             copy_max, 0))
      {
      حالت 0:
        بازگشت some_copied;

      حالت -1:
        اگر (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          بازگشت 0;
        error (0, errno, "%s", quotef (infile));
        بازگشت -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ایستا صحیح
splice_cat (تهی)
{
  bool some_copied = false;
  bool in_ok = true;
  bool out_ok = true;

#اگر HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  ایستا صحیح pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  ایستا idx_t pipefd_pipe_size = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  اگر (pipefd[0] < 0)
    {
      اگر (pipe (pipefd) < 0)
        بازگشت false;
      pipefd_pipe_size = increase_pipe_size (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  ایستا صحیح stdout_is_pipe = -1;
  ایستا idx_t stdout_pipe_size = 0;
  اگر (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      اگر (stdout_is_pipe)
        stdout_pipe_size = increase_pipe_size (STDOUT_FILENO);
    }

  idx_t pipe_size = MAX (pipefd_pipe_size, stdout_pipe_size);

  وقتی (true)
    {
      ssize_t bytes_read = splice (درون_داد_desc, پوچ, pipefd[1], پوچ,
                                   pipe_size, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      in_ok = 0 <= bytes_read || ! some_copied;
      اگر (bytes_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      اگر (bytes_read <= 0)
        برو done;
      /* We need to drain the intermediate pipe to standard output.  */
      وقتی (0 < bytes_read)
        {
          ssize_t bytes_written = splice (pipefd[0], پوچ, STDOUT_FILENO, پوچ,
                                          pipe_size, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          اگر (bytes_written < 0)
            {
              اگر (some_copied)
                out_ok = false;
              وگرنه
                {
                  نویسه buf[BUFSIZ];
                  وقتی (0 < bytes_read)
                    {
                      ssize_t شمار = MIN (bytes_read, اندازه buf);
                      ssize_t n_read = اقرأ (pipefd[0], buf, شمار);
                      /* Failure not associated with in or out.  */
                      in_ok = out_ok = 0 <= n_read;
                      اگر (n_read <= 0)
                        برو done;
                      اگر (full_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      bytes_read -= n_read;
                    }
                }
            }
          اگر (bytes_written <= 0)
            برو done;
          some_copied = true;
          bytes_read -= bytes_written;
        }
    }

 done:
  اگر (! in_ok && ! out_ok)
    {
      /* Recreate the pipe on internal error.  */
      صحیح saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_size = 0;
      error (0, errno, "%s", _("splice error"));
    }
  وگرنه اگر (! in_ok)
    error (0, errno, "%s", quotef (infile));
  وگرنه اگر (! out_ok)
    write_error ();
#endif

  بازگشت (in_ok && out_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

ایستا نویسه *
ensure_buf_size (نویسه *buf, idx_t *buf_alloc, idx_t alignment, idx_t size)
{
  affirm (buf != پوچ || *buf_alloc < size);

  اگر (*buf_alloc < size)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, size);
      *buf_alloc = size;
    }

  بازگشت buf;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  ساختار stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool عدد = false;
  bool عدد_nonblank = false;
  bool squeeze_blank = false;
  bool show_ends = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  صحیح file_open_mode = O_RDONLY;

  ایستا ساختار option ثابت long_options[] =
  {
    {"number-nonblank", no_argument, پوچ, 'b'},
    {"number", no_argument, پوچ, 'n'},
    {"squeeze-blank", no_argument, پوچ, 's'},
    {"show-nonprinting", no_argument, پوچ, 'v'},
    {"show-ends", no_argument, پوچ, 'E'},
    {"show-tabs", no_argument, پوچ, 'T'},
    {"show-all", no_argument, پوچ, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {پوچ, 0, پوچ, 0}
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

  صحیح c;
  وقتی ((c = getopt_long (argc, argv, "benstuvAET", long_options, پوچ))
         != -1)
    {
      انتخاب (c)
        {
        حالت 'b':
          عدد = true;
          عدد_nonblank = true;
          بشکن;

        حالت 'e':
          show_ends = true;
          show_nonprinting = true;
          بشکن;

        حالت 'n':
          عدد = true;
          بشکن;

        حالت 's':
          squeeze_blank = true;
          بشکن;

        حالت 't':
          show_tabs = true;
          show_nonprinting = true;
          بشکن;

        حالت 'u':
          /* We provide the -u feature unconditionally.  */
          بشکن;

        حالت 'v':
          show_nonprinting = true;
          بشکن;

        حالت 'A':
          show_nonprinting = true;
          show_ends = true;
          show_tabs = true;
          بشکن;

        حالت 'E':
          show_ends = true;
          بشکن;

        حالت 'T':
          show_tabs = true;
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  اگر (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  صحیح out_flags = -2;

  /* True if the output is a regular file.  */
  bool out_isreg = S_ISREG (ostat_buf.st_mode) != 0;

  اگر (! (عدد || show_ends || squeeze_blank))
    {
      file_open_mode |= O_دودویی;
      xset_دودویی_mode (STDOUT_FILENO, O_دودویی);
    }

  /* Main loop.  */

  infile = "-";
  صحیح argind = optind;
  bool ok = true;
  idx_t page_size = getpagesize ();
  نویسه *inbuf = پوچ;
  نویسه *outbuf = پوچ;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  انجامبده
    {
      اگر (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      اگر (reading_stdin)
        {
          have_read_stdin = true;
          درون_داد_desc = STDIN_FILENO;
          اگر (file_open_mode & O_دودویی)
            xset_دودویی_mode (STDIN_FILENO, O_دودویی);
        }
      وگرنه
        {
          درون_داد_desc = open (infile, file_open_mode);
          اگر (درون_داد_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              ادامه;
            }
        }

      ساختار stat istat_buf;
      اگر (fstat (درون_داد_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          برو contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (درون_داد_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      اگر (! (S_ISFIFO (istat_buf.st_mode) || S_ISSOCK (istat_buf.st_mode)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t in_pos = lseek (درون_داد_desc, 0, SEEK_CUR);
          اگر (0 <= in_pos)
            {
              اگر (out_flags < -1)
                out_flags = fcntl (STDOUT_FILENO, F_GETFL);
              صحیح whence = (0 <= out_flags && out_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              اگر (in_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  برو contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      اگر (! (عدد || show_ends || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          صحیح copy_cat_status =
            out_isreg && S_ISREG (istat_buf.st_mode) ? copy_cat () : 0;
          اگر (copy_cat_status != 0)
            ok &= 0 < copy_cat_status;
          وگرنه
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              صحیح splice_cat_status = ((usable_st_size (&istat_buf)
                                        && istat_buf.st_size <= 32768)
                                       ? 0 : splice_cat ());
              اگر (splice_cat_status != 0)
                ok &= 0 < splice_cat_status;
              وگرنه
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_size (inbuf, &inbuf_alloc,
                                           page_size, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      وگرنه
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
          اگر (ckd_mul (&bufsize, insize, 4)
              || ckd_add (&bufsize, bufsize, outsize)
              || ckd_add (&bufsize, bufsize, سطر_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_size (outbuf, &outbuf_alloc,
                                    page_size, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, عدد, عدد_nonblank, show_ends,
                     squeeze_blank);
        }

    contin:
      اگر (!reading_stdin && close (درون_داد_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  وقتی (++argind < argc);

  اگر (pending_cr)
    {
      اگر (full_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#ifdef lint
  alignfree (outbuf);
  alignfree (inbuf);
#endif

  اگر (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
