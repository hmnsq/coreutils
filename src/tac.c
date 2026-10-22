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
#تعریف_کن برنامه_NAME "tac"

#تعریف_کن AUTHORS \
  proper_name ("Jay Lepreau"), \
  proper_name ("David MacKenzie")


/* The number of bytes per atomic read. */
#تعریف_کن INITIAL_READSIZE 8192

/* The number of bytes per atomic write. */
#تعریف_کن WRITESIZE 8192

/* The string that separates the records of the file. */
ساكن حرف ثابت *separator = "\n";

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
ساكن bool separator_دوسر_رکورد = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
ساكن size_t sentinel_درازی = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
ساكن size_t تطابق_درازی;

/* The input buffer. */
ساكن حرف *G_میانگیر;

/* The number of bytes to read at once into 'buffer'. */
ساكن size_t read_اندازه;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
ساكن size_t G_میانگیر_اندازه;

/* The compiled regular expression representing 'separator'. */
ساكن بنية re_الگو_میانگیر compiled_separator;
ساكن حرف compiled_separator_fastmap[UCHAR_MAX + 1];
ساكن بنية re_registers regs;

ساكن بنية option ثابت longopts[] =
{
  {"before", no_آرگومان, عدم, 'b'},
  {"regex", no_آرگومان, عدم, 'r'},
  {"separator", required_آرگومان, عدم, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              برنامه_name);
      fputs (_("\
Write each FILE مقصد standard output, آخرین سطر اولی.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --before             attach the separator before instead از after\n\
"));
      oputs (_("\
  -r, --regex              interpret the separator as a regular عبارت_2\n\
"));
      oputs (_("\
  -s, --separator=STRING   use STRING as the separator instead از newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\n\
Non-seekable درون_داد is buffered مقصد $TMPDIR, defaulting مقصد /tmp.\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

ساكن فراغ
output (حرف ثابت *شروع, حرف ثابت *past_انتها)
{
  ساكن حرف میانگیر[WRITESIZE];
  ساكن size_t بایتها_درون_میانگیر = 0;
  size_t بایتها_مقصد_جمع_کن = past_انتها - شروع;
  size_t بایتها_موجود = WRITESIZE - بایتها_درون_میانگیر;

  إذا (!شروع)
    {
      إذا (پر_write (STDOUT_FILENO, میانگیر, بایتها_درون_میانگیر)
          != بایتها_درون_میانگیر)
        write_error ();
      بایتها_درون_میانگیر = 0;
      أرجع;
    }

  /* Write out as many full buffers as possible. */
  طالما (بایتها_مقصد_جمع_کن >= بایتها_موجود)
    {
      کپی_حافظه (میانگیر + بایتها_درون_میانگیر, شروع, بایتها_موجود);
      بایتها_مقصد_جمع_کن -= بایتها_موجود;
      شروع += بایتها_موجود;
      إذا (پر_write (STDOUT_FILENO, میانگیر, WRITESIZE) != WRITESIZE)
        write_error ();
      بایتها_درون_میانگیر = 0;
      بایتها_موجود = WRITESIZE;
    }

  کپی_حافظه (میانگیر + بایتها_درون_میانگیر, شروع, بایتها_مقصد_جمع_کن);
  بایتها_درون_میانگیر += بایتها_مقصد_جمع_کن;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

ساكن bool
tac_seekable (صحيح درون_داد_fd, حرف ثابت *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  حرف *تطابق_شروع;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  حرف *past_انتها;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_رکورد_اندازه;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool اولی_زمان = true;
  حرف اولی_char = *separator;	/* Speed optimization, non-regexp. */
  حرف ثابت *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t تطابق_length1 = تطابق_درازی - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_اندازه;
  إذا (remainder != 0)
    {
      file_pos -= remainder;
      إذا (lseek (درون_داد_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  طالما ((saved_رکورد_اندازه = اقرأ (درون_داد_fd, G_میانگیر, read_اندازه)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_اندازه;
      إذا (lseek (درون_داد_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_اندازه;
    }

  /* Now scan forward, looking for end of file.  */
  طالما (saved_رکورد_اندازه == read_اندازه)
    {
      ssize_t nread = اقرأ (درون_داد_fd, G_میانگیر, read_اندازه);
      إذا (nread == 0)
        اكسر;
      saved_رکورد_اندازه = nread;
      إذا (saved_رکورد_اندازه < 0)
        اكسر;
      file_pos += nread;
    }

  إذا (saved_رکورد_اندازه < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      أرجع false;
    }

  تطابق_شروع = past_انتها = G_میانگیر + saved_رکورد_اندازه;
  /* For non-regexp search, move past impossible positions for a match. */
  إذا (sentinel_درازی)
    تطابق_شروع -= تطابق_length1;

  طالما (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      إذا (sentinel_درازی == 0)
        {
          size_t i = تطابق_شروع - G_میانگیر;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          إذا (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          إذا (range == 1
              || ((ret = re_جستجو (&compiled_separator, G_میانگیر,
                                    i, i - 1, range, &regs))
                  == -1))
            تطابق_شروع = G_میانگیر - 1;
          وإلا إذا (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          وإلا
            {
              تطابق_شروع = G_میانگیر + regs.شروع[0];
              تطابق_درازی = regs.انتها[0] - regs.شروع[0];
            }
        }
      وإلا
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          طالما (*--تطابق_شروع != اولی_char
                 || (تطابق_length1 && !STREQ_LEN (تطابق_شروع + 1, separator1,
                                                  تطابق_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      إذا (تطابق_شروع < G_میانگیر)
        {
          إذا (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_میانگیر, past_انتها);
              أرجع true;
            }

          saved_رکورد_اندازه = past_انتها - G_میانگیر;
          إذا (saved_رکورد_اندازه > read_اندازه)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              حرف *newbuffer;
              size_t جابه_جایی = sentinel_درازی ? sentinel_درازی : 1;
              size_t old_G_میانگیر_اندازه = G_میانگیر_اندازه;

              read_اندازه *= 2;
              G_میانگیر_اندازه = read_اندازه * 2 + sentinel_درازی + 2;
              إذا (G_میانگیر_اندازه < old_G_میانگیر_اندازه)
                xalloc_die ();
              newbuffer = xrealloc (G_میانگیر - جابه_جایی, G_میانگیر_اندازه);
              newbuffer += جابه_جایی;
              G_میانگیر = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          إذا (file_pos >= read_اندازه)
            file_pos -= read_اندازه;
          وإلا
            {
              read_اندازه = file_pos;
              file_pos = 0;
            }
          إذا (lseek (درون_داد_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_میانگیر + read_اندازه, G_میانگیر, saved_رکورد_اندازه);
          past_انتها = G_میانگیر + read_اندازه + saved_رکورد_اندازه;
          /* For non-regexp searches, avoid unnecessary scanning. */
          إذا (sentinel_درازی)
            تطابق_شروع = G_میانگیر + read_اندازه;
          وإلا
            تطابق_شروع = past_انتها;

          إذا (پر_read (درون_داد_fd, G_میانگیر, read_اندازه) != read_اندازه)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              أرجع false;
            }
        }
      وإلا
        {
          /* Found a match of 'separator'. */
          إذا (separator_دوسر_رکورد)
            {
              حرف *تطابق_انتها = تطابق_شروع + تطابق_درازی;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              إذا (!اولی_زمان || تطابق_انتها != past_انتها)
                output (تطابق_انتها, past_انتها);
              past_انتها = تطابق_انتها;
              اولی_زمان = false;
            }
          وإلا
            {
              output (تطابق_شروع, past_انتها);
              past_انتها = تطابق_شروع;
            }

          /* For non-regex matching, we can back up.  */
          إذا (sentinel_درازی > 0)
            تطابق_شروع -= تطابق_درازی - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

ساكن off_t
copy_مقصد_موقت (FILE **g_tmp, حرف **g_tempfile, صحيح درون_داد_fd, حرف ثابت *file)
{
  FILE *fp;
  حرف *file_name;
  uintmax_t بایتها_copied = 0;
  إذا (!موقت_جریان (&fp, &file_name))
    أرجع -1;

  طالما (true)
    {
      ssize_t بایتها_read = اقرأ (درون_داد_fd, G_میانگیر, read_اندازه);
      إذا (بایتها_read == 0)
        اكسر;
      إذا (بایتها_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          أرجع -1;
        }

      إذا (fwrite (G_میانگیر, 1, بایتها_read, fp) != بایتها_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          أرجع -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      بایتها_copied += بایتها_read;
    }

  إذا (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      أرجع -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  أرجع بایتها_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

ساكن bool
tac_nonseekable (صحيح درون_داد_fd, حرف ثابت *file)
{
  FILE *tmp_جریان;
  حرف *tmp_file;
  off_t بایتها_copied = copy_مقصد_موقت (&tmp_جریان, &tmp_file, درون_داد_fd, file);
  إذا (بایتها_copied < 0)
    أرجع false;

  bool ok = tac_seekable (fileno (tmp_جریان), tmp_file, بایتها_copied);
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
      xset_دودویی_نما (STDIN_FILENO, O_دودویی);
    }
  وإلا
    {
      fd = open (filename, O_RDONLY | O_دودویی);
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
اصلی (صحيح argc, حرف **argv)
{
  حرف ثابت *error_پیام_2;	/* Return value from re_compile_pattern. */
  صحيح optc;
  bool ok;
  size_t half_میانگیر_اندازه;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ساكن حرف ثابت *ثابت default_file_list[] = {"-", عدم};
  حرف ثابت *ثابت *file;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "brs:", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'b':
          separator_دوسر_رکورد = false;
          اكسر;
        حالة 'r':
          sentinel_درازی = 0;
          اكسر;
        حالة 's':
          separator = optarg;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (sentinel_درازی == 0)
    {
      إذا (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.میانگیر = عدم;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = عدم;
      error_پیام_2 = re_compile_الگو (separator, طول_رشته (separator),
                                          &compiled_separator);
      إذا (error_پیام_2)
        error (EXIT_FAILURE, 0, "%s", (error_پیام_2));
    }
  وإلا
    تطابق_درازی = sentinel_درازی = *separator ? طول_رشته (separator) : 1;

  read_اندازه = INITIAL_READSIZE;
  طالما (sentinel_درازی >= read_اندازه / 2)
    {
      إذا (SIZE_MAX / 2 < read_اندازه)
        xalloc_die ();
      read_اندازه *= 2;
    }
  half_میانگیر_اندازه = read_اندازه + sentinel_درازی + 1;
  G_میانگیر_اندازه = 2 * half_میانگیر_اندازه;
  إذا (! (read_اندازه < half_میانگیر_اندازه && half_میانگیر_اندازه < G_میانگیر_اندازه))
    xalloc_die ();
  G_میانگیر = xmalloc (G_میانگیر_اندازه);
  إذا (sentinel_درازی)
    {
      کپی_حافظه (G_میانگیر, separator, sentinel_درازی + !!*separator);
      G_میانگیر += sentinel_درازی;
    }
  وإلا
    {
      ++G_میانگیر;
    }

  file = (optind < argc
          ? (حرف ثابت *ثابت *) &argv[optind]
          : default_file_list);

  xset_دودویی_نما (STDOUT_FILENO, O_دودویی);

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
