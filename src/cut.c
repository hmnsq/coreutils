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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "ioblksize.h"
#অন্তর্ভাব "mbbuf.h"
#অন্তর্ভাব "memchr2.h"

#অন্তর্ভাব "set-fields.h"
#অন্তর্ভাব "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cut"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#define FATAL_ERROR(Message)						\
  কুরু									\
    {									\
      error (0, 0, (Message));						\
      usage (EXIT_FAILURE);						\
    }									\
  যাবত্ (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#define SMALL_BYTE_THRESHOLD 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
স্থির সংরচনা field_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
স্থির বর্ণ *field_1_buffer;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
স্থির idx_t field_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
স্থির bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
স্থির bool complement;

/* The delimiter character for multibyte field mode.  */
স্থির mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
স্থির বর্ণ delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
স্থির unsigned বর্ণ সারি_delim = '\n';

/* The length of output_delimiter_string.  */
স্থির size_t output_delimiter_length;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
স্থির বর্ণ *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
স্থির বর্ণ output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
স্থির bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
স্থির bool no_split;

/* If true, interpret each run of whitespace as one field delimiter.  */
স্থির bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
স্থির bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
স্থির bool space_output_delimiter_default;

গণনা whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

স্থির বর্ণ ধ্রুব *ধ্রুব whitespace_option_args[] =
{
  "trimmed", রিক্ত
};

স্থির গণনা whitespace_option ধ্রুব whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_args, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
স্থির গণনা
{
  CUT_MODE_NONE,
  CUT_MODE_BYTES,
  CUT_MODE_CHARACTERS,
  CUT_MODE_FIELDS
} cut_mode = CUT_MODE_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

স্থির সংরচনা option ধ্রুব longopts[] =
{
  {"bytes", required_argument, রিক্ত, 'b'},
  {"characters", required_argument, রিক্ত, 'c'},
  {"fields", required_argument, রিক্ত, 'f'},
  {"delimiter", required_argument, রিক্ত, 'd'},
  {"no-partial", no_argument, রিক্ত, 'n'},
  {"whitespace-delimited", optional_argument, রিক্ত, 'w'},
  {"only-delimited", no_argument, রিক্ত, 's'},
  {"output-delimiter", required_argument, রিক্ত, 'O'},
  {"complement", no_argument, রিক্ত, COMPLEMENT_OPTION},
  {"zero-terminated", no_argument, রিক্ত, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Print selected parts of সারিগুলি from each FILE to standard output.\n\
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
         use DELIM instead of TAB প্রতি field delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any সারি that contains\n\
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
         the স্বভাব is to use the প্রবেশ delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         কুরু not print সারিগুলি not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a run of blank characters as the field delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         সারি delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one of -b, -c, -f or -F.  Each LIST is made up of one\n\
range, or many ranges separated by commas.  Selected প্রবেশ is written\n\
in the same order that it is পঠ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one of:\n\
\n\
  N     N'th byte, character or field, counted from 1\n\
  N-    from N'th byte, character or field, to end of সারি\n\
  N-M   from N'th to M'th (included) byte, character or field\n\
  -M    from first to M'th (included) byte, character or field\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

স্থির অন্তঃস্থ শূন্য
next_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  যদি ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

স্থির অন্তঃস্থ bool
print_kth (uintmax_t k)
{
  প্রত্যাগম current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

স্থির অন্তঃস্থ bool
is_range_start_index (uintmax_t k)
{
  প্রত্যাগম k == current_rp->lo;
}

স্থির অন্তঃস্থ bool
field_delim_is_সারি_delim (শূন্য)
{
  প্রত্যাগম delim_mcel.len == 1 && delim_bytes[0] == সারি_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

স্থির bool
mcel_isblank (mcel_t g)
{
  প্রত্যাগম (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

স্থির অন্তঃস্থ bool
bytesearch_field_delim_ok (শূন্য)
{
  unsigned বর্ণ delim_0 = delim_bytes[0];

  প্রত্যাগম (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


গণনা field_terminator
{
  FIELD_DATA,
  FIELD_DELIMITER,
  FIELD_সারি_DELIMITER,
  FIELD_EOF
};

গণনা bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_সারি_ONLY
};

সংরচনা bytesearch_context
{
  গণনা bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  বর্ণ *সারি_end;
  bool সারি_end_known;
};

স্থির অন্তঃস্থ শূন্য
bytesearch_context_reset (সংরচনা bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->সারি_end = রিক্ত;
  ctx->সারি_end_known = false;
}

সংরচনা mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_সারি_start;
  bool have_saved;
  mcel_t saved_g;
};

স্থির অন্তঃস্থ mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  যদি (*have_saved)
    {
      *have_saved = false;
      প্রত্যাগম *saved_g;
    }
  প্রত্যাগম mbbuf_get_char (mbbuf);
}

স্থির অন্তঃস্থ গণনা field_terminator
skip_whitespace_run (mbbuf_t *mbuf, সংরচনা mbfield_parser *parser,
                     bool *have_pending_সারি,
                     bool have_initial_whitespace)
{
  mcel_t g;

  কুরু
    {
      g = mbbuf_get_char (mbuf);
      যদি (g.ch != MBBUF_EOF)
        *have_pending_সারি = true;
    }
  যাবত্ (g.ch != MBBUF_EOF && g.ch != সারি_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_সারি_start;

  যদি (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == সারি_delim))
    {
      প্রত্যাগম g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_সারি_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  প্রত্যাগম trim_start && !have_initial_whitespace ? FIELD_DATA : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

স্থির শূন্য
write_bytes (বর্ণ ধ্রুব *buf, size_t n_bytes)
{
  যদি (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      প্রতি (size_t i = 0; i < n_bytes; i++)
        যদি (putchar (buf[i]) < 0)
          write_error ();
      প্রত্যাগম;
    }

  যদি (fwrite (buf, পরিমাণ (বর্ণ), n_bytes, stdout) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

স্থির অন্তঃস্থ শূন্য
copy_bytes (বর্ণ *dst, বর্ণ ধ্রুব *src, size_t n_bytes)
{
  যদি (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      প্রতি (size_t i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      প্রত্যাগম;
    }

  memcpy (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

স্থির অন্তঃস্থ শূন্য*
অনুসন্ধান_bytes (বর্ণ ধ্রুব *buf, পূর্ণাঙ্ক c, size_t n_bytes)
{
  যদি (n_bytes != 0 && to_uchar (buf[0]) == c)
    প্রত্যাগম (বর্ণ *) buf;
  যদি (1 < n_bytes && to_uchar (buf[1]) == c)
    প্রত্যাগম (বর্ণ *) buf + 1;

  প্রত্যাগম 2 < n_bytes ? (বর্ণ *) memchr (buf + 2, c, n_bytes - 2) : রিক্ত;
}

স্থির অন্তঃস্থ শূন্য
write_সারি_delim (শূন্য)
{
  যদি (putchar (সারি_delim) < 0)
    write_error ();
}

স্থির অন্তঃস্থ শূন্য
reset_item_সারি (uintmax_t *item_idx, bool *print_delimiter)
{
  write_সারি_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

স্থির অন্তঃস্থ শূন্য
write_pending_সারি_delim (uintmax_t item_idx)
{
  যদি (item_idx > 0)
    write_সারি_delim ();
}

স্থির অন্তঃস্থ শূন্য
write_selected_item (bool *print_delimiter, bool range_start,
                     বর্ণ ধ্রুব *buf, size_t n_bytes)
{
  যদি (output_delimiter_string != output_delimiter_default)
    {
      যদি (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

স্থির অন্তঃস্থ শূন্য
append_field_1_chunk (বর্ণ ধ্রুব *buf, idx_t len, idx_t *n_bytes)
{
  যদি (field_1_bufsize - *n_bytes < len)
    {
      field_1_buffer = xpalloc (field_1_buffer, &field_1_bufsize,
                                len, -1, পরিমাণ *field_1_buffer);
    }

  copy_bytes (field_1_buffer + *n_bytes, buf, len);
  *n_bytes += len;
}

স্থির অন্তঃস্থ শূন্য
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

স্থির অন্তঃস্থ গণনা field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, সংরচনা mbfield_parser *parser,
                     bool *have_pending_সারি, bool write_field,
                     idx_t *n_bytes)
{
  যদি (parser->trim_outer_whitespace && parser->at_সারি_start)
    {
      গণনা field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_সারি, false);
      যদি (terminator != FIELD_DATA)
        প্রত্যাগম terminator;
    }

  parser->at_সারি_start = false;

  যাবত্ (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      যদি (g.ch == MBBUF_EOF)
        প্রত্যাগম FIELD_EOF;

      *have_pending_সারি = true;

      যদি (g.ch == সারি_delim)
        প্রত্যাগম FIELD_সারি_DELIMITER;

      যদি (mcel_isblank (g))
        প্রত্যাগম skip_whitespace_run (mbbuf, parser, have_pending_সারি, true);

      যদি (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      অন্যথা যদি (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

স্থির অন্তঃস্থ গণনা field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_সারি,
                     bool write_field, idx_t *n_bytes)
{
  যাবত্ (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      যদি (g.ch == MBBUF_EOF)
        প্রত্যাগম FIELD_EOF;

      *have_pending_সারি = true;

      যদি (g.ch == সারি_delim)
        প্রত্যাগম FIELD_সারি_DELIMITER;

      যদি (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        প্রত্যাগম FIELD_DELIMITER;

      যদি (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      অন্যথা যদি (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

স্থির অন্তঃস্থ গণনা field_terminator
scan_mb_field (mbbuf_t *mbbuf, সংরচনা mbfield_parser *parser,
               bool *have_pending_সারি, bool write_field, idx_t *n_bytes)
{
  প্রত্যাগম (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_সারি,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_সারি, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

স্থির গণনা field_terminator
scan_mb_সারি_end (mbbuf_t *mbbuf, সংরচনা mbfield_parser *parser,
                  bool *have_pending_সারি)
{
  যদি (parser->have_saved)
    {
      parser->have_saved = false;
      যদি (parser->saved_g.ch == MBBUF_EOF)
        প্রত্যাগম FIELD_EOF;

      *have_pending_সারি = true;
      যদি (parser->saved_g.ch == সারি_delim)
        প্রত্যাগম FIELD_সারি_DELIMITER;
    }

  যাবত্ (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      যদি (available == 0)
        প্রত্যাগম FIELD_EOF;

      বর্ণ *buffer = mbbuf->buffer + mbbuf->offset;
      বর্ণ *সারি_end = অনুসন্ধান_bytes (buffer, সারি_delim, available);
      idx_t n = সারি_end ? সারি_end - buffer + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_সারি = true;

      যদি (সারি_end)
        প্রত্যাগম FIELD_সারি_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
স্থির বর্ণ *
find_field_delim (বর্ণ *buf, size_t len)
{
  যদি (len < delim_mcel.len)
    প্রত্যাগম রিক্ত;

#যদি ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  প্রত্যাগম memmem (buf, len, delim_bytes, delim_mcel.len);
#অন্যথা
  যদি (delim_mcel.len == 1)
    প্রত্যাগম memchr (buf, delim_bytes[0], len);

  বর্ণ ধ্রুব *p = buf;
  বর্ণ ধ্রুব *end = buf + len;

  বর্ণ saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  যাবত্ (p < end)
    {
      বর্ণ ধ্রুব *nul = memchr (p, '\0', end - p);
      যদি (!nul)
        {
          বর্ণ ধ্রুব *match = strstr (p, delim_bytes);
          buf[len] = saved;
          প্রত্যাগম (বর্ণ *) match;
        }

      যদি (p < nul)
        {
          বর্ণ ধ্রুব *match = strstr (p, delim_bytes);
          যদি (match)
            {
              buf[len] = saved;
              প্রত্যাগম (বর্ণ *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  প্রত্যাগম রিক্ত;
#endif
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
স্থির বর্ণ *
find_c_blank (বর্ণ *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  প্রতি (idx_t i = 0; i < n; i++)
    যদি (c_isblank (buf[i]))
      প্রত্যাগম buf + i;

  প্রত্যাগম memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_PURE
স্থির idx_t
field_delim_overlap (বর্ণ ধ্রুব *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  যাবত্ (0 < overlap)
    {
      যদি (memeq (buf + len - overlap, delim_bytes, overlap))
        প্রত্যাগম overlap;
      overlap--;
    }

  প্রত্যাগম 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

স্থির অন্তঃস্থ গণনা field_terminator
find_field_terminator (বর্ণ *buf, idx_t len,
                       সংরচনা bytesearch_context *ctx, বর্ণ **terminator)
{
  যদি (ctx->mode == BYTESEARCH_সারি_ONLY)
    {
      যদি (!ctx->সারি_end_known)
        {
          ctx->সারি_end = memchr (buf, সারি_delim, len);
          ctx->সারি_end_known = true;
        }

      *terminator = ctx->সারি_end;
      প্রত্যাগম *terminator ? FIELD_সারি_DELIMITER : FIELD_DATA;
    }

  যদি (field_delim_is_সারি_delim ())
    {
      *terminator = memchr (buf, সারি_delim, len);
      প্রত্যাগম *terminator ? FIELD_DELIMITER : FIELD_DATA;
    }

  যদি (!ctx->সারি_end_known)
    {
      ctx->সারি_end = memchr (buf, সারি_delim, len);
      ctx->সারি_end_known = true;
    }

  idx_t field_len = ctx->সারি_end ? ctx->সারি_end - buf : len;

  বর্ণ *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  যদি (field_end)
    {
      *terminator = field_end;
      প্রত্যাগম FIELD_DELIMITER;
    }

  *terminator = ctx->সারি_end;
  প্রত্যাগম ctx->সারি_end ? FIELD_সারি_DELIMITER : FIELD_DATA;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

স্থির অন্তঃস্থ শূন্য
maybe_write_সারি_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  যদি (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_সারি_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

স্থির অন্তঃস্থ bool
begin_field_output (uintmax_t field_idx, bool buffer_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  যদি (write_field && ! (field_idx == 1 && buffer_first_field))
    {
      যদি (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *found_any_selected_field = true;
    }

  প্রত্যাগম write_field;
}

স্থির অন্তঃস্থ bool
handle_field_1 (uintmax_t field_idx, bool buffer_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  যদি (field_idx == 1 && buffer_first_field)
    {
      যদি ((field_1_selected = print_kth (1)))
        write_bytes (field_1_buffer, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  প্রত্যাগম field_1_selected;
}

স্থির অন্তঃস্থ শূন্য
handle_field_delimiter (uintmax_t *field_idx, bool buffer_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  যদি (handle_field_1 (*field_idx, buffer_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  next_item (field_idx);
  *write_field = begin_field_output (*field_idx, buffer_first_field,
                                     found_any_selected_field);
}

স্থির অন্তঃস্থ bool
field_selection_exhausted (uintmax_t field_idx)
{
  প্রত্যাগম !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

স্থির অন্তঃস্থ শূন্য
finish_current_সারি (uintmax_t field_idx, bool buffer_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool সারি_terminated)
{
  যদি (field_idx == 1 && buffer_first_field)
    {
      যদি (!suppress_non_delimited
          && (সারি_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_buffer, *field_1_n_bytes);
          write_সারি_delim ();
        }
      *field_1_n_bytes = 0;
    }
  অন্যথা
    maybe_write_সারি_delim (found_any_selected_field, field_idx);
}

স্থির অন্তঃস্থ শূন্য
sync_byte_selection (uintmax_t byte_idx)
{
  যাবত্ (current_rp->hi <= byte_idx)
    current_rp++;
}

স্থির অন্তঃস্থ শূন্য
reset_field_সারি (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_সারি, সংরচনা mbfield_parser *parser)
{
  *field_idx = 1;
  current_rp = frp;
  *found_any_selected_field = false;
  *have_pending_সারি = false;
  parser->have_saved = false;
  parser->at_সারি_start = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

স্থির শূন্য
cut_bytes (FILE *stream)
{
  স্থির বর্ণ bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  পূর্ণাঙ্ক fd = fileno (stream);

  current_rp = frp;

  যাবত্ (true)
    {
      idx_t available = পঠ (fd, bytes_in, পরিমাণ bytes_in);
      যদি (available <= 0)
        {
          যদি (available < 0)
            fseterr (stream);
          write_pending_সারি_delim (byte_idx);
          ভঙ্গ;
        }

      idx_t processed = 0;

      যাবত্ (processed < available)
        {
          বর্ণ *সারি = bytes_in + processed;
          idx_t to_process = available - processed;
          বর্ণ *সারি_end = অনুসন্ধান_bytes (সারি, সারি_delim, to_process);
          বর্ণ *end = সারি + (সারি_end ? সারি_end - সারি : to_process);
          বর্ণ *p = সারি;

          যাবত্ (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              যদি (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              অন্যথা
                {
                  idx_t n = MIN (end - p, current_rp->hi - byte_idx);
                  write_selected_item (&print_delimiter,
                                       is_range_start_index (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += end - সারি;
          যদি (সারি_end)
            {
              processed++;
              reset_item_সারি (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

স্থির শূন্য
cut_characters_mode (FILE *stream, bool byte_mode)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  স্থির বর্ণ bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, পরিমাণ bytes_in, stream);

  যাবত্ (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      যদি (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          বর্ণ *p = mbbuf.buffer + mbbuf.offset;
          বর্ণ *end = অনুসন্ধান_bytes (p, সারি_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          যদি (!end && available)
            অনুবর্ত;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      যদি (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          বর্ণ ধ্রুব *p = mbbuf.buffer + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          বর্ণ ধ্রুব *end = অনুসন্ধান_bytes (p, সারি_delim, n);
          যদি (end)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, end - p + 1);
              reset_item_সারি (&idx, &print_delimiter);
              অনুবর্ত;
            }

          idx += u8_গুনতি (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      যদি (g.ch == সারি_delim)
        reset_item_সারি (&idx, &print_delimiter);
      অন্যথা যদি (g.ch == MBBUF_EOF)
        {
          write_pending_সারি_delim (idx);
          ভঙ্গ;
        }
      অন্যথা যদি (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          প্রতি (idx_t i = 0; i < g.len; i++)
            {
              next_item (&idx);
              যদি (print_kth (idx))
                {
                  যদি (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_index (idx);
                    }
                }
              অন্যথা যদি (seen_selected)
                suffix_selected = false;
            }

          যদি (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      অন্যথা
        {
          next_item (&idx);
          যদি (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_index (idx),
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
    }
}

স্থির শূন্য
cut_bytes_no_split (FILE *stream)
{
  cut_characters_mode (stream, true);
}

স্থির শূন্য
cut_characters (FILE *stream)
{
  cut_characters_mode (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

স্থির শূন্য
cut_fields_mb_any (FILE *stream, bool whitespace_mode)
{
  স্থির বর্ণ bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  সংরচনা mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_mode,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_সারি_start = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool buffer_first_field;
  bool have_pending_সারি = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, পরিমাণ bytes_in, stream);

  buffer_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  যাবত্ (true)
    {
      গণনা field_terminator terminator;
      যদি (!buffer_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_সারি,
                                    write_field, রিক্ত);
      অন্যথা
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_সারি,
                                    false, &field_1_n_bytes);

      যদি (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          যদি (! field_selection_exhausted (field_idx))
            অনুবর্ত;

          terminator = scan_mb_সারি_end (&mbbuf, &parser,
                                         &have_pending_সারি);
        }

      যদি (terminator == FIELD_EOF && !have_pending_সারি)
        ভঙ্গ;

      finish_current_সারি (field_idx, buffer_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_সারি_DELIMITER);
      যদি (terminator == FIELD_EOF)
        ভঙ্গ;

      reset_field_সারি (&field_idx, &found_any_selected_field,
                        &have_pending_সারি, &parser);
      write_field = begin_field_output (field_idx, buffer_first_field,
                                        &found_any_selected_field);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

স্থির শূন্য
cut_fields_mb (FILE *stream)
{
  cut_fields_mb_any (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

স্থির শূন্য
cut_fields_bytesearch (FILE *stream)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  স্থির বর্ণ bytes_in[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool have_pending_সারি = false;
  bool skip_blank_run = false;
  bool pending_সারি_delim_field = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  bool buffer_first_field = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, bytes_in, পরিমাণ bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  সংরচনা bytesearch_context অনুসন্ধান = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&অনুসন্ধান);

  যাবত্ (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      অনুসন্ধান.at_eof = mbbuf.eof;
      অনুসন্ধান.সারি_end_known = false;

      যদি (pending_সারি_delim_field)
        {
          pending_সারি_delim_field = false;

          যদি (n_avail == 0)
            {
              যদি (handle_field_1 (field_idx, buffer_first_field,
                                  &field_1_n_bytes))
                next_item (&field_idx);
              maybe_write_সারি_delim (found_any_selected_field, field_idx);
              প্রত্যাগম;
            }

          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          যদি (whitespace_delimited)
            skip_blank_run = true;
        }

      যদি (n_avail == 0)
        ভঙ্গ;

      বর্ণ *chunk = mbbuf.buffer + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      যদি (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_সারি_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          বর্ণ *last_সারি_delim = অনুসন্ধান.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, সারি_delim, n_avail);
          যদি (last_সারি_delim)
            {
              idx_t n = last_সারি_delim - chunk + 1;
              যদি (! suppress_non_delimited)
                {
                  write_bytes (field_1_buffer, field_1_n_bytes);
                  write_bytes (chunk, n);
                  যদি (অনুসন্ধান.at_eof && chunk[n - 1] != সারি_delim)
                    write_সারি_delim ();
                }
              field_1_n_bytes = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_সারি = false;
              যদি (অনুসন্ধান.at_eof)
                প্রত্যাগম;
              অনুবর্ত;
            }
        }

      যাবত্ (processed < n_avail)
        {
          বর্ণ *terminator = রিক্ত;

          যদি (skip_blank_run)
            {
              যাবত্ (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              যদি (processed == n_avail)
                ভঙ্গ;
              skip_blank_run = false;
            }

          যদি (field_selection_exhausted (field_idx))
            {
              যদি (field_delim_is_সারি_delim ())
                {
                  maybe_write_সারি_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  প্রত্যাগম;
                }
              অনুসন্ধান.mode = BYTESEARCH_সারি_ONLY;
              গণনা field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &অনুসন্ধান, &terminator);
              যদি (terminator_kind == FIELD_সারি_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_সারি_delim (found_any_selected_field,
                                          field_idx);
                  গচ্ছ reset_সারি;
                }
              processed = n_avail;
              ভঙ্গ;
            }

          গণনা field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &অনুসন্ধান, &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          যদি (terminator_kind == FIELD_DATA
              && !অনুসন্ধান.at_eof
              && !whitespace_delimited
              && !field_delim_is_সারি_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          যদি (field_len || terminator)
            have_pending_সারি = true;

          /* Output field content.  */
          যদি (field_idx == 1 && buffer_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          অন্যথা যদি (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          যদি (terminator_kind == FIELD_DATA)
            ভঙ্গ;

          যদি (terminator_kind == FIELD_DELIMITER)
            {
              যদি (field_delim_is_সারি_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_সারি_delim_field = true;
                  ভঙ্গ;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, buffer_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              যদি (whitespace_delimited)
                skip_blank_run = true;
            }
          অন্যথা
            {
              affirm (terminator_kind == FIELD_সারি_DELIMITER);
              processed++;
              finish_current_সারি (field_idx, buffer_first_field,
                                   &field_1_n_bytes,
                                   found_any_selected_field, true);

            reset_সারি:
              field_idx = 1;
              current_rp = frp;
              found_any_selected_field = false;
              have_pending_সারি = false;
              bytesearch_context_reset (&অনুসন্ধান);
              write_field = begin_field_output (field_idx, buffer_first_field,
                                                &found_any_selected_field);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  যদি (!have_pending_সারি)
    প্রত্যাগম;

  finish_current_সারি (field_idx, buffer_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

স্থির শূন্য
cut_fields_ws (FILE *stream)
{
  যদি (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  অন্যথা
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

স্থির bool
cut_file (বর্ণ ধ্রুব *file, শূন্য (*cut_stream) (FILE *))
{
  FILE *stream;

  যদি (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  অন্যথা
    {
      stream = fopen (file, "r");
      যদি (stream == রিক্ত)
        {
          error (0, errno, "%s", quotef (file));
          প্রত্যাগম false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  পূর্ণাঙ্ক err = errno;
  যদি (!ferror (stream))
    err = 0;
  যদি (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF.  */
  অন্যথা যদি (fclose (stream) == EOF)
    err = errno;
  যদি (err)
    {
      error (0, err, "%s", quotef (file));
      প্রত্যাগম false;
    }
  প্রত্যাগম true;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  পূর্ণাঙ্ক optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  বর্ণ *spec_list_string = রিক্ত;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যাবত্ ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, রিক্ত))
         != -1)
    {
      বিকল্প (optc)
        {
        পক্ষ 'b':
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        পক্ষ 'c':
          যদি (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        পক্ষ 'f':
        পক্ষ 'F':
          যদি (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
          যদি (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          যদি (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          ভঙ্গ;

        পক্ষ 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            যদি (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            ভঙ্গ;
          }

        পক্ষ 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          ভঙ্গ;

        পক্ষ 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_length = (optarg[0] == '\0'
                                     ? 1 : strlen (optarg));
          output_delimiter_string = optarg;
          ভঙ্গ;

        পক্ষ 'n':
          no_split = true;
          ভঙ্গ;

        পক্ষ 's':
          suppress_non_delimited = true;
          ভঙ্গ;

        পক্ষ 'z':
          সারি_delim = '\0';
          ভঙ্গ;

        পক্ষ COMPLEMENT_OPTION:
          complement = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্বভাব:
          usage (EXIT_FAILURE);
        }
    }

  যদি (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  যদি (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
    {
      যদি (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an প্রবেশ delimiter makes sense\n\
\tonly when operating on fields"));

      যদি (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited সারিগুলি makes sense\n\
\tonly when operating on fields"));
    }

  যদি (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  যদি (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  যদি (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  যদি (output_delimiter_string == রিক্ত)
    {
      output_delimiter_string = output_delimiter_default;
      যদি (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_length = 1;
        }
      অন্যথা
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_length = delim_mcel.len;
        }
    }

  শূন্য (*cut_stream) (FILE *) = রিক্ত;
  বিকল্প (cut_mode)
    {
    পক্ষ CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      ভঙ্গ;

    পক্ষ CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      ভঙ্গ;

    পক্ষ CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      ভঙ্গ;

    পক্ষ CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  যদি (optind == argc)
    ok = cut_file ("-", cut_stream);
  অন্যথা
    প্রতি (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  যদি (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  প্রত্যাগম ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
