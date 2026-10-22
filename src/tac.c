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

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश "system.h"

#समावेश <regex.h>

#समावेश "filenamecat.h"
#समावेश "full-read.h"
#समावेश "full-write.h"
#समावेश "temp-stream.h"
#समावेश "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "tac"

#घोषणा AUTHORS \
  proper_name ("Jay Lepreau"), \
  proper_name ("David MacKenzie")


/* The number of bytes per atomic read. */
#घोषणा INITIAL_READSIZE 8192

/* The number of bytes per atomic write. */
#घोषणा WRITESIZE 8192

/* The string that separates the records of the file. */
अटल अक्षर स्थिर *separator = "\n";

/* True if we have ever read standard input.  */
अटल bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
अटल bool separator_ends_record = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
अटल माप_प्रकार sentinel_लंबाई = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
अटल माप_प्रकार match_लंबाई;

/* The input buffer. */
अटल अक्षर *G_बफर;

/* The number of bytes to read at once into 'buffer'. */
अटल माप_प्रकार read_आकार;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
अटल माप_प्रकार G_बफर_आकार;

/* The compiled regular expression representing 'separator'. */
अटल काष्ठा re_pattern_बफर compiled_separator;
अटल अक्षर compiled_separator_fastmap[अच_अक्षर_उच्च + 1];
अटल काष्ठा re_registers regs;

अटल काष्ठा option स्थिर longopts[] =
{
  {"before", no_argument, शून्य, 'b'},
  {"regex", no_argument, शून्य, 'r'},
  {"separator", required_argument, शून्य, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      ख_माला_दो (_("\
Write each खाता to standard output, last पंक्ति first.\n\
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
Non-seekable निवेश is buffered to $TMPDIR, defaulting to /tmp.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

अटल व्योम
output (अक्षर स्थिर *start, अक्षर स्थिर *past_end)
{
  अटल अक्षर बफर[WRITESIZE];
  अटल माप_प्रकार bytes_in_बफर = 0;
  माप_प्रकार bytes_to_add = past_end - start;
  माप_प्रकार bytes_available = WRITESIZE - bytes_in_बफर;

  अगर (!start)
    {
      अगर (full_write (STDOUT_FILENO, बफर, bytes_in_बफर)
          != bytes_in_बफर)
        write_error ();
      bytes_in_बफर = 0;
      वापस;
    }

  /* Write out as many full buffers as possible. */
  जबतक (bytes_to_add >= bytes_available)
    {
      स_नकल (बफर + bytes_in_बफर, start, bytes_available);
      bytes_to_add -= bytes_available;
      start += bytes_available;
      अगर (full_write (STDOUT_FILENO, बफर, WRITESIZE) != WRITESIZE)
        write_error ();
      bytes_in_बफर = 0;
      bytes_available = WRITESIZE;
    }

  स_नकल (बफर + bytes_in_बफर, start, bytes_to_add);
  bytes_in_बफर += bytes_to_add;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

अटल bool
tac_seekable (पूर्णांक निवेश_fd, अक्षर स्थिर *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  अक्षर *match_start;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  अक्षर *past_end;

  /* Length of the record growing in 'G_buffer'. */
  सूचक_भेद_प्रकार saved_record_आकार;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool first_time = true;
  अक्षर first_char = *separator;	/* Speed optimization, non-regexp. */
  अक्षर स्थिर *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  माप_प्रकार match_length1 = match_लंबाई - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  माप_प्रकार remainder = file_pos % read_आकार;
  अगर (remainder != 0)
    {
      file_pos -= remainder;
      अगर (lseek (निवेश_fd, file_pos, शुरू_से) < 0)
        error (0, त्रुटि_सं, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  जबतक ((saved_record_आकार = पढ़ो (निवेश_fd, G_बफर, read_आकार)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_आकार;
      अगर (lseek (निवेश_fd, -rsize, प्रस्तुत_से) < 0)
        error (0, त्रुटि_सं, _("%s: seek failed"), quotef (file));
      file_pos -= read_आकार;
    }

  /* Now scan forward, looking for end of file.  */
  जबतक (saved_record_आकार == read_आकार)
    {
      ssize_t nread = पढ़ो (निवेश_fd, G_बफर, read_आकार);
      अगर (nread == 0)
        अवरोध;
      saved_record_आकार = nread;
      अगर (saved_record_आकार < 0)
        अवरोध;
      file_pos += nread;
    }

  अगर (saved_record_आकार < 0)
    {
      error (0, त्रुटि_सं, _("%s: read error"), quotef (file));
      वापस false;
    }

  match_start = past_end = G_बफर + saved_record_आकार;
  /* For non-regexp search, move past impossible positions for a match. */
  अगर (sentinel_लंबाई)
    match_start -= match_length1;

  जबतक (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      अगर (sentinel_लंबाई == 0)
        {
          माप_प्रकार i = match_start - G_बफर;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          अगर (1 < range)
            error (निकास_त्रुटि, 0, _("record too large"));

          अगर (range == 1
              || ((ret = re_खोज (&compiled_separator, G_बफर,
                                    i, i - 1, range, &regs))
                  == -1))
            match_start = G_बफर - 1;
          अन्यथा अगर (ret == -2)
            error (निकास_त्रुटि, 0,
                   _("error in regular expression search"));
          अन्यथा
            {
              match_start = G_बफर + regs.start[0];
              match_लंबाई = regs.end[0] - regs.start[0];
            }
        }
      अन्यथा
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          जबतक (*--match_start != first_char
                 || (match_length1 && !STREQ_LEN (match_start + 1, separator1,
                                                  match_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      अगर (match_start < G_बफर)
        {
          अगर (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_बफर, past_end);
              वापस true;
            }

          saved_record_आकार = past_end - G_बफर;
          अगर (saved_record_आकार > read_आकार)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              अक्षर *newbuffer;
              माप_प्रकार offset = sentinel_लंबाई ? sentinel_लंबाई : 1;
              माप_प्रकार old_G_बफर_आकार = G_बफर_आकार;

              read_आकार *= 2;
              G_बफर_आकार = read_आकार * 2 + sentinel_लंबाई + 2;
              अगर (G_बफर_आकार < old_G_बफर_आकार)
                xalloc_die ();
              newbuffer = xrealloc (G_बफर - offset, G_बफर_आकार);
              newbuffer += offset;
              G_बफर = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          अगर (file_pos >= read_आकार)
            file_pos -= read_आकार;
          अन्यथा
            {
              read_आकार = file_pos;
              file_pos = 0;
            }
          अगर (lseek (निवेश_fd, file_pos, शुरू_से) < 0)
            error (0, त्रुटि_सं, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          स_हटाओ (G_बफर + read_आकार, G_बफर, saved_record_आकार);
          past_end = G_बफर + read_आकार + saved_record_आकार;
          /* For non-regexp searches, avoid unnecessary scanning. */
          अगर (sentinel_लंबाई)
            match_start = G_बफर + read_आकार;
          अन्यथा
            match_start = past_end;

          अगर (full_read (निवेश_fd, G_बफर, read_आकार) != read_आकार)
            {
              error (0, त्रुटि_सं, _("%s: read error"), quotef (file));
              वापस false;
            }
        }
      अन्यथा
        {
          /* Found a match of 'separator'. */
          अगर (separator_ends_record)
            {
              अक्षर *match_end = match_start + match_लंबाई;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              अगर (!first_time || match_end != past_end)
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
          अगर (sentinel_लंबाई > 0)
            match_start -= match_लंबाई - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

अटल off_t
copy_to_अस्थायी (खाता **g_tmp, अक्षर **g_tempfile, पूर्णांक निवेश_fd, अक्षर स्थिर *file)
{
  खाता *fp;
  अक्षर *file_name;
  uintmax_t bytes_copied = 0;
  अगर (!अस्थायी_stream (&fp, &file_name))
    वापस -1;

  जबतक (true)
    {
      ssize_t bytes_read = पढ़ो (निवेश_fd, G_बफर, read_आकार);
      अगर (bytes_read == 0)
        अवरोध;
      अगर (bytes_read < 0)
        {
          error (0, त्रुटि_सं, _("%s: read error"), quotef (file));
          वापस -1;
        }

      अगर (ख_डालो (G_बफर, 1, bytes_read, fp) != bytes_read)
        {
          error (0, त्रुटि_सं, _("%s: write error"), quotef (file_name));
          वापस -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      bytes_copied += bytes_read;
    }

  अगर (ख_साफ (fp) != 0)
    {
      error (0, त्रुटि_सं, _("%s: write error"), quotef (file_name));
      वापस -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  वापस bytes_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

अटल bool
tac_nonseekable (पूर्णांक निवेश_fd, अक्षर स्थिर *file)
{
  खाता *tmp_stream;
  अक्षर *tmp_file;
  off_t bytes_copied = copy_to_अस्थायी (&tmp_stream, &tmp_file, निवेश_fd, file);
  अगर (bytes_copied < 0)
    वापस false;

  bool ok = tac_seekable (fileno (tmp_stream), tmp_file, bytes_copied);
  वापस ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

अटल bool
tac_file (अक्षर स्थिर *filename)
{
  bool ok;
  off_t file_आकार;
  पूर्णांक fd;
  bool is_stdin = streq (filename, "-");

  अगर (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_द्विआधारी_mode (STDIN_FILENO, O_द्विआधारी);
    }
  अन्यथा
    {
      fd = खोलो (filename, O_RDONLY | O_द्विआधारी);
      अगर (fd < 0)
        {
          error (0, त्रुटि_सं, _("failed to open %s for reading"),
                 quoteaf (filename));
          वापस false;
        }
    }

  file_आकार = lseek (fd, 0, अंत_से);

  ok = (file_आकार < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_आकार));

  अगर (!is_stdin && बंद (fd) != 0)
    {
      error (0, त्रुटि_सं, _("%s: read error"), quotef (filename));
      ok = false;
    }
  वापस ok;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अक्षर स्थिर *error_message;	/* Return value from re_compile_pattern. */
  पूर्णांक optc;
  bool ok;
  माप_प्रकार half_बफर_आकार;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  अटल अक्षर स्थिर *स्थिर default_file_list[] = {"-", शून्य};
  अक्षर स्थिर *स्थिर *file;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((optc = getopt_long (argc, argv, "brs:", longopts, शून्य)) != -1)
    {
      चयन (optc)
        {
        हाल 'b':
          separator_ends_record = false;
          अवरोध;
        हाल 'r':
          sentinel_लंबाई = 0;
          अवरोध;
        हाल 's':
          separator = optarg;
          अवरोध;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (sentinel_लंबाई == 0)
    {
      अगर (*separator == 0)
        error (निकास_त्रुटि, 0, _("separator cannot be empty"));

      compiled_separator.बफर = शून्य;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = शून्य;
      error_message = re_compile_pattern (separator, म_माप (separator),
                                          &compiled_separator);
      अगर (error_message)
        error (निकास_त्रुटि, 0, "%s", (error_message));
    }
  अन्यथा
    match_लंबाई = sentinel_लंबाई = *separator ? म_माप (separator) : 1;

  read_आकार = INITIAL_READSIZE;
  जबतक (sentinel_लंबाई >= read_आकार / 2)
    {
      अगर (SIZE_MAX / 2 < read_आकार)
        xalloc_die ();
      read_आकार *= 2;
    }
  half_बफर_आकार = read_आकार + sentinel_लंबाई + 1;
  G_बफर_आकार = 2 * half_बफर_आकार;
  अगर (! (read_आकार < half_बफर_आकार && half_बफर_आकार < G_बफर_आकार))
    xalloc_die ();
  G_बफर = xmalloc (G_बफर_आकार);
  अगर (sentinel_लंबाई)
    {
      स_नकल (G_बफर, separator, sentinel_लंबाई + !!*separator);
      G_बफर += sentinel_लंबाई;
    }
  अन्यथा
    {
      ++G_बफर;
    }

  file = (optind < argc
          ? (अक्षर स्थिर *स्थिर *) &argv[optind]
          : default_file_list);

  xset_द्विआधारी_mode (STDOUT_FILENO, O_द्विआधारी);

  {
    ok = true;
    क्रम (माप_प्रकार i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((अक्षर *) शून्य, (अक्षर *) शून्य);

  अगर (have_read_stdin && बंद (STDIN_FILENO) < 0)
    {
      error (0, त्रुटि_सं, "-");
      ok = false;
    }

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
