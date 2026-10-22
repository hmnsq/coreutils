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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن "system.h"

#ضمن <regex.h>

#ضمن "filenamecat.h"
#ضمن "full-read.h"
#ضمن "full-write.h"
#ضمن "temp-stream.h"
#ضمن "xbinary-io.h"

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
ساكن حرف ثابت *separator = "\n";

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
ساكن bool separator_ends_record = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
ساكن size_t sentinel_length = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
ساكن size_t match_length;

/* The input buffer. */
ساكن حرف *G_buffer;

/* The number of bytes to read at once into 'buffer'. */
ساكن size_t read_size;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
ساكن size_t G_buffer_size;

/* The compiled regular expression representing 'separator'. */
ساكن بنية re_pattern_buffer compiled_separator;
ساكن حرف compiled_separator_fastmap[UCHAR_MAX + 1];
ساكن بنية re_registers regs;

ساكن بنية option ثابت longopts[] =
{
  {"before", no_argument, عدم, 'b'},
  {"regex", no_argument, عدم, 'r'},
  {"separator", required_argument, عدم, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  𐤀𐤌 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, last line first.\n\
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
Non-seekable input is buffered to $TMPDIR, defaulting to /tmp.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

ساكن فراغ
output (حرف ثابت *start, حرف ثابت *past_end)
{
  ساكن حرف buffer[WRITESIZE];
  ساكن size_t bytes_in_buffer = 0;
  size_t bytes_to_add = past_end - start;
  size_t bytes_available = WRITESIZE - bytes_in_buffer;

  𐤀𐤌 (!start)
    {
      𐤀𐤌 (full_write (STDOUT_FILENO, buffer, bytes_in_buffer)
          != bytes_in_buffer)
        write_error ();
      bytes_in_buffer = 0;
      أرجع;
    }

  /* Write out as many full buffers as possible. */
  طالما (bytes_to_add >= bytes_available)
    {
      memcpy (buffer + bytes_in_buffer, start, bytes_available);
      bytes_to_add -= bytes_available;
      start += bytes_available;
      𐤀𐤌 (full_write (STDOUT_FILENO, buffer, WRITESIZE) != WRITESIZE)
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

ساكن bool
tac_seekable (صحيح input_fd, حرف ثابت *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  حرف *match_start;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  حرف *past_end;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_record_size;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool first_time = true;
  حرف first_char = *separator;	/* Speed optimization, non-regexp. */
  حرف ثابت *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t match_length1 = match_length - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_size;
  𐤀𐤌 (remainder != 0)
    {
      file_pos -= remainder;
      𐤀𐤌 (lseek (input_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  طالما ((saved_record_size = اقرأ (input_fd, G_buffer, read_size)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_size;
      𐤀𐤌 (lseek (input_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_size;
    }

  /* Now scan forward, looking for end of file.  */
  طالما (saved_record_size == read_size)
    {
      ssize_t nread = اقرأ (input_fd, G_buffer, read_size);
      𐤀𐤌 (nread == 0)
        اكسر;
      saved_record_size = nread;
      𐤀𐤌 (saved_record_size < 0)
        اكسر;
      file_pos += nread;
    }

  𐤀𐤌 (saved_record_size < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      أرجع false;
    }

  match_start = past_end = G_buffer + saved_record_size;
  /* For non-regexp search, move past impossible positions for a match. */
  𐤀𐤌 (sentinel_length)
    match_start -= match_length1;

  طالما (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      𐤀𐤌 (sentinel_length == 0)
        {
          size_t i = match_start - G_buffer;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          𐤀𐤌 (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          𐤀𐤌 (range == 1
              || ((ret = re_search (&compiled_separator, G_buffer,
                                    i, i - 1, range, &regs))
                  == -1))
            match_start = G_buffer - 1;
          وإلا 𐤀𐤌 (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          وإلا
            {
              match_start = G_buffer + regs.start[0];
              match_length = regs.end[0] - regs.start[0];
            }
        }
      وإلا
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          طالما (*--match_start != first_char
                 || (match_length1 && !STREQ_LEN (match_start + 1, separator1,
                                                  match_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      𐤀𐤌 (match_start < G_buffer)
        {
          𐤀𐤌 (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_buffer, past_end);
              أرجع true;
            }

          saved_record_size = past_end - G_buffer;
          𐤀𐤌 (saved_record_size > read_size)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              حرف *newbuffer;
              size_t offset = sentinel_length ? sentinel_length : 1;
              size_t old_G_buffer_size = G_buffer_size;

              read_size *= 2;
              G_buffer_size = read_size * 2 + sentinel_length + 2;
              𐤀𐤌 (G_buffer_size < old_G_buffer_size)
                xalloc_die ();
              newbuffer = xrealloc (G_buffer - offset, G_buffer_size);
              newbuffer += offset;
              G_buffer = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          𐤀𐤌 (file_pos >= read_size)
            file_pos -= read_size;
          وإلا
            {
              read_size = file_pos;
              file_pos = 0;
            }
          𐤀𐤌 (lseek (input_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_buffer + read_size, G_buffer, saved_record_size);
          past_end = G_buffer + read_size + saved_record_size;
          /* For non-regexp searches, avoid unnecessary scanning. */
          𐤀𐤌 (sentinel_length)
            match_start = G_buffer + read_size;
          وإلا
            match_start = past_end;

          𐤀𐤌 (full_read (input_fd, G_buffer, read_size) != read_size)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              أرجع false;
            }
        }
      وإلا
        {
          /* Found a match of 'separator'. */
          𐤀𐤌 (separator_ends_record)
            {
              حرف *match_end = match_start + match_length;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              𐤀𐤌 (!first_time || match_end != past_end)
                output (match_end, past_end);
              past_end = match_end;
              first_time = false;
            }
          وإلا
            {
              output (match_start, past_end);
              past_end = match_start;
            }

          /* For non-regex matching, we can back up.  */
          𐤀𐤌 (sentinel_length > 0)
            match_start -= match_length - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

ساكن off_t
copy_to_temp (FILE **g_tmp, حرف **g_tempfile, صحيح input_fd, حرف ثابت *file)
{
  FILE *fp;
  حرف *file_name;
  uintmax_t bytes_copied = 0;
  𐤀𐤌 (!temp_stream (&fp, &file_name))
    أرجع -1;

  طالما (true)
    {
      ssize_t bytes_read = اقرأ (input_fd, G_buffer, read_size);
      𐤀𐤌 (bytes_read == 0)
        اكسر;
      𐤀𐤌 (bytes_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          أرجع -1;
        }

      𐤀𐤌 (fwrite (G_buffer, 1, bytes_read, fp) != bytes_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          أرجع -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      bytes_copied += bytes_read;
    }

  𐤀𐤌 (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      أرجع -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  أرجع bytes_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

ساكن bool
tac_nonseekable (صحيح input_fd, حرف ثابت *file)
{
  FILE *tmp_stream;
  حرف *tmp_file;
  off_t bytes_copied = copy_to_temp (&tmp_stream, &tmp_file, input_fd, file);
  𐤀𐤌 (bytes_copied < 0)
    أرجع false;

  bool ok = tac_seekable (fileno (tmp_stream), tmp_file, bytes_copied);
  أرجع ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

ساكن bool
tac_file (حرف ثابت *filename)
{
  bool ok;
  off_t file_size;
  صحيح fd;
  bool is_stdin = streq (filename, "-");

  𐤀𐤌 (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_binary_mode (STDIN_FILENO, O_BINARY);
    }
  وإلا
    {
      fd = open (filename, O_RDONLY | O_BINARY);
      𐤀𐤌 (fd < 0)
        {
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          أرجع false;
        }
    }

  file_size = lseek (fd, 0, SEEK_END);

  ok = (file_size < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_size));

  𐤀𐤌 (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  أرجع ok;
}

صحيح
main (صحيح argc, حرف **argv)
{
  حرف ثابت *error_message;	/* Return value from re_compile_pattern. */
  صحيح optc;
  bool ok;
  size_t half_buffer_size;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ساكن حرف ثابت *ثابت default_file_list[] = {"-", عدم};
  حرف ثابت *ثابت *file;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "brs:", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'b':
          separator_ends_record = false;
          اكسر;
        حالة 'r':
          sentinel_length = 0;
          اكسر;
        حالة 's':
          separator = optarg;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  𐤀𐤌 (sentinel_length == 0)
    {
      𐤀𐤌 (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.buffer = عدم;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = عدم;
      error_message = re_compile_pattern (separator, strlen (separator),
                                          &compiled_separator);
      𐤀𐤌 (error_message)
        error (EXIT_FAILURE, 0, "%s", (error_message));
    }
  وإلا
    match_length = sentinel_length = *separator ? strlen (separator) : 1;

  read_size = INITIAL_READSIZE;
  طالما (sentinel_length >= read_size / 2)
    {
      𐤀𐤌 (SIZE_MAX / 2 < read_size)
        xalloc_die ();
      read_size *= 2;
    }
  half_buffer_size = read_size + sentinel_length + 1;
  G_buffer_size = 2 * half_buffer_size;
  𐤀𐤌 (! (read_size < half_buffer_size && half_buffer_size < G_buffer_size))
    xalloc_die ();
  G_buffer = xmalloc (G_buffer_size);
  𐤀𐤌 (sentinel_length)
    {
      memcpy (G_buffer, separator, sentinel_length + !!*separator);
      G_buffer += sentinel_length;
    }
  وإلا
    {
      ++G_buffer;
    }

  file = (optind < argc
          ? (حرف ثابت *ثابت *) &argv[optind]
          : default_file_list);

  xset_binary_mode (STDOUT_FILENO, O_BINARY);

  {
    ok = true;
    لكل (size_t i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((حرف *) عدم, (حرف *) عدم);

  𐤀𐤌 (have_read_stdin && close (STDIN_FILENO) < 0)
    {
      error (0, errno, "-");
      ok = false;
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
