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

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل "system.h"

#شامل <regex.h>

#شامل "filenamecat.h"
#شامل "full-read.h"
#شامل "full-write.h"
#شامل "temp-stream.h"
#شامل "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "tac"

#تعریف_کرو AUTHORS \
  proper_name ("Jay Lepreau"), \
  proper_name ("David MacKenzie")


/* The number of bytes per atomic read. */
#تعریف_کرو INITIAL_READSIZE 8192

/* The number of bytes per atomic write. */
#تعریف_کرو WRITESIZE 8192

/* The string that separates the records of the file. */
ساکن حرف ثابت *separator = "\n";

/* True if we have ever read standard input.  */
ساکن bool have_read_stdin = false;

/* If true, print 'separator' along with the record preceding it
   in the file; otherwise with the record following it. */
ساکن bool separator_سرے_ریکارڈ = true;

/* 0 if 'separator' is to be matched as a regular expression;
   otherwise, the length of 'separator', used as a sentinel to
   stop the search. */
ساکن size_t sentinel_لمبائی = 1;

/* The length of a match with 'separator'.  If 'sentinel_length' is 0,
   'match_length' is computed every time a match succeeds;
   otherwise, it is simply the length of 'separator'. */
ساکن size_t مطابقت_لمبائی;

/* The input buffer. */
ساکن حرف *G_بفر;

/* The number of bytes to read at once into 'buffer'. */
ساکن size_t read_سائز;

/* The size of 'buffer'.  This is read_size * 2 + sentinel_length + 2.
   The extra 2 bytes allow 'past_end' to have a value beyond the
   end of 'G_buffer' and 'match_start' to run off the front of 'G_buffer'. */
ساکن size_t G_بفر_سائز;

/* The compiled regular expression representing 'separator'. */
ساکن ساخت re_نمونہ_بفر compiled_separator;
ساکن حرف compiled_separator_fastmap[UCHAR_MAX + 1];
ساکن ساخت re_registers regs;

ساکن ساخت option ثابت longopts[] =
{
  {"before", no_دلیل, تہی, 'b'},
  {"regex", no_دلیل, تہی, 'r'},
  {"separator", required_دلیل, تہی, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Write each FILE مقصد standard output, آخری سطر پہلا.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --before             attach the separator before instead کا after\n\
"));
      oputs (_("\
  -r, --regex              interpret the separator as a regular عبارت_2\n\
"));
      oputs (_("\
  -s, --separator=STRING   use STRING as the separator instead کا newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\n\
Non-seekable داخلہ is buffered مقصد $TMPDIR, defaulting مقصد /tmp.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Print the characters from START to PAST_END - 1.
   If START is null, just flush the buffer. */

ساکن خالی
output (حرف ثابت *آغاز, حرف ثابت *past_انتہا)
{
  ساکن حرف بفر[WRITESIZE];
  ساکن size_t بائٹیں_اندر_بفر = 0;
  size_t بائٹیں_مقصد_جوڑو = past_انتہا - آغاز;
  size_t بائٹیں_دستیاب = WRITESIZE - بائٹیں_اندر_بفر;

  اگر (!آغاز)
    {
      اگر (بھرا_write (STDOUT_FILENO, بفر, بائٹیں_اندر_بفر)
          != بائٹیں_اندر_بفر)
        write_error ();
      بائٹیں_اندر_بفر = 0;
      واپس;
    }

  /* Write out as many full buffers as possible. */
  جب (بائٹیں_مقصد_جوڑو >= بائٹیں_دستیاب)
    {
      حافظہ_نقل (بفر + بائٹیں_اندر_بفر, آغاز, بائٹیں_دستیاب);
      بائٹیں_مقصد_جوڑو -= بائٹیں_دستیاب;
      آغاز += بائٹیں_دستیاب;
      اگر (بھرا_write (STDOUT_FILENO, بفر, WRITESIZE) != WRITESIZE)
        write_error ();
      بائٹیں_اندر_بفر = 0;
      بائٹیں_دستیاب = WRITESIZE;
    }

  حافظہ_نقل (بفر + بائٹیں_اندر_بفر, آغاز, بائٹیں_مقصد_جوڑو);
  بائٹیں_اندر_بفر += بائٹیں_مقصد_جوڑو;
}

/* Print in reverse the file open on descriptor FD for reading FILE.
   The file is already positioned at FILE_POS, which should be near its end.
   Return true if successful.  */

ساکن bool
tac_seekable (صحیح داخلہ_fd, حرف ثابت *file, off_t file_pos)
{
  /* Pointer to the location in 'G_buffer' where the search for
     the next separator will begin. */
  حرف *مطابقت_آغاز;

  /* Pointer to one past the rightmost character in 'G_buffer' that
     has not been printed yet. */
  حرف *past_انتہا;

  /* Length of the record growing in 'G_buffer'. */
  ptrdiff_t saved_ریکارڈ_سائز;

  /* True if 'output' has not been called yet for any file.
     Only used when the separator is attached to the preceding record. */
  bool پہلا_وقت = true;
  حرف پہلا_char = *separator;	/* Speed optimization, non-regexp. */
  حرف ثابت *separator1 = separator + 1; /* Speed optimization, non-regexp. */
  size_t مطابقت_length1 = مطابقت_لمبائی - 1; /* Speed optimization, non-regexp. */

  /* Arrange for the first read to lop off enough to leave the rest of the
     file a multiple of 'read_size'.  Since 'read_size' can change, this may
     not always hold during the program run, but since it usually will, leave
     it here for i/o efficiency (page/sector boundaries and all that).
     Note: the efficiency gain has not been verified. */
  size_t remainder = file_pos % read_سائز;
  اگر (remainder != 0)
    {
      file_pos -= remainder;
      اگر (lseek (داخلہ_fd, file_pos, SEEK_SET) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
    }

  /* Scan backward, looking for end of file.  This caters to proc-like
     file systems where the file size is just an estimate.  */
  جب ((saved_ریکارڈ_سائز = اقرأ (داخلہ_fd, G_بفر, read_سائز)) == 0
         && file_pos != 0)
    {
      off_t rsize = read_سائز;
      اگر (lseek (داخلہ_fd, -rsize, SEEK_CUR) < 0)
        error (0, errno, _("%s: seek failed"), quotef (file));
      file_pos -= read_سائز;
    }

  /* Now scan forward, looking for end of file.  */
  جب (saved_ریکارڈ_سائز == read_سائز)
    {
      ssize_t nread = اقرأ (داخلہ_fd, G_بفر, read_سائز);
      اگر (nread == 0)
        توڑو;
      saved_ریکارڈ_سائز = nread;
      اگر (saved_ریکارڈ_سائز < 0)
        توڑو;
      file_pos += nread;
    }

  اگر (saved_ریکارڈ_سائز < 0)
    {
      error (0, errno, _("%s: read error"), quotef (file));
      واپس false;
    }

  مطابقت_آغاز = past_انتہا = G_بفر + saved_ریکارڈ_سائز;
  /* For non-regexp search, move past impossible positions for a match. */
  اگر (sentinel_لمبائی)
    مطابقت_آغاز -= مطابقت_length1;

  جب (true)
    {
      /* Search backward from 'match_start' - 1 to 'G_buffer' for a match
         with 'separator'; for speed, use strncmp if 'separator' contains no
         metacharacters.
         If the match succeeds, set 'match_start' to point to the start of
         the match and 'match_length' to the length of the match.
         Otherwise, make 'match_start' < 'G_buffer'. */
      اگر (sentinel_لمبائی == 0)
        {
          size_t i = مطابقت_آغاز - G_بفر;
          regoff_t ri = i;
          regoff_t range = 1 - ri;
          regoff_t ret;

          اگر (1 < range)
            error (EXIT_FAILURE, 0, _("record too large"));

          اگر (range == 1
              || ((ret = re_تلاش (&compiled_separator, G_بفر,
                                    i, i - 1, range, &regs))
                  == -1))
            مطابقت_آغاز = G_بفر - 1;
          ورنہ اگر (ret == -2)
            error (EXIT_FAILURE, 0,
                   _("error in regular expression search"));
          ورنہ
            {
              مطابقت_آغاز = G_بفر + regs.آغاز[0];
              مطابقت_لمبائی = regs.انتہا[0] - regs.آغاز[0];
            }
        }
      ورنہ
        {
          /* 'match_length' is constant for non-regexp boundaries. */
          جب (*--مطابقت_آغاز != پہلا_char
                 || (مطابقت_length1 && !STREQ_LEN (مطابقت_آغاز + 1, separator1,
                                                  مطابقت_length1)))
            /* Do nothing. */ ;
        }

      /* Check whether we backed off the front of 'G_buffer' without finding
         a match for 'separator'. */
      اگر (مطابقت_آغاز < G_بفر)
        {
          اگر (file_pos == 0)
            {
              /* Hit the beginning of the file; print the remaining record. */
              output (G_بفر, past_انتہا);
              واپس true;
            }

          saved_ریکارڈ_سائز = past_انتہا - G_بفر;
          اگر (saved_ریکارڈ_سائز > read_سائز)
            {
              /* 'G_buffer_size' is about twice 'read_size', so since
                 we want to read in another 'read_size' bytes before
                 the data already in 'G_buffer', we need to increase
                 'G_buffer_size'. */
              حرف *newbuffer;
              size_t انحراف = sentinel_لمبائی ? sentinel_لمبائی : 1;
              size_t old_G_بفر_سائز = G_بفر_سائز;

              read_سائز *= 2;
              G_بفر_سائز = read_سائز * 2 + sentinel_لمبائی + 2;
              اگر (G_بفر_سائز < old_G_بفر_سائز)
                xalloc_die ();
              newbuffer = xrealloc (G_بفر - انحراف, G_بفر_سائز);
              newbuffer += انحراف;
              G_بفر = newbuffer;
            }

          /* Back up to the start of the next bufferfull of the file.  */
          اگر (file_pos >= read_سائز)
            file_pos -= read_سائز;
          ورنہ
            {
              read_سائز = file_pos;
              file_pos = 0;
            }
          اگر (lseek (داخلہ_fd, file_pos, SEEK_SET) < 0)
            error (0, errno, _("%s: seek failed"), quotef (file));

          /* Shift the pending record data right to make room for the new.
             The source and destination regions probably overlap.  */
          memmove (G_بفر + read_سائز, G_بفر, saved_ریکارڈ_سائز);
          past_انتہا = G_بفر + read_سائز + saved_ریکارڈ_سائز;
          /* For non-regexp searches, avoid unnecessary scanning. */
          اگر (sentinel_لمبائی)
            مطابقت_آغاز = G_بفر + read_سائز;
          ورنہ
            مطابقت_آغاز = past_انتہا;

          اگر (بھرا_read (داخلہ_fd, G_بفر, read_سائز) != read_سائز)
            {
              error (0, errno, _("%s: read error"), quotef (file));
              واپس false;
            }
        }
      ورنہ
        {
          /* Found a match of 'separator'. */
          اگر (separator_سرے_ریکارڈ)
            {
              حرف *مطابقت_انتہا = مطابقت_آغاز + مطابقت_لمبائی;

              /* If this match of 'separator' isn't at the end of the
                 file, print the record. */
              اگر (!پہلا_وقت || مطابقت_انتہا != past_انتہا)
                output (مطابقت_انتہا, past_انتہا);
              past_انتہا = مطابقت_انتہا;
              پہلا_وقت = false;
            }
          ورنہ
            {
              output (مطابقت_آغاز, past_انتہا);
              past_انتہا = مطابقت_آغاز;
            }

          /* For non-regex matching, we can back up.  */
          اگر (sentinel_لمبائی > 0)
            مطابقت_آغاز -= مطابقت_لمبائی - 1;
        }
    }
}

/* Copy from file descriptor INPUT_FD (corresponding to the named FILE) to
   a temporary file, and set *G_TMP and *G_TEMPFILE to the resulting stream
   and file name.  Return the number of bytes copied, or -1 on error.  */

ساکن off_t
copy_مقصد_عارضی (FILE **g_tmp, حرف **g_tempfile, صحیح داخلہ_fd, حرف ثابت *file)
{
  FILE *fp;
  حرف *file_name;
  uintmax_t بائٹیں_copied = 0;
  اگر (!عارضی_دھارا (&fp, &file_name))
    واپس -1;

  جب (true)
    {
      ssize_t بائٹیں_read = اقرأ (داخلہ_fd, G_بفر, read_سائز);
      اگر (بائٹیں_read == 0)
        توڑو;
      اگر (بائٹیں_read < 0)
        {
          error (0, errno, _("%s: read error"), quotef (file));
          واپس -1;
        }

      اگر (fwrite (G_بفر, 1, بائٹیں_read, fp) != بائٹیں_read)
        {
          error (0, errno, _("%s: write error"), quotef (file_name));
          واپس -1;
        }

      /* Implicitly <= OFF_T_MAX due to preceding fwrite(),
         but unsigned type used to avoid compiler warnings
         not aware of this fact.  */
      بائٹیں_copied += بائٹیں_read;
    }

  اگر (fflush (fp) != 0)
    {
      error (0, errno, _("%s: write error"), quotef (file_name));
      واپس -1;
    }

  *g_tmp = fp;
  *g_tempfile = file_name;
  واپس بائٹیں_copied;
}

/* Copy INPUT_FD to a temporary, then tac that file.
   Return true if successful.  */

ساکن bool
tac_nonseekable (صحیح داخلہ_fd, حرف ثابت *file)
{
  FILE *tmp_دھارا;
  حرف *tmp_file;
  off_t بائٹیں_copied = copy_مقصد_عارضی (&tmp_دھارا, &tmp_file, داخلہ_fd, file);
  اگر (بائٹیں_copied < 0)
    واپس false;

  bool ok = tac_seekable (fileno (tmp_دھارا), tmp_file, بائٹیں_copied);
  واپس ok;
}

/* Print FILE in reverse, copying it to a temporary
   file first if it is not seekable.
   Return true if successful.  */

ساکن bool
tac_file (حرف ثابت *filename)
{
  bool ok;
  off_t file_سائز;
  صحیح fd;
  bool is_stdin = streq (filename, "-");

  اگر (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      filename = _("standard input");
      xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
    }
  ورنہ
    {
      fd = open (filename, O_RDONLY | O_ثنائی);
      اگر (fd < 0)
        {
          error (0, errno, _("failed to open %s for reading"),
                 quoteaf (filename));
          واپس false;
        }
    }

  file_سائز = lseek (fd, 0, SEEK_END);

  ok = (file_سائز < 0 || isatty (fd)
        ? tac_nonseekable (fd, filename)
        : tac_seekable (fd, filename, file_سائز));

  اگر (!is_stdin && close (fd) != 0)
    {
      error (0, errno, _("%s: read error"), quotef (filename));
      ok = false;
    }
  واپس ok;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف ثابت *error_پیغام_2;	/* Return value from re_compile_pattern. */
  صحیح optc;
  bool ok;
  size_t half_بفر_سائز;

  /* Initializer for file_list if no file-arguments
     were specified on the command line.  */
  ساکن حرف ثابت *ثابت default_file_list[] = {"-", تہی};
  حرف ثابت *ثابت *file;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "brs:", longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'b':
          separator_سرے_ریکارڈ = false;
          توڑو;
        حالت 'r':
          sentinel_لمبائی = 0;
          توڑو;
        حالت 's':
          separator = optarg;
          توڑو;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (sentinel_لمبائی == 0)
    {
      اگر (*separator == 0)
        error (EXIT_FAILURE, 0, _("separator cannot be empty"));

      compiled_separator.بفر = تہی;
      compiled_separator.allocated = 0;
      compiled_separator.fastmap = compiled_separator_fastmap;
      compiled_separator.translate = تہی;
      error_پیغام_2 = re_compile_نمونہ (separator, لڑی_لمبائی (separator),
                                          &compiled_separator);
      اگر (error_پیغام_2)
        error (EXIT_FAILURE, 0, "%s", (error_پیغام_2));
    }
  ورنہ
    مطابقت_لمبائی = sentinel_لمبائی = *separator ? لڑی_لمبائی (separator) : 1;

  read_سائز = INITIAL_READSIZE;
  جب (sentinel_لمبائی >= read_سائز / 2)
    {
      اگر (SIZE_MAX / 2 < read_سائز)
        xalloc_die ();
      read_سائز *= 2;
    }
  half_بفر_سائز = read_سائز + sentinel_لمبائی + 1;
  G_بفر_سائز = 2 * half_بفر_سائز;
  اگر (! (read_سائز < half_بفر_سائز && half_بفر_سائز < G_بفر_سائز))
    xalloc_die ();
  G_بفر = xmalloc (G_بفر_سائز);
  اگر (sentinel_لمبائی)
    {
      حافظہ_نقل (G_بفر, separator, sentinel_لمبائی + !!*separator);
      G_بفر += sentinel_لمبائی;
    }
  ورنہ
    {
      ++G_بفر;
    }

  file = (optind < argc
          ? (حرف ثابت *ثابت *) &argv[optind]
          : default_file_list);

  xset_ثنائی_منوال (STDOUT_FILENO, O_ثنائی);

  {
    ok = true;
    برائے (size_t i = 0; file[i]; ++i)
      ok &= tac_file (file[i]);
  }

  /* Flush the output buffer. */
  output ((حرف *) تہی, (حرف *) تہی);

  اگر (have_read_stdin && close (STDIN_FILENO) < 0)
    {
      error (0, errno, "-");
      ok = false;
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
