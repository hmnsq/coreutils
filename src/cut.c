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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "argmatch.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "ioblksize.h"
#અન્તર્ભાવ "mbbuf.h"
#અન્તર્ભાવ "memchr2.h"

#અન્તર્ભાવ "set-fields.h"
#અન્તર્ભાવ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cut"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#define FATAL_ERROR(Message)						\
  કરો									\
    {									\
      error (0, 0, (Message));						\
      usage (EXIT_FAILURE);						\
    }									\
  જ્યારે (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#define SMALL_BYTE_THRESHOLD 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
સ્થિર સંરચના field_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
સ્થિર અક્ષર *field_1_buffer;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
સ્થિર idx_t field_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
સ્થિર bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
સ્થિર bool complement;

/* The delimiter character for multibyte field mode.  */
સ્થિર mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
સ્થિર અક્ષર delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
સ્થિર unsigned અક્ષર લીટી_delim = '\n';

/* The length of output_delimiter_string.  */
સ્થિર size_t output_delimiter_length;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
સ્થિર અક્ષર *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
સ્થિર અક્ષર output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
સ્થિર bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
સ્થિર bool no_split;

/* If true, interpret each run of whitespace as one field delimiter.  */
સ્થિર bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
સ્થિર bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
સ્થિર bool space_output_delimiter_default;

ગણના whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

સ્થિર અક્ષર અચલ *અચલ whitespace_option_args[] =
{
  "trimmed", ખાલી
};

સ્થિર ગણના whitespace_option અચલ whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_args, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
સ્થિર ગણના
{
  CUT_MODE_NONE,
  CUT_MODE_BYTES,
  CUT_MODE_CHARACTERS,
  CUT_MODE_FIELDS
} cut_mode = CUT_MODE_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"bytes", required_argument, ખાલી, 'b'},
  {"characters", required_argument, ખાલી, 'c'},
  {"fields", required_argument, ખાલી, 'f'},
  {"delimiter", required_argument, ખાલી, 'd'},
  {"no-partial", no_argument, ખાલી, 'n'},
  {"whitespace-delimited", optional_argument, ખાલી, 'w'},
  {"only-delimited", no_argument, ખાલી, 's'},
  {"output-delimiter", required_argument, ખાલી, 'O'},
  {"complement", no_argument, ખાલી, COMPLEMENT_OPTION},
  {"zero-terminated", no_argument, ખાલી, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Print selected parts of લીટીઓ from each FILE to standard output.\n\
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
         use DELIM instead of TAB માટે field delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any લીટી that contains\n\
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
         the મૂળભૂત is to use the આગત delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         કરો not print લીટીઓ not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a run of blank characters as the field delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         લીટી delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one of -b, -c, -f or -F.  Each LIST is made up of one\n\
range, or many ranges separated by commas.  Selected આગત is written\n\
in the same order that it is પઠ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one of:\n\
\n\
  N     N'th byte, character or field, counted from 1\n\
  N-    from N'th byte, character or field, to end of લીટી\n\
  N-M   from N'th to M'th (included) byte, character or field\n\
  -M    from first to M'th (included) byte, character or field\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

સ્થિર અંતઃરેખ શૂન્ય
next_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  જો ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

સ્થિર અંતઃરેખ bool
print_kth (uintmax_t k)
{
  પાછા current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

સ્થિર અંતઃરેખ bool
is_range_start_index (uintmax_t k)
{
  પાછા k == current_rp->lo;
}

સ્થિર અંતઃરેખ bool
field_delim_is_લીટી_delim (શૂન્ય)
{
  પાછા delim_mcel.len == 1 && delim_bytes[0] == લીટી_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

સ્થિર bool
mcel_isblank (mcel_t g)
{
  પાછા (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

સ્થિર અંતઃરેખ bool
bytesearch_field_delim_ok (શૂન્ય)
{
  unsigned અક્ષર delim_0 = delim_bytes[0];

  પાછા (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


ગણના field_terminator
{
  FIELD_DATA,
  FIELD_DELIMITER,
  FIELD_લીટી_DELIMITER,
  FIELD_EOF
};

ગણના bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_લીટી_ONLY
};

સંરચના bytesearch_context
{
  ગણના bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  અક્ષર *લીટી_end;
  bool લીટી_end_known;
};

સ્થિર અંતઃરેખ શૂન્ય
bytesearch_context_reset (સંરચના bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->લીટી_end = ખાલી;
  ctx->લીટી_end_known = false;
}

સંરચના mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_લીટી_start;
  bool have_saved;
  mcel_t saved_g;
};

સ્થિર અંતઃરેખ mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  જો (*have_saved)
    {
      *have_saved = false;
      પાછા *saved_g;
    }
  પાછા mbbuf_get_char (mbbuf);
}

સ્થિર અંતઃરેખ ગણના field_terminator
skip_whitespace_run (mbbuf_t *mbuf, સંરચના mbfield_parser *parser,
                     bool *have_pending_લીટી,
                     bool have_initial_whitespace)
{
  mcel_t g;

  કરો
    {
      g = mbbuf_get_char (mbuf);
      જો (g.ch != MBBUF_EOF)
        *have_pending_લીટી = true;
    }
  જ્યારે (g.ch != MBBUF_EOF && g.ch != લીટી_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_લીટી_start;

  જો (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == લીટી_delim))
    {
      પાછા g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_લીટી_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  પાછા trim_start && !have_initial_whitespace ? FIELD_DATA : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

સ્થિર શૂન્ય
write_bytes (અક્ષર અચલ *buf, size_t n_bytes)
{
  જો (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      માટે (size_t i = 0; i < n_bytes; i++)
        જો (putchar (buf[i]) < 0)
          write_error ();
      પાછા;
    }

  જો (fwrite (buf, કદ (અક્ષર), n_bytes, stdout) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

સ્થિર અંતઃરેખ શૂન્ય
copy_bytes (અક્ષર *dst, અક્ષર અચલ *src, size_t n_bytes)
{
  જો (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      માટે (size_t i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      પાછા;
    }

  memcpy (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

સ્થિર અંતઃરેખ શૂન્ય*
શોધ_bytes (અક્ષર અચલ *buf, પૂર્ણાંક c, size_t n_bytes)
{
  જો (n_bytes != 0 && to_uchar (buf[0]) == c)
    પાછા (અક્ષર *) buf;
  જો (1 < n_bytes && to_uchar (buf[1]) == c)
    પાછા (અક્ષર *) buf + 1;

  પાછા 2 < n_bytes ? (અક્ષર *) memchr (buf + 2, c, n_bytes - 2) : ખાલી;
}

સ્થિર અંતઃરેખ શૂન્ય
write_લીટી_delim (શૂન્ય)
{
  જો (putchar (લીટી_delim) < 0)
    write_error ();
}

સ્થિર અંતઃરેખ શૂન્ય
reset_item_લીટી (uintmax_t *item_idx, bool *print_delimiter)
{
  write_લીટી_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

સ્થિર અંતઃરેખ શૂન્ય
write_pending_લીટી_delim (uintmax_t item_idx)
{
  જો (item_idx > 0)
    write_લીટી_delim ();
}

સ્થિર અંતઃરેખ શૂન્ય
write_selected_item (bool *print_delimiter, bool range_start,
                     અક્ષર અચલ *buf, size_t n_bytes)
{
  જો (output_delimiter_string != output_delimiter_default)
    {
      જો (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

સ્થિર અંતઃરેખ શૂન્ય
append_field_1_chunk (અક્ષર અચલ *buf, idx_t len, idx_t *n_bytes)
{
  જો (field_1_bufsize - *n_bytes < len)
    {
      field_1_buffer = xpalloc (field_1_buffer, &field_1_bufsize,
                                len, -1, કદ *field_1_buffer);
    }

  copy_bytes (field_1_buffer + *n_bytes, buf, len);
  *n_bytes += len;
}

સ્થિર અંતઃરેખ શૂન્ય
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

સ્થિર અંતઃરેખ ગણના field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, સંરચના mbfield_parser *parser,
                     bool *have_pending_લીટી, bool write_field,
                     idx_t *n_bytes)
{
  જો (parser->trim_outer_whitespace && parser->at_લીટી_start)
    {
      ગણના field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_લીટી, false);
      જો (terminator != FIELD_DATA)
        પાછા terminator;
    }

  parser->at_લીટી_start = false;

  જ્યારે (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      જો (g.ch == MBBUF_EOF)
        પાછા FIELD_EOF;

      *have_pending_લીટી = true;

      જો (g.ch == લીટી_delim)
        પાછા FIELD_લીટી_DELIMITER;

      જો (mcel_isblank (g))
        પાછા skip_whitespace_run (mbbuf, parser, have_pending_લીટી, true);

      જો (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      નહીંતર જો (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

સ્થિર અંતઃરેખ ગણના field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_લીટી,
                     bool write_field, idx_t *n_bytes)
{
  જ્યારે (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      જો (g.ch == MBBUF_EOF)
        પાછા FIELD_EOF;

      *have_pending_લીટી = true;

      જો (g.ch == લીટી_delim)
        પાછા FIELD_લીટી_DELIMITER;

      જો (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        પાછા FIELD_DELIMITER;

      જો (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      નહીંતર જો (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

સ્થિર અંતઃરેખ ગણના field_terminator
scan_mb_field (mbbuf_t *mbbuf, સંરચના mbfield_parser *parser,
               bool *have_pending_લીટી, bool write_field, idx_t *n_bytes)
{
  પાછા (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_લીટી,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_લીટી, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

સ્થિર ગણના field_terminator
scan_mb_લીટી_end (mbbuf_t *mbbuf, સંરચના mbfield_parser *parser,
                  bool *have_pending_લીટી)
{
  જો (parser->have_saved)
    {
      parser->have_saved = false;
      જો (parser->saved_g.ch == MBBUF_EOF)
        પાછા FIELD_EOF;

      *have_pending_લીટી = true;
      જો (parser->saved_g.ch == લીટી_delim)
        પાછા FIELD_લીટી_DELIMITER;
    }

  જ્યારે (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      જો (available == 0)
        પાછા FIELD_EOF;

      અક્ષર *buffer = mbbuf->buffer + mbbuf->offset;
      અક્ષર *લીટી_end = શોધ_bytes (buffer, લીટી_delim, available);
      idx_t n = લીટી_end ? લીટી_end - buffer + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_લીટી = true;

      જો (લીટી_end)
        પાછા FIELD_લીટી_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
સ્થિર અક્ષર *
find_field_delim (અક્ષર *buf, size_t len)
{
  જો (len < delim_mcel.len)
    પાછા ખાલી;

#જો ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  પાછા memmem (buf, len, delim_bytes, delim_mcel.len);
#નહીંતર
  જો (delim_mcel.len == 1)
    પાછા memchr (buf, delim_bytes[0], len);

  અક્ષર અચલ *p = buf;
  અક્ષર અચલ *end = buf + len;

  અક્ષર saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  જ્યારે (p < end)
    {
      અક્ષર અચલ *nul = memchr (p, '\0', end - p);
      જો (!nul)
        {
          અક્ષર અચલ *match = strstr (p, delim_bytes);
          buf[len] = saved;
          પાછા (અક્ષર *) match;
        }

      જો (p < nul)
        {
          અક્ષર અચલ *match = strstr (p, delim_bytes);
          જો (match)
            {
              buf[len] = saved;
              પાછા (અક્ષર *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  પાછા ખાલી;
#endif
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
સ્થિર અક્ષર *
find_c_blank (અક્ષર *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  માટે (idx_t i = 0; i < n; i++)
    જો (c_isblank (buf[i]))
      પાછા buf + i;

  પાછા memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_PURE
સ્થિર idx_t
field_delim_overlap (અક્ષર અચલ *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  જ્યારે (0 < overlap)
    {
      જો (memeq (buf + len - overlap, delim_bytes, overlap))
        પાછા overlap;
      overlap--;
    }

  પાછા 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

સ્થિર અંતઃરેખ ગણના field_terminator
find_field_terminator (અક્ષર *buf, idx_t len,
                       સંરચના bytesearch_context *ctx, અક્ષર **terminator)
{
  જો (ctx->mode == BYTESEARCH_લીટી_ONLY)
    {
      જો (!ctx->લીટી_end_known)
        {
          ctx->લીટી_end = memchr (buf, લીટી_delim, len);
          ctx->લીટી_end_known = true;
        }

      *terminator = ctx->લીટી_end;
      પાછા *terminator ? FIELD_લીટી_DELIMITER : FIELD_DATA;
    }

  જો (field_delim_is_લીટી_delim ())
    {
      *terminator = memchr (buf, લીટી_delim, len);
      પાછા *terminator ? FIELD_DELIMITER : FIELD_DATA;
    }

  જો (!ctx->લીટી_end_known)
    {
      ctx->લીટી_end = memchr (buf, લીટી_delim, len);
      ctx->લીટી_end_known = true;
    }

  idx_t field_len = ctx->લીટી_end ? ctx->લીટી_end - buf : len;

  અક્ષર *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  જો (field_end)
    {
      *terminator = field_end;
      પાછા FIELD_DELIMITER;
    }

  *terminator = ctx->લીટી_end;
  પાછા ctx->લીટી_end ? FIELD_લીટી_DELIMITER : FIELD_DATA;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

સ્થિર અંતઃરેખ શૂન્ય
maybe_write_લીટી_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  જો (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_લીટી_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

સ્થિર અંતઃરેખ bool
begin_field_output (uintmax_t field_idx, bool buffer_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  જો (write_field && ! (field_idx == 1 && buffer_first_field))
    {
      જો (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *found_any_selected_field = true;
    }

  પાછા write_field;
}

સ્થિર અંતઃરેખ bool
handle_field_1 (uintmax_t field_idx, bool buffer_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  જો (field_idx == 1 && buffer_first_field)
    {
      જો ((field_1_selected = print_kth (1)))
        write_bytes (field_1_buffer, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  પાછા field_1_selected;
}

સ્થિર અંતઃરેખ શૂન્ય
handle_field_delimiter (uintmax_t *field_idx, bool buffer_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  જો (handle_field_1 (*field_idx, buffer_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  next_item (field_idx);
  *write_field = begin_field_output (*field_idx, buffer_first_field,
                                     found_any_selected_field);
}

સ્થિર અંતઃરેખ bool
field_selection_exhausted (uintmax_t field_idx)
{
  પાછા !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

સ્થિર અંતઃરેખ શૂન્ય
finish_current_લીટી (uintmax_t field_idx, bool buffer_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool લીટી_terminated)
{
  જો (field_idx == 1 && buffer_first_field)
    {
      જો (!suppress_non_delimited
          && (લીટી_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_buffer, *field_1_n_bytes);
          write_લીટી_delim ();
        }
      *field_1_n_bytes = 0;
    }
  નહીંતર
    maybe_write_લીટી_delim (found_any_selected_field, field_idx);
}

સ્થિર અંતઃરેખ શૂન્ય
sync_byte_selection (uintmax_t byte_idx)
{
  જ્યારે (current_rp->hi <= byte_idx)
    current_rp++;
}

સ્થિર અંતઃરેખ શૂન્ય
reset_field_લીટી (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_લીટી, સંરચના mbfield_parser *parser)
{
  *field_idx = 1;
  current_rp = frp;
  *found_any_selected_field = false;
  *have_pending_લીટી = false;
  parser->have_saved = false;
  parser->at_લીટી_start = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

સ્થિર શૂન્ય
cut_bytes (FILE *stream)
{
  સ્થિર અક્ષર bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  પૂર્ણાંક fd = fileno (stream);

  current_rp = frp;

  જ્યારે (true)
    {
      idx_t available = પઠ (fd, bytes_in, કદ bytes_in);
      જો (available <= 0)
        {
          જો (available < 0)
            fseterr (stream);
          write_pending_લીટી_delim (byte_idx);
          તોડો;
        }

      idx_t processed = 0;

      જ્યારે (processed < available)
        {
          અક્ષર *લીટી = bytes_in + processed;
          idx_t to_process = available - processed;
          અક્ષર *લીટી_end = શોધ_bytes (લીટી, લીટી_delim, to_process);
          અક્ષર *end = લીટી + (લીટી_end ? લીટી_end - લીટી : to_process);
          અક્ષર *p = લીટી;

          જ્યારે (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              જો (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              નહીંતર
                {
                  idx_t n = MIN (end - p, current_rp->hi - byte_idx);
                  write_selected_item (&print_delimiter,
                                       is_range_start_index (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += end - લીટી;
          જો (લીટી_end)
            {
              processed++;
              reset_item_લીટી (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

સ્થિર શૂન્ય
cut_characters_mode (FILE *stream, bool byte_mode)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  સ્થિર અક્ષર bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, કદ bytes_in, stream);

  જ્યારે (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      જો (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          અક્ષર *p = mbbuf.buffer + mbbuf.offset;
          અક્ષર *end = શોધ_bytes (p, લીટી_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          જો (!end && available)
            ચાલુ;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      જો (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          અક્ષર અચલ *p = mbbuf.buffer + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          અક્ષર અચલ *end = શોધ_bytes (p, લીટી_delim, n);
          જો (end)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, end - p + 1);
              reset_item_લીટી (&idx, &print_delimiter);
              ચાલુ;
            }

          idx += u8_ગણતરી (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      જો (g.ch == લીટી_delim)
        reset_item_લીટી (&idx, &print_delimiter);
      નહીંતર જો (g.ch == MBBUF_EOF)
        {
          write_pending_લીટી_delim (idx);
          તોડો;
        }
      નહીંતર જો (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          માટે (idx_t i = 0; i < g.len; i++)
            {
              next_item (&idx);
              જો (print_kth (idx))
                {
                  જો (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_index (idx);
                    }
                }
              નહીંતર જો (seen_selected)
                suffix_selected = false;
            }

          જો (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      નહીંતર
        {
          next_item (&idx);
          જો (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_index (idx),
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
    }
}

સ્થિર શૂન્ય
cut_bytes_no_split (FILE *stream)
{
  cut_characters_mode (stream, true);
}

સ્થિર શૂન્ય
cut_characters (FILE *stream)
{
  cut_characters_mode (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

સ્થિર શૂન્ય
cut_fields_mb_any (FILE *stream, bool whitespace_mode)
{
  સ્થિર અક્ષર bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  સંરચના mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_mode,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_લીટી_start = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool buffer_first_field;
  bool have_pending_લીટી = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, કદ bytes_in, stream);

  buffer_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  જ્યારે (true)
    {
      ગણના field_terminator terminator;
      જો (!buffer_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_લીટી,
                                    write_field, ખાલી);
      નહીંતર
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_લીટી,
                                    false, &field_1_n_bytes);

      જો (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          જો (! field_selection_exhausted (field_idx))
            ચાલુ;

          terminator = scan_mb_લીટી_end (&mbbuf, &parser,
                                         &have_pending_લીટી);
        }

      જો (terminator == FIELD_EOF && !have_pending_લીટી)
        તોડો;

      finish_current_લીટી (field_idx, buffer_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_લીટી_DELIMITER);
      જો (terminator == FIELD_EOF)
        તોડો;

      reset_field_લીટી (&field_idx, &found_any_selected_field,
                        &have_pending_લીટી, &parser);
      write_field = begin_field_output (field_idx, buffer_first_field,
                                        &found_any_selected_field);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

સ્થિર શૂન્ય
cut_fields_mb (FILE *stream)
{
  cut_fields_mb_any (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

સ્થિર શૂન્ય
cut_fields_bytesearch (FILE *stream)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  સ્થિર અક્ષર bytes_in[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool have_pending_લીટી = false;
  bool skip_blank_run = false;
  bool pending_લીટી_delim_field = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  bool buffer_first_field = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, bytes_in, કદ bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  સંરચના bytesearch_context શોધ = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&શોધ);

  જ્યારે (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      શોધ.at_eof = mbbuf.eof;
      શોધ.લીટી_end_known = false;

      જો (pending_લીટી_delim_field)
        {
          pending_લીટી_delim_field = false;

          જો (n_avail == 0)
            {
              જો (handle_field_1 (field_idx, buffer_first_field,
                                  &field_1_n_bytes))
                next_item (&field_idx);
              maybe_write_લીટી_delim (found_any_selected_field, field_idx);
              પાછા;
            }

          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          જો (whitespace_delimited)
            skip_blank_run = true;
        }

      જો (n_avail == 0)
        તોડો;

      અક્ષર *chunk = mbbuf.buffer + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      જો (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_લીટી_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          અક્ષર *last_લીટી_delim = શોધ.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, લીટી_delim, n_avail);
          જો (last_લીટી_delim)
            {
              idx_t n = last_લીટી_delim - chunk + 1;
              જો (! suppress_non_delimited)
                {
                  write_bytes (field_1_buffer, field_1_n_bytes);
                  write_bytes (chunk, n);
                  જો (શોધ.at_eof && chunk[n - 1] != લીટી_delim)
                    write_લીટી_delim ();
                }
              field_1_n_bytes = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_લીટી = false;
              જો (શોધ.at_eof)
                પાછા;
              ચાલુ;
            }
        }

      જ્યારે (processed < n_avail)
        {
          અક્ષર *terminator = ખાલી;

          જો (skip_blank_run)
            {
              જ્યારે (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              જો (processed == n_avail)
                તોડો;
              skip_blank_run = false;
            }

          જો (field_selection_exhausted (field_idx))
            {
              જો (field_delim_is_લીટી_delim ())
                {
                  maybe_write_લીટી_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  પાછા;
                }
              શોધ.mode = BYTESEARCH_લીટી_ONLY;
              ગણના field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &શોધ, &terminator);
              જો (terminator_kind == FIELD_લીટી_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_લીટી_delim (found_any_selected_field,
                                          field_idx);
                  જાઓ reset_લીટી;
                }
              processed = n_avail;
              તોડો;
            }

          ગણના field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &શોધ, &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          જો (terminator_kind == FIELD_DATA
              && !શોધ.at_eof
              && !whitespace_delimited
              && !field_delim_is_લીટી_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          જો (field_len || terminator)
            have_pending_લીટી = true;

          /* Output field content.  */
          જો (field_idx == 1 && buffer_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          નહીંતર જો (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          જો (terminator_kind == FIELD_DATA)
            તોડો;

          જો (terminator_kind == FIELD_DELIMITER)
            {
              જો (field_delim_is_લીટી_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_લીટી_delim_field = true;
                  તોડો;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, buffer_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              જો (whitespace_delimited)
                skip_blank_run = true;
            }
          નહીંતર
            {
              affirm (terminator_kind == FIELD_લીટી_DELIMITER);
              processed++;
              finish_current_લીટી (field_idx, buffer_first_field,
                                   &field_1_n_bytes,
                                   found_any_selected_field, true);

            reset_લીટી:
              field_idx = 1;
              current_rp = frp;
              found_any_selected_field = false;
              have_pending_લીટી = false;
              bytesearch_context_reset (&શોધ);
              write_field = begin_field_output (field_idx, buffer_first_field,
                                                &found_any_selected_field);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  જો (!have_pending_લીટી)
    પાછા;

  finish_current_લીટી (field_idx, buffer_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

સ્થિર શૂન્ય
cut_fields_ws (FILE *stream)
{
  જો (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  નહીંતર
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

સ્થિર bool
cut_file (અક્ષર અચલ *file, શૂન્ય (*cut_stream) (FILE *))
{
  FILE *stream;

  જો (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  નહીંતર
    {
      stream = fopen (file, "r");
      જો (stream == ખાલી)
        {
          error (0, errno, "%s", quotef (file));
          પાછા false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  પૂર્ણાંક err = errno;
  જો (!ferror (stream))
    err = 0;
  જો (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF.  */
  નહીંતર જો (fclose (stream) == EOF)
    err = errno;
  જો (err)
    {
      error (0, err, "%s", quotef (file));
      પાછા false;
    }
  પાછા true;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  અક્ષર *spec_list_string = ખાલી;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, ખાલી))
         != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'b':
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        સ્થિતિ 'c':
          જો (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        સ્થિતિ 'f':
        સ્થિતિ 'F':
          જો (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
          જો (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          જો (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          તોડો;

        સ્થિતિ 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            જો (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            તોડો;
          }

        સ્થિતિ 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          તોડો;

        સ્થિતિ 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_length = (optarg[0] == '\0'
                                     ? 1 : strlen (optarg));
          output_delimiter_string = optarg;
          તોડો;

        સ્થિતિ 'n':
          no_split = true;
          તોડો;

        સ્થિતિ 's':
          suppress_non_delimited = true;
          તોડો;

        સ્થિતિ 'z':
          લીટી_delim = '\0';
          તોડો;

        સ્થિતિ COMPLEMENT_OPTION:
          complement = true;
          તોડો;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  જો (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
    {
      જો (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an આગત delimiter makes sense\n\
\tonly when operating on fields"));

      જો (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited લીટીઓ makes sense\n\
\tonly when operating on fields"));
    }

  જો (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  જો (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  જો (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  જો (output_delimiter_string == ખાલી)
    {
      output_delimiter_string = output_delimiter_default;
      જો (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_length = 1;
        }
      નહીંતર
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_length = delim_mcel.len;
        }
    }

  શૂન્ય (*cut_stream) (FILE *) = ખાલી;
  પસંદગી (cut_mode)
    {
    સ્થિતિ CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      તોડો;

    સ્થિતિ CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      તોડો;

    સ્થિતિ CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      તોડો;

    સ્થિતિ CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  જો (optind == argc)
    ok = cut_file ("-", cut_stream);
  નહીંતર
    માટે (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  જો (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
