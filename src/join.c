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

#समावेश <config.h>

#समावेश <sys/types.h>
#समावेश <getopt.h>

#समावेश "system.h"
#समावेश "assure.h"
#समावेश "fadvise.h"
#समावेश "hard-locale.h"
#समावेश "linebuffer.h"
#समावेश "mcel.h"
#समावेश "memcasecmp.h"
#समावेश "quote.h"
#समावेश "skipchars.h"
#समावेश "stdio--.h"
#समावेश "xmemcoll.h"
#समावेश "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "join"

#घोषणा AUTHORS proper_name ("Mike Haertel")

#घोषणा join system_join

#घोषणा SWAPLINES(a, b) करो { \
  काष्ठा पंक्ति *tmp = a; \
  a = b; \
  b = tmp; \
} जबतक (0);

/* An element of the list identifying which fields to print for each
   output line.  */
काष्ठा outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    पूर्णांक file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t field;

    काष्ठा outlist *अग्र;
  };

/* A field of a line.  */
काष्ठा field
  {
    अक्षर *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
काष्ठा पंक्ति
  {
    काष्ठा linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    काष्ठा field *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
काष्ठा seq
  {
    idx_t गिनती;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    काष्ठा पंक्ति **पंक्तियाँ COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
अटल काष्ठा पंक्ति *prevline[2] = {शून्य, शून्य};

/* The number of lines read from each file.  */
अटल uintmax_t पंक्ति_no[2] = {0, 0};

/* The input file names.  */
अटल अक्षर *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
अटल काष्ठा पंक्ति *spareline[2] = {शून्य, शून्य};

/* True if the LC_COLLATE locale is hard.  */
अटल bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
अटल bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
अटल bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
अटल bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
अटल bool issued_disorder_warning[2];

/* Empty output field filler.  */
अटल अक्षर स्थिर *empty_filler;

/* Whether to ensure the same number of fields are output from each line.  */
अटल bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
अटल idx_t autocount_1;
अटल idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
अटल सूचक_भेद_प्रकार join_field_1 = -1;
अटल सूचक_भेद_प्रकार join_field_2 = -1;

/* List of fields to print.  */
अटल काष्ठा outlist outlist_शीर्ष;

/* Last element in 'outlist', where a new element can be added.  */
अटल काष्ठा outlist *outlist_end = &outlist_शीर्ष;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
अटल mcel_t tab;

/* The output separator to use, and its length in bytes.  */
अटल अक्षर स्थिर *output_separator = " ";
अटल idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
अटल क्रमागत
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_निवेश_order;

क्रमागत
{
  CHECK_ORDER_OPTION = अक्षर_उच्च + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_पंक्ति_OPTION
};


अटल काष्ठा option स्थिर longopts[] =
{
  {"ignore-case", no_argument, शून्य, 'i'},
  {"check-order", no_argument, शून्य, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, शून्य, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, शून्य, 'z'},
  {"header", no_argument, शून्य, HEADER_पंक्ति_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Used to print non-joining lines */
अटल काष्ठा पंक्ति uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
अटल bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
अटल bool join_header_पंक्तियाँ;

/* The character marking end of line. Default to \n. */
अटल अक्षर eolchar = '\n';

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      ख_माला_दो (_("\
For each pair of निवेश पंक्तियाँ with identical join fields, ग_लिखो a पंक्ति to\n\
standard output.  The शेष join field is the first, delimited by blanks.\
\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
When FILE1 or FILE2 (not both) is -, पढ़ो standard निवेश.\n\
"), मानक_निकास);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable पंक्तियाँ from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding to FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (empty) निवेश fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-हाल\n\
         ignore differences in हाल when comparing fields\n\
"));
      oputs (_("\
  -j FIELD\n\
         equivalent to '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT जबतक constructing output पंक्ति\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as निवेश and output field separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output पंक्तियाँ\n\
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
         check that the निवेश is correctly sorted, even\n\
         अगर all निवेश पंक्तियाँ are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         करो not check that the निवेश is correctly sorted\n\
"));
      oputs (_("\
      --header\n\
         treat the first पंक्ति in each file as field headers,\n\
         print them without trying to pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पंक्ति delimiter is NUL, not newline\n\
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
पंक्ति of each file determines the संख्या of fields output क्रम each पंक्ति.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" अगर 'join' has no options,\n\
or use \"join -t ''\" अगर 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the निवेश is not sorted and some पंक्तियाँ cannot be joined, a\n\
warning message will be given.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

अटल व्योम
extract_field (काष्ठा पंक्ति *पंक्ति, अक्षर *field, idx_t len)
{
  अगर (पंक्ति->nfields >= पंक्ति->nfields_allocated)
    पंक्ति->fields = xpalloc (पंक्ति->fields, &पंक्ति->nfields_allocated, 1,
                            -1, माप *पंक्ति->fields);
  idx_t स्थिर field_सूचकांक = पंक्ति->nfields++;
  पंक्ति->fields[field_सूचकांक].beg = field;
  पंक्ति->fields[field_सूचकांक].len = len;
}

अटल bool
eq_tab (mcel_t g)
{
  वापस mcel_eq (g, tab);
}

अटल bool
newline_or_blank (mcel_t g)
{
  वापस g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

अटल व्योम
xfields (काष्ठा पंक्ति *पंक्ति)
{
  अक्षर *ptr = पंक्ति->buf.बफर;
  अक्षर स्थिर *lim = ptr + पंक्ति->buf.लंबाई - 1;

  अगर (ptr == lim)
    वापस;

  अगर (!tab.len)
    जबतक ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        अक्षर *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (पंक्ति, ptr, sep - ptr);
        ptr = sep;
      }
  अन्यथा
    {
      अगर (tab.ch != '\n')
        क्रम (अक्षर *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (पंक्ति, ptr, sep - ptr);

      extract_field (पंक्ति, ptr, lim - ptr);
    }
}

अटल व्योम
freeline (काष्ठा पंक्ति *पंक्ति)
{
  अगर (पंक्ति == शून्य)
    वापस;
  मुक्त (पंक्ति->fields);
  पंक्ति->fields = शून्य;
  पंक्ति->nfields_allocated = 0;
  मुक्त (पंक्ति->buf.बफर);
  पंक्ति->buf.बफर = शून्य;
  पंक्ति->buf.आकार = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

अटल पूर्णांक
keycmp (काष्ठा पंक्ति स्थिर *line1, काष्ठा पंक्ति स्थिर *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  अक्षर *beg1;
  अक्षर *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  पूर्णांक diff;

  अगर (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  अन्यथा
    {
      beg1 = शून्य;
      len1 = 0;
    }

  अगर (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  अन्यथा
    {
      beg2 = शून्य;
      len2 = 0;
    }

  अगर (len1 == 0)
    वापस len2 == 0 ? 0 : -1;
  अगर (len2 == 0)
    वापस 1;

  अगर (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  अन्यथा
    {
      अगर (hard_LC_COLLATE)
        वापस xmemcoll (beg1, len1, beg2, len2);
      diff = स_भेद (beg1, beg2, MIN (len1, len2));
    }

  अगर (diff)
    वापस diff;
  वापस _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

अटल व्योम
check_order (स्थिर काष्ठा पंक्ति *पूर्व,
             स्थिर काष्ठा पंक्ति *current,
             पूर्णांक whatfile)
{
  अगर (check_निवेश_order != CHECK_ORDER_DISABLED
      && ((check_निवेश_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      अगर (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          अगर (keycmp (पूर्व, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.लंबाई;
              अगर (0 < len && current->buf.बफर[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (पूर्णांक_उच्च, len);

              error ((check_निवेश_order == CHECK_ORDER_ENABLED
                      ? निकास_त्रुटि : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], पंक्ति_no[whatfile - 1],
                     (पूर्णांक) len, current->buf.बफर);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

अटल अंतरभूत व्योम
reset_पंक्ति (काष्ठा पंक्ति *पंक्ति)
{
  पंक्ति->nfields = 0;
}

अटल काष्ठा पंक्ति *
init_linep (काष्ठा पंक्ति **linep)
{
  काष्ठा पंक्ति *पंक्ति = xzalloc (माप *पंक्ति);
  *linep = पंक्ति;
  वापस पंक्ति;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

अटल bool
get_पंक्ति (खाता *fp, काष्ठा पंक्ति **linep, पूर्णांक which)
{
  काष्ठा पंक्ति *पंक्ति = *linep;

  अगर (पंक्ति == prevline[which - 1])
    {
      SWAPLINES (पंक्ति, spareline[which - 1]);
      *linep = पंक्ति;
    }

  अगर (पंक्ति)
    reset_पंक्ति (पंक्ति);
  अन्यथा
    पंक्ति = init_linep (linep);

  अगर (! readlinebuffer_delim (&पंक्ति->buf, fp, eolchar))
    {
      अगर (ख_त्रुटि (fp))
        error (निकास_त्रुटि, त्रुटि_सं, _("read error"));
      freeline (पंक्ति);
      वापस false;
    }
  ++पंक्ति_no[which - 1];

  xfields (पंक्ति);

  अगर (prevline[which - 1])
    check_order (prevline[which - 1], पंक्ति, which);

  prevline[which - 1] = पंक्ति;
  वापस true;
}

अटल व्योम
free_spareline (व्योम)
{
  क्रम (idx_t i = 0; i < countof (spareline); i++)
    {
      अगर (spareline[i])
        {
          freeline (spareline[i]);
          मुक्त (spareline[i]);
        }
    }
}

अटल व्योम
initseq (काष्ठा seq *seq)
{
  seq->गिनती = 0;
  seq->alloc = 0;
  seq->पंक्तियाँ = शून्य;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

अटल bool
getseq (खाता *fp, काष्ठा seq *seq, पूर्णांक whichfile)
{
  अगर (seq->गिनती == seq->alloc)
    {
      seq->पंक्तियाँ = xpalloc (seq->पंक्तियाँ, &seq->alloc, 1, -1, माप *seq->पंक्तियाँ);
      क्रम (idx_t i = seq->गिनती; i < seq->alloc; i++)
        seq->पंक्तियाँ[i] = शून्य;
    }

  अगर (get_पंक्ति (fp, &seq->पंक्तियाँ[seq->गिनती], whichfile))
    {
      ++seq->गिनती;
      वापस true;
    }
  वापस false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
अटल bool
advance_seq (खाता *fp, काष्ठा seq *seq, bool first, पूर्णांक whichfile)
{
  अगर (first)
    seq->गिनती = 0;

  वापस getseq (fp, seq, whichfile);
}

अटल व्योम
delseq (काष्ठा seq *seq)
{
  क्रम (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->पंक्तियाँ[i]);
      मुक्त (seq->पंक्तियाँ[i]);
    }
  मुक्त (seq->पंक्तियाँ);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

अटल व्योम
prfield (idx_t n, काष्ठा पंक्ति स्थिर *पंक्ति)
{
  अगर (n < पंक्ति->nfields)
    {
      idx_t len = पंक्ति->fields[n].len;
      अगर (len)
        ख_डालो (पंक्ति->fields[n].beg, 1, len, मानक_निकास);
      अन्यथा अगर (empty_filler)
        ख_माला_दो (empty_filler, मानक_निकास);
    }
  अन्यथा अगर (empty_filler)
    ख_माला_दो (empty_filler, मानक_निकास);
}

/* Output all the fields in line, other than the join field.  */

अटल व्योम
prfields (काष्ठा पंक्ति स्थिर *पंक्ति, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : पंक्ति->nfields;

  क्रम (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      ख_डालो (output_separator, 1, output_seplen, मानक_निकास);
      prfield (i, पंक्ति);
    }
  क्रम (idx_t i = join_field + 1; i < nfields; ++i)
    {
      ख_डालो (output_separator, 1, output_seplen, मानक_निकास);
      prfield (i, पंक्ति);
    }
}

/* Print the join of LINE1 and LINE2.  */

अटल व्योम
prjoin (काष्ठा पंक्ति स्थिर *line1, काष्ठा पंक्ति स्थिर *line2)
{
  स्थिर काष्ठा outlist *outlist;
  idx_t field;
  काष्ठा पंक्ति स्थिर *पंक्ति;

  outlist = outlist_शीर्ष.अग्र;
  अगर (outlist)
    {
      स्थिर काष्ठा outlist *o;

      o = outlist;
      जबतक (true)
        {
          अगर (o->file == 0)
            {
              अगर (line1 == &uni_blank)
                {
                  पंक्ति = line2;
                  field = join_field_2;
                }
              अन्यथा
                {
                  पंक्ति = line1;
                  field = join_field_1;
                }
            }
          अन्यथा
            {
              पंक्ति = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, पंक्ति);
          o = o->अग्र;
          अगर (o == शून्य)
            अवरोध;
          ख_डालो (output_separator, 1, output_seplen, मानक_निकास);
        }
      अक्षर_दो (eolchar);
    }
  अन्यथा
    {
      अगर (line1 == &uni_blank)
        {
          पंक्ति = line2;
          field = join_field_2;
        }
      अन्यथा
        {
          पंक्ति = line1;
          field = join_field_1;
        }

      /* Output the join field.  */
      prfield (field, पंक्ति);

      /* Output other fields.  */
      prfields (line1, join_field_1, autocount_1);
      prfields (line2, join_field_2, autocount_2);

      अक्षर_दो (eolchar);
    }

  अगर (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

अटल व्योम
join (खाता *fp1, खाता *fp2)
{
  काष्ठा seq seq1, seq2;
  पूर्णांक diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  अगर (autoformat)
    {
      autocount_1 = seq1.गिनती ? seq1.पंक्तियाँ[0]->nfields : 0;
      autocount_2 = seq2.गिनती ? seq2.पंक्तियाँ[0]->nfields : 0;
    }

  अगर (join_header_पंक्तियाँ && (seq1.गिनती || seq2.गिनती))
    {
      काष्ठा पंक्ति स्थिर *hline1 = seq1.गिनती ? seq1.पंक्तियाँ[0] : &uni_blank;
      काष्ठा पंक्ति स्थिर *hline2 = seq2.गिनती ? seq2.पंक्तियाँ[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = शून्य;
      prevline[1] = शून्य;
      अगर (seq1.गिनती)
        advance_seq (fp1, &seq1, true, 1);
      अगर (seq2.गिनती)
        advance_seq (fp2, &seq2, true, 2);
    }

  जबतक (seq1.गिनती && seq2.गिनती)
    {
      diff = keycmp (seq1.पंक्तियाँ[0], seq2.पंक्तियाँ[0],
                     join_field_1, join_field_2);
      अगर (diff < 0)
        {
          अगर (print_unpairables_1)
            prjoin (seq1.पंक्तियाँ[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          जारी;
        }
      अगर (diff > 0)
        {
          अगर (print_unpairables_2)
            prjoin (&uni_blank, seq2.पंक्तियाँ[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          जारी;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      करो
        अगर (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.गिनती;
            अवरोध;
          }
      जबतक (!keycmp (seq1.पंक्तियाँ[seq1.गिनती - 1], seq2.पंक्तियाँ[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      करो
        अगर (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.गिनती;
            अवरोध;
          }
      जबतक (!keycmp (seq1.पंक्तियाँ[0], seq2.पंक्तियाँ[seq2.गिनती - 1],
                      join_field_1, join_field_2));

      अगर (print_pairables)
        {
          क्रम (idx_t i = 0; i < seq1.गिनती - 1; ++i)
            {
              क्रम (idx_t j = 0; j < seq2.गिनती - 1; ++j)
                prjoin (seq1.पंक्तियाँ[i], seq2.पंक्तियाँ[j]);
            }
        }

      अगर (!eof1)
        {
          SWAPLINES (seq1.पंक्तियाँ[0], seq1.पंक्तियाँ[seq1.गिनती - 1]);
          seq1.गिनती = 1;
        }
      अन्यथा
        seq1.गिनती = 0;

      अगर (!eof2)
        {
          SWAPLINES (seq2.पंक्तियाँ[0], seq2.पंक्तियाँ[seq2.गिनती - 1]);
          seq2.गिनती = 1;
        }
      अन्यथा
        seq2.गिनती = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  काष्ठा पंक्ति *पंक्ति = शून्य;
  bool checktail = false;

  अगर (check_निवेश_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  अगर ((print_unpairables_1 || checktail) && seq1.गिनती)
    {
      अगर (print_unpairables_1)
        prjoin (seq1.पंक्तियाँ[0], &uni_blank);
      अगर (seq2.गिनती)
        seen_unpairable = true;
      जबतक (get_पंक्ति (fp1, &पंक्ति, 1))
        {
          अगर (print_unpairables_1)
            prjoin (पंक्ति, &uni_blank);
          अगर (issued_disorder_warning[0] && !print_unpairables_1)
            अवरोध;
        }
    }

  अगर ((print_unpairables_2 || checktail) && seq2.गिनती)
    {
      अगर (print_unpairables_2)
        prjoin (&uni_blank, seq2.पंक्तियाँ[0]);
      अगर (seq1.गिनती)
        seen_unpairable = true;
      जबतक (get_पंक्ति (fp2, &पंक्ति, 2))
        {
          अगर (print_unpairables_2)
            prjoin (&uni_blank, पंक्ति);
          अगर (issued_disorder_warning[1] && !print_unpairables_2)
            अवरोध;
        }
    }

  freeline (पंक्ति);
  मुक्त (पंक्ति);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

अटल व्योम
add_field (पूर्णांक file, idx_t field)
{
  काष्ठा outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || field == 0);

  o = xmalloc (माप *o);
  o->file = file;
  o->field = field;
  o->अग्र = शून्य;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_end->अग्र = o;
  outlist_end = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

अटल idx_t
string_to_join_field (अक्षर स्थिर *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, शून्य, 10, &val, "");
  अगर (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  अन्यथा अगर (s_err != LONGINT_OK || val <= 0)
    error (निकास_त्रुटि, 0, _("invalid field number: %s"), quote (str));

  वापस val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

अटल व्योम
decode_field_spec (अक्षर स्थिर *s, पूर्णांक *file_सूचकांक, idx_t *field_सूचकांक)
{
  /* The first character must be 0, 1, or 2.  */
  चयन (s[0])
    {
    हाल '0':
      अगर (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (निकास_त्रुटि, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_सूचकांक = 0;
      *field_सूचकांक = 0;
      अवरोध;

    हाल '1':
    हाल '2':
      अगर (s[1] != '.')
        error (निकास_त्रुटि, 0, _("invalid field specifier: %s"), quote (s));
      *file_सूचकांक = s[0] - '0';
      *field_सूचकांक = string_to_join_field (s + 2);
      अवरोध;

    शेष:
      error (निकास_त्रुटि, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

अटल bool
comma_or_blank (mcel_t g)
{
  वापस g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

अटल व्योम
add_field_list (अक्षर *str)
{
  अक्षर *p = str;

  करो
    {
      पूर्णांक file_सूचकांक;
      idx_t field_सूचकांक;
      अक्षर स्थिर *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      अगर (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_सूचकांक, &field_सूचकांक);
      add_field (file_सूचकांक, field_सूचकांक);
    }
  जबतक (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

अटल व्योम
set_join_field (सूचक_भेद_प्रकार *var, idx_t val)
{
  अगर (0 <= *var && *var != val)
    error (निकास_त्रुटि, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

क्रमागत operand_status
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

अटल व्योम
add_file_name (अक्षर *name, अक्षर *names[2],
               पूर्णांक operand_status[2], पूर्णांक joption_गिनती[2], पूर्णांक *nfiles,
               पूर्णांक *पूर्व_optc_status, पूर्णांक *optc_status)
{
  पूर्णांक n = *nfiles;

  अगर (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      अक्षर *arg = names[op0];
      चयन (operand_status[op0])
        {
        हाल MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (निकास_त्रुटि);

        हाल MIGHT_BE_J1_ARG:
          joption_गिनती[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          अवरोध;

        हाल MIGHT_BE_J2_ARG:
          joption_गिनती[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          अवरोध;

        हाल MIGHT_BE_O_ARG:
          add_field_list (arg);
          अवरोध;
        }
      अगर (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *पूर्व_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  अगर (*पूर्व_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc_status;
  पूर्णांक पूर्व_optc_status = MUST_BE_OPERAND;
  पूर्णांक operand_status[2];
  पूर्णांक joption_गिनती[2] = { 0, 0 };
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

  जबतक ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, शून्य))
         != -1)
    {
      optc_status = MUST_BE_OPERAND;

      चयन (optc)
        {
        हाल 'v':
            print_pairables = false;
            FALLTHROUGH;

        हाल 'a':
          {
            दीर्घ पूर्णांक val;
            अगर (xstrtol (optarg, शून्य, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (निकास_त्रुटि, 0,
                     _("invalid file number: %s"), quote (optarg));
            अगर (val == 1)
              print_unpairables_1 = true;
            अन्यथा
              print_unpairables_2 = true;
          }
          अवरोध;

        हाल 'e':
          अगर (empty_filler && ! streq (empty_filler, optarg))
            error (निकास_त्रुटि, 0,
                   _("conflicting empty-field replacement strings"));
          empty_filler = optarg;
          अवरोध;

        हाल 'i':
          ignore_case = true;
          अवरोध;

        हाल '1':
          set_join_field (&join_field_1, string_to_join_field (optarg));
          अवरोध;

        हाल '2':
          set_join_field (&join_field_2, string_to_join_field (optarg));
          अवरोध;

        हाल 'j':
          अगर ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_गिनती[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          अन्यथा
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          अवरोध;

        हाल 'o':
          अगर (streq (optarg, "auto"))
            autoformat = true;
          अन्यथा
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          अवरोध;

        हाल 't':
          {
            mcel_t newtab;
            अगर (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            अन्यथा अगर (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            अन्यथा
              {
                newtab = mcel_scanz (optarg);
                अगर (optarg[newtab.len])
                  error (निकास_त्रुटि, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            अगर (tab.len && ! mcel_eq (tab, newtab))
              error (निकास_त्रुटि, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          अवरोध;

        हाल 'z':
          eolchar = 0;
          अवरोध;

        हाल NOCHECK_ORDER_OPTION:
          check_निवेश_order = CHECK_ORDER_DISABLED;
          अवरोध;

        हाल CHECK_ORDER_OPTION:
          check_निवेश_order = CHECK_ORDER_ENABLED;
          अवरोध;

        हाल 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_गिनती,
                         &nfiles, &पूर्व_optc_status, &optc_status);
          अवरोध;

        हाल HEADER_पंक्ति_OPTION:
          join_header_पंक्तियाँ = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }

      पूर्व_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  पूर्व_optc_status = MUST_BE_OPERAND;
  जबतक (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_गिनती,
                   &nfiles, &पूर्व_optc_status, &optc_status);

  अगर (nfiles != 2)
    {
      अगर (nfiles == 0)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  क्रम (पूर्णांक i = 0; i < 2; i++)
    अगर (joption_गिनती[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  अगर (join_field_1 < 0)
    join_field_1 = 0;
  अगर (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? मानक_निवेश : ख_खोलो (g_names[0], "r");
  अगर (!fp1)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? मानक_निवेश : ख_खोलो (g_names[1], "r");
  अगर (!fp2)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[1]));
  अगर (fp1 == fp2)
    error (निकास_त्रुटि, त्रुटि_सं, _("both files cannot be standard input"));
  join (fp1, fp2);

  अगर (ख_बंद (fp1) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[0]));
  अगर (ख_बंद (fp2) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[1]));

  अगर (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (निकास_त्रुटि, 0, _("input is not in sorted order"));
  अन्यथा
    वापस निकास_सफल;
}
