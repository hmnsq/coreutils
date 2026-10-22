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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ <argmatch.h>
#ଅନ୍ତର୍ଭାଵ <argv-iter.h>
#ଅନ୍ତର୍ଭାଵ <fadvise.h>
#ଅନ୍ତର୍ଭାଵ <physmem.h>
#ଅନ୍ତର୍ଭାଵ <readtokens0.h>
#ଅନ୍ତର୍ଭାଵ <stat-size.h>
#ଅନ୍ତର୍ଭାଵ <xbinary-io.h>

#ifdef USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#endif

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "cpu-supports.h"
#ଅନ୍ତର୍ଭାଵ "ioblksize.h"
#ଅନ୍ତର୍ଭାଵ "wc.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "wc"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

ସ୍ଥିର bool wc_isprint[UCHAR_MAX + 1];
ସ୍ଥିର bool wc_isspace[UCHAR_MAX + 1];

ସ୍ଥିର bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
ସ୍ଥିର uintmax_t total_ଧାଡ଼ିଗୁଡ଼ିକ;
ସ୍ଥିର uintmax_t total_ଶବ୍ଦଗୁଡ଼ିକ;
ସ୍ଥିର uintmax_t total_chars;
ସ୍ଥିର uintmax_t total_bytes;
ସ୍ଥିର bool total_ଧାଡ଼ିଗୁଡ଼ିକ_overflow;
ସ୍ଥିର bool total_ଶବ୍ଦଗୁଡ଼ିକ_overflow;
ସ୍ଥିର bool total_chars_overflow;
ସ୍ଥିର bool total_bytes_overflow;
ସ୍ଥିର intmax_t max_ଧାଡ଼ି_length;

/* Which counts to print. */
ସ୍ଥିର bool print_ଧାଡ଼ିଗୁଡ଼ିକ, print_ଶବ୍ଦଗୁଡ଼ିକ, print_chars, print_bytes;
ସ୍ଥିର bool print_linelength;

/* The print width of each count.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା_width;

/* True if we have ever read the standard input. */
ସ୍ଥିର bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
ସ୍ଥିର idx_t page_size;

/* Enable to _not_ treat non breaking space as a word separator.  */
ସ୍ଥିର bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
ସଂରଚନା fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା failed;

  /* If FAILED is zero, this is the file's status.  */
  ସଂରଚନା stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  DEBUG_PROGRAM_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  TOTAL_OPTION,
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"bytes", no_argument, ଖାଲି, 'c'},
  {"chars", no_argument, ଖାଲି, 'm'},
  {"lines", no_argument, ଖାଲି, 'l'},
  {"words", no_argument, ଖାଲି, 'w'},
  {"debug", no_argument, ଖାଲି, DEBUG_PROGRAM_OPTION},
  {"files0-from", required_argument, ଖାଲି, FILES0_FROM_OPTION},
  {"max-line-length", no_argument, ଖାଲି, 'L'},
  {"total", required_argument, ଖାଲି, TOTAL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଗଣନା total_type
  {
    total_auto,         /* 0: default or --total=auto */
    total_always,       /* 1: --total=always */
    total_only,         /* 2: --total=only */
    total_never         /* 3: --total=never */
  };
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ total_args[] =
{
  "auto", "always", "only", "never", ଖାଲି
};
ସ୍ଥିର ଗଣନା total_type ଅଚଳ total_types[] =
{
  total_auto, total_always, total_only, total_never
};
ARGMATCH_VERIFY (total_args, total_types);
ସ୍ଥିର ଗଣନା total_type total_mode = total_auto;

#ifdef USE_AVX2_WC_LINECOUNT
ସ୍ଥିର bool
avx2_supported (ଶୂନ୍ୟ)
{
  bool avx2_enabled = cpu_supports ("avx2");
  ଯଦି (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  ଫେରନ୍ତୁ avx2_enabled;
}
#endif

#ifdef USE_AVX512_WC_LINECOUNT
ସ୍ଥିର bool
avx512_supported (ଶୂନ୍ୟ)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  ଯଦି (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  ଫେରନ୍ତୁ avx512_enabled;
}
#endif

#ifdef USE_NEON_WC_LINECOUNT
ସ୍ଥିର bool
neon_supported (ଶୂନ୍ୟ)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  ଯଦି (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  ଫେରନ୍ତୁ neon_enabled;
}
#endif

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      fputs (_("\
Print newline, word, and byte counts ପାଇଁ each FILE, and a total ଧାଡ଼ି ଯଦି\n\
more than one FILE is specified.  A word is a nonempty sequence of non white\n\
space delimited by white space characters or by start or end of ନିବେଶ.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum ଧାଡ଼ି length.\n\
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
  -l, --ଧାଡ଼ିଗୁଡ଼ିକ\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what ଧାଡ଼ି ଗଣନା_2 acceleration is used\n\
"));
      oputs (_("\
      --files0-from=F\n\
         ପଠ ନିବେଶ from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, ପଠ names from standard ନିବେଶ\n\
"));
      oputs (_("\
  -L, --max-ଧାଡ଼ି-length\n\
         print the maximum display width\n\
"));
      oputs (_("\
  -w, --ଶବ୍ଦଗୁଡ଼ିକ\n\
         print the word counts\n\
"));
      oputs (_("\
      --total=WHEN\n\
         when to print a ଧାଡ଼ି with total counts;\n\
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
maybe_c32isnbspace (char32_t wc)
{
  ଫେରନ୍ତୁ ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
ସ୍ଥିର ଶୂନ୍ୟ
write_counts (uintmax_t ଧାଡ଼ିଗୁଡ଼ିକ,
              uintmax_t ଶବ୍ଦଗୁଡ଼ିକ,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              ଅକ୍ଷର ଅଚଳ *file)
{
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ format_sp_int[] = " %*s";
  ଅକ୍ଷର ଅଚଳ *format_int = format_sp_int + 1;
  ଅକ୍ଷର buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  ଯଦି (print_ଧାଡ଼ିଗୁଡ଼ିକ)
    {
      printf (format_int, ସଂଖ୍ୟା_width, umaxtostr (ଧାଡ଼ିଗୁଡ଼ିକ, buf));
      format_int = format_sp_int;
    }
  ଯଦି (print_ଶବ୍ଦଗୁଡ଼ିକ)
    {
      printf (format_int, ସଂଖ୍ୟା_width, umaxtostr (ଶବ୍ଦଗୁଡ଼ିକ, buf));
      format_int = format_sp_int;
    }
  ଯଦି (print_chars)
    {
      printf (format_int, ସଂଖ୍ୟା_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  ଯଦି (print_bytes)
    {
      printf (format_int, ସଂଖ୍ୟା_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  ଯଦି (print_linelength)
    printf (format_int, ସଂଖ୍ୟା_width, imaxtostr (linelength, buf));
  ଯଦି (file)
    printf (" %s", strchr (file, '\n') ? quotef (file) : file);
  putchar ('\n');

  ଯଦି (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
ସ୍ଥିର ସଂରଚନା wc_ଧାଡ଼ିଗୁଡ଼ିକ
wc_ଧାଡ଼ିଗୁଡ଼ିକ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
#ifdef USE_AVX512_WC_LINECOUNT
  ସ୍ଥିର signed ଅକ୍ଷର use_avx512;
  ଯଦି (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  ଯଦି (0 < use_avx512)
    ଫେରନ୍ତୁ wc_ଧାଡ଼ିଗୁଡ଼ିକ_avx512 (fd);
#endif
#ifdef USE_AVX2_WC_LINECOUNT
  ସ୍ଥିର signed ଅକ୍ଷର use_avx2;
  ଯଦି (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  ଯଦି (0 < use_avx2)
    ଫେରନ୍ତୁ wc_ଧାଡ଼ିଗୁଡ଼ିକ_avx2 (fd);
#endif
#ifdef USE_NEON_WC_LINECOUNT
  ସ୍ଥିର signed ଅକ୍ଷର use_neon;
  ଯଦି (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  ଯଦି (0 < use_neon)
    ଫେରନ୍ତୁ wc_ଧାଡ଼ିଗୁଡ଼ିକ_neon (fd);
#endif

  intmax_t ଧାଡ଼ିଗୁଡ଼ିକ = 0, bytes = 0;
  bool long_ଧାଡ଼ିଗୁଡ଼ିକ = false;

  ଯେତେବେଳେ (true)
    {
      ଅକ୍ଷର buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = ପଠ (fd, buf, IO_BUFSIZE);
      ଯଦି (bytes_read <= 0)
        ଫେରନ୍ତୁ (ସଂରଚନା wc_ଧାଡ଼ିଗୁଡ଼ିକ) { bytes_read == 0 ? 0 : errno, ଧାଡ଼ିଗୁଡ଼ିକ, bytes };

      bytes += bytes_read;
      ଅକ୍ଷର *end = buf + bytes_read;
      idx_t buflines = 0;

      ଯଦି (! long_ଧାଡ଼ିଗୁଡ଼ିକ)
        {
          /* Avoid function call overhead for shorter lines.  */
          ପାଇଁ (ଅକ୍ଷର *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      ଅନ୍ୟଥା
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          ପାଇଁ (ଅକ୍ଷର *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_ଧାଡ଼ିଗୁଡ଼ିକ = 15 * buflines <= bytes_read;
      ଧାଡ଼ିଗୁଡ଼ିକ += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
ସ୍ଥିର bool
wc (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର ଅଚଳ *file_x, ସଂରଚନା fstatus *fstatus)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;
  ଅକ୍ଷର buf[IO_BUFSIZE + 1];
  intmax_t ଧାଡ଼ିଗୁଡ଼ିକ, ଶବ୍ଦଗୁଡ଼ିକ, chars, bytes, linelength;
  bool ଗଣନା_bytes, ଗଣନା_chars, ଗଣନା_complicated;
  ଅକ୍ଷର ଅଚଳ *file = file_x ? file_x : _("standard input");

  ଧାଡ଼ିଗୁଡ଼ିକ = ଶବ୍ଦଗୁଡ଼ିକ = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  ଯଦି (MB_CUR_MAX > 1)
    {
      ଗଣନା_bytes = print_bytes;
      ଗଣନା_chars = print_chars;
    }
  ଅନ୍ୟଥା
    {
      ଗଣନା_bytes = print_bytes || print_chars;
      ଗଣନା_chars = false;
    }
  ଗଣନା_complicated = print_ଶବ୍ଦଗୁଡ଼ିକ || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  ଯଦି (!ଗଣନା_bytes || ଗଣନା_chars || print_ଧାଡ଼ିଗୁଡ଼ିକ || ଗଣନା_complicated)
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

  ଯଦି (ଗଣନା_bytes && !ଗଣନା_chars && !print_ଧାଡ଼ିଗୁଡ଼ିକ && !ଗଣନା_complicated)
    {
      bool skip_read = false;

      ଯଦି (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      ଯଦି (! fstatus->failed && usable_st_size (&fstatus->st)
          && 0 <= fstatus->st.st_size)
        {
          off_t end_pos = fstatus->st.st_size;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          ଯଦି (current_pos < 0)
            ;
          ଅନ୍ୟଥା ଯଦି (end_pos % page_size)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              ଯଦି (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          ଅନ୍ୟଥା ଯଦି (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              ଯଦି (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          ଯଦି (0 < bytes && lseek (fd, bytes, SEEK_CUR) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      ଯଦି (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          ପାଇଁ (ssize_t bytes_read;
               (bytes_read = ପଠ (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            ଯଦି (bytes_read < 0)
              {
                err = errno;
                ଭାଙ୍ଗ;
              }
        }
    }
  ଅନ୍ୟଥା ଯଦି (!ଗଣନା_chars && !ଗଣନା_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      ସଂରଚନା wc_ଧାଡ଼ିଗୁଡ଼ିକ w = wc_ଧାଡ଼ିଗୁଡ଼ିକ (fd);
      err = w.err;
      ଧାଡ଼ିଗୁଡ଼ିକ = w.ଧାଡ଼ିଗୁଡ଼ିକ;
      bytes = w.bytes;
    }
  ଅନ୍ୟଥା ଯଦି (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool ଗଣନା_utf8 = !ଗଣନା_complicated && !print_ଧାଡ଼ିଗୁଡ଼ିକ
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      ପାଇଁ (ssize_t bytes_read;
           ((bytes_read = ପଠ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          ଯଦି (bytes_read < 0)
            {
              err = errno;
              ଭାଙ୍ଗ;
            }

          bytes += bytes_read;
          ଅକ୍ଷର ଅଚଳ *p = buf;
          ଅକ୍ଷର ଅଚଳ *plim = p + prev + bytes_read;
          bool try_ଗଣନା_utf8 = ଗଣନା_utf8;
          କର
            {
              ଯଦି (try_ଗଣନା_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_ଗଣନା_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_ଗଣନା (p, &n);
                  p += n;
                  ଯଦି (p == plim)
                    ଭାଙ୍ଗ;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              ଯଦି (!in_shift && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              ଅନ୍ୟଥା
                {
                  idx_t prev_bytes = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  ଯଦି (scanbytes < n)
                    {
                      ଯଦି (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          in_shift = true;
                          ଭାଙ୍ଗ;
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
                      ଶବ୍ଦଗୁଡ଼ିକ += !in_word;
                      in_word = true;
                      ଜାରି;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              ବାଛ (wide_char)
                {
                ସ୍ଥିତି '\n':
                  ଧାଡ଼ିଗୁଡ଼ିକ++;
                  FALLTHROUGH;
                ସ୍ଥିତି '\r':
                ସ୍ଥିତି '\f':
                  ଯଦି (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  ଭାଙ୍ଗ;

                ସ୍ଥିତି '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  ଭାଙ୍ଗ;

                ସ୍ଥିତି ' ':
                  linepos++;
                  FALLTHROUGH;
                ସ୍ଥିତି '\v':
                  in_word = false;
                  ଭାଙ୍ଗ;

                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:;
                  bool in_word2;
                  ଯଦି (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  ଅନ୍ୟଥା
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      ଯଦି (print_linelength)
                        {
                          ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = c32width (wide_char);
                          ଯଦି (width > 0)
                            linepos += width;
                        }
                      in_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  ଶବ୍ଦଗୁଡ଼ିକ += !in_word & in_word2;
                  in_word = in_word2;
                  ଭାଙ୍ଗ;
                }

              p += charbytes;
              chars++;
            }
          ଯେତେବେଳେ (p < plim);
        }
      ଯଦି (linepos > linelength)
        linelength = linepos;
    }
  ଅନ୍ୟଥା
    {
      bool in_word = false;
      intmax_t linepos = 0;

      ପାଇଁ (ssize_t bytes_read; (bytes_read = ପଠ (fd, buf, IO_BUFSIZE)); )
        {
          ଯଦି (bytes_read < 0)
            {
              err = errno;
              ଭାଙ୍ଗ;
            }

          bytes += bytes_read;
          ଅକ୍ଷର ଅଚଳ *p = buf;
          କର
            {
              unsigned ଅକ୍ଷର c = *p++;
              ବାଛ (c)
                {
                ସ୍ଥିତି '\n':
                  ଧାଡ଼ିଗୁଡ଼ିକ++;
                  FALLTHROUGH;
                ସ୍ଥିତି '\r':
                ସ୍ଥିତି '\f':
                  ଯଦି (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  ଭାଙ୍ଗ;

                ସ୍ଥିତି '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  ଭାଙ୍ଗ;

                ସ୍ଥିତି ' ':
                  linepos++;
                  FALLTHROUGH;
                ସ୍ଥିତି '\v':
                  in_word = false;
                  ଭାଙ୍ଗ;

                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  ଶବ୍ଦଗୁଡ଼ିକ += !in_word & in_word2;
                  in_word = in_word2;
                  ଭାଙ୍ଗ;
                }
            }
          ଯେତେବେଳେ (--bytes_read);
        }
      ଯଦି (linepos > linelength)
        linelength = linepos;
    }

  ଯଦି (ଗଣନା_chars < print_chars)
    chars = bytes;

  ଯଦି (total_mode != total_only)
    write_counts (ଧାଡ଼ିଗୁଡ଼ିକ, ଶବ୍ଦଗୁଡ଼ିକ, chars, bytes, linelength, file_x);

  total_ଧାଡ଼ିଗୁଡ଼ିକ_overflow |= ckd_add (&total_ଧାଡ଼ିଗୁଡ଼ିକ, total_ଧାଡ଼ିଗୁଡ଼ିକ, ଧାଡ଼ିଗୁଡ଼ିକ);
  total_ଶବ୍ଦଗୁଡ଼ିକ_overflow |= ckd_add (&total_ଶବ୍ଦଗୁଡ଼ିକ, total_ଶବ୍ଦଗୁଡ଼ିକ, ଶବ୍ଦଗୁଡ଼ିକ);
  total_chars_overflow |= ckd_add (&total_chars, total_chars, chars);
  total_bytes_overflow |= ckd_add (&total_bytes, total_bytes, bytes);

  ଯଦି (linelength > max_ଧାଡ଼ି_length)
    max_ଧାଡ଼ି_length = linelength;

  ଯଦି (err)
    error (0, err, "%s", quotef (file));
  ଫେରନ୍ତୁ !err;
}

ସ୍ଥିର bool
wc_file (ଅକ୍ଷର ଅଚଳ *file, ସଂରଚନା fstatus *fstatus)
{
  ଯଦି (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_ଦ୍ୱିମିକ_mode (STDIN_FILENO, O_ଦ୍ୱିମିକ);
      ଫେରନ୍ତୁ wc (STDIN_FILENO, file, fstatus);
    }
  ଅନ୍ୟଥା
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open (file, O_RDONLY | O_ଦ୍ୱିମିକ);
      ଯଦି (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          ଫେରନ୍ତୁ false;
        }
      ଅନ୍ୟଥା
        {
          bool ok = wc (fd, file, fstatus);
          ଯଦି (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              ଫେରନ୍ତୁ false;
            }
          ଫେରନ୍ତୁ ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

ସ୍ଥିର ସଂରଚନା fstatus *
get_ନିବେଶ_fstatus (idx_t nfiles, ଅକ୍ଷର *ଅଚଳ *file)
{
  ସଂରଚନା fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, ଆକାର *fstatus);

  ଯଦି (nfiles == 0
      || (nfiles == 1
          && ((print_ଧାଡ଼ିଗୁଡ଼ିକ + print_ଶବ୍ଦଗୁଡ଼ିକ + print_chars
               + print_bytes + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  ଅନ୍ୟଥା
    {
      ପାଇଁ (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  ଫେରନ୍ତୁ fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_PURE
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compute_ସଂଖ୍ୟା_width (idx_t nfiles, ସଂରଚନା fstatus ଅଚଳ *fstatus)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = 1;

  ଯଦି (0 < nfiles && fstatus[0].failed <= 0)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା minimum_width = 1;
      uintmax_t regular_total = 0;

      ପାଇଁ (idx_t i = 0; i < nfiles; i++)
        ଯଦି (! fstatus[i].failed)
          {
            ଯଦି (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            ଅନ୍ୟଥା ଯଦି (ckd_add (&regular_total, regular_total,
                              fstatus[i].st.st_size))
              {
                regular_total = UINTMAX_MAX;
                ଭାଙ୍ଗ;
              }
          }

      ପାଇଁ (; 10 <= regular_total; regular_total /= 10)
        width++;
      ଯଦି (width < minimum_width)
        width = minimum_width;
    }

  ଫେରନ୍ତୁ width;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  idx_t nfiles;
  ଅକ୍ଷର **files;
  ଅକ୍ଷର *files_from = ଖାଲି;
  ସଂରଚନା fstatus *fstatus;
  ସଂରଚନା Tokens tok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  page_size = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ଖାଲି, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ଖାଲି);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "clLmw", longopts, ଖାଲି)) != -1)
    ବାଛ (optc)
      {
      ସ୍ଥିତି 'c':
        print_bytes = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'm':
        print_chars = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'l':
        print_ଧାଡ଼ିଗୁଡ଼ିକ = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'w':
        print_ଶବ୍ଦଗୁଡ଼ିକ = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'L':
        print_linelength = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି DEBUG_PROGRAM_OPTION:
        debug = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି FILES0_FROM_OPTION:
        files_from = optarg;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି TOTAL_OPTION:
        total_mode = XARGMATCH ("--total", optarg, total_args, total_types);
        ଭାଙ୍ଗ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        usage (EXIT_FAILURE);
      }

  ଯଦି (! (print_ଧାଡ଼ିଗୁଡ଼ିକ || print_ଶବ୍ଦଗୁଡ଼ିକ || print_chars || print_bytes
         || print_linelength))
    print_ଧାଡ଼ିଗୁଡ଼ିକ = print_ଶବ୍ଦଗୁଡ଼ିକ = print_bytes = true;

  ଯଦି (print_linelength)
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  ଯଦି (print_ଶବ୍ଦଗୁଡ଼ିକ)
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = isspace (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  ସଂରଚନା argv_iterator *ai;
  ଯଦି (files_from)
    {
      FILE *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ଯଦି (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      ଯଦି (streq (files_from, "-"))
        stream = stdin;
      ଅନ୍ୟଥା
        {
          stream = fopen (files_from, "r");
          ଯଦି (stream == ଖାଲି)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      ସଂରଚନା stat st;
      ଯଦି (fstat (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_size <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          ଯଦି (! readtokens0 (stream, &tok) || fclose (stream) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      ଅନ୍ୟଥା
        {
          files = ଖାଲି;
          nfiles = 0;
          ai = argv_iter_init_stream (stream);
        }
    }
  ଅନ୍ୟଥା
    {
      ସ୍ଥିର ଅକ୍ଷର *stdin_only[] = { ଖାଲି };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  ଯଦି (!ai)
    xalloc_die ();

  fstatus = get_ନିବେଶ_fstatus (nfiles, files);
  ଯଦି (total_mode == total_only)
    ସଂଖ୍ୟା_width = 1;  /* No extra padding, since no alignment requirement.  */
  ଅନ୍ୟଥା
    ସଂଖ୍ୟା_width = compute_ସଂଖ୍ୟା_width (nfiles, fstatus);

  bool ok = true;
  ଗଣନା argv_iter_err ai_err;
  ଅକ୍ଷର *file_name;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      ଯଦି (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      ଯଦି (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          ଯଦି (files_from == ଖାଲି)
            error (0, 0, "%s", _("invalid zero-length file name"));
          ଅନ୍ୟଥା
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_args (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      ଯଦି (skip_file)
        ok = false;
      ଅନ୍ୟଥା
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      ଯଦି (! nfiles)
        fstatus[0].failed = 1;
    }
  ବାଛ (ai_err)
    {
    ସ୍ଥିତି AI_ERR_EOF:
      ଭାଙ୍ଗ;

    ସ୍ଥିତି AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି AI_ERR_MEM:
      xalloc_die ();

    ସ୍ଥିତି AI_ERR_OK: ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  ଯଦି (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (ଖାଲି, &fstatus[0]);

  ଯଦି (read_tokens)
    readtokens0_free (&tok);

  ଯଦି (total_mode != total_never
      && (total_mode != total_auto || 1 < argv_iter_n_args (ai)))
    {
      ଯଦି (total_ଧାଡ଼ିଗୁଡ଼ିକ_overflow)
        {
          total_ଧାଡ଼ିଗୁଡ଼ିକ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      ଯଦି (total_ଶବ୍ଦଗୁଡ଼ିକ_overflow)
        {
          total_ଶବ୍ଦଗୁଡ଼ିକ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      ଯଦି (total_chars_overflow)
        {
          total_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      ଯଦି (total_bytes_overflow)
        {
          total_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (total_ଧାଡ଼ିଗୁଡ଼ିକ, total_ଶବ୍ଦଗୁଡ଼ିକ, total_chars, total_bytes,
                    max_ଧାଡ଼ି_length,
                    total_mode != total_only ? _("total") : ଖାଲି);
    }

  argv_iter_free (ai);

  ମୁକ୍ତ (fstatus);

  ଯଦି (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
