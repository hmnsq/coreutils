/* Reformat numbers like 11505426432 to the more human-readable 11G
   Copyright (C) 2012-2026 Free Software Foundation, Inc.

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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <float.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <langinfo.h>

#அந்தர்பாவ "argmatch.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "mbswidth.h"
#அந்தர்பாவ "mcel.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "skipchars.h"
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "utf8.h"
#அந்தர்பாவ "xstrtol.h"

#அந்தர்பாவ "set-fields.h"

#எனில் HAVE_FPSETPREC
# include <ieeefp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "numfmt"

#define AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
எண்ணிக்கை { EXIT_CONVERSION_WARNINGS = 2 };

எண்ணிக்கை
{
  FROM_OPTION = CHAR_MAX + 1,
  FROM_UNIT_OPTION,
  TO_OPTION,
  TO_UNIT_OPTION,
  ROUND_OPTION,
  SUFFIX_OPTION,
  GROUPING_OPTION,
  PADDING_OPTION,
  FIELD_OPTION,
  DEBUG_OPTION,
  DEV_DEBUG_OPTION,
  HEADER_OPTION,
  FORMAT_OPTION,
  INVALID_OPTION,
  UNIT_SEPARATOR_OPTION
};

எண்ணிக்கை scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

நிலையான எழுத்து மாறிலி *மாறிலி scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", வெற்று
};

நிலையான எண்ணிக்கை scale_type மாறிலி scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

நிலையான எழுத்து மாறிலி *மாறிலி scale_to_args[] =
{
  "none", "si", "iec", "iec-i", வெற்று
};

நிலையான எண்ணிக்கை scale_type மாறிலி scale_to_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


எண்ணிக்கை round_type
{
  round_ceiling,
  round_floor,
  round_from_zero,
  round_to_zero,
  round_nearest,
};

நிலையான எழுத்து மாறிலி *மாறிலி round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", வெற்று
};

நிலையான எண்ணிக்கை round_type மாறிலி round_types[] =
{
  round_ceiling, round_floor, round_from_zero, round_to_zero, round_nearest
};


எண்ணிக்கை inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

நிலையான எழுத்து மாறிலி *மாறிலி inval_args[] =
{
  "abort", "fail", "warn", "ignore", வெற்று
};

நிலையான எண்ணிக்கை inval_type மாறிலி inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"from", required_argument, வெற்று, FROM_OPTION},
  {"from-unit", required_argument, வெற்று, FROM_UNIT_OPTION},
  {"to", required_argument, வெற்று, TO_OPTION},
  {"to-unit", required_argument, வெற்று, TO_UNIT_OPTION},
  {"round", required_argument, வெற்று, ROUND_OPTION},
  {"padding", required_argument, வெற்று, PADDING_OPTION},
  {"suffix", required_argument, வெற்று, SUFFIX_OPTION},
  {"unit-separator", required_argument, வெற்று, UNIT_SEPARATOR_OPTION},
  {"grouping", no_argument, வெற்று, GROUPING_OPTION},
  {"delimiter", required_argument, வெற்று, 'd'},
  {"field", required_argument, வெற்று, FIELD_OPTION},
  {"debug", no_argument, வெற்று, DEBUG_OPTION},
  {"-debug", no_argument, வெற்று, DEV_DEBUG_OPTION},
  {"header", optional_argument, வெற்று, HEADER_OPTION},
  {"format", required_argument, வெற்று, FORMAT_OPTION},
  {"invalid", required_argument, வெற்று, INVALID_OPTION},
  {"zero-terminated", no_argument, வெற்று, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
எண்ணிக்கை { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
எண்ணிக்கை { MAX_ACCEPTABLE_DIGITS = 33 };

நிலையான எண்ணிக்கை scale_type scale_from = scale_none;
நிலையான எண்ணிக்கை scale_type scale_to = scale_none;
நிலையான எண்ணிக்கை round_type round_style = round_from_zero;
நிலையான எண்ணிக்கை inval_type inval_style = inval_abort;
நிலையான எழுத்து மாறிலி *suffix = வெற்று;
நிலையான எழுத்து மாறிலி *unit_separator = வெற்று;
நிலையான uintmax_t from_unit_size = 1;
நிலையான uintmax_t to_unit_size = 1;
நிலையான முழு grouping = 0;
நிலையான எழுத்து *padding_buffer = வெற்று;
நிலையான idx_t padding_buffer_size = 0;
நிலையான intmax_t padding_width = 0;
நிலையான முழு zero_padding_width = 0;
நிலையான long முழு user_precision = -1;
நிலையான எழுத்து மாறிலி *format_str = வெற்று;
நிலையான எழுத்து *format_str_prefix = வெற்று;
நிலையான எழுத்து *format_str_suffix = வெற்று;

/* By default, any conversion error will terminate the program.  */
நிலையான முழு conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
நிலையான முழு auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
நிலையான எழுத்து மாறிலி *delimiter = வெற்று;

/* line delimiter.  */
நிலையான unsigned எழுத்து வரி_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
நிலையான uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
நிலையான bool debug;

/* will be set according to the current locale.  */
நிலையான எழுத்து மாறிலி *decimal_point;
நிலையான முழு decimal_point_length;
நிலையான எழுத்து மாறிலி *thousands_sep;
நிலையான முழு thousands_sep_length;

/* debugging for developers.  Enables devmsg().  */
நிலையான bool dev_debug = false;

நிலையான bool
newline_or_blank (mcel_t g)
{
  திரும்பு g.ch == '\n' || c32issep (g.ch);
}

நிலையான உள்வரி முழு
default_scale_base (எண்ணிக்கை scale_type scale)
{
  தேர்வு (scale)
    {
    நிலை scale_IEC:
    நிலை scale_IEC_I:
      திரும்பு 1024;

    நிலை scale_none:
    நிலை scale_auto:
    நிலை scale_SI:
    இயல்பு:
      திரும்பு 1000;
    }
}

நிலையான எழுத்து மாறிலி zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
நிலையான எழுத்து மாறிலி *valid_suffixes = 1 + zero_and_valid_suffixes;

நிலையான உள்வரி bool
valid_suffix (மாறிலி எழுத்து suf)
{
  திரும்பு strchr (valid_suffixes, suf) != வெற்று;
}

நிலையான உள்வரி முழு
suffix_power (மாறிலி எழுத்து suf)
{
  தேர்வு (suf)
    {
    நிலை 'k':                  /* kilo.  */
    நிலை 'K':                  /* kilo or kibi.  */
      திரும்பு 1;

    நிலை 'M':                  /* mega or mebi.  */
      திரும்பு 2;

    நிலை 'G':                  /* giga or gibi.  */
      திரும்பு 3;

    நிலை 'T':                  /* tera or tebi.  */
      திரும்பு 4;

    நிலை 'P':                  /* peta or pebi.  */
      திரும்பு 5;

    நிலை 'E':                  /* exa or exbi.  */
      திரும்பு 6;

    நிலை 'Z':                  /* zetta or 2**70.  */
      திரும்பு 7;

    நிலை 'Y':                  /* yotta or 2**80.  */
      திரும்பு 8;

    நிலை 'R':                  /* ronna or 2**90.  */
      திரும்பு 9;

    நிலை 'Q':                  /* quetta or 2**100.  */
      திரும்பு 10;

    இயல்பு:                   /* should never happen. assert?  */
      திரும்பு 0;
    }
}

நிலையான உள்வரி எழுத்து மாறிலி *
suffix_power_char (முழு power)
{
  தேர்வு (power)
    {
    நிலை 0:
      திரும்பு "";

    நிலை 1:
      திரும்பு "K";

    நிலை 2:
      திரும்பு "M";

    நிலை 3:
      திரும்பு "G";

    நிலை 4:
      திரும்பு "T";

    நிலை 5:
      திரும்பு "P";

    நிலை 6:
      திரும்பு "E";

    நிலை 7:
      திரும்பு "Z";

    நிலை 8:
      திரும்பு "Y";

    நிலை 9:
      திரும்பு "R";

    நிலை 10:
      திரும்பு "Q";

    இயல்பு:
      திரும்பு "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
நிலையான long இரட்டை
powerld (long இரட்டை base, ptrdiff_t x)
{
  long இரட்டை result = base;
  எனில் (x == 0)
    திரும்பு 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  போது (--x)
    result *= base;
  திரும்பு result;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
நிலையான உள்வரி long இரட்டை
absld (long இரட்டை val)
{
  திரும்பு val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
நிலையான long இரட்டை
expld (long இரட்டை val, முழு base, முழு /*output */ *x)
{
  முழு power = 0;

  எனில் (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      போது (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  எனில் (x)
    *x = power;
  திரும்பு val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
நிலையான உள்வரி intmax_t
simple_round_ceiling (long இரட்டை val)
{
  intmax_t intval = val;
  எனில் (intval < val)
    intval++;
  திரும்பு intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
நிலையான உள்வரி intmax_t
simple_round_floor (long இரட்டை val)
{
  திரும்பு -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
நிலையான உள்வரி intmax_t
simple_round_from_zero (long இரட்டை val)
{
  திரும்பு val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
நிலையான உள்வரி intmax_t
simple_round_to_zero (long இரட்டை val)
{
  திரும்பு val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
நிலையான உள்வரி intmax_t
simple_round_nearest (long இரட்டை val)
{
  திரும்பு val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
நிலையான உள்வரி long இரட்டை
simple_round (long இரட்டை val, எண்ணிக்கை round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (long இரட்டை) INTMAX_MAX * intmax_mul;

  தேர்வு (t)
    {
    நிலை round_ceiling:
      rval = simple_round_ceiling (val);
      நிறுத்து;

    நிலை round_floor:
      rval = simple_round_floor (val);
      நிறுத்து;

    நிலை round_from_zero:
      rval = simple_round_from_zero (val);
      நிறுத்து;

    நிலை round_to_zero:
      rval = simple_round_to_zero (val);
      நிறுத்து;

    நிலை round_nearest:
      rval = simple_round_nearest (val);
      நிறுத்து;

    இயல்பு:
      /* to silence the compiler - this should never happen.  */
      திரும்பு 0;
    }

  திரும்பு (long இரட்டை) INTMAX_MAX * intmax_mul + rval;
}

எண்ணிக்கை simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_OVERFLOW,
  SSE_INVALID_எண்,

  /* the following are returned by 'simple_strtod_human'.  */
  SSE_VALID_BUT_FORBIDDEN_SUFFIX,
  SSE_INVALID_SUFFIX,
  SSE_MISSING_I_SUFFIX
};

/* Read an *integer* INPUT_STR,
   but return the integer value in a 'long double' VALUE
   hence, no UINTMAX_MAX limitation.
   NEGATIVE is updated, and is stored separately from the VALUE
   so that signbit() isn't required to determine the sign of -0..
   ENDPTR is required (unlike strtod) and is used to store a pointer
   to the character after the last character used in the conversion.

   Note locale'd grouping is not supported,
   nor is skipping of white-space supported.

   Returns:
      SSE_OK - valid number.
      SSE_OK_PRECISION_LOSS - if more than 18 digits were used.
      SSE_OVERFLOW          - if more than 33 digits (999Q) were used.
      SSE_INVALID_NUMBER    - if no digits were found.  */
நிலையான எண்ணிக்கை simple_strtod_error
simple_strtod_int (எழுத்து மாறிலி *உள்ளீடு_str,
                   எழுத்து **endptr, long இரட்டை *மதிப்பு, bool *negative)
{
  எண்ணிக்கை simple_strtod_error e = SSE_OK;

  long இரட்டை val = 0;
  முழு digits = 0;
  bool found_digit = false;

  எனில் (*உள்ளீடு_str == '-')
    {
      உள்ளீடு_str++;
      *negative = true;
    }
  இல்லையெனில்
    *negative = false;

  *endptr = (எழுத்து *) உள்ளீடு_str;
  போது (c_isdigit (**endptr))
    {
      முழு digit = (**endptr) - '0';

      found_digit = true;

      எனில் (val || digit)
        digits++;

      எனில் (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      எனில் (digits > MAX_ACCEPTABLE_DIGITS)
        திரும்பு SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      எனில் (thousands_sep_length > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_length)
          && c_isdigit ((*endptr)[thousands_sep_length]))
        (*endptr) += thousands_sep_length;
    }
  எனில் (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    திரும்பு SSE_INVALID_எண்;
  எனில் (*negative)
    val = -val;

  எனில் (மதிப்பு)
    *மதிப்பு = val;

  திரும்பு e;
}

/* Read a floating-point INPUT_STR represented as "NNNN[.NNNNN]",
   and return the value in a 'long double' VALUE.
   ENDPTR is required (unlike strtod) and is used to store a pointer
   to the character after the last character used in the conversion.
   PRECISION is optional and used to indicate fractions are present.

   Note locale'd grouping is not supported,
   nor is skipping of white-space supported.

   Returns:
      SSE_OK - valid number.
      SSE_OK_PRECISION_LOSS - if more than 18 digits were used.
      SSE_OVERFLOW          - if more than 33 digits (999Q) were used.
      SSE_INVALID_NUMBER    - if no digits were found.  */
நிலையான எண்ணிக்கை simple_strtod_error
simple_strtod_float (எழுத்து மாறிலி *உள்ளீடு_str,
                     எழுத்து **endptr,
                     long இரட்டை *மதிப்பு,
                     size_t *precision)
{
  bool negative;
  எண்ணிக்கை simple_strtod_error e = SSE_OK;

  எனில் (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (உள்ளீடு_str, endptr, மதிப்பு, &negative);
  எனில் (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    திரும்பு e;

  /* optional decimal point + fraction.  */
  எனில் (STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    {
      எழுத்து *ptr2;
      long இரட்டை val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_length;
      எண்ணிக்கை simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      எனில் (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        திரும்பு e2;
      எனில் (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      எனில் (neg_frac)
        திரும்பு SSE_INVALID_எண்;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      எனில் (மதிப்பு)
        {
          எனில் (negative)
            *மதிப்பு -= val_frac;
          இல்லையெனில்
            *மதிப்பு += val_frac;
        }

      எனில் (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  திரும்பு e;
}

/* Read a 'human' INPUT_STR represented as "NNNN[.NNNNN] + suffix",
   and return the value in a 'long double' VALUE,
   with the precision of the input returned in PRECISION.
   ENDPTR is required (unlike strtod) and is used to store a pointer
   to the character after the last character used in the conversion.
   ALLOWED_SCALING determines the scaling supported.

   TODO:
     support locale'd grouping
     accept scientific and hex floats (probably use strtold directly)

   Returns:
      SSE_OK - valid number.
      SSE_OK_PRECISION_LOSS - if more than LDBL_DIG digits were used.
      SSE_OVERFLOW          - if more than 33 digits (999Q) were used.
      SSE_INVALID_NUMBER    - if no digits were found.
      SSE_VALID_BUT_FORBIDDEN_SUFFIX
      SSE_INVALID_SUFFIX
      SSE_MISSING_I_SUFFIX  */
நிலையான எண்ணிக்கை simple_strtod_error
simple_strtod_human (எழுத்து மாறிலி *உள்ளீடு_str,
                     எழுத்து **endptr, long இரட்டை *மதிப்பு, size_t *precision,
                     எண்ணிக்கை scale_type allowed_scaling)
{
  முழு power = 0;
  /* 'scale_auto' is checked below.  */
  முழு scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, உள்ளீடு_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  எண்ணிக்கை simple_strtod_error e =
    simple_strtod_float (உள்ளீடு_str, endptr, மதிப்பு, precision);
  எனில் (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    திரும்பு e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *மதிப்பு, (முழு)*precision);

  போது (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      எனில் (unit_separator)
        {
          size_t sep_len = strlen (unit_separator);
          எனில் (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      எனில் (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          எனில் (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      எனில் (**endptr == '\0')
        நிறுத்து;  /* Treat as no suffix.  */

      எனில் (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          எனில் (**endptr == '\0')
            நிறுத்து;

          திரும்பு SSE_INVALID_SUFFIX;
        }

      எனில் (allowed_scaling == scale_none)
        திரும்பு SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      எனில் (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      இல்லையெனில் எனில் (allowed_scaling == scale_IEC_I)
        {
          எனில் (**endptr == 'i')
            (*endptr)++;
          இல்லையெனில்
            திரும்பு SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      நிறுத்து;
    }

  long இரட்டை multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*மதிப்பு) = (*மதிப்பு) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *மதிப்பு, *மதிப்பு);

  திரும்பு e;
}


நிலையான வெறுமை
simple_strtod_fatal (எண்ணிக்கை simple_strtod_error err, எழுத்து மாறிலி *உள்ளீடு_str)
{
  எழுத்து மாறிலி *msgid = வெற்று;

  தேர்வு (err)
    {
    நிலை SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      நிறுத்து;

    நிலை SSE_INVALID_எண்:
      msgid = N_("invalid number: %s");
      நிறுத்து;

    நிலை SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      நிறுத்து;

    நிலை SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      நிறுத்து;

    நிலை SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      நிறுத்து;

    நிலை SSE_OK_PRECISION_LOSS:
    நிலை SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  எனில் (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (உள்ளீடு_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
நிலையான முழு
double_to_human (long இரட்டை val, முழு precision,
                 எழுத்து *buf, idx_t buf_size,
                 எண்ணிக்கை scale_type scale, முழு group, எண்ணிக்கை round_type round)
{
  எழுத்து fmt[அளவு "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  எழுத்து *pfmt = fmt;
  *pfmt++ = '%';

  எனில் (group)
    *pfmt++ = '\'';

  எனில் (zero_padding_width)
    pfmt += sprintf (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  எனில் (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      திரும்பு snprintf (buf, buf_size, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  இரட்டை scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  முழு power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  முழு power_adjust = 0;
  எனில் (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  இல்லையெனில் எனில் (absld (val) < 10)
    {
      /* for values less than 10, we allow one decimal-point digit,
         so adjust before rounding. */
      power_adjust = 1;
    }

  val *= powerld (10, power_adjust);
  val = simple_round (val, round);
  val /= powerld (10, power_adjust);

  /* two special cases after rounding:
     1. a "999.99" can turn into 1000 - so scale down
     2. a "9.99" can turn into 10 - so don't display decimal-point.  */
  எனில் (absld (val) >= scale_base)
    {
      val /= scale_base;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  முழு show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  முழு prec = user_precision == -1 ? show_decimal_point : user_precision;

  திரும்பு snprintf (buf, buf_size, fmt, prec, val,
                   (power > 0 && unit_separator) ? unit_separator : "",
                   power == 1 && scale == scale_SI
                   ? "k" : suffix_power_char (power),
                   &"i"[! (scale == scale_IEC_I && 0 < power)],
                   suffix ? suffix : "");
}

/* Convert a string of decimal digits, N_STRING, with an optional suffix
   to an integral value.  Suffixes are handled as with --from=auto.
   Upon successful conversion, return that value.
   If it cannot be converted, give a diagnostic and exit.  */
நிலையான uintmax_t
unit_to_umax (எழுத்து மாறிலி *n_string)
{
  strtol_error s_err;
  எழுத்து மாறிலி *c_string = n_string;
  எழுத்து *t_string = வெற்று;
  size_t n_len = strlen (n_string);
  எழுத்து *end = வெற்று;
  uintmax_t n;
  எழுத்து மாறிலி *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  எனில் (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      memcpy (t_string, n_string, n_len);

      எனில் (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
        *end = '\0';
      இல்லையெனில்
        {
          *++end = 'B';
          *++end = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &end, 10, &n, suffixes);

  எனில் (s_err != LONGINT_OK || *end || n == 0)
    {
      விடுவி (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  விடுவி (t_string);

  திரும்பு n;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [எண்_2]...\n\
"), program_name);
      fputs (_("\
Reformat எண்_2(s), or the numbers from standard உள்ளீடு எனில் none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid உள்ளீடு\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace க்கு field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these உள்ளீடு fields (இயல்பு=1);\n\
         see FIELDS below க்கு details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use printf style floating-point FORMAT;\n\
         see FORMAT below க்கு details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         auto-scale உள்ளீடு numbers to UNITs; இயல்பு is 'none';\n\
         see UNIT below க்கு details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the உள்ளீடு unit size (instead of the இயல்பு 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header வரிகள்;\n\
         N defaults to 1 எனில் not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode க்கு invalid numbers;\n\
         MODE can be: abort (இயல்பு), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will வலது-align, negative N will இடது-align;\n\
         padding is ignored எனில் the output is wider than N;\n\
         the இயல்பு is to automatically pad எனில் a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD க்கு rounding when scaling; METHOD can be:\n\
         up, down, from-zero (இயல்பு), towards-zero, nearest\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         add SUFFIX to output numbers,\n\
         and accept an optional SUFFIX in உள்ளீடு numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         செருகு SEP between எண் and unit on output,\n\
         and accept an optional SEP in உள்ளீடு numbers\n\
"));
      oputs (_("\
      --to=UNIT\n\
         auto-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit size (instead of the இயல்பு 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         வரி delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\
\n\
UNIT options:\n"), stdout);
      fputs (_("\
  none       no auto-scaling is done; suffixes will trigger an error\n\
"), stdout);
      fputs (_("\
  auto       accept optional single/two letter suffix:\n\
               1K = 1000, 1k = 1000,\n\
               1Ki = 1024,\n\
               1M = 1000000,\n\
               1Mi = 1048576,\n"), stdout);
      fputs (_("\
  si         accept optional single letter suffix:\n\
               1k = 1000, 1K = 1000,\n\
               1M = 1000000,\n\
               ...\n"), stdout);
      fputs (_("\
  iec        accept optional single letter suffix:\n\
               1K = 1024, 1k = 1024,\n\
               1M = 1048576,\n\
               ...\n"), stdout);
      fputs (_("\
  iec-i      accept optional two-letter suffix:\n\
               1Ki = 1024, 1ki = 1024,\n\
               1Mi = 1048576,\n\
               ...\n"), stdout);

      fputs (_("\n\
FIELDS supports cut(1) style field ranges:\n\
  N    N'th field, counted from 1\n\
  N-   from N'th field, to end of வரி\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable க்கு printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (எனில் supported by current locale).\n\
Optional width மதிப்பு (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the எண். Optional negative மதிப்புகள் (%-10f) will இடது align.\n\
Optional precision (%.1f) will override the உள்ளீடு determined precision.\n\
"), stdout);

      printf (_("\n\
Exit status is 0 எனில் all உள்ளீடு numbers were successfully converted.\n\
By இயல்பு, %s will stop at the first conversion error with exit status 2.\n\
With --invalid='fail' a warning is printed க்கு each conversion error\n\
and the exit status is 2.  With --invalid='warn' each conversion error is\n\
diagnosed, but the exit status is 0.  With --invalid='ignore' conversion\n\
errors are not diagnosed and the exit status is 0.\n\
"), program_name);

      printf (_("\n\
Examples:\n\
  $ %s --to=si 1000\n\
            -> \"1.0k\"\n\
  $ %s --to=iec 2048\n\
           -> \"2.0K\"\n\
  $ %s --to=iec-i 4096\n\
           -> \"4.0Ki\"\n\
  $ echo 1K | %s --from=si\n\
           -> \"1000\"\n\
  $ echo 1K | %s --from=iec\n\
           -> \"1024\"\n\
  $ df -B1 | %s --header --field 2-4 --to=si\n\
  $ ls -l  | %s --header --field 5 --to=iec\n\
  $ ls -lh | %s --header --field 5 --from=iec --padding=10\n\
  $ ls -lh | %s --header --field 5 --from=iec --format %%10f\n"),
              program_name, program_name, program_name,
              program_name, program_name, program_name,
              program_name, program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Given 'fmt' (a printf(3) compatible format string), extracts the following:
    1. padding (e.g. %20f)
    2. alignment (e.g. %-20f)
    3. grouping (e.g. %'f)

   Only a limited subset of printf(3) syntax is supported.

   TODO:
     support %e %g etc. rather than just %f

   NOTES:
   1. This function sets the global variables:
       padding_width, grouping,
       format_str_prefix, format_str_suffix
   2. The function aborts on any errors.  */
நிலையான வெறுமை
parse_format_string (எழுத்து மாறிலி *fmt)
{
  size_t i;
  size_t prefix_len = 0;
  size_t suffix_pos;
  எழுத்து *endptr = வெற்று;
  bool zero_padding = false;

  க்கு (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      எனில் (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  போது (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      எனில் (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      இல்லையெனில் எனில் (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      இல்லையெனில் எனில் (! skip)
        நிறுத்து;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  எனில் (pad != 0)
    {
      எனில் (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      எனில் (pad < 0)
        padding_width = pad;
      இல்லையெனில்
        {
          எனில் (zero_padding)
            zero_padding_width = MIN (pad, INT_MAX);
          இல்லையெனில்
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  எனில் (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  எனில் (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      எனில் (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
          || c_isblank (fmt[i]) || fmt[i] == '+')
        {
          /* Note we disallow negative user_precision to be
             consistent with printf(1).  POSIX states that
             negative precision is only supported (and ignored)
             when used with '.*f'.  glibc at least will malform
             output when passed a direct negative precision.  */
          error (EXIT_FAILURE, 0,
                 _("invalid precision in format %s"), quote (fmt));
        }
      i = endptr - fmt;
    }

  எனில் (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  க்கு (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    எனில் (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  எனில் (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  எனில் (fmt[suffix_pos] != '\0')
    format_str_suffix = xstrdup (fmt + suffix_pos);

  devmsg ("format String:\n  input: %s\n  grouping: %s\n"
                   "  padding width: %jd\n"
                   "  prefix: %s\n  suffix: %s\n",
          quote_n (0, fmt), (grouping) ? "yes" : "no",
          padding_width,
          quote_n (1, format_str_prefix ? format_str_prefix : ""),
          quote_n (2, format_str_suffix ? format_str_suffix : ""));
}

/* Parse a numeric value (with optional suffix) from a string.
   Returns a long double value, with input precision.

   If there's an error converting the string to value - exits with
   an error.

   If there are any trailing characters after the number
   (besides a valid suffix) - exits with an error.  */
நிலையான எண்ணிக்கை simple_strtod_error
parse_human_எண் (எழுத்து மாறிலி *str, long இரட்டை /*output */ *மதிப்பு,
                    size_t *precision)
{
  எழுத்து *ptr = வெற்று;

  எண்ணிக்கை simple_strtod_error e =
    simple_strtod_human (str, &ptr, மதிப்பு, precision, scale_from);
  எனில் (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      திரும்பு e;
    }

  எனில் (ptr && *ptr != '\0')
    {
      எனில் (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  திரும்பு e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
நிலையான bool
prepare_padded_எண் (மாறிலி long இரட்டை val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  முழு x;
  expld (val, 10, &x);

  எனில் (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      எனில் (inval_style != inval_ignore)
        {
          எனில் (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          இல்லையெனில்
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      திரும்பு false;
    }

  எனில் (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      எனில் (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      திரும்பு false;
    }

  போது (true)
    {
      முழு numlen = double_to_human (val, precision_used,
                                    padding_buffer, padding_buffer_size,
                                    scale_to, grouping, round_style);
      ptrdiff_t growth;
      எனில் (numlen < 0 || ckd_sub (&growth, numlen, padding_buffer_size - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      எனில் (growth <= 0)
        நிறுத்து;
      padding_buffer = xpalloc (padding_buffer, &padding_buffer_size,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_buffer));

  intmax_t pad = 0;
  எனில் (padding_width)
    {
      முழு buf_width = mbswidth (padding_buffer,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      எனில் (0 <= buf_width)
        {
          எனில் (padding_width < 0)
            {
              எனில் (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          இல்லையெனில்
            {
              எனில் (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  திரும்பு true;
}

நிலையான வெறுமை
print_padded_எண் (intmax_t padding)
{
  எனில் (format_str_prefix)
    fputs (format_str_prefix, stdout);

  க்கு (intmax_t p = padding; 0 < p; p--)
    putchar (' ');

  fputs (padding_buffer, stdout);

  க்கு (intmax_t p = padding; p < 0; p++)
    putchar (' ');

  எனில் (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
நிலையான bool
process_suffixed_எண் (எழுத்து *text, long இரட்டை *result,
                         size_t *precision, long முழு field)
{
  எழுத்து saved_suffix = '\0';

  எனில் (suffix)
    {
      எனில் (mbs_endswith (text, suffix))
        {
          saved_suffix = *(text + strlen (text) - strlen (suffix));
          *(text + strlen (text) - strlen (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      இல்லையெனில்
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  எழுத்து *p = skip_str_matching (text, newline_or_blank, true);

  /* setup auto-padding.  */
  எனில் (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      எனில் (padding_width < 0)
        padding_width = strlen (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  long இரட்டை val = 0;
  எண்ணிக்கை simple_strtod_error e = parse_human_எண் (p, &val, precision);
  எனில் (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  எனில் (from_unit_size != 1 || to_unit_size != 1)
    val = (val * from_unit_size) / to_unit_size;

  *result = val;

  எனில் (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    திரும்பு true;
  இல்லையெனில்
    {
      எனில் (saved_suffix)
        *(text + strlen (text)) = saved_suffix;
      திரும்பு false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
நிலையான எழுத்து *
mbsmbchr (எழுத்து மாறிலி *s, எழுத்து மாறிலி *c)
{
  unsigned எழுத்து uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  எனில் (uc < 0x30 || MB_CUR_MAX == 1)
    திரும்பு (எழுத்து *) strchr (s, uc);
  இல்லையெனில் எனில் (is_utf8_charset ())
    திரும்பு (எழுத்து *) (uc < 0x80 ? strchr (s, uc) : strstr (s, c));
  இல்லையெனில்
    திரும்பு *(c + 1) == '\0' ? mbschr (s, uc) : (எழுத்து *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
நிலையான எழுத்து *
next_field (எழுத்து **வரி)
{
  எழுத்து *field_start = *வரி;
  எழுத்து *field_end   = field_start;

  எனில் (delimiter)
    {
      எனில் (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = strchr (field_start, '\0');
    }
  இல்லையெனில்
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *வரி = field_end;
  திரும்பு field_start;
}

ATTRIBUTE_PURE
நிலையான bool
include_field (uintmax_t field)
{
  கட்டமைப்பு field_range_pair *p = frp;
  எனில் (!p)
    திரும்பு field == 1;

  போது (p->lo != UINTMAX_MAX)
    {
      எனில் (p->lo <= field && p->hi >= field)
        திரும்பு true;
      ++p;
    }
  திரும்பு false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
நிலையான bool
process_field (எழுத்து *text, uintmax_t field)
{
  long இரட்டை val = 0;
  size_t precision = 0;
  bool valid_எண் = true;

  எனில் (include_field (field))
    {
      valid_எண் =
        process_suffixed_எண் (text, &val, &precision, field);

      intmax_t padding;
      எனில் (valid_எண்)
        valid_எண் = prepare_padded_எண் (val, precision, &padding);

      எனில் (valid_எண்)
        print_padded_எண் (padding);
      இல்லையெனில்
        fputs (text, stdout);
    }
  இல்லையெனில்
    fputs (text, stdout);

  திரும்பு valid_எண்;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
நிலையான முழு
process_வரி (எழுத்து *வரி, bool newline)
{
  எழுத்து *next;
  uintmax_t field = 0;
  bool valid_எண் = true;

  போது (true) {
    ++field;
    next = next_field (&வரி);

    எனில் (*வரி != '\0')
      {
        /* NUL terminate the current field string and process */
        எழுத்து end_field = *வரி;
        *வரி = '\0';

        எனில் (! process_field (next, field))
          valid_எண் = false;

        எனில் (delimiter != வெற்று)
          fputs (delimiter, stdout);
        இல்லையெனில்
          fputc (' ', stdout);

        எனில் (delimiter)
          வரி += MAX (strlen (delimiter), 1);
        இல்லையெனில்
          {
            *வரி = end_field;
            mcel_t g = mcel_scanz (வரி);
            வரி += g.len;
          }
      }
    இல்லையெனில்
      {
        /* end of the line, process the last field and finish */
        எனில் (! process_field (next, field))
          valid_எண் = false;

        நிறுத்து;
      }
  }

  எனில் (newline)
    putchar (வரி_delim);

 எனில் (ferror (stdout))
   write_error ();

  திரும்பு valid_எண்;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#எனில் HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#endif

  decimal_point = nl_langinfo (RADIXCHAR);
  எனில் (decimal_point == வெற்று || strlen (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_length = strlen (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  எனில் (thousands_sep == வெற்று)
    thousands_sep = "";
  thousands_sep_length = strlen (thousands_sep);

  atexit (close_stdout);

  போது (true)
    {
      முழு c = getopt_long (argc, argv, "d:z", longopts, வெற்று);

      எனில் (c == -1)
        நிறுத்து;

      தேர்வு (c)
        {
        நிலை FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          நிறுத்து;

        நிலை FROM_UNIT_OPTION:
          from_unit_size = unit_to_umax (optarg);
          நிறுத்து;

        நிலை TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          நிறுத்து;

        நிலை TO_UNIT_OPTION:
          to_unit_size = unit_to_umax (optarg);
          நிறுத்து;

        நிலை ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          நிறுத்து;

        நிலை GROUPING_OPTION:
          grouping = 1;
          நிறுத்து;

        நிலை PADDING_OPTION:
          எனில் (((xstrtoimax (optarg, வெற்று, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          நிறுத்து;

        நிலை FIELD_OPTION:
          எனில் (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          நிறுத்து;

        நிலை 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          எனில் (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              எனில் (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          நிறுத்து;

        நிலை 'z':
          வரி_delim = '\0';
          நிறுத்து;

        நிலை SUFFIX_OPTION:
          suffix = optarg;
          நிறுத்து;

        நிலை UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          நிறுத்து;

        நிலை DEBUG_OPTION:
          debug = true;
          நிறுத்து;

        நிலை DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          நிறுத்து;

        நிலை HEADER_OPTION:
          எனில் (optarg)
            {
              எனில் (xstrtoumax (optarg, வெற்று, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          இல்லையெனில்
            {
              header = 1;
            }
          நிறுத்து;

        நிலை FORMAT_OPTION:
          format_str = optarg;
          நிறுத்து;

        நிலை INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          நிறுத்து;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (format_str != வெற்று && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  எனில் (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  எனில் (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == வெற்று))
    error (0, 0, _("no conversion option specified"));

  எனில் (debug && unit_separator && delimiter == வெற்று)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  எனில் (format_str)
    parse_format_string (format_str);

  எனில் (grouping)
    {
      எனில் (scale_to != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      எனில் (debug && thousands_sep_length == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == வெற்று);

  எனில் (inval_style != inval_abort)
    conv_exit_code = 0;

  எனில் (argc > optind)
    {
      எனில் (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      க்கு (; optind < argc; optind++)
        valid_numbers &= process_வரி (argv[optind], true);
    }
  இல்லையெனில்
    {
      எழுத்து *வரி = வெற்று;
      size_t வரி_allocated = 0;
      ssize_t len;

      போது (header-- && getdelim (&வரி, &வரி_allocated,
                                   வரி_delim, stdin) > 0)
        {
          எனில் (fputs (வரி, stdout) == EOF)
            write_error ();
        }

      போது ((len = getdelim (&வரி, &வரி_allocated,
                              வரி_delim, stdin)) > 0)
        {
          bool newline = வரி[len - 1] == வரி_delim;
          எனில் (newline)
            வரி[len - 1] = '\0';
          valid_numbers &= process_வரி (வரி, newline);
        }

      எனில் (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  எனில் (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  முழு exit_status = EXIT_SUCCESS;
  எனில் (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
