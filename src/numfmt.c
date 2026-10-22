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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <float.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <langinfo.h>

#অন্তর্ভাৱ "argmatch.h"
#অন্তর্ভাৱ "assure.h"
#অন্তর্ভাৱ "c-ctype.h"
#অন্তর্ভাৱ "mbswidth.h"
#অন্তর্ভাৱ "mcel.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "skipchars.h"
#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "utf8.h"
#অন্তর্ভাৱ "xstrtol.h"

#অন্তর্ভাৱ "set-fields.h"

#যদি HAVE_FPSETPREC
# include <ieeefp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "numfmt"

#define AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
গণনা { EXIT_CONVERSION_WARNINGS = 2 };

গণনা
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

গণনা scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", রিক্ত
};

স্থির গণনা scale_type ধ্রুৱ scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ scale_to_args[] =
{
  "none", "si", "iec", "iec-i", রিক্ত
};

স্থির গণনা scale_type ধ্রুৱ scale_to_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


গণনা round_type
{
  round_ceiling,
  round_floor,
  round_from_zero,
  round_to_zero,
  round_nearest,
};

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", রিক্ত
};

স্থির গণনা round_type ধ্রুৱ round_types[] =
{
  round_ceiling, round_floor, round_from_zero, round_to_zero, round_nearest
};


গণনা inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ inval_args[] =
{
  "abort", "fail", "warn", "ignore", রিক্ত
};

স্থির গণনা inval_type ধ্রুৱ inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {"from", required_argument, রিক্ত, FROM_OPTION},
  {"from-unit", required_argument, রিক্ত, FROM_UNIT_OPTION},
  {"to", required_argument, রিক্ত, TO_OPTION},
  {"to-unit", required_argument, রিক্ত, TO_UNIT_OPTION},
  {"round", required_argument, রিক্ত, ROUND_OPTION},
  {"padding", required_argument, রিক্ত, PADDING_OPTION},
  {"suffix", required_argument, রিক্ত, SUFFIX_OPTION},
  {"unit-separator", required_argument, রিক্ত, UNIT_SEPARATOR_OPTION},
  {"grouping", no_argument, রিক্ত, GROUPING_OPTION},
  {"delimiter", required_argument, রিক্ত, 'd'},
  {"field", required_argument, রিক্ত, FIELD_OPTION},
  {"debug", no_argument, রিক্ত, DEBUG_OPTION},
  {"-debug", no_argument, রিক্ত, DEV_DEBUG_OPTION},
  {"header", optional_argument, রিক্ত, HEADER_OPTION},
  {"format", required_argument, রিক্ত, FORMAT_OPTION},
  {"invalid", required_argument, রিক্ত, INVALID_OPTION},
  {"zero-terminated", no_argument, রিক্ত, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
গণনা { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
গণনা { MAX_ACCEPTABLE_DIGITS = 33 };

স্থির গণনা scale_type scale_from = scale_none;
স্থির গণনা scale_type scale_to = scale_none;
স্থির গণনা round_type round_style = round_from_zero;
স্থির গণনা inval_type inval_style = inval_abort;
স্থির ৱর্ণ ধ্রুৱ *suffix = রিক্ত;
স্থির ৱর্ণ ধ্রুৱ *unit_separator = রিক্ত;
স্থির uintmax_t from_unit_size = 1;
স্থির uintmax_t to_unit_size = 1;
স্থির পূর্ণাঙ্ক grouping = 0;
স্থির ৱর্ণ *padding_buffer = রিক্ত;
স্থির idx_t padding_buffer_size = 0;
স্থির intmax_t padding_width = 0;
স্থির পূর্ণাঙ্ক zero_padding_width = 0;
স্থির long পূর্ণাঙ্ক user_precision = -1;
স্থির ৱর্ণ ধ্রুৱ *format_str = রিক্ত;
স্থির ৱর্ণ *format_str_prefix = রিক্ত;
স্থির ৱর্ণ *format_str_suffix = রিক্ত;

/* By default, any conversion error will terminate the program.  */
স্থির পূর্ণাঙ্ক conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
স্থির পূর্ণাঙ্ক auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
স্থির ৱর্ণ ধ্রুৱ *delimiter = রিক্ত;

/* line delimiter.  */
স্থির unsigned ৱর্ণ সাৰি_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
স্থির uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
স্থির bool debug;

/* will be set according to the current locale.  */
স্থির ৱর্ণ ধ্রুৱ *decimal_point;
স্থির পূর্ণাঙ্ক decimal_point_length;
স্থির ৱর্ণ ধ্রুৱ *thousands_sep;
স্থির পূর্ণাঙ্ক thousands_sep_length;

/* debugging for developers.  Enables devmsg().  */
স্থির bool dev_debug = false;

স্থির bool
newline_or_blank (mcel_t g)
{
  ঘূৰাই g.ch == '\n' || c32issep (g.ch);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
default_scale_base (গণনা scale_type scale)
{
  ৱিকল্প (scale)
    {
    পক্ষ scale_IEC:
    পক্ষ scale_IEC_I:
      ঘূৰাই 1024;

    পক্ষ scale_none:
    পক্ষ scale_auto:
    পক্ষ scale_SI:
    স্ৱভাৱ:
      ঘূৰাই 1000;
    }
}

স্থির ৱর্ণ ধ্রুৱ zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
স্থির ৱর্ণ ধ্রুৱ *valid_suffixes = 1 + zero_and_valid_suffixes;

স্থির অন্তঃস্থ bool
valid_suffix (ধ্রুৱ ৱর্ণ suf)
{
  ঘূৰাই strchr (valid_suffixes, suf) != রিক্ত;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
suffix_power (ধ্রুৱ ৱর্ণ suf)
{
  ৱিকল্প (suf)
    {
    পক্ষ 'k':                  /* kilo.  */
    পক্ষ 'K':                  /* kilo or kibi.  */
      ঘূৰাই 1;

    পক্ষ 'M':                  /* mega or mebi.  */
      ঘূৰাই 2;

    পক্ষ 'G':                  /* giga or gibi.  */
      ঘূৰাই 3;

    পক্ষ 'T':                  /* tera or tebi.  */
      ঘূৰাই 4;

    পক্ষ 'P':                  /* peta or pebi.  */
      ঘূৰাই 5;

    পক্ষ 'E':                  /* exa or exbi.  */
      ঘূৰাই 6;

    পক্ষ 'Z':                  /* zetta or 2**70.  */
      ঘূৰাই 7;

    পক্ষ 'Y':                  /* yotta or 2**80.  */
      ঘূৰাই 8;

    পক্ষ 'R':                  /* ronna or 2**90.  */
      ঘূৰাই 9;

    পক্ষ 'Q':                  /* quetta or 2**100.  */
      ঘূৰাই 10;

    স্ৱভাৱ:                   /* should never happen. assert?  */
      ঘূৰাই 0;
    }
}

স্থির অন্তঃস্থ ৱর্ণ ধ্রুৱ *
suffix_power_char (পূর্ণাঙ্ক power)
{
  ৱিকল্প (power)
    {
    পক্ষ 0:
      ঘূৰাই "";

    পক্ষ 1:
      ঘূৰাই "K";

    পক্ষ 2:
      ঘূৰাই "M";

    পক্ষ 3:
      ঘূৰাই "G";

    পক্ষ 4:
      ঘূৰাই "T";

    পক্ষ 5:
      ঘূৰাই "P";

    পক্ষ 6:
      ঘূৰাই "E";

    পক্ষ 7:
      ঘূৰাই "Z";

    পক্ষ 8:
      ঘূৰাই "Y";

    পক্ষ 9:
      ঘূৰাই "R";

    পক্ষ 10:
      ঘূৰাই "Q";

    স্ৱভাৱ:
      ঘূৰাই "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
স্থির long দ্ৱিগুণ
powerld (long দ্ৱিগুণ base, ptrdiff_t x)
{
  long দ্ৱিগুণ result = base;
  যদি (x == 0)
    ঘূৰাই 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  যেতিয়ালৈ (--x)
    result *= base;
  ঘূৰাই result;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
স্থির অন্তঃস্থ long দ্ৱিগুণ
absld (long দ্ৱিগুণ val)
{
  ঘূৰাই val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
স্থির long দ্ৱিগুণ
expld (long দ্ৱিগুণ val, পূর্ণাঙ্ক base, পূর্ণাঙ্ক /*output */ *x)
{
  পূর্ণাঙ্ক power = 0;

  যদি (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      যেতিয়ালৈ (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  যদি (x)
    *x = power;
  ঘূৰাই val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
স্থির অন্তঃস্থ intmax_t
simple_round_ceiling (long দ্ৱিগুণ val)
{
  intmax_t intval = val;
  যদি (intval < val)
    intval++;
  ঘূৰাই intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
স্থির অন্তঃস্থ intmax_t
simple_round_floor (long দ্ৱিগুণ val)
{
  ঘূৰাই -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
স্থির অন্তঃস্থ intmax_t
simple_round_from_zero (long দ্ৱিগুণ val)
{
  ঘূৰাই val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
স্থির অন্তঃস্থ intmax_t
simple_round_to_zero (long দ্ৱিগুণ val)
{
  ঘূৰাই val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
স্থির অন্তঃস্থ intmax_t
simple_round_nearest (long দ্ৱিগুণ val)
{
  ঘূৰাই val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
স্থির অন্তঃস্থ long দ্ৱিগুণ
simple_round (long দ্ৱিগুণ val, গণনা round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (long দ্ৱিগুণ) INTMAX_MAX * intmax_mul;

  ৱিকল্প (t)
    {
    পক্ষ round_ceiling:
      rval = simple_round_ceiling (val);
      ভঙ্গ;

    পক্ষ round_floor:
      rval = simple_round_floor (val);
      ভঙ্গ;

    পক্ষ round_from_zero:
      rval = simple_round_from_zero (val);
      ভঙ্গ;

    পক্ষ round_to_zero:
      rval = simple_round_to_zero (val);
      ভঙ্গ;

    পক্ষ round_nearest:
      rval = simple_round_nearest (val);
      ভঙ্গ;

    স্ৱভাৱ:
      /* to silence the compiler - this should never happen.  */
      ঘূৰাই 0;
    }

  ঘূৰাই (long দ্ৱিগুণ) INTMAX_MAX * intmax_mul + rval;
}

গণনা simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_OVERFLOW,
  SSE_INVALID_সংখ্যা,

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
স্থির গণনা simple_strtod_error
simple_strtod_int (ৱর্ণ ধ্রুৱ *প্ৰবেশ_str,
                   ৱর্ণ **endptr, long দ্ৱিগুণ *মান, bool *negative)
{
  গণনা simple_strtod_error e = SSE_OK;

  long দ্ৱিগুণ val = 0;
  পূর্ণাঙ্ক digits = 0;
  bool found_digit = false;

  যদি (*প্ৰবেশ_str == '-')
    {
      প্ৰবেশ_str++;
      *negative = true;
    }
  অন্যথা
    *negative = false;

  *endptr = (ৱর্ণ *) প্ৰবেশ_str;
  যেতিয়ালৈ (c_isdigit (**endptr))
    {
      পূর্ণাঙ্ক digit = (**endptr) - '0';

      found_digit = true;

      যদি (val || digit)
        digits++;

      যদি (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      যদি (digits > MAX_ACCEPTABLE_DIGITS)
        ঘূৰাই SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      যদি (thousands_sep_length > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_length)
          && c_isdigit ((*endptr)[thousands_sep_length]))
        (*endptr) += thousands_sep_length;
    }
  যদি (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    ঘূৰাই SSE_INVALID_সংখ্যা;
  যদি (*negative)
    val = -val;

  যদি (মান)
    *মান = val;

  ঘূৰাই e;
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
স্থির গণনা simple_strtod_error
simple_strtod_float (ৱর্ণ ধ্রুৱ *প্ৰবেশ_str,
                     ৱর্ণ **endptr,
                     long দ্ৱিগুণ *মান,
                     size_t *precision)
{
  bool negative;
  গণনা simple_strtod_error e = SSE_OK;

  যদি (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (প্ৰবেশ_str, endptr, মান, &negative);
  যদি (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ঘূৰাই e;

  /* optional decimal point + fraction.  */
  যদি (STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    {
      ৱর্ণ *ptr2;
      long দ্ৱিগুণ val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_length;
      গণনা simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      যদি (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        ঘূৰাই e2;
      যদি (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      যদি (neg_frac)
        ঘূৰাই SSE_INVALID_সংখ্যা;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      যদি (মান)
        {
          যদি (negative)
            *মান -= val_frac;
          অন্যথা
            *মান += val_frac;
        }

      যদি (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  ঘূৰাই e;
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
স্থির গণনা simple_strtod_error
simple_strtod_human (ৱর্ণ ধ্রুৱ *প্ৰবেশ_str,
                     ৱর্ণ **endptr, long দ্ৱিগুণ *মান, size_t *precision,
                     গণনা scale_type allowed_scaling)
{
  পূর্ণাঙ্ক power = 0;
  /* 'scale_auto' is checked below.  */
  পূর্ণাঙ্ক scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, প্ৰবেশ_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  গণনা simple_strtod_error e =
    simple_strtod_float (প্ৰবেশ_str, endptr, মান, precision);
  যদি (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ঘূৰাই e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *মান, (পূর্ণাঙ্ক)*precision);

  যেতিয়ালৈ (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      যদি (unit_separator)
        {
          size_t sep_len = strlen (unit_separator);
          যদি (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      যদি (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          যদি (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      যদি (**endptr == '\0')
        ভঙ্গ;  /* Treat as no suffix.  */

      যদি (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          যদি (**endptr == '\0')
            ভঙ্গ;

          ঘূৰাই SSE_INVALID_SUFFIX;
        }

      যদি (allowed_scaling == scale_none)
        ঘূৰাই SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      যদি (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      অন্যথা যদি (allowed_scaling == scale_IEC_I)
        {
          যদি (**endptr == 'i')
            (*endptr)++;
          অন্যথা
            ঘূৰাই SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      ভঙ্গ;
    }

  long দ্ৱিগুণ multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*মান) = (*মান) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *মান, *মান);

  ঘূৰাই e;
}


স্থির শূন্য
simple_strtod_fatal (গণনা simple_strtod_error err, ৱর্ণ ধ্রুৱ *প্ৰবেশ_str)
{
  ৱর্ণ ধ্রুৱ *msgid = রিক্ত;

  ৱিকল্প (err)
    {
    পক্ষ SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      ভঙ্গ;

    পক্ষ SSE_INVALID_সংখ্যা:
      msgid = N_("invalid number: %s");
      ভঙ্গ;

    পক্ষ SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      ভঙ্গ;

    পক্ষ SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      ভঙ্গ;

    পক্ষ SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      ভঙ্গ;

    পক্ষ SSE_OK_PRECISION_LOSS:
    পক্ষ SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  যদি (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (প্ৰবেশ_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
স্থির পূর্ণাঙ্ক
double_to_human (long দ্ৱিগুণ val, পূর্ণাঙ্ক precision,
                 ৱর্ণ *buf, idx_t buf_size,
                 গণনা scale_type scale, পূর্ণাঙ্ক group, গণনা round_type round)
{
  ৱর্ণ fmt[পরিমাণ "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  ৱর্ণ *pfmt = fmt;
  *pfmt++ = '%';

  যদি (group)
    *pfmt++ = '\'';

  যদি (zero_padding_width)
    pfmt += sprintf (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  যদি (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      ঘূৰাই snprintf (buf, buf_size, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  দ্ৱিগুণ scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  পূর্ণাঙ্ক power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  পূর্ণাঙ্ক power_adjust = 0;
  যদি (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  অন্যথা যদি (absld (val) < 10)
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
  যদি (absld (val) >= scale_base)
    {
      val /= scale_base;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  পূর্ণাঙ্ক show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  পূর্ণাঙ্ক prec = user_precision == -1 ? show_decimal_point : user_precision;

  ঘূৰাই snprintf (buf, buf_size, fmt, prec, val,
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
স্থির uintmax_t
unit_to_umax (ৱর্ণ ধ্রুৱ *n_string)
{
  strtol_error s_err;
  ৱর্ণ ধ্রুৱ *c_string = n_string;
  ৱর্ণ *t_string = রিক্ত;
  size_t n_len = strlen (n_string);
  ৱর্ণ *end = রিক্ত;
  uintmax_t n;
  ৱর্ণ ধ্রুৱ *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  যদি (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      memcpy (t_string, n_string, n_len);

      যদি (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
        *end = '\0';
      অন্যথা
        {
          *++end = 'B';
          *++end = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &end, 10, &n, suffixes);

  যদি (s_err != LONGINT_OK || *end || n == 0)
    {
      মুক্ত (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  মুক্ত (t_string);

  ঘূৰাই n;
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [সংখ্যা_2]...\n\
"), program_name);
      fputs (_("\
Reformat সংখ্যা_2(s), or the numbers from standard প্ৰবেশ যদি none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid প্ৰবেশ\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace বাবে field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these প্ৰবেশ fields (স্ৱভাৱ=1);\n\
         see FIELDS below বাবে details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use printf style floating-point FORMAT;\n\
         see FORMAT below বাবে details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         auto-scale প্ৰবেশ numbers to UNITs; স্ৱভাৱ is 'none';\n\
         see UNIT below বাবে details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the প্ৰবেশ unit size (instead of the স্ৱভাৱ 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header সাৰিগুলি;\n\
         N defaults to 1 যদি not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode বাবে invalid numbers;\n\
         MODE can be: abort (স্ৱভাৱ), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will ডান-align, negative N will বাম-align;\n\
         padding is ignored যদি the output is wider than N;\n\
         the স্ৱভাৱ is to automatically pad যদি a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD বাবে rounding when scaling; METHOD can be:\n\
         up, down, from-zero (স্ৱভাৱ), towards-zero, nearest\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         add SUFFIX to output numbers,\n\
         and accept an optional SUFFIX in প্ৰবেশ numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         ঢোকাও SEP between সংখ্যা and unit on output,\n\
         and accept an optional SEP in প্ৰবেশ numbers\n\
"));
      oputs (_("\
      --to=UNIT\n\
         auto-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit size (instead of the স্ৱভাৱ 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         সাৰি delimiter is NUL, not newline\n\
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
  N-   from N'th field, to end of সাৰি\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable বাবে printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (যদি supported by current locale).\n\
Optional width মান (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the সংখ্যা. Optional negative মানসমূহ (%-10f) will বাম align.\n\
Optional precision (%.1f) will override the প্ৰবেশ determined precision.\n\
"), stdout);

      printf (_("\n\
Exit status is 0 যদি all প্ৰবেশ numbers were successfully converted.\n\
By স্ৱভাৱ, %s will stop at the first conversion error with exit status 2.\n\
With --invalid='fail' a warning is printed বাবে each conversion error\n\
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
স্থির শূন্য
parse_format_string (ৱর্ণ ধ্রুৱ *fmt)
{
  size_t i;
  size_t prefix_len = 0;
  size_t suffix_pos;
  ৱর্ণ *endptr = রিক্ত;
  bool zero_padding = false;

  বাবে (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      যদি (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  যেতিয়ালৈ (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      যদি (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      অন্যথা যদি (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      অন্যথা যদি (! skip)
        ভঙ্গ;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  যদি (pad != 0)
    {
      যদি (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      যদি (pad < 0)
        padding_width = pad;
      অন্যথা
        {
          যদি (zero_padding)
            zero_padding_width = MIN (pad, INT_MAX);
          অন্যথা
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  যদি (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  যদি (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      যদি (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  যদি (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  বাবে (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    যদি (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  যদি (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  যদি (fmt[suffix_pos] != '\0')
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
স্থির গণনা simple_strtod_error
parse_human_সংখ্যা (ৱর্ণ ধ্রুৱ *str, long দ্ৱিগুণ /*output */ *মান,
                    size_t *precision)
{
  ৱর্ণ *ptr = রিক্ত;

  গণনা simple_strtod_error e =
    simple_strtod_human (str, &ptr, মান, precision, scale_from);
  যদি (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      ঘূৰাই e;
    }

  যদি (ptr && *ptr != '\0')
    {
      যদি (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  ঘূৰাই e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
স্থির bool
prepare_padded_সংখ্যা (ধ্রুৱ long দ্ৱিগুণ val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  পূর্ণাঙ্ক x;
  expld (val, 10, &x);

  যদি (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      যদি (inval_style != inval_ignore)
        {
          যদি (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          অন্যথা
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      ঘূৰাই false;
    }

  যদি (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      যদি (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      ঘূৰাই false;
    }

  যেতিয়ালৈ (true)
    {
      পূর্ণাঙ্ক numlen = double_to_human (val, precision_used,
                                    padding_buffer, padding_buffer_size,
                                    scale_to, grouping, round_style);
      ptrdiff_t growth;
      যদি (numlen < 0 || ckd_sub (&growth, numlen, padding_buffer_size - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      যদি (growth <= 0)
        ভঙ্গ;
      padding_buffer = xpalloc (padding_buffer, &padding_buffer_size,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_buffer));

  intmax_t pad = 0;
  যদি (padding_width)
    {
      পূর্ণাঙ্ক buf_width = mbswidth (padding_buffer,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      যদি (0 <= buf_width)
        {
          যদি (padding_width < 0)
            {
              যদি (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          অন্যথা
            {
              যদি (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  ঘূৰাই true;
}

স্থির শূন্য
print_padded_সংখ্যা (intmax_t padding)
{
  যদি (format_str_prefix)
    fputs (format_str_prefix, stdout);

  বাবে (intmax_t p = padding; 0 < p; p--)
    putchar (' ');

  fputs (padding_buffer, stdout);

  বাবে (intmax_t p = padding; p < 0; p++)
    putchar (' ');

  যদি (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
স্থির bool
process_suffixed_সংখ্যা (ৱর্ণ *text, long দ্ৱিগুণ *result,
                         size_t *precision, long পূর্ণাঙ্ক field)
{
  ৱর্ণ saved_suffix = '\0';

  যদি (suffix)
    {
      যদি (mbs_endswith (text, suffix))
        {
          saved_suffix = *(text + strlen (text) - strlen (suffix));
          *(text + strlen (text) - strlen (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      অন্যথা
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  ৱর্ণ *p = skip_str_matching (text, newline_or_blank, true);

  /* setup auto-padding.  */
  যদি (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      যদি (padding_width < 0)
        padding_width = strlen (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  long দ্ৱিগুণ val = 0;
  গণনা simple_strtod_error e = parse_human_সংখ্যা (p, &val, precision);
  যদি (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  যদি (from_unit_size != 1 || to_unit_size != 1)
    val = (val * from_unit_size) / to_unit_size;

  *result = val;

  যদি (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    ঘূৰাই true;
  অন্যথা
    {
      যদি (saved_suffix)
        *(text + strlen (text)) = saved_suffix;
      ঘূৰাই false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
স্থির ৱর্ণ *
mbsmbchr (ৱর্ণ ধ্রুৱ *s, ৱর্ণ ধ্রুৱ *c)
{
  unsigned ৱর্ণ uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  যদি (uc < 0x30 || MB_CUR_MAX == 1)
    ঘূৰাই (ৱর্ণ *) strchr (s, uc);
  অন্যথা যদি (is_utf8_charset ())
    ঘূৰাই (ৱর্ণ *) (uc < 0x80 ? strchr (s, uc) : strstr (s, c));
  অন্যথা
    ঘূৰাই *(c + 1) == '\0' ? mbschr (s, uc) : (ৱর্ণ *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
স্থির ৱর্ণ *
next_field (ৱর্ণ **সাৰি)
{
  ৱর্ণ *field_start = *সাৰি;
  ৱর্ণ *field_end   = field_start;

  যদি (delimiter)
    {
      যদি (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = strchr (field_start, '\0');
    }
  অন্যথা
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *সাৰি = field_end;
  ঘূৰাই field_start;
}

ATTRIBUTE_PURE
স্থির bool
include_field (uintmax_t field)
{
  সংরচনা field_range_pair *p = frp;
  যদি (!p)
    ঘূৰাই field == 1;

  যেতিয়ালৈ (p->lo != UINTMAX_MAX)
    {
      যদি (p->lo <= field && p->hi >= field)
        ঘূৰাই true;
      ++p;
    }
  ঘূৰাই false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
স্থির bool
process_field (ৱর্ণ *text, uintmax_t field)
{
  long দ্ৱিগুণ val = 0;
  size_t precision = 0;
  bool valid_সংখ্যা = true;

  যদি (include_field (field))
    {
      valid_সংখ্যা =
        process_suffixed_সংখ্যা (text, &val, &precision, field);

      intmax_t padding;
      যদি (valid_সংখ্যা)
        valid_সংখ্যা = prepare_padded_সংখ্যা (val, precision, &padding);

      যদি (valid_সংখ্যা)
        print_padded_সংখ্যা (padding);
      অন্যথা
        fputs (text, stdout);
    }
  অন্যথা
    fputs (text, stdout);

  ঘূৰাই valid_সংখ্যা;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
স্থির পূর্ণাঙ্ক
process_সাৰি (ৱর্ণ *সাৰি, bool newline)
{
  ৱর্ণ *next;
  uintmax_t field = 0;
  bool valid_সংখ্যা = true;

  যেতিয়ালৈ (true) {
    ++field;
    next = next_field (&সাৰি);

    যদি (*সাৰি != '\0')
      {
        /* NUL terminate the current field string and process */
        ৱর্ণ end_field = *সাৰি;
        *সাৰি = '\0';

        যদি (! process_field (next, field))
          valid_সংখ্যা = false;

        যদি (delimiter != রিক্ত)
          fputs (delimiter, stdout);
        অন্যথা
          fputc (' ', stdout);

        যদি (delimiter)
          সাৰি += MAX (strlen (delimiter), 1);
        অন্যথা
          {
            *সাৰি = end_field;
            mcel_t g = mcel_scanz (সাৰি);
            সাৰি += g.len;
          }
      }
    অন্যথা
      {
        /* end of the line, process the last field and finish */
        যদি (! process_field (next, field))
          valid_সংখ্যা = false;

        ভঙ্গ;
      }
  }

  যদি (newline)
    putchar (সাৰি_delim);

 যদি (ferror (stdout))
   write_error ();

  ঘূৰাই valid_সংখ্যা;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  পূর্ণাঙ্ক valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#যদি HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#endif

  decimal_point = nl_langinfo (RADIXCHAR);
  যদি (decimal_point == রিক্ত || strlen (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_length = strlen (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  যদি (thousands_sep == রিক্ত)
    thousands_sep = "";
  thousands_sep_length = strlen (thousands_sep);

  atexit (close_stdout);

  যেতিয়ালৈ (true)
    {
      পূর্ণাঙ্ক c = getopt_long (argc, argv, "d:z", longopts, রিক্ত);

      যদি (c == -1)
        ভঙ্গ;

      ৱিকল্প (c)
        {
        পক্ষ FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          ভঙ্গ;

        পক্ষ FROM_UNIT_OPTION:
          from_unit_size = unit_to_umax (optarg);
          ভঙ্গ;

        পক্ষ TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          ভঙ্গ;

        পক্ষ TO_UNIT_OPTION:
          to_unit_size = unit_to_umax (optarg);
          ভঙ্গ;

        পক্ষ ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          ভঙ্গ;

        পক্ষ GROUPING_OPTION:
          grouping = 1;
          ভঙ্গ;

        পক্ষ PADDING_OPTION:
          যদি (((xstrtoimax (optarg, রিক্ত, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          ভঙ্গ;

        পক্ষ FIELD_OPTION:
          যদি (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          ভঙ্গ;

        পক্ষ 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          যদি (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              যদি (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          ভঙ্গ;

        পক্ষ 'z':
          সাৰি_delim = '\0';
          ভঙ্গ;

        পক্ষ SUFFIX_OPTION:
          suffix = optarg;
          ভঙ্গ;

        পক্ষ UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          ভঙ্গ;

        পক্ষ DEBUG_OPTION:
          debug = true;
          ভঙ্গ;

        পক্ষ DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          ভঙ্গ;

        পক্ষ HEADER_OPTION:
          যদি (optarg)
            {
              যদি (xstrtoumax (optarg, রিক্ত, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          অন্যথা
            {
              header = 1;
            }
          ভঙ্গ;

        পক্ষ FORMAT_OPTION:
          format_str = optarg;
          ভঙ্গ;

        পক্ষ INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          ভঙ্গ;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  যদি (format_str != রিক্ত && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  যদি (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  যদি (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == রিক্ত))
    error (0, 0, _("no conversion option specified"));

  যদি (debug && unit_separator && delimiter == রিক্ত)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  যদি (format_str)
    parse_format_string (format_str);

  যদি (grouping)
    {
      যদি (scale_to != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      যদি (debug && thousands_sep_length == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == রিক্ত);

  যদি (inval_style != inval_abort)
    conv_exit_code = 0;

  যদি (argc > optind)
    {
      যদি (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      বাবে (; optind < argc; optind++)
        valid_numbers &= process_সাৰি (argv[optind], true);
    }
  অন্যথা
    {
      ৱর্ণ *সাৰি = রিক্ত;
      size_t সাৰি_allocated = 0;
      ssize_t len;

      যেতিয়ালৈ (header-- && getdelim (&সাৰি, &সাৰি_allocated,
                                   সাৰি_delim, stdin) > 0)
        {
          যদি (fputs (সাৰি, stdout) == EOF)
            write_error ();
        }

      যেতিয়ালৈ ((len = getdelim (&সাৰি, &সাৰি_allocated,
                              সাৰি_delim, stdin)) > 0)
        {
          bool newline = সাৰি[len - 1] == সাৰি_delim;
          যদি (newline)
            সাৰি[len - 1] = '\0';
          valid_numbers &= process_সাৰি (সাৰি, newline);
        }

      যদি (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  যদি (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  পূর্ণাঙ্ক exit_status = EXIT_SUCCESS;
  যদি (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
