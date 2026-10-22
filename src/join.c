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

#define SWAPLINES(a, b) افعل { \
  بنية لي *tmp = a; \
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
بنية لي
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
    idx_t كدي;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    بنية لي **ليوك COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
ساكن بنية لي *prevline[2] = {عدم, عدم};

/* The number of lines read from each file.  */
ساكن uintmax_t لي_no[2] = {0, 0};

/* The input file names.  */
ساكن حرف *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
ساكن بنية لي *spareline[2] = {عدم, عدم};

/* True if the LC_COLLATE locale is hard.  */
ساكن bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
ساكن bool بغ_unpairables_1, بغ_unpairables_2;

/* If nonzero, print pairable lines.  */
ساكن bool بغ_pairables = true;

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
  } check_شغرو_order;

تعداد
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_لي_OPTION
};


ساكن بنية option ثابت longopts[] =
{
  {"ignore-case", no_argument, عدم, 'i'},
  {"check-order", no_argument, عدم, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, عدم, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, عدم, 'z'},
  {"header", no_argument, عدم, HEADER_لي_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Used to print non-joining lines */
ساكن بنية لي uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
ساكن bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
ساكن bool join_header_ليوك;

/* The character marking end of line. Default to \n. */
ساكن حرف eolchar = '\n';

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
For each pair of شغرو ليوك with identical join fields, اكتب a لي to\n\
standard output.  The افتراضي join field is the first, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, اقرأ standard شغرو.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also بغ_2 unpairable ليوك from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding to FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (empty) شغرو fields with STRING;\n\
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
         obey FORMAT طالما constructing output لي\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as شغرو and output field separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output ليوك\n\
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
         check that the شغرو is correctly sorted, even\n\
         إذا all شغرو ليوك are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         افعل not check that the شغرو is correctly sorted\n\
"));
      oputs (_("\
      --header\n\
         treat the first لي in each file as field headers,\n\
         بغ_2 them without trying to pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         لي delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
وإلا fields are separated by CHAR.  Any FIELD is a field لمب_2 counted\n\
from 1.  FORMAT is one or more comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join field,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
لي of each file determines the لمب_2 of fields output لكل each لي.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" إذا 'join' has no options,\n\
or use \"join -t ''\" إذا 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the شغرو is not sorted and some ليوك cannot be joined, a\n\
warning message will be given.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

ساكن فراغ
extract_field (بنية لي *لي, حرف *field, idx_t len)
{
  إذا (لي->nfields >= لي->nfields_allocated)
    لي->fields = xpalloc (لي->fields, &لي->nfields_allocated, 1,
                            -1, حجم *لي->fields);
  idx_t ثابت field_index = لي->nfields++;
  لي->fields[field_index].beg = field;
  لي->fields[field_index].len = len;
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
xfields (بنية لي *لي)
{
  حرف *ptr = لي->buf.buffer;
  حرف ثابت *lim = ptr + لي->buf.length - 1;

  إذا (ptr == lim)
    أرجع;

  إذا (!tab.len)
    طالما ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        حرف *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (لي, ptr, sep - ptr);
        ptr = sep;
      }
  وإلا
    {
      إذا (tab.ch != '\n')
        لكل (حرف *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (لي, ptr, sep - ptr);

      extract_field (لي, ptr, lim - ptr);
    }
}

ساكن فراغ
freeline (بنية لي *لي)
{
  إذا (لي == عدم)
    أرجع;
  حرر (لي->fields);
  لي->fields = عدم;
  لي->nfields_allocated = 0;
  حرر (لي->buf.buffer);
  لي->buf.buffer = عدم;
  لي->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

ساكن صحيح
keycmp (بنية لي ثابت *line1, بنية لي ثابت *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  حرف *beg1;
  حرف *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  صحيح diff;

  إذا (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  وإلا
    {
      beg1 = عدم;
      len1 = 0;
    }

  إذا (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  وإلا
    {
      beg2 = عدم;
      len2 = 0;
    }

  إذا (len1 == 0)
    أرجع len2 == 0 ? 0 : -1;
  إذا (len2 == 0)
    أرجع 1;

  إذا (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  وإلا
    {
      إذا (hard_LC_COLLATE)
        أرجع xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  إذا (diff)
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
check_order (ثابت بنية لي *prev,
             ثابت بنية لي *current,
             صحيح whatfile)
{
  إذا (check_شغرو_order != CHECK_ORDER_DISABLED
      && ((check_شغرو_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      إذا (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          إذا (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              إذا (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (INT_MAX, len);

              error ((check_شغرو_order == CHECK_ORDER_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], لي_no[whatfile - 1],
                     (صحيح) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

ساكن مضمن فراغ
reset_لي (بنية لي *لي)
{
  لي->nfields = 0;
}

ساكن بنية لي *
init_linep (بنية لي **linep)
{
  بنية لي *لي = xzalloc (حجم *لي);
  *linep = لي;
  أرجع لي;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

ساكن bool
get_لي (FILE *fp, بنية لي **linep, صحيح which)
{
  بنية لي *لي = *linep;

  إذا (لي == prevline[which - 1])
    {
      SWAPLINES (لي, spareline[which - 1]);
      *linep = لي;
    }

  إذا (لي)
    reset_لي (لي);
  وإلا
    لي = init_linep (linep);

  إذا (! readlinebuffer_delim (&لي->buf, fp, eolchar))
    {
      إذا (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (لي);
      أرجع false;
    }
  ++لي_no[which - 1];

  xfields (لي);

  إذا (prevline[which - 1])
    check_order (prevline[which - 1], لي, which);

  prevline[which - 1] = لي;
  أرجع true;
}

ساكن فراغ
free_spareline (فراغ)
{
  لكل (idx_t i = 0; i < countof (spareline); i++)
    {
      إذا (spareline[i])
        {
          freeline (spareline[i]);
          حرر (spareline[i]);
        }
    }
}

ساكن فراغ
initseq (بنية seq *seq)
{
  seq->كدي = 0;
  seq->alloc = 0;
  seq->ليوك = عدم;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

ساكن bool
getseq (FILE *fp, بنية seq *seq, صحيح whichfile)
{
  إذا (seq->كدي == seq->alloc)
    {
      seq->ليوك = xpalloc (seq->ليوك, &seq->alloc, 1, -1, حجم *seq->ليوك);
      لكل (idx_t i = seq->كدي; i < seq->alloc; i++)
        seq->ليوك[i] = عدم;
    }

  إذا (get_لي (fp, &seq->ليوك[seq->كدي], whichfile))
    {
      ++seq->كدي;
      أرجع true;
    }
  أرجع false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
ساكن bool
advance_seq (FILE *fp, بنية seq *seq, bool first, صحيح whichfile)
{
  إذا (first)
    seq->كدي = 0;

  أرجع getseq (fp, seq, whichfile);
}

ساكن فراغ
delseq (بنية seq *seq)
{
  لكل (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->ليوك[i]);
      حرر (seq->ليوك[i]);
    }
  حرر (seq->ليوك);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

ساكن فراغ
prfield (idx_t n, بنية لي ثابت *لي)
{
  إذا (n < لي->nfields)
    {
      idx_t len = لي->fields[n].len;
      إذا (len)
        fwrite (لي->fields[n].beg, 1, len, stdout);
      وإلا إذا (empty_filler)
        fputs (empty_filler, stdout);
    }
  وإلا إذا (empty_filler)
    fputs (empty_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

ساكن فراغ
prfields (بنية لي ثابت *لي, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : لي->nfields;

  لكل (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, لي);
    }
  لكل (idx_t i = join_field + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, لي);
    }
}

/* Print the join of LINE1 and LINE2.  */

ساكن فراغ
prjoin (بنية لي ثابت *line1, بنية لي ثابت *line2)
{
  ثابت بنية outlist *outlist;
  idx_t field;
  بنية لي ثابت *لي;

  outlist = outlist_head.next;
  إذا (outlist)
    {
      ثابت بنية outlist *o;

      o = outlist;
      طالما (true)
        {
          إذا (o->file == 0)
            {
              إذا (line1 == &uni_blank)
                {
                  لي = line2;
                  field = join_field_2;
                }
              وإلا
                {
                  لي = line1;
                  field = join_field_1;
                }
            }
          وإلا
            {
              لي = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, لي);
          o = o->next;
          إذا (o == عدم)
            اكسر;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      putchar (eolchar);
    }
  وإلا
    {
      إذا (line1 == &uni_blank)
        {
          لي = line2;
          field = join_field_2;
        }
      وإلا
        {
          لي = line1;
          field = join_field_1;
        }

      /* Output the join field.  */
      prfield (field, لي);

      /* Output other fields.  */
      prfields (line1, join_field_1, autocount_1);
      prfields (line2, join_field_2, autocount_2);

      putchar (eolchar);
    }

  إذا (ferror (stdout))
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

  إذا (autoformat)
    {
      autocount_1 = seq1.كدي ? seq1.ليوك[0]->nfields : 0;
      autocount_2 = seq2.كدي ? seq2.ليوك[0]->nfields : 0;
    }

  إذا (join_header_ليوك && (seq1.كدي || seq2.كدي))
    {
      بنية لي ثابت *hline1 = seq1.كدي ? seq1.ليوك[0] : &uni_blank;
      بنية لي ثابت *hline2 = seq2.كدي ? seq2.ليوك[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = عدم;
      prevline[1] = عدم;
      إذا (seq1.كدي)
        advance_seq (fp1, &seq1, true, 1);
      إذا (seq2.كدي)
        advance_seq (fp2, &seq2, true, 2);
    }

  طالما (seq1.كدي && seq2.كدي)
    {
      diff = keycmp (seq1.ليوك[0], seq2.ليوك[0],
                     join_field_1, join_field_2);
      إذا (diff < 0)
        {
          إذا (بغ_unpairables_1)
            prjoin (seq1.ليوك[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          استمر;
        }
      إذا (diff > 0)
        {
          إذا (بغ_unpairables_2)
            prjoin (&uni_blank, seq2.ليوك[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          استمر;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      افعل
        إذا (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.كدي;
            اكسر;
          }
      طالما (!keycmp (seq1.ليوك[seq1.كدي - 1], seq2.ليوك[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      افعل
        إذا (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.كدي;
            اكسر;
          }
      طالما (!keycmp (seq1.ليوك[0], seq2.ليوك[seq2.كدي - 1],
                      join_field_1, join_field_2));

      إذا (بغ_pairables)
        {
          لكل (idx_t i = 0; i < seq1.كدي - 1; ++i)
            {
              لكل (idx_t j = 0; j < seq2.كدي - 1; ++j)
                prjoin (seq1.ليوك[i], seq2.ليوك[j]);
            }
        }

      إذا (!eof1)
        {
          SWAPLINES (seq1.ليوك[0], seq1.ليوك[seq1.كدي - 1]);
          seq1.كدي = 1;
        }
      وإلا
        seq1.كدي = 0;

      إذا (!eof2)
        {
          SWAPLINES (seq2.ليوك[0], seq2.ليوك[seq2.كدي - 1]);
          seq2.كدي = 1;
        }
      وإلا
        seq2.كدي = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  بنية لي *لي = عدم;
  bool checktail = false;

  إذا (check_شغرو_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  إذا ((بغ_unpairables_1 || checktail) && seq1.كدي)
    {
      إذا (بغ_unpairables_1)
        prjoin (seq1.ليوك[0], &uni_blank);
      إذا (seq2.كدي)
        seen_unpairable = true;
      طالما (get_لي (fp1, &لي, 1))
        {
          إذا (بغ_unpairables_1)
            prjoin (لي, &uni_blank);
          إذا (issued_disorder_warning[0] && !بغ_unpairables_1)
            اكسر;
        }
    }

  إذا ((بغ_unpairables_2 || checktail) && seq2.كدي)
    {
      إذا (بغ_unpairables_2)
        prjoin (&uni_blank, seq2.ليوك[0]);
      إذا (seq1.كدي)
        seen_unpairable = true;
      طالما (get_لي (fp2, &لي, 2))
        {
          إذا (بغ_unpairables_2)
            prjoin (&uni_blank, لي);
          إذا (issued_disorder_warning[1] && !بغ_unpairables_2)
            اكسر;
        }
    }

  freeline (لي);
  حرر (لي);

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
  إذا (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  وإلا إذا (s_err != LONGINT_OK || val <= 0)
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
      إذا (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      اكسر;

    حالة '1':
    حالة '2':
      إذا (s[1] != '.')
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

  افعل
    {
      صحيح file_index;
      idx_t field_index;
      حرف ثابت *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      إذا (*p)
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
  إذا (0 <= *var && *var != val)
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
               صحيح operand_status[2], صحيح joption_كدي[2], صحيح *nfiles,
               صحيح *prev_optc_status, صحيح *optc_status)
{
  صحيح n = *nfiles;

  إذا (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      حرف *arg = names[op0];
      بدل (operand_status[op0])
        {
        حالة MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        حالة MIGHT_BE_J1_ARG:
          joption_كدي[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          اكسر;

        حالة MIGHT_BE_J2_ARG:
          joption_كدي[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          اكسر;

        حالة MIGHT_BE_O_ARG:
          add_field_list (arg);
          اكسر;
        }
      إذا (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  إذا (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

صحيح
main (صحيح argc, حرف **argv)
{
  صحيح optc_status;
  صحيح prev_optc_status = MUST_BE_OPERAND;
  صحيح operand_status[2];
  صحيح joption_كدي[2] = { 0, 0 };
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
            بغ_pairables = false;
            FALLTHROUGH;

        حالة 'a':
          {
            long صحيح val;
            إذا (xstrtol (optarg, عدم, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            إذا (val == 1)
              بغ_unpairables_1 = true;
            وإلا
              بغ_unpairables_2 = true;
          }
          اكسر;

        حالة 'e':
          إذا (empty_filler && ! streq (empty_filler, optarg))
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
          إذا ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_كدي[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          وإلا
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          اكسر;

        حالة 'o':
          إذا (streq (optarg, "auto"))
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
            إذا (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            وإلا إذا (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            وإلا
              {
                newtab = mcel_scanz (optarg);
                إذا (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            إذا (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          اكسر;

        حالة 'z':
          eolchar = 0;
          اكسر;

        حالة NOCHECK_ORDER_OPTION:
          check_شغرو_order = CHECK_ORDER_DISABLED;
          اكسر;

        حالة CHECK_ORDER_OPTION:
          check_شغرو_order = CHECK_ORDER_ENABLED;
          اكسر;

        حالة 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_كدي,
                         &nfiles, &prev_optc_status, &optc_status);
          اكسر;

        حالة HEADER_لي_OPTION:
          join_header_ليوك = true;
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
    add_file_name (argv[optind++], g_names, operand_status, joption_كدي,
                   &nfiles, &prev_optc_status, &optc_status);

  إذا (nfiles != 2)
    {
      إذا (nfiles == 0)
        error (0, 0, _("missing operand"));
      وإلا
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  لكل (صحيح i = 0; i < 2; i++)
    إذا (joption_كدي[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  إذا (join_field_1 < 0)
    join_field_1 = 0;
  إذا (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? stdin : fopen (g_names[0], "r");
  إذا (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? stdin : fopen (g_names[1], "r");
  إذا (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));
  إذا (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  إذا (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  إذا (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));

  إذا (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  وإلا
    أرجع EXIT_SUCCESS;
}
