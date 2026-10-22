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
#تعریف_کن برنامه_NAME "head"

#تعریف_کن AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#تعریف_کن DEFAULT_عدد 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ساكن bool presume_درون_داد_pipe;

/* If true, print filename headers. */
ساكن bool print_سرآیندها;

/* Character to split lines by. */
ساكن حرف سطر_انتها = '\n';

/* When to print the filename banners. */
تعداد سرآیند_نما
{
  مضرب_files, always, never
};

/* Have we ever read standard input?  */
ساكن bool have_read_stdin;

تعداد Copy_fd_وضعیت
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  PRESUME_درون_داد_PIPE_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت long_options[] =
{
  {"bytes", required_آرگومان, عدم, 'c'},
  {"lines", required_آرگومان, عدم, 'n'},
  {"-presume-input-pipe", no_آرگومان, عدم,
   PRESUME_درون_داد_PIPE_OPTION}, /* do not document */
  {"quiet", no_آرگومان, عدم, 'q'},
  {"silent", no_آرگومان, عدم, 'q'},
  {"verbose", no_آرگومان, عدم, 'v'},
  {"zero-terminated", no_آرگومان, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              برنامه_name);
      چاپ_کن (_("\
Print the اولی %d سطرها از each FILE مقصد standard output.\n\
With بیشتر than one FILE, precede each with a سرآیند giving the file name.\n\
"), DEFAULT_عدد);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --بایتها=[-]رقم_2\n\
         print the اولی رقم_2 بایتها از each file;\n\
         with the leading '-', print all but the آخرین رقم_2 بایتها از each file\n\
"));
      oprintf (_("\
  -n, --سطرها=[-]رقم_2\n\
         print the اولی رقم_2 سطرها instead از the اولی %d;\n\
         with the leading '-', print all but the آخرین رقم_2 سطرها از each file\n\
"), DEFAULT_عدد);
      oputs (_("\
  -q, --quiet, --silent\n\
         never print سرآیندها giving file نامها\n\
"));
      oputs (_("\
  -v, --verbose\n\
         always print سرآیندها giving file نامها\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
رقم_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on لكل T, P, E, Z, Y, R, Q.\n\
دودویی_2 prefixes can be استفاده_شده, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

ساكن فراغ
diagnose_read_failure (حرف ثابت *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

ساكن فراغ
diagnose_copy_fd_failure (تعداد Copy_fd_وضعیت err, حرف ثابت *filename)
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
write_سرآیند (حرف ثابت *filename)
{
  ساكن bool اولی_file = true;

  چاپ_کن ("%s==> %s <==\n", (اولی_file ? "" : "\n"), quotef (filename));
  اولی_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ساكن فراغ
xwrite_stdout (حرف ثابت *میانگیر, size_t n_بایتها)
{
  إذا (n_بایتها > 0 && fwrite (میانگیر, 1, n_بایتها, stdout) < n_بایتها)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

ساكن تعداد Copy_fd_وضعیت
copy_fd (صحيح src_fd, uintmax_t n_بایتها)
{
  حرف buf[BUFSIZ];

  /* Copy the file contents.  */
  طالما (0 < n_بایتها)
    {
      idx_t n_مقصد_read = MIN (n_بایتها, حجم buf);
      ssize_t n_read = اقرأ (src_fd, buf, n_مقصد_read);
      إذا (n_read < 0)
        أرجع COPY_FD_READ_ERROR;

      n_بایتها -= n_read;

      إذا (n_read == 0 && n_بایتها != 0)
        أرجع COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  أرجع COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
ساكن فراغ
elseek_diagnostic (off_t جابه_جایی, صحيح whence, حرف ثابت *filename)
{
  intmax_t off = جابه_جایی;
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
elseek (صحيح fd, off_t جابه_جایی, صحيح whence, حرف ثابت *filename)
{
  off_t new_جابه_جایی = lseek (fd, جابه_جایی, whence);

  إذا (new_جابه_جایی < 0)
    elseek_diagnostic (جابه_جایی, whence, filename);

  أرجع new_جابه_جایی;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
ساكن bool
elide_دم_بایتها_pipe (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       off_t current_pos)
{
  uintmax_t desired_pos = current_pos;
  bool ok = true;

#اگر_تعریف_نشده سر_دم_PIPE_READ_BUFSIZE
# define سر_دم_PIPE_READ_BUFSIZE BUFSIZ
#پایان_اگر
#تعریف_کن READ_BUFSIZE سر_دم_PIPE_READ_BUFSIZE

  /* If we're eliding no more than this many bytes, then it's ok to allocate
     more memory in order to use a more time-efficient algorithm.
     FIXME: use a fraction of available memory instead, as in sort.
     FIXME: is this even worthwhile?  */
#اگر_تعریف_نشده سر_دم_PIPE_BYTECOUNT_آستانه
# define سر_دم_PIPE_BYTECOUNT_آستانه 1024 * 1024
#پایان_اگر

#إذا سر_دم_PIPE_BYTECOUNT_آستانه < 2 * READ_BUFSIZE
# error "HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD must be at least 2 * READ_BUFSIZE"
#پایان_اگر

  /* Two cases to consider...
     1) n_elide is small enough that we can afford to double-buffer:
        allocate 2 * (READ_BUFSIZE + n_elide) bytes
     2) n_elide is too big for that, so we allocate only
        (READ_BUFSIZE + n_elide) bytes

     FIXME: profile, to see if double-buffering is worthwhile

     CAUTION: do not fail (out of memory) when asked to elide
     a ridiculous amount, but when given only a small input.  */

  static_assert (READ_BUFSIZE <= IDX_MAX);
  static_assert (سر_دم_PIPE_BYTECOUNT_آستانه <= IDX_MAX - READ_BUFSIZE);
  إذا (n_elide <= سر_دم_PIPE_BYTECOUNT_آستانه)
    {
      idx_t درون_elide = n_elide;
      bool اولی = true;
      bool eof = false;
      idx_t n_مقصد_read = READ_BUFSIZE + n_elide;
      حرف *b[2];
      b[0] = xnmalloc (2, n_مقصد_read);
      b[1] = b[0] + n_مقصد_read;

      لكل (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = پر_read (fd, b[i], n_مقصد_read);
          idx_t delta = 0;
          إذا (n_read < n_مقصد_read)
            {
              إذا (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  اكسر;
                }

              /* reached EOF */
              إذا (n_read <= درون_elide)
                {
                  إذا (اولی)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  وإلا
                    {
                      delta = درون_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          إذا (! اولی)
            {
              desired_pos += درون_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, درون_elide - delta);
            }
          اولی = false;

          إذا (درون_elide < n_read)
            {
              desired_pos += n_read - درون_elide;
              xwrite_stdout (b[i], n_read - درون_elide);
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
      idx_t i, i_بعدی;
      لكل (i = 0, i_بعدی = 1; !eof; i = i_بعدی, i_بعدی = (i_بعدی + 1) % n_bufs)
        {
          إذا (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         حجم *b);

          إذا (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = پر_read (fd, b[i], READ_BUFSIZE);
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
              xwrite_stdout (b[i_بعدی], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      إذا (buffered_enough)
        {
          idx_t n_بایتها_چپ_درون_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          إذا (rem < n_بایتها_چپ_درون_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          وإلا
            {
              xwrite_stdout (b[i] + n_read, n_بایتها_چپ_درون_b_i);
              xwrite_stdout (b[i_بعدی], rem - n_بایتها_چپ_درون_b_i);
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
          xwrite_stdout (b[i_بعدی], x);
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
elide_دم_بایتها_file (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       بنية stat ثابت *st, off_t current_pos)
{
  off_t اندازه = st->st_اندازه;
  إذا (current_pos < 0 || اندازه <= STP_BLKSIZE (st))
    أرجع elide_دم_بایتها_pipe (filename, fd, n_elide, current_pos);
  وإلا
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = اندازه - current_pos;
      off_t بایتها_مانده = diff < 0 ? 0 : diff;

      إذا (بایتها_مانده <= n_elide)
        أرجع true;

      تعداد Copy_fd_وضعیت err = copy_fd (fd, بایتها_مانده - n_elide);
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
elide_دم_سطرها_pipe (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       off_t current_pos)
{
  بنية linebuffer
  {
    حرف میانگیر[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    بنية linebuffer *بعدی;
  };
  off_t desired_pos = current_pos;
  تعريفنوع بنية linebuffer LBUFFER;
  LBUFFER *اولی, *آخرین, *tmp;
  size_t مجموع_سطرها = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  اولی = آخرین = xmalloc (حجم (LBUFFER));
  اولی->nbytes = اولی->nlines = 0;
  اولی->بعدی = عدم;
  tmp = xmalloc (حجم (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  طالما (true)
    {
      n_read = اقرأ (fd, tmp->میانگیر, BUFSIZ);
      إذا (n_read <= 0)
        اكسر;

      إذا (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->میانگیر, n_read);
          استمر;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->بعدی = عدم;

      /* Count the number of newlines just read.  */
      {
        حرف *میانگیر_انتها = tmp->میانگیر + n_read;
        *میانگیر_انتها = سطر_انتها;
        حرف ثابت *p = tmp->میانگیر;
        طالما ((p = rawmemchr (p, سطر_انتها)) < میانگیر_انتها)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      مجموع_سطرها += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      إذا (tmp->nbytes + آخرین->nbytes < BUFSIZ)
        {
          کپی_حافظه (&آخرین->میانگیر[آخرین->nbytes], tmp->میانگیر, tmp->nbytes);
          آخرین->nbytes += tmp->nbytes;
          آخرین->nlines += tmp->nlines;
        }
      وإلا
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          آخرین = آخرین->بعدی = tmp;
          إذا (n_elide < مجموع_سطرها - اولی->nlines)
            {
              desired_pos += اولی->nbytes;
              xwrite_stdout (اولی->میانگیر, اولی->nbytes);
              tmp = اولی;
              مجموع_سطرها -= اولی->nlines;
              اولی = اولی->بعدی;
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
  إذا (آخرین->nbytes && آخرین->میانگیر[آخرین->nbytes - 1] != سطر_انتها)
    {
      ++آخرین->nlines;
      ++مجموع_سطرها;
    }

  لكل (tmp = اولی; n_elide < مجموع_سطرها - tmp->nlines; tmp = tmp->بعدی)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->میانگیر, tmp->nbytes);
      مجموع_سطرها -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  إذا (n_elide < مجموع_سطرها)
    {
      size_t n = مجموع_سطرها - n_elide;
      حرف ثابت *میانگیر_انتها = tmp->میانگیر + tmp->nbytes;
      حرف ثابت *p = tmp->میانگیر;
      طالما (n && (p = memchr (p, سطر_انتها, میانگیر_انتها - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->میانگیر;
      xwrite_stdout (tmp->میانگیر, p - tmp->میانگیر);
    }

free_lbuffers:
  طالما (اولی)
    {
      tmp = اولی->بعدی;
      حرر (اولی);
      اولی = tmp;
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
elide_دم_سطرها_seekable (حرف ثابت *pretty_filename, صحيح fd,
                           uintmax_t n_سطرها,
                           off_t شروع_pos, off_t اندازه)
{
  حرف میانگیر[BUFSIZ];
  ssize_t بایتها_read;
  off_t pos = اندازه;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  بایتها_read = (pos - شروع_pos) % BUFSIZ;
  إذا (بایتها_read == 0)
    بایتها_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= بایتها_read;
  إذا (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    أرجع false;
  بایتها_read = اقرأ (fd, میانگیر, بایتها_read);
  إذا (بایتها_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      أرجع false;
    }

  /* n_lines == 0 case needs special treatment. */
  ثابت bool all_سطرها = !n_سطرها;

  /* Count the incomplete line on files that don't end with a newline.  */
  إذا (n_سطرها && بایتها_read && میانگیر[بایتها_read - 1] != سطر_انتها)
    --n_سطرها;

  طالما (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = بایتها_read;
      طالما (n)
        {
          إذا (all_سطرها)
            n -= 1;
          وإلا
            {
              حرف ثابت *nl;
              nl = memrchr (میانگیر, سطر_انتها, n);
              إذا (nl == عدم)
                اكسر;
              n = nl - میانگیر;
            }
          إذا (n_سطرها-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              إذا (شروع_pos < pos)
                {
                  تعداد Copy_fd_وضعیت err;
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
              xwrite_stdout (میانگیر, n + 1);

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

      بایتها_read = اقرأ (fd, میانگیر, BUFSIZ);
      إذا (بایتها_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          أرجع false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      إذا (بایتها_read == 0)
        أرجع true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

ساكن bool
elide_دم_سطرها_file (حرف ثابت *filename, صحيح fd, uintmax_t n_elide,
                       بنية stat ثابت *st, off_t current_pos)
{
  off_t اندازه = st->st_اندازه;
  إذا (current_pos < 0 || اندازه <= STP_BLKSIZE (st))
    أرجع elide_دم_سطرها_pipe (filename, fd, n_elide, current_pos);
  وإلا
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      أرجع (اندازه <= current_pos
              || elide_دم_سطرها_seekable (filename, fd, n_elide,
                                            current_pos, اندازه));
    }
}

ساكن bool
سر_بایتها (حرف ثابت *filename, صحيح fd, uintmax_t بایتها_مقصد_write)
{
  إذا (copy_fd (fd, بایتها_مقصد_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      أرجع false;
    }
  أرجع true;
}

ساكن bool
سر_سطرها (حرف ثابت *filename, صحيح fd, uintmax_t سطرها_مقصد_write)
{
  حرف میانگیر[BUFSIZ];

  طالما (سطرها_مقصد_write)
    {
      ssize_t بایتها_read = اقرأ (fd, میانگیر, BUFSIZ);
      idx_t بایتها_مقصد_write = 0;

      إذا (بایتها_read < 0)
        {
          diagnose_read_failure (filename);
          أرجع false;
        }
      إذا (بایتها_read == 0)
        اكسر;
      طالما (بایتها_مقصد_write < بایتها_read)
        إذا (میانگیر[بایتها_مقصد_write++] == سطر_انتها && --سطرها_مقصد_write == 0)
          {
            off_t n_بایتها_past_EOL = بایتها_read - بایتها_مقصد_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            إذا (lseek (fd, -n_بایتها_past_EOL, SEEK_CUR) < 0)
              {
                بنية stat st;
                إذا (fstat (fd, &st) != 0 || S_ISREG (st.st_نما))
                  elseek_diagnostic (-n_بایتها_past_EOL, SEEK_CUR, filename);
              }
            اكسر;
          }
      xwrite_stdout (میانگیر, بایتها_مقصد_write);
    }
  أرجع true;
}

ساكن bool
سر (حرف ثابت *filename, صحيح fd, uintmax_t n_units, bool شمار_سطرها,
      bool elide_from_انتها)
{
  إذا (print_سرآیندها)
    write_سرآیند (filename);

  إذا (elide_from_انتها)
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
      إذا (! presume_درون_داد_pipe && S_ISREG (st.st_نما))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          إذا (current_pos < 0)
            أرجع false;
        }
      إذا (شمار_سطرها)
        أرجع elide_دم_سطرها_file (filename, fd, n_units, &st, current_pos);
      وإلا
        أرجع elide_دم_بایتها_file (filename, fd, n_units, &st, current_pos);
    }
  إذا (شمار_سطرها)
    أرجع سر_سطرها (filename, fd, n_units);
  وإلا
    أرجع سر_بایتها (filename, fd, n_units);
}

ساكن bool
سر_file (حرف ثابت *filename, uintmax_t n_units, bool شمار_سطرها,
           bool elide_from_انتها)
{
  صحيح fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  إذا (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_دودویی_نما (STDIN_FILENO, O_دودویی);
    }
  وإلا
    {
      fd = open (filename, O_RDONLY | O_دودویی);
      إذا (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          أرجع false;
        }
    }

  ok = سر (filename, fd, n_units, شمار_سطرها, elide_from_انتها);
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
string_مقصد_integer (bool شمار_سطرها, حرف ثابت *n_string)
{
  أرجع xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (شمار_سطرها
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  تعداد سرآیند_نما سرآیند_نما = مضرب_files;
  bool ok = true;
  صحيح c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_عدد;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool شمار_سطرها = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_انتها = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ساكن حرف ثابت *ثابت default_file_list[] = {"-", عدم};
  حرف ثابت *ثابت *file_list;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  إذا (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      حرف *a = argv[1];
      حرف *n_string = ++a;
      حرف *انتها_n_string;
      حرف multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      افعل ++a;
      طالما (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      انتها_n_string = a;

      /* Parse any appended option letters. */
      لكل (; *a; a++)
        {
          بدل (*a)
            {
            حالة 'c':
              شمار_سطرها = false;
              multiplier_char = 0;
              اكسر;

            حالة 'b':
            حالة 'k':
            حالة 'm':
              شمار_سطرها = false;
              multiplier_char = *a;
              اكسر;

            حالة 'l':
              شمار_سطرها = true;
              اكسر;

            حالة 'q':
              سرآیند_نما = never;
              اكسر;

            حالة 'v':
              سرآیند_نما = always;
              اكسر;

            حالة 'z':
              سطر_انتها = '\0';
              اكسر;

            افتراضي:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *انتها_n_string = multiplier_char;
      إذا (multiplier_char)
        *(++انتها_n_string) = 0;

      n_units = string_مقصد_integer (شمار_سطرها, n_string);

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
        حالة PRESUME_درون_داد_PIPE_OPTION:
          presume_درون_داد_pipe = true;
          اكسر;

        حالة 'c':
          شمار_سطرها = false;
          elide_from_انتها = (*optarg == '-');
          إذا (elide_from_انتها)
            ++optarg;
          n_units = string_مقصد_integer (شمار_سطرها, optarg);
          اكسر;

        حالة 'n':
          شمار_سطرها = true;
          elide_from_انتها = (*optarg == '-');
          إذا (elide_from_انتها)
            ++optarg;
          n_units = string_مقصد_integer (شمار_سطرها, optarg);
          اكسر;

        حالة 'q':
          سرآیند_نما = never;
          اكسر;

        حالة 'v':
          سرآیند_نما = always;
          اكسر;

        حالة 'z':
          سطر_انتها = '\0';
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          إذا (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  إذا (سرآیند_نما == always
      || (سرآیند_نما == مضرب_files && optind < argc - 1))
    print_سرآیندها = true;

  file_list = (optind < argc
               ? (حرف ثابت *ثابت *) &argv[optind]
               : default_file_list);

  xset_دودویی_نما (STDOUT_FILENO, O_دودویی);

  لكل (size_t i = 0; file_list[i]; ++i)
    ok &= سر_file (file_list[i], n_units, شمار_سطرها, elide_from_انتها);

  إذا (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
