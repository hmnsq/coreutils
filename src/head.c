/* head -- output first part of file(s)
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Options: (see usage)
   Reads from standard input if no files are given or when a filename of
   ''-'' is encountered.
   By default, filename headers are printed only if more than one file
   is given.
   By default, prints the first 10 lines (head -n 10).

   David MacKenzie <djm@gnu.ai.mit.edu> */

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"

#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "full-read.h"
#ضمن "stat-size.h"
#ضمن "xbinary-io.h"
#ضمن "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "head"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#define DEFAULT_NUMBER 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ساكن bool presume_input_pipe;

/* If true, print filename headers. */
ساكن bool print_headers;

/* Character to split lines by. */
ساكن حرف line_end = '\n';

/* When to print the filename banners. */
تعداد header_mode
{
  multiple_files, always, never
};

/* Have we ever read standard input?  */
ساكن bool have_read_stdin;

تعداد Copy_fd_status
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  PRESUME_INPUT_PIPE_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت long_options[] =
{
  {"bytes", required_argument, عدم, 'c'},
  {"lines", required_argument, عدم, 'n'},
  {"-presume-input-pipe", no_argument, عدم,
   PRESUME_INPUT_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, عدم, 'q'},
  {"silent", no_argument, عدم, 'q'},
  {"verbose", no_argument, عدم, 'v'},
  {"zero-terminated", no_argument, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  𐡄𐡍 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      printf (_("\
Print the first %d lines of each FILE to standard output.\n\
With more than one FILE, precede each with a header giving the file name.\n\
"), DEFAULT_NUMBER);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --bytes=[-]NUM\n\
         print the first NUM bytes of each file;\n\
         with the leading '-', print all but the last NUM bytes of each file\n\
"));
      oprintf (_("\
  -n, --lines=[-]NUM\n\
         print the first NUM lines instead of the first %d;\n\
         with the leading '-', print all but the last NUM lines of each file\n\
"), DEFAULT_NUMBER);
      oputs (_("\
  -q, --quiet, --silent\n\
         never print headers giving file names\n\
"));
      oputs (_("\
  -v, --verbose\n\
         always print headers giving file names\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         line delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
NUM may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on لكل T, P, E, Z, Y, R, Q.\n\
Binary prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ساكن فراغ
diagnose_read_failure (حرف ثابت *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

ساكن فراغ
diagnose_copy_fd_failure (تعداد Copy_fd_status err, حرف ثابت *filename)
{
  بدل (err)
    {
    حالة COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      اكسر;
    حالة COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      اكسر;
    حالة COPY_FD_OK: افتراضي:
      affirm (false);
    }
}

ساكن فراغ
write_header (حرف ثابت *filename)
{
  ساكن bool first_file = true;

  printf ("%s==> %s <==\n", (first_file ? "" : "\n"), quotef (filename));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ساكن فراغ
xwrite_stdout (حرف ثابت *buffer, size_t n_bytes)
{
  𐡄𐡍 (n_bytes > 0 && fwrite (buffer, 1, n_bytes, stdout) < n_bytes)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

ساكن تعداد Copy_fd_status
copy_fd (صحيح src_fd, uintmax_t n_bytes)
{
  حرف buf[BUFSIZ];

  /* Copy the file contents.  */
  طالما (0 < n_bytes)
    {
      idx_t n_to_read = MIN (n_bytes, حجم buf);
      ssize_t n_read = اقرأ (src_fd, buf, n_to_read);
      𐡄𐡍 (n_read < 0)
        أرجع COPY_FD_READ_ERROR;

      n_bytes -= n_read;

      𐡄𐡍 (n_read == 0 && n_bytes != 0)
        أرجع COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  أرجع COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
ساكن فراغ
elseek_diagnostic (off_t offset, صحيح whence, حرف ثابت *filename)
{
  intmax_t off = offset;
  error (0, errno,
         _(whence == SEEK_SET
           ? N_("%s: cannot seek to offset %jd")
           : N_("%s: cannot seek to relative offset %jd")),
         quotef (filename), off);
}

/* Call lseek (FD, OFFSET, WHENCE), where file descriptor FD
   corresponds to the file FILENAME.  WHENCE must be SEEK_SET or
   SEEK_CUR.  Return the resulting offset.  Give a diagnostic and
   return -1 if lseek fails.  */

ساكن off_t
elseek (صحيح fd, off_t offset, صحيح whence, حرف ثابت *filename)
{
  off_t new_offset = lseek (fd, offset, whence);

  𐡄𐡍 (new_offset < 0)
    elseek_diagnostic (offset, whence, filename);

  أرجع new_offset;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
ساكن bool
elide_tail_bytes_pipe (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       off_t current_pos)
{
  uintmax_t desired_pos = current_pos;
  bool ok = true;

#ifndef HEAD_TAIL_PIPE_READ_BUFSIZE
# define HEAD_TAIL_PIPE_READ_BUFSIZE BUFSIZ
#endif
#define READ_BUFSIZE HEAD_TAIL_PIPE_READ_BUFSIZE

  /* If we're eliding no more than this many bytes, then it's ok to allocate
     more memory in order to use a more time-efficient algorithm.
     FIXME: use a fraction of available memory instead, as in sort.
     FIXME: is this even worthwhile?  */
#ifndef HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD
# define HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD 1024 * 1024
#endif

#𐡄𐡍 HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD < 2 * READ_BUFSIZE
# error "HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD must be at least 2 * READ_BUFSIZE"
#endif

  /* Two cases to consider...
     1) n_elide is small enough that we can afford to double-buffer:
        allocate 2 * (READ_BUFSIZE + n_elide) bytes
     2) n_elide is too big for that, so we allocate only
        (READ_BUFSIZE + n_elide) bytes

     FIXME: profile, to see if double-buffering is worthwhile

     CAUTION: do not fail (out of memory) when asked to elide
     a ridiculous amount, but when given only a small input.  */

  static_assert (READ_BUFSIZE <= IDX_MAX);
  static_assert (HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD <= IDX_MAX - READ_BUFSIZE);
  𐡄𐡍 (n_elide <= HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD)
    {
      idx_t in_elide = n_elide;
      bool first = true;
      bool eof = false;
      idx_t n_to_read = READ_BUFSIZE + n_elide;
      حرف *b[2];
      b[0] = xnmalloc (2, n_to_read);
      b[1] = b[0] + n_to_read;

      لكل (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = full_read (fd, b[i], n_to_read);
          idx_t delta = 0;
          𐡄𐡍 (n_read < n_to_read)
            {
              𐡄𐡍 (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  اكسر;
                }

              /* reached EOF */
              𐡄𐡍 (n_read <= in_elide)
                {
                  𐡄𐡍 (first)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  وإلا
                    {
                      delta = in_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          𐡄𐡍 (! first)
            {
              desired_pos += in_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, in_elide - delta);
            }
          first = false;

          𐡄𐡍 (in_elide < n_read)
            {
              desired_pos += n_read - in_elide;
              xwrite_stdout (b[i], n_read - in_elide);
            }
        }

      حرر (b[0]);
    }
  وإلا
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      حرف **b = عدم;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_next;
      لكل (i = 0, i_next = 1; !eof; i = i_next, i_next = (i_next + 1) % n_bufs)
        {
          𐡄𐡍 (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         حجم *b);

          𐡄𐡍 (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = full_read (fd, b[i], READ_BUFSIZE);
          𐡄𐡍 (n_read < READ_BUFSIZE)
            {
              𐡄𐡍 (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  اذهب free_mem;
                }
              eof = true;
            }

          𐡄𐡍 (i + 1 == n_bufs)
            buffered_enough = true;

          𐡄𐡍 (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_next], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      𐡄𐡍 (buffered_enough)
        {
          idx_t n_bytes_left_in_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          𐡄𐡍 (rem < n_bytes_left_in_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          وإلا
            {
              xwrite_stdout (b[i] + n_read, n_bytes_left_in_b_i);
              xwrite_stdout (b[i_next], rem - n_bytes_left_in_b_i);
            }
        }
      وإلا 𐡄𐡍 (i + 1 == n_bufs)
        {
          /* This happens when
             n_elide < file_size < (n_bufs - 1) * READ_BUFSIZE.

             |READ_BUF.|
             |                      |  rem |
             |---------!---------!---------!---------|
             |---- n_elide----------|
             |                      | x |
             |                   |y |
             |---- file size -----------|
             |                   |n_read|
             |(n_bufs - 1) * READ_BUFSIZE--|
           */
          idx_t y = READ_BUFSIZE - rem;
          idx_t x = n_read - y;
          desired_pos += x;
          xwrite_stdout (b[i_next], x);
        }

    free_mem:
      لكل (i = 0; i < n_alloc; i++)
        حرر (b[i]);
      حرر (b);
    }

  𐡄𐡍 (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  أرجع ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

ساكن bool
elide_tail_bytes_file (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       بنية stat ثابت *st, off_t current_pos)
{
  off_t size = st->st_size;
  𐡄𐡍 (current_pos < 0 || size <= STP_BLKSIZE (st))
    أرجع elide_tail_bytes_pipe (filename, fd, n_elide, current_pos);
  وإلا
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = size - current_pos;
      off_t bytes_remaining = diff < 0 ? 0 : diff;

      𐡄𐡍 (bytes_remaining <= n_elide)
        أرجع true;

      تعداد Copy_fd_status err = copy_fd (fd, bytes_remaining - n_elide);
      𐡄𐡍 (err == COPY_FD_OK)
        أرجع true;

      diagnose_copy_fd_failure (err, filename);
      أرجع false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

ساكن bool
elide_tail_lines_pipe (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       off_t current_pos)
{
  بنية linebuffer
  {
    حرف buffer[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    بنية linebuffer *next;
  };
  off_t desired_pos = current_pos;
  تعريفنوع بنية linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  size_t total_lines = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (حجم (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = عدم;
  tmp = xmalloc (حجم (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  طالما (true)
    {
      n_read = اقرأ (fd, tmp->buffer, BUFSIZ);
      𐡄𐡍 (n_read <= 0)
        اكسر;

      𐡄𐡍 (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->buffer, n_read);
          استمر;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = عدم;

      /* Count the number of newlines just read.  */
      {
        حرف *buffer_end = tmp->buffer + n_read;
        *buffer_end = line_end;
        حرف ثابت *p = tmp->buffer;
        طالما ((p = rawmemchr (p, line_end)) < buffer_end)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_lines += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      𐡄𐡍 (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      وإلا
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          𐡄𐡍 (n_elide < total_lines - first->nlines)
            {
              desired_pos += first->nbytes;
              xwrite_stdout (first->buffer, first->nbytes);
              tmp = first;
              total_lines -= first->nlines;
              first = first->next;
            }
          وإلا
            tmp = xmalloc (حجم (LBUFFER));
        }
    }

  حرر (tmp);

  𐡄𐡍 (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      اذهب free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  𐡄𐡍 (last->nbytes && last->buffer[last->nbytes - 1] != line_end)
    {
      ++last->nlines;
      ++total_lines;
    }

  لكل (tmp = first; n_elide < total_lines - tmp->nlines; tmp = tmp->next)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->buffer, tmp->nbytes);
      total_lines -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  𐡄𐡍 (n_elide < total_lines)
    {
      size_t n = total_lines - n_elide;
      حرف ثابت *buffer_end = tmp->buffer + tmp->nbytes;
      حرف ثابت *p = tmp->buffer;
      طالما (n && (p = memchr (p, line_end, buffer_end - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->buffer;
      xwrite_stdout (tmp->buffer, p - tmp->buffer);
    }

free_lbuffers:
  طالما (first)
    {
      tmp = first->next;
      حرر (first);
      first = tmp;
    }

  𐡄𐡍 (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  أرجع ok;
}

/* Output all but the last N_LINES lines of the input stream defined by
   FD, START_POS, and SIZE.
   START_POS is the starting position of the read pointer for the file
   associated with FD (may be nonzero).
   SIZE is the file size in bytes.
   Return true upon success.
   Give a diagnostic and return false upon error.

   NOTE: this code is very similar to that of tail.c's file_lines function.
   Unfortunately, factoring out some common core looks like it'd result
   in a less efficient implementation or a messy interface.  */
ساكن bool
elide_tail_lines_seekable (حرف ثابت *pretty_filename, صحيح fd,
                           uintmax_t n_lines,
                           off_t start_pos, off_t size)
{
  حرف buffer[BUFSIZ];
  ssize_t bytes_read;
  off_t pos = size;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  bytes_read = (pos - start_pos) % BUFSIZ;
  𐡄𐡍 (bytes_read == 0)
    bytes_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= bytes_read;
  𐡄𐡍 (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    أرجع false;
  bytes_read = اقرأ (fd, buffer, bytes_read);
  𐡄𐡍 (bytes_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      أرجع false;
    }

  /* n_lines == 0 case needs special treatment. */
  ثابت bool all_lines = !n_lines;

  /* Count the incomplete line on files that don't end with a newline.  */
  𐡄𐡍 (n_lines && bytes_read && buffer[bytes_read - 1] != line_end)
    --n_lines;

  طالما (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      طالما (n)
        {
          𐡄𐡍 (all_lines)
            n -= 1;
          وإلا
            {
              حرف ثابت *nl;
              nl = memrchr (buffer, line_end, n);
              𐡄𐡍 (nl == عدم)
                اكسر;
              n = nl - buffer;
            }
          𐡄𐡍 (n_lines-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              𐡄𐡍 (start_pos < pos)
                {
                  تعداد Copy_fd_status err;
                  𐡄𐡍 (elseek (fd, start_pos, SEEK_SET, pretty_filename) < 0)
                    أرجع false;

                  err = copy_fd (fd, pos - start_pos);
                  𐡄𐡍 (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      أرجع false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (buffer, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              أرجع 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      𐡄𐡍 (pos == start_pos)
        {
          /* Not enough lines in the file.  */
          أرجع true;
        }
      pos -= BUFSIZ;
      𐡄𐡍 (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        أرجع false;

      bytes_read = اقرأ (fd, buffer, BUFSIZ);
      𐡄𐡍 (bytes_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          أرجع false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      𐡄𐡍 (bytes_read == 0)
        أرجع true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

ساكن bool
elide_tail_lines_file (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       بنية stat ثابت *st, off_t current_pos)
{
  off_t size = st->st_size;
  𐡄𐡍 (current_pos < 0 || size <= STP_BLKSIZE (st))
    أرجع elide_tail_lines_pipe (filename, fd, n_elide, current_pos);
  وإلا
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      أرجع (size <= current_pos
              || elide_tail_lines_seekable (filename, fd, n_elide,
                                            current_pos, size));
    }
}

ساكن bool
head_bytes (حرف ثابت *filename, صحيح fd, uintmax_t bytes_to_write)
{
  𐡄𐡍 (copy_fd (fd, bytes_to_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      أرجع false;
    }
  أرجع true;
}

ساكن bool
head_lines (حرف ثابت *filename, صحيح fd, uintmax_t lines_to_write)
{
  حرف buffer[BUFSIZ];

  طالما (lines_to_write)
    {
      ssize_t bytes_read = اقرأ (fd, buffer, BUFSIZ);
      idx_t bytes_to_write = 0;

      𐡄𐡍 (bytes_read < 0)
        {
          diagnose_read_failure (filename);
          أرجع false;
        }
      𐡄𐡍 (bytes_read == 0)
        اكسر;
      طالما (bytes_to_write < bytes_read)
        𐡄𐡍 (buffer[bytes_to_write++] == line_end && --lines_to_write == 0)
          {
            off_t n_bytes_past_EOL = bytes_read - bytes_to_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            𐡄𐡍 (lseek (fd, -n_bytes_past_EOL, SEEK_CUR) < 0)
              {
                بنية stat st;
                𐡄𐡍 (fstat (fd, &st) != 0 || S_ISREG (st.st_mode))
                  elseek_diagnostic (-n_bytes_past_EOL, SEEK_CUR, filename);
              }
            اكسر;
          }
      xwrite_stdout (buffer, bytes_to_write);
    }
  أرجع true;
}

ساكن bool
head (حرف ثابت *filename, صحيح fd, uintmax_t n_units, bool count_lines,
      bool elide_from_end)
{
  𐡄𐡍 (print_headers)
    write_header (filename);

  𐡄𐡍 (elide_from_end)
    {
      /* Optimize for "infinite" elisions.  */
      𐡄𐡍 (n_units == UINTMAX_MAX)
        أرجع true;

      off_t current_pos = -1;
      بنية stat st;
      𐡄𐡍 (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          أرجع false;
        }
      𐡄𐡍 (! presume_input_pipe && S_ISREG (st.st_mode))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          𐡄𐡍 (current_pos < 0)
            أرجع false;
        }
      𐡄𐡍 (count_lines)
        أرجع elide_tail_lines_file (filename, fd, n_units, &st, current_pos);
      وإلا
        أرجع elide_tail_bytes_file (filename, fd, n_units, &st, current_pos);
    }
  𐡄𐡍 (count_lines)
    أرجع head_lines (filename, fd, n_units);
  وإلا
    أرجع head_bytes (filename, fd, n_units);
}

ساكن bool
head_file (حرف ثابت *filename, uintmax_t n_units, bool count_lines,
           bool elide_from_end)
{
  صحيح fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  𐡄𐡍 (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_binary_mode (STDIN_FILENO, O_BINARY);
    }
  وإلا
    {
      fd = open (filename, O_RDONLY | O_BINARY);
      𐡄𐡍 (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          أرجع false;
        }
    }

  ok = head (filename, fd, n_units, count_lines, elide_from_end);
  𐡄𐡍 (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      أرجع false;
    }
  أرجع ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

ساكن uintmax_t
string_to_integer (bool count_lines, حرف ثابت *n_string)
{
  أرجع xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (count_lines
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

صحيح
main (صحيح argc, حرف **argv)
{
  تعداد header_mode header_mode = multiple_files;
  bool ok = true;
  صحيح c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_NUMBER;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool count_lines = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_end = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ساكن حرف ثابت *ثابت default_file_list[] = {"-", عدم};
  حرف ثابت *ثابت *file_list;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  𐡄𐡍 (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      حرف *a = argv[1];
      حرف *n_string = ++a;
      حرف *end_n_string;
      حرف multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      𐡏𐡁𐡃 ++a;
      طالما (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      end_n_string = a;

      /* Parse any appended option letters. */
      لكل (; *a; a++)
        {
          بدل (*a)
            {
            حالة 'c':
              count_lines = false;
              multiplier_char = 0;
              اكسر;

            حالة 'b':
            حالة 'k':
            حالة 'm':
              count_lines = false;
              multiplier_char = *a;
              اكسر;

            حالة 'l':
              count_lines = true;
              اكسر;

            حالة 'q':
              header_mode = never;
              اكسر;

            حالة 'v':
              header_mode = always;
              اكسر;

            حالة 'z':
              line_end = '\0';
              اكسر;

            افتراضي:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *end_n_string = multiplier_char;
      𐡄𐡍 (multiplier_char)
        *(++end_n_string) = 0;

      n_units = string_to_integer (count_lines, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  طالما ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, عدم))
         != -1)
    {
      بدل (c)
        {
        حالة PRESUME_INPUT_PIPE_OPTION:
          presume_input_pipe = true;
          اكسر;

        حالة 'c':
          count_lines = false;
          elide_from_end = (*optarg == '-');
          𐡄𐡍 (elide_from_end)
            ++optarg;
          n_units = string_to_integer (count_lines, optarg);
          اكسر;

        حالة 'n':
          count_lines = true;
          elide_from_end = (*optarg == '-');
          𐡄𐡍 (elide_from_end)
            ++optarg;
          n_units = string_to_integer (count_lines, optarg);
          اكسر;

        حالة 'q':
          header_mode = never;
          اكسر;

        حالة 'v':
          header_mode = always;
          اكسر;

        حالة 'z':
          line_end = '\0';
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          𐡄𐡍 (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  𐡄𐡍 (header_mode == always
      || (header_mode == multiple_files && optind < argc - 1))
    print_headers = true;

  file_list = (optind < argc
               ? (حرف ثابت *ثابت *) &argv[optind]
               : default_file_list);

  xset_binary_mode (STDOUT_FILENO, O_BINARY);

  لكل (size_t i = 0; file_list[i]; ++i)
    ok &= head_file (file_list[i], n_units, count_lines, elide_from_end);

  𐡄𐡍 (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
