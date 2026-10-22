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

#اگر_تعریف_شدہ USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#اگر_ختم

#شامل "system.h"
#شامل "assure.h"
#شامل "cpu-supports.h"
#شامل "ioblksize.h"
#شامل "wc.h"
#شامل "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "wc"

#تعریف_کرو AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

ساکن bool wc_isprint[UCHAR_MAX + 1];
ساکن bool wc_isspace[UCHAR_MAX + 1];

ساکن bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
ساکن uintmax_t کل_سطریں;
ساکن uintmax_t کل_الفاظ;
ساکن uintmax_t کل_chars;
ساکن uintmax_t کل_بائٹیں;
ساکن bool کل_سطریں_حد_سے_تجاوز;
ساکن bool کل_الفاظ_حد_سے_تجاوز;
ساکن bool کل_chars_حد_سے_تجاوز;
ساکن bool کل_بائٹیں_حد_سے_تجاوز;
ساکن intmax_t max_سطر_لمبائی;

/* Which counts to print. */
ساکن bool print_سطریں, print_الفاظ, print_chars, print_بائٹیں;
ساکن bool print_linelength;

/* The print width of each count.  */
ساکن صحیح نمبر_چوڑائی;

/* True if we have ever read the standard input. */
ساکن bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
ساکن idx_t صفحہ_سائز;

/* Enable to _not_ treat non breaking space as a word separator.  */
ساکن bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
ساخت fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  صحیح failed;

  /* If FAILED is zero, this is the file's status.  */
  ساخت stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  DEBUG_پروگرام_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  کل_OPTION,
};

ساکن ساخت option ثابت longopts[] =
{
  {"bytes", no_دلیل, تہی, 'c'},
  {"chars", no_دلیل, تہی, 'm'},
  {"lines", no_دلیل, تہی, 'l'},
  {"words", no_دلیل, تہی, 'w'},
  {"debug", no_دلیل, تہی, DEBUG_پروگرام_OPTION},
  {"files0-from", required_دلیل, تہی, FILES0_FROM_OPTION},
  {"max-line-length", no_دلیل, تہی, 'L'},
  {"total", required_دلیل, تہی, کل_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

شمار کل_type
  {
    کل_auto,         /* 0: default or --total=auto */
    کل_always,       /* 1: --total=always */
    کل_only,         /* 2: --total=only */
    کل_never         /* 3: --total=never */
  };
ساکن حرف ثابت *ثابت کل_دلائل[] =
{
  "auto", "always", "only", "never", تہی
};
ساکن شمار کل_type ثابت کل_types[] =
{
  کل_auto, کل_always, کل_only, کل_never
};
ARGMATCH_VERIFY (کل_دلائل, کل_types);
ساکن شمار کل_type کل_منوال = کل_auto;

#اگر_تعریف_شدہ USE_AVX2_WC_LINECOUNT
ساکن bool
avx2_supported (خالی)
{
  bool avx2_enabled = cpu_supports ("avx2");
  اگر (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  واپس avx2_enabled;
}
#اگر_ختم

#اگر_تعریف_شدہ USE_AVX512_WC_LINECOUNT
ساکن bool
avx512_supported (خالی)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  اگر (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  واپس avx512_enabled;
}
#اگر_ختم

#اگر_تعریف_شدہ USE_NEON_WC_LINECOUNT
ساکن bool
neon_supported (خالی)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  اگر (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  واپس neon_enabled;
}
#اگر_ختم

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Print newline, لفظ_2, and byte counts برائے each FILE, and a کل سطر اگر\n\
مزید than one FILE is specified.  A لفظ_2 is a nonempty sequence کا non white\n\
space delimited by white space characters or by آغاز or انتہا کا داخلہ.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options نیچے may be مستعمل مقصد select which counts are printed, always اندر\n\
the following ترتیب: newline, لفظ_2, character, byte, زیادہ_سے_زیادہ سطر لمبائی.\n\
"), stdout);
      oputs (_("\
  -c, --بائٹیں\n\
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
         indicate what سطر گنتی acceleration is مستعمل\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ داخلہ from the files specified by\n\
         NUL-terminated نام اندر file F;\n\
         If F is -, اقرأ نام from standard داخلہ\n\
"));
      oputs (_("\
  -L, --max-سطر-لمبائی\n\
         print the زیادہ_سے_زیادہ display چوڑائی\n\
"));
      oputs (_("\
  -w, --الفاظ\n\
         print the لفظ_2 counts\n\
"));
      oputs (_("\
      --کل=WHEN\n\
         when مقصد print a سطر with کل counts;\n\
         WHEN can be: خودکار, always, only, never\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Return non zero if POSIXLY_CORRECT is not set and WC is a non breaking
   space.  */
ATTRIBUTE_خالص
ساکن صحیح
maybe_c32isnbspace (char32_t wc)
{
  واپس ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
ساکن خالی
write_counts (uintmax_t سطریں,
              uintmax_t الفاظ,
              uintmax_t chars,
              uintmax_t بائٹیں,
              intmax_t linelength,
              حرف ثابت *file)
{
  ساکن حرف ثابت format_sp_int[] = " %*s";
  حرف ثابت *format_int = format_sp_int + 1;
  حرف buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  اگر (print_سطریں)
    {
      لکھو (format_int, نمبر_چوڑائی, umaxtostr (سطریں, buf));
      format_int = format_sp_int;
    }
  اگر (print_الفاظ)
    {
      لکھو (format_int, نمبر_چوڑائی, umaxtostr (الفاظ, buf));
      format_int = format_sp_int;
    }
  اگر (print_chars)
    {
      لکھو (format_int, نمبر_چوڑائی, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  اگر (print_بائٹیں)
    {
      لکھو (format_int, نمبر_چوڑائی, umaxtostr (بائٹیں, buf));
      format_int = format_sp_int;
    }
  اگر (print_linelength)
    لکھو (format_int, نمبر_چوڑائی, imaxtostr (linelength, buf));
  اگر (file)
    لکھو (" %s", strchr (file, '\n') ? quotef (file) : file);
  حرف_لکھو ('\n');

  اگر (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
ساکن ساخت wc_سطریں
wc_سطریں (صحیح fd)
{
#اگر_تعریف_شدہ USE_AVX512_WC_LINECOUNT
  ساکن علامتی حرف use_avx512;
  اگر (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  اگر (0 < use_avx512)
    واپس wc_سطریں_avx512 (fd);
#اگر_ختم
#اگر_تعریف_شدہ USE_AVX2_WC_LINECOUNT
  ساکن علامتی حرف use_avx2;
  اگر (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  اگر (0 < use_avx2)
    واپس wc_سطریں_avx2 (fd);
#اگر_ختم
#اگر_تعریف_شدہ USE_NEON_WC_LINECOUNT
  ساکن علامتی حرف use_neon;
  اگر (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  اگر (0 < use_neon)
    واپس wc_سطریں_neon (fd);
#اگر_ختم

  intmax_t سطریں = 0, بائٹیں = 0;
  bool long_سطریں = false;

  جب (true)
    {
      حرف buf[IO_BUFSIZE + 1];
      ssize_t بائٹیں_read = اقرأ (fd, buf, IO_BUFSIZE);
      اگر (بائٹیں_read <= 0)
        واپس (ساخت wc_سطریں) { بائٹیں_read == 0 ? 0 : errno, سطریں, بائٹیں };

      بائٹیں += بائٹیں_read;
      حرف *انتہا = buf + بائٹیں_read;
      idx_t buflines = 0;

      اگر (! long_سطریں)
        {
          /* Avoid function call overhead for shorter lines.  */
          برائے (حرف *p = buf; p < انتہا; p++)
            buflines += *p == '\n';
        }
      ورنہ
        {
          /* rawmemchr is more efficient with longer lines.  */
          *انتہا = '\n';
          برائے (حرف *p = buf; (p = rawmemchr (p, '\n')) < انتہا; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_سطریں = 15 * buflines <= بائٹیں_read;
      سطریں += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
ساکن bool
wc (صحیح fd, حرف ثابت *file_x, ساخت fstatus *fstatus)
{
  صحیح err = 0;
  حرف buf[IO_BUFSIZE + 1];
  intmax_t سطریں, الفاظ, chars, بائٹیں, linelength;
  bool گنتی_بائٹیں_2, گنتی_chars, گنتی_complicated;
  حرف ثابت *file = file_x ? file_x : _("standard input");

  سطریں = الفاظ = chars = بائٹیں = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  اگر (MB_CUR_MAX > 1)
    {
      گنتی_بائٹیں_2 = print_بائٹیں;
      گنتی_chars = print_chars;
    }
  ورنہ
    {
      گنتی_بائٹیں_2 = print_بائٹیں || print_chars;
      گنتی_chars = false;
    }
  گنتی_complicated = print_الفاظ || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  اگر (!گنتی_بائٹیں_2 || گنتی_chars || print_سطریں || گنتی_complicated)
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

  اگر (گنتی_بائٹیں_2 && !گنتی_chars && !print_سطریں && !گنتی_complicated)
    {
      bool skip_read = false;

      اگر (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      اگر (! fstatus->failed && usable_st_سائز (&fstatus->st)
          && 0 <= fstatus->st.st_سائز)
        {
          off_t انتہا_pos = fstatus->st.st_سائز;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          اگر (current_pos < 0)
            ;
          ورنہ اگر (انتہا_pos % صفحہ_سائز)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              اگر (current_pos < انتہا_pos)
                {
                  بائٹیں = انتہا_pos - current_pos;
                  skip_read = true;
                }
            }
          ورنہ اگر (0 < انتہا_pos)
            {
              off_t ep_1 = انتہا_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              اگر (current_pos < hi_pos)
                بائٹیں = hi_pos - current_pos;
            }

          اگر (0 < بائٹیں && lseek (fd, بائٹیں, SEEK_CUR) < 0)
            {
              بائٹیں = 0;
              skip_read = false;
            }
        }

      اگر (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          برائے (ssize_t بائٹیں_read;
               (بائٹیں_read = اقرأ (fd, buf, IO_BUFSIZE));
               بائٹیں += بائٹیں_read)
            اگر (بائٹیں_read < 0)
              {
                err = errno;
                توڑو;
              }
        }
    }
  ورنہ اگر (!گنتی_chars && !گنتی_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      ساخت wc_سطریں w = wc_سطریں (fd);
      err = w.err;
      سطریں = w.سطریں;
      بائٹیں = w.بائٹیں;
    }
  ورنہ اگر (MB_CUR_MAX > 1)
    {
      bool اندر_لفظ = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool اندر_سرکاؤ = false;
      bool گنتی_utf8 = !گنتی_complicated && !print_سطریں
                        && is_utf8_charset ();
      idx_t پچھلا = 0; /* Number of bytes carried over from previous round.  */

      برائے (ssize_t بائٹیں_read;
           ((بائٹیں_read = اقرأ (fd, buf + پچھلا, IO_BUFSIZE - پچھلا))
            || پچھلا);
           )
        {
          اگر (بائٹیں_read < 0)
            {
              err = errno;
              توڑو;
            }

          بائٹیں += بائٹیں_read;
          حرف ثابت *p = buf;
          حرف ثابت *plim = p + پچھلا + بائٹیں_read;
          bool try_گنتی_utf8 = گنتی_utf8;
          کرو
            {
              اگر (try_گنتی_utf8 && !اندر_سرکاؤ)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_گنتی_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_گنتی (p, &n);
                  p += n;
                  اگر (p == plim)
                    توڑو;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              اگر (!اندر_سرکاؤ && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              ورنہ
                {
                  idx_t پچھلا_بائٹیں = پچھلا;
                  idx_t scanbytes = plim - (p + پچھلا);
                  size_t n = mbrtoc32 (&wide_char, p + پچھلا, scanbytes, &state);
                  پچھلا = 0;

                  اگر (scanbytes < n)
                    {
                      اگر (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && بائٹیں_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          پچھلا = plim - p;
                          memmove (buf, p, پچھلا);
                          اندر_سرکاؤ = true;
                          توڑو;
                        }

                      /* Remember that we read a byte, but don't complain
                         about the error.  Because of the decoding error,
                         this is a considered to be byte but not a
                         character (that is, chars is not incremented).  */
                      p++;
                      mbszero (&state);
                      اندر_سرکاؤ = false;

                      /* Treat encoding errors as non white space.
                         POSIX says a word is "a non-zero-length string of
                         characters delimited by white space".  This is
                         wrong in some sense, as the string can be delimited
                         by start or end of input, and it is unclear what it
                         means when the input contains encoding errors.
                         Since encoding errors are not white space,
                         treat them that way here.  */
                      الفاظ += !اندر_لفظ;
                      اندر_لفظ = true;
                      جاری;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = پچھلا_بائٹیں + n + !n;
                  single_byte = charbytes == !اندر_سرکاؤ;
                  اندر_سرکاؤ = !mbsinit (&state);
                }

              انتخاب (wide_char)
                {
                حالت '\n':
                  سطریں++;
                  FALLTHROUGH;
                حالت '\r':
                حالت '\f':
                  اگر (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  اندر_لفظ = false;
                  توڑو;

                حالت '\t':
                  linepos += 8 - (linepos % 8);
                  اندر_لفظ = false;
                  توڑو;

                حالت ' ':
                  linepos++;
                  FALLTHROUGH;
                حالت '\v':
                  اندر_لفظ = false;
                  توڑو;

                طےشدہ:;
                  bool اندر_word2;
                  اگر (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      اندر_word2 = !wc_isspace[wide_char];
                    }
                  ورنہ
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      اگر (print_linelength)
                        {
                          صحیح چوڑائی = c32width (wide_char);
                          اگر (چوڑائی > 0)
                            linepos += چوڑائی;
                        }
                      اندر_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  الفاظ += !اندر_لفظ & اندر_word2;
                  اندر_لفظ = اندر_word2;
                  توڑو;
                }

              p += charbytes;
              chars++;
            }
          جب (p < plim);
        }
      اگر (linepos > linelength)
        linelength = linepos;
    }
  ورنہ
    {
      bool اندر_لفظ = false;
      intmax_t linepos = 0;

      برائے (ssize_t بائٹیں_read; (بائٹیں_read = اقرأ (fd, buf, IO_BUFSIZE)); )
        {
          اگر (بائٹیں_read < 0)
            {
              err = errno;
              توڑو;
            }

          بائٹیں += بائٹیں_read;
          حرف ثابت *p = buf;
          کرو
            {
              غیرعلامتی حرف c = *p++;
              انتخاب (c)
                {
                حالت '\n':
                  سطریں++;
                  FALLTHROUGH;
                حالت '\r':
                حالت '\f':
                  اگر (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  اندر_لفظ = false;
                  توڑو;

                حالت '\t':
                  linepos += 8 - (linepos % 8);
                  اندر_لفظ = false;
                  توڑو;

                حالت ' ':
                  linepos++;
                  FALLTHROUGH;
                حالت '\v':
                  اندر_لفظ = false;
                  توڑو;

                طےشدہ:
                  linepos += wc_isprint[c];
                  bool اندر_word2 = !wc_isspace[c];
                  الفاظ += !اندر_لفظ & اندر_word2;
                  اندر_لفظ = اندر_word2;
                  توڑو;
                }
            }
          جب (--بائٹیں_read);
        }
      اگر (linepos > linelength)
        linelength = linepos;
    }

  اگر (گنتی_chars < print_chars)
    chars = بائٹیں;

  اگر (کل_منوال != کل_only)
    write_counts (سطریں, الفاظ, chars, بائٹیں, linelength, file_x);

  کل_سطریں_حد_سے_تجاوز |= ckd_جوڑو (&کل_سطریں, کل_سطریں, سطریں);
  کل_الفاظ_حد_سے_تجاوز |= ckd_جوڑو (&کل_الفاظ, کل_الفاظ, الفاظ);
  کل_chars_حد_سے_تجاوز |= ckd_جوڑو (&کل_chars, کل_chars, chars);
  کل_بائٹیں_حد_سے_تجاوز |= ckd_جوڑو (&کل_بائٹیں, کل_بائٹیں, بائٹیں);

  اگر (linelength > max_سطر_لمبائی)
    max_سطر_لمبائی = linelength;

  اگر (err)
    error (0, err, "%s", quotef (file));
  واپس !err;
}

ساکن bool
wc_file (حرف ثابت *file, ساخت fstatus *fstatus)
{
  اگر (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
      واپس wc (STDIN_FILENO, file, fstatus);
    }
  ورنہ
    {
      صحیح fd = open (file, O_RDONLY | O_ثنائی);
      اگر (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          واپس false;
        }
      ورنہ
        {
          bool ok = wc (fd, file, fstatus);
          اگر (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              واپس false;
            }
          واپس ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

ساکن ساخت fstatus *
get_داخلہ_fstatus (idx_t nfiles, حرف *ثابت *file)
{
  ساخت fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, حجم *fstatus);

  اگر (nfiles == 0
      || (nfiles == 1
          && ((print_سطریں + print_الفاظ + print_chars
               + print_بائٹیں + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  ورنہ
    {
      برائے (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  واپس fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_خالص
ساکن صحیح
compute_نمبر_چوڑائی (idx_t nfiles, ساخت fstatus ثابت *fstatus)
{
  صحیح چوڑائی = 1;

  اگر (0 < nfiles && fstatus[0].failed <= 0)
    {
      صحیح minimum_چوڑائی = 1;
      uintmax_t regular_کل = 0;

      برائے (idx_t i = 0; i < nfiles; i++)
        اگر (! fstatus[i].failed)
          {
            اگر (!S_ISREG (fstatus[i].st.st_منوال))
              minimum_چوڑائی = 7;
            ورنہ اگر (ckd_جوڑو (&regular_کل, regular_کل,
                              fstatus[i].st.st_سائز))
              {
                regular_کل = UINTMAX_MAX;
                توڑو;
              }
          }

      برائے (; 10 <= regular_کل; regular_کل /= 10)
        چوڑائی++;
      اگر (چوڑائی < minimum_چوڑائی)
        چوڑائی = minimum_چوڑائی;
    }

  واپس چوڑائی;
}


صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optc;
  idx_t nfiles;
  حرف **files;
  حرف *files_from = تہی;
  ساخت fstatus *fstatus;
  ساخت علامات tok;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صفحہ_سائز = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, تہی, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != تہی);

  جب ((optc = getopt_long (argc, argv, "clLmw", longopts, تہی)) != -1)
    انتخاب (optc)
      {
      حالت 'c':
        print_بائٹیں = true;
        توڑو;

      حالت 'm':
        print_chars = true;
        توڑو;

      حالت 'l':
        print_سطریں = true;
        توڑو;

      حالت 'w':
        print_الفاظ = true;
        توڑو;

      حالت 'L':
        print_linelength = true;
        توڑو;

      حالت DEBUG_پروگرام_OPTION:
        debug = true;
        توڑو;

      حالت FILES0_FROM_OPTION:
        files_from = optarg;
        توڑو;

      حالت کل_OPTION:
        کل_منوال = XARGMATCH ("--total", optarg, کل_دلائل, کل_types);
        توڑو;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

      طےشدہ:
        usage (EXIT_FAILURE);
      }

  اگر (! (print_سطریں || print_الفاظ || print_chars || print_بائٹیں
         || print_linelength))
    print_سطریں = print_الفاظ = print_بائٹیں = true;

  اگر (print_linelength)
    برائے (صحیح i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  اگر (print_الفاظ)
    برائے (صحیح i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = خلا_ہے (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_علامات = false;
  ساخت argv_iterator *ai;
  اگر (files_from)
    {
      FILE *دھارا;

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
        دھارا = stdin;
      ورنہ
        {
          دھارا = fopen (files_from, "r");
          اگر (دھارا == تہی)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      ساخت stat st;
      اگر (fstat (fileno (دھارا), &st) == 0
          && S_ISREG (st.st_منوال)
          && st.st_سائز <= MIN (10 * 1024 * 1024, physmem_دستیاب () / 2))
        {
          read_علامات = true;
          readtokens0_init (&tok);
          اگر (! readtokens0 (دھارا, &tok) || fclose (دھارا) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      ورنہ
        {
          files = تہی;
          nfiles = 0;
          ai = argv_iter_init_دھارا (دھارا);
        }
    }
  ورنہ
    {
      ساکن حرف *stdin_only[] = { تہی };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  اگر (!ai)
    xalloc_die ();

  fstatus = get_داخلہ_fstatus (nfiles, files);
  اگر (کل_منوال == کل_only)
    نمبر_چوڑائی = 1;  /* No extra padding, since no alignment requirement.  */
  ورنہ
    نمبر_چوڑائی = compute_نمبر_چوڑائی (nfiles, fstatus);

  bool ok = true;
  شمار argv_iter_err ai_err;
  حرف *file_name;
  برائے (صحیح i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
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
          اگر (files_from == تہی)
            error (0, 0, "%s", _("invalid zero-length file name"));
          ورنہ
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_دلائل (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      اگر (skip_file)
        ok = false;
      ورنہ
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      اگر (! nfiles)
        fstatus[0].failed = 1;
    }
  انتخاب (ai_err)
    {
    حالت AI_ERR_EOF:
      توڑو;

    حالت AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      توڑو;

    حالت AI_ERR_MEM:
      xalloc_die ();

    حالت AI_ERR_OK: طےشدہ:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  اگر (ok && !files_from && argv_iter_n_دلائل (ai) == 0)
    ok &= wc_file (تہی, &fstatus[0]);

  اگر (read_علامات)
    readtokens0_free (&tok);

  اگر (کل_منوال != کل_never
      && (کل_منوال != کل_auto || 1 < argv_iter_n_دلائل (ai)))
    {
      اگر (کل_سطریں_حد_سے_تجاوز)
        {
          کل_سطریں = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      اگر (کل_الفاظ_حد_سے_تجاوز)
        {
          کل_الفاظ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      اگر (کل_chars_حد_سے_تجاوز)
        {
          کل_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      اگر (کل_بائٹیں_حد_سے_تجاوز)
        {
          کل_بائٹیں = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (کل_سطریں, کل_الفاظ, کل_chars, کل_بائٹیں,
                    max_سطر_لمبائی,
                    کل_منوال != کل_only ? _("total") : تہی);
    }

  argv_iter_free (ai);

  آزاد (fstatus);

  اگر (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
