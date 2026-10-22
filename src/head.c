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

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"

#شامل "assure.h"
#شامل "c-ctype.h"
#شامل "full-read.h"
#شامل "stat-size.h"
#شامل "xbinary-io.h"
#شامل "xdectoint.h"

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
ساکن bool presume_داخلہ_pipe;

/* If true, print filename headers. */
ساکن bool print_سرخیاں;

/* Character to split lines by. */
ساکن حرف سطر_انتہا = '\n';

/* When to print the filename banners. */
شمار سرخی_منوال
{
  مضرب_files, always, never
};

/* Have we ever read standard input?  */
ساکن bool have_read_stdin;

شمار Copy_fd_حالت
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  PRESUME_داخلہ_PIPE_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت long_options[] =
{
  {"bytes", required_دلیل, تہی, 'c'},
  {"lines", required_دلیل, تہی, 'n'},
  {"-presume-input-pipe", no_دلیل, تہی,
   PRESUME_داخلہ_PIPE_OPTION}, /* do not document */
  {"quiet", no_دلیل, تہی, 'q'},
  {"silent", no_دلیل, تہی, 'q'},
  {"verbose", no_دلیل, تہی, 'v'},
  {"zero-terminated", no_دلیل, تہی, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      لکھو (_("\
Print the پہلا %d سطریں کا each FILE مقصد standard output.\n\
With مزید than one FILE, precede each with a سرخی giving the file name.\n\
"), DEFAULT_نمبر);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --بائٹیں=[-]عدد_2\n\
         print the پہلا عدد_2 بائٹیں کا each file;\n\
         with the leading '-', print all but the آخری عدد_2 بائٹیں کا each file\n\
"));
      oprintf (_("\
  -n, --سطریں=[-]عدد_2\n\
         print the پہلا عدد_2 سطریں instead کا the پہلا %d;\n\
         with the leading '-', print all but the آخری عدد_2 سطریں کا each file\n\
"), DEFAULT_نمبر);
      oputs (_("\
  -q, --quiet, --silent\n\
         never print سرخیاں giving file نام\n\
"));
      oputs (_("\
  -v, --verbose\n\
         always print سرخیاں giving file نام\n\
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
GB 1000*1000*1000, G 1024*1024*1024, and so on برائے T, P, E, Z, Y, R, Q.\n\
ثنائی_2 prefixes can be مستعمل, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

ساکن خالی
diagnose_read_failure (حرف ثابت *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

ساکن خالی
diagnose_copy_fd_failure (شمار Copy_fd_حالت err, حرف ثابت *filename)
{
  انتخاب (err)
    {
    حالت COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      توڑو;
    حالت COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      توڑو;
    حالت COPY_FD_OK: طےشدہ:
      affirm (false);
    }
}

ساکن خالی
write_سرخی (حرف ثابت *filename)
{
  ساکن bool پہلا_file = true;

  لکھو ("%s==> %s <==\n", (پہلا_file ? "" : "\n"), quotef (filename));
  پہلا_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ساکن خالی
xwrite_stdout (حرف ثابت *بفر, size_t n_بائٹیں)
{
  اگر (n_بائٹیں > 0 && fwrite (بفر, 1, n_بائٹیں, stdout) < n_بائٹیں)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

ساکن شمار Copy_fd_حالت
copy_fd (صحیح src_fd, uintmax_t n_بائٹیں)
{
  حرف buf[BUFSIZ];

  /* Copy the file contents.  */
  جب (0 < n_بائٹیں)
    {
      idx_t n_مقصد_read = MIN (n_بائٹیں, حجم buf);
      ssize_t n_read = اقرأ (src_fd, buf, n_مقصد_read);
      اگر (n_read < 0)
        واپس COPY_FD_READ_ERROR;

      n_بائٹیں -= n_read;

      اگر (n_read == 0 && n_بائٹیں != 0)
        واپس COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  واپس COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
ساکن خالی
elseek_diagnostic (off_t انحراف, صحیح whence, حرف ثابت *filename)
{
  intmax_t off = انحراف;
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

ساکن off_t
elseek (صحیح fd, off_t انحراف, صحیح whence, حرف ثابت *filename)
{
  off_t new_انحراف = lseek (fd, انحراف, whence);

  اگر (new_انحراف < 0)
    elseek_diagnostic (انحراف, whence, filename);

  واپس new_انحراف;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
ساکن bool
elide_دم_بائٹیں_pipe (حرف ثابت *filename, صحیح fd, uintmax_t n_elide,
                       off_t current_pos)
{
  uintmax_t desired_pos = current_pos;
  bool ok = true;

#اگر_غیر_تعریف_شدہ سر_دم_PIPE_READ_BUFSIZE
# define سر_دم_PIPE_READ_BUFSIZE BUFSIZ
#اگر_ختم
#تعریف_کرو READ_BUFSIZE سر_دم_PIPE_READ_BUFSIZE

  /* If we're eliding no more than this many bytes, then it's ok to allocate
     more memory in order to use a more time-efficient algorithm.
     FIXME: use a fraction of available memory instead, as in sort.
     FIXME: is this even worthwhile?  */
#اگر_غیر_تعریف_شدہ سر_دم_PIPE_BYTECOUNT_حد
# define سر_دم_PIPE_BYTECOUNT_حد 1024 * 1024
#اگر_ختم

#اگر سر_دم_PIPE_BYTECOUNT_حد < 2 * READ_BUFSIZE
# error "HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD must be at least 2 * READ_BUFSIZE"
#اگر_ختم

  /* Two cases to consider...
     1) n_elide is small enough that we can afford to double-buffer:
        allocate 2 * (READ_BUFSIZE + n_elide) bytes
     2) n_elide is too big for that, so we allocate only
        (READ_BUFSIZE + n_elide) bytes

     FIXME: profile, to see if double-buffering is worthwhile

     CAUTION: do not fail (out of memory) when asked to elide
     a ridiculous amount, but when given only a small input.  */

  static_assert (READ_BUFSIZE <= IDX_MAX);
  static_assert (سر_دم_PIPE_BYTECOUNT_حد <= IDX_MAX - READ_BUFSIZE);
  اگر (n_elide <= سر_دم_PIPE_BYTECOUNT_حد)
    {
      idx_t اندر_elide = n_elide;
      bool پہلا = true;
      bool eof = false;
      idx_t n_مقصد_read = READ_BUFSIZE + n_elide;
      حرف *b[2];
      b[0] = xnmalloc (2, n_مقصد_read);
      b[1] = b[0] + n_مقصد_read;

      برائے (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = بھرا_read (fd, b[i], n_مقصد_read);
          idx_t delta = 0;
          اگر (n_read < n_مقصد_read)
            {
              اگر (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  توڑو;
                }

              /* reached EOF */
              اگر (n_read <= اندر_elide)
                {
                  اگر (پہلا)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  ورنہ
                    {
                      delta = اندر_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          اگر (! پہلا)
            {
              desired_pos += اندر_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, اندر_elide - delta);
            }
          پہلا = false;

          اگر (اندر_elide < n_read)
            {
              desired_pos += n_read - اندر_elide;
              xwrite_stdout (b[i], n_read - اندر_elide);
            }
        }

      آزاد (b[0]);
    }
  ورنہ
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      حرف **b = تہی;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_اگلا;
      برائے (i = 0, i_اگلا = 1; !eof; i = i_اگلا, i_اگلا = (i_اگلا + 1) % n_bufs)
        {
          اگر (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         حجم *b);

          اگر (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = بھرا_read (fd, b[i], READ_BUFSIZE);
          اگر (n_read < READ_BUFSIZE)
            {
              اگر (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  جاؤ free_mem;
                }
              eof = true;
            }

          اگر (i + 1 == n_bufs)
            buffered_enough = true;

          اگر (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_اگلا], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      اگر (buffered_enough)
        {
          idx_t n_بائٹیں_بایاں_اندر_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          اگر (rem < n_بائٹیں_بایاں_اندر_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          ورنہ
            {
              xwrite_stdout (b[i] + n_read, n_بائٹیں_بایاں_اندر_b_i);
              xwrite_stdout (b[i_اگلا], rem - n_بائٹیں_بایاں_اندر_b_i);
            }
        }
      ورنہ اگر (i + 1 == n_bufs)
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
      برائے (i = 0; i < n_alloc; i++)
        آزاد (b[i]);
      آزاد (b);
    }

  اگر (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  واپس ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

ساکن bool
elide_دم_بائٹیں_file (حرف ثابت *filename, صحیح fd, uintmax_t n_elide,
                       ساخت stat ثابت *st, off_t current_pos)
{
  off_t سائز = st->st_سائز;
  اگر (current_pos < 0 || سائز <= STP_BLKSIZE (st))
    واپس elide_دم_بائٹیں_pipe (filename, fd, n_elide, current_pos);
  ورنہ
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = سائز - current_pos;
      off_t بائٹیں_بقایا = diff < 0 ? 0 : diff;

      اگر (بائٹیں_بقایا <= n_elide)
        واپس true;

      شمار Copy_fd_حالت err = copy_fd (fd, بائٹیں_بقایا - n_elide);
      اگر (err == COPY_FD_OK)
        واپس true;

      diagnose_copy_fd_failure (err, filename);
      واپس false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

ساکن bool
elide_دم_سطریں_pipe (حرف ثابت *filename, صحیح fd, uintmax_t n_elide,
                       off_t current_pos)
{
  ساخت linebuffer
  {
    حرف بفر[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    ساخت linebuffer *اگلا;
  };
  off_t desired_pos = current_pos;
  قسمتعریف ساخت linebuffer LBUFFER;
  LBUFFER *پہلا, *آخری, *tmp;
  size_t کل_سطریں = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  پہلا = آخری = xmalloc (حجم (LBUFFER));
  پہلا->nbytes = پہلا->nlines = 0;
  پہلا->اگلا = تہی;
  tmp = xmalloc (حجم (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  جب (true)
    {
      n_read = اقرأ (fd, tmp->بفر, BUFSIZ);
      اگر (n_read <= 0)
        توڑو;

      اگر (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->بفر, n_read);
          جاری;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->اگلا = تہی;

      /* Count the number of newlines just read.  */
      {
        حرف *بفر_انتہا = tmp->بفر + n_read;
        *بفر_انتہا = سطر_انتہا;
        حرف ثابت *p = tmp->بفر;
        جب ((p = rawmemchr (p, سطر_انتہا)) < بفر_انتہا)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      کل_سطریں += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      اگر (tmp->nbytes + آخری->nbytes < BUFSIZ)
        {
          حافظہ_نقل (&آخری->بفر[آخری->nbytes], tmp->بفر, tmp->nbytes);
          آخری->nbytes += tmp->nbytes;
          آخری->nlines += tmp->nlines;
        }
      ورنہ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          آخری = آخری->اگلا = tmp;
          اگر (n_elide < کل_سطریں - پہلا->nlines)
            {
              desired_pos += پہلا->nbytes;
              xwrite_stdout (پہلا->بفر, پہلا->nbytes);
              tmp = پہلا;
              کل_سطریں -= پہلا->nlines;
              پہلا = پہلا->اگلا;
            }
          ورنہ
            tmp = xmalloc (حجم (LBUFFER));
        }
    }

  آزاد (tmp);

  اگر (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      جاؤ free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  اگر (آخری->nbytes && آخری->بفر[آخری->nbytes - 1] != سطر_انتہا)
    {
      ++آخری->nlines;
      ++کل_سطریں;
    }

  برائے (tmp = پہلا; n_elide < کل_سطریں - tmp->nlines; tmp = tmp->اگلا)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->بفر, tmp->nbytes);
      کل_سطریں -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  اگر (n_elide < کل_سطریں)
    {
      size_t n = کل_سطریں - n_elide;
      حرف ثابت *بفر_انتہا = tmp->بفر + tmp->nbytes;
      حرف ثابت *p = tmp->بفر;
      جب (n && (p = memchr (p, سطر_انتہا, بفر_انتہا - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->بفر;
      xwrite_stdout (tmp->بفر, p - tmp->بفر);
    }

free_lbuffers:
  جب (پہلا)
    {
      tmp = پہلا->اگلا;
      آزاد (پہلا);
      پہلا = tmp;
    }

  اگر (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  واپس ok;
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
ساکن bool
elide_دم_سطریں_seekable (حرف ثابت *pretty_filename, صحیح fd,
                           uintmax_t n_سطریں,
                           off_t آغاز_pos, off_t سائز)
{
  حرف بفر[BUFSIZ];
  ssize_t بائٹیں_read;
  off_t pos = سائز;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  بائٹیں_read = (pos - آغاز_pos) % BUFSIZ;
  اگر (بائٹیں_read == 0)
    بائٹیں_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= بائٹیں_read;
  اگر (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    واپس false;
  بائٹیں_read = اقرأ (fd, بفر, بائٹیں_read);
  اگر (بائٹیں_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      واپس false;
    }

  /* n_lines == 0 case needs special treatment. */
  ثابت bool all_سطریں = !n_سطریں;

  /* Count the incomplete line on files that don't end with a newline.  */
  اگر (n_سطریں && بائٹیں_read && بفر[بائٹیں_read - 1] != سطر_انتہا)
    --n_سطریں;

  جب (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = بائٹیں_read;
      جب (n)
        {
          اگر (all_سطریں)
            n -= 1;
          ورنہ
            {
              حرف ثابت *nl;
              nl = memrchr (بفر, سطر_انتہا, n);
              اگر (nl == تہی)
                توڑو;
              n = nl - بفر;
            }
          اگر (n_سطریں-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              اگر (آغاز_pos < pos)
                {
                  شمار Copy_fd_حالت err;
                  اگر (elseek (fd, آغاز_pos, SEEK_SET, pretty_filename) < 0)
                    واپس false;

                  err = copy_fd (fd, pos - آغاز_pos);
                  اگر (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      واپس false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (بفر, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              واپس 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      اگر (pos == آغاز_pos)
        {
          /* Not enough lines in the file.  */
          واپس true;
        }
      pos -= BUFSIZ;
      اگر (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        واپس false;

      بائٹیں_read = اقرأ (fd, بفر, BUFSIZ);
      اگر (بائٹیں_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          واپس false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      اگر (بائٹیں_read == 0)
        واپس true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

ساکن bool
elide_دم_سطریں_file (حرف ثابت *filename, صحیح fd, uintmax_t n_elide,
                       ساخت stat ثابت *st, off_t current_pos)
{
  off_t سائز = st->st_سائز;
  اگر (current_pos < 0 || سائز <= STP_BLKSIZE (st))
    واپس elide_دم_سطریں_pipe (filename, fd, n_elide, current_pos);
  ورنہ
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      واپس (سائز <= current_pos
              || elide_دم_سطریں_seekable (filename, fd, n_elide,
                                            current_pos, سائز));
    }
}

ساکن bool
سر_بائٹیں (حرف ثابت *filename, صحیح fd, uintmax_t بائٹیں_مقصد_write)
{
  اگر (copy_fd (fd, بائٹیں_مقصد_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      واپس false;
    }
  واپس true;
}

ساکن bool
سر_سطریں (حرف ثابت *filename, صحیح fd, uintmax_t سطریں_مقصد_write)
{
  حرف بفر[BUFSIZ];

  جب (سطریں_مقصد_write)
    {
      ssize_t بائٹیں_read = اقرأ (fd, بفر, BUFSIZ);
      idx_t بائٹیں_مقصد_write = 0;

      اگر (بائٹیں_read < 0)
        {
          diagnose_read_failure (filename);
          واپس false;
        }
      اگر (بائٹیں_read == 0)
        توڑو;
      جب (بائٹیں_مقصد_write < بائٹیں_read)
        اگر (بفر[بائٹیں_مقصد_write++] == سطر_انتہا && --سطریں_مقصد_write == 0)
          {
            off_t n_بائٹیں_past_EOL = بائٹیں_read - بائٹیں_مقصد_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            اگر (lseek (fd, -n_بائٹیں_past_EOL, SEEK_CUR) < 0)
              {
                ساخت stat st;
                اگر (fstat (fd, &st) != 0 || S_ISREG (st.st_منوال))
                  elseek_diagnostic (-n_بائٹیں_past_EOL, SEEK_CUR, filename);
              }
            توڑو;
          }
      xwrite_stdout (بفر, بائٹیں_مقصد_write);
    }
  واپس true;
}

ساکن bool
سر (حرف ثابت *filename, صحیح fd, uintmax_t n_units, bool گنتی_سطریں,
      bool elide_from_انتہا)
{
  اگر (print_سرخیاں)
    write_سرخی (filename);

  اگر (elide_from_انتہا)
    {
      /* Optimize for "infinite" elisions.  */
      اگر (n_units == UINTMAX_MAX)
        واپس true;

      off_t current_pos = -1;
      ساخت stat st;
      اگر (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          واپس false;
        }
      اگر (! presume_داخلہ_pipe && S_ISREG (st.st_منوال))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          اگر (current_pos < 0)
            واپس false;
        }
      اگر (گنتی_سطریں)
        واپس elide_دم_سطریں_file (filename, fd, n_units, &st, current_pos);
      ورنہ
        واپس elide_دم_بائٹیں_file (filename, fd, n_units, &st, current_pos);
    }
  اگر (گنتی_سطریں)
    واپس سر_سطریں (filename, fd, n_units);
  ورنہ
    واپس سر_بائٹیں (filename, fd, n_units);
}

ساکن bool
سر_file (حرف ثابت *filename, uintmax_t n_units, bool گنتی_سطریں,
           bool elide_from_انتہا)
{
  صحیح fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  اگر (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
    }
  ورنہ
    {
      fd = open (filename, O_RDONLY | O_ثنائی);
      اگر (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          واپس false;
        }
    }

  ok = سر (filename, fd, n_units, گنتی_سطریں, elide_from_انتہا);
  اگر (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      واپس false;
    }
  واپس ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

ساکن uintmax_t
string_مقصد_integer (bool گنتی_سطریں, حرف ثابت *n_string)
{
  واپس xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (گنتی_سطریں
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  شمار سرخی_منوال سرخی_منوال = مضرب_files;
  bool ok = true;
  صحیح c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_نمبر;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool گنتی_سطریں = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_انتہا = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ساکن حرف ثابت *ثابت default_file_list[] = {"-", تہی};
  حرف ثابت *ثابت *file_list;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  اگر (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      حرف *a = argv[1];
      حرف *n_string = ++a;
      حرف *انتہا_n_string;
      حرف multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      کرو ++a;
      جب (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      انتہا_n_string = a;

      /* Parse any appended option letters. */
      برائے (; *a; a++)
        {
          انتخاب (*a)
            {
            حالت 'c':
              گنتی_سطریں = false;
              multiplier_char = 0;
              توڑو;

            حالت 'b':
            حالت 'k':
            حالت 'm':
              گنتی_سطریں = false;
              multiplier_char = *a;
              توڑو;

            حالت 'l':
              گنتی_سطریں = true;
              توڑو;

            حالت 'q':
              سرخی_منوال = never;
              توڑو;

            حالت 'v':
              سرخی_منوال = always;
              توڑو;

            حالت 'z':
              سطر_انتہا = '\0';
              توڑو;

            طےشدہ:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *انتہا_n_string = multiplier_char;
      اگر (multiplier_char)
        *(++انتہا_n_string) = 0;

      n_units = string_مقصد_integer (گنتی_سطریں, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  جب ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, تہی))
         != -1)
    {
      انتخاب (c)
        {
        حالت PRESUME_داخلہ_PIPE_OPTION:
          presume_داخلہ_pipe = true;
          توڑو;

        حالت 'c':
          گنتی_سطریں = false;
          elide_from_انتہا = (*optarg == '-');
          اگر (elide_from_انتہا)
            ++optarg;
          n_units = string_مقصد_integer (گنتی_سطریں, optarg);
          توڑو;

        حالت 'n':
          گنتی_سطریں = true;
          elide_from_انتہا = (*optarg == '-');
          اگر (elide_from_انتہا)
            ++optarg;
          n_units = string_مقصد_integer (گنتی_سطریں, optarg);
          توڑو;

        حالت 'q':
          سرخی_منوال = never;
          توڑو;

        حالت 'v':
          سرخی_منوال = always;
          توڑو;

        حالت 'z':
          سطر_انتہا = '\0';
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          اگر (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  اگر (سرخی_منوال == always
      || (سرخی_منوال == مضرب_files && optind < argc - 1))
    print_سرخیاں = true;

  file_list = (optind < argc
               ? (حرف ثابت *ثابت *) &argv[optind]
               : default_file_list);

  xset_ثنائی_منوال (STDOUT_FILENO, O_ثنائی);

  برائے (size_t i = 0; file_list[i]; ++i)
    ok &= سر_file (file_list[i], n_units, گنتی_سطریں, elide_from_انتہا);

  اگر (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
