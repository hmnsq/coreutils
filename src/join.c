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

#अन्तर्भाव <config.h>

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "hard-locale.h"
#अन्तर्भाव "linebuffer.h"
#अन्तर्भाव "mcel.h"
#अन्तर्भाव "memcasecmp.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "skipchars.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "xmemcoll.h"
#अन्तर्भाव "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "join"

#define AUTHORS proper_name ("Mike Haertel")

#define join system_join

#define SWAPLINES(a, b) करा { \
  संरचना ओळ *tmp = a; \
  a = b; \
  b = tmp; \
} पर्यंत (0);

/* An element of the list identifying which fields to print for each
   output line.  */
संरचना outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    पूर्णांक file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t field;

    संरचना outlist *next;
  };

/* A field of a line.  */
संरचना field
  {
    अक्षर *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
संरचना ओळ
  {
    संरचना linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    संरचना field *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
संरचना seq
  {
    idx_t मोजणी;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    संरचना ओळ **ओळी COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
स्थिर संरचना ओळ *prevline[2] = {रिक्त, रिक्त};

/* The number of lines read from each file.  */
स्थिर uintmax_t ओळ_no[2] = {0, 0};

/* The input file names.  */
स्थिर अक्षर *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
स्थिर संरचना ओळ *spareline[2] = {रिक्त, रिक्त};

/* True if the LC_COLLATE locale is hard.  */
स्थिर bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
स्थिर bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
स्थिर bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
स्थिर bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
स्थिर bool issued_disorder_warning[2];

/* Empty output field filler.  */
स्थिर अक्षर स्थिरांक *empty_filler;

/* Whether to ensure the same number of fields are output from each line.  */
स्थिर bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
स्थिर idx_t autocount_1;
स्थिर idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
स्थिर सूचक_भेद_प्रकार join_field_1 = -1;
स्थिर सूचक_भेद_प्रकार join_field_2 = -1;

/* List of fields to print.  */
स्थिर संरचना outlist outlist_head;

/* Last element in 'outlist', where a new element can be added.  */
स्थिर संरचना outlist *outlist_end = &outlist_head;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
स्थिर mcel_t tab;

/* The output separator to use, and its length in bytes.  */
स्थिर अक्षर स्थिरांक *output_separator = " ";
स्थिर idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
स्थिर गणना
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_आदान_order;

गणना
{
  CHECK_ORDER_OPTION = अक्षर_उच्च + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_ओळ_OPTION
};


स्थिर संरचना option स्थिरांक longopts[] =
{
  {"ignore-case", no_argument, रिक्त, 'i'},
  {"check-order", no_argument, रिक्त, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, रिक्त, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {"header", no_argument, रिक्त, HEADER_ओळ_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Used to print non-joining lines */
स्थिर संरचना ओळ uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
स्थिर bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
स्थिर bool join_header_ओळी;

/* The character marking end of line. Default to \n. */
स्थिर अक्षर eolchar = '\n';

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      ख_माला_दो (_("\
For each pair of आदान ओळी with identical join fields, लिख a ओळ to\n\
standard output.  The पूर्वनिर्धारित join field is the first, delimited by blanks.\
\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
When FILE1 or FILE2 (not both) is -, पठ standard आदान.\n\
"), मानक_निकास);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable ओळी from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding to FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (empty) आदान fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-स्थिती\n\
         ignore differences in स्थिती when comparing fields\n\
"));
      oputs (_("\
  -j FIELD\n\
         equivalent to '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT पर्यंत constructing output ओळ\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as आदान and output field separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output ओळी\n\
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
         check that the आदान is correctly sorted, even\n\
         जर all आदान ओळी are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         करा not check that the आदान is correctly sorted\n\
"));
      oputs (_("\
      --header\n\
         treat the first ओळ in each file as field headers,\n\
         print them without trying to pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ओळ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
अन्यथा fields are separated by CHAR.  Any FIELD is a field संख्या counted\n\
from 1.  FORMAT is one or more comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join field,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
ओळ of each file determines the संख्या of fields output साठी each ओळ.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" जर 'join' has no options,\n\
or use \"join -t ''\" जर 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the आदान is not sorted and some ओळी cannot be joined, a\n\
warning message will be given.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

स्थिर शून्य
extract_field (संरचना ओळ *ओळ, अक्षर *field, idx_t len)
{
  जर (ओळ->nfields >= ओळ->nfields_allocated)
    ओळ->fields = xpalloc (ओळ->fields, &ओळ->nfields_allocated, 1,
                            -1, आकार *ओळ->fields);
  idx_t स्थिरांक field_index = ओळ->nfields++;
  ओळ->fields[field_index].beg = field;
  ओळ->fields[field_index].len = len;
}

स्थिर bool
eq_tab (mcel_t g)
{
  परत mcel_eq (g, tab);
}

स्थिर bool
newline_or_blank (mcel_t g)
{
  परत g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

स्थिर शून्य
xfields (संरचना ओळ *ओळ)
{
  अक्षर *ptr = ओळ->buf.buffer;
  अक्षर स्थिरांक *lim = ptr + ओळ->buf.length - 1;

  जर (ptr == lim)
    परत;

  जर (!tab.len)
    पर्यंत ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        अक्षर *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (ओळ, ptr, sep - ptr);
        ptr = sep;
      }
  अन्यथा
    {
      जर (tab.ch != '\n')
        साठी (अक्षर *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (ओळ, ptr, sep - ptr);

      extract_field (ओळ, ptr, lim - ptr);
    }
}

स्थिर शून्य
freeline (संरचना ओळ *ओळ)
{
  जर (ओळ == रिक्त)
    परत;
  मुक्त (ओळ->fields);
  ओळ->fields = रिक्त;
  ओळ->nfields_allocated = 0;
  मुक्त (ओळ->buf.buffer);
  ओळ->buf.buffer = रिक्त;
  ओळ->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

स्थिर पूर्णांक
keycmp (संरचना ओळ स्थिरांक *line1, संरचना ओळ स्थिरांक *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  अक्षर *beg1;
  अक्षर *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  पूर्णांक diff;

  जर (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  अन्यथा
    {
      beg1 = रिक्त;
      len1 = 0;
    }

  जर (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  अन्यथा
    {
      beg2 = रिक्त;
      len2 = 0;
    }

  जर (len1 == 0)
    परत len2 == 0 ? 0 : -1;
  जर (len2 == 0)
    परत 1;

  जर (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  अन्यथा
    {
      जर (hard_LC_COLLATE)
        परत xmemcoll (beg1, len1, beg2, len2);
      diff = स_भेद (beg1, beg2, MIN (len1, len2));
    }

  जर (diff)
    परत diff;
  परत _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

स्थिर शून्य
check_order (स्थिरांक संरचना ओळ *prev,
             स्थिरांक संरचना ओळ *current,
             पूर्णांक whatfile)
{
  जर (check_आदान_order != CHECK_ORDER_DISABLED
      && ((check_आदान_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      जर (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          जर (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              जर (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (पूर्णांक_उच्च, len);

              error ((check_आदान_order == CHECK_ORDER_ENABLED
                      ? निकास_त्रुटि : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], ओळ_no[whatfile - 1],
                     (पूर्णांक) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

स्थिर ओळीत शून्य
reset_ओळ (संरचना ओळ *ओळ)
{
  ओळ->nfields = 0;
}

स्थिर संरचना ओळ *
init_linep (संरचना ओळ **linep)
{
  संरचना ओळ *ओळ = xzalloc (आकार *ओळ);
  *linep = ओळ;
  परत ओळ;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

स्थिर bool
get_ओळ (खाता *fp, संरचना ओळ **linep, पूर्णांक which)
{
  संरचना ओळ *ओळ = *linep;

  जर (ओळ == prevline[which - 1])
    {
      SWAPLINES (ओळ, spareline[which - 1]);
      *linep = ओळ;
    }

  जर (ओळ)
    reset_ओळ (ओळ);
  अन्यथा
    ओळ = init_linep (linep);

  जर (! readlinebuffer_delim (&ओळ->buf, fp, eolchar))
    {
      जर (ख_त्रुटि (fp))
        error (निकास_त्रुटि, त्रुटि_सं, _("read error"));
      freeline (ओळ);
      परत false;
    }
  ++ओळ_no[which - 1];

  xfields (ओळ);

  जर (prevline[which - 1])
    check_order (prevline[which - 1], ओळ, which);

  prevline[which - 1] = ओळ;
  परत true;
}

स्थिर शून्य
free_spareline (शून्य)
{
  साठी (idx_t i = 0; i < countof (spareline); i++)
    {
      जर (spareline[i])
        {
          freeline (spareline[i]);
          मुक्त (spareline[i]);
        }
    }
}

स्थिर शून्य
initseq (संरचना seq *seq)
{
  seq->मोजणी = 0;
  seq->alloc = 0;
  seq->ओळी = रिक्त;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

स्थिर bool
getseq (खाता *fp, संरचना seq *seq, पूर्णांक whichfile)
{
  जर (seq->मोजणी == seq->alloc)
    {
      seq->ओळी = xpalloc (seq->ओळी, &seq->alloc, 1, -1, आकार *seq->ओळी);
      साठी (idx_t i = seq->मोजणी; i < seq->alloc; i++)
        seq->ओळी[i] = रिक्त;
    }

  जर (get_ओळ (fp, &seq->ओळी[seq->मोजणी], whichfile))
    {
      ++seq->मोजणी;
      परत true;
    }
  परत false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
स्थिर bool
advance_seq (खाता *fp, संरचना seq *seq, bool first, पूर्णांक whichfile)
{
  जर (first)
    seq->मोजणी = 0;

  परत getseq (fp, seq, whichfile);
}

स्थिर शून्य
delseq (संरचना seq *seq)
{
  साठी (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->ओळी[i]);
      मुक्त (seq->ओळी[i]);
    }
  मुक्त (seq->ओळी);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

स्थिर शून्य
prfield (idx_t n, संरचना ओळ स्थिरांक *ओळ)
{
  जर (n < ओळ->nfields)
    {
      idx_t len = ओळ->fields[n].len;
      जर (len)
        ख_डालो (ओळ->fields[n].beg, 1, len, मानक_निकास);
      अन्यथा जर (empty_filler)
        ख_माला_दो (empty_filler, मानक_निकास);
    }
  अन्यथा जर (empty_filler)
    ख_माला_दो (empty_filler, मानक_निकास);
}

/* Output all the fields in line, other than the join field.  */

स्थिर शून्य
prfields (संरचना ओळ स्थिरांक *ओळ, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : ओळ->nfields;

  साठी (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      ख_डालो (output_separator, 1, output_seplen, मानक_निकास);
      prfield (i, ओळ);
    }
  साठी (idx_t i = join_field + 1; i < nfields; ++i)
    {
      ख_डालो (output_separator, 1, output_seplen, मानक_निकास);
      prfield (i, ओळ);
    }
}

/* Print the join of LINE1 and LINE2.  */

स्थिर शून्य
prjoin (संरचना ओळ स्थिरांक *line1, संरचना ओळ स्थिरांक *line2)
{
  स्थिरांक संरचना outlist *outlist;
  idx_t field;
  संरचना ओळ स्थिरांक *ओळ;

  outlist = outlist_head.next;
  जर (outlist)
    {
      स्थिरांक संरचना outlist *o;

      o = outlist;
      पर्यंत (true)
        {
          जर (o->file == 0)
            {
              जर (line1 == &uni_blank)
                {
                  ओळ = line2;
                  field = join_field_2;
                }
              अन्यथा
                {
                  ओळ = line1;
                  field = join_field_1;
                }
            }
          अन्यथा
            {
              ओळ = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, ओळ);
          o = o->next;
          जर (o == रिक्त)
            थांबा;
          ख_डालो (output_separator, 1, output_seplen, मानक_निकास);
        }
      अक्षर_दो (eolchar);
    }
  अन्यथा
    {
      जर (line1 == &uni_blank)
        {
          ओळ = line2;
          field = join_field_2;
        }
      अन्यथा
        {
          ओळ = line1;
          field = join_field_1;
        }

      /* Output the join field.  */
      prfield (field, ओळ);

      /* Output other fields.  */
      prfields (line1, join_field_1, autocount_1);
      prfields (line2, join_field_2, autocount_2);

      अक्षर_दो (eolchar);
    }

  जर (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

स्थिर शून्य
join (खाता *fp1, खाता *fp2)
{
  संरचना seq seq1, seq2;
  पूर्णांक diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  जर (autoformat)
    {
      autocount_1 = seq1.मोजणी ? seq1.ओळी[0]->nfields : 0;
      autocount_2 = seq2.मोजणी ? seq2.ओळी[0]->nfields : 0;
    }

  जर (join_header_ओळी && (seq1.मोजणी || seq2.मोजणी))
    {
      संरचना ओळ स्थिरांक *hline1 = seq1.मोजणी ? seq1.ओळी[0] : &uni_blank;
      संरचना ओळ स्थिरांक *hline2 = seq2.मोजणी ? seq2.ओळी[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = रिक्त;
      prevline[1] = रिक्त;
      जर (seq1.मोजणी)
        advance_seq (fp1, &seq1, true, 1);
      जर (seq2.मोजणी)
        advance_seq (fp2, &seq2, true, 2);
    }

  पर्यंत (seq1.मोजणी && seq2.मोजणी)
    {
      diff = keycmp (seq1.ओळी[0], seq2.ओळी[0],
                     join_field_1, join_field_2);
      जर (diff < 0)
        {
          जर (print_unpairables_1)
            prjoin (seq1.ओळी[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          सुरूठेवा;
        }
      जर (diff > 0)
        {
          जर (print_unpairables_2)
            prjoin (&uni_blank, seq2.ओळी[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          सुरूठेवा;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      करा
        जर (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.मोजणी;
            थांबा;
          }
      पर्यंत (!keycmp (seq1.ओळी[seq1.मोजणी - 1], seq2.ओळी[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      करा
        जर (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.मोजणी;
            थांबा;
          }
      पर्यंत (!keycmp (seq1.ओळी[0], seq2.ओळी[seq2.मोजणी - 1],
                      join_field_1, join_field_2));

      जर (print_pairables)
        {
          साठी (idx_t i = 0; i < seq1.मोजणी - 1; ++i)
            {
              साठी (idx_t j = 0; j < seq2.मोजणी - 1; ++j)
                prjoin (seq1.ओळी[i], seq2.ओळी[j]);
            }
        }

      जर (!eof1)
        {
          SWAPLINES (seq1.ओळी[0], seq1.ओळी[seq1.मोजणी - 1]);
          seq1.मोजणी = 1;
        }
      अन्यथा
        seq1.मोजणी = 0;

      जर (!eof2)
        {
          SWAPLINES (seq2.ओळी[0], seq2.ओळी[seq2.मोजणी - 1]);
          seq2.मोजणी = 1;
        }
      अन्यथा
        seq2.मोजणी = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  संरचना ओळ *ओळ = रिक्त;
  bool checktail = false;

  जर (check_आदान_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  जर ((print_unpairables_1 || checktail) && seq1.मोजणी)
    {
      जर (print_unpairables_1)
        prjoin (seq1.ओळी[0], &uni_blank);
      जर (seq2.मोजणी)
        seen_unpairable = true;
      पर्यंत (get_ओळ (fp1, &ओळ, 1))
        {
          जर (print_unpairables_1)
            prjoin (ओळ, &uni_blank);
          जर (issued_disorder_warning[0] && !print_unpairables_1)
            थांबा;
        }
    }

  जर ((print_unpairables_2 || checktail) && seq2.मोजणी)
    {
      जर (print_unpairables_2)
        prjoin (&uni_blank, seq2.ओळी[0]);
      जर (seq1.मोजणी)
        seen_unpairable = true;
      पर्यंत (get_ओळ (fp2, &ओळ, 2))
        {
          जर (print_unpairables_2)
            prjoin (&uni_blank, ओळ);
          जर (issued_disorder_warning[1] && !print_unpairables_2)
            थांबा;
        }
    }

  freeline (ओळ);
  मुक्त (ओळ);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

स्थिर शून्य
add_field (पूर्णांक file, idx_t field)
{
  संरचना outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || field == 0);

  o = xmalloc (आकार *o);
  o->file = file;
  o->field = field;
  o->next = रिक्त;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_end->next = o;
  outlist_end = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

स्थिर idx_t
string_to_join_field (अक्षर स्थिरांक *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, रिक्त, 10, &val, "");
  जर (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  अन्यथा जर (s_err != LONGINT_OK || val <= 0)
    error (निकास_त्रुटि, 0, _("invalid field number: %s"), quote (str));

  परत val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

स्थिर शून्य
decode_field_spec (अक्षर स्थिरांक *s, पूर्णांक *file_index, idx_t *field_index)
{
  /* The first character must be 0, 1, or 2.  */
  निवड (s[0])
    {
    स्थिती '0':
      जर (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (निकास_त्रुटि, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      थांबा;

    स्थिती '1':
    स्थिती '2':
      जर (s[1] != '.')
        error (निकास_त्रुटि, 0, _("invalid field specifier: %s"), quote (s));
      *file_index = s[0] - '0';
      *field_index = string_to_join_field (s + 2);
      थांबा;

    पूर्वनिर्धारित:
      error (निकास_त्रुटि, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

स्थिर bool
comma_or_blank (mcel_t g)
{
  परत g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

स्थिर शून्य
add_field_list (अक्षर *str)
{
  अक्षर *p = str;

  करा
    {
      पूर्णांक file_index;
      idx_t field_index;
      अक्षर स्थिरांक *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      जर (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_index, &field_index);
      add_field (file_index, field_index);
    }
  पर्यंत (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

स्थिर शून्य
set_join_field (सूचक_भेद_प्रकार *var, idx_t val)
{
  जर (0 <= *var && *var != val)
    error (निकास_त्रुटि, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

गणना operand_status
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

स्थिर शून्य
add_file_name (अक्षर *name, अक्षर *names[2],
               पूर्णांक operand_status[2], पूर्णांक joption_मोजणी[2], पूर्णांक *nfiles,
               पूर्णांक *prev_optc_status, पूर्णांक *optc_status)
{
  पूर्णांक n = *nfiles;

  जर (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      अक्षर *arg = names[op0];
      निवड (operand_status[op0])
        {
        स्थिती MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (निकास_त्रुटि);

        स्थिती MIGHT_BE_J1_ARG:
          joption_मोजणी[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          थांबा;

        स्थिती MIGHT_BE_J2_ARG:
          joption_मोजणी[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          थांबा;

        स्थिती MIGHT_BE_O_ARG:
          add_field_list (arg);
          थांबा;
        }
      जर (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  जर (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc_status;
  पूर्णांक prev_optc_status = MUST_BE_OPERAND;
  पूर्णांक operand_status[2];
  पूर्णांक joption_मोजणी[2] = { 0, 0 };
  खाता *fp1, *fp2;
  पूर्णांक optc;
  पूर्णांक nfiles = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  निकास_पर (close_stdout);
  निकास_पर (free_spareline);

  पर्यंत ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, रिक्त))
         != -1)
    {
      optc_status = MUST_BE_OPERAND;

      निवड (optc)
        {
        स्थिती 'v':
            print_pairables = false;
            FALLTHROUGH;

        स्थिती 'a':
          {
            दीर्घ पूर्णांक val;
            जर (xstrtol (optarg, रिक्त, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (निकास_त्रुटि, 0,
                     _("invalid file number: %s"), quote (optarg));
            जर (val == 1)
              print_unpairables_1 = true;
            अन्यथा
              print_unpairables_2 = true;
          }
          थांबा;

        स्थिती 'e':
          जर (empty_filler && ! streq (empty_filler, optarg))
            error (निकास_त्रुटि, 0,
                   _("conflicting empty-field replacement strings"));
          empty_filler = optarg;
          थांबा;

        स्थिती 'i':
          ignore_case = true;
          थांबा;

        स्थिती '1':
          set_join_field (&join_field_1, string_to_join_field (optarg));
          थांबा;

        स्थिती '2':
          set_join_field (&join_field_2, string_to_join_field (optarg));
          थांबा;

        स्थिती 'j':
          जर ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_मोजणी[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          अन्यथा
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          थांबा;

        स्थिती 'o':
          जर (streq (optarg, "auto"))
            autoformat = true;
          अन्यथा
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          थांबा;

        स्थिती 't':
          {
            mcel_t newtab;
            जर (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            अन्यथा जर (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            अन्यथा
              {
                newtab = mcel_scanz (optarg);
                जर (optarg[newtab.len])
                  error (निकास_त्रुटि, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            जर (tab.len && ! mcel_eq (tab, newtab))
              error (निकास_त्रुटि, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          थांबा;

        स्थिती 'z':
          eolchar = 0;
          थांबा;

        स्थिती NOCHECK_ORDER_OPTION:
          check_आदान_order = CHECK_ORDER_DISABLED;
          थांबा;

        स्थिती CHECK_ORDER_OPTION:
          check_आदान_order = CHECK_ORDER_ENABLED;
          थांबा;

        स्थिती 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_मोजणी,
                         &nfiles, &prev_optc_status, &optc_status);
          थांबा;

        स्थिती HEADER_ओळ_OPTION:
          join_header_ओळी = true;
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }

      prev_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  prev_optc_status = MUST_BE_OPERAND;
  पर्यंत (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_मोजणी,
                   &nfiles, &prev_optc_status, &optc_status);

  जर (nfiles != 2)
    {
      जर (nfiles == 0)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  साठी (पूर्णांक i = 0; i < 2; i++)
    जर (joption_मोजणी[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  जर (join_field_1 < 0)
    join_field_1 = 0;
  जर (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? मानक_निवेश : ख_खोलो (g_names[0], "r");
  जर (!fp1)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? मानक_निवेश : ख_खोलो (g_names[1], "r");
  जर (!fp2)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[1]));
  जर (fp1 == fp2)
    error (निकास_त्रुटि, त्रुटि_सं, _("both files cannot be standard input"));
  join (fp1, fp2);

  जर (ख_बंद (fp1) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[0]));
  जर (ख_बंद (fp2) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[1]));

  जर (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (निकास_त्रुटि, 0, _("input is not in sorted order"));
  अन्यथा
    परत निकास_सफल;
}
