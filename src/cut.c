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

#include <config.h>

#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include "system.h"

#include "argmatch.h"
#include "assure.h"
#include "c-ctype.h"
#include "fadvise.h"
#include "ioblksize.h"
#include "mbbuf.h"
#include "memchr2.h"

#include "set-fields.h"
#include "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cut"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#define FATAL_ERROR(Message)						\
  do									\
    {									\
      error (0, 0, (Message));						\
      usage (EXIT_FAILURE);						\
    }									\
  ขณะ (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#define SMALL_BYTE_THRESHOLD 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
static sa_nracanaa field_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
static var_na *field_1_buffer;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
static idx_t field_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
static bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
static bool complement;

/* The delimiter character for multibyte field mode.  */
static mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
static var_na delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
static unsigned var_na line_delim = '\n';

/* The length of output_delimiter_string.  */
static size_t output_delimiter_length;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
static var_na *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
static var_na output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
static bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
static bool no_split;

/* If true, interpret each run of whitespace as one field delimiter.  */
static bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
static bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
static bool space_output_delimiter_default;

ga_nanaa whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

static var_na const *const whitespace_option_args[] =
{
  "trimmed", NULL
};

static ga_nanaa whitespace_option const whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_args, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
static ga_nanaa
{
  CUT_MODE_NONE,
  CUT_MODE_BYTES,
  CUT_MODE_CHARACTERS,
  CUT_MODE_FIELDS
} cut_mode = CUT_MODE_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

static sa_nracanaa option const longopts[] =
{
  {"bytes", required_argument, NULL, 'b'},
  {"characters", required_argument, NULL, 'c'},
  {"fields", required_argument, NULL, 'f'},
  {"delimiter", required_argument, NULL, 'd'},
  {"no-partial", no_argument, NULL, 'n'},
  {"whitespace-delimited", optional_argument, NULL, 'w'},
  {"only-delimited", no_argument, NULL, 's'},
  {"output-delimiter", required_argument, NULL, 'O'},
  {"complement", no_argument, NULL, COMPLEMENT_OPTION},
  {"zero-terminated", no_argument, NULL, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (จำนวนเต็ม status)
{
  ถ้า (status != EXIT_SUCCESS)
    emit_try_help ();
  มิฉะนั้น
    {
      printf (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Print selected parts of lines from each FILE to standard output.\n\
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
         use DELIM instead of TAB สำหรับ field delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any line that contains\n\
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
         the default is to use the input delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         do not print lines not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a run of blank characters as the field delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         line delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one of -b, -c, -f or -F.  Each LIST is made up of one\n\
range, or many ranges separated by commas.  Selected input is written\n\
in the same order that it is read, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one of:\n\
\n\
  N     N'th byte, character or field, counted from 1\n\
  N-    from N'th byte, character or field, to end of line\n\
  N-M   from N'th to M'th (included) byte, character or field\n\
  -M    from first to M'th (included) byte, character or field\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

static anta_hstha void
next_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  ถ้า ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

static anta_hstha bool
print_kth (uintmax_t k)
{
  คืน current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

static anta_hstha bool
is_range_start_index (uintmax_t k)
{
  คืน k == current_rp->lo;
}

static anta_hstha bool
field_delim_is_line_delim (void)
{
  คืน delim_mcel.len == 1 && delim_bytes[0] == line_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

static bool
mcel_isblank (mcel_t g)
{
  คืน (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

static anta_hstha bool
bytesearch_field_delim_ok (void)
{
  unsigned var_na delim_0 = delim_bytes[0];

  คืน (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


ga_nanaa field_terminator
{
  FIELD_DATA,
  FIELD_DELIMITER,
  FIELD_LINE_DELIMITER,
  FIELD_EOF
};

ga_nanaa bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_LINE_ONLY
};

sa_nracanaa bytesearch_context
{
  ga_nanaa bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  var_na *line_end;
  bool line_end_known;
};

static anta_hstha void
bytesearch_context_reset (sa_nracanaa bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->line_end = NULL;
  ctx->line_end_known = false;
}

sa_nracanaa mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_line_start;
  bool have_saved;
  mcel_t saved_g;
};

static anta_hstha mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  ถ้า (*have_saved)
    {
      *have_saved = false;
      คืน *saved_g;
    }
  คืน mbbuf_get_char (mbbuf);
}

static anta_hstha ga_nanaa field_terminator
skip_whitespace_run (mbbuf_t *mbuf, sa_nracanaa mbfield_parser *parser,
                     bool *have_pending_line,
                     bool have_initial_whitespace)
{
  mcel_t g;

  do
    {
      g = mbbuf_get_char (mbuf);
      ถ้า (g.ch != MBBUF_EOF)
        *have_pending_line = true;
    }
  ขณะ (g.ch != MBBUF_EOF && g.ch != line_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_line_start;

  ถ้า (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == line_delim))
    {
      คืน g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_LINE_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  คืน trim_start && !have_initial_whitespace ? FIELD_DATA : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

static void
write_bytes (var_na const *buf, size_t n_bytes)
{
  ถ้า (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      สำหรับ (size_t i = 0; i < n_bytes; i++)
        ถ้า (putchar (buf[i]) < 0)
          write_error ();
      คืน;
    }

  ถ้า (fwrite (buf, parimaa_na (var_na), n_bytes, stdout) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

static anta_hstha void
copy_bytes (var_na *dst, var_na const *src, size_t n_bytes)
{
  ถ้า (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      สำหรับ (size_t i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      คืน;
    }

  memcpy (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

static anta_hstha void*
search_bytes (var_na const *buf, จำนวนเต็ม c, size_t n_bytes)
{
  ถ้า (n_bytes != 0 && to_uchar (buf[0]) == c)
    คืน (var_na *) buf;
  ถ้า (1 < n_bytes && to_uchar (buf[1]) == c)
    คืน (var_na *) buf + 1;

  คืน 2 < n_bytes ? (var_na *) memchr (buf + 2, c, n_bytes - 2) : NULL;
}

static anta_hstha void
write_line_delim (void)
{
  ถ้า (putchar (line_delim) < 0)
    write_error ();
}

static anta_hstha void
reset_item_line (uintmax_t *item_idx, bool *print_delimiter)
{
  write_line_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

static anta_hstha void
write_pending_line_delim (uintmax_t item_idx)
{
  ถ้า (item_idx > 0)
    write_line_delim ();
}

static anta_hstha void
write_selected_item (bool *print_delimiter, bool range_start,
                     var_na const *buf, size_t n_bytes)
{
  ถ้า (output_delimiter_string != output_delimiter_default)
    {
      ถ้า (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

static anta_hstha void
append_field_1_chunk (var_na const *buf, idx_t len, idx_t *n_bytes)
{
  ถ้า (field_1_bufsize - *n_bytes < len)
    {
      field_1_buffer = xpalloc (field_1_buffer, &field_1_bufsize,
                                len, -1, parimaa_na *field_1_buffer);
    }

  copy_bytes (field_1_buffer + *n_bytes, buf, len);
  *n_bytes += len;
}

static anta_hstha void
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

static anta_hstha ga_nanaa field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, sa_nracanaa mbfield_parser *parser,
                     bool *have_pending_line, bool write_field,
                     idx_t *n_bytes)
{
  ถ้า (parser->trim_outer_whitespace && parser->at_line_start)
    {
      ga_nanaa field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_line, false);
      ถ้า (terminator != FIELD_DATA)
        คืน terminator;
    }

  parser->at_line_start = false;

  ขณะ (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      ถ้า (g.ch == MBBUF_EOF)
        คืน FIELD_EOF;

      *have_pending_line = true;

      ถ้า (g.ch == line_delim)
        คืน FIELD_LINE_DELIMITER;

      ถ้า (mcel_isblank (g))
        คืน skip_whitespace_run (mbbuf, parser, have_pending_line, true);

      ถ้า (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      มิฉะนั้น ถ้า (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

static anta_hstha ga_nanaa field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_line,
                     bool write_field, idx_t *n_bytes)
{
  ขณะ (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      ถ้า (g.ch == MBBUF_EOF)
        คืน FIELD_EOF;

      *have_pending_line = true;

      ถ้า (g.ch == line_delim)
        คืน FIELD_LINE_DELIMITER;

      ถ้า (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        คืน FIELD_DELIMITER;

      ถ้า (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      มิฉะนั้น ถ้า (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

static anta_hstha ga_nanaa field_terminator
scan_mb_field (mbbuf_t *mbbuf, sa_nracanaa mbfield_parser *parser,
               bool *have_pending_line, bool write_field, idx_t *n_bytes)
{
  คืน (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_line,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_line, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

static ga_nanaa field_terminator
scan_mb_line_end (mbbuf_t *mbbuf, sa_nracanaa mbfield_parser *parser,
                  bool *have_pending_line)
{
  ถ้า (parser->have_saved)
    {
      parser->have_saved = false;
      ถ้า (parser->saved_g.ch == MBBUF_EOF)
        คืน FIELD_EOF;

      *have_pending_line = true;
      ถ้า (parser->saved_g.ch == line_delim)
        คืน FIELD_LINE_DELIMITER;
    }

  ขณะ (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      ถ้า (available == 0)
        คืน FIELD_EOF;

      var_na *buffer = mbbuf->buffer + mbbuf->offset;
      var_na *line_end = search_bytes (buffer, line_delim, available);
      idx_t n = line_end ? line_end - buffer + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_line = true;

      ถ้า (line_end)
        คืน FIELD_LINE_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
static var_na *
find_field_delim (var_na *buf, size_t len)
{
  ถ้า (len < delim_mcel.len)
    คืน NULL;

#ถ้า ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  คืน memmem (buf, len, delim_bytes, delim_mcel.len);
#มิฉะนั้น
  ถ้า (delim_mcel.len == 1)
    คืน memchr (buf, delim_bytes[0], len);

  var_na const *p = buf;
  var_na const *end = buf + len;

  var_na saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  ขณะ (p < end)
    {
      var_na const *nul = memchr (p, '\0', end - p);
      ถ้า (!nul)
        {
          var_na const *match = strstr (p, delim_bytes);
          buf[len] = saved;
          คืน (var_na *) match;
        }

      ถ้า (p < nul)
        {
          var_na const *match = strstr (p, delim_bytes);
          ถ้า (match)
            {
              buf[len] = saved;
              คืน (var_na *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  คืน NULL;
#endif
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
static var_na *
find_c_blank (var_na *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  สำหรับ (idx_t i = 0; i < n; i++)
    ถ้า (c_isblank (buf[i]))
      คืน buf + i;

  คืน memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_PURE
static idx_t
field_delim_overlap (var_na const *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  ขณะ (0 < overlap)
    {
      ถ้า (memeq (buf + len - overlap, delim_bytes, overlap))
        คืน overlap;
      overlap--;
    }

  คืน 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

static anta_hstha ga_nanaa field_terminator
find_field_terminator (var_na *buf, idx_t len,
                       sa_nracanaa bytesearch_context *ctx, var_na **terminator)
{
  ถ้า (ctx->mode == BYTESEARCH_LINE_ONLY)
    {
      ถ้า (!ctx->line_end_known)
        {
          ctx->line_end = memchr (buf, line_delim, len);
          ctx->line_end_known = true;
        }

      *terminator = ctx->line_end;
      คืน *terminator ? FIELD_LINE_DELIMITER : FIELD_DATA;
    }

  ถ้า (field_delim_is_line_delim ())
    {
      *terminator = memchr (buf, line_delim, len);
      คืน *terminator ? FIELD_DELIMITER : FIELD_DATA;
    }

  ถ้า (!ctx->line_end_known)
    {
      ctx->line_end = memchr (buf, line_delim, len);
      ctx->line_end_known = true;
    }

  idx_t field_len = ctx->line_end ? ctx->line_end - buf : len;

  var_na *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  ถ้า (field_end)
    {
      *terminator = field_end;
      คืน FIELD_DELIMITER;
    }

  *terminator = ctx->line_end;
  คืน ctx->line_end ? FIELD_LINE_DELIMITER : FIELD_DATA;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

static anta_hstha void
maybe_write_line_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  ถ้า (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_line_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

static anta_hstha bool
begin_field_output (uintmax_t field_idx, bool buffer_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  ถ้า (write_field && ! (field_idx == 1 && buffer_first_field))
    {
      ถ้า (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *found_any_selected_field = true;
    }

  คืน write_field;
}

static anta_hstha bool
handle_field_1 (uintmax_t field_idx, bool buffer_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  ถ้า (field_idx == 1 && buffer_first_field)
    {
      ถ้า ((field_1_selected = print_kth (1)))
        write_bytes (field_1_buffer, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  คืน field_1_selected;
}

static anta_hstha void
handle_field_delimiter (uintmax_t *field_idx, bool buffer_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  ถ้า (handle_field_1 (*field_idx, buffer_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  next_item (field_idx);
  *write_field = begin_field_output (*field_idx, buffer_first_field,
                                     found_any_selected_field);
}

static anta_hstha bool
field_selection_exhausted (uintmax_t field_idx)
{
  คืน !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

static anta_hstha void
finish_current_line (uintmax_t field_idx, bool buffer_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool line_terminated)
{
  ถ้า (field_idx == 1 && buffer_first_field)
    {
      ถ้า (!suppress_non_delimited
          && (line_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_buffer, *field_1_n_bytes);
          write_line_delim ();
        }
      *field_1_n_bytes = 0;
    }
  มิฉะนั้น
    maybe_write_line_delim (found_any_selected_field, field_idx);
}

static anta_hstha void
sync_byte_selection (uintmax_t byte_idx)
{
  ขณะ (current_rp->hi <= byte_idx)
    current_rp++;
}

static anta_hstha void
reset_field_line (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_line, sa_nracanaa mbfield_parser *parser)
{
  *field_idx = 1;
  current_rp = frp;
  *found_any_selected_field = false;
  *have_pending_line = false;
  parser->have_saved = false;
  parser->at_line_start = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

static void
cut_bytes (FILE *stream)
{
  static var_na bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  จำนวนเต็ม fd = fileno (stream);

  current_rp = frp;

  ขณะ (true)
    {
      idx_t available = read (fd, bytes_in, parimaa_na bytes_in);
      ถ้า (available <= 0)
        {
          ถ้า (available < 0)
            fseterr (stream);
          write_pending_line_delim (byte_idx);
          bha_nga;
        }

      idx_t processed = 0;

      ขณะ (processed < available)
        {
          var_na *line = bytes_in + processed;
          idx_t to_process = available - processed;
          var_na *line_end = search_bytes (line, line_delim, to_process);
          var_na *end = line + (line_end ? line_end - line : to_process);
          var_na *p = line;

          ขณะ (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              ถ้า (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              มิฉะนั้น
                {
                  idx_t n = MIN (end - p, current_rp->hi - byte_idx);
                  write_selected_item (&print_delimiter,
                                       is_range_start_index (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += end - line;
          ถ้า (line_end)
            {
              processed++;
              reset_item_line (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

static void
cut_characters_mode (FILE *stream, bool byte_mode)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  static var_na bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, parimaa_na bytes_in, stream);

  ขณะ (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      ถ้า (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          var_na *p = mbbuf.buffer + mbbuf.offset;
          var_na *end = search_bytes (p, line_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          ถ้า (!end && available)
            continue;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      ถ้า (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          var_na const *p = mbbuf.buffer + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          var_na const *end = search_bytes (p, line_delim, n);
          ถ้า (end)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, end - p + 1);
              reset_item_line (&idx, &print_delimiter);
              continue;
            }

          idx += u8_count (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      ถ้า (g.ch == line_delim)
        reset_item_line (&idx, &print_delimiter);
      มิฉะนั้น ถ้า (g.ch == MBBUF_EOF)
        {
          write_pending_line_delim (idx);
          bha_nga;
        }
      มิฉะนั้น ถ้า (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          สำหรับ (idx_t i = 0; i < g.len; i++)
            {
              next_item (&idx);
              ถ้า (print_kth (idx))
                {
                  ถ้า (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_index (idx);
                    }
                }
              มิฉะนั้น ถ้า (seen_selected)
                suffix_selected = false;
            }

          ถ้า (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      มิฉะนั้น
        {
          next_item (&idx);
          ถ้า (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_index (idx),
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
    }
}

static void
cut_bytes_no_split (FILE *stream)
{
  cut_characters_mode (stream, true);
}

static void
cut_characters (FILE *stream)
{
  cut_characters_mode (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

static void
cut_fields_mb_any (FILE *stream, bool whitespace_mode)
{
  static var_na bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  sa_nracanaa mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_mode,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_line_start = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool buffer_first_field;
  bool have_pending_line = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, parimaa_na bytes_in, stream);

  buffer_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  ขณะ (true)
    {
      ga_nanaa field_terminator terminator;
      ถ้า (!buffer_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_line,
                                    write_field, NULL);
      มิฉะนั้น
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_line,
                                    false, &field_1_n_bytes);

      ถ้า (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          ถ้า (! field_selection_exhausted (field_idx))
            continue;

          terminator = scan_mb_line_end (&mbbuf, &parser,
                                         &have_pending_line);
        }

      ถ้า (terminator == FIELD_EOF && !have_pending_line)
        bha_nga;

      finish_current_line (field_idx, buffer_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_LINE_DELIMITER);
      ถ้า (terminator == FIELD_EOF)
        bha_nga;

      reset_field_line (&field_idx, &found_any_selected_field,
                        &have_pending_line, &parser);
      write_field = begin_field_output (field_idx, buffer_first_field,
                                        &found_any_selected_field);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

static void
cut_fields_mb (FILE *stream)
{
  cut_fields_mb_any (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

static void
cut_fields_bytesearch (FILE *stream)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  static var_na bytes_in[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool have_pending_line = false;
  bool skip_blank_run = false;
  bool pending_line_delim_field = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  bool buffer_first_field = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, bytes_in, parimaa_na bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  sa_nracanaa bytesearch_context search = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&search);

  ขณะ (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      search.at_eof = mbbuf.eof;
      search.line_end_known = false;

      ถ้า (pending_line_delim_field)
        {
          pending_line_delim_field = false;

          ถ้า (n_avail == 0)
            {
              ถ้า (handle_field_1 (field_idx, buffer_first_field,
                                  &field_1_n_bytes))
                next_item (&field_idx);
              maybe_write_line_delim (found_any_selected_field, field_idx);
              คืน;
            }

          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          ถ้า (whitespace_delimited)
            skip_blank_run = true;
        }

      ถ้า (n_avail == 0)
        bha_nga;

      var_na *chunk = mbbuf.buffer + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      ถ้า (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_line_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          var_na *last_line_delim = search.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, line_delim, n_avail);
          ถ้า (last_line_delim)
            {
              idx_t n = last_line_delim - chunk + 1;
              ถ้า (! suppress_non_delimited)
                {
                  write_bytes (field_1_buffer, field_1_n_bytes);
                  write_bytes (chunk, n);
                  ถ้า (search.at_eof && chunk[n - 1] != line_delim)
                    write_line_delim ();
                }
              field_1_n_bytes = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_line = false;
              ถ้า (search.at_eof)
                คืน;
              continue;
            }
        }

      ขณะ (processed < n_avail)
        {
          var_na *terminator = NULL;

          ถ้า (skip_blank_run)
            {
              ขณะ (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              ถ้า (processed == n_avail)
                bha_nga;
              skip_blank_run = false;
            }

          ถ้า (field_selection_exhausted (field_idx))
            {
              ถ้า (field_delim_is_line_delim ())
                {
                  maybe_write_line_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  คืน;
                }
              search.mode = BYTESEARCH_LINE_ONLY;
              ga_nanaa field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &search, &terminator);
              ถ้า (terminator_kind == FIELD_LINE_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_line_delim (found_any_selected_field,
                                          field_idx);
                  goto reset_line;
                }
              processed = n_avail;
              bha_nga;
            }

          ga_nanaa field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &search, &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          ถ้า (terminator_kind == FIELD_DATA
              && !search.at_eof
              && !whitespace_delimited
              && !field_delim_is_line_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          ถ้า (field_len || terminator)
            have_pending_line = true;

          /* Output field content.  */
          ถ้า (field_idx == 1 && buffer_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          มิฉะนั้น ถ้า (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          ถ้า (terminator_kind == FIELD_DATA)
            bha_nga;

          ถ้า (terminator_kind == FIELD_DELIMITER)
            {
              ถ้า (field_delim_is_line_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_line_delim_field = true;
                  bha_nga;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, buffer_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              ถ้า (whitespace_delimited)
                skip_blank_run = true;
            }
          มิฉะนั้น
            {
              affirm (terminator_kind == FIELD_LINE_DELIMITER);
              processed++;
              finish_current_line (field_idx, buffer_first_field,
                                   &field_1_n_bytes,
                                   found_any_selected_field, true);

            reset_line:
              field_idx = 1;
              current_rp = frp;
              found_any_selected_field = false;
              have_pending_line = false;
              bytesearch_context_reset (&search);
              write_field = begin_field_output (field_idx, buffer_first_field,
                                                &found_any_selected_field);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  ถ้า (!have_pending_line)
    คืน;

  finish_current_line (field_idx, buffer_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

static void
cut_fields_ws (FILE *stream)
{
  ถ้า (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  มิฉะนั้น
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

static bool
cut_file (var_na const *file, void (*cut_stream) (FILE *))
{
  FILE *stream;

  ถ้า (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  มิฉะนั้น
    {
      stream = fopen (file, "r");
      ถ้า (stream == NULL)
        {
          error (0, errno, "%s", quotef (file));
          คืน false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  จำนวนเต็ม err = errno;
  ถ้า (!ferror (stream))
    err = 0;
  ถ้า (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF.  */
  มิฉะนั้น ถ้า (fclose (stream) == EOF)
    err = errno;
  ถ้า (err)
    {
      error (0, err, "%s", quotef (file));
      คืน false;
    }
  คืน true;
}

จำนวนเต็ม
หลัก (จำนวนเต็ม argc, var_na **argv)
{
  จำนวนเต็ม optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  var_na *spec_list_string = NULL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ขณะ ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, NULL))
         != -1)
    {
      switch (optc)
        {
        pak_sa 'b':
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        pak_sa 'c':
          ถ้า (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        pak_sa 'f':
        pak_sa 'F':
          ถ้า (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
          ถ้า (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          ถ้า (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          bha_nga;

        pak_sa 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            ถ้า (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            bha_nga;
          }

        pak_sa 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          bha_nga;

        pak_sa 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_length = (optarg[0] == '\0'
                                     ? 1 : strlen (optarg));
          output_delimiter_string = optarg;
          bha_nga;

        pak_sa 'n':
          no_split = true;
          bha_nga;

        pak_sa 's':
          suppress_non_delimited = true;
          bha_nga;

        pak_sa 'z':
          line_delim = '\0';
          bha_nga;

        pak_sa COMPLEMENT_OPTION:
          complement = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  ถ้า (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  ถ้า (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
    {
      ถ้า (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an input delimiter makes sense\n\
\tonly when operating on fields"));

      ถ้า (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited lines makes sense\n\
\tonly when operating on fields"));
    }

  ถ้า (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  ถ้า (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  ถ้า (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  ถ้า (output_delimiter_string == NULL)
    {
      output_delimiter_string = output_delimiter_default;
      ถ้า (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_length = 1;
        }
      มิฉะนั้น
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_length = delim_mcel.len;
        }
    }

  void (*cut_stream) (FILE *) = NULL;
  switch (cut_mode)
    {
    pak_sa CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      bha_nga;

    pak_sa CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      bha_nga;

    pak_sa CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      bha_nga;

    pak_sa CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  ถ้า (optind == argc)
    ok = cut_file ("-", cut_stream);
  มิฉะนั้น
    สำหรับ (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  ถ้า (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  คืน ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
