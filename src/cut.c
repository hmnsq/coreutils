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
#define PROGRAM_NAME "cut"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#define FATAL_ERROR(Message)						\
  انجامبده									\
    {									\
      error (0, 0, (Message));						\
      usage (EXIT_FAILURE);						\
    }									\
  وقتی (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#define SMALL_BYTE_THRESHOLD 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
ایستا ساختار field_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
ایستا نویسه *field_1_buffer;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
ایستا idx_t field_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
ایستا bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
ایستا bool complement;

/* The delimiter character for multibyte field mode.  */
ایستا mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
ایستا نویسه delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
ایستا unsigned نویسه سطر_delim = '\n';

/* The length of output_delimiter_string.  */
ایستا size_t output_delimiter_length;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
ایستا نویسه *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
ایستا نویسه output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
ایستا bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
ایستا bool no_split;

/* If true, interpret each run of whitespace as one field delimiter.  */
ایستا bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
ایستا bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
ایستا bool space_output_delimiter_default;

شمارش whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

ایستا نویسه ثابت *ثابت whitespace_option_args[] =
{
  "trimmed", پوچ
};

ایستا شمارش whitespace_option ثابت whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_args, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
ایستا شمارش
{
  CUT_MODE_NONE,
  CUT_MODE_BYTES,
  CUT_MODE_CHARACTERS,
  CUT_MODE_FIELDS
} cut_mode = CUT_MODE_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

ایستا ساختار option ثابت longopts[] =
{
  {"bytes", required_argument, پوچ, 'b'},
  {"characters", required_argument, پوچ, 'c'},
  {"fields", required_argument, پوچ, 'f'},
  {"delimiter", required_argument, پوچ, 'd'},
  {"no-partial", no_argument, پوچ, 'n'},
  {"whitespace-delimited", optional_argument, پوچ, 'w'},
  {"only-delimited", no_argument, پوچ, 's'},
  {"output-delimiter", required_argument, پوچ, 'O'},
  {"complement", no_argument, پوچ, COMPLEMENT_OPTION},
  {"zero-terminated", no_argument, پوچ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Print selected parts of سطرها from each FILE to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes=LIST\n\
         select only these byte positions\n\
"));
      oputs (_("\
  -c, --characters=LIST\n\
         select only these character positions\n\
"));
      oputs (_("\
      --complement\n\
         complement the set of selected bytes, characters or fields\n\
"));
      oputs (_("\
  -d, --delimiter=DELIM\n\
         use DELIM instead of TAB برای field delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any سطر that contains\n\
         no delimiter character, unless the -s option is specified\n\
"));
      oputs (_("\
  -F LIST\n\
         like -f, but also implies -w and -O ' '\n\
"));
      oputs (_("\
  -n, --no-partial\n\
         with -b, don't output partial multi-byte characters\n\
"));
      oputs (_("\
  -O, --output-delimiter=STRING\n\
         use STRING as the output delimiter;\n\
         the پیشفرض is to use the درون_داد delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         انجامبده not print سطرها not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a run of blank characters as the field delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one of -b, -c, -f or -F.  Each LIST is made up of one\n\
range, or many ranges separated by commas.  Selected درون_داد is written\n\
in the same order that it is اقرأ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one of:\n\
\n\
  N     N'th byte, character or field, counted from 1\n\
  N-    from N'th byte, character or field, to end of سطر\n\
  N-M   from N'th to M'th (included) byte, character or field\n\
  -M    from first to M'th (included) byte, character or field\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

ایستا درونخطی تهی
next_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  اگر ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

ایستا درونخطی bool
print_kth (uintmax_t k)
{
  بازگشت current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

ایستا درونخطی bool
is_range_start_index (uintmax_t k)
{
  بازگشت k == current_rp->lo;
}

ایستا درونخطی bool
field_delim_is_سطر_delim (تهی)
{
  بازگشت delim_mcel.len == 1 && delim_bytes[0] == سطر_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

ایستا bool
mcel_isblank (mcel_t g)
{
  بازگشت (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

ایستا درونخطی bool
bytesearch_field_delim_ok (تهی)
{
  unsigned نویسه delim_0 = delim_bytes[0];

  بازگشت (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


شمارش field_terminator
{
  FIELD_DATA,
  FIELD_DELIMITER,
  FIELD_سطر_DELIMITER,
  FIELD_EOF
};

شمارش bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_سطر_ONLY
};

ساختار bytesearch_context
{
  شمارش bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  نویسه *سطر_end;
  bool سطر_end_known;
};

ایستا درونخطی تهی
bytesearch_context_reset (ساختار bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->سطر_end = پوچ;
  ctx->سطر_end_known = false;
}

ساختار mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_سطر_start;
  bool have_saved;
  mcel_t saved_g;
};

ایستا درونخطی mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  اگر (*have_saved)
    {
      *have_saved = false;
      بازگشت *saved_g;
    }
  بازگشت mbbuf_get_char (mbbuf);
}

ایستا درونخطی شمارش field_terminator
skip_whitespace_run (mbbuf_t *mbuf, ساختار mbfield_parser *parser,
                     bool *have_pending_سطر,
                     bool have_initial_whitespace)
{
  mcel_t g;

  انجامبده
    {
      g = mbbuf_get_char (mbuf);
      اگر (g.ch != MBBUF_EOF)
        *have_pending_سطر = true;
    }
  وقتی (g.ch != MBBUF_EOF && g.ch != سطر_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_سطر_start;

  اگر (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == سطر_delim))
    {
      بازگشت g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_سطر_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  بازگشت trim_start && !have_initial_whitespace ? FIELD_DATA : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

ایستا تهی
write_bytes (نویسه ثابت *buf, size_t n_bytes)
{
  اگر (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      برای (size_t i = 0; i < n_bytes; i++)
        اگر (putchar (buf[i]) < 0)
          write_error ();
      بازگشت;
    }

  اگر (fwrite (buf, اندازه (نویسه), n_bytes, stdout) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

ایستا درونخطی تهی
copy_bytes (نویسه *dst, نویسه ثابت *src, size_t n_bytes)
{
  اگر (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      برای (size_t i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      بازگشت;
    }

  memcpy (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

ایستا درونخطی تهی*
جستجو_bytes (نویسه ثابت *buf, صحیح c, size_t n_bytes)
{
  اگر (n_bytes != 0 && to_uchar (buf[0]) == c)
    بازگشت (نویسه *) buf;
  اگر (1 < n_bytes && to_uchar (buf[1]) == c)
    بازگشت (نویسه *) buf + 1;

  بازگشت 2 < n_bytes ? (نویسه *) memchr (buf + 2, c, n_bytes - 2) : پوچ;
}

ایستا درونخطی تهی
write_سطر_delim (تهی)
{
  اگر (putchar (سطر_delim) < 0)
    write_error ();
}

ایستا درونخطی تهی
reset_item_سطر (uintmax_t *item_idx, bool *print_delimiter)
{
  write_سطر_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

ایستا درونخطی تهی
write_pending_سطر_delim (uintmax_t item_idx)
{
  اگر (item_idx > 0)
    write_سطر_delim ();
}

ایستا درونخطی تهی
write_selected_item (bool *print_delimiter, bool range_start,
                     نویسه ثابت *buf, size_t n_bytes)
{
  اگر (output_delimiter_string != output_delimiter_default)
    {
      اگر (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

ایستا درونخطی تهی
append_field_1_chunk (نویسه ثابت *buf, idx_t len, idx_t *n_bytes)
{
  اگر (field_1_bufsize - *n_bytes < len)
    {
      field_1_buffer = xpalloc (field_1_buffer, &field_1_bufsize,
                                len, -1, اندازه *field_1_buffer);
    }

  copy_bytes (field_1_buffer + *n_bytes, buf, len);
  *n_bytes += len;
}

ایستا درونخطی تهی
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

ایستا درونخطی شمارش field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, ساختار mbfield_parser *parser,
                     bool *have_pending_سطر, bool write_field,
                     idx_t *n_bytes)
{
  اگر (parser->trim_outer_whitespace && parser->at_سطر_start)
    {
      شمارش field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_سطر, false);
      اگر (terminator != FIELD_DATA)
        بازگشت terminator;
    }

  parser->at_سطر_start = false;

  وقتی (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      اگر (g.ch == MBBUF_EOF)
        بازگشت FIELD_EOF;

      *have_pending_سطر = true;

      اگر (g.ch == سطر_delim)
        بازگشت FIELD_سطر_DELIMITER;

      اگر (mcel_isblank (g))
        بازگشت skip_whitespace_run (mbbuf, parser, have_pending_سطر, true);

      اگر (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      وگرنه اگر (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

ایستا درونخطی شمارش field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_سطر,
                     bool write_field, idx_t *n_bytes)
{
  وقتی (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      اگر (g.ch == MBBUF_EOF)
        بازگشت FIELD_EOF;

      *have_pending_سطر = true;

      اگر (g.ch == سطر_delim)
        بازگشت FIELD_سطر_DELIMITER;

      اگر (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        بازگشت FIELD_DELIMITER;

      اگر (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      وگرنه اگر (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

ایستا درونخطی شمارش field_terminator
scan_mb_field (mbbuf_t *mbbuf, ساختار mbfield_parser *parser,
               bool *have_pending_سطر, bool write_field, idx_t *n_bytes)
{
  بازگشت (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_سطر,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_سطر, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

ایستا شمارش field_terminator
scan_mb_سطر_end (mbbuf_t *mbbuf, ساختار mbfield_parser *parser,
                  bool *have_pending_سطر)
{
  اگر (parser->have_saved)
    {
      parser->have_saved = false;
      اگر (parser->saved_g.ch == MBBUF_EOF)
        بازگشت FIELD_EOF;

      *have_pending_سطر = true;
      اگر (parser->saved_g.ch == سطر_delim)
        بازگشت FIELD_سطر_DELIMITER;
    }

  وقتی (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      اگر (available == 0)
        بازگشت FIELD_EOF;

      نویسه *buffer = mbbuf->buffer + mbbuf->offset;
      نویسه *سطر_end = جستجو_bytes (buffer, سطر_delim, available);
      idx_t n = سطر_end ? سطر_end - buffer + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_سطر = true;

      اگر (سطر_end)
        بازگشت FIELD_سطر_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
ایستا نویسه *
find_field_delim (نویسه *buf, size_t len)
{
  اگر (len < delim_mcel.len)
    بازگشت پوچ;

#اگر ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  بازگشت memmem (buf, len, delim_bytes, delim_mcel.len);
#وگرنه
  اگر (delim_mcel.len == 1)
    بازگشت memchr (buf, delim_bytes[0], len);

  نویسه ثابت *p = buf;
  نویسه ثابت *end = buf + len;

  نویسه saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  وقتی (p < end)
    {
      نویسه ثابت *nul = memchr (p, '\0', end - p);
      اگر (!nul)
        {
          نویسه ثابت *match = strstr (p, delim_bytes);
          buf[len] = saved;
          بازگشت (نویسه *) match;
        }

      اگر (p < nul)
        {
          نویسه ثابت *match = strstr (p, delim_bytes);
          اگر (match)
            {
              buf[len] = saved;
              بازگشت (نویسه *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  بازگشت پوچ;
#endif
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
ایستا نویسه *
find_c_blank (نویسه *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  برای (idx_t i = 0; i < n; i++)
    اگر (c_isblank (buf[i]))
      بازگشت buf + i;

  بازگشت memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_PURE
ایستا idx_t
field_delim_overlap (نویسه ثابت *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  وقتی (0 < overlap)
    {
      اگر (memeq (buf + len - overlap, delim_bytes, overlap))
        بازگشت overlap;
      overlap--;
    }

  بازگشت 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

ایستا درونخطی شمارش field_terminator
find_field_terminator (نویسه *buf, idx_t len,
                       ساختار bytesearch_context *ctx, نویسه **terminator)
{
  اگر (ctx->mode == BYTESEARCH_سطر_ONLY)
    {
      اگر (!ctx->سطر_end_known)
        {
          ctx->سطر_end = memchr (buf, سطر_delim, len);
          ctx->سطر_end_known = true;
        }

      *terminator = ctx->سطر_end;
      بازگشت *terminator ? FIELD_سطر_DELIMITER : FIELD_DATA;
    }

  اگر (field_delim_is_سطر_delim ())
    {
      *terminator = memchr (buf, سطر_delim, len);
      بازگشت *terminator ? FIELD_DELIMITER : FIELD_DATA;
    }

  اگر (!ctx->سطر_end_known)
    {
      ctx->سطر_end = memchr (buf, سطر_delim, len);
      ctx->سطر_end_known = true;
    }

  idx_t field_len = ctx->سطر_end ? ctx->سطر_end - buf : len;

  نویسه *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  اگر (field_end)
    {
      *terminator = field_end;
      بازگشت FIELD_DELIMITER;
    }

  *terminator = ctx->سطر_end;
  بازگشت ctx->سطر_end ? FIELD_سطر_DELIMITER : FIELD_DATA;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

ایستا درونخطی تهی
maybe_write_سطر_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  اگر (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_سطر_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

ایستا درونخطی bool
begin_field_output (uintmax_t field_idx, bool buffer_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  اگر (write_field && ! (field_idx == 1 && buffer_first_field))
    {
      اگر (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *found_any_selected_field = true;
    }

  بازگشت write_field;
}

ایستا درونخطی bool
handle_field_1 (uintmax_t field_idx, bool buffer_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  اگر (field_idx == 1 && buffer_first_field)
    {
      اگر ((field_1_selected = print_kth (1)))
        write_bytes (field_1_buffer, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  بازگشت field_1_selected;
}

ایستا درونخطی تهی
handle_field_delimiter (uintmax_t *field_idx, bool buffer_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  اگر (handle_field_1 (*field_idx, buffer_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  next_item (field_idx);
  *write_field = begin_field_output (*field_idx, buffer_first_field,
                                     found_any_selected_field);
}

ایستا درونخطی bool
field_selection_exhausted (uintmax_t field_idx)
{
  بازگشت !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

ایستا درونخطی تهی
finish_current_سطر (uintmax_t field_idx, bool buffer_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool سطر_terminated)
{
  اگر (field_idx == 1 && buffer_first_field)
    {
      اگر (!suppress_non_delimited
          && (سطر_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_buffer, *field_1_n_bytes);
          write_سطر_delim ();
        }
      *field_1_n_bytes = 0;
    }
  وگرنه
    maybe_write_سطر_delim (found_any_selected_field, field_idx);
}

ایستا درونخطی تهی
sync_byte_selection (uintmax_t byte_idx)
{
  وقتی (current_rp->hi <= byte_idx)
    current_rp++;
}

ایستا درونخطی تهی
reset_field_سطر (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_سطر, ساختار mbfield_parser *parser)
{
  *field_idx = 1;
  current_rp = frp;
  *found_any_selected_field = false;
  *have_pending_سطر = false;
  parser->have_saved = false;
  parser->at_سطر_start = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

ایستا تهی
cut_bytes (FILE *stream)
{
  ایستا نویسه bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  صحیح fd = fileno (stream);

  current_rp = frp;

  وقتی (true)
    {
      idx_t available = اقرأ (fd, bytes_in, اندازه bytes_in);
      اگر (available <= 0)
        {
          اگر (available < 0)
            fseterr (stream);
          write_pending_سطر_delim (byte_idx);
          بشکن;
        }

      idx_t processed = 0;

      وقتی (processed < available)
        {
          نویسه *سطر = bytes_in + processed;
          idx_t to_process = available - processed;
          نویسه *سطر_end = جستجو_bytes (سطر, سطر_delim, to_process);
          نویسه *end = سطر + (سطر_end ? سطر_end - سطر : to_process);
          نویسه *p = سطر;

          وقتی (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              اگر (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              وگرنه
                {
                  idx_t n = MIN (end - p, current_rp->hi - byte_idx);
                  write_selected_item (&print_delimiter,
                                       is_range_start_index (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += end - سطر;
          اگر (سطر_end)
            {
              processed++;
              reset_item_سطر (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

ایستا تهی
cut_characters_mode (FILE *stream, bool byte_mode)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  ایستا نویسه bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, اندازه bytes_in, stream);

  وقتی (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      اگر (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          نویسه *p = mbbuf.buffer + mbbuf.offset;
          نویسه *end = جستجو_bytes (p, سطر_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          اگر (!end && available)
            ادامه;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      اگر (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          نویسه ثابت *p = mbbuf.buffer + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          نویسه ثابت *end = جستجو_bytes (p, سطر_delim, n);
          اگر (end)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, end - p + 1);
              reset_item_سطر (&idx, &print_delimiter);
              ادامه;
            }

          idx += u8_شمار (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      اگر (g.ch == سطر_delim)
        reset_item_سطر (&idx, &print_delimiter);
      وگرنه اگر (g.ch == MBBUF_EOF)
        {
          write_pending_سطر_delim (idx);
          بشکن;
        }
      وگرنه اگر (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          برای (idx_t i = 0; i < g.len; i++)
            {
              next_item (&idx);
              اگر (print_kth (idx))
                {
                  اگر (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_index (idx);
                    }
                }
              وگرنه اگر (seen_selected)
                suffix_selected = false;
            }

          اگر (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      وگرنه
        {
          next_item (&idx);
          اگر (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_index (idx),
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
    }
}

ایستا تهی
cut_bytes_no_split (FILE *stream)
{
  cut_characters_mode (stream, true);
}

ایستا تهی
cut_characters (FILE *stream)
{
  cut_characters_mode (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

ایستا تهی
cut_fields_mb_any (FILE *stream, bool whitespace_mode)
{
  ایستا نویسه bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  ساختار mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_mode,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_سطر_start = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool buffer_first_field;
  bool have_pending_سطر = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, اندازه bytes_in, stream);

  buffer_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  وقتی (true)
    {
      شمارش field_terminator terminator;
      اگر (!buffer_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_سطر,
                                    write_field, پوچ);
      وگرنه
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_سطر,
                                    false, &field_1_n_bytes);

      اگر (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          اگر (! field_selection_exhausted (field_idx))
            ادامه;

          terminator = scan_mb_سطر_end (&mbbuf, &parser,
                                         &have_pending_سطر);
        }

      اگر (terminator == FIELD_EOF && !have_pending_سطر)
        بشکن;

      finish_current_سطر (field_idx, buffer_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_سطر_DELIMITER);
      اگر (terminator == FIELD_EOF)
        بشکن;

      reset_field_سطر (&field_idx, &found_any_selected_field,
                        &have_pending_سطر, &parser);
      write_field = begin_field_output (field_idx, buffer_first_field,
                                        &found_any_selected_field);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

ایستا تهی
cut_fields_mb (FILE *stream)
{
  cut_fields_mb_any (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

ایستا تهی
cut_fields_bytesearch (FILE *stream)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  ایستا نویسه bytes_in[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool have_pending_سطر = false;
  bool skip_blank_run = false;
  bool pending_سطر_delim_field = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  bool buffer_first_field = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, bytes_in, اندازه bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  ساختار bytesearch_context جستجو = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&جستجو);

  وقتی (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      جستجو.at_eof = mbbuf.eof;
      جستجو.سطر_end_known = false;

      اگر (pending_سطر_delim_field)
        {
          pending_سطر_delim_field = false;

          اگر (n_avail == 0)
            {
              اگر (handle_field_1 (field_idx, buffer_first_field,
                                  &field_1_n_bytes))
                next_item (&field_idx);
              maybe_write_سطر_delim (found_any_selected_field, field_idx);
              بازگشت;
            }

          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          اگر (whitespace_delimited)
            skip_blank_run = true;
        }

      اگر (n_avail == 0)
        بشکن;

      نویسه *chunk = mbbuf.buffer + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      اگر (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_سطر_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          نویسه *last_سطر_delim = جستجو.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, سطر_delim, n_avail);
          اگر (last_سطر_delim)
            {
              idx_t n = last_سطر_delim - chunk + 1;
              اگر (! suppress_non_delimited)
                {
                  write_bytes (field_1_buffer, field_1_n_bytes);
                  write_bytes (chunk, n);
                  اگر (جستجو.at_eof && chunk[n - 1] != سطر_delim)
                    write_سطر_delim ();
                }
              field_1_n_bytes = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_سطر = false;
              اگر (جستجو.at_eof)
                بازگشت;
              ادامه;
            }
        }

      وقتی (processed < n_avail)
        {
          نویسه *terminator = پوچ;

          اگر (skip_blank_run)
            {
              وقتی (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              اگر (processed == n_avail)
                بشکن;
              skip_blank_run = false;
            }

          اگر (field_selection_exhausted (field_idx))
            {
              اگر (field_delim_is_سطر_delim ())
                {
                  maybe_write_سطر_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  بازگشت;
                }
              جستجو.mode = BYTESEARCH_سطر_ONLY;
              شمارش field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &جستجو, &terminator);
              اگر (terminator_kind == FIELD_سطر_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_سطر_delim (found_any_selected_field,
                                          field_idx);
                  برو reset_سطر;
                }
              processed = n_avail;
              بشکن;
            }

          شمارش field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &جستجو, &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          اگر (terminator_kind == FIELD_DATA
              && !جستجو.at_eof
              && !whitespace_delimited
              && !field_delim_is_سطر_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          اگر (field_len || terminator)
            have_pending_سطر = true;

          /* Output field content.  */
          اگر (field_idx == 1 && buffer_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          وگرنه اگر (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          اگر (terminator_kind == FIELD_DATA)
            بشکن;

          اگر (terminator_kind == FIELD_DELIMITER)
            {
              اگر (field_delim_is_سطر_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_سطر_delim_field = true;
                  بشکن;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, buffer_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              اگر (whitespace_delimited)
                skip_blank_run = true;
            }
          وگرنه
            {
              affirm (terminator_kind == FIELD_سطر_DELIMITER);
              processed++;
              finish_current_سطر (field_idx, buffer_first_field,
                                   &field_1_n_bytes,
                                   found_any_selected_field, true);

            reset_سطر:
              field_idx = 1;
              current_rp = frp;
              found_any_selected_field = false;
              have_pending_سطر = false;
              bytesearch_context_reset (&جستجو);
              write_field = begin_field_output (field_idx, buffer_first_field,
                                                &found_any_selected_field);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  اگر (!have_pending_سطر)
    بازگشت;

  finish_current_سطر (field_idx, buffer_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

ایستا تهی
cut_fields_ws (FILE *stream)
{
  اگر (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  وگرنه
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

ایستا bool
cut_file (نویسه ثابت *file, تهی (*cut_stream) (FILE *))
{
  FILE *stream;

  اگر (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  وگرنه
    {
      stream = fopen (file, "r");
      اگر (stream == پوچ)
        {
          error (0, errno, "%s", quotef (file));
          بازگشت false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  صحیح err = errno;
  اگر (!ferror (stream))
    err = 0;
  اگر (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF.  */
  وگرنه اگر (fclose (stream) == EOF)
    err = errno;
  اگر (err)
    {
      error (0, err, "%s", quotef (file));
      بازگشت false;
    }
  بازگشت true;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  نویسه *spec_list_string = پوچ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, پوچ))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 'b':
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        حالت 'c':
          اگر (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        حالت 'f':
        حالت 'F':
          اگر (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
          اگر (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          اگر (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          بشکن;

        حالت 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            اگر (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            بشکن;
          }

        حالت 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          بشکن;

        حالت 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_length = (optarg[0] == '\0'
                                     ? 1 : strlen (optarg));
          output_delimiter_string = optarg;
          بشکن;

        حالت 'n':
          no_split = true;
          بشکن;

        حالت 's':
          suppress_non_delimited = true;
          بشکن;

        حالت 'z':
          سطر_delim = '\0';
          بشکن;

        حالت COMPLEMENT_OPTION:
          complement = true;
          بشکن;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  اگر (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
    {
      اگر (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an درون_داد delimiter makes sense\n\
\tonly when operating on fields"));

      اگر (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited سطرها makes sense\n\
\tonly when operating on fields"));
    }

  اگر (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  اگر (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  اگر (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  اگر (output_delimiter_string == پوچ)
    {
      output_delimiter_string = output_delimiter_default;
      اگر (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_length = 1;
        }
      وگرنه
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_length = delim_mcel.len;
        }
    }

  تهی (*cut_stream) (FILE *) = پوچ;
  انتخاب (cut_mode)
    {
    حالت CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      بشکن;

    حالت CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      بشکن;

    حالت CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      بشکن;

    حالت CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  اگر (optind == argc)
    ok = cut_file ("-", cut_stream);
  وگرنه
    برای (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  اگر (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
