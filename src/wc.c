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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ <argmatch.h>
#ಅನ್ತರ್ಭಾವ <argv-iter.h>
#ಅನ್ತರ್ಭಾವ <fadvise.h>
#ಅನ್ತರ್ಭಾವ <physmem.h>
#ಅನ್ತರ್ಭಾವ <readtokens0.h>
#ಅನ್ತರ್ಭಾವ <stat-size.h>
#ಅನ್ತರ್ಭಾವ <xbinary-io.h>

#ifdef USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#endif

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "cpu-supports.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "wc.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "wc"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

ಸ್ಥಿರ bool wc_isprint[UCHAR_MAX + 1];
ಸ್ಥಿರ bool wc_isspace[UCHAR_MAX + 1];

ಸ್ಥಿರ bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
ಸ್ಥಿರ uintmax_t total_ಸಾಲುಗಳು;
ಸ್ಥಿರ uintmax_t total_ಪದಗಳು;
ಸ್ಥಿರ uintmax_t total_chars;
ಸ್ಥಿರ uintmax_t total_bytes;
ಸ್ಥಿರ bool total_ಸಾಲುಗಳು_overflow;
ಸ್ಥಿರ bool total_ಪದಗಳು_overflow;
ಸ್ಥಿರ bool total_chars_overflow;
ಸ್ಥಿರ bool total_bytes_overflow;
ಸ್ಥಿರ intmax_t max_ಸಾಲು_length;

/* Which counts to print. */
ಸ್ಥಿರ bool print_ಸಾಲುಗಳು, print_ಪದಗಳು, print_chars, print_bytes;
ಸ್ಥಿರ bool print_linelength;

/* The print width of each count.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಸಂಖ್ಯೆ_width;

/* True if we have ever read the standard input. */
ಸ್ಥಿರ bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
ಸ್ಥಿರ idx_t page_size;

/* Enable to _not_ treat non breaking space as a word separator.  */
ಸ್ಥಿರ bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
ಸಂರಚನಾ fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  ಪೂರ್ಣಾಙ್ಕ failed;

  /* If FAILED is zero, this is the file's status.  */
  ಸಂರಚನಾ stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  DEBUG_PROGRAM_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  TOTAL_OPTION,
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"bytes", no_argument, ರಿಕ್ತ, 'c'},
  {"chars", no_argument, ರಿಕ್ತ, 'm'},
  {"lines", no_argument, ರಿಕ್ತ, 'l'},
  {"words", no_argument, ರಿಕ್ತ, 'w'},
  {"debug", no_argument, ರಿಕ್ತ, DEBUG_PROGRAM_OPTION},
  {"files0-from", required_argument, ರಿಕ್ತ, FILES0_FROM_OPTION},
  {"max-line-length", no_argument, ರಿಕ್ತ, 'L'},
  {"total", required_argument, ರಿಕ್ತ, TOTAL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಗಣನಾ total_type
  {
    total_auto,         /* 0: default or --total=auto */
    total_always,       /* 1: --total=always */
    total_only,         /* 2: --total=only */
    total_never         /* 3: --total=never */
  };
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ total_args[] =
{
  "auto", "always", "only", "never", ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ total_type ಧ್ರುವ total_types[] =
{
  total_auto, total_always, total_only, total_never
};
ARGMATCH_VERIFY (total_args, total_types);
ಸ್ಥಿರ ಗಣನಾ total_type total_mode = total_auto;

#ifdef USE_AVX2_WC_LINECOUNT
ಸ್ಥಿರ bool
avx2_supported (ಶೂನ್ಯ)
{
  bool avx2_enabled = cpu_supports ("avx2");
  ಯದಿ (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  ಪ್ರತ್ಯಾಗಮ avx2_enabled;
}
#endif

#ifdef USE_AVX512_WC_LINECOUNT
ಸ್ಥಿರ bool
avx512_supported (ಶೂನ್ಯ)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  ಯದಿ (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  ಪ್ರತ್ಯಾಗಮ avx512_enabled;
}
#endif

#ifdef USE_NEON_WC_LINECOUNT
ಸ್ಥಿರ bool
neon_supported (ಶೂನ್ಯ)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  ಯದಿ (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  ಪ್ರತ್ಯಾಗಮ neon_enabled;
}
#endif

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      fputs (_("\
Print newline, word, and byte counts ಪ್ರತಿ each FILE, and a total ಸಾಲು ಯದಿ\n\
more than one FILE is specified.  A word is a nonempty sequence of non white\n\
space delimited by white space characters or by start or end of ಒಳಹರಿವು.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum ಸಾಲು length.\n\
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
  -l, --ಸಾಲುಗಳು\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what ಸಾಲು ಎಣಿಕೆ acceleration is used\n\
"));
      oputs (_("\
      --files0-from=F\n\
         ಪಠ ಒಳಹರಿವು from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, ಪಠ names from standard ಒಳಹರಿವು\n\
"));
      oputs (_("\
  -L, --max-ಸಾಲು-length\n\
         print the maximum display width\n\
"));
      oputs (_("\
  -w, --ಪದಗಳು\n\
         print the word counts\n\
"));
      oputs (_("\
      --total=WHEN\n\
         when to print a ಸಾಲು with total counts;\n\
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
maybe_c32isnbspace (char32_t wc)
{
  ಪ್ರತ್ಯಾಗಮ ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
ಸ್ಥಿರ ಶೂನ್ಯ
write_counts (uintmax_t ಸಾಲುಗಳು,
              uintmax_t ಪದಗಳು,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              ವರ್ಣ ಧ್ರುವ *file)
{
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ format_sp_int[] = " %*s";
  ವರ್ಣ ಧ್ರುವ *format_int = format_sp_int + 1;
  ವರ್ಣ buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  ಯದಿ (print_ಸಾಲುಗಳು)
    {
      printf (format_int, ಸಂಖ್ಯೆ_width, umaxtostr (ಸಾಲುಗಳು, buf));
      format_int = format_sp_int;
    }
  ಯದಿ (print_ಪದಗಳು)
    {
      printf (format_int, ಸಂಖ್ಯೆ_width, umaxtostr (ಪದಗಳು, buf));
      format_int = format_sp_int;
    }
  ಯದಿ (print_chars)
    {
      printf (format_int, ಸಂಖ್ಯೆ_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  ಯದಿ (print_bytes)
    {
      printf (format_int, ಸಂಖ್ಯೆ_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  ಯದಿ (print_linelength)
    printf (format_int, ಸಂಖ್ಯೆ_width, imaxtostr (linelength, buf));
  ಯದಿ (file)
    printf (" %s", strchr (file, '\n') ? quotef (file) : file);
  putchar ('\n');

  ಯದಿ (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
ಸ್ಥಿರ ಸಂರಚನಾ wc_ಸಾಲುಗಳು
wc_ಸಾಲುಗಳು (ಪೂರ್ಣಾಙ್ಕ fd)
{
#ifdef USE_AVX512_WC_LINECOUNT
  ಸ್ಥಿರ signed ವರ್ಣ use_avx512;
  ಯದಿ (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  ಯದಿ (0 < use_avx512)
    ಪ್ರತ್ಯಾಗಮ wc_ಸಾಲುಗಳು_avx512 (fd);
#endif
#ifdef USE_AVX2_WC_LINECOUNT
  ಸ್ಥಿರ signed ವರ್ಣ use_avx2;
  ಯದಿ (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  ಯದಿ (0 < use_avx2)
    ಪ್ರತ್ಯಾಗಮ wc_ಸಾಲುಗಳು_avx2 (fd);
#endif
#ifdef USE_NEON_WC_LINECOUNT
  ಸ್ಥಿರ signed ವರ್ಣ use_neon;
  ಯದಿ (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  ಯದಿ (0 < use_neon)
    ಪ್ರತ್ಯಾಗಮ wc_ಸಾಲುಗಳು_neon (fd);
#endif

  intmax_t ಸಾಲುಗಳು = 0, bytes = 0;
  bool long_ಸಾಲುಗಳು = false;

  ಯಾವತ್ (true)
    {
      ವರ್ಣ buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = ಪಠ (fd, buf, IO_BUFSIZE);
      ಯದಿ (bytes_read <= 0)
        ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ wc_ಸಾಲುಗಳು) { bytes_read == 0 ? 0 : errno, ಸಾಲುಗಳು, bytes };

      bytes += bytes_read;
      ವರ್ಣ *end = buf + bytes_read;
      idx_t buflines = 0;

      ಯದಿ (! long_ಸಾಲುಗಳು)
        {
          /* Avoid function call overhead for shorter lines.  */
          ಪ್ರತಿ (ವರ್ಣ *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      ಅನ್ಯಥಾ
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          ಪ್ರತಿ (ವರ್ಣ *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_ಸಾಲುಗಳು = 15 * buflines <= bytes_read;
      ಸಾಲುಗಳು += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
ಸ್ಥಿರ bool
wc (ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ ಧ್ರುವ *file_x, ಸಂರಚನಾ fstatus *fstatus)
{
  ಪೂರ್ಣಾಙ್ಕ err = 0;
  ವರ್ಣ buf[IO_BUFSIZE + 1];
  intmax_t ಸಾಲುಗಳು, ಪದಗಳು, chars, bytes, linelength;
  bool ಎಣಿಕೆ_bytes, ಎಣಿಕೆ_chars, ಎಣಿಕೆ_complicated;
  ವರ್ಣ ಧ್ರುವ *file = file_x ? file_x : _("standard input");

  ಸಾಲುಗಳು = ಪದಗಳು = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  ಯದಿ (MB_CUR_MAX > 1)
    {
      ಎಣಿಕೆ_bytes = print_bytes;
      ಎಣಿಕೆ_chars = print_chars;
    }
  ಅನ್ಯಥಾ
    {
      ಎಣಿಕೆ_bytes = print_bytes || print_chars;
      ಎಣಿಕೆ_chars = false;
    }
  ಎಣಿಕೆ_complicated = print_ಪದಗಳು || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  ಯದಿ (!ಎಣಿಕೆ_bytes || ಎಣಿಕೆ_chars || print_ಸಾಲುಗಳು || ಎಣಿಕೆ_complicated)
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

  ಯದಿ (ಎಣಿಕೆ_bytes && !ಎಣಿಕೆ_chars && !print_ಸಾಲುಗಳು && !ಎಣಿಕೆ_complicated)
    {
      bool skip_read = false;

      ಯದಿ (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      ಯದಿ (! fstatus->failed && usable_st_size (&fstatus->st)
          && 0 <= fstatus->st.st_size)
        {
          off_t end_pos = fstatus->st.st_size;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          ಯದಿ (current_pos < 0)
            ;
          ಅನ್ಯಥಾ ಯದಿ (end_pos % page_size)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              ಯದಿ (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          ಅನ್ಯಥಾ ಯದಿ (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              ಯದಿ (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          ಯದಿ (0 < bytes && lseek (fd, bytes, SEEK_CUR) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      ಯದಿ (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          ಪ್ರತಿ (ssize_t bytes_read;
               (bytes_read = ಪಠ (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            ಯದಿ (bytes_read < 0)
              {
                err = errno;
                ಭಙ್ಗ;
              }
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (!ಎಣಿಕೆ_chars && !ಎಣಿಕೆ_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      ಸಂರಚನಾ wc_ಸಾಲುಗಳು w = wc_ಸಾಲುಗಳು (fd);
      err = w.err;
      ಸಾಲುಗಳು = w.ಸಾಲುಗಳು;
      bytes = w.bytes;
    }
  ಅನ್ಯಥಾ ಯದಿ (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool ಎಣಿಕೆ_utf8 = !ಎಣಿಕೆ_complicated && !print_ಸಾಲುಗಳು
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      ಪ್ರತಿ (ssize_t bytes_read;
           ((bytes_read = ಪಠ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          ಯದಿ (bytes_read < 0)
            {
              err = errno;
              ಭಙ್ಗ;
            }

          bytes += bytes_read;
          ವರ್ಣ ಧ್ರುವ *p = buf;
          ವರ್ಣ ಧ್ರುವ *plim = p + prev + bytes_read;
          bool try_ಎಣಿಕೆ_utf8 = ಎಣಿಕೆ_utf8;
          ಕುರು
            {
              ಯದಿ (try_ಎಣಿಕೆ_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_ಎಣಿಕೆ_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_ಎಣಿಕೆ (p, &n);
                  p += n;
                  ಯದಿ (p == plim)
                    ಭಙ್ಗ;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              ಯದಿ (!in_shift && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              ಅನ್ಯಥಾ
                {
                  idx_t prev_bytes = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  ಯದಿ (scanbytes < n)
                    {
                      ಯದಿ (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          in_shift = true;
                          ಭಙ್ಗ;
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
                      ಪದಗಳು += !in_word;
                      in_word = true;
                      ಅನುವರ್ತ;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              ವಿಕಲ್ಪ (wide_char)
                {
                ಪಕ್ಷ '\n':
                  ಸಾಲುಗಳು++;
                  FALLTHROUGH;
                ಪಕ್ಷ '\r':
                ಪಕ್ಷ '\f':
                  ಯದಿ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  ಭಙ್ಗ;

                ಪಕ್ಷ '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  ಭಙ್ಗ;

                ಪಕ್ಷ ' ':
                  linepos++;
                  FALLTHROUGH;
                ಪಕ್ಷ '\v':
                  in_word = false;
                  ಭಙ್ಗ;

                ಸ್ವಭಾವ:;
                  bool in_word2;
                  ಯದಿ (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  ಅನ್ಯಥಾ
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      ಯದಿ (print_linelength)
                        {
                          ಪೂರ್ಣಾಙ್ಕ width = c32width (wide_char);
                          ಯದಿ (width > 0)
                            linepos += width;
                        }
                      in_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  ಪದಗಳು += !in_word & in_word2;
                  in_word = in_word2;
                  ಭಙ್ಗ;
                }

              p += charbytes;
              chars++;
            }
          ಯಾವತ್ (p < plim);
        }
      ಯದಿ (linepos > linelength)
        linelength = linepos;
    }
  ಅನ್ಯಥಾ
    {
      bool in_word = false;
      intmax_t linepos = 0;

      ಪ್ರತಿ (ssize_t bytes_read; (bytes_read = ಪಠ (fd, buf, IO_BUFSIZE)); )
        {
          ಯದಿ (bytes_read < 0)
            {
              err = errno;
              ಭಙ್ಗ;
            }

          bytes += bytes_read;
          ವರ್ಣ ಧ್ರುವ *p = buf;
          ಕುರು
            {
              unsigned ವರ್ಣ c = *p++;
              ವಿಕಲ್ಪ (c)
                {
                ಪಕ್ಷ '\n':
                  ಸಾಲುಗಳು++;
                  FALLTHROUGH;
                ಪಕ್ಷ '\r':
                ಪಕ್ಷ '\f':
                  ಯದಿ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  ಭಙ್ಗ;

                ಪಕ್ಷ '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  ಭಙ್ಗ;

                ಪಕ್ಷ ' ':
                  linepos++;
                  FALLTHROUGH;
                ಪಕ್ಷ '\v':
                  in_word = false;
                  ಭಙ್ಗ;

                ಸ್ವಭಾವ:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  ಪದಗಳು += !in_word & in_word2;
                  in_word = in_word2;
                  ಭಙ್ಗ;
                }
            }
          ಯಾವತ್ (--bytes_read);
        }
      ಯದಿ (linepos > linelength)
        linelength = linepos;
    }

  ಯದಿ (ಎಣಿಕೆ_chars < print_chars)
    chars = bytes;

  ಯದಿ (total_mode != total_only)
    write_counts (ಸಾಲುಗಳು, ಪದಗಳು, chars, bytes, linelength, file_x);

  total_ಸಾಲುಗಳು_overflow |= ckd_add (&total_ಸಾಲುಗಳು, total_ಸಾಲುಗಳು, ಸಾಲುಗಳು);
  total_ಪದಗಳು_overflow |= ckd_add (&total_ಪದಗಳು, total_ಪದಗಳು, ಪದಗಳು);
  total_chars_overflow |= ckd_add (&total_chars, total_chars, chars);
  total_bytes_overflow |= ckd_add (&total_bytes, total_bytes, bytes);

  ಯದಿ (linelength > max_ಸಾಲು_length)
    max_ಸಾಲು_length = linelength;

  ಯದಿ (err)
    error (0, err, "%s", quotef (file));
  ಪ್ರತ್ಯಾಗಮ !err;
}

ಸ್ಥಿರ bool
wc_file (ವರ್ಣ ಧ್ರುವ *file, ಸಂರಚನಾ fstatus *fstatus)
{
  ಯದಿ (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_ದ್ವಿಮಾನ_mode (STDIN_FILENO, O_ದ್ವಿಮಾನ);
      ಪ್ರತ್ಯಾಗಮ wc (STDIN_FILENO, file, fstatus);
    }
  ಅನ್ಯಥಾ
    {
      ಪೂರ್ಣಾಙ್ಕ fd = open (file, O_RDONLY | O_ದ್ವಿಮಾನ);
      ಯದಿ (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          ಪ್ರತ್ಯಾಗಮ false;
        }
      ಅನ್ಯಥಾ
        {
          bool ok = wc (fd, file, fstatus);
          ಯದಿ (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              ಪ್ರತ್ಯಾಗಮ false;
            }
          ಪ್ರತ್ಯಾಗಮ ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

ಸ್ಥಿರ ಸಂರಚನಾ fstatus *
get_ಒಳಹರಿವು_fstatus (idx_t nfiles, ವರ್ಣ *ಧ್ರುವ *file)
{
  ಸಂರಚನಾ fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, ಪರಿಮಾಣ *fstatus);

  ಯದಿ (nfiles == 0
      || (nfiles == 1
          && ((print_ಸಾಲುಗಳು + print_ಪದಗಳು + print_chars
               + print_bytes + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  ಅನ್ಯಥಾ
    {
      ಪ್ರತಿ (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  ಪ್ರತ್ಯಾಗಮ fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_PURE
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compute_ಸಂಖ್ಯೆ_width (idx_t nfiles, ಸಂರಚನಾ fstatus ಧ್ರುವ *fstatus)
{
  ಪೂರ್ಣಾಙ್ಕ width = 1;

  ಯದಿ (0 < nfiles && fstatus[0].failed <= 0)
    {
      ಪೂರ್ಣಾಙ್ಕ minimum_width = 1;
      uintmax_t regular_total = 0;

      ಪ್ರತಿ (idx_t i = 0; i < nfiles; i++)
        ಯದಿ (! fstatus[i].failed)
          {
            ಯದಿ (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            ಅನ್ಯಥಾ ಯದಿ (ckd_add (&regular_total, regular_total,
                              fstatus[i].st.st_size))
              {
                regular_total = UINTMAX_MAX;
                ಭಙ್ಗ;
              }
          }

      ಪ್ರತಿ (; 10 <= regular_total; regular_total /= 10)
        width++;
      ಯದಿ (width < minimum_width)
        width = minimum_width;
    }

  ಪ್ರತ್ಯಾಗಮ width;
}


ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ optc;
  idx_t nfiles;
  ವರ್ಣ **files;
  ವರ್ಣ *files_from = ರಿಕ್ತ;
  ಸಂರಚನಾ fstatus *fstatus;
  ಸಂರಚನಾ Tokens tok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  page_size = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ರಿಕ್ತ, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ರಿಕ್ತ);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "clLmw", longopts, ರಿಕ್ತ)) != -1)
    ವಿಕಲ್ಪ (optc)
      {
      ಪಕ್ಷ 'c':
        print_bytes = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'm':
        print_chars = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'l':
        print_ಸಾಲುಗಳು = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'w':
        print_ಪದಗಳು = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'L':
        print_linelength = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ DEBUG_PROGRAM_OPTION:
        debug = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ FILES0_FROM_OPTION:
        files_from = optarg;
        ಭಙ್ಗ;

      ಪಕ್ಷ TOTAL_OPTION:
        total_mode = XARGMATCH ("--total", optarg, total_args, total_types);
        ಭಙ್ಗ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      ಸ್ವಭಾವ:
        usage (EXIT_FAILURE);
      }

  ಯದಿ (! (print_ಸಾಲುಗಳು || print_ಪದಗಳು || print_chars || print_bytes
         || print_linelength))
    print_ಸಾಲುಗಳು = print_ಪದಗಳು = print_bytes = true;

  ಯದಿ (print_linelength)
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  ಯದಿ (print_ಪದಗಳು)
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = isspace (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  ಸಂರಚನಾ argv_iterator *ai;
  ಯದಿ (files_from)
    {
      FILE *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ಯದಿ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      ಯದಿ (streq (files_from, "-"))
        stream = stdin;
      ಅನ್ಯಥಾ
        {
          stream = fopen (files_from, "r");
          ಯದಿ (stream == ರಿಕ್ತ)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      ಸಂರಚನಾ stat st;
      ಯದಿ (fstat (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_size <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          ಯದಿ (! readtokens0 (stream, &tok) || fclose (stream) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      ಅನ್ಯಥಾ
        {
          files = ರಿಕ್ತ;
          nfiles = 0;
          ai = argv_iter_init_stream (stream);
        }
    }
  ಅನ್ಯಥಾ
    {
      ಸ್ಥಿರ ವರ್ಣ *stdin_only[] = { ರಿಕ್ತ };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  ಯದಿ (!ai)
    xalloc_die ();

  fstatus = get_ಒಳಹರಿವು_fstatus (nfiles, files);
  ಯದಿ (total_mode == total_only)
    ಸಂಖ್ಯೆ_width = 1;  /* No extra padding, since no alignment requirement.  */
  ಅನ್ಯಥಾ
    ಸಂಖ್ಯೆ_width = compute_ಸಂಖ್ಯೆ_width (nfiles, fstatus);

  bool ok = true;
  ಗಣನಾ argv_iter_err ai_err;
  ವರ್ಣ *file_name;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      ಯದಿ (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      ಯದಿ (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          ಯದಿ (files_from == ರಿಕ್ತ)
            error (0, 0, "%s", _("invalid zero-length file name"));
          ಅನ್ಯಥಾ
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_args (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      ಯದಿ (skip_file)
        ok = false;
      ಅನ್ಯಥಾ
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      ಯದಿ (! nfiles)
        fstatus[0].failed = 1;
    }
  ವಿಕಲ್ಪ (ai_err)
    {
    ಪಕ್ಷ AI_ERR_EOF:
      ಭಙ್ಗ;

    ಪಕ್ಷ AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      ಭಙ್ಗ;

    ಪಕ್ಷ AI_ERR_MEM:
      xalloc_die ();

    ಪಕ್ಷ AI_ERR_OK: ಸ್ವಭಾವ:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  ಯದಿ (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (ರಿಕ್ತ, &fstatus[0]);

  ಯದಿ (read_tokens)
    readtokens0_free (&tok);

  ಯದಿ (total_mode != total_never
      && (total_mode != total_auto || 1 < argv_iter_n_args (ai)))
    {
      ಯದಿ (total_ಸಾಲುಗಳು_overflow)
        {
          total_ಸಾಲುಗಳು = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      ಯದಿ (total_ಪದಗಳು_overflow)
        {
          total_ಪದಗಳು = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      ಯದಿ (total_chars_overflow)
        {
          total_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      ಯದಿ (total_bytes_overflow)
        {
          total_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (total_ಸಾಲುಗಳು, total_ಪದಗಳು, total_chars, total_bytes,
                    max_ಸಾಲು_length,
                    total_mode != total_only ? _("total") : ರಿಕ್ತ);
    }

  argv_iter_free (ai);

  ಮುಕ್ತ (fstatus);

  ಯದಿ (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
