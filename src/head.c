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
#تعریف_کرو پروگرام_NAME "head"

#تعریف_کرو AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#تعریف_کرو DEFAULT_نمبر 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ساكن bool presume_داخلہ_pipe;

/* If true, print filename headers. */
ساكن bool print_سرخیاں;

/* Character to split lines by. */
ساكن حرف سطر_سرا = '\n';

/* When to print the filename banners. */
تعداد سرخی_منوال
{
  مضرب_files, always, never
};

/* Have we ever read standard input?  */
ساكن bool have_read_stdin;

تعداد Copy_fd_حالت
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  PRESUME_داخلہ_PIPE_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت long_options[] =
{
  {"bytes", required_دلیل, عدم, 'c'},
  {"lines", required_دلیل, عدم, 'n'},
  {"-presume-input-pipe", no_دلیل, عدم,
   PRESUME_داخلہ_PIPE_OPTION}, /* do not document */
  {"quiet", no_دلیل, عدم, 'q'},
  {"silent", no_دلیل, عدم, 'q'},
  {"verbose", no_دلیل, عدم, 'v'},
  {"zero-terminated", no_دلیل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      لکھو (_("\
Print the پہلا %d سطریں دا each FILE منزل standard output.\n\
With ہور than one FILE, precede each with a سرخی giving the file name.\n\
"), DEFAULT_نمبر);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --بائٹاں=[-]عدد_2\n\
         print the پہلا عدد_2 بائٹاں دا each file;\n\
         with the leading '-', print all but the آخری عدد_2 بائٹاں دا each file\n\
"));
      oprintf (_("\
  -n, --سطریں=[-]عدد_2\n\
         print the پہلا عدد_2 سطریں instead دا the پہلا %d;\n\
         with the leading '-', print all but the آخری عدد_2 سطریں دا each file\n\
"), DEFAULT_نمبر);
      oputs (_("\
  -q, --quiet, --silent\n\
         never print سرخیاں giving file ناں\n\
"));
      oputs (_("\
  -v, --verbose\n\
         always print سرخیاں giving file ناں\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
عدد_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on لكل T, P, E, Z, Y, R, Q.\n\
ثنائی_2 prefixes can be ورتیا, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

ساكن فراغ
diagnose_read_failure (حرف ثابت *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

ساكن فراغ
diagnose_copy_fd_failure (تعداد Copy_fd_حالت err, حرف ثابت *filename)
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
write_سرخی (حرف ثابت *filename)
{
  ساكن bool پہلا_file = true;

  لکھو ("%s==> %s <==\n", (پہلا_file ? "" : "\n"), quotef (filename));
  پہلا_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ساكن فراغ
xwrite_stdout (حرف ثابت *بفر, size_t n_بائٹاں)
{
  إذا (n_بائٹاں > 0 && fwrite (بفر, 1, n_بائٹاں, stdout) < n_بائٹاں)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

ساكن تعداد Copy_fd_حالت
copy_fd (صحيح src_fd, uintmax_t n_بائٹاں)
{
  حرف buf[BUFSIZ];

  /* Copy the file contents.  */
  طالما (0 < n_بائٹاں)
    {
      idx_t n_منزل_read = MIN (n_بائٹاں, حجم buf);
      ssize_t n_read = اقرأ (src_fd, buf, n_منزل_read);
      إذا (n_read < 0)
        أرجع COPY_FD_READ_ERROR;

      n_بائٹاں -= n_read;

      إذا (n_read == 0 && n_بائٹاں != 0)
        أرجع COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  أرجع COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
ساكن فراغ
elseek_diagnostic (off_t وتھ, صحيح whence, حرف ثابت *filename)
{
  intmax_t off = وتھ;
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
elseek (صحيح fd, off_t وتھ, صحيح whence, حرف ثابت *filename)
{
  off_t new_وتھ = lseek (fd, وتھ, whence);

  إذا (new_وتھ < 0)
    elseek_diagnostic (وتھ, whence, filename);

  أرجع new_وتھ;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
ساكن bool
elide_پوچھ_بائٹاں_pipe (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       off_t current_pos)
{
  uintmax_t desired_pos = current_pos;
  bool ok = true;

#جے_بنا_تعریف سر_پوچھ_PIPE_READ_BUFSIZE
# define سر_پوچھ_PIPE_READ_BUFSIZE BUFSIZ
#جے_انت
#تعریف_کرو READ_BUFSIZE سر_پوچھ_PIPE_READ_BUFSIZE

  /* If we're eliding no more than this many bytes, then it's ok to allocate
     more memory in order to use a more time-efficient algorithm.
     FIXME: use a fraction of available memory instead, as in sort.
     FIXME: is this even worthwhile?  */
#جے_بنا_تعریف سر_پوچھ_PIPE_BYTECOUNT_حد
# define سر_پوچھ_PIPE_BYTECOUNT_حد 1024 * 1024
#جے_انت

#إذا سر_پوچھ_PIPE_BYTECOUNT_حد < 2 * READ_BUFSIZE
# error "HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD must be at least 2 * READ_BUFSIZE"
#جے_انت

  /* Two cases to consider...
     1) n_elide is small enough that we can afford to double-buffer:
        allocate 2 * (READ_BUFSIZE + n_elide) bytes
     2) n_elide is too big for that, so we allocate only
        (READ_BUFSIZE + n_elide) bytes

     FIXME: profile, to see if double-buffering is worthwhile

     CAUTION: do not fail (out of memory) when asked to elide
     a ridiculous amount, but when given only a small input.  */

  static_assert (READ_BUFSIZE <= IDX_MAX);
  static_assert (سر_پوچھ_PIPE_BYTECOUNT_حد <= IDX_MAX - READ_BUFSIZE);
  إذا (n_elide <= سر_پوچھ_PIPE_BYTECOUNT_حد)
    {
      idx_t وچ_elide = n_elide;
      bool پہلا = true;
      bool eof = false;
      idx_t n_منزل_read = READ_BUFSIZE + n_elide;
      حرف *b[2];
      b[0] = xnmalloc (2, n_منزل_read);
      b[1] = b[0] + n_منزل_read;

      لكل (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = بھریا_read (fd, b[i], n_منزل_read);
          idx_t delta = 0;
          إذا (n_read < n_منزل_read)
            {
              إذا (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  اكسر;
                }

              /* reached EOF */
              إذا (n_read <= وچ_elide)
                {
                  إذا (پہلا)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  وإلا
                    {
                      delta = وچ_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          إذا (! پہلا)
            {
              desired_pos += وچ_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, وچ_elide - delta);
            }
          پہلا = false;

          إذا (وچ_elide < n_read)
            {
              desired_pos += n_read - وچ_elide;
              xwrite_stdout (b[i], n_read - وچ_elide);
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
      idx_t i, i_اگلا;
      لكل (i = 0, i_اگلا = 1; !eof; i = i_اگلا, i_اگلا = (i_اگلا + 1) % n_bufs)
        {
          إذا (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         حجم *b);

          إذا (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = بھریا_read (fd, b[i], READ_BUFSIZE);
          إذا (n_read < READ_BUFSIZE)
            {
              إذا (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  اذهب free_mem;
                }
              eof = true;
            }

          إذا (i + 1 == n_bufs)
            buffered_enough = true;

          إذا (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_اگلا], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      إذا (buffered_enough)
        {
          idx_t n_بائٹاں_بایاں_وچ_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          إذا (rem < n_بائٹاں_بایاں_وچ_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          وإلا
            {
              xwrite_stdout (b[i] + n_read, n_بائٹاں_بایاں_وچ_b_i);
              xwrite_stdout (b[i_اگلا], rem - n_بائٹاں_بایاں_وچ_b_i);
            }
        }
      وإلا إذا (i + 1 == n_bufs)
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
          xwrite_stdout (b[i_اگلا], x);
        }

    free_mem:
      لكل (i = 0; i < n_alloc; i++)
        حرر (b[i]);
      حرر (b);
    }

  إذا (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
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
elide_پوچھ_بائٹاں_file (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       بنية stat ثابت *st, off_t current_pos)
{
  off_t ناپ = st->st_ناپ;
  إذا (current_pos < 0 || ناپ <= STP_BLKSIZE (st))
    أرجع elide_پوچھ_بائٹاں_pipe (filename, fd, n_elide, current_pos);
  وإلا
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = ناپ - current_pos;
      off_t بائٹاں_بچیا = diff < 0 ? 0 : diff;

      إذا (بائٹاں_بچیا <= n_elide)
        أرجع true;

      تعداد Copy_fd_حالت err = copy_fd (fd, بائٹاں_بچیا - n_elide);
      إذا (err == COPY_FD_OK)
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
elide_پوچھ_سطریں_pipe (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       off_t current_pos)
{
  بنية linebuffer
  {
    حرف بفر[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    بنية linebuffer *اگلا;
  };
  off_t desired_pos = current_pos;
  تعريفنوع بنية linebuffer LBUFFER;
  LBUFFER *پہلا, *آخری, *tmp;
  size_t کل_سطریں = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  پہلا = آخری = xmalloc (حجم (LBUFFER));
  پہلا->nbytes = پہلا->nlines = 0;
  پہلا->اگلا = عدم;
  tmp = xmalloc (حجم (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  طالما (true)
    {
      n_read = اقرأ (fd, tmp->بفر, BUFSIZ);
      إذا (n_read <= 0)
        اكسر;

      إذا (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->بفر, n_read);
          استمر;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->اگلا = عدم;

      /* Count the number of newlines just read.  */
      {
        حرف *بفر_سرا = tmp->بفر + n_read;
        *بفر_سرا = سطر_سرا;
        حرف ثابت *p = tmp->بفر;
        طالما ((p = rawmemchr (p, سطر_سرا)) < بفر_سرا)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      کل_سطریں += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      إذا (tmp->nbytes + آخری->nbytes < BUFSIZ)
        {
          حافظہ_نقل (&آخری->بفر[آخری->nbytes], tmp->بفر, tmp->nbytes);
          آخری->nbytes += tmp->nbytes;
          آخری->nlines += tmp->nlines;
        }
      وإلا
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          آخری = آخری->اگلا = tmp;
          إذا (n_elide < کل_سطریں - پہلا->nlines)
            {
              desired_pos += پہلا->nbytes;
              xwrite_stdout (پہلا->بفر, پہلا->nbytes);
              tmp = پہلا;
              کل_سطریں -= پہلا->nlines;
              پہلا = پہلا->اگلا;
            }
          وإلا
            tmp = xmalloc (حجم (LBUFFER));
        }
    }

  حرر (tmp);

  إذا (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      اذهب free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  إذا (آخری->nbytes && آخری->بفر[آخری->nbytes - 1] != سطر_سرا)
    {
      ++آخری->nlines;
      ++کل_سطریں;
    }

  لكل (tmp = پہلا; n_elide < کل_سطریں - tmp->nlines; tmp = tmp->اگلا)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->بفر, tmp->nbytes);
      کل_سطریں -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  إذا (n_elide < کل_سطریں)
    {
      size_t n = کل_سطریں - n_elide;
      حرف ثابت *بفر_سرا = tmp->بفر + tmp->nbytes;
      حرف ثابت *p = tmp->بفر;
      طالما (n && (p = memchr (p, سطر_سرا, بفر_سرا - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->بفر;
      xwrite_stdout (tmp->بفر, p - tmp->بفر);
    }

free_lbuffers:
  طالما (پہلا)
    {
      tmp = پہلا->اگلا;
      حرر (پہلا);
      پہلا = tmp;
    }

  إذا (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
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
elide_پوچھ_سطریں_seekable (حرف ثابت *pretty_filename, صحيح fd,
                           uintmax_t n_سطریں,
                           off_t شروع_pos, off_t ناپ)
{
  حرف بفر[BUFSIZ];
  ssize_t بائٹاں_read;
  off_t pos = ناپ;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  بائٹاں_read = (pos - شروع_pos) % BUFSIZ;
  إذا (بائٹاں_read == 0)
    بائٹاں_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= بائٹاں_read;
  إذا (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    أرجع false;
  بائٹاں_read = اقرأ (fd, بفر, بائٹاں_read);
  إذا (بائٹاں_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      أرجع false;
    }

  /* n_lines == 0 case needs special treatment. */
  ثابت bool all_سطریں = !n_سطریں;

  /* Count the incomplete line on files that don't end with a newline.  */
  إذا (n_سطریں && بائٹاں_read && بفر[بائٹاں_read - 1] != سطر_سرا)
    --n_سطریں;

  طالما (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = بائٹاں_read;
      طالما (n)
        {
          إذا (all_سطریں)
            n -= 1;
          وإلا
            {
              حرف ثابت *nl;
              nl = memrchr (بفر, سطر_سرا, n);
              إذا (nl == عدم)
                اكسر;
              n = nl - بفر;
            }
          إذا (n_سطریں-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              إذا (شروع_pos < pos)
                {
                  تعداد Copy_fd_حالت err;
                  إذا (elseek (fd, شروع_pos, SEEK_SET, pretty_filename) < 0)
                    أرجع false;

                  err = copy_fd (fd, pos - شروع_pos);
                  إذا (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      أرجع false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (بفر, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              أرجع 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      إذا (pos == شروع_pos)
        {
          /* Not enough lines in the file.  */
          أرجع true;
        }
      pos -= BUFSIZ;
      إذا (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        أرجع false;

      بائٹاں_read = اقرأ (fd, بفر, BUFSIZ);
      إذا (بائٹاں_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          أرجع false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      إذا (بائٹاں_read == 0)
        أرجع true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

ساكن bool
elide_پوچھ_سطریں_file (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       بنية stat ثابت *st, off_t current_pos)
{
  off_t ناپ = st->st_ناپ;
  إذا (current_pos < 0 || ناپ <= STP_BLKSIZE (st))
    أرجع elide_پوچھ_سطریں_pipe (filename, fd, n_elide, current_pos);
  وإلا
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      أرجع (ناپ <= current_pos
              || elide_پوچھ_سطریں_seekable (filename, fd, n_elide,
                                            current_pos, ناپ));
    }
}

ساكن bool
سر_بائٹاں (حرف ثابت *filename, صحيح fd, uintmax_t بائٹاں_منزل_write)
{
  إذا (copy_fd (fd, بائٹاں_منزل_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      أرجع false;
    }
  أرجع true;
}

ساكن bool
سر_سطریں (حرف ثابت *filename, صحيح fd, uintmax_t سطریں_منزل_write)
{
  حرف بفر[BUFSIZ];

  طالما (سطریں_منزل_write)
    {
      ssize_t بائٹاں_read = اقرأ (fd, بفر, BUFSIZ);
      idx_t بائٹاں_منزل_write = 0;

      إذا (بائٹاں_read < 0)
        {
          diagnose_read_failure (filename);
          أرجع false;
        }
      إذا (بائٹاں_read == 0)
        اكسر;
      طالما (بائٹاں_منزل_write < بائٹاں_read)
        إذا (بفر[بائٹاں_منزل_write++] == سطر_سرا && --سطریں_منزل_write == 0)
          {
            off_t n_بائٹاں_past_EOL = بائٹاں_read - بائٹاں_منزل_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            إذا (lseek (fd, -n_بائٹاں_past_EOL, SEEK_CUR) < 0)
              {
                بنية stat st;
                إذا (fstat (fd, &st) != 0 || S_ISREG (st.st_منوال))
                  elseek_diagnostic (-n_بائٹاں_past_EOL, SEEK_CUR, filename);
              }
            اكسر;
          }
      xwrite_stdout (بفر, بائٹاں_منزل_write);
    }
  أرجع true;
}

ساكن bool
سر (حرف ثابت *filename, صحيح fd, uintmax_t n_units, bool گنتی_سطریں,
      bool elide_from_سرا)
{
  إذا (print_سرخیاں)
    write_سرخی (filename);

  إذا (elide_from_سرا)
    {
      /* Optimize for "infinite" elisions.  */
      إذا (n_units == UINTMAX_MAX)
        أرجع true;

      off_t current_pos = -1;
      بنية stat st;
      إذا (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          أرجع false;
        }
      إذا (! presume_داخلہ_pipe && S_ISREG (st.st_منوال))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          إذا (current_pos < 0)
            أرجع false;
        }
      إذا (گنتی_سطریں)
        أرجع elide_پوچھ_سطریں_file (filename, fd, n_units, &st, current_pos);
      وإلا
        أرجع elide_پوچھ_بائٹاں_file (filename, fd, n_units, &st, current_pos);
    }
  إذا (گنتی_سطریں)
    أرجع سر_سطریں (filename, fd, n_units);
  وإلا
    أرجع سر_بائٹاں (filename, fd, n_units);
}

ساكن bool
سر_file (حرف ثابت *filename, uintmax_t n_units, bool گنتی_سطریں,
           bool elide_from_سرا)
{
  صحيح fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  إذا (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
    }
  وإلا
    {
      fd = open (filename, O_RDONLY | O_ثنائی);
      إذا (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          أرجع false;
        }
    }

  ok = سر (filename, fd, n_units, گنتی_سطریں, elide_from_سرا);
  إذا (!is_stdin && close (fd) != 0)
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
string_منزل_integer (bool گنتی_سطریں, حرف ثابت *n_string)
{
  أرجع xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (گنتی_سطریں
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  تعداد سرخی_منوال سرخی_منوال = مضرب_files;
  bool ok = true;
  صحيح c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_نمبر;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool گنتی_سطریں = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_سرا = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ساكن حرف ثابت *ثابت default_file_list[] = {"-", عدم};
  حرف ثابت *ثابت *file_list;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  إذا (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      حرف *a = argv[1];
      حرف *n_string = ++a;
      حرف *سرا_n_string;
      حرف multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      افعل ++a;
      طالما (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      سرا_n_string = a;

      /* Parse any appended option letters. */
      لكل (; *a; a++)
        {
          بدل (*a)
            {
            حالة 'c':
              گنتی_سطریں = false;
              multiplier_char = 0;
              اكسر;

            حالة 'b':
            حالة 'k':
            حالة 'm':
              گنتی_سطریں = false;
              multiplier_char = *a;
              اكسر;

            حالة 'l':
              گنتی_سطریں = true;
              اكسر;

            حالة 'q':
              سرخی_منوال = never;
              اكسر;

            حالة 'v':
              سرخی_منوال = always;
              اكسر;

            حالة 'z':
              سطر_سرا = '\0';
              اكسر;

            افتراضي:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *سرا_n_string = multiplier_char;
      إذا (multiplier_char)
        *(++سرا_n_string) = 0;

      n_units = string_منزل_integer (گنتی_سطریں, n_string);

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
        حالة PRESUME_داخلہ_PIPE_OPTION:
          presume_داخلہ_pipe = true;
          اكسر;

        حالة 'c':
          گنتی_سطریں = false;
          elide_from_سرا = (*optarg == '-');
          إذا (elide_from_سرا)
            ++optarg;
          n_units = string_منزل_integer (گنتی_سطریں, optarg);
          اكسر;

        حالة 'n':
          گنتی_سطریں = true;
          elide_from_سرا = (*optarg == '-');
          إذا (elide_from_سرا)
            ++optarg;
          n_units = string_منزل_integer (گنتی_سطریں, optarg);
          اكسر;

        حالة 'q':
          سرخی_منوال = never;
          اكسر;

        حالة 'v':
          سرخی_منوال = always;
          اكسر;

        حالة 'z':
          سطر_سرا = '\0';
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          إذا (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  إذا (سرخی_منوال == always
      || (سرخی_منوال == مضرب_files && optind < argc - 1))
    print_سرخیاں = true;

  file_list = (optind < argc
               ? (حرف ثابت *ثابت *) &argv[optind]
               : default_file_list);

  xset_ثنائی_منوال (STDOUT_FILENO, O_ثنائی);

  لكل (size_t i = 0; file_list[i]; ++i)
    ok &= سر_file (file_list[i], n_units, گنتی_سطریں, elide_from_سرا);

  إذا (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
