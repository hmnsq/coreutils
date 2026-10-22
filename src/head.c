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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "full-read.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-size.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "head"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_ਸੰਖਿਆ 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ਸਥਿਰ bool presume_ਨਿਵੇਸ਼_pipe;

/* If true, print filename headers. */
ਸਥਿਰ bool print_ਸਿਰਲੇਖਾਂ;

/* Character to split lines by. */
ਸਥਿਰ ਅੱਖਰ ਸਤਰ_ਸਿਰਾ = '\n';

/* When to print the filename banners. */
ਗਿਣਤੀ ਸਿਰਲੇਖ_ਬਹੁਲਕ
{
  ਗੁਣਜ_files, always, never
};

/* Have we ever read standard input?  */
ਸਥਿਰ bool have_read_stdin;

ਗਿਣਤੀ Copy_fd_ਸਥਿਤੀ
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  PRESUME_ਨਿਵੇਸ਼_PIPE_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"bytes", required_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"lines", required_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"-presume-input-pipe", no_ਦਲੀਲ, ਨਿਰਾ,
   PRESUME_ਨਿਵੇਸ਼_PIPE_OPTION}, /* do not document */
  {"quiet", no_ਦਲੀਲ, ਨਿਰਾ, 'q'},
  {"silent", no_ਦਲੀਲ, ਨਿਰਾ, 'q'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {"zero-terminated", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      ਲਿਖੋ (_("\
Print the ਪਹਿਲਾ %d ਸਤਰਾਂ ਦਾ each FILE ਮੰਜ਼ਿਲ standard output.\n\
With ਹੋਰ than one FILE, precede each with a ਸਿਰਲੇਖ giving the file name.\n\
"), DEFAULT_ਸੰਖਿਆ);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --ਬਾਈਟਾਂ=[-]ਅੰਕ_2\n\
         print the ਪਹਿਲਾ ਅੰਕ_2 ਬਾਈਟਾਂ ਦਾ each file;\n\
         with the leading '-', print all but the ਆਖਰੀ ਅੰਕ_2 ਬਾਈਟਾਂ ਦਾ each file\n\
"));
      oprintf (_("\
  -n, --ਸਤਰਾਂ=[-]ਅੰਕ_2\n\
         print the ਪਹਿਲਾ ਅੰਕ_2 ਸਤਰਾਂ instead ਦਾ the ਪਹਿਲਾ %d;\n\
         with the leading '-', print all but the ਆਖਰੀ ਅੰਕ_2 ਸਤਰਾਂ ਦਾ each file\n\
"), DEFAULT_ਸੰਖਿਆ);
      oputs (_("\
  -q, --quiet, --silent\n\
         never print ਸਿਰਲੇਖਾਂ giving file ਨਾਂ\n\
"));
      oputs (_("\
  -v, --verbose\n\
         always print ਸਿਰਲੇਖਾਂ giving file ਨਾਂ\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ਸਤਰ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ਅੰਕ_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on ਲਈ T, P, E, Z, Y, R, Q.\n\
ਦੁਆਧਾਰੀ_2 prefixes can be ਵਰਤਿਆ, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਸਥਿਰ ਖਾਲੀ
diagnose_read_failure (ਅੱਖਰ ਅਚਲ *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

ਸਥਿਰ ਖਾਲੀ
diagnose_copy_fd_failure (ਗਿਣਤੀ Copy_fd_ਸਥਿਤੀ err, ਅੱਖਰ ਅਚਲ *filename)
{
  ਚੋਣ (err)
    {
    ਹਾਲਤ COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      ਰੋਕੋ;
    ਹਾਲਤ COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      ਰੋਕੋ;
    ਹਾਲਤ COPY_FD_OK: ਮੂਲ:
      affirm (false);
    }
}

ਸਥਿਰ ਖਾਲੀ
write_ਸਿਰਲੇਖ (ਅੱਖਰ ਅਚਲ *filename)
{
  ਸਥਿਰ bool ਪਹਿਲਾ_file = true;

  ਲਿਖੋ ("%s==> %s <==\n", (ਪਹਿਲਾ_file ? "" : "\n"), quotef (filename));
  ਪਹਿਲਾ_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ਸਥਿਰ ਖਾਲੀ
xwrite_stdout (ਅੱਖਰ ਅਚਲ *ਬਫਰ, size_t n_ਬਾਈਟਾਂ)
{
  ਜੇ (n_ਬਾਈਟਾਂ > 0 && fwrite (ਬਫਰ, 1, n_ਬਾਈਟਾਂ, stdout) < n_ਬਾਈਟਾਂ)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

ਸਥਿਰ ਗਿਣਤੀ Copy_fd_ਸਥਿਤੀ
copy_fd (ਪੂਰਨ src_fd, uintmax_t n_ਬਾਈਟਾਂ)
{
  ਅੱਖਰ buf[BUFSIZ];

  /* Copy the file contents.  */
  ਜਦੋਂ (0 < n_ਬਾਈਟਾਂ)
    {
      idx_t n_ਮੰਜ਼ਿਲ_read = MIN (n_ਬਾਈਟਾਂ, ਆਕਾਰ buf);
      ssize_t n_read = ਪਠ (src_fd, buf, n_ਮੰਜ਼ਿਲ_read);
      ਜੇ (n_read < 0)
        ਵਾਪਸ COPY_FD_READ_ERROR;

      n_ਬਾਈਟਾਂ -= n_read;

      ਜੇ (n_read == 0 && n_ਬਾਈਟਾਂ != 0)
        ਵਾਪਸ COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  ਵਾਪਸ COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
ਸਥਿਰ ਖਾਲੀ
elseek_diagnostic (off_t ਵਿੱਥ, ਪੂਰਨ whence, ਅੱਖਰ ਅਚਲ *filename)
{
  intmax_t off = ਵਿੱਥ;
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

ਸਥਿਰ off_t
elseek (ਪੂਰਨ fd, off_t ਵਿੱਥ, ਪੂਰਨ whence, ਅੱਖਰ ਅਚਲ *filename)
{
  off_t new_ਵਿੱਥ = lseek (fd, ਵਿੱਥ, whence);

  ਜੇ (new_ਵਿੱਥ < 0)
    elseek_diagnostic (ਵਿੱਥ, whence, filename);

  ਵਾਪਸ new_ਵਿੱਥ;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
ਸਥਿਰ bool
elide_ਪੂਛ_ਬਾਈਟਾਂ_pipe (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ fd, uintmax_t n_elide,
                       off_t current_pos)
{
  uintmax_t desired_pos = current_pos;
  bool ok = true;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਸਿਰ_ਪੂਛ_PIPE_READ_BUFSIZE
# define ਸਿਰ_ਪੂਛ_PIPE_READ_BUFSIZE BUFSIZ
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ READ_BUFSIZE ਸਿਰ_ਪੂਛ_PIPE_READ_BUFSIZE

  /* If we're eliding no more than this many bytes, then it's ok to allocate
     more memory in order to use a more time-efficient algorithm.
     FIXME: use a fraction of available memory instead, as in sort.
     FIXME: is this even worthwhile?  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਸਿਰ_ਪੂਛ_PIPE_BYTECOUNT_ਹੱਦ
# define ਸਿਰ_ਪੂਛ_PIPE_BYTECOUNT_ਹੱਦ 1024 * 1024
#ਜੇ_ਅੰਤ

#ਜੇ ਸਿਰ_ਪੂਛ_PIPE_BYTECOUNT_ਹੱਦ < 2 * READ_BUFSIZE
# error "HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD must be at least 2 * READ_BUFSIZE"
#ਜੇ_ਅੰਤ

  /* Two cases to consider...
     1) n_elide is small enough that we can afford to double-buffer:
        allocate 2 * (READ_BUFSIZE + n_elide) bytes
     2) n_elide is too big for that, so we allocate only
        (READ_BUFSIZE + n_elide) bytes

     FIXME: profile, to see if double-buffering is worthwhile

     CAUTION: do not fail (out of memory) when asked to elide
     a ridiculous amount, but when given only a small input.  */

  static_assert (READ_BUFSIZE <= IDX_MAX);
  static_assert (ਸਿਰ_ਪੂਛ_PIPE_BYTECOUNT_ਹੱਦ <= IDX_MAX - READ_BUFSIZE);
  ਜੇ (n_elide <= ਸਿਰ_ਪੂਛ_PIPE_BYTECOUNT_ਹੱਦ)
    {
      idx_t ਵਿੱਚ_elide = n_elide;
      bool ਪਹਿਲਾ = true;
      bool eof = false;
      idx_t n_ਮੰਜ਼ਿਲ_read = READ_BUFSIZE + n_elide;
      ਅੱਖਰ *b[2];
      b[0] = xnmalloc (2, n_ਮੰਜ਼ਿਲ_read);
      b[1] = b[0] + n_ਮੰਜ਼ਿਲ_read;

      ਲਈ (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = ਭਰਿਆ_read (fd, b[i], n_ਮੰਜ਼ਿਲ_read);
          idx_t delta = 0;
          ਜੇ (n_read < n_ਮੰਜ਼ਿਲ_read)
            {
              ਜੇ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  ਰੋਕੋ;
                }

              /* reached EOF */
              ਜੇ (n_read <= ਵਿੱਚ_elide)
                {
                  ਜੇ (ਪਹਿਲਾ)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  ਨਹੀਂਤਾਂ
                    {
                      delta = ਵਿੱਚ_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          ਜੇ (! ਪਹਿਲਾ)
            {
              desired_pos += ਵਿੱਚ_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, ਵਿੱਚ_elide - delta);
            }
          ਪਹਿਲਾ = false;

          ਜੇ (ਵਿੱਚ_elide < n_read)
            {
              desired_pos += n_read - ਵਿੱਚ_elide;
              xwrite_stdout (b[i], n_read - ਵਿੱਚ_elide);
            }
        }

      ਮੁਕਤ (b[0]);
    }
  ਨਹੀਂਤਾਂ
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      ਅੱਖਰ **b = ਨਿਰਾ;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_ਅਗਲਾ;
      ਲਈ (i = 0, i_ਅਗਲਾ = 1; !eof; i = i_ਅਗਲਾ, i_ਅਗਲਾ = (i_ਅਗਲਾ + 1) % n_bufs)
        {
          ਜੇ (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         ਆਕਾਰ *b);

          ਜੇ (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = ਭਰਿਆ_read (fd, b[i], READ_BUFSIZE);
          ਜੇ (n_read < READ_BUFSIZE)
            {
              ਜੇ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  ਜਾਓ free_mem;
                }
              eof = true;
            }

          ਜੇ (i + 1 == n_bufs)
            buffered_enough = true;

          ਜੇ (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_ਅਗਲਾ], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      ਜੇ (buffered_enough)
        {
          idx_t n_ਬਾਈਟਾਂ_ਖੱਬਾ_ਵਿੱਚ_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          ਜੇ (rem < n_ਬਾਈਟਾਂ_ਖੱਬਾ_ਵਿੱਚ_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          ਨਹੀਂਤਾਂ
            {
              xwrite_stdout (b[i] + n_read, n_ਬਾਈਟਾਂ_ਖੱਬਾ_ਵਿੱਚ_b_i);
              xwrite_stdout (b[i_ਅਗਲਾ], rem - n_ਬਾਈਟਾਂ_ਖੱਬਾ_ਵਿੱਚ_b_i);
            }
        }
      ਨਹੀਂਤਾਂ ਜੇ (i + 1 == n_bufs)
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
          xwrite_stdout (b[i_ਅਗਲਾ], x);
        }

    free_mem:
      ਲਈ (i = 0; i < n_alloc; i++)
        ਮੁਕਤ (b[i]);
      ਮੁਕਤ (b);
    }

  ਜੇ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  ਵਾਪਸ ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

ਸਥਿਰ bool
elide_ਪੂਛ_ਬਾਈਟਾਂ_file (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ fd, uintmax_t n_elide,
                       ਬਣਤਰ stat ਅਚਲ *st, off_t current_pos)
{
  off_t ਮਾਪ = st->st_ਮਾਪ;
  ਜੇ (current_pos < 0 || ਮਾਪ <= STP_BLKSIZE (st))
    ਵਾਪਸ elide_ਪੂਛ_ਬਾਈਟਾਂ_pipe (filename, fd, n_elide, current_pos);
  ਨਹੀਂਤਾਂ
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = ਮਾਪ - current_pos;
      off_t ਬਾਈਟਾਂ_ਬਚਿਆ = diff < 0 ? 0 : diff;

      ਜੇ (ਬਾਈਟਾਂ_ਬਚਿਆ <= n_elide)
        ਵਾਪਸ true;

      ਗਿਣਤੀ Copy_fd_ਸਥਿਤੀ err = copy_fd (fd, ਬਾਈਟਾਂ_ਬਚਿਆ - n_elide);
      ਜੇ (err == COPY_FD_OK)
        ਵਾਪਸ true;

      diagnose_copy_fd_failure (err, filename);
      ਵਾਪਸ false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

ਸਥਿਰ bool
elide_ਪੂਛ_ਸਤਰਾਂ_pipe (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ fd, uintmax_t n_elide,
                       off_t current_pos)
{
  ਬਣਤਰ linebuffer
  {
    ਅੱਖਰ ਬਫਰ[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    ਬਣਤਰ linebuffer *ਅਗਲਾ;
  };
  off_t desired_pos = current_pos;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ linebuffer LBUFFER;
  LBUFFER *ਪਹਿਲਾ, *ਆਖਰੀ, *tmp;
  size_t ਕੁੱਲ_ਸਤਰਾਂ = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  ਪਹਿਲਾ = ਆਖਰੀ = xmalloc (ਆਕਾਰ (LBUFFER));
  ਪਹਿਲਾ->nbytes = ਪਹਿਲਾ->nlines = 0;
  ਪਹਿਲਾ->ਅਗਲਾ = ਨਿਰਾ;
  tmp = xmalloc (ਆਕਾਰ (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  ਜਦੋਂ (true)
    {
      n_read = ਪਠ (fd, tmp->ਬਫਰ, BUFSIZ);
      ਜੇ (n_read <= 0)
        ਰੋਕੋ;

      ਜੇ (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->ਬਫਰ, n_read);
          ਜਾਰੀ;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->ਅਗਲਾ = ਨਿਰਾ;

      /* Count the number of newlines just read.  */
      {
        ਅੱਖਰ *ਬਫਰ_ਸਿਰਾ = tmp->ਬਫਰ + n_read;
        *ਬਫਰ_ਸਿਰਾ = ਸਤਰ_ਸਿਰਾ;
        ਅੱਖਰ ਅਚਲ *p = tmp->ਬਫਰ;
        ਜਦੋਂ ((p = rawmemchr (p, ਸਤਰ_ਸਿਰਾ)) < ਬਫਰ_ਸਿਰਾ)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      ਕੁੱਲ_ਸਤਰਾਂ += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      ਜੇ (tmp->nbytes + ਆਖਰੀ->nbytes < BUFSIZ)
        {
          ਸਮ੍ਰਿਤੀ_ਨਕਲ (&ਆਖਰੀ->ਬਫਰ[ਆਖਰੀ->nbytes], tmp->ਬਫਰ, tmp->nbytes);
          ਆਖਰੀ->nbytes += tmp->nbytes;
          ਆਖਰੀ->nlines += tmp->nlines;
        }
      ਨਹੀਂਤਾਂ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          ਆਖਰੀ = ਆਖਰੀ->ਅਗਲਾ = tmp;
          ਜੇ (n_elide < ਕੁੱਲ_ਸਤਰਾਂ - ਪਹਿਲਾ->nlines)
            {
              desired_pos += ਪਹਿਲਾ->nbytes;
              xwrite_stdout (ਪਹਿਲਾ->ਬਫਰ, ਪਹਿਲਾ->nbytes);
              tmp = ਪਹਿਲਾ;
              ਕੁੱਲ_ਸਤਰਾਂ -= ਪਹਿਲਾ->nlines;
              ਪਹਿਲਾ = ਪਹਿਲਾ->ਅਗਲਾ;
            }
          ਨਹੀਂਤਾਂ
            tmp = xmalloc (ਆਕਾਰ (LBUFFER));
        }
    }

  ਮੁਕਤ (tmp);

  ਜੇ (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      ਜਾਓ free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  ਜੇ (ਆਖਰੀ->nbytes && ਆਖਰੀ->ਬਫਰ[ਆਖਰੀ->nbytes - 1] != ਸਤਰ_ਸਿਰਾ)
    {
      ++ਆਖਰੀ->nlines;
      ++ਕੁੱਲ_ਸਤਰਾਂ;
    }

  ਲਈ (tmp = ਪਹਿਲਾ; n_elide < ਕੁੱਲ_ਸਤਰਾਂ - tmp->nlines; tmp = tmp->ਅਗਲਾ)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->ਬਫਰ, tmp->nbytes);
      ਕੁੱਲ_ਸਤਰਾਂ -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  ਜੇ (n_elide < ਕੁੱਲ_ਸਤਰਾਂ)
    {
      size_t n = ਕੁੱਲ_ਸਤਰਾਂ - n_elide;
      ਅੱਖਰ ਅਚਲ *ਬਫਰ_ਸਿਰਾ = tmp->ਬਫਰ + tmp->nbytes;
      ਅੱਖਰ ਅਚਲ *p = tmp->ਬਫਰ;
      ਜਦੋਂ (n && (p = memchr (p, ਸਤਰ_ਸਿਰਾ, ਬਫਰ_ਸਿਰਾ - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->ਬਫਰ;
      xwrite_stdout (tmp->ਬਫਰ, p - tmp->ਬਫਰ);
    }

free_lbuffers:
  ਜਦੋਂ (ਪਹਿਲਾ)
    {
      tmp = ਪਹਿਲਾ->ਅਗਲਾ;
      ਮੁਕਤ (ਪਹਿਲਾ);
      ਪਹਿਲਾ = tmp;
    }

  ਜੇ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  ਵਾਪਸ ok;
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
ਸਥਿਰ bool
elide_ਪੂਛ_ਸਤਰਾਂ_seekable (ਅੱਖਰ ਅਚਲ *pretty_filename, ਪੂਰਨ fd,
                           uintmax_t n_ਸਤਰਾਂ,
                           off_t ਸ਼ੁਰੂ_pos, off_t ਮਾਪ)
{
  ਅੱਖਰ ਬਫਰ[BUFSIZ];
  ssize_t ਬਾਈਟਾਂ_read;
  off_t pos = ਮਾਪ;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  ਬਾਈਟਾਂ_read = (pos - ਸ਼ੁਰੂ_pos) % BUFSIZ;
  ਜੇ (ਬਾਈਟਾਂ_read == 0)
    ਬਾਈਟਾਂ_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= ਬਾਈਟਾਂ_read;
  ਜੇ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    ਵਾਪਸ false;
  ਬਾਈਟਾਂ_read = ਪਠ (fd, ਬਫਰ, ਬਾਈਟਾਂ_read);
  ਜੇ (ਬਾਈਟਾਂ_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      ਵਾਪਸ false;
    }

  /* n_lines == 0 case needs special treatment. */
  ਅਚਲ bool all_ਸਤਰਾਂ = !n_ਸਤਰਾਂ;

  /* Count the incomplete line on files that don't end with a newline.  */
  ਜੇ (n_ਸਤਰਾਂ && ਬਾਈਟਾਂ_read && ਬਫਰ[ਬਾਈਟਾਂ_read - 1] != ਸਤਰ_ਸਿਰਾ)
    --n_ਸਤਰਾਂ;

  ਜਦੋਂ (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = ਬਾਈਟਾਂ_read;
      ਜਦੋਂ (n)
        {
          ਜੇ (all_ਸਤਰਾਂ)
            n -= 1;
          ਨਹੀਂਤਾਂ
            {
              ਅੱਖਰ ਅਚਲ *nl;
              nl = memrchr (ਬਫਰ, ਸਤਰ_ਸਿਰਾ, n);
              ਜੇ (nl == ਨਿਰਾ)
                ਰੋਕੋ;
              n = nl - ਬਫਰ;
            }
          ਜੇ (n_ਸਤਰਾਂ-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              ਜੇ (ਸ਼ੁਰੂ_pos < pos)
                {
                  ਗਿਣਤੀ Copy_fd_ਸਥਿਤੀ err;
                  ਜੇ (elseek (fd, ਸ਼ੁਰੂ_pos, SEEK_SET, pretty_filename) < 0)
                    ਵਾਪਸ false;

                  err = copy_fd (fd, pos - ਸ਼ੁਰੂ_pos);
                  ਜੇ (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      ਵਾਪਸ false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (ਬਫਰ, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              ਵਾਪਸ 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      ਜੇ (pos == ਸ਼ੁਰੂ_pos)
        {
          /* Not enough lines in the file.  */
          ਵਾਪਸ true;
        }
      pos -= BUFSIZ;
      ਜੇ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        ਵਾਪਸ false;

      ਬਾਈਟਾਂ_read = ਪਠ (fd, ਬਫਰ, BUFSIZ);
      ਜੇ (ਬਾਈਟਾਂ_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          ਵਾਪਸ false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      ਜੇ (ਬਾਈਟਾਂ_read == 0)
        ਵਾਪਸ true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

ਸਥਿਰ bool
elide_ਪੂਛ_ਸਤਰਾਂ_file (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ fd, uintmax_t n_elide,
                       ਬਣਤਰ stat ਅਚਲ *st, off_t current_pos)
{
  off_t ਮਾਪ = st->st_ਮਾਪ;
  ਜੇ (current_pos < 0 || ਮਾਪ <= STP_BLKSIZE (st))
    ਵਾਪਸ elide_ਪੂਛ_ਸਤਰਾਂ_pipe (filename, fd, n_elide, current_pos);
  ਨਹੀਂਤਾਂ
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      ਵਾਪਸ (ਮਾਪ <= current_pos
              || elide_ਪੂਛ_ਸਤਰਾਂ_seekable (filename, fd, n_elide,
                                            current_pos, ਮਾਪ));
    }
}

ਸਥਿਰ bool
ਸਿਰ_ਬਾਈਟਾਂ (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ fd, uintmax_t ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_write)
{
  ਜੇ (copy_fd (fd, ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      ਵਾਪਸ false;
    }
  ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਸਿਰ_ਸਤਰਾਂ (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ fd, uintmax_t ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_write)
{
  ਅੱਖਰ ਬਫਰ[BUFSIZ];

  ਜਦੋਂ (ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_write)
    {
      ssize_t ਬਾਈਟਾਂ_read = ਪਠ (fd, ਬਫਰ, BUFSIZ);
      idx_t ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_write = 0;

      ਜੇ (ਬਾਈਟਾਂ_read < 0)
        {
          diagnose_read_failure (filename);
          ਵਾਪਸ false;
        }
      ਜੇ (ਬਾਈਟਾਂ_read == 0)
        ਰੋਕੋ;
      ਜਦੋਂ (ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_write < ਬਾਈਟਾਂ_read)
        ਜੇ (ਬਫਰ[ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_write++] == ਸਤਰ_ਸਿਰਾ && --ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_write == 0)
          {
            off_t n_ਬਾਈਟਾਂ_past_EOL = ਬਾਈਟਾਂ_read - ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            ਜੇ (lseek (fd, -n_ਬਾਈਟਾਂ_past_EOL, SEEK_CUR) < 0)
              {
                ਬਣਤਰ stat st;
                ਜੇ (fstat (fd, &st) != 0 || S_ISREG (st.st_ਬਹੁਲਕ))
                  elseek_diagnostic (-n_ਬਾਈਟਾਂ_past_EOL, SEEK_CUR, filename);
              }
            ਰੋਕੋ;
          }
      xwrite_stdout (ਬਫਰ, ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_write);
    }
  ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਸਿਰ (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ fd, uintmax_t n_units, bool ਗਿਣਤੀ_ਸਤਰਾਂ,
      bool elide_from_ਸਿਰਾ)
{
  ਜੇ (print_ਸਿਰਲੇਖਾਂ)
    write_ਸਿਰਲੇਖ (filename);

  ਜੇ (elide_from_ਸਿਰਾ)
    {
      /* Optimize for "infinite" elisions.  */
      ਜੇ (n_units == UINTMAX_MAX)
        ਵਾਪਸ true;

      off_t current_pos = -1;
      ਬਣਤਰ stat st;
      ਜੇ (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          ਵਾਪਸ false;
        }
      ਜੇ (! presume_ਨਿਵੇਸ਼_pipe && S_ISREG (st.st_ਬਹੁਲਕ))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          ਜੇ (current_pos < 0)
            ਵਾਪਸ false;
        }
      ਜੇ (ਗਿਣਤੀ_ਸਤਰਾਂ)
        ਵਾਪਸ elide_ਪੂਛ_ਸਤਰਾਂ_file (filename, fd, n_units, &st, current_pos);
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ elide_ਪੂਛ_ਬਾਈਟਾਂ_file (filename, fd, n_units, &st, current_pos);
    }
  ਜੇ (ਗਿਣਤੀ_ਸਤਰਾਂ)
    ਵਾਪਸ ਸਿਰ_ਸਤਰਾਂ (filename, fd, n_units);
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਸਿਰ_ਬਾਈਟਾਂ (filename, fd, n_units);
}

ਸਥਿਰ bool
ਸਿਰ_file (ਅੱਖਰ ਅਚਲ *filename, uintmax_t n_units, bool ਗਿਣਤੀ_ਸਤਰਾਂ,
           bool elide_from_ਸਿਰਾ)
{
  ਪੂਰਨ fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  ਜੇ (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDIN_FILENO, O_ਦੁਆਧਾਰੀ);
    }
  ਨਹੀਂਤਾਂ
    {
      fd = open (filename, O_RDONLY | O_ਦੁਆਧਾਰੀ);
      ਜੇ (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          ਵਾਪਸ false;
        }
    }

  ok = ਸਿਰ (filename, fd, n_units, ਗਿਣਤੀ_ਸਤਰਾਂ, elide_from_ਸਿਰਾ);
  ਜੇ (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      ਵਾਪਸ false;
    }
  ਵਾਪਸ ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

ਸਥਿਰ uintmax_t
string_ਮੰਜ਼ਿਲ_integer (bool ਗਿਣਤੀ_ਸਤਰਾਂ, ਅੱਖਰ ਅਚਲ *n_string)
{
  ਵਾਪਸ xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (ਗਿਣਤੀ_ਸਤਰਾਂ
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਗਿਣਤੀ ਸਿਰਲੇਖ_ਬਹੁਲਕ ਸਿਰਲੇਖ_ਬਹੁਲਕ = ਗੁਣਜ_files;
  bool ok = true;
  ਪੂਰਨ c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_ਸੰਖਿਆ;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool ਗਿਣਤੀ_ਸਤਰਾਂ = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_ਸਿਰਾ = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ default_file_list[] = {"-", ਨਿਰਾ};
  ਅੱਖਰ ਅਚਲ *ਅਚਲ *file_list;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜੇ (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      ਅੱਖਰ *a = argv[1];
      ਅੱਖਰ *n_string = ++a;
      ਅੱਖਰ *ਸਿਰਾ_n_string;
      ਅੱਖਰ multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      ਕਰੋ ++a;
      ਜਦੋਂ (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      ਸਿਰਾ_n_string = a;

      /* Parse any appended option letters. */
      ਲਈ (; *a; a++)
        {
          ਚੋਣ (*a)
            {
            ਹਾਲਤ 'c':
              ਗਿਣਤੀ_ਸਤਰਾਂ = false;
              multiplier_char = 0;
              ਰੋਕੋ;

            ਹਾਲਤ 'b':
            ਹਾਲਤ 'k':
            ਹਾਲਤ 'm':
              ਗਿਣਤੀ_ਸਤਰਾਂ = false;
              multiplier_char = *a;
              ਰੋਕੋ;

            ਹਾਲਤ 'l':
              ਗਿਣਤੀ_ਸਤਰਾਂ = true;
              ਰੋਕੋ;

            ਹਾਲਤ 'q':
              ਸਿਰਲੇਖ_ਬਹੁਲਕ = never;
              ਰੋਕੋ;

            ਹਾਲਤ 'v':
              ਸਿਰਲੇਖ_ਬਹੁਲਕ = always;
              ਰੋਕੋ;

            ਹਾਲਤ 'z':
              ਸਤਰ_ਸਿਰਾ = '\0';
              ਰੋਕੋ;

            ਮੂਲ:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *ਸਿਰਾ_n_string = multiplier_char;
      ਜੇ (multiplier_char)
        *(++ਸਿਰਾ_n_string) = 0;

      n_units = string_ਮੰਜ਼ਿਲ_integer (ਗਿਣਤੀ_ਸਤਰਾਂ, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  ਜਦੋਂ ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ PRESUME_ਨਿਵੇਸ਼_PIPE_OPTION:
          presume_ਨਿਵੇਸ਼_pipe = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'c':
          ਗਿਣਤੀ_ਸਤਰਾਂ = false;
          elide_from_ਸਿਰਾ = (*optarg == '-');
          ਜੇ (elide_from_ਸਿਰਾ)
            ++optarg;
          n_units = string_ਮੰਜ਼ਿਲ_integer (ਗਿਣਤੀ_ਸਤਰਾਂ, optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'n':
          ਗਿਣਤੀ_ਸਤਰਾਂ = true;
          elide_from_ਸਿਰਾ = (*optarg == '-');
          ਜੇ (elide_from_ਸਿਰਾ)
            ++optarg;
          n_units = string_ਮੰਜ਼ਿਲ_integer (ਗਿਣਤੀ_ਸਤਰਾਂ, optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'q':
          ਸਿਰਲੇਖ_ਬਹੁਲਕ = never;
          ਰੋਕੋ;

        ਹਾਲਤ 'v':
          ਸਿਰਲੇਖ_ਬਹੁਲਕ = always;
          ਰੋਕੋ;

        ਹਾਲਤ 'z':
          ਸਤਰ_ਸਿਰਾ = '\0';
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          ਜੇ (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (ਸਿਰਲੇਖ_ਬਹੁਲਕ == always
      || (ਸਿਰਲੇਖ_ਬਹੁਲਕ == ਗੁਣਜ_files && optind < argc - 1))
    print_ਸਿਰਲੇਖਾਂ = true;

  file_list = (optind < argc
               ? (ਅੱਖਰ ਅਚਲ *ਅਚਲ *) &argv[optind]
               : default_file_list);

  xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDOUT_FILENO, O_ਦੁਆਧਾਰੀ);

  ਲਈ (size_t i = 0; file_list[i]; ++i)
    ok &= ਸਿਰ_file (file_list[i], n_units, ਗਿਣਤੀ_ਸਤਰਾਂ, elide_from_ਸਿਰਾ);

  ਜੇ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
