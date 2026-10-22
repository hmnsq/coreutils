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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "set-fields.h"

/* Array of `struct field_range_pair' holding all the finite ranges. */
ರಚನೆ ಕ್ಷೇತ್ರ_range_pair *frp;

/* Number of finite ranges specified by the user. */
idx_t n_frp;

/* Number of `struct field_range_pair's allocated. */
ಸ್ಥಿರ idx_t n_frp_allocated;

#ವ್ಯಾಖ್ಯಾನಿಸು FATAL_ERROR(ಸಂದೇಶ)                                            \
  ಮಾಡು                                                                    \
    {                                                                   \
      error (0, 0, (ಸಂದೇಶ));                                          \
      usage (EXIT_FAILURE);                                             \
    }                                                                   \
  ಆಗ (0)

/* Append LOW, HIGH to the list RP of range pairs, allocating additional
   space if necessary.  Update global variable N_FRP.  When allocating,
   update global variable N_FRP_ALLOCATED.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_range_pair (uintmax_t lo, uintmax_t hi)
{
  ಆದರೆ (n_frp == n_frp_allocated)
    frp = xpalloc (frp, &n_frp_allocated, 1, -1, ಗಾತ್ರ *frp);
  frp[n_frp].lo = lo;
  frp[n_frp].hi = hi;
  ++n_frp;
}


/* Comparison function for qsort to order the list of
   struct field_range_pairs.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compare_ranges (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
  ರಚನೆ ಕ್ಷೇತ್ರ_range_pair ಸ್ಥಿರಾಂಕ *ap = a, *bp = b;
  ಹಿಂತಿರುಗಿ _GL_CMP (ap->lo, bp->lo);
}

/* Reallocate Range Pair entries, with corresponding
   entries outside the range of each specified entry.  */

ಸ್ಥಿರ ಶೂನ್ಯ
complement_rp (ಶೂನ್ಯ)
{
  ರಚನೆ ಕ್ಷೇತ್ರ_range_pair *c = frp;
  idx_t n = n_frp;

  frp = ಖಾಲಿ;
  n_frp = 0;
  n_frp_allocated = 0;

  ಆದರೆ (c[0].lo > 1)
    ಕೂಡಿಸು_range_pair (1, c[0].lo - 1);

  ಗಾಗಿ (idx_t i = 1; i < n; ++i)
    {
      ಆದರೆ (c[i - 1].hi + 1 == c[i].lo)
        ಮುಂದುವರಿ;

      ಕೂಡಿಸು_range_pair (c[i - 1].hi + 1, c[i].lo - 1);
    }

  ಆದರೆ (c[n - 1].hi < UINTMAX_MAX)
    ಕೂಡಿಸು_range_pair (c[n - 1].hi + 1, UINTMAX_MAX);

  ಬಿಡುಗಡೆ (c);
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
ಶೂನ್ಯ
ಗಣ_fields (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fieldstr, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ options)
{
  uintmax_t initial = 1;	/* Value of first number in a range.  */
  uintmax_t ಮೌಲ್ಯ = 0;		/* If nonzero, a number being accumulated.  */
  bool lhs_specified = false;
  bool rhs_specified = false;
  bool dash_ಸಿಕ್ಕಿತು = false;	/* True if a '-' is found in this field.  */

  bool ಒಳ_digits = false;

  /* Collect and store in RP the range end points. */

  /* Special case: '--field=-' means all fields, emulate '--field=1-' . */
  ಆದರೆ ((options & SETFLD_ALLOW_DASH) && streq (fieldstr,"-"))
    {
      ಮೌಲ್ಯ = 1;
      lhs_specified = true;
      dash_ಸಿಕ್ಕಿತು = true;
      fieldstr++;
    }

  ಆಗ (true)
    {
      ಆದರೆ (*fieldstr == '-')
        {
          ಒಳ_digits = false;
          /* Starting a range. */
          ಆದರೆ (dash_ಸಿಕ್ಕಿತು)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("invalid byte or character range")
                         : _("invalid field range"));

          dash_ಸಿಕ್ಕಿತು = true;
          fieldstr++;

          ಆದರೆ (lhs_specified && !ಮೌಲ್ಯ)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("byte/character positions are numbered from 1")
                         : _("fields are numbered from 1"));

          initial = (lhs_specified ? ಮೌಲ್ಯ : 1);
          ಮೌಲ್ಯ = 0;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*fieldstr == ','
               || isblank (ಗಮ್ಯ_uchar (*fieldstr)) || *fieldstr == '\0')
        {
          ಒಳ_digits = false;
          /* Ending the string, or this field/byte sublist. */
          ಆದರೆ (dash_ಸಿಕ್ಕಿತು)
            {
              dash_ಸಿಕ್ಕಿತು = false;

              ಆದರೆ (!lhs_specified && !rhs_specified)
                {
                  /* if a lone dash is allowed, emulate '1-' for all fields */
                  ಆದರೆ (options & SETFLD_ALLOW_DASH)
                    initial = 1;
                  ಇಲ್ಲದಿದ್ದರೆ
                    FATAL_ERROR (_("invalid range with no endpoint: -"));
                }

              /* A range.  Possibilities: -n, m-n, n-.
                 In any case, 'initial' contains the start of the range. */
              ಆದರೆ (!rhs_specified)
                {
                  /* 'n-'.  From 'initial' to end of line. */
                  ಕೂಡಿಸು_range_pair (initial, UINTMAX_MAX);
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  /* 'm-n' or '-n' (1-n). */
                  ಆದರೆ (ಮೌಲ್ಯ < initial)
                    FATAL_ERROR (_("invalid decreasing range"));

                  ಕೂಡಿಸು_range_pair (initial, ಮೌಲ್ಯ);
                }
              ಮೌಲ್ಯ = 0;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* A simple field number, not a range. */
              ಆದರೆ (ಮೌಲ್ಯ == 0)
                FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                             ? _("byte/character positions are numbered from 1")
                             : _("fields are numbered from 1"));

              ಕೂಡಿಸು_range_pair (ಮೌಲ್ಯ, ಮೌಲ್ಯ);
              ಮೌಲ್ಯ = 0;
            }

          ಆದರೆ (*fieldstr == '\0')
            ನಿಲ್ಲಿಸು;

          fieldstr++;
          lhs_specified = false;
          rhs_specified = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c_isdigit (*fieldstr))
        {
          /* Record beginning of digit string, in case we have to
             complain about it.  */
          ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಅಂಕೆ_ಆರಂಭ;
          ಆದರೆ (!ಒಳ_digits || !ಅಂಕೆ_ಆರಂಭ)
            ಅಂಕೆ_ಆರಂಭ = fieldstr;
          ಒಳ_digits = true;

          ಆದರೆ (dash_ಸಿಕ್ಕಿತು)
            rhs_specified = 1;
          ಇಲ್ಲದಿದ್ದರೆ
            lhs_specified = 1;

          /* Detect overflow.  */
          ಆದರೆ (!DECIMAL_DIGIT_ACCUMULATE (ಮೌಲ್ಯ, *fieldstr - '0')
              || ಮೌಲ್ಯ == UINTMAX_MAX)
            {
              /* In case the user specified -c$(echo 2^64|bc),22,
                 complain only about the first number.  */
              ಅಕ್ಷರ *bad_ಅಂಕೆ = ximemdup0 (ಅಂಕೆ_ಆರಂಭ,
                                         strspn (ಅಂಕೆ_ಆರಂಭ, "0123456789"));
              error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                           ?_("byte/character offset %s is too large")
                           :_("field number %s is too large"),
                           quote (bad_ಅಂಕೆ));
              ಬಿಡುಗಡೆ (bad_ಅಂಕೆ);
              usage (EXIT_FAILURE);
            }

          fieldstr++;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                       ?_("invalid byte/character position %s")
                       :_("invalid field value %s"),
                       quote (fieldstr));
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (!n_frp)
    FATAL_ERROR ((options&SETFLD_ERRMSG_USE_POS)
                 ? _("missing list of byte/character positions")
                 : _("missing list of fields"));

  qsort (frp, n_frp, ಗಾತ್ರ (frp[0]), compare_ranges);

  /* Merge range pairs (e.g. `2-5,3-4' becomes `2-5'). */
  ಗಾಗಿ (idx_t i = 0; i < n_frp; ++i)
    {
      ಗಾಗಿ (idx_t j = i + 1; j < n_frp; ++j)
        {
          ಆದರೆ (frp[j].lo <= frp[i].hi)
            {
              frp[i].hi = MAX (frp[j].hi, frp[i].hi);
              memmove (frp + j, frp + j + 1, (n_frp - j - 1) * ಗಾತ್ರ *frp);
              n_frp--;
              j--;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (options & SETFLD_COMPLEMENT)
    complement_rp ();

  /* After merging, reallocate RP so we release memory to the system.
     Also add a sentinel at the end of RP, to avoid out of bounds access
     and for performance reasons.  */
  ++n_frp;
  frp = xreallocarray (frp, n_frp, ಗಾತ್ರ (ರಚನೆ ಕ್ಷೇತ್ರ_range_pair));
  frp[n_frp - 1].lo = frp[n_frp - 1].hi = UINTMAX_MAX;
}
