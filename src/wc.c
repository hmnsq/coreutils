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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव <argmatch.h>
#अन्तर्भाव <argv-iter.h>
#अन्तर्भाव <fadvise.h>
#अन्तर्भाव <physmem.h>
#अन्तर्भाव <readtokens0.h>
#अन्तर्भाव <stat-size.h>
#अन्तर्भाव <xbinary-io.h>

#यदि_परिभाषितम् USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#यदि_अन्तः

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "cpu-supports.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "wc.h"
#अन्तर्भाव "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "wc"

#परिभाषय AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

स्थिर bool wc_isprint[UCHAR_MAX + 1];
स्थिर bool wc_isspace[UCHAR_MAX + 1];

स्थिर bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
स्थिर uintmax_t योग_पङ्क्तयः;
स्थिर uintmax_t योग_शब्दाः;
स्थिर uintmax_t योग_chars;
स्थिर uintmax_t योग_अष्टकानि;
स्थिर bool योग_पङ्क्तयः_अतिप्रवाहः;
स्थिर bool योग_शब्दाः_अतिप्रवाहः;
स्थिर bool योग_chars_अतिप्रवाहः;
स्थिर bool योग_अष्टकानि_अतिप्रवाहः;
स्थिर intmax_t max_पङ्क्तिः_दीर्घता;

/* Which counts to print. */
स्थिर bool print_पङ्क्तयः, print_शब्दाः, print_chars, print_अष्टकानि;
स्थिर bool print_linelength;

/* The print width of each count.  */
स्थिर पूर्णाङ्क सङ्ख्या_विस्तारः;

/* True if we have ever read the standard input. */
स्थिर bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
स्थिर idx_t page_आकार;

/* Enable to _not_ treat non breaking space as a word separator.  */
स्थिर bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
संरचना fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  पूर्णाङ्क failed;

  /* If FAILED is zero, this is the file's status.  */
  संरचना stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEBUG_क्रमादेशः_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  योग_OPTION,
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"bytes", no_तर्कः, रिक्त, 'c'},
  {"chars", no_तर्कः, रिक्त, 'm'},
  {"lines", no_तर्कः, रिक्त, 'l'},
  {"words", no_तर्कः, रिक्त, 'w'},
  {"debug", no_तर्कः, रिक्त, DEBUG_क्रमादेशः_OPTION},
  {"files0-from", required_तर्कः, रिक्त, FILES0_FROM_OPTION},
  {"max-line-length", no_तर्कः, रिक्त, 'L'},
  {"total", required_तर्कः, रिक्त, योग_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

गणना योग_type
  {
    योग_auto,         /* 0: default or --total=auto */
    योग_always,       /* 1: --total=always */
    योग_only,         /* 2: --total=only */
    योग_never         /* 3: --total=never */
  };
स्थिर वर्ण ध्रुव *ध्रुव योग_तर्काः[] =
{
  "auto", "always", "only", "never", रिक्त
};
स्थिर गणना योग_type ध्रुव योग_types[] =
{
  योग_auto, योग_always, योग_only, योग_never
};
ARGMATCH_VERIFY (योग_तर्काः, योग_types);
स्थिर गणना योग_type योग_बहुलकम् = योग_auto;

#यदि_परिभाषितम् USE_AVX2_WC_LINECOUNT
स्थिर bool
avx2_supported (शून्य)
{
  bool avx2_enabled = cpu_supports ("avx2");
  यदि (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  प्रत्यागम avx2_enabled;
}
#यदि_अन्तः

#यदि_परिभाषितम् USE_AVX512_WC_LINECOUNT
स्थिर bool
avx512_supported (शून्य)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  यदि (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  प्रत्यागम avx512_enabled;
}
#यदि_अन्तः

#यदि_परिभाषितम् USE_NEON_WC_LINECOUNT
स्थिर bool
neon_supported (शून्य)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  यदि (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  प्रत्यागम neon_enabled;
}
#यदि_अन्तः

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
Print newline, word, and byte counts प्रति each FILE, and a योग_2 पङ्क्तिः यदि\n\
अधिकम् than one FILE is specified.  A word is a nonempty sequence सम्बन्धि non white\n\
space delimited by white space characters or by start or अन्तः सम्बन्धि निवेशः.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options अधः may be प्रयुक्तम् गन्तव्यम् select which counts are printed, always अन्तर्\n\
the following क्रमः: newline, word, character, byte, अधिकतमम् पङ्क्तिः दीर्घता.\n\
"), stdout);
      oputs (_("\
  -c, --अष्टकानि\n\
         print the byte counts\n\
"));
      oputs (_("\
  -m, --chars\n\
         print the character counts\n\
"));
      oputs (_("\
  -l, --पङ्क्तयः\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what पङ्क्तिः गणनम् acceleration is प्रयुक्तम्\n\
"));
      oputs (_("\
      --files0-from=F\n\
         पठ निवेशः from the files specified by\n\
         NUL-terminated नामानि अन्तर् file F;\n\
         If F is -, पठ नामानि from standard निवेशः\n\
"));
      oputs (_("\
  -L, --max-पङ्क्तिः-दीर्घता\n\
         print the अधिकतमम् display विस्तारः\n\
"));
      oputs (_("\
  -w, --शब्दाः\n\
         print the word counts\n\
"));
      oputs (_("\
      --योग_2=WHEN\n\
         when गन्तव्यम् print a पङ्क्तिः with योग_2 counts;\n\
         WHEN can be: स्वचालित, always, only, never\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Return non zero if POSIXLY_CORRECT is not set and WC is a non breaking
   space.  */
ATTRIBUTE_विशुद्धम्
स्थिर पूर्णाङ्क
maybe_c32isnbspace (char32_t wc)
{
  प्रत्यागम ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
स्थिर शून्य
write_counts (uintmax_t पङ्क्तयः,
              uintmax_t शब्दाः,
              uintmax_t chars,
              uintmax_t अष्टकानि,
              intmax_t linelength,
              वर्ण ध्रुव *file)
{
  स्थिर वर्ण ध्रुव format_sp_int[] = " %*s";
  वर्ण ध्रुव *format_int = format_sp_int + 1;
  वर्ण buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  यदि (print_पङ्क्तयः)
    {
      प्रारूप_लिख (format_int, सङ्ख्या_विस्तारः, umaxtostr (पङ्क्तयः, buf));
      format_int = format_sp_int;
    }
  यदि (print_शब्दाः)
    {
      प्रारूप_लिख (format_int, सङ्ख्या_विस्तारः, umaxtostr (शब्दाः, buf));
      format_int = format_sp_int;
    }
  यदि (print_chars)
    {
      प्रारूप_लिख (format_int, सङ्ख्या_विस्तारः, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  यदि (print_अष्टकानि)
    {
      प्रारूप_लिख (format_int, सङ्ख्या_विस्तारः, umaxtostr (अष्टकानि, buf));
      format_int = format_sp_int;
    }
  यदि (print_linelength)
    प्रारूप_लिख (format_int, सङ्ख्या_विस्तारः, imaxtostr (linelength, buf));
  यदि (file)
    प्रारूप_लिख (" %s", strchr (file, '\n') ? quotef (file) : file);
  अक्षरं_लिख ('\n');

  यदि (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
स्थिर संरचना wc_पङ्क्तयः
wc_पङ्क्तयः (पूर्णाङ्क fd)
{
#यदि_परिभाषितम् USE_AVX512_WC_LINECOUNT
  स्थिर चिह्नित वर्ण use_avx512;
  यदि (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  यदि (0 < use_avx512)
    प्रत्यागम wc_पङ्क्तयः_avx512 (fd);
#यदि_अन्तः
#यदि_परिभाषितम् USE_AVX2_WC_LINECOUNT
  स्थिर चिह्नित वर्ण use_avx2;
  यदि (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  यदि (0 < use_avx2)
    प्रत्यागम wc_पङ्क्तयः_avx2 (fd);
#यदि_अन्तः
#यदि_परिभाषितम् USE_NEON_WC_LINECOUNT
  स्थिर चिह्नित वर्ण use_neon;
  यदि (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  यदि (0 < use_neon)
    प्रत्यागम wc_पङ्क्तयः_neon (fd);
#यदि_अन्तः

  intmax_t पङ्क्तयः = 0, अष्टकानि = 0;
  bool long_पङ्क्तयः = false;

  यावत् (true)
    {
      वर्ण buf[IO_BUFSIZE + 1];
      ssize_t अष्टकानि_read = पठ (fd, buf, IO_BUFSIZE);
      यदि (अष्टकानि_read <= 0)
        प्रत्यागम (संरचना wc_पङ्क्तयः) { अष्टकानि_read == 0 ? 0 : errno, पङ्क्तयः, अष्टकानि };

      अष्टकानि += अष्टकानि_read;
      वर्ण *अन्तः = buf + अष्टकानि_read;
      idx_t buflines = 0;

      यदि (! long_पङ्क्तयः)
        {
          /* Avoid function call overhead for shorter lines.  */
          प्रति (वर्ण *p = buf; p < अन्तः; p++)
            buflines += *p == '\n';
        }
      अन्यथा
        {
          /* rawmemchr is more efficient with longer lines.  */
          *अन्तः = '\n';
          प्रति (वर्ण *p = buf; (p = rawmemchr (p, '\n')) < अन्तः; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_पङ्क्तयः = 15 * buflines <= अष्टकानि_read;
      पङ्क्तयः += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
स्थिर bool
wc (पूर्णाङ्क fd, वर्ण ध्रुव *file_x, संरचना fstatus *fstatus)
{
  पूर्णाङ्क err = 0;
  वर्ण buf[IO_BUFSIZE + 1];
  intmax_t पङ्क्तयः, शब्दाः, chars, अष्टकानि, linelength;
  bool गणनम्_अष्टकानि_2, गणनम्_chars, गणनम्_complicated;
  वर्ण ध्रुव *file = file_x ? file_x : _("standard input");

  पङ्क्तयः = शब्दाः = chars = अष्टकानि = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  यदि (MB_CUR_MAX > 1)
    {
      गणनम्_अष्टकानि_2 = print_अष्टकानि;
      गणनम्_chars = print_chars;
    }
  अन्यथा
    {
      गणनम्_अष्टकानि_2 = print_अष्टकानि || print_chars;
      गणनम्_chars = false;
    }
  गणनम्_complicated = print_शब्दाः || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  यदि (!गणनम्_अष्टकानि_2 || गणनम्_chars || print_पङ्क्तयः || गणनम्_complicated)
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

  यदि (गणनम्_अष्टकानि_2 && !गणनम्_chars && !print_पङ्क्तयः && !गणनम्_complicated)
    {
      bool skip_read = false;

      यदि (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      यदि (! fstatus->failed && usable_st_आकार (&fstatus->st)
          && 0 <= fstatus->st.st_आकार)
        {
          off_t अन्तः_pos = fstatus->st.st_आकार;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          यदि (current_pos < 0)
            ;
          अन्यथा यदि (अन्तः_pos % page_आकार)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              यदि (current_pos < अन्तः_pos)
                {
                  अष्टकानि = अन्तः_pos - current_pos;
                  skip_read = true;
                }
            }
          अन्यथा यदि (0 < अन्तः_pos)
            {
              off_t ep_1 = अन्तः_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              यदि (current_pos < hi_pos)
                अष्टकानि = hi_pos - current_pos;
            }

          यदि (0 < अष्टकानि && lseek (fd, अष्टकानि, SEEK_CUR) < 0)
            {
              अष्टकानि = 0;
              skip_read = false;
            }
        }

      यदि (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          प्रति (ssize_t अष्टकानि_read;
               (अष्टकानि_read = पठ (fd, buf, IO_BUFSIZE));
               अष्टकानि += अष्टकानि_read)
            यदि (अष्टकानि_read < 0)
              {
                err = errno;
                भङ्ग;
              }
        }
    }
  अन्यथा यदि (!गणनम्_chars && !गणनम्_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      संरचना wc_पङ्क्तयः w = wc_पङ्क्तयः (fd);
      err = w.err;
      पङ्क्तयः = w.पङ्क्तयः;
      अष्टकानि = w.अष्टकानि;
    }
  अन्यथा यदि (MB_CUR_MAX > 1)
    {
      bool अन्तर्_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool अन्तर्_सरणम् = false;
      bool गणनम्_utf8 = !गणनम्_complicated && !print_पङ्क्तयः
                        && is_utf8_charset ();
      idx_t पूर्व = 0; /* Number of bytes carried over from previous round.  */

      प्रति (ssize_t अष्टकानि_read;
           ((अष्टकानि_read = पठ (fd, buf + पूर्व, IO_BUFSIZE - पूर्व))
            || पूर्व);
           )
        {
          यदि (अष्टकानि_read < 0)
            {
              err = errno;
              भङ्ग;
            }

          अष्टकानि += अष्टकानि_read;
          वर्ण ध्रुव *p = buf;
          वर्ण ध्रुव *plim = p + पूर्व + अष्टकानि_read;
          bool try_गणनम्_utf8 = गणनम्_utf8;
          कुरु
            {
              यदि (try_गणनम्_utf8 && !अन्तर्_सरणम्)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_गणनम्_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_गणनम् (p, &n);
                  p += n;
                  यदि (p == plim)
                    भङ्ग;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              यदि (!अन्तर्_सरणम् && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              अन्यथा
                {
                  idx_t पूर्व_अष्टकानि = पूर्व;
                  idx_t scanbytes = plim - (p + पूर्व);
                  size_t n = mbrtoc32 (&wide_char, p + पूर्व, scanbytes, &state);
                  पूर्व = 0;

                  यदि (scanbytes < n)
                    {
                      यदि (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && अष्टकानि_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          पूर्व = plim - p;
                          memmove (buf, p, पूर्व);
                          अन्तर्_सरणम् = true;
                          भङ्ग;
                        }

                      /* Remember that we read a byte, but don't complain
                         about the error.  Because of the decoding error,
                         this is a considered to be byte but not a
                         character (that is, chars is not incremented).  */
                      p++;
                      mbszero (&state);
                      अन्तर्_सरणम् = false;

                      /* Treat encoding errors as non white space.
                         POSIX says a word is "a non-zero-length string of
                         characters delimited by white space".  This is
                         wrong in some sense, as the string can be delimited
                         by start or end of input, and it is unclear what it
                         means when the input contains encoding errors.
                         Since encoding errors are not white space,
                         treat them that way here.  */
                      शब्दाः += !अन्तर्_word;
                      अन्तर्_word = true;
                      अनुवर्त;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = पूर्व_अष्टकानि + n + !n;
                  single_byte = charbytes == !अन्तर्_सरणम्;
                  अन्तर्_सरणम् = !mbsinit (&state);
                }

              विकल्प (wide_char)
                {
                पक्ष '\n':
                  पङ्क्तयः++;
                  FALLTHROUGH;
                पक्ष '\r':
                पक्ष '\f':
                  यदि (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  अन्तर्_word = false;
                  भङ्ग;

                पक्ष '\t':
                  linepos += 8 - (linepos % 8);
                  अन्तर्_word = false;
                  भङ्ग;

                पक्ष ' ':
                  linepos++;
                  FALLTHROUGH;
                पक्ष '\v':
                  अन्तर्_word = false;
                  भङ्ग;

                स्वभाव:;
                  bool अन्तर्_word2;
                  यदि (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      अन्तर्_word2 = !wc_isspace[wide_char];
                    }
                  अन्यथा
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      यदि (print_linelength)
                        {
                          पूर्णाङ्क विस्तारः = c32width (wide_char);
                          यदि (विस्तारः > 0)
                            linepos += विस्तारः;
                        }
                      अन्तर्_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  शब्दाः += !अन्तर्_word & अन्तर्_word2;
                  अन्तर्_word = अन्तर्_word2;
                  भङ्ग;
                }

              p += charbytes;
              chars++;
            }
          यावत् (p < plim);
        }
      यदि (linepos > linelength)
        linelength = linepos;
    }
  अन्यथा
    {
      bool अन्तर्_word = false;
      intmax_t linepos = 0;

      प्रति (ssize_t अष्टकानि_read; (अष्टकानि_read = पठ (fd, buf, IO_BUFSIZE)); )
        {
          यदि (अष्टकानि_read < 0)
            {
              err = errno;
              भङ्ग;
            }

          अष्टकानि += अष्टकानि_read;
          वर्ण ध्रुव *p = buf;
          कुरु
            {
              अचिह्नित वर्ण c = *p++;
              विकल्प (c)
                {
                पक्ष '\n':
                  पङ्क्तयः++;
                  FALLTHROUGH;
                पक्ष '\r':
                पक्ष '\f':
                  यदि (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  अन्तर्_word = false;
                  भङ्ग;

                पक्ष '\t':
                  linepos += 8 - (linepos % 8);
                  अन्तर्_word = false;
                  भङ्ग;

                पक्ष ' ':
                  linepos++;
                  FALLTHROUGH;
                पक्ष '\v':
                  अन्तर्_word = false;
                  भङ्ग;

                स्वभाव:
                  linepos += wc_isprint[c];
                  bool अन्तर्_word2 = !wc_isspace[c];
                  शब्दाः += !अन्तर्_word & अन्तर्_word2;
                  अन्तर्_word = अन्तर्_word2;
                  भङ्ग;
                }
            }
          यावत् (--अष्टकानि_read);
        }
      यदि (linepos > linelength)
        linelength = linepos;
    }

  यदि (गणनम्_chars < print_chars)
    chars = अष्टकानि;

  यदि (योग_बहुलकम् != योग_only)
    write_counts (पङ्क्तयः, शब्दाः, chars, अष्टकानि, linelength, file_x);

  योग_पङ्क्तयः_अतिप्रवाहः |= ckd_सङ्कलय (&योग_पङ्क्तयः, योग_पङ्क्तयः, पङ्क्तयः);
  योग_शब्दाः_अतिप्रवाहः |= ckd_सङ्कलय (&योग_शब्दाः, योग_शब्दाः, शब्दाः);
  योग_chars_अतिप्रवाहः |= ckd_सङ्कलय (&योग_chars, योग_chars, chars);
  योग_अष्टकानि_अतिप्रवाहः |= ckd_सङ्कलय (&योग_अष्टकानि, योग_अष्टकानि, अष्टकानि);

  यदि (linelength > max_पङ्क्तिः_दीर्घता)
    max_पङ्क्तिः_दीर्घता = linelength;

  यदि (err)
    error (0, err, "%s", quotef (file));
  प्रत्यागम !err;
}

स्थिर bool
wc_file (वर्ण ध्रुव *file, संरचना fstatus *fstatus)
{
  यदि (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_द्विमान_बहुलकम् (STDIN_FILENO, O_द्विमान);
      प्रत्यागम wc (STDIN_FILENO, file, fstatus);
    }
  अन्यथा
    {
      पूर्णाङ्क fd = उद्घाटय (file, O_RDONLY | O_द्विमान);
      यदि (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          प्रत्यागम false;
        }
      अन्यथा
        {
          bool ok = wc (fd, file, fstatus);
          यदि (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              प्रत्यागम false;
            }
          प्रत्यागम ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

स्थिर संरचना fstatus *
get_निवेशः_fstatus (idx_t nfiles, वर्ण *ध्रुव *file)
{
  संरचना fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, परिमाण *fstatus);

  यदि (nfiles == 0
      || (nfiles == 1
          && ((print_पङ्क्तयः + print_शब्दाः + print_chars
               + print_अष्टकानि + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  अन्यथा
    {
      प्रति (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  प्रत्यागम fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_विशुद्धम्
स्थिर पूर्णाङ्क
compute_सङ्ख्या_विस्तारः (idx_t nfiles, संरचना fstatus ध्रुव *fstatus)
{
  पूर्णाङ्क विस्तारः = 1;

  यदि (0 < nfiles && fstatus[0].failed <= 0)
    {
      पूर्णाङ्क minimum_विस्तारः = 1;
      uintmax_t regular_योग = 0;

      प्रति (idx_t i = 0; i < nfiles; i++)
        यदि (! fstatus[i].failed)
          {
            यदि (!S_ISREG (fstatus[i].st.st_बहुलकम्))
              minimum_विस्तारः = 7;
            अन्यथा यदि (ckd_सङ्कलय (&regular_योग, regular_योग,
                              fstatus[i].st.st_आकार))
              {
                regular_योग = UINTMAX_MAX;
                भङ्ग;
              }
          }

      प्रति (; 10 <= regular_योग; regular_योग /= 10)
        विस्तारः++;
      यदि (विस्तारः < minimum_विस्तारः)
        विस्तारः = minimum_विस्तारः;
    }

  प्रत्यागम विस्तारः;
}


पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc;
  idx_t nfiles;
  वर्ण **files;
  वर्ण *files_from = रिक्त;
  संरचना fstatus *fstatus;
  संरचना Tokens tok;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  page_आकार = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, रिक्त, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != रिक्त);

  यावत् ((optc = getopt_long (argc, argv, "clLmw", longopts, रिक्त)) != -1)
    विकल्प (optc)
      {
      पक्ष 'c':
        print_अष्टकानि = true;
        भङ्ग;

      पक्ष 'm':
        print_chars = true;
        भङ्ग;

      पक्ष 'l':
        print_पङ्क्तयः = true;
        भङ्ग;

      पक्ष 'w':
        print_शब्दाः = true;
        भङ्ग;

      पक्ष 'L':
        print_linelength = true;
        भङ्ग;

      पक्ष DEBUG_क्रमादेशः_OPTION:
        debug = true;
        भङ्ग;

      पक्ष FILES0_FROM_OPTION:
        files_from = optarg;
        भङ्ग;

      पक्ष योग_OPTION:
        योग_बहुलकम् = XARGMATCH ("--total", optarg, योग_तर्काः, योग_types);
        भङ्ग;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

      स्वभाव:
        usage (EXIT_FAILURE);
      }

  यदि (! (print_पङ्क्तयः || print_शब्दाः || print_chars || print_अष्टकानि
         || print_linelength))
    print_पङ्क्तयः = print_शब्दाः = print_अष्टकानि = true;

  यदि (print_linelength)
    प्रति (पूर्णाङ्क i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  यदि (print_शब्दाः)
    प्रति (पूर्णाङ्क i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = रिक्तम्_वा (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  संरचना argv_iterator *ai;
  यदि (files_from)
    {
      FILE *प्रवाहः;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      यदि (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      यदि (streq (files_from, "-"))
        प्रवाहः = stdin;
      अन्यथा
        {
          प्रवाहः = fopen (files_from, "r");
          यदि (प्रवाहः == रिक्त)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      संरचना stat st;
      यदि (fstat (fileno (प्रवाहः), &st) == 0
          && S_ISREG (st.st_बहुलकम्)
          && st.st_आकार <= MIN (10 * 1024 * 1024, physmem_उपलब्धम् () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          यदि (! readtokens0 (प्रवाहः, &tok) || fclose (प्रवाहः) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      अन्यथा
        {
          files = रिक्त;
          nfiles = 0;
          ai = argv_iter_init_प्रवाहः (प्रवाहः);
        }
    }
  अन्यथा
    {
      स्थिर वर्ण *stdin_only[] = { रिक्त };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  यदि (!ai)
    xalloc_die ();

  fstatus = get_निवेशः_fstatus (nfiles, files);
  यदि (योग_बहुलकम् == योग_only)
    सङ्ख्या_विस्तारः = 1;  /* No extra padding, since no alignment requirement.  */
  अन्यथा
    सङ्ख्या_विस्तारः = compute_सङ्ख्या_विस्तारः (nfiles, fstatus);

  bool ok = true;
  गणना argv_iter_err ai_err;
  वर्ण *file_name;
  प्रति (पूर्णाङ्क i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      यदि (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      यदि (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          यदि (files_from == रिक्त)
            error (0, 0, "%s", _("invalid zero-length file name"));
          अन्यथा
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_तर्काः (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      यदि (skip_file)
        ok = false;
      अन्यथा
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      यदि (! nfiles)
        fstatus[0].failed = 1;
    }
  विकल्प (ai_err)
    {
    पक्ष AI_ERR_EOF:
      भङ्ग;

    पक्ष AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      भङ्ग;

    पक्ष AI_ERR_MEM:
      xalloc_die ();

    पक्ष AI_ERR_OK: स्वभाव:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  यदि (ok && !files_from && argv_iter_n_तर्काः (ai) == 0)
    ok &= wc_file (रिक्त, &fstatus[0]);

  यदि (read_tokens)
    readtokens0_free (&tok);

  यदि (योग_बहुलकम् != योग_never
      && (योग_बहुलकम् != योग_auto || 1 < argv_iter_n_तर्काः (ai)))
    {
      यदि (योग_पङ्क्तयः_अतिप्रवाहः)
        {
          योग_पङ्क्तयः = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      यदि (योग_शब्दाः_अतिप्रवाहः)
        {
          योग_शब्दाः = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      यदि (योग_chars_अतिप्रवाहः)
        {
          योग_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      यदि (योग_अष्टकानि_अतिप्रवाहः)
        {
          योग_अष्टकानि = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (योग_पङ्क्तयः, योग_शब्दाः, योग_chars, योग_अष्टकानि,
                    max_पङ्क्तिः_दीर्घता,
                    योग_बहुलकम् != योग_only ? _("total") : रिक्त);
    }

  argv_iter_free (ai);

  मुक्त (fstatus);

  यदि (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
