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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <regex.h>

#अन्तर्भाव "filenamecat.h"
#अन्तर्भाव "full-read.h"
#अन्तर्भाव "full-write.h"
#अन्तर्भाव "temp-stream.h"
#अन्तर्भाव "xbinary-io.h"

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
स्थिर अक्षर स्थिरांक *separator = "\n";

/* True if we have ever read standard input.  */
स्थिर bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
स्थिर bool separator_ends_record = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
स्थिर माप_प्रकार sentinel_length = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
स्थिर माप_प्रकार match_length;

/* The input buffer. */
स्थिर अक्षर *G_buffer;

/* The number of bytes to read at once into 'buffer'. */
स्थिर माप_प्रकार read_size;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
स्थिर माप_प्रकार G_buffer_size;

/* The compiled regular expression representing 'separator'. */
स्थिर संरचना re_pattern_buffer compiled_separator;
स्थिर अक्षर compiled_separator_fastmap[अच_अक्षर_उच्च + 1];
स्थिर संरचना re_registers regs;

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"before", no_argument, रिक्त, 'b'},
  {"regex", no_argument, रिक्त, 'r'},
  {"separator", required_argument, रिक्त, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      ख_माला_दो (_("\
Write each खाता to standard output, last ओळ first.\n\
"), मानक_निकास);

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
      ख_माला_दो (_("\n\
Non-seekable आदान is buffered to $TMPDIR, defaulting to /tmp.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

स्थिर शून्य
output (अक्षर स्थिरांक *start, अक्षर स्थिरांक *past_end)
{
  स्थिर अक्षर buffer[WRITESIZE];
  स्थिर माप_प्रकार bytes_in_buffer = 0;
  माप_प्रकार bytes_to_add = past_end - start;
  माप_प्रकार bytes_available = WRITESIZE - bytes_in_buffer;

  जर (!start)
    {
      जर (full_write (STDOUT_FILENO, buffer, bytes_in_buffer)
          != bytes_in_buffer)
        write_error ();
      bytes_in_buffer = 0;
      परत;
    }

  /* Write out as many full buffers as possible. */
  पर्यंत (bytes_to_add >= bytes_available)
    {
      स_नकल (buffer + bytes_in_buffer, start, bytes_available);
      bytes_to_add -= bytes_available;
      start += bytes_available;
      जर (full_write (STDOUT_FILENO, buffer, WRITESIZE) != WRITESIZE)
        write_error ();
      bytes_in_buffer = 0;
      bytes_available = WRITESIZE;
    }

  स_नकल (buffer + bytes_in_buffer, start, bytes_to_add);
  bytes_in_buffer += bytes_to_add;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

स्थिर bool
tac_seekable (पूर्णांक आदान_fd, अक्षर स्थिरांक *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  अक्षर *match_start;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  अक्षर *past_end;

  /* Length of the record growing in 'G_buffer'. */
  सूचक_भेद_प्रकार saved_record_size;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool first_time = true;
  अक्षर first_char = *separator;	/* Speed optimization, non-regexp. */
  अक्षर स्थिरांक *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  माप_प्रकार match_length1 = match_length - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  माप_प्रकार remainder = file_pos % read_size;
  जर (remainder != 0)
    {
      file_pos -= remainder;
      जर (lseek (आदान_fd, file_pos, शुरू_से) < 0)
        error (0, त्रुटि_सं, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  पर्यंत ((saved_record_size = पठ (आदान_fd, G_buffer, read_size)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_size;
      जर (lseek (आदान_fd, -rsize, प्रस्तुत_से) < 0)
        error (0, त्रुटि_सं, _("%s: seek failed"), quotef (file));
      file_pos -= read_size;
    }

  /* Now scan forward, looking for end of file.  */
  पर्यंत (saved_record_size == read_size)
    {
      ssize_t nread = पठ (आदान_fd, G_buffer, read_size);
      जर (nread == 0)
        थांबा;
      saved_record_size = nread;
      जर (saved_record_size < 0)
        थांबा;
      file_pos += nread;
    }

  जर (saved_record_size < 0)
    {
      error (0, त्रुटि_सं, _("%s: read error"), quotef (file));
      परत false;
    }

  match_start = past_end = G_buffer + saved_record_size;
  /* For non-regexp search, move past impossible positions for a match. */
  जर (sentinel_length)
    match_start -= match_length1;

  पर्यंत (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      जर (sentinel_length == 0)
        {
          माप_प्रकार i = match_start - G_buffer;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          जर (1 < range)
            error (निकास_त्रुटि, 0, _("record too large"));

          जर (range == 1
              || ((ret = re_शोध (&compiled_separator, G_buffer,
                                    i, i - 1, range, &regs))
                  == -1))
            match_start = G_buffer - 1;
          अन्यथा जर (ret == -2)
            error (निकास_त्रुटि, 0,
                   _("error in regular expression search"));
          अन्यथा
            {
              match_start = G_buffer + regs.start[0];
              match_length = regs.end[0] - regs.start[0];
            }
        }
      अन्यथा
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          पर्यंत (*--match_start != first_char
                 || (match_length1 && !STREQ_LEN (match_start + 1, separator1,
                                                  match_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      जर (match_start < G_buffer)
        {
          जर (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_buffer, past_end);
              परत true;
            }

          saved_record_size = past_end - G_buffer;
          जर (saved_record_size > read_size)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              अक्षर *newbuffer;
              माप_प्रकार offset = sentinel_length ? sentinel_length : 1;
              माप_प्रकार old_G_buffer_size = G_buffer_size;

              read_size *= 2;
              G_buffer_size = read_size * 2 + sentinel_length + 2;
              जर (G_buffer_size < old_G_buffer_size)
                xalloc_die ();
              newbuffer = xrealloc (G_buffer - offset, G_buffer_size);
              newbuffer += offset;
              G_buffer = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          जर (file_pos >= read_size)
            file_pos -= read_size;
          अन्यथा
            {
              read_size = file_pos;
              file_pos = 0;
            }
          जर (lseek (आदान_fd, file_pos, शुरू_से) < 0)
            error (0, त्रुटि_सं, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          स_हटाओ (G_buffer + read_size, G_buffer, saved_record_size);
          past_end = G_buffer + read_size + saved_record_size;
          /* For non-regexp searches, avoid unnecessary scanning. */
          जर (sentinel_length)
            match_start = G_buffer + read_size;
          अन्यथा
            match_start = past_end;

          जर (full_read (आदान_fd, G_buffer, read_size) != read_size)
            {
              error (0, त्रुटि_सं, _("%s: read error"), quotef (file));
              परत false;
            }
        }
      अन्यथा
        {
          /* Found a match of 'separator'. */
          जर (separator_ends_record)
            {
              अक्षर *match_end = match_start + match_length;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              जर (!first_time || match_end != past_end)
                output (match_end, past_end);
              past_end = match_end;
              first_time = false;
            }
          अन्यथा
            {
              output (match_start, past_end);
              past_end = match_start;
            }

          /* For non-regex matching, we can back up.  */
          जर (sentinel_length > 0)
            match_start -= match_length - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

स्थिर off_t
copy_to_temp (खाता **g_tmp, अक्षर **g_tempfile, पूर्णांक आदान_fd, अक्षर स्थिरांक *file)
{
  खाता *fp;
  अक्षर *file_name;
  uintmax_t bytes_copied = 0;
  जर (!temp_stream (&fp, &file_name))
    परत -1;

  पर्यंत (true)
    {
      ssize_t bytes_read = पठ (आदान_fd, G_buffer, read_size);
      जर (bytes_read == 0)
        थांबा;
      जर (bytes_read < 0)
        {
          error (0, त्रुटि_सं, _("%s: read error"), quotef (file));
          परत -1;
        }

      जर (ख_डालो (G_buffer, 1, bytes_read, fp) != bytes_read)
        {
          error (0, त्रुटि_सं, _("%s: write error"), quotef (file_name));
          परत -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      bytes_copied += bytes_read;
    }

  जर (ख_साफ (fp) != 0)
    {
      error (0, त्रुटि_सं, _("%s: write error"), quotef (file_name));
      परत -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  परत bytes_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

स्थिर bool
tac_nonseekable (पूर्णांक आदान_fd, अक्षर स्थिरांक *file)
{
  खाता *tmp_stream;
  अक्षर *tmp_file;
  off_t bytes_copied = copy_to_temp (&tmp_stream, &tmp_file, आदान_fd, file);
  जर (bytes_copied < 0)
    परत false;

  bool ok = tac_seekable (fileno (tmp_stream), tmp_file, bytes_copied);
  परत ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

स्थिर bool
tac_file (अक्षर स्थिरांक *filename)
{
  bool ok;
  off_t file_size;
  पूर्णांक fd;
  bool is_stdin = streq (filename, "-");

  जर (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
    }
  अन्यथा
    {
      fd = खोलो (filename, O_RDONLY | O_द्विमान);
      जर (fd < 0)
        {
          error (0, त्रुटि_सं, _("failed to open %s for reading"),
                 quoteaf (filename));
          परत false;
        }
    }

  file_size = lseek (fd, 0, अंत_से);

  ok = (file_size < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_size));

  जर (!is_stdin && बंद (fd) != 0)
    {
      error (0, त्रुटि_सं, _("%s: read error"), quotef (filename));
      ok = false;
    }
  परत ok;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  अक्षर स्थिरांक *error_message;	/* Return value from re_compile_pattern. */
  पूर्णांक optc;
  bool ok;
  माप_प्रकार half_buffer_size;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  स्थिर अक्षर स्थिरांक *स्थिरांक default_file_list[] = {"-", रिक्त};
  अक्षर स्थिरांक *स्थिरांक *file;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत ((optc = getopt_long (argc, argv, "brs:", longopts, रिक्त)) != -1)
    {
      निवड (optc)
        {
        स्थिती 'b':
          separator_ends_record = false;
          थांबा;
        स्थिती 'r':
          sentinel_length = 0;
          थांबा;
        स्थिती 's':
          separator = optarg;
          थांबा;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (sentinel_length == 0)
    {
      जर (*separator == 0)
        error (निकास_त्रुटि, 0, _("separator cannot be empty"));

      compiled_separator.buffer = रिक्त;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = रिक्त;
      error_message = re_compile_pattern (separator, म_माप (separator),
                                          &compiled_separator);
      जर (error_message)
        error (निकास_त्रुटि, 0, "%s", (error_message));
    }
  अन्यथा
    match_length = sentinel_length = *separator ? म_माप (separator) : 1;

  read_size = INITIAL_READSIZE;
  पर्यंत (sentinel_length >= read_size / 2)
    {
      जर (SIZE_MAX / 2 < read_size)
        xalloc_die ();
      read_size *= 2;
    }
  half_buffer_size = read_size + sentinel_length + 1;
  G_buffer_size = 2 * half_buffer_size;
  जर (! (read_size < half_buffer_size && half_buffer_size < G_buffer_size))
    xalloc_die ();
  G_buffer = xmalloc (G_buffer_size);
  जर (sentinel_length)
    {
      स_नकल (G_buffer, separator, sentinel_length + !!*separator);
      G_buffer += sentinel_length;
    }
  अन्यथा
    {
      ++G_buffer;
    }

  file = (optind < argc
          ? (अक्षर स्थिरांक *स्थिरांक *) &argv[optind]
          : default_file_list);

  xset_द्विमान_mode (STDOUT_FILENO, O_द्विमान);

  {
    ok = true;
    साठी (माप_प्रकार i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((अक्षर *) रिक्त, (अक्षर *) रिक्त);

  जर (have_read_stdin && बंद (STDIN_FILENO) < 0)
    {
      error (0, त्रुटि_सं, "-");
      ok = false;
    }

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
