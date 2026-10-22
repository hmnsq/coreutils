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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <getopt.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "hard-locale.h"
#અન્તર્ભાવ "linebuffer.h"
#અન્તર્ભાવ "mcel.h"
#અન્તર્ભાવ "memcasecmp.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "skipchars.h"
#અન્તર્ભાવ "stdio--.h"
#અન્તર્ભાવ "xmemcoll.h"
#અન્તર્ભાવ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "join"

#define AUTHORS proper_name ("Mike Haertel")

#define join system_join

#define SWAPLINES(a, b) કરો { \
  સંરચના લીટી *tmp = a; \
  a = b; \
  b = tmp; \
} જ્યારે (0);

/* An element of the list identifying which fields to print for each
   output line.  */
સંરચના outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    પૂર્ણાંક file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t field;

    સંરચના outlist *next;
  };

/* A field of a line.  */
સંરચના field
  {
    અક્ષર *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
સંરચના લીટી
  {
    સંરચના linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    સંરચના field *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
સંરચના seq
  {
    idx_t ગણતરી;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    સંરચના લીટી **લીટીઓ COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
સ્થિર સંરચના લીટી *prevline[2] = {ખાલી, ખાલી};

/* The number of lines read from each file.  */
સ્થિર uintmax_t લીટી_no[2] = {0, 0};

/* The input file names.  */
સ્થિર અક્ષર *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
સ્થિર સંરચના લીટી *spareline[2] = {ખાલી, ખાલી};

/* True if the LC_COLLATE locale is hard.  */
સ્થિર bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
સ્થિર bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
સ્થિર bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
સ્થિર bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
સ્થિર bool issued_disorder_warning[2];

/* Empty output field filler.  */
સ્થિર અક્ષર અચલ *empty_filler;

/* Whether to ensure the same number of fields are output from each line.  */
સ્થિર bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
સ્થિર idx_t autocount_1;
સ્થિર idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
સ્થિર ptrdiff_t join_field_1 = -1;
સ્થિર ptrdiff_t join_field_2 = -1;

/* List of fields to print.  */
સ્થિર સંરચના outlist outlist_head;

/* Last element in 'outlist', where a new element can be added.  */
સ્થિર સંરચના outlist *outlist_end = &outlist_head;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
સ્થિર mcel_t tab;

/* The output separator to use, and its length in bytes.  */
સ્થિર અક્ષર અચલ *output_separator = " ";
સ્થિર idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
સ્થિર ગણના
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_આગત_order;

ગણના
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_લીટી_OPTION
};


સ્થિર સંરચના option અચલ longopts[] =
{
  {"ignore-case", no_argument, ખાલી, 'i'},
  {"check-order", no_argument, ખાલી, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, ખાલી, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, ખાલી, 'z'},
  {"header", no_argument, ખાલી, HEADER_લીટી_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Used to print non-joining lines */
સ્થિર સંરચના લીટી uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
સ્થિર bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
સ્થિર bool join_header_લીટીઓ;

/* The character marking end of line. Default to \n. */
સ્થિર અક્ષર eolchar = '\n';

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
For each pair of આગત લીટીઓ with identical join fields, લિખ a લીટી to\n\
standard output.  The મૂળભૂત join field is the first, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, પઠ standard આગત.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable લીટીઓ from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding to FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (empty) આગત fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-સ્થિતિ\n\
         ignore differences in સ્થિતિ when comparing fields\n\
"));
      oputs (_("\
  -j FIELD\n\
         equivalent to '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT જ્યારે constructing output લીટી\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as આગત and output field separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output લીટીઓ\n\
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
         check that the આગત is correctly sorted, even\n\
         જો all આગત લીટીઓ are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         કરો not check that the આગત is correctly sorted\n\
"));
      oputs (_("\
      --header\n\
         treat the first લીટી in each file as field headers,\n\
         print them without trying to pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         લીટી delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
નહીંતર fields are separated by CHAR.  Any FIELD is a field સંખ્યા counted\n\
from 1.  FORMAT is one or more comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join field,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
લીટી of each file determines the સંખ્યા of fields output માટે each લીટી.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" જો 'join' has no options,\n\
or use \"join -t ''\" જો 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the આગત is not sorted and some લીટીઓ cannot be joined, a\n\
warning message will be given.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

સ્થિર શૂન્ય
extract_field (સંરચના લીટી *લીટી, અક્ષર *field, idx_t len)
{
  જો (લીટી->nfields >= લીટી->nfields_allocated)
    લીટી->fields = xpalloc (લીટી->fields, &લીટી->nfields_allocated, 1,
                            -1, કદ *લીટી->fields);
  idx_t અચલ field_index = લીટી->nfields++;
  લીટી->fields[field_index].beg = field;
  લીટી->fields[field_index].len = len;
}

સ્થિર bool
eq_tab (mcel_t g)
{
  પાછા mcel_eq (g, tab);
}

સ્થિર bool
newline_or_blank (mcel_t g)
{
  પાછા g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

સ્થિર શૂન્ય
xfields (સંરચના લીટી *લીટી)
{
  અક્ષર *ptr = લીટી->buf.buffer;
  અક્ષર અચલ *lim = ptr + લીટી->buf.length - 1;

  જો (ptr == lim)
    પાછા;

  જો (!tab.len)
    જ્યારે ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        અક્ષર *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (લીટી, ptr, sep - ptr);
        ptr = sep;
      }
  નહીંતર
    {
      જો (tab.ch != '\n')
        માટે (અક્ષર *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (લીટી, ptr, sep - ptr);

      extract_field (લીટી, ptr, lim - ptr);
    }
}

સ્થિર શૂન્ય
freeline (સંરચના લીટી *લીટી)
{
  જો (લીટી == ખાલી)
    પાછા;
  મુક્ત (લીટી->fields);
  લીટી->fields = ખાલી;
  લીટી->nfields_allocated = 0;
  મુક્ત (લીટી->buf.buffer);
  લીટી->buf.buffer = ખાલી;
  લીટી->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

સ્થિર પૂર્ણાંક
keycmp (સંરચના લીટી અચલ *line1, સંરચના લીટી અચલ *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  અક્ષર *beg1;
  અક્ષર *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  પૂર્ણાંક diff;

  જો (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  નહીંતર
    {
      beg1 = ખાલી;
      len1 = 0;
    }

  જો (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  નહીંતર
    {
      beg2 = ખાલી;
      len2 = 0;
    }

  જો (len1 == 0)
    પાછા len2 == 0 ? 0 : -1;
  જો (len2 == 0)
    પાછા 1;

  જો (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  નહીંતર
    {
      જો (hard_LC_COLLATE)
        પાછા xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  જો (diff)
    પાછા diff;
  પાછા _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

સ્થિર શૂન્ય
check_order (અચલ સંરચના લીટી *prev,
             અચલ સંરચના લીટી *current,
             પૂર્ણાંક whatfile)
{
  જો (check_આગત_order != CHECK_ORDER_DISABLED
      && ((check_આગત_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      જો (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          જો (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              જો (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (INT_MAX, len);

              error ((check_આગત_order == CHECK_ORDER_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], લીટી_no[whatfile - 1],
                     (પૂર્ણાંક) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

સ્થિર અંતઃરેખ શૂન્ય
reset_લીટી (સંરચના લીટી *લીટી)
{
  લીટી->nfields = 0;
}

સ્થિર સંરચના લીટી *
init_linep (સંરચના લીટી **linep)
{
  સંરચના લીટી *લીટી = xzalloc (કદ *લીટી);
  *linep = લીટી;
  પાછા લીટી;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

સ્થિર bool
get_લીટી (FILE *fp, સંરચના લીટી **linep, પૂર્ણાંક which)
{
  સંરચના લીટી *લીટી = *linep;

  જો (લીટી == prevline[which - 1])
    {
      SWAPLINES (લીટી, spareline[which - 1]);
      *linep = લીટી;
    }

  જો (લીટી)
    reset_લીટી (લીટી);
  નહીંતર
    લીટી = init_linep (linep);

  જો (! readlinebuffer_delim (&લીટી->buf, fp, eolchar))
    {
      જો (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (લીટી);
      પાછા false;
    }
  ++લીટી_no[which - 1];

  xfields (લીટી);

  જો (prevline[which - 1])
    check_order (prevline[which - 1], લીટી, which);

  prevline[which - 1] = લીટી;
  પાછા true;
}

સ્થિર શૂન્ય
free_spareline (શૂન્ય)
{
  માટે (idx_t i = 0; i < countof (spareline); i++)
    {
      જો (spareline[i])
        {
          freeline (spareline[i]);
          મુક્ત (spareline[i]);
        }
    }
}

સ્થિર શૂન્ય
initseq (સંરચના seq *seq)
{
  seq->ગણતરી = 0;
  seq->alloc = 0;
  seq->લીટીઓ = ખાલી;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

સ્થિર bool
getseq (FILE *fp, સંરચના seq *seq, પૂર્ણાંક whichfile)
{
  જો (seq->ગણતરી == seq->alloc)
    {
      seq->લીટીઓ = xpalloc (seq->લીટીઓ, &seq->alloc, 1, -1, કદ *seq->લીટીઓ);
      માટે (idx_t i = seq->ગણતરી; i < seq->alloc; i++)
        seq->લીટીઓ[i] = ખાલી;
    }

  જો (get_લીટી (fp, &seq->લીટીઓ[seq->ગણતરી], whichfile))
    {
      ++seq->ગણતરી;
      પાછા true;
    }
  પાછા false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
સ્થિર bool
advance_seq (FILE *fp, સંરચના seq *seq, bool first, પૂર્ણાંક whichfile)
{
  જો (first)
    seq->ગણતરી = 0;

  પાછા getseq (fp, seq, whichfile);
}

સ્થિર શૂન્ય
delseq (સંરચના seq *seq)
{
  માટે (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->લીટીઓ[i]);
      મુક્ત (seq->લીટીઓ[i]);
    }
  મુક્ત (seq->લીટીઓ);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

સ્થિર શૂન્ય
prfield (idx_t n, સંરચના લીટી અચલ *લીટી)
{
  જો (n < લીટી->nfields)
    {
      idx_t len = લીટી->fields[n].len;
      જો (len)
        fwrite (લીટી->fields[n].beg, 1, len, stdout);
      નહીંતર જો (empty_filler)
        fputs (empty_filler, stdout);
    }
  નહીંતર જો (empty_filler)
    fputs (empty_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

સ્થિર શૂન્ય
prfields (સંરચના લીટી અચલ *લીટી, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : લીટી->nfields;

  માટે (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, લીટી);
    }
  માટે (idx_t i = join_field + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, લીટી);
    }
}

/* Print the join of LINE1 and LINE2.  */

સ્થિર શૂન્ય
prjoin (સંરચના લીટી અચલ *line1, સંરચના લીટી અચલ *line2)
{
  અચલ સંરચના outlist *outlist;
  idx_t field;
  સંરચના લીટી અચલ *લીટી;

  outlist = outlist_head.next;
  જો (outlist)
    {
      અચલ સંરચના outlist *o;

      o = outlist;
      જ્યારે (true)
        {
          જો (o->file == 0)
            {
              જો (line1 == &uni_blank)
                {
                  લીટી = line2;
                  field = join_field_2;
                }
              નહીંતર
                {
                  લીટી = line1;
                  field = join_field_1;
                }
            }
          નહીંતર
            {
              લીટી = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, લીટી);
          o = o->next;
          જો (o == ખાલી)
            તોડો;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      putchar (eolchar);
    }
  નહીંતર
    {
      જો (line1 == &uni_blank)
        {
          લીટી = line2;
          field = join_field_2;
        }
      નહીંતર
        {
          લીટી = line1;
          field = join_field_1;
        }

      /* Output the join field.  */
      prfield (field, લીટી);

      /* Output other fields.  */
      prfields (line1, join_field_1, autocount_1);
      prfields (line2, join_field_2, autocount_2);

      putchar (eolchar);
    }

  જો (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

સ્થિર શૂન્ય
join (FILE *fp1, FILE *fp2)
{
  સંરચના seq seq1, seq2;
  પૂર્ણાંક diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  જો (autoformat)
    {
      autocount_1 = seq1.ગણતરી ? seq1.લીટીઓ[0]->nfields : 0;
      autocount_2 = seq2.ગણતરી ? seq2.લીટીઓ[0]->nfields : 0;
    }

  જો (join_header_લીટીઓ && (seq1.ગણતરી || seq2.ગણતરી))
    {
      સંરચના લીટી અચલ *hline1 = seq1.ગણતરી ? seq1.લીટીઓ[0] : &uni_blank;
      સંરચના લીટી અચલ *hline2 = seq2.ગણતરી ? seq2.લીટીઓ[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = ખાલી;
      prevline[1] = ખાલી;
      જો (seq1.ગણતરી)
        advance_seq (fp1, &seq1, true, 1);
      જો (seq2.ગણતરી)
        advance_seq (fp2, &seq2, true, 2);
    }

  જ્યારે (seq1.ગણતરી && seq2.ગણતરી)
    {
      diff = keycmp (seq1.લીટીઓ[0], seq2.લીટીઓ[0],
                     join_field_1, join_field_2);
      જો (diff < 0)
        {
          જો (print_unpairables_1)
            prjoin (seq1.લીટીઓ[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          ચાલુ;
        }
      જો (diff > 0)
        {
          જો (print_unpairables_2)
            prjoin (&uni_blank, seq2.લીટીઓ[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          ચાલુ;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      કરો
        જો (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.ગણતરી;
            તોડો;
          }
      જ્યારે (!keycmp (seq1.લીટીઓ[seq1.ગણતરી - 1], seq2.લીટીઓ[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      કરો
        જો (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.ગણતરી;
            તોડો;
          }
      જ્યારે (!keycmp (seq1.લીટીઓ[0], seq2.લીટીઓ[seq2.ગણતરી - 1],
                      join_field_1, join_field_2));

      જો (print_pairables)
        {
          માટે (idx_t i = 0; i < seq1.ગણતરી - 1; ++i)
            {
              માટે (idx_t j = 0; j < seq2.ગણતરી - 1; ++j)
                prjoin (seq1.લીટીઓ[i], seq2.લીટીઓ[j]);
            }
        }

      જો (!eof1)
        {
          SWAPLINES (seq1.લીટીઓ[0], seq1.લીટીઓ[seq1.ગણતરી - 1]);
          seq1.ગણતરી = 1;
        }
      નહીંતર
        seq1.ગણતરી = 0;

      જો (!eof2)
        {
          SWAPLINES (seq2.લીટીઓ[0], seq2.લીટીઓ[seq2.ગણતરી - 1]);
          seq2.ગણતરી = 1;
        }
      નહીંતર
        seq2.ગણતરી = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  સંરચના લીટી *લીટી = ખાલી;
  bool checktail = false;

  જો (check_આગત_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  જો ((print_unpairables_1 || checktail) && seq1.ગણતરી)
    {
      જો (print_unpairables_1)
        prjoin (seq1.લીટીઓ[0], &uni_blank);
      જો (seq2.ગણતરી)
        seen_unpairable = true;
      જ્યારે (get_લીટી (fp1, &લીટી, 1))
        {
          જો (print_unpairables_1)
            prjoin (લીટી, &uni_blank);
          જો (issued_disorder_warning[0] && !print_unpairables_1)
            તોડો;
        }
    }

  જો ((print_unpairables_2 || checktail) && seq2.ગણતરી)
    {
      જો (print_unpairables_2)
        prjoin (&uni_blank, seq2.લીટીઓ[0]);
      જો (seq1.ગણતરી)
        seen_unpairable = true;
      જ્યારે (get_લીટી (fp2, &લીટી, 2))
        {
          જો (print_unpairables_2)
            prjoin (&uni_blank, લીટી);
          જો (issued_disorder_warning[1] && !print_unpairables_2)
            તોડો;
        }
    }

  freeline (લીટી);
  મુક્ત (લીટી);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

સ્થિર શૂન્ય
add_field (પૂર્ણાંક file, idx_t field)
{
  સંરચના outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || field == 0);

  o = xmalloc (કદ *o);
  o->file = file;
  o->field = field;
  o->next = ખાલી;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_end->next = o;
  outlist_end = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

સ્થિર idx_t
string_to_join_field (અક્ષર અચલ *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, ખાલી, 10, &val, "");
  જો (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  નહીંતર જો (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  પાછા val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

સ્થિર શૂન્ય
decode_field_spec (અક્ષર અચલ *s, પૂર્ણાંક *file_index, idx_t *field_index)
{
  /* The first character must be 0, 1, or 2.  */
  પસંદગી (s[0])
    {
    સ્થિતિ '0':
      જો (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      તોડો;

    સ્થિતિ '1':
    સ્થિતિ '2':
      જો (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_index = s[0] - '0';
      *field_index = string_to_join_field (s + 2);
      તોડો;

    મૂળભૂત:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

સ્થિર bool
comma_or_blank (mcel_t g)
{
  પાછા g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

સ્થિર શૂન્ય
add_field_list (અક્ષર *str)
{
  અક્ષર *p = str;

  કરો
    {
      પૂર્ણાંક file_index;
      idx_t field_index;
      અક્ષર અચલ *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      જો (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_index, &field_index);
      add_field (file_index, field_index);
    }
  જ્યારે (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

સ્થિર શૂન્ય
set_join_field (ptrdiff_t *var, idx_t val)
{
  જો (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

ગણના operand_status
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

સ્થિર શૂન્ય
add_file_name (અક્ષર *name, અક્ષર *names[2],
               પૂર્ણાંક operand_status[2], પૂર્ણાંક joption_ગણતરી[2], પૂર્ણાંક *nfiles,
               પૂર્ણાંક *prev_optc_status, પૂર્ણાંક *optc_status)
{
  પૂર્ણાંક n = *nfiles;

  જો (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      અક્ષર *arg = names[op0];
      પસંદગી (operand_status[op0])
        {
        સ્થિતિ MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        સ્થિતિ MIGHT_BE_J1_ARG:
          joption_ગણતરી[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          તોડો;

        સ્થિતિ MIGHT_BE_J2_ARG:
          joption_ગણતરી[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          તોડો;

        સ્થિતિ MIGHT_BE_O_ARG:
          add_field_list (arg);
          તોડો;
        }
      જો (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  જો (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક optc_status;
  પૂર્ણાંક prev_optc_status = MUST_BE_OPERAND;
  પૂર્ણાંક operand_status[2];
  પૂર્ણાંક joption_ગણતરી[2] = { 0, 0 };
  FILE *fp1, *fp2;
  પૂર્ણાંક optc;
  પૂર્ણાંક nfiles = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  જ્યારે ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, ખાલી))
         != -1)
    {
      optc_status = MUST_BE_OPERAND;

      પસંદગી (optc)
        {
        સ્થિતિ 'v':
            print_pairables = false;
            FALLTHROUGH;

        સ્થિતિ 'a':
          {
            long પૂર્ણાંક val;
            જો (xstrtol (optarg, ખાલી, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            જો (val == 1)
              print_unpairables_1 = true;
            નહીંતર
              print_unpairables_2 = true;
          }
          તોડો;

        સ્થિતિ 'e':
          જો (empty_filler && ! streq (empty_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          empty_filler = optarg;
          તોડો;

        સ્થિતિ 'i':
          ignore_case = true;
          તોડો;

        સ્થિતિ '1':
          set_join_field (&join_field_1, string_to_join_field (optarg));
          તોડો;

        સ્થિતિ '2':
          set_join_field (&join_field_2, string_to_join_field (optarg));
          તોડો;

        સ્થિતિ 'j':
          જો ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_ગણતરી[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          નહીંતર
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          તોડો;

        સ્થિતિ 'o':
          જો (streq (optarg, "auto"))
            autoformat = true;
          નહીંતર
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          તોડો;

        સ્થિતિ 't':
          {
            mcel_t newtab;
            જો (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            નહીંતર જો (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            નહીંતર
              {
                newtab = mcel_scanz (optarg);
                જો (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            જો (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          તોડો;

        સ્થિતિ 'z':
          eolchar = 0;
          તોડો;

        સ્થિતિ NOCHECK_ORDER_OPTION:
          check_આગત_order = CHECK_ORDER_DISABLED;
          તોડો;

        સ્થિતિ CHECK_ORDER_OPTION:
          check_આગત_order = CHECK_ORDER_ENABLED;
          તોડો;

        સ્થિતિ 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_ગણતરી,
                         &nfiles, &prev_optc_status, &optc_status);
          તોડો;

        સ્થિતિ HEADER_લીટી_OPTION:
          join_header_લીટીઓ = true;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }

      prev_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  prev_optc_status = MUST_BE_OPERAND;
  જ્યારે (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_ગણતરી,
                   &nfiles, &prev_optc_status, &optc_status);

  જો (nfiles != 2)
    {
      જો (nfiles == 0)
        error (0, 0, _("missing operand"));
      નહીંતર
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  માટે (પૂર્ણાંક i = 0; i < 2; i++)
    જો (joption_ગણતરી[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  જો (join_field_1 < 0)
    join_field_1 = 0;
  જો (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? stdin : fopen (g_names[0], "r");
  જો (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? stdin : fopen (g_names[1], "r");
  જો (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));
  જો (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  જો (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  જો (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));

  જો (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  નહીંતર
    પાછા EXIT_SUCCESS;
}
