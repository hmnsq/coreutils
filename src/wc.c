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

#جے_تعریف_شدہ USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#جے_انت

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "cpu-supports.h"
#ضمن "ioblksize.h"
#ضمن "wc.h"
#ضمن "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "wc"

#تعریف_کرو AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

ساكن bool wc_isprint[UCHAR_MAX + 1];
ساكن bool wc_isspace[UCHAR_MAX + 1];

ساكن bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
ساكن uintmax_t کل_سطریں;
ساكن uintmax_t کل_الفاظ;
ساكن uintmax_t کل_chars;
ساكن uintmax_t کل_بائٹاں;
ساكن bool کل_سطریں_حد_توں_ودھ;
ساكن bool کل_الفاظ_حد_توں_ودھ;
ساكن bool کل_chars_حد_توں_ودھ;
ساكن bool کل_بائٹاں_حد_توں_ودھ;
ساكن intmax_t max_سطر_لمائی;

/* Which counts to print. */
ساكن bool print_سطریں, print_الفاظ, print_chars, print_بائٹاں;
ساكن bool print_linelength;

/* The print width of each count.  */
ساكن صحيح نمبر_چوڑائی;

/* True if we have ever read the standard input. */
ساكن bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
ساكن idx_t ورقہ_ناپ;

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
  کل_OPTION,
};

ساكن بنية option ثابت longopts[] =
{
  {"bytes", no_دلیل, عدم, 'c'},
  {"chars", no_دلیل, عدم, 'm'},
  {"lines", no_دلیل, عدم, 'l'},
  {"words", no_دلیل, عدم, 'w'},
  {"debug", no_دلیل, عدم, DEBUG_پروگرام_OPTION},
  {"files0-from", required_دلیل, عدم, FILES0_FROM_OPTION},
  {"max-line-length", no_دلیل, عدم, 'L'},
  {"total", required_دلیل, عدم, کل_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

تعداد کل_type
  {
    کل_auto,         /* 0: default or --total=auto */
    کل_always,       /* 1: --total=always */
    کل_only,         /* 2: --total=only */
    کل_never         /* 3: --total=never */
  };
ساكن حرف ثابت *ثابت کل_دلیلاں[] =
{
  "auto", "always", "only", "never", عدم
};
ساكن تعداد کل_type ثابت کل_types[] =
{
  کل_auto, کل_always, کل_only, کل_never
};
ARGMATCH_VERIFY (کل_دلیلاں, کل_types);
ساكن تعداد کل_type کل_منوال = کل_auto;

#جے_تعریف_شدہ USE_AVX2_WC_LINECOUNT
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
#جے_انت

#جے_تعریف_شدہ USE_AVX512_WC_LINECOUNT
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
#جے_انت

#جے_تعریف_شدہ USE_NEON_WC_LINECOUNT
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
#جے_انت

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Print newline, لفظ_2, and byte counts لكل each FILE, and a کل سطر إذا\n\
ہور than one FILE is specified.  A لفظ_2 is a nonempty sequence دا non white\n\
space delimited by white space characters or by شروع or سرا دا داخلہ.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options ہیٹھاں may be ورتیا منزل select which counts are printed, always وچ\n\
the following ترتیب: newline, لفظ_2, character, byte, ودھ_توں_ودھ سطر لمائی.\n\
"), stdout);
      oputs (_("\
  -c, --بائٹاں\n\
         print the byte counts\n\
"));
      oputs (_("\
  -m, --chars\n\
         print the character counts\n\
"));
      oputs (_("\
  -l, --سطریں\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what سطر گنتی acceleration is ورتیا\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ داخلہ from the files specified by\n\
         NUL-terminated ناں وچ file F;\n\
         If F is -, اقرأ ناں from standard داخلہ\n\
"));
      oputs (_("\
  -L, --max-سطر-لمائی\n\
         print the ودھ_توں_ودھ display چوڑائی\n\
"));
      oputs (_("\
  -w, --الفاظ\n\
         print the لفظ_2 counts\n\
"));
      oputs (_("\
      --کل=WHEN\n\
         when منزل print a سطر with کل counts;\n\
         WHEN can be: خودکار, always, only, never\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
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
write_counts (uintmax_t سطریں,
              uintmax_t الفاظ,
              uintmax_t chars,
              uintmax_t بائٹاں,
              intmax_t linelength,
              حرف ثابت *file)
{
  ساكن حرف ثابت format_sp_int[] = " %*s";
  حرف ثابت *format_int = format_sp_int + 1;
  حرف buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  إذا (print_سطریں)
    {
      لکھو (format_int, نمبر_چوڑائی, umaxtostr (سطریں, buf));
      format_int = format_sp_int;
    }
  إذا (print_الفاظ)
    {
      لکھو (format_int, نمبر_چوڑائی, umaxtostr (الفاظ, buf));
      format_int = format_sp_int;
    }
  إذا (print_chars)
    {
      لکھو (format_int, نمبر_چوڑائی, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  إذا (print_بائٹاں)
    {
      لکھو (format_int, نمبر_چوڑائی, umaxtostr (بائٹاں, buf));
      format_int = format_sp_int;
    }
  إذا (print_linelength)
    لکھو (format_int, نمبر_چوڑائی, imaxtostr (linelength, buf));
  إذا (file)
    لکھو (" %s", strchr (file, '\n') ? quotef (file) : file);
  حرف_لکھو ('\n');

  إذا (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
ساكن بنية wc_سطریں
wc_سطریں (صحيح fd)
{
#جے_تعریف_شدہ USE_AVX512_WC_LINECOUNT
  ساكن نشانیوالا حرف use_avx512;
  إذا (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  إذا (0 < use_avx512)
    أرجع wc_سطریں_avx512 (fd);
#جے_انت
#جے_تعریف_شدہ USE_AVX2_WC_LINECOUNT
  ساكن نشانیوالا حرف use_avx2;
  إذا (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  إذا (0 < use_avx2)
    أرجع wc_سطریں_avx2 (fd);
#جے_انت
#جے_تعریف_شدہ USE_NEON_WC_LINECOUNT
  ساكن نشانیوالا حرف use_neon;
  إذا (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  إذا (0 < use_neon)
    أرجع wc_سطریں_neon (fd);
#جے_انت

  intmax_t سطریں = 0, بائٹاں = 0;
  bool long_سطریں = false;

  طالما (true)
    {
      حرف buf[IO_BUFSIZE + 1];
      ssize_t بائٹاں_read = اقرأ (fd, buf, IO_BUFSIZE);
      إذا (بائٹاں_read <= 0)
        أرجع (بنية wc_سطریں) { بائٹاں_read == 0 ? 0 : errno, سطریں, بائٹاں };

      بائٹاں += بائٹاں_read;
      حرف *سرا = buf + بائٹاں_read;
      idx_t buflines = 0;

      إذا (! long_سطریں)
        {
          /* Avoid function call overhead for shorter lines.  */
          لكل (حرف *p = buf; p < سرا; p++)
            buflines += *p == '\n';
        }
      وإلا
        {
          /* rawmemchr is more efficient with longer lines.  */
          *سرا = '\n';
          لكل (حرف *p = buf; (p = rawmemchr (p, '\n')) < سرا; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_سطریں = 15 * buflines <= بائٹاں_read;
      سطریں += buflines;
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
  intmax_t سطریں, الفاظ, chars, بائٹاں, linelength;
  bool گنتی_بائٹاں_2, گنتی_chars, گنتی_complicated;
  حرف ثابت *file = file_x ? file_x : _("standard input");

  سطریں = الفاظ = chars = بائٹاں = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  إذا (MB_CUR_MAX > 1)
    {
      گنتی_بائٹاں_2 = print_بائٹاں;
      گنتی_chars = print_chars;
    }
  وإلا
    {
      گنتی_بائٹاں_2 = print_بائٹاں || print_chars;
      گنتی_chars = false;
    }
  گنتی_complicated = print_الفاظ || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  إذا (!گنتی_بائٹاں_2 || گنتی_chars || print_سطریں || گنتی_complicated)
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

  إذا (گنتی_بائٹاں_2 && !گنتی_chars && !print_سطریں && !گنتی_complicated)
    {
      bool skip_read = false;

      إذا (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      إذا (! fstatus->failed && usable_st_ناپ (&fstatus->st)
          && 0 <= fstatus->st.st_ناپ)
        {
          off_t سرا_pos = fstatus->st.st_ناپ;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          إذا (current_pos < 0)
            ;
          وإلا إذا (سرا_pos % ورقہ_ناپ)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              إذا (current_pos < سرا_pos)
                {
                  بائٹاں = سرا_pos - current_pos;
                  skip_read = true;
                }
            }
          وإلا إذا (0 < سرا_pos)
            {
              off_t ep_1 = سرا_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              إذا (current_pos < hi_pos)
                بائٹاں = hi_pos - current_pos;
            }

          إذا (0 < بائٹاں && lseek (fd, بائٹاں, SEEK_CUR) < 0)
            {
              بائٹاں = 0;
              skip_read = false;
            }
        }

      إذا (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          لكل (ssize_t بائٹاں_read;
               (بائٹاں_read = اقرأ (fd, buf, IO_BUFSIZE));
               بائٹاں += بائٹاں_read)
            إذا (بائٹاں_read < 0)
              {
                err = errno;
                اكسر;
              }
        }
    }
  وإلا إذا (!گنتی_chars && !گنتی_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      بنية wc_سطریں w = wc_سطریں (fd);
      err = w.err;
      سطریں = w.سطریں;
      بائٹاں = w.بائٹاں;
    }
  وإلا إذا (MB_CUR_MAX > 1)
    {
      bool وچ_لفظ = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool وچ_کھسکاؤ = false;
      bool گنتی_utf8 = !گنتی_complicated && !print_سطریں
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      لكل (ssize_t بائٹاں_read;
           ((بائٹاں_read = اقرأ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          إذا (بائٹاں_read < 0)
            {
              err = errno;
              اكسر;
            }

          بائٹاں += بائٹاں_read;
          حرف ثابت *p = buf;
          حرف ثابت *plim = p + prev + بائٹاں_read;
          bool try_گنتی_utf8 = گنتی_utf8;
          افعل
            {
              إذا (try_گنتی_utf8 && !وچ_کھسکاؤ)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_گنتی_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_گنتی (p, &n);
                  p += n;
                  إذا (p == plim)
                    اكسر;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              إذا (!وچ_کھسکاؤ && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              وإلا
                {
                  idx_t prev_بائٹاں = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  إذا (scanbytes < n)
                    {
                      إذا (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && بائٹاں_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          وچ_کھسکاؤ = true;
                          اكسر;
                        }

                      /* Remember that we read a byte, but don't complain
                         about the error.  Because of the decoding error,
                         this is a considered to be byte but not a
                         character (that is, chars is not incremented).  */
                      p++;
                      mbszero (&state);
                      وچ_کھسکاؤ = false;

                      /* Treat encoding errors as non white space.
                         POSIX says a word is "a non-zero-length string of
                         characters delimited by white space".  This is
                         wrong in some sense, as the string can be delimited
                         by start or end of input, and it is unclear what it
                         means when the input contains encoding errors.
                         Since encoding errors are not white space,
                         treat them that way here.  */
                      الفاظ += !وچ_لفظ;
                      وچ_لفظ = true;
                      استمر;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_بائٹاں + n + !n;
                  single_byte = charbytes == !وچ_کھسکاؤ;
                  وچ_کھسکاؤ = !mbsinit (&state);
                }

              بدل (wide_char)
                {
                حالة '\n':
                  سطریں++;
                  FALLTHROUGH;
                حالة '\r':
                حالة '\f':
                  إذا (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  وچ_لفظ = false;
                  اكسر;

                حالة '\t':
                  linepos += 8 - (linepos % 8);
                  وچ_لفظ = false;
                  اكسر;

                حالة ' ':
                  linepos++;
                  FALLTHROUGH;
                حالة '\v':
                  وچ_لفظ = false;
                  اكسر;

                افتراضي:;
                  bool وچ_word2;
                  إذا (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      وچ_word2 = !wc_isspace[wide_char];
                    }
                  وإلا
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      إذا (print_linelength)
                        {
                          صحيح چوڑائی = c32width (wide_char);
                          إذا (چوڑائی > 0)
                            linepos += چوڑائی;
                        }
                      وچ_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  الفاظ += !وچ_لفظ & وچ_word2;
                  وچ_لفظ = وچ_word2;
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
      bool وچ_لفظ = false;
      intmax_t linepos = 0;

      لكل (ssize_t بائٹاں_read; (بائٹاں_read = اقرأ (fd, buf, IO_BUFSIZE)); )
        {
          إذا (بائٹاں_read < 0)
            {
              err = errno;
              اكسر;
            }

          بائٹاں += بائٹاں_read;
          حرف ثابت *p = buf;
          افعل
            {
              بنانشانی حرف c = *p++;
              بدل (c)
                {
                حالة '\n':
                  سطریں++;
                  FALLTHROUGH;
                حالة '\r':
                حالة '\f':
                  إذا (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  وچ_لفظ = false;
                  اكسر;

                حالة '\t':
                  linepos += 8 - (linepos % 8);
                  وچ_لفظ = false;
                  اكسر;

                حالة ' ':
                  linepos++;
                  FALLTHROUGH;
                حالة '\v':
                  وچ_لفظ = false;
                  اكسر;

                افتراضي:
                  linepos += wc_isprint[c];
                  bool وچ_word2 = !wc_isspace[c];
                  الفاظ += !وچ_لفظ & وچ_word2;
                  وچ_لفظ = وچ_word2;
                  اكسر;
                }
            }
          طالما (--بائٹاں_read);
        }
      إذا (linepos > linelength)
        linelength = linepos;
    }

  إذا (گنتی_chars < print_chars)
    chars = بائٹاں;

  إذا (کل_منوال != کل_only)
    write_counts (سطریں, الفاظ, chars, بائٹاں, linelength, file_x);

  کل_سطریں_حد_توں_ودھ |= ckd_جوڑو (&کل_سطریں, کل_سطریں, سطریں);
  کل_الفاظ_حد_توں_ودھ |= ckd_جوڑو (&کل_الفاظ, کل_الفاظ, الفاظ);
  کل_chars_حد_توں_ودھ |= ckd_جوڑو (&کل_chars, کل_chars, chars);
  کل_بائٹاں_حد_توں_ودھ |= ckd_جوڑو (&کل_بائٹاں, کل_بائٹاں, بائٹاں);

  إذا (linelength > max_سطر_لمائی)
    max_سطر_لمائی = linelength;

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
      xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
      أرجع wc (STDIN_FILENO, file, fstatus);
    }
  وإلا
    {
      صحيح fd = open (file, O_RDONLY | O_ثنائی);
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
get_داخلہ_fstatus (idx_t nfiles, حرف *ثابت *file)
{
  بنية fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, حجم *fstatus);

  إذا (nfiles == 0
      || (nfiles == 1
          && ((print_سطریں + print_الفاظ + print_chars
               + print_بائٹاں + print_linelength)
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
compute_نمبر_چوڑائی (idx_t nfiles, بنية fstatus ثابت *fstatus)
{
  صحيح چوڑائی = 1;

  إذا (0 < nfiles && fstatus[0].failed <= 0)
    {
      صحيح minimum_چوڑائی = 1;
      uintmax_t regular_کل = 0;

      لكل (idx_t i = 0; i < nfiles; i++)
        إذا (! fstatus[i].failed)
          {
            إذا (!S_ISREG (fstatus[i].st.st_منوال))
              minimum_چوڑائی = 7;
            وإلا إذا (ckd_جوڑو (&regular_کل, regular_کل,
                              fstatus[i].st.st_ناپ))
              {
                regular_کل = UINTMAX_MAX;
                اكسر;
              }
          }

      لكل (; 10 <= regular_کل; regular_کل /= 10)
        چوڑائی++;
      إذا (چوڑائی < minimum_چوڑائی)
        چوڑائی = minimum_چوڑائی;
    }

  أرجع چوڑائی;
}


صحيح
مکھ (صحيح argc, حرف **argv)
{
  صحيح optc;
  idx_t nfiles;
  حرف **files;
  حرف *files_from = عدم;
  بنية fstatus *fstatus;
  بنية ٹوکن tok;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ورقہ_ناپ = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, عدم, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);

  طالما ((optc = getopt_long (argc, argv, "clLmw", longopts, عدم)) != -1)
    بدل (optc)
      {
      حالة 'c':
        print_بائٹاں = true;
        اكسر;

      حالة 'm':
        print_chars = true;
        اكسر;

      حالة 'l':
        print_سطریں = true;
        اكسر;

      حالة 'w':
        print_الفاظ = true;
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

      حالة کل_OPTION:
        کل_منوال = XARGMATCH ("--total", optarg, کل_دلیلاں, کل_types);
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

      افتراضي:
        usage (EXIT_FAILURE);
      }

  إذا (! (print_سطریں || print_الفاظ || print_chars || print_بائٹاں
         || print_linelength))
    print_سطریں = print_الفاظ = print_بائٹاں = true;

  إذا (print_linelength)
    لكل (صحيح i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  إذا (print_الفاظ)
    لكل (صحيح i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = خلا_اے (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_ٹوکن = false;
  بنية argv_iterator *ai;
  إذا (files_from)
    {
      FILE *دھارا;

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
        دھارا = stdin;
      وإلا
        {
          دھارا = fopen (files_from, "r");
          إذا (دھارا == عدم)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      بنية stat st;
      إذا (fstat (fileno (دھارا), &st) == 0
          && S_ISREG (st.st_منوال)
          && st.st_ناپ <= MIN (10 * 1024 * 1024, physmem_دستیاب () / 2))
        {
          read_ٹوکن = true;
          readtokens0_init (&tok);
          إذا (! readtokens0 (دھارا, &tok) || fclose (دھارا) != 0)
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
          ai = argv_iter_init_دھارا (دھارا);
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

  fstatus = get_داخلہ_fstatus (nfiles, files);
  إذا (کل_منوال == کل_only)
    نمبر_چوڑائی = 1;  /* No extra padding, since no alignment requirement.  */
  وإلا
    نمبر_چوڑائی = compute_نمبر_چوڑائی (nfiles, fstatus);

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
                     argv_iter_n_دلیلاں (ai), _("invalid zero-length file name"));
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
  إذا (ok && !files_from && argv_iter_n_دلیلاں (ai) == 0)
    ok &= wc_file (عدم, &fstatus[0]);

  إذا (read_ٹوکن)
    readtokens0_free (&tok);

  إذا (کل_منوال != کل_never
      && (کل_منوال != کل_auto || 1 < argv_iter_n_دلیلاں (ai)))
    {
      إذا (کل_سطریں_حد_توں_ودھ)
        {
          کل_سطریں = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      إذا (کل_الفاظ_حد_توں_ودھ)
        {
          کل_الفاظ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      إذا (کل_chars_حد_توں_ودھ)
        {
          کل_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      إذا (کل_بائٹاں_حد_توں_ودھ)
        {
          کل_بائٹاں = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (کل_سطریں, کل_الفاظ, کل_chars, کل_بائٹاں,
                    max_سطر_لمائی,
                    کل_منوال != کل_only ? _("total") : عدم);
    }

  argv_iter_free (ai);

  حرر (fstatus);

  إذا (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
