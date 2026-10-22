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
#تعریف_کن برنامه_NAME "cut"

#تعریف_کن AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#تعریف_کن FATAL_ERROR(پیام)						\
  افعل									\
    {									\
      error (0, 0, (پیام));						\
      usage (EXIT_FAILURE);						\
    }									\
  طالما (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#تعریف_کن SMALL_BYTE_آستانه 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
ساكن بنية فیلد_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
ساكن حرف *فیلد_1_میانگیر;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
ساكن idx_t فیلد_1_bufsize;

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
ساكن حرف delim_بایتها[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
ساكن بیعلامت حرف سطر_delim = '\n';

/* The length of output_delimiter_string.  */
ساكن size_t output_delimiter_درازی;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
ساكن حرف *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
ساكن حرف output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
ساكن bool no_تقسیم;

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

ساكن حرف ثابت *ثابت whitespace_option_آرگومانها[] =
{
  "trimmed", عدم
};

ساكن تعداد whitespace_option ثابت whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_آرگومانها, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
ساكن تعداد
{
  CUT_نما_NONE,
  CUT_نما_بایتها,
  CUT_نما_CHARACTERS,
  CUT_نما_FIELDS
} cut_نما = CUT_نما_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"bytes", required_آرگومان, عدم, 'b'},
  {"characters", required_آرگومان, عدم, 'c'},
  {"fields", required_آرگومان, عدم, 'f'},
  {"delimiter", required_آرگومان, عدم, 'd'},
  {"no-partial", no_آرگومان, عدم, 'n'},
  {"whitespace-delimited", optional_آرگومان, عدم, 'w'},
  {"only-delimited", no_آرگومان, عدم, 's'},
  {"output-delimiter", required_آرگومان, عدم, 'O'},
  {"complement", no_آرگومان, عدم, COMPLEMENT_OPTION},
  {"zero-terminated", no_آرگومان, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              برنامه_name);
      fputs (_("\
Print selected parts از سطرها from each FILE مقصد standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --بایتها=LIST\n\
         select only these byte positions\n\
"));
      oputs (_("\
  -c, --characters=LIST\n\
         select only these character positions\n\
"));
      oputs (_("\
      --complement\n\
         complement the مجموعه_2 از selected بایتها, characters or fields\n\
"));
      oputs (_("\
  -d, --delimiter=DELIM\n\
         use DELIM instead از TAB لكل فیلد delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any سطر that شامل_است\n\
         no delimiter character, unless the -s option is specified\n\
"));
      oputs (_("\
  -F LIST\n\
         like -f, but also نتیجه_میدهد -w and -O ' '\n\
"));
      oputs (_("\
  -n, --no-partial\n\
         with -b, don't output partial multi-byte characters\n\
"));
      oputs (_("\
  -O, --output-delimiter=STRING\n\
         use STRING as the output delimiter;\n\
         the افتراضي is مقصد use the درون_داد delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         افعل not print سطرها not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a پیاپی_2 از blank characters as the فیلد delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one از -b, -c, -f or -F.  Each LIST is made به_بالا از one\n\
range, or many ranges separated by commas.  Selected درون_داد is written\n\
درون the same ترتیب that it is اقرأ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one از:\n\
\n\
  N     N'th byte, character or فیلد, counted from 1\n\
  N-    from N'th byte, character or فیلد, مقصد انتها از سطر\n\
  N-M   from N'th to M'th (included) byte, character or فیلد\n\
  -M    from اولی مقصد M'th (included) byte, character or فیلد\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

ساكن مضمن فراغ
بعدی_قلم (uintmax_t *قلم_idx)
{
  (*قلم_idx)++;
  إذا ((*قلم_idx) > current_rp->hi)
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
is_range_شروع_شاخص (uintmax_t k)
{
  أرجع k == current_rp->lo;
}

ساكن مضمن bool
فیلد_delim_is_سطر_delim (فراغ)
{
  أرجع delim_mcel.len == 1 && delim_بایتها[0] == سطر_delim;
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
bytesearch_فیلد_delim_ok (فراغ)
{
  بیعلامت حرف delim_0 = delim_بایتها[0];

  أرجع (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


تعداد فیلد_terminator
{
  فیلد_داده,
  فیلد_DELIMITER,
  فیلد_سطر_DELIMITER,
  فیلد_EOF
};

تعداد bytesearch_نما
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_سطر_ONLY
};

بنية bytesearch_context
{
  تعداد bytesearch_نما نما_2;
  bool blank_delimited;
  bool at_eof;
  حرف *سطر_انتها;
  bool سطر_انتها_معلوم;
};

ساكن مضمن فراغ
bytesearch_context_reset (بنية bytesearch_context *ctx)
{
  ctx->نما_2 = BYTESEARCH_FIELDS;
  ctx->سطر_انتها = عدم;
  ctx->سطر_انتها_معلوم = false;
}

بنية mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_سطر_شروع;
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

ساكن مضمن تعداد فیلد_terminator
skip_whitespace_پیاپی (mbbuf_t *mbuf, بنية mbfield_parser *parser,
                     bool *have_pending_سطر,
                     bool have_initial_whitespace)
{
  mcel_t g;

  افعل
    {
      g = mbbuf_get_char (mbuf);
      إذا (g.ch != MBBUF_EOF)
        *have_pending_سطر = true;
    }
  طالما (g.ch != MBBUF_EOF && g.ch != سطر_delim && mcel_isblank (g));

  bool trim_شروع = parser->trim_outer_whitespace && parser->at_سطر_شروع;

  إذا (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == سطر_delim))
    {
      أرجع g.ch == MBBUF_EOF ? فیلد_EOF : فیلد_سطر_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  أرجع trim_شروع && !have_initial_whitespace ? فیلد_داده : فیلد_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

ساكن فراغ
write_بایتها (حرف ثابت *buf, size_t n_بایتها)
{
  إذا (n_بایتها <= SMALL_BYTE_آستانه)
    {
      لكل (size_t i = 0; i < n_بایتها; i++)
        إذا (حرف_چاپ_کن (buf[i]) < 0)
          write_error ();
      أرجع;
    }

  إذا (fwrite (buf, حجم (حرف), n_بایتها, stdout) != n_بایتها)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

ساكن مضمن فراغ
copy_بایتها (حرف *dst, حرف ثابت *src, size_t n_بایتها)
{
  إذا (n_بایتها <= SMALL_BYTE_آستانه)
    {
      لكل (size_t i = 0; i < n_بایتها; i++)
        dst[i] = src[i];
      أرجع;
    }

  کپی_حافظه (dst, src, n_بایتها);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

ساكن مضمن فراغ*
جستجو_بایتها (حرف ثابت *buf, صحيح c, size_t n_بایتها)
{
  إذا (n_بایتها != 0 && مقصد_uchar (buf[0]) == c)
    أرجع (حرف *) buf;
  إذا (1 < n_بایتها && مقصد_uchar (buf[1]) == c)
    أرجع (حرف *) buf + 1;

  أرجع 2 < n_بایتها ? (حرف *) memchr (buf + 2, c, n_بایتها - 2) : عدم;
}

ساكن مضمن فراغ
write_سطر_delim (فراغ)
{
  إذا (حرف_چاپ_کن (سطر_delim) < 0)
    write_error ();
}

ساكن مضمن فراغ
reset_قلم_سطر (uintmax_t *قلم_idx, bool *print_delimiter)
{
  write_سطر_delim ();
  *قلم_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

ساكن مضمن فراغ
write_pending_سطر_delim (uintmax_t قلم_idx)
{
  إذا (قلم_idx > 0)
    write_سطر_delim ();
}

ساكن مضمن فراغ
write_selected_قلم (bool *print_delimiter, bool range_شروع,
                     حرف ثابت *buf, size_t n_بایتها)
{
  إذا (output_delimiter_string != output_delimiter_default)
    {
      إذا (*print_delimiter && range_شروع)
        write_بایتها (output_delimiter_string, output_delimiter_درازی);
      *print_delimiter = true;
    }

  write_بایتها (buf, n_بایتها);
}

ساكن مضمن فراغ
append_فیلد_1_chunk (حرف ثابت *buf, idx_t len, idx_t *n_بایتها)
{
  إذا (فیلد_1_bufsize - *n_بایتها < len)
    {
      فیلد_1_میانگیر = xpalloc (فیلد_1_میانگیر, &فیلد_1_bufsize,
                                len, -1, حجم *فیلد_1_میانگیر);
    }

  copy_بایتها (فیلد_1_میانگیر + *n_بایتها, buf, len);
  *n_بایتها += len;
}

ساكن مضمن فراغ
append_فیلد_1_بایتها (mbbuf_t *mbbuf, mcel_t g, idx_t *n_بایتها)
{
  append_فیلد_1_chunk (mbbuf_char_جابه_جایی (mbbuf, g), g.len, n_بایتها);
}

ساكن مضمن تعداد فیلد_terminator
پیمایش_mb_blank_فیلد (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
                     bool *have_pending_سطر, bool write_فیلد,
                     idx_t *n_بایتها)
{
  إذا (parser->trim_outer_whitespace && parser->at_سطر_شروع)
    {
      تعداد فیلد_terminator terminator
        = skip_whitespace_پیاپی (mbbuf, parser, have_pending_سطر, false);
      إذا (terminator != فیلد_داده)
        أرجع terminator;
    }

  parser->at_سطر_شروع = false;

  طالما (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      إذا (g.ch == MBBUF_EOF)
        أرجع فیلد_EOF;

      *have_pending_سطر = true;

      إذا (g.ch == سطر_delim)
        أرجع فیلد_سطر_DELIMITER;

      إذا (mcel_isblank (g))
        أرجع skip_whitespace_پیاپی (mbbuf, parser, have_pending_سطر, true);

      إذا (n_بایتها)
        append_فیلد_1_بایتها (mbbuf, g, n_بایتها);
      وإلا إذا (write_فیلد)
        write_بایتها (mbbuf_char_جابه_جایی (mbbuf, g), g.len);
    }
}

ساكن مضمن تعداد فیلد_terminator
پیمایش_mb_delim_فیلد (mbbuf_t *mbbuf, bool *have_pending_سطر,
                     bool write_فیلد, idx_t *n_بایتها)
{
  طالما (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      إذا (g.ch == MBBUF_EOF)
        أرجع فیلد_EOF;

      *have_pending_سطر = true;

      إذا (g.ch == سطر_delim)
        أرجع فیلد_سطر_DELIMITER;

      إذا (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        أرجع فیلد_DELIMITER;

      إذا (n_بایتها)
        append_فیلد_1_بایتها (mbbuf, g, n_بایتها);
      وإلا إذا (write_فیلد)
        write_بایتها (mbbuf_char_جابه_جایی (mbbuf, g), g.len);
    }
}

ساكن مضمن تعداد فیلد_terminator
پیمایش_mb_فیلد (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
               bool *have_pending_سطر, bool write_فیلد, idx_t *n_بایتها)
{
  أرجع (parser->whitespace_delimited
          ? پیمایش_mb_blank_فیلد (mbbuf, parser, have_pending_سطر,
                                 write_فیلد, n_بایتها)
          : پیمایش_mb_delim_فیلد (mbbuf, have_pending_سطر, write_فیلد,
                                 n_بایتها));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

ساكن تعداد فیلد_terminator
پیمایش_mb_سطر_انتها (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
                  bool *have_pending_سطر)
{
  إذا (parser->have_saved)
    {
      parser->have_saved = false;
      إذا (parser->saved_g.ch == MBBUF_EOF)
        أرجع فیلد_EOF;

      *have_pending_سطر = true;
      إذا (parser->saved_g.ch == سطر_delim)
        أرجع فیلد_سطر_DELIMITER;
    }

  طالما (true)
    {
      idx_t موجود = mbbuf_topup (mbbuf);
      إذا (موجود == 0)
        أرجع فیلد_EOF;

      حرف *میانگیر = mbbuf->میانگیر + mbbuf->جابه_جایی;
      حرف *سطر_انتها = جستجو_بایتها (میانگیر, سطر_delim, موجود);
      idx_t n = سطر_انتها ? سطر_انتها - میانگیر + 1 : موجود;
      mbbuf_advance (mbbuf, n);
      *have_pending_سطر = true;

      إذا (سطر_انتها)
        أرجع فیلد_سطر_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_خالص
ساكن حرف *
پیدا_کن_فیلد_delim (حرف *buf, size_t len)
{
  إذا (len < delim_mcel.len)
    أرجع عدم;

#إذا ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  أرجع memmem (buf, len, delim_بایتها, delim_mcel.len);
#وإلا
  إذا (delim_mcel.len == 1)
    أرجع memchr (buf, delim_بایتها[0], len);

  حرف ثابت *p = buf;
  حرف ثابت *انتها = buf + len;

  حرف saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  طالما (p < انتها)
    {
      حرف ثابت *nul = memchr (p, '\0', انتها - p);
      إذا (!nul)
        {
          حرف ثابت *تطابق = جستجوی_رشته (p, delim_بایتها);
          buf[len] = saved;
          أرجع (حرف *) تطابق;
        }

      إذا (p < nul)
        {
          حرف ثابت *تطابق = جستجوی_رشته (p, delim_بایتها);
          إذا (تطابق)
            {
              buf[len] = saved;
              أرجع (حرف *) تطابق;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  أرجع عدم;
#پایان_اگر
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_خالص
ساكن حرف *
پیدا_کن_c_blank (حرف *buf, size_t len)
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
فیلد_delim_overlap (حرف ثابت *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  طالما (0 < overlap)
    {
      إذا (memeq (buf + len - overlap, delim_بایتها, overlap))
        أرجع overlap;
      overlap--;
    }

  أرجع 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

ساكن مضمن تعداد فیلد_terminator
پیدا_کن_فیلد_terminator (حرف *buf, idx_t len,
                       بنية bytesearch_context *ctx, حرف **terminator)
{
  إذا (ctx->نما_2 == BYTESEARCH_سطر_ONLY)
    {
      إذا (!ctx->سطر_انتها_معلوم)
        {
          ctx->سطر_انتها = memchr (buf, سطر_delim, len);
          ctx->سطر_انتها_معلوم = true;
        }

      *terminator = ctx->سطر_انتها;
      أرجع *terminator ? فیلد_سطر_DELIMITER : فیلد_داده;
    }

  إذا (فیلد_delim_is_سطر_delim ())
    {
      *terminator = memchr (buf, سطر_delim, len);
      أرجع *terminator ? فیلد_DELIMITER : فیلد_داده;
    }

  إذا (!ctx->سطر_انتها_معلوم)
    {
      ctx->سطر_انتها = memchr (buf, سطر_delim, len);
      ctx->سطر_انتها_معلوم = true;
    }

  idx_t فیلد_len = ctx->سطر_انتها ? ctx->سطر_انتها - buf : len;

  حرف *فیلد_انتها = (ctx->blank_delimited
                     ? پیدا_کن_c_blank (buf, فیلد_len)
                     : پیدا_کن_فیلد_delim (buf, فیلد_len));

  إذا (فیلد_انتها)
    {
      *terminator = فیلد_انتها;
      أرجع فیلد_DELIMITER;
    }

  *terminator = ctx->سطر_انتها;
  أرجع ctx->سطر_انتها ? فیلد_سطر_DELIMITER : فیلد_داده;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

ساكن مضمن فراغ
maybe_write_سطر_delim (bool یافت_any_selected_فیلد, uintmax_t فیلد_idx)
{
  إذا (یافت_any_selected_فیلد
      || !(suppress_non_delimited && فیلد_idx == 1))
    write_سطر_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

ساكن مضمن bool
begin_فیلد_output (uintmax_t فیلد_idx, bool میانگیر_اولی_فیلد,
                    bool *یافت_any_selected_فیلد)
{
  bool write_فیلد = print_kth (فیلد_idx);

  إذا (write_فیلد && ! (فیلد_idx == 1 && میانگیر_اولی_فیلد))
    {
      إذا (*یافت_any_selected_فیلد)
        write_بایتها (output_delimiter_string, output_delimiter_درازی);
      *یافت_any_selected_فیلد = true;
    }

  أرجع write_فیلد;
}

ساكن مضمن bool
handle_فیلد_1 (uintmax_t فیلد_idx, bool میانگیر_اولی_فیلد,
                idx_t *فیلد_1_n_بایتها)
{
  bool فیلد_1_selected = false;

  إذا (فیلد_idx == 1 && میانگیر_اولی_فیلد)
    {
      إذا ((فیلد_1_selected = print_kth (1)))
        write_بایتها (فیلد_1_میانگیر, *فیلد_1_n_بایتها);
      *فیلد_1_n_بایتها = 0;
    }

  أرجع فیلد_1_selected;
}

ساكن مضمن فراغ
handle_فیلد_delimiter (uintmax_t *فیلد_idx, bool میانگیر_اولی_فیلد,
                        idx_t *فیلد_1_n_بایتها,
                        bool *یافت_any_selected_فیلد, bool *write_فیلد)
{
  إذا (handle_فیلد_1 (*فیلد_idx, میانگیر_اولی_فیلد, فیلد_1_n_بایتها))
    *یافت_any_selected_فیلد = true;

  بعدی_قلم (فیلد_idx);
  *write_فیلد = begin_فیلد_output (*فیلد_idx, میانگیر_اولی_فیلد,
                                     یافت_any_selected_فیلد);
}

ساكن مضمن bool
فیلد_selection_exhausted (uintmax_t فیلد_idx)
{
  أرجع !print_kth (فیلد_idx) && current_rp->lo == UINTMAX_MAX;
}

ساكن مضمن فراغ
پایان_current_سطر (uintmax_t فیلد_idx, bool میانگیر_اولی_فیلد,
                     idx_t *فیلد_1_n_بایتها,
                     bool یافت_any_selected_فیلد, bool سطر_terminated)
{
  إذا (فیلد_idx == 1 && میانگیر_اولی_فیلد)
    {
      إذا (!suppress_non_delimited
          && (سطر_terminated || *فیلد_1_n_بایتها != 0))
        {
          write_بایتها (فیلد_1_میانگیر, *فیلد_1_n_بایتها);
          write_سطر_delim ();
        }
      *فیلد_1_n_بایتها = 0;
    }
  وإلا
    maybe_write_سطر_delim (یافت_any_selected_فیلد, فیلد_idx);
}

ساكن مضمن فراغ
sync_byte_selection (uintmax_t byte_idx)
{
  طالما (current_rp->hi <= byte_idx)
    current_rp++;
}

ساكن مضمن فراغ
reset_فیلد_سطر (uintmax_t *فیلد_idx, bool *یافت_any_selected_فیلد,
                  bool *have_pending_سطر, بنية mbfield_parser *parser)
{
  *فیلد_idx = 1;
  current_rp = frp;
  *یافت_any_selected_فیلد = false;
  *have_pending_سطر = false;
  parser->have_saved = false;
  parser->at_سطر_شروع = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

ساكن فراغ
cut_بایتها (FILE *جریان)
{
  ساكن حرف بایتها_درون[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  صحيح fd = fileno (جریان);

  current_rp = frp;

  طالما (true)
    {
      idx_t موجود = اقرأ (fd, بایتها_درون, حجم بایتها_درون);
      إذا (موجود <= 0)
        {
          إذا (موجود < 0)
            fseterr (جریان);
          write_pending_سطر_delim (byte_idx);
          اكسر;
        }

      idx_t processed = 0;

      طالما (processed < موجود)
        {
          حرف *سطر = بایتها_درون + processed;
          idx_t مقصد_process = موجود - processed;
          حرف *سطر_انتها = جستجو_بایتها (سطر, سطر_delim, مقصد_process);
          حرف *انتها = سطر + (سطر_انتها ? سطر_انتها - سطر : مقصد_process);
          حرف *p = سطر;

          طالما (0 < انتها - p)
            {
              sync_byte_selection (byte_idx);

              إذا (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (انتها - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              وإلا
                {
                  idx_t n = MIN (انتها - p, current_rp->hi - byte_idx);
                  write_selected_قلم (&print_delimiter,
                                       is_range_شروع_شاخص (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += انتها - سطر;
          إذا (سطر_انتها)
            {
              processed++;
              reset_قلم_سطر (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

ساكن فراغ
cut_characters_نما (FILE *جریان, bool byte_نما)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  ساكن حرف بایتها_درون[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, بایتها_درون, حجم بایتها_درون, جریان);

  طالما (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      إذا (idx && فیلد_selection_exhausted (idx))
        {
          idx_t موجود = mbbuf_topup (&mbbuf);
          حرف *p = mbbuf.میانگیر + mbbuf.جابه_جایی;
          حرف *انتها = جستجو_بایتها (p, سطر_delim, موجود);
          mbbuf_advance (&mbbuf, انتها ? انتها - p : موجود);
          إذا (!انتها && موجود)
            استمر;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      إذا (!byte_نما && utf8 && idx + 1 < current_rp->lo)
        {
          حرف ثابت *p = mbbuf.میانگیر + mbbuf.جابه_جایی;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          حرف ثابت *انتها = جستجو_بایتها (p, سطر_delim, n);
          إذا (انتها)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, انتها - p + 1);
              reset_قلم_سطر (&idx, &print_delimiter);
              استمر;
            }

          idx += u8_شمار (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      إذا (g.ch == سطر_delim)
        reset_قلم_سطر (&idx, &print_delimiter);
      وإلا إذا (g.ch == MBBUF_EOF)
        {
          write_pending_سطر_delim (idx);
          اكسر;
        }
      وإلا إذا (byte_نما)
        {
          bool اولی_selected_is_range_شروع = false;
          bool دیده_selected = false;
          bool suffix_selected = true;

          لكل (idx_t i = 0; i < g.len; i++)
            {
              بعدی_قلم (&idx);
              إذا (print_kth (idx))
                {
                  إذا (!دیده_selected)
                    {
                      دیده_selected = true;
                      اولی_selected_is_range_شروع
                        = is_range_شروع_شاخص (idx);
                    }
                }
              وإلا إذا (دیده_selected)
                suffix_selected = false;
            }

          إذا (دیده_selected && suffix_selected)
            write_selected_قلم (&print_delimiter,اولی_selected_is_range_شروع,
                                 mbbuf_char_جابه_جایی (&mbbuf, g), g.len);
        }
      وإلا
        {
          بعدی_قلم (&idx);
          إذا (print_kth (idx))
            write_selected_قلم (&print_delimiter,
                                 is_range_شروع_شاخص (idx),
                                 mbbuf_char_جابه_جایی (&mbbuf, g), g.len);
        }
    }
}

ساكن فراغ
cut_بایتها_no_تقسیم (FILE *جریان)
{
  cut_characters_نما (جریان, true);
}

ساكن فراغ
cut_characters (FILE *جریان)
{
  cut_characters_نما (جریان, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

ساكن فراغ
cut_fields_mb_any (FILE *جریان, bool whitespace_نما)
{
  ساكن حرف بایتها_درون[IO_BUFSIZE];
  mbbuf_t mbbuf;
  بنية mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_نما,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_سطر_شروع = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t فیلد_idx = 1;
  bool یافت_any_selected_فیلد = false;
  bool میانگیر_اولی_فیلد;
  bool have_pending_سطر = false;
  bool write_فیلد;
  idx_t فیلد_1_n_بایتها = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, بایتها_درون, حجم بایتها_درون, جریان);

  میانگیر_اولی_فیلد = (suppress_non_delimited ^ !print_kth (1));
  write_فیلد = begin_فیلد_output (فیلد_idx, میانگیر_اولی_فیلد,
                                    &یافت_any_selected_فیلد);

  طالما (true)
    {
      تعداد فیلد_terminator terminator;
      إذا (!میانگیر_اولی_فیلد || فیلد_idx != 1)
        terminator = پیمایش_mb_فیلد (&mbbuf, &parser, &have_pending_سطر,
                                    write_فیلد, عدم);
      وإلا
        terminator = پیمایش_mb_فیلد (&mbbuf, &parser, &have_pending_سطر,
                                    false, &فیلد_1_n_بایتها);

      إذا (terminator == فیلد_DELIMITER)
        {
          handle_فیلد_delimiter (&فیلد_idx, میانگیر_اولی_فیلد,
                                  &فیلد_1_n_بایتها,
                                  &یافت_any_selected_فیلد, &write_فیلد);
          إذا (! فیلد_selection_exhausted (فیلد_idx))
            استمر;

          terminator = پیمایش_mb_سطر_انتها (&mbbuf, &parser,
                                         &have_pending_سطر);
        }

      إذا (terminator == فیلد_EOF && !have_pending_سطر)
        اكسر;

      پایان_current_سطر (فیلد_idx, میانگیر_اولی_فیلد,
                           &فیلد_1_n_بایتها, یافت_any_selected_فیلد,
                           terminator == فیلد_سطر_DELIMITER);
      إذا (terminator == فیلد_EOF)
        اكسر;

      reset_فیلد_سطر (&فیلد_idx, &یافت_any_selected_فیلد,
                        &have_pending_سطر, &parser);
      write_فیلد = begin_فیلد_output (فیلد_idx, میانگیر_اولی_فیلد,
                                        &یافت_any_selected_فیلد);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

ساكن فراغ
cut_fields_mb (FILE *جریان)
{
  cut_fields_mb_any (جریان, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

ساكن فراغ
cut_fields_bytesearch (FILE *جریان)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  ساكن حرف بایتها_درون[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t فیلد_idx = 1;
  bool یافت_any_selected_فیلد = false;
  bool have_pending_سطر = false;
  bool skip_blank_پیاپی = false;
  bool pending_سطر_delim_فیلد = false;
  bool write_فیلد;
  idx_t فیلد_1_n_بایتها = 0;

  current_rp = frp;
  bool میانگیر_اولی_فیلد = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, بایتها_درون, حجم بایتها_درون - 1, جریان);
  write_فیلد = begin_فیلد_output (فیلد_idx, میانگیر_اولی_فیلد,
                                    &یافت_any_selected_فیلد);

  بنية bytesearch_context جستجو = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&جستجو);

  طالما (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      جستجو.at_eof = mbbuf.eof;
      جستجو.سطر_انتها_معلوم = false;

      إذا (pending_سطر_delim_فیلد)
        {
          pending_سطر_delim_فیلد = false;

          إذا (n_avail == 0)
            {
              إذا (handle_فیلد_1 (فیلد_idx, میانگیر_اولی_فیلد,
                                  &فیلد_1_n_بایتها))
                بعدی_قلم (&فیلد_idx);
              maybe_write_سطر_delim (یافت_any_selected_فیلد, فیلد_idx);
              أرجع;
            }

          handle_فیلد_delimiter (&فیلد_idx, میانگیر_اولی_فیلد,
                                  &فیلد_1_n_بایتها,
                                  &یافت_any_selected_فیلد, &write_فیلد);
          إذا (whitespace_delimited)
            skip_blank_پیاپی = true;
        }

      إذا (n_avail == 0)
        اكسر;

      حرف *chunk = mbbuf.میانگیر + mbbuf.جابه_جایی;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      إذا (فیلد_idx == 1
          && (!whitespace_delimited || !فیلد_selection_exhausted (فیلد_idx))
          && !فیلد_delim_is_سطر_delim ()
          && !(whitespace_delimited
               ? پیدا_کن_c_blank (chunk, n_avail)
               : پیدا_کن_فیلد_delim (chunk, n_avail)))
        {
          حرف *آخرین_سطر_delim = جستجو.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, سطر_delim, n_avail);
          إذا (آخرین_سطر_delim)
            {
              idx_t n = آخرین_سطر_delim - chunk + 1;
              إذا (! suppress_non_delimited)
                {
                  write_بایتها (فیلد_1_میانگیر, فیلد_1_n_بایتها);
                  write_بایتها (chunk, n);
                  إذا (جستجو.at_eof && chunk[n - 1] != سطر_delim)
                    write_سطر_delim ();
                }
              فیلد_1_n_بایتها = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_سطر = false;
              إذا (جستجو.at_eof)
                أرجع;
              استمر;
            }
        }

      طالما (processed < n_avail)
        {
          حرف *terminator = عدم;

          إذا (skip_blank_پیاپی)
            {
              طالما (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              إذا (processed == n_avail)
                اكسر;
              skip_blank_پیاپی = false;
            }

          إذا (فیلد_selection_exhausted (فیلد_idx))
            {
              إذا (فیلد_delim_is_سطر_delim ())
                {
                  maybe_write_سطر_delim (یافت_any_selected_فیلد,
                                          فیلد_idx);
                  mbbuf_advance (&mbbuf, processed);
                  أرجع;
                }
              جستجو.نما_2 = BYTESEARCH_سطر_ONLY;
              تعداد فیلد_terminator terminator_نوع
                = پیدا_کن_فیلد_terminator (chunk + processed, n_avail - processed,
                                         &جستجو, &terminator);
              إذا (terminator_نوع == فیلد_سطر_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_سطر_delim (یافت_any_selected_فیلد,
                                          فیلد_idx);
                  اذهب reset_سطر;
                }
              processed = n_avail;
              اكسر;
            }

          تعداد فیلد_terminator terminator_نوع
            = پیدا_کن_فیلد_terminator (chunk + processed, n_avail - processed,
                                     &جستجو, &terminator);
          idx_t فیلد_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          إذا (terminator_نوع == فیلد_داده
              && !جستجو.at_eof
              && !whitespace_delimited
              && !فیلد_delim_is_سطر_delim ())
            فیلد_len -= فیلد_delim_overlap (chunk + processed, فیلد_len);

          إذا (فیلد_len || terminator)
            have_pending_سطر = true;

          /* Output field content.  */
          إذا (فیلد_idx == 1 && میانگیر_اولی_فیلد)
            append_فیلد_1_chunk (chunk + processed, فیلد_len,
                                  &فیلد_1_n_بایتها);
          وإلا إذا (write_فیلد)
            write_بایتها (chunk + processed, فیلد_len);
          processed += فیلد_len;

          إذا (terminator_نوع == فیلد_داده)
            اكسر;

          إذا (terminator_نوع == فیلد_DELIMITER)
            {
              إذا (فیلد_delim_is_سطر_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_سطر_delim_فیلد = true;
                  اكسر;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_فیلد_delimiter (&فیلد_idx, میانگیر_اولی_فیلد,
                                      &فیلد_1_n_بایتها,
                                      &یافت_any_selected_فیلد, &write_فیلد);
              إذا (whitespace_delimited)
                skip_blank_پیاپی = true;
            }
          وإلا
            {
              affirm (terminator_نوع == فیلد_سطر_DELIMITER);
              processed++;
              پایان_current_سطر (فیلد_idx, میانگیر_اولی_فیلد,
                                   &فیلد_1_n_بایتها,
                                   یافت_any_selected_فیلد, true);

            reset_سطر:
              فیلد_idx = 1;
              current_rp = frp;
              یافت_any_selected_فیلد = false;
              have_pending_سطر = false;
              bytesearch_context_reset (&جستجو);
              write_فیلد = begin_فیلد_output (فیلد_idx, میانگیر_اولی_فیلد,
                                                &یافت_any_selected_فیلد);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  إذا (!have_pending_سطر)
    أرجع;

  پایان_current_سطر (فیلد_idx, میانگیر_اولی_فیلد, &فیلد_1_n_بایتها,
                       یافت_any_selected_فیلد, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

ساكن فراغ
cut_fields_ws (FILE *جریان)
{
  إذا (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (جریان);
  وإلا
    cut_fields_mb_any (جریان, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

ساكن bool
cut_file (حرف ثابت *file, فراغ (*cut_جریان) (FILE *))
{
  FILE *جریان;

  إذا (streq (file, "-"))
    {
      have_read_stdin = true;
      جریان = stdin;
      assume (جریان);  /* Pacify GCC bug#109613.  */
    }
  وإلا
    {
      جریان = fopen (file, "r");
      إذا (جریان == عدم)
        {
          error (0, errno, "%s", quotef (file));
          أرجع false;
        }
    }

  fadvise (جریان, FADVISE_SEQUENTIAL);

  cut_جریان (جریان);

  صحيح err = errno;
  إذا (!ferror (جریان))
    err = 0;
  إذا (streq (file, "-"))
    clearerr (جریان);		/* Also clear EOF.  */
  وإلا إذا (fclose (جریان) == EOF)
    err = errno;
  إذا (err)
    {
      error (0, err, "%s", quotef (file));
      أرجع false;
    }
  أرجع true;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  حرف *spec_list_string = عدم;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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
          cut_نما = CUT_نما_بایتها;
          FALLTHROUGH;
        حالة 'c':
          إذا (optc == 'c')
            cut_نما = CUT_نما_CHARACTERS;
          FALLTHROUGH;
        حالة 'f':
        حالة 'F':
          إذا (optc == 'f' || optc == 'F')
            cut_نما = CUT_نما_FIELDS;
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
            copy_بایتها (delim_بایتها, optarg, g.len + 1);
            delim_specified = true;
            اكسر;
          }

        حالة 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_آرگومانها,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          اكسر;

        حالة 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_درازی = (optarg[0] == '\0'
                                     ? 1 : طول_رشته (optarg));
          output_delimiter_string = optarg;
          اكسر;

        حالة 'n':
          no_تقسیم = true;
          اكسر;

        حالة 's':
          suppress_non_delimited = true;
          اكسر;

        حالة 'z':
          سطر_delim = '\0';
          اكسر;

        حالة COMPLEMENT_OPTION:
          complement = true;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  إذا (cut_نما == CUT_نما_بایتها || cut_نما == CUT_نما_CHARACTERS)
    {
      إذا (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an درون_داد delimiter makes sense\n\
\tonly when operating on fields"));

      إذا (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited سطرها makes sense\n\
\tonly when operating on fields"));
    }

  إذا (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  إذا (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  مجموعه_fields (spec_list_string,
              (((cut_نما == CUT_نما_بایتها
                 || cut_نما == CUT_نما_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  إذا (!delim_specified)
    {
      delim_بایتها[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  إذا (output_delimiter_string == عدم)
    {
      output_delimiter_string = output_delimiter_default;
      إذا (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_درازی = 1;
        }
      وإلا
        {
          copy_بایتها (output_delimiter_default, delim_بایتها, delim_mcel.len);
          output_delimiter_درازی = delim_mcel.len;
        }
    }

  فراغ (*cut_جریان) (FILE *) = عدم;
  بدل (cut_نما)
    {
    حالة CUT_نما_بایتها:
      cut_جریان = MB_CUR_MAX <= 1 || !no_تقسیم
                   ? cut_بایتها : cut_بایتها_no_تقسیم;
      اكسر;

    حالة CUT_نما_CHARACTERS:
      cut_جریان = MB_CUR_MAX <= 1 ? cut_بایتها : cut_characters;
      اكسر;

    حالة CUT_نما_FIELDS:
      cut_جریان = whitespace_delimited ? cut_fields_ws
                   : bytesearch_فیلد_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      اكسر;

    حالة CUT_نما_NONE:
      affirm (false);
    }
  affirm (cut_جریان);
  إذا (optind == argc)
    ok = cut_file ("-", cut_جریان);
  وإلا
    لكل (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_جریان);


  إذا (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
