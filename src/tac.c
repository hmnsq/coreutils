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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <regex.h>

#અન્તર્ભાવ "filenamecat.h"
#અન્તર્ભાવ "full-read.h"
#અન્તર્ભાવ "full-write.h"
#અન્તર્ભાવ "temp-stream.h"
#અન્તર્ભાવ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tac"

#define AUTHORS \
  proper_name ("Jay Lepreau"), \
  proper_name ("David MacKenzie")


/* The number of bytes per atomic read. */
#define INITIAL_READSIZE 8192

/* The number of bytes per atomic write. */
#define WRITESIZE 8192

/* The string that separates the records of the file. */
સ્થિર અક્ષર અચલ *separator = "\n";

/* True if we have ever read standard input.  */
સ્થિર bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
સ્થિર bool separator_ends_record = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
સ્થિર size_t sentinel_length = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
સ્થિર size_t match_length;

/* The input buffer. */
સ્થિર અક્ષર *G_buffer;

/* The number of bytes to read at once into 'buffer'. */
સ્થિર size_t read_size;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
સ્થિર size_t G_buffer_size;

/* The compiled regular expression representing 'separator'. */
સ્થિર સંરચના re_pattern_buffer compiled_separator;
સ્થિર અક્ષર compiled_separator_fastmap[UCHAR_MAX + 1];
સ્થિર સંરચના re_registers regs;

સ્થિર સંરચના option અચલ longopts[] =
{
  {"before", no_argument, ખાલી, 'b'},
  {"regex", no_argument, ખાલી, 'r'},
  {"separator", required_argument, ખાલી, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, last લીટી first.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --before             attach the separator before instead of after\n\
"));
      oputs (_("\
  -r, --regex              interpret the separator as a regular expression\n\
"));
      oputs (_("\
  -s, --separator=STRING   use STRING as the separator instead of newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
Non-seekable આગત is buffered to $TMPDIR, defaulting to /tmp.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

સ્થિર શૂન્ય
output (અક્ષર અચલ *start, અક્ષર અચલ *past_end)
{
  સ્થિર અક્ષર buffer[WRITESIZE];
  સ્થિર size_t bytes_in_buffer = 0;
  size_t bytes_to_add = past_end - start;
  size_t bytes_available = WRITESIZE - bytes_in_buffer;

  જો (!start)
    {
      જો (full_write (STDOUT_FILENO, buffer, bytes_in_buffer)
          != bytes_in_buffer)
        write_error ();
      bytes_in_buffer = 0;
      પાછા;
    }

  /* Write out as many full buffers as possible. */
  જ્યારે (bytes_to_add >= bytes_available)
    {
      memcpy (buffer + bytes_in_buffer, start, bytes_available);
      bytes_to_add -= bytes_available;
      start += bytes_available;
      જો (full_write (STDOUT_FILENO, buffer, WRITESIZE) != WRITESIZE)
        write_error ();
      bytes_in_buffer = 0;
      bytes_available = WRITESIZE;
    }

  memcpy (buffer + bytes_in_buffer, start, bytes_to_add);
  bytes_in_buffer += bytes_to_add;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

સ્થિર bool
tac_seekable (પૂર્ણાંક આગત_fd, અક્ષર અચલ *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  અક્ષર *match_start;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  અક્ષર *past_end;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_record_size;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool first_time = true;
  અક્ષર first_char = *separator;	/* Speed optimization, non-regexp. */
  અક્ષર અચલ *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t match_length1 = match_length - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_size;
  જો (remainder != 0)
    {
      file_pos -= remainder;
      જો (lseek (આગત_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  જ્યારે ((saved_record_size = પઠ (આગત_fd, G_buffer, read_size)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_size;
      જો (lseek (આગત_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_size;
    }

  /* Now scan forward, looking for end of file.  */
  જ્યારે (saved_record_size == read_size)
    {
      ssize_t nread = પઠ (આગત_fd, G_buffer, read_size);
      જો (nread == 0)
        તોડો;
      saved_record_size = nread;
      જો (saved_record_size < 0)
        તોડો;
      file_pos += nread;
    }

  જો (saved_record_size < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      પાછા false;
    }

  match_start = past_end = G_buffer + saved_record_size;
  /* For non-regexp search, move past impossible positions for a match. */
  જો (sentinel_length)
    match_start -= match_length1;

  જ્યારે (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      જો (sentinel_length == 0)
        {
          size_t i = match_start - G_buffer;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          જો (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          જો (range == 1
              || ((ret = re_શોધ (&compiled_separator, G_buffer,
                                    i, i - 1, range, &regs))
                  == -1))
            match_start = G_buffer - 1;
          નહીંતર જો (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          નહીંતર
            {
              match_start = G_buffer + regs.start[0];
              match_length = regs.end[0] - regs.start[0];
            }
        }
      નહીંતર
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          જ્યારે (*--match_start != first_char
                 || (match_length1 && !STREQ_LEN (match_start + 1, separator1,
                                                  match_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      જો (match_start < G_buffer)
        {
          જો (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_buffer, past_end);
              પાછા true;
            }

          saved_record_size = past_end - G_buffer;
          જો (saved_record_size > read_size)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              અક્ષર *newbuffer;
              size_t offset = sentinel_length ? sentinel_length : 1;
              size_t old_G_buffer_size = G_buffer_size;

              read_size *= 2;
              G_buffer_size = read_size * 2 + sentinel_length + 2;
              જો (G_buffer_size < old_G_buffer_size)
                xalloc_die ();
              newbuffer = xrealloc (G_buffer - offset, G_buffer_size);
              newbuffer += offset;
              G_buffer = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          જો (file_pos >= read_size)
            file_pos -= read_size;
          નહીંતર
            {
              read_size = file_pos;
              file_pos = 0;
            }
          જો (lseek (આગત_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_buffer + read_size, G_buffer, saved_record_size);
          past_end = G_buffer + read_size + saved_record_size;
          /* For non-regexp searches, avoid unnecessary scanning. */
          જો (sentinel_length)
            match_start = G_buffer + read_size;
          નહીંતર
            match_start = past_end;

          જો (full_read (આગત_fd, G_buffer, read_size) != read_size)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              પાછા false;
            }
        }
      નહીંતર
        {
          /* Found a match of 'separator'. */
          જો (separator_ends_record)
            {
              અક્ષર *match_end = match_start + match_length;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              જો (!first_time || match_end != past_end)
                output (match_end, past_end);
              past_end = match_end;
              first_time = false;
            }
          નહીંતર
            {
              output (match_start, past_end);
              past_end = match_start;
            }

          /* For non-regex matching, we can back up.  */
          જો (sentinel_length > 0)
            match_start -= match_length - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

સ્થિર off_t
copy_to_temp (FILE **g_tmp, અક્ષર **g_tempfile, પૂર્ણાંક આગત_fd, અક્ષર અચલ *file)
{
  FILE *fp;
  અક્ષર *file_name;
  uintmax_t bytes_copied = 0;
  જો (!temp_stream (&fp, &file_name))
    પાછા -1;

  જ્યારે (true)
    {
      ssize_t bytes_read = પઠ (આગત_fd, G_buffer, read_size);
      જો (bytes_read == 0)
        તોડો;
      જો (bytes_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          પાછા -1;
        }

      જો (fwrite (G_buffer, 1, bytes_read, fp) != bytes_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          પાછા -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      bytes_copied += bytes_read;
    }

  જો (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      પાછા -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  પાછા bytes_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

સ્થિર bool
tac_nonseekable (પૂર્ણાંક આગત_fd, અક્ષર અચલ *file)
{
  FILE *tmp_stream;
  અક્ષર *tmp_file;
  off_t bytes_copied = copy_to_temp (&tmp_stream, &tmp_file, આગત_fd, file);
  જો (bytes_copied < 0)
    પાછા false;

  bool ok = tac_seekable (fileno (tmp_stream), tmp_file, bytes_copied);
  પાછા ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

સ્થિર bool
tac_file (અક્ષર અચલ *filename)
{
  bool ok;
  off_t file_size;
  પૂર્ણાંક fd;
  bool is_stdin = streq (filename, "-");

  જો (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_દ્વિઅંકી_mode (STDIN_FILENO, O_દ્વિઅંકી);
    }
  નહીંતર
    {
      fd = open (filename, O_RDONLY | O_દ્વિઅંકી);
      જો (fd < 0)
        {
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          પાછા false;
        }
    }

  file_size = lseek (fd, 0, SEEK_END);

  ok = (file_size < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_size));

  જો (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  પાછા ok;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  અક્ષર અચલ *error_message;	/* Return value from re_compile_pattern. */
  પૂર્ણાંક optc;
  bool ok;
  size_t half_buffer_size;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  સ્થિર અક્ષર અચલ *અચલ default_file_list[] = {"-", ખાલી};
  અક્ષર અચલ *અચલ *file;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((optc = getopt_long (argc, argv, "brs:", longopts, ખાલી)) != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'b':
          separator_ends_record = false;
          તોડો;
        સ્થિતિ 'r':
          sentinel_length = 0;
          તોડો;
        સ્થિતિ 's':
          separator = optarg;
          તોડો;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (sentinel_length == 0)
    {
      જો (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.buffer = ખાલી;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = ખાલી;
      error_message = re_compile_pattern (separator, strlen (separator),
                                          &compiled_separator);
      જો (error_message)
        error (EXIT_FAILURE, 0, "%s", (error_message));
    }
  નહીંતર
    match_length = sentinel_length = *separator ? strlen (separator) : 1;

  read_size = INITIAL_READSIZE;
  જ્યારે (sentinel_length >= read_size / 2)
    {
      જો (SIZE_MAX / 2 < read_size)
        xalloc_die ();
      read_size *= 2;
    }
  half_buffer_size = read_size + sentinel_length + 1;
  G_buffer_size = 2 * half_buffer_size;
  જો (! (read_size < half_buffer_size && half_buffer_size < G_buffer_size))
    xalloc_die ();
  G_buffer = xmalloc (G_buffer_size);
  જો (sentinel_length)
    {
      memcpy (G_buffer, separator, sentinel_length + !!*separator);
      G_buffer += sentinel_length;
    }
  નહીંતર
    {
      ++G_buffer;
    }

  file = (optind < argc
          ? (અક્ષર અચલ *અચલ *) &argv[optind]
          : default_file_list);

  xset_દ્વિઅંકી_mode (STDOUT_FILENO, O_દ્વિઅંકી);

  {
    ok = true;
    માટે (size_t i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((અક્ષર *) ખાલી, (અક્ષર *) ખાલી);

  જો (have_read_stdin && close (STDIN_FILENO) < 0)
    {
      error (0, errno, "-");
      ok = false;
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
