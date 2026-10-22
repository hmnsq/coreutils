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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"

#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "full-read.h"
#অন্তর্ভাব "stat-size.h"
#অন্তর্ভাব "xbinary-io.h"
#অন্তর্ভাব "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "head"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#define DEFAULT_সংখ্যা 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
স্থির bool presume_প্রবেশ_pipe;

/* If true, print filename headers. */
স্থির bool print_headers;

/* Character to split lines by. */
স্থির অক্ষর সারি_end = '\n';

/* When to print the filename banners. */
গণনা header_mode
{
  multiple_files, always, never
};

/* Have we ever read standard input?  */
স্থির bool have_read_stdin;

গণনা Copy_fd_status
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  PRESUME_প্রবেশ_PIPE_OPTION = CHAR_MAX + 1
};

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"bytes", required_argument, ফাঁকা, 'c'},
  {"lines", required_argument, ফাঁকা, 'n'},
  {"-presume-input-pipe", no_argument, ফাঁকা,
   PRESUME_প্রবেশ_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, ফাঁকা, 'q'},
  {"silent", no_argument, ফাঁকা, 'q'},
  {"verbose", no_argument, ফাঁকা, 'v'},
  {"zero-terminated", no_argument, ফাঁকা, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      printf (_("\
Print the first %d সারিগুলি of each FILE to standard output.\n\
With more than one FILE, precede each with a header giving the file name.\n\
"), DEFAULT_সংখ্যা);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --bytes=[-]অঙ্ক_2\n\
         print the first অঙ্ক_2 bytes of each file;\n\
         with the leading '-', print all but the last অঙ্ক_2 bytes of each file\n\
"));
      oprintf (_("\
  -n, --সারিগুলি=[-]অঙ্ক_2\n\
         print the first অঙ্ক_2 সারিগুলি instead of the first %d;\n\
         with the leading '-', print all but the last অঙ্ক_2 সারিগুলি of each file\n\
"), DEFAULT_সংখ্যা);
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
         সারি delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
অঙ্ক_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on জন্য T, P, E, Z, Y, R, Q.\n\
দ্বিমিক_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

স্থির শূন্য
diagnose_read_failure (অক্ষর ধ্রুবক *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

স্থির শূন্য
diagnose_copy_fd_failure (গণনা Copy_fd_status err, অক্ষর ধ্রুবক *filename)
{
  নির্বাচন (err)
    {
    ক্ষেত্র COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      বিরতি;
    ক্ষেত্র COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      বিরতি;
    ক্ষেত্র COPY_FD_OK: পূর্বনির্ধারিত:
      affirm (false);
    }
}

স্থির শূন্য
write_header (অক্ষর ধ্রুবক *filename)
{
  স্থির bool first_file = true;

  printf ("%s==> %s <==\n", (first_file ? "" : "\n"), quotef (filename));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

স্থির শূন্য
xwrite_stdout (অক্ষর ধ্রুবক *বাফার, size_t n_bytes)
{
  যদি (n_bytes > 0 && fwrite (বাফার, 1, n_bytes, stdout) < n_bytes)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

স্থির গণনা Copy_fd_status
copy_fd (পূর্ণসংখ্যা src_fd, uintmax_t n_bytes)
{
  অক্ষর buf[BUFSIZ];

  /* Copy the file contents.  */
  যখন (0 < n_bytes)
    {
      idx_t n_to_read = MIN (n_bytes, আকার buf);
      ssize_t n_read = পঠ (src_fd, buf, n_to_read);
      যদি (n_read < 0)
        ফেরত COPY_FD_READ_ERROR;

      n_bytes -= n_read;

      যদি (n_read == 0 && n_bytes != 0)
        ফেরত COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  ফেরত COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
স্থির শূন্য
elseek_diagnostic (off_t offset, পূর্ণসংখ্যা whence, অক্ষর ধ্রুবক *filename)
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

স্থির off_t
elseek (পূর্ণসংখ্যা fd, off_t offset, পূর্ণসংখ্যা whence, অক্ষর ধ্রুবক *filename)
{
  off_t new_offset = lseek (fd, offset, whence);

  যদি (new_offset < 0)
    elseek_diagnostic (offset, whence, filename);

  ফেরত new_offset;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
স্থির bool
elide_লেজ_bytes_pipe (অক্ষর ধ্রুবক *filename, পূর্ণসংখ্যা fd, uintmax_t n_elide,
                       off_t current_pos)
{
  uintmax_t desired_pos = current_pos;
  bool ok = true;

#ifndef শীর্ষ_লেজ_PIPE_READ_BUFSIZE
# define শীর্ষ_লেজ_PIPE_READ_BUFSIZE BUFSIZ
#endif
#define READ_BUFSIZE শীর্ষ_লেজ_PIPE_READ_BUFSIZE

  /* If we're eliding no more than this many bytes, then it's ok to allocate
     more memory in order to use a more time-efficient algorithm.
     FIXME: use a fraction of available memory instead, as in sort.
     FIXME: is this even worthwhile?  */
#ifndef শীর্ষ_লেজ_PIPE_BYTECOUNT_THRESHOLD
# define শীর্ষ_লেজ_PIPE_BYTECOUNT_THRESHOLD 1024 * 1024
#endif

#যদি শীর্ষ_লেজ_PIPE_BYTECOUNT_THRESHOLD < 2 * READ_BUFSIZE
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
  static_assert (শীর্ষ_লেজ_PIPE_BYTECOUNT_THRESHOLD <= IDX_MAX - READ_BUFSIZE);
  যদি (n_elide <= শীর্ষ_লেজ_PIPE_BYTECOUNT_THRESHOLD)
    {
      idx_t in_elide = n_elide;
      bool first = true;
      bool eof = false;
      idx_t n_to_read = READ_BUFSIZE + n_elide;
      অক্ষর *b[2];
      b[0] = xnmalloc (2, n_to_read);
      b[1] = b[0] + n_to_read;

      জন্য (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = full_read (fd, b[i], n_to_read);
          idx_t delta = 0;
          যদি (n_read < n_to_read)
            {
              যদি (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  বিরতি;
                }

              /* reached EOF */
              যদি (n_read <= in_elide)
                {
                  যদি (first)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  অন্যথা
                    {
                      delta = in_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          যদি (! first)
            {
              desired_pos += in_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, in_elide - delta);
            }
          first = false;

          যদি (in_elide < n_read)
            {
              desired_pos += n_read - in_elide;
              xwrite_stdout (b[i], n_read - in_elide);
            }
        }

      মুক্ত (b[0]);
    }
  অন্যথা
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      অক্ষর **b = ফাঁকা;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_পরবর্তী;
      জন্য (i = 0, i_পরবর্তী = 1; !eof; i = i_পরবর্তী, i_পরবর্তী = (i_পরবর্তী + 1) % n_bufs)
        {
          যদি (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         আকার *b);

          যদি (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = full_read (fd, b[i], READ_BUFSIZE);
          যদি (n_read < READ_BUFSIZE)
            {
              যদি (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  যাও free_mem;
                }
              eof = true;
            }

          যদি (i + 1 == n_bufs)
            buffered_enough = true;

          যদি (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_পরবর্তী], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      যদি (buffered_enough)
        {
          idx_t n_bytes_বাম_in_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          যদি (rem < n_bytes_বাম_in_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          অন্যথা
            {
              xwrite_stdout (b[i] + n_read, n_bytes_বাম_in_b_i);
              xwrite_stdout (b[i_পরবর্তী], rem - n_bytes_বাম_in_b_i);
            }
        }
      অন্যথা যদি (i + 1 == n_bufs)
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
          xwrite_stdout (b[i_পরবর্তী], x);
        }

    free_mem:
      জন্য (i = 0; i < n_alloc; i++)
        মুক্ত (b[i]);
      মুক্ত (b);
    }

  যদি (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  ফেরত ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

স্থির bool
elide_লেজ_bytes_file (অক্ষর ধ্রুবক *filename, পূর্ণসংখ্যা fd, uintmax_t n_elide,
                       কাঠামো stat ধ্রুবক *st, off_t current_pos)
{
  off_t আকার_2 = st->st_আকার;
  যদি (current_pos < 0 || আকার_2 <= STP_BLKSIZE (st))
    ফেরত elide_লেজ_bytes_pipe (filename, fd, n_elide, current_pos);
  অন্যথা
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = আকার_2 - current_pos;
      off_t bytes_remaining = diff < 0 ? 0 : diff;

      যদি (bytes_remaining <= n_elide)
        ফেরত true;

      গণনা Copy_fd_status err = copy_fd (fd, bytes_remaining - n_elide);
      যদি (err == COPY_FD_OK)
        ফেরত true;

      diagnose_copy_fd_failure (err, filename);
      ফেরত false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

স্থির bool
elide_লেজ_সারিগুলি_pipe (অক্ষর ধ্রুবক *filename, পূর্ণসংখ্যা fd, uintmax_t n_elide,
                       off_t current_pos)
{
  কাঠামো linebuffer
  {
    অক্ষর বাফার[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    কাঠামো linebuffer *পরবর্তী;
  };
  off_t desired_pos = current_pos;
  প্রকারলেখ কাঠামো linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  size_t যোগ_সারিগুলি = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (আকার (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->পরবর্তী = ফাঁকা;
  tmp = xmalloc (আকার (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  যখন (true)
    {
      n_read = পঠ (fd, tmp->বাফার, BUFSIZ);
      যদি (n_read <= 0)
        বিরতি;

      যদি (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->বাফার, n_read);
          চালু;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->পরবর্তী = ফাঁকা;

      /* Count the number of newlines just read.  */
      {
        অক্ষর *বাফার_end = tmp->বাফার + n_read;
        *বাফার_end = সারি_end;
        অক্ষর ধ্রুবক *p = tmp->বাফার;
        যখন ((p = rawmemchr (p, সারি_end)) < বাফার_end)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      যোগ_সারিগুলি += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      যদি (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->বাফার[last->nbytes], tmp->বাফার, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      অন্যথা
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->পরবর্তী = tmp;
          যদি (n_elide < যোগ_সারিগুলি - first->nlines)
            {
              desired_pos += first->nbytes;
              xwrite_stdout (first->বাফার, first->nbytes);
              tmp = first;
              যোগ_সারিগুলি -= first->nlines;
              first = first->পরবর্তী;
            }
          অন্যথা
            tmp = xmalloc (আকার (LBUFFER));
        }
    }

  মুক্ত (tmp);

  যদি (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      যাও free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  যদি (last->nbytes && last->বাফার[last->nbytes - 1] != সারি_end)
    {
      ++last->nlines;
      ++যোগ_সারিগুলি;
    }

  জন্য (tmp = first; n_elide < যোগ_সারিগুলি - tmp->nlines; tmp = tmp->পরবর্তী)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->বাফার, tmp->nbytes);
      যোগ_সারিগুলি -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  যদি (n_elide < যোগ_সারিগুলি)
    {
      size_t n = যোগ_সারিগুলি - n_elide;
      অক্ষর ধ্রুবক *বাফার_end = tmp->বাফার + tmp->nbytes;
      অক্ষর ধ্রুবক *p = tmp->বাফার;
      যখন (n && (p = memchr (p, সারি_end, বাফার_end - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->বাফার;
      xwrite_stdout (tmp->বাফার, p - tmp->বাফার);
    }

free_lbuffers:
  যখন (first)
    {
      tmp = first->পরবর্তী;
      মুক্ত (first);
      first = tmp;
    }

  যদি (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  ফেরত ok;
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
স্থির bool
elide_লেজ_সারিগুলি_seekable (অক্ষর ধ্রুবক *pretty_filename, পূর্ণসংখ্যা fd,
                           uintmax_t n_সারিগুলি,
                           off_t start_pos, off_t আকার_2)
{
  অক্ষর বাফার[BUFSIZ];
  ssize_t bytes_read;
  off_t pos = আকার_2;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  bytes_read = (pos - start_pos) % BUFSIZ;
  যদি (bytes_read == 0)
    bytes_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= bytes_read;
  যদি (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    ফেরত false;
  bytes_read = পঠ (fd, বাফার, bytes_read);
  যদি (bytes_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      ফেরত false;
    }

  /* n_lines == 0 case needs special treatment. */
  ধ্রুবক bool all_সারিগুলি = !n_সারিগুলি;

  /* Count the incomplete line on files that don't end with a newline.  */
  যদি (n_সারিগুলি && bytes_read && বাফার[bytes_read - 1] != সারি_end)
    --n_সারিগুলি;

  যখন (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      যখন (n)
        {
          যদি (all_সারিগুলি)
            n -= 1;
          অন্যথা
            {
              অক্ষর ধ্রুবক *nl;
              nl = memrchr (বাফার, সারি_end, n);
              যদি (nl == ফাঁকা)
                বিরতি;
              n = nl - বাফার;
            }
          যদি (n_সারিগুলি-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              যদি (start_pos < pos)
                {
                  গণনা Copy_fd_status err;
                  যদি (elseek (fd, start_pos, SEEK_SET, pretty_filename) < 0)
                    ফেরত false;

                  err = copy_fd (fd, pos - start_pos);
                  যদি (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      ফেরত false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (বাফার, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              ফেরত 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      যদি (pos == start_pos)
        {
          /* Not enough lines in the file.  */
          ফেরত true;
        }
      pos -= BUFSIZ;
      যদি (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        ফেরত false;

      bytes_read = পঠ (fd, বাফার, BUFSIZ);
      যদি (bytes_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          ফেরত false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      যদি (bytes_read == 0)
        ফেরত true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

স্থির bool
elide_লেজ_সারিগুলি_file (অক্ষর ধ্রুবক *filename, পূর্ণসংখ্যা fd, uintmax_t n_elide,
                       কাঠামো stat ধ্রুবক *st, off_t current_pos)
{
  off_t আকার_2 = st->st_আকার;
  যদি (current_pos < 0 || আকার_2 <= STP_BLKSIZE (st))
    ফেরত elide_লেজ_সারিগুলি_pipe (filename, fd, n_elide, current_pos);
  অন্যথা
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      ফেরত (আকার_2 <= current_pos
              || elide_লেজ_সারিগুলি_seekable (filename, fd, n_elide,
                                            current_pos, আকার_2));
    }
}

স্থির bool
শীর্ষ_bytes (অক্ষর ধ্রুবক *filename, পূর্ণসংখ্যা fd, uintmax_t bytes_to_write)
{
  যদি (copy_fd (fd, bytes_to_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      ফেরত false;
    }
  ফেরত true;
}

স্থির bool
শীর্ষ_সারিগুলি (অক্ষর ধ্রুবক *filename, পূর্ণসংখ্যা fd, uintmax_t সারিগুলি_to_write)
{
  অক্ষর বাফার[BUFSIZ];

  যখন (সারিগুলি_to_write)
    {
      ssize_t bytes_read = পঠ (fd, বাফার, BUFSIZ);
      idx_t bytes_to_write = 0;

      যদি (bytes_read < 0)
        {
          diagnose_read_failure (filename);
          ফেরত false;
        }
      যদি (bytes_read == 0)
        বিরতি;
      যখন (bytes_to_write < bytes_read)
        যদি (বাফার[bytes_to_write++] == সারি_end && --সারিগুলি_to_write == 0)
          {
            off_t n_bytes_past_EOL = bytes_read - bytes_to_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            যদি (lseek (fd, -n_bytes_past_EOL, SEEK_CUR) < 0)
              {
                কাঠামো stat st;
                যদি (fstat (fd, &st) != 0 || S_ISREG (st.st_mode))
                  elseek_diagnostic (-n_bytes_past_EOL, SEEK_CUR, filename);
              }
            বিরতি;
          }
      xwrite_stdout (বাফার, bytes_to_write);
    }
  ফেরত true;
}

স্থির bool
শীর্ষ (অক্ষর ধ্রুবক *filename, পূর্ণসংখ্যা fd, uintmax_t n_units, bool গুনতি_সারিগুলি,
      bool elide_from_end)
{
  যদি (print_headers)
    write_header (filename);

  যদি (elide_from_end)
    {
      /* Optimize for "infinite" elisions.  */
      যদি (n_units == UINTMAX_MAX)
        ফেরত true;

      off_t current_pos = -1;
      কাঠামো stat st;
      যদি (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          ফেরত false;
        }
      যদি (! presume_প্রবেশ_pipe && S_ISREG (st.st_mode))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          যদি (current_pos < 0)
            ফেরত false;
        }
      যদি (গুনতি_সারিগুলি)
        ফেরত elide_লেজ_সারিগুলি_file (filename, fd, n_units, &st, current_pos);
      অন্যথা
        ফেরত elide_লেজ_bytes_file (filename, fd, n_units, &st, current_pos);
    }
  যদি (গুনতি_সারিগুলি)
    ফেরত শীর্ষ_সারিগুলি (filename, fd, n_units);
  অন্যথা
    ফেরত শীর্ষ_bytes (filename, fd, n_units);
}

স্থির bool
শীর্ষ_file (অক্ষর ধ্রুবক *filename, uintmax_t n_units, bool গুনতি_সারিগুলি,
           bool elide_from_end)
{
  পূর্ণসংখ্যা fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  যদি (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_দ্বিমিক_mode (STDIN_FILENO, O_দ্বিমিক);
    }
  অন্যথা
    {
      fd = open (filename, O_RDONLY | O_দ্বিমিক);
      যদি (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          ফেরত false;
        }
    }

  ok = শীর্ষ (filename, fd, n_units, গুনতি_সারিগুলি, elide_from_end);
  যদি (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      ফেরত false;
    }
  ফেরত ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

স্থির uintmax_t
string_to_integer (bool গুনতি_সারিগুলি, অক্ষর ধ্রুবক *n_string)
{
  ফেরত xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (গুনতি_সারিগুলি
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  গণনা header_mode header_mode = multiple_files;
  bool ok = true;
  পূর্ণসংখ্যা c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_সংখ্যা;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool গুনতি_সারিগুলি = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_end = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  স্থির অক্ষর ধ্রুবক *ধ্রুবক default_file_list[] = {"-", ফাঁকা};
  অক্ষর ধ্রুবক *ধ্রুবক *file_list;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যদি (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      অক্ষর *a = argv[1];
      অক্ষর *n_string = ++a;
      অক্ষর *end_n_string;
      অক্ষর multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      করো ++a;
      যখন (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      end_n_string = a;

      /* Parse any appended option letters. */
      জন্য (; *a; a++)
        {
          নির্বাচন (*a)
            {
            ক্ষেত্র 'c':
              গুনতি_সারিগুলি = false;
              multiplier_char = 0;
              বিরতি;

            ক্ষেত্র 'b':
            ক্ষেত্র 'k':
            ক্ষেত্র 'm':
              গুনতি_সারিগুলি = false;
              multiplier_char = *a;
              বিরতি;

            ক্ষেত্র 'l':
              গুনতি_সারিগুলি = true;
              বিরতি;

            ক্ষেত্র 'q':
              header_mode = never;
              বিরতি;

            ক্ষেত্র 'v':
              header_mode = always;
              বিরতি;

            ক্ষেত্র 'z':
              সারি_end = '\0';
              বিরতি;

            পূর্বনির্ধারিত:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *end_n_string = multiplier_char;
      যদি (multiplier_char)
        *(++end_n_string) = 0;

      n_units = string_to_integer (গুনতি_সারিগুলি, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  যখন ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, ফাঁকা))
         != -1)
    {
      নির্বাচন (c)
        {
        ক্ষেত্র PRESUME_প্রবেশ_PIPE_OPTION:
          presume_প্রবেশ_pipe = true;
          বিরতি;

        ক্ষেত্র 'c':
          গুনতি_সারিগুলি = false;
          elide_from_end = (*optarg == '-');
          যদি (elide_from_end)
            ++optarg;
          n_units = string_to_integer (গুনতি_সারিগুলি, optarg);
          বিরতি;

        ক্ষেত্র 'n':
          গুনতি_সারিগুলি = true;
          elide_from_end = (*optarg == '-');
          যদি (elide_from_end)
            ++optarg;
          n_units = string_to_integer (গুনতি_সারিগুলি, optarg);
          বিরতি;

        ক্ষেত্র 'q':
          header_mode = never;
          বিরতি;

        ক্ষেত্র 'v':
          header_mode = always;
          বিরতি;

        ক্ষেত্র 'z':
          সারি_end = '\0';
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          যদি (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  যদি (header_mode == always
      || (header_mode == multiple_files && optind < argc - 1))
    print_headers = true;

  file_list = (optind < argc
               ? (অক্ষর ধ্রুবক *ধ্রুবক *) &argv[optind]
               : default_file_list);

  xset_দ্বিমিক_mode (STDOUT_FILENO, O_দ্বিমিক);

  জন্য (size_t i = 0; file_list[i]; ++i)
    ok &= শীর্ষ_file (file_list[i], n_units, গুনতি_সারিগুলি, elide_from_end);

  যদি (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
