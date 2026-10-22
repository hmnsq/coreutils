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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"

#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "full-read.h"
#அந்தர்பாவ "stat-size.h"
#அந்தர்பாவ "xbinary-io.h"
#அந்தர்பாவ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "head"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#define DEFAULT_எண் 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
நிலையான bool presume_உள்ளீடு_pipe;

/* If true, print filename headers. */
நிலையான bool print_headers;

/* Character to split lines by. */
நிலையான எழுத்து வரி_end = '\n';

/* When to print the filename banners. */
எண்ணிக்கை header_mode
{
  multiple_files, always, never
};

/* Have we ever read standard input?  */
நிலையான bool have_read_stdin;

எண்ணிக்கை Copy_fd_status
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  PRESUME_உள்ளீடு_PIPE_OPTION = CHAR_MAX + 1
};

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"bytes", required_argument, வெற்று, 'c'},
  {"lines", required_argument, வெற்று, 'n'},
  {"-presume-input-pipe", no_argument, வெற்று,
   PRESUME_உள்ளீடு_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, வெற்று, 'q'},
  {"silent", no_argument, வெற்று, 'q'},
  {"verbose", no_argument, வெற்று, 'v'},
  {"zero-terminated", no_argument, வெற்று, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      printf (_("\
Print the first %d வரிகள் of each FILE to standard output.\n\
With more than one FILE, precede each with a header giving the file name.\n\
"), DEFAULT_எண்);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --bytes=[-]இலக்கம்_2\n\
         print the first இலக்கம்_2 bytes of each file;\n\
         with the leading '-', print all but the last இலக்கம்_2 bytes of each file\n\
"));
      oprintf (_("\
  -n, --வரிகள்=[-]இலக்கம்_2\n\
         print the first இலக்கம்_2 வரிகள் instead of the first %d;\n\
         with the leading '-', print all but the last இலக்கம்_2 வரிகள் of each file\n\
"), DEFAULT_எண்);
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
         வரி delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
இலக்கம்_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on க்கு T, P, E, Z, Y, R, Q.\n\
இருமம்_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

நிலையான வெறுமை
diagnose_read_failure (எழுத்து மாறிலி *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

நிலையான வெறுமை
diagnose_copy_fd_failure (எண்ணிக்கை Copy_fd_status err, எழுத்து மாறிலி *filename)
{
  தேர்வு (err)
    {
    நிலை COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      நிறுத்து;
    நிலை COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      நிறுத்து;
    நிலை COPY_FD_OK: இயல்பு:
      affirm (false);
    }
}

நிலையான வெறுமை
write_header (எழுத்து மாறிலி *filename)
{
  நிலையான bool first_file = true;

  printf ("%s==> %s <==\n", (first_file ? "" : "\n"), quotef (filename));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

நிலையான வெறுமை
xwrite_stdout (எழுத்து மாறிலி *buffer, size_t n_bytes)
{
  எனில் (n_bytes > 0 && fwrite (buffer, 1, n_bytes, stdout) < n_bytes)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

நிலையான எண்ணிக்கை Copy_fd_status
copy_fd (முழு src_fd, uintmax_t n_bytes)
{
  எழுத்து buf[BUFSIZ];

  /* Copy the file contents.  */
  போது (0 < n_bytes)
    {
      idx_t n_to_read = MIN (n_bytes, அளவு buf);
      ssize_t n_read = பட (src_fd, buf, n_to_read);
      எனில் (n_read < 0)
        திரும்பு COPY_FD_READ_ERROR;

      n_bytes -= n_read;

      எனில் (n_read == 0 && n_bytes != 0)
        திரும்பு COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  திரும்பு COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
நிலையான வெறுமை
elseek_diagnostic (off_t offset, முழு whence, எழுத்து மாறிலி *filename)
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

நிலையான off_t
elseek (முழு fd, off_t offset, முழு whence, எழுத்து மாறிலி *filename)
{
  off_t new_offset = lseek (fd, offset, whence);

  எனில் (new_offset < 0)
    elseek_diagnostic (offset, whence, filename);

  திரும்பு new_offset;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
நிலையான bool
elide_tail_bytes_pipe (எழுத்து மாறிலி *filename, முழு fd, uintmax_t n_elide,
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

#எனில் HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD < 2 * READ_BUFSIZE
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
  எனில் (n_elide <= HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD)
    {
      idx_t in_elide = n_elide;
      bool first = true;
      bool eof = false;
      idx_t n_to_read = READ_BUFSIZE + n_elide;
      எழுத்து *b[2];
      b[0] = xnmalloc (2, n_to_read);
      b[1] = b[0] + n_to_read;

      க்கு (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = full_read (fd, b[i], n_to_read);
          idx_t delta = 0;
          எனில் (n_read < n_to_read)
            {
              எனில் (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  நிறுத்து;
                }

              /* reached EOF */
              எனில் (n_read <= in_elide)
                {
                  எனில் (first)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  இல்லையெனில்
                    {
                      delta = in_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          எனில் (! first)
            {
              desired_pos += in_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, in_elide - delta);
            }
          first = false;

          எனில் (in_elide < n_read)
            {
              desired_pos += n_read - in_elide;
              xwrite_stdout (b[i], n_read - in_elide);
            }
        }

      விடுவி (b[0]);
    }
  இல்லையெனில்
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      எழுத்து **b = வெற்று;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_next;
      க்கு (i = 0, i_next = 1; !eof; i = i_next, i_next = (i_next + 1) % n_bufs)
        {
          எனில் (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         அளவு *b);

          எனில் (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = full_read (fd, b[i], READ_BUFSIZE);
          எனில் (n_read < READ_BUFSIZE)
            {
              எனில் (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  செல் free_mem;
                }
              eof = true;
            }

          எனில் (i + 1 == n_bufs)
            buffered_enough = true;

          எனில் (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_next], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      எனில் (buffered_enough)
        {
          idx_t n_bytes_இடது_in_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          எனில் (rem < n_bytes_இடது_in_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          இல்லையெனில்
            {
              xwrite_stdout (b[i] + n_read, n_bytes_இடது_in_b_i);
              xwrite_stdout (b[i_next], rem - n_bytes_இடது_in_b_i);
            }
        }
      இல்லையெனில் எனில் (i + 1 == n_bufs)
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
      க்கு (i = 0; i < n_alloc; i++)
        விடுவி (b[i]);
      விடுவி (b);
    }

  எனில் (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  திரும்பு ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

நிலையான bool
elide_tail_bytes_file (எழுத்து மாறிலி *filename, முழு fd, uintmax_t n_elide,
                       கட்டமைப்பு stat மாறிலி *st, off_t current_pos)
{
  off_t size = st->st_size;
  எனில் (current_pos < 0 || size <= STP_BLKSIZE (st))
    திரும்பு elide_tail_bytes_pipe (filename, fd, n_elide, current_pos);
  இல்லையெனில்
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = size - current_pos;
      off_t bytes_remaining = diff < 0 ? 0 : diff;

      எனில் (bytes_remaining <= n_elide)
        திரும்பு true;

      எண்ணிக்கை Copy_fd_status err = copy_fd (fd, bytes_remaining - n_elide);
      எனில் (err == COPY_FD_OK)
        திரும்பு true;

      diagnose_copy_fd_failure (err, filename);
      திரும்பு false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

நிலையான bool
elide_tail_வரிகள்_pipe (எழுத்து மாறிலி *filename, முழு fd, uintmax_t n_elide,
                       off_t current_pos)
{
  கட்டமைப்பு linebuffer
  {
    எழுத்து buffer[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    கட்டமைப்பு linebuffer *next;
  };
  off_t desired_pos = current_pos;
  வகைவரை கட்டமைப்பு linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  size_t total_வரிகள் = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (அளவு (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = வெற்று;
  tmp = xmalloc (அளவு (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  போது (true)
    {
      n_read = பட (fd, tmp->buffer, BUFSIZ);
      எனில் (n_read <= 0)
        நிறுத்து;

      எனில் (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->buffer, n_read);
          தொடர்;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = வெற்று;

      /* Count the number of newlines just read.  */
      {
        எழுத்து *buffer_end = tmp->buffer + n_read;
        *buffer_end = வரி_end;
        எழுத்து மாறிலி *p = tmp->buffer;
        போது ((p = rawmemchr (p, வரி_end)) < buffer_end)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_வரிகள் += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      எனில் (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      இல்லையெனில்
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          எனில் (n_elide < total_வரிகள் - first->nlines)
            {
              desired_pos += first->nbytes;
              xwrite_stdout (first->buffer, first->nbytes);
              tmp = first;
              total_வரிகள் -= first->nlines;
              first = first->next;
            }
          இல்லையெனில்
            tmp = xmalloc (அளவு (LBUFFER));
        }
    }

  விடுவி (tmp);

  எனில் (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      செல் free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  எனில் (last->nbytes && last->buffer[last->nbytes - 1] != வரி_end)
    {
      ++last->nlines;
      ++total_வரிகள்;
    }

  க்கு (tmp = first; n_elide < total_வரிகள் - tmp->nlines; tmp = tmp->next)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->buffer, tmp->nbytes);
      total_வரிகள் -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  எனில் (n_elide < total_வரிகள்)
    {
      size_t n = total_வரிகள் - n_elide;
      எழுத்து மாறிலி *buffer_end = tmp->buffer + tmp->nbytes;
      எழுத்து மாறிலி *p = tmp->buffer;
      போது (n && (p = memchr (p, வரி_end, buffer_end - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->buffer;
      xwrite_stdout (tmp->buffer, p - tmp->buffer);
    }

free_lbuffers:
  போது (first)
    {
      tmp = first->next;
      விடுவி (first);
      first = tmp;
    }

  எனில் (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  திரும்பு ok;
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
நிலையான bool
elide_tail_வரிகள்_seekable (எழுத்து மாறிலி *pretty_filename, முழு fd,
                           uintmax_t n_வரிகள்,
                           off_t start_pos, off_t size)
{
  எழுத்து buffer[BUFSIZ];
  ssize_t bytes_read;
  off_t pos = size;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  bytes_read = (pos - start_pos) % BUFSIZ;
  எனில் (bytes_read == 0)
    bytes_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= bytes_read;
  எனில் (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    திரும்பு false;
  bytes_read = பட (fd, buffer, bytes_read);
  எனில் (bytes_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      திரும்பு false;
    }

  /* n_lines == 0 case needs special treatment. */
  மாறிலி bool all_வரிகள் = !n_வரிகள்;

  /* Count the incomplete line on files that don't end with a newline.  */
  எனில் (n_வரிகள் && bytes_read && buffer[bytes_read - 1] != வரி_end)
    --n_வரிகள்;

  போது (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      போது (n)
        {
          எனில் (all_வரிகள்)
            n -= 1;
          இல்லையெனில்
            {
              எழுத்து மாறிலி *nl;
              nl = memrchr (buffer, வரி_end, n);
              எனில் (nl == வெற்று)
                நிறுத்து;
              n = nl - buffer;
            }
          எனில் (n_வரிகள்-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              எனில் (start_pos < pos)
                {
                  எண்ணிக்கை Copy_fd_status err;
                  எனில் (elseek (fd, start_pos, SEEK_SET, pretty_filename) < 0)
                    திரும்பு false;

                  err = copy_fd (fd, pos - start_pos);
                  எனில் (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      திரும்பு false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (buffer, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              திரும்பு 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      எனில் (pos == start_pos)
        {
          /* Not enough lines in the file.  */
          திரும்பு true;
        }
      pos -= BUFSIZ;
      எனில் (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        திரும்பு false;

      bytes_read = பட (fd, buffer, BUFSIZ);
      எனில் (bytes_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          திரும்பு false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      எனில் (bytes_read == 0)
        திரும்பு true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

நிலையான bool
elide_tail_வரிகள்_file (எழுத்து மாறிலி *filename, முழு fd, uintmax_t n_elide,
                       கட்டமைப்பு stat மாறிலி *st, off_t current_pos)
{
  off_t size = st->st_size;
  எனில் (current_pos < 0 || size <= STP_BLKSIZE (st))
    திரும்பு elide_tail_வரிகள்_pipe (filename, fd, n_elide, current_pos);
  இல்லையெனில்
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      திரும்பு (size <= current_pos
              || elide_tail_வரிகள்_seekable (filename, fd, n_elide,
                                            current_pos, size));
    }
}

நிலையான bool
head_bytes (எழுத்து மாறிலி *filename, முழு fd, uintmax_t bytes_to_write)
{
  எனில் (copy_fd (fd, bytes_to_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      திரும்பு false;
    }
  திரும்பு true;
}

நிலையான bool
head_வரிகள் (எழுத்து மாறிலி *filename, முழு fd, uintmax_t வரிகள்_to_write)
{
  எழுத்து buffer[BUFSIZ];

  போது (வரிகள்_to_write)
    {
      ssize_t bytes_read = பட (fd, buffer, BUFSIZ);
      idx_t bytes_to_write = 0;

      எனில் (bytes_read < 0)
        {
          diagnose_read_failure (filename);
          திரும்பு false;
        }
      எனில் (bytes_read == 0)
        நிறுத்து;
      போது (bytes_to_write < bytes_read)
        எனில் (buffer[bytes_to_write++] == வரி_end && --வரிகள்_to_write == 0)
          {
            off_t n_bytes_past_EOL = bytes_read - bytes_to_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            எனில் (lseek (fd, -n_bytes_past_EOL, SEEK_CUR) < 0)
              {
                கட்டமைப்பு stat st;
                எனில் (fstat (fd, &st) != 0 || S_ISREG (st.st_mode))
                  elseek_diagnostic (-n_bytes_past_EOL, SEEK_CUR, filename);
              }
            நிறுத்து;
          }
      xwrite_stdout (buffer, bytes_to_write);
    }
  திரும்பு true;
}

நிலையான bool
head (எழுத்து மாறிலி *filename, முழு fd, uintmax_t n_units, bool மொத்தம்_வரிகள்,
      bool elide_from_end)
{
  எனில் (print_headers)
    write_header (filename);

  எனில் (elide_from_end)
    {
      /* Optimize for "infinite" elisions.  */
      எனில் (n_units == UINTMAX_MAX)
        திரும்பு true;

      off_t current_pos = -1;
      கட்டமைப்பு stat st;
      எனில் (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          திரும்பு false;
        }
      எனில் (! presume_உள்ளீடு_pipe && S_ISREG (st.st_mode))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          எனில் (current_pos < 0)
            திரும்பு false;
        }
      எனில் (மொத்தம்_வரிகள்)
        திரும்பு elide_tail_வரிகள்_file (filename, fd, n_units, &st, current_pos);
      இல்லையெனில்
        திரும்பு elide_tail_bytes_file (filename, fd, n_units, &st, current_pos);
    }
  எனில் (மொத்தம்_வரிகள்)
    திரும்பு head_வரிகள் (filename, fd, n_units);
  இல்லையெனில்
    திரும்பு head_bytes (filename, fd, n_units);
}

நிலையான bool
head_file (எழுத்து மாறிலி *filename, uintmax_t n_units, bool மொத்தம்_வரிகள்,
           bool elide_from_end)
{
  முழு fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  எனில் (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_இருமம்_mode (STDIN_FILENO, O_இருமம்);
    }
  இல்லையெனில்
    {
      fd = open (filename, O_RDONLY | O_இருமம்);
      எனில் (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          திரும்பு false;
        }
    }

  ok = head (filename, fd, n_units, மொத்தம்_வரிகள், elide_from_end);
  எனில் (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      திரும்பு false;
    }
  திரும்பு ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

நிலையான uintmax_t
string_to_integer (bool மொத்தம்_வரிகள், எழுத்து மாறிலி *n_string)
{
  திரும்பு xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (மொத்தம்_வரிகள்
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  எண்ணிக்கை header_mode header_mode = multiple_files;
  bool ok = true;
  முழு c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_எண்;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool மொத்தம்_வரிகள் = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_end = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  நிலையான எழுத்து மாறிலி *மாறிலி default_file_list[] = {"-", வெற்று};
  எழுத்து மாறிலி *மாறிலி *file_list;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  எனில் (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      எழுத்து *a = argv[1];
      எழுத்து *n_string = ++a;
      எழுத்து *end_n_string;
      எழுத்து multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      செய் ++a;
      போது (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      end_n_string = a;

      /* Parse any appended option letters. */
      க்கு (; *a; a++)
        {
          தேர்வு (*a)
            {
            நிலை 'c':
              மொத்தம்_வரிகள் = false;
              multiplier_char = 0;
              நிறுத்து;

            நிலை 'b':
            நிலை 'k':
            நிலை 'm':
              மொத்தம்_வரிகள் = false;
              multiplier_char = *a;
              நிறுத்து;

            நிலை 'l':
              மொத்தம்_வரிகள் = true;
              நிறுத்து;

            நிலை 'q':
              header_mode = never;
              நிறுத்து;

            நிலை 'v':
              header_mode = always;
              நிறுத்து;

            நிலை 'z':
              வரி_end = '\0';
              நிறுத்து;

            இயல்பு:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *end_n_string = multiplier_char;
      எனில் (multiplier_char)
        *(++end_n_string) = 0;

      n_units = string_to_integer (மொத்தம்_வரிகள், n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  போது ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, வெற்று))
         != -1)
    {
      தேர்வு (c)
        {
        நிலை PRESUME_உள்ளீடு_PIPE_OPTION:
          presume_உள்ளீடு_pipe = true;
          நிறுத்து;

        நிலை 'c':
          மொத்தம்_வரிகள் = false;
          elide_from_end = (*optarg == '-');
          எனில் (elide_from_end)
            ++optarg;
          n_units = string_to_integer (மொத்தம்_வரிகள், optarg);
          நிறுத்து;

        நிலை 'n':
          மொத்தம்_வரிகள் = true;
          elide_from_end = (*optarg == '-');
          எனில் (elide_from_end)
            ++optarg;
          n_units = string_to_integer (மொத்தம்_வரிகள், optarg);
          நிறுத்து;

        நிலை 'q':
          header_mode = never;
          நிறுத்து;

        நிலை 'v':
          header_mode = always;
          நிறுத்து;

        நிலை 'z':
          வரி_end = '\0';
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          எனில் (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  எனில் (header_mode == always
      || (header_mode == multiple_files && optind < argc - 1))
    print_headers = true;

  file_list = (optind < argc
               ? (எழுத்து மாறிலி *மாறிலி *) &argv[optind]
               : default_file_list);

  xset_இருமம்_mode (STDOUT_FILENO, O_இருமம்);

  க்கு (size_t i = 0; file_list[i]; ++i)
    ok &= head_file (file_list[i], n_units, மொத்தம்_வரிகள், elide_from_end);

  எனில் (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
