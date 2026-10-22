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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "hard-locale.h"
#ಅನ್ತರ್ಭಾವ "linebuffer.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"
#ಅನ್ತರ್ಭಾವ "memcasecmp.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "skipchars.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "xmemcoll.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "join"

#define AUTHORS proper_name ("Mike Haertel")

#define join system_join

#define SWAPLINES(a, b) ಕುರು { \
  ಸಂರಚನಾ ಸಾಲು *tmp = a; \
  a = b; \
  b = tmp; \
} ಯಾವತ್ (0);

/* An element of the list identifying which fields to print for each
   output line.  */
ಸಂರಚನಾ outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    ಪೂರ್ಣಾಙ್ಕ file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t field;

    ಸಂರಚನಾ outlist *next;
  };

/* A field of a line.  */
ಸಂರಚನಾ field
  {
    ವರ್ಣ *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
ಸಂರಚನಾ ಸಾಲು
  {
    ಸಂರಚನಾ linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    ಸಂರಚನಾ field *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
ಸಂರಚನಾ seq
  {
    idx_t ಎಣಿಕೆ;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    ಸಂರಚನಾ ಸಾಲು **ಸಾಲುಗಳು COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
ಸ್ಥಿರ ಸಂರಚನಾ ಸಾಲು *prevline[2] = {ರಿಕ್ತ, ರಿಕ್ತ};

/* The number of lines read from each file.  */
ಸ್ಥಿರ uintmax_t ಸಾಲು_no[2] = {0, 0};

/* The input file names.  */
ಸ್ಥಿರ ವರ್ಣ *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
ಸ್ಥಿರ ಸಂರಚನಾ ಸಾಲು *spareline[2] = {ರಿಕ್ತ, ರಿಕ್ತ};

/* True if the LC_COLLATE locale is hard.  */
ಸ್ಥಿರ bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
ಸ್ಥಿರ bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
ಸ್ಥಿರ bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
ಸ್ಥಿರ bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ಸ್ಥಿರ bool issued_disorder_warning[2];

/* Empty output field filler.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *empty_filler;

/* Whether to ensure the same number of fields are output from each line.  */
ಸ್ಥಿರ bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
ಸ್ಥಿರ idx_t autocount_1;
ಸ್ಥಿರ idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
ಸ್ಥಿರ ptrdiff_t join_field_1 = -1;
ಸ್ಥಿರ ptrdiff_t join_field_2 = -1;

/* List of fields to print.  */
ಸ್ಥಿರ ಸಂರಚನಾ outlist outlist_head;

/* Last element in 'outlist', where a new element can be added.  */
ಸ್ಥಿರ ಸಂರಚನಾ outlist *outlist_end = &outlist_head;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
ಸ್ಥಿರ mcel_t tab;

/* The output separator to use, and its length in bytes.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *output_separator = " ";
ಸ್ಥಿರ idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
ಸ್ಥಿರ ಗಣನಾ
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_ಒಳಹರಿವು_order;

ಗಣನಾ
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_ಸಾಲು_OPTION
};


ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"ignore-case", no_argument, ರಿಕ್ತ, 'i'},
  {"check-order", no_argument, ರಿಕ್ತ, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, ರಿಕ್ತ, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, ರಿಕ್ತ, 'z'},
  {"header", no_argument, ರಿಕ್ತ, HEADER_ಸಾಲು_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Used to print non-joining lines */
ಸ್ಥಿರ ಸಂರಚನಾ ಸಾಲು uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
ಸ್ಥಿರ bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
ಸ್ಥಿರ bool join_header_ಸಾಲುಗಳು;

/* The character marking end of line. Default to \n. */
ಸ್ಥಿರ ವರ್ಣ eolchar = '\n';

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
For each pair of ಒಳಹರಿವು ಸಾಲುಗಳು with identical join fields, ಲಿಖ a ಸಾಲು to\n\
standard output.  The ಸ್ವಭಾವ join field is the first, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, ಪಠ standard ಒಳಹರಿವು.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable ಸಾಲುಗಳು from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding to FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (empty) ಒಳಹರಿವು fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-ಪಕ್ಷ\n\
         ignore differences in ಪಕ್ಷ when comparing fields\n\
"));
      oputs (_("\
  -j FIELD\n\
         equivalent to '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT ಯಾವತ್ constructing output ಸಾಲು\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as ಒಳಹರಿವು and output field separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output ಸಾಲುಗಳು\n\
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
         check that the ಒಳಹರಿವು is correctly sorted, even\n\
         ಯದಿ all ಒಳಹರಿವು ಸಾಲುಗಳು are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         ಕುರು not check that the ಒಳಹರಿವು is correctly sorted\n\
"));
      oputs (_("\
      --header\n\
         treat the first ಸಾಲು in each file as field headers,\n\
         print them without trying to pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
ಅನ್ಯಥಾ fields are separated by CHAR.  Any FIELD is a field ಸಂಖ್ಯೆ counted\n\
from 1.  FORMAT is one or more comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join field,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
ಸಾಲು of each file determines the ಸಂಖ್ಯೆ of fields output ಪ್ರತಿ each ಸಾಲು.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" ಯದಿ 'join' has no options,\n\
or use \"join -t ''\" ಯದಿ 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the ಒಳಹರಿವು is not sorted and some ಸಾಲುಗಳು cannot be joined, a\n\
warning message will be given.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

ಸ್ಥಿರ ಶೂನ್ಯ
extract_field (ಸಂರಚನಾ ಸಾಲು *ಸಾಲು, ವರ್ಣ *field, idx_t len)
{
  ಯದಿ (ಸಾಲು->nfields >= ಸಾಲು->nfields_allocated)
    ಸಾಲು->fields = xpalloc (ಸಾಲು->fields, &ಸಾಲು->nfields_allocated, 1,
                            -1, ಪರಿಮಾಣ *ಸಾಲು->fields);
  idx_t ಧ್ರುವ field_index = ಸಾಲು->nfields++;
  ಸಾಲು->fields[field_index].beg = field;
  ಸಾಲು->fields[field_index].len = len;
}

ಸ್ಥಿರ bool
eq_tab (mcel_t g)
{
  ಪ್ರತ್ಯಾಗಮ mcel_eq (g, tab);
}

ಸ್ಥಿರ bool
newline_or_blank (mcel_t g)
{
  ಪ್ರತ್ಯಾಗಮ g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

ಸ್ಥಿರ ಶೂನ್ಯ
xfields (ಸಂರಚನಾ ಸಾಲು *ಸಾಲು)
{
  ವರ್ಣ *ptr = ಸಾಲು->buf.buffer;
  ವರ್ಣ ಧ್ರುವ *lim = ptr + ಸಾಲು->buf.length - 1;

  ಯದಿ (ptr == lim)
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (!tab.len)
    ಯಾವತ್ ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        ವರ್ಣ *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (ಸಾಲು, ptr, sep - ptr);
        ptr = sep;
      }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (tab.ch != '\n')
        ಪ್ರತಿ (ವರ್ಣ *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (ಸಾಲು, ptr, sep - ptr);

      extract_field (ಸಾಲು, ptr, lim - ptr);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
freeline (ಸಂರಚನಾ ಸಾಲು *ಸಾಲು)
{
  ಯದಿ (ಸಾಲು == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;
  ಮುಕ್ತ (ಸಾಲು->fields);
  ಸಾಲು->fields = ರಿಕ್ತ;
  ಸಾಲು->nfields_allocated = 0;
  ಮುಕ್ತ (ಸಾಲು->buf.buffer);
  ಸಾಲು->buf.buffer = ರಿಕ್ತ;
  ಸಾಲು->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
keycmp (ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *line1, ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  ವರ್ಣ *beg1;
  ವರ್ಣ *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  ಪೂರ್ಣಾಙ್ಕ diff;

  ಯದಿ (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  ಅನ್ಯಥಾ
    {
      beg1 = ರಿಕ್ತ;
      len1 = 0;
    }

  ಯದಿ (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  ಅನ್ಯಥಾ
    {
      beg2 = ರಿಕ್ತ;
      len2 = 0;
    }

  ಯದಿ (len1 == 0)
    ಪ್ರತ್ಯಾಗಮ len2 == 0 ? 0 : -1;
  ಯದಿ (len2 == 0)
    ಪ್ರತ್ಯಾಗಮ 1;

  ಯದಿ (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (hard_LC_COLLATE)
        ಪ್ರತ್ಯಾಗಮ xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  ಯದಿ (diff)
    ಪ್ರತ್ಯಾಗಮ diff;
  ಪ್ರತ್ಯಾಗಮ _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

ಸ್ಥಿರ ಶೂನ್ಯ
check_order (ಧ್ರುವ ಸಂರಚನಾ ಸಾಲು *prev,
             ಧ್ರುವ ಸಂರಚನಾ ಸಾಲು *current,
             ಪೂರ್ಣಾಙ್ಕ whatfile)
{
  ಯದಿ (check_ಒಳಹರಿವು_order != CHECK_ORDER_DISABLED
      && ((check_ಒಳಹರಿವು_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      ಯದಿ (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          ಯದಿ (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              ಯದಿ (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (INT_MAX, len);

              error ((check_ಒಳಹರಿವು_order == CHECK_ORDER_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], ಸಾಲು_no[whatfile - 1],
                     (ಪೂರ್ಣಾಙ್ಕ) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
reset_ಸಾಲು (ಸಂರಚನಾ ಸಾಲು *ಸಾಲು)
{
  ಸಾಲು->nfields = 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ ಸಾಲು *
init_linep (ಸಂರಚನಾ ಸಾಲು **linep)
{
  ಸಂರಚನಾ ಸಾಲು *ಸಾಲು = xzalloc (ಪರಿಮಾಣ *ಸಾಲು);
  *linep = ಸಾಲು;
  ಪ್ರತ್ಯಾಗಮ ಸಾಲು;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

ಸ್ಥಿರ bool
get_ಸಾಲು (FILE *fp, ಸಂರಚನಾ ಸಾಲು **linep, ಪೂರ್ಣಾಙ್ಕ which)
{
  ಸಂರಚನಾ ಸಾಲು *ಸಾಲು = *linep;

  ಯದಿ (ಸಾಲು == prevline[which - 1])
    {
      SWAPLINES (ಸಾಲು, spareline[which - 1]);
      *linep = ಸಾಲು;
    }

  ಯದಿ (ಸಾಲು)
    reset_ಸಾಲು (ಸಾಲು);
  ಅನ್ಯಥಾ
    ಸಾಲು = init_linep (linep);

  ಯದಿ (! readlinebuffer_delim (&ಸಾಲು->buf, fp, eolchar))
    {
      ಯದಿ (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (ಸಾಲು);
      ಪ್ರತ್ಯಾಗಮ false;
    }
  ++ಸಾಲು_no[which - 1];

  xfields (ಸಾಲು);

  ಯದಿ (prevline[which - 1])
    check_order (prevline[which - 1], ಸಾಲು, which);

  prevline[which - 1] = ಸಾಲು;
  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
free_spareline (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (idx_t i = 0; i < countof (spareline); i++)
    {
      ಯದಿ (spareline[i])
        {
          freeline (spareline[i]);
          ಮುಕ್ತ (spareline[i]);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
initseq (ಸಂರಚನಾ seq *seq)
{
  seq->ಎಣಿಕೆ = 0;
  seq->alloc = 0;
  seq->ಸಾಲುಗಳು = ರಿಕ್ತ;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

ಸ್ಥಿರ bool
getseq (FILE *fp, ಸಂರಚನಾ seq *seq, ಪೂರ್ಣಾಙ್ಕ whichfile)
{
  ಯದಿ (seq->ಎಣಿಕೆ == seq->alloc)
    {
      seq->ಸಾಲುಗಳು = xpalloc (seq->ಸಾಲುಗಳು, &seq->alloc, 1, -1, ಪರಿಮಾಣ *seq->ಸಾಲುಗಳು);
      ಪ್ರತಿ (idx_t i = seq->ಎಣಿಕೆ; i < seq->alloc; i++)
        seq->ಸಾಲುಗಳು[i] = ರಿಕ್ತ;
    }

  ಯದಿ (get_ಸಾಲು (fp, &seq->ಸಾಲುಗಳು[seq->ಎಣಿಕೆ], whichfile))
    {
      ++seq->ಎಣಿಕೆ;
      ಪ್ರತ್ಯಾಗಮ true;
    }
  ಪ್ರತ್ಯಾಗಮ false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
ಸ್ಥಿರ bool
advance_seq (FILE *fp, ಸಂರಚನಾ seq *seq, bool first, ಪೂರ್ಣಾಙ್ಕ whichfile)
{
  ಯದಿ (first)
    seq->ಎಣಿಕೆ = 0;

  ಪ್ರತ್ಯಾಗಮ getseq (fp, seq, whichfile);
}

ಸ್ಥಿರ ಶೂನ್ಯ
delseq (ಸಂರಚನಾ seq *seq)
{
  ಪ್ರತಿ (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->ಸಾಲುಗಳು[i]);
      ಮುಕ್ತ (seq->ಸಾಲುಗಳು[i]);
    }
  ಮುಕ್ತ (seq->ಸಾಲುಗಳು);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

ಸ್ಥಿರ ಶೂನ್ಯ
prfield (idx_t n, ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *ಸಾಲು)
{
  ಯದಿ (n < ಸಾಲು->nfields)
    {
      idx_t len = ಸಾಲು->fields[n].len;
      ಯದಿ (len)
        fwrite (ಸಾಲು->fields[n].beg, 1, len, stdout);
      ಅನ್ಯಥಾ ಯದಿ (empty_filler)
        fputs (empty_filler, stdout);
    }
  ಅನ್ಯಥಾ ಯದಿ (empty_filler)
    fputs (empty_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

ಸ್ಥಿರ ಶೂನ್ಯ
prfields (ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *ಸಾಲು, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : ಸಾಲು->nfields;

  ಪ್ರತಿ (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, ಸಾಲು);
    }
  ಪ್ರತಿ (idx_t i = join_field + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, ಸಾಲು);
    }
}

/* Print the join of LINE1 and LINE2.  */

ಸ್ಥಿರ ಶೂನ್ಯ
prjoin (ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *line1, ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *line2)
{
  ಧ್ರುವ ಸಂರಚನಾ outlist *outlist;
  idx_t field;
  ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *ಸಾಲು;

  outlist = outlist_head.next;
  ಯದಿ (outlist)
    {
      ಧ್ರುವ ಸಂರಚನಾ outlist *o;

      o = outlist;
      ಯಾವತ್ (true)
        {
          ಯದಿ (o->file == 0)
            {
              ಯದಿ (line1 == &uni_blank)
                {
                  ಸಾಲು = line2;
                  field = join_field_2;
                }
              ಅನ್ಯಥಾ
                {
                  ಸಾಲು = line1;
                  field = join_field_1;
                }
            }
          ಅನ್ಯಥಾ
            {
              ಸಾಲು = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, ಸಾಲು);
          o = o->next;
          ಯದಿ (o == ರಿಕ್ತ)
            ಭಙ್ಗ;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      putchar (eolchar);
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (line1 == &uni_blank)
        {
          ಸಾಲು = line2;
          field = join_field_2;
        }
      ಅನ್ಯಥಾ
        {
          ಸಾಲು = line1;
          field = join_field_1;
        }

      /* Output the join field.  */
      prfield (field, ಸಾಲು);

      /* Output other fields.  */
      prfields (line1, join_field_1, autocount_1);
      prfields (line2, join_field_2, autocount_2);

      putchar (eolchar);
    }

  ಯದಿ (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

ಸ್ಥಿರ ಶೂನ್ಯ
join (FILE *fp1, FILE *fp2)
{
  ಸಂರಚನಾ seq seq1, seq2;
  ಪೂರ್ಣಾಙ್ಕ diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  ಯದಿ (autoformat)
    {
      autocount_1 = seq1.ಎಣಿಕೆ ? seq1.ಸಾಲುಗಳು[0]->nfields : 0;
      autocount_2 = seq2.ಎಣಿಕೆ ? seq2.ಸಾಲುಗಳು[0]->nfields : 0;
    }

  ಯದಿ (join_header_ಸಾಲುಗಳು && (seq1.ಎಣಿಕೆ || seq2.ಎಣಿಕೆ))
    {
      ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *hline1 = seq1.ಎಣಿಕೆ ? seq1.ಸಾಲುಗಳು[0] : &uni_blank;
      ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *hline2 = seq2.ಎಣಿಕೆ ? seq2.ಸಾಲುಗಳು[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = ರಿಕ್ತ;
      prevline[1] = ರಿಕ್ತ;
      ಯದಿ (seq1.ಎಣಿಕೆ)
        advance_seq (fp1, &seq1, true, 1);
      ಯದಿ (seq2.ಎಣಿಕೆ)
        advance_seq (fp2, &seq2, true, 2);
    }

  ಯಾವತ್ (seq1.ಎಣಿಕೆ && seq2.ಎಣಿಕೆ)
    {
      diff = keycmp (seq1.ಸಾಲುಗಳು[0], seq2.ಸಾಲುಗಳು[0],
                     join_field_1, join_field_2);
      ಯದಿ (diff < 0)
        {
          ಯದಿ (print_unpairables_1)
            prjoin (seq1.ಸಾಲುಗಳು[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          ಅನುವರ್ತ;
        }
      ಯದಿ (diff > 0)
        {
          ಯದಿ (print_unpairables_2)
            prjoin (&uni_blank, seq2.ಸಾಲುಗಳು[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          ಅನುವರ್ತ;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      ಕುರು
        ಯದಿ (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.ಎಣಿಕೆ;
            ಭಙ್ಗ;
          }
      ಯಾವತ್ (!keycmp (seq1.ಸಾಲುಗಳು[seq1.ಎಣಿಕೆ - 1], seq2.ಸಾಲುಗಳು[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      ಕುರು
        ಯದಿ (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.ಎಣಿಕೆ;
            ಭಙ್ಗ;
          }
      ಯಾವತ್ (!keycmp (seq1.ಸಾಲುಗಳು[0], seq2.ಸಾಲುಗಳು[seq2.ಎಣಿಕೆ - 1],
                      join_field_1, join_field_2));

      ಯದಿ (print_pairables)
        {
          ಪ್ರತಿ (idx_t i = 0; i < seq1.ಎಣಿಕೆ - 1; ++i)
            {
              ಪ್ರತಿ (idx_t j = 0; j < seq2.ಎಣಿಕೆ - 1; ++j)
                prjoin (seq1.ಸಾಲುಗಳು[i], seq2.ಸಾಲುಗಳು[j]);
            }
        }

      ಯದಿ (!eof1)
        {
          SWAPLINES (seq1.ಸಾಲುಗಳು[0], seq1.ಸಾಲುಗಳು[seq1.ಎಣಿಕೆ - 1]);
          seq1.ಎಣಿಕೆ = 1;
        }
      ಅನ್ಯಥಾ
        seq1.ಎಣಿಕೆ = 0;

      ಯದಿ (!eof2)
        {
          SWAPLINES (seq2.ಸಾಲುಗಳು[0], seq2.ಸಾಲುಗಳು[seq2.ಎಣಿಕೆ - 1]);
          seq2.ಎಣಿಕೆ = 1;
        }
      ಅನ್ಯಥಾ
        seq2.ಎಣಿಕೆ = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  ಸಂರಚನಾ ಸಾಲು *ಸಾಲು = ರಿಕ್ತ;
  bool checktail = false;

  ಯದಿ (check_ಒಳಹರಿವು_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  ಯದಿ ((print_unpairables_1 || checktail) && seq1.ಎಣಿಕೆ)
    {
      ಯದಿ (print_unpairables_1)
        prjoin (seq1.ಸಾಲುಗಳು[0], &uni_blank);
      ಯದಿ (seq2.ಎಣಿಕೆ)
        seen_unpairable = true;
      ಯಾವತ್ (get_ಸಾಲು (fp1, &ಸಾಲು, 1))
        {
          ಯದಿ (print_unpairables_1)
            prjoin (ಸಾಲು, &uni_blank);
          ಯದಿ (issued_disorder_warning[0] && !print_unpairables_1)
            ಭಙ್ಗ;
        }
    }

  ಯದಿ ((print_unpairables_2 || checktail) && seq2.ಎಣಿಕೆ)
    {
      ಯದಿ (print_unpairables_2)
        prjoin (&uni_blank, seq2.ಸಾಲುಗಳು[0]);
      ಯದಿ (seq1.ಎಣಿಕೆ)
        seen_unpairable = true;
      ಯಾವತ್ (get_ಸಾಲು (fp2, &ಸಾಲು, 2))
        {
          ಯದಿ (print_unpairables_2)
            prjoin (&uni_blank, ಸಾಲು);
          ಯದಿ (issued_disorder_warning[1] && !print_unpairables_2)
            ಭಙ್ಗ;
        }
    }

  freeline (ಸಾಲು);
  ಮುಕ್ತ (ಸಾಲು);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
add_field (ಪೂರ್ಣಾಙ್ಕ file, idx_t field)
{
  ಸಂರಚನಾ outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || field == 0);

  o = xmalloc (ಪರಿಮಾಣ *o);
  o->file = file;
  o->field = field;
  o->next = ರಿಕ್ತ;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_end->next = o;
  outlist_end = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

ಸ್ಥಿರ idx_t
string_to_join_field (ವರ್ಣ ಧ್ರುವ *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, ರಿಕ್ತ, 10, &val, "");
  ಯದಿ (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  ಅನ್ಯಥಾ ಯದಿ (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  ಪ್ರತ್ಯಾಗಮ val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

ಸ್ಥಿರ ಶೂನ್ಯ
decode_field_spec (ವರ್ಣ ಧ್ರುವ *s, ಪೂರ್ಣಾಙ್ಕ *file_index, idx_t *field_index)
{
  /* The first character must be 0, 1, or 2.  */
  ವಿಕಲ್ಪ (s[0])
    {
    ಪಕ್ಷ '0':
      ಯದಿ (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      ಭಙ್ಗ;

    ಪಕ್ಷ '1':
    ಪಕ್ಷ '2':
      ಯದಿ (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_index = s[0] - '0';
      *field_index = string_to_join_field (s + 2);
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

ಸ್ಥಿರ bool
comma_or_blank (mcel_t g)
{
  ಪ್ರತ್ಯಾಗಮ g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
add_field_list (ವರ್ಣ *str)
{
  ವರ್ಣ *p = str;

  ಕುರು
    {
      ಪೂರ್ಣಾಙ್ಕ file_index;
      idx_t field_index;
      ವರ್ಣ ಧ್ರುವ *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      ಯದಿ (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_index, &field_index);
      add_field (file_index, field_index);
    }
  ಯಾವತ್ (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

ಸ್ಥಿರ ಶೂನ್ಯ
set_join_field (ptrdiff_t *var, idx_t val)
{
  ಯದಿ (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

ಗಣನಾ operand_status
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

ಸ್ಥಿರ ಶೂನ್ಯ
add_file_name (ವರ್ಣ *name, ವರ್ಣ *names[2],
               ಪೂರ್ಣಾಙ್ಕ operand_status[2], ಪೂರ್ಣಾಙ್ಕ joption_ಎಣಿಕೆ[2], ಪೂರ್ಣಾಙ್ಕ *nfiles,
               ಪೂರ್ಣಾಙ್ಕ *prev_optc_status, ಪೂರ್ಣಾಙ್ಕ *optc_status)
{
  ಪೂರ್ಣಾಙ್ಕ n = *nfiles;

  ಯದಿ (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      ವರ್ಣ *arg = names[op0];
      ವಿಕಲ್ಪ (operand_status[op0])
        {
        ಪಕ್ಷ MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        ಪಕ್ಷ MIGHT_BE_J1_ARG:
          joption_ಎಣಿಕೆ[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          ಭಙ್ಗ;

        ಪಕ್ಷ MIGHT_BE_J2_ARG:
          joption_ಎಣಿಕೆ[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          ಭಙ್ಗ;

        ಪಕ್ಷ MIGHT_BE_O_ARG:
          add_field_list (arg);
          ಭಙ್ಗ;
        }
      ಯದಿ (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  ಯದಿ (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ optc_status;
  ಪೂರ್ಣಾಙ್ಕ prev_optc_status = MUST_BE_OPERAND;
  ಪೂರ್ಣಾಙ್ಕ operand_status[2];
  ಪೂರ್ಣಾಙ್ಕ joption_ಎಣಿಕೆ[2] = { 0, 0 };
  FILE *fp1, *fp2;
  ಪೂರ್ಣಾಙ್ಕ optc;
  ಪೂರ್ಣಾಙ್ಕ nfiles = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, ರಿಕ್ತ))
         != -1)
    {
      optc_status = MUST_BE_OPERAND;

      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'v':
            print_pairables = false;
            FALLTHROUGH;

        ಪಕ್ಷ 'a':
          {
            long ಪೂರ್ಣಾಙ್ಕ val;
            ಯದಿ (xstrtol (optarg, ರಿಕ್ತ, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            ಯದಿ (val == 1)
              print_unpairables_1 = true;
            ಅನ್ಯಥಾ
              print_unpairables_2 = true;
          }
          ಭಙ್ಗ;

        ಪಕ್ಷ 'e':
          ಯದಿ (empty_filler && ! streq (empty_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          empty_filler = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'i':
          ignore_case = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ '1':
          set_join_field (&join_field_1, string_to_join_field (optarg));
          ಭಙ್ಗ;

        ಪಕ್ಷ '2':
          set_join_field (&join_field_2, string_to_join_field (optarg));
          ಭಙ್ಗ;

        ಪಕ್ಷ 'j':
          ಯದಿ ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_ಎಣಿಕೆ[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          ಅನ್ಯಥಾ
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ 'o':
          ಯದಿ (streq (optarg, "auto"))
            autoformat = true;
          ಅನ್ಯಥಾ
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ 't':
          {
            mcel_t newtab;
            ಯದಿ (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            ಅನ್ಯಥಾ ಯದಿ (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            ಅನ್ಯಥಾ
              {
                newtab = mcel_scanz (optarg);
                ಯದಿ (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            ಯದಿ (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          ಭಙ್ಗ;

        ಪಕ್ಷ 'z':
          eolchar = 0;
          ಭಙ್ಗ;

        ಪಕ್ಷ NOCHECK_ORDER_OPTION:
          check_ಒಳಹರಿವು_order = CHECK_ORDER_DISABLED;
          ಭಙ್ಗ;

        ಪಕ್ಷ CHECK_ORDER_OPTION:
          check_ಒಳಹರಿವು_order = CHECK_ORDER_ENABLED;
          ಭಙ್ಗ;

        ಪಕ್ಷ 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_ಎಣಿಕೆ,
                         &nfiles, &prev_optc_status, &optc_status);
          ಭಙ್ಗ;

        ಪಕ್ಷ HEADER_ಸಾಲು_OPTION:
          join_header_ಸಾಲುಗಳು = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }

      prev_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  prev_optc_status = MUST_BE_OPERAND;
  ಯಾವತ್ (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_ಎಣಿಕೆ,
                   &nfiles, &prev_optc_status, &optc_status);

  ಯದಿ (nfiles != 2)
    {
      ಯದಿ (nfiles == 0)
        error (0, 0, _("missing operand"));
      ಅನ್ಯಥಾ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 2; i++)
    ಯದಿ (joption_ಎಣಿಕೆ[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  ಯದಿ (join_field_1 < 0)
    join_field_1 = 0;
  ಯದಿ (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? stdin : fopen (g_names[0], "r");
  ಯದಿ (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? stdin : fopen (g_names[1], "r");
  ಯದಿ (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));
  ಯದಿ (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  ಯದಿ (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  ಯದಿ (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));

  ಯದಿ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}
