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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"

#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "full-read.h"
#अन्तर्भाव "stat-size.h"
#अन्तर्भाव "xbinary-io.h"
#अन्तर्भाव "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "head"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#define DEFAULT_सङ्ख्या 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
स्थिर bool presume_निवेशः_pipe;

/* If true, print filename headers. */
स्थिर bool print_headers;

/* Character to split lines by. */
स्थिर वर्ण पङ्क्तिः_end = '\n';

/* When to print the filename banners. */
गणना header_mode
{
  multiple_files, always, never
};

/* Have we ever read standard input?  */
स्थिर bool have_read_stdin;

गणना Copy_fd_status
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  PRESUME_निवेशः_PIPE_OPTION = CHAR_MAX + 1
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"bytes", required_argument, रिक्त, 'c'},
  {"lines", required_argument, रिक्त, 'n'},
  {"-presume-input-pipe", no_argument, रिक्त,
   PRESUME_निवेशः_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, रिक्त, 'q'},
  {"silent", no_argument, रिक्त, 'q'},
  {"verbose", no_argument, रिक्त, 'v'},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      printf (_("\
Print the first %d पङ्क्तयः of each FILE to standard output.\n\
With more than one FILE, precede each with a header giving the file name.\n\
"), DEFAULT_सङ्ख्या);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --bytes=[-]अङ्कः_2\n\
         print the first अङ्कः_2 bytes of each file;\n\
         with the leading '-', print all but the last अङ्कः_2 bytes of each file\n\
"));
      oprintf (_("\
  -n, --पङ्क्तयः=[-]अङ्कः_2\n\
         print the first अङ्कः_2 पङ्क्तयः instead of the first %d;\n\
         with the leading '-', print all but the last अङ्कः_2 पङ्क्तयः of each file\n\
"), DEFAULT_सङ्ख्या);
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
         पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
अङ्कः_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on अत्थाय T, P, E, Z, Y, R, Q.\n\
द्विमान_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

स्थिर शून्य
diagnose_read_failure (वर्ण ध्रुव *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

स्थिर शून्य
diagnose_copy_fd_failure (गणना Copy_fd_status err, वर्ण ध्रुव *filename)
{
  विकल्प (err)
    {
    पक्ष COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      भङ्ग;
    पक्ष COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      भङ्ग;
    पक्ष COPY_FD_OK: स्वभाव:
      affirm (false);
    }
}

स्थिर शून्य
write_header (वर्ण ध्रुव *filename)
{
  स्थिर bool first_file = true;

  printf ("%s==> %s <==\n", (first_file ? "" : "\n"), quotef (filename));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

स्थिर शून्य
xwrite_stdout (वर्ण ध्रुव *buffer, size_t n_bytes)
{
  सचे (n_bytes > 0 && fwrite (buffer, 1, n_bytes, stdout) < n_bytes)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

स्थिर गणना Copy_fd_status
copy_fd (पूर्णाङ्क src_fd, uintmax_t n_bytes)
{
  वर्ण buf[BUFSIZ];

  /* Copy the file contents.  */
  याव (0 < n_bytes)
    {
      idx_t n_to_read = MIN (n_bytes, परिमाण buf);
      ssize_t n_read = पठ (src_fd, buf, n_to_read);
      सचे (n_read < 0)
        पटिनिवत्त COPY_FD_READ_ERROR;

      n_bytes -= n_read;

      सचे (n_read == 0 && n_bytes != 0)
        पटिनिवत्त COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  पटिनिवत्त COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
स्थिर शून्य
elseek_diagnostic (off_t offset, पूर्णाङ्क whence, वर्ण ध्रुव *filename)
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

स्थिर off_t
elseek (पूर्णाङ्क fd, off_t offset, पूर्णाङ्क whence, वर्ण ध्रुव *filename)
{
  off_t new_offset = lseek (fd, offset, whence);

  सचे (new_offset < 0)
    elseek_diagnostic (offset, whence, filename);

  पटिनिवत्त new_offset;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
स्थिर bool
elide_tail_bytes_pipe (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t n_elide,
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

#सचे HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD < 2 * READ_BUFSIZE
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
  सचे (n_elide <= HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD)
    {
      idx_t in_elide = n_elide;
      bool first = true;
      bool eof = false;
      idx_t n_to_read = READ_BUFSIZE + n_elide;
      वर्ण *b[2];
      b[0] = xnmalloc (2, n_to_read);
      b[1] = b[0] + n_to_read;

      अत्थाय (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = full_read (fd, b[i], n_to_read);
          idx_t delta = 0;
          सचे (n_read < n_to_read)
            {
              सचे (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  भङ्ग;
                }

              /* reached EOF */
              सचे (n_read <= in_elide)
                {
                  सचे (first)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  अञ्ञथा
                    {
                      delta = in_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          सचे (! first)
            {
              desired_pos += in_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, in_elide - delta);
            }
          first = false;

          सचे (in_elide < n_read)
            {
              desired_pos += n_read - in_elide;
              xwrite_stdout (b[i], n_read - in_elide);
            }
        }

      मुक्त (b[0]);
    }
  अञ्ञथा
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      वर्ण **b = रिक्त;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_next;
      अत्थाय (i = 0, i_next = 1; !eof; i = i_next, i_next = (i_next + 1) % n_bufs)
        {
          सचे (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         परिमाण *b);

          सचे (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = full_read (fd, b[i], READ_BUFSIZE);
          सचे (n_read < READ_BUFSIZE)
            {
              सचे (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  गच्छ free_mem;
                }
              eof = true;
            }

          सचे (i + 1 == n_bufs)
            buffered_enough = true;

          सचे (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_next], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      सचे (buffered_enough)
        {
          idx_t n_bytes_वामः_in_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          सचे (rem < n_bytes_वामः_in_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          अञ्ञथा
            {
              xwrite_stdout (b[i] + n_read, n_bytes_वामः_in_b_i);
              xwrite_stdout (b[i_next], rem - n_bytes_वामः_in_b_i);
            }
        }
      अञ्ञथा सचे (i + 1 == n_bufs)
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
      अत्थाय (i = 0; i < n_alloc; i++)
        मुक्त (b[i]);
      मुक्त (b);
    }

  सचे (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  पटिनिवत्त ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

स्थिर bool
elide_tail_bytes_file (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t n_elide,
                       संरचना stat ध्रुव *st, off_t current_pos)
{
  off_t size = st->st_size;
  सचे (current_pos < 0 || size <= STP_BLKSIZE (st))
    पटिनिवत्त elide_tail_bytes_pipe (filename, fd, n_elide, current_pos);
  अञ्ञथा
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = size - current_pos;
      off_t bytes_remaining = diff < 0 ? 0 : diff;

      सचे (bytes_remaining <= n_elide)
        पटिनिवत्त true;

      गणना Copy_fd_status err = copy_fd (fd, bytes_remaining - n_elide);
      सचे (err == COPY_FD_OK)
        पटिनिवत्त true;

      diagnose_copy_fd_failure (err, filename);
      पटिनिवत्त false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

स्थिर bool
elide_tail_पङ्क्तयः_pipe (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t n_elide,
                       off_t current_pos)
{
  संरचना linebuffer
  {
    वर्ण buffer[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    संरचना linebuffer *next;
  };
  off_t desired_pos = current_pos;
  प्रकारनिर्देश संरचना linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  size_t total_पङ्क्तयः = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (परिमाण (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = रिक्त;
  tmp = xmalloc (परिमाण (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  याव (true)
    {
      n_read = पठ (fd, tmp->buffer, BUFSIZ);
      सचे (n_read <= 0)
        भङ्ग;

      सचे (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->buffer, n_read);
          अनुवर्त;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = रिक्त;

      /* Count the number of newlines just read.  */
      {
        वर्ण *buffer_end = tmp->buffer + n_read;
        *buffer_end = पङ्क्तिः_end;
        वर्ण ध्रुव *p = tmp->buffer;
        याव ((p = rawmemchr (p, पङ्क्तिः_end)) < buffer_end)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_पङ्क्तयः += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      सचे (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      अञ्ञथा
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          सचे (n_elide < total_पङ्क्तयः - first->nlines)
            {
              desired_pos += first->nbytes;
              xwrite_stdout (first->buffer, first->nbytes);
              tmp = first;
              total_पङ्क्तयः -= first->nlines;
              first = first->next;
            }
          अञ्ञथा
            tmp = xmalloc (परिमाण (LBUFFER));
        }
    }

  मुक्त (tmp);

  सचे (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      गच्छ free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  सचे (last->nbytes && last->buffer[last->nbytes - 1] != पङ्क्तिः_end)
    {
      ++last->nlines;
      ++total_पङ्क्तयः;
    }

  अत्थाय (tmp = first; n_elide < total_पङ्क्तयः - tmp->nlines; tmp = tmp->next)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->buffer, tmp->nbytes);
      total_पङ्क्तयः -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  सचे (n_elide < total_पङ्क्तयः)
    {
      size_t n = total_पङ्क्तयः - n_elide;
      वर्ण ध्रुव *buffer_end = tmp->buffer + tmp->nbytes;
      वर्ण ध्रुव *p = tmp->buffer;
      याव (n && (p = memchr (p, पङ्क्तिः_end, buffer_end - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->buffer;
      xwrite_stdout (tmp->buffer, p - tmp->buffer);
    }

free_lbuffers:
  याव (first)
    {
      tmp = first->next;
      मुक्त (first);
      first = tmp;
    }

  सचे (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  पटिनिवत्त ok;
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
स्थिर bool
elide_tail_पङ्क्तयः_seekable (वर्ण ध्रुव *pretty_filename, पूर्णाङ्क fd,
                           uintmax_t n_पङ्क्तयः,
                           off_t start_pos, off_t size)
{
  वर्ण buffer[BUFSIZ];
  ssize_t bytes_read;
  off_t pos = size;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  bytes_read = (pos - start_pos) % BUFSIZ;
  सचे (bytes_read == 0)
    bytes_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= bytes_read;
  सचे (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    पटिनिवत्त false;
  bytes_read = पठ (fd, buffer, bytes_read);
  सचे (bytes_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      पटिनिवत्त false;
    }

  /* n_lines == 0 case needs special treatment. */
  ध्रुव bool all_पङ्क्तयः = !n_पङ्क्तयः;

  /* Count the incomplete line on files that don't end with a newline.  */
  सचे (n_पङ्क्तयः && bytes_read && buffer[bytes_read - 1] != पङ्क्तिः_end)
    --n_पङ्क्तयः;

  याव (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      याव (n)
        {
          सचे (all_पङ्क्तयः)
            n -= 1;
          अञ्ञथा
            {
              वर्ण ध्रुव *nl;
              nl = memrchr (buffer, पङ्क्तिः_end, n);
              सचे (nl == रिक्त)
                भङ्ग;
              n = nl - buffer;
            }
          सचे (n_पङ्क्तयः-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              सचे (start_pos < pos)
                {
                  गणना Copy_fd_status err;
                  सचे (elseek (fd, start_pos, SEEK_SET, pretty_filename) < 0)
                    पटिनिवत्त false;

                  err = copy_fd (fd, pos - start_pos);
                  सचे (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      पटिनिवत्त false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (buffer, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              पटिनिवत्त 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      सचे (pos == start_pos)
        {
          /* Not enough lines in the file.  */
          पटिनिवत्त true;
        }
      pos -= BUFSIZ;
      सचे (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        पटिनिवत्त false;

      bytes_read = पठ (fd, buffer, BUFSIZ);
      सचे (bytes_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          पटिनिवत्त false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      सचे (bytes_read == 0)
        पटिनिवत्त true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

स्थिर bool
elide_tail_पङ्क्तयः_file (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t n_elide,
                       संरचना stat ध्रुव *st, off_t current_pos)
{
  off_t size = st->st_size;
  सचे (current_pos < 0 || size <= STP_BLKSIZE (st))
    पटिनिवत्त elide_tail_पङ्क्तयः_pipe (filename, fd, n_elide, current_pos);
  अञ्ञथा
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      पटिनिवत्त (size <= current_pos
              || elide_tail_पङ्क्तयः_seekable (filename, fd, n_elide,
                                            current_pos, size));
    }
}

स्थिर bool
head_bytes (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t bytes_to_write)
{
  सचे (copy_fd (fd, bytes_to_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      पटिनिवत्त false;
    }
  पटिनिवत्त true;
}

स्थिर bool
head_पङ्क्तयः (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t पङ्क्तयः_to_write)
{
  वर्ण buffer[BUFSIZ];

  याव (पङ्क्तयः_to_write)
    {
      ssize_t bytes_read = पठ (fd, buffer, BUFSIZ);
      idx_t bytes_to_write = 0;

      सचे (bytes_read < 0)
        {
          diagnose_read_failure (filename);
          पटिनिवत्त false;
        }
      सचे (bytes_read == 0)
        भङ्ग;
      याव (bytes_to_write < bytes_read)
        सचे (buffer[bytes_to_write++] == पङ्क्तिः_end && --पङ्क्तयः_to_write == 0)
          {
            off_t n_bytes_past_EOL = bytes_read - bytes_to_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            सचे (lseek (fd, -n_bytes_past_EOL, SEEK_CUR) < 0)
              {
                संरचना stat st;
                सचे (fstat (fd, &st) != 0 || S_ISREG (st.st_mode))
                  elseek_diagnostic (-n_bytes_past_EOL, SEEK_CUR, filename);
              }
            भङ्ग;
          }
      xwrite_stdout (buffer, bytes_to_write);
    }
  पटिनिवत्त true;
}

स्थिर bool
head (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t n_units, bool गणनम्_पङ्क्तयः,
      bool elide_from_end)
{
  सचे (print_headers)
    write_header (filename);

  सचे (elide_from_end)
    {
      /* Optimize for "infinite" elisions.  */
      सचे (n_units == UINTMAX_MAX)
        पटिनिवत्त true;

      off_t current_pos = -1;
      संरचना stat st;
      सचे (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          पटिनिवत्त false;
        }
      सचे (! presume_निवेशः_pipe && S_ISREG (st.st_mode))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          सचे (current_pos < 0)
            पटिनिवत्त false;
        }
      सचे (गणनम्_पङ्क्तयः)
        पटिनिवत्त elide_tail_पङ्क्तयः_file (filename, fd, n_units, &st, current_pos);
      अञ्ञथा
        पटिनिवत्त elide_tail_bytes_file (filename, fd, n_units, &st, current_pos);
    }
  सचे (गणनम्_पङ्क्तयः)
    पटिनिवत्त head_पङ्क्तयः (filename, fd, n_units);
  अञ्ञथा
    पटिनिवत्त head_bytes (filename, fd, n_units);
}

स्थिर bool
head_file (वर्ण ध्रुव *filename, uintmax_t n_units, bool गणनम्_पङ्क्तयः,
           bool elide_from_end)
{
  पूर्णाङ्क fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  सचे (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
    }
  अञ्ञथा
    {
      fd = open (filename, O_RDONLY | O_द्विमान);
      सचे (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          पटिनिवत्त false;
        }
    }

  ok = head (filename, fd, n_units, गणनम्_पङ्क्तयः, elide_from_end);
  सचे (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      पटिनिवत्त false;
    }
  पटिनिवत्त ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

स्थिर uintmax_t
string_to_integer (bool गणनम्_पङ्क्तयः, वर्ण ध्रुव *n_string)
{
  पटिनिवत्त xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (गणनम्_पङ्क्तयः
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  गणना header_mode header_mode = multiple_files;
  bool ok = true;
  पूर्णाङ्क c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_सङ्ख्या;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool गणनम्_पङ्क्तयः = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_end = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  स्थिर वर्ण ध्रुव *ध्रुव default_file_list[] = {"-", रिक्त};
  वर्ण ध्रुव *ध्रुव *file_list;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  सचे (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      वर्ण *a = argv[1];
      वर्ण *n_string = ++a;
      वर्ण *end_n_string;
      वर्ण multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      करोहि ++a;
      याव (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      end_n_string = a;

      /* Parse any appended option letters. */
      अत्थाय (; *a; a++)
        {
          विकल्प (*a)
            {
            पक्ष 'c':
              गणनम्_पङ्क्तयः = false;
              multiplier_char = 0;
              भङ्ग;

            पक्ष 'b':
            पक्ष 'k':
            पक्ष 'm':
              गणनम्_पङ्क्तयः = false;
              multiplier_char = *a;
              भङ्ग;

            पक्ष 'l':
              गणनम्_पङ्क्तयः = true;
              भङ्ग;

            पक्ष 'q':
              header_mode = never;
              भङ्ग;

            पक्ष 'v':
              header_mode = always;
              भङ्ग;

            पक्ष 'z':
              पङ्क्तिः_end = '\0';
              भङ्ग;

            स्वभाव:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *end_n_string = multiplier_char;
      सचे (multiplier_char)
        *(++end_n_string) = 0;

      n_units = string_to_integer (गणनम्_पङ्क्तयः, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  याव ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, रिक्त))
         != -1)
    {
      विकल्प (c)
        {
        पक्ष PRESUME_निवेशः_PIPE_OPTION:
          presume_निवेशः_pipe = true;
          भङ्ग;

        पक्ष 'c':
          गणनम्_पङ्क्तयः = false;
          elide_from_end = (*optarg == '-');
          सचे (elide_from_end)
            ++optarg;
          n_units = string_to_integer (गणनम्_पङ्क्तयः, optarg);
          भङ्ग;

        पक्ष 'n':
          गणनम्_पङ्क्तयः = true;
          elide_from_end = (*optarg == '-');
          सचे (elide_from_end)
            ++optarg;
          n_units = string_to_integer (गणनम्_पङ्क्तयः, optarg);
          भङ्ग;

        पक्ष 'q':
          header_mode = never;
          भङ्ग;

        पक्ष 'v':
          header_mode = always;
          भङ्ग;

        पक्ष 'z':
          पङ्क्तिः_end = '\0';
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          सचे (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  सचे (header_mode == always
      || (header_mode == multiple_files && optind < argc - 1))
    print_headers = true;

  file_list = (optind < argc
               ? (वर्ण ध्रुव *ध्रुव *) &argv[optind]
               : default_file_list);

  xset_द्विमान_mode (STDOUT_FILENO, O_द्विमान);

  अत्थाय (size_t i = 0; file_list[i]; ++i)
    ok &= head_file (file_list[i], n_units, गणनम्_पङ्क्तयः, elide_from_end);

  सचे (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  पटिनिवत्त ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
