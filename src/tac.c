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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <regex.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "filenamecat.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "full-read.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "full-write.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "temp-stream.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "tac"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Jay Lepreau"), \
  proper_name ("David MacKenzie")


/* The number of bytes per atomic read. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INITIAL_READSIZE 8192

/* The number of bytes per atomic write. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WRITESIZE 8192

/* The string that separates the records of the file. */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *separator = "\n";

/* True if we have ever read standard input.  */
ਸਥਿਰ bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
ਸਥਿਰ bool separator_ਸਿਰੇ_ਰਿਕਾਰਡ = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
ਸਥਿਰ size_t sentinel_ਲੰਮਾਈ = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
ਸਥਿਰ size_t ਮੇਲ_ਲੰਮਾਈ;

/* The input buffer. */
ਸਥਿਰ ਅੱਖਰ *G_ਬਫਰ;

/* The number of bytes to read at once into 'buffer'. */
ਸਥਿਰ size_t read_ਮਾਪ;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
ਸਥਿਰ size_t G_ਬਫਰ_ਮਾਪ;

/* The compiled regular expression representing 'separator'. */
ਸਥਿਰ ਬਣਤਰ re_ਨਮੂਨਾ_ਬਫਰ compiled_separator;
ਸਥਿਰ ਅੱਖਰ compiled_separator_fastmap[UCHAR_MAX + 1];
ਸਥਿਰ ਬਣਤਰ re_registers regs;

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"before", no_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"regex", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"separator", required_ਦਲੀਲ, ਨਿਰਾ, 's'},
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
      fputs (_("\
Write each FILE ਮੰਜ਼ਿਲ standard output, ਆਖਰੀ ਸਤਰ ਪਹਿਲਾ.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --before             attach the separator before instead ਦਾ after\n\
"));
      oputs (_("\
  -r, --regex              interpret the separator as a regular ਵਿਅੰਜਕ_2\n\
"));
      oputs (_("\
  -s, --separator=STRING   use STRING as the separator instead ਦਾ newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\n\
Non-seekable ਨਿਵੇਸ਼ is buffered ਮੰਜ਼ਿਲ $TMPDIR, defaulting ਮੰਜ਼ਿਲ /tmp.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

ਸਥਿਰ ਖਾਲੀ
output (ਅੱਖਰ ਅਚਲ *ਸ਼ੁਰੂ, ਅੱਖਰ ਅਚਲ *past_ਸਿਰਾ)
{
  ਸਥਿਰ ਅੱਖਰ ਬਫਰ[WRITESIZE];
  ਸਥਿਰ size_t ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ = 0;
  size_t ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_ਜੋੜੋ = past_ਸਿਰਾ - ਸ਼ੁਰੂ;
  size_t ਬਾਈਟਾਂ_ਉਪਲਬਧ = WRITESIZE - ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ;

  ਜੇ (!ਸ਼ੁਰੂ)
    {
      ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, ਬਫਰ, ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ)
          != ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ)
        write_error ();
      ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ = 0;
      ਵਾਪਸ;
    }

  /* Write out as many full buffers as possible. */
  ਜਦੋਂ (ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_ਜੋੜੋ >= ਬਾਈਟਾਂ_ਉਪਲਬਧ)
    {
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (ਬਫਰ + ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ, ਸ਼ੁਰੂ, ਬਾਈਟਾਂ_ਉਪਲਬਧ);
      ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_ਜੋੜੋ -= ਬਾਈਟਾਂ_ਉਪਲਬਧ;
      ਸ਼ੁਰੂ += ਬਾਈਟਾਂ_ਉਪਲਬਧ;
      ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, ਬਫਰ, WRITESIZE) != WRITESIZE)
        write_error ();
      ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ = 0;
      ਬਾਈਟਾਂ_ਉਪਲਬਧ = WRITESIZE;
    }

  ਸਮ੍ਰਿਤੀ_ਨਕਲ (ਬਫਰ + ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ, ਸ਼ੁਰੂ, ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_ਜੋੜੋ);
  ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ += ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_ਜੋੜੋ;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

ਸਥਿਰ bool
tac_seekable (ਪੂਰਨ ਨਿਵੇਸ਼_fd, ਅੱਖਰ ਅਚਲ *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  ਅੱਖਰ *ਮੇਲ_ਸ਼ੁਰੂ;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  ਅੱਖਰ *past_ਸਿਰਾ;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_ਰਿਕਾਰਡ_ਮਾਪ;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool ਪਹਿਲਾ_ਸਮਾਂ = true;
  ਅੱਖਰ ਪਹਿਲਾ_char = *separator;	/* Speed optimization, non-regexp. */
  ਅੱਖਰ ਅਚਲ *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t ਮੇਲ_length1 = ਮੇਲ_ਲੰਮਾਈ - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_ਮਾਪ;
  ਜੇ (remainder != 0)
    {
      file_pos -= remainder;
      ਜੇ (lseek (ਨਿਵੇਸ਼_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  ਜਦੋਂ ((saved_ਰਿਕਾਰਡ_ਮਾਪ = ਪਠ (ਨਿਵੇਸ਼_fd, G_ਬਫਰ, read_ਮਾਪ)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_ਮਾਪ;
      ਜੇ (lseek (ਨਿਵੇਸ਼_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_ਮਾਪ;
    }

  /* Now scan forward, looking for end of file.  */
  ਜਦੋਂ (saved_ਰਿਕਾਰਡ_ਮਾਪ == read_ਮਾਪ)
    {
      ssize_t nread = ਪਠ (ਨਿਵੇਸ਼_fd, G_ਬਫਰ, read_ਮਾਪ);
      ਜੇ (nread == 0)
        ਰੋਕੋ;
      saved_ਰਿਕਾਰਡ_ਮਾਪ = nread;
      ਜੇ (saved_ਰਿਕਾਰਡ_ਮਾਪ < 0)
        ਰੋਕੋ;
      file_pos += nread;
    }

  ਜੇ (saved_ਰਿਕਾਰਡ_ਮਾਪ < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      ਵਾਪਸ false;
    }

  ਮੇਲ_ਸ਼ੁਰੂ = past_ਸਿਰਾ = G_ਬਫਰ + saved_ਰਿਕਾਰਡ_ਮਾਪ;
  /* For non-regexp search, move past impossible positions for a match. */
  ਜੇ (sentinel_ਲੰਮਾਈ)
    ਮੇਲ_ਸ਼ੁਰੂ -= ਮੇਲ_length1;

  ਜਦੋਂ (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      ਜੇ (sentinel_ਲੰਮਾਈ == 0)
        {
          size_t i = ਮੇਲ_ਸ਼ੁਰੂ - G_ਬਫਰ;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          ਜੇ (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          ਜੇ (range == 1
              || ((ret = re_ਖੋਜ (&compiled_separator, G_ਬਫਰ,
                                    i, i - 1, range, &regs))
                  == -1))
            ਮੇਲ_ਸ਼ੁਰੂ = G_ਬਫਰ - 1;
          ਨਹੀਂਤਾਂ ਜੇ (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          ਨਹੀਂਤਾਂ
            {
              ਮੇਲ_ਸ਼ੁਰੂ = G_ਬਫਰ + regs.ਸ਼ੁਰੂ[0];
              ਮੇਲ_ਲੰਮਾਈ = regs.ਸਿਰਾ[0] - regs.ਸ਼ੁਰੂ[0];
            }
        }
      ਨਹੀਂਤਾਂ
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          ਜਦੋਂ (*--ਮੇਲ_ਸ਼ੁਰੂ != ਪਹਿਲਾ_char
                 || (ਮੇਲ_length1 && !STREQ_LEN (ਮੇਲ_ਸ਼ੁਰੂ + 1, separator1,
                                                  ਮੇਲ_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      ਜੇ (ਮੇਲ_ਸ਼ੁਰੂ < G_ਬਫਰ)
        {
          ਜੇ (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_ਬਫਰ, past_ਸਿਰਾ);
              ਵਾਪਸ true;
            }

          saved_ਰਿਕਾਰਡ_ਮਾਪ = past_ਸਿਰਾ - G_ਬਫਰ;
          ਜੇ (saved_ਰਿਕਾਰਡ_ਮਾਪ > read_ਮਾਪ)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              ਅੱਖਰ *newbuffer;
              size_t ਵਿੱਥ = sentinel_ਲੰਮਾਈ ? sentinel_ਲੰਮਾਈ : 1;
              size_t old_G_ਬਫਰ_ਮਾਪ = G_ਬਫਰ_ਮਾਪ;

              read_ਮਾਪ *= 2;
              G_ਬਫਰ_ਮਾਪ = read_ਮਾਪ * 2 + sentinel_ਲੰਮਾਈ + 2;
              ਜੇ (G_ਬਫਰ_ਮਾਪ < old_G_ਬਫਰ_ਮਾਪ)
                xalloc_die ();
              newbuffer = xrealloc (G_ਬਫਰ - ਵਿੱਥ, G_ਬਫਰ_ਮਾਪ);
              newbuffer += ਵਿੱਥ;
              G_ਬਫਰ = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          ਜੇ (file_pos >= read_ਮਾਪ)
            file_pos -= read_ਮਾਪ;
          ਨਹੀਂਤਾਂ
            {
              read_ਮਾਪ = file_pos;
              file_pos = 0;
            }
          ਜੇ (lseek (ਨਿਵੇਸ਼_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_ਬਫਰ + read_ਮਾਪ, G_ਬਫਰ, saved_ਰਿਕਾਰਡ_ਮਾਪ);
          past_ਸਿਰਾ = G_ਬਫਰ + read_ਮਾਪ + saved_ਰਿਕਾਰਡ_ਮਾਪ;
          /* For non-regexp searches, avoid unnecessary scanning. */
          ਜੇ (sentinel_ਲੰਮਾਈ)
            ਮੇਲ_ਸ਼ੁਰੂ = G_ਬਫਰ + read_ਮਾਪ;
          ਨਹੀਂਤਾਂ
            ਮੇਲ_ਸ਼ੁਰੂ = past_ਸਿਰਾ;

          ਜੇ (ਭਰਿਆ_read (ਨਿਵੇਸ਼_fd, G_ਬਫਰ, read_ਮਾਪ) != read_ਮਾਪ)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              ਵਾਪਸ false;
            }
        }
      ਨਹੀਂਤਾਂ
        {
          /* Found a match of 'separator'. */
          ਜੇ (separator_ਸਿਰੇ_ਰਿਕਾਰਡ)
            {
              ਅੱਖਰ *ਮੇਲ_ਸਿਰਾ = ਮੇਲ_ਸ਼ੁਰੂ + ਮੇਲ_ਲੰਮਾਈ;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              ਜੇ (!ਪਹਿਲਾ_ਸਮਾਂ || ਮੇਲ_ਸਿਰਾ != past_ਸਿਰਾ)
                output (ਮੇਲ_ਸਿਰਾ, past_ਸਿਰਾ);
              past_ਸਿਰਾ = ਮੇਲ_ਸਿਰਾ;
              ਪਹਿਲਾ_ਸਮਾਂ = false;
            }
          ਨਹੀਂਤਾਂ
            {
              output (ਮੇਲ_ਸ਼ੁਰੂ, past_ਸਿਰਾ);
              past_ਸਿਰਾ = ਮੇਲ_ਸ਼ੁਰੂ;
            }

          /* For non-regex matching, we can back up.  */
          ਜੇ (sentinel_ਲੰਮਾਈ > 0)
            ਮੇਲ_ਸ਼ੁਰੂ -= ਮੇਲ_ਲੰਮਾਈ - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

ਸਥਿਰ off_t
copy_ਮੰਜ਼ਿਲ_ਆਰਜ਼ੀ (FILE **g_tmp, ਅੱਖਰ **g_tempfile, ਪੂਰਨ ਨਿਵੇਸ਼_fd, ਅੱਖਰ ਅਚਲ *file)
{
  FILE *fp;
  ਅੱਖਰ *file_name;
  uintmax_t ਬਾਈਟਾਂ_copied = 0;
  ਜੇ (!ਆਰਜ਼ੀ_ਧਾਰਾ (&fp, &file_name))
    ਵਾਪਸ -1;

  ਜਦੋਂ (true)
    {
      ssize_t ਬਾਈਟਾਂ_read = ਪਠ (ਨਿਵੇਸ਼_fd, G_ਬਫਰ, read_ਮਾਪ);
      ਜੇ (ਬਾਈਟਾਂ_read == 0)
        ਰੋਕੋ;
      ਜੇ (ਬਾਈਟਾਂ_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          ਵਾਪਸ -1;
        }

      ਜੇ (fwrite (G_ਬਫਰ, 1, ਬਾਈਟਾਂ_read, fp) != ਬਾਈਟਾਂ_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          ਵਾਪਸ -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      ਬਾਈਟਾਂ_copied += ਬਾਈਟਾਂ_read;
    }

  ਜੇ (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      ਵਾਪਸ -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  ਵਾਪਸ ਬਾਈਟਾਂ_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

ਸਥਿਰ bool
tac_nonseekable (ਪੂਰਨ ਨਿਵੇਸ਼_fd, ਅੱਖਰ ਅਚਲ *file)
{
  FILE *tmp_ਧਾਰਾ;
  ਅੱਖਰ *tmp_file;
  off_t ਬਾਈਟਾਂ_copied = copy_ਮੰਜ਼ਿਲ_ਆਰਜ਼ੀ (&tmp_ਧਾਰਾ, &tmp_file, ਨਿਵੇਸ਼_fd, file);
  ਜੇ (ਬਾਈਟਾਂ_copied < 0)
    ਵਾਪਸ false;

  bool ok = tac_seekable (fileno (tmp_ਧਾਰਾ), tmp_file, ਬਾਈਟਾਂ_copied);
  ਵਾਪਸ ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

ਸਥਿਰ bool
tac_file (ਅੱਖਰ ਅਚਲ *filename)
{
  bool ok;
  off_t file_ਮਾਪ;
  ਪੂਰਨ fd;
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
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          ਵਾਪਸ false;
        }
    }

  file_ਮਾਪ = lseek (fd, 0, SEEK_END);

  ok = (file_ਮਾਪ < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_ਮਾਪ));

  ਜੇ (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  ਵਾਪਸ ok;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ ਅਚਲ *error_ਸੁਨੇਹਾ_2;	/* Return value from re_compile_pattern. */
  ਪੂਰਨ optc;
  bool ok;
  size_t half_ਬਫਰ_ਮਾਪ;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ default_file_list[] = {"-", ਨਿਰਾ};
  ਅੱਖਰ ਅਚਲ *ਅਚਲ *file;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "brs:", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'b':
          separator_ਸਿਰੇ_ਰਿਕਾਰਡ = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'r':
          sentinel_ਲੰਮਾਈ = 0;
          ਰੋਕੋ;
        ਹਾਲਤ 's':
          separator = optarg;
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (sentinel_ਲੰਮਾਈ == 0)
    {
      ਜੇ (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.ਬਫਰ = ਨਿਰਾ;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = ਨਿਰਾ;
      error_ਸੁਨੇਹਾ_2 = re_compile_ਨਮੂਨਾ (separator, ਲੜੀ_ਲੰਬਾਈ (separator),
                                          &compiled_separator);
      ਜੇ (error_ਸੁਨੇਹਾ_2)
        error (EXIT_FAILURE, 0, "%s", (error_ਸੁਨੇਹਾ_2));
    }
  ਨਹੀਂਤਾਂ
    ਮੇਲ_ਲੰਮਾਈ = sentinel_ਲੰਮਾਈ = *separator ? ਲੜੀ_ਲੰਬਾਈ (separator) : 1;

  read_ਮਾਪ = INITIAL_READSIZE;
  ਜਦੋਂ (sentinel_ਲੰਮਾਈ >= read_ਮਾਪ / 2)
    {
      ਜੇ (SIZE_MAX / 2 < read_ਮਾਪ)
        xalloc_die ();
      read_ਮਾਪ *= 2;
    }
  half_ਬਫਰ_ਮਾਪ = read_ਮਾਪ + sentinel_ਲੰਮਾਈ + 1;
  G_ਬਫਰ_ਮਾਪ = 2 * half_ਬਫਰ_ਮਾਪ;
  ਜੇ (! (read_ਮਾਪ < half_ਬਫਰ_ਮਾਪ && half_ਬਫਰ_ਮਾਪ < G_ਬਫਰ_ਮਾਪ))
    xalloc_die ();
  G_ਬਫਰ = xmalloc (G_ਬਫਰ_ਮਾਪ);
  ਜੇ (sentinel_ਲੰਮਾਈ)
    {
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (G_ਬਫਰ, separator, sentinel_ਲੰਮਾਈ + !!*separator);
      G_ਬਫਰ += sentinel_ਲੰਮਾਈ;
    }
  ਨਹੀਂਤਾਂ
    {
      ++G_ਬਫਰ;
    }

  file = (optind < argc
          ? (ਅੱਖਰ ਅਚਲ *ਅਚਲ *) &argv[optind]
          : default_file_list);

  xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDOUT_FILENO, O_ਦੁਆਧਾਰੀ);

  {
    ok = true;
    ਲਈ (size_t i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((ਅੱਖਰ *) ਨਿਰਾ, (ਅੱਖਰ *) ਨਿਰਾ);

  ਜੇ (have_read_stdin && close (STDIN_FILENO) < 0)
    {
      error (0, errno, "-");
      ok = false;
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
