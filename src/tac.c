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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <regex.h>

#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "full-read.h"
#ಅನ್ತರ್ಭಾವ "full-write.h"
#ಅನ್ತರ್ಭಾವ "temp-stream.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "tac"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Jay Lepreau"), \
  proper_name ("David MacKenzie")


/* The number of bytes per atomic read. */
#ವ್ಯಾಖ್ಯಾನಿಸು INITIAL_READSIZE 8192

/* The number of bytes per atomic write. */
#ವ್ಯಾಖ್ಯಾನಿಸು WRITESIZE 8192

/* The string that separates the records of the file. */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *separator = "\n";

/* True if we have ever read standard input.  */
ಸ್ಥಿರ bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
ಸ್ಥಿರ bool separator_ತುದಿಗಳು_ದಾಖಲೆ = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
ಸ್ಥಿರ size_t sentinel_ಉದ್ದಳತೆ = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
ಸ್ಥಿರ size_t ಹೊಂದಿಕೆ_ಉದ್ದಳತೆ;

/* The input buffer. */
ಸ್ಥಿರ ಅಕ್ಷರ *G_ಬಫರ್;

/* The number of bytes to read at once into 'buffer'. */
ಸ್ಥಿರ size_t read_ಗಾತ್ರ;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
ಸ್ಥಿರ size_t G_ಬಫರ್_ಗಾತ್ರ;

/* The compiled regular expression representing 'separator'. */
ಸ್ಥಿರ ರಚನೆ re_ನಮೂನೆ_ಬಫರ್ compiled_separator;
ಸ್ಥಿರ ಅಕ್ಷರ compiled_separator_fastmap[UCHAR_MAX + 1];
ಸ್ಥಿರ ರಚನೆ re_registers regs;

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"before", no_ವಾದ, ಖಾಲಿ, 'b'},
  {"regex", no_ವಾದ, ಖಾಲಿ, 'r'},
  {"separator", required_ವಾದ, ಖಾಲಿ, 's'},
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
      fputs (_("\
Write each FILE ಗಮ್ಯ standard output, ಕೊನೆಯ ಸಾಲು ಮೊದಲ.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --before             attach the separator before instead ದ after\n\
"));
      oputs (_("\
  -r, --regex              interpret the separator as a regular ಅಭಿವ್ಯಕ್ತಿ_2\n\
"));
      oputs (_("\
  -s, --separator=STRING   use STRING as the separator instead ದ newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\n\
Non-seekable ಒಳಹರಿವು is buffered ಗಮ್ಯ $TMPDIR, defaulting ಗಮ್ಯ /tmp.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

ಸ್ಥಿರ ಶೂನ್ಯ
output (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಆರಂಭ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *past_ಕೊನೆ)
{
  ಸ್ಥಿರ ಅಕ್ಷರ ಬಫರ್[WRITESIZE];
  ಸ್ಥಿರ size_t ಬೈಟುಗಳು_ಒಳ_ಬಫರ್ = 0;
  size_t ಬೈಟುಗಳು_ಗಮ್ಯ_ಕೂಡಿಸು = past_ಕೊನೆ - ಆರಂಭ;
  size_t ಬೈಟುಗಳು_ಲಭ್ಯ = WRITESIZE - ಬೈಟುಗಳು_ಒಳ_ಬಫರ್;

  ಆದರೆ (!ಆರಂಭ)
    {
      ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, ಬಫರ್, ಬೈಟುಗಳು_ಒಳ_ಬಫರ್)
          != ಬೈಟುಗಳು_ಒಳ_ಬಫರ್)
        write_error ();
      ಬೈಟುಗಳು_ಒಳ_ಬಫರ್ = 0;
      ಹಿಂತಿರುಗಿ;
    }

  /* Write out as many full buffers as possible. */
  ಆಗ (ಬೈಟುಗಳು_ಗಮ್ಯ_ಕೂಡಿಸು >= ಬೈಟುಗಳು_ಲಭ್ಯ)
    {
      ಸ್ಮರಣೆ_ನಕಲು (ಬಫರ್ + ಬೈಟುಗಳು_ಒಳ_ಬಫರ್, ಆರಂಭ, ಬೈಟುಗಳು_ಲಭ್ಯ);
      ಬೈಟುಗಳು_ಗಮ್ಯ_ಕೂಡಿಸು -= ಬೈಟುಗಳು_ಲಭ್ಯ;
      ಆರಂಭ += ಬೈಟುಗಳು_ಲಭ್ಯ;
      ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, ಬಫರ್, WRITESIZE) != WRITESIZE)
        write_error ();
      ಬೈಟುಗಳು_ಒಳ_ಬಫರ್ = 0;
      ಬೈಟುಗಳು_ಲಭ್ಯ = WRITESIZE;
    }

  ಸ್ಮರಣೆ_ನಕಲು (ಬಫರ್ + ಬೈಟುಗಳು_ಒಳ_ಬಫರ್, ಆರಂಭ, ಬೈಟುಗಳು_ಗಮ್ಯ_ಕೂಡಿಸು);
  ಬೈಟುಗಳು_ಒಳ_ಬಫರ್ += ಬೈಟುಗಳು_ಗಮ್ಯ_ಕೂಡಿಸು;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

ಸ್ಥಿರ bool
tac_seekable (ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  ಅಕ್ಷರ *ಹೊಂದಿಕೆ_ಆರಂಭ;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  ಅಕ್ಷರ *past_ಕೊನೆ;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_ದಾಖಲೆ_ಗಾತ್ರ;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool ಮೊದಲ_ಸಮಯ = true;
  ಅಕ್ಷರ ಮೊದಲ_char = *separator;	/* Speed optimization, non-regexp. */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t ಹೊಂದಿಕೆ_length1 = ಹೊಂದಿಕೆ_ಉದ್ದಳತೆ - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_ಗಾತ್ರ;
  ಆದರೆ (remainder != 0)
    {
      file_pos -= remainder;
      ಆದರೆ (lseek (ಒಳಹರಿವು_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  ಆಗ ((saved_ದಾಖಲೆ_ಗಾತ್ರ = ಪಠ (ಒಳಹರಿವು_fd, G_ಬಫರ್, read_ಗಾತ್ರ)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_ಗಾತ್ರ;
      ಆದರೆ (lseek (ಒಳಹರಿವು_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_ಗಾತ್ರ;
    }

  /* Now scan forward, looking for end of file.  */
  ಆಗ (saved_ದಾಖಲೆ_ಗಾತ್ರ == read_ಗಾತ್ರ)
    {
      ssize_t nread = ಪಠ (ಒಳಹರಿವು_fd, G_ಬಫರ್, read_ಗಾತ್ರ);
      ಆದರೆ (nread == 0)
        ನಿಲ್ಲಿಸು;
      saved_ದಾಖಲೆ_ಗಾತ್ರ = nread;
      ಆದರೆ (saved_ದಾಖಲೆ_ಗಾತ್ರ < 0)
        ನಿಲ್ಲಿಸು;
      file_pos += nread;
    }

  ಆದರೆ (saved_ದಾಖಲೆ_ಗಾತ್ರ < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      ಹಿಂತಿರುಗಿ false;
    }

  ಹೊಂದಿಕೆ_ಆರಂಭ = past_ಕೊನೆ = G_ಬಫರ್ + saved_ದಾಖಲೆ_ಗಾತ್ರ;
  /* For non-regexp search, move past impossible positions for a match. */
  ಆದರೆ (sentinel_ಉದ್ದಳತೆ)
    ಹೊಂದಿಕೆ_ಆರಂಭ -= ಹೊಂದಿಕೆ_length1;

  ಆಗ (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      ಆದರೆ (sentinel_ಉದ್ದಳತೆ == 0)
        {
          size_t i = ಹೊಂದಿಕೆ_ಆರಂಭ - G_ಬಫರ್;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          ಆದರೆ (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          ಆದರೆ (range == 1
              || ((ret = re_ಹುಡುಕು (&compiled_separator, G_ಬಫರ್,
                                    i, i - 1, range, &regs))
                  == -1))
            ಹೊಂದಿಕೆ_ಆರಂಭ = G_ಬಫರ್ - 1;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಹೊಂದಿಕೆ_ಆರಂಭ = G_ಬಫರ್ + regs.ಆರಂಭ[0];
              ಹೊಂದಿಕೆ_ಉದ್ದಳತೆ = regs.ಕೊನೆ[0] - regs.ಆರಂಭ[0];
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          ಆಗ (*--ಹೊಂದಿಕೆ_ಆರಂಭ != ಮೊದಲ_char
                 || (ಹೊಂದಿಕೆ_length1 && !STREQ_LEN (ಹೊಂದಿಕೆ_ಆರಂಭ + 1, separator1,
                                                  ಹೊಂದಿಕೆ_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      ಆದರೆ (ಹೊಂದಿಕೆ_ಆರಂಭ < G_ಬಫರ್)
        {
          ಆದರೆ (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_ಬಫರ್, past_ಕೊನೆ);
              ಹಿಂತಿರುಗಿ true;
            }

          saved_ದಾಖಲೆ_ಗಾತ್ರ = past_ಕೊನೆ - G_ಬಫರ್;
          ಆದರೆ (saved_ದಾಖಲೆ_ಗಾತ್ರ > read_ಗಾತ್ರ)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              ಅಕ್ಷರ *newbuffer;
              size_t ಸ್ಥಾನಾಂತರ = sentinel_ಉದ್ದಳತೆ ? sentinel_ಉದ್ದಳತೆ : 1;
              size_t old_G_ಬಫರ್_ಗಾತ್ರ = G_ಬಫರ್_ಗಾತ್ರ;

              read_ಗಾತ್ರ *= 2;
              G_ಬಫರ್_ಗಾತ್ರ = read_ಗಾತ್ರ * 2 + sentinel_ಉದ್ದಳತೆ + 2;
              ಆದರೆ (G_ಬಫರ್_ಗಾತ್ರ < old_G_ಬಫರ್_ಗಾತ್ರ)
                xalloc_die ();
              newbuffer = xrealloc (G_ಬಫರ್ - ಸ್ಥಾನಾಂತರ, G_ಬಫರ್_ಗಾತ್ರ);
              newbuffer += ಸ್ಥಾನಾಂತರ;
              G_ಬಫರ್ = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          ಆದರೆ (file_pos >= read_ಗಾತ್ರ)
            file_pos -= read_ಗಾತ್ರ;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              read_ಗಾತ್ರ = file_pos;
              file_pos = 0;
            }
          ಆದರೆ (lseek (ಒಳಹರಿವು_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_ಬಫರ್ + read_ಗಾತ್ರ, G_ಬಫರ್, saved_ದಾಖಲೆ_ಗಾತ್ರ);
          past_ಕೊನೆ = G_ಬಫರ್ + read_ಗಾತ್ರ + saved_ದಾಖಲೆ_ಗಾತ್ರ;
          /* For non-regexp searches, avoid unnecessary scanning. */
          ಆದರೆ (sentinel_ಉದ್ದಳತೆ)
            ಹೊಂದಿಕೆ_ಆರಂಭ = G_ಬಫರ್ + read_ಗಾತ್ರ;
          ಇಲ್ಲದಿದ್ದರೆ
            ಹೊಂದಿಕೆ_ಆರಂಭ = past_ಕೊನೆ;

          ಆದರೆ (ತುಂಬಿದ_read (ಒಳಹರಿವು_fd, G_ಬಫರ್, read_ಗಾತ್ರ) != read_ಗಾತ್ರ)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              ಹಿಂತಿರುಗಿ false;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Found a match of 'separator'. */
          ಆದರೆ (separator_ತುದಿಗಳು_ದಾಖಲೆ)
            {
              ಅಕ್ಷರ *ಹೊಂದಿಕೆ_ಕೊನೆ = ಹೊಂದಿಕೆ_ಆರಂಭ + ಹೊಂದಿಕೆ_ಉದ್ದಳತೆ;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              ಆದರೆ (!ಮೊದಲ_ಸಮಯ || ಹೊಂದಿಕೆ_ಕೊನೆ != past_ಕೊನೆ)
                output (ಹೊಂದಿಕೆ_ಕೊನೆ, past_ಕೊನೆ);
              past_ಕೊನೆ = ಹೊಂದಿಕೆ_ಕೊನೆ;
              ಮೊದಲ_ಸಮಯ = false;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              output (ಹೊಂದಿಕೆ_ಆರಂಭ, past_ಕೊನೆ);
              past_ಕೊನೆ = ಹೊಂದಿಕೆ_ಆರಂಭ;
            }

          /* For non-regex matching, we can back up.  */
          ಆದರೆ (sentinel_ಉದ್ದಳತೆ > 0)
            ಹೊಂದಿಕೆ_ಆರಂಭ -= ಹೊಂದಿಕೆ_ಉದ್ದಳತೆ - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

ಸ್ಥಿರ off_t
copy_ಗಮ್ಯ_ತಾತ್ಕಾಲಿಕ (FILE **g_tmp, ಅಕ್ಷರ **g_tempfile, ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  FILE *fp;
  ಅಕ್ಷರ *file_name;
  uintmax_t ಬೈಟುಗಳು_copied = 0;
  ಆದರೆ (!ತಾತ್ಕಾಲಿಕ_ಹರಿವು (&fp, &file_name))
    ಹಿಂತಿರುಗಿ -1;

  ಆಗ (true)
    {
      ssize_t ಬೈಟುಗಳು_read = ಪಠ (ಒಳಹರಿವು_fd, G_ಬಫರ್, read_ಗಾತ್ರ);
      ಆದರೆ (ಬೈಟುಗಳು_read == 0)
        ನಿಲ್ಲಿಸು;
      ಆದರೆ (ಬೈಟುಗಳು_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          ಹಿಂತಿರುಗಿ -1;
        }

      ಆದರೆ (fwrite (G_ಬಫರ್, 1, ಬೈಟುಗಳು_read, fp) != ಬೈಟುಗಳು_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          ಹಿಂತಿರುಗಿ -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      ಬೈಟುಗಳು_copied += ಬೈಟುಗಳು_read;
    }

  ಆದರೆ (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      ಹಿಂತಿರುಗಿ -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  ಹಿಂತಿರುಗಿ ಬೈಟುಗಳು_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

ಸ್ಥಿರ bool
tac_nonseekable (ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  FILE *tmp_ಹರಿವು;
  ಅಕ್ಷರ *tmp_file;
  off_t ಬೈಟುಗಳು_copied = copy_ಗಮ್ಯ_ತಾತ್ಕಾಲಿಕ (&tmp_ಹರಿವು, &tmp_file, ಒಳಹರಿವು_fd, file);
  ಆದರೆ (ಬೈಟುಗಳು_copied < 0)
    ಹಿಂತಿರುಗಿ false;

  bool ok = tac_seekable (fileno (tmp_ಹರಿವು), tmp_file, ಬೈಟುಗಳು_copied);
  ಹಿಂತಿರುಗಿ ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

ಸ್ಥಿರ bool
tac_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  bool ok;
  off_t file_ಗಾತ್ರ;
  ಪೂರ್ಣಾಂಕ fd;
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
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          ಹಿಂತಿರುಗಿ false;
        }
    }

  file_ಗಾತ್ರ = lseek (fd, 0, SEEK_END);

  ok = (file_ಗಾತ್ರ < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_ಗಾತ್ರ));

  ಆದರೆ (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  ಹಿಂತಿರುಗಿ ok;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *error_ಸಂದೇಶ_2;	/* Return value from re_compile_pattern. */
  ಪೂರ್ಣಾಂಕ optc;
  bool ok;
  size_t half_ಬಫರ್_ಗಾತ್ರ;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ default_file_list[] = {"-", ಖಾಲಿ};
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ *file;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, "brs:", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'b':
          separator_ತುದಿಗಳು_ದಾಖಲೆ = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'r':
          sentinel_ಉದ್ದಳತೆ = 0;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 's':
          separator = optarg;
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (sentinel_ಉದ್ದಳತೆ == 0)
    {
      ಆದರೆ (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.ಬಫರ್ = ಖಾಲಿ;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = ಖಾಲಿ;
      error_ಸಂದೇಶ_2 = re_compile_ನಮೂನೆ (separator, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (separator),
                                          &compiled_separator);
      ಆದರೆ (error_ಸಂದೇಶ_2)
        error (EXIT_FAILURE, 0, "%s", (error_ಸಂದೇಶ_2));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹೊಂದಿಕೆ_ಉದ್ದಳತೆ = sentinel_ಉದ್ದಳತೆ = *separator ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (separator) : 1;

  read_ಗಾತ್ರ = INITIAL_READSIZE;
  ಆಗ (sentinel_ಉದ್ದಳತೆ >= read_ಗಾತ್ರ / 2)
    {
      ಆದರೆ (SIZE_MAX / 2 < read_ಗಾತ್ರ)
        xalloc_die ();
      read_ಗಾತ್ರ *= 2;
    }
  half_ಬಫರ್_ಗಾತ್ರ = read_ಗಾತ್ರ + sentinel_ಉದ್ದಳತೆ + 1;
  G_ಬಫರ್_ಗಾತ್ರ = 2 * half_ಬಫರ್_ಗಾತ್ರ;
  ಆದರೆ (! (read_ಗಾತ್ರ < half_ಬಫರ್_ಗಾತ್ರ && half_ಬಫರ್_ಗಾತ್ರ < G_ಬಫರ್_ಗಾತ್ರ))
    xalloc_die ();
  G_ಬಫರ್ = xmalloc (G_ಬಫರ್_ಗಾತ್ರ);
  ಆದರೆ (sentinel_ಉದ್ದಳತೆ)
    {
      ಸ್ಮರಣೆ_ನಕಲು (G_ಬಫರ್, separator, sentinel_ಉದ್ದಳತೆ + !!*separator);
      G_ಬಫರ್ += sentinel_ಉದ್ದಳತೆ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ++G_ಬಫರ್;
    }

  file = (optind < argc
          ? (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ *) &argv[optind]
          : default_file_list);

  xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDOUT_FILENO, O_ದ್ವಿಮಾನ);

  {
    ok = true;
    ಗಾಗಿ (size_t i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((ಅಕ್ಷರ *) ಖಾಲಿ, (ಅಕ್ಷರ *) ಖಾಲಿ);

  ಆದರೆ (have_read_stdin && close (STDIN_FILENO) < 0)
    {
      error (0, errno, "-");
      ok = false;
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
