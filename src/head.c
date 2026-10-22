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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "full-read.h"
#અન્તર્ભાવ "stat-size.h"
#અન્તર્ભાવ "xbinary-io.h"
#અન્તર્ભાવ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "head"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#define DEFAULT_સંખ્યા 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
સ્થિર bool presume_આગત_pipe;

/* If true, print filename headers. */
સ્થિર bool print_headers;

/* Character to split lines by. */
સ્થિર અક્ષર લીટી_end = '\n';

/* When to print the filename banners. */
ગણના header_mode
{
  multiple_files, always, never
};

/* Have we ever read standard input?  */
સ્થિર bool have_read_stdin;

ગણના Copy_fd_status
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  PRESUME_આગત_PIPE_OPTION = CHAR_MAX + 1
};

સ્થિર સંરચના option અચલ long_options[] =
{
  {"bytes", required_argument, ખાલી, 'c'},
  {"lines", required_argument, ખાલી, 'n'},
  {"-presume-input-pipe", no_argument, ખાલી,
   PRESUME_આગત_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, ખાલી, 'q'},
  {"silent", no_argument, ખાલી, 'q'},
  {"verbose", no_argument, ખાલી, 'v'},
  {"zero-terminated", no_argument, ખાલી, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      printf (_("\
Print the first %d લીટીઓ of each FILE to standard output.\n\
With more than one FILE, precede each with a header giving the file name.\n\
"), DEFAULT_સંખ્યા);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --bytes=[-]અંક_2\n\
         print the first અંક_2 bytes of each file;\n\
         with the leading '-', print all but the last અંક_2 bytes of each file\n\
"));
      oprintf (_("\
  -n, --લીટીઓ=[-]અંક_2\n\
         print the first અંક_2 લીટીઓ instead of the first %d;\n\
         with the leading '-', print all but the last અંક_2 લીટીઓ of each file\n\
"), DEFAULT_સંખ્યા);
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
         લીટી delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
અંક_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on માટે T, P, E, Z, Y, R, Q.\n\
દ્વિઅંકી_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

સ્થિર શૂન્ય
diagnose_read_failure (અક્ષર અચલ *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

સ્થિર શૂન્ય
diagnose_copy_fd_failure (ગણના Copy_fd_status err, અક્ષર અચલ *filename)
{
  પસંદગી (err)
    {
    સ્થિતિ COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      તોડો;
    સ્થિતિ COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      તોડો;
    સ્થિતિ COPY_FD_OK: મૂળભૂત:
      affirm (false);
    }
}

સ્થિર શૂન્ય
write_header (અક્ષર અચલ *filename)
{
  સ્થિર bool first_file = true;

  printf ("%s==> %s <==\n", (first_file ? "" : "\n"), quotef (filename));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

સ્થિર શૂન્ય
xwrite_stdout (અક્ષર અચલ *buffer, size_t n_bytes)
{
  જો (n_bytes > 0 && fwrite (buffer, 1, n_bytes, stdout) < n_bytes)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

સ્થિર ગણના Copy_fd_status
copy_fd (પૂર્ણાંક src_fd, uintmax_t n_bytes)
{
  અક્ષર buf[BUFSIZ];

  /* Copy the file contents.  */
  જ્યારે (0 < n_bytes)
    {
      idx_t n_to_read = MIN (n_bytes, કદ buf);
      ssize_t n_read = પઠ (src_fd, buf, n_to_read);
      જો (n_read < 0)
        પાછા COPY_FD_READ_ERROR;

      n_bytes -= n_read;

      જો (n_read == 0 && n_bytes != 0)
        પાછા COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  પાછા COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
સ્થિર શૂન્ય
elseek_diagnostic (off_t offset, પૂર્ણાંક whence, અક્ષર અચલ *filename)
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

સ્થિર off_t
elseek (પૂર્ણાંક fd, off_t offset, પૂર્ણાંક whence, અક્ષર અચલ *filename)
{
  off_t new_offset = lseek (fd, offset, whence);

  જો (new_offset < 0)
    elseek_diagnostic (offset, whence, filename);

  પાછા new_offset;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
સ્થિર bool
elide_tail_bytes_pipe (અક્ષર અચલ *filename, પૂર્ણાંક fd, uintmax_t n_elide,
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

#જો HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD < 2 * READ_BUFSIZE
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
  જો (n_elide <= HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD)
    {
      idx_t in_elide = n_elide;
      bool first = true;
      bool eof = false;
      idx_t n_to_read = READ_BUFSIZE + n_elide;
      અક્ષર *b[2];
      b[0] = xnmalloc (2, n_to_read);
      b[1] = b[0] + n_to_read;

      માટે (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = full_read (fd, b[i], n_to_read);
          idx_t delta = 0;
          જો (n_read < n_to_read)
            {
              જો (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  તોડો;
                }

              /* reached EOF */
              જો (n_read <= in_elide)
                {
                  જો (first)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  નહીંતર
                    {
                      delta = in_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          જો (! first)
            {
              desired_pos += in_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, in_elide - delta);
            }
          first = false;

          જો (in_elide < n_read)
            {
              desired_pos += n_read - in_elide;
              xwrite_stdout (b[i], n_read - in_elide);
            }
        }

      મુક્ત (b[0]);
    }
  નહીંતર
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      અક્ષર **b = ખાલી;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_next;
      માટે (i = 0, i_next = 1; !eof; i = i_next, i_next = (i_next + 1) % n_bufs)
        {
          જો (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         કદ *b);

          જો (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = full_read (fd, b[i], READ_BUFSIZE);
          જો (n_read < READ_BUFSIZE)
            {
              જો (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  જાઓ free_mem;
                }
              eof = true;
            }

          જો (i + 1 == n_bufs)
            buffered_enough = true;

          જો (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_next], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      જો (buffered_enough)
        {
          idx_t n_bytes_ડાબું_in_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          જો (rem < n_bytes_ડાબું_in_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          નહીંતર
            {
              xwrite_stdout (b[i] + n_read, n_bytes_ડાબું_in_b_i);
              xwrite_stdout (b[i_next], rem - n_bytes_ડાબું_in_b_i);
            }
        }
      નહીંતર જો (i + 1 == n_bufs)
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
      માટે (i = 0; i < n_alloc; i++)
        મુક્ત (b[i]);
      મુક્ત (b);
    }

  જો (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  પાછા ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

સ્થિર bool
elide_tail_bytes_file (અક્ષર અચલ *filename, પૂર્ણાંક fd, uintmax_t n_elide,
                       સંરચના stat અચલ *st, off_t current_pos)
{
  off_t size = st->st_size;
  જો (current_pos < 0 || size <= STP_BLKSIZE (st))
    પાછા elide_tail_bytes_pipe (filename, fd, n_elide, current_pos);
  નહીંતર
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = size - current_pos;
      off_t bytes_remaining = diff < 0 ? 0 : diff;

      જો (bytes_remaining <= n_elide)
        પાછા true;

      ગણના Copy_fd_status err = copy_fd (fd, bytes_remaining - n_elide);
      જો (err == COPY_FD_OK)
        પાછા true;

      diagnose_copy_fd_failure (err, filename);
      પાછા false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

સ્થિર bool
elide_tail_લીટીઓ_pipe (અક્ષર અચલ *filename, પૂર્ણાંક fd, uintmax_t n_elide,
                       off_t current_pos)
{
  સંરચના linebuffer
  {
    અક્ષર buffer[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    સંરચના linebuffer *next;
  };
  off_t desired_pos = current_pos;
  પ્રકારવ્યાખ્યા સંરચના linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  size_t total_લીટીઓ = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (કદ (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = ખાલી;
  tmp = xmalloc (કદ (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  જ્યારે (true)
    {
      n_read = પઠ (fd, tmp->buffer, BUFSIZ);
      જો (n_read <= 0)
        તોડો;

      જો (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->buffer, n_read);
          ચાલુ;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = ખાલી;

      /* Count the number of newlines just read.  */
      {
        અક્ષર *buffer_end = tmp->buffer + n_read;
        *buffer_end = લીટી_end;
        અક્ષર અચલ *p = tmp->buffer;
        જ્યારે ((p = rawmemchr (p, લીટી_end)) < buffer_end)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_લીટીઓ += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      જો (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      નહીંતર
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          જો (n_elide < total_લીટીઓ - first->nlines)
            {
              desired_pos += first->nbytes;
              xwrite_stdout (first->buffer, first->nbytes);
              tmp = first;
              total_લીટીઓ -= first->nlines;
              first = first->next;
            }
          નહીંતર
            tmp = xmalloc (કદ (LBUFFER));
        }
    }

  મુક્ત (tmp);

  જો (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      જાઓ free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  જો (last->nbytes && last->buffer[last->nbytes - 1] != લીટી_end)
    {
      ++last->nlines;
      ++total_લીટીઓ;
    }

  માટે (tmp = first; n_elide < total_લીટીઓ - tmp->nlines; tmp = tmp->next)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->buffer, tmp->nbytes);
      total_લીટીઓ -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  જો (n_elide < total_લીટીઓ)
    {
      size_t n = total_લીટીઓ - n_elide;
      અક્ષર અચલ *buffer_end = tmp->buffer + tmp->nbytes;
      અક્ષર અચલ *p = tmp->buffer;
      જ્યારે (n && (p = memchr (p, લીટી_end, buffer_end - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->buffer;
      xwrite_stdout (tmp->buffer, p - tmp->buffer);
    }

free_lbuffers:
  જ્યારે (first)
    {
      tmp = first->next;
      મુક્ત (first);
      first = tmp;
    }

  જો (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  પાછા ok;
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
સ્થિર bool
elide_tail_લીટીઓ_seekable (અક્ષર અચલ *pretty_filename, પૂર્ણાંક fd,
                           uintmax_t n_લીટીઓ,
                           off_t start_pos, off_t size)
{
  અક્ષર buffer[BUFSIZ];
  ssize_t bytes_read;
  off_t pos = size;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  bytes_read = (pos - start_pos) % BUFSIZ;
  જો (bytes_read == 0)
    bytes_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= bytes_read;
  જો (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    પાછા false;
  bytes_read = પઠ (fd, buffer, bytes_read);
  જો (bytes_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      પાછા false;
    }

  /* n_lines == 0 case needs special treatment. */
  અચલ bool all_લીટીઓ = !n_લીટીઓ;

  /* Count the incomplete line on files that don't end with a newline.  */
  જો (n_લીટીઓ && bytes_read && buffer[bytes_read - 1] != લીટી_end)
    --n_લીટીઓ;

  જ્યારે (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      જ્યારે (n)
        {
          જો (all_લીટીઓ)
            n -= 1;
          નહીંતર
            {
              અક્ષર અચલ *nl;
              nl = memrchr (buffer, લીટી_end, n);
              જો (nl == ખાલી)
                તોડો;
              n = nl - buffer;
            }
          જો (n_લીટીઓ-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              જો (start_pos < pos)
                {
                  ગણના Copy_fd_status err;
                  જો (elseek (fd, start_pos, SEEK_SET, pretty_filename) < 0)
                    પાછા false;

                  err = copy_fd (fd, pos - start_pos);
                  જો (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      પાછા false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (buffer, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              પાછા 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      જો (pos == start_pos)
        {
          /* Not enough lines in the file.  */
          પાછા true;
        }
      pos -= BUFSIZ;
      જો (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        પાછા false;

      bytes_read = પઠ (fd, buffer, BUFSIZ);
      જો (bytes_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          પાછા false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      જો (bytes_read == 0)
        પાછા true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

સ્થિર bool
elide_tail_લીટીઓ_file (અક્ષર અચલ *filename, પૂર્ણાંક fd, uintmax_t n_elide,
                       સંરચના stat અચલ *st, off_t current_pos)
{
  off_t size = st->st_size;
  જો (current_pos < 0 || size <= STP_BLKSIZE (st))
    પાછા elide_tail_લીટીઓ_pipe (filename, fd, n_elide, current_pos);
  નહીંતર
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      પાછા (size <= current_pos
              || elide_tail_લીટીઓ_seekable (filename, fd, n_elide,
                                            current_pos, size));
    }
}

સ્થિર bool
head_bytes (અક્ષર અચલ *filename, પૂર્ણાંક fd, uintmax_t bytes_to_write)
{
  જો (copy_fd (fd, bytes_to_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      પાછા false;
    }
  પાછા true;
}

સ્થિર bool
head_લીટીઓ (અક્ષર અચલ *filename, પૂર્ણાંક fd, uintmax_t લીટીઓ_to_write)
{
  અક્ષર buffer[BUFSIZ];

  જ્યારે (લીટીઓ_to_write)
    {
      ssize_t bytes_read = પઠ (fd, buffer, BUFSIZ);
      idx_t bytes_to_write = 0;

      જો (bytes_read < 0)
        {
          diagnose_read_failure (filename);
          પાછા false;
        }
      જો (bytes_read == 0)
        તોડો;
      જ્યારે (bytes_to_write < bytes_read)
        જો (buffer[bytes_to_write++] == લીટી_end && --લીટીઓ_to_write == 0)
          {
            off_t n_bytes_past_EOL = bytes_read - bytes_to_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            જો (lseek (fd, -n_bytes_past_EOL, SEEK_CUR) < 0)
              {
                સંરચના stat st;
                જો (fstat (fd, &st) != 0 || S_ISREG (st.st_mode))
                  elseek_diagnostic (-n_bytes_past_EOL, SEEK_CUR, filename);
              }
            તોડો;
          }
      xwrite_stdout (buffer, bytes_to_write);
    }
  પાછા true;
}

સ્થિર bool
head (અક્ષર અચલ *filename, પૂર્ણાંક fd, uintmax_t n_units, bool ગણતરી_લીટીઓ,
      bool elide_from_end)
{
  જો (print_headers)
    write_header (filename);

  જો (elide_from_end)
    {
      /* Optimize for "infinite" elisions.  */
      જો (n_units == UINTMAX_MAX)
        પાછા true;

      off_t current_pos = -1;
      સંરચના stat st;
      જો (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          પાછા false;
        }
      જો (! presume_આગત_pipe && S_ISREG (st.st_mode))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          જો (current_pos < 0)
            પાછા false;
        }
      જો (ગણતરી_લીટીઓ)
        પાછા elide_tail_લીટીઓ_file (filename, fd, n_units, &st, current_pos);
      નહીંતર
        પાછા elide_tail_bytes_file (filename, fd, n_units, &st, current_pos);
    }
  જો (ગણતરી_લીટીઓ)
    પાછા head_લીટીઓ (filename, fd, n_units);
  નહીંતર
    પાછા head_bytes (filename, fd, n_units);
}

સ્થિર bool
head_file (અક્ષર અચલ *filename, uintmax_t n_units, bool ગણતરી_લીટીઓ,
           bool elide_from_end)
{
  પૂર્ણાંક fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  જો (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_દ્વિઅંકી_mode (STDIN_FILENO, O_દ્વિઅંકી);
    }
  નહીંતર
    {
      fd = open (filename, O_RDONLY | O_દ્વિઅંકી);
      જો (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          પાછા false;
        }
    }

  ok = head (filename, fd, n_units, ગણતરી_લીટીઓ, elide_from_end);
  જો (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      પાછા false;
    }
  પાછા ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

સ્થિર uintmax_t
string_to_integer (bool ગણતરી_લીટીઓ, અક્ષર અચલ *n_string)
{
  પાછા xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (ગણતરી_લીટીઓ
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  ગણના header_mode header_mode = multiple_files;
  bool ok = true;
  પૂર્ણાંક c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_સંખ્યા;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool ગણતરી_લીટીઓ = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_end = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  સ્થિર અક્ષર અચલ *અચલ default_file_list[] = {"-", ખાલી};
  અક્ષર અચલ *અચલ *file_list;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જો (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      અક્ષર *a = argv[1];
      અક્ષર *n_string = ++a;
      અક્ષર *end_n_string;
      અક્ષર multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      કરો ++a;
      જ્યારે (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      end_n_string = a;

      /* Parse any appended option letters. */
      માટે (; *a; a++)
        {
          પસંદગી (*a)
            {
            સ્થિતિ 'c':
              ગણતરી_લીટીઓ = false;
              multiplier_char = 0;
              તોડો;

            સ્થિતિ 'b':
            સ્થિતિ 'k':
            સ્થિતિ 'm':
              ગણતરી_લીટીઓ = false;
              multiplier_char = *a;
              તોડો;

            સ્થિતિ 'l':
              ગણતરી_લીટીઓ = true;
              તોડો;

            સ્થિતિ 'q':
              header_mode = never;
              તોડો;

            સ્થિતિ 'v':
              header_mode = always;
              તોડો;

            સ્થિતિ 'z':
              લીટી_end = '\0';
              તોડો;

            મૂળભૂત:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *end_n_string = multiplier_char;
      જો (multiplier_char)
        *(++end_n_string) = 0;

      n_units = string_to_integer (ગણતરી_લીટીઓ, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  જ્યારે ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, ખાલી))
         != -1)
    {
      પસંદગી (c)
        {
        સ્થિતિ PRESUME_આગત_PIPE_OPTION:
          presume_આગત_pipe = true;
          તોડો;

        સ્થિતિ 'c':
          ગણતરી_લીટીઓ = false;
          elide_from_end = (*optarg == '-');
          જો (elide_from_end)
            ++optarg;
          n_units = string_to_integer (ગણતરી_લીટીઓ, optarg);
          તોડો;

        સ્થિતિ 'n':
          ગણતરી_લીટીઓ = true;
          elide_from_end = (*optarg == '-');
          જો (elide_from_end)
            ++optarg;
          n_units = string_to_integer (ગણતરી_લીટીઓ, optarg);
          તોડો;

        સ્થિતિ 'q':
          header_mode = never;
          તોડો;

        સ્થિતિ 'v':
          header_mode = always;
          તોડો;

        સ્થિતિ 'z':
          લીટી_end = '\0';
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          જો (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  જો (header_mode == always
      || (header_mode == multiple_files && optind < argc - 1))
    print_headers = true;

  file_list = (optind < argc
               ? (અક્ષર અચલ *અચલ *) &argv[optind]
               : default_file_list);

  xset_દ્વિઅંકી_mode (STDOUT_FILENO, O_દ્વિઅંકી);

  માટે (size_t i = 0; file_list[i]; ++i)
    ok &= head_file (file_list[i], n_units, ગણતરી_લીટીઓ, elide_from_end);

  જો (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
