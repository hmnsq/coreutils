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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن <argmatch.h>
#ضمن <argv-iter.h>
#ضمن <fadvise.h>
#ضمن <physmem.h>
#ضمن <readtokens0.h>
#ضمن <stat-size.h>
#ضمن <xbinary-io.h>

#جيڪڏهن_وصف_ٿيل USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#جيڪڏهن_پورو

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "cpu-supports.h"
#ضمن "ioblksize.h"
#ضمن "wc.h"
#ضمن "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "wc"

#وصف_ڪريو AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

ساكن bool wc_isprint[UCHAR_MAX + 1];
ساكن bool wc_isspace[UCHAR_MAX + 1];

ساكن bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
ساكن uintmax_t ڪل_سٽون;
ساكن uintmax_t ڪل_لفظ;
ساكن uintmax_t ڪل_chars;
ساكن uintmax_t ڪل_بائيٽون;
ساكن bool ڪل_سٽون_حد_کان_مٿي;
ساكن bool ڪل_لفظ_حد_کان_مٿي;
ساكن bool ڪل_chars_حد_کان_مٿي;
ساكن bool ڪل_بائيٽون_حد_کان_مٿي;
ساكن intmax_t max_سٽ_ڊيگهائي;

/* Which counts to print. */
ساكن bool print_سٽون, print_لفظ, print_chars, print_بائيٽون;
ساكن bool print_linelength;

/* The print width of each count.  */
ساكن صحيح عدد_ويڪر;

/* True if we have ever read the standard input. */
ساكن bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
ساكن idx_t صفحو_سائيز;

/* Enable to _not_ treat non breaking space as a word separator.  */
ساكن bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
بنية fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  صحيح failed;

  /* If FAILED is zero, this is the file's status.  */
  بنية stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEBUG_پروگرام_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  ڪل_OPTION,
};

ساكن بنية option ثابت longopts[] =
{
  {"bytes", no_دليل, عدم, 'c'},
  {"chars", no_دليل, عدم, 'm'},
  {"lines", no_دليل, عدم, 'l'},
  {"words", no_دليل, عدم, 'w'},
  {"debug", no_دليل, عدم, DEBUG_پروگرام_OPTION},
  {"files0-from", required_دليل, عدم, FILES0_FROM_OPTION},
  {"max-line-length", no_دليل, عدم, 'L'},
  {"total", required_دليل, عدم, ڪل_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

تعداد ڪل_type
  {
    ڪل_auto,         /* 0: default or --total=auto */
    ڪل_always,       /* 1: --total=always */
    ڪل_only,         /* 2: --total=only */
    ڪل_never         /* 3: --total=never */
  };
ساكن حرف ثابت *ثابت ڪل_دليلون[] =
{
  "auto", "always", "only", "never", عدم
};
ساكن تعداد ڪل_type ثابت ڪل_types[] =
{
  ڪل_auto, ڪل_always, ڪل_only, ڪل_never
};
ARGMATCH_VERIFY (ڪل_دليلون, ڪل_types);
ساكن تعداد ڪل_type ڪل_گهڻائي = ڪل_auto;

#جيڪڏهن_وصف_ٿيل USE_AVX2_WC_LINECOUNT
ساكن bool
avx2_supported (فراغ)
{
  bool avx2_enabled = cpu_supports ("avx2");
  إذا (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  أرجع avx2_enabled;
}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل USE_AVX512_WC_LINECOUNT
ساكن bool
avx512_supported (فراغ)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  إذا (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  أرجع avx512_enabled;
}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل USE_NEON_WC_LINECOUNT
ساكن bool
neon_supported (فراغ)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  إذا (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  أرجع neon_enabled;
}
#جيڪڏهن_پورو

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Print newline, لفظ_2, and byte counts لكل each FILE, and a ڪل سٽ إذا\n\
وڌيڪ than one FILE is specified.  A لفظ_2 is a nonempty sequence جو non white\n\
space delimited by white space characters or by شروعات or پڇاڙي جو داخلا.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options هيٺ may be استعمال_ٿيل منزل select which counts are printed, always اندريون\n\
the following ترتيب: newline, لفظ_2, character, byte, وڌ_کان_وڌ سٽ ڊيگهائي.\n\
"), stdout);
      oputs (_("\
  -c, --بائيٽون\n\
         print the byte counts\n\
"));
      oputs (_("\
  -m, --chars\n\
         print the character counts\n\
"));
      oputs (_("\
  -l, --سٽون\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what سٽ ڳڻپ acceleration is استعمال_ٿيل\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ داخلا from the files specified by\n\
         NUL-terminated نالا اندريون file F;\n\
         If F is -, اقرأ نالا from standard داخلا\n\
"));
      oputs (_("\
  -L, --max-سٽ-ڊيگهائي\n\
         print the وڌ_کان_وڌ display ويڪر\n\
"));
      oputs (_("\
  -w, --لفظ_4\n\
         print the لفظ_2 counts\n\
"));
      oputs (_("\
      --ڪل=WHEN\n\
         when منزل print a سٽ with ڪل counts;\n\
         WHEN can be: خودڪار, always, only, never\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Return non zero if POSIXLY_CORRECT is not set and WC is a non breaking
   space.  */
ATTRIBUTE_خالص
ساكن صحيح
maybe_c32isnbspace (char32_t wc)
{
  أرجع ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
ساكن فراغ
write_counts (uintmax_t سٽون,
              uintmax_t لفظ_4,
              uintmax_t chars,
              uintmax_t بائيٽون,
              intmax_t linelength,
              حرف ثابت *file)
{
  ساكن حرف ثابت format_sp_int[] = " %*s";
  حرف ثابت *format_int = format_sp_int + 1;
  حرف buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  إذا (print_سٽون)
    {
      لکو (format_int, عدد_ويڪر, umaxtostr (سٽون, buf));
      format_int = format_sp_int;
    }
  إذا (print_لفظ)
    {
      لکو (format_int, عدد_ويڪر, umaxtostr (لفظ_4, buf));
      format_int = format_sp_int;
    }
  إذا (print_chars)
    {
      لکو (format_int, عدد_ويڪر, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  إذا (print_بائيٽون)
    {
      لکو (format_int, عدد_ويڪر, umaxtostr (بائيٽون, buf));
      format_int = format_sp_int;
    }
  إذا (print_linelength)
    لکو (format_int, عدد_ويڪر, imaxtostr (linelength, buf));
  إذا (file)
    لکو (" %s", strchr (file, '\n') ? quotef (file) : file);
  اکر_لکو ('\n');

  إذا (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
ساكن بنية wc_سٽون
wc_سٽون (صحيح fd)
{
#جيڪڏهن_وصف_ٿيل USE_AVX512_WC_LINECOUNT
  ساكن نشانوارو حرف use_avx512;
  إذا (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  إذا (0 < use_avx512)
    أرجع wc_سٽون_avx512 (fd);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل USE_AVX2_WC_LINECOUNT
  ساكن نشانوارو حرف use_avx2;
  إذا (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  إذا (0 < use_avx2)
    أرجع wc_سٽون_avx2 (fd);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل USE_NEON_WC_LINECOUNT
  ساكن نشانوارو حرف use_neon;
  إذا (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  إذا (0 < use_neon)
    أرجع wc_سٽون_neon (fd);
#جيڪڏهن_پورو

  intmax_t سٽون = 0, بائيٽون = 0;
  bool long_سٽون = false;

  طالما (true)
    {
      حرف buf[IO_BUFSIZE + 1];
      ssize_t بائيٽون_read = اقرأ (fd, buf, IO_BUFSIZE);
      إذا (بائيٽون_read <= 0)
        أرجع (بنية wc_سٽون) { بائيٽون_read == 0 ? 0 : errno, سٽون, بائيٽون };

      بائيٽون += بائيٽون_read;
      حرف *پڇاڙي = buf + بائيٽون_read;
      idx_t buflines = 0;

      إذا (! long_سٽون)
        {
          /* Avoid function call overhead for shorter lines.  */
          لكل (حرف *p = buf; p < پڇاڙي; p++)
            buflines += *p == '\n';
        }
      وإلا
        {
          /* rawmemchr is more efficient with longer lines.  */
          *پڇاڙي = '\n';
          لكل (حرف *p = buf; (p = rawmemchr (p, '\n')) < پڇاڙي; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_سٽون = 15 * buflines <= بائيٽون_read;
      سٽون += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
ساكن bool
wc (صحيح fd, حرف ثابت *file_x, بنية fstatus *fstatus)
{
  صحيح err = 0;
  حرف buf[IO_BUFSIZE + 1];
  intmax_t سٽون, لفظ_4, chars, بائيٽون, linelength;
  bool ڳڻپ_بائيٽون_2, ڳڻپ_chars, ڳڻپ_complicated;
  حرف ثابت *file = file_x ? file_x : _("standard input");

  سٽون = لفظ_4 = chars = بائيٽون = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  إذا (MB_CUR_MAX > 1)
    {
      ڳڻپ_بائيٽون_2 = print_بائيٽون;
      ڳڻپ_chars = print_chars;
    }
  وإلا
    {
      ڳڻپ_بائيٽون_2 = print_بائيٽون || print_chars;
      ڳڻپ_chars = false;
    }
  ڳڻپ_complicated = print_لفظ || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  إذا (!ڳڻپ_بائيٽون_2 || ڳڻپ_chars || print_سٽون || ڳڻپ_complicated)
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

  إذا (ڳڻپ_بائيٽون_2 && !ڳڻپ_chars && !print_سٽون && !ڳڻپ_complicated)
    {
      bool skip_read = false;

      إذا (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      إذا (! fstatus->failed && usable_st_سائيز (&fstatus->st)
          && 0 <= fstatus->st.st_سائيز)
        {
          off_t پڇاڙي_pos = fstatus->st.st_سائيز;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          إذا (current_pos < 0)
            ;
          وإلا إذا (پڇاڙي_pos % صفحو_سائيز)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              إذا (current_pos < پڇاڙي_pos)
                {
                  بائيٽون = پڇاڙي_pos - current_pos;
                  skip_read = true;
                }
            }
          وإلا إذا (0 < پڇاڙي_pos)
            {
              off_t ep_1 = پڇاڙي_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              إذا (current_pos < hi_pos)
                بائيٽون = hi_pos - current_pos;
            }

          إذا (0 < بائيٽون && lseek (fd, بائيٽون, SEEK_CUR) < 0)
            {
              بائيٽون = 0;
              skip_read = false;
            }
        }

      إذا (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          لكل (ssize_t بائيٽون_read;
               (بائيٽون_read = اقرأ (fd, buf, IO_BUFSIZE));
               بائيٽون += بائيٽون_read)
            إذا (بائيٽون_read < 0)
              {
                err = errno;
                اكسر;
              }
        }
    }
  وإلا إذا (!ڳڻپ_chars && !ڳڻپ_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      بنية wc_سٽون w = wc_سٽون (fd);
      err = w.err;
      سٽون = w.سٽون;
      بائيٽون = w.بائيٽون;
    }
  وإلا إذا (MB_CUR_MAX > 1)
    {
      bool اندريون_لفظ = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool اندريون_سرڪايو = false;
      bool ڳڻپ_utf8 = !ڳڻپ_complicated && !print_سٽون
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      لكل (ssize_t بائيٽون_read;
           ((بائيٽون_read = اقرأ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          إذا (بائيٽون_read < 0)
            {
              err = errno;
              اكسر;
            }

          بائيٽون += بائيٽون_read;
          حرف ثابت *p = buf;
          حرف ثابت *plim = p + prev + بائيٽون_read;
          bool try_ڳڻپ_utf8 = ڳڻپ_utf8;
          افعل
            {
              إذا (try_ڳڻپ_utf8 && !اندريون_سرڪايو)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_ڳڻپ_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_ڳڻپ (p, &n);
                  p += n;
                  إذا (p == plim)
                    اكسر;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              إذا (!اندريون_سرڪايو && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              وإلا
                {
                  idx_t prev_بائيٽون = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  إذا (scanbytes < n)
                    {
                      إذا (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && بائيٽون_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          اندريون_سرڪايو = true;
                          اكسر;
                        }

                      /* Remember that we read a byte, but don't complain
                         about the error.  Because of the decoding error,
                         this is a considered to be byte but not a
                         character (that is, chars is not incremented).  */
                      p++;
                      mbszero (&state);
                      اندريون_سرڪايو = false;

                      /* Treat encoding errors as non white space.
                         POSIX says a word is "a non-zero-length string of
                         characters delimited by white space".  This is
                         wrong in some sense, as the string can be delimited
                         by start or end of input, and it is unclear what it
                         means when the input contains encoding errors.
                         Since encoding errors are not white space,
                         treat them that way here.  */
                      لفظ_4 += !اندريون_لفظ;
                      اندريون_لفظ = true;
                      استمر;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_بائيٽون + n + !n;
                  single_byte = charbytes == !اندريون_سرڪايو;
                  اندريون_سرڪايو = !mbsinit (&state);
                }

              بدل (wide_char)
                {
                حالة '\n':
                  سٽون++;
                  FALLTHROUGH;
                حالة '\r':
                حالة '\f':
                  إذا (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  اندريون_لفظ = false;
                  اكسر;

                حالة '\t':
                  linepos += 8 - (linepos % 8);
                  اندريون_لفظ = false;
                  اكسر;

                حالة ' ':
                  linepos++;
                  FALLTHROUGH;
                حالة '\v':
                  اندريون_لفظ = false;
                  اكسر;

                افتراضي:;
                  bool اندريون_word2;
                  إذا (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      اندريون_word2 = !wc_isspace[wide_char];
                    }
                  وإلا
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      إذا (print_linelength)
                        {
                          صحيح ويڪر = c32width (wide_char);
                          إذا (ويڪر > 0)
                            linepos += ويڪر;
                        }
                      اندريون_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  لفظ_4 += !اندريون_لفظ & اندريون_word2;
                  اندريون_لفظ = اندريون_word2;
                  اكسر;
                }

              p += charbytes;
              chars++;
            }
          طالما (p < plim);
        }
      إذا (linepos > linelength)
        linelength = linepos;
    }
  وإلا
    {
      bool اندريون_لفظ = false;
      intmax_t linepos = 0;

      لكل (ssize_t بائيٽون_read; (بائيٽون_read = اقرأ (fd, buf, IO_BUFSIZE)); )
        {
          إذا (بائيٽون_read < 0)
            {
              err = errno;
              اكسر;
            }

          بائيٽون += بائيٽون_read;
          حرف ثابت *p = buf;
          افعل
            {
              بنانشان حرف c = *p++;
              بدل (c)
                {
                حالة '\n':
                  سٽون++;
                  FALLTHROUGH;
                حالة '\r':
                حالة '\f':
                  إذا (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  اندريون_لفظ = false;
                  اكسر;

                حالة '\t':
                  linepos += 8 - (linepos % 8);
                  اندريون_لفظ = false;
                  اكسر;

                حالة ' ':
                  linepos++;
                  FALLTHROUGH;
                حالة '\v':
                  اندريون_لفظ = false;
                  اكسر;

                افتراضي:
                  linepos += wc_isprint[c];
                  bool اندريون_word2 = !wc_isspace[c];
                  لفظ_4 += !اندريون_لفظ & اندريون_word2;
                  اندريون_لفظ = اندريون_word2;
                  اكسر;
                }
            }
          طالما (--بائيٽون_read);
        }
      إذا (linepos > linelength)
        linelength = linepos;
    }

  إذا (ڳڻپ_chars < print_chars)
    chars = بائيٽون;

  إذا (ڪل_گهڻائي != ڪل_only)
    write_counts (سٽون, لفظ_4, chars, بائيٽون, linelength, file_x);

  ڪل_سٽون_حد_کان_مٿي |= ckd_جوڙيو (&ڪل_سٽون, ڪل_سٽون, سٽون);
  ڪل_لفظ_حد_کان_مٿي |= ckd_جوڙيو (&ڪل_لفظ, ڪل_لفظ, لفظ_4);
  ڪل_chars_حد_کان_مٿي |= ckd_جوڙيو (&ڪل_chars, ڪل_chars, chars);
  ڪل_بائيٽون_حد_کان_مٿي |= ckd_جوڙيو (&ڪل_بائيٽون, ڪل_بائيٽون, بائيٽون);

  إذا (linelength > max_سٽ_ڊيگهائي)
    max_سٽ_ڊيگهائي = linelength;

  إذا (err)
    error (0, err, "%s", quotef (file));
  أرجع !err;
}

ساكن bool
wc_file (حرف ثابت *file, بنية fstatus *fstatus)
{
  إذا (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_ٻٽو_گهڻائي (STDIN_FILENO, O_ٻٽو);
      أرجع wc (STDIN_FILENO, file, fstatus);
    }
  وإلا
    {
      صحيح fd = open (file, O_RDONLY | O_ٻٽو);
      إذا (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          أرجع false;
        }
      وإلا
        {
          bool ok = wc (fd, file, fstatus);
          إذا (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              أرجع false;
            }
          أرجع ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

ساكن بنية fstatus *
get_داخلا_fstatus (idx_t nfiles, حرف *ثابت *file)
{
  بنية fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, حجم *fstatus);

  إذا (nfiles == 0
      || (nfiles == 1
          && ((print_سٽون + print_لفظ + print_chars
               + print_بائيٽون + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  وإلا
    {
      لكل (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  أرجع fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_خالص
ساكن صحيح
compute_عدد_ويڪر (idx_t nfiles, بنية fstatus ثابت *fstatus)
{
  صحيح ويڪر = 1;

  إذا (0 < nfiles && fstatus[0].failed <= 0)
    {
      صحيح minimum_ويڪر = 1;
      uintmax_t regular_ڪل = 0;

      لكل (idx_t i = 0; i < nfiles; i++)
        إذا (! fstatus[i].failed)
          {
            إذا (!S_ISREG (fstatus[i].st.st_گهڻائي))
              minimum_ويڪر = 7;
            وإلا إذا (ckd_جوڙيو (&regular_ڪل, regular_ڪل,
                              fstatus[i].st.st_سائيز))
              {
                regular_ڪل = UINTMAX_MAX;
                اكسر;
              }
          }

      لكل (; 10 <= regular_ڪل; regular_ڪل /= 10)
        ويڪر++;
      إذا (ويڪر < minimum_ويڪر)
        ويڪر = minimum_ويڪر;
    }

  أرجع ويڪر;
}


صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح optc;
  idx_t nfiles;
  حرف **files;
  حرف *files_from = عدم;
  بنية fstatus *fstatus;
  بنية ٽوڪن tok;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صفحو_سائيز = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, عدم, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);

  طالما ((optc = getopt_long (argc, argv, "clLmw", longopts, عدم)) != -1)
    بدل (optc)
      {
      حالة 'c':
        print_بائيٽون = true;
        اكسر;

      حالة 'm':
        print_chars = true;
        اكسر;

      حالة 'l':
        print_سٽون = true;
        اكسر;

      حالة 'w':
        print_لفظ = true;
        اكسر;

      حالة 'L':
        print_linelength = true;
        اكسر;

      حالة DEBUG_پروگرام_OPTION:
        debug = true;
        اكسر;

      حالة FILES0_FROM_OPTION:
        files_from = optarg;
        اكسر;

      حالة ڪل_OPTION:
        ڪل_گهڻائي = XARGMATCH ("--total", optarg, ڪل_دليلون, ڪل_types);
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

      افتراضي:
        usage (EXIT_FAILURE);
      }

  إذا (! (print_سٽون || print_لفظ || print_chars || print_بائيٽون
         || print_linelength))
    print_سٽون = print_لفظ = print_بائيٽون = true;

  إذا (print_linelength)
    لكل (صحيح i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  إذا (print_لفظ)
    لكل (صحيح i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = خال_آهي (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_ٽوڪن = false;
  بنية argv_iterator *ai;
  إذا (files_from)
    {
      FILE *وهڪرو;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      إذا (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      إذا (streq (files_from, "-"))
        وهڪرو = stdin;
      وإلا
        {
          وهڪرو = fopen (files_from, "r");
          إذا (وهڪرو == عدم)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      بنية stat st;
      إذا (fstat (fileno (وهڪرو), &st) == 0
          && S_ISREG (st.st_گهڻائي)
          && st.st_سائيز <= MIN (10 * 1024 * 1024, physmem_موجود () / 2))
        {
          read_ٽوڪن = true;
          readtokens0_init (&tok);
          إذا (! readtokens0 (وهڪرو, &tok) || fclose (وهڪرو) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      وإلا
        {
          files = عدم;
          nfiles = 0;
          ai = argv_iter_init_وهڪرو (وهڪرو);
        }
    }
  وإلا
    {
      ساكن حرف *stdin_only[] = { عدم };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  إذا (!ai)
    xalloc_die ();

  fstatus = get_داخلا_fstatus (nfiles, files);
  إذا (ڪل_گهڻائي == ڪل_only)
    عدد_ويڪر = 1;  /* No extra padding, since no alignment requirement.  */
  وإلا
    عدد_ويڪر = compute_عدد_ويڪر (nfiles, fstatus);

  bool ok = true;
  تعداد argv_iter_err ai_err;
  حرف *file_name;
  لكل (صحيح i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      إذا (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      إذا (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          إذا (files_from == عدم)
            error (0, 0, "%s", _("invalid zero-length file name"));
          وإلا
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_دليلون (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      إذا (skip_file)
        ok = false;
      وإلا
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      إذا (! nfiles)
        fstatus[0].failed = 1;
    }
  بدل (ai_err)
    {
    حالة AI_ERR_EOF:
      اكسر;

    حالة AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      اكسر;

    حالة AI_ERR_MEM:
      xalloc_die ();

    حالة AI_ERR_OK: افتراضي:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  إذا (ok && !files_from && argv_iter_n_دليلون (ai) == 0)
    ok &= wc_file (عدم, &fstatus[0]);

  إذا (read_ٽوڪن)
    readtokens0_free (&tok);

  إذا (ڪل_گهڻائي != ڪل_never
      && (ڪل_گهڻائي != ڪل_auto || 1 < argv_iter_n_دليلون (ai)))
    {
      إذا (ڪل_سٽون_حد_کان_مٿي)
        {
          ڪل_سٽون = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      إذا (ڪل_لفظ_حد_کان_مٿي)
        {
          ڪل_لفظ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      إذا (ڪل_chars_حد_کان_مٿي)
        {
          ڪل_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      إذا (ڪل_بائيٽون_حد_کان_مٿي)
        {
          ڪل_بائيٽون = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (ڪل_سٽون, ڪل_لفظ, ڪل_chars, ڪل_بائيٽون,
                    max_سٽ_ڊيگهائي,
                    ڪل_گهڻائي != ڪل_only ? _("total") : عدم);
    }

  argv_iter_free (ai);

  حرر (fstatus);

  إذا (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
