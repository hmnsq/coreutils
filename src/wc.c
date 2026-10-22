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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "cpu-supports.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "wc.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "wc"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

ಸ್ಥಿರ bool wc_isprint[UCHAR_MAX + 1];
ಸ್ಥಿರ bool wc_isspace[UCHAR_MAX + 1];

ಸ್ಥಿರ bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
ಸ್ಥಿರ uintmax_t ಒಟ್ಟು_ಸಾಲುಗಳು;
ಸ್ಥಿರ uintmax_t ಒಟ್ಟು_ಪದಗಳು;
ಸ್ಥಿರ uintmax_t ಒಟ್ಟು_chars;
ಸ್ಥಿರ uintmax_t ಒಟ್ಟು_ಬೈಟುಗಳು;
ಸ್ಥಿರ bool ಒಟ್ಟು_ಸಾಲುಗಳು_ಅತಿಪ್ರವಾಹ;
ಸ್ಥಿರ bool ಒಟ್ಟು_ಪದಗಳು_ಅತಿಪ್ರವಾಹ;
ಸ್ಥಿರ bool ಒಟ್ಟು_chars_ಅತಿಪ್ರವಾಹ;
ಸ್ಥಿರ bool ಒಟ್ಟು_ಬೈಟುಗಳು_ಅತಿಪ್ರವಾಹ;
ಸ್ಥಿರ intmax_t max_ಸಾಲು_ಉದ್ದಳತೆ;

/* Which counts to print. */
ಸ್ಥಿರ bool print_ಸಾಲುಗಳು, print_ಪದಗಳು, print_chars, print_ಬೈಟುಗಳು;
ಸ್ಥಿರ bool print_linelength;

/* The print width of each count.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ_ಅಗಲ;

/* True if we have ever read the standard input. */
ಸ್ಥಿರ bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
ಸ್ಥಿರ idx_t ಪುಟ_ಗಾತ್ರ;

/* Enable to _not_ treat non breaking space as a word separator.  */
ಸ್ಥಿರ bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
ರಚನೆ fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  ಪೂರ್ಣಾಂಕ failed;

  /* If FAILED is zero, this is the file's status.  */
  ರಚನೆ stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  ಒಟ್ಟು_OPTION,
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"bytes", no_ವಾದ, ಖಾಲಿ, 'c'},
  {"chars", no_ವಾದ, ಖಾಲಿ, 'm'},
  {"lines", no_ವಾದ, ಖಾಲಿ, 'l'},
  {"words", no_ವಾದ, ಖಾಲಿ, 'w'},
  {"debug", no_ವಾದ, ಖಾಲಿ, DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION},
  {"files0-from", required_ವಾದ, ಖಾಲಿ, FILES0_FROM_OPTION},
  {"max-line-length", no_ವಾದ, ಖಾಲಿ, 'L'},
  {"total", required_ವಾದ, ಖಾಲಿ, ಒಟ್ಟು_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಎಣಿಕೆ ಒಟ್ಟು_type
  {
    ಒಟ್ಟು_auto,         /* 0: default or --total=auto */
    ಒಟ್ಟು_always,       /* 1: --total=always */
    ಒಟ್ಟು_only,         /* 2: --total=only */
    ಒಟ್ಟು_never         /* 3: --total=never */
  };
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ಒಟ್ಟು_ವಾದಗಳು[] =
{
  "auto", "always", "only", "never", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ ಒಟ್ಟು_type ಸ್ಥಿರಾಂಕ ಒಟ್ಟು_types[] =
{
  ಒಟ್ಟು_auto, ಒಟ್ಟು_always, ಒಟ್ಟು_only, ಒಟ್ಟು_never
};
ARGMATCH_VERIFY (ಒಟ್ಟು_ವಾದಗಳು, ಒಟ್ಟು_types);
ಸ್ಥಿರ ಎಣಿಕೆ ಒಟ್ಟು_type ಒಟ್ಟು_ಬಹುಲಕ = ಒಟ್ಟು_auto;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_AVX2_WC_LINECOUNT
ಸ್ಥಿರ bool
avx2_supported (ಶೂನ್ಯ)
{
  bool avx2_enabled = cpu_supports ("avx2");
  ಆದರೆ (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  ಹಿಂತಿರುಗಿ avx2_enabled;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_AVX512_WC_LINECOUNT
ಸ್ಥಿರ bool
avx512_supported (ಶೂನ್ಯ)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  ಆದರೆ (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  ಹಿಂತಿರುಗಿ avx512_enabled;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_NEON_WC_LINECOUNT
ಸ್ಥಿರ bool
neon_supported (ಶೂನ್ಯ)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  ಆದರೆ (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  ಹಿಂತಿರುಗಿ neon_enabled;
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print newline, ಶಬ್ದ_2, and byte counts ಗಾಗಿ each FILE, and a ಒಟ್ಟು ಸಾಲು ಆದರೆ\n\
ಇನ್ನಷ್ಟು than one FILE is specified.  A ಶಬ್ದ_2 is a nonempty sequence ದ non white\n\
space delimited by white space characters or by ಆರಂಭ or ಕೊನೆ ದ ಒಳಹರಿವು.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options ಕೆಳಗೆ may be ಬಳಸಿದ ಗಮ್ಯ select which counts are printed, always ಒಳ\n\
the following ಕ್ರಮ: newline, ಶಬ್ದ_2, character, byte, ಗರಿಷ್ಠ ಸಾಲು ಉದ್ದಳತೆ.\n\
"), stdout);
      oputs (_("\
  -c, --ಬೈಟುಗಳು\n\
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
         indicate what ಸಾಲು ಎಣಿಕೆ_2 acceleration is ಬಳಸಿದ\n\
"));
      oputs (_("\
      --files0-from=F\n\
         ಪಠ ಒಳಹರಿವು from the files specified by\n\
         NUL-terminated ಹೆಸರುಗಳು ಒಳ file F;\n\
         If F is -, ಪಠ ಹೆಸರುಗಳು from standard ಒಳಹರಿವು\n\
"));
      oputs (_("\
  -L, --max-ಸಾಲು-ಉದ್ದಳತೆ\n\
         print the ಗರಿಷ್ಠ display ಅಗಲ\n\
"));
      oputs (_("\
  -w, --ಪದಗಳು\n\
         print the ಶಬ್ದ_2 counts\n\
"));
      oputs (_("\
      --ಒಟ್ಟು=WHEN\n\
         when ಗಮ್ಯ print a ಸಾಲು with ಒಟ್ಟು counts;\n\
         WHEN can be: ಸ್ವಯಂಚಾಲಿತ, always, only, never\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Return non zero if POSIXLY_CORRECT is not set and WC is a non breaking
   space.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
maybe_c32isnbspace (char32_t wc)
{
  ಹಿಂತಿರುಗಿ ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
ಸ್ಥಿರ ಶೂನ್ಯ
write_counts (uintmax_t ಸಾಲುಗಳು,
              uintmax_t ಪದಗಳು,
              uintmax_t chars,
              uintmax_t ಬೈಟುಗಳು,
              intmax_t linelength,
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ format_sp_int[] = " %*s";
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format_int = format_sp_int + 1;
  ಅಕ್ಷರ buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  ಆದರೆ (print_ಸಾಲುಗಳು)
    {
      ಬರೆ (format_int, ಸಂಖ್ಯೆ_ಅಗಲ, umaxtostr (ಸಾಲುಗಳು, buf));
      format_int = format_sp_int;
    }
  ಆದರೆ (print_ಪದಗಳು)
    {
      ಬರೆ (format_int, ಸಂಖ್ಯೆ_ಅಗಲ, umaxtostr (ಪದಗಳು, buf));
      format_int = format_sp_int;
    }
  ಆದರೆ (print_chars)
    {
      ಬರೆ (format_int, ಸಂಖ್ಯೆ_ಅಗಲ, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  ಆದರೆ (print_ಬೈಟುಗಳು)
    {
      ಬರೆ (format_int, ಸಂಖ್ಯೆ_ಅಗಲ, umaxtostr (ಬೈಟುಗಳು, buf));
      format_int = format_sp_int;
    }
  ಆದರೆ (print_linelength)
    ಬರೆ (format_int, ಸಂಖ್ಯೆ_ಅಗಲ, imaxtostr (linelength, buf));
  ಆದರೆ (file)
    ಬರೆ (" %s", strchr (file, '\n') ? quotef (file) : file);
  ಅಕ್ಷರ_ಬರೆ ('\n');

  ಆದರೆ (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
ಸ್ಥಿರ ರಚನೆ wc_ಸಾಲುಗಳು
wc_ಸಾಲುಗಳು (ಪೂರ್ಣಾಂಕ fd)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_AVX512_WC_LINECOUNT
  ಸ್ಥಿರ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ use_avx512;
  ಆದರೆ (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  ಆದರೆ (0 < use_avx512)
    ಹಿಂತಿರುಗಿ wc_ಸಾಲುಗಳು_avx512 (fd);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_AVX2_WC_LINECOUNT
  ಸ್ಥಿರ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ use_avx2;
  ಆದರೆ (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  ಆದರೆ (0 < use_avx2)
    ಹಿಂತಿರುಗಿ wc_ಸಾಲುಗಳು_avx2 (fd);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_NEON_WC_LINECOUNT
  ಸ್ಥಿರ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ use_neon;
  ಆದರೆ (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  ಆದರೆ (0 < use_neon)
    ಹಿಂತಿರುಗಿ wc_ಸಾಲುಗಳು_neon (fd);
#ಆದರೆ_ಅಂತ್ಯ

  intmax_t ಸಾಲುಗಳು = 0, ಬೈಟುಗಳು = 0;
  bool long_ಸಾಲುಗಳು = false;

  ಆಗ (true)
    {
      ಅಕ್ಷರ buf[IO_BUFSIZE + 1];
      ssize_t ಬೈಟುಗಳು_read = ಪಠ (fd, buf, IO_BUFSIZE);
      ಆದರೆ (ಬೈಟುಗಳು_read <= 0)
        ಹಿಂತಿರುಗಿ (ರಚನೆ wc_ಸಾಲುಗಳು) { ಬೈಟುಗಳು_read == 0 ? 0 : errno, ಸಾಲುಗಳು, ಬೈಟುಗಳು };

      ಬೈಟುಗಳು += ಬೈಟುಗಳು_read;
      ಅಕ್ಷರ *ಕೊನೆ = buf + ಬೈಟುಗಳು_read;
      idx_t buflines = 0;

      ಆದರೆ (! long_ಸಾಲುಗಳು)
        {
          /* Avoid function call overhead for shorter lines.  */
          ಗಾಗಿ (ಅಕ್ಷರ *p = buf; p < ಕೊನೆ; p++)
            buflines += *p == '\n';
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* rawmemchr is more efficient with longer lines.  */
          *ಕೊನೆ = '\n';
          ಗಾಗಿ (ಅಕ್ಷರ *p = buf; (p = rawmemchr (p, '\n')) < ಕೊನೆ; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_ಸಾಲುಗಳು = 15 * buflines <= ಬೈಟುಗಳು_read;
      ಸಾಲುಗಳು += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
ಸ್ಥಿರ bool
wc (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_x, ರಚನೆ fstatus *fstatus)
{
  ಪೂರ್ಣಾಂಕ err = 0;
  ಅಕ್ಷರ buf[IO_BUFSIZE + 1];
  intmax_t ಸಾಲುಗಳು, ಪದಗಳು, chars, ಬೈಟುಗಳು, linelength;
  bool ಎಣಿಕೆ_ಬೈಟುಗಳು_2, ಎಣಿಕೆ_chars, ಎಣಿಕೆ_complicated;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file = file_x ? file_x : _("standard input");

  ಸಾಲುಗಳು = ಪದಗಳು = chars = ಬೈಟುಗಳು = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  ಆದರೆ (MB_CUR_MAX > 1)
    {
      ಎಣಿಕೆ_ಬೈಟುಗಳು_2 = print_ಬೈಟುಗಳು;
      ಎಣಿಕೆ_chars = print_chars;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಎಣಿಕೆ_ಬೈಟುಗಳು_2 = print_ಬೈಟುಗಳು || print_chars;
      ಎಣಿಕೆ_chars = false;
    }
  ಎಣಿಕೆ_complicated = print_ಪದಗಳು || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  ಆದರೆ (!ಎಣಿಕೆ_ಬೈಟುಗಳು_2 || ಎಣಿಕೆ_chars || print_ಸಾಲುಗಳು || ಎಣಿಕೆ_complicated)
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

  ಆದರೆ (ಎಣಿಕೆ_ಬೈಟುಗಳು_2 && !ಎಣಿಕೆ_chars && !print_ಸಾಲುಗಳು && !ಎಣಿಕೆ_complicated)
    {
      bool skip_read = false;

      ಆದರೆ (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      ಆದರೆ (! fstatus->failed && usable_st_ಗಾತ್ರ (&fstatus->st)
          && 0 <= fstatus->st.st_ಗಾತ್ರ)
        {
          off_t ಕೊನೆ_pos = fstatus->st.st_ಗಾತ್ರ;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          ಆದರೆ (current_pos < 0)
            ;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೊನೆ_pos % ಪುಟ_ಗಾತ್ರ)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              ಆದರೆ (current_pos < ಕೊನೆ_pos)
                {
                  ಬೈಟುಗಳು = ಕೊನೆ_pos - current_pos;
                  skip_read = true;
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 < ಕೊನೆ_pos)
            {
              off_t ep_1 = ಕೊನೆ_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              ಆದರೆ (current_pos < hi_pos)
                ಬೈಟುಗಳು = hi_pos - current_pos;
            }

          ಆದರೆ (0 < ಬೈಟುಗಳು && lseek (fd, ಬೈಟುಗಳು, SEEK_CUR) < 0)
            {
              ಬೈಟುಗಳು = 0;
              skip_read = false;
            }
        }

      ಆದರೆ (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          ಗಾಗಿ (ssize_t ಬೈಟುಗಳು_read;
               (ಬೈಟುಗಳು_read = ಪಠ (fd, buf, IO_BUFSIZE));
               ಬೈಟುಗಳು += ಬೈಟುಗಳು_read)
            ಆದರೆ (ಬೈಟುಗಳು_read < 0)
              {
                err = errno;
                ನಿಲ್ಲಿಸು;
              }
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಎಣಿಕೆ_chars && !ಎಣಿಕೆ_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      ರಚನೆ wc_ಸಾಲುಗಳು w = wc_ಸಾಲುಗಳು (fd);
      err = w.err;
      ಸಾಲುಗಳು = w.ಸಾಲುಗಳು;
      ಬೈಟುಗಳು = w.ಬೈಟುಗಳು;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (MB_CUR_MAX > 1)
    {
      bool ಒಳ_ಶಬ್ದ = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool ಒಳ_ಸರಿಸು = false;
      bool ಎಣಿಕೆ_utf8 = !ಎಣಿಕೆ_complicated && !print_ಸಾಲುಗಳು
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      ಗಾಗಿ (ssize_t ಬೈಟುಗಳು_read;
           ((ಬೈಟುಗಳು_read = ಪಠ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          ಆದರೆ (ಬೈಟುಗಳು_read < 0)
            {
              err = errno;
              ನಿಲ್ಲಿಸು;
            }

          ಬೈಟುಗಳು += ಬೈಟುಗಳು_read;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = buf;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *plim = p + prev + ಬೈಟುಗಳು_read;
          bool try_ಎಣಿಕೆ_utf8 = ಎಣಿಕೆ_utf8;
          ಮಾಡು
            {
              ಆದರೆ (try_ಎಣಿಕೆ_utf8 && !ಒಳ_ಸರಿಸು)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_ಎಣಿಕೆ_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_ಎಣಿಕೆ (p, &n);
                  p += n;
                  ಆದರೆ (p == plim)
                    ನಿಲ್ಲಿಸು;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              ಆದರೆ (!ಒಳ_ಸರಿಸು && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  idx_t prev_ಬೈಟುಗಳು = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  ಆದರೆ (scanbytes < n)
                    {
                      ಆದರೆ (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && ಬೈಟುಗಳು_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          ಒಳ_ಸರಿಸು = true;
                          ನಿಲ್ಲಿಸು;
                        }

                      /* Remember that we read a byte, but don't complain
                         about the error.  Because of the decoding error,
                         this is a considered to be byte but not a
                         character (that is, chars is not incremented).  */
                      p++;
                      mbszero (&state);
                      ಒಳ_ಸರಿಸು = false;

                      /* Treat encoding errors as non white space.
                         POSIX says a word is "a non-zero-length string of
                         characters delimited by white space".  This is
                         wrong in some sense, as the string can be delimited
                         by start or end of input, and it is unclear what it
                         means when the input contains encoding errors.
                         Since encoding errors are not white space,
                         treat them that way here.  */
                      ಪದಗಳು += !ಒಳ_ಶಬ್ದ;
                      ಒಳ_ಶಬ್ದ = true;
                      ಮುಂದುವರಿ;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_ಬೈಟುಗಳು + n + !n;
                  single_byte = charbytes == !ಒಳ_ಸರಿಸು;
                  ಒಳ_ಸರಿಸು = !mbsinit (&state);
                }

              ಆಯ್ಕೆ (wide_char)
                {
                ಸ್ಥಿತಿ '\n':
                  ಸಾಲುಗಳು++;
                  FALLTHROUGH;
                ಸ್ಥಿತಿ '\r':
                ಸ್ಥಿತಿ '\f':
                  ಆದರೆ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  ಒಳ_ಶಬ್ದ = false;
                  ನಿಲ್ಲಿಸು;

                ಸ್ಥಿತಿ '\t':
                  linepos += 8 - (linepos % 8);
                  ಒಳ_ಶಬ್ದ = false;
                  ನಿಲ್ಲಿಸು;

                ಸ್ಥಿತಿ ' ':
                  linepos++;
                  FALLTHROUGH;
                ಸ್ಥಿತಿ '\v':
                  ಒಳ_ಶಬ್ದ = false;
                  ನಿಲ್ಲಿಸು;

                ಪೂರ್ವನಿಯೋಜಿತ:;
                  bool ಒಳ_word2;
                  ಆದರೆ (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      ಒಳ_word2 = !wc_isspace[wide_char];
                    }
                  ಇಲ್ಲದಿದ್ದರೆ
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      ಆದರೆ (print_linelength)
                        {
                          ಪೂರ್ಣಾಂಕ ಅಗಲ = c32width (wide_char);
                          ಆದರೆ (ಅಗಲ > 0)
                            linepos += ಅಗಲ;
                        }
                      ಒಳ_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  ಪದಗಳು += !ಒಳ_ಶಬ್ದ & ಒಳ_word2;
                  ಒಳ_ಶಬ್ದ = ಒಳ_word2;
                  ನಿಲ್ಲಿಸು;
                }

              p += charbytes;
              chars++;
            }
          ಆಗ (p < plim);
        }
      ಆದರೆ (linepos > linelength)
        linelength = linepos;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      bool ಒಳ_ಶಬ್ದ = false;
      intmax_t linepos = 0;

      ಗಾಗಿ (ssize_t ಬೈಟುಗಳು_read; (ಬೈಟುಗಳು_read = ಪಠ (fd, buf, IO_BUFSIZE)); )
        {
          ಆದರೆ (ಬೈಟುಗಳು_read < 0)
            {
              err = errno;
              ನಿಲ್ಲಿಸು;
            }

          ಬೈಟುಗಳು += ಬೈಟುಗಳು_read;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = buf;
          ಮಾಡು
            {
              ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = *p++;
              ಆಯ್ಕೆ (c)
                {
                ಸ್ಥಿತಿ '\n':
                  ಸಾಲುಗಳು++;
                  FALLTHROUGH;
                ಸ್ಥಿತಿ '\r':
                ಸ್ಥಿತಿ '\f':
                  ಆದರೆ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  ಒಳ_ಶಬ್ದ = false;
                  ನಿಲ್ಲಿಸು;

                ಸ್ಥಿತಿ '\t':
                  linepos += 8 - (linepos % 8);
                  ಒಳ_ಶಬ್ದ = false;
                  ನಿಲ್ಲಿಸು;

                ಸ್ಥಿತಿ ' ':
                  linepos++;
                  FALLTHROUGH;
                ಸ್ಥಿತಿ '\v':
                  ಒಳ_ಶಬ್ದ = false;
                  ನಿಲ್ಲಿಸು;

                ಪೂರ್ವನಿಯೋಜಿತ:
                  linepos += wc_isprint[c];
                  bool ಒಳ_word2 = !wc_isspace[c];
                  ಪದಗಳು += !ಒಳ_ಶಬ್ದ & ಒಳ_word2;
                  ಒಳ_ಶಬ್ದ = ಒಳ_word2;
                  ನಿಲ್ಲಿಸು;
                }
            }
          ಆಗ (--ಬೈಟುಗಳು_read);
        }
      ಆದರೆ (linepos > linelength)
        linelength = linepos;
    }

  ಆದರೆ (ಎಣಿಕೆ_chars < print_chars)
    chars = ಬೈಟುಗಳು;

  ಆದರೆ (ಒಟ್ಟು_ಬಹುಲಕ != ಒಟ್ಟು_only)
    write_counts (ಸಾಲುಗಳು, ಪದಗಳು, chars, ಬೈಟುಗಳು, linelength, file_x);

  ಒಟ್ಟು_ಸಾಲುಗಳು_ಅತಿಪ್ರವಾಹ |= ckd_ಕೂಡಿಸು (&ಒಟ್ಟು_ಸಾಲುಗಳು, ಒಟ್ಟು_ಸಾಲುಗಳು, ಸಾಲುಗಳು);
  ಒಟ್ಟು_ಪದಗಳು_ಅತಿಪ್ರವಾಹ |= ckd_ಕೂಡಿಸು (&ಒಟ್ಟು_ಪದಗಳು, ಒಟ್ಟು_ಪದಗಳು, ಪದಗಳು);
  ಒಟ್ಟು_chars_ಅತಿಪ್ರವಾಹ |= ckd_ಕೂಡಿಸು (&ಒಟ್ಟು_chars, ಒಟ್ಟು_chars, chars);
  ಒಟ್ಟು_ಬೈಟುಗಳು_ಅತಿಪ್ರವಾಹ |= ckd_ಕೂಡಿಸು (&ಒಟ್ಟು_ಬೈಟುಗಳು, ಒಟ್ಟು_ಬೈಟುಗಳು, ಬೈಟುಗಳು);

  ಆದರೆ (linelength > max_ಸಾಲು_ಉದ್ದಳತೆ)
    max_ಸಾಲು_ಉದ್ದಳತೆ = linelength;

  ಆದರೆ (err)
    error (0, err, "%s", quotef (file));
  ಹಿಂತಿರುಗಿ !err;
}

ಸ್ಥಿರ bool
wc_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ರಚನೆ fstatus *fstatus)
{
  ಆದರೆ (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDIN_FILENO, O_ದ್ವಿಮಾನ);
      ಹಿಂತಿರುಗಿ wc (STDIN_FILENO, file, fstatus);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಪೂರ್ಣಾಂಕ fd = open (file, O_RDONLY | O_ದ್ವಿಮಾನ);
      ಆದರೆ (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          ಹಿಂತಿರುಗಿ false;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          bool ok = wc (fd, file, fstatus);
          ಆದರೆ (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              ಹಿಂತಿರುಗಿ false;
            }
          ಹಿಂತಿರುಗಿ ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

ಸ್ಥಿರ ರಚನೆ fstatus *
get_ಒಳಹರಿವು_fstatus (idx_t nfiles, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *file)
{
  ರಚನೆ fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, ಗಾತ್ರ *fstatus);

  ಆದರೆ (nfiles == 0
      || (nfiles == 1
          && ((print_ಸಾಲುಗಳು + print_ಪದಗಳು + print_chars
               + print_ಬೈಟುಗಳು + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಗಾಗಿ (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  ಹಿಂತಿರುಗಿ fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compute_ಸಂಖ್ಯೆ_ಅಗಲ (idx_t nfiles, ರಚನೆ fstatus ಸ್ಥಿರಾಂಕ *fstatus)
{
  ಪೂರ್ಣಾಂಕ ಅಗಲ = 1;

  ಆದರೆ (0 < nfiles && fstatus[0].failed <= 0)
    {
      ಪೂರ್ಣಾಂಕ minimum_ಅಗಲ = 1;
      uintmax_t regular_ಒಟ್ಟು = 0;

      ಗಾಗಿ (idx_t i = 0; i < nfiles; i++)
        ಆದರೆ (! fstatus[i].failed)
          {
            ಆದರೆ (!S_ISREG (fstatus[i].st.st_ಬಹುಲಕ))
              minimum_ಅಗಲ = 7;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ckd_ಕೂಡಿಸು (&regular_ಒಟ್ಟು, regular_ಒಟ್ಟು,
                              fstatus[i].st.st_ಗಾತ್ರ))
              {
                regular_ಒಟ್ಟು = UINTMAX_MAX;
                ನಿಲ್ಲಿಸು;
              }
          }

      ಗಾಗಿ (; 10 <= regular_ಒಟ್ಟು; regular_ಒಟ್ಟು /= 10)
        ಅಗಲ++;
      ಆದರೆ (ಅಗಲ < minimum_ಅಗಲ)
        ಅಗಲ = minimum_ಅಗಲ;
    }

  ಹಿಂತಿರುಗಿ ಅಗಲ;
}


ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ optc;
  idx_t nfiles;
  ಅಕ್ಷರ **files;
  ಅಕ್ಷರ *files_from = ಖಾಲಿ;
  ರಚನೆ fstatus *fstatus;
  ರಚನೆ ಟೋಕನುಗಳು tok;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪುಟ_ಗಾತ್ರ = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ಖಾಲಿ, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ಖಾಲಿ);

  ಆಗ ((optc = getopt_long (argc, argv, "clLmw", longopts, ಖಾಲಿ)) != -1)
    ಆಯ್ಕೆ (optc)
      {
      ಸ್ಥಿತಿ 'c':
        print_ಬೈಟುಗಳು = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'm':
        print_chars = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'l':
        print_ಸಾಲುಗಳು = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'w':
        print_ಪದಗಳು = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'L':
        print_linelength = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION:
        debug = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ FILES0_FROM_OPTION:
        files_from = optarg;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ ಒಟ್ಟು_OPTION:
        ಒಟ್ಟು_ಬಹುಲಕ = XARGMATCH ("--total", optarg, ಒಟ್ಟು_ವಾದಗಳು, ಒಟ್ಟು_types);
        ನಿಲ್ಲಿಸು;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

      ಪೂರ್ವನಿಯೋಜಿತ:
        usage (EXIT_FAILURE);
      }

  ಆದರೆ (! (print_ಸಾಲುಗಳು || print_ಪದಗಳು || print_chars || print_ಬೈಟುಗಳು
         || print_linelength))
    print_ಸಾಲುಗಳು = print_ಪದಗಳು = print_ಬೈಟುಗಳು = true;

  ಆದರೆ (print_linelength)
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  ಆದರೆ (print_ಪದಗಳು)
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = ಖಾಲಿಯೇ (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_ಟೋಕನುಗಳು = false;
  ರಚನೆ argv_iterator *ai;
  ಆದರೆ (files_from)
    {
      FILE *ಹರಿವು;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ಆದರೆ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      ಆದರೆ (streq (files_from, "-"))
        ಹರಿವು = stdin;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಹರಿವು = fopen (files_from, "r");
          ಆದರೆ (ಹರಿವು == ಖಾಲಿ)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      ರಚನೆ stat st;
      ಆದರೆ (fstat (fileno (ಹರಿವು), &st) == 0
          && S_ISREG (st.st_ಬಹುಲಕ)
          && st.st_ಗಾತ್ರ <= MIN (10 * 1024 * 1024, physmem_ಲಭ್ಯ () / 2))
        {
          read_ಟೋಕನುಗಳು = true;
          readtokens0_init (&tok);
          ಆದರೆ (! readtokens0 (ಹರಿವು, &tok) || fclose (ಹರಿವು) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          files = ಖಾಲಿ;
          nfiles = 0;
          ai = argv_iter_init_ಹರಿವು (ಹರಿವು);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರ ಅಕ್ಷರ *stdin_only[] = { ಖಾಲಿ };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  ಆದರೆ (!ai)
    xalloc_die ();

  fstatus = get_ಒಳಹರಿವು_fstatus (nfiles, files);
  ಆದರೆ (ಒಟ್ಟು_ಬಹುಲಕ == ಒಟ್ಟು_only)
    ಸಂಖ್ಯೆ_ಅಗಲ = 1;  /* No extra padding, since no alignment requirement.  */
  ಇಲ್ಲದಿದ್ದರೆ
    ಸಂಖ್ಯೆ_ಅಗಲ = compute_ಸಂಖ್ಯೆ_ಅಗಲ (nfiles, fstatus);

  bool ok = true;
  ಎಣಿಕೆ argv_iter_err ai_err;
  ಅಕ್ಷರ *file_name;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      ಆದರೆ (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      ಆದರೆ (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          ಆದರೆ (files_from == ಖಾಲಿ)
            error (0, 0, "%s", _("invalid zero-length file name"));
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_ವಾದಗಳು (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      ಆದರೆ (skip_file)
        ok = false;
      ಇಲ್ಲದಿದ್ದರೆ
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      ಆದರೆ (! nfiles)
        fstatus[0].failed = 1;
    }
  ಆಯ್ಕೆ (ai_err)
    {
    ಸ್ಥಿತಿ AI_ERR_EOF:
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ AI_ERR_MEM:
      xalloc_die ();

    ಸ್ಥಿತಿ AI_ERR_OK: ಪೂರ್ವನಿಯೋಜಿತ:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  ಆದರೆ (ok && !files_from && argv_iter_n_ವಾದಗಳು (ai) == 0)
    ok &= wc_file (ಖಾಲಿ, &fstatus[0]);

  ಆದರೆ (read_ಟೋಕನುಗಳು)
    readtokens0_free (&tok);

  ಆದರೆ (ಒಟ್ಟು_ಬಹುಲಕ != ಒಟ್ಟು_never
      && (ಒಟ್ಟು_ಬಹುಲಕ != ಒಟ್ಟು_auto || 1 < argv_iter_n_ವಾದಗಳು (ai)))
    {
      ಆದರೆ (ಒಟ್ಟು_ಸಾಲುಗಳು_ಅತಿಪ್ರವಾಹ)
        {
          ಒಟ್ಟು_ಸಾಲುಗಳು = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      ಆದರೆ (ಒಟ್ಟು_ಪದಗಳು_ಅತಿಪ್ರವಾಹ)
        {
          ಒಟ್ಟು_ಪದಗಳು = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      ಆದರೆ (ಒಟ್ಟು_chars_ಅತಿಪ್ರವಾಹ)
        {
          ಒಟ್ಟು_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      ಆದರೆ (ಒಟ್ಟು_ಬೈಟುಗಳು_ಅತಿಪ್ರವಾಹ)
        {
          ಒಟ್ಟು_ಬೈಟುಗಳು = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (ಒಟ್ಟು_ಸಾಲುಗಳು, ಒಟ್ಟು_ಪದಗಳು, ಒಟ್ಟು_chars, ಒಟ್ಟು_ಬೈಟುಗಳು,
                    max_ಸಾಲು_ಉದ್ದಳತೆ,
                    ಒಟ್ಟು_ಬಹುಲಕ != ಒಟ್ಟು_only ? _("total") : ಖಾಲಿ);
    }

  argv_iter_free (ai);

  ಬಿಡುಗಡೆ (fstatus);

  ಆದರೆ (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
