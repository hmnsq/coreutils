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

#include <config.h>

#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>

#include <argmatch.h>
#include <argv-iter.h>
#include <fadvise.h>
#include <physmem.h>
#include <readtokens0.h>
#include <stat-size.h>
#include <xbinary-io.h>

#ifdef USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#endif

#include "system.h"
#include "assure.h"
#include "cpu-supports.h"
#include "ioblksize.h"
#include "wc.h"
#include "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "wc"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

static bool wc_isprint[UCHAR_MAX + 1];
static bool wc_isspace[UCHAR_MAX + 1];

static bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
static uintmax_t total_lines;
static uintmax_t total_words;
static uintmax_t total_chars;
static uintmax_t total_bytes;
static bool total_lines_overflow;
static bool total_words_overflow;
static bool total_chars_overflow;
static bool total_bytes_overflow;
static intmax_t max_line_length;

/* Which counts to print. */
static bool print_lines, print_words, print_chars, print_bytes;
static bool print_linelength;

/* The print width of each count.  */
static puur_naa_nka number_width;

/* True if we have ever read the standard input. */
static bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
static idx_t page_size;

/* Enable to _not_ treat non breaking space as a word separator.  */
static bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
sa_nracanaa fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  puur_naa_nka failed;

  /* If FAILED is zero, this is the file's status.  */
  sa_nracanaa stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  DEBUG_PROGRAM_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  TOTAL_OPTION,
};

static sa_nracanaa option const longopts[] =
{
  {"bytes", no_argument, NULL, 'c'},
  {"chars", no_argument, NULL, 'm'},
  {"lines", no_argument, NULL, 'l'},
  {"words", no_argument, NULL, 'w'},
  {"debug", no_argument, NULL, DEBUG_PROGRAM_OPTION},
  {"files0-from", required_argument, NULL, FILES0_FROM_OPTION},
  {"max-line-length", no_argument, NULL, 'L'},
  {"total", required_argument, NULL, TOTAL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

ga_nanaa total_type
  {
    total_auto,         /* 0: default or --total=auto */
    total_always,       /* 1: --total=always */
    total_only,         /* 2: --total=only */
    total_never         /* 3: --total=never */
  };
static var_na const *const total_args[] =
{
  "auto", "always", "only", "never", NULL
};
static ga_nanaa total_type const total_types[] =
{
  total_auto, total_always, total_only, total_never
};
ARGMATCH_VERIFY (total_args, total_types);
static ga_nanaa total_type total_mode = total_auto;

#ifdef USE_AVX2_WC_LINECOUNT
static bool
avx2_supported (void)
{
  bool avx2_enabled = cpu_supports ("avx2");
  ᰀᰧ (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  ᰀᰧᰌ avx2_enabled;
}
#endif

#ifdef USE_AVX512_WC_LINECOUNT
static bool
avx512_supported (void)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  ᰀᰧ (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  ᰀᰧᰌ avx512_enabled;
}
#endif

#ifdef USE_NEON_WC_LINECOUNT
static bool
neon_supported (void)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  ᰀᰧ (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  ᰀᰧᰌ neon_enabled;
}
#endif

void
usage (puur_naa_nka status)
{
  ᰀᰧ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᰀᰧᰀ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      fputs (_("\
Print newline, word, and byte counts for each FILE, and a total line ᰀᰧ\n\
more than one FILE is specified.  A word is a nonempty sequence of non white\n\
space delimited by white space characters or by start or end of input.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum line length.\n\
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
  -l, --lines\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what line count acceleration is used\n\
"));
      oputs (_("\
      --files0-from=F\n\
         read input from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, read names from standard input\n\
"));
      oputs (_("\
  -L, --max-line-length\n\
         print the maximum display width\n\
"));
      oputs (_("\
  -w, --words\n\
         print the word counts\n\
"));
      oputs (_("\
      --total=WHEN\n\
         when to print a line with total counts;\n\
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
static puur_naa_nka
maybe_c32isnbspace (char32_t wc)
{
  ᰀᰧᰌ ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
static void
write_counts (uintmax_t lines,
              uintmax_t words,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              var_na const *file)
{
  static var_na const format_sp_int[] = " %*s";
  var_na const *format_int = format_sp_int + 1;
  var_na buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  ᰀᰧ (print_lines)
    {
      printf (format_int, number_width, umaxtostr (lines, buf));
      format_int = format_sp_int;
    }
  ᰀᰧ (print_words)
    {
      printf (format_int, number_width, umaxtostr (words, buf));
      format_int = format_sp_int;
    }
  ᰀᰧ (print_chars)
    {
      printf (format_int, number_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  ᰀᰧ (print_bytes)
    {
      printf (format_int, number_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  ᰀᰧ (print_linelength)
    printf (format_int, number_width, imaxtostr (linelength, buf));
  ᰀᰧ (file)
    printf (" %s", strchr (file, '\n') ? quotef (file) : file);
  putchar ('\n');

  ᰀᰧ (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
static sa_nracanaa wc_lines
wc_lines (puur_naa_nka fd)
{
#ifdef USE_AVX512_WC_LINECOUNT
  static signed var_na use_avx512;
  ᰀᰧ (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  ᰀᰧ (0 < use_avx512)
    ᰀᰧᰌ wc_lines_avx512 (fd);
#endif
#ifdef USE_AVX2_WC_LINECOUNT
  static signed var_na use_avx2;
  ᰀᰧ (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  ᰀᰧ (0 < use_avx2)
    ᰀᰧᰌ wc_lines_avx2 (fd);
#endif
#ifdef USE_NEON_WC_LINECOUNT
  static signed var_na use_neon;
  ᰀᰧ (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  ᰀᰧ (0 < use_neon)
    ᰀᰧᰌ wc_lines_neon (fd);
#endif

  intmax_t lines = 0, bytes = 0;
  bool long_lines = false;

  ᰀᰬ (true)
    {
      var_na buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = read (fd, buf, IO_BUFSIZE);
      ᰀᰧ (bytes_read <= 0)
        ᰀᰧᰌ (sa_nracanaa wc_lines) { bytes_read == 0 ? 0 : errno, lines, bytes };

      bytes += bytes_read;
      var_na *end = buf + bytes_read;
      idx_t buflines = 0;

      ᰀᰧ (! long_lines)
        {
          /* Avoid function call overhead for shorter lines.  */
          for (var_na *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      ᰀᰧᰀ
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          for (var_na *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_lines = 15 * buflines <= bytes_read;
      lines += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
static bool
wc (puur_naa_nka fd, var_na const *file_x, sa_nracanaa fstatus *fstatus)
{
  puur_naa_nka err = 0;
  var_na buf[IO_BUFSIZE + 1];
  intmax_t lines, words, chars, bytes, linelength;
  bool count_bytes, count_chars, count_complicated;
  var_na const *file = file_x ? file_x : _("standard input");

  lines = words = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  ᰀᰧ (MB_CUR_MAX > 1)
    {
      count_bytes = print_bytes;
      count_chars = print_chars;
    }
  ᰀᰧᰀ
    {
      count_bytes = print_bytes || print_chars;
      count_chars = false;
    }
  count_complicated = print_words || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  ᰀᰧ (!count_bytes || count_chars || print_lines || count_complicated)
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

  ᰀᰧ (count_bytes && !count_chars && !print_lines && !count_complicated)
    {
      bool skip_read = false;

      ᰀᰧ (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      ᰀᰧ (! fstatus->failed && usable_st_size (&fstatus->st)
          && 0 <= fstatus->st.st_size)
        {
          off_t end_pos = fstatus->st.st_size;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          ᰀᰧ (current_pos < 0)
            ;
          ᰀᰧᰀ ᰀᰧ (end_pos % page_size)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              ᰀᰧ (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          ᰀᰧᰀ ᰀᰧ (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              ᰀᰧ (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          ᰀᰧ (0 < bytes && lseek (fd, bytes, SEEK_CUR) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      ᰀᰧ (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          for (ssize_t bytes_read;
               (bytes_read = read (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            ᰀᰧ (bytes_read < 0)
              {
                err = errno;
                bha_nga;
              }
        }
    }
  ᰀᰧᰀ ᰀᰧ (!count_chars && !count_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      sa_nracanaa wc_lines w = wc_lines (fd);
      err = w.err;
      lines = w.lines;
      bytes = w.bytes;
    }
  ᰀᰧᰀ ᰀᰧ (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool count_utf8 = !count_complicated && !print_lines
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      for (ssize_t bytes_read;
           ((bytes_read = read (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          ᰀᰧ (bytes_read < 0)
            {
              err = errno;
              bha_nga;
            }

          bytes += bytes_read;
          var_na const *p = buf;
          var_na const *plim = p + prev + bytes_read;
          bool try_count_utf8 = count_utf8;
          do
            {
              ᰀᰧ (try_count_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_count_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_count (p, &n);
                  p += n;
                  ᰀᰧ (p == plim)
                    bha_nga;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              ᰀᰧ (!in_shift && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              ᰀᰧᰀ
                {
                  idx_t prev_bytes = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  ᰀᰧ (scanbytes < n)
                    {
                      ᰀᰧ (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          in_shift = true;
                          bha_nga;
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
                      words += !in_word;
                      in_word = true;
                      continue;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              switch (wide_char)
                {
                pak_sa '\n':
                  lines++;
                  FALLTHROUGH;
                pak_sa '\r':
                pak_sa '\f':
                  ᰀᰧ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  bha_nga;

                pak_sa '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  bha_nga;

                pak_sa ' ':
                  linepos++;
                  FALLTHROUGH;
                pak_sa '\v':
                  in_word = false;
                  bha_nga;

                default:;
                  bool in_word2;
                  ᰀᰧ (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  ᰀᰧᰀ
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      ᰀᰧ (print_linelength)
                        {
                          puur_naa_nka width = c32width (wide_char);
                          ᰀᰧ (width > 0)
                            linepos += width;
                        }
                      in_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  words += !in_word & in_word2;
                  in_word = in_word2;
                  bha_nga;
                }

              p += charbytes;
              chars++;
            }
          ᰀᰬ (p < plim);
        }
      ᰀᰧ (linepos > linelength)
        linelength = linepos;
    }
  ᰀᰧᰀ
    {
      bool in_word = false;
      intmax_t linepos = 0;

      for (ssize_t bytes_read; (bytes_read = read (fd, buf, IO_BUFSIZE)); )
        {
          ᰀᰧ (bytes_read < 0)
            {
              err = errno;
              bha_nga;
            }

          bytes += bytes_read;
          var_na const *p = buf;
          do
            {
              unsigned var_na c = *p++;
              switch (c)
                {
                pak_sa '\n':
                  lines++;
                  FALLTHROUGH;
                pak_sa '\r':
                pak_sa '\f':
                  ᰀᰧ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  bha_nga;

                pak_sa '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  bha_nga;

                pak_sa ' ':
                  linepos++;
                  FALLTHROUGH;
                pak_sa '\v':
                  in_word = false;
                  bha_nga;

                default:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  words += !in_word & in_word2;
                  in_word = in_word2;
                  bha_nga;
                }
            }
          ᰀᰬ (--bytes_read);
        }
      ᰀᰧ (linepos > linelength)
        linelength = linepos;
    }

  ᰀᰧ (count_chars < print_chars)
    chars = bytes;

  ᰀᰧ (total_mode != total_only)
    write_counts (lines, words, chars, bytes, linelength, file_x);

  total_lines_overflow |= ckd_add (&total_lines, total_lines, lines);
  total_words_overflow |= ckd_add (&total_words, total_words, words);
  total_chars_overflow |= ckd_add (&total_chars, total_chars, chars);
  total_bytes_overflow |= ckd_add (&total_bytes, total_bytes, bytes);

  ᰀᰧ (linelength > max_line_length)
    max_line_length = linelength;

  ᰀᰧ (err)
    error (0, err, "%s", quotef (file));
  ᰀᰧᰌ !err;
}

static bool
wc_file (var_na const *file, sa_nracanaa fstatus *fstatus)
{
  ᰀᰧ (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_binary_mode (STDIN_FILENO, O_BINARY);
      ᰀᰧᰌ wc (STDIN_FILENO, file, fstatus);
    }
  ᰀᰧᰀ
    {
      puur_naa_nka fd = open (file, O_RDONLY | O_BINARY);
      ᰀᰧ (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          ᰀᰧᰌ false;
        }
      ᰀᰧᰀ
        {
          bool ok = wc (fd, file, fstatus);
          ᰀᰧ (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              ᰀᰧᰌ false;
            }
          ᰀᰧᰌ ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

static sa_nracanaa fstatus *
get_input_fstatus (idx_t nfiles, var_na *const *file)
{
  sa_nracanaa fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, parimaa_na *fstatus);

  ᰀᰧ (nfiles == 0
      || (nfiles == 1
          && ((print_lines + print_words + print_chars
               + print_bytes + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  ᰀᰧᰀ
    {
      for (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  ᰀᰧᰌ fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_PURE
static puur_naa_nka
compute_number_width (idx_t nfiles, sa_nracanaa fstatus const *fstatus)
{
  puur_naa_nka width = 1;

  ᰀᰧ (0 < nfiles && fstatus[0].failed <= 0)
    {
      puur_naa_nka minimum_width = 1;
      uintmax_t regular_total = 0;

      for (idx_t i = 0; i < nfiles; i++)
        ᰀᰧ (! fstatus[i].failed)
          {
            ᰀᰧ (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            ᰀᰧᰀ ᰀᰧ (ckd_add (&regular_total, regular_total,
                              fstatus[i].st.st_size))
              {
                regular_total = UINTMAX_MAX;
                bha_nga;
              }
          }

      for (; 10 <= regular_total; regular_total /= 10)
        width++;
      ᰀᰧ (width < minimum_width)
        width = minimum_width;
    }

  ᰀᰧᰌ width;
}


puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  puur_naa_nka optc;
  idx_t nfiles;
  var_na **files;
  var_na *files_from = NULL;
  sa_nracanaa fstatus *fstatus;
  sa_nracanaa Tokens tok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  page_size = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, NULL, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != NULL);

  ᰀᰬ ((optc = getopt_long (argc, argv, "clLmw", longopts, NULL)) != -1)
    switch (optc)
      {
      pak_sa 'c':
        print_bytes = true;
        bha_nga;

      pak_sa 'm':
        print_chars = true;
        bha_nga;

      pak_sa 'l':
        print_lines = true;
        bha_nga;

      pak_sa 'w':
        print_words = true;
        bha_nga;

      pak_sa 'L':
        print_linelength = true;
        bha_nga;

      pak_sa DEBUG_PROGRAM_OPTION:
        debug = true;
        bha_nga;

      pak_sa FILES0_FROM_OPTION:
        files_from = optarg;
        bha_nga;

      pak_sa TOTAL_OPTION:
        total_mode = XARGMATCH ("--total", optarg, total_args, total_types);
        bha_nga;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      default:
        usage (EXIT_FAILURE);
      }

  ᰀᰧ (! (print_lines || print_words || print_chars || print_bytes
         || print_linelength))
    print_lines = print_words = print_bytes = true;

  ᰀᰧ (print_linelength)
    for (puur_naa_nka i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  ᰀᰧ (print_words)
    for (puur_naa_nka i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = isspace (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  sa_nracanaa argv_iterator *ai;
  ᰀᰧ (files_from)
    {
      FILE *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ᰀᰧ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      ᰀᰧ (streq (files_from, "-"))
        stream = stdin;
      ᰀᰧᰀ
        {
          stream = fopen (files_from, "r");
          ᰀᰧ (stream == NULL)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      sa_nracanaa stat st;
      ᰀᰧ (fstat (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_size <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          ᰀᰧ (! readtokens0 (stream, &tok) || fclose (stream) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      ᰀᰧᰀ
        {
          files = NULL;
          nfiles = 0;
          ai = argv_iter_init_stream (stream);
        }
    }
  ᰀᰧᰀ
    {
      static var_na *stdin_only[] = { NULL };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  ᰀᰧ (!ai)
    xalloc_die ();

  fstatus = get_input_fstatus (nfiles, files);
  ᰀᰧ (total_mode == total_only)
    number_width = 1;  /* No extra padding, since no alignment requirement.  */
  ᰀᰧᰀ
    number_width = compute_number_width (nfiles, fstatus);

  bool ok = true;
  ga_nanaa argv_iter_err ai_err;
  var_na *file_name;
  for (puur_naa_nka i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      ᰀᰧ (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      ᰀᰧ (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          ᰀᰧ (files_from == NULL)
            error (0, 0, "%s", _("invalid zero-length file name"));
          ᰀᰧᰀ
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_args (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      ᰀᰧ (skip_file)
        ok = false;
      ᰀᰧᰀ
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      ᰀᰧ (! nfiles)
        fstatus[0].failed = 1;
    }
  switch (ai_err)
    {
    pak_sa AI_ERR_EOF:
      bha_nga;

    pak_sa AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      bha_nga;

    pak_sa AI_ERR_MEM:
      xalloc_die ();

    pak_sa AI_ERR_OK: default:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  ᰀᰧ (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (NULL, &fstatus[0]);

  ᰀᰧ (read_tokens)
    readtokens0_free (&tok);

  ᰀᰧ (total_mode != total_never
      && (total_mode != total_auto || 1 < argv_iter_n_args (ai)))
    {
      ᰀᰧ (total_lines_overflow)
        {
          total_lines = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      ᰀᰧ (total_words_overflow)
        {
          total_words = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      ᰀᰧ (total_chars_overflow)
        {
          total_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      ᰀᰧ (total_bytes_overflow)
        {
          total_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (total_lines, total_words, total_chars, total_bytes,
                    max_line_length,
                    total_mode != total_only ? _("total") : NULL);
    }

  argv_iter_free (ai);

  free (fstatus);

  ᰀᰧ (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  ᰀᰧᰌ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
