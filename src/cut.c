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
#define PROGRAM_NAME "cut"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#define FATAL_ERROR(Message)						\
  افعل									\
    {									\
      error (0, 0, (Message));						\
      usage (EXIT_FAILURE);						\
    }									\
  طالما (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#define SMALL_BYTE_THRESHOLD 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
ساكن بنية field_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
ساكن حرف *field_1_buffer;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
ساكن idx_t field_1_bufsize;

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
ساكن حرف delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
ساكن unsigned حرف سطر_delim = '\n';

/* The length of output_delimiter_string.  */
ساكن size_t output_delimiter_length;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
ساكن حرف *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
ساكن حرف output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
ساكن bool no_split;

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

ساكن حرف ثابت *ثابت whitespace_option_args[] =
{
  "trimmed", عدم
};

ساكن تعداد whitespace_option ثابت whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_args, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
ساكن تعداد
{
  CUT_MODE_NONE,
  CUT_MODE_BYTES,
  CUT_MODE_CHARACTERS,
  CUT_MODE_FIELDS
} cut_mode = CUT_MODE_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"bytes", required_argument, عدم, 'b'},
  {"characters", required_argument, عدم, 'c'},
  {"fields", required_argument, عدم, 'f'},
  {"delimiter", required_argument, عدم, 'd'},
  {"no-partial", no_argument, عدم, 'n'},
  {"whitespace-delimited", optional_argument, عدم, 'w'},
  {"only-delimited", no_argument, عدم, 's'},
  {"output-delimiter", required_argument, عدم, 'O'},
  {"complement", no_argument, عدم, COMPLEMENT_OPTION},
  {"zero-terminated", no_argument, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Print selected parts of سطریں from each FILE to standard output.\n\
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
         use DELIM instead of TAB لكل field delimiter\n\
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
         the افتراضي is to use the داخلہ delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         افعل not print سطریں not containing delimiters\n\
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
range, or many ranges separated by commas.  Selected داخلہ is written\n\
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

ساكن مضمن فراغ
next_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  إذا ((*item_idx) > current_rp->hi)
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
is_range_start_index (uintmax_t k)
{
  أرجع k == current_rp->lo;
}

ساكن مضمن bool
field_delim_is_سطر_delim (فراغ)
{
  أرجع delim_mcel.len == 1 && delim_bytes[0] == سطر_delim;
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
bytesearch_field_delim_ok (فراغ)
{
  unsigned حرف delim_0 = delim_bytes[0];

  أرجع (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


تعداد field_terminator
{
  FIELD_DATA,
  FIELD_DELIMITER,
  FIELD_سطر_DELIMITER,
  FIELD_EOF
};

تعداد bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_سطر_ONLY
};

بنية bytesearch_context
{
  تعداد bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  حرف *سطر_end;
  bool سطر_end_known;
};

ساكن مضمن فراغ
bytesearch_context_reset (بنية bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->سطر_end = عدم;
  ctx->سطر_end_known = false;
}

بنية mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_سطر_start;
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

ساكن مضمن تعداد field_terminator
skip_whitespace_run (mbbuf_t *mbuf, بنية mbfield_parser *parser,
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

  bool trim_start = parser->trim_outer_whitespace && parser->at_سطر_start;

  إذا (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == سطر_delim))
    {
      أرجع g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_سطر_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  أرجع trim_start && !have_initial_whitespace ? FIELD_DATA : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

ساكن فراغ
write_bytes (حرف ثابت *buf, size_t n_bytes)
{
  إذا (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      لكل (size_t i = 0; i < n_bytes; i++)
        إذا (putchar (buf[i]) < 0)
          write_error ();
      أرجع;
    }

  إذا (fwrite (buf, حجم (حرف), n_bytes, stdout) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

ساكن مضمن فراغ
copy_bytes (حرف *dst, حرف ثابت *src, size_t n_bytes)
{
  إذا (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      لكل (size_t i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      أرجع;
    }

  memcpy (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

ساكن مضمن فراغ*
تلاش_bytes (حرف ثابت *buf, صحيح c, size_t n_bytes)
{
  إذا (n_bytes != 0 && to_uchar (buf[0]) == c)
    أرجع (حرف *) buf;
  إذا (1 < n_bytes && to_uchar (buf[1]) == c)
    أرجع (حرف *) buf + 1;

  أرجع 2 < n_bytes ? (حرف *) memchr (buf + 2, c, n_bytes - 2) : عدم;
}

ساكن مضمن فراغ
write_سطر_delim (فراغ)
{
  إذا (putchar (سطر_delim) < 0)
    write_error ();
}

ساكن مضمن فراغ
reset_item_سطر (uintmax_t *item_idx, bool *print_delimiter)
{
  write_سطر_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

ساكن مضمن فراغ
write_pending_سطر_delim (uintmax_t item_idx)
{
  إذا (item_idx > 0)
    write_سطر_delim ();
}

ساكن مضمن فراغ
write_selected_item (bool *print_delimiter, bool range_start,
                     حرف ثابت *buf, size_t n_bytes)
{
  إذا (output_delimiter_string != output_delimiter_default)
    {
      إذا (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

ساكن مضمن فراغ
append_field_1_chunk (حرف ثابت *buf, idx_t len, idx_t *n_bytes)
{
  إذا (field_1_bufsize - *n_bytes < len)
    {
      field_1_buffer = xpalloc (field_1_buffer, &field_1_bufsize,
                                len, -1, حجم *field_1_buffer);
    }

  copy_bytes (field_1_buffer + *n_bytes, buf, len);
  *n_bytes += len;
}

ساكن مضمن فراغ
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

ساكن مضمن تعداد field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
                     bool *have_pending_سطر, bool write_field,
                     idx_t *n_bytes)
{
  إذا (parser->trim_outer_whitespace && parser->at_سطر_start)
    {
      تعداد field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_سطر, false);
      إذا (terminator != FIELD_DATA)
        أرجع terminator;
    }

  parser->at_سطر_start = false;

  طالما (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      إذا (g.ch == MBBUF_EOF)
        أرجع FIELD_EOF;

      *have_pending_سطر = true;

      إذا (g.ch == سطر_delim)
        أرجع FIELD_سطر_DELIMITER;

      إذا (mcel_isblank (g))
        أرجع skip_whitespace_run (mbbuf, parser, have_pending_سطر, true);

      إذا (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      وإلا إذا (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

ساكن مضمن تعداد field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_سطر,
                     bool write_field, idx_t *n_bytes)
{
  طالما (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      إذا (g.ch == MBBUF_EOF)
        أرجع FIELD_EOF;

      *have_pending_سطر = true;

      إذا (g.ch == سطر_delim)
        أرجع FIELD_سطر_DELIMITER;

      إذا (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        أرجع FIELD_DELIMITER;

      إذا (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      وإلا إذا (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

ساكن مضمن تعداد field_terminator
scan_mb_field (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
               bool *have_pending_سطر, bool write_field, idx_t *n_bytes)
{
  أرجع (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_سطر,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_سطر, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

ساكن تعداد field_terminator
scan_mb_سطر_end (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
                  bool *have_pending_سطر)
{
  إذا (parser->have_saved)
    {
      parser->have_saved = false;
      إذا (parser->saved_g.ch == MBBUF_EOF)
        أرجع FIELD_EOF;

      *have_pending_سطر = true;
      إذا (parser->saved_g.ch == سطر_delim)
        أرجع FIELD_سطر_DELIMITER;
    }

  طالما (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      إذا (available == 0)
        أرجع FIELD_EOF;

      حرف *buffer = mbbuf->buffer + mbbuf->offset;
      حرف *سطر_end = تلاش_bytes (buffer, سطر_delim, available);
      idx_t n = سطر_end ? سطر_end - buffer + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_سطر = true;

      إذا (سطر_end)
        أرجع FIELD_سطر_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
ساكن حرف *
find_field_delim (حرف *buf, size_t len)
{
  إذا (len < delim_mcel.len)
    أرجع عدم;

#إذا ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  أرجع memmem (buf, len, delim_bytes, delim_mcel.len);
#وإلا
  إذا (delim_mcel.len == 1)
    أرجع memchr (buf, delim_bytes[0], len);

  حرف ثابت *p = buf;
  حرف ثابت *end = buf + len;

  حرف saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  طالما (p < end)
    {
      حرف ثابت *nul = memchr (p, '\0', end - p);
      إذا (!nul)
        {
          حرف ثابت *match = strstr (p, delim_bytes);
          buf[len] = saved;
          أرجع (حرف *) match;
        }

      إذا (p < nul)
        {
          حرف ثابت *match = strstr (p, delim_bytes);
          إذا (match)
            {
              buf[len] = saved;
              أرجع (حرف *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  أرجع عدم;
#endif
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
ساكن حرف *
find_c_blank (حرف *buf, size_t len)
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

ATTRIBUTE_PURE
ساكن idx_t
field_delim_overlap (حرف ثابت *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  طالما (0 < overlap)
    {
      إذا (memeq (buf + len - overlap, delim_bytes, overlap))
        أرجع overlap;
      overlap--;
    }

  أرجع 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

ساكن مضمن تعداد field_terminator
find_field_terminator (حرف *buf, idx_t len,
                       بنية bytesearch_context *ctx, حرف **terminator)
{
  إذا (ctx->mode == BYTESEARCH_سطر_ONLY)
    {
      إذا (!ctx->سطر_end_known)
        {
          ctx->سطر_end = memchr (buf, سطر_delim, len);
          ctx->سطر_end_known = true;
        }

      *terminator = ctx->سطر_end;
      أرجع *terminator ? FIELD_سطر_DELIMITER : FIELD_DATA;
    }

  إذا (field_delim_is_سطر_delim ())
    {
      *terminator = memchr (buf, سطر_delim, len);
      أرجع *terminator ? FIELD_DELIMITER : FIELD_DATA;
    }

  إذا (!ctx->سطر_end_known)
    {
      ctx->سطر_end = memchr (buf, سطر_delim, len);
      ctx->سطر_end_known = true;
    }

  idx_t field_len = ctx->سطر_end ? ctx->سطر_end - buf : len;

  حرف *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  إذا (field_end)
    {
      *terminator = field_end;
      أرجع FIELD_DELIMITER;
    }

  *terminator = ctx->سطر_end;
  أرجع ctx->سطر_end ? FIELD_سطر_DELIMITER : FIELD_DATA;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

ساكن مضمن فراغ
maybe_write_سطر_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  إذا (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_سطر_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

ساكن مضمن bool
begin_field_output (uintmax_t field_idx, bool buffer_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  إذا (write_field && ! (field_idx == 1 && buffer_first_field))
    {
      إذا (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *found_any_selected_field = true;
    }

  أرجع write_field;
}

ساكن مضمن bool
handle_field_1 (uintmax_t field_idx, bool buffer_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  إذا (field_idx == 1 && buffer_first_field)
    {
      إذا ((field_1_selected = print_kth (1)))
        write_bytes (field_1_buffer, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  أرجع field_1_selected;
}

ساكن مضمن فراغ
handle_field_delimiter (uintmax_t *field_idx, bool buffer_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  إذا (handle_field_1 (*field_idx, buffer_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  next_item (field_idx);
  *write_field = begin_field_output (*field_idx, buffer_first_field,
                                     found_any_selected_field);
}

ساكن مضمن bool
field_selection_exhausted (uintmax_t field_idx)
{
  أرجع !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

ساكن مضمن فراغ
finish_current_سطر (uintmax_t field_idx, bool buffer_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool سطر_terminated)
{
  إذا (field_idx == 1 && buffer_first_field)
    {
      إذا (!suppress_non_delimited
          && (سطر_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_buffer, *field_1_n_bytes);
          write_سطر_delim ();
        }
      *field_1_n_bytes = 0;
    }
  وإلا
    maybe_write_سطر_delim (found_any_selected_field, field_idx);
}

ساكن مضمن فراغ
sync_byte_selection (uintmax_t byte_idx)
{
  طالما (current_rp->hi <= byte_idx)
    current_rp++;
}

ساكن مضمن فراغ
reset_field_سطر (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_سطر, بنية mbfield_parser *parser)
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

ساكن فراغ
cut_bytes (FILE *stream)
{
  ساكن حرف bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  صحيح fd = fileno (stream);

  current_rp = frp;

  طالما (true)
    {
      idx_t available = اقرأ (fd, bytes_in, حجم bytes_in);
      إذا (available <= 0)
        {
          إذا (available < 0)
            fseterr (stream);
          write_pending_سطر_delim (byte_idx);
          اكسر;
        }

      idx_t processed = 0;

      طالما (processed < available)
        {
          حرف *سطر = bytes_in + processed;
          idx_t to_process = available - processed;
          حرف *سطر_end = تلاش_bytes (سطر, سطر_delim, to_process);
          حرف *end = سطر + (سطر_end ? سطر_end - سطر : to_process);
          حرف *p = سطر;

          طالما (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              إذا (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              وإلا
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
          إذا (سطر_end)
            {
              processed++;
              reset_item_سطر (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

ساكن فراغ
cut_characters_mode (FILE *stream, bool byte_mode)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  ساكن حرف bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, حجم bytes_in, stream);

  طالما (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      إذا (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          حرف *p = mbbuf.buffer + mbbuf.offset;
          حرف *end = تلاش_bytes (p, سطر_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          إذا (!end && available)
            استمر;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      إذا (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          حرف ثابت *p = mbbuf.buffer + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          حرف ثابت *end = تلاش_bytes (p, سطر_delim, n);
          إذا (end)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, end - p + 1);
              reset_item_سطر (&idx, &print_delimiter);
              استمر;
            }

          idx += u8_گنتی (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      إذا (g.ch == سطر_delim)
        reset_item_سطر (&idx, &print_delimiter);
      وإلا إذا (g.ch == MBBUF_EOF)
        {
          write_pending_سطر_delim (idx);
          اكسر;
        }
      وإلا إذا (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          لكل (idx_t i = 0; i < g.len; i++)
            {
              next_item (&idx);
              إذا (print_kth (idx))
                {
                  إذا (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_index (idx);
                    }
                }
              وإلا إذا (seen_selected)
                suffix_selected = false;
            }

          إذا (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      وإلا
        {
          next_item (&idx);
          إذا (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_index (idx),
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
    }
}

ساكن فراغ
cut_bytes_no_split (FILE *stream)
{
  cut_characters_mode (stream, true);
}

ساكن فراغ
cut_characters (FILE *stream)
{
  cut_characters_mode (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

ساكن فراغ
cut_fields_mb_any (FILE *stream, bool whitespace_mode)
{
  ساكن حرف bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  بنية mbfield_parser parser =
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
  mbbuf_init (&mbbuf, bytes_in, حجم bytes_in, stream);

  buffer_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  طالما (true)
    {
      تعداد field_terminator terminator;
      إذا (!buffer_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_سطر,
                                    write_field, عدم);
      وإلا
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_سطر,
                                    false, &field_1_n_bytes);

      إذا (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          إذا (! field_selection_exhausted (field_idx))
            استمر;

          terminator = scan_mb_سطر_end (&mbbuf, &parser,
                                         &have_pending_سطر);
        }

      إذا (terminator == FIELD_EOF && !have_pending_سطر)
        اكسر;

      finish_current_سطر (field_idx, buffer_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_سطر_DELIMITER);
      إذا (terminator == FIELD_EOF)
        اكسر;

      reset_field_سطر (&field_idx, &found_any_selected_field,
                        &have_pending_سطر, &parser);
      write_field = begin_field_output (field_idx, buffer_first_field,
                                        &found_any_selected_field);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

ساكن فراغ
cut_fields_mb (FILE *stream)
{
  cut_fields_mb_any (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

ساكن فراغ
cut_fields_bytesearch (FILE *stream)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  ساكن حرف bytes_in[IO_BUFSIZE+1];
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
  mbbuf_init (&mbbuf, bytes_in, حجم bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  بنية bytesearch_context تلاش = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&تلاش);

  طالما (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      تلاش.at_eof = mbbuf.eof;
      تلاش.سطر_end_known = false;

      إذا (pending_سطر_delim_field)
        {
          pending_سطر_delim_field = false;

          إذا (n_avail == 0)
            {
              إذا (handle_field_1 (field_idx, buffer_first_field,
                                  &field_1_n_bytes))
                next_item (&field_idx);
              maybe_write_سطر_delim (found_any_selected_field, field_idx);
              أرجع;
            }

          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          إذا (whitespace_delimited)
            skip_blank_run = true;
        }

      إذا (n_avail == 0)
        اكسر;

      حرف *chunk = mbbuf.buffer + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      إذا (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_سطر_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          حرف *last_سطر_delim = تلاش.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, سطر_delim, n_avail);
          إذا (last_سطر_delim)
            {
              idx_t n = last_سطر_delim - chunk + 1;
              إذا (! suppress_non_delimited)
                {
                  write_bytes (field_1_buffer, field_1_n_bytes);
                  write_bytes (chunk, n);
                  إذا (تلاش.at_eof && chunk[n - 1] != سطر_delim)
                    write_سطر_delim ();
                }
              field_1_n_bytes = 0;
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

          إذا (skip_blank_run)
            {
              طالما (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              إذا (processed == n_avail)
                اكسر;
              skip_blank_run = false;
            }

          إذا (field_selection_exhausted (field_idx))
            {
              إذا (field_delim_is_سطر_delim ())
                {
                  maybe_write_سطر_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  أرجع;
                }
              تلاش.mode = BYTESEARCH_سطر_ONLY;
              تعداد field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &تلاش, &terminator);
              إذا (terminator_kind == FIELD_سطر_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_سطر_delim (found_any_selected_field,
                                          field_idx);
                  اذهب reset_سطر;
                }
              processed = n_avail;
              اكسر;
            }

          تعداد field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &تلاش, &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          إذا (terminator_kind == FIELD_DATA
              && !تلاش.at_eof
              && !whitespace_delimited
              && !field_delim_is_سطر_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          إذا (field_len || terminator)
            have_pending_سطر = true;

          /* Output field content.  */
          إذا (field_idx == 1 && buffer_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          وإلا إذا (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          إذا (terminator_kind == FIELD_DATA)
            اكسر;

          إذا (terminator_kind == FIELD_DELIMITER)
            {
              إذا (field_delim_is_سطر_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_سطر_delim_field = true;
                  اكسر;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, buffer_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              إذا (whitespace_delimited)
                skip_blank_run = true;
            }
          وإلا
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
              bytesearch_context_reset (&تلاش);
              write_field = begin_field_output (field_idx, buffer_first_field,
                                                &found_any_selected_field);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  إذا (!have_pending_سطر)
    أرجع;

  finish_current_سطر (field_idx, buffer_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

ساكن فراغ
cut_fields_ws (FILE *stream)
{
  إذا (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  وإلا
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

ساكن bool
cut_file (حرف ثابت *file, فراغ (*cut_stream) (FILE *))
{
  FILE *stream;

  إذا (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  وإلا
    {
      stream = fopen (file, "r");
      إذا (stream == عدم)
        {
          error (0, errno, "%s", quotef (file));
          أرجع false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  صحيح err = errno;
  إذا (!ferror (stream))
    err = 0;
  إذا (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF.  */
  وإلا إذا (fclose (stream) == EOF)
    err = errno;
  إذا (err)
    {
      error (0, err, "%s", quotef (file));
      أرجع false;
    }
  أرجع true;
}

صحيح
main (صحيح argc, حرف **argv)
{
  صحيح optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  حرف *spec_list_string = عدم;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
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
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        حالة 'c':
          إذا (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        حالة 'f':
        حالة 'F':
          إذا (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
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
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            اكسر;
          }

        حالة 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          اكسر;

        حالة 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_length = (optarg[0] == '\0'
                                     ? 1 : strlen (optarg));
          output_delimiter_string = optarg;
          اكسر;

        حالة 'n':
          no_split = true;
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
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  إذا (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
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

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  إذا (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  إذا (output_delimiter_string == عدم)
    {
      output_delimiter_string = output_delimiter_default;
      إذا (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_length = 1;
        }
      وإلا
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_length = delim_mcel.len;
        }
    }

  فراغ (*cut_stream) (FILE *) = عدم;
  بدل (cut_mode)
    {
    حالة CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      اكسر;

    حالة CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      اكسر;

    حالة CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      اكسر;

    حالة CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  إذا (optind == argc)
    ok = cut_file ("-", cut_stream);
  وإلا
    لكل (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  إذا (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
