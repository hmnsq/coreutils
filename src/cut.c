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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "cut"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#ವ್ಯಾಖ್ಯಾನಿಸು FATAL_ERROR(ಸಂದೇಶ)						\
  ಮಾಡು									\
    {									\
      error (0, 0, (ಸಂದೇಶ));						\
      usage (EXIT_FAILURE);						\
    }									\
  ಆಗ (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#ವ್ಯಾಖ್ಯಾನಿಸು SMALL_BYTE_ಮಿತಿ 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
ಸ್ಥಿರ ರಚನೆ ಕ್ಷೇತ್ರ_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
ಸ್ಥಿರ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_1_ಬಫರ್;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
ಸ್ಥಿರ idx_t ಕ್ಷೇತ್ರ_1_bufsize;

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
ಸ್ಥಿರ ಅಕ್ಷರ delim_ಬೈಟುಗಳು[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸಾಲು_delim = '\n';

/* The length of output_delimiter_string.  */
ಸ್ಥಿರ size_t output_delimiter_ಉದ್ದಳತೆ;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
ಸ್ಥಿರ ಅಕ್ಷರ *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
ಸ್ಥಿರ ಅಕ್ಷರ output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
ಸ್ಥಿರ bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
ಸ್ಥಿರ bool no_ವಿಭಜಿಸು;

/* If true, interpret each run of whitespace as one field delimiter.  */
ಸ್ಥಿರ bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
ಸ್ಥಿರ bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
ಸ್ಥಿರ bool space_output_delimiter_default;

ಎಣಿಕೆ whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ whitespace_option_ವಾದಗಳು[] =
{
  "trimmed", ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ whitespace_option ಸ್ಥಿರಾಂಕ whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_ವಾದಗಳು, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
ಸ್ಥಿರ ಎಣಿಕೆ
{
  CUT_ಬಹುಲಕ_NONE,
  CUT_ಬಹುಲಕ_ಬೈಟುಗಳು,
  CUT_ಬಹುಲಕ_CHARACTERS,
  CUT_ಬಹುಲಕ_FIELDS
} cut_ಬಹುಲಕ = CUT_ಬಹುಲಕ_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"bytes", required_ವಾದ, ಖಾಲಿ, 'b'},
  {"characters", required_ವಾದ, ಖಾಲಿ, 'c'},
  {"fields", required_ವಾದ, ಖಾಲಿ, 'f'},
  {"delimiter", required_ವಾದ, ಖಾಲಿ, 'd'},
  {"no-partial", no_ವಾದ, ಖಾಲಿ, 'n'},
  {"whitespace-delimited", optional_ವಾದ, ಖಾಲಿ, 'w'},
  {"only-delimited", no_ವಾದ, ಖಾಲಿ, 's'},
  {"output-delimiter", required_ವಾದ, ಖಾಲಿ, 'O'},
  {"complement", no_ವಾದ, ಖಾಲಿ, COMPLEMENT_OPTION},
  {"zero-terminated", no_ವಾದ, ಖಾಲಿ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print selected parts ದ ಸಾಲುಗಳು from each FILE ಗಮ್ಯ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --ಬೈಟುಗಳು=LIST\n\
         select only these byte positions\n\
"));
      oputs (_("\
  -c, --characters=LIST\n\
         select only these character positions\n\
"));
      oputs (_("\
      --complement\n\
         complement the ಗಣ_2 ದ selected ಬೈಟುಗಳು, characters or fields\n\
"));
      oputs (_("\
  -d, --delimiter=DELIM\n\
         use DELIM instead ದ TAB ಗಾಗಿ ಕ್ಷೇತ್ರ delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any ಸಾಲು that ಒಳಗೊಂಡಿದೆ\n\
         no delimiter character, unless the -s option is specified\n\
"));
      oputs (_("\
  -F LIST\n\
         like -f, but also ಸೂಚಿಸುತ್ತದೆ -w and -O ' '\n\
"));
      oputs (_("\
  -n, --no-partial\n\
         with -b, don't output partial multi-byte characters\n\
"));
      oputs (_("\
  -O, --output-delimiter=STRING\n\
         use STRING as the output delimiter;\n\
         the ಪೂರ್ವನಿಯೋಜಿತ is ಗಮ್ಯ use the ಒಳಹರಿವು delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         ಮಾಡು not print ಸಾಲುಗಳು not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a ಸತತ_2 ದ blank characters as the ಕ್ಷೇತ್ರ delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one ದ -b, -c, -f or -F.  Each LIST is made ಮೇಲೆ ದ one\n\
range, or many ranges separated by commas.  Selected ಒಳಹರಿವು is written\n\
ಒಳ the same ಕ್ರಮ that it is ಪಠ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one ದ:\n\
\n\
  N     N'th byte, character or ಕ್ಷೇತ್ರ, counted from 1\n\
  N-    from N'th byte, character or ಕ್ಷೇತ್ರ, ಗಮ್ಯ ಕೊನೆ ದ ಸಾಲು\n\
  N-M   from N'th to M'th (included) byte, character or ಕ್ಷೇತ್ರ\n\
  -M    from ಮೊದಲ ಗಮ್ಯ M'th (included) byte, character or ಕ್ಷೇತ್ರ\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
ಮುಂದಿನ_ಅಂಶ (uintmax_t *ಅಂಶ_idx)
{
  (*ಅಂಶ_idx)++;
  ಆದರೆ ((*ಅಂಶ_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
print_kth (uintmax_t k)
{
  ಹಿಂತಿರುಗಿ current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_range_ಆರಂಭ_ಸೂಚ್ಯಂಕ (uintmax_t k)
{
  ಹಿಂತಿರುಗಿ k == current_rp->lo;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ಕ್ಷೇತ್ರ_delim_is_ಸಾಲು_delim (ಶೂನ್ಯ)
{
  ಹಿಂತಿರುಗಿ delim_mcel.len == 1 && delim_ಬೈಟುಗಳು[0] == ಸಾಲು_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

ಸ್ಥಿರ bool
mcel_isblank (mcel_t g)
{
  ಹಿಂತಿರುಗಿ (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
bytesearch_ಕ್ಷೇತ್ರ_delim_ok (ಶೂನ್ಯ)
{
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ delim_0 = delim_ಬೈಟುಗಳು[0];

  ಹಿಂತಿರುಗಿ (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


ಎಣಿಕೆ ಕ್ಷೇತ್ರ_terminator
{
  ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ,
  ಕ್ಷೇತ್ರ_DELIMITER,
  ಕ್ಷೇತ್ರ_ಸಾಲು_DELIMITER,
  ಕ್ಷೇತ್ರ_EOF
};

ಎಣಿಕೆ bytesearch_ಬಹುಲಕ
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_ಸಾಲು_ONLY
};

ರಚನೆ bytesearch_context
{
  ಎಣಿಕೆ bytesearch_ಬಹುಲಕ ಬಹುಲಕ_2;
  bool blank_delimited;
  bool at_eof;
  ಅಕ್ಷರ *ಸಾಲು_ಕೊನೆ;
  bool ಸಾಲು_ಕೊನೆ_ತಿಳಿದ;
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
bytesearch_context_reset (ರಚನೆ bytesearch_context *ctx)
{
  ctx->ಬಹುಲಕ_2 = BYTESEARCH_FIELDS;
  ctx->ಸಾಲು_ಕೊನೆ = ಖಾಲಿ;
  ctx->ಸಾಲು_ಕೊನೆ_ತಿಳಿದ = false;
}

ರಚನೆ mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_ಸಾಲು_ಆರಂಭ;
  bool have_saved;
  mcel_t saved_g;
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  ಆದರೆ (*have_saved)
    {
      *have_saved = false;
      ಹಿಂತಿರುಗಿ *saved_g;
    }
  ಹಿಂತಿರುಗಿ mbbuf_get_char (mbbuf);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ ಕ್ಷೇತ್ರ_terminator
skip_whitespace_ಸತತ (mbbuf_t *mbuf, ರಚನೆ mbfield_parser *parser,
                     bool *have_pending_ಸಾಲು,
                     bool have_initial_whitespace)
{
  mcel_t g;

  ಮಾಡು
    {
      g = mbbuf_get_char (mbuf);
      ಆದರೆ (g.ch != MBBUF_EOF)
        *have_pending_ಸಾಲು = true;
    }
  ಆಗ (g.ch != MBBUF_EOF && g.ch != ಸಾಲು_delim && mcel_isblank (g));

  bool trim_ಆರಂಭ = parser->trim_outer_whitespace && parser->at_ಸಾಲು_ಆರಂಭ;

  ಆದರೆ (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == ಸಾಲು_delim))
    {
      ಹಿಂತಿರುಗಿ g.ch == MBBUF_EOF ? ಕ್ಷೇತ್ರ_EOF : ಕ್ಷೇತ್ರ_ಸಾಲು_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  ಹಿಂತಿರುಗಿ trim_ಆರಂಭ && !have_initial_whitespace ? ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ : ಕ್ಷೇತ್ರ_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

ಸ್ಥಿರ ಶೂನ್ಯ
write_ಬೈಟುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *buf, size_t n_ಬೈಟುಗಳು)
{
  ಆದರೆ (n_ಬೈಟುಗಳು <= SMALL_BYTE_ಮಿತಿ)
    {
      ಗಾಗಿ (size_t i = 0; i < n_ಬೈಟುಗಳು; i++)
        ಆದರೆ (ಅಕ್ಷರ_ಬರೆ (buf[i]) < 0)
          write_error ();
      ಹಿಂತಿರುಗಿ;
    }

  ಆದರೆ (fwrite (buf, ಗಾತ್ರ (ಅಕ್ಷರ), n_ಬೈಟುಗಳು, stdout) != n_ಬೈಟುಗಳು)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
copy_ಬೈಟುಗಳು (ಅಕ್ಷರ *dst, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src, size_t n_ಬೈಟುಗಳು)
{
  ಆದರೆ (n_ಬೈಟುಗಳು <= SMALL_BYTE_ಮಿತಿ)
    {
      ಗಾಗಿ (size_t i = 0; i < n_ಬೈಟುಗಳು; i++)
        dst[i] = src[i];
      ಹಿಂತಿರುಗಿ;
    }

  ಸ್ಮರಣೆ_ನಕಲು (dst, src, n_ಬೈಟುಗಳು);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ*
ಹುಡುಕು_ಬೈಟುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *buf, ಪೂರ್ಣಾಂಕ c, size_t n_ಬೈಟುಗಳು)
{
  ಆದರೆ (n_ಬೈಟುಗಳು != 0 && ಗಮ್ಯ_uchar (buf[0]) == c)
    ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) buf;
  ಆದರೆ (1 < n_ಬೈಟುಗಳು && ಗಮ್ಯ_uchar (buf[1]) == c)
    ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) buf + 1;

  ಹಿಂತಿರುಗಿ 2 < n_ಬೈಟುಗಳು ? (ಅಕ್ಷರ *) memchr (buf + 2, c, n_ಬೈಟುಗಳು - 2) : ಖಾಲಿ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
write_ಸಾಲು_delim (ಶೂನ್ಯ)
{
  ಆದರೆ (ಅಕ್ಷರ_ಬರೆ (ಸಾಲು_delim) < 0)
    write_error ();
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
reset_ಅಂಶ_ಸಾಲು (uintmax_t *ಅಂಶ_idx, bool *print_delimiter)
{
  write_ಸಾಲು_delim ();
  *ಅಂಶ_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
write_pending_ಸಾಲು_delim (uintmax_t ಅಂಶ_idx)
{
  ಆದರೆ (ಅಂಶ_idx > 0)
    write_ಸಾಲು_delim ();
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
write_selected_ಅಂಶ (bool *print_delimiter, bool range_ಆರಂಭ,
                     ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *buf, size_t n_ಬೈಟುಗಳು)
{
  ಆದರೆ (output_delimiter_string != output_delimiter_default)
    {
      ಆದರೆ (*print_delimiter && range_ಆರಂಭ)
        write_ಬೈಟುಗಳು (output_delimiter_string, output_delimiter_ಉದ್ದಳತೆ);
      *print_delimiter = true;
    }

  write_ಬೈಟುಗಳು (buf, n_ಬೈಟುಗಳು);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
append_ಕ್ಷೇತ್ರ_1_chunk (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *buf, idx_t len, idx_t *n_ಬೈಟುಗಳು)
{
  ಆದರೆ (ಕ್ಷೇತ್ರ_1_bufsize - *n_ಬೈಟುಗಳು < len)
    {
      ಕ್ಷೇತ್ರ_1_ಬಫರ್ = xpalloc (ಕ್ಷೇತ್ರ_1_ಬಫರ್, &ಕ್ಷೇತ್ರ_1_bufsize,
                                len, -1, ಗಾತ್ರ *ಕ್ಷೇತ್ರ_1_ಬಫರ್);
    }

  copy_ಬೈಟುಗಳು (ಕ್ಷೇತ್ರ_1_ಬಫರ್ + *n_ಬೈಟುಗಳು, buf, len);
  *n_ಬೈಟುಗಳು += len;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
append_ಕ್ಷೇತ್ರ_1_ಬೈಟುಗಳು (mbbuf_t *mbbuf, mcel_t g, idx_t *n_ಬೈಟುಗಳು)
{
  append_ಕ್ಷೇತ್ರ_1_chunk (mbbuf_char_ಸ್ಥಾನಾಂತರ (mbbuf, g), g.len, n_ಬೈಟುಗಳು);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ ಕ್ಷೇತ್ರ_terminator
ಪರಿಶೀಲನೆ_mb_blank_ಕ್ಷೇತ್ರ (mbbuf_t *mbbuf, ರಚನೆ mbfield_parser *parser,
                     bool *have_pending_ಸಾಲು, bool write_ಕ್ಷೇತ್ರ,
                     idx_t *n_ಬೈಟುಗಳು)
{
  ಆದರೆ (parser->trim_outer_whitespace && parser->at_ಸಾಲು_ಆರಂಭ)
    {
      ಎಣಿಕೆ ಕ್ಷೇತ್ರ_terminator terminator
        = skip_whitespace_ಸತತ (mbbuf, parser, have_pending_ಸಾಲು, false);
      ಆದರೆ (terminator != ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ)
        ಹಿಂತಿರುಗಿ terminator;
    }

  parser->at_ಸಾಲು_ಆರಂಭ = false;

  ಆಗ (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      ಆದರೆ (g.ch == MBBUF_EOF)
        ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_EOF;

      *have_pending_ಸಾಲು = true;

      ಆದರೆ (g.ch == ಸಾಲು_delim)
        ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_ಸಾಲು_DELIMITER;

      ಆದರೆ (mcel_isblank (g))
        ಹಿಂತಿರುಗಿ skip_whitespace_ಸತತ (mbbuf, parser, have_pending_ಸಾಲು, true);

      ಆದರೆ (n_ಬೈಟುಗಳು)
        append_ಕ್ಷೇತ್ರ_1_ಬೈಟುಗಳು (mbbuf, g, n_ಬೈಟುಗಳು);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (write_ಕ್ಷೇತ್ರ)
        write_ಬೈಟುಗಳು (mbbuf_char_ಸ್ಥಾನಾಂತರ (mbbuf, g), g.len);
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ ಕ್ಷೇತ್ರ_terminator
ಪರಿಶೀಲನೆ_mb_delim_ಕ್ಷೇತ್ರ (mbbuf_t *mbbuf, bool *have_pending_ಸಾಲು,
                     bool write_ಕ್ಷೇತ್ರ, idx_t *n_ಬೈಟುಗಳು)
{
  ಆಗ (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      ಆದರೆ (g.ch == MBBUF_EOF)
        ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_EOF;

      *have_pending_ಸಾಲು = true;

      ಆದರೆ (g.ch == ಸಾಲು_delim)
        ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_ಸಾಲು_DELIMITER;

      ಆದರೆ (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_DELIMITER;

      ಆದರೆ (n_ಬೈಟುಗಳು)
        append_ಕ್ಷೇತ್ರ_1_ಬೈಟುಗಳು (mbbuf, g, n_ಬೈಟುಗಳು);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (write_ಕ್ಷೇತ್ರ)
        write_ಬೈಟುಗಳು (mbbuf_char_ಸ್ಥಾನಾಂತರ (mbbuf, g), g.len);
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ ಕ್ಷೇತ್ರ_terminator
ಪರಿಶೀಲನೆ_mb_ಕ್ಷೇತ್ರ (mbbuf_t *mbbuf, ರಚನೆ mbfield_parser *parser,
               bool *have_pending_ಸಾಲು, bool write_ಕ್ಷೇತ್ರ, idx_t *n_ಬೈಟುಗಳು)
{
  ಹಿಂತಿರುಗಿ (parser->whitespace_delimited
          ? ಪರಿಶೀಲನೆ_mb_blank_ಕ್ಷೇತ್ರ (mbbuf, parser, have_pending_ಸಾಲು,
                                 write_ಕ್ಷೇತ್ರ, n_ಬೈಟುಗಳು)
          : ಪರಿಶೀಲನೆ_mb_delim_ಕ್ಷೇತ್ರ (mbbuf, have_pending_ಸಾಲು, write_ಕ್ಷೇತ್ರ,
                                 n_ಬೈಟುಗಳು));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

ಸ್ಥಿರ ಎಣಿಕೆ ಕ್ಷೇತ್ರ_terminator
ಪರಿಶೀಲನೆ_mb_ಸಾಲು_ಕೊನೆ (mbbuf_t *mbbuf, ರಚನೆ mbfield_parser *parser,
                  bool *have_pending_ಸಾಲು)
{
  ಆದರೆ (parser->have_saved)
    {
      parser->have_saved = false;
      ಆದರೆ (parser->saved_g.ch == MBBUF_EOF)
        ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_EOF;

      *have_pending_ಸಾಲು = true;
      ಆದರೆ (parser->saved_g.ch == ಸಾಲು_delim)
        ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_ಸಾಲು_DELIMITER;
    }

  ಆಗ (true)
    {
      idx_t ಲಭ್ಯ = mbbuf_topup (mbbuf);
      ಆದರೆ (ಲಭ್ಯ == 0)
        ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_EOF;

      ಅಕ್ಷರ *ಬಫರ್ = mbbuf->ಬಫರ್ + mbbuf->ಸ್ಥಾನಾಂತರ;
      ಅಕ್ಷರ *ಸಾಲು_ಕೊನೆ = ಹುಡುಕು_ಬೈಟುಗಳು (ಬಫರ್, ಸಾಲು_delim, ಲಭ್ಯ);
      idx_t n = ಸಾಲು_ಕೊನೆ ? ಸಾಲು_ಕೊನೆ - ಬಫರ್ + 1 : ಲಭ್ಯ;
      mbbuf_advance (mbbuf, n);
      *have_pending_ಸಾಲು = true;

      ಆದರೆ (ಸಾಲು_ಕೊನೆ)
        ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_ಸಾಲು_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಅಕ್ಷರ *
ಕಂಡುಹಿಡಿ_ಕ್ಷೇತ್ರ_delim (ಅಕ್ಷರ *buf, size_t len)
{
  ಆದರೆ (len < delim_mcel.len)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

#ಆದರೆ ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  ಹಿಂತಿರುಗಿ memmem (buf, len, delim_ಬೈಟುಗಳು, delim_mcel.len);
#ಇಲ್ಲದಿದ್ದರೆ
  ಆದರೆ (delim_mcel.len == 1)
    ಹಿಂತಿರುಗಿ memchr (buf, delim_ಬೈಟುಗಳು[0], len);

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = buf;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೊನೆ = buf + len;

  ಅಕ್ಷರ saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  ಆಗ (p < ಕೊನೆ)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *nul = memchr (p, '\0', ಕೊನೆ - p);
      ಆದರೆ (!nul)
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಹೊಂದಿಕೆ = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (p, delim_ಬೈಟುಗಳು);
          buf[len] = saved;
          ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) ಹೊಂದಿಕೆ;
        }

      ಆದರೆ (p < nul)
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಹೊಂದಿಕೆ = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (p, delim_ಬೈಟುಗಳು);
          ಆದರೆ (ಹೊಂದಿಕೆ)
            {
              buf[len] = saved;
              ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) ಹೊಂದಿಕೆ;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಅಕ್ಷರ *
ಕಂಡುಹಿಡಿ_c_blank (ಅಕ್ಷರ *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  ಗಾಗಿ (idx_t i = 0; i < n; i++)
    ಆದರೆ (c_isblank (buf[i]))
      ಹಿಂತಿರುಗಿ buf + i;

  ಹಿಂತಿರುಗಿ memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ idx_t
ಕ್ಷೇತ್ರ_delim_overlap (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  ಆಗ (0 < overlap)
    {
      ಆದರೆ (memeq (buf + len - overlap, delim_ಬೈಟುಗಳು, overlap))
        ಹಿಂತಿರುಗಿ overlap;
      overlap--;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ ಕ್ಷೇತ್ರ_terminator
ಕಂಡುಹಿಡಿ_ಕ್ಷೇತ್ರ_terminator (ಅಕ್ಷರ *buf, idx_t len,
                       ರಚನೆ bytesearch_context *ctx, ಅಕ್ಷರ **terminator)
{
  ಆದರೆ (ctx->ಬಹುಲಕ_2 == BYTESEARCH_ಸಾಲು_ONLY)
    {
      ಆದರೆ (!ctx->ಸಾಲು_ಕೊನೆ_ತಿಳಿದ)
        {
          ctx->ಸಾಲು_ಕೊನೆ = memchr (buf, ಸಾಲು_delim, len);
          ctx->ಸಾಲು_ಕೊನೆ_ತಿಳಿದ = true;
        }

      *terminator = ctx->ಸಾಲು_ಕೊನೆ;
      ಹಿಂತಿರುಗಿ *terminator ? ಕ್ಷೇತ್ರ_ಸಾಲು_DELIMITER : ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ;
    }

  ಆದರೆ (ಕ್ಷೇತ್ರ_delim_is_ಸಾಲು_delim ())
    {
      *terminator = memchr (buf, ಸಾಲು_delim, len);
      ಹಿಂತಿರುಗಿ *terminator ? ಕ್ಷೇತ್ರ_DELIMITER : ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ;
    }

  ಆದರೆ (!ctx->ಸಾಲು_ಕೊನೆ_ತಿಳಿದ)
    {
      ctx->ಸಾಲು_ಕೊನೆ = memchr (buf, ಸಾಲು_delim, len);
      ctx->ಸಾಲು_ಕೊನೆ_ತಿಳಿದ = true;
    }

  idx_t ಕ್ಷೇತ್ರ_len = ctx->ಸಾಲು_ಕೊನೆ ? ctx->ಸಾಲು_ಕೊನೆ - buf : len;

  ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_ಕೊನೆ = (ctx->blank_delimited
                     ? ಕಂಡುಹಿಡಿ_c_blank (buf, ಕ್ಷೇತ್ರ_len)
                     : ಕಂಡುಹಿಡಿ_ಕ್ಷೇತ್ರ_delim (buf, ಕ್ಷೇತ್ರ_len));

  ಆದರೆ (ಕ್ಷೇತ್ರ_ಕೊನೆ)
    {
      *terminator = ಕ್ಷೇತ್ರ_ಕೊನೆ;
      ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_DELIMITER;
    }

  *terminator = ctx->ಸಾಲು_ಕೊನೆ;
  ಹಿಂತಿರುಗಿ ctx->ಸಾಲು_ಕೊನೆ ? ಕ್ಷೇತ್ರ_ಸಾಲು_DELIMITER : ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
maybe_write_ಸಾಲು_delim (bool ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ, uintmax_t ಕ್ಷೇತ್ರ_idx)
{
  ಆದರೆ (ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ
      || !(suppress_non_delimited && ಕ್ಷೇತ್ರ_idx == 1))
    write_ಸಾಲು_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
begin_ಕ್ಷೇತ್ರ_output (uintmax_t ಕ್ಷೇತ್ರ_idx, bool ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                    bool *ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ)
{
  bool write_ಕ್ಷೇತ್ರ = print_kth (ಕ್ಷೇತ್ರ_idx);

  ಆದರೆ (write_ಕ್ಷೇತ್ರ && ! (ಕ್ಷೇತ್ರ_idx == 1 && ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ))
    {
      ಆದರೆ (*ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ)
        write_ಬೈಟುಗಳು (output_delimiter_string, output_delimiter_ಉದ್ದಳತೆ);
      *ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ = true;
    }

  ಹಿಂತಿರುಗಿ write_ಕ್ಷೇತ್ರ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
handle_ಕ್ಷೇತ್ರ_1 (uintmax_t ಕ್ಷೇತ್ರ_idx, bool ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                idx_t *ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು)
{
  bool ಕ್ಷೇತ್ರ_1_selected = false;

  ಆದರೆ (ಕ್ಷೇತ್ರ_idx == 1 && ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ)
    {
      ಆದರೆ ((ಕ್ಷೇತ್ರ_1_selected = print_kth (1)))
        write_ಬೈಟುಗಳು (ಕ್ಷೇತ್ರ_1_ಬಫರ್, *ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು);
      *ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು = 0;
    }

  ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_1_selected;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
handle_ಕ್ಷೇತ್ರ_delimiter (uintmax_t *ಕ್ಷೇತ್ರ_idx, bool ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                        idx_t *ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು,
                        bool *ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ, bool *write_ಕ್ಷೇತ್ರ)
{
  ಆದರೆ (handle_ಕ್ಷೇತ್ರ_1 (*ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ, ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು))
    *ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ = true;

  ಮುಂದಿನ_ಅಂಶ (ಕ್ಷೇತ್ರ_idx);
  *write_ಕ್ಷೇತ್ರ = begin_ಕ್ಷೇತ್ರ_output (*ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                                     ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ಕ್ಷೇತ್ರ_selection_exhausted (uintmax_t ಕ್ಷೇತ್ರ_idx)
{
  ಹಿಂತಿರುಗಿ !print_kth (ಕ್ಷೇತ್ರ_idx) && current_rp->lo == UINTMAX_MAX;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
ಮುಕ್ತಾಯ_current_ಸಾಲು (uintmax_t ಕ್ಷೇತ್ರ_idx, bool ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                     idx_t *ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು,
                     bool ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ, bool ಸಾಲು_terminated)
{
  ಆದರೆ (ಕ್ಷೇತ್ರ_idx == 1 && ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ)
    {
      ಆದರೆ (!suppress_non_delimited
          && (ಸಾಲು_terminated || *ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು != 0))
        {
          write_ಬೈಟುಗಳು (ಕ್ಷೇತ್ರ_1_ಬಫರ್, *ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು);
          write_ಸಾಲು_delim ();
        }
      *ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು = 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    maybe_write_ಸಾಲು_delim (ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ, ಕ್ಷೇತ್ರ_idx);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
sync_byte_selection (uintmax_t byte_idx)
{
  ಆಗ (current_rp->hi <= byte_idx)
    current_rp++;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
reset_ಕ್ಷೇತ್ರ_ಸಾಲು (uintmax_t *ಕ್ಷೇತ್ರ_idx, bool *ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ,
                  bool *have_pending_ಸಾಲು, ರಚನೆ mbfield_parser *parser)
{
  *ಕ್ಷೇತ್ರ_idx = 1;
  current_rp = frp;
  *ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ = false;
  *have_pending_ಸಾಲು = false;
  parser->have_saved = false;
  parser->at_ಸಾಲು_ಆರಂಭ = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_ಬೈಟುಗಳು (FILE *ಹರಿವು)
{
  ಸ್ಥಿರ ಅಕ್ಷರ ಬೈಟುಗಳು_ಒಳ[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  ಪೂರ್ಣಾಂಕ fd = fileno (ಹರಿವು);

  current_rp = frp;

  ಆಗ (true)
    {
      idx_t ಲಭ್ಯ = ಪಠ (fd, ಬೈಟುಗಳು_ಒಳ, ಗಾತ್ರ ಬೈಟುಗಳು_ಒಳ);
      ಆದರೆ (ಲಭ್ಯ <= 0)
        {
          ಆದರೆ (ಲಭ್ಯ < 0)
            fseterr (ಹರಿವು);
          write_pending_ಸಾಲು_delim (byte_idx);
          ನಿಲ್ಲಿಸು;
        }

      idx_t processed = 0;

      ಆಗ (processed < ಲಭ್ಯ)
        {
          ಅಕ್ಷರ *ಸಾಲು = ಬೈಟುಗಳು_ಒಳ + processed;
          idx_t ಗಮ್ಯ_process = ಲಭ್ಯ - processed;
          ಅಕ್ಷರ *ಸಾಲು_ಕೊನೆ = ಹುಡುಕು_ಬೈಟುಗಳು (ಸಾಲು, ಸಾಲು_delim, ಗಮ್ಯ_process);
          ಅಕ್ಷರ *ಕೊನೆ = ಸಾಲು + (ಸಾಲು_ಕೊನೆ ? ಸಾಲು_ಕೊನೆ - ಸಾಲು : ಗಮ್ಯ_process);
          ಅಕ್ಷರ *p = ಸಾಲು;

          ಆಗ (0 < ಕೊನೆ - p)
            {
              sync_byte_selection (byte_idx);

              ಆದರೆ (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (ಕೊನೆ - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  idx_t n = MIN (ಕೊನೆ - p, current_rp->hi - byte_idx);
                  write_selected_ಅಂಶ (&print_delimiter,
                                       is_range_ಆರಂಭ_ಸೂಚ್ಯಂಕ (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += ಕೊನೆ - ಸಾಲು;
          ಆದರೆ (ಸಾಲು_ಕೊನೆ)
            {
              processed++;
              reset_ಅಂಶ_ಸಾಲು (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_characters_ಬಹುಲಕ (FILE *ಹರಿವು, bool byte_ಬಹುಲಕ)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  ಸ್ಥಿರ ಅಕ್ಷರ ಬೈಟುಗಳು_ಒಳ[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, ಬೈಟುಗಳು_ಒಳ, ಗಾತ್ರ ಬೈಟುಗಳು_ಒಳ, ಹರಿವು);

  ಆಗ (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      ಆದರೆ (idx && ಕ್ಷೇತ್ರ_selection_exhausted (idx))
        {
          idx_t ಲಭ್ಯ = mbbuf_topup (&mbbuf);
          ಅಕ್ಷರ *p = mbbuf.ಬಫರ್ + mbbuf.ಸ್ಥಾನಾಂತರ;
          ಅಕ್ಷರ *ಕೊನೆ = ಹುಡುಕು_ಬೈಟುಗಳು (p, ಸಾಲು_delim, ಲಭ್ಯ);
          mbbuf_advance (&mbbuf, ಕೊನೆ ? ಕೊನೆ - p : ಲಭ್ಯ);
          ಆದರೆ (!ಕೊನೆ && ಲಭ್ಯ)
            ಮುಂದುವರಿ;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      ಆದರೆ (!byte_ಬಹುಲಕ && utf8 && idx + 1 < current_rp->lo)
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = mbbuf.ಬಫರ್ + mbbuf.ಸ್ಥಾನಾಂತರ;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೊನೆ = ಹುಡುಕು_ಬೈಟುಗಳು (p, ಸಾಲು_delim, n);
          ಆದರೆ (ಕೊನೆ)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, ಕೊನೆ - p + 1);
              reset_ಅಂಶ_ಸಾಲು (&idx, &print_delimiter);
              ಮುಂದುವರಿ;
            }

          idx += u8_ಎಣಿಕೆ (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      ಆದರೆ (g.ch == ಸಾಲು_delim)
        reset_ಅಂಶ_ಸಾಲು (&idx, &print_delimiter);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (g.ch == MBBUF_EOF)
        {
          write_pending_ಸಾಲು_delim (idx);
          ನಿಲ್ಲಿಸು;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (byte_ಬಹುಲಕ)
        {
          bool ಮೊದಲ_selected_is_range_ಆರಂಭ = false;
          bool ನೋಡಿದ_selected = false;
          bool suffix_selected = true;

          ಗಾಗಿ (idx_t i = 0; i < g.len; i++)
            {
              ಮುಂದಿನ_ಅಂಶ (&idx);
              ಆದರೆ (print_kth (idx))
                {
                  ಆದರೆ (!ನೋಡಿದ_selected)
                    {
                      ನೋಡಿದ_selected = true;
                      ಮೊದಲ_selected_is_range_ಆರಂಭ
                        = is_range_ಆರಂಭ_ಸೂಚ್ಯಂಕ (idx);
                    }
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ನೋಡಿದ_selected)
                suffix_selected = false;
            }

          ಆದರೆ (ನೋಡಿದ_selected && suffix_selected)
            write_selected_ಅಂಶ (&print_delimiter,ಮೊದಲ_selected_is_range_ಆರಂಭ,
                                 mbbuf_char_ಸ್ಥಾನಾಂತರ (&mbbuf, g), g.len);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಮುಂದಿನ_ಅಂಶ (&idx);
          ಆದರೆ (print_kth (idx))
            write_selected_ಅಂಶ (&print_delimiter,
                                 is_range_ಆರಂಭ_ಸೂಚ್ಯಂಕ (idx),
                                 mbbuf_char_ಸ್ಥಾನಾಂತರ (&mbbuf, g), g.len);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
cut_ಬೈಟುಗಳು_no_ವಿಭಜಿಸು (FILE *ಹರಿವು)
{
  cut_characters_ಬಹುಲಕ (ಹರಿವು, true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
cut_characters (FILE *ಹರಿವು)
{
  cut_characters_ಬಹುಲಕ (ಹರಿವು, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_fields_mb_any (FILE *ಹರಿವು, bool whitespace_ಬಹುಲಕ)
{
  ಸ್ಥಿರ ಅಕ್ಷರ ಬೈಟುಗಳು_ಒಳ[IO_BUFSIZE];
  mbbuf_t mbbuf;
  ರಚನೆ mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_ಬಹುಲಕ,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_ಸಾಲು_ಆರಂಭ = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t ಕ್ಷೇತ್ರ_idx = 1;
  bool ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ = false;
  bool ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ;
  bool have_pending_ಸಾಲು = false;
  bool write_ಕ್ಷೇತ್ರ;
  idx_t ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, ಬೈಟುಗಳು_ಒಳ, ಗಾತ್ರ ಬೈಟುಗಳು_ಒಳ, ಹರಿವು);

  ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ = (suppress_non_delimited ^ !print_kth (1));
  write_ಕ್ಷೇತ್ರ = begin_ಕ್ಷೇತ್ರ_output (ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                                    &ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ);

  ಆಗ (true)
    {
      ಎಣಿಕೆ ಕ್ಷೇತ್ರ_terminator terminator;
      ಆದರೆ (!ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ || ಕ್ಷೇತ್ರ_idx != 1)
        terminator = ಪರಿಶೀಲನೆ_mb_ಕ್ಷೇತ್ರ (&mbbuf, &parser, &have_pending_ಸಾಲು,
                                    write_ಕ್ಷೇತ್ರ, ಖಾಲಿ);
      ಇಲ್ಲದಿದ್ದರೆ
        terminator = ಪರಿಶೀಲನೆ_mb_ಕ್ಷೇತ್ರ (&mbbuf, &parser, &have_pending_ಸಾಲು,
                                    false, &ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು);

      ಆದರೆ (terminator == ಕ್ಷೇತ್ರ_DELIMITER)
        {
          handle_ಕ್ಷೇತ್ರ_delimiter (&ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                                  &ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು,
                                  &ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ, &write_ಕ್ಷೇತ್ರ);
          ಆದರೆ (! ಕ್ಷೇತ್ರ_selection_exhausted (ಕ್ಷೇತ್ರ_idx))
            ಮುಂದುವರಿ;

          terminator = ಪರಿಶೀಲನೆ_mb_ಸಾಲು_ಕೊನೆ (&mbbuf, &parser,
                                         &have_pending_ಸಾಲು);
        }

      ಆದರೆ (terminator == ಕ್ಷೇತ್ರ_EOF && !have_pending_ಸಾಲು)
        ನಿಲ್ಲಿಸು;

      ಮುಕ್ತಾಯ_current_ಸಾಲು (ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                           &ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು, ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ,
                           terminator == ಕ್ಷೇತ್ರ_ಸಾಲು_DELIMITER);
      ಆದರೆ (terminator == ಕ್ಷೇತ್ರ_EOF)
        ನಿಲ್ಲಿಸು;

      reset_ಕ್ಷೇತ್ರ_ಸಾಲು (&ಕ್ಷೇತ್ರ_idx, &ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ,
                        &have_pending_ಸಾಲು, &parser);
      write_ಕ್ಷೇತ್ರ = begin_ಕ್ಷೇತ್ರ_output (ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                                        &ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_fields_mb (FILE *ಹರಿವು)
{
  cut_fields_mb_any (ಹರಿವು, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_fields_bytesearch (FILE *ಹರಿವು)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  ಸ್ಥಿರ ಅಕ್ಷರ ಬೈಟುಗಳು_ಒಳ[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t ಕ್ಷೇತ್ರ_idx = 1;
  bool ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ = false;
  bool have_pending_ಸಾಲು = false;
  bool skip_blank_ಸತತ = false;
  bool pending_ಸಾಲು_delim_ಕ್ಷೇತ್ರ = false;
  bool write_ಕ್ಷೇತ್ರ;
  idx_t ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು = 0;

  current_rp = frp;
  bool ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, ಬೈಟುಗಳು_ಒಳ, ಗಾತ್ರ ಬೈಟುಗಳು_ಒಳ - 1, ಹರಿವು);
  write_ಕ್ಷೇತ್ರ = begin_ಕ್ಷೇತ್ರ_output (ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                                    &ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ);

  ರಚನೆ bytesearch_context ಹುಡುಕು = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&ಹುಡುಕು);

  ಆಗ (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      ಹುಡುಕು.at_eof = mbbuf.eof;
      ಹುಡುಕು.ಸಾಲು_ಕೊನೆ_ತಿಳಿದ = false;

      ಆದರೆ (pending_ಸಾಲು_delim_ಕ್ಷೇತ್ರ)
        {
          pending_ಸಾಲು_delim_ಕ್ಷೇತ್ರ = false;

          ಆದರೆ (n_avail == 0)
            {
              ಆದರೆ (handle_ಕ್ಷೇತ್ರ_1 (ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                                  &ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು))
                ಮುಂದಿನ_ಅಂಶ (&ಕ್ಷೇತ್ರ_idx);
              maybe_write_ಸಾಲು_delim (ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ, ಕ್ಷೇತ್ರ_idx);
              ಹಿಂತಿರುಗಿ;
            }

          handle_ಕ್ಷೇತ್ರ_delimiter (&ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                                  &ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು,
                                  &ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ, &write_ಕ್ಷೇತ್ರ);
          ಆದರೆ (whitespace_delimited)
            skip_blank_ಸತತ = true;
        }

      ಆದರೆ (n_avail == 0)
        ನಿಲ್ಲಿಸು;

      ಅಕ್ಷರ *chunk = mbbuf.ಬಫರ್ + mbbuf.ಸ್ಥಾನಾಂತರ;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      ಆದರೆ (ಕ್ಷೇತ್ರ_idx == 1
          && (!whitespace_delimited || !ಕ್ಷೇತ್ರ_selection_exhausted (ಕ್ಷೇತ್ರ_idx))
          && !ಕ್ಷೇತ್ರ_delim_is_ಸಾಲು_delim ()
          && !(whitespace_delimited
               ? ಕಂಡುಹಿಡಿ_c_blank (chunk, n_avail)
               : ಕಂಡುಹಿಡಿ_ಕ್ಷೇತ್ರ_delim (chunk, n_avail)))
        {
          ಅಕ್ಷರ *ಕೊನೆಯ_ಸಾಲು_delim = ಹುಡುಕು.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, ಸಾಲು_delim, n_avail);
          ಆದರೆ (ಕೊನೆಯ_ಸಾಲು_delim)
            {
              idx_t n = ಕೊನೆಯ_ಸಾಲು_delim - chunk + 1;
              ಆದರೆ (! suppress_non_delimited)
                {
                  write_ಬೈಟುಗಳು (ಕ್ಷೇತ್ರ_1_ಬಫರ್, ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು);
                  write_ಬೈಟುಗಳು (chunk, n);
                  ಆದರೆ (ಹುಡುಕು.at_eof && chunk[n - 1] != ಸಾಲು_delim)
                    write_ಸಾಲು_delim ();
                }
              ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_ಸಾಲು = false;
              ಆದರೆ (ಹುಡುಕು.at_eof)
                ಹಿಂತಿರುಗಿ;
              ಮುಂದುವರಿ;
            }
        }

      ಆಗ (processed < n_avail)
        {
          ಅಕ್ಷರ *terminator = ಖಾಲಿ;

          ಆದರೆ (skip_blank_ಸತತ)
            {
              ಆಗ (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              ಆದರೆ (processed == n_avail)
                ನಿಲ್ಲಿಸು;
              skip_blank_ಸತತ = false;
            }

          ಆದರೆ (ಕ್ಷೇತ್ರ_selection_exhausted (ಕ್ಷೇತ್ರ_idx))
            {
              ಆದರೆ (ಕ್ಷೇತ್ರ_delim_is_ಸಾಲು_delim ())
                {
                  maybe_write_ಸಾಲು_delim (ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ,
                                          ಕ್ಷೇತ್ರ_idx);
                  mbbuf_advance (&mbbuf, processed);
                  ಹಿಂತಿರುಗಿ;
                }
              ಹುಡುಕು.ಬಹುಲಕ_2 = BYTESEARCH_ಸಾಲು_ONLY;
              ಎಣಿಕೆ ಕ್ಷೇತ್ರ_terminator terminator_ಬಗೆ
                = ಕಂಡುಹಿಡಿ_ಕ್ಷೇತ್ರ_terminator (chunk + processed, n_avail - processed,
                                         &ಹುಡುಕು, &terminator);
              ಆದರೆ (terminator_ಬಗೆ == ಕ್ಷೇತ್ರ_ಸಾಲು_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_ಸಾಲು_delim (ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ,
                                          ಕ್ಷೇತ್ರ_idx);
                  ಹೋಗು reset_ಸಾಲು;
                }
              processed = n_avail;
              ನಿಲ್ಲಿಸು;
            }

          ಎಣಿಕೆ ಕ್ಷೇತ್ರ_terminator terminator_ಬಗೆ
            = ಕಂಡುಹಿಡಿ_ಕ್ಷೇತ್ರ_terminator (chunk + processed, n_avail - processed,
                                     &ಹುಡುಕು, &terminator);
          idx_t ಕ್ಷೇತ್ರ_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          ಆದರೆ (terminator_ಬಗೆ == ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ
              && !ಹುಡುಕು.at_eof
              && !whitespace_delimited
              && !ಕ್ಷೇತ್ರ_delim_is_ಸಾಲು_delim ())
            ಕ್ಷೇತ್ರ_len -= ಕ್ಷೇತ್ರ_delim_overlap (chunk + processed, ಕ್ಷೇತ್ರ_len);

          ಆದರೆ (ಕ್ಷೇತ್ರ_len || terminator)
            have_pending_ಸಾಲು = true;

          /* Output field content.  */
          ಆದರೆ (ಕ್ಷೇತ್ರ_idx == 1 && ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ)
            append_ಕ್ಷೇತ್ರ_1_chunk (chunk + processed, ಕ್ಷೇತ್ರ_len,
                                  &ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (write_ಕ್ಷೇತ್ರ)
            write_ಬೈಟುಗಳು (chunk + processed, ಕ್ಷೇತ್ರ_len);
          processed += ಕ್ಷೇತ್ರ_len;

          ಆದರೆ (terminator_ಬಗೆ == ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ)
            ನಿಲ್ಲಿಸು;

          ಆದರೆ (terminator_ಬಗೆ == ಕ್ಷೇತ್ರ_DELIMITER)
            {
              ಆದರೆ (ಕ್ಷೇತ್ರ_delim_is_ಸಾಲು_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_ಸಾಲು_delim_ಕ್ಷೇತ್ರ = true;
                  ನಿಲ್ಲಿಸು;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_ಕ್ಷೇತ್ರ_delimiter (&ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                                      &ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು,
                                      &ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ, &write_ಕ್ಷೇತ್ರ);
              ಆದರೆ (whitespace_delimited)
                skip_blank_ಸತತ = true;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              affirm (terminator_ಬಗೆ == ಕ್ಷೇತ್ರ_ಸಾಲು_DELIMITER);
              processed++;
              ಮುಕ್ತಾಯ_current_ಸಾಲು (ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                                   &ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು,
                                   ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ, true);

            reset_ಸಾಲು:
              ಕ್ಷೇತ್ರ_idx = 1;
              current_rp = frp;
              ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ = false;
              have_pending_ಸಾಲು = false;
              bytesearch_context_reset (&ಹುಡುಕು);
              write_ಕ್ಷೇತ್ರ = begin_ಕ್ಷೇತ್ರ_output (ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ,
                                                &ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  ಆದರೆ (!have_pending_ಸಾಲು)
    ಹಿಂತಿರುಗಿ;

  ಮುಕ್ತಾಯ_current_ಸಾಲು (ಕ್ಷೇತ್ರ_idx, ಬಫರ್_ಮೊದಲ_ಕ್ಷೇತ್ರ, &ಕ್ಷೇತ್ರ_1_n_ಬೈಟುಗಳು,
                       ಸಿಕ್ಕಿತು_any_selected_ಕ್ಷೇತ್ರ, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cut_fields_ws (FILE *ಹರಿವು)
{
  ಆದರೆ (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (ಹರಿವು);
  ಇಲ್ಲದಿದ್ದರೆ
    cut_fields_mb_any (ಹರಿವು, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

ಸ್ಥಿರ bool
cut_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ಶೂನ್ಯ (*cut_ಹರಿವು) (FILE *))
{
  FILE *ಹರಿವು;

  ಆದರೆ (streq (file, "-"))
    {
      have_read_stdin = true;
      ಹರಿವು = stdin;
      assume (ಹರಿವು);  /* Pacify GCC bug#109613.  */
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಹರಿವು = fopen (file, "r");
      ಆದರೆ (ಹರಿವು == ಖಾಲಿ)
        {
          error (0, errno, "%s", quotef (file));
          ಹಿಂತಿರುಗಿ false;
        }
    }

  fadvise (ಹರಿವು, FADVISE_SEQUENTIAL);

  cut_ಹರಿವು (ಹರಿವು);

  ಪೂರ್ಣಾಂಕ err = errno;
  ಆದರೆ (!ferror (ಹರಿವು))
    err = 0;
  ಆದರೆ (streq (file, "-"))
    clearerr (ಹರಿವು);		/* Also clear EOF.  */
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose (ಹರಿವು) == EOF)
    err = errno;
  ಆದರೆ (err)
    {
      error (0, err, "%s", quotef (file));
      ಹಿಂತಿರುಗಿ false;
    }
  ಹಿಂತಿರುಗಿ true;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  ಅಕ್ಷರ *spec_list_string = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'b':
          cut_ಬಹುಲಕ = CUT_ಬಹುಲಕ_ಬೈಟುಗಳು;
          FALLTHROUGH;
        ಸ್ಥಿತಿ 'c':
          ಆದರೆ (optc == 'c')
            cut_ಬಹುಲಕ = CUT_ಬಹುಲಕ_CHARACTERS;
          FALLTHROUGH;
        ಸ್ಥಿತಿ 'f':
        ಸ್ಥಿತಿ 'F':
          ಆದರೆ (optc == 'f' || optc == 'F')
            cut_ಬಹುಲಕ = CUT_ಬಹುಲಕ_FIELDS;
          ಆದರೆ (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          ಆದರೆ (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            ಆದರೆ (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_ಬೈಟುಗಳು (delim_ಬೈಟುಗಳು, optarg, g.len + 1);
            delim_specified = true;
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_ವಾದಗಳು,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_ಉದ್ದಳತೆ = (optarg[0] == '\0'
                                     ? 1 : ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (optarg));
          output_delimiter_string = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'n':
          no_ವಿಭಜಿಸು = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          suppress_non_delimited = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'z':
          ಸಾಲು_delim = '\0';
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ COMPLEMENT_OPTION:
          complement = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  ಆದರೆ (cut_ಬಹುಲಕ == CUT_ಬಹುಲಕ_ಬೈಟುಗಳು || cut_ಬಹುಲಕ == CUT_ಬಹುಲಕ_CHARACTERS)
    {
      ಆದರೆ (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an ಒಳಹರಿವು delimiter makes sense\n\
\tonly when operating on fields"));

      ಆದರೆ (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited ಸಾಲುಗಳು makes sense\n\
\tonly when operating on fields"));
    }

  ಆದರೆ (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  ಆದರೆ (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  ಗಣ_fields (spec_list_string,
              (((cut_ಬಹುಲಕ == CUT_ಬಹುಲಕ_ಬೈಟುಗಳು
                 || cut_ಬಹುಲಕ == CUT_ಬಹುಲಕ_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  ಆದರೆ (!delim_specified)
    {
      delim_ಬೈಟುಗಳು[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  ಆದರೆ (output_delimiter_string == ಖಾಲಿ)
    {
      output_delimiter_string = output_delimiter_default;
      ಆದರೆ (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_ಉದ್ದಳತೆ = 1;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          copy_ಬೈಟುಗಳು (output_delimiter_default, delim_ಬೈಟುಗಳು, delim_mcel.len);
          output_delimiter_ಉದ್ದಳತೆ = delim_mcel.len;
        }
    }

  ಶೂನ್ಯ (*cut_ಹರಿವು) (FILE *) = ಖಾಲಿ;
  ಆಯ್ಕೆ (cut_ಬಹುಲಕ)
    {
    ಸ್ಥಿತಿ CUT_ಬಹುಲಕ_ಬೈಟುಗಳು:
      cut_ಹರಿವು = MB_CUR_MAX <= 1 || !no_ವಿಭಜಿಸು
                   ? cut_ಬೈಟುಗಳು : cut_ಬೈಟುಗಳು_no_ವಿಭಜಿಸು;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CUT_ಬಹುಲಕ_CHARACTERS:
      cut_ಹರಿವು = MB_CUR_MAX <= 1 ? cut_ಬೈಟುಗಳು : cut_characters;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CUT_ಬಹುಲಕ_FIELDS:
      cut_ಹರಿವು = whitespace_delimited ? cut_fields_ws
                   : bytesearch_ಕ್ಷೇತ್ರ_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CUT_ಬಹುಲಕ_NONE:
      affirm (false);
    }
  affirm (cut_ಹರಿವು);
  ಆದರೆ (optind == argc)
    ok = cut_file ("-", cut_ಹರಿವು);
  ಇಲ್ಲದಿದ್ದರೆ
    ಗಾಗಿ (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_ಹರಿವು);


  ಆದರೆ (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
