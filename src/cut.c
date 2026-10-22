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

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل "system.h"

#شامل "argmatch.h"
#شامل "assure.h"
#شامل "c-ctype.h"
#شامل "fadvise.h"
#شامل "ioblksize.h"
#شامل "mbbuf.h"
#شامل "memchr2.h"

#شامل "set-fields.h"
#شامل "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "cut"

#تعریف_کرو AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#تعریف_کرو FATAL_ERROR(پیغام)						\
  کرو									\
    {									\
      error (0, 0, (پیغام));						\
      usage (EXIT_FAILURE);						\
    }									\
  جب (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#تعریف_کرو SMALL_BYTE_حد 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
ساکن ساخت میدان_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
ساکن حرف *میدان_1_بفر;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
ساکن idx_t میدان_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
ساکن bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
ساکن bool complement;

/* The delimiter character for multibyte field mode.  */
ساکن mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
ساکن حرف delim_بائٹیں[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
ساکن غیرعلامتی حرف سطر_delim = '\n';

/* The length of output_delimiter_string.  */
ساکن size_t output_delimiter_لمبائی;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
ساکن حرف *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
ساکن حرف output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
ساکن bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
ساکن bool no_بانٹو;

/* If true, interpret each run of whitespace as one field delimiter.  */
ساکن bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
ساکن bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
ساکن bool space_output_delimiter_default;

شمار whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

ساکن حرف ثابت *ثابت whitespace_option_دلائل[] =
{
  "trimmed", تہی
};

ساکن شمار whitespace_option ثابت whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_دلائل, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
ساکن شمار
{
  CUT_منوال_NONE,
  CUT_منوال_بائٹیں,
  CUT_منوال_CHARACTERS,
  CUT_منوال_FIELDS
} cut_منوال = CUT_منوال_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت longopts[] =
{
  {"bytes", required_دلیل, تہی, 'b'},
  {"characters", required_دلیل, تہی, 'c'},
  {"fields", required_دلیل, تہی, 'f'},
  {"delimiter", required_دلیل, تہی, 'd'},
  {"no-partial", no_دلیل, تہی, 'n'},
  {"whitespace-delimited", optional_دلیل, تہی, 'w'},
  {"only-delimited", no_دلیل, تہی, 's'},
  {"output-delimiter", required_دلیل, تہی, 'O'},
  {"complement", no_دلیل, تہی, COMPLEMENT_OPTION},
  {"zero-terminated", no_دلیل, تہی, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Print selected parts کا سطریں from each FILE مقصد standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --بائٹیں=LIST\n\
         select only these byte positions\n\
"));
      oputs (_("\
  -c, --characters=LIST\n\
         select only these character positions\n\
"));
      oputs (_("\
      --complement\n\
         complement the سیٹ_2 کا selected بائٹیں, characters or fields\n\
"));
      oputs (_("\
  -d, --delimiter=DELIM\n\
         use DELIM instead کا TAB برائے میدان delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any سطر that شامل_ہے\n\
         no delimiter character, unless the -s option is specified\n\
"));
      oputs (_("\
  -F LIST\n\
         like -f, but also مستلزم -w and -O ' '\n\
"));
      oputs (_("\
  -n, --no-partial\n\
         with -b, don't output partial multi-byte characters\n\
"));
      oputs (_("\
  -O, --output-delimiter=STRING\n\
         use STRING as the output delimiter;\n\
         the طےشدہ is مقصد use the داخلہ delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         کرو not print سطریں not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a تسلسل_2 کا blank characters as the میدان delimiter;\n\
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
Use one, and only one کا -b, -c, -f or -F.  Each LIST is made اوپر کا one\n\
range, or many ranges separated by commas.  Selected داخلہ is written\n\
اندر the same ترتیب that it is اقرأ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one کا:\n\
\n\
  N     N'th byte, character or میدان, counted from 1\n\
  N-    from N'th byte, character or میدان, مقصد انتہا کا سطر\n\
  N-M   from N'th to M'th (included) byte, character or میدان\n\
  -M    from پہلا مقصد M'th (included) byte, character or میدان\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

ساکن اندرونی خالی
اگلا_شے (uintmax_t *شے_idx)
{
  (*شے_idx)++;
  اگر ((*شے_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

ساکن اندرونی bool
print_kth (uintmax_t k)
{
  واپس current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

ساکن اندرونی bool
is_range_آغاز_اشاریہ (uintmax_t k)
{
  واپس k == current_rp->lo;
}

ساکن اندرونی bool
میدان_delim_is_سطر_delim (خالی)
{
  واپس delim_mcel.len == 1 && delim_بائٹیں[0] == سطر_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

ساکن bool
mcel_isblank (mcel_t g)
{
  واپس (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

ساکن اندرونی bool
bytesearch_میدان_delim_ok (خالی)
{
  غیرعلامتی حرف delim_0 = delim_بائٹیں[0];

  واپس (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


شمار میدان_terminator
{
  میدان_ڈیٹا,
  میدان_DELIMITER,
  میدان_سطر_DELIMITER,
  میدان_EOF
};

شمار bytesearch_منوال
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_سطر_ONLY
};

ساخت bytesearch_context
{
  شمار bytesearch_منوال منوال_2;
  bool blank_delimited;
  bool at_eof;
  حرف *سطر_انتہا;
  bool سطر_انتہا_معلوم;
};

ساکن اندرونی خالی
bytesearch_context_reset (ساخت bytesearch_context *ctx)
{
  ctx->منوال_2 = BYTESEARCH_FIELDS;
  ctx->سطر_انتہا = تہی;
  ctx->سطر_انتہا_معلوم = false;
}

ساخت mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_سطر_آغاز;
  bool have_saved;
  mcel_t saved_g;
};

ساکن اندرونی mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  اگر (*have_saved)
    {
      *have_saved = false;
      واپس *saved_g;
    }
  واپس mbbuf_get_char (mbbuf);
}

ساکن اندرونی شمار میدان_terminator
skip_whitespace_تسلسل (mbbuf_t *mbuf, ساخت mbfield_parser *parser,
                     bool *have_pending_سطر,
                     bool have_initial_whitespace)
{
  mcel_t g;

  کرو
    {
      g = mbbuf_get_char (mbuf);
      اگر (g.ch != MBBUF_EOF)
        *have_pending_سطر = true;
    }
  جب (g.ch != MBBUF_EOF && g.ch != سطر_delim && mcel_isblank (g));

  bool trim_آغاز = parser->trim_outer_whitespace && parser->at_سطر_آغاز;

  اگر (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == سطر_delim))
    {
      واپس g.ch == MBBUF_EOF ? میدان_EOF : میدان_سطر_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  واپس trim_آغاز && !have_initial_whitespace ? میدان_ڈیٹا : میدان_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

ساکن خالی
write_بائٹیں (حرف ثابت *buf, size_t n_بائٹیں)
{
  اگر (n_بائٹیں <= SMALL_BYTE_حد)
    {
      برائے (size_t i = 0; i < n_بائٹیں; i++)
        اگر (حرف_لکھو (buf[i]) < 0)
          write_error ();
      واپس;
    }

  اگر (fwrite (buf, حجم (حرف), n_بائٹیں, stdout) != n_بائٹیں)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

ساکن اندرونی خالی
copy_بائٹیں (حرف *dst, حرف ثابت *src, size_t n_بائٹیں)
{
  اگر (n_بائٹیں <= SMALL_BYTE_حد)
    {
      برائے (size_t i = 0; i < n_بائٹیں; i++)
        dst[i] = src[i];
      واپس;
    }

  حافظہ_نقل (dst, src, n_بائٹیں);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

ساکن اندرونی خالی*
تلاش_بائٹیں (حرف ثابت *buf, صحیح c, size_t n_بائٹیں)
{
  اگر (n_بائٹیں != 0 && مقصد_uchar (buf[0]) == c)
    واپس (حرف *) buf;
  اگر (1 < n_بائٹیں && مقصد_uchar (buf[1]) == c)
    واپس (حرف *) buf + 1;

  واپس 2 < n_بائٹیں ? (حرف *) memchr (buf + 2, c, n_بائٹیں - 2) : تہی;
}

ساکن اندرونی خالی
write_سطر_delim (خالی)
{
  اگر (حرف_لکھو (سطر_delim) < 0)
    write_error ();
}

ساکن اندرونی خالی
reset_شے_سطر (uintmax_t *شے_idx, bool *print_delimiter)
{
  write_سطر_delim ();
  *شے_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

ساکن اندرونی خالی
write_pending_سطر_delim (uintmax_t شے_idx)
{
  اگر (شے_idx > 0)
    write_سطر_delim ();
}

ساکن اندرونی خالی
write_selected_شے (bool *print_delimiter, bool range_آغاز,
                     حرف ثابت *buf, size_t n_بائٹیں)
{
  اگر (output_delimiter_string != output_delimiter_default)
    {
      اگر (*print_delimiter && range_آغاز)
        write_بائٹیں (output_delimiter_string, output_delimiter_لمبائی);
      *print_delimiter = true;
    }

  write_بائٹیں (buf, n_بائٹیں);
}

ساکن اندرونی خالی
append_میدان_1_chunk (حرف ثابت *buf, idx_t len, idx_t *n_بائٹیں)
{
  اگر (میدان_1_bufsize - *n_بائٹیں < len)
    {
      میدان_1_بفر = xpalloc (میدان_1_بفر, &میدان_1_bufsize,
                                len, -1, حجم *میدان_1_بفر);
    }

  copy_بائٹیں (میدان_1_بفر + *n_بائٹیں, buf, len);
  *n_بائٹیں += len;
}

ساکن اندرونی خالی
append_میدان_1_بائٹیں (mbbuf_t *mbbuf, mcel_t g, idx_t *n_بائٹیں)
{
  append_میدان_1_chunk (mbbuf_char_انحراف (mbbuf, g), g.len, n_بائٹیں);
}

ساکن اندرونی شمار میدان_terminator
جائزہ_mb_blank_میدان (mbbuf_t *mbbuf, ساخت mbfield_parser *parser,
                     bool *have_pending_سطر, bool write_میدان,
                     idx_t *n_بائٹیں)
{
  اگر (parser->trim_outer_whitespace && parser->at_سطر_آغاز)
    {
      شمار میدان_terminator terminator
        = skip_whitespace_تسلسل (mbbuf, parser, have_pending_سطر, false);
      اگر (terminator != میدان_ڈیٹا)
        واپس terminator;
    }

  parser->at_سطر_آغاز = false;

  جب (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      اگر (g.ch == MBBUF_EOF)
        واپس میدان_EOF;

      *have_pending_سطر = true;

      اگر (g.ch == سطر_delim)
        واپس میدان_سطر_DELIMITER;

      اگر (mcel_isblank (g))
        واپس skip_whitespace_تسلسل (mbbuf, parser, have_pending_سطر, true);

      اگر (n_بائٹیں)
        append_میدان_1_بائٹیں (mbbuf, g, n_بائٹیں);
      ورنہ اگر (write_میدان)
        write_بائٹیں (mbbuf_char_انحراف (mbbuf, g), g.len);
    }
}

ساکن اندرونی شمار میدان_terminator
جائزہ_mb_delim_میدان (mbbuf_t *mbbuf, bool *have_pending_سطر,
                     bool write_میدان, idx_t *n_بائٹیں)
{
  جب (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      اگر (g.ch == MBBUF_EOF)
        واپس میدان_EOF;

      *have_pending_سطر = true;

      اگر (g.ch == سطر_delim)
        واپس میدان_سطر_DELIMITER;

      اگر (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        واپس میدان_DELIMITER;

      اگر (n_بائٹیں)
        append_میدان_1_بائٹیں (mbbuf, g, n_بائٹیں);
      ورنہ اگر (write_میدان)
        write_بائٹیں (mbbuf_char_انحراف (mbbuf, g), g.len);
    }
}

ساکن اندرونی شمار میدان_terminator
جائزہ_mb_میدان (mbbuf_t *mbbuf, ساخت mbfield_parser *parser,
               bool *have_pending_سطر, bool write_میدان, idx_t *n_بائٹیں)
{
  واپس (parser->whitespace_delimited
          ? جائزہ_mb_blank_میدان (mbbuf, parser, have_pending_سطر,
                                 write_میدان, n_بائٹیں)
          : جائزہ_mb_delim_میدان (mbbuf, have_pending_سطر, write_میدان,
                                 n_بائٹیں));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

ساکن شمار میدان_terminator
جائزہ_mb_سطر_انتہا (mbbuf_t *mbbuf, ساخت mbfield_parser *parser,
                  bool *have_pending_سطر)
{
  اگر (parser->have_saved)
    {
      parser->have_saved = false;
      اگر (parser->saved_g.ch == MBBUF_EOF)
        واپس میدان_EOF;

      *have_pending_سطر = true;
      اگر (parser->saved_g.ch == سطر_delim)
        واپس میدان_سطر_DELIMITER;
    }

  جب (true)
    {
      idx_t دستیاب = mbbuf_topup (mbbuf);
      اگر (دستیاب == 0)
        واپس میدان_EOF;

      حرف *بفر = mbbuf->بفر + mbbuf->انحراف;
      حرف *سطر_انتہا = تلاش_بائٹیں (بفر, سطر_delim, دستیاب);
      idx_t n = سطر_انتہا ? سطر_انتہا - بفر + 1 : دستیاب;
      mbbuf_advance (mbbuf, n);
      *have_pending_سطر = true;

      اگر (سطر_انتہا)
        واپس میدان_سطر_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_خالص
ساکن حرف *
ڈھونڈو_میدان_delim (حرف *buf, size_t len)
{
  اگر (len < delim_mcel.len)
    واپس تہی;

#اگر ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  واپس memmem (buf, len, delim_بائٹیں, delim_mcel.len);
#ورنہ
  اگر (delim_mcel.len == 1)
    واپس memchr (buf, delim_بائٹیں[0], len);

  حرف ثابت *p = buf;
  حرف ثابت *انتہا = buf + len;

  حرف saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  جب (p < انتہا)
    {
      حرف ثابت *nul = memchr (p, '\0', انتہا - p);
      اگر (!nul)
        {
          حرف ثابت *مطابقت = لڑی_تلاش (p, delim_بائٹیں);
          buf[len] = saved;
          واپس (حرف *) مطابقت;
        }

      اگر (p < nul)
        {
          حرف ثابت *مطابقت = لڑی_تلاش (p, delim_بائٹیں);
          اگر (مطابقت)
            {
              buf[len] = saved;
              واپس (حرف *) مطابقت;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  واپس تہی;
#اگر_ختم
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_خالص
ساکن حرف *
ڈھونڈو_c_blank (حرف *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  برائے (idx_t i = 0; i < n; i++)
    اگر (c_isblank (buf[i]))
      واپس buf + i;

  واپس memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_خالص
ساکن idx_t
میدان_delim_overlap (حرف ثابت *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  جب (0 < overlap)
    {
      اگر (memeq (buf + len - overlap, delim_بائٹیں, overlap))
        واپس overlap;
      overlap--;
    }

  واپس 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

ساکن اندرونی شمار میدان_terminator
ڈھونڈو_میدان_terminator (حرف *buf, idx_t len,
                       ساخت bytesearch_context *ctx, حرف **terminator)
{
  اگر (ctx->منوال_2 == BYTESEARCH_سطر_ONLY)
    {
      اگر (!ctx->سطر_انتہا_معلوم)
        {
          ctx->سطر_انتہا = memchr (buf, سطر_delim, len);
          ctx->سطر_انتہا_معلوم = true;
        }

      *terminator = ctx->سطر_انتہا;
      واپس *terminator ? میدان_سطر_DELIMITER : میدان_ڈیٹا;
    }

  اگر (میدان_delim_is_سطر_delim ())
    {
      *terminator = memchr (buf, سطر_delim, len);
      واپس *terminator ? میدان_DELIMITER : میدان_ڈیٹا;
    }

  اگر (!ctx->سطر_انتہا_معلوم)
    {
      ctx->سطر_انتہا = memchr (buf, سطر_delim, len);
      ctx->سطر_انتہا_معلوم = true;
    }

  idx_t میدان_len = ctx->سطر_انتہا ? ctx->سطر_انتہا - buf : len;

  حرف *میدان_انتہا = (ctx->blank_delimited
                     ? ڈھونڈو_c_blank (buf, میدان_len)
                     : ڈھونڈو_میدان_delim (buf, میدان_len));

  اگر (میدان_انتہا)
    {
      *terminator = میدان_انتہا;
      واپس میدان_DELIMITER;
    }

  *terminator = ctx->سطر_انتہا;
  واپس ctx->سطر_انتہا ? میدان_سطر_DELIMITER : میدان_ڈیٹا;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

ساکن اندرونی خالی
maybe_write_سطر_delim (bool ملا_any_selected_میدان, uintmax_t میدان_idx)
{
  اگر (ملا_any_selected_میدان
      || !(suppress_non_delimited && میدان_idx == 1))
    write_سطر_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

ساکن اندرونی bool
begin_میدان_output (uintmax_t میدان_idx, bool بفر_پہلا_میدان,
                    bool *ملا_any_selected_میدان)
{
  bool write_میدان = print_kth (میدان_idx);

  اگر (write_میدان && ! (میدان_idx == 1 && بفر_پہلا_میدان))
    {
      اگر (*ملا_any_selected_میدان)
        write_بائٹیں (output_delimiter_string, output_delimiter_لمبائی);
      *ملا_any_selected_میدان = true;
    }

  واپس write_میدان;
}

ساکن اندرونی bool
handle_میدان_1 (uintmax_t میدان_idx, bool بفر_پہلا_میدان,
                idx_t *میدان_1_n_بائٹیں)
{
  bool میدان_1_selected = false;

  اگر (میدان_idx == 1 && بفر_پہلا_میدان)
    {
      اگر ((میدان_1_selected = print_kth (1)))
        write_بائٹیں (میدان_1_بفر, *میدان_1_n_بائٹیں);
      *میدان_1_n_بائٹیں = 0;
    }

  واپس میدان_1_selected;
}

ساکن اندرونی خالی
handle_میدان_delimiter (uintmax_t *میدان_idx, bool بفر_پہلا_میدان,
                        idx_t *میدان_1_n_بائٹیں,
                        bool *ملا_any_selected_میدان, bool *write_میدان)
{
  اگر (handle_میدان_1 (*میدان_idx, بفر_پہلا_میدان, میدان_1_n_بائٹیں))
    *ملا_any_selected_میدان = true;

  اگلا_شے (میدان_idx);
  *write_میدان = begin_میدان_output (*میدان_idx, بفر_پہلا_میدان,
                                     ملا_any_selected_میدان);
}

ساکن اندرونی bool
میدان_selection_exhausted (uintmax_t میدان_idx)
{
  واپس !print_kth (میدان_idx) && current_rp->lo == UINTMAX_MAX;
}

ساکن اندرونی خالی
اختتام_current_سطر (uintmax_t میدان_idx, bool بفر_پہلا_میدان,
                     idx_t *میدان_1_n_بائٹیں,
                     bool ملا_any_selected_میدان, bool سطر_terminated)
{
  اگر (میدان_idx == 1 && بفر_پہلا_میدان)
    {
      اگر (!suppress_non_delimited
          && (سطر_terminated || *میدان_1_n_بائٹیں != 0))
        {
          write_بائٹیں (میدان_1_بفر, *میدان_1_n_بائٹیں);
          write_سطر_delim ();
        }
      *میدان_1_n_بائٹیں = 0;
    }
  ورنہ
    maybe_write_سطر_delim (ملا_any_selected_میدان, میدان_idx);
}

ساکن اندرونی خالی
sync_byte_selection (uintmax_t byte_idx)
{
  جب (current_rp->hi <= byte_idx)
    current_rp++;
}

ساکن اندرونی خالی
reset_میدان_سطر (uintmax_t *میدان_idx, bool *ملا_any_selected_میدان,
                  bool *have_pending_سطر, ساخت mbfield_parser *parser)
{
  *میدان_idx = 1;
  current_rp = frp;
  *ملا_any_selected_میدان = false;
  *have_pending_سطر = false;
  parser->have_saved = false;
  parser->at_سطر_آغاز = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

ساکن خالی
cut_بائٹیں (FILE *دھارا)
{
  ساکن حرف بائٹیں_اندر[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  صحیح fd = fileno (دھارا);

  current_rp = frp;

  جب (true)
    {
      idx_t دستیاب = اقرأ (fd, بائٹیں_اندر, حجم بائٹیں_اندر);
      اگر (دستیاب <= 0)
        {
          اگر (دستیاب < 0)
            fseterr (دھارا);
          write_pending_سطر_delim (byte_idx);
          توڑو;
        }

      idx_t processed = 0;

      جب (processed < دستیاب)
        {
          حرف *سطر = بائٹیں_اندر + processed;
          idx_t مقصد_process = دستیاب - processed;
          حرف *سطر_انتہا = تلاش_بائٹیں (سطر, سطر_delim, مقصد_process);
          حرف *انتہا = سطر + (سطر_انتہا ? سطر_انتہا - سطر : مقصد_process);
          حرف *p = سطر;

          جب (0 < انتہا - p)
            {
              sync_byte_selection (byte_idx);

              اگر (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (انتہا - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              ورنہ
                {
                  idx_t n = MIN (انتہا - p, current_rp->hi - byte_idx);
                  write_selected_شے (&print_delimiter,
                                       is_range_آغاز_اشاریہ (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += انتہا - سطر;
          اگر (سطر_انتہا)
            {
              processed++;
              reset_شے_سطر (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

ساکن خالی
cut_characters_منوال (FILE *دھارا, bool byte_منوال)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  ساکن حرف بائٹیں_اندر[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, بائٹیں_اندر, حجم بائٹیں_اندر, دھارا);

  جب (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      اگر (idx && میدان_selection_exhausted (idx))
        {
          idx_t دستیاب = mbbuf_topup (&mbbuf);
          حرف *p = mbbuf.بفر + mbbuf.انحراف;
          حرف *انتہا = تلاش_بائٹیں (p, سطر_delim, دستیاب);
          mbbuf_advance (&mbbuf, انتہا ? انتہا - p : دستیاب);
          اگر (!انتہا && دستیاب)
            جاری;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      اگر (!byte_منوال && utf8 && idx + 1 < current_rp->lo)
        {
          حرف ثابت *p = mbbuf.بفر + mbbuf.انحراف;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          حرف ثابت *انتہا = تلاش_بائٹیں (p, سطر_delim, n);
          اگر (انتہا)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, انتہا - p + 1);
              reset_شے_سطر (&idx, &print_delimiter);
              جاری;
            }

          idx += u8_گنتی (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      اگر (g.ch == سطر_delim)
        reset_شے_سطر (&idx, &print_delimiter);
      ورنہ اگر (g.ch == MBBUF_EOF)
        {
          write_pending_سطر_delim (idx);
          توڑو;
        }
      ورنہ اگر (byte_منوال)
        {
          bool پہلا_selected_is_range_آغاز = false;
          bool نظر_آیا_selected = false;
          bool suffix_selected = true;

          برائے (idx_t i = 0; i < g.len; i++)
            {
              اگلا_شے (&idx);
              اگر (print_kth (idx))
                {
                  اگر (!نظر_آیا_selected)
                    {
                      نظر_آیا_selected = true;
                      پہلا_selected_is_range_آغاز
                        = is_range_آغاز_اشاریہ (idx);
                    }
                }
              ورنہ اگر (نظر_آیا_selected)
                suffix_selected = false;
            }

          اگر (نظر_آیا_selected && suffix_selected)
            write_selected_شے (&print_delimiter,پہلا_selected_is_range_آغاز,
                                 mbbuf_char_انحراف (&mbbuf, g), g.len);
        }
      ورنہ
        {
          اگلا_شے (&idx);
          اگر (print_kth (idx))
            write_selected_شے (&print_delimiter,
                                 is_range_آغاز_اشاریہ (idx),
                                 mbbuf_char_انحراف (&mbbuf, g), g.len);
        }
    }
}

ساکن خالی
cut_بائٹیں_no_بانٹو (FILE *دھارا)
{
  cut_characters_منوال (دھارا, true);
}

ساکن خالی
cut_characters (FILE *دھارا)
{
  cut_characters_منوال (دھارا, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

ساکن خالی
cut_fields_mb_any (FILE *دھارا, bool whitespace_منوال)
{
  ساکن حرف بائٹیں_اندر[IO_BUFSIZE];
  mbbuf_t mbbuf;
  ساخت mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_منوال,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_سطر_آغاز = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t میدان_idx = 1;
  bool ملا_any_selected_میدان = false;
  bool بفر_پہلا_میدان;
  bool have_pending_سطر = false;
  bool write_میدان;
  idx_t میدان_1_n_بائٹیں = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, بائٹیں_اندر, حجم بائٹیں_اندر, دھارا);

  بفر_پہلا_میدان = (suppress_non_delimited ^ !print_kth (1));
  write_میدان = begin_میدان_output (میدان_idx, بفر_پہلا_میدان,
                                    &ملا_any_selected_میدان);

  جب (true)
    {
      شمار میدان_terminator terminator;
      اگر (!بفر_پہلا_میدان || میدان_idx != 1)
        terminator = جائزہ_mb_میدان (&mbbuf, &parser, &have_pending_سطر,
                                    write_میدان, تہی);
      ورنہ
        terminator = جائزہ_mb_میدان (&mbbuf, &parser, &have_pending_سطر,
                                    false, &میدان_1_n_بائٹیں);

      اگر (terminator == میدان_DELIMITER)
        {
          handle_میدان_delimiter (&میدان_idx, بفر_پہلا_میدان,
                                  &میدان_1_n_بائٹیں,
                                  &ملا_any_selected_میدان, &write_میدان);
          اگر (! میدان_selection_exhausted (میدان_idx))
            جاری;

          terminator = جائزہ_mb_سطر_انتہا (&mbbuf, &parser,
                                         &have_pending_سطر);
        }

      اگر (terminator == میدان_EOF && !have_pending_سطر)
        توڑو;

      اختتام_current_سطر (میدان_idx, بفر_پہلا_میدان,
                           &میدان_1_n_بائٹیں, ملا_any_selected_میدان,
                           terminator == میدان_سطر_DELIMITER);
      اگر (terminator == میدان_EOF)
        توڑو;

      reset_میدان_سطر (&میدان_idx, &ملا_any_selected_میدان,
                        &have_pending_سطر, &parser);
      write_میدان = begin_میدان_output (میدان_idx, بفر_پہلا_میدان,
                                        &ملا_any_selected_میدان);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

ساکن خالی
cut_fields_mb (FILE *دھارا)
{
  cut_fields_mb_any (دھارا, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

ساکن خالی
cut_fields_bytesearch (FILE *دھارا)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  ساکن حرف بائٹیں_اندر[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t میدان_idx = 1;
  bool ملا_any_selected_میدان = false;
  bool have_pending_سطر = false;
  bool skip_blank_تسلسل = false;
  bool pending_سطر_delim_میدان = false;
  bool write_میدان;
  idx_t میدان_1_n_بائٹیں = 0;

  current_rp = frp;
  bool بفر_پہلا_میدان = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, بائٹیں_اندر, حجم بائٹیں_اندر - 1, دھارا);
  write_میدان = begin_میدان_output (میدان_idx, بفر_پہلا_میدان,
                                    &ملا_any_selected_میدان);

  ساخت bytesearch_context تلاش = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&تلاش);

  جب (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      تلاش.at_eof = mbbuf.eof;
      تلاش.سطر_انتہا_معلوم = false;

      اگر (pending_سطر_delim_میدان)
        {
          pending_سطر_delim_میدان = false;

          اگر (n_avail == 0)
            {
              اگر (handle_میدان_1 (میدان_idx, بفر_پہلا_میدان,
                                  &میدان_1_n_بائٹیں))
                اگلا_شے (&میدان_idx);
              maybe_write_سطر_delim (ملا_any_selected_میدان, میدان_idx);
              واپس;
            }

          handle_میدان_delimiter (&میدان_idx, بفر_پہلا_میدان,
                                  &میدان_1_n_بائٹیں,
                                  &ملا_any_selected_میدان, &write_میدان);
          اگر (whitespace_delimited)
            skip_blank_تسلسل = true;
        }

      اگر (n_avail == 0)
        توڑو;

      حرف *chunk = mbbuf.بفر + mbbuf.انحراف;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      اگر (میدان_idx == 1
          && (!whitespace_delimited || !میدان_selection_exhausted (میدان_idx))
          && !میدان_delim_is_سطر_delim ()
          && !(whitespace_delimited
               ? ڈھونڈو_c_blank (chunk, n_avail)
               : ڈھونڈو_میدان_delim (chunk, n_avail)))
        {
          حرف *آخری_سطر_delim = تلاش.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, سطر_delim, n_avail);
          اگر (آخری_سطر_delim)
            {
              idx_t n = آخری_سطر_delim - chunk + 1;
              اگر (! suppress_non_delimited)
                {
                  write_بائٹیں (میدان_1_بفر, میدان_1_n_بائٹیں);
                  write_بائٹیں (chunk, n);
                  اگر (تلاش.at_eof && chunk[n - 1] != سطر_delim)
                    write_سطر_delim ();
                }
              میدان_1_n_بائٹیں = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_سطر = false;
              اگر (تلاش.at_eof)
                واپس;
              جاری;
            }
        }

      جب (processed < n_avail)
        {
          حرف *terminator = تہی;

          اگر (skip_blank_تسلسل)
            {
              جب (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              اگر (processed == n_avail)
                توڑو;
              skip_blank_تسلسل = false;
            }

          اگر (میدان_selection_exhausted (میدان_idx))
            {
              اگر (میدان_delim_is_سطر_delim ())
                {
                  maybe_write_سطر_delim (ملا_any_selected_میدان,
                                          میدان_idx);
                  mbbuf_advance (&mbbuf, processed);
                  واپس;
                }
              تلاش.منوال_2 = BYTESEARCH_سطر_ONLY;
              شمار میدان_terminator terminator_قسم
                = ڈھونڈو_میدان_terminator (chunk + processed, n_avail - processed,
                                         &تلاش, &terminator);
              اگر (terminator_قسم == میدان_سطر_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_سطر_delim (ملا_any_selected_میدان,
                                          میدان_idx);
                  جاؤ reset_سطر;
                }
              processed = n_avail;
              توڑو;
            }

          شمار میدان_terminator terminator_قسم
            = ڈھونڈو_میدان_terminator (chunk + processed, n_avail - processed,
                                     &تلاش, &terminator);
          idx_t میدان_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          اگر (terminator_قسم == میدان_ڈیٹا
              && !تلاش.at_eof
              && !whitespace_delimited
              && !میدان_delim_is_سطر_delim ())
            میدان_len -= میدان_delim_overlap (chunk + processed, میدان_len);

          اگر (میدان_len || terminator)
            have_pending_سطر = true;

          /* Output field content.  */
          اگر (میدان_idx == 1 && بفر_پہلا_میدان)
            append_میدان_1_chunk (chunk + processed, میدان_len,
                                  &میدان_1_n_بائٹیں);
          ورنہ اگر (write_میدان)
            write_بائٹیں (chunk + processed, میدان_len);
          processed += میدان_len;

          اگر (terminator_قسم == میدان_ڈیٹا)
            توڑو;

          اگر (terminator_قسم == میدان_DELIMITER)
            {
              اگر (میدان_delim_is_سطر_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_سطر_delim_میدان = true;
                  توڑو;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_میدان_delimiter (&میدان_idx, بفر_پہلا_میدان,
                                      &میدان_1_n_بائٹیں,
                                      &ملا_any_selected_میدان, &write_میدان);
              اگر (whitespace_delimited)
                skip_blank_تسلسل = true;
            }
          ورنہ
            {
              affirm (terminator_قسم == میدان_سطر_DELIMITER);
              processed++;
              اختتام_current_سطر (میدان_idx, بفر_پہلا_میدان,
                                   &میدان_1_n_بائٹیں,
                                   ملا_any_selected_میدان, true);

            reset_سطر:
              میدان_idx = 1;
              current_rp = frp;
              ملا_any_selected_میدان = false;
              have_pending_سطر = false;
              bytesearch_context_reset (&تلاش);
              write_میدان = begin_میدان_output (میدان_idx, بفر_پہلا_میدان,
                                                &ملا_any_selected_میدان);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  اگر (!have_pending_سطر)
    واپس;

  اختتام_current_سطر (میدان_idx, بفر_پہلا_میدان, &میدان_1_n_بائٹیں,
                       ملا_any_selected_میدان, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

ساکن خالی
cut_fields_ws (FILE *دھارا)
{
  اگر (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (دھارا);
  ورنہ
    cut_fields_mb_any (دھارا, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

ساکن bool
cut_file (حرف ثابت *file, خالی (*cut_دھارا) (FILE *))
{
  FILE *دھارا;

  اگر (streq (file, "-"))
    {
      have_read_stdin = true;
      دھارا = stdin;
      assume (دھارا);  /* Pacify GCC bug#109613.  */
    }
  ورنہ
    {
      دھارا = fopen (file, "r");
      اگر (دھارا == تہی)
        {
          error (0, errno, "%s", quotef (file));
          واپس false;
        }
    }

  fadvise (دھارا, FADVISE_SEQUENTIAL);

  cut_دھارا (دھارا);

  صحیح err = errno;
  اگر (!ferror (دھارا))
    err = 0;
  اگر (streq (file, "-"))
    clearerr (دھارا);		/* Also clear EOF.  */
  ورنہ اگر (fclose (دھارا) == EOF)
    err = errno;
  اگر (err)
    {
      error (0, err, "%s", quotef (file));
      واپس false;
    }
  واپس true;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  حرف *spec_list_string = تہی;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, تہی))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 'b':
          cut_منوال = CUT_منوال_بائٹیں;
          FALLTHROUGH;
        حالت 'c':
          اگر (optc == 'c')
            cut_منوال = CUT_منوال_CHARACTERS;
          FALLTHROUGH;
        حالت 'f':
        حالت 'F':
          اگر (optc == 'f' || optc == 'F')
            cut_منوال = CUT_منوال_FIELDS;
          اگر (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          اگر (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          توڑو;

        حالت 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            اگر (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_بائٹیں (delim_بائٹیں, optarg, g.len + 1);
            delim_specified = true;
            توڑو;
          }

        حالت 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_دلائل,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          توڑو;

        حالت 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_لمبائی = (optarg[0] == '\0'
                                     ? 1 : لڑی_لمبائی (optarg));
          output_delimiter_string = optarg;
          توڑو;

        حالت 'n':
          no_بانٹو = true;
          توڑو;

        حالت 's':
          suppress_non_delimited = true;
          توڑو;

        حالت 'z':
          سطر_delim = '\0';
          توڑو;

        حالت COMPLEMENT_OPTION:
          complement = true;
          توڑو;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  اگر (cut_منوال == CUT_منوال_بائٹیں || cut_منوال == CUT_منوال_CHARACTERS)
    {
      اگر (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an داخلہ delimiter makes sense\n\
\tonly when operating on fields"));

      اگر (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited سطریں makes sense\n\
\tonly when operating on fields"));
    }

  اگر (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  اگر (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  سیٹ_fields (spec_list_string,
              (((cut_منوال == CUT_منوال_بائٹیں
                 || cut_منوال == CUT_منوال_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  اگر (!delim_specified)
    {
      delim_بائٹیں[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  اگر (output_delimiter_string == تہی)
    {
      output_delimiter_string = output_delimiter_default;
      اگر (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_لمبائی = 1;
        }
      ورنہ
        {
          copy_بائٹیں (output_delimiter_default, delim_بائٹیں, delim_mcel.len);
          output_delimiter_لمبائی = delim_mcel.len;
        }
    }

  خالی (*cut_دھارا) (FILE *) = تہی;
  انتخاب (cut_منوال)
    {
    حالت CUT_منوال_بائٹیں:
      cut_دھارا = MB_CUR_MAX <= 1 || !no_بانٹو
                   ? cut_بائٹیں : cut_بائٹیں_no_بانٹو;
      توڑو;

    حالت CUT_منوال_CHARACTERS:
      cut_دھارا = MB_CUR_MAX <= 1 ? cut_بائٹیں : cut_characters;
      توڑو;

    حالت CUT_منوال_FIELDS:
      cut_دھارا = whitespace_delimited ? cut_fields_ws
                   : bytesearch_میدان_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      توڑو;

    حالت CUT_منوال_NONE:
      affirm (false);
    }
  affirm (cut_دھارا);
  اگر (optind == argc)
    ok = cut_file ("-", cut_دھارا);
  ورنہ
    برائے (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_دھارا);


  اگر (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
