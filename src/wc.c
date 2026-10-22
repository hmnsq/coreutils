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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ <argmatch.h>
#అన్తర్భావ <argv-iter.h>
#అన్తర్భావ <fadvise.h>
#అన్తర్భావ <physmem.h>
#అన్తర్భావ <readtokens0.h>
#అన్తర్భావ <stat-size.h>
#అన్తర్భావ <xbinary-io.h>

#నిర్వచితమైతే USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#అయితే_ముగింపు

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "cpu-supports.h"
#అన్తర్భావ "ioblksize.h"
#అన్తర్భావ "wc.h"
#అన్తర్భావ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "wc"

#నిర్వచించు AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

స్థిర bool wc_isprint[UCHAR_MAX + 1];
స్థిర bool wc_isspace[UCHAR_MAX + 1];

స్థిర bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
స్థిర uintmax_t మొత్తం_పంక్తులు;
స్థిర uintmax_t మొత్తం_పదాలు;
స్థిర uintmax_t మొత్తం_chars;
స్థిర uintmax_t మొత్తం_బైట్లు;
స్థిర bool మొత్తం_పంక్తులు_అధికప్రవాహం;
స్థిర bool మొత్తం_పదాలు_అధికప్రవాహం;
స్థిర bool మొత్తం_chars_అధికప్రవాహం;
స్థిర bool మొత్తం_బైట్లు_అధికప్రవాహం;
స్థిర intmax_t max_పంక్తి_నిడివి;

/* Which counts to print. */
స్థిర bool print_పంక్తులు, print_పదాలు, print_chars, print_బైట్లు;
స్థిర bool print_linelength;

/* The print width of each count.  */
స్థిర పూర్ణాంకం సంఖ్య_వెడల్పు;

/* True if we have ever read the standard input. */
స్థిర bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
స్థిర idx_t పుట_కొలత;

/* Enable to _not_ treat non breaking space as a word separator.  */
స్థిర bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
నిర్మాణం fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  పూర్ణాంకం failed;

  /* If FAILED is zero, this is the file's status.  */
  నిర్మాణం stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  DEBUG_కార్యక్రమం_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  మొత్తం_OPTION,
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"bytes", no_వాదం, ఖాళీ, 'c'},
  {"chars", no_వాదం, ఖాళీ, 'm'},
  {"lines", no_వాదం, ఖాళీ, 'l'},
  {"words", no_వాదం, ఖాళీ, 'w'},
  {"debug", no_వాదం, ఖాళీ, DEBUG_కార్యక్రమం_OPTION},
  {"files0-from", required_వాదం, ఖాళీ, FILES0_FROM_OPTION},
  {"max-line-length", no_వాదం, ఖాళీ, 'L'},
  {"total", required_వాదం, ఖాళీ, మొత్తం_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

గణన మొత్తం_type
  {
    మొత్తం_auto,         /* 0: default or --total=auto */
    మొత్తం_always,       /* 1: --total=always */
    మొత్తం_only,         /* 2: --total=only */
    మొత్తం_never         /* 3: --total=never */
  };
స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి మొత్తం_వాదనలు[] =
{
  "auto", "always", "only", "never", ఖాళీ
};
స్థిర గణన మొత్తం_type స్థిరరాశి మొత్తం_types[] =
{
  మొత్తం_auto, మొత్తం_always, మొత్తం_only, మొత్తం_never
};
ARGMATCH_VERIFY (మొత్తం_వాదనలు, మొత్తం_types);
స్థిర గణన మొత్తం_type మొత్తం_బహుళకం = మొత్తం_auto;

#నిర్వచితమైతే USE_AVX2_WC_LINECOUNT
స్థిర bool
avx2_supported (శూన్య)
{
  bool avx2_enabled = cpu_supports ("avx2");
  ఒకవేళ (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  తిరిగి avx2_enabled;
}
#అయితే_ముగింపు

#నిర్వచితమైతే USE_AVX512_WC_LINECOUNT
స్థిర bool
avx512_supported (శూన్య)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  ఒకవేళ (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  తిరిగి avx512_enabled;
}
#అయితే_ముగింపు

#నిర్వచితమైతే USE_NEON_WC_LINECOUNT
స్థిర bool
neon_supported (శూన్య)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  ఒకవేళ (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  తిరిగి neon_enabled;
}
#అయితే_ముగింపు

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Print newline, మాట_2, and byte counts కొరకు each FILE, and a మొత్తం పంక్తి ఒకవేళ\n\
మరిన్ని than one FILE is specified.  A మాట_2 is a nonempty sequence యొక్క non white\n\
space delimited by white space characters or by ప్రారంభం or చివర యొక్క ఆదానం.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
The options క్రింద may be వాడిన గమ్యం select which counts are printed, always లోపల\n\
the following క్రమం: newline, మాట_2, character, byte, గరిష్ఠ పంక్తి నిడివి.\n\
"), stdout);
      oputs (_("\
  -c, --బైట్లు\n\
         print the byte counts\n\
"));
      oputs (_("\
  -m, --chars\n\
         print the character counts\n\
"));
      oputs (_("\
  -l, --పంక్తులు\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what పంక్తి లెక్క acceleration is వాడిన\n\
"));
      oputs (_("\
      --files0-from=F\n\
         పఠ ఆదానం from the files specified by\n\
         NUL-terminated పేర్లు లోపల file F;\n\
         If F is -, పఠ పేర్లు from standard ఆదానం\n\
"));
      oputs (_("\
  -L, --max-పంక్తి-నిడివి\n\
         print the గరిష్ఠ display వెడల్పు\n\
"));
      oputs (_("\
  -w, --పదాలు\n\
         print the మాట_2 counts\n\
"));
      oputs (_("\
      --మొత్తం=WHEN\n\
         when గమ్యం print a పంక్తి with మొత్తం counts;\n\
         WHEN can be: స్వయంచాలక, always, only, never\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Return non zero if POSIXLY_CORRECT is not set and WC is a non breaking
   space.  */
ATTRIBUTE_శుద్ధ
స్థిర పూర్ణాంకం
maybe_c32isnbspace (char32_t wc)
{
  తిరిగి ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
స్థిర శూన్య
write_counts (uintmax_t పంక్తులు,
              uintmax_t పదాలు,
              uintmax_t chars,
              uintmax_t బైట్లు,
              intmax_t linelength,
              అక్షరం స్థిరరాశి *file)
{
  స్థిర అక్షరం స్థిరరాశి format_sp_int[] = " %*s";
  అక్షరం స్థిరరాశి *format_int = format_sp_int + 1;
  అక్షరం buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  ఒకవేళ (print_పంక్తులు)
    {
      రాయి (format_int, సంఖ్య_వెడల్పు, umaxtostr (పంక్తులు, buf));
      format_int = format_sp_int;
    }
  ఒకవేళ (print_పదాలు)
    {
      రాయి (format_int, సంఖ్య_వెడల్పు, umaxtostr (పదాలు, buf));
      format_int = format_sp_int;
    }
  ఒకవేళ (print_chars)
    {
      రాయి (format_int, సంఖ్య_వెడల్పు, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  ఒకవేళ (print_బైట్లు)
    {
      రాయి (format_int, సంఖ్య_వెడల్పు, umaxtostr (బైట్లు, buf));
      format_int = format_sp_int;
    }
  ఒకవేళ (print_linelength)
    రాయి (format_int, సంఖ్య_వెడల్పు, imaxtostr (linelength, buf));
  ఒకవేళ (file)
    రాయి (" %s", strchr (file, '\n') ? quotef (file) : file);
  అక్షరం_రాయి ('\n');

  ఒకవేళ (ferror (stdout))
    write_error ();
}

/* Read FD and return a summary.  */
స్థిర నిర్మాణం wc_పంక్తులు
wc_పంక్తులు (పూర్ణాంకం fd)
{
#నిర్వచితమైతే USE_AVX512_WC_LINECOUNT
  స్థిర గుర్తుగల అక్షరం use_avx512;
  ఒకవేళ (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  ఒకవేళ (0 < use_avx512)
    తిరిగి wc_పంక్తులు_avx512 (fd);
#అయితే_ముగింపు
#నిర్వచితమైతే USE_AVX2_WC_LINECOUNT
  స్థిర గుర్తుగల అక్షరం use_avx2;
  ఒకవేళ (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  ఒకవేళ (0 < use_avx2)
    తిరిగి wc_పంక్తులు_avx2 (fd);
#అయితే_ముగింపు
#నిర్వచితమైతే USE_NEON_WC_LINECOUNT
  స్థిర గుర్తుగల అక్షరం use_neon;
  ఒకవేళ (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  ఒకవేళ (0 < use_neon)
    తిరిగి wc_పంక్తులు_neon (fd);
#అయితే_ముగింపు

  intmax_t పంక్తులు = 0, బైట్లు = 0;
  bool long_పంక్తులు = false;

  అయితే (true)
    {
      అక్షరం buf[IO_BUFSIZE + 1];
      ssize_t బైట్లు_read = పఠ (fd, buf, IO_BUFSIZE);
      ఒకవేళ (బైట్లు_read <= 0)
        తిరిగి (నిర్మాణం wc_పంక్తులు) { బైట్లు_read == 0 ? 0 : errno, పంక్తులు, బైట్లు };

      బైట్లు += బైట్లు_read;
      అక్షరం *చివర = buf + బైట్లు_read;
      idx_t buflines = 0;

      ఒకవేళ (! long_పంక్తులు)
        {
          /* Avoid function call overhead for shorter lines.  */
          కొరకు (అక్షరం *p = buf; p < చివర; p++)
            buflines += *p == '\n';
        }
      లేకపోతే
        {
          /* rawmemchr is more efficient with longer lines.  */
          *చివర = '\n';
          కొరకు (అక్షరం *p = buf; (p = rawmemchr (p, '\n')) < చివర; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_పంక్తులు = 15 * buflines <= బైట్లు_read;
      పంక్తులు += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
స్థిర bool
wc (పూర్ణాంకం fd, అక్షరం స్థిరరాశి *file_x, నిర్మాణం fstatus *fstatus)
{
  పూర్ణాంకం err = 0;
  అక్షరం buf[IO_BUFSIZE + 1];
  intmax_t పంక్తులు, పదాలు, chars, బైట్లు, linelength;
  bool లెక్క_బైట్లు_2, లెక్క_chars, లెక్క_complicated;
  అక్షరం స్థిరరాశి *file = file_x ? file_x : _("standard input");

  పంక్తులు = పదాలు = chars = బైట్లు = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  ఒకవేళ (MB_CUR_MAX > 1)
    {
      లెక్క_బైట్లు_2 = print_బైట్లు;
      లెక్క_chars = print_chars;
    }
  లేకపోతే
    {
      లెక్క_బైట్లు_2 = print_బైట్లు || print_chars;
      లెక్క_chars = false;
    }
  లెక్క_complicated = print_పదాలు || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  ఒకవేళ (!లెక్క_బైట్లు_2 || లెక్క_chars || print_పంక్తులు || లెక్క_complicated)
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

  ఒకవేళ (లెక్క_బైట్లు_2 && !లెక్క_chars && !print_పంక్తులు && !లెక్క_complicated)
    {
      bool skip_read = false;

      ఒకవేళ (0 < fstatus->failed)
        fstatus->failed = fstat (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      ఒకవేళ (! fstatus->failed && usable_st_కొలత (&fstatus->st)
          && 0 <= fstatus->st.st_కొలత)
        {
          off_t చివర_pos = fstatus->st.st_కొలత;
          off_t current_pos = lseek (fd, 0, SEEK_CUR);
          ఒకవేళ (current_pos < 0)
            ;
          లేకపోతే ఒకవేళ (చివర_pos % పుట_కొలత)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              ఒకవేళ (current_pos < చివర_pos)
                {
                  బైట్లు = చివర_pos - current_pos;
                  skip_read = true;
                }
            }
          లేకపోతే ఒకవేళ (0 < చివర_pos)
            {
              off_t ep_1 = చివర_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              ఒకవేళ (current_pos < hi_pos)
                బైట్లు = hi_pos - current_pos;
            }

          ఒకవేళ (0 < బైట్లు && lseek (fd, బైట్లు, SEEK_CUR) < 0)
            {
              బైట్లు = 0;
              skip_read = false;
            }
        }

      ఒకవేళ (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          కొరకు (ssize_t బైట్లు_read;
               (బైట్లు_read = పఠ (fd, buf, IO_BUFSIZE));
               బైట్లు += బైట్లు_read)
            ఒకవేళ (బైట్లు_read < 0)
              {
                err = errno;
                ఆపు;
              }
        }
    }
  లేకపోతే ఒకవేళ (!లెక్క_chars && !లెక్క_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      నిర్మాణం wc_పంక్తులు w = wc_పంక్తులు (fd);
      err = w.err;
      పంక్తులు = w.పంక్తులు;
      బైట్లు = w.బైట్లు;
    }
  లేకపోతే ఒకవేళ (MB_CUR_MAX > 1)
    {
      bool లోపల_మాట = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool లోపల_జరుపు = false;
      bool లెక్క_utf8 = !లెక్క_complicated && !print_పంక్తులు
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      కొరకు (ssize_t బైట్లు_read;
           ((బైట్లు_read = పఠ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          ఒకవేళ (బైట్లు_read < 0)
            {
              err = errno;
              ఆపు;
            }

          బైట్లు += బైట్లు_read;
          అక్షరం స్థిరరాశి *p = buf;
          అక్షరం స్థిరరాశి *plim = p + prev + బైట్లు_read;
          bool try_లెక్క_utf8 = లెక్క_utf8;
          చేయి
            {
              ఒకవేళ (try_లెక్క_utf8 && !లోపల_జరుపు)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_లెక్క_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_లెక్క (p, &n);
                  p += n;
                  ఒకవేళ (p == plim)
                    ఆపు;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              ఒకవేళ (!లోపల_జరుపు && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              లేకపోతే
                {
                  idx_t prev_బైట్లు = prev;
                  idx_t scanbytes = plim - (p + prev);
                  size_t n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  ఒకవేళ (scanbytes < n)
                    {
                      ఒకవేళ (n == (size_t) -2 && plim - p < IO_BUFSIZE
                          && బైట్లు_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          memmove (buf, p, prev);
                          లోపల_జరుపు = true;
                          ఆపు;
                        }

                      /* Remember that we read a byte, but don't complain
                         about the error.  Because of the decoding error,
                         this is a considered to be byte but not a
                         character (that is, chars is not incremented).  */
                      p++;
                      mbszero (&state);
                      లోపల_జరుపు = false;

                      /* Treat encoding errors as non white space.
                         POSIX says a word is "a non-zero-length string of
                         characters delimited by white space".  This is
                         wrong in some sense, as the string can be delimited
                         by start or end of input, and it is unclear what it
                         means when the input contains encoding errors.
                         Since encoding errors are not white space,
                         treat them that way here.  */
                      పదాలు += !లోపల_మాట;
                      లోపల_మాట = true;
                      కొనసాగు;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_బైట్లు + n + !n;
                  single_byte = charbytes == !లోపల_జరుపు;
                  లోపల_జరుపు = !mbsinit (&state);
                }

              ఎంపిక (wide_char)
                {
                స్థితి '\n':
                  పంక్తులు++;
                  FALLTHROUGH;
                స్థితి '\r':
                స్థితి '\f':
                  ఒకవేళ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  లోపల_మాట = false;
                  ఆపు;

                స్థితి '\t':
                  linepos += 8 - (linepos % 8);
                  లోపల_మాట = false;
                  ఆపు;

                స్థితి ' ':
                  linepos++;
                  FALLTHROUGH;
                స్థితి '\v':
                  లోపల_మాట = false;
                  ఆపు;

                అప్రమేయ:;
                  bool లోపల_word2;
                  ఒకవేళ (single_byte && wide_char <= UCHAR_MAX)
                    {
                      linepos += wc_isprint[wide_char];
                      లోపల_word2 = !wc_isspace[wide_char];
                    }
                  లేకపోతే
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      ఒకవేళ (print_linelength)
                        {
                          పూర్ణాంకం వెడల్పు = c32width (wide_char);
                          ఒకవేళ (వెడల్పు > 0)
                            linepos += వెడల్పు;
                        }
                      లోపల_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  పదాలు += !లోపల_మాట & లోపల_word2;
                  లోపల_మాట = లోపల_word2;
                  ఆపు;
                }

              p += charbytes;
              chars++;
            }
          అయితే (p < plim);
        }
      ఒకవేళ (linepos > linelength)
        linelength = linepos;
    }
  లేకపోతే
    {
      bool లోపల_మాట = false;
      intmax_t linepos = 0;

      కొరకు (ssize_t బైట్లు_read; (బైట్లు_read = పఠ (fd, buf, IO_BUFSIZE)); )
        {
          ఒకవేళ (బైట్లు_read < 0)
            {
              err = errno;
              ఆపు;
            }

          బైట్లు += బైట్లు_read;
          అక్షరం స్థిరరాశి *p = buf;
          చేయి
            {
              గుర్తులేని అక్షరం c = *p++;
              ఎంపిక (c)
                {
                స్థితి '\n':
                  పంక్తులు++;
                  FALLTHROUGH;
                స్థితి '\r':
                స్థితి '\f':
                  ఒకవేళ (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  లోపల_మాట = false;
                  ఆపు;

                స్థితి '\t':
                  linepos += 8 - (linepos % 8);
                  లోపల_మాట = false;
                  ఆపు;

                స్థితి ' ':
                  linepos++;
                  FALLTHROUGH;
                స్థితి '\v':
                  లోపల_మాట = false;
                  ఆపు;

                అప్రమేయ:
                  linepos += wc_isprint[c];
                  bool లోపల_word2 = !wc_isspace[c];
                  పదాలు += !లోపల_మాట & లోపల_word2;
                  లోపల_మాట = లోపల_word2;
                  ఆపు;
                }
            }
          అయితే (--బైట్లు_read);
        }
      ఒకవేళ (linepos > linelength)
        linelength = linepos;
    }

  ఒకవేళ (లెక్క_chars < print_chars)
    chars = బైట్లు;

  ఒకవేళ (మొత్తం_బహుళకం != మొత్తం_only)
    write_counts (పంక్తులు, పదాలు, chars, బైట్లు, linelength, file_x);

  మొత్తం_పంక్తులు_అధికప్రవాహం |= ckd_కూడు (&మొత్తం_పంక్తులు, మొత్తం_పంక్తులు, పంక్తులు);
  మొత్తం_పదాలు_అధికప్రవాహం |= ckd_కూడు (&మొత్తం_పదాలు, మొత్తం_పదాలు, పదాలు);
  మొత్తం_chars_అధికప్రవాహం |= ckd_కూడు (&మొత్తం_chars, మొత్తం_chars, chars);
  మొత్తం_బైట్లు_అధికప్రవాహం |= ckd_కూడు (&మొత్తం_బైట్లు, మొత్తం_బైట్లు, బైట్లు);

  ఒకవేళ (linelength > max_పంక్తి_నిడివి)
    max_పంక్తి_నిడివి = linelength;

  ఒకవేళ (err)
    error (0, err, "%s", quotef (file));
  తిరిగి !err;
}

స్థిర bool
wc_file (అక్షరం స్థిరరాశి *file, నిర్మాణం fstatus *fstatus)
{
  ఒకవేళ (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_ద్విమాన_బహుళకం (STDIN_FILENO, O_ద్విమాన);
      తిరిగి wc (STDIN_FILENO, file, fstatus);
    }
  లేకపోతే
    {
      పూర్ణాంకం fd = open (file, O_RDONLY | O_ద్విమాన);
      ఒకవేళ (fd == -1)
        {
          error (0, errno, "%s", quotef (file));
          తిరిగి false;
        }
      లేకపోతే
        {
          bool ok = wc (fd, file, fstatus);
          ఒకవేళ (close (fd) != 0)
            {
              error (0, errno, "%s", quotef (file));
              తిరిగి false;
            }
          తిరిగి ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

స్థిర నిర్మాణం fstatus *
get_ఆదానం_fstatus (idx_t nfiles, అక్షరం *స్థిరరాశి *file)
{
  నిర్మాణం fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, పరిమాణం *fstatus);

  ఒకవేళ (nfiles == 0
      || (nfiles == 1
          && ((print_పంక్తులు + print_పదాలు + print_chars
               + print_బైట్లు + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  లేకపోతే
    {
      కొరకు (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? fstat (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  తిరిగి fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_శుద్ధ
స్థిర పూర్ణాంకం
compute_సంఖ్య_వెడల్పు (idx_t nfiles, నిర్మాణం fstatus స్థిరరాశి *fstatus)
{
  పూర్ణాంకం వెడల్పు = 1;

  ఒకవేళ (0 < nfiles && fstatus[0].failed <= 0)
    {
      పూర్ణాంకం minimum_వెడల్పు = 1;
      uintmax_t regular_మొత్తం = 0;

      కొరకు (idx_t i = 0; i < nfiles; i++)
        ఒకవేళ (! fstatus[i].failed)
          {
            ఒకవేళ (!S_ISREG (fstatus[i].st.st_బహుళకం))
              minimum_వెడల్పు = 7;
            లేకపోతే ఒకవేళ (ckd_కూడు (&regular_మొత్తం, regular_మొత్తం,
                              fstatus[i].st.st_కొలత))
              {
                regular_మొత్తం = UINTMAX_MAX;
                ఆపు;
              }
          }

      కొరకు (; 10 <= regular_మొత్తం; regular_మొత్తం /= 10)
        వెడల్పు++;
      ఒకవేళ (వెడల్పు < minimum_వెడల్పు)
        వెడల్పు = minimum_వెడల్పు;
    }

  తిరిగి వెడల్పు;
}


పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optc;
  idx_t nfiles;
  అక్షరం **files;
  అక్షరం *files_from = ఖాళీ;
  నిర్మాణం fstatus *fstatus;
  నిర్మాణం టోకెన్లు tok;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  పుట_కొలత = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ఖాళీ, _IOLBF, 0);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ఖాళీ);

  అయితే ((optc = getopt_long (argc, argv, "clLmw", longopts, ఖాళీ)) != -1)
    ఎంపిక (optc)
      {
      స్థితి 'c':
        print_బైట్లు = true;
        ఆపు;

      స్థితి 'm':
        print_chars = true;
        ఆపు;

      స్థితి 'l':
        print_పంక్తులు = true;
        ఆపు;

      స్థితి 'w':
        print_పదాలు = true;
        ఆపు;

      స్థితి 'L':
        print_linelength = true;
        ఆపు;

      స్థితి DEBUG_కార్యక్రమం_OPTION:
        debug = true;
        ఆపు;

      స్థితి FILES0_FROM_OPTION:
        files_from = optarg;
        ఆపు;

      స్థితి మొత్తం_OPTION:
        మొత్తం_బహుళకం = XARGMATCH ("--total", optarg, మొత్తం_వాదనలు, మొత్తం_types);
        ఆపు;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

      అప్రమేయ:
        usage (EXIT_FAILURE);
      }

  ఒకవేళ (! (print_పంక్తులు || print_పదాలు || print_chars || print_బైట్లు
         || print_linelength))
    print_పంక్తులు = print_పదాలు = print_బైట్లు = true;

  ఒకవేళ (print_linelength)
    కొరకు (పూర్ణాంకం i = 0; i <= UCHAR_MAX; i++)
      wc_isprint[i] = !!isprint (i);
  ఒకవేళ (print_పదాలు)
    కొరకు (పూర్ణాంకం i = 0; i <= UCHAR_MAX; i++)
      wc_isspace[i] = ఖాళీనా (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_టోకెన్లు = false;
  నిర్మాణం argv_iterator *ai;
  ఒకవేళ (files_from)
    {
      FILE *ప్రవాహం;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ఒకవేళ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      ఒకవేళ (streq (files_from, "-"))
        ప్రవాహం = stdin;
      లేకపోతే
        {
          ప్రవాహం = fopen (files_from, "r");
          ఒకవేళ (ప్రవాహం == ఖాళీ)
            error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      నిర్మాణం stat st;
      ఒకవేళ (fstat (fileno (ప్రవాహం), &st) == 0
          && S_ISREG (st.st_బహుళకం)
          && st.st_కొలత <= MIN (10 * 1024 * 1024, physmem_అందుబాటు () / 2))
        {
          read_టోకెన్లు = true;
          readtokens0_init (&tok);
          ఒకవేళ (! readtokens0 (ప్రవాహం, &tok) || fclose (ప్రవాహం) != 0)
            error (EXIT_FAILURE, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      లేకపోతే
        {
          files = ఖాళీ;
          nfiles = 0;
          ai = argv_iter_init_ప్రవాహం (ప్రవాహం);
        }
    }
  లేకపోతే
    {
      స్థిర అక్షరం *stdin_only[] = { ఖాళీ };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  ఒకవేళ (!ai)
    xalloc_die ();

  fstatus = get_ఆదానం_fstatus (nfiles, files);
  ఒకవేళ (మొత్తం_బహుళకం == మొత్తం_only)
    సంఖ్య_వెడల్పు = 1;  /* No extra padding, since no alignment requirement.  */
  లేకపోతే
    సంఖ్య_వెడల్పు = compute_సంఖ్య_వెడల్పు (nfiles, fstatus);

  bool ok = true;
  గణన argv_iter_err ai_err;
  అక్షరం *file_name;
  కొరకు (పూర్ణాంకం i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      ఒకవేళ (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      ఒకవేళ (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          ఒకవేళ (files_from == ఖాళీ)
            error (0, 0, "%s", _("invalid zero-length file name"));
          లేకపోతే
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_వాదనలు (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      ఒకవేళ (skip_file)
        ok = false;
      లేకపోతే
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      ఒకవేళ (! nfiles)
        fstatus[0].failed = 1;
    }
  ఎంపిక (ai_err)
    {
    స్థితి AI_ERR_EOF:
      ఆపు;

    స్థితి AI_ERR_READ:
      error (0, errno, _("%s: read error"), quotef (files_from));
      ok = false;
      ఆపు;

    స్థితి AI_ERR_MEM:
      xalloc_die ();

    స్థితి AI_ERR_OK: అప్రమేయ:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  ఒకవేళ (ok && !files_from && argv_iter_n_వాదనలు (ai) == 0)
    ok &= wc_file (ఖాళీ, &fstatus[0]);

  ఒకవేళ (read_టోకెన్లు)
    readtokens0_free (&tok);

  ఒకవేళ (మొత్తం_బహుళకం != మొత్తం_never
      && (మొత్తం_బహుళకం != మొత్తం_auto || 1 < argv_iter_n_వాదనలు (ai)))
    {
      ఒకవేళ (మొత్తం_పంక్తులు_అధికప్రవాహం)
        {
          మొత్తం_పంక్తులు = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      ఒకవేళ (మొత్తం_పదాలు_అధికప్రవాహం)
        {
          మొత్తం_పదాలు = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      ఒకవేళ (మొత్తం_chars_అధికప్రవాహం)
        {
          మొత్తం_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      ఒకవేళ (మొత్తం_బైట్లు_అధికప్రవాహం)
        {
          మొత్తం_బైట్లు = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (మొత్తం_పంక్తులు, మొత్తం_పదాలు, మొత్తం_chars, మొత్తం_బైట్లు,
                    max_పంక్తి_నిడివి,
                    మొత్తం_బహుళకం != మొత్తం_only ? _("total") : ఖాళీ);
    }

  argv_iter_free (ai);

  విడుదల (fstatus);

  ఒకవేళ (have_read_stdin && close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "-");

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
