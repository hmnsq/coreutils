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

#ifdef USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#endif

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "cpu-supports.h"
#ضمن "ioblksize.h"
#ضمن "wc.h"
#ضمن "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "wc"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

ساكن bool wc_isprint[UCHAR_MAX + 1];
ساكن bool wc_isspace[UCHAR_MAX + 1];

ساكن bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
ساكن uintmax_t total_lines;
ساكن uintmax_t total_words;
ساكن uintmax_t total_chars;
ساكن uintmax_t total_bytes;
ساكن bool total_lines_overflow;
ساكن bool total_words_overflow;
ساكن bool total_chars_overflow;
ساكن bool total_bytes_overflow;
ساكن intmax_t max_line_length;

/* Which counts to print. */
ساكن bool print_lines, print_words, print_chars, print_bytes;
ساكن bool print_linelength;

/* The print width of each count.  */
ساكن صحيح number_width;

/* True if we have ever read the standard input. */
ساكن bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
ساكن idx_t page_size;

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
  DEBUG_PROGRAM_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  TOTAL_OPTION,
};

ساكن بنية option ثابت longopts[] =
{
  {"bytes", no_argument, عدم, 'c'},
  {"chars", no_argument, عدم, 'm'},
  {"lines", no_argument, عدم, 'l'},
  {"words", no_argument, عدم, 'w'},
  {"debug", no_argument, عدم, DEBUG_PROGRAM_OPTION},
  {"files0-from", required_argument, عدم, FILES0_FROM_OPTION},
  {"max-line-length", no_argument, عدم, 'L'},
  {"total", required_argument, عدم, TOTAL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

تعداد total_type
  {
    total_auto,         /* 0: default or --total=auto */
    total_always,       /* 1: --total=always */
    total_only,         /* 2: --total=only */
    total_never         /* 3: --total=never */
  };
ساكن حرف ثابت *ثابت total_args[] =
{
  "auto", "always", "only", "never", عدم
};
ساكن تعداد total_type ثابت total_types[] =
{
  total_auto, total_always, total_only, total_never
};
ARGMATCH_VERIFY (total_args, total_types);
ساكن تعداد total_type total_mode = total_auto;

#ifdef USE_AVX2_WC_LINECOUNT
ساكن bool
avx2_supported (فراغ)
{
  bool avx2_enabled = cpu_supports ("avx2");
  𐤀𐤌 (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  أرجع avx2_enabled;
}
#endif

#ifdef USE_AVX512_WC_LINECOUNT
ساكن bool
avx512_supported (فراغ)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  𐤀𐤌 (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  أرجع avx512_enabled;
}
#endif

#ifdef USE_NEON_WC_LINECOUNT
ساكن bool
neon_supported (فراغ)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  𐤀𐤌 (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  أرجع neon_enabled;
}
#endif

فراغ
usage (صحيح status)
{
  𐤀𐤌 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      fputs (_("\
Print newline, word, and byte counts لكل each FILE, and a total line 𐤀𐤌\n\
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
         اقرأ input from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, اقرأ names from standard input\n\
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
ساكن صحيح
maybe_c32isnbspace (char32_t wc)
{
  أرجع ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
ساكن فراغ
write_counts (uintmax_t lines,
              uintmax_t words,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              حرف ثابت *file)
{
  ساكن حرف ثابت format_sp_int[] = " %*s";
  حرف ثابت *format_int = format_sp_int + 1;
  حرف buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  𐤀𐤌 (print_lines)
    {
      printf (format_int, number_width, umaxtostr (lines, buf));
      format_int = format_sp_int;
    }
  𐤀𐤌 (print_words)
    {
      printf (format_int, number_width, umaxtostr (words, buf));
      format_int = format_sp_int;
    }
  𐤀𐤌 (print_chars)
    {
      printf (format_int, number_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  𐤀𐤌 (print_bytes)
    {
      printf (format_int, number_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  𐤀𐤌 (print_linelength)
    printf (format_int, number_width, imaxtostr (linelength, buf));
  𐤀𐤌 (file)
    printf (" %s", strchr (file, '\n') ? quotef (file) : file);
  putchar ('\n');

  𐤀𐤌 (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
ساكن بنية wc_lines
wc_lines (صحيح fd)
{
#ifdef USE_AVX512_WC_LINECOUNT
  ساكن signed حرف use_avx512;
  𐤀𐤌 (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  𐤀𐤌 (0 < use_avx512)
    أرجع wc_lines_avx512 (fd);
#endif
#ifdef USE_AVX2_WC_LINECOUNT
  ساكن signed حرف use_avx2;
  𐤀𐤌 (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  𐤀𐤌 (0 < use_avx2)
    أرجع wc_lines_avx2 (fd);
#endif
#ifdef USE_NEON_WC_LINECOUNT
  ساكن signed حرف use_neon;
  𐤀𐤌 (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  𐤀𐤌 (0 < use_neon)
    أرجع wc_lines_neon (fd);
#endif

  intmax_t lines = 0, bytes = 0;
  bool long_lines = false;

  طالما (true)
    {
      حرف buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = اقرأ (fd, buf, IO_BUFSIZE);
      𐤀𐤌 (bytes_read <= 0)
        أرجع (بنية wc_lines) { bytes_read == 0 ? 0 : errno, lines, bytes };

      bytes += bytes_read;
      حرف *end = buf + bytes_read;
      idx_t buflines = 0;

      𐤀𐤌 (! long_lines)
        {
          /* Avoid function call overhead for shorter lines.  */
          لكل (حرف *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      وإلا
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          لكل (حرف *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
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
ساكن bool
wc (صحيح fd, حرف ثابت *file_x, بنية fstatus *fstatus)
{
  صحيح err = 0;
  حرف buf[IO_BUFSIZE + 1];
  intmax_t lines, words, chars, bytes, linelength;
  bool count_bytes, count_chars, count_complicated;
  حرف ثابت *file = file_x ? file_x : _("standard input");

  lines = words = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  𐤀𐤌 (MB_CUR_MAX > 1)
    {
      count_bytes = print_bytes;
      count_chars = print_chars;
    }
  وإلا
    {
      count_bytes = print_bytes || print_chars;
      count_chars = false;
    }
  count_complicated = print_words || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  𐤀𐤌 (!count_bytes || count_chars || print_lines || count_complicated)
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

  𐤀𐤌 (count_bytes && !count_chars && !print_lines && !count_complicated)
    {
      bool skip_read = false;

      𐤀𐤌 (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      𐤀𐤌 (! fstatus->failed && usable_st_size (&fstatus->st)
          && 0 <= fstatus->st.st_size)
        {
          off_t end_pos = fstatus->st.st_size;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          𐤀𐤌 (current_pos < 0)
            ;
          وإلا 𐤀𐤌 (end_pos % page_size)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              𐤀𐤌 (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          وإلا 𐤀𐤌 (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              𐤀𐤌 (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          𐤀𐤌 (0 < bytes && lseek (fd, bytes, SEEK_CUR) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      𐤀𐤌 (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          لكل (ssize_t bytes_read;
               (bytes_read = اقرأ (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            𐤀𐤌 (bytes_read < 0)
              {
                err = errno;
                اكسر;
              }
        }
    }
  وإلا 𐤀𐤌 (!count_chars && !count_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      بنية wc_lines w = wc_lines (fd);
      err = w.err;
      lines = w.lines;
      bytes = w.bytes;
    }
  وإلا 𐤀𐤌 (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool count_utf8 = !count_complicated && !print_lines
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      لكل (ssize_t bytes_read;
           ((bytes_read = اقرأ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          𐤀𐤌 (bytes_read < 0)
            {
              err = errno;
              اكسر;
            }

          bytes += bytes_read;
          حرف ثابت *p = buf;
          حرف ثابت *plim = p + prev + bytes_read;
          bool try_count_utf8 = count_utf8;
          𐤐𐤏𐤋
            {
              𐤀𐤌 (try_count_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_count_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_count (p, &n);
                  p += n;
                  𐤀𐤌 (p == plim)
                    اكسر;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              𐤀𐤌 (!in_shift && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              وإلا
                {
                  idx_t prev_bytes = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  𐤀𐤌 (scanbytes < n)
                    {
                      𐤀𐤌 (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          in_shift = true;
                          اكسر;
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
                      استمر;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              بدل (wide_char)
                {
                حالة '\n':
                  lines++;
                  FALLTHROUGH;
                حالة '\r':
                حالة '\f':
                  𐤀𐤌 (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  اكسر;

                حالة '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  اكسر;

                حالة ' ':
                  linepos++;
                  FALLTHROUGH;
                حالة '\v':
                  in_word = false;
                  اكسر;

                افتراضي:;
                  bool in_word2;
                  𐤀𐤌 (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  وإلا
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      𐤀𐤌 (print_linelength)
                        {
                          صحيح width = c32width (wide_char);
                          𐤀𐤌 (width > 0)
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
                  اكسر;
                }

              p += charbytes;
              chars++;
            }
          طالما (p < plim);
        }
      𐤀𐤌 (linepos > linelength)
        linelength = linepos;
    }
  وإلا
    {
      bool in_word = false;
      intmax_t linepos = 0;

      لكل (ssize_t bytes_read; (bytes_read = اقرأ (fd, buf, IO_BUFSIZE)); )
        {
          𐤀𐤌 (bytes_read < 0)
            {
              err = errno;
              اكسر;
            }

          bytes += bytes_read;
          حرف ثابت *p = buf;
          𐤐𐤏𐤋
            {
              unsigned حرف c = *p++;
              بدل (c)
                {
                حالة '\n':
                  lines++;
                  FALLTHROUGH;
                حالة '\r':
                حالة '\f':
                  𐤀𐤌 (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  اكسر;

                حالة '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  اكسر;

                حالة ' ':
                  linepos++;
                  FALLTHROUGH;
                حالة '\v':
                  in_word = false;
                  اكسر;

                افتراضي:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  words += !in_word & in_word2;
                  in_word = in_word2;
                  اكسر;
                }
            }
          طالما (--bytes_read);
        }
      𐤀𐤌 (linepos > linelength)
        linelength = linepos;
    }

  𐤀𐤌 (count_chars < print_chars)
    chars = bytes;

  𐤀𐤌 (total_mode != total_only)
    write_counts (lines, words, chars, bytes, linelength, file_x);

  total_lines_overflow |= ckd_add (&total_lines, total_lines, lines);
  total_words_overflow |= ckd_add (&total_words, total_words, words);
  total_chars_overflow |= ckd_add (&total_chars, total_chars, chars);
  total_bytes_overflow |= ckd_add (&total_bytes, total_bytes, bytes);

  𐤀𐤌 (linelength > max_line_length)
    max_line_length = linelength;

  𐤀𐤌 (err)
    error (0, err, "%s", quotef (file));
  أرجع !err;
}

ساكن bool
wc_file (حرف ثابت *file, بنية fstatus *fstatus)
{
  𐤀𐤌 (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_binary_mode (STDIN_FILENO, O_BINARY);
      أرجع wc (STDIN_FILENO, file, fstatus);
    }
  وإلا
    {
      صحيح fd = open (file, O_RDONLY | O_BINARY);
      𐤀𐤌 (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          أرجع false;
        }
      وإلا
        {
          bool ok = wc (fd, file, fstatus);
          𐤀𐤌 (close (fd) != 0)
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
get_input_fstatus (idx_t nfiles, حرف *ثابت *file)
{
  بنية fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, حجم *fstatus);

  𐤀𐤌 (nfiles == 0
      || (nfiles == 1
          && ((print_lines + print_words + print_chars
               + print_bytes + print_linelength)
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

ATTRIBUTE_PURE
ساكن صحيح
compute_number_width (idx_t nfiles, بنية fstatus ثابت *fstatus)
{
  صحيح width = 1;

  𐤀𐤌 (0 < nfiles && fstatus[0].failed <= 0)
    {
      صحيح minimum_width = 1;
      uintmax_t regular_total = 0;

      لكل (idx_t i = 0; i < nfiles; i++)
        𐤀𐤌 (! fstatus[i].failed)
          {
            𐤀𐤌 (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            وإلا 𐤀𐤌 (ckd_add (&regular_total, regular_total,
                              fstatus[i].st.st_size))
              {
                regular_total = UINTMAX_MAX;
                اكسر;
              }
          }

      لكل (; 10 <= regular_total; regular_total /= 10)
        width++;
      𐤀𐤌 (width < minimum_width)
        width = minimum_width;
    }

  أرجع width;
}


صحيح
main (صحيح argc, حرف **argv)
{
  صحيح optc;
  idx_t nfiles;
  حرف **files;
  حرف *files_from = عدم;
  بنية fstatus *fstatus;
  بنية Tokens tok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  page_size = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, عدم, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);

  طالما ((optc = getopt_long (argc, argv, "clLmw", longopts, عدم)) != -1)
    بدل (optc)
      {
      حالة 'c':
        print_bytes = true;
        اكسر;

      حالة 'm':
        print_chars = true;
        اكسر;

      حالة 'l':
        print_lines = true;
        اكسر;

      حالة 'w':
        print_words = true;
        اكسر;

      حالة 'L':
        print_linelength = true;
        اكسر;

      حالة DEBUG_PROGRAM_OPTION:
        debug = true;
        اكسر;

      حالة FILES0_FROM_OPTION:
        files_from = optarg;
        اكسر;

      حالة TOTAL_OPTION:
        total_mode = XARGMATCH ("--total", optarg, total_args, total_types);
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      افتراضي:
        usage (EXIT_FAILURE);
      }

  𐤀𐤌 (! (print_lines || print_words || print_chars || print_bytes
         || print_linelength))
    print_lines = print_words = print_bytes = true;

  𐤀𐤌 (print_linelength)
    لكل (صحيح i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  𐤀𐤌 (print_words)
    لكل (صحيح i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = isspace (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  بنية argv_iterator *ai;
  𐤀𐤌 (files_from)
    {
      FILE *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      𐤀𐤌 (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      𐤀𐤌 (streq (files_from, "-"))
        stream = stdin;
      وإلا
        {
          stream = fopen (files_from, "r");
          𐤀𐤌 (stream == عدم)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      بنية stat st;
      𐤀𐤌 (fstat (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_size <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          𐤀𐤌 (! readtokens0 (stream, &tok) || fclose (stream) != 0)
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
          ai = argv_iter_init_stream (stream);
        }
    }
  وإلا
    {
      ساكن حرف *stdin_only[] = { عدم };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  𐤀𐤌 (!ai)
    xalloc_die ();

  fstatus = get_input_fstatus (nfiles, files);
  𐤀𐤌 (total_mode == total_only)
    number_width = 1;  /* No extra padding, since no alignment requirement.  */
  وإلا
    number_width = compute_number_width (nfiles, fstatus);

  bool ok = true;
  تعداد argv_iter_err ai_err;
  حرف *file_name;
  لكل (صحيح i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      𐤀𐤌 (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      𐤀𐤌 (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          𐤀𐤌 (files_from == عدم)
            error (0, 0, "%s", _("invalid zero-length file name"));
          وإلا
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_args (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      𐤀𐤌 (skip_file)
        ok = false;
      وإلا
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      𐤀𐤌 (! nfiles)
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
  𐤀𐤌 (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (عدم, &fstatus[0]);

  𐤀𐤌 (read_tokens)
    readtokens0_free (&tok);

  𐤀𐤌 (total_mode != total_never
      && (total_mode != total_auto || 1 < argv_iter_n_args (ai)))
    {
      𐤀𐤌 (total_lines_overflow)
        {
          total_lines = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      𐤀𐤌 (total_words_overflow)
        {
          total_words = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      𐤀𐤌 (total_chars_overflow)
        {
          total_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      𐤀𐤌 (total_bytes_overflow)
        {
          total_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (total_lines, total_words, total_chars, total_bytes,
                    max_line_length,
                    total_mode != total_only ? _("total") : عدم);
    }

  argv_iter_free (ai);

  حرر (fstatus);

  𐤀𐤌 (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
