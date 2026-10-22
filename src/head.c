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
#define DEFAULT_संख्या 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
स्थिर bool presume_आदान_pipe;

/* If true, print filename headers. */
स्थिर bool print_headers;

/* Character to split lines by. */
स्थिर अक्षर ओळ_end = '\n';

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
  PRESUME_आदान_PIPE_OPTION = अक्षर_उच्च + 1
};

स्थिर संरचना option स्थिरांक long_options[] =
{
  {"bytes", required_argument, रिक्त, 'c'},
  {"lines", required_argument, रिक्त, 'n'},
  {"-presume-input-pipe", no_argument, रिक्त,
   PRESUME_आदान_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, रिक्त, 'q'},
  {"silent", no_argument, रिक्त, 'q'},
  {"verbose", no_argument, रिक्त, 'v'},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      म_लिखो (_("\
Print the first %d ओळी of each खाता to standard output.\n\
With more than one खाता, precede each with a header giving the file name.\n\
"), DEFAULT_संख्या);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --bytes=[-]अंक_2\n\
         print the first अंक_2 bytes of each file;\n\
         with the leading '-', print all but the last अंक_2 bytes of each file\n\
"));
      oprintf (_("\
  -n, --ओळी=[-]अंक_2\n\
         print the first अंक_2 ओळी instead of the first %d;\n\
         with the leading '-', print all but the last अंक_2 ओळी of each file\n\
"), DEFAULT_संख्या);
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
         ओळ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
अंक_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on साठी T, P, E, Z, Y, R, Q.\n\
द्विमान_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

स्थिर शून्य
diagnose_read_failure (अक्षर स्थिरांक *filename)
{
  error (0, त्रुटि_सं, _("error reading %s"), quoteaf (filename));
}

स्थिर शून्य
diagnose_copy_fd_failure (गणना Copy_fd_status err, अक्षर स्थिरांक *filename)
{
  निवड (err)
    {
    स्थिती COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      थांबा;
    स्थिती COPY_FD_UNEXPECTED_EOF:
      error (0, त्रुटि_सं, _("%s: file has shrunk too much"), quotef (filename));
      थांबा;
    स्थिती COPY_FD_OK: पूर्वनिर्धारित:
      affirm (false);
    }
}

स्थिर शून्य
write_header (अक्षर स्थिरांक *filename)
{
  स्थिर bool first_file = true;

  म_लिखो ("%s==> %s <==\n", (first_file ? "" : "\n"), quotef (filename));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

स्थिर शून्य
xwrite_stdout (अक्षर स्थिरांक *buffer, माप_प्रकार n_bytes)
{
  जर (n_bytes > 0 && ख_डालो (buffer, 1, n_bytes, मानक_निकास) < n_bytes)
    {
      साफ_त्रुटि (मानक_निकास); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (मानक_निकास);
      error (निकास_त्रुटि, त्रुटि_सं, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

स्थिर गणना Copy_fd_status
copy_fd (पूर्णांक src_fd, uintmax_t n_bytes)
{
  अक्षर buf[बफ_मान];

  /* Copy the file contents.  */
  पर्यंत (0 < n_bytes)
    {
      idx_t n_to_read = MIN (n_bytes, आकार buf);
      ssize_t n_read = पठ (src_fd, buf, n_to_read);
      जर (n_read < 0)
        परत COPY_FD_READ_ERROR;

      n_bytes -= n_read;

      जर (n_read == 0 && n_bytes != 0)
        परत COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  परत COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
स्थिर शून्य
elseek_diagnostic (off_t offset, पूर्णांक whence, अक्षर स्थिरांक *filename)
{
  intmax_t off = offset;
  error (0, त्रुटि_सं,
         _(whence == शुरू_से
           ? N_("%s: cannot seek to offset %jd")
           : N_("%s: cannot seek to relative offset %jd")),
         quotef (filename), off);
}

/* Call lseek (FD, OFFSET, WHENCE), where file descriptor FD
   corresponds to the file FILENAME.  WHENCE must be SEEK_SET or
   SEEK_CUR.  Return the resulting offset.  Give a diagnostic and
   return -1 if lseek fails.  */

स्थिर off_t
elseek (पूर्णांक fd, off_t offset, पूर्णांक whence, अक्षर स्थिरांक *filename)
{
  off_t new_offset = lseek (fd, offset, whence);

  जर (new_offset < 0)
    elseek_diagnostic (offset, whence, filename);

  परत new_offset;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
स्थिर bool
elide_tail_bytes_pipe (अक्षर स्थिरांक *filename, पूर्णांक fd, uintmax_t n_elide,
                       off_t current_pos)
{
  uintmax_t desired_pos = current_pos;
  bool ok = true;

#ifndef HEAD_TAIL_PIPE_READ_BUFSIZE
# define HEAD_TAIL_PIPE_READ_BUFSIZE बफ_मान
#endif
#define READ_BUFSIZE HEAD_TAIL_PIPE_READ_BUFSIZE

  /* If we're eliding no more than this many bytes, then it's ok to allocate
     more memory in order to use a more time-efficient algorithm.
     FIXME: use a fraction of available memory instead, as in sort.
     FIXME: is this even worthwhile?  */
#ifndef HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD
# define HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD 1024 * 1024
#endif

#जर HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD < 2 * READ_BUFSIZE
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
  जर (n_elide <= HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD)
    {
      idx_t in_elide = n_elide;
      bool first = true;
      bool eof = false;
      idx_t n_to_read = READ_BUFSIZE + n_elide;
      अक्षर *b[2];
      b[0] = xnmalloc (2, n_to_read);
      b[1] = b[0] + n_to_read;

      साठी (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = full_read (fd, b[i], n_to_read);
          idx_t delta = 0;
          जर (n_read < n_to_read)
            {
              जर (त्रुटि_सं != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  थांबा;
                }

              /* reached EOF */
              जर (n_read <= in_elide)
                {
                  जर (first)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  अन्यथा
                    {
                      delta = in_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          जर (! first)
            {
              desired_pos += in_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, in_elide - delta);
            }
          first = false;

          जर (in_elide < n_read)
            {
              desired_pos += n_read - in_elide;
              xwrite_stdout (b[i], n_read - in_elide);
            }
        }

      मुक्त (b[0]);
    }
  अन्यथा
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      अक्षर **b = रिक्त;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_next;
      साठी (i = 0, i_next = 1; !eof; i = i_next, i_next = (i_next + 1) % n_bufs)
        {
          जर (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         आकार *b);

          जर (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = full_read (fd, b[i], READ_BUFSIZE);
          जर (n_read < READ_BUFSIZE)
            {
              जर (त्रुटि_सं != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  जा free_mem;
                }
              eof = true;
            }

          जर (i + 1 == n_bufs)
            buffered_enough = true;

          जर (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_next], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      जर (buffered_enough)
        {
          idx_t n_bytes_डावा_in_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          जर (rem < n_bytes_डावा_in_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          अन्यथा
            {
              xwrite_stdout (b[i] + n_read, n_bytes_डावा_in_b_i);
              xwrite_stdout (b[i_next], rem - n_bytes_डावा_in_b_i);
            }
        }
      अन्यथा जर (i + 1 == n_bufs)
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
      साठी (i = 0; i < n_alloc; i++)
        मुक्त (b[i]);
      मुक्त (b);
    }

  जर (0 <= current_pos && elseek (fd, desired_pos, शुरू_से, filename) < 0)
    ok = false;
  परत ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

स्थिर bool
elide_tail_bytes_file (अक्षर स्थिरांक *filename, पूर्णांक fd, uintmax_t n_elide,
                       संरचना stat स्थिरांक *st, off_t current_pos)
{
  off_t size = st->st_size;
  जर (current_pos < 0 || size <= STP_BLKSIZE (st))
    परत elide_tail_bytes_pipe (filename, fd, n_elide, current_pos);
  अन्यथा
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = size - current_pos;
      off_t bytes_remaining = diff < 0 ? 0 : diff;

      जर (bytes_remaining <= n_elide)
        परत true;

      गणना Copy_fd_status err = copy_fd (fd, bytes_remaining - n_elide);
      जर (err == COPY_FD_OK)
        परत true;

      diagnose_copy_fd_failure (err, filename);
      परत false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

स्थिर bool
elide_tail_ओळी_pipe (अक्षर स्थिरांक *filename, पूर्णांक fd, uintmax_t n_elide,
                       off_t current_pos)
{
  संरचना linebuffer
  {
    अक्षर buffer[बफ_मान + 1];
    माप_प्रकार nbytes;
    माप_प्रकार nlines;
    संरचना linebuffer *next;
  };
  off_t desired_pos = current_pos;
  प्रकारव्याख्या संरचना linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  माप_प्रकार total_ओळी = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (आकार (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = रिक्त;
  tmp = xmalloc (आकार (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  पर्यंत (true)
    {
      n_read = पठ (fd, tmp->buffer, बफ_मान);
      जर (n_read <= 0)
        थांबा;

      जर (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->buffer, n_read);
          सुरूठेवा;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = रिक्त;

      /* Count the number of newlines just read.  */
      {
        अक्षर *buffer_end = tmp->buffer + n_read;
        *buffer_end = ओळ_end;
        अक्षर स्थिरांक *p = tmp->buffer;
        पर्यंत ((p = rawmemchr (p, ओळ_end)) < buffer_end)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_ओळी += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      जर (tmp->nbytes + last->nbytes < बफ_मान)
        {
          स_नकल (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      अन्यथा
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          जर (n_elide < total_ओळी - first->nlines)
            {
              desired_pos += first->nbytes;
              xwrite_stdout (first->buffer, first->nbytes);
              tmp = first;
              total_ओळी -= first->nlines;
              first = first->next;
            }
          अन्यथा
            tmp = xmalloc (आकार (LBUFFER));
        }
    }

  मुक्त (tmp);

  जर (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      जा free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  जर (last->nbytes && last->buffer[last->nbytes - 1] != ओळ_end)
    {
      ++last->nlines;
      ++total_ओळी;
    }

  साठी (tmp = first; n_elide < total_ओळी - tmp->nlines; tmp = tmp->next)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->buffer, tmp->nbytes);
      total_ओळी -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  जर (n_elide < total_ओळी)
    {
      माप_प्रकार n = total_ओळी - n_elide;
      अक्षर स्थिरांक *buffer_end = tmp->buffer + tmp->nbytes;
      अक्षर स्थिरांक *p = tmp->buffer;
      पर्यंत (n && (p = स_प्रथम (p, ओळ_end, buffer_end - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->buffer;
      xwrite_stdout (tmp->buffer, p - tmp->buffer);
    }

free_lbuffers:
  पर्यंत (first)
    {
      tmp = first->next;
      मुक्त (first);
      first = tmp;
    }

  जर (0 <= current_pos && elseek (fd, desired_pos, शुरू_से, filename) < 0)
    ok = false;
  परत ok;
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
elide_tail_ओळी_seekable (अक्षर स्थिरांक *pretty_filename, पूर्णांक fd,
                           uintmax_t n_ओळी,
                           off_t start_pos, off_t size)
{
  अक्षर buffer[बफ_मान];
  ssize_t bytes_read;
  off_t pos = size;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  bytes_read = (pos - start_pos) % बफ_मान;
  जर (bytes_read == 0)
    bytes_read = बफ_मान;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= bytes_read;
  जर (elseek (fd, pos, शुरू_से, pretty_filename) < 0)
    परत false;
  bytes_read = पठ (fd, buffer, bytes_read);
  जर (bytes_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      परत false;
    }

  /* n_lines == 0 case needs special treatment. */
  स्थिरांक bool all_ओळी = !n_ओळी;

  /* Count the incomplete line on files that don't end with a newline.  */
  जर (n_ओळी && bytes_read && buffer[bytes_read - 1] != ओळ_end)
    --n_ओळी;

  पर्यंत (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      पर्यंत (n)
        {
          जर (all_ओळी)
            n -= 1;
          अन्यथा
            {
              अक्षर स्थिरांक *nl;
              nl = memrchr (buffer, ओळ_end, n);
              जर (nl == रिक्त)
                थांबा;
              n = nl - buffer;
            }
          जर (n_ओळी-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              जर (start_pos < pos)
                {
                  गणना Copy_fd_status err;
                  जर (elseek (fd, start_pos, शुरू_से, pretty_filename) < 0)
                    परत false;

                  err = copy_fd (fd, pos - start_pos);
                  जर (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      परत false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (buffer, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              परत 0 <= elseek (fd, pos + n + 1, शुरू_से, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      जर (pos == start_pos)
        {
          /* Not enough lines in the file.  */
          परत true;
        }
      pos -= बफ_मान;
      जर (elseek (fd, pos, शुरू_से, pretty_filename) < 0)
        परत false;

      bytes_read = पठ (fd, buffer, बफ_मान);
      जर (bytes_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          परत false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      जर (bytes_read == 0)
        परत true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

स्थिर bool
elide_tail_ओळी_file (अक्षर स्थिरांक *filename, पूर्णांक fd, uintmax_t n_elide,
                       संरचना stat स्थिरांक *st, off_t current_pos)
{
  off_t size = st->st_size;
  जर (current_pos < 0 || size <= STP_BLKSIZE (st))
    परत elide_tail_ओळी_pipe (filename, fd, n_elide, current_pos);
  अन्यथा
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      परत (size <= current_pos
              || elide_tail_ओळी_seekable (filename, fd, n_elide,
                                            current_pos, size));
    }
}

स्थिर bool
head_bytes (अक्षर स्थिरांक *filename, पूर्णांक fd, uintmax_t bytes_to_write)
{
  जर (copy_fd (fd, bytes_to_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      परत false;
    }
  परत true;
}

स्थिर bool
head_ओळी (अक्षर स्थिरांक *filename, पूर्णांक fd, uintmax_t ओळी_to_write)
{
  अक्षर buffer[बफ_मान];

  पर्यंत (ओळी_to_write)
    {
      ssize_t bytes_read = पठ (fd, buffer, बफ_मान);
      idx_t bytes_to_write = 0;

      जर (bytes_read < 0)
        {
          diagnose_read_failure (filename);
          परत false;
        }
      जर (bytes_read == 0)
        थांबा;
      पर्यंत (bytes_to_write < bytes_read)
        जर (buffer[bytes_to_write++] == ओळ_end && --ओळी_to_write == 0)
          {
            off_t n_bytes_past_EOL = bytes_read - bytes_to_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            जर (lseek (fd, -n_bytes_past_EOL, प्रस्तुत_से) < 0)
              {
                संरचना stat st;
                जर (ख_स्थिति (fd, &st) != 0 || S_ISREG (st.st_mode))
                  elseek_diagnostic (-n_bytes_past_EOL, प्रस्तुत_से, filename);
              }
            थांबा;
          }
      xwrite_stdout (buffer, bytes_to_write);
    }
  परत true;
}

स्थिर bool
head (अक्षर स्थिरांक *filename, पूर्णांक fd, uintmax_t n_units, bool मोजणी_ओळी,
      bool elide_from_end)
{
  जर (print_headers)
    write_header (filename);

  जर (elide_from_end)
    {
      /* Optimize for "infinite" elisions.  */
      जर (n_units == UINTMAX_MAX)
        परत true;

      off_t current_pos = -1;
      संरचना stat st;
      जर (ख_स्थिति (fd, &st) != 0)
        {
          error (0, त्रुटि_सं, _("cannot fstat %s"),
                 quoteaf (filename));
          परत false;
        }
      जर (! presume_आदान_pipe && S_ISREG (st.st_mode))
        {
          current_pos = elseek (fd, 0, प्रस्तुत_से, filename);
          जर (current_pos < 0)
            परत false;
        }
      जर (मोजणी_ओळी)
        परत elide_tail_ओळी_file (filename, fd, n_units, &st, current_pos);
      अन्यथा
        परत elide_tail_bytes_file (filename, fd, n_units, &st, current_pos);
    }
  जर (मोजणी_ओळी)
    परत head_ओळी (filename, fd, n_units);
  अन्यथा
    परत head_bytes (filename, fd, n_units);
}

स्थिर bool
head_file (अक्षर स्थिरांक *filename, uintmax_t n_units, bool मोजणी_ओळी,
           bool elide_from_end)
{
  पूर्णांक fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  जर (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
    }
  अन्यथा
    {
      fd = खोलो (filename, O_RDONLY | O_द्विमान);
      जर (fd < 0)
        {
          error (0, त्रुटि_सं, _("cannot open %s for reading"), quoteaf (filename));
          परत false;
        }
    }

  ok = head (filename, fd, n_units, मोजणी_ओळी, elide_from_end);
  जर (!is_stdin && बंद (fd) != 0)
    {
      error (0, त्रुटि_सं, _("failed to close %s"), quoteaf (filename));
      परत false;
    }
  परत ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

स्थिर uintmax_t
string_to_integer (bool मोजणी_ओळी, अक्षर स्थिरांक *n_string)
{
  परत xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (मोजणी_ओळी
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  गणना header_mode header_mode = multiple_files;
  bool ok = true;
  पूर्णांक c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_संख्या;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool मोजणी_ओळी = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_end = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  स्थिर अक्षर स्थिरांक *स्थिरांक default_file_list[] = {"-", रिक्त};
  अक्षर स्थिरांक *स्थिरांक *file_list;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जर (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      अक्षर *a = argv[1];
      अक्षर *n_string = ++a;
      अक्षर *end_n_string;
      अक्षर multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      करा ++a;
      पर्यंत (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      end_n_string = a;

      /* Parse any appended option letters. */
      साठी (; *a; a++)
        {
          निवड (*a)
            {
            स्थिती 'c':
              मोजणी_ओळी = false;
              multiplier_char = 0;
              थांबा;

            स्थिती 'b':
            स्थिती 'k':
            स्थिती 'm':
              मोजणी_ओळी = false;
              multiplier_char = *a;
              थांबा;

            स्थिती 'l':
              मोजणी_ओळी = true;
              थांबा;

            स्थिती 'q':
              header_mode = never;
              थांबा;

            स्थिती 'v':
              header_mode = always;
              थांबा;

            स्थिती 'z':
              ओळ_end = '\0';
              थांबा;

            पूर्वनिर्धारित:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (निकास_त्रुटि);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *end_n_string = multiplier_char;
      जर (multiplier_char)
        *(++end_n_string) = 0;

      n_units = string_to_integer (मोजणी_ओळी, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  पर्यंत ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, रिक्त))
         != -1)
    {
      निवड (c)
        {
        स्थिती PRESUME_आदान_PIPE_OPTION:
          presume_आदान_pipe = true;
          थांबा;

        स्थिती 'c':
          मोजणी_ओळी = false;
          elide_from_end = (*optarg == '-');
          जर (elide_from_end)
            ++optarg;
          n_units = string_to_integer (मोजणी_ओळी, optarg);
          थांबा;

        स्थिती 'n':
          मोजणी_ओळी = true;
          elide_from_end = (*optarg == '-');
          जर (elide_from_end)
            ++optarg;
          n_units = string_to_integer (मोजणी_ओळी, optarg);
          थांबा;

        स्थिती 'q':
          header_mode = never;
          थांबा;

        स्थिती 'v':
          header_mode = always;
          थांबा;

        स्थिती 'z':
          ओळ_end = '\0';
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          जर (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (निकास_त्रुटि);
        }
    }

  जर (header_mode == always
      || (header_mode == multiple_files && optind < argc - 1))
    print_headers = true;

  file_list = (optind < argc
               ? (अक्षर स्थिरांक *स्थिरांक *) &argv[optind]
               : default_file_list);

  xset_द्विमान_mode (STDOUT_FILENO, O_द्विमान);

  साठी (माप_प्रकार i = 0; file_list[i]; ++i)
    ok &= head_file (file_list[i], n_units, मोजणी_ओळी, elide_from_end);

  जर (have_read_stdin && बंद (STDIN_FILENO) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, "-");

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
