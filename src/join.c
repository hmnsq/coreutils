/* join - join lines of two files on a common field
   Copyright (C) 1991-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

   Written by Mike Haertel, mike@gnu.ai.mit.edu.  */

#ضمن <config.h>

#ضمن <sys/types.h>
#ضمن <getopt.h>

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "fadvise.h"
#ضمن "hard-locale.h"
#ضمن "linebuffer.h"
#ضمن "mcel.h"
#ضمن "memcasecmp.h"
#ضمن "quote.h"
#ضمن "skipchars.h"
#ضمن "stdio--.h"
#ضمن "xmemcoll.h"
#ضمن "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "join"

#define AUTHORS proper_name ("Mike Haertel")

#define join system_join

#define SWAPLINES(a, b) 𐡏𐡁𐡃 { \
  بنية line *tmp = a; \
  a = b; \
  b = tmp; \
} طالما (0);

/* An element of the list identifying which fields to print for each
   output line.  */
بنية outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    صحيح file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t field;

    بنية outlist *next;
  };

/* A field of a line.  */
بنية field
  {
    حرف *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
بنية line
  {
    بنية linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    بنية field *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
بنية seq
  {
    idx_t count;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    بنية line **lines COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
ساكن بنية line *prevline[2] = {عدم, عدم};

/* The number of lines read from each file.  */
ساكن uintmax_t line_no[2] = {0, 0};

/* The input file names.  */
ساكن حرف *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
ساكن بنية line *spareline[2] = {عدم, عدم};

/* True if the LC_COLLATE locale is hard.  */
ساكن bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
ساكن bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
ساكن bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
ساكن bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ساكن bool issued_disorder_warning[2];

/* Empty output field filler.  */
ساكن حرف ثابت *empty_filler;

/* Whether to ensure the same number of fields are output from each line.  */
ساكن bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
ساكن idx_t autocount_1;
ساكن idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
ساكن ptrdiff_t join_field_1 = -1;
ساكن ptrdiff_t join_field_2 = -1;

/* List of fields to print.  */
ساكن بنية outlist outlist_head;

/* Last element in 'outlist', where a new element can be added.  */
ساكن بنية outlist *outlist_end = &outlist_head;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
ساكن mcel_t tab;

/* The output separator to use, and its length in bytes.  */
ساكن حرف ثابت *output_separator = " ";
ساكن idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
ساكن تعداد
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_input_order;

تعداد
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_LINE_OPTION
};


ساكن بنية option ثابت longopts[] =
{
  {"ignore-case", no_argument, عدم, 'i'},
  {"check-order", no_argument, عدم, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, عدم, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, عدم, 'z'},
  {"header", no_argument, عدم, HEADER_LINE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Used to print non-joining lines */
ساكن بنية line uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
ساكن bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
ساكن bool join_header_lines;

/* The character marking end of line. Default to \n. */
ساكن حرف eolchar = '\n';

فراغ
usage (صحيح status)
{
  𐡄𐡍 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
For each pair of input lines with identical join fields, اكتب a line to\n\
standard output.  The افتراضي join field is the first, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, اقرأ standard input.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable lines from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding to FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (empty) input fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-حالة\n\
         ignore differences in حالة when comparing fields\n\
"));
      oputs (_("\
  -j FIELD\n\
         equivalent to '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT طالما constructing output line\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as input and output field separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output lines\n\
"));
      oputs (_("\
  -1 FIELD\n\
         join on this FIELD of file 1\n\
"));
      oputs (_("\
  -2 FIELD\n\
         join on this FIELD of file 2\n\
"));
      oputs (_("\
      --check-order\n\
         check that the input is correctly sorted, even\n\
         𐡄𐡍 all input lines are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         𐡏𐡁𐡃 not check that the input is correctly sorted\n\
"));
      oputs (_("\
      --header\n\
         treat the first line in each file as field headers,\n\
         print them without trying to pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         line delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
وإلا fields are separated by CHAR.  Any FIELD is a field number counted\n\
from 1.  FORMAT is one or more comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join field,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
line of each file determines the number of fields output لكل each line.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" 𐡄𐡍 'join' has no options,\n\
or use \"join -t ''\" 𐡄𐡍 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the input is not sorted and some lines cannot be joined, a\n\
warning message will be given.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

ساكن فراغ
extract_field (بنية line *line, حرف *field, idx_t len)
{
  𐡄𐡍 (line->nfields >= line->nfields_allocated)
    line->fields = xpalloc (line->fields, &line->nfields_allocated, 1,
                            -1, حجم *line->fields);
  idx_t ثابت field_index = line->nfields++;
  line->fields[field_index].beg = field;
  line->fields[field_index].len = len;
}

ساكن bool
eq_tab (mcel_t g)
{
  أرجع mcel_eq (g, tab);
}

ساكن bool
newline_or_blank (mcel_t g)
{
  أرجع g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

ساكن فراغ
xfields (بنية line *line)
{
  حرف *ptr = line->buf.buffer;
  حرف ثابت *lim = ptr + line->buf.length - 1;

  𐡄𐡍 (ptr == lim)
    أرجع;

  𐡄𐡍 (!tab.len)
    طالما ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        حرف *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (line, ptr, sep - ptr);
        ptr = sep;
      }
  وإلا
    {
      𐡄𐡍 (tab.ch != '\n')
        لكل (حرف *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (line, ptr, sep - ptr);

      extract_field (line, ptr, lim - ptr);
    }
}

ساكن فراغ
freeline (بنية line *line)
{
  𐡄𐡍 (line == عدم)
    أرجع;
  حرر (line->fields);
  line->fields = عدم;
  line->nfields_allocated = 0;
  حرر (line->buf.buffer);
  line->buf.buffer = عدم;
  line->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

ساكن صحيح
keycmp (بنية line ثابت *line1, بنية line ثابت *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  حرف *beg1;
  حرف *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  صحيح diff;

  𐡄𐡍 (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  وإلا
    {
      beg1 = عدم;
      len1 = 0;
    }

  𐡄𐡍 (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  وإلا
    {
      beg2 = عدم;
      len2 = 0;
    }

  𐡄𐡍 (len1 == 0)
    أرجع len2 == 0 ? 0 : -1;
  𐡄𐡍 (len2 == 0)
    أرجع 1;

  𐡄𐡍 (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  وإلا
    {
      𐡄𐡍 (hard_LC_COLLATE)
        أرجع xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  𐡄𐡍 (diff)
    أرجع diff;
  أرجع _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

ساكن فراغ
check_order (ثابت بنية line *prev,
             ثابت بنية line *current,
             صحيح whatfile)
{
  𐡄𐡍 (check_input_order != CHECK_ORDER_DISABLED
      && ((check_input_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      𐡄𐡍 (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          𐡄𐡍 (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              𐡄𐡍 (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (INT_MAX, len);

              error ((check_input_order == CHECK_ORDER_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], line_no[whatfile - 1],
                     (صحيح) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

ساكن مضمن فراغ
reset_line (بنية line *line)
{
  line->nfields = 0;
}

ساكن بنية line *
init_linep (بنية line **linep)
{
  بنية line *line = xzalloc (حجم *line);
  *linep = line;
  أرجع line;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

ساكن bool
get_line (FILE *fp, بنية line **linep, صحيح which)
{
  بنية line *line = *linep;

  𐡄𐡍 (line == prevline[which - 1])
    {
      SWAPLINES (line, spareline[which - 1]);
      *linep = line;
    }

  𐡄𐡍 (line)
    reset_line (line);
  وإلا
    line = init_linep (linep);

  𐡄𐡍 (! readlinebuffer_delim (&line->buf, fp, eolchar))
    {
      𐡄𐡍 (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (line);
      أرجع false;
    }
  ++line_no[which - 1];

  xfields (line);

  𐡄𐡍 (prevline[which - 1])
    check_order (prevline[which - 1], line, which);

  prevline[which - 1] = line;
  أرجع true;
}

ساكن فراغ
free_spareline (فراغ)
{
  لكل (idx_t i = 0; i < countof (spareline); i++)
    {
      𐡄𐡍 (spareline[i])
        {
          freeline (spareline[i]);
          حرر (spareline[i]);
        }
    }
}

ساكن فراغ
initseq (بنية seq *seq)
{
  seq->count = 0;
  seq->alloc = 0;
  seq->lines = عدم;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

ساكن bool
getseq (FILE *fp, بنية seq *seq, صحيح whichfile)
{
  𐡄𐡍 (seq->count == seq->alloc)
    {
      seq->lines = xpalloc (seq->lines, &seq->alloc, 1, -1, حجم *seq->lines);
      لكل (idx_t i = seq->count; i < seq->alloc; i++)
        seq->lines[i] = عدم;
    }

  𐡄𐡍 (get_line (fp, &seq->lines[seq->count], whichfile))
    {
      ++seq->count;
      أرجع true;
    }
  أرجع false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
ساكن bool
advance_seq (FILE *fp, بنية seq *seq, bool first, صحيح whichfile)
{
  𐡄𐡍 (first)
    seq->count = 0;

  أرجع getseq (fp, seq, whichfile);
}

ساكن فراغ
delseq (بنية seq *seq)
{
  لكل (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->lines[i]);
      حرر (seq->lines[i]);
    }
  حرر (seq->lines);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

ساكن فراغ
prfield (idx_t n, بنية line ثابت *line)
{
  𐡄𐡍 (n < line->nfields)
    {
      idx_t len = line->fields[n].len;
      𐡄𐡍 (len)
        fwrite (line->fields[n].beg, 1, len, stdout);
      وإلا 𐡄𐡍 (empty_filler)
        fputs (empty_filler, stdout);
    }
  وإلا 𐡄𐡍 (empty_filler)
    fputs (empty_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

ساكن فراغ
prfields (بنية line ثابت *line, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : line->nfields;

  لكل (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, line);
    }
  لكل (idx_t i = join_field + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, line);
    }
}

/* Print the join of LINE1 and LINE2.  */

ساكن فراغ
prjoin (بنية line ثابت *line1, بنية line ثابت *line2)
{
  ثابت بنية outlist *outlist;
  idx_t field;
  بنية line ثابت *line;

  outlist = outlist_head.next;
  𐡄𐡍 (outlist)
    {
      ثابت بنية outlist *o;

      o = outlist;
      طالما (true)
        {
          𐡄𐡍 (o->file == 0)
            {
              𐡄𐡍 (line1 == &uni_blank)
                {
                  line = line2;
                  field = join_field_2;
                }
              وإلا
                {
                  line = line1;
                  field = join_field_1;
                }
            }
          وإلا
            {
              line = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, line);
          o = o->next;
          𐡄𐡍 (o == عدم)
            اكسر;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      putchar (eolchar);
    }
  وإلا
    {
      𐡄𐡍 (line1 == &uni_blank)
        {
          line = line2;
          field = join_field_2;
        }
      وإلا
        {
          line = line1;
          field = join_field_1;
        }

      /* Output the join field.  */
      prfield (field, line);

      /* Output other fields.  */
      prfields (line1, join_field_1, autocount_1);
      prfields (line2, join_field_2, autocount_2);

      putchar (eolchar);
    }

  𐡄𐡍 (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

ساكن فراغ
join (FILE *fp1, FILE *fp2)
{
  بنية seq seq1, seq2;
  صحيح diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  𐡄𐡍 (autoformat)
    {
      autocount_1 = seq1.count ? seq1.lines[0]->nfields : 0;
      autocount_2 = seq2.count ? seq2.lines[0]->nfields : 0;
    }

  𐡄𐡍 (join_header_lines && (seq1.count || seq2.count))
    {
      بنية line ثابت *hline1 = seq1.count ? seq1.lines[0] : &uni_blank;
      بنية line ثابت *hline2 = seq2.count ? seq2.lines[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = عدم;
      prevline[1] = عدم;
      𐡄𐡍 (seq1.count)
        advance_seq (fp1, &seq1, true, 1);
      𐡄𐡍 (seq2.count)
        advance_seq (fp2, &seq2, true, 2);
    }

  طالما (seq1.count && seq2.count)
    {
      diff = keycmp (seq1.lines[0], seq2.lines[0],
                     join_field_1, join_field_2);
      𐡄𐡍 (diff < 0)
        {
          𐡄𐡍 (print_unpairables_1)
            prjoin (seq1.lines[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          استمر;
        }
      𐡄𐡍 (diff > 0)
        {
          𐡄𐡍 (print_unpairables_2)
            prjoin (&uni_blank, seq2.lines[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          استمر;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      𐡏𐡁𐡃
        𐡄𐡍 (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.count;
            اكسر;
          }
      طالما (!keycmp (seq1.lines[seq1.count - 1], seq2.lines[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      𐡏𐡁𐡃
        𐡄𐡍 (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.count;
            اكسر;
          }
      طالما (!keycmp (seq1.lines[0], seq2.lines[seq2.count - 1],
                      join_field_1, join_field_2));

      𐡄𐡍 (print_pairables)
        {
          لكل (idx_t i = 0; i < seq1.count - 1; ++i)
            {
              لكل (idx_t j = 0; j < seq2.count - 1; ++j)
                prjoin (seq1.lines[i], seq2.lines[j]);
            }
        }

      𐡄𐡍 (!eof1)
        {
          SWAPLINES (seq1.lines[0], seq1.lines[seq1.count - 1]);
          seq1.count = 1;
        }
      وإلا
        seq1.count = 0;

      𐡄𐡍 (!eof2)
        {
          SWAPLINES (seq2.lines[0], seq2.lines[seq2.count - 1]);
          seq2.count = 1;
        }
      وإلا
        seq2.count = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  بنية line *line = عدم;
  bool checktail = false;

  𐡄𐡍 (check_input_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  𐡄𐡍 ((print_unpairables_1 || checktail) && seq1.count)
    {
      𐡄𐡍 (print_unpairables_1)
        prjoin (seq1.lines[0], &uni_blank);
      𐡄𐡍 (seq2.count)
        seen_unpairable = true;
      طالما (get_line (fp1, &line, 1))
        {
          𐡄𐡍 (print_unpairables_1)
            prjoin (line, &uni_blank);
          𐡄𐡍 (issued_disorder_warning[0] && !print_unpairables_1)
            اكسر;
        }
    }

  𐡄𐡍 ((print_unpairables_2 || checktail) && seq2.count)
    {
      𐡄𐡍 (print_unpairables_2)
        prjoin (&uni_blank, seq2.lines[0]);
      𐡄𐡍 (seq1.count)
        seen_unpairable = true;
      طالما (get_line (fp2, &line, 2))
        {
          𐡄𐡍 (print_unpairables_2)
            prjoin (&uni_blank, line);
          𐡄𐡍 (issued_disorder_warning[1] && !print_unpairables_2)
            اكسر;
        }
    }

  freeline (line);
  حرر (line);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

ساكن فراغ
add_field (صحيح file, idx_t field)
{
  بنية outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || field == 0);

  o = xmalloc (حجم *o);
  o->file = file;
  o->field = field;
  o->next = عدم;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_end->next = o;
  outlist_end = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

ساكن idx_t
string_to_join_field (حرف ثابت *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, عدم, 10, &val, "");
  𐡄𐡍 (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  وإلا 𐡄𐡍 (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  أرجع val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

ساكن فراغ
decode_field_spec (حرف ثابت *s, صحيح *file_index, idx_t *field_index)
{
  /* The first character must be 0, 1, or 2.  */
  بدل (s[0])
    {
    حالة '0':
      𐡄𐡍 (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      اكسر;

    حالة '1':
    حالة '2':
      𐡄𐡍 (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_index = s[0] - '0';
      *field_index = string_to_join_field (s + 2);
      اكسر;

    افتراضي:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

ساكن bool
comma_or_blank (mcel_t g)
{
  أرجع g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

ساكن فراغ
add_field_list (حرف *str)
{
  حرف *p = str;

  𐡏𐡁𐡃
    {
      صحيح file_index;
      idx_t field_index;
      حرف ثابت *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      𐡄𐡍 (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_index, &field_index);
      add_field (file_index, field_index);
    }
  طالما (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

ساكن فراغ
set_join_field (ptrdiff_t *var, idx_t val)
{
  𐡄𐡍 (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

تعداد operand_status
  {
    /* This argument must be an operand, i.e., one of the files to be
       joined.  */
    MUST_BE_OPERAND,

    /* This might be the argument of the preceding -j1 or -j2 option,
       or it might be an operand.  */
    MIGHT_BE_J1_ARG,
    MIGHT_BE_J2_ARG,

    /* This might be the argument of the preceding -o option, or it might be
       an operand.  */
    MIGHT_BE_O_ARG
  };

/* Add NAME to the array of input file NAMES with operand statuses
   OPERAND_STATUS; currently there are NFILES names in the list.  */

ساكن فراغ
add_file_name (حرف *name, حرف *names[2],
               صحيح operand_status[2], صحيح joption_count[2], صحيح *nfiles,
               صحيح *prev_optc_status, صحيح *optc_status)
{
  صحيح n = *nfiles;

  𐡄𐡍 (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      حرف *arg = names[op0];
      بدل (operand_status[op0])
        {
        حالة MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        حالة MIGHT_BE_J1_ARG:
          joption_count[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          اكسر;

        حالة MIGHT_BE_J2_ARG:
          joption_count[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          اكسر;

        حالة MIGHT_BE_O_ARG:
          add_field_list (arg);
          اكسر;
        }
      𐡄𐡍 (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  𐡄𐡍 (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

صحيح
main (صحيح argc, حرف **argv)
{
  صحيح optc_status;
  صحيح prev_optc_status = MUST_BE_OPERAND;
  صحيح operand_status[2];
  صحيح joption_count[2] = { 0, 0 };
  FILE *fp1, *fp2;
  صحيح optc;
  صحيح nfiles = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  طالما ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, عدم))
         != -1)
    {
      optc_status = MUST_BE_OPERAND;

      بدل (optc)
        {
        حالة 'v':
            print_pairables = false;
            FALLTHROUGH;

        حالة 'a':
          {
            long صحيح val;
            𐡄𐡍 (xstrtol (optarg, عدم, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            𐡄𐡍 (val == 1)
              print_unpairables_1 = true;
            وإلا
              print_unpairables_2 = true;
          }
          اكسر;

        حالة 'e':
          𐡄𐡍 (empty_filler && ! streq (empty_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          empty_filler = optarg;
          اكسر;

        حالة 'i':
          ignore_case = true;
          اكسر;

        حالة '1':
          set_join_field (&join_field_1, string_to_join_field (optarg));
          اكسر;

        حالة '2':
          set_join_field (&join_field_2, string_to_join_field (optarg));
          اكسر;

        حالة 'j':
          𐡄𐡍 ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_count[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          وإلا
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          اكسر;

        حالة 'o':
          𐡄𐡍 (streq (optarg, "auto"))
            autoformat = true;
          وإلا
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          اكسر;

        حالة 't':
          {
            mcel_t newtab;
            𐡄𐡍 (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            وإلا 𐡄𐡍 (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            وإلا
              {
                newtab = mcel_scanz (optarg);
                𐡄𐡍 (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            𐡄𐡍 (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          اكسر;

        حالة 'z':
          eolchar = 0;
          اكسر;

        حالة NOCHECK_ORDER_OPTION:
          check_input_order = CHECK_ORDER_DISABLED;
          اكسر;

        حالة CHECK_ORDER_OPTION:
          check_input_order = CHECK_ORDER_ENABLED;
          اكسر;

        حالة 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_count,
                         &nfiles, &prev_optc_status, &optc_status);
          اكسر;

        حالة HEADER_LINE_OPTION:
          join_header_lines = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }

      prev_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  prev_optc_status = MUST_BE_OPERAND;
  طالما (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_count,
                   &nfiles, &prev_optc_status, &optc_status);

  𐡄𐡍 (nfiles != 2)
    {
      𐡄𐡍 (nfiles == 0)
        error (0, 0, _("missing operand"));
      وإلا
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  لكل (صحيح i = 0; i < 2; i++)
    𐡄𐡍 (joption_count[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  𐡄𐡍 (join_field_1 < 0)
    join_field_1 = 0;
  𐡄𐡍 (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? stdin : fopen (g_names[0], "r");
  𐡄𐡍 (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? stdin : fopen (g_names[1], "r");
  𐡄𐡍 (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));
  𐡄𐡍 (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  𐡄𐡍 (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  𐡄𐡍 (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));

  𐡄𐡍 (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  وإلا
    أرجع EXIT_SUCCESS;
}
