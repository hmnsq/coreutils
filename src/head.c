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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "full-read.h"
#അന്തര്ഭാവ "stat-size.h"
#അന്തര്ഭാവ "xbinary-io.h"
#അന്തര്ഭാവ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "head"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#define DEFAULT_സംഖ്യ 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
സ്ഥിതം bool presume_നിവേശം_pipe;

/* If true, print filename headers. */
സ്ഥിതം bool print_headers;

/* Character to split lines by. */
സ്ഥിതം അക്ഷരം വരി_end = '\n';

/* When to print the filename banners. */
എണ്ണൽ header_mode
{
  multiple_files, always, never
};

/* Have we ever read standard input?  */
സ്ഥിതം bool have_read_stdin;

എണ്ണൽ Copy_fd_status
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  PRESUME_നിവേശം_PIPE_OPTION = CHAR_MAX + 1
};

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"bytes", required_argument, ശൂന്യ, 'c'},
  {"lines", required_argument, ശൂന്യ, 'n'},
  {"-presume-input-pipe", no_argument, ശൂന്യ,
   PRESUME_നിവേശം_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, ശൂന്യ, 'q'},
  {"silent", no_argument, ശൂന്യ, 'q'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {"zero-terminated", no_argument, ശൂന്യ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      printf (_("\
Print the first %d വരികൾ of each FILE to standard output.\n\
With more than one FILE, precede each with a header giving the file name.\n\
"), DEFAULT_സംഖ്യ);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --bytes=[-]അക്കം_2\n\
         print the first അക്കം_2 bytes of each file;\n\
         with the leading '-', print all but the last അക്കം_2 bytes of each file\n\
"));
      oprintf (_("\
  -n, --വരികൾ=[-]അക്കം_2\n\
         print the first അക്കം_2 വരികൾ instead of the first %d;\n\
         with the leading '-', print all but the last അക്കം_2 വരികൾ of each file\n\
"), DEFAULT_സംഖ്യ);
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
         വരി delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
അക്കം_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on വേണ്ടി T, P, E, Z, Y, R, Q.\n\
ദ്വിമാന_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

സ്ഥിതം ശൂന്യം
diagnose_read_failure (അക്ഷരം സ്ഥിരം *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

സ്ഥിതം ശൂന്യം
diagnose_copy_fd_failure (എണ്ണൽ Copy_fd_status err, അക്ഷരം സ്ഥിരം *filename)
{
  തിരഞ്ഞെടുപ്പ് (err)
    {
    അവസ്ഥ COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      നിർത്തുക;
    അവസ്ഥ COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      നിർത്തുക;
    അവസ്ഥ COPY_FD_OK: സ്ഥിരസ്ഥിതി:
      affirm (false);
    }
}

സ്ഥിതം ശൂന്യം
write_header (അക്ഷരം സ്ഥിരം *filename)
{
  സ്ഥിതം bool first_file = true;

  printf ("%s==> %s <==\n", (first_file ? "" : "\n"), quotef (filename));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

സ്ഥിതം ശൂന്യം
xwrite_stdout (അക്ഷരം സ്ഥിരം *buffer, size_t n_bytes)
{
  എങ്കിൽ (n_bytes > 0 && fwrite (buffer, 1, n_bytes, stdout) < n_bytes)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

സ്ഥിതം എണ്ണൽ Copy_fd_status
copy_fd (പൂർണ്ണസംഖ്യ src_fd, uintmax_t n_bytes)
{
  അക്ഷരം buf[BUFSIZ];

  /* Copy the file contents.  */
  ആകുമ്പോൾ (0 < n_bytes)
    {
      idx_t n_to_read = MIN (n_bytes, വലുപ്പം buf);
      ssize_t n_read = പഠ (src_fd, buf, n_to_read);
      എങ്കിൽ (n_read < 0)
        തിരികെ COPY_FD_READ_ERROR;

      n_bytes -= n_read;

      എങ്കിൽ (n_read == 0 && n_bytes != 0)
        തിരികെ COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  തിരികെ COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
സ്ഥിതം ശൂന്യം
elseek_diagnostic (off_t offset, പൂർണ്ണസംഖ്യ whence, അക്ഷരം സ്ഥിരം *filename)
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

സ്ഥിതം off_t
elseek (പൂർണ്ണസംഖ്യ fd, off_t offset, പൂർണ്ണസംഖ്യ whence, അക്ഷരം സ്ഥിരം *filename)
{
  off_t new_offset = lseek (fd, offset, whence);

  എങ്കിൽ (new_offset < 0)
    elseek_diagnostic (offset, whence, filename);

  തിരികെ new_offset;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
സ്ഥിതം bool
elide_tail_bytes_pipe (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ fd, uintmax_t n_elide,
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

#എങ്കിൽ HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD < 2 * READ_BUFSIZE
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
  എങ്കിൽ (n_elide <= HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD)
    {
      idx_t in_elide = n_elide;
      bool first = true;
      bool eof = false;
      idx_t n_to_read = READ_BUFSIZE + n_elide;
      അക്ഷരം *b[2];
      b[0] = xnmalloc (2, n_to_read);
      b[1] = b[0] + n_to_read;

      വേണ്ടി (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = full_read (fd, b[i], n_to_read);
          idx_t delta = 0;
          എങ്കിൽ (n_read < n_to_read)
            {
              എങ്കിൽ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  നിർത്തുക;
                }

              /* reached EOF */
              എങ്കിൽ (n_read <= in_elide)
                {
                  എങ്കിൽ (first)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  അല്ലെങ്കിൽ
                    {
                      delta = in_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          എങ്കിൽ (! first)
            {
              desired_pos += in_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, in_elide - delta);
            }
          first = false;

          എങ്കിൽ (in_elide < n_read)
            {
              desired_pos += n_read - in_elide;
              xwrite_stdout (b[i], n_read - in_elide);
            }
        }

      സ്വതന്ത്രം (b[0]);
    }
  അല്ലെങ്കിൽ
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      അക്ഷരം **b = ശൂന്യ;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_next;
      വേണ്ടി (i = 0, i_next = 1; !eof; i = i_next, i_next = (i_next + 1) % n_bufs)
        {
          എങ്കിൽ (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         വലുപ്പം *b);

          എങ്കിൽ (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = full_read (fd, b[i], READ_BUFSIZE);
          എങ്കിൽ (n_read < READ_BUFSIZE)
            {
              എങ്കിൽ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  പോകുക free_mem;
                }
              eof = true;
            }

          എങ്കിൽ (i + 1 == n_bufs)
            buffered_enough = true;

          എങ്കിൽ (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_next], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      എങ്കിൽ (buffered_enough)
        {
          idx_t n_bytes_ഇടത്_in_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          എങ്കിൽ (rem < n_bytes_ഇടത്_in_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          അല്ലെങ്കിൽ
            {
              xwrite_stdout (b[i] + n_read, n_bytes_ഇടത്_in_b_i);
              xwrite_stdout (b[i_next], rem - n_bytes_ഇടത്_in_b_i);
            }
        }
      അല്ലെങ്കിൽ എങ്കിൽ (i + 1 == n_bufs)
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
      വേണ്ടി (i = 0; i < n_alloc; i++)
        സ്വതന്ത്രം (b[i]);
      സ്വതന്ത്രം (b);
    }

  എങ്കിൽ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  തിരികെ ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

സ്ഥിതം bool
elide_tail_bytes_file (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ fd, uintmax_t n_elide,
                       ഘടന stat സ്ഥിരം *st, off_t current_pos)
{
  off_t size = st->st_size;
  എങ്കിൽ (current_pos < 0 || size <= STP_BLKSIZE (st))
    തിരികെ elide_tail_bytes_pipe (filename, fd, n_elide, current_pos);
  അല്ലെങ്കിൽ
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = size - current_pos;
      off_t bytes_remaining = diff < 0 ? 0 : diff;

      എങ്കിൽ (bytes_remaining <= n_elide)
        തിരികെ true;

      എണ്ണൽ Copy_fd_status err = copy_fd (fd, bytes_remaining - n_elide);
      എങ്കിൽ (err == COPY_FD_OK)
        തിരികെ true;

      diagnose_copy_fd_failure (err, filename);
      തിരികെ false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

സ്ഥിതം bool
elide_tail_വരികൾ_pipe (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ fd, uintmax_t n_elide,
                       off_t current_pos)
{
  ഘടന linebuffer
  {
    അക്ഷരം buffer[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    ഘടന linebuffer *next;
  };
  off_t desired_pos = current_pos;
  തരനിർവചനം ഘടന linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  size_t total_വരികൾ = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (വലുപ്പം (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = ശൂന്യ;
  tmp = xmalloc (വലുപ്പം (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  ആകുമ്പോൾ (true)
    {
      n_read = പഠ (fd, tmp->buffer, BUFSIZ);
      എങ്കിൽ (n_read <= 0)
        നിർത്തുക;

      എങ്കിൽ (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->buffer, n_read);
          തുടരുക;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = ശൂന്യ;

      /* Count the number of newlines just read.  */
      {
        അക്ഷരം *buffer_end = tmp->buffer + n_read;
        *buffer_end = വരി_end;
        അക്ഷരം സ്ഥിരം *p = tmp->buffer;
        ആകുമ്പോൾ ((p = rawmemchr (p, വരി_end)) < buffer_end)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_വരികൾ += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      എങ്കിൽ (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      അല്ലെങ്കിൽ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          എങ്കിൽ (n_elide < total_വരികൾ - first->nlines)
            {
              desired_pos += first->nbytes;
              xwrite_stdout (first->buffer, first->nbytes);
              tmp = first;
              total_വരികൾ -= first->nlines;
              first = first->next;
            }
          അല്ലെങ്കിൽ
            tmp = xmalloc (വലുപ്പം (LBUFFER));
        }
    }

  സ്വതന്ത്രം (tmp);

  എങ്കിൽ (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      പോകുക free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  എങ്കിൽ (last->nbytes && last->buffer[last->nbytes - 1] != വരി_end)
    {
      ++last->nlines;
      ++total_വരികൾ;
    }

  വേണ്ടി (tmp = first; n_elide < total_വരികൾ - tmp->nlines; tmp = tmp->next)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->buffer, tmp->nbytes);
      total_വരികൾ -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  എങ്കിൽ (n_elide < total_വരികൾ)
    {
      size_t n = total_വരികൾ - n_elide;
      അക്ഷരം സ്ഥിരം *buffer_end = tmp->buffer + tmp->nbytes;
      അക്ഷരം സ്ഥിരം *p = tmp->buffer;
      ആകുമ്പോൾ (n && (p = memchr (p, വരി_end, buffer_end - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->buffer;
      xwrite_stdout (tmp->buffer, p - tmp->buffer);
    }

free_lbuffers:
  ആകുമ്പോൾ (first)
    {
      tmp = first->next;
      സ്വതന്ത്രം (first);
      first = tmp;
    }

  എങ്കിൽ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  തിരികെ ok;
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
സ്ഥിതം bool
elide_tail_വരികൾ_seekable (അക്ഷരം സ്ഥിരം *pretty_filename, പൂർണ്ണസംഖ്യ fd,
                           uintmax_t n_വരികൾ,
                           off_t start_pos, off_t size)
{
  അക്ഷരം buffer[BUFSIZ];
  ssize_t bytes_read;
  off_t pos = size;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  bytes_read = (pos - start_pos) % BUFSIZ;
  എങ്കിൽ (bytes_read == 0)
    bytes_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= bytes_read;
  എങ്കിൽ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    തിരികെ false;
  bytes_read = പഠ (fd, buffer, bytes_read);
  എങ്കിൽ (bytes_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      തിരികെ false;
    }

  /* n_lines == 0 case needs special treatment. */
  സ്ഥിരം bool all_വരികൾ = !n_വരികൾ;

  /* Count the incomplete line on files that don't end with a newline.  */
  എങ്കിൽ (n_വരികൾ && bytes_read && buffer[bytes_read - 1] != വരി_end)
    --n_വരികൾ;

  ആകുമ്പോൾ (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      ആകുമ്പോൾ (n)
        {
          എങ്കിൽ (all_വരികൾ)
            n -= 1;
          അല്ലെങ്കിൽ
            {
              അക്ഷരം സ്ഥിരം *nl;
              nl = memrchr (buffer, വരി_end, n);
              എങ്കിൽ (nl == ശൂന്യ)
                നിർത്തുക;
              n = nl - buffer;
            }
          എങ്കിൽ (n_വരികൾ-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              എങ്കിൽ (start_pos < pos)
                {
                  എണ്ണൽ Copy_fd_status err;
                  എങ്കിൽ (elseek (fd, start_pos, SEEK_SET, pretty_filename) < 0)
                    തിരികെ false;

                  err = copy_fd (fd, pos - start_pos);
                  എങ്കിൽ (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      തിരികെ false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (buffer, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              തിരികെ 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      എങ്കിൽ (pos == start_pos)
        {
          /* Not enough lines in the file.  */
          തിരികെ true;
        }
      pos -= BUFSIZ;
      എങ്കിൽ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        തിരികെ false;

      bytes_read = പഠ (fd, buffer, BUFSIZ);
      എങ്കിൽ (bytes_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          തിരികെ false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      എങ്കിൽ (bytes_read == 0)
        തിരികെ true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

സ്ഥിതം bool
elide_tail_വരികൾ_file (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ fd, uintmax_t n_elide,
                       ഘടന stat സ്ഥിരം *st, off_t current_pos)
{
  off_t size = st->st_size;
  എങ്കിൽ (current_pos < 0 || size <= STP_BLKSIZE (st))
    തിരികെ elide_tail_വരികൾ_pipe (filename, fd, n_elide, current_pos);
  അല്ലെങ്കിൽ
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      തിരികെ (size <= current_pos
              || elide_tail_വരികൾ_seekable (filename, fd, n_elide,
                                            current_pos, size));
    }
}

സ്ഥിതം bool
head_bytes (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ fd, uintmax_t bytes_to_write)
{
  എങ്കിൽ (copy_fd (fd, bytes_to_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      തിരികെ false;
    }
  തിരികെ true;
}

സ്ഥിതം bool
head_വരികൾ (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ fd, uintmax_t വരികൾ_to_write)
{
  അക്ഷരം buffer[BUFSIZ];

  ആകുമ്പോൾ (വരികൾ_to_write)
    {
      ssize_t bytes_read = പഠ (fd, buffer, BUFSIZ);
      idx_t bytes_to_write = 0;

      എങ്കിൽ (bytes_read < 0)
        {
          diagnose_read_failure (filename);
          തിരികെ false;
        }
      എങ്കിൽ (bytes_read == 0)
        നിർത്തുക;
      ആകുമ്പോൾ (bytes_to_write < bytes_read)
        എങ്കിൽ (buffer[bytes_to_write++] == വരി_end && --വരികൾ_to_write == 0)
          {
            off_t n_bytes_past_EOL = bytes_read - bytes_to_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            എങ്കിൽ (lseek (fd, -n_bytes_past_EOL, SEEK_CUR) < 0)
              {
                ഘടന stat st;
                എങ്കിൽ (fstat (fd, &st) != 0 || S_ISREG (st.st_mode))
                  elseek_diagnostic (-n_bytes_past_EOL, SEEK_CUR, filename);
              }
            നിർത്തുക;
          }
      xwrite_stdout (buffer, bytes_to_write);
    }
  തിരികെ true;
}

സ്ഥിതം bool
head (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ fd, uintmax_t n_units, bool എണ്ണം_വരികൾ,
      bool elide_from_end)
{
  എങ്കിൽ (print_headers)
    write_header (filename);

  എങ്കിൽ (elide_from_end)
    {
      /* Optimize for "infinite" elisions.  */
      എങ്കിൽ (n_units == UINTMAX_MAX)
        തിരികെ true;

      off_t current_pos = -1;
      ഘടന stat st;
      എങ്കിൽ (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          തിരികെ false;
        }
      എങ്കിൽ (! presume_നിവേശം_pipe && S_ISREG (st.st_mode))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          എങ്കിൽ (current_pos < 0)
            തിരികെ false;
        }
      എങ്കിൽ (എണ്ണം_വരികൾ)
        തിരികെ elide_tail_വരികൾ_file (filename, fd, n_units, &st, current_pos);
      അല്ലെങ്കിൽ
        തിരികെ elide_tail_bytes_file (filename, fd, n_units, &st, current_pos);
    }
  എങ്കിൽ (എണ്ണം_വരികൾ)
    തിരികെ head_വരികൾ (filename, fd, n_units);
  അല്ലെങ്കിൽ
    തിരികെ head_bytes (filename, fd, n_units);
}

സ്ഥിതം bool
head_file (അക്ഷരം സ്ഥിരം *filename, uintmax_t n_units, bool എണ്ണം_വരികൾ,
           bool elide_from_end)
{
  പൂർണ്ണസംഖ്യ fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  എങ്കിൽ (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_ദ്വിമാന_mode (STDIN_FILENO, O_ദ്വിമാന);
    }
  അല്ലെങ്കിൽ
    {
      fd = open (filename, O_RDONLY | O_ദ്വിമാന);
      എങ്കിൽ (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          തിരികെ false;
        }
    }

  ok = head (filename, fd, n_units, എണ്ണം_വരികൾ, elide_from_end);
  എങ്കിൽ (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      തിരികെ false;
    }
  തിരികെ ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

സ്ഥിതം uintmax_t
string_to_integer (bool എണ്ണം_വരികൾ, അക്ഷരം സ്ഥിരം *n_string)
{
  തിരികെ xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (എണ്ണം_വരികൾ
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  എണ്ണൽ header_mode header_mode = multiple_files;
  bool ok = true;
  പൂർണ്ണസംഖ്യ c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_സംഖ്യ;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool എണ്ണം_വരികൾ = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_end = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം default_file_list[] = {"-", ശൂന്യ};
  അക്ഷരം സ്ഥിരം *സ്ഥിരം *file_list;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  എങ്കിൽ (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      അക്ഷരം *a = argv[1];
      അക്ഷരം *n_string = ++a;
      അക്ഷരം *end_n_string;
      അക്ഷരം multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      ചെയ്യുക ++a;
      ആകുമ്പോൾ (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      end_n_string = a;

      /* Parse any appended option letters. */
      വേണ്ടി (; *a; a++)
        {
          തിരഞ്ഞെടുപ്പ് (*a)
            {
            അവസ്ഥ 'c':
              എണ്ണം_വരികൾ = false;
              multiplier_char = 0;
              നിർത്തുക;

            അവസ്ഥ 'b':
            അവസ്ഥ 'k':
            അവസ്ഥ 'm':
              എണ്ണം_വരികൾ = false;
              multiplier_char = *a;
              നിർത്തുക;

            അവസ്ഥ 'l':
              എണ്ണം_വരികൾ = true;
              നിർത്തുക;

            അവസ്ഥ 'q':
              header_mode = never;
              നിർത്തുക;

            അവസ്ഥ 'v':
              header_mode = always;
              നിർത്തുക;

            അവസ്ഥ 'z':
              വരി_end = '\0';
              നിർത്തുക;

            സ്ഥിരസ്ഥിതി:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *end_n_string = multiplier_char;
      എങ്കിൽ (multiplier_char)
        *(++end_n_string) = 0;

      n_units = string_to_integer (എണ്ണം_വരികൾ, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ PRESUME_നിവേശം_PIPE_OPTION:
          presume_നിവേശം_pipe = true;
          നിർത്തുക;

        അവസ്ഥ 'c':
          എണ്ണം_വരികൾ = false;
          elide_from_end = (*optarg == '-');
          എങ്കിൽ (elide_from_end)
            ++optarg;
          n_units = string_to_integer (എണ്ണം_വരികൾ, optarg);
          നിർത്തുക;

        അവസ്ഥ 'n':
          എണ്ണം_വരികൾ = true;
          elide_from_end = (*optarg == '-');
          എങ്കിൽ (elide_from_end)
            ++optarg;
          n_units = string_to_integer (എണ്ണം_വരികൾ, optarg);
          നിർത്തുക;

        അവസ്ഥ 'q':
          header_mode = never;
          നിർത്തുക;

        അവസ്ഥ 'v':
          header_mode = always;
          നിർത്തുക;

        അവസ്ഥ 'z':
          വരി_end = '\0';
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          എങ്കിൽ (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (header_mode == always
      || (header_mode == multiple_files && optind < argc - 1))
    print_headers = true;

  file_list = (optind < argc
               ? (അക്ഷരം സ്ഥിരം *സ്ഥിരം *) &argv[optind]
               : default_file_list);

  xset_ദ്വിമാന_mode (STDOUT_FILENO, O_ദ്വിമാന);

  വേണ്ടി (size_t i = 0; file_list[i]; ++i)
    ok &= head_file (file_list[i], n_units, എണ്ണം_വരികൾ, elide_from_end);

  എങ്കിൽ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
