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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव <argmatch.h>
#अन्तर्भाव <argv-iter.h>
#अन्तर्भाव <fadvise.h>
#अन्तर्भाव <physmem.h>
#अन्तर्भाव <readtokens0.h>
#अन्तर्भाव <stat-size.h>
#अन्तर्भाव <xbinary-io.h>

#ifdef USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#endif

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "cpu-supports.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "wc.h"
#अन्तर्भाव "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "wc"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

स्थिर bool wc_isprint[अच_अक्षर_उच्च + 1];
स्थिर bool wc_isspace[अच_अक्षर_उच्च + 1];

स्थिर bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
स्थिर uintmax_t total_पङ्क्तिहरू;
स्थिर uintmax_t total_शब्दहरू;
स्थिर uintmax_t total_chars;
स्थिर uintmax_t total_bytes;
स्थिर bool total_पङ्क्तिहरू_overflow;
स्थिर bool total_शब्दहरू_overflow;
स्थिर bool total_chars_overflow;
स्थिर bool total_bytes_overflow;
स्थिर intmax_t max_पङ्क्ति_length;

/* Which counts to print. */
स्थिर bool print_पङ्क्तिहरू, print_शब्दहरू, print_chars, print_bytes;
स्थिर bool print_linelength;

/* The print width of each count.  */
स्थिर पूर्णाङ्क सङ्ख्या_width;

/* True if we have ever read the standard input. */
स्थिर bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
स्थिर idx_t page_size;

/* Enable to _not_ treat non breaking space as a word separator.  */
स्थिर bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
संरचना fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  पूर्णाङ्क failed;

  /* If FAILED is zero, this is the file's status.  */
  संरचना stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEBUG_PROGRAM_OPTION = अक्षर_उच्च + 1,
  FILES0_FROM_OPTION,
  TOTAL_OPTION,
};

स्थिर संरचना option अचर longopts[] =
{
  {"bytes", no_argument, रिक्त, 'c'},
  {"chars", no_argument, रिक्त, 'm'},
  {"lines", no_argument, रिक्त, 'l'},
  {"words", no_argument, रिक्त, 'w'},
  {"debug", no_argument, रिक्त, DEBUG_PROGRAM_OPTION},
  {"files0-from", required_argument, रिक्त, FILES0_FROM_OPTION},
  {"max-line-length", no_argument, रिक्त, 'L'},
  {"total", required_argument, रिक्त, TOTAL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

गणना total_type
  {
    total_auto,         /* 0: default or --total=auto */
    total_always,       /* 1: --total=always */
    total_only,         /* 2: --total=only */
    total_never         /* 3: --total=never */
  };
स्थिर वर्ण अचर *अचर total_args[] =
{
  "auto", "always", "only", "never", रिक्त
};
स्थिर गणना total_type अचर total_types[] =
{
  total_auto, total_always, total_only, total_never
};
ARGMATCH_VERIFY (total_args, total_types);
स्थिर गणना total_type total_mode = total_auto;

#ifdef USE_AVX2_WC_LINECOUNT
स्थिर bool
avx2_supported (शून्य)
{
  bool avx2_enabled = cpu_supports ("avx2");
  यदि (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  फर्काऊ avx2_enabled;
}
#endif

#ifdef USE_AVX512_WC_LINECOUNT
स्थिर bool
avx512_supported (शून्य)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  यदि (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  फर्काऊ avx512_enabled;
}
#endif

#ifdef USE_NEON_WC_LINECOUNT
स्थिर bool
neon_supported (शून्य)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  यदि (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  फर्काऊ neon_enabled;
}
#endif

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      ख_माला_दो (_("\
Print newline, word, and byte counts लागि each खाता, and a total पङ्क्ति यदि\n\
more than one खाता is specified.  A word is a nonempty sequence of non white\n\
space delimited by white space characters or by start or end of आगत.\n\
"), मानक_निकास);

      emit_stdin_note ();

      ख_माला_दो (_("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum पङ्क्ति length.\n\
"), मानक_निकास);
      oputs (_("\
  -c, --bytes\n\
         print the byte counts\n\
"));
      oputs (_("\
  -m, --chars\n\
         print the character counts\n\
"));
      oputs (_("\
  -l, --पङ्क्तिहरू\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what पङ्क्ति गन्ती acceleration is used\n\
"));
      oputs (_("\
      --files0-from=F\n\
         पठ आगत from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, पठ names from standard आगत\n\
"));
      oputs (_("\
  -L, --max-पङ्क्ति-length\n\
         print the maximum display width\n\
"));
      oputs (_("\
  -w, --शब्दहरू\n\
         print the word counts\n\
"));
      oputs (_("\
      --total=WHEN\n\
         when to print a पङ्क्ति with total counts;\n\
         WHEN can be: स्वतः, always, only, never\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Return non zero if POSIXLY_CORRECT is not set and WC is a non breaking
   space.  */
ATTRIBUTE_PURE
स्थिर पूर्णाङ्क
maybe_c32isnbspace (char32_t wc)
{
  फर्काऊ ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
स्थिर शून्य
write_counts (uintmax_t पङ्क्तिहरू,
              uintmax_t शब्दहरू,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              वर्ण अचर *file)
{
  स्थिर वर्ण अचर format_sp_int[] = " %*s";
  वर्ण अचर *format_int = format_sp_int + 1;
  वर्ण buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  यदि (print_पङ्क्तिहरू)
    {
      म_लिखो (format_int, सङ्ख्या_width, umaxtostr (पङ्क्तिहरू, buf));
      format_int = format_sp_int;
    }
  यदि (print_शब्दहरू)
    {
      म_लिखो (format_int, सङ्ख्या_width, umaxtostr (शब्दहरू, buf));
      format_int = format_sp_int;
    }
  यदि (print_chars)
    {
      म_लिखो (format_int, सङ्ख्या_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  यदि (print_bytes)
    {
      म_लिखो (format_int, सङ्ख्या_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  यदि (print_linelength)
    म_लिखो (format_int, सङ्ख्या_width, imaxtostr (linelength, buf));
  यदि (file)
    म_लिखो (" %s", म_अक्षर (file, '\n') ? quotef (file) : file);
  अक्षर_दो ('\n');

  यदि (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Read FD and return a summary.  */
स्थिर संरचना wc_पङ्क्तिहरू
wc_पङ्क्तिहरू (पूर्णाङ्क fd)
{
#ifdef USE_AVX512_WC_LINECOUNT
  स्थिर चिन्हित वर्ण use_avx512;
  यदि (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  यदि (0 < use_avx512)
    फर्काऊ wc_पङ्क्तिहरू_avx512 (fd);
#endif
#ifdef USE_AVX2_WC_LINECOUNT
  स्थिर चिन्हित वर्ण use_avx2;
  यदि (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  यदि (0 < use_avx2)
    फर्काऊ wc_पङ्क्तिहरू_avx2 (fd);
#endif
#ifdef USE_NEON_WC_LINECOUNT
  स्थिर चिन्हित वर्ण use_neon;
  यदि (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  यदि (0 < use_neon)
    फर्काऊ wc_पङ्क्तिहरू_neon (fd);
#endif

  intmax_t पङ्क्तिहरू = 0, bytes = 0;
  bool long_पङ्क्तिहरू = false;

  जबसम्म (true)
    {
      वर्ण buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = पठ (fd, buf, IO_BUFSIZE);
      यदि (bytes_read <= 0)
        फर्काऊ (संरचना wc_पङ्क्तिहरू) { bytes_read == 0 ? 0 : त्रुटि_सं, पङ्क्तिहरू, bytes };

      bytes += bytes_read;
      वर्ण *end = buf + bytes_read;
      idx_t buflines = 0;

      यदि (! long_पङ्क्तिहरू)
        {
          /* Avoid function call overhead for shorter lines.  */
          लागि (वर्ण *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      अन्यथा
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          लागि (वर्ण *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_पङ्क्तिहरू = 15 * buflines <= bytes_read;
      पङ्क्तिहरू += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
स्थिर bool
wc (पूर्णाङ्क fd, वर्ण अचर *file_x, संरचना fstatus *fstatus)
{
  पूर्णाङ्क err = 0;
  वर्ण buf[IO_BUFSIZE + 1];
  intmax_t पङ्क्तिहरू, शब्दहरू, chars, bytes, linelength;
  bool गन्ती_bytes, गन्ती_chars, गन्ती_complicated;
  वर्ण अचर *file = file_x ? file_x : _("standard input");

  पङ्क्तिहरू = शब्दहरू = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  यदि (MB_CUR_MAX > 1)
    {
      गन्ती_bytes = print_bytes;
      गन्ती_chars = print_chars;
    }
  अन्यथा
    {
      गन्ती_bytes = print_bytes || print_chars;
      गन्ती_chars = false;
    }
  गन्ती_complicated = print_शब्दहरू || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  यदि (!गन्ती_bytes || गन्ती_chars || print_पङ्क्तिहरू || गन्ती_complicated)
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

  यदि (गन्ती_bytes && !गन्ती_chars && !print_पङ्क्तिहरू && !गन्ती_complicated)
    {
      bool skip_read = false;

      यदि (0 < fstatus->failed)
        fstatus->failed = ख_स्थिति (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      यदि (! fstatus->failed && usable_st_size (&fstatus->st)
          && 0 <= fstatus->st.st_size)
        {
          off_t end_pos = fstatus->st.st_size;
          off_t current_pos = lseek (fd, 0, प्रस्तुत_से);
          यदि (current_pos < 0)
            ;
          अन्यथा यदि (end_pos % page_size)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              यदि (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          अन्यथा यदि (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              यदि (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          यदि (0 < bytes && lseek (fd, bytes, प्रस्तुत_से) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      यदि (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          लागि (ssize_t bytes_read;
               (bytes_read = पठ (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            यदि (bytes_read < 0)
              {
                err = त्रुटि_सं;
                रोक;
              }
        }
    }
  अन्यथा यदि (!गन्ती_chars && !गन्ती_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      संरचना wc_पङ्क्तिहरू w = wc_पङ्क्तिहरू (fd);
      err = w.err;
      पङ्क्तिहरू = w.पङ्क्तिहरू;
      bytes = w.bytes;
    }
  अन्यथा यदि (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool गन्ती_utf8 = !गन्ती_complicated && !print_पङ्क्तिहरू
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      लागि (ssize_t bytes_read;
           ((bytes_read = पठ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          यदि (bytes_read < 0)
            {
              err = त्रुटि_सं;
              रोक;
            }

          bytes += bytes_read;
          वर्ण अचर *p = buf;
          वर्ण अचर *plim = p + prev + bytes_read;
          bool try_गन्ती_utf8 = गन्ती_utf8;
          गर
            {
              यदि (try_गन्ती_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_गन्ती_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_गन्ती (p, &n);
                  p += n;
                  यदि (p == plim)
                    रोक;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              यदि (!in_shift && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              अन्यथा
                {
                  idx_t prev_bytes = prev;
                  idx_t scanbytes = plim - (p + prev);
                  माप_प्रकार n = mbrtoc32 (&wide_char, p + prev, scanbytes, &state);
                  prev = 0;

                  यदि (scanbytes < n)
                    {
                      यदि (n == (माप_प्रकार) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          स_हटाओ (buf, p, prev);
                          in_shift = true;
                          रोक;
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
                      शब्दहरू += !in_word;
                      in_word = true;
                      जारी;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              विकल्प (wide_char)
                {
                पक्ष '\n':
                  पङ्क्तिहरू++;
                  FALLTHROUGH;
                पक्ष '\r':
                पक्ष '\f':
                  यदि (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  रोक;

                पक्ष '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  रोक;

                पक्ष ' ':
                  linepos++;
                  FALLTHROUGH;
                पक्ष '\v':
                  in_word = false;
                  रोक;

                स्वभाव:;
                  bool in_word2;
                  यदि (single_byte && wide_char <= अच_अक्षर_उच्च)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  अन्यथा
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      यदि (print_linelength)
                        {
                          पूर्णाङ्क width = c32width (wide_char);
                          यदि (width > 0)
                            linepos += width;
                        }
                      in_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  शब्दहरू += !in_word & in_word2;
                  in_word = in_word2;
                  रोक;
                }

              p += charbytes;
              chars++;
            }
          जबसम्म (p < plim);
        }
      यदि (linepos > linelength)
        linelength = linepos;
    }
  अन्यथा
    {
      bool in_word = false;
      intmax_t linepos = 0;

      लागि (ssize_t bytes_read; (bytes_read = पठ (fd, buf, IO_BUFSIZE)); )
        {
          यदि (bytes_read < 0)
            {
              err = त्रुटि_सं;
              रोक;
            }

          bytes += bytes_read;
          वर्ण अचर *p = buf;
          गर
            {
              अचिन्हित वर्ण c = *p++;
              विकल्प (c)
                {
                पक्ष '\n':
                  पङ्क्तिहरू++;
                  FALLTHROUGH;
                पक्ष '\r':
                पक्ष '\f':
                  यदि (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  रोक;

                पक्ष '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  रोक;

                पक्ष ' ':
                  linepos++;
                  FALLTHROUGH;
                पक्ष '\v':
                  in_word = false;
                  रोक;

                स्वभाव:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  शब्दहरू += !in_word & in_word2;
                  in_word = in_word2;
                  रोक;
                }
            }
          जबसम्म (--bytes_read);
        }
      यदि (linepos > linelength)
        linelength = linepos;
    }

  यदि (गन्ती_chars < print_chars)
    chars = bytes;

  यदि (total_mode != total_only)
    write_counts (पङ्क्तिहरू, शब्दहरू, chars, bytes, linelength, file_x);

  total_पङ्क्तिहरू_overflow |= ckd_add (&total_पङ्क्तिहरू, total_पङ्क्तिहरू, पङ्क्तिहरू);
  total_शब्दहरू_overflow |= ckd_add (&total_शब्दहरू, total_शब्दहरू, शब्दहरू);
  total_chars_overflow |= ckd_add (&total_chars, total_chars, chars);
  total_bytes_overflow |= ckd_add (&total_bytes, total_bytes, bytes);

  यदि (linelength > max_पङ्क्ति_length)
    max_पङ्क्ति_length = linelength;

  यदि (err)
    error (0, err, "%s", quotef (file));
  फर्काऊ !err;
}

स्थिर bool
wc_file (वर्ण अचर *file, संरचना fstatus *fstatus)
{
  यदि (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_द्विआधारी_mode (STDIN_FILENO, O_द्विआधारी);
      फर्काऊ wc (STDIN_FILENO, file, fstatus);
    }
  अन्यथा
    {
      पूर्णाङ्क fd = खोलो (file, O_RDONLY | O_द्विआधारी);
      यदि (fd == -1)
        {
          error (0, त्रुटि_सं, "%s", quotef (file));
          फर्काऊ false;
        }
      अन्यथा
        {
          bool ok = wc (fd, file, fstatus);
          यदि (बंद (fd) != 0)
            {
              error (0, त्रुटि_सं, "%s", quotef (file));
              फर्काऊ false;
            }
          फर्काऊ ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

स्थिर संरचना fstatus *
get_आगत_fstatus (idx_t nfiles, वर्ण *अचर *file)
{
  संरचना fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, परिमाण *fstatus);

  यदि (nfiles == 0
      || (nfiles == 1
          && ((print_पङ्क्तिहरू + print_शब्दहरू + print_chars
               + print_bytes + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  अन्यथा
    {
      लागि (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? ख_स्थिति (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  फर्काऊ fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_PURE
स्थिर पूर्णाङ्क
compute_सङ्ख्या_width (idx_t nfiles, संरचना fstatus अचर *fstatus)
{
  पूर्णाङ्क width = 1;

  यदि (0 < nfiles && fstatus[0].failed <= 0)
    {
      पूर्णाङ्क minimum_width = 1;
      uintmax_t regular_total = 0;

      लागि (idx_t i = 0; i < nfiles; i++)
        यदि (! fstatus[i].failed)
          {
            यदि (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            अन्यथा यदि (ckd_add (&regular_total, regular_total,
                              fstatus[i].st.st_size))
              {
                regular_total = UINTMAX_MAX;
                रोक;
              }
          }

      लागि (; 10 <= regular_total; regular_total /= 10)
        width++;
      यदि (width < minimum_width)
        width = minimum_width;
    }

  फर्काऊ width;
}


पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc;
  idx_t nfiles;
  वर्ण **files;
  वर्ण *files_from = रिक्त;
  संरचना fstatus *fstatus;
  संरचना Tokens tok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  page_size = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  रखो_भबफ (मानक_निकास, रिक्त, _IOLBF, 0);

  posixly_correct = (दो_पर्या ("POSIXLY_CORRECT") != रिक्त);

  जबसम्म ((optc = getopt_long (argc, argv, "clLmw", longopts, रिक्त)) != -1)
    विकल्प (optc)
      {
      पक्ष 'c':
        print_bytes = true;
        रोक;

      पक्ष 'm':
        print_chars = true;
        रोक;

      पक्ष 'l':
        print_पङ्क्तिहरू = true;
        रोक;

      पक्ष 'w':
        print_शब्दहरू = true;
        रोक;

      पक्ष 'L':
        print_linelength = true;
        रोक;

      पक्ष DEBUG_PROGRAM_OPTION:
        debug = true;
        रोक;

      पक्ष FILES0_FROM_OPTION:
        files_from = optarg;
        रोक;

      पक्ष TOTAL_OPTION:
        total_mode = XARGMATCH ("--total", optarg, total_args, total_types);
        रोक;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      स्वभाव:
        usage (निकास_त्रुटि);
      }

  यदि (! (print_पङ्क्तिहरू || print_शब्दहरू || print_chars || print_bytes
         || print_linelength))
    print_पङ्क्तिहरू = print_शब्दहरू = print_bytes = true;

  यदि (print_linelength)
    लागि (पूर्णाङ्क i = 0; i <= अच_अक्षर_उच्च; i++)
      wc_isprint[i] = !!है_छाप (i);
  यदि (print_शब्दहरू)
    लागि (पूर्णाङ्क i = 0; i <= अच_अक्षर_उच्च; i++)
      wc_isspace[i] = है_खाली (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  संरचना argv_iterator *ai;
  यदि (files_from)
    {
      खाता *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      यदि (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          ख_लिखो (मानक_त्रुटि, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (निकास_त्रुटि);
        }

      यदि (streq (files_from, "-"))
        stream = मानक_निवेश;
      अन्यथा
        {
          stream = ख_खोलो (files_from, "r");
          यदि (stream == रिक्त)
            error (निकास_त्रुटि, त्रुटि_सं, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      संरचना stat st;
      यदि (ख_स्थिति (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_size <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          यदि (! readtokens0 (stream, &tok) || ख_बंद (stream) != 0)
            error (निकास_त्रुटि, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      अन्यथा
        {
          files = रिक्त;
          nfiles = 0;
          ai = argv_iter_init_stream (stream);
        }
    }
  अन्यथा
    {
      स्थिर वर्ण *stdin_only[] = { रिक्त };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  यदि (!ai)
    xalloc_die ();

  fstatus = get_आगत_fstatus (nfiles, files);
  यदि (total_mode == total_only)
    सङ्ख्या_width = 1;  /* No extra padding, since no alignment requirement.  */
  अन्यथा
    सङ्ख्या_width = compute_सङ्ख्या_width (nfiles, fstatus);

  bool ok = true;
  गणना argv_iter_err ai_err;
  वर्ण *file_name;
  लागि (पूर्णाङ्क i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      यदि (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      यदि (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          यदि (files_from == रिक्त)
            error (0, 0, "%s", _("invalid zero-length file name"));
          अन्यथा
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              error (0, 0, "%s:%zu: %s", quotef (files_from),
                     argv_iter_n_args (ai), _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      यदि (skip_file)
        ok = false;
      अन्यथा
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      यदि (! nfiles)
        fstatus[0].failed = 1;
    }
  विकल्प (ai_err)
    {
    पक्ष AI_ERR_EOF:
      रोक;

    पक्ष AI_ERR_READ:
      error (0, त्रुटि_सं, _("%s: read error"), quotef (files_from));
      ok = false;
      रोक;

    पक्ष AI_ERR_MEM:
      xalloc_die ();

    पक्ष AI_ERR_OK: स्वभाव:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  यदि (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (रिक्त, &fstatus[0]);

  यदि (read_tokens)
    readtokens0_free (&tok);

  यदि (total_mode != total_never
      && (total_mode != total_auto || 1 < argv_iter_n_args (ai)))
    {
      यदि (total_पङ्क्तिहरू_overflow)
        {
          total_पङ्क्तिहरू = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      यदि (total_शब्दहरू_overflow)
        {
          total_शब्दहरू = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      यदि (total_chars_overflow)
        {
          total_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      यदि (total_bytes_overflow)
        {
          total_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (total_पङ्क्तिहरू, total_शब्दहरू, total_chars, total_bytes,
                    max_पङ्क्ति_length,
                    total_mode != total_only ? _("total") : रिक्त);
    }

  argv_iter_free (ai);

  मुक्त (fstatus);

  यदि (have_read_stdin && बंद (STDIN_FILENO) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "-");

  फर्काऊ ok ? निकास_सफल : निकास_त्रुटि;
}
