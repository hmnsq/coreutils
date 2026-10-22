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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <argmatch.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argv-iter.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fadvise.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <physmem.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <readtokens0.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stat-size.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <xbinary-io.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cpu-supports.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "wc"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

ਸਥਿਰ bool wc_isprint[UCHAR_MAX + 1];
ਸਥਿਰ bool wc_isspace[UCHAR_MAX + 1];

ਸਥਿਰ bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
ਸਥਿਰ uintmax_t ਕੁੱਲ_ਸਤਰਾਂ;
ਸਥਿਰ uintmax_t ਕੁੱਲ_ਸ਼ਬਦ;
ਸਥਿਰ uintmax_t ਕੁੱਲ_chars;
ਸਥਿਰ uintmax_t ਕੁੱਲ_ਬਾਈਟਾਂ;
ਸਥਿਰ bool ਕੁੱਲ_ਸਤਰਾਂ_ਉਛਾਲ;
ਸਥਿਰ bool ਕੁੱਲ_ਸ਼ਬਦ_ਉਛਾਲ;
ਸਥਿਰ bool ਕੁੱਲ_chars_ਉਛਾਲ;
ਸਥਿਰ bool ਕੁੱਲ_ਬਾਈਟਾਂ_ਉਛਾਲ;
ਸਥਿਰ intmax_t max_ਸਤਰ_ਲੰਮਾਈ;

/* Which counts to print. */
ਸਥਿਰ bool print_ਸਤਰਾਂ, print_ਸ਼ਬਦ, print_chars, print_ਬਾਈਟਾਂ;
ਸਥਿਰ bool print_linelength;

/* The print width of each count.  */
ਸਥਿਰ ਪੂਰਨ ਸੰਖਿਆ_ਚੌੜਾਈ;

/* True if we have ever read the standard input. */
ਸਥਿਰ bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
ਸਥਿਰ idx_t ਪੰਨਾ_ਮਾਪ;

/* Enable to _not_ treat non breaking space as a word separator.  */
ਸਥਿਰ bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
ਬਣਤਰ fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  ਪੂਰਨ failed;

  /* If FAILED is zero, this is the file's status.  */
  ਬਣਤਰ stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  ਕੁੱਲ_OPTION,
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"bytes", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"chars", no_ਦਲੀਲ, ਨਿਰਾ, 'm'},
  {"lines", no_ਦਲੀਲ, ਨਿਰਾ, 'l'},
  {"words", no_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  {"debug", no_ਦਲੀਲ, ਨਿਰਾ, DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION},
  {"files0-from", required_ਦਲੀਲ, ਨਿਰਾ, FILES0_FROM_OPTION},
  {"max-line-length", no_ਦਲੀਲ, ਨਿਰਾ, 'L'},
  {"total", required_ਦਲੀਲ, ਨਿਰਾ, ਕੁੱਲ_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਗਿਣਤੀ ਕੁੱਲ_type
  {
    ਕੁੱਲ_auto,         /* 0: default or --total=auto */
    ਕੁੱਲ_always,       /* 1: --total=always */
    ਕੁੱਲ_only,         /* 2: --total=only */
    ਕੁੱਲ_never         /* 3: --total=never */
  };
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ ਕੁੱਲ_ਦਲੀਲਾਂ[] =
{
  "auto", "always", "only", "never", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ ਕੁੱਲ_type ਅਚਲ ਕੁੱਲ_types[] =
{
  ਕੁੱਲ_auto, ਕੁੱਲ_always, ਕੁੱਲ_only, ਕੁੱਲ_never
};
ARGMATCH_VERIFY (ਕੁੱਲ_ਦਲੀਲਾਂ, ਕੁੱਲ_types);
ਸਥਿਰ ਗਿਣਤੀ ਕੁੱਲ_type ਕੁੱਲ_ਬਹੁਲਕ = ਕੁੱਲ_auto;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_AVX2_WC_LINECOUNT
ਸਥਿਰ bool
avx2_supported (ਖਾਲੀ)
{
  bool avx2_enabled = cpu_supports ("avx2");
  ਜੇ (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  ਵਾਪਸ avx2_enabled;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_AVX512_WC_LINECOUNT
ਸਥਿਰ bool
avx512_supported (ਖਾਲੀ)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  ਜੇ (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  ਵਾਪਸ avx512_enabled;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_NEON_WC_LINECOUNT
ਸਥਿਰ bool
neon_supported (ਖਾਲੀ)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  ਜੇ (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  ਵਾਪਸ neon_enabled;
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Print newline, ਸ਼ਬਦ_2, and byte counts ਲਈ each FILE, and a ਕੁੱਲ ਸਤਰ ਜੇ\n\
ਹੋਰ than one FILE is specified.  A ਸ਼ਬਦ_2 is a nonempty sequence ਦਾ non white\n\
space delimited by white space characters or by ਸ਼ੁਰੂ or ਸਿਰਾ ਦਾ ਨਿਵੇਸ਼.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options ਹੇਠਾਂ may be ਵਰਤਿਆ ਮੰਜ਼ਿਲ select which counts are printed, always ਵਿੱਚ\n\
the following ਕ੍ਰਮ: newline, ਸ਼ਬਦ_2, character, byte, ਵੱਧ_ਤੋਂ_ਵੱਧ ਸਤਰ ਲੰਮਾਈ.\n\
"), stdout);
      oputs (_("\
  -c, --ਬਾਈਟਾਂ\n\
         print the byte counts\n\
"));
      oputs (_("\
  -m, --chars\n\
         print the character counts\n\
"));
      oputs (_("\
  -l, --ਸਤਰਾਂ\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what ਸਤਰ ਗਿਣਤੀ_2 acceleration is ਵਰਤਿਆ\n\
"));
      oputs (_("\
      --files0-from=F\n\
         ਪਠ ਨਿਵੇਸ਼ from the files specified by\n\
         NUL-terminated ਨਾਂ ਵਿੱਚ file F;\n\
         If F is -, ਪਠ ਨਾਂ from standard ਨਿਵੇਸ਼\n\
"));
      oputs (_("\
  -L, --max-ਸਤਰ-ਲੰਮਾਈ\n\
         print the ਵੱਧ_ਤੋਂ_ਵੱਧ display ਚੌੜਾਈ\n\
"));
      oputs (_("\
  -w, --ਸ਼ਬਦ_4\n\
         print the ਸ਼ਬਦ_2 counts\n\
"));
      oputs (_("\
      --ਕੁੱਲ=WHEN\n\
         when ਮੰਜ਼ਿਲ print a ਸਤਰ with ਕੁੱਲ counts;\n\
         WHEN can be: ਸਵੈਚਾਲਿਤ, always, only, never\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Return non zero if POSIXLY_CORRECT is not set and WC is a non breaking
   space.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਪੂਰਨ
maybe_c32isnbspace (char32_t wc)
{
  ਵਾਪਸ ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
ਸਥਿਰ ਖਾਲੀ
write_counts (uintmax_t ਸਤਰਾਂ,
              uintmax_t ਸ਼ਬਦ_4,
              uintmax_t chars,
              uintmax_t ਬਾਈਟਾਂ,
              intmax_t linelength,
              ਅੱਖਰ ਅਚਲ *file)
{
  ਸਥਿਰ ਅੱਖਰ ਅਚਲ format_sp_int[] = " %*s";
  ਅੱਖਰ ਅਚਲ *format_int = format_sp_int + 1;
  ਅੱਖਰ buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  ਜੇ (print_ਸਤਰਾਂ)
    {
      ਲਿਖੋ (format_int, ਸੰਖਿਆ_ਚੌੜਾਈ, umaxtostr (ਸਤਰਾਂ, buf));
      format_int = format_sp_int;
    }
  ਜੇ (print_ਸ਼ਬਦ)
    {
      ਲਿਖੋ (format_int, ਸੰਖਿਆ_ਚੌੜਾਈ, umaxtostr (ਸ਼ਬਦ_4, buf));
      format_int = format_sp_int;
    }
  ਜੇ (print_chars)
    {
      ਲਿਖੋ (format_int, ਸੰਖਿਆ_ਚੌੜਾਈ, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  ਜੇ (print_ਬਾਈਟਾਂ)
    {
      ਲਿਖੋ (format_int, ਸੰਖਿਆ_ਚੌੜਾਈ, umaxtostr (ਬਾਈਟਾਂ, buf));
      format_int = format_sp_int;
    }
  ਜੇ (print_linelength)
    ਲਿਖੋ (format_int, ਸੰਖਿਆ_ਚੌੜਾਈ, imaxtostr (linelength, buf));
  ਜੇ (file)
    ਲਿਖੋ (" %s", strchr (file, '\n') ? quotef (file) : file);
  ਅੱਖਰ_ਲਿਖੋ ('\n');

  ਜੇ (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
ਸਥਿਰ ਬਣਤਰ wc_ਸਤਰਾਂ
wc_ਸਤਰਾਂ (ਪੂਰਨ fd)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_AVX512_WC_LINECOUNT
  ਸਥਿਰ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ use_avx512;
  ਜੇ (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  ਜੇ (0 < use_avx512)
    ਵਾਪਸ wc_ਸਤਰਾਂ_avx512 (fd);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_AVX2_WC_LINECOUNT
  ਸਥਿਰ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ use_avx2;
  ਜੇ (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  ਜੇ (0 < use_avx2)
    ਵਾਪਸ wc_ਸਤਰਾਂ_avx2 (fd);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_NEON_WC_LINECOUNT
  ਸਥਿਰ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ use_neon;
  ਜੇ (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  ਜੇ (0 < use_neon)
    ਵਾਪਸ wc_ਸਤਰਾਂ_neon (fd);
#ਜੇ_ਅੰਤ

  intmax_t ਸਤਰਾਂ = 0, ਬਾਈਟਾਂ = 0;
  bool long_ਸਤਰਾਂ = false;

  ਜਦੋਂ (true)
    {
      ਅੱਖਰ buf[IO_BUFSIZE + 1];
      ssize_t ਬਾਈਟਾਂ_read = ਪਠ (fd, buf, IO_BUFSIZE);
      ਜੇ (ਬਾਈਟਾਂ_read <= 0)
        ਵਾਪਸ (ਬਣਤਰ wc_ਸਤਰਾਂ) { ਬਾਈਟਾਂ_read == 0 ? 0 : errno, ਸਤਰਾਂ, ਬਾਈਟਾਂ };

      ਬਾਈਟਾਂ += ਬਾਈਟਾਂ_read;
      ਅੱਖਰ *ਸਿਰਾ = buf + ਬਾਈਟਾਂ_read;
      idx_t buflines = 0;

      ਜੇ (! long_ਸਤਰਾਂ)
        {
          /* Avoid function call overhead for shorter lines.  */
          ਲਈ (ਅੱਖਰ *p = buf; p < ਸਿਰਾ; p++)
            buflines += *p == '\n';
        }
      ਨਹੀਂਤਾਂ
        {
          /* rawmemchr is more efficient with longer lines.  */
          *ਸਿਰਾ = '\n';
          ਲਈ (ਅੱਖਰ *p = buf; (p = rawmemchr (p, '\n')) < ਸਿਰਾ; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_ਸਤਰਾਂ = 15 * buflines <= ਬਾਈਟਾਂ_read;
      ਸਤਰਾਂ += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
ਸਥਿਰ bool
wc (ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *file_x, ਬਣਤਰ fstatus *fstatus)
{
  ਪੂਰਨ err = 0;
  ਅੱਖਰ buf[IO_BUFSIZE + 1];
  intmax_t ਸਤਰਾਂ, ਸ਼ਬਦ_4, chars, ਬਾਈਟਾਂ, linelength;
  bool ਗਿਣਤੀ_ਬਾਈਟਾਂ_2, ਗਿਣਤੀ_chars, ਗਿਣਤੀ_complicated;
  ਅੱਖਰ ਅਚਲ *file = file_x ? file_x : _("standard input");

  ਸਤਰਾਂ = ਸ਼ਬਦ_4 = chars = ਬਾਈਟਾਂ = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  ਜੇ (MB_CUR_MAX > 1)
    {
      ਗਿਣਤੀ_ਬਾਈਟਾਂ_2 = print_ਬਾਈਟਾਂ;
      ਗਿਣਤੀ_chars = print_chars;
    }
  ਨਹੀਂਤਾਂ
    {
      ਗਿਣਤੀ_ਬਾਈਟਾਂ_2 = print_ਬਾਈਟਾਂ || print_chars;
      ਗਿਣਤੀ_chars = false;
    }
  ਗਿਣਤੀ_complicated = print_ਸ਼ਬਦ || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  ਜੇ (!ਗਿਣਤੀ_ਬਾਈਟਾਂ_2 || ਗਿਣਤੀ_chars || print_ਸਤਰਾਂ || ਗਿਣਤੀ_complicated)
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

  ਜੇ (ਗਿਣਤੀ_ਬਾਈਟਾਂ_2 && !ਗਿਣਤੀ_chars && !print_ਸਤਰਾਂ && !ਗਿਣਤੀ_complicated)
    {
      bool skip_read = false;

      ਜੇ (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      ਜੇ (! fstatus->failed && usable_st_ਮਾਪ (&fstatus->st)
          && 0 <= fstatus->st.st_ਮਾਪ)
        {
          off_t ਸਿਰਾ_pos = fstatus->st.st_ਮਾਪ;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          ਜੇ (current_pos < 0)
            ;
          ਨਹੀਂਤਾਂ ਜੇ (ਸਿਰਾ_pos % ਪੰਨਾ_ਮਾਪ)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              ਜੇ (current_pos < ਸਿਰਾ_pos)
                {
                  ਬਾਈਟਾਂ = ਸਿਰਾ_pos - current_pos;
                  skip_read = true;
                }
            }
          ਨਹੀਂਤਾਂ ਜੇ (0 < ਸਿਰਾ_pos)
            {
              off_t ep_1 = ਸਿਰਾ_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              ਜੇ (current_pos < hi_pos)
                ਬਾਈਟਾਂ = hi_pos - current_pos;
            }

          ਜੇ (0 < ਬਾਈਟਾਂ && lseek (fd, ਬਾਈਟਾਂ, SEEK_CUR) < 0)
            {
              ਬਾਈਟਾਂ = 0;
              skip_read = false;
            }
        }

      ਜੇ (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          ਲਈ (ssize_t ਬਾਈਟਾਂ_read;
               (ਬਾਈਟਾਂ_read = ਪਠ (fd, buf, IO_BUFSIZE));
               ਬਾਈਟਾਂ += ਬਾਈਟਾਂ_read)
            ਜੇ (ਬਾਈਟਾਂ_read < 0)
              {
                err = errno;
                ਰੋਕੋ;
              }
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (!ਗਿਣਤੀ_chars && !ਗਿਣਤੀ_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      ਬਣਤਰ wc_ਸਤਰਾਂ w = wc_ਸਤਰਾਂ (fd);
      err = w.err;
      ਸਤਰਾਂ = w.ਸਤਰਾਂ;
      ਬਾਈਟਾਂ = w.ਬਾਈਟਾਂ;
    }
  ਨਹੀਂਤਾਂ ਜੇ (MB_CUR_MAX > 1)
    {
      bool ਵਿੱਚ_ਸ਼ਬਦ = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool ਵਿੱਚ_ਖਿਸਕਾਓ = false;
      bool ਗਿਣਤੀ_utf8 = !ਗਿਣਤੀ_complicated && !print_ਸਤਰਾਂ
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      ਲਈ (ssize_t ਬਾਈਟਾਂ_read;
           ((ਬਾਈਟਾਂ_read = ਪਠ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          ਜੇ (ਬਾਈਟਾਂ_read < 0)
            {
              err = errno;
              ਰੋਕੋ;
            }

          ਬਾਈਟਾਂ += ਬਾਈਟਾਂ_read;
          ਅੱਖਰ ਅਚਲ *p = buf;
          ਅੱਖਰ ਅਚਲ *plim = p + prev + ਬਾਈਟਾਂ_read;
          bool try_ਗਿਣਤੀ_utf8 = ਗਿਣਤੀ_utf8;
          ਕਰੋ
            {
              ਜੇ (try_ਗਿਣਤੀ_utf8 && !ਵਿੱਚ_ਖਿਸਕਾਓ)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_ਗਿਣਤੀ_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_ਗਿਣਤੀ (p, &n);
                  p += n;
                  ਜੇ (p == plim)
                    ਰੋਕੋ;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              ਜੇ (!ਵਿੱਚ_ਖਿਸਕਾਓ && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              ਨਹੀਂਤਾਂ
                {
                  idx_t prev_ਬਾਈਟਾਂ = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  ਜੇ (scanbytes < n)
                    {
                      ਜੇ (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && ਬਾਈਟਾਂ_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          ਵਿੱਚ_ਖਿਸਕਾਓ = true;
                          ਰੋਕੋ;
                        }

                      /* Remember that we read a byte, but don't complain
                         about the error.  Because of the decoding error,
                         this is a considered to be byte but not a
                         character (that is, chars is not incremented).  */
                      p++;
                      mbszero (&state);
                      ਵਿੱਚ_ਖਿਸਕਾਓ = false;

                      /* Treat encoding errors as non white space.
                         POSIX says a word is "a non-zero-length string of
                         characters delimited by white space".  This is
                         wrong in some sense, as the string can be delimited
                         by start or end of input, and it is unclear what it
                         means when the input contains encoding errors.
                         Since encoding errors are not white space,
                         treat them that way here.  */
                      ਸ਼ਬਦ_4 += !ਵਿੱਚ_ਸ਼ਬਦ;
                      ਵਿੱਚ_ਸ਼ਬਦ = true;
                      ਜਾਰੀ;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_ਬਾਈਟਾਂ + n + !n;
                  single_byte = charbytes == !ਵਿੱਚ_ਖਿਸਕਾਓ;
                  ਵਿੱਚ_ਖਿਸਕਾਓ = !mbsinit (&state);
                }

              ਚੋਣ (wide_char)
                {
                ਹਾਲਤ '\n':
                  ਸਤਰਾਂ++;
                  FALLTHROUGH;
                ਹਾਲਤ '\r':
                ਹਾਲਤ '\f':
                  ਜੇ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  ਵਿੱਚ_ਸ਼ਬਦ = false;
                  ਰੋਕੋ;

                ਹਾਲਤ '\t':
                  linepos += 8 - (linepos % 8);
                  ਵਿੱਚ_ਸ਼ਬਦ = false;
                  ਰੋਕੋ;

                ਹਾਲਤ ' ':
                  linepos++;
                  FALLTHROUGH;
                ਹਾਲਤ '\v':
                  ਵਿੱਚ_ਸ਼ਬਦ = false;
                  ਰੋਕੋ;

                ਮੂਲ:;
                  bool ਵਿੱਚ_word2;
                  ਜੇ (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      ਵਿੱਚ_word2 = !wc_isspace[wide_char];
                    }
                  ਨਹੀਂਤਾਂ
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      ਜੇ (print_linelength)
                        {
                          ਪੂਰਨ ਚੌੜਾਈ = c32width (wide_char);
                          ਜੇ (ਚੌੜਾਈ > 0)
                            linepos += ਚੌੜਾਈ;
                        }
                      ਵਿੱਚ_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  ਸ਼ਬਦ_4 += !ਵਿੱਚ_ਸ਼ਬਦ & ਵਿੱਚ_word2;
                  ਵਿੱਚ_ਸ਼ਬਦ = ਵਿੱਚ_word2;
                  ਰੋਕੋ;
                }

              p += charbytes;
              chars++;
            }
          ਜਦੋਂ (p < plim);
        }
      ਜੇ (linepos > linelength)
        linelength = linepos;
    }
  ਨਹੀਂਤਾਂ
    {
      bool ਵਿੱਚ_ਸ਼ਬਦ = false;
      intmax_t linepos = 0;

      ਲਈ (ssize_t ਬਾਈਟਾਂ_read; (ਬਾਈਟਾਂ_read = ਪਠ (fd, buf, IO_BUFSIZE)); )
        {
          ਜੇ (ਬਾਈਟਾਂ_read < 0)
            {
              err = errno;
              ਰੋਕੋ;
            }

          ਬਾਈਟਾਂ += ਬਾਈਟਾਂ_read;
          ਅੱਖਰ ਅਚਲ *p = buf;
          ਕਰੋ
            {
              ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = *p++;
              ਚੋਣ (c)
                {
                ਹਾਲਤ '\n':
                  ਸਤਰਾਂ++;
                  FALLTHROUGH;
                ਹਾਲਤ '\r':
                ਹਾਲਤ '\f':
                  ਜੇ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  ਵਿੱਚ_ਸ਼ਬਦ = false;
                  ਰੋਕੋ;

                ਹਾਲਤ '\t':
                  linepos += 8 - (linepos % 8);
                  ਵਿੱਚ_ਸ਼ਬਦ = false;
                  ਰੋਕੋ;

                ਹਾਲਤ ' ':
                  linepos++;
                  FALLTHROUGH;
                ਹਾਲਤ '\v':
                  ਵਿੱਚ_ਸ਼ਬਦ = false;
                  ਰੋਕੋ;

                ਮੂਲ:
                  linepos += wc_isprint[c];
                  bool ਵਿੱਚ_word2 = !wc_isspace[c];
                  ਸ਼ਬਦ_4 += !ਵਿੱਚ_ਸ਼ਬਦ & ਵਿੱਚ_word2;
                  ਵਿੱਚ_ਸ਼ਬਦ = ਵਿੱਚ_word2;
                  ਰੋਕੋ;
                }
            }
          ਜਦੋਂ (--ਬਾਈਟਾਂ_read);
        }
      ਜੇ (linepos > linelength)
        linelength = linepos;
    }

  ਜੇ (ਗਿਣਤੀ_chars < print_chars)
    chars = ਬਾਈਟਾਂ;

  ਜੇ (ਕੁੱਲ_ਬਹੁਲਕ != ਕੁੱਲ_only)
    write_counts (ਸਤਰਾਂ, ਸ਼ਬਦ_4, chars, ਬਾਈਟਾਂ, linelength, file_x);

  ਕੁੱਲ_ਸਤਰਾਂ_ਉਛਾਲ |= ckd_ਜੋੜੋ (&ਕੁੱਲ_ਸਤਰਾਂ, ਕੁੱਲ_ਸਤਰਾਂ, ਸਤਰਾਂ);
  ਕੁੱਲ_ਸ਼ਬਦ_ਉਛਾਲ |= ckd_ਜੋੜੋ (&ਕੁੱਲ_ਸ਼ਬਦ, ਕੁੱਲ_ਸ਼ਬਦ, ਸ਼ਬਦ_4);
  ਕੁੱਲ_chars_ਉਛਾਲ |= ckd_ਜੋੜੋ (&ਕੁੱਲ_chars, ਕੁੱਲ_chars, chars);
  ਕੁੱਲ_ਬਾਈਟਾਂ_ਉਛਾਲ |= ckd_ਜੋੜੋ (&ਕੁੱਲ_ਬਾਈਟਾਂ, ਕੁੱਲ_ਬਾਈਟਾਂ, ਬਾਈਟਾਂ);

  ਜੇ (linelength > max_ਸਤਰ_ਲੰਮਾਈ)
    max_ਸਤਰ_ਲੰਮਾਈ = linelength;

  ਜੇ (err)
    error (0, err, "%s", quotef (file));
  ਵਾਪਸ !err;
}

ਸਥਿਰ bool
wc_file (ਅੱਖਰ ਅਚਲ *file, ਬਣਤਰ fstatus *fstatus)
{
  ਜੇ (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDIN_FILENO, O_ਦੁਆਧਾਰੀ);
      ਵਾਪਸ wc (STDIN_FILENO, file, fstatus);
    }
  ਨਹੀਂਤਾਂ
    {
      ਪੂਰਨ fd = open (file, O_RDONLY | O_ਦੁਆਧਾਰੀ);
      ਜੇ (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          ਵਾਪਸ false;
        }
      ਨਹੀਂਤਾਂ
        {
          bool ok = wc (fd, file, fstatus);
          ਜੇ (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              ਵਾਪਸ false;
            }
          ਵਾਪਸ ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

ਸਥਿਰ ਬਣਤਰ fstatus *
get_ਨਿਵੇਸ਼_fstatus (idx_t nfiles, ਅੱਖਰ *ਅਚਲ *file)
{
  ਬਣਤਰ fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, ਆਕਾਰ *fstatus);

  ਜੇ (nfiles == 0
      || (nfiles == 1
          && ((print_ਸਤਰਾਂ + print_ਸ਼ਬਦ + print_chars
               + print_ਬਾਈਟਾਂ + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  ਨਹੀਂਤਾਂ
    {
      ਲਈ (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  ਵਾਪਸ fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਪੂਰਨ
compute_ਸੰਖਿਆ_ਚੌੜਾਈ (idx_t nfiles, ਬਣਤਰ fstatus ਅਚਲ *fstatus)
{
  ਪੂਰਨ ਚੌੜਾਈ = 1;

  ਜੇ (0 < nfiles && fstatus[0].failed <= 0)
    {
      ਪੂਰਨ minimum_ਚੌੜਾਈ = 1;
      uintmax_t regular_ਕੁੱਲ = 0;

      ਲਈ (idx_t i = 0; i < nfiles; i++)
        ਜੇ (! fstatus[i].failed)
          {
            ਜੇ (!S_ISREG (fstatus[i].st.st_ਬਹੁਲਕ))
              minimum_ਚੌੜਾਈ = 7;
            ਨਹੀਂਤਾਂ ਜੇ (ckd_ਜੋੜੋ (&regular_ਕੁੱਲ, regular_ਕੁੱਲ,
                              fstatus[i].st.st_ਮਾਪ))
              {
                regular_ਕੁੱਲ = UINTMAX_MAX;
                ਰੋਕੋ;
              }
          }

      ਲਈ (; 10 <= regular_ਕੁੱਲ; regular_ਕੁੱਲ /= 10)
        ਚੌੜਾਈ++;
      ਜੇ (ਚੌੜਾਈ < minimum_ਚੌੜਾਈ)
        ਚੌੜਾਈ = minimum_ਚੌੜਾਈ;
    }

  ਵਾਪਸ ਚੌੜਾਈ;
}


ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ optc;
  idx_t nfiles;
  ਅੱਖਰ **files;
  ਅੱਖਰ *files_from = ਨਿਰਾ;
  ਬਣਤਰ fstatus *fstatus;
  ਬਣਤਰ ਟੋਕਨ tok;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਪੰਨਾ_ਮਾਪ = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ਨਿਰਾ, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ਨਿਰਾ);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "clLmw", longopts, ਨਿਰਾ)) != -1)
    ਚੋਣ (optc)
      {
      ਹਾਲਤ 'c':
        print_ਬਾਈਟਾਂ = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'm':
        print_chars = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'l':
        print_ਸਤਰਾਂ = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'w':
        print_ਸ਼ਬਦ = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'L':
        print_linelength = true;
        ਰੋਕੋ;

      ਹਾਲਤ DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION:
        debug = true;
        ਰੋਕੋ;

      ਹਾਲਤ FILES0_FROM_OPTION:
        files_from = optarg;
        ਰੋਕੋ;

      ਹਾਲਤ ਕੁੱਲ_OPTION:
        ਕੁੱਲ_ਬਹੁਲਕ = XARGMATCH ("--total", optarg, ਕੁੱਲ_ਦਲੀਲਾਂ, ਕੁੱਲ_types);
        ਰੋਕੋ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

      ਮੂਲ:
        usage (EXIT_FAILURE);
      }

  ਜੇ (! (print_ਸਤਰਾਂ || print_ਸ਼ਬਦ || print_chars || print_ਬਾਈਟਾਂ
         || print_linelength))
    print_ਸਤਰਾਂ = print_ਸ਼ਬਦ = print_ਬਾਈਟਾਂ = true;

  ਜੇ (print_linelength)
    ਲਈ (ਪੂਰਨ i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  ਜੇ (print_ਸ਼ਬਦ)
    ਲਈ (ਪੂਰਨ i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = ਖਾਲੀ_ਥਾਂ_ਹੈ (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_ਟੋਕਨ = false;
  ਬਣਤਰ argv_iterator *ai;
  ਜੇ (files_from)
    {
      FILE *ਧਾਰਾ;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ਜੇ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      ਜੇ (streq (files_from, "-"))
        ਧਾਰਾ = stdin;
      ਨਹੀਂਤਾਂ
        {
          ਧਾਰਾ = fopen (files_from, "r");
          ਜੇ (ਧਾਰਾ == ਨਿਰਾ)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      ਬਣਤਰ stat st;
      ਜੇ (fstat (fileno (ਧਾਰਾ), &st) == 0
          && S_ISREG (st.st_ਬਹੁਲਕ)
          && st.st_ਮਾਪ <= MIN (10 * 1024 * 1024, physmem_ਉਪਲਬਧ () / 2))
        {
          read_ਟੋਕਨ = true;
          readtokens0_init (&tok);
          ਜੇ (! readtokens0 (ਧਾਰਾ, &tok) || fclose (ਧਾਰਾ) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      ਨਹੀਂਤਾਂ
        {
          files = ਨਿਰਾ;
          nfiles = 0;
          ai = argv_iter_init_ਧਾਰਾ (ਧਾਰਾ);
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਸਥਿਰ ਅੱਖਰ *stdin_only[] = { ਨਿਰਾ };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  ਜੇ (!ai)
    xalloc_die ();

  fstatus = get_ਨਿਵੇਸ਼_fstatus (nfiles, files);
  ਜੇ (ਕੁੱਲ_ਬਹੁਲਕ == ਕੁੱਲ_only)
    ਸੰਖਿਆ_ਚੌੜਾਈ = 1;  /* No extra padding, since no alignment requirement.  */
  ਨਹੀਂਤਾਂ
    ਸੰਖਿਆ_ਚੌੜਾਈ = compute_ਸੰਖਿਆ_ਚੌੜਾਈ (nfiles, fstatus);

  bool ok = true;
  ਗਿਣਤੀ argv_iter_err ai_err;
  ਅੱਖਰ *file_name;
  ਲਈ (ਪੂਰਨ i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      ਜੇ (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      ਜੇ (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          ਜੇ (files_from == ਨਿਰਾ)
            error (0, 0, "%s", _("invalid zero-length file name"));
          ਨਹੀਂਤਾਂ
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_ਦਲੀਲਾਂ (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      ਜੇ (skip_file)
        ok = false;
      ਨਹੀਂਤਾਂ
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      ਜੇ (! nfiles)
        fstatus[0].failed = 1;
    }
  ਚੋਣ (ai_err)
    {
    ਹਾਲਤ AI_ERR_EOF:
      ਰੋਕੋ;

    ਹਾਲਤ AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      ਰੋਕੋ;

    ਹਾਲਤ AI_ERR_MEM:
      xalloc_die ();

    ਹਾਲਤ AI_ERR_OK: ਮੂਲ:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  ਜੇ (ok && !files_from && argv_iter_n_ਦਲੀਲਾਂ (ai) == 0)
    ok &= wc_file (ਨਿਰਾ, &fstatus[0]);

  ਜੇ (read_ਟੋਕਨ)
    readtokens0_free (&tok);

  ਜੇ (ਕੁੱਲ_ਬਹੁਲਕ != ਕੁੱਲ_never
      && (ਕੁੱਲ_ਬਹੁਲਕ != ਕੁੱਲ_auto || 1 < argv_iter_n_ਦਲੀਲਾਂ (ai)))
    {
      ਜੇ (ਕੁੱਲ_ਸਤਰਾਂ_ਉਛਾਲ)
        {
          ਕੁੱਲ_ਸਤਰਾਂ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      ਜੇ (ਕੁੱਲ_ਸ਼ਬਦ_ਉਛਾਲ)
        {
          ਕੁੱਲ_ਸ਼ਬਦ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      ਜੇ (ਕੁੱਲ_chars_ਉਛਾਲ)
        {
          ਕੁੱਲ_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      ਜੇ (ਕੁੱਲ_ਬਾਈਟਾਂ_ਉਛਾਲ)
        {
          ਕੁੱਲ_ਬਾਈਟਾਂ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (ਕੁੱਲ_ਸਤਰਾਂ, ਕੁੱਲ_ਸ਼ਬਦ, ਕੁੱਲ_chars, ਕੁੱਲ_ਬਾਈਟਾਂ,
                    max_ਸਤਰ_ਲੰਮਾਈ,
                    ਕੁੱਲ_ਬਹੁਲਕ != ਕੁੱਲ_only ? _("total") : ਨਿਰਾ);
    }

  argv_iter_free (ai);

  ਮੁਕਤ (fstatus);

  ਜੇ (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
