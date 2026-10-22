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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "join"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Mike Haertel")

#ವ್ಯಾಖ್ಯಾನಿಸು join system_join

#ವ್ಯಾಖ್ಯಾನಿಸು SWAPLINES(a, b) ಮಾಡು { \
  ರಚನೆ ಸಾಲು *tmp = a; \
  a = b; \
  b = tmp; \
} ಆಗ (0);

/* An element of the list identifying which fields to print for each
   output line.  */
ರಚನೆ outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    ಪೂರ್ಣಾಂಕ file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t ಕ್ಷೇತ್ರ;

    ರಚನೆ outlist *ಮುಂದಿನ;
  };

/* A field of a line.  */
ರಚನೆ ಕ್ಷೇತ್ರ
  {
    ಅಕ್ಷರ *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
ರಚನೆ ಸಾಲು
  {
    ರಚನೆ linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    ರಚನೆ ಕ್ಷೇತ್ರ *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
ರಚನೆ seq
  {
    idx_t ಎಣಿಕೆ_2;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    ರಚನೆ ಸಾಲು **ಸಾಲುಗಳು COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
ಸ್ಥಿರ ರಚನೆ ಸಾಲು *prevline[2] = {ಖಾಲಿ, ಖಾಲಿ};

/* The number of lines read from each file.  */
ಸ್ಥಿರ uintmax_t ಸಾಲು_no[2] = {0, 0};

/* The input file names.  */
ಸ್ಥಿರ ಅಕ್ಷರ *g_ಹೆಸರುಗಳು[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
ಸ್ಥಿರ ರಚನೆ ಸಾಲು *spareline[2] = {ಖಾಲಿ, ಖಾಲಿ};

/* True if the LC_COLLATE locale is hard.  */
ಸ್ಥಿರ bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
ಸ್ಥಿರ bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
ಸ್ಥಿರ bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
ಸ್ಥಿರ bool ನೋಡಿದ_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ಸ್ಥಿರ bool issued_disorder_warning[2];

/* Empty output field filler.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಖಾಲಿಗಳು_filler;

/* Whether to ensure the same number of fields are output from each line.  */
ಸ್ಥಿರ bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
ಸ್ಥಿರ idx_t autocount_1;
ಸ್ಥಿರ idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
ಸ್ಥಿರ ptrdiff_t join_ಕ್ಷೇತ್ರ_1 = -1;
ಸ್ಥಿರ ptrdiff_t join_ಕ್ಷೇತ್ರ_2 = -1;

/* List of fields to print.  */
ಸ್ಥಿರ ರಚನೆ outlist outlist_ತಲೆ;

/* Last element in 'outlist', where a new element can be added.  */
ಸ್ಥಿರ ರಚನೆ outlist *outlist_ಕೊನೆ = &outlist_ತಲೆ;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
ಸ್ಥಿರ mcel_t tab;

/* The output separator to use, and its length in bytes.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *output_separator = " ";
ಸ್ಥಿರ idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
ಸ್ಥಿರ ಎಣಿಕೆ
  {
    CHECK_ಕ್ರಮ_DEFAULT,
    CHECK_ಕ್ರಮ_ENABLED,
    CHECK_ಕ್ರಮ_DISABLED
  } check_ಒಳಹರಿವು_ಕ್ರಮ;

ಎಣಿಕೆ
{
  CHECK_ಕ್ರಮ_OPTION = CHAR_MAX + 1,
  NOCHECK_ಕ್ರಮ_OPTION,
  ಶೀರ್ಷಿಕೆ_ಸಾಲು_OPTION
};


ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"ignore-case", no_ವಾದ, ಖಾಲಿ, 'i'},
  {"check-order", no_ವಾದ, ಖಾಲಿ, CHECK_ಕ್ರಮ_OPTION},
  {"nocheck-order", no_ವಾದ, ಖಾಲಿ, NOCHECK_ಕ್ರಮ_OPTION},
  {"zero-terminated", no_ವಾದ, ಖಾಲಿ, 'z'},
  {"header", no_ವಾದ, ಖಾಲಿ, ಶೀರ್ಷಿಕೆ_ಸಾಲು_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Used to print non-joining lines */
ಸ್ಥಿರ ರಚನೆ ಸಾಲು uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
ಸ್ಥಿರ bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
ಸ್ಥಿರ bool join_ಶೀರ್ಷಿಕೆ_ಸಾಲುಗಳು;

/* The character marking end of line. Default to \n. */
ಸ್ಥಿರ ಅಕ್ಷರ eolchar = '\n';

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
For each pair ದ ಒಳಹರಿವು ಸಾಲುಗಳು with identical join fields, ಲಿಖ a ಸಾಲು ಗಮ್ಯ\n\
standard output.  The ಪೂರ್ವನಿಯೋಜಿತ join ಕ್ಷೇತ್ರ is the ಮೊದಲ, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, ಪಠ standard ಒಳಹರಿವು.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable ಸಾಲುಗಳು from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding ಗಮ್ಯ FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (ಖಾಲಿಗಳು) ಒಳಹರಿವು fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-ಸ್ಥಿತಿ\n\
         ignore differences ಒಳ ಸ್ಥಿತಿ when comparing fields\n\
"));
      oputs (_("\
  -j ಕ್ಷೇತ್ರ_2\n\
         equivalent ಗಮ್ಯ '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT ಆಗ constructing output ಸಾಲು\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as ಒಳಹರಿವು and output ಕ್ಷೇತ್ರ separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -1 ಕ್ಷೇತ್ರ_2\n\
         join on this ಕ್ಷೇತ್ರ_2 ದ file 1\n\
"));
      oputs (_("\
  -2 ಕ್ಷೇತ್ರ_2\n\
         join on this ಕ್ಷೇತ್ರ_2 ದ file 2\n\
"));
      oputs (_("\
      --check-ಕ್ರಮ\n\
         check that the ಒಳಹರಿವು is correctly sorted, even\n\
         ಆದರೆ all ಒಳಹರಿವು ಸಾಲುಗಳು are pairable\n\
"));
      oputs (_("\
      --nocheck-ಕ್ರಮ\n\
         ಮಾಡು not check that the ಒಳಹರಿವು is correctly sorted\n\
"));
      oputs (_("\
      --ಶೀರ್ಷಿಕೆ\n\
         treat the ಮೊದಲ ಸಾಲು ಒಳ each file as ಕ್ಷೇತ್ರ ಶೀರ್ಷಿಕೆಗಳು,\n\
         print them without trying ಗಮ್ಯ pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
ಇಲ್ಲದಿದ್ದರೆ fields are separated by CHAR.  Any ಕ್ಷೇತ್ರ_2 is a ಕ್ಷೇತ್ರ ಸಂಖ್ಯೆ counted\n\
from 1.  FORMAT is one or ಇನ್ನಷ್ಟು comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join ಕ್ಷೇತ್ರ,\n\
the ಉಳಿದ fields from FILE1, the ಉಳಿದ fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the ಮೊದಲ\n\
ಸಾಲು ದ each file determines the ಸಂಖ್ಯೆ ದ fields output ಗಾಗಿ each ಸಾಲು.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"ವಿಂಗಡಿಸು -k 1b,1\" ಆದರೆ 'join' has no options,\n\
or use \"join -t ''\" ಆದರೆ 'sort' has no options.\n\
Comparisons honor the ನಿಯಮಗಳು specified by 'LC_COLLATE'.\n\
If the ಒಳಹರಿವು is not sorted and some ಸಾಲುಗಳು cannot be joined, a\n\
warning ಸಂದೇಶ_2 will be given.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಹೊರತೆಗೆ_ಕ್ಷೇತ್ರ (ರಚನೆ ಸಾಲು *ಸಾಲು, ಅಕ್ಷರ *ಕ್ಷೇತ್ರ, idx_t len)
{
  ಆದರೆ (ಸಾಲು->nfields >= ಸಾಲು->nfields_allocated)
    ಸಾಲು->fields = xpalloc (ಸಾಲು->fields, &ಸಾಲು->nfields_allocated, 1,
                            -1, ಗಾತ್ರ *ಸಾಲು->fields);
  idx_t ಸ್ಥಿರಾಂಕ ಕ್ಷೇತ್ರ_ಸೂಚ್ಯಂಕ = ಸಾಲು->nfields++;
  ಸಾಲು->fields[ಕ್ಷೇತ್ರ_ಸೂಚ್ಯಂಕ].beg = ಕ್ಷೇತ್ರ;
  ಸಾಲು->fields[ಕ್ಷೇತ್ರ_ಸೂಚ್ಯಂಕ].len = len;
}

ಸ್ಥಿರ bool
eq_tab (mcel_t g)
{
  ಹಿಂತಿರುಗಿ mcel_eq (g, tab);
}

ಸ್ಥಿರ bool
newline_or_blank (mcel_t g)
{
  ಹಿಂತಿರುಗಿ g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

ಸ್ಥಿರ ಶೂನ್ಯ
xfields (ರಚನೆ ಸಾಲು *ಸಾಲು)
{
  ಅಕ್ಷರ *ptr = ಸಾಲು->buf.ಬಫರ್;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lim = ptr + ಸಾಲು->buf.ಉದ್ದಳತೆ - 1;

  ಆದರೆ (ptr == lim)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (!tab.len)
    ಆಗ ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        ಅಕ್ಷರ *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        ಹೊರತೆಗೆ_ಕ್ಷೇತ್ರ (ಸಾಲು, ptr, sep - ptr);
        ptr = sep;
      }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (tab.ch != '\n')
        ಗಾಗಿ (ಅಕ್ಷರ *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_ಪರಿಶೀಲನೆ (sep, lim).len)
          ಹೊರತೆಗೆ_ಕ್ಷೇತ್ರ (ಸಾಲು, ptr, sep - ptr);

      ಹೊರತೆಗೆ_ಕ್ಷೇತ್ರ (ಸಾಲು, ptr, lim - ptr);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
freeline (ರಚನೆ ಸಾಲು *ಸಾಲು)
{
  ಆದರೆ (ಸಾಲು == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;
  ಬಿಡುಗಡೆ (ಸಾಲು->fields);
  ಸಾಲು->fields = ಖಾಲಿ;
  ಸಾಲು->nfields_allocated = 0;
  ಬಿಡುಗಡೆ (ಸಾಲು->buf.ಬಫರ್);
  ಸಾಲು->buf.ಬಫರ್ = ಖಾಲಿ;
  ಸಾಲು->buf.ಗಾತ್ರ_2 = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
keycmp (ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *line1, ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  ಅಕ್ಷರ *beg1;
  ಅಕ್ಷರ *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  ಪೂರ್ಣಾಂಕ diff;

  ಆದರೆ (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      beg1 = ಖಾಲಿ;
      len1 = 0;
    }

  ಆದರೆ (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      beg2 = ಖಾಲಿ;
      len2 = 0;
    }

  ಆದರೆ (len1 == 0)
    ಹಿಂತಿರುಗಿ len2 == 0 ? 0 : -1;
  ಆದರೆ (len2 == 0)
    ಹಿಂತಿರುಗಿ 1;

  ಆದರೆ (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (hard_LC_COLLATE)
        ಹಿಂತಿರುಗಿ xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  ಆದರೆ (diff)
    ಹಿಂತಿರುಗಿ diff;
  ಹಿಂತಿರುಗಿ _GL_CMP (len1, len2);
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
check_ಕ್ರಮ (ಸ್ಥಿರಾಂಕ ರಚನೆ ಸಾಲು *prev,
             ಸ್ಥಿರಾಂಕ ರಚನೆ ಸಾಲು *current,
             ಪೂರ್ಣಾಂಕ whatfile)
{
  ಆದರೆ (check_ಒಳಹರಿವು_ಕ್ರಮ != CHECK_ಕ್ರಮ_DISABLED
      && ((check_ಒಳಹರಿವು_ಕ್ರಮ == CHECK_ಕ್ರಮ_ENABLED) || ನೋಡಿದ_unpairable))
    {
      ಆದರೆ (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_ಕ್ಷೇತ್ರ = whatfile == 1 ? join_ಕ್ಷೇತ್ರ_1 : join_ಕ್ಷೇತ್ರ_2;
          ಆದರೆ (keycmp (prev, current, join_ಕ್ಷೇತ್ರ, join_ಕ್ಷೇತ್ರ) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.ಉದ್ದಳತೆ;
              ಆದರೆ (0 < len && current->buf.ಬಫರ್[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, len);

              error ((check_ಒಳಹರಿವು_ಕ್ರಮ == CHECK_ಕ್ರಮ_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_ಹೆಸರುಗಳು[whatfile - 1], ಸಾಲು_no[whatfile - 1],
                     (ಪೂರ್ಣಾಂಕ) len, current->buf.ಬಫರ್);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
reset_ಸಾಲು (ರಚನೆ ಸಾಲು *ಸಾಲು)
{
  ಸಾಲು->nfields = 0;
}

ಸ್ಥಿರ ರಚನೆ ಸಾಲು *
init_linep (ರಚನೆ ಸಾಲು **linep)
{
  ರಚನೆ ಸಾಲು *ಸಾಲು = xzalloc (ಗಾತ್ರ *ಸಾಲು);
  *linep = ಸಾಲು;
  ಹಿಂತಿರುಗಿ ಸಾಲು;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

ಸ್ಥಿರ bool
get_ಸಾಲು (FILE *fp, ರಚನೆ ಸಾಲು **linep, ಪೂರ್ಣಾಂಕ which)
{
  ರಚನೆ ಸಾಲು *ಸಾಲು = *linep;

  ಆದರೆ (ಸಾಲು == prevline[which - 1])
    {
      SWAPLINES (ಸಾಲು, spareline[which - 1]);
      *linep = ಸಾಲು;
    }

  ಆದರೆ (ಸಾಲು)
    reset_ಸಾಲು (ಸಾಲು);
  ಇಲ್ಲದಿದ್ದರೆ
    ಸಾಲು = init_linep (linep);

  ಆದರೆ (! readlinebuffer_delim (&ಸಾಲು->buf, fp, eolchar))
    {
      ಆದರೆ (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (ಸಾಲು);
      ಹಿಂತಿರುಗಿ false;
    }
  ++ಸಾಲು_no[which - 1];

  xfields (ಸಾಲು);

  ಆದರೆ (prevline[which - 1])
    check_ಕ್ರಮ (prevline[which - 1], ಸಾಲು, which);

  prevline[which - 1] = ಸಾಲು;
  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
free_spareline (ಶೂನ್ಯ)
{
  ಗಾಗಿ (idx_t i = 0; i < countof (spareline); i++)
    {
      ಆದರೆ (spareline[i])
        {
          freeline (spareline[i]);
          ಬಿಡುಗಡೆ (spareline[i]);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
initseq (ರಚನೆ seq *seq)
{
  seq->ಎಣಿಕೆ_2 = 0;
  seq->alloc = 0;
  seq->ಸಾಲುಗಳು = ಖಾಲಿ;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

ಸ್ಥಿರ bool
getseq (FILE *fp, ರಚನೆ seq *seq, ಪೂರ್ಣಾಂಕ whichfile)
{
  ಆದರೆ (seq->ಎಣಿಕೆ_2 == seq->alloc)
    {
      seq->ಸಾಲುಗಳು = xpalloc (seq->ಸಾಲುಗಳು, &seq->alloc, 1, -1, ಗಾತ್ರ *seq->ಸಾಲುಗಳು);
      ಗಾಗಿ (idx_t i = seq->ಎಣಿಕೆ_2; i < seq->alloc; i++)
        seq->ಸಾಲುಗಳು[i] = ಖಾಲಿ;
    }

  ಆದರೆ (get_ಸಾಲು (fp, &seq->ಸಾಲುಗಳು[seq->ಎಣಿಕೆ_2], whichfile))
    {
      ++seq->ಎಣಿಕೆ_2;
      ಹಿಂತಿರುಗಿ true;
    }
  ಹಿಂತಿರುಗಿ false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
ಸ್ಥಿರ bool
advance_seq (FILE *fp, ರಚನೆ seq *seq, bool ಮೊದಲ, ಪೂರ್ಣಾಂಕ whichfile)
{
  ಆದರೆ (ಮೊದಲ)
    seq->ಎಣಿಕೆ_2 = 0;

  ಹಿಂತಿರುಗಿ getseq (fp, seq, whichfile);
}

ಸ್ಥಿರ ಶೂನ್ಯ
delseq (ರಚನೆ seq *seq)
{
  ಗಾಗಿ (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->ಸಾಲುಗಳು[i]);
      ಬಿಡುಗಡೆ (seq->ಸಾಲುಗಳು[i]);
    }
  ಬಿಡುಗಡೆ (seq->ಸಾಲುಗಳು);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

ಸ್ಥಿರ ಶೂನ್ಯ
prfield (idx_t n, ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಸಾಲು)
{
  ಆದರೆ (n < ಸಾಲು->nfields)
    {
      idx_t len = ಸಾಲು->fields[n].len;
      ಆದರೆ (len)
        fwrite (ಸಾಲು->fields[n].beg, 1, len, stdout);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಖಾಲಿಗಳು_filler)
        fputs (ಖಾಲಿಗಳು_filler, stdout);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಖಾಲಿಗಳು_filler)
    fputs (ಖಾಲಿಗಳು_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

ಸ್ಥಿರ ಶೂನ್ಯ
prfields (ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಸಾಲು, idx_t join_ಕ್ಷೇತ್ರ, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : ಸಾಲು->nfields;

  ಗಾಗಿ (idx_t i = 0; i < join_ಕ್ಷೇತ್ರ && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, ಸಾಲು);
    }
  ಗಾಗಿ (idx_t i = join_ಕ್ಷೇತ್ರ + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, ಸಾಲು);
    }
}

/* Print the join of LINE1 and LINE2.  */

ಸ್ಥಿರ ಶೂನ್ಯ
prjoin (ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *line1, ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *line2)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ outlist *outlist;
  idx_t ಕ್ಷೇತ್ರ;
  ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಸಾಲು;

  outlist = outlist_ತಲೆ.ಮುಂದಿನ;
  ಆದರೆ (outlist)
    {
      ಸ್ಥಿರಾಂಕ ರಚನೆ outlist *o;

      o = outlist;
      ಆಗ (true)
        {
          ಆದರೆ (o->file == 0)
            {
              ಆದರೆ (line1 == &uni_blank)
                {
                  ಸಾಲು = line2;
                  ಕ್ಷೇತ್ರ = join_ಕ್ಷೇತ್ರ_2;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಸಾಲು = line1;
                  ಕ್ಷೇತ್ರ = join_ಕ್ಷೇತ್ರ_1;
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಸಾಲು = (o->file == 1 ? line1 : line2);
              ಕ್ಷೇತ್ರ = o->ಕ್ಷೇತ್ರ;
            }
          prfield (ಕ್ಷೇತ್ರ, ಸಾಲು);
          o = o->ಮುಂದಿನ;
          ಆದರೆ (o == ಖಾಲಿ)
            ನಿಲ್ಲಿಸು;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      ಅಕ್ಷರ_ಬರೆ (eolchar);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (line1 == &uni_blank)
        {
          ಸಾಲು = line2;
          ಕ್ಷೇತ್ರ = join_ಕ್ಷೇತ್ರ_2;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಸಾಲು = line1;
          ಕ್ಷೇತ್ರ = join_ಕ್ಷೇತ್ರ_1;
        }

      /* Output the join field.  */
      prfield (ಕ್ಷೇತ್ರ, ಸಾಲು);

      /* Output other fields.  */
      prfields (line1, join_ಕ್ಷೇತ್ರ_1, autocount_1);
      prfields (line2, join_ಕ್ಷೇತ್ರ_2, autocount_2);

      ಅಕ್ಷರ_ಬರೆ (eolchar);
    }

  ಆದರೆ (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

ಸ್ಥಿರ ಶೂನ್ಯ
join (FILE *fp1, FILE *fp2)
{
  ರಚನೆ seq seq1, seq2;
  ಪೂರ್ಣಾಂಕ diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  ಆದರೆ (autoformat)
    {
      autocount_1 = seq1.ಎಣಿಕೆ_2 ? seq1.ಸಾಲುಗಳು[0]->nfields : 0;
      autocount_2 = seq2.ಎಣಿಕೆ_2 ? seq2.ಸಾಲುಗಳು[0]->nfields : 0;
    }

  ಆದರೆ (join_ಶೀರ್ಷಿಕೆ_ಸಾಲುಗಳು && (seq1.ಎಣಿಕೆ_2 || seq2.ಎಣಿಕೆ_2))
    {
      ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *hline1 = seq1.ಎಣಿಕೆ_2 ? seq1.ಸಾಲುಗಳು[0] : &uni_blank;
      ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *hline2 = seq2.ಎಣಿಕೆ_2 ? seq2.ಸಾಲುಗಳು[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = ಖಾಲಿ;
      prevline[1] = ಖಾಲಿ;
      ಆದರೆ (seq1.ಎಣಿಕೆ_2)
        advance_seq (fp1, &seq1, true, 1);
      ಆದರೆ (seq2.ಎಣಿಕೆ_2)
        advance_seq (fp2, &seq2, true, 2);
    }

  ಆಗ (seq1.ಎಣಿಕೆ_2 && seq2.ಎಣಿಕೆ_2)
    {
      diff = keycmp (seq1.ಸಾಲುಗಳು[0], seq2.ಸಾಲುಗಳು[0],
                     join_ಕ್ಷೇತ್ರ_1, join_ಕ್ಷೇತ್ರ_2);
      ಆದರೆ (diff < 0)
        {
          ಆದರೆ (print_unpairables_1)
            prjoin (seq1.ಸಾಲುಗಳು[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          ನೋಡಿದ_unpairable = true;
          ಮುಂದುವರಿ;
        }
      ಆದರೆ (diff > 0)
        {
          ಆದರೆ (print_unpairables_2)
            prjoin (&uni_blank, seq2.ಸಾಲುಗಳು[0]);
          advance_seq (fp2, &seq2, true, 2);
          ನೋಡಿದ_unpairable = true;
          ಮುಂದುವರಿ;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      ಮಾಡು
        ಆದರೆ (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.ಎಣಿಕೆ_2;
            ನಿಲ್ಲಿಸು;
          }
      ಆಗ (!keycmp (seq1.ಸಾಲುಗಳು[seq1.ಎಣಿಕೆ_2 - 1], seq2.ಸಾಲುಗಳು[0],
                      join_ಕ್ಷೇತ್ರ_1, join_ಕ್ಷೇತ್ರ_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      ಮಾಡು
        ಆದರೆ (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.ಎಣಿಕೆ_2;
            ನಿಲ್ಲಿಸು;
          }
      ಆಗ (!keycmp (seq1.ಸಾಲುಗಳು[0], seq2.ಸಾಲುಗಳು[seq2.ಎಣಿಕೆ_2 - 1],
                      join_ಕ್ಷೇತ್ರ_1, join_ಕ್ಷೇತ್ರ_2));

      ಆದರೆ (print_pairables)
        {
          ಗಾಗಿ (idx_t i = 0; i < seq1.ಎಣಿಕೆ_2 - 1; ++i)
            {
              ಗಾಗಿ (idx_t j = 0; j < seq2.ಎಣಿಕೆ_2 - 1; ++j)
                prjoin (seq1.ಸಾಲುಗಳು[i], seq2.ಸಾಲುಗಳು[j]);
            }
        }

      ಆದರೆ (!eof1)
        {
          SWAPLINES (seq1.ಸಾಲುಗಳು[0], seq1.ಸಾಲುಗಳು[seq1.ಎಣಿಕೆ_2 - 1]);
          seq1.ಎಣಿಕೆ_2 = 1;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        seq1.ಎಣಿಕೆ_2 = 0;

      ಆದರೆ (!eof2)
        {
          SWAPLINES (seq2.ಸಾಲುಗಳು[0], seq2.ಸಾಲುಗಳು[seq2.ಎಣಿಕೆ_2 - 1]);
          seq2.ಎಣಿಕೆ_2 = 1;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        seq2.ಎಣಿಕೆ_2 = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  ರಚನೆ ಸಾಲು *ಸಾಲು = ಖಾಲಿ;
  bool checktail = false;

  ಆದರೆ (check_ಒಳಹರಿವು_ಕ್ರಮ != CHECK_ಕ್ರಮ_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  ಆದರೆ ((print_unpairables_1 || checktail) && seq1.ಎಣಿಕೆ_2)
    {
      ಆದರೆ (print_unpairables_1)
        prjoin (seq1.ಸಾಲುಗಳು[0], &uni_blank);
      ಆದರೆ (seq2.ಎಣಿಕೆ_2)
        ನೋಡಿದ_unpairable = true;
      ಆಗ (get_ಸಾಲು (fp1, &ಸಾಲು, 1))
        {
          ಆದರೆ (print_unpairables_1)
            prjoin (ಸಾಲು, &uni_blank);
          ಆದರೆ (issued_disorder_warning[0] && !print_unpairables_1)
            ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ ((print_unpairables_2 || checktail) && seq2.ಎಣಿಕೆ_2)
    {
      ಆದರೆ (print_unpairables_2)
        prjoin (&uni_blank, seq2.ಸಾಲುಗಳು[0]);
      ಆದರೆ (seq1.ಎಣಿಕೆ_2)
        ನೋಡಿದ_unpairable = true;
      ಆಗ (get_ಸಾಲು (fp2, &ಸಾಲು, 2))
        {
          ಆದರೆ (print_unpairables_2)
            prjoin (&uni_blank, ಸಾಲು);
          ಆದರೆ (issued_disorder_warning[1] && !print_unpairables_2)
            ನಿಲ್ಲಿಸು;
        }
    }

  freeline (ಸಾಲು);
  ಬಿಡುಗಡೆ (ಸಾಲು);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_ಕ್ಷೇತ್ರ (ಪೂರ್ಣಾಂಕ file, idx_t ಕ್ಷೇತ್ರ)
{
  ರಚನೆ outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || ಕ್ಷೇತ್ರ == 0);

  o = xmalloc (ಗಾತ್ರ *o);
  o->file = file;
  o->ಕ್ಷೇತ್ರ = ಕ್ಷೇತ್ರ;
  o->ಮುಂದಿನ = ಖಾಲಿ;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_ಕೊನೆ->ಮುಂದಿನ = o;
  outlist_ಕೊನೆ = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

ಸ್ಥಿರ idx_t
string_ಗಮ್ಯ_join_ಕ್ಷೇತ್ರ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, ಖಾಲಿ, 10, &val, "");
  ಆದರೆ (s_err == LONGINT_ಅತಿಪ್ರವಾಹ || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  ಹಿಂತಿರುಗಿ val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

ಸ್ಥಿರ ಶೂನ್ಯ
decode_ಕ್ಷೇತ್ರ_spec (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, ಪೂರ್ಣಾಂಕ *file_ಸೂಚ್ಯಂಕ, idx_t *ಕ್ಷೇತ್ರ_ಸೂಚ್ಯಂಕ)
{
  /* The first character must be 0, 1, or 2.  */
  ಆಯ್ಕೆ (s[0])
    {
    ಸ್ಥಿತಿ '0':
      ಆದರೆ (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_ಸೂಚ್ಯಂಕ = 0;
      *ಕ್ಷೇತ್ರ_ಸೂಚ್ಯಂಕ = 0;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ '1':
    ಸ್ಥಿತಿ '2':
      ಆದರೆ (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_ಸೂಚ್ಯಂಕ = s[0] - '0';
      *ಕ್ಷೇತ್ರ_ಸೂಚ್ಯಂಕ = string_ಗಮ್ಯ_join_ಕ್ಷೇತ್ರ (s + 2);
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

ಸ್ಥಿರ bool
comma_or_blank (mcel_t g)
{
  ಹಿಂತಿರುಗಿ g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_ಕ್ಷೇತ್ರ_list (ಅಕ್ಷರ *str)
{
  ಅಕ್ಷರ *p = str;

  ಮಾಡು
    {
      ಪೂರ್ಣಾಂಕ file_ಸೂಚ್ಯಂಕ;
      idx_t ಕ್ಷೇತ್ರ_ಸೂಚ್ಯಂಕ;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *spec_ಅಂಶ = p;
      p = skip_str_matching (spec_ಅಂಶ, comma_or_blank, false);
      ಆದರೆ (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_ಕ್ಷೇತ್ರ_spec (spec_ಅಂಶ, &file_ಸೂಚ್ಯಂಕ, &ಕ್ಷೇತ್ರ_ಸೂಚ್ಯಂಕ);
      ಕೂಡಿಸು_ಕ್ಷೇತ್ರ (file_ಸೂಚ್ಯಂಕ, ಕ್ಷೇತ್ರ_ಸೂಚ್ಯಂಕ);
    }
  ಆಗ (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_join_ಕ್ಷೇತ್ರ (ptrdiff_t *var, idx_t val)
{
  ಆದರೆ (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

ಎಣಿಕೆ ಕಾರ್ಯಾಂಶ_ಸ್ಥಿತಿ
  {
    /* This argument must be an operand, i.e., one of the files to be
       joined.  */
    MUST_BE_ಕಾರ್ಯಾಂಶ,

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
ಕೂಡಿಸು_file_name (ಅಕ್ಷರ *name, ಅಕ್ಷರ *ಹೆಸರುಗಳು[2],
               ಪೂರ್ಣಾಂಕ ಕಾರ್ಯಾಂಶ_ಸ್ಥಿತಿ[2], ಪೂರ್ಣಾಂಕ joption_ಎಣಿಕೆ[2], ಪೂರ್ಣಾಂಕ *nfiles,
               ಪೂರ್ಣಾಂಕ *prev_optc_ಸ್ಥಿತಿ, ಪೂರ್ಣಾಂಕ *optc_ಸ್ಥಿತಿ)
{
  ಪೂರ್ಣಾಂಕ n = *nfiles;

  ಆದರೆ (n == 2)
    {
      bool op0 = (ಕಾರ್ಯಾಂಶ_ಸ್ಥಿತಿ[0] == MUST_BE_ಕಾರ್ಯಾಂಶ);
      ಅಕ್ಷರ *arg = ಹೆಸರುಗಳು[op0];
      ಆಯ್ಕೆ (ಕಾರ್ಯಾಂಶ_ಸ್ಥಿತಿ[op0])
        {
        ಸ್ಥಿತಿ MUST_BE_ಕಾರ್ಯಾಂಶ:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        ಸ್ಥಿತಿ MIGHT_BE_J1_ARG:
          joption_ಎಣಿಕೆ[0]--;
          ಗಣ_join_ಕ್ಷೇತ್ರ (&join_ಕ್ಷೇತ್ರ_1, string_ಗಮ್ಯ_join_ಕ್ಷೇತ್ರ (arg));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ MIGHT_BE_J2_ARG:
          joption_ಎಣಿಕೆ[1]--;
          ಗಣ_join_ಕ್ಷೇತ್ರ (&join_ಕ್ಷೇತ್ರ_2, string_ಗಮ್ಯ_join_ಕ್ಷೇತ್ರ (arg));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ MIGHT_BE_O_ARG:
          ಕೂಡಿಸು_ಕ್ಷೇತ್ರ_list (arg);
          ನಿಲ್ಲಿಸು;
        }
      ಆದರೆ (!op0)
        {
          ಕಾರ್ಯಾಂಶ_ಸ್ಥಿತಿ[0] = ಕಾರ್ಯಾಂಶ_ಸ್ಥಿತಿ[1];
          ಹೆಸರುಗಳು[0] = ಹೆಸರುಗಳು[1];
        }
      n = 1;
    }

  ಕಾರ್ಯಾಂಶ_ಸ್ಥಿತಿ[n] = *prev_optc_ಸ್ಥಿತಿ;
  ಹೆಸರುಗಳು[n] = name;
  *nfiles = n + 1;
  ಆದರೆ (*prev_optc_ಸ್ಥಿತಿ == MIGHT_BE_O_ARG)
    *optc_ಸ್ಥಿತಿ = MIGHT_BE_O_ARG;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ optc_ಸ್ಥಿತಿ;
  ಪೂರ್ಣಾಂಕ prev_optc_ಸ್ಥಿತಿ = MUST_BE_ಕಾರ್ಯಾಂಶ;
  ಪೂರ್ಣಾಂಕ ಕಾರ್ಯಾಂಶ_ಸ್ಥಿತಿ[2];
  ಪೂರ್ಣಾಂಕ joption_ಎಣಿಕೆ[2] = { 0, 0 };
  FILE *fp1, *fp2;
  ಪೂರ್ಣಾಂಕ optc;
  ಪೂರ್ಣಾಂಕ nfiles = 0;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  ಆಗ ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, ಖಾಲಿ))
         != -1)
    {
      optc_ಸ್ಥಿತಿ = MUST_BE_ಕಾರ್ಯಾಂಶ;

      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'v':
            print_pairables = false;
            FALLTHROUGH;

        ಸ್ಥಿತಿ 'a':
          {
            ದೀರ್ಘ ಪೂರ್ಣಾಂಕ val;
            ಆದರೆ (xstrtol (optarg, ಖಾಲಿ, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            ಆದರೆ (val == 1)
              print_unpairables_1 = true;
            ಇಲ್ಲದಿದ್ದರೆ
              print_unpairables_2 = true;
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'e':
          ಆದರೆ (ಖಾಲಿಗಳು_filler && ! streq (ಖಾಲಿಗಳು_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          ಖಾಲಿಗಳು_filler = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'i':
          ignore_case = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '1':
          ಗಣ_join_ಕ್ಷೇತ್ರ (&join_ಕ್ಷೇತ್ರ_1, string_ಗಮ್ಯ_join_ಕ್ಷೇತ್ರ (optarg));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '2':
          ಗಣ_join_ಕ್ಷೇತ್ರ (&join_ಕ್ಷೇತ್ರ_2, string_ಗಮ್ಯ_join_ಕ್ಷೇತ್ರ (optarg));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'j':
          ಆದರೆ ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_ಎಣಿಕೆ[is_j2]++;
              optc_ಸ್ಥಿತಿ = MIGHT_BE_J1_ARG + is_j2;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಗಣ_join_ಕ್ಷೇತ್ರ (&join_ಕ್ಷೇತ್ರ_1, string_ಗಮ್ಯ_join_ಕ್ಷೇತ್ರ (optarg));
              ಗಣ_join_ಕ್ಷೇತ್ರ (&join_ಕ್ಷೇತ್ರ_2, join_ಕ್ಷೇತ್ರ_1);
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'o':
          ಆದರೆ (streq (optarg, "auto"))
            autoformat = true;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಕೂಡಿಸು_ಕ್ಷೇತ್ರ_list (optarg);
              optc_ಸ್ಥಿತಿ = MIGHT_BE_O_ARG;
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          {
            mcel_t newtab;
            ಆದರೆ (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            ಇಲ್ಲದಿದ್ದರೆ
              {
                newtab = mcel_scanz (optarg);
                ಆದರೆ (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            ಆದರೆ (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'z':
          eolchar = 0;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ NOCHECK_ಕ್ರಮ_OPTION:
          check_ಒಳಹರಿವು_ಕ್ರಮ = CHECK_ಕ್ರಮ_DISABLED;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ CHECK_ಕ್ರಮ_OPTION:
          check_ಒಳಹರಿವು_ಕ್ರಮ = CHECK_ಕ್ರಮ_ENABLED;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 1:		/* Non-option argument.  */
          ಕೂಡಿಸು_file_name (optarg, g_ಹೆಸರುಗಳು, ಕಾರ್ಯಾಂಶ_ಸ್ಥಿತಿ, joption_ಎಣಿಕೆ,
                         &nfiles, &prev_optc_ಸ್ಥಿತಿ, &optc_ಸ್ಥಿತಿ);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಶೀರ್ಷಿಕೆ_ಸಾಲು_OPTION:
          join_ಶೀರ್ಷಿಕೆ_ಸಾಲುಗಳು = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }

      prev_optc_ಸ್ಥಿತಿ = optc_ಸ್ಥಿತಿ;
    }

  /* Process any operands after "--".  */
  prev_optc_ಸ್ಥಿತಿ = MUST_BE_ಕಾರ್ಯಾಂಶ;
  ಆಗ (optind < argc)
    ಕೂಡಿಸು_file_name (argv[optind++], g_ಹೆಸರುಗಳು, ಕಾರ್ಯಾಂಶ_ಸ್ಥಿತಿ, joption_ಎಣಿಕೆ,
                   &nfiles, &prev_optc_ಸ್ಥಿತಿ, &optc_ಸ್ಥಿತಿ);

  ಆದರೆ (nfiles != 2)
    {
      ಆದರೆ (nfiles == 0)
        error (0, 0, _("missing operand"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2; i++)
    ಆದರೆ (joption_ಎಣಿಕೆ[i] != 0)
      {
        ಗಣ_join_ಕ್ಷೇತ್ರ (&join_ಕ್ಷೇತ್ರ_1, i);
        ಗಣ_join_ಕ್ಷೇತ್ರ (&join_ಕ್ಷೇತ್ರ_2, i);
      }

  ಆದರೆ (join_ಕ್ಷೇತ್ರ_1 < 0)
    join_ಕ್ಷೇತ್ರ_1 = 0;
  ಆದರೆ (join_ಕ್ಷೇತ್ರ_2 < 0)
    join_ಕ್ಷೇತ್ರ_2 = 0;

  fp1 = streq (g_ಹೆಸರುಗಳು[0], "-") ? stdin : fopen (g_ಹೆಸರುಗಳು[0], "r");
  ಆದರೆ (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_ಹೆಸರುಗಳು[0]));
  fp2 = streq (g_ಹೆಸರುಗಳು[1], "-") ? stdin : fopen (g_ಹೆಸರುಗಳು[1], "r");
  ಆದರೆ (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_ಹೆಸರುಗಳು[1]));
  ಆದರೆ (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  ಆದರೆ (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_ಹೆಸರುಗಳು[0]));
  ಆದರೆ (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_ಹೆಸರುಗಳು[1]));

  ಆದರೆ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
