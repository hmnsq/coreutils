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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mbbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "memchr2.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "set-fields.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "cut"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FATAL_ERROR(ਸੁਨੇਹਾ)						\
  ਕਰੋ									\
    {									\
      error (0, 0, (ਸੁਨੇਹਾ));						\
      usage (EXIT_FAILURE);						\
    }									\
  ਜਦੋਂ (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SMALL_BYTE_ਹੱਦ 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
ਸਥਿਰ ਬਣਤਰ ਖੇਤਰ_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
ਸਥਿਰ ਅੱਖਰ *ਖੇਤਰ_1_ਬਫਰ;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
ਸਥਿਰ idx_t ਖੇਤਰ_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
ਸਥਿਰ bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
ਸਥਿਰ bool complement;

/* The delimiter character for multibyte field mode.  */
ਸਥਿਰ mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
ਸਥਿਰ ਅੱਖਰ delim_ਬਾਈਟਾਂ[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਸਤਰ_delim = '\n';

/* The length of output_delimiter_string.  */
ਸਥਿਰ size_t output_delimiter_ਲੰਮਾਈ;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
ਸਥਿਰ ਅੱਖਰ *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
ਸਥਿਰ ਅੱਖਰ output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
ਸਥਿਰ bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
ਸਥਿਰ bool no_ਵੰਡੋ;

/* If true, interpret each run of whitespace as one field delimiter.  */
ਸਥਿਰ bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
ਸਥਿਰ bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
ਸਥਿਰ bool space_output_delimiter_default;

ਗਿਣਤੀ whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ whitespace_option_ਦਲੀਲਾਂ[] =
{
  "trimmed", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ whitespace_option ਅਚਲ whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_ਦਲੀਲਾਂ, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
ਸਥਿਰ ਗਿਣਤੀ
{
  CUT_ਬਹੁਲਕ_NONE,
  CUT_ਬਹੁਲਕ_ਬਾਈਟਾਂ,
  CUT_ਬਹੁਲਕ_CHARACTERS,
  CUT_ਬਹੁਲਕ_FIELDS
} cut_ਬਹੁਲਕ = CUT_ਬਹੁਲਕ_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"bytes", required_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"characters", required_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"fields", required_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"delimiter", required_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"no-partial", no_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"whitespace-delimited", optional_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  {"only-delimited", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"output-delimiter", required_ਦਲੀਲ, ਨਿਰਾ, 'O'},
  {"complement", no_ਦਲੀਲ, ਨਿਰਾ, COMPLEMENT_OPTION},
  {"zero-terminated", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Print selected parts ਦਾ ਸਤਰਾਂ from each FILE ਮੰਜ਼ਿਲ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --ਬਾਈਟਾਂ=LIST\n\
         select only these byte positions\n\
"));
      oputs (_("\
  -c, --characters=LIST\n\
         select only these character positions\n\
"));
      oputs (_("\
      --complement\n\
         complement the ਸੈੱਟ_2 ਦਾ selected ਬਾਈਟਾਂ, characters or fields\n\
"));
      oputs (_("\
  -d, --delimiter=DELIM\n\
         use DELIM instead ਦਾ TAB ਲਈ ਖੇਤਰ delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any ਸਤਰ that ਸ਼ਾਮਲ_ਹੈ\n\
         no delimiter character, unless the -s option is specified\n\
"));
      oputs (_("\
  -F LIST\n\
         like -f, but also ਸਿੱਟਾ_ਦਿੰਦਾ -w and -O ' '\n\
"));
      oputs (_("\
  -n, --no-partial\n\
         with -b, don't output partial multi-byte characters\n\
"));
      oputs (_("\
  -O, --output-delimiter=STRING\n\
         use STRING as the output delimiter;\n\
         the ਮੂਲ is ਮੰਜ਼ਿਲ use the ਨਿਵੇਸ਼ delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         ਕਰੋ not print ਸਤਰਾਂ not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a ਲੜੀਵਾਰ_2 ਦਾ blank characters as the ਖੇਤਰ delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ਸਤਰ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one ਦਾ -b, -c, -f or -F.  Each LIST is made ਉੱਪਰ ਦਾ one\n\
range, or many ranges separated by commas.  Selected ਨਿਵੇਸ਼ is written\n\
ਵਿੱਚ the same ਕ੍ਰਮ that it is ਪਠ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one ਦਾ:\n\
\n\
  N     N'th byte, character or ਖੇਤਰ, counted from 1\n\
  N-    from N'th byte, character or ਖੇਤਰ, ਮੰਜ਼ਿਲ ਸਿਰਾ ਦਾ ਸਤਰ\n\
  N-M   from N'th to M'th (included) byte, character or ਖੇਤਰ\n\
  -M    from ਪਹਿਲਾ ਮੰਜ਼ਿਲ M'th (included) byte, character or ਖੇਤਰ\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਅਗਲਾ_ਵਸਤੂ (uintmax_t *ਵਸਤੂ_idx)
{
  (*ਵਸਤੂ_idx)++;
  ਜੇ ((*ਵਸਤੂ_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
print_kth (uintmax_t k)
{
  ਵਾਪਸ current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_range_ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ (uintmax_t k)
{
  ਵਾਪਸ k == current_rp->lo;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ਖੇਤਰ_delim_is_ਸਤਰ_delim (ਖਾਲੀ)
{
  ਵਾਪਸ delim_mcel.len == 1 && delim_ਬਾਈਟਾਂ[0] == ਸਤਰ_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

ਸਥਿਰ bool
mcel_isblank (mcel_t g)
{
  ਵਾਪਸ (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
bytesearch_ਖੇਤਰ_delim_ok (ਖਾਲੀ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ delim_0 = delim_ਬਾਈਟਾਂ[0];

  ਵਾਪਸ (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


ਗਿਣਤੀ ਖੇਤਰ_terminator
{
  ਖੇਤਰ_ਅੰਕੜੇ,
  ਖੇਤਰ_DELIMITER,
  ਖੇਤਰ_ਸਤਰ_DELIMITER,
  ਖੇਤਰ_EOF
};

ਗਿਣਤੀ bytesearch_ਬਹੁਲਕ
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_ਸਤਰ_ONLY
};

ਬਣਤਰ bytesearch_context
{
  ਗਿਣਤੀ bytesearch_ਬਹੁਲਕ ਬਹੁਲਕ_2;
  bool blank_delimited;
  bool at_eof;
  ਅੱਖਰ *ਸਤਰ_ਸਿਰਾ;
  bool ਸਤਰ_ਸਿਰਾ_ਗਿਆਤ;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
bytesearch_context_reset (ਬਣਤਰ bytesearch_context *ctx)
{
  ctx->ਬਹੁਲਕ_2 = BYTESEARCH_FIELDS;
  ctx->ਸਤਰ_ਸਿਰਾ = ਨਿਰਾ;
  ctx->ਸਤਰ_ਸਿਰਾ_ਗਿਆਤ = false;
}

ਬਣਤਰ mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_ਸਤਰ_ਸ਼ੁਰੂ;
  bool have_saved;
  mcel_t saved_g;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  ਜੇ (*have_saved)
    {
      *have_saved = false;
      ਵਾਪਸ *saved_g;
    }
  ਵਾਪਸ mbbuf_get_char (mbbuf);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ ਖੇਤਰ_terminator
skip_whitespace_ਲੜੀਵਾਰ (mbbuf_t *mbuf, ਬਣਤਰ mbfield_parser *parser,
                     bool *have_pending_ਸਤਰ,
                     bool have_initial_whitespace)
{
  mcel_t g;

  ਕਰੋ
    {
      g = mbbuf_get_char (mbuf);
      ਜੇ (g.ch != MBBUF_EOF)
        *have_pending_ਸਤਰ = true;
    }
  ਜਦੋਂ (g.ch != MBBUF_EOF && g.ch != ਸਤਰ_delim && mcel_isblank (g));

  bool trim_ਸ਼ੁਰੂ = parser->trim_outer_whitespace && parser->at_ਸਤਰ_ਸ਼ੁਰੂ;

  ਜੇ (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == ਸਤਰ_delim))
    {
      ਵਾਪਸ g.ch == MBBUF_EOF ? ਖੇਤਰ_EOF : ਖੇਤਰ_ਸਤਰ_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  ਵਾਪਸ trim_ਸ਼ੁਰੂ && !have_initial_whitespace ? ਖੇਤਰ_ਅੰਕੜੇ : ਖੇਤਰ_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

ਸਥਿਰ ਖਾਲੀ
write_ਬਾਈਟਾਂ (ਅੱਖਰ ਅਚਲ *buf, size_t n_ਬਾਈਟਾਂ)
{
  ਜੇ (n_ਬਾਈਟਾਂ <= SMALL_BYTE_ਹੱਦ)
    {
      ਲਈ (size_t i = 0; i < n_ਬਾਈਟਾਂ; i++)
        ਜੇ (ਅੱਖਰ_ਲਿਖੋ (buf[i]) < 0)
          write_error ();
      ਵਾਪਸ;
    }

  ਜੇ (fwrite (buf, ਆਕਾਰ (ਅੱਖਰ), n_ਬਾਈਟਾਂ, stdout) != n_ਬਾਈਟਾਂ)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
copy_ਬਾਈਟਾਂ (ਅੱਖਰ *dst, ਅੱਖਰ ਅਚਲ *src, size_t n_ਬਾਈਟਾਂ)
{
  ਜੇ (n_ਬਾਈਟਾਂ <= SMALL_BYTE_ਹੱਦ)
    {
      ਲਈ (size_t i = 0; i < n_ਬਾਈਟਾਂ; i++)
        dst[i] = src[i];
      ਵਾਪਸ;
    }

  ਸਮ੍ਰਿਤੀ_ਨਕਲ (dst, src, n_ਬਾਈਟਾਂ);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ*
ਖੋਜ_ਬਾਈਟਾਂ (ਅੱਖਰ ਅਚਲ *buf, ਪੂਰਨ c, size_t n_ਬਾਈਟਾਂ)
{
  ਜੇ (n_ਬਾਈਟਾਂ != 0 && ਮੰਜ਼ਿਲ_uchar (buf[0]) == c)
    ਵਾਪਸ (ਅੱਖਰ *) buf;
  ਜੇ (1 < n_ਬਾਈਟਾਂ && ਮੰਜ਼ਿਲ_uchar (buf[1]) == c)
    ਵਾਪਸ (ਅੱਖਰ *) buf + 1;

  ਵਾਪਸ 2 < n_ਬਾਈਟਾਂ ? (ਅੱਖਰ *) memchr (buf + 2, c, n_ਬਾਈਟਾਂ - 2) : ਨਿਰਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
write_ਸਤਰ_delim (ਖਾਲੀ)
{
  ਜੇ (ਅੱਖਰ_ਲਿਖੋ (ਸਤਰ_delim) < 0)
    write_error ();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
reset_ਵਸਤੂ_ਸਤਰ (uintmax_t *ਵਸਤੂ_idx, bool *print_delimiter)
{
  write_ਸਤਰ_delim ();
  *ਵਸਤੂ_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
write_pending_ਸਤਰ_delim (uintmax_t ਵਸਤੂ_idx)
{
  ਜੇ (ਵਸਤੂ_idx > 0)
    write_ਸਤਰ_delim ();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
write_selected_ਵਸਤੂ (bool *print_delimiter, bool range_ਸ਼ੁਰੂ,
                     ਅੱਖਰ ਅਚਲ *buf, size_t n_ਬਾਈਟਾਂ)
{
  ਜੇ (output_delimiter_string != output_delimiter_default)
    {
      ਜੇ (*print_delimiter && range_ਸ਼ੁਰੂ)
        write_ਬਾਈਟਾਂ (output_delimiter_string, output_delimiter_ਲੰਮਾਈ);
      *print_delimiter = true;
    }

  write_ਬਾਈਟਾਂ (buf, n_ਬਾਈਟਾਂ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
append_ਖੇਤਰ_1_chunk (ਅੱਖਰ ਅਚਲ *buf, idx_t len, idx_t *n_ਬਾਈਟਾਂ)
{
  ਜੇ (ਖੇਤਰ_1_bufsize - *n_ਬਾਈਟਾਂ < len)
    {
      ਖੇਤਰ_1_ਬਫਰ = xpalloc (ਖੇਤਰ_1_ਬਫਰ, &ਖੇਤਰ_1_bufsize,
                                len, -1, ਆਕਾਰ *ਖੇਤਰ_1_ਬਫਰ);
    }

  copy_ਬਾਈਟਾਂ (ਖੇਤਰ_1_ਬਫਰ + *n_ਬਾਈਟਾਂ, buf, len);
  *n_ਬਾਈਟਾਂ += len;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
append_ਖੇਤਰ_1_ਬਾਈਟਾਂ (mbbuf_t *mbbuf, mcel_t g, idx_t *n_ਬਾਈਟਾਂ)
{
  append_ਖੇਤਰ_1_chunk (mbbuf_char_ਵਿੱਥ (mbbuf, g), g.len, n_ਬਾਈਟਾਂ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ ਖੇਤਰ_terminator
ਜਾਂਚ_mb_blank_ਖੇਤਰ (mbbuf_t *mbbuf, ਬਣਤਰ mbfield_parser *parser,
                     bool *have_pending_ਸਤਰ, bool write_ਖੇਤਰ,
                     idx_t *n_ਬਾਈਟਾਂ)
{
  ਜੇ (parser->trim_outer_whitespace && parser->at_ਸਤਰ_ਸ਼ੁਰੂ)
    {
      ਗਿਣਤੀ ਖੇਤਰ_terminator terminator
        = skip_whitespace_ਲੜੀਵਾਰ (mbbuf, parser, have_pending_ਸਤਰ, false);
      ਜੇ (terminator != ਖੇਤਰ_ਅੰਕੜੇ)
        ਵਾਪਸ terminator;
    }

  parser->at_ਸਤਰ_ਸ਼ੁਰੂ = false;

  ਜਦੋਂ (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      ਜੇ (g.ch == MBBUF_EOF)
        ਵਾਪਸ ਖੇਤਰ_EOF;

      *have_pending_ਸਤਰ = true;

      ਜੇ (g.ch == ਸਤਰ_delim)
        ਵਾਪਸ ਖੇਤਰ_ਸਤਰ_DELIMITER;

      ਜੇ (mcel_isblank (g))
        ਵਾਪਸ skip_whitespace_ਲੜੀਵਾਰ (mbbuf, parser, have_pending_ਸਤਰ, true);

      ਜੇ (n_ਬਾਈਟਾਂ)
        append_ਖੇਤਰ_1_ਬਾਈਟਾਂ (mbbuf, g, n_ਬਾਈਟਾਂ);
      ਨਹੀਂਤਾਂ ਜੇ (write_ਖੇਤਰ)
        write_ਬਾਈਟਾਂ (mbbuf_char_ਵਿੱਥ (mbbuf, g), g.len);
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ ਖੇਤਰ_terminator
ਜਾਂਚ_mb_delim_ਖੇਤਰ (mbbuf_t *mbbuf, bool *have_pending_ਸਤਰ,
                     bool write_ਖੇਤਰ, idx_t *n_ਬਾਈਟਾਂ)
{
  ਜਦੋਂ (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      ਜੇ (g.ch == MBBUF_EOF)
        ਵਾਪਸ ਖੇਤਰ_EOF;

      *have_pending_ਸਤਰ = true;

      ਜੇ (g.ch == ਸਤਰ_delim)
        ਵਾਪਸ ਖੇਤਰ_ਸਤਰ_DELIMITER;

      ਜੇ (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        ਵਾਪਸ ਖੇਤਰ_DELIMITER;

      ਜੇ (n_ਬਾਈਟਾਂ)
        append_ਖੇਤਰ_1_ਬਾਈਟਾਂ (mbbuf, g, n_ਬਾਈਟਾਂ);
      ਨਹੀਂਤਾਂ ਜੇ (write_ਖੇਤਰ)
        write_ਬਾਈਟਾਂ (mbbuf_char_ਵਿੱਥ (mbbuf, g), g.len);
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ ਖੇਤਰ_terminator
ਜਾਂਚ_mb_ਖੇਤਰ (mbbuf_t *mbbuf, ਬਣਤਰ mbfield_parser *parser,
               bool *have_pending_ਸਤਰ, bool write_ਖੇਤਰ, idx_t *n_ਬਾਈਟਾਂ)
{
  ਵਾਪਸ (parser->whitespace_delimited
          ? ਜਾਂਚ_mb_blank_ਖੇਤਰ (mbbuf, parser, have_pending_ਸਤਰ,
                                 write_ਖੇਤਰ, n_ਬਾਈਟਾਂ)
          : ਜਾਂਚ_mb_delim_ਖੇਤਰ (mbbuf, have_pending_ਸਤਰ, write_ਖੇਤਰ,
                                 n_ਬਾਈਟਾਂ));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

ਸਥਿਰ ਗਿਣਤੀ ਖੇਤਰ_terminator
ਜਾਂਚ_mb_ਸਤਰ_ਸਿਰਾ (mbbuf_t *mbbuf, ਬਣਤਰ mbfield_parser *parser,
                  bool *have_pending_ਸਤਰ)
{
  ਜੇ (parser->have_saved)
    {
      parser->have_saved = false;
      ਜੇ (parser->saved_g.ch == MBBUF_EOF)
        ਵਾਪਸ ਖੇਤਰ_EOF;

      *have_pending_ਸਤਰ = true;
      ਜੇ (parser->saved_g.ch == ਸਤਰ_delim)
        ਵਾਪਸ ਖੇਤਰ_ਸਤਰ_DELIMITER;
    }

  ਜਦੋਂ (true)
    {
      idx_t ਉਪਲਬਧ = mbbuf_topup (mbbuf);
      ਜੇ (ਉਪਲਬਧ == 0)
        ਵਾਪਸ ਖੇਤਰ_EOF;

      ਅੱਖਰ *ਬਫਰ = mbbuf->ਬਫਰ + mbbuf->ਵਿੱਥ;
      ਅੱਖਰ *ਸਤਰ_ਸਿਰਾ = ਖੋਜ_ਬਾਈਟਾਂ (ਬਫਰ, ਸਤਰ_delim, ਉਪਲਬਧ);
      idx_t n = ਸਤਰ_ਸਿਰਾ ? ਸਤਰ_ਸਿਰਾ - ਬਫਰ + 1 : ਉਪਲਬਧ;
      mbbuf_advance (mbbuf, n);
      *have_pending_ਸਤਰ = true;

      ਜੇ (ਸਤਰ_ਸਿਰਾ)
        ਵਾਪਸ ਖੇਤਰ_ਸਤਰ_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਅੱਖਰ *
ਲੱਭੋ_ਖੇਤਰ_delim (ਅੱਖਰ *buf, size_t len)
{
  ਜੇ (len < delim_mcel.len)
    ਵਾਪਸ ਨਿਰਾ;

#ਜੇ ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  ਵਾਪਸ memmem (buf, len, delim_ਬਾਈਟਾਂ, delim_mcel.len);
#ਨਹੀਂਤਾਂ
  ਜੇ (delim_mcel.len == 1)
    ਵਾਪਸ memchr (buf, delim_ਬਾਈਟਾਂ[0], len);

  ਅੱਖਰ ਅਚਲ *p = buf;
  ਅੱਖਰ ਅਚਲ *ਸਿਰਾ = buf + len;

  ਅੱਖਰ saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  ਜਦੋਂ (p < ਸਿਰਾ)
    {
      ਅੱਖਰ ਅਚਲ *nul = memchr (p, '\0', ਸਿਰਾ - p);
      ਜੇ (!nul)
        {
          ਅੱਖਰ ਅਚਲ *ਮੇਲ = ਲੜੀ_ਖੋਜ (p, delim_ਬਾਈਟਾਂ);
          buf[len] = saved;
          ਵਾਪਸ (ਅੱਖਰ *) ਮੇਲ;
        }

      ਜੇ (p < nul)
        {
          ਅੱਖਰ ਅਚਲ *ਮੇਲ = ਲੜੀ_ਖੋਜ (p, delim_ਬਾਈਟਾਂ);
          ਜੇ (ਮੇਲ)
            {
              buf[len] = saved;
              ਵਾਪਸ (ਅੱਖਰ *) ਮੇਲ;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  ਵਾਪਸ ਨਿਰਾ;
#ਜੇ_ਅੰਤ
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਅੱਖਰ *
ਲੱਭੋ_c_blank (ਅੱਖਰ *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  ਲਈ (idx_t i = 0; i < n; i++)
    ਜੇ (c_isblank (buf[i]))
      ਵਾਪਸ buf + i;

  ਵਾਪਸ memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ idx_t
ਖੇਤਰ_delim_overlap (ਅੱਖਰ ਅਚਲ *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  ਜਦੋਂ (0 < overlap)
    {
      ਜੇ (memeq (buf + len - overlap, delim_ਬਾਈਟਾਂ, overlap))
        ਵਾਪਸ overlap;
      overlap--;
    }

  ਵਾਪਸ 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ ਖੇਤਰ_terminator
ਲੱਭੋ_ਖੇਤਰ_terminator (ਅੱਖਰ *buf, idx_t len,
                       ਬਣਤਰ bytesearch_context *ctx, ਅੱਖਰ **terminator)
{
  ਜੇ (ctx->ਬਹੁਲਕ_2 == BYTESEARCH_ਸਤਰ_ONLY)
    {
      ਜੇ (!ctx->ਸਤਰ_ਸਿਰਾ_ਗਿਆਤ)
        {
          ctx->ਸਤਰ_ਸਿਰਾ = memchr (buf, ਸਤਰ_delim, len);
          ctx->ਸਤਰ_ਸਿਰਾ_ਗਿਆਤ = true;
        }

      *terminator = ctx->ਸਤਰ_ਸਿਰਾ;
      ਵਾਪਸ *terminator ? ਖੇਤਰ_ਸਤਰ_DELIMITER : ਖੇਤਰ_ਅੰਕੜੇ;
    }

  ਜੇ (ਖੇਤਰ_delim_is_ਸਤਰ_delim ())
    {
      *terminator = memchr (buf, ਸਤਰ_delim, len);
      ਵਾਪਸ *terminator ? ਖੇਤਰ_DELIMITER : ਖੇਤਰ_ਅੰਕੜੇ;
    }

  ਜੇ (!ctx->ਸਤਰ_ਸਿਰਾ_ਗਿਆਤ)
    {
      ctx->ਸਤਰ_ਸਿਰਾ = memchr (buf, ਸਤਰ_delim, len);
      ctx->ਸਤਰ_ਸਿਰਾ_ਗਿਆਤ = true;
    }

  idx_t ਖੇਤਰ_len = ctx->ਸਤਰ_ਸਿਰਾ ? ctx->ਸਤਰ_ਸਿਰਾ - buf : len;

  ਅੱਖਰ *ਖੇਤਰ_ਸਿਰਾ = (ctx->blank_delimited
                     ? ਲੱਭੋ_c_blank (buf, ਖੇਤਰ_len)
                     : ਲੱਭੋ_ਖੇਤਰ_delim (buf, ਖੇਤਰ_len));

  ਜੇ (ਖੇਤਰ_ਸਿਰਾ)
    {
      *terminator = ਖੇਤਰ_ਸਿਰਾ;
      ਵਾਪਸ ਖੇਤਰ_DELIMITER;
    }

  *terminator = ctx->ਸਤਰ_ਸਿਰਾ;
  ਵਾਪਸ ctx->ਸਤਰ_ਸਿਰਾ ? ਖੇਤਰ_ਸਤਰ_DELIMITER : ਖੇਤਰ_ਅੰਕੜੇ;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
maybe_write_ਸਤਰ_delim (bool ਮਿਲਿਆ_any_selected_ਖੇਤਰ, uintmax_t ਖੇਤਰ_idx)
{
  ਜੇ (ਮਿਲਿਆ_any_selected_ਖੇਤਰ
      || !(suppress_non_delimited && ਖੇਤਰ_idx == 1))
    write_ਸਤਰ_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
begin_ਖੇਤਰ_output (uintmax_t ਖੇਤਰ_idx, bool ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                    bool *ਮਿਲਿਆ_any_selected_ਖੇਤਰ)
{
  bool write_ਖੇਤਰ = print_kth (ਖੇਤਰ_idx);

  ਜੇ (write_ਖੇਤਰ && ! (ਖੇਤਰ_idx == 1 && ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ))
    {
      ਜੇ (*ਮਿਲਿਆ_any_selected_ਖੇਤਰ)
        write_ਬਾਈਟਾਂ (output_delimiter_string, output_delimiter_ਲੰਮਾਈ);
      *ਮਿਲਿਆ_any_selected_ਖੇਤਰ = true;
    }

  ਵਾਪਸ write_ਖੇਤਰ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
handle_ਖੇਤਰ_1 (uintmax_t ਖੇਤਰ_idx, bool ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                idx_t *ਖੇਤਰ_1_n_ਬਾਈਟਾਂ)
{
  bool ਖੇਤਰ_1_selected = false;

  ਜੇ (ਖੇਤਰ_idx == 1 && ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ)
    {
      ਜੇ ((ਖੇਤਰ_1_selected = print_kth (1)))
        write_ਬਾਈਟਾਂ (ਖੇਤਰ_1_ਬਫਰ, *ਖੇਤਰ_1_n_ਬਾਈਟਾਂ);
      *ਖੇਤਰ_1_n_ਬਾਈਟਾਂ = 0;
    }

  ਵਾਪਸ ਖੇਤਰ_1_selected;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
handle_ਖੇਤਰ_delimiter (uintmax_t *ਖੇਤਰ_idx, bool ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                        idx_t *ਖੇਤਰ_1_n_ਬਾਈਟਾਂ,
                        bool *ਮਿਲਿਆ_any_selected_ਖੇਤਰ, bool *write_ਖੇਤਰ)
{
  ਜੇ (handle_ਖੇਤਰ_1 (*ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ, ਖੇਤਰ_1_n_ਬਾਈਟਾਂ))
    *ਮਿਲਿਆ_any_selected_ਖੇਤਰ = true;

  ਅਗਲਾ_ਵਸਤੂ (ਖੇਤਰ_idx);
  *write_ਖੇਤਰ = begin_ਖੇਤਰ_output (*ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                                     ਮਿਲਿਆ_any_selected_ਖੇਤਰ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ਖੇਤਰ_selection_exhausted (uintmax_t ਖੇਤਰ_idx)
{
  ਵਾਪਸ !print_kth (ਖੇਤਰ_idx) && current_rp->lo == UINTMAX_MAX;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਅੰਤ_current_ਸਤਰ (uintmax_t ਖੇਤਰ_idx, bool ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                     idx_t *ਖੇਤਰ_1_n_ਬਾਈਟਾਂ,
                     bool ਮਿਲਿਆ_any_selected_ਖੇਤਰ, bool ਸਤਰ_terminated)
{
  ਜੇ (ਖੇਤਰ_idx == 1 && ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ)
    {
      ਜੇ (!suppress_non_delimited
          && (ਸਤਰ_terminated || *ਖੇਤਰ_1_n_ਬਾਈਟਾਂ != 0))
        {
          write_ਬਾਈਟਾਂ (ਖੇਤਰ_1_ਬਫਰ, *ਖੇਤਰ_1_n_ਬਾਈਟਾਂ);
          write_ਸਤਰ_delim ();
        }
      *ਖੇਤਰ_1_n_ਬਾਈਟਾਂ = 0;
    }
  ਨਹੀਂਤਾਂ
    maybe_write_ਸਤਰ_delim (ਮਿਲਿਆ_any_selected_ਖੇਤਰ, ਖੇਤਰ_idx);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
sync_byte_selection (uintmax_t byte_idx)
{
  ਜਦੋਂ (current_rp->hi <= byte_idx)
    current_rp++;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
reset_ਖੇਤਰ_ਸਤਰ (uintmax_t *ਖੇਤਰ_idx, bool *ਮਿਲਿਆ_any_selected_ਖੇਤਰ,
                  bool *have_pending_ਸਤਰ, ਬਣਤਰ mbfield_parser *parser)
{
  *ਖੇਤਰ_idx = 1;
  current_rp = frp;
  *ਮਿਲਿਆ_any_selected_ਖੇਤਰ = false;
  *have_pending_ਸਤਰ = false;
  parser->have_saved = false;
  parser->at_ਸਤਰ_ਸ਼ੁਰੂ = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

ਸਥਿਰ ਖਾਲੀ
cut_ਬਾਈਟਾਂ (FILE *ਧਾਰਾ)
{
  ਸਥਿਰ ਅੱਖਰ ਬਾਈਟਾਂ_ਵਿੱਚ[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  ਪੂਰਨ fd = fileno (ਧਾਰਾ);

  current_rp = frp;

  ਜਦੋਂ (true)
    {
      idx_t ਉਪਲਬਧ = ਪਠ (fd, ਬਾਈਟਾਂ_ਵਿੱਚ, ਆਕਾਰ ਬਾਈਟਾਂ_ਵਿੱਚ);
      ਜੇ (ਉਪਲਬਧ <= 0)
        {
          ਜੇ (ਉਪਲਬਧ < 0)
            fseterr (ਧਾਰਾ);
          write_pending_ਸਤਰ_delim (byte_idx);
          ਰੋਕੋ;
        }

      idx_t processed = 0;

      ਜਦੋਂ (processed < ਉਪਲਬਧ)
        {
          ਅੱਖਰ *ਸਤਰ = ਬਾਈਟਾਂ_ਵਿੱਚ + processed;
          idx_t ਮੰਜ਼ਿਲ_process = ਉਪਲਬਧ - processed;
          ਅੱਖਰ *ਸਤਰ_ਸਿਰਾ = ਖੋਜ_ਬਾਈਟਾਂ (ਸਤਰ, ਸਤਰ_delim, ਮੰਜ਼ਿਲ_process);
          ਅੱਖਰ *ਸਿਰਾ = ਸਤਰ + (ਸਤਰ_ਸਿਰਾ ? ਸਤਰ_ਸਿਰਾ - ਸਤਰ : ਮੰਜ਼ਿਲ_process);
          ਅੱਖਰ *p = ਸਤਰ;

          ਜਦੋਂ (0 < ਸਿਰਾ - p)
            {
              sync_byte_selection (byte_idx);

              ਜੇ (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (ਸਿਰਾ - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              ਨਹੀਂਤਾਂ
                {
                  idx_t n = MIN (ਸਿਰਾ - p, current_rp->hi - byte_idx);
                  write_selected_ਵਸਤੂ (&print_delimiter,
                                       is_range_ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += ਸਿਰਾ - ਸਤਰ;
          ਜੇ (ਸਤਰ_ਸਿਰਾ)
            {
              processed++;
              reset_ਵਸਤੂ_ਸਤਰ (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

ਸਥਿਰ ਖਾਲੀ
cut_characters_ਬਹੁਲਕ (FILE *ਧਾਰਾ, bool byte_ਬਹੁਲਕ)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  ਸਥਿਰ ਅੱਖਰ ਬਾਈਟਾਂ_ਵਿੱਚ[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, ਬਾਈਟਾਂ_ਵਿੱਚ, ਆਕਾਰ ਬਾਈਟਾਂ_ਵਿੱਚ, ਧਾਰਾ);

  ਜਦੋਂ (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      ਜੇ (idx && ਖੇਤਰ_selection_exhausted (idx))
        {
          idx_t ਉਪਲਬਧ = mbbuf_topup (&mbbuf);
          ਅੱਖਰ *p = mbbuf.ਬਫਰ + mbbuf.ਵਿੱਥ;
          ਅੱਖਰ *ਸਿਰਾ = ਖੋਜ_ਬਾਈਟਾਂ (p, ਸਤਰ_delim, ਉਪਲਬਧ);
          mbbuf_advance (&mbbuf, ਸਿਰਾ ? ਸਿਰਾ - p : ਉਪਲਬਧ);
          ਜੇ (!ਸਿਰਾ && ਉਪਲਬਧ)
            ਜਾਰੀ;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      ਜੇ (!byte_ਬਹੁਲਕ && utf8 && idx + 1 < current_rp->lo)
        {
          ਅੱਖਰ ਅਚਲ *p = mbbuf.ਬਫਰ + mbbuf.ਵਿੱਥ;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          ਅੱਖਰ ਅਚਲ *ਸਿਰਾ = ਖੋਜ_ਬਾਈਟਾਂ (p, ਸਤਰ_delim, n);
          ਜੇ (ਸਿਰਾ)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, ਸਿਰਾ - p + 1);
              reset_ਵਸਤੂ_ਸਤਰ (&idx, &print_delimiter);
              ਜਾਰੀ;
            }

          idx += u8_ਗਿਣਤੀ (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      ਜੇ (g.ch == ਸਤਰ_delim)
        reset_ਵਸਤੂ_ਸਤਰ (&idx, &print_delimiter);
      ਨਹੀਂਤਾਂ ਜੇ (g.ch == MBBUF_EOF)
        {
          write_pending_ਸਤਰ_delim (idx);
          ਰੋਕੋ;
        }
      ਨਹੀਂਤਾਂ ਜੇ (byte_ਬਹੁਲਕ)
        {
          bool ਪਹਿਲਾ_selected_is_range_ਸ਼ੁਰੂ = false;
          bool ਵੇਖਿਆ_selected = false;
          bool suffix_selected = true;

          ਲਈ (idx_t i = 0; i < g.len; i++)
            {
              ਅਗਲਾ_ਵਸਤੂ (&idx);
              ਜੇ (print_kth (idx))
                {
                  ਜੇ (!ਵੇਖਿਆ_selected)
                    {
                      ਵੇਖਿਆ_selected = true;
                      ਪਹਿਲਾ_selected_is_range_ਸ਼ੁਰੂ
                        = is_range_ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ (idx);
                    }
                }
              ਨਹੀਂਤਾਂ ਜੇ (ਵੇਖਿਆ_selected)
                suffix_selected = false;
            }

          ਜੇ (ਵੇਖਿਆ_selected && suffix_selected)
            write_selected_ਵਸਤੂ (&print_delimiter,ਪਹਿਲਾ_selected_is_range_ਸ਼ੁਰੂ,
                                 mbbuf_char_ਵਿੱਥ (&mbbuf, g), g.len);
        }
      ਨਹੀਂਤਾਂ
        {
          ਅਗਲਾ_ਵਸਤੂ (&idx);
          ਜੇ (print_kth (idx))
            write_selected_ਵਸਤੂ (&print_delimiter,
                                 is_range_ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ (idx),
                                 mbbuf_char_ਵਿੱਥ (&mbbuf, g), g.len);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
cut_ਬਾਈਟਾਂ_no_ਵੰਡੋ (FILE *ਧਾਰਾ)
{
  cut_characters_ਬਹੁਲਕ (ਧਾਰਾ, true);
}

ਸਥਿਰ ਖਾਲੀ
cut_characters (FILE *ਧਾਰਾ)
{
  cut_characters_ਬਹੁਲਕ (ਧਾਰਾ, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

ਸਥਿਰ ਖਾਲੀ
cut_fields_mb_any (FILE *ਧਾਰਾ, bool whitespace_ਬਹੁਲਕ)
{
  ਸਥਿਰ ਅੱਖਰ ਬਾਈਟਾਂ_ਵਿੱਚ[IO_BUFSIZE];
  mbbuf_t mbbuf;
  ਬਣਤਰ mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_ਬਹੁਲਕ,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_ਸਤਰ_ਸ਼ੁਰੂ = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t ਖੇਤਰ_idx = 1;
  bool ਮਿਲਿਆ_any_selected_ਖੇਤਰ = false;
  bool ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ;
  bool have_pending_ਸਤਰ = false;
  bool write_ਖੇਤਰ;
  idx_t ਖੇਤਰ_1_n_ਬਾਈਟਾਂ = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, ਬਾਈਟਾਂ_ਵਿੱਚ, ਆਕਾਰ ਬਾਈਟਾਂ_ਵਿੱਚ, ਧਾਰਾ);

  ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ = (suppress_non_delimited ^ !print_kth (1));
  write_ਖੇਤਰ = begin_ਖੇਤਰ_output (ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                                    &ਮਿਲਿਆ_any_selected_ਖੇਤਰ);

  ਜਦੋਂ (true)
    {
      ਗਿਣਤੀ ਖੇਤਰ_terminator terminator;
      ਜੇ (!ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ || ਖੇਤਰ_idx != 1)
        terminator = ਜਾਂਚ_mb_ਖੇਤਰ (&mbbuf, &parser, &have_pending_ਸਤਰ,
                                    write_ਖੇਤਰ, ਨਿਰਾ);
      ਨਹੀਂਤਾਂ
        terminator = ਜਾਂਚ_mb_ਖੇਤਰ (&mbbuf, &parser, &have_pending_ਸਤਰ,
                                    false, &ਖੇਤਰ_1_n_ਬਾਈਟਾਂ);

      ਜੇ (terminator == ਖੇਤਰ_DELIMITER)
        {
          handle_ਖੇਤਰ_delimiter (&ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                                  &ਖੇਤਰ_1_n_ਬਾਈਟਾਂ,
                                  &ਮਿਲਿਆ_any_selected_ਖੇਤਰ, &write_ਖੇਤਰ);
          ਜੇ (! ਖੇਤਰ_selection_exhausted (ਖੇਤਰ_idx))
            ਜਾਰੀ;

          terminator = ਜਾਂਚ_mb_ਸਤਰ_ਸਿਰਾ (&mbbuf, &parser,
                                         &have_pending_ਸਤਰ);
        }

      ਜੇ (terminator == ਖੇਤਰ_EOF && !have_pending_ਸਤਰ)
        ਰੋਕੋ;

      ਅੰਤ_current_ਸਤਰ (ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                           &ਖੇਤਰ_1_n_ਬਾਈਟਾਂ, ਮਿਲਿਆ_any_selected_ਖੇਤਰ,
                           terminator == ਖੇਤਰ_ਸਤਰ_DELIMITER);
      ਜੇ (terminator == ਖੇਤਰ_EOF)
        ਰੋਕੋ;

      reset_ਖੇਤਰ_ਸਤਰ (&ਖੇਤਰ_idx, &ਮਿਲਿਆ_any_selected_ਖੇਤਰ,
                        &have_pending_ਸਤਰ, &parser);
      write_ਖੇਤਰ = begin_ਖੇਤਰ_output (ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                                        &ਮਿਲਿਆ_any_selected_ਖੇਤਰ);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

ਸਥਿਰ ਖਾਲੀ
cut_fields_mb (FILE *ਧਾਰਾ)
{
  cut_fields_mb_any (ਧਾਰਾ, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

ਸਥਿਰ ਖਾਲੀ
cut_fields_bytesearch (FILE *ਧਾਰਾ)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  ਸਥਿਰ ਅੱਖਰ ਬਾਈਟਾਂ_ਵਿੱਚ[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t ਖੇਤਰ_idx = 1;
  bool ਮਿਲਿਆ_any_selected_ਖੇਤਰ = false;
  bool have_pending_ਸਤਰ = false;
  bool skip_blank_ਲੜੀਵਾਰ = false;
  bool pending_ਸਤਰ_delim_ਖੇਤਰ = false;
  bool write_ਖੇਤਰ;
  idx_t ਖੇਤਰ_1_n_ਬਾਈਟਾਂ = 0;

  current_rp = frp;
  bool ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, ਬਾਈਟਾਂ_ਵਿੱਚ, ਆਕਾਰ ਬਾਈਟਾਂ_ਵਿੱਚ - 1, ਧਾਰਾ);
  write_ਖੇਤਰ = begin_ਖੇਤਰ_output (ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                                    &ਮਿਲਿਆ_any_selected_ਖੇਤਰ);

  ਬਣਤਰ bytesearch_context ਖੋਜ = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&ਖੋਜ);

  ਜਦੋਂ (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      ਖੋਜ.at_eof = mbbuf.eof;
      ਖੋਜ.ਸਤਰ_ਸਿਰਾ_ਗਿਆਤ = false;

      ਜੇ (pending_ਸਤਰ_delim_ਖੇਤਰ)
        {
          pending_ਸਤਰ_delim_ਖੇਤਰ = false;

          ਜੇ (n_avail == 0)
            {
              ਜੇ (handle_ਖੇਤਰ_1 (ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                                  &ਖੇਤਰ_1_n_ਬਾਈਟਾਂ))
                ਅਗਲਾ_ਵਸਤੂ (&ਖੇਤਰ_idx);
              maybe_write_ਸਤਰ_delim (ਮਿਲਿਆ_any_selected_ਖੇਤਰ, ਖੇਤਰ_idx);
              ਵਾਪਸ;
            }

          handle_ਖੇਤਰ_delimiter (&ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                                  &ਖੇਤਰ_1_n_ਬਾਈਟਾਂ,
                                  &ਮਿਲਿਆ_any_selected_ਖੇਤਰ, &write_ਖੇਤਰ);
          ਜੇ (whitespace_delimited)
            skip_blank_ਲੜੀਵਾਰ = true;
        }

      ਜੇ (n_avail == 0)
        ਰੋਕੋ;

      ਅੱਖਰ *chunk = mbbuf.ਬਫਰ + mbbuf.ਵਿੱਥ;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      ਜੇ (ਖੇਤਰ_idx == 1
          && (!whitespace_delimited || !ਖੇਤਰ_selection_exhausted (ਖੇਤਰ_idx))
          && !ਖੇਤਰ_delim_is_ਸਤਰ_delim ()
          && !(whitespace_delimited
               ? ਲੱਭੋ_c_blank (chunk, n_avail)
               : ਲੱਭੋ_ਖੇਤਰ_delim (chunk, n_avail)))
        {
          ਅੱਖਰ *ਆਖਰੀ_ਸਤਰ_delim = ਖੋਜ.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, ਸਤਰ_delim, n_avail);
          ਜੇ (ਆਖਰੀ_ਸਤਰ_delim)
            {
              idx_t n = ਆਖਰੀ_ਸਤਰ_delim - chunk + 1;
              ਜੇ (! suppress_non_delimited)
                {
                  write_ਬਾਈਟਾਂ (ਖੇਤਰ_1_ਬਫਰ, ਖੇਤਰ_1_n_ਬਾਈਟਾਂ);
                  write_ਬਾਈਟਾਂ (chunk, n);
                  ਜੇ (ਖੋਜ.at_eof && chunk[n - 1] != ਸਤਰ_delim)
                    write_ਸਤਰ_delim ();
                }
              ਖੇਤਰ_1_n_ਬਾਈਟਾਂ = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_ਸਤਰ = false;
              ਜੇ (ਖੋਜ.at_eof)
                ਵਾਪਸ;
              ਜਾਰੀ;
            }
        }

      ਜਦੋਂ (processed < n_avail)
        {
          ਅੱਖਰ *terminator = ਨਿਰਾ;

          ਜੇ (skip_blank_ਲੜੀਵਾਰ)
            {
              ਜਦੋਂ (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              ਜੇ (processed == n_avail)
                ਰੋਕੋ;
              skip_blank_ਲੜੀਵਾਰ = false;
            }

          ਜੇ (ਖੇਤਰ_selection_exhausted (ਖੇਤਰ_idx))
            {
              ਜੇ (ਖੇਤਰ_delim_is_ਸਤਰ_delim ())
                {
                  maybe_write_ਸਤਰ_delim (ਮਿਲਿਆ_any_selected_ਖੇਤਰ,
                                          ਖੇਤਰ_idx);
                  mbbuf_advance (&mbbuf, processed);
                  ਵਾਪਸ;
                }
              ਖੋਜ.ਬਹੁਲਕ_2 = BYTESEARCH_ਸਤਰ_ONLY;
              ਗਿਣਤੀ ਖੇਤਰ_terminator terminator_ਕਿਸਮ
                = ਲੱਭੋ_ਖੇਤਰ_terminator (chunk + processed, n_avail - processed,
                                         &ਖੋਜ, &terminator);
              ਜੇ (terminator_ਕਿਸਮ == ਖੇਤਰ_ਸਤਰ_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_ਸਤਰ_delim (ਮਿਲਿਆ_any_selected_ਖੇਤਰ,
                                          ਖੇਤਰ_idx);
                  ਜਾਓ reset_ਸਤਰ;
                }
              processed = n_avail;
              ਰੋਕੋ;
            }

          ਗਿਣਤੀ ਖੇਤਰ_terminator terminator_ਕਿਸਮ
            = ਲੱਭੋ_ਖੇਤਰ_terminator (chunk + processed, n_avail - processed,
                                     &ਖੋਜ, &terminator);
          idx_t ਖੇਤਰ_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          ਜੇ (terminator_ਕਿਸਮ == ਖੇਤਰ_ਅੰਕੜੇ
              && !ਖੋਜ.at_eof
              && !whitespace_delimited
              && !ਖੇਤਰ_delim_is_ਸਤਰ_delim ())
            ਖੇਤਰ_len -= ਖੇਤਰ_delim_overlap (chunk + processed, ਖੇਤਰ_len);

          ਜੇ (ਖੇਤਰ_len || terminator)
            have_pending_ਸਤਰ = true;

          /* Output field content.  */
          ਜੇ (ਖੇਤਰ_idx == 1 && ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ)
            append_ਖੇਤਰ_1_chunk (chunk + processed, ਖੇਤਰ_len,
                                  &ਖੇਤਰ_1_n_ਬਾਈਟਾਂ);
          ਨਹੀਂਤਾਂ ਜੇ (write_ਖੇਤਰ)
            write_ਬਾਈਟਾਂ (chunk + processed, ਖੇਤਰ_len);
          processed += ਖੇਤਰ_len;

          ਜੇ (terminator_ਕਿਸਮ == ਖੇਤਰ_ਅੰਕੜੇ)
            ਰੋਕੋ;

          ਜੇ (terminator_ਕਿਸਮ == ਖੇਤਰ_DELIMITER)
            {
              ਜੇ (ਖੇਤਰ_delim_is_ਸਤਰ_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_ਸਤਰ_delim_ਖੇਤਰ = true;
                  ਰੋਕੋ;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_ਖੇਤਰ_delimiter (&ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                                      &ਖੇਤਰ_1_n_ਬਾਈਟਾਂ,
                                      &ਮਿਲਿਆ_any_selected_ਖੇਤਰ, &write_ਖੇਤਰ);
              ਜੇ (whitespace_delimited)
                skip_blank_ਲੜੀਵਾਰ = true;
            }
          ਨਹੀਂਤਾਂ
            {
              affirm (terminator_ਕਿਸਮ == ਖੇਤਰ_ਸਤਰ_DELIMITER);
              processed++;
              ਅੰਤ_current_ਸਤਰ (ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                                   &ਖੇਤਰ_1_n_ਬਾਈਟਾਂ,
                                   ਮਿਲਿਆ_any_selected_ਖੇਤਰ, true);

            reset_ਸਤਰ:
              ਖੇਤਰ_idx = 1;
              current_rp = frp;
              ਮਿਲਿਆ_any_selected_ਖੇਤਰ = false;
              have_pending_ਸਤਰ = false;
              bytesearch_context_reset (&ਖੋਜ);
              write_ਖੇਤਰ = begin_ਖੇਤਰ_output (ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ,
                                                &ਮਿਲਿਆ_any_selected_ਖੇਤਰ);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  ਜੇ (!have_pending_ਸਤਰ)
    ਵਾਪਸ;

  ਅੰਤ_current_ਸਤਰ (ਖੇਤਰ_idx, ਬਫਰ_ਪਹਿਲਾ_ਖੇਤਰ, &ਖੇਤਰ_1_n_ਬਾਈਟਾਂ,
                       ਮਿਲਿਆ_any_selected_ਖੇਤਰ, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

ਸਥਿਰ ਖਾਲੀ
cut_fields_ws (FILE *ਧਾਰਾ)
{
  ਜੇ (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (ਧਾਰਾ);
  ਨਹੀਂਤਾਂ
    cut_fields_mb_any (ਧਾਰਾ, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

ਸਥਿਰ bool
cut_file (ਅੱਖਰ ਅਚਲ *file, ਖਾਲੀ (*cut_ਧਾਰਾ) (FILE *))
{
  FILE *ਧਾਰਾ;

  ਜੇ (streq (file, "-"))
    {
      have_read_stdin = true;
      ਧਾਰਾ = stdin;
      assume (ਧਾਰਾ);  /* Pacify GCC bug#109613.  */
    }
  ਨਹੀਂਤਾਂ
    {
      ਧਾਰਾ = fopen (file, "r");
      ਜੇ (ਧਾਰਾ == ਨਿਰਾ)
        {
          error (0, errno, "%s", quotef (file));
          ਵਾਪਸ false;
        }
    }

  fadvise (ਧਾਰਾ, FADVISE_SEQUENTIAL);

  cut_ਧਾਰਾ (ਧਾਰਾ);

  ਪੂਰਨ err = errno;
  ਜੇ (!ferror (ਧਾਰਾ))
    err = 0;
  ਜੇ (streq (file, "-"))
    clearerr (ਧਾਰਾ);		/* Also clear EOF.  */
  ਨਹੀਂਤਾਂ ਜੇ (fclose (ਧਾਰਾ) == EOF)
    err = errno;
  ਜੇ (err)
    {
      error (0, err, "%s", quotef (file));
      ਵਾਪਸ false;
    }
  ਵਾਪਸ true;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  ਅੱਖਰ *spec_list_string = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'b':
          cut_ਬਹੁਲਕ = CUT_ਬਹੁਲਕ_ਬਾਈਟਾਂ;
          FALLTHROUGH;
        ਹਾਲਤ 'c':
          ਜੇ (optc == 'c')
            cut_ਬਹੁਲਕ = CUT_ਬਹੁਲਕ_CHARACTERS;
          FALLTHROUGH;
        ਹਾਲਤ 'f':
        ਹਾਲਤ 'F':
          ਜੇ (optc == 'f' || optc == 'F')
            cut_ਬਹੁਲਕ = CUT_ਬਹੁਲਕ_FIELDS;
          ਜੇ (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          ਜੇ (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            ਜੇ (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_ਬਾਈਟਾਂ (delim_ਬਾਈਟਾਂ, optarg, g.len + 1);
            delim_specified = true;
            ਰੋਕੋ;
          }

        ਹਾਲਤ 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_ਦਲੀਲਾਂ,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          ਰੋਕੋ;

        ਹਾਲਤ 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_ਲੰਮਾਈ = (optarg[0] == '\0'
                                     ? 1 : ਲੜੀ_ਲੰਬਾਈ (optarg));
          output_delimiter_string = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'n':
          no_ਵੰਡੋ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          suppress_non_delimited = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'z':
          ਸਤਰ_delim = '\0';
          ਰੋਕੋ;

        ਹਾਲਤ COMPLEMENT_OPTION:
          complement = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  ਜੇ (cut_ਬਹੁਲਕ == CUT_ਬਹੁਲਕ_ਬਾਈਟਾਂ || cut_ਬਹੁਲਕ == CUT_ਬਹੁਲਕ_CHARACTERS)
    {
      ਜੇ (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an ਨਿਵੇਸ਼ delimiter makes sense\n\
\tonly when operating on fields"));

      ਜੇ (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited ਸਤਰਾਂ makes sense\n\
\tonly when operating on fields"));
    }

  ਜੇ (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  ਜੇ (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  ਸੈੱਟ_fields (spec_list_string,
              (((cut_ਬਹੁਲਕ == CUT_ਬਹੁਲਕ_ਬਾਈਟਾਂ
                 || cut_ਬਹੁਲਕ == CUT_ਬਹੁਲਕ_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  ਜੇ (!delim_specified)
    {
      delim_ਬਾਈਟਾਂ[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  ਜੇ (output_delimiter_string == ਨਿਰਾ)
    {
      output_delimiter_string = output_delimiter_default;
      ਜੇ (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_ਲੰਮਾਈ = 1;
        }
      ਨਹੀਂਤਾਂ
        {
          copy_ਬਾਈਟਾਂ (output_delimiter_default, delim_ਬਾਈਟਾਂ, delim_mcel.len);
          output_delimiter_ਲੰਮਾਈ = delim_mcel.len;
        }
    }

  ਖਾਲੀ (*cut_ਧਾਰਾ) (FILE *) = ਨਿਰਾ;
  ਚੋਣ (cut_ਬਹੁਲਕ)
    {
    ਹਾਲਤ CUT_ਬਹੁਲਕ_ਬਾਈਟਾਂ:
      cut_ਧਾਰਾ = MB_CUR_MAX <= 1 || !no_ਵੰਡੋ
                   ? cut_ਬਾਈਟਾਂ : cut_ਬਾਈਟਾਂ_no_ਵੰਡੋ;
      ਰੋਕੋ;

    ਹਾਲਤ CUT_ਬਹੁਲਕ_CHARACTERS:
      cut_ਧਾਰਾ = MB_CUR_MAX <= 1 ? cut_ਬਾਈਟਾਂ : cut_characters;
      ਰੋਕੋ;

    ਹਾਲਤ CUT_ਬਹੁਲਕ_FIELDS:
      cut_ਧਾਰਾ = whitespace_delimited ? cut_fields_ws
                   : bytesearch_ਖੇਤਰ_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      ਰੋਕੋ;

    ਹਾਲਤ CUT_ਬਹੁਲਕ_NONE:
      affirm (false);
    }
  affirm (cut_ਧਾਰਾ);
  ਜੇ (optind == argc)
    ok = cut_file ("-", cut_ਧਾਰਾ);
  ਨਹੀਂਤਾਂ
    ਲਈ (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_ਧਾਰਾ);


  ਜੇ (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
