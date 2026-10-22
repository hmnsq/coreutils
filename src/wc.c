/* wc - print the number of lines, words, and bytes in files
   Copyright (C) 1985-2026 Free Software Foundation, Inc.

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

/* Written by Paul Rubin, phr@ocf.berkeley.edu
   and David MacKenzie, djm@gnu.ai.mit.edu. */

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل <argmatch.h>
#شامل <argv-iter.h>
#شامل <fadvise.h>
#شامل <physmem.h>
#شامل <readtokens0.h>
#شامل <stat-size.h>
#شامل <xbinary-io.h>

#ifdef USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#endif

#شامل "system.h"
#شامل "assure.h"
#شامل "cpu-supports.h"
#شامل "ioblksize.h"
#شامل "wc.h"
#شامل "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "wc"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

ایستا bool wc_isprint[UCHAR_MAX + 1];
ایستا bool wc_isspace[UCHAR_MAX + 1];

ایستا bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
ایستا uintmax_t total_سطرها;
ایستا uintmax_t total_واژگان;
ایستا uintmax_t total_chars;
ایستا uintmax_t total_bytes;
ایستا bool total_سطرها_overflow;
ایستا bool total_واژگان_overflow;
ایستا bool total_chars_overflow;
ایستا bool total_bytes_overflow;
ایستا intmax_t max_سطر_length;

/* Which counts to print. */
ایستا bool print_سطرها, print_واژگان, print_chars, print_bytes;
ایستا bool print_linelength;

/* The print width of each count.  */
ایستا صحیح عدد_width;

/* True if we have ever read the standard input. */
ایستا bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
ایستا idx_t page_size;

/* Enable to _not_ treat non breaking space as a word separator.  */
ایستا bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
ساختار fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  صحیح failed;

  /* If FAILED is zero, this is the file's status.  */
  ساختار stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  DEBUG_PROGRAM_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  TOTAL_OPTION,
};

ایستا ساختار option ثابت longopts[] =
{
  {"bytes", no_argument, پوچ, 'c'},
  {"chars", no_argument, پوچ, 'm'},
  {"lines", no_argument, پوچ, 'l'},
  {"words", no_argument, پوچ, 'w'},
  {"debug", no_argument, پوچ, DEBUG_PROGRAM_OPTION},
  {"files0-from", required_argument, پوچ, FILES0_FROM_OPTION},
  {"max-line-length", no_argument, پوچ, 'L'},
  {"total", required_argument, پوچ, TOTAL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

شمارش total_type
  {
    total_auto,         /* 0: default or --total=auto */
    total_always,       /* 1: --total=always */
    total_only,         /* 2: --total=only */
    total_never         /* 3: --total=never */
  };
ایستا نویسه ثابت *ثابت total_args[] =
{
  "auto", "always", "only", "never", پوچ
};
ایستا شمارش total_type ثابت total_types[] =
{
  total_auto, total_always, total_only, total_never
};
ARGMATCH_VERIFY (total_args, total_types);
ایستا شمارش total_type total_mode = total_auto;

#ifdef USE_AVX2_WC_LINECOUNT
ایستا bool
avx2_supported (تهی)
{
  bool avx2_enabled = cpu_supports ("avx2");
  اگر (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  بازگشت avx2_enabled;
}
#endif

#ifdef USE_AVX512_WC_LINECOUNT
ایستا bool
avx512_supported (تهی)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  اگر (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  بازگشت avx512_enabled;
}
#endif

#ifdef USE_NEON_WC_LINECOUNT
ایستا bool
neon_supported (تهی)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  اگر (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  بازگشت neon_enabled;
}
#endif

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      fputs (_("\
Print newline, word, and byte counts برای each FILE, and a total سطر اگر\n\
more than one FILE is specified.  A word is a nonempty sequence of non white\n\
space delimited by white space characters or by start or end of درون_داد.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum سطر length.\n\
"), stdout);
      oputs (_("\
  -c, --bytes\n\
         print the byte counts\n\
"));
      oputs (_("\
  -m, --chars\n\
         print the character counts\n\
"));
      oputs (_("\
  -l, --سطرها\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what سطر شمار acceleration is used\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ درون_داد from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, اقرأ names from standard درون_داد\n\
"));
      oputs (_("\
  -L, --max-سطر-length\n\
         print the maximum display width\n\
"));
      oputs (_("\
  -w, --واژگان\n\
         print the word counts\n\
"));
      oputs (_("\
      --total=WHEN\n\
         when to print a سطر with total counts;\n\
         WHEN can be: auto, always, only, never\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Return non zero if POSIXLY_CORRECT is not set and WC is a non breaking
   space.  */
ATTRIBUTE_PURE
ایستا صحیح
maybe_c32isnbspace (char32_t wc)
{
  بازگشت ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
ایستا تهی
write_counts (uintmax_t سطرها,
              uintmax_t واژگان,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              نویسه ثابت *file)
{
  ایستا نویسه ثابت format_sp_int[] = " %*s";
  نویسه ثابت *format_int = format_sp_int + 1;
  نویسه buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  اگر (print_سطرها)
    {
      printf (format_int, عدد_width, umaxtostr (سطرها, buf));
      format_int = format_sp_int;
    }
  اگر (print_واژگان)
    {
      printf (format_int, عدد_width, umaxtostr (واژگان, buf));
      format_int = format_sp_int;
    }
  اگر (print_chars)
    {
      printf (format_int, عدد_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  اگر (print_bytes)
    {
      printf (format_int, عدد_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  اگر (print_linelength)
    printf (format_int, عدد_width, imaxtostr (linelength, buf));
  اگر (file)
    printf (" %s", strchr (file, '\n') ? quotef (file) : file);
  putchar ('\n');

  اگر (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
ایستا ساختار wc_سطرها
wc_سطرها (صحیح fd)
{
#ifdef USE_AVX512_WC_LINECOUNT
  ایستا signed نویسه use_avx512;
  اگر (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  اگر (0 < use_avx512)
    بازگشت wc_سطرها_avx512 (fd);
#endif
#ifdef USE_AVX2_WC_LINECOUNT
  ایستا signed نویسه use_avx2;
  اگر (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  اگر (0 < use_avx2)
    بازگشت wc_سطرها_avx2 (fd);
#endif
#ifdef USE_NEON_WC_LINECOUNT
  ایستا signed نویسه use_neon;
  اگر (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  اگر (0 < use_neon)
    بازگشت wc_سطرها_neon (fd);
#endif

  intmax_t سطرها = 0, bytes = 0;
  bool long_سطرها = false;

  وقتی (true)
    {
      نویسه buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = اقرأ (fd, buf, IO_BUFSIZE);
      اگر (bytes_read <= 0)
        بازگشت (ساختار wc_سطرها) { bytes_read == 0 ? 0 : errno, سطرها, bytes };

      bytes += bytes_read;
      نویسه *end = buf + bytes_read;
      idx_t buflines = 0;

      اگر (! long_سطرها)
        {
          /* Avoid function call overhead for shorter lines.  */
          برای (نویسه *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      وگرنه
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          برای (نویسه *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_سطرها = 15 * buflines <= bytes_read;
      سطرها += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
ایستا bool
wc (صحیح fd, نویسه ثابت *file_x, ساختار fstatus *fstatus)
{
  صحیح err = 0;
  نویسه buf[IO_BUFSIZE + 1];
  intmax_t سطرها, واژگان, chars, bytes, linelength;
  bool شمار_bytes, شمار_chars, شمار_complicated;
  نویسه ثابت *file = file_x ? file_x : _("standard input");

  سطرها = واژگان = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  اگر (MB_CUR_MAX > 1)
    {
      شمار_bytes = print_bytes;
      شمار_chars = print_chars;
    }
  وگرنه
    {
      شمار_bytes = print_bytes || print_chars;
      شمار_chars = false;
    }
  شمار_complicated = print_واژگان || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  اگر (!شمار_bytes || شمار_chars || print_سطرها || شمار_complicated)
    fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);

  /* When counting only bytes, save some line- and word-counting
     overhead.  If FD is a 'regular' Unix file, using lseek is enough
     to get its 'size' in bytes.  Otherwise, read blocks of IO_BUFSIZE
     bytes at a time until EOF.  Note that the 'size' (number of bytes)
     that wc reports is smaller than stats.st_size when the file is not
     positioned at its beginning.  That's why the lseek calls below are
     necessary.  For example the command
     '(dd ibs=99k skip=1 count=0; ./wc -c) < /etc/group'
     should make wc report '0' bytes.  */

  اگر (شمار_bytes && !شمار_chars && !print_سطرها && !شمار_complicated)
    {
      bool skip_read = false;

      اگر (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      اگر (! fstatus->failed && usable_st_size (&fstatus->st)
          && 0 <= fstatus->st.st_size)
        {
          off_t end_pos = fstatus->st.st_size;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          اگر (current_pos < 0)
            ;
          وگرنه اگر (end_pos % page_size)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              اگر (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          وگرنه اگر (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              اگر (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          اگر (0 < bytes && lseek (fd, bytes, SEEK_CUR) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      اگر (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          برای (ssize_t bytes_read;
               (bytes_read = اقرأ (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            اگر (bytes_read < 0)
              {
                err = errno;
                بشکن;
              }
        }
    }
  وگرنه اگر (!شمار_chars && !شمار_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      ساختار wc_سطرها w = wc_سطرها (fd);
      err = w.err;
      سطرها = w.سطرها;
      bytes = w.bytes;
    }
  وگرنه اگر (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool شمار_utf8 = !شمار_complicated && !print_سطرها
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      برای (ssize_t bytes_read;
           ((bytes_read = اقرأ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          اگر (bytes_read < 0)
            {
              err = errno;
              بشکن;
            }

          bytes += bytes_read;
          نویسه ثابت *p = buf;
          نویسه ثابت *plim = p + prev + bytes_read;
          bool try_شمار_utf8 = شمار_utf8;
          انجامبده
            {
              اگر (try_شمار_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_شمار_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_شمار (p, &n);
                  p += n;
                  اگر (p == plim)
                    بشکن;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              اگر (!in_shift && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              وگرنه
                {
                  idx_t prev_bytes = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  اگر (scanbytes < n)
                    {
                      اگر (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          in_shift = true;
                          بشکن;
                        }

                      /* Remember that we read a byte, but don't complain
                         about the error.  Because of the decoding error,
                         this is a considered to be byte but not a
                         character (that is, chars is not incremented).  */
                      p++;
                      mbszero (&state);
                      in_shift = false;

                      /* Treat encoding errors as non white space.
                         POSIX says a word is "a non-zero-length string of
                         characters delimited by white space".  This is
                         wrong in some sense, as the string can be delimited
                         by start or end of input, and it is unclear what it
                         means when the input contains encoding errors.
                         Since encoding errors are not white space,
                         treat them that way here.  */
                      واژگان += !in_word;
                      in_word = true;
                      ادامه;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              انتخاب (wide_char)
                {
                حالت '\n':
                  سطرها++;
                  FALLTHROUGH;
                حالت '\r':
                حالت '\f':
                  اگر (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  بشکن;

                حالت '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  بشکن;

                حالت ' ':
                  linepos++;
                  FALLTHROUGH;
                حالت '\v':
                  in_word = false;
                  بشکن;

                پیشفرض:;
                  bool in_word2;
                  اگر (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  وگرنه
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      اگر (print_linelength)
                        {
                          صحیح width = c32width (wide_char);
                          اگر (width > 0)
                            linepos += width;
                        }
                      in_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  واژگان += !in_word & in_word2;
                  in_word = in_word2;
                  بشکن;
                }

              p += charbytes;
              chars++;
            }
          وقتی (p < plim);
        }
      اگر (linepos > linelength)
        linelength = linepos;
    }
  وگرنه
    {
      bool in_word = false;
      intmax_t linepos = 0;

      برای (ssize_t bytes_read; (bytes_read = اقرأ (fd, buf, IO_BUFSIZE)); )
        {
          اگر (bytes_read < 0)
            {
              err = errno;
              بشکن;
            }

          bytes += bytes_read;
          نویسه ثابت *p = buf;
          انجامبده
            {
              unsigned نویسه c = *p++;
              انتخاب (c)
                {
                حالت '\n':
                  سطرها++;
                  FALLTHROUGH;
                حالت '\r':
                حالت '\f':
                  اگر (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  بشکن;

                حالت '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  بشکن;

                حالت ' ':
                  linepos++;
                  FALLTHROUGH;
                حالت '\v':
                  in_word = false;
                  بشکن;

                پیشفرض:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  واژگان += !in_word & in_word2;
                  in_word = in_word2;
                  بشکن;
                }
            }
          وقتی (--bytes_read);
        }
      اگر (linepos > linelength)
        linelength = linepos;
    }

  اگر (شمار_chars < print_chars)
    chars = bytes;

  اگر (total_mode != total_only)
    write_counts (سطرها, واژگان, chars, bytes, linelength, file_x);

  total_سطرها_overflow |= ckd_add (&total_سطرها, total_سطرها, سطرها);
  total_واژگان_overflow |= ckd_add (&total_واژگان, total_واژگان, واژگان);
  total_chars_overflow |= ckd_add (&total_chars, total_chars, chars);
  total_bytes_overflow |= ckd_add (&total_bytes, total_bytes, bytes);

  اگر (linelength > max_سطر_length)
    max_سطر_length = linelength;

  اگر (err)
    error (0, err, "%s", quotef (file));
  بازگشت !err;
}

ایستا bool
wc_file (نویسه ثابت *file, ساختار fstatus *fstatus)
{
  اگر (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_دودویی_mode (STDIN_FILENO, O_دودویی);
      بازگشت wc (STDIN_FILENO, file, fstatus);
    }
  وگرنه
    {
      صحیح fd = open (file, O_RDONLY | O_دودویی);
      اگر (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          بازگشت false;
        }
      وگرنه
        {
          bool ok = wc (fd, file, fstatus);
          اگر (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              بازگشت false;
            }
          بازگشت ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

ایستا ساختار fstatus *
get_درون_داد_fstatus (idx_t nfiles, نویسه *ثابت *file)
{
  ساختار fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, اندازه *fstatus);

  اگر (nfiles == 0
      || (nfiles == 1
          && ((print_سطرها + print_واژگان + print_chars
               + print_bytes + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  وگرنه
    {
      برای (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  بازگشت fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_PURE
ایستا صحیح
compute_عدد_width (idx_t nfiles, ساختار fstatus ثابت *fstatus)
{
  صحیح width = 1;

  اگر (0 < nfiles && fstatus[0].failed <= 0)
    {
      صحیح minimum_width = 1;
      uintmax_t regular_total = 0;

      برای (idx_t i = 0; i < nfiles; i++)
        اگر (! fstatus[i].failed)
          {
            اگر (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            وگرنه اگر (ckd_add (&regular_total, regular_total,
                              fstatus[i].st.st_size))
              {
                regular_total = UINTMAX_MAX;
                بشکن;
              }
          }

      برای (; 10 <= regular_total; regular_total /= 10)
        width++;
      اگر (width < minimum_width)
        width = minimum_width;
    }

  بازگشت width;
}


صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح optc;
  idx_t nfiles;
  نویسه **files;
  نویسه *files_from = پوچ;
  ساختار fstatus *fstatus;
  ساختار Tokens tok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  page_size = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, پوچ, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != پوچ);

  وقتی ((optc = getopt_long (argc, argv, "clLmw", longopts, پوچ)) != -1)
    انتخاب (optc)
      {
      حالت 'c':
        print_bytes = true;
        بشکن;

      حالت 'm':
        print_chars = true;
        بشکن;

      حالت 'l':
        print_سطرها = true;
        بشکن;

      حالت 'w':
        print_واژگان = true;
        بشکن;

      حالت 'L':
        print_linelength = true;
        بشکن;

      حالت DEBUG_PROGRAM_OPTION:
        debug = true;
        بشکن;

      حالت FILES0_FROM_OPTION:
        files_from = optarg;
        بشکن;

      حالت TOTAL_OPTION:
        total_mode = XARGMATCH ("--total", optarg, total_args, total_types);
        بشکن;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      پیشفرض:
        usage (EXIT_FAILURE);
      }

  اگر (! (print_سطرها || print_واژگان || print_chars || print_bytes
         || print_linelength))
    print_سطرها = print_واژگان = print_bytes = true;

  اگر (print_linelength)
    برای (صحیح i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  اگر (print_واژگان)
    برای (صحیح i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = isspace (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  ساختار argv_iterator *ai;
  اگر (files_from)
    {
      FILE *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      اگر (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      اگر (streq (files_from, "-"))
        stream = stdin;
      وگرنه
        {
          stream = fopen (files_from, "r");
          اگر (stream == پوچ)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      ساختار stat st;
      اگر (fstat (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_size <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          اگر (! readtokens0 (stream, &tok) || fclose (stream) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      وگرنه
        {
          files = پوچ;
          nfiles = 0;
          ai = argv_iter_init_stream (stream);
        }
    }
  وگرنه
    {
      ایستا نویسه *stdin_only[] = { پوچ };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  اگر (!ai)
    xalloc_die ();

  fstatus = get_درون_داد_fstatus (nfiles, files);
  اگر (total_mode == total_only)
    عدد_width = 1;  /* No extra padding, since no alignment requirement.  */
  وگرنه
    عدد_width = compute_عدد_width (nfiles, fstatus);

  bool ok = true;
  شمارش argv_iter_err ai_err;
  نویسه *file_name;
  برای (صحیح i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      اگر (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      اگر (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          اگر (files_from == پوچ)
            error (0, 0, "%s", _("invalid zero-length file name"));
          وگرنه
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_args (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      اگر (skip_file)
        ok = false;
      وگرنه
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      اگر (! nfiles)
        fstatus[0].failed = 1;
    }
  انتخاب (ai_err)
    {
    حالت AI_ERR_EOF:
      بشکن;

    حالت AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      بشکن;

    حالت AI_ERR_MEM:
      xalloc_die ();

    حالت AI_ERR_OK: پیشفرض:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  اگر (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (پوچ, &fstatus[0]);

  اگر (read_tokens)
    readtokens0_free (&tok);

  اگر (total_mode != total_never
      && (total_mode != total_auto || 1 < argv_iter_n_args (ai)))
    {
      اگر (total_سطرها_overflow)
        {
          total_سطرها = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      اگر (total_واژگان_overflow)
        {
          total_واژگان = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      اگر (total_chars_overflow)
        {
          total_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      اگر (total_bytes_overflow)
        {
          total_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (total_سطرها, total_واژگان, total_chars, total_bytes,
                    max_سطر_length,
                    total_mode != total_only ? _("total") : پوچ);
    }

  argv_iter_free (ai);

  آزاد (fstatus);

  اگر (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
