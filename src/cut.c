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
  пока (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#define SMALL_BYTE_THRESHOLD 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
static struct field_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
static char *field_1_buffer;

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
static char delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
static unsigned char line_delim = '\n';

/* The length of output_delimiter_string.  */
static size_t output_delimiter_length;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
static char *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
static char output_delimiter_default[MCEL_LEN_MAX];

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

enum whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

static char const *const whitespace_option_args[] =
{
  "trimmed", NULL
};

static enum whitespace_option const whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_args, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
static enum
{
  CUT_MODE_NONE,
  CUT_MODE_BYTES,
  CUT_MODE_CHARACTERS,
  CUT_MODE_FIELDS
} cut_mode = CUT_MODE_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
enum
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

static struct option const longopts[] =
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
usage (целое status)
{
  если (status != EXIT_SUCCESS)
    emit_try_help ();
  иначе
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
         use DELIM instead of TAB для field delimiter\n\
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

static inline void
next_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  если ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

static inline bool
print_kth (uintmax_t k)
{
  вернуть current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

static inline bool
is_range_start_index (uintmax_t k)
{
  вернуть k == current_rp->lo;
}

static inline bool
field_delim_is_line_delim (void)
{
  вернуть delim_mcel.len == 1 && delim_bytes[0] == line_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

static bool
mcel_isblank (mcel_t g)
{
  вернуть (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

static inline bool
bytesearch_field_delim_ok (void)
{
  unsigned char delim_0 = delim_bytes[0];

  вернуть (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


enum field_terminator
{
  FIELD_DATA,
  FIELD_DELIMITER,
  FIELD_LINE_DELIMITER,
  FIELD_EOF
};

enum bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_LINE_ONLY
};

struct bytesearch_context
{
  enum bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  char *line_end;
  bool line_end_known;
};

static inline void
bytesearch_context_reset (struct bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->line_end = NULL;
  ctx->line_end_known = false;
}

struct mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_line_start;
  bool have_saved;
  mcel_t saved_g;
};

static inline mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  если (*have_saved)
    {
      *have_saved = false;
      вернуть *saved_g;
    }
  вернуть mbbuf_get_char (mbbuf);
}

static inline enum field_terminator
skip_whitespace_run (mbbuf_t *mbuf, struct mbfield_parser *parser,
                     bool *have_pending_line,
                     bool have_initial_whitespace)
{
  mcel_t g;

  do
    {
      g = mbbuf_get_char (mbuf);
      если (g.ch != MBBUF_EOF)
        *have_pending_line = true;
    }
  пока (g.ch != MBBUF_EOF && g.ch != line_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_line_start;

  если (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == line_delim))
    {
      вернуть g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_LINE_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  вернуть trim_start && !have_initial_whitespace ? FIELD_DATA : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

static void
write_bytes (char const *buf, size_t n_bytes)
{
  если (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      для (size_t i = 0; i < n_bytes; i++)
        если (putchar (buf[i]) < 0)
          write_error ();
      вернуть;
    }

  если (fwrite (buf, sizeof (char), n_bytes, stdout) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

static inline void
copy_bytes (char *dst, char const *src, size_t n_bytes)
{
  если (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      для (size_t i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      вернуть;
    }

  memcpy (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

static inline void*
search_bytes (char const *buf, целое c, size_t n_bytes)
{
  если (n_bytes != 0 && to_uchar (buf[0]) == c)
    вернуть (char *) buf;
  если (1 < n_bytes && to_uchar (buf[1]) == c)
    вернуть (char *) buf + 1;

  вернуть 2 < n_bytes ? (char *) memchr (buf + 2, c, n_bytes - 2) : NULL;
}

static inline void
write_line_delim (void)
{
  если (putchar (line_delim) < 0)
    write_error ();
}

static inline void
reset_item_line (uintmax_t *item_idx, bool *print_delimiter)
{
  write_line_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

static inline void
write_pending_line_delim (uintmax_t item_idx)
{
  если (item_idx > 0)
    write_line_delim ();
}

static inline void
write_selected_item (bool *print_delimiter, bool range_start,
                     char const *buf, size_t n_bytes)
{
  если (output_delimiter_string != output_delimiter_default)
    {
      если (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

static inline void
append_field_1_chunk (char const *buf, idx_t len, idx_t *n_bytes)
{
  если (field_1_bufsize - *n_bytes < len)
    {
      field_1_buffer = xpalloc (field_1_buffer, &field_1_bufsize,
                                len, -1, sizeof *field_1_buffer);
    }

  copy_bytes (field_1_buffer + *n_bytes, buf, len);
  *n_bytes += len;
}

static inline void
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

static inline enum field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, struct mbfield_parser *parser,
                     bool *have_pending_line, bool write_field,
                     idx_t *n_bytes)
{
  если (parser->trim_outer_whitespace && parser->at_line_start)
    {
      enum field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_line, false);
      если (terminator != FIELD_DATA)
        вернуть terminator;
    }

  parser->at_line_start = false;

  пока (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      если (g.ch == MBBUF_EOF)
        вернуть FIELD_EOF;

      *have_pending_line = true;

      если (g.ch == line_delim)
        вернуть FIELD_LINE_DELIMITER;

      если (mcel_isblank (g))
        вернуть skip_whitespace_run (mbbuf, parser, have_pending_line, true);

      если (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      иначе если (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

static inline enum field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_line,
                     bool write_field, idx_t *n_bytes)
{
  пока (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      если (g.ch == MBBUF_EOF)
        вернуть FIELD_EOF;

      *have_pending_line = true;

      если (g.ch == line_delim)
        вернуть FIELD_LINE_DELIMITER;

      если (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        вернуть FIELD_DELIMITER;

      если (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      иначе если (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

static inline enum field_terminator
scan_mb_field (mbbuf_t *mbbuf, struct mbfield_parser *parser,
               bool *have_pending_line, bool write_field, idx_t *n_bytes)
{
  вернуть (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_line,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_line, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

static enum field_terminator
scan_mb_line_end (mbbuf_t *mbbuf, struct mbfield_parser *parser,
                  bool *have_pending_line)
{
  если (parser->have_saved)
    {
      parser->have_saved = false;
      если (parser->saved_g.ch == MBBUF_EOF)
        вернуть FIELD_EOF;

      *have_pending_line = true;
      если (parser->saved_g.ch == line_delim)
        вернуть FIELD_LINE_DELIMITER;
    }

  пока (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      если (available == 0)
        вернуть FIELD_EOF;

      char *buffer = mbbuf->buffer + mbbuf->offset;
      char *line_end = search_bytes (buffer, line_delim, available);
      idx_t n = line_end ? line_end - buffer + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_line = true;

      если (line_end)
        вернуть FIELD_LINE_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
static char *
find_field_delim (char *buf, size_t len)
{
  если (len < delim_mcel.len)
    вернуть NULL;

#если ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  вернуть memmem (buf, len, delim_bytes, delim_mcel.len);
#иначе
  если (delim_mcel.len == 1)
    вернуть memchr (buf, delim_bytes[0], len);

  char const *p = buf;
  char const *end = buf + len;

  char saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  пока (p < end)
    {
      char const *nul = memchr (p, '\0', end - p);
      если (!nul)
        {
          char const *match = strstr (p, delim_bytes);
          buf[len] = saved;
          вернуть (char *) match;
        }

      если (p < nul)
        {
          char const *match = strstr (p, delim_bytes);
          если (match)
            {
              buf[len] = saved;
              вернуть (char *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  вернуть NULL;
#endif
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
static char *
find_c_blank (char *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  для (idx_t i = 0; i < n; i++)
    если (c_isblank (buf[i]))
      вернуть buf + i;

  вернуть memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_PURE
static idx_t
field_delim_overlap (char const *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  пока (0 < overlap)
    {
      если (memeq (buf + len - overlap, delim_bytes, overlap))
        вернуть overlap;
      overlap--;
    }

  вернуть 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

static inline enum field_terminator
find_field_terminator (char *buf, idx_t len,
                       struct bytesearch_context *ctx, char **terminator)
{
  если (ctx->mode == BYTESEARCH_LINE_ONLY)
    {
      если (!ctx->line_end_known)
        {
          ctx->line_end = memchr (buf, line_delim, len);
          ctx->line_end_known = true;
        }

      *terminator = ctx->line_end;
      вернуть *terminator ? FIELD_LINE_DELIMITER : FIELD_DATA;
    }

  если (field_delim_is_line_delim ())
    {
      *terminator = memchr (buf, line_delim, len);
      вернуть *terminator ? FIELD_DELIMITER : FIELD_DATA;
    }

  если (!ctx->line_end_known)
    {
      ctx->line_end = memchr (buf, line_delim, len);
      ctx->line_end_known = true;
    }

  idx_t field_len = ctx->line_end ? ctx->line_end - buf : len;

  char *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  если (field_end)
    {
      *terminator = field_end;
      вернуть FIELD_DELIMITER;
    }

  *terminator = ctx->line_end;
  вернуть ctx->line_end ? FIELD_LINE_DELIMITER : FIELD_DATA;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

static inline void
maybe_write_line_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  если (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_line_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

static inline bool
begin_field_output (uintmax_t field_idx, bool buffer_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  если (write_field && ! (field_idx == 1 && buffer_first_field))
    {
      если (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *found_any_selected_field = true;
    }

  вернуть write_field;
}

static inline bool
handle_field_1 (uintmax_t field_idx, bool buffer_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  если (field_idx == 1 && buffer_first_field)
    {
      если ((field_1_selected = print_kth (1)))
        write_bytes (field_1_buffer, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  вернуть field_1_selected;
}

static inline void
handle_field_delimiter (uintmax_t *field_idx, bool buffer_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  если (handle_field_1 (*field_idx, buffer_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  next_item (field_idx);
  *write_field = begin_field_output (*field_idx, buffer_first_field,
                                     found_any_selected_field);
}

static inline bool
field_selection_exhausted (uintmax_t field_idx)
{
  вернуть !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

static inline void
finish_current_line (uintmax_t field_idx, bool buffer_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool line_terminated)
{
  если (field_idx == 1 && buffer_first_field)
    {
      если (!suppress_non_delimited
          && (line_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_buffer, *field_1_n_bytes);
          write_line_delim ();
        }
      *field_1_n_bytes = 0;
    }
  иначе
    maybe_write_line_delim (found_any_selected_field, field_idx);
}

static inline void
sync_byte_selection (uintmax_t byte_idx)
{
  пока (current_rp->hi <= byte_idx)
    current_rp++;
}

static inline void
reset_field_line (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_line, struct mbfield_parser *parser)
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
  static char bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  целое fd = fileno (stream);

  current_rp = frp;

  пока (true)
    {
      idx_t available = read (fd, bytes_in, sizeof bytes_in);
      если (available <= 0)
        {
          если (available < 0)
            fseterr (stream);
          write_pending_line_delim (byte_idx);
          break;
        }

      idx_t processed = 0;

      пока (processed < available)
        {
          char *line = bytes_in + processed;
          idx_t to_process = available - processed;
          char *line_end = search_bytes (line, line_delim, to_process);
          char *end = line + (line_end ? line_end - line : to_process);
          char *p = line;

          пока (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              если (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              иначе
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
          если (line_end)
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
  static char bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, sizeof bytes_in, stream);

  пока (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      если (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          char *p = mbbuf.buffer + mbbuf.offset;
          char *end = search_bytes (p, line_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          если (!end && available)
            continue;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      если (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          char const *p = mbbuf.buffer + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          char const *end = search_bytes (p, line_delim, n);
          если (end)
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

      если (g.ch == line_delim)
        reset_item_line (&idx, &print_delimiter);
      иначе если (g.ch == MBBUF_EOF)
        {
          write_pending_line_delim (idx);
          break;
        }
      иначе если (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          для (idx_t i = 0; i < g.len; i++)
            {
              next_item (&idx);
              если (print_kth (idx))
                {
                  если (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_index (idx);
                    }
                }
              иначе если (seen_selected)
                suffix_selected = false;
            }

          если (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      иначе
        {
          next_item (&idx);
          если (print_kth (idx))
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
  static char bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  struct mbfield_parser parser =
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
  mbbuf_init (&mbbuf, bytes_in, sizeof bytes_in, stream);

  buffer_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  пока (true)
    {
      enum field_terminator terminator;
      если (!buffer_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_line,
                                    write_field, NULL);
      иначе
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_line,
                                    false, &field_1_n_bytes);

      если (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          если (! field_selection_exhausted (field_idx))
            continue;

          terminator = scan_mb_line_end (&mbbuf, &parser,
                                         &have_pending_line);
        }

      если (terminator == FIELD_EOF && !have_pending_line)
        break;

      finish_current_line (field_idx, buffer_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_LINE_DELIMITER);
      если (terminator == FIELD_EOF)
        break;

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
  static char bytes_in[IO_BUFSIZE+1];
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
  mbbuf_init (&mbbuf, bytes_in, sizeof bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  struct bytesearch_context search = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&search);

  пока (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      search.at_eof = mbbuf.eof;
      search.line_end_known = false;

      если (pending_line_delim_field)
        {
          pending_line_delim_field = false;

          если (n_avail == 0)
            {
              если (handle_field_1 (field_idx, buffer_first_field,
                                  &field_1_n_bytes))
                next_item (&field_idx);
              maybe_write_line_delim (found_any_selected_field, field_idx);
              вернуть;
            }

          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          если (whitespace_delimited)
            skip_blank_run = true;
        }

      если (n_avail == 0)
        break;

      char *chunk = mbbuf.buffer + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      если (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_line_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          char *last_line_delim = search.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, line_delim, n_avail);
          если (last_line_delim)
            {
              idx_t n = last_line_delim - chunk + 1;
              если (! suppress_non_delimited)
                {
                  write_bytes (field_1_buffer, field_1_n_bytes);
                  write_bytes (chunk, n);
                  если (search.at_eof && chunk[n - 1] != line_delim)
                    write_line_delim ();
                }
              field_1_n_bytes = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_line = false;
              если (search.at_eof)
                вернуть;
              continue;
            }
        }

      пока (processed < n_avail)
        {
          char *terminator = NULL;

          если (skip_blank_run)
            {
              пока (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              если (processed == n_avail)
                break;
              skip_blank_run = false;
            }

          если (field_selection_exhausted (field_idx))
            {
              если (field_delim_is_line_delim ())
                {
                  maybe_write_line_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  вернуть;
                }
              search.mode = BYTESEARCH_LINE_ONLY;
              enum field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &search, &terminator);
              если (terminator_kind == FIELD_LINE_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_line_delim (found_any_selected_field,
                                          field_idx);
                  goto reset_line;
                }
              processed = n_avail;
              break;
            }

          enum field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &search, &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          если (terminator_kind == FIELD_DATA
              && !search.at_eof
              && !whitespace_delimited
              && !field_delim_is_line_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          если (field_len || terminator)
            have_pending_line = true;

          /* Output field content.  */
          если (field_idx == 1 && buffer_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          иначе если (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          если (terminator_kind == FIELD_DATA)
            break;

          если (terminator_kind == FIELD_DELIMITER)
            {
              если (field_delim_is_line_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_line_delim_field = true;
                  break;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, buffer_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              если (whitespace_delimited)
                skip_blank_run = true;
            }
          иначе
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

  если (!have_pending_line)
    вернуть;

  finish_current_line (field_idx, buffer_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

static void
cut_fields_ws (FILE *stream)
{
  если (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  иначе
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

static bool
cut_file (char const *file, void (*cut_stream) (FILE *))
{
  FILE *stream;

  если (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  иначе
    {
      stream = fopen (file, "r");
      если (stream == NULL)
        {
          error (0, errno, "%s", quotef (file));
          вернуть false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  целое err = errno;
  если (!ferror (stream))
    err = 0;
  если (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF.  */
  иначе если (fclose (stream) == EOF)
    err = errno;
  если (err)
    {
      error (0, err, "%s", quotef (file));
      вернуть false;
    }
  вернуть true;
}

целое
основной (целое argc, char **argv)
{
  целое optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  char *spec_list_string = NULL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  пока ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, NULL))
         != -1)
    {
      switch (optc)
        {
        case 'b':
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        case 'c':
          если (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        case 'f':
        case 'F':
          если (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
          если (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          если (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          break;

        case 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            если (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            break;
          }

        case 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          break;

        case 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_length = (optarg[0] == '\0'
                                     ? 1 : strlen (optarg));
          output_delimiter_string = optarg;
          break;

        case 'n':
          no_split = true;
          break;

        case 's':
          suppress_non_delimited = true;
          break;

        case 'z':
          line_delim = '\0';
          break;

        case COMPLEMENT_OPTION:
          complement = true;
          break;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  если (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  если (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
    {
      если (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an input delimiter makes sense\n\
\tonly when operating on fields"));

      если (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited lines makes sense\n\
\tonly when operating on fields"));
    }

  если (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  если (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  если (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  если (output_delimiter_string == NULL)
    {
      output_delimiter_string = output_delimiter_default;
      если (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_length = 1;
        }
      иначе
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_length = delim_mcel.len;
        }
    }

  void (*cut_stream) (FILE *) = NULL;
  switch (cut_mode)
    {
    case CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      break;

    case CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      break;

    case CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      break;

    case CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  если (optind == argc)
    ok = cut_file ("-", cut_stream);
  иначе
    для (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  если (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  вернуть ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
