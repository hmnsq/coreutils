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

#define SWAPLINES(a, b) करेहि { \
  संरचना पङ्क्तिः *tmp = a; \
  a = b; \
  b = tmp; \
} जाव (0);

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
संरचना पङ्क्तिः
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
    idx_t गणनम्;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    संरचना पङ्क्तिः **पङ्क्तयः COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
स्थिर संरचना पङ्क्तिः *prevline[2] = {रिक्त, रिक्त};

/* The number of lines read from each file.  */
स्थिर uintmax_t पङ्क्तिः_no[2] = {0, 0};

/* The input file names.  */
स्थिर वर्ण *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
स्थिर संरचना पङ्क्तिः *spareline[2] = {रिक्त, रिक्त};

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
स्थिर ptrdiff_t join_field_1 = -1;
स्थिर ptrdiff_t join_field_2 = -1;

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
  } check_निवेशः_order;

गणना
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_पङ्क्तिः_OPTION
};


स्थिर संरचना option ध्रुव longopts[] =
{
  {"ignore-case", no_argument, रिक्त, 'i'},
  {"check-order", no_argument, रिक्त, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, रिक्त, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {"header", no_argument, रिक्त, HEADER_पङ्क्तिः_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Used to print non-joining lines */
स्थिर संरचना पङ्क्तिः uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
स्थिर bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
स्थिर bool join_header_पङ्क्तयः;

/* The character marking end of line. Default to \n. */
स्थिर वर्ण eolchar = '\n';

शून्य
usage (पूर्णाङ्क status)
{
  जइ (status != EXIT_SUCCESS)
    emit_try_help ();
  अण्णहा
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
For each pair of निवेशः पङ्क्तयः with identical join fields, लिख a पङ्क्तिः to\n\
standard output.  The स्वभाव join field is the first, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, पठ standard निवेशः.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable पङ्क्तयः from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding to FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (empty) निवेशः fields with STRING;\n\
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
         obey FORMAT जाव constructing output पङ्क्तिः\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as निवेशः and output field separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output पङ्क्तयः\n\
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
         check that the निवेशः is correctly sorted, even\n\
         जइ all निवेशः पङ्क्तयः are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         करेहि not check that the निवेशः is correctly sorted\n\
"));
      oputs (_("\
      --header\n\
         treat the first पङ्क्तिः in each file as field headers,\n\
         print them without trying to pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
अण्णहा fields are separated by CHAR.  Any FIELD is a field सङ्ख्या counted\n\
from 1.  FORMAT is one or more comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join field,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
पङ्क्तिः of each file determines the सङ्ख्या of fields output प्रति each पङ्क्तिः.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" जइ 'join' has no options,\n\
or use \"join -t ''\" जइ 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the निवेशः is not sorted and some पङ्क्तयः cannot be joined, a\n\
warning message will be given.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

स्थिर शून्य
extract_field (संरचना पङ्क्तिः *पङ्क्तिः, वर्ण *field, idx_t len)
{
  जइ (पङ्क्तिः->nfields >= पङ्क्तिः->nfields_allocated)
    पङ्क्तिः->fields = xpalloc (पङ्क्तिः->fields, &पङ्क्तिः->nfields_allocated, 1,
                            -1, परिमाण *पङ्क्तिः->fields);
  idx_t ध्रुव field_index = पङ्क्तिः->nfields++;
  पङ्क्तिः->fields[field_index].beg = field;
  पङ्क्तिः->fields[field_index].len = len;
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
xfields (संरचना पङ्क्तिः *पङ्क्तिः)
{
  वर्ण *ptr = पङ्क्तिः->buf.buffer;
  वर्ण ध्रुव *lim = ptr + पङ्क्तिः->buf.length - 1;

  जइ (ptr == lim)
    प्रत्यागम;

  जइ (!tab.len)
    जाव ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        वर्ण *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (पङ्क्तिः, ptr, sep - ptr);
        ptr = sep;
      }
  अण्णहा
    {
      जइ (tab.ch != '\n')
        प्रति (वर्ण *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (पङ्क्तिः, ptr, sep - ptr);

      extract_field (पङ्क्तिः, ptr, lim - ptr);
    }
}

स्थिर शून्य
freeline (संरचना पङ्क्तिः *पङ्क्तिः)
{
  जइ (पङ्क्तिः == रिक्त)
    प्रत्यागम;
  मुक्त (पङ्क्तिः->fields);
  पङ्क्तिः->fields = रिक्त;
  पङ्क्तिः->nfields_allocated = 0;
  मुक्त (पङ्क्तिः->buf.buffer);
  पङ्क्तिः->buf.buffer = रिक्त;
  पङ्क्तिः->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

स्थिर पूर्णाङ्क
keycmp (संरचना पङ्क्तिः ध्रुव *line1, संरचना पङ्क्तिः ध्रुव *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  वर्ण *beg1;
  वर्ण *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  पूर्णाङ्क diff;

  जइ (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  अण्णहा
    {
      beg1 = रिक्त;
      len1 = 0;
    }

  जइ (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  अण्णहा
    {
      beg2 = रिक्त;
      len2 = 0;
    }

  जइ (len1 == 0)
    प्रत्यागम len2 == 0 ? 0 : -1;
  जइ (len2 == 0)
    प्रत्यागम 1;

  जइ (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  अण्णहा
    {
      जइ (hard_LC_COLLATE)
        प्रत्यागम xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  जइ (diff)
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
check_order (ध्रुव संरचना पङ्क्तिः *prev,
             ध्रुव संरचना पङ्क्तिः *current,
             पूर्णाङ्क whatfile)
{
  जइ (check_निवेशः_order != CHECK_ORDER_DISABLED
      && ((check_निवेशः_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      जइ (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          जइ (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              जइ (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (INT_MAX, len);

              error ((check_निवेशः_order == CHECK_ORDER_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], पङ्क्तिः_no[whatfile - 1],
                     (पूर्णाङ्क) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

स्थिर अन्तःस्थ शून्य
reset_पङ्क्तिः (संरचना पङ्क्तिः *पङ्क्तिः)
{
  पङ्क्तिः->nfields = 0;
}

स्थिर संरचना पङ्क्तिः *
init_linep (संरचना पङ्क्तिः **linep)
{
  संरचना पङ्क्तिः *पङ्क्तिः = xzalloc (परिमाण *पङ्क्तिः);
  *linep = पङ्क्तिः;
  प्रत्यागम पङ्क्तिः;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

स्थिर bool
get_पङ्क्तिः (FILE *fp, संरचना पङ्क्तिः **linep, पूर्णाङ्क which)
{
  संरचना पङ्क्तिः *पङ्क्तिः = *linep;

  जइ (पङ्क्तिः == prevline[which - 1])
    {
      SWAPLINES (पङ्क्तिः, spareline[which - 1]);
      *linep = पङ्क्तिः;
    }

  जइ (पङ्क्तिः)
    reset_पङ्क्तिः (पङ्क्तिः);
  अण्णहा
    पङ्क्तिः = init_linep (linep);

  जइ (! readlinebuffer_delim (&पङ्क्तिः->buf, fp, eolchar))
    {
      जइ (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (पङ्क्तिः);
      प्रत्यागम false;
    }
  ++पङ्क्तिः_no[which - 1];

  xfields (पङ्क्तिः);

  जइ (prevline[which - 1])
    check_order (prevline[which - 1], पङ्क्तिः, which);

  prevline[which - 1] = पङ्क्तिः;
  प्रत्यागम true;
}

स्थिर शून्य
free_spareline (शून्य)
{
  प्रति (idx_t i = 0; i < countof (spareline); i++)
    {
      जइ (spareline[i])
        {
          freeline (spareline[i]);
          मुक्त (spareline[i]);
        }
    }
}

स्थिर शून्य
initseq (संरचना seq *seq)
{
  seq->गणनम् = 0;
  seq->alloc = 0;
  seq->पङ्क्तयः = रिक्त;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

स्थिर bool
getseq (FILE *fp, संरचना seq *seq, पूर्णाङ्क whichfile)
{
  जइ (seq->गणनम् == seq->alloc)
    {
      seq->पङ्क्तयः = xpalloc (seq->पङ्क्तयः, &seq->alloc, 1, -1, परिमाण *seq->पङ्क्तयः);
      प्रति (idx_t i = seq->गणनम्; i < seq->alloc; i++)
        seq->पङ्क्तयः[i] = रिक्त;
    }

  जइ (get_पङ्क्तिः (fp, &seq->पङ्क्तयः[seq->गणनम्], whichfile))
    {
      ++seq->गणनम्;
      प्रत्यागम true;
    }
  प्रत्यागम false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
स्थिर bool
advance_seq (FILE *fp, संरचना seq *seq, bool first, पूर्णाङ्क whichfile)
{
  जइ (first)
    seq->गणनम् = 0;

  प्रत्यागम getseq (fp, seq, whichfile);
}

स्थिर शून्य
delseq (संरचना seq *seq)
{
  प्रति (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->पङ्क्तयः[i]);
      मुक्त (seq->पङ्क्तयः[i]);
    }
  मुक्त (seq->पङ्क्तयः);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

स्थिर शून्य
prfield (idx_t n, संरचना पङ्क्तिः ध्रुव *पङ्क्तिः)
{
  जइ (n < पङ्क्तिः->nfields)
    {
      idx_t len = पङ्क्तिः->fields[n].len;
      जइ (len)
        fwrite (पङ्क्तिः->fields[n].beg, 1, len, stdout);
      अण्णहा जइ (empty_filler)
        fputs (empty_filler, stdout);
    }
  अण्णहा जइ (empty_filler)
    fputs (empty_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

स्थिर शून्य
prfields (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : पङ्क्तिः->nfields;

  प्रति (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, पङ्क्तिः);
    }
  प्रति (idx_t i = join_field + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, पङ्क्तिः);
    }
}

/* Print the join of LINE1 and LINE2.  */

स्थिर शून्य
prjoin (संरचना पङ्क्तिः ध्रुव *line1, संरचना पङ्क्तिः ध्रुव *line2)
{
  ध्रुव संरचना outlist *outlist;
  idx_t field;
  संरचना पङ्क्तिः ध्रुव *पङ्क्तिः;

  outlist = outlist_head.next;
  जइ (outlist)
    {
      ध्रुव संरचना outlist *o;

      o = outlist;
      जाव (true)
        {
          जइ (o->file == 0)
            {
              जइ (line1 == &uni_blank)
                {
                  पङ्क्तिः = line2;
                  field = join_field_2;
                }
              अण्णहा
                {
                  पङ्क्तिः = line1;
                  field = join_field_1;
                }
            }
          अण्णहा
            {
              पङ्क्तिः = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, पङ्क्तिः);
          o = o->next;
          जइ (o == रिक्त)
            भङ्ग;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      putchar (eolchar);
    }
  अण्णहा
    {
      जइ (line1 == &uni_blank)
        {
          पङ्क्तिः = line2;
          field = join_field_2;
        }
      अण्णहा
        {
          पङ्क्तिः = line1;
          field = join_field_1;
        }

      /* Output the join field.  */
      prfield (field, पङ्क्तिः);

      /* Output other fields.  */
      prfields (line1, join_field_1, autocount_1);
      prfields (line2, join_field_2, autocount_2);

      putchar (eolchar);
    }

  जइ (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

स्थिर शून्य
join (FILE *fp1, FILE *fp2)
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

  जइ (autoformat)
    {
      autocount_1 = seq1.गणनम् ? seq1.पङ्क्तयः[0]->nfields : 0;
      autocount_2 = seq2.गणनम् ? seq2.पङ्क्तयः[0]->nfields : 0;
    }

  जइ (join_header_पङ्क्तयः && (seq1.गणनम् || seq2.गणनम्))
    {
      संरचना पङ्क्तिः ध्रुव *hline1 = seq1.गणनम् ? seq1.पङ्क्तयः[0] : &uni_blank;
      संरचना पङ्क्तिः ध्रुव *hline2 = seq2.गणनम् ? seq2.पङ्क्तयः[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = रिक्त;
      prevline[1] = रिक्त;
      जइ (seq1.गणनम्)
        advance_seq (fp1, &seq1, true, 1);
      जइ (seq2.गणनम्)
        advance_seq (fp2, &seq2, true, 2);
    }

  जाव (seq1.गणनम् && seq2.गणनम्)
    {
      diff = keycmp (seq1.पङ्क्तयः[0], seq2.पङ्क्तयः[0],
                     join_field_1, join_field_2);
      जइ (diff < 0)
        {
          जइ (print_unpairables_1)
            prjoin (seq1.पङ्क्तयः[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          अनुवर्त;
        }
      जइ (diff > 0)
        {
          जइ (print_unpairables_2)
            prjoin (&uni_blank, seq2.पङ्क्तयः[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          अनुवर्त;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      करेहि
        जइ (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.गणनम्;
            भङ्ग;
          }
      जाव (!keycmp (seq1.पङ्क्तयः[seq1.गणनम् - 1], seq2.पङ्क्तयः[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      करेहि
        जइ (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.गणनम्;
            भङ्ग;
          }
      जाव (!keycmp (seq1.पङ्क्तयः[0], seq2.पङ्क्तयः[seq2.गणनम् - 1],
                      join_field_1, join_field_2));

      जइ (print_pairables)
        {
          प्रति (idx_t i = 0; i < seq1.गणनम् - 1; ++i)
            {
              प्रति (idx_t j = 0; j < seq2.गणनम् - 1; ++j)
                prjoin (seq1.पङ्क्तयः[i], seq2.पङ्क्तयः[j]);
            }
        }

      जइ (!eof1)
        {
          SWAPLINES (seq1.पङ्क्तयः[0], seq1.पङ्क्तयः[seq1.गणनम् - 1]);
          seq1.गणनम् = 1;
        }
      अण्णहा
        seq1.गणनम् = 0;

      जइ (!eof2)
        {
          SWAPLINES (seq2.पङ्क्तयः[0], seq2.पङ्क्तयः[seq2.गणनम् - 1]);
          seq2.गणनम् = 1;
        }
      अण्णहा
        seq2.गणनम् = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  संरचना पङ्क्तिः *पङ्क्तिः = रिक्त;
  bool checktail = false;

  जइ (check_निवेशः_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  जइ ((print_unpairables_1 || checktail) && seq1.गणनम्)
    {
      जइ (print_unpairables_1)
        prjoin (seq1.पङ्क्तयः[0], &uni_blank);
      जइ (seq2.गणनम्)
        seen_unpairable = true;
      जाव (get_पङ्क्तिः (fp1, &पङ्क्तिः, 1))
        {
          जइ (print_unpairables_1)
            prjoin (पङ्क्तिः, &uni_blank);
          जइ (issued_disorder_warning[0] && !print_unpairables_1)
            भङ्ग;
        }
    }

  जइ ((print_unpairables_2 || checktail) && seq2.गणनम्)
    {
      जइ (print_unpairables_2)
        prjoin (&uni_blank, seq2.पङ्क्तयः[0]);
      जइ (seq1.गणनम्)
        seen_unpairable = true;
      जाव (get_पङ्क्तिः (fp2, &पङ्क्तिः, 2))
        {
          जइ (print_unpairables_2)
            prjoin (&uni_blank, पङ्क्तिः);
          जइ (issued_disorder_warning[1] && !print_unpairables_2)
            भङ्ग;
        }
    }

  freeline (पङ्क्तिः);
  मुक्त (पङ्क्तिः);

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
  जइ (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  अण्णहा जइ (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

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
      जइ (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      भङ्ग;

    पक्ष '1':
    पक्ष '2':
      जइ (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_index = s[0] - '0';
      *field_index = string_to_join_field (s + 2);
      भङ्ग;

    स्वभाव:
      error (EXIT_FAILURE, 0,
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

  करेहि
    {
      पूर्णाङ्क file_index;
      idx_t field_index;
      वर्ण ध्रुव *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      जइ (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_index, &field_index);
      add_field (file_index, field_index);
    }
  जाव (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

स्थिर शून्य
set_join_field (ptrdiff_t *var, idx_t val)
{
  जइ (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
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
               पूर्णाङ्क operand_status[2], पूर्णाङ्क joption_गणनम्[2], पूर्णाङ्क *nfiles,
               पूर्णाङ्क *prev_optc_status, पूर्णाङ्क *optc_status)
{
  पूर्णाङ्क n = *nfiles;

  जइ (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      वर्ण *arg = names[op0];
      विकल्प (operand_status[op0])
        {
        पक्ष MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        पक्ष MIGHT_BE_J1_ARG:
          joption_गणनम्[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          भङ्ग;

        पक्ष MIGHT_BE_J2_ARG:
          joption_गणनम्[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          भङ्ग;

        पक्ष MIGHT_BE_O_ARG:
          add_field_list (arg);
          भङ्ग;
        }
      जइ (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  जइ (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc_status;
  पूर्णाङ्क prev_optc_status = MUST_BE_OPERAND;
  पूर्णाङ्क operand_status[2];
  पूर्णाङ्क joption_गणनम्[2] = { 0, 0 };
  FILE *fp1, *fp2;
  पूर्णाङ्क optc;
  पूर्णाङ्क nfiles = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  जाव ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
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
            long पूर्णाङ्क val;
            जइ (xstrtol (optarg, रिक्त, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            जइ (val == 1)
              print_unpairables_1 = true;
            अण्णहा
              print_unpairables_2 = true;
          }
          भङ्ग;

        पक्ष 'e':
          जइ (empty_filler && ! streq (empty_filler, optarg))
            error (EXIT_FAILURE, 0,
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
          जइ ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_गणनम्[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          अण्णहा
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          भङ्ग;

        पक्ष 'o':
          जइ (streq (optarg, "auto"))
            autoformat = true;
          अण्णहा
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          भङ्ग;

        पक्ष 't':
          {
            mcel_t newtab;
            जइ (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            अण्णहा जइ (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            अण्णहा
              {
                newtab = mcel_scanz (optarg);
                जइ (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            जइ (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          भङ्ग;

        पक्ष 'z':
          eolchar = 0;
          भङ्ग;

        पक्ष NOCHECK_ORDER_OPTION:
          check_निवेशः_order = CHECK_ORDER_DISABLED;
          भङ्ग;

        पक्ष CHECK_ORDER_OPTION:
          check_निवेशः_order = CHECK_ORDER_ENABLED;
          भङ्ग;

        पक्ष 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_गणनम्,
                         &nfiles, &prev_optc_status, &optc_status);
          भङ्ग;

        पक्ष HEADER_पङ्क्तिः_OPTION:
          join_header_पङ्क्तयः = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }

      prev_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  prev_optc_status = MUST_BE_OPERAND;
  जाव (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_गणनम्,
                   &nfiles, &prev_optc_status, &optc_status);

  जइ (nfiles != 2)
    {
      जइ (nfiles == 0)
        error (0, 0, _("missing operand"));
      अण्णहा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  प्रति (पूर्णाङ्क i = 0; i < 2; i++)
    जइ (joption_गणनम्[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  जइ (join_field_1 < 0)
    join_field_1 = 0;
  जइ (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? stdin : fopen (g_names[0], "r");
  जइ (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? stdin : fopen (g_names[1], "r");
  जइ (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));
  जइ (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  जइ (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  जइ (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));

  जइ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  अण्णहा
    प्रत्यागम EXIT_SUCCESS;
}
