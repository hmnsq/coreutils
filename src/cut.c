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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن "system.h"

#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "fadvise.h"
#ضمن "ioblksize.h"
#ضمن "mbbuf.h"
#ضمن "memchr2.h"

#ضمن "set-fields.h"
#ضمن "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کړه پروګرام_NAME "cut"

#تعریف_کړه AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#تعریف_کړه FATAL_ERROR(پیغام)						\
  افعل									\
    {									\
      error (0, 0, (پیغام));						\
      usage (EXIT_FAILURE);						\
    }									\
  طالما (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#تعریف_کړه SMALL_BYTE_پوله 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
ساكن بنية ډګر_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
ساكن حرف *ډګر_1_بفر;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
ساكن idx_t ډګر_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
ساكن bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
ساكن bool complement;

/* The delimiter character for multibyte field mode.  */
ساكن mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
ساكن حرف delim_بایټونه[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
ساكن بینښه حرف کرښه_delim = '\n';

/* The length of output_delimiter_string.  */
ساكن size_t output_delimiter_اوږدتیا;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
ساكن حرف *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
ساكن حرف output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
ساكن bool no_وېشل;

/* If true, interpret each run of whitespace as one field delimiter.  */
ساكن bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
ساكن bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
ساكن bool space_output_delimiter_default;

تعداد whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

ساكن حرف ثابت *ثابت whitespace_option_دلیلونه[] =
{
  "trimmed", عدم
};

ساكن تعداد whitespace_option ثابت whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_دلیلونه, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
ساكن تعداد
{
  CUT_ډېرپېښ_NONE,
  CUT_ډېرپېښ_بایټونه,
  CUT_ډېرپېښ_CHARACTERS,
  CUT_ډېرپېښ_FIELDS
} cut_ډېرپېښ = CUT_ډېرپېښ_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"bytes", required_دلیل, عدم, 'b'},
  {"characters", required_دلیل, عدم, 'c'},
  {"fields", required_دلیل, عدم, 'f'},
  {"delimiter", required_دلیل, عدم, 'd'},
  {"no-partial", no_دلیل, عدم, 'n'},
  {"whitespace-delimited", optional_دلیل, عدم, 'w'},
  {"only-delimited", no_دلیل, عدم, 's'},
  {"output-delimiter", required_دلیل, عدم, 'O'},
  {"complement", no_دلیل, عدم, COMPLEMENT_OPTION},
  {"zero-terminated", no_دلیل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              پروګرام_name);
      fputs (_("\
Print selected parts د کرښې from each FILE منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --بایټونه=LIST\n\
         select only these byte positions\n\
"));
      oputs (_("\
  -c, --characters=LIST\n\
         select only these character positions\n\
"));
      oputs (_("\
      --complement\n\
         complement the سېټ_2 د selected بایټونه, characters or fields\n\
"));
      oputs (_("\
  -d, --delimiter=DELIM\n\
         use DELIM instead د TAB لكل ډګر delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any کرښه that لري\n\
         no delimiter character, unless the -s option is specified\n\
"));
      oputs (_("\
  -F LIST\n\
         like -f, but also پایله_ورکوي -w and -O ' '\n\
"));
      oputs (_("\
  -n, --no-partial\n\
         with -b, don't output partial multi-byte characters\n\
"));
      oputs (_("\
  -O, --output-delimiter=STRING\n\
         use STRING as the output delimiter;\n\
         the افتراضي is منزل use the ننوت delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         افعل not print کرښې not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a پرلپسې_2 د blank characters as the ډګر delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         کرښه delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one د -b, -c, -f or -F.  Each LIST is made پورته د one\n\
range, or many ranges separated by commas.  Selected ننوت is written\n\
دننه the same ترتیب that it is اقرأ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one د:\n\
\n\
  N     N'th byte, character or ډګر, counted from 1\n\
  N-    from N'th byte, character or ډګر, منزل پای_ټکی د کرښه\n\
  N-M   from N'th to M'th (included) byte, character or ډګر\n\
  -M    from لومړی منزل M'th (included) byte, character or ډګر\n\
"), stdout);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

ساكن مضمن فراغ
بل_توکی (uintmax_t *توکی_idx)
{
  (*توکی_idx)++;
  إذا ((*توکی_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

ساكن مضمن bool
print_kth (uintmax_t k)
{
  أرجع current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

ساكن مضمن bool
is_range_پیل_شاخص (uintmax_t k)
{
  أرجع k == current_rp->lo;
}

ساكن مضمن bool
ډګر_delim_is_کرښه_delim (فراغ)
{
  أرجع delim_mcel.len == 1 && delim_بایټونه[0] == کرښه_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

ساكن bool
mcel_isblank (mcel_t g)
{
  أرجع (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

ساكن مضمن bool
bytesearch_ډګر_delim_ok (فراغ)
{
  بینښه حرف delim_0 = delim_بایټونه[0];

  أرجع (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


تعداد ډګر_terminator
{
  ډګر_معلومات,
  ډګر_DELIMITER,
  ډګر_کرښه_DELIMITER,
  ډګر_EOF
};

تعداد bytesearch_ډېرپېښ
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_کرښه_ONLY
};

بنية bytesearch_context
{
  تعداد bytesearch_ډېرپېښ ډېرپېښ_2;
  bool blank_delimited;
  bool at_eof;
  حرف *کرښه_پای_ټکی;
  bool کرښه_پای_ټکی_معلوم;
};

ساكن مضمن فراغ
bytesearch_context_reset (بنية bytesearch_context *ctx)
{
  ctx->ډېرپېښ_2 = BYTESEARCH_FIELDS;
  ctx->کرښه_پای_ټکی = عدم;
  ctx->کرښه_پای_ټکی_معلوم = false;
}

بنية mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_کرښه_پیل;
  bool have_saved;
  mcel_t saved_g;
};

ساكن مضمن mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  إذا (*have_saved)
    {
      *have_saved = false;
      أرجع *saved_g;
    }
  أرجع mbbuf_get_char (mbbuf);
}

ساكن مضمن تعداد ډګر_terminator
skip_whitespace_پرلپسې (mbbuf_t *mbuf, بنية mbfield_parser *parser,
                     bool *have_pending_کرښه,
                     bool have_initial_whitespace)
{
  mcel_t g;

  افعل
    {
      g = mbbuf_get_char (mbuf);
      إذا (g.ch != MBBUF_EOF)
        *have_pending_کرښه = true;
    }
  طالما (g.ch != MBBUF_EOF && g.ch != کرښه_delim && mcel_isblank (g));

  bool trim_پیل = parser->trim_outer_whitespace && parser->at_کرښه_پیل;

  إذا (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == کرښه_delim))
    {
      أرجع g.ch == MBBUF_EOF ? ډګر_EOF : ډګر_کرښه_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  أرجع trim_پیل && !have_initial_whitespace ? ډګر_معلومات : ډګر_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

ساكن فراغ
write_بایټونه (حرف ثابت *buf, size_t n_بایټونه)
{
  إذا (n_بایټونه <= SMALL_BYTE_پوله)
    {
      لكل (size_t i = 0; i < n_بایټونه; i++)
        إذا (توری_چاپ_کړه (buf[i]) < 0)
          write_error ();
      أرجع;
    }

  إذا (fwrite (buf, حجم (حرف), n_بایټونه, stdout) != n_بایټونه)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

ساكن مضمن فراغ
copy_بایټونه (حرف *dst, حرف ثابت *src, size_t n_بایټونه)
{
  إذا (n_بایټونه <= SMALL_BYTE_پوله)
    {
      لكل (size_t i = 0; i < n_بایټونه; i++)
        dst[i] = src[i];
      أرجع;
    }

  حافظه_نقل (dst, src, n_بایټونه);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

ساكن مضمن فراغ*
پلټنه_بایټونه (حرف ثابت *buf, صحيح c, size_t n_بایټونه)
{
  إذا (n_بایټونه != 0 && منزل_uchar (buf[0]) == c)
    أرجع (حرف *) buf;
  إذا (1 < n_بایټونه && منزل_uchar (buf[1]) == c)
    أرجع (حرف *) buf + 1;

  أرجع 2 < n_بایټونه ? (حرف *) memchr (buf + 2, c, n_بایټونه - 2) : عدم;
}

ساكن مضمن فراغ
write_کرښه_delim (فراغ)
{
  إذا (توری_چاپ_کړه (کرښه_delim) < 0)
    write_error ();
}

ساكن مضمن فراغ
reset_توکی_کرښه (uintmax_t *توکی_idx, bool *print_delimiter)
{
  write_کرښه_delim ();
  *توکی_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

ساكن مضمن فراغ
write_pending_کرښه_delim (uintmax_t توکی_idx)
{
  إذا (توکی_idx > 0)
    write_کرښه_delim ();
}

ساكن مضمن فراغ
write_selected_توکی (bool *print_delimiter, bool range_پیل,
                     حرف ثابت *buf, size_t n_بایټونه)
{
  إذا (output_delimiter_string != output_delimiter_default)
    {
      إذا (*print_delimiter && range_پیل)
        write_بایټونه (output_delimiter_string, output_delimiter_اوږدتیا);
      *print_delimiter = true;
    }

  write_بایټونه (buf, n_بایټونه);
}

ساكن مضمن فراغ
append_ډګر_1_chunk (حرف ثابت *buf, idx_t len, idx_t *n_بایټونه)
{
  إذا (ډګر_1_bufsize - *n_بایټونه < len)
    {
      ډګر_1_بفر = xpalloc (ډګر_1_بفر, &ډګر_1_bufsize,
                                len, -1, حجم *ډګر_1_بفر);
    }

  copy_بایټونه (ډګر_1_بفر + *n_بایټونه, buf, len);
  *n_بایټونه += len;
}

ساكن مضمن فراغ
append_ډګر_1_بایټونه (mbbuf_t *mbbuf, mcel_t g, idx_t *n_بایټونه)
{
  append_ډګر_1_chunk (mbbuf_char_دننه_واټن (mbbuf, g), g.len, n_بایټونه);
}

ساكن مضمن تعداد ډګر_terminator
لټون_mb_blank_ډګر (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
                     bool *have_pending_کرښه, bool write_ډګر,
                     idx_t *n_بایټونه)
{
  إذا (parser->trim_outer_whitespace && parser->at_کرښه_پیل)
    {
      تعداد ډګر_terminator terminator
        = skip_whitespace_پرلپسې (mbbuf, parser, have_pending_کرښه, false);
      إذا (terminator != ډګر_معلومات)
        أرجع terminator;
    }

  parser->at_کرښه_پیل = false;

  طالما (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      إذا (g.ch == MBBUF_EOF)
        أرجع ډګر_EOF;

      *have_pending_کرښه = true;

      إذا (g.ch == کرښه_delim)
        أرجع ډګر_کرښه_DELIMITER;

      إذا (mcel_isblank (g))
        أرجع skip_whitespace_پرلپسې (mbbuf, parser, have_pending_کرښه, true);

      إذا (n_بایټونه)
        append_ډګر_1_بایټونه (mbbuf, g, n_بایټونه);
      وإلا إذا (write_ډګر)
        write_بایټونه (mbbuf_char_دننه_واټن (mbbuf, g), g.len);
    }
}

ساكن مضمن تعداد ډګر_terminator
لټون_mb_delim_ډګر (mbbuf_t *mbbuf, bool *have_pending_کرښه,
                     bool write_ډګر, idx_t *n_بایټونه)
{
  طالما (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      إذا (g.ch == MBBUF_EOF)
        أرجع ډګر_EOF;

      *have_pending_کرښه = true;

      إذا (g.ch == کرښه_delim)
        أرجع ډګر_کرښه_DELIMITER;

      إذا (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        أرجع ډګر_DELIMITER;

      إذا (n_بایټونه)
        append_ډګر_1_بایټونه (mbbuf, g, n_بایټونه);
      وإلا إذا (write_ډګر)
        write_بایټونه (mbbuf_char_دننه_واټن (mbbuf, g), g.len);
    }
}

ساكن مضمن تعداد ډګر_terminator
لټون_mb_ډګر (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
               bool *have_pending_کرښه, bool write_ډګر, idx_t *n_بایټونه)
{
  أرجع (parser->whitespace_delimited
          ? لټون_mb_blank_ډګر (mbbuf, parser, have_pending_کرښه,
                                 write_ډګر, n_بایټونه)
          : لټون_mb_delim_ډګر (mbbuf, have_pending_کرښه, write_ډګر,
                                 n_بایټونه));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

ساكن تعداد ډګر_terminator
لټون_mb_کرښه_پای_ټکی (mbbuf_t *mbbuf, بنية mbfield_parser *parser,
                  bool *have_pending_کرښه)
{
  إذا (parser->have_saved)
    {
      parser->have_saved = false;
      إذا (parser->saved_g.ch == MBBUF_EOF)
        أرجع ډګر_EOF;

      *have_pending_کرښه = true;
      إذا (parser->saved_g.ch == کرښه_delim)
        أرجع ډګر_کرښه_DELIMITER;
    }

  طالما (true)
    {
      idx_t شته = mbbuf_topup (mbbuf);
      إذا (شته == 0)
        أرجع ډګر_EOF;

      حرف *بفر = mbbuf->بفر + mbbuf->دننه_واټن;
      حرف *کرښه_پای_ټکی = پلټنه_بایټونه (بفر, کرښه_delim, شته);
      idx_t n = کرښه_پای_ټکی ? کرښه_پای_ټکی - بفر + 1 : شته;
      mbbuf_advance (mbbuf, n);
      *have_pending_کرښه = true;

      إذا (کرښه_پای_ټکی)
        أرجع ډګر_کرښه_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_خالص
ساكن حرف *
پیدا_کړه_ډګر_delim (حرف *buf, size_t len)
{
  إذا (len < delim_mcel.len)
    أرجع عدم;

#إذا ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  أرجع memmem (buf, len, delim_بایټونه, delim_mcel.len);
#وإلا
  إذا (delim_mcel.len == 1)
    أرجع memchr (buf, delim_بایټونه[0], len);

  حرف ثابت *p = buf;
  حرف ثابت *پای_ټکی = buf + len;

  حرف saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  طالما (p < پای_ټکی)
    {
      حرف ثابت *nul = memchr (p, '\0', پای_ټکی - p);
      إذا (!nul)
        {
          حرف ثابت *سمون = مزي_پلټنه (p, delim_بایټونه);
          buf[len] = saved;
          أرجع (حرف *) سمون;
        }

      إذا (p < nul)
        {
          حرف ثابت *سمون = مزي_پلټنه (p, delim_بایټونه);
          إذا (سمون)
            {
              buf[len] = saved;
              أرجع (حرف *) سمون;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  أرجع عدم;
#د_که_پای
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_خالص
ساكن حرف *
پیدا_کړه_c_blank (حرف *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  لكل (idx_t i = 0; i < n; i++)
    إذا (c_isblank (buf[i]))
      أرجع buf + i;

  أرجع memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_خالص
ساكن idx_t
ډګر_delim_overlap (حرف ثابت *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  طالما (0 < overlap)
    {
      إذا (memeq (buf + len - overlap, delim_بایټونه, overlap))
        أرجع overlap;
      overlap--;
    }

  أرجع 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

ساكن مضمن تعداد ډګر_terminator
پیدا_کړه_ډګر_terminator (حرف *buf, idx_t len,
                       بنية bytesearch_context *ctx, حرف **terminator)
{
  إذا (ctx->ډېرپېښ_2 == BYTESEARCH_کرښه_ONLY)
    {
      إذا (!ctx->کرښه_پای_ټکی_معلوم)
        {
          ctx->کرښه_پای_ټکی = memchr (buf, کرښه_delim, len);
          ctx->کرښه_پای_ټکی_معلوم = true;
        }

      *terminator = ctx->کرښه_پای_ټکی;
      أرجع *terminator ? ډګر_کرښه_DELIMITER : ډګر_معلومات;
    }

  إذا (ډګر_delim_is_کرښه_delim ())
    {
      *terminator = memchr (buf, کرښه_delim, len);
      أرجع *terminator ? ډګر_DELIMITER : ډګر_معلومات;
    }

  إذا (!ctx->کرښه_پای_ټکی_معلوم)
    {
      ctx->کرښه_پای_ټکی = memchr (buf, کرښه_delim, len);
      ctx->کرښه_پای_ټکی_معلوم = true;
    }

  idx_t ډګر_len = ctx->کرښه_پای_ټکی ? ctx->کرښه_پای_ټکی - buf : len;

  حرف *ډګر_پای_ټکی = (ctx->blank_delimited
                     ? پیدا_کړه_c_blank (buf, ډګر_len)
                     : پیدا_کړه_ډګر_delim (buf, ډګر_len));

  إذا (ډګر_پای_ټکی)
    {
      *terminator = ډګر_پای_ټکی;
      أرجع ډګر_DELIMITER;
    }

  *terminator = ctx->کرښه_پای_ټکی;
  أرجع ctx->کرښه_پای_ټکی ? ډګر_کرښه_DELIMITER : ډګر_معلومات;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

ساكن مضمن فراغ
maybe_write_کرښه_delim (bool وموندل_any_selected_ډګر, uintmax_t ډګر_idx)
{
  إذا (وموندل_any_selected_ډګر
      || !(suppress_non_delimited && ډګر_idx == 1))
    write_کرښه_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

ساكن مضمن bool
begin_ډګر_output (uintmax_t ډګر_idx, bool بفر_لومړی_ډګر,
                    bool *وموندل_any_selected_ډګر)
{
  bool write_ډګر = print_kth (ډګر_idx);

  إذا (write_ډګر && ! (ډګر_idx == 1 && بفر_لومړی_ډګر))
    {
      إذا (*وموندل_any_selected_ډګر)
        write_بایټونه (output_delimiter_string, output_delimiter_اوږدتیا);
      *وموندل_any_selected_ډګر = true;
    }

  أرجع write_ډګر;
}

ساكن مضمن bool
handle_ډګر_1 (uintmax_t ډګر_idx, bool بفر_لومړی_ډګر,
                idx_t *ډګر_1_n_بایټونه)
{
  bool ډګر_1_selected = false;

  إذا (ډګر_idx == 1 && بفر_لومړی_ډګر)
    {
      إذا ((ډګر_1_selected = print_kth (1)))
        write_بایټونه (ډګر_1_بفر, *ډګر_1_n_بایټونه);
      *ډګر_1_n_بایټونه = 0;
    }

  أرجع ډګر_1_selected;
}

ساكن مضمن فراغ
handle_ډګر_delimiter (uintmax_t *ډګر_idx, bool بفر_لومړی_ډګر,
                        idx_t *ډګر_1_n_بایټونه,
                        bool *وموندل_any_selected_ډګر, bool *write_ډګر)
{
  إذا (handle_ډګر_1 (*ډګر_idx, بفر_لومړی_ډګر, ډګر_1_n_بایټونه))
    *وموندل_any_selected_ډګر = true;

  بل_توکی (ډګر_idx);
  *write_ډګر = begin_ډګر_output (*ډګر_idx, بفر_لومړی_ډګر,
                                     وموندل_any_selected_ډګر);
}

ساكن مضمن bool
ډګر_selection_exhausted (uintmax_t ډګر_idx)
{
  أرجع !print_kth (ډګر_idx) && current_rp->lo == UINTMAX_MAX;
}

ساكن مضمن فراغ
پای_current_کرښه (uintmax_t ډګر_idx, bool بفر_لومړی_ډګر,
                     idx_t *ډګر_1_n_بایټونه,
                     bool وموندل_any_selected_ډګر, bool کرښه_terminated)
{
  إذا (ډګر_idx == 1 && بفر_لومړی_ډګر)
    {
      إذا (!suppress_non_delimited
          && (کرښه_terminated || *ډګر_1_n_بایټونه != 0))
        {
          write_بایټونه (ډګر_1_بفر, *ډګر_1_n_بایټونه);
          write_کرښه_delim ();
        }
      *ډګر_1_n_بایټونه = 0;
    }
  وإلا
    maybe_write_کرښه_delim (وموندل_any_selected_ډګر, ډګر_idx);
}

ساكن مضمن فراغ
sync_byte_selection (uintmax_t byte_idx)
{
  طالما (current_rp->hi <= byte_idx)
    current_rp++;
}

ساكن مضمن فراغ
reset_ډګر_کرښه (uintmax_t *ډګر_idx, bool *وموندل_any_selected_ډګر,
                  bool *have_pending_کرښه, بنية mbfield_parser *parser)
{
  *ډګر_idx = 1;
  current_rp = frp;
  *وموندل_any_selected_ډګر = false;
  *have_pending_کرښه = false;
  parser->have_saved = false;
  parser->at_کرښه_پیل = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

ساكن فراغ
cut_بایټونه (FILE *بهیر)
{
  ساكن حرف بایټونه_دننه[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  صحيح fd = fileno (بهیر);

  current_rp = frp;

  طالما (true)
    {
      idx_t شته = اقرأ (fd, بایټونه_دننه, حجم بایټونه_دننه);
      إذا (شته <= 0)
        {
          إذا (شته < 0)
            fseterr (بهیر);
          write_pending_کرښه_delim (byte_idx);
          اكسر;
        }

      idx_t processed = 0;

      طالما (processed < شته)
        {
          حرف *کرښه = بایټونه_دننه + processed;
          idx_t منزل_process = شته - processed;
          حرف *کرښه_پای_ټکی = پلټنه_بایټونه (کرښه, کرښه_delim, منزل_process);
          حرف *پای_ټکی = کرښه + (کرښه_پای_ټکی ? کرښه_پای_ټکی - کرښه : منزل_process);
          حرف *p = کرښه;

          طالما (0 < پای_ټکی - p)
            {
              sync_byte_selection (byte_idx);

              إذا (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (پای_ټکی - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              وإلا
                {
                  idx_t n = MIN (پای_ټکی - p, current_rp->hi - byte_idx);
                  write_selected_توکی (&print_delimiter,
                                       is_range_پیل_شاخص (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += پای_ټکی - کرښه;
          إذا (کرښه_پای_ټکی)
            {
              processed++;
              reset_توکی_کرښه (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

ساكن فراغ
cut_characters_ډېرپېښ (FILE *بهیر, bool byte_ډېرپېښ)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  ساكن حرف بایټونه_دننه[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, بایټونه_دننه, حجم بایټونه_دننه, بهیر);

  طالما (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      إذا (idx && ډګر_selection_exhausted (idx))
        {
          idx_t شته = mbbuf_topup (&mbbuf);
          حرف *p = mbbuf.بفر + mbbuf.دننه_واټن;
          حرف *پای_ټکی = پلټنه_بایټونه (p, کرښه_delim, شته);
          mbbuf_advance (&mbbuf, پای_ټکی ? پای_ټکی - p : شته);
          إذا (!پای_ټکی && شته)
            استمر;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      إذا (!byte_ډېرپېښ && utf8 && idx + 1 < current_rp->lo)
        {
          حرف ثابت *p = mbbuf.بفر + mbbuf.دننه_واټن;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          حرف ثابت *پای_ټکی = پلټنه_بایټونه (p, کرښه_delim, n);
          إذا (پای_ټکی)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, پای_ټکی - p + 1);
              reset_توکی_کرښه (&idx, &print_delimiter);
              استمر;
            }

          idx += u8_شمېر (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      إذا (g.ch == کرښه_delim)
        reset_توکی_کرښه (&idx, &print_delimiter);
      وإلا إذا (g.ch == MBBUF_EOF)
        {
          write_pending_کرښه_delim (idx);
          اكسر;
        }
      وإلا إذا (byte_ډېرپېښ)
        {
          bool لومړی_selected_is_range_پیل = false;
          bool کتل_شوی_selected = false;
          bool suffix_selected = true;

          لكل (idx_t i = 0; i < g.len; i++)
            {
              بل_توکی (&idx);
              إذا (print_kth (idx))
                {
                  إذا (!کتل_شوی_selected)
                    {
                      کتل_شوی_selected = true;
                      لومړی_selected_is_range_پیل
                        = is_range_پیل_شاخص (idx);
                    }
                }
              وإلا إذا (کتل_شوی_selected)
                suffix_selected = false;
            }

          إذا (کتل_شوی_selected && suffix_selected)
            write_selected_توکی (&print_delimiter,لومړی_selected_is_range_پیل,
                                 mbbuf_char_دننه_واټن (&mbbuf, g), g.len);
        }
      وإلا
        {
          بل_توکی (&idx);
          إذا (print_kth (idx))
            write_selected_توکی (&print_delimiter,
                                 is_range_پیل_شاخص (idx),
                                 mbbuf_char_دننه_واټن (&mbbuf, g), g.len);
        }
    }
}

ساكن فراغ
cut_بایټونه_no_وېشل (FILE *بهیر)
{
  cut_characters_ډېرپېښ (بهیر, true);
}

ساكن فراغ
cut_characters (FILE *بهیر)
{
  cut_characters_ډېرپېښ (بهیر, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

ساكن فراغ
cut_fields_mb_any (FILE *بهیر, bool whitespace_ډېرپېښ)
{
  ساكن حرف بایټونه_دننه[IO_BUFSIZE];
  mbbuf_t mbbuf;
  بنية mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_ډېرپېښ,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_کرښه_پیل = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t ډګر_idx = 1;
  bool وموندل_any_selected_ډګر = false;
  bool بفر_لومړی_ډګر;
  bool have_pending_کرښه = false;
  bool write_ډګر;
  idx_t ډګر_1_n_بایټونه = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, بایټونه_دننه, حجم بایټونه_دننه, بهیر);

  بفر_لومړی_ډګر = (suppress_non_delimited ^ !print_kth (1));
  write_ډګر = begin_ډګر_output (ډګر_idx, بفر_لومړی_ډګر,
                                    &وموندل_any_selected_ډګر);

  طالما (true)
    {
      تعداد ډګر_terminator terminator;
      إذا (!بفر_لومړی_ډګر || ډګر_idx != 1)
        terminator = لټون_mb_ډګر (&mbbuf, &parser, &have_pending_کرښه,
                                    write_ډګر, عدم);
      وإلا
        terminator = لټون_mb_ډګر (&mbbuf, &parser, &have_pending_کرښه,
                                    false, &ډګر_1_n_بایټونه);

      إذا (terminator == ډګر_DELIMITER)
        {
          handle_ډګر_delimiter (&ډګر_idx, بفر_لومړی_ډګر,
                                  &ډګر_1_n_بایټونه,
                                  &وموندل_any_selected_ډګر, &write_ډګر);
          إذا (! ډګر_selection_exhausted (ډګر_idx))
            استمر;

          terminator = لټون_mb_کرښه_پای_ټکی (&mbbuf, &parser,
                                         &have_pending_کرښه);
        }

      إذا (terminator == ډګر_EOF && !have_pending_کرښه)
        اكسر;

      پای_current_کرښه (ډګر_idx, بفر_لومړی_ډګر,
                           &ډګر_1_n_بایټونه, وموندل_any_selected_ډګر,
                           terminator == ډګر_کرښه_DELIMITER);
      إذا (terminator == ډګر_EOF)
        اكسر;

      reset_ډګر_کرښه (&ډګر_idx, &وموندل_any_selected_ډګر,
                        &have_pending_کرښه, &parser);
      write_ډګر = begin_ډګر_output (ډګر_idx, بفر_لومړی_ډګر,
                                        &وموندل_any_selected_ډګر);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

ساكن فراغ
cut_fields_mb (FILE *بهیر)
{
  cut_fields_mb_any (بهیر, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

ساكن فراغ
cut_fields_bytesearch (FILE *بهیر)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  ساكن حرف بایټونه_دننه[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t ډګر_idx = 1;
  bool وموندل_any_selected_ډګر = false;
  bool have_pending_کرښه = false;
  bool skip_blank_پرلپسې = false;
  bool pending_کرښه_delim_ډګر = false;
  bool write_ډګر;
  idx_t ډګر_1_n_بایټونه = 0;

  current_rp = frp;
  bool بفر_لومړی_ډګر = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, بایټونه_دننه, حجم بایټونه_دننه - 1, بهیر);
  write_ډګر = begin_ډګر_output (ډګر_idx, بفر_لومړی_ډګر,
                                    &وموندل_any_selected_ډګر);

  بنية bytesearch_context پلټنه = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&پلټنه);

  طالما (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      پلټنه.at_eof = mbbuf.eof;
      پلټنه.کرښه_پای_ټکی_معلوم = false;

      إذا (pending_کرښه_delim_ډګر)
        {
          pending_کرښه_delim_ډګر = false;

          إذا (n_avail == 0)
            {
              إذا (handle_ډګر_1 (ډګر_idx, بفر_لومړی_ډګر,
                                  &ډګر_1_n_بایټونه))
                بل_توکی (&ډګر_idx);
              maybe_write_کرښه_delim (وموندل_any_selected_ډګر, ډګر_idx);
              أرجع;
            }

          handle_ډګر_delimiter (&ډګر_idx, بفر_لومړی_ډګر,
                                  &ډګر_1_n_بایټونه,
                                  &وموندل_any_selected_ډګر, &write_ډګر);
          إذا (whitespace_delimited)
            skip_blank_پرلپسې = true;
        }

      إذا (n_avail == 0)
        اكسر;

      حرف *chunk = mbbuf.بفر + mbbuf.دننه_واټن;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      إذا (ډګر_idx == 1
          && (!whitespace_delimited || !ډګر_selection_exhausted (ډګر_idx))
          && !ډګر_delim_is_کرښه_delim ()
          && !(whitespace_delimited
               ? پیدا_کړه_c_blank (chunk, n_avail)
               : پیدا_کړه_ډګر_delim (chunk, n_avail)))
        {
          حرف *وروستی_کرښه_delim = پلټنه.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, کرښه_delim, n_avail);
          إذا (وروستی_کرښه_delim)
            {
              idx_t n = وروستی_کرښه_delim - chunk + 1;
              إذا (! suppress_non_delimited)
                {
                  write_بایټونه (ډګر_1_بفر, ډګر_1_n_بایټونه);
                  write_بایټونه (chunk, n);
                  إذا (پلټنه.at_eof && chunk[n - 1] != کرښه_delim)
                    write_کرښه_delim ();
                }
              ډګر_1_n_بایټونه = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_کرښه = false;
              إذا (پلټنه.at_eof)
                أرجع;
              استمر;
            }
        }

      طالما (processed < n_avail)
        {
          حرف *terminator = عدم;

          إذا (skip_blank_پرلپسې)
            {
              طالما (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              إذا (processed == n_avail)
                اكسر;
              skip_blank_پرلپسې = false;
            }

          إذا (ډګر_selection_exhausted (ډګر_idx))
            {
              إذا (ډګر_delim_is_کرښه_delim ())
                {
                  maybe_write_کرښه_delim (وموندل_any_selected_ډګر,
                                          ډګر_idx);
                  mbbuf_advance (&mbbuf, processed);
                  أرجع;
                }
              پلټنه.ډېرپېښ_2 = BYTESEARCH_کرښه_ONLY;
              تعداد ډګر_terminator terminator_ډول
                = پیدا_کړه_ډګر_terminator (chunk + processed, n_avail - processed,
                                         &پلټنه, &terminator);
              إذا (terminator_ډول == ډګر_کرښه_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_کرښه_delim (وموندل_any_selected_ډګر,
                                          ډګر_idx);
                  اذهب reset_کرښه;
                }
              processed = n_avail;
              اكسر;
            }

          تعداد ډګر_terminator terminator_ډول
            = پیدا_کړه_ډګر_terminator (chunk + processed, n_avail - processed,
                                     &پلټنه, &terminator);
          idx_t ډګر_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          إذا (terminator_ډول == ډګر_معلومات
              && !پلټنه.at_eof
              && !whitespace_delimited
              && !ډګر_delim_is_کرښه_delim ())
            ډګر_len -= ډګر_delim_overlap (chunk + processed, ډګر_len);

          إذا (ډګر_len || terminator)
            have_pending_کرښه = true;

          /* Output field content.  */
          إذا (ډګر_idx == 1 && بفر_لومړی_ډګر)
            append_ډګر_1_chunk (chunk + processed, ډګر_len,
                                  &ډګر_1_n_بایټونه);
          وإلا إذا (write_ډګر)
            write_بایټونه (chunk + processed, ډګر_len);
          processed += ډګر_len;

          إذا (terminator_ډول == ډګر_معلومات)
            اكسر;

          إذا (terminator_ډول == ډګر_DELIMITER)
            {
              إذا (ډګر_delim_is_کرښه_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_کرښه_delim_ډګر = true;
                  اكسر;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_ډګر_delimiter (&ډګر_idx, بفر_لومړی_ډګر,
                                      &ډګر_1_n_بایټونه,
                                      &وموندل_any_selected_ډګر, &write_ډګر);
              إذا (whitespace_delimited)
                skip_blank_پرلپسې = true;
            }
          وإلا
            {
              affirm (terminator_ډول == ډګر_کرښه_DELIMITER);
              processed++;
              پای_current_کرښه (ډګر_idx, بفر_لومړی_ډګر,
                                   &ډګر_1_n_بایټونه,
                                   وموندل_any_selected_ډګر, true);

            reset_کرښه:
              ډګر_idx = 1;
              current_rp = frp;
              وموندل_any_selected_ډګر = false;
              have_pending_کرښه = false;
              bytesearch_context_reset (&پلټنه);
              write_ډګر = begin_ډګر_output (ډګر_idx, بفر_لومړی_ډګر,
                                                &وموندل_any_selected_ډګر);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  إذا (!have_pending_کرښه)
    أرجع;

  پای_current_کرښه (ډګر_idx, بفر_لومړی_ډګر, &ډګر_1_n_بایټونه,
                       وموندل_any_selected_ډګر, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

ساكن فراغ
cut_fields_ws (FILE *بهیر)
{
  إذا (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (بهیر);
  وإلا
    cut_fields_mb_any (بهیر, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

ساكن bool
cut_file (حرف ثابت *file, فراغ (*cut_بهیر) (FILE *))
{
  FILE *بهیر;

  إذا (streq (file, "-"))
    {
      have_read_stdin = true;
      بهیر = stdin;
      assume (بهیر);  /* Pacify GCC bug#109613.  */
    }
  وإلا
    {
      بهیر = fopen (file, "r");
      إذا (بهیر == عدم)
        {
          error (0, errno, "%s", quotef (file));
          أرجع false;
        }
    }

  fadvise (بهیر, FADVISE_SEQUENTIAL);

  cut_بهیر (بهیر);

  صحيح err = errno;
  إذا (!ferror (بهیر))
    err = 0;
  إذا (streq (file, "-"))
    clearerr (بهیر);		/* Also clear EOF.  */
  وإلا إذا (fclose (بهیر) == EOF)
    err = errno;
  إذا (err)
    {
      error (0, err, "%s", quotef (file));
      أرجع false;
    }
  أرجع true;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  صحيح optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  حرف *spec_list_string = عدم;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, عدم))
         != -1)
    {
      بدل (optc)
        {
        حالة 'b':
          cut_ډېرپېښ = CUT_ډېرپېښ_بایټونه;
          FALLTHROUGH;
        حالة 'c':
          إذا (optc == 'c')
            cut_ډېرپېښ = CUT_ډېرپېښ_CHARACTERS;
          FALLTHROUGH;
        حالة 'f':
        حالة 'F':
          إذا (optc == 'f' || optc == 'F')
            cut_ډېرپېښ = CUT_ډېرپېښ_FIELDS;
          إذا (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          إذا (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          اكسر;

        حالة 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            إذا (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_بایټونه (delim_بایټونه, optarg, g.len + 1);
            delim_specified = true;
            اكسر;
          }

        حالة 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_دلیلونه,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          اكسر;

        حالة 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_اوږدتیا = (optarg[0] == '\0'
                                     ? 1 : مزي_اوږدوالی (optarg));
          output_delimiter_string = optarg;
          اكسر;

        حالة 'n':
          no_وېشل = true;
          اكسر;

        حالة 's':
          suppress_non_delimited = true;
          اكسر;

        حالة 'z':
          کرښه_delim = '\0';
          اكسر;

        حالة COMPLEMENT_OPTION:
          complement = true;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  إذا (cut_ډېرپېښ == CUT_ډېرپېښ_بایټونه || cut_ډېرپېښ == CUT_ډېرپېښ_CHARACTERS)
    {
      إذا (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an ننوت delimiter makes sense\n\
\tonly when operating on fields"));

      إذا (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited کرښې makes sense\n\
\tonly when operating on fields"));
    }

  إذا (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  إذا (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  سېټ_fields (spec_list_string,
              (((cut_ډېرپېښ == CUT_ډېرپېښ_بایټونه
                 || cut_ډېرپېښ == CUT_ډېرپېښ_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  إذا (!delim_specified)
    {
      delim_بایټونه[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  إذا (output_delimiter_string == عدم)
    {
      output_delimiter_string = output_delimiter_default;
      إذا (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_اوږدتیا = 1;
        }
      وإلا
        {
          copy_بایټونه (output_delimiter_default, delim_بایټونه, delim_mcel.len);
          output_delimiter_اوږدتیا = delim_mcel.len;
        }
    }

  فراغ (*cut_بهیر) (FILE *) = عدم;
  بدل (cut_ډېرپېښ)
    {
    حالة CUT_ډېرپېښ_بایټونه:
      cut_بهیر = MB_CUR_MAX <= 1 || !no_وېشل
                   ? cut_بایټونه : cut_بایټونه_no_وېشل;
      اكسر;

    حالة CUT_ډېرپېښ_CHARACTERS:
      cut_بهیر = MB_CUR_MAX <= 1 ? cut_بایټونه : cut_characters;
      اكسر;

    حالة CUT_ډېرپېښ_FIELDS:
      cut_بهیر = whitespace_delimited ? cut_fields_ws
                   : bytesearch_ډګر_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      اكسر;

    حالة CUT_ډېرپېښ_NONE:
      affirm (false);
    }
  affirm (cut_بهیر);
  إذا (optind == argc)
    ok = cut_file ("-", cut_بهیر);
  وإلا
    لكل (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_بهیر);


  إذا (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
