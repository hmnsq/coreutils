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

#include <config.h>

#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include "system.h"

#include <regex.h>

#include "filenamecat.h"
#include "full-read.h"
#include "full-write.h"
#include "temp-stream.h"
#include "xbinary-io.h"

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
static char const *separator = "\n";

/* True if we have ever read standard input.  */
static bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
static bool separator_ends_record = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
static size_t sentinel_length = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
static size_t match_length;

/* The input buffer. */
static char *G_buffer;

/* The number of bytes to read at once into 'buffer'. */
static size_t read_size;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
static size_t G_buffer_size;

/* The compiled regular expression representing 'separator'. */
static struct re_pattern_buffer compiled_separator;
static char compiled_separator_fastmap[UCHAR_MAX + 1];
static struct re_registers regs;

static struct option const longopts[] =
{
  {"before", no_argument, NULL, 'b'},
  {"regex", no_argument, NULL, 'r'},
  {"separator", required_argument, NULL, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (цяло status)
{
  ако (status != EXIT_SUCCESS)
    emit_try_help ();
  иначе
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

static void
output (char const *start, char const *past_end)
{
  static char buffer[WRITESIZE];
  static size_t bytes_in_buffer = 0;
  size_t bytes_to_add = past_end - start;
  size_t bytes_available = WRITESIZE - bytes_in_buffer;

  ако (!start)
    {
      ако (full_write (STDOUT_FILENO, buffer, bytes_in_buffer)
          != bytes_in_buffer)
        write_error ();
      bytes_in_buffer = 0;
      върни;
    }

  /* Write out as many full buffers as possible. */
  докато (bytes_to_add >= bytes_available)
    {
      memcpy (buffer + bytes_in_buffer, start, bytes_available);
      bytes_to_add -= bytes_available;
      start += bytes_available;
      ако (full_write (STDOUT_FILENO, buffer, WRITESIZE) != WRITESIZE)
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

static bool
tac_seekable (цяло input_fd, char const *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  char *match_start;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  char *past_end;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_record_size;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool first_time = true;
  char first_char = *separator;	/* Speed optimization, non-regexp. */
  char const *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t match_length1 = match_length - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_size;
  ако (remainder != 0)
    {
      file_pos -= remainder;
      ако (lseek (input_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  докато ((saved_record_size = read (input_fd, G_buffer, read_size)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_size;
      ако (lseek (input_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_size;
    }

  /* Now scan forward, looking for end of file.  */
  докато (saved_record_size == read_size)
    {
      ssize_t nread = read (input_fd, G_buffer, read_size);
      ако (nread == 0)
        break;
      saved_record_size = nread;
      ако (saved_record_size < 0)
        break;
      file_pos += nread;
    }

  ако (saved_record_size < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      върни false;
    }

  match_start = past_end = G_buffer + saved_record_size;
  /* For non-regexp search, move past impossible positions for a match. */
  ако (sentinel_length)
    match_start -= match_length1;

  докато (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      ако (sentinel_length == 0)
        {
          size_t i = match_start - G_buffer;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          ако (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          ако (range == 1
              || ((ret = re_search (&compiled_separator, G_buffer,
                                    i, i - 1, range, &regs))
                  == -1))
            match_start = G_buffer - 1;
          иначе ако (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          иначе
            {
              match_start = G_buffer + regs.start[0];
              match_length = regs.end[0] - regs.start[0];
            }
        }
      иначе
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          докато (*--match_start != first_char
                 || (match_length1 && !STREQ_LEN (match_start + 1, separator1,
                                                  match_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      ако (match_start < G_buffer)
        {
          ако (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_buffer, past_end);
              върни true;
            }

          saved_record_size = past_end - G_buffer;
          ако (saved_record_size > read_size)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              char *newbuffer;
              size_t offset = sentinel_length ? sentinel_length : 1;
              size_t old_G_buffer_size = G_buffer_size;

              read_size *= 2;
              G_buffer_size = read_size * 2 + sentinel_length + 2;
              ако (G_buffer_size < old_G_buffer_size)
                xalloc_die ();
              newbuffer = xrealloc (G_buffer - offset, G_buffer_size);
              newbuffer += offset;
              G_buffer = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          ако (file_pos >= read_size)
            file_pos -= read_size;
          иначе
            {
              read_size = file_pos;
              file_pos = 0;
            }
          ако (lseek (input_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_buffer + read_size, G_buffer, saved_record_size);
          past_end = G_buffer + read_size + saved_record_size;
          /* For non-regexp searches, avoid unnecessary scanning. */
          ако (sentinel_length)
            match_start = G_buffer + read_size;
          иначе
            match_start = past_end;

          ако (full_read (input_fd, G_buffer, read_size) != read_size)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              върни false;
            }
        }
      иначе
        {
          /* Found a match of 'separator'. */
          ако (separator_ends_record)
            {
              char *match_end = match_start + match_length;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              ако (!first_time || match_end != past_end)
                output (match_end, past_end);
              past_end = match_end;
              first_time = false;
            }
          иначе
            {
              output (match_start, past_end);
              past_end = match_start;
            }

          /* For non-regex matching, we can back up.  */
          ако (sentinel_length > 0)
            match_start -= match_length - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

static off_t
copy_to_temp (FILE **g_tmp, char **g_tempfile, цяло input_fd, char const *file)
{
  FILE *fp;
  char *file_name;
  uintmax_t bytes_copied = 0;
  ако (!temp_stream (&fp, &file_name))
    върни -1;

  докато (true)
    {
      ssize_t bytes_read = read (input_fd, G_buffer, read_size);
      ако (bytes_read == 0)
        break;
      ако (bytes_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          върни -1;
        }

      ако (fwrite (G_buffer, 1, bytes_read, fp) != bytes_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          върни -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      bytes_copied += bytes_read;
    }

  ако (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      върни -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  върни bytes_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

static bool
tac_nonseekable (цяло input_fd, char const *file)
{
  FILE *tmp_stream;
  char *tmp_file;
  off_t bytes_copied = copy_to_temp (&tmp_stream, &tmp_file, input_fd, file);
  ако (bytes_copied < 0)
    върни false;

  bool ok = tac_seekable (fileno (tmp_stream), tmp_file, bytes_copied);
  върни ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

static bool
tac_file (char const *filename)
{
  bool ok;
  off_t file_size;
  цяло fd;
  bool is_stdin = streq (filename, "-");

  ако (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_binary_mode (STDIN_FILENO, O_BINARY);
    }
  иначе
    {
      fd = open (filename, O_RDONLY | O_BINARY);
      ако (fd < 0)
        {
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          върни false;
        }
    }

  file_size = lseek (fd, 0, SEEK_END);

  ok = (file_size < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_size));

  ако (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  върни ok;
}

цяло
main (цяло argc, char **argv)
{
  char const *error_message;	/* Return value from re_compile_pattern. */
  цяло optc;
  bool ok;
  size_t half_buffer_size;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  static char const *const default_file_list[] = {"-", NULL};
  char const *const *file;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  докато ((optc = getopt_long (argc, argv, "brs:", longopts, NULL)) != -1)
    {
      switch (optc)
        {
        case 'b':
          separator_ends_record = false;
          break;
        case 'r':
          sentinel_length = 0;
          break;
        case 's':
          separator = optarg;
          break;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  ако (sentinel_length == 0)
    {
      ако (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.buffer = NULL;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = NULL;
      error_message = re_compile_pattern (separator, strlen (separator),
                                          &compiled_separator);
      ако (error_message)
        error (EXIT_FAILURE, 0, "%s", (error_message));
    }
  иначе
    match_length = sentinel_length = *separator ? strlen (separator) : 1;

  read_size = INITIAL_READSIZE;
  докато (sentinel_length >= read_size / 2)
    {
      ако (SIZE_MAX / 2 < read_size)
        xalloc_die ();
      read_size *= 2;
    }
  half_buffer_size = read_size + sentinel_length + 1;
  G_buffer_size = 2 * half_buffer_size;
  ако (! (read_size < half_buffer_size && half_buffer_size < G_buffer_size))
    xalloc_die ();
  G_buffer = xmalloc (G_buffer_size);
  ако (sentinel_length)
    {
      memcpy (G_buffer, separator, sentinel_length + !!*separator);
      G_buffer += sentinel_length;
    }
  иначе
    {
      ++G_buffer;
    }

  file = (optind < argc
          ? (char const *const *) &argv[optind]
          : default_file_list);

  xset_binary_mode (STDOUT_FILENO, O_BINARY);

  {
    ok = true;
    за (size_t i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((char *) NULL, (char *) NULL);

  ако (have_read_stdin && close (STDIN_FILENO) < 0)
    {
      error (0, errno, "-");
      ok = false;
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
