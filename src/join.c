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

#شامل <config.h>

#شامل <sys/types.h>
#شامل <getopt.h>

#شامل "system.h"
#شامل "assure.h"
#شامل "fadvise.h"
#شامل "hard-locale.h"
#شامل "linebuffer.h"
#شامل "mcel.h"
#شامل "memcasecmp.h"
#شامل "quote.h"
#شامل "skipchars.h"
#شامل "stdio--.h"
#شامل "xmemcoll.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "join"

#define AUTHORS proper_name ("Mike Haertel")

#define join system_join

#define SWAPLINES(a, b) انجامبده { \
  ساختار سطر *tmp = a; \
  a = b; \
  b = tmp; \
} وقتی (0);

/* An element of the list identifying which fields to print for each
   output line.  */
ساختار outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    صحیح file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t field;

    ساختار outlist *next;
  };

/* A field of a line.  */
ساختار field
  {
    نویسه *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
ساختار سطر
  {
    ساختار linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    ساختار field *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
ساختار seq
  {
    idx_t شمار;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    ساختار سطر **سطرها COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
ایستا ساختار سطر *prevline[2] = {پوچ, پوچ};

/* The number of lines read from each file.  */
ایستا uintmax_t سطر_no[2] = {0, 0};

/* The input file names.  */
ایستا نویسه *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
ایستا ساختار سطر *spareline[2] = {پوچ, پوچ};

/* True if the LC_COLLATE locale is hard.  */
ایستا bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
ایستا bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
ایستا bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
ایستا bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ایستا bool issued_disorder_warning[2];

/* Empty output field filler.  */
ایستا نویسه ثابت *empty_filler;

/* Whether to ensure the same number of fields are output from each line.  */
ایستا bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
ایستا idx_t autocount_1;
ایستا idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
ایستا ptrdiff_t join_field_1 = -1;
ایستا ptrdiff_t join_field_2 = -1;

/* List of fields to print.  */
ایستا ساختار outlist outlist_head;

/* Last element in 'outlist', where a new element can be added.  */
ایستا ساختار outlist *outlist_end = &outlist_head;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
ایستا mcel_t tab;

/* The output separator to use, and its length in bytes.  */
ایستا نویسه ثابت *output_separator = " ";
ایستا idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
ایستا شمارش
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_درون_داد_order;

شمارش
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_سطر_OPTION
};


ایستا ساختار option ثابت longopts[] =
{
  {"ignore-case", no_argument, پوچ, 'i'},
  {"check-order", no_argument, پوچ, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, پوچ, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, پوچ, 'z'},
  {"header", no_argument, پوچ, HEADER_سطر_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Used to print non-joining lines */
ایستا ساختار سطر uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
ایستا bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
ایستا bool join_header_سطرها;

/* The character marking end of line. Default to \n. */
ایستا نویسه eolchar = '\n';

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
For each pair of درون_داد سطرها with identical join fields, اكتب a سطر to\n\
standard output.  The پیشفرض join field is the first, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, اقرأ standard درون_داد.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable سطرها from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding to FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (empty) درون_داد fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-حالت\n\
         ignore differences in حالت when comparing fields\n\
"));
      oputs (_("\
  -j FIELD\n\
         equivalent to '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT وقتی constructing output سطر\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as درون_داد and output field separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output سطرها\n\
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
         check that the درون_داد is correctly sorted, even\n\
         اگر all درون_داد سطرها are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         انجامبده not check that the درون_داد is correctly sorted\n\
"));
      oputs (_("\
      --header\n\
         treat the first سطر in each file as field headers,\n\
         print them without trying to pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
وگرنه fields are separated by CHAR.  Any FIELD is a field عدد counted\n\
from 1.  FORMAT is one or more comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join field,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
سطر of each file determines the عدد of fields output برای each سطر.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" اگر 'join' has no options,\n\
or use \"join -t ''\" اگر 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the درون_داد is not sorted and some سطرها cannot be joined, a\n\
warning message will be given.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

ایستا تهی
extract_field (ساختار سطر *سطر, نویسه *field, idx_t len)
{
  اگر (سطر->nfields >= سطر->nfields_allocated)
    سطر->fields = xpalloc (سطر->fields, &سطر->nfields_allocated, 1,
                            -1, اندازه *سطر->fields);
  idx_t ثابت field_index = سطر->nfields++;
  سطر->fields[field_index].beg = field;
  سطر->fields[field_index].len = len;
}

ایستا bool
eq_tab (mcel_t g)
{
  بازگشت mcel_eq (g, tab);
}

ایستا bool
newline_or_blank (mcel_t g)
{
  بازگشت g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

ایستا تهی
xfields (ساختار سطر *سطر)
{
  نویسه *ptr = سطر->buf.buffer;
  نویسه ثابت *lim = ptr + سطر->buf.length - 1;

  اگر (ptr == lim)
    بازگشت;

  اگر (!tab.len)
    وقتی ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        نویسه *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (سطر, ptr, sep - ptr);
        ptr = sep;
      }
  وگرنه
    {
      اگر (tab.ch != '\n')
        برای (نویسه *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (سطر, ptr, sep - ptr);

      extract_field (سطر, ptr, lim - ptr);
    }
}

ایستا تهی
freeline (ساختار سطر *سطر)
{
  اگر (سطر == پوچ)
    بازگشت;
  آزاد (سطر->fields);
  سطر->fields = پوچ;
  سطر->nfields_allocated = 0;
  آزاد (سطر->buf.buffer);
  سطر->buf.buffer = پوچ;
  سطر->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

ایستا صحیح
keycmp (ساختار سطر ثابت *line1, ساختار سطر ثابت *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  نویسه *beg1;
  نویسه *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  صحیح diff;

  اگر (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  وگرنه
    {
      beg1 = پوچ;
      len1 = 0;
    }

  اگر (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  وگرنه
    {
      beg2 = پوچ;
      len2 = 0;
    }

  اگر (len1 == 0)
    بازگشت len2 == 0 ? 0 : -1;
  اگر (len2 == 0)
    بازگشت 1;

  اگر (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  وگرنه
    {
      اگر (hard_LC_COLLATE)
        بازگشت xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  اگر (diff)
    بازگشت diff;
  بازگشت _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

ایستا تهی
check_order (ثابت ساختار سطر *prev,
             ثابت ساختار سطر *current,
             صحیح whatfile)
{
  اگر (check_درون_داد_order != CHECK_ORDER_DISABLED
      && ((check_درون_داد_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      اگر (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          اگر (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              اگر (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (INT_MAX, len);

              error ((check_درون_داد_order == CHECK_ORDER_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], سطر_no[whatfile - 1],
                     (صحیح) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

ایستا درونخطی تهی
reset_سطر (ساختار سطر *سطر)
{
  سطر->nfields = 0;
}

ایستا ساختار سطر *
init_linep (ساختار سطر **linep)
{
  ساختار سطر *سطر = xzalloc (اندازه *سطر);
  *linep = سطر;
  بازگشت سطر;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

ایستا bool
get_سطر (FILE *fp, ساختار سطر **linep, صحیح which)
{
  ساختار سطر *سطر = *linep;

  اگر (سطر == prevline[which - 1])
    {
      SWAPLINES (سطر, spareline[which - 1]);
      *linep = سطر;
    }

  اگر (سطر)
    reset_سطر (سطر);
  وگرنه
    سطر = init_linep (linep);

  اگر (! readlinebuffer_delim (&سطر->buf, fp, eolchar))
    {
      اگر (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (سطر);
      بازگشت false;
    }
  ++سطر_no[which - 1];

  xfields (سطر);

  اگر (prevline[which - 1])
    check_order (prevline[which - 1], سطر, which);

  prevline[which - 1] = سطر;
  بازگشت true;
}

ایستا تهی
free_spareline (تهی)
{
  برای (idx_t i = 0; i < countof (spareline); i++)
    {
      اگر (spareline[i])
        {
          freeline (spareline[i]);
          آزاد (spareline[i]);
        }
    }
}

ایستا تهی
initseq (ساختار seq *seq)
{
  seq->شمار = 0;
  seq->alloc = 0;
  seq->سطرها = پوچ;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

ایستا bool
getseq (FILE *fp, ساختار seq *seq, صحیح whichfile)
{
  اگر (seq->شمار == seq->alloc)
    {
      seq->سطرها = xpalloc (seq->سطرها, &seq->alloc, 1, -1, اندازه *seq->سطرها);
      برای (idx_t i = seq->شمار; i < seq->alloc; i++)
        seq->سطرها[i] = پوچ;
    }

  اگر (get_سطر (fp, &seq->سطرها[seq->شمار], whichfile))
    {
      ++seq->شمار;
      بازگشت true;
    }
  بازگشت false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
ایستا bool
advance_seq (FILE *fp, ساختار seq *seq, bool first, صحیح whichfile)
{
  اگر (first)
    seq->شمار = 0;

  بازگشت getseq (fp, seq, whichfile);
}

ایستا تهی
delseq (ساختار seq *seq)
{
  برای (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->سطرها[i]);
      آزاد (seq->سطرها[i]);
    }
  آزاد (seq->سطرها);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

ایستا تهی
prfield (idx_t n, ساختار سطر ثابت *سطر)
{
  اگر (n < سطر->nfields)
    {
      idx_t len = سطر->fields[n].len;
      اگر (len)
        fwrite (سطر->fields[n].beg, 1, len, stdout);
      وگرنه اگر (empty_filler)
        fputs (empty_filler, stdout);
    }
  وگرنه اگر (empty_filler)
    fputs (empty_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

ایستا تهی
prfields (ساختار سطر ثابت *سطر, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : سطر->nfields;

  برای (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, سطر);
    }
  برای (idx_t i = join_field + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, سطر);
    }
}

/* Print the join of LINE1 and LINE2.  */

ایستا تهی
prjoin (ساختار سطر ثابت *line1, ساختار سطر ثابت *line2)
{
  ثابت ساختار outlist *outlist;
  idx_t field;
  ساختار سطر ثابت *سطر;

  outlist = outlist_head.next;
  اگر (outlist)
    {
      ثابت ساختار outlist *o;

      o = outlist;
      وقتی (true)
        {
          اگر (o->file == 0)
            {
              اگر (line1 == &uni_blank)
                {
                  سطر = line2;
                  field = join_field_2;
                }
              وگرنه
                {
                  سطر = line1;
                  field = join_field_1;
                }
            }
          وگرنه
            {
              سطر = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, سطر);
          o = o->next;
          اگر (o == پوچ)
            بشکن;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      putchar (eolchar);
    }
  وگرنه
    {
      اگر (line1 == &uni_blank)
        {
          سطر = line2;
          field = join_field_2;
        }
      وگرنه
        {
          سطر = line1;
          field = join_field_1;
        }

      /* Output the join field.  */
      prfield (field, سطر);

      /* Output other fields.  */
      prfields (line1, join_field_1, autocount_1);
      prfields (line2, join_field_2, autocount_2);

      putchar (eolchar);
    }

  اگر (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

ایستا تهی
join (FILE *fp1, FILE *fp2)
{
  ساختار seq seq1, seq2;
  صحیح diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  اگر (autoformat)
    {
      autocount_1 = seq1.شمار ? seq1.سطرها[0]->nfields : 0;
      autocount_2 = seq2.شمار ? seq2.سطرها[0]->nfields : 0;
    }

  اگر (join_header_سطرها && (seq1.شمار || seq2.شمار))
    {
      ساختار سطر ثابت *hline1 = seq1.شمار ? seq1.سطرها[0] : &uni_blank;
      ساختار سطر ثابت *hline2 = seq2.شمار ? seq2.سطرها[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = پوچ;
      prevline[1] = پوچ;
      اگر (seq1.شمار)
        advance_seq (fp1, &seq1, true, 1);
      اگر (seq2.شمار)
        advance_seq (fp2, &seq2, true, 2);
    }

  وقتی (seq1.شمار && seq2.شمار)
    {
      diff = keycmp (seq1.سطرها[0], seq2.سطرها[0],
                     join_field_1, join_field_2);
      اگر (diff < 0)
        {
          اگر (print_unpairables_1)
            prjoin (seq1.سطرها[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          ادامه;
        }
      اگر (diff > 0)
        {
          اگر (print_unpairables_2)
            prjoin (&uni_blank, seq2.سطرها[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          ادامه;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      انجامبده
        اگر (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.شمار;
            بشکن;
          }
      وقتی (!keycmp (seq1.سطرها[seq1.شمار - 1], seq2.سطرها[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      انجامبده
        اگر (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.شمار;
            بشکن;
          }
      وقتی (!keycmp (seq1.سطرها[0], seq2.سطرها[seq2.شمار - 1],
                      join_field_1, join_field_2));

      اگر (print_pairables)
        {
          برای (idx_t i = 0; i < seq1.شمار - 1; ++i)
            {
              برای (idx_t j = 0; j < seq2.شمار - 1; ++j)
                prjoin (seq1.سطرها[i], seq2.سطرها[j]);
            }
        }

      اگر (!eof1)
        {
          SWAPLINES (seq1.سطرها[0], seq1.سطرها[seq1.شمار - 1]);
          seq1.شمار = 1;
        }
      وگرنه
        seq1.شمار = 0;

      اگر (!eof2)
        {
          SWAPLINES (seq2.سطرها[0], seq2.سطرها[seq2.شمار - 1]);
          seq2.شمار = 1;
        }
      وگرنه
        seq2.شمار = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  ساختار سطر *سطر = پوچ;
  bool checktail = false;

  اگر (check_درون_داد_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  اگر ((print_unpairables_1 || checktail) && seq1.شمار)
    {
      اگر (print_unpairables_1)
        prjoin (seq1.سطرها[0], &uni_blank);
      اگر (seq2.شمار)
        seen_unpairable = true;
      وقتی (get_سطر (fp1, &سطر, 1))
        {
          اگر (print_unpairables_1)
            prjoin (سطر, &uni_blank);
          اگر (issued_disorder_warning[0] && !print_unpairables_1)
            بشکن;
        }
    }

  اگر ((print_unpairables_2 || checktail) && seq2.شمار)
    {
      اگر (print_unpairables_2)
        prjoin (&uni_blank, seq2.سطرها[0]);
      اگر (seq1.شمار)
        seen_unpairable = true;
      وقتی (get_سطر (fp2, &سطر, 2))
        {
          اگر (print_unpairables_2)
            prjoin (&uni_blank, سطر);
          اگر (issued_disorder_warning[1] && !print_unpairables_2)
            بشکن;
        }
    }

  freeline (سطر);
  آزاد (سطر);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

ایستا تهی
add_field (صحیح file, idx_t field)
{
  ساختار outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || field == 0);

  o = xmalloc (اندازه *o);
  o->file = file;
  o->field = field;
  o->next = پوچ;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_end->next = o;
  outlist_end = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

ایستا idx_t
string_to_join_field (نویسه ثابت *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, پوچ, 10, &val, "");
  اگر (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  وگرنه اگر (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  بازگشت val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

ایستا تهی
decode_field_spec (نویسه ثابت *s, صحیح *file_index, idx_t *field_index)
{
  /* The first character must be 0, 1, or 2.  */
  انتخاب (s[0])
    {
    حالت '0':
      اگر (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      بشکن;

    حالت '1':
    حالت '2':
      اگر (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_index = s[0] - '0';
      *field_index = string_to_join_field (s + 2);
      بشکن;

    پیشفرض:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

ایستا bool
comma_or_blank (mcel_t g)
{
  بازگشت g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

ایستا تهی
add_field_list (نویسه *str)
{
  نویسه *p = str;

  انجامبده
    {
      صحیح file_index;
      idx_t field_index;
      نویسه ثابت *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      اگر (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_index, &field_index);
      add_field (file_index, field_index);
    }
  وقتی (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

ایستا تهی
set_join_field (ptrdiff_t *var, idx_t val)
{
  اگر (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

شمارش operand_status
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

ایستا تهی
add_file_name (نویسه *name, نویسه *names[2],
               صحیح operand_status[2], صحیح joption_شمار[2], صحیح *nfiles,
               صحیح *prev_optc_status, صحیح *optc_status)
{
  صحیح n = *nfiles;

  اگر (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      نویسه *arg = names[op0];
      انتخاب (operand_status[op0])
        {
        حالت MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        حالت MIGHT_BE_J1_ARG:
          joption_شمار[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          بشکن;

        حالت MIGHT_BE_J2_ARG:
          joption_شمار[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          بشکن;

        حالت MIGHT_BE_O_ARG:
          add_field_list (arg);
          بشکن;
        }
      اگر (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  اگر (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح optc_status;
  صحیح prev_optc_status = MUST_BE_OPERAND;
  صحیح operand_status[2];
  صحیح joption_شمار[2] = { 0, 0 };
  FILE *fp1, *fp2;
  صحیح optc;
  صحیح nfiles = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  وقتی ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, پوچ))
         != -1)
    {
      optc_status = MUST_BE_OPERAND;

      انتخاب (optc)
        {
        حالت 'v':
            print_pairables = false;
            FALLTHROUGH;

        حالت 'a':
          {
            long صحیح val;
            اگر (xstrtol (optarg, پوچ, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            اگر (val == 1)
              print_unpairables_1 = true;
            وگرنه
              print_unpairables_2 = true;
          }
          بشکن;

        حالت 'e':
          اگر (empty_filler && ! streq (empty_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          empty_filler = optarg;
          بشکن;

        حالت 'i':
          ignore_case = true;
          بشکن;

        حالت '1':
          set_join_field (&join_field_1, string_to_join_field (optarg));
          بشکن;

        حالت '2':
          set_join_field (&join_field_2, string_to_join_field (optarg));
          بشکن;

        حالت 'j':
          اگر ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_شمار[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          وگرنه
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          بشکن;

        حالت 'o':
          اگر (streq (optarg, "auto"))
            autoformat = true;
          وگرنه
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          بشکن;

        حالت 't':
          {
            mcel_t newtab;
            اگر (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            وگرنه اگر (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            وگرنه
              {
                newtab = mcel_scanz (optarg);
                اگر (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            اگر (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          بشکن;

        حالت 'z':
          eolchar = 0;
          بشکن;

        حالت NOCHECK_ORDER_OPTION:
          check_درون_داد_order = CHECK_ORDER_DISABLED;
          بشکن;

        حالت CHECK_ORDER_OPTION:
          check_درون_داد_order = CHECK_ORDER_ENABLED;
          بشکن;

        حالت 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_شمار,
                         &nfiles, &prev_optc_status, &optc_status);
          بشکن;

        حالت HEADER_سطر_OPTION:
          join_header_سطرها = true;
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }

      prev_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  prev_optc_status = MUST_BE_OPERAND;
  وقتی (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_شمار,
                   &nfiles, &prev_optc_status, &optc_status);

  اگر (nfiles != 2)
    {
      اگر (nfiles == 0)
        error (0, 0, _("missing operand"));
      وگرنه
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  برای (صحیح i = 0; i < 2; i++)
    اگر (joption_شمار[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  اگر (join_field_1 < 0)
    join_field_1 = 0;
  اگر (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? stdin : fopen (g_names[0], "r");
  اگر (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? stdin : fopen (g_names[1], "r");
  اگر (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));
  اگر (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  اگر (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  اگر (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));

  اگر (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  وگرنه
    بازگشت EXIT_SUCCESS;
}
