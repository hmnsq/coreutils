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
#وصف_ڪريو پروگرام_NAME "tac"

#وصف_ڪريو AUTHORS \
  proper_name ("Jay Lepreau"), \
  proper_name ("David MacKenzie")


/* The number of bytes per atomic read. */
#وصف_ڪريو INITIAL_READSIZE 8192

/* The number of bytes per atomic write. */
#وصف_ڪريو WRITESIZE 8192

/* The string that separates the records of the file. */
ساكن حرف ثابت *separator = "\n";

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
ساكن bool separator_پڇاڙيون_رڪارڊ = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
ساكن size_t sentinel_ڊيگهائي = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
ساكن size_t ميلاپ_ڊيگهائي;

/* The input buffer. */
ساكن حرف *G_بفر;

/* The number of bytes to read at once into 'buffer'. */
ساكن size_t read_سائيز;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
ساكن size_t G_بفر_سائيز;

/* The compiled regular expression representing 'separator'. */
ساكن بنية re_نمونو_بفر compiled_separator;
ساكن حرف compiled_separator_fastmap[UCHAR_MAX + 1];
ساكن بنية re_registers regs;

ساكن بنية option ثابت longopts[] =
{
  {"before", no_دليل, عدم, 'b'},
  {"regex", no_دليل, عدم, 'r'},
  {"separator", required_دليل, عدم, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Write each FILE منزل standard output, آخري سٽ پهريون.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --before             attach the separator before instead جو after\n\
"));
      oputs (_("\
  -r, --regex              interpret the separator as a regular اظهار_2\n\
"));
      oputs (_("\
  -s, --separator=STRING   use STRING as the separator instead جو newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\n\
Non-seekable داخلا is buffered منزل $TMPDIR, defaulting منزل /tmp.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

ساكن فراغ
output (حرف ثابت *شروعات, حرف ثابت *past_پڇاڙي)
{
  ساكن حرف بفر[WRITESIZE];
  ساكن size_t بائيٽون_اندريون_بفر = 0;
  size_t بائيٽون_منزل_جوڙيو = past_پڇاڙي - شروعات;
  size_t بائيٽون_موجود = WRITESIZE - بائيٽون_اندريون_بفر;

  إذا (!شروعات)
    {
      إذا (ڀريل_write (STDOUT_FILENO, بفر, بائيٽون_اندريون_بفر)
          != بائيٽون_اندريون_بفر)
        write_error ();
      بائيٽون_اندريون_بفر = 0;
      أرجع;
    }

  /* Write out as many full buffers as possible. */
  طالما (بائيٽون_منزل_جوڙيو >= بائيٽون_موجود)
    {
      يادگيري_نقل (بفر + بائيٽون_اندريون_بفر, شروعات, بائيٽون_موجود);
      بائيٽون_منزل_جوڙيو -= بائيٽون_موجود;
      شروعات += بائيٽون_موجود;
      إذا (ڀريل_write (STDOUT_FILENO, بفر, WRITESIZE) != WRITESIZE)
        write_error ();
      بائيٽون_اندريون_بفر = 0;
      بائيٽون_موجود = WRITESIZE;
    }

  يادگيري_نقل (بفر + بائيٽون_اندريون_بفر, شروعات, بائيٽون_منزل_جوڙيو);
  بائيٽون_اندريون_بفر += بائيٽون_منزل_جوڙيو;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

ساكن bool
tac_seekable (صحيح داخلا_fd, حرف ثابت *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  حرف *ميلاپ_شروعات;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  حرف *past_پڇاڙي;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_رڪارڊ_سائيز;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool پهريون_وقت = true;
  حرف پهريون_char = *separator;	/* Speed optimization, non-regexp. */
  حرف ثابت *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t ميلاپ_length1 = ميلاپ_ڊيگهائي - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_سائيز;
  إذا (remainder != 0)
    {
      file_pos -= remainder;
      إذا (lseek (داخلا_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  طالما ((saved_رڪارڊ_سائيز = اقرأ (داخلا_fd, G_بفر, read_سائيز)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_سائيز;
      إذا (lseek (داخلا_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_سائيز;
    }

  /* Now scan forward, looking for end of file.  */
  طالما (saved_رڪارڊ_سائيز == read_سائيز)
    {
      ssize_t nread = اقرأ (داخلا_fd, G_بفر, read_سائيز);
      إذا (nread == 0)
        اكسر;
      saved_رڪارڊ_سائيز = nread;
      إذا (saved_رڪارڊ_سائيز < 0)
        اكسر;
      file_pos += nread;
    }

  إذا (saved_رڪارڊ_سائيز < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      أرجع false;
    }

  ميلاپ_شروعات = past_پڇاڙي = G_بفر + saved_رڪارڊ_سائيز;
  /* For non-regexp search, move past impossible positions for a match. */
  إذا (sentinel_ڊيگهائي)
    ميلاپ_شروعات -= ميلاپ_length1;

  طالما (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      إذا (sentinel_ڊيگهائي == 0)
        {
          size_t i = ميلاپ_شروعات - G_بفر;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          إذا (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          إذا (range == 1
              || ((ret = re_ڳولا (&compiled_separator, G_بفر,
                                    i, i - 1, range, &regs))
                  == -1))
            ميلاپ_شروعات = G_بفر - 1;
          وإلا إذا (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          وإلا
            {
              ميلاپ_شروعات = G_بفر + regs.شروعات[0];
              ميلاپ_ڊيگهائي = regs.پڇاڙي[0] - regs.شروعات[0];
            }
        }
      وإلا
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          طالما (*--ميلاپ_شروعات != پهريون_char
                 || (ميلاپ_length1 && !STREQ_LEN (ميلاپ_شروعات + 1, separator1,
                                                  ميلاپ_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      إذا (ميلاپ_شروعات < G_بفر)
        {
          إذا (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_بفر, past_پڇاڙي);
              أرجع true;
            }

          saved_رڪارڊ_سائيز = past_پڇاڙي - G_بفر;
          إذا (saved_رڪارڊ_سائيز > read_سائيز)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              حرف *newbuffer;
              size_t وٿي = sentinel_ڊيگهائي ? sentinel_ڊيگهائي : 1;
              size_t old_G_بفر_سائيز = G_بفر_سائيز;

              read_سائيز *= 2;
              G_بفر_سائيز = read_سائيز * 2 + sentinel_ڊيگهائي + 2;
              إذا (G_بفر_سائيز < old_G_بفر_سائيز)
                xalloc_die ();
              newbuffer = xrealloc (G_بفر - وٿي, G_بفر_سائيز);
              newbuffer += وٿي;
              G_بفر = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          إذا (file_pos >= read_سائيز)
            file_pos -= read_سائيز;
          وإلا
            {
              read_سائيز = file_pos;
              file_pos = 0;
            }
          إذا (lseek (داخلا_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_بفر + read_سائيز, G_بفر, saved_رڪارڊ_سائيز);
          past_پڇاڙي = G_بفر + read_سائيز + saved_رڪارڊ_سائيز;
          /* For non-regexp searches, avoid unnecessary scanning. */
          إذا (sentinel_ڊيگهائي)
            ميلاپ_شروعات = G_بفر + read_سائيز;
          وإلا
            ميلاپ_شروعات = past_پڇاڙي;

          إذا (ڀريل_read (داخلا_fd, G_بفر, read_سائيز) != read_سائيز)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              أرجع false;
            }
        }
      وإلا
        {
          /* Found a match of 'separator'. */
          إذا (separator_پڇاڙيون_رڪارڊ)
            {
              حرف *ميلاپ_پڇاڙي = ميلاپ_شروعات + ميلاپ_ڊيگهائي;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              إذا (!پهريون_وقت || ميلاپ_پڇاڙي != past_پڇاڙي)
                output (ميلاپ_پڇاڙي, past_پڇاڙي);
              past_پڇاڙي = ميلاپ_پڇاڙي;
              پهريون_وقت = false;
            }
          وإلا
            {
              output (ميلاپ_شروعات, past_پڇاڙي);
              past_پڇاڙي = ميلاپ_شروعات;
            }

          /* For non-regex matching, we can back up.  */
          إذا (sentinel_ڊيگهائي > 0)
            ميلاپ_شروعات -= ميلاپ_ڊيگهائي - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

ساكن off_t
copy_منزل_عارضي (FILE **g_tmp, حرف **g_tempfile, صحيح داخلا_fd, حرف ثابت *file)
{
  FILE *fp;
  حرف *file_name;
  uintmax_t بائيٽون_copied = 0;
  إذا (!عارضي_وهڪرو (&fp, &file_name))
    أرجع -1;

  طالما (true)
    {
      ssize_t بائيٽون_read = اقرأ (داخلا_fd, G_بفر, read_سائيز);
      إذا (بائيٽون_read == 0)
        اكسر;
      إذا (بائيٽون_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          أرجع -1;
        }

      إذا (fwrite (G_بفر, 1, بائيٽون_read, fp) != بائيٽون_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          أرجع -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      بائيٽون_copied += بائيٽون_read;
    }

  إذا (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      أرجع -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  أرجع بائيٽون_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

ساكن bool
tac_nonseekable (صحيح داخلا_fd, حرف ثابت *file)
{
  FILE *tmp_وهڪرو;
  حرف *tmp_file;
  off_t بائيٽون_copied = copy_منزل_عارضي (&tmp_وهڪرو, &tmp_file, داخلا_fd, file);
  إذا (بائيٽون_copied < 0)
    أرجع false;

  bool ok = tac_seekable (fileno (tmp_وهڪرو), tmp_file, بائيٽون_copied);
  أرجع ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

ساكن bool
tac_file (حرف ثابت *filename)
{
  bool ok;
  off_t file_سائيز;
  صحيح fd;
  bool is_stdin = streq (filename, "-");

  إذا (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_ٻٽو_گهڻائي (STDIN_FILENO, O_ٻٽو);
    }
  وإلا
    {
      fd = open (filename, O_RDONLY | O_ٻٽو);
      إذا (fd < 0)
        {
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          أرجع false;
        }
    }

  file_سائيز = lseek (fd, 0, SEEK_END);

  ok = (file_سائيز < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_سائيز));

  إذا (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  أرجع ok;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  حرف ثابت *error_پيغام;	/* Return value from re_compile_pattern. */
  صحيح optc;
  bool ok;
  size_t half_بفر_سائيز;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ساكن حرف ثابت *ثابت default_file_list[] = {"-", عدم};
  حرف ثابت *ثابت *file;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "brs:", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'b':
          separator_پڇاڙيون_رڪارڊ = false;
          اكسر;
        حالة 'r':
          sentinel_ڊيگهائي = 0;
          اكسر;
        حالة 's':
          separator = optarg;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (sentinel_ڊيگهائي == 0)
    {
      إذا (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.بفر = عدم;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = عدم;
      error_پيغام = re_compile_نمونو (separator, لڙي_ڊيگهه (separator),
                                          &compiled_separator);
      إذا (error_پيغام)
        error (EXIT_FAILURE, 0, "%s", (error_پيغام));
    }
  وإلا
    ميلاپ_ڊيگهائي = sentinel_ڊيگهائي = *separator ? لڙي_ڊيگهه (separator) : 1;

  read_سائيز = INITIAL_READSIZE;
  طالما (sentinel_ڊيگهائي >= read_سائيز / 2)
    {
      إذا (SIZE_MAX / 2 < read_سائيز)
        xalloc_die ();
      read_سائيز *= 2;
    }
  half_بفر_سائيز = read_سائيز + sentinel_ڊيگهائي + 1;
  G_بفر_سائيز = 2 * half_بفر_سائيز;
  إذا (! (read_سائيز < half_بفر_سائيز && half_بفر_سائيز < G_بفر_سائيز))
    xalloc_die ();
  G_بفر = xmalloc (G_بفر_سائيز);
  إذا (sentinel_ڊيگهائي)
    {
      يادگيري_نقل (G_بفر, separator, sentinel_ڊيگهائي + !!*separator);
      G_بفر += sentinel_ڊيگهائي;
    }
  وإلا
    {
      ++G_بفر;
    }

  file = (optind < argc
          ? (حرف ثابت *ثابت *) &argv[optind]
          : default_file_list);

  xset_ٻٽو_گهڻائي (STDOUT_FILENO, O_ٻٽو);

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
