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
#परिभाषय क्रमादेशः_NAME "tac"

#परिभाषय AUTHORS \
  proper_name ("Jay Lepreau"), \
  proper_name ("David MacKenzie")


/* The number of bytes per atomic read. */
#परिभाषय INITIAL_READSIZE 8192

/* The number of bytes per atomic write. */
#परिभाषय WRITESIZE 8192

/* The string that separates the records of the file. */
स्थिर वर्ण ध्रुव *separator = "\n";

/* True if we have ever read standard input.  */
स्थिर bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
स्थिर bool separator_अन्ताः_लेखः = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
स्थिर size_t sentinel_दीर्घता = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
स्थिर size_t मेलनम्_दीर्घता;

/* The input buffer. */
स्थिर वर्ण *G_बफर;

/* The number of bytes to read at once into 'buffer'. */
स्थिर size_t read_आकार;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
स्थिर size_t G_बफर_आकार;

/* The compiled regular expression representing 'separator'. */
स्थिर संरचना re_आदर्शः_बफर compiled_separator;
स्थिर वर्ण compiled_separator_fastmap[UCHAR_MAX + 1];
स्थिर संरचना re_registers regs;

स्थिर संरचना option ध्रुव longopts[] =
{
  {"before", no_तर्कः, रिक्त, 'b'},
  {"regex", no_तर्कः, रिक्त, 'r'},
  {"separator", required_तर्कः, रिक्त, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              क्रमादेशः_name);
      fputs (_("\
Write each FILE गन्तव्यम् standard output, चरमः पङ्क्तिः first.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --before             attach the separator before instead सम्बन्धि after\n\
"));
      oputs (_("\
  -r, --regex              interpret the separator as a regular expression\n\
"));
      oputs (_("\
  -s, --separator=STRING   use STRING as the separator instead सम्बन्धि newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\n\
Non-seekable निवेशः is buffered गन्तव्यम् $TMPDIR, defaulting गन्तव्यम् /tmp.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

स्थिर शून्य
output (वर्ण ध्रुव *start, वर्ण ध्रुव *past_अन्तः)
{
  स्थिर वर्ण बफर[WRITESIZE];
  स्थिर size_t अष्टकानि_अन्तर्_बफर = 0;
  size_t अष्टकानि_गन्तव्यम्_सङ्कलय = past_अन्तः - start;
  size_t अष्टकानि_उपलब्धम् = WRITESIZE - अष्टकानि_अन्तर्_बफर;

  यदि (!start)
    {
      यदि (पूर्णम्_write (STDOUT_FILENO, बफर, अष्टकानि_अन्तर्_बफर)
          != अष्टकानि_अन्तर्_बफर)
        write_error ();
      अष्टकानि_अन्तर्_बफर = 0;
      प्रत्यागम;
    }

  /* Write out as many full buffers as possible. */
  यावत् (अष्टकानि_गन्तव्यम्_सङ्कलय >= अष्टकानि_उपलब्धम्)
    {
      स्मृतिं_प्रतिलिख (बफर + अष्टकानि_अन्तर्_बफर, start, अष्टकानि_उपलब्धम्);
      अष्टकानि_गन्तव्यम्_सङ्कलय -= अष्टकानि_उपलब्धम्;
      start += अष्टकानि_उपलब्धम्;
      यदि (पूर्णम्_write (STDOUT_FILENO, बफर, WRITESIZE) != WRITESIZE)
        write_error ();
      अष्टकानि_अन्तर्_बफर = 0;
      अष्टकानि_उपलब्धम् = WRITESIZE;
    }

  स्मृतिं_प्रतिलिख (बफर + अष्टकानि_अन्तर्_बफर, start, अष्टकानि_गन्तव्यम्_सङ्कलय);
  अष्टकानि_अन्तर्_बफर += अष्टकानि_गन्तव्यम्_सङ्कलय;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

स्थिर bool
tac_seekable (पूर्णाङ्क निवेशः_fd, वर्ण ध्रुव *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  वर्ण *मेलनम्_start;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  वर्ण *past_अन्तः;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_लेखः_आकार;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool first_time = true;
  वर्ण first_char = *separator;	/* Speed optimization, non-regexp. */
  वर्ण ध्रुव *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t मेलनम्_length1 = मेलनम्_दीर्घता - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_आकार;
  यदि (remainder != 0)
    {
      file_pos -= remainder;
      यदि (lseek (निवेशः_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  यावत् ((saved_लेखः_आकार = पठ (निवेशः_fd, G_बफर, read_आकार)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_आकार;
      यदि (lseek (निवेशः_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_आकार;
    }

  /* Now scan forward, looking for end of file.  */
  यावत् (saved_लेखः_आकार == read_आकार)
    {
      ssize_t nread = पठ (निवेशः_fd, G_बफर, read_आकार);
      यदि (nread == 0)
        भङ्ग;
      saved_लेखः_आकार = nread;
      यदि (saved_लेखः_आकार < 0)
        भङ्ग;
      file_pos += nread;
    }

  यदि (saved_लेखः_आकार < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      प्रत्यागम false;
    }

  मेलनम्_start = past_अन्तः = G_बफर + saved_लेखः_आकार;
  /* For non-regexp search, move past impossible positions for a match. */
  यदि (sentinel_दीर्घता)
    मेलनम्_start -= मेलनम्_length1;

  यावत् (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      यदि (sentinel_दीर्घता == 0)
        {
          size_t i = मेलनम्_start - G_बफर;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          यदि (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          यदि (range == 1
              || ((ret = re_अन्वेषणम् (&compiled_separator, G_बफर,
                                    i, i - 1, range, &regs))
                  == -1))
            मेलनम्_start = G_बफर - 1;
          अन्यथा यदि (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          अन्यथा
            {
              मेलनम्_start = G_बफर + regs.start[0];
              मेलनम्_दीर्घता = regs.अन्तः[0] - regs.start[0];
            }
        }
      अन्यथा
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          यावत् (*--मेलनम्_start != first_char
                 || (मेलनम्_length1 && !STREQ_LEN (मेलनम्_start + 1, separator1,
                                                  मेलनम्_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      यदि (मेलनम्_start < G_बफर)
        {
          यदि (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_बफर, past_अन्तः);
              प्रत्यागम true;
            }

          saved_लेखः_आकार = past_अन्तः - G_बफर;
          यदि (saved_लेखः_आकार > read_आकार)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              वर्ण *newbuffer;
              size_t विस्थापनम् = sentinel_दीर्घता ? sentinel_दीर्घता : 1;
              size_t old_G_बफर_आकार = G_बफर_आकार;

              read_आकार *= 2;
              G_बफर_आकार = read_आकार * 2 + sentinel_दीर्घता + 2;
              यदि (G_बफर_आकार < old_G_बफर_आकार)
                xalloc_die ();
              newbuffer = xrealloc (G_बफर - विस्थापनम्, G_बफर_आकार);
              newbuffer += विस्थापनम्;
              G_बफर = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          यदि (file_pos >= read_आकार)
            file_pos -= read_आकार;
          अन्यथा
            {
              read_आकार = file_pos;
              file_pos = 0;
            }
          यदि (lseek (निवेशः_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_बफर + read_आकार, G_बफर, saved_लेखः_आकार);
          past_अन्तः = G_बफर + read_आकार + saved_लेखः_आकार;
          /* For non-regexp searches, avoid unnecessary scanning. */
          यदि (sentinel_दीर्घता)
            मेलनम्_start = G_बफर + read_आकार;
          अन्यथा
            मेलनम्_start = past_अन्तः;

          यदि (पूर्णम्_read (निवेशः_fd, G_बफर, read_आकार) != read_आकार)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              प्रत्यागम false;
            }
        }
      अन्यथा
        {
          /* Found a match of 'separator'. */
          यदि (separator_अन्ताः_लेखः)
            {
              वर्ण *मेलनम्_अन्तः = मेलनम्_start + मेलनम्_दीर्घता;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              यदि (!first_time || मेलनम्_अन्तः != past_अन्तः)
                output (मेलनम्_अन्तः, past_अन्तः);
              past_अन्तः = मेलनम्_अन्तः;
              first_time = false;
            }
          अन्यथा
            {
              output (मेलनम्_start, past_अन्तः);
              past_अन्तः = मेलनम्_start;
            }

          /* For non-regex matching, we can back up.  */
          यदि (sentinel_दीर्घता > 0)
            मेलनम्_start -= मेलनम्_दीर्घता - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

स्थिर off_t
copy_गन्तव्यम्_अस्थायी (FILE **g_tmp, वर्ण **g_tempfile, पूर्णाङ्क निवेशः_fd, वर्ण ध्रुव *file)
{
  FILE *fp;
  वर्ण *file_name;
  uintmax_t अष्टकानि_copied = 0;
  यदि (!अस्थायी_प्रवाहः (&fp, &file_name))
    प्रत्यागम -1;

  यावत् (true)
    {
      ssize_t अष्टकानि_read = पठ (निवेशः_fd, G_बफर, read_आकार);
      यदि (अष्टकानि_read == 0)
        भङ्ग;
      यदि (अष्टकानि_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          प्रत्यागम -1;
        }

      यदि (fwrite (G_बफर, 1, अष्टकानि_read, fp) != अष्टकानि_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          प्रत्यागम -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      अष्टकानि_copied += अष्टकानि_read;
    }

  यदि (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      प्रत्यागम -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  प्रत्यागम अष्टकानि_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

स्थिर bool
tac_nonseekable (पूर्णाङ्क निवेशः_fd, वर्ण ध्रुव *file)
{
  FILE *tmp_प्रवाहः;
  वर्ण *tmp_file;
  off_t अष्टकानि_copied = copy_गन्तव्यम्_अस्थायी (&tmp_प्रवाहः, &tmp_file, निवेशः_fd, file);
  यदि (अष्टकानि_copied < 0)
    प्रत्यागम false;

  bool ok = tac_seekable (fileno (tmp_प्रवाहः), tmp_file, अष्टकानि_copied);
  प्रत्यागम ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

स्थिर bool
tac_file (वर्ण ध्रुव *filename)
{
  bool ok;
  off_t file_आकार;
  पूर्णाङ्क fd;
  bool is_stdin = streq (filename, "-");

  यदि (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_द्विमान_बहुलकम् (STDIN_FILENO, O_द्विमान);
    }
  अन्यथा
    {
      fd = उद्घाटय (filename, O_RDONLY | O_द्विमान);
      यदि (fd < 0)
        {
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          प्रत्यागम false;
        }
    }

  file_आकार = lseek (fd, 0, SEEK_END);

  ok = (file_आकार < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_आकार));

  यदि (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  प्रत्यागम ok;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण ध्रुव *error_सन्देशः_2;	/* Return value from re_compile_pattern. */
  पूर्णाङ्क optc;
  bool ok;
  size_t half_बफर_आकार;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  स्थिर वर्ण ध्रुव *ध्रुव default_file_list[] = {"-", रिक्त};
  वर्ण ध्रुव *ध्रुव *file;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((optc = getopt_long (argc, argv, "brs:", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'b':
          separator_अन्ताः_लेखः = false;
          भङ्ग;
        पक्ष 'r':
          sentinel_दीर्घता = 0;
          भङ्ग;
        पक्ष 's':
          separator = optarg;
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (sentinel_दीर्घता == 0)
    {
      यदि (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.बफर = रिक्त;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = रिक्त;
      error_सन्देशः_2 = re_compile_आदर्शः (separator, वाक्यदैर्घ्यम् (separator),
                                          &compiled_separator);
      यदि (error_सन्देशः_2)
        error (EXIT_FAILURE, 0, "%s", (error_सन्देशः_2));
    }
  अन्यथा
    मेलनम्_दीर्घता = sentinel_दीर्घता = *separator ? वाक्यदैर्घ्यम् (separator) : 1;

  read_आकार = INITIAL_READSIZE;
  यावत् (sentinel_दीर्घता >= read_आकार / 2)
    {
      यदि (SIZE_MAX / 2 < read_आकार)
        xalloc_die ();
      read_आकार *= 2;
    }
  half_बफर_आकार = read_आकार + sentinel_दीर्घता + 1;
  G_बफर_आकार = 2 * half_बफर_आकार;
  यदि (! (read_आकार < half_बफर_आकार && half_बफर_आकार < G_बफर_आकार))
    xalloc_die ();
  G_बफर = xmalloc (G_बफर_आकार);
  यदि (sentinel_दीर्घता)
    {
      स्मृतिं_प्रतिलिख (G_बफर, separator, sentinel_दीर्घता + !!*separator);
      G_बफर += sentinel_दीर्घता;
    }
  अन्यथा
    {
      ++G_बफर;
    }

  file = (optind < argc
          ? (वर्ण ध्रुव *ध्रुव *) &argv[optind]
          : default_file_list);

  xset_द्विमान_बहुलकम् (STDOUT_FILENO, O_द्विमान);

  {
    ok = true;
    प्रति (size_t i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((वर्ण *) रिक्त, (वर्ण *) रिक्त);

  यदि (have_read_stdin && close (STDIN_FILENO) < 0)
    {
      error (0, errno, "-");
      ok = false;
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
