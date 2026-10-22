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
#وصف_ڪريو پروگرام_NAME "join"

#وصف_ڪريو AUTHORS proper_name ("Mike Haertel")

#وصف_ڪريو join system_join

#وصف_ڪريو SWAPLINES(a, b) افعل { \
  بنية سٽ *tmp = a; \
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
    idx_t ميدان;

    بنية outlist *ايندڙ;
  };

/* A field of a line.  */
بنية ميدان
  {
    حرف *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
بنية سٽ
  {
    بنية linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    بنية ميدان *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
بنية seq
  {
    idx_t ڳڻپ;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    بنية سٽ **سٽون COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
ساكن بنية سٽ *prevline[2] = {عدم, عدم};

/* The number of lines read from each file.  */
ساكن uintmax_t سٽ_no[2] = {0, 0};

/* The input file names.  */
ساكن حرف *g_نالا[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
ساكن بنية سٽ *spareline[2] = {عدم, عدم};

/* True if the LC_COLLATE locale is hard.  */
ساكن bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
ساكن bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
ساكن bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
ساكن bool ڏٺل_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ساكن bool issued_disorder_warning[2];

/* Empty output field filler.  */
ساكن حرف ثابت *خالي_جايون_filler;

/* Whether to ensure the same number of fields are output from each line.  */
ساكن bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
ساكن idx_t autocount_1;
ساكن idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
ساكن ptrdiff_t join_ميدان_1 = -1;
ساكن ptrdiff_t join_ميدان_2 = -1;

/* List of fields to print.  */
ساكن بنية outlist outlist_سر;

/* Last element in 'outlist', where a new element can be added.  */
ساكن بنية outlist *outlist_پڇاڙي = &outlist_سر;

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
    CHECK_ترتيب_DEFAULT,
    CHECK_ترتيب_ENABLED,
    CHECK_ترتيب_DISABLED
  } check_داخلا_ترتيب;

تعداد
{
  CHECK_ترتيب_OPTION = CHAR_MAX + 1,
  NOCHECK_ترتيب_OPTION,
  سرو_سٽ_OPTION
};


ساكن بنية option ثابت longopts[] =
{
  {"ignore-case", no_دليل, عدم, 'i'},
  {"check-order", no_دليل, عدم, CHECK_ترتيب_OPTION},
  {"nocheck-order", no_دليل, عدم, NOCHECK_ترتيب_OPTION},
  {"zero-terminated", no_دليل, عدم, 'z'},
  {"header", no_دليل, عدم, سرو_سٽ_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Used to print non-joining lines */
ساكن بنية سٽ uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
ساكن bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
ساكن bool join_سرو_سٽون;

/* The character marking end of line. Default to \n. */
ساكن حرف eolchar = '\n';

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              پروگرام_name);
      fputs (_("\
For each pair جو داخلا سٽون with identical join fields, اكتب a سٽ منزل\n\
standard output.  The افتراضي join ميدان is the پهريون, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, اقرأ standard داخلا.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable سٽون from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding منزل FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (خالي_جايون) داخلا fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-حالة\n\
         ignore differences اندريون حالة when comparing fields\n\
"));
      oputs (_("\
  -j ميدان_2\n\
         equivalent منزل '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT طالما constructing output سٽ\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as داخلا and output ميدان separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output سٽون\n\
"));
      oputs (_("\
  -1 ميدان_2\n\
         join on this ميدان_2 جو file 1\n\
"));
      oputs (_("\
  -2 ميدان_2\n\
         join on this ميدان_2 جو file 2\n\
"));
      oputs (_("\
      --check-ترتيب\n\
         check that the داخلا is correctly sorted, even\n\
         إذا all داخلا سٽون are pairable\n\
"));
      oputs (_("\
      --nocheck-ترتيب\n\
         افعل not check that the داخلا is correctly sorted\n\
"));
      oputs (_("\
      --سرو\n\
         treat the پهريون سٽ اندريون each file as ميدان سرا,\n\
         print them without trying منزل pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سٽ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
وإلا fields are separated by CHAR.  Any ميدان_2 is a ميدان عدد counted\n\
from 1.  FORMAT is one or وڌيڪ comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join ميدان,\n\
the بچيل fields from FILE1, the بچيل fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the پهريون\n\
سٽ جو each file determines the عدد جو fields output لكل each سٽ.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"ترتيب_ڏيو -k 1b,1\" إذا 'join' has no options,\n\
or use \"join -t ''\" إذا 'sort' has no options.\n\
Comparisons honor the قاعدا specified by 'LC_COLLATE'.\n\
If the داخلا is not sorted and some سٽون cannot be joined, a\n\
warning پيغام_2 will be given.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

ساكن فراغ
ڪڍو_ميدان (بنية سٽ *سٽ, حرف *ميدان, idx_t len)
{
  إذا (سٽ->nfields >= سٽ->nfields_allocated)
    سٽ->fields = xpalloc (سٽ->fields, &سٽ->nfields_allocated, 1,
                            -1, حجم *سٽ->fields);
  idx_t ثابت ميدان_اشاريو = سٽ->nfields++;
  سٽ->fields[ميدان_اشاريو].beg = ميدان;
  سٽ->fields[ميدان_اشاريو].len = len;
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
xfields (بنية سٽ *سٽ)
{
  حرف *ptr = سٽ->buf.بفر;
  حرف ثابت *lim = ptr + سٽ->buf.ڊيگهائي - 1;

  إذا (ptr == lim)
    أرجع;

  إذا (!tab.len)
    طالما ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        حرف *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        ڪڍو_ميدان (سٽ, ptr, sep - ptr);
        ptr = sep;
      }
  وإلا
    {
      إذا (tab.ch != '\n')
        لكل (حرف *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_جاچ (sep, lim).len)
          ڪڍو_ميدان (سٽ, ptr, sep - ptr);

      ڪڍو_ميدان (سٽ, ptr, lim - ptr);
    }
}

ساكن فراغ
freeline (بنية سٽ *سٽ)
{
  إذا (سٽ == عدم)
    أرجع;
  حرر (سٽ->fields);
  سٽ->fields = عدم;
  سٽ->nfields_allocated = 0;
  حرر (سٽ->buf.بفر);
  سٽ->buf.بفر = عدم;
  سٽ->buf.سائيز = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

ساكن صحيح
keycmp (بنية سٽ ثابت *line1, بنية سٽ ثابت *line2,
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
check_ترتيب (ثابت بنية سٽ *prev,
             ثابت بنية سٽ *current,
             صحيح whatfile)
{
  إذا (check_داخلا_ترتيب != CHECK_ترتيب_DISABLED
      && ((check_داخلا_ترتيب == CHECK_ترتيب_ENABLED) || ڏٺل_unpairable))
    {
      إذا (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_ميدان = whatfile == 1 ? join_ميدان_1 : join_ميدان_2;
          إذا (keycmp (prev, current, join_ميدان, join_ميدان) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.ڊيگهائي;
              إذا (0 < len && current->buf.بفر[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (وڌ_کان_وڌ_عدد, len);

              error ((check_داخلا_ترتيب == CHECK_ترتيب_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_نالا[whatfile - 1], سٽ_no[whatfile - 1],
                     (صحيح) len, current->buf.بفر);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

ساكن مضمن فراغ
reset_سٽ (بنية سٽ *سٽ)
{
  سٽ->nfields = 0;
}

ساكن بنية سٽ *
init_linep (بنية سٽ **linep)
{
  بنية سٽ *سٽ = xzalloc (حجم *سٽ);
  *linep = سٽ;
  أرجع سٽ;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

ساكن bool
get_سٽ (FILE *fp, بنية سٽ **linep, صحيح which)
{
  بنية سٽ *سٽ = *linep;

  إذا (سٽ == prevline[which - 1])
    {
      SWAPLINES (سٽ, spareline[which - 1]);
      *linep = سٽ;
    }

  إذا (سٽ)
    reset_سٽ (سٽ);
  وإلا
    سٽ = init_linep (linep);

  إذا (! readlinebuffer_delim (&سٽ->buf, fp, eolchar))
    {
      إذا (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (سٽ);
      أرجع false;
    }
  ++سٽ_no[which - 1];

  xfields (سٽ);

  إذا (prevline[which - 1])
    check_ترتيب (prevline[which - 1], سٽ, which);

  prevline[which - 1] = سٽ;
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
  seq->ڳڻپ = 0;
  seq->alloc = 0;
  seq->سٽون = عدم;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

ساكن bool
getseq (FILE *fp, بنية seq *seq, صحيح whichfile)
{
  إذا (seq->ڳڻپ == seq->alloc)
    {
      seq->سٽون = xpalloc (seq->سٽون, &seq->alloc, 1, -1, حجم *seq->سٽون);
      لكل (idx_t i = seq->ڳڻپ; i < seq->alloc; i++)
        seq->سٽون[i] = عدم;
    }

  إذا (get_سٽ (fp, &seq->سٽون[seq->ڳڻپ], whichfile))
    {
      ++seq->ڳڻپ;
      أرجع true;
    }
  أرجع false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
ساكن bool
advance_seq (FILE *fp, بنية seq *seq, bool پهريون, صحيح whichfile)
{
  إذا (پهريون)
    seq->ڳڻپ = 0;

  أرجع getseq (fp, seq, whichfile);
}

ساكن فراغ
delseq (بنية seq *seq)
{
  لكل (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->سٽون[i]);
      حرر (seq->سٽون[i]);
    }
  حرر (seq->سٽون);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

ساكن فراغ
prfield (idx_t n, بنية سٽ ثابت *سٽ)
{
  إذا (n < سٽ->nfields)
    {
      idx_t len = سٽ->fields[n].len;
      إذا (len)
        fwrite (سٽ->fields[n].beg, 1, len, stdout);
      وإلا إذا (خالي_جايون_filler)
        fputs (خالي_جايون_filler, stdout);
    }
  وإلا إذا (خالي_جايون_filler)
    fputs (خالي_جايون_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

ساكن فراغ
prfields (بنية سٽ ثابت *سٽ, idx_t join_ميدان, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : سٽ->nfields;

  لكل (idx_t i = 0; i < join_ميدان && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, سٽ);
    }
  لكل (idx_t i = join_ميدان + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, سٽ);
    }
}

/* Print the join of LINE1 and LINE2.  */

ساكن فراغ
prjoin (بنية سٽ ثابت *line1, بنية سٽ ثابت *line2)
{
  ثابت بنية outlist *outlist;
  idx_t ميدان;
  بنية سٽ ثابت *سٽ;

  outlist = outlist_سر.ايندڙ;
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
                  سٽ = line2;
                  ميدان = join_ميدان_2;
                }
              وإلا
                {
                  سٽ = line1;
                  ميدان = join_ميدان_1;
                }
            }
          وإلا
            {
              سٽ = (o->file == 1 ? line1 : line2);
              ميدان = o->ميدان;
            }
          prfield (ميدان, سٽ);
          o = o->ايندڙ;
          إذا (o == عدم)
            اكسر;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      اکر_لکو (eolchar);
    }
  وإلا
    {
      إذا (line1 == &uni_blank)
        {
          سٽ = line2;
          ميدان = join_ميدان_2;
        }
      وإلا
        {
          سٽ = line1;
          ميدان = join_ميدان_1;
        }

      /* Output the join field.  */
      prfield (ميدان, سٽ);

      /* Output other fields.  */
      prfields (line1, join_ميدان_1, autocount_1);
      prfields (line2, join_ميدان_2, autocount_2);

      اکر_لکو (eolchar);
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
      autocount_1 = seq1.ڳڻپ ? seq1.سٽون[0]->nfields : 0;
      autocount_2 = seq2.ڳڻپ ? seq2.سٽون[0]->nfields : 0;
    }

  إذا (join_سرو_سٽون && (seq1.ڳڻپ || seq2.ڳڻپ))
    {
      بنية سٽ ثابت *hline1 = seq1.ڳڻپ ? seq1.سٽون[0] : &uni_blank;
      بنية سٽ ثابت *hline2 = seq2.ڳڻپ ? seq2.سٽون[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = عدم;
      prevline[1] = عدم;
      إذا (seq1.ڳڻپ)
        advance_seq (fp1, &seq1, true, 1);
      إذا (seq2.ڳڻپ)
        advance_seq (fp2, &seq2, true, 2);
    }

  طالما (seq1.ڳڻپ && seq2.ڳڻپ)
    {
      diff = keycmp (seq1.سٽون[0], seq2.سٽون[0],
                     join_ميدان_1, join_ميدان_2);
      إذا (diff < 0)
        {
          إذا (print_unpairables_1)
            prjoin (seq1.سٽون[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          ڏٺل_unpairable = true;
          استمر;
        }
      إذا (diff > 0)
        {
          إذا (print_unpairables_2)
            prjoin (&uni_blank, seq2.سٽون[0]);
          advance_seq (fp2, &seq2, true, 2);
          ڏٺل_unpairable = true;
          استمر;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      افعل
        إذا (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.ڳڻپ;
            اكسر;
          }
      طالما (!keycmp (seq1.سٽون[seq1.ڳڻپ - 1], seq2.سٽون[0],
                      join_ميدان_1, join_ميدان_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      افعل
        إذا (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.ڳڻپ;
            اكسر;
          }
      طالما (!keycmp (seq1.سٽون[0], seq2.سٽون[seq2.ڳڻپ - 1],
                      join_ميدان_1, join_ميدان_2));

      إذا (print_pairables)
        {
          لكل (idx_t i = 0; i < seq1.ڳڻپ - 1; ++i)
            {
              لكل (idx_t j = 0; j < seq2.ڳڻپ - 1; ++j)
                prjoin (seq1.سٽون[i], seq2.سٽون[j]);
            }
        }

      إذا (!eof1)
        {
          SWAPLINES (seq1.سٽون[0], seq1.سٽون[seq1.ڳڻپ - 1]);
          seq1.ڳڻپ = 1;
        }
      وإلا
        seq1.ڳڻپ = 0;

      إذا (!eof2)
        {
          SWAPLINES (seq2.سٽون[0], seq2.سٽون[seq2.ڳڻپ - 1]);
          seq2.ڳڻپ = 1;
        }
      وإلا
        seq2.ڳڻپ = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  بنية سٽ *سٽ = عدم;
  bool checktail = false;

  إذا (check_داخلا_ترتيب != CHECK_ترتيب_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  إذا ((print_unpairables_1 || checktail) && seq1.ڳڻپ)
    {
      إذا (print_unpairables_1)
        prjoin (seq1.سٽون[0], &uni_blank);
      إذا (seq2.ڳڻپ)
        ڏٺل_unpairable = true;
      طالما (get_سٽ (fp1, &سٽ, 1))
        {
          إذا (print_unpairables_1)
            prjoin (سٽ, &uni_blank);
          إذا (issued_disorder_warning[0] && !print_unpairables_1)
            اكسر;
        }
    }

  إذا ((print_unpairables_2 || checktail) && seq2.ڳڻپ)
    {
      إذا (print_unpairables_2)
        prjoin (&uni_blank, seq2.سٽون[0]);
      إذا (seq1.ڳڻپ)
        ڏٺل_unpairable = true;
      طالما (get_سٽ (fp2, &سٽ, 2))
        {
          إذا (print_unpairables_2)
            prjoin (&uni_blank, سٽ);
          إذا (issued_disorder_warning[1] && !print_unpairables_2)
            اكسر;
        }
    }

  freeline (سٽ);
  حرر (سٽ);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

ساكن فراغ
جوڙيو_ميدان (صحيح file, idx_t ميدان)
{
  بنية outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || ميدان == 0);

  o = xmalloc (حجم *o);
  o->file = file;
  o->ميدان = ميدان;
  o->ايندڙ = عدم;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_پڇاڙي->ايندڙ = o;
  outlist_پڇاڙي = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

ساكن idx_t
string_منزل_join_ميدان (حرف ثابت *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, عدم, 10, &val, "");
  إذا (s_err == LONGINT_حد_کان_مٿي || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  وإلا إذا (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  أرجع val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

ساكن فراغ
decode_ميدان_spec (حرف ثابت *s, صحيح *file_اشاريو, idx_t *ميدان_اشاريو)
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
      *file_اشاريو = 0;
      *ميدان_اشاريو = 0;
      اكسر;

    حالة '1':
    حالة '2':
      إذا (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_اشاريو = s[0] - '0';
      *ميدان_اشاريو = string_منزل_join_ميدان (s + 2);
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
جوڙيو_ميدان_list (حرف *str)
{
  حرف *p = str;

  افعل
    {
      صحيح file_اشاريو;
      idx_t ميدان_اشاريو;
      حرف ثابت *spec_شي = p;
      p = skip_str_matching (spec_شي, comma_or_blank, false);
      إذا (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_ميدان_spec (spec_شي, &file_اشاريو, &ميدان_اشاريو);
      جوڙيو_ميدان (file_اشاريو, ميدان_اشاريو);
    }
  طالما (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

ساكن فراغ
سيٽ_join_ميدان (ptrdiff_t *var, idx_t val)
{
  إذا (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

تعداد عمليو_حالت
  {
    /* This argument must be an operand, i.e., one of the files to be
       joined.  */
    MUST_BE_عمليو,

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
جوڙيو_file_name (حرف *name, حرف *نالا[2],
               صحيح عمليو_حالت[2], صحيح joption_ڳڻپ[2], صحيح *nfiles,
               صحيح *prev_optc_حالت, صحيح *optc_حالت)
{
  صحيح n = *nfiles;

  إذا (n == 2)
    {
      bool op0 = (عمليو_حالت[0] == MUST_BE_عمليو);
      حرف *arg = نالا[op0];
      بدل (عمليو_حالت[op0])
        {
        حالة MUST_BE_عمليو:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        حالة MIGHT_BE_J1_ARG:
          joption_ڳڻپ[0]--;
          سيٽ_join_ميدان (&join_ميدان_1, string_منزل_join_ميدان (arg));
          اكسر;

        حالة MIGHT_BE_J2_ARG:
          joption_ڳڻپ[1]--;
          سيٽ_join_ميدان (&join_ميدان_2, string_منزل_join_ميدان (arg));
          اكسر;

        حالة MIGHT_BE_O_ARG:
          جوڙيو_ميدان_list (arg);
          اكسر;
        }
      إذا (!op0)
        {
          عمليو_حالت[0] = عمليو_حالت[1];
          نالا[0] = نالا[1];
        }
      n = 1;
    }

  عمليو_حالت[n] = *prev_optc_حالت;
  نالا[n] = name;
  *nfiles = n + 1;
  إذا (*prev_optc_حالت == MIGHT_BE_O_ARG)
    *optc_حالت = MIGHT_BE_O_ARG;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح optc_حالت;
  صحيح prev_optc_حالت = MUST_BE_عمليو;
  صحيح عمليو_حالت[2];
  صحيح joption_ڳڻپ[2] = { 0, 0 };
  FILE *fp1, *fp2;
  صحيح optc;
  صحيح nfiles = 0;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
      optc_حالت = MUST_BE_عمليو;

      بدل (optc)
        {
        حالة 'v':
            print_pairables = false;
            FALLTHROUGH;

        حالة 'a':
          {
            ڊگهو صحيح val;
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
          إذا (خالي_جايون_filler && ! streq (خالي_جايون_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          خالي_جايون_filler = optarg;
          اكسر;

        حالة 'i':
          ignore_case = true;
          اكسر;

        حالة '1':
          سيٽ_join_ميدان (&join_ميدان_1, string_منزل_join_ميدان (optarg));
          اكسر;

        حالة '2':
          سيٽ_join_ميدان (&join_ميدان_2, string_منزل_join_ميدان (optarg));
          اكسر;

        حالة 'j':
          إذا ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_ڳڻپ[is_j2]++;
              optc_حالت = MIGHT_BE_J1_ARG + is_j2;
            }
          وإلا
            {
              سيٽ_join_ميدان (&join_ميدان_1, string_منزل_join_ميدان (optarg));
              سيٽ_join_ميدان (&join_ميدان_2, join_ميدان_1);
            }
          اكسر;

        حالة 'o':
          إذا (streq (optarg, "auto"))
            autoformat = true;
          وإلا
            {
              جوڙيو_ميدان_list (optarg);
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

        حالة NOCHECK_ترتيب_OPTION:
          check_داخلا_ترتيب = CHECK_ترتيب_DISABLED;
          اكسر;

        حالة CHECK_ترتيب_OPTION:
          check_داخلا_ترتيب = CHECK_ترتيب_ENABLED;
          اكسر;

        حالة 1:		/* Non-option argument.  */
          جوڙيو_file_name (optarg, g_نالا, عمليو_حالت, joption_ڳڻپ,
                         &nfiles, &prev_optc_حالت, &optc_حالت);
          اكسر;

        حالة سرو_سٽ_OPTION:
          join_سرو_سٽون = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }

      prev_optc_حالت = optc_حالت;
    }

  /* Process any operands after "--".  */
  prev_optc_حالت = MUST_BE_عمليو;
  طالما (optind < argc)
    جوڙيو_file_name (argv[optind++], g_نالا, عمليو_حالت, joption_ڳڻپ,
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
    إذا (joption_ڳڻپ[i] != 0)
      {
        سيٽ_join_ميدان (&join_ميدان_1, i);
        سيٽ_join_ميدان (&join_ميدان_2, i);
      }

  إذا (join_ميدان_1 < 0)
    join_ميدان_1 = 0;
  إذا (join_ميدان_2 < 0)
    join_ميدان_2 = 0;

  fp1 = streq (g_نالا[0], "-") ? stdin : fopen (g_نالا[0], "r");
  إذا (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نالا[0]));
  fp2 = streq (g_نالا[1], "-") ? stdin : fopen (g_نالا[1], "r");
  إذا (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نالا[1]));
  إذا (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  إذا (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نالا[0]));
  إذا (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_نالا[1]));

  إذا (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  وإلا
    أرجع EXIT_SUCCESS;
}
