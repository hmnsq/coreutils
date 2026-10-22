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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "mbbuf.h"
#अन्तर्भाव "memchr2.h"

#अन्तर्भाव "set-fields.h"
#अन्तर्भाव "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cut"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#define FATAL_ERROR(Message)						\
  करोहि									\
    {									\
      error (0, 0, (Message));						\
      usage (EXIT_FAILURE);						\
    }									\
  याव (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#define SMALL_BYTE_THRESHOLD 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
स्थिर संरचना field_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
स्थिर वर्ण *field_1_buffer;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
स्थिर idx_t field_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
स्थिर bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
स्थिर bool complement;

/* The delimiter character for multibyte field mode.  */
स्थिर mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
स्थिर वर्ण delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
स्थिर unsigned वर्ण पङ्क्तिः_delim = '\n';

/* The length of output_delimiter_string.  */
स्थिर size_t output_delimiter_length;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
स्थिर वर्ण *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
स्थिर वर्ण output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
स्थिर bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
स्थिर bool no_split;

/* If true, interpret each run of whitespace as one field delimiter.  */
स्थिर bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
स्थिर bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
स्थिर bool space_output_delimiter_default;

गणना whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

स्थिर वर्ण ध्रुव *ध्रुव whitespace_option_args[] =
{
  "trimmed", रिक्त
};

स्थिर गणना whitespace_option ध्रुव whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_args, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
स्थिर गणना
{
  CUT_MODE_NONE,
  CUT_MODE_BYTES,
  CUT_MODE_CHARACTERS,
  CUT_MODE_FIELDS
} cut_mode = CUT_MODE_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"bytes", required_argument, रिक्त, 'b'},
  {"characters", required_argument, रिक्त, 'c'},
  {"fields", required_argument, रिक्त, 'f'},
  {"delimiter", required_argument, रिक्त, 'd'},
  {"no-partial", no_argument, रिक्त, 'n'},
  {"whitespace-delimited", optional_argument, रिक्त, 'w'},
  {"only-delimited", no_argument, रिक्त, 's'},
  {"output-delimiter", required_argument, रिक्त, 'O'},
  {"complement", no_argument, रिक्त, COMPLEMENT_OPTION},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
    {
      printf (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Print selected parts of पङ्क्तयः from each FILE to standard output.\n\
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
         use DELIM instead of TAB अत्थाय field delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any पङ्क्तिः that contains\n\
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
         the स्वभाव is to use the निवेशः delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         करोहि not print पङ्क्तयः not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a run of blank characters as the field delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one of -b, -c, -f or -F.  Each LIST is made up of one\n\
range, or many ranges separated by commas.  Selected निवेशः is written\n\
in the same order that it is पठ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one of:\n\
\n\
  N     N'th byte, character or field, counted from 1\n\
  N-    from N'th byte, character or field, to end of पङ्क्तिः\n\
  N-M   from N'th to M'th (included) byte, character or field\n\
  -M    from first to M'th (included) byte, character or field\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

स्थिर अन्तःस्थ शून्य
next_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  सचे ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

स्थिर अन्तःस्थ bool
print_kth (uintmax_t k)
{
  पटिनिवत्त current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

स्थिर अन्तःस्थ bool
is_range_start_index (uintmax_t k)
{
  पटिनिवत्त k == current_rp->lo;
}

स्थिर अन्तःस्थ bool
field_delim_is_पङ्क्तिः_delim (शून्य)
{
  पटिनिवत्त delim_mcel.len == 1 && delim_bytes[0] == पङ्क्तिः_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

स्थिर bool
mcel_isblank (mcel_t g)
{
  पटिनिवत्त (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

स्थिर अन्तःस्थ bool
bytesearch_field_delim_ok (शून्य)
{
  unsigned वर्ण delim_0 = delim_bytes[0];

  पटिनिवत्त (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


गणना field_terminator
{
  FIELD_DATA,
  FIELD_DELIMITER,
  FIELD_पङ्क्तिः_DELIMITER,
  FIELD_EOF
};

गणना bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_पङ्क्तिः_ONLY
};

संरचना bytesearch_context
{
  गणना bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  वर्ण *पङ्क्तिः_end;
  bool पङ्क्तिः_end_known;
};

स्थिर अन्तःस्थ शून्य
bytesearch_context_reset (संरचना bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->पङ्क्तिः_end = रिक्त;
  ctx->पङ्क्तिः_end_known = false;
}

संरचना mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_पङ्क्तिः_start;
  bool have_saved;
  mcel_t saved_g;
};

स्थिर अन्तःस्थ mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  सचे (*have_saved)
    {
      *have_saved = false;
      पटिनिवत्त *saved_g;
    }
  पटिनिवत्त mbbuf_get_char (mbbuf);
}

स्थिर अन्तःस्थ गणना field_terminator
skip_whitespace_run (mbbuf_t *mbuf, संरचना mbfield_parser *parser,
                     bool *have_pending_पङ्क्तिः,
                     bool have_initial_whitespace)
{
  mcel_t g;

  करोहि
    {
      g = mbbuf_get_char (mbuf);
      सचे (g.ch != MBBUF_EOF)
        *have_pending_पङ्क्तिः = true;
    }
  याव (g.ch != MBBUF_EOF && g.ch != पङ्क्तिः_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_पङ्क्तिः_start;

  सचे (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == पङ्क्तिः_delim))
    {
      पटिनिवत्त g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_पङ्क्तिः_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  पटिनिवत्त trim_start && !have_initial_whitespace ? FIELD_DATA : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

स्थिर शून्य
write_bytes (वर्ण ध्रुव *buf, size_t n_bytes)
{
  सचे (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      अत्थाय (size_t i = 0; i < n_bytes; i++)
        सचे (putchar (buf[i]) < 0)
          write_error ();
      पटिनिवत्त;
    }

  सचे (fwrite (buf, परिमाण (वर्ण), n_bytes, stdout) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

स्थिर अन्तःस्थ शून्य
copy_bytes (वर्ण *dst, वर्ण ध्रुव *src, size_t n_bytes)
{
  सचे (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      अत्थाय (size_t i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      पटिनिवत्त;
    }

  memcpy (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

स्थिर अन्तःस्थ शून्य*
अन्वेषणम्_bytes (वर्ण ध्रुव *buf, पूर्णाङ्क c, size_t n_bytes)
{
  सचे (n_bytes != 0 && to_uchar (buf[0]) == c)
    पटिनिवत्त (वर्ण *) buf;
  सचे (1 < n_bytes && to_uchar (buf[1]) == c)
    पटिनिवत्त (वर्ण *) buf + 1;

  पटिनिवत्त 2 < n_bytes ? (वर्ण *) memchr (buf + 2, c, n_bytes - 2) : रिक्त;
}

स्थिर अन्तःस्थ शून्य
write_पङ्क्तिः_delim (शून्य)
{
  सचे (putchar (पङ्क्तिः_delim) < 0)
    write_error ();
}

स्थिर अन्तःस्थ शून्य
reset_item_पङ्क्तिः (uintmax_t *item_idx, bool *print_delimiter)
{
  write_पङ्क्तिः_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

स्थिर अन्तःस्थ शून्य
write_pending_पङ्क्तिः_delim (uintmax_t item_idx)
{
  सचे (item_idx > 0)
    write_पङ्क्तिः_delim ();
}

स्थिर अन्तःस्थ शून्य
write_selected_item (bool *print_delimiter, bool range_start,
                     वर्ण ध्रुव *buf, size_t n_bytes)
{
  सचे (output_delimiter_string != output_delimiter_default)
    {
      सचे (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

स्थिर अन्तःस्थ शून्य
append_field_1_chunk (वर्ण ध्रुव *buf, idx_t len, idx_t *n_bytes)
{
  सचे (field_1_bufsize - *n_bytes < len)
    {
      field_1_buffer = xpalloc (field_1_buffer, &field_1_bufsize,
                                len, -1, परिमाण *field_1_buffer);
    }

  copy_bytes (field_1_buffer + *n_bytes, buf, len);
  *n_bytes += len;
}

स्थिर अन्तःस्थ शून्य
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

स्थिर अन्तःस्थ गणना field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, संरचना mbfield_parser *parser,
                     bool *have_pending_पङ्क्तिः, bool write_field,
                     idx_t *n_bytes)
{
  सचे (parser->trim_outer_whitespace && parser->at_पङ्क्तिः_start)
    {
      गणना field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_पङ्क्तिः, false);
      सचे (terminator != FIELD_DATA)
        पटिनिवत्त terminator;
    }

  parser->at_पङ्क्तिः_start = false;

  याव (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      सचे (g.ch == MBBUF_EOF)
        पटिनिवत्त FIELD_EOF;

      *have_pending_पङ्क्तिः = true;

      सचे (g.ch == पङ्क्तिः_delim)
        पटिनिवत्त FIELD_पङ्क्तिः_DELIMITER;

      सचे (mcel_isblank (g))
        पटिनिवत्त skip_whitespace_run (mbbuf, parser, have_pending_पङ्क्तिः, true);

      सचे (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      अञ्ञथा सचे (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

स्थिर अन्तःस्थ गणना field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_पङ्क्तिः,
                     bool write_field, idx_t *n_bytes)
{
  याव (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      सचे (g.ch == MBBUF_EOF)
        पटिनिवत्त FIELD_EOF;

      *have_pending_पङ्क्तिः = true;

      सचे (g.ch == पङ्क्तिः_delim)
        पटिनिवत्त FIELD_पङ्क्तिः_DELIMITER;

      सचे (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        पटिनिवत्त FIELD_DELIMITER;

      सचे (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      अञ्ञथा सचे (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

स्थिर अन्तःस्थ गणना field_terminator
scan_mb_field (mbbuf_t *mbbuf, संरचना mbfield_parser *parser,
               bool *have_pending_पङ्क्तिः, bool write_field, idx_t *n_bytes)
{
  पटिनिवत्त (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_पङ्क्तिः,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_पङ्क्तिः, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

स्थिर गणना field_terminator
scan_mb_पङ्क्तिः_end (mbbuf_t *mbbuf, संरचना mbfield_parser *parser,
                  bool *have_pending_पङ्क्तिः)
{
  सचे (parser->have_saved)
    {
      parser->have_saved = false;
      सचे (parser->saved_g.ch == MBBUF_EOF)
        पटिनिवत्त FIELD_EOF;

      *have_pending_पङ्क्तिः = true;
      सचे (parser->saved_g.ch == पङ्क्तिः_delim)
        पटिनिवत्त FIELD_पङ्क्तिः_DELIMITER;
    }

  याव (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      सचे (available == 0)
        पटिनिवत्त FIELD_EOF;

      वर्ण *buffer = mbbuf->buffer + mbbuf->offset;
      वर्ण *पङ्क्तिः_end = अन्वेषणम्_bytes (buffer, पङ्क्तिः_delim, available);
      idx_t n = पङ्क्तिः_end ? पङ्क्तिः_end - buffer + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_पङ्क्तिः = true;

      सचे (पङ्क्तिः_end)
        पटिनिवत्त FIELD_पङ्क्तिः_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
स्थिर वर्ण *
find_field_delim (वर्ण *buf, size_t len)
{
  सचे (len < delim_mcel.len)
    पटिनिवत्त रिक्त;

#सचे ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  पटिनिवत्त memmem (buf, len, delim_bytes, delim_mcel.len);
#अञ्ञथा
  सचे (delim_mcel.len == 1)
    पटिनिवत्त memchr (buf, delim_bytes[0], len);

  वर्ण ध्रुव *p = buf;
  वर्ण ध्रुव *end = buf + len;

  वर्ण saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  याव (p < end)
    {
      वर्ण ध्रुव *nul = memchr (p, '\0', end - p);
      सचे (!nul)
        {
          वर्ण ध्रुव *match = strstr (p, delim_bytes);
          buf[len] = saved;
          पटिनिवत्त (वर्ण *) match;
        }

      सचे (p < nul)
        {
          वर्ण ध्रुव *match = strstr (p, delim_bytes);
          सचे (match)
            {
              buf[len] = saved;
              पटिनिवत्त (वर्ण *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  पटिनिवत्त रिक्त;
#endif
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
स्थिर वर्ण *
find_c_blank (वर्ण *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  अत्थाय (idx_t i = 0; i < n; i++)
    सचे (c_isblank (buf[i]))
      पटिनिवत्त buf + i;

  पटिनिवत्त memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_PURE
स्थिर idx_t
field_delim_overlap (वर्ण ध्रुव *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  याव (0 < overlap)
    {
      सचे (memeq (buf + len - overlap, delim_bytes, overlap))
        पटिनिवत्त overlap;
      overlap--;
    }

  पटिनिवत्त 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

स्थिर अन्तःस्थ गणना field_terminator
find_field_terminator (वर्ण *buf, idx_t len,
                       संरचना bytesearch_context *ctx, वर्ण **terminator)
{
  सचे (ctx->mode == BYTESEARCH_पङ्क्तिः_ONLY)
    {
      सचे (!ctx->पङ्क्तिः_end_known)
        {
          ctx->पङ्क्तिः_end = memchr (buf, पङ्क्तिः_delim, len);
          ctx->पङ्क्तिः_end_known = true;
        }

      *terminator = ctx->पङ्क्तिः_end;
      पटिनिवत्त *terminator ? FIELD_पङ्क्तिः_DELIMITER : FIELD_DATA;
    }

  सचे (field_delim_is_पङ्क्तिः_delim ())
    {
      *terminator = memchr (buf, पङ्क्तिः_delim, len);
      पटिनिवत्त *terminator ? FIELD_DELIMITER : FIELD_DATA;
    }

  सचे (!ctx->पङ्क्तिः_end_known)
    {
      ctx->पङ्क्तिः_end = memchr (buf, पङ्क्तिः_delim, len);
      ctx->पङ्क्तिः_end_known = true;
    }

  idx_t field_len = ctx->पङ्क्तिः_end ? ctx->पङ्क्तिः_end - buf : len;

  वर्ण *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  सचे (field_end)
    {
      *terminator = field_end;
      पटिनिवत्त FIELD_DELIMITER;
    }

  *terminator = ctx->पङ्क्तिः_end;
  पटिनिवत्त ctx->पङ्क्तिः_end ? FIELD_पङ्क्तिः_DELIMITER : FIELD_DATA;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

स्थिर अन्तःस्थ शून्य
maybe_write_पङ्क्तिः_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  सचे (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_पङ्क्तिः_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

स्थिर अन्तःस्थ bool
begin_field_output (uintmax_t field_idx, bool buffer_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  सचे (write_field && ! (field_idx == 1 && buffer_first_field))
    {
      सचे (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *found_any_selected_field = true;
    }

  पटिनिवत्त write_field;
}

स्थिर अन्तःस्थ bool
handle_field_1 (uintmax_t field_idx, bool buffer_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  सचे (field_idx == 1 && buffer_first_field)
    {
      सचे ((field_1_selected = print_kth (1)))
        write_bytes (field_1_buffer, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  पटिनिवत्त field_1_selected;
}

स्थिर अन्तःस्थ शून्य
handle_field_delimiter (uintmax_t *field_idx, bool buffer_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  सचे (handle_field_1 (*field_idx, buffer_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  next_item (field_idx);
  *write_field = begin_field_output (*field_idx, buffer_first_field,
                                     found_any_selected_field);
}

स्थिर अन्तःस्थ bool
field_selection_exhausted (uintmax_t field_idx)
{
  पटिनिवत्त !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

स्थिर अन्तःस्थ शून्य
finish_current_पङ्क्तिः (uintmax_t field_idx, bool buffer_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool पङ्क्तिः_terminated)
{
  सचे (field_idx == 1 && buffer_first_field)
    {
      सचे (!suppress_non_delimited
          && (पङ्क्तिः_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_buffer, *field_1_n_bytes);
          write_पङ्क्तिः_delim ();
        }
      *field_1_n_bytes = 0;
    }
  अञ्ञथा
    maybe_write_पङ्क्तिः_delim (found_any_selected_field, field_idx);
}

स्थिर अन्तःस्थ शून्य
sync_byte_selection (uintmax_t byte_idx)
{
  याव (current_rp->hi <= byte_idx)
    current_rp++;
}

स्थिर अन्तःस्थ शून्य
reset_field_पङ्क्तिः (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_पङ्क्तिः, संरचना mbfield_parser *parser)
{
  *field_idx = 1;
  current_rp = frp;
  *found_any_selected_field = false;
  *have_pending_पङ्क्तिः = false;
  parser->have_saved = false;
  parser->at_पङ्क्तिः_start = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

स्थिर शून्य
cut_bytes (FILE *stream)
{
  स्थिर वर्ण bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  पूर्णाङ्क fd = fileno (stream);

  current_rp = frp;

  याव (true)
    {
      idx_t available = पठ (fd, bytes_in, परिमाण bytes_in);
      सचे (available <= 0)
        {
          सचे (available < 0)
            fseterr (stream);
          write_pending_पङ्क्तिः_delim (byte_idx);
          भङ्ग;
        }

      idx_t processed = 0;

      याव (processed < available)
        {
          वर्ण *पङ्क्तिः = bytes_in + processed;
          idx_t to_process = available - processed;
          वर्ण *पङ्क्तिः_end = अन्वेषणम्_bytes (पङ्क्तिः, पङ्क्तिः_delim, to_process);
          वर्ण *end = पङ्क्तिः + (पङ्क्तिः_end ? पङ्क्तिः_end - पङ्क्तिः : to_process);
          वर्ण *p = पङ्क्तिः;

          याव (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              सचे (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              अञ्ञथा
                {
                  idx_t n = MIN (end - p, current_rp->hi - byte_idx);
                  write_selected_item (&print_delimiter,
                                       is_range_start_index (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += end - पङ्क्तिः;
          सचे (पङ्क्तिः_end)
            {
              processed++;
              reset_item_पङ्क्तिः (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

स्थिर शून्य
cut_characters_mode (FILE *stream, bool byte_mode)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  स्थिर वर्ण bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, परिमाण bytes_in, stream);

  याव (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      सचे (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          वर्ण *p = mbbuf.buffer + mbbuf.offset;
          वर्ण *end = अन्वेषणम्_bytes (p, पङ्क्तिः_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          सचे (!end && available)
            अनुवर्त;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      सचे (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          वर्ण ध्रुव *p = mbbuf.buffer + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          वर्ण ध्रुव *end = अन्वेषणम्_bytes (p, पङ्क्तिः_delim, n);
          सचे (end)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, end - p + 1);
              reset_item_पङ्क्तिः (&idx, &print_delimiter);
              अनुवर्त;
            }

          idx += u8_गणनम् (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      सचे (g.ch == पङ्क्तिः_delim)
        reset_item_पङ्क्तिः (&idx, &print_delimiter);
      अञ्ञथा सचे (g.ch == MBBUF_EOF)
        {
          write_pending_पङ्क्तिः_delim (idx);
          भङ्ग;
        }
      अञ्ञथा सचे (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          अत्थाय (idx_t i = 0; i < g.len; i++)
            {
              next_item (&idx);
              सचे (print_kth (idx))
                {
                  सचे (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_index (idx);
                    }
                }
              अञ्ञथा सचे (seen_selected)
                suffix_selected = false;
            }

          सचे (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      अञ्ञथा
        {
          next_item (&idx);
          सचे (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_index (idx),
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
    }
}

स्थिर शून्य
cut_bytes_no_split (FILE *stream)
{
  cut_characters_mode (stream, true);
}

स्थिर शून्य
cut_characters (FILE *stream)
{
  cut_characters_mode (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

स्थिर शून्य
cut_fields_mb_any (FILE *stream, bool whitespace_mode)
{
  स्थिर वर्ण bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  संरचना mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_mode,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_पङ्क्तिः_start = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool buffer_first_field;
  bool have_pending_पङ्क्तिः = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, परिमाण bytes_in, stream);

  buffer_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  याव (true)
    {
      गणना field_terminator terminator;
      सचे (!buffer_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_पङ्क्तिः,
                                    write_field, रिक्त);
      अञ्ञथा
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_पङ्क्तिः,
                                    false, &field_1_n_bytes);

      सचे (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          सचे (! field_selection_exhausted (field_idx))
            अनुवर्त;

          terminator = scan_mb_पङ्क्तिः_end (&mbbuf, &parser,
                                         &have_pending_पङ्क्तिः);
        }

      सचे (terminator == FIELD_EOF && !have_pending_पङ्क्तिः)
        भङ्ग;

      finish_current_पङ्क्तिः (field_idx, buffer_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_पङ्क्तिः_DELIMITER);
      सचे (terminator == FIELD_EOF)
        भङ्ग;

      reset_field_पङ्क्तिः (&field_idx, &found_any_selected_field,
                        &have_pending_पङ्क्तिः, &parser);
      write_field = begin_field_output (field_idx, buffer_first_field,
                                        &found_any_selected_field);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

स्थिर शून्य
cut_fields_mb (FILE *stream)
{
  cut_fields_mb_any (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

स्थिर शून्य
cut_fields_bytesearch (FILE *stream)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  स्थिर वर्ण bytes_in[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool have_pending_पङ्क्तिः = false;
  bool skip_blank_run = false;
  bool pending_पङ्क्तिः_delim_field = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  bool buffer_first_field = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, bytes_in, परिमाण bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  संरचना bytesearch_context अन्वेषणम् = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&अन्वेषणम्);

  याव (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      अन्वेषणम्.at_eof = mbbuf.eof;
      अन्वेषणम्.पङ्क्तिः_end_known = false;

      सचे (pending_पङ्क्तिः_delim_field)
        {
          pending_पङ्क्तिः_delim_field = false;

          सचे (n_avail == 0)
            {
              सचे (handle_field_1 (field_idx, buffer_first_field,
                                  &field_1_n_bytes))
                next_item (&field_idx);
              maybe_write_पङ्क्तिः_delim (found_any_selected_field, field_idx);
              पटिनिवत्त;
            }

          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          सचे (whitespace_delimited)
            skip_blank_run = true;
        }

      सचे (n_avail == 0)
        भङ्ग;

      वर्ण *chunk = mbbuf.buffer + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      सचे (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_पङ्क्तिः_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          वर्ण *last_पङ्क्तिः_delim = अन्वेषणम्.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, पङ्क्तिः_delim, n_avail);
          सचे (last_पङ्क्तिः_delim)
            {
              idx_t n = last_पङ्क्तिः_delim - chunk + 1;
              सचे (! suppress_non_delimited)
                {
                  write_bytes (field_1_buffer, field_1_n_bytes);
                  write_bytes (chunk, n);
                  सचे (अन्वेषणम्.at_eof && chunk[n - 1] != पङ्क्तिः_delim)
                    write_पङ्क्तिः_delim ();
                }
              field_1_n_bytes = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_पङ्क्तिः = false;
              सचे (अन्वेषणम्.at_eof)
                पटिनिवत्त;
              अनुवर्त;
            }
        }

      याव (processed < n_avail)
        {
          वर्ण *terminator = रिक्त;

          सचे (skip_blank_run)
            {
              याव (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              सचे (processed == n_avail)
                भङ्ग;
              skip_blank_run = false;
            }

          सचे (field_selection_exhausted (field_idx))
            {
              सचे (field_delim_is_पङ्क्तिः_delim ())
                {
                  maybe_write_पङ्क्तिः_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  पटिनिवत्त;
                }
              अन्वेषणम्.mode = BYTESEARCH_पङ्क्तिः_ONLY;
              गणना field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &अन्वेषणम्, &terminator);
              सचे (terminator_kind == FIELD_पङ्क्तिः_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_पङ्क्तिः_delim (found_any_selected_field,
                                          field_idx);
                  गच्छ reset_पङ्क्तिः;
                }
              processed = n_avail;
              भङ्ग;
            }

          गणना field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &अन्वेषणम्, &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          सचे (terminator_kind == FIELD_DATA
              && !अन्वेषणम्.at_eof
              && !whitespace_delimited
              && !field_delim_is_पङ्क्तिः_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          सचे (field_len || terminator)
            have_pending_पङ्क्तिः = true;

          /* Output field content.  */
          सचे (field_idx == 1 && buffer_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          अञ्ञथा सचे (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          सचे (terminator_kind == FIELD_DATA)
            भङ्ग;

          सचे (terminator_kind == FIELD_DELIMITER)
            {
              सचे (field_delim_is_पङ्क्तिः_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_पङ्क्तिः_delim_field = true;
                  भङ्ग;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, buffer_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              सचे (whitespace_delimited)
                skip_blank_run = true;
            }
          अञ्ञथा
            {
              affirm (terminator_kind == FIELD_पङ्क्तिः_DELIMITER);
              processed++;
              finish_current_पङ्क्तिः (field_idx, buffer_first_field,
                                   &field_1_n_bytes,
                                   found_any_selected_field, true);

            reset_पङ्क्तिः:
              field_idx = 1;
              current_rp = frp;
              found_any_selected_field = false;
              have_pending_पङ्क्तिः = false;
              bytesearch_context_reset (&अन्वेषणम्);
              write_field = begin_field_output (field_idx, buffer_first_field,
                                                &found_any_selected_field);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  सचे (!have_pending_पङ्क्तिः)
    पटिनिवत्त;

  finish_current_पङ्क्तिः (field_idx, buffer_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

स्थिर शून्य
cut_fields_ws (FILE *stream)
{
  सचे (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  अञ्ञथा
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

स्थिर bool
cut_file (वर्ण ध्रुव *file, शून्य (*cut_stream) (FILE *))
{
  FILE *stream;

  सचे (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = stdin;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  अञ्ञथा
    {
      stream = fopen (file, "r");
      सचे (stream == रिक्त)
        {
          error (0, errno, "%s", quotef (file));
          पटिनिवत्त false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  पूर्णाङ्क err = errno;
  सचे (!ferror (stream))
    err = 0;
  सचे (streq (file, "-"))
    clearerr (stream);		/* Also clear EOF.  */
  अञ्ञथा सचे (fclose (stream) == EOF)
    err = errno;
  सचे (err)
    {
      error (0, err, "%s", quotef (file));
      पटिनिवत्त false;
    }
  पटिनिवत्त true;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  वर्ण *spec_list_string = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  याव ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, रिक्त))
         != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'b':
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        पक्ष 'c':
          सचे (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        पक्ष 'f':
        पक्ष 'F':
          सचे (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
          सचे (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          सचे (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          भङ्ग;

        पक्ष 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            सचे (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            भङ्ग;
          }

        पक्ष 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          भङ्ग;

        पक्ष 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_length = (optarg[0] == '\0'
                                     ? 1 : strlen (optarg));
          output_delimiter_string = optarg;
          भङ्ग;

        पक्ष 'n':
          no_split = true;
          भङ्ग;

        पक्ष 's':
          suppress_non_delimited = true;
          भङ्ग;

        पक्ष 'z':
          पङ्क्तिः_delim = '\0';
          भङ्ग;

        पक्ष COMPLEMENT_OPTION:
          complement = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  सचे (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  सचे (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
    {
      सचे (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an निवेशः delimiter makes sense\n\
\tonly when operating on fields"));

      सचे (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited पङ्क्तयः makes sense\n\
\tonly when operating on fields"));
    }

  सचे (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  सचे (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  सचे (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  सचे (output_delimiter_string == रिक्त)
    {
      output_delimiter_string = output_delimiter_default;
      सचे (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_length = 1;
        }
      अञ्ञथा
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_length = delim_mcel.len;
        }
    }

  शून्य (*cut_stream) (FILE *) = रिक्त;
  विकल्प (cut_mode)
    {
    पक्ष CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      भङ्ग;

    पक्ष CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      भङ्ग;

    पक्ष CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      भङ्ग;

    पक्ष CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  सचे (optind == argc)
    ok = cut_file ("-", cut_stream);
  अञ्ञथा
    अत्थाय (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  सचे (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  पटिनिवत्त ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
