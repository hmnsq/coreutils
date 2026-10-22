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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ <argmatch.h>
#அந்தர்பாவ <argv-iter.h>
#அந்தர்பாவ <fadvise.h>
#அந்தர்பாவ <physmem.h>
#அந்தர்பாவ <readtokens0.h>
#அந்தர்பாவ <stat-size.h>
#அந்தர்பாவ <xbinary-io.h>

#ifdef USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#endif

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "cpu-supports.h"
#அந்தர்பாவ "ioblksize.h"
#அந்தர்பாவ "wc.h"
#அந்தர்பாவ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "wc"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

நிலையான bool wc_isprint[UCHAR_MAX + 1];
நிலையான bool wc_isspace[UCHAR_MAX + 1];

நிலையான bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
நிலையான uintmax_t total_வரிகள்;
நிலையான uintmax_t total_சொற்கள்;
நிலையான uintmax_t total_chars;
நிலையான uintmax_t total_bytes;
நிலையான bool total_வரிகள்_overflow;
நிலையான bool total_சொற்கள்_overflow;
நிலையான bool total_chars_overflow;
நிலையான bool total_bytes_overflow;
நிலையான intmax_t max_வரி_length;

/* Which counts to print. */
நிலையான bool print_வரிகள், print_சொற்கள், print_chars, print_bytes;
நிலையான bool print_linelength;

/* The print width of each count.  */
நிலையான முழு எண்_width;

/* True if we have ever read the standard input. */
நிலையான bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
நிலையான idx_t page_size;

/* Enable to _not_ treat non breaking space as a word separator.  */
நிலையான bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
கட்டமைப்பு fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  முழு failed;

  /* If FAILED is zero, this is the file's status.  */
  கட்டமைப்பு stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  DEBUG_PROGRAM_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  TOTAL_OPTION,
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"bytes", no_argument, வெற்று, 'c'},
  {"chars", no_argument, வெற்று, 'm'},
  {"lines", no_argument, வெற்று, 'l'},
  {"words", no_argument, வெற்று, 'w'},
  {"debug", no_argument, வெற்று, DEBUG_PROGRAM_OPTION},
  {"files0-from", required_argument, வெற்று, FILES0_FROM_OPTION},
  {"max-line-length", no_argument, வெற்று, 'L'},
  {"total", required_argument, வெற்று, TOTAL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

எண்ணிக்கை total_type
  {
    total_auto,         /* 0: default or --total=auto */
    total_always,       /* 1: --total=always */
    total_only,         /* 2: --total=only */
    total_never         /* 3: --total=never */
  };
நிலையான எழுத்து மாறிலி *மாறிலி total_args[] =
{
  "auto", "always", "only", "never", வெற்று
};
நிலையான எண்ணிக்கை total_type மாறிலி total_types[] =
{
  total_auto, total_always, total_only, total_never
};
ARGMATCH_VERIFY (total_args, total_types);
நிலையான எண்ணிக்கை total_type total_mode = total_auto;

#ifdef USE_AVX2_WC_LINECOUNT
நிலையான bool
avx2_supported (வெறுமை)
{
  bool avx2_enabled = cpu_supports ("avx2");
  எனில் (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  திரும்பு avx2_enabled;
}
#endif

#ifdef USE_AVX512_WC_LINECOUNT
நிலையான bool
avx512_supported (வெறுமை)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  எனில் (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  திரும்பு avx512_enabled;
}
#endif

#ifdef USE_NEON_WC_LINECOUNT
நிலையான bool
neon_supported (வெறுமை)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  எனில் (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  திரும்பு neon_enabled;
}
#endif

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      fputs (_("\
Print newline, word, and byte counts க்கு each FILE, and a total வரி எனில்\n\
more than one FILE is specified.  A word is a nonempty sequence of non white\n\
space delimited by white space characters or by start or end of உள்ளீடு.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum வரி length.\n\
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
  -l, --வரிகள்\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what வரி மொத்தம் acceleration is used\n\
"));
      oputs (_("\
      --files0-from=F\n\
         பட உள்ளீடு from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, பட names from standard உள்ளீடு\n\
"));
      oputs (_("\
  -L, --max-வரி-length\n\
         print the maximum display width\n\
"));
      oputs (_("\
  -w, --சொற்கள்\n\
         print the word counts\n\
"));
      oputs (_("\
      --total=WHEN\n\
         when to print a வரி with total counts;\n\
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
நிலையான முழு
maybe_c32isnbspace (char32_t wc)
{
  திரும்பு ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
நிலையான வெறுமை
write_counts (uintmax_t வரிகள்,
              uintmax_t சொற்கள்,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              எழுத்து மாறிலி *file)
{
  நிலையான எழுத்து மாறிலி format_sp_int[] = " %*s";
  எழுத்து மாறிலி *format_int = format_sp_int + 1;
  எழுத்து buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  எனில் (print_வரிகள்)
    {
      printf (format_int, எண்_width, umaxtostr (வரிகள், buf));
      format_int = format_sp_int;
    }
  எனில் (print_சொற்கள்)
    {
      printf (format_int, எண்_width, umaxtostr (சொற்கள், buf));
      format_int = format_sp_int;
    }
  எனில் (print_chars)
    {
      printf (format_int, எண்_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  எனில் (print_bytes)
    {
      printf (format_int, எண்_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  எனில் (print_linelength)
    printf (format_int, எண்_width, imaxtostr (linelength, buf));
  எனில் (file)
    printf (" %s", strchr (file, '\n') ? quotef (file) : file);
  putchar ('\n');

  எனில் (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
நிலையான கட்டமைப்பு wc_வரிகள்
wc_வரிகள் (முழு fd)
{
#ifdef USE_AVX512_WC_LINECOUNT
  நிலையான signed எழுத்து use_avx512;
  எனில் (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  எனில் (0 < use_avx512)
    திரும்பு wc_வரிகள்_avx512 (fd);
#endif
#ifdef USE_AVX2_WC_LINECOUNT
  நிலையான signed எழுத்து use_avx2;
  எனில் (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  எனில் (0 < use_avx2)
    திரும்பு wc_வரிகள்_avx2 (fd);
#endif
#ifdef USE_NEON_WC_LINECOUNT
  நிலையான signed எழுத்து use_neon;
  எனில் (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  எனில் (0 < use_neon)
    திரும்பு wc_வரிகள்_neon (fd);
#endif

  intmax_t வரிகள் = 0, bytes = 0;
  bool long_வரிகள் = false;

  போது (true)
    {
      எழுத்து buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = பட (fd, buf, IO_BUFSIZE);
      எனில் (bytes_read <= 0)
        திரும்பு (கட்டமைப்பு wc_வரிகள்) { bytes_read == 0 ? 0 : errno, வரிகள், bytes };

      bytes += bytes_read;
      எழுத்து *end = buf + bytes_read;
      idx_t buflines = 0;

      எனில் (! long_வரிகள்)
        {
          /* Avoid function call overhead for shorter lines.  */
          க்கு (எழுத்து *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      இல்லையெனில்
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          க்கு (எழுத்து *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_வரிகள் = 15 * buflines <= bytes_read;
      வரிகள் += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
நிலையான bool
wc (முழு fd, எழுத்து மாறிலி *file_x, கட்டமைப்பு fstatus *fstatus)
{
  முழு err = 0;
  எழுத்து buf[IO_BUFSIZE + 1];
  intmax_t வரிகள், சொற்கள், chars, bytes, linelength;
  bool மொத்தம்_bytes, மொத்தம்_chars, மொத்தம்_complicated;
  எழுத்து மாறிலி *file = file_x ? file_x : _("standard input");

  வரிகள் = சொற்கள் = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  எனில் (MB_CUR_MAX > 1)
    {
      மொத்தம்_bytes = print_bytes;
      மொத்தம்_chars = print_chars;
    }
  இல்லையெனில்
    {
      மொத்தம்_bytes = print_bytes || print_chars;
      மொத்தம்_chars = false;
    }
  மொத்தம்_complicated = print_சொற்கள் || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  எனில் (!மொத்தம்_bytes || மொத்தம்_chars || print_வரிகள் || மொத்தம்_complicated)
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

  எனில் (மொத்தம்_bytes && !மொத்தம்_chars && !print_வரிகள் && !மொத்தம்_complicated)
    {
      bool skip_read = false;

      எனில் (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      எனில் (! fstatus->failed && usable_st_size (&fstatus->st)
          && 0 <= fstatus->st.st_size)
        {
          off_t end_pos = fstatus->st.st_size;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          எனில் (current_pos < 0)
            ;
          இல்லையெனில் எனில் (end_pos % page_size)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              எனில் (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          இல்லையெனில் எனில் (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              எனில் (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          எனில் (0 < bytes && lseek (fd, bytes, SEEK_CUR) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      எனில் (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          க்கு (ssize_t bytes_read;
               (bytes_read = பட (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            எனில் (bytes_read < 0)
              {
                err = errno;
                நிறுத்து;
              }
        }
    }
  இல்லையெனில் எனில் (!மொத்தம்_chars && !மொத்தம்_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      கட்டமைப்பு wc_வரிகள் w = wc_வரிகள் (fd);
      err = w.err;
      வரிகள் = w.வரிகள்;
      bytes = w.bytes;
    }
  இல்லையெனில் எனில் (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool மொத்தம்_utf8 = !மொத்தம்_complicated && !print_வரிகள்
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      க்கு (ssize_t bytes_read;
           ((bytes_read = பட (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          எனில் (bytes_read < 0)
            {
              err = errno;
              நிறுத்து;
            }

          bytes += bytes_read;
          எழுத்து மாறிலி *p = buf;
          எழுத்து மாறிலி *plim = p + prev + bytes_read;
          bool try_மொத்தம்_utf8 = மொத்தம்_utf8;
          செய்
            {
              எனில் (try_மொத்தம்_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_மொத்தம்_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_மொத்தம் (p, &n);
                  p += n;
                  எனில் (p == plim)
                    நிறுத்து;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              எனில் (!in_shift && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              இல்லையெனில்
                {
                  idx_t prev_bytes = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  எனில் (scanbytes < n)
                    {
                      எனில் (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          in_shift = true;
                          நிறுத்து;
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
                      சொற்கள் += !in_word;
                      in_word = true;
                      தொடர்;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              தேர்வு (wide_char)
                {
                நிலை '\n':
                  வரிகள்++;
                  FALLTHROUGH;
                நிலை '\r':
                நிலை '\f':
                  எனில் (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  நிறுத்து;

                நிலை '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  நிறுத்து;

                நிலை ' ':
                  linepos++;
                  FALLTHROUGH;
                நிலை '\v':
                  in_word = false;
                  நிறுத்து;

                இயல்பு:;
                  bool in_word2;
                  எனில் (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  இல்லையெனில்
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      எனில் (print_linelength)
                        {
                          முழு width = c32width (wide_char);
                          எனில் (width > 0)
                            linepos += width;
                        }
                      in_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  சொற்கள் += !in_word & in_word2;
                  in_word = in_word2;
                  நிறுத்து;
                }

              p += charbytes;
              chars++;
            }
          போது (p < plim);
        }
      எனில் (linepos > linelength)
        linelength = linepos;
    }
  இல்லையெனில்
    {
      bool in_word = false;
      intmax_t linepos = 0;

      க்கு (ssize_t bytes_read; (bytes_read = பட (fd, buf, IO_BUFSIZE)); )
        {
          எனில் (bytes_read < 0)
            {
              err = errno;
              நிறுத்து;
            }

          bytes += bytes_read;
          எழுத்து மாறிலி *p = buf;
          செய்
            {
              unsigned எழுத்து c = *p++;
              தேர்வு (c)
                {
                நிலை '\n':
                  வரிகள்++;
                  FALLTHROUGH;
                நிலை '\r':
                நிலை '\f':
                  எனில் (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  நிறுத்து;

                நிலை '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  நிறுத்து;

                நிலை ' ':
                  linepos++;
                  FALLTHROUGH;
                நிலை '\v':
                  in_word = false;
                  நிறுத்து;

                இயல்பு:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  சொற்கள் += !in_word & in_word2;
                  in_word = in_word2;
                  நிறுத்து;
                }
            }
          போது (--bytes_read);
        }
      எனில் (linepos > linelength)
        linelength = linepos;
    }

  எனில் (மொத்தம்_chars < print_chars)
    chars = bytes;

  எனில் (total_mode != total_only)
    write_counts (வரிகள், சொற்கள், chars, bytes, linelength, file_x);

  total_வரிகள்_overflow |= ckd_add (&total_வரிகள், total_வரிகள், வரிகள்);
  total_சொற்கள்_overflow |= ckd_add (&total_சொற்கள், total_சொற்கள், சொற்கள்);
  total_chars_overflow |= ckd_add (&total_chars, total_chars, chars);
  total_bytes_overflow |= ckd_add (&total_bytes, total_bytes, bytes);

  எனில் (linelength > max_வரி_length)
    max_வரி_length = linelength;

  எனில் (err)
    error (0, err, "%s", quotef (file));
  திரும்பு !err;
}

நிலையான bool
wc_file (எழுத்து மாறிலி *file, கட்டமைப்பு fstatus *fstatus)
{
  எனில் (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_இருமம்_mode (STDIN_FILENO, O_இருமம்);
      திரும்பு wc (STDIN_FILENO, file, fstatus);
    }
  இல்லையெனில்
    {
      முழு fd = open (file, O_RDONLY | O_இருமம்);
      எனில் (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          திரும்பு false;
        }
      இல்லையெனில்
        {
          bool ok = wc (fd, file, fstatus);
          எனில் (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              திரும்பு false;
            }
          திரும்பு ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

நிலையான கட்டமைப்பு fstatus *
get_உள்ளீடு_fstatus (idx_t nfiles, எழுத்து *மாறிலி *file)
{
  கட்டமைப்பு fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, அளவு *fstatus);

  எனில் (nfiles == 0
      || (nfiles == 1
          && ((print_வரிகள் + print_சொற்கள் + print_chars
               + print_bytes + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  இல்லையெனில்
    {
      க்கு (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  திரும்பு fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_PURE
நிலையான முழு
compute_எண்_width (idx_t nfiles, கட்டமைப்பு fstatus மாறிலி *fstatus)
{
  முழு width = 1;

  எனில் (0 < nfiles && fstatus[0].failed <= 0)
    {
      முழு minimum_width = 1;
      uintmax_t regular_total = 0;

      க்கு (idx_t i = 0; i < nfiles; i++)
        எனில் (! fstatus[i].failed)
          {
            எனில் (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            இல்லையெனில் எனில் (ckd_add (&regular_total, regular_total,
                              fstatus[i].st.st_size))
              {
                regular_total = UINTMAX_MAX;
                நிறுத்து;
              }
          }

      க்கு (; 10 <= regular_total; regular_total /= 10)
        width++;
      எனில் (width < minimum_width)
        width = minimum_width;
    }

  திரும்பு width;
}


முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு optc;
  idx_t nfiles;
  எழுத்து **files;
  எழுத்து *files_from = வெற்று;
  கட்டமைப்பு fstatus *fstatus;
  கட்டமைப்பு Tokens tok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  page_size = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, வெற்று, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != வெற்று);

  போது ((optc = getopt_long (argc, argv, "clLmw", longopts, வெற்று)) != -1)
    தேர்வு (optc)
      {
      நிலை 'c':
        print_bytes = true;
        நிறுத்து;

      நிலை 'm':
        print_chars = true;
        நிறுத்து;

      நிலை 'l':
        print_வரிகள் = true;
        நிறுத்து;

      நிலை 'w':
        print_சொற்கள் = true;
        நிறுத்து;

      நிலை 'L':
        print_linelength = true;
        நிறுத்து;

      நிலை DEBUG_PROGRAM_OPTION:
        debug = true;
        நிறுத்து;

      நிலை FILES0_FROM_OPTION:
        files_from = optarg;
        நிறுத்து;

      நிலை TOTAL_OPTION:
        total_mode = XARGMATCH ("--total", optarg, total_args, total_types);
        நிறுத்து;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      இயல்பு:
        usage (EXIT_FAILURE);
      }

  எனில் (! (print_வரிகள் || print_சொற்கள் || print_chars || print_bytes
         || print_linelength))
    print_வரிகள் = print_சொற்கள் = print_bytes = true;

  எனில் (print_linelength)
    க்கு (முழு i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  எனில் (print_சொற்கள்)
    க்கு (முழு i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = isspace (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  கட்டமைப்பு argv_iterator *ai;
  எனில் (files_from)
    {
      FILE *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      எனில் (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      எனில் (streq (files_from, "-"))
        stream = stdin;
      இல்லையெனில்
        {
          stream = fopen (files_from, "r");
          எனில் (stream == வெற்று)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      கட்டமைப்பு stat st;
      எனில் (fstat (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_size <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          எனில் (! readtokens0 (stream, &tok) || fclose (stream) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      இல்லையெனில்
        {
          files = வெற்று;
          nfiles = 0;
          ai = argv_iter_init_stream (stream);
        }
    }
  இல்லையெனில்
    {
      நிலையான எழுத்து *stdin_only[] = { வெற்று };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  எனில் (!ai)
    xalloc_die ();

  fstatus = get_உள்ளீடு_fstatus (nfiles, files);
  எனில் (total_mode == total_only)
    எண்_width = 1;  /* No extra padding, since no alignment requirement.  */
  இல்லையெனில்
    எண்_width = compute_எண்_width (nfiles, fstatus);

  bool ok = true;
  எண்ணிக்கை argv_iter_err ai_err;
  எழுத்து *file_name;
  க்கு (முழு i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      எனில் (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      எனில் (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          எனில் (files_from == வெற்று)
            error (0, 0, "%s", _("invalid zero-length file name"));
          இல்லையெனில்
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_args (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      எனில் (skip_file)
        ok = false;
      இல்லையெனில்
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      எனில் (! nfiles)
        fstatus[0].failed = 1;
    }
  தேர்வு (ai_err)
    {
    நிலை AI_ERR_EOF:
      நிறுத்து;

    நிலை AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      நிறுத்து;

    நிலை AI_ERR_MEM:
      xalloc_die ();

    நிலை AI_ERR_OK: இயல்பு:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  எனில் (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (வெற்று, &fstatus[0]);

  எனில் (read_tokens)
    readtokens0_free (&tok);

  எனில் (total_mode != total_never
      && (total_mode != total_auto || 1 < argv_iter_n_args (ai)))
    {
      எனில் (total_வரிகள்_overflow)
        {
          total_வரிகள் = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      எனில் (total_சொற்கள்_overflow)
        {
          total_சொற்கள் = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      எனில் (total_chars_overflow)
        {
          total_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      எனில் (total_bytes_overflow)
        {
          total_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (total_வரிகள், total_சொற்கள், total_chars, total_bytes,
                    max_வரி_length,
                    total_mode != total_only ? _("total") : வெற்று);
    }

  argv_iter_free (ai);

  விடுவி (fstatus);

  எனில் (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
