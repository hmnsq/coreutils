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
#تعریف_کرو پروگرام_NAME "cut"

#تعریف_کرو AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#تعریف_کرو FATAL_ERROR(سنیہا)						\
  افعل									\
    {									\
      error (0, 0, (سنیہا));						\
      usage (EXIT_FAILURE);						\
    }									\
  طالما (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#تعریف_کرو SMALL_BYTE_حد 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
ساكن بنية میدان_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
ساكن حرف *میدان_1_بفر;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
ساكن idx_t میدان_1_bufsize;

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
ساكن حرف delim_بائٹاں[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
ساكن بنانشانی حرف سطر_delim = '\n';

/* The length of output_delimiter_string.  */
ساكن size_t output_delimiter_لمائی;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
ساكن حرف *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
ساكن حرف output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
ساكن bool no_ونڈو;

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

ساكن حرف ثابت *ثابت whitespace_option_دلیلاں[] =
{
  "trimmed", عدم
};

ساكن تعداد whitespace_option ثابت whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_دلیلاں, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
ساكن تعداد
{
  CUT_منوال_NONE,
  CUT_منوال_بائٹاں,
  CUT_منوال_CHARACTERS,
  CUT_منوال_FIELDS
} cut_منوال = CUT_منوال_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"bytes", required_دلیل, عدم, 'b'},
  {"characters", required_دلیل, عدم, 'c'},
  {"fields", required_دلیل, عدم, 'f'},
  {"delimiter", required_دلیل, عدم, 'd'},
  {"no-partial", no_دلیل, عدم, 'n'},
  {"whitespace-delimited", optional_دلیل, عدم, 'w'},
  {"only-delimited", no_دلیل, عدم, 's'},
  {"output-delimiter", required_دلیل, عدم, 'O'},
  {"complement", no_دلیل, عدم, COMPLEMENT_OPTION},
  {"zero-terminated", no_دلیل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Print selected parts دا سطریں from each FILE منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --بائٹاں=LIST\n\
         select only these byte positions\n\
"));
      oputs (_("\
  -c, --characters=LIST\n\
         select only these character positions\n\
"));
      oputs (_("\
      --complement\n\
         complement the سیٹ_2 دا selected بائٹاں, characters or fields\n\
"));
      oputs (_("\
  -d, --delimiter=DELIM\n\
         use DELIM instead دا TAB لكل میدان delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any سطر that شامل_اے\n\
         no delimiter character, unless the -s option is specified\n\
"));
      oputs (_("\
  -F LIST\n\
         like -f, but also سٹا_دیندا -w and -O ' '\n\
"));
      oputs (_("\
  -n, --no-partial\n\
         with -b, don't output partial multi-byte characters\n\
"));
      oputs (_("\
  -O, --output-delimiter=STRING\n\
         use STRING as the output delimiter;\n\
         the افتراضي is منزل use the داخلہ delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         افعل not print سطریں not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a لگاتار_2 دا blank characters as the میدان delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one دا -b, -c, -f or -F.  Each LIST is made اتے دا one\n\
range, or many ranges separated by commas.  Selected داخلہ is written\n\
وچ the same ترتیب that it is اقرأ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one دا:\n\
\n\
  N     N'th byte, character or میدان, counted from 1\n\
  N-    from N'th byte, character or میدان, منزل سرا دا سطر\n\
  N-M   from N'th to M'th (included) byte, character or میدان\n\
  -M    from پہلا منزل M'th (included) byte, character or میدان\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

ساكن مضمن فراغ
اگلا_شے (uintmax_t *شے_idx)
{
  (*شے_idx)++;
  إذا ((*شے_idx) > current_rp->hi)
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
is_range_شروع_اشاریہ (uintmax_t k)
{
  أرجع k == current_rp->lo;
}

ساكن مضمن bool
میدان_delim_is_سطر_delim (فراغ)
{
  أرجع delim_mcel.len == 1 && delim_بائٹاں[0] == سطر_delim;
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
bytesearch_میدان_delim_ok (فراغ)
{
  بنانشانی حرف delim_0 = delim_بائٹاں[0];

  أرجع (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


تعداد میدان_terminator
{
  میدان_انکڑے,
  میدان_DELIMITER,
  میدان_سطر_DELIMITER,
  میدان_EOF
};

تعداد bytesearch_منوال
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_سطر_ONLY
};

بنية bytesearch_context
{
  تعداد bytesearch_منوال منوال_2;
  bool blank_delimited;
  bool at_eof;
  حرف *سطر_سرا;
  bool سطر_سرا_معلوم;
};

ساكن مضمن فراغ
bytesearch_context_reset (بنية bytesearch_context *ctx)
{
  ctx->منوال_2 = BYTESEARCH_FIELDS;
  ctx->سطر_سرا = عدم;
  ctx->سطر_سرا_معلوم = false;
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

ساكن مضمن تعداد میدان_terminator
skip_whitespace_لگاتار (mbbuf_t *mbuf, بنية mbfield_parser *parser,
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
      أرجع g.ch == MBBUF_EOF ? میدان_EOF : میدان_سطر_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  أرجع trim_شروع && !have_initial_whitespace ? میدان_انکڑے : میدان_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

ساكن فراغ
write_بائٹاں (حرف ثابت *buf, size_t n_بائٹاں)
{
  إذا (n_بائٹاں <= SMALL_BYTE_حد)
    {
      لكل (size_t i = 0; i < n_بائٹاں; i++)
        إذا (حرف_لکھو (buf[i]) < 0)
          write_error ();
      أرجع;
    }

  إذا (fwrite (buf, حجم (حرف), n_بائٹاں, stdout) != n_بائٹاں)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

ساكن مضمن فراغ
copy_بائٹاں (حرف *dst, حرف ثابت *src, size_t n_بائٹاں)
{
  إذا (n_بائٹاں <= SMALL_BYTE_حد)
    {
      لكل (size_t i = 0; i < n_بائٹاں; i++)
        dst[i] = src[i];
      أرجع;
    }

  حافظہ_نقل (dst, src, n_بائٹاں);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

ساكن مضمن فراغ*
تلاش_بائٹاں (حرف ثابت *buf, صحيح c, size_t n_بائٹاں)
{
  إذا (n_بائٹاں != 0 && منزل_uchar (buf[0]) == c)
    أرجع (حرف *) buf;
  إذا (1 < n_بائٹاں && منزل_uchar (buf[1]) == c)
    أرجع (حرف *) buf + 1;

  أرجع 2 < n_بائٹاں ? (حرف *) memchr (buf + 2, c, n_بائٹاں - 2) : عدم;
}

ساكن مضمن فراغ
write_سطر_delim (فراغ)
{
  إذا (حرف_لکھو (سطر_delim) < 0)
    write_error ();
}

ساكن مضمن فراغ
reset_شے_سطر (uintmax_t *شے_idx, bool *print_delimiter)
{
  write_سطر_delim ();
  *شے_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

ساكن مضمن فراغ
write_pending_سطر_delim (uintmax_t شے_idx)
{
  إذا (شے_idx > 0)
    write_سطر_delim ();
}

ساكن مضمن فراغ
write_selected_شے (bool *print_delimiter, bool range_شروع,
                     حرف ثابت *buf, size_t n_بائٹاں)
{
  إذا (output_delimiter_string != output_delimiter_default)
    {
      إذا (*print_delimiter && range_شروع)
        write_بائٹاں (output_delimiter_string, output_delimiter_لمائی);
      *print_delimiter = true;
    }

  write_بائٹاں (buf, n_بائٹاں);
}

ساكن مضمن فراغ
append_میدان_1_chunk (حرف ثابت *buf, idx_t len, idx_t *n_بائٹاں)
{
  إذا (میدان_1_bufsize - *n_بائٹاں < len)
    {
      میدان_1_بفر = xpalloc (میدان_1_بفر, &میدان_1_bufsize,
                                len, -1, حجم *میدان_1_بفر);
    }

  copy_بائٹاں (میدان_1_بفر + *n_بائٹاں, buf, len);
  *n_بائٹاں += len;
}

ساكن مضمن فراغ
append_میدان_1_بائٹاں (mbbuf_t *mbbuf, mcel_t g, idx_t *n_بائٹاں)
{
  append_میدان_1_chunk (mbbuf_char_وتھ (mbbuf, g), g.len, n_بائٹاں);
}

ساكن مضمن تعداد میدان_terminator
جانچ_mb_blank_میدان (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
                     bool *have_pending_سطر, bool write_میدان,
                     idx_t *n_بائٹاں)
{
  إذا (parser->trim_outer_whitespace && parser->at_سطر_شروع)
    {
      تعداد میدان_terminator terminator
        = skip_whitespace_لگاتار (mbbuf, parser, have_pending_سطر, false);
      إذا (terminator != میدان_انکڑے)
        أرجع terminator;
    }

  parser->at_سطر_شروع = false;

  طالما (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      إذا (g.ch == MBBUF_EOF)
        أرجع میدان_EOF;

      *have_pending_سطر = true;

      إذا (g.ch == سطر_delim)
        أرجع میدان_سطر_DELIMITER;

      إذا (mcel_isblank (g))
        أرجع skip_whitespace_لگاتار (mbbuf, parser, have_pending_سطر, true);

      إذا (n_بائٹاں)
        append_میدان_1_بائٹاں (mbbuf, g, n_بائٹاں);
      وإلا إذا (write_میدان)
        write_بائٹاں (mbbuf_char_وتھ (mbbuf, g), g.len);
    }
}

ساكن مضمن تعداد میدان_terminator
جانچ_mb_delim_میدان (mbbuf_t *mbbuf, bool *have_pending_سطر,
                     bool write_میدان, idx_t *n_بائٹاں)
{
  طالما (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      إذا (g.ch == MBBUF_EOF)
        أرجع میدان_EOF;

      *have_pending_سطر = true;

      إذا (g.ch == سطر_delim)
        أرجع میدان_سطر_DELIMITER;

      إذا (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        أرجع میدان_DELIMITER;

      إذا (n_بائٹاں)
        append_میدان_1_بائٹاں (mbbuf, g, n_بائٹاں);
      وإلا إذا (write_میدان)
        write_بائٹاں (mbbuf_char_وتھ (mbbuf, g), g.len);
    }
}

ساكن مضمن تعداد میدان_terminator
جانچ_mb_میدان (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
               bool *have_pending_سطر, bool write_میدان, idx_t *n_بائٹاں)
{
  أرجع (parser->whitespace_delimited
          ? جانچ_mb_blank_میدان (mbbuf, parser, have_pending_سطر,
                                 write_میدان, n_بائٹاں)
          : جانچ_mb_delim_میدان (mbbuf, have_pending_سطر, write_میدان,
                                 n_بائٹاں));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

ساكن تعداد میدان_terminator
جانچ_mb_سطر_سرا (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
                  bool *have_pending_سطر)
{
  إذا (parser->have_saved)
    {
      parser->have_saved = false;
      إذا (parser->saved_g.ch == MBBUF_EOF)
        أرجع میدان_EOF;

      *have_pending_سطر = true;
      إذا (parser->saved_g.ch == سطر_delim)
        أرجع میدان_سطر_DELIMITER;
    }

  طالما (true)
    {
      idx_t دستیاب = mbbuf_topup (mbbuf);
      إذا (دستیاب == 0)
        أرجع میدان_EOF;

      حرف *بفر = mbbuf->بفر + mbbuf->وتھ;
      حرف *سطر_سرا = تلاش_بائٹاں (بفر, سطر_delim, دستیاب);
      idx_t n = سطر_سرا ? سطر_سرا - بفر + 1 : دستیاب;
      mbbuf_advance (mbbuf, n);
      *have_pending_سطر = true;

      إذا (سطر_سرا)
        أرجع میدان_سطر_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_خالص
ساكن حرف *
لبھو_میدان_delim (حرف *buf, size_t len)
{
  إذا (len < delim_mcel.len)
    أرجع عدم;

#إذا ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  أرجع memmem (buf, len, delim_بائٹاں, delim_mcel.len);
#وإلا
  إذا (delim_mcel.len == 1)
    أرجع memchr (buf, delim_بائٹاں[0], len);

  حرف ثابت *p = buf;
  حرف ثابت *سرا = buf + len;

  حرف saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  طالما (p < سرا)
    {
      حرف ثابت *nul = memchr (p, '\0', سرا - p);
      إذا (!nul)
        {
          حرف ثابت *میل = لڑی_کھوج (p, delim_بائٹاں);
          buf[len] = saved;
          أرجع (حرف *) میل;
        }

      إذا (p < nul)
        {
          حرف ثابت *میل = لڑی_کھوج (p, delim_بائٹاں);
          إذا (میل)
            {
              buf[len] = saved;
              أرجع (حرف *) میل;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  أرجع عدم;
#جے_انت
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_خالص
ساكن حرف *
لبھو_c_blank (حرف *buf, size_t len)
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
میدان_delim_overlap (حرف ثابت *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  طالما (0 < overlap)
    {
      إذا (memeq (buf + len - overlap, delim_بائٹاں, overlap))
        أرجع overlap;
      overlap--;
    }

  أرجع 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

ساكن مضمن تعداد میدان_terminator
لبھو_میدان_terminator (حرف *buf, idx_t len,
                       بنية bytesearch_context *ctx, حرف **terminator)
{
  إذا (ctx->منوال_2 == BYTESEARCH_سطر_ONLY)
    {
      إذا (!ctx->سطر_سرا_معلوم)
        {
          ctx->سطر_سرا = memchr (buf, سطر_delim, len);
          ctx->سطر_سرا_معلوم = true;
        }

      *terminator = ctx->سطر_سرا;
      أرجع *terminator ? میدان_سطر_DELIMITER : میدان_انکڑے;
    }

  إذا (میدان_delim_is_سطر_delim ())
    {
      *terminator = memchr (buf, سطر_delim, len);
      أرجع *terminator ? میدان_DELIMITER : میدان_انکڑے;
    }

  إذا (!ctx->سطر_سرا_معلوم)
    {
      ctx->سطر_سرا = memchr (buf, سطر_delim, len);
      ctx->سطر_سرا_معلوم = true;
    }

  idx_t میدان_len = ctx->سطر_سرا ? ctx->سطر_سرا - buf : len;

  حرف *میدان_سرا = (ctx->blank_delimited
                     ? لبھو_c_blank (buf, میدان_len)
                     : لبھو_میدان_delim (buf, میدان_len));

  إذا (میدان_سرا)
    {
      *terminator = میدان_سرا;
      أرجع میدان_DELIMITER;
    }

  *terminator = ctx->سطر_سرا;
  أرجع ctx->سطر_سرا ? میدان_سطر_DELIMITER : میدان_انکڑے;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

ساكن مضمن فراغ
maybe_write_سطر_delim (bool لبھیا_any_selected_میدان, uintmax_t میدان_idx)
{
  إذا (لبھیا_any_selected_میدان
      || !(suppress_non_delimited && میدان_idx == 1))
    write_سطر_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

ساكن مضمن bool
begin_میدان_output (uintmax_t میدان_idx, bool بفر_پہلا_میدان,
                    bool *لبھیا_any_selected_میدان)
{
  bool write_میدان = print_kth (میدان_idx);

  إذا (write_میدان && ! (میدان_idx == 1 && بفر_پہلا_میدان))
    {
      إذا (*لبھیا_any_selected_میدان)
        write_بائٹاں (output_delimiter_string, output_delimiter_لمائی);
      *لبھیا_any_selected_میدان = true;
    }

  أرجع write_میدان;
}

ساكن مضمن bool
handle_میدان_1 (uintmax_t میدان_idx, bool بفر_پہلا_میدان,
                idx_t *میدان_1_n_بائٹاں)
{
  bool میدان_1_selected = false;

  إذا (میدان_idx == 1 && بفر_پہلا_میدان)
    {
      إذا ((میدان_1_selected = print_kth (1)))
        write_بائٹاں (میدان_1_بفر, *میدان_1_n_بائٹاں);
      *میدان_1_n_بائٹاں = 0;
    }

  أرجع میدان_1_selected;
}

ساكن مضمن فراغ
handle_میدان_delimiter (uintmax_t *میدان_idx, bool بفر_پہلا_میدان,
                        idx_t *میدان_1_n_بائٹاں,
                        bool *لبھیا_any_selected_میدان, bool *write_میدان)
{
  إذا (handle_میدان_1 (*میدان_idx, بفر_پہلا_میدان, میدان_1_n_بائٹاں))
    *لبھیا_any_selected_میدان = true;

  اگلا_شے (میدان_idx);
  *write_میدان = begin_میدان_output (*میدان_idx, بفر_پہلا_میدان,
                                     لبھیا_any_selected_میدان);
}

ساكن مضمن bool
میدان_selection_exhausted (uintmax_t میدان_idx)
{
  أرجع !print_kth (میدان_idx) && current_rp->lo == UINTMAX_MAX;
}

ساكن مضمن فراغ
انت_current_سطر (uintmax_t میدان_idx, bool بفر_پہلا_میدان,
                     idx_t *میدان_1_n_بائٹاں,
                     bool لبھیا_any_selected_میدان, bool سطر_terminated)
{
  إذا (میدان_idx == 1 && بفر_پہلا_میدان)
    {
      إذا (!suppress_non_delimited
          && (سطر_terminated || *میدان_1_n_بائٹاں != 0))
        {
          write_بائٹاں (میدان_1_بفر, *میدان_1_n_بائٹاں);
          write_سطر_delim ();
        }
      *میدان_1_n_بائٹاں = 0;
    }
  وإلا
    maybe_write_سطر_delim (لبھیا_any_selected_میدان, میدان_idx);
}

ساكن مضمن فراغ
sync_byte_selection (uintmax_t byte_idx)
{
  طالما (current_rp->hi <= byte_idx)
    current_rp++;
}

ساكن مضمن فراغ
reset_میدان_سطر (uintmax_t *میدان_idx, bool *لبھیا_any_selected_میدان,
                  bool *have_pending_سطر, بنية mbfield_parser *parser)
{
  *میدان_idx = 1;
  current_rp = frp;
  *لبھیا_any_selected_میدان = false;
  *have_pending_سطر = false;
  parser->have_saved = false;
  parser->at_سطر_شروع = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

ساكن فراغ
cut_بائٹاں (FILE *دھارا)
{
  ساكن حرف بائٹاں_وچ[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  صحيح fd = fileno (دھارا);

  current_rp = frp;

  طالما (true)
    {
      idx_t دستیاب = اقرأ (fd, بائٹاں_وچ, حجم بائٹاں_وچ);
      إذا (دستیاب <= 0)
        {
          إذا (دستیاب < 0)
            fseterr (دھارا);
          write_pending_سطر_delim (byte_idx);
          اكسر;
        }

      idx_t processed = 0;

      طالما (processed < دستیاب)
        {
          حرف *سطر = بائٹاں_وچ + processed;
          idx_t منزل_process = دستیاب - processed;
          حرف *سطر_سرا = تلاش_بائٹاں (سطر, سطر_delim, منزل_process);
          حرف *سرا = سطر + (سطر_سرا ? سطر_سرا - سطر : منزل_process);
          حرف *p = سطر;

          طالما (0 < سرا - p)
            {
              sync_byte_selection (byte_idx);

              إذا (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (سرا - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              وإلا
                {
                  idx_t n = MIN (سرا - p, current_rp->hi - byte_idx);
                  write_selected_شے (&print_delimiter,
                                       is_range_شروع_اشاریہ (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += سرا - سطر;
          إذا (سطر_سرا)
            {
              processed++;
              reset_شے_سطر (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

ساكن فراغ
cut_characters_منوال (FILE *دھارا, bool byte_منوال)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  ساكن حرف بائٹاں_وچ[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, بائٹاں_وچ, حجم بائٹاں_وچ, دھارا);

  طالما (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      إذا (idx && میدان_selection_exhausted (idx))
        {
          idx_t دستیاب = mbbuf_topup (&mbbuf);
          حرف *p = mbbuf.بفر + mbbuf.وتھ;
          حرف *سرا = تلاش_بائٹاں (p, سطر_delim, دستیاب);
          mbbuf_advance (&mbbuf, سرا ? سرا - p : دستیاب);
          إذا (!سرا && دستیاب)
            استمر;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      إذا (!byte_منوال && utf8 && idx + 1 < current_rp->lo)
        {
          حرف ثابت *p = mbbuf.بفر + mbbuf.وتھ;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          حرف ثابت *سرا = تلاش_بائٹاں (p, سطر_delim, n);
          إذا (سرا)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, سرا - p + 1);
              reset_شے_سطر (&idx, &print_delimiter);
              استمر;
            }

          idx += u8_گنتی (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      إذا (g.ch == سطر_delim)
        reset_شے_سطر (&idx, &print_delimiter);
      وإلا إذا (g.ch == MBBUF_EOF)
        {
          write_pending_سطر_delim (idx);
          اكسر;
        }
      وإلا إذا (byte_منوال)
        {
          bool پہلا_selected_is_range_شروع = false;
          bool تکیا_selected = false;
          bool suffix_selected = true;

          لكل (idx_t i = 0; i < g.len; i++)
            {
              اگلا_شے (&idx);
              إذا (print_kth (idx))
                {
                  إذا (!تکیا_selected)
                    {
                      تکیا_selected = true;
                      پہلا_selected_is_range_شروع
                        = is_range_شروع_اشاریہ (idx);
                    }
                }
              وإلا إذا (تکیا_selected)
                suffix_selected = false;
            }

          إذا (تکیا_selected && suffix_selected)
            write_selected_شے (&print_delimiter,پہلا_selected_is_range_شروع,
                                 mbbuf_char_وتھ (&mbbuf, g), g.len);
        }
      وإلا
        {
          اگلا_شے (&idx);
          إذا (print_kth (idx))
            write_selected_شے (&print_delimiter,
                                 is_range_شروع_اشاریہ (idx),
                                 mbbuf_char_وتھ (&mbbuf, g), g.len);
        }
    }
}

ساكن فراغ
cut_بائٹاں_no_ونڈو (FILE *دھارا)
{
  cut_characters_منوال (دھارا, true);
}

ساكن فراغ
cut_characters (FILE *دھارا)
{
  cut_characters_منوال (دھارا, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

ساكن فراغ
cut_fields_mb_any (FILE *دھارا, bool whitespace_منوال)
{
  ساكن حرف بائٹاں_وچ[IO_BUFSIZE];
  mbbuf_t mbbuf;
  بنية mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_منوال,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_سطر_شروع = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t میدان_idx = 1;
  bool لبھیا_any_selected_میدان = false;
  bool بفر_پہلا_میدان;
  bool have_pending_سطر = false;
  bool write_میدان;
  idx_t میدان_1_n_بائٹاں = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, بائٹاں_وچ, حجم بائٹاں_وچ, دھارا);

  بفر_پہلا_میدان = (suppress_non_delimited ^ !print_kth (1));
  write_میدان = begin_میدان_output (میدان_idx, بفر_پہلا_میدان,
                                    &لبھیا_any_selected_میدان);

  طالما (true)
    {
      تعداد میدان_terminator terminator;
      إذا (!بفر_پہلا_میدان || میدان_idx != 1)
        terminator = جانچ_mb_میدان (&mbbuf, &parser, &have_pending_سطر,
                                    write_میدان, عدم);
      وإلا
        terminator = جانچ_mb_میدان (&mbbuf, &parser, &have_pending_سطر,
                                    false, &میدان_1_n_بائٹاں);

      إذا (terminator == میدان_DELIMITER)
        {
          handle_میدان_delimiter (&میدان_idx, بفر_پہلا_میدان,
                                  &میدان_1_n_بائٹاں,
                                  &لبھیا_any_selected_میدان, &write_میدان);
          إذا (! میدان_selection_exhausted (میدان_idx))
            استمر;

          terminator = جانچ_mb_سطر_سرا (&mbbuf, &parser,
                                         &have_pending_سطر);
        }

      إذا (terminator == میدان_EOF && !have_pending_سطر)
        اكسر;

      انت_current_سطر (میدان_idx, بفر_پہلا_میدان,
                           &میدان_1_n_بائٹاں, لبھیا_any_selected_میدان,
                           terminator == میدان_سطر_DELIMITER);
      إذا (terminator == میدان_EOF)
        اكسر;

      reset_میدان_سطر (&میدان_idx, &لبھیا_any_selected_میدان,
                        &have_pending_سطر, &parser);
      write_میدان = begin_میدان_output (میدان_idx, بفر_پہلا_میدان,
                                        &لبھیا_any_selected_میدان);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

ساكن فراغ
cut_fields_mb (FILE *دھارا)
{
  cut_fields_mb_any (دھارا, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

ساكن فراغ
cut_fields_bytesearch (FILE *دھارا)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  ساكن حرف بائٹاں_وچ[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t میدان_idx = 1;
  bool لبھیا_any_selected_میدان = false;
  bool have_pending_سطر = false;
  bool skip_blank_لگاتار = false;
  bool pending_سطر_delim_میدان = false;
  bool write_میدان;
  idx_t میدان_1_n_بائٹاں = 0;

  current_rp = frp;
  bool بفر_پہلا_میدان = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, بائٹاں_وچ, حجم بائٹاں_وچ - 1, دھارا);
  write_میدان = begin_میدان_output (میدان_idx, بفر_پہلا_میدان,
                                    &لبھیا_any_selected_میدان);

  بنية bytesearch_context تلاش = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&تلاش);

  طالما (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      تلاش.at_eof = mbbuf.eof;
      تلاش.سطر_سرا_معلوم = false;

      إذا (pending_سطر_delim_میدان)
        {
          pending_سطر_delim_میدان = false;

          إذا (n_avail == 0)
            {
              إذا (handle_میدان_1 (میدان_idx, بفر_پہلا_میدان,
                                  &میدان_1_n_بائٹاں))
                اگلا_شے (&میدان_idx);
              maybe_write_سطر_delim (لبھیا_any_selected_میدان, میدان_idx);
              أرجع;
            }

          handle_میدان_delimiter (&میدان_idx, بفر_پہلا_میدان,
                                  &میدان_1_n_بائٹاں,
                                  &لبھیا_any_selected_میدان, &write_میدان);
          إذا (whitespace_delimited)
            skip_blank_لگاتار = true;
        }

      إذا (n_avail == 0)
        اكسر;

      حرف *chunk = mbbuf.بفر + mbbuf.وتھ;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      إذا (میدان_idx == 1
          && (!whitespace_delimited || !میدان_selection_exhausted (میدان_idx))
          && !میدان_delim_is_سطر_delim ()
          && !(whitespace_delimited
               ? لبھو_c_blank (chunk, n_avail)
               : لبھو_میدان_delim (chunk, n_avail)))
        {
          حرف *آخری_سطر_delim = تلاش.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, سطر_delim, n_avail);
          إذا (آخری_سطر_delim)
            {
              idx_t n = آخری_سطر_delim - chunk + 1;
              إذا (! suppress_non_delimited)
                {
                  write_بائٹاں (میدان_1_بفر, میدان_1_n_بائٹاں);
                  write_بائٹاں (chunk, n);
                  إذا (تلاش.at_eof && chunk[n - 1] != سطر_delim)
                    write_سطر_delim ();
                }
              میدان_1_n_بائٹاں = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_سطر = false;
              إذا (تلاش.at_eof)
                أرجع;
              استمر;
            }
        }

      طالما (processed < n_avail)
        {
          حرف *terminator = عدم;

          إذا (skip_blank_لگاتار)
            {
              طالما (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              إذا (processed == n_avail)
                اكسر;
              skip_blank_لگاتار = false;
            }

          إذا (میدان_selection_exhausted (میدان_idx))
            {
              إذا (میدان_delim_is_سطر_delim ())
                {
                  maybe_write_سطر_delim (لبھیا_any_selected_میدان,
                                          میدان_idx);
                  mbbuf_advance (&mbbuf, processed);
                  أرجع;
                }
              تلاش.منوال_2 = BYTESEARCH_سطر_ONLY;
              تعداد میدان_terminator terminator_قسم
                = لبھو_میدان_terminator (chunk + processed, n_avail - processed,
                                         &تلاش, &terminator);
              إذا (terminator_قسم == میدان_سطر_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_سطر_delim (لبھیا_any_selected_میدان,
                                          میدان_idx);
                  اذهب reset_سطر;
                }
              processed = n_avail;
              اكسر;
            }

          تعداد میدان_terminator terminator_قسم
            = لبھو_میدان_terminator (chunk + processed, n_avail - processed,
                                     &تلاش, &terminator);
          idx_t میدان_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          إذا (terminator_قسم == میدان_انکڑے
              && !تلاش.at_eof
              && !whitespace_delimited
              && !میدان_delim_is_سطر_delim ())
            میدان_len -= میدان_delim_overlap (chunk + processed, میدان_len);

          إذا (میدان_len || terminator)
            have_pending_سطر = true;

          /* Output field content.  */
          إذا (میدان_idx == 1 && بفر_پہلا_میدان)
            append_میدان_1_chunk (chunk + processed, میدان_len,
                                  &میدان_1_n_بائٹاں);
          وإلا إذا (write_میدان)
            write_بائٹاں (chunk + processed, میدان_len);
          processed += میدان_len;

          إذا (terminator_قسم == میدان_انکڑے)
            اكسر;

          إذا (terminator_قسم == میدان_DELIMITER)
            {
              إذا (میدان_delim_is_سطر_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_سطر_delim_میدان = true;
                  اكسر;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_میدان_delimiter (&میدان_idx, بفر_پہلا_میدان,
                                      &میدان_1_n_بائٹاں,
                                      &لبھیا_any_selected_میدان, &write_میدان);
              إذا (whitespace_delimited)
                skip_blank_لگاتار = true;
            }
          وإلا
            {
              affirm (terminator_قسم == میدان_سطر_DELIMITER);
              processed++;
              انت_current_سطر (میدان_idx, بفر_پہلا_میدان,
                                   &میدان_1_n_بائٹاں,
                                   لبھیا_any_selected_میدان, true);

            reset_سطر:
              میدان_idx = 1;
              current_rp = frp;
              لبھیا_any_selected_میدان = false;
              have_pending_سطر = false;
              bytesearch_context_reset (&تلاش);
              write_میدان = begin_میدان_output (میدان_idx, بفر_پہلا_میدان,
                                                &لبھیا_any_selected_میدان);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  إذا (!have_pending_سطر)
    أرجع;

  انت_current_سطر (میدان_idx, بفر_پہلا_میدان, &میدان_1_n_بائٹاں,
                       لبھیا_any_selected_میدان, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

ساكن فراغ
cut_fields_ws (FILE *دھارا)
{
  إذا (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (دھارا);
  وإلا
    cut_fields_mb_any (دھارا, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

ساكن bool
cut_file (حرف ثابت *file, فراغ (*cut_دھارا) (FILE *))
{
  FILE *دھارا;

  إذا (streq (file, "-"))
    {
      have_read_stdin = true;
      دھارا = stdin;
      assume (دھارا);  /* Pacify GCC bug#109613.  */
    }
  وإلا
    {
      دھارا = fopen (file, "r");
      إذا (دھارا == عدم)
        {
          error (0, errno, "%s", quotef (file));
          أرجع false;
        }
    }

  fadvise (دھارا, FADVISE_SEQUENTIAL);

  cut_دھارا (دھارا);

  صحيح err = errno;
  إذا (!ferror (دھارا))
    err = 0;
  إذا (streq (file, "-"))
    clearerr (دھارا);		/* Also clear EOF.  */
  وإلا إذا (fclose (دھارا) == EOF)
    err = errno;
  إذا (err)
    {
      error (0, err, "%s", quotef (file));
      أرجع false;
    }
  أرجع true;
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  صحيح optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  حرف *spec_list_string = عدم;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
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
          cut_منوال = CUT_منوال_بائٹاں;
          FALLTHROUGH;
        حالة 'c':
          إذا (optc == 'c')
            cut_منوال = CUT_منوال_CHARACTERS;
          FALLTHROUGH;
        حالة 'f':
        حالة 'F':
          إذا (optc == 'f' || optc == 'F')
            cut_منوال = CUT_منوال_FIELDS;
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
            copy_بائٹاں (delim_بائٹاں, optarg, g.len + 1);
            delim_specified = true;
            اكسر;
          }

        حالة 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_دلیلاں,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          اكسر;

        حالة 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_لمائی = (optarg[0] == '\0'
                                     ? 1 : لڑی_لمبائی (optarg));
          output_delimiter_string = optarg;
          اكسر;

        حالة 'n':
          no_ونڈو = true;
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
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  إذا (cut_منوال == CUT_منوال_بائٹاں || cut_منوال == CUT_منوال_CHARACTERS)
    {
      إذا (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an داخلہ delimiter makes sense\n\
\tonly when operating on fields"));

      إذا (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited سطریں makes sense\n\
\tonly when operating on fields"));
    }

  إذا (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  إذا (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  سیٹ_fields (spec_list_string,
              (((cut_منوال == CUT_منوال_بائٹاں
                 || cut_منوال == CUT_منوال_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  إذا (!delim_specified)
    {
      delim_بائٹاں[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  إذا (output_delimiter_string == عدم)
    {
      output_delimiter_string = output_delimiter_default;
      إذا (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_لمائی = 1;
        }
      وإلا
        {
          copy_بائٹاں (output_delimiter_default, delim_بائٹاں, delim_mcel.len);
          output_delimiter_لمائی = delim_mcel.len;
        }
    }

  فراغ (*cut_دھارا) (FILE *) = عدم;
  بدل (cut_منوال)
    {
    حالة CUT_منوال_بائٹاں:
      cut_دھارا = MB_CUR_MAX <= 1 || !no_ونڈو
                   ? cut_بائٹاں : cut_بائٹاں_no_ونڈو;
      اكسر;

    حالة CUT_منوال_CHARACTERS:
      cut_دھارا = MB_CUR_MAX <= 1 ? cut_بائٹاں : cut_characters;
      اكسر;

    حالة CUT_منوال_FIELDS:
      cut_دھارا = whitespace_delimited ? cut_fields_ws
                   : bytesearch_میدان_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      اكسر;

    حالة CUT_منوال_NONE:
      affirm (false);
    }
  affirm (cut_دھارا);
  إذا (optind == argc)
    ok = cut_file ("-", cut_دھارا);
  وإلا
    لكل (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_دھارا);


  إذا (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
