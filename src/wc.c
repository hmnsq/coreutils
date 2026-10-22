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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ <argmatch.h>
#અન્તર્ભાવ <argv-iter.h>
#અન્તર્ભાવ <fadvise.h>
#અન્તર્ભાવ <physmem.h>
#અન્તર્ભાવ <readtokens0.h>
#અન્તર્ભાવ <stat-size.h>
#અન્તર્ભાવ <xbinary-io.h>

#ifdef USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#endif

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "cpu-supports.h"
#અન્તર્ભાવ "ioblksize.h"
#અન્તર્ભાવ "wc.h"
#અન્તર્ભાવ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "wc"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

સ્થિર bool wc_isprint[UCHAR_MAX + 1];
સ્થિર bool wc_isspace[UCHAR_MAX + 1];

સ્થિર bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
સ્થિર uintmax_t total_લીટીઓ;
સ્થિર uintmax_t total_શબ્દો;
સ્થિર uintmax_t total_chars;
સ્થિર uintmax_t total_bytes;
સ્થિર bool total_લીટીઓ_overflow;
સ્થિર bool total_શબ્દો_overflow;
સ્થિર bool total_chars_overflow;
સ્થિર bool total_bytes_overflow;
સ્થિર intmax_t max_લીટી_length;

/* Which counts to print. */
સ્થિર bool print_લીટીઓ, print_શબ્દો, print_chars, print_bytes;
સ્થિર bool print_linelength;

/* The print width of each count.  */
સ્થિર પૂર્ણાંક સંખ્યા_width;

/* True if we have ever read the standard input. */
સ્થિર bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
સ્થિર idx_t page_size;

/* Enable to _not_ treat non breaking space as a word separator.  */
સ્થિર bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
સંરચના fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  પૂર્ણાંક failed;

  /* If FAILED is zero, this is the file's status.  */
  સંરચના stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  DEBUG_PROGRAM_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  TOTAL_OPTION,
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"bytes", no_argument, ખાલી, 'c'},
  {"chars", no_argument, ખાલી, 'm'},
  {"lines", no_argument, ખાલી, 'l'},
  {"words", no_argument, ખાલી, 'w'},
  {"debug", no_argument, ખાલી, DEBUG_PROGRAM_OPTION},
  {"files0-from", required_argument, ખાલી, FILES0_FROM_OPTION},
  {"max-line-length", no_argument, ખાલી, 'L'},
  {"total", required_argument, ખાલી, TOTAL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

ગણના total_type
  {
    total_auto,         /* 0: default or --total=auto */
    total_always,       /* 1: --total=always */
    total_only,         /* 2: --total=only */
    total_never         /* 3: --total=never */
  };
સ્થિર અક્ષર અચલ *અચલ total_args[] =
{
  "auto", "always", "only", "never", ખાલી
};
સ્થિર ગણના total_type અચલ total_types[] =
{
  total_auto, total_always, total_only, total_never
};
ARGMATCH_VERIFY (total_args, total_types);
સ્થિર ગણના total_type total_mode = total_auto;

#ifdef USE_AVX2_WC_LINECOUNT
સ્થિર bool
avx2_supported (શૂન્ય)
{
  bool avx2_enabled = cpu_supports ("avx2");
  જો (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  પાછા avx2_enabled;
}
#endif

#ifdef USE_AVX512_WC_LINECOUNT
સ્થિર bool
avx512_supported (શૂન્ય)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  જો (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  પાછા avx512_enabled;
}
#endif

#ifdef USE_NEON_WC_LINECOUNT
સ્થિર bool
neon_supported (શૂન્ય)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  જો (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  પાછા neon_enabled;
}
#endif

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      fputs (_("\
Print newline, word, and byte counts માટે each FILE, and a total લીટી જો\n\
more than one FILE is specified.  A word is a nonempty sequence of non white\n\
space delimited by white space characters or by start or end of આગત.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum લીટી length.\n\
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
  -l, --લીટીઓ\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what લીટી ગણતરી acceleration is used\n\
"));
      oputs (_("\
      --files0-from=F\n\
         પઠ આગત from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, પઠ names from standard આગત\n\
"));
      oputs (_("\
  -L, --max-લીટી-length\n\
         print the maximum display width\n\
"));
      oputs (_("\
  -w, --શબ્દો\n\
         print the word counts\n\
"));
      oputs (_("\
      --total=WHEN\n\
         when to print a લીટી with total counts;\n\
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
સ્થિર પૂર્ણાંક
maybe_c32isnbspace (char32_t wc)
{
  પાછા ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
સ્થિર શૂન્ય
write_counts (uintmax_t લીટીઓ,
              uintmax_t શબ્દો,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              અક્ષર અચલ *file)
{
  સ્થિર અક્ષર અચલ format_sp_int[] = " %*s";
  અક્ષર અચલ *format_int = format_sp_int + 1;
  અક્ષર buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  જો (print_લીટીઓ)
    {
      printf (format_int, સંખ્યા_width, umaxtostr (લીટીઓ, buf));
      format_int = format_sp_int;
    }
  જો (print_શબ્દો)
    {
      printf (format_int, સંખ્યા_width, umaxtostr (શબ્દો, buf));
      format_int = format_sp_int;
    }
  જો (print_chars)
    {
      printf (format_int, સંખ્યા_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  જો (print_bytes)
    {
      printf (format_int, સંખ્યા_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  જો (print_linelength)
    printf (format_int, સંખ્યા_width, imaxtostr (linelength, buf));
  જો (file)
    printf (" %s", strchr (file, '\n') ? quotef (file) : file);
  putchar ('\n');

  જો (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
સ્થિર સંરચના wc_લીટીઓ
wc_લીટીઓ (પૂર્ણાંક fd)
{
#ifdef USE_AVX512_WC_LINECOUNT
  સ્થિર signed અક્ષર use_avx512;
  જો (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  જો (0 < use_avx512)
    પાછા wc_લીટીઓ_avx512 (fd);
#endif
#ifdef USE_AVX2_WC_LINECOUNT
  સ્થિર signed અક્ષર use_avx2;
  જો (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  જો (0 < use_avx2)
    પાછા wc_લીટીઓ_avx2 (fd);
#endif
#ifdef USE_NEON_WC_LINECOUNT
  સ્થિર signed અક્ષર use_neon;
  જો (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  જો (0 < use_neon)
    પાછા wc_લીટીઓ_neon (fd);
#endif

  intmax_t લીટીઓ = 0, bytes = 0;
  bool long_લીટીઓ = false;

  જ્યારે (true)
    {
      અક્ષર buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = પઠ (fd, buf, IO_BUFSIZE);
      જો (bytes_read <= 0)
        પાછા (સંરચના wc_લીટીઓ) { bytes_read == 0 ? 0 : errno, લીટીઓ, bytes };

      bytes += bytes_read;
      અક્ષર *end = buf + bytes_read;
      idx_t buflines = 0;

      જો (! long_લીટીઓ)
        {
          /* Avoid function call overhead for shorter lines.  */
          માટે (અક્ષર *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      નહીંતર
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          માટે (અક્ષર *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_લીટીઓ = 15 * buflines <= bytes_read;
      લીટીઓ += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
સ્થિર bool
wc (પૂર્ણાંક fd, અક્ષર અચલ *file_x, સંરચના fstatus *fstatus)
{
  પૂર્ણાંક err = 0;
  અક્ષર buf[IO_BUFSIZE + 1];
  intmax_t લીટીઓ, શબ્દો, chars, bytes, linelength;
  bool ગણતરી_bytes, ગણતરી_chars, ગણતરી_complicated;
  અક્ષર અચલ *file = file_x ? file_x : _("standard input");

  લીટીઓ = શબ્દો = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  જો (MB_CUR_MAX > 1)
    {
      ગણતરી_bytes = print_bytes;
      ગણતરી_chars = print_chars;
    }
  નહીંતર
    {
      ગણતરી_bytes = print_bytes || print_chars;
      ગણતરી_chars = false;
    }
  ગણતરી_complicated = print_શબ્દો || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  જો (!ગણતરી_bytes || ગણતરી_chars || print_લીટીઓ || ગણતરી_complicated)
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

  જો (ગણતરી_bytes && !ગણતરી_chars && !print_લીટીઓ && !ગણતરી_complicated)
    {
      bool skip_read = false;

      જો (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      જો (! fstatus->failed && usable_st_size (&fstatus->st)
          && 0 <= fstatus->st.st_size)
        {
          off_t end_pos = fstatus->st.st_size;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          જો (current_pos < 0)
            ;
          નહીંતર જો (end_pos % page_size)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              જો (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          નહીંતર જો (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              જો (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          જો (0 < bytes && lseek (fd, bytes, SEEK_CUR) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      જો (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          માટે (ssize_t bytes_read;
               (bytes_read = પઠ (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            જો (bytes_read < 0)
              {
                err = errno;
                તોડો;
              }
        }
    }
  નહીંતર જો (!ગણતરી_chars && !ગણતરી_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      સંરચના wc_લીટીઓ w = wc_લીટીઓ (fd);
      err = w.err;
      લીટીઓ = w.લીટીઓ;
      bytes = w.bytes;
    }
  નહીંતર જો (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool ગણતરી_utf8 = !ગણતરી_complicated && !print_લીટીઓ
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      માટે (ssize_t bytes_read;
           ((bytes_read = પઠ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          જો (bytes_read < 0)
            {
              err = errno;
              તોડો;
            }

          bytes += bytes_read;
          અક્ષર અચલ *p = buf;
          અક્ષર અચલ *plim = p + prev + bytes_read;
          bool try_ગણતરી_utf8 = ગણતરી_utf8;
          કરો
            {
              જો (try_ગણતરી_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_ગણતરી_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_ગણતરી (p, &n);
                  p += n;
                  જો (p == plim)
                    તોડો;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              જો (!in_shift && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              નહીંતર
                {
                  idx_t prev_bytes = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  જો (scanbytes < n)
                    {
                      જો (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          in_shift = true;
                          તોડો;
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
                      શબ્દો += !in_word;
                      in_word = true;
                      ચાલુ;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              પસંદગી (wide_char)
                {
                સ્થિતિ '\n':
                  લીટીઓ++;
                  FALLTHROUGH;
                સ્થિતિ '\r':
                સ્થિતિ '\f':
                  જો (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  તોડો;

                સ્થિતિ '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  તોડો;

                સ્થિતિ ' ':
                  linepos++;
                  FALLTHROUGH;
                સ્થિતિ '\v':
                  in_word = false;
                  તોડો;

                મૂળભૂત:;
                  bool in_word2;
                  જો (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  નહીંતર
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      જો (print_linelength)
                        {
                          પૂર્ણાંક width = c32width (wide_char);
                          જો (width > 0)
                            linepos += width;
                        }
                      in_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  શબ્દો += !in_word & in_word2;
                  in_word = in_word2;
                  તોડો;
                }

              p += charbytes;
              chars++;
            }
          જ્યારે (p < plim);
        }
      જો (linepos > linelength)
        linelength = linepos;
    }
  નહીંતર
    {
      bool in_word = false;
      intmax_t linepos = 0;

      માટે (ssize_t bytes_read; (bytes_read = પઠ (fd, buf, IO_BUFSIZE)); )
        {
          જો (bytes_read < 0)
            {
              err = errno;
              તોડો;
            }

          bytes += bytes_read;
          અક્ષર અચલ *p = buf;
          કરો
            {
              unsigned અક્ષર c = *p++;
              પસંદગી (c)
                {
                સ્થિતિ '\n':
                  લીટીઓ++;
                  FALLTHROUGH;
                સ્થિતિ '\r':
                સ્થિતિ '\f':
                  જો (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  તોડો;

                સ્થિતિ '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  તોડો;

                સ્થિતિ ' ':
                  linepos++;
                  FALLTHROUGH;
                સ્થિતિ '\v':
                  in_word = false;
                  તોડો;

                મૂળભૂત:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  શબ્દો += !in_word & in_word2;
                  in_word = in_word2;
                  તોડો;
                }
            }
          જ્યારે (--bytes_read);
        }
      જો (linepos > linelength)
        linelength = linepos;
    }

  જો (ગણતરી_chars < print_chars)
    chars = bytes;

  જો (total_mode != total_only)
    write_counts (લીટીઓ, શબ્દો, chars, bytes, linelength, file_x);

  total_લીટીઓ_overflow |= ckd_add (&total_લીટીઓ, total_લીટીઓ, લીટીઓ);
  total_શબ્દો_overflow |= ckd_add (&total_શબ્દો, total_શબ્દો, શબ્દો);
  total_chars_overflow |= ckd_add (&total_chars, total_chars, chars);
  total_bytes_overflow |= ckd_add (&total_bytes, total_bytes, bytes);

  જો (linelength > max_લીટી_length)
    max_લીટી_length = linelength;

  જો (err)
    error (0, err, "%s", quotef (file));
  પાછા !err;
}

સ્થિર bool
wc_file (અક્ષર અચલ *file, સંરચના fstatus *fstatus)
{
  જો (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_દ્વિઅંકી_mode (STDIN_FILENO, O_દ્વિઅંકી);
      પાછા wc (STDIN_FILENO, file, fstatus);
    }
  નહીંતર
    {
      પૂર્ણાંક fd = open (file, O_RDONLY | O_દ્વિઅંકી);
      જો (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          પાછા false;
        }
      નહીંતર
        {
          bool ok = wc (fd, file, fstatus);
          જો (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              પાછા false;
            }
          પાછા ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

સ્થિર સંરચના fstatus *
get_આગત_fstatus (idx_t nfiles, અક્ષર *અચલ *file)
{
  સંરચના fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, કદ *fstatus);

  જો (nfiles == 0
      || (nfiles == 1
          && ((print_લીટીઓ + print_શબ્દો + print_chars
               + print_bytes + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  નહીંતર
    {
      માટે (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  પાછા fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_PURE
સ્થિર પૂર્ણાંક
compute_સંખ્યા_width (idx_t nfiles, સંરચના fstatus અચલ *fstatus)
{
  પૂર્ણાંક width = 1;

  જો (0 < nfiles && fstatus[0].failed <= 0)
    {
      પૂર્ણાંક minimum_width = 1;
      uintmax_t regular_total = 0;

      માટે (idx_t i = 0; i < nfiles; i++)
        જો (! fstatus[i].failed)
          {
            જો (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            નહીંતર જો (ckd_add (&regular_total, regular_total,
                              fstatus[i].st.st_size))
              {
                regular_total = UINTMAX_MAX;
                તોડો;
              }
          }

      માટે (; 10 <= regular_total; regular_total /= 10)
        width++;
      જો (width < minimum_width)
        width = minimum_width;
    }

  પાછા width;
}


પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક optc;
  idx_t nfiles;
  અક્ષર **files;
  અક્ષર *files_from = ખાલી;
  સંરચના fstatus *fstatus;
  સંરચના Tokens tok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  page_size = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ખાલી, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ખાલી);

  જ્યારે ((optc = getopt_long (argc, argv, "clLmw", longopts, ખાલી)) != -1)
    પસંદગી (optc)
      {
      સ્થિતિ 'c':
        print_bytes = true;
        તોડો;

      સ્થિતિ 'm':
        print_chars = true;
        તોડો;

      સ્થિતિ 'l':
        print_લીટીઓ = true;
        તોડો;

      સ્થિતિ 'w':
        print_શબ્દો = true;
        તોડો;

      સ્થિતિ 'L':
        print_linelength = true;
        તોડો;

      સ્થિતિ DEBUG_PROGRAM_OPTION:
        debug = true;
        તોડો;

      સ્થિતિ FILES0_FROM_OPTION:
        files_from = optarg;
        તોડો;

      સ્થિતિ TOTAL_OPTION:
        total_mode = XARGMATCH ("--total", optarg, total_args, total_types);
        તોડો;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      મૂળભૂત:
        usage (EXIT_FAILURE);
      }

  જો (! (print_લીટીઓ || print_શબ્દો || print_chars || print_bytes
         || print_linelength))
    print_લીટીઓ = print_શબ્દો = print_bytes = true;

  જો (print_linelength)
    માટે (પૂર્ણાંક i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  જો (print_શબ્દો)
    માટે (પૂર્ણાંક i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = isspace (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  સંરચના argv_iterator *ai;
  જો (files_from)
    {
      FILE *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      જો (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      જો (streq (files_from, "-"))
        stream = stdin;
      નહીંતર
        {
          stream = fopen (files_from, "r");
          જો (stream == ખાલી)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      સંરચના stat st;
      જો (fstat (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_size <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          જો (! readtokens0 (stream, &tok) || fclose (stream) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      નહીંતર
        {
          files = ખાલી;
          nfiles = 0;
          ai = argv_iter_init_stream (stream);
        }
    }
  નહીંતર
    {
      સ્થિર અક્ષર *stdin_only[] = { ખાલી };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  જો (!ai)
    xalloc_die ();

  fstatus = get_આગત_fstatus (nfiles, files);
  જો (total_mode == total_only)
    સંખ્યા_width = 1;  /* No extra padding, since no alignment requirement.  */
  નહીંતર
    સંખ્યા_width = compute_સંખ્યા_width (nfiles, fstatus);

  bool ok = true;
  ગણના argv_iter_err ai_err;
  અક્ષર *file_name;
  માટે (પૂર્ણાંક i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      જો (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      જો (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          જો (files_from == ખાલી)
            error (0, 0, "%s", _("invalid zero-length file name"));
          નહીંતર
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_args (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      જો (skip_file)
        ok = false;
      નહીંતર
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      જો (! nfiles)
        fstatus[0].failed = 1;
    }
  પસંદગી (ai_err)
    {
    સ્થિતિ AI_ERR_EOF:
      તોડો;

    સ્થિતિ AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      તોડો;

    સ્થિતિ AI_ERR_MEM:
      xalloc_die ();

    સ્થિતિ AI_ERR_OK: મૂળભૂત:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  જો (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (ખાલી, &fstatus[0]);

  જો (read_tokens)
    readtokens0_free (&tok);

  જો (total_mode != total_never
      && (total_mode != total_auto || 1 < argv_iter_n_args (ai)))
    {
      જો (total_લીટીઓ_overflow)
        {
          total_લીટીઓ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      જો (total_શબ્દો_overflow)
        {
          total_શબ્દો = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      જો (total_chars_overflow)
        {
          total_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      જો (total_bytes_overflow)
        {
          total_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (total_લીટીઓ, total_શબ્દો, total_chars, total_bytes,
                    max_લીટી_length,
                    total_mode != total_only ? _("total") : ખાલી);
    }

  argv_iter_free (ai);

  મુક્ત (fstatus);

  જો (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
