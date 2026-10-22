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
  करा                                                                    \
    {                                                                   \
      error (0, 0, (Message));                                          \
      usage (निकास_त्रुटि);                                             \
    }                                                                   \
  पर्यंत (0)

/* Append LOW, HIGH to the list RP of range pairs, allocating additional
   space if necessary.  Update global variable N_FRP.  When allocating,
   update global variable N_FRP_ALLOCATED.  */
स्थिर शून्य
add_range_pair (uintmax_t lo, uintmax_t hi)
{
  जर (n_frp == n_frp_allocated)
    frp = xpalloc (frp, &n_frp_allocated, 1, -1, आकार *frp);
  frp[n_frp].lo = lo;
  frp[n_frp].hi = hi;
  ++n_frp;
}


/* Comparison function for qsort to order the list of
   struct field_range_pairs.  */
स्थिर पूर्णांक
compare_ranges (स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
  संरचना field_range_pair स्थिरांक *ap = a, *bp = b;
  परत _GL_CMP (ap->lo, bp->lo);
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

  जर (c[0].lo > 1)
    add_range_pair (1, c[0].lo - 1);

  साठी (idx_t i = 1; i < n; ++i)
    {
      जर (c[i - 1].hi + 1 == c[i].lo)
        सुरूठेवा;

      add_range_pair (c[i - 1].hi + 1, c[i].lo - 1);
    }

  जर (c[n - 1].hi < UINTMAX_MAX)
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
set_fields (अक्षर स्थिरांक *fieldstr, अचिन्हित पूर्णांक options)
{
  uintmax_t initial = 1;	/* Value of first number in a range.  */
  uintmax_t मूल्य = 0;		/* If nonzero, a number being accumulated.  */
  bool lhs_specified = false;
  bool rhs_specified = false;
  bool dash_found = false;	/* True if a '-' is found in this field.  */

  bool in_digits = false;

  /* Collect and store in RP the range end points. */

  /* Special case: '--field=-' means all fields, emulate '--field=1-' . */
  जर ((options & SETFLD_ALLOW_DASH) && streq (fieldstr,"-"))
    {
      मूल्य = 1;
      lhs_specified = true;
      dash_found = true;
      fieldstr++;
    }

  पर्यंत (true)
    {
      जर (*fieldstr == '-')
        {
          in_digits = false;
          /* Starting a range. */
          जर (dash_found)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("invalid byte or character range")
                         : _("invalid field range"));

          dash_found = true;
          fieldstr++;

          जर (lhs_specified && !मूल्य)
            FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                         ? _("byte/character positions are numbered from 1")
                         : _("fields are numbered from 1"));

          initial = (lhs_specified ? मूल्य : 1);
          मूल्य = 0;
        }
      अन्यथा जर (*fieldstr == ','
               || isblank (to_uchar (*fieldstr)) || *fieldstr == '\0')
        {
          in_digits = false;
          /* Ending the string, or this field/byte sublist. */
          जर (dash_found)
            {
              dash_found = false;

              जर (!lhs_specified && !rhs_specified)
                {
                  /* if a lone dash is allowed, emulate '1-' for all fields */
                  जर (options & SETFLD_ALLOW_DASH)
                    initial = 1;
                  अन्यथा
                    FATAL_ERROR (_("invalid range with no endpoint: -"));
                }

              /* A range.  Possibilities: -n, m-n, n-.
                 In any case, 'initial' contains the start of the range. */
              जर (!rhs_specified)
                {
                  /* 'n-'.  From 'initial' to end of line. */
                  add_range_pair (initial, UINTMAX_MAX);
                }
              अन्यथा
                {
                  /* 'm-n' or '-n' (1-n). */
                  जर (मूल्य < initial)
                    FATAL_ERROR (_("invalid decreasing range"));

                  add_range_pair (initial, मूल्य);
                }
              मूल्य = 0;
            }
          अन्यथा
            {
              /* A simple field number, not a range. */
              जर (मूल्य == 0)
                FATAL_ERROR ((options & SETFLD_ERRMSG_USE_POS)
                             ? _("byte/character positions are numbered from 1")
                             : _("fields are numbered from 1"));

              add_range_pair (मूल्य, मूल्य);
              मूल्य = 0;
            }

          जर (*fieldstr == '\0')
            थांबा;

          fieldstr++;
          lhs_specified = false;
          rhs_specified = false;
        }
      अन्यथा जर (c_isdigit (*fieldstr))
        {
          /* Record beginning of digit string, in case we have to
             complain about it.  */
          स्थिर अक्षर स्थिरांक *अंक_start;
          जर (!in_digits || !अंक_start)
            अंक_start = fieldstr;
          in_digits = true;

          जर (dash_found)
            rhs_specified = 1;
          अन्यथा
            lhs_specified = 1;

          /* Detect overflow.  */
          जर (!DECIMAL_DIGIT_ACCUMULATE (मूल्य, *fieldstr - '0')
              || मूल्य == UINTMAX_MAX)
            {
              /* In case the user specified -c$(echo 2^64|bc),22,
                 complain only about the first number.  */
              अक्षर *bad_अंक = ximemdup0 (अंक_start,
                                         म_अखोज (अंक_start, "0123456789"));
              error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                           ?_("byte/character offset %s is too large")
                           :_("field number %s is too large"),
                           quote (bad_अंक));
              मुक्त (bad_अंक);
              usage (निकास_त्रुटि);
            }

          fieldstr++;
        }
      अन्यथा
        {
          error (0, 0, (options & SETFLD_ERRMSG_USE_POS)
                       ?_("invalid byte/character position %s")
                       :_("invalid field value %s"),
                       quote (fieldstr));
          usage (निकास_त्रुटि);
        }
    }

  जर (!n_frp)
    FATAL_ERROR ((options&SETFLD_ERRMSG_USE_POS)
                 ? _("missing list of byte/character positions")
                 : _("missing list of fields"));

  क्विक (frp, n_frp, आकार (frp[0]), compare_ranges);

  /* Merge range pairs (e.g. `2-5,3-4' becomes `2-5'). */
  साठी (idx_t i = 0; i < n_frp; ++i)
    {
      साठी (idx_t j = i + 1; j < n_frp; ++j)
        {
          जर (frp[j].lo <= frp[i].hi)
            {
              frp[i].hi = MAX (frp[j].hi, frp[i].hi);
              स_हटाओ (frp + j, frp + j + 1, (n_frp - j - 1) * आकार *frp);
              n_frp--;
              j--;
            }
          अन्यथा
            थांबा;
        }
    }

  जर (options & SETFLD_COMPLEMENT)
    complement_rp ();

  /* After merging, reallocate RP so we release memory to the system.
     Also add a sentinel at the end of RP, to avoid out of bounds access
     and for performance reasons.  */
  ++n_frp;
  frp = xreallocarray (frp, n_frp, आकार (संरचना field_range_pair));
  frp[n_frp - 1].lo = frp[n_frp - 1].hi = UINTMAX_MAX;
}
