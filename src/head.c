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

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"

#समावेश "assure.h"
#समावेश "c-ctype.h"
#समावेश "full-read.h"
#समावेश "stat-size.h"
#समावेश "xbinary-io.h"
#समावेश "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "head"

#घोषणा AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#घोषणा DEFAULT_संख्या 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
अटल bool presume_निवेश_pipe;

/* If true, print filename headers. */
अटल bool print_headers;

/* Character to split lines by. */
अटल अक्षर पंक्ति_end = '\n';

/* When to print the filename banners. */
क्रमागत header_mode
{
  multiple_files, always, never
};

/* Have we ever read standard input?  */
अटल bool have_read_stdin;

क्रमागत Copy_fd_status
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  PRESUME_निवेश_PIPE_OPTION = अक्षर_उच्च + 1
};

अटल काष्ठा option स्थिर long_options[] =
{
  {"bytes", required_argument, शून्य, 'c'},
  {"lines", required_argument, शून्य, 'n'},
  {"-presume-input-pipe", no_argument, शून्य,
   PRESUME_निवेश_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, शून्य, 'q'},
  {"silent", no_argument, शून्य, 'q'},
  {"verbose", no_argument, शून्य, 'v'},
  {"zero-terminated", no_argument, शून्य, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      म_लिखो (_("\
Print the first %d पंक्तियाँ of each खाता to standard output.\n\
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
  -n, --पंक्तियाँ=[-]अंक_2\n\
         print the first अंक_2 पंक्तियाँ instead of the first %d;\n\
         with the leading '-', print all but the last अंक_2 पंक्तियाँ of each file\n\
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
         पंक्ति delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
अंक_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on क्रम T, P, E, Z, Y, R, Q.\n\
द्विआधारी_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

अटल व्योम
diagnose_read_failure (अक्षर स्थिर *filename)
{
  error (0, त्रुटि_सं, _("error reading %s"), quoteaf (filename));
}

अटल व्योम
diagnose_copy_fd_failure (क्रमागत Copy_fd_status err, अक्षर स्थिर *filename)
{
  चयन (err)
    {
    हाल COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      अवरोध;
    हाल COPY_FD_UNEXPECTED_EOF:
      error (0, त्रुटि_सं, _("%s: file has shrunk too much"), quotef (filename));
      अवरोध;
    हाल COPY_FD_OK: शेष:
      affirm (false);
    }
}

अटल व्योम
write_header (अक्षर स्थिर *filename)
{
  अटल bool first_file = true;

  म_लिखो ("%s==> %s <==\n", (first_file ? "" : "\n"), quotef (filename));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

अटल व्योम
xwrite_stdout (अक्षर स्थिर *बफर, माप_प्रकार n_bytes)
{
  अगर (n_bytes > 0 && ख_डालो (बफर, 1, n_bytes, मानक_निकास) < n_bytes)
    {
      साफ_त्रुटि (मानक_निकास); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (मानक_निकास);
      error (निकास_त्रुटि, त्रुटि_सं, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

अटल क्रमागत Copy_fd_status
copy_fd (पूर्णांक src_fd, uintmax_t n_bytes)
{
  अक्षर buf[बफ_मान];

  /* Copy the file contents.  */
  जबतक (0 < n_bytes)
    {
      idx_t n_to_read = MIN (n_bytes, माप buf);
      ssize_t n_read = पढ़ो (src_fd, buf, n_to_read);
      अगर (n_read < 0)
        वापस COPY_FD_READ_ERROR;

      n_bytes -= n_read;

      अगर (n_read == 0 && n_bytes != 0)
        वापस COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  वापस COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
अटल व्योम
elseek_diagnostic (off_t offset, पूर्णांक whence, अक्षर स्थिर *filename)
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

अटल off_t
elseek (पूर्णांक fd, off_t offset, पूर्णांक whence, अक्षर स्थिर *filename)
{
  off_t new_offset = lseek (fd, offset, whence);

  अगर (new_offset < 0)
    elseek_diagnostic (offset, whence, filename);

  वापस new_offset;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
अटल bool
elide_पुच्छ_bytes_pipe (अक्षर स्थिर *filename, पूर्णांक fd, uintmax_t n_elide,
                       off_t current_pos)
{
  uintmax_t desired_pos = current_pos;
  bool ok = true;

#अगर_अघोषित शीर्ष_पुच्छ_PIPE_READ_BUFSIZE
# define शीर्ष_पुच्छ_PIPE_READ_BUFSIZE बफ_मान
#पूर्ण_अगर
#घोषणा READ_BUFSIZE शीर्ष_पुच्छ_PIPE_READ_BUFSIZE

  /* If we're eliding no more than this many bytes, then it's ok to allocate
     more memory in order to use a more time-efficient algorithm.
     FIXME: use a fraction of available memory instead, as in sort.
     FIXME: is this even worthwhile?  */
#अगर_अघोषित शीर्ष_पुच्छ_PIPE_BYTECOUNT_THRESHOLD
# define शीर्ष_पुच्छ_PIPE_BYTECOUNT_THRESHOLD 1024 * 1024
#पूर्ण_अगर

#अगर शीर्ष_पुच्छ_PIPE_BYTECOUNT_THRESHOLD < 2 * READ_BUFSIZE
# error "HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD must be at least 2 * READ_BUFSIZE"
#पूर्ण_अगर

  /* Two cases to consider...
     1) n_elide is small enough that we can afford to double-buffer:
        allocate 2 * (READ_BUFSIZE + n_elide) bytes
     2) n_elide is too big for that, so we allocate only
        (READ_BUFSIZE + n_elide) bytes

     FIXME: profile, to see if double-buffering is worthwhile

     CAUTION: do not fail (out of memory) when asked to elide
     a ridiculous amount, but when given only a small input.  */

  static_assert (READ_BUFSIZE <= IDX_MAX);
  static_assert (शीर्ष_पुच्छ_PIPE_BYTECOUNT_THRESHOLD <= IDX_MAX - READ_BUFSIZE);
  अगर (n_elide <= शीर्ष_पुच्छ_PIPE_BYTECOUNT_THRESHOLD)
    {
      idx_t in_elide = n_elide;
      bool first = true;
      bool eof = false;
      idx_t n_to_read = READ_BUFSIZE + n_elide;
      अक्षर *b[2];
      b[0] = xnmalloc (2, n_to_read);
      b[1] = b[0] + n_to_read;

      क्रम (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = full_read (fd, b[i], n_to_read);
          idx_t delta = 0;
          अगर (n_read < n_to_read)
            {
              अगर (त्रुटि_सं != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  अवरोध;
                }

              /* reached EOF */
              अगर (n_read <= in_elide)
                {
                  अगर (first)
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
          अगर (! first)
            {
              desired_pos += in_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, in_elide - delta);
            }
          first = false;

          अगर (in_elide < n_read)
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
      अक्षर **b = शून्य;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_अग्र;
      क्रम (i = 0, i_अग्र = 1; !eof; i = i_अग्र, i_अग्र = (i_अग्र + 1) % n_bufs)
        {
          अगर (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         माप *b);

          अगर (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = full_read (fd, b[i], READ_BUFSIZE);
          अगर (n_read < READ_BUFSIZE)
            {
              अगर (त्रुटि_सं != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  जाओ free_mem;
                }
              eof = true;
            }

          अगर (i + 1 == n_bufs)
            buffered_enough = true;

          अगर (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_अग्र], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      अगर (buffered_enough)
        {
          idx_t n_bytes_बायाँ_in_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          अगर (rem < n_bytes_बायाँ_in_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          अन्यथा
            {
              xwrite_stdout (b[i] + n_read, n_bytes_बायाँ_in_b_i);
              xwrite_stdout (b[i_अग्र], rem - n_bytes_बायाँ_in_b_i);
            }
        }
      अन्यथा अगर (i + 1 == n_bufs)
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
          xwrite_stdout (b[i_अग्र], x);
        }

    free_mem:
      क्रम (i = 0; i < n_alloc; i++)
        मुक्त (b[i]);
      मुक्त (b);
    }

  अगर (0 <= current_pos && elseek (fd, desired_pos, शुरू_से, filename) < 0)
    ok = false;
  वापस ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

अटल bool
elide_पुच्छ_bytes_file (अक्षर स्थिर *filename, पूर्णांक fd, uintmax_t n_elide,
                       काष्ठा stat स्थिर *st, off_t current_pos)
{
  off_t आकार = st->st_आकार;
  अगर (current_pos < 0 || आकार <= STP_BLKSIZE (st))
    वापस elide_पुच्छ_bytes_pipe (filename, fd, n_elide, current_pos);
  अन्यथा
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = आकार - current_pos;
      off_t bytes_remaining = diff < 0 ? 0 : diff;

      अगर (bytes_remaining <= n_elide)
        वापस true;

      क्रमागत Copy_fd_status err = copy_fd (fd, bytes_remaining - n_elide);
      अगर (err == COPY_FD_OK)
        वापस true;

      diagnose_copy_fd_failure (err, filename);
      वापस false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

अटल bool
elide_पुच्छ_पंक्तियाँ_pipe (अक्षर स्थिर *filename, पूर्णांक fd, uintmax_t n_elide,
                       off_t current_pos)
{
  काष्ठा linebuffer
  {
    अक्षर बफर[बफ_मान + 1];
    माप_प्रकार nbytes;
    माप_प्रकार nlines;
    काष्ठा linebuffer *अग्र;
  };
  off_t desired_pos = current_pos;
  प्रकार काष्ठा linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  माप_प्रकार योग_पंक्तियाँ = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (माप (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->अग्र = शून्य;
  tmp = xmalloc (माप (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  जबतक (true)
    {
      n_read = पढ़ो (fd, tmp->बफर, बफ_मान);
      अगर (n_read <= 0)
        अवरोध;

      अगर (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->बफर, n_read);
          जारी;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->अग्र = शून्य;

      /* Count the number of newlines just read.  */
      {
        अक्षर *बफर_end = tmp->बफर + n_read;
        *बफर_end = पंक्ति_end;
        अक्षर स्थिर *p = tmp->बफर;
        जबतक ((p = rawmemchr (p, पंक्ति_end)) < बफर_end)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      योग_पंक्तियाँ += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      अगर (tmp->nbytes + last->nbytes < बफ_मान)
        {
          स_नकल (&last->बफर[last->nbytes], tmp->बफर, tmp->nbytes);
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
          last = last->अग्र = tmp;
          अगर (n_elide < योग_पंक्तियाँ - first->nlines)
            {
              desired_pos += first->nbytes;
              xwrite_stdout (first->बफर, first->nbytes);
              tmp = first;
              योग_पंक्तियाँ -= first->nlines;
              first = first->अग्र;
            }
          अन्यथा
            tmp = xmalloc (माप (LBUFFER));
        }
    }

  मुक्त (tmp);

  अगर (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      जाओ free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  अगर (last->nbytes && last->बफर[last->nbytes - 1] != पंक्ति_end)
    {
      ++last->nlines;
      ++योग_पंक्तियाँ;
    }

  क्रम (tmp = first; n_elide < योग_पंक्तियाँ - tmp->nlines; tmp = tmp->अग्र)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->बफर, tmp->nbytes);
      योग_पंक्तियाँ -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  अगर (n_elide < योग_पंक्तियाँ)
    {
      माप_प्रकार n = योग_पंक्तियाँ - n_elide;
      अक्षर स्थिर *बफर_end = tmp->बफर + tmp->nbytes;
      अक्षर स्थिर *p = tmp->बफर;
      जबतक (n && (p = स_प्रथम (p, पंक्ति_end, बफर_end - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->बफर;
      xwrite_stdout (tmp->बफर, p - tmp->बफर);
    }

free_lbuffers:
  जबतक (first)
    {
      tmp = first->अग्र;
      मुक्त (first);
      first = tmp;
    }

  अगर (0 <= current_pos && elseek (fd, desired_pos, शुरू_से, filename) < 0)
    ok = false;
  वापस ok;
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
अटल bool
elide_पुच्छ_पंक्तियाँ_seekable (अक्षर स्थिर *pretty_filename, पूर्णांक fd,
                           uintmax_t n_पंक्तियाँ,
                           off_t start_pos, off_t आकार)
{
  अक्षर बफर[बफ_मान];
  ssize_t bytes_read;
  off_t pos = आकार;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  bytes_read = (pos - start_pos) % बफ_मान;
  अगर (bytes_read == 0)
    bytes_read = बफ_मान;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= bytes_read;
  अगर (elseek (fd, pos, शुरू_से, pretty_filename) < 0)
    वापस false;
  bytes_read = पढ़ो (fd, बफर, bytes_read);
  अगर (bytes_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      वापस false;
    }

  /* n_lines == 0 case needs special treatment. */
  स्थिर bool all_पंक्तियाँ = !n_पंक्तियाँ;

  /* Count the incomplete line on files that don't end with a newline.  */
  अगर (n_पंक्तियाँ && bytes_read && बफर[bytes_read - 1] != पंक्ति_end)
    --n_पंक्तियाँ;

  जबतक (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      जबतक (n)
        {
          अगर (all_पंक्तियाँ)
            n -= 1;
          अन्यथा
            {
              अक्षर स्थिर *nl;
              nl = memrchr (बफर, पंक्ति_end, n);
              अगर (nl == शून्य)
                अवरोध;
              n = nl - बफर;
            }
          अगर (n_पंक्तियाँ-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              अगर (start_pos < pos)
                {
                  क्रमागत Copy_fd_status err;
                  अगर (elseek (fd, start_pos, शुरू_से, pretty_filename) < 0)
                    वापस false;

                  err = copy_fd (fd, pos - start_pos);
                  अगर (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      वापस false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (बफर, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              वापस 0 <= elseek (fd, pos + n + 1, शुरू_से, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      अगर (pos == start_pos)
        {
          /* Not enough lines in the file.  */
          वापस true;
        }
      pos -= बफ_मान;
      अगर (elseek (fd, pos, शुरू_से, pretty_filename) < 0)
        वापस false;

      bytes_read = पढ़ो (fd, बफर, बफ_मान);
      अगर (bytes_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          वापस false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      अगर (bytes_read == 0)
        वापस true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

अटल bool
elide_पुच्छ_पंक्तियाँ_file (अक्षर स्थिर *filename, पूर्णांक fd, uintmax_t n_elide,
                       काष्ठा stat स्थिर *st, off_t current_pos)
{
  off_t आकार = st->st_आकार;
  अगर (current_pos < 0 || आकार <= STP_BLKSIZE (st))
    वापस elide_पुच्छ_पंक्तियाँ_pipe (filename, fd, n_elide, current_pos);
  अन्यथा
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      वापस (आकार <= current_pos
              || elide_पुच्छ_पंक्तियाँ_seekable (filename, fd, n_elide,
                                            current_pos, आकार));
    }
}

अटल bool
शीर्ष_bytes (अक्षर स्थिर *filename, पूर्णांक fd, uintmax_t bytes_to_write)
{
  अगर (copy_fd (fd, bytes_to_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      वापस false;
    }
  वापस true;
}

अटल bool
शीर्ष_पंक्तियाँ (अक्षर स्थिर *filename, पूर्णांक fd, uintmax_t पंक्तियाँ_to_write)
{
  अक्षर बफर[बफ_मान];

  जबतक (पंक्तियाँ_to_write)
    {
      ssize_t bytes_read = पढ़ो (fd, बफर, बफ_मान);
      idx_t bytes_to_write = 0;

      अगर (bytes_read < 0)
        {
          diagnose_read_failure (filename);
          वापस false;
        }
      अगर (bytes_read == 0)
        अवरोध;
      जबतक (bytes_to_write < bytes_read)
        अगर (बफर[bytes_to_write++] == पंक्ति_end && --पंक्तियाँ_to_write == 0)
          {
            off_t n_bytes_past_EOL = bytes_read - bytes_to_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            अगर (lseek (fd, -n_bytes_past_EOL, प्रस्तुत_से) < 0)
              {
                काष्ठा stat st;
                अगर (ख_स्थिति (fd, &st) != 0 || S_ISREG (st.st_mode))
                  elseek_diagnostic (-n_bytes_past_EOL, प्रस्तुत_से, filename);
              }
            अवरोध;
          }
      xwrite_stdout (बफर, bytes_to_write);
    }
  वापस true;
}

अटल bool
शीर्ष (अक्षर स्थिर *filename, पूर्णांक fd, uintmax_t n_units, bool गिनती_पंक्तियाँ,
      bool elide_from_end)
{
  अगर (print_headers)
    write_header (filename);

  अगर (elide_from_end)
    {
      /* Optimize for "infinite" elisions.  */
      अगर (n_units == UINTMAX_MAX)
        वापस true;

      off_t current_pos = -1;
      काष्ठा stat st;
      अगर (ख_स्थिति (fd, &st) != 0)
        {
          error (0, त्रुटि_सं, _("cannot fstat %s"),
                 quoteaf (filename));
          वापस false;
        }
      अगर (! presume_निवेश_pipe && S_ISREG (st.st_mode))
        {
          current_pos = elseek (fd, 0, प्रस्तुत_से, filename);
          अगर (current_pos < 0)
            वापस false;
        }
      अगर (गिनती_पंक्तियाँ)
        वापस elide_पुच्छ_पंक्तियाँ_file (filename, fd, n_units, &st, current_pos);
      अन्यथा
        वापस elide_पुच्छ_bytes_file (filename, fd, n_units, &st, current_pos);
    }
  अगर (गिनती_पंक्तियाँ)
    वापस शीर्ष_पंक्तियाँ (filename, fd, n_units);
  अन्यथा
    वापस शीर्ष_bytes (filename, fd, n_units);
}

अटल bool
शीर्ष_file (अक्षर स्थिर *filename, uintmax_t n_units, bool गिनती_पंक्तियाँ,
           bool elide_from_end)
{
  पूर्णांक fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  अगर (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_द्विआधारी_mode (STDIN_FILENO, O_द्विआधारी);
    }
  अन्यथा
    {
      fd = खोलो (filename, O_RDONLY | O_द्विआधारी);
      अगर (fd < 0)
        {
          error (0, त्रुटि_सं, _("cannot open %s for reading"), quoteaf (filename));
          वापस false;
        }
    }

  ok = शीर्ष (filename, fd, n_units, गिनती_पंक्तियाँ, elide_from_end);
  अगर (!is_stdin && बंद (fd) != 0)
    {
      error (0, त्रुटि_सं, _("failed to close %s"), quoteaf (filename));
      वापस false;
    }
  वापस ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

अटल uintmax_t
string_to_integer (bool गिनती_पंक्तियाँ, अक्षर स्थिर *n_string)
{
  वापस xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (गिनती_पंक्तियाँ
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  क्रमागत header_mode header_mode = multiple_files;
  bool ok = true;
  पूर्णांक c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_संख्या;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool गिनती_पंक्तियाँ = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_end = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  अटल अक्षर स्थिर *स्थिर default_file_list[] = {"-", शून्य};
  अक्षर स्थिर *स्थिर *file_list;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  अगर (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      अक्षर *a = argv[1];
      अक्षर *n_string = ++a;
      अक्षर *end_n_string;
      अक्षर multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      करो ++a;
      जबतक (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      end_n_string = a;

      /* Parse any appended option letters. */
      क्रम (; *a; a++)
        {
          चयन (*a)
            {
            हाल 'c':
              गिनती_पंक्तियाँ = false;
              multiplier_char = 0;
              अवरोध;

            हाल 'b':
            हाल 'k':
            हाल 'm':
              गिनती_पंक्तियाँ = false;
              multiplier_char = *a;
              अवरोध;

            हाल 'l':
              गिनती_पंक्तियाँ = true;
              अवरोध;

            हाल 'q':
              header_mode = never;
              अवरोध;

            हाल 'v':
              header_mode = always;
              अवरोध;

            हाल 'z':
              पंक्ति_end = '\0';
              अवरोध;

            शेष:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (निकास_त्रुटि);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *end_n_string = multiplier_char;
      अगर (multiplier_char)
        *(++end_n_string) = 0;

      n_units = string_to_integer (गिनती_पंक्तियाँ, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  जबतक ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, शून्य))
         != -1)
    {
      चयन (c)
        {
        हाल PRESUME_निवेश_PIPE_OPTION:
          presume_निवेश_pipe = true;
          अवरोध;

        हाल 'c':
          गिनती_पंक्तियाँ = false;
          elide_from_end = (*optarg == '-');
          अगर (elide_from_end)
            ++optarg;
          n_units = string_to_integer (गिनती_पंक्तियाँ, optarg);
          अवरोध;

        हाल 'n':
          गिनती_पंक्तियाँ = true;
          elide_from_end = (*optarg == '-');
          अगर (elide_from_end)
            ++optarg;
          n_units = string_to_integer (गिनती_पंक्तियाँ, optarg);
          अवरोध;

        हाल 'q':
          header_mode = never;
          अवरोध;

        हाल 'v':
          header_mode = always;
          अवरोध;

        हाल 'z':
          पंक्ति_end = '\0';
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          अगर (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (निकास_त्रुटि);
        }
    }

  अगर (header_mode == always
      || (header_mode == multiple_files && optind < argc - 1))
    print_headers = true;

  file_list = (optind < argc
               ? (अक्षर स्थिर *स्थिर *) &argv[optind]
               : default_file_list);

  xset_द्विआधारी_mode (STDOUT_FILENO, O_द्विआधारी);

  क्रम (माप_प्रकार i = 0; file_list[i]; ++i)
    ok &= शीर्ष_file (file_list[i], n_units, गिनती_पंक्तियाँ, elide_from_end);

  अगर (have_read_stdin && बंद (STDIN_FILENO) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, "-");

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
