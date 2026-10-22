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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব <argmatch.h>
#অন্তর্ভাব <argv-iter.h>
#অন্তর্ভাব <fadvise.h>
#অন্তর্ভাব <physmem.h>
#অন্তর্ভাব <readtokens0.h>
#অন্তর্ভাব <stat-size.h>
#অন্তর্ভাব <xbinary-io.h>

#ifdef USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#endif

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "cpu-supports.h"
#অন্তর্ভাব "ioblksize.h"
#অন্তর্ভাব "wc.h"
#অন্তর্ভাব "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "wc"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

স্থির bool wc_isprint[UCHAR_MAX + 1];
স্থির bool wc_isspace[UCHAR_MAX + 1];

স্থির bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
স্থির uintmax_t যোগ_সারিগুলি;
স্থির uintmax_t যোগ_শব্দগুলি;
স্থির uintmax_t যোগ_chars;
স্থির uintmax_t যোগ_bytes;
স্থির bool যোগ_সারিগুলি_overflow;
স্থির bool যোগ_শব্দগুলি_overflow;
স্থির bool যোগ_chars_overflow;
স্থির bool যোগ_bytes_overflow;
স্থির intmax_t max_সারি_দৈর্ঘ্য;

/* Which counts to print. */
স্থির bool print_সারিগুলি, print_শব্দগুলি, print_chars, print_bytes;
স্থির bool print_linelength;

/* The print width of each count.  */
স্থির পূর্ণসংখ্যা সংখ্যা_width;

/* True if we have ever read the standard input. */
স্থির bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
স্থির idx_t page_আকার;

/* Enable to _not_ treat non breaking space as a word separator.  */
স্থির bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
কাঠামো fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  পূর্ণসংখ্যা failed;

  /* If FAILED is zero, this is the file's status.  */
  কাঠামো stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  DEBUG_PROGRAM_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  যোগ_OPTION,
};

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"bytes", no_argument, ফাঁকা, 'c'},
  {"chars", no_argument, ফাঁকা, 'm'},
  {"lines", no_argument, ফাঁকা, 'l'},
  {"words", no_argument, ফাঁকা, 'w'},
  {"debug", no_argument, ফাঁকা, DEBUG_PROGRAM_OPTION},
  {"files0-from", required_argument, ফাঁকা, FILES0_FROM_OPTION},
  {"max-line-length", no_argument, ফাঁকা, 'L'},
  {"total", required_argument, ফাঁকা, যোগ_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

গণনা যোগ_type
  {
    যোগ_auto,         /* 0: default or --total=auto */
    যোগ_always,       /* 1: --total=always */
    যোগ_only,         /* 2: --total=only */
    যোগ_never         /* 3: --total=never */
  };
স্থির অক্ষর ধ্রুবক *ধ্রুবক যোগ_args[] =
{
  "auto", "always", "only", "never", ফাঁকা
};
স্থির গণনা যোগ_type ধ্রুবক যোগ_types[] =
{
  যোগ_auto, যোগ_always, যোগ_only, যোগ_never
};
ARGMATCH_VERIFY (যোগ_args, যোগ_types);
স্থির গণনা যোগ_type যোগ_mode = যোগ_auto;

#ifdef USE_AVX2_WC_LINECOUNT
স্থির bool
avx2_supported (শূন্য)
{
  bool avx2_enabled = cpu_supports ("avx2");
  যদি (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  ফেরত avx2_enabled;
}
#endif

#ifdef USE_AVX512_WC_LINECOUNT
স্থির bool
avx512_supported (শূন্য)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  যদি (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  ফেরত avx512_enabled;
}
#endif

#ifdef USE_NEON_WC_LINECOUNT
স্থির bool
neon_supported (শূন্য)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  যদি (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  ফেরত neon_enabled;
}
#endif

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      fputs (_("\
Print newline, word, and byte counts জন্য each FILE, and a যোগ_2 সারি যদি\n\
more than one FILE is specified.  A word is a nonempty sequence of non white\n\
space delimited by white space characters or by start or end of প্রবেশ.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum সারি দৈর্ঘ্য.\n\
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
  -l, --সারিগুলি\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what সারি গুনতি acceleration is used\n\
"));
      oputs (_("\
      --files0-from=F\n\
         পঠ প্রবেশ from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, পঠ names from standard প্রবেশ\n\
"));
      oputs (_("\
  -L, --max-সারি-দৈর্ঘ্য\n\
         print the maximum display width\n\
"));
      oputs (_("\
  -w, --শব্দগুলি\n\
         print the word counts\n\
"));
      oputs (_("\
      --যোগ_2=WHEN\n\
         when to print a সারি with যোগ_2 counts;\n\
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
স্থির পূর্ণসংখ্যা
maybe_c32isnbspace (char32_t wc)
{
  ফেরত ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
স্থির শূন্য
write_counts (uintmax_t সারিগুলি,
              uintmax_t শব্দগুলি,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              অক্ষর ধ্রুবক *file)
{
  স্থির অক্ষর ধ্রুবক format_sp_int[] = " %*s";
  অক্ষর ধ্রুবক *format_int = format_sp_int + 1;
  অক্ষর buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  যদি (print_সারিগুলি)
    {
      printf (format_int, সংখ্যা_width, umaxtostr (সারিগুলি, buf));
      format_int = format_sp_int;
    }
  যদি (print_শব্দগুলি)
    {
      printf (format_int, সংখ্যা_width, umaxtostr (শব্দগুলি, buf));
      format_int = format_sp_int;
    }
  যদি (print_chars)
    {
      printf (format_int, সংখ্যা_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  যদি (print_bytes)
    {
      printf (format_int, সংখ্যা_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  যদি (print_linelength)
    printf (format_int, সংখ্যা_width, imaxtostr (linelength, buf));
  যদি (file)
    printf (" %s", strchr (file, '\n') ? quotef (file) : file);
  putchar ('\n');

  যদি (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
স্থির কাঠামো wc_সারিগুলি
wc_সারিগুলি (পূর্ণসংখ্যা fd)
{
#ifdef USE_AVX512_WC_LINECOUNT
  স্থির signed অক্ষর use_avx512;
  যদি (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  যদি (0 < use_avx512)
    ফেরত wc_সারিগুলি_avx512 (fd);
#endif
#ifdef USE_AVX2_WC_LINECOUNT
  স্থির signed অক্ষর use_avx2;
  যদি (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  যদি (0 < use_avx2)
    ফেরত wc_সারিগুলি_avx2 (fd);
#endif
#ifdef USE_NEON_WC_LINECOUNT
  স্থির signed অক্ষর use_neon;
  যদি (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  যদি (0 < use_neon)
    ফেরত wc_সারিগুলি_neon (fd);
#endif

  intmax_t সারিগুলি = 0, bytes = 0;
  bool long_সারিগুলি = false;

  যখন (true)
    {
      অক্ষর buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = পঠ (fd, buf, IO_BUFSIZE);
      যদি (bytes_read <= 0)
        ফেরত (কাঠামো wc_সারিগুলি) { bytes_read == 0 ? 0 : errno, সারিগুলি, bytes };

      bytes += bytes_read;
      অক্ষর *end = buf + bytes_read;
      idx_t buflines = 0;

      যদি (! long_সারিগুলি)
        {
          /* Avoid function call overhead for shorter lines.  */
          জন্য (অক্ষর *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      অন্যথা
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          জন্য (অক্ষর *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_সারিগুলি = 15 * buflines <= bytes_read;
      সারিগুলি += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
স্থির bool
wc (পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *file_x, কাঠামো fstatus *fstatus)
{
  পূর্ণসংখ্যা err = 0;
  অক্ষর buf[IO_BUFSIZE + 1];
  intmax_t সারিগুলি, শব্দগুলি, chars, bytes, linelength;
  bool গুনতি_bytes, গুনতি_chars, গুনতি_complicated;
  অক্ষর ধ্রুবক *file = file_x ? file_x : _("standard input");

  সারিগুলি = শব্দগুলি = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  যদি (MB_CUR_MAX > 1)
    {
      গুনতি_bytes = print_bytes;
      গুনতি_chars = print_chars;
    }
  অন্যথা
    {
      গুনতি_bytes = print_bytes || print_chars;
      গুনতি_chars = false;
    }
  গুনতি_complicated = print_শব্দগুলি || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  যদি (!গুনতি_bytes || গুনতি_chars || print_সারিগুলি || গুনতি_complicated)
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

  যদি (গুনতি_bytes && !গুনতি_chars && !print_সারিগুলি && !গুনতি_complicated)
    {
      bool skip_read = false;

      যদি (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      যদি (! fstatus->failed && usable_st_আকার (&fstatus->st)
          && 0 <= fstatus->st.st_আকার)
        {
          off_t end_pos = fstatus->st.st_আকার;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          যদি (current_pos < 0)
            ;
          অন্যথা যদি (end_pos % page_আকার)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              যদি (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          অন্যথা যদি (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              যদি (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          যদি (0 < bytes && lseek (fd, bytes, SEEK_CUR) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      যদি (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          জন্য (ssize_t bytes_read;
               (bytes_read = পঠ (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            যদি (bytes_read < 0)
              {
                err = errno;
                বিরতি;
              }
        }
    }
  অন্যথা যদি (!গুনতি_chars && !গুনতি_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      কাঠামো wc_সারিগুলি w = wc_সারিগুলি (fd);
      err = w.err;
      সারিগুলি = w.সারিগুলি;
      bytes = w.bytes;
    }
  অন্যথা যদি (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool গুনতি_utf8 = !গুনতি_complicated && !print_সারিগুলি
                        && is_utf8_charset ();
      idx_t পূর্ব = 0; /* Number of bytes carried over from previous round.  */

      জন্য (ssize_t bytes_read;
           ((bytes_read = পঠ (fd, buf + পূর্ব, IO_BUFSIZE - পূর্ব))
            || পূর্ব);
           )
        {
          যদি (bytes_read < 0)
            {
              err = errno;
              বিরতি;
            }

          bytes += bytes_read;
          অক্ষর ধ্রুবক *p = buf;
          অক্ষর ধ্রুবক *plim = p + পূর্ব + bytes_read;
          bool try_গুনতি_utf8 = গুনতি_utf8;
          করো
            {
              যদি (try_গুনতি_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_গুনতি_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_গুনতি (p, &n);
                  p += n;
                  যদি (p == plim)
                    বিরতি;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              যদি (!in_shift && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              অন্যথা
                {
                  idx_t পূর্ব_bytes = পূর্ব;
                  idx_t scanbytes = plim - (p + পূর্ব);
                  size_t n = mbrtoc32 (&wide_char, p + পূর্ব, scanbytes, &state);
                  পূর্ব = 0;

                  যদি (scanbytes < n)
                    {
                      যদি (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          পূর্ব = plim - p;
                          memmove (buf, p, পূর্ব);
                          in_shift = true;
                          বিরতি;
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
                      শব্দগুলি += !in_word;
                      in_word = true;
                      চালু;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = পূর্ব_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              নির্বাচন (wide_char)
                {
                ক্ষেত্র '\n':
                  সারিগুলি++;
                  FALLTHROUGH;
                ক্ষেত্র '\r':
                ক্ষেত্র '\f':
                  যদি (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  বিরতি;

                ক্ষেত্র '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  বিরতি;

                ক্ষেত্র ' ':
                  linepos++;
                  FALLTHROUGH;
                ক্ষেত্র '\v':
                  in_word = false;
                  বিরতি;

                পূর্বনির্ধারিত:;
                  bool in_word2;
                  যদি (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  অন্যথা
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      যদি (print_linelength)
                        {
                          পূর্ণসংখ্যা width = c32width (wide_char);
                          যদি (width > 0)
                            linepos += width;
                        }
                      in_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  শব্দগুলি += !in_word & in_word2;
                  in_word = in_word2;
                  বিরতি;
                }

              p += charbytes;
              chars++;
            }
          যখন (p < plim);
        }
      যদি (linepos > linelength)
        linelength = linepos;
    }
  অন্যথা
    {
      bool in_word = false;
      intmax_t linepos = 0;

      জন্য (ssize_t bytes_read; (bytes_read = পঠ (fd, buf, IO_BUFSIZE)); )
        {
          যদি (bytes_read < 0)
            {
              err = errno;
              বিরতি;
            }

          bytes += bytes_read;
          অক্ষর ধ্রুবক *p = buf;
          করো
            {
              unsigned অক্ষর c = *p++;
              নির্বাচন (c)
                {
                ক্ষেত্র '\n':
                  সারিগুলি++;
                  FALLTHROUGH;
                ক্ষেত্র '\r':
                ক্ষেত্র '\f':
                  যদি (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  বিরতি;

                ক্ষেত্র '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  বিরতি;

                ক্ষেত্র ' ':
                  linepos++;
                  FALLTHROUGH;
                ক্ষেত্র '\v':
                  in_word = false;
                  বিরতি;

                পূর্বনির্ধারিত:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  শব্দগুলি += !in_word & in_word2;
                  in_word = in_word2;
                  বিরতি;
                }
            }
          যখন (--bytes_read);
        }
      যদি (linepos > linelength)
        linelength = linepos;
    }

  যদি (গুনতি_chars < print_chars)
    chars = bytes;

  যদি (যোগ_mode != যোগ_only)
    write_counts (সারিগুলি, শব্দগুলি, chars, bytes, linelength, file_x);

  যোগ_সারিগুলি_overflow |= ckd_add (&যোগ_সারিগুলি, যোগ_সারিগুলি, সারিগুলি);
  যোগ_শব্দগুলি_overflow |= ckd_add (&যোগ_শব্দগুলি, যোগ_শব্দগুলি, শব্দগুলি);
  যোগ_chars_overflow |= ckd_add (&যোগ_chars, যোগ_chars, chars);
  যোগ_bytes_overflow |= ckd_add (&যোগ_bytes, যোগ_bytes, bytes);

  যদি (linelength > max_সারি_দৈর্ঘ্য)
    max_সারি_দৈর্ঘ্য = linelength;

  যদি (err)
    error (0, err, "%s", quotef (file));
  ফেরত !err;
}

স্থির bool
wc_file (অক্ষর ধ্রুবক *file, কাঠামো fstatus *fstatus)
{
  যদি (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_দ্বিমিক_mode (STDIN_FILENO, O_দ্বিমিক);
      ফেরত wc (STDIN_FILENO, file, fstatus);
    }
  অন্যথা
    {
      পূর্ণসংখ্যা fd = open (file, O_RDONLY | O_দ্বিমিক);
      যদি (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          ফেরত false;
        }
      অন্যথা
        {
          bool ok = wc (fd, file, fstatus);
          যদি (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              ফেরত false;
            }
          ফেরত ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

স্থির কাঠামো fstatus *
get_প্রবেশ_fstatus (idx_t nfiles, অক্ষর *ধ্রুবক *file)
{
  কাঠামো fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, আকার *fstatus);

  যদি (nfiles == 0
      || (nfiles == 1
          && ((print_সারিগুলি + print_শব্দগুলি + print_chars
               + print_bytes + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  অন্যথা
    {
      জন্য (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  ফেরত fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_PURE
স্থির পূর্ণসংখ্যা
compute_সংখ্যা_width (idx_t nfiles, কাঠামো fstatus ধ্রুবক *fstatus)
{
  পূর্ণসংখ্যা width = 1;

  যদি (0 < nfiles && fstatus[0].failed <= 0)
    {
      পূর্ণসংখ্যা minimum_width = 1;
      uintmax_t regular_যোগ = 0;

      জন্য (idx_t i = 0; i < nfiles; i++)
        যদি (! fstatus[i].failed)
          {
            যদি (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            অন্যথা যদি (ckd_add (&regular_যোগ, regular_যোগ,
                              fstatus[i].st.st_আকার))
              {
                regular_যোগ = UINTMAX_MAX;
                বিরতি;
              }
          }

      জন্য (; 10 <= regular_যোগ; regular_যোগ /= 10)
        width++;
      যদি (width < minimum_width)
        width = minimum_width;
    }

  ফেরত width;
}


পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা optc;
  idx_t nfiles;
  অক্ষর **files;
  অক্ষর *files_from = ফাঁকা;
  কাঠামো fstatus *fstatus;
  কাঠামো Tokens tok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  page_আকার = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ফাঁকা, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ফাঁকা);

  যখন ((optc = getopt_long (argc, argv, "clLmw", longopts, ফাঁকা)) != -1)
    নির্বাচন (optc)
      {
      ক্ষেত্র 'c':
        print_bytes = true;
        বিরতি;

      ক্ষেত্র 'm':
        print_chars = true;
        বিরতি;

      ক্ষেত্র 'l':
        print_সারিগুলি = true;
        বিরতি;

      ক্ষেত্র 'w':
        print_শব্দগুলি = true;
        বিরতি;

      ক্ষেত্র 'L':
        print_linelength = true;
        বিরতি;

      ক্ষেত্র DEBUG_PROGRAM_OPTION:
        debug = true;
        বিরতি;

      ক্ষেত্র FILES0_FROM_OPTION:
        files_from = optarg;
        বিরতি;

      ক্ষেত্র যোগ_OPTION:
        যোগ_mode = XARGMATCH ("--total", optarg, যোগ_args, যোগ_types);
        বিরতি;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      পূর্বনির্ধারিত:
        usage (EXIT_FAILURE);
      }

  যদি (! (print_সারিগুলি || print_শব্দগুলি || print_chars || print_bytes
         || print_linelength))
    print_সারিগুলি = print_শব্দগুলি = print_bytes = true;

  যদি (print_linelength)
    জন্য (পূর্ণসংখ্যা i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  যদি (print_শব্দগুলি)
    জন্য (পূর্ণসংখ্যা i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = isspace (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  কাঠামো argv_iterator *ai;
  যদি (files_from)
    {
      FILE *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      যদি (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      যদি (streq (files_from, "-"))
        stream = stdin;
      অন্যথা
        {
          stream = fopen (files_from, "r");
          যদি (stream == ফাঁকা)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      কাঠামো stat st;
      যদি (fstat (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_আকার <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          যদি (! readtokens0 (stream, &tok) || fclose (stream) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      অন্যথা
        {
          files = ফাঁকা;
          nfiles = 0;
          ai = argv_iter_init_stream (stream);
        }
    }
  অন্যথা
    {
      স্থির অক্ষর *stdin_only[] = { ফাঁকা };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  যদি (!ai)
    xalloc_die ();

  fstatus = get_প্রবেশ_fstatus (nfiles, files);
  যদি (যোগ_mode == যোগ_only)
    সংখ্যা_width = 1;  /* No extra padding, since no alignment requirement.  */
  অন্যথা
    সংখ্যা_width = compute_সংখ্যা_width (nfiles, fstatus);

  bool ok = true;
  গণনা argv_iter_err ai_err;
  অক্ষর *file_name;
  জন্য (পূর্ণসংখ্যা i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      যদি (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      যদি (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          যদি (files_from == ফাঁকা)
            error (0, 0, "%s", _("invalid zero-length file name"));
          অন্যথা
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_args (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      যদি (skip_file)
        ok = false;
      অন্যথা
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      যদি (! nfiles)
        fstatus[0].failed = 1;
    }
  নির্বাচন (ai_err)
    {
    ক্ষেত্র AI_ERR_EOF:
      বিরতি;

    ক্ষেত্র AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      বিরতি;

    ক্ষেত্র AI_ERR_MEM:
      xalloc_die ();

    ক্ষেত্র AI_ERR_OK: পূর্বনির্ধারিত:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  যদি (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (ফাঁকা, &fstatus[0]);

  যদি (read_tokens)
    readtokens0_free (&tok);

  যদি (যোগ_mode != যোগ_never
      && (যোগ_mode != যোগ_auto || 1 < argv_iter_n_args (ai)))
    {
      যদি (যোগ_সারিগুলি_overflow)
        {
          যোগ_সারিগুলি = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      যদি (যোগ_শব্দগুলি_overflow)
        {
          যোগ_শব্দগুলি = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      যদি (যোগ_chars_overflow)
        {
          যোগ_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      যদি (যোগ_bytes_overflow)
        {
          যোগ_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (যোগ_সারিগুলি, যোগ_শব্দগুলি, যোগ_chars, যোগ_bytes,
                    max_সারি_দৈর্ঘ্য,
                    যোগ_mode != যোগ_only ? _("total") : ফাঁকা);
    }

  argv_iter_free (ai);

  মুক্ত (fstatus);

  যদি (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
