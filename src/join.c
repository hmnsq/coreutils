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
#تعریف_کرو پروگرام_NAME "join"

#تعریف_کرو AUTHORS proper_name ("Mike Haertel")

#تعریف_کرو join system_join

#تعریف_کرو SWAPLINES(a, b) کرو { \
  ساخت سطر *tmp = a; \
  a = b; \
  b = tmp; \
} جب (0);

/* An element of the list identifying which fields to print for each
   output line.  */
ساخت outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    صحیح file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t میدان;

    ساخت outlist *اگلا;
  };

/* A field of a line.  */
ساخت میدان
  {
    حرف *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
ساخت سطر
  {
    ساخت linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    ساخت میدان *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
ساخت seq
  {
    idx_t گنتی;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    ساخت سطر **سطریں COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
ساکن ساخت سطر *prevline[2] = {تہی, تہی};

/* The number of lines read from each file.  */
ساکن uintmax_t سطر_no[2] = {0, 0};

/* The input file names.  */
ساکن حرف *g_نام[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
ساکن ساخت سطر *spareline[2] = {تہی, تہی};

/* True if the LC_COLLATE locale is hard.  */
ساکن bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
ساکن bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
ساکن bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
ساکن bool نظر_آیا_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ساکن bool issued_disorder_warning[2];

/* Empty output field filler.  */
ساکن حرف ثابت *خالی_خانے_filler;

/* Whether to ensure the same number of fields are output from each line.  */
ساکن bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
ساکن idx_t autocount_1;
ساکن idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
ساکن ptrdiff_t join_میدان_1 = -1;
ساکن ptrdiff_t join_میدان_2 = -1;

/* List of fields to print.  */
ساکن ساخت outlist outlist_سر;

/* Last element in 'outlist', where a new element can be added.  */
ساکن ساخت outlist *outlist_انتہا = &outlist_سر;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
ساکن mcel_t tab;

/* The output separator to use, and its length in bytes.  */
ساکن حرف ثابت *output_separator = " ";
ساکن idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
ساکن شمار
  {
    CHECK_ترتیب_DEFAULT,
    CHECK_ترتیب_ENABLED,
    CHECK_ترتیب_DISABLED
  } check_داخلہ_ترتیب;

شمار
{
  CHECK_ترتیب_OPTION = CHAR_MAX + 1,
  NOCHECK_ترتیب_OPTION,
  سرخی_سطر_OPTION
};


ساکن ساخت option ثابت longopts[] =
{
  {"ignore-case", no_دلیل, تہی, 'i'},
  {"check-order", no_دلیل, تہی, CHECK_ترتیب_OPTION},
  {"nocheck-order", no_دلیل, تہی, NOCHECK_ترتیب_OPTION},
  {"zero-terminated", no_دلیل, تہی, 'z'},
  {"header", no_دلیل, تہی, سرخی_سطر_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Used to print non-joining lines */
ساکن ساخت سطر uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
ساکن bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
ساکن bool join_سرخی_سطریں;

/* The character marking end of line. Default to \n. */
ساکن حرف eolchar = '\n';

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              پروگرام_name);
      fputs (_("\
For each pair کا داخلہ سطریں with identical join fields, اكتب a سطر مقصد\n\
standard output.  The طےشدہ join میدان is the پہلا, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, اقرأ standard داخلہ.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable سطریں from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding مقصد FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (خالی_خانے) داخلہ fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-حالت\n\
         ignore differences اندر حالت when comparing fields\n\
"));
      oputs (_("\
  -j میدان_2\n\
         equivalent مقصد '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT جب constructing output سطر\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as داخلہ and output میدان separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output سطریں\n\
"));
      oputs (_("\
  -1 میدان_2\n\
         join on this میدان_2 کا file 1\n\
"));
      oputs (_("\
  -2 میدان_2\n\
         join on this میدان_2 کا file 2\n\
"));
      oputs (_("\
      --check-ترتیب\n\
         check that the داخلہ is correctly sorted, even\n\
         اگر all داخلہ سطریں are pairable\n\
"));
      oputs (_("\
      --nocheck-ترتیب\n\
         کرو not check that the داخلہ is correctly sorted\n\
"));
      oputs (_("\
      --سرخی\n\
         treat the پہلا سطر اندر each file as میدان سرخیاں,\n\
         print them without trying مقصد pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
ورنہ fields are separated by CHAR.  Any میدان_2 is a میدان نمبر counted\n\
from 1.  FORMAT is one or مزید comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join میدان,\n\
the بقایا fields from FILE1, the بقایا fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the پہلا\n\
سطر کا each file determines the نمبر کا fields output برائے each سطر.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"ترتیب_دو -k 1b,1\" اگر 'join' has no options,\n\
or use \"join -t ''\" اگر 'sort' has no options.\n\
Comparisons honor the قواعد specified by 'LC_COLLATE'.\n\
If the داخلہ is not sorted and some سطریں cannot be joined, a\n\
warning پیغام_2 will be given.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

ساکن خالی
نکالو_میدان (ساخت سطر *سطر, حرف *میدان, idx_t len)
{
  اگر (سطر->nfields >= سطر->nfields_allocated)
    سطر->fields = xpalloc (سطر->fields, &سطر->nfields_allocated, 1,
                            -1, حجم *سطر->fields);
  idx_t ثابت میدان_اشاریہ = سطر->nfields++;
  سطر->fields[میدان_اشاریہ].beg = میدان;
  سطر->fields[میدان_اشاریہ].len = len;
}

ساکن bool
eq_tab (mcel_t g)
{
  واپس mcel_eq (g, tab);
}

ساکن bool
newline_or_blank (mcel_t g)
{
  واپس g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

ساکن خالی
xfields (ساخت سطر *سطر)
{
  حرف *ptr = سطر->buf.بفر;
  حرف ثابت *lim = ptr + سطر->buf.لمبائی - 1;

  اگر (ptr == lim)
    واپس;

  اگر (!tab.len)
    جب ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        حرف *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        نکالو_میدان (سطر, ptr, sep - ptr);
        ptr = sep;
      }
  ورنہ
    {
      اگر (tab.ch != '\n')
        برائے (حرف *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_جائزہ (sep, lim).len)
          نکالو_میدان (سطر, ptr, sep - ptr);

      نکالو_میدان (سطر, ptr, lim - ptr);
    }
}

ساکن خالی
freeline (ساخت سطر *سطر)
{
  اگر (سطر == تہی)
    واپس;
  آزاد (سطر->fields);
  سطر->fields = تہی;
  سطر->nfields_allocated = 0;
  آزاد (سطر->buf.بفر);
  سطر->buf.بفر = تہی;
  سطر->buf.سائز = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

ساکن صحیح
keycmp (ساخت سطر ثابت *line1, ساخت سطر ثابت *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  حرف *beg1;
  حرف *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  صحیح diff;

  اگر (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  ورنہ
    {
      beg1 = تہی;
      len1 = 0;
    }

  اگر (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  ورنہ
    {
      beg2 = تہی;
      len2 = 0;
    }

  اگر (len1 == 0)
    واپس len2 == 0 ? 0 : -1;
  اگر (len2 == 0)
    واپس 1;

  اگر (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  ورنہ
    {
      اگر (hard_LC_COLLATE)
        واپس xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  اگر (diff)
    واپس diff;
  واپس _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

ساکن خالی
check_ترتیب (ثابت ساخت سطر *پچھلا,
             ثابت ساخت سطر *current,
             صحیح whatfile)
{
  اگر (check_داخلہ_ترتیب != CHECK_ترتیب_DISABLED
      && ((check_داخلہ_ترتیب == CHECK_ترتیب_ENABLED) || نظر_آیا_unpairable))
    {
      اگر (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_میدان = whatfile == 1 ? join_میدان_1 : join_میدان_2;
          اگر (keycmp (پچھلا, current, join_میدان, join_میدان) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.لمبائی;
              اگر (0 < len && current->buf.بفر[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (زیادہ_سے_زیادہ_عدد, len);

              error ((check_داخلہ_ترتیب == CHECK_ترتیب_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_نام[whatfile - 1], سطر_no[whatfile - 1],
                     (صحیح) len, current->buf.بفر);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

ساکن اندرونی خالی
reset_سطر (ساخت سطر *سطر)
{
  سطر->nfields = 0;
}

ساکن ساخت سطر *
init_linep (ساخت سطر **linep)
{
  ساخت سطر *سطر = xzalloc (حجم *سطر);
  *linep = سطر;
  واپس سطر;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

ساکن bool
get_سطر (FILE *fp, ساخت سطر **linep, صحیح which)
{
  ساخت سطر *سطر = *linep;

  اگر (سطر == prevline[which - 1])
    {
      SWAPLINES (سطر, spareline[which - 1]);
      *linep = سطر;
    }

  اگر (سطر)
    reset_سطر (سطر);
  ورنہ
    سطر = init_linep (linep);

  اگر (! readlinebuffer_delim (&سطر->buf, fp, eolchar))
    {
      اگر (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (سطر);
      واپس false;
    }
  ++سطر_no[which - 1];

  xfields (سطر);

  اگر (prevline[which - 1])
    check_ترتیب (prevline[which - 1], سطر, which);

  prevline[which - 1] = سطر;
  واپس true;
}

ساکن خالی
free_spareline (خالی)
{
  برائے (idx_t i = 0; i < countof (spareline); i++)
    {
      اگر (spareline[i])
        {
          freeline (spareline[i]);
          آزاد (spareline[i]);
        }
    }
}

ساکن خالی
initseq (ساخت seq *seq)
{
  seq->گنتی = 0;
  seq->alloc = 0;
  seq->سطریں = تہی;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

ساکن bool
getseq (FILE *fp, ساخت seq *seq, صحیح whichfile)
{
  اگر (seq->گنتی == seq->alloc)
    {
      seq->سطریں = xpalloc (seq->سطریں, &seq->alloc, 1, -1, حجم *seq->سطریں);
      برائے (idx_t i = seq->گنتی; i < seq->alloc; i++)
        seq->سطریں[i] = تہی;
    }

  اگر (get_سطر (fp, &seq->سطریں[seq->گنتی], whichfile))
    {
      ++seq->گنتی;
      واپس true;
    }
  واپس false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
ساکن bool
advance_seq (FILE *fp, ساخت seq *seq, bool پہلا, صحیح whichfile)
{
  اگر (پہلا)
    seq->گنتی = 0;

  واپس getseq (fp, seq, whichfile);
}

ساکن خالی
delseq (ساخت seq *seq)
{
  برائے (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->سطریں[i]);
      آزاد (seq->سطریں[i]);
    }
  آزاد (seq->سطریں);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

ساکن خالی
prfield (idx_t n, ساخت سطر ثابت *سطر)
{
  اگر (n < سطر->nfields)
    {
      idx_t len = سطر->fields[n].len;
      اگر (len)
        fwrite (سطر->fields[n].beg, 1, len, stdout);
      ورنہ اگر (خالی_خانے_filler)
        fputs (خالی_خانے_filler, stdout);
    }
  ورنہ اگر (خالی_خانے_filler)
    fputs (خالی_خانے_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

ساکن خالی
prfields (ساخت سطر ثابت *سطر, idx_t join_میدان, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : سطر->nfields;

  برائے (idx_t i = 0; i < join_میدان && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, سطر);
    }
  برائے (idx_t i = join_میدان + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, سطر);
    }
}

/* Print the join of LINE1 and LINE2.  */

ساکن خالی
prjoin (ساخت سطر ثابت *line1, ساخت سطر ثابت *line2)
{
  ثابت ساخت outlist *outlist;
  idx_t میدان;
  ساخت سطر ثابت *سطر;

  outlist = outlist_سر.اگلا;
  اگر (outlist)
    {
      ثابت ساخت outlist *o;

      o = outlist;
      جب (true)
        {
          اگر (o->file == 0)
            {
              اگر (line1 == &uni_blank)
                {
                  سطر = line2;
                  میدان = join_میدان_2;
                }
              ورنہ
                {
                  سطر = line1;
                  میدان = join_میدان_1;
                }
            }
          ورنہ
            {
              سطر = (o->file == 1 ? line1 : line2);
              میدان = o->میدان;
            }
          prfield (میدان, سطر);
          o = o->اگلا;
          اگر (o == تہی)
            توڑو;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      حرف_لکھو (eolchar);
    }
  ورنہ
    {
      اگر (line1 == &uni_blank)
        {
          سطر = line2;
          میدان = join_میدان_2;
        }
      ورنہ
        {
          سطر = line1;
          میدان = join_میدان_1;
        }

      /* Output the join field.  */
      prfield (میدان, سطر);

      /* Output other fields.  */
      prfields (line1, join_میدان_1, autocount_1);
      prfields (line2, join_میدان_2, autocount_2);

      حرف_لکھو (eolchar);
    }

  اگر (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

ساکن خالی
join (FILE *fp1, FILE *fp2)
{
  ساخت seq seq1, seq2;
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
      autocount_1 = seq1.گنتی ? seq1.سطریں[0]->nfields : 0;
      autocount_2 = seq2.گنتی ? seq2.سطریں[0]->nfields : 0;
    }

  اگر (join_سرخی_سطریں && (seq1.گنتی || seq2.گنتی))
    {
      ساخت سطر ثابت *hline1 = seq1.گنتی ? seq1.سطریں[0] : &uni_blank;
      ساخت سطر ثابت *hline2 = seq2.گنتی ? seq2.سطریں[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = تہی;
      prevline[1] = تہی;
      اگر (seq1.گنتی)
        advance_seq (fp1, &seq1, true, 1);
      اگر (seq2.گنتی)
        advance_seq (fp2, &seq2, true, 2);
    }

  جب (seq1.گنتی && seq2.گنتی)
    {
      diff = keycmp (seq1.سطریں[0], seq2.سطریں[0],
                     join_میدان_1, join_میدان_2);
      اگر (diff < 0)
        {
          اگر (print_unpairables_1)
            prjoin (seq1.سطریں[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          نظر_آیا_unpairable = true;
          جاری;
        }
      اگر (diff > 0)
        {
          اگر (print_unpairables_2)
            prjoin (&uni_blank, seq2.سطریں[0]);
          advance_seq (fp2, &seq2, true, 2);
          نظر_آیا_unpairable = true;
          جاری;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      کرو
        اگر (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.گنتی;
            توڑو;
          }
      جب (!keycmp (seq1.سطریں[seq1.گنتی - 1], seq2.سطریں[0],
                      join_میدان_1, join_میدان_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      کرو
        اگر (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.گنتی;
            توڑو;
          }
      جب (!keycmp (seq1.سطریں[0], seq2.سطریں[seq2.گنتی - 1],
                      join_میدان_1, join_میدان_2));

      اگر (print_pairables)
        {
          برائے (idx_t i = 0; i < seq1.گنتی - 1; ++i)
            {
              برائے (idx_t j = 0; j < seq2.گنتی - 1; ++j)
                prjoin (seq1.سطریں[i], seq2.سطریں[j]);
            }
        }

      اگر (!eof1)
        {
          SWAPLINES (seq1.سطریں[0], seq1.سطریں[seq1.گنتی - 1]);
          seq1.گنتی = 1;
        }
      ورنہ
        seq1.گنتی = 0;

      اگر (!eof2)
        {
          SWAPLINES (seq2.سطریں[0], seq2.سطریں[seq2.گنتی - 1]);
          seq2.گنتی = 1;
        }
      ورنہ
        seq2.گنتی = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  ساخت سطر *سطر = تہی;
  bool checktail = false;

  اگر (check_داخلہ_ترتیب != CHECK_ترتیب_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  اگر ((print_unpairables_1 || checktail) && seq1.گنتی)
    {
      اگر (print_unpairables_1)
        prjoin (seq1.سطریں[0], &uni_blank);
      اگر (seq2.گنتی)
        نظر_آیا_unpairable = true;
      جب (get_سطر (fp1, &سطر, 1))
        {
          اگر (print_unpairables_1)
            prjoin (سطر, &uni_blank);
          اگر (issued_disorder_warning[0] && !print_unpairables_1)
            توڑو;
        }
    }

  اگر ((print_unpairables_2 || checktail) && seq2.گنتی)
    {
      اگر (print_unpairables_2)
        prjoin (&uni_blank, seq2.سطریں[0]);
      اگر (seq1.گنتی)
        نظر_آیا_unpairable = true;
      جب (get_سطر (fp2, &سطر, 2))
        {
          اگر (print_unpairables_2)
            prjoin (&uni_blank, سطر);
          اگر (issued_disorder_warning[1] && !print_unpairables_2)
            توڑو;
        }
    }

  freeline (سطر);
  آزاد (سطر);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

ساکن خالی
جوڑو_میدان (صحیح file, idx_t میدان)
{
  ساخت outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || میدان == 0);

  o = xmalloc (حجم *o);
  o->file = file;
  o->میدان = میدان;
  o->اگلا = تہی;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_انتہا->اگلا = o;
  outlist_انتہا = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

ساکن idx_t
string_مقصد_join_میدان (حرف ثابت *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, تہی, 10, &val, "");
  اگر (s_err == LONGINT_حد_سے_تجاوز || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  ورنہ اگر (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  واپس val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

ساکن خالی
decode_میدان_spec (حرف ثابت *s, صحیح *file_اشاریہ, idx_t *میدان_اشاریہ)
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
      *file_اشاریہ = 0;
      *میدان_اشاریہ = 0;
      توڑو;

    حالت '1':
    حالت '2':
      اگر (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_اشاریہ = s[0] - '0';
      *میدان_اشاریہ = string_مقصد_join_میدان (s + 2);
      توڑو;

    طےشدہ:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

ساکن bool
comma_or_blank (mcel_t g)
{
  واپس g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

ساکن خالی
جوڑو_میدان_list (حرف *str)
{
  حرف *p = str;

  کرو
    {
      صحیح file_اشاریہ;
      idx_t میدان_اشاریہ;
      حرف ثابت *spec_شے = p;
      p = skip_str_matching (spec_شے, comma_or_blank, false);
      اگر (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_میدان_spec (spec_شے, &file_اشاریہ, &میدان_اشاریہ);
      جوڑو_میدان (file_اشاریہ, میدان_اشاریہ);
    }
  جب (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

ساکن خالی
سیٹ_join_میدان (ptrdiff_t *var, idx_t val)
{
  اگر (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

شمار عملیہ_حالت
  {
    /* This argument must be an operand, i.e., one of the files to be
       joined.  */
    MUST_BE_عملیہ,

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

ساکن خالی
جوڑو_file_name (حرف *name, حرف *نام[2],
               صحیح عملیہ_حالت[2], صحیح joption_گنتی[2], صحیح *nfiles,
               صحیح *پچھلا_optc_حالت, صحیح *optc_حالت)
{
  صحیح n = *nfiles;

  اگر (n == 2)
    {
      bool op0 = (عملیہ_حالت[0] == MUST_BE_عملیہ);
      حرف *arg = نام[op0];
      انتخاب (عملیہ_حالت[op0])
        {
        حالت MUST_BE_عملیہ:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        حالت MIGHT_BE_J1_ARG:
          joption_گنتی[0]--;
          سیٹ_join_میدان (&join_میدان_1, string_مقصد_join_میدان (arg));
          توڑو;

        حالت MIGHT_BE_J2_ARG:
          joption_گنتی[1]--;
          سیٹ_join_میدان (&join_میدان_2, string_مقصد_join_میدان (arg));
          توڑو;

        حالت MIGHT_BE_O_ARG:
          جوڑو_میدان_list (arg);
          توڑو;
        }
      اگر (!op0)
        {
          عملیہ_حالت[0] = عملیہ_حالت[1];
          نام[0] = نام[1];
        }
      n = 1;
    }

  عملیہ_حالت[n] = *پچھلا_optc_حالت;
  نام[n] = name;
  *nfiles = n + 1;
  اگر (*پچھلا_optc_حالت == MIGHT_BE_O_ARG)
    *optc_حالت = MIGHT_BE_O_ARG;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optc_حالت;
  صحیح پچھلا_optc_حالت = MUST_BE_عملیہ;
  صحیح عملیہ_حالت[2];
  صحیح joption_گنتی[2] = { 0, 0 };
  FILE *fp1, *fp2;
  صحیح optc;
  صحیح nfiles = 0;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  جب ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, تہی))
         != -1)
    {
      optc_حالت = MUST_BE_عملیہ;

      انتخاب (optc)
        {
        حالت 'v':
            print_pairables = false;
            FALLTHROUGH;

        حالت 'a':
          {
            طویل صحیح val;
            اگر (xstrtol (optarg, تہی, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            اگر (val == 1)
              print_unpairables_1 = true;
            ورنہ
              print_unpairables_2 = true;
          }
          توڑو;

        حالت 'e':
          اگر (خالی_خانے_filler && ! streq (خالی_خانے_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          خالی_خانے_filler = optarg;
          توڑو;

        حالت 'i':
          ignore_case = true;
          توڑو;

        حالت '1':
          سیٹ_join_میدان (&join_میدان_1, string_مقصد_join_میدان (optarg));
          توڑو;

        حالت '2':
          سیٹ_join_میدان (&join_میدان_2, string_مقصد_join_میدان (optarg));
          توڑو;

        حالت 'j':
          اگر ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_گنتی[is_j2]++;
              optc_حالت = MIGHT_BE_J1_ARG + is_j2;
            }
          ورنہ
            {
              سیٹ_join_میدان (&join_میدان_1, string_مقصد_join_میدان (optarg));
              سیٹ_join_میدان (&join_میدان_2, join_میدان_1);
            }
          توڑو;

        حالت 'o':
          اگر (streq (optarg, "auto"))
            autoformat = true;
          ورنہ
            {
              جوڑو_میدان_list (optarg);
              optc_حالت = MIGHT_BE_O_ARG;
            }
          توڑو;

        حالت 't':
          {
            mcel_t newtab;
            اگر (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            ورنہ اگر (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            ورنہ
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
          توڑو;

        حالت 'z':
          eolchar = 0;
          توڑو;

        حالت NOCHECK_ترتیب_OPTION:
          check_داخلہ_ترتیب = CHECK_ترتیب_DISABLED;
          توڑو;

        حالت CHECK_ترتیب_OPTION:
          check_داخلہ_ترتیب = CHECK_ترتیب_ENABLED;
          توڑو;

        حالت 1:		/* Non-option argument.  */
          جوڑو_file_name (optarg, g_نام, عملیہ_حالت, joption_گنتی,
                         &nfiles, &پچھلا_optc_حالت, &optc_حالت);
          توڑو;

        حالت سرخی_سطر_OPTION:
          join_سرخی_سطریں = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }

      پچھلا_optc_حالت = optc_حالت;
    }

  /* Process any operands after "--".  */
  پچھلا_optc_حالت = MUST_BE_عملیہ;
  جب (optind < argc)
    جوڑو_file_name (argv[optind++], g_نام, عملیہ_حالت, joption_گنتی,
                   &nfiles, &پچھلا_optc_حالت, &optc_حالت);

  اگر (nfiles != 2)
    {
      اگر (nfiles == 0)
        error (0, 0, _("missing operand"));
      ورنہ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  برائے (صحیح i = 0; i < 2; i++)
    اگر (joption_گنتی[i] != 0)
      {
        سیٹ_join_میدان (&join_میدان_1, i);
        سیٹ_join_میدان (&join_میدان_2, i);
      }

  اگر (join_میدان_1 < 0)
    join_میدان_1 = 0;
  اگر (join_میدان_2 < 0)
    join_میدان_2 = 0;

  fp1 = streq (g_نام[0], "-") ? stdin : fopen (g_نام[0], "r");
  اگر (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نام[0]));
  fp2 = streq (g_نام[1], "-") ? stdin : fopen (g_نام[1], "r");
  اگر (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نام[1]));
  اگر (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  اگر (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نام[0]));
  اگر (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نام[1]));

  اگر (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  ورنہ
    واپس EXIT_SUCCESS;
}
