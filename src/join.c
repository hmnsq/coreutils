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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <getopt.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "hard-locale.h"
#അന്തര്ഭാവ "linebuffer.h"
#അന്തര്ഭാവ "mcel.h"
#അന്തര്ഭാവ "memcasecmp.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "skipchars.h"
#അന്തര്ഭാവ "stdio--.h"
#അന്തര്ഭാവ "xmemcoll.h"
#അന്തര്ഭാവ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "join"

#define AUTHORS proper_name ("Mike Haertel")

#define join system_join

#define SWAPLINES(a, b) ചെയ്യുക { \
  ഘടന വരി *tmp = a; \
  a = b; \
  b = tmp; \
} ആകുമ്പോൾ (0);

/* An element of the list identifying which fields to print for each
   output line.  */
ഘടന outlist
  {
    /* File number: 0, 1, or 2.  0 means use the join field.
       1 means use the first file argument, 2 the second.  */
    പൂർണ്ണസംഖ്യ file;

    /* Field index (zero-based), specified only when FILE is 1 or 2.  */
    idx_t field;

    ഘടന outlist *next;
  };

/* A field of a line.  */
ഘടന field
  {
    അക്ഷരം *beg;			/* First character in field.  */
    idx_t len;			/* The length of the field.  */
  };

/* A line read from an input file.  */
ഘടന വരി
  {
    ഘടന linebuffer buf;	/* The line itself.  */
    idx_t nfields;		/* Number of elements in 'fields'.  */
    idx_t nfields_allocated;	/* Number of elements allocated for 'fields'. */
    ഘടന field *fields COUNTED_BY (nfields);
  };

/* One or more consecutive lines read from a file that all have the
   same join field value.  */
ഘടന seq
  {
    idx_t എണ്ണം;		/* Elements used in 'lines'.  */
    idx_t alloc;		/* Elements allocated in 'lines'.  */
    ഘടന വരി **വരികൾ COUNTED_BY (alloc);
  };

/* The previous line read from each file.  */
സ്ഥിതം ഘടന വരി *prevline[2] = {ശൂന്യ, ശൂന്യ};

/* The number of lines read from each file.  */
സ്ഥിതം uintmax_t വരി_no[2] = {0, 0};

/* The input file names.  */
സ്ഥിതം അക്ഷരം *g_names[2];

/* This provides an extra line buffer for each file.  We need these if we
   try to read two consecutive lines into the same buffer, since we don't
   want to overwrite the previous buffer before we check order. */
സ്ഥിതം ഘടന വരി *spareline[2] = {ശൂന്യ, ശൂന്യ};

/* True if the LC_COLLATE locale is hard.  */
സ്ഥിതം bool hard_LC_COLLATE;

/* If nonzero, print unpairable lines in file 1 or 2.  */
സ്ഥിതം bool print_unpairables_1, print_unpairables_2;

/* If nonzero, print pairable lines.  */
സ്ഥിതം bool print_pairables = true;

/* If nonzero, we have seen at least one unpairable line. */
സ്ഥിതം bool seen_unpairable;

/* If nonzero, we have warned about disorder in that file. */
സ്ഥിതം bool issued_disorder_warning[2];

/* Empty output field filler.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *empty_filler;

/* Whether to ensure the same number of fields are output from each line.  */
സ്ഥിതം bool autoformat;
/* The number of fields to output for each line.
   Only significant when autoformat is true.  */
സ്ഥിതം idx_t autocount_1;
സ്ഥിതം idx_t autocount_2;

/* Field to join on; -1 means they haven't been determined yet.  */
സ്ഥിതം ptrdiff_t join_field_1 = -1;
സ്ഥിതം ptrdiff_t join_field_2 = -1;

/* List of fields to print.  */
സ്ഥിതം ഘടന outlist outlist_head;

/* Last element in 'outlist', where a new element can be added.  */
സ്ഥിതം ഘടന outlist *outlist_end = &outlist_head;

/* Tab character (or encoding error) separating fields.  If TAB.len == 0,
   fields are separated by any nonempty string of blanks, otherwise by
   exactly one tab character (or encoding error) equal to TAB.  */
സ്ഥിതം mcel_t tab;

/* The output separator to use, and its length in bytes.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *output_separator = " ";
സ്ഥിതം idx_t output_seplen = 1;

/* If nonzero, check that the input is correctly ordered. */
സ്ഥിതം എണ്ണൽ
  {
    CHECK_ORDER_DEFAULT,
    CHECK_ORDER_ENABLED,
    CHECK_ORDER_DISABLED
  } check_നിവേശം_order;

എണ്ണൽ
{
  CHECK_ORDER_OPTION = CHAR_MAX + 1,
  NOCHECK_ORDER_OPTION,
  HEADER_വരി_OPTION
};


സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"ignore-case", no_argument, ശൂന്യ, 'i'},
  {"check-order", no_argument, ശൂന്യ, CHECK_ORDER_OPTION},
  {"nocheck-order", no_argument, ശൂന്യ, NOCHECK_ORDER_OPTION},
  {"zero-terminated", no_argument, ശൂന്യ, 'z'},
  {"header", no_argument, ശൂന്യ, HEADER_വരി_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Used to print non-joining lines */
സ്ഥിതം ഘടന വരി uni_blank;

/* If nonzero, ignore case when comparing join fields.  */
സ്ഥിതം bool ignore_case;

/* If nonzero, treat the first line of each file as column headers --
   join them without checking for ordering */
സ്ഥിതം bool join_header_വരികൾ;

/* The character marking end of line. Default to \n. */
സ്ഥിതം അക്ഷരം eolchar = '\n';

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... FILE1 FILE2\n\
"),
              program_name);
      fputs (_("\
For each pair of നിവേശം വരികൾ with identical join fields, ലിഖ a വരി to\n\
standard output.  The സ്ഥിരസ്ഥിതി join field is the first, delimited by blanks.\
\n\
"), stdout);
      fputs (_("\
\n\
When FILE1 or FILE2 (not both) is -, പഠ standard നിവേശം.\n\
"), stdout);

      oputs (_("\
  -a FILENUM\n\
         also print unpairable വരികൾ from file FILENUM,\n\
         where FILENUM is 1 or 2, corresponding to FILE1 or FILE2\n\
"));
      oputs (_("\
  -e STRING\n\
         replace missing (empty) നിവേശം fields with STRING;\n\
         I.e., missing fields specified with '-12jo' options\n\
"));
      oputs (_("\
  -i, --ignore-അവസ്ഥ\n\
         ignore differences in അവസ്ഥ when comparing fields\n\
"));
      oputs (_("\
  -j FIELD\n\
         equivalent to '-1 FIELD -2 FIELD'\n\
"));
      oputs (_("\
  -o FORMAT\n\
         obey FORMAT ആകുമ്പോൾ constructing output വരി\n\
"));
      oputs (_("\
  -t CHAR\n\
         use CHAR as നിവേശം and output field separator\n\
"));
      oputs (_("\
  -v FILENUM\n\
         like -a FILENUM, but suppress joined output വരികൾ\n\
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
         check that the നിവേശം is correctly sorted, even\n\
         എങ്കിൽ all നിവേശം വരികൾ are pairable\n\
"));
      oputs (_("\
      --nocheck-order\n\
         ചെയ്യുക not check that the നിവേശം is correctly sorted\n\
"));
      oputs (_("\
      --header\n\
         treat the first വരി in each file as field headers,\n\
         print them without trying to pair them\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         വരി delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Unless -t CHAR is given, leading blanks separate fields and are ignored,\n\
അല്ലെങ്കിൽ fields are separated by CHAR.  Any FIELD is a field സംഖ്യ counted\n\
from 1.  FORMAT is one or more comma or blank separated specifications,\n\
each being 'FILENUM.FIELD' or '0'.  Default FORMAT outputs the join field,\n\
the remaining fields from FILE1, the remaining fields from FILE2, all\n\
separated by CHAR.  If FORMAT is the keyword 'auto', then the first\n\
വരി of each file determines the സംഖ്യ of fields output വേണ്ടി each വരി.\n\
\n\
Important: FILE1 and FILE2 must be sorted on the join fields.\n\
E.g., use \"sort -k 1b,1\" എങ്കിൽ 'join' has no options,\n\
or use \"join -t ''\" എങ്കിൽ 'sort' has no options.\n\
Comparisons honor the rules specified by 'LC_COLLATE'.\n\
If the നിവേശം is not sorted and some വരികൾ cannot be joined, a\n\
warning message will be given.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Record a field in LINE, with location FIELD and size LEN.  */

സ്ഥിതം ശൂന്യം
extract_field (ഘടന വരി *വരി, അക്ഷരം *field, idx_t len)
{
  എങ്കിൽ (വരി->nfields >= വരി->nfields_allocated)
    വരി->fields = xpalloc (വരി->fields, &വരി->nfields_allocated, 1,
                            -1, വലുപ്പം *വരി->fields);
  idx_t സ്ഥിരം field_index = വരി->nfields++;
  വരി->fields[field_index].beg = field;
  വരി->fields[field_index].len = len;
}

സ്ഥിതം bool
eq_tab (mcel_t g)
{
  തിരികെ mcel_eq (g, tab);
}

സ്ഥിതം bool
newline_or_blank (mcel_t g)
{
  തിരികെ g.ch == '\n' || c32issep (g.ch);
}

/* Fill in the 'fields' structure in LINE.  */

സ്ഥിതം ശൂന്യം
xfields (ഘടന വരി *വരി)
{
  അക്ഷരം *ptr = വരി->buf.buffer;
  അക്ഷരം സ്ഥിരം *lim = ptr + വരി->buf.length - 1;

  എങ്കിൽ (ptr == lim)
    തിരികെ;

  എങ്കിൽ (!tab.len)
    ആകുമ്പോൾ ((ptr = skip_buf_matching (ptr, lim, newline_or_blank, true)) < lim)
      {
        അക്ഷരം *sep = skip_buf_matching (ptr, lim, newline_or_blank, false);
        extract_field (വരി, ptr, sep - ptr);
        ptr = sep;
      }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (tab.ch != '\n')
        വേണ്ടി (അക്ഷരം *sep;
             (sep = skip_buf_matching (ptr, lim, eq_tab, false)) < lim;
             ptr = sep + mcel_scan (sep, lim).len)
          extract_field (വരി, ptr, sep - ptr);

      extract_field (വരി, ptr, lim - ptr);
    }
}

സ്ഥിതം ശൂന്യം
freeline (ഘടന വരി *വരി)
{
  എങ്കിൽ (വരി == ശൂന്യ)
    തിരികെ;
  സ്വതന്ത്രം (വരി->fields);
  വരി->fields = ശൂന്യ;
  വരി->nfields_allocated = 0;
  സ്വതന്ത്രം (വരി->buf.buffer);
  വരി->buf.buffer = ശൂന്യ;
  വരി->buf.size = 0;
}

/* Return <0 if the join field in LINE1 compares less than the one in LINE2;
   >0 if it compares greater; 0 if it compares equal.
   Report an error and exit if the comparison fails.
   Use join fields JF_1 and JF_2 respectively.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
keycmp (ഘടന വരി സ്ഥിരം *line1, ഘടന വരി സ്ഥിരം *line2,
        idx_t jf_1, idx_t jf_2)
{
  /* Start of field to compare in each file.  */
  അക്ഷരം *beg1;
  അക്ഷരം *beg2;

  idx_t len1;
  idx_t len2;		/* Length of fields to compare.  */
  പൂർണ്ണസംഖ്യ diff;

  എങ്കിൽ (jf_1 < line1->nfields)
    {
      beg1 = line1->fields[jf_1].beg;
      len1 = line1->fields[jf_1].len;
    }
  അല്ലെങ്കിൽ
    {
      beg1 = ശൂന്യ;
      len1 = 0;
    }

  എങ്കിൽ (jf_2 < line2->nfields)
    {
      beg2 = line2->fields[jf_2].beg;
      len2 = line2->fields[jf_2].len;
    }
  അല്ലെങ്കിൽ
    {
      beg2 = ശൂന്യ;
      len2 = 0;
    }

  എങ്കിൽ (len1 == 0)
    തിരികെ len2 == 0 ? 0 : -1;
  എങ്കിൽ (len2 == 0)
    തിരികെ 1;

  എങ്കിൽ (ignore_case)
    {
      /* FIXME: ignore_case does not work with NLS (in particular,
         with multibyte chars).  */
      diff = memcasecmp (beg1, beg2, MIN (len1, len2));
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (hard_LC_COLLATE)
        തിരികെ xmemcoll (beg1, len1, beg2, len2);
      diff = memcmp (beg1, beg2, MIN (len1, len2));
    }

  എങ്കിൽ (diff)
    തിരികെ diff;
  തിരികെ _GL_CMP (len1, len2);
}

/* Check that successive input lines PREV and CURRENT from input file
   WHATFILE are presented in order, unless the user may be relying on
   the GNU extension that input lines may be out of order if no input
   lines are unpairable.

   If the user specified --nocheck-order, the check is not made.
   If the user specified --check-order, the problem is fatal.
   Otherwise (the default), the message is simply a warning.

   A message is printed at most once per input file. */

സ്ഥിതം ശൂന്യം
check_order (സ്ഥിരം ഘടന വരി *prev,
             സ്ഥിരം ഘടന വരി *current,
             പൂർണ്ണസംഖ്യ whatfile)
{
  എങ്കിൽ (check_നിവേശം_order != CHECK_ORDER_DISABLED
      && ((check_നിവേശം_order == CHECK_ORDER_ENABLED) || seen_unpairable))
    {
      എങ്കിൽ (!issued_disorder_warning[whatfile - 1])
        {
          idx_t join_field = whatfile == 1 ? join_field_1 : join_field_2;
          എങ്കിൽ (keycmp (prev, current, join_field, join_field) > 0)
            {
              /* Exclude any trailing newline. */
              idx_t len = current->buf.length;
              എങ്കിൽ (0 < len && current->buf.buffer[len - 1] == '\n')
                --len;

              /* If the offending line is longer than INT_MAX, output
                 only the first INT_MAX bytes in this diagnostic.  */
              len = MIN (INT_MAX, len);

              error ((check_നിവേശം_order == CHECK_ORDER_ENABLED
                      ? EXIT_FAILURE : 0),
                     0, _("%s:%ju: is not sorted: %.*s"),
                     g_names[whatfile - 1], വരി_no[whatfile - 1],
                     (പൂർണ്ണസംഖ്യ) len, current->buf.buffer);

              /* If we get to here, the message was merely a warning.
                 Arrange to issue it only once per file.  */
              issued_disorder_warning[whatfile - 1] = true;
            }
        }
    }
}

സ്ഥിതം വരിയിൽ ശൂന്യം
reset_വരി (ഘടന വരി *വരി)
{
  വരി->nfields = 0;
}

സ്ഥിതം ഘടന വരി *
init_linep (ഘടന വരി **linep)
{
  ഘടന വരി *വരി = xzalloc (വലുപ്പം *വരി);
  *linep = വരി;
  തിരികെ വരി;
}

/* Read a line from FP into LINE and split it into fields.
   Return true if successful.  */

സ്ഥിതം bool
get_വരി (FILE *fp, ഘടന വരി **linep, പൂർണ്ണസംഖ്യ which)
{
  ഘടന വരി *വരി = *linep;

  എങ്കിൽ (വരി == prevline[which - 1])
    {
      SWAPLINES (വരി, spareline[which - 1]);
      *linep = വരി;
    }

  എങ്കിൽ (വരി)
    reset_വരി (വരി);
  അല്ലെങ്കിൽ
    വരി = init_linep (linep);

  എങ്കിൽ (! readlinebuffer_delim (&വരി->buf, fp, eolchar))
    {
      എങ്കിൽ (ferror (fp))
        error (EXIT_FAILURE, errno, _("read error"));
      freeline (വരി);
      തിരികെ false;
    }
  ++വരി_no[which - 1];

  xfields (വരി);

  എങ്കിൽ (prevline[which - 1])
    check_order (prevline[which - 1], വരി, which);

  prevline[which - 1] = വരി;
  തിരികെ true;
}

സ്ഥിതം ശൂന്യം
free_spareline (ശൂന്യം)
{
  വേണ്ടി (idx_t i = 0; i < countof (spareline); i++)
    {
      എങ്കിൽ (spareline[i])
        {
          freeline (spareline[i]);
          സ്വതന്ത്രം (spareline[i]);
        }
    }
}

സ്ഥിതം ശൂന്യം
initseq (ഘടന seq *seq)
{
  seq->എണ്ണം = 0;
  seq->alloc = 0;
  seq->വരികൾ = ശൂന്യ;
}

/* Read a line from FP and add it to SEQ.  Return true if successful.  */

സ്ഥിതം bool
getseq (FILE *fp, ഘടന seq *seq, പൂർണ്ണസംഖ്യ whichfile)
{
  എങ്കിൽ (seq->എണ്ണം == seq->alloc)
    {
      seq->വരികൾ = xpalloc (seq->വരികൾ, &seq->alloc, 1, -1, വലുപ്പം *seq->വരികൾ);
      വേണ്ടി (idx_t i = seq->എണ്ണം; i < seq->alloc; i++)
        seq->വരികൾ[i] = ശൂന്യ;
    }

  എങ്കിൽ (get_വരി (fp, &seq->വരികൾ[seq->എണ്ണം], whichfile))
    {
      ++seq->എണ്ണം;
      തിരികെ true;
    }
  തിരികെ false;
}

/* Read a line from FP and add it to SEQ, as the first item if FIRST is
   true, else as the next.  */
സ്ഥിതം bool
advance_seq (FILE *fp, ഘടന seq *seq, bool first, പൂർണ്ണസംഖ്യ whichfile)
{
  എങ്കിൽ (first)
    seq->എണ്ണം = 0;

  തിരികെ getseq (fp, seq, whichfile);
}

സ്ഥിതം ശൂന്യം
delseq (ഘടന seq *seq)
{
  വേണ്ടി (idx_t i = 0; i < seq->alloc; i++)
    {
      freeline (seq->വരികൾ[i]);
      സ്വതന്ത്രം (seq->വരികൾ[i]);
    }
  സ്വതന്ത്രം (seq->വരികൾ);
}


/* Print field N of LINE if it exists and is nonempty, otherwise
   'empty_filler' if it is nonempty.  */

സ്ഥിതം ശൂന്യം
prfield (idx_t n, ഘടന വരി സ്ഥിരം *വരി)
{
  എങ്കിൽ (n < വരി->nfields)
    {
      idx_t len = വരി->fields[n].len;
      എങ്കിൽ (len)
        fwrite (വരി->fields[n].beg, 1, len, stdout);
      അല്ലെങ്കിൽ എങ്കിൽ (empty_filler)
        fputs (empty_filler, stdout);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (empty_filler)
    fputs (empty_filler, stdout);
}

/* Output all the fields in line, other than the join field.  */

സ്ഥിതം ശൂന്യം
prfields (ഘടന വരി സ്ഥിരം *വരി, idx_t join_field, idx_t autocount)
{
  idx_t nfields = autoformat ? autocount : വരി->nfields;

  വേണ്ടി (idx_t i = 0; i < join_field && i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, വരി);
    }
  വേണ്ടി (idx_t i = join_field + 1; i < nfields; ++i)
    {
      fwrite (output_separator, 1, output_seplen, stdout);
      prfield (i, വരി);
    }
}

/* Print the join of LINE1 and LINE2.  */

സ്ഥിതം ശൂന്യം
prjoin (ഘടന വരി സ്ഥിരം *line1, ഘടന വരി സ്ഥിരം *line2)
{
  സ്ഥിരം ഘടന outlist *outlist;
  idx_t field;
  ഘടന വരി സ്ഥിരം *വരി;

  outlist = outlist_head.next;
  എങ്കിൽ (outlist)
    {
      സ്ഥിരം ഘടന outlist *o;

      o = outlist;
      ആകുമ്പോൾ (true)
        {
          എങ്കിൽ (o->file == 0)
            {
              എങ്കിൽ (line1 == &uni_blank)
                {
                  വരി = line2;
                  field = join_field_2;
                }
              അല്ലെങ്കിൽ
                {
                  വരി = line1;
                  field = join_field_1;
                }
            }
          അല്ലെങ്കിൽ
            {
              വരി = (o->file == 1 ? line1 : line2);
              field = o->field;
            }
          prfield (field, വരി);
          o = o->next;
          എങ്കിൽ (o == ശൂന്യ)
            നിർത്തുക;
          fwrite (output_separator, 1, output_seplen, stdout);
        }
      putchar (eolchar);
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (line1 == &uni_blank)
        {
          വരി = line2;
          field = join_field_2;
        }
      അല്ലെങ്കിൽ
        {
          വരി = line1;
          field = join_field_1;
        }

      /* Output the join field.  */
      prfield (field, വരി);

      /* Output other fields.  */
      prfields (line1, join_field_1, autocount_1);
      prfields (line2, join_field_2, autocount_2);

      putchar (eolchar);
    }

  എങ്കിൽ (ferror (stdout))
    write_error ();
}

/* Print the join of the files in FP1 and FP2.  */

സ്ഥിതം ശൂന്യം
join (FILE *fp1, FILE *fp2)
{
  ഘടന seq seq1, seq2;
  പൂർണ്ണസംഖ്യ diff;
  bool eof1, eof2;

  fadvise (fp1, FADVISE_SEQUENTIAL);
  fadvise (fp2, FADVISE_SEQUENTIAL);

  /* Read the first line of each file.  */
  initseq (&seq1);
  getseq (fp1, &seq1, 1);
  initseq (&seq2);
  getseq (fp2, &seq2, 2);

  എങ്കിൽ (autoformat)
    {
      autocount_1 = seq1.എണ്ണം ? seq1.വരികൾ[0]->nfields : 0;
      autocount_2 = seq2.എണ്ണം ? seq2.വരികൾ[0]->nfields : 0;
    }

  എങ്കിൽ (join_header_വരികൾ && (seq1.എണ്ണം || seq2.എണ്ണം))
    {
      ഘടന വരി സ്ഥിരം *hline1 = seq1.എണ്ണം ? seq1.വരികൾ[0] : &uni_blank;
      ഘടന വരി സ്ഥിരം *hline2 = seq2.എണ്ണം ? seq2.വരികൾ[0] : &uni_blank;
      prjoin (hline1, hline2);
      prevline[0] = ശൂന്യ;
      prevline[1] = ശൂന്യ;
      എങ്കിൽ (seq1.എണ്ണം)
        advance_seq (fp1, &seq1, true, 1);
      എങ്കിൽ (seq2.എണ്ണം)
        advance_seq (fp2, &seq2, true, 2);
    }

  ആകുമ്പോൾ (seq1.എണ്ണം && seq2.എണ്ണം)
    {
      diff = keycmp (seq1.വരികൾ[0], seq2.വരികൾ[0],
                     join_field_1, join_field_2);
      എങ്കിൽ (diff < 0)
        {
          എങ്കിൽ (print_unpairables_1)
            prjoin (seq1.വരികൾ[0], &uni_blank);
          advance_seq (fp1, &seq1, true, 1);
          seen_unpairable = true;
          തുടരുക;
        }
      എങ്കിൽ (diff > 0)
        {
          എങ്കിൽ (print_unpairables_2)
            prjoin (&uni_blank, seq2.വരികൾ[0]);
          advance_seq (fp2, &seq2, true, 2);
          seen_unpairable = true;
          തുടരുക;
        }

      /* Keep reading lines from file1 as long as they continue to
         match the current line from file2.  */
      eof1 = false;
      ചെയ്യുക
        എങ്കിൽ (!advance_seq (fp1, &seq1, false, 1))
          {
            eof1 = true;
            ++seq1.എണ്ണം;
            നിർത്തുക;
          }
      ആകുമ്പോൾ (!keycmp (seq1.വരികൾ[seq1.എണ്ണം - 1], seq2.വരികൾ[0],
                      join_field_1, join_field_2));

      /* Keep reading lines from file2 as long as they continue to
         match the current line from file1.  */
      eof2 = false;
      ചെയ്യുക
        എങ്കിൽ (!advance_seq (fp2, &seq2, false, 2))
          {
            eof2 = true;
            ++seq2.എണ്ണം;
            നിർത്തുക;
          }
      ആകുമ്പോൾ (!keycmp (seq1.വരികൾ[0], seq2.വരികൾ[seq2.എണ്ണം - 1],
                      join_field_1, join_field_2));

      എങ്കിൽ (print_pairables)
        {
          വേണ്ടി (idx_t i = 0; i < seq1.എണ്ണം - 1; ++i)
            {
              വേണ്ടി (idx_t j = 0; j < seq2.എണ്ണം - 1; ++j)
                prjoin (seq1.വരികൾ[i], seq2.വരികൾ[j]);
            }
        }

      എങ്കിൽ (!eof1)
        {
          SWAPLINES (seq1.വരികൾ[0], seq1.വരികൾ[seq1.എണ്ണം - 1]);
          seq1.എണ്ണം = 1;
        }
      അല്ലെങ്കിൽ
        seq1.എണ്ണം = 0;

      എങ്കിൽ (!eof2)
        {
          SWAPLINES (seq2.വരികൾ[0], seq2.വരികൾ[seq2.എണ്ണം - 1]);
          seq2.എണ്ണം = 1;
        }
      അല്ലെങ്കിൽ
        seq2.എണ്ണം = 0;
    }

  /* If the user did not specify --nocheck-order, then we read the
     tail ends of both inputs to verify that they are in order.  We
     skip the rest of the tail once we have issued a warning for that
     file, unless we actually need to print the unpairable lines.  */
  ഘടന വരി *വരി = ശൂന്യ;
  bool checktail = false;

  എങ്കിൽ (check_നിവേശം_order != CHECK_ORDER_DISABLED
      && !(issued_disorder_warning[0] && issued_disorder_warning[1]))
    checktail = true;

  എങ്കിൽ ((print_unpairables_1 || checktail) && seq1.എണ്ണം)
    {
      എങ്കിൽ (print_unpairables_1)
        prjoin (seq1.വരികൾ[0], &uni_blank);
      എങ്കിൽ (seq2.എണ്ണം)
        seen_unpairable = true;
      ആകുമ്പോൾ (get_വരി (fp1, &വരി, 1))
        {
          എങ്കിൽ (print_unpairables_1)
            prjoin (വരി, &uni_blank);
          എങ്കിൽ (issued_disorder_warning[0] && !print_unpairables_1)
            നിർത്തുക;
        }
    }

  എങ്കിൽ ((print_unpairables_2 || checktail) && seq2.എണ്ണം)
    {
      എങ്കിൽ (print_unpairables_2)
        prjoin (&uni_blank, seq2.വരികൾ[0]);
      എങ്കിൽ (seq1.എണ്ണം)
        seen_unpairable = true;
      ആകുമ്പോൾ (get_വരി (fp2, &വരി, 2))
        {
          എങ്കിൽ (print_unpairables_2)
            prjoin (&uni_blank, വരി);
          എങ്കിൽ (issued_disorder_warning[1] && !print_unpairables_2)
            നിർത്തുക;
        }
    }

  freeline (വരി);
  സ്വതന്ത്രം (വരി);

  delseq (&seq1);
  delseq (&seq2);
}

/* Add a field spec for field FIELD of file FILE to 'outlist'.  */

സ്ഥിതം ശൂന്യം
add_field (പൂർണ്ണസംഖ്യ file, idx_t field)
{
  ഘടന outlist *o;

  affirm (file == 0 || file == 1 || file == 2);
  affirm (file != 0 || field == 0);

  o = xmalloc (വലുപ്പം *o);
  o->file = file;
  o->field = field;
  o->next = ശൂന്യ;

  /* Add to the end of the list so the fields are in the right order.  */
  outlist_end->next = o;
  outlist_end = o;
}

/* Convert a string of decimal digits, STR (the 1-based join field number),
   to an integral value.  Upon successful conversion, return one less
   (the zero-based field number).  Silently convert too-large values
   to PTRDIFF_MAX.  Otherwise, if a value cannot be converted, give a
   diagnostic and exit.  */

സ്ഥിതം idx_t
string_to_join_field (അക്ഷരം സ്ഥിരം *str)
{
  intmax_t val;

  strtol_error s_err = xstrtoimax (str, ശൂന്യ, 10, &val, "");
  എങ്കിൽ (s_err == LONGINT_OVERFLOW || (s_err == LONGINT_OK && PTRDIFF_MAX < val))
    val = PTRDIFF_MAX;
  അല്ലെങ്കിൽ എങ്കിൽ (s_err != LONGINT_OK || val <= 0)
    error (EXIT_FAILURE, 0, _("invalid field number: %s"), quote (str));

  തിരികെ val - 1;
}

/* Convert a single field specifier string, S, to a *FILE_INDEX, *FIELD_INDEX
   pair.  In S, the field index string is 1-based; *FIELD_INDEX is zero-based.
   If S is valid, return true.  Otherwise, give a diagnostic and exit.  */

സ്ഥിതം ശൂന്യം
decode_field_spec (അക്ഷരം സ്ഥിരം *s, പൂർണ്ണസംഖ്യ *file_index, idx_t *field_index)
{
  /* The first character must be 0, 1, or 2.  */
  തിരഞ്ഞെടുപ്പ് (s[0])
    {
    അവസ്ഥ '0':
      എങ്കിൽ (s[1])
        {
          /* '0' must be all alone -- no '.FIELD'.  */
          error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
        }
      *file_index = 0;
      *field_index = 0;
      നിർത്തുക;

    അവസ്ഥ '1':
    അവസ്ഥ '2':
      എങ്കിൽ (s[1] != '.')
        error (EXIT_FAILURE, 0, _("invalid field specifier: %s"), quote (s));
      *file_index = s[0] - '0';
      *field_index = string_to_join_field (s + 2);
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      error (EXIT_FAILURE, 0,
             _("invalid file number in field spec: %s"), quote (s));
    }
}

സ്ഥിതം bool
comma_or_blank (mcel_t g)
{
  തിരികെ g.ch == ',' || c32issep (g.ch);
}

/* Add the comma or blank separated field spec(s) in STR to 'outlist'.  */

സ്ഥിതം ശൂന്യം
add_field_list (അക്ഷരം *str)
{
  അക്ഷരം *p = str;

  ചെയ്യുക
    {
      പൂർണ്ണസംഖ്യ file_index;
      idx_t field_index;
      അക്ഷരം സ്ഥിരം *spec_item = p;
      p = skip_str_matching (spec_item, comma_or_blank, false);
      എങ്കിൽ (*p)
        {
          mcel_t g = mcel_scanz (p);
          *p = '\0';
          p += g.len;
        }
      decode_field_spec (spec_item, &file_index, &field_index);
      add_field (file_index, field_index);
    }
  ആകുമ്പോൾ (*p);
}

/* Set the join field *VAR to VAL, but report an error if *VAR is set
   more than once to incompatible values.  */

സ്ഥിതം ശൂന്യം
set_join_field (ptrdiff_t *var, idx_t val)
{
  എങ്കിൽ (0 <= *var && *var != val)
    error (EXIT_FAILURE, 0,
           _("incompatible join fields %td, %td"), *var, val);
  *var = val;
}

/* Status of command-line arguments.  */

എണ്ണൽ operand_status
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

സ്ഥിതം ശൂന്യം
add_file_name (അക്ഷരം *name, അക്ഷരം *names[2],
               പൂർണ്ണസംഖ്യ operand_status[2], പൂർണ്ണസംഖ്യ joption_എണ്ണം[2], പൂർണ്ണസംഖ്യ *nfiles,
               പൂർണ്ണസംഖ്യ *prev_optc_status, പൂർണ്ണസംഖ്യ *optc_status)
{
  പൂർണ്ണസംഖ്യ n = *nfiles;

  എങ്കിൽ (n == 2)
    {
      bool op0 = (operand_status[0] == MUST_BE_OPERAND);
      അക്ഷരം *arg = names[op0];
      തിരഞ്ഞെടുപ്പ് (operand_status[op0])
        {
        അവസ്ഥ MUST_BE_OPERAND:
          error (0, 0, _("extra operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);

        അവസ്ഥ MIGHT_BE_J1_ARG:
          joption_എണ്ണം[0]--;
          set_join_field (&join_field_1, string_to_join_field (arg));
          നിർത്തുക;

        അവസ്ഥ MIGHT_BE_J2_ARG:
          joption_എണ്ണം[1]--;
          set_join_field (&join_field_2, string_to_join_field (arg));
          നിർത്തുക;

        അവസ്ഥ MIGHT_BE_O_ARG:
          add_field_list (arg);
          നിർത്തുക;
        }
      എങ്കിൽ (!op0)
        {
          operand_status[0] = operand_status[1];
          names[0] = names[1];
        }
      n = 1;
    }

  operand_status[n] = *prev_optc_status;
  names[n] = name;
  *nfiles = n + 1;
  എങ്കിൽ (*prev_optc_status == MIGHT_BE_O_ARG)
    *optc_status = MIGHT_BE_O_ARG;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ optc_status;
  പൂർണ്ണസംഖ്യ prev_optc_status = MUST_BE_OPERAND;
  പൂർണ്ണസംഖ്യ operand_status[2];
  പൂർണ്ണസംഖ്യ joption_എണ്ണം[2] = { 0, 0 };
  FILE *fp1, *fp2;
  പൂർണ്ണസംഖ്യ optc;
  പൂർണ്ണസംഖ്യ nfiles = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  hard_LC_COLLATE = hard_locale (LC_COLLATE);

  atexit (close_stdout);
  atexit (free_spareline);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "-a:e:i1:2:j:o:t:v:z",
                              longopts, ശൂന്യ))
         != -1)
    {
      optc_status = MUST_BE_OPERAND;

      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'v':
            print_pairables = false;
            FALLTHROUGH;

        അവസ്ഥ 'a':
          {
            long പൂർണ്ണസംഖ്യ val;
            എങ്കിൽ (xstrtol (optarg, ശൂന്യ, 10, &val, "") != LONGINT_OK
                || (val != 1 && val != 2))
              error (EXIT_FAILURE, 0,
                     _("invalid file number: %s"), quote (optarg));
            എങ്കിൽ (val == 1)
              print_unpairables_1 = true;
            അല്ലെങ്കിൽ
              print_unpairables_2 = true;
          }
          നിർത്തുക;

        അവസ്ഥ 'e':
          എങ്കിൽ (empty_filler && ! streq (empty_filler, optarg))
            error (EXIT_FAILURE, 0,
                   _("conflicting empty-field replacement strings"));
          empty_filler = optarg;
          നിർത്തുക;

        അവസ്ഥ 'i':
          ignore_case = true;
          നിർത്തുക;

        അവസ്ഥ '1':
          set_join_field (&join_field_1, string_to_join_field (optarg));
          നിർത്തുക;

        അവസ്ഥ '2':
          set_join_field (&join_field_2, string_to_join_field (optarg));
          നിർത്തുക;

        അവസ്ഥ 'j':
          എങ്കിൽ ((optarg[0] == '1' || optarg[0] == '2') && !optarg[1]
              && optarg == argv[optind - 1] + 2)
            {
              /* The argument was either "-j1" or "-j2".  */
              bool is_j2 = (optarg[0] == '2');
              joption_എണ്ണം[is_j2]++;
              optc_status = MIGHT_BE_J1_ARG + is_j2;
            }
          അല്ലെങ്കിൽ
            {
              set_join_field (&join_field_1, string_to_join_field (optarg));
              set_join_field (&join_field_2, join_field_1);
            }
          നിർത്തുക;

        അവസ്ഥ 'o':
          എങ്കിൽ (streq (optarg, "auto"))
            autoformat = true;
          അല്ലെങ്കിൽ
            {
              add_field_list (optarg);
              optc_status = MIGHT_BE_O_ARG;
            }
          നിർത്തുക;

        അവസ്ഥ 't':
          {
            mcel_t newtab;
            എങ്കിൽ (!*optarg)
              {
                /* '' => process the whole line.  */
                newtab = mcel_ch ('\n', 1);
                /* output_separator does not matter.  */
              }
            അല്ലെങ്കിൽ എങ്കിൽ (streq (optarg, "\\0"))
              {
                newtab = mcel_ch ('\0', 1);
                output_separator = "";
              }
            അല്ലെങ്കിൽ
              {
                newtab = mcel_scanz (optarg);
                എങ്കിൽ (optarg[newtab.len])
                  error (EXIT_FAILURE, 0, _("multi-character tab %s"),
                         quote (optarg));
                output_separator = optarg;
              }
            എങ്കിൽ (tab.len && ! mcel_eq (tab, newtab))
              error (EXIT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
            output_seplen = newtab.len;
          }
          നിർത്തുക;

        അവസ്ഥ 'z':
          eolchar = 0;
          നിർത്തുക;

        അവസ്ഥ NOCHECK_ORDER_OPTION:
          check_നിവേശം_order = CHECK_ORDER_DISABLED;
          നിർത്തുക;

        അവസ്ഥ CHECK_ORDER_OPTION:
          check_നിവേശം_order = CHECK_ORDER_ENABLED;
          നിർത്തുക;

        അവസ്ഥ 1:		/* Non-option argument.  */
          add_file_name (optarg, g_names, operand_status, joption_എണ്ണം,
                         &nfiles, &prev_optc_status, &optc_status);
          നിർത്തുക;

        അവസ്ഥ HEADER_വരി_OPTION:
          join_header_വരികൾ = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }

      prev_optc_status = optc_status;
    }

  /* Process any operands after "--".  */
  prev_optc_status = MUST_BE_OPERAND;
  ആകുമ്പോൾ (optind < argc)
    add_file_name (argv[optind++], g_names, operand_status, joption_എണ്ണം,
                   &nfiles, &prev_optc_status, &optc_status);

  എങ്കിൽ (nfiles != 2)
    {
      എങ്കിൽ (nfiles == 0)
        error (0, 0, _("missing operand"));
      അല്ലെങ്കിൽ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  /* If "-j1" was specified and it turns out not to have had an argument,
     treat it as "-j 1".  Likewise for -j2.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 2; i++)
    എങ്കിൽ (joption_എണ്ണം[i] != 0)
      {
        set_join_field (&join_field_1, i);
        set_join_field (&join_field_2, i);
      }

  എങ്കിൽ (join_field_1 < 0)
    join_field_1 = 0;
  എങ്കിൽ (join_field_2 < 0)
    join_field_2 = 0;

  fp1 = streq (g_names[0], "-") ? stdin : fopen (g_names[0], "r");
  എങ്കിൽ (!fp1)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  fp2 = streq (g_names[1], "-") ? stdin : fopen (g_names[1], "r");
  എങ്കിൽ (!fp2)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));
  എങ്കിൽ (fp1 == fp2)
    error (EXIT_FAILURE, errno, _("both files cannot be standard input"));
  join (fp1, fp2);

  എങ്കിൽ (fclose (fp1) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[0]));
  എങ്കിൽ (fclose (fp2) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (g_names[1]));

  എങ്കിൽ (issued_disorder_warning[0] || issued_disorder_warning[1])
    error (EXIT_FAILURE, 0, _("input is not in sorted order"));
  അല്ലെങ്കിൽ
    തിരികെ EXIT_SUCCESS;
}
