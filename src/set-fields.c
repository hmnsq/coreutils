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
ಸಂರಚನಾ field_range_pair *frp;

/* Number of finite ranges specified by the user. */
idx_t n_frp;

/* Number of `struct field_range_pair's allocated. */
ಸ್ಥಿರ idx_t n_frp_allocated;

#define FATAL_ERROR(Message)                                            \
  ಕುರು                                                                    \
    {                                                                   \
      error (0, 0, (Message));                                          \
      usage (EXIT_FAILURE);                                             \
    }                                                                   \
  ಯಾವತ್ (0)

/* Append LOW, HIGH to the list RP of range pairs, allocating additional
   space if necessary.  Update global variable N_FRP.  When allocating,
   update global variable N_FRP_ALLOCATED.  */
ಸ್ಥಿರ ಶೂನ್ಯ
add_range_pair (uintmax_t lo, uintmax_t hi)
{
  ಯದಿ (n_frp == n_frp_allocated)
    frp = xpalloc (frp, &n_frp_allocated, 1, -1, ಪರಿಮಾಣ *frp);
  frp[n_frp].lo = lo;
  frp[n_frp].hi = hi;
  ++n_frp;
}


/* Comparison function for qsort to order the list of
   struct field_range_pairs.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compare_ranges (ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
  ಸಂರಚನಾ field_range_pair ಧ್ರುವ *ap = a, *bp = b;
  ಪ್ರತ್ಯಾಗಮ _GL_CMP (ap->lo, bp->lo);
}

/* Reallocate Range Pair entries, with corresponding
   entries outside the range of each specified entry.  */

ಸ್ಥಿರ ಶೂನ್ಯ
complement_rp (ಶೂನ್ಯ)
{
  ಸಂರಚನಾ field_range_pair *c = frp;
  idx_t n = n_frp;

  frp = ರಿಕ್ತ;
  n_frp = 0;
  n_frp_allocated = 0;

  ಯದಿ (c[0].lo > 1)
    add_range_pair (1, c[0].lo - 1);

  ಪ್ರತಿ (idx_t i = 1; i < n; ++i)
    {
      ಯದಿ (c[i - 1].hi + 1 == c[i].lo)
        ಅನುವರ್ತ;

      add_range_pair (c[i - 1].hi + 1, c[i].lo - 1);
    }

  ಯದಿ (c[n - 1].hi < UINTMAX_MAX)
    add_range_pair (c[n - 1].hi + 1, UINTMAX_MAX);

  ಮುಕ್ತ (c);
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
set_fields (ವರ್ಣ ಧ್ರುವ *fieldstr, unsigned ಪೂರ್ಣಾಙ್ಕ options)
{
  uintmax_t initial = 1;	/* Value of first number in a range.  */
  uintmax_t ಮೌಲ್ಯ = 0;		/* If nonzero, a number being accumulated.  */
  bool lhs_specified = false;
  bool rhs_specified = false;
  bool dash_found = false;	/* True if a '-' is found in this field.  */

  bool in_digits = false;

  /* Collect and store in RP the range end points. */

  /* Special case: '--field=-' means all fields, emulate '--field=1-' . */
  ಯದಿ ((options & SETFLD_ALLOW_DASH) && streq (fieldstr,"-"))
    {
      ಮೌಲ್ಯ = 1;
      lhs_specified = true;
      dash_found = true;
      fieldstr++;
    }

  ಯಾವತ್ (true)
    {
      ಯದಿ (*fieldstr == '-')
        {
          in_digits = false;
          /* Starting a range. */
          ಯದಿ (dash_found)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("invalid byte or character range")
                         : _("invalid field range"));

          dash_found = true;
          fieldstr++;

          ಯದಿ (lhs_specified && !ಮೌಲ್ಯ)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("byte/character positions are numbered from 1")
                         : _("fields are numbered from 1"));

          initial = (lhs_specified ? ಮೌಲ್ಯ : 1);
          ಮೌಲ್ಯ = 0;
        }
      ಅನ್ಯಥಾ ಯದಿ (*fieldstr == ','
               || isblank (to_uchar (*fieldstr)) || *fieldstr == '\0')
        {
          in_digits = false;
          /* Ending the string, or this field/byte sublist. */
          ಯದಿ (dash_found)
            {
              dash_found = false;

              ಯದಿ (!lhs_specified && !rhs_specified)
                {
                  /* if a lone dash is allowed, emulate '1-' for all fields */
                  ಯದಿ (options & SETFLD_ALLOW_DASH)
                    initial = 1;
                  ಅನ್ಯಥಾ
                    FATAL_ERROR (_("invalid range with no endpoint: -"));
                }

              /* A range.  Possibilities: -n, m-n, n-.
                 In any case, 'initial' contains the start of the range. */
              ಯದಿ (!rhs_specified)
                {
                  /* 'n-'.  From 'initial' to end of line. */
                  add_range_pair (initial, UINTMAX_MAX);
                }
              ಅನ್ಯಥಾ
                {
                  /* 'm-n' or '-n' (1-n). */
                  ಯದಿ (ಮೌಲ್ಯ < initial)
                    FATAL_ERROR (_("invalid decreasing range"));

                  add_range_pair (initial, ಮೌಲ್ಯ);
                }
              ಮೌಲ್ಯ = 0;
            }
          ಅನ್ಯಥಾ
            {
              /* A simple field number, not a range. */
              ಯದಿ (ಮೌಲ್ಯ == 0)
                FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                             ? _("byte/character positions are numbered from 1")
                             : _("fields are numbered from 1"));

              add_range_pair (ಮೌಲ್ಯ, ಮೌಲ್ಯ);
              ಮೌಲ್ಯ = 0;
            }

          ಯದಿ (*fieldstr == '\0')
            ಭಙ್ಗ;

          fieldstr++;
          lhs_specified = false;
          rhs_specified = false;
        }
      ಅನ್ಯಥಾ ಯದಿ (c_isdigit (*fieldstr))
        {
          /* Record beginning of digit string, in case we have to
             complain about it.  */
          ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಅಂಕೆ_start;
          ಯದಿ (!in_digits || !ಅಂಕೆ_start)
            ಅಂಕೆ_start = fieldstr;
          in_digits = true;

          ಯದಿ (dash_found)
            rhs_specified = 1;
          ಅನ್ಯಥಾ
            lhs_specified = 1;

          /* Detect overflow.  */
          ಯದಿ (!DECIMAL_DIGIT_ACCUMULATE (ಮೌಲ್ಯ, *fieldstr - '0')
              || ಮೌಲ್ಯ == UINTMAX_MAX)
            {
              /* In case the user specified -c$(echo 2^64|bc),22,
                 complain only about the first number.  */
              ವರ್ಣ *bad_ಅಂಕೆ = ximemdup0 (ಅಂಕೆ_start,
                                         strspn (ಅಂಕೆ_start, "0123456789"));
              error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                           ?_("byte/character offset %s is too large")
                           :_("field number %s is too large"),
                           quote (bad_ಅಂಕೆ));
              ಮುಕ್ತ (bad_ಅಂಕೆ);
              usage (EXIT_FAILURE);
            }

          fieldstr++;
        }
      ಅನ್ಯಥಾ
        {
          error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                       ?_("invalid byte/character position %s")
                       :_("invalid field value %s"),
                       quote (fieldstr));
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (!n_frp)
    FATAL_ERROR ((options&SETFLD_ERRMSG_USE_POS)
                 ? _("missing list of byte/character positions")
                 : _("missing list of fields"));

  qsort (frp, n_frp, ಪರಿಮಾಣ (frp[0]), compare_ranges);

  /* Merge range pairs (e.g. `2-5,3-4' becomes `2-5'). */
  ಪ್ರತಿ (idx_t i = 0; i < n_frp; ++i)
    {
      ಪ್ರತಿ (idx_t j = i + 1; j < n_frp; ++j)
        {
          ಯದಿ (frp[j].lo <= frp[i].hi)
            {
              frp[i].hi = MAX (frp[j].hi, frp[i].hi);
              memmove (frp + j, frp + j + 1, (n_frp - j - 1) * ಪರಿಮಾಣ *frp);
              n_frp--;
              j--;
            }
          ಅನ್ಯಥಾ
            ಭಙ್ಗ;
        }
    }

  ಯದಿ (options & SETFLD_COMPLEMENT)
    complement_rp ();

  /* After merging, reallocate RP so we release memory to the system.
     Also add a sentinel at the end of RP, to avoid out of bounds access
     and for performance reasons.  */
  ++n_frp;
  frp = xreallocarray (frp, n_frp, ಪರಿಮಾಣ (ಸಂರಚನಾ field_range_pair));
  frp[n_frp - 1].lo = frp[n_frp - 1].hi = UINTMAX_MAX;
}
