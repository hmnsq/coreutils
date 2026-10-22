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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>

#যদি HAVE_STROPTS_H
# include <stropts.h>
#endif
#অন্তর্ভাৱ <sys/ioctl.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "alignalloc.h"
#অন্তর্ভাৱ "assure.h"
#অন্তর্ভাৱ "ioblksize.h"
#অন্তর্ভাৱ "fadvise.h"
#অন্তর্ভাৱ "full-write.h"
#অন্তর্ভাৱ "isapipe.h"
#অন্তর্ভাৱ "splice.h"
#অন্তর্ভাৱ "unistd--.h"
#অন্তর্ভাৱ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cat"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
স্থির ৱর্ণ ধ্রুৱ *infile;

/* Descriptor on which input file is open.  */
স্থির পূর্ণাঙ্ক প্ৰবেশ_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#define সাৰি_COUNTER_BUF_LEN 20
স্থির ৱর্ণ সাৰি_buf[সাৰি_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
স্থির ৱর্ণ *সাৰি_অঙ্ক_print = সাৰি_buf + সাৰি_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
স্থির ৱর্ণ *সাৰি_অঙ্ক_start = সাৰি_buf + সাৰি_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
স্থির ৱর্ণ *সাৰি_অঙ্ক_end = সাৰি_buf + সাৰি_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
স্থির পূর্ণাঙ্ক newlines2 = 0;

/* Whether there is a pending CR to process.  */
স্থির bool pending_cr = false;

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
  -b, --সংখ্যা-nonblank    সংখ্যা nonempty output সাৰিগুলি, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent to -vE\n\
"));
      oputs (_("\
  -E, --show-ends          display $ or ^M$ at end of each সাৰি\n\
"));
      oputs (_("\
  -n, --সংখ্যা             সংখ্যা all output সাৰিগুলি\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated empty output সাৰিগুলি\n\
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
  -v, --show-nonprinting   use ^ and M- notation, except বাবে LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard প্ৰবেশ to standard output.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Compute the next line number.  */

স্থির শূন্য
next_সাৰি_অঙ্ক (শূন্য)
{
  ৱর্ণ *endp = সাৰি_অঙ্ক_end;
  কৰা
    {
      যদি ((*endp)++ < '9')
        ঘূৰাই;
      *endp-- = '0';
    }
  যেতিয়ালৈ (endp >= সাৰি_অঙ্ক_start);

  যদি (সাৰি_অঙ্ক_start > সাৰি_buf)
    *--সাৰি_অঙ্ক_start = '1';
  অন্যথা
    *সাৰি_buf = '>';
  যদি (সাৰি_অঙ্ক_start < সাৰি_অঙ্ক_print)
    সাৰি_অঙ্ক_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

স্থির bool
simple_cat (ৱর্ণ *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  যেতিয়ালৈ (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = পঠ (প্ৰবেশ_desc, buf, bufsize);
      যদি (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ঘূৰাই false;
        }

      /* End of this file?  */

      যদি (n_read == 0)
        ঘূৰাই true;

      /* Write this block out.  */

      যদি (full_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

স্থির অন্তঃস্থ শূন্য
write_pending (ৱর্ণ *outbuf, ৱর্ণ **bpout)
{
  idx_t n_write = *bpout - outbuf;
  যদি (0 < n_write)
    {
      যদি (full_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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

স্থির bool
cat (ৱর্ণ *inbuf, idx_t insize, ৱর্ণ *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool সংখ্যা, bool সংখ্যা_nonblank,
     bool show_ends, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  unsigned ৱর্ণ ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  পূর্ণাঙ্ক newlines = newlines2;

#ifdef FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#endif

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  ৱর্ণ *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  ৱর্ণ *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  ৱর্ণ *bpout = outbuf;

  যেতিয়ালৈ (true)
    {
      কৰা
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          যদি (outbuf + outsize <= bpout)
            {
              ৱর্ণ *wp = outbuf;
              idx_t remaining_bytes;
              কৰা
                {
                  যদি (full_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  remaining_bytes = bpout - wp;
                }
              যেতিয়ালৈ (outsize <= remaining_bytes);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, remaining_bytes);
              bpout = outbuf + remaining_bytes;
            }

          /* Is INBUF empty?  */

          যদি (bpin > eob)
            {
              bool প্ৰবেশ_pending = false;
#ifdef FIONREAD
              পূর্ণাঙ্ক n_to_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              যদি (use_fionread
                  && ioctl (প্ৰবেশ_desc, FIONREAD, &n_to_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  যদি (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  অন্যথা
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      ঘূৰাই false;
                    }
                }
              যদি (n_to_read != 0)
                প্ৰবেশ_pending = true;
#endif

              যদি (!প্ৰবেশ_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = পঠ (প্ৰবেশ_desc, inbuf, insize);
              যদি (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  ঘূৰাই false;
                }
              যদি (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  ঘূৰাই true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          অন্যথা
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              যদি (++newlines > 0)
                {
                  যদি (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      যদি (squeeze_blank)
                        {
                          ch = *bpin++;
                          অনুৱর্ত;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  যদি (সংখ্যা && !সংখ্যা_nonblank)
                    {
                      next_সাৰি_অঙ্ক ();
                      bpout = stpcpy (bpout, সাৰি_অঙ্ক_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              যদি (show_ends)
                {
                  যদি (pending_cr)
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
      যেতিয়ালৈ (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      যদি (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      যদি (newlines >= 0 && সংখ্যা)
        {
          next_সাৰি_অঙ্ক ();
          bpout = stpcpy (bpout, সাৰি_অঙ্ক_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      যদি (show_nonprinting)
        {
          যেতিয়ালৈ (true)
            {
              যদি (ch >= 32)
                {
                  যদি (ch < 127)
                    *bpout++ = ch;
                  অন্যথা যদি (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  অন্যথা
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      যদি (ch >= 128 + 32)
                        {
                          যদি (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          অন্যথা
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      অন্যথা
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              অন্যথা যদি (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              অন্যথা যদি (ch == '\n')
                {
                  newlines = -1;
                  ভঙ্গ;
                }
              অন্যথা
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      অন্যথা
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          যেতিয়ালৈ (true)
            {
              যদি (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              অন্যথা যদি (ch != '\n')
                {
                  যদি (ch == '\r' && *bpin == '\n' && show_ends)
                    {
                      যদি (bpin == eob)
                        pending_cr = true;
                      অন্যথা
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  অন্যথা
                    *bpout++ = ch;
                }
              অন্যথা
                {
                  newlines = -1;
                  ভঙ্গ;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

স্থির পূর্ণাঙ্ক
copy_cat (শূন্য)
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

  বাবে (bool some_copied = false; ; some_copied = true)
    ৱিকল্প (copy_file_range (প্ৰবেশ_desc, রিক্ত, STDOUT_FILENO, রিক্ত,
                             copy_max, 0))
      {
      পক্ষ 0:
        ঘূৰাই some_copied;

      পক্ষ -1:
        যদি (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          ঘূৰাই 0;
        error (0, errno, "%s", quotef (infile));
        ঘূৰাই -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

স্থির পূর্ণাঙ্ক
splice_cat (শূন্য)
{
  bool some_copied = false;
  bool in_ok = true;
  bool out_ok = true;

#যদি HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  স্থির পূর্ণাঙ্ক pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  স্থির idx_t pipefd_pipe_size = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  যদি (pipefd[0] < 0)
    {
      যদি (pipe (pipefd) < 0)
        ঘূৰাই false;
      pipefd_pipe_size = increase_pipe_size (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  স্থির পূর্ণাঙ্ক stdout_is_pipe = -1;
  স্থির idx_t stdout_pipe_size = 0;
  যদি (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      যদি (stdout_is_pipe)
        stdout_pipe_size = increase_pipe_size (STDOUT_FILENO);
    }

  idx_t pipe_size = MAX (pipefd_pipe_size, stdout_pipe_size);

  যেতিয়ালৈ (true)
    {
      ssize_t bytes_read = splice (প্ৰবেশ_desc, রিক্ত, pipefd[1], রিক্ত,
                                   pipe_size, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      in_ok = 0 <= bytes_read || ! some_copied;
      যদি (bytes_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      যদি (bytes_read <= 0)
        গচ্ছ done;
      /* We need to drain the intermediate pipe to standard output.  */
      যেতিয়ালৈ (0 < bytes_read)
        {
          ssize_t bytes_written = splice (pipefd[0], রিক্ত, STDOUT_FILENO, রিক্ত,
                                          pipe_size, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          যদি (bytes_written < 0)
            {
              যদি (some_copied)
                out_ok = false;
              অন্যথা
                {
                  ৱর্ণ buf[BUFSIZ];
                  যেতিয়ালৈ (0 < bytes_read)
                    {
                      ssize_t গুনতি = MIN (bytes_read, পরিমাণ buf);
                      ssize_t n_read = পঠ (pipefd[0], buf, গুনতি);
                      /* Failure not associated with in or out.  */
                      in_ok = out_ok = 0 <= n_read;
                      যদি (n_read <= 0)
                        গচ্ছ done;
                      যদি (full_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      bytes_read -= n_read;
                    }
                }
            }
          যদি (bytes_written <= 0)
            গচ্ছ done;
          some_copied = true;
          bytes_read -= bytes_written;
        }
    }

 done:
  যদি (! in_ok && ! out_ok)
    {
      /* Recreate the pipe on internal error.  */
      পূর্ণাঙ্ক saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_size = 0;
      error (0, errno, "%s", _("splice error"));
    }
  অন্যথা যদি (! in_ok)
    error (0, errno, "%s", quotef (infile));
  অন্যথা যদি (! out_ok)
    write_error ();
#endif

  ঘূৰাই (in_ok && out_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

স্থির ৱর্ণ *
ensure_buf_size (ৱর্ণ *buf, idx_t *buf_alloc, idx_t alignment, idx_t size)
{
  affirm (buf != রিক্ত || *buf_alloc < size);

  যদি (*buf_alloc < size)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, size);
      *buf_alloc = size;
    }

  ঘূৰাই buf;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  সংরচনা stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool সংখ্যা = false;
  bool সংখ্যা_nonblank = false;
  bool squeeze_blank = false;
  bool show_ends = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  পূর্ণাঙ্ক file_open_mode = O_RDONLY;

  স্থির সংরচনা option ধ্রুৱ long_options[] =
  {
    {"number-nonblank", no_argument, রিক্ত, 'b'},
    {"number", no_argument, রিক্ত, 'n'},
    {"squeeze-blank", no_argument, রিক্ত, 's'},
    {"show-nonprinting", no_argument, রিক্ত, 'v'},
    {"show-ends", no_argument, রিক্ত, 'E'},
    {"show-tabs", no_argument, রিক্ত, 'T'},
    {"show-all", no_argument, রিক্ত, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {রিক্ত, 0, রিক্ত, 0}
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

  পূর্ণাঙ্ক c;
  যেতিয়ালৈ ((c = getopt_long (argc, argv, "benstuvAET", long_options, রিক্ত))
         != -1)
    {
      ৱিকল্প (c)
        {
        পক্ষ 'b':
          সংখ্যা = true;
          সংখ্যা_nonblank = true;
          ভঙ্গ;

        পক্ষ 'e':
          show_ends = true;
          show_nonprinting = true;
          ভঙ্গ;

        পক্ষ 'n':
          সংখ্যা = true;
          ভঙ্গ;

        পক্ষ 's':
          squeeze_blank = true;
          ভঙ্গ;

        পক্ষ 't':
          show_tabs = true;
          show_nonprinting = true;
          ভঙ্গ;

        পক্ষ 'u':
          /* We provide the -u feature unconditionally.  */
          ভঙ্গ;

        পক্ষ 'v':
          show_nonprinting = true;
          ভঙ্গ;

        পক্ষ 'A':
          show_nonprinting = true;
          show_ends = true;
          show_tabs = true;
          ভঙ্গ;

        পক্ষ 'E':
          show_ends = true;
          ভঙ্গ;

        পক্ষ 'T':
          show_tabs = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  যদি (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  পূর্ণাঙ্ক out_flags = -2;

  /* True if the output is a regular file.  */
  bool out_isreg = S_ISREG (ostat_buf.st_mode) != 0;

  যদি (! (সংখ্যা || show_ends || squeeze_blank))
    {
      file_open_mode |= O_দ্বিমিক;
      xset_দ্বিমিক_mode (STDOUT_FILENO, O_দ্বিমিক);
    }

  /* Main loop.  */

  infile = "-";
  পূর্ণাঙ্ক argind = optind;
  bool ok = true;
  idx_t page_size = getpagesize ();
  ৱর্ণ *inbuf = রিক্ত;
  ৱর্ণ *outbuf = রিক্ত;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  কৰা
    {
      যদি (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      যদি (reading_stdin)
        {
          have_read_stdin = true;
          প্ৰবেশ_desc = STDIN_FILENO;
          যদি (file_open_mode & O_দ্বিমিক)
            xset_দ্বিমিক_mode (STDIN_FILENO, O_দ্বিমিক);
        }
      অন্যথা
        {
          প্ৰবেশ_desc = open (infile, file_open_mode);
          যদি (প্ৰবেশ_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              অনুৱর্ত;
            }
        }

      সংরচনা stat istat_buf;
      যদি (fstat (প্ৰবেশ_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          গচ্ছ contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (প্ৰবেশ_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      যদি (! (S_ISFIFO (istat_buf.st_mode) || S_ISSOCK (istat_buf.st_mode)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t in_pos = lseek (প্ৰবেশ_desc, 0, SEEK_CUR);
          যদি (0 <= in_pos)
            {
              যদি (out_flags < -1)
                out_flags = fcntl (STDOUT_FILENO, F_GETFL);
              পূর্ণাঙ্ক whence = (0 <= out_flags && out_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              যদি (in_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  গচ্ছ contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      যদি (! (সংখ্যা || show_ends || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          পূর্ণাঙ্ক copy_cat_status =
            out_isreg && S_ISREG (istat_buf.st_mode) ? copy_cat () : 0;
          যদি (copy_cat_status != 0)
            ok &= 0 < copy_cat_status;
          অন্যথা
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              পূর্ণাঙ্ক splice_cat_status = ((usable_st_size (&istat_buf)
                                        && istat_buf.st_size <= 32768)
                                       ? 0 : splice_cat ());
              যদি (splice_cat_status != 0)
                ok &= 0 < splice_cat_status;
              অন্যথা
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_size (inbuf, &inbuf_alloc,
                                           page_size, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      অন্যথা
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
          যদি (ckd_mul (&bufsize, insize, 4)
              || ckd_add (&bufsize, bufsize, outsize)
              || ckd_add (&bufsize, bufsize, সাৰি_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_size (outbuf, &outbuf_alloc,
                                    page_size, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, সংখ্যা, সংখ্যা_nonblank, show_ends,
                     squeeze_blank);
        }

    contin:
      যদি (!reading_stdin && close (প্ৰবেশ_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  যেতিয়ালৈ (++argind < argc);

  যদি (pending_cr)
    {
      যদি (full_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#ifdef lint
  alignfree (outbuf);
  alignfree (inbuf);
#endif

  যদি (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  ঘূৰাই ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
