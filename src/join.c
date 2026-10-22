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

#అన్తర్భావ <config.h>

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "hard-locale.h"
#అన్తర్భావ "linebuffer.h"
#అన్తర్భావ "mcel.h"
#అన్తర్భావ "memcasecmp.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "skipchars.h"
#అన్తర్భావ "stdio--.h"
#అన్తర్భావ "xmemcoll.h"
#అన్తర్భావ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "join"

#నిర్వచించు AUTHORS proper_name ("Mike Haertel")

#నిర్వచించు join system_join

#నిర్వచించు SWAPLINES(a, b) చేయి { \
  నిర్మాణం పంక్తి *tmp = a; \
  a = b; \
  b = tmp; \
} అయితే (0);

/* An element of the list identifying which fields to print for each
   output line.  */
నిర్మాణం outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    పూర్ణాంకం file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t క్షేత్రం;

    నిర్మాణం outlist *తదుపరి;
  };

/* A field of a line.  */
నిర్మాణం క్షేత్రం
  {
    అక్షరం *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
నిర్మాణం పంక్తి
  {
    నిర్మాణం linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    నిర్మాణం క్షేత్రం *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
నిర్మాణం seq
  {
    idx_t లెక్క;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    నిర్మాణం పంక్తి **పంక్తులు COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
స్థిర నిర్మాణం పంక్తి *prevline[2] = {ఖాళీ, ఖాళీ};

/* The number of lines read from each file.  */
స్థిర uintmax_t పంక్తి_no[2] = {0, 0};

/* The input file names.  */
స్థిర అక్షరం *g_పేర్లు[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
స్థిర నిర్మాణం పంక్తి *spareline[2] = {ఖాళీ, ఖాళీ};

/* True if the LC_COLLATE locale is hard.  */
స్థిర bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
స్థిర bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
స్థిర bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
స్థిర bool చూసిన_unpairable;

/* If nonzero, we have warned about disorder in that file. */
స్థిర bool issued_disorder_warning[2];

/* Empty output field filler.  */
స్థిర అక్షరం స్థిరరాశి *ఖాళీలు_filler;

/* Whether to ensure the same number of fields are output from each line.  */
స్థిర bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
స్థిర idx_t autocount_1;
స్థిర idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
స్థిర ptrdiff_t join_క్షేత్రం_1 = -1;
స్థిర ptrdiff_t join_క్షేత్రం_2 = -1;

/* List of fields to print.  */
స్థిర నిర్మాణం outlist outlist_తల;

/* Last element in 'outlist', where a new element can be added.  */
స్థిర నిర్మాణం outlist *outlist_చివర = &outlist_తల;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
స్థిర mcel_t tab;

/* The output separator to use, and its length in bytes.  */
స్థిర అక్షరం స్థిరరాశి *output_separator = " ";
స్థిర idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
స్థిర గణన
  {
    CHECK_క్రమం_DEFAULT,
    CHECK_క్రమం_ENABLED,
    CHECK_క్రమం_DISABLED
  } check_ఆదానం_క్రమం;

గణన
{
  CHECK_క్రమం_OPTION = CHAR_MAX + 1,
  NOCHECK_క్రమం_OPTION,
  శీర్షిక_పంక్తి_OPTION
};


స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"ignore-case", no_వాదం, ఖాళీ, 'i'},
  {"check-order", no_వాదం, ఖాళీ, CHECK_క్రమం_OPTION},
  {"nocheck-order", no_వాదం, ఖాళీ, NOCHECK_క్రమం_OPTION},
  {"zero-terminated", no_వాదం, ఖాళీ, 'z'},
  {"header", no_వాదం, ఖాళీ, శీర్షిక_పంక్తి_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Used to print non-joining lines */
స్థిర నిర్మాణం పంక్తి uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
స్థిర bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
స్థిర bool join_శీర్షిక_పంక్తులు;

/* The character marking end of line. Default to \n. */
స్థిర అక్షరం eolchar = '\n';

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
For each pair యొక్క ఆదానం పంక్తులు with identical join fields, లిఖ a పంక్తి గమ్యం\n\
standard output.  The అప్రమేయ join క్షేత్రం is the మొదటి, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, పఠ standard ఆదానం.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable పంక్తులు from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding గమ్యం FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (ఖాళీలు) ఆదానం fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-స్థితి\n\
         ignore differences లోపల స్థితి when comparing fields\n\
"));
      oputs (_("\
  -j క్షేత్రం_2\n\
         equivalent గమ్యం '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT అయితే constructing output పంక్తి\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as ఆదానం and output క్షేత్రం separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output పంక్తులు\n\
"));
      oputs (_("\
  -1 క్షేత్రం_2\n\
         join on this క్షేత్రం_2 యొక్క file 1\n\
"));
      oputs (_("\
  -2 క్షేత్రం_2\n\
         join on this క్షేత్రం_2 యొక్క file 2\n\
"));
      oputs (_("\
      --check-క్రమం\n\
         check that the ఆదానం is correctly sorted, even\n\
         ఒకవేళ all ఆదానం పంక్తులు are pairable\n\
"));
      oputs (_("\
      --nocheck-క్రమం\n\
         చేయి not check that the ఆదానం is correctly sorted\n\
"));
      oputs (_("\
      --శీర్షిక\n\
         treat the మొదటి పంక్తి లోపల each file as క్షేత్రం శీర్షికలు,\n\
         print them without trying గమ్యం pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         పంక్తి delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
లేకపోతే fields are separated by CHAR.  Any క్షేత్రం_2 is a క్షేత్రం సంఖ్య counted\n\
from 1.  FORMAT is one or మరిన్ని comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join క్షేత్రం,\n\
the మిగిలిన fields from FILE1, the మిగిలిన fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the మొదటి\n\
పంక్తి యొక్క each file determines the సంఖ్య యొక్క fields output కొరకు each పంక్తి.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"క్రమీకరించు -k 1b,1\" ఒకవేళ 'join' has no options,\n\
or use \"join -t ''\" ఒకవేళ 'sort' has no options.\n\
Comparisons honor the నియమాలు specified by 'LC_COLLATE'.\n\
If the ఆదానం is not sorted and some పంక్తులు cannot be joined, a\n\
warning సందేశం_2 will be given.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

స్థిర శూన్య
వెలికితీయి_క్షేత్రం (నిర్మాణం పంక్తి *పంక్తి, అక్షరం *క్షేత్రం, idx_t len)
{
  ఒకవేళ (పంక్తి->nfields >= పంక్తి->nfields_allocated)
    పంక్తి->fields = xpalloc (పంక్తి->fields, &పంక్తి->nfields_allocated, 1,
                            -1, పరిమాణం *పంక్తి->fields);
  idx_t స్థిరరాశి క్షేత్రం_సూచీ = పంక్తి->nfields++;
  పంక్తి->fields[క్షేత్రం_సూచీ].beg = క్షేత్రం;
  పంక్తి->fields[క్షేత్రం_సూచీ].len = len;
}

స్థిర bool
eq_tab (mcel_t g)
{
  తిరిగి mcel_eq (g, tab);
}

స్థిర bool
newline_or_blank (mcel_t g)
{
  తిరిగి g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

స్థిర శూన్య
xfields (నిర్మాణం పంక్తి *పంక్తి)
{
  అక్షరం *ptr = పంక్తి->buf.బఫరు;
  అక్షరం స్థిరరాశి *lim = ptr + పంక్తి->buf.నిడివి - 1;

  ఒకవేళ (ptr == lim)
    తిరిగి;

  ఒకవేళ (!tab.len)
    అయితే ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        అక్షరం *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        వెలికితీయి_క్షేత్రం (పంక్తి, ptr, sep - ptr);
        ptr = sep;
      }
  లేకపోతే
    {
      ఒకవేళ (tab.ch != '\n')
        కొరకు (అక్షరం *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_పరిశీలన (sep, lim).len)
          వెలికితీయి_క్షేత్రం (పంక్తి, ptr, sep - ptr);

      వెలికితీయి_క్షేత్రం (పంక్తి, ptr, lim - ptr);
    }
}

స్థిర శూన్య
freeline (నిర్మాణం పంక్తి *పంక్తి)
{
  ఒకవేళ (పంక్తి == ఖాళీ)
    తిరిగి;
  విడుదల (పంక్తి->fields);
  పంక్తి->fields = ఖాళీ;
  పంక్తి->nfields_allocated = 0;
  విడుదల (పంక్తి->buf.బఫరు);
  పంక్తి->buf.బఫరు = ఖాళీ;
  పంక్తి->buf.కొలత = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

స్థిర పూర్ణాంకం
keycmp (నిర్మాణం పంక్తి స్థిరరాశి *line1, నిర్మాణం పంక్తి స్థిరరాశి *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  అక్షరం *beg1;
  అక్షరం *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  పూర్ణాంకం diff;

  ఒకవేళ (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  లేకపోతే
    {
      beg1 = ఖాళీ;
      len1 = 0;
    }

  ఒకవేళ (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  లేకపోతే
    {
      beg2 = ఖాళీ;
      len2 = 0;
    }

  ఒకవేళ (len1 == 0)
    తిరిగి len2 == 0 ? 0 : -1;
  ఒకవేళ (len2 == 0)
    తిరిగి 1;

  ఒకవేళ (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  లేకపోతే
    {
      ఒకవేళ (hard_LC_COLLATE)
        తిరిగి xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  ఒకవేళ (diff)
    తిరిగి diff;
  తిరిగి _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

స్థిర శూన్య
check_క్రమం (స్థిరరాశి నిర్మాణం పంక్తి *prev,
             స్థిరరాశి నిర్మాణం పంక్తి *current,
             పూర్ణాంకం whatfile)
{
  ఒకవేళ (check_ఆదానం_క్రమం != CHECK_క్రమం_DISABLED
      && ((check_ఆదానం_క్రమం == CHECK_క్రమం_ENABLED) || చూసిన_unpairable))
    {
      ఒకవేళ (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_క్షేత్రం = whatfile == 1 ? join_క్షేత్రం_1 : join_క్షేత్రం_2;
          ఒకవేళ (keycmp (prev, current, join_క్షేత్రం, join_క్షేత్రం) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.నిడివి;
              ఒకవేళ (0 < len && current->buf.బఫరు[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (గరిష్ఠ_పూర్ణాంకం, len);

              error ((check_ఆదానం_క్రమం == CHECK_క్రమం_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_పేర్లు[whatfile - 1], పంక్తి_no[whatfile - 1],
                     (పూర్ణాంకం) len, current->buf.బఫరు);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

స్థిర అంతఃవరుస శూన్య
reset_పంక్తి (నిర్మాణం పంక్తి *పంక్తి)
{
  పంక్తి->nfields = 0;
}

స్థిర నిర్మాణం పంక్తి *
init_linep (నిర్మాణం పంక్తి **linep)
{
  నిర్మాణం పంక్తి *పంక్తి = xzalloc (పరిమాణం *పంక్తి);
  *linep = పంక్తి;
  తిరిగి పంక్తి;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

స్థిర bool
get_పంక్తి (FILE *fp, నిర్మాణం పంక్తి **linep, పూర్ణాంకం which)
{
  నిర్మాణం పంక్తి *పంక్తి = *linep;

  ఒకవేళ (పంక్తి == prevline[which - 1])
    {
      SWAPLINES (పంక్తి, spareline[which - 1]);
      *linep = పంక్తి;
    }

  ఒకవేళ (పంక్తి)
    reset_పంక్తి (పంక్తి);
  లేకపోతే
    పంక్తి = init_linep (linep);

  ఒకవేళ (! readlinebuffer_delim (&పంక్తి->buf, fp, eolchar))
    {
      ఒకవేళ (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (పంక్తి);
      తిరిగి false;
    }
  ++పంక్తి_no[which - 1];

  xfields (పంక్తి);

  ఒకవేళ (prevline[which - 1])
    check_క్రమం (prevline[which - 1], పంక్తి, which);

  prevline[which - 1] = పంక్తి;
  తిరిగి true;
}

స్థిర శూన్య
free_spareline (శూన్య)
{
  కొరకు (idx_t i = 0; i < countof (spareline); i++)
    {
      ఒకవేళ (spareline[i])
        {
          freeline (spareline[i]);
          విడుదల (spareline[i]);
        }
    }
}

స్థిర శూన్య
initseq (నిర్మాణం seq *seq)
{
  seq->లెక్క = 0;
  seq->alloc = 0;
  seq->పంక్తులు = ఖాళీ;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

స్థిర bool
getseq (FILE *fp, నిర్మాణం seq *seq, పూర్ణాంకం whichfile)
{
  ఒకవేళ (seq->లెక్క == seq->alloc)
    {
      seq->పంక్తులు = xpalloc (seq->పంక్తులు, &seq->alloc, 1, -1, పరిమాణం *seq->పంక్తులు);
      కొరకు (idx_t i = seq->లెక్క; i < seq->alloc; i++)
        seq->పంక్తులు[i] = ఖాళీ;
    }

  ఒకవేళ (get_పంక్తి (fp, &seq->పంక్తులు[seq->లెక్క], whichfile))
    {
      ++seq->లెక్క;
      తిరిగి true;
    }
  తిరిగి false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
స్థిర bool
advance_seq (FILE *fp, నిర్మాణం seq *seq, bool మొదటి, పూర్ణాంకం whichfile)
{
  ఒకవేళ (మొదటి)
    seq->లెక్క = 0;

  తిరిగి getseq (fp, seq, whichfile);
}

స్థిర శూన్య
delseq (నిర్మాణం seq *seq)
{
  కొరకు (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->పంక్తులు[i]);
      విడుదల (seq->పంక్తులు[i]);
    }
  విడుదల (seq->పంక్తులు);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

స్థిర శూన్య
prfield (idx_t n, నిర్మాణం పంక్తి స్థిరరాశి *పంక్తి)
{
  ఒకవేళ (n < పంక్తి->nfields)
    {
      idx_t len = పంక్తి->fields[n].len;
      ఒకవేళ (len)
        fwrite (పంక్తి->fields[n].beg, 1, len, stdout);
      లేకపోతే ఒకవేళ (ఖాళీలు_filler)
        fputs (ఖాళీలు_filler, stdout);
    }
  లేకపోతే ఒకవేళ (ఖాళీలు_filler)
    fputs (ఖాళీలు_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

స్థిర శూన్య
prfields (నిర్మాణం పంక్తి స్థిరరాశి *పంక్తి, idx_t join_క్షేత్రం, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : పంక్తి->nfields;

  కొరకు (idx_t i = 0; i < join_క్షేత్రం && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, పంక్తి);
    }
  కొరకు (idx_t i = join_క్షేత్రం + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, పంక్తి);
    }
}

/* Print the join of LINE1 and LINE2.  */

స్థిర శూన్య
prjoin (నిర్మాణం పంక్తి స్థిరరాశి *line1, నిర్మాణం పంక్తి స్థిరరాశి *line2)
{
  స్థిరరాశి నిర్మాణం outlist *outlist;
  idx_t క్షేత్రం;
  నిర్మాణం పంక్తి స్థిరరాశి *పంక్తి;

  outlist = outlist_తల.తదుపరి;
  ఒకవేళ (outlist)
    {
      స్థిరరాశి నిర్మాణం outlist *o;

      o = outlist;
      అయితే (true)
        {
          ఒకవేళ (o->file == 0)
            {
              ఒకవేళ (line1 == &uni_blank)
                {
                  పంక్తి = line2;
                  క్షేత్రం = join_క్షేత్రం_2;
                }
              లేకపోతే
                {
                  పంక్తి = line1;
                  క్షేత్రం = join_క్షేత్రం_1;
                }
            }
          లేకపోతే
            {
              పంక్తి = (o->file == 1 ? line1 : line2);
              క్షేత్రం = o->క్షేత్రం;
            }
          prfield (క్షేత్రం, పంక్తి);
          o = o->తదుపరి;
          ఒకవేళ (o == ఖాళీ)
            ఆపు;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      అక్షరం_రాయి (eolchar);
    }
  లేకపోతే
    {
      ఒకవేళ (line1 == &uni_blank)
        {
          పంక్తి = line2;
          క్షేత్రం = join_క్షేత్రం_2;
        }
      లేకపోతే
        {
          పంక్తి = line1;
          క్షేత్రం = join_క్షేత్రం_1;
        }

      /* Output the join field.  */
      prfield (క్షేత్రం, పంక్తి);

      /* Output other fields.  */
      prfields (line1, join_క్షేత్రం_1, autocount_1);
      prfields (line2, join_క్షేత్రం_2, autocount_2);

      అక్షరం_రాయి (eolchar);
    }

  ఒకవేళ (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

స్థిర శూన్య
join (FILE *fp1, FILE *fp2)
{
  నిర్మాణం seq seq1, seq2;
  పూర్ణాంకం diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  ఒకవేళ (autoformat)
    {
      autocount_1 = seq1.లెక్క ? seq1.పంక్తులు[0]->nfields : 0;
      autocount_2 = seq2.లెక్క ? seq2.పంక్తులు[0]->nfields : 0;
    }

  ఒకవేళ (join_శీర్షిక_పంక్తులు && (seq1.లెక్క || seq2.లెక్క))
    {
      నిర్మాణం పంక్తి స్థిరరాశి *hline1 = seq1.లెక్క ? seq1.పంక్తులు[0] : &uni_blank;
      నిర్మాణం పంక్తి స్థిరరాశి *hline2 = seq2.లెక్క ? seq2.పంక్తులు[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = ఖాళీ;
      prevline[1] = ఖాళీ;
      ఒకవేళ (seq1.లెక్క)
        advance_seq (fp1, &seq1, true, 1);
      ఒకవేళ (seq2.లెక్క)
        advance_seq (fp2, &seq2, true, 2);
    }

  అయితే (seq1.లెక్క && seq2.లెక్క)
    {
      diff = keycmp (seq1.పంక్తులు[0], seq2.పంక్తులు[0],
                     join_క్షేత్రం_1, join_క్షేత్రం_2);
      ఒకవేళ (diff < 0)
        {
          ఒకవేళ (print_unpairables_1)
            prjoin (seq1.పంక్తులు[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          చూసిన_unpairable = true;
          కొనసాగు;
        }
      ఒకవేళ (diff > 0)
        {
          ఒకవేళ (print_unpairables_2)
            prjoin (&uni_blank, seq2.పంక్తులు[0]);
          advance_seq (fp2, &seq2, true, 2);
          చూసిన_unpairable = true;
          కొనసాగు;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      చేయి
        ఒకవేళ (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.లెక్క;
            ఆపు;
          }
      అయితే (!keycmp (seq1.పంక్తులు[seq1.లెక్క - 1], seq2.పంక్తులు[0],
                      join_క్షేత్రం_1, join_క్షేత్రం_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      చేయి
        ఒకవేళ (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.లెక్క;
            ఆపు;
          }
      అయితే (!keycmp (seq1.పంక్తులు[0], seq2.పంక్తులు[seq2.లెక్క - 1],
                      join_క్షేత్రం_1, join_క్షేత్రం_2));

      ఒకవేళ (print_pairables)
        {
          కొరకు (idx_t i = 0; i < seq1.లెక్క - 1; ++i)
            {
              కొరకు (idx_t j = 0; j < seq2.లెక్క - 1; ++j)
                prjoin (seq1.పంక్తులు[i], seq2.పంక్తులు[j]);
            }
        }

      ఒకవేళ (!eof1)
        {
          SWAPLINES (seq1.పంక్తులు[0], seq1.పంక్తులు[seq1.లెక్క - 1]);
          seq1.లెక్క = 1;
        }
      లేకపోతే
        seq1.లెక్క = 0;

      ఒకవేళ (!eof2)
        {
          SWAPLINES (seq2.పంక్తులు[0], seq2.పంక్తులు[seq2.లెక్క - 1]);
          seq2.లెక్క = 1;
        }
      లేకపోతే
        seq2.లెక్క = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  నిర్మాణం పంక్తి *పంక్తి = ఖాళీ;
  bool checktail = false;

  ఒకవేళ (check_ఆదానం_క్రమం != CHECK_క్రమం_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  ఒకవేళ ((print_unpairables_1 || checktail) && seq1.లెక్క)
    {
      ఒకవేళ (print_unpairables_1)
        prjoin (seq1.పంక్తులు[0], &uni_blank);
      ఒకవేళ (seq2.లెక్క)
        చూసిన_unpairable = true;
      అయితే (get_పంక్తి (fp1, &పంక్తి, 1))
        {
          ఒకవేళ (print_unpairables_1)
            prjoin (పంక్తి, &uni_blank);
          ఒకవేళ (issued_disorder_warning[0] && !print_unpairables_1)
            ఆపు;
        }
    }

  ఒకవేళ ((print_unpairables_2 || checktail) && seq2.లెక్క)
    {
      ఒకవేళ (print_unpairables_2)
        prjoin (&uni_blank, seq2.పంక్తులు[0]);
      ఒకవేళ (seq1.లెక్క)
        చూసిన_unpairable = true;
      అయితే (get_పంక్తి (fp2, &పంక్తి, 2))
        {
          ఒకవేళ (print_unpairables_2)
            prjoin (&uni_blank, పంక్తి);
          ఒకవేళ (issued_disorder_warning[1] && !print_unpairables_2)
            ఆపు;
        }
    }

  freeline (పంక్తి);
  విడుదల (పంక్తి);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

స్థిర శూన్య
కూడు_క్షేత్రం (పూర్ణాంకం file, idx_t క్షేత్రం)
{
  నిర్మాణం outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || క్షేత్రం == 0);

  o = xmalloc (పరిమాణం *o);
  o->file = file;
  o->క్షేత్రం = క్షేత్రం;
  o->తదుపరి = ఖాళీ;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_చివర->తదుపరి = o;
  outlist_చివర = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

స్థిర idx_t
string_గమ్యం_join_క్షేత్రం (అక్షరం స్థిరరాశి *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, ఖాళీ, 10, &val, "");
  ఒకవేళ (s_err == LONGINT_అధికప్రవాహం || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  లేకపోతే ఒకవేళ (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  తిరిగి val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

స్థిర శూన్య
decode_క్షేత్రం_spec (అక్షరం స్థిరరాశి *s, పూర్ణాంకం *file_సూచీ, idx_t *క్షేత్రం_సూచీ)
{
  /* The first character must be 0, 1, or 2.  */
  ఎంపిక (s[0])
    {
    స్థితి '0':
      ఒకవేళ (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_సూచీ = 0;
      *క్షేత్రం_సూచీ = 0;
      ఆపు;

    స్థితి '1':
    స్థితి '2':
      ఒకవేళ (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_సూచీ = s[0] - '0';
      *క్షేత్రం_సూచీ = string_గమ్యం_join_క్షేత్రం (s + 2);
      ఆపు;

    అప్రమేయ:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

స్థిర bool
comma_or_blank (mcel_t g)
{
  తిరిగి g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

స్థిర శూన్య
కూడు_క్షేత్రం_list (అక్షరం *str)
{
  అక్షరం *p = str;

  చేయి
    {
      పూర్ణాంకం file_సూచీ;
      idx_t క్షేత్రం_సూచీ;
      అక్షరం స్థిరరాశి *spec_అంశం = p;
      p = skip_str_matching (spec_అంశం, comma_or_blank, false);
      ఒకవేళ (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_క్షేత్రం_spec (spec_అంశం, &file_సూచీ, &క్షేత్రం_సూచీ);
      కూడు_క్షేత్రం (file_సూచీ, క్షేత్రం_సూచీ);
    }
  అయితే (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

స్థిర శూన్య
సమితి_join_క్షేత్రం (ptrdiff_t *var, idx_t val)
{
  ఒకవేళ (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

గణన నిర్వాహ్యం_స్థితి
  {
    /* This argument must be an operand, i.e., one of the files to be
       joined.  */
    MUST_BE_నిర్వాహ్యం,

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

స్థిర శూన్య
కూడు_file_name (అక్షరం *name, అక్షరం *పేర్లు[2],
               పూర్ణాంకం నిర్వాహ్యం_స్థితి[2], పూర్ణాంకం joption_లెక్క[2], పూర్ణాంకం *nfiles,
               పూర్ణాంకం *prev_optc_స్థితి, పూర్ణాంకం *optc_స్థితి)
{
  పూర్ణాంకం n = *nfiles;

  ఒకవేళ (n == 2)
    {
      bool op0 = (నిర్వాహ్యం_స్థితి[0] == MUST_BE_నిర్వాహ్యం);
      అక్షరం *arg = పేర్లు[op0];
      ఎంపిక (నిర్వాహ్యం_స్థితి[op0])
        {
        స్థితి MUST_BE_నిర్వాహ్యం:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        స్థితి MIGHT_BE_J1_ARG:
          joption_లెక్క[0]--;
          సమితి_join_క్షేత్రం (&join_క్షేత్రం_1, string_గమ్యం_join_క్షేత్రం (arg));
          ఆపు;

        స్థితి MIGHT_BE_J2_ARG:
          joption_లెక్క[1]--;
          సమితి_join_క్షేత్రం (&join_క్షేత్రం_2, string_గమ్యం_join_క్షేత్రం (arg));
          ఆపు;

        స్థితి MIGHT_BE_O_ARG:
          కూడు_క్షేత్రం_list (arg);
          ఆపు;
        }
      ఒకవేళ (!op0)
        {
          నిర్వాహ్యం_స్థితి[0] = నిర్వాహ్యం_స్థితి[1];
          పేర్లు[0] = పేర్లు[1];
        }
      n = 1;
    }

  నిర్వాహ్యం_స్థితి[n] = *prev_optc_స్థితి;
  పేర్లు[n] = name;
  *nfiles = n + 1;
  ఒకవేళ (*prev_optc_స్థితి == MIGHT_BE_O_ARG)
    *optc_స్థితి = MIGHT_BE_O_ARG;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optc_స్థితి;
  పూర్ణాంకం prev_optc_స్థితి = MUST_BE_నిర్వాహ్యం;
  పూర్ణాంకం నిర్వాహ్యం_స్థితి[2];
  పూర్ణాంకం joption_లెక్క[2] = { 0, 0 };
  FILE *fp1, *fp2;
  పూర్ణాంకం optc;
  పూర్ణాంకం nfiles = 0;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  అయితే ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, ఖాళీ))
         != -1)
    {
      optc_స్థితి = MUST_BE_నిర్వాహ్యం;

      ఎంపిక (optc)
        {
        స్థితి 'v':
            print_pairables = false;
            FALLTHROUGH;

        స్థితి 'a':
          {
            దీర్ఘ పూర్ణాంకం val;
            ఒకవేళ (xstrtol (optarg, ఖాళీ, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            ఒకవేళ (val == 1)
              print_unpairables_1 = true;
            లేకపోతే
              print_unpairables_2 = true;
          }
          ఆపు;

        స్థితి 'e':
          ఒకవేళ (ఖాళీలు_filler && ! streq (ఖాళీలు_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          ఖాళీలు_filler = optarg;
          ఆపు;

        స్థితి 'i':
          ignore_case = true;
          ఆపు;

        స్థితి '1':
          సమితి_join_క్షేత్రం (&join_క్షేత్రం_1, string_గమ్యం_join_క్షేత్రం (optarg));
          ఆపు;

        స్థితి '2':
          సమితి_join_క్షేత్రం (&join_క్షేత్రం_2, string_గమ్యం_join_క్షేత్రం (optarg));
          ఆపు;

        స్థితి 'j':
          ఒకవేళ ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_లెక్క[is_j2]++;
              optc_స్థితి = MIGHT_BE_J1_ARG + is_j2;
            }
          లేకపోతే
            {
              సమితి_join_క్షేత్రం (&join_క్షేత్రం_1, string_గమ్యం_join_క్షేత్రం (optarg));
              సమితి_join_క్షేత్రం (&join_క్షేత్రం_2, join_క్షేత్రం_1);
            }
          ఆపు;

        స్థితి 'o':
          ఒకవేళ (streq (optarg, "auto"))
            autoformat = true;
          లేకపోతే
            {
              కూడు_క్షేత్రం_list (optarg);
              optc_స్థితి = MIGHT_BE_O_ARG;
            }
          ఆపు;

        స్థితి 't':
          {
            mcel_t newtab;
            ఒకవేళ (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            లేకపోతే ఒకవేళ (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            లేకపోతే
              {
                newtab = mcel_scanz (optarg);
                ఒకవేళ (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            ఒకవేళ (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          ఆపు;

        స్థితి 'z':
          eolchar = 0;
          ఆపు;

        స్థితి NOCHECK_క్రమం_OPTION:
          check_ఆదానం_క్రమం = CHECK_క్రమం_DISABLED;
          ఆపు;

        స్థితి CHECK_క్రమం_OPTION:
          check_ఆదానం_క్రమం = CHECK_క్రమం_ENABLED;
          ఆపు;

        స్థితి 1:		/* Non-option argument.  */
          కూడు_file_name (optarg, g_పేర్లు, నిర్వాహ్యం_స్థితి, joption_లెక్క,
                         &nfiles, &prev_optc_స్థితి, &optc_స్థితి);
          ఆపు;

        స్థితి శీర్షిక_పంక్తి_OPTION:
          join_శీర్షిక_పంక్తులు = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }

      prev_optc_స్థితి = optc_స్థితి;
    }

  /* Process any operands after "--".  */
  prev_optc_స్థితి = MUST_BE_నిర్వాహ్యం;
  అయితే (optind < argc)
    కూడు_file_name (argv[optind++], g_పేర్లు, నిర్వాహ్యం_స్థితి, joption_లెక్క,
                   &nfiles, &prev_optc_స్థితి, &optc_స్థితి);

  ఒకవేళ (nfiles != 2)
    {
      ఒకవేళ (nfiles == 0)
        error (0, 0, _("missing operand"));
      లేకపోతే
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  కొరకు (పూర్ణాంకం i = 0; i < 2; i++)
    ఒకవేళ (joption_లెక్క[i] != 0)
      {
        సమితి_join_క్షేత్రం (&join_క్షేత్రం_1, i);
        సమితి_join_క్షేత్రం (&join_క్షేత్రం_2, i);
      }

  ఒకవేళ (join_క్షేత్రం_1 < 0)
    join_క్షేత్రం_1 = 0;
  ఒకవేళ (join_క్షేత్రం_2 < 0)
    join_క్షేత్రం_2 = 0;

  fp1 = streq (g_పేర్లు[0], "-") ? stdin : fopen (g_పేర్లు[0], "r");
  ఒకవేళ (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_పేర్లు[0]));
  fp2 = streq (g_పేర్లు[1], "-") ? stdin : fopen (g_పేర్లు[1], "r");
  ఒకవేళ (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_పేర్లు[1]));
  ఒకవేళ (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  ఒకవేళ (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_పేర్లు[0]));
  ఒకవేళ (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_పేర్లు[1]));

  ఒకవేళ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  లేకపోతే
    తిరిగి EXIT_SUCCESS;
}
