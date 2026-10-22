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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <regex.h>

#অন্তর্ভাব "filenamecat.h"
#অন্তর্ভাব "full-read.h"
#অন্তর্ভাব "full-write.h"
#অন্তর্ভাব "temp-stream.h"
#অন্তর্ভাব "xbinary-io.h"

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
স্থির অক্ষর ধ্রুবক *separator = "\n";

/* True if we have ever read standard input.  */
স্থির bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
স্থির bool separator_ends_record = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
স্থির size_t sentinel_দৈর্ঘ্য = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
স্থির size_t match_দৈর্ঘ্য;

/* The input buffer. */
স্থির অক্ষর *G_বাফার;

/* The number of bytes to read at once into 'buffer'. */
স্থির size_t read_আকার;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
স্থির size_t G_বাফার_আকার;

/* The compiled regular expression representing 'separator'. */
স্থির কাঠামো re_pattern_বাফার compiled_separator;
স্থির অক্ষর compiled_separator_fastmap[UCHAR_MAX + 1];
স্থির কাঠামো re_registers regs;

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"before", no_argument, ফাঁকা, 'b'},
  {"regex", no_argument, ফাঁকা, 'r'},
  {"separator", required_argument, ফাঁকা, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write each FILE to standard output, last সারি first.\n\
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
Non-seekable প্রবেশ is buffered to $TMPDIR, defaulting to /tmp.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

স্থির শূন্য
output (অক্ষর ধ্রুবক *start, অক্ষর ধ্রুবক *past_end)
{
  স্থির অক্ষর বাফার[WRITESIZE];
  স্থির size_t bytes_in_বাফার = 0;
  size_t bytes_to_add = past_end - start;
  size_t bytes_available = WRITESIZE - bytes_in_বাফার;

  যদি (!start)
    {
      যদি (full_write (STDOUT_FILENO, বাফার, bytes_in_বাফার)
          != bytes_in_বাফার)
        write_error ();
      bytes_in_বাফার = 0;
      ফেরত;
    }

  /* Write out as many full buffers as possible. */
  যখন (bytes_to_add >= bytes_available)
    {
      memcpy (বাফার + bytes_in_বাফার, start, bytes_available);
      bytes_to_add -= bytes_available;
      start += bytes_available;
      যদি (full_write (STDOUT_FILENO, বাফার, WRITESIZE) != WRITESIZE)
        write_error ();
      bytes_in_বাফার = 0;
      bytes_available = WRITESIZE;
    }

  memcpy (বাফার + bytes_in_বাফার, start, bytes_to_add);
  bytes_in_বাফার += bytes_to_add;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

স্থির bool
tac_seekable (পূর্ণসংখ্যা প্রবেশ_fd, অক্ষর ধ্রুবক *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  অক্ষর *match_start;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  অক্ষর *past_end;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_record_আকার;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool first_time = true;
  অক্ষর first_char = *separator;	/* Speed optimization, non-regexp. */
  অক্ষর ধ্রুবক *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t match_length1 = match_দৈর্ঘ্য - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_আকার;
  যদি (remainder != 0)
    {
      file_pos -= remainder;
      যদি (lseek (প্রবেশ_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  যখন ((saved_record_আকার = পঠ (প্রবেশ_fd, G_বাফার, read_আকার)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_আকার;
      যদি (lseek (প্রবেশ_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_আকার;
    }

  /* Now scan forward, looking for end of file.  */
  যখন (saved_record_আকার == read_আকার)
    {
      ssize_t nread = পঠ (প্রবেশ_fd, G_বাফার, read_আকার);
      যদি (nread == 0)
        বিরতি;
      saved_record_আকার = nread;
      যদি (saved_record_আকার < 0)
        বিরতি;
      file_pos += nread;
    }

  যদি (saved_record_আকার < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      ফেরত false;
    }

  match_start = past_end = G_বাফার + saved_record_আকার;
  /* For non-regexp search, move past impossible positions for a match. */
  যদি (sentinel_দৈর্ঘ্য)
    match_start -= match_length1;

  যখন (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      যদি (sentinel_দৈর্ঘ্য == 0)
        {
          size_t i = match_start - G_বাফার;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          যদি (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          যদি (range == 1
              || ((ret = re_অনুসন্ধান (&compiled_separator, G_বাফার,
                                    i, i - 1, range, &regs))
                  == -1))
            match_start = G_বাফার - 1;
          অন্যথা যদি (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          অন্যথা
            {
              match_start = G_বাফার + regs.start[0];
              match_দৈর্ঘ্য = regs.end[0] - regs.start[0];
            }
        }
      অন্যথা
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          যখন (*--match_start != first_char
                 || (match_length1 && !STREQ_LEN (match_start + 1, separator1,
                                                  match_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      যদি (match_start < G_বাফার)
        {
          যদি (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_বাফার, past_end);
              ফেরত true;
            }

          saved_record_আকার = past_end - G_বাফার;
          যদি (saved_record_আকার > read_আকার)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              অক্ষর *newbuffer;
              size_t offset = sentinel_দৈর্ঘ্য ? sentinel_দৈর্ঘ্য : 1;
              size_t old_G_বাফার_আকার = G_বাফার_আকার;

              read_আকার *= 2;
              G_বাফার_আকার = read_আকার * 2 + sentinel_দৈর্ঘ্য + 2;
              যদি (G_বাফার_আকার < old_G_বাফার_আকার)
                xalloc_die ();
              newbuffer = xrealloc (G_বাফার - offset, G_বাফার_আকার);
              newbuffer += offset;
              G_বাফার = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          যদি (file_pos >= read_আকার)
            file_pos -= read_আকার;
          অন্যথা
            {
              read_আকার = file_pos;
              file_pos = 0;
            }
          যদি (lseek (প্রবেশ_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_বাফার + read_আকার, G_বাফার, saved_record_আকার);
          past_end = G_বাফার + read_আকার + saved_record_আকার;
          /* For non-regexp searches, avoid unnecessary scanning. */
          যদি (sentinel_দৈর্ঘ্য)
            match_start = G_বাফার + read_আকার;
          অন্যথা
            match_start = past_end;

          যদি (full_read (প্রবেশ_fd, G_বাফার, read_আকার) != read_আকার)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              ফেরত false;
            }
        }
      অন্যথা
        {
          /* Found a match of 'separator'. */
          যদি (separator_ends_record)
            {
              অক্ষর *match_end = match_start + match_দৈর্ঘ্য;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              যদি (!first_time || match_end != past_end)
                output (match_end, past_end);
              past_end = match_end;
              first_time = false;
            }
          অন্যথা
            {
              output (match_start, past_end);
              past_end = match_start;
            }

          /* For non-regex matching, we can back up.  */
          যদি (sentinel_দৈর্ঘ্য > 0)
            match_start -= match_দৈর্ঘ্য - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

স্থির off_t
copy_to_অস্থায়ী (FILE **g_tmp, অক্ষর **g_tempfile, পূর্ণসংখ্যা প্রবেশ_fd, অক্ষর ধ্রুবক *file)
{
  FILE *fp;
  অক্ষর *file_name;
  uintmax_t bytes_copied = 0;
  যদি (!অস্থায়ী_stream (&fp, &file_name))
    ফেরত -1;

  যখন (true)
    {
      ssize_t bytes_read = পঠ (প্রবেশ_fd, G_বাফার, read_আকার);
      যদি (bytes_read == 0)
        বিরতি;
      যদি (bytes_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          ফেরত -1;
        }

      যদি (fwrite (G_বাফার, 1, bytes_read, fp) != bytes_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          ফেরত -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      bytes_copied += bytes_read;
    }

  যদি (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      ফেরত -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  ফেরত bytes_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

স্থির bool
tac_nonseekable (পূর্ণসংখ্যা প্রবেশ_fd, অক্ষর ধ্রুবক *file)
{
  FILE *tmp_stream;
  অক্ষর *tmp_file;
  off_t bytes_copied = copy_to_অস্থায়ী (&tmp_stream, &tmp_file, প্রবেশ_fd, file);
  যদি (bytes_copied < 0)
    ফেরত false;

  bool ok = tac_seekable (fileno (tmp_stream), tmp_file, bytes_copied);
  ফেরত ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

স্থির bool
tac_file (অক্ষর ধ্রুবক *filename)
{
  bool ok;
  off_t file_আকার;
  পূর্ণসংখ্যা fd;
  bool is_stdin = streq (filename, "-");

  যদি (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_দ্বিমিক_mode (STDIN_FILENO, O_দ্বিমিক);
    }
  অন্যথা
    {
      fd = open (filename, O_RDONLY | O_দ্বিমিক);
      যদি (fd < 0)
        {
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          ফেরত false;
        }
    }

  file_আকার = lseek (fd, 0, SEEK_END);

  ok = (file_আকার < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_আকার));

  যদি (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  ফেরত ok;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  অক্ষর ধ্রুবক *error_message;	/* Return value from re_compile_pattern. */
  পূর্ণসংখ্যা optc;
  bool ok;
  size_t half_বাফার_আকার;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  স্থির অক্ষর ধ্রুবক *ধ্রুবক default_file_list[] = {"-", ফাঁকা};
  অক্ষর ধ্রুবক *ধ্রুবক *file;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন ((optc = getopt_long (argc, argv, "brs:", longopts, ফাঁকা)) != -1)
    {
      নির্বাচন (optc)
        {
        ক্ষেত্র 'b':
          separator_ends_record = false;
          বিরতি;
        ক্ষেত্র 'r':
          sentinel_দৈর্ঘ্য = 0;
          বিরতি;
        ক্ষেত্র 's':
          separator = optarg;
          বিরতি;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (sentinel_দৈর্ঘ্য == 0)
    {
      যদি (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.বাফার = ফাঁকা;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = ফাঁকা;
      error_message = re_compile_pattern (separator, strlen (separator),
                                          &compiled_separator);
      যদি (error_message)
        error (EXIT_FAILURE, 0, "%s", (error_message));
    }
  অন্যথা
    match_দৈর্ঘ্য = sentinel_দৈর্ঘ্য = *separator ? strlen (separator) : 1;

  read_আকার = INITIAL_READSIZE;
  যখন (sentinel_দৈর্ঘ্য >= read_আকার / 2)
    {
      যদি (SIZE_MAX / 2 < read_আকার)
        xalloc_die ();
      read_আকার *= 2;
    }
  half_বাফার_আকার = read_আকার + sentinel_দৈর্ঘ্য + 1;
  G_বাফার_আকার = 2 * half_বাফার_আকার;
  যদি (! (read_আকার < half_বাফার_আকার && half_বাফার_আকার < G_বাফার_আকার))
    xalloc_die ();
  G_বাফার = xmalloc (G_বাফার_আকার);
  যদি (sentinel_দৈর্ঘ্য)
    {
      memcpy (G_বাফার, separator, sentinel_দৈর্ঘ্য + !!*separator);
      G_বাফার += sentinel_দৈর্ঘ্য;
    }
  অন্যথা
    {
      ++G_বাফার;
    }

  file = (optind < argc
          ? (অক্ষর ধ্রুবক *ধ্রুবক *) &argv[optind]
          : default_file_list);

  xset_দ্বিমিক_mode (STDOUT_FILENO, O_দ্বিমিক);

  {
    ok = true;
    জন্য (size_t i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((অক্ষর *) ফাঁকা, (অক্ষর *) ফাঁকা);

  যদি (have_read_stdin && close (STDIN_FILENO) < 0)
    {
      error (0, errno, "-");
      ok = false;
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
