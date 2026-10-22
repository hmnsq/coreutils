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

#اگر_تعریف_شده USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#پایان_اگر

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "cpu-supports.h"
#ضمن "ioblksize.h"
#ضمن "wc.h"
#ضمن "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "wc"

#تعریف_کن AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

ساكن bool wc_isprint[UCHAR_MAX + 1];
ساكن bool wc_isspace[UCHAR_MAX + 1];

ساكن bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
ساكن uintmax_t مجموع_سطرها;
ساكن uintmax_t مجموع_واژگان;
ساكن uintmax_t مجموع_chars;
ساكن uintmax_t مجموع_بایتها;
ساكن bool مجموع_سطرها_سرریز;
ساكن bool مجموع_واژگان_سرریز;
ساكن bool مجموع_chars_سرریز;
ساكن bool مجموع_بایتها_سرریز;
ساكن intmax_t max_سطر_درازی;

/* Which counts to print. */
ساكن bool print_سطرها, print_واژگان, print_chars, print_بایتها;
ساكن bool print_linelength;

/* The print width of each count.  */
ساكن صحيح عدد_عرض;

/* True if we have ever read the standard input. */
ساكن bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
ساكن idx_t صفحه_اندازه;

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
  DEBUG_برنامه_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  مجموع_OPTION,
};

ساكن بنية option ثابت longopts[] =
{
  {"bytes", no_آرگومان, عدم, 'c'},
  {"chars", no_آرگومان, عدم, 'm'},
  {"lines", no_آرگومان, عدم, 'l'},
  {"words", no_آرگومان, عدم, 'w'},
  {"debug", no_آرگومان, عدم, DEBUG_برنامه_OPTION},
  {"files0-from", required_آرگومان, عدم, FILES0_FROM_OPTION},
  {"max-line-length", no_آرگومان, عدم, 'L'},
  {"total", required_آرگومان, عدم, مجموع_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

تعداد مجموع_type
  {
    مجموع_auto,         /* 0: default or --total=auto */
    مجموع_always,       /* 1: --total=always */
    مجموع_only,         /* 2: --total=only */
    مجموع_never         /* 3: --total=never */
  };
ساكن حرف ثابت *ثابت مجموع_آرگومانها[] =
{
  "auto", "always", "only", "never", عدم
};
ساكن تعداد مجموع_type ثابت مجموع_types[] =
{
  مجموع_auto, مجموع_always, مجموع_only, مجموع_never
};
ARGMATCH_VERIFY (مجموع_آرگومانها, مجموع_types);
ساكن تعداد مجموع_type مجموع_نما = مجموع_auto;

#اگر_تعریف_شده USE_AVX2_WC_LINECOUNT
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
#پایان_اگر

#اگر_تعریف_شده USE_AVX512_WC_LINECOUNT
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
#پایان_اگر

#اگر_تعریف_شده USE_NEON_WC_LINECOUNT
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
#پایان_اگر

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              برنامه_name, برنامه_name);
      fputs (_("\
Print newline, کلمه_2, and byte counts لكل each FILE, and a مجموع سطر إذا\n\
بیشتر than one FILE is specified.  A کلمه_2 is a nonempty sequence از non white\n\
space delimited by white space characters or by شروع or انتها از درون_داد.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options زیر may be استفاده_شده مقصد select which counts are printed, always درون\n\
the following ترتیب: newline, کلمه_2, character, byte, بیشینه سطر درازی.\n\
"), stdout);
      oputs (_("\
  -c, --بایتها\n\
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
         indicate what سطر شمار acceleration is استفاده_شده\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ درون_داد from the files specified by\n\
         NUL-terminated نامها درون file F;\n\
         If F is -, اقرأ نامها from standard درون_داد\n\
"));
      oputs (_("\
  -L, --max-سطر-درازی\n\
         print the بیشینه display عرض\n\
"));
      oputs (_("\
  -w, --واژگان\n\
         print the کلمه_2 counts\n\
"));
      oputs (_("\
      --مجموع=WHEN\n\
         when مقصد print a سطر with مجموع counts;\n\
         WHEN can be: خودکار, always, only, never\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
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
write_counts (uintmax_t سطرها,
              uintmax_t واژگان,
              uintmax_t chars,
              uintmax_t بایتها,
              intmax_t linelength,
              حرف ثابت *file)
{
  ساكن حرف ثابت format_sp_int[] = " %*s";
  حرف ثابت *format_int = format_sp_int + 1;
  حرف buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  إذا (print_سطرها)
    {
      چاپ_کن (format_int, عدد_عرض, umaxtostr (سطرها, buf));
      format_int = format_sp_int;
    }
  إذا (print_واژگان)
    {
      چاپ_کن (format_int, عدد_عرض, umaxtostr (واژگان, buf));
      format_int = format_sp_int;
    }
  إذا (print_chars)
    {
      چاپ_کن (format_int, عدد_عرض, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  إذا (print_بایتها)
    {
      چاپ_کن (format_int, عدد_عرض, umaxtostr (بایتها, buf));
      format_int = format_sp_int;
    }
  إذا (print_linelength)
    چاپ_کن (format_int, عدد_عرض, imaxtostr (linelength, buf));
  إذا (file)
    چاپ_کن (" %s", strchr (file, '\n') ? quotef (file) : file);
  حرف_چاپ_کن ('\n');

  إذا (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
ساكن بنية wc_سطرها
wc_سطرها (صحيح fd)
{
#اگر_تعریف_شده USE_AVX512_WC_LINECOUNT
  ساكن علامتدار حرف use_avx512;
  إذا (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  إذا (0 < use_avx512)
    أرجع wc_سطرها_avx512 (fd);
#پایان_اگر
#اگر_تعریف_شده USE_AVX2_WC_LINECOUNT
  ساكن علامتدار حرف use_avx2;
  إذا (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  إذا (0 < use_avx2)
    أرجع wc_سطرها_avx2 (fd);
#پایان_اگر
#اگر_تعریف_شده USE_NEON_WC_LINECOUNT
  ساكن علامتدار حرف use_neon;
  إذا (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  إذا (0 < use_neon)
    أرجع wc_سطرها_neon (fd);
#پایان_اگر

  intmax_t سطرها = 0, بایتها = 0;
  bool long_سطرها = false;

  طالما (true)
    {
      حرف buf[IO_BUFSIZE + 1];
      ssize_t بایتها_read = اقرأ (fd, buf, IO_BUFSIZE);
      إذا (بایتها_read <= 0)
        أرجع (بنية wc_سطرها) { بایتها_read == 0 ? 0 : errno, سطرها, بایتها };

      بایتها += بایتها_read;
      حرف *انتها = buf + بایتها_read;
      idx_t buflines = 0;

      إذا (! long_سطرها)
        {
          /* Avoid function call overhead for shorter lines.  */
          لكل (حرف *p = buf; p < انتها; p++)
            buflines += *p == '\n';
        }
      وإلا
        {
          /* rawmemchr is more efficient with longer lines.  */
          *انتها = '\n';
          لكل (حرف *p = buf; (p = rawmemchr (p, '\n')) < انتها; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_سطرها = 15 * buflines <= بایتها_read;
      سطرها += buflines;
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
  intmax_t سطرها, واژگان, chars, بایتها, linelength;
  bool شمار_بایتها_2, شمار_chars, شمار_complicated;
  حرف ثابت *file = file_x ? file_x : _("standard input");

  سطرها = واژگان = chars = بایتها = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  إذا (MB_CUR_MAX > 1)
    {
      شمار_بایتها_2 = print_بایتها;
      شمار_chars = print_chars;
    }
  وإلا
    {
      شمار_بایتها_2 = print_بایتها || print_chars;
      شمار_chars = false;
    }
  شمار_complicated = print_واژگان || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  إذا (!شمار_بایتها_2 || شمار_chars || print_سطرها || شمار_complicated)
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

  إذا (شمار_بایتها_2 && !شمار_chars && !print_سطرها && !شمار_complicated)
    {
      bool skip_read = false;

      إذا (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      إذا (! fstatus->failed && usable_st_اندازه (&fstatus->st)
          && 0 <= fstatus->st.st_اندازه)
        {
          off_t انتها_pos = fstatus->st.st_اندازه;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          إذا (current_pos < 0)
            ;
          وإلا إذا (انتها_pos % صفحه_اندازه)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              إذا (current_pos < انتها_pos)
                {
                  بایتها = انتها_pos - current_pos;
                  skip_read = true;
                }
            }
          وإلا إذا (0 < انتها_pos)
            {
              off_t ep_1 = انتها_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              إذا (current_pos < hi_pos)
                بایتها = hi_pos - current_pos;
            }

          إذا (0 < بایتها && lseek (fd, بایتها, SEEK_CUR) < 0)
            {
              بایتها = 0;
              skip_read = false;
            }
        }

      إذا (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          لكل (ssize_t بایتها_read;
               (بایتها_read = اقرأ (fd, buf, IO_BUFSIZE));
               بایتها += بایتها_read)
            إذا (بایتها_read < 0)
              {
                err = errno;
                اكسر;
              }
        }
    }
  وإلا إذا (!شمار_chars && !شمار_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      بنية wc_سطرها w = wc_سطرها (fd);
      err = w.err;
      سطرها = w.سطرها;
      بایتها = w.بایتها;
    }
  وإلا إذا (MB_CUR_MAX > 1)
    {
      bool درون_کلمه = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool درون_لغزش = false;
      bool شمار_utf8 = !شمار_complicated && !print_سطرها
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      لكل (ssize_t بایتها_read;
           ((بایتها_read = اقرأ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          إذا (بایتها_read < 0)
            {
              err = errno;
              اكسر;
            }

          بایتها += بایتها_read;
          حرف ثابت *p = buf;
          حرف ثابت *plim = p + prev + بایتها_read;
          bool try_شمار_utf8 = شمار_utf8;
          افعل
            {
              إذا (try_شمار_utf8 && !درون_لغزش)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_شمار_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_شمار (p, &n);
                  p += n;
                  إذا (p == plim)
                    اكسر;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              إذا (!درون_لغزش && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              وإلا
                {
                  idx_t prev_بایتها = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  إذا (scanbytes < n)
                    {
                      إذا (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && بایتها_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          درون_لغزش = true;
                          اكسر;
                        }

                      /* Remember that we read a byte, but don't complain
                         about the error.  Because of the decoding error,
                         this is a considered to be byte but not a
                         character (that is, chars is not incremented).  */
                      p++;
                      mbszero (&state);
                      درون_لغزش = false;

                      /* Treat encoding errors as non white space.
                         POSIX says a word is "a non-zero-length string of
                         characters delimited by white space".  This is
                         wrong in some sense, as the string can be delimited
                         by start or end of input, and it is unclear what it
                         means when the input contains encoding errors.
                         Since encoding errors are not white space,
                         treat them that way here.  */
                      واژگان += !درون_کلمه;
                      درون_کلمه = true;
                      استمر;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_بایتها + n + !n;
                  single_byte = charbytes == !درون_لغزش;
                  درون_لغزش = !mbsinit (&state);
                }

              بدل (wide_char)
                {
                حالة '\n':
                  سطرها++;
                  FALLTHROUGH;
                حالة '\r':
                حالة '\f':
                  إذا (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  درون_کلمه = false;
                  اكسر;

                حالة '\t':
                  linepos += 8 - (linepos % 8);
                  درون_کلمه = false;
                  اكسر;

                حالة ' ':
                  linepos++;
                  FALLTHROUGH;
                حالة '\v':
                  درون_کلمه = false;
                  اكسر;

                افتراضي:;
                  bool درون_word2;
                  إذا (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      درون_word2 = !wc_isspace[wide_char];
                    }
                  وإلا
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      إذا (print_linelength)
                        {
                          صحيح عرض = c32width (wide_char);
                          إذا (عرض > 0)
                            linepos += عرض;
                        }
                      درون_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  واژگان += !درون_کلمه & درون_word2;
                  درون_کلمه = درون_word2;
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
      bool درون_کلمه = false;
      intmax_t linepos = 0;

      لكل (ssize_t بایتها_read; (بایتها_read = اقرأ (fd, buf, IO_BUFSIZE)); )
        {
          إذا (بایتها_read < 0)
            {
              err = errno;
              اكسر;
            }

          بایتها += بایتها_read;
          حرف ثابت *p = buf;
          افعل
            {
              بیعلامت حرف c = *p++;
              بدل (c)
                {
                حالة '\n':
                  سطرها++;
                  FALLTHROUGH;
                حالة '\r':
                حالة '\f':
                  إذا (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  درون_کلمه = false;
                  اكسر;

                حالة '\t':
                  linepos += 8 - (linepos % 8);
                  درون_کلمه = false;
                  اكسر;

                حالة ' ':
                  linepos++;
                  FALLTHROUGH;
                حالة '\v':
                  درون_کلمه = false;
                  اكسر;

                افتراضي:
                  linepos += wc_isprint[c];
                  bool درون_word2 = !wc_isspace[c];
                  واژگان += !درون_کلمه & درون_word2;
                  درون_کلمه = درون_word2;
                  اكسر;
                }
            }
          طالما (--بایتها_read);
        }
      إذا (linepos > linelength)
        linelength = linepos;
    }

  إذا (شمار_chars < print_chars)
    chars = بایتها;

  إذا (مجموع_نما != مجموع_only)
    write_counts (سطرها, واژگان, chars, بایتها, linelength, file_x);

  مجموع_سطرها_سرریز |= ckd_جمع_کن (&مجموع_سطرها, مجموع_سطرها, سطرها);
  مجموع_واژگان_سرریز |= ckd_جمع_کن (&مجموع_واژگان, مجموع_واژگان, واژگان);
  مجموع_chars_سرریز |= ckd_جمع_کن (&مجموع_chars, مجموع_chars, chars);
  مجموع_بایتها_سرریز |= ckd_جمع_کن (&مجموع_بایتها, مجموع_بایتها, بایتها);

  إذا (linelength > max_سطر_درازی)
    max_سطر_درازی = linelength;

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
      xset_دودویی_نما (STDIN_FILENO, O_دودویی);
      أرجع wc (STDIN_FILENO, file, fstatus);
    }
  وإلا
    {
      صحيح fd = open (file, O_RDONLY | O_دودویی);
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
get_درون_داد_fstatus (idx_t nfiles, حرف *ثابت *file)
{
  بنية fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, حجم *fstatus);

  إذا (nfiles == 0
      || (nfiles == 1
          && ((print_سطرها + print_واژگان + print_chars
               + print_بایتها + print_linelength)
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
compute_عدد_عرض (idx_t nfiles, بنية fstatus ثابت *fstatus)
{
  صحيح عرض = 1;

  إذا (0 < nfiles && fstatus[0].failed <= 0)
    {
      صحيح minimum_عرض = 1;
      uintmax_t regular_مجموع = 0;

      لكل (idx_t i = 0; i < nfiles; i++)
        إذا (! fstatus[i].failed)
          {
            إذا (!S_ISREG (fstatus[i].st.st_نما))
              minimum_عرض = 7;
            وإلا إذا (ckd_جمع_کن (&regular_مجموع, regular_مجموع,
                              fstatus[i].st.st_اندازه))
              {
                regular_مجموع = UINTMAX_MAX;
                اكسر;
              }
          }

      لكل (; 10 <= regular_مجموع; regular_مجموع /= 10)
        عرض++;
      إذا (عرض < minimum_عرض)
        عرض = minimum_عرض;
    }

  أرجع عرض;
}


صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح optc;
  idx_t nfiles;
  حرف **files;
  حرف *files_from = عدم;
  بنية fstatus *fstatus;
  بنية نشانهها tok;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صفحه_اندازه = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, عدم, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);

  طالما ((optc = getopt_long (argc, argv, "clLmw", longopts, عدم)) != -1)
    بدل (optc)
      {
      حالة 'c':
        print_بایتها = true;
        اكسر;

      حالة 'm':
        print_chars = true;
        اكسر;

      حالة 'l':
        print_سطرها = true;
        اكسر;

      حالة 'w':
        print_واژگان = true;
        اكسر;

      حالة 'L':
        print_linelength = true;
        اكسر;

      حالة DEBUG_برنامه_OPTION:
        debug = true;
        اكسر;

      حالة FILES0_FROM_OPTION:
        files_from = optarg;
        اكسر;

      حالة مجموع_OPTION:
        مجموع_نما = XARGMATCH ("--total", optarg, مجموع_آرگومانها, مجموع_types);
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

      افتراضي:
        usage (EXIT_FAILURE);
      }

  إذا (! (print_سطرها || print_واژگان || print_chars || print_بایتها
         || print_linelength))
    print_سطرها = print_واژگان = print_بایتها = true;

  إذا (print_linelength)
    لكل (صحيح i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  إذا (print_واژگان)
    لكل (صحيح i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = آیا_فاصله (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_نشانهها = false;
  بنية argv_iterator *ai;
  إذا (files_from)
    {
      FILE *جریان;

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
        جریان = stdin;
      وإلا
        {
          جریان = fopen (files_from, "r");
          إذا (جریان == عدم)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      بنية stat st;
      إذا (fstat (fileno (جریان), &st) == 0
          && S_ISREG (st.st_نما)
          && st.st_اندازه <= MIN (10 * 1024 * 1024, physmem_موجود () / 2))
        {
          read_نشانهها = true;
          readtokens0_init (&tok);
          إذا (! readtokens0 (جریان, &tok) || fclose (جریان) != 0)
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
          ai = argv_iter_init_جریان (جریان);
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

  fstatus = get_درون_داد_fstatus (nfiles, files);
  إذا (مجموع_نما == مجموع_only)
    عدد_عرض = 1;  /* No extra padding, since no alignment requirement.  */
  وإلا
    عدد_عرض = compute_عدد_عرض (nfiles, fstatus);

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
                     argv_iter_n_آرگومانها (ai), _("invalid zero-length file name"));
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
  إذا (ok && !files_from && argv_iter_n_آرگومانها (ai) == 0)
    ok &= wc_file (عدم, &fstatus[0]);

  إذا (read_نشانهها)
    readtokens0_free (&tok);

  إذا (مجموع_نما != مجموع_never
      && (مجموع_نما != مجموع_auto || 1 < argv_iter_n_آرگومانها (ai)))
    {
      إذا (مجموع_سطرها_سرریز)
        {
          مجموع_سطرها = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      إذا (مجموع_واژگان_سرریز)
        {
          مجموع_واژگان = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      إذا (مجموع_chars_سرریز)
        {
          مجموع_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      إذا (مجموع_بایتها_سرریز)
        {
          مجموع_بایتها = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (مجموع_سطرها, مجموع_واژگان, مجموع_chars, مجموع_بایتها,
                    max_سطر_درازی,
                    مجموع_نما != مجموع_only ? _("total") : عدم);
    }

  argv_iter_free (ai);

  حرر (fstatus);

  إذا (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
