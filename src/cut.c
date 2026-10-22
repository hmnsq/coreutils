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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"

#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "ioblksize.h"
#అన్తర్భావ "mbbuf.h"
#అన్తర్భావ "memchr2.h"

#అన్తర్భావ "set-fields.h"
#అన్తర్భావ "utf8.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "cut"

#నిర్వచించు AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#నిర్వచించు FATAL_ERROR(సందేశం)						\
  చేయి									\
    {									\
      error (0, 0, (సందేశం));						\
      usage (EXIT_FAILURE);						\
    }									\
  అయితే (0)

/* Above this function call overhead becomes less of a concern.
   At this and below we avoid fwrite(), memchr() etc.  */
#నిర్వచించు SMALL_BYTE_పరిమితి 3

/* Pointer inside RP.  When checking if a -b,-c,-f is selected
   by a finite range, we check if it is between CURRENT_RP.LO
   and CURRENT_RP.HI.  If the index is greater than
   CURRENT_RP.HI then we make CURRENT_RP to point to the next range pair.  */
స్థిర నిర్మాణం క్షేత్రం_range_pair *current_rp;

/* This buffer is used to support the semantics of the -s option
   (or lack of same) when the specified field list includes (does
   not include) the first field.  In both of those cases, the entire
   first field must be read into this buffer to determine whether it
   is followed by a delimiter or a newline before any of it may be
   output.  Otherwise, cut_fields can do the job without using this
   buffer.  */
స్థిర అక్షరం *క్షేత్రం_1_బఫరు;

/* The number of bytes allocated for FIELD_1_BUFFER.  */
స్థిర idx_t క్షేత్రం_1_bufsize;

/* If true, do not output lines containing no delimiter characters.
   Otherwise, all such lines are printed.  This option is valid only
   with field mode.  */
స్థిర bool suppress_non_delimited;

/* If true, print all bytes, characters, or fields _except_
   those that were specified.  */
స్థిర bool complement;

/* The delimiter character for multibyte field mode.  */
స్థిర mcel_t delim_mcel;

/* The delimiter bytes
   (to be NUL terminated for strstr() optimization).  */
స్థిర అక్షరం delim_బైట్లు[MCEL_LEN_MAX + 1];

/* The delimiter for each line/record.  */
స్థిర గుర్తులేని అక్షరం పంక్తి_delim = '\n';

/* The length of output_delimiter_string.  */
స్థిర size_t output_delimiter_నిడివి;

/* The output field separator string.  Defaults to the 1-character
   string consisting of the input delimiter.  */
స్థిర అక్షరం *output_delimiter_string;

/* The output delimiter string contents, if the default.  */
స్థిర అక్షరం output_delimiter_default[MCEL_LEN_MAX];

/* True if we have ever read standard input.  */
స్థిర bool have_read_stdin;

/* If true, don't split multibyte characters in byte mode.  */
స్థిర bool no_విభజించు;

/* If true, interpret each run of whitespace as one field delimiter.  */
స్థిర bool whitespace_delimited;

/* If true, ignore leading and trailing whitespace in -w mode.  */
స్థిర bool trim_outer_whitespace;

/* If true, default the output delimiter to a single space.  */
స్థిర bool space_output_delimiter_default;

గణన whitespace_option
{
  WHITESPACE_OPTION_TRIMMED
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి whitespace_option_వాదనలు[] =
{
  "trimmed", ఖాళీ
};

స్థిర గణన whitespace_option స్థిరరాశి whitespace_option_types[] =
{
  WHITESPACE_OPTION_TRIMMED
};

ARGMATCH_VERIFY (whitespace_option_వాదనలు, whitespace_option_types);

/* Whether to cut bytes, characters, or fields.  */
స్థిర గణన
{
  CUT_బహుళకం_NONE,
  CUT_బహుళకం_బైట్లు,
  CUT_బహుళకం_CHARACTERS,
  CUT_బహుళకం_FIELDS
} cut_బహుళకం = CUT_బహుళకం_NONE;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  COMPLEMENT_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"bytes", required_వాదం, ఖాళీ, 'b'},
  {"characters", required_వాదం, ఖాళీ, 'c'},
  {"fields", required_వాదం, ఖాళీ, 'f'},
  {"delimiter", required_వాదం, ఖాళీ, 'd'},
  {"no-partial", no_వాదం, ఖాళీ, 'n'},
  {"whitespace-delimited", optional_వాదం, ఖాళీ, 'w'},
  {"only-delimited", no_వాదం, ఖాళీ, 's'},
  {"output-delimiter", required_వాదం, ఖాళీ, 'O'},
  {"complement", no_వాదం, ఖాళీ, COMPLEMENT_OPTION},
  {"zero-terminated", no_వాదం, ఖాళీ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s OPTION... [FILE]...\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Print selected parts యొక్క పంక్తులు from each FILE గమ్యం standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --బైట్లు=LIST\n\
         select only these byte positions\n\
"));
      oputs (_("\
  -c, --characters=LIST\n\
         select only these character positions\n\
"));
      oputs (_("\
      --complement\n\
         complement the సమితి_2 యొక్క selected బైట్లు, characters or fields\n\
"));
      oputs (_("\
  -d, --delimiter=DELIM\n\
         use DELIM instead యొక్క TAB కొరకు క్షేత్రం delimiter\n\
"));
      oputs (_("\
  -f, --fields=LIST\n\
         select only these fields;  also print any పంక్తి that కలిగియుంది\n\
         no delimiter character, unless the -s option is specified\n\
"));
      oputs (_("\
  -F LIST\n\
         like -f, but also సూచిస్తుంది -w and -O ' '\n\
"));
      oputs (_("\
  -n, --no-partial\n\
         with -b, don't output partial multi-byte characters\n\
"));
      oputs (_("\
  -O, --output-delimiter=STRING\n\
         use STRING as the output delimiter;\n\
         the అప్రమేయ is గమ్యం use the ఆదానం delimiter\n\
"));
      oputs (_("\
  -s, --only-delimited\n\
         చేయి not print పంక్తులు not containing delimiters\n\
"));
      oputs (_("\
  -w, --whitespace-delimited[=trimmed]\n\
         use a నిరంతరం_2 యొక్క blank characters as the క్షేత్రం delimiter;\n\
         with 'trimmed', ignore leading and trailing blanks\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         పంక్తి delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Use one, and only one యొక్క -b, -c, -f or -F.  Each LIST is made పైకి యొక్క one\n\
range, or many ranges separated by commas.  Selected ఆదానం is written\n\
లోపల the same క్రమం that it is పఠ, and is written exactly once.\n\
"), stdout);
      fputs (_("\
Each range is one యొక్క:\n\
\n\
  N     N'th byte, character or క్షేత్రం, counted from 1\n\
  N-    from N'th byte, character or క్షేత్రం, గమ్యం చివర యొక్క పంక్తి\n\
  N-M   from N'th to M'th (included) byte, character or క్షేత్రం\n\
  -M    from మొదటి గమ్యం M'th (included) byte, character or క్షేత్రం\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}


/* Increment *ITEM_IDX (i.e., a field or byte index),
   and if required CURRENT_RP.  */

స్థిర అంతఃవరుస శూన్య
తదుపరి_అంశం (uintmax_t *అంశం_idx)
{
  (*అంశం_idx)++;
  ఒకవేళ ((*అంశం_idx) > current_rp->hi)
    current_rp++;
}

/* Return nonzero if the K'th field or byte is printable.  */

స్థిర అంతఃవరుస bool
print_kth (uintmax_t k)
{
  తిరిగి current_rp->lo <= k;
}

/* Return nonzero if K'th byte is the beginning of a range.  */

స్థిర అంతఃవరుస bool
is_range_ప్రారంభం_సూచీ (uintmax_t k)
{
  తిరిగి k == current_rp->lo;
}

స్థిర అంతఃవరుస bool
క్షేత్రం_delim_is_పంక్తి_delim (శూన్య)
{
  తిరిగి delim_mcel.len == 1 && delim_బైట్లు[0] == పంక్తి_delim;
}

/* This is equivalent to but faster than calling c32issep directly.
   It assumes all unibyte locales match c_isblank.  */

స్థిర bool
mcel_isblank (mcel_t g)
{
  తిరిగి (g.len == 1 && c_isblank (g.ch)) || (g.len > 1 && c32issep (g.ch));
}

/* Return TRUE if it's valid to do a simple byte search
   for the delimiter bytes.  */

స్థిర అంతఃవరుస bool
bytesearch_క్షేత్రం_delim_ok (శూన్య)
{
  గుర్తులేని అక్షరం delim_0 = delim_బైట్లు[0];

  తిరిగి (delim_mcel.len == 1
          ? (MB_CUR_MAX <= 1
             || (is_utf8_charset ()
                 ? (delim_0 < 0x80 || delim_0 > 0xF4) : delim_0 < 0x30))
          : is_utf8_charset () && ! delim_mcel.err);
}


గణన క్షేత్రం_terminator
{
  క్షేత్రం_దత్తాంశం,
  క్షేత్రం_DELIMITER,
  క్షేత్రం_పంక్తి_DELIMITER,
  క్షేత్రం_EOF
};

గణన bytesearch_బహుళకం
{
  BYTESEARCH_FIELDS,
  BYTESEARCH_పంక్తి_ONLY
};

నిర్మాణం bytesearch_context
{
  గణన bytesearch_బహుళకం బహుళకం_2;
  bool blank_delimited;
  bool at_eof;
  అక్షరం *పంక్తి_చివర;
  bool పంక్తి_చివర_తెలిసిన;
};

స్థిర అంతఃవరుస శూన్య
bytesearch_context_reset (నిర్మాణం bytesearch_context *ctx)
{
  ctx->బహుళకం_2 = BYTESEARCH_FIELDS;
  ctx->పంక్తి_చివర = ఖాళీ;
  ctx->పంక్తి_చివర_తెలిసిన = false;
}

నిర్మాణం mbfield_parser
{
  bool whitespace_delimited;
  bool trim_outer_whitespace;
  bool at_పంక్తి_ప్రారంభం;
  bool have_saved;
  mcel_t saved_g;
};

స్థిర అంతఃవరుస mcel_t
mbbuf_get_saved_char (mbbuf_t *mbbuf, bool *have_saved, mcel_t *saved_g)
{
  ఒకవేళ (*have_saved)
    {
      *have_saved = false;
      తిరిగి *saved_g;
    }
  తిరిగి mbbuf_get_char (mbbuf);
}

స్థిర అంతఃవరుస గణన క్షేత్రం_terminator
skip_whitespace_నిరంతరం (mbbuf_t *mbuf, నిర్మాణం mbfield_parser *parser,
                     bool *have_pending_పంక్తి,
                     bool have_initial_whitespace)
{
  mcel_t g;

  చేయి
    {
      g = mbbuf_get_char (mbuf);
      ఒకవేళ (g.ch != MBBUF_EOF)
        *have_pending_పంక్తి = true;
    }
  అయితే (g.ch != MBBUF_EOF && g.ch != పంక్తి_delim && mcel_isblank (g));

  bool trim_ప్రారంభం = parser->trim_outer_whitespace && parser->at_పంక్తి_ప్రారంభం;

  ఒకవేళ (parser->trim_outer_whitespace
      && (g.ch == MBBUF_EOF || g.ch == పంక్తి_delim))
    {
      తిరిగి g.ch == MBBUF_EOF ? క్షేత్రం_EOF : క్షేత్రం_పంక్తి_DELIMITER;
    }

  parser->saved_g = g;
  parser->have_saved = true;
  తిరిగి trim_ప్రారంభం && !have_initial_whitespace ? క్షేత్రం_దత్తాంశం : క్షేత్రం_DELIMITER;
}

/* Like fwrite, but avoid a function call for smaller amounts,
   and exit immediately upon error.  */

స్థిర శూన్య
write_బైట్లు (అక్షరం స్థిరరాశి *buf, size_t n_బైట్లు)
{
  ఒకవేళ (n_బైట్లు <= SMALL_BYTE_పరిమితి)
    {
      కొరకు (size_t i = 0; i < n_బైట్లు; i++)
        ఒకవేళ (అక్షరం_రాయి (buf[i]) < 0)
          write_error ();
      తిరిగి;
    }

  ఒకవేళ (fwrite (buf, పరిమాణం (అక్షరం), n_బైట్లు, stdout) != n_బైట్లు)
    write_error ();
}

/* Like memcpy, but avoid a function call for smaller amounts.  */

స్థిర అంతఃవరుస శూన్య
copy_బైట్లు (అక్షరం *dst, అక్షరం స్థిరరాశి *src, size_t n_బైట్లు)
{
  ఒకవేళ (n_బైట్లు <= SMALL_BYTE_పరిమితి)
    {
      కొరకు (size_t i = 0; i < n_బైట్లు; i++)
        dst[i] = src[i];
      తిరిగి;
    }

  జ్ఞాపకం_నకలు (dst, src, n_బైట్లు);
}

/* Like memchr, but avoid a function call for smaller amounts.  */

స్థిర అంతఃవరుస శూన్య*
శోధన_బైట్లు (అక్షరం స్థిరరాశి *buf, పూర్ణాంకం c, size_t n_బైట్లు)
{
  ఒకవేళ (n_బైట్లు != 0 && గమ్యం_uchar (buf[0]) == c)
    తిరిగి (అక్షరం *) buf;
  ఒకవేళ (1 < n_బైట్లు && గమ్యం_uchar (buf[1]) == c)
    తిరిగి (అక్షరం *) buf + 1;

  తిరిగి 2 < n_బైట్లు ? (అక్షరం *) memchr (buf + 2, c, n_బైట్లు - 2) : ఖాళీ;
}

స్థిర అంతఃవరుస శూన్య
write_పంక్తి_delim (శూన్య)
{
  ఒకవేళ (అక్షరం_రాయి (పంక్తి_delim) < 0)
    write_error ();
}

స్థిర అంతఃవరుస శూన్య
reset_అంశం_పంక్తి (uintmax_t *అంశం_idx, bool *print_delimiter)
{
  write_పంక్తి_delim ();
  *అంశం_idx = 0;
  *print_delimiter = false;
  current_rp = frp;
}

స్థిర అంతఃవరుస శూన్య
write_pending_పంక్తి_delim (uintmax_t అంశం_idx)
{
  ఒకవేళ (అంశం_idx > 0)
    write_పంక్తి_delim ();
}

స్థిర అంతఃవరుస శూన్య
write_selected_అంశం (bool *print_delimiter, bool range_ప్రారంభం,
                     అక్షరం స్థిరరాశి *buf, size_t n_బైట్లు)
{
  ఒకవేళ (output_delimiter_string != output_delimiter_default)
    {
      ఒకవేళ (*print_delimiter && range_ప్రారంభం)
        write_బైట్లు (output_delimiter_string, output_delimiter_నిడివి);
      *print_delimiter = true;
    }

  write_బైట్లు (buf, n_బైట్లు);
}

స్థిర అంతఃవరుస శూన్య
append_క్షేత్రం_1_chunk (అక్షరం స్థిరరాశి *buf, idx_t len, idx_t *n_బైట్లు)
{
  ఒకవేళ (క్షేత్రం_1_bufsize - *n_బైట్లు < len)
    {
      క్షేత్రం_1_బఫరు = xpalloc (క్షేత్రం_1_బఫరు, &క్షేత్రం_1_bufsize,
                                len, -1, పరిమాణం *క్షేత్రం_1_బఫరు);
    }

  copy_బైట్లు (క్షేత్రం_1_బఫరు + *n_బైట్లు, buf, len);
  *n_బైట్లు += len;
}

స్థిర అంతఃవరుస శూన్య
append_క్షేత్రం_1_బైట్లు (mbbuf_t *mbbuf, mcel_t g, idx_t *n_బైట్లు)
{
  append_క్షేత్రం_1_chunk (mbbuf_char_స్థానభ్రంశం (mbbuf, g), g.len, n_బైట్లు);
}

స్థిర అంతఃవరుస గణన క్షేత్రం_terminator
పరిశీలన_mb_blank_క్షేత్రం (mbbuf_t *mbbuf, నిర్మాణం mbfield_parser *parser,
                     bool *have_pending_పంక్తి, bool write_క్షేత్రం,
                     idx_t *n_బైట్లు)
{
  ఒకవేళ (parser->trim_outer_whitespace && parser->at_పంక్తి_ప్రారంభం)
    {
      గణన క్షేత్రం_terminator terminator
        = skip_whitespace_నిరంతరం (mbbuf, parser, have_pending_పంక్తి, false);
      ఒకవేళ (terminator != క్షేత్రం_దత్తాంశం)
        తిరిగి terminator;
    }

  parser->at_పంక్తి_ప్రారంభం = false;

  అయితే (true)
    {
      mcel_t g = mbbuf_get_saved_char (mbbuf, &parser->have_saved,
                                      &parser->saved_g);
      ఒకవేళ (g.ch == MBBUF_EOF)
        తిరిగి క్షేత్రం_EOF;

      *have_pending_పంక్తి = true;

      ఒకవేళ (g.ch == పంక్తి_delim)
        తిరిగి క్షేత్రం_పంక్తి_DELIMITER;

      ఒకవేళ (mcel_isblank (g))
        తిరిగి skip_whitespace_నిరంతరం (mbbuf, parser, have_pending_పంక్తి, true);

      ఒకవేళ (n_బైట్లు)
        append_క్షేత్రం_1_బైట్లు (mbbuf, g, n_బైట్లు);
      లేకపోతే ఒకవేళ (write_క్షేత్రం)
        write_బైట్లు (mbbuf_char_స్థానభ్రంశం (mbbuf, g), g.len);
    }
}

స్థిర అంతఃవరుస గణన క్షేత్రం_terminator
పరిశీలన_mb_delim_క్షేత్రం (mbbuf_t *mbbuf, bool *have_pending_పంక్తి,
                     bool write_క్షేత్రం, idx_t *n_బైట్లు)
{
  అయితే (true)
    {
      mcel_t g = mbbuf_get_char (mbbuf);
      ఒకవేళ (g.ch == MBBUF_EOF)
        తిరిగి క్షేత్రం_EOF;

      *have_pending_పంక్తి = true;

      ఒకవేళ (g.ch == పంక్తి_delim)
        తిరిగి క్షేత్రం_పంక్తి_DELIMITER;

      ఒకవేళ (delim_mcel.err ? g.err == delim_mcel.err : mcel_eq (g, delim_mcel))
        తిరిగి క్షేత్రం_DELIMITER;

      ఒకవేళ (n_బైట్లు)
        append_క్షేత్రం_1_బైట్లు (mbbuf, g, n_బైట్లు);
      లేకపోతే ఒకవేళ (write_క్షేత్రం)
        write_బైట్లు (mbbuf_char_స్థానభ్రంశం (mbbuf, g), g.len);
    }
}

స్థిర అంతఃవరుస గణన క్షేత్రం_terminator
పరిశీలన_mb_క్షేత్రం (mbbuf_t *mbbuf, నిర్మాణం mbfield_parser *parser,
               bool *have_pending_పంక్తి, bool write_క్షేత్రం, idx_t *n_బైట్లు)
{
  తిరిగి (parser->whitespace_delimited
          ? పరిశీలన_mb_blank_క్షేత్రం (mbbuf, parser, have_pending_పంక్తి,
                                 write_క్షేత్రం, n_బైట్లు)
          : పరిశీలన_mb_delim_క్షేత్రం (mbbuf, have_pending_పంక్తి, write_క్షేత్రం,
                                 n_బైట్లు));
}

/* Skip input through the next line delimiter without decoding characters.
   PARSER may have saved a character beyond a whitespace delimiter.  */

స్థిర గణన క్షేత్రం_terminator
పరిశీలన_mb_పంక్తి_చివర (mbbuf_t *mbbuf, నిర్మాణం mbfield_parser *parser,
                  bool *have_pending_పంక్తి)
{
  ఒకవేళ (parser->have_saved)
    {
      parser->have_saved = false;
      ఒకవేళ (parser->saved_g.ch == MBBUF_EOF)
        తిరిగి క్షేత్రం_EOF;

      *have_pending_పంక్తి = true;
      ఒకవేళ (parser->saved_g.ch == పంక్తి_delim)
        తిరిగి క్షేత్రం_పంక్తి_DELIMITER;
    }

  అయితే (true)
    {
      idx_t అందుబాటు = mbbuf_topup (mbbuf);
      ఒకవేళ (అందుబాటు == 0)
        తిరిగి క్షేత్రం_EOF;

      అక్షరం *బఫరు = mbbuf->బఫరు + mbbuf->స్థానభ్రంశం;
      అక్షరం *పంక్తి_చివర = శోధన_బైట్లు (బఫరు, పంక్తి_delim, అందుబాటు);
      idx_t n = పంక్తి_చివర ? పంక్తి_చివర - బఫరు + 1 : అందుబాటు;
      mbbuf_advance (mbbuf, n);
      *have_pending_పంక్తి = true;

      ఒకవేళ (పంక్తి_చివర)
        తిరిగి క్షేత్రం_పంక్తి_DELIMITER;
    }
}

/* Return a pointer to the next field delimiter in BUF, searching LEN bytes.
   Return NULL if none is found.  DELIM_BYTES must be a single byte or
   represent a valid UTF-8 character.  BUF can contain invalid/NUL bytes,
   and must have room for a trailing NUL byte at BUF[LEN].  */

ATTRIBUTE_శుద్ధ
స్థిర అక్షరం *
కనుగొను_క్షేత్రం_delim (అక్షరం *buf, size_t len)
{
  ఒకవేళ (len < delim_mcel.len)
    తిరిగి ఖాళీ;

#ఒకవేళ ! __GLIBC__  /* Only S390 has optimized memmem on glibc-2.42  */
  తిరిగి memmem (buf, len, delim_బైట్లు, delim_mcel.len);
#లేకపోతే
  ఒకవేళ (delim_mcel.len == 1)
    తిరిగి memchr (buf, delim_బైట్లు[0], len);

  అక్షరం స్థిరరాశి *p = buf;
  అక్షరం స్థిరరాశి *చివర = buf + len;

  అక్షరం saved = buf[len];
  buf[len] = '\0'; /* for strstr.  */

  అయితే (p < చివర)
    {
      అక్షరం స్థిరరాశి *nul = memchr (p, '\0', చివర - p);
      ఒకవేళ (!nul)
        {
          అక్షరం స్థిరరాశి *సరిపోలిక = అక్షరశ్రేణి_శోధన (p, delim_బైట్లు);
          buf[len] = saved;
          తిరిగి (అక్షరం *) సరిపోలిక;
        }

      ఒకవేళ (p < nul)
        {
          అక్షరం స్థిరరాశి *సరిపోలిక = అక్షరశ్రేణి_శోధన (p, delim_బైట్లు);
          ఒకవేళ (సరిపోలిక)
            {
              buf[len] = saved;
              తిరిగి (అక్షరం *) సరిపోలిక;
            }
        }

      p = nul + 1;
    }

  buf[len] = saved;
  తిరిగి ఖాళీ;
#అయితే_ముగింపు
}

/* Return a pointer to the next TAB or SPACE in BUF, searching LEN bytes.
   Return NULL if none is found.
   It works for all unibyte locales, i.e., assumes they match c_isblank.  */

ATTRIBUTE_శుద్ధ
స్థిర అక్షరం *
కనుగొను_c_blank (అక్షరం *buf, size_t len)
{
  /* Avoid memchr2 setup for short fields.  */
  idx_t n = MIN (len, 16);
  కొరకు (idx_t i = 0; i < n; i++)
    ఒకవేళ (c_isblank (buf[i]))
      తిరిగి buf + i;

  తిరిగి memchr2 (buf + n, ' ', '\t', len - n);
}

/* Return the number of trailing bytes in BUF that could be the initial
   bytes of a delimiter split across buffers.  */

ATTRIBUTE_శుద్ధ
స్థిర idx_t
క్షేత్రం_delim_overlap (అక్షరం స్థిరరాశి *buf, idx_t len)
{
  idx_t overlap = MIN (len, delim_mcel.len - 1);

  అయితే (0 < overlap)
    {
      ఒకవేళ (memeq (buf + len - overlap, delim_బైట్లు, overlap))
        తిరిగి overlap;
      overlap--;
    }

  తిరిగి 0;
}

/* Byte search for line end or delimiter in BUF,
   returning results in CTX.  */

స్థిర అంతఃవరుస గణన క్షేత్రం_terminator
కనుగొను_క్షేత్రం_terminator (అక్షరం *buf, idx_t len,
                       నిర్మాణం bytesearch_context *ctx, అక్షరం **terminator)
{
  ఒకవేళ (ctx->బహుళకం_2 == BYTESEARCH_పంక్తి_ONLY)
    {
      ఒకవేళ (!ctx->పంక్తి_చివర_తెలిసిన)
        {
          ctx->పంక్తి_చివర = memchr (buf, పంక్తి_delim, len);
          ctx->పంక్తి_చివర_తెలిసిన = true;
        }

      *terminator = ctx->పంక్తి_చివర;
      తిరిగి *terminator ? క్షేత్రం_పంక్తి_DELIMITER : క్షేత్రం_దత్తాంశం;
    }

  ఒకవేళ (క్షేత్రం_delim_is_పంక్తి_delim ())
    {
      *terminator = memchr (buf, పంక్తి_delim, len);
      తిరిగి *terminator ? క్షేత్రం_DELIMITER : క్షేత్రం_దత్తాంశం;
    }

  ఒకవేళ (!ctx->పంక్తి_చివర_తెలిసిన)
    {
      ctx->పంక్తి_చివర = memchr (buf, పంక్తి_delim, len);
      ctx->పంక్తి_చివర_తెలిసిన = true;
    }

  idx_t క్షేత్రం_len = ctx->పంక్తి_చివర ? ctx->పంక్తి_చివర - buf : len;

  అక్షరం *క్షేత్రం_చివర = (ctx->blank_delimited
                     ? కనుగొను_c_blank (buf, క్షేత్రం_len)
                     : కనుగొను_క్షేత్రం_delim (buf, క్షేత్రం_len));

  ఒకవేళ (క్షేత్రం_చివర)
    {
      *terminator = క్షేత్రం_చివర;
      తిరిగి క్షేత్రం_DELIMITER;
    }

  *terminator = ctx->పంక్తి_చివర;
  తిరిగి ctx->పంక్తి_చివర ? క్షేత్రం_పంక్తి_DELIMITER : క్షేత్రం_దత్తాంశం;
}

/* Write the end-of-line delimiter if appropriate for the current line.  */

స్థిర అంతఃవరుస శూన్య
maybe_write_పంక్తి_delim (bool దొరికింది_any_selected_క్షేత్రం, uintmax_t క్షేత్రం_idx)
{
  ఒకవేళ (దొరికింది_any_selected_క్షేత్రం
      || !(suppress_non_delimited && క్షేత్రం_idx == 1))
    write_పంక్తి_delim ();
}

/* Return TRUE if FIELD_IDX is selected,
   and write the output delimiter if appropriate.  */

స్థిర అంతఃవరుస bool
begin_క్షేత్రం_output (uintmax_t క్షేత్రం_idx, bool బఫరు_మొదటి_క్షేత్రం,
                    bool *దొరికింది_any_selected_క్షేత్రం)
{
  bool write_క్షేత్రం = print_kth (క్షేత్రం_idx);

  ఒకవేళ (write_క్షేత్రం && ! (క్షేత్రం_idx == 1 && బఫరు_మొదటి_క్షేత్రం))
    {
      ఒకవేళ (*దొరికింది_any_selected_క్షేత్రం)
        write_బైట్లు (output_delimiter_string, output_delimiter_నిడివి);
      *దొరికింది_any_selected_క్షేత్రం = true;
    }

  తిరిగి write_క్షేత్రం;
}

స్థిర అంతఃవరుస bool
handle_క్షేత్రం_1 (uintmax_t క్షేత్రం_idx, bool బఫరు_మొదటి_క్షేత్రం,
                idx_t *క్షేత్రం_1_n_బైట్లు)
{
  bool క్షేత్రం_1_selected = false;

  ఒకవేళ (క్షేత్రం_idx == 1 && బఫరు_మొదటి_క్షేత్రం)
    {
      ఒకవేళ ((క్షేత్రం_1_selected = print_kth (1)))
        write_బైట్లు (క్షేత్రం_1_బఫరు, *క్షేత్రం_1_n_బైట్లు);
      *క్షేత్రం_1_n_బైట్లు = 0;
    }

  తిరిగి క్షేత్రం_1_selected;
}

స్థిర అంతఃవరుస శూన్య
handle_క్షేత్రం_delimiter (uintmax_t *క్షేత్రం_idx, bool బఫరు_మొదటి_క్షేత్రం,
                        idx_t *క్షేత్రం_1_n_బైట్లు,
                        bool *దొరికింది_any_selected_క్షేత్రం, bool *write_క్షేత్రం)
{
  ఒకవేళ (handle_క్షేత్రం_1 (*క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం, క్షేత్రం_1_n_బైట్లు))
    *దొరికింది_any_selected_క్షేత్రం = true;

  తదుపరి_అంశం (క్షేత్రం_idx);
  *write_క్షేత్రం = begin_క్షేత్రం_output (*క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం,
                                     దొరికింది_any_selected_క్షేత్రం);
}

స్థిర అంతఃవరుస bool
క్షేత్రం_selection_exhausted (uintmax_t క్షేత్రం_idx)
{
  తిరిగి !print_kth (క్షేత్రం_idx) && current_rp->lo == UINTMAX_MAX;
}

స్థిర అంతఃవరుస శూన్య
ముగింపు_current_పంక్తి (uintmax_t క్షేత్రం_idx, bool బఫరు_మొదటి_క్షేత్రం,
                     idx_t *క్షేత్రం_1_n_బైట్లు,
                     bool దొరికింది_any_selected_క్షేత్రం, bool పంక్తి_terminated)
{
  ఒకవేళ (క్షేత్రం_idx == 1 && బఫరు_మొదటి_క్షేత్రం)
    {
      ఒకవేళ (!suppress_non_delimited
          && (పంక్తి_terminated || *క్షేత్రం_1_n_బైట్లు != 0))
        {
          write_బైట్లు (క్షేత్రం_1_బఫరు, *క్షేత్రం_1_n_బైట్లు);
          write_పంక్తి_delim ();
        }
      *క్షేత్రం_1_n_బైట్లు = 0;
    }
  లేకపోతే
    maybe_write_పంక్తి_delim (దొరికింది_any_selected_క్షేత్రం, క్షేత్రం_idx);
}

స్థిర అంతఃవరుస శూన్య
sync_byte_selection (uintmax_t byte_idx)
{
  అయితే (current_rp->hi <= byte_idx)
    current_rp++;
}

స్థిర అంతఃవరుస శూన్య
reset_క్షేత్రం_పంక్తి (uintmax_t *క్షేత్రం_idx, bool *దొరికింది_any_selected_క్షేత్రం,
                  bool *have_pending_పంక్తి, నిర్మాణం mbfield_parser *parser)
{
  *క్షేత్రం_idx = 1;
  current_rp = frp;
  *దొరికింది_any_selected_క్షేత్రం = false;
  *have_pending_పంక్తి = false;
  parser->have_saved = false;
  parser->at_పంక్తి_ప్రారంభం = true;
}

/* Read from STREAM using buffered block reads, printing selected bytes.
   BYTE_IDX and PRINT_DELIMITER track the current line state and allow
   callers to hand off mid-line.  */

స్థిర శూన్య
cut_బైట్లు (FILE *ప్రవాహం)
{
  స్థిర అక్షరం బైట్లు_లోపల[IO_BUFSIZE];
  uintmax_t byte_idx = 0;
  bool print_delimiter = false;
  పూర్ణాంకం fd = fileno (ప్రవాహం);

  current_rp = frp;

  అయితే (true)
    {
      idx_t అందుబాటు = పఠ (fd, బైట్లు_లోపల, పరిమాణం బైట్లు_లోపల);
      ఒకవేళ (అందుబాటు <= 0)
        {
          ఒకవేళ (అందుబాటు < 0)
            fseterr (ప్రవాహం);
          write_pending_పంక్తి_delim (byte_idx);
          ఆపు;
        }

      idx_t processed = 0;

      అయితే (processed < అందుబాటు)
        {
          అక్షరం *పంక్తి = బైట్లు_లోపల + processed;
          idx_t గమ్యం_process = అందుబాటు - processed;
          అక్షరం *పంక్తి_చివర = శోధన_బైట్లు (పంక్తి, పంక్తి_delim, గమ్యం_process);
          అక్షరం *చివర = పంక్తి + (పంక్తి_చివర ? పంక్తి_చివర - పంక్తి : గమ్యం_process);
          అక్షరం *p = పంక్తి;

          అయితే (0 < చివర - p)
            {
              sync_byte_selection (byte_idx);

              ఒకవేళ (byte_idx + 1 < current_rp->lo)
                {
                  idx_t skip = MIN (చివర - p, current_rp->lo - (byte_idx + 1));
                  p += skip;
                  byte_idx += skip;
                }
              లేకపోతే
                {
                  idx_t n = MIN (చివర - p, current_rp->hi - byte_idx);
                  write_selected_అంశం (&print_delimiter,
                                       is_range_ప్రారంభం_సూచీ (byte_idx + 1),
                                       p, n);
                  p += n;
                  byte_idx += n;
                }
            }

          processed += చివర - పంక్తి;
          ఒకవేళ (పంక్తి_చివర)
            {
              processed++;
              reset_అంశం_పంక్తి (&byte_idx, &print_delimiter);
            }
        }
    }
}

/* Read from STREAM, printing selected bytes without splitting
   multibyte characters.  */

స్థిర శూన్య
cut_characters_బహుళకం (FILE *ప్రవాహం, bool byte_బహుళకం)
{
  bool utf8 = is_utf8_charset ();
  uintmax_t idx = 0;
  bool print_delimiter = false;
  స్థిర అక్షరం బైట్లు_లోపల[IO_BUFSIZE];
  mbbuf_t mbbuf;

  current_rp = frp;
  mbbuf_init (&mbbuf, బైట్లు_లోపల, పరిమాణం బైట్లు_లోపల, ప్రవాహం);

  అయితే (true)
    {
      /* Skip unselected line suffixes without decoding each character.  */
      ఒకవేళ (idx && క్షేత్రం_selection_exhausted (idx))
        {
          idx_t అందుబాటు = mbbuf_topup (&mbbuf);
          అక్షరం *p = mbbuf.బఫరు + mbbuf.స్థానభ్రంశం;
          అక్షరం *చివర = శోధన_బైట్లు (p, పంక్తి_delim, అందుబాటు);
          mbbuf_advance (&mbbuf, చివర ? చివర - p : అందుబాటు);
          ఒకవేళ (!చివర && అందుబాటు)
            కొనసాగు;
        }

      /* Count unselected ASCII/UTF-8 characters directly, for efficiency.  */
      ఒకవేళ (!byte_బహుళకం && utf8 && idx + 1 < current_rp->lo)
        {
          అక్షరం స్థిరరాశి *p = mbbuf.బఫరు + mbbuf.స్థానభ్రంశం;
          idx_t n = MIN (mbbuf_avail (&mbbuf), current_rp->lo - idx - 1);
          అక్షరం స్థిరరాశి *చివర = శోధన_బైట్లు (p, పంక్తి_delim, n);
          ఒకవేళ (చివర)
            {
              /* Even with ASCII, this cannot reach the next selection.  */
              mbbuf_advance (&mbbuf, చివర - p + 1);
              reset_అంశం_పంక్తి (&idx, &print_delimiter);
              కొనసాగు;
            }

          idx += u8_లెక్క (p, &n);
          mbbuf_advance (&mbbuf, n);
        }

      mcel_t g = mbbuf_get_char (&mbbuf);

      ఒకవేళ (g.ch == పంక్తి_delim)
        reset_అంశం_పంక్తి (&idx, &print_delimiter);
      లేకపోతే ఒకవేళ (g.ch == MBBUF_EOF)
        {
          write_pending_పంక్తి_delim (idx);
          ఆపు;
        }
      లేకపోతే ఒకవేళ (byte_బహుళకం)
        {
          bool మొదటి_selected_is_range_ప్రారంభం = false;
          bool చూసిన_selected = false;
          bool suffix_selected = true;

          కొరకు (idx_t i = 0; i < g.len; i++)
            {
              తదుపరి_అంశం (&idx);
              ఒకవేళ (print_kth (idx))
                {
                  ఒకవేళ (!చూసిన_selected)
                    {
                      చూసిన_selected = true;
                      మొదటి_selected_is_range_ప్రారంభం
                        = is_range_ప్రారంభం_సూచీ (idx);
                    }
                }
              లేకపోతే ఒకవేళ (చూసిన_selected)
                suffix_selected = false;
            }

          ఒకవేళ (చూసిన_selected && suffix_selected)
            write_selected_అంశం (&print_delimiter,మొదటి_selected_is_range_ప్రారంభం,
                                 mbbuf_char_స్థానభ్రంశం (&mbbuf, g), g.len);
        }
      లేకపోతే
        {
          తదుపరి_అంశం (&idx);
          ఒకవేళ (print_kth (idx))
            write_selected_అంశం (&print_delimiter,
                                 is_range_ప్రారంభం_సూచీ (idx),
                                 mbbuf_char_స్థానభ్రంశం (&mbbuf, g), g.len);
        }
    }
}

స్థిర శూన్య
cut_బైట్లు_no_విభజించు (FILE *ప్రవాహం)
{
  cut_characters_బహుళకం (ప్రవాహం, true);
}

స్థిర శూన్య
cut_characters (FILE *ప్రవాహం)
{
  cut_characters_బహుళకం (ప్రవాహం, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte-aware field delimiter parser.  */

స్థిర శూన్య
cut_fields_mb_any (FILE *ప్రవాహం, bool whitespace_బహుళకం)
{
  స్థిర అక్షరం బైట్లు_లోపల[IO_BUFSIZE];
  mbbuf_t mbbuf;
  నిర్మాణం mbfield_parser parser =
    {
      .whitespace_delimited = whitespace_బహుళకం,
      .trim_outer_whitespace = trim_outer_whitespace,
      .at_పంక్తి_ప్రారంభం = true,
      .saved_g = { .ch = MBBUF_EOF }
    };
  uintmax_t క్షేత్రం_idx = 1;
  bool దొరికింది_any_selected_క్షేత్రం = false;
  bool బఫరు_మొదటి_క్షేత్రం;
  bool have_pending_పంక్తి = false;
  bool write_క్షేత్రం;
  idx_t క్షేత్రం_1_n_బైట్లు = 0;

  current_rp = frp;
  mbbuf_init (&mbbuf, బైట్లు_లోపల, పరిమాణం బైట్లు_లోపల, ప్రవాహం);

  బఫరు_మొదటి_క్షేత్రం = (suppress_non_delimited ^ !print_kth (1));
  write_క్షేత్రం = begin_క్షేత్రం_output (క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం,
                                    &దొరికింది_any_selected_క్షేత్రం);

  అయితే (true)
    {
      గణన క్షేత్రం_terminator terminator;
      ఒకవేళ (!బఫరు_మొదటి_క్షేత్రం || క్షేత్రం_idx != 1)
        terminator = పరిశీలన_mb_క్షేత్రం (&mbbuf, &parser, &have_pending_పంక్తి,
                                    write_క్షేత్రం, ఖాళీ);
      లేకపోతే
        terminator = పరిశీలన_mb_క్షేత్రం (&mbbuf, &parser, &have_pending_పంక్తి,
                                    false, &క్షేత్రం_1_n_బైట్లు);

      ఒకవేళ (terminator == క్షేత్రం_DELIMITER)
        {
          handle_క్షేత్రం_delimiter (&క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం,
                                  &క్షేత్రం_1_n_బైట్లు,
                                  &దొరికింది_any_selected_క్షేత్రం, &write_క్షేత్రం);
          ఒకవేళ (! క్షేత్రం_selection_exhausted (క్షేత్రం_idx))
            కొనసాగు;

          terminator = పరిశీలన_mb_పంక్తి_చివర (&mbbuf, &parser,
                                         &have_pending_పంక్తి);
        }

      ఒకవేళ (terminator == క్షేత్రం_EOF && !have_pending_పంక్తి)
        ఆపు;

      ముగింపు_current_పంక్తి (క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం,
                           &క్షేత్రం_1_n_బైట్లు, దొరికింది_any_selected_క్షేత్రం,
                           terminator == క్షేత్రం_పంక్తి_DELIMITER);
      ఒకవేళ (terminator == క్షేత్రం_EOF)
        ఆపు;

      reset_క్షేత్రం_పంక్తి (&క్షేత్రం_idx, &దొరికింది_any_selected_క్షేత్రం,
                        &have_pending_పంక్తి, &parser);
      write_క్షేత్రం = begin_క్షేత్రం_output (క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం,
                                        &దొరికింది_any_selected_క్షేత్రం);
    }
}

/* Read from STREAM, printing to standard output any selected fields,
   using a multibyte field delimiter.  */

స్థిర శూన్య
cut_fields_mb (FILE *ప్రవాహం)
{
  cut_fields_mb_any (ప్రవాహం, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using byte searches for a single-byte or valid UTF-8 field delimiter.  */

స్థిర శూన్య
cut_fields_bytesearch (FILE *ప్రవాహం)
{
  /* Leave 1 byte unused so space to NUL terminate (for strstr).  */
  స్థిర అక్షరం బైట్లు_లోపల[IO_BUFSIZE+1];
  mbbuf_t mbbuf;
  uintmax_t క్షేత్రం_idx = 1;
  bool దొరికింది_any_selected_క్షేత్రం = false;
  bool have_pending_పంక్తి = false;
  bool skip_blank_నిరంతరం = false;
  bool pending_పంక్తి_delim_క్షేత్రం = false;
  bool write_క్షేత్రం;
  idx_t క్షేత్రం_1_n_బైట్లు = 0;

  current_rp = frp;
  bool బఫరు_మొదటి_క్షేత్రం = suppress_non_delimited ^ !print_kth (1);
  mbbuf_init (&mbbuf, బైట్లు_లోపల, పరిమాణం బైట్లు_లోపల - 1, ప్రవాహం);
  write_క్షేత్రం = begin_క్షేత్రం_output (క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం,
                                    &దొరికింది_any_selected_క్షేత్రం);

  నిర్మాణం bytesearch_context శోధన = { .blank_delimited = whitespace_delimited};
  bytesearch_context_reset (&శోధన);

  అయితే (true)
    {
      idx_t n_avail = mbbuf_topup (&mbbuf);
      శోధన.at_eof = mbbuf.eof;
      శోధన.పంక్తి_చివర_తెలిసిన = false;

      ఒకవేళ (pending_పంక్తి_delim_క్షేత్రం)
        {
          pending_పంక్తి_delim_క్షేత్రం = false;

          ఒకవేళ (n_avail == 0)
            {
              ఒకవేళ (handle_క్షేత్రం_1 (క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం,
                                  &క్షేత్రం_1_n_బైట్లు))
                తదుపరి_అంశం (&క్షేత్రం_idx);
              maybe_write_పంక్తి_delim (దొరికింది_any_selected_క్షేత్రం, క్షేత్రం_idx);
              తిరిగి;
            }

          handle_క్షేత్రం_delimiter (&క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం,
                                  &క్షేత్రం_1_n_బైట్లు,
                                  &దొరికింది_any_selected_క్షేత్రం, &write_క్షేత్రం);
          ఒకవేళ (whitespace_delimited)
            skip_blank_నిరంతరం = true;
        }

      ఒకవేళ (n_avail == 0)
        ఆపు;

      అక్షరం *chunk = mbbuf.బఫరు + mbbuf.స్థానభ్రంశం;
      idx_t processed = 0;

      /* Fast path: no field delimiter in chunk — output or skip lines.  */
      ఒకవేళ (క్షేత్రం_idx == 1
          && (!whitespace_delimited || !క్షేత్రం_selection_exhausted (క్షేత్రం_idx))
          && !క్షేత్రం_delim_is_పంక్తి_delim ()
          && !(whitespace_delimited
               ? కనుగొను_c_blank (chunk, n_avail)
               : కనుగొను_క్షేత్రం_delim (chunk, n_avail)))
        {
          అక్షరం *చివరి_పంక్తి_delim = శోధన.at_eof
                                  ? chunk + n_avail - 1
                                  : memrchr (chunk, పంక్తి_delim, n_avail);
          ఒకవేళ (చివరి_పంక్తి_delim)
            {
              idx_t n = చివరి_పంక్తి_delim - chunk + 1;
              ఒకవేళ (! suppress_non_delimited)
                {
                  write_బైట్లు (క్షేత్రం_1_బఫరు, క్షేత్రం_1_n_బైట్లు);
                  write_బైట్లు (chunk, n);
                  ఒకవేళ (శోధన.at_eof && chunk[n - 1] != పంక్తి_delim)
                    write_పంక్తి_delim ();
                }
              క్షేత్రం_1_n_బైట్లు = 0;
              mbbuf_advance (&mbbuf, n);
              have_pending_పంక్తి = false;
              ఒకవేళ (శోధన.at_eof)
                తిరిగి;
              కొనసాగు;
            }
        }

      అయితే (processed < n_avail)
        {
          అక్షరం *terminator = ఖాళీ;

          ఒకవేళ (skip_blank_నిరంతరం)
            {
              అయితే (processed < n_avail && c_isblank (chunk[processed]))
                processed++;
              ఒకవేళ (processed == n_avail)
                ఆపు;
              skip_blank_నిరంతరం = false;
            }

          ఒకవేళ (క్షేత్రం_selection_exhausted (క్షేత్రం_idx))
            {
              ఒకవేళ (క్షేత్రం_delim_is_పంక్తి_delim ())
                {
                  maybe_write_పంక్తి_delim (దొరికింది_any_selected_క్షేత్రం,
                                          క్షేత్రం_idx);
                  mbbuf_advance (&mbbuf, processed);
                  తిరిగి;
                }
              శోధన.బహుళకం_2 = BYTESEARCH_పంక్తి_ONLY;
              గణన క్షేత్రం_terminator terminator_రకం
                = కనుగొను_క్షేత్రం_terminator (chunk + processed, n_avail - processed,
                                         &శోధన, &terminator);
              ఒకవేళ (terminator_రకం == క్షేత్రం_పంక్తి_DELIMITER)
                {
                  processed = terminator - chunk + 1;
                  maybe_write_పంక్తి_delim (దొరికింది_any_selected_క్షేత్రం,
                                          క్షేత్రం_idx);
                  వెళ్ళు reset_పంక్తి;
                }
              processed = n_avail;
              ఆపు;
            }

          గణన క్షేత్రం_terminator terminator_రకం
            = కనుగొను_క్షేత్రం_terminator (chunk + processed, n_avail - processed,
                                     &శోధన, &terminator);
          idx_t క్షేత్రం_len = terminator ? terminator - (chunk + processed)
                                       : n_avail - processed;

          ఒకవేళ (terminator_రకం == క్షేత్రం_దత్తాంశం
              && !శోధన.at_eof
              && !whitespace_delimited
              && !క్షేత్రం_delim_is_పంక్తి_delim ())
            క్షేత్రం_len -= క్షేత్రం_delim_overlap (chunk + processed, క్షేత్రం_len);

          ఒకవేళ (క్షేత్రం_len || terminator)
            have_pending_పంక్తి = true;

          /* Output field content.  */
          ఒకవేళ (క్షేత్రం_idx == 1 && బఫరు_మొదటి_క్షేత్రం)
            append_క్షేత్రం_1_chunk (chunk + processed, క్షేత్రం_len,
                                  &క్షేత్రం_1_n_బైట్లు);
          లేకపోతే ఒకవేళ (write_క్షేత్రం)
            write_బైట్లు (chunk + processed, క్షేత్రం_len);
          processed += క్షేత్రం_len;

          ఒకవేళ (terminator_రకం == క్షేత్రం_దత్తాంశం)
            ఆపు;

          ఒకవేళ (terminator_రకం == క్షేత్రం_DELIMITER)
            {
              ఒకవేళ (క్షేత్రం_delim_is_పంక్తి_delim () && processed + 1 == n_avail)
                {
                  processed++;
                  pending_పంక్తి_delim_క్షేత్రం = true;
                  ఆపు;
                }

              processed += whitespace_delimited ? 1 : delim_mcel.len;
              handle_క్షేత్రం_delimiter (&క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం,
                                      &క్షేత్రం_1_n_బైట్లు,
                                      &దొరికింది_any_selected_క్షేత్రం, &write_క్షేత్రం);
              ఒకవేళ (whitespace_delimited)
                skip_blank_నిరంతరం = true;
            }
          లేకపోతే
            {
              affirm (terminator_రకం == క్షేత్రం_పంక్తి_DELIMITER);
              processed++;
              ముగింపు_current_పంక్తి (క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం,
                                   &క్షేత్రం_1_n_బైట్లు,
                                   దొరికింది_any_selected_క్షేత్రం, true);

            reset_పంక్తి:
              క్షేత్రం_idx = 1;
              current_rp = frp;
              దొరికింది_any_selected_క్షేత్రం = false;
              have_pending_పంక్తి = false;
              bytesearch_context_reset (&శోధన);
              write_క్షేత్రం = begin_క్షేత్రం_output (క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం,
                                                &దొరికింది_any_selected_క్షేత్రం);
            }
        }

      mbbuf_advance (&mbbuf, processed);
    }

  ఒకవేళ (!have_pending_పంక్తి)
    తిరిగి;

  ముగింపు_current_పంక్తి (క్షేత్రం_idx, బఫరు_మొదటి_క్షేత్రం, &క్షేత్రం_1_n_బైట్లు,
                       దొరికింది_any_selected_క్షేత్రం, false);
}

/* Read from STREAM, printing to standard output any selected fields,
   using runs of whitespace as the field delimiter.  */

స్థిర శూన్య
cut_fields_ws (FILE *ప్రవాహం)
{
  ఒకవేళ (MB_CUR_MAX <= 1 && !trim_outer_whitespace)
    cut_fields_bytesearch (ప్రవాహం);
  లేకపోతే
    cut_fields_mb_any (ప్రవాహం, true);
}

/* Process file FILE to standard output, using CUT_STREAM.
   Return true if successful.  */

స్థిర bool
cut_file (అక్షరం స్థిరరాశి *file, శూన్య (*cut_ప్రవాహం) (FILE *))
{
  FILE *ప్రవాహం;

  ఒకవేళ (streq (file, "-"))
    {
      have_read_stdin = true;
      ప్రవాహం = stdin;
      assume (ప్రవాహం);  /* Pacify GCC bug#109613.  */
    }
  లేకపోతే
    {
      ప్రవాహం = fopen (file, "r");
      ఒకవేళ (ప్రవాహం == ఖాళీ)
        {
          error (0, errno, "%s", quotef (file));
          తిరిగి false;
        }
    }

  fadvise (ప్రవాహం, FADVISE_SEQUENTIAL);

  cut_ప్రవాహం (ప్రవాహం);

  పూర్ణాంకం err = errno;
  ఒకవేళ (!ferror (ప్రవాహం))
    err = 0;
  ఒకవేళ (streq (file, "-"))
    clearerr (ప్రవాహం);		/* Also clear EOF.  */
  లేకపోతే ఒకవేళ (fclose (ప్రవాహం) == EOF)
    err = errno;
  ఒకవేళ (err)
    {
      error (0, err, "%s", quotef (file));
      తిరిగి false;
    }
  తిరిగి true;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optc;
  bool ok;
  bool delim_specified = false;
  bool whitespace_delimited_specified = false;
  అక్షరం *spec_list_string = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "b:c:d:f:F:nO:szw", longopts, ఖాళీ))
         != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'b':
          cut_బహుళకం = CUT_బహుళకం_బైట్లు;
          FALLTHROUGH;
        స్థితి 'c':
          ఒకవేళ (optc == 'c')
            cut_బహుళకం = CUT_బహుళకం_CHARACTERS;
          FALLTHROUGH;
        స్థితి 'f':
        స్థితి 'F':
          ఒకవేళ (optc == 'f' || optc == 'F')
            cut_బహుళకం = CUT_బహుళకం_FIELDS;
          ఒకవేళ (optc == 'F')
            {
              whitespace_delimited = true;
              space_output_delimiter_default = true;
            }
          ఒకవేళ (spec_list_string)
            FATAL_ERROR (_("only one list may be specified"));
          spec_list_string = optarg;
          ఆపు;

        స్థితి 'd':
          {
            /* New delimiter.  */
            /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
            mcel_t g = delim_mcel = mcel_scanz (optarg);
            ఒకవేళ (optarg[0] && optarg[g.len])
              FATAL_ERROR (_("the delimiter must be a single character"));
            /* Note NUL is copied for use with strstr.  */
            copy_బైట్లు (delim_బైట్లు, optarg, g.len + 1);
            delim_specified = true;
            ఆపు;
          }

        స్థితి 'w':
          whitespace_delimited = true;
          whitespace_delimited_specified = true;
          trim_outer_whitespace
            = (optarg
               && XARGMATCH ("--whitespace-delimited", optarg,
                             whitespace_option_వాదనలు,
                             whitespace_option_types)
                  == WHITESPACE_OPTION_TRIMMED);
          ఆపు;

        స్థితి 'O':
          /* Interpret --output-delimiter='' to mean
             'use the NUL byte as the delimiter.'  */
          output_delimiter_నిడివి = (optarg[0] == '\0'
                                     ? 1 : అక్షరశ్రేణి_పొడవు (optarg));
          output_delimiter_string = optarg;
          ఆపు;

        స్థితి 'n':
          no_విభజించు = true;
          ఆపు;

        స్థితి 's':
          suppress_non_delimited = true;
          ఆపు;

        స్థితి 'z':
          పంక్తి_delim = '\0';
          ఆపు;

        స్థితి COMPLEMENT_OPTION:
          complement = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (!spec_list_string)
    FATAL_ERROR (_("you must specify a list of bytes, characters, or fields"));

  ఒకవేళ (cut_బహుళకం == CUT_బహుళకం_బైట్లు || cut_బహుళకం == CUT_బహుళకం_CHARACTERS)
    {
      ఒకవేళ (delim_specified || whitespace_delimited)
        FATAL_ERROR (_("an ఆదానం delimiter makes sense\n\
\tonly when operating on fields"));

      ఒకవేళ (suppress_non_delimited)
        FATAL_ERROR (_("suppressing non-delimited పంక్తులు makes sense\n\
\tonly when operating on fields"));
    }

  ఒకవేళ (delim_specified && whitespace_delimited_specified)
    FATAL_ERROR (_("-d and -w are mutually exclusive"));

  ఒకవేళ (delim_specified && !whitespace_delimited_specified)
    whitespace_delimited = false;

  సమితి_fields (spec_list_string,
              (((cut_బహుళకం == CUT_బహుళకం_బైట్లు
                 || cut_బహుళకం == CUT_బహుళకం_CHARACTERS)
                ? SETFLD_ERRMSG_USE_POS : 0)
               | (complement ? SETFLD_COMPLEMENT : 0)));

  ఒకవేళ (!delim_specified)
    {
      delim_బైట్లు[0] = '\t';
      delim_mcel = mcel_ch ('\t', 1);
    }

  ఒకవేళ (output_delimiter_string == ఖాళీ)
    {
      output_delimiter_string = output_delimiter_default;
      ఒకవేళ (space_output_delimiter_default)
        {
          output_delimiter_default[0] = ' ';
          output_delimiter_నిడివి = 1;
        }
      లేకపోతే
        {
          copy_బైట్లు (output_delimiter_default, delim_బైట్లు, delim_mcel.len);
          output_delimiter_నిడివి = delim_mcel.len;
        }
    }

  శూన్య (*cut_ప్రవాహం) (FILE *) = ఖాళీ;
  ఎంపిక (cut_బహుళకం)
    {
    స్థితి CUT_బహుళకం_బైట్లు:
      cut_ప్రవాహం = MB_CUR_MAX <= 1 || !no_విభజించు
                   ? cut_బైట్లు : cut_బైట్లు_no_విభజించు;
      ఆపు;

    స్థితి CUT_బహుళకం_CHARACTERS:
      cut_ప్రవాహం = MB_CUR_MAX <= 1 ? cut_బైట్లు : cut_characters;
      ఆపు;

    స్థితి CUT_బహుళకం_FIELDS:
      cut_ప్రవాహం = whitespace_delimited ? cut_fields_ws
                   : bytesearch_క్షేత్రం_delim_ok () ? cut_fields_bytesearch
                   : cut_fields_mb;
      ఆపు;

    స్థితి CUT_బహుళకం_NONE:
      affirm (false);
    }
  affirm (cut_ప్రవాహం);
  ఒకవేళ (optind == argc)
    ok = cut_file ("-", cut_ప్రవాహం);
  లేకపోతే
    కొరకు (ok = true; optind < argc; optind++)
      ok &= cut_file (argv[optind], cut_ప్రవాహం);


  ఒకవేళ (have_read_stdin && fclose (stdin) == EOF)
    {
      error (0, errno, "-");
      ok = false;
    }

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
