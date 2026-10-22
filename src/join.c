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

#define SWAPLINES(a, b) कुरु { \
  संरचना पंक्ति *tmp = a; \
  a = b; \
  b = tmp; \
} यावत् (0);

/* An element of the list identifying which fields to print for each
   output line.  */
संरचना outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    पूर्णाङ्क file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t field;

    संरचना outlist *next;
  };

/* A field of a line.  */
संरचना field
  {
    वर्ण *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
संरचना पंक्ति
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
    idx_t गिनती;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    संरचना पंक्ति **पंक्तियाँ COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
स्थिर संरचना पंक्ति *prevline[2] = {रिक्त, रिक्त};

/* The number of lines read from each file.  */
स्थिर uintmax_t पंक्ति_no[2] = {0, 0};

/* The input file names.  */
स्थिर वर्ण *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
स्थिर संरचना पंक्ति *spareline[2] = {रिक्त, रिक्त};

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
स्थिर वर्ण ध्रुव *empty_filler;

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
स्थिर वर्ण ध्रुव *output_separator = " ";
स्थिर idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
स्थिर गणना
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_निवेश_order;

गणना
{
  CHECK_ORDER_OPTION = अक्षर_उच्च + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_पंक्ति_OPTION
};


स्थिर संरचना option ध्रुव longopts[] =
{
  {"ignore-case", no_argument, रिक्त, 'i'},
  {"check-order", no_argument, रिक्त, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, रिक्त, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {"header", no_argument, रिक्त, HEADER_पंक्ति_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Used to print non-joining lines */
स्थिर संरचना पंक्ति uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
स्थिर bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
स्थिर bool join_header_पंक्तियाँ;

/* The character marking end of line. Default to \n. */
स्थिर वर्ण eolchar = '\n';

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      ख_माला_दो (_("\
For each pair of निवेश पंक्तियाँ with identical join fields, लिख a पंक्ति to\n\
standard output.  The स्वभाव join field is the first, delimited by blanks.\
\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
When FILE1 or FILE2 (not both) is -, पठ standard निवेश.\n\
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
  -i, --ignore-पक्ष\n\
         ignore differences in पक्ष when comparing fields\n\
"));
      oputs (_("\
  -j FIELD\n\
         equivalent to '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT यावत् constructing output पंक्ति\n\
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
         यदि all निवेश पंक्तियाँ are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         कुरु not check that the निवेश is correctly sorted\n\
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
पंक्ति of each file determines the संख्या of fields output प्रति each पंक्ति.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" यदि 'join' has no options,\n\
or use \"join -t ''\" यदि 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the निवेश is not sorted and some पंक्तियाँ cannot be joined, a\n\
warning message will be given.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

स्थिर शून्य
extract_field (संरचना पंक्ति *पंक्ति, वर्ण *field, idx_t len)
{
  यदि (पंक्ति->nfields >= पंक्ति->nfields_allocated)
    पंक्ति->fields = xpalloc (पंक्ति->fields, &पंक्ति->nfields_allocated, 1,
                            -1, परिमाण *पंक्ति->fields);
  idx_t ध्रुव field_index = पंक्ति->nfields++;
  पंक्ति->fields[field_index].beg = field;
  पंक्ति->fields[field_index].len = len;
}

स्थिर bool
eq_tab (mcel_t g)
{
  प्रत्यागम mcel_eq (g, tab);
}

स्थिर bool
newline_or_blank (mcel_t g)
{
  प्रत्यागम g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

स्थिर शून्य
xfields (संरचना पंक्ति *पंक्ति)
{
  वर्ण *ptr = पंक्ति->buf.buffer;
  वर्ण ध्रुव *lim = ptr + पंक्ति->buf.length - 1;

  यदि (ptr == lim)
    प्रत्यागम;

  यदि (!tab.len)
    यावत् ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        वर्ण *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (पंक्ति, ptr, sep - ptr);
        ptr = sep;
      }
  अन्यथा
    {
      यदि (tab.ch != '\n')
        प्रति (वर्ण *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (पंक्ति, ptr, sep - ptr);

      extract_field (पंक्ति, ptr, lim - ptr);
    }
}

स्थिर शून्य
freeline (संरचना पंक्ति *पंक्ति)
{
  यदि (पंक्ति == रिक्त)
    प्रत्यागम;
  मुक्त (पंक्ति->fields);
  पंक्ति->fields = रिक्त;
  पंक्ति->nfields_allocated = 0;
  मुक्त (पंक्ति->buf.buffer);
  पंक्ति->buf.buffer = रिक्त;
  पंक्ति->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

स्थिर पूर्णाङ्क
keycmp (संरचना पंक्ति ध्रुव *line1, संरचना पंक्ति ध्रुव *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  वर्ण *beg1;
  वर्ण *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  पूर्णाङ्क diff;

  यदि (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  अन्यथा
    {
      beg1 = रिक्त;
      len1 = 0;
    }

  यदि (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  अन्यथा
    {
      beg2 = रिक्त;
      len2 = 0;
    }

  यदि (len1 == 0)
    प्रत्यागम len2 == 0 ? 0 : -1;
  यदि (len2 == 0)
    प्रत्यागम 1;

  यदि (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  अन्यथा
    {
      यदि (hard_LC_COLLATE)
        प्रत्यागम xmemcoll (beg1, len1, beg2, len2);
      diff = स_भेद (beg1, beg2, MIN (len1, len2));
    }

  यदि (diff)
    प्रत्यागम diff;
  प्रत्यागम _GL_CMP (len1, len2);
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
check_order (ध्रुव संरचना पंक्ति *prev,
             ध्रुव संरचना पंक्ति *current,
             पूर्णाङ्क whatfile)
{
  यदि (check_निवेश_order != CHECK_ORDER_DISABLED
      && ((check_निवेश_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      यदि (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          यदि (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              यदि (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (पूर्णांक_उच्च, len);

              error ((check_निवेश_order == CHECK_ORDER_ENABLED
                      ? निकास_त्रुटि : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], पंक्ति_no[whatfile - 1],
                     (पूर्णाङ्क) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

स्थिर अन्तःस्थ शून्य
reset_पंक्ति (संरचना पंक्ति *पंक्ति)
{
  पंक्ति->nfields = 0;
}

स्थिर संरचना पंक्ति *
init_linep (संरचना पंक्ति **linep)
{
  संरचना पंक्ति *पंक्ति = xzalloc (परिमाण *पंक्ति);
  *linep = पंक्ति;
  प्रत्यागम पंक्ति;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

स्थिर bool
get_पंक्ति (खाता *fp, संरचना पंक्ति **linep, पूर्णाङ्क which)
{
  संरचना पंक्ति *पंक्ति = *linep;

  यदि (पंक्ति == prevline[which - 1])
    {
      SWAPLINES (पंक्ति, spareline[which - 1]);
      *linep = पंक्ति;
    }

  यदि (पंक्ति)
    reset_पंक्ति (पंक्ति);
  अन्यथा
    पंक्ति = init_linep (linep);

  यदि (! readlinebuffer_delim (&पंक्ति->buf, fp, eolchar))
    {
      यदि (ख_त्रुटि (fp))
        error (निकास_त्रुटि, त्रुटि_सं, _("read error"));
      freeline (पंक्ति);
      प्रत्यागम false;
    }
  ++पंक्ति_no[which - 1];

  xfields (पंक्ति);

  यदि (prevline[which - 1])
    check_order (prevline[which - 1], पंक्ति, which);

  prevline[which - 1] = पंक्ति;
  प्रत्यागम true;
}

स्थिर शून्य
free_spareline (शून्य)
{
  प्रति (idx_t i = 0; i < countof (spareline); i++)
    {
      यदि (spareline[i])
        {
          freeline (spareline[i]);
          मुक्त (spareline[i]);
        }
    }
}

स्थिर शून्य
initseq (संरचना seq *seq)
{
  seq->गिनती = 0;
  seq->alloc = 0;
  seq->पंक्तियाँ = रिक्त;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

स्थिर bool
getseq (खाता *fp, संरचना seq *seq, पूर्णाङ्क whichfile)
{
  यदि (seq->गिनती == seq->alloc)
    {
      seq->पंक्तियाँ = xpalloc (seq->पंक्तियाँ, &seq->alloc, 1, -1, परिमाण *seq->पंक्तियाँ);
      प्रति (idx_t i = seq->गिनती; i < seq->alloc; i++)
        seq->पंक्तियाँ[i] = रिक्त;
    }

  यदि (get_पंक्ति (fp, &seq->पंक्तियाँ[seq->गिनती], whichfile))
    {
      ++seq->गिनती;
      प्रत्यागम true;
    }
  प्रत्यागम false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
स्थिर bool
advance_seq (खाता *fp, संरचना seq *seq, bool first, पूर्णाङ्क whichfile)
{
  यदि (first)
    seq->गिनती = 0;

  प्रत्यागम getseq (fp, seq, whichfile);
}

स्थिर शून्य
delseq (संरचना seq *seq)
{
  प्रति (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->पंक्तियाँ[i]);
      मुक्त (seq->पंक्तियाँ[i]);
    }
  मुक्त (seq->पंक्तियाँ);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

स्थिर शून्य
prfield (idx_t n, संरचना पंक्ति ध्रुव *पंक्ति)
{
  यदि (n < पंक्ति->nfields)
    {
      idx_t len = पंक्ति->fields[n].len;
      यदि (len)
        ख_डालो (पंक्ति->fields[n].beg, 1, len, मानक_निकास);
      अन्यथा यदि (empty_filler)
        ख_माला_दो (empty_filler, मानक_निकास);
    }
  अन्यथा यदि (empty_filler)
    ख_माला_दो (empty_filler, मानक_निकास);
}

/* Output all the fields in line, other than the join field.  */

स्थिर शून्य
prfields (संरचना पंक्ति ध्रुव *पंक्ति, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : पंक्ति->nfields;

  प्रति (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      ख_डालो (output_separator, 1, output_seplen, मानक_निकास);
      prfield (i, पंक्ति);
    }
  प्रति (idx_t i = join_field + 1; i < nfields; ++i)
    {
      ख_डालो (output_separator, 1, output_seplen, मानक_निकास);
      prfield (i, पंक्ति);
    }
}

/* Print the join of LINE1 and LINE2.  */

स्थिर शून्य
prjoin (संरचना पंक्ति ध्रुव *line1, संरचना पंक्ति ध्रुव *line2)
{
  ध्रुव संरचना outlist *outlist;
  idx_t field;
  संरचना पंक्ति ध्रुव *पंक्ति;

  outlist = outlist_head.next;
  यदि (outlist)
    {
      ध्रुव संरचना outlist *o;

      o = outlist;
      यावत् (true)
        {
          यदि (o->file == 0)
            {
              यदि (line1 == &uni_blank)
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
          o = o->next;
          यदि (o == रिक्त)
            भङ्ग;
          ख_डालो (output_separator, 1, output_seplen, मानक_निकास);
        }
      अक्षर_दो (eolchar);
    }
  अन्यथा
    {
      यदि (line1 == &uni_blank)
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

  यदि (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

स्थिर शून्य
join (खाता *fp1, खाता *fp2)
{
  संरचना seq seq1, seq2;
  पूर्णाङ्क diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  यदि (autoformat)
    {
      autocount_1 = seq1.गिनती ? seq1.पंक्तियाँ[0]->nfields : 0;
      autocount_2 = seq2.गिनती ? seq2.पंक्तियाँ[0]->nfields : 0;
    }

  यदि (join_header_पंक्तियाँ && (seq1.गिनती || seq2.गिनती))
    {
      संरचना पंक्ति ध्रुव *hline1 = seq1.गिनती ? seq1.पंक्तियाँ[0] : &uni_blank;
      संरचना पंक्ति ध्रुव *hline2 = seq2.गिनती ? seq2.पंक्तियाँ[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = रिक्त;
      prevline[1] = रिक्त;
      यदि (seq1.गिनती)
        advance_seq (fp1, &seq1, true, 1);
      यदि (seq2.गिनती)
        advance_seq (fp2, &seq2, true, 2);
    }

  यावत् (seq1.गिनती && seq2.गिनती)
    {
      diff = keycmp (seq1.पंक्तियाँ[0], seq2.पंक्तियाँ[0],
                     join_field_1, join_field_2);
      यदि (diff < 0)
        {
          यदि (print_unpairables_1)
            prjoin (seq1.पंक्तियाँ[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          अनुवर्त;
        }
      यदि (diff > 0)
        {
          यदि (print_unpairables_2)
            prjoin (&uni_blank, seq2.पंक्तियाँ[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          अनुवर्त;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      कुरु
        यदि (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.गिनती;
            भङ्ग;
          }
      यावत् (!keycmp (seq1.पंक्तियाँ[seq1.गिनती - 1], seq2.पंक्तियाँ[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      कुरु
        यदि (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.गिनती;
            भङ्ग;
          }
      यावत् (!keycmp (seq1.पंक्तियाँ[0], seq2.पंक्तियाँ[seq2.गिनती - 1],
                      join_field_1, join_field_2));

      यदि (print_pairables)
        {
          प्रति (idx_t i = 0; i < seq1.गिनती - 1; ++i)
            {
              प्रति (idx_t j = 0; j < seq2.गिनती - 1; ++j)
                prjoin (seq1.पंक्तियाँ[i], seq2.पंक्तियाँ[j]);
            }
        }

      यदि (!eof1)
        {
          SWAPLINES (seq1.पंक्तियाँ[0], seq1.पंक्तियाँ[seq1.गिनती - 1]);
          seq1.गिनती = 1;
        }
      अन्यथा
        seq1.गिनती = 0;

      यदि (!eof2)
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
  संरचना पंक्ति *पंक्ति = रिक्त;
  bool checktail = false;

  यदि (check_निवेश_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  यदि ((print_unpairables_1 || checktail) && seq1.गिनती)
    {
      यदि (print_unpairables_1)
        prjoin (seq1.पंक्तियाँ[0], &uni_blank);
      यदि (seq2.गिनती)
        seen_unpairable = true;
      यावत् (get_पंक्ति (fp1, &पंक्ति, 1))
        {
          यदि (print_unpairables_1)
            prjoin (पंक्ति, &uni_blank);
          यदि (issued_disorder_warning[0] && !print_unpairables_1)
            भङ्ग;
        }
    }

  यदि ((print_unpairables_2 || checktail) && seq2.गिनती)
    {
      यदि (print_unpairables_2)
        prjoin (&uni_blank, seq2.पंक्तियाँ[0]);
      यदि (seq1.गिनती)
        seen_unpairable = true;
      यावत् (get_पंक्ति (fp2, &पंक्ति, 2))
        {
          यदि (print_unpairables_2)
            prjoin (&uni_blank, पंक्ति);
          यदि (issued_disorder_warning[1] && !print_unpairables_2)
            भङ्ग;
        }
    }

  freeline (पंक्ति);
  मुक्त (पंक्ति);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

स्थिर शून्य
add_field (पूर्णाङ्क file, idx_t field)
{
  संरचना outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || field == 0);

  o = xmalloc (परिमाण *o);
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
string_to_join_field (वर्ण ध्रुव *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, रिक्त, 10, &val, "");
  यदि (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  अन्यथा यदि (s_err != LONGINT_OK || val <= 0)
    error (निकास_त्रुटि, 0, _("invalid field number: %s"), quote (str));

  प्रत्यागम val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

स्थिर शून्य
decode_field_spec (वर्ण ध्रुव *s, पूर्णाङ्क *file_index, idx_t *field_index)
{
  /* The first character must be 0, 1, or 2.  */
  विकल्प (s[0])
    {
    पक्ष '0':
      यदि (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (निकास_त्रुटि, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      भङ्ग;

    पक्ष '1':
    पक्ष '2':
      यदि (s[1] != '.')
        error (निकास_त्रुटि, 0, _("invalid field specifier: %s"), quote (s));
      *file_index = s[0] - '0';
      *field_index = string_to_join_field (s + 2);
      भङ्ग;

    स्वभाव:
      error (निकास_त्रुटि, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

स्थिर bool
comma_or_blank (mcel_t g)
{
  प्रत्यागम g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

स्थिर शून्य
add_field_list (वर्ण *str)
{
  वर्ण *p = str;

  कुरु
    {
      पूर्णाङ्क file_index;
      idx_t field_index;
      वर्ण ध्रुव *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      यदि (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_index, &field_index);
      add_field (file_index, field_index);
    }
  यावत् (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

स्थिर शून्य
set_join_field (सूचक_भेद_प्रकार *var, idx_t val)
{
  यदि (0 <= *var && *var != val)
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
add_file_name (वर्ण *name, वर्ण *names[2],
               पूर्णाङ्क operand_status[2], पूर्णाङ्क joption_गिनती[2], पूर्णाङ्क *nfiles,
               पूर्णाङ्क *prev_optc_status, पूर्णाङ्क *optc_status)
{
  पूर्णाङ्क n = *nfiles;

  यदि (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      वर्ण *arg = names[op0];
      विकल्प (operand_status[op0])
        {
        पक्ष MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (निकास_त्रुटि);

        पक्ष MIGHT_BE_J1_ARG:
          joption_गिनती[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          भङ्ग;

        पक्ष MIGHT_BE_J2_ARG:
          joption_गिनती[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          भङ्ग;

        पक्ष MIGHT_BE_O_ARG:
          add_field_list (arg);
          भङ्ग;
        }
      यदि (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  यदि (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc_status;
  पूर्णाङ्क prev_optc_status = MUST_BE_OPERAND;
  पूर्णाङ्क operand_status[2];
  पूर्णाङ्क joption_गिनती[2] = { 0, 0 };
  खाता *fp1, *fp2;
  पूर्णाङ्क optc;
  पूर्णाङ्क nfiles = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  निकास_पर (close_stdout);
  निकास_पर (free_spareline);

  यावत् ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, रिक्त))
         != -1)
    {
      optc_status = MUST_BE_OPERAND;

      विकल्प (optc)
        {
        पक्ष 'v':
            print_pairables = false;
            FALLTHROUGH;

        पक्ष 'a':
          {
            दीर्घ पूर्णाङ्क val;
            यदि (xstrtol (optarg, रिक्त, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (निकास_त्रुटि, 0,
                     _("invalid file number: %s"), quote (optarg));
            यदि (val == 1)
              print_unpairables_1 = true;
            अन्यथा
              print_unpairables_2 = true;
          }
          भङ्ग;

        पक्ष 'e':
          यदि (empty_filler && ! streq (empty_filler, optarg))
            error (निकास_त्रुटि, 0,
                   _("conflicting empty-field replacement strings"));
          empty_filler = optarg;
          भङ्ग;

        पक्ष 'i':
          ignore_case = true;
          भङ्ग;

        पक्ष '1':
          set_join_field (&join_field_1, string_to_join_field (optarg));
          भङ्ग;

        पक्ष '2':
          set_join_field (&join_field_2, string_to_join_field (optarg));
          भङ्ग;

        पक्ष 'j':
          यदि ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
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
          भङ्ग;

        पक्ष 'o':
          यदि (streq (optarg, "auto"))
            autoformat = true;
          अन्यथा
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          भङ्ग;

        पक्ष 't':
          {
            mcel_t newtab;
            यदि (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            अन्यथा यदि (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            अन्यथा
              {
                newtab = mcel_scanz (optarg);
                यदि (optarg[newtab.len])
                  error (निकास_त्रुटि, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            यदि (tab.len && ! mcel_eq (tab, newtab))
              error (निकास_त्रुटि, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          भङ्ग;

        पक्ष 'z':
          eolchar = 0;
          भङ्ग;

        पक्ष NOCHECK_ORDER_OPTION:
          check_निवेश_order = CHECK_ORDER_DISABLED;
          भङ्ग;

        पक्ष CHECK_ORDER_OPTION:
          check_निवेश_order = CHECK_ORDER_ENABLED;
          भङ्ग;

        पक्ष 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_गिनती,
                         &nfiles, &prev_optc_status, &optc_status);
          भङ्ग;

        पक्ष HEADER_पंक्ति_OPTION:
          join_header_पंक्तियाँ = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (निकास_त्रुटि);
        }

      prev_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  prev_optc_status = MUST_BE_OPERAND;
  यावत् (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_गिनती,
                   &nfiles, &prev_optc_status, &optc_status);

  यदि (nfiles != 2)
    {
      यदि (nfiles == 0)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  प्रति (पूर्णाङ्क i = 0; i < 2; i++)
    यदि (joption_गिनती[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  यदि (join_field_1 < 0)
    join_field_1 = 0;
  यदि (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? मानक_निवेश : ख_खोलो (g_names[0], "r");
  यदि (!fp1)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? मानक_निवेश : ख_खोलो (g_names[1], "r");
  यदि (!fp2)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[1]));
  यदि (fp1 == fp2)
    error (निकास_त्रुटि, त्रुटि_सं, _("both files cannot be standard input"));
  join (fp1, fp2);

  यदि (ख_बंद (fp1) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[0]));
  यदि (ख_बंद (fp2) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (g_names[1]));

  यदि (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (निकास_त्रुटि, 0, _("input is not in sorted order"));
  अन्यथा
    प्रत्यागम निकास_सफल;
}
