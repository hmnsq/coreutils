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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <float.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <langinfo.h>

#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "mbswidth.h"
#অন্তর্ভাব "mcel.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "skipchars.h"
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "utf8.h"
#অন্তর্ভাব "xstrtol.h"

#অন্তর্ভাব "set-fields.h"

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

স্থির অক্ষর ধ্রুবক *ধ্রুবক scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", ফাঁকা
};

স্থির গণনা scale_type ধ্রুবক scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

স্থির অক্ষর ধ্রুবক *ধ্রুবক scale_to_args[] =
{
  "none", "si", "iec", "iec-i", ফাঁকা
};

স্থির গণনা scale_type ধ্রুবক scale_to_types[] =
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

স্থির অক্ষর ধ্রুবক *ধ্রুবক round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", ফাঁকা
};

স্থির গণনা round_type ধ্রুবক round_types[] =
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

স্থির অক্ষর ধ্রুবক *ধ্রুবক inval_args[] =
{
  "abort", "fail", "warn", "ignore", ফাঁকা
};

স্থির গণনা inval_type ধ্রুবক inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"from", required_argument, ফাঁকা, FROM_OPTION},
  {"from-unit", required_argument, ফাঁকা, FROM_UNIT_OPTION},
  {"to", required_argument, ফাঁকা, TO_OPTION},
  {"to-unit", required_argument, ফাঁকা, TO_UNIT_OPTION},
  {"round", required_argument, ফাঁকা, ROUND_OPTION},
  {"padding", required_argument, ফাঁকা, PADDING_OPTION},
  {"suffix", required_argument, ফাঁকা, SUFFIX_OPTION},
  {"unit-separator", required_argument, ফাঁকা, UNIT_SEPARATOR_OPTION},
  {"grouping", no_argument, ফাঁকা, GROUPING_OPTION},
  {"delimiter", required_argument, ফাঁকা, 'd'},
  {"field", required_argument, ফাঁকা, FIELD_OPTION},
  {"debug", no_argument, ফাঁকা, DEBUG_OPTION},
  {"-debug", no_argument, ফাঁকা, DEV_DEBUG_OPTION},
  {"header", optional_argument, ফাঁকা, HEADER_OPTION},
  {"format", required_argument, ফাঁকা, FORMAT_OPTION},
  {"invalid", required_argument, ফাঁকা, INVALID_OPTION},
  {"zero-terminated", no_argument, ফাঁকা, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
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
স্থির অক্ষর ধ্রুবক *suffix = ফাঁকা;
স্থির অক্ষর ধ্রুবক *unit_separator = ফাঁকা;
স্থির uintmax_t from_unit_আকার = 1;
স্থির uintmax_t to_unit_আকার = 1;
স্থির পূর্ণসংখ্যা grouping = 0;
স্থির অক্ষর *padding_বাফার = ফাঁকা;
স্থির idx_t padding_বাফার_আকার = 0;
স্থির intmax_t padding_width = 0;
স্থির পূর্ণসংখ্যা zero_padding_width = 0;
স্থির long পূর্ণসংখ্যা user_precision = -1;
স্থির অক্ষর ধ্রুবক *format_str = ফাঁকা;
স্থির অক্ষর *format_str_prefix = ফাঁকা;
স্থির অক্ষর *format_str_suffix = ফাঁকা;

/* By default, any conversion error will terminate the program.  */
স্থির পূর্ণসংখ্যা conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
স্থির পূর্ণসংখ্যা auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
স্থির অক্ষর ধ্রুবক *delimiter = ফাঁকা;

/* line delimiter.  */
স্থির unsigned অক্ষর সারি_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
স্থির uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
স্থির bool debug;

/* will be set according to the current locale.  */
স্থির অক্ষর ধ্রুবক *decimal_point;
স্থির পূর্ণসংখ্যা decimal_point_দৈর্ঘ্য;
স্থির অক্ষর ধ্রুবক *thousands_sep;
স্থির পূর্ণসংখ্যা thousands_sep_দৈর্ঘ্য;

/* debugging for developers.  Enables devmsg().  */
স্থির bool dev_debug = false;

স্থির bool
newline_or_blank (mcel_t g)
{
  ফেরত g.ch == '\n' || c32issep (g.ch);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
default_scale_base (গণনা scale_type scale)
{
  নির্বাচন (scale)
    {
    ক্ষেত্র scale_IEC:
    ক্ষেত্র scale_IEC_I:
      ফেরত 1024;

    ক্ষেত্র scale_none:
    ক্ষেত্র scale_auto:
    ক্ষেত্র scale_SI:
    পূর্বনির্ধারিত:
      ফেরত 1000;
    }
}

স্থির অক্ষর ধ্রুবক zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
স্থির অক্ষর ধ্রুবক *valid_suffixes = 1 + zero_and_valid_suffixes;

স্থির অন্তঃরেখ bool
valid_suffix (ধ্রুবক অক্ষর suf)
{
  ফেরত strchr (valid_suffixes, suf) != ফাঁকা;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
suffix_power (ধ্রুবক অক্ষর suf)
{
  নির্বাচন (suf)
    {
    ক্ষেত্র 'k':                  /* kilo.  */
    ক্ষেত্র 'K':                  /* kilo or kibi.  */
      ফেরত 1;

    ক্ষেত্র 'M':                  /* mega or mebi.  */
      ফেরত 2;

    ক্ষেত্র 'G':                  /* giga or gibi.  */
      ফেরত 3;

    ক্ষেত্র 'T':                  /* tera or tebi.  */
      ফেরত 4;

    ক্ষেত্র 'P':                  /* peta or pebi.  */
      ফেরত 5;

    ক্ষেত্র 'E':                  /* exa or exbi.  */
      ফেরত 6;

    ক্ষেত্র 'Z':                  /* zetta or 2**70.  */
      ফেরত 7;

    ক্ষেত্র 'Y':                  /* yotta or 2**80.  */
      ফেরত 8;

    ক্ষেত্র 'R':                  /* ronna or 2**90.  */
      ফেরত 9;

    ক্ষেত্র 'Q':                  /* quetta or 2**100.  */
      ফেরত 10;

    পূর্বনির্ধারিত:                   /* should never happen. assert?  */
      ফেরত 0;
    }
}

স্থির অন্তঃরেখ অক্ষর ধ্রুবক *
suffix_power_char (পূর্ণসংখ্যা power)
{
  নির্বাচন (power)
    {
    ক্ষেত্র 0:
      ফেরত "";

    ক্ষেত্র 1:
      ফেরত "K";

    ক্ষেত্র 2:
      ফেরত "M";

    ক্ষেত্র 3:
      ফেরত "G";

    ক্ষেত্র 4:
      ফেরত "T";

    ক্ষেত্র 5:
      ফেরত "P";

    ক্ষেত্র 6:
      ফেরত "E";

    ক্ষেত্র 7:
      ফেরত "Z";

    ক্ষেত্র 8:
      ফেরত "Y";

    ক্ষেত্র 9:
      ফেরত "R";

    ক্ষেত্র 10:
      ফেরত "Q";

    পূর্বনির্ধারিত:
      ফেরত "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
স্থির long দ্বিগুণ
powerld (long দ্বিগুণ base, ptrdiff_t x)
{
  long দ্বিগুণ ফলাফল = base;
  যদি (x == 0)
    ফেরত 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  যখন (--x)
    ফলাফল *= base;
  ফেরত ফলাফল;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
স্থির অন্তঃরেখ long দ্বিগুণ
absld (long দ্বিগুণ val)
{
  ফেরত val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
স্থির long দ্বিগুণ
expld (long দ্বিগুণ val, পূর্ণসংখ্যা base, পূর্ণসংখ্যা /*output */ *x)
{
  পূর্ণসংখ্যা power = 0;

  যদি (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      যখন (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  যদি (x)
    *x = power;
  ফেরত val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
স্থির অন্তঃরেখ intmax_t
simple_round_ceiling (long দ্বিগুণ val)
{
  intmax_t intval = val;
  যদি (intval < val)
    intval++;
  ফেরত intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
স্থির অন্তঃরেখ intmax_t
simple_round_floor (long দ্বিগুণ val)
{
  ফেরত -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
স্থির অন্তঃরেখ intmax_t
simple_round_from_zero (long দ্বিগুণ val)
{
  ফেরত val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
স্থির অন্তঃরেখ intmax_t
simple_round_to_zero (long দ্বিগুণ val)
{
  ফেরত val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
স্থির অন্তঃরেখ intmax_t
simple_round_nearest (long দ্বিগুণ val)
{
  ফেরত val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
স্থির অন্তঃরেখ long দ্বিগুণ
simple_round (long দ্বিগুণ val, গণনা round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (long দ্বিগুণ) INTMAX_MAX * intmax_mul;

  নির্বাচন (t)
    {
    ক্ষেত্র round_ceiling:
      rval = simple_round_ceiling (val);
      বিরতি;

    ক্ষেত্র round_floor:
      rval = simple_round_floor (val);
      বিরতি;

    ক্ষেত্র round_from_zero:
      rval = simple_round_from_zero (val);
      বিরতি;

    ক্ষেত্র round_to_zero:
      rval = simple_round_to_zero (val);
      বিরতি;

    ক্ষেত্র round_nearest:
      rval = simple_round_nearest (val);
      বিরতি;

    পূর্বনির্ধারিত:
      /* to silence the compiler - this should never happen.  */
      ফেরত 0;
    }

  ফেরত (long দ্বিগুণ) INTMAX_MAX * intmax_mul + rval;
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
simple_strtod_int (অক্ষর ধ্রুবক *প্রবেশ_str,
                   অক্ষর **endptr, long দ্বিগুণ *মান, bool *negative)
{
  গণনা simple_strtod_error e = SSE_OK;

  long দ্বিগুণ val = 0;
  পূর্ণসংখ্যা digits = 0;
  bool found_digit = false;

  যদি (*প্রবেশ_str == '-')
    {
      প্রবেশ_str++;
      *negative = true;
    }
  অন্যথা
    *negative = false;

  *endptr = (অক্ষর *) প্রবেশ_str;
  যখন (c_isdigit (**endptr))
    {
      পূর্ণসংখ্যা digit = (**endptr) - '0';

      found_digit = true;

      যদি (val || digit)
        digits++;

      যদি (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      যদি (digits > MAX_ACCEPTABLE_DIGITS)
        ফেরত SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      যদি (thousands_sep_দৈর্ঘ্য > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_দৈর্ঘ্য)
          && c_isdigit ((*endptr)[thousands_sep_দৈর্ঘ্য]))
        (*endptr) += thousands_sep_দৈর্ঘ্য;
    }
  যদি (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_দৈর্ঘ্য))
    ফেরত SSE_INVALID_সংখ্যা;
  যদি (*negative)
    val = -val;

  যদি (মান)
    *মান = val;

  ফেরত e;
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
simple_strtod_float (অক্ষর ধ্রুবক *প্রবেশ_str,
                     অক্ষর **endptr,
                     long দ্বিগুণ *মান,
                     size_t *precision)
{
  bool negative;
  গণনা simple_strtod_error e = SSE_OK;

  যদি (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (প্রবেশ_str, endptr, মান, &negative);
  যদি (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ফেরত e;

  /* optional decimal point + fraction.  */
  যদি (STREQ_LEN (*endptr, decimal_point, decimal_point_দৈর্ঘ্য))
    {
      অক্ষর *ptr2;
      long দ্বিগুণ val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_দৈর্ঘ্য;
      গণনা simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      যদি (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        ফেরত e2;
      যদি (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      যদি (neg_frac)
        ফেরত SSE_INVALID_সংখ্যা;

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
  ফেরত e;
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
simple_strtod_human (অক্ষর ধ্রুবক *প্রবেশ_str,
                     অক্ষর **endptr, long দ্বিগুণ *মান, size_t *precision,
                     গণনা scale_type allowed_scaling)
{
  পূর্ণসংখ্যা power = 0;
  /* 'scale_auto' is checked below.  */
  পূর্ণসংখ্যা scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, প্রবেশ_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  গণনা simple_strtod_error e =
    simple_strtod_float (প্রবেশ_str, endptr, মান, precision);
  যদি (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ফেরত e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *মান, (পূর্ণসংখ্যা)*precision);

  যখন (**endptr)
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
        বিরতি;  /* Treat as no suffix.  */

      যদি (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          যদি (**endptr == '\0')
            বিরতি;

          ফেরত SSE_INVALID_SUFFIX;
        }

      যদি (allowed_scaling == scale_none)
        ফেরত SSE_VALID_BUT_FORBIDDEN_SUFFIX;

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
            ফেরত SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      বিরতি;
    }

  long দ্বিগুণ multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*মান) = (*মান) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *মান, *মান);

  ফেরত e;
}


স্থির শূন্য
simple_strtod_fatal (গণনা simple_strtod_error err, অক্ষর ধ্রুবক *প্রবেশ_str)
{
  অক্ষর ধ্রুবক *msgid = ফাঁকা;

  নির্বাচন (err)
    {
    ক্ষেত্র SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      বিরতি;

    ক্ষেত্র SSE_INVALID_সংখ্যা:
      msgid = N_("invalid number: %s");
      বিরতি;

    ক্ষেত্র SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      বিরতি;

    ক্ষেত্র SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      বিরতি;

    ক্ষেত্র SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      বিরতি;

    ক্ষেত্র SSE_OK_PRECISION_LOSS:
    ক্ষেত্র SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  যদি (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (প্রবেশ_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
স্থির পূর্ণসংখ্যা
double_to_human (long দ্বিগুণ val, পূর্ণসংখ্যা precision,
                 অক্ষর *buf, idx_t buf_আকার,
                 গণনা scale_type scale, পূর্ণসংখ্যা group, গণনা round_type round)
{
  অক্ষর fmt[আকার "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  অক্ষর *pfmt = fmt;
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

      ফেরত snprintf (buf, buf_আকার, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  দ্বিগুণ scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  পূর্ণসংখ্যা power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  পূর্ণসংখ্যা power_adjust = 0;
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
  পূর্ণসংখ্যা show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  পূর্ণসংখ্যা prec = user_precision == -1 ? show_decimal_point : user_precision;

  ফেরত snprintf (buf, buf_আকার, fmt, prec, val,
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
unit_to_umax (অক্ষর ধ্রুবক *n_string)
{
  strtol_error s_err;
  অক্ষর ধ্রুবক *c_string = n_string;
  অক্ষর *t_string = ফাঁকা;
  size_t n_len = strlen (n_string);
  অক্ষর *end = ফাঁকা;
  uintmax_t n;
  অক্ষর ধ্রুবক *suffixes = valid_suffixes;

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

  ফেরত n;
}

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [সংখ্যা_2]...\n\
"), program_name);
      fputs (_("\
Reformat সংখ্যা_2(s), or the numbers from standard প্রবেশ যদি none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid প্রবেশ\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace জন্য field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these প্রবেশ fields (পূর্বনির্ধারিত=1);\n\
         see FIELDS below জন্য details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use printf style floating-point FORMAT;\n\
         see FORMAT below জন্য details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         auto-scale প্রবেশ numbers to UNITs; পূর্বনির্ধারিত is 'none';\n\
         see UNIT below জন্য details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the প্রবেশ unit আকার_2 (instead of the পূর্বনির্ধারিত 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header সারিগুলি;\n\
         N defaults to 1 যদি not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode জন্য invalid numbers;\n\
         MODE can be: abort (পূর্বনির্ধারিত), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will ডান-align, negative N will বাম-align;\n\
         padding is ignored যদি the output is wider than N;\n\
         the পূর্বনির্ধারিত is to automatically pad যদি a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD জন্য rounding when scaling; METHOD can be:\n\
         up, down, from-zero (পূর্বনির্ধারিত), towards-zero, nearest\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         add SUFFIX to output numbers,\n\
         and accept an optional SUFFIX in প্রবেশ numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         ঢোকাও SEP between সংখ্যা and unit on output,\n\
         and accept an optional SEP in প্রবেশ numbers\n\
"));
      oputs (_("\
      --to=UNIT\n\
         auto-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit আকার_2 (instead of the পূর্বনির্ধারিত 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         সারি delimiter is NUL, not newline\n\
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
  N-   from N'th field, to end of সারি\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable জন্য printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (যদি supported by current locale).\n\
Optional width মান (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the সংখ্যা. Optional negative মানসমূহ (%-10f) will বাম align.\n\
Optional precision (%.1f) will override the প্রবেশ determined precision.\n\
"), stdout);

      printf (_("\n\
Exit status is 0 যদি all প্রবেশ numbers were successfully converted.\n\
By পূর্বনির্ধারিত, %s will stop at the first conversion error with exit status 2.\n\
With --invalid='fail' a warning is printed জন্য each conversion error\n\
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
parse_format_string (অক্ষর ধ্রুবক *fmt)
{
  size_t i;
  size_t prefix_len = 0;
  size_t suffix_pos;
  অক্ষর *endptr = ফাঁকা;
  bool zero_padding = false;

  জন্য (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      যদি (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  যখন (true)
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
        বিরতি;
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

  জন্য (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
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
parse_human_সংখ্যা (অক্ষর ধ্রুবক *str, long দ্বিগুণ /*output */ *মান,
                    size_t *precision)
{
  অক্ষর *ptr = ফাঁকা;

  গণনা simple_strtod_error e =
    simple_strtod_human (str, &ptr, মান, precision, scale_from);
  যদি (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      ফেরত e;
    }

  যদি (ptr && *ptr != '\0')
    {
      যদি (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  ফেরত e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
স্থির bool
prepare_padded_সংখ্যা (ধ্রুবক long দ্বিগুণ val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  পূর্ণসংখ্যা x;
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
      ফেরত false;
    }

  যদি (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      যদি (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      ফেরত false;
    }

  যখন (true)
    {
      পূর্ণসংখ্যা numlen = double_to_human (val, precision_used,
                                    padding_বাফার, padding_বাফার_আকার,
                                    scale_to, grouping, round_style);
      ptrdiff_t growth;
      যদি (numlen < 0 || ckd_sub (&growth, numlen, padding_বাফার_আকার - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      যদি (growth <= 0)
        বিরতি;
      padding_বাফার = xpalloc (padding_বাফার, &padding_বাফার_আকার,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_বাফার));

  intmax_t pad = 0;
  যদি (padding_width)
    {
      পূর্ণসংখ্যা buf_width = mbswidth (padding_বাফার,
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
  ফেরত true;
}

স্থির শূন্য
print_padded_সংখ্যা (intmax_t padding)
{
  যদি (format_str_prefix)
    fputs (format_str_prefix, stdout);

  জন্য (intmax_t p = padding; 0 < p; p--)
    putchar (' ');

  fputs (padding_বাফার, stdout);

  জন্য (intmax_t p = padding; p < 0; p++)
    putchar (' ');

  যদি (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
স্থির bool
process_suffixed_সংখ্যা (অক্ষর *text, long দ্বিগুণ *ফলাফল,
                         size_t *precision, long পূর্ণসংখ্যা field)
{
  অক্ষর saved_suffix = '\0';

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
  অক্ষর *p = skip_str_matching (text, newline_or_blank, true);

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

  long দ্বিগুণ val = 0;
  গণনা simple_strtod_error e = parse_human_সংখ্যা (p, &val, precision);
  যদি (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  যদি (from_unit_আকার != 1 || to_unit_আকার != 1)
    val = (val * from_unit_আকার) / to_unit_আকার;

  *ফলাফল = val;

  যদি (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    ফেরত true;
  অন্যথা
    {
      যদি (saved_suffix)
        *(text + strlen (text)) = saved_suffix;
      ফেরত false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
স্থির অক্ষর *
mbsmbchr (অক্ষর ধ্রুবক *s, অক্ষর ধ্রুবক *c)
{
  unsigned অক্ষর uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  যদি (uc < 0x30 || MB_CUR_MAX == 1)
    ফেরত (অক্ষর *) strchr (s, uc);
  অন্যথা যদি (is_utf8_charset ())
    ফেরত (অক্ষর *) (uc < 0x80 ? strchr (s, uc) : strstr (s, c));
  অন্যথা
    ফেরত *(c + 1) == '\0' ? mbschr (s, uc) : (অক্ষর *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
স্থির অক্ষর *
পরবর্তী_field (অক্ষর **সারি)
{
  অক্ষর *field_start = *সারি;
  অক্ষর *field_end   = field_start;

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

  *সারি = field_end;
  ফেরত field_start;
}

ATTRIBUTE_PURE
স্থির bool
include_field (uintmax_t field)
{
  কাঠামো field_range_pair *p = frp;
  যদি (!p)
    ফেরত field == 1;

  যখন (p->lo != UINTMAX_MAX)
    {
      যদি (p->lo <= field && p->hi >= field)
        ফেরত true;
      ++p;
    }
  ফেরত false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
স্থির bool
process_field (অক্ষর *text, uintmax_t field)
{
  long দ্বিগুণ val = 0;
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

  ফেরত valid_সংখ্যা;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
স্থির পূর্ণসংখ্যা
process_সারি (অক্ষর *সারি, bool newline)
{
  অক্ষর *পরবর্তী;
  uintmax_t field = 0;
  bool valid_সংখ্যা = true;

  যখন (true) {
    ++field;
    পরবর্তী = পরবর্তী_field (&সারি);

    যদি (*সারি != '\0')
      {
        /* NUL terminate the current field string and process */
        অক্ষর end_field = *সারি;
        *সারি = '\0';

        যদি (! process_field (পরবর্তী, field))
          valid_সংখ্যা = false;

        যদি (delimiter != ফাঁকা)
          fputs (delimiter, stdout);
        অন্যথা
          fputc (' ', stdout);

        যদি (delimiter)
          সারি += MAX (strlen (delimiter), 1);
        অন্যথা
          {
            *সারি = end_field;
            mcel_t g = mcel_scanz (সারি);
            সারি += g.len;
          }
      }
    অন্যথা
      {
        /* end of the line, process the last field and finish */
        যদি (! process_field (পরবর্তী, field))
          valid_সংখ্যা = false;

        বিরতি;
      }
  }

  যদি (newline)
    putchar (সারি_delim);

 যদি (ferror (stdout))
   write_error ();

  ফেরত valid_সংখ্যা;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা valid_numbers = 1;
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
  যদি (decimal_point == ফাঁকা || strlen (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_দৈর্ঘ্য = strlen (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  যদি (thousands_sep == ফাঁকা)
    thousands_sep = "";
  thousands_sep_দৈর্ঘ্য = strlen (thousands_sep);

  atexit (close_stdout);

  যখন (true)
    {
      পূর্ণসংখ্যা c = getopt_long (argc, argv, "d:z", longopts, ফাঁকা);

      যদি (c == -1)
        বিরতি;

      নির্বাচন (c)
        {
        ক্ষেত্র FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          বিরতি;

        ক্ষেত্র FROM_UNIT_OPTION:
          from_unit_আকার = unit_to_umax (optarg);
          বিরতি;

        ক্ষেত্র TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          বিরতি;

        ক্ষেত্র TO_UNIT_OPTION:
          to_unit_আকার = unit_to_umax (optarg);
          বিরতি;

        ক্ষেত্র ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          বিরতি;

        ক্ষেত্র GROUPING_OPTION:
          grouping = 1;
          বিরতি;

        ক্ষেত্র PADDING_OPTION:
          যদি (((xstrtoimax (optarg, ফাঁকা, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          বিরতি;

        ক্ষেত্র FIELD_OPTION:
          যদি (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          বিরতি;

        ক্ষেত্র 'd':
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
          বিরতি;

        ক্ষেত্র 'z':
          সারি_delim = '\0';
          বিরতি;

        ক্ষেত্র SUFFIX_OPTION:
          suffix = optarg;
          বিরতি;

        ক্ষেত্র UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          বিরতি;

        ক্ষেত্র DEBUG_OPTION:
          debug = true;
          বিরতি;

        ক্ষেত্র DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          বিরতি;

        ক্ষেত্র HEADER_OPTION:
          যদি (optarg)
            {
              যদি (xstrtoumax (optarg, ফাঁকা, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          অন্যথা
            {
              header = 1;
            }
          বিরতি;

        ক্ষেত্র FORMAT_OPTION:
          format_str = optarg;
          বিরতি;

        ক্ষেত্র INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          বিরতি;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (format_str != ফাঁকা && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  যদি (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  যদি (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == ফাঁকা))
    error (0, 0, _("no conversion option specified"));

  যদি (debug && unit_separator && delimiter == ফাঁকা)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  যদি (format_str)
    parse_format_string (format_str);

  যদি (grouping)
    {
      যদি (scale_to != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      যদি (debug && thousands_sep_দৈর্ঘ্য == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == ফাঁকা);

  যদি (inval_style != inval_abort)
    conv_exit_code = 0;

  যদি (argc > optind)
    {
      যদি (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      জন্য (; optind < argc; optind++)
        valid_numbers &= process_সারি (argv[optind], true);
    }
  অন্যথা
    {
      অক্ষর *সারি = ফাঁকা;
      size_t সারি_allocated = 0;
      ssize_t len;

      যখন (header-- && getdelim (&সারি, &সারি_allocated,
                                   সারি_delim, stdin) > 0)
        {
          যদি (fputs (সারি, stdout) == EOF)
            write_error ();
        }

      যখন ((len = getdelim (&সারি, &সারি_allocated,
                              সারি_delim, stdin)) > 0)
        {
          bool newline = সারি[len - 1] == সারি_delim;
          যদি (newline)
            সারি[len - 1] = '\0';
          valid_numbers &= process_সারি (সারি, newline);
        }

      যদি (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  যদি (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  পূর্ণসংখ্যা exit_status = EXIT_SUCCESS;
  যদি (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
