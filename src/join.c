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
#تعریف_کن برنامه_NAME "join"

#تعریف_کن AUTHORS proper_name ("Mike Haertel")

#تعریف_کن join system_join

#تعریف_کن SWAPLINES(a, b) افعل { \
  بنية سطر *tmp = a; \
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
    idx_t فیلد;

    بنية outlist *بعدی;
  };

/* A field of a line.  */
بنية فیلد
  {
    حرف *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
بنية سطر
  {
    بنية linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    بنية فیلد *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
بنية seq
  {
    idx_t شمار;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    بنية سطر **سطرها COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
ساكن بنية سطر *prevline[2] = {عدم, عدم};

/* The number of lines read from each file.  */
ساكن uintmax_t سطر_no[2] = {0, 0};

/* The input file names.  */
ساكن حرف *g_نامها[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
ساكن بنية سطر *spareline[2] = {عدم, عدم};

/* True if the LC_COLLATE locale is hard.  */
ساكن bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
ساكن bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
ساكن bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
ساكن bool دیده_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ساكن bool issued_disorder_warning[2];

/* Empty output field filler.  */
ساكن حرف ثابت *خالی_filler;

/* Whether to ensure the same number of fields are output from each line.  */
ساكن bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
ساكن idx_t autocount_1;
ساكن idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
ساكن ptrdiff_t join_فیلد_1 = -1;
ساكن ptrdiff_t join_فیلد_2 = -1;

/* List of fields to print.  */
ساكن بنية outlist outlist_سر;

/* Last element in 'outlist', where a new element can be added.  */
ساكن بنية outlist *outlist_انتها = &outlist_سر;

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
    CHECK_ترتیب_DEFAULT,
    CHECK_ترتیب_ENABLED,
    CHECK_ترتیب_DISABLED
  } check_درون_داد_ترتیب;

تعداد
{
  CHECK_ترتیب_OPTION = CHAR_MAX + 1,
  NOCHECK_ترتیب_OPTION,
  سرآیند_سطر_OPTION
};


ساكن بنية option ثابت longopts[] =
{
  {"ignore-case", no_آرگومان, عدم, 'i'},
  {"check-order", no_آرگومان, عدم, CHECK_ترتیب_OPTION},
  {"nocheck-order", no_آرگومان, عدم, NOCHECK_ترتیب_OPTION},
  {"zero-terminated", no_آرگومان, عدم, 'z'},
  {"header", no_آرگومان, عدم, سرآیند_سطر_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Used to print non-joining lines */
ساكن بنية سطر uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
ساكن bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
ساكن bool join_سرآیند_سطرها;

/* The character marking end of line. Default to \n. */
ساكن حرف eolchar = '\n';

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              برنامه_name);
      fputs (_("\
For each pair از درون_داد سطرها with identical join fields, اكتب a سطر مقصد\n\
standard output.  The افتراضي join فیلد is the اولی, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, اقرأ standard درون_داد.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable سطرها from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding مقصد FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (خالی) درون_داد fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-حالة\n\
         ignore differences درون حالة when comparing fields\n\
"));
      oputs (_("\
  -j فیلد_2\n\
         equivalent مقصد '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT طالما constructing output سطر\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as درون_داد and output فیلد separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output سطرها\n\
"));
      oputs (_("\
  -1 فیلد_2\n\
         join on this فیلد_2 از file 1\n\
"));
      oputs (_("\
  -2 فیلد_2\n\
         join on this فیلد_2 از file 2\n\
"));
      oputs (_("\
      --check-ترتیب\n\
         check that the درون_داد is correctly sorted, even\n\
         إذا all درون_داد سطرها are pairable\n\
"));
      oputs (_("\
      --nocheck-ترتیب\n\
         افعل not check that the درون_داد is correctly sorted\n\
"));
      oputs (_("\
      --سرآیند\n\
         treat the اولی سطر درون each file as فیلد سرآیندها,\n\
         print them without trying مقصد pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
وإلا fields are separated by CHAR.  Any فیلد_2 is a فیلد عدد counted\n\
from 1.  FORMAT is one or بیشتر comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join فیلد,\n\
the مانده fields from FILE1, the مانده fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the اولی\n\
سطر از each file determines the عدد از fields output لكل each سطر.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"مرتب_سازی -k 1b,1\" إذا 'join' has no options,\n\
or use \"join -t ''\" إذا 'sort' has no options.\n\
Comparisons honor the قواعد specified by 'LC_COLLATE'.\n\
If the درون_داد is not sorted and some سطرها cannot be joined, a\n\
warning پیام_2 will be given.\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

ساكن فراغ
استخراج_فیلد (بنية سطر *سطر, حرف *فیلد, idx_t len)
{
  إذا (سطر->nfields >= سطر->nfields_allocated)
    سطر->fields = xpalloc (سطر->fields, &سطر->nfields_allocated, 1,
                            -1, حجم *سطر->fields);
  idx_t ثابت فیلد_شاخص = سطر->nfields++;
  سطر->fields[فیلد_شاخص].beg = فیلد;
  سطر->fields[فیلد_شاخص].len = len;
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
xfields (بنية سطر *سطر)
{
  حرف *ptr = سطر->buf.میانگیر;
  حرف ثابت *lim = ptr + سطر->buf.درازی - 1;

  إذا (ptr == lim)
    أرجع;

  إذا (!tab.len)
    طالما ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        حرف *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        استخراج_فیلد (سطر, ptr, sep - ptr);
        ptr = sep;
      }
  وإلا
    {
      إذا (tab.ch != '\n')
        لكل (حرف *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_پیمایش (sep, lim).len)
          استخراج_فیلد (سطر, ptr, sep - ptr);

      استخراج_فیلد (سطر, ptr, lim - ptr);
    }
}

ساكن فراغ
freeline (بنية سطر *سطر)
{
  إذا (سطر == عدم)
    أرجع;
  حرر (سطر->fields);
  سطر->fields = عدم;
  سطر->nfields_allocated = 0;
  حرر (سطر->buf.میانگیر);
  سطر->buf.میانگیر = عدم;
  سطر->buf.اندازه = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

ساكن صحيح
keycmp (بنية سطر ثابت *line1, بنية سطر ثابت *line2,
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
check_ترتیب (ثابت بنية سطر *prev,
             ثابت بنية سطر *current,
             صحيح whatfile)
{
  إذا (check_درون_داد_ترتیب != CHECK_ترتیب_DISABLED
      && ((check_درون_داد_ترتیب == CHECK_ترتیب_ENABLED) || دیده_unpairable))
    {
      إذا (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_فیلد = whatfile == 1 ? join_فیلد_1 : join_فیلد_2;
          إذا (keycmp (prev, current, join_فیلد, join_فیلد) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.درازی;
              إذا (0 < len && current->buf.میانگیر[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (بیشترین_صحیح, len);

              error ((check_درون_داد_ترتیب == CHECK_ترتیب_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_نامها[whatfile - 1], سطر_no[whatfile - 1],
                     (صحيح) len, current->buf.میانگیر);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

ساكن مضمن فراغ
reset_سطر (بنية سطر *سطر)
{
  سطر->nfields = 0;
}

ساكن بنية سطر *
init_linep (بنية سطر **linep)
{
  بنية سطر *سطر = xzalloc (حجم *سطر);
  *linep = سطر;
  أرجع سطر;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

ساكن bool
get_سطر (FILE *fp, بنية سطر **linep, صحيح which)
{
  بنية سطر *سطر = *linep;

  إذا (سطر == prevline[which - 1])
    {
      SWAPLINES (سطر, spareline[which - 1]);
      *linep = سطر;
    }

  إذا (سطر)
    reset_سطر (سطر);
  وإلا
    سطر = init_linep (linep);

  إذا (! readlinebuffer_delim (&سطر->buf, fp, eolchar))
    {
      إذا (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (سطر);
      أرجع false;
    }
  ++سطر_no[which - 1];

  xfields (سطر);

  إذا (prevline[which - 1])
    check_ترتیب (prevline[which - 1], سطر, which);

  prevline[which - 1] = سطر;
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
  seq->شمار = 0;
  seq->alloc = 0;
  seq->سطرها = عدم;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

ساكن bool
getseq (FILE *fp, بنية seq *seq, صحيح whichfile)
{
  إذا (seq->شمار == seq->alloc)
    {
      seq->سطرها = xpalloc (seq->سطرها, &seq->alloc, 1, -1, حجم *seq->سطرها);
      لكل (idx_t i = seq->شمار; i < seq->alloc; i++)
        seq->سطرها[i] = عدم;
    }

  إذا (get_سطر (fp, &seq->سطرها[seq->شمار], whichfile))
    {
      ++seq->شمار;
      أرجع true;
    }
  أرجع false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
ساكن bool
advance_seq (FILE *fp, بنية seq *seq, bool اولی, صحيح whichfile)
{
  إذا (اولی)
    seq->شمار = 0;

  أرجع getseq (fp, seq, whichfile);
}

ساكن فراغ
delseq (بنية seq *seq)
{
  لكل (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->سطرها[i]);
      حرر (seq->سطرها[i]);
    }
  حرر (seq->سطرها);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

ساكن فراغ
prfield (idx_t n, بنية سطر ثابت *سطر)
{
  إذا (n < سطر->nfields)
    {
      idx_t len = سطر->fields[n].len;
      إذا (len)
        fwrite (سطر->fields[n].beg, 1, len, stdout);
      وإلا إذا (خالی_filler)
        fputs (خالی_filler, stdout);
    }
  وإلا إذا (خالی_filler)
    fputs (خالی_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

ساكن فراغ
prfields (بنية سطر ثابت *سطر, idx_t join_فیلد, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : سطر->nfields;

  لكل (idx_t i = 0; i < join_فیلد && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, سطر);
    }
  لكل (idx_t i = join_فیلد + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, سطر);
    }
}

/* Print the join of LINE1 and LINE2.  */

ساكن فراغ
prjoin (بنية سطر ثابت *line1, بنية سطر ثابت *line2)
{
  ثابت بنية outlist *outlist;
  idx_t فیلد;
  بنية سطر ثابت *سطر;

  outlist = outlist_سر.بعدی;
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
                  سطر = line2;
                  فیلد = join_فیلد_2;
                }
              وإلا
                {
                  سطر = line1;
                  فیلد = join_فیلد_1;
                }
            }
          وإلا
            {
              سطر = (o->file == 1 ? line1 : line2);
              فیلد = o->فیلد;
            }
          prfield (فیلد, سطر);
          o = o->بعدی;
          إذا (o == عدم)
            اكسر;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      حرف_چاپ_کن (eolchar);
    }
  وإلا
    {
      إذا (line1 == &uni_blank)
        {
          سطر = line2;
          فیلد = join_فیلد_2;
        }
      وإلا
        {
          سطر = line1;
          فیلد = join_فیلد_1;
        }

      /* Output the join field.  */
      prfield (فیلد, سطر);

      /* Output other fields.  */
      prfields (line1, join_فیلد_1, autocount_1);
      prfields (line2, join_فیلد_2, autocount_2);

      حرف_چاپ_کن (eolchar);
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
      autocount_1 = seq1.شمار ? seq1.سطرها[0]->nfields : 0;
      autocount_2 = seq2.شمار ? seq2.سطرها[0]->nfields : 0;
    }

  إذا (join_سرآیند_سطرها && (seq1.شمار || seq2.شمار))
    {
      بنية سطر ثابت *hline1 = seq1.شمار ? seq1.سطرها[0] : &uni_blank;
      بنية سطر ثابت *hline2 = seq2.شمار ? seq2.سطرها[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = عدم;
      prevline[1] = عدم;
      إذا (seq1.شمار)
        advance_seq (fp1, &seq1, true, 1);
      إذا (seq2.شمار)
        advance_seq (fp2, &seq2, true, 2);
    }

  طالما (seq1.شمار && seq2.شمار)
    {
      diff = keycmp (seq1.سطرها[0], seq2.سطرها[0],
                     join_فیلد_1, join_فیلد_2);
      إذا (diff < 0)
        {
          إذا (print_unpairables_1)
            prjoin (seq1.سطرها[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          دیده_unpairable = true;
          استمر;
        }
      إذا (diff > 0)
        {
          إذا (print_unpairables_2)
            prjoin (&uni_blank, seq2.سطرها[0]);
          advance_seq (fp2, &seq2, true, 2);
          دیده_unpairable = true;
          استمر;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      افعل
        إذا (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.شمار;
            اكسر;
          }
      طالما (!keycmp (seq1.سطرها[seq1.شمار - 1], seq2.سطرها[0],
                      join_فیلد_1, join_فیلد_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      افعل
        إذا (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.شمار;
            اكسر;
          }
      طالما (!keycmp (seq1.سطرها[0], seq2.سطرها[seq2.شمار - 1],
                      join_فیلد_1, join_فیلد_2));

      إذا (print_pairables)
        {
          لكل (idx_t i = 0; i < seq1.شمار - 1; ++i)
            {
              لكل (idx_t j = 0; j < seq2.شمار - 1; ++j)
                prjoin (seq1.سطرها[i], seq2.سطرها[j]);
            }
        }

      إذا (!eof1)
        {
          SWAPLINES (seq1.سطرها[0], seq1.سطرها[seq1.شمار - 1]);
          seq1.شمار = 1;
        }
      وإلا
        seq1.شمار = 0;

      إذا (!eof2)
        {
          SWAPLINES (seq2.سطرها[0], seq2.سطرها[seq2.شمار - 1]);
          seq2.شمار = 1;
        }
      وإلا
        seq2.شمار = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  بنية سطر *سطر = عدم;
  bool checktail = false;

  إذا (check_درون_داد_ترتیب != CHECK_ترتیب_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  إذا ((print_unpairables_1 || checktail) && seq1.شمار)
    {
      إذا (print_unpairables_1)
        prjoin (seq1.سطرها[0], &uni_blank);
      إذا (seq2.شمار)
        دیده_unpairable = true;
      طالما (get_سطر (fp1, &سطر, 1))
        {
          إذا (print_unpairables_1)
            prjoin (سطر, &uni_blank);
          إذا (issued_disorder_warning[0] && !print_unpairables_1)
            اكسر;
        }
    }

  إذا ((print_unpairables_2 || checktail) && seq2.شمار)
    {
      إذا (print_unpairables_2)
        prjoin (&uni_blank, seq2.سطرها[0]);
      إذا (seq1.شمار)
        دیده_unpairable = true;
      طالما (get_سطر (fp2, &سطر, 2))
        {
          إذا (print_unpairables_2)
            prjoin (&uni_blank, سطر);
          إذا (issued_disorder_warning[1] && !print_unpairables_2)
            اكسر;
        }
    }

  freeline (سطر);
  حرر (سطر);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

ساكن فراغ
جمع_کن_فیلد (صحيح file, idx_t فیلد)
{
  بنية outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || فیلد == 0);

  o = xmalloc (حجم *o);
  o->file = file;
  o->فیلد = فیلد;
  o->بعدی = عدم;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_انتها->بعدی = o;
  outlist_انتها = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

ساكن idx_t
string_مقصد_join_فیلد (حرف ثابت *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, عدم, 10, &val, "");
  إذا (s_err == LONGINT_سرریز || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  وإلا إذا (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  أرجع val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

ساكن فراغ
decode_فیلد_spec (حرف ثابت *s, صحيح *file_شاخص, idx_t *فیلد_شاخص)
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
      *file_شاخص = 0;
      *فیلد_شاخص = 0;
      اكسر;

    حالة '1':
    حالة '2':
      إذا (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_شاخص = s[0] - '0';
      *فیلد_شاخص = string_مقصد_join_فیلد (s + 2);
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
جمع_کن_فیلد_list (حرف *str)
{
  حرف *p = str;

  افعل
    {
      صحيح file_شاخص;
      idx_t فیلد_شاخص;
      حرف ثابت *spec_قلم = p;
      p = skip_str_matching (spec_قلم, comma_or_blank, false);
      إذا (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_فیلد_spec (spec_قلم, &file_شاخص, &فیلد_شاخص);
      جمع_کن_فیلد (file_شاخص, فیلد_شاخص);
    }
  طالما (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

ساكن فراغ
مجموعه_join_فیلد (ptrdiff_t *var, idx_t val)
{
  إذا (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

تعداد عملوند_وضعیت
  {
    /* This argument must be an operand, i.e., one of the files to be
       joined.  */
    MUST_BE_عملوند,

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
جمع_کن_file_name (حرف *name, حرف *نامها[2],
               صحيح عملوند_وضعیت[2], صحيح joption_شمار[2], صحيح *nfiles,
               صحيح *prev_optc_وضعیت, صحيح *optc_وضعیت)
{
  صحيح n = *nfiles;

  إذا (n == 2)
    {
      bool op0 = (عملوند_وضعیت[0] == MUST_BE_عملوند);
      حرف *arg = نامها[op0];
      بدل (عملوند_وضعیت[op0])
        {
        حالة MUST_BE_عملوند:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        حالة MIGHT_BE_J1_ARG:
          joption_شمار[0]--;
          مجموعه_join_فیلد (&join_فیلد_1, string_مقصد_join_فیلد (arg));
          اكسر;

        حالة MIGHT_BE_J2_ARG:
          joption_شمار[1]--;
          مجموعه_join_فیلد (&join_فیلد_2, string_مقصد_join_فیلد (arg));
          اكسر;

        حالة MIGHT_BE_O_ARG:
          جمع_کن_فیلد_list (arg);
          اكسر;
        }
      إذا (!op0)
        {
          عملوند_وضعیت[0] = عملوند_وضعیت[1];
          نامها[0] = نامها[1];
        }
      n = 1;
    }

  عملوند_وضعیت[n] = *prev_optc_وضعیت;
  نامها[n] = name;
  *nfiles = n + 1;
  إذا (*prev_optc_وضعیت == MIGHT_BE_O_ARG)
    *optc_وضعیت = MIGHT_BE_O_ARG;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح optc_وضعیت;
  صحيح prev_optc_وضعیت = MUST_BE_عملوند;
  صحيح عملوند_وضعیت[2];
  صحيح joption_شمار[2] = { 0, 0 };
  FILE *fp1, *fp2;
  صحيح optc;
  صحيح nfiles = 0;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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
      optc_وضعیت = MUST_BE_عملوند;

      بدل (optc)
        {
        حالة 'v':
            print_pairables = false;
            FALLTHROUGH;

        حالة 'a':
          {
            دراز صحيح val;
            إذا (xstrtol (optarg, عدم, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            إذا (val == 1)
              print_unpairables_1 = true;
            وإلا
              print_unpairables_2 = true;
          }
          اكسر;

        حالة 'e':
          إذا (خالی_filler && ! streq (خالی_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          خالی_filler = optarg;
          اكسر;

        حالة 'i':
          ignore_case = true;
          اكسر;

        حالة '1':
          مجموعه_join_فیلد (&join_فیلد_1, string_مقصد_join_فیلد (optarg));
          اكسر;

        حالة '2':
          مجموعه_join_فیلد (&join_فیلد_2, string_مقصد_join_فیلد (optarg));
          اكسر;

        حالة 'j':
          إذا ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_شمار[is_j2]++;
              optc_وضعیت = MIGHT_BE_J1_ARG + is_j2;
            }
          وإلا
            {
              مجموعه_join_فیلد (&join_فیلد_1, string_مقصد_join_فیلد (optarg));
              مجموعه_join_فیلد (&join_فیلد_2, join_فیلد_1);
            }
          اكسر;

        حالة 'o':
          إذا (streq (optarg, "auto"))
            autoformat = true;
          وإلا
            {
              جمع_کن_فیلد_list (optarg);
              optc_وضعیت = MIGHT_BE_O_ARG;
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

        حالة NOCHECK_ترتیب_OPTION:
          check_درون_داد_ترتیب = CHECK_ترتیب_DISABLED;
          اكسر;

        حالة CHECK_ترتیب_OPTION:
          check_درون_داد_ترتیب = CHECK_ترتیب_ENABLED;
          اكسر;

        حالة 1:		/* Non-option argument.  */
          جمع_کن_file_name (optarg, g_نامها, عملوند_وضعیت, joption_شمار,
                         &nfiles, &prev_optc_وضعیت, &optc_وضعیت);
          اكسر;

        حالة سرآیند_سطر_OPTION:
          join_سرآیند_سطرها = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }

      prev_optc_وضعیت = optc_وضعیت;
    }

  /* Process any operands after "--".  */
  prev_optc_وضعیت = MUST_BE_عملوند;
  طالما (optind < argc)
    جمع_کن_file_name (argv[optind++], g_نامها, عملوند_وضعیت, joption_شمار,
                   &nfiles, &prev_optc_وضعیت, &optc_وضعیت);

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
    إذا (joption_شمار[i] != 0)
      {
        مجموعه_join_فیلد (&join_فیلد_1, i);
        مجموعه_join_فیلد (&join_فیلد_2, i);
      }

  إذا (join_فیلد_1 < 0)
    join_فیلد_1 = 0;
  إذا (join_فیلد_2 < 0)
    join_فیلد_2 = 0;

  fp1 = streq (g_نامها[0], "-") ? stdin : fopen (g_نامها[0], "r");
  إذا (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نامها[0]));
  fp2 = streq (g_نامها[1], "-") ? stdin : fopen (g_نامها[1], "r");
  إذا (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نامها[1]));
  إذا (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  إذا (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نامها[0]));
  إذا (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نامها[1]));

  إذا (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  وإلا
    أرجع EXIT_SUCCESS;
}
