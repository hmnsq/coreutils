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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <getopt.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "hard-locale.h"
#অন্তর্ভাব "linebuffer.h"
#অন্তর্ভাব "mcel.h"
#অন্তর্ভাব "memcasecmp.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "skipchars.h"
#অন্তর্ভাব "stdio--.h"
#অন্তর্ভাব "xmemcoll.h"
#অন্তর্ভাব "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "join"

#define AUTHORS proper_name ("Mike Haertel")

#define join system_join

#define SWAPLINES(a, b) কুরু { \
  সংরচনা সারি *tmp = a; \
  a = b; \
  b = tmp; \
} যাবত্ (0);

/* An element of the list identifying which fields to print for each
   output line.  */
সংরচনা outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    পূর্ণাঙ্ক file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t field;

    সংরচনা outlist *next;
  };

/* A field of a line.  */
সংরচনা field
  {
    বর্ণ *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
সংরচনা সারি
  {
    সংরচনা linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    সংরচনা field *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
সংরচনা seq
  {
    idx_t গুনতি;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    সংরচনা সারি **সারিগুলি COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
স্থির সংরচনা সারি *prevline[2] = {রিক্ত, রিক্ত};

/* The number of lines read from each file.  */
স্থির uintmax_t সারি_no[2] = {0, 0};

/* The input file names.  */
স্থির বর্ণ *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
স্থির সংরচনা সারি *spareline[2] = {রিক্ত, রিক্ত};

/* True if the LC_COLLATE locale is hard.  */
স্থির bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
স্থির bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
স্থির bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
স্থির bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
স্থির bool issued_disorder_warning[2];

/* Empty output field filler.  */
স্থির বর্ণ ধ্রুব *empty_filler;

/* Whether to ensure the same number of fields are output from each line.  */
স্থির bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
স্থির idx_t autocount_1;
স্থির idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
স্থির ptrdiff_t join_field_1 = -1;
স্থির ptrdiff_t join_field_2 = -1;

/* List of fields to print.  */
স্থির সংরচনা outlist outlist_head;

/* Last element in 'outlist', where a new element can be added.  */
স্থির সংরচনা outlist *outlist_end = &outlist_head;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
স্থির mcel_t tab;

/* The output separator to use, and its length in bytes.  */
স্থির বর্ণ ধ্রুব *output_separator = " ";
স্থির idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
স্থির গণনা
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_প্রবেশ_order;

গণনা
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_সারি_OPTION
};


স্থির সংরচনা option ধ্রুব longopts[] =
{
  {"ignore-case", no_argument, রিক্ত, 'i'},
  {"check-order", no_argument, রিক্ত, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, রিক্ত, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, রিক্ত, 'z'},
  {"header", no_argument, রিক্ত, HEADER_সারি_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Used to print non-joining lines */
স্থির সংরচনা সারি uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
স্থির bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
স্থির bool join_header_সারিগুলি;

/* The character marking end of line. Default to \n. */
স্থির বর্ণ eolchar = '\n';

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
For each pair of প্রবেশ সারিগুলি with identical join fields, লিখ a সারি to\n\
standard output.  The স্বভাব join field is the first, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, পঠ standard প্রবেশ.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable সারিগুলি from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding to FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (empty) প্রবেশ fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-পক্ষ\n\
         ignore differences in পক্ষ when comparing fields\n\
"));
      oputs (_("\
  -j FIELD\n\
         equivalent to '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT যাবত্ constructing output সারি\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as প্রবেশ and output field separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output সারিগুলি\n\
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
         check that the প্রবেশ is correctly sorted, even\n\
         যদি all প্রবেশ সারিগুলি are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         কুরু not check that the প্রবেশ is correctly sorted\n\
"));
      oputs (_("\
      --header\n\
         treat the first সারি in each file as field headers,\n\
         print them without trying to pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         সারি delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
অন্যথা fields are separated by CHAR.  Any FIELD is a field সংখ্যা counted\n\
from 1.  FORMAT is one or more comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join field,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
সারি of each file determines the সংখ্যা of fields output প্রতি each সারি.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" যদি 'join' has no options,\n\
or use \"join -t ''\" যদি 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the প্রবেশ is not sorted and some সারিগুলি cannot be joined, a\n\
warning message will be given.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

স্থির শূন্য
extract_field (সংরচনা সারি *সারি, বর্ণ *field, idx_t len)
{
  যদি (সারি->nfields >= সারি->nfields_allocated)
    সারি->fields = xpalloc (সারি->fields, &সারি->nfields_allocated, 1,
                            -1, পরিমাণ *সারি->fields);
  idx_t ধ্রুব field_index = সারি->nfields++;
  সারি->fields[field_index].beg = field;
  সারি->fields[field_index].len = len;
}

স্থির bool
eq_tab (mcel_t g)
{
  প্রত্যাগম mcel_eq (g, tab);
}

স্থির bool
newline_or_blank (mcel_t g)
{
  প্রত্যাগম g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

স্থির শূন্য
xfields (সংরচনা সারি *সারি)
{
  বর্ণ *ptr = সারি->buf.buffer;
  বর্ণ ধ্রুব *lim = ptr + সারি->buf.length - 1;

  যদি (ptr == lim)
    প্রত্যাগম;

  যদি (!tab.len)
    যাবত্ ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        বর্ণ *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (সারি, ptr, sep - ptr);
        ptr = sep;
      }
  অন্যথা
    {
      যদি (tab.ch != '\n')
        প্রতি (বর্ণ *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (সারি, ptr, sep - ptr);

      extract_field (সারি, ptr, lim - ptr);
    }
}

স্থির শূন্য
freeline (সংরচনা সারি *সারি)
{
  যদি (সারি == রিক্ত)
    প্রত্যাগম;
  মুক্ত (সারি->fields);
  সারি->fields = রিক্ত;
  সারি->nfields_allocated = 0;
  মুক্ত (সারি->buf.buffer);
  সারি->buf.buffer = রিক্ত;
  সারি->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

স্থির পূর্ণাঙ্ক
keycmp (সংরচনা সারি ধ্রুব *line1, সংরচনা সারি ধ্রুব *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  বর্ণ *beg1;
  বর্ণ *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  পূর্ণাঙ্ক diff;

  যদি (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  অন্যথা
    {
      beg1 = রিক্ত;
      len1 = 0;
    }

  যদি (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  অন্যথা
    {
      beg2 = রিক্ত;
      len2 = 0;
    }

  যদি (len1 == 0)
    প্রত্যাগম len2 == 0 ? 0 : -1;
  যদি (len2 == 0)
    প্রত্যাগম 1;

  যদি (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  অন্যথা
    {
      যদি (hard_LC_COLLATE)
        প্রত্যাগম xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  যদি (diff)
    প্রত্যাগম diff;
  প্রত্যাগম _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

স্থির শূন্য
check_order (ধ্রুব সংরচনা সারি *prev,
             ধ্রুব সংরচনা সারি *current,
             পূর্ণাঙ্ক whatfile)
{
  যদি (check_প্রবেশ_order != CHECK_ORDER_DISABLED
      && ((check_প্রবেশ_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      যদি (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          যদি (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              যদি (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (INT_MAX, len);

              error ((check_প্রবেশ_order == CHECK_ORDER_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], সারি_no[whatfile - 1],
                     (পূর্ণাঙ্ক) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

স্থির অন্তঃস্থ শূন্য
reset_সারি (সংরচনা সারি *সারি)
{
  সারি->nfields = 0;
}

স্থির সংরচনা সারি *
init_linep (সংরচনা সারি **linep)
{
  সংরচনা সারি *সারি = xzalloc (পরিমাণ *সারি);
  *linep = সারি;
  প্রত্যাগম সারি;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

স্থির bool
get_সারি (FILE *fp, সংরচনা সারি **linep, পূর্ণাঙ্ক which)
{
  সংরচনা সারি *সারি = *linep;

  যদি (সারি == prevline[which - 1])
    {
      SWAPLINES (সারি, spareline[which - 1]);
      *linep = সারি;
    }

  যদি (সারি)
    reset_সারি (সারি);
  অন্যথা
    সারি = init_linep (linep);

  যদি (! readlinebuffer_delim (&সারি->buf, fp, eolchar))
    {
      যদি (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (সারি);
      প্রত্যাগম false;
    }
  ++সারি_no[which - 1];

  xfields (সারি);

  যদি (prevline[which - 1])
    check_order (prevline[which - 1], সারি, which);

  prevline[which - 1] = সারি;
  প্রত্যাগম true;
}

স্থির শূন্য
free_spareline (শূন্য)
{
  প্রতি (idx_t i = 0; i < countof (spareline); i++)
    {
      যদি (spareline[i])
        {
          freeline (spareline[i]);
          মুক্ত (spareline[i]);
        }
    }
}

স্থির শূন্য
initseq (সংরচনা seq *seq)
{
  seq->গুনতি = 0;
  seq->alloc = 0;
  seq->সারিগুলি = রিক্ত;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

স্থির bool
getseq (FILE *fp, সংরচনা seq *seq, পূর্ণাঙ্ক whichfile)
{
  যদি (seq->গুনতি == seq->alloc)
    {
      seq->সারিগুলি = xpalloc (seq->সারিগুলি, &seq->alloc, 1, -1, পরিমাণ *seq->সারিগুলি);
      প্রতি (idx_t i = seq->গুনতি; i < seq->alloc; i++)
        seq->সারিগুলি[i] = রিক্ত;
    }

  যদি (get_সারি (fp, &seq->সারিগুলি[seq->গুনতি], whichfile))
    {
      ++seq->গুনতি;
      প্রত্যাগম true;
    }
  প্রত্যাগম false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
স্থির bool
advance_seq (FILE *fp, সংরচনা seq *seq, bool first, পূর্ণাঙ্ক whichfile)
{
  যদি (first)
    seq->গুনতি = 0;

  প্রত্যাগম getseq (fp, seq, whichfile);
}

স্থির শূন্য
delseq (সংরচনা seq *seq)
{
  প্রতি (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->সারিগুলি[i]);
      মুক্ত (seq->সারিগুলি[i]);
    }
  মুক্ত (seq->সারিগুলি);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

স্থির শূন্য
prfield (idx_t n, সংরচনা সারি ধ্রুব *সারি)
{
  যদি (n < সারি->nfields)
    {
      idx_t len = সারি->fields[n].len;
      যদি (len)
        fwrite (সারি->fields[n].beg, 1, len, stdout);
      অন্যথা যদি (empty_filler)
        fputs (empty_filler, stdout);
    }
  অন্যথা যদি (empty_filler)
    fputs (empty_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

স্থির শূন্য
prfields (সংরচনা সারি ধ্রুব *সারি, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : সারি->nfields;

  প্রতি (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, সারি);
    }
  প্রতি (idx_t i = join_field + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, সারি);
    }
}

/* Print the join of LINE1 and LINE2.  */

স্থির শূন্য
prjoin (সংরচনা সারি ধ্রুব *line1, সংরচনা সারি ধ্রুব *line2)
{
  ধ্রুব সংরচনা outlist *outlist;
  idx_t field;
  সংরচনা সারি ধ্রুব *সারি;

  outlist = outlist_head.next;
  যদি (outlist)
    {
      ধ্রুব সংরচনা outlist *o;

      o = outlist;
      যাবত্ (true)
        {
          যদি (o->file == 0)
            {
              যদি (line1 == &uni_blank)
                {
                  সারি = line2;
                  field = join_field_2;
                }
              অন্যথা
                {
                  সারি = line1;
                  field = join_field_1;
                }
            }
          অন্যথা
            {
              সারি = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, সারি);
          o = o->next;
          যদি (o == রিক্ত)
            ভঙ্গ;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      putchar (eolchar);
    }
  অন্যথা
    {
      যদি (line1 == &uni_blank)
        {
          সারি = line2;
          field = join_field_2;
        }
      অন্যথা
        {
          সারি = line1;
          field = join_field_1;
        }

      /* Output the join field.  */
      prfield (field, সারি);

      /* Output other fields.  */
      prfields (line1, join_field_1, autocount_1);
      prfields (line2, join_field_2, autocount_2);

      putchar (eolchar);
    }

  যদি (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

স্থির শূন্য
join (FILE *fp1, FILE *fp2)
{
  সংরচনা seq seq1, seq2;
  পূর্ণাঙ্ক diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  যদি (autoformat)
    {
      autocount_1 = seq1.গুনতি ? seq1.সারিগুলি[0]->nfields : 0;
      autocount_2 = seq2.গুনতি ? seq2.সারিগুলি[0]->nfields : 0;
    }

  যদি (join_header_সারিগুলি && (seq1.গুনতি || seq2.গুনতি))
    {
      সংরচনা সারি ধ্রুব *hline1 = seq1.গুনতি ? seq1.সারিগুলি[0] : &uni_blank;
      সংরচনা সারি ধ্রুব *hline2 = seq2.গুনতি ? seq2.সারিগুলি[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = রিক্ত;
      prevline[1] = রিক্ত;
      যদি (seq1.গুনতি)
        advance_seq (fp1, &seq1, true, 1);
      যদি (seq2.গুনতি)
        advance_seq (fp2, &seq2, true, 2);
    }

  যাবত্ (seq1.গুনতি && seq2.গুনতি)
    {
      diff = keycmp (seq1.সারিগুলি[0], seq2.সারিগুলি[0],
                     join_field_1, join_field_2);
      যদি (diff < 0)
        {
          যদি (print_unpairables_1)
            prjoin (seq1.সারিগুলি[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          অনুবর্ত;
        }
      যদি (diff > 0)
        {
          যদি (print_unpairables_2)
            prjoin (&uni_blank, seq2.সারিগুলি[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          অনুবর্ত;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      কুরু
        যদি (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.গুনতি;
            ভঙ্গ;
          }
      যাবত্ (!keycmp (seq1.সারিগুলি[seq1.গুনতি - 1], seq2.সারিগুলি[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      কুরু
        যদি (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.গুনতি;
            ভঙ্গ;
          }
      যাবত্ (!keycmp (seq1.সারিগুলি[0], seq2.সারিগুলি[seq2.গুনতি - 1],
                      join_field_1, join_field_2));

      যদি (print_pairables)
        {
          প্রতি (idx_t i = 0; i < seq1.গুনতি - 1; ++i)
            {
              প্রতি (idx_t j = 0; j < seq2.গুনতি - 1; ++j)
                prjoin (seq1.সারিগুলি[i], seq2.সারিগুলি[j]);
            }
        }

      যদি (!eof1)
        {
          SWAPLINES (seq1.সারিগুলি[0], seq1.সারিগুলি[seq1.গুনতি - 1]);
          seq1.গুনতি = 1;
        }
      অন্যথা
        seq1.গুনতি = 0;

      যদি (!eof2)
        {
          SWAPLINES (seq2.সারিগুলি[0], seq2.সারিগুলি[seq2.গুনতি - 1]);
          seq2.গুনতি = 1;
        }
      অন্যথা
        seq2.গুনতি = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  সংরচনা সারি *সারি = রিক্ত;
  bool checktail = false;

  যদি (check_প্রবেশ_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  যদি ((print_unpairables_1 || checktail) && seq1.গুনতি)
    {
      যদি (print_unpairables_1)
        prjoin (seq1.সারিগুলি[0], &uni_blank);
      যদি (seq2.গুনতি)
        seen_unpairable = true;
      যাবত্ (get_সারি (fp1, &সারি, 1))
        {
          যদি (print_unpairables_1)
            prjoin (সারি, &uni_blank);
          যদি (issued_disorder_warning[0] && !print_unpairables_1)
            ভঙ্গ;
        }
    }

  যদি ((print_unpairables_2 || checktail) && seq2.গুনতি)
    {
      যদি (print_unpairables_2)
        prjoin (&uni_blank, seq2.সারিগুলি[0]);
      যদি (seq1.গুনতি)
        seen_unpairable = true;
      যাবত্ (get_সারি (fp2, &সারি, 2))
        {
          যদি (print_unpairables_2)
            prjoin (&uni_blank, সারি);
          যদি (issued_disorder_warning[1] && !print_unpairables_2)
            ভঙ্গ;
        }
    }

  freeline (সারি);
  মুক্ত (সারি);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

স্থির শূন্য
add_field (পূর্ণাঙ্ক file, idx_t field)
{
  সংরচনা outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || field == 0);

  o = xmalloc (পরিমাণ *o);
  o->file = file;
  o->field = field;
  o->next = রিক্ত;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_end->next = o;
  outlist_end = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

স্থির idx_t
string_to_join_field (বর্ণ ধ্রুব *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, রিক্ত, 10, &val, "");
  যদি (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  অন্যথা যদি (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  প্রত্যাগম val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

স্থির শূন্য
decode_field_spec (বর্ণ ধ্রুব *s, পূর্ণাঙ্ক *file_index, idx_t *field_index)
{
  /* The first character must be 0, 1, or 2.  */
  বিকল্প (s[0])
    {
    পক্ষ '0':
      যদি (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      ভঙ্গ;

    পক্ষ '1':
    পক্ষ '2':
      যদি (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_index = s[0] - '0';
      *field_index = string_to_join_field (s + 2);
      ভঙ্গ;

    স্বভাব:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

স্থির bool
comma_or_blank (mcel_t g)
{
  প্রত্যাগম g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

স্থির শূন্য
add_field_list (বর্ণ *str)
{
  বর্ণ *p = str;

  কুরু
    {
      পূর্ণাঙ্ক file_index;
      idx_t field_index;
      বর্ণ ধ্রুব *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      যদি (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_index, &field_index);
      add_field (file_index, field_index);
    }
  যাবত্ (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

স্থির শূন্য
set_join_field (ptrdiff_t *var, idx_t val)
{
  যদি (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

গণনা operand_status
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

স্থির শূন্য
add_file_name (বর্ণ *name, বর্ণ *names[2],
               পূর্ণাঙ্ক operand_status[2], পূর্ণাঙ্ক joption_গুনতি[2], পূর্ণাঙ্ক *nfiles,
               পূর্ণাঙ্ক *prev_optc_status, পূর্ণাঙ্ক *optc_status)
{
  পূর্ণাঙ্ক n = *nfiles;

  যদি (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      বর্ণ *arg = names[op0];
      বিকল্প (operand_status[op0])
        {
        পক্ষ MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        পক্ষ MIGHT_BE_J1_ARG:
          joption_গুনতি[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          ভঙ্গ;

        পক্ষ MIGHT_BE_J2_ARG:
          joption_গুনতি[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          ভঙ্গ;

        পক্ষ MIGHT_BE_O_ARG:
          add_field_list (arg);
          ভঙ্গ;
        }
      যদি (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  যদি (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  পূর্ণাঙ্ক optc_status;
  পূর্ণাঙ্ক prev_optc_status = MUST_BE_OPERAND;
  পূর্ণাঙ্ক operand_status[2];
  পূর্ণাঙ্ক joption_গুনতি[2] = { 0, 0 };
  FILE *fp1, *fp2;
  পূর্ণাঙ্ক optc;
  পূর্ণাঙ্ক nfiles = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  যাবত্ ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, রিক্ত))
         != -1)
    {
      optc_status = MUST_BE_OPERAND;

      বিকল্প (optc)
        {
        পক্ষ 'v':
            print_pairables = false;
            FALLTHROUGH;

        পক্ষ 'a':
          {
            long পূর্ণাঙ্ক val;
            যদি (xstrtol (optarg, রিক্ত, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            যদি (val == 1)
              print_unpairables_1 = true;
            অন্যথা
              print_unpairables_2 = true;
          }
          ভঙ্গ;

        পক্ষ 'e':
          যদি (empty_filler && ! streq (empty_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          empty_filler = optarg;
          ভঙ্গ;

        পক্ষ 'i':
          ignore_case = true;
          ভঙ্গ;

        পক্ষ '1':
          set_join_field (&join_field_1, string_to_join_field (optarg));
          ভঙ্গ;

        পক্ষ '2':
          set_join_field (&join_field_2, string_to_join_field (optarg));
          ভঙ্গ;

        পক্ষ 'j':
          যদি ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_গুনতি[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          অন্যথা
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          ভঙ্গ;

        পক্ষ 'o':
          যদি (streq (optarg, "auto"))
            autoformat = true;
          অন্যথা
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          ভঙ্গ;

        পক্ষ 't':
          {
            mcel_t newtab;
            যদি (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            অন্যথা যদি (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            অন্যথা
              {
                newtab = mcel_scanz (optarg);
                যদি (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            যদি (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          ভঙ্গ;

        পক্ষ 'z':
          eolchar = 0;
          ভঙ্গ;

        পক্ষ NOCHECK_ORDER_OPTION:
          check_প্রবেশ_order = CHECK_ORDER_DISABLED;
          ভঙ্গ;

        পক্ষ CHECK_ORDER_OPTION:
          check_প্রবেশ_order = CHECK_ORDER_ENABLED;
          ভঙ্গ;

        পক্ষ 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_গুনতি,
                         &nfiles, &prev_optc_status, &optc_status);
          ভঙ্গ;

        পক্ষ HEADER_সারি_OPTION:
          join_header_সারিগুলি = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্বভাব:
          usage (EXIT_FAILURE);
        }

      prev_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  prev_optc_status = MUST_BE_OPERAND;
  যাবত্ (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_গুনতি,
                   &nfiles, &prev_optc_status, &optc_status);

  যদি (nfiles != 2)
    {
      যদি (nfiles == 0)
        error (0, 0, _("missing operand"));
      অন্যথা
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  প্রতি (পূর্ণাঙ্ক i = 0; i < 2; i++)
    যদি (joption_গুনতি[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  যদি (join_field_1 < 0)
    join_field_1 = 0;
  যদি (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? stdin : fopen (g_names[0], "r");
  যদি (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? stdin : fopen (g_names[1], "r");
  যদি (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));
  যদি (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  যদি (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  যদি (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));

  যদি (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  অন্যথা
    প্রত্যাগম EXIT_SUCCESS;
}
