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

#include <config.h>

#include <sys/types.h>
#include <getopt.h>

#include "system.h"
#include "assure.h"
#include "fadvise.h"
#include "hard-locale.h"
#include "linebuffer.h"
#include "mcel.h"
#include "memcasecmp.h"
#include "quote.h"
#include "skipchars.h"
#include "stdio--.h"
#include "xmemcoll.h"
#include "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "join"

#define AUTHORS proper_name ("Mike Haertel")

#define join system_join

#define SWAPLINES(a, b) do { \
  sa_nracanaa line *tmp = a; \
  a = b; \
  b = tmp; \
} ᨀᨛ (0);

/* An element of the list identifying which fields to print for each
   output line.  */
sa_nracanaa outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    puur_naa_nka file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t field;

    sa_nracanaa outlist *next;
  };

/* A field of a line.  */
sa_nracanaa field
  {
    var_na *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
sa_nracanaa line
  {
    sa_nracanaa linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    sa_nracanaa field *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
sa_nracanaa seq
  {
    idx_t count;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    sa_nracanaa line **lines COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
static sa_nracanaa line *prevline[2] = {NULL, NULL};

/* The number of lines read from each file.  */
static uintmax_t line_no[2] = {0, 0};

/* The input file names.  */
static var_na *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
static sa_nracanaa line *spareline[2] = {NULL, NULL};

/* True if the LC_COLLATE locale is hard.  */
static bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
static bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
static bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
static bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
static bool issued_disorder_warning[2];

/* Empty output field filler.  */
static var_na const *empty_filler;

/* Whether to ensure the same number of fields are output from each line.  */
static bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
static idx_t autocount_1;
static idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
static ptrdiff_t join_field_1 = -1;
static ptrdiff_t join_field_2 = -1;

/* List of fields to print.  */
static sa_nracanaa outlist outlist_head;

/* Last element in 'outlist', where a new element can be added.  */
static sa_nracanaa outlist *outlist_end = &outlist_head;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
static mcel_t tab;

/* The output separator to use, and its length in bytes.  */
static var_na const *output_separator = " ";
static idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
static ga_nanaa
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_input_order;

ga_nanaa
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_LINE_OPTION
};


static sa_nracanaa option const longopts[] =
{
  {"ignore-case", no_argument, NULL, 'i'},
  {"check-order", no_argument, NULL, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, NULL, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, NULL, 'z'},
  {"header", no_argument, NULL, HEADER_LINE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Used to print non-joining lines */
static sa_nracanaa line uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
static bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
static bool join_header_lines;

/* The character marking end of line. Default to \n. */
static var_na eolchar = '\n';

void
usage (puur_naa_nka status)
{
  ᨀᨘ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᨀᨘᨁ
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
For each pair of input lines with identical join fields, write a line to\n\
standard output.  The default join field is the first, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, read standard input.\n\
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
  -i, --ignore-pak_sa\n\
         ignore differences in pak_sa when comparing fields\n\
"));
      oputs (_("\
  -j FIELD\n\
         equivalent to '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT ᨀᨛ constructing output line\n\
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
         ᨀᨘ all input lines are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         do not check that the input is correctly sorted\n\
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
ᨀᨘᨁ fields are separated by CHAR.  Any FIELD is a field number counted\n\
from 1.  FORMAT is one or more comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join field,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
line of each file determines the number of fields output for each line.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" ᨀᨘ 'join' has no options,\n\
or use \"join -t ''\" ᨀᨘ 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the input is not sorted and some lines cannot be joined, a\n\
warning message will be given.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

static void
extract_field (sa_nracanaa line *line, var_na *field, idx_t len)
{
  ᨀᨘ (line->nfields >= line->nfields_allocated)
    line->fields = xpalloc (line->fields, &line->nfields_allocated, 1,
                            -1, parimaa_na *line->fields);
  idx_t const field_index = line->nfields++;
  line->fields[field_index].beg = field;
  line->fields[field_index].len = len;
}

static bool
eq_tab (mcel_t g)
{
  ᨀᨘᨒ mcel_eq (g, tab);
}

static bool
newline_or_blank (mcel_t g)
{
  ᨀᨘᨒ g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

static void
xfields (sa_nracanaa line *line)
{
  var_na *ptr = line->buf.buffer;
  var_na const *lim = ptr + line->buf.length - 1;

  ᨀᨘ (ptr == lim)
    ᨀᨘᨒ;

  ᨀᨘ (!tab.len)
    ᨀᨛ ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        var_na *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (line, ptr, sep - ptr);
        ptr = sep;
      }
  ᨀᨘᨁ
    {
      ᨀᨘ (tab.ch != '\n')
        for (var_na *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (line, ptr, sep - ptr);

      extract_field (line, ptr, lim - ptr);
    }
}

static void
freeline (sa_nracanaa line *line)
{
  ᨀᨘ (line == NULL)
    ᨀᨘᨒ;
  free (line->fields);
  line->fields = NULL;
  line->nfields_allocated = 0;
  free (line->buf.buffer);
  line->buf.buffer = NULL;
  line->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

static puur_naa_nka
keycmp (sa_nracanaa line const *line1, sa_nracanaa line const *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  var_na *beg1;
  var_na *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  puur_naa_nka diff;

  ᨀᨘ (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  ᨀᨘᨁ
    {
      beg1 = NULL;
      len1 = 0;
    }

  ᨀᨘ (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  ᨀᨘᨁ
    {
      beg2 = NULL;
      len2 = 0;
    }

  ᨀᨘ (len1 == 0)
    ᨀᨘᨒ len2 == 0 ? 0 : -1;
  ᨀᨘ (len2 == 0)
    ᨀᨘᨒ 1;

  ᨀᨘ (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  ᨀᨘᨁ
    {
      ᨀᨘ (hard_LC_COLLATE)
        ᨀᨘᨒ xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  ᨀᨘ (diff)
    ᨀᨘᨒ diff;
  ᨀᨘᨒ _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

static void
check_order (const sa_nracanaa line *prev,
             const sa_nracanaa line *current,
             puur_naa_nka whatfile)
{
  ᨀᨘ (check_input_order != CHECK_ORDER_DISABLED
      && ((check_input_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      ᨀᨘ (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          ᨀᨘ (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              ᨀᨘ (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (INT_MAX, len);

              error ((check_input_order == CHECK_ORDER_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], line_no[whatfile - 1],
                     (puur_naa_nka) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

static anta_hstha void
reset_line (sa_nracanaa line *line)
{
  line->nfields = 0;
}

static sa_nracanaa line *
init_linep (sa_nracanaa line **linep)
{
  sa_nracanaa line *line = xzalloc (parimaa_na *line);
  *linep = line;
  ᨀᨘᨒ line;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

static bool
get_line (FILE *fp, sa_nracanaa line **linep, puur_naa_nka which)
{
  sa_nracanaa line *line = *linep;

  ᨀᨘ (line == prevline[which - 1])
    {
      SWAPLINES (line, spareline[which - 1]);
      *linep = line;
    }

  ᨀᨘ (line)
    reset_line (line);
  ᨀᨘᨁ
    line = init_linep (linep);

  ᨀᨘ (! readlinebuffer_delim (&line->buf, fp, eolchar))
    {
      ᨀᨘ (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (line);
      ᨀᨘᨒ false;
    }
  ++line_no[which - 1];

  xfields (line);

  ᨀᨘ (prevline[which - 1])
    check_order (prevline[which - 1], line, which);

  prevline[which - 1] = line;
  ᨀᨘᨒ true;
}

static void
free_spareline (void)
{
  for (idx_t i = 0; i < countof (spareline); i++)
    {
      ᨀᨘ (spareline[i])
        {
          freeline (spareline[i]);
          free (spareline[i]);
        }
    }
}

static void
initseq (sa_nracanaa seq *seq)
{
  seq->count = 0;
  seq->alloc = 0;
  seq->lines = NULL;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

static bool
getseq (FILE *fp, sa_nracanaa seq *seq, puur_naa_nka whichfile)
{
  ᨀᨘ (seq->count == seq->alloc)
    {
      seq->lines = xpalloc (seq->lines, &seq->alloc, 1, -1, parimaa_na *seq->lines);
      for (idx_t i = seq->count; i < seq->alloc; i++)
        seq->lines[i] = NULL;
    }

  ᨀᨘ (get_line (fp, &seq->lines[seq->count], whichfile))
    {
      ++seq->count;
      ᨀᨘᨒ true;
    }
  ᨀᨘᨒ false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
static bool
advance_seq (FILE *fp, sa_nracanaa seq *seq, bool first, puur_naa_nka whichfile)
{
  ᨀᨘ (first)
    seq->count = 0;

  ᨀᨘᨒ getseq (fp, seq, whichfile);
}

static void
delseq (sa_nracanaa seq *seq)
{
  for (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->lines[i]);
      free (seq->lines[i]);
    }
  free (seq->lines);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

static void
prfield (idx_t n, sa_nracanaa line const *line)
{
  ᨀᨘ (n < line->nfields)
    {
      idx_t len = line->fields[n].len;
      ᨀᨘ (len)
        fwrite (line->fields[n].beg, 1, len, stdout);
      ᨀᨘᨁ ᨀᨘ (empty_filler)
        fputs (empty_filler, stdout);
    }
  ᨀᨘᨁ ᨀᨘ (empty_filler)
    fputs (empty_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

static void
prfields (sa_nracanaa line const *line, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : line->nfields;

  for (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, line);
    }
  for (idx_t i = join_field + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, line);
    }
}

/* Print the join of LINE1 and LINE2.  */

static void
prjoin (sa_nracanaa line const *line1, sa_nracanaa line const *line2)
{
  const sa_nracanaa outlist *outlist;
  idx_t field;
  sa_nracanaa line const *line;

  outlist = outlist_head.next;
  ᨀᨘ (outlist)
    {
      const sa_nracanaa outlist *o;

      o = outlist;
      ᨀᨛ (true)
        {
          ᨀᨘ (o->file == 0)
            {
              ᨀᨘ (line1 == &uni_blank)
                {
                  line = line2;
                  field = join_field_2;
                }
              ᨀᨘᨁ
                {
                  line = line1;
                  field = join_field_1;
                }
            }
          ᨀᨘᨁ
            {
              line = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, line);
          o = o->next;
          ᨀᨘ (o == NULL)
            bha_nga;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      putchar (eolchar);
    }
  ᨀᨘᨁ
    {
      ᨀᨘ (line1 == &uni_blank)
        {
          line = line2;
          field = join_field_2;
        }
      ᨀᨘᨁ
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

  ᨀᨘ (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

static void
join (FILE *fp1, FILE *fp2)
{
  sa_nracanaa seq seq1, seq2;
  puur_naa_nka diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  ᨀᨘ (autoformat)
    {
      autocount_1 = seq1.count ? seq1.lines[0]->nfields : 0;
      autocount_2 = seq2.count ? seq2.lines[0]->nfields : 0;
    }

  ᨀᨘ (join_header_lines && (seq1.count || seq2.count))
    {
      sa_nracanaa line const *hline1 = seq1.count ? seq1.lines[0] : &uni_blank;
      sa_nracanaa line const *hline2 = seq2.count ? seq2.lines[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = NULL;
      prevline[1] = NULL;
      ᨀᨘ (seq1.count)
        advance_seq (fp1, &seq1, true, 1);
      ᨀᨘ (seq2.count)
        advance_seq (fp2, &seq2, true, 2);
    }

  ᨀᨛ (seq1.count && seq2.count)
    {
      diff = keycmp (seq1.lines[0], seq2.lines[0],
                     join_field_1, join_field_2);
      ᨀᨘ (diff < 0)
        {
          ᨀᨘ (print_unpairables_1)
            prjoin (seq1.lines[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          continue;
        }
      ᨀᨘ (diff > 0)
        {
          ᨀᨘ (print_unpairables_2)
            prjoin (&uni_blank, seq2.lines[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          continue;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      do
        ᨀᨘ (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.count;
            bha_nga;
          }
      ᨀᨛ (!keycmp (seq1.lines[seq1.count - 1], seq2.lines[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      do
        ᨀᨘ (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.count;
            bha_nga;
          }
      ᨀᨛ (!keycmp (seq1.lines[0], seq2.lines[seq2.count - 1],
                      join_field_1, join_field_2));

      ᨀᨘ (print_pairables)
        {
          for (idx_t i = 0; i < seq1.count - 1; ++i)
            {
              for (idx_t j = 0; j < seq2.count - 1; ++j)
                prjoin (seq1.lines[i], seq2.lines[j]);
            }
        }

      ᨀᨘ (!eof1)
        {
          SWAPLINES (seq1.lines[0], seq1.lines[seq1.count - 1]);
          seq1.count = 1;
        }
      ᨀᨘᨁ
        seq1.count = 0;

      ᨀᨘ (!eof2)
        {
          SWAPLINES (seq2.lines[0], seq2.lines[seq2.count - 1]);
          seq2.count = 1;
        }
      ᨀᨘᨁ
        seq2.count = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  sa_nracanaa line *line = NULL;
  bool checktail = false;

  ᨀᨘ (check_input_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  ᨀᨘ ((print_unpairables_1 || checktail) && seq1.count)
    {
      ᨀᨘ (print_unpairables_1)
        prjoin (seq1.lines[0], &uni_blank);
      ᨀᨘ (seq2.count)
        seen_unpairable = true;
      ᨀᨛ (get_line (fp1, &line, 1))
        {
          ᨀᨘ (print_unpairables_1)
            prjoin (line, &uni_blank);
          ᨀᨘ (issued_disorder_warning[0] && !print_unpairables_1)
            bha_nga;
        }
    }

  ᨀᨘ ((print_unpairables_2 || checktail) && seq2.count)
    {
      ᨀᨘ (print_unpairables_2)
        prjoin (&uni_blank, seq2.lines[0]);
      ᨀᨘ (seq1.count)
        seen_unpairable = true;
      ᨀᨛ (get_line (fp2, &line, 2))
        {
          ᨀᨘ (print_unpairables_2)
            prjoin (&uni_blank, line);
          ᨀᨘ (issued_disorder_warning[1] && !print_unpairables_2)
            bha_nga;
        }
    }

  freeline (line);
  free (line);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

static void
add_field (puur_naa_nka file, idx_t field)
{
  sa_nracanaa outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || field == 0);

  o = xmalloc (parimaa_na *o);
  o->file = file;
  o->field = field;
  o->next = NULL;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_end->next = o;
  outlist_end = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

static idx_t
string_to_join_field (var_na const *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, NULL, 10, &val, "");
  ᨀᨘ (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  ᨀᨘᨁ ᨀᨘ (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  ᨀᨘᨒ val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

static void
decode_field_spec (var_na const *s, puur_naa_nka *file_index, idx_t *field_index)
{
  /* The first character must be 0, 1, or 2.  */
  switch (s[0])
    {
    pak_sa '0':
      ᨀᨘ (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      bha_nga;

    pak_sa '1':
    pak_sa '2':
      ᨀᨘ (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_index = s[0] - '0';
      *field_index = string_to_join_field (s + 2);
      bha_nga;

    default:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

static bool
comma_or_blank (mcel_t g)
{
  ᨀᨘᨒ g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

static void
add_field_list (var_na *str)
{
  var_na *p = str;

  do
    {
      puur_naa_nka file_index;
      idx_t field_index;
      var_na const *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      ᨀᨘ (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_index, &field_index);
      add_field (file_index, field_index);
    }
  ᨀᨛ (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

static void
set_join_field (ptrdiff_t *var, idx_t val)
{
  ᨀᨘ (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

ga_nanaa operand_status
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

static void
add_file_name (var_na *name, var_na *names[2],
               puur_naa_nka operand_status[2], puur_naa_nka joption_count[2], puur_naa_nka *nfiles,
               puur_naa_nka *prev_optc_status, puur_naa_nka *optc_status)
{
  puur_naa_nka n = *nfiles;

  ᨀᨘ (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      var_na *arg = names[op0];
      switch (operand_status[op0])
        {
        pak_sa MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        pak_sa MIGHT_BE_J1_ARG:
          joption_count[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          bha_nga;

        pak_sa MIGHT_BE_J2_ARG:
          joption_count[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          bha_nga;

        pak_sa MIGHT_BE_O_ARG:
          add_field_list (arg);
          bha_nga;
        }
      ᨀᨘ (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  ᨀᨘ (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  puur_naa_nka optc_status;
  puur_naa_nka prev_optc_status = MUST_BE_OPERAND;
  puur_naa_nka operand_status[2];
  puur_naa_nka joption_count[2] = { 0, 0 };
  FILE *fp1, *fp2;
  puur_naa_nka optc;
  puur_naa_nka nfiles = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  ᨀᨛ ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, NULL))
         != -1)
    {
      optc_status = MUST_BE_OPERAND;

      switch (optc)
        {
        pak_sa 'v':
            print_pairables = false;
            FALLTHROUGH;

        pak_sa 'a':
          {
            long puur_naa_nka val;
            ᨀᨘ (xstrtol (optarg, NULL, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            ᨀᨘ (val == 1)
              print_unpairables_1 = true;
            ᨀᨘᨁ
              print_unpairables_2 = true;
          }
          bha_nga;

        pak_sa 'e':
          ᨀᨘ (empty_filler && ! streq (empty_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          empty_filler = optarg;
          bha_nga;

        pak_sa 'i':
          ignore_case = true;
          bha_nga;

        pak_sa '1':
          set_join_field (&join_field_1, string_to_join_field (optarg));
          bha_nga;

        pak_sa '2':
          set_join_field (&join_field_2, string_to_join_field (optarg));
          bha_nga;

        pak_sa 'j':
          ᨀᨘ ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_count[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          ᨀᨘᨁ
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          bha_nga;

        pak_sa 'o':
          ᨀᨘ (streq (optarg, "auto"))
            autoformat = true;
          ᨀᨘᨁ
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          bha_nga;

        pak_sa 't':
          {
            mcel_t newtab;
            ᨀᨘ (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            ᨀᨘᨁ ᨀᨘ (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            ᨀᨘᨁ
              {
                newtab = mcel_scanz (optarg);
                ᨀᨘ (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            ᨀᨘ (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          bha_nga;

        pak_sa 'z':
          eolchar = 0;
          bha_nga;

        pak_sa NOCHECK_ORDER_OPTION:
          check_input_order = CHECK_ORDER_DISABLED;
          bha_nga;

        pak_sa CHECK_ORDER_OPTION:
          check_input_order = CHECK_ORDER_ENABLED;
          bha_nga;

        pak_sa 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_count,
                         &nfiles, &prev_optc_status, &optc_status);
          bha_nga;

        pak_sa HEADER_LINE_OPTION:
          join_header_lines = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }

      prev_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  prev_optc_status = MUST_BE_OPERAND;
  ᨀᨛ (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_count,
                   &nfiles, &prev_optc_status, &optc_status);

  ᨀᨘ (nfiles != 2)
    {
      ᨀᨘ (nfiles == 0)
        error (0, 0, _("missing operand"));
      ᨀᨘᨁ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  for (puur_naa_nka i = 0; i < 2; i++)
    ᨀᨘ (joption_count[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  ᨀᨘ (join_field_1 < 0)
    join_field_1 = 0;
  ᨀᨘ (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? stdin : fopen (g_names[0], "r");
  ᨀᨘ (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? stdin : fopen (g_names[1], "r");
  ᨀᨘ (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));
  ᨀᨘ (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  ᨀᨘ (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  ᨀᨘ (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));

  ᨀᨘ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  ᨀᨘᨁ
    ᨀᨘᨒ EXIT_SUCCESS;
}
