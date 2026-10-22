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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "argmatch.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "ioblksize.h"
#ଅନ୍ତର୍ଭାଵ "mbbuf.h"
#ଅନ୍ତର୍ଭାଵ "memchr2.h"

#ଅନ୍ତର୍ଭାଵ "set-fields.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cut"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#define FATAL_ERROR(Message)						\
  କର									\
    {									\
      error (0, 0, (Message));						\
      usage (EXIT_FAILURE);						\
    }									\
  ଯେତେବେଳେ (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#define SMALL_BYTE_THRESHOLD 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
ସ୍ଥିର ସଂରଚନା field_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
ସ୍ଥିର ଅକ୍ଷର *field_1_buffer;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
ସ୍ଥିର idx_t field_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
ସ୍ଥିର bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
ସ୍ଥିର bool complement;

/* The delimiter character for multibyte field mode.  */
ସ୍ଥିର mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
ସ୍ଥିର ଅକ୍ଷର delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
ସ୍ଥିର unsigned ଅକ୍ଷର ଧାଡ଼ି_delim = '\n';

/* The length of output_delimiter_string.  */
ସ୍ଥିର size_t output_delimiter_length;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
ସ୍ଥିର ଅକ୍ଷର *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
ସ୍ଥିର ଅକ୍ଷର output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
ସ୍ଥିର bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
ସ୍ଥିର bool no_split;

/* If true, interpret each run of whitespace as one field delimiter.  */
ସ୍ଥିର bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
ସ୍ଥିର bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
ସ୍ଥିର bool space_output_delimiter_default;

ଗଣନା whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ whitespace_option_args[] =
{
  "trimmed", ଖାଲି
};

ସ୍ଥିର ଗଣନା whitespace_option ଅଚଳ whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_args, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
ସ୍ଥିର ଗଣନା
{
  CUT_MODE_NONE,
  CUT_MODE_BYTES,
  CUT_MODE_CHARACTERS,
  CUT_MODE_FIELDS
} cut_mode = CUT_MODE_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"bytes", required_argument, ଖାଲି, 'b'},
  {"characters", required_argument, ଖାଲି, 'c'},
  {"fields", required_argument, ଖାଲି, 'f'},
  {"delimiter", required_argument, ଖାଲି, 'd'},
  {"no-partial", no_argument, ଖାଲି, 'n'},
  {"whitespace-delimited", optional_argument, ଖାଲି, 'w'},
  {"only-delimited", no_argument, ଖାଲି, 's'},
  {"output-delimiter", required_argument, ଖାଲି, 'O'},
  {"complement", no_argument, ଖାଲି, COMPLEMENT_OPTION},
  {"zero-terminated", no_argument, ଖାଲି, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Print selected parts of ଧାଡ଼ିଗୁଡ଼ିକ from each FILE to standard output.\n\
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
         use DELIM instead of TAB ପାଇଁ field delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any ଧାଡ଼ି that contains\n\
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
         the ପୂର୍ବନିର୍ଦ୍ଧାରିତ is to use the ନିବେଶ delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         କର not print ଧାଡ଼ିଗୁଡ଼ିକ not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a run of blank characters as the field delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ଧାଡ଼ି delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one of -b, -c, -f or -F.  Each LIST is made up of one\n\
range, or many ranges separated by commas.  Selected ନିବେଶ is written\n\
in the same order that it is ପଠ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one of:\n\
\n\
  N     N'th byte, character or field, counted from 1\n\
  N-    from N'th byte, character or field, to end of ଧାଡ଼ି\n\
  N-M   from N'th to M'th (included) byte, character or field\n\
  -M    from first to M'th (included) byte, character or field\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
next_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  ଯଦି ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
print_kth (uintmax_t k)
{
  ଫେରନ୍ତୁ current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_range_start_index (uintmax_t k)
{
  ଫେରନ୍ତୁ k == current_rp->lo;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
field_delim_is_ଧାଡ଼ି_delim (ଶୂନ୍ୟ)
{
  ଫେରନ୍ତୁ delim_mcel.len == 1 && delim_bytes[0] == ଧାଡ଼ି_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

ସ୍ଥିର bool
mcel_isblank (mcel_t g)
{
  ଫେରନ୍ତୁ (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
bytesearch_field_delim_ok (ଶୂନ୍ୟ)
{
  unsigned ଅକ୍ଷର delim_0 = delim_bytes[0];

  ଫେରନ୍ତୁ (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


ଗଣନା field_terminator
{
  FIELD_DATA,
  FIELD_DELIMITER,
  FIELD_ଧାଡ଼ି_DELIMITER,
  FIELD_EOF
};

ଗଣନା bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_ଧାଡ଼ି_ONLY
};

ସଂରଚନା bytesearch_context
{
  ଗଣନା bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  ଅକ୍ଷର *ଧାଡ଼ି_end;
  bool ଧାଡ଼ି_end_known;
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
bytesearch_context_reset (ସଂରଚନା bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->ଧାଡ଼ି_end = ଖାଲି;
  ctx->ଧାଡ଼ି_end_known = false;
}

ସଂରଚନା mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_ଧାଡ଼ି_start;
  bool have_saved;
  mcel_t saved_g;
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  ଯଦି (*have_saved)
    {
      *have_saved = false;
      ଫେରନ୍ତୁ *saved_g;
    }
  ଫେରନ୍ତୁ mbbuf_get_char (mbbuf);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା field_terminator
skip_whitespace_run (mbbuf_t *mbuf, ସଂରଚନା mbfield_parser *parser,
                     bool *have_pending_ଧାଡ଼ି,
                     bool have_initial_whitespace)
{
  mcel_t g;

  କର
    {
      g = mbbuf_get_char (mbuf);
      ଯଦି (g.ch != MBBUF_EOF)
        *have_pending_ଧାଡ଼ି = true;
    }
  ଯେତେବେଳେ (g.ch != MBBUF_EOF && g.ch != ଧାଡ଼ି_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_ଧାଡ଼ି_start;

  ଯଦି (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == ଧାଡ଼ି_delim))
    {
      ଫେରନ୍ତୁ g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_ଧାଡ଼ି_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  ଫେରନ୍ତୁ trim_start && !have_initial_whitespace ? FIELD_DATA : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

ସ୍ଥିର ଶୂନ୍ୟ
write_bytes (ଅକ୍ଷର ଅଚଳ *buf, size_t n_bytes)
{
  ଯଦି (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      ପାଇଁ (size_t i = 0; i < n_bytes; i++)
        ଯଦି (putchar (buf[i]) < 0)
          write_error ();
      ଫେରନ୍ତୁ;
    }

  ଯଦି (fwrite (buf, ଆକାର (ଅକ୍ଷର), n_bytes, stdout) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
copy_bytes (ଅକ୍ଷର *dst, ଅକ୍ଷର ଅଚଳ *src, size_t n_bytes)
{
  ଯଦି (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      ପାଇଁ (size_t i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      ଫେରନ୍ତୁ;
    }

  memcpy (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ*
ସନ୍ଧାନ_bytes (ଅକ୍ଷର ଅଚଳ *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, size_t n_bytes)
{
  ଯଦି (n_bytes != 0 && to_uchar (buf[0]) == c)
    ଫେରନ୍ତୁ (ଅକ୍ଷର *) buf;
  ଯଦି (1 < n_bytes && to_uchar (buf[1]) == c)
    ଫେରନ୍ତୁ (ଅକ୍ଷର *) buf + 1;

  ଫେରନ୍ତୁ 2 < n_bytes ? (ଅକ୍ଷର *) memchr (buf + 2, c, n_bytes - 2) : ଖାଲି;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
write_ଧାଡ଼ି_delim (ଶୂନ୍ୟ)
{
  ଯଦି (putchar (ଧାଡ଼ି_delim) < 0)
    write_error ();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
reset_item_ଧାଡ଼ି (uintmax_t *item_idx, bool *print_delimiter)
{
  write_ଧାଡ଼ି_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
write_pending_ଧାଡ଼ି_delim (uintmax_t item_idx)
{
  ଯଦି (item_idx > 0)
    write_ଧାଡ଼ି_delim ();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
write_selected_item (bool *print_delimiter, bool range_start,
                     ଅକ୍ଷର ଅଚଳ *buf, size_t n_bytes)
{
  ଯଦି (output_delimiter_string != output_delimiter_default)
    {
      ଯଦି (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
append_field_1_chunk (ଅକ୍ଷର ଅଚଳ *buf, idx_t len, idx_t *n_bytes)
{
  ଯଦି (field_1_bufsize - *n_bytes < len)
    {
      field_1_buffer = xpalloc (field_1_buffer, &field_1_bufsize,
                                len, -1, ଆକାର *field_1_buffer);
    }

  copy_bytes (field_1_buffer + *n_bytes, buf, len);
  *n_bytes += len;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, ସଂରଚନା mbfield_parser *parser,
                     bool *have_pending_ଧାଡ଼ି, bool write_field,
                     idx_t *n_bytes)
{
  ଯଦି (parser->trim_outer_whitespace && parser->at_ଧାଡ଼ି_start)
    {
      ଗଣନା field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_ଧାଡ଼ି, false);
      ଯଦି (terminator != FIELD_DATA)
        ଫେରନ୍ତୁ terminator;
    }

  parser->at_ଧାଡ଼ି_start = false;

  ଯେତେବେଳେ (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      ଯଦି (g.ch == MBBUF_EOF)
        ଫେରନ୍ତୁ FIELD_EOF;

      *have_pending_ଧାଡ଼ି = true;

      ଯଦି (g.ch == ଧାଡ଼ି_delim)
        ଫେରନ୍ତୁ FIELD_ଧାଡ଼ି_DELIMITER;

      ଯଦି (mcel_isblank (g))
        ଫେରନ୍ତୁ skip_whitespace_run (mbbuf, parser, have_pending_ଧାଡ଼ି, true);

      ଯଦି (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      ଅନ୍ୟଥା ଯଦି (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_ଧାଡ଼ି,
                     bool write_field, idx_t *n_bytes)
{
  ଯେତେବେଳେ (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      ଯଦି (g.ch == MBBUF_EOF)
        ଫେରନ୍ତୁ FIELD_EOF;

      *have_pending_ଧାଡ଼ି = true;

      ଯଦି (g.ch == ଧାଡ଼ି_delim)
        ଫେରନ୍ତୁ FIELD_ଧାଡ଼ି_DELIMITER;

      ଯଦି (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        ଫେରନ୍ତୁ FIELD_DELIMITER;

      ଯଦି (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      ଅନ୍ୟଥା ଯଦି (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା field_terminator
scan_mb_field (mbbuf_t *mbbuf, ସଂରଚନା mbfield_parser *parser,
               bool *have_pending_ଧାଡ଼ି, bool write_field, idx_t *n_bytes)
{
  ଫେରନ୍ତୁ (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_ଧାଡ଼ି,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_ଧାଡ଼ି, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

ସ୍ଥିର ଗଣନା field_terminator
scan_mb_ଧାଡ଼ି_end (mbbuf_t *mbbuf, ସଂରଚନା mbfield_parser *parser,
                  bool *have_pending_ଧାଡ଼ି)
{
  ଯଦି (parser->have_saved)
    {
      parser->have_saved = false;
      ଯଦି (parser->saved_g.ch == MBBUF_EOF)
        ଫେରନ୍ତୁ FIELD_EOF;

      *have_pending_ଧାଡ଼ି = true;
      ଯଦି (parser->saved_g.ch == ଧାଡ଼ି_delim)
        ଫେରନ୍ତୁ FIELD_ଧାଡ଼ି_DELIMITER;
    }

  ଯେତେବେଳେ (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      ଯଦି (available == 0)
        ଫେରନ୍ତୁ FIELD_EOF;

      ଅକ୍ଷର *buffer = mbbuf->buffer + mbbuf->offset;
      ଅକ୍ଷର *ଧାଡ଼ି_end = ସନ୍ଧାନ_bytes (buffer, ଧାଡ଼ି_delim, available);
      idx_t n = ଧାଡ଼ି_end ? ଧାଡ଼ି_end - buffer + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_ଧାଡ଼ି = true;

      ଯଦି (ଧାଡ଼ି_end)
        ଫେରନ୍ତୁ FIELD_ଧାଡ଼ି_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
ସ୍ଥିର ଅକ୍ଷର *
find_field_delim (ଅକ୍ଷର *buf, size_t len)
{
  ଯଦି (len < delim_mcel.len)
    ଫେରନ୍ତୁ ଖାଲି;

#ଯଦି ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  ଫେରନ୍ତୁ memmem (buf, len, delim_bytes, delim_mcel.len);
#ଅନ୍ୟଥା
  ଯଦି (delim_mcel.len == 1)
    ଫେରନ୍ତୁ memchr (buf, delim_bytes[0], len);

  ଅକ୍ଷର ଅଚଳ *p = buf;
  ଅକ୍ଷର ଅଚଳ *end = buf + len;

  ଅକ୍ଷର saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  ଯେତେବେଳେ (p < end)
    {
      ଅକ୍ଷର ଅଚଳ *nul = memchr (p, '\0', end - p);
      ଯଦି (!nul)
        {
          ଅକ୍ଷର ଅଚଳ *match = strstr (p, delim_bytes);
          buf[len] = saved;
          ଫେରନ୍ତୁ (ଅକ୍ଷର *) match;
        }

      ଯଦି (p < nul)
        {
          ଅକ୍ଷର ଅଚଳ *match = strstr (p, delim_bytes);
          ଯଦି (match)
            {
              buf[len] = saved;
              ଫେରନ୍ତୁ (ଅକ୍ଷର *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  ଫେରନ୍ତୁ ଖାଲି;
#endif
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
ସ୍ଥିର ଅକ୍ଷର *
find_c_blank (ଅକ୍ଷର *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  ପାଇଁ (idx_t i = 0; i < n; i++)
    ଯଦି (c_isblank (buf[i]))
      ଫେରନ୍ତୁ buf + i;

  ଫେରନ୍ତୁ memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_PURE
ସ୍ଥିର idx_t
field_delim_overlap (ଅକ୍ଷର ଅଚଳ *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  ଯେତେବେଳେ (0 < overlap)
    {
      ଯଦି (memeq (buf + len - overlap, delim_bytes, overlap))
        ଫେରନ୍ତୁ overlap;
      overlap--;
    }

  ଫେରନ୍ତୁ 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା field_terminator
find_field_terminator (ଅକ୍ଷର *buf, idx_t len,
                       ସଂରଚନା bytesearch_context *ctx, ଅକ୍ଷର **terminator)
{
  ଯଦି (ctx->mode == BYTESEARCH_ଧାଡ଼ି_ONLY)
    {
      ଯଦି (!ctx->ଧାଡ଼ି_end_known)
        {
          ctx->ଧାଡ଼ି_end = memchr (buf, ଧାଡ଼ି_delim, len);
          ctx->ଧାଡ଼ି_end_known = true;
        }

      *terminator = ctx->ଧାଡ଼ି_end;
      ଫେରନ୍ତୁ *terminator ? FIELD_ଧାଡ଼ି_DELIMITER : FIELD_DATA;
    }

  ଯଦି (field_delim_is_ଧାଡ଼ି_delim ())
    {
      *terminator = memchr (buf, ଧାଡ଼ି_delim, len);
      ଫେରନ୍ତୁ *terminator ? FIELD_DELIMITER : FIELD_DATA;
    }

  ଯଦି (!ctx->ଧାଡ଼ି_end_known)
    {
      ctx->ଧାଡ଼ି_end = memchr (buf, ଧାଡ଼ି_delim, len);
      ctx->ଧାଡ଼ି_end_known = true;
    }

  idx_t field_len = ctx->ଧାଡ଼ି_end ? ctx->ଧାଡ଼ି_end - buf : len;

  ଅକ୍ଷର *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  ଯଦି (field_end)
    {
      *terminator = field_end;
      ଫେରନ୍ତୁ FIELD_DELIMITER;
    }

  *terminator = ctx->ଧାଡ଼ି_end;
  ଫେରନ୍ତୁ ctx->ଧାଡ଼ି_end ? FIELD_ଧାଡ଼ି_DELIMITER : FIELD_DATA;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
maybe_write_ଧାଡ଼ି_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  ଯଦି (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_ଧାଡ଼ି_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
begin_field_output (uintmax_t field_idx, bool buffer_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  ଯଦି (write_field && ! (field_idx == 1 && buffer_first_field))
    {
      ଯଦି (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *found_any_selected_field = true;
    }

  ଫେରନ୍ତୁ write_field;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
handle_field_1 (uintmax_t field_idx, bool buffer_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  ଯଦି (field_idx == 1 && buffer_first_field)
    {
      ଯଦି ((field_1_selected = print_kth (1)))
        write_bytes (field_1_buffer, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  ଫେରନ୍ତୁ field_1_selected;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
handle_field_delimiter (uintmax_t *field_idx, bool buffer_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  ଯଦି (handle_field_1 (*field_idx, buffer_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  next_item (field_idx);
  *write_field = begin_field_output (*field_idx, buffer_first_field,
                                     found_any_selected_field);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
field_selection_exhausted (uintmax_t field_idx)
{
  ଫେରନ୍ତୁ !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
finish_current_ଧାଡ଼ି (uintmax_t field_idx, bool buffer_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool ଧାଡ଼ି_terminated)
{
  ଯଦି (field_idx == 1 && buffer_first_field)
    {
      ଯଦି (!suppress_non_delimited
          && (ଧାଡ଼ି_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_buffer, *field_1_n_bytes);
          write_ଧାଡ଼ି_delim ();
        }
      *field_1_n_bytes = 0;
    }
  ଅନ୍ୟଥା
    maybe_write_ଧାଡ଼ି_delim (found_any_selected_field, field_idx);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
sync_byte_selection (uintmax_t byte_idx)
{
  ଯେତେବେଳେ (current_rp->hi <= byte_idx)
    current_rp++;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
reset_field_ଧାଡ଼ି (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_ଧାଡ଼ି, ସଂରଚନା mbfield_parser *parser)
{
  *field_idx = 1;
  current_rp = frp;
  *found_any_selected_field = false;
  *have_pending_ଧାଡ଼ି = false;
  parser->have_saved = false;
  parser->at_ଧାଡ଼ି_start = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

ସ୍ଥିର ଶୂନ୍ୟ
cut_bytes (FILE *stream)
{
  ସ୍ଥିର ଅକ୍ଷର bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = fileno (stream);

  current_rp = frp;

  ଯେତେବେଳେ (true)
    {
      idx_t available = ପଠ (fd, bytes_in, ଆକାର bytes_in);
      ଯଦି (available <= 0)
        {
          ଯଦି (available < 0)
            fseterr (stream);
          write_pending_ଧାଡ଼ି_delim (byte_idx);
          ଭାଙ୍ଗ;
        }

      idx_t processed = 0;

      ଯେତେବେଳେ (processed < available)
        {
          ଅକ୍ଷର *ଧାଡ଼ି = bytes_in + processed;
          idx_t to_process = available - processed;
          ଅକ୍ଷର *ଧାଡ଼ି_end = ସନ୍ଧାନ_bytes (ଧାଡ଼ି, ଧାଡ଼ି_delim, to_process);
          ଅକ୍ଷର *end = ଧାଡ଼ି + (ଧାଡ଼ି_end ? ଧାଡ଼ି_end - ଧାଡ଼ି : to_process);
          ଅକ୍ଷର *p = ଧାଡ଼ି;

          ଯେତେବେଳେ (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              ଯଦି (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              ଅନ୍ୟଥା
                {
                  idx_t n = MIN (end - p, current_rp->hi - byte_idx);
                  write_selected_item (&print_delimiter,
                                       is_range_start_index (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += end - ଧାଡ଼ି;
          ଯଦି (ଧାଡ଼ି_end)
            {
              processed++;
              reset_item_ଧାଡ଼ି (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

ସ୍ଥିର ଶୂନ୍ୟ
cut_characters_mode (FILE *stream, bool byte_mode)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  ସ୍ଥିର ଅକ୍ଷର bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, ଆକାର bytes_in, stream);

  ଯେତେବେଳେ (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      ଯଦି (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          ଅକ୍ଷର *p = mbbuf.buffer + mbbuf.offset;
          ଅକ୍ଷର *end = ସନ୍ଧାନ_bytes (p, ଧାଡ଼ି_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          ଯଦି (!end && available)
            ଜାରି;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      ଯଦି (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          ଅକ୍ଷର ଅଚଳ *p = mbbuf.buffer + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          ଅକ୍ଷର ଅଚଳ *end = ସନ୍ଧାନ_bytes (p, ଧାଡ଼ି_delim, n);
          ଯଦି (end)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, end - p + 1);
              reset_item_ଧାଡ଼ି (&idx, &print_delimiter);
              ଜାରି;
            }

          idx += u8_ଗଣନା (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      ଯଦି (g.ch == ଧାଡ଼ି_delim)
        reset_item_ଧାଡ଼ି (&idx, &print_delimiter);
      ଅନ୍ୟଥା ଯଦି (g.ch == MBBUF_EOF)
        {
          write_pending_ଧାଡ଼ି_delim (idx);
          ଭାଙ୍ଗ;
        }
      ଅନ୍ୟଥା ଯଦି (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          ପାଇଁ (idx_t i = 0; i < g.len; i++)
            {
              next_item (&idx);
              ଯଦି (print_kth (idx))
                {
                  ଯଦି (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_index (idx);
                    }
                }
              ଅନ୍ୟଥା ଯଦି (seen_selected)
                suffix_selected = false;
            }

          ଯଦି (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      ଅନ୍ୟଥା
        {
          next_item (&idx);
          ଯଦି (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_index (idx),
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
cut_bytes_no_split (FILE *stream)
{
  cut_characters_mode (stream, true);
}

ସ୍ଥିର ଶୂନ୍ୟ
cut_characters (FILE *stream)
{
  cut_characters_mode (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

ସ୍ଥିର ଶୂନ୍ୟ
cut_fields_mb_any (FILE *stream, bool whitespace_mode)
{
  ସ୍ଥିର ଅକ୍ଷର bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  ସଂରଚନା mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_mode,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_ଧାଡ଼ି_start = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool buffer_first_field;
  bool have_pending_ଧାଡ଼ି = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, ଆକାର bytes_in, stream);

  buffer_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  ଯେତେବେଳେ (true)
    {
      ଗଣନା field_terminator terminator;
      ଯଦି (!buffer_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_ଧାଡ଼ି,
                                    write_field, ଖାଲି);
      ଅନ୍ୟଥା
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_ଧାଡ଼ି,
                                    false, &field_1_n_bytes);

      ଯଦି (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          ଯଦି (! field_selection_exhausted (field_idx))
            ଜାରି;

          terminator = scan_mb_ଧାଡ଼ି_end (&mbbuf, &parser,
                                         &have_pending_ଧାଡ଼ି);
        }

      ଯଦି (terminator == FIELD_EOF && !have_pending_ଧାଡ଼ି)
        ଭାଙ୍ଗ;

      finish_current_ଧାଡ଼ି (field_idx, buffer_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_ଧାଡ଼ି_DELIMITER);
      ଯଦି (terminator == FIELD_EOF)
        ଭାଙ୍ଗ;

      reset_field_ଧାଡ଼ି (&field_idx, &found_any_selected_field,
                        &have_pending_ଧାଡ଼ି, &parser);
      write_field = begin_field_output (field_idx, buffer_first_field,
                                        &found_any_selected_field);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

ସ୍ଥିର ଶୂନ୍ୟ
cut_fields_mb (FILE *stream)
{
  cut_fields_mb_any (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

ସ୍ଥିର ଶୂନ୍ୟ
cut_fields_bytesearch (FILE *stream)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  ସ୍ଥିର ଅକ୍ଷର bytes_in[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool have_pending_ଧାଡ଼ି = false;
  bool skip_blank_run = false;
  bool pending_ଧାଡ଼ି_delim_field = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  bool buffer_first_field = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, bytes_in, ଆକାର bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  ସଂରଚନା bytesearch_context ସନ୍ଧାନ = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&ସନ୍ଧାନ);

  ଯେତେବେଳେ (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      ସନ୍ଧାନ.at_eof = mbbuf.eof;
      ସନ୍ଧାନ.ଧାଡ଼ି_end_known = false;

      ଯଦି (pending_ଧାଡ଼ି_delim_field)
        {
          pending_ଧାଡ଼ି_delim_field = false;

          ଯଦି (n_avail == 0)
            {
              ଯଦି (handle_field_1 (field_idx, buffer_first_field,
                                  &field_1_n_bytes))
                next_item (&field_idx);
              maybe_write_ଧାଡ଼ି_delim (found_any_selected_field, field_idx);
              ଫେରନ୍ତୁ;
            }

          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          ଯଦି (whitespace_delimited)
            skip_blank_run = true;
        }

      ଯଦି (n_avail == 0)
        ଭାଙ୍ଗ;

      ଅକ୍ଷର *chunk = mbbuf.buffer + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      ଯଦି (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_ଧାଡ଼ି_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          ଅକ୍ଷର *last_ଧାଡ଼ି_delim = ସନ୍ଧାନ.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, ଧାଡ଼ି_delim, n_avail);
          ଯଦି (last_ଧାଡ଼ି_delim)
            {
              idx_t n = last_ଧାଡ଼ି_delim - chunk + 1;
              ଯଦି (! suppress_non_delimited)
                {
                  write_bytes (field_1_buffer, field_1_n_bytes);
                  write_bytes (chunk, n);
                  ଯଦି (ସନ୍ଧାନ.at_eof && chunk[n - 1] != ଧାଡ଼ି_delim)
                    write_ଧାଡ଼ି_delim ();
                }
              field_1_n_bytes = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_ଧାଡ଼ି = false;
              ଯଦି (ସନ୍ଧାନ.at_eof)
                ଫେରନ୍ତୁ;
              ଜାରି;
            }
        }

      ଯେତେବେଳେ (processed < n_avail)
        {
          ଅକ୍ଷର *terminator = ଖାଲି;

          ଯଦି (skip_blank_run)
            {
              ଯେତେବେଳେ (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              ଯଦି (processed == n_avail)
                ଭାଙ୍ଗ;
              skip_blank_run = false;
            }

          ଯଦି (field_selection_exhausted (field_idx))
            {
              ଯଦି (field_delim_is_ଧାଡ଼ି_delim ())
                {
                  maybe_write_ଧାଡ଼ି_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  ଫେରନ୍ତୁ;
                }
              ସନ୍ଧାନ.mode = BYTESEARCH_ଧାଡ଼ି_ONLY;
              ଗଣନା field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &ସନ୍ଧାନ, &terminator);
              ଯଦି (terminator_kind == FIELD_ଧାଡ଼ି_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_ଧାଡ଼ି_delim (found_any_selected_field,
                                          field_idx);
                  ଯାଅ reset_ଧାଡ଼ି;
                }
              processed = n_avail;
              ଭାଙ୍ଗ;
            }

          ଗଣନା field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &ସନ୍ଧାନ, &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          ଯଦି (terminator_kind == FIELD_DATA
              && !ସନ୍ଧାନ.at_eof
              && !whitespace_delimited
              && !field_delim_is_ଧାଡ଼ି_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          ଯଦି (field_len || terminator)
            have_pending_ଧାଡ଼ି = true;

          /* Output field content.  */
          ଯଦି (field_idx == 1 && buffer_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          ଅନ୍ୟଥା ଯଦି (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          ଯଦି (terminator_kind == FIELD_DATA)
            ଭାଙ୍ଗ;

          ଯଦି (terminator_kind == FIELD_DELIMITER)
            {
              ଯଦି (field_delim_is_ଧାଡ଼ି_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_ଧାଡ଼ି_delim_field = true;
                  ଭାଙ୍ଗ;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, buffer_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              ଯଦି (whitespace_delimited)
                skip_blank_run = true;
            }
          ଅନ୍ୟଥା
            {
              affirm (terminator_kind == FIELD_ଧାଡ଼ି_DELIMITER);
              processed++;
              finish_current_ଧାଡ଼ି (field_idx, buffer_first_field,
                                   &field_1_n_bytes,
                                   found_any_selected_field, true);

            reset_ଧାଡ଼ି:
              field_idx = 1;
              current_rp = frp;
              found_any_selected_field = false;
              have_pending_ଧାଡ଼ି = false;
              bytesearch_context_reset (&ସନ୍ଧାନ);
              write_field = begin_field_output (field_idx, buffer_first_field,
                                                &found_any_selected_field);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  ଯଦି (!have_pending_ଧାଡ଼ି)
    ଫେରନ୍ତୁ;

  finish_current_ଧାଡ଼ି (field_idx, buffer_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

ସ୍ଥିର ଶୂନ୍ୟ
cut_fields_ws (FILE *stream)
{
  ଯଦି (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  ଅନ୍ୟଥା
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

ସ୍ଥିର bool
cut_file (ଅକ୍ଷର ଅଚଳ *file, ଶୂନ୍ୟ (*cut_stream) (FILE *))
{
  FILE *stream;

  ଯଦି (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  ଅନ୍ୟଥା
    {
      stream = fopen (file, "r");
      ଯଦି (stream == ଖାଲି)
        {
          error (0, errno, "%s", quotef (file));
          ଫେରନ୍ତୁ false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = errno;
  ଯଦି (!ferror (stream))
    err = 0;
  ଯଦି (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF.  */
  ଅନ୍ୟଥା ଯଦି (fclose (stream) == EOF)
    err = errno;
  ଯଦି (err)
    {
      error (0, err, "%s", quotef (file));
      ଫେରନ୍ତୁ false;
    }
  ଫେରନ୍ତୁ true;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  ଅକ୍ଷର *spec_list_string = ଖାଲି;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, ଖାଲି))
         != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'b':
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        ସ୍ଥିତି 'c':
          ଯଦି (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        ସ୍ଥିତି 'f':
        ସ୍ଥିତି 'F':
          ଯଦି (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
          ଯଦି (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          ଯଦି (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            ଯଦି (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            ଭାଙ୍ଗ;
          }

        ସ୍ଥିତି 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_length = (optarg[0] == '\0'
                                     ? 1 : strlen (optarg));
          output_delimiter_string = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'n':
          no_split = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 's':
          suppress_non_delimited = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'z':
          ଧାଡ଼ି_delim = '\0';
          ଭାଙ୍ଗ;

        ସ୍ଥିତି COMPLEMENT_OPTION:
          complement = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  ଯଦି (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
    {
      ଯଦି (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an ନିବେଶ delimiter makes sense\n\
\tonly when operating on fields"));

      ଯଦି (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited ଧାଡ଼ିଗୁଡ଼ିକ makes sense\n\
\tonly when operating on fields"));
    }

  ଯଦି (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  ଯଦି (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  ଯଦି (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  ଯଦି (output_delimiter_string == ଖାଲି)
    {
      output_delimiter_string = output_delimiter_default;
      ଯଦି (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_length = 1;
        }
      ଅନ୍ୟଥା
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_length = delim_mcel.len;
        }
    }

  ଶୂନ୍ୟ (*cut_stream) (FILE *) = ଖାଲି;
  ବାଛ (cut_mode)
    {
    ସ୍ଥିତି CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  ଯଦି (optind == argc)
    ok = cut_file ("-", cut_stream);
  ଅନ୍ୟଥା
    ପାଇଁ (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  ଯଦି (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
