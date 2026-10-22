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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#എങ്കിൽ HAVE_STROPTS_H
# include <stropts.h>
#endif
#അന്തര്ഭാവ <sys/ioctl.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "alignalloc.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "ioblksize.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "full-write.h"
#അന്തര്ഭാവ "isapipe.h"
#അന്തര്ഭാവ "splice.h"
#അന്തര്ഭാവ "unistd--.h"
#അന്തര്ഭാവ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cat"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Name of input file.  May be "-".  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *infile;

/* Descriptor on which input file is open.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ നിവേശം_desc;

/* Buffer for line numbers.
   An 11 digit counter may overflow within an hour on a P2/466,
   an 18 digit counter needs about 1000y */
#define വരി_COUNTER_BUF_LEN 20
സ്ഥിതം അക്ഷരം വരി_buf[വരി_COUNTER_BUF_LEN] =
  {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
    '\t', '\0'
  };

/* Position in 'line_buf' where printing starts.  This will not change
   unless the number of lines is larger than 999999.  */
സ്ഥിതം അക്ഷരം *വരി_അക്കം_print = വരി_buf + വരി_COUNTER_BUF_LEN - 8;

/* Position of the first digit in 'line_buf'.  */
സ്ഥിതം അക്ഷരം *വരി_അക്കം_start = വരി_buf + വരി_COUNTER_BUF_LEN - 3;

/* Position of the last digit in 'line_buf'.  */
സ്ഥിതം അക്ഷരം *വരി_അക്കം_end = വരി_buf + വരി_COUNTER_BUF_LEN - 3;

/* Preserves the 'cat' function's local 'newlines' between invocations.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ newlines2 = 0;

/* Whether there is a pending CR to process.  */
സ്ഥിതം bool pending_cr = false;

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
  -b, --സംഖ്യ-nonblank    സംഖ്യ nonempty output വരികൾ, overrides -n\n\
"));
      oputs (_("\
  -e                       equivalent to -vE\n\
"));
      oputs (_("\
  -E, --show-ends          display $ or ^M$ at end of each വരി\n\
"));
      oputs (_("\
  -n, --സംഖ്യ             സംഖ്യ all output വരികൾ\n\
"));
      oputs (_("\
  -s, --squeeze-blank      suppress repeated empty output വരികൾ\n\
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
  -v, --show-nonprinting   use ^ and M- notation, except വേണ്ടി LFD and TAB\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
Examples:\n\
  %s f - g  Output f's contents, then standard input, then g's contents.\n\
  %s        Copy standard നിവേശം to standard output.\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Compute the next line number.  */

സ്ഥിതം ശൂന്യം
next_വരി_അക്കം (ശൂന്യം)
{
  അക്ഷരം *endp = വരി_അക്കം_end;
  ചെയ്യുക
    {
      എങ്കിൽ ((*endp)++ < '9')
        തിരികെ;
      *endp-- = '0';
    }
  ആകുമ്പോൾ (endp >= വരി_അക്കം_start);

  എങ്കിൽ (വരി_അക്കം_start > വരി_buf)
    *--വരി_അക്കം_start = '1';
  അല്ലെങ്കിൽ
    *വരി_buf = '>';
  എങ്കിൽ (വരി_അക്കം_start < വരി_അക്കം_print)
    വരി_അക്കം_print--;
}

/* Plain cat.  Copy the file behind 'input_desc' to STDOUT_FILENO.
   BUF (of size BUFSIZE) is the I/O buffer, used by reads and writes.
   Return true if successful.  */

സ്ഥിതം bool
simple_cat (അക്ഷരം *buf, idx_t bufsize)
{
  /* Loop until the end of the file.  */

  ആകുമ്പോൾ (true)
    {
      /* Read a block of input.  */

      ssize_t n_read = പഠ (നിവേശം_desc, buf, bufsize);
      എങ്കിൽ (n_read < 0)
        {
          error (0, errno, "%s", quotef (infile));
          തിരികെ false;
        }

      /* End of this file?  */

      എങ്കിൽ (n_read == 0)
        തിരികെ true;

      /* Write this block out.  */

      എങ്കിൽ (full_write (STDOUT_FILENO, buf, n_read) != n_read)
        write_error ();
    }
}

/* Write any pending output to STDOUT_FILENO.
   Pending is defined to be the *BPOUT - OUTBUF bytes starting at OUTBUF.
   Then set *BPOUT to OUTPUT if it's not already that value.  */

സ്ഥിതം വരിയിൽ ശൂന്യം
write_pending (അക്ഷരം *outbuf, അക്ഷരം **bpout)
{
  idx_t n_write = *bpout - outbuf;
  എങ്കിൽ (0 < n_write)
    {
      എങ്കിൽ (full_write (STDOUT_FILENO, outbuf, n_write) != n_write)
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

സ്ഥിതം bool
cat (അക്ഷരം *inbuf, idx_t insize, അക്ഷരം *outbuf, idx_t outsize,
     bool show_nonprinting, bool show_tabs, bool സംഖ്യ, bool സംഖ്യ_nonblank,
     bool show_ends, bool squeeze_blank)
{
  /* Last character read from the input buffer.  */
  unsigned അക്ഷരം ch;

  /* Determines how many consecutive newlines there have been in the
     input.  0 newlines makes NEWLINES -1, 1 newline makes NEWLINES 1,
     etc.  Initially 0 to indicate that we are at the beginning of a
     new line.  The "state" of the procedure is determined by
     NEWLINES.  */
  പൂർണ്ണസംഖ്യ newlines = newlines2;

#ifdef FIONREAD
  /* If nonzero, use the FIONREAD ioctl, as an optimization.
     (On Ultrix, it is not supported on NFS file systems.)  */
  bool use_fionread = true;
#endif

  /* The inbuf pointers are initialized so that BPIN > EOB, and thereby input
     is read immediately.  */

  /* Pointer to the first non-valid byte in the input buffer, i.e., the
     current end of the buffer.  */
  അക്ഷരം *eob = inbuf;

  /* Pointer to the next character in the input buffer.  */
  അക്ഷരം *bpin = eob + 1;

  /* Pointer to the position where the next character shall be written.  */
  അക്ഷരം *bpout = outbuf;

  ആകുമ്പോൾ (true)
    {
      ചെയ്യുക
        {
          /* Write if there are at least OUTSIZE bytes in OUTBUF.  */

          എങ്കിൽ (outbuf + outsize <= bpout)
            {
              അക്ഷരം *wp = outbuf;
              idx_t remaining_bytes;
              ചെയ്യുക
                {
                  എങ്കിൽ (full_write (STDOUT_FILENO, wp, outsize) != outsize)
                    write_error ();
                  wp += outsize;
                  remaining_bytes = bpout - wp;
                }
              ആകുമ്പോൾ (outsize <= remaining_bytes);

              /* Move the remaining bytes to the beginning of the
                 buffer.  */

              memmove (outbuf, wp, remaining_bytes);
              bpout = outbuf + remaining_bytes;
            }

          /* Is INBUF empty?  */

          എങ്കിൽ (bpin > eob)
            {
              bool നിവേശം_pending = false;
#ifdef FIONREAD
              പൂർണ്ണസംഖ്യ n_to_read = 0;

              /* Is there any input to read immediately?
                 If not, we are about to wait,
                 so write all buffered output before waiting.  */

              എങ്കിൽ (use_fionread
                  && ioctl (നിവേശം_desc, FIONREAD, &n_to_read) < 0)
                {
                  /* Ultrix returns EOPNOTSUPP on NFS;
                     HP-UX returns ENOTTY on pipes.
                     SunOS returns EINVAL and
                     More/BSD returns ENODEV on special files
                     like /dev/null.
                     Irix-5 returns ENOSYS on pipes.  */
                  എങ്കിൽ (errno == EOPNOTSUPP || errno == ENOTTY
                      || errno == EINVAL || errno == ENODEV
                      || errno == ENOSYS)
                    use_fionread = false;
                  അല്ലെങ്കിൽ
                    {
                      error (0, errno, _("cannot do ioctl on %s"),
                             quoteaf (infile));
                      newlines2 = newlines;
                      തിരികെ false;
                    }
                }
              എങ്കിൽ (n_to_read != 0)
                നിവേശം_pending = true;
#endif

              എങ്കിൽ (!നിവേശം_pending)
                write_pending (outbuf, &bpout);

              /* Read more input into INBUF.  */

              ssize_t n_read = പഠ (നിവേശം_desc, inbuf, insize);
              എങ്കിൽ (n_read < 0)
                {
                  error (0, errno, "%s", quotef (infile));
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  തിരികെ false;
                }
              എങ്കിൽ (n_read == 0)
                {
                  write_pending (outbuf, &bpout);
                  newlines2 = newlines;
                  തിരികെ true;
                }

              /* Update the pointers and insert a sentinel at the buffer
                 end.  */

              bpin = inbuf;
              eob = bpin + n_read;
              *eob = '\n';
            }
          അല്ലെങ്കിൽ
            {
              /* It was a real (not a sentinel) newline.  */

              /* Was the last line empty?
                 (i.e., have two or more consecutive newlines been read?)  */

              എങ്കിൽ (++newlines > 0)
                {
                  എങ്കിൽ (newlines >= 2)
                    {
                      /* Limit this to 2 here.  Otherwise, with lots of
                         consecutive newlines, the counter could wrap
                         around at INT_MAX.  */
                      newlines = 2;

                      /* Are multiple adjacent empty lines to be substituted
                         by single ditto (-s), and this was the second empty
                         line?  */
                      എങ്കിൽ (squeeze_blank)
                        {
                          ch = *bpin++;
                          തുടരുക;
                        }
                    }

                  /* Are line numbers to be written at empty lines (-n)?  */

                  എങ്കിൽ (സംഖ്യ && !സംഖ്യ_nonblank)
                    {
                      next_വരി_അക്കം ();
                      bpout = stpcpy (bpout, വരി_അക്കം_print);
                    }
                }

              /* Output a currency symbol if requested (-e).  */
              എങ്കിൽ (show_ends)
                {
                  എങ്കിൽ (pending_cr)
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
      ആകുമ്പോൾ (ch == '\n');

      /* Here CH cannot contain a newline character.  */

      എങ്കിൽ (pending_cr)
        {
          *bpout++ = '\r';
          pending_cr = false;
        }

      /* Are we at the beginning of a line, and line numbers are requested?  */

      എങ്കിൽ (newlines >= 0 && സംഖ്യ)
        {
          next_വരി_അക്കം ();
          bpout = stpcpy (bpout, വരി_അക്കം_print);
        }

      /* The loops below continue until a newline character is found,
         which means that the buffer is empty or that a proper newline
         has been found.  */

      /* If quoting, i.e., at least one of -v, -e, or -t specified,
         scan for chars that need conversion.  */
      എങ്കിൽ (show_nonprinting)
        {
          ആകുമ്പോൾ (true)
            {
              എങ്കിൽ (ch >= 32)
                {
                  എങ്കിൽ (ch < 127)
                    *bpout++ = ch;
                  അല്ലെങ്കിൽ എങ്കിൽ (ch == 127)
                    {
                      *bpout++ = '^';
                      *bpout++ = '?';
                    }
                  അല്ലെങ്കിൽ
                    {
                      *bpout++ = 'M';
                      *bpout++ = '-';
                      എങ്കിൽ (ch >= 128 + 32)
                        {
                          എങ്കിൽ (ch < 128 + 127)
                            *bpout++ = ch - 128;
                          അല്ലെങ്കിൽ
                            {
                              *bpout++ = '^';
                              *bpout++ = '?';
                            }
                        }
                      അല്ലെങ്കിൽ
                        {
                          *bpout++ = '^';
                          *bpout++ = ch - 128 + 64;
                        }
                    }
                }
              അല്ലെങ്കിൽ എങ്കിൽ (ch == '\t' && !show_tabs)
                *bpout++ = '\t';
              അല്ലെങ്കിൽ എങ്കിൽ (ch == '\n')
                {
                  newlines = -1;
                  നിർത്തുക;
                }
              അല്ലെങ്കിൽ
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }

              ch = *bpin++;
            }
        }
      അല്ലെങ്കിൽ
        {
          /* Not quoting, neither of -v, -e, or -t specified.  */
          ആകുമ്പോൾ (true)
            {
              എങ്കിൽ (ch == '\t' && show_tabs)
                {
                  *bpout++ = '^';
                  *bpout++ = ch + 64;
                }
              അല്ലെങ്കിൽ എങ്കിൽ (ch != '\n')
                {
                  എങ്കിൽ (ch == '\r' && *bpin == '\n' && show_ends)
                    {
                      എങ്കിൽ (bpin == eob)
                        pending_cr = true;
                      അല്ലെങ്കിൽ
                        {
                          *bpout++ = '^';
                          *bpout++ = 'M';
                        }
                    }
                  അല്ലെങ്കിൽ
                    *bpout++ = ch;
                }
              അല്ലെങ്കിൽ
                {
                  newlines = -1;
                  നിർത്തുക;
                }

              ch = *bpin++;
            }
        }
    }
}

/* Copy data from input to output using copy_file_range if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
copy_cat (ശൂന്യം)
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

  വേണ്ടി (bool some_copied = false; ; some_copied = true)
    തിരഞ്ഞെടുപ്പ് (copy_file_range (നിവേശം_desc, ശൂന്യ, STDOUT_FILENO, ശൂന്യ,
                             copy_max, 0))
      {
      അവസ്ഥ 0:
        തിരികെ some_copied;

      അവസ്ഥ -1:
        എങ്കിൽ (errno == ENOSYS || is_ENOTSUP (errno) || errno == EINVAL
            || errno == EBADF || errno == EXDEV || errno == ETXTBSY
            || errno == EPERM || errno == EFBIG)
          തിരികെ 0;
        error (0, errno, "%s", quotef (infile));
        തിരികെ -1;
      }
}

/* Copy data from input to output using splice if possible.
   Return 1 if successful, 0 if ordinary read+write should be tried,
   -1 if a serious problem has been diagnosed.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
splice_cat (ശൂന്യം)
{
  bool some_copied = false;
  bool in_ok = true;
  bool out_ok = true;

#എങ്കിൽ HAVE_SPLICE

  /* If PIPEFD[0] is a non-negative value, we have an open pipe from a
     previous call to this function.  At the start of this function the
     pipe will always be empty.  */
  സ്ഥിതം പൂർണ്ണസംഖ്യ pipefd[2] = { -1, -1 };

  /* The size of the pipe referred to by PIPEFD.  */
  സ്ഥിതം idx_t pipefd_pipe_size = 0;

  /* Create an intermediate pipe if it is not already open.
     Even if both input and output are pipes,
     so that read and write errors can be distinguished.  */
  എങ്കിൽ (pipefd[0] < 0)
    {
      എങ്കിൽ (pipe (pipefd) < 0)
        തിരികെ false;
      pipefd_pipe_size = increase_pipe_size (pipefd[1]);
    }

  /* Increase the size of the pipe referred to by standard output.  */
  സ്ഥിതം പൂർണ്ണസംഖ്യ stdout_is_pipe = -1;
  സ്ഥിതം idx_t stdout_pipe_size = 0;
  എങ്കിൽ (stdout_is_pipe == -1)
    {
      stdout_is_pipe = 0 < isapipe (STDOUT_FILENO);
      എങ്കിൽ (stdout_is_pipe)
        stdout_pipe_size = increase_pipe_size (STDOUT_FILENO);
    }

  idx_t pipe_size = MAX (pipefd_pipe_size, stdout_pipe_size);

  ആകുമ്പോൾ (true)
    {
      ssize_t bytes_read = splice (നിവേശം_desc, ശൂന്യ, pipefd[1], ശൂന്യ,
                                   pipe_size, 0);
      /* If we successfully splice'd input previously, assume that any
         subsequent error is fatal.  If not, then fall back to read
         and write.  */
      in_ok = 0 <= bytes_read || ! some_copied;
      എങ്കിൽ (bytes_read == 0)
        some_copied = true;  /* Indicate splice complete.  */
      എങ്കിൽ (bytes_read <= 0)
        പോകുക done;
      /* We need to drain the intermediate pipe to standard output.  */
      ആകുമ്പോൾ (0 < bytes_read)
        {
          ssize_t bytes_written = splice (pipefd[0], ശൂന്യ, STDOUT_FILENO, ശൂന്യ,
                                          pipe_size, 0);
          /* If we successfully splice'd output, assume any subsequent
             error is fatal.  If not, than drain the intermediate pipe and
             continue using read and write.  */
          എങ്കിൽ (bytes_written < 0)
            {
              എങ്കിൽ (some_copied)
                out_ok = false;
              അല്ലെങ്കിൽ
                {
                  അക്ഷരം buf[BUFSIZ];
                  ആകുമ്പോൾ (0 < bytes_read)
                    {
                      ssize_t എണ്ണം = MIN (bytes_read, വലുപ്പം buf);
                      ssize_t n_read = പഠ (pipefd[0], buf, എണ്ണം);
                      /* Failure not associated with in or out.  */
                      in_ok = out_ok = 0 <= n_read;
                      എങ്കിൽ (n_read <= 0)
                        പോകുക done;
                      എങ്കിൽ (full_write (STDOUT_FILENO, buf, n_read) != n_read)
                        write_error ();
                      bytes_read -= n_read;
                    }
                }
            }
          എങ്കിൽ (bytes_written <= 0)
            പോകുക done;
          some_copied = true;
          bytes_read -= bytes_written;
        }
    }

 done:
  എങ്കിൽ (! in_ok && ! out_ok)
    {
      /* Recreate the pipe on internal error.  */
      പൂർണ്ണസംഖ്യ saved_errno = errno;
      close (pipefd[0]);
      close (pipefd[1]);
      errno = saved_errno;
      pipefd[0] = pipefd[1] = -1;
      pipefd_pipe_size = 0;
      error (0, errno, "%s", _("splice error"));
    }
  അല്ലെങ്കിൽ എങ്കിൽ (! in_ok)
    error (0, errno, "%s", quotef (infile));
  അല്ലെങ്കിൽ എങ്കിൽ (! out_ok)
    write_error ();
#endif

  തിരികെ (in_ok && out_ok) ? some_copied : -1;
}

/* Reuse an aligned buffer across inputs, growing it only as needed.  */

സ്ഥിതം അക്ഷരം *
ensure_buf_size (അക്ഷരം *buf, idx_t *buf_alloc, idx_t alignment, idx_t size)
{
  affirm (buf != ശൂന്യ || *buf_alloc < size);

  എങ്കിൽ (*buf_alloc < size)
    {
      alignfree (buf);
      buf = xalignalloc (alignment, size);
      *buf_alloc = size;
    }

  തിരികെ buf;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  /* Nonzero if we have ever read standard input.  */
  bool have_read_stdin = false;

  ഘടന stat ostat_buf;

  /* Variables that are set according to the specified options.  */
  bool സംഖ്യ = false;
  bool സംഖ്യ_nonblank = false;
  bool squeeze_blank = false;
  bool show_ends = false;
  bool show_nonprinting = false;
  bool show_tabs = false;
  പൂർണ്ണസംഖ്യ file_open_mode = O_RDONLY;

  സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
  {
    {"number-nonblank", no_argument, ശൂന്യ, 'b'},
    {"number", no_argument, ശൂന്യ, 'n'},
    {"squeeze-blank", no_argument, ശൂന്യ, 's'},
    {"show-nonprinting", no_argument, ശൂന്യ, 'v'},
    {"show-ends", no_argument, ശൂന്യ, 'E'},
    {"show-tabs", no_argument, ശൂന്യ, 'T'},
    {"show-all", no_argument, ശൂന്യ, 'A'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {ശൂന്യ, 0, ശൂന്യ, 0}
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

  പൂർണ്ണസംഖ്യ c;
  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "benstuvAET", long_options, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'b':
          സംഖ്യ = true;
          സംഖ്യ_nonblank = true;
          നിർത്തുക;

        അവസ്ഥ 'e':
          show_ends = true;
          show_nonprinting = true;
          നിർത്തുക;

        അവസ്ഥ 'n':
          സംഖ്യ = true;
          നിർത്തുക;

        അവസ്ഥ 's':
          squeeze_blank = true;
          നിർത്തുക;

        അവസ്ഥ 't':
          show_tabs = true;
          show_nonprinting = true;
          നിർത്തുക;

        അവസ്ഥ 'u':
          /* We provide the -u feature unconditionally.  */
          നിർത്തുക;

        അവസ്ഥ 'v':
          show_nonprinting = true;
          നിർത്തുക;

        അവസ്ഥ 'A':
          show_nonprinting = true;
          show_ends = true;
          show_tabs = true;
          നിർത്തുക;

        അവസ്ഥ 'E':
          show_ends = true;
          നിർത്തുക;

        അവസ്ഥ 'T':
          show_tabs = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  /* Get device, i-node number, and optimal blocksize of output.  */

  എങ്കിൽ (fstat (STDOUT_FILENO, &ostat_buf) < 0)
    error (EXIT_FAILURE, errno, _("standard output"));

  /* Optimal size of i/o operations of output.  */
  idx_t outsize = io_blksize (&ostat_buf);

  /* Device, I-node number and lazily-acquired flags of the output.  */
  പൂർണ്ണസംഖ്യ out_flags = -2;

  /* True if the output is a regular file.  */
  bool out_isreg = S_ISREG (ostat_buf.st_mode) != 0;

  എങ്കിൽ (! (സംഖ്യ || show_ends || squeeze_blank))
    {
      file_open_mode |= O_ദ്വിമാന;
      xset_ദ്വിമാന_mode (STDOUT_FILENO, O_ദ്വിമാന);
    }

  /* Main loop.  */

  infile = "-";
  പൂർണ്ണസംഖ്യ argind = optind;
  bool ok = true;
  idx_t page_size = getpagesize ();
  അക്ഷരം *inbuf = ശൂന്യ;
  അക്ഷരം *outbuf = ശൂന്യ;
  idx_t inbuf_alloc = 0;
  idx_t outbuf_alloc = 0;

  ചെയ്യുക
    {
      എങ്കിൽ (argind < argc)
        infile = argv[argind];

      bool reading_stdin = streq (infile, "-");
      എങ്കിൽ (reading_stdin)
        {
          have_read_stdin = true;
          നിവേശം_desc = STDIN_FILENO;
          എങ്കിൽ (file_open_mode & O_ദ്വിമാന)
            xset_ദ്വിമാന_mode (STDIN_FILENO, O_ദ്വിമാന);
        }
      അല്ലെങ്കിൽ
        {
          നിവേശം_desc = open (infile, file_open_mode);
          എങ്കിൽ (നിവേശം_desc < 0)
            {
              error (0, errno, "%s", quotef (infile));
              ok = false;
              തുടരുക;
            }
        }

      ഘടന stat istat_buf;
      എങ്കിൽ (fstat (നിവേശം_desc, &istat_buf) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
          പോകുക contin;
        }

      /* Optimal size of i/o operations of input.  */
      idx_t insize = io_blksize (&istat_buf);

      fdadvise (നിവേശം_desc, 0, 0, FADVISE_SEQUENTIAL);

      /* Don't copy a file to itself if that would merely exhaust the
         output device.  It's better to catch this error earlier
         rather than later.  */

      എങ്കിൽ (! (S_ISFIFO (istat_buf.st_mode) || S_ISSOCK (istat_buf.st_mode)
             || S_TYPEISSHM (&istat_buf) || S_TYPEISTMO (&istat_buf))
          && ! (S_TYPEISSHM (&ostat_buf) || S_TYPEISTMO (&ostat_buf))
          && SAME_INODE (istat_buf, ostat_buf))
        {
          off_t in_pos = lseek (നിവേശം_desc, 0, SEEK_CUR);
          എങ്കിൽ (0 <= in_pos)
            {
              എങ്കിൽ (out_flags < -1)
                out_flags = fcntl (STDOUT_FILENO, F_GETFL);
              പൂർണ്ണസംഖ്യ whence = (0 <= out_flags && out_flags & O_APPEND
                            ? SEEK_END : SEEK_CUR);
              എങ്കിൽ (in_pos < lseek (STDOUT_FILENO, 0, whence))
                {
                  error (0, 0, _("%s: input file is output file"),
                         quotef (infile));
                  ok = false;
                  പോകുക contin;
                }
            }
        }

      /* Select which version of 'cat' to use.  If any format-oriented
         options were given use 'cat'; if not, use 'copy_cat' if it
         works, 'simple_cat' otherwise.  */

      എങ്കിൽ (! (സംഖ്യ || show_ends || show_nonprinting
             || show_tabs || squeeze_blank))
        {
          പൂർണ്ണസംഖ്യ copy_cat_status =
            out_isreg && S_ISREG (istat_buf.st_mode) ? copy_cat () : 0;
          എങ്കിൽ (copy_cat_status != 0)
            ok &= 0 < copy_cat_status;
          അല്ലെങ്കിൽ
            {
              /* Note 32768 was determined as the limit when splice
                 starts to have a performance advantage.
                 It also excludes zero length files which may not
                 be compatible with splice in some edge cases.  */
              പൂർണ്ണസംഖ്യ splice_cat_status = ((usable_st_size (&istat_buf)
                                        && istat_buf.st_size <= 32768)
                                       ? 0 : splice_cat ());
              എങ്കിൽ (splice_cat_status != 0)
                ok &= 0 < splice_cat_status;
              അല്ലെങ്കിൽ
                {
                  insize = MAX (insize, outsize);
                  inbuf = ensure_buf_size (inbuf, &inbuf_alloc,
                                           page_size, insize);
                  ok &= simple_cat (inbuf, insize);
                }
            }
        }
      അല്ലെങ്കിൽ
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
          എങ്കിൽ (ckd_mul (&bufsize, insize, 4)
              || ckd_add (&bufsize, bufsize, outsize)
              || ckd_add (&bufsize, bufsize, വരി_COUNTER_BUF_LEN - 1))
            xalloc_die ();
          outbuf = ensure_buf_size (outbuf, &outbuf_alloc,
                                    page_size, bufsize);

          ok &= cat (inbuf, insize, outbuf, outsize, show_nonprinting,
                     show_tabs, സംഖ്യ, സംഖ്യ_nonblank, show_ends,
                     squeeze_blank);
        }

    contin:
      എങ്കിൽ (!reading_stdin && close (നിവേശം_desc) < 0)
        {
          error (0, errno, "%s", quotef (infile));
          ok = false;
        }
    }
  ആകുമ്പോൾ (++argind < argc);

  എങ്കിൽ (pending_cr)
    {
      എങ്കിൽ (full_write (STDOUT_FILENO, "\r", 1) != 1)
        write_error ();
    }

#ifdef lint
  alignfree (outbuf);
  alignfree (inbuf);
#endif

  എങ്കിൽ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, _("closing standard input"));

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
