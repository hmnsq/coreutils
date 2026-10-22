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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "head"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_ಸಂಖ್ಯೆ 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ಸ್ಥಿರ bool presume_ಒಳಹರಿವು_pipe;

/* If true, print filename headers. */
ಸ್ಥಿರ bool print_ಶೀರ್ಷಿಕೆಗಳು;

/* Character to split lines by. */
ಸ್ಥಿರ ಅಕ್ಷರ ಸಾಲು_ಕೊನೆ = '\n';

/* When to print the filename banners. */
ಎಣಿಕೆ ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ
{
  ಅಪವರ್ತ್ಯ_files, always, never
};

/* Have we ever read standard input?  */
ಸ್ಥಿರ bool have_read_stdin;

ಎಣಿಕೆ Copy_fd_ಸ್ಥಿತಿ
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  PRESUME_ಒಳಹರಿವು_PIPE_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"bytes", required_ವಾದ, ಖಾಲಿ, 'c'},
  {"lines", required_ವಾದ, ಖಾಲಿ, 'n'},
  {"-presume-input-pipe", no_ವಾದ, ಖಾಲಿ,
   PRESUME_ಒಳಹರಿವು_PIPE_OPTION}, /* do not document */
  {"quiet", no_ವಾದ, ಖಾಲಿ, 'q'},
  {"silent", no_ವಾದ, ಖಾಲಿ, 'q'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {"zero-terminated", no_ವಾದ, ಖಾಲಿ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      ಬರೆ (_("\
Print the ಮೊದಲ %d ಸಾಲುಗಳು ದ each FILE ಗಮ್ಯ standard output.\n\
With ಇನ್ನಷ್ಟು than one FILE, precede each with a ಶೀರ್ಷಿಕೆ giving the file name.\n\
"), DEFAULT_ಸಂಖ್ಯೆ);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --ಬೈಟುಗಳು=[-]ಅಂಕೆ_2\n\
         print the ಮೊದಲ ಅಂಕೆ_2 ಬೈಟುಗಳು ದ each file;\n\
         with the leading '-', print all but the ಕೊನೆಯ ಅಂಕೆ_2 ಬೈಟುಗಳು ದ each file\n\
"));
      oprintf (_("\
  -n, --ಸಾಲುಗಳು=[-]ಅಂಕೆ_2\n\
         print the ಮೊದಲ ಅಂಕೆ_2 ಸಾಲುಗಳು instead ದ the ಮೊದಲ %d;\n\
         with the leading '-', print all but the ಕೊನೆಯ ಅಂಕೆ_2 ಸಾಲುಗಳು ದ each file\n\
"), DEFAULT_ಸಂಖ್ಯೆ);
      oputs (_("\
  -q, --quiet, --silent\n\
         never print ಶೀರ್ಷಿಕೆಗಳು giving file ಹೆಸರುಗಳು\n\
"));
      oputs (_("\
  -v, --verbose\n\
         always print ಶೀರ್ಷಿಕೆಗಳು giving file ಹೆಸರುಗಳು\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ಅಂಕೆ_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on ಗಾಗಿ T, P, E, Z, Y, R, Q.\n\
ದ್ವಿಮಾನ_2 prefixes can be ಬಳಸಿದ, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagnose_read_failure (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagnose_copy_fd_failure (ಎಣಿಕೆ Copy_fd_ಸ್ಥಿತಿ err, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  ಆಯ್ಕೆ (err)
    {
    ಸ್ಥಿತಿ COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ COPY_FD_OK: ಪೂರ್ವನಿಯೋಜಿತ:
      affirm (false);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
write_ಶೀರ್ಷಿಕೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  ಸ್ಥಿರ bool ಮೊದಲ_file = true;

  ಬರೆ ("%s==> %s <==\n", (ಮೊದಲ_file ? "" : "\n"), quotef (filename));
  ಮೊದಲ_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ಸ್ಥಿರ ಶೂನ್ಯ
xwrite_stdout (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಫರ್, size_t n_ಬೈಟುಗಳು)
{
  ಆದರೆ (n_ಬೈಟುಗಳು > 0 && fwrite (ಬಫರ್, 1, n_ಬೈಟುಗಳು, stdout) < n_ಬೈಟುಗಳು)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

ಸ್ಥಿರ ಎಣಿಕೆ Copy_fd_ಸ್ಥಿತಿ
copy_fd (ಪೂರ್ಣಾಂಕ src_fd, uintmax_t n_ಬೈಟುಗಳು)
{
  ಅಕ್ಷರ buf[BUFSIZ];

  /* Copy the file contents.  */
  ಆಗ (0 < n_ಬೈಟುಗಳು)
    {
      idx_t n_ಗಮ್ಯ_read = MIN (n_ಬೈಟುಗಳು, ಗಾತ್ರ buf);
      ssize_t n_read = ಪಠ (src_fd, buf, n_ಗಮ್ಯ_read);
      ಆದರೆ (n_read < 0)
        ಹಿಂತಿರುಗಿ COPY_FD_READ_ERROR;

      n_ಬೈಟುಗಳು -= n_read;

      ಆದರೆ (n_read == 0 && n_ಬೈಟುಗಳು != 0)
        ಹಿಂತಿರುಗಿ COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  ಹಿಂತಿರುಗಿ COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
ಸ್ಥಿರ ಶೂನ್ಯ
elseek_diagnostic (off_t ಸ್ಥಾನಾಂತರ, ಪೂರ್ಣಾಂಕ whence, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  intmax_t off = ಸ್ಥಾನಾಂತರ;
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
elseek (ಪೂರ್ಣಾಂಕ fd, off_t ಸ್ಥಾನಾಂತರ, ಪೂರ್ಣಾಂಕ whence, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  off_t new_ಸ್ಥಾನಾಂತರ = lseek (fd, ಸ್ಥಾನಾಂತರ, whence);

  ಆದರೆ (new_ಸ್ಥಾನಾಂತರ < 0)
    elseek_diagnostic (ಸ್ಥಾನಾಂತರ, whence, filename);

  ಹಿಂತಿರುಗಿ new_ಸ್ಥಾನಾಂತರ;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
ಸ್ಥಿರ bool
elide_ಬಾಲ_ಬೈಟುಗಳು_pipe (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ fd, uintmax_t n_elide,
                       off_t current_pos)
{
  uintmax_t desired_pos = current_pos;
  bool ok = true;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ತಲೆ_ಬಾಲ_PIPE_READ_BUFSIZE
# define ತಲೆ_ಬಾಲ_PIPE_READ_BUFSIZE BUFSIZ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು READ_BUFSIZE ತಲೆ_ಬಾಲ_PIPE_READ_BUFSIZE

  /* If we're eliding no more than this many bytes, then it's ok to allocate
     more memory in order to use a more time-efficient algorithm.
     FIXME: use a fraction of available memory instead, as in sort.
     FIXME: is this even worthwhile?  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ತಲೆ_ಬಾಲ_PIPE_BYTECOUNT_ಮಿತಿ
# define ತಲೆ_ಬಾಲ_PIPE_BYTECOUNT_ಮಿತಿ 1024 * 1024
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ತಲೆ_ಬಾಲ_PIPE_BYTECOUNT_ಮಿತಿ < 2 * READ_BUFSIZE
# error "HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD must be at least 2 * READ_BUFSIZE"
#ಆದರೆ_ಅಂತ್ಯ

  /* Two cases to consider...
     1) n_elide is small enough that we can afford to double-buffer:
        allocate 2 * (READ_BUFSIZE + n_elide) bytes
     2) n_elide is too big for that, so we allocate only
        (READ_BUFSIZE + n_elide) bytes

     FIXME: profile, to see if double-buffering is worthwhile

     CAUTION: do not fail (out of memory) when asked to elide
     a ridiculous amount, but when given only a small input.  */

  static_assert (READ_BUFSIZE <= IDX_MAX);
  static_assert (ತಲೆ_ಬಾಲ_PIPE_BYTECOUNT_ಮಿತಿ <= IDX_MAX - READ_BUFSIZE);
  ಆದರೆ (n_elide <= ತಲೆ_ಬಾಲ_PIPE_BYTECOUNT_ಮಿತಿ)
    {
      idx_t ಒಳ_elide = n_elide;
      bool ಮೊದಲ = true;
      bool eof = false;
      idx_t n_ಗಮ್ಯ_read = READ_BUFSIZE + n_elide;
      ಅಕ್ಷರ *b[2];
      b[0] = xnmalloc (2, n_ಗಮ್ಯ_read);
      b[1] = b[0] + n_ಗಮ್ಯ_read;

      ಗಾಗಿ (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = ತುಂಬಿದ_read (fd, b[i], n_ಗಮ್ಯ_read);
          idx_t delta = 0;
          ಆದರೆ (n_read < n_ಗಮ್ಯ_read)
            {
              ಆದರೆ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  ನಿಲ್ಲಿಸು;
                }

              /* reached EOF */
              ಆದರೆ (n_read <= ಒಳ_elide)
                {
                  ಆದರೆ (ಮೊದಲ)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  ಇಲ್ಲದಿದ್ದರೆ
                    {
                      delta = ಒಳ_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          ಆದರೆ (! ಮೊದಲ)
            {
              desired_pos += ಒಳ_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, ಒಳ_elide - delta);
            }
          ಮೊದಲ = false;

          ಆದರೆ (ಒಳ_elide < n_read)
            {
              desired_pos += n_read - ಒಳ_elide;
              xwrite_stdout (b[i], n_read - ಒಳ_elide);
            }
        }

      ಬಿಡುಗಡೆ (b[0]);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      ಅಕ್ಷರ **b = ಖಾಲಿ;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_ಮುಂದಿನ;
      ಗಾಗಿ (i = 0, i_ಮುಂದಿನ = 1; !eof; i = i_ಮುಂದಿನ, i_ಮುಂದಿನ = (i_ಮುಂದಿನ + 1) % n_bufs)
        {
          ಆದರೆ (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         ಗಾತ್ರ *b);

          ಆದರೆ (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = ತುಂಬಿದ_read (fd, b[i], READ_BUFSIZE);
          ಆದರೆ (n_read < READ_BUFSIZE)
            {
              ಆದರೆ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  ಹೋಗು free_mem;
                }
              eof = true;
            }

          ಆದರೆ (i + 1 == n_bufs)
            buffered_enough = true;

          ಆದರೆ (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_ಮುಂದಿನ], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      ಆದರೆ (buffered_enough)
        {
          idx_t n_ಬೈಟುಗಳು_ಎಡ_ಒಳ_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          ಆದರೆ (rem < n_ಬೈಟುಗಳು_ಎಡ_ಒಳ_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              xwrite_stdout (b[i] + n_read, n_ಬೈಟುಗಳು_ಎಡ_ಒಳ_b_i);
              xwrite_stdout (b[i_ಮುಂದಿನ], rem - n_ಬೈಟುಗಳು_ಎಡ_ಒಳ_b_i);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (i + 1 == n_bufs)
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
          xwrite_stdout (b[i_ಮುಂದಿನ], x);
        }

    free_mem:
      ಗಾಗಿ (i = 0; i < n_alloc; i++)
        ಬಿಡುಗಡೆ (b[i]);
      ಬಿಡುಗಡೆ (b);
    }

  ಆದರೆ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  ಹಿಂತಿರುಗಿ ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

ಸ್ಥಿರ bool
elide_ಬಾಲ_ಬೈಟುಗಳು_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ fd, uintmax_t n_elide,
                       ರಚನೆ stat ಸ್ಥಿರಾಂಕ *st, off_t current_pos)
{
  off_t ಗಾತ್ರ_2 = st->st_ಗಾತ್ರ;
  ಆದರೆ (current_pos < 0 || ಗಾತ್ರ_2 <= STP_BLKSIZE (st))
    ಹಿಂತಿರುಗಿ elide_ಬಾಲ_ಬೈಟುಗಳು_pipe (filename, fd, n_elide, current_pos);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = ಗಾತ್ರ_2 - current_pos;
      off_t ಬೈಟುಗಳು_ಉಳಿದ = diff < 0 ? 0 : diff;

      ಆದರೆ (ಬೈಟುಗಳು_ಉಳಿದ <= n_elide)
        ಹಿಂತಿರುಗಿ true;

      ಎಣಿಕೆ Copy_fd_ಸ್ಥಿತಿ err = copy_fd (fd, ಬೈಟುಗಳು_ಉಳಿದ - n_elide);
      ಆದರೆ (err == COPY_FD_OK)
        ಹಿಂತಿರುಗಿ true;

      diagnose_copy_fd_failure (err, filename);
      ಹಿಂತಿರುಗಿ false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

ಸ್ಥಿರ bool
elide_ಬಾಲ_ಸಾಲುಗಳು_pipe (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ fd, uintmax_t n_elide,
                       off_t current_pos)
{
  ರಚನೆ linebuffer
  {
    ಅಕ್ಷರ ಬಫರ್[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    ರಚನೆ linebuffer *ಮುಂದಿನ;
  };
  off_t desired_pos = current_pos;
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ linebuffer LBUFFER;
  LBUFFER *ಮೊದಲ, *ಕೊನೆಯ, *tmp;
  size_t ಒಟ್ಟು_ಸಾಲುಗಳು = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  ಮೊದಲ = ಕೊನೆಯ = xmalloc (ಗಾತ್ರ (LBUFFER));
  ಮೊದಲ->nbytes = ಮೊದಲ->nlines = 0;
  ಮೊದಲ->ಮುಂದಿನ = ಖಾಲಿ;
  tmp = xmalloc (ಗಾತ್ರ (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  ಆಗ (true)
    {
      n_read = ಪಠ (fd, tmp->ಬಫರ್, BUFSIZ);
      ಆದರೆ (n_read <= 0)
        ನಿಲ್ಲಿಸು;

      ಆದರೆ (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->ಬಫರ್, n_read);
          ಮುಂದುವರಿ;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->ಮುಂದಿನ = ಖಾಲಿ;

      /* Count the number of newlines just read.  */
      {
        ಅಕ್ಷರ *ಬಫರ್_ಕೊನೆ = tmp->ಬಫರ್ + n_read;
        *ಬಫರ್_ಕೊನೆ = ಸಾಲು_ಕೊನೆ;
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = tmp->ಬಫರ್;
        ಆಗ ((p = rawmemchr (p, ಸಾಲು_ಕೊನೆ)) < ಬಫರ್_ಕೊನೆ)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      ಒಟ್ಟು_ಸಾಲುಗಳು += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      ಆದರೆ (tmp->nbytes + ಕೊನೆಯ->nbytes < BUFSIZ)
        {
          ಸ್ಮರಣೆ_ನಕಲು (&ಕೊನೆಯ->ಬಫರ್[ಕೊನೆಯ->nbytes], tmp->ಬಫರ್, tmp->nbytes);
          ಕೊನೆಯ->nbytes += tmp->nbytes;
          ಕೊನೆಯ->nlines += tmp->nlines;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          ಕೊನೆಯ = ಕೊನೆಯ->ಮುಂದಿನ = tmp;
          ಆದರೆ (n_elide < ಒಟ್ಟು_ಸಾಲುಗಳು - ಮೊದಲ->nlines)
            {
              desired_pos += ಮೊದಲ->nbytes;
              xwrite_stdout (ಮೊದಲ->ಬಫರ್, ಮೊದಲ->nbytes);
              tmp = ಮೊದಲ;
              ಒಟ್ಟು_ಸಾಲುಗಳು -= ಮೊದಲ->nlines;
              ಮೊದಲ = ಮೊದಲ->ಮುಂದಿನ;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            tmp = xmalloc (ಗಾತ್ರ (LBUFFER));
        }
    }

  ಬಿಡುಗಡೆ (tmp);

  ಆದರೆ (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      ಹೋಗು free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  ಆದರೆ (ಕೊನೆಯ->nbytes && ಕೊನೆಯ->ಬಫರ್[ಕೊನೆಯ->nbytes - 1] != ಸಾಲು_ಕೊನೆ)
    {
      ++ಕೊನೆಯ->nlines;
      ++ಒಟ್ಟು_ಸಾಲುಗಳು;
    }

  ಗಾಗಿ (tmp = ಮೊದಲ; n_elide < ಒಟ್ಟು_ಸಾಲುಗಳು - tmp->nlines; tmp = tmp->ಮುಂದಿನ)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->ಬಫರ್, tmp->nbytes);
      ಒಟ್ಟು_ಸಾಲುಗಳು -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  ಆದರೆ (n_elide < ಒಟ್ಟು_ಸಾಲುಗಳು)
    {
      size_t n = ಒಟ್ಟು_ಸಾಲುಗಳು - n_elide;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಫರ್_ಕೊನೆ = tmp->ಬಫರ್ + tmp->nbytes;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = tmp->ಬಫರ್;
      ಆಗ (n && (p = memchr (p, ಸಾಲು_ಕೊನೆ, ಬಫರ್_ಕೊನೆ - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->ಬಫರ್;
      xwrite_stdout (tmp->ಬಫರ್, p - tmp->ಬಫರ್);
    }

free_lbuffers:
  ಆಗ (ಮೊದಲ)
    {
      tmp = ಮೊದಲ->ಮುಂದಿನ;
      ಬಿಡುಗಡೆ (ಮೊದಲ);
      ಮೊದಲ = tmp;
    }

  ಆದರೆ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  ಹಿಂತಿರುಗಿ ok;
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
elide_ಬಾಲ_ಸಾಲುಗಳು_seekable (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *pretty_filename, ಪೂರ್ಣಾಂಕ fd,
                           uintmax_t n_ಸಾಲುಗಳು,
                           off_t ಆರಂಭ_pos, off_t ಗಾತ್ರ_2)
{
  ಅಕ್ಷರ ಬಫರ್[BUFSIZ];
  ssize_t ಬೈಟುಗಳು_read;
  off_t pos = ಗಾತ್ರ_2;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  ಬೈಟುಗಳು_read = (pos - ಆರಂಭ_pos) % BUFSIZ;
  ಆದರೆ (ಬೈಟುಗಳು_read == 0)
    ಬೈಟುಗಳು_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= ಬೈಟುಗಳು_read;
  ಆದರೆ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    ಹಿಂತಿರುಗಿ false;
  ಬೈಟುಗಳು_read = ಪಠ (fd, ಬಫರ್, ಬೈಟುಗಳು_read);
  ಆದರೆ (ಬೈಟುಗಳು_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      ಹಿಂತಿರುಗಿ false;
    }

  /* n_lines == 0 case needs special treatment. */
  ಸ್ಥಿರಾಂಕ bool all_ಸಾಲುಗಳು = !n_ಸಾಲುಗಳು;

  /* Count the incomplete line on files that don't end with a newline.  */
  ಆದರೆ (n_ಸಾಲುಗಳು && ಬೈಟುಗಳು_read && ಬಫರ್[ಬೈಟುಗಳು_read - 1] != ಸಾಲು_ಕೊನೆ)
    --n_ಸಾಲುಗಳು;

  ಆಗ (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = ಬೈಟುಗಳು_read;
      ಆಗ (n)
        {
          ಆದರೆ (all_ಸಾಲುಗಳು)
            n -= 1;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *nl;
              nl = memrchr (ಬಫರ್, ಸಾಲು_ಕೊನೆ, n);
              ಆದರೆ (nl == ಖಾಲಿ)
                ನಿಲ್ಲಿಸು;
              n = nl - ಬಫರ್;
            }
          ಆದರೆ (n_ಸಾಲುಗಳು-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              ಆದರೆ (ಆರಂಭ_pos < pos)
                {
                  ಎಣಿಕೆ Copy_fd_ಸ್ಥಿತಿ err;
                  ಆದರೆ (elseek (fd, ಆರಂಭ_pos, SEEK_SET, pretty_filename) < 0)
                    ಹಿಂತಿರುಗಿ false;

                  err = copy_fd (fd, pos - ಆರಂಭ_pos);
                  ಆದರೆ (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      ಹಿಂತಿರುಗಿ false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (ಬಫರ್, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              ಹಿಂತಿರುಗಿ 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      ಆದರೆ (pos == ಆರಂಭ_pos)
        {
          /* Not enough lines in the file.  */
          ಹಿಂತಿರುಗಿ true;
        }
      pos -= BUFSIZ;
      ಆದರೆ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        ಹಿಂತಿರುಗಿ false;

      ಬೈಟುಗಳು_read = ಪಠ (fd, ಬಫರ್, BUFSIZ);
      ಆದರೆ (ಬೈಟುಗಳು_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          ಹಿಂತಿರುಗಿ false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      ಆದರೆ (ಬೈಟುಗಳು_read == 0)
        ಹಿಂತಿರುಗಿ true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

ಸ್ಥಿರ bool
elide_ಬಾಲ_ಸಾಲುಗಳು_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ fd, uintmax_t n_elide,
                       ರಚನೆ stat ಸ್ಥಿರಾಂಕ *st, off_t current_pos)
{
  off_t ಗಾತ್ರ_2 = st->st_ಗಾತ್ರ;
  ಆದರೆ (current_pos < 0 || ಗಾತ್ರ_2 <= STP_BLKSIZE (st))
    ಹಿಂತಿರುಗಿ elide_ಬಾಲ_ಸಾಲುಗಳು_pipe (filename, fd, n_elide, current_pos);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      ಹಿಂತಿರುಗಿ (ಗಾತ್ರ_2 <= current_pos
              || elide_ಬಾಲ_ಸಾಲುಗಳು_seekable (filename, fd, n_elide,
                                            current_pos, ಗಾತ್ರ_2));
    }
}

ಸ್ಥಿರ bool
ತಲೆ_ಬೈಟುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ fd, uintmax_t ಬೈಟುಗಳು_ಗಮ್ಯ_write)
{
  ಆದರೆ (copy_fd (fd, ಬೈಟುಗಳು_ಗಮ್ಯ_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      ಹಿಂತಿರುಗಿ false;
    }
  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ತಲೆ_ಸಾಲುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ fd, uintmax_t ಸಾಲುಗಳು_ಗಮ್ಯ_write)
{
  ಅಕ್ಷರ ಬಫರ್[BUFSIZ];

  ಆಗ (ಸಾಲುಗಳು_ಗಮ್ಯ_write)
    {
      ssize_t ಬೈಟುಗಳು_read = ಪಠ (fd, ಬಫರ್, BUFSIZ);
      idx_t ಬೈಟುಗಳು_ಗಮ್ಯ_write = 0;

      ಆದರೆ (ಬೈಟುಗಳು_read < 0)
        {
          diagnose_read_failure (filename);
          ಹಿಂತಿರುಗಿ false;
        }
      ಆದರೆ (ಬೈಟುಗಳು_read == 0)
        ನಿಲ್ಲಿಸು;
      ಆಗ (ಬೈಟುಗಳು_ಗಮ್ಯ_write < ಬೈಟುಗಳು_read)
        ಆದರೆ (ಬಫರ್[ಬೈಟುಗಳು_ಗಮ್ಯ_write++] == ಸಾಲು_ಕೊನೆ && --ಸಾಲುಗಳು_ಗಮ್ಯ_write == 0)
          {
            off_t n_ಬೈಟುಗಳು_past_EOL = ಬೈಟುಗಳು_read - ಬೈಟುಗಳು_ಗಮ್ಯ_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            ಆದರೆ (lseek (fd, -n_ಬೈಟುಗಳು_past_EOL, SEEK_CUR) < 0)
              {
                ರಚನೆ stat st;
                ಆದರೆ (fstat (fd, &st) != 0 || S_ISREG (st.st_ಬಹುಲಕ))
                  elseek_diagnostic (-n_ಬೈಟುಗಳು_past_EOL, SEEK_CUR, filename);
              }
            ನಿಲ್ಲಿಸು;
          }
      xwrite_stdout (ಬಫರ್, ಬೈಟುಗಳು_ಗಮ್ಯ_write);
    }
  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ತಲೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ fd, uintmax_t n_units, bool ಎಣಿಕೆ_ಸಾಲುಗಳು,
      bool elide_from_ಕೊನೆ)
{
  ಆದರೆ (print_ಶೀರ್ಷಿಕೆಗಳು)
    write_ಶೀರ್ಷಿಕೆ (filename);

  ಆದರೆ (elide_from_ಕೊನೆ)
    {
      /* Optimize for "infinite" elisions.  */
      ಆದರೆ (n_units == UINTMAX_MAX)
        ಹಿಂತಿರುಗಿ true;

      off_t current_pos = -1;
      ರಚನೆ stat st;
      ಆದರೆ (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          ಹಿಂತಿರುಗಿ false;
        }
      ಆದರೆ (! presume_ಒಳಹರಿವು_pipe && S_ISREG (st.st_ಬಹುಲಕ))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          ಆದರೆ (current_pos < 0)
            ಹಿಂತಿರುಗಿ false;
        }
      ಆದರೆ (ಎಣಿಕೆ_ಸಾಲುಗಳು)
        ಹಿಂತಿರುಗಿ elide_ಬಾಲ_ಸಾಲುಗಳು_file (filename, fd, n_units, &st, current_pos);
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ elide_ಬಾಲ_ಬೈಟುಗಳು_file (filename, fd, n_units, &st, current_pos);
    }
  ಆದರೆ (ಎಣಿಕೆ_ಸಾಲುಗಳು)
    ಹಿಂತಿರುಗಿ ತಲೆ_ಸಾಲುಗಳು (filename, fd, n_units);
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ ತಲೆ_ಬೈಟುಗಳು (filename, fd, n_units);
}

ಸ್ಥಿರ bool
ತಲೆ_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, uintmax_t n_units, bool ಎಣಿಕೆ_ಸಾಲುಗಳು,
           bool elide_from_ಕೊನೆ)
{
  ಪೂರ್ಣಾಂಕ fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  ಆದರೆ (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDIN_FILENO, O_ದ್ವಿಮಾನ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      fd = open (filename, O_RDONLY | O_ದ್ವಿಮಾನ);
      ಆದರೆ (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          ಹಿಂತಿರುಗಿ false;
        }
    }

  ok = ತಲೆ (filename, fd, n_units, ಎಣಿಕೆ_ಸಾಲುಗಳು, elide_from_ಕೊನೆ);
  ಆದರೆ (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      ಹಿಂತಿರುಗಿ false;
    }
  ಹಿಂತಿರುಗಿ ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

ಸ್ಥಿರ uintmax_t
string_ಗಮ್ಯ_integer (bool ಎಣಿಕೆ_ಸಾಲುಗಳು, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *n_string)
{
  ಹಿಂತಿರುಗಿ xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (ಎಣಿಕೆ_ಸಾಲುಗಳು
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಎಣಿಕೆ ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = ಅಪವರ್ತ್ಯ_files;
  bool ok = true;
  ಪೂರ್ಣಾಂಕ c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_ಸಂಖ್ಯೆ;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool ಎಣಿಕೆ_ಸಾಲುಗಳು = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_ಕೊನೆ = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ default_file_list[] = {"-", ಖಾಲಿ};
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ *file_list;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆದರೆ (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      ಅಕ್ಷರ *a = argv[1];
      ಅಕ್ಷರ *n_string = ++a;
      ಅಕ್ಷರ *ಕೊನೆ_n_string;
      ಅಕ್ಷರ multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      ಮಾಡು ++a;
      ಆಗ (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      ಕೊನೆ_n_string = a;

      /* Parse any appended option letters. */
      ಗಾಗಿ (; *a; a++)
        {
          ಆಯ್ಕೆ (*a)
            {
            ಸ್ಥಿತಿ 'c':
              ಎಣಿಕೆ_ಸಾಲುಗಳು = false;
              multiplier_char = 0;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'b':
            ಸ್ಥಿತಿ 'k':
            ಸ್ಥಿತಿ 'm':
              ಎಣಿಕೆ_ಸಾಲುಗಳು = false;
              multiplier_char = *a;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'l':
              ಎಣಿಕೆ_ಸಾಲುಗಳು = true;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'q':
              ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = never;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'v':
              ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = always;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'z':
              ಸಾಲು_ಕೊನೆ = '\0';
              ನಿಲ್ಲಿಸು;

            ಪೂರ್ವನಿಯೋಜಿತ:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *ಕೊನೆ_n_string = multiplier_char;
      ಆದರೆ (multiplier_char)
        *(++ಕೊನೆ_n_string) = 0;

      n_units = string_ಗಮ್ಯ_integer (ಎಣಿಕೆ_ಸಾಲುಗಳು, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  ಆಗ ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ PRESUME_ಒಳಹರಿವು_PIPE_OPTION:
          presume_ಒಳಹರಿವು_pipe = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'c':
          ಎಣಿಕೆ_ಸಾಲುಗಳು = false;
          elide_from_ಕೊನೆ = (*optarg == '-');
          ಆದರೆ (elide_from_ಕೊನೆ)
            ++optarg;
          n_units = string_ಗಮ್ಯ_integer (ಎಣಿಕೆ_ಸಾಲುಗಳು, optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'n':
          ಎಣಿಕೆ_ಸಾಲುಗಳು = true;
          elide_from_ಕೊನೆ = (*optarg == '-');
          ಆದರೆ (elide_from_ಕೊನೆ)
            ++optarg;
          n_units = string_ಗಮ್ಯ_integer (ಎಣಿಕೆ_ಸಾಲುಗಳು, optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'q':
          ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = never;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'v':
          ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = always;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'z':
          ಸಾಲು_ಕೊನೆ = '\0';
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          ಆದರೆ (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == always
      || (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == ಅಪವರ್ತ್ಯ_files && optind < argc - 1))
    print_ಶೀರ್ಷಿಕೆಗಳು = true;

  file_list = (optind < argc
               ? (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ *) &argv[optind]
               : default_file_list);

  xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDOUT_FILENO, O_ದ್ವಿಮಾನ);

  ಗಾಗಿ (size_t i = 0; file_list[i]; ++i)
    ok &= ತಲೆ_file (file_list[i], n_units, ಎಣಿಕೆ_ಸಾಲುಗಳು, elide_from_ಕೊನೆ);

  ಆದರೆ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
