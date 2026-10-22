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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "linebuffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mcel.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "posixver.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "skipchars.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdio--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "memcasecmp.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "uniq"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

ਸਥਿਰ ਖਾਲੀ
ਅਦਲਾ_ਬਦਲੀ_ਸਤਰਾਂ (ਬਣਤਰ linebuffer **a, ਬਣਤਰ linebuffer **b)
{
  ਬਣਤਰ linebuffer *tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Number of fields to skip on each line when doing comparisons. */
ਸਥਿਰ idx_t skip_fields = false;

/* Number of chars to skip after skipping any fields. */
ਸਥਿਰ idx_t skip_chars = false;

/* Number of chars to compare. */
ਸਥਿਰ idx_t check_chars = IDX_MAX;

/* Whether to precede the output lines with a count of the number of
   times they occurred in the input. */
ਸਥਿਰ bool ਗਿਣਤੀ_occurrences = false;

/* Which lines to output: unique lines, the first of a group of
   repeated lines, and the second and subsequent of a group of
   repeated lines.  */
ਸਥਿਰ bool output_unique = true;
ਸਥਿਰ bool output_ਪਹਿਲਾ_repeated = true;
ਸਥਿਰ bool output_later_repeated = false;

/* If true, ignore case when comparing.  */
ਸਥਿਰ bool ignore_case;

ਗਿਣਤੀ delimit_ਵਿਧੀ
{
  /* No delimiters output.  --all-repeated[=none] */
  DM_NONE,

  /* Delimiter precedes all groups.  --all-repeated=prepend */
  DM_PREPEND,

  /* Delimit all groups.  --all-repeated=separate */
  DM_SEPARATE
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ delimit_ਵਿਧੀ_string[] =
{
  "none", "prepend", "separate", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ delimit_ਵਿਧੀ ਅਚਲ delimit_ਵਿਧੀ_map[] =
{
  DM_NONE, DM_PREPEND, DM_SEPARATE
};

/* Select whether/how to delimit groups of duplicate lines.  */
ਸਥਿਰ ਗਿਣਤੀ delimit_ਵਿਧੀ delimit_groups = DM_NONE;

ਗਿਣਤੀ grouping_ਵਿਧੀ
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

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ grouping_ਵਿਧੀ_string[] =
{
  "prepend", "append", "separate", "both", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ grouping_ਵਿਧੀ ਅਚਲ grouping_ਵਿਧੀ_map[] =
{
  GM_PREPEND, GM_APPEND, GM_SEPARATE, GM_BOTH
};

ਸਥਿਰ ਗਿਣਤੀ grouping_ਵਿਧੀ grouping = GM_NONE;

ਗਿਣਤੀ
{
  ਸਮੂਹ_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"count", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"repeated", no_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"all-repeated", optional_ਦਲੀਲ, ਨਿਰਾ, 'D'},
  {"group", optional_ਦਲੀਲ, ਨਿਰਾ, ਸਮੂਹ_OPTION},
  {"ignore-case", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"unique", no_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"skip-fields", required_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"skip-chars", required_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"check-chars", required_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  {"zero-terminated", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [ਨਿਵੇਸ਼_2 [OUTPUT]]\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Filter adjacent matching ਸਤਰਾਂ from ਨਿਵੇਸ਼_2 (or standard ਨਿਵੇਸ਼),\n\
writing ਮੰਜ਼ਿਲ OUTPUT (or standard output).\n\
\n\
With no options, matching ਸਤਰਾਂ are merged ਮੰਜ਼ਿਲ the ਪਹਿਲਾ occurrence.\n\
"), stdout);

      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --ਗਿਣਤੀ_2\n\
         ਅਗੇਤਰ ਸਤਰਾਂ by the ਸੰਖਿਆ ਦਾ occurrences\n\
"));
     oputs (_("\
  -d, --repeated\n\
         only print duplicate ਸਤਰਾਂ, one ਲਈ each ਸਮੂਹ\n\
"));
     oputs (_("\
  -D\n\
         print all duplicate ਸਤਰਾਂ\n\
"));
     oputs (_("\
      --all-repeated[=ਵਿਧੀ]\n\
         like -D, but allow separating groups with an ਖਾਲੀ_ਥਾਵਾਂ ਸਤਰ;\n\
         ਵਿਧੀ={none(ਮੂਲ),prepend,separate}\n\
"));
     oputs (_("\
  -f, --skip-fields=N\n\
         avoid comparing the ਪਹਿਲਾ N fields\n\
"));
     oputs (_("\
      --ਸਮੂਹ[=ਵਿਧੀ]\n\
         show all ਚੀਜ਼ਾਂ, separating groups with an ਖਾਲੀ_ਥਾਵਾਂ ਸਤਰ;\n\
         ਵਿਧੀ={separate(ਮੂਲ),prepend,append,both}\n\
"));
     oputs (_("\
  -i, --ignore-ਹਾਲਤ\n\
         ignore differences ਵਿੱਚ ਹਾਲਤ when comparing\n\
"));
     oputs (_("\
  -s, --skip-chars=N\n\
         avoid comparing the ਪਹਿਲਾ N characters\n\
"));
     oputs (_("\
  -u, --unique\n\
         only print unique ਸਤਰਾਂ\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         ਸਤਰ delimiter is NUL, not newline\n\
"));
     oputs (_("\
  -w, --check-chars=N\n\
         compare no ਹੋਰ than N characters ਵਿੱਚ ਸਤਰਾਂ\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
     fputs (_("\
\n\
A ਖੇਤਰ is a ਲੜੀਵਾਰ_2 ਦਾ blanks (usually spaces and/or TABs), then non-blank\n\
characters.  Fields are skipped before chars.\n\
"), stdout);
     fputs (_("\
\n\
'uniq' does not detect repeated ਸਤਰਾਂ unless they are adjacent.\n\
You may want ਮੰਜ਼ਿਲ ਛਾਂਟੋ the ਨਿਵੇਸ਼ ਪਹਿਲਾ, or use 'sort -u' without 'uniq'.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਸਥਿਰ bool
strict_posix2 (ਖਾਲੀ)
{
  ਪੂਰਨ posix_ver = posix2_ਸੰਸਕਰਣ ();
  ਵਾਪਸ 200112 <= posix_ver && posix_ver < 200809;
}

/* Convert OPT to idx_t, reporting an error using MSGID if OPT is
   invalid.  Silently convert too-large values to IDX_MAX.  */

ਸਥਿਰ idx_t
ਮਾਪ_opt (ਅੱਖਰ ਅਚਲ *opt, ਅੱਖਰ ਅਚਲ *msgid)
{
  intmax_t ਮਾਪ;
  ਜੇ (LONGINT_ਉਛਾਲ < xstrtoimax (opt, ਨਿਰਾ, 10, &ਮਾਪ, "")
      || ਮਾਪ < 0)
    error (EXIT_FAILURE, 0, "%s: %s", opt, _(msgid));
  ਵਾਪਸ MIN (ਮਾਪ, IDX_MAX);
}

ਸਥਿਰ bool
newline_or_blank (mcel_t g)
{
  ਵਾਪਸ g.ch == '\n' || c32issep (g.ch);
}

/* Given a linebuffer LINE,
   return a pointer to the beginning of the line's field to be compared.
   Store into *PLEN the length in bytes of FIELD.  */

ਸਥਿਰ ਅੱਖਰ *
ਲੱਭੋ_ਖੇਤਰ (ਬਣਤਰ linebuffer ਅਚਲ *ਸਤਰ, idx_t *plen)
{
  ਅੱਖਰ *lp = ਸਤਰ->ਬਫਰ;
  ਅੱਖਰ ਅਚਲ *lim = lp + ਸਤਰ->ਲੰਮਾਈ - 1;

  ਲਈ (idx_t i = skip_fields; 0 < i && lp < lim; i--)
    {
      lp = skip_buf_matching (lp, lim, newline_or_blank, true);
      lp = skip_buf_matching (lp, lim, newline_or_blank, false);
    }

  ਲਈ (idx_t i = skip_chars; 0 < i && lp < lim; i--)
    lp += mcel_ਜਾਂਚ (lp, lim).len;

  /* Compute the length in bytes cheaply if possible; otherwise, scan.  */
  idx_t len;
  ਜੇ (lim - lp <= check_chars)
    len = lim - lp;
  ਨਹੀਂਤਾਂ ਜੇ (MB_CUR_MAX <= 1)
    len = check_chars;
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ *ep = lp;
      ਲਈ (idx_t i = check_chars; 0 < i && ep < lim; i--)
        ep += mcel_ਜਾਂਚ (ep, lim).len;
      len = ep - lp;
    }

  *plen = len;
  ਵਾਪਸ lp;
}

/* Return false if two strings OLD and NEW match, true if not.
   OLD and NEW point not to the beginnings of the lines
   but rather to the beginnings of the fields to compare.
   OLDLEN and NEWLEN are their lengths. */

ਸਥਿਰ bool
different (ਅੱਖਰ *old, ਅੱਖਰ *new, idx_t oldlen, idx_t newlen)
{
  ਜੇ (ignore_case)
    ਵਾਪਸ oldlen != newlen || memcasecmp (old, new, oldlen);
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ oldlen != newlen || memcmp (old, new, oldlen);
}

/* Output the line in linebuffer LINE to standard output
   provided that the switches say it should be output.
   MATCH is true if the line matches the previous line.
   If requested, print the number of times it occurred, as well;
   LINECOUNT + 1 is the number of times that the line occurred. */

ਸਥਿਰ ਖਾਲੀ
writeline (ਬਣਤਰ linebuffer ਅਚਲ *ਸਤਰ,
           bool ਮੇਲ, intmax_t linecount)
{
  ਜੇ (! (linecount == 0 ? output_unique
         : !ਮੇਲ ? output_ਪਹਿਲਾ_repeated
         : output_later_repeated))
    ਵਾਪਸ;

  ਜੇ (ਗਿਣਤੀ_occurrences)
    {
      ਅੱਖਰ buf[7 + INT_BUFSIZE_BOUND (intmax_t)];
      ਅੱਖਰ *ਸਿਰਾ = buf + ਆਕਾਰ buf;
      ਅੱਖਰ *p = ਸਿਰਾ;
      *--p = ' ';
      intmax_t i = linecount + 1;
      affirm (0 < i);
      ਕਰੋ
        *--p = '0' + i % 10;
      ਜਦੋਂ ((i /= 10));
      /* 7 characters, padded with spaces if LINECOUNT + 1 is too small.  */
      ਜਦੋਂ (ਸਿਰਾ - p < 8)
        *--p = ' ';
      ਅਚਲ idx_t nbytes = ਸਿਰਾ - p;
      ਜੇ (fwrite (p, ਆਕਾਰ (ਅੱਖਰ), nbytes, stdout) != nbytes)
        write_error ();
    }

  ਜੇ (fwrite (ਸਤਰ->ਬਫਰ, ਆਕਾਰ (ਅੱਖਰ), ਸਤਰ->ਲੰਮਾਈ, stdout)
      != ਸਤਰ->ਲੰਮਾਈ)
    write_error ();
}

/* Process input file INFILE with output to OUTFILE.
   If either is "-", use the standard I/O stream for it instead. */

ਸਥਿਰ ਖਾਲੀ
check_file (ਅੱਖਰ ਅਚਲ *infile, ਅੱਖਰ ਅਚਲ *outfile, ਅੱਖਰ delimiter)
{
  ਬਣਤਰ linebuffer lb1, lb2;
  ਬਣਤਰ linebuffer *thisline, *prevline;

  ਜੇ (! (streq (infile, "-") || freopen (infile, "r", stdin)))
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  ਜੇ (! (streq (outfile, "-") || freopen (outfile, "w", stdout)))
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
  ਜੇ (output_unique && output_ਪਹਿਲਾ_repeated && !ਗਿਣਤੀ_occurrences)
    {
      ਅੱਖਰ *prevfield = ਨਿਰਾ;
      idx_t prevlen;
      bool ਪਹਿਲਾ_ਸਮੂਹ_printed = false;

      ਜਦੋਂ (!feof (stdin)
             && readlinebuffer_delim (thisline, stdin, delimiter))
        {
          idx_t thislen;
          ਅੱਖਰ *thisfield = ਲੱਭੋ_ਖੇਤਰ (thisline, &thislen);
          bool new_ਸਮੂਹ = (!prevfield
                            || different (thisfield, prevfield,
                                          thislen, prevlen));

          ਜੇ (new_ਸਮੂਹ && grouping != GM_NONE
              && (grouping == GM_PREPEND || grouping == GM_BOTH
                  || (ਪਹਿਲਾ_ਸਮੂਹ_printed && (grouping == GM_APPEND
                                              || grouping == GM_SEPARATE))))
            ਅੱਖਰ_ਲਿਖੋ (delimiter);

          ਜੇ (new_ਸਮੂਹ || grouping != GM_NONE)
            {
              ਜੇ (fwrite (thisline->ਬਫਰ, ਆਕਾਰ (ਅੱਖਰ), thisline->ਲੰਮਾਈ,
                  stdout) != thisline->ਲੰਮਾਈ)
                write_error ();

              ਅਦਲਾ_ਬਦਲੀ_ਸਤਰਾਂ (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              ਪਹਿਲਾ_ਸਮੂਹ_printed = true;
            }
        }
      ਜੇ ((grouping == GM_BOTH || grouping == GM_APPEND) && ਪਹਿਲਾ_ਸਮੂਹ_printed)
        ਅੱਖਰ_ਲਿਖੋ (delimiter);
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (!readlinebuffer_delim (prevline, stdin, delimiter))
        ਜਾਓ closefiles;

      idx_t prevlen;
      ਅੱਖਰ *prevfield = ਲੱਭੋ_ਖੇਤਰ (prevline, &prevlen);
      intmax_t ਮੇਲ_ਗਿਣਤੀ = 0;
      bool ਪਹਿਲਾ_delimiter = true;

      ਜਦੋਂ (!feof (stdin))
        {
          ਜੇ (!readlinebuffer_delim (thisline, stdin, delimiter))
            {
              ਜੇ (ferror (stdin))
                ਜਾਓ closefiles;
              ਰੋਕੋ;
            }
          idx_t thislen;
          ਅੱਖਰ *thisfield = ਲੱਭੋ_ਖੇਤਰ (thisline, &thislen);
          bool ਮੇਲ = !different (thisfield, prevfield, thislen, prevlen);
          ਮੇਲ_ਗਿਣਤੀ += ਮੇਲ;

          ਜੇ (ਮੇਲ_ਗਿਣਤੀ == INTMAX_MAX)
            {
              ਜੇ (ਗਿਣਤੀ_occurrences)
                error (EXIT_FAILURE, 0, _("too many repeated lines"));
              ਮੇਲ_ਗਿਣਤੀ--;
            }

          ਜੇ (delimit_groups != DM_NONE)
            {
              ਜੇ (!ਮੇਲ)
                {
                  ਜੇ (ਮੇਲ_ਗਿਣਤੀ) /* a previous match */
                    ਪਹਿਲਾ_delimiter = false; /* Only used when DM_SEPARATE */
                }
              ਨਹੀਂਤਾਂ ਜੇ (ਮੇਲ_ਗਿਣਤੀ == 1)
                {
                  ਜੇ ((delimit_groups == DM_PREPEND)
                      || (delimit_groups == DM_SEPARATE
                          && !ਪਹਿਲਾ_delimiter))
                    ਅੱਖਰ_ਲਿਖੋ (delimiter);
                }
            }

          ਜੇ (!ਮੇਲ || output_later_repeated)
            {
              writeline (prevline, ਮੇਲ, ਮੇਲ_ਗਿਣਤੀ);
              ਅਦਲਾ_ਬਦਲੀ_ਸਤਰਾਂ (&prevline, &thisline);
              prevfield = thisfield;
              prevlen = thislen;
              ਜੇ (!ਮੇਲ)
                ਮੇਲ_ਗਿਣਤੀ = 0;
            }
        }

      writeline (prevline, false, ਮੇਲ_ਗਿਣਤੀ);
    }

 closefiles:
  ਜੇ (ferror (stdin) || fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, _("error reading %s"), quoteaf (infile));

  /* stdout is handled via the atexit-invoked close_stdout function.  */

  ਮੁਕਤ (lb1.ਬਫਰ);
  ਮੁਕਤ (lb2.ਬਫਰ);
}

ਗਿਣਤੀ Skip_ਖੇਤਰ_option_type
  {
    SFO_NONE,
    SFO_OBSOLETE,
    SFO_NEW
  };

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ optc = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ਨਿਰਾ);
  ਗਿਣਤੀ Skip_ਖੇਤਰ_option_type skip_ਖੇਤਰ_option_type = SFO_NONE;
  ਪੂਰਨ nfiles = 0;
  ਅੱਖਰ ਅਚਲ *file[2];
  ਅੱਖਰ delimiter = '\n';	/* change with --zero-terminated, -z */
  bool output_option_ਵਰਤਿਆ = false;   /* if true, one of -u/-d/-D/-c was used */

  file[0] = file[1] = "-";
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ (true)
    {
      /* Parse an operand with leading "+" as a file after "--" was
         seen; or if pedantic and a file was seen; or if not
         obsolete.  */

      ਜੇ (optc == -1
          || (posixly_correct && nfiles != 0)
          || ((optc = getopt_long (argc, argv,
                                   "-0123456789Dcdf:is:uw:z",
                                   longopts, ਨਿਰਾ))
              == -1))
        {
          ਜੇ (argc <= optind)
            ਰੋਕੋ;
          ਜੇ (nfiles == 2)
            {
              error (0, 0, _("extra operand %s"), quote (argv[optind]));
              usage (EXIT_FAILURE);
            }
          file[nfiles++] = argv[optind++];
        }
      ਨਹੀਂਤਾਂ ਚੋਣ (optc)
        {
        ਹਾਲਤ 1:
          {
            intmax_t ਮਾਪ;
            ਜੇ (optarg[0] == '+'
                && ! strict_posix2 ()
                && (xstrtoimax (optarg, ਨਿਰਾ, 10, &ਮਾਪ, "")
                    <= LONGINT_ਉਛਾਲ))
              skip_chars = MIN (ਮਾਪ, IDX_MAX);
            ਨਹੀਂਤਾਂ ਜੇ (nfiles == 2)
              {
                error (0, 0, _("extra operand %s"), quote (optarg));
                usage (EXIT_FAILURE);
              }
            ਨਹੀਂਤਾਂ
              file[nfiles++] = optarg;
          }
          ਰੋਕੋ;

        ਹਾਲਤ '0':
        ਹਾਲਤ '1':
        ਹਾਲਤ '2':
        ਹਾਲਤ '3':
        ਹਾਲਤ '4':
        ਹਾਲਤ '5':
        ਹਾਲਤ '6':
        ਹਾਲਤ '7':
        ਹਾਲਤ '8':
        ਹਾਲਤ '9':
          {
            ਜੇ (skip_ਖੇਤਰ_option_type == SFO_NEW)
              skip_fields = 0;

            ਜੇ (!DECIMAL_DIGIT_ACCUMULATE (skip_fields, optc - '0'))
              skip_fields = IDX_MAX;

            skip_ਖੇਤਰ_option_type = SFO_OBSOLETE;
          }
          ਰੋਕੋ;

        ਹਾਲਤ 'c':
          ਗਿਣਤੀ_occurrences = true;
          output_option_ਵਰਤਿਆ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'd':
          output_unique = false;
          output_option_ਵਰਤਿਆ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'D':
          output_unique = false;
          output_later_repeated = true;
          ਜੇ (optarg == ਨਿਰਾ)
            delimit_groups = DM_NONE;
          ਨਹੀਂਤਾਂ
            delimit_groups = XARGMATCH ("--all-repeated", optarg,
                                        delimit_ਵਿਧੀ_string,
                                        delimit_ਵਿਧੀ_map);
          output_option_ਵਰਤਿਆ = true;
          ਰੋਕੋ;

        ਹਾਲਤ ਸਮੂਹ_OPTION:
          ਜੇ (optarg == ਨਿਰਾ)
            grouping = GM_SEPARATE;
          ਨਹੀਂਤਾਂ
            grouping = XARGMATCH ("--group", optarg,
                                  grouping_ਵਿਧੀ_string,
                                  grouping_ਵਿਧੀ_map);
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
          skip_ਖੇਤਰ_option_type = SFO_NEW;
          skip_fields = ਮਾਪ_opt (optarg,
                                  N_("invalid number of fields to skip"));
          ਰੋਕੋ;

        ਹਾਲਤ 'i':
          ignore_case = true;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          skip_chars = ਮਾਪ_opt (optarg,
                                 N_("invalid number of bytes to skip"));
          ਰੋਕੋ;

        ਹਾਲਤ 'u':
          output_ਪਹਿਲਾ_repeated = false;
          output_option_ਵਰਤਿਆ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'w':
          check_chars = ਮਾਪ_opt (optarg,
                                  N_("invalid number of bytes to compare"));
          ਰੋਕੋ;

        ਹਾਲਤ 'z':
          delimiter = '\0';
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  /* Note we could allow --group with -D at least, and that would
     avoid the need to specify a grouping method to --all-repeated.
     It was thought best to avoid deprecating those parameters though
     and keep --group separate to other options.  */
  ਜੇ (grouping != GM_NONE && output_option_ਵਰਤਿਆ)
    {
      error (0, 0, _("--group is mutually exclusive with -c/-d/-D/-u"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (grouping != GM_NONE && ਗਿਣਤੀ_occurrences)
    {
      error (0, 0,
           _("grouping and printing repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (ਗਿਣਤੀ_occurrences && output_later_repeated)
    {
      error (0, 0,
           _("printing all duplicated lines and repeat counts is meaningless"));
      usage (EXIT_FAILURE);
    }

  check_file (file[0], file[1], delimiter);

  ਵਾਪਸ EXIT_SUCCESS;
}
