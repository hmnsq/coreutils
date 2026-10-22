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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "set-fields.h"

/* Array of `struct field_range_pair' holding all the finite ranges. */
ਬਣਤਰ ਖੇਤਰ_range_pair *frp;

/* Number of finite ranges specified by the user. */
idx_t n_frp;

/* Number of `struct field_range_pair's allocated. */
ਸਥਿਰ idx_t n_frp_allocated;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FATAL_ERROR(ਸੁਨੇਹਾ)                                            \
  ਕਰੋ                                                                    \
    {                                                                   \
      error (0, 0, (ਸੁਨੇਹਾ));                                          \
      usage (EXIT_FAILURE);                                             \
    }                                                                   \
  ਜਦੋਂ (0)

/* Append LOW, HIGH to the list RP of range pairs, allocating additional
   space if necessary.  Update global variable N_FRP.  When allocating,
   update global variable N_FRP_ALLOCATED.  */
ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_range_pair (uintmax_t lo, uintmax_t hi)
{
  ਜੇ (n_frp == n_frp_allocated)
    frp = xpalloc (frp, &n_frp_allocated, 1, -1, ਆਕਾਰ *frp);
  frp[n_frp].lo = lo;
  frp[n_frp].hi = hi;
  ++n_frp;
}


/* Comparison function for qsort to order the list of
   struct field_range_pairs.  */
ਸਥਿਰ ਪੂਰਨ
compare_ranges (ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
  ਬਣਤਰ ਖੇਤਰ_range_pair ਅਚਲ *ap = a, *bp = b;
  ਵਾਪਸ _GL_CMP (ap->lo, bp->lo);
}

/* Reallocate Range Pair entries, with corresponding
   entries outside the range of each specified entry.  */

ਸਥਿਰ ਖਾਲੀ
complement_rp (ਖਾਲੀ)
{
  ਬਣਤਰ ਖੇਤਰ_range_pair *c = frp;
  idx_t n = n_frp;

  frp = ਨਿਰਾ;
  n_frp = 0;
  n_frp_allocated = 0;

  ਜੇ (c[0].lo > 1)
    ਜੋੜੋ_range_pair (1, c[0].lo - 1);

  ਲਈ (idx_t i = 1; i < n; ++i)
    {
      ਜੇ (c[i - 1].hi + 1 == c[i].lo)
        ਜਾਰੀ;

      ਜੋੜੋ_range_pair (c[i - 1].hi + 1, c[i].lo - 1);
    }

  ਜੇ (c[n - 1].hi < UINTMAX_MAX)
    ਜੋੜੋ_range_pair (c[n - 1].hi + 1, UINTMAX_MAX);

  ਮੁਕਤ (c);
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
ਖਾਲੀ
ਸੈੱਟ_fields (ਅੱਖਰ ਅਚਲ *fieldstr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ options)
{
  uintmax_t initial = 1;	/* Value of first number in a range.  */
  uintmax_t ਮੁੱਲ = 0;		/* If nonzero, a number being accumulated.  */
  bool lhs_specified = false;
  bool rhs_specified = false;
  bool dash_ਮਿਲਿਆ = false;	/* True if a '-' is found in this field.  */

  bool ਵਿੱਚ_digits = false;

  /* Collect and store in RP the range end points. */

  /* Special case: '--field=-' means all fields, emulate '--field=1-' . */
  ਜੇ ((options & SETFLD_ALLOW_DASH) && streq (fieldstr,"-"))
    {
      ਮੁੱਲ = 1;
      lhs_specified = true;
      dash_ਮਿਲਿਆ = true;
      fieldstr++;
    }

  ਜਦੋਂ (true)
    {
      ਜੇ (*fieldstr == '-')
        {
          ਵਿੱਚ_digits = false;
          /* Starting a range. */
          ਜੇ (dash_ਮਿਲਿਆ)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("invalid byte or character range")
                         : _("invalid field range"));

          dash_ਮਿਲਿਆ = true;
          fieldstr++;

          ਜੇ (lhs_specified && !ਮੁੱਲ)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("byte/character positions are numbered from 1")
                         : _("fields are numbered from 1"));

          initial = (lhs_specified ? ਮੁੱਲ : 1);
          ਮੁੱਲ = 0;
        }
      ਨਹੀਂਤਾਂ ਜੇ (*fieldstr == ','
               || isblank (ਮੰਜ਼ਿਲ_uchar (*fieldstr)) || *fieldstr == '\0')
        {
          ਵਿੱਚ_digits = false;
          /* Ending the string, or this field/byte sublist. */
          ਜੇ (dash_ਮਿਲਿਆ)
            {
              dash_ਮਿਲਿਆ = false;

              ਜੇ (!lhs_specified && !rhs_specified)
                {
                  /* if a lone dash is allowed, emulate '1-' for all fields */
                  ਜੇ (options & SETFLD_ALLOW_DASH)
                    initial = 1;
                  ਨਹੀਂਤਾਂ
                    FATAL_ERROR (_("invalid range with no endpoint: -"));
                }

              /* A range.  Possibilities: -n, m-n, n-.
                 In any case, 'initial' contains the start of the range. */
              ਜੇ (!rhs_specified)
                {
                  /* 'n-'.  From 'initial' to end of line. */
                  ਜੋੜੋ_range_pair (initial, UINTMAX_MAX);
                }
              ਨਹੀਂਤਾਂ
                {
                  /* 'm-n' or '-n' (1-n). */
                  ਜੇ (ਮੁੱਲ < initial)
                    FATAL_ERROR (_("invalid decreasing range"));

                  ਜੋੜੋ_range_pair (initial, ਮੁੱਲ);
                }
              ਮੁੱਲ = 0;
            }
          ਨਹੀਂਤਾਂ
            {
              /* A simple field number, not a range. */
              ਜੇ (ਮੁੱਲ == 0)
                FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                             ? _("byte/character positions are numbered from 1")
                             : _("fields are numbered from 1"));

              ਜੋੜੋ_range_pair (ਮੁੱਲ, ਮੁੱਲ);
              ਮੁੱਲ = 0;
            }

          ਜੇ (*fieldstr == '\0')
            ਰੋਕੋ;

          fieldstr++;
          lhs_specified = false;
          rhs_specified = false;
        }
      ਨਹੀਂਤਾਂ ਜੇ (c_isdigit (*fieldstr))
        {
          /* Record beginning of digit string, in case we have to
             complain about it.  */
          ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅੰਕ_ਸ਼ੁਰੂ;
          ਜੇ (!ਵਿੱਚ_digits || !ਅੰਕ_ਸ਼ੁਰੂ)
            ਅੰਕ_ਸ਼ੁਰੂ = fieldstr;
          ਵਿੱਚ_digits = true;

          ਜੇ (dash_ਮਿਲਿਆ)
            rhs_specified = 1;
          ਨਹੀਂਤਾਂ
            lhs_specified = 1;

          /* Detect overflow.  */
          ਜੇ (!DECIMAL_DIGIT_ACCUMULATE (ਮੁੱਲ, *fieldstr - '0')
              || ਮੁੱਲ == UINTMAX_MAX)
            {
              /* In case the user specified -c$(echo 2^64|bc),22,
                 complain only about the first number.  */
              ਅੱਖਰ *bad_ਅੰਕ = ximemdup0 (ਅੰਕ_ਸ਼ੁਰੂ,
                                         strspn (ਅੰਕ_ਸ਼ੁਰੂ, "0123456789"));
              error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                           ?_("byte/character offset %s is too large")
                           :_("field number %s is too large"),
                           quote (bad_ਅੰਕ));
              ਮੁਕਤ (bad_ਅੰਕ);
              usage (EXIT_FAILURE);
            }

          fieldstr++;
        }
      ਨਹੀਂਤਾਂ
        {
          error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                       ?_("invalid byte/character position %s")
                       :_("invalid field value %s"),
                       quote (fieldstr));
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (!n_frp)
    FATAL_ERROR ((options&SETFLD_ERRMSG_USE_POS)
                 ? _("missing list of byte/character positions")
                 : _("missing list of fields"));

  qsort (frp, n_frp, ਆਕਾਰ (frp[0]), compare_ranges);

  /* Merge range pairs (e.g. `2-5,3-4' becomes `2-5'). */
  ਲਈ (idx_t i = 0; i < n_frp; ++i)
    {
      ਲਈ (idx_t j = i + 1; j < n_frp; ++j)
        {
          ਜੇ (frp[j].lo <= frp[i].hi)
            {
              frp[i].hi = MAX (frp[j].hi, frp[i].hi);
              memmove (frp + j, frp + j + 1, (n_frp - j - 1) * ਆਕਾਰ *frp);
              n_frp--;
              j--;
            }
          ਨਹੀਂਤਾਂ
            ਰੋਕੋ;
        }
    }

  ਜੇ (options & SETFLD_COMPLEMENT)
    complement_rp ();

  /* After merging, reallocate RP so we release memory to the system.
     Also add a sentinel at the end of RP, to avoid out of bounds access
     and for performance reasons.  */
  ++n_frp;
  frp = xreallocarray (frp, n_frp, ਆਕਾਰ (ਬਣਤਰ ਖੇਤਰ_range_pair));
  frp[n_frp - 1].lo = frp[n_frp - 1].hi = UINTMAX_MAX;
}
