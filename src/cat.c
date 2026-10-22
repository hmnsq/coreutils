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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#إذا HAVE_STROPTS_H
# include <stropts.h>
#جے_انت
#ضمن <sys/ioctl.h>

#ضمن "system.h"
#ضمن "alignalloc.h"
#ضمن "assure.h"
#ضمن "ioblksize.h"
#ضمن "fadvise.h"
#ضمن "full-write.h"
#ضمن "isapipe.h"
#ضمن "splice.h"
#ضمن "unistd--.h"
#ضمن "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "cat"

#تعریف_کرو AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
ساكن حرف ثابت *infile;

/* Descriptor on which input file is open.  */
ساكن صحيح داخلہ_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#تعریف_کرو سطر_COUNTER_BUF_LEN 20
ساكن حرف سطر_buf[سطر_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
ساكن حرف *سطر_عدد_print = سطر_buf + سطر_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
ساكن حرف *سطر_عدد_شروع = سطر_buf + سطر_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
ساكن حرف *سطر_عدد_سرا = سطر_buf + سطر_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
ساكن صحيح newlines2 = 0;

/* Whether there is a pending CR to process.  */
ساكن bool pending_cr = false;

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Concatenate FILE(s) منزل standard output.\n\
"), stdout);

      emit_stdin_note ();

      oputs (_("\
  -A, --show-all           equivalent منزل -vET\n\
"));
      oputs (_("\
  -b, --نمبر-nonblank    نمبر nonempty output سطریں, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent منزل -vE\n\
"));
      oputs (_("\
  -E, --show-سرے          display $ or ^M$ at سرا دا each سطر\n\
"));
      oputs (_("\
  -n, --نمبر             نمبر all output سطریں\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated خالی_تھاواں output سطریں\n\
"));
      oputs (_("\
  -t                       equivalent منزل -vT\n\
"));
      oputs (_("\
  -T, --show-tabs          display TAB characters as ^I\n\
"));
      oputs (_("\
  -u                       (ignored)\n\
"));
      oputs (_("\
  -v, --show-nonprinting   use ^ and M- notation, except لكل LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      لکھو (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard داخلہ منزل standard output.\n\
"),
              پروگرام_name, پروگرام_name);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Compute the next line number.  */

ساكن فراغ
اگلا_سطر_عدد (فراغ)
{
  حرف *endp = سطر_عدد_سرا;
  افعل
    {
      إذا ((*endp)++ < '9')
        أرجع;
      *endp-- = '0';
    }
  طالما (endp >= سطر_عدد_شروع);

  إذا (سطر_عدد_شروع > سطر_buf)
    *--سطر_عدد_شروع = '1';
  وإلا
    *سطر_buf = '>';
  إذا (سطر_عدد_شروع < سطر_عدد_print)
    سطر_عدد_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

ساكن bool
simple_cat (حرف *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  طالما (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = اقرأ (داخلہ_desc, buf, bufsize);
      إذا (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          أرجع false;
        }

      /* End of this file?  */

      إذا (n_read == 0)
        أرجع true;

      /* Write this block out.  */

      إذا (بھریا_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

ساكن مضمن فراغ
write_pending (حرف *outbuf, حرف **bpout)
{
  idx_t n_write = *bpout - outbuf;
  إذا (0 < n_write)
    {
      إذا (بھریا_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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

ساكن bool
cat (حرف *inbuf, idx_t insize, حرف *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool نمبر, bool نمبر_nonblank,
     bool show_سرے, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  بنانشانی حرف ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  صحيح newlines = newlines2;

#جے_تعریف_شدہ FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#جے_انت

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  حرف *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  حرف *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  حرف *bpout = outbuf;

  طالما (true)
    {
      افعل
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          إذا (outbuf + outsize <= bpout)
            {
              حرف *wp = outbuf;
              idx_t بچیا_بائٹاں;
              افعل
                {
                  إذا (بھریا_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  بچیا_بائٹاں = bpout - wp;
                }
              طالما (outsize <= بچیا_بائٹاں);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, بچیا_بائٹاں);
              bpout = outbuf + بچیا_بائٹاں;
            }

          /* Is INBUF empty?  */

          إذا (bpin > eob)
            {
              bool داخلہ_pending = false;
#جے_تعریف_شدہ FIONREAD
              صحيح n_منزل_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              إذا (use_fionread
                  && ioctl (داخلہ_desc, FIONREAD, &n_منزل_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  إذا (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  وإلا
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      أرجع false;
                    }
                }
              إذا (n_منزل_read != 0)
                داخلہ_pending = true;
#جے_انت

              إذا (!داخلہ_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = اقرأ (داخلہ_desc, inbuf, insize);
              إذا (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  أرجع false;
                }
              إذا (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  أرجع true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          وإلا
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              إذا (++newlines > 0)
                {
                  إذا (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      إذا (squeeze_blank)
                        {
                          ch = *bpin++;
                          استمر;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  إذا (نمبر && !نمبر_nonblank)
                    {
                      اگلا_سطر_عدد ();
                      bpout = stpcpy (bpout, سطر_عدد_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              إذا (show_سرے)
                {
                  إذا (pending_cr)
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
      طالما (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      إذا (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      إذا (newlines >= 0 && نمبر)
        {
          اگلا_سطر_عدد ();
          bpout = stpcpy (bpout, سطر_عدد_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      إذا (show_nonprinting)
        {
          طالما (true)
            {
              إذا (ch >= 32)
                {
                  إذا (ch < 127)
                    *bpout++ = ch;
                  وإلا إذا (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  وإلا
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      إذا (ch >= 128 + 32)
                        {
                          إذا (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          وإلا
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      وإلا
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              وإلا إذا (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              وإلا إذا (ch == '\n')
                {
                  newlines = -1;
                  اكسر;
                }
              وإلا
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      وإلا
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          طالما (true)
            {
              إذا (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              وإلا إذا (ch != '\n')
                {
                  إذا (ch == '\r' && *bpin == '\n' && show_سرے)
                    {
                      إذا (bpin == eob)
                        pending_cr = true;
                      وإلا
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  وإلا
                    *bpout++ = ch;
                }
              وإلا
                {
                  newlines = -1;
                  اكسر;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ساكن صحيح
copy_cat (فراغ)
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

  لكل (bool some_copied = false; ; some_copied = true)
    بدل (copy_file_range (داخلہ_desc, عدم, STDOUT_FILENO, عدم,
                             copy_max, 0))
      {
      حالة 0:
        أرجع some_copied;

      حالة -1:
        إذا (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          أرجع 0;
        error (0, errno, "%s", quotef (infile));
        أرجع -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

ساكن صحيح
splice_cat (فراغ)
{
  bool some_copied = false;
  bool وچ_ok = true;
  bool باہرلا_ok = true;

#إذا HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  ساكن صحيح pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  ساكن idx_t pipefd_pipe_ناپ = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  إذا (pipefd[0] < 0)
    {
      إذا (pipe (pipefd) < 0)
        أرجع false;
      pipefd_pipe_ناپ = increase_pipe_ناپ (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  ساكن صحيح stdout_is_pipe = -1;
  ساكن idx_t stdout_pipe_ناپ = 0;
  إذا (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      إذا (stdout_is_pipe)
        stdout_pipe_ناپ = increase_pipe_ناپ (STDOUT_FILENO);
    }

  idx_t pipe_ناپ = MAX (pipefd_pipe_ناپ, stdout_pipe_ناپ);

  طالما (true)
    {
      ssize_t بائٹاں_read = splice (داخلہ_desc, عدم, pipefd[1], عدم,
                                   pipe_ناپ, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      وچ_ok = 0 <= بائٹاں_read || ! some_copied;
      إذا (بائٹاں_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      إذا (بائٹاں_read <= 0)
        اذهب پورا;
      /* We need to drain the intermediate pipe to standard output.  */
      طالما (0 < بائٹاں_read)
        {
          ssize_t بائٹاں_written = splice (pipefd[0], عدم, STDOUT_FILENO, عدم,
                                          pipe_ناپ, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          إذا (بائٹاں_written < 0)
            {
              إذا (some_copied)
                باہرلا_ok = false;
              وإلا
                {
                  حرف buf[BUFSIZ];
                  طالما (0 < بائٹاں_read)
                    {
                      ssize_t گنتی = MIN (بائٹاں_read, حجم buf);
                      ssize_t n_read = اقرأ (pipefd[0], buf, گنتی);
                      /* Failure not associated with in or out.  */
                      وچ_ok = باہرلا_ok = 0 <= n_read;
                      إذا (n_read <= 0)
                        اذهب پورا;
                      إذا (بھریا_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      بائٹاں_read -= n_read;
                    }
                }
            }
          إذا (بائٹاں_written <= 0)
            اذهب پورا;
          some_copied = true;
          بائٹاں_read -= بائٹاں_written;
        }
    }

 پورا:
  إذا (! وچ_ok && ! باہرلا_ok)
    {
      /* Recreate the pipe on internal error.  */
      صحيح saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_ناپ = 0;
      error (0, errno, "%s", _("splice error"));
    }
  وإلا إذا (! وچ_ok)
    error (0, errno, "%s", quotef (infile));
  وإلا إذا (! باہرلا_ok)
    write_error ();
#جے_انت

  أرجع (وچ_ok && باہرلا_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

ساكن حرف *
ensure_buf_ناپ (حرف *buf, idx_t *buf_alloc, idx_t alignment, idx_t ناپ)
{
  affirm (buf != عدم || *buf_alloc < ناپ);

  إذا (*buf_alloc < ناپ)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, ناپ);
      *buf_alloc = ناپ;
    }

  أرجع buf;
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  بنية stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool نمبر = false;
  bool نمبر_nonblank = false;
  bool squeeze_blank = false;
  bool show_سرے = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  صحيح file_open_منوال = O_RDONLY;

  ساكن بنية option ثابت long_options[] =
  {
    {"number-nonblank", no_دلیل, عدم, 'b'},
    {"number", no_دلیل, عدم, 'n'},
    {"squeeze-blank", no_دلیل, عدم, 's'},
    {"show-nonprinting", no_دلیل, عدم, 'v'},
    {"show-ends", no_دلیل, عدم, 'E'},
    {"show-tabs", no_دلیل, عدم, 'T'},
    {"show-all", no_دلیل, عدم, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_نسخہ_OPTION_DECL},
    {عدم, 0, عدم, 0}
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

  صحيح c;
  طالما ((c = getopt_long (argc, argv, "benstuvAET", long_options, عدم))
         != -1)
    {
      بدل (c)
        {
        حالة 'b':
          نمبر = true;
          نمبر_nonblank = true;
          اكسر;

        حالة 'e':
          show_سرے = true;
          show_nonprinting = true;
          اكسر;

        حالة 'n':
          نمبر = true;
          اكسر;

        حالة 's':
          squeeze_blank = true;
          اكسر;

        حالة 't':
          show_tabs = true;
          show_nonprinting = true;
          اكسر;

        حالة 'u':
          /* We provide the -u feature unconditionally.  */
          اكسر;

        حالة 'v':
          show_nonprinting = true;
          اكسر;

        حالة 'A':
          show_nonprinting = true;
          show_سرے = true;
          show_tabs = true;
          اكسر;

        حالة 'E':
          show_سرے = true;
          اكسر;

        حالة 'T':
          show_tabs = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  إذا (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  صحيح باہرلا_flags = -2;

  /* True if the output is a regular file.  */
  bool باہرلا_isreg = S_ISREG (ostat_buf.st_منوال) != 0;

  إذا (! (نمبر || show_سرے || squeeze_blank))
    {
      file_open_منوال |= O_ثنائی;
      xset_ثنائی_منوال (STDOUT_FILENO, O_ثنائی);
    }

  /* Main loop.  */

  infile = "-";
  صحيح argind = optind;
  bool ok = true;
  idx_t ورقہ_ناپ = getpagesize ();
  حرف *inbuf = عدم;
  حرف *outbuf = عدم;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  افعل
    {
      إذا (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      إذا (reading_stdin)
        {
          have_read_stdin = true;
          داخلہ_desc = STDIN_FILENO;
          إذا (file_open_منوال & O_ثنائی)
            xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
        }
      وإلا
        {
          داخلہ_desc = open (infile, file_open_منوال);
          إذا (داخلہ_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              استمر;
            }
        }

      بنية stat istat_buf;
      إذا (fstat (داخلہ_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          اذهب contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (داخلہ_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      إذا (! (S_ISFIFO (istat_buf.st_منوال) || S_ISSOCK (istat_buf.st_منوال)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t وچ_pos = lseek (داخلہ_desc, 0, SEEK_CUR);
          إذا (0 <= وچ_pos)
            {
              إذا (باہرلا_flags < -1)
                باہرلا_flags = fcntl (STDOUT_FILENO, F_GETFL);
              صحيح whence = (0 <= باہرلا_flags && باہرلا_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              إذا (وچ_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  اذهب contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      إذا (! (نمبر || show_سرے || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          صحيح copy_cat_حالت =
            باہرلا_isreg && S_ISREG (istat_buf.st_منوال) ? copy_cat () : 0;
          إذا (copy_cat_حالت != 0)
            ok &= 0 < copy_cat_حالت;
          وإلا
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              صحيح splice_cat_حالت = ((usable_st_ناپ (&istat_buf)
                                        && istat_buf.st_ناپ <= 32768)
                                       ? 0 : splice_cat ());
              إذا (splice_cat_حالت != 0)
                ok &= 0 < splice_cat_حالت;
              وإلا
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_ناپ (inbuf, &inbuf_alloc,
                                           ورقہ_ناپ, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      وإلا
        {
          /* Allocate, with an extra byte for a newline sentinel.  */
          inbuf = ensure_buf_ناپ (inbuf, &inbuf_alloc,
                                   ورقہ_ناپ, insize + 1);

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
          إذا (ckd_mul (&bufsize, insize, 4)
              || ckd_جوڑو (&bufsize, bufsize, outsize)
              || ckd_جوڑو (&bufsize, bufsize, سطر_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_ناپ (outbuf, &outbuf_alloc,
                                    ورقہ_ناپ, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, نمبر, نمبر_nonblank, show_سرے,
                     squeeze_blank);
        }

    contin:
      إذا (!reading_stdin && close (داخلہ_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  طالما (++argind < argc);

  إذا (pending_cr)
    {
      إذا (بھریا_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#جے_تعریف_شدہ lint
  alignfree (outbuf);
  alignfree (inbuf);
#جے_انت

  إذا (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
