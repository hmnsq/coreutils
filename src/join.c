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
#परिभाषय क्रमादेशः_NAME "join"

#परिभाषय AUTHORS proper_name ("Mike Haertel")

#परिभाषय join system_join

#परिभाषय SWAPLINES(a, b) कुरु { \
  संरचना पङ्क्तिः *tmp = a; \
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
    idx_t क्षेत्रम्;

    संरचना outlist *अग्र;
  };

/* A field of a line.  */
संरचना क्षेत्रम्
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
    संरचना क्षेत्रम् *fields COUNTED_BY (nfields);
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
स्थिर वर्ण *g_नामानि[2];

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
स्थिर bool आलोकितम्_unpairable;

/* If nonzero, we have warned about disorder in that file. */
स्थिर bool issued_disorder_warning[2];

/* Empty output field filler.  */
स्थिर वर्ण ध्रुव *रिक्तानि_filler;

/* Whether to ensure the same number of fields are output from each line.  */
स्थिर bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
स्थिर idx_t autocount_1;
स्थिर idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
स्थिर ptrdiff_t join_क्षेत्रम्_1 = -1;
स्थिर ptrdiff_t join_क्षेत्रम्_2 = -1;

/* List of fields to print.  */
स्थिर संरचना outlist outlist_शीर्ष;

/* Last element in 'outlist', where a new element can be added.  */
स्थिर संरचना outlist *outlist_अन्तः = &outlist_शीर्ष;

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
    CHECK_क्रमः_DEFAULT,
    CHECK_क्रमः_ENABLED,
    CHECK_क्रमः_DISABLED
  } check_निवेशः_क्रमः;

गणना
{
  CHECK_क्रमः_OPTION = CHAR_MAX + 1,
  NOCHECK_क्रमः_OPTION,
  शीर्षकम्_पङ्क्तिः_OPTION
};


स्थिर संरचना option ध्रुव longopts[] =
{
  {"ignore-case", no_तर्कः, रिक्त, 'i'},
  {"check-order", no_तर्कः, रिक्त, CHECK_क्रमः_OPTION},
  {"nocheck-order", no_तर्कः, रिक्त, NOCHECK_क्रमः_OPTION},
  {"zero-terminated", no_तर्कः, रिक्त, 'z'},
  {"header", no_तर्कः, रिक्त, शीर्षकम्_पङ्क्तिः_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Used to print non-joining lines */
स्थिर संरचना पङ्क्तिः uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
स्थिर bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
स्थिर bool join_शीर्षकम्_पङ्क्तयः;

/* The character marking end of line. Default to \n. */
स्थिर वर्ण eolchar = '\n';

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              क्रमादेशः_name);
      fputs (_("\
For each pair सम्बन्धि निवेशः पङ्क्तयः with identical join fields, लिख a पङ्क्तिः गन्तव्यम्\n\
standard output.  The स्वभाव join क्षेत्रम् is the first, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, पठ standard निवेशः.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable पङ्क्तयः from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding गन्तव्यम् FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (रिक्तानि) निवेशः fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-पक्ष\n\
         ignore differences अन्तर् पक्ष when comparing fields\n\
"));
      oputs (_("\
  -j क्षेत्रम्_2\n\
         equivalent गन्तव्यम् '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT यावत् constructing output पङ्क्तिः\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as निवेशः and output क्षेत्रम् separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output पङ्क्तयः\n\
"));
      oputs (_("\
  -1 क्षेत्रम्_2\n\
         join on this क्षेत्रम्_2 सम्बन्धि file 1\n\
"));
      oputs (_("\
  -2 क्षेत्रम्_2\n\
         join on this क्षेत्रम्_2 सम्बन्धि file 2\n\
"));
      oputs (_("\
      --check-क्रमः\n\
         check that the निवेशः is correctly sorted, even\n\
         यदि all निवेशः पङ्क्तयः are pairable\n\
"));
      oputs (_("\
      --nocheck-क्रमः\n\
         कुरु not check that the निवेशः is correctly sorted\n\
"));
      oputs (_("\
      --शीर्षकम्\n\
         treat the first पङ्क्तिः अन्तर् each file as क्षेत्रम् शीर्षकाणि,\n\
         print them without trying गन्तव्यम् pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
अन्यथा fields are separated by CHAR.  Any क्षेत्रम्_2 is a क्षेत्रम् सङ्ख्या counted\n\
from 1.  FORMAT is one or अधिकम् comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join क्षेत्रम्,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
पङ्क्तिः सम्बन्धि each file determines the सङ्ख्या सम्बन्धि fields output प्रति each पङ्क्तिः.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"क्रमय -k 1b,1\" यदि 'join' has no options,\n\
or use \"join -t ''\" यदि 'sort' has no options.\n\
Comparisons honor the नियमाः specified by 'LC_COLLATE'.\n\
If the निवेशः is not sorted and some पङ्क्तयः cannot be joined, a\n\
warning सन्देशः_2 will be given.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

स्थिर शून्य
extract_क्षेत्रम् (संरचना पङ्क्तिः *पङ्क्तिः, वर्ण *क्षेत्रम्, idx_t len)
{
  यदि (पङ्क्तिः->nfields >= पङ्क्तिः->nfields_allocated)
    पङ्क्तिः->fields = xpalloc (पङ्क्तिः->fields, &पङ्क्तिः->nfields_allocated, 1,
                            -1, परिमाण *पङ्क्तिः->fields);
  idx_t ध्रुव क्षेत्रम्_सूचकांक = पङ्क्तिः->nfields++;
  पङ्क्तिः->fields[क्षेत्रम्_सूचकांक].beg = क्षेत्रम्;
  पङ्क्तिः->fields[क्षेत्रम्_सूचकांक].len = len;
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
  वर्ण *ptr = पङ्क्तिः->buf.बफर;
  वर्ण ध्रुव *lim = ptr + पङ्क्तिः->buf.दीर्घता - 1;

  यदि (ptr == lim)
    प्रत्यागम;

  यदि (!tab.len)
    यावत् ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        वर्ण *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_क्षेत्रम् (पङ्क्तिः, ptr, sep - ptr);
        ptr = sep;
      }
  अन्यथा
    {
      यदि (tab.ch != '\n')
        प्रति (वर्ण *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_अवलोकनम् (sep, lim).len)
          extract_क्षेत्रम् (पङ्क्तिः, ptr, sep - ptr);

      extract_क्षेत्रम् (पङ्क्तिः, ptr, lim - ptr);
    }
}

स्थिर शून्य
freeline (संरचना पङ्क्तिः *पङ्क्तिः)
{
  यदि (पङ्क्तिः == रिक्त)
    प्रत्यागम;
  मुक्त (पङ्क्तिः->fields);
  पङ्क्तिः->fields = रिक्त;
  पङ्क्तिः->nfields_allocated = 0;
  मुक्त (पङ्क्तिः->buf.बफर);
  पङ्क्तिः->buf.बफर = रिक्त;
  पङ्क्तिः->buf.आकार = 0;
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
      diff = memcmp (beg1, beg2, MIN (len1, len2));
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
check_क्रमः (ध्रुव संरचना पङ्क्तिः *पूर्व,
             ध्रुव संरचना पङ्क्तिः *current,
             पूर्णाङ्क whatfile)
{
  यदि (check_निवेशः_क्रमः != CHECK_क्रमः_DISABLED
      && ((check_निवेशः_क्रमः == CHECK_क्रमः_ENABLED) || आलोकितम्_unpairable))
    {
      यदि (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_क्षेत्रम् = whatfile == 1 ? join_क्षेत्रम्_1 : join_क्षेत्रम्_2;
          यदि (keycmp (पूर्व, current, join_क्षेत्रम्, join_क्षेत्रम्) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.दीर्घता;
              यदि (0 < len && current->buf.बफर[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (अधिकतम_पूर्णाङ्कः, len);

              error ((check_निवेशः_क्रमः == CHECK_क्रमः_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_नामानि[whatfile - 1], पङ्क्तिः_no[whatfile - 1],
                     (पूर्णाङ्क) len, current->buf.बफर);

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

  यदि (पङ्क्तिः == prevline[which - 1])
    {
      SWAPLINES (पङ्क्तिः, spareline[which - 1]);
      *linep = पङ्क्तिः;
    }

  यदि (पङ्क्तिः)
    reset_पङ्क्तिः (पङ्क्तिः);
  अन्यथा
    पङ्क्तिः = init_linep (linep);

  यदि (! readlinebuffer_delim (&पङ्क्तिः->buf, fp, eolchar))
    {
      यदि (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (पङ्क्तिः);
      प्रत्यागम false;
    }
  ++पङ्क्तिः_no[which - 1];

  xfields (पङ्क्तिः);

  यदि (prevline[which - 1])
    check_क्रमः (prevline[which - 1], पङ्क्तिः, which);

  prevline[which - 1] = पङ्क्तिः;
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
  seq->गणनम् = 0;
  seq->alloc = 0;
  seq->पङ्क्तयः = रिक्त;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

स्थिर bool
getseq (FILE *fp, संरचना seq *seq, पूर्णाङ्क whichfile)
{
  यदि (seq->गणनम् == seq->alloc)
    {
      seq->पङ्क्तयः = xpalloc (seq->पङ्क्तयः, &seq->alloc, 1, -1, परिमाण *seq->पङ्क्तयः);
      प्रति (idx_t i = seq->गणनम्; i < seq->alloc; i++)
        seq->पङ्क्तयः[i] = रिक्त;
    }

  यदि (get_पङ्क्तिः (fp, &seq->पङ्क्तयः[seq->गणनम्], whichfile))
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
  यदि (first)
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
  यदि (n < पङ्क्तिः->nfields)
    {
      idx_t len = पङ्क्तिः->fields[n].len;
      यदि (len)
        fwrite (पङ्क्तिः->fields[n].beg, 1, len, stdout);
      अन्यथा यदि (रिक्तानि_filler)
        fputs (रिक्तानि_filler, stdout);
    }
  अन्यथा यदि (रिक्तानि_filler)
    fputs (रिक्तानि_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

स्थिर शून्य
prfields (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, idx_t join_क्षेत्रम्, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : पङ्क्तिः->nfields;

  प्रति (idx_t i = 0; i < join_क्षेत्रम् && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, पङ्क्तिः);
    }
  प्रति (idx_t i = join_क्षेत्रम् + 1; i < nfields; ++i)
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
  idx_t क्षेत्रम्;
  संरचना पङ्क्तिः ध्रुव *पङ्क्तिः;

  outlist = outlist_शीर्ष.अग्र;
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
                  पङ्क्तिः = line2;
                  क्षेत्रम् = join_क्षेत्रम्_2;
                }
              अन्यथा
                {
                  पङ्क्तिः = line1;
                  क्षेत्रम् = join_क्षेत्रम्_1;
                }
            }
          अन्यथा
            {
              पङ्क्तिः = (o->file == 1 ? line1 : line2);
              क्षेत्रम् = o->क्षेत्रम्;
            }
          prfield (क्षेत्रम्, पङ्क्तिः);
          o = o->अग्र;
          यदि (o == रिक्त)
            भङ्ग;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      अक्षरं_लिख (eolchar);
    }
  अन्यथा
    {
      यदि (line1 == &uni_blank)
        {
          पङ्क्तिः = line2;
          क्षेत्रम् = join_क्षेत्रम्_2;
        }
      अन्यथा
        {
          पङ्क्तिः = line1;
          क्षेत्रम् = join_क्षेत्रम्_1;
        }

      /* Output the join field.  */
      prfield (क्षेत्रम्, पङ्क्तिः);

      /* Output other fields.  */
      prfields (line1, join_क्षेत्रम्_1, autocount_1);
      prfields (line2, join_क्षेत्रम्_2, autocount_2);

      अक्षरं_लिख (eolchar);
    }

  यदि (ferror (stdout))
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

  यदि (autoformat)
    {
      autocount_1 = seq1.गणनम् ? seq1.पङ्क्तयः[0]->nfields : 0;
      autocount_2 = seq2.गणनम् ? seq2.पङ्क्तयः[0]->nfields : 0;
    }

  यदि (join_शीर्षकम्_पङ्क्तयः && (seq1.गणनम् || seq2.गणनम्))
    {
      संरचना पङ्क्तिः ध्रुव *hline1 = seq1.गणनम् ? seq1.पङ्क्तयः[0] : &uni_blank;
      संरचना पङ्क्तिः ध्रुव *hline2 = seq2.गणनम् ? seq2.पङ्क्तयः[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = रिक्त;
      prevline[1] = रिक्त;
      यदि (seq1.गणनम्)
        advance_seq (fp1, &seq1, true, 1);
      यदि (seq2.गणनम्)
        advance_seq (fp2, &seq2, true, 2);
    }

  यावत् (seq1.गणनम् && seq2.गणनम्)
    {
      diff = keycmp (seq1.पङ्क्तयः[0], seq2.पङ्क्तयः[0],
                     join_क्षेत्रम्_1, join_क्षेत्रम्_2);
      यदि (diff < 0)
        {
          यदि (print_unpairables_1)
            prjoin (seq1.पङ्क्तयः[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          आलोकितम्_unpairable = true;
          अनुवर्त;
        }
      यदि (diff > 0)
        {
          यदि (print_unpairables_2)
            prjoin (&uni_blank, seq2.पङ्क्तयः[0]);
          advance_seq (fp2, &seq2, true, 2);
          आलोकितम्_unpairable = true;
          अनुवर्त;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      कुरु
        यदि (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.गणनम्;
            भङ्ग;
          }
      यावत् (!keycmp (seq1.पङ्क्तयः[seq1.गणनम् - 1], seq2.पङ्क्तयः[0],
                      join_क्षेत्रम्_1, join_क्षेत्रम्_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      कुरु
        यदि (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.गणनम्;
            भङ्ग;
          }
      यावत् (!keycmp (seq1.पङ्क्तयः[0], seq2.पङ्क्तयः[seq2.गणनम् - 1],
                      join_क्षेत्रम्_1, join_क्षेत्रम्_2));

      यदि (print_pairables)
        {
          प्रति (idx_t i = 0; i < seq1.गणनम् - 1; ++i)
            {
              प्रति (idx_t j = 0; j < seq2.गणनम् - 1; ++j)
                prjoin (seq1.पङ्क्तयः[i], seq2.पङ्क्तयः[j]);
            }
        }

      यदि (!eof1)
        {
          SWAPLINES (seq1.पङ्क्तयः[0], seq1.पङ्क्तयः[seq1.गणनम् - 1]);
          seq1.गणनम् = 1;
        }
      अन्यथा
        seq1.गणनम् = 0;

      यदि (!eof2)
        {
          SWAPLINES (seq2.पङ्क्तयः[0], seq2.पङ्क्तयः[seq2.गणनम् - 1]);
          seq2.गणनम् = 1;
        }
      अन्यथा
        seq2.गणनम् = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  संरचना पङ्क्तिः *पङ्क्तिः = रिक्त;
  bool checktail = false;

  यदि (check_निवेशः_क्रमः != CHECK_क्रमः_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  यदि ((print_unpairables_1 || checktail) && seq1.गणनम्)
    {
      यदि (print_unpairables_1)
        prjoin (seq1.पङ्क्तयः[0], &uni_blank);
      यदि (seq2.गणनम्)
        आलोकितम्_unpairable = true;
      यावत् (get_पङ्क्तिः (fp1, &पङ्क्तिः, 1))
        {
          यदि (print_unpairables_1)
            prjoin (पङ्क्तिः, &uni_blank);
          यदि (issued_disorder_warning[0] && !print_unpairables_1)
            भङ्ग;
        }
    }

  यदि ((print_unpairables_2 || checktail) && seq2.गणनम्)
    {
      यदि (print_unpairables_2)
        prjoin (&uni_blank, seq2.पङ्क्तयः[0]);
      यदि (seq1.गणनम्)
        आलोकितम्_unpairable = true;
      यावत् (get_पङ्क्तिः (fp2, &पङ्क्तिः, 2))
        {
          यदि (print_unpairables_2)
            prjoin (&uni_blank, पङ्क्तिः);
          यदि (issued_disorder_warning[1] && !print_unpairables_2)
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
सङ्कलय_क्षेत्रम् (पूर्णाङ्क file, idx_t क्षेत्रम्)
{
  संरचना outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || क्षेत्रम् == 0);

  o = xmalloc (परिमाण *o);
  o->file = file;
  o->क्षेत्रम् = क्षेत्रम्;
  o->अग्र = रिक्त;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_अन्तः->अग्र = o;
  outlist_अन्तः = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

स्थिर idx_t
string_गन्तव्यम्_join_क्षेत्रम् (वर्ण ध्रुव *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, रिक्त, 10, &val, "");
  यदि (s_err == LONGINT_अतिप्रवाहः || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  अन्यथा यदि (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  प्रत्यागम val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

स्थिर शून्य
decode_क्षेत्रम्_spec (वर्ण ध्रुव *s, पूर्णाङ्क *file_सूचकांक, idx_t *क्षेत्रम्_सूचकांक)
{
  /* The first character must be 0, 1, or 2.  */
  विकल्प (s[0])
    {
    पक्ष '0':
      यदि (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_सूचकांक = 0;
      *क्षेत्रम्_सूचकांक = 0;
      भङ्ग;

    पक्ष '1':
    पक्ष '2':
      यदि (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_सूचकांक = s[0] - '0';
      *क्षेत्रम्_सूचकांक = string_गन्तव्यम्_join_क्षेत्रम् (s + 2);
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
सङ्कलय_क्षेत्रम्_list (वर्ण *str)
{
  वर्ण *p = str;

  कुरु
    {
      पूर्णाङ्क file_सूचकांक;
      idx_t क्षेत्रम्_सूचकांक;
      वर्ण ध्रुव *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      यदि (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_क्षेत्रम्_spec (spec_item, &file_सूचकांक, &क्षेत्रम्_सूचकांक);
      सङ्कलय_क्षेत्रम् (file_सूचकांक, क्षेत्रम्_सूचकांक);
    }
  यावत् (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

स्थिर शून्य
समुच्चयः_join_क्षेत्रम् (ptrdiff_t *var, idx_t val)
{
  यदि (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

गणना सङ्कार्यम्_स्थितिः
  {
    /* This argument must be an operand, i.e., one of the files to be
       joined.  */
    MUST_BE_सङ्कार्यम्,

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
सङ्कलय_file_name (वर्ण *name, वर्ण *नामानि[2],
               पूर्णाङ्क सङ्कार्यम्_स्थितिः[2], पूर्णाङ्क joption_गणनम्[2], पूर्णाङ्क *nfiles,
               पूर्णाङ्क *पूर्व_optc_स्थितिः, पूर्णाङ्क *optc_स्थितिः)
{
  पूर्णाङ्क n = *nfiles;

  यदि (n == 2)
    {
      bool op0 = (सङ्कार्यम्_स्थितिः[0] == MUST_BE_सङ्कार्यम्);
      वर्ण *arg = नामानि[op0];
      विकल्प (सङ्कार्यम्_स्थितिः[op0])
        {
        पक्ष MUST_BE_सङ्कार्यम्:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        पक्ष MIGHT_BE_J1_ARG:
          joption_गणनम्[0]--;
          समुच्चयः_join_क्षेत्रम् (&join_क्षेत्रम्_1, string_गन्तव्यम्_join_क्षेत्रम् (arg));
          भङ्ग;

        पक्ष MIGHT_BE_J2_ARG:
          joption_गणनम्[1]--;
          समुच्चयः_join_क्षेत्रम् (&join_क्षेत्रम्_2, string_गन्तव्यम्_join_क्षेत्रम् (arg));
          भङ्ग;

        पक्ष MIGHT_BE_O_ARG:
          सङ्कलय_क्षेत्रम्_list (arg);
          भङ्ग;
        }
      यदि (!op0)
        {
          सङ्कार्यम्_स्थितिः[0] = सङ्कार्यम्_स्थितिः[1];
          नामानि[0] = नामानि[1];
        }
      n = 1;
    }

  सङ्कार्यम्_स्थितिः[n] = *पूर्व_optc_स्थितिः;
  नामानि[n] = name;
  *nfiles = n + 1;
  यदि (*पूर्व_optc_स्थितिः == MIGHT_BE_O_ARG)
    *optc_स्थितिः = MIGHT_BE_O_ARG;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc_स्थितिः;
  पूर्णाङ्क पूर्व_optc_स्थितिः = MUST_BE_सङ्कार्यम्;
  पूर्णाङ्क सङ्कार्यम्_स्थितिः[2];
  पूर्णाङ्क joption_गणनम्[2] = { 0, 0 };
  FILE *fp1, *fp2;
  पूर्णाङ्क optc;
  पूर्णाङ्क nfiles = 0;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  यावत् ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, रिक्त))
         != -1)
    {
      optc_स्थितिः = MUST_BE_सङ्कार्यम्;

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
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            यदि (val == 1)
              print_unpairables_1 = true;
            अन्यथा
              print_unpairables_2 = true;
          }
          भङ्ग;

        पक्ष 'e':
          यदि (रिक्तानि_filler && ! streq (रिक्तानि_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          रिक्तानि_filler = optarg;
          भङ्ग;

        पक्ष 'i':
          ignore_case = true;
          भङ्ग;

        पक्ष '1':
          समुच्चयः_join_क्षेत्रम् (&join_क्षेत्रम्_1, string_गन्तव्यम्_join_क्षेत्रम् (optarg));
          भङ्ग;

        पक्ष '2':
          समुच्चयः_join_क्षेत्रम् (&join_क्षेत्रम्_2, string_गन्तव्यम्_join_क्षेत्रम् (optarg));
          भङ्ग;

        पक्ष 'j':
          यदि ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_गणनम्[is_j2]++;
              optc_स्थितिः = MIGHT_BE_J1_ARG + is_j2;
            }
          अन्यथा
            {
              समुच्चयः_join_क्षेत्रम् (&join_क्षेत्रम्_1, string_गन्तव्यम्_join_क्षेत्रम् (optarg));
              समुच्चयः_join_क्षेत्रम् (&join_क्षेत्रम्_2, join_क्षेत्रम्_1);
            }
          भङ्ग;

        पक्ष 'o':
          यदि (streq (optarg, "auto"))
            autoformat = true;
          अन्यथा
            {
              सङ्कलय_क्षेत्रम्_list (optarg);
              optc_स्थितिः = MIGHT_BE_O_ARG;
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
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            यदि (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          भङ्ग;

        पक्ष 'z':
          eolchar = 0;
          भङ्ग;

        पक्ष NOCHECK_क्रमः_OPTION:
          check_निवेशः_क्रमः = CHECK_क्रमः_DISABLED;
          भङ्ग;

        पक्ष CHECK_क्रमः_OPTION:
          check_निवेशः_क्रमः = CHECK_क्रमः_ENABLED;
          भङ्ग;

        पक्ष 1:		/* Non-option argument.  */
          सङ्कलय_file_name (optarg, g_नामानि, सङ्कार्यम्_स्थितिः, joption_गणनम्,
                         &nfiles, &पूर्व_optc_स्थितिः, &optc_स्थितिः);
          भङ्ग;

        पक्ष शीर्षकम्_पङ्क्तिः_OPTION:
          join_शीर्षकम्_पङ्क्तयः = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }

      पूर्व_optc_स्थितिः = optc_स्थितिः;
    }

  /* Process any operands after "--".  */
  पूर्व_optc_स्थितिः = MUST_BE_सङ्कार्यम्;
  यावत् (optind < argc)
    सङ्कलय_file_name (argv[optind++], g_नामानि, सङ्कार्यम्_स्थितिः, joption_गणनम्,
                   &nfiles, &पूर्व_optc_स्थितिः, &optc_स्थितिः);

  यदि (nfiles != 2)
    {
      यदि (nfiles == 0)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  प्रति (पूर्णाङ्क i = 0; i < 2; i++)
    यदि (joption_गणनम्[i] != 0)
      {
        समुच्चयः_join_क्षेत्रम् (&join_क्षेत्रम्_1, i);
        समुच्चयः_join_क्षेत्रम् (&join_क्षेत्रम्_2, i);
      }

  यदि (join_क्षेत्रम्_1 < 0)
    join_क्षेत्रम्_1 = 0;
  यदि (join_क्षेत्रम्_2 < 0)
    join_क्षेत्रम्_2 = 0;

  fp1 = streq (g_नामानि[0], "-") ? stdin : fopen (g_नामानि[0], "r");
  यदि (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_नामानि[0]));
  fp2 = streq (g_नामानि[1], "-") ? stdin : fopen (g_नामानि[1], "r");
  यदि (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_नामानि[1]));
  यदि (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  यदि (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_नामानि[0]));
  यदि (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_नामानि[1]));

  यदि (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  अन्यथा
    प्रत्यागम EXIT_SUCCESS;
}
