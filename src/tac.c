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
#تعریف_کړه پروګرام_NAME "tac"

#تعریف_کړه AUTHORS \
  proper_name ("Jay Lepreau"), \
  proper_name ("David MacKenzie")


/* The number of bytes per atomic read. */
#تعریف_کړه INITIAL_READSIZE 8192

/* The number of bytes per atomic write. */
#تعریف_کړه WRITESIZE 8192

/* The string that separates the records of the file. */
ساكن حرف ثابت *separator = "\n";

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
ساكن bool separator_پایونه_ریکارډ = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
ساكن size_t sentinel_اوږدتیا = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
ساكن size_t سمون_اوږدتیا;

/* The input buffer. */
ساكن حرف *G_بفر;

/* The number of bytes to read at once into 'buffer'. */
ساكن size_t read_اندازه;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
ساكن size_t G_بفر_اندازه;

/* The compiled regular expression representing 'separator'. */
ساكن بنية re_نمونه_بفر compiled_separator;
ساكن حرف compiled_separator_fastmap[UCHAR_MAX + 1];
ساكن بنية re_registers regs;

ساكن بنية option ثابت longopts[] =
{
  {"before", no_دلیل, عدم, 'b'},
  {"regex", no_دلیل, عدم, 'r'},
  {"separator", required_دلیل, عدم, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروګرام_name);
      fputs (_("\
Write each FILE منزل standard output, وروستی کرښه لومړی.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --before             attach the separator before instead د after\n\
"));
      oputs (_("\
  -r, --regex              interpret the separator as a regular افاده_2\n\
"));
      oputs (_("\
  -s, --separator=STRING   use STRING as the separator instead د newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\n\
Non-seekable ننوت is buffered منزل $TMPDIR, defaulting منزل /tmp.\n\
"), stdout);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

ساكن فراغ
output (حرف ثابت *پیل, حرف ثابت *past_پای_ټکی)
{
  ساكن حرف بفر[WRITESIZE];
  ساكن size_t بایټونه_دننه_بفر = 0;
  size_t بایټونه_منزل_جمع_کړه = past_پای_ټکی - پیل;
  size_t بایټونه_شته = WRITESIZE - بایټونه_دننه_بفر;

  إذا (!پیل)
    {
      إذا (ډک_write (STDOUT_FILENO, بفر, بایټونه_دننه_بفر)
          != بایټونه_دننه_بفر)
        write_error ();
      بایټونه_دننه_بفر = 0;
      أرجع;
    }

  /* Write out as many full buffers as possible. */
  طالما (بایټونه_منزل_جمع_کړه >= بایټونه_شته)
    {
      حافظه_نقل (بفر + بایټونه_دننه_بفر, پیل, بایټونه_شته);
      بایټونه_منزل_جمع_کړه -= بایټونه_شته;
      پیل += بایټونه_شته;
      إذا (ډک_write (STDOUT_FILENO, بفر, WRITESIZE) != WRITESIZE)
        write_error ();
      بایټونه_دننه_بفر = 0;
      بایټونه_شته = WRITESIZE;
    }

  حافظه_نقل (بفر + بایټونه_دننه_بفر, پیل, بایټونه_منزل_جمع_کړه);
  بایټونه_دننه_بفر += بایټونه_منزل_جمع_کړه;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

ساكن bool
tac_seekable (صحيح ننوت_fd, حرف ثابت *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  حرف *سمون_پیل;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  حرف *past_پای_ټکی;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_ریکارډ_اندازه;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool لومړی_وخت = true;
  حرف لومړی_char = *separator;	/* Speed optimization, non-regexp. */
  حرف ثابت *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t سمون_length1 = سمون_اوږدتیا - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_اندازه;
  إذا (remainder != 0)
    {
      file_pos -= remainder;
      إذا (lseek (ننوت_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  طالما ((saved_ریکارډ_اندازه = اقرأ (ننوت_fd, G_بفر, read_اندازه)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_اندازه;
      إذا (lseek (ننوت_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_اندازه;
    }

  /* Now scan forward, looking for end of file.  */
  طالما (saved_ریکارډ_اندازه == read_اندازه)
    {
      ssize_t nread = اقرأ (ننوت_fd, G_بفر, read_اندازه);
      إذا (nread == 0)
        اكسر;
      saved_ریکارډ_اندازه = nread;
      إذا (saved_ریکارډ_اندازه < 0)
        اكسر;
      file_pos += nread;
    }

  إذا (saved_ریکارډ_اندازه < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      أرجع false;
    }

  سمون_پیل = past_پای_ټکی = G_بفر + saved_ریکارډ_اندازه;
  /* For non-regexp search, move past impossible positions for a match. */
  إذا (sentinel_اوږدتیا)
    سمون_پیل -= سمون_length1;

  طالما (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      إذا (sentinel_اوږدتیا == 0)
        {
          size_t i = سمون_پیل - G_بفر;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          إذا (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          إذا (range == 1
              || ((ret = re_پلټنه (&compiled_separator, G_بفر,
                                    i, i - 1, range, &regs))
                  == -1))
            سمون_پیل = G_بفر - 1;
          وإلا إذا (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          وإلا
            {
              سمون_پیل = G_بفر + regs.پیل[0];
              سمون_اوږدتیا = regs.پای_ټکی[0] - regs.پیل[0];
            }
        }
      وإلا
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          طالما (*--سمون_پیل != لومړی_char
                 || (سمون_length1 && !STREQ_LEN (سمون_پیل + 1, separator1,
                                                  سمون_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      إذا (سمون_پیل < G_بفر)
        {
          إذا (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_بفر, past_پای_ټکی);
              أرجع true;
            }

          saved_ریکارډ_اندازه = past_پای_ټکی - G_بفر;
          إذا (saved_ریکارډ_اندازه > read_اندازه)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              حرف *newbuffer;
              size_t دننه_واټن = sentinel_اوږدتیا ? sentinel_اوږدتیا : 1;
              size_t old_G_بفر_اندازه = G_بفر_اندازه;

              read_اندازه *= 2;
              G_بفر_اندازه = read_اندازه * 2 + sentinel_اوږدتیا + 2;
              إذا (G_بفر_اندازه < old_G_بفر_اندازه)
                xalloc_die ();
              newbuffer = xrealloc (G_بفر - دننه_واټن, G_بفر_اندازه);
              newbuffer += دننه_واټن;
              G_بفر = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          إذا (file_pos >= read_اندازه)
            file_pos -= read_اندازه;
          وإلا
            {
              read_اندازه = file_pos;
              file_pos = 0;
            }
          إذا (lseek (ننوت_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_بفر + read_اندازه, G_بفر, saved_ریکارډ_اندازه);
          past_پای_ټکی = G_بفر + read_اندازه + saved_ریکارډ_اندازه;
          /* For non-regexp searches, avoid unnecessary scanning. */
          إذا (sentinel_اوږدتیا)
            سمون_پیل = G_بفر + read_اندازه;
          وإلا
            سمون_پیل = past_پای_ټکی;

          إذا (ډک_read (ننوت_fd, G_بفر, read_اندازه) != read_اندازه)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              أرجع false;
            }
        }
      وإلا
        {
          /* Found a match of 'separator'. */
          إذا (separator_پایونه_ریکارډ)
            {
              حرف *سمون_پای_ټکی = سمون_پیل + سمون_اوږدتیا;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              إذا (!لومړی_وخت || سمون_پای_ټکی != past_پای_ټکی)
                output (سمون_پای_ټکی, past_پای_ټکی);
              past_پای_ټکی = سمون_پای_ټکی;
              لومړی_وخت = false;
            }
          وإلا
            {
              output (سمون_پیل, past_پای_ټکی);
              past_پای_ټکی = سمون_پیل;
            }

          /* For non-regex matching, we can back up.  */
          إذا (sentinel_اوږدتیا > 0)
            سمون_پیل -= سمون_اوږدتیا - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

ساكن off_t
copy_منزل_لنډمهاله (FILE **g_tmp, حرف **g_tempfile, صحيح ننوت_fd, حرف ثابت *file)
{
  FILE *fp;
  حرف *file_name;
  uintmax_t بایټونه_copied = 0;
  إذا (!لنډمهاله_بهیر (&fp, &file_name))
    أرجع -1;

  طالما (true)
    {
      ssize_t بایټونه_read = اقرأ (ننوت_fd, G_بفر, read_اندازه);
      إذا (بایټونه_read == 0)
        اكسر;
      إذا (بایټونه_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          أرجع -1;
        }

      إذا (fwrite (G_بفر, 1, بایټونه_read, fp) != بایټونه_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          أرجع -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      بایټونه_copied += بایټونه_read;
    }

  إذا (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      أرجع -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  أرجع بایټونه_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

ساكن bool
tac_nonseekable (صحيح ننوت_fd, حرف ثابت *file)
{
  FILE *tmp_بهیر;
  حرف *tmp_file;
  off_t بایټونه_copied = copy_منزل_لنډمهاله (&tmp_بهیر, &tmp_file, ننوت_fd, file);
  إذا (بایټونه_copied < 0)
    أرجع false;

  bool ok = tac_seekable (fileno (tmp_بهیر), tmp_file, بایټونه_copied);
  أرجع ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

ساكن bool
tac_file (حرف ثابت *filename)
{
  bool ok;
  off_t file_اندازه;
  صحيح fd;
  bool is_stdin = streq (filename, "-");

  إذا (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_دوګونی_ډېرپېښ (STDIN_FILENO, O_دوګونی);
    }
  وإلا
    {
      fd = open (filename, O_RDONLY | O_دوګونی);
      إذا (fd < 0)
        {
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          أرجع false;
        }
    }

  file_اندازه = lseek (fd, 0, SEEK_END);

  ok = (file_اندازه < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_اندازه));

  إذا (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  أرجع ok;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  حرف ثابت *error_پیغام_2;	/* Return value from re_compile_pattern. */
  صحيح optc;
  bool ok;
  size_t half_بفر_اندازه;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ساكن حرف ثابت *ثابت default_file_list[] = {"-", عدم};
  حرف ثابت *ثابت *file;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "brs:", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'b':
          separator_پایونه_ریکارډ = false;
          اكسر;
        حالة 'r':
          sentinel_اوږدتیا = 0;
          اكسر;
        حالة 's':
          separator = optarg;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (sentinel_اوږدتیا == 0)
    {
      إذا (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.بفر = عدم;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = عدم;
      error_پیغام_2 = re_compile_نمونه (separator, مزي_اوږدوالی (separator),
                                          &compiled_separator);
      إذا (error_پیغام_2)
        error (EXIT_FAILURE, 0, "%s", (error_پیغام_2));
    }
  وإلا
    سمون_اوږدتیا = sentinel_اوږدتیا = *separator ? مزي_اوږدوالی (separator) : 1;

  read_اندازه = INITIAL_READSIZE;
  طالما (sentinel_اوږدتیا >= read_اندازه / 2)
    {
      إذا (SIZE_MAX / 2 < read_اندازه)
        xalloc_die ();
      read_اندازه *= 2;
    }
  half_بفر_اندازه = read_اندازه + sentinel_اوږدتیا + 1;
  G_بفر_اندازه = 2 * half_بفر_اندازه;
  إذا (! (read_اندازه < half_بفر_اندازه && half_بفر_اندازه < G_بفر_اندازه))
    xalloc_die ();
  G_بفر = xmalloc (G_بفر_اندازه);
  إذا (sentinel_اوږدتیا)
    {
      حافظه_نقل (G_بفر, separator, sentinel_اوږدتیا + !!*separator);
      G_بفر += sentinel_اوږدتیا;
    }
  وإلا
    {
      ++G_بفر;
    }

  file = (optind < argc
          ? (حرف ثابت *ثابت *) &argv[optind]
          : default_file_list);

  xset_دوګونی_ډېرپېښ (STDOUT_FILENO, O_دوګونی);

  {
    ok = true;
    لكل (size_t i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((حرف *) عدم, (حرف *) عدم);

  إذا (have_read_stdin && close (STDIN_FILENO) < 0)
    {
      error (0, errno, "-");
      ok = false;
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
