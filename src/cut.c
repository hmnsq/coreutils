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
#परिभाषय क्रमादेशः_NAME "cut"

#परिभाषय AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#परिभाषय FATAL_ERROR(सन्देशः)						\
  कुरु									\
    {									\
      error (0, 0, (सन्देशः));						\
      usage (EXIT_FAILURE);						\
    }									\
  यावत् (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#परिभाषय SMALL_BYTE_देहली 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
स्थिर संरचना क्षेत्रम्_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
स्थिर वर्ण *क्षेत्रम्_1_बफर;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
स्थिर idx_t क्षेत्रम्_1_bufsize;

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
स्थिर वर्ण delim_अष्टकानि[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
स्थिर अचिह्नित वर्ण पङ्क्तिः_delim = '\n';

/* The length of output_delimiter_string.  */
स्थिर size_t output_delimiter_दीर्घता;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
स्थिर वर्ण *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
स्थिर वर्ण output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
स्थिर bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
स्थिर bool no_विभज;

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

स्थिर वर्ण ध्रुव *ध्रुव whitespace_option_तर्काः[] =
{
  "trimmed", रिक्त
};

स्थिर गणना whitespace_option ध्रुव whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_तर्काः, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
स्थिर गणना
{
  CUT_बहुलकम्_NONE,
  CUT_बहुलकम्_अष्टकानि,
  CUT_बहुलकम्_CHARACTERS,
  CUT_बहुलकम्_FIELDS
} cut_बहुलकम् = CUT_बहुलकम्_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"bytes", required_तर्कः, रिक्त, 'b'},
  {"characters", required_तर्कः, रिक्त, 'c'},
  {"fields", required_तर्कः, रिक्त, 'f'},
  {"delimiter", required_तर्कः, रिक्त, 'd'},
  {"no-partial", no_तर्कः, रिक्त, 'n'},
  {"whitespace-delimited", optional_तर्कः, रिक्त, 'w'},
  {"only-delimited", no_तर्कः, रिक्त, 's'},
  {"output-delimiter", required_तर्कः, रिक्त, 'O'},
  {"complement", no_तर्कः, रिक्त, COMPLEMENT_OPTION},
  {"zero-terminated", no_तर्कः, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              क्रमादेशः_name);
      fputs (_("\
Print selected parts सम्बन्धि पङ्क्तयः from each FILE गन्तव्यम् standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --अष्टकानि=LIST\n\
         select only these byte positions\n\
"));
      oputs (_("\
  -c, --characters=LIST\n\
         select only these character positions\n\
"));
      oputs (_("\
      --complement\n\
         complement the समुच्चयः_2 सम्बन्धि selected अष्टकानि, characters or fields\n\
"));
      oputs (_("\
  -d, --delimiter=DELIM\n\
         use DELIM instead सम्बन्धि TAB प्रति क्षेत्रम् delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any पङ्क्तिः that अन्तर्भवति\n\
         no delimiter character, unless the -s option is specified\n\
"));
      oputs (_("\
  -F LIST\n\
         like -f, but also उपपादयति -w and -O ' '\n\
"));
      oputs (_("\
  -n, --no-partial\n\
         with -b, don't output partial multi-byte characters\n\
"));
      oputs (_("\
  -O, --output-delimiter=STRING\n\
         use STRING as the output delimiter;\n\
         the स्वभाव is गन्तव्यम् use the निवेशः delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         कुरु not print पङ्क्तयः not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a सततम्_2 सम्बन्धि blank characters as the क्षेत्रम् delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one सम्बन्धि -b, -c, -f or -F.  Each LIST is made ऊर्ध्वम् सम्बन्धि one\n\
range, or many ranges separated by commas.  Selected निवेशः is written\n\
अन्तर् the same क्रमः that it is पठ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one सम्बन्धि:\n\
\n\
  N     N'th byte, character or क्षेत्रम्, counted from 1\n\
  N-    from N'th byte, character or क्षेत्रम्, गन्तव्यम् अन्तः सम्बन्धि पङ्क्तिः\n\
  N-M   from N'th to M'th (included) byte, character or क्षेत्रम्\n\
  -M    from first गन्तव्यम् M'th (included) byte, character or क्षेत्रम्\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

स्थिर अन्तःस्थ शून्य
अग्र_item (uintmax_t *item_idx)
{
  (*item_idx)++;
  यदि ((*item_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

स्थिर अन्तःस्थ bool
print_kth (uintmax_t k)
{
  प्रत्यागम current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

स्थिर अन्तःस्थ bool
is_range_start_सूचकांक (uintmax_t k)
{
  प्रत्यागम k == current_rp->lo;
}

स्थिर अन्तःस्थ bool
क्षेत्रम्_delim_is_पङ्क्तिः_delim (शून्य)
{
  प्रत्यागम delim_mcel.len == 1 && delim_अष्टकानि[0] == पङ्क्तिः_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

स्थिर bool
mcel_isblank (mcel_t g)
{
  प्रत्यागम (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

स्थिर अन्तःस्थ bool
bytesearch_क्षेत्रम्_delim_ok (शून्य)
{
  अचिह्नित वर्ण delim_0 = delim_अष्टकानि[0];

  प्रत्यागम (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


गणना क्षेत्रम्_terminator
{
  क्षेत्रम्_दत्त,
  क्षेत्रम्_DELIMITER,
  क्षेत्रम्_पङ्क्तिः_DELIMITER,
  क्षेत्रम्_EOF
};

गणना bytesearch_बहुलकम्
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_पङ्क्तिः_ONLY
};

संरचना bytesearch_context
{
  गणना bytesearch_बहुलकम् बहुलकम्_2;
  bool blank_delimited;
  bool at_eof;
  वर्ण *पङ्क्तिः_अन्तः;
  bool पङ्क्तिः_अन्तः_ज्ञातम्;
};

स्थिर अन्तःस्थ शून्य
bytesearch_context_reset (संरचना bytesearch_context *ctx)
{
  ctx->बहुलकम्_2 = BYTESEARCH_FIELDS;
  ctx->पङ्क्तिः_अन्तः = रिक्त;
  ctx->पङ्क्तिः_अन्तः_ज्ञातम् = false;
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
  यदि (*have_saved)
    {
      *have_saved = false;
      प्रत्यागम *saved_g;
    }
  प्रत्यागम mbbuf_get_char (mbbuf);
}

स्थिर अन्तःस्थ गणना क्षेत्रम्_terminator
skip_whitespace_सततम् (mbbuf_t *mbuf, संरचना mbfield_parser *parser,
                     bool *have_pending_पङ्क्तिः,
                     bool have_initial_whitespace)
{
  mcel_t g;

  कुरु
    {
      g = mbbuf_get_char (mbuf);
      यदि (g.ch != MBBUF_EOF)
        *have_pending_पङ्क्तिः = true;
    }
  यावत् (g.ch != MBBUF_EOF && g.ch != पङ्क्तिः_delim && mcel_isblank (g));

  bool trim_start = parser->trim_outer_whitespace && parser->at_पङ्क्तिः_start;

  यदि (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == पङ्क्तिः_delim))
    {
      प्रत्यागम g.ch == MBBUF_EOF ? क्षेत्रम्_EOF : क्षेत्रम्_पङ्क्तिः_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  प्रत्यागम trim_start && !have_initial_whitespace ? क्षेत्रम्_दत्त : क्षेत्रम्_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

स्थिर शून्य
write_अष्टकानि (वर्ण ध्रुव *buf, size_t n_अष्टकानि)
{
  यदि (n_अष्टकानि <= SMALL_BYTE_देहली)
    {
      प्रति (size_t i = 0; i < n_अष्टकानि; i++)
        यदि (अक्षरं_लिख (buf[i]) < 0)
          write_error ();
      प्रत्यागम;
    }

  यदि (fwrite (buf, परिमाण (वर्ण), n_अष्टकानि, stdout) != n_अष्टकानि)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

स्थिर अन्तःस्थ शून्य
copy_अष्टकानि (वर्ण *dst, वर्ण ध्रुव *src, size_t n_अष्टकानि)
{
  यदि (n_अष्टकानि <= SMALL_BYTE_देहली)
    {
      प्रति (size_t i = 0; i < n_अष्टकानि; i++)
        dst[i] = src[i];
      प्रत्यागम;
    }

  स्मृतिं_प्रतिलिख (dst, src, n_अष्टकानि);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

स्थिर अन्तःस्थ शून्य*
अन्वेषणम्_अष्टकानि (वर्ण ध्रुव *buf, पूर्णाङ्क c, size_t n_अष्टकानि)
{
  यदि (n_अष्टकानि != 0 && गन्तव्यम्_uchar (buf[0]) == c)
    प्रत्यागम (वर्ण *) buf;
  यदि (1 < n_अष्टकानि && गन्तव्यम्_uchar (buf[1]) == c)
    प्रत्यागम (वर्ण *) buf + 1;

  प्रत्यागम 2 < n_अष्टकानि ? (वर्ण *) memchr (buf + 2, c, n_अष्टकानि - 2) : रिक्त;
}

स्थिर अन्तःस्थ शून्य
write_पङ्क्तिः_delim (शून्य)
{
  यदि (अक्षरं_लिख (पङ्क्तिः_delim) < 0)
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
  यदि (item_idx > 0)
    write_पङ्क्तिः_delim ();
}

स्थिर अन्तःस्थ शून्य
write_selected_item (bool *print_delimiter, bool range_start,
                     वर्ण ध्रुव *buf, size_t n_अष्टकानि)
{
  यदि (output_delimiter_string != output_delimiter_default)
    {
      यदि (*print_delimiter && range_start)
        write_अष्टकानि (output_delimiter_string, output_delimiter_दीर्घता);
      *print_delimiter = true;
    }

  write_अष्टकानि (buf, n_अष्टकानि);
}

स्थिर अन्तःस्थ शून्य
append_क्षेत्रम्_1_chunk (वर्ण ध्रुव *buf, idx_t len, idx_t *n_अष्टकानि)
{
  यदि (क्षेत्रम्_1_bufsize - *n_अष्टकानि < len)
    {
      क्षेत्रम्_1_बफर = xpalloc (क्षेत्रम्_1_बफर, &क्षेत्रम्_1_bufsize,
                                len, -1, परिमाण *क्षेत्रम्_1_बफर);
    }

  copy_अष्टकानि (क्षेत्रम्_1_बफर + *n_अष्टकानि, buf, len);
  *n_अष्टकानि += len;
}

स्थिर अन्तःस्थ शून्य
append_क्षेत्रम्_1_अष्टकानि (mbbuf_t *mbbuf, mcel_t g, idx_t *n_अष्टकानि)
{
  append_क्षेत्रम्_1_chunk (mbbuf_char_विस्थापनम् (mbbuf, g), g.len, n_अष्टकानि);
}

स्थिर अन्तःस्थ गणना क्षेत्रम्_terminator
अवलोकनम्_mb_blank_क्षेत्रम् (mbbuf_t *mbbuf, संरचना mbfield_parser *parser,
                     bool *have_pending_पङ्क्तिः, bool write_क्षेत्रम्,
                     idx_t *n_अष्टकानि)
{
  यदि (parser->trim_outer_whitespace && parser->at_पङ्क्तिः_start)
    {
      गणना क्षेत्रम्_terminator terminator
        = skip_whitespace_सततम् (mbbuf, parser, have_pending_पङ्क्तिः, false);
      यदि (terminator != क्षेत्रम्_दत्त)
        प्रत्यागम terminator;
    }

  parser->at_पङ्क्तिः_start = false;

  यावत् (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      यदि (g.ch == MBBUF_EOF)
        प्रत्यागम क्षेत्रम्_EOF;

      *have_pending_पङ्क्तिः = true;

      यदि (g.ch == पङ्क्तिः_delim)
        प्रत्यागम क्षेत्रम्_पङ्क्तिः_DELIMITER;

      यदि (mcel_isblank (g))
        प्रत्यागम skip_whitespace_सततम् (mbbuf, parser, have_pending_पङ्क्तिः, true);

      यदि (n_अष्टकानि)
        append_क्षेत्रम्_1_अष्टकानि (mbbuf, g, n_अष्टकानि);
      अन्यथा यदि (write_क्षेत्रम्)
        write_अष्टकानि (mbbuf_char_विस्थापनम् (mbbuf, g), g.len);
    }
}

स्थिर अन्तःस्थ गणना क्षेत्रम्_terminator
अवलोकनम्_mb_delim_क्षेत्रम् (mbbuf_t *mbbuf, bool *have_pending_पङ्क्तिः,
                     bool write_क्षेत्रम्, idx_t *n_अष्टकानि)
{
  यावत् (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      यदि (g.ch == MBBUF_EOF)
        प्रत्यागम क्षेत्रम्_EOF;

      *have_pending_पङ्क्तिः = true;

      यदि (g.ch == पङ्क्तिः_delim)
        प्रत्यागम क्षेत्रम्_पङ्क्तिः_DELIMITER;

      यदि (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        प्रत्यागम क्षेत्रम्_DELIMITER;

      यदि (n_अष्टकानि)
        append_क्षेत्रम्_1_अष्टकानि (mbbuf, g, n_अष्टकानि);
      अन्यथा यदि (write_क्षेत्रम्)
        write_अष्टकानि (mbbuf_char_विस्थापनम् (mbbuf, g), g.len);
    }
}

स्थिर अन्तःस्थ गणना क्षेत्रम्_terminator
अवलोकनम्_mb_क्षेत्रम् (mbbuf_t *mbbuf, संरचना mbfield_parser *parser,
               bool *have_pending_पङ्क्तिः, bool write_क्षेत्रम्, idx_t *n_अष्टकानि)
{
  प्रत्यागम (parser->whitespace_delimited
          ? अवलोकनम्_mb_blank_क्षेत्रम् (mbbuf, parser, have_pending_पङ्क्तिः,
                                 write_क्षेत्रम्, n_अष्टकानि)
          : अवलोकनम्_mb_delim_क्षेत्रम् (mbbuf, have_pending_पङ्क्तिः, write_क्षेत्रम्,
                                 n_अष्टकानि));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

स्थिर गणना क्षेत्रम्_terminator
अवलोकनम्_mb_पङ्क्तिः_अन्तः (mbbuf_t *mbbuf, संरचना mbfield_parser *parser,
                  bool *have_pending_पङ्क्तिः)
{
  यदि (parser->have_saved)
    {
      parser->have_saved = false;
      यदि (parser->saved_g.ch == MBBUF_EOF)
        प्रत्यागम क्षेत्रम्_EOF;

      *have_pending_पङ्क्तिः = true;
      यदि (parser->saved_g.ch == पङ्क्तिः_delim)
        प्रत्यागम क्षेत्रम्_पङ्क्तिः_DELIMITER;
    }

  यावत् (true)
    {
      idx_t उपलब्धम् = mbbuf_topup (mbbuf);
      यदि (उपलब्धम् == 0)
        प्रत्यागम क्षेत्रम्_EOF;

      वर्ण *बफर = mbbuf->बफर + mbbuf->विस्थापनम्;
      वर्ण *पङ्क्तिः_अन्तः = अन्वेषणम्_अष्टकानि (बफर, पङ्क्तिः_delim, उपलब्धम्);
      idx_t n = पङ्क्तिः_अन्तः ? पङ्क्तिः_अन्तः - बफर + 1 : उपलब्धम्;
      mbbuf_advance (mbbuf, n);
      *have_pending_पङ्क्तिः = true;

      यदि (पङ्क्तिः_अन्तः)
        प्रत्यागम क्षेत्रम्_पङ्क्तिः_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_विशुद्धम्
स्थिर वर्ण *
find_क्षेत्रम्_delim (वर्ण *buf, size_t len)
{
  यदि (len < delim_mcel.len)
    प्रत्यागम रिक्त;

#यदि ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  प्रत्यागम memmem (buf, len, delim_अष्टकानि, delim_mcel.len);
#अन्यथा
  यदि (delim_mcel.len == 1)
    प्रत्यागम memchr (buf, delim_अष्टकानि[0], len);

  वर्ण ध्रुव *p = buf;
  वर्ण ध्रुव *अन्तः = buf + len;

  वर्ण saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  यावत् (p < अन्तः)
    {
      वर्ण ध्रुव *nul = memchr (p, '\0', अन्तः - p);
      यदि (!nul)
        {
          वर्ण ध्रुव *मेलनम् = वाक्ये_अन्विष (p, delim_अष्टकानि);
          buf[len] = saved;
          प्रत्यागम (वर्ण *) मेलनम्;
        }

      यदि (p < nul)
        {
          वर्ण ध्रुव *मेलनम् = वाक्ये_अन्विष (p, delim_अष्टकानि);
          यदि (मेलनम्)
            {
              buf[len] = saved;
              प्रत्यागम (वर्ण *) मेलनम्;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  प्रत्यागम रिक्त;
#यदि_अन्तः
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_विशुद्धम्
स्थिर वर्ण *
find_c_blank (वर्ण *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  प्रति (idx_t i = 0; i < n; i++)
    यदि (c_isblank (buf[i]))
      प्रत्यागम buf + i;

  प्रत्यागम memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_विशुद्धम्
स्थिर idx_t
क्षेत्रम्_delim_overlap (वर्ण ध्रुव *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  यावत् (0 < overlap)
    {
      यदि (memeq (buf + len - overlap, delim_अष्टकानि, overlap))
        प्रत्यागम overlap;
      overlap--;
    }

  प्रत्यागम 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

स्थिर अन्तःस्थ गणना क्षेत्रम्_terminator
find_क्षेत्रम्_terminator (वर्ण *buf, idx_t len,
                       संरचना bytesearch_context *ctx, वर्ण **terminator)
{
  यदि (ctx->बहुलकम्_2 == BYTESEARCH_पङ्क्तिः_ONLY)
    {
      यदि (!ctx->पङ्क्तिः_अन्तः_ज्ञातम्)
        {
          ctx->पङ्क्तिः_अन्तः = memchr (buf, पङ्क्तिः_delim, len);
          ctx->पङ्क्तिः_अन्तः_ज्ञातम् = true;
        }

      *terminator = ctx->पङ्क्तिः_अन्तः;
      प्रत्यागम *terminator ? क्षेत्रम्_पङ्क्तिः_DELIMITER : क्षेत्रम्_दत्त;
    }

  यदि (क्षेत्रम्_delim_is_पङ्क्तिः_delim ())
    {
      *terminator = memchr (buf, पङ्क्तिः_delim, len);
      प्रत्यागम *terminator ? क्षेत्रम्_DELIMITER : क्षेत्रम्_दत्त;
    }

  यदि (!ctx->पङ्क्तिः_अन्तः_ज्ञातम्)
    {
      ctx->पङ्क्तिः_अन्तः = memchr (buf, पङ्क्तिः_delim, len);
      ctx->पङ्क्तिः_अन्तः_ज्ञातम् = true;
    }

  idx_t क्षेत्रम्_len = ctx->पङ्क्तिः_अन्तः ? ctx->पङ्क्तिः_अन्तः - buf : len;

  वर्ण *क्षेत्रम्_अन्तः = (ctx->blank_delimited
                     ? find_c_blank (buf, क्षेत्रम्_len)
                     : find_क्षेत्रम्_delim (buf, क्षेत्रम्_len));

  यदि (क्षेत्रम्_अन्तः)
    {
      *terminator = क्षेत्रम्_अन्तः;
      प्रत्यागम क्षेत्रम्_DELIMITER;
    }

  *terminator = ctx->पङ्क्तिः_अन्तः;
  प्रत्यागम ctx->पङ्क्तिः_अन्तः ? क्षेत्रम्_पङ्क्तिः_DELIMITER : क्षेत्रम्_दत्त;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

स्थिर अन्तःस्थ शून्य
maybe_write_पङ्क्तिः_delim (bool प्राप्तम्_any_selected_क्षेत्रम्, uintmax_t क्षेत्रम्_idx)
{
  यदि (प्राप्तम्_any_selected_क्षेत्रम्
      || !(suppress_non_delimited && क्षेत्रम्_idx == 1))
    write_पङ्क्तिः_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

स्थिर अन्तःस्थ bool
begin_क्षेत्रम्_output (uintmax_t क्षेत्रम्_idx, bool बफर_first_क्षेत्रम्,
                    bool *प्राप्तम्_any_selected_क्षेत्रम्)
{
  bool write_क्षेत्रम् = print_kth (क्षेत्रम्_idx);

  यदि (write_क्षेत्रम् && ! (क्षेत्रम्_idx == 1 && बफर_first_क्षेत्रम्))
    {
      यदि (*प्राप्तम्_any_selected_क्षेत्रम्)
        write_अष्टकानि (output_delimiter_string, output_delimiter_दीर्घता);
      *प्राप्तम्_any_selected_क्षेत्रम् = true;
    }

  प्रत्यागम write_क्षेत्रम्;
}

स्थिर अन्तःस्थ bool
handle_क्षेत्रम्_1 (uintmax_t क्षेत्रम्_idx, bool बफर_first_क्षेत्रम्,
                idx_t *क्षेत्रम्_1_n_अष्टकानि)
{
  bool क्षेत्रम्_1_selected = false;

  यदि (क्षेत्रम्_idx == 1 && बफर_first_क्षेत्रम्)
    {
      यदि ((क्षेत्रम्_1_selected = print_kth (1)))
        write_अष्टकानि (क्षेत्रम्_1_बफर, *क्षेत्रम्_1_n_अष्टकानि);
      *क्षेत्रम्_1_n_अष्टकानि = 0;
    }

  प्रत्यागम क्षेत्रम्_1_selected;
}

स्थिर अन्तःस्थ शून्य
handle_क्षेत्रम्_delimiter (uintmax_t *क्षेत्रम्_idx, bool बफर_first_क्षेत्रम्,
                        idx_t *क्षेत्रम्_1_n_अष्टकानि,
                        bool *प्राप्तम्_any_selected_क्षेत्रम्, bool *write_क्षेत्रम्)
{
  यदि (handle_क्षेत्रम्_1 (*क्षेत्रम्_idx, बफर_first_क्षेत्रम्, क्षेत्रम्_1_n_अष्टकानि))
    *प्राप्तम्_any_selected_क्षेत्रम् = true;

  अग्र_item (क्षेत्रम्_idx);
  *write_क्षेत्रम् = begin_क्षेत्रम्_output (*क्षेत्रम्_idx, बफर_first_क्षेत्रम्,
                                     प्राप्तम्_any_selected_क्षेत्रम्);
}

स्थिर अन्तःस्थ bool
क्षेत्रम्_selection_exhausted (uintmax_t क्षेत्रम्_idx)
{
  प्रत्यागम !print_kth (क्षेत्रम्_idx) && current_rp->lo == UINTMAX_MAX;
}

स्थिर अन्तःस्थ शून्य
finish_current_पङ्क्तिः (uintmax_t क्षेत्रम्_idx, bool बफर_first_क्षेत्रम्,
                     idx_t *क्षेत्रम्_1_n_अष्टकानि,
                     bool प्राप्तम्_any_selected_क्षेत्रम्, bool पङ्क्तिः_terminated)
{
  यदि (क्षेत्रम्_idx == 1 && बफर_first_क्षेत्रम्)
    {
      यदि (!suppress_non_delimited
          && (पङ्क्तिः_terminated || *क्षेत्रम्_1_n_अष्टकानि != 0))
        {
          write_अष्टकानि (क्षेत्रम्_1_बफर, *क्षेत्रम्_1_n_अष्टकानि);
          write_पङ्क्तिः_delim ();
        }
      *क्षेत्रम्_1_n_अष्टकानि = 0;
    }
  अन्यथा
    maybe_write_पङ्क्तिः_delim (प्राप्तम्_any_selected_क्षेत्रम्, क्षेत्रम्_idx);
}

स्थिर अन्तःस्थ शून्य
sync_byte_selection (uintmax_t byte_idx)
{
  यावत् (current_rp->hi <= byte_idx)
    current_rp++;
}

स्थिर अन्तःस्थ शून्य
reset_क्षेत्रम्_पङ्क्तिः (uintmax_t *क्षेत्रम्_idx, bool *प्राप्तम्_any_selected_क्षेत्रम्,
                  bool *have_pending_पङ्क्तिः, संरचना mbfield_parser *parser)
{
  *क्षेत्रम्_idx = 1;
  current_rp = frp;
  *प्राप्तम्_any_selected_क्षेत्रम् = false;
  *have_pending_पङ्क्तिः = false;
  parser->have_saved = false;
  parser->at_पङ्क्तिः_start = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

स्थिर शून्य
cut_अष्टकानि (FILE *प्रवाहः)
{
  स्थिर वर्ण अष्टकानि_अन्तर्[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  पूर्णाङ्क fd = fileno (प्रवाहः);

  current_rp = frp;

  यावत् (true)
    {
      idx_t उपलब्धम् = पठ (fd, अष्टकानि_अन्तर्, परिमाण अष्टकानि_अन्तर्);
      यदि (उपलब्धम् <= 0)
        {
          यदि (उपलब्धम् < 0)
            fseterr (प्रवाहः);
          write_pending_पङ्क्तिः_delim (byte_idx);
          भङ्ग;
        }

      idx_t processed = 0;

      यावत् (processed < उपलब्धम्)
        {
          वर्ण *पङ्क्तिः = अष्टकानि_अन्तर् + processed;
          idx_t गन्तव्यम्_process = उपलब्धम् - processed;
          वर्ण *पङ्क्तिः_अन्तः = अन्वेषणम्_अष्टकानि (पङ्क्तिः, पङ्क्तिः_delim, गन्तव्यम्_process);
          वर्ण *अन्तः = पङ्क्तिः + (पङ्क्तिः_अन्तः ? पङ्क्तिः_अन्तः - पङ्क्तिः : गन्तव्यम्_process);
          वर्ण *p = पङ्क्तिः;

          यावत् (0 < अन्तः - p)
            {
              sync_byte_selection (byte_idx);

              यदि (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (अन्तः - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              अन्यथा
                {
                  idx_t n = MIN (अन्तः - p, current_rp->hi - byte_idx);
                  write_selected_item (&print_delimiter,
                                       is_range_start_सूचकांक (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += अन्तः - पङ्क्तिः;
          यदि (पङ्क्तिः_अन्तः)
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
cut_characters_बहुलकम् (FILE *प्रवाहः, bool byte_बहुलकम्)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  स्थिर वर्ण अष्टकानि_अन्तर्[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, अष्टकानि_अन्तर्, परिमाण अष्टकानि_अन्तर्, प्रवाहः);

  यावत् (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      यदि (idx && क्षेत्रम्_selection_exhausted (idx))
        {
          idx_t उपलब्धम् = mbbuf_topup (&mbbuf);
          वर्ण *p = mbbuf.बफर + mbbuf.विस्थापनम्;
          वर्ण *अन्तः = अन्वेषणम्_अष्टकानि (p, पङ्क्तिः_delim, उपलब्धम्);
          mbbuf_advance (&mbbuf, अन्तः ? अन्तः - p : उपलब्धम्);
          यदि (!अन्तः && उपलब्धम्)
            अनुवर्त;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      यदि (!byte_बहुलकम् && utf8 && idx + 1 < current_rp->lo)
        {
          वर्ण ध्रुव *p = mbbuf.बफर + mbbuf.विस्थापनम्;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          वर्ण ध्रुव *अन्तः = अन्वेषणम्_अष्टकानि (p, पङ्क्तिः_delim, n);
          यदि (अन्तः)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, अन्तः - p + 1);
              reset_item_पङ्क्तिः (&idx, &print_delimiter);
              अनुवर्त;
            }

          idx += u8_गणनम् (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      यदि (g.ch == पङ्क्तिः_delim)
        reset_item_पङ्क्तिः (&idx, &print_delimiter);
      अन्यथा यदि (g.ch == MBBUF_EOF)
        {
          write_pending_पङ्क्तिः_delim (idx);
          भङ्ग;
        }
      अन्यथा यदि (byte_बहुलकम्)
        {
          bool first_selected_is_range_start = false;
          bool आलोकितम्_selected = false;
          bool suffix_selected = true;

          प्रति (idx_t i = 0; i < g.len; i++)
            {
              अग्र_item (&idx);
              यदि (print_kth (idx))
                {
                  यदि (!आलोकितम्_selected)
                    {
                      आलोकितम्_selected = true;
                      first_selected_is_range_start
                        = is_range_start_सूचकांक (idx);
                    }
                }
              अन्यथा यदि (आलोकितम्_selected)
                suffix_selected = false;
            }

          यदि (आलोकितम्_selected && suffix_selected)
            write_selected_item (&print_delimiter,first_selected_is_range_start,
                                 mbbuf_char_विस्थापनम् (&mbbuf, g), g.len);
        }
      अन्यथा
        {
          अग्र_item (&idx);
          यदि (print_kth (idx))
            write_selected_item (&print_delimiter,
                                 is_range_start_सूचकांक (idx),
                                 mbbuf_char_विस्थापनम् (&mbbuf, g), g.len);
        }
    }
}

स्थिर शून्य
cut_अष्टकानि_no_विभज (FILE *प्रवाहः)
{
  cut_characters_बहुलकम् (प्रवाहः, true);
}

स्थिर शून्य
cut_characters (FILE *प्रवाहः)
{
  cut_characters_बहुलकम् (प्रवाहः, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

स्थिर शून्य
cut_fields_mb_any (FILE *प्रवाहः, bool whitespace_बहुलकम्)
{
  स्थिर वर्ण अष्टकानि_अन्तर्[IO_BUFSIZE];
  mbbuf_t mbbuf;
  संरचना mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_बहुलकम्,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_पङ्क्तिः_start = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t क्षेत्रम्_idx = 1;
  bool प्राप्तम्_any_selected_क्षेत्रम् = false;
  bool बफर_first_क्षेत्रम्;
  bool have_pending_पङ्क्तिः = false;
  bool write_क्षेत्रम्;
  idx_t क्षेत्रम्_1_n_अष्टकानि = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, अष्टकानि_अन्तर्, परिमाण अष्टकानि_अन्तर्, प्रवाहः);

  बफर_first_क्षेत्रम् = (suppress_non_delimited ^ !print_kth (1));
  write_क्षेत्रम् = begin_क्षेत्रम्_output (क्षेत्रम्_idx, बफर_first_क्षेत्रम्,
                                    &प्राप्तम्_any_selected_क्षेत्रम्);

  यावत् (true)
    {
      गणना क्षेत्रम्_terminator terminator;
      यदि (!बफर_first_क्षेत्रम् || क्षेत्रम्_idx != 1)
        terminator = अवलोकनम्_mb_क्षेत्रम् (&mbbuf, &parser, &have_pending_पङ्क्तिः,
                                    write_क्षेत्रम्, रिक्त);
      अन्यथा
        terminator = अवलोकनम्_mb_क्षेत्रम् (&mbbuf, &parser, &have_pending_पङ्क्तिः,
                                    false, &क्षेत्रम्_1_n_अष्टकानि);

      यदि (terminator == क्षेत्रम्_DELIMITER)
        {
          handle_क्षेत्रम्_delimiter (&क्षेत्रम्_idx, बफर_first_क्षेत्रम्,
                                  &क्षेत्रम्_1_n_अष्टकानि,
                                  &प्राप्तम्_any_selected_क्षेत्रम्, &write_क्षेत्रम्);
          यदि (! क्षेत्रम्_selection_exhausted (क्षेत्रम्_idx))
            अनुवर्त;

          terminator = अवलोकनम्_mb_पङ्क्तिः_अन्तः (&mbbuf, &parser,
                                         &have_pending_पङ्क्तिः);
        }

      यदि (terminator == क्षेत्रम्_EOF && !have_pending_पङ्क्तिः)
        भङ्ग;

      finish_current_पङ्क्तिः (क्षेत्रम्_idx, बफर_first_क्षेत्रम्,
                           &क्षेत्रम्_1_n_अष्टकानि, प्राप्तम्_any_selected_क्षेत्रम्,
                           terminator == क्षेत्रम्_पङ्क्तिः_DELIMITER);
      यदि (terminator == क्षेत्रम्_EOF)
        भङ्ग;

      reset_क्षेत्रम्_पङ्क्तिः (&क्षेत्रम्_idx, &प्राप्तम्_any_selected_क्षेत्रम्,
                        &have_pending_पङ्क्तिः, &parser);
      write_क्षेत्रम् = begin_क्षेत्रम्_output (क्षेत्रम्_idx, बफर_first_क्षेत्रम्,
                                        &प्राप्तम्_any_selected_क्षेत्रम्);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

स्थिर शून्य
cut_fields_mb (FILE *प्रवाहः)
{
  cut_fields_mb_any (प्रवाहः, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

स्थिर शून्य
cut_fields_bytesearch (FILE *प्रवाहः)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  स्थिर वर्ण अष्टकानि_अन्तर्[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t क्षेत्रम्_idx = 1;
  bool प्राप्तम्_any_selected_क्षेत्रम् = false;
  bool have_pending_पङ्क्तिः = false;
  bool skip_blank_सततम् = false;
  bool pending_पङ्क्तिः_delim_क्षेत्रम् = false;
  bool write_क्षेत्रम्;
  idx_t क्षेत्रम्_1_n_अष्टकानि = 0;

  current_rp = frp;
  bool बफर_first_क्षेत्रम् = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, अष्टकानि_अन्तर्, परिमाण अष्टकानि_अन्तर् - 1, प्रवाहः);
  write_क्षेत्रम् = begin_क्षेत्रम्_output (क्षेत्रम्_idx, बफर_first_क्षेत्रम्,
                                    &प्राप्तम्_any_selected_क्षेत्रम्);

  संरचना bytesearch_context अन्वेषणम् = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&अन्वेषणम्);

  यावत् (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      अन्वेषणम्.at_eof = mbbuf.eof;
      अन्वेषणम्.पङ्क्तिः_अन्तः_ज्ञातम् = false;

      यदि (pending_पङ्क्तिः_delim_क्षेत्रम्)
        {
          pending_पङ्क्तिः_delim_क्षेत्रम् = false;

          यदि (n_avail == 0)
            {
              यदि (handle_क्षेत्रम्_1 (क्षेत्रम्_idx, बफर_first_क्षेत्रम्,
                                  &क्षेत्रम्_1_n_अष्टकानि))
                अग्र_item (&क्षेत्रम्_idx);
              maybe_write_पङ्क्तिः_delim (प्राप्तम्_any_selected_क्षेत्रम्, क्षेत्रम्_idx);
              प्रत्यागम;
            }

          handle_क्षेत्रम्_delimiter (&क्षेत्रम्_idx, बफर_first_क्षेत्रम्,
                                  &क्षेत्रम्_1_n_अष्टकानि,
                                  &प्राप्तम्_any_selected_क्षेत्रम्, &write_क्षेत्रम्);
          यदि (whitespace_delimited)
            skip_blank_सततम् = true;
        }

      यदि (n_avail == 0)
        भङ्ग;

      वर्ण *chunk = mbbuf.बफर + mbbuf.विस्थापनम्;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      यदि (क्षेत्रम्_idx == 1
          && (!whitespace_delimited || !क्षेत्रम्_selection_exhausted (क्षेत्रम्_idx))
          && !क्षेत्रम्_delim_is_पङ्क्तिः_delim ()
          && !(whitespace_delimited
               ? find_c_blank (chunk, n_avail)
               : find_क्षेत्रम्_delim (chunk, n_avail)))
        {
          वर्ण *चरमः_पङ्क्तिः_delim = अन्वेषणम्.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, पङ्क्तिः_delim, n_avail);
          यदि (चरमः_पङ्क्तिः_delim)
            {
              idx_t n = चरमः_पङ्क्तिः_delim - chunk + 1;
              यदि (! suppress_non_delimited)
                {
                  write_अष्टकानि (क्षेत्रम्_1_बफर, क्षेत्रम्_1_n_अष्टकानि);
                  write_अष्टकानि (chunk, n);
                  यदि (अन्वेषणम्.at_eof && chunk[n - 1] != पङ्क्तिः_delim)
                    write_पङ्क्तिः_delim ();
                }
              क्षेत्रम्_1_n_अष्टकानि = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_पङ्क्तिः = false;
              यदि (अन्वेषणम्.at_eof)
                प्रत्यागम;
              अनुवर्त;
            }
        }

      यावत् (processed < n_avail)
        {
          वर्ण *terminator = रिक्त;

          यदि (skip_blank_सततम्)
            {
              यावत् (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              यदि (processed == n_avail)
                भङ्ग;
              skip_blank_सततम् = false;
            }

          यदि (क्षेत्रम्_selection_exhausted (क्षेत्रम्_idx))
            {
              यदि (क्षेत्रम्_delim_is_पङ्क्तिः_delim ())
                {
                  maybe_write_पङ्क्तिः_delim (प्राप्तम्_any_selected_क्षेत्रम्,
                                          क्षेत्रम्_idx);
                  mbbuf_advance (&mbbuf, processed);
                  प्रत्यागम;
                }
              अन्वेषणम्.बहुलकम्_2 = BYTESEARCH_पङ्क्तिः_ONLY;
              गणना क्षेत्रम्_terminator terminator_विधा
                = find_क्षेत्रम्_terminator (chunk + processed, n_avail - processed,
                                         &अन्वेषणम्, &terminator);
              यदि (terminator_विधा == क्षेत्रम्_पङ्क्तिः_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_पङ्क्तिः_delim (प्राप्तम्_any_selected_क्षेत्रम्,
                                          क्षेत्रम्_idx);
                  गच्छ reset_पङ्क्तिः;
                }
              processed = n_avail;
              भङ्ग;
            }

          गणना क्षेत्रम्_terminator terminator_विधा
            = find_क्षेत्रम्_terminator (chunk + processed, n_avail - processed,
                                     &अन्वेषणम्, &terminator);
          idx_t क्षेत्रम्_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          यदि (terminator_विधा == क्षेत्रम्_दत्त
              && !अन्वेषणम्.at_eof
              && !whitespace_delimited
              && !क्षेत्रम्_delim_is_पङ्क्तिः_delim ())
            क्षेत्रम्_len -= क्षेत्रम्_delim_overlap (chunk + processed, क्षेत्रम्_len);

          यदि (क्षेत्रम्_len || terminator)
            have_pending_पङ्क्तिः = true;

          /* Output field content.  */
          यदि (क्षेत्रम्_idx == 1 && बफर_first_क्षेत्रम्)
            append_क्षेत्रम्_1_chunk (chunk + processed, क्षेत्रम्_len,
                                  &क्षेत्रम्_1_n_अष्टकानि);
          अन्यथा यदि (write_क्षेत्रम्)
            write_अष्टकानि (chunk + processed, क्षेत्रम्_len);
          processed += क्षेत्रम्_len;

          यदि (terminator_विधा == क्षेत्रम्_दत्त)
            भङ्ग;

          यदि (terminator_विधा == क्षेत्रम्_DELIMITER)
            {
              यदि (क्षेत्रम्_delim_is_पङ्क्तिः_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_पङ्क्तिः_delim_क्षेत्रम् = true;
                  भङ्ग;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_क्षेत्रम्_delimiter (&क्षेत्रम्_idx, बफर_first_क्षेत्रम्,
                                      &क्षेत्रम्_1_n_अष्टकानि,
                                      &प्राप्तम्_any_selected_क्षेत्रम्, &write_क्षेत्रम्);
              यदि (whitespace_delimited)
                skip_blank_सततम् = true;
            }
          अन्यथा
            {
              affirm (terminator_विधा == क्षेत्रम्_पङ्क्तिः_DELIMITER);
              processed++;
              finish_current_पङ्क्तिः (क्षेत्रम्_idx, बफर_first_क्षेत्रम्,
                                   &क्षेत्रम्_1_n_अष्टकानि,
                                   प्राप्तम्_any_selected_क्षेत्रम्, true);

            reset_पङ्क्तिः:
              क्षेत्रम्_idx = 1;
              current_rp = frp;
              प्राप्तम्_any_selected_क्षेत्रम् = false;
              have_pending_पङ्क्तिः = false;
              bytesearch_context_reset (&अन्वेषणम्);
              write_क्षेत्रम् = begin_क्षेत्रम्_output (क्षेत्रम्_idx, बफर_first_क्षेत्रम्,
                                                &प्राप्तम्_any_selected_क्षेत्रम्);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  यदि (!have_pending_पङ्क्तिः)
    प्रत्यागम;

  finish_current_पङ्क्तिः (क्षेत्रम्_idx, बफर_first_क्षेत्रम्, &क्षेत्रम्_1_n_अष्टकानि,
                       प्राप्तम्_any_selected_क्षेत्रम्, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

स्थिर शून्य
cut_fields_ws (FILE *प्रवाहः)
{
  यदि (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (प्रवाहः);
  अन्यथा
    cut_fields_mb_any (प्रवाहः, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

स्थिर bool
cut_file (वर्ण ध्रुव *file, शून्य (*cut_प्रवाहः) (FILE *))
{
  FILE *प्रवाहः;

  यदि (streq (file, "-"))
    {
      have_read_stdin = true;
      प्रवाहः = stdin;
      assume (प्रवाहः);  /* Pacify GCC bug#109613.  */
    }
  अन्यथा
    {
      प्रवाहः = fopen (file, "r");
      यदि (प्रवाहः == रिक्त)
        {
          error (0, errno, "%s", quotef (file));
          प्रत्यागम false;
        }
    }

  fadvise (प्रवाहः, FADVISE_SEQUENTIAL);

  cut_प्रवाहः (प्रवाहः);

  पूर्णाङ्क err = errno;
  यदि (!ferror (प्रवाहः))
    err = 0;
  यदि (streq (file, "-"))
    clearerr (प्रवाहः);		/* Also clear EOF.  */
  अन्यथा यदि (fclose (प्रवाहः) == EOF)
    err = errno;
  यदि (err)
    {
      error (0, err, "%s", quotef (file));
      प्रत्यागम false;
    }
  प्रत्यागम true;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  वर्ण *spec_list_string = रिक्त;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, रिक्त))
         != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'b':
          cut_बहुलकम् = CUT_बहुलकम्_अष्टकानि;
          FALLTHROUGH;
        पक्ष 'c':
          यदि (optc == 'c')
            cut_बहुलकम् = CUT_बहुलकम्_CHARACTERS;
          FALLTHROUGH;
        पक्ष 'f':
        पक्ष 'F':
          यदि (optc == 'f' || optc == 'F')
            cut_बहुलकम् = CUT_बहुलकम्_FIELDS;
          यदि (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          यदि (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          भङ्ग;

        पक्ष 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            यदि (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_अष्टकानि (delim_अष्टकानि, optarg, g.len + 1);
            delim_specified = true;
            भङ्ग;
          }

        पक्ष 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_तर्काः,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          भङ्ग;

        पक्ष 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_दीर्घता = (optarg[0] == '\0'
                                     ? 1 : वाक्यदैर्घ्यम् (optarg));
          output_delimiter_string = optarg;
          भङ्ग;

        पक्ष 'n':
          no_विभज = true;
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
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  यदि (cut_बहुलकम् == CUT_बहुलकम्_अष्टकानि || cut_बहुलकम् == CUT_बहुलकम्_CHARACTERS)
    {
      यदि (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an निवेशः delimiter makes sense\n\
\tonly when operating on fields"));

      यदि (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited पङ्क्तयः makes sense\n\
\tonly when operating on fields"));
    }

  यदि (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  यदि (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  समुच्चयः_fields (spec_list_string,
              (((cut_बहुलकम् == CUT_बहुलकम्_अष्टकानि
                 || cut_बहुलकम् == CUT_बहुलकम्_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  यदि (!delim_specified)
    {
      delim_अष्टकानि[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  यदि (output_delimiter_string == रिक्त)
    {
      output_delimiter_string = output_delimiter_default;
      यदि (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_दीर्घता = 1;
        }
      अन्यथा
        {
          copy_अष्टकानि (output_delimiter_default, delim_अष्टकानि, delim_mcel.len);
          output_delimiter_दीर्घता = delim_mcel.len;
        }
    }

  शून्य (*cut_प्रवाहः) (FILE *) = रिक्त;
  विकल्प (cut_बहुलकम्)
    {
    पक्ष CUT_बहुलकम्_अष्टकानि:
      cut_प्रवाहः = MB_CUR_MAX <= 1 || !no_विभज
                   ? cut_अष्टकानि : cut_अष्टकानि_no_विभज;
      भङ्ग;

    पक्ष CUT_बहुलकम्_CHARACTERS:
      cut_प्रवाहः = MB_CUR_MAX <= 1 ? cut_अष्टकानि : cut_characters;
      भङ्ग;

    पक्ष CUT_बहुलकम्_FIELDS:
      cut_प्रवाहः = whitespace_delimited ? cut_fields_ws
                   : bytesearch_क्षेत्रम्_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      भङ्ग;

    पक्ष CUT_बहुलकम्_NONE:
      affirm (false);
    }
  affirm (cut_प्रवाहः);
  यदि (optind == argc)
    ok = cut_file ("-", cut_प्रवाहः);
  अन्यथा
    प्रति (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_प्रवाहः);


  यदि (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
