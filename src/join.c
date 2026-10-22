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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hard-locale.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "linebuffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mcel.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "memcasecmp.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "skipchars.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdio--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xmemcoll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "join"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Mike Haertel")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ join system_join

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SWAPLINES(a, b) ਕਰੋ { \
  ਬਣਤਰ ਸਤਰ *tmp = a; \
  a = b; \
  b = tmp; \
} ਜਦੋਂ (0);

/* An element of the list identifying which fields to print for each
   output line.  */
ਬਣਤਰ outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    ਪੂਰਨ file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t ਖੇਤਰ;

    ਬਣਤਰ outlist *ਅਗਲਾ;
  };

/* A field of a line.  */
ਬਣਤਰ ਖੇਤਰ
  {
    ਅੱਖਰ *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
ਬਣਤਰ ਸਤਰ
  {
    ਬਣਤਰ linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    ਬਣਤਰ ਖੇਤਰ *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
ਬਣਤਰ seq
  {
    idx_t ਗਿਣਤੀ_2;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    ਬਣਤਰ ਸਤਰ **ਸਤਰਾਂ COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
ਸਥਿਰ ਬਣਤਰ ਸਤਰ *prevline[2] = {ਨਿਰਾ, ਨਿਰਾ};

/* The number of lines read from each file.  */
ਸਥਿਰ uintmax_t ਸਤਰ_no[2] = {0, 0};

/* The input file names.  */
ਸਥਿਰ ਅੱਖਰ *g_ਨਾਂ[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
ਸਥਿਰ ਬਣਤਰ ਸਤਰ *spareline[2] = {ਨਿਰਾ, ਨਿਰਾ};

/* True if the LC_COLLATE locale is hard.  */
ਸਥਿਰ bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
ਸਥਿਰ bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
ਸਥਿਰ bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
ਸਥਿਰ bool ਵੇਖਿਆ_unpairable;

/* If nonzero, we have warned about disorder in that file. */
ਸਥਿਰ bool issued_disorder_warning[2];

/* Empty output field filler.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਖਾਲੀ_ਥਾਵਾਂ_filler;

/* Whether to ensure the same number of fields are output from each line.  */
ਸਥਿਰ bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
ਸਥਿਰ idx_t autocount_1;
ਸਥਿਰ idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
ਸਥਿਰ ptrdiff_t join_ਖੇਤਰ_1 = -1;
ਸਥਿਰ ptrdiff_t join_ਖੇਤਰ_2 = -1;

/* List of fields to print.  */
ਸਥਿਰ ਬਣਤਰ outlist outlist_ਸਿਰ;

/* Last element in 'outlist', where a new element can be added.  */
ਸਥਿਰ ਬਣਤਰ outlist *outlist_ਸਿਰਾ = &outlist_ਸਿਰ;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
ਸਥਿਰ mcel_t tab;

/* The output separator to use, and its length in bytes.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *output_separator = " ";
ਸਥਿਰ idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
ਸਥਿਰ ਗਿਣਤੀ
  {
    CHECK_ਕ੍ਰਮ_DEFAULT,
    CHECK_ਕ੍ਰਮ_ENABLED,
    CHECK_ਕ੍ਰਮ_DISABLED
  } check_ਨਿਵੇਸ਼_ਕ੍ਰਮ;

ਗਿਣਤੀ
{
  CHECK_ਕ੍ਰਮ_OPTION = CHAR_MAX + 1,
  NOCHECK_ਕ੍ਰਮ_OPTION,
  ਸਿਰਲੇਖ_ਸਤਰ_OPTION
};


ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"ignore-case", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"check-order", no_ਦਲੀਲ, ਨਿਰਾ, CHECK_ਕ੍ਰਮ_OPTION},
  {"nocheck-order", no_ਦਲੀਲ, ਨਿਰਾ, NOCHECK_ਕ੍ਰਮ_OPTION},
  {"zero-terminated", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {"header", no_ਦਲੀਲ, ਨਿਰਾ, ਸਿਰਲੇਖ_ਸਤਰ_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Used to print non-joining lines */
ਸਥਿਰ ਬਣਤਰ ਸਤਰ uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
ਸਥਿਰ bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
ਸਥਿਰ bool join_ਸਿਰਲੇਖ_ਸਤਰਾਂ;

/* The character marking end of line. Default to \n. */
ਸਥਿਰ ਅੱਖਰ eolchar = '\n';

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
For each pair ਦਾ ਨਿਵੇਸ਼ ਸਤਰਾਂ with identical join fields, ਲਿਖ a ਸਤਰ ਮੰਜ਼ਿਲ\n\
standard output.  The ਮੂਲ join ਖੇਤਰ is the ਪਹਿਲਾ, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, ਪਠ standard ਨਿਵੇਸ਼.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable ਸਤਰਾਂ from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding ਮੰਜ਼ਿਲ FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (ਖਾਲੀ_ਥਾਵਾਂ) ਨਿਵੇਸ਼ fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-ਹਾਲਤ\n\
         ignore differences ਵਿੱਚ ਹਾਲਤ when comparing fields\n\
"));
      oputs (_("\
  -j ਖੇਤਰ_2\n\
         equivalent ਮੰਜ਼ਿਲ '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT ਜਦੋਂ constructing output ਸਤਰ\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as ਨਿਵੇਸ਼ and output ਖੇਤਰ separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output ਸਤਰਾਂ\n\
"));
      oputs (_("\
  -1 ਖੇਤਰ_2\n\
         join on this ਖੇਤਰ_2 ਦਾ file 1\n\
"));
      oputs (_("\
  -2 ਖੇਤਰ_2\n\
         join on this ਖੇਤਰ_2 ਦਾ file 2\n\
"));
      oputs (_("\
      --check-ਕ੍ਰਮ\n\
         check that the ਨਿਵੇਸ਼ is correctly sorted, even\n\
         ਜੇ all ਨਿਵੇਸ਼ ਸਤਰਾਂ are pairable\n\
"));
      oputs (_("\
      --nocheck-ਕ੍ਰਮ\n\
         ਕਰੋ not check that the ਨਿਵੇਸ਼ is correctly sorted\n\
"));
      oputs (_("\
      --ਸਿਰਲੇਖ\n\
         treat the ਪਹਿਲਾ ਸਤਰ ਵਿੱਚ each file as ਖੇਤਰ ਸਿਰਲੇਖਾਂ,\n\
         print them without trying ਮੰਜ਼ਿਲ pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ਸਤਰ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
ਨਹੀਂਤਾਂ fields are separated by CHAR.  Any ਖੇਤਰ_2 is a ਖੇਤਰ ਸੰਖਿਆ counted\n\
from 1.  FORMAT is one or ਹੋਰ comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join ਖੇਤਰ,\n\
the ਬਚਿਆ fields from FILE1, the ਬਚਿਆ fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the ਪਹਿਲਾ\n\
ਸਤਰ ਦਾ each file determines the ਸੰਖਿਆ ਦਾ fields output ਲਈ each ਸਤਰ.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"ਛਾਂਟੋ -k 1b,1\" ਜੇ 'join' has no options,\n\
or use \"join -t ''\" ਜੇ 'sort' has no options.\n\
Comparisons honor the ਨਿਯਮਾਂ specified by 'LC_COLLATE'.\n\
If the ਨਿਵੇਸ਼ is not sorted and some ਸਤਰਾਂ cannot be joined, a\n\
warning ਸੁਨੇਹਾ_2 will be given.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

ਸਥਿਰ ਖਾਲੀ
ਕੱਢੋ_ਖੇਤਰ (ਬਣਤਰ ਸਤਰ *ਸਤਰ, ਅੱਖਰ *ਖੇਤਰ, idx_t len)
{
  ਜੇ (ਸਤਰ->nfields >= ਸਤਰ->nfields_allocated)
    ਸਤਰ->fields = xpalloc (ਸਤਰ->fields, &ਸਤਰ->nfields_allocated, 1,
                            -1, ਆਕਾਰ *ਸਤਰ->fields);
  idx_t ਅਚਲ ਖੇਤਰ_ਸੂਚਕਾਂਕ = ਸਤਰ->nfields++;
  ਸਤਰ->fields[ਖੇਤਰ_ਸੂਚਕਾਂਕ].beg = ਖੇਤਰ;
  ਸਤਰ->fields[ਖੇਤਰ_ਸੂਚਕਾਂਕ].len = len;
}

ਸਥਿਰ bool
eq_tab (mcel_t g)
{
  ਵਾਪਸ mcel_eq (g, tab);
}

ਸਥਿਰ bool
newline_or_blank (mcel_t g)
{
  ਵਾਪਸ g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

ਸਥਿਰ ਖਾਲੀ
xfields (ਬਣਤਰ ਸਤਰ *ਸਤਰ)
{
  ਅੱਖਰ *ptr = ਸਤਰ->buf.ਬਫਰ;
  ਅੱਖਰ ਅਚਲ *lim = ptr + ਸਤਰ->buf.ਲੰਮਾਈ - 1;

  ਜੇ (ptr == lim)
    ਵਾਪਸ;

  ਜੇ (!tab.len)
    ਜਦੋਂ ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        ਅੱਖਰ *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        ਕੱਢੋ_ਖੇਤਰ (ਸਤਰ, ptr, sep - ptr);
        ptr = sep;
      }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (tab.ch != '\n')
        ਲਈ (ਅੱਖਰ *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_ਜਾਂਚ (sep, lim).len)
          ਕੱਢੋ_ਖੇਤਰ (ਸਤਰ, ptr, sep - ptr);

      ਕੱਢੋ_ਖੇਤਰ (ਸਤਰ, ptr, lim - ptr);
    }
}

ਸਥਿਰ ਖਾਲੀ
freeline (ਬਣਤਰ ਸਤਰ *ਸਤਰ)
{
  ਜੇ (ਸਤਰ == ਨਿਰਾ)
    ਵਾਪਸ;
  ਮੁਕਤ (ਸਤਰ->fields);
  ਸਤਰ->fields = ਨਿਰਾ;
  ਸਤਰ->nfields_allocated = 0;
  ਮੁਕਤ (ਸਤਰ->buf.ਬਫਰ);
  ਸਤਰ->buf.ਬਫਰ = ਨਿਰਾ;
  ਸਤਰ->buf.ਮਾਪ = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

ਸਥਿਰ ਪੂਰਨ
keycmp (ਬਣਤਰ ਸਤਰ ਅਚਲ *line1, ਬਣਤਰ ਸਤਰ ਅਚਲ *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  ਅੱਖਰ *beg1;
  ਅੱਖਰ *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  ਪੂਰਨ diff;

  ਜੇ (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  ਨਹੀਂਤਾਂ
    {
      beg1 = ਨਿਰਾ;
      len1 = 0;
    }

  ਜੇ (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  ਨਹੀਂਤਾਂ
    {
      beg2 = ਨਿਰਾ;
      len2 = 0;
    }

  ਜੇ (len1 == 0)
    ਵਾਪਸ len2 == 0 ? 0 : -1;
  ਜੇ (len2 == 0)
    ਵਾਪਸ 1;

  ਜੇ (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (hard_LC_COLLATE)
        ਵਾਪਸ xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  ਜੇ (diff)
    ਵਾਪਸ diff;
  ਵਾਪਸ _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

ਸਥਿਰ ਖਾਲੀ
check_ਕ੍ਰਮ (ਅਚਲ ਬਣਤਰ ਸਤਰ *prev,
             ਅਚਲ ਬਣਤਰ ਸਤਰ *current,
             ਪੂਰਨ whatfile)
{
  ਜੇ (check_ਨਿਵੇਸ਼_ਕ੍ਰਮ != CHECK_ਕ੍ਰਮ_DISABLED
      && ((check_ਨਿਵੇਸ਼_ਕ੍ਰਮ == CHECK_ਕ੍ਰਮ_ENABLED) || ਵੇਖਿਆ_unpairable))
    {
      ਜੇ (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_ਖੇਤਰ = whatfile == 1 ? join_ਖੇਤਰ_1 : join_ਖੇਤਰ_2;
          ਜੇ (keycmp (prev, current, join_ਖੇਤਰ, join_ਖੇਤਰ) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.ਲੰਮਾਈ;
              ਜੇ (0 < len && current->buf.ਬਫਰ[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, len);

              error ((check_ਨਿਵੇਸ਼_ਕ੍ਰਮ == CHECK_ਕ੍ਰਮ_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_ਨਾਂ[whatfile - 1], ਸਤਰ_no[whatfile - 1],
                     (ਪੂਰਨ) len, current->buf.ਬਫਰ);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
reset_ਸਤਰ (ਬਣਤਰ ਸਤਰ *ਸਤਰ)
{
  ਸਤਰ->nfields = 0;
}

ਸਥਿਰ ਬਣਤਰ ਸਤਰ *
init_linep (ਬਣਤਰ ਸਤਰ **linep)
{
  ਬਣਤਰ ਸਤਰ *ਸਤਰ = xzalloc (ਆਕਾਰ *ਸਤਰ);
  *linep = ਸਤਰ;
  ਵਾਪਸ ਸਤਰ;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

ਸਥਿਰ bool
get_ਸਤਰ (FILE *fp, ਬਣਤਰ ਸਤਰ **linep, ਪੂਰਨ which)
{
  ਬਣਤਰ ਸਤਰ *ਸਤਰ = *linep;

  ਜੇ (ਸਤਰ == prevline[which - 1])
    {
      SWAPLINES (ਸਤਰ, spareline[which - 1]);
      *linep = ਸਤਰ;
    }

  ਜੇ (ਸਤਰ)
    reset_ਸਤਰ (ਸਤਰ);
  ਨਹੀਂਤਾਂ
    ਸਤਰ = init_linep (linep);

  ਜੇ (! readlinebuffer_delim (&ਸਤਰ->buf, fp, eolchar))
    {
      ਜੇ (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (ਸਤਰ);
      ਵਾਪਸ false;
    }
  ++ਸਤਰ_no[which - 1];

  xfields (ਸਤਰ);

  ਜੇ (prevline[which - 1])
    check_ਕ੍ਰਮ (prevline[which - 1], ਸਤਰ, which);

  prevline[which - 1] = ਸਤਰ;
  ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ
free_spareline (ਖਾਲੀ)
{
  ਲਈ (idx_t i = 0; i < countof (spareline); i++)
    {
      ਜੇ (spareline[i])
        {
          freeline (spareline[i]);
          ਮੁਕਤ (spareline[i]);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
initseq (ਬਣਤਰ seq *seq)
{
  seq->ਗਿਣਤੀ_2 = 0;
  seq->alloc = 0;
  seq->ਸਤਰਾਂ = ਨਿਰਾ;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

ਸਥਿਰ bool
getseq (FILE *fp, ਬਣਤਰ seq *seq, ਪੂਰਨ whichfile)
{
  ਜੇ (seq->ਗਿਣਤੀ_2 == seq->alloc)
    {
      seq->ਸਤਰਾਂ = xpalloc (seq->ਸਤਰਾਂ, &seq->alloc, 1, -1, ਆਕਾਰ *seq->ਸਤਰਾਂ);
      ਲਈ (idx_t i = seq->ਗਿਣਤੀ_2; i < seq->alloc; i++)
        seq->ਸਤਰਾਂ[i] = ਨਿਰਾ;
    }

  ਜੇ (get_ਸਤਰ (fp, &seq->ਸਤਰਾਂ[seq->ਗਿਣਤੀ_2], whichfile))
    {
      ++seq->ਗਿਣਤੀ_2;
      ਵਾਪਸ true;
    }
  ਵਾਪਸ false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
ਸਥਿਰ bool
advance_seq (FILE *fp, ਬਣਤਰ seq *seq, bool ਪਹਿਲਾ, ਪੂਰਨ whichfile)
{
  ਜੇ (ਪਹਿਲਾ)
    seq->ਗਿਣਤੀ_2 = 0;

  ਵਾਪਸ getseq (fp, seq, whichfile);
}

ਸਥਿਰ ਖਾਲੀ
delseq (ਬਣਤਰ seq *seq)
{
  ਲਈ (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->ਸਤਰਾਂ[i]);
      ਮੁਕਤ (seq->ਸਤਰਾਂ[i]);
    }
  ਮੁਕਤ (seq->ਸਤਰਾਂ);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

ਸਥਿਰ ਖਾਲੀ
prfield (idx_t n, ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸਤਰ)
{
  ਜੇ (n < ਸਤਰ->nfields)
    {
      idx_t len = ਸਤਰ->fields[n].len;
      ਜੇ (len)
        fwrite (ਸਤਰ->fields[n].beg, 1, len, stdout);
      ਨਹੀਂਤਾਂ ਜੇ (ਖਾਲੀ_ਥਾਵਾਂ_filler)
        fputs (ਖਾਲੀ_ਥਾਵਾਂ_filler, stdout);
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਖਾਲੀ_ਥਾਵਾਂ_filler)
    fputs (ਖਾਲੀ_ਥਾਵਾਂ_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

ਸਥਿਰ ਖਾਲੀ
prfields (ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸਤਰ, idx_t join_ਖੇਤਰ, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : ਸਤਰ->nfields;

  ਲਈ (idx_t i = 0; i < join_ਖੇਤਰ && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, ਸਤਰ);
    }
  ਲਈ (idx_t i = join_ਖੇਤਰ + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, ਸਤਰ);
    }
}

/* Print the join of LINE1 and LINE2.  */

ਸਥਿਰ ਖਾਲੀ
prjoin (ਬਣਤਰ ਸਤਰ ਅਚਲ *line1, ਬਣਤਰ ਸਤਰ ਅਚਲ *line2)
{
  ਅਚਲ ਬਣਤਰ outlist *outlist;
  idx_t ਖੇਤਰ;
  ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸਤਰ;

  outlist = outlist_ਸਿਰ.ਅਗਲਾ;
  ਜੇ (outlist)
    {
      ਅਚਲ ਬਣਤਰ outlist *o;

      o = outlist;
      ਜਦੋਂ (true)
        {
          ਜੇ (o->file == 0)
            {
              ਜੇ (line1 == &uni_blank)
                {
                  ਸਤਰ = line2;
                  ਖੇਤਰ = join_ਖੇਤਰ_2;
                }
              ਨਹੀਂਤਾਂ
                {
                  ਸਤਰ = line1;
                  ਖੇਤਰ = join_ਖੇਤਰ_1;
                }
            }
          ਨਹੀਂਤਾਂ
            {
              ਸਤਰ = (o->file == 1 ? line1 : line2);
              ਖੇਤਰ = o->ਖੇਤਰ;
            }
          prfield (ਖੇਤਰ, ਸਤਰ);
          o = o->ਅਗਲਾ;
          ਜੇ (o == ਨਿਰਾ)
            ਰੋਕੋ;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      ਅੱਖਰ_ਲਿਖੋ (eolchar);
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (line1 == &uni_blank)
        {
          ਸਤਰ = line2;
          ਖੇਤਰ = join_ਖੇਤਰ_2;
        }
      ਨਹੀਂਤਾਂ
        {
          ਸਤਰ = line1;
          ਖੇਤਰ = join_ਖੇਤਰ_1;
        }

      /* Output the join field.  */
      prfield (ਖੇਤਰ, ਸਤਰ);

      /* Output other fields.  */
      prfields (line1, join_ਖੇਤਰ_1, autocount_1);
      prfields (line2, join_ਖੇਤਰ_2, autocount_2);

      ਅੱਖਰ_ਲਿਖੋ (eolchar);
    }

  ਜੇ (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

ਸਥਿਰ ਖਾਲੀ
join (FILE *fp1, FILE *fp2)
{
  ਬਣਤਰ seq seq1, seq2;
  ਪੂਰਨ diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  ਜੇ (autoformat)
    {
      autocount_1 = seq1.ਗਿਣਤੀ_2 ? seq1.ਸਤਰਾਂ[0]->nfields : 0;
      autocount_2 = seq2.ਗਿਣਤੀ_2 ? seq2.ਸਤਰਾਂ[0]->nfields : 0;
    }

  ਜੇ (join_ਸਿਰਲੇਖ_ਸਤਰਾਂ && (seq1.ਗਿਣਤੀ_2 || seq2.ਗਿਣਤੀ_2))
    {
      ਬਣਤਰ ਸਤਰ ਅਚਲ *hline1 = seq1.ਗਿਣਤੀ_2 ? seq1.ਸਤਰਾਂ[0] : &uni_blank;
      ਬਣਤਰ ਸਤਰ ਅਚਲ *hline2 = seq2.ਗਿਣਤੀ_2 ? seq2.ਸਤਰਾਂ[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = ਨਿਰਾ;
      prevline[1] = ਨਿਰਾ;
      ਜੇ (seq1.ਗਿਣਤੀ_2)
        advance_seq (fp1, &seq1, true, 1);
      ਜੇ (seq2.ਗਿਣਤੀ_2)
        advance_seq (fp2, &seq2, true, 2);
    }

  ਜਦੋਂ (seq1.ਗਿਣਤੀ_2 && seq2.ਗਿਣਤੀ_2)
    {
      diff = keycmp (seq1.ਸਤਰਾਂ[0], seq2.ਸਤਰਾਂ[0],
                     join_ਖੇਤਰ_1, join_ਖੇਤਰ_2);
      ਜੇ (diff < 0)
        {
          ਜੇ (print_unpairables_1)
            prjoin (seq1.ਸਤਰਾਂ[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          ਵੇਖਿਆ_unpairable = true;
          ਜਾਰੀ;
        }
      ਜੇ (diff > 0)
        {
          ਜੇ (print_unpairables_2)
            prjoin (&uni_blank, seq2.ਸਤਰਾਂ[0]);
          advance_seq (fp2, &seq2, true, 2);
          ਵੇਖਿਆ_unpairable = true;
          ਜਾਰੀ;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      ਕਰੋ
        ਜੇ (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.ਗਿਣਤੀ_2;
            ਰੋਕੋ;
          }
      ਜਦੋਂ (!keycmp (seq1.ਸਤਰਾਂ[seq1.ਗਿਣਤੀ_2 - 1], seq2.ਸਤਰਾਂ[0],
                      join_ਖੇਤਰ_1, join_ਖੇਤਰ_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      ਕਰੋ
        ਜੇ (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.ਗਿਣਤੀ_2;
            ਰੋਕੋ;
          }
      ਜਦੋਂ (!keycmp (seq1.ਸਤਰਾਂ[0], seq2.ਸਤਰਾਂ[seq2.ਗਿਣਤੀ_2 - 1],
                      join_ਖੇਤਰ_1, join_ਖੇਤਰ_2));

      ਜੇ (print_pairables)
        {
          ਲਈ (idx_t i = 0; i < seq1.ਗਿਣਤੀ_2 - 1; ++i)
            {
              ਲਈ (idx_t j = 0; j < seq2.ਗਿਣਤੀ_2 - 1; ++j)
                prjoin (seq1.ਸਤਰਾਂ[i], seq2.ਸਤਰਾਂ[j]);
            }
        }

      ਜੇ (!eof1)
        {
          SWAPLINES (seq1.ਸਤਰਾਂ[0], seq1.ਸਤਰਾਂ[seq1.ਗਿਣਤੀ_2 - 1]);
          seq1.ਗਿਣਤੀ_2 = 1;
        }
      ਨਹੀਂਤਾਂ
        seq1.ਗਿਣਤੀ_2 = 0;

      ਜੇ (!eof2)
        {
          SWAPLINES (seq2.ਸਤਰਾਂ[0], seq2.ਸਤਰਾਂ[seq2.ਗਿਣਤੀ_2 - 1]);
          seq2.ਗਿਣਤੀ_2 = 1;
        }
      ਨਹੀਂਤਾਂ
        seq2.ਗਿਣਤੀ_2 = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  ਬਣਤਰ ਸਤਰ *ਸਤਰ = ਨਿਰਾ;
  bool checktail = false;

  ਜੇ (check_ਨਿਵੇਸ਼_ਕ੍ਰਮ != CHECK_ਕ੍ਰਮ_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  ਜੇ ((print_unpairables_1 || checktail) && seq1.ਗਿਣਤੀ_2)
    {
      ਜੇ (print_unpairables_1)
        prjoin (seq1.ਸਤਰਾਂ[0], &uni_blank);
      ਜੇ (seq2.ਗਿਣਤੀ_2)
        ਵੇਖਿਆ_unpairable = true;
      ਜਦੋਂ (get_ਸਤਰ (fp1, &ਸਤਰ, 1))
        {
          ਜੇ (print_unpairables_1)
            prjoin (ਸਤਰ, &uni_blank);
          ਜੇ (issued_disorder_warning[0] && !print_unpairables_1)
            ਰੋਕੋ;
        }
    }

  ਜੇ ((print_unpairables_2 || checktail) && seq2.ਗਿਣਤੀ_2)
    {
      ਜੇ (print_unpairables_2)
        prjoin (&uni_blank, seq2.ਸਤਰਾਂ[0]);
      ਜੇ (seq1.ਗਿਣਤੀ_2)
        ਵੇਖਿਆ_unpairable = true;
      ਜਦੋਂ (get_ਸਤਰ (fp2, &ਸਤਰ, 2))
        {
          ਜੇ (print_unpairables_2)
            prjoin (&uni_blank, ਸਤਰ);
          ਜੇ (issued_disorder_warning[1] && !print_unpairables_2)
            ਰੋਕੋ;
        }
    }

  freeline (ਸਤਰ);
  ਮੁਕਤ (ਸਤਰ);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_ਖੇਤਰ (ਪੂਰਨ file, idx_t ਖੇਤਰ)
{
  ਬਣਤਰ outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || ਖੇਤਰ == 0);

  o = xmalloc (ਆਕਾਰ *o);
  o->file = file;
  o->ਖੇਤਰ = ਖੇਤਰ;
  o->ਅਗਲਾ = ਨਿਰਾ;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_ਸਿਰਾ->ਅਗਲਾ = o;
  outlist_ਸਿਰਾ = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

ਸਥਿਰ idx_t
string_ਮੰਜ਼ਿਲ_join_ਖੇਤਰ (ਅੱਖਰ ਅਚਲ *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, ਨਿਰਾ, 10, &val, "");
  ਜੇ (s_err == LONGINT_ਉਛਾਲ || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  ਨਹੀਂਤਾਂ ਜੇ (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  ਵਾਪਸ val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

ਸਥਿਰ ਖਾਲੀ
decode_ਖੇਤਰ_spec (ਅੱਖਰ ਅਚਲ *s, ਪੂਰਨ *file_ਸੂਚਕਾਂਕ, idx_t *ਖੇਤਰ_ਸੂਚਕਾਂਕ)
{
  /* The first character must be 0, 1, or 2.  */
  ਚੋਣ (s[0])
    {
    ਹਾਲਤ '0':
      ਜੇ (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_ਸੂਚਕਾਂਕ = 0;
      *ਖੇਤਰ_ਸੂਚਕਾਂਕ = 0;
      ਰੋਕੋ;

    ਹਾਲਤ '1':
    ਹਾਲਤ '2':
      ਜੇ (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_ਸੂਚਕਾਂਕ = s[0] - '0';
      *ਖੇਤਰ_ਸੂਚਕਾਂਕ = string_ਮੰਜ਼ਿਲ_join_ਖੇਤਰ (s + 2);
      ਰੋਕੋ;

    ਮੂਲ:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

ਸਥਿਰ bool
comma_or_blank (mcel_t g)
{
  ਵਾਪਸ g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_ਖੇਤਰ_list (ਅੱਖਰ *str)
{
  ਅੱਖਰ *p = str;

  ਕਰੋ
    {
      ਪੂਰਨ file_ਸੂਚਕਾਂਕ;
      idx_t ਖੇਤਰ_ਸੂਚਕਾਂਕ;
      ਅੱਖਰ ਅਚਲ *spec_ਵਸਤੂ = p;
      p = skip_str_matching (spec_ਵਸਤੂ, comma_or_blank, false);
      ਜੇ (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_ਖੇਤਰ_spec (spec_ਵਸਤੂ, &file_ਸੂਚਕਾਂਕ, &ਖੇਤਰ_ਸੂਚਕਾਂਕ);
      ਜੋੜੋ_ਖੇਤਰ (file_ਸੂਚਕਾਂਕ, ਖੇਤਰ_ਸੂਚਕਾਂਕ);
    }
  ਜਦੋਂ (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_join_ਖੇਤਰ (ptrdiff_t *var, idx_t val)
{
  ਜੇ (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

ਗਿਣਤੀ ਸੰਕਾਰਜ_ਸਥਿਤੀ
  {
    /* This argument must be an operand, i.e., one of the files to be
       joined.  */
    MUST_BE_ਸੰਕਾਰਜ,

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

ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_file_name (ਅੱਖਰ *name, ਅੱਖਰ *ਨਾਂ[2],
               ਪੂਰਨ ਸੰਕਾਰਜ_ਸਥਿਤੀ[2], ਪੂਰਨ joption_ਗਿਣਤੀ[2], ਪੂਰਨ *nfiles,
               ਪੂਰਨ *prev_optc_ਸਥਿਤੀ, ਪੂਰਨ *optc_ਸਥਿਤੀ)
{
  ਪੂਰਨ n = *nfiles;

  ਜੇ (n == 2)
    {
      bool op0 = (ਸੰਕਾਰਜ_ਸਥਿਤੀ[0] == MUST_BE_ਸੰਕਾਰਜ);
      ਅੱਖਰ *arg = ਨਾਂ[op0];
      ਚੋਣ (ਸੰਕਾਰਜ_ਸਥਿਤੀ[op0])
        {
        ਹਾਲਤ MUST_BE_ਸੰਕਾਰਜ:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        ਹਾਲਤ MIGHT_BE_J1_ARG:
          joption_ਗਿਣਤੀ[0]--;
          ਸੈੱਟ_join_ਖੇਤਰ (&join_ਖੇਤਰ_1, string_ਮੰਜ਼ਿਲ_join_ਖੇਤਰ (arg));
          ਰੋਕੋ;

        ਹਾਲਤ MIGHT_BE_J2_ARG:
          joption_ਗਿਣਤੀ[1]--;
          ਸੈੱਟ_join_ਖੇਤਰ (&join_ਖੇਤਰ_2, string_ਮੰਜ਼ਿਲ_join_ਖੇਤਰ (arg));
          ਰੋਕੋ;

        ਹਾਲਤ MIGHT_BE_O_ARG:
          ਜੋੜੋ_ਖੇਤਰ_list (arg);
          ਰੋਕੋ;
        }
      ਜੇ (!op0)
        {
          ਸੰਕਾਰਜ_ਸਥਿਤੀ[0] = ਸੰਕਾਰਜ_ਸਥਿਤੀ[1];
          ਨਾਂ[0] = ਨਾਂ[1];
        }
      n = 1;
    }

  ਸੰਕਾਰਜ_ਸਥਿਤੀ[n] = *prev_optc_ਸਥਿਤੀ;
  ਨਾਂ[n] = name;
  *nfiles = n + 1;
  ਜੇ (*prev_optc_ਸਥਿਤੀ == MIGHT_BE_O_ARG)
    *optc_ਸਥਿਤੀ = MIGHT_BE_O_ARG;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ optc_ਸਥਿਤੀ;
  ਪੂਰਨ prev_optc_ਸਥਿਤੀ = MUST_BE_ਸੰਕਾਰਜ;
  ਪੂਰਨ ਸੰਕਾਰਜ_ਸਥਿਤੀ[2];
  ਪੂਰਨ joption_ਗਿਣਤੀ[2] = { 0, 0 };
  FILE *fp1, *fp2;
  ਪੂਰਨ optc;
  ਪੂਰਨ nfiles = 0;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, ਨਿਰਾ))
         != -1)
    {
      optc_ਸਥਿਤੀ = MUST_BE_ਸੰਕਾਰਜ;

      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'v':
            print_pairables = false;
            FALLTHROUGH;

        ਹਾਲਤ 'a':
          {
            ਲੰਮਾ ਪੂਰਨ val;
            ਜੇ (xstrtol (optarg, ਨਿਰਾ, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            ਜੇ (val == 1)
              print_unpairables_1 = true;
            ਨਹੀਂਤਾਂ
              print_unpairables_2 = true;
          }
          ਰੋਕੋ;

        ਹਾਲਤ 'e':
          ਜੇ (ਖਾਲੀ_ਥਾਵਾਂ_filler && ! streq (ਖਾਲੀ_ਥਾਵਾਂ_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          ਖਾਲੀ_ਥਾਵਾਂ_filler = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'i':
          ignore_case = true;
          ਰੋਕੋ;

        ਹਾਲਤ '1':
          ਸੈੱਟ_join_ਖੇਤਰ (&join_ਖੇਤਰ_1, string_ਮੰਜ਼ਿਲ_join_ਖੇਤਰ (optarg));
          ਰੋਕੋ;

        ਹਾਲਤ '2':
          ਸੈੱਟ_join_ਖੇਤਰ (&join_ਖੇਤਰ_2, string_ਮੰਜ਼ਿਲ_join_ਖੇਤਰ (optarg));
          ਰੋਕੋ;

        ਹਾਲਤ 'j':
          ਜੇ ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_ਗਿਣਤੀ[is_j2]++;
              optc_ਸਥਿਤੀ = MIGHT_BE_J1_ARG + is_j2;
            }
          ਨਹੀਂਤਾਂ
            {
              ਸੈੱਟ_join_ਖੇਤਰ (&join_ਖੇਤਰ_1, string_ਮੰਜ਼ਿਲ_join_ਖੇਤਰ (optarg));
              ਸੈੱਟ_join_ਖੇਤਰ (&join_ਖੇਤਰ_2, join_ਖੇਤਰ_1);
            }
          ਰੋਕੋ;

        ਹਾਲਤ 'o':
          ਜੇ (streq (optarg, "auto"))
            autoformat = true;
          ਨਹੀਂਤਾਂ
            {
              ਜੋੜੋ_ਖੇਤਰ_list (optarg);
              optc_ਸਥਿਤੀ = MIGHT_BE_O_ARG;
            }
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          {
            mcel_t newtab;
            ਜੇ (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            ਨਹੀਂਤਾਂ ਜੇ (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            ਨਹੀਂਤਾਂ
              {
                newtab = mcel_scanz (optarg);
                ਜੇ (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            ਜੇ (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          ਰੋਕੋ;

        ਹਾਲਤ 'z':
          eolchar = 0;
          ਰੋਕੋ;

        ਹਾਲਤ NOCHECK_ਕ੍ਰਮ_OPTION:
          check_ਨਿਵੇਸ਼_ਕ੍ਰਮ = CHECK_ਕ੍ਰਮ_DISABLED;
          ਰੋਕੋ;

        ਹਾਲਤ CHECK_ਕ੍ਰਮ_OPTION:
          check_ਨਿਵੇਸ਼_ਕ੍ਰਮ = CHECK_ਕ੍ਰਮ_ENABLED;
          ਰੋਕੋ;

        ਹਾਲਤ 1:		/* Non-option argument.  */
          ਜੋੜੋ_file_name (optarg, g_ਨਾਂ, ਸੰਕਾਰਜ_ਸਥਿਤੀ, joption_ਗਿਣਤੀ,
                         &nfiles, &prev_optc_ਸਥਿਤੀ, &optc_ਸਥਿਤੀ);
          ਰੋਕੋ;

        ਹਾਲਤ ਸਿਰਲੇਖ_ਸਤਰ_OPTION:
          join_ਸਿਰਲੇਖ_ਸਤਰਾਂ = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }

      prev_optc_ਸਥਿਤੀ = optc_ਸਥਿਤੀ;
    }

  /* Process any operands after "--".  */
  prev_optc_ਸਥਿਤੀ = MUST_BE_ਸੰਕਾਰਜ;
  ਜਦੋਂ (optind < argc)
    ਜੋੜੋ_file_name (argv[optind++], g_ਨਾਂ, ਸੰਕਾਰਜ_ਸਥਿਤੀ, joption_ਗਿਣਤੀ,
                   &nfiles, &prev_optc_ਸਥਿਤੀ, &optc_ਸਥਿਤੀ);

  ਜੇ (nfiles != 2)
    {
      ਜੇ (nfiles == 0)
        error (0, 0, _("missing operand"));
      ਨਹੀਂਤਾਂ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  ਲਈ (ਪੂਰਨ i = 0; i < 2; i++)
    ਜੇ (joption_ਗਿਣਤੀ[i] != 0)
      {
        ਸੈੱਟ_join_ਖੇਤਰ (&join_ਖੇਤਰ_1, i);
        ਸੈੱਟ_join_ਖੇਤਰ (&join_ਖੇਤਰ_2, i);
      }

  ਜੇ (join_ਖੇਤਰ_1 < 0)
    join_ਖੇਤਰ_1 = 0;
  ਜੇ (join_ਖੇਤਰ_2 < 0)
    join_ਖੇਤਰ_2 = 0;

  fp1 = streq (g_ਨਾਂ[0], "-") ? stdin : fopen (g_ਨਾਂ[0], "r");
  ਜੇ (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_ਨਾਂ[0]));
  fp2 = streq (g_ਨਾਂ[1], "-") ? stdin : fopen (g_ਨਾਂ[1], "r");
  ਜੇ (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_ਨਾਂ[1]));
  ਜੇ (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  ਜੇ (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_ਨਾਂ[0]));
  ਜੇ (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_ਨਾਂ[1]));

  ਜੇ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ EXIT_SUCCESS;
}
