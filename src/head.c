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

#include <config.h>

#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>

#include "system.h"

#include "assure.h"
#include "c-ctype.h"
#include "full-read.h"
#include "stat-size.h"
#include "xbinary-io.h"
#include "xdectoint.h"

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
static bool presume_input_pipe;

/* If true, print filename headers. */
static bool print_headers;

/* Character to split lines by. */
static var_na line_end = '\n';

/* When to print the filename banners. */
ga_nanaa header_mode
{
  multiple_files, always, never
};

/* Have we ever read standard input?  */
static bool have_read_stdin;

ga_nanaa Copy_fd_status
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  PRESUME_INPUT_PIPE_OPTION = CHAR_MAX + 1
};

static sa_nracanaa option const long_options[] =
{
  {"bytes", required_argument, NULL, 'c'},
  {"lines", required_argument, NULL, 'n'},
  {"-presume-input-pipe", no_argument, NULL,
   PRESUME_INPUT_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, NULL, 'q'},
  {"silent", no_argument, NULL, 'q'},
  {"verbose", no_argument, NULL, 'v'},
  {"zero-terminated", no_argument, NULL, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (puur_naa_nka status)
{
  ᱡᱩᱫᱤ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᱵᱟᱝ
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
GB 1000*1000*1000, G 1024*1024*1024, and so on for T, P, E, Z, Y, R, Q.\n\
Binary prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

static void
diagnose_read_failure (var_na const *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

static void
diagnose_copy_fd_failure (ga_nanaa Copy_fd_status err, var_na const *filename)
{
  switch (err)
    {
    pak_sa COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      bha_nga;
    pak_sa COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      bha_nga;
    pak_sa COPY_FD_OK: default:
      affirm (false);
    }
}

static void
write_header (var_na const *filename)
{
  static bool first_file = true;

  printf ("%s==> %s <==\n", (first_file ? "" : "\n"), quotef (filename));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

static void
xwrite_stdout (var_na const *buffer, size_t n_bytes)
{
  ᱡᱩᱫᱤ (n_bytes > 0 && fwrite (buffer, 1, n_bytes, stdout) < n_bytes)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

static ga_nanaa Copy_fd_status
copy_fd (puur_naa_nka src_fd, uintmax_t n_bytes)
{
  var_na buf[BUFSIZ];

  /* Copy the file contents.  */
  ᱡᱚᱠᱷᱚᱱ (0 < n_bytes)
    {
      idx_t n_to_read = MIN (n_bytes, parimaa_na buf);
      ssize_t n_read = read (src_fd, buf, n_to_read);
      ᱡᱩᱫᱤ (n_read < 0)
        ᱨᱩᱣᱟᱹᱲ COPY_FD_READ_ERROR;

      n_bytes -= n_read;

      ᱡᱩᱫᱤ (n_read == 0 && n_bytes != 0)
        ᱨᱩᱣᱟᱹᱲ COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  ᱨᱩᱣᱟᱹᱲ COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
static void
elseek_diagnostic (off_t offset, puur_naa_nka whence, var_na const *filename)
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

static off_t
elseek (puur_naa_nka fd, off_t offset, puur_naa_nka whence, var_na const *filename)
{
  off_t new_offset = lseek (fd, offset, whence);

  ᱡᱩᱫᱤ (new_offset < 0)
    elseek_diagnostic (offset, whence, filename);

  ᱨᱩᱣᱟᱹᱲ new_offset;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
static bool
elide_tail_bytes_pipe (var_na const *filename, puur_naa_nka fd, uintmax_t n_elide,
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

#ᱡᱩᱫᱤ HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD < 2 * READ_BUFSIZE
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
  ᱡᱩᱫᱤ (n_elide <= HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD)
    {
      idx_t in_elide = n_elide;
      bool first = true;
      bool eof = false;
      idx_t n_to_read = READ_BUFSIZE + n_elide;
      var_na *b[2];
      b[0] = xnmalloc (2, n_to_read);
      b[1] = b[0] + n_to_read;

      for (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = full_read (fd, b[i], n_to_read);
          idx_t delta = 0;
          ᱡᱩᱫᱤ (n_read < n_to_read)
            {
              ᱡᱩᱫᱤ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  bha_nga;
                }

              /* reached EOF */
              ᱡᱩᱫᱤ (n_read <= in_elide)
                {
                  ᱡᱩᱫᱤ (first)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  ᱵᱟᱝ
                    {
                      delta = in_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          ᱡᱩᱫᱤ (! first)
            {
              desired_pos += in_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, in_elide - delta);
            }
          first = false;

          ᱡᱩᱫᱤ (in_elide < n_read)
            {
              desired_pos += n_read - in_elide;
              xwrite_stdout (b[i], n_read - in_elide);
            }
        }

      free (b[0]);
    }
  ᱵᱟᱝ
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      var_na **b = NULL;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_next;
      for (i = 0, i_next = 1; !eof; i = i_next, i_next = (i_next + 1) % n_bufs)
        {
          ᱡᱩᱫᱤ (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         parimaa_na *b);

          ᱡᱩᱫᱤ (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = full_read (fd, b[i], READ_BUFSIZE);
          ᱡᱩᱫᱤ (n_read < READ_BUFSIZE)
            {
              ᱡᱩᱫᱤ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  goto free_mem;
                }
              eof = true;
            }

          ᱡᱩᱫᱤ (i + 1 == n_bufs)
            buffered_enough = true;

          ᱡᱩᱫᱤ (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_next], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      ᱡᱩᱫᱤ (buffered_enough)
        {
          idx_t n_bytes_left_in_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          ᱡᱩᱫᱤ (rem < n_bytes_left_in_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          ᱵᱟᱝ
            {
              xwrite_stdout (b[i] + n_read, n_bytes_left_in_b_i);
              xwrite_stdout (b[i_next], rem - n_bytes_left_in_b_i);
            }
        }
      ᱵᱟᱝ ᱡᱩᱫᱤ (i + 1 == n_bufs)
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
      for (i = 0; i < n_alloc; i++)
        free (b[i]);
      free (b);
    }

  ᱡᱩᱫᱤ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  ᱨᱩᱣᱟᱹᱲ ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

static bool
elide_tail_bytes_file (var_na const *filename, puur_naa_nka fd, uintmax_t n_elide,
                       sa_nracanaa stat const *st, off_t current_pos)
{
  off_t size = st->st_size;
  ᱡᱩᱫᱤ (current_pos < 0 || size <= STP_BLKSIZE (st))
    ᱨᱩᱣᱟᱹᱲ elide_tail_bytes_pipe (filename, fd, n_elide, current_pos);
  ᱵᱟᱝ
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = size - current_pos;
      off_t bytes_remaining = diff < 0 ? 0 : diff;

      ᱡᱩᱫᱤ (bytes_remaining <= n_elide)
        ᱨᱩᱣᱟᱹᱲ true;

      ga_nanaa Copy_fd_status err = copy_fd (fd, bytes_remaining - n_elide);
      ᱡᱩᱫᱤ (err == COPY_FD_OK)
        ᱨᱩᱣᱟᱹᱲ true;

      diagnose_copy_fd_failure (err, filename);
      ᱨᱩᱣᱟᱹᱲ false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

static bool
elide_tail_lines_pipe (var_na const *filename, puur_naa_nka fd, uintmax_t n_elide,
                       off_t current_pos)
{
  sa_nracanaa linebuffer
  {
    var_na buffer[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    sa_nracanaa linebuffer *next;
  };
  off_t desired_pos = current_pos;
  typedef sa_nracanaa linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  size_t total_lines = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (parimaa_na (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = NULL;
  tmp = xmalloc (parimaa_na (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  ᱡᱚᱠᱷᱚᱱ (true)
    {
      n_read = read (fd, tmp->buffer, BUFSIZ);
      ᱡᱩᱫᱤ (n_read <= 0)
        bha_nga;

      ᱡᱩᱫᱤ (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->buffer, n_read);
          continue;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = NULL;

      /* Count the number of newlines just read.  */
      {
        var_na *buffer_end = tmp->buffer + n_read;
        *buffer_end = line_end;
        var_na const *p = tmp->buffer;
        ᱡᱚᱠᱷᱚᱱ ((p = rawmemchr (p, line_end)) < buffer_end)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_lines += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      ᱡᱩᱫᱤ (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      ᱵᱟᱝ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          ᱡᱩᱫᱤ (n_elide < total_lines - first->nlines)
            {
              desired_pos += first->nbytes;
              xwrite_stdout (first->buffer, first->nbytes);
              tmp = first;
              total_lines -= first->nlines;
              first = first->next;
            }
          ᱵᱟᱝ
            tmp = xmalloc (parimaa_na (LBUFFER));
        }
    }

  free (tmp);

  ᱡᱩᱫᱤ (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      goto free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  ᱡᱩᱫᱤ (last->nbytes && last->buffer[last->nbytes - 1] != line_end)
    {
      ++last->nlines;
      ++total_lines;
    }

  for (tmp = first; n_elide < total_lines - tmp->nlines; tmp = tmp->next)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->buffer, tmp->nbytes);
      total_lines -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  ᱡᱩᱫᱤ (n_elide < total_lines)
    {
      size_t n = total_lines - n_elide;
      var_na const *buffer_end = tmp->buffer + tmp->nbytes;
      var_na const *p = tmp->buffer;
      ᱡᱚᱠᱷᱚᱱ (n && (p = memchr (p, line_end, buffer_end - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->buffer;
      xwrite_stdout (tmp->buffer, p - tmp->buffer);
    }

free_lbuffers:
  ᱡᱚᱠᱷᱚᱱ (first)
    {
      tmp = first->next;
      free (first);
      first = tmp;
    }

  ᱡᱩᱫᱤ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  ᱨᱩᱣᱟᱹᱲ ok;
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
static bool
elide_tail_lines_seekable (var_na const *pretty_filename, puur_naa_nka fd,
                           uintmax_t n_lines,
                           off_t start_pos, off_t size)
{
  var_na buffer[BUFSIZ];
  ssize_t bytes_read;
  off_t pos = size;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  bytes_read = (pos - start_pos) % BUFSIZ;
  ᱡᱩᱫᱤ (bytes_read == 0)
    bytes_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= bytes_read;
  ᱡᱩᱫᱤ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    ᱨᱩᱣᱟᱹᱲ false;
  bytes_read = read (fd, buffer, bytes_read);
  ᱡᱩᱫᱤ (bytes_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      ᱨᱩᱣᱟᱹᱲ false;
    }

  /* n_lines == 0 case needs special treatment. */
  const bool all_lines = !n_lines;

  /* Count the incomplete line on files that don't end with a newline.  */
  ᱡᱩᱫᱤ (n_lines && bytes_read && buffer[bytes_read - 1] != line_end)
    --n_lines;

  ᱡᱚᱠᱷᱚᱱ (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      ᱡᱚᱠᱷᱚᱱ (n)
        {
          ᱡᱩᱫᱤ (all_lines)
            n -= 1;
          ᱵᱟᱝ
            {
              var_na const *nl;
              nl = memrchr (buffer, line_end, n);
              ᱡᱩᱫᱤ (nl == NULL)
                bha_nga;
              n = nl - buffer;
            }
          ᱡᱩᱫᱤ (n_lines-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              ᱡᱩᱫᱤ (start_pos < pos)
                {
                  ga_nanaa Copy_fd_status err;
                  ᱡᱩᱫᱤ (elseek (fd, start_pos, SEEK_SET, pretty_filename) < 0)
                    ᱨᱩᱣᱟᱹᱲ false;

                  err = copy_fd (fd, pos - start_pos);
                  ᱡᱩᱫᱤ (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      ᱨᱩᱣᱟᱹᱲ false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (buffer, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              ᱨᱩᱣᱟᱹᱲ 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      ᱡᱩᱫᱤ (pos == start_pos)
        {
          /* Not enough lines in the file.  */
          ᱨᱩᱣᱟᱹᱲ true;
        }
      pos -= BUFSIZ;
      ᱡᱩᱫᱤ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        ᱨᱩᱣᱟᱹᱲ false;

      bytes_read = read (fd, buffer, BUFSIZ);
      ᱡᱩᱫᱤ (bytes_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          ᱨᱩᱣᱟᱹᱲ false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      ᱡᱩᱫᱤ (bytes_read == 0)
        ᱨᱩᱣᱟᱹᱲ true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

static bool
elide_tail_lines_file (var_na const *filename, puur_naa_nka fd, uintmax_t n_elide,
                       sa_nracanaa stat const *st, off_t current_pos)
{
  off_t size = st->st_size;
  ᱡᱩᱫᱤ (current_pos < 0 || size <= STP_BLKSIZE (st))
    ᱨᱩᱣᱟᱹᱲ elide_tail_lines_pipe (filename, fd, n_elide, current_pos);
  ᱵᱟᱝ
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      ᱨᱩᱣᱟᱹᱲ (size <= current_pos
              || elide_tail_lines_seekable (filename, fd, n_elide,
                                            current_pos, size));
    }
}

static bool
head_bytes (var_na const *filename, puur_naa_nka fd, uintmax_t bytes_to_write)
{
  ᱡᱩᱫᱤ (copy_fd (fd, bytes_to_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      ᱨᱩᱣᱟᱹᱲ false;
    }
  ᱨᱩᱣᱟᱹᱲ true;
}

static bool
head_lines (var_na const *filename, puur_naa_nka fd, uintmax_t lines_to_write)
{
  var_na buffer[BUFSIZ];

  ᱡᱚᱠᱷᱚᱱ (lines_to_write)
    {
      ssize_t bytes_read = read (fd, buffer, BUFSIZ);
      idx_t bytes_to_write = 0;

      ᱡᱩᱫᱤ (bytes_read < 0)
        {
          diagnose_read_failure (filename);
          ᱨᱩᱣᱟᱹᱲ false;
        }
      ᱡᱩᱫᱤ (bytes_read == 0)
        bha_nga;
      ᱡᱚᱠᱷᱚᱱ (bytes_to_write < bytes_read)
        ᱡᱩᱫᱤ (buffer[bytes_to_write++] == line_end && --lines_to_write == 0)
          {
            off_t n_bytes_past_EOL = bytes_read - bytes_to_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            ᱡᱩᱫᱤ (lseek (fd, -n_bytes_past_EOL, SEEK_CUR) < 0)
              {
                sa_nracanaa stat st;
                ᱡᱩᱫᱤ (fstat (fd, &st) != 0 || S_ISREG (st.st_mode))
                  elseek_diagnostic (-n_bytes_past_EOL, SEEK_CUR, filename);
              }
            bha_nga;
          }
      xwrite_stdout (buffer, bytes_to_write);
    }
  ᱨᱩᱣᱟᱹᱲ true;
}

static bool
head (var_na const *filename, puur_naa_nka fd, uintmax_t n_units, bool count_lines,
      bool elide_from_end)
{
  ᱡᱩᱫᱤ (print_headers)
    write_header (filename);

  ᱡᱩᱫᱤ (elide_from_end)
    {
      /* Optimize for "infinite" elisions.  */
      ᱡᱩᱫᱤ (n_units == UINTMAX_MAX)
        ᱨᱩᱣᱟᱹᱲ true;

      off_t current_pos = -1;
      sa_nracanaa stat st;
      ᱡᱩᱫᱤ (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          ᱨᱩᱣᱟᱹᱲ false;
        }
      ᱡᱩᱫᱤ (! presume_input_pipe && S_ISREG (st.st_mode))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          ᱡᱩᱫᱤ (current_pos < 0)
            ᱨᱩᱣᱟᱹᱲ false;
        }
      ᱡᱩᱫᱤ (count_lines)
        ᱨᱩᱣᱟᱹᱲ elide_tail_lines_file (filename, fd, n_units, &st, current_pos);
      ᱵᱟᱝ
        ᱨᱩᱣᱟᱹᱲ elide_tail_bytes_file (filename, fd, n_units, &st, current_pos);
    }
  ᱡᱩᱫᱤ (count_lines)
    ᱨᱩᱣᱟᱹᱲ head_lines (filename, fd, n_units);
  ᱵᱟᱝ
    ᱨᱩᱣᱟᱹᱲ head_bytes (filename, fd, n_units);
}

static bool
head_file (var_na const *filename, uintmax_t n_units, bool count_lines,
           bool elide_from_end)
{
  puur_naa_nka fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  ᱡᱩᱫᱤ (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_binary_mode (STDIN_FILENO, O_BINARY);
    }
  ᱵᱟᱝ
    {
      fd = open (filename, O_RDONLY | O_BINARY);
      ᱡᱩᱫᱤ (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          ᱨᱩᱣᱟᱹᱲ false;
        }
    }

  ok = head (filename, fd, n_units, count_lines, elide_from_end);
  ᱡᱩᱫᱤ (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      ᱨᱩᱣᱟᱹᱲ false;
    }
  ᱨᱩᱣᱟᱹᱲ ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

static uintmax_t
string_to_integer (bool count_lines, var_na const *n_string)
{
  ᱨᱩᱣᱟᱹᱲ xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (count_lines
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  ga_nanaa header_mode header_mode = multiple_files;
  bool ok = true;
  puur_naa_nka c;

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
  static var_na const *const default_file_list[] = {"-", NULL};
  var_na const *const *file_list;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ᱡᱩᱫᱤ (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      var_na *a = argv[1];
      var_na *n_string = ++a;
      var_na *end_n_string;
      var_na multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      do ++a;
      ᱡᱚᱠᱷᱚᱱ (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      end_n_string = a;

      /* Parse any appended option letters. */
      for (; *a; a++)
        {
          switch (*a)
            {
            pak_sa 'c':
              count_lines = false;
              multiplier_char = 0;
              bha_nga;

            pak_sa 'b':
            pak_sa 'k':
            pak_sa 'm':
              count_lines = false;
              multiplier_char = *a;
              bha_nga;

            pak_sa 'l':
              count_lines = true;
              bha_nga;

            pak_sa 'q':
              header_mode = never;
              bha_nga;

            pak_sa 'v':
              header_mode = always;
              bha_nga;

            pak_sa 'z':
              line_end = '\0';
              bha_nga;

            default:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *end_n_string = multiplier_char;
      ᱡᱩᱫᱤ (multiplier_char)
        *(++end_n_string) = 0;

      n_units = string_to_integer (count_lines, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  ᱡᱚᱠᱷᱚᱱ ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, NULL))
         != -1)
    {
      switch (c)
        {
        pak_sa PRESUME_INPUT_PIPE_OPTION:
          presume_input_pipe = true;
          bha_nga;

        pak_sa 'c':
          count_lines = false;
          elide_from_end = (*optarg == '-');
          ᱡᱩᱫᱤ (elide_from_end)
            ++optarg;
          n_units = string_to_integer (count_lines, optarg);
          bha_nga;

        pak_sa 'n':
          count_lines = true;
          elide_from_end = (*optarg == '-');
          ᱡᱩᱫᱤ (elide_from_end)
            ++optarg;
          n_units = string_to_integer (count_lines, optarg);
          bha_nga;

        pak_sa 'q':
          header_mode = never;
          bha_nga;

        pak_sa 'v':
          header_mode = always;
          bha_nga;

        pak_sa 'z':
          line_end = '\0';
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          ᱡᱩᱫᱤ (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  ᱡᱩᱫᱤ (header_mode == always
      || (header_mode == multiple_files && optind < argc - 1))
    print_headers = true;

  file_list = (optind < argc
               ? (var_na const *const *) &argv[optind]
               : default_file_list);

  xset_binary_mode (STDOUT_FILENO, O_BINARY);

  for (size_t i = 0; file_list[i]; ++i)
    ok &= head_file (file_list[i], n_units, count_lines, elide_from_end);

  ᱡᱩᱫᱤ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  ᱨᱩᱣᱟᱹᱲ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
