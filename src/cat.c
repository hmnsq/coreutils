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
#اگر_ختم
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
#تعریف_کرو پروگرام_NAME "cat"

#تعریف_کرو AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
ساکن حرف ثابت *infile;

/* Descriptor on which input file is open.  */
ساکن صحیح داخلہ_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#تعریف_کرو سطر_شمار_BUF_LEN 20
ساکن حرف سطر_buf[سطر_شمار_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
ساکن حرف *سطر_عدد_print = سطر_buf + سطر_شمار_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
ساکن حرف *سطر_عدد_آغاز = سطر_buf + سطر_شمار_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
ساکن حرف *سطر_عدد_انتہا = سطر_buf + سطر_شمار_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
ساکن صحیح newlines2 = 0;

/* Whether there is a pending CR to process.  */
ساکن bool pending_cr = false;

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Concatenate FILE(s) مقصد standard output.\n\
"), stdout);

      emit_stdin_note ();

      oputs (_("\
  -A, --show-all           equivalent مقصد -vET\n\
"));
      oputs (_("\
  -b, --نمبر-nonblank    نمبر nonempty output سطریں, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent مقصد -vE\n\
"));
      oputs (_("\
  -E, --show-سرے          display $ or ^M$ at انتہا کا each سطر\n\
"));
      oputs (_("\
  -n, --نمبر             نمبر all output سطریں\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated خالی_خانے output سطریں\n\
"));
      oputs (_("\
  -t                       equivalent مقصد -vT\n\
"));
      oputs (_("\
  -T, --show-tabs          display TAB characters as ^I\n\
"));
      oputs (_("\
  -u                       (ignored)\n\
"));
      oputs (_("\
  -v, --show-nonprinting   use ^ and M- notation, except برائے LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      لکھو (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard داخلہ مقصد standard output.\n\
"),
              پروگرام_name, پروگرام_name);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Compute the next line number.  */

ساکن خالی
اگلا_سطر_عدد (خالی)
{
  حرف *endp = سطر_عدد_انتہا;
  کرو
    {
      اگر ((*endp)++ < '9')
        واپس;
      *endp-- = '0';
    }
  جب (endp >= سطر_عدد_آغاز);

  اگر (سطر_عدد_آغاز > سطر_buf)
    *--سطر_عدد_آغاز = '1';
  ورنہ
    *سطر_buf = '>';
  اگر (سطر_عدد_آغاز < سطر_عدد_print)
    سطر_عدد_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

ساکن bool
simple_cat (حرف *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  جب (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = اقرأ (داخلہ_desc, buf, bufsize);
      اگر (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          واپس false;
        }

      /* End of this file?  */

      اگر (n_read == 0)
        واپس true;

      /* Write this block out.  */

      اگر (بھرا_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

ساکن اندرونی خالی
write_pending (حرف *outbuf, حرف **bpout)
{
  idx_t n_write = *bpout - outbuf;
  اگر (0 < n_write)
    {
      اگر (بھرا_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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

ساکن bool
cat (حرف *inbuf, idx_t insize, حرف *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool نمبر, bool نمبر_nonblank,
     bool show_سرے, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  غیرعلامتی حرف ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  صحیح newlines = newlines2;

#اگر_تعریف_شدہ FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#اگر_ختم

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  حرف *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  حرف *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  حرف *bpout = outbuf;

  جب (true)
    {
      کرو
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          اگر (outbuf + outsize <= bpout)
            {
              حرف *wp = outbuf;
              idx_t بقایا_بائٹیں;
              کرو
                {
                  اگر (بھرا_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  بقایا_بائٹیں = bpout - wp;
                }
              جب (outsize <= بقایا_بائٹیں);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, بقایا_بائٹیں);
              bpout = outbuf + بقایا_بائٹیں;
            }

          /* Is INBUF empty?  */

          اگر (bpin > eob)
            {
              bool داخلہ_pending = false;
#اگر_تعریف_شدہ FIONREAD
              صحیح n_مقصد_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              اگر (use_fionread
                  && ioctl (داخلہ_desc, FIONREAD, &n_مقصد_read) < 0)
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
                  ورنہ
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      واپس false;
                    }
                }
              اگر (n_مقصد_read != 0)
                داخلہ_pending = true;
#اگر_ختم

              اگر (!داخلہ_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = اقرأ (داخلہ_desc, inbuf, insize);
              اگر (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  واپس false;
                }
              اگر (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  واپس true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          ورنہ
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
                          جاری;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  اگر (نمبر && !نمبر_nonblank)
                    {
                      اگلا_سطر_عدد ();
                      bpout = stpcpy (bpout, سطر_عدد_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              اگر (show_سرے)
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
      جب (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      اگر (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      اگر (newlines >= 0 && نمبر)
        {
          اگلا_سطر_عدد ();
          bpout = stpcpy (bpout, سطر_عدد_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      اگر (show_nonprinting)
        {
          جب (true)
            {
              اگر (ch >= 32)
                {
                  اگر (ch < 127)
                    *bpout++ = ch;
                  ورنہ اگر (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  ورنہ
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      اگر (ch >= 128 + 32)
                        {
                          اگر (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          ورنہ
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      ورنہ
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              ورنہ اگر (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              ورنہ اگر (ch == '\n')
                {
                  newlines = -1;
                  توڑو;
                }
              ورنہ
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      ورنہ
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          جب (true)
            {
              اگر (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              ورنہ اگر (ch != '\n')
                {
                  اگر (ch == '\r' && *bpin == '\n' && show_سرے)
                    {
                      اگر (bpin == eob)
                        pending_cr = true;
                      ورنہ
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  ورنہ
                    *bpout++ = ch;
                }
              ورنہ
                {
                  newlines = -1;
                  توڑو;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ساکن صحیح
copy_cat (خالی)
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

  برائے (bool some_copied = false; ; some_copied = true)
    انتخاب (copy_file_range (داخلہ_desc, تہی, STDOUT_FILENO, تہی,
                             copy_max, 0))
      {
      حالت 0:
        واپس some_copied;

      حالت -1:
        اگر (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          واپس 0;
        error (0, errno, "%s", quotef (infile));
        واپس -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ساکن صحیح
splice_cat (خالی)
{
  bool some_copied = false;
  bool اندر_ok = true;
  bool برآمد_ok = true;

#اگر HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  ساکن صحیح pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  ساکن idx_t pipefd_pipe_سائز = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  اگر (pipefd[0] < 0)
    {
      اگر (pipe (pipefd) < 0)
        واپس false;
      pipefd_pipe_سائز = increase_pipe_سائز (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  ساکن صحیح stdout_is_pipe = -1;
  ساکن idx_t stdout_pipe_سائز = 0;
  اگر (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      اگر (stdout_is_pipe)
        stdout_pipe_سائز = increase_pipe_سائز (STDOUT_FILENO);
    }

  idx_t pipe_سائز = MAX (pipefd_pipe_سائز, stdout_pipe_سائز);

  جب (true)
    {
      ssize_t بائٹیں_read = splice (داخلہ_desc, تہی, pipefd[1], تہی,
                                   pipe_سائز, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      اندر_ok = 0 <= بائٹیں_read || ! some_copied;
      اگر (بائٹیں_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      اگر (بائٹیں_read <= 0)
        جاؤ مکمل;
      /* We need to drain the intermediate pipe to standard output.  */
      جب (0 < بائٹیں_read)
        {
          ssize_t بائٹیں_written = splice (pipefd[0], تہی, STDOUT_FILENO, تہی,
                                          pipe_سائز, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          اگر (بائٹیں_written < 0)
            {
              اگر (some_copied)
                برآمد_ok = false;
              ورنہ
                {
                  حرف buf[BUFSIZ];
                  جب (0 < بائٹیں_read)
                    {
                      ssize_t گنتی = MIN (بائٹیں_read, حجم buf);
                      ssize_t n_read = اقرأ (pipefd[0], buf, گنتی);
                      /* Failure not associated with in or out.  */
                      اندر_ok = برآمد_ok = 0 <= n_read;
                      اگر (n_read <= 0)
                        جاؤ مکمل;
                      اگر (بھرا_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      بائٹیں_read -= n_read;
                    }
                }
            }
          اگر (بائٹیں_written <= 0)
            جاؤ مکمل;
          some_copied = true;
          بائٹیں_read -= بائٹیں_written;
        }
    }

 مکمل:
  اگر (! اندر_ok && ! برآمد_ok)
    {
      /* Recreate the pipe on internal error.  */
      صحیح saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_سائز = 0;
      error (0, errno, "%s", _("splice error"));
    }
  ورنہ اگر (! اندر_ok)
    error (0, errno, "%s", quotef (infile));
  ورنہ اگر (! برآمد_ok)
    write_error ();
#اگر_ختم

  واپس (اندر_ok && برآمد_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

ساکن حرف *
ensure_buf_سائز (حرف *buf, idx_t *buf_alloc, idx_t alignment, idx_t سائز)
{
  affirm (buf != تہی || *buf_alloc < سائز);

  اگر (*buf_alloc < سائز)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, سائز);
      *buf_alloc = سائز;
    }

  واپس buf;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  ساخت stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool نمبر = false;
  bool نمبر_nonblank = false;
  bool squeeze_blank = false;
  bool show_سرے = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  صحیح file_open_منوال = O_RDONLY;

  ساکن ساخت option ثابت long_options[] =
  {
    {"number-nonblank", no_دلیل, تہی, 'b'},
    {"number", no_دلیل, تہی, 'n'},
    {"squeeze-blank", no_دلیل, تہی, 's'},
    {"show-nonprinting", no_دلیل, تہی, 'v'},
    {"show-ends", no_دلیل, تہی, 'E'},
    {"show-tabs", no_دلیل, تہی, 'T'},
    {"show-all", no_دلیل, تہی, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_نسخہ_OPTION_DECL},
    {تہی, 0, تہی, 0}
  };

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
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
  جب ((c = getopt_long (argc, argv, "benstuvAET", long_options, تہی))
         != -1)
    {
      انتخاب (c)
        {
        حالت 'b':
          نمبر = true;
          نمبر_nonblank = true;
          توڑو;

        حالت 'e':
          show_سرے = true;
          show_nonprinting = true;
          توڑو;

        حالت 'n':
          نمبر = true;
          توڑو;

        حالت 's':
          squeeze_blank = true;
          توڑو;

        حالت 't':
          show_tabs = true;
          show_nonprinting = true;
          توڑو;

        حالت 'u':
          /* We provide the -u feature unconditionally.  */
          توڑو;

        حالت 'v':
          show_nonprinting = true;
          توڑو;

        حالت 'A':
          show_nonprinting = true;
          show_سرے = true;
          show_tabs = true;
          توڑو;

        حالت 'E':
          show_سرے = true;
          توڑو;

        حالت 'T':
          show_tabs = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  اگر (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  صحیح برآمد_flags = -2;

  /* True if the output is a regular file.  */
  bool برآمد_isreg = S_ISREG (ostat_buf.st_منوال) != 0;

  اگر (! (نمبر || show_سرے || squeeze_blank))
    {
      file_open_منوال |= O_ثنائی;
      xset_ثنائی_منوال (STDOUT_FILENO, O_ثنائی);
    }

  /* Main loop.  */

  infile = "-";
  صحیح argind = optind;
  bool ok = true;
  idx_t صفحہ_سائز = getpagesize ();
  حرف *inbuf = تہی;
  حرف *outbuf = تہی;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  کرو
    {
      اگر (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      اگر (reading_stdin)
        {
          have_read_stdin = true;
          داخلہ_desc = STDIN_FILENO;
          اگر (file_open_منوال & O_ثنائی)
            xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
        }
      ورنہ
        {
          داخلہ_desc = open (infile, file_open_منوال);
          اگر (داخلہ_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              جاری;
            }
        }

      ساخت stat istat_buf;
      اگر (fstat (داخلہ_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          جاؤ contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (داخلہ_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      اگر (! (S_ISFIFO (istat_buf.st_منوال) || S_ISSOCK (istat_buf.st_منوال)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t اندر_pos = lseek (داخلہ_desc, 0, SEEK_CUR);
          اگر (0 <= اندر_pos)
            {
              اگر (برآمد_flags < -1)
                برآمد_flags = fcntl (STDOUT_FILENO, F_GETFL);
              صحیح whence = (0 <= برآمد_flags && برآمد_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              اگر (اندر_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  جاؤ contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      اگر (! (نمبر || show_سرے || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          صحیح copy_cat_حالت =
            برآمد_isreg && S_ISREG (istat_buf.st_منوال) ? copy_cat () : 0;
          اگر (copy_cat_حالت != 0)
            ok &= 0 < copy_cat_حالت;
          ورنہ
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              صحیح splice_cat_حالت = ((usable_st_سائز (&istat_buf)
                                        && istat_buf.st_سائز <= 32768)
                                       ? 0 : splice_cat ());
              اگر (splice_cat_حالت != 0)
                ok &= 0 < splice_cat_حالت;
              ورنہ
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_سائز (inbuf, &inbuf_alloc,
                                           صفحہ_سائز, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      ورنہ
        {
          /* Allocate, with an extra byte for a newline sentinel.  */
          inbuf = ensure_buf_سائز (inbuf, &inbuf_alloc,
                                   صفحہ_سائز, insize + 1);

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
              || ckd_جوڑو (&bufsize, bufsize, outsize)
              || ckd_جوڑو (&bufsize, bufsize, سطر_شمار_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_سائز (outbuf, &outbuf_alloc,
                                    صفحہ_سائز, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, نمبر, نمبر_nonblank, show_سرے,
                     squeeze_blank);
        }

    contin:
      اگر (!reading_stdin && close (داخلہ_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  جب (++argind < argc);

  اگر (pending_cr)
    {
      اگر (بھرا_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#اگر_تعریف_شدہ lint
  alignfree (outbuf);
  alignfree (inbuf);
#اگر_ختم

  اگر (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
