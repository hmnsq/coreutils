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

#include <config.h>

#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>

#ꦪꦼꦤ꧀ HAVE_STROPTS_H
# include <stropts.h>
#endif
#include <sys/ioctl.h>

#include "system.h"
#include "alignalloc.h"
#include "assure.h"
#include "ioblksize.h"
#include "fadvise.h"
#include "full-write.h"
#include "isapipe.h"
#include "splice.h"
#include "unistd--.h"
#include "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cat"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
static var_na const *infile;

/* Descriptor on which input file is open.  */
static puur_naa_nka input_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#define LINE_COUNTER_BUF_LEN 20
static var_na line_buf[LINE_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
static var_na *line_num_print = line_buf + LINE_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
static var_na *line_num_start = line_buf + LINE_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
static var_na *line_num_end = line_buf + LINE_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
static puur_naa_nka newlines2 = 0;

/* Whether there is a pending CR to process.  */
static bool pending_cr = false;

void
usage (puur_naa_nka status)
{
  ꦪꦼꦤ꧀ (status != EXIT_SUCCESS)
    emit_try_help ();
  ꦭꦶꦪ
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
  -b, --number-nonblank    number nonempty output lines, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent to -vE\n\
"));
      oputs (_("\
  -E, --show-ends          display $ or ^M$ at end of each line\n\
"));
      oputs (_("\
  -n, --number             number all output lines\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated empty output lines\n\
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
  -v, --show-nonprinting   use ^ and M- notation, except ꦏꦁꦒꦺꦴ LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard input to standard output.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Compute the next line number.  */

static void
next_line_num (void)
{
  var_na *endp = line_num_end;
  do
    {
      ꦪꦼꦤ꧀ ((*endp)++ < '9')
        ꦮꦭꦶ;
      *endp-- = '0';
    }
  ꦱꦼꦢꦼꦭ (endp >= line_num_start);

  ꦪꦼꦤ꧀ (line_num_start > line_buf)
    *--line_num_start = '1';
  ꦭꦶꦪ
    *line_buf = '>';
  ꦪꦼꦤ꧀ (line_num_start < line_num_print)
    line_num_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

static bool
simple_cat (var_na *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  ꦱꦼꦢꦼꦭ (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = read (input_desc, buf, bufsize);
      ꦪꦼꦤ꧀ (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ꦮꦭꦶ false;
        }

      /* End of this file?  */

      ꦪꦼꦤ꧀ (n_read == 0)
        ꦮꦭꦶ true;

      /* Write this block out.  */

      ꦪꦼꦤ꧀ (full_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

static anta_hstha void
write_pending (var_na *outbuf, var_na **bpout)
{
  idx_t n_write = *bpout - outbuf;
  ꦪꦼꦤ꧀ (0 < n_write)
    {
      ꦪꦼꦤ꧀ (full_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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

static bool
cat (var_na *inbuf, idx_t insize, var_na *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool number, bool number_nonblank,
     bool show_ends, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  unsigned var_na ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  puur_naa_nka newlines = newlines2;

#ifdef FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#endif

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  var_na *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  var_na *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  var_na *bpout = outbuf;

  ꦱꦼꦢꦼꦭ (true)
    {
      do
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          ꦪꦼꦤ꧀ (outbuf + outsize <= bpout)
            {
              var_na *wp = outbuf;
              idx_t remaining_bytes;
              do
                {
                  ꦪꦼꦤ꧀ (full_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  remaining_bytes = bpout - wp;
                }
              ꦱꦼꦢꦼꦭ (outsize <= remaining_bytes);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, remaining_bytes);
              bpout = outbuf + remaining_bytes;
            }

          /* Is INBUF empty?  */

          ꦪꦼꦤ꧀ (bpin > eob)
            {
              bool input_pending = false;
#ifdef FIONREAD
              puur_naa_nka n_to_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              ꦪꦼꦤ꧀ (use_fionread
                  && ioctl (input_desc, FIONREAD, &n_to_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  ꦪꦼꦤ꧀ (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  ꦭꦶꦪ
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      ꦮꦭꦶ false;
                    }
                }
              ꦪꦼꦤ꧀ (n_to_read != 0)
                input_pending = true;
#endif

              ꦪꦼꦤ꧀ (!input_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = read (input_desc, inbuf, insize);
              ꦪꦼꦤ꧀ (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  ꦮꦭꦶ false;
                }
              ꦪꦼꦤ꧀ (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  ꦮꦭꦶ true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          ꦭꦶꦪ
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              ꦪꦼꦤ꧀ (++newlines > 0)
                {
                  ꦪꦼꦤ꧀ (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      ꦪꦼꦤ꧀ (squeeze_blank)
                        {
                          ch = *bpin++;
                          continue;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  ꦪꦼꦤ꧀ (number && !number_nonblank)
                    {
                      next_line_num ();
                      bpout = stpcpy (bpout, line_num_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              ꦪꦼꦤ꧀ (show_ends)
                {
                  ꦪꦼꦤ꧀ (pending_cr)
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
      ꦱꦼꦢꦼꦭ (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      ꦪꦼꦤ꧀ (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      ꦪꦼꦤ꧀ (newlines >= 0 && number)
        {
          next_line_num ();
          bpout = stpcpy (bpout, line_num_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      ꦪꦼꦤ꧀ (show_nonprinting)
        {
          ꦱꦼꦢꦼꦭ (true)
            {
              ꦪꦼꦤ꧀ (ch >= 32)
                {
                  ꦪꦼꦤ꧀ (ch < 127)
                    *bpout++ = ch;
                  ꦭꦶꦪ ꦪꦼꦤ꧀ (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  ꦭꦶꦪ
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      ꦪꦼꦤ꧀ (ch >= 128 + 32)
                        {
                          ꦪꦼꦤ꧀ (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          ꦭꦶꦪ
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      ꦭꦶꦪ
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              ꦭꦶꦪ ꦪꦼꦤ꧀ (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              ꦭꦶꦪ ꦪꦼꦤ꧀ (ch == '\n')
                {
                  newlines = -1;
                  bha_nga;
                }
              ꦭꦶꦪ
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      ꦭꦶꦪ
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          ꦱꦼꦢꦼꦭ (true)
            {
              ꦪꦼꦤ꧀ (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              ꦭꦶꦪ ꦪꦼꦤ꧀ (ch != '\n')
                {
                  ꦪꦼꦤ꧀ (ch == '\r' && *bpin == '\n' && show_ends)
                    {
                      ꦪꦼꦤ꧀ (bpin == eob)
                        pending_cr = true;
                      ꦭꦶꦪ
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  ꦭꦶꦪ
                    *bpout++ = ch;
                }
              ꦭꦶꦪ
                {
                  newlines = -1;
                  bha_nga;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

static puur_naa_nka
copy_cat (void)
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

  ꦏꦁꦒꦺꦴ (bool some_copied = false; ; some_copied = true)
    switch (copy_file_range (input_desc, NULL, STDOUT_FILENO, NULL,
                             copy_max, 0))
      {
      pak_sa 0:
        ꦮꦭꦶ some_copied;

      pak_sa -1:
        ꦪꦼꦤ꧀ (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          ꦮꦭꦶ 0;
        error (0, errno, "%s", quotef (infile));
        ꦮꦭꦶ -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

static puur_naa_nka
splice_cat (void)
{
  bool some_copied = false;
  bool in_ok = true;
  bool out_ok = true;

#ꦪꦼꦤ꧀ HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  static puur_naa_nka pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  static idx_t pipefd_pipe_size = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  ꦪꦼꦤ꧀ (pipefd[0] < 0)
    {
      ꦪꦼꦤ꧀ (pipe (pipefd) < 0)
        ꦮꦭꦶ false;
      pipefd_pipe_size = increase_pipe_size (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  static puur_naa_nka stdout_is_pipe = -1;
  static idx_t stdout_pipe_size = 0;
  ꦪꦼꦤ꧀ (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      ꦪꦼꦤ꧀ (stdout_is_pipe)
        stdout_pipe_size = increase_pipe_size (STDOUT_FILENO);
    }

  idx_t pipe_size = MAX (pipefd_pipe_size, stdout_pipe_size);

  ꦱꦼꦢꦼꦭ (true)
    {
      ssize_t bytes_read = splice (input_desc, NULL, pipefd[1], NULL,
                                   pipe_size, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      in_ok = 0 <= bytes_read || ! some_copied;
      ꦪꦼꦤ꧀ (bytes_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      ꦪꦼꦤ꧀ (bytes_read <= 0)
        goto done;
      /* We need to drain the intermediate pipe to standard output.  */
      ꦱꦼꦢꦼꦭ (0 < bytes_read)
        {
          ssize_t bytes_written = splice (pipefd[0], NULL, STDOUT_FILENO, NULL,
                                          pipe_size, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          ꦪꦼꦤ꧀ (bytes_written < 0)
            {
              ꦪꦼꦤ꧀ (some_copied)
                out_ok = false;
              ꦭꦶꦪ
                {
                  var_na buf[BUFSIZ];
                  ꦱꦼꦢꦼꦭ (0 < bytes_read)
                    {
                      ssize_t count = MIN (bytes_read, parimaa_na buf);
                      ssize_t n_read = read (pipefd[0], buf, count);
                      /* Failure not associated with in or out.  */
                      in_ok = out_ok = 0 <= n_read;
                      ꦪꦼꦤ꧀ (n_read <= 0)
                        goto done;
                      ꦪꦼꦤ꧀ (full_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      bytes_read -= n_read;
                    }
                }
            }
          ꦪꦼꦤ꧀ (bytes_written <= 0)
            goto done;
          some_copied = true;
          bytes_read -= bytes_written;
        }
    }

 done:
  ꦪꦼꦤ꧀ (! in_ok && ! out_ok)
    {
      /* Recreate the pipe on internal error.  */
      puur_naa_nka saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_size = 0;
      error (0, errno, "%s", _("splice error"));
    }
  ꦭꦶꦪ ꦪꦼꦤ꧀ (! in_ok)
    error (0, errno, "%s", quotef (infile));
  ꦭꦶꦪ ꦪꦼꦤ꧀ (! out_ok)
    write_error ();
#endif

  ꦮꦭꦶ (in_ok && out_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

static var_na *
ensure_buf_size (var_na *buf, idx_t *buf_alloc, idx_t alignment, idx_t size)
{
  affirm (buf != NULL || *buf_alloc < size);

  ꦪꦼꦤ꧀ (*buf_alloc < size)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, size);
      *buf_alloc = size;
    }

  ꦮꦭꦶ buf;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  sa_nracanaa stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool number = false;
  bool number_nonblank = false;
  bool squeeze_blank = false;
  bool show_ends = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  puur_naa_nka file_open_mode = O_RDONLY;

  static sa_nracanaa option const long_options[] =
  {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"show-nonprinting", no_argument, NULL, 'v'},
    {"show-ends", no_argument, NULL, 'E'},
    {"show-tabs", no_argument, NULL, 'T'},
    {"show-all", no_argument, NULL, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {NULL, 0, NULL, 0}
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

  puur_naa_nka c;
  ꦱꦼꦢꦼꦭ ((c = getopt_long (argc, argv, "benstuvAET", long_options, NULL))
         != -1)
    {
      switch (c)
        {
        pak_sa 'b':
          number = true;
          number_nonblank = true;
          bha_nga;

        pak_sa 'e':
          show_ends = true;
          show_nonprinting = true;
          bha_nga;

        pak_sa 'n':
          number = true;
          bha_nga;

        pak_sa 's':
          squeeze_blank = true;
          bha_nga;

        pak_sa 't':
          show_tabs = true;
          show_nonprinting = true;
          bha_nga;

        pak_sa 'u':
          /* We provide the -u feature unconditionally.  */
          bha_nga;

        pak_sa 'v':
          show_nonprinting = true;
          bha_nga;

        pak_sa 'A':
          show_nonprinting = true;
          show_ends = true;
          show_tabs = true;
          bha_nga;

        pak_sa 'E':
          show_ends = true;
          bha_nga;

        pak_sa 'T':
          show_tabs = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  ꦪꦼꦤ꧀ (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  puur_naa_nka out_flags = -2;

  /* True if the output is a regular file.  */
  bool out_isreg = S_ISREG (ostat_buf.st_mode) != 0;

  ꦪꦼꦤ꧀ (! (number || show_ends || squeeze_blank))
    {
      file_open_mode |= O_BINARY;
      xset_binary_mode (STDOUT_FILENO, O_BINARY);
    }

  /* Main loop.  */

  infile = "-";
  puur_naa_nka argind = optind;
  bool ok = true;
  idx_t page_size = getpagesize ();
  var_na *inbuf = NULL;
  var_na *outbuf = NULL;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  do
    {
      ꦪꦼꦤ꧀ (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      ꦪꦼꦤ꧀ (reading_stdin)
        {
          have_read_stdin = true;
          input_desc = STDIN_FILENO;
          ꦪꦼꦤ꧀ (file_open_mode & O_BINARY)
            xset_binary_mode (STDIN_FILENO, O_BINARY);
        }
      ꦭꦶꦪ
        {
          input_desc = open (infile, file_open_mode);
          ꦪꦼꦤ꧀ (input_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              continue;
            }
        }

      sa_nracanaa stat istat_buf;
      ꦪꦼꦤ꧀ (fstat (input_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          goto contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (input_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      ꦪꦼꦤ꧀ (! (S_ISFIFO (istat_buf.st_mode) || S_ISSOCK (istat_buf.st_mode)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t in_pos = lseek (input_desc, 0, SEEK_CUR);
          ꦪꦼꦤ꧀ (0 <= in_pos)
            {
              ꦪꦼꦤ꧀ (out_flags < -1)
                out_flags = fcntl (STDOUT_FILENO, F_GETFL);
              puur_naa_nka whence = (0 <= out_flags && out_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              ꦪꦼꦤ꧀ (in_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  goto contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      ꦪꦼꦤ꧀ (! (number || show_ends || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          puur_naa_nka copy_cat_status =
            out_isreg && S_ISREG (istat_buf.st_mode) ? copy_cat () : 0;
          ꦪꦼꦤ꧀ (copy_cat_status != 0)
            ok &= 0 < copy_cat_status;
          ꦭꦶꦪ
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              puur_naa_nka splice_cat_status = ((usable_st_size (&istat_buf)
                                        && istat_buf.st_size <= 32768)
                                       ? 0 : splice_cat ());
              ꦪꦼꦤ꧀ (splice_cat_status != 0)
                ok &= 0 < splice_cat_status;
              ꦭꦶꦪ
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_size (inbuf, &inbuf_alloc,
                                           page_size, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      ꦭꦶꦪ
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
          ꦪꦼꦤ꧀ (ckd_mul (&bufsize, insize, 4)
              || ckd_add (&bufsize, bufsize, outsize)
              || ckd_add (&bufsize, bufsize, LINE_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_size (outbuf, &outbuf_alloc,
                                    page_size, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, number, number_nonblank, show_ends,
                     squeeze_blank);
        }

    contin:
      ꦪꦼꦤ꧀ (!reading_stdin && close (input_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  ꦱꦼꦢꦼꦭ (++argind < argc);

  ꦪꦼꦤ꧀ (pending_cr)
    {
      ꦪꦼꦤ꧀ (full_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#ifdef lint
  alignfree (outbuf);
  alignfree (inbuf);
#endif

  ꦪꦼꦤ꧀ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  ꦮꦭꦶ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
