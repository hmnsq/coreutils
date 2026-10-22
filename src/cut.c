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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "argmatch.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "ioblksize.h"
#அந்தர்பாவ "mbbuf.h"
#அந்தர்பாவ "memchr2.h"

#அந்தர்பாவ "set-fields.h"
#அந்தர்பாவ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cut"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#define FATAL_ERROR(Message)						\
  செய்									\
    {									\
      error (0, 0, (Message));						\
      usage (EXIT_FAILURE);						\
    }									\
  போது (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#define SMALL_BYTE_THRESHOLD 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
நிலையான கட்டமைப்பு field_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
நிலையான எழுத்து *field_1_buffer;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
நிலையான idx_t field_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
நிலையான bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
நிலையான bool complement;

/* The delimiter character for multibyte field mode.  */
நிலையான mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
நிலையான எழுத்து delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
நிலையான unsigned எழுத்து வரி_delim = '\n';

/* The length of output_delimiter_string.  */
நிலையான size_t output_delimiter_length;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
நிலையான எழுத்து *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
நிலையான எழுத்து output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
நிலையான bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
நிலையான bool no_split;

/* If true, interpret each run of whitespace as one field delimiter.  */
நிலையான bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
நிலையான bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
நிலையான bool space_output_delimiter_default;

எண்ணிக்கை whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

நிலையான எழுத்து மாறிலி *மாறிலி whitespace_option_args[] =
{
  "trimmed", வெற்று
};

நிலையான எண்ணிக்கை whitespace_option மாறிலி whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_args, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
நிலையான எண்ணிக்கை
{
  CUT_MODE_NONE,
  CUT_MODE_BYTES,
  CUT_MODE_CHARACTERS,
  CUT_MODE_FIELDS
} cut_mode = CUT_MODE_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"bytes", required_argument, வெற்று, 'b'},
  {"characters", required_argument, வெற்று, 'c'},
  {"fields", required_argument, வெற்று, 'f'},
  {"delimiter", required_argument, வெற்று, 'd'},
  {"no-partial", no_argument, வெற்று, 'n'},
  {"whitespace-delimited", optional_argument, வெற்று, 'w'},
  {"only-delimited", no_argument, வெற்று, 's'},
  {"output-delimiter", required_argument, வெற்று, 'O'},
  {"complement", no_argument, வெற்று, COMPLEMENT_OPTION},
  {"zero-terminated", no_argument, வெற்று, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Print selected parts of வரிகள் from each FILE to standard output.\n\
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
         use DELIM instead of TAB க்கு field delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any வரி that contains\n\
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
         the இயல்பு is to use the உள்ளீடு delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         செய் not print வரிகள் not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a run of blank characters as the field delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         வரி delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one of -b, -c, -f or -F.  Each LIST is made up of one\n\
range, or many ranges separated by commas.  Selected உள்ளீடு is written\n\
in the same order that it is பட, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one of:\n\
\n\
  N     N'th byte, character or field, counted from 1\n\
  N-    from N'th byte, character or field, to end of வரி\n\
  N-M   from N'th to M'th (included) byte, character or field\n\
  -M    from first to M'th (included) byte, character or field\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

நிலையான உள்வரி வெறுமை
next_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  எனில் ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

நிலையான உள்வரி bool
print_kth (uintmax_t k)
{
  திரும்பு current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

நிலையான உள்வரி bool
is_range_start_index (uintmax_t k)
{
  திரும்பு k == current_rp->lo;
}

நிலையான உள்வரி bool
field_delim_is_வரி_delim (வெறுமை)
{
  திரும்பு delim_mcel.len == 1 && delim_bytes[0] == வரி_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

நிலையான bool
mcel_isblank (mcel_t g)
{
  திரும்பு (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

நிலையான உள்வரி bool
bytesearch_field_delim_ok (வெறுமை)
{
  unsigned எழுத்து delim_0 = delim_bytes[0];

  திரும்பு (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


எண்ணிக்கை field_terminator
{
  FIELD_DATA,
  FIELD_DELIMITER,
  FIELD_வரி_DELIMITER,
  FIELD_EOF
};

எண்ணிக்கை bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_வரி_ONLY
};

கட்டமைப்பு bytesearch_context
{
  எண்ணிக்கை bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  எழுத்து *வரி_end;
  bool வரி_end_known;
};

நிலையான உள்வரி வெறுமை
bytesearch_context_reset (கட்டமைப்பு bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->வரி_end = வெற்று;
  ctx->வரி_end_known = false;
}

கட்டமைப்பு mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_வரி_start;
  bool have_saved;
  mcel_t saved_g;
};

நிலையான உள்வரி mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  எனில் (*have_saved)
    {
      *have_saved = false;
      திரும்பு *saved_g;
    }
  திரும்பு mbbuf_get_char (mbbuf);
}

நிலையான உள்வரி எண்ணிக்கை field_terminator
skip_whitespace_run (mbbuf_t *mbuf, கட்டமைப்பு mbfield_parser *parser,
                     bool *have_pending_வரி,
                     bool have_initial_whitespace)
{
  mcel_t g;

  செய்
    {
      g = mbbuf_get_char (mbuf);
      எனில் (g.ch != MBBUF_EOF)
        *have_pending_வரி = true;
    }
  போது (g.ch != MBBUF_EOF && g.ch != வரி_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_வரி_start;

  எனில் (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == வரி_delim))
    {
      திரும்பு g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_வரி_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  திரும்பு trim_start && !have_initial_whitespace ? FIELD_DATA : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

நிலையான வெறுமை
write_bytes (எழுத்து மாறிலி *buf, size_t n_bytes)
{
  எனில் (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      க்கு (size_t i = 0; i < n_bytes; i++)
        எனில் (putchar (buf[i]) < 0)
          write_error ();
      திரும்பு;
    }

  எனில் (fwrite (buf, அளவு (எழுத்து), n_bytes, stdout) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

நிலையான உள்வரி வெறுமை
copy_bytes (எழுத்து *dst, எழுத்து மாறிலி *src, size_t n_bytes)
{
  எனில் (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      க்கு (size_t i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      திரும்பு;
    }

  memcpy (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

நிலையான உள்வரி வெறுமை*
தேடல்_bytes (எழுத்து மாறிலி *buf, முழு c, size_t n_bytes)
{
  எனில் (n_bytes != 0 && to_uchar (buf[0]) == c)
    திரும்பு (எழுத்து *) buf;
  எனில் (1 < n_bytes && to_uchar (buf[1]) == c)
    திரும்பு (எழுத்து *) buf + 1;

  திரும்பு 2 < n_bytes ? (எழுத்து *) memchr (buf + 2, c, n_bytes - 2) : வெற்று;
}

நிலையான உள்வரி வெறுமை
write_வரி_delim (வெறுமை)
{
  எனில் (putchar (வரி_delim) < 0)
    write_error ();
}

நிலையான உள்வரி வெறுமை
reset_item_வரி (uintmax_t *item_idx, bool *print_delimiter)
{
  write_வரி_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

நிலையான உள்வரி வெறுமை
write_pending_வரி_delim (uintmax_t item_idx)
{
  எனில் (item_idx > 0)
    write_வரி_delim ();
}

நிலையான உள்வரி வெறுமை
write_selected_item (bool *print_delimiter, bool range_start,
                     எழுத்து மாறிலி *buf, size_t n_bytes)
{
  எனில் (output_delimiter_string != output_delimiter_default)
    {
      எனில் (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

நிலையான உள்வரி வெறுமை
append_field_1_chunk (எழுத்து மாறிலி *buf, idx_t len, idx_t *n_bytes)
{
  எனில் (field_1_bufsize - *n_bytes < len)
    {
      field_1_buffer = xpalloc (field_1_buffer, &field_1_bufsize,
                                len, -1, அளவு *field_1_buffer);
    }

  copy_bytes (field_1_buffer + *n_bytes, buf, len);
  *n_bytes += len;
}

நிலையான உள்வரி வெறுமை
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

நிலையான உள்வரி எண்ணிக்கை field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, கட்டமைப்பு mbfield_parser *parser,
                     bool *have_pending_வரி, bool write_field,
                     idx_t *n_bytes)
{
  எனில் (parser->trim_outer_whitespace && parser->at_வரி_start)
    {
      எண்ணிக்கை field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_வரி, false);
      எனில் (terminator != FIELD_DATA)
        திரும்பு terminator;
    }

  parser->at_வரி_start = false;

  போது (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      எனில் (g.ch == MBBUF_EOF)
        திரும்பு FIELD_EOF;

      *have_pending_வரி = true;

      எனில் (g.ch == வரி_delim)
        திரும்பு FIELD_வரி_DELIMITER;

      எனில் (mcel_isblank (g))
        திரும்பு skip_whitespace_run (mbbuf, parser, have_pending_வரி, true);

      எனில் (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      இல்லையெனில் எனில் (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

நிலையான உள்வரி எண்ணிக்கை field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_வரி,
                     bool write_field, idx_t *n_bytes)
{
  போது (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      எனில் (g.ch == MBBUF_EOF)
        திரும்பு FIELD_EOF;

      *have_pending_வரி = true;

      எனில் (g.ch == வரி_delim)
        திரும்பு FIELD_வரி_DELIMITER;

      எனில் (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        திரும்பு FIELD_DELIMITER;

      எனில் (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      இல்லையெனில் எனில் (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

நிலையான உள்வரி எண்ணிக்கை field_terminator
scan_mb_field (mbbuf_t *mbbuf, கட்டமைப்பு mbfield_parser *parser,
               bool *have_pending_வரி, bool write_field, idx_t *n_bytes)
{
  திரும்பு (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_வரி,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_வரி, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

நிலையான எண்ணிக்கை field_terminator
scan_mb_வரி_end (mbbuf_t *mbbuf, கட்டமைப்பு mbfield_parser *parser,
                  bool *have_pending_வரி)
{
  எனில் (parser->have_saved)
    {
      parser->have_saved = false;
      எனில் (parser->saved_g.ch == MBBUF_EOF)
        திரும்பு FIELD_EOF;

      *have_pending_வரி = true;
      எனில் (parser->saved_g.ch == வரி_delim)
        திரும்பு FIELD_வரி_DELIMITER;
    }

  போது (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      எனில் (available == 0)
        திரும்பு FIELD_EOF;

      எழுத்து *buffer = mbbuf->buffer + mbbuf->offset;
      எழுத்து *வரி_end = தேடல்_bytes (buffer, வரி_delim, available);
      idx_t n = வரி_end ? வரி_end - buffer + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_வரி = true;

      எனில் (வரி_end)
        திரும்பு FIELD_வரி_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
நிலையான எழுத்து *
find_field_delim (எழுத்து *buf, size_t len)
{
  எனில் (len < delim_mcel.len)
    திரும்பு வெற்று;

#எனில் ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  திரும்பு memmem (buf, len, delim_bytes, delim_mcel.len);
#இல்லையெனில்
  எனில் (delim_mcel.len == 1)
    திரும்பு memchr (buf, delim_bytes[0], len);

  எழுத்து மாறிலி *p = buf;
  எழுத்து மாறிலி *end = buf + len;

  எழுத்து saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  போது (p < end)
    {
      எழுத்து மாறிலி *nul = memchr (p, '\0', end - p);
      எனில் (!nul)
        {
          எழுத்து மாறிலி *match = strstr (p, delim_bytes);
          buf[len] = saved;
          திரும்பு (எழுத்து *) match;
        }

      எனில் (p < nul)
        {
          எழுத்து மாறிலி *match = strstr (p, delim_bytes);
          எனில் (match)
            {
              buf[len] = saved;
              திரும்பு (எழுத்து *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  திரும்பு வெற்று;
#endif
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
நிலையான எழுத்து *
find_c_blank (எழுத்து *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  க்கு (idx_t i = 0; i < n; i++)
    எனில் (c_isblank (buf[i]))
      திரும்பு buf + i;

  திரும்பு memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_PURE
நிலையான idx_t
field_delim_overlap (எழுத்து மாறிலி *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  போது (0 < overlap)
    {
      எனில் (memeq (buf + len - overlap, delim_bytes, overlap))
        திரும்பு overlap;
      overlap--;
    }

  திரும்பு 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

நிலையான உள்வரி எண்ணிக்கை field_terminator
find_field_terminator (எழுத்து *buf, idx_t len,
                       கட்டமைப்பு bytesearch_context *ctx, எழுத்து **terminator)
{
  எனில் (ctx->mode == BYTESEARCH_வரி_ONLY)
    {
      எனில் (!ctx->வரி_end_known)
        {
          ctx->வரி_end = memchr (buf, வரி_delim, len);
          ctx->வரி_end_known = true;
        }

      *terminator = ctx->வரி_end;
      திரும்பு *terminator ? FIELD_வரி_DELIMITER : FIELD_DATA;
    }

  எனில் (field_delim_is_வரி_delim ())
    {
      *terminator = memchr (buf, வரி_delim, len);
      திரும்பு *terminator ? FIELD_DELIMITER : FIELD_DATA;
    }

  எனில் (!ctx->வரி_end_known)
    {
      ctx->வரி_end = memchr (buf, வரி_delim, len);
      ctx->வரி_end_known = true;
    }

  idx_t field_len = ctx->வரி_end ? ctx->வரி_end - buf : len;

  எழுத்து *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  எனில் (field_end)
    {
      *terminator = field_end;
      திரும்பு FIELD_DELIMITER;
    }

  *terminator = ctx->வரி_end;
  திரும்பு ctx->வரி_end ? FIELD_வரி_DELIMITER : FIELD_DATA;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

நிலையான உள்வரி வெறுமை
maybe_write_வரி_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  எனில் (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_வரி_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

நிலையான உள்வரி bool
begin_field_output (uintmax_t field_idx, bool buffer_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  எனில் (write_field && ! (field_idx == 1 && buffer_first_field))
    {
      எனில் (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *found_any_selected_field = true;
    }

  திரும்பு write_field;
}

நிலையான உள்வரி bool
handle_field_1 (uintmax_t field_idx, bool buffer_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  எனில் (field_idx == 1 && buffer_first_field)
    {
      எனில் ((field_1_selected = print_kth (1)))
        write_bytes (field_1_buffer, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  திரும்பு field_1_selected;
}

நிலையான உள்வரி வெறுமை
handle_field_delimiter (uintmax_t *field_idx, bool buffer_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  எனில் (handle_field_1 (*field_idx, buffer_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  next_item (field_idx);
  *write_field = begin_field_output (*field_idx, buffer_first_field,
                                     found_any_selected_field);
}

நிலையான உள்வரி bool
field_selection_exhausted (uintmax_t field_idx)
{
  திரும்பு !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

நிலையான உள்வரி வெறுமை
finish_current_வரி (uintmax_t field_idx, bool buffer_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool வரி_terminated)
{
  எனில் (field_idx == 1 && buffer_first_field)
    {
      எனில் (!suppress_non_delimited
          && (வரி_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_buffer, *field_1_n_bytes);
          write_வரி_delim ();
        }
      *field_1_n_bytes = 0;
    }
  இல்லையெனில்
    maybe_write_வரி_delim (found_any_selected_field, field_idx);
}

நிலையான உள்வரி வெறுமை
sync_byte_selection (uintmax_t byte_idx)
{
  போது (current_rp->hi <= byte_idx)
    current_rp++;
}

நிலையான உள்வரி வெறுமை
reset_field_வரி (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_வரி, கட்டமைப்பு mbfield_parser *parser)
{
  *field_idx = 1;
  current_rp = frp;
  *found_any_selected_field = false;
  *have_pending_வரி = false;
  parser->have_saved = false;
  parser->at_வரி_start = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

நிலையான வெறுமை
cut_bytes (FILE *stream)
{
  நிலையான எழுத்து bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  முழு fd = fileno (stream);

  current_rp = frp;

  போது (true)
    {
      idx_t available = பட (fd, bytes_in, அளவு bytes_in);
      எனில் (available <= 0)
        {
          எனில் (available < 0)
            fseterr (stream);
          write_pending_வரி_delim (byte_idx);
          நிறுத்து;
        }

      idx_t processed = 0;

      போது (processed < available)
        {
          எழுத்து *வரி = bytes_in + processed;
          idx_t to_process = available - processed;
          எழுத்து *வரி_end = தேடல்_bytes (வரி, வரி_delim, to_process);
          எழுத்து *end = வரி + (வரி_end ? வரி_end - வரி : to_process);
          எழுத்து *p = வரி;

          போது (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              எனில் (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              இல்லையெனில்
                {
                  idx_t n = MIN (end - p, current_rp->hi - byte_idx);
                  write_selected_item (&print_delimiter,
                                       is_range_start_index (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += end - வரி;
          எனில் (வரி_end)
            {
              processed++;
              reset_item_வரி (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

நிலையான வெறுமை
cut_characters_mode (FILE *stream, bool byte_mode)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  நிலையான எழுத்து bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, அளவு bytes_in, stream);

  போது (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      எனில் (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          எழுத்து *p = mbbuf.buffer + mbbuf.offset;
          எழுத்து *end = தேடல்_bytes (p, வரி_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          எனில் (!end && available)
            தொடர்;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      எனில் (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          எழுத்து மாறிலி *p = mbbuf.buffer + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          எழுத்து மாறிலி *end = தேடல்_bytes (p, வரி_delim, n);
          எனில் (end)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, end - p + 1);
              reset_item_வரி (&idx, &print_delimiter);
              தொடர்;
            }

          idx += u8_மொத்தம் (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      எனில் (g.ch == வரி_delim)
        reset_item_வரி (&idx, &print_delimiter);
      இல்லையெனில் எனில் (g.ch == MBBUF_EOF)
        {
          write_pending_வரி_delim (idx);
          நிறுத்து;
        }
      இல்லையெனில் எனில் (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          க்கு (idx_t i = 0; i < g.len; i++)
            {
              next_item (&idx);
              எனில் (print_kth (idx))
                {
                  எனில் (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_index (idx);
                    }
                }
              இல்லையெனில் எனில் (seen_selected)
                suffix_selected = false;
            }

          எனில் (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      இல்லையெனில்
        {
          next_item (&idx);
          எனில் (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_index (idx),
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
    }
}

நிலையான வெறுமை
cut_bytes_no_split (FILE *stream)
{
  cut_characters_mode (stream, true);
}

நிலையான வெறுமை
cut_characters (FILE *stream)
{
  cut_characters_mode (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

நிலையான வெறுமை
cut_fields_mb_any (FILE *stream, bool whitespace_mode)
{
  நிலையான எழுத்து bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  கட்டமைப்பு mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_mode,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_வரி_start = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool buffer_first_field;
  bool have_pending_வரி = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, அளவு bytes_in, stream);

  buffer_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  போது (true)
    {
      எண்ணிக்கை field_terminator terminator;
      எனில் (!buffer_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_வரி,
                                    write_field, வெற்று);
      இல்லையெனில்
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_வரி,
                                    false, &field_1_n_bytes);

      எனில் (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          எனில் (! field_selection_exhausted (field_idx))
            தொடர்;

          terminator = scan_mb_வரி_end (&mbbuf, &parser,
                                         &have_pending_வரி);
        }

      எனில் (terminator == FIELD_EOF && !have_pending_வரி)
        நிறுத்து;

      finish_current_வரி (field_idx, buffer_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_வரி_DELIMITER);
      எனில் (terminator == FIELD_EOF)
        நிறுத்து;

      reset_field_வரி (&field_idx, &found_any_selected_field,
                        &have_pending_வரி, &parser);
      write_field = begin_field_output (field_idx, buffer_first_field,
                                        &found_any_selected_field);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

நிலையான வெறுமை
cut_fields_mb (FILE *stream)
{
  cut_fields_mb_any (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

நிலையான வெறுமை
cut_fields_bytesearch (FILE *stream)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  நிலையான எழுத்து bytes_in[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool have_pending_வரி = false;
  bool skip_blank_run = false;
  bool pending_வரி_delim_field = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  bool buffer_first_field = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, bytes_in, அளவு bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  கட்டமைப்பு bytesearch_context தேடல் = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&தேடல்);

  போது (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      தேடல்.at_eof = mbbuf.eof;
      தேடல்.வரி_end_known = false;

      எனில் (pending_வரி_delim_field)
        {
          pending_வரி_delim_field = false;

          எனில் (n_avail == 0)
            {
              எனில் (handle_field_1 (field_idx, buffer_first_field,
                                  &field_1_n_bytes))
                next_item (&field_idx);
              maybe_write_வரி_delim (found_any_selected_field, field_idx);
              திரும்பு;
            }

          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          எனில் (whitespace_delimited)
            skip_blank_run = true;
        }

      எனில் (n_avail == 0)
        நிறுத்து;

      எழுத்து *chunk = mbbuf.buffer + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      எனில் (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_வரி_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          எழுத்து *last_வரி_delim = தேடல்.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, வரி_delim, n_avail);
          எனில் (last_வரி_delim)
            {
              idx_t n = last_வரி_delim - chunk + 1;
              எனில் (! suppress_non_delimited)
                {
                  write_bytes (field_1_buffer, field_1_n_bytes);
                  write_bytes (chunk, n);
                  எனில் (தேடல்.at_eof && chunk[n - 1] != வரி_delim)
                    write_வரி_delim ();
                }
              field_1_n_bytes = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_வரி = false;
              எனில் (தேடல்.at_eof)
                திரும்பு;
              தொடர்;
            }
        }

      போது (processed < n_avail)
        {
          எழுத்து *terminator = வெற்று;

          எனில் (skip_blank_run)
            {
              போது (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              எனில் (processed == n_avail)
                நிறுத்து;
              skip_blank_run = false;
            }

          எனில் (field_selection_exhausted (field_idx))
            {
              எனில் (field_delim_is_வரி_delim ())
                {
                  maybe_write_வரி_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  திரும்பு;
                }
              தேடல்.mode = BYTESEARCH_வரி_ONLY;
              எண்ணிக்கை field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &தேடல், &terminator);
              எனில் (terminator_kind == FIELD_வரி_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_வரி_delim (found_any_selected_field,
                                          field_idx);
                  செல் reset_வரி;
                }
              processed = n_avail;
              நிறுத்து;
            }

          எண்ணிக்கை field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &தேடல், &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          எனில் (terminator_kind == FIELD_DATA
              && !தேடல்.at_eof
              && !whitespace_delimited
              && !field_delim_is_வரி_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          எனில் (field_len || terminator)
            have_pending_வரி = true;

          /* Output field content.  */
          எனில் (field_idx == 1 && buffer_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          இல்லையெனில் எனில் (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          எனில் (terminator_kind == FIELD_DATA)
            நிறுத்து;

          எனில் (terminator_kind == FIELD_DELIMITER)
            {
              எனில் (field_delim_is_வரி_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_வரி_delim_field = true;
                  நிறுத்து;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, buffer_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              எனில் (whitespace_delimited)
                skip_blank_run = true;
            }
          இல்லையெனில்
            {
              affirm (terminator_kind == FIELD_வரி_DELIMITER);
              processed++;
              finish_current_வரி (field_idx, buffer_first_field,
                                   &field_1_n_bytes,
                                   found_any_selected_field, true);

            reset_வரி:
              field_idx = 1;
              current_rp = frp;
              found_any_selected_field = false;
              have_pending_வரி = false;
              bytesearch_context_reset (&தேடல்);
              write_field = begin_field_output (field_idx, buffer_first_field,
                                                &found_any_selected_field);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  எனில் (!have_pending_வரி)
    திரும்பு;

  finish_current_வரி (field_idx, buffer_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

நிலையான வெறுமை
cut_fields_ws (FILE *stream)
{
  எனில் (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  இல்லையெனில்
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

நிலையான bool
cut_file (எழுத்து மாறிலி *file, வெறுமை (*cut_stream) (FILE *))
{
  FILE *stream;

  எனில் (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  இல்லையெனில்
    {
      stream = fopen (file, "r");
      எனில் (stream == வெற்று)
        {
          error (0, errno, "%s", quotef (file));
          திரும்பு false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  முழு err = errno;
  எனில் (!ferror (stream))
    err = 0;
  எனில் (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF.  */
  இல்லையெனில் எனில் (fclose (stream) == EOF)
    err = errno;
  எனில் (err)
    {
      error (0, err, "%s", quotef (file));
      திரும்பு false;
    }
  திரும்பு true;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  எழுத்து *spec_list_string = வெற்று;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, வெற்று))
         != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'b':
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        நிலை 'c':
          எனில் (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        நிலை 'f':
        நிலை 'F':
          எனில் (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
          எனில் (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          எனில் (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          நிறுத்து;

        நிலை 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            எனில் (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            நிறுத்து;
          }

        நிலை 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          நிறுத்து;

        நிலை 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_length = (optarg[0] == '\0'
                                     ? 1 : strlen (optarg));
          output_delimiter_string = optarg;
          நிறுத்து;

        நிலை 'n':
          no_split = true;
          நிறுத்து;

        நிலை 's':
          suppress_non_delimited = true;
          நிறுத்து;

        நிலை 'z':
          வரி_delim = '\0';
          நிறுத்து;

        நிலை COMPLEMENT_OPTION:
          complement = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  எனில் (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
    {
      எனில் (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an உள்ளீடு delimiter makes sense\n\
\tonly when operating on fields"));

      எனில் (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited வரிகள் makes sense\n\
\tonly when operating on fields"));
    }

  எனில் (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  எனில் (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  எனில் (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  எனில் (output_delimiter_string == வெற்று)
    {
      output_delimiter_string = output_delimiter_default;
      எனில் (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_length = 1;
        }
      இல்லையெனில்
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_length = delim_mcel.len;
        }
    }

  வெறுமை (*cut_stream) (FILE *) = வெற்று;
  தேர்வு (cut_mode)
    {
    நிலை CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      நிறுத்து;

    நிலை CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      நிறுத்து;

    நிலை CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      நிறுத்து;

    நிலை CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  எனில் (optind == argc)
    ok = cut_file ("-", cut_stream);
  இல்லையெனில்
    க்கு (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  எனில் (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
