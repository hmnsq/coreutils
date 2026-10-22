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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ <argmatch.h>
#അന്തര്ഭാവ <argv-iter.h>
#അന്തര്ഭാവ <fadvise.h>
#അന്തര്ഭാവ <physmem.h>
#അന്തര്ഭാവ <readtokens0.h>
#അന്തര്ഭാവ <stat-size.h>
#അന്തര്ഭാവ <xbinary-io.h>

#ifdef USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#endif

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "cpu-supports.h"
#അന്തര്ഭാവ "ioblksize.h"
#അന്തര്ഭാവ "wc.h"
#അന്തര്ഭാവ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "wc"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

സ്ഥിതം bool wc_isprint[UCHAR_MAX + 1];
സ്ഥിതം bool wc_isspace[UCHAR_MAX + 1];

സ്ഥിതം bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
സ്ഥിതം uintmax_t total_വരികൾ;
സ്ഥിതം uintmax_t total_വാക്കുകൾ;
സ്ഥിതം uintmax_t total_chars;
സ്ഥിതം uintmax_t total_bytes;
സ്ഥിതം bool total_വരികൾ_overflow;
സ്ഥിതം bool total_വാക്കുകൾ_overflow;
സ്ഥിതം bool total_chars_overflow;
സ്ഥിതം bool total_bytes_overflow;
സ്ഥിതം intmax_t max_വരി_length;

/* Which counts to print. */
സ്ഥിതം bool print_വരികൾ, print_വാക്കുകൾ, print_chars, print_bytes;
സ്ഥിതം bool print_linelength;

/* The print width of each count.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ സംഖ്യ_width;

/* True if we have ever read the standard input. */
സ്ഥിതം bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
സ്ഥിതം idx_t page_size;

/* Enable to _not_ treat non breaking space as a word separator.  */
സ്ഥിതം bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
ഘടന fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  പൂർണ്ണസംഖ്യ failed;

  /* If FAILED is zero, this is the file's status.  */
  ഘടന stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  DEBUG_PROGRAM_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  TOTAL_OPTION,
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"bytes", no_argument, ശൂന്യ, 'c'},
  {"chars", no_argument, ശൂന്യ, 'm'},
  {"lines", no_argument, ശൂന്യ, 'l'},
  {"words", no_argument, ശൂന്യ, 'w'},
  {"debug", no_argument, ശൂന്യ, DEBUG_PROGRAM_OPTION},
  {"files0-from", required_argument, ശൂന്യ, FILES0_FROM_OPTION},
  {"max-line-length", no_argument, ശൂന്യ, 'L'},
  {"total", required_argument, ശൂന്യ, TOTAL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

എണ്ണൽ total_type
  {
    total_auto,         /* 0: default or --total=auto */
    total_always,       /* 1: --total=always */
    total_only,         /* 2: --total=only */
    total_never         /* 3: --total=never */
  };
സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം total_args[] =
{
  "auto", "always", "only", "never", ശൂന്യ
};
സ്ഥിതം എണ്ണൽ total_type സ്ഥിരം total_types[] =
{
  total_auto, total_always, total_only, total_never
};
ARGMATCH_VERIFY (total_args, total_types);
സ്ഥിതം എണ്ണൽ total_type total_mode = total_auto;

#ifdef USE_AVX2_WC_LINECOUNT
സ്ഥിതം bool
avx2_supported (ശൂന്യം)
{
  bool avx2_enabled = cpu_supports ("avx2");
  എങ്കിൽ (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  തിരികെ avx2_enabled;
}
#endif

#ifdef USE_AVX512_WC_LINECOUNT
സ്ഥിതം bool
avx512_supported (ശൂന്യം)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  എങ്കിൽ (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  തിരികെ avx512_enabled;
}
#endif

#ifdef USE_NEON_WC_LINECOUNT
സ്ഥിതം bool
neon_supported (ശൂന്യം)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  എങ്കിൽ (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  തിരികെ neon_enabled;
}
#endif

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      fputs (_("\
Print newline, word, and byte counts വേണ്ടി each FILE, and a total വരി എങ്കിൽ\n\
more than one FILE is specified.  A word is a nonempty sequence of non white\n\
space delimited by white space characters or by start or end of നിവേശം.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum വരി length.\n\
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
  -l, --വരികൾ\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what വരി എണ്ണം acceleration is used\n\
"));
      oputs (_("\
      --files0-from=F\n\
         പഠ നിവേശം from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, പഠ names from standard നിവേശം\n\
"));
      oputs (_("\
  -L, --max-വരി-length\n\
         print the maximum display width\n\
"));
      oputs (_("\
  -w, --വാക്കുകൾ\n\
         print the word counts\n\
"));
      oputs (_("\
      --total=WHEN\n\
         when to print a വരി with total counts;\n\
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
maybe_c32isnbspace (char32_t wc)
{
  തിരികെ ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
സ്ഥിതം ശൂന്യം
write_counts (uintmax_t വരികൾ,
              uintmax_t വാക്കുകൾ,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              അക്ഷരം സ്ഥിരം *file)
{
  സ്ഥിതം അക്ഷരം സ്ഥിരം format_sp_int[] = " %*s";
  അക്ഷരം സ്ഥിരം *format_int = format_sp_int + 1;
  അക്ഷരം buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  എങ്കിൽ (print_വരികൾ)
    {
      printf (format_int, സംഖ്യ_width, umaxtostr (വരികൾ, buf));
      format_int = format_sp_int;
    }
  എങ്കിൽ (print_വാക്കുകൾ)
    {
      printf (format_int, സംഖ്യ_width, umaxtostr (വാക്കുകൾ, buf));
      format_int = format_sp_int;
    }
  എങ്കിൽ (print_chars)
    {
      printf (format_int, സംഖ്യ_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  എങ്കിൽ (print_bytes)
    {
      printf (format_int, സംഖ്യ_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  എങ്കിൽ (print_linelength)
    printf (format_int, സംഖ്യ_width, imaxtostr (linelength, buf));
  എങ്കിൽ (file)
    printf (" %s", strchr (file, '\n') ? quotef (file) : file);
  putchar ('\n');

  എങ്കിൽ (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
സ്ഥിതം ഘടന wc_വരികൾ
wc_വരികൾ (പൂർണ്ണസംഖ്യ fd)
{
#ifdef USE_AVX512_WC_LINECOUNT
  സ്ഥിതം signed അക്ഷരം use_avx512;
  എങ്കിൽ (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  എങ്കിൽ (0 < use_avx512)
    തിരികെ wc_വരികൾ_avx512 (fd);
#endif
#ifdef USE_AVX2_WC_LINECOUNT
  സ്ഥിതം signed അക്ഷരം use_avx2;
  എങ്കിൽ (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  എങ്കിൽ (0 < use_avx2)
    തിരികെ wc_വരികൾ_avx2 (fd);
#endif
#ifdef USE_NEON_WC_LINECOUNT
  സ്ഥിതം signed അക്ഷരം use_neon;
  എങ്കിൽ (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  എങ്കിൽ (0 < use_neon)
    തിരികെ wc_വരികൾ_neon (fd);
#endif

  intmax_t വരികൾ = 0, bytes = 0;
  bool long_വരികൾ = false;

  ആകുമ്പോൾ (true)
    {
      അക്ഷരം buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = പഠ (fd, buf, IO_BUFSIZE);
      എങ്കിൽ (bytes_read <= 0)
        തിരികെ (ഘടന wc_വരികൾ) { bytes_read == 0 ? 0 : errno, വരികൾ, bytes };

      bytes += bytes_read;
      അക്ഷരം *end = buf + bytes_read;
      idx_t buflines = 0;

      എങ്കിൽ (! long_വരികൾ)
        {
          /* Avoid function call overhead for shorter lines.  */
          വേണ്ടി (അക്ഷരം *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      അല്ലെങ്കിൽ
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          വേണ്ടി (അക്ഷരം *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_വരികൾ = 15 * buflines <= bytes_read;
      വരികൾ += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
സ്ഥിതം bool
wc (പൂർണ്ണസംഖ്യ fd, അക്ഷരം സ്ഥിരം *file_x, ഘടന fstatus *fstatus)
{
  പൂർണ്ണസംഖ്യ err = 0;
  അക്ഷരം buf[IO_BUFSIZE + 1];
  intmax_t വരികൾ, വാക്കുകൾ, chars, bytes, linelength;
  bool എണ്ണം_bytes, എണ്ണം_chars, എണ്ണം_complicated;
  അക്ഷരം സ്ഥിരം *file = file_x ? file_x : _("standard input");

  വരികൾ = വാക്കുകൾ = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  എങ്കിൽ (MB_CUR_MAX > 1)
    {
      എണ്ണം_bytes = print_bytes;
      എണ്ണം_chars = print_chars;
    }
  അല്ലെങ്കിൽ
    {
      എണ്ണം_bytes = print_bytes || print_chars;
      എണ്ണം_chars = false;
    }
  എണ്ണം_complicated = print_വാക്കുകൾ || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  എങ്കിൽ (!എണ്ണം_bytes || എണ്ണം_chars || print_വരികൾ || എണ്ണം_complicated)
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

  എങ്കിൽ (എണ്ണം_bytes && !എണ്ണം_chars && !print_വരികൾ && !എണ്ണം_complicated)
    {
      bool skip_read = false;

      എങ്കിൽ (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      എങ്കിൽ (! fstatus->failed && usable_st_size (&fstatus->st)
          && 0 <= fstatus->st.st_size)
        {
          off_t end_pos = fstatus->st.st_size;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          എങ്കിൽ (current_pos < 0)
            ;
          അല്ലെങ്കിൽ എങ്കിൽ (end_pos % page_size)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              എങ്കിൽ (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          അല്ലെങ്കിൽ എങ്കിൽ (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              എങ്കിൽ (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          എങ്കിൽ (0 < bytes && lseek (fd, bytes, SEEK_CUR) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      എങ്കിൽ (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          വേണ്ടി (ssize_t bytes_read;
               (bytes_read = പഠ (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            എങ്കിൽ (bytes_read < 0)
              {
                err = errno;
                നിർത്തുക;
              }
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (!എണ്ണം_chars && !എണ്ണം_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      ഘടന wc_വരികൾ w = wc_വരികൾ (fd);
      err = w.err;
      വരികൾ = w.വരികൾ;
      bytes = w.bytes;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool എണ്ണം_utf8 = !എണ്ണം_complicated && !print_വരികൾ
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      വേണ്ടി (ssize_t bytes_read;
           ((bytes_read = പഠ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          എങ്കിൽ (bytes_read < 0)
            {
              err = errno;
              നിർത്തുക;
            }

          bytes += bytes_read;
          അക്ഷരം സ്ഥിരം *p = buf;
          അക്ഷരം സ്ഥിരം *plim = p + prev + bytes_read;
          bool try_എണ്ണം_utf8 = എണ്ണം_utf8;
          ചെയ്യുക
            {
              എങ്കിൽ (try_എണ്ണം_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_എണ്ണം_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_എണ്ണം (p, &n);
                  p += n;
                  എങ്കിൽ (p == plim)
                    നിർത്തുക;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              എങ്കിൽ (!in_shift && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              അല്ലെങ്കിൽ
                {
                  idx_t prev_bytes = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  എങ്കിൽ (scanbytes < n)
                    {
                      എങ്കിൽ (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          in_shift = true;
                          നിർത്തുക;
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
                      വാക്കുകൾ += !in_word;
                      in_word = true;
                      തുടരുക;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              തിരഞ്ഞെടുപ്പ് (wide_char)
                {
                അവസ്ഥ '\n':
                  വരികൾ++;
                  FALLTHROUGH;
                അവസ്ഥ '\r':
                അവസ്ഥ '\f':
                  എങ്കിൽ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  നിർത്തുക;

                അവസ്ഥ '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  നിർത്തുക;

                അവസ്ഥ ' ':
                  linepos++;
                  FALLTHROUGH;
                അവസ്ഥ '\v':
                  in_word = false;
                  നിർത്തുക;

                സ്ഥിരസ്ഥിതി:;
                  bool in_word2;
                  എങ്കിൽ (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  അല്ലെങ്കിൽ
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      എങ്കിൽ (print_linelength)
                        {
                          പൂർണ്ണസംഖ്യ width = c32width (wide_char);
                          എങ്കിൽ (width > 0)
                            linepos += width;
                        }
                      in_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  വാക്കുകൾ += !in_word & in_word2;
                  in_word = in_word2;
                  നിർത്തുക;
                }

              p += charbytes;
              chars++;
            }
          ആകുമ്പോൾ (p < plim);
        }
      എങ്കിൽ (linepos > linelength)
        linelength = linepos;
    }
  അല്ലെങ്കിൽ
    {
      bool in_word = false;
      intmax_t linepos = 0;

      വേണ്ടി (ssize_t bytes_read; (bytes_read = പഠ (fd, buf, IO_BUFSIZE)); )
        {
          എങ്കിൽ (bytes_read < 0)
            {
              err = errno;
              നിർത്തുക;
            }

          bytes += bytes_read;
          അക്ഷരം സ്ഥിരം *p = buf;
          ചെയ്യുക
            {
              unsigned അക്ഷരം c = *p++;
              തിരഞ്ഞെടുപ്പ് (c)
                {
                അവസ്ഥ '\n':
                  വരികൾ++;
                  FALLTHROUGH;
                അവസ്ഥ '\r':
                അവസ്ഥ '\f':
                  എങ്കിൽ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  നിർത്തുക;

                അവസ്ഥ '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  നിർത്തുക;

                അവസ്ഥ ' ':
                  linepos++;
                  FALLTHROUGH;
                അവസ്ഥ '\v':
                  in_word = false;
                  നിർത്തുക;

                സ്ഥിരസ്ഥിതി:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  വാക്കുകൾ += !in_word & in_word2;
                  in_word = in_word2;
                  നിർത്തുക;
                }
            }
          ആകുമ്പോൾ (--bytes_read);
        }
      എങ്കിൽ (linepos > linelength)
        linelength = linepos;
    }

  എങ്കിൽ (എണ്ണം_chars < print_chars)
    chars = bytes;

  എങ്കിൽ (total_mode != total_only)
    write_counts (വരികൾ, വാക്കുകൾ, chars, bytes, linelength, file_x);

  total_വരികൾ_overflow |= ckd_add (&total_വരികൾ, total_വരികൾ, വരികൾ);
  total_വാക്കുകൾ_overflow |= ckd_add (&total_വാക്കുകൾ, total_വാക്കുകൾ, വാക്കുകൾ);
  total_chars_overflow |= ckd_add (&total_chars, total_chars, chars);
  total_bytes_overflow |= ckd_add (&total_bytes, total_bytes, bytes);

  എങ്കിൽ (linelength > max_വരി_length)
    max_വരി_length = linelength;

  എങ്കിൽ (err)
    error (0, err, "%s", quotef (file));
  തിരികെ !err;
}

സ്ഥിതം bool
wc_file (അക്ഷരം സ്ഥിരം *file, ഘടന fstatus *fstatus)
{
  എങ്കിൽ (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_ദ്വിമാന_mode (STDIN_FILENO, O_ദ്വിമാന);
      തിരികെ wc (STDIN_FILENO, file, fstatus);
    }
  അല്ലെങ്കിൽ
    {
      പൂർണ്ണസംഖ്യ fd = open (file, O_RDONLY | O_ദ്വിമാന);
      എങ്കിൽ (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          തിരികെ false;
        }
      അല്ലെങ്കിൽ
        {
          bool ok = wc (fd, file, fstatus);
          എങ്കിൽ (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              തിരികെ false;
            }
          തിരികെ ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

സ്ഥിതം ഘടന fstatus *
get_നിവേശം_fstatus (idx_t nfiles, അക്ഷരം *സ്ഥിരം *file)
{
  ഘടന fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, വലുപ്പം *fstatus);

  എങ്കിൽ (nfiles == 0
      || (nfiles == 1
          && ((print_വരികൾ + print_വാക്കുകൾ + print_chars
               + print_bytes + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  അല്ലെങ്കിൽ
    {
      വേണ്ടി (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  തിരികെ fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_PURE
സ്ഥിതം പൂർണ്ണസംഖ്യ
compute_സംഖ്യ_width (idx_t nfiles, ഘടന fstatus സ്ഥിരം *fstatus)
{
  പൂർണ്ണസംഖ്യ width = 1;

  എങ്കിൽ (0 < nfiles && fstatus[0].failed <= 0)
    {
      പൂർണ്ണസംഖ്യ minimum_width = 1;
      uintmax_t regular_total = 0;

      വേണ്ടി (idx_t i = 0; i < nfiles; i++)
        എങ്കിൽ (! fstatus[i].failed)
          {
            എങ്കിൽ (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            അല്ലെങ്കിൽ എങ്കിൽ (ckd_add (&regular_total, regular_total,
                              fstatus[i].st.st_size))
              {
                regular_total = UINTMAX_MAX;
                നിർത്തുക;
              }
          }

      വേണ്ടി (; 10 <= regular_total; regular_total /= 10)
        width++;
      എങ്കിൽ (width < minimum_width)
        width = minimum_width;
    }

  തിരികെ width;
}


പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ optc;
  idx_t nfiles;
  അക്ഷരം **files;
  അക്ഷരം *files_from = ശൂന്യ;
  ഘടന fstatus *fstatus;
  ഘടന Tokens tok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  page_size = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ശൂന്യ, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ശൂന്യ);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "clLmw", longopts, ശൂന്യ)) != -1)
    തിരഞ്ഞെടുപ്പ് (optc)
      {
      അവസ്ഥ 'c':
        print_bytes = true;
        നിർത്തുക;

      അവസ്ഥ 'm':
        print_chars = true;
        നിർത്തുക;

      അവസ്ഥ 'l':
        print_വരികൾ = true;
        നിർത്തുക;

      അവസ്ഥ 'w':
        print_വാക്കുകൾ = true;
        നിർത്തുക;

      അവസ്ഥ 'L':
        print_linelength = true;
        നിർത്തുക;

      അവസ്ഥ DEBUG_PROGRAM_OPTION:
        debug = true;
        നിർത്തുക;

      അവസ്ഥ FILES0_FROM_OPTION:
        files_from = optarg;
        നിർത്തുക;

      അവസ്ഥ TOTAL_OPTION:
        total_mode = XARGMATCH ("--total", optarg, total_args, total_types);
        നിർത്തുക;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      സ്ഥിരസ്ഥിതി:
        usage (EXIT_FAILURE);
      }

  എങ്കിൽ (! (print_വരികൾ || print_വാക്കുകൾ || print_chars || print_bytes
         || print_linelength))
    print_വരികൾ = print_വാക്കുകൾ = print_bytes = true;

  എങ്കിൽ (print_linelength)
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  എങ്കിൽ (print_വാക്കുകൾ)
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = isspace (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  ഘടന argv_iterator *ai;
  എങ്കിൽ (files_from)
    {
      FILE *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      എങ്കിൽ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      എങ്കിൽ (streq (files_from, "-"))
        stream = stdin;
      അല്ലെങ്കിൽ
        {
          stream = fopen (files_from, "r");
          എങ്കിൽ (stream == ശൂന്യ)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      ഘടന stat st;
      എങ്കിൽ (fstat (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_size <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          എങ്കിൽ (! readtokens0 (stream, &tok) || fclose (stream) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      അല്ലെങ്കിൽ
        {
          files = ശൂന്യ;
          nfiles = 0;
          ai = argv_iter_init_stream (stream);
        }
    }
  അല്ലെങ്കിൽ
    {
      സ്ഥിതം അക്ഷരം *stdin_only[] = { ശൂന്യ };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  എങ്കിൽ (!ai)
    xalloc_die ();

  fstatus = get_നിവേശം_fstatus (nfiles, files);
  എങ്കിൽ (total_mode == total_only)
    സംഖ്യ_width = 1;  /* No extra padding, since no alignment requirement.  */
  അല്ലെങ്കിൽ
    സംഖ്യ_width = compute_സംഖ്യ_width (nfiles, fstatus);

  bool ok = true;
  എണ്ണൽ argv_iter_err ai_err;
  അക്ഷരം *file_name;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      എങ്കിൽ (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      എങ്കിൽ (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          എങ്കിൽ (files_from == ശൂന്യ)
            error (0, 0, "%s", _("invalid zero-length file name"));
          അല്ലെങ്കിൽ
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_args (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      എങ്കിൽ (skip_file)
        ok = false;
      അല്ലെങ്കിൽ
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      എങ്കിൽ (! nfiles)
        fstatus[0].failed = 1;
    }
  തിരഞ്ഞെടുപ്പ് (ai_err)
    {
    അവസ്ഥ AI_ERR_EOF:
      നിർത്തുക;

    അവസ്ഥ AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      നിർത്തുക;

    അവസ്ഥ AI_ERR_MEM:
      xalloc_die ();

    അവസ്ഥ AI_ERR_OK: സ്ഥിരസ്ഥിതി:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  എങ്കിൽ (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (ശൂന്യ, &fstatus[0]);

  എങ്കിൽ (read_tokens)
    readtokens0_free (&tok);

  എങ്കിൽ (total_mode != total_never
      && (total_mode != total_auto || 1 < argv_iter_n_args (ai)))
    {
      എങ്കിൽ (total_വരികൾ_overflow)
        {
          total_വരികൾ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      എങ്കിൽ (total_വാക്കുകൾ_overflow)
        {
          total_വാക്കുകൾ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      എങ്കിൽ (total_chars_overflow)
        {
          total_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      എങ്കിൽ (total_bytes_overflow)
        {
          total_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (total_വരികൾ, total_വാക്കുകൾ, total_chars, total_bytes,
                    max_വരി_length,
                    total_mode != total_only ? _("total") : ശൂന്യ);
    }

  argv_iter_free (ai);

  സ്വതന്ത്രം (fstatus);

  എങ്കിൽ (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
