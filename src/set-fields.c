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

#अन्तर्भाव <config.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "set-fields.h"

/* Array of `struct field_range_pair' holding all the finite ranges. */
संरचना field_range_pair *frp;

/* Number of finite ranges specified by the user. */
idx_t n_frp;

/* Number of `struct field_range_pair's allocated. */
स्थिर idx_t n_frp_allocated;

#define FATAL_ERROR(Message)                                            \
  करेहि                                                                    \
    {                                                                   \
      error (0, 0, (Message));                                          \
      usage (EXIT_FAILURE);                                             \
    }                                                                   \
  जाव (0)

/* Append LOW, HIGH to the list RP of range pairs, allocating additional
   space if necessary.  Update global variable N_FRP.  When allocating,
   update global variable N_FRP_ALLOCATED.  */
स्थिर शून्य
add_range_pair (uintmax_t lo, uintmax_t hi)
{
  जइ (n_frp == n_frp_allocated)
    frp = xpalloc (frp, &n_frp_allocated, 1, -1, परिमाण *frp);
  frp[n_frp].lo = lo;
  frp[n_frp].hi = hi;
  ++n_frp;
}


/* Comparison function for qsort to order the list of
   struct field_range_pairs.  */
स्थिर पूर्णाङ्क
compare_ranges (ध्रुव शून्य *a, ध्रुव शून्य *b)
{
  संरचना field_range_pair ध्रुव *ap = a, *bp = b;
  प्रत्यागम _GL_CMP (ap->lo, bp->lo);
}

/* Reallocate Range Pair entries, with corresponding
   entries outside the range of each specified entry.  */

स्थिर शून्य
complement_rp (शून्य)
{
  संरचना field_range_pair *c = frp;
  idx_t n = n_frp;

  frp = रिक्त;
  n_frp = 0;
  n_frp_allocated = 0;

  जइ (c[0].lo > 1)
    add_range_pair (1, c[0].lo - 1);

  प्रति (idx_t i = 1; i < n; ++i)
    {
      जइ (c[i - 1].hi + 1 == c[i].lo)
        अनुवर्त;

      add_range_pair (c[i - 1].hi + 1, c[i].lo - 1);
    }

  जइ (c[n - 1].hi < UINTMAX_MAX)
    add_range_pair (c[n - 1].hi + 1, UINTMAX_MAX);

  मुक्त (c);
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
शून्य
set_fields (वर्ण ध्रुव *fieldstr, unsigned पूर्णाङ्क options)
{
  uintmax_t initial = 1;	/* Value of first number in a range.  */
  uintmax_t मानम् = 0;		/* If nonzero, a number being accumulated.  */
  bool lhs_specified = false;
  bool rhs_specified = false;
  bool dash_found = false;	/* True if a '-' is found in this field.  */

  bool in_digits = false;

  /* Collect and store in RP the range end points. */

  /* Special case: '--field=-' means all fields, emulate '--field=1-' . */
  जइ ((options & SETFLD_ALLOW_DASH) && streq (fieldstr,"-"))
    {
      मानम् = 1;
      lhs_specified = true;
      dash_found = true;
      fieldstr++;
    }

  जाव (true)
    {
      जइ (*fieldstr == '-')
        {
          in_digits = false;
          /* Starting a range. */
          जइ (dash_found)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("invalid byte or character range")
                         : _("invalid field range"));

          dash_found = true;
          fieldstr++;

          जइ (lhs_specified && !मानम्)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("byte/character positions are numbered from 1")
                         : _("fields are numbered from 1"));

          initial = (lhs_specified ? मानम् : 1);
          मानम् = 0;
        }
      अण्णहा जइ (*fieldstr == ','
               || isblank (to_uchar (*fieldstr)) || *fieldstr == '\0')
        {
          in_digits = false;
          /* Ending the string, or this field/byte sublist. */
          जइ (dash_found)
            {
              dash_found = false;

              जइ (!lhs_specified && !rhs_specified)
                {
                  /* if a lone dash is allowed, emulate '1-' for all fields */
                  जइ (options & SETFLD_ALLOW_DASH)
                    initial = 1;
                  अण्णहा
                    FATAL_ERROR (_("invalid range with no endpoint: -"));
                }

              /* A range.  Possibilities: -n, m-n, n-.
                 In any case, 'initial' contains the start of the range. */
              जइ (!rhs_specified)
                {
                  /* 'n-'.  From 'initial' to end of line. */
                  add_range_pair (initial, UINTMAX_MAX);
                }
              अण्णहा
                {
                  /* 'm-n' or '-n' (1-n). */
                  जइ (मानम् < initial)
                    FATAL_ERROR (_("invalid decreasing range"));

                  add_range_pair (initial, मानम्);
                }
              मानम् = 0;
            }
          अण्णहा
            {
              /* A simple field number, not a range. */
              जइ (मानम् == 0)
                FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                             ? _("byte/character positions are numbered from 1")
                             : _("fields are numbered from 1"));

              add_range_pair (मानम्, मानम्);
              मानम् = 0;
            }

          जइ (*fieldstr == '\0')
            भङ्ग;

          fieldstr++;
          lhs_specified = false;
          rhs_specified = false;
        }
      अण्णहा जइ (c_isdigit (*fieldstr))
        {
          /* Record beginning of digit string, in case we have to
             complain about it.  */
          स्थिर वर्ण ध्रुव *अङ्कः_start;
          जइ (!in_digits || !अङ्कः_start)
            अङ्कः_start = fieldstr;
          in_digits = true;

          जइ (dash_found)
            rhs_specified = 1;
          अण्णहा
            lhs_specified = 1;

          /* Detect overflow.  */
          जइ (!DECIMAL_DIGIT_ACCUMULATE (मानम्, *fieldstr - '0')
              || मानम् == UINTMAX_MAX)
            {
              /* In case the user specified -c$(echo 2^64|bc),22,
                 complain only about the first number.  */
              वर्ण *bad_अङ्कः = ximemdup0 (अङ्कः_start,
                                         strspn (अङ्कः_start, "0123456789"));
              error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                           ?_("byte/character offset %s is too large")
                           :_("field number %s is too large"),
                           quote (bad_अङ्कः));
              मुक्त (bad_अङ्कः);
              usage (EXIT_FAILURE);
            }

          fieldstr++;
        }
      अण्णहा
        {
          error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                       ?_("invalid byte/character position %s")
                       :_("invalid field value %s"),
                       quote (fieldstr));
          usage (EXIT_FAILURE);
        }
    }

  जइ (!n_frp)
    FATAL_ERROR ((options&SETFLD_ERRMSG_USE_POS)
                 ? _("missing list of byte/character positions")
                 : _("missing list of fields"));

  qsort (frp, n_frp, परिमाण (frp[0]), compare_ranges);

  /* Merge range pairs (e.g. `2-5,3-4' becomes `2-5'). */
  प्रति (idx_t i = 0; i < n_frp; ++i)
    {
      प्रति (idx_t j = i + 1; j < n_frp; ++j)
        {
          जइ (frp[j].lo <= frp[i].hi)
            {
              frp[i].hi = MAX (frp[j].hi, frp[i].hi);
              memmove (frp + j, frp + j + 1, (n_frp - j - 1) * परिमाण *frp);
              n_frp--;
              j--;
            }
          अण्णहा
            भङ्ग;
        }
    }

  जइ (options & SETFLD_COMPLEMENT)
    complement_rp ();

  /* After merging, reallocate RP so we release memory to the system.
     Also add a sentinel at the end of RP, to avoid out of bounds access
     and for performance reasons.  */
  ++n_frp;
  frp = xreallocarray (frp, n_frp, परिमाण (संरचना field_range_pair));
  frp[n_frp - 1].lo = frp[n_frp - 1].hi = UINTMAX_MAX;
}
