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
  करा									\
    {									\
      error (0, 0, (Message));						\
      usage (निकास_त्रुटि);						\
    }									\
  पर्यंत (0)

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
स्थिर अक्षर *field_1_buffer;

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
स्थिर अक्षर delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
स्थिर अचिन्हित अक्षर ओळ_delim = '\n';

/* The length of output_delimiter_string.  */
स्थिर माप_प्रकार output_delimiter_length;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
स्थिर अक्षर *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
स्थिर अक्षर output_delimiter_default[MCEL_LEN_MAX];

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

स्थिर अक्षर स्थिरांक *स्थिरांक whitespace_option_args[] =
{
  "trimmed", रिक्त
};

स्थिर गणना whitespace_option स्थिरांक whitespace_option_types[] =
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
  COMPLEMENT_OPTION = अक्षर_उच्च + 1
};

स्थिर संरचना option स्थिरांक longopts[] =
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
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s OPTION... [खाता]...\n\
"),
              program_name);
      ख_माला_दो (_("\
Print selected parts of ओळी from each खाता to standard output.\n\
"), मानक_निकास);

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
         use DELIM instead of TAB साठी field delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any ओळ that contains\n\
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
         the पूर्वनिर्धारित is to use the आदान delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         करा not print ओळी not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a run of blank characters as the field delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ओळ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Use one, and only one of -b, -c, -f or -F.  Each LIST is made up of one\n\
range, or many ranges separated by commas.  Selected आदान is written\n\
in the same order that it is पठ, and is written exactly once.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
Each range is one of:\n\
\n\
  N     N'th byte, character or field, counted from 1\n\
  N-    from N'th byte, character or field, to end of ओळ\n\
  N-M   from N'th to M'th (included) byte, character or field\n\
  -M    from first to M'th (included) byte, character or field\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

स्थिर ओळीत शून्य
next_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  जर ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

स्थिर ओळीत bool
print_kth (uintmax_t k)
{
  परत current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

स्थिर ओळीत bool
is_range_start_index (uintmax_t k)
{
  परत k == current_rp->lo;
}

स्थिर ओळीत bool
field_delim_is_ओळ_delim (शून्य)
{
  परत delim_mcel.len == 1 && delim_bytes[0] == ओळ_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

स्थिर bool
mcel_isblank (mcel_t g)
{
  परत (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

स्थिर ओळीत bool
bytesearch_field_delim_ok (शून्य)
{
  अचिन्हित अक्षर delim_0 = delim_bytes[0];

  परत (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


गणना field_terminator
{
  FIELD_DATA,
  FIELD_DELIMITER,
  FIELD_ओळ_DELIMITER,
  FIELD_EOF
};

गणना bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_ओळ_ONLY
};

संरचना bytesearch_context
{
  गणना bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  अक्षर *ओळ_end;
  bool ओळ_end_known;
};

स्थिर ओळीत शून्य
bytesearch_context_reset (संरचना bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->ओळ_end = रिक्त;
  ctx->ओळ_end_known = false;
}

संरचना mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_ओळ_start;
  bool have_saved;
  mcel_t saved_g;
};

स्थिर ओळीत mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  जर (*have_saved)
    {
      *have_saved = false;
      परत *saved_g;
    }
  परत mbbuf_get_char (mbbuf);
}

स्थिर ओळीत गणना field_terminator
skip_whitespace_run (mbbuf_t *mbuf, संरचना mbfield_parser *parser,
                     bool *have_pending_ओळ,
                     bool have_initial_whitespace)
{
  mcel_t g;

  करा
    {
      g = mbbuf_get_char (mbuf);
      जर (g.ch != MBBUF_EOF)
        *have_pending_ओळ = true;
    }
  पर्यंत (g.ch != MBBUF_EOF && g.ch != ओळ_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_ओळ_start;

  जर (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == ओळ_delim))
    {
      परत g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_ओळ_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  परत trim_start && !have_initial_whitespace ? FIELD_DATA : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

स्थिर शून्य
write_bytes (अक्षर स्थिरांक *buf, माप_प्रकार n_bytes)
{
  जर (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      साठी (माप_प्रकार i = 0; i < n_bytes; i++)
        जर (अक्षर_दो (buf[i]) < 0)
          write_error ();
      परत;
    }

  जर (ख_डालो (buf, आकार (अक्षर), n_bytes, मानक_निकास) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

स्थिर ओळीत शून्य
copy_bytes (अक्षर *dst, अक्षर स्थिरांक *src, माप_प्रकार n_bytes)
{
  जर (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      साठी (माप_प्रकार i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      परत;
    }

  स_नकल (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

स्थिर ओळीत शून्य*
शोध_bytes (अक्षर स्थिरांक *buf, पूर्णांक c, माप_प्रकार n_bytes)
{
  जर (n_bytes != 0 && to_uchar (buf[0]) == c)
    परत (अक्षर *) buf;
  जर (1 < n_bytes && to_uchar (buf[1]) == c)
    परत (अक्षर *) buf + 1;

  परत 2 < n_bytes ? (अक्षर *) स_प्रथम (buf + 2, c, n_bytes - 2) : रिक्त;
}

स्थिर ओळीत शून्य
write_ओळ_delim (शून्य)
{
  जर (अक्षर_दो (ओळ_delim) < 0)
    write_error ();
}

स्थिर ओळीत शून्य
reset_item_ओळ (uintmax_t *item_idx, bool *print_delimiter)
{
  write_ओळ_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

स्थिर ओळीत शून्य
write_pending_ओळ_delim (uintmax_t item_idx)
{
  जर (item_idx > 0)
    write_ओळ_delim ();
}

स्थिर ओळीत शून्य
write_selected_item (bool *print_delimiter, bool range_start,
                     अक्षर स्थिरांक *buf, माप_प्रकार n_bytes)
{
  जर (output_delimiter_string != output_delimiter_default)
    {
      जर (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

स्थिर ओळीत शून्य
append_field_1_chunk (अक्षर स्थिरांक *buf, idx_t len, idx_t *n_bytes)
{
  जर (field_1_bufsize - *n_bytes < len)
    {
      field_1_buffer = xpalloc (field_1_buffer, &field_1_bufsize,
                                len, -1, आकार *field_1_buffer);
    }

  copy_bytes (field_1_buffer + *n_bytes, buf, len);
  *n_bytes += len;
}

स्थिर ओळीत शून्य
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

स्थिर ओळीत गणना field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, संरचना mbfield_parser *parser,
                     bool *have_pending_ओळ, bool write_field,
                     idx_t *n_bytes)
{
  जर (parser->trim_outer_whitespace && parser->at_ओळ_start)
    {
      गणना field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_ओळ, false);
      जर (terminator != FIELD_DATA)
        परत terminator;
    }

  parser->at_ओळ_start = false;

  पर्यंत (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      जर (g.ch == MBBUF_EOF)
        परत FIELD_EOF;

      *have_pending_ओळ = true;

      जर (g.ch == ओळ_delim)
        परत FIELD_ओळ_DELIMITER;

      जर (mcel_isblank (g))
        परत skip_whitespace_run (mbbuf, parser, have_pending_ओळ, true);

      जर (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      अन्यथा जर (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

स्थिर ओळीत गणना field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_ओळ,
                     bool write_field, idx_t *n_bytes)
{
  पर्यंत (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      जर (g.ch == MBBUF_EOF)
        परत FIELD_EOF;

      *have_pending_ओळ = true;

      जर (g.ch == ओळ_delim)
        परत FIELD_ओळ_DELIMITER;

      जर (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        परत FIELD_DELIMITER;

      जर (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      अन्यथा जर (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

स्थिर ओळीत गणना field_terminator
scan_mb_field (mbbuf_t *mbbuf, संरचना mbfield_parser *parser,
               bool *have_pending_ओळ, bool write_field, idx_t *n_bytes)
{
  परत (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_ओळ,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_ओळ, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

स्थिर गणना field_terminator
scan_mb_ओळ_end (mbbuf_t *mbbuf, संरचना mbfield_parser *parser,
                  bool *have_pending_ओळ)
{
  जर (parser->have_saved)
    {
      parser->have_saved = false;
      जर (parser->saved_g.ch == MBBUF_EOF)
        परत FIELD_EOF;

      *have_pending_ओळ = true;
      जर (parser->saved_g.ch == ओळ_delim)
        परत FIELD_ओळ_DELIMITER;
    }

  पर्यंत (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      जर (available == 0)
        परत FIELD_EOF;

      अक्षर *buffer = mbbuf->buffer + mbbuf->offset;
      अक्षर *ओळ_end = शोध_bytes (buffer, ओळ_delim, available);
      idx_t n = ओळ_end ? ओळ_end - buffer + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_ओळ = true;

      जर (ओळ_end)
        परत FIELD_ओळ_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
स्थिर अक्षर *
find_field_delim (अक्षर *buf, माप_प्रकार len)
{
  जर (len < delim_mcel.len)
    परत रिक्त;

#जर ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  परत memmem (buf, len, delim_bytes, delim_mcel.len);
#अन्यथा
  जर (delim_mcel.len == 1)
    परत स_प्रथम (buf, delim_bytes[0], len);

  अक्षर स्थिरांक *p = buf;
  अक्षर स्थिरांक *end = buf + len;

  अक्षर saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  पर्यंत (p < end)
    {
      अक्षर स्थिरांक *nul = स_प्रथम (p, '\0', end - p);
      जर (!nul)
        {
          अक्षर स्थिरांक *match = म_माला (p, delim_bytes);
          buf[len] = saved;
          परत (अक्षर *) match;
        }

      जर (p < nul)
        {
          अक्षर स्थिरांक *match = म_माला (p, delim_bytes);
          जर (match)
            {
              buf[len] = saved;
              परत (अक्षर *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  परत रिक्त;
#endif
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
स्थिर अक्षर *
find_c_blank (अक्षर *buf, माप_प्रकार len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  साठी (idx_t i = 0; i < n; i++)
    जर (c_isblank (buf[i]))
      परत buf + i;

  परत memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_PURE
स्थिर idx_t
field_delim_overlap (अक्षर स्थिरांक *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  पर्यंत (0 < overlap)
    {
      जर (memeq (buf + len - overlap, delim_bytes, overlap))
        परत overlap;
      overlap--;
    }

  परत 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

स्थिर ओळीत गणना field_terminator
find_field_terminator (अक्षर *buf, idx_t len,
                       संरचना bytesearch_context *ctx, अक्षर **terminator)
{
  जर (ctx->mode == BYTESEARCH_ओळ_ONLY)
    {
      जर (!ctx->ओळ_end_known)
        {
          ctx->ओळ_end = स_प्रथम (buf, ओळ_delim, len);
          ctx->ओळ_end_known = true;
        }

      *terminator = ctx->ओळ_end;
      परत *terminator ? FIELD_ओळ_DELIMITER : FIELD_DATA;
    }

  जर (field_delim_is_ओळ_delim ())
    {
      *terminator = स_प्रथम (buf, ओळ_delim, len);
      परत *terminator ? FIELD_DELIMITER : FIELD_DATA;
    }

  जर (!ctx->ओळ_end_known)
    {
      ctx->ओळ_end = स_प्रथम (buf, ओळ_delim, len);
      ctx->ओळ_end_known = true;
    }

  idx_t field_len = ctx->ओळ_end ? ctx->ओळ_end - buf : len;

  अक्षर *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  जर (field_end)
    {
      *terminator = field_end;
      परत FIELD_DELIMITER;
    }

  *terminator = ctx->ओळ_end;
  परत ctx->ओळ_end ? FIELD_ओळ_DELIMITER : FIELD_DATA;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

स्थिर ओळीत शून्य
maybe_write_ओळ_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  जर (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_ओळ_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

स्थिर ओळीत bool
begin_field_output (uintmax_t field_idx, bool buffer_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  जर (write_field && ! (field_idx == 1 && buffer_first_field))
    {
      जर (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_length);
      *found_any_selected_field = true;
    }

  परत write_field;
}

स्थिर ओळीत bool
handle_field_1 (uintmax_t field_idx, bool buffer_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  जर (field_idx == 1 && buffer_first_field)
    {
      जर ((field_1_selected = print_kth (1)))
        write_bytes (field_1_buffer, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  परत field_1_selected;
}

स्थिर ओळीत शून्य
handle_field_delimiter (uintmax_t *field_idx, bool buffer_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  जर (handle_field_1 (*field_idx, buffer_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  next_item (field_idx);
  *write_field = begin_field_output (*field_idx, buffer_first_field,
                                     found_any_selected_field);
}

स्थिर ओळीत bool
field_selection_exhausted (uintmax_t field_idx)
{
  परत !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

स्थिर ओळीत शून्य
finish_current_ओळ (uintmax_t field_idx, bool buffer_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool ओळ_terminated)
{
  जर (field_idx == 1 && buffer_first_field)
    {
      जर (!suppress_non_delimited
          && (ओळ_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_buffer, *field_1_n_bytes);
          write_ओळ_delim ();
        }
      *field_1_n_bytes = 0;
    }
  अन्यथा
    maybe_write_ओळ_delim (found_any_selected_field, field_idx);
}

स्थिर ओळीत शून्य
sync_byte_selection (uintmax_t byte_idx)
{
  पर्यंत (current_rp->hi <= byte_idx)
    current_rp++;
}

स्थिर ओळीत शून्य
reset_field_ओळ (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_ओळ, संरचना mbfield_parser *parser)
{
  *field_idx = 1;
  current_rp = frp;
  *found_any_selected_field = false;
  *have_pending_ओळ = false;
  parser->have_saved = false;
  parser->at_ओळ_start = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

स्थिर शून्य
cut_bytes (खाता *stream)
{
  स्थिर अक्षर bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  पूर्णांक fd = fileno (stream);

  current_rp = frp;

  पर्यंत (true)
    {
      idx_t available = पठ (fd, bytes_in, आकार bytes_in);
      जर (available <= 0)
        {
          जर (available < 0)
            fseterr (stream);
          write_pending_ओळ_delim (byte_idx);
          थांबा;
        }

      idx_t processed = 0;

      पर्यंत (processed < available)
        {
          अक्षर *ओळ = bytes_in + processed;
          idx_t to_process = available - processed;
          अक्षर *ओळ_end = शोध_bytes (ओळ, ओळ_delim, to_process);
          अक्षर *end = ओळ + (ओळ_end ? ओळ_end - ओळ : to_process);
          अक्षर *p = ओळ;

          पर्यंत (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              जर (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              अन्यथा
                {
                  idx_t n = MIN (end - p, current_rp->hi - byte_idx);
                  write_selected_item (&print_delimiter,
                                       is_range_start_index (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += end - ओळ;
          जर (ओळ_end)
            {
              processed++;
              reset_item_ओळ (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

स्थिर शून्य
cut_characters_mode (खाता *stream, bool byte_mode)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  स्थिर अक्षर bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, आकार bytes_in, stream);

  पर्यंत (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      जर (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          अक्षर *p = mbbuf.buffer + mbbuf.offset;
          अक्षर *end = शोध_bytes (p, ओळ_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          जर (!end && available)
            सुरूठेवा;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      जर (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          अक्षर स्थिरांक *p = mbbuf.buffer + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          अक्षर स्थिरांक *end = शोध_bytes (p, ओळ_delim, n);
          जर (end)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, end - p + 1);
              reset_item_ओळ (&idx, &print_delimiter);
              सुरूठेवा;
            }

          idx += u8_मोजणी (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      जर (g.ch == ओळ_delim)
        reset_item_ओळ (&idx, &print_delimiter);
      अन्यथा जर (g.ch == MBBUF_EOF)
        {
          write_pending_ओळ_delim (idx);
          थांबा;
        }
      अन्यथा जर (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          साठी (idx_t i = 0; i < g.len; i++)
            {
              next_item (&idx);
              जर (print_kth (idx))
                {
                  जर (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_index (idx);
                    }
                }
              अन्यथा जर (seen_selected)
                suffix_selected = false;
            }

          जर (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      अन्यथा
        {
          next_item (&idx);
          जर (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_index (idx),
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
    }
}

स्थिर शून्य
cut_bytes_no_split (खाता *stream)
{
  cut_characters_mode (stream, true);
}

स्थिर शून्य
cut_characters (खाता *stream)
{
  cut_characters_mode (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

स्थिर शून्य
cut_fields_mb_any (खाता *stream, bool whitespace_mode)
{
  स्थिर अक्षर bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  संरचना mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_mode,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_ओळ_start = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool buffer_first_field;
  bool have_pending_ओळ = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, आकार bytes_in, stream);

  buffer_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  पर्यंत (true)
    {
      गणना field_terminator terminator;
      जर (!buffer_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_ओळ,
                                    write_field, रिक्त);
      अन्यथा
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_ओळ,
                                    false, &field_1_n_bytes);

      जर (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          जर (! field_selection_exhausted (field_idx))
            सुरूठेवा;

          terminator = scan_mb_ओळ_end (&mbbuf, &parser,
                                         &have_pending_ओळ);
        }

      जर (terminator == FIELD_EOF && !have_pending_ओळ)
        थांबा;

      finish_current_ओळ (field_idx, buffer_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_ओळ_DELIMITER);
      जर (terminator == FIELD_EOF)
        थांबा;

      reset_field_ओळ (&field_idx, &found_any_selected_field,
                        &have_pending_ओळ, &parser);
      write_field = begin_field_output (field_idx, buffer_first_field,
                                        &found_any_selected_field);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

स्थिर शून्य
cut_fields_mb (खाता *stream)
{
  cut_fields_mb_any (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

स्थिर शून्य
cut_fields_bytesearch (खाता *stream)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  स्थिर अक्षर bytes_in[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool have_pending_ओळ = false;
  bool skip_blank_run = false;
  bool pending_ओळ_delim_field = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  bool buffer_first_field = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, bytes_in, आकार bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, buffer_first_field,
                                    &found_any_selected_field);

  संरचना bytesearch_context शोध = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&शोध);

  पर्यंत (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      शोध.at_eof = mbbuf.eof;
      शोध.ओळ_end_known = false;

      जर (pending_ओळ_delim_field)
        {
          pending_ओळ_delim_field = false;

          जर (n_avail == 0)
            {
              जर (handle_field_1 (field_idx, buffer_first_field,
                                  &field_1_n_bytes))
                next_item (&field_idx);
              maybe_write_ओळ_delim (found_any_selected_field, field_idx);
              परत;
            }

          handle_field_delimiter (&field_idx, buffer_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          जर (whitespace_delimited)
            skip_blank_run = true;
        }

      जर (n_avail == 0)
        थांबा;

      अक्षर *chunk = mbbuf.buffer + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      जर (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_ओळ_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          अक्षर *last_ओळ_delim = शोध.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, ओळ_delim, n_avail);
          जर (last_ओळ_delim)
            {
              idx_t n = last_ओळ_delim - chunk + 1;
              जर (! suppress_non_delimited)
                {
                  write_bytes (field_1_buffer, field_1_n_bytes);
                  write_bytes (chunk, n);
                  जर (शोध.at_eof && chunk[n - 1] != ओळ_delim)
                    write_ओळ_delim ();
                }
              field_1_n_bytes = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_ओळ = false;
              जर (शोध.at_eof)
                परत;
              सुरूठेवा;
            }
        }

      पर्यंत (processed < n_avail)
        {
          अक्षर *terminator = रिक्त;

          जर (skip_blank_run)
            {
              पर्यंत (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              जर (processed == n_avail)
                थांबा;
              skip_blank_run = false;
            }

          जर (field_selection_exhausted (field_idx))
            {
              जर (field_delim_is_ओळ_delim ())
                {
                  maybe_write_ओळ_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  परत;
                }
              शोध.mode = BYTESEARCH_ओळ_ONLY;
              गणना field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &शोध, &terminator);
              जर (terminator_kind == FIELD_ओळ_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_ओळ_delim (found_any_selected_field,
                                          field_idx);
                  जा reset_ओळ;
                }
              processed = n_avail;
              थांबा;
            }

          गणना field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &शोध, &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          जर (terminator_kind == FIELD_DATA
              && !शोध.at_eof
              && !whitespace_delimited
              && !field_delim_is_ओळ_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          जर (field_len || terminator)
            have_pending_ओळ = true;

          /* Output field content.  */
          जर (field_idx == 1 && buffer_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          अन्यथा जर (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          जर (terminator_kind == FIELD_DATA)
            थांबा;

          जर (terminator_kind == FIELD_DELIMITER)
            {
              जर (field_delim_is_ओळ_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_ओळ_delim_field = true;
                  थांबा;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, buffer_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              जर (whitespace_delimited)
                skip_blank_run = true;
            }
          अन्यथा
            {
              affirm (terminator_kind == FIELD_ओळ_DELIMITER);
              processed++;
              finish_current_ओळ (field_idx, buffer_first_field,
                                   &field_1_n_bytes,
                                   found_any_selected_field, true);

            reset_ओळ:
              field_idx = 1;
              current_rp = frp;
              found_any_selected_field = false;
              have_pending_ओळ = false;
              bytesearch_context_reset (&शोध);
              write_field = begin_field_output (field_idx, buffer_first_field,
                                                &found_any_selected_field);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  जर (!have_pending_ओळ)
    परत;

  finish_current_ओळ (field_idx, buffer_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

स्थिर शून्य
cut_fields_ws (खाता *stream)
{
  जर (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  अन्यथा
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

स्थिर bool
cut_file (अक्षर स्थिरांक *file, शून्य (*cut_stream) (खाता *))
{
  खाता *stream;

  जर (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = मानक_निवेश;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  अन्यथा
    {
      stream = ख_खोलो (file, "r");
      जर (stream == रिक्त)
        {
          error (0, त्रुटि_सं, "%s", quotef (file));
          परत false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  पूर्णांक err = त्रुटि_सं;
  जर (!ख_त्रुटि (stream))
    err = 0;
  जर (streq (file, "-"))
    साफ_त्रुटि (stream);		/* Also clear EOF.  */
  अन्यथा जर (ख_बंद (stream) == खातापूर्ण)
    err = त्रुटि_सं;
  जर (err)
    {
      error (0, err, "%s", quotef (file));
      परत false;
    }
  परत true;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  अक्षर *spec_list_string = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, रिक्त))
         != -1)
    {
      निवड (optc)
        {
        स्थिती 'b':
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        स्थिती 'c':
          जर (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        स्थिती 'f':
        स्थिती 'F':
          जर (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
          जर (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          जर (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          थांबा;

        स्थिती 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            जर (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            थांबा;
          }

        स्थिती 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          थांबा;

        स्थिती 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_length = (optarg[0] == '\0'
                                     ? 1 : म_माप (optarg));
          output_delimiter_string = optarg;
          थांबा;

        स्थिती 'n':
          no_split = true;
          थांबा;

        स्थिती 's':
          suppress_non_delimited = true;
          थांबा;

        स्थिती 'z':
          ओळ_delim = '\0';
          थांबा;

        स्थिती COMPLEMENT_OPTION:
          complement = true;
          थांबा;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  जर (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
    {
      जर (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an आदान delimiter makes sense\n\
\tonly when operating on fields"));

      जर (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited ओळी makes sense\n\
\tonly when operating on fields"));
    }

  जर (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  जर (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  जर (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  जर (output_delimiter_string == रिक्त)
    {
      output_delimiter_string = output_delimiter_default;
      जर (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_length = 1;
        }
      अन्यथा
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_length = delim_mcel.len;
        }
    }

  शून्य (*cut_stream) (खाता *) = रिक्त;
  निवड (cut_mode)
    {
    स्थिती CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      थांबा;

    स्थिती CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      थांबा;

    स्थिती CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      थांबा;

    स्थिती CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  जर (optind == argc)
    ok = cut_file ("-", cut_stream);
  अन्यथा
    साठी (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  जर (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    {
      error (0, त्रुटि_सं, "-");
      ok = false;
    }

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
