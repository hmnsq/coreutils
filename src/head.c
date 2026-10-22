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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"

#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "full-read.h"
#అన్తర్భావ "stat-size.h"
#అన్తర్భావ "xbinary-io.h"
#అన్తర్భావ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "head"

#నిర్వచించు AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Number of lines/chars/blocks to head. */
#నిర్వచించు DEFAULT_సంఖ్య 10

/* Useful only when eliding tail bytes or lines.
   If true, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
స్థిర bool presume_ఆదానం_pipe;

/* If true, print filename headers. */
స్థిర bool print_శీర్షికలు;

/* Character to split lines by. */
స్థిర అక్షరం పంక్తి_చివర = '\n';

/* When to print the filename banners. */
గణన శీర్షిక_బహుళకం
{
  గుణిజం_files, always, never
};

/* Have we ever read standard input?  */
స్థిర bool have_read_stdin;

గణన Copy_fd_స్థితి
  {
    COPY_FD_OK = 0,
    COPY_FD_READ_ERROR,
    COPY_FD_UNEXPECTED_EOF
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  PRESUME_ఆదానం_PIPE_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"bytes", required_వాదం, ఖాళీ, 'c'},
  {"lines", required_వాదం, ఖాళీ, 'n'},
  {"-presume-input-pipe", no_వాదం, ఖాళీ,
   PRESUME_ఆదానం_PIPE_OPTION}, /* do not document */
  {"quiet", no_వాదం, ఖాళీ, 'q'},
  {"silent", no_వాదం, ఖాళీ, 'q'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {"zero-terminated", no_వాదం, ఖాళీ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              కార్యక్రమం_name);
      రాయి (_("\
Print the మొదటి %d పంక్తులు యొక్క each FILE గమ్యం standard output.\n\
With మరిన్ని than one FILE, precede each with a శీర్షిక giving the file name.\n\
"), DEFAULT_సంఖ్య);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --బైట్లు=[-]అంకె_2\n\
         print the మొదటి అంకె_2 బైట్లు యొక్క each file;\n\
         with the leading '-', print all but the చివరి అంకె_2 బైట్లు యొక్క each file\n\
"));
      oprintf (_("\
  -n, --పంక్తులు=[-]అంకె_2\n\
         print the మొదటి అంకె_2 పంక్తులు instead యొక్క the మొదటి %d;\n\
         with the leading '-', print all but the చివరి అంకె_2 పంక్తులు యొక్క each file\n\
"), DEFAULT_సంఖ్య);
      oputs (_("\
  -q, --quiet, --silent\n\
         never print శీర్షికలు giving file పేర్లు\n\
"));
      oputs (_("\
  -v, --verbose\n\
         always print శీర్షికలు giving file పేర్లు\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         పంక్తి delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
అంకె_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on కొరకు T, P, E, Z, Y, R, Q.\n\
ద్విమాన_2 prefixes can be వాడిన, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

స్థిర శూన్య
diagnose_read_failure (అక్షరం స్థిరరాశి *filename)
{
  error (0, errno, _("error reading %s"), quoteaf (filename));
}

స్థిర శూన్య
diagnose_copy_fd_failure (గణన Copy_fd_స్థితి err, అక్షరం స్థిరరాశి *filename)
{
  ఎంపిక (err)
    {
    స్థితి COPY_FD_READ_ERROR:
      diagnose_read_failure (filename);
      ఆపు;
    స్థితి COPY_FD_UNEXPECTED_EOF:
      error (0, errno, _("%s: file has shrunk too much"), quotef (filename));
      ఆపు;
    స్థితి COPY_FD_OK: అప్రమేయ:
      affirm (false);
    }
}

స్థిర శూన్య
write_శీర్షిక (అక్షరం స్థిరరాశి *filename)
{
  స్థిర bool మొదటి_file = true;

  రాయి ("%s==> %s <==\n", (మొదటి_file ? "" : "\n"), quotef (filename));
  మొదటి_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

స్థిర శూన్య
xwrite_stdout (అక్షరం స్థిరరాశి *బఫరు, size_t n_బైట్లు)
{
  ఒకవేళ (n_బైట్లు > 0 && fwrite (బఫరు, 1, n_బైట్లు, stdout) < n_బైట్లు)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      fpurge (stdout);
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Copy no more than N_BYTES from file descriptor SRC_FD to stdout.
   Return an appropriate indication of success or read failure.  */

స్థిర గణన Copy_fd_స్థితి
copy_fd (పూర్ణాంకం src_fd, uintmax_t n_బైట్లు)
{
  అక్షరం buf[BUFSIZ];

  /* Copy the file contents.  */
  అయితే (0 < n_బైట్లు)
    {
      idx_t n_గమ్యం_read = MIN (n_బైట్లు, పరిమాణం buf);
      ssize_t n_read = పఠ (src_fd, buf, n_గమ్యం_read);
      ఒకవేళ (n_read < 0)
        తిరిగి COPY_FD_READ_ERROR;

      n_బైట్లు -= n_read;

      ఒకవేళ (n_read == 0 && n_బైట్లు != 0)
        తిరిగి COPY_FD_UNEXPECTED_EOF;

      xwrite_stdout (buf, n_read);
    }

  తిరిగి COPY_FD_OK;
}

/* Report an lseek failure at OFFSET compared to WHENCE, for FILENAME.  */
స్థిర శూన్య
elseek_diagnostic (off_t స్థానభ్రంశం, పూర్ణాంకం whence, అక్షరం స్థిరరాశి *filename)
{
  intmax_t off = స్థానభ్రంశం;
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

స్థిర off_t
elseek (పూర్ణాంకం fd, off_t స్థానభ్రంశం, పూర్ణాంకం whence, అక్షరం స్థిరరాశి *filename)
{
  off_t new_స్థానభ్రంశం = lseek (fd, స్థానభ్రంశం, whence);

  ఒకవేళ (new_స్థానభ్రంశం < 0)
    elseek_diagnostic (స్థానభ్రంశం, whence, filename);

  తిరిగి new_స్థానభ్రంశం;
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, assume that the input file is
   positioned at CURRENT_POS and that it should be repositioned to
   just before the elided bytes before returning.
   Return true upon success.
   Give a diagnostic and return false upon error.  */
స్థిర bool
elide_తోక_బైట్లు_pipe (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం fd, uintmax_t n_elide,
                       off_t current_pos)
{
  uintmax_t desired_pos = current_pos;
  bool ok = true;

#నిర్వచితం_కాకపోతే తల_తోక_PIPE_READ_BUFSIZE
# define తల_తోక_PIPE_READ_BUFSIZE BUFSIZ
#అయితే_ముగింపు
#నిర్వచించు READ_BUFSIZE తల_తోక_PIPE_READ_BUFSIZE

  /* If we're eliding no more than this many bytes, then it's ok to allocate
     more memory in order to use a more time-efficient algorithm.
     FIXME: use a fraction of available memory instead, as in sort.
     FIXME: is this even worthwhile?  */
#నిర్వచితం_కాకపోతే తల_తోక_PIPE_BYTECOUNT_పరిమితి
# define తల_తోక_PIPE_BYTECOUNT_పరిమితి 1024 * 1024
#అయితే_ముగింపు

#ఒకవేళ తల_తోక_PIPE_BYTECOUNT_పరిమితి < 2 * READ_BUFSIZE
# error "HEAD_TAIL_PIPE_BYTECOUNT_THRESHOLD must be at least 2 * READ_BUFSIZE"
#అయితే_ముగింపు

  /* Two cases to consider...
     1) n_elide is small enough that we can afford to double-buffer:
        allocate 2 * (READ_BUFSIZE + n_elide) bytes
     2) n_elide is too big for that, so we allocate only
        (READ_BUFSIZE + n_elide) bytes

     FIXME: profile, to see if double-buffering is worthwhile

     CAUTION: do not fail (out of memory) when asked to elide
     a ridiculous amount, but when given only a small input.  */

  static_assert (READ_BUFSIZE <= IDX_MAX);
  static_assert (తల_తోక_PIPE_BYTECOUNT_పరిమితి <= IDX_MAX - READ_BUFSIZE);
  ఒకవేళ (n_elide <= తల_తోక_PIPE_BYTECOUNT_పరిమితి)
    {
      idx_t లోపల_elide = n_elide;
      bool మొదటి = true;
      bool eof = false;
      idx_t n_గమ్యం_read = READ_BUFSIZE + n_elide;
      అక్షరం *b[2];
      b[0] = xnmalloc (2, n_గమ్యం_read);
      b[1] = b[0] + n_గమ్యం_read;

      కొరకు (bool i = false; ! eof ; i = !i)
        {
          idx_t n_read = నిండిన_read (fd, b[i], n_గమ్యం_read);
          idx_t delta = 0;
          ఒకవేళ (n_read < n_గమ్యం_read)
            {
              ఒకవేళ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  ఆపు;
                }

              /* reached EOF */
              ఒకవేళ (n_read <= లోపల_elide)
                {
                  ఒకవేళ (మొదటి)
                    {
                      /* The input is no larger than the number of bytes
                         to elide.  So there's nothing to output, and
                         we're done.  */
                    }
                  లేకపోతే
                    {
                      delta = లోపల_elide - n_read;
                    }
                }
              eof = true;
            }

          /* Output any (but maybe just part of the) elided data from
             the previous round.  */
          ఒకవేళ (! మొదటి)
            {
              desired_pos += లోపల_elide - delta;
              xwrite_stdout (b[!i] + READ_BUFSIZE, లోపల_elide - delta);
            }
          మొదటి = false;

          ఒకవేళ (లోపల_elide < n_read)
            {
              desired_pos += n_read - లోపల_elide;
              xwrite_stdout (b[i], n_read - లోపల_elide);
            }
        }

      విడుదల (b[0]);
    }
  లేకపోతే
    {
      /* Read blocks of size READ_BUFSIZE, until we've read at least n_elide
         bytes.  Then, for each new buffer we read, also write an old one.  */

      bool eof = false;
      idx_t n_read;
      అక్షరం **b = ఖాళీ;

      idx_t remainder = n_elide % READ_BUFSIZE;
      /* The number of buffers needed to hold n_elide bytes plus one
         extra buffer.  They are allocated lazily, so don't report
         overflow now simply because the number does not fit into idx_t.  */
      uintmax_t n_bufs = n_elide / READ_BUFSIZE + (remainder != 0) + 1;
      idx_t n_alloc = 0;
      idx_t n_array_alloc = 0;

      bool buffered_enough = false;
      idx_t i, i_తదుపరి;
      కొరకు (i = 0, i_తదుపరి = 1; !eof; i = i_తదుపరి, i_తదుపరి = (i_తదుపరి + 1) % n_bufs)
        {
          ఒకవేళ (n_array_alloc == i)
            b = xpalloc (b, &n_array_alloc, 1, MIN (n_bufs, PTRDIFF_MAX),
                         పరిమాణం *b);

          ఒకవేళ (! buffered_enough)
            {
              b[i] = xmalloc (READ_BUFSIZE);
              n_alloc = i + 1;
            }
          n_read = నిండిన_read (fd, b[i], READ_BUFSIZE);
          ఒకవేళ (n_read < READ_BUFSIZE)
            {
              ఒకవేళ (errno != 0)
                {
                  diagnose_read_failure (filename);
                  ok = false;
                  వెళ్ళు free_mem;
                }
              eof = true;
            }

          ఒకవేళ (i + 1 == n_bufs)
            buffered_enough = true;

          ఒకవేళ (buffered_enough)
            {
              desired_pos += n_read;
              xwrite_stdout (b[i_తదుపరి], n_read);
            }
        }

      /* Output the remainder: rem bytes from b[i] + n_read.  */
      idx_t rem = READ_BUFSIZE - remainder;
      ఒకవేళ (buffered_enough)
        {
          idx_t n_బైట్లు_ఎడమ_లోపల_b_i = READ_BUFSIZE - n_read;
          desired_pos += rem;
          ఒకవేళ (rem < n_బైట్లు_ఎడమ_లోపల_b_i)
            {
              xwrite_stdout (b[i] + n_read, rem);
            }
          లేకపోతే
            {
              xwrite_stdout (b[i] + n_read, n_బైట్లు_ఎడమ_లోపల_b_i);
              xwrite_stdout (b[i_తదుపరి], rem - n_బైట్లు_ఎడమ_లోపల_b_i);
            }
        }
      లేకపోతే ఒకవేళ (i + 1 == n_bufs)
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
          xwrite_stdout (b[i_తదుపరి], x);
        }

    free_mem:
      కొరకు (i = 0; i < n_alloc; i++)
        విడుదల (b[i]);
      విడుదల (b);
    }

  ఒకవేళ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  తిరిగి ok;
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   bytes.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

/* NOTE: if the input file shrinks by more than N_ELIDE bytes between
   the length determination and the actual reading, then head fails.  */

స్థిర bool
elide_తోక_బైట్లు_file (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం fd, uintmax_t n_elide,
                       నిర్మాణం stat స్థిరరాశి *st, off_t current_pos)
{
  off_t కొలత = st->st_కొలత;
  ఒకవేళ (current_pos < 0 || కొలత <= STP_BLKSIZE (st))
    తిరిగి elide_తోక_బైట్లు_pipe (filename, fd, n_elide, current_pos);
  లేకపోతే
    {
      /* Be careful here.  The current position may actually be
         beyond the end of the file.  */
      off_t diff = కొలత - current_pos;
      off_t బైట్లు_మిగిలిన = diff < 0 ? 0 : diff;

      ఒకవేళ (బైట్లు_మిగిలిన <= n_elide)
        తిరిగి true;

      గణన Copy_fd_స్థితి err = copy_fd (fd, బైట్లు_మిగిలిన - n_elide);
      ఒకవేళ (err == COPY_FD_OK)
        తిరిగి true;

      diagnose_copy_fd_failure (err, filename);
      తిరిగి false;
    }
}

/* For an input file with name FILENAME and descriptor FD,
   output all but the last N_ELIDE bytes.
   If CURRENT_POS is nonnegative, the input file is positioned there
   and should be repositioned to just before the elided bytes.
   Buffer the specified number of lines as a linked list of LBUFFERs,
   adding them as needed.  Return true if successful.  */

స్థిర bool
elide_తోక_పంక్తులు_pipe (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం fd, uintmax_t n_elide,
                       off_t current_pos)
{
  నిర్మాణం linebuffer
  {
    అక్షరం బఫరు[BUFSIZ + 1];
    size_t nbytes;
    size_t nlines;
    నిర్మాణం linebuffer *తదుపరి;
  };
  off_t desired_pos = current_pos;
  రకనిర్వచనం నిర్మాణం linebuffer LBUFFER;
  LBUFFER *మొదటి, *చివరి, *tmp;
  size_t మొత్తం_పంక్తులు = 0;	/* Total number of newlines in all buffers.  */
  bool ok = true;
  ssize_t n_read;		/* Size in bytes of most recent read */

  మొదటి = చివరి = xmalloc (పరిమాణం (LBUFFER));
  మొదటి->nbytes = మొదటి->nlines = 0;
  మొదటి->తదుపరి = ఖాళీ;
  tmp = xmalloc (పరిమాణం (LBUFFER));

  /* Always read into a fresh buffer.
     Read, (producing no output) until we've accumulated at least
     n_elide newlines, or until EOF, whichever comes first.  */
  అయితే (true)
    {
      n_read = పఠ (fd, tmp->బఫరు, BUFSIZ);
      ఒకవేళ (n_read <= 0)
        ఆపు;

      ఒకవేళ (! n_elide)
        {
          desired_pos += n_read;
          xwrite_stdout (tmp->బఫరు, n_read);
          కొనసాగు;
        }

      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->తదుపరి = ఖాళీ;

      /* Count the number of newlines just read.  */
      {
        అక్షరం *బఫరు_చివర = tmp->బఫరు + n_read;
        *బఫరు_చివర = పంక్తి_చివర;
        అక్షరం స్థిరరాశి *p = tmp->బఫరు;
        అయితే ((p = rawmemchr (p, పంక్తి_చివర)) < బఫరు_చివర)
          {
            ++p;
            ++tmp->nlines;
          }
      }
      మొత్తం_పంక్తులు += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      ఒకవేళ (tmp->nbytes + చివరి->nbytes < BUFSIZ)
        {
          జ్ఞాపకం_నకలు (&చివరి->బఫరు[చివరి->nbytes], tmp->బఫరు, tmp->nbytes);
          చివరి->nbytes += tmp->nbytes;
          చివరి->nlines += tmp->nlines;
        }
      లేకపోతే
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          చివరి = చివరి->తదుపరి = tmp;
          ఒకవేళ (n_elide < మొత్తం_పంక్తులు - మొదటి->nlines)
            {
              desired_pos += మొదటి->nbytes;
              xwrite_stdout (మొదటి->బఫరు, మొదటి->nbytes);
              tmp = మొదటి;
              మొత్తం_పంక్తులు -= మొదటి->nlines;
              మొదటి = మొదటి->తదుపరి;
            }
          లేకపోతే
            tmp = xmalloc (పరిమాణం (LBUFFER));
        }
    }

  విడుదల (tmp);

  ఒకవేళ (n_read < 0)
    {
      diagnose_read_failure (filename);
      ok = false;
      వెళ్ళు free_lbuffers;
    }

  /* If we read any bytes at all, count the incomplete line
     on files that don't end with a newline.  */
  ఒకవేళ (చివరి->nbytes && చివరి->బఫరు[చివరి->nbytes - 1] != పంక్తి_చివర)
    {
      ++చివరి->nlines;
      ++మొత్తం_పంక్తులు;
    }

  కొరకు (tmp = మొదటి; n_elide < మొత్తం_పంక్తులు - tmp->nlines; tmp = tmp->తదుపరి)
    {
      desired_pos += tmp->nbytes;
      xwrite_stdout (tmp->బఫరు, tmp->nbytes);
      మొత్తం_పంక్తులు -= tmp->nlines;
    }

  /* Print the first 'total_lines - n_elide' lines of tmp->buffer.  */
  ఒకవేళ (n_elide < మొత్తం_పంక్తులు)
    {
      size_t n = మొత్తం_పంక్తులు - n_elide;
      అక్షరం స్థిరరాశి *బఫరు_చివర = tmp->బఫరు + tmp->nbytes;
      అక్షరం స్థిరరాశి *p = tmp->బఫరు;
      అయితే (n && (p = memchr (p, పంక్తి_చివర, బఫరు_చివర - p)))
        {
          ++p;
          ++tmp->nlines;
          --n;
        }
      desired_pos += p - tmp->బఫరు;
      xwrite_stdout (tmp->బఫరు, p - tmp->బఫరు);
    }

free_lbuffers:
  అయితే (మొదటి)
    {
      tmp = మొదటి->తదుపరి;
      విడుదల (మొదటి);
      మొదటి = tmp;
    }

  ఒకవేళ (0 <= current_pos && elseek (fd, desired_pos, SEEK_SET, filename) < 0)
    ok = false;
  తిరిగి ok;
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
స్థిర bool
elide_తోక_పంక్తులు_seekable (అక్షరం స్థిరరాశి *pretty_filename, పూర్ణాంకం fd,
                           uintmax_t n_పంక్తులు,
                           off_t ప్రారంభం_pos, off_t కొలత)
{
  అక్షరం బఫరు[BUFSIZ];
  ssize_t బైట్లు_read;
  off_t pos = కొలత;

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'BUFSIZ'.  */
  బైట్లు_read = (pos - ప్రారంభం_pos) % BUFSIZ;
  ఒకవేళ (బైట్లు_read == 0)
    బైట్లు_read = BUFSIZ;
  /* Make 'pos' a multiple of 'BUFSIZ' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos -= బైట్లు_read;
  ఒకవేళ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
    తిరిగి false;
  బైట్లు_read = పఠ (fd, బఫరు, బైట్లు_read);
  ఒకవేళ (బైట్లు_read < 0)
    {
      diagnose_read_failure (pretty_filename);
      తిరిగి false;
    }

  /* n_lines == 0 case needs special treatment. */
  స్థిరరాశి bool all_పంక్తులు = !n_పంక్తులు;

  /* Count the incomplete line on files that don't end with a newline.  */
  ఒకవేళ (n_పంక్తులు && బైట్లు_read && బఫరు[బైట్లు_read - 1] != పంక్తి_చివర)
    --n_పంక్తులు;

  అయితే (true)
    {
      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = బైట్లు_read;
      అయితే (n)
        {
          ఒకవేళ (all_పంక్తులు)
            n -= 1;
          లేకపోతే
            {
              అక్షరం స్థిరరాశి *nl;
              nl = memrchr (బఫరు, పంక్తి_చివర, n);
              ఒకవేళ (nl == ఖాళీ)
                ఆపు;
              n = nl - బఫరు;
            }
          ఒకవేళ (n_పంక్తులు-- == 0)
            {
              /* Found it.  */
              /* If necessary, restore the file pointer and copy
                 input to output up to position, POS.  */
              ఒకవేళ (ప్రారంభం_pos < pos)
                {
                  గణన Copy_fd_స్థితి err;
                  ఒకవేళ (elseek (fd, ప్రారంభం_pos, SEEK_SET, pretty_filename) < 0)
                    తిరిగి false;

                  err = copy_fd (fd, pos - ప్రారంభం_pos);
                  ఒకవేళ (err != COPY_FD_OK)
                    {
                      diagnose_copy_fd_failure (err, pretty_filename);
                      తిరిగి false;
                    }
                }

              /* Output the initial portion of the buffer
                 in which we found the desired newline byte.  */
              xwrite_stdout (బఫరు, n + 1);

              /* Set file pointer to the byte after what we've output.  */
              తిరిగి 0 <= elseek (fd, pos + n + 1, SEEK_SET, pretty_filename);
            }
        }

      /* Not enough newlines in that bufferfull.  */
      ఒకవేళ (pos == ప్రారంభం_pos)
        {
          /* Not enough lines in the file.  */
          తిరిగి true;
        }
      pos -= BUFSIZ;
      ఒకవేళ (elseek (fd, pos, SEEK_SET, pretty_filename) < 0)
        తిరిగి false;

      బైట్లు_read = పఠ (fd, బఫరు, BUFSIZ);
      ఒకవేళ (బైట్లు_read < 0)
        {
          diagnose_read_failure (pretty_filename);
          తిరిగి false;
        }

      /* FIXME: is this dead code?
         Consider the test, pos == start_pos, above. */
      ఒకవేళ (బైట్లు_read == 0)
        తిరిగి true;
    }
}

/* For the file FILENAME with descriptor FD, output all but the last N_ELIDE
   lines.  If CURRENT_POS is nonnegative, this is a regular file positioned
   at CURRENT_POS.  The file's status is ST.  Return true on success.
   Give a diagnostic and return false upon error.  */

స్థిర bool
elide_తోక_పంక్తులు_file (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం fd, uintmax_t n_elide,
                       నిర్మాణం stat స్థిరరాశి *st, off_t current_pos)
{
  off_t కొలత = st->st_కొలత;
  ఒకవేళ (current_pos < 0 || కొలత <= STP_BLKSIZE (st))
    తిరిగి elide_తోక_పంక్తులు_pipe (filename, fd, n_elide, current_pos);
  లేకపోతే
    {
      /* Find the offset, OFF, of the Nth newline from the end,
         but not counting the last byte of the file.
         If found, write from current position to OFF, inclusive.
         Otherwise, just return true.  */

      తిరిగి (కొలత <= current_pos
              || elide_తోక_పంక్తులు_seekable (filename, fd, n_elide,
                                            current_pos, కొలత));
    }
}

స్థిర bool
తల_బైట్లు (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం fd, uintmax_t బైట్లు_గమ్యం_write)
{
  ఒకవేళ (copy_fd (fd, బైట్లు_గమ్యం_write) == COPY_FD_READ_ERROR)
    {
      diagnose_read_failure (filename);
      తిరిగి false;
    }
  తిరిగి true;
}

స్థిర bool
తల_పంక్తులు (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం fd, uintmax_t పంక్తులు_గమ్యం_write)
{
  అక్షరం బఫరు[BUFSIZ];

  అయితే (పంక్తులు_గమ్యం_write)
    {
      ssize_t బైట్లు_read = పఠ (fd, బఫరు, BUFSIZ);
      idx_t బైట్లు_గమ్యం_write = 0;

      ఒకవేళ (బైట్లు_read < 0)
        {
          diagnose_read_failure (filename);
          తిరిగి false;
        }
      ఒకవేళ (బైట్లు_read == 0)
        ఆపు;
      అయితే (బైట్లు_గమ్యం_write < బైట్లు_read)
        ఒకవేళ (బఫరు[బైట్లు_గమ్యం_write++] == పంక్తి_చివర && --పంక్తులు_గమ్యం_write == 0)
          {
            off_t n_బైట్లు_past_EOL = బైట్లు_read - బైట్లు_గమ్యం_write;
            /* If we have read more data than that on the specified number
               of lines, try to seek back to the position we would have
               gotten to had we been reading one byte at a time.  */
            ఒకవేళ (lseek (fd, -n_బైట్లు_past_EOL, SEEK_CUR) < 0)
              {
                నిర్మాణం stat st;
                ఒకవేళ (fstat (fd, &st) != 0 || S_ISREG (st.st_బహుళకం))
                  elseek_diagnostic (-n_బైట్లు_past_EOL, SEEK_CUR, filename);
              }
            ఆపు;
          }
      xwrite_stdout (బఫరు, బైట్లు_గమ్యం_write);
    }
  తిరిగి true;
}

స్థిర bool
తల (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం fd, uintmax_t n_units, bool లెక్క_పంక్తులు,
      bool elide_from_చివర)
{
  ఒకవేళ (print_శీర్షికలు)
    write_శీర్షిక (filename);

  ఒకవేళ (elide_from_చివర)
    {
      /* Optimize for "infinite" elisions.  */
      ఒకవేళ (n_units == UINTMAX_MAX)
        తిరిగి true;

      off_t current_pos = -1;
      నిర్మాణం stat st;
      ఒకవేళ (fstat (fd, &st) != 0)
        {
          error (0, errno, _("cannot fstat %s"),
                 quoteaf (filename));
          తిరిగి false;
        }
      ఒకవేళ (! presume_ఆదానం_pipe && S_ISREG (st.st_బహుళకం))
        {
          current_pos = elseek (fd, 0, SEEK_CUR, filename);
          ఒకవేళ (current_pos < 0)
            తిరిగి false;
        }
      ఒకవేళ (లెక్క_పంక్తులు)
        తిరిగి elide_తోక_పంక్తులు_file (filename, fd, n_units, &st, current_pos);
      లేకపోతే
        తిరిగి elide_తోక_బైట్లు_file (filename, fd, n_units, &st, current_pos);
    }
  ఒకవేళ (లెక్క_పంక్తులు)
    తిరిగి తల_పంక్తులు (filename, fd, n_units);
  లేకపోతే
    తిరిగి తల_బైట్లు (filename, fd, n_units);
}

స్థిర bool
తల_file (అక్షరం స్థిరరాశి *filename, uintmax_t n_units, bool లెక్క_పంక్తులు,
           bool elide_from_చివర)
{
  పూర్ణాంకం fd;
  bool ok;
  bool is_stdin = streq (filename, "-");

  ఒకవేళ (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_ద్విమాన_బహుళకం (STDIN_FILENO, O_ద్విమాన);
    }
  లేకపోతే
    {
      fd = open (filename, O_RDONLY | O_ద్విమాన);
      ఒకవేళ (fd < 0)
        {
          error (0, errno, _("cannot open %s for reading"), quoteaf (filename));
          తిరిగి false;
        }
    }

  ok = తల (filename, fd, n_units, లెక్క_పంక్తులు, elide_from_చివర);
  ఒకవేళ (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (filename));
      తిరిగి false;
    }
  తిరిగి ok;
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Upon successful conversion,
   return that value.  If it cannot be converted, give a diagnostic and exit.
   If it is too large, silently return UINTMAX_MAX.
   COUNT_LINES indicates whether N_STRING is a number of bytes or a number
   of lines.  It is used solely to give a more specific diagnostic.  */

స్థిర uintmax_t
string_గమ్యం_integer (bool లెక్క_పంక్తులు, అక్షరం స్థిరరాశి *n_string)
{
  తిరిగి xnumtoumax (n_string, 10, 0, UINTMAX_MAX, "bkKmMGTPEZYRQ0",
                     (లెక్క_పంక్తులు
                      ? _("invalid number of lines")
                      : _("invalid number of bytes")),
                     0, XTOINT_MAX_QUIET);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  గణన శీర్షిక_బహుళకం శీర్షిక_బహుళకం = గుణిజం_files;
  bool ok = true;
  పూర్ణాంకం c;

  /* Number of items to output, or to elide from the end.
     UINTMAX_MAX stands for an essentially unlimited number.  */
  uintmax_t n_units = DEFAULT_సంఖ్య;

  /* If true, interpret the numeric argument as the number of lines.
     Otherwise, interpret it as the number of bytes.  */
  bool లెక్క_పంక్తులు = true;

  /* Elide the specified number of lines or bytes, counting from
     the end of the file.  */
  bool elide_from_చివర = false;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి default_file_list[] = {"-", ఖాళీ};
  అక్షరం స్థిరరాశి *స్థిరరాశి *file_list;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ఒకవేళ (1 < argc && argv[1][0] == '-' && c_isdigit (argv[1][1]))
    {
      అక్షరం *a = argv[1];
      అక్షరం *n_string = ++a;
      అక్షరం *చివర_n_string;
      అక్షరం multiplier_char = 0;

      /* Old option syntax; a dash, one or more digits, and one or
         more option letters.  Move past the number. */
      చేయి ++a;
      అయితే (c_isdigit (*a));

      /* Pointer to the byte after the last digit.  */
      చివర_n_string = a;

      /* Parse any appended option letters. */
      కొరకు (; *a; a++)
        {
          ఎంపిక (*a)
            {
            స్థితి 'c':
              లెక్క_పంక్తులు = false;
              multiplier_char = 0;
              ఆపు;

            స్థితి 'b':
            స్థితి 'k':
            స్థితి 'm':
              లెక్క_పంక్తులు = false;
              multiplier_char = *a;
              ఆపు;

            స్థితి 'l':
              లెక్క_పంక్తులు = true;
              ఆపు;

            స్థితి 'q':
              శీర్షిక_బహుళకం = never;
              ఆపు;

            స్థితి 'v':
              శీర్షిక_బహుళకం = always;
              ఆపు;

            స్థితి 'z':
              పంక్తి_చివర = '\0';
              ఆపు;

            అప్రమేయ:
              error (0, 0, _("invalid trailing option -- %c"), *a);
              usage (EXIT_FAILURE);
            }
        }

      /* Append the multiplier character (if any) onto the end of
         the digit string.  Then add NUL byte if necessary.  */
      *చివర_n_string = multiplier_char;
      ఒకవేళ (multiplier_char)
        *(++చివర_n_string) = 0;

      n_units = string_గమ్యం_integer (లెక్క_పంక్తులు, n_string);

      /* Make the options we just parsed invisible to getopt. */
      argv[1] = argv[0];
      argv++;
      argc--;
    }

  అయితే ((c = getopt_long (argc, argv, "c:n:qvz0123456789",
                           long_options, ఖాళీ))
         != -1)
    {
      ఎంపిక (c)
        {
        స్థితి PRESUME_ఆదానం_PIPE_OPTION:
          presume_ఆదానం_pipe = true;
          ఆపు;

        స్థితి 'c':
          లెక్క_పంక్తులు = false;
          elide_from_చివర = (*optarg == '-');
          ఒకవేళ (elide_from_చివర)
            ++optarg;
          n_units = string_గమ్యం_integer (లెక్క_పంక్తులు, optarg);
          ఆపు;

        స్థితి 'n':
          లెక్క_పంక్తులు = true;
          elide_from_చివర = (*optarg == '-');
          ఒకవేళ (elide_from_చివర)
            ++optarg;
          n_units = string_గమ్యం_integer (లెక్క_పంక్తులు, optarg);
          ఆపు;

        స్థితి 'q':
          శీర్షిక_బహుళకం = never;
          ఆపు;

        స్థితి 'v':
          శీర్షిక_బహుళకం = always;
          ఆపు;

        స్థితి 'z':
          పంక్తి_చివర = '\0';
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          ఒకవేళ (c_isdigit (c))
            error (0, 0, _("invalid trailing option -- %c"), c);
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (శీర్షిక_బహుళకం == always
      || (శీర్షిక_బహుళకం == గుణిజం_files && optind < argc - 1))
    print_శీర్షికలు = true;

  file_list = (optind < argc
               ? (అక్షరం స్థిరరాశి *స్థిరరాశి *) &argv[optind]
               : default_file_list);

  xset_ద్విమాన_బహుళకం (STDOUT_FILENO, O_ద్విమాన);

  కొరకు (size_t i = 0; file_list[i]; ++i)
    ok &= తల_file (file_list[i], n_units, లెక్క_పంక్తులు, elide_from_చివర);

  ఒకవేళ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
