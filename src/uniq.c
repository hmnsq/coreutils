/* uniq -- remove duplicate lines from a sorted file
   Copyright (C) 1986-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Written by Richard M. Stallman and David MacKenzie. */

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "linebuffer.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"
#ಅನ್ತರ್ಭಾವ "posixver.h"
#ಅನ್ತರ್ಭಾವ "skipchars.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"
#ಅನ್ತರ್ಭಾವ "memcasecmp.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "uniq"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ಸ್ಥಿರ ಶೂನ್ಯ
ಅದಲುಬದಲು_ಸಾಲುಗಳು (ರಚನೆ linebuffer **a, ರಚನೆ linebuffer **b)
{
  ರಚನೆ linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
ಸ್ಥಿರ idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
ಸ್ಥಿರ idx_t skip_chars = false;

/* Number of chars to compare. */
ಸ್ಥಿರ idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
ಸ್ಥಿರ bool ಎಣಿಕೆ_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
ಸ್ಥಿರ bool output_unique = true;
ಸ್ಥಿರ bool output_ಮೊದಲ_repeated = true;
ಸ್ಥಿರ bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
ಸ್ಥಿರ bool ignore_case;

ಎಣಿಕೆ delimit_ವಿಧಾನ
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ delimit_ವಿಧಾನ_string[] =
{
  "none", "prepend", "separate", ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ delimit_ವಿಧಾನ ಸ್ಥಿರಾಂಕ delimit_ವಿಧಾನ_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
ಸ್ಥಿರ ಎಣಿಕೆ delimit_ವಿಧಾನ delimit_groups = DM_NONE;

ಎಣಿಕೆ grouping_ವಿಧಾನ
{
  /* No grouping, when "--group" isn't used */
  GM_NONE,

  /* Delimiter precedes all groups.  --group=prepend */
  GM_PREPEND,

  /* Delimiter follows all groups.   --group=append */
  GM_APPEND,

  /* Delimiter between groups.    --group[=separate] */
  GM_SEPARATE,

  /* Delimiter before and after each group. --group=both */
  GM_BOTH
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ grouping_ವಿಧಾನ_string[] =
{
  "prepend", "append", "separate", "both", ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ grouping_ವಿಧಾನ ಸ್ಥಿರಾಂಕ grouping_ವಿಧಾನ_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

ಸ್ಥಿರ ಎಣಿಕೆ grouping_ವಿಧಾನ grouping = GM_NONE;

ಎಣಿಕೆ
{
  ಗುಂಪು_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"count", no_ವಾದ, ಖಾಲಿ, 'c'},
  {"repeated", no_ವಾದ, ಖಾಲಿ, 'd'},
  {"all-repeated", optional_ವಾದ, ಖಾಲಿ, 'D'},
  {"group", optional_ವಾದ, ಖಾಲಿ, ಗುಂಪು_OPTION},
  {"ignore-case", no_ವಾದ, ಖಾಲಿ, 'i'},
  {"unique", no_ವಾದ, ಖಾಲಿ, 'u'},
  {"skip-fields", required_ವಾದ, ಖಾಲಿ, 'f'},
  {"skip-chars", required_ವಾದ, ಖಾಲಿ, 's'},
  {"check-chars", required_ವಾದ, ಖಾಲಿ, 'w'},
  {"zero-terminated", no_ವಾದ, ಖಾಲಿ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [ಒಳಹರಿವು_2 [OUTPUT]]\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Filter adjacent matching ಸಾಲುಗಳು from ಒಳಹರಿವು_2 (or standard ಒಳಹರಿವು),\n\
writing ಗಮ್ಯ OUTPUT (or standard output).\n\
\n\
With no options, matching ಸಾಲುಗಳು are merged ಗಮ್ಯ the ಮೊದಲ occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --ಎಣಿಕೆ_2\n\
         ಪೂರ್ವಪ್ರತ್ಯಯ ಸಾಲುಗಳು by the ಸಂಖ್ಯೆ ದ occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate ಸಾಲುಗಳು, one ಗಾಗಿ each ಗುಂಪು\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate ಸಾಲುಗಳು\n\
"));
     oputs (_("\
      --all-repeated[=ವಿಧಾನ]\n\
         like -D, but allow separating groups with an ಖಾಲಿಗಳು ಸಾಲು;\n\
         ವಿಧಾನ={none(ಪೂರ್ವನಿಯೋಜಿತ),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the ಮೊದಲ N fields\n\
"));
     oputs (_("\
      --ಗುಂಪು[=ವಿಧಾನ]\n\
         show all ವಸ್ತುಗಳು, separating groups with an ಖಾಲಿಗಳು ಸಾಲು;\n\
         ವಿಧಾನ={separate(ಪೂರ್ವನಿಯೋಜಿತ),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-ಸ್ಥಿತಿ\n\
         ignore differences ಒಳ ಸ್ಥಿತಿ when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the ಮೊದಲ N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique ಸಾಲುಗಳು\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no ಇನ್ನಷ್ಟು than N characters ಒಳ ಸಾಲುಗಳು\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
     fputs (_("\
\n\
A ಕ್ಷೇತ್ರ is a ಸತತ_2 ದ blanks (usually spaces and/or TABs), then non-blank\n\
characters.  Fields are skipped before chars.\n\
"), stdout);
     fputs (_("\
\n\
'uniq' does not detect repeated ಸಾಲುಗಳು unless they are adjacent.\n\
You may want ಗಮ್ಯ ವಿಂಗಡಿಸು the ಒಳಹರಿವು ಮೊದಲ, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಸ್ಥಿರ bool
strict_posix2 (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ posix_ver = posix2_ಆವೃತ್ತಿ ();
  ಹಿಂತಿರುಗಿ 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

ಸ್ಥಿರ idx_t
ಗಾತ್ರ_opt (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *opt, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *msgid)
{
  intmax_t ಗಾತ್ರ_2;
  ಆದರೆ (LONGINT_ಅತಿಪ್ರವಾಹ < xstrtoimax (opt, ಖಾಲಿ, 10, &ಗಾತ್ರ_2, "")
      || ಗಾತ್ರ_2 < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  ಹಿಂತಿರುಗಿ MIN (ಗಾತ್ರ_2, IDX_MAX);
}

ಸ್ಥಿರ bool
newline_or_blank (mcel_t g)
{
  ಹಿಂತಿರುಗಿ g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
ಕಂಡುಹಿಡಿ_ಕ್ಷೇತ್ರ (ರಚನೆ linebuffer ಸ್ಥಿರಾಂಕ *ಸಾಲು, idx_t *plen)
{
  ಅಕ್ಷರ *lp = ಸಾಲು->ಬಫರ್;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lim = lp + ಸಾಲು->ಉದ್ದಳತೆ - 1;

  ಗಾಗಿ (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  ಗಾಗಿ (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_ಪರಿಶೀಲನೆ (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  ಆದರೆ (lim - lp <= check_chars)
    len = lim - lp;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (MB_CUR_MAX <= 1)
    len = check_chars;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ *ep = lp;
      ಗಾಗಿ (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_ಪರಿಶೀಲನೆ (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  ಹಿಂತಿರುಗಿ lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

ಸ್ಥಿರ bool
different (ಅಕ್ಷರ *old, ಅಕ್ಷರ *new, idx_t oldlen, idx_t newlen)
{
  ಆದರೆ (ignore_case)
    ಹಿಂತಿರುಗಿ oldlen != newlen || memcasecmp (old, new, oldlen);
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

ಸ್ಥಿರ ಶೂನ್ಯ
writeline (ರಚನೆ linebuffer ಸ್ಥಿರಾಂಕ *ಸಾಲು,
           bool ಹೊಂದಿಕೆ, intmax_t linecount)
{
  ಆದರೆ (! (linecount == 0 ? output_unique
         : !ಹೊಂದಿಕೆ ? output_ಮೊದಲ_repeated
         : output_later_repeated))
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (ಎಣಿಕೆ_occurrences)
    {
      ಅಕ್ಷರ buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      ಅಕ್ಷರ *ಕೊನೆ = buf + ಗಾತ್ರ buf;
      ಅಕ್ಷರ *p = ಕೊನೆ;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      ಮಾಡು
        *--p = '0' + i % 10;
      ಆಗ ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      ಆಗ (ಕೊನೆ - p < 8)
        *--p = ' ';
      ಸ್ಥಿರಾಂಕ idx_t nbytes = ಕೊನೆ - p;
      ಆದರೆ (fwrite (p, ಗಾತ್ರ (ಅಕ್ಷರ), nbytes, stdout) != nbytes)
        write_error ();
    }

  ಆದರೆ (fwrite (ಸಾಲು->ಬಫರ್, ಗಾತ್ರ (ಅಕ್ಷರ), ಸಾಲು->ಉದ್ದಳತೆ, stdout)
      != ಸಾಲು->ಉದ್ದಳತೆ)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

ಸ್ಥಿರ ಶೂನ್ಯ
check_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *infile, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *outfile, ಅಕ್ಷರ delimiter)
{
  ರಚನೆ linebuffer lb1, lb2;
  ರಚನೆ linebuffer *thisline, *prevline;

  ಆದರೆ (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  ಆದರೆ (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  thisline = &lb1;
  prevline = &lb2;

  initbuffer (thisline);
  initbuffer (prevline);

  /* The duplication in the following 'if' and 'else' blocks is an
     optimization to distinguish between when we can print input
     lines immediately (1. & 2.) or not.

     1. --group => all input lines are printed.
        checking for unique/duplicated lines is used only for printing
        group separators.

     2. The default case in which none of these options has been specified:
          --count, --repeated,  --all-repeated, --unique
        In the default case, this optimization lets uniq output each different
        line right away, without waiting to see if the next one is different.

     3. All other cases.
  */
  ಆದರೆ (output_unique && output_ಮೊದಲ_repeated && !ಎಣಿಕೆ_occurrences)
    {
      ಅಕ್ಷರ *prevfield = ಖಾಲಿ;
      idx_t prevlen;
      bool ಮೊದಲ_ಗುಂಪು_printed = false;

      ಆಗ (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          ಅಕ್ಷರ *thisfield = ಕಂಡುಹಿಡಿ_ಕ್ಷೇತ್ರ (thisline, &thislen);
          bool new_ಗುಂಪು = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          ಆದರೆ (new_ಗುಂಪು && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (ಮೊದಲ_ಗುಂಪು_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            ಅಕ್ಷರ_ಬರೆ (delimiter);

          ಆದರೆ (new_ಗುಂಪು || grouping != GM_NONE)
            {
              ಆದರೆ (fwrite (thisline->ಬಫರ್, ಗಾತ್ರ (ಅಕ್ಷರ), thisline->ಉದ್ದಳತೆ,
                  stdout) != thisline->ಉದ್ದಳತೆ)
                write_error ();

              ಅದಲುಬದಲು_ಸಾಲುಗಳು (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              ಮೊದಲ_ಗುಂಪು_printed = true;
            }
        }
      ಆದರೆ ((grouping == GM_BOTH || grouping == GM_APPEND) && ಮೊದಲ_ಗುಂಪು_printed)
        ಅಕ್ಷರ_ಬರೆ (delimiter);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (!readlinebuffer_delim (prevline, stdin, delimiter))
        ಹೋಗು closefiles;

      idx_t prevlen;
      ಅಕ್ಷರ *prevfield = ಕಂಡುಹಿಡಿ_ಕ್ಷೇತ್ರ (prevline, &prevlen);
      intmax_t ಹೊಂದಿಕೆ_ಎಣಿಕೆ = 0;
      bool ಮೊದಲ_delimiter = true;

      ಆಗ (!feof (stdin))
        {
          ಆದರೆ (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              ಆದರೆ (ferror (stdin))
                ಹೋಗು closefiles;
              ನಿಲ್ಲಿಸು;
            }
          idx_t thislen;
          ಅಕ್ಷರ *thisfield = ಕಂಡುಹಿಡಿ_ಕ್ಷೇತ್ರ (thisline, &thislen);
          bool ಹೊಂದಿಕೆ = !different (thisfield, prevfield, thislen, prevlen);
          ಹೊಂದಿಕೆ_ಎಣಿಕೆ += ಹೊಂದಿಕೆ;

          ಆದರೆ (ಹೊಂದಿಕೆ_ಎಣಿಕೆ == INTMAX_MAX)
            {
              ಆದರೆ (ಎಣಿಕೆ_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              ಹೊಂದಿಕೆ_ಎಣಿಕೆ--;
            }

          ಆದರೆ (delimit_groups != DM_NONE)
            {
              ಆದರೆ (!ಹೊಂದಿಕೆ)
                {
                  ಆದರೆ (ಹೊಂದಿಕೆ_ಎಣಿಕೆ) /* a previous match */
                    ಮೊದಲ_delimiter = false; /* Only used when DM_SEPARATE */
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಹೊಂದಿಕೆ_ಎಣಿಕೆ == 1)
                {
                  ಆದರೆ ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !ಮೊದಲ_delimiter))
                    ಅಕ್ಷರ_ಬರೆ (delimiter);
                }
            }

          ಆದರೆ (!ಹೊಂದಿಕೆ || output_later_repeated)
            {
              writeline (prevline, ಹೊಂದಿಕೆ, ಹೊಂದಿಕೆ_ಎಣಿಕೆ);
              ಅದಲುಬದಲು_ಸಾಲುಗಳು (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              ಆದರೆ (!ಹೊಂದಿಕೆ)
                ಹೊಂದಿಕೆ_ಎಣಿಕೆ = 0;
            }
        }

      writeline (prevline, false, ಹೊಂದಿಕೆ_ಎಣಿಕೆ);
    }

 closefiles:
  ಆದರೆ (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  ಬಿಡುಗಡೆ (lb1.ಬಫರ್);
  ಬಿಡುಗಡೆ (lb2.ಬಫರ್);
}

ಎಣಿಕೆ Skip_ಕ್ಷೇತ್ರ_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ಖಾಲಿ);
  ಎಣಿಕೆ Skip_ಕ್ಷೇತ್ರ_option_type skip_ಕ್ಷೇತ್ರ_option_type = SFO_NONE;
  ಪೂರ್ಣಾಂಕ nfiles = 0;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file[2];
  ಅಕ್ಷರ delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_ಬಳಸಿದ = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      ಆದರೆ (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, ಖಾಲಿ))
              == -1))
        {
          ಆದರೆ (argc <= optind)
            ನಿಲ್ಲಿಸು;
          ಆದರೆ (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 1:
          {
            intmax_t ಗಾತ್ರ_2;
            ಆದರೆ (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, ಖಾಲಿ, 10, &ಗಾತ್ರ_2, "")
                    <= LONGINT_ಅತಿಪ್ರವಾಹ))
              skip_chars = MIN (ಗಾತ್ರ_2, IDX_MAX);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            ಇಲ್ಲದಿದ್ದರೆ
              file[nfiles++] = optarg;
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '0':
        ಸ್ಥಿತಿ '1':
        ಸ್ಥಿತಿ '2':
        ಸ್ಥಿತಿ '3':
        ಸ್ಥಿತಿ '4':
        ಸ್ಥಿತಿ '5':
        ಸ್ಥಿತಿ '6':
        ಸ್ಥಿತಿ '7':
        ಸ್ಥಿತಿ '8':
        ಸ್ಥಿತಿ '9':
          {
            ಆದರೆ (skip_ಕ್ಷೇತ್ರ_option_type == SFO_NEW)
              skip_fields = 0;

            ಆದರೆ (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_ಕ್ಷೇತ್ರ_option_type = SFO_OBSOLETE;
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'c':
          ಎಣಿಕೆ_occurrences = true;
          output_option_ಬಳಸಿದ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'd':
          output_unique = false;
          output_option_ಬಳಸಿದ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'D':
          output_unique = false;
          output_later_repeated = true;
          ಆದರೆ (optarg == ಖಾಲಿ)
            delimit_groups = DM_NONE;
          ಇಲ್ಲದಿದ್ದರೆ
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_ವಿಧಾನ_string,
                                        delimit_ವಿಧಾನ_map);
          output_option_ಬಳಸಿದ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಗುಂಪು_OPTION:
          ಆದರೆ (optarg == ಖಾಲಿ)
            grouping = GM_SEPARATE;
          ಇಲ್ಲದಿದ್ದರೆ
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_ವಿಧಾನ_string,
                                  grouping_ವಿಧಾನ_map);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
          skip_ಕ್ಷೇತ್ರ_option_type = SFO_NEW;
          skip_fields = ಗಾತ್ರ_opt (optarg,
                                  N_("invalid number of fields to skip"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'i':
          ignore_case = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          skip_chars = ಗಾತ್ರ_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'u':
          output_ಮೊದಲ_repeated = false;
          output_option_ಬಳಸಿದ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'w':
          check_chars = ಗಾತ್ರ_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'z':
          delimiter = '\0';
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  ಆದರೆ (grouping != GM_NONE && output_option_ಬಳಸಿದ)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (grouping != GM_NONE && ಎಣಿಕೆ_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (ಎಣಿಕೆ_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
