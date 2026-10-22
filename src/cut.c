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

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश "system.h"

#समावेश "argmatch.h"
#समावेश "assure.h"
#समावेश "c-ctype.h"
#समावेश "fadvise.h"
#समावेश "ioblksize.h"
#समावेश "mbbuf.h"
#समावेश "memchr2.h"

#समावेश "set-fields.h"
#समावेश "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "cut"

#घोषणा AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#घोषणा FATAL_ERROR(Message)						\
  करो									\
    {									\
      error (0, 0, (Message));						\
      usage (निकास_त्रुटि);						\
    }									\
  जबतक (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#घोषणा SMALL_BYTE_THRESHOLD 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
अटल काष्ठा field_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
अटल अक्षर *field_1_बफर;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
अटल idx_t field_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
अटल bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
अटल bool complement;

/* The delimiter character for multibyte field mode.  */
अटल mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
अटल अक्षर delim_bytes[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
अटल अचिन्हित अक्षर पंक्ति_delim = '\n';

/* The length of output_delimiter_string.  */
अटल माप_प्रकार output_delimiter_लंबाई;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
अटल अक्षर *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
अटल अक्षर output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
अटल bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
अटल bool no_split;

/* If true, interpret each run of whitespace as one field delimiter.  */
अटल bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
अटल bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
अटल bool space_output_delimiter_default;

क्रमागत whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

अटल अक्षर स्थिर *स्थिर whitespace_option_args[] =
{
  "trimmed", शून्य
};

अटल क्रमागत whitespace_option स्थिर whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_args, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
अटल क्रमागत
{
  CUT_MODE_NONE,
  CUT_MODE_BYTES,
  CUT_MODE_CHARACTERS,
  CUT_MODE_FIELDS
} cut_mode = CUT_MODE_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  COMPLEMENT_OPTION = अक्षर_उच्च + 1
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"bytes", required_argument, शून्य, 'b'},
  {"characters", required_argument, शून्य, 'c'},
  {"fields", required_argument, शून्य, 'f'},
  {"delimiter", required_argument, शून्य, 'd'},
  {"no-partial", no_argument, शून्य, 'n'},
  {"whitespace-delimited", optional_argument, शून्य, 'w'},
  {"only-delimited", no_argument, शून्य, 's'},
  {"output-delimiter", required_argument, शून्य, 'O'},
  {"complement", no_argument, शून्य, COMPLEMENT_OPTION},
  {"zero-terminated", no_argument, शून्य, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s OPTION... [खाता]...\n\
"),
              program_name);
      ख_माला_दो (_("\
Print selected parts of पंक्तियाँ from each खाता to standard output.\n\
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
         use DELIM instead of TAB क्रम field delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any पंक्ति that contains\n\
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
         the शेष is to use the निवेश delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         करो not print पंक्तियाँ not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a run of blank characters as the field delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पंक्ति delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Use one, and only one of -b, -c, -f or -F.  Each LIST is made up of one\n\
range, or many ranges separated by commas.  Selected निवेश is written\n\
in the same order that it is पढ़ो, and is written exactly once.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
Each range is one of:\n\
\n\
  N     N'th byte, character or field, counted from 1\n\
  N-    from N'th byte, character or field, to end of पंक्ति\n\
  N-M   from N'th to M'th (included) byte, character or field\n\
  -M    from first to M'th (included) byte, character or field\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

अटल अंतरभूत व्योम
अग्र_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  अगर ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

अटल अंतरभूत bool
print_kth (uintmax_t k)
{
  वापस current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

अटल अंतरभूत bool
is_range_start_सूचकांक (uintmax_t k)
{
  वापस k == current_rp->lo;
}

अटल अंतरभूत bool
field_delim_is_पंक्ति_delim (व्योम)
{
  वापस delim_mcel.len == 1 && delim_bytes[0] == पंक्ति_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

अटल bool
mcel_isblank (mcel_t g)
{
  वापस (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

अटल अंतरभूत bool
bytesearch_field_delim_ok (व्योम)
{
  अचिन्हित अक्षर delim_0 = delim_bytes[0];

  वापस (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


क्रमागत field_terminator
{
  FIELD_तथ्य,
  FIELD_DELIMITER,
  FIELD_पंक्ति_DELIMITER,
  FIELD_EOF
};

क्रमागत bytesearch_mode
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_पंक्ति_ONLY
};

काष्ठा bytesearch_context
{
  क्रमागत bytesearch_mode mode;
  bool blank_delimited;
  bool at_eof;
  अक्षर *पंक्ति_end;
  bool पंक्ति_end_known;
};

अटल अंतरभूत व्योम
bytesearch_context_reset (काष्ठा bytesearch_context *ctx)
{
  ctx->mode = BYTESEARCH_FIELDS;
  ctx->पंक्ति_end = शून्य;
  ctx->पंक्ति_end_known = false;
}

काष्ठा mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_पंक्ति_start;
  bool have_saved;
  mcel_t saved_g;
};

अटल अंतरभूत mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  अगर (*have_saved)
    {
      *have_saved = false;
      वापस *saved_g;
    }
  वापस mbbuf_get_char (mbbuf);
}

अटल अंतरभूत क्रमागत field_terminator
skip_whitespace_run (mbbuf_t *mbuf, काष्ठा mbfield_parser *parser,
                     bool *have_pending_पंक्ति,
                     bool have_initial_whitespace)
{
  mcel_t g;

  करो
    {
      g = mbbuf_get_char (mbuf);
      अगर (g.ch != MBBUF_EOF)
        *have_pending_पंक्ति = true;
    }
  जबतक (g.ch != MBBUF_EOF && g.ch != पंक्ति_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_पंक्ति_start;

  अगर (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == पंक्ति_delim))
    {
      वापस g.ch == MBBUF_EOF ? FIELD_EOF : FIELD_पंक्ति_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  वापस trim_start && !have_initial_whitespace ? FIELD_तथ्य : FIELD_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

अटल व्योम
write_bytes (अक्षर स्थिर *buf, माप_प्रकार n_bytes)
{
  अगर (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      क्रम (माप_प्रकार i = 0; i < n_bytes; i++)
        अगर (अक्षर_दो (buf[i]) < 0)
          write_error ();
      वापस;
    }

  अगर (ख_डालो (buf, माप (अक्षर), n_bytes, मानक_निकास) != n_bytes)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

अटल अंतरभूत व्योम
copy_bytes (अक्षर *dst, अक्षर स्थिर *src, माप_प्रकार n_bytes)
{
  अगर (n_bytes <= SMALL_BYTE_THRESHOLD)
    {
      क्रम (माप_प्रकार i = 0; i < n_bytes; i++)
        dst[i] = src[i];
      वापस;
    }

  स_नकल (dst, src, n_bytes);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

अटल अंतरभूत व्योम*
खोज_bytes (अक्षर स्थिर *buf, पूर्णांक c, माप_प्रकार n_bytes)
{
  अगर (n_bytes != 0 && to_uchar (buf[0]) == c)
    वापस (अक्षर *) buf;
  अगर (1 < n_bytes && to_uchar (buf[1]) == c)
    वापस (अक्षर *) buf + 1;

  वापस 2 < n_bytes ? (अक्षर *) स_प्रथम (buf + 2, c, n_bytes - 2) : शून्य;
}

अटल अंतरभूत व्योम
write_पंक्ति_delim (व्योम)
{
  अगर (अक्षर_दो (पंक्ति_delim) < 0)
    write_error ();
}

अटल अंतरभूत व्योम
reset_item_पंक्ति (uintmax_t *item_idx, bool *print_delimiter)
{
  write_पंक्ति_delim ();
  *item_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

अटल अंतरभूत व्योम
write_pending_पंक्ति_delim (uintmax_t item_idx)
{
  अगर (item_idx > 0)
    write_पंक्ति_delim ();
}

अटल अंतरभूत व्योम
write_selected_item (bool *print_delimiter, bool range_start,
                     अक्षर स्थिर *buf, माप_प्रकार n_bytes)
{
  अगर (output_delimiter_string != output_delimiter_default)
    {
      अगर (*print_delimiter && range_start)
        write_bytes (output_delimiter_string, output_delimiter_लंबाई);
      *print_delimiter = true;
    }

  write_bytes (buf, n_bytes);
}

अटल अंतरभूत व्योम
append_field_1_chunk (अक्षर स्थिर *buf, idx_t len, idx_t *n_bytes)
{
  अगर (field_1_bufsize - *n_bytes < len)
    {
      field_1_बफर = xpalloc (field_1_बफर, &field_1_bufsize,
                                len, -1, माप *field_1_बफर);
    }

  copy_bytes (field_1_बफर + *n_bytes, buf, len);
  *n_bytes += len;
}

अटल अंतरभूत व्योम
append_field_1_bytes (mbbuf_t *mbbuf, mcel_t g, idx_t *n_bytes)
{
  append_field_1_chunk (mbbuf_char_offset (mbbuf, g), g.len, n_bytes);
}

अटल अंतरभूत क्रमागत field_terminator
scan_mb_blank_field (mbbuf_t *mbbuf, काष्ठा mbfield_parser *parser,
                     bool *have_pending_पंक्ति, bool write_field,
                     idx_t *n_bytes)
{
  अगर (parser->trim_outer_whitespace && parser->at_पंक्ति_start)
    {
      क्रमागत field_terminator terminator
        = skip_whitespace_run (mbbuf, parser, have_pending_पंक्ति, false);
      अगर (terminator != FIELD_तथ्य)
        वापस terminator;
    }

  parser->at_पंक्ति_start = false;

  जबतक (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      अगर (g.ch == MBBUF_EOF)
        वापस FIELD_EOF;

      *have_pending_पंक्ति = true;

      अगर (g.ch == पंक्ति_delim)
        वापस FIELD_पंक्ति_DELIMITER;

      अगर (mcel_isblank (g))
        वापस skip_whitespace_run (mbbuf, parser, have_pending_पंक्ति, true);

      अगर (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      अन्यथा अगर (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

अटल अंतरभूत क्रमागत field_terminator
scan_mb_delim_field (mbbuf_t *mbbuf, bool *have_pending_पंक्ति,
                     bool write_field, idx_t *n_bytes)
{
  जबतक (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      अगर (g.ch == MBBUF_EOF)
        वापस FIELD_EOF;

      *have_pending_पंक्ति = true;

      अगर (g.ch == पंक्ति_delim)
        वापस FIELD_पंक्ति_DELIMITER;

      अगर (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        वापस FIELD_DELIMITER;

      अगर (n_bytes)
        append_field_1_bytes (mbbuf, g, n_bytes);
      अन्यथा अगर (write_field)
        write_bytes (mbbuf_char_offset (mbbuf, g), g.len);
    }
}

अटल अंतरभूत क्रमागत field_terminator
scan_mb_field (mbbuf_t *mbbuf, काष्ठा mbfield_parser *parser,
               bool *have_pending_पंक्ति, bool write_field, idx_t *n_bytes)
{
  वापस (parser->whitespace_delimited
          ? scan_mb_blank_field (mbbuf, parser, have_pending_पंक्ति,
                                 write_field, n_bytes)
          : scan_mb_delim_field (mbbuf, have_pending_पंक्ति, write_field,
                                 n_bytes));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

अटल क्रमागत field_terminator
scan_mb_पंक्ति_end (mbbuf_t *mbbuf, काष्ठा mbfield_parser *parser,
                  bool *have_pending_पंक्ति)
{
  अगर (parser->have_saved)
    {
      parser->have_saved = false;
      अगर (parser->saved_g.ch == MBBUF_EOF)
        वापस FIELD_EOF;

      *have_pending_पंक्ति = true;
      अगर (parser->saved_g.ch == पंक्ति_delim)
        वापस FIELD_पंक्ति_DELIMITER;
    }

  जबतक (true)
    {
      idx_t available = mbbuf_topup (mbbuf);
      अगर (available == 0)
        वापस FIELD_EOF;

      अक्षर *बफर = mbbuf->बफर + mbbuf->offset;
      अक्षर *पंक्ति_end = खोज_bytes (बफर, पंक्ति_delim, available);
      idx_t n = पंक्ति_end ? पंक्ति_end - बफर + 1 : available;
      mbbuf_advance (mbbuf, n);
      *have_pending_पंक्ति = true;

      अगर (पंक्ति_end)
        वापस FIELD_पंक्ति_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_PURE
अटल अक्षर *
find_field_delim (अक्षर *buf, माप_प्रकार len)
{
  अगर (len < delim_mcel.len)
    वापस शून्य;

#अगर ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  वापस memmem (buf, len, delim_bytes, delim_mcel.len);
#अन्यथा
  अगर (delim_mcel.len == 1)
    वापस स_प्रथम (buf, delim_bytes[0], len);

  अक्षर स्थिर *p = buf;
  अक्षर स्थिर *end = buf + len;

  अक्षर saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  जबतक (p < end)
    {
      अक्षर स्थिर *nul = स_प्रथम (p, '\0', end - p);
      अगर (!nul)
        {
          अक्षर स्थिर *match = म_माला (p, delim_bytes);
          buf[len] = saved;
          वापस (अक्षर *) match;
        }

      अगर (p < nul)
        {
          अक्षर स्थिर *match = म_माला (p, delim_bytes);
          अगर (match)
            {
              buf[len] = saved;
              वापस (अक्षर *) match;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  वापस शून्य;
#पूर्ण_अगर
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_PURE
अटल अक्षर *
find_c_blank (अक्षर *buf, माप_प्रकार len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  क्रम (idx_t i = 0; i < n; i++)
    अगर (c_isblank (buf[i]))
      वापस buf + i;

  वापस memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_PURE
अटल idx_t
field_delim_overlap (अक्षर स्थिर *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  जबतक (0 < overlap)
    {
      अगर (memeq (buf + len - overlap, delim_bytes, overlap))
        वापस overlap;
      overlap--;
    }

  वापस 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

अटल अंतरभूत क्रमागत field_terminator
find_field_terminator (अक्षर *buf, idx_t len,
                       काष्ठा bytesearch_context *ctx, अक्षर **terminator)
{
  अगर (ctx->mode == BYTESEARCH_पंक्ति_ONLY)
    {
      अगर (!ctx->पंक्ति_end_known)
        {
          ctx->पंक्ति_end = स_प्रथम (buf, पंक्ति_delim, len);
          ctx->पंक्ति_end_known = true;
        }

      *terminator = ctx->पंक्ति_end;
      वापस *terminator ? FIELD_पंक्ति_DELIMITER : FIELD_तथ्य;
    }

  अगर (field_delim_is_पंक्ति_delim ())
    {
      *terminator = स_प्रथम (buf, पंक्ति_delim, len);
      वापस *terminator ? FIELD_DELIMITER : FIELD_तथ्य;
    }

  अगर (!ctx->पंक्ति_end_known)
    {
      ctx->पंक्ति_end = स_प्रथम (buf, पंक्ति_delim, len);
      ctx->पंक्ति_end_known = true;
    }

  idx_t field_len = ctx->पंक्ति_end ? ctx->पंक्ति_end - buf : len;

  अक्षर *field_end = (ctx->blank_delimited
                     ? find_c_blank (buf, field_len)
                     : find_field_delim (buf, field_len));

  अगर (field_end)
    {
      *terminator = field_end;
      वापस FIELD_DELIMITER;
    }

  *terminator = ctx->पंक्ति_end;
  वापस ctx->पंक्ति_end ? FIELD_पंक्ति_DELIMITER : FIELD_तथ्य;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

अटल अंतरभूत व्योम
maybe_write_पंक्ति_delim (bool found_any_selected_field, uintmax_t field_idx)
{
  अगर (found_any_selected_field
      || !(suppress_non_delimited && field_idx == 1))
    write_पंक्ति_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

अटल अंतरभूत bool
begin_field_output (uintmax_t field_idx, bool बफर_first_field,
                    bool *found_any_selected_field)
{
  bool write_field = print_kth (field_idx);

  अगर (write_field && ! (field_idx == 1 && बफर_first_field))
    {
      अगर (*found_any_selected_field)
        write_bytes (output_delimiter_string, output_delimiter_लंबाई);
      *found_any_selected_field = true;
    }

  वापस write_field;
}

अटल अंतरभूत bool
handle_field_1 (uintmax_t field_idx, bool बफर_first_field,
                idx_t *field_1_n_bytes)
{
  bool field_1_selected = false;

  अगर (field_idx == 1 && बफर_first_field)
    {
      अगर ((field_1_selected = print_kth (1)))
        write_bytes (field_1_बफर, *field_1_n_bytes);
      *field_1_n_bytes = 0;
    }

  वापस field_1_selected;
}

अटल अंतरभूत व्योम
handle_field_delimiter (uintmax_t *field_idx, bool बफर_first_field,
                        idx_t *field_1_n_bytes,
                        bool *found_any_selected_field, bool *write_field)
{
  अगर (handle_field_1 (*field_idx, बफर_first_field, field_1_n_bytes))
    *found_any_selected_field = true;

  अग्र_item (field_idx);
  *write_field = begin_field_output (*field_idx, बफर_first_field,
                                     found_any_selected_field);
}

अटल अंतरभूत bool
field_selection_exhausted (uintmax_t field_idx)
{
  वापस !print_kth (field_idx) && current_rp->lo == UINTMAX_MAX;
}

अटल अंतरभूत व्योम
finish_current_पंक्ति (uintmax_t field_idx, bool बफर_first_field,
                     idx_t *field_1_n_bytes,
                     bool found_any_selected_field, bool पंक्ति_terminated)
{
  अगर (field_idx == 1 && बफर_first_field)
    {
      अगर (!suppress_non_delimited
          && (पंक्ति_terminated || *field_1_n_bytes != 0))
        {
          write_bytes (field_1_बफर, *field_1_n_bytes);
          write_पंक्ति_delim ();
        }
      *field_1_n_bytes = 0;
    }
  अन्यथा
    maybe_write_पंक्ति_delim (found_any_selected_field, field_idx);
}

अटल अंतरभूत व्योम
sync_byte_selection (uintmax_t byte_idx)
{
  जबतक (current_rp->hi <= byte_idx)
    current_rp++;
}

अटल अंतरभूत व्योम
reset_field_पंक्ति (uintmax_t *field_idx, bool *found_any_selected_field,
                  bool *have_pending_पंक्ति, काष्ठा mbfield_parser *parser)
{
  *field_idx = 1;
  current_rp = frp;
  *found_any_selected_field = false;
  *have_pending_पंक्ति = false;
  parser->have_saved = false;
  parser->at_पंक्ति_start = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

अटल व्योम
cut_bytes (खाता *stream)
{
  अटल अक्षर bytes_in[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  पूर्णांक fd = fileno (stream);

  current_rp = frp;

  जबतक (true)
    {
      idx_t available = पढ़ो (fd, bytes_in, माप bytes_in);
      अगर (available <= 0)
        {
          अगर (available < 0)
            fseterr (stream);
          write_pending_पंक्ति_delim (byte_idx);
          अवरोध;
        }

      idx_t processed = 0;

      जबतक (processed < available)
        {
          अक्षर *पंक्ति = bytes_in + processed;
          idx_t to_process = available - processed;
          अक्षर *पंक्ति_end = खोज_bytes (पंक्ति, पंक्ति_delim, to_process);
          अक्षर *end = पंक्ति + (पंक्ति_end ? पंक्ति_end - पंक्ति : to_process);
          अक्षर *p = पंक्ति;

          जबतक (0 < end - p)
            {
              sync_byte_selection (byte_idx);

              अगर (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (end - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              अन्यथा
                {
                  idx_t n = MIN (end - p, current_rp->hi - byte_idx);
                  write_selected_item (&print_delimiter,
                                       is_range_start_सूचकांक (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += end - पंक्ति;
          अगर (पंक्ति_end)
            {
              processed++;
              reset_item_पंक्ति (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

अटल व्योम
cut_characters_mode (खाता *stream, bool byte_mode)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  अटल अक्षर bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, माप bytes_in, stream);

  जबतक (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      अगर (idx && field_selection_exhausted (idx))
        {
          idx_t available = mbbuf_topup (&mbbuf);
          अक्षर *p = mbbuf.बफर + mbbuf.offset;
          अक्षर *end = खोज_bytes (p, पंक्ति_delim, available);
          mbbuf_advance (&mbbuf, end ? end - p : available);
          अगर (!end && available)
            जारी;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      अगर (!byte_mode && utf8 && idx + 1 < current_rp->lo)
        {
          अक्षर स्थिर *p = mbbuf.बफर + mbbuf.offset;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          अक्षर स्थिर *end = खोज_bytes (p, पंक्ति_delim, n);
          अगर (end)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, end - p + 1);
              reset_item_पंक्ति (&idx, &print_delimiter);
              जारी;
            }

          idx += u8_गिनती (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      अगर (g.ch == पंक्ति_delim)
        reset_item_पंक्ति (&idx, &print_delimiter);
      अन्यथा अगर (g.ch == MBBUF_EOF)
        {
          write_pending_पंक्ति_delim (idx);
          अवरोध;
        }
      अन्यथा अगर (byte_mode)
        {
          bool first_selected_is_range_start = false;
          bool seen_selected = false;
          bool suffix_selected = true;

          क्रम (idx_t i = 0; i < g.len; i++)
            {
              अग्र_item (&idx);
              अगर (print_kth (idx))
                {
                  अगर (!seen_selected)
                    {
                      seen_selected = true;
                      first_selected_is_range_start
                        = is_range_start_सूचकांक (idx);
                    }
                }
              अन्यथा अगर (seen_selected)
                suffix_selected = false;
            }

          अगर (seen_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
      अन्यथा
        {
          अग्र_item (&idx);
          अगर (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_सूचकांक (idx),
                                 mbbuf_char_offset (&mbbuf, g), g.len);
        }
    }
}

अटल व्योम
cut_bytes_no_split (खाता *stream)
{
  cut_characters_mode (stream, true);
}

अटल व्योम
cut_characters (खाता *stream)
{
  cut_characters_mode (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

अटल व्योम
cut_fields_mb_any (खाता *stream, bool whitespace_mode)
{
  अटल अक्षर bytes_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  काष्ठा mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_mode,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_पंक्ति_start = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool बफर_first_field;
  bool have_pending_पंक्ति = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, bytes_in, माप bytes_in, stream);

  बफर_first_field = (suppress_non_delimited ^ !print_kth (1));
  write_field = begin_field_output (field_idx, बफर_first_field,
                                    &found_any_selected_field);

  जबतक (true)
    {
      क्रमागत field_terminator terminator;
      अगर (!बफर_first_field || field_idx != 1)
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_पंक्ति,
                                    write_field, शून्य);
      अन्यथा
        terminator = scan_mb_field (&mbbuf, &parser, &have_pending_पंक्ति,
                                    false, &field_1_n_bytes);

      अगर (terminator == FIELD_DELIMITER)
        {
          handle_field_delimiter (&field_idx, बफर_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          अगर (! field_selection_exhausted (field_idx))
            जारी;

          terminator = scan_mb_पंक्ति_end (&mbbuf, &parser,
                                         &have_pending_पंक्ति);
        }

      अगर (terminator == FIELD_EOF && !have_pending_पंक्ति)
        अवरोध;

      finish_current_पंक्ति (field_idx, बफर_first_field,
                           &field_1_n_bytes, found_any_selected_field,
                           terminator == FIELD_पंक्ति_DELIMITER);
      अगर (terminator == FIELD_EOF)
        अवरोध;

      reset_field_पंक्ति (&field_idx, &found_any_selected_field,
                        &have_pending_पंक्ति, &parser);
      write_field = begin_field_output (field_idx, बफर_first_field,
                                        &found_any_selected_field);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

अटल व्योम
cut_fields_mb (खाता *stream)
{
  cut_fields_mb_any (stream, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

अटल व्योम
cut_fields_bytesearch (खाता *stream)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  अटल अक्षर bytes_in[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t field_idx = 1;
  bool found_any_selected_field = false;
  bool have_pending_पंक्ति = false;
  bool skip_blank_run = false;
  bool pending_पंक्ति_delim_field = false;
  bool write_field;
  idx_t field_1_n_bytes = 0;

  current_rp = frp;
  bool बफर_first_field = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, bytes_in, माप bytes_in - 1, stream);
  write_field = begin_field_output (field_idx, बफर_first_field,
                                    &found_any_selected_field);

  काष्ठा bytesearch_context खोज = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&खोज);

  जबतक (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      खोज.at_eof = mbbuf.eof;
      खोज.पंक्ति_end_known = false;

      अगर (pending_पंक्ति_delim_field)
        {
          pending_पंक्ति_delim_field = false;

          अगर (n_avail == 0)
            {
              अगर (handle_field_1 (field_idx, बफर_first_field,
                                  &field_1_n_bytes))
                अग्र_item (&field_idx);
              maybe_write_पंक्ति_delim (found_any_selected_field, field_idx);
              वापस;
            }

          handle_field_delimiter (&field_idx, बफर_first_field,
                                  &field_1_n_bytes,
                                  &found_any_selected_field, &write_field);
          अगर (whitespace_delimited)
            skip_blank_run = true;
        }

      अगर (n_avail == 0)
        अवरोध;

      अक्षर *chunk = mbbuf.बफर + mbbuf.offset;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      अगर (field_idx == 1
          && (!whitespace_delimited || !field_selection_exhausted (field_idx))
          && !field_delim_is_पंक्ति_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_field_delim (chunk, n_avail)))
        {
          अक्षर *last_पंक्ति_delim = खोज.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, पंक्ति_delim, n_avail);
          अगर (last_पंक्ति_delim)
            {
              idx_t n = last_पंक्ति_delim - chunk + 1;
              अगर (! suppress_non_delimited)
                {
                  write_bytes (field_1_बफर, field_1_n_bytes);
                  write_bytes (chunk, n);
                  अगर (खोज.at_eof && chunk[n - 1] != पंक्ति_delim)
                    write_पंक्ति_delim ();
                }
              field_1_n_bytes = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_पंक्ति = false;
              अगर (खोज.at_eof)
                वापस;
              जारी;
            }
        }

      जबतक (processed < n_avail)
        {
          अक्षर *terminator = शून्य;

          अगर (skip_blank_run)
            {
              जबतक (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              अगर (processed == n_avail)
                अवरोध;
              skip_blank_run = false;
            }

          अगर (field_selection_exhausted (field_idx))
            {
              अगर (field_delim_is_पंक्ति_delim ())
                {
                  maybe_write_पंक्ति_delim (found_any_selected_field,
                                          field_idx);
                  mbbuf_advance (&mbbuf, processed);
                  वापस;
                }
              खोज.mode = BYTESEARCH_पंक्ति_ONLY;
              क्रमागत field_terminator terminator_kind
                = find_field_terminator (chunk + processed, n_avail - processed,
                                         &खोज, &terminator);
              अगर (terminator_kind == FIELD_पंक्ति_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_पंक्ति_delim (found_any_selected_field,
                                          field_idx);
                  जाओ reset_पंक्ति;
                }
              processed = n_avail;
              अवरोध;
            }

          क्रमागत field_terminator terminator_kind
            = find_field_terminator (chunk + processed, n_avail - processed,
                                     &खोज, &terminator);
          idx_t field_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          अगर (terminator_kind == FIELD_तथ्य
              && !खोज.at_eof
              && !whitespace_delimited
              && !field_delim_is_पंक्ति_delim ())
            field_len -= field_delim_overlap (chunk + processed, field_len);

          अगर (field_len || terminator)
            have_pending_पंक्ति = true;

          /* Output field content.  */
          अगर (field_idx == 1 && बफर_first_field)
            append_field_1_chunk (chunk + processed, field_len,
                                  &field_1_n_bytes);
          अन्यथा अगर (write_field)
            write_bytes (chunk + processed, field_len);
          processed += field_len;

          अगर (terminator_kind == FIELD_तथ्य)
            अवरोध;

          अगर (terminator_kind == FIELD_DELIMITER)
            {
              अगर (field_delim_is_पंक्ति_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_पंक्ति_delim_field = true;
                  अवरोध;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_field_delimiter (&field_idx, बफर_first_field,
                                      &field_1_n_bytes,
                                      &found_any_selected_field, &write_field);
              अगर (whitespace_delimited)
                skip_blank_run = true;
            }
          अन्यथा
            {
              affirm (terminator_kind == FIELD_पंक्ति_DELIMITER);
              processed++;
              finish_current_पंक्ति (field_idx, बफर_first_field,
                                   &field_1_n_bytes,
                                   found_any_selected_field, true);

            reset_पंक्ति:
              field_idx = 1;
              current_rp = frp;
              found_any_selected_field = false;
              have_pending_पंक्ति = false;
              bytesearch_context_reset (&खोज);
              write_field = begin_field_output (field_idx, बफर_first_field,
                                                &found_any_selected_field);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  अगर (!have_pending_पंक्ति)
    वापस;

  finish_current_पंक्ति (field_idx, बफर_first_field, &field_1_n_bytes,
                       found_any_selected_field, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

अटल व्योम
cut_fields_ws (खाता *stream)
{
  अगर (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (stream);
  अन्यथा
    cut_fields_mb_any (stream, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

अटल bool
cut_file (अक्षर स्थिर *file, व्योम (*cut_stream) (खाता *))
{
  खाता *stream;

  अगर (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = मानक_निवेश;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  अन्यथा
    {
      stream = ख_खोलो (file, "r");
      अगर (stream == शून्य)
        {
          error (0, त्रुटि_सं, "%s", quotef (file));
          वापस false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  cut_stream (stream);

  पूर्णांक err = त्रुटि_सं;
  अगर (!ख_त्रुटि (stream))
    err = 0;
  अगर (streq (file, "-"))
    साफ_त्रुटि (stream);		/* Also clear EOF.  */
  अन्यथा अगर (ख_बंद (stream) == खातापूर्ण)
    err = त्रुटि_सं;
  अगर (err)
    {
      error (0, err, "%s", quotef (file));
      वापस false;
    }
  वापस true;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  अक्षर *spec_list_string = शून्य;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, शून्य))
         != -1)
    {
      चयन (optc)
        {
        हाल 'b':
          cut_mode = CUT_MODE_BYTES;
          FALLTHROUGH;
        हाल 'c':
          अगर (optc == 'c')
            cut_mode = CUT_MODE_CHARACTERS;
          FALLTHROUGH;
        हाल 'f':
        हाल 'F':
          अगर (optc == 'f' || optc == 'F')
            cut_mode = CUT_MODE_FIELDS;
          अगर (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          अगर (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          अवरोध;

        हाल 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            अगर (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_bytes (delim_bytes, optarg, g.len + 1);
            delim_specified = true;
            अवरोध;
          }

        हाल 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_args,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          अवरोध;

        हाल 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_लंबाई = (optarg[0] == '\0'
                                     ? 1 : म_माप (optarg));
          output_delimiter_string = optarg;
          अवरोध;

        हाल 'n':
          no_split = true;
          अवरोध;

        हाल 's':
          suppress_non_delimited = true;
          अवरोध;

        हाल 'z':
          पंक्ति_delim = '\0';
          अवरोध;

        हाल COMPLEMENT_OPTION:
          complement = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  अगर (cut_mode == CUT_MODE_BYTES || cut_mode == CUT_MODE_CHARACTERS)
    {
      अगर (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an निवेश delimiter makes sense\n\
\tonly when operating on fields"));

      अगर (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited पंक्तियाँ makes sense\n\
\tonly when operating on fields"));
    }

  अगर (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  अगर (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  set_fields (spec_list_string,
              (((cut_mode == CUT_MODE_BYTES
                 || cut_mode == CUT_MODE_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  अगर (!delim_specified)
    {
      delim_bytes[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  अगर (output_delimiter_string == शून्य)
    {
      output_delimiter_string = output_delimiter_default;
      अगर (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_लंबाई = 1;
        }
      अन्यथा
        {
          copy_bytes (output_delimiter_default, delim_bytes, delim_mcel.len);
          output_delimiter_लंबाई = delim_mcel.len;
        }
    }

  व्योम (*cut_stream) (खाता *) = शून्य;
  चयन (cut_mode)
    {
    हाल CUT_MODE_BYTES:
      cut_stream = MB_CUR_MAX <= 1 || !no_split
                   ? cut_bytes : cut_bytes_no_split;
      अवरोध;

    हाल CUT_MODE_CHARACTERS:
      cut_stream = MB_CUR_MAX <= 1 ? cut_bytes : cut_characters;
      अवरोध;

    हाल CUT_MODE_FIELDS:
      cut_stream = whitespace_delimited ? cut_fields_ws
                   : bytesearch_field_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      अवरोध;

    हाल CUT_MODE_NONE:
      affirm (false);
    }
  affirm (cut_stream);
  अगर (optind == argc)
    ok = cut_file ("-", cut_stream);
  अन्यथा
    क्रम (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_stream);


  अगर (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    {
      error (0, त्रुटि_सं, "-");
      ok = false;
    }

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
