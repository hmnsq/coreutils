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

#ضمن <config.h>

#ضمن "system.h"
#ضمن "c-ctype.h"
#ضمن "quote.h"
#ضمن "set-fields.h"

/* Array of `struct field_range_pair' holding all the finite ranges. */
بنية field_range_pair *frp;

/* Number of finite ranges specified by the user. */
idx_t n_frp;

/* Number of `struct field_range_pair's allocated. */
ساكن idx_t n_frp_allocated;

#define FATAL_ERROR(Message)                                            \
  עשה                                                                    \
    {                                                                   \
      error (0, 0, (Message));                                          \
      usage (EXIT_FAILURE);                                             \
    }                                                                   \
  כלעוד (0)

/* Append LOW, HIGH to the list RP of range pairs, allocating additional
   space if necessary.  Update global variable N_FRP.  When allocating,
   update global variable N_FRP_ALLOCATED.  */
ساكن فراغ
add_range_pair (uintmax_t lo, uintmax_t hi)
{
  אם (n_frp == n_frp_allocated)
    frp = xpalloc (frp, &n_frp_allocated, 1, -1, حجم *frp);
  frp[n_frp].lo = lo;
  frp[n_frp].hi = hi;
  ++n_frp;
}


/* Comparison function for qsort to order the list of
   struct field_range_pairs.  */
ساكن صحيح
compare_ranges (ثابت فراغ *a, ثابت فراغ *b)
{
  بنية field_range_pair ثابت *ap = a, *bp = b;
  החזר _GL_CMP (ap->lo, bp->lo);
}

/* Reallocate Range Pair entries, with corresponding
   entries outside the range of each specified entry.  */

ساكن فراغ
complement_rp (فراغ)
{
  بنية field_range_pair *c = frp;
  idx_t n = n_frp;

  frp = عدم;
  n_frp = 0;
  n_frp_allocated = 0;

  אם (c[0].lo > 1)
    add_range_pair (1, c[0].lo - 1);

  עבור (idx_t i = 1; i < n; ++i)
    {
      אם (c[i - 1].hi + 1 == c[i].lo)
        استمر;

      add_range_pair (c[i - 1].hi + 1, c[i].lo - 1);
    }

  אם (c[n - 1].hi < UINTMAX_MAX)
    add_range_pair (c[n - 1].hi + 1, UINTMAX_MAX);

  حرر (c);
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
فراغ
set_fields (حرف ثابت *fieldstr, unsigned صحيح options)
{
  uintmax_t initial = 1;	/* Value of first number in a range.  */
  uintmax_t value = 0;		/* If nonzero, a number being accumulated.  */
  bool lhs_specified = false;
  bool rhs_specified = false;
  bool dash_found = false;	/* True if a '-' is found in this field.  */

  bool in_digits = false;

  /* Collect and store in RP the range end points. */

  /* Special case: '--field=-' means all fields, emulate '--field=1-' . */
  אם ((options & SETFLD_ALLOW_DASH) && streq (fieldstr,"-"))
    {
      value = 1;
      lhs_specified = true;
      dash_found = true;
      fieldstr++;
    }

  כלעוד (true)
    {
      אם (*fieldstr == '-')
        {
          in_digits = false;
          /* Starting a range. */
          אם (dash_found)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("invalid byte or character range")
                         : _("invalid field range"));

          dash_found = true;
          fieldstr++;

          אם (lhs_specified && !value)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("byte/character positions are numbered from 1")
                         : _("fields are numbered from 1"));

          initial = (lhs_specified ? value : 1);
          value = 0;
        }
      אחרת אם (*fieldstr == ','
               || isblank (to_uchar (*fieldstr)) || *fieldstr == '\0')
        {
          in_digits = false;
          /* Ending the string, or this field/byte sublist. */
          אם (dash_found)
            {
              dash_found = false;

              אם (!lhs_specified && !rhs_specified)
                {
                  /* if a lone dash is allowed, emulate '1-' for all fields */
                  אם (options & SETFLD_ALLOW_DASH)
                    initial = 1;
                  אחרת
                    FATAL_ERROR (_("invalid range with no endpoint: -"));
                }

              /* A range.  Possibilities: -n, m-n, n-.
                 In any case, 'initial' contains the start of the range. */
              אם (!rhs_specified)
                {
                  /* 'n-'.  From 'initial' to end of line. */
                  add_range_pair (initial, UINTMAX_MAX);
                }
              אחרת
                {
                  /* 'm-n' or '-n' (1-n). */
                  אם (value < initial)
                    FATAL_ERROR (_("invalid decreasing range"));

                  add_range_pair (initial, value);
                }
              value = 0;
            }
          אחרת
            {
              /* A simple field number, not a range. */
              אם (value == 0)
                FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                             ? _("byte/character positions are numbered from 1")
                             : _("fields are numbered from 1"));

              add_range_pair (value, value);
              value = 0;
            }

          אם (*fieldstr == '\0')
            اكسر;

          fieldstr++;
          lhs_specified = false;
          rhs_specified = false;
        }
      אחרת אם (c_isdigit (*fieldstr))
        {
          /* Record beginning of digit string, in case we have to
             complain about it.  */
          ساكن حرف ثابت *num_start;
          אם (!in_digits || !num_start)
            num_start = fieldstr;
          in_digits = true;

          אם (dash_found)
            rhs_specified = 1;
          אחרת
            lhs_specified = 1;

          /* Detect overflow.  */
          אם (!DECIMAL_DIGIT_ACCUMULATE (value, *fieldstr - '0')
              || value == UINTMAX_MAX)
            {
              /* In case the user specified -c$(echo 2^64|bc),22,
                 complain only about the first number.  */
              حرف *bad_num = ximemdup0 (num_start,
                                         strspn (num_start, "0123456789"));
              error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                           ?_("byte/character offset %s is too large")
                           :_("field number %s is too large"),
                           quote (bad_num));
              حرر (bad_num);
              usage (EXIT_FAILURE);
            }

          fieldstr++;
        }
      אחרת
        {
          error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                       ?_("invalid byte/character position %s")
                       :_("invalid field value %s"),
                       quote (fieldstr));
          usage (EXIT_FAILURE);
        }
    }

  אם (!n_frp)
    FATAL_ERROR ((options&SETFLD_ERRMSG_USE_POS)
                 ? _("missing list of byte/character positions")
                 : _("missing list of fields"));

  qsort (frp, n_frp, حجم (frp[0]), compare_ranges);

  /* Merge range pairs (e.g. `2-5,3-4' becomes `2-5'). */
  עבור (idx_t i = 0; i < n_frp; ++i)
    {
      עבור (idx_t j = i + 1; j < n_frp; ++j)
        {
          אם (frp[j].lo <= frp[i].hi)
            {
              frp[i].hi = MAX (frp[j].hi, frp[i].hi);
              memmove (frp + j, frp + j + 1, (n_frp - j - 1) * حجم *frp);
              n_frp--;
              j--;
            }
          אחרת
            اكسر;
        }
    }

  אם (options & SETFLD_COMPLEMENT)
    complement_rp ();

  /* After merging, reallocate RP so we release memory to the system.
     Also add a sentinel at the end of RP, to avoid out of bounds access
     and for performance reasons.  */
  ++n_frp;
  frp = xreallocarray (frp, n_frp, حجم (بنية field_range_pair));
  frp[n_frp - 1].lo = frp[n_frp - 1].hi = UINTMAX_MAX;
}
