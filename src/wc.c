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

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश <argmatch.h>
#समावेश <argv-iter.h>
#समावेश <fadvise.h>
#समावेश <physmem.h>
#समावेश <readtokens0.h>
#समावेश <stat-size.h>
#समावेश <xbinary-io.h>

#अगर_घोषित USE_NEON_WC_LINECOUNT
# include <sys/auxv.h>
# include <asm/hwcap.h>
#पूर्ण_अगर

#समावेश "system.h"
#समावेश "assure.h"
#समावेश "cpu-supports.h"
#समावेश "ioblksize.h"
#समावेश "wc.h"
#समावेश "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "wc"

#घोषणा AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie")

अटल bool wc_isprint[अच_अक्षर_उच्च + 1];
अटल bool wc_isspace[अच_अक्षर_उच्च + 1];

अटल bool debug;

/* Cumulative number of lines, words, chars and bytes in all files so far.
   max_line_length is the maximum over all files processed so far.  */
अटल uintmax_t योग_पंक्तियाँ;
अटल uintmax_t योग_शब्दगण;
अटल uintmax_t योग_chars;
अटल uintmax_t योग_bytes;
अटल bool योग_पंक्तियाँ_overflow;
अटल bool योग_शब्दगण_overflow;
अटल bool योग_chars_overflow;
अटल bool योग_bytes_overflow;
अटल intmax_t max_पंक्ति_लंबाई;

/* Which counts to print. */
अटल bool print_पंक्तियाँ, print_शब्दगण, print_chars, print_bytes;
अटल bool print_linelength;

/* The print width of each count.  */
अटल पूर्णांक संख्या_width;

/* True if we have ever read the standard input. */
अटल bool have_read_stdin;

/* Used to determine if file size can be determined without reading.  */
अटल idx_t page_आकार;

/* Enable to _not_ treat non breaking space as a word separator.  */
अटल bool posixly_correct;

/* The result of calling fstat or stat on a file descriptor or file.  */
काष्ठा fstatus
{
  /* If positive, fstat or stat has not been called yet.  Otherwise,
     this is the value returned from fstat or stat.  */
  पूर्णांक failed;

  /* If FAILED is zero, this is the file's status.  */
  काष्ठा stat st;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  DEBUG_PROGRAM_OPTION = अक्षर_उच्च + 1,
  FILES0_FROM_OPTION,
  योग_OPTION,
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"bytes", no_argument, शून्य, 'c'},
  {"chars", no_argument, शून्य, 'm'},
  {"lines", no_argument, शून्य, 'l'},
  {"words", no_argument, शून्य, 'w'},
  {"debug", no_argument, शून्य, DEBUG_PROGRAM_OPTION},
  {"files0-from", required_argument, शून्य, FILES0_FROM_OPTION},
  {"max-line-length", no_argument, शून्य, 'L'},
  {"total", required_argument, शून्य, योग_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

क्रमागत योग_type
  {
    योग_auto,         /* 0: default or --total=auto */
    योग_always,       /* 1: --total=always */
    योग_only,         /* 2: --total=only */
    योग_never         /* 3: --total=never */
  };
अटल अक्षर स्थिर *स्थिर योग_args[] =
{
  "auto", "always", "only", "never", शून्य
};
अटल क्रमागत योग_type स्थिर योग_types[] =
{
  योग_auto, योग_always, योग_only, योग_never
};
ARGMATCH_VERIFY (योग_args, योग_types);
अटल क्रमागत योग_type योग_mode = योग_auto;

#अगर_घोषित USE_AVX2_WC_LINECOUNT
अटल bool
avx2_supported (व्योम)
{
  bool avx2_enabled = cpu_supports ("avx2");
  अगर (debug)
    error (0, 0, (avx2_enabled
                  ? _("using avx2 hardware support")
                  : _("avx2 support not detected")));

  वापस avx2_enabled;
}
#पूर्ण_अगर

#अगर_घोषित USE_AVX512_WC_LINECOUNT
अटल bool
avx512_supported (व्योम)
{
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw"));

  अगर (debug)
    error (0, 0, (avx512_enabled
                  ? _("using avx512 hardware support")
                  : _("avx512 support not detected")));

  वापस avx512_enabled;
}
#पूर्ण_अगर

#अगर_घोषित USE_NEON_WC_LINECOUNT
अटल bool
neon_supported (व्योम)
{
  bool neon_enabled = (cpu_may_support ("asimd")
                       && 0 < (getauxval (AT_HWCAP) & HWCAP_ASIMD));
  अगर (debug)
    error (0, 0, (neon_enabled
                  ? _("using neon hardware support")
                  : _("neon support not detected")));

  वापस neon_enabled;
}
#पूर्ण_अगर

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      ख_माला_दो (_("\
Print newline, word, and byte counts क्रम each खाता, and a योग_2 पंक्ति अगर\n\
more than one खाता is specified.  A word is a nonempty sequence of non white\n\
space delimited by white space characters or by start or end of निवेश.\n\
"), मानक_निकास);

      emit_stdin_note ();

      ख_माला_दो (_("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum पंक्ति लंबाई.\n\
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
  -l, --पंक्तियाँ\n\
         print the newline counts\n\
"));
      oputs (_("\
      --debug\n\
         indicate what पंक्ति गिनती acceleration is used\n\
"));
      oputs (_("\
      --files0-from=F\n\
         पढ़ो निवेश from the files specified by\n\
         NUL-terminated names in file F;\n\
         If F is -, पढ़ो names from standard निवेश\n\
"));
      oputs (_("\
  -L, --max-पंक्ति-लंबाई\n\
         print the maximum display width\n\
"));
      oputs (_("\
  -w, --शब्दगण\n\
         print the word counts\n\
"));
      oputs (_("\
      --योग_2=WHEN\n\
         when to print a पंक्ति with योग_2 counts;\n\
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
अटल पूर्णांक
maybe_c32isnbspace (char32_t wc)
{
  वापस ! posixly_correct && c32isnbspace (wc);
}

/* FILE is the name of the file (or null for standard input)
   associated with the specified counters.  */
अटल व्योम
write_counts (uintmax_t पंक्तियाँ,
              uintmax_t शब्दगण,
              uintmax_t chars,
              uintmax_t bytes,
              intmax_t linelength,
              अक्षर स्थिर *file)
{
  अटल अक्षर स्थिर format_sp_int[] = " %*s";
  अक्षर स्थिर *format_int = format_sp_int + 1;
  अक्षर buf[MAX (INT_BUFSIZE_BOUND (intmax_t),
                INT_BUFSIZE_BOUND (uintmax_t))];

  अगर (print_पंक्तियाँ)
    {
      म_लिखो (format_int, संख्या_width, umaxtostr (पंक्तियाँ, buf));
      format_int = format_sp_int;
    }
  अगर (print_शब्दगण)
    {
      म_लिखो (format_int, संख्या_width, umaxtostr (शब्दगण, buf));
      format_int = format_sp_int;
    }
  अगर (print_chars)
    {
      म_लिखो (format_int, संख्या_width, umaxtostr (chars, buf));
      format_int = format_sp_int;
    }
  अगर (print_bytes)
    {
      म_लिखो (format_int, संख्या_width, umaxtostr (bytes, buf));
      format_int = format_sp_int;
    }
  अगर (print_linelength)
    म_लिखो (format_int, संख्या_width, imaxtostr (linelength, buf));
  अगर (file)
    म_लिखो (" %s", म_अक्षर (file, '\n') ? quotef (file) : file);
  अक्षर_दो ('\n');

  अगर (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Read FD and return a summary.  */
अटल काष्ठा wc_पंक्तियाँ
wc_पंक्तियाँ (पूर्णांक fd)
{
#अगर_घोषित USE_AVX512_WC_LINECOUNT
  अटल चिन्हित अक्षर use_avx512;
  अगर (!use_avx512)
    use_avx512 = avx512_supported () ? 1 : -1;
  अगर (0 < use_avx512)
    वापस wc_पंक्तियाँ_avx512 (fd);
#पूर्ण_अगर
#अगर_घोषित USE_AVX2_WC_LINECOUNT
  अटल चिन्हित अक्षर use_avx2;
  अगर (!use_avx2)
    use_avx2 = avx2_supported () ? 1 : -1;
  अगर (0 < use_avx2)
    वापस wc_पंक्तियाँ_avx2 (fd);
#पूर्ण_अगर
#अगर_घोषित USE_NEON_WC_LINECOUNT
  अटल चिन्हित अक्षर use_neon;
  अगर (!use_neon)
    use_neon = neon_supported () ? 1 : -1;
  अगर (0 < use_neon)
    वापस wc_पंक्तियाँ_neon (fd);
#पूर्ण_अगर

  intmax_t पंक्तियाँ = 0, bytes = 0;
  bool long_पंक्तियाँ = false;

  जबतक (true)
    {
      अक्षर buf[IO_BUFSIZE + 1];
      ssize_t bytes_read = पढ़ो (fd, buf, IO_BUFSIZE);
      अगर (bytes_read <= 0)
        वापस (काष्ठा wc_पंक्तियाँ) { bytes_read == 0 ? 0 : त्रुटि_सं, पंक्तियाँ, bytes };

      bytes += bytes_read;
      अक्षर *end = buf + bytes_read;
      idx_t buflines = 0;

      अगर (! long_पंक्तियाँ)
        {
          /* Avoid function call overhead for shorter lines.  */
          क्रम (अक्षर *p = buf; p < end; p++)
            buflines += *p == '\n';
        }
      अन्यथा
        {
          /* rawmemchr is more efficient with longer lines.  */
          *end = '\n';
          क्रम (अक्षर *p = buf; (p = rawmemchr (p, '\n')) < end; p++)
            buflines++;
        }

      /* If the average line length in the block is >= 15, then use
          memchr for the next block, where system specific optimizations
          may outweigh function call overhead.
          FIXME: This line length was determined in 2015, on both
          x86_64 and ppc64, but it's worth re-evaluating in future with
          newer compilers, CPUs, or memchr() implementations etc.  */
      long_पंक्तियाँ = 15 * buflines <= bytes_read;
      पंक्तियाँ += buflines;
    }
}

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   Return true if successful.  */
अटल bool
wc (पूर्णांक fd, अक्षर स्थिर *file_x, काष्ठा fstatus *fstatus)
{
  पूर्णांक err = 0;
  अक्षर buf[IO_BUFSIZE + 1];
  intmax_t पंक्तियाँ, शब्दगण, chars, bytes, linelength;
  bool गिनती_bytes, गिनती_chars, गिनती_complicated;
  अक्षर स्थिर *file = file_x ? file_x : _("standard input");

  पंक्तियाँ = शब्दगण = chars = bytes = linelength = 0;

  /* If in the current locale, chars are equivalent to bytes, we prefer
     counting bytes, because that's easier.  */
  अगर (MB_CUR_MAX > 1)
    {
      गिनती_bytes = print_bytes;
      गिनती_chars = print_chars;
    }
  अन्यथा
    {
      गिनती_bytes = print_bytes || print_chars;
      गिनती_chars = false;
    }
  गिनती_complicated = print_शब्दगण || print_linelength;

  /* Advise the kernel of our access pattern only if we will read().  */
  अगर (!गिनती_bytes || गिनती_chars || print_पंक्तियाँ || गिनती_complicated)
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

  अगर (गिनती_bytes && !गिनती_chars && !print_पंक्तियाँ && !गिनती_complicated)
    {
      bool skip_read = false;

      अगर (0 < fstatus->failed)
        fstatus->failed = ख_स्थिति (fd, &fstatus->st);

      /* For sized files, seek to one st_blksize before EOF rather than to EOF.
         This works better for files in proc-like file systems where
         the size is only approximate.  */
      अगर (! fstatus->failed && usable_st_आकार (&fstatus->st)
          && 0 <= fstatus->st.st_आकार)
        {
          off_t end_pos = fstatus->st.st_आकार;
          off_t current_pos = lseek (fd, 0, प्रस्तुत_से);
          अगर (current_pos < 0)
            ;
          अन्यथा अगर (end_pos % page_आकार)
            {
              /* We need special handling of /proc and /sys files etc. only
                 when they're a multiple of PAGE_SIZE.  In the common case
                 for files with st_size not a multiple of PAGE_SIZE,
                 it's more efficient and accurate to use st_size.

                 Be careful here.  The current position may actually be
                 beyond the end of the file.  As in the example above.  */

              अगर (current_pos < end_pos)
                {
                  bytes = end_pos - current_pos;
                  skip_read = true;
                }
            }
          अन्यथा अगर (0 < end_pos)
            {
              off_t ep_1 = end_pos - 1;
              off_t hi_pos = ep_1 - ep_1 % STP_BLKSIZE (&fstatus->st);
              अगर (current_pos < hi_pos)
                bytes = hi_pos - current_pos;
            }

          अगर (0 < bytes && lseek (fd, bytes, प्रस्तुत_से) < 0)
            {
              bytes = 0;
              skip_read = false;
            }
        }

      अगर (! skip_read)
        {
          fdadvise (fd, 0, 0, FADVISE_SEQUENTIAL);
          क्रम (ssize_t bytes_read;
               (bytes_read = पढ़ो (fd, buf, IO_BUFSIZE));
               bytes += bytes_read)
            अगर (bytes_read < 0)
              {
                err = त्रुटि_सं;
                अवरोध;
              }
        }
    }
  अन्यथा अगर (!गिनती_chars && !गिनती_complicated)
    {
      /* Use a separate loop when counting only lines or lines and bytes --
         but not chars or words.  */
      काष्ठा wc_पंक्तियाँ w = wc_पंक्तियाँ (fd);
      err = w.err;
      पंक्तियाँ = w.पंक्तियाँ;
      bytes = w.bytes;
    }
  अन्यथा अगर (MB_CUR_MAX > 1)
    {
      bool in_word = false;
      intmax_t linepos = 0;
      mbstate_t state; mbszero (&state);
      bool in_shift = false;
      bool गिनती_utf8 = !गिनती_complicated && !print_पंक्तियाँ
                        && is_utf8_charset ();
      idx_t पूर्व = 0; /* Number of bytes carried over from previous round.  */

      क्रम (ssize_t bytes_read;
           ((bytes_read = पढ़ो (fd, buf + पूर्व, IO_BUFSIZE - पूर्व))
            || पूर्व);
           )
        {
          अगर (bytes_read < 0)
            {
              err = त्रुटि_सं;
              अवरोध;
            }

          bytes += bytes_read;
          अक्षर स्थिर *p = buf;
          अक्षर स्थिर *plim = p + पूर्व + bytes_read;
          bool try_गिनती_utf8 = गिनती_utf8;
          करो
            {
              अगर (try_गिनती_utf8 && !in_shift)
                {
                  /* Scan at most once per buffer, to avoid repeatedly
                     scanning ASCII after decoding errors.  */
                  try_गिनती_utf8 = false;
                  idx_t n = plim - p;
                  chars += u8_गिनती (p, &n);
                  p += n;
                  अगर (p == plim)
                    अवरोध;
                }

              char32_t wide_char;
              idx_t charbytes;
              bool single_byte;

              अगर (!in_shift && 0 <= *p && *p < 0x80)
                {
                  /* Handle most ASCII characters quickly, without calling
                     mbrtoc32.  */
                  charbytes = 1;
                  wide_char = *p;
                  single_byte = true;
                }
              अन्यथा
                {
                  idx_t पूर्व_bytes = पूर्व;
                  idx_t scanbytes = plim - (p + पूर्व);
                  माप_प्रकार n = mbrtoc32 (&wide_char, p + पूर्व, scanbytes, &state);
                  पूर्व = 0;

                  अगर (scanbytes < n)
                    {
                      अगर (n == (माप_प्रकार) -2 && plim - p < IO_BUFSIZE
                          && bytes_read)
                        {
                          /* An incomplete character that is not ridiculously
                             long and there may be more input.  Move the bytes
                             to buffer start and prepare to read more data.  */
                          पूर्व = plim - p;
                          स_हटाओ (buf, p, पूर्व);
                          in_shift = true;
                          अवरोध;
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
                      शब्दगण += !in_word;
                      in_word = true;
                      जारी;
                    }

                  /* Include bytes already consumed into STATE in an earlier
                     read, but still present at P in the buffer.  */
                  charbytes = पूर्व_bytes + n + !n;
                  single_byte = charbytes == !in_shift;
                  in_shift = !mbsinit (&state);
                }

              चयन (wide_char)
                {
                हाल '\n':
                  पंक्तियाँ++;
                  FALLTHROUGH;
                हाल '\r':
                हाल '\f':
                  अगर (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  अवरोध;

                हाल '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  अवरोध;

                हाल ' ':
                  linepos++;
                  FALLTHROUGH;
                हाल '\v':
                  in_word = false;
                  अवरोध;

                शेष:;
                  bool in_word2;
                  अगर (single_byte && wide_char <= अच_अक्षर_उच्च)
                    {
                      linepos += wc_isprint[wide_char];
                      in_word2 = !wc_isspace[wide_char];
                    }
                  अन्यथा
                    {
                      /* c32width can be expensive on macOS for example,
                         so avoid if not needed.  */
                      अगर (print_linelength)
                        {
                          पूर्णांक width = c32width (wide_char);
                          अगर (width > 0)
                            linepos += width;
                        }
                      in_word2 = (! c32isspace (wide_char)
                                  && ! maybe_c32isnbspace (wide_char));
                    }

                  /* Count words by counting word starts, i.e., each
                     white space character (or the start of input)
                     followed by non white space.  */
                  शब्दगण += !in_word & in_word2;
                  in_word = in_word2;
                  अवरोध;
                }

              p += charbytes;
              chars++;
            }
          जबतक (p < plim);
        }
      अगर (linepos > linelength)
        linelength = linepos;
    }
  अन्यथा
    {
      bool in_word = false;
      intmax_t linepos = 0;

      क्रम (ssize_t bytes_read; (bytes_read = पढ़ो (fd, buf, IO_BUFSIZE)); )
        {
          अगर (bytes_read < 0)
            {
              err = त्रुटि_सं;
              अवरोध;
            }

          bytes += bytes_read;
          अक्षर स्थिर *p = buf;
          करो
            {
              अचिन्हित अक्षर c = *p++;
              चयन (c)
                {
                हाल '\n':
                  पंक्तियाँ++;
                  FALLTHROUGH;
                हाल '\r':
                हाल '\f':
                  अगर (linepos > linelength)
                    linelength = linepos;
                  linepos = 0;
                  in_word = false;
                  अवरोध;

                हाल '\t':
                  linepos += 8 - (linepos % 8);
                  in_word = false;
                  अवरोध;

                हाल ' ':
                  linepos++;
                  FALLTHROUGH;
                हाल '\v':
                  in_word = false;
                  अवरोध;

                शेष:
                  linepos += wc_isprint[c];
                  bool in_word2 = !wc_isspace[c];
                  शब्दगण += !in_word & in_word2;
                  in_word = in_word2;
                  अवरोध;
                }
            }
          जबतक (--bytes_read);
        }
      अगर (linepos > linelength)
        linelength = linepos;
    }

  अगर (गिनती_chars < print_chars)
    chars = bytes;

  अगर (योग_mode != योग_only)
    write_counts (पंक्तियाँ, शब्दगण, chars, bytes, linelength, file_x);

  योग_पंक्तियाँ_overflow |= ckd_add (&योग_पंक्तियाँ, योग_पंक्तियाँ, पंक्तियाँ);
  योग_शब्दगण_overflow |= ckd_add (&योग_शब्दगण, योग_शब्दगण, शब्दगण);
  योग_chars_overflow |= ckd_add (&योग_chars, योग_chars, chars);
  योग_bytes_overflow |= ckd_add (&योग_bytes, योग_bytes, bytes);

  अगर (linelength > max_पंक्ति_लंबाई)
    max_पंक्ति_लंबाई = linelength;

  अगर (err)
    error (0, err, "%s", quotef (file));
  वापस !err;
}

अटल bool
wc_file (अक्षर स्थिर *file, काष्ठा fstatus *fstatus)
{
  अगर (! file || streq (file, "-"))
    {
      have_read_stdin = true;
      xset_द्विआधारी_mode (STDIN_FILENO, O_द्विआधारी);
      वापस wc (STDIN_FILENO, file, fstatus);
    }
  अन्यथा
    {
      पूर्णांक fd = खोलो (file, O_RDONLY | O_द्विआधारी);
      अगर (fd == -1)
        {
          error (0, त्रुटि_सं, "%s", quotef (file));
          वापस false;
        }
      अन्यथा
        {
          bool ok = wc (fd, file, fstatus);
          अगर (बंद (fd) != 0)
            {
              error (0, त्रुटि_सं, "%s", quotef (file));
              वापस false;
            }
          वापस ok;
        }
    }
}

/* Return the file status for the NFILES files addressed by FILE.
   Optimize the case where only one number is printed, for just one
   file; in that case we can use a print width of 1, so we don't need
   to stat the file.  Handle the case of (nfiles == 0) in the same way;
   that happens when we don't know how long the list of file names will be.  */

अटल काष्ठा fstatus *
get_निवेश_fstatus (idx_t nfiles, अक्षर *स्थिर *file)
{
  काष्ठा fstatus *fstatus = xnmalloc (nfiles ? nfiles : 1, माप *fstatus);

  अगर (nfiles == 0
      || (nfiles == 1
          && ((print_पंक्तियाँ + print_शब्दगण + print_chars
               + print_bytes + print_linelength)
              == 1)))
    fstatus[0].failed = 1;
  अन्यथा
    {
      क्रम (idx_t i = 0; i < nfiles; i++)
        fstatus[i].failed = (! file[i] || streq (file[i], "-")
                             ? ख_स्थिति (STDIN_FILENO, &fstatus[i].st)
                             : stat (file[i], &fstatus[i].st));
    }

  वापस fstatus;
}

/* Return a print width suitable for the NFILES files whose status is
   recorded in FSTATUS.  Optimize the same special case that
   get_input_fstatus optimizes.  */

ATTRIBUTE_PURE
अटल पूर्णांक
compute_संख्या_width (idx_t nfiles, काष्ठा fstatus स्थिर *fstatus)
{
  पूर्णांक width = 1;

  अगर (0 < nfiles && fstatus[0].failed <= 0)
    {
      पूर्णांक minimum_width = 1;
      uintmax_t regular_योग = 0;

      क्रम (idx_t i = 0; i < nfiles; i++)
        अगर (! fstatus[i].failed)
          {
            अगर (!S_ISREG (fstatus[i].st.st_mode))
              minimum_width = 7;
            अन्यथा अगर (ckd_add (&regular_योग, regular_योग,
                              fstatus[i].st.st_आकार))
              {
                regular_योग = UINTMAX_MAX;
                अवरोध;
              }
          }

      क्रम (; 10 <= regular_योग; regular_योग /= 10)
        width++;
      अगर (width < minimum_width)
        width = minimum_width;
    }

  वापस width;
}


पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;
  idx_t nfiles;
  अक्षर **files;
  अक्षर *files_from = शून्य;
  काष्ठा fstatus *fstatus;
  काष्ठा Tokens tok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  page_आकार = getpagesize ();
  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  रखो_भबफ (मानक_निकास, शून्य, _IOLBF, 0);

  posixly_correct = (दो_पर्या ("POSIXLY_CORRECT") != शून्य);

  जबतक ((optc = getopt_long (argc, argv, "clLmw", longopts, शून्य)) != -1)
    चयन (optc)
      {
      हाल 'c':
        print_bytes = true;
        अवरोध;

      हाल 'm':
        print_chars = true;
        अवरोध;

      हाल 'l':
        print_पंक्तियाँ = true;
        अवरोध;

      हाल 'w':
        print_शब्दगण = true;
        अवरोध;

      हाल 'L':
        print_linelength = true;
        अवरोध;

      हाल DEBUG_PROGRAM_OPTION:
        debug = true;
        अवरोध;

      हाल FILES0_FROM_OPTION:
        files_from = optarg;
        अवरोध;

      हाल योग_OPTION:
        योग_mode = XARGMATCH ("--total", optarg, योग_args, योग_types);
        अवरोध;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      शेष:
        usage (निकास_त्रुटि);
      }

  अगर (! (print_पंक्तियाँ || print_शब्दगण || print_chars || print_bytes
         || print_linelength))
    print_पंक्तियाँ = print_शब्दगण = print_bytes = true;

  अगर (print_linelength)
    क्रम (पूर्णांक i = 0; i <= अच_अक्षर_उच्च; i++)
      wc_isprint[i] = !!है_छाप (i);
  अगर (print_शब्दगण)
    क्रम (पूर्णांक i = 0; i <= अच_अक्षर_उच्च; i++)
      wc_isspace[i] = है_खाली (i) || maybe_c32isnbspace (btoc32 (i));

  bool read_tokens = false;
  काष्ठा argv_iterator *ai;
  अगर (files_from)
    {
      खाता *stream;

      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      अगर (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quoteaf (argv[optind]));
          ख_लिखो (मानक_त्रुटि, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (निकास_त्रुटि);
        }

      अगर (streq (files_from, "-"))
        stream = मानक_निवेश;
      अन्यथा
        {
          stream = ख_खोलो (files_from, "r");
          अगर (stream == शून्य)
            error (निकास_त्रुटि, त्रुटि_सं, _("cannot open %s for reading"),
                   quoteaf (files_from));
        }

      /* Read the file list into RAM if we can detect its size and that
         size is reasonable.  Otherwise, we'll read a name at a time.  */
      काष्ठा stat st;
      अगर (ख_स्थिति (fileno (stream), &st) == 0
          && S_ISREG (st.st_mode)
          && st.st_आकार <= MIN (10 * 1024 * 1024, physmem_available () / 2))
        {
          read_tokens = true;
          readtokens0_init (&tok);
          अगर (! readtokens0 (stream, &tok) || ख_बंद (stream) != 0)
            error (निकास_त्रुटि, 0, _("cannot read file names from %s"),
                   quoteaf (files_from));
          files = tok.tok;
          nfiles = tok.n_tok;
          ai = argv_iter_init_argv (files);
        }
      अन्यथा
        {
          files = शून्य;
          nfiles = 0;
          ai = argv_iter_init_stream (stream);
        }
    }
  अन्यथा
    {
      अटल अक्षर *stdin_only[] = { शून्य };
      files = (optind < argc ? argv + optind : stdin_only);
      nfiles = (optind < argc ? argc - optind : 1);
      ai = argv_iter_init_argv (files);
    }

  अगर (!ai)
    xalloc_die ();

  fstatus = get_निवेश_fstatus (nfiles, files);
  अगर (योग_mode == योग_only)
    संख्या_width = 1;  /* No extra padding, since no alignment requirement.  */
  अन्यथा
    संख्या_width = compute_संख्या_width (nfiles, fstatus);

  bool ok = true;
  क्रमागत argv_iter_err ai_err;
  अक्षर *file_name;
  क्रम (पूर्णांक i = 0; (file_name = argv_iter (ai, &ai_err)); i++)
    {
      bool skip_file = false;
      अगर (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | wc --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      अगर (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          अगर (files_from == शून्य)
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

      अगर (skip_file)
        ok = false;
      अन्यथा
        ok &= wc_file (file_name, &fstatus[nfiles ? i : 0]);

      अगर (! nfiles)
        fstatus[0].failed = 1;
    }
  चयन (ai_err)
    {
    हाल AI_ERR_EOF:
      अवरोध;

    हाल AI_ERR_READ:
      error (0, त्रुटि_सं, _("%s: read error"), quotef (files_from));
      ok = false;
      अवरोध;

    हाल AI_ERR_MEM:
      xalloc_die ();

    हाल AI_ERR_OK: शेष:
      affirm (false);
    }

  /* No arguments on the command line is fine.  That means read from stdin.
     However, no arguments on the --files0-from input stream is an error
     means don't read anything.  */
  अगर (ok && !files_from && argv_iter_n_args (ai) == 0)
    ok &= wc_file (शून्य, &fstatus[0]);

  अगर (read_tokens)
    readtokens0_free (&tok);

  अगर (योग_mode != योग_never
      && (योग_mode != योग_auto || 1 < argv_iter_n_args (ai)))
    {
      अगर (योग_पंक्तियाँ_overflow)
        {
          योग_पंक्तियाँ = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total lines"));
          ok = false;
        }
      अगर (योग_शब्दगण_overflow)
        {
          योग_शब्दगण = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total words"));
          ok = false;
        }
      अगर (योग_chars_overflow)
        {
          योग_chars = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total characters"));
          ok = false;
        }
      अगर (योग_bytes_overflow)
        {
          योग_bytes = UINTMAX_MAX;
          error (0, EOVERFLOW, _("total bytes"));
          ok = false;
        }

      write_counts (योग_पंक्तियाँ, योग_शब्दगण, योग_chars, योग_bytes,
                    max_पंक्ति_लंबाई,
                    योग_mode != योग_only ? _("total") : शून्य);
    }

  argv_iter_free (ai);

  मुक्त (fstatus);

  अगर (have_read_stdin && बंद (STDIN_FILENO) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "-");

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
