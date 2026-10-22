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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "mbbuf.h"
#ಅನ್ತರ್ಭಾವ "memchr2.h"

#ಅನ್ತರ್ಭಾವ "set-fields.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cut"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#define FATAL_ERROR(Message)						\
  ಕುರು									\
    {									\
      error (0, 0, (Message));						\
      usage (EXIT_FAILURE);						\
    }									\
  ಯಾವತ್ (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#define SMALL_BYTE_THRESHOLD 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
ಸ್ಥಿರ ಸಂರಚನಾ field_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
ಸ್ಥಿರ ವರ್ಣ *field_1_buffer;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
ಸ್ಥಿರ idx_t field_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
ಸ್ಥಿರ bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
ಸ್ಥಿರ bool complement;

/* The delimiter character for multibyte field mode.  */
ಸ್ಥಿರ mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
ಸ್ಥಿರ ವರ್ಣ delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
ಸ್ಥಿರ unsigned ವರ್ಣ ಸಾಲು_delim = '\n';

/* The length of output_delimiter_string.  */
ಸ್ಥಿರ size_t output_delimiter_length;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
ಸ್ಥಿರ ವರ್ಣ *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
ಸ್ಥಿರ ವರ್ಣ output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
ಸ್ಥಿರ bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
ಸ್ಥಿರ bool no_split;

/* If true, interpret each run of whitespace as one field delimiter.  */
ಸ್ಥಿರ bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
ಸ್ಥಿರ bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
ಸ್ಥಿರ bool space_output_delimiter_default;

ಗಣನಾ whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ whitespace_option_args[] =
{
  "trimmed", ರಿಕ್ತ
};

ಸ್ಥಿರ ಗಣನಾ whitespace_option ಧ್ರುವ whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_args, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
ಸ್ಥಿರ ಗಣನಾ
{
  CUT_MODE_NONE,
  CUT_MODE_BYTES,
  CUT_MODE_CHARACTERS,
  CUT_MODE_FIELDS
} cut_mode = CUT_MODE_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"bytes", required_argument, ರಿಕ್ತ, 'b'},
  {"characters", required_argument, ರಿಕ್ತ, 'c'},
  {"fields", required_argument, ರಿಕ್ತ, 'f'},
  {"delimiter", required_argument, ರಿಕ್ತ, 'd'},
  {"no-partial", no_argument, ರಿಕ್ತ, 'n'},
  {"whitespace-delimited", optional_argument, ರಿಕ್ತ, 'w'},
  {"only-delimited", no_argument, ರಿಕ್ತ, 's'},
  {"output-delimiter", required_argument, ರಿಕ್ತ, 'O'},
  {"complement", no_argument, ರಿಕ್ತ, COMPLEMENT_OPTION},
  {"zero-terminated", no_argument, ರಿಕ್ತ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Print selected parts of ಸಾಲುಗಳು from each FILE to standard output.\n\
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
         use DELIM instead of TAB ಪ್ರತಿ field delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any ಸಾಲು that contains\n\
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
         the ಸ್ವಭಾವ is to use the ಒಳಹರಿವು delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         ಕುರು not print ಸಾಲುಗಳು not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a run of blank characters as the field delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one of -b, -c, -f or -F.  Each LIST is made up of one\n\
range, or many ranges separated by commas.  Selected ಒಳಹರಿವು is written\n\
in the same order that it is ಪಠ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one of:\n\
\n\
  N     N'th byte, character or field, counted from 1\n\
  N-    from N'th byte, character or field, to end of ಸಾಲು\n\
  N-M   from N'th to M'th (included) byte, character or field\n\
  -M    from first to M'th (included) byte, character or field\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
next_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  ಯದಿ ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
print_kth (uintmax_t k)
{
  ಪ್ರತ್ಯಾಗಮ current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_range_start_index (uintmax_t k)
{
  ಪ್ರತ್ಯಾಗಮ k == current_rp->lo;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
field_delim_is_ಸಾಲು_delim (ಶೂನ್ಯ)
{
  ಪ್ರತ್ಯಾಗಮ delim_mcel.len == 1 && delim_bytes[0] == ಸಾಲು_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

ಸ್ಥಿರ bool
mcel_isblank (mcel_t g)
{
  ಪ್ರತ್ಯಾಗಮ (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
bytesearch_field_delim_ok (ಶೂನ್ಯ)
{
  unsigned ವರ್ಣ delim_0 = delim_bytes[0];

  ಪ್ರತ್ಯಾಗಮ (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


ಗಣನಾ field_terminator
{
  FIELD_DATA,
  FIELD_DELIMITER,
  FIELD_ಸಾಲು_DELIMITER,
  FIELD_EOF
};

ಗಣನಾ bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_ಸಾಲು_ONLY
};

ಸಂರಚನಾ bytesearch_context
{
  ಗಣನಾ bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  ವರ್ಣ *ಸಾಲು_end;
  bool ಸಾಲು_end_known;
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
bytesearch_context_reset (ಸಂರಚನಾ bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->ಸಾಲು_end = ರಿಕ್ತ;
  ctx->ಸಾಲು_end_known = false;
}

ಸಂರಚನಾ mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_ಸಾಲು_start;
  bool have_saved;
  mcel_t saved_g;
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  ಯದಿ (*have_saved)
    {
      *have_saved = false;
      ಪ್ರತ್ಯಾಗಮ *saved_g;
    }
  ಪ್ರತ್ಯಾಗಮ mbbuf_get_char (mbbuf);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ field_terminator
skip_whitespace_run (mbbuf_t *mbuf, ಸಂರಚನಾ mbfield_parser *parser,
                     bool *have_pending_ಸಾಲು,
                     bool have_initial_whitespace)
{
  mcel_t g;

  ಕುರು
    {
      g = mbbuf_get_char (mbuf);
      ಯದಿ (g.ch != MBBUF_EOF)
        *have_pending_ಸಾಲು = true;
    }
  ಯಾವತ್ (g.ch != MBBUF_EOF && g.ch != ಸಾಲು_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_ಸಾಲು_start;

  ಯದಿ (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == ಸಾಲು_delim))
    {
      ಪ್ರತ್ಯಾಗಮ g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_ಸಾಲು_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  ಪ್ರತ್ಯಾಗಮ trim_start && !have_initial_whitespace ? FIELD_DATA : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

ಸ್ಥಿರ ಶೂನ್ಯ
write_bytes (ವರ್ಣ ಧ್ರುವ *buf, size_t n_bytes)
{
  ಯದಿ (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      ಪ್ರತಿ (size_t i = 0; i < n_bytes; i++)
        ಯದಿ (putchar (buf[i]) < 0)
          write_error ();
      ಪ್ರತ್ಯಾಗಮ;
    }

  ಯದಿ (fwrite (buf, ಪರಿಮಾಣ (ವರ್ಣ), n_bytes, stdout) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
copy_bytes (ವರ್ಣ *dst, ವರ್ಣ ಧ್ರುವ *src, size_t n_bytes)
{
  ಯದಿ (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      ಪ್ರತಿ (size_t i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      ಪ್ರತ್ಯಾಗಮ;
    }

  memcpy (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ*
ಹುಡುಕು_bytes (ವರ್ಣ ಧ್ರುವ *buf, ಪೂರ್ಣಾಙ್ಕ c, size_t n_bytes)
{
  ಯದಿ (n_bytes != 0 && to_uchar (buf[0]) == c)
    ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *) buf;
  ಯದಿ (1 < n_bytes && to_uchar (buf[1]) == c)
    ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *) buf + 1;

  ಪ್ರತ್ಯಾಗಮ 2 < n_bytes ? (ವರ್ಣ *) memchr (buf + 2, c, n_bytes - 2) : ರಿಕ್ತ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
write_ಸಾಲು_delim (ಶೂನ್ಯ)
{
  ಯದಿ (putchar (ಸಾಲು_delim) < 0)
    write_error ();
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
reset_item_ಸಾಲು (uintmax_t *item_idx, bool *print_delimiter)
{
  write_ಸಾಲು_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
write_pending_ಸಾಲು_delim (uintmax_t item_idx)
{
  ಯದಿ (item_idx > 0)
    write_ಸಾಲು_delim ();
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
write_selected_item (bool *print_delimiter, bool range_start,
                     ವರ್ಣ ಧ್ರುವ *buf, size_t n_bytes)
{
  ಯದಿ (output_delimiter_string != output_delimiter_default)
    {
      ಯದಿ (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
append_field_1_chunk (ವರ್ಣ ಧ್ರುವ *buf, idx_t len, idx_t *n_bytes)
{
  ಯದಿ (field_1_bufsize - *n_bytes < len)
    {
      field_1_buffer = xpalloc (field_1_buffer, &field_1_bufsize,
                                len, -1, ಪರಿಮಾಣ *field_1_buffer);
    }

  copy_bytes (field_1_buffer + *n_bytes, buf, len);
  *n_bytes += len;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, ಸಂರಚನಾ mbfield_parser *parser,
                     bool *have_pending_ಸಾಲು, bool write_field,
                     idx_t *n_bytes)
{
  ಯದಿ (parser->trim_outer_whitespace && parser->at_ಸಾಲು_start)
    {
      ಗಣನಾ field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_ಸಾಲು, false);
      ಯದಿ (terminator != FIELD_DATA)
        ಪ್ರತ್ಯಾಗಮ terminator;
    }

  parser->at_ಸಾಲು_start = false;

  ಯಾವತ್ (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      ಯದಿ (g.ch == MBBUF_EOF)
        ಪ್ರತ್ಯಾಗಮ FIELD_EOF;

      *have_pending_ಸಾಲು = true;

      ಯದಿ (g.ch == ಸಾಲು_delim)
        ಪ್ರತ್ಯಾಗಮ FIELD_ಸಾಲು_DELIMITER;

      ಯದಿ (mcel_isblank (g))
        ಪ್ರತ್ಯಾಗಮ skip_whitespace_run (mbbuf, parser, have_pending_ಸಾಲು, true);

      ಯದಿ (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      ಅನ್ಯಥಾ ಯದಿ (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_ಸಾಲು,
                     bool write_field, idx_t *n_bytes)
{
  ಯಾವತ್ (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      ಯದಿ (g.ch == MBBUF_EOF)
        ಪ್ರತ್ಯಾಗಮ FIELD_EOF;

      *have_pending_ಸಾಲು = true;

      ಯದಿ (g.ch == ಸಾಲು_delim)
        ಪ್ರತ್ಯಾಗಮ FIELD_ಸಾಲು_DELIMITER;

      ಯದಿ (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        ಪ್ರತ್ಯಾಗಮ FIELD_DELIMITER;

      ಯದಿ (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      ಅನ್ಯಥಾ ಯದಿ (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ field_terminator
scan_mb_field (mbbuf_t *mbbuf, ಸಂರಚನಾ mbfield_parser *parser,
               bool *have_pending_ಸಾಲು, bool write_field, idx_t *n_bytes)
{
  ಪ್ರತ್ಯಾಗಮ (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_ಸಾಲು,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_ಸಾಲು, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

ಸ್ಥಿರ ಗಣನಾ field_terminator
scan_mb_ಸಾಲು_end (mbbuf_t *mbbuf, ಸಂರಚನಾ mbfield_parser *parser,
                  bool *have_pending_ಸಾಲು)
{
  ಯದಿ (parser->have_saved)
    {
      parser->have_saved = false;
      ಯದಿ (parser->saved_g.ch == MBBUF_EOF)
        ಪ್ರತ್ಯಾಗಮ FIELD_EOF;

      *have_pending_ಸಾಲು = true;
      ಯದಿ (parser->saved_g.ch == ಸಾಲು_delim)
        ಪ್ರತ್ಯಾಗಮ FIELD_ಸಾಲು_DELIMITER;
    }

  ಯಾವತ್ (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      ಯದಿ (available == 0)
        ಪ್ರತ್ಯಾಗಮ FIELD_EOF;

      ವರ್ಣ *buffer = mbbuf->buffer + mbbuf->offset;
      ವರ್ಣ *ಸಾಲು_end = ಹುಡುಕು_bytes (buffer, ಸಾಲು_delim, available);
      idx_t n = ಸಾಲು_end ? ಸಾಲು_end - buffer + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_ಸಾಲು = true;

      ಯದಿ (ಸಾಲು_end)
        ಪ್ರತ್ಯಾಗಮ FIELD_ಸಾಲು_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
ಸ್ಥಿರ ವರ್ಣ *
find_field_delim (ವರ್ಣ *buf, size_t len)
{
  ಯದಿ (len < delim_mcel.len)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

#ಯದಿ ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  ಪ್ರತ್ಯಾಗಮ memmem (buf, len, delim_bytes, delim_mcel.len);
#ಅನ್ಯಥಾ
  ಯದಿ (delim_mcel.len == 1)
    ಪ್ರತ್ಯಾಗಮ memchr (buf, delim_bytes[0], len);

  ವರ್ಣ ಧ್ರುವ *p = buf;
  ವರ್ಣ ಧ್ರುವ *end = buf + len;

  ವರ್ಣ saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  ಯಾವತ್ (p < end)
    {
      ವರ್ಣ ಧ್ರುವ *nul = memchr (p, '\0', end - p);
      ಯದಿ (!nul)
        {
          ವರ್ಣ ಧ್ರುವ *match = strstr (p, delim_bytes);
          buf[len] = saved;
          ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *) match;
        }

      ಯದಿ (p < nul)
        {
          ವರ್ಣ ಧ್ರುವ *match = strstr (p, delim_bytes);
          ಯದಿ (match)
            {
              buf[len] = saved;
              ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
#endif
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
ಸ್ಥಿರ ವರ್ಣ *
find_c_blank (ವರ್ಣ *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  ಪ್ರತಿ (idx_t i = 0; i < n; i++)
    ಯದಿ (c_isblank (buf[i]))
      ಪ್ರತ್ಯಾಗಮ buf + i;

  ಪ್ರತ್ಯಾಗಮ memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_PURE
ಸ್ಥಿರ idx_t
field_delim_overlap (ವರ್ಣ ಧ್ರುವ *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  ಯಾವತ್ (0 < overlap)
    {
      ಯದಿ (memeq (buf + len - overlap, delim_bytes, overlap))
        ಪ್ರತ್ಯಾಗಮ overlap;
      overlap--;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ field_terminator
find_field_terminator (ವರ್ಣ *buf, idx_t len,
                       ಸಂರಚನಾ bytesearch_context *ctx, ವರ್ಣ **terminator)
{
  ಯದಿ (ctx->mode == BYTESEARCH_ಸಾಲು_ONLY)
    {
      ಯದಿ (!ctx->ಸಾಲು_end_known)
        {
          ctx->ಸಾಲು_end = memchr (buf, ಸಾಲು_delim, len);
          ctx->ಸಾಲು_end_known = true;
        }

      *terminator = ctx->ಸಾಲು_end;
      ಪ್ರತ್ಯಾಗಮ *terminator ? FIELD_ಸಾಲು_DELIMITER : FIELD_DATA;
    }

  ಯದಿ (field_delim_is_ಸಾಲು_delim ())
    {
      *terminator = memchr (buf, ಸಾಲು_delim, len);
      ಪ್ರತ್ಯಾಗಮ *terminator ? FIELD_DELIMITER : FIELD_DATA;
    }

  ಯದಿ (!ctx->ಸಾಲು_end_known)
    {
      ctx->ಸಾಲು_end = memchr (buf, ಸಾಲು_delim, len);
      ctx->ಸಾಲು_end_known = true;
    }

  idx_t field_len = ctx->ಸಾಲು_end ? ctx->ಸಾಲು_end - buf : len;

  ವರ್ಣ *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  ಯದಿ (field_end)
    {
      *terminator = field_end;
      ಪ್ರತ್ಯಾಗಮ FIELD_DELIMITER;
    }

  *terminator = ctx->ಸಾಲು_end;
  ಪ್ರತ್ಯಾಗಮ ctx->ಸಾಲು_end ? FIELD_ಸಾಲು_DELIMITER : FIELD_DATA;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
maybe_write_ಸಾಲು_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  ಯದಿ (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_ಸಾಲು_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
begin_field_output (uintmax_t field_idx, bool buffer_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  ಯದಿ (write_field && ! (field_idx == 1 && buffer_first_field))
    {
      ಯದಿ (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *found_any_selected_field = true;
    }

  ಪ್ರತ್ಯಾಗಮ write_field;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
handle_field_1 (uintmax_t field_idx, bool buffer_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  ಯದಿ (field_idx == 1 && buffer_first_field)
    {
      ಯದಿ ((field_1_selected = print_kth (1)))
        write_bytes (field_1_buffer, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  ಪ್ರತ್ಯಾಗಮ field_1_selected;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
handle_field_delimiter (uintmax_t *field_idx, bool buffer_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  ಯದಿ (handle_field_1 (*field_idx, buffer_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  next_item (field_idx);
  *write_field = begin_field_output (*field_idx, buffer_first_field,
                                     found_any_selected_field);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
field_selection_exhausted (uintmax_t field_idx)
{
  ಪ್ರತ್ಯಾಗಮ !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
finish_current_ಸಾಲು (uintmax_t field_idx, bool buffer_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool ಸಾಲು_terminated)
{
  ಯದಿ (field_idx == 1 && buffer_first_field)
    {
      ಯದಿ (!suppress_non_delimited
          && (ಸಾಲು_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_buffer, *field_1_n_bytes);
          write_ಸಾಲು_delim ();
        }
      *field_1_n_bytes = 0;
    }
  ಅನ್ಯಥಾ
    maybe_write_ಸಾಲು_delim (found_any_selected_field, field_idx);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
sync_byte_selection (uintmax_t byte_idx)
{
  ಯಾವತ್ (current_rp->hi <= byte_idx)
    current_rp++;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
reset_field_ಸಾಲು (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_ಸಾಲು, ಸಂರಚನಾ mbfield_parser *parser)
{
  *field_idx = 1;
  current_rp = frp;
  *found_any_selected_field = false;
  *have_pending_ಸಾಲು = false;
  parser->have_saved = false;
  parser->at_ಸಾಲು_start = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_bytes (FILE *stream)
{
  ಸ್ಥಿರ ವರ್ಣ bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  ಪೂರ್ಣಾಙ್ಕ fd = fileno (stream);

  current_rp = frp;

  ಯಾವತ್ (true)
    {
      idx_t available = ಪಠ (fd, bytes_in, ಪರಿಮಾಣ bytes_in);
      ಯದಿ (available <= 0)
        {
          ಯದಿ (available < 0)
            fseterr (stream);
          write_pending_ಸಾಲು_delim (byte_idx);
          ಭಙ್ಗ;
        }

      idx_t processed = 0;

      ಯಾವತ್ (processed < available)
        {
          ವರ್ಣ *ಸಾಲು = bytes_in + processed;
          idx_t to_process = available - processed;
          ವರ್ಣ *ಸಾಲು_end = ಹುಡುಕು_bytes (ಸಾಲು, ಸಾಲು_delim, to_process);
          ವರ್ಣ *end = ಸಾಲು + (ಸಾಲು_end ? ಸಾಲು_end - ಸಾಲು : to_process);
          ವರ್ಣ *p = ಸಾಲು;

          ಯಾವತ್ (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              ಯದಿ (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              ಅನ್ಯಥಾ
                {
                  idx_t n = MIN (end - p, current_rp->hi - byte_idx);
                  write_selected_item (&print_delimiter,
                                       is_range_start_index (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += end - ಸಾಲು;
          ಯದಿ (ಸಾಲು_end)
            {
              processed++;
              reset_item_ಸಾಲು (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_characters_mode (FILE *stream, bool byte_mode)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  ಸ್ಥಿರ ವರ್ಣ bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, ಪರಿಮಾಣ bytes_in, stream);

  ಯಾವತ್ (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      ಯದಿ (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          ವರ್ಣ *p = mbbuf.buffer + mbbuf.offset;
          ವರ್ಣ *end = ಹುಡುಕು_bytes (p, ಸಾಲು_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          ಯದಿ (!end && available)
            ಅನುವರ್ತ;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      ಯದಿ (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          ವರ್ಣ ಧ್ರುವ *p = mbbuf.buffer + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          ವರ್ಣ ಧ್ರುವ *end = ಹುಡುಕು_bytes (p, ಸಾಲು_delim, n);
          ಯದಿ (end)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, end - p + 1);
              reset_item_ಸಾಲು (&idx, &print_delimiter);
              ಅನುವರ್ತ;
            }

          idx += u8_ಎಣಿಕೆ (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      ಯದಿ (g.ch == ಸಾಲು_delim)
        reset_item_ಸಾಲು (&idx, &print_delimiter);
      ಅನ್ಯಥಾ ಯದಿ (g.ch == MBBUF_EOF)
        {
          write_pending_ಸಾಲು_delim (idx);
          ಭಙ್ಗ;
        }
      ಅನ್ಯಥಾ ಯದಿ (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          ಪ್ರತಿ (idx_t i = 0; i < g.len; i++)
            {
              next_item (&idx);
              ಯದಿ (print_kth (idx))
                {
                  ಯದಿ (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_index (idx);
                    }
                }
              ಅನ್ಯಥಾ ಯದಿ (seen_selected)
                suffix_selected = false;
            }

          ಯದಿ (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      ಅನ್ಯಥಾ
        {
          next_item (&idx);
          ಯದಿ (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_index (idx),
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
cut_bytes_no_split (FILE *stream)
{
  cut_characters_mode (stream, true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
cut_characters (FILE *stream)
{
  cut_characters_mode (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_fields_mb_any (FILE *stream, bool whitespace_mode)
{
  ಸ್ಥಿರ ವರ್ಣ bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  ಸಂರಚನಾ mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_mode,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_ಸಾಲು_start = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool buffer_first_field;
  bool have_pending_ಸಾಲು = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, ಪರಿಮಾಣ bytes_in, stream);

  buffer_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  ಯಾವತ್ (true)
    {
      ಗಣನಾ field_terminator terminator;
      ಯದಿ (!buffer_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_ಸಾಲು,
                                    write_field, ರಿಕ್ತ);
      ಅನ್ಯಥಾ
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_ಸಾಲು,
                                    false, &field_1_n_bytes);

      ಯದಿ (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          ಯದಿ (! field_selection_exhausted (field_idx))
            ಅನುವರ್ತ;

          terminator = scan_mb_ಸಾಲು_end (&mbbuf, &parser,
                                         &have_pending_ಸಾಲು);
        }

      ಯದಿ (terminator == FIELD_EOF && !have_pending_ಸಾಲು)
        ಭಙ್ಗ;

      finish_current_ಸಾಲು (field_idx, buffer_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_ಸಾಲು_DELIMITER);
      ಯದಿ (terminator == FIELD_EOF)
        ಭಙ್ಗ;

      reset_field_ಸಾಲು (&field_idx, &found_any_selected_field,
                        &have_pending_ಸಾಲು, &parser);
      write_field = begin_field_output (field_idx, buffer_first_field,
                                        &found_any_selected_field);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_fields_mb (FILE *stream)
{
  cut_fields_mb_any (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_fields_bytesearch (FILE *stream)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  ಸ್ಥಿರ ವರ್ಣ bytes_in[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool have_pending_ಸಾಲು = false;
  bool skip_blank_run = false;
  bool pending_ಸಾಲು_delim_field = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  bool buffer_first_field = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, bytes_in, ಪರಿಮಾಣ bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  ಸಂರಚನಾ bytesearch_context ಹುಡುಕು = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&ಹುಡುಕು);

  ಯಾವತ್ (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      ಹುಡುಕು.at_eof = mbbuf.eof;
      ಹುಡುಕು.ಸಾಲು_end_known = false;

      ಯದಿ (pending_ಸಾಲು_delim_field)
        {
          pending_ಸಾಲು_delim_field = false;

          ಯದಿ (n_avail == 0)
            {
              ಯದಿ (handle_field_1 (field_idx, buffer_first_field,
                                  &field_1_n_bytes))
                next_item (&field_idx);
              maybe_write_ಸಾಲು_delim (found_any_selected_field, field_idx);
              ಪ್ರತ್ಯಾಗಮ;
            }

          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          ಯದಿ (whitespace_delimited)
            skip_blank_run = true;
        }

      ಯದಿ (n_avail == 0)
        ಭಙ್ಗ;

      ವರ್ಣ *chunk = mbbuf.buffer + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      ಯದಿ (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_ಸಾಲು_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          ವರ್ಣ *last_ಸಾಲು_delim = ಹುಡುಕು.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, ಸಾಲು_delim, n_avail);
          ಯದಿ (last_ಸಾಲು_delim)
            {
              idx_t n = last_ಸಾಲು_delim - chunk + 1;
              ಯದಿ (! suppress_non_delimited)
                {
                  write_bytes (field_1_buffer, field_1_n_bytes);
                  write_bytes (chunk, n);
                  ಯದಿ (ಹುಡುಕು.at_eof && chunk[n - 1] != ಸಾಲು_delim)
                    write_ಸಾಲು_delim ();
                }
              field_1_n_bytes = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_ಸಾಲು = false;
              ಯದಿ (ಹುಡುಕು.at_eof)
                ಪ್ರತ್ಯಾಗಮ;
              ಅನುವರ್ತ;
            }
        }

      ಯಾವತ್ (processed < n_avail)
        {
          ವರ್ಣ *terminator = ರಿಕ್ತ;

          ಯದಿ (skip_blank_run)
            {
              ಯಾವತ್ (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              ಯದಿ (processed == n_avail)
                ಭಙ್ಗ;
              skip_blank_run = false;
            }

          ಯದಿ (field_selection_exhausted (field_idx))
            {
              ಯದಿ (field_delim_is_ಸಾಲು_delim ())
                {
                  maybe_write_ಸಾಲು_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  ಪ್ರತ್ಯಾಗಮ;
                }
              ಹುಡುಕು.mode = BYTESEARCH_ಸಾಲು_ONLY;
              ಗಣನಾ field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &ಹುಡುಕು, &terminator);
              ಯದಿ (terminator_kind == FIELD_ಸಾಲು_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_ಸಾಲು_delim (found_any_selected_field,
                                          field_idx);
                  ಗಚ್ಛ reset_ಸಾಲು;
                }
              processed = n_avail;
              ಭಙ್ಗ;
            }

          ಗಣನಾ field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &ಹುಡುಕು, &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          ಯದಿ (terminator_kind == FIELD_DATA
              && !ಹುಡುಕು.at_eof
              && !whitespace_delimited
              && !field_delim_is_ಸಾಲು_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          ಯದಿ (field_len || terminator)
            have_pending_ಸಾಲು = true;

          /* Output field content.  */
          ಯದಿ (field_idx == 1 && buffer_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          ಅನ್ಯಥಾ ಯದಿ (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          ಯದಿ (terminator_kind == FIELD_DATA)
            ಭಙ್ಗ;

          ಯದಿ (terminator_kind == FIELD_DELIMITER)
            {
              ಯದಿ (field_delim_is_ಸಾಲು_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_ಸಾಲು_delim_field = true;
                  ಭಙ್ಗ;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, buffer_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              ಯದಿ (whitespace_delimited)
                skip_blank_run = true;
            }
          ಅನ್ಯಥಾ
            {
              affirm (terminator_kind == FIELD_ಸಾಲು_DELIMITER);
              processed++;
              finish_current_ಸಾಲು (field_idx, buffer_first_field,
                                   &field_1_n_bytes,
                                   found_any_selected_field, true);

            reset_ಸಾಲು:
              field_idx = 1;
              current_rp = frp;
              found_any_selected_field = false;
              have_pending_ಸಾಲು = false;
              bytesearch_context_reset (&ಹುಡುಕು);
              write_field = begin_field_output (field_idx, buffer_first_field,
                                                &found_any_selected_field);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  ಯದಿ (!have_pending_ಸಾಲು)
    ಪ್ರತ್ಯಾಗಮ;

  finish_current_ಸಾಲು (field_idx, buffer_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_fields_ws (FILE *stream)
{
  ಯದಿ (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  ಅನ್ಯಥಾ
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

ಸ್ಥಿರ bool
cut_file (ವರ್ಣ ಧ್ರುವ *file, ಶೂನ್ಯ (*cut_stream) (FILE *))
{
  FILE *stream;

  ಯದಿ (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  ಅನ್ಯಥಾ
    {
      stream = fopen (file, "r");
      ಯದಿ (stream == ರಿಕ್ತ)
        {
          error (0, errno, "%s", quotef (file));
          ಪ್ರತ್ಯಾಗಮ false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  ಪೂರ್ಣಾಙ್ಕ err = errno;
  ಯದಿ (!ferror (stream))
    err = 0;
  ಯದಿ (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF.  */
  ಅನ್ಯಥಾ ಯದಿ (fclose (stream) == EOF)
    err = errno;
  ಯದಿ (err)
    {
      error (0, err, "%s", quotef (file));
      ಪ್ರತ್ಯಾಗಮ false;
    }
  ಪ್ರತ್ಯಾಗಮ true;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  ವರ್ಣ *spec_list_string = ರಿಕ್ತ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'b':
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        ಪಕ್ಷ 'c':
          ಯದಿ (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        ಪಕ್ಷ 'f':
        ಪಕ್ಷ 'F':
          ಯದಿ (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
          ಯದಿ (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          ಯದಿ (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            ಯದಿ (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_length = (optarg[0] == '\0'
                                     ? 1 : strlen (optarg));
          output_delimiter_string = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'n':
          no_split = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 's':
          suppress_non_delimited = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'z':
          ಸಾಲು_delim = '\0';
          ಭಙ್ಗ;

        ಪಕ್ಷ COMPLEMENT_OPTION:
          complement = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  ಯದಿ (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
    {
      ಯದಿ (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an ಒಳಹರಿವು delimiter makes sense\n\
\tonly when operating on fields"));

      ಯದಿ (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited ಸಾಲುಗಳು makes sense\n\
\tonly when operating on fields"));
    }

  ಯದಿ (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  ಯದಿ (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  ಯದಿ (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  ಯದಿ (output_delimiter_string == ರಿಕ್ತ)
    {
      output_delimiter_string = output_delimiter_default;
      ಯದಿ (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_length = 1;
        }
      ಅನ್ಯಥಾ
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_length = delim_mcel.len;
        }
    }

  ಶೂನ್ಯ (*cut_stream) (FILE *) = ರಿಕ್ತ;
  ವಿಕಲ್ಪ (cut_mode)
    {
    ಪಕ್ಷ CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      ಭಙ್ಗ;

    ಪಕ್ಷ CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      ಭಙ್ಗ;

    ಪಕ್ಷ CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      ಭಙ್ಗ;

    ಪಕ್ಷ CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  ಯದಿ (optind == argc)
    ok = cut_file ("-", cut_stream);
  ಅನ್ಯಥಾ
    ಪ್ರತಿ (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  ಯದಿ (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
