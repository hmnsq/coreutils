/* set-fields.c -- common functions for parsing field list
   Copyright (C) 2015-2026 Free Software Foundation, Inc.

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

/* Extracted from cut.c by Assaf Gordon */

#అన్తర్భావ <config.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "set-fields.h"

/* Array of `struct field_range_pair' holding all the finite ranges. */
నిర్మాణం క్షేత్రం_range_pair *frp;

/* Number of finite ranges specified by the user. */
idx_t n_frp;

/* Number of `struct field_range_pair's allocated. */
స్థిర idx_t n_frp_allocated;

#నిర్వచించు FATAL_ERROR(సందేశం)                                            \
  చేయి                                                                    \
    {                                                                   \
      error (0, 0, (సందేశం));                                          \
      usage (EXIT_FAILURE);                                             \
    }                                                                   \
  అయితే (0)

/* Append LOW, HIGH to the list RP of range pairs, allocating additional
   space if necessary.  Update global variable N_FRP.  When allocating,
   update global variable N_FRP_ALLOCATED.  */
స్థిర శూన్య
కూడు_range_pair (uintmax_t lo, uintmax_t hi)
{
  ఒకవేళ (n_frp == n_frp_allocated)
    frp = xpalloc (frp, &n_frp_allocated, 1, -1, పరిమాణం *frp);
  frp[n_frp].lo = lo;
  frp[n_frp].hi = hi;
  ++n_frp;
}


/* Comparison function for qsort to order the list of
   struct field_range_pairs.  */
స్థిర పూర్ణాంకం
compare_ranges (స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
  నిర్మాణం క్షేత్రం_range_pair స్థిరరాశి *ap = a, *bp = b;
  తిరిగి _GL_CMP (ap->lo, bp->lo);
}

/* Reallocate Range Pair entries, with corresponding
   entries outside the range of each specified entry.  */

స్థిర శూన్య
complement_rp (శూన్య)
{
  నిర్మాణం క్షేత్రం_range_pair *c = frp;
  idx_t n = n_frp;

  frp = ఖాళీ;
  n_frp = 0;
  n_frp_allocated = 0;

  ఒకవేళ (c[0].lo > 1)
    కూడు_range_pair (1, c[0].lo - 1);

  కొరకు (idx_t i = 1; i < n; ++i)
    {
      ఒకవేళ (c[i - 1].hi + 1 == c[i].lo)
        కొనసాగు;

      కూడు_range_pair (c[i - 1].hi + 1, c[i].lo - 1);
    }

  ఒకవేళ (c[n - 1].hi < UINTMAX_MAX)
    కూడు_range_pair (c[n - 1].hi + 1, UINTMAX_MAX);

  విడుదల (c);
}

/* Given the list of field or byte range specifications FIELDSTR,
   allocate and initialize the FRP array. FIELDSTR should
   be composed of one or more numbers or ranges of numbers, separated
   by blanks or commas.  Incomplete ranges may be given: '-m' means '1-m';
   'n-' means 'n' through end of line.
   n=0 and n>=UINTMAX_MAX values will trigger an error.

   if SETFLD_ALLOW_DASH option is used, a single '-' means all fields
   (otherwise a single dash triggers an error).

   if SETFLD_COMPLEMENT option is used, the specified field list
   is complemented (e.g. '1-3' will result in fields '4-').

   if SETFLD_ERRMSG_USE_POS option is used, error messages
   will say 'position' (or 'byte/character positions')
   instead of fields (used with cut -b/-c).

   The function terminates on failure.

   Upon return, the FRP array is initialized to contain
   a non-overlapping, increasing list of field ranges.

   N_FRP holds the number of field ranges in the FRP array.

   The first field is stored as 1 (zero is not used).
   An open-ended range (i.e., until the last field of the input line)
   is indicated with hi = UINTMAX_MAX.

   A sentinel of UINTMAX_MAX/UINTMAX_MAX is always added as the last
   field range pair.

   Examples:
   given '1-2,4', frp = [ { .lo = 1,           .hi = 2 },
                          { .lo = 4,           .hi = 4 },
                          { .lo = UINTMAX_MAX, .hi = UINTMAX_MAX } ];

   given '3-',    frp = [ { .lo = 3,           .hi = UINTMAX_MAX },
                          { .lo = UINTMAX_MAX, .hi = UINTMAX_MAX } ];
*/
శూన్య
సమితి_fields (అక్షరం స్థిరరాశి *fieldstr, గుర్తులేని పూర్ణాంకం options)
{
  uintmax_t initial = 1;	/* Value of first number in a range.  */
  uintmax_t విలువ = 0;		/* If nonzero, a number being accumulated.  */
  bool lhs_specified = false;
  bool rhs_specified = false;
  bool dash_దొరికింది = false;	/* True if a '-' is found in this field.  */

  bool లోపల_digits = false;

  /* Collect and store in RP the range end points. */

  /* Special case: '--field=-' means all fields, emulate '--field=1-' . */
  ఒకవేళ ((options & SETFLD_ALLOW_DASH) && streq (fieldstr,"-"))
    {
      విలువ = 1;
      lhs_specified = true;
      dash_దొరికింది = true;
      fieldstr++;
    }

  అయితే (true)
    {
      ఒకవేళ (*fieldstr == '-')
        {
          లోపల_digits = false;
          /* Starting a range. */
          ఒకవేళ (dash_దొరికింది)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("invalid byte or character range")
                         : _("invalid field range"));

          dash_దొరికింది = true;
          fieldstr++;

          ఒకవేళ (lhs_specified && !విలువ)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("byte/character positions are numbered from 1")
                         : _("fields are numbered from 1"));

          initial = (lhs_specified ? విలువ : 1);
          విలువ = 0;
        }
      లేకపోతే ఒకవేళ (*fieldstr == ','
               || isblank (గమ్యం_uchar (*fieldstr)) || *fieldstr == '\0')
        {
          లోపల_digits = false;
          /* Ending the string, or this field/byte sublist. */
          ఒకవేళ (dash_దొరికింది)
            {
              dash_దొరికింది = false;

              ఒకవేళ (!lhs_specified && !rhs_specified)
                {
                  /* if a lone dash is allowed, emulate '1-' for all fields */
                  ఒకవేళ (options & SETFLD_ALLOW_DASH)
                    initial = 1;
                  లేకపోతే
                    FATAL_ERROR (_("invalid range with no endpoint: -"));
                }

              /* A range.  Possibilities: -n, m-n, n-.
                 In any case, 'initial' contains the start of the range. */
              ఒకవేళ (!rhs_specified)
                {
                  /* 'n-'.  From 'initial' to end of line. */
                  కూడు_range_pair (initial, UINTMAX_MAX);
                }
              లేకపోతే
                {
                  /* 'm-n' or '-n' (1-n). */
                  ఒకవేళ (విలువ < initial)
                    FATAL_ERROR (_("invalid decreasing range"));

                  కూడు_range_pair (initial, విలువ);
                }
              విలువ = 0;
            }
          లేకపోతే
            {
              /* A simple field number, not a range. */
              ఒకవేళ (విలువ == 0)
                FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                             ? _("byte/character positions are numbered from 1")
                             : _("fields are numbered from 1"));

              కూడు_range_pair (విలువ, విలువ);
              విలువ = 0;
            }

          ఒకవేళ (*fieldstr == '\0')
            ఆపు;

          fieldstr++;
          lhs_specified = false;
          rhs_specified = false;
        }
      లేకపోతే ఒకవేళ (c_isdigit (*fieldstr))
        {
          /* Record beginning of digit string, in case we have to
             complain about it.  */
          స్థిర అక్షరం స్థిరరాశి *అంకె_ప్రారంభం;
          ఒకవేళ (!లోపల_digits || !అంకె_ప్రారంభం)
            అంకె_ప్రారంభం = fieldstr;
          లోపల_digits = true;

          ఒకవేళ (dash_దొరికింది)
            rhs_specified = 1;
          లేకపోతే
            lhs_specified = 1;

          /* Detect overflow.  */
          ఒకవేళ (!DECIMAL_DIGIT_ACCUMULATE (విలువ, *fieldstr - '0')
              || విలువ == UINTMAX_MAX)
            {
              /* In case the user specified -c$(echo 2^64|bc),22,
                 complain only about the first number.  */
              అక్షరం *bad_అంకె = ximemdup0 (అంకె_ప్రారంభం,
                                         strspn (అంకె_ప్రారంభం, "0123456789"));
              error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                           ?_("byte/character offset %s is too large")
                           :_("field number %s is too large"),
                           quote (bad_అంకె));
              విడుదల (bad_అంకె);
              usage (EXIT_FAILURE);
            }

          fieldstr++;
        }
      లేకపోతే
        {
          error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                       ?_("invalid byte/character position %s")
                       :_("invalid field value %s"),
                       quote (fieldstr));
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (!n_frp)
    FATAL_ERROR ((options&SETFLD_ERRMSG_USE_POS)
                 ? _("missing list of byte/character positions")
                 : _("missing list of fields"));

  qsort (frp, n_frp, పరిమాణం (frp[0]), compare_ranges);

  /* Merge range pairs (e.g. `2-5,3-4' becomes `2-5'). */
  కొరకు (idx_t i = 0; i < n_frp; ++i)
    {
      కొరకు (idx_t j = i + 1; j < n_frp; ++j)
        {
          ఒకవేళ (frp[j].lo <= frp[i].hi)
            {
              frp[i].hi = MAX (frp[j].hi, frp[i].hi);
              memmove (frp + j, frp + j + 1, (n_frp - j - 1) * పరిమాణం *frp);
              n_frp--;
              j--;
            }
          లేకపోతే
            ఆపు;
        }
    }

  ఒకవేళ (options & SETFLD_COMPLEMENT)
    complement_rp ();

  /* After merging, reallocate RP so we release memory to the system.
     Also add a sentinel at the end of RP, to avoid out of bounds access
     and for performance reasons.  */
  ++n_frp;
  frp = xreallocarray (frp, n_frp, పరిమాణం (నిర్మాణం క్షేత్రం_range_pair));
  frp[n_frp - 1].lo = frp[n_frp - 1].hi = UINTMAX_MAX;
}
