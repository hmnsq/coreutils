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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "full-read.h"
#ಅನ್ತರ್ಭಾವ "stat-size.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "head"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#define DEFAULT_ಸಂಖ್ಯೆ 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ಸ್ಥಿರ bool presume_ಒಳಹರಿವು_pipe;

/* If true, print filename headers. */
ಸ್ಥಿರ bool print_headers;

/* Character to split lines by. */
ಸ್ಥಿರ ವರ್ಣ ಸಾಲು_end = '\n';

/* When to print the filename banners. */
ಗಣನಾ header_mode
{
  multiple_files, always, never
};

/* Have we ever read standard input?  */
ಸ್ಥಿರ bool have_read_stdin;

ಗಣನಾ Copy_fd_status
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  PRESUME_ಒಳಹರಿವು_PIPE_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"bytes", required_argument, ರಿಕ್ತ, 'c'},
  {"lines", required_argument, ರಿಕ್ತ, 'n'},
  {"-presume-input-pipe", no_argument, ರಿಕ್ತ,
   PRESUME_ಒಳಹರಿವು_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, ರಿಕ್ತ, 'q'},
  {"silent", no_argument, ರಿಕ್ತ, 'q'},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {"zero-terminated", no_argument, ರಿಕ್ತ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      printf (_("\
Print the first %d ಸಾಲುಗಳು of each FILE to standard output.\n\
With more than one FILE, precede each with a header giving the file name.\n\
"), DEFAULT_ಸಂಖ್ಯೆ);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --bytes=[-]ಅಂಕೆ_2\n\
         print the first ಅಂಕೆ_2 bytes of each file;\n\
         with the leading '-', print all but the last ಅಂಕೆ_2 bytes of each file\n\
"));
      oprintf (_("\
  -n, --ಸಾಲುಗಳು=[-]ಅಂಕೆ_2\n\
         print the first ಅಂಕೆ_2 ಸಾಲುಗಳು instead of the first %d;\n\
         with the leading '-', print all but the last ಅಂಕೆ_2 ಸಾಲುಗಳು of each file\n\
"), DEFAULT_ಸಂಖ್ಯೆ);
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
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ಅಂಕೆ_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on ಪ್ರತಿ T, P, E, Z, Y, R, Q.\n\
ದ್ವಿಮಾನ_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagnose_read_failure (ವರ್ಣ ಧ್ರುವ *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagnose_copy_fd_failure (ಗಣನಾ Copy_fd_status err, ವರ್ಣ ಧ್ರುವ *filename)
{
  ವಿಕಲ್ಪ (err)
    {
    ಪಕ್ಷ COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      ಭಙ್ಗ;
    ಪಕ್ಷ COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      ಭಙ್ಗ;
    ಪಕ್ಷ COPY_FD_OK: ಸ್ವಭಾವ:
      affirm (false);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
write_header (ವರ್ಣ ಧ್ರುವ *filename)
{
  ಸ್ಥಿರ bool first_file = true;

  printf ("%s==> %s <==\n", (first_file ? "" : "\n"), quotef (filename));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ಸ್ಥಿರ ಶೂನ್ಯ
xwrite_stdout (ವರ್ಣ ಧ್ರುವ *buffer, size_t n_bytes)
{
  ಯದಿ (n_bytes > 0 && fwrite (buffer, 1, n_bytes, stdout) < n_bytes)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

ಸ್ಥಿರ ಗಣನಾ Copy_fd_status
copy_fd (ಪೂರ್ಣಾಙ್ಕ src_fd, uintmax_t n_bytes)
{
  ವರ್ಣ buf[BUFSIZ];

  /* Copy the file contents.  */
  ಯಾವತ್ (0 < n_bytes)
    {
      idx_t n_to_read = MIN (n_bytes, ಪರಿಮಾಣ buf);
      ssize_t n_read = ಪಠ (src_fd, buf, n_to_read);
      ಯದಿ (n_read < 0)
        ಪ್ರತ್ಯಾಗಮ COPY_FD_READ_ERROR;

      n_bytes -= n_read;

      ಯದಿ (n_read == 0 && n_bytes != 0)
        ಪ್ರತ್ಯಾಗಮ COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  ಪ್ರತ್ಯಾಗಮ COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
ಸ್ಥಿರ ಶೂನ್ಯ
elseek_diagnostic (off_t offset, ಪೂರ್ಣಾಙ್ಕ whence, ವರ್ಣ ಧ್ರುವ *filename)
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

ಸ್ಥಿರ off_t
elseek (ಪೂರ್ಣಾಙ್ಕ fd, off_t offset, ಪೂರ್ಣಾಙ್ಕ whence, ವರ್ಣ ಧ್ರುವ *filename)
{
  off_t new_offset = lseek (fd, offset, whence);

  ಯದಿ (new_offset < 0)
    elseek_diagnostic (offset, whence, filename);

  ಪ್ರತ್ಯಾಗಮ new_offset;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
ಸ್ಥಿರ bool
elide_tail_bytes_pipe (ವರ್ಣ ಧ್ರುವ *filename, ಪೂರ್ಣಾಙ್ಕ fd, uintmax_t n_elide,
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

#ಯದಿ HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD < 2 * READ_BUFSIZE
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
  ಯದಿ (n_elide <= HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD)
    {
      idx_t in_elide = n_elide;
      bool first = true;
      bool eof = false;
      idx_t n_to_read = READ_BUFSIZE + n_elide;
      ವರ್ಣ *b[2];
      b[0] = xnmalloc (2, n_to_read);
      b[1] = b[0] + n_to_read;

      ಪ್ರತಿ (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = full_read (fd, b[i], n_to_read);
          idx_t delta = 0;
          ಯದಿ (n_read < n_to_read)
            {
              ಯದಿ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  ಭಙ್ಗ;
                }

              /* reached EOF */
              ಯದಿ (n_read <= in_elide)
                {
                  ಯದಿ (first)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  ಅನ್ಯಥಾ
                    {
                      delta = in_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          ಯದಿ (! first)
            {
              desired_pos += in_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, in_elide - delta);
            }
          first = false;

          ಯದಿ (in_elide < n_read)
            {
              desired_pos += n_read - in_elide;
              xwrite_stdout (b[i], n_read - in_elide);
            }
        }

      ಮುಕ್ತ (b[0]);
    }
  ಅನ್ಯಥಾ
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      ವರ್ಣ **b = ರಿಕ್ತ;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_next;
      ಪ್ರತಿ (i = 0, i_next = 1; !eof; i = i_next, i_next = (i_next + 1) % n_bufs)
        {
          ಯದಿ (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         ಪರಿಮಾಣ *b);

          ಯದಿ (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = full_read (fd, b[i], READ_BUFSIZE);
          ಯದಿ (n_read < READ_BUFSIZE)
            {
              ಯದಿ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  ಗಚ್ಛ free_mem;
                }
              eof = true;
            }

          ಯದಿ (i + 1 == n_bufs)
            buffered_enough = true;

          ಯದಿ (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_next], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      ಯದಿ (buffered_enough)
        {
          idx_t n_bytes_ಎಡ_in_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          ಯದಿ (rem < n_bytes_ಎಡ_in_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          ಅನ್ಯಥಾ
            {
              xwrite_stdout (b[i] + n_read, n_bytes_ಎಡ_in_b_i);
              xwrite_stdout (b[i_next], rem - n_bytes_ಎಡ_in_b_i);
            }
        }
      ಅನ್ಯಥಾ ಯದಿ (i + 1 == n_bufs)
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
      ಪ್ರತಿ (i = 0; i < n_alloc; i++)
        ಮುಕ್ತ (b[i]);
      ಮುಕ್ತ (b);
    }

  ಯದಿ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  ಪ್ರತ್ಯಾಗಮ ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

ಸ್ಥಿರ bool
elide_tail_bytes_file (ವರ್ಣ ಧ್ರುವ *filename, ಪೂರ್ಣಾಙ್ಕ fd, uintmax_t n_elide,
                       ಸಂರಚನಾ stat ಧ್ರುವ *st, off_t current_pos)
{
  off_t size = st->st_size;
  ಯದಿ (current_pos < 0 || size <= STP_BLKSIZE (st))
    ಪ್ರತ್ಯಾಗಮ elide_tail_bytes_pipe (filename, fd, n_elide, current_pos);
  ಅನ್ಯಥಾ
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = size - current_pos;
      off_t bytes_remaining = diff < 0 ? 0 : diff;

      ಯದಿ (bytes_remaining <= n_elide)
        ಪ್ರತ್ಯಾಗಮ true;

      ಗಣನಾ Copy_fd_status err = copy_fd (fd, bytes_remaining - n_elide);
      ಯದಿ (err == COPY_FD_OK)
        ಪ್ರತ್ಯಾಗಮ true;

      diagnose_copy_fd_failure (err, filename);
      ಪ್ರತ್ಯಾಗಮ false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

ಸ್ಥಿರ bool
elide_tail_ಸಾಲುಗಳು_pipe (ವರ್ಣ ಧ್ರುವ *filename, ಪೂರ್ಣಾಙ್ಕ fd, uintmax_t n_elide,
                       off_t current_pos)
{
  ಸಂರಚನಾ linebuffer
  {
    ವರ್ಣ buffer[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    ಸಂರಚನಾ linebuffer *next;
  };
  off_t desired_pos = current_pos;
  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  size_t total_ಸಾಲುಗಳು = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (ಪರಿಮಾಣ (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = ರಿಕ್ತ;
  tmp = xmalloc (ಪರಿಮಾಣ (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  ಯಾವತ್ (true)
    {
      n_read = ಪಠ (fd, tmp->buffer, BUFSIZ);
      ಯದಿ (n_read <= 0)
        ಭಙ್ಗ;

      ಯದಿ (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->buffer, n_read);
          ಅನುವರ್ತ;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = ರಿಕ್ತ;

      /* Count the number of newlines just read.  */
      {
        ವರ್ಣ *buffer_end = tmp->buffer + n_read;
        *buffer_end = ಸಾಲು_end;
        ವರ್ಣ ಧ್ರುವ *p = tmp->buffer;
        ಯಾವತ್ ((p = rawmemchr (p, ಸಾಲು_end)) < buffer_end)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_ಸಾಲುಗಳು += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      ಯದಿ (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      ಅನ್ಯಥಾ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          ಯದಿ (n_elide < total_ಸಾಲುಗಳು - first->nlines)
            {
              desired_pos += first->nbytes;
              xwrite_stdout (first->buffer, first->nbytes);
              tmp = first;
              total_ಸಾಲುಗಳು -= first->nlines;
              first = first->next;
            }
          ಅನ್ಯಥಾ
            tmp = xmalloc (ಪರಿಮಾಣ (LBUFFER));
        }
    }

  ಮುಕ್ತ (tmp);

  ಯದಿ (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      ಗಚ್ಛ free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  ಯದಿ (last->nbytes && last->buffer[last->nbytes - 1] != ಸಾಲು_end)
    {
      ++last->nlines;
      ++total_ಸಾಲುಗಳು;
    }

  ಪ್ರತಿ (tmp = first; n_elide < total_ಸಾಲುಗಳು - tmp->nlines; tmp = tmp->next)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->buffer, tmp->nbytes);
      total_ಸಾಲುಗಳು -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  ಯದಿ (n_elide < total_ಸಾಲುಗಳು)
    {
      size_t n = total_ಸಾಲುಗಳು - n_elide;
      ವರ್ಣ ಧ್ರುವ *buffer_end = tmp->buffer + tmp->nbytes;
      ವರ್ಣ ಧ್ರುವ *p = tmp->buffer;
      ಯಾವತ್ (n && (p = memchr (p, ಸಾಲು_end, buffer_end - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->buffer;
      xwrite_stdout (tmp->buffer, p - tmp->buffer);
    }

free_lbuffers:
  ಯಾವತ್ (first)
    {
      tmp = first->next;
      ಮುಕ್ತ (first);
      first = tmp;
    }

  ಯದಿ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  ಪ್ರತ್ಯಾಗಮ ok;
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
ಸ್ಥಿರ bool
elide_tail_ಸಾಲುಗಳು_seekable (ವರ್ಣ ಧ್ರುವ *pretty_filename, ಪೂರ್ಣಾಙ್ಕ fd,
                           uintmax_t n_ಸಾಲುಗಳು,
                           off_t start_pos, off_t size)
{
  ವರ್ಣ buffer[BUFSIZ];
  ssize_t bytes_read;
  off_t pos = size;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  bytes_read = (pos - start_pos) % BUFSIZ;
  ಯದಿ (bytes_read == 0)
    bytes_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= bytes_read;
  ಯದಿ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    ಪ್ರತ್ಯಾಗಮ false;
  bytes_read = ಪಠ (fd, buffer, bytes_read);
  ಯದಿ (bytes_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      ಪ್ರತ್ಯಾಗಮ false;
    }

  /* n_lines == 0 case needs special treatment. */
  ಧ್ರುವ bool all_ಸಾಲುಗಳು = !n_ಸಾಲುಗಳು;

  /* Count the incomplete line on files that don't end with a newline.  */
  ಯದಿ (n_ಸಾಲುಗಳು && bytes_read && buffer[bytes_read - 1] != ಸಾಲು_end)
    --n_ಸಾಲುಗಳು;

  ಯಾವತ್ (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      ಯಾವತ್ (n)
        {
          ಯದಿ (all_ಸಾಲುಗಳು)
            n -= 1;
          ಅನ್ಯಥಾ
            {
              ವರ್ಣ ಧ್ರುವ *nl;
              nl = memrchr (buffer, ಸಾಲು_end, n);
              ಯದಿ (nl == ರಿಕ್ತ)
                ಭಙ್ಗ;
              n = nl - buffer;
            }
          ಯದಿ (n_ಸಾಲುಗಳು-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              ಯದಿ (start_pos < pos)
                {
                  ಗಣನಾ Copy_fd_status err;
                  ಯದಿ (elseek (fd, start_pos, SEEK_SET, pretty_filename) < 0)
                    ಪ್ರತ್ಯಾಗಮ false;

                  err = copy_fd (fd, pos - start_pos);
                  ಯದಿ (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      ಪ್ರತ್ಯಾಗಮ false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (buffer, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              ಪ್ರತ್ಯಾಗಮ 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      ಯದಿ (pos == start_pos)
        {
          /* Not enough lines in the file.  */
          ಪ್ರತ್ಯಾಗಮ true;
        }
      pos -= BUFSIZ;
      ಯದಿ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        ಪ್ರತ್ಯಾಗಮ false;

      bytes_read = ಪಠ (fd, buffer, BUFSIZ);
      ಯದಿ (bytes_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          ಪ್ರತ್ಯಾಗಮ false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      ಯದಿ (bytes_read == 0)
        ಪ್ರತ್ಯಾಗಮ true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

ಸ್ಥಿರ bool
elide_tail_ಸಾಲುಗಳು_file (ವರ್ಣ ಧ್ರುವ *filename, ಪೂರ್ಣಾಙ್ಕ fd, uintmax_t n_elide,
                       ಸಂರಚನಾ stat ಧ್ರುವ *st, off_t current_pos)
{
  off_t size = st->st_size;
  ಯದಿ (current_pos < 0 || size <= STP_BLKSIZE (st))
    ಪ್ರತ್ಯಾಗಮ elide_tail_ಸಾಲುಗಳು_pipe (filename, fd, n_elide, current_pos);
  ಅನ್ಯಥಾ
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      ಪ್ರತ್ಯಾಗಮ (size <= current_pos
              || elide_tail_ಸಾಲುಗಳು_seekable (filename, fd, n_elide,
                                            current_pos, size));
    }
}

ಸ್ಥಿರ bool
head_bytes (ವರ್ಣ ಧ್ರುವ *filename, ಪೂರ್ಣಾಙ್ಕ fd, uintmax_t bytes_to_write)
{
  ಯದಿ (copy_fd (fd, bytes_to_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      ಪ್ರತ್ಯಾಗಮ false;
    }
  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
head_ಸಾಲುಗಳು (ವರ್ಣ ಧ್ರುವ *filename, ಪೂರ್ಣಾಙ್ಕ fd, uintmax_t ಸಾಲುಗಳು_to_write)
{
  ವರ್ಣ buffer[BUFSIZ];

  ಯಾವತ್ (ಸಾಲುಗಳು_to_write)
    {
      ssize_t bytes_read = ಪಠ (fd, buffer, BUFSIZ);
      idx_t bytes_to_write = 0;

      ಯದಿ (bytes_read < 0)
        {
          diagnose_read_failure (filename);
          ಪ್ರತ್ಯಾಗಮ false;
        }
      ಯದಿ (bytes_read == 0)
        ಭಙ್ಗ;
      ಯಾವತ್ (bytes_to_write < bytes_read)
        ಯದಿ (buffer[bytes_to_write++] == ಸಾಲು_end && --ಸಾಲುಗಳು_to_write == 0)
          {
            off_t n_bytes_past_EOL = bytes_read - bytes_to_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            ಯದಿ (lseek (fd, -n_bytes_past_EOL, SEEK_CUR) < 0)
              {
                ಸಂರಚನಾ stat st;
                ಯದಿ (fstat (fd, &st) != 0 || S_ISREG (st.st_mode))
                  elseek_diagnostic (-n_bytes_past_EOL, SEEK_CUR, filename);
              }
            ಭಙ್ಗ;
          }
      xwrite_stdout (buffer, bytes_to_write);
    }
  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
head (ವರ್ಣ ಧ್ರುವ *filename, ಪೂರ್ಣಾಙ್ಕ fd, uintmax_t n_units, bool ಎಣಿಕೆ_ಸಾಲುಗಳು,
      bool elide_from_end)
{
  ಯದಿ (print_headers)
    write_header (filename);

  ಯದಿ (elide_from_end)
    {
      /* Optimize for "infinite" elisions.  */
      ಯದಿ (n_units == UINTMAX_MAX)
        ಪ್ರತ್ಯಾಗಮ true;

      off_t current_pos = -1;
      ಸಂರಚನಾ stat st;
      ಯದಿ (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          ಪ್ರತ್ಯಾಗಮ false;
        }
      ಯದಿ (! presume_ಒಳಹರಿವು_pipe && S_ISREG (st.st_mode))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          ಯದಿ (current_pos < 0)
            ಪ್ರತ್ಯಾಗಮ false;
        }
      ಯದಿ (ಎಣಿಕೆ_ಸಾಲುಗಳು)
        ಪ್ರತ್ಯಾಗಮ elide_tail_ಸಾಲುಗಳು_file (filename, fd, n_units, &st, current_pos);
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ elide_tail_bytes_file (filename, fd, n_units, &st, current_pos);
    }
  ಯದಿ (ಎಣಿಕೆ_ಸಾಲುಗಳು)
    ಪ್ರತ್ಯಾಗಮ head_ಸಾಲುಗಳು (filename, fd, n_units);
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ head_bytes (filename, fd, n_units);
}

ಸ್ಥಿರ bool
head_file (ವರ್ಣ ಧ್ರುವ *filename, uintmax_t n_units, bool ಎಣಿಕೆ_ಸಾಲುಗಳು,
           bool elide_from_end)
{
  ಪೂರ್ಣಾಙ್ಕ fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  ಯದಿ (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_ದ್ವಿಮಾನ_mode (STDIN_FILENO, O_ದ್ವಿಮಾನ);
    }
  ಅನ್ಯಥಾ
    {
      fd = open (filename, O_RDONLY | O_ದ್ವಿಮಾನ);
      ಯದಿ (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          ಪ್ರತ್ಯಾಗಮ false;
        }
    }

  ok = head (filename, fd, n_units, ಎಣಿಕೆ_ಸಾಲುಗಳು, elide_from_end);
  ಯದಿ (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      ಪ್ರತ್ಯಾಗಮ false;
    }
  ಪ್ರತ್ಯಾಗಮ ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

ಸ್ಥಿರ uintmax_t
string_to_integer (bool ಎಣಿಕೆ_ಸಾಲುಗಳು, ವರ್ಣ ಧ್ರುವ *n_string)
{
  ಪ್ರತ್ಯಾಗಮ xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (ಎಣಿಕೆ_ಸಾಲುಗಳು
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಗಣನಾ header_mode header_mode = multiple_files;
  bool ok = true;
  ಪೂರ್ಣಾಙ್ಕ c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_ಸಂಖ್ಯೆ;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool ಎಣಿಕೆ_ಸಾಲುಗಳು = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_end = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ default_file_list[] = {"-", ರಿಕ್ತ};
  ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ *file_list;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯದಿ (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      ವರ್ಣ *a = argv[1];
      ವರ್ಣ *n_string = ++a;
      ವರ್ಣ *end_n_string;
      ವರ್ಣ multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      ಕುರು ++a;
      ಯಾವತ್ (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      end_n_string = a;

      /* Parse any appended option letters. */
      ಪ್ರತಿ (; *a; a++)
        {
          ವಿಕಲ್ಪ (*a)
            {
            ಪಕ್ಷ 'c':
              ಎಣಿಕೆ_ಸಾಲುಗಳು = false;
              multiplier_char = 0;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'b':
            ಪಕ್ಷ 'k':
            ಪಕ್ಷ 'm':
              ಎಣಿಕೆ_ಸಾಲುಗಳು = false;
              multiplier_char = *a;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'l':
              ಎಣಿಕೆ_ಸಾಲುಗಳು = true;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'q':
              header_mode = never;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'v':
              header_mode = always;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'z':
              ಸಾಲು_end = '\0';
              ಭಙ್ಗ;

            ಸ್ವಭಾವ:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *end_n_string = multiplier_char;
      ಯದಿ (multiplier_char)
        *(++end_n_string) = 0;

      n_units = string_to_integer (ಎಣಿಕೆ_ಸಾಲುಗಳು, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  ಯಾವತ್ ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ PRESUME_ಒಳಹರಿವು_PIPE_OPTION:
          presume_ಒಳಹರಿವು_pipe = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'c':
          ಎಣಿಕೆ_ಸಾಲುಗಳು = false;
          elide_from_end = (*optarg == '-');
          ಯದಿ (elide_from_end)
            ++optarg;
          n_units = string_to_integer (ಎಣಿಕೆ_ಸಾಲುಗಳು, optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'n':
          ಎಣಿಕೆ_ಸಾಲುಗಳು = true;
          elide_from_end = (*optarg == '-');
          ಯದಿ (elide_from_end)
            ++optarg;
          n_units = string_to_integer (ಎಣಿಕೆ_ಸಾಲುಗಳು, optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'q':
          header_mode = never;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'v':
          header_mode = always;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'z':
          ಸಾಲು_end = '\0';
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          ಯದಿ (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (header_mode == always
      || (header_mode == multiple_files && optind < argc - 1))
    print_headers = true;

  file_list = (optind < argc
               ? (ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ *) &argv[optind]
               : default_file_list);

  xset_ದ್ವಿಮಾನ_mode (STDOUT_FILENO, O_ದ್ವಿಮಾನ);

  ಪ್ರತಿ (size_t i = 0; file_list[i]; ++i)
    ok &= head_file (file_list[i], n_units, ಎಣಿಕೆ_ಸಾಲುಗಳು, elide_from_end);

  ಯದಿ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
