/* tac - concatenate and print files in reverse
   Copyright (C) 1988-2026 Free Software Foundation, Inc.

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

/* Written by Jay Lepreau (lepreau@cs.utah.edu).
   GNU enhancements by David MacKenzie (djm@gnu.ai.mit.edu). */

/* Copy each FILE, or the standard input if none are given or when a
   FILE name of "-" is encountered, to the standard output with the
   order of the records reversed.  The records are separated by
   instances of a string, or a newline if none is given.  By default, the
   separator string is attached to the end of the record that it
   follows in the file.

   Options:
   -b, --before			The separator is attached to the beginning
                                of the record that it precedes in the file.
   -r, --regex			The separator is a regular expression.
   -s, --separator=separator	Use SEPARATOR as the record separator.

   To reverse a file byte by byte, use (in bash, ksh, or sh):
tac -r -s '.\|
' file */

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"

#అన్తర్భావ <regex.h>

#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "full-read.h"
#అన్తర్భావ "full-write.h"
#అన్తర్భావ "temp-stream.h"
#అన్తర్భావ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "tac"

#నిర్వచించు AUTHORS \
  proper_name ("Jay Lepreau"), \
  proper_name ("David MacKenzie")


/* The number of bytes per atomic read. */
#నిర్వచించు INITIAL_READSIZE 8192

/* The number of bytes per atomic write. */
#నిర్వచించు WRITESIZE 8192

/* The string that separates the records of the file. */
స్థిర అక్షరం స్థిరరాశి *separator = "\n";

/* True if we have ever read standard input.  */
స్థిర bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
స్థిర bool separator_చివర్లు_రికార్డు = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
స్థిర size_t sentinel_నిడివి = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
స్థిర size_t సరిపోలిక_నిడివి;

/* The input buffer. */
స్థిర అక్షరం *G_బఫరు;

/* The number of bytes to read at once into 'buffer'. */
స్థిర size_t read_కొలత;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
స్థిర size_t G_బఫరు_కొలత;

/* The compiled regular expression representing 'separator'. */
స్థిర నిర్మాణం re_నమూనా_బఫరు compiled_separator;
స్థిర అక్షరం compiled_separator_fastmap[UCHAR_MAX + 1];
స్థిర నిర్మాణం re_registers regs;

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"before", no_వాదం, ఖాళీ, 'b'},
  {"regex", no_వాదం, ఖాళీ, 'r'},
  {"separator", required_వాదం, ఖాళీ, 's'},
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
      fputs (_("\
Write each FILE గమ్యం standard output, చివరి పంక్తి మొదటి.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --before             attach the separator before instead యొక్క after\n\
"));
      oputs (_("\
  -r, --regex              interpret the separator as a regular వ్యక్తీకరణ_2\n\
"));
      oputs (_("\
  -s, --separator=STRING   use STRING as the separator instead యొక్క newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\n\
Non-seekable ఆదానం is buffered గమ్యం $TMPDIR, defaulting గమ్యం /tmp.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

స్థిర శూన్య
output (అక్షరం స్థిరరాశి *ప్రారంభం, అక్షరం స్థిరరాశి *past_చివర)
{
  స్థిర అక్షరం బఫరు[WRITESIZE];
  స్థిర size_t బైట్లు_లోపల_బఫరు = 0;
  size_t బైట్లు_గమ్యం_కూడు = past_చివర - ప్రారంభం;
  size_t బైట్లు_అందుబాటు = WRITESIZE - బైట్లు_లోపల_బఫరు;

  ఒకవేళ (!ప్రారంభం)
    {
      ఒకవేళ (నిండిన_write (STDOUT_FILENO, బఫరు, బైట్లు_లోపల_బఫరు)
          != బైట్లు_లోపల_బఫరు)
        write_error ();
      బైట్లు_లోపల_బఫరు = 0;
      తిరిగి;
    }

  /* Write out as many full buffers as possible. */
  అయితే (బైట్లు_గమ్యం_కూడు >= బైట్లు_అందుబాటు)
    {
      జ్ఞాపకం_నకలు (బఫరు + బైట్లు_లోపల_బఫరు, ప్రారంభం, బైట్లు_అందుబాటు);
      బైట్లు_గమ్యం_కూడు -= బైట్లు_అందుబాటు;
      ప్రారంభం += బైట్లు_అందుబాటు;
      ఒకవేళ (నిండిన_write (STDOUT_FILENO, బఫరు, WRITESIZE) != WRITESIZE)
        write_error ();
      బైట్లు_లోపల_బఫరు = 0;
      బైట్లు_అందుబాటు = WRITESIZE;
    }

  జ్ఞాపకం_నకలు (బఫరు + బైట్లు_లోపల_బఫరు, ప్రారంభం, బైట్లు_గమ్యం_కూడు);
  బైట్లు_లోపల_బఫరు += బైట్లు_గమ్యం_కూడు;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

స్థిర bool
tac_seekable (పూర్ణాంకం ఆదానం_fd, అక్షరం స్థిరరాశి *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  అక్షరం *సరిపోలిక_ప్రారంభం;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  అక్షరం *past_చివర;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_రికార్డు_కొలత;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool మొదటి_సమయం = true;
  అక్షరం మొదటి_char = *separator;	/* Speed optimization, non-regexp. */
  అక్షరం స్థిరరాశి *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t సరిపోలిక_length1 = సరిపోలిక_నిడివి - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_కొలత;
  ఒకవేళ (remainder != 0)
    {
      file_pos -= remainder;
      ఒకవేళ (lseek (ఆదానం_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  అయితే ((saved_రికార్డు_కొలత = పఠ (ఆదానం_fd, G_బఫరు, read_కొలత)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_కొలత;
      ఒకవేళ (lseek (ఆదానం_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_కొలత;
    }

  /* Now scan forward, looking for end of file.  */
  అయితే (saved_రికార్డు_కొలత == read_కొలత)
    {
      ssize_t nread = పఠ (ఆదానం_fd, G_బఫరు, read_కొలత);
      ఒకవేళ (nread == 0)
        ఆపు;
      saved_రికార్డు_కొలత = nread;
      ఒకవేళ (saved_రికార్డు_కొలత < 0)
        ఆపు;
      file_pos += nread;
    }

  ఒకవేళ (saved_రికార్డు_కొలత < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      తిరిగి false;
    }

  సరిపోలిక_ప్రారంభం = past_చివర = G_బఫరు + saved_రికార్డు_కొలత;
  /* For non-regexp search, move past impossible positions for a match. */
  ఒకవేళ (sentinel_నిడివి)
    సరిపోలిక_ప్రారంభం -= సరిపోలిక_length1;

  అయితే (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      ఒకవేళ (sentinel_నిడివి == 0)
        {
          size_t i = సరిపోలిక_ప్రారంభం - G_బఫరు;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          ఒకవేళ (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          ఒకవేళ (range == 1
              || ((ret = re_శోధన (&compiled_separator, G_బఫరు,
                                    i, i - 1, range, &regs))
                  == -1))
            సరిపోలిక_ప్రారంభం = G_బఫరు - 1;
          లేకపోతే ఒకవేళ (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          లేకపోతే
            {
              సరిపోలిక_ప్రారంభం = G_బఫరు + regs.ప్రారంభం[0];
              సరిపోలిక_నిడివి = regs.చివర[0] - regs.ప్రారంభం[0];
            }
        }
      లేకపోతే
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          అయితే (*--సరిపోలిక_ప్రారంభం != మొదటి_char
                 || (సరిపోలిక_length1 && !STREQ_LEN (సరిపోలిక_ప్రారంభం + 1, separator1,
                                                  సరిపోలిక_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      ఒకవేళ (సరిపోలిక_ప్రారంభం < G_బఫరు)
        {
          ఒకవేళ (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_బఫరు, past_చివర);
              తిరిగి true;
            }

          saved_రికార్డు_కొలత = past_చివర - G_బఫరు;
          ఒకవేళ (saved_రికార్డు_కొలత > read_కొలత)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              అక్షరం *newbuffer;
              size_t స్థానభ్రంశం = sentinel_నిడివి ? sentinel_నిడివి : 1;
              size_t old_G_బఫరు_కొలత = G_బఫరు_కొలత;

              read_కొలత *= 2;
              G_బఫరు_కొలత = read_కొలత * 2 + sentinel_నిడివి + 2;
              ఒకవేళ (G_బఫరు_కొలత < old_G_బఫరు_కొలత)
                xalloc_die ();
              newbuffer = xrealloc (G_బఫరు - స్థానభ్రంశం, G_బఫరు_కొలత);
              newbuffer += స్థానభ్రంశం;
              G_బఫరు = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          ఒకవేళ (file_pos >= read_కొలత)
            file_pos -= read_కొలత;
          లేకపోతే
            {
              read_కొలత = file_pos;
              file_pos = 0;
            }
          ఒకవేళ (lseek (ఆదానం_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_బఫరు + read_కొలత, G_బఫరు, saved_రికార్డు_కొలత);
          past_చివర = G_బఫరు + read_కొలత + saved_రికార్డు_కొలత;
          /* For non-regexp searches, avoid unnecessary scanning. */
          ఒకవేళ (sentinel_నిడివి)
            సరిపోలిక_ప్రారంభం = G_బఫరు + read_కొలత;
          లేకపోతే
            సరిపోలిక_ప్రారంభం = past_చివర;

          ఒకవేళ (నిండిన_read (ఆదానం_fd, G_బఫరు, read_కొలత) != read_కొలత)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              తిరిగి false;
            }
        }
      లేకపోతే
        {
          /* Found a match of 'separator'. */
          ఒకవేళ (separator_చివర్లు_రికార్డు)
            {
              అక్షరం *సరిపోలిక_చివర = సరిపోలిక_ప్రారంభం + సరిపోలిక_నిడివి;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              ఒకవేళ (!మొదటి_సమయం || సరిపోలిక_చివర != past_చివర)
                output (సరిపోలిక_చివర, past_చివర);
              past_చివర = సరిపోలిక_చివర;
              మొదటి_సమయం = false;
            }
          లేకపోతే
            {
              output (సరిపోలిక_ప్రారంభం, past_చివర);
              past_చివర = సరిపోలిక_ప్రారంభం;
            }

          /* For non-regex matching, we can back up.  */
          ఒకవేళ (sentinel_నిడివి > 0)
            సరిపోలిక_ప్రారంభం -= సరిపోలిక_నిడివి - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

స్థిర off_t
copy_గమ్యం_తాత్కాలిక (FILE **g_tmp, అక్షరం **g_tempfile, పూర్ణాంకం ఆదానం_fd, అక్షరం స్థిరరాశి *file)
{
  FILE *fp;
  అక్షరం *file_name;
  uintmax_t బైట్లు_copied = 0;
  ఒకవేళ (!తాత్కాలిక_ప్రవాహం (&fp, &file_name))
    తిరిగి -1;

  అయితే (true)
    {
      ssize_t బైట్లు_read = పఠ (ఆదానం_fd, G_బఫరు, read_కొలత);
      ఒకవేళ (బైట్లు_read == 0)
        ఆపు;
      ఒకవేళ (బైట్లు_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          తిరిగి -1;
        }

      ఒకవేళ (fwrite (G_బఫరు, 1, బైట్లు_read, fp) != బైట్లు_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          తిరిగి -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      బైట్లు_copied += బైట్లు_read;
    }

  ఒకవేళ (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      తిరిగి -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  తిరిగి బైట్లు_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

స్థిర bool
tac_nonseekable (పూర్ణాంకం ఆదానం_fd, అక్షరం స్థిరరాశి *file)
{
  FILE *tmp_ప్రవాహం;
  అక్షరం *tmp_file;
  off_t బైట్లు_copied = copy_గమ్యం_తాత్కాలిక (&tmp_ప్రవాహం, &tmp_file, ఆదానం_fd, file);
  ఒకవేళ (బైట్లు_copied < 0)
    తిరిగి false;

  bool ok = tac_seekable (fileno (tmp_ప్రవాహం), tmp_file, బైట్లు_copied);
  తిరిగి ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

స్థిర bool
tac_file (అక్షరం స్థిరరాశి *filename)
{
  bool ok;
  off_t file_కొలత;
  పూర్ణాంకం fd;
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
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          తిరిగి false;
        }
    }

  file_కొలత = lseek (fd, 0, SEEK_END);

  ok = (file_కొలత < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_కొలత));

  ఒకవేళ (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  తిరిగి ok;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం స్థిరరాశి *error_సందేశం_2;	/* Return value from re_compile_pattern. */
  పూర్ణాంకం optc;
  bool ok;
  size_t half_బఫరు_కొలత;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి default_file_list[] = {"-", ఖాళీ};
  అక్షరం స్థిరరాశి *స్థిరరాశి *file;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "brs:", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'b':
          separator_చివర్లు_రికార్డు = false;
          ఆపు;
        స్థితి 'r':
          sentinel_నిడివి = 0;
          ఆపు;
        స్థితి 's':
          separator = optarg;
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (sentinel_నిడివి == 0)
    {
      ఒకవేళ (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.బఫరు = ఖాళీ;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = ఖాళీ;
      error_సందేశం_2 = re_compile_నమూనా (separator, అక్షరశ్రేణి_పొడవు (separator),
                                          &compiled_separator);
      ఒకవేళ (error_సందేశం_2)
        error (EXIT_FAILURE, 0, "%s", (error_సందేశం_2));
    }
  లేకపోతే
    సరిపోలిక_నిడివి = sentinel_నిడివి = *separator ? అక్షరశ్రేణి_పొడవు (separator) : 1;

  read_కొలత = INITIAL_READSIZE;
  అయితే (sentinel_నిడివి >= read_కొలత / 2)
    {
      ఒకవేళ (SIZE_MAX / 2 < read_కొలత)
        xalloc_die ();
      read_కొలత *= 2;
    }
  half_బఫరు_కొలత = read_కొలత + sentinel_నిడివి + 1;
  G_బఫరు_కొలత = 2 * half_బఫరు_కొలత;
  ఒకవేళ (! (read_కొలత < half_బఫరు_కొలత && half_బఫరు_కొలత < G_బఫరు_కొలత))
    xalloc_die ();
  G_బఫరు = xmalloc (G_బఫరు_కొలత);
  ఒకవేళ (sentinel_నిడివి)
    {
      జ్ఞాపకం_నకలు (G_బఫరు, separator, sentinel_నిడివి + !!*separator);
      G_బఫరు += sentinel_నిడివి;
    }
  లేకపోతే
    {
      ++G_బఫరు;
    }

  file = (optind < argc
          ? (అక్షరం స్థిరరాశి *స్థిరరాశి *) &argv[optind]
          : default_file_list);

  xset_ద్విమాన_బహుళకం (STDOUT_FILENO, O_ద్విమాన);

  {
    ok = true;
    కొరకు (size_t i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((అక్షరం *) ఖాళీ, (అక్షరం *) ఖాళీ);

  ఒకవేళ (have_read_stdin && close (STDIN_FILENO) < 0)
    {
      error (0, errno, "-");
      ok = false;
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
