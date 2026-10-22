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
स्थिर uintmax_t total_ओळी;
स्थिर uintmax_t total_शब्द;
स्थिर uintmax_t total_chars;
स्थिर uintmax_t total_bytes;
स्थिर bool total_ओळी_overflow;
स्थिर bool total_शब्द_overflow;
स्थिर bool total_chars_overflow;
स्थिर bool total_bytes_overflow;
स्थिर intmax_t max_ओळ_length;

/* Which counts to print. */
स्थिर bool print_ओळी, print_शब्द, print_chars, print_bytes;
स्थिर bool print_linelength;

/* The print width of each count.  */
स्थिर पूर्णांक संख्या_width;

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
  पूर्णांक failed;

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

स्थिर संरचना option स्थिरांक longopts[] =
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
स्थिर अक्षर स्थिरांक *स्थिरांक total_args[] =
{
  "auto", "always", "only", "never", रिक्त
};
स्थिर गणना total_type स्थिरांक total_types[] =
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
  जर (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  परत avx2_enabled;
}
#endif

#ifdef USE_AVX512_WC_LINECOUNT
स्थिर bool
avx512_supported (शून्य)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  जर (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  परत avx512_enabled;
}
#endif

#ifdef USE_NEON_WC_LINECOUNT
स्थिर bool
neon_supported (शून्य)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  जर (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  परत neon_enabled;
}
#endif

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      ख_माला_दो (_("\
Print newline, word, and byte counts साठी each खाता, and a total ओळ जर\n\
more than one खाता is specified.  A word is a nonempty sequence of non white\n\
space delimited by white space characters or by start or end of आदान.\n\
"), मानक_निकास);

      emit_stdin_note ();

      ख_माला_दो (_("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum ओळ length.\n\
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
  -l, --ओळी\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what ओळ मोजणी acceleration is used\n\
"));
      oputs (_("\
      --files0-from=F\n\
         पठ आदान from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, पठ names from standard आदान\n\
"));
      oputs (_("\
  -L, --max-ओळ-length\n\
         print the maximum display width\n\
"));
      oputs (_("\
  -w, --शब्द\n\
         print the word counts\n\
"));
      oputs (_("\
      --total=WHEN\n\
         when to print a ओळ with total counts;\n\
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
स्थिर पूर्णांक
maybe_c32isnbspace (char32_t wc)
{
  परत ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
स्थिर शून्य
write_counts (uintmax_t ओळी,
              uintmax_t शब्द,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              अक्षर स्थिरांक *file)
{
  स्थिर अक्षर स्थिरांक format_sp_int[] = " %*s";
  अक्षर स्थिरांक *format_int = format_sp_int + 1;
  अक्षर buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  जर (print_ओळी)
    {
      म_लिखो (format_int, संख्या_width, umaxtostr (ओळी, buf));
      format_int = format_sp_int;
    }
  जर (print_शब्द)
    {
      म_लिखो (format_int, संख्या_width, umaxtostr (शब्द, buf));
      format_int = format_sp_int;
    }
  जर (print_chars)
    {
      म_लिखो (format_int, संख्या_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  जर (print_bytes)
    {
      म_लिखो (format_int, संख्या_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  जर (print_linelength)
    म_लिखो (format_int, संख्या_width, imaxtostr (linelength, buf));
  जर (file)
    म_लिखो (" %s", म_अक्षर (file, '\n') ? quotef (file) : file);
  अक्षर_दो ('\n');

  जर (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Read FD and return a summary.  */
स्थिर संरचना wc_ओळी
wc_ओळी (पूर्णांक fd)
{
#ifdef USE_AVX512_WC_LINECOUNT
  स्थिर चिन्हित अक्षर use_avx512;
  जर (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  जर (0 < use_avx512)
    परत wc_ओळी_avx512 (fd);
#endif
#ifdef USE_AVX2_WC_LINECOUNT
  स्थिर चिन्हित अक्षर use_avx2;
  जर (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  जर (0 < use_avx2)
    परत wc_ओळी_avx2 (fd);
#endif
#ifdef USE_NEON_WC_LINECOUNT
  स्थिर चिन्हित अक्षर use_neon;
  जर (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  जर (0 < use_neon)
    परत wc_ओळी_neon (fd);
#endif

  intmax_t ओळी = 0, bytes = 0;
  bool long_ओळी = false;

  पर्यंत (true)
    {
      अक्षर buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = पठ (fd, buf, IO_BUFSIZE);
      जर (bytes_read <= 0)
        परत (संरचना wc_ओळी) { bytes_read == 0 ? 0 : त्रुटि_सं, ओळी, bytes };

      bytes += bytes_read;
      अक्षर *end = buf + bytes_read;
      idx_t buflines = 0;

      जर (! long_ओळी)
        {
          /* Avoid function call overhead for shorter lines.  */
          साठी (अक्षर *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      अन्यथा
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          साठी (अक्षर *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_ओळी = 15 * buflines <= bytes_read;
      ओळी += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
स्थिर bool
wc (पूर्णांक fd, अक्षर स्थिरांक *file_x, संरचना fstatus *fstatus)
{
  पूर्णांक err = 0;
  अक्षर buf[IO_BUFSIZE + 1];
  intmax_t ओळी, शब्द, chars, bytes, linelength;
  bool मोजणी_bytes, मोजणी_chars, मोजणी_complicated;
  अक्षर स्थिरांक *file = file_x ? file_x : _("standard input");

  ओळी = शब्द = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  जर (MB_CUR_MAX > 1)
    {
      मोजणी_bytes = print_bytes;
      मोजणी_chars = print_chars;
    }
  अन्यथा
    {
      मोजणी_bytes = print_bytes || print_chars;
      मोजणी_chars = false;
    }
  मोजणी_complicated = print_शब्द || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  जर (!मोजणी_bytes || मोजणी_chars || print_ओळी || मोजणी_complicated)
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

  जर (मोजणी_bytes && !मोजणी_chars && !print_ओळी && !मोजणी_complicated)
    {
      bool skip_read = false;

      जर (0 < fstatus->failed)
        fstatus->failed = ख_स्थिति (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      जर (! fstatus->failed && usable_st_size (&fstatus->st)
          && 0 <= fstatus->st.st_size)
        {
          off_t end_pos = fstatus->st.st_size;
          off_t current_pos = lseek (fd, 0, प्रस्तुत_से);
          जर (current_pos < 0)
            ;
          अन्यथा जर (end_pos % page_size)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              जर (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          अन्यथा जर (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              जर (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          जर (0 < bytes && lseek (fd, bytes, प्रस्तुत_से) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      जर (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          साठी (ssize_t bytes_read;
               (bytes_read = पठ (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            जर (bytes_read < 0)
              {
                err = त्रुटि_सं;
                थांबा;
              }
        }
    }
  अन्यथा जर (!मोजणी_chars && !मोजणी_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      संरचना wc_ओळी w = wc_ओळी (fd);
      err = w.err;
      ओळी = w.ओळी;
      bytes = w.bytes;
    }
  अन्यथा जर (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool मोजणी_utf8 = !मोजणी_complicated && !print_ओळी
                        && is_utf8_charset ();
      idx_t prev = 0; /* Number of bytes carried over from previous round.  */

      साठी (ssize_t bytes_read;
           ((bytes_read = पठ (fd, buf + prev, IO_BUFSIZE - prev))
            || prev);
           )
        {
          जर (bytes_read < 0)
            {
              err = त्रुटि_सं;
              थांबा;
            }

          bytes += bytes_read;
          अक्षर स्थिरांक *p = buf;
          अक्षर स्थिरांक *plim = p + prev + bytes_read;
          bool try_मोजणी_utf8 = मोजणी_utf8;
          करा
            {
              जर (try_मोजणी_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_मोजणी_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_मोजणी (p, &n);
                  p += n;
                  जर (p == plim)
                    थांबा;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              जर (!in_shift && 0 <= *p && *p < 0x80)
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

                  जर (scanbytes < n)
                    {
                      जर (n == (माप_प्रकार) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          prev = plim - p;
                          स_हटाओ (buf, p, prev);
                          in_shift = true;
                          थांबा;
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
                      शब्द += !in_word;
                      in_word = true;
                      सुरूठेवा;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = prev_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              निवड (wide_char)
                {
                स्थिती '\n':
                  ओळी++;
                  FALLTHROUGH;
                स्थिती '\r':
                स्थिती '\f':
                  जर (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  थांबा;

                स्थिती '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  थांबा;

                स्थिती ' ':
                  linepos++;
                  FALLTHROUGH;
                स्थिती '\v':
                  in_word = false;
                  थांबा;

                पूर्वनिर्धारित:;
                  bool in_word2;
                  जर (single_byte && wide_char <= अच_अक्षर_उच्च)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  अन्यथा
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      जर (print_linelength)
                        {
                          पूर्णांक width = c32width (wide_char);
                          जर (width > 0)
                            linepos += width;
                        }
                      in_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  शब्द += !in_word & in_word2;
                  in_word = in_word2;
                  थांबा;
                }

              p += charbytes;
              chars++;
            }
          पर्यंत (p < plim);
        }
      जर (linepos > linelength)
        linelength = linepos;
    }
  अन्यथा
    {
      bool in_word = false;
      intmax_t linepos = 0;

      साठी (ssize_t bytes_read; (bytes_read = पठ (fd, buf, IO_BUFSIZE)); )
        {
          जर (bytes_read < 0)
            {
              err = त्रुटि_सं;
              थांबा;
            }

          bytes += bytes_read;
          अक्षर स्थिरांक *p = buf;
          करा
            {
              अचिन्हित अक्षर c = *p++;
              निवड (c)
                {
                स्थिती '\n':
                  ओळी++;
                  FALLTHROUGH;
                स्थिती '\r':
                स्थिती '\f':
                  जर (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  थांबा;

                स्थिती '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  थांबा;

                स्थिती ' ':
                  linepos++;
                  FALLTHROUGH;
                स्थिती '\v':
                  in_word = false;
                  थांबा;

                पूर्वनिर्धारित:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  शब्द += !in_word & in_word2;
                  in_word = in_word2;
                  थांबा;
                }
            }
          पर्यंत (--bytes_read);
        }
      जर (linepos > linelength)
        linelength = linepos;
    }

  जर (मोजणी_chars < print_chars)
    chars = bytes;

  जर (total_mode != total_only)
    write_counts (ओळी, शब्द, chars, bytes, linelength, file_x);

  total_ओळी_overflow |= ckd_add (&total_ओळी, total_ओळी, ओळी);
  total_शब्द_overflow |= ckd_add (&total_शब्द, total_शब्द, शब्द);
  total_chars_overflow |= ckd_add (&total_chars, total_chars, chars);
  total_bytes_overflow |= ckd_add (&total_bytes, total_bytes, bytes);

  जर (linelength > max_ओळ_length)
    max_ओळ_length = linelength;

  जर (err)
    error (0, err, "%s", quotef (file));
  परत !err;
}

स्थिर bool
wc_file (अक्षर स्थिरांक *file, संरचना fstatus *fstatus)
{
  जर (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
      परत wc (STDIN_FILENO, file, fstatus);
    }
  अन्यथा
    {
      पूर्णांक fd = खोलो (file, O_RDONLY | O_द्विमान);
      जर (fd == -1)
        {
          error (0, त्रुटि_सं, "%s", quotef (file));
          परत false;
        }
      अन्यथा
        {
          bool ok = wc (fd, file, fstatus);
          जर (बंद (fd) != 0)
            {
              error (0, त्रुटि_सं, "%s", quotef (file));
              परत false;
            }
          परत ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

स्थिर संरचना fstatus *
get_आदान_fstatus (idx_t nfiles, अक्षर *स्थिरांक *file)
{
  संरचना fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, आकार *fstatus);

  जर (nfiles == 0
      || (nfiles == 1
          && ((print_ओळी + print_शब्द + print_chars
               + print_bytes + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  अन्यथा
    {
      साठी (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? ख_स्थिति (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  परत fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_PURE
स्थिर पूर्णांक
compute_संख्या_width (idx_t nfiles, संरचना fstatus स्थिरांक *fstatus)
{
  पूर्णांक width = 1;

  जर (0 < nfiles && fstatus[0].failed <= 0)
    {
      पूर्णांक minimum_width = 1;
      uintmax_t regular_total = 0;

      साठी (idx_t i = 0; i < nfiles; i++)
        जर (! fstatus[i].failed)
          {
            जर (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            अन्यथा जर (ckd_add (&regular_total, regular_total,
                              fstatus[i].st.st_size))
              {
                regular_total = UINTMAX_MAX;
                थांबा;
              }
          }

      साठी (; 10 <= regular_total; regular_total /= 10)
        width++;
      जर (width < minimum_width)
        width = minimum_width;
    }

  परत width;
}


पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;
  idx_t nfiles;
  अक्षर **files;
  अक्षर *files_from = रिक्त;
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

  पर्यंत ((optc = getopt_long (argc, argv, "clLmw", longopts, रिक्त)) != -1)
    निवड (optc)
      {
      स्थिती 'c':
        print_bytes = true;
        थांबा;

      स्थिती 'm':
        print_chars = true;
        थांबा;

      स्थिती 'l':
        print_ओळी = true;
        थांबा;

      स्थिती 'w':
        print_शब्द = true;
        थांबा;

      स्थिती 'L':
        print_linelength = true;
        थांबा;

      स्थिती DEBUG_PROGRAM_OPTION:
        debug = true;
        थांबा;

      स्थिती FILES0_FROM_OPTION:
        files_from = optarg;
        थांबा;

      स्थिती TOTAL_OPTION:
        total_mode = XARGMATCH ("--total", optarg, total_args, total_types);
        थांबा;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      पूर्वनिर्धारित:
        usage (निकास_त्रुटि);
      }

  जर (! (print_ओळी || print_शब्द || print_chars || print_bytes
         || print_linelength))
    print_ओळी = print_शब्द = print_bytes = true;

  जर (print_linelength)
    साठी (पूर्णांक i = 0; i <= अच_अक्षर_उच्च; i++)
      wc_isprint[i] = !!है_छाप (i);
  जर (print_शब्द)
    साठी (पूर्णांक i = 0; i <= अच_अक्षर_उच्च; i++)
      wc_isspace[i] = है_खाली (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  संरचना argv_iterator *ai;
  जर (files_from)
    {
      खाता *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      जर (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          ख_लिखो (मानक_त्रुटि, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (निकास_त्रुटि);
        }

      जर (streq (files_from, "-"))
        stream = मानक_निवेश;
      अन्यथा
        {
          stream = ख_खोलो (files_from, "r");
          जर (stream == रिक्त)
            error (निकास_त्रुटि, त्रुटि_सं, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      संरचना stat st;
      जर (ख_स्थिति (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_size <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          जर (! readtokens0 (stream, &tok) || ख_बंद (stream) != 0)
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
      स्थिर अक्षर *stdin_only[] = { रिक्त };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  जर (!ai)
    xalloc_die ();

  fstatus = get_आदान_fstatus (nfiles, files);
  जर (total_mode == total_only)
    संख्या_width = 1;  /* No extra padding, since no alignment requirement.  */
  अन्यथा
    संख्या_width = compute_संख्या_width (nfiles, fstatus);

  bool ok = true;
  गणना argv_iter_err ai_err;
  अक्षर *file_name;
  साठी (पूर्णांक i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      जर (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      जर (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          जर (files_from == रिक्त)
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

      जर (skip_file)
        ok = false;
      अन्यथा
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      जर (! nfiles)
        fstatus[0].failed = 1;
    }
  निवड (ai_err)
    {
    स्थिती AI_ERR_EOF:
      थांबा;

    स्थिती AI_ERR_READ:
      error (0, त्रुटि_सं, _("%s: read error"), quotef (files_from));
      ok = false;
      थांबा;

    स्थिती AI_ERR_MEM:
      xalloc_die ();

    स्थिती AI_ERR_OK: पूर्वनिर्धारित:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  जर (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (रिक्त, &fstatus[0]);

  जर (read_tokens)
    readtokens0_free (&tok);

  जर (total_mode != total_never
      && (total_mode != total_auto || 1 < argv_iter_n_args (ai)))
    {
      जर (total_ओळी_overflow)
        {
          total_ओळी = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      जर (total_शब्द_overflow)
        {
          total_शब्द = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      जर (total_chars_overflow)
        {
          total_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      जर (total_bytes_overflow)
        {
          total_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (total_ओळी, total_शब्द, total_chars, total_bytes,
                    max_ओळ_length,
                    total_mode != total_only ? _("total") : रिक्त);
    }

  argv_iter_free (ai);

  मुक्त (fstatus);

  जर (have_read_stdin && बंद (STDIN_FILENO) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "-");

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
