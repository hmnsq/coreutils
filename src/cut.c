/* cut - remove parts of lines of files
   Copyright (C) 1997-2026 Free Software Foundation, Inc.
   Copyright (C) 1984 David M. Ihnat

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

/* Written by David Ihnat.  */

/* POSIX changes, bug fixes, long-named options, and cleanup
   by David MacKenzie <djm@gnu.ai.mit.edu>.

   Rewrite cut_fields and cut_bytes -- Jim Meyering.  */

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن "system.h"

#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "fadvise.h"
#ضمن "ioblksize.h"
#ضمن "mbbuf.h"
#ضمن "memchr2.h"

#ضمن "set-fields.h"
#ضمن "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "cut"

#وصف_ڪريو AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#وصف_ڪريو FATAL_ERROR(پيغام)						\
  افعل									\
    {									\
      error (0, 0, (پيغام));						\
      usage (EXIT_FAILURE);						\
    }									\
  طالما (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#وصف_ڪريو SMALL_BYTE_حد 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
ساكن بنية ميدان_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
ساكن حرف *ميدان_1_بفر;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
ساكن idx_t ميدان_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
ساكن bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
ساكن bool complement;

/* The delimiter character for multibyte field mode.  */
ساكن mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
ساكن حرف delim_بائيٽون[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
ساكن بنانشان حرف سٽ_delim = '\n';

/* The length of output_delimiter_string.  */
ساكن size_t output_delimiter_ڊيگهائي;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
ساكن حرف *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
ساكن حرف output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
ساكن bool no_ورهايو;

/* If true, interpret each run of whitespace as one field delimiter.  */
ساكن bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
ساكن bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
ساكن bool space_output_delimiter_default;

تعداد whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

ساكن حرف ثابت *ثابت whitespace_option_دليلون[] =
{
  "trimmed", عدم
};

ساكن تعداد whitespace_option ثابت whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_دليلون, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
ساكن تعداد
{
  CUT_گهڻائي_NONE,
  CUT_گهڻائي_بائيٽون,
  CUT_گهڻائي_CHARACTERS,
  CUT_گهڻائي_FIELDS
} cut_گهڻائي = CUT_گهڻائي_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"bytes", required_دليل, عدم, 'b'},
  {"characters", required_دليل, عدم, 'c'},
  {"fields", required_دليل, عدم, 'f'},
  {"delimiter", required_دليل, عدم, 'd'},
  {"no-partial", no_دليل, عدم, 'n'},
  {"whitespace-delimited", optional_دليل, عدم, 'w'},
  {"only-delimited", no_دليل, عدم, 's'},
  {"output-delimiter", required_دليل, عدم, 'O'},
  {"complement", no_دليل, عدم, COMPLEMENT_OPTION},
  {"zero-terminated", no_دليل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Print selected parts جو سٽون from each FILE منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --بائيٽون=LIST\n\
         select only these byte positions\n\
"));
      oputs (_("\
  -c, --characters=LIST\n\
         select only these character positions\n\
"));
      oputs (_("\
      --complement\n\
         complement the سيٽ_2 جو selected بائيٽون, characters or fields\n\
"));
      oputs (_("\
  -d, --delimiter=DELIM\n\
         use DELIM instead جو TAB لكل ميدان delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any سٽ that شامل_آهي\n\
         no delimiter character, unless the -s option is specified\n\
"));
      oputs (_("\
  -F LIST\n\
         like -f, but also نتيجو_ڏئي -w and -O ' '\n\
"));
      oputs (_("\
  -n, --no-partial\n\
         with -b, don't output partial multi-byte characters\n\
"));
      oputs (_("\
  -O, --output-delimiter=STRING\n\
         use STRING as the output delimiter;\n\
         the افتراضي is منزل use the داخلا delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         افعل not print سٽون not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a لاڳيتو_2 جو blank characters as the ميدان delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سٽ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one جو -b, -c, -f or -F.  Each LIST is made مٿي جو one\n\
range, or many ranges separated by commas.  Selected داخلا is written\n\
اندريون the same ترتيب that it is اقرأ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one جو:\n\
\n\
  N     N'th byte, character or ميدان, counted from 1\n\
  N-    from N'th byte, character or ميدان, منزل پڇاڙي جو سٽ\n\
  N-M   from N'th to M'th (included) byte, character or ميدان\n\
  -M    from پهريون منزل M'th (included) byte, character or ميدان\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

ساكن مضمن فراغ
ايندڙ_شي (uintmax_t *شي_idx)
{
  (*شي_idx)++;
  إذا ((*شي_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

ساكن مضمن bool
print_kth (uintmax_t k)
{
  أرجع current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

ساكن مضمن bool
is_range_شروعات_اشاريو (uintmax_t k)
{
  أرجع k == current_rp->lo;
}

ساكن مضمن bool
ميدان_delim_is_سٽ_delim (فراغ)
{
  أرجع delim_mcel.len == 1 && delim_بائيٽون[0] == سٽ_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

ساكن bool
mcel_isblank (mcel_t g)
{
  أرجع (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

ساكن مضمن bool
bytesearch_ميدان_delim_ok (فراغ)
{
  بنانشان حرف delim_0 = delim_بائيٽون[0];

  أرجع (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


تعداد ميدان_terminator
{
  ميدان_معلومات,
  ميدان_DELIMITER,
  ميدان_سٽ_DELIMITER,
  ميدان_EOF
};

تعداد bytesearch_گهڻائي
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_سٽ_ONLY
};

بنية bytesearch_context
{
  تعداد bytesearch_گهڻائي گهڻائي_2;
  bool blank_delimited;
  bool at_eof;
  حرف *سٽ_پڇاڙي;
  bool سٽ_پڇاڙي_معلوم;
};

ساكن مضمن فراغ
bytesearch_context_reset (بنية bytesearch_context *ctx)
{
  ctx->گهڻائي_2 = BYTESEARCH_FIELDS;
  ctx->سٽ_پڇاڙي = عدم;
  ctx->سٽ_پڇاڙي_معلوم = false;
}

بنية mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_سٽ_شروعات;
  bool have_saved;
  mcel_t saved_g;
};

ساكن مضمن mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  إذا (*have_saved)
    {
      *have_saved = false;
      أرجع *saved_g;
    }
  أرجع mbbuf_get_char (mbbuf);
}

ساكن مضمن تعداد ميدان_terminator
skip_whitespace_لاڳيتو (mbbuf_t *mbuf, بنية mbfield_parser *parser,
                     bool *have_pending_سٽ,
                     bool have_initial_whitespace)
{
  mcel_t g;

  افعل
    {
      g = mbbuf_get_char (mbuf);
      إذا (g.ch != MBBUF_EOF)
        *have_pending_سٽ = true;
    }
  طالما (g.ch != MBBUF_EOF && g.ch != سٽ_delim && mcel_isblank (g));

  bool trim_شروعات = parser->trim_outer_whitespace && parser->at_سٽ_شروعات;

  إذا (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == سٽ_delim))
    {
      أرجع g.ch == MBBUF_EOF ? ميدان_EOF : ميدان_سٽ_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  أرجع trim_شروعات && !have_initial_whitespace ? ميدان_معلومات : ميدان_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

ساكن فراغ
write_بائيٽون (حرف ثابت *buf, size_t n_بائيٽون)
{
  إذا (n_بائيٽون <= SMALL_BYTE_حد)
    {
      لكل (size_t i = 0; i < n_بائيٽون; i++)
        إذا (اکر_لکو (buf[i]) < 0)
          write_error ();
      أرجع;
    }

  إذا (fwrite (buf, حجم (حرف), n_بائيٽون, stdout) != n_بائيٽون)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

ساكن مضمن فراغ
copy_بائيٽون (حرف *dst, حرف ثابت *src, size_t n_بائيٽون)
{
  إذا (n_بائيٽون <= SMALL_BYTE_حد)
    {
      لكل (size_t i = 0; i < n_بائيٽون; i++)
        dst[i] = src[i];
      أرجع;
    }

  يادگيري_نقل (dst, src, n_بائيٽون);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

ساكن مضمن فراغ*
ڳولا_بائيٽون (حرف ثابت *buf, صحيح c, size_t n_بائيٽون)
{
  إذا (n_بائيٽون != 0 && منزل_uchar (buf[0]) == c)
    أرجع (حرف *) buf;
  إذا (1 < n_بائيٽون && منزل_uchar (buf[1]) == c)
    أرجع (حرف *) buf + 1;

  أرجع 2 < n_بائيٽون ? (حرف *) memchr (buf + 2, c, n_بائيٽون - 2) : عدم;
}

ساكن مضمن فراغ
write_سٽ_delim (فراغ)
{
  إذا (اکر_لکو (سٽ_delim) < 0)
    write_error ();
}

ساكن مضمن فراغ
reset_شي_سٽ (uintmax_t *شي_idx, bool *print_delimiter)
{
  write_سٽ_delim ();
  *شي_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

ساكن مضمن فراغ
write_pending_سٽ_delim (uintmax_t شي_idx)
{
  إذا (شي_idx > 0)
    write_سٽ_delim ();
}

ساكن مضمن فراغ
write_selected_شي (bool *print_delimiter, bool range_شروعات,
                     حرف ثابت *buf, size_t n_بائيٽون)
{
  إذا (output_delimiter_string != output_delimiter_default)
    {
      إذا (*print_delimiter && range_شروعات)
        write_بائيٽون (output_delimiter_string, output_delimiter_ڊيگهائي);
      *print_delimiter = true;
    }

  write_بائيٽون (buf, n_بائيٽون);
}

ساكن مضمن فراغ
append_ميدان_1_chunk (حرف ثابت *buf, idx_t len, idx_t *n_بائيٽون)
{
  إذا (ميدان_1_bufsize - *n_بائيٽون < len)
    {
      ميدان_1_بفر = xpalloc (ميدان_1_بفر, &ميدان_1_bufsize,
                                len, -1, حجم *ميدان_1_بفر);
    }

  copy_بائيٽون (ميدان_1_بفر + *n_بائيٽون, buf, len);
  *n_بائيٽون += len;
}

ساكن مضمن فراغ
append_ميدان_1_بائيٽون (mbbuf_t *mbbuf, mcel_t g, idx_t *n_بائيٽون)
{
  append_ميدان_1_chunk (mbbuf_char_وٿي (mbbuf, g), g.len, n_بائيٽون);
}

ساكن مضمن تعداد ميدان_terminator
جاچ_mb_blank_ميدان (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
                     bool *have_pending_سٽ, bool write_ميدان,
                     idx_t *n_بائيٽون)
{
  إذا (parser->trim_outer_whitespace && parser->at_سٽ_شروعات)
    {
      تعداد ميدان_terminator terminator
        = skip_whitespace_لاڳيتو (mbbuf, parser, have_pending_سٽ, false);
      إذا (terminator != ميدان_معلومات)
        أرجع terminator;
    }

  parser->at_سٽ_شروعات = false;

  طالما (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      إذا (g.ch == MBBUF_EOF)
        أرجع ميدان_EOF;

      *have_pending_سٽ = true;

      إذا (g.ch == سٽ_delim)
        أرجع ميدان_سٽ_DELIMITER;

      إذا (mcel_isblank (g))
        أرجع skip_whitespace_لاڳيتو (mbbuf, parser, have_pending_سٽ, true);

      إذا (n_بائيٽون)
        append_ميدان_1_بائيٽون (mbbuf, g, n_بائيٽون);
      وإلا إذا (write_ميدان)
        write_بائيٽون (mbbuf_char_وٿي (mbbuf, g), g.len);
    }
}

ساكن مضمن تعداد ميدان_terminator
جاچ_mb_delim_ميدان (mbbuf_t *mbbuf, bool *have_pending_سٽ,
                     bool write_ميدان, idx_t *n_بائيٽون)
{
  طالما (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      إذا (g.ch == MBBUF_EOF)
        أرجع ميدان_EOF;

      *have_pending_سٽ = true;

      إذا (g.ch == سٽ_delim)
        أرجع ميدان_سٽ_DELIMITER;

      إذا (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        أرجع ميدان_DELIMITER;

      إذا (n_بائيٽون)
        append_ميدان_1_بائيٽون (mbbuf, g, n_بائيٽون);
      وإلا إذا (write_ميدان)
        write_بائيٽون (mbbuf_char_وٿي (mbbuf, g), g.len);
    }
}

ساكن مضمن تعداد ميدان_terminator
جاچ_mb_ميدان (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
               bool *have_pending_سٽ, bool write_ميدان, idx_t *n_بائيٽون)
{
  أرجع (parser->whitespace_delimited
          ? جاچ_mb_blank_ميدان (mbbuf, parser, have_pending_سٽ,
                                 write_ميدان, n_بائيٽون)
          : جاچ_mb_delim_ميدان (mbbuf, have_pending_سٽ, write_ميدان,
                                 n_بائيٽون));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

ساكن تعداد ميدان_terminator
جاچ_mb_سٽ_پڇاڙي (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
                  bool *have_pending_سٽ)
{
  إذا (parser->have_saved)
    {
      parser->have_saved = false;
      إذا (parser->saved_g.ch == MBBUF_EOF)
        أرجع ميدان_EOF;

      *have_pending_سٽ = true;
      إذا (parser->saved_g.ch == سٽ_delim)
        أرجع ميدان_سٽ_DELIMITER;
    }

  طالما (true)
    {
      idx_t موجود = mbbuf_topup (mbbuf);
      إذا (موجود == 0)
        أرجع ميدان_EOF;

      حرف *بفر = mbbuf->بفر + mbbuf->وٿي;
      حرف *سٽ_پڇاڙي = ڳولا_بائيٽون (بفر, سٽ_delim, موجود);
      idx_t n = سٽ_پڇاڙي ? سٽ_پڇاڙي - بفر + 1 : موجود;
      mbbuf_advance (mbbuf, n);
      *have_pending_سٽ = true;

      إذا (سٽ_پڇاڙي)
        أرجع ميدان_سٽ_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_خالص
ساكن حرف *
ڳوليو_ميدان_delim (حرف *buf, size_t len)
{
  إذا (len < delim_mcel.len)
    أرجع عدم;

#إذا ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  أرجع memmem (buf, len, delim_بائيٽون, delim_mcel.len);
#وإلا
  إذا (delim_mcel.len == 1)
    أرجع memchr (buf, delim_بائيٽون[0], len);

  حرف ثابت *p = buf;
  حرف ثابت *پڇاڙي = buf + len;

  حرف saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  طالما (p < پڇاڙي)
    {
      حرف ثابت *nul = memchr (p, '\0', پڇاڙي - p);
      إذا (!nul)
        {
          حرف ثابت *ميلاپ = لڙي_ڳولا (p, delim_بائيٽون);
          buf[len] = saved;
          أرجع (حرف *) ميلاپ;
        }

      إذا (p < nul)
        {
          حرف ثابت *ميلاپ = لڙي_ڳولا (p, delim_بائيٽون);
          إذا (ميلاپ)
            {
              buf[len] = saved;
              أرجع (حرف *) ميلاپ;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  أرجع عدم;
#جيڪڏهن_پورو
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_خالص
ساكن حرف *
ڳوليو_c_blank (حرف *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  لكل (idx_t i = 0; i < n; i++)
    إذا (c_isblank (buf[i]))
      أرجع buf + i;

  أرجع memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_خالص
ساكن idx_t
ميدان_delim_overlap (حرف ثابت *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  طالما (0 < overlap)
    {
      إذا (memeq (buf + len - overlap, delim_بائيٽون, overlap))
        أرجع overlap;
      overlap--;
    }

  أرجع 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

ساكن مضمن تعداد ميدان_terminator
ڳوليو_ميدان_terminator (حرف *buf, idx_t len,
                       بنية bytesearch_context *ctx, حرف **terminator)
{
  إذا (ctx->گهڻائي_2 == BYTESEARCH_سٽ_ONLY)
    {
      إذا (!ctx->سٽ_پڇاڙي_معلوم)
        {
          ctx->سٽ_پڇاڙي = memchr (buf, سٽ_delim, len);
          ctx->سٽ_پڇاڙي_معلوم = true;
        }

      *terminator = ctx->سٽ_پڇاڙي;
      أرجع *terminator ? ميدان_سٽ_DELIMITER : ميدان_معلومات;
    }

  إذا (ميدان_delim_is_سٽ_delim ())
    {
      *terminator = memchr (buf, سٽ_delim, len);
      أرجع *terminator ? ميدان_DELIMITER : ميدان_معلومات;
    }

  إذا (!ctx->سٽ_پڇاڙي_معلوم)
    {
      ctx->سٽ_پڇاڙي = memchr (buf, سٽ_delim, len);
      ctx->سٽ_پڇاڙي_معلوم = true;
    }

  idx_t ميدان_len = ctx->سٽ_پڇاڙي ? ctx->سٽ_پڇاڙي - buf : len;

  حرف *ميدان_پڇاڙي = (ctx->blank_delimited
                     ? ڳوليو_c_blank (buf, ميدان_len)
                     : ڳوليو_ميدان_delim (buf, ميدان_len));

  إذا (ميدان_پڇاڙي)
    {
      *terminator = ميدان_پڇاڙي;
      أرجع ميدان_DELIMITER;
    }

  *terminator = ctx->سٽ_پڇاڙي;
  أرجع ctx->سٽ_پڇاڙي ? ميدان_سٽ_DELIMITER : ميدان_معلومات;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

ساكن مضمن فراغ
maybe_write_سٽ_delim (bool مليو_any_selected_ميدان, uintmax_t ميدان_idx)
{
  إذا (مليو_any_selected_ميدان
      || !(suppress_non_delimited && ميدان_idx == 1))
    write_سٽ_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

ساكن مضمن bool
begin_ميدان_output (uintmax_t ميدان_idx, bool بفر_پهريون_ميدان,
                    bool *مليو_any_selected_ميدان)
{
  bool write_ميدان = print_kth (ميدان_idx);

  إذا (write_ميدان && ! (ميدان_idx == 1 && بفر_پهريون_ميدان))
    {
      إذا (*مليو_any_selected_ميدان)
        write_بائيٽون (output_delimiter_string, output_delimiter_ڊيگهائي);
      *مليو_any_selected_ميدان = true;
    }

  أرجع write_ميدان;
}

ساكن مضمن bool
handle_ميدان_1 (uintmax_t ميدان_idx, bool بفر_پهريون_ميدان,
                idx_t *ميدان_1_n_بائيٽون)
{
  bool ميدان_1_selected = false;

  إذا (ميدان_idx == 1 && بفر_پهريون_ميدان)
    {
      إذا ((ميدان_1_selected = print_kth (1)))
        write_بائيٽون (ميدان_1_بفر, *ميدان_1_n_بائيٽون);
      *ميدان_1_n_بائيٽون = 0;
    }

  أرجع ميدان_1_selected;
}

ساكن مضمن فراغ
handle_ميدان_delimiter (uintmax_t *ميدان_idx, bool بفر_پهريون_ميدان,
                        idx_t *ميدان_1_n_بائيٽون,
                        bool *مليو_any_selected_ميدان, bool *write_ميدان)
{
  إذا (handle_ميدان_1 (*ميدان_idx, بفر_پهريون_ميدان, ميدان_1_n_بائيٽون))
    *مليو_any_selected_ميدان = true;

  ايندڙ_شي (ميدان_idx);
  *write_ميدان = begin_ميدان_output (*ميدان_idx, بفر_پهريون_ميدان,
                                     مليو_any_selected_ميدان);
}

ساكن مضمن bool
ميدان_selection_exhausted (uintmax_t ميدان_idx)
{
  أرجع !print_kth (ميدان_idx) && current_rp->lo == UINTMAX_MAX;
}

ساكن مضمن فراغ
پڄاڻي_current_سٽ (uintmax_t ميدان_idx, bool بفر_پهريون_ميدان,
                     idx_t *ميدان_1_n_بائيٽون,
                     bool مليو_any_selected_ميدان, bool سٽ_terminated)
{
  إذا (ميدان_idx == 1 && بفر_پهريون_ميدان)
    {
      إذا (!suppress_non_delimited
          && (سٽ_terminated || *ميدان_1_n_بائيٽون != 0))
        {
          write_بائيٽون (ميدان_1_بفر, *ميدان_1_n_بائيٽون);
          write_سٽ_delim ();
        }
      *ميدان_1_n_بائيٽون = 0;
    }
  وإلا
    maybe_write_سٽ_delim (مليو_any_selected_ميدان, ميدان_idx);
}

ساكن مضمن فراغ
sync_byte_selection (uintmax_t byte_idx)
{
  طالما (current_rp->hi <= byte_idx)
    current_rp++;
}

ساكن مضمن فراغ
reset_ميدان_سٽ (uintmax_t *ميدان_idx, bool *مليو_any_selected_ميدان,
                  bool *have_pending_سٽ, بنية mbfield_parser *parser)
{
  *ميدان_idx = 1;
  current_rp = frp;
  *مليو_any_selected_ميدان = false;
  *have_pending_سٽ = false;
  parser->have_saved = false;
  parser->at_سٽ_شروعات = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

ساكن فراغ
cut_بائيٽون (FILE *وهڪرو)
{
  ساكن حرف بائيٽون_اندريون[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  صحيح fd = fileno (وهڪرو);

  current_rp = frp;

  طالما (true)
    {
      idx_t موجود = اقرأ (fd, بائيٽون_اندريون, حجم بائيٽون_اندريون);
      إذا (موجود <= 0)
        {
          إذا (موجود < 0)
            fseterr (وهڪرو);
          write_pending_سٽ_delim (byte_idx);
          اكسر;
        }

      idx_t processed = 0;

      طالما (processed < موجود)
        {
          حرف *سٽ = بائيٽون_اندريون + processed;
          idx_t منزل_process = موجود - processed;
          حرف *سٽ_پڇاڙي = ڳولا_بائيٽون (سٽ, سٽ_delim, منزل_process);
          حرف *پڇاڙي = سٽ + (سٽ_پڇاڙي ? سٽ_پڇاڙي - سٽ : منزل_process);
          حرف *p = سٽ;

          طالما (0 < پڇاڙي - p)
            {
              sync_byte_selection (byte_idx);

              إذا (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (پڇاڙي - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              وإلا
                {
                  idx_t n = MIN (پڇاڙي - p, current_rp->hi - byte_idx);
                  write_selected_شي (&print_delimiter,
                                       is_range_شروعات_اشاريو (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += پڇاڙي - سٽ;
          إذا (سٽ_پڇاڙي)
            {
              processed++;
              reset_شي_سٽ (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

ساكن فراغ
cut_characters_گهڻائي (FILE *وهڪرو, bool byte_گهڻائي)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  ساكن حرف بائيٽون_اندريون[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, بائيٽون_اندريون, حجم بائيٽون_اندريون, وهڪرو);

  طالما (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      إذا (idx && ميدان_selection_exhausted (idx))
        {
          idx_t موجود = mbbuf_topup (&mbbuf);
          حرف *p = mbbuf.بفر + mbbuf.وٿي;
          حرف *پڇاڙي = ڳولا_بائيٽون (p, سٽ_delim, موجود);
          mbbuf_advance (&mbbuf, پڇاڙي ? پڇاڙي - p : موجود);
          إذا (!پڇاڙي && موجود)
            استمر;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      إذا (!byte_گهڻائي && utf8 && idx + 1 < current_rp->lo)
        {
          حرف ثابت *p = mbbuf.بفر + mbbuf.وٿي;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          حرف ثابت *پڇاڙي = ڳولا_بائيٽون (p, سٽ_delim, n);
          إذا (پڇاڙي)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, پڇاڙي - p + 1);
              reset_شي_سٽ (&idx, &print_delimiter);
              استمر;
            }

          idx += u8_ڳڻپ (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      إذا (g.ch == سٽ_delim)
        reset_شي_سٽ (&idx, &print_delimiter);
      وإلا إذا (g.ch == MBBUF_EOF)
        {
          write_pending_سٽ_delim (idx);
          اكسر;
        }
      وإلا إذا (byte_گهڻائي)
        {
          bool پهريون_selected_is_range_شروعات = false;
          bool ڏٺل_selected = false;
          bool suffix_selected = true;

          لكل (idx_t i = 0; i < g.len; i++)
            {
              ايندڙ_شي (&idx);
              إذا (print_kth (idx))
                {
                  إذا (!ڏٺل_selected)
                    {
                      ڏٺل_selected = true;
                      پهريون_selected_is_range_شروعات
                        = is_range_شروعات_اشاريو (idx);
                    }
                }
              وإلا إذا (ڏٺل_selected)
                suffix_selected = false;
            }

          إذا (ڏٺل_selected && suffix_selected)
            write_selected_شي (&print_delimiter,پهريون_selected_is_range_شروعات,
                                 mbbuf_char_وٿي (&mbbuf, g), g.len);
        }
      وإلا
        {
          ايندڙ_شي (&idx);
          إذا (print_kth (idx))
            write_selected_شي (&print_delimiter,
                                 is_range_شروعات_اشاريو (idx),
                                 mbbuf_char_وٿي (&mbbuf, g), g.len);
        }
    }
}

ساكن فراغ
cut_بائيٽون_no_ورهايو (FILE *وهڪرو)
{
  cut_characters_گهڻائي (وهڪرو, true);
}

ساكن فراغ
cut_characters (FILE *وهڪرو)
{
  cut_characters_گهڻائي (وهڪرو, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

ساكن فراغ
cut_fields_mb_any (FILE *وهڪرو, bool whitespace_گهڻائي)
{
  ساكن حرف بائيٽون_اندريون[IO_BUFSIZE];
  mbbuf_t mbbuf;
  بنية mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_گهڻائي,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_سٽ_شروعات = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t ميدان_idx = 1;
  bool مليو_any_selected_ميدان = false;
  bool بفر_پهريون_ميدان;
  bool have_pending_سٽ = false;
  bool write_ميدان;
  idx_t ميدان_1_n_بائيٽون = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, بائيٽون_اندريون, حجم بائيٽون_اندريون, وهڪرو);

  بفر_پهريون_ميدان = (suppress_non_delimited ^ !print_kth (1));
  write_ميدان = begin_ميدان_output (ميدان_idx, بفر_پهريون_ميدان,
                                    &مليو_any_selected_ميدان);

  طالما (true)
    {
      تعداد ميدان_terminator terminator;
      إذا (!بفر_پهريون_ميدان || ميدان_idx != 1)
        terminator = جاچ_mb_ميدان (&mbbuf, &parser, &have_pending_سٽ,
                                    write_ميدان, عدم);
      وإلا
        terminator = جاچ_mb_ميدان (&mbbuf, &parser, &have_pending_سٽ,
                                    false, &ميدان_1_n_بائيٽون);

      إذا (terminator == ميدان_DELIMITER)
        {
          handle_ميدان_delimiter (&ميدان_idx, بفر_پهريون_ميدان,
                                  &ميدان_1_n_بائيٽون,
                                  &مليو_any_selected_ميدان, &write_ميدان);
          إذا (! ميدان_selection_exhausted (ميدان_idx))
            استمر;

          terminator = جاچ_mb_سٽ_پڇاڙي (&mbbuf, &parser,
                                         &have_pending_سٽ);
        }

      إذا (terminator == ميدان_EOF && !have_pending_سٽ)
        اكسر;

      پڄاڻي_current_سٽ (ميدان_idx, بفر_پهريون_ميدان,
                           &ميدان_1_n_بائيٽون, مليو_any_selected_ميدان,
                           terminator == ميدان_سٽ_DELIMITER);
      إذا (terminator == ميدان_EOF)
        اكسر;

      reset_ميدان_سٽ (&ميدان_idx, &مليو_any_selected_ميدان,
                        &have_pending_سٽ, &parser);
      write_ميدان = begin_ميدان_output (ميدان_idx, بفر_پهريون_ميدان,
                                        &مليو_any_selected_ميدان);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

ساكن فراغ
cut_fields_mb (FILE *وهڪرو)
{
  cut_fields_mb_any (وهڪرو, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

ساكن فراغ
cut_fields_bytesearch (FILE *وهڪرو)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  ساكن حرف بائيٽون_اندريون[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t ميدان_idx = 1;
  bool مليو_any_selected_ميدان = false;
  bool have_pending_سٽ = false;
  bool skip_blank_لاڳيتو = false;
  bool pending_سٽ_delim_ميدان = false;
  bool write_ميدان;
  idx_t ميدان_1_n_بائيٽون = 0;

  current_rp = frp;
  bool بفر_پهريون_ميدان = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, بائيٽون_اندريون, حجم بائيٽون_اندريون - 1, وهڪرو);
  write_ميدان = begin_ميدان_output (ميدان_idx, بفر_پهريون_ميدان,
                                    &مليو_any_selected_ميدان);

  بنية bytesearch_context ڳولا = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&ڳولا);

  طالما (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      ڳولا.at_eof = mbbuf.eof;
      ڳولا.سٽ_پڇاڙي_معلوم = false;

      إذا (pending_سٽ_delim_ميدان)
        {
          pending_سٽ_delim_ميدان = false;

          إذا (n_avail == 0)
            {
              إذا (handle_ميدان_1 (ميدان_idx, بفر_پهريون_ميدان,
                                  &ميدان_1_n_بائيٽون))
                ايندڙ_شي (&ميدان_idx);
              maybe_write_سٽ_delim (مليو_any_selected_ميدان, ميدان_idx);
              أرجع;
            }

          handle_ميدان_delimiter (&ميدان_idx, بفر_پهريون_ميدان,
                                  &ميدان_1_n_بائيٽون,
                                  &مليو_any_selected_ميدان, &write_ميدان);
          إذا (whitespace_delimited)
            skip_blank_لاڳيتو = true;
        }

      إذا (n_avail == 0)
        اكسر;

      حرف *chunk = mbbuf.بفر + mbbuf.وٿي;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      إذا (ميدان_idx == 1
          && (!whitespace_delimited || !ميدان_selection_exhausted (ميدان_idx))
          && !ميدان_delim_is_سٽ_delim ()
          && !(whitespace_delimited
               ? ڳوليو_c_blank (chunk, n_avail)
               : ڳوليو_ميدان_delim (chunk, n_avail)))
        {
          حرف *آخري_سٽ_delim = ڳولا.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, سٽ_delim, n_avail);
          إذا (آخري_سٽ_delim)
            {
              idx_t n = آخري_سٽ_delim - chunk + 1;
              إذا (! suppress_non_delimited)
                {
                  write_بائيٽون (ميدان_1_بفر, ميدان_1_n_بائيٽون);
                  write_بائيٽون (chunk, n);
                  إذا (ڳولا.at_eof && chunk[n - 1] != سٽ_delim)
                    write_سٽ_delim ();
                }
              ميدان_1_n_بائيٽون = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_سٽ = false;
              إذا (ڳولا.at_eof)
                أرجع;
              استمر;
            }
        }

      طالما (processed < n_avail)
        {
          حرف *terminator = عدم;

          إذا (skip_blank_لاڳيتو)
            {
              طالما (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              إذا (processed == n_avail)
                اكسر;
              skip_blank_لاڳيتو = false;
            }

          إذا (ميدان_selection_exhausted (ميدان_idx))
            {
              إذا (ميدان_delim_is_سٽ_delim ())
                {
                  maybe_write_سٽ_delim (مليو_any_selected_ميدان,
                                          ميدان_idx);
                  mbbuf_advance (&mbbuf, processed);
                  أرجع;
                }
              ڳولا.گهڻائي_2 = BYTESEARCH_سٽ_ONLY;
              تعداد ميدان_terminator terminator_قسم
                = ڳوليو_ميدان_terminator (chunk + processed, n_avail - processed,
                                         &ڳولا, &terminator);
              إذا (terminator_قسم == ميدان_سٽ_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_سٽ_delim (مليو_any_selected_ميدان,
                                          ميدان_idx);
                  اذهب reset_سٽ;
                }
              processed = n_avail;
              اكسر;
            }

          تعداد ميدان_terminator terminator_قسم
            = ڳوليو_ميدان_terminator (chunk + processed, n_avail - processed,
                                     &ڳولا, &terminator);
          idx_t ميدان_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          إذا (terminator_قسم == ميدان_معلومات
              && !ڳولا.at_eof
              && !whitespace_delimited
              && !ميدان_delim_is_سٽ_delim ())
            ميدان_len -= ميدان_delim_overlap (chunk + processed, ميدان_len);

          إذا (ميدان_len || terminator)
            have_pending_سٽ = true;

          /* Output field content.  */
          إذا (ميدان_idx == 1 && بفر_پهريون_ميدان)
            append_ميدان_1_chunk (chunk + processed, ميدان_len,
                                  &ميدان_1_n_بائيٽون);
          وإلا إذا (write_ميدان)
            write_بائيٽون (chunk + processed, ميدان_len);
          processed += ميدان_len;

          إذا (terminator_قسم == ميدان_معلومات)
            اكسر;

          إذا (terminator_قسم == ميدان_DELIMITER)
            {
              إذا (ميدان_delim_is_سٽ_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_سٽ_delim_ميدان = true;
                  اكسر;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_ميدان_delimiter (&ميدان_idx, بفر_پهريون_ميدان,
                                      &ميدان_1_n_بائيٽون,
                                      &مليو_any_selected_ميدان, &write_ميدان);
              إذا (whitespace_delimited)
                skip_blank_لاڳيتو = true;
            }
          وإلا
            {
              affirm (terminator_قسم == ميدان_سٽ_DELIMITER);
              processed++;
              پڄاڻي_current_سٽ (ميدان_idx, بفر_پهريون_ميدان,
                                   &ميدان_1_n_بائيٽون,
                                   مليو_any_selected_ميدان, true);

            reset_سٽ:
              ميدان_idx = 1;
              current_rp = frp;
              مليو_any_selected_ميدان = false;
              have_pending_سٽ = false;
              bytesearch_context_reset (&ڳولا);
              write_ميدان = begin_ميدان_output (ميدان_idx, بفر_پهريون_ميدان,
                                                &مليو_any_selected_ميدان);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  إذا (!have_pending_سٽ)
    أرجع;

  پڄاڻي_current_سٽ (ميدان_idx, بفر_پهريون_ميدان, &ميدان_1_n_بائيٽون,
                       مليو_any_selected_ميدان, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

ساكن فراغ
cut_fields_ws (FILE *وهڪرو)
{
  إذا (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (وهڪرو);
  وإلا
    cut_fields_mb_any (وهڪرو, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

ساكن bool
cut_file (حرف ثابت *file, فراغ (*cut_وهڪرو) (FILE *))
{
  FILE *وهڪرو;

  إذا (streq (file, "-"))
    {
      have_read_stdin = true;
      وهڪرو = stdin;
      assume (وهڪرو);  /* Pacify GCC bug#109613.  */
    }
  وإلا
    {
      وهڪرو = fopen (file, "r");
      إذا (وهڪرو == عدم)
        {
          error (0, errno, "%s", quotef (file));
          أرجع false;
        }
    }

  fadvise (وهڪرو, FADVISE_SEQUENTIAL);

  cut_وهڪرو (وهڪرو);

  صحيح err = errno;
  إذا (!ferror (وهڪرو))
    err = 0;
  إذا (streq (file, "-"))
    clearerr (وهڪرو);		/* Also clear EOF.  */
  وإلا إذا (fclose (وهڪرو) == EOF)
    err = errno;
  إذا (err)
    {
      error (0, err, "%s", quotef (file));
      أرجع false;
    }
  أرجع true;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  حرف *spec_list_string = عدم;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, عدم))
         != -1)
    {
      بدل (optc)
        {
        حالة 'b':
          cut_گهڻائي = CUT_گهڻائي_بائيٽون;
          FALLTHROUGH;
        حالة 'c':
          إذا (optc == 'c')
            cut_گهڻائي = CUT_گهڻائي_CHARACTERS;
          FALLTHROUGH;
        حالة 'f':
        حالة 'F':
          إذا (optc == 'f' || optc == 'F')
            cut_گهڻائي = CUT_گهڻائي_FIELDS;
          إذا (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          إذا (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          اكسر;

        حالة 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            إذا (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_بائيٽون (delim_بائيٽون, optarg, g.len + 1);
            delim_specified = true;
            اكسر;
          }

        حالة 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_دليلون,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          اكسر;

        حالة 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_ڊيگهائي = (optarg[0] == '\0'
                                     ? 1 : لڙي_ڊيگهه (optarg));
          output_delimiter_string = optarg;
          اكسر;

        حالة 'n':
          no_ورهايو = true;
          اكسر;

        حالة 's':
          suppress_non_delimited = true;
          اكسر;

        حالة 'z':
          سٽ_delim = '\0';
          اكسر;

        حالة COMPLEMENT_OPTION:
          complement = true;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  إذا (cut_گهڻائي == CUT_گهڻائي_بائيٽون || cut_گهڻائي == CUT_گهڻائي_CHARACTERS)
    {
      إذا (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an داخلا delimiter makes sense\n\
\tonly when operating on fields"));

      إذا (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited سٽون makes sense\n\
\tonly when operating on fields"));
    }

  إذا (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  إذا (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  سيٽ_fields (spec_list_string,
              (((cut_گهڻائي == CUT_گهڻائي_بائيٽون
                 || cut_گهڻائي == CUT_گهڻائي_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  إذا (!delim_specified)
    {
      delim_بائيٽون[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  إذا (output_delimiter_string == عدم)
    {
      output_delimiter_string = output_delimiter_default;
      إذا (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_ڊيگهائي = 1;
        }
      وإلا
        {
          copy_بائيٽون (output_delimiter_default, delim_بائيٽون, delim_mcel.len);
          output_delimiter_ڊيگهائي = delim_mcel.len;
        }
    }

  فراغ (*cut_وهڪرو) (FILE *) = عدم;
  بدل (cut_گهڻائي)
    {
    حالة CUT_گهڻائي_بائيٽون:
      cut_وهڪرو = MB_CUR_MAX <= 1 || !no_ورهايو
                   ? cut_بائيٽون : cut_بائيٽون_no_ورهايو;
      اكسر;

    حالة CUT_گهڻائي_CHARACTERS:
      cut_وهڪرو = MB_CUR_MAX <= 1 ? cut_بائيٽون : cut_characters;
      اكسر;

    حالة CUT_گهڻائي_FIELDS:
      cut_وهڪرو = whitespace_delimited ? cut_fields_ws
                   : bytesearch_ميدان_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      اكسر;

    حالة CUT_گهڻائي_NONE:
      affirm (false);
    }
  affirm (cut_وهڪرو);
  إذا (optind == argc)
    ok = cut_file ("-", cut_وهڪرو);
  وإلا
    لكل (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_وهڪرو);


  إذا (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
