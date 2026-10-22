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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <getopt.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "hard-locale.h"
#அந்தர்பாவ "linebuffer.h"
#அந்தர்பாவ "mcel.h"
#அந்தர்பாவ "memcasecmp.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "skipchars.h"
#அந்தர்பாவ "stdio--.h"
#அந்தர்பாவ "xmemcoll.h"
#அந்தர்பாவ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "join"

#define AUTHORS proper_name ("Mike Haertel")

#define join system_join

#define SWAPLINES(a, b) செய் { \
  கட்டமைப்பு வரி *tmp = a; \
  a = b; \
  b = tmp; \
} போது (0);

/* An element of the list identifying which fields to print for each
   output line.  */
கட்டமைப்பு outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    முழு file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t field;

    கட்டமைப்பு outlist *next;
  };

/* A field of a line.  */
கட்டமைப்பு field
  {
    எழுத்து *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
கட்டமைப்பு வரி
  {
    கட்டமைப்பு linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    கட்டமைப்பு field *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
கட்டமைப்பு seq
  {
    idx_t மொத்தம்;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    கட்டமைப்பு வரி **வரிகள் COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
நிலையான கட்டமைப்பு வரி *prevline[2] = {வெற்று, வெற்று};

/* The number of lines read from each file.  */
நிலையான uintmax_t வரி_no[2] = {0, 0};

/* The input file names.  */
நிலையான எழுத்து *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
நிலையான கட்டமைப்பு வரி *spareline[2] = {வெற்று, வெற்று};

/* True if the LC_COLLATE locale is hard.  */
நிலையான bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
நிலையான bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
நிலையான bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
நிலையான bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
நிலையான bool issued_disorder_warning[2];

/* Empty output field filler.  */
நிலையான எழுத்து மாறிலி *empty_filler;

/* Whether to ensure the same number of fields are output from each line.  */
நிலையான bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
நிலையான idx_t autocount_1;
நிலையான idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
நிலையான ptrdiff_t join_field_1 = -1;
நிலையான ptrdiff_t join_field_2 = -1;

/* List of fields to print.  */
நிலையான கட்டமைப்பு outlist outlist_head;

/* Last element in 'outlist', where a new element can be added.  */
நிலையான கட்டமைப்பு outlist *outlist_end = &outlist_head;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
நிலையான mcel_t tab;

/* The output separator to use, and its length in bytes.  */
நிலையான எழுத்து மாறிலி *output_separator = " ";
நிலையான idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
நிலையான எண்ணிக்கை
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_உள்ளீடு_order;

எண்ணிக்கை
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_வரி_OPTION
};


நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"ignore-case", no_argument, வெற்று, 'i'},
  {"check-order", no_argument, வெற்று, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, வெற்று, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, வெற்று, 'z'},
  {"header", no_argument, வெற்று, HEADER_வரி_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Used to print non-joining lines */
நிலையான கட்டமைப்பு வரி uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
நிலையான bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
நிலையான bool join_header_வரிகள்;

/* The character marking end of line. Default to \n. */
நிலையான எழுத்து eolchar = '\n';

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
For each pair of உள்ளீடு வரிகள் with identical join fields, லிக a வரி to\n\
standard output.  The இயல்பு join field is the first, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, பட standard உள்ளீடு.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable வரிகள் from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding to FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (empty) உள்ளீடு fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-நிலை\n\
         ignore differences in நிலை when comparing fields\n\
"));
      oputs (_("\
  -j FIELD\n\
         equivalent to '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT போது constructing output வரி\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as உள்ளீடு and output field separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output வரிகள்\n\
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
         check that the உள்ளீடு is correctly sorted, even\n\
         எனில் all உள்ளீடு வரிகள் are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         செய் not check that the உள்ளீடு is correctly sorted\n\
"));
      oputs (_("\
      --header\n\
         treat the first வரி in each file as field headers,\n\
         print them without trying to pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         வரி delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
இல்லையெனில் fields are separated by CHAR.  Any FIELD is a field எண் counted\n\
from 1.  FORMAT is one or more comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join field,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
வரி of each file determines the எண் of fields output க்கு each வரி.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" எனில் 'join' has no options,\n\
or use \"join -t ''\" எனில் 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the உள்ளீடு is not sorted and some வரிகள் cannot be joined, a\n\
warning message will be given.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

நிலையான வெறுமை
extract_field (கட்டமைப்பு வரி *வரி, எழுத்து *field, idx_t len)
{
  எனில் (வரி->nfields >= வரி->nfields_allocated)
    வரி->fields = xpalloc (வரி->fields, &வரி->nfields_allocated, 1,
                            -1, அளவு *வரி->fields);
  idx_t மாறிலி field_index = வரி->nfields++;
  வரி->fields[field_index].beg = field;
  வரி->fields[field_index].len = len;
}

நிலையான bool
eq_tab (mcel_t g)
{
  திரும்பு mcel_eq (g, tab);
}

நிலையான bool
newline_or_blank (mcel_t g)
{
  திரும்பு g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

நிலையான வெறுமை
xfields (கட்டமைப்பு வரி *வரி)
{
  எழுத்து *ptr = வரி->buf.buffer;
  எழுத்து மாறிலி *lim = ptr + வரி->buf.length - 1;

  எனில் (ptr == lim)
    திரும்பு;

  எனில் (!tab.len)
    போது ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        எழுத்து *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (வரி, ptr, sep - ptr);
        ptr = sep;
      }
  இல்லையெனில்
    {
      எனில் (tab.ch != '\n')
        க்கு (எழுத்து *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (வரி, ptr, sep - ptr);

      extract_field (வரி, ptr, lim - ptr);
    }
}

நிலையான வெறுமை
freeline (கட்டமைப்பு வரி *வரி)
{
  எனில் (வரி == வெற்று)
    திரும்பு;
  விடுவி (வரி->fields);
  வரி->fields = வெற்று;
  வரி->nfields_allocated = 0;
  விடுவி (வரி->buf.buffer);
  வரி->buf.buffer = வெற்று;
  வரி->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

நிலையான முழு
keycmp (கட்டமைப்பு வரி மாறிலி *line1, கட்டமைப்பு வரி மாறிலி *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  எழுத்து *beg1;
  எழுத்து *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  முழு diff;

  எனில் (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  இல்லையெனில்
    {
      beg1 = வெற்று;
      len1 = 0;
    }

  எனில் (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  இல்லையெனில்
    {
      beg2 = வெற்று;
      len2 = 0;
    }

  எனில் (len1 == 0)
    திரும்பு len2 == 0 ? 0 : -1;
  எனில் (len2 == 0)
    திரும்பு 1;

  எனில் (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  இல்லையெனில்
    {
      எனில் (hard_LC_COLLATE)
        திரும்பு xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  எனில் (diff)
    திரும்பு diff;
  திரும்பு _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

நிலையான வெறுமை
check_order (மாறிலி கட்டமைப்பு வரி *prev,
             மாறிலி கட்டமைப்பு வரி *current,
             முழு whatfile)
{
  எனில் (check_உள்ளீடு_order != CHECK_ORDER_DISABLED
      && ((check_உள்ளீடு_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      எனில் (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          எனில் (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              எனில் (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (INT_MAX, len);

              error ((check_உள்ளீடு_order == CHECK_ORDER_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], வரி_no[whatfile - 1],
                     (முழு) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

நிலையான உள்வரி வெறுமை
reset_வரி (கட்டமைப்பு வரி *வரி)
{
  வரி->nfields = 0;
}

நிலையான கட்டமைப்பு வரி *
init_linep (கட்டமைப்பு வரி **linep)
{
  கட்டமைப்பு வரி *வரி = xzalloc (அளவு *வரி);
  *linep = வரி;
  திரும்பு வரி;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

நிலையான bool
get_வரி (FILE *fp, கட்டமைப்பு வரி **linep, முழு which)
{
  கட்டமைப்பு வரி *வரி = *linep;

  எனில் (வரி == prevline[which - 1])
    {
      SWAPLINES (வரி, spareline[which - 1]);
      *linep = வரி;
    }

  எனில் (வரி)
    reset_வரி (வரி);
  இல்லையெனில்
    வரி = init_linep (linep);

  எனில் (! readlinebuffer_delim (&வரி->buf, fp, eolchar))
    {
      எனில் (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (வரி);
      திரும்பு false;
    }
  ++வரி_no[which - 1];

  xfields (வரி);

  எனில் (prevline[which - 1])
    check_order (prevline[which - 1], வரி, which);

  prevline[which - 1] = வரி;
  திரும்பு true;
}

நிலையான வெறுமை
free_spareline (வெறுமை)
{
  க்கு (idx_t i = 0; i < countof (spareline); i++)
    {
      எனில் (spareline[i])
        {
          freeline (spareline[i]);
          விடுவி (spareline[i]);
        }
    }
}

நிலையான வெறுமை
initseq (கட்டமைப்பு seq *seq)
{
  seq->மொத்தம் = 0;
  seq->alloc = 0;
  seq->வரிகள் = வெற்று;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

நிலையான bool
getseq (FILE *fp, கட்டமைப்பு seq *seq, முழு whichfile)
{
  எனில் (seq->மொத்தம் == seq->alloc)
    {
      seq->வரிகள் = xpalloc (seq->வரிகள், &seq->alloc, 1, -1, அளவு *seq->வரிகள்);
      க்கு (idx_t i = seq->மொத்தம்; i < seq->alloc; i++)
        seq->வரிகள்[i] = வெற்று;
    }

  எனில் (get_வரி (fp, &seq->வரிகள்[seq->மொத்தம்], whichfile))
    {
      ++seq->மொத்தம்;
      திரும்பு true;
    }
  திரும்பு false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
நிலையான bool
advance_seq (FILE *fp, கட்டமைப்பு seq *seq, bool first, முழு whichfile)
{
  எனில் (first)
    seq->மொத்தம் = 0;

  திரும்பு getseq (fp, seq, whichfile);
}

நிலையான வெறுமை
delseq (கட்டமைப்பு seq *seq)
{
  க்கு (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->வரிகள்[i]);
      விடுவி (seq->வரிகள்[i]);
    }
  விடுவி (seq->வரிகள்);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

நிலையான வெறுமை
prfield (idx_t n, கட்டமைப்பு வரி மாறிலி *வரி)
{
  எனில் (n < வரி->nfields)
    {
      idx_t len = வரி->fields[n].len;
      எனில் (len)
        fwrite (வரி->fields[n].beg, 1, len, stdout);
      இல்லையெனில் எனில் (empty_filler)
        fputs (empty_filler, stdout);
    }
  இல்லையெனில் எனில் (empty_filler)
    fputs (empty_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

நிலையான வெறுமை
prfields (கட்டமைப்பு வரி மாறிலி *வரி, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : வரி->nfields;

  க்கு (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, வரி);
    }
  க்கு (idx_t i = join_field + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, வரி);
    }
}

/* Print the join of LINE1 and LINE2.  */

நிலையான வெறுமை
prjoin (கட்டமைப்பு வரி மாறிலி *line1, கட்டமைப்பு வரி மாறிலி *line2)
{
  மாறிலி கட்டமைப்பு outlist *outlist;
  idx_t field;
  கட்டமைப்பு வரி மாறிலி *வரி;

  outlist = outlist_head.next;
  எனில் (outlist)
    {
      மாறிலி கட்டமைப்பு outlist *o;

      o = outlist;
      போது (true)
        {
          எனில் (o->file == 0)
            {
              எனில் (line1 == &uni_blank)
                {
                  வரி = line2;
                  field = join_field_2;
                }
              இல்லையெனில்
                {
                  வரி = line1;
                  field = join_field_1;
                }
            }
          இல்லையெனில்
            {
              வரி = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, வரி);
          o = o->next;
          எனில் (o == வெற்று)
            நிறுத்து;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      putchar (eolchar);
    }
  இல்லையெனில்
    {
      எனில் (line1 == &uni_blank)
        {
          வரி = line2;
          field = join_field_2;
        }
      இல்லையெனில்
        {
          வரி = line1;
          field = join_field_1;
        }

      /* Output the join field.  */
      prfield (field, வரி);

      /* Output other fields.  */
      prfields (line1, join_field_1, autocount_1);
      prfields (line2, join_field_2, autocount_2);

      putchar (eolchar);
    }

  எனில் (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

நிலையான வெறுமை
join (FILE *fp1, FILE *fp2)
{
  கட்டமைப்பு seq seq1, seq2;
  முழு diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  எனில் (autoformat)
    {
      autocount_1 = seq1.மொத்தம் ? seq1.வரிகள்[0]->nfields : 0;
      autocount_2 = seq2.மொத்தம் ? seq2.வரிகள்[0]->nfields : 0;
    }

  எனில் (join_header_வரிகள் && (seq1.மொத்தம் || seq2.மொத்தம்))
    {
      கட்டமைப்பு வரி மாறிலி *hline1 = seq1.மொத்தம் ? seq1.வரிகள்[0] : &uni_blank;
      கட்டமைப்பு வரி மாறிலி *hline2 = seq2.மொத்தம் ? seq2.வரிகள்[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = வெற்று;
      prevline[1] = வெற்று;
      எனில் (seq1.மொத்தம்)
        advance_seq (fp1, &seq1, true, 1);
      எனில் (seq2.மொத்தம்)
        advance_seq (fp2, &seq2, true, 2);
    }

  போது (seq1.மொத்தம் && seq2.மொத்தம்)
    {
      diff = keycmp (seq1.வரிகள்[0], seq2.வரிகள்[0],
                     join_field_1, join_field_2);
      எனில் (diff < 0)
        {
          எனில் (print_unpairables_1)
            prjoin (seq1.வரிகள்[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          தொடர்;
        }
      எனில் (diff > 0)
        {
          எனில் (print_unpairables_2)
            prjoin (&uni_blank, seq2.வரிகள்[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          தொடர்;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      செய்
        எனில் (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.மொத்தம்;
            நிறுத்து;
          }
      போது (!keycmp (seq1.வரிகள்[seq1.மொத்தம் - 1], seq2.வரிகள்[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      செய்
        எனில் (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.மொத்தம்;
            நிறுத்து;
          }
      போது (!keycmp (seq1.வரிகள்[0], seq2.வரிகள்[seq2.மொத்தம் - 1],
                      join_field_1, join_field_2));

      எனில் (print_pairables)
        {
          க்கு (idx_t i = 0; i < seq1.மொத்தம் - 1; ++i)
            {
              க்கு (idx_t j = 0; j < seq2.மொத்தம் - 1; ++j)
                prjoin (seq1.வரிகள்[i], seq2.வரிகள்[j]);
            }
        }

      எனில் (!eof1)
        {
          SWAPLINES (seq1.வரிகள்[0], seq1.வரிகள்[seq1.மொத்தம் - 1]);
          seq1.மொத்தம் = 1;
        }
      இல்லையெனில்
        seq1.மொத்தம் = 0;

      எனில் (!eof2)
        {
          SWAPLINES (seq2.வரிகள்[0], seq2.வரிகள்[seq2.மொத்தம் - 1]);
          seq2.மொத்தம் = 1;
        }
      இல்லையெனில்
        seq2.மொத்தம் = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  கட்டமைப்பு வரி *வரி = வெற்று;
  bool checktail = false;

  எனில் (check_உள்ளீடு_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  எனில் ((print_unpairables_1 || checktail) && seq1.மொத்தம்)
    {
      எனில் (print_unpairables_1)
        prjoin (seq1.வரிகள்[0], &uni_blank);
      எனில் (seq2.மொத்தம்)
        seen_unpairable = true;
      போது (get_வரி (fp1, &வரி, 1))
        {
          எனில் (print_unpairables_1)
            prjoin (வரி, &uni_blank);
          எனில் (issued_disorder_warning[0] && !print_unpairables_1)
            நிறுத்து;
        }
    }

  எனில் ((print_unpairables_2 || checktail) && seq2.மொத்தம்)
    {
      எனில் (print_unpairables_2)
        prjoin (&uni_blank, seq2.வரிகள்[0]);
      எனில் (seq1.மொத்தம்)
        seen_unpairable = true;
      போது (get_வரி (fp2, &வரி, 2))
        {
          எனில் (print_unpairables_2)
            prjoin (&uni_blank, வரி);
          எனில் (issued_disorder_warning[1] && !print_unpairables_2)
            நிறுத்து;
        }
    }

  freeline (வரி);
  விடுவி (வரி);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

நிலையான வெறுமை
add_field (முழு file, idx_t field)
{
  கட்டமைப்பு outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || field == 0);

  o = xmalloc (அளவு *o);
  o->file = file;
  o->field = field;
  o->next = வெற்று;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_end->next = o;
  outlist_end = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

நிலையான idx_t
string_to_join_field (எழுத்து மாறிலி *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, வெற்று, 10, &val, "");
  எனில் (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  இல்லையெனில் எனில் (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  திரும்பு val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

நிலையான வெறுமை
decode_field_spec (எழுத்து மாறிலி *s, முழு *file_index, idx_t *field_index)
{
  /* The first character must be 0, 1, or 2.  */
  தேர்வு (s[0])
    {
    நிலை '0':
      எனில் (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      நிறுத்து;

    நிலை '1':
    நிலை '2':
      எனில் (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_index = s[0] - '0';
      *field_index = string_to_join_field (s + 2);
      நிறுத்து;

    இயல்பு:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

நிலையான bool
comma_or_blank (mcel_t g)
{
  திரும்பு g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

நிலையான வெறுமை
add_field_list (எழுத்து *str)
{
  எழுத்து *p = str;

  செய்
    {
      முழு file_index;
      idx_t field_index;
      எழுத்து மாறிலி *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      எனில் (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_index, &field_index);
      add_field (file_index, field_index);
    }
  போது (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

நிலையான வெறுமை
set_join_field (ptrdiff_t *var, idx_t val)
{
  எனில் (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

எண்ணிக்கை operand_status
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

நிலையான வெறுமை
add_file_name (எழுத்து *name, எழுத்து *names[2],
               முழு operand_status[2], முழு joption_மொத்தம்[2], முழு *nfiles,
               முழு *prev_optc_status, முழு *optc_status)
{
  முழு n = *nfiles;

  எனில் (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      எழுத்து *arg = names[op0];
      தேர்வு (operand_status[op0])
        {
        நிலை MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        நிலை MIGHT_BE_J1_ARG:
          joption_மொத்தம்[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          நிறுத்து;

        நிலை MIGHT_BE_J2_ARG:
          joption_மொத்தம்[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          நிறுத்து;

        நிலை MIGHT_BE_O_ARG:
          add_field_list (arg);
          நிறுத்து;
        }
      எனில் (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  எனில் (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு optc_status;
  முழு prev_optc_status = MUST_BE_OPERAND;
  முழு operand_status[2];
  முழு joption_மொத்தம்[2] = { 0, 0 };
  FILE *fp1, *fp2;
  முழு optc;
  முழு nfiles = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  போது ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, வெற்று))
         != -1)
    {
      optc_status = MUST_BE_OPERAND;

      தேர்வு (optc)
        {
        நிலை 'v':
            print_pairables = false;
            FALLTHROUGH;

        நிலை 'a':
          {
            long முழு val;
            எனில் (xstrtol (optarg, வெற்று, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            எனில் (val == 1)
              print_unpairables_1 = true;
            இல்லையெனில்
              print_unpairables_2 = true;
          }
          நிறுத்து;

        நிலை 'e':
          எனில் (empty_filler && ! streq (empty_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          empty_filler = optarg;
          நிறுத்து;

        நிலை 'i':
          ignore_case = true;
          நிறுத்து;

        நிலை '1':
          set_join_field (&join_field_1, string_to_join_field (optarg));
          நிறுத்து;

        நிலை '2':
          set_join_field (&join_field_2, string_to_join_field (optarg));
          நிறுத்து;

        நிலை 'j':
          எனில் ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_மொத்தம்[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          இல்லையெனில்
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          நிறுத்து;

        நிலை 'o':
          எனில் (streq (optarg, "auto"))
            autoformat = true;
          இல்லையெனில்
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          நிறுத்து;

        நிலை 't':
          {
            mcel_t newtab;
            எனில் (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            இல்லையெனில் எனில் (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            இல்லையெனில்
              {
                newtab = mcel_scanz (optarg);
                எனில் (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            எனில் (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          நிறுத்து;

        நிலை 'z':
          eolchar = 0;
          நிறுத்து;

        நிலை NOCHECK_ORDER_OPTION:
          check_உள்ளீடு_order = CHECK_ORDER_DISABLED;
          நிறுத்து;

        நிலை CHECK_ORDER_OPTION:
          check_உள்ளீடு_order = CHECK_ORDER_ENABLED;
          நிறுத்து;

        நிலை 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_மொத்தம்,
                         &nfiles, &prev_optc_status, &optc_status);
          நிறுத்து;

        நிலை HEADER_வரி_OPTION:
          join_header_வரிகள் = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }

      prev_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  prev_optc_status = MUST_BE_OPERAND;
  போது (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_மொத்தம்,
                   &nfiles, &prev_optc_status, &optc_status);

  எனில் (nfiles != 2)
    {
      எனில் (nfiles == 0)
        error (0, 0, _("missing operand"));
      இல்லையெனில்
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  க்கு (முழு i = 0; i < 2; i++)
    எனில் (joption_மொத்தம்[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  எனில் (join_field_1 < 0)
    join_field_1 = 0;
  எனில் (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? stdin : fopen (g_names[0], "r");
  எனில் (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? stdin : fopen (g_names[1], "r");
  எனில் (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));
  எனில் (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  எனில் (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  எனில் (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));

  எனில் (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  இல்லையெனில்
    திரும்பு EXIT_SUCCESS;
}
