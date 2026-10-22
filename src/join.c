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
#تعریف_کړه پروګرام_NAME "join"

#تعریف_کړه AUTHORS proper_name ("Mike Haertel")

#تعریف_کړه join system_join

#تعریف_کړه SWAPLINES(a, b) افعل { \
  بنية کرښه *tmp = a; \
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
    idx_t ډګر;

    بنية outlist *بل;
  };

/* A field of a line.  */
بنية ډګر
  {
    حرف *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
بنية کرښه
  {
    بنية linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    بنية ډګر *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
بنية seq
  {
    idx_t شمېر;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    بنية کرښه **کرښې COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
ساكن بنية کرښه *prevline[2] = {عدم, عدم};

/* The number of lines read from each file.  */
ساكن uintmax_t کرښه_no[2] = {0, 0};

/* The input file names.  */
ساكن حرف *g_نومونه[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
ساكن بنية کرښه *spareline[2] = {عدم, عدم};

/* True if the LC_COLLATE locale is hard.  */
ساكن bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
ساكن bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
ساكن bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
ساكن bool کتل_شوی_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ساكن bool issued_disorder_warning[2];

/* Empty output field filler.  */
ساكن حرف ثابت *تشه_filler;

/* Whether to ensure the same number of fields are output from each line.  */
ساكن bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
ساكن idx_t autocount_1;
ساكن idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
ساكن ptrdiff_t join_ډګر_1 = -1;
ساكن ptrdiff_t join_ډګر_2 = -1;

/* List of fields to print.  */
ساكن بنية outlist outlist_سر;

/* Last element in 'outlist', where a new element can be added.  */
ساكن بنية outlist *outlist_پای_ټکی = &outlist_سر;

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
  } check_ننوت_ترتیب;

تعداد
{
  CHECK_ترتیب_OPTION = CHAR_MAX + 1,
  NOCHECK_ترتیب_OPTION,
  سرلیک_کرښه_OPTION
};


ساكن بنية option ثابت longopts[] =
{
  {"ignore-case", no_دلیل, عدم, 'i'},
  {"check-order", no_دلیل, عدم, CHECK_ترتیب_OPTION},
  {"nocheck-order", no_دلیل, عدم, NOCHECK_ترتیب_OPTION},
  {"zero-terminated", no_دلیل, عدم, 'z'},
  {"header", no_دلیل, عدم, سرلیک_کرښه_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Used to print non-joining lines */
ساكن بنية کرښه uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
ساكن bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
ساكن bool join_سرلیک_کرښې;

/* The character marking end of line. Default to \n. */
ساكن حرف eolchar = '\n';

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              پروګرام_name);
      fputs (_("\
For each pair د ننوت کرښې with identical join fields, اكتب a کرښه منزل\n\
standard output.  The افتراضي join ډګر is the لومړی, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, اقرأ standard ننوت.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable کرښې from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding منزل FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (تشه) ننوت fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-حالة\n\
         ignore differences دننه حالة when comparing fields\n\
"));
      oputs (_("\
  -j ډګر_2\n\
         equivalent منزل '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT طالما constructing output کرښه\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as ننوت and output ډګر separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output کرښې\n\
"));
      oputs (_("\
  -1 ډګر_2\n\
         join on this ډګر_2 د file 1\n\
"));
      oputs (_("\
  -2 ډګر_2\n\
         join on this ډګر_2 د file 2\n\
"));
      oputs (_("\
      --check-ترتیب\n\
         check that the ننوت is correctly sorted, even\n\
         إذا all ننوت کرښې are pairable\n\
"));
      oputs (_("\
      --nocheck-ترتیب\n\
         افعل not check that the ننوت is correctly sorted\n\
"));
      oputs (_("\
      --سرلیک\n\
         treat the لومړی کرښه دننه each file as ډګر سرلیکونه,\n\
         print them without trying منزل pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         کرښه delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
وإلا fields are separated by CHAR.  Any ډګر_2 is a ډګر شمېره counted\n\
from 1.  FORMAT is one or نور comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join ډګر,\n\
the پاتې_شوی fields from FILE1, the پاتې_شوی fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the لومړی\n\
کرښه د each file determines the شمېره د fields output لكل each کرښه.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"ترتیبول -k 1b,1\" إذا 'join' has no options,\n\
or use \"join -t ''\" إذا 'sort' has no options.\n\
Comparisons honor the قواعد specified by 'LC_COLLATE'.\n\
If the ننوت is not sorted and some کرښې cannot be joined, a\n\
warning پیغام_2 will be given.\n\
"), stdout);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

ساكن فراغ
ایستل_ډګر (بنية کرښه *کرښه, حرف *ډګر, idx_t len)
{
  إذا (کرښه->nfields >= کرښه->nfields_allocated)
    کرښه->fields = xpalloc (کرښه->fields, &کرښه->nfields_allocated, 1,
                            -1, حجم *کرښه->fields);
  idx_t ثابت ډګر_شاخص = کرښه->nfields++;
  کرښه->fields[ډګر_شاخص].beg = ډګر;
  کرښه->fields[ډګر_شاخص].len = len;
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
xfields (بنية کرښه *کرښه)
{
  حرف *ptr = کرښه->buf.بفر;
  حرف ثابت *lim = ptr + کرښه->buf.اوږدتیا - 1;

  إذا (ptr == lim)
    أرجع;

  إذا (!tab.len)
    طالما ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        حرف *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        ایستل_ډګر (کرښه, ptr, sep - ptr);
        ptr = sep;
      }
  وإلا
    {
      إذا (tab.ch != '\n')
        لكل (حرف *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_لټون (sep, lim).len)
          ایستل_ډګر (کرښه, ptr, sep - ptr);

      ایستل_ډګر (کرښه, ptr, lim - ptr);
    }
}

ساكن فراغ
freeline (بنية کرښه *کرښه)
{
  إذا (کرښه == عدم)
    أرجع;
  حرر (کرښه->fields);
  کرښه->fields = عدم;
  کرښه->nfields_allocated = 0;
  حرر (کرښه->buf.بفر);
  کرښه->buf.بفر = عدم;
  کرښه->buf.اندازه = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

ساكن صحيح
keycmp (بنية کرښه ثابت *line1, بنية کرښه ثابت *line2,
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
check_ترتیب (ثابت بنية کرښه *prev,
             ثابت بنية کرښه *current,
             صحيح whatfile)
{
  إذا (check_ننوت_ترتیب != CHECK_ترتیب_DISABLED
      && ((check_ننوت_ترتیب == CHECK_ترتیب_ENABLED) || کتل_شوی_unpairable))
    {
      إذا (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_ډګر = whatfile == 1 ? join_ډګر_1 : join_ډګر_2;
          إذا (keycmp (prev, current, join_ډګر, join_ډګر) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.اوږدتیا;
              إذا (0 < len && current->buf.بفر[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (لوی_عدد, len);

              error ((check_ننوت_ترتیب == CHECK_ترتیب_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_نومونه[whatfile - 1], کرښه_no[whatfile - 1],
                     (صحيح) len, current->buf.بفر);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

ساكن مضمن فراغ
reset_کرښه (بنية کرښه *کرښه)
{
  کرښه->nfields = 0;
}

ساكن بنية کرښه *
init_linep (بنية کرښه **linep)
{
  بنية کرښه *کرښه = xzalloc (حجم *کرښه);
  *linep = کرښه;
  أرجع کرښه;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

ساكن bool
get_کرښه (FILE *fp, بنية کرښه **linep, صحيح which)
{
  بنية کرښه *کرښه = *linep;

  إذا (کرښه == prevline[which - 1])
    {
      SWAPLINES (کرښه, spareline[which - 1]);
      *linep = کرښه;
    }

  إذا (کرښه)
    reset_کرښه (کرښه);
  وإلا
    کرښه = init_linep (linep);

  إذا (! readlinebuffer_delim (&کرښه->buf, fp, eolchar))
    {
      إذا (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (کرښه);
      أرجع false;
    }
  ++کرښه_no[which - 1];

  xfields (کرښه);

  إذا (prevline[which - 1])
    check_ترتیب (prevline[which - 1], کرښه, which);

  prevline[which - 1] = کرښه;
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
  seq->شمېر = 0;
  seq->alloc = 0;
  seq->کرښې = عدم;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

ساكن bool
getseq (FILE *fp, بنية seq *seq, صحيح whichfile)
{
  إذا (seq->شمېر == seq->alloc)
    {
      seq->کرښې = xpalloc (seq->کرښې, &seq->alloc, 1, -1, حجم *seq->کرښې);
      لكل (idx_t i = seq->شمېر; i < seq->alloc; i++)
        seq->کرښې[i] = عدم;
    }

  إذا (get_کرښه (fp, &seq->کرښې[seq->شمېر], whichfile))
    {
      ++seq->شمېر;
      أرجع true;
    }
  أرجع false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
ساكن bool
advance_seq (FILE *fp, بنية seq *seq, bool لومړی, صحيح whichfile)
{
  إذا (لومړی)
    seq->شمېر = 0;

  أرجع getseq (fp, seq, whichfile);
}

ساكن فراغ
delseq (بنية seq *seq)
{
  لكل (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->کرښې[i]);
      حرر (seq->کرښې[i]);
    }
  حرر (seq->کرښې);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

ساكن فراغ
prfield (idx_t n, بنية کرښه ثابت *کرښه)
{
  إذا (n < کرښه->nfields)
    {
      idx_t len = کرښه->fields[n].len;
      إذا (len)
        fwrite (کرښه->fields[n].beg, 1, len, stdout);
      وإلا إذا (تشه_filler)
        fputs (تشه_filler, stdout);
    }
  وإلا إذا (تشه_filler)
    fputs (تشه_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

ساكن فراغ
prfields (بنية کرښه ثابت *کرښه, idx_t join_ډګر, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : کرښه->nfields;

  لكل (idx_t i = 0; i < join_ډګر && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, کرښه);
    }
  لكل (idx_t i = join_ډګر + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, کرښه);
    }
}

/* Print the join of LINE1 and LINE2.  */

ساكن فراغ
prjoin (بنية کرښه ثابت *line1, بنية کرښه ثابت *line2)
{
  ثابت بنية outlist *outlist;
  idx_t ډګر;
  بنية کرښه ثابت *کرښه;

  outlist = outlist_سر.بل;
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
                  کرښه = line2;
                  ډګر = join_ډګر_2;
                }
              وإلا
                {
                  کرښه = line1;
                  ډګر = join_ډګر_1;
                }
            }
          وإلا
            {
              کرښه = (o->file == 1 ? line1 : line2);
              ډګر = o->ډګر;
            }
          prfield (ډګر, کرښه);
          o = o->بل;
          إذا (o == عدم)
            اكسر;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      توری_چاپ_کړه (eolchar);
    }
  وإلا
    {
      إذا (line1 == &uni_blank)
        {
          کرښه = line2;
          ډګر = join_ډګر_2;
        }
      وإلا
        {
          کرښه = line1;
          ډګر = join_ډګر_1;
        }

      /* Output the join field.  */
      prfield (ډګر, کرښه);

      /* Output other fields.  */
      prfields (line1, join_ډګر_1, autocount_1);
      prfields (line2, join_ډګر_2, autocount_2);

      توری_چاپ_کړه (eolchar);
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
      autocount_1 = seq1.شمېر ? seq1.کرښې[0]->nfields : 0;
      autocount_2 = seq2.شمېر ? seq2.کرښې[0]->nfields : 0;
    }

  إذا (join_سرلیک_کرښې && (seq1.شمېر || seq2.شمېر))
    {
      بنية کرښه ثابت *hline1 = seq1.شمېر ? seq1.کرښې[0] : &uni_blank;
      بنية کرښه ثابت *hline2 = seq2.شمېر ? seq2.کرښې[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = عدم;
      prevline[1] = عدم;
      إذا (seq1.شمېر)
        advance_seq (fp1, &seq1, true, 1);
      إذا (seq2.شمېر)
        advance_seq (fp2, &seq2, true, 2);
    }

  طالما (seq1.شمېر && seq2.شمېر)
    {
      diff = keycmp (seq1.کرښې[0], seq2.کرښې[0],
                     join_ډګر_1, join_ډګر_2);
      إذا (diff < 0)
        {
          إذا (print_unpairables_1)
            prjoin (seq1.کرښې[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          کتل_شوی_unpairable = true;
          استمر;
        }
      إذا (diff > 0)
        {
          إذا (print_unpairables_2)
            prjoin (&uni_blank, seq2.کرښې[0]);
          advance_seq (fp2, &seq2, true, 2);
          کتل_شوی_unpairable = true;
          استمر;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      افعل
        إذا (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.شمېر;
            اكسر;
          }
      طالما (!keycmp (seq1.کرښې[seq1.شمېر - 1], seq2.کرښې[0],
                      join_ډګر_1, join_ډګر_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      افعل
        إذا (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.شمېر;
            اكسر;
          }
      طالما (!keycmp (seq1.کرښې[0], seq2.کرښې[seq2.شمېر - 1],
                      join_ډګر_1, join_ډګر_2));

      إذا (print_pairables)
        {
          لكل (idx_t i = 0; i < seq1.شمېر - 1; ++i)
            {
              لكل (idx_t j = 0; j < seq2.شمېر - 1; ++j)
                prjoin (seq1.کرښې[i], seq2.کرښې[j]);
            }
        }

      إذا (!eof1)
        {
          SWAPLINES (seq1.کرښې[0], seq1.کرښې[seq1.شمېر - 1]);
          seq1.شمېر = 1;
        }
      وإلا
        seq1.شمېر = 0;

      إذا (!eof2)
        {
          SWAPLINES (seq2.کرښې[0], seq2.کرښې[seq2.شمېر - 1]);
          seq2.شمېر = 1;
        }
      وإلا
        seq2.شمېر = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  بنية کرښه *کرښه = عدم;
  bool checktail = false;

  إذا (check_ننوت_ترتیب != CHECK_ترتیب_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  إذا ((print_unpairables_1 || checktail) && seq1.شمېر)
    {
      إذا (print_unpairables_1)
        prjoin (seq1.کرښې[0], &uni_blank);
      إذا (seq2.شمېر)
        کتل_شوی_unpairable = true;
      طالما (get_کرښه (fp1, &کرښه, 1))
        {
          إذا (print_unpairables_1)
            prjoin (کرښه, &uni_blank);
          إذا (issued_disorder_warning[0] && !print_unpairables_1)
            اكسر;
        }
    }

  إذا ((print_unpairables_2 || checktail) && seq2.شمېر)
    {
      إذا (print_unpairables_2)
        prjoin (&uni_blank, seq2.کرښې[0]);
      إذا (seq1.شمېر)
        کتل_شوی_unpairable = true;
      طالما (get_کرښه (fp2, &کرښه, 2))
        {
          إذا (print_unpairables_2)
            prjoin (&uni_blank, کرښه);
          إذا (issued_disorder_warning[1] && !print_unpairables_2)
            اكسر;
        }
    }

  freeline (کرښه);
  حرر (کرښه);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

ساكن فراغ
جمع_کړه_ډګر (صحيح file, idx_t ډګر)
{
  بنية outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || ډګر == 0);

  o = xmalloc (حجم *o);
  o->file = file;
  o->ډګر = ډګر;
  o->بل = عدم;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_پای_ټکی->بل = o;
  outlist_پای_ټکی = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

ساكن idx_t
string_منزل_join_ډګر (حرف ثابت *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, عدم, 10, &val, "");
  إذا (s_err == LONGINT_ډکېدنه || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  وإلا إذا (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  أرجع val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

ساكن فراغ
decode_ډګر_spec (حرف ثابت *s, صحيح *file_شاخص, idx_t *ډګر_شاخص)
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
      *ډګر_شاخص = 0;
      اكسر;

    حالة '1':
    حالة '2':
      إذا (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_شاخص = s[0] - '0';
      *ډګر_شاخص = string_منزل_join_ډګر (s + 2);
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
جمع_کړه_ډګر_list (حرف *str)
{
  حرف *p = str;

  افعل
    {
      صحيح file_شاخص;
      idx_t ډګر_شاخص;
      حرف ثابت *spec_توکی = p;
      p = skip_str_matching (spec_توکی, comma_or_blank, false);
      إذا (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_ډګر_spec (spec_توکی, &file_شاخص, &ډګر_شاخص);
      جمع_کړه_ډګر (file_شاخص, ډګر_شاخص);
    }
  طالما (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

ساكن فراغ
سېټ_join_ډګر (ptrdiff_t *var, idx_t val)
{
  إذا (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

تعداد عملوند_حالت
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
جمع_کړه_file_name (حرف *name, حرف *نومونه[2],
               صحيح عملوند_حالت[2], صحيح joption_شمېر[2], صحيح *nfiles,
               صحيح *prev_optc_حالت, صحيح *optc_حالت)
{
  صحيح n = *nfiles;

  إذا (n == 2)
    {
      bool op0 = (عملوند_حالت[0] == MUST_BE_عملوند);
      حرف *arg = نومونه[op0];
      بدل (عملوند_حالت[op0])
        {
        حالة MUST_BE_عملوند:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        حالة MIGHT_BE_J1_ARG:
          joption_شمېر[0]--;
          سېټ_join_ډګر (&join_ډګر_1, string_منزل_join_ډګر (arg));
          اكسر;

        حالة MIGHT_BE_J2_ARG:
          joption_شمېر[1]--;
          سېټ_join_ډګر (&join_ډګر_2, string_منزل_join_ډګر (arg));
          اكسر;

        حالة MIGHT_BE_O_ARG:
          جمع_کړه_ډګر_list (arg);
          اكسر;
        }
      إذا (!op0)
        {
          عملوند_حالت[0] = عملوند_حالت[1];
          نومونه[0] = نومونه[1];
        }
      n = 1;
    }

  عملوند_حالت[n] = *prev_optc_حالت;
  نومونه[n] = name;
  *nfiles = n + 1;
  إذا (*prev_optc_حالت == MIGHT_BE_O_ARG)
    *optc_حالت = MIGHT_BE_O_ARG;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  صحيح optc_حالت;
  صحيح prev_optc_حالت = MUST_BE_عملوند;
  صحيح عملوند_حالت[2];
  صحيح joption_شمېر[2] = { 0, 0 };
  FILE *fp1, *fp2;
  صحيح optc;
  صحيح nfiles = 0;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
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
      optc_حالت = MUST_BE_عملوند;

      بدل (optc)
        {
        حالة 'v':
            print_pairables = false;
            FALLTHROUGH;

        حالة 'a':
          {
            اوږد صحيح val;
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
          إذا (تشه_filler && ! streq (تشه_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          تشه_filler = optarg;
          اكسر;

        حالة 'i':
          ignore_case = true;
          اكسر;

        حالة '1':
          سېټ_join_ډګر (&join_ډګر_1, string_منزل_join_ډګر (optarg));
          اكسر;

        حالة '2':
          سېټ_join_ډګر (&join_ډګر_2, string_منزل_join_ډګر (optarg));
          اكسر;

        حالة 'j':
          إذا ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_شمېر[is_j2]++;
              optc_حالت = MIGHT_BE_J1_ARG + is_j2;
            }
          وإلا
            {
              سېټ_join_ډګر (&join_ډګر_1, string_منزل_join_ډګر (optarg));
              سېټ_join_ډګر (&join_ډګر_2, join_ډګر_1);
            }
          اكسر;

        حالة 'o':
          إذا (streq (optarg, "auto"))
            autoformat = true;
          وإلا
            {
              جمع_کړه_ډګر_list (optarg);
              optc_حالت = MIGHT_BE_O_ARG;
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
          check_ننوت_ترتیب = CHECK_ترتیب_DISABLED;
          اكسر;

        حالة CHECK_ترتیب_OPTION:
          check_ننوت_ترتیب = CHECK_ترتیب_ENABLED;
          اكسر;

        حالة 1:		/* Non-option argument.  */
          جمع_کړه_file_name (optarg, g_نومونه, عملوند_حالت, joption_شمېر,
                         &nfiles, &prev_optc_حالت, &optc_حالت);
          اكسر;

        حالة سرلیک_کرښه_OPTION:
          join_سرلیک_کرښې = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }

      prev_optc_حالت = optc_حالت;
    }

  /* Process any operands after "--".  */
  prev_optc_حالت = MUST_BE_عملوند;
  طالما (optind < argc)
    جمع_کړه_file_name (argv[optind++], g_نومونه, عملوند_حالت, joption_شمېر,
                   &nfiles, &prev_optc_حالت, &optc_حالت);

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
    إذا (joption_شمېر[i] != 0)
      {
        سېټ_join_ډګر (&join_ډګر_1, i);
        سېټ_join_ډګر (&join_ډګر_2, i);
      }

  إذا (join_ډګر_1 < 0)
    join_ډګر_1 = 0;
  إذا (join_ډګر_2 < 0)
    join_ډګر_2 = 0;

  fp1 = streq (g_نومونه[0], "-") ? stdin : fopen (g_نومونه[0], "r");
  إذا (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نومونه[0]));
  fp2 = streq (g_نومونه[1], "-") ? stdin : fopen (g_نومونه[1], "r");
  إذا (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نومونه[1]));
  إذا (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  إذا (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نومونه[0]));
  إذا (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نومونه[1]));

  إذا (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  وإلا
    أرجع EXIT_SUCCESS;
}
