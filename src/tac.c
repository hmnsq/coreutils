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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <regex.h>

#അന്തര്ഭാവ "filenamecat.h"
#അന്തര്ഭാവ "full-read.h"
#അന്തര്ഭാവ "full-write.h"
#അന്തര്ഭാവ "temp-stream.h"
#അന്തര്ഭാവ "xbinary-io.h"

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
സ്ഥിതം അക്ഷരം സ്ഥിരം *separator = "\n";

/* True if we have ever read standard input.  */
സ്ഥിതം bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
സ്ഥിതം bool separator_ends_record = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
സ്ഥിതം size_t sentinel_length = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
സ്ഥിതം size_t match_length;

/* The input buffer. */
സ്ഥിതം അക്ഷരം *G_buffer;

/* The number of bytes to read at once into 'buffer'. */
സ്ഥിതം size_t read_size;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
സ്ഥിതം size_t G_buffer_size;

/* The compiled regular expression representing 'separator'. */
സ്ഥിതം ഘടന re_pattern_buffer compiled_separator;
സ്ഥിതം അക്ഷരം compiled_separator_fastmap[UCHAR_MAX + 1];
സ്ഥിതം ഘടന re_registers regs;

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"before", no_argument, ശൂന്യ, 'b'},
  {"regex", no_argument, ശൂന്യ, 'r'},
  {"separator", required_argument, ശൂന്യ, 's'},
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
      fputs (_("\
Write each FILE to standard output, last വരി first.\n\
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
Non-seekable നിവേശം is buffered to $TMPDIR, defaulting to /tmp.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

സ്ഥിതം ശൂന്യം
output (അക്ഷരം സ്ഥിരം *start, അക്ഷരം സ്ഥിരം *past_end)
{
  സ്ഥിതം അക്ഷരം buffer[WRITESIZE];
  സ്ഥിതം size_t bytes_in_buffer = 0;
  size_t bytes_to_add = past_end - start;
  size_t bytes_available = WRITESIZE - bytes_in_buffer;

  എങ്കിൽ (!start)
    {
      എങ്കിൽ (full_write (STDOUT_FILENO, buffer, bytes_in_buffer)
          != bytes_in_buffer)
        write_error ();
      bytes_in_buffer = 0;
      തിരികെ;
    }

  /* Write out as many full buffers as possible. */
  ആകുമ്പോൾ (bytes_to_add >= bytes_available)
    {
      memcpy (buffer + bytes_in_buffer, start, bytes_available);
      bytes_to_add -= bytes_available;
      start += bytes_available;
      എങ്കിൽ (full_write (STDOUT_FILENO, buffer, WRITESIZE) != WRITESIZE)
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

സ്ഥിതം bool
tac_seekable (പൂർണ്ണസംഖ്യ നിവേശം_fd, അക്ഷരം സ്ഥിരം *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  അക്ഷരം *match_start;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  അക്ഷരം *past_end;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_record_size;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool first_time = true;
  അക്ഷരം first_char = *separator;	/* Speed optimization, non-regexp. */
  അക്ഷരം സ്ഥിരം *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t match_length1 = match_length - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_size;
  എങ്കിൽ (remainder != 0)
    {
      file_pos -= remainder;
      എങ്കിൽ (lseek (നിവേശം_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  ആകുമ്പോൾ ((saved_record_size = പഠ (നിവേശം_fd, G_buffer, read_size)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_size;
      എങ്കിൽ (lseek (നിവേശം_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_size;
    }

  /* Now scan forward, looking for end of file.  */
  ആകുമ്പോൾ (saved_record_size == read_size)
    {
      ssize_t nread = പഠ (നിവേശം_fd, G_buffer, read_size);
      എങ്കിൽ (nread == 0)
        നിർത്തുക;
      saved_record_size = nread;
      എങ്കിൽ (saved_record_size < 0)
        നിർത്തുക;
      file_pos += nread;
    }

  എങ്കിൽ (saved_record_size < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      തിരികെ false;
    }

  match_start = past_end = G_buffer + saved_record_size;
  /* For non-regexp search, move past impossible positions for a match. */
  എങ്കിൽ (sentinel_length)
    match_start -= match_length1;

  ആകുമ്പോൾ (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      എങ്കിൽ (sentinel_length == 0)
        {
          size_t i = match_start - G_buffer;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          എങ്കിൽ (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          എങ്കിൽ (range == 1
              || ((ret = re_തിരയൽ (&compiled_separator, G_buffer,
                                    i, i - 1, range, &regs))
                  == -1))
            match_start = G_buffer - 1;
          അല്ലെങ്കിൽ എങ്കിൽ (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          അല്ലെങ്കിൽ
            {
              match_start = G_buffer + regs.start[0];
              match_length = regs.end[0] - regs.start[0];
            }
        }
      അല്ലെങ്കിൽ
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          ആകുമ്പോൾ (*--match_start != first_char
                 || (match_length1 && !STREQ_LEN (match_start + 1, separator1,
                                                  match_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      എങ്കിൽ (match_start < G_buffer)
        {
          എങ്കിൽ (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_buffer, past_end);
              തിരികെ true;
            }

          saved_record_size = past_end - G_buffer;
          എങ്കിൽ (saved_record_size > read_size)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              അക്ഷരം *newbuffer;
              size_t offset = sentinel_length ? sentinel_length : 1;
              size_t old_G_buffer_size = G_buffer_size;

              read_size *= 2;
              G_buffer_size = read_size * 2 + sentinel_length + 2;
              എങ്കിൽ (G_buffer_size < old_G_buffer_size)
                xalloc_die ();
              newbuffer = xrealloc (G_buffer - offset, G_buffer_size);
              newbuffer += offset;
              G_buffer = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          എങ്കിൽ (file_pos >= read_size)
            file_pos -= read_size;
          അല്ലെങ്കിൽ
            {
              read_size = file_pos;
              file_pos = 0;
            }
          എങ്കിൽ (lseek (നിവേശം_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_buffer + read_size, G_buffer, saved_record_size);
          past_end = G_buffer + read_size + saved_record_size;
          /* For non-regexp searches, avoid unnecessary scanning. */
          എങ്കിൽ (sentinel_length)
            match_start = G_buffer + read_size;
          അല്ലെങ്കിൽ
            match_start = past_end;

          എങ്കിൽ (full_read (നിവേശം_fd, G_buffer, read_size) != read_size)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              തിരികെ false;
            }
        }
      അല്ലെങ്കിൽ
        {
          /* Found a match of 'separator'. */
          എങ്കിൽ (separator_ends_record)
            {
              അക്ഷരം *match_end = match_start + match_length;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              എങ്കിൽ (!first_time || match_end != past_end)
                output (match_end, past_end);
              past_end = match_end;
              first_time = false;
            }
          അല്ലെങ്കിൽ
            {
              output (match_start, past_end);
              past_end = match_start;
            }

          /* For non-regex matching, we can back up.  */
          എങ്കിൽ (sentinel_length > 0)
            match_start -= match_length - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

സ്ഥിതം off_t
copy_to_temp (FILE **g_tmp, അക്ഷരം **g_tempfile, പൂർണ്ണസംഖ്യ നിവേശം_fd, അക്ഷരം സ്ഥിരം *file)
{
  FILE *fp;
  അക്ഷരം *file_name;
  uintmax_t bytes_copied = 0;
  എങ്കിൽ (!temp_stream (&fp, &file_name))
    തിരികെ -1;

  ആകുമ്പോൾ (true)
    {
      ssize_t bytes_read = പഠ (നിവേശം_fd, G_buffer, read_size);
      എങ്കിൽ (bytes_read == 0)
        നിർത്തുക;
      എങ്കിൽ (bytes_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          തിരികെ -1;
        }

      എങ്കിൽ (fwrite (G_buffer, 1, bytes_read, fp) != bytes_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          തിരികെ -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      bytes_copied += bytes_read;
    }

  എങ്കിൽ (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      തിരികെ -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  തിരികെ bytes_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

സ്ഥിതം bool
tac_nonseekable (പൂർണ്ണസംഖ്യ നിവേശം_fd, അക്ഷരം സ്ഥിരം *file)
{
  FILE *tmp_stream;
  അക്ഷരം *tmp_file;
  off_t bytes_copied = copy_to_temp (&tmp_stream, &tmp_file, നിവേശം_fd, file);
  എങ്കിൽ (bytes_copied < 0)
    തിരികെ false;

  bool ok = tac_seekable (fileno (tmp_stream), tmp_file, bytes_copied);
  തിരികെ ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

സ്ഥിതം bool
tac_file (അക്ഷരം സ്ഥിരം *filename)
{
  bool ok;
  off_t file_size;
  പൂർണ്ണസംഖ്യ fd;
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
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          തിരികെ false;
        }
    }

  file_size = lseek (fd, 0, SEEK_END);

  ok = (file_size < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_size));

  എങ്കിൽ (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  തിരികെ ok;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  അക്ഷരം സ്ഥിരം *error_message;	/* Return value from re_compile_pattern. */
  പൂർണ്ണസംഖ്യ optc;
  bool ok;
  size_t half_buffer_size;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം default_file_list[] = {"-", ശൂന്യ};
  അക്ഷരം സ്ഥിരം *സ്ഥിരം *file;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "brs:", longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'b':
          separator_ends_record = false;
          നിർത്തുക;
        അവസ്ഥ 'r':
          sentinel_length = 0;
          നിർത്തുക;
        അവസ്ഥ 's':
          separator = optarg;
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (sentinel_length == 0)
    {
      എങ്കിൽ (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.buffer = ശൂന്യ;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = ശൂന്യ;
      error_message = re_compile_pattern (separator, strlen (separator),
                                          &compiled_separator);
      എങ്കിൽ (error_message)
        error (EXIT_FAILURE, 0, "%s", (error_message));
    }
  അല്ലെങ്കിൽ
    match_length = sentinel_length = *separator ? strlen (separator) : 1;

  read_size = INITIAL_READSIZE;
  ആകുമ്പോൾ (sentinel_length >= read_size / 2)
    {
      എങ്കിൽ (SIZE_MAX / 2 < read_size)
        xalloc_die ();
      read_size *= 2;
    }
  half_buffer_size = read_size + sentinel_length + 1;
  G_buffer_size = 2 * half_buffer_size;
  എങ്കിൽ (! (read_size < half_buffer_size && half_buffer_size < G_buffer_size))
    xalloc_die ();
  G_buffer = xmalloc (G_buffer_size);
  എങ്കിൽ (sentinel_length)
    {
      memcpy (G_buffer, separator, sentinel_length + !!*separator);
      G_buffer += sentinel_length;
    }
  അല്ലെങ്കിൽ
    {
      ++G_buffer;
    }

  file = (optind < argc
          ? (അക്ഷരം സ്ഥിരം *സ്ഥിരം *) &argv[optind]
          : default_file_list);

  xset_ദ്വിമാന_mode (STDOUT_FILENO, O_ദ്വിമാന);

  {
    ok = true;
    വേണ്ടി (size_t i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((അക്ഷരം *) ശൂന്യ, (അക്ഷരം *) ശൂന്യ);

  എങ്കിൽ (have_read_stdin && close (STDIN_FILENO) < 0)
    {
      error (0, errno, "-");
      ok = false;
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
