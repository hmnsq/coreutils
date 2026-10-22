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

#अन्तर्भाव <config.h>
#अन्तर्भाव <float.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <langinfo.h>

#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "mbswidth.h"
#अन्तर्भाव "mcel.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "skipchars.h"
#अन्तर्भाव "system.h"
#अन्तर्भाव "utf8.h"
#अन्तर्भाव "xstrtol.h"

#अन्तर्भाव "set-fields.h"

#यदि HAVE_FPSETPREC
# include <ieeefp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "numfmt"

#define AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
गणना { EXIT_CONVERSION_WARNINGS = 2 };

गणना
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

गणना scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

स्थिर वर्ण ध्रुव *ध्रुव scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", रिक्त
};

स्थिर गणना scale_type ध्रुव scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

स्थिर वर्ण ध्रुव *ध्रुव scale_to_args[] =
{
  "none", "si", "iec", "iec-i", रिक्त
};

स्थिर गणना scale_type ध्रुव scale_to_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


गणना round_type
{
  round_ceiling,
  round_floor,
  round_from_zero,
  round_to_zero,
  round_nearest,
};

स्थिर वर्ण ध्रुव *ध्रुव round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", रिक्त
};

स्थिर गणना round_type ध्रुव round_types[] =
{
  round_ceiling, round_floor, round_from_zero, round_to_zero, round_nearest
};


गणना inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

स्थिर वर्ण ध्रुव *ध्रुव inval_args[] =
{
  "abort", "fail", "warn", "ignore", रिक्त
};

स्थिर गणना inval_type ध्रुव inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"from", required_argument, रिक्त, FROM_OPTION},
  {"from-unit", required_argument, रिक्त, FROM_UNIT_OPTION},
  {"to", required_argument, रिक्त, TO_OPTION},
  {"to-unit", required_argument, रिक्त, TO_UNIT_OPTION},
  {"round", required_argument, रिक्त, ROUND_OPTION},
  {"padding", required_argument, रिक्त, PADDING_OPTION},
  {"suffix", required_argument, रिक्त, SUFFIX_OPTION},
  {"unit-separator", required_argument, रिक्त, UNIT_SEPARATOR_OPTION},
  {"grouping", no_argument, रिक्त, GROUPING_OPTION},
  {"delimiter", required_argument, रिक्त, 'd'},
  {"field", required_argument, रिक्त, FIELD_OPTION},
  {"debug", no_argument, रिक्त, DEBUG_OPTION},
  {"-debug", no_argument, रिक्त, DEV_DEBUG_OPTION},
  {"header", optional_argument, रिक्त, HEADER_OPTION},
  {"format", required_argument, रिक्त, FORMAT_OPTION},
  {"invalid", required_argument, रिक्त, INVALID_OPTION},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
गणना { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
गणना { MAX_ACCEPTABLE_DIGITS = 33 };

स्थिर गणना scale_type scale_from = scale_none;
स्थिर गणना scale_type scale_to = scale_none;
स्थिर गणना round_type round_style = round_from_zero;
स्थिर गणना inval_type inval_style = inval_abort;
स्थिर वर्ण ध्रुव *suffix = रिक्त;
स्थिर वर्ण ध्रुव *unit_separator = रिक्त;
स्थिर uintmax_t from_unit_size = 1;
स्थिर uintmax_t to_unit_size = 1;
स्थिर पूर्णाङ्क grouping = 0;
स्थिर वर्ण *padding_buffer = रिक्त;
स्थिर idx_t padding_buffer_size = 0;
स्थिर intmax_t padding_width = 0;
स्थिर पूर्णाङ्क zero_padding_width = 0;
स्थिर long पूर्णाङ्क user_precision = -1;
स्थिर वर्ण ध्रुव *format_str = रिक्त;
स्थिर वर्ण *format_str_prefix = रिक्त;
स्थिर वर्ण *format_str_suffix = रिक्त;

/* By default, any conversion error will terminate the program.  */
स्थिर पूर्णाङ्क conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
स्थिर पूर्णाङ्क auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
स्थिर वर्ण ध्रुव *delimiter = रिक्त;

/* line delimiter.  */
स्थिर unsigned वर्ण ओळ_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
स्थिर uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
स्थिर bool debug;

/* will be set according to the current locale.  */
स्थिर वर्ण ध्रुव *decimal_point;
स्थिर पूर्णाङ्क decimal_point_length;
स्थिर वर्ण ध्रुव *thousands_sep;
स्थिर पूर्णाङ्क thousands_sep_length;

/* debugging for developers.  Enables devmsg().  */
स्थिर bool dev_debug = false;

स्थिर bool
newline_or_blank (mcel_t g)
{
  प्रत्यागम g.ch == '\n' || c32issep (g.ch);
}

स्थिर अन्तःस्थ पूर्णाङ्क
default_scale_base (गणना scale_type scale)
{
  विकल्प (scale)
    {
    पक्ष scale_IEC:
    पक्ष scale_IEC_I:
      प्रत्यागम 1024;

    पक्ष scale_none:
    पक्ष scale_auto:
    पक्ष scale_SI:
    स्वभाव:
      प्रत्यागम 1000;
    }
}

स्थिर वर्ण ध्रुव zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
स्थिर वर्ण ध्रुव *valid_suffixes = 1 + zero_and_valid_suffixes;

स्थिर अन्तःस्थ bool
valid_suffix (ध्रुव वर्ण suf)
{
  प्रत्यागम strchr (valid_suffixes, suf) != रिक्त;
}

स्थिर अन्तःस्थ पूर्णाङ्क
suffix_power (ध्रुव वर्ण suf)
{
  विकल्प (suf)
    {
    पक्ष 'k':                  /* kilo.  */
    पक्ष 'K':                  /* kilo or kibi.  */
      प्रत्यागम 1;

    पक्ष 'M':                  /* mega or mebi.  */
      प्रत्यागम 2;

    पक्ष 'G':                  /* giga or gibi.  */
      प्रत्यागम 3;

    पक्ष 'T':                  /* tera or tebi.  */
      प्रत्यागम 4;

    पक्ष 'P':                  /* peta or pebi.  */
      प्रत्यागम 5;

    पक्ष 'E':                  /* exa or exbi.  */
      प्रत्यागम 6;

    पक्ष 'Z':                  /* zetta or 2**70.  */
      प्रत्यागम 7;

    पक्ष 'Y':                  /* yotta or 2**80.  */
      प्रत्यागम 8;

    पक्ष 'R':                  /* ronna or 2**90.  */
      प्रत्यागम 9;

    पक्ष 'Q':                  /* quetta or 2**100.  */
      प्रत्यागम 10;

    स्वभाव:                   /* should never happen. assert?  */
      प्रत्यागम 0;
    }
}

स्थिर अन्तःस्थ वर्ण ध्रुव *
suffix_power_char (पूर्णाङ्क power)
{
  विकल्प (power)
    {
    पक्ष 0:
      प्रत्यागम "";

    पक्ष 1:
      प्रत्यागम "K";

    पक्ष 2:
      प्रत्यागम "M";

    पक्ष 3:
      प्रत्यागम "G";

    पक्ष 4:
      प्रत्यागम "T";

    पक्ष 5:
      प्रत्यागम "P";

    पक्ष 6:
      प्रत्यागम "E";

    पक्ष 7:
      प्रत्यागम "Z";

    पक्ष 8:
      प्रत्यागम "Y";

    पक्ष 9:
      प्रत्यागम "R";

    पक्ष 10:
      प्रत्यागम "Q";

    स्वभाव:
      प्रत्यागम "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
स्थिर long द्विगुण
powerld (long द्विगुण base, ptrdiff_t x)
{
  long द्विगुण result = base;
  यदि (x == 0)
    प्रत्यागम 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  यावत् (--x)
    result *= base;
  प्रत्यागम result;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
स्थिर अन्तःस्थ long द्विगुण
absld (long द्विगुण val)
{
  प्रत्यागम val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
स्थिर long द्विगुण
expld (long द्विगुण val, पूर्णाङ्क base, पूर्णाङ्क /*output */ *x)
{
  पूर्णाङ्क power = 0;

  यदि (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      यावत् (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  यदि (x)
    *x = power;
  प्रत्यागम val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
स्थिर अन्तःस्थ intmax_t
simple_round_ceiling (long द्विगुण val)
{
  intmax_t intval = val;
  यदि (intval < val)
    intval++;
  प्रत्यागम intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
स्थिर अन्तःस्थ intmax_t
simple_round_floor (long द्विगुण val)
{
  प्रत्यागम -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
स्थिर अन्तःस्थ intmax_t
simple_round_from_zero (long द्विगुण val)
{
  प्रत्यागम val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
स्थिर अन्तःस्थ intmax_t
simple_round_to_zero (long द्विगुण val)
{
  प्रत्यागम val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
स्थिर अन्तःस्थ intmax_t
simple_round_nearest (long द्विगुण val)
{
  प्रत्यागम val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
स्थिर अन्तःस्थ long द्विगुण
simple_round (long द्विगुण val, गणना round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (long द्विगुण) INTMAX_MAX * intmax_mul;

  विकल्प (t)
    {
    पक्ष round_ceiling:
      rval = simple_round_ceiling (val);
      भङ्ग;

    पक्ष round_floor:
      rval = simple_round_floor (val);
      भङ्ग;

    पक्ष round_from_zero:
      rval = simple_round_from_zero (val);
      भङ्ग;

    पक्ष round_to_zero:
      rval = simple_round_to_zero (val);
      भङ्ग;

    पक्ष round_nearest:
      rval = simple_round_nearest (val);
      भङ्ग;

    स्वभाव:
      /* to silence the compiler - this should never happen.  */
      प्रत्यागम 0;
    }

  प्रत्यागम (long द्विगुण) INTMAX_MAX * intmax_mul + rval;
}

गणना simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_OVERFLOW,
  SSE_INVALID_संख्या,

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
स्थिर गणना simple_strtod_error
simple_strtod_int (वर्ण ध्रुव *आदान_str,
                   वर्ण **endptr, long द्विगुण *मूल्य, bool *negative)
{
  गणना simple_strtod_error e = SSE_OK;

  long द्विगुण val = 0;
  पूर्णाङ्क digits = 0;
  bool found_digit = false;

  यदि (*आदान_str == '-')
    {
      आदान_str++;
      *negative = true;
    }
  अन्यथा
    *negative = false;

  *endptr = (वर्ण *) आदान_str;
  यावत् (c_isdigit (**endptr))
    {
      पूर्णाङ्क digit = (**endptr) - '0';

      found_digit = true;

      यदि (val || digit)
        digits++;

      यदि (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      यदि (digits > MAX_ACCEPTABLE_DIGITS)
        प्रत्यागम SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      यदि (thousands_sep_length > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_length)
          && c_isdigit ((*endptr)[thousands_sep_length]))
        (*endptr) += thousands_sep_length;
    }
  यदि (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    प्रत्यागम SSE_INVALID_संख्या;
  यदि (*negative)
    val = -val;

  यदि (मूल्य)
    *मूल्य = val;

  प्रत्यागम e;
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
स्थिर गणना simple_strtod_error
simple_strtod_float (वर्ण ध्रुव *आदान_str,
                     वर्ण **endptr,
                     long द्विगुण *मूल्य,
                     size_t *precision)
{
  bool negative;
  गणना simple_strtod_error e = SSE_OK;

  यदि (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (आदान_str, endptr, मूल्य, &negative);
  यदि (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    प्रत्यागम e;

  /* optional decimal point + fraction.  */
  यदि (STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    {
      वर्ण *ptr2;
      long द्विगुण val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_length;
      गणना simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      यदि (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        प्रत्यागम e2;
      यदि (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      यदि (neg_frac)
        प्रत्यागम SSE_INVALID_संख्या;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      यदि (मूल्य)
        {
          यदि (negative)
            *मूल्य -= val_frac;
          अन्यथा
            *मूल्य += val_frac;
        }

      यदि (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  प्रत्यागम e;
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
स्थिर गणना simple_strtod_error
simple_strtod_human (वर्ण ध्रुव *आदान_str,
                     वर्ण **endptr, long द्विगुण *मूल्य, size_t *precision,
                     गणना scale_type allowed_scaling)
{
  पूर्णाङ्क power = 0;
  /* 'scale_auto' is checked below.  */
  पूर्णाङ्क scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, आदान_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  गणना simple_strtod_error e =
    simple_strtod_float (आदान_str, endptr, मूल्य, precision);
  यदि (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    प्रत्यागम e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *मूल्य, (पूर्णाङ्क)*precision);

  यावत् (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      यदि (unit_separator)
        {
          size_t sep_len = strlen (unit_separator);
          यदि (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      यदि (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          यदि (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      यदि (**endptr == '\0')
        भङ्ग;  /* Treat as no suffix.  */

      यदि (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          यदि (**endptr == '\0')
            भङ्ग;

          प्रत्यागम SSE_INVALID_SUFFIX;
        }

      यदि (allowed_scaling == scale_none)
        प्रत्यागम SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      यदि (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      अन्यथा यदि (allowed_scaling == scale_IEC_I)
        {
          यदि (**endptr == 'i')
            (*endptr)++;
          अन्यथा
            प्रत्यागम SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      भङ्ग;
    }

  long द्विगुण multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*मूल्य) = (*मूल्य) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *मूल्य, *मूल्य);

  प्रत्यागम e;
}


स्थिर शून्य
simple_strtod_fatal (गणना simple_strtod_error err, वर्ण ध्रुव *आदान_str)
{
  वर्ण ध्रुव *msgid = रिक्त;

  विकल्प (err)
    {
    पक्ष SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      भङ्ग;

    पक्ष SSE_INVALID_संख्या:
      msgid = N_("invalid number: %s");
      भङ्ग;

    पक्ष SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      भङ्ग;

    पक्ष SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      भङ्ग;

    पक्ष SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      भङ्ग;

    पक्ष SSE_OK_PRECISION_LOSS:
    पक्ष SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  यदि (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (आदान_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
स्थिर पूर्णाङ्क
double_to_human (long द्विगुण val, पूर्णाङ्क precision,
                 वर्ण *buf, idx_t buf_size,
                 गणना scale_type scale, पूर्णाङ्क group, गणना round_type round)
{
  वर्ण fmt[परिमाण "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  वर्ण *pfmt = fmt;
  *pfmt++ = '%';

  यदि (group)
    *pfmt++ = '\'';

  यदि (zero_padding_width)
    pfmt += sprintf (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  यदि (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      प्रत्यागम snprintf (buf, buf_size, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  द्विगुण scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  पूर्णाङ्क power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  पूर्णाङ्क power_adjust = 0;
  यदि (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  अन्यथा यदि (absld (val) < 10)
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
  यदि (absld (val) >= scale_base)
    {
      val /= scale_base;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  पूर्णाङ्क show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  पूर्णाङ्क prec = user_precision == -1 ? show_decimal_point : user_precision;

  प्रत्यागम snprintf (buf, buf_size, fmt, prec, val,
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
स्थिर uintmax_t
unit_to_umax (वर्ण ध्रुव *n_string)
{
  strtol_error s_err;
  वर्ण ध्रुव *c_string = n_string;
  वर्ण *t_string = रिक्त;
  size_t n_len = strlen (n_string);
  वर्ण *end = रिक्त;
  uintmax_t n;
  वर्ण ध्रुव *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  यदि (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      memcpy (t_string, n_string, n_len);

      यदि (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
        *end = '\0';
      अन्यथा
        {
          *++end = 'B';
          *++end = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &end, 10, &n, suffixes);

  यदि (s_err != LONGINT_OK || *end || n == 0)
    {
      मुक्त (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  मुक्त (t_string);

  प्रत्यागम n;
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      printf (_("\
Usage: %s [OPTION]... [संख्या_2]...\n\
"), program_name);
      fputs (_("\
Reformat संख्या_2(s), or the numbers from standard आदान यदि none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid आदान\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace प्रति field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these आदान fields (स्वभाव=1);\n\
         see FIELDS below प्रति details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use printf style floating-point FORMAT;\n\
         see FORMAT below प्रति details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         auto-scale आदान numbers to UNITs; स्वभाव is 'none';\n\
         see UNIT below प्रति details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the आदान unit size (instead of the स्वभाव 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header ओळी;\n\
         N defaults to 1 यदि not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode प्रति invalid numbers;\n\
         MODE can be: abort (स्वभाव), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will उजवा-align, negative N will डावा-align;\n\
         padding is ignored यदि the output is wider than N;\n\
         the स्वभाव is to automatically pad यदि a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD प्रति rounding when scaling; METHOD can be:\n\
         up, down, from-zero (स्वभाव), towards-zero, nearest\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         add SUFFIX to output numbers,\n\
         and accept an optional SUFFIX in आदान numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         घाला SEP between संख्या and unit on output,\n\
         and accept an optional SEP in आदान numbers\n\
"));
      oputs (_("\
      --to=UNIT\n\
         auto-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit size (instead of the स्वभाव 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ओळ delimiter is NUL, not newline\n\
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
  N-   from N'th field, to end of ओळ\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable प्रति printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (यदि supported by current locale).\n\
Optional width मूल्य (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the संख्या. Optional negative मूल्ये (%-10f) will डावा align.\n\
Optional precision (%.1f) will override the आदान determined precision.\n\
"), stdout);

      printf (_("\n\
Exit status is 0 यदि all आदान numbers were successfully converted.\n\
By स्वभाव, %s will stop at the first conversion error with exit status 2.\n\
With --invalid='fail' a warning is printed प्रति each conversion error\n\
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
स्थिर शून्य
parse_format_string (वर्ण ध्रुव *fmt)
{
  size_t i;
  size_t prefix_len = 0;
  size_t suffix_pos;
  वर्ण *endptr = रिक्त;
  bool zero_padding = false;

  प्रति (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      यदि (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  यावत् (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      यदि (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      अन्यथा यदि (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      अन्यथा यदि (! skip)
        भङ्ग;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  यदि (pad != 0)
    {
      यदि (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      यदि (pad < 0)
        padding_width = pad;
      अन्यथा
        {
          यदि (zero_padding)
            zero_padding_width = MIN (pad, INT_MAX);
          अन्यथा
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  यदि (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  यदि (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      यदि (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  यदि (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  प्रति (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    यदि (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  यदि (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  यदि (fmt[suffix_pos] != '\0')
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
स्थिर गणना simple_strtod_error
parse_human_संख्या (वर्ण ध्रुव *str, long द्विगुण /*output */ *मूल्य,
                    size_t *precision)
{
  वर्ण *ptr = रिक्त;

  गणना simple_strtod_error e =
    simple_strtod_human (str, &ptr, मूल्य, precision, scale_from);
  यदि (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      प्रत्यागम e;
    }

  यदि (ptr && *ptr != '\0')
    {
      यदि (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  प्रत्यागम e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
स्थिर bool
prepare_padded_संख्या (ध्रुव long द्विगुण val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  पूर्णाङ्क x;
  expld (val, 10, &x);

  यदि (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      यदि (inval_style != inval_ignore)
        {
          यदि (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          अन्यथा
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      प्रत्यागम false;
    }

  यदि (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      यदि (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      प्रत्यागम false;
    }

  यावत् (true)
    {
      पूर्णाङ्क numlen = double_to_human (val, precision_used,
                                    padding_buffer, padding_buffer_size,
                                    scale_to, grouping, round_style);
      ptrdiff_t growth;
      यदि (numlen < 0 || ckd_sub (&growth, numlen, padding_buffer_size - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      यदि (growth <= 0)
        भङ्ग;
      padding_buffer = xpalloc (padding_buffer, &padding_buffer_size,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_buffer));

  intmax_t pad = 0;
  यदि (padding_width)
    {
      पूर्णाङ्क buf_width = mbswidth (padding_buffer,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      यदि (0 <= buf_width)
        {
          यदि (padding_width < 0)
            {
              यदि (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          अन्यथा
            {
              यदि (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  प्रत्यागम true;
}

स्थिर शून्य
print_padded_संख्या (intmax_t padding)
{
  यदि (format_str_prefix)
    fputs (format_str_prefix, stdout);

  प्रति (intmax_t p = padding; 0 < p; p--)
    putchar (' ');

  fputs (padding_buffer, stdout);

  प्रति (intmax_t p = padding; p < 0; p++)
    putchar (' ');

  यदि (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
स्थिर bool
process_suffixed_संख्या (वर्ण *text, long द्विगुण *result,
                         size_t *precision, long पूर्णाङ्क field)
{
  वर्ण saved_suffix = '\0';

  यदि (suffix)
    {
      यदि (mbs_endswith (text, suffix))
        {
          saved_suffix = *(text + strlen (text) - strlen (suffix));
          *(text + strlen (text) - strlen (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      अन्यथा
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  वर्ण *p = skip_str_matching (text, newline_or_blank, true);

  /* setup auto-padding.  */
  यदि (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      यदि (padding_width < 0)
        padding_width = strlen (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  long द्विगुण val = 0;
  गणना simple_strtod_error e = parse_human_संख्या (p, &val, precision);
  यदि (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  यदि (from_unit_size != 1 || to_unit_size != 1)
    val = (val * from_unit_size) / to_unit_size;

  *result = val;

  यदि (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    प्रत्यागम true;
  अन्यथा
    {
      यदि (saved_suffix)
        *(text + strlen (text)) = saved_suffix;
      प्रत्यागम false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
स्थिर वर्ण *
mbsmbchr (वर्ण ध्रुव *s, वर्ण ध्रुव *c)
{
  unsigned वर्ण uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  यदि (uc < 0x30 || MB_CUR_MAX == 1)
    प्रत्यागम (वर्ण *) strchr (s, uc);
  अन्यथा यदि (is_utf8_charset ())
    प्रत्यागम (वर्ण *) (uc < 0x80 ? strchr (s, uc) : strstr (s, c));
  अन्यथा
    प्रत्यागम *(c + 1) == '\0' ? mbschr (s, uc) : (वर्ण *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
स्थिर वर्ण *
next_field (वर्ण **ओळ)
{
  वर्ण *field_start = *ओळ;
  वर्ण *field_end   = field_start;

  यदि (delimiter)
    {
      यदि (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = strchr (field_start, '\0');
    }
  अन्यथा
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *ओळ = field_end;
  प्रत्यागम field_start;
}

ATTRIBUTE_PURE
स्थिर bool
include_field (uintmax_t field)
{
  संरचना field_range_pair *p = frp;
  यदि (!p)
    प्रत्यागम field == 1;

  यावत् (p->lo != UINTMAX_MAX)
    {
      यदि (p->lo <= field && p->hi >= field)
        प्रत्यागम true;
      ++p;
    }
  प्रत्यागम false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
स्थिर bool
process_field (वर्ण *text, uintmax_t field)
{
  long द्विगुण val = 0;
  size_t precision = 0;
  bool valid_संख्या = true;

  यदि (include_field (field))
    {
      valid_संख्या =
        process_suffixed_संख्या (text, &val, &precision, field);

      intmax_t padding;
      यदि (valid_संख्या)
        valid_संख्या = prepare_padded_संख्या (val, precision, &padding);

      यदि (valid_संख्या)
        print_padded_संख्या (padding);
      अन्यथा
        fputs (text, stdout);
    }
  अन्यथा
    fputs (text, stdout);

  प्रत्यागम valid_संख्या;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
स्थिर पूर्णाङ्क
process_ओळ (वर्ण *ओळ, bool newline)
{
  वर्ण *next;
  uintmax_t field = 0;
  bool valid_संख्या = true;

  यावत् (true) {
    ++field;
    next = next_field (&ओळ);

    यदि (*ओळ != '\0')
      {
        /* NUL terminate the current field string and process */
        वर्ण end_field = *ओळ;
        *ओळ = '\0';

        यदि (! process_field (next, field))
          valid_संख्या = false;

        यदि (delimiter != रिक्त)
          fputs (delimiter, stdout);
        अन्यथा
          fputc (' ', stdout);

        यदि (delimiter)
          ओळ += MAX (strlen (delimiter), 1);
        अन्यथा
          {
            *ओळ = end_field;
            mcel_t g = mcel_scanz (ओळ);
            ओळ += g.len;
          }
      }
    अन्यथा
      {
        /* end of the line, process the last field and finish */
        यदि (! process_field (next, field))
          valid_संख्या = false;

        भङ्ग;
      }
  }

  यदि (newline)
    putchar (ओळ_delim);

 यदि (ferror (stdout))
   write_error ();

  प्रत्यागम valid_संख्या;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#यदि HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#endif

  decimal_point = nl_langinfo (RADIXCHAR);
  यदि (decimal_point == रिक्त || strlen (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_length = strlen (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  यदि (thousands_sep == रिक्त)
    thousands_sep = "";
  thousands_sep_length = strlen (thousands_sep);

  atexit (close_stdout);

  यावत् (true)
    {
      पूर्णाङ्क c = getopt_long (argc, argv, "d:z", longopts, रिक्त);

      यदि (c == -1)
        भङ्ग;

      विकल्प (c)
        {
        पक्ष FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          भङ्ग;

        पक्ष FROM_UNIT_OPTION:
          from_unit_size = unit_to_umax (optarg);
          भङ्ग;

        पक्ष TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          भङ्ग;

        पक्ष TO_UNIT_OPTION:
          to_unit_size = unit_to_umax (optarg);
          भङ्ग;

        पक्ष ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          भङ्ग;

        पक्ष GROUPING_OPTION:
          grouping = 1;
          भङ्ग;

        पक्ष PADDING_OPTION:
          यदि (((xstrtoimax (optarg, रिक्त, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          भङ्ग;

        पक्ष FIELD_OPTION:
          यदि (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          भङ्ग;

        पक्ष 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          यदि (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              यदि (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          भङ्ग;

        पक्ष 'z':
          ओळ_delim = '\0';
          भङ्ग;

        पक्ष SUFFIX_OPTION:
          suffix = optarg;
          भङ्ग;

        पक्ष UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          भङ्ग;

        पक्ष DEBUG_OPTION:
          debug = true;
          भङ्ग;

        पक्ष DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          भङ्ग;

        पक्ष HEADER_OPTION:
          यदि (optarg)
            {
              यदि (xstrtoumax (optarg, रिक्त, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          अन्यथा
            {
              header = 1;
            }
          भङ्ग;

        पक्ष FORMAT_OPTION:
          format_str = optarg;
          भङ्ग;

        पक्ष INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          भङ्ग;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (format_str != रिक्त && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  यदि (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  यदि (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == रिक्त))
    error (0, 0, _("no conversion option specified"));

  यदि (debug && unit_separator && delimiter == रिक्त)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  यदि (format_str)
    parse_format_string (format_str);

  यदि (grouping)
    {
      यदि (scale_to != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      यदि (debug && thousands_sep_length == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == रिक्त);

  यदि (inval_style != inval_abort)
    conv_exit_code = 0;

  यदि (argc > optind)
    {
      यदि (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      प्रति (; optind < argc; optind++)
        valid_numbers &= process_ओळ (argv[optind], true);
    }
  अन्यथा
    {
      वर्ण *ओळ = रिक्त;
      size_t ओळ_allocated = 0;
      ssize_t len;

      यावत् (header-- && getdelim (&ओळ, &ओळ_allocated,
                                   ओळ_delim, stdin) > 0)
        {
          यदि (fputs (ओळ, stdout) == EOF)
            write_error ();
        }

      यावत् ((len = getdelim (&ओळ, &ओळ_allocated,
                              ओळ_delim, stdin)) > 0)
        {
          bool newline = ओळ[len - 1] == ओळ_delim;
          यदि (newline)
            ओळ[len - 1] = '\0';
          valid_numbers &= process_ओळ (ओळ, newline);
        }

      यदि (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  यदि (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  पूर्णाङ्क exit_status = EXIT_SUCCESS;
  यदि (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
