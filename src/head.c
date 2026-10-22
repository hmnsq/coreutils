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
#परिभाषय क्रमादेशः_NAME "head"

#परिभाषय AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#परिभाषय DEFAULT_सङ्ख्या 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
स्थिर bool presume_निवेशः_pipe;

/* If true, print filename headers. */
स्थिर bool print_शीर्षकाणि;

/* Character to split lines by. */
स्थिर वर्ण पङ्क्तिः_अन्तः = '\n';

/* When to print the filename banners. */
गणना शीर्षकम्_बहुलकम्
{
  गुणजः_files, always, never
};

/* Have we ever read standard input?  */
स्थिर bool have_read_stdin;

गणना Copy_fd_स्थितिः
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
  {"bytes", required_तर्कः, रिक्त, 'c'},
  {"lines", required_तर्कः, रिक्त, 'n'},
  {"-presume-input-pipe", no_तर्कः, रिक्त,
   PRESUME_निवेशः_PIPE_OPTION}, /* do not document */
  {"quiet", no_तर्कः, रिक्त, 'q'},
  {"silent", no_तर्कः, रिक्त, 'q'},
  {"verbose", no_तर्कः, रिक्त, 'v'},
  {"zero-terminated", no_तर्कः, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              क्रमादेशः_name);
      प्रारूप_लिख (_("\
Print the first %d पङ्क्तयः सम्बन्धि each FILE गन्तव्यम् standard output.\n\
With अधिकम् than one FILE, precede each with a शीर्षकम् giving the file name.\n\
"), DEFAULT_सङ्ख्या);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --अष्टकानि=[-]अङ्कः_2\n\
         print the first अङ्कः_2 अष्टकानि सम्बन्धि each file;\n\
         with the leading '-', print all but the चरमः अङ्कः_2 अष्टकानि सम्बन्धि each file\n\
"));
      oprintf (_("\
  -n, --पङ्क्तयः=[-]अङ्कः_2\n\
         print the first अङ्कः_2 पङ्क्तयः instead सम्बन्धि the first %d;\n\
         with the leading '-', print all but the चरमः अङ्कः_2 पङ्क्तयः सम्बन्धि each file\n\
"), DEFAULT_सङ्ख्या);
      oputs (_("\
  -q, --quiet, --silent\n\
         never print शीर्षकाणि giving file नामानि\n\
"));
      oputs (_("\
  -v, --verbose\n\
         always print शीर्षकाणि giving file नामानि\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
अङ्कः_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on प्रति T, P, E, Z, Y, R, Q.\n\
द्विमान_2 prefixes can be प्रयुक्तम्, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

स्थिर शून्य
diagnose_read_failure (वर्ण ध्रुव *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

स्थिर शून्य
diagnose_copy_fd_failure (गणना Copy_fd_स्थितिः err, वर्ण ध्रुव *filename)
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
write_शीर्षकम् (वर्ण ध्रुव *filename)
{
  स्थिर bool first_file = true;

  प्रारूप_लिख ("%s==> %s <==\n", (first_file ? "" : "\n"), quotef (filename));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

स्थिर शून्य
xwrite_stdout (वर्ण ध्रुव *बफर, size_t n_अष्टकानि)
{
  यदि (n_अष्टकानि > 0 && fwrite (बफर, 1, n_अष्टकानि, stdout) < n_अष्टकानि)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

स्थिर गणना Copy_fd_स्थितिः
copy_fd (पूर्णाङ्क src_fd, uintmax_t n_अष्टकानि)
{
  वर्ण buf[BUFSIZ];

  /* Copy the file contents.  */
  यावत् (0 < n_अष्टकानि)
    {
      idx_t n_गन्तव्यम्_read = MIN (n_अष्टकानि, परिमाण buf);
      ssize_t n_read = पठ (src_fd, buf, n_गन्तव्यम्_read);
      यदि (n_read < 0)
        प्रत्यागम COPY_FD_READ_ERROR;

      n_अष्टकानि -= n_read;

      यदि (n_read == 0 && n_अष्टकानि != 0)
        प्रत्यागम COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  प्रत्यागम COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
स्थिर शून्य
elseek_diagnostic (off_t विस्थापनम्, पूर्णाङ्क whence, वर्ण ध्रुव *filename)
{
  intmax_t off = विस्थापनम्;
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
elseek (पूर्णाङ्क fd, off_t विस्थापनम्, पूर्णाङ्क whence, वर्ण ध्रुव *filename)
{
  off_t new_विस्थापनम् = lseek (fd, विस्थापनम्, whence);

  यदि (new_विस्थापनम् < 0)
    elseek_diagnostic (विस्थापनम्, whence, filename);

  प्रत्यागम new_विस्थापनम्;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
स्थिर bool
elide_पुच्छ_अष्टकानि_pipe (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t n_elide,
                       off_t current_pos)
{
  uintmax_t desired_pos = current_pos;
  bool ok = true;

#यदि_अपरिभाषितम् शीर्ष_पुच्छ_PIPE_READ_BUFSIZE
# define शीर्ष_पुच्छ_PIPE_READ_BUFSIZE BUFSIZ
#यदि_अन्तः
#परिभाषय READ_BUFSIZE शीर्ष_पुच्छ_PIPE_READ_BUFSIZE

  /* If we're eliding no more than this many bytes, then it's ok to allocate
     more memory in order to use a more time-efficient algorithm.
     FIXME: use a fraction of available memory instead, as in sort.
     FIXME: is this even worthwhile?  */
#यदि_अपरिभाषितम् शीर्ष_पुच्छ_PIPE_BYTECOUNT_देहली
# define शीर्ष_पुच्छ_PIPE_BYTECOUNT_देहली 1024 * 1024
#यदि_अन्तः

#यदि शीर्ष_पुच्छ_PIPE_BYTECOUNT_देहली < 2 * READ_BUFSIZE
# error "HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD must be at least 2 * READ_BUFSIZE"
#यदि_अन्तः

  /* Two cases to consider...
     1) n_elide is small enough that we can afford to double-buffer:
        allocate 2 * (READ_BUFSIZE + n_elide) bytes
     2) n_elide is too big for that, so we allocate only
        (READ_BUFSIZE + n_elide) bytes

     FIXME: profile, to see if double-buffering is worthwhile

     CAUTION: do not fail (out of memory) when asked to elide
     a ridiculous amount, but when given only a small input.  */

  static_assert (READ_BUFSIZE <= IDX_MAX);
  static_assert (शीर्ष_पुच्छ_PIPE_BYTECOUNT_देहली <= IDX_MAX - READ_BUFSIZE);
  यदि (n_elide <= शीर्ष_पुच्छ_PIPE_BYTECOUNT_देहली)
    {
      idx_t अन्तर्_elide = n_elide;
      bool first = true;
      bool eof = false;
      idx_t n_गन्तव्यम्_read = READ_BUFSIZE + n_elide;
      वर्ण *b[2];
      b[0] = xnmalloc (2, n_गन्तव्यम्_read);
      b[1] = b[0] + n_गन्तव्यम्_read;

      प्रति (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = पूर्णम्_read (fd, b[i], n_गन्तव्यम्_read);
          idx_t delta = 0;
          यदि (n_read < n_गन्तव्यम्_read)
            {
              यदि (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  भङ्ग;
                }

              /* reached EOF */
              यदि (n_read <= अन्तर्_elide)
                {
                  यदि (first)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  अन्यथा
                    {
                      delta = अन्तर्_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          यदि (! first)
            {
              desired_pos += अन्तर्_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, अन्तर्_elide - delta);
            }
          first = false;

          यदि (अन्तर्_elide < n_read)
            {
              desired_pos += n_read - अन्तर्_elide;
              xwrite_stdout (b[i], n_read - अन्तर्_elide);
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
      वर्ण **b = रिक्त;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_अग्र;
      प्रति (i = 0, i_अग्र = 1; !eof; i = i_अग्र, i_अग्र = (i_अग्र + 1) % n_bufs)
        {
          यदि (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         परिमाण *b);

          यदि (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = पूर्णम्_read (fd, b[i], READ_BUFSIZE);
          यदि (n_read < READ_BUFSIZE)
            {
              यदि (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  गच्छ free_mem;
                }
              eof = true;
            }

          यदि (i + 1 == n_bufs)
            buffered_enough = true;

          यदि (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_अग्र], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      यदि (buffered_enough)
        {
          idx_t n_अष्टकानि_वामः_अन्तर्_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          यदि (rem < n_अष्टकानि_वामः_अन्तर्_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          अन्यथा
            {
              xwrite_stdout (b[i] + n_read, n_अष्टकानि_वामः_अन्तर्_b_i);
              xwrite_stdout (b[i_अग्र], rem - n_अष्टकानि_वामः_अन्तर्_b_i);
            }
        }
      अन्यथा यदि (i + 1 == n_bufs)
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
      प्रति (i = 0; i < n_alloc; i++)
        मुक्त (b[i]);
      मुक्त (b);
    }

  यदि (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  प्रत्यागम ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

स्थिर bool
elide_पुच्छ_अष्टकानि_file (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t n_elide,
                       संरचना stat ध्रुव *st, off_t current_pos)
{
  off_t आकार = st->st_आकार;
  यदि (current_pos < 0 || आकार <= STP_BLKSIZE (st))
    प्रत्यागम elide_पुच्छ_अष्टकानि_pipe (filename, fd, n_elide, current_pos);
  अन्यथा
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = आकार - current_pos;
      off_t अष्टकानि_remaining = diff < 0 ? 0 : diff;

      यदि (अष्टकानि_remaining <= n_elide)
        प्रत्यागम true;

      गणना Copy_fd_स्थितिः err = copy_fd (fd, अष्टकानि_remaining - n_elide);
      यदि (err == COPY_FD_OK)
        प्रत्यागम true;

      diagnose_copy_fd_failure (err, filename);
      प्रत्यागम false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

स्थिर bool
elide_पुच्छ_पङ्क्तयः_pipe (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t n_elide,
                       off_t current_pos)
{
  संरचना linebuffer
  {
    वर्ण बफर[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    संरचना linebuffer *अग्र;
  };
  off_t desired_pos = current_pos;
  प्रकारनिर्देश संरचना linebuffer LBUFFER;
  LBUFFER *first, *चरमः, *tmp;
  size_t योग_पङ्क्तयः = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = चरमः = xmalloc (परिमाण (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->अग्र = रिक्त;
  tmp = xmalloc (परिमाण (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  यावत् (true)
    {
      n_read = पठ (fd, tmp->बफर, BUFSIZ);
      यदि (n_read <= 0)
        भङ्ग;

      यदि (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->बफर, n_read);
          अनुवर्त;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->अग्र = रिक्त;

      /* Count the number of newlines just read.  */
      {
        वर्ण *बफर_अन्तः = tmp->बफर + n_read;
        *बफर_अन्तः = पङ्क्तिः_अन्तः;
        वर्ण ध्रुव *p = tmp->बफर;
        यावत् ((p = rawmemchr (p, पङ्क्तिः_अन्तः)) < बफर_अन्तः)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      योग_पङ्क्तयः += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      यदि (tmp->nbytes + चरमः->nbytes < BUFSIZ)
        {
          स्मृतिं_प्रतिलिख (&चरमः->बफर[चरमः->nbytes], tmp->बफर, tmp->nbytes);
          चरमः->nbytes += tmp->nbytes;
          चरमः->nlines += tmp->nlines;
        }
      अन्यथा
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          चरमः = चरमः->अग्र = tmp;
          यदि (n_elide < योग_पङ्क्तयः - first->nlines)
            {
              desired_pos += first->nbytes;
              xwrite_stdout (first->बफर, first->nbytes);
              tmp = first;
              योग_पङ्क्तयः -= first->nlines;
              first = first->अग्र;
            }
          अन्यथा
            tmp = xmalloc (परिमाण (LBUFFER));
        }
    }

  मुक्त (tmp);

  यदि (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      गच्छ free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  यदि (चरमः->nbytes && चरमः->बफर[चरमः->nbytes - 1] != पङ्क्तिः_अन्तः)
    {
      ++चरमः->nlines;
      ++योग_पङ्क्तयः;
    }

  प्रति (tmp = first; n_elide < योग_पङ्क्तयः - tmp->nlines; tmp = tmp->अग्र)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->बफर, tmp->nbytes);
      योग_पङ्क्तयः -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  यदि (n_elide < योग_पङ्क्तयः)
    {
      size_t n = योग_पङ्क्तयः - n_elide;
      वर्ण ध्रुव *बफर_अन्तः = tmp->बफर + tmp->nbytes;
      वर्ण ध्रुव *p = tmp->बफर;
      यावत् (n && (p = memchr (p, पङ्क्तिः_अन्तः, बफर_अन्तः - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->बफर;
      xwrite_stdout (tmp->बफर, p - tmp->बफर);
    }

free_lbuffers:
  यावत् (first)
    {
      tmp = first->अग्र;
      मुक्त (first);
      first = tmp;
    }

  यदि (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  प्रत्यागम ok;
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
elide_पुच्छ_पङ्क्तयः_seekable (वर्ण ध्रुव *pretty_filename, पूर्णाङ्क fd,
                           uintmax_t n_पङ्क्तयः,
                           off_t start_pos, off_t आकार)
{
  वर्ण बफर[BUFSIZ];
  ssize_t अष्टकानि_read;
  off_t pos = आकार;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  अष्टकानि_read = (pos - start_pos) % BUFSIZ;
  यदि (अष्टकानि_read == 0)
    अष्टकानि_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= अष्टकानि_read;
  यदि (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    प्रत्यागम false;
  अष्टकानि_read = पठ (fd, बफर, अष्टकानि_read);
  यदि (अष्टकानि_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      प्रत्यागम false;
    }

  /* n_lines == 0 case needs special treatment. */
  ध्रुव bool all_पङ्क्तयः = !n_पङ्क्तयः;

  /* Count the incomplete line on files that don't end with a newline.  */
  यदि (n_पङ्क्तयः && अष्टकानि_read && बफर[अष्टकानि_read - 1] != पङ्क्तिः_अन्तः)
    --n_पङ्क्तयः;

  यावत् (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = अष्टकानि_read;
      यावत् (n)
        {
          यदि (all_पङ्क्तयः)
            n -= 1;
          अन्यथा
            {
              वर्ण ध्रुव *nl;
              nl = memrchr (बफर, पङ्क्तिः_अन्तः, n);
              यदि (nl == रिक्त)
                भङ्ग;
              n = nl - बफर;
            }
          यदि (n_पङ्क्तयः-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              यदि (start_pos < pos)
                {
                  गणना Copy_fd_स्थितिः err;
                  यदि (elseek (fd, start_pos, SEEK_SET, pretty_filename) < 0)
                    प्रत्यागम false;

                  err = copy_fd (fd, pos - start_pos);
                  यदि (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      प्रत्यागम false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (बफर, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              प्रत्यागम 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      यदि (pos == start_pos)
        {
          /* Not enough lines in the file.  */
          प्रत्यागम true;
        }
      pos -= BUFSIZ;
      यदि (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        प्रत्यागम false;

      अष्टकानि_read = पठ (fd, बफर, BUFSIZ);
      यदि (अष्टकानि_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          प्रत्यागम false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      यदि (अष्टकानि_read == 0)
        प्रत्यागम true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

स्थिर bool
elide_पुच्छ_पङ्क्तयः_file (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t n_elide,
                       संरचना stat ध्रुव *st, off_t current_pos)
{
  off_t आकार = st->st_आकार;
  यदि (current_pos < 0 || आकार <= STP_BLKSIZE (st))
    प्रत्यागम elide_पुच्छ_पङ्क्तयः_pipe (filename, fd, n_elide, current_pos);
  अन्यथा
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      प्रत्यागम (आकार <= current_pos
              || elide_पुच्छ_पङ्क्तयः_seekable (filename, fd, n_elide,
                                            current_pos, आकार));
    }
}

स्थिर bool
शीर्ष_अष्टकानि (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t अष्टकानि_गन्तव्यम्_write)
{
  यदि (copy_fd (fd, अष्टकानि_गन्तव्यम्_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      प्रत्यागम false;
    }
  प्रत्यागम true;
}

स्थिर bool
शीर्ष_पङ्क्तयः (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t पङ्क्तयः_गन्तव्यम्_write)
{
  वर्ण बफर[BUFSIZ];

  यावत् (पङ्क्तयः_गन्तव्यम्_write)
    {
      ssize_t अष्टकानि_read = पठ (fd, बफर, BUFSIZ);
      idx_t अष्टकानि_गन्तव्यम्_write = 0;

      यदि (अष्टकानि_read < 0)
        {
          diagnose_read_failure (filename);
          प्रत्यागम false;
        }
      यदि (अष्टकानि_read == 0)
        भङ्ग;
      यावत् (अष्टकानि_गन्तव्यम्_write < अष्टकानि_read)
        यदि (बफर[अष्टकानि_गन्तव्यम्_write++] == पङ्क्तिः_अन्तः && --पङ्क्तयः_गन्तव्यम्_write == 0)
          {
            off_t n_अष्टकानि_past_EOL = अष्टकानि_read - अष्टकानि_गन्तव्यम्_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            यदि (lseek (fd, -n_अष्टकानि_past_EOL, SEEK_CUR) < 0)
              {
                संरचना stat st;
                यदि (fstat (fd, &st) != 0 || S_ISREG (st.st_बहुलकम्))
                  elseek_diagnostic (-n_अष्टकानि_past_EOL, SEEK_CUR, filename);
              }
            भङ्ग;
          }
      xwrite_stdout (बफर, अष्टकानि_गन्तव्यम्_write);
    }
  प्रत्यागम true;
}

स्थिर bool
शीर्ष (वर्ण ध्रुव *filename, पूर्णाङ्क fd, uintmax_t n_units, bool गणनम्_पङ्क्तयः,
      bool elide_from_अन्तः)
{
  यदि (print_शीर्षकाणि)
    write_शीर्षकम् (filename);

  यदि (elide_from_अन्तः)
    {
      /* Optimize for "infinite" elisions.  */
      यदि (n_units == UINTMAX_MAX)
        प्रत्यागम true;

      off_t current_pos = -1;
      संरचना stat st;
      यदि (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          प्रत्यागम false;
        }
      यदि (! presume_निवेशः_pipe && S_ISREG (st.st_बहुलकम्))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          यदि (current_pos < 0)
            प्रत्यागम false;
        }
      यदि (गणनम्_पङ्क्तयः)
        प्रत्यागम elide_पुच्छ_पङ्क्तयः_file (filename, fd, n_units, &st, current_pos);
      अन्यथा
        प्रत्यागम elide_पुच्छ_अष्टकानि_file (filename, fd, n_units, &st, current_pos);
    }
  यदि (गणनम्_पङ्क्तयः)
    प्रत्यागम शीर्ष_पङ्क्तयः (filename, fd, n_units);
  अन्यथा
    प्रत्यागम शीर्ष_अष्टकानि (filename, fd, n_units);
}

स्थिर bool
शीर्ष_file (वर्ण ध्रुव *filename, uintmax_t n_units, bool गणनम्_पङ्क्तयः,
           bool elide_from_अन्तः)
{
  पूर्णाङ्क fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  यदि (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_द्विमान_बहुलकम् (STDIN_FILENO, O_द्विमान);
    }
  अन्यथा
    {
      fd = उद्घाटय (filename, O_RDONLY | O_द्विमान);
      यदि (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          प्रत्यागम false;
        }
    }

  ok = शीर्ष (filename, fd, n_units, गणनम्_पङ्क्तयः, elide_from_अन्तः);
  यदि (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      प्रत्यागम false;
    }
  प्रत्यागम ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

स्थिर uintmax_t
string_गन्तव्यम्_integer (bool गणनम्_पङ्क्तयः, वर्ण ध्रुव *n_string)
{
  प्रत्यागम xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (गणनम्_पङ्क्तयः
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  गणना शीर्षकम्_बहुलकम् शीर्षकम्_बहुलकम् = गुणजः_files;
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
  bool elide_from_अन्तः = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  स्थिर वर्ण ध्रुव *ध्रुव default_file_list[] = {"-", रिक्त};
  वर्ण ध्रुव *ध्रुव *file_list;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यदि (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      वर्ण *a = argv[1];
      वर्ण *n_string = ++a;
      वर्ण *अन्तः_n_string;
      वर्ण multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      कुरु ++a;
      यावत् (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      अन्तः_n_string = a;

      /* Parse any appended option letters. */
      प्रति (; *a; a++)
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
              शीर्षकम्_बहुलकम् = never;
              भङ्ग;

            पक्ष 'v':
              शीर्षकम्_बहुलकम् = always;
              भङ्ग;

            पक्ष 'z':
              पङ्क्तिः_अन्तः = '\0';
              भङ्ग;

            स्वभाव:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *अन्तः_n_string = multiplier_char;
      यदि (multiplier_char)
        *(++अन्तः_n_string) = 0;

      n_units = string_गन्तव्यम्_integer (गणनम्_पङ्क्तयः, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  यावत् ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
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
          elide_from_अन्तः = (*optarg == '-');
          यदि (elide_from_अन्तः)
            ++optarg;
          n_units = string_गन्तव्यम्_integer (गणनम्_पङ्क्तयः, optarg);
          भङ्ग;

        पक्ष 'n':
          गणनम्_पङ्क्तयः = true;
          elide_from_अन्तः = (*optarg == '-');
          यदि (elide_from_अन्तः)
            ++optarg;
          n_units = string_गन्तव्यम्_integer (गणनम्_पङ्क्तयः, optarg);
          भङ्ग;

        पक्ष 'q':
          शीर्षकम्_बहुलकम् = never;
          भङ्ग;

        पक्ष 'v':
          शीर्षकम्_बहुलकम् = always;
          भङ्ग;

        पक्ष 'z':
          पङ्क्तिः_अन्तः = '\0';
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          यदि (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  यदि (शीर्षकम्_बहुलकम् == always
      || (शीर्षकम्_बहुलकम् == गुणजः_files && optind < argc - 1))
    print_शीर्षकाणि = true;

  file_list = (optind < argc
               ? (वर्ण ध्रुव *ध्रुव *) &argv[optind]
               : default_file_list);

  xset_द्विमान_बहुलकम् (STDOUT_FILENO, O_द्विमान);

  प्रति (size_t i = 0; file_list[i]; ++i)
    ok &= शीर्ष_file (file_list[i], n_units, गणनम्_पङ्क्तयः, elide_from_अन्तः);

  यदि (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
