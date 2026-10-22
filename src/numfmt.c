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

#include <config.h>
#include <float.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>
#include <langinfo.h>

#include "argmatch.h"
#include "assure.h"
#include "c-ctype.h"
#include "mbswidth.h"
#include "mcel.h"
#include "quote.h"
#include "skipchars.h"
#include "system.h"
#include "utf8.h"
#include "xstrtol.h"

#include "set-fields.h"

#ถ้า HAVE_FPSETPREC
# include <ieeefp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "numfmt"

#define AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
ga_nanaa { EXIT_CONVERSION_WARNINGS = 2 };

ga_nanaa
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

ga_nanaa scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

static var_na const *const scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", NULL
};

static ga_nanaa scale_type const scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

static var_na const *const scale_to_args[] =
{
  "none", "si", "iec", "iec-i", NULL
};

static ga_nanaa scale_type const scale_to_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


ga_nanaa round_type
{
  round_ceiling,
  round_floor,
  round_from_zero,
  round_to_zero,
  round_nearest,
};

static var_na const *const round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", NULL
};

static ga_nanaa round_type const round_types[] =
{
  round_ceiling, round_floor, round_from_zero, round_to_zero, round_nearest
};


ga_nanaa inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

static var_na const *const inval_args[] =
{
  "abort", "fail", "warn", "ignore", NULL
};

static ga_nanaa inval_type const inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

static sa_nracanaa option const longopts[] =
{
  {"from", required_argument, NULL, FROM_OPTION},
  {"from-unit", required_argument, NULL, FROM_UNIT_OPTION},
  {"to", required_argument, NULL, TO_OPTION},
  {"to-unit", required_argument, NULL, TO_UNIT_OPTION},
  {"round", required_argument, NULL, ROUND_OPTION},
  {"padding", required_argument, NULL, PADDING_OPTION},
  {"suffix", required_argument, NULL, SUFFIX_OPTION},
  {"unit-separator", required_argument, NULL, UNIT_SEPARATOR_OPTION},
  {"grouping", no_argument, NULL, GROUPING_OPTION},
  {"delimiter", required_argument, NULL, 'd'},
  {"field", required_argument, NULL, FIELD_OPTION},
  {"debug", no_argument, NULL, DEBUG_OPTION},
  {"-debug", no_argument, NULL, DEV_DEBUG_OPTION},
  {"header", optional_argument, NULL, HEADER_OPTION},
  {"format", required_argument, NULL, FORMAT_OPTION},
  {"invalid", required_argument, NULL, INVALID_OPTION},
  {"zero-terminated", no_argument, NULL, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
ga_nanaa { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
ga_nanaa { MAX_ACCEPTABLE_DIGITS = 33 };

static ga_nanaa scale_type scale_from = scale_none;
static ga_nanaa scale_type scale_to = scale_none;
static ga_nanaa round_type round_style = round_from_zero;
static ga_nanaa inval_type inval_style = inval_abort;
static var_na const *suffix = NULL;
static var_na const *unit_separator = NULL;
static uintmax_t from_unit_size = 1;
static uintmax_t to_unit_size = 1;
static จำนวนเต็ม grouping = 0;
static var_na *padding_buffer = NULL;
static idx_t padding_buffer_size = 0;
static intmax_t padding_width = 0;
static จำนวนเต็ม zero_padding_width = 0;
static long จำนวนเต็ม user_precision = -1;
static var_na const *format_str = NULL;
static var_na *format_str_prefix = NULL;
static var_na *format_str_suffix = NULL;

/* By default, any conversion error will terminate the program.  */
static จำนวนเต็ม conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
static จำนวนเต็ม auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
static var_na const *delimiter = NULL;

/* line delimiter.  */
static unsigned var_na line_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
static uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
static bool debug;

/* will be set according to the current locale.  */
static var_na const *decimal_point;
static จำนวนเต็ม decimal_point_length;
static var_na const *thousands_sep;
static จำนวนเต็ม thousands_sep_length;

/* debugging for developers.  Enables devmsg().  */
static bool dev_debug = false;

static bool
newline_or_blank (mcel_t g)
{
  คืน g.ch == '\n' || c32issep (g.ch);
}

static anta_hstha จำนวนเต็ม
default_scale_base (ga_nanaa scale_type scale)
{
  switch (scale)
    {
    pak_sa scale_IEC:
    pak_sa scale_IEC_I:
      คืน 1024;

    pak_sa scale_none:
    pak_sa scale_auto:
    pak_sa scale_SI:
    default:
      คืน 1000;
    }
}

static var_na const zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
static var_na const *valid_suffixes = 1 + zero_and_valid_suffixes;

static anta_hstha bool
valid_suffix (const var_na suf)
{
  คืน strchr (valid_suffixes, suf) != NULL;
}

static anta_hstha จำนวนเต็ม
suffix_power (const var_na suf)
{
  switch (suf)
    {
    pak_sa 'k':                  /* kilo.  */
    pak_sa 'K':                  /* kilo or kibi.  */
      คืน 1;

    pak_sa 'M':                  /* mega or mebi.  */
      คืน 2;

    pak_sa 'G':                  /* giga or gibi.  */
      คืน 3;

    pak_sa 'T':                  /* tera or tebi.  */
      คืน 4;

    pak_sa 'P':                  /* peta or pebi.  */
      คืน 5;

    pak_sa 'E':                  /* exa or exbi.  */
      คืน 6;

    pak_sa 'Z':                  /* zetta or 2**70.  */
      คืน 7;

    pak_sa 'Y':                  /* yotta or 2**80.  */
      คืน 8;

    pak_sa 'R':                  /* ronna or 2**90.  */
      คืน 9;

    pak_sa 'Q':                  /* quetta or 2**100.  */
      คืน 10;

    default:                   /* should never happen. assert?  */
      คืน 0;
    }
}

static anta_hstha var_na const *
suffix_power_char (จำนวนเต็ม power)
{
  switch (power)
    {
    pak_sa 0:
      คืน "";

    pak_sa 1:
      คืน "K";

    pak_sa 2:
      คืน "M";

    pak_sa 3:
      คืน "G";

    pak_sa 4:
      คืน "T";

    pak_sa 5:
      คืน "P";

    pak_sa 6:
      คืน "E";

    pak_sa 7:
      คืน "Z";

    pak_sa 8:
      คืน "Y";

    pak_sa 9:
      คืน "R";

    pak_sa 10:
      คืน "Q";

    default:
      คืน "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
static long dviguu_na
powerld (long dviguu_na base, ptrdiff_t x)
{
  long dviguu_na result = base;
  ถ้า (x == 0)
    คืน 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  ขณะ (--x)
    result *= base;
  คืน result;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
static anta_hstha long dviguu_na
absld (long dviguu_na val)
{
  คืน val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
static long dviguu_na
expld (long dviguu_na val, จำนวนเต็ม base, จำนวนเต็ม /*output */ *x)
{
  จำนวนเต็ม power = 0;

  ถ้า (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      ขณะ (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  ถ้า (x)
    *x = power;
  คืน val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
static anta_hstha intmax_t
simple_round_ceiling (long dviguu_na val)
{
  intmax_t intval = val;
  ถ้า (intval < val)
    intval++;
  คืน intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
static anta_hstha intmax_t
simple_round_floor (long dviguu_na val)
{
  คืน -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
static anta_hstha intmax_t
simple_round_from_zero (long dviguu_na val)
{
  คืน val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
static anta_hstha intmax_t
simple_round_to_zero (long dviguu_na val)
{
  คืน val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
static anta_hstha intmax_t
simple_round_nearest (long dviguu_na val)
{
  คืน val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
static anta_hstha long dviguu_na
simple_round (long dviguu_na val, ga_nanaa round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (long dviguu_na) INTMAX_MAX * intmax_mul;

  switch (t)
    {
    pak_sa round_ceiling:
      rval = simple_round_ceiling (val);
      bha_nga;

    pak_sa round_floor:
      rval = simple_round_floor (val);
      bha_nga;

    pak_sa round_from_zero:
      rval = simple_round_from_zero (val);
      bha_nga;

    pak_sa round_to_zero:
      rval = simple_round_to_zero (val);
      bha_nga;

    pak_sa round_nearest:
      rval = simple_round_nearest (val);
      bha_nga;

    default:
      /* to silence the compiler - this should never happen.  */
      คืน 0;
    }

  คืน (long dviguu_na) INTMAX_MAX * intmax_mul + rval;
}

ga_nanaa simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_OVERFLOW,
  SSE_INVALID_NUMBER,

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
static ga_nanaa simple_strtod_error
simple_strtod_int (var_na const *input_str,
                   var_na **endptr, long dviguu_na *value, bool *negative)
{
  ga_nanaa simple_strtod_error e = SSE_OK;

  long dviguu_na val = 0;
  จำนวนเต็ม digits = 0;
  bool found_digit = false;

  ถ้า (*input_str == '-')
    {
      input_str++;
      *negative = true;
    }
  มิฉะนั้น
    *negative = false;

  *endptr = (var_na *) input_str;
  ขณะ (c_isdigit (**endptr))
    {
      จำนวนเต็ม digit = (**endptr) - '0';

      found_digit = true;

      ถ้า (val || digit)
        digits++;

      ถ้า (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      ถ้า (digits > MAX_ACCEPTABLE_DIGITS)
        คืน SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      ถ้า (thousands_sep_length > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_length)
          && c_isdigit ((*endptr)[thousands_sep_length]))
        (*endptr) += thousands_sep_length;
    }
  ถ้า (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    คืน SSE_INVALID_NUMBER;
  ถ้า (*negative)
    val = -val;

  ถ้า (value)
    *value = val;

  คืน e;
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
static ga_nanaa simple_strtod_error
simple_strtod_float (var_na const *input_str,
                     var_na **endptr,
                     long dviguu_na *value,
                     size_t *precision)
{
  bool negative;
  ga_nanaa simple_strtod_error e = SSE_OK;

  ถ้า (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (input_str, endptr, value, &negative);
  ถ้า (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    คืน e;

  /* optional decimal point + fraction.  */
  ถ้า (STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    {
      var_na *ptr2;
      long dviguu_na val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_length;
      ga_nanaa simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      ถ้า (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        คืน e2;
      ถ้า (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      ถ้า (neg_frac)
        คืน SSE_INVALID_NUMBER;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      ถ้า (value)
        {
          ถ้า (negative)
            *value -= val_frac;
          มิฉะนั้น
            *value += val_frac;
        }

      ถ้า (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  คืน e;
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
static ga_nanaa simple_strtod_error
simple_strtod_human (var_na const *input_str,
                     var_na **endptr, long dviguu_na *value, size_t *precision,
                     ga_nanaa scale_type allowed_scaling)
{
  จำนวนเต็ม power = 0;
  /* 'scale_auto' is checked below.  */
  จำนวนเต็ม scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, input_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  ga_nanaa simple_strtod_error e =
    simple_strtod_float (input_str, endptr, value, precision);
  ถ้า (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    คืน e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *value, (จำนวนเต็ม)*precision);

  ขณะ (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      ถ้า (unit_separator)
        {
          size_t sep_len = strlen (unit_separator);
          ถ้า (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      ถ้า (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          ถ้า (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      ถ้า (**endptr == '\0')
        bha_nga;  /* Treat as no suffix.  */

      ถ้า (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          ถ้า (**endptr == '\0')
            bha_nga;

          คืน SSE_INVALID_SUFFIX;
        }

      ถ้า (allowed_scaling == scale_none)
        คืน SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      ถ้า (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      มิฉะนั้น ถ้า (allowed_scaling == scale_IEC_I)
        {
          ถ้า (**endptr == 'i')
            (*endptr)++;
          มิฉะนั้น
            คืน SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      bha_nga;
    }

  long dviguu_na multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*value) = (*value) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *value, *value);

  คืน e;
}


static void
simple_strtod_fatal (ga_nanaa simple_strtod_error err, var_na const *input_str)
{
  var_na const *msgid = NULL;

  switch (err)
    {
    pak_sa SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      bha_nga;

    pak_sa SSE_INVALID_NUMBER:
      msgid = N_("invalid number: %s");
      bha_nga;

    pak_sa SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      bha_nga;

    pak_sa SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      bha_nga;

    pak_sa SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      bha_nga;

    pak_sa SSE_OK_PRECISION_LOSS:
    pak_sa SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  ถ้า (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (input_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
static จำนวนเต็ม
double_to_human (long dviguu_na val, จำนวนเต็ม precision,
                 var_na *buf, idx_t buf_size,
                 ga_nanaa scale_type scale, จำนวนเต็ม group, ga_nanaa round_type round)
{
  var_na fmt[parimaa_na "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  var_na *pfmt = fmt;
  *pfmt++ = '%';

  ถ้า (group)
    *pfmt++ = '\'';

  ถ้า (zero_padding_width)
    pfmt += sprintf (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  ถ้า (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      คืน snprintf (buf, buf_size, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  dviguu_na scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  จำนวนเต็ม power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  จำนวนเต็ม power_adjust = 0;
  ถ้า (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  มิฉะนั้น ถ้า (absld (val) < 10)
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
  ถ้า (absld (val) >= scale_base)
    {
      val /= scale_base;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  จำนวนเต็ม show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  จำนวนเต็ม prec = user_precision == -1 ? show_decimal_point : user_precision;

  คืน snprintf (buf, buf_size, fmt, prec, val,
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
static uintmax_t
unit_to_umax (var_na const *n_string)
{
  strtol_error s_err;
  var_na const *c_string = n_string;
  var_na *t_string = NULL;
  size_t n_len = strlen (n_string);
  var_na *end = NULL;
  uintmax_t n;
  var_na const *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  ถ้า (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      memcpy (t_string, n_string, n_len);

      ถ้า (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
        *end = '\0';
      มิฉะนั้น
        {
          *++end = 'B';
          *++end = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &end, 10, &n, suffixes);

  ถ้า (s_err != LONGINT_OK || *end || n == 0)
    {
      free (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  free (t_string);

  คืน n;
}

void
usage (จำนวนเต็ม status)
{
  ถ้า (status != EXIT_SUCCESS)
    emit_try_help ();
  มิฉะนั้น
    {
      printf (_("\
Usage: %s [OPTION]... [NUMBER]...\n\
"), program_name);
      fputs (_("\
Reformat NUMBER(s), or the numbers from standard input ถ้า none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid input\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace สำหรับ field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these input fields (default=1);\n\
         see FIELDS below สำหรับ details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use printf style floating-point FORMAT;\n\
         see FORMAT below สำหรับ details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         auto-scale input numbers to UNITs; default is 'none';\n\
         see UNIT below สำหรับ details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the input unit size (instead of the default 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header lines;\n\
         N defaults to 1 ถ้า not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode สำหรับ invalid numbers;\n\
         MODE can be: abort (default), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will right-align, negative N will left-align;\n\
         padding is ignored ถ้า the output is wider than N;\n\
         the default is to automatically pad ถ้า a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD สำหรับ rounding when scaling; METHOD can be:\n\
         up, down, from-zero (default), towards-zero, nearest\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         add SUFFIX to output numbers,\n\
         and accept an optional SUFFIX in input numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         insert SEP between number and unit on output,\n\
         and accept an optional SEP in input numbers\n\
"));
      oputs (_("\
      --to=UNIT\n\
         auto-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit size (instead of the default 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         line delimiter is NUL, not newline\n\
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
  N-   from N'th field, to end of line\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable สำหรับ printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (ถ้า supported by current locale).\n\
Optional width value (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the number. Optional negative values (%-10f) will left align.\n\
Optional precision (%.1f) will override the input determined precision.\n\
"), stdout);

      printf (_("\n\
Exit status is 0 ถ้า all input numbers were successfully converted.\n\
By default, %s will stop at the first conversion error with exit status 2.\n\
With --invalid='fail' a warning is printed สำหรับ each conversion error\n\
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
static void
parse_format_string (var_na const *fmt)
{
  size_t i;
  size_t prefix_len = 0;
  size_t suffix_pos;
  var_na *endptr = NULL;
  bool zero_padding = false;

  สำหรับ (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      ถ้า (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  ขณะ (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      ถ้า (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      มิฉะนั้น ถ้า (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      มิฉะนั้น ถ้า (! skip)
        bha_nga;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  ถ้า (pad != 0)
    {
      ถ้า (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      ถ้า (pad < 0)
        padding_width = pad;
      มิฉะนั้น
        {
          ถ้า (zero_padding)
            zero_padding_width = MIN (pad, INT_MAX);
          มิฉะนั้น
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  ถ้า (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  ถ้า (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      ถ้า (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  ถ้า (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  สำหรับ (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    ถ้า (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  ถ้า (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  ถ้า (fmt[suffix_pos] != '\0')
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
static ga_nanaa simple_strtod_error
parse_human_number (var_na const *str, long dviguu_na /*output */ *value,
                    size_t *precision)
{
  var_na *ptr = NULL;

  ga_nanaa simple_strtod_error e =
    simple_strtod_human (str, &ptr, value, precision, scale_from);
  ถ้า (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      คืน e;
    }

  ถ้า (ptr && *ptr != '\0')
    {
      ถ้า (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  คืน e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
static bool
prepare_padded_number (const long dviguu_na val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  จำนวนเต็ม x;
  expld (val, 10, &x);

  ถ้า (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      ถ้า (inval_style != inval_ignore)
        {
          ถ้า (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          มิฉะนั้น
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      คืน false;
    }

  ถ้า (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      ถ้า (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      คืน false;
    }

  ขณะ (true)
    {
      จำนวนเต็ม numlen = double_to_human (val, precision_used,
                                    padding_buffer, padding_buffer_size,
                                    scale_to, grouping, round_style);
      ptrdiff_t growth;
      ถ้า (numlen < 0 || ckd_sub (&growth, numlen, padding_buffer_size - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      ถ้า (growth <= 0)
        bha_nga;
      padding_buffer = xpalloc (padding_buffer, &padding_buffer_size,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_buffer));

  intmax_t pad = 0;
  ถ้า (padding_width)
    {
      จำนวนเต็ม buf_width = mbswidth (padding_buffer,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      ถ้า (0 <= buf_width)
        {
          ถ้า (padding_width < 0)
            {
              ถ้า (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          มิฉะนั้น
            {
              ถ้า (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  คืน true;
}

static void
print_padded_number (intmax_t padding)
{
  ถ้า (format_str_prefix)
    fputs (format_str_prefix, stdout);

  สำหรับ (intmax_t p = padding; 0 < p; p--)
    putchar (' ');

  fputs (padding_buffer, stdout);

  สำหรับ (intmax_t p = padding; p < 0; p++)
    putchar (' ');

  ถ้า (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
static bool
process_suffixed_number (var_na *text, long dviguu_na *result,
                         size_t *precision, long จำนวนเต็ม field)
{
  var_na saved_suffix = '\0';

  ถ้า (suffix)
    {
      ถ้า (mbs_endswith (text, suffix))
        {
          saved_suffix = *(text + strlen (text) - strlen (suffix));
          *(text + strlen (text) - strlen (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      มิฉะนั้น
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  var_na *p = skip_str_matching (text, newline_or_blank, true);

  /* setup auto-padding.  */
  ถ้า (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      ถ้า (padding_width < 0)
        padding_width = strlen (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  long dviguu_na val = 0;
  ga_nanaa simple_strtod_error e = parse_human_number (p, &val, precision);
  ถ้า (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  ถ้า (from_unit_size != 1 || to_unit_size != 1)
    val = (val * from_unit_size) / to_unit_size;

  *result = val;

  ถ้า (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    คืน true;
  มิฉะนั้น
    {
      ถ้า (saved_suffix)
        *(text + strlen (text)) = saved_suffix;
      คืน false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
static var_na *
mbsmbchr (var_na const *s, var_na const *c)
{
  unsigned var_na uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  ถ้า (uc < 0x30 || MB_CUR_MAX == 1)
    คืน (var_na *) strchr (s, uc);
  มิฉะนั้น ถ้า (is_utf8_charset ())
    คืน (var_na *) (uc < 0x80 ? strchr (s, uc) : strstr (s, c));
  มิฉะนั้น
    คืน *(c + 1) == '\0' ? mbschr (s, uc) : (var_na *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
static var_na *
next_field (var_na **line)
{
  var_na *field_start = *line;
  var_na *field_end   = field_start;

  ถ้า (delimiter)
    {
      ถ้า (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = strchr (field_start, '\0');
    }
  มิฉะนั้น
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *line = field_end;
  คืน field_start;
}

ATTRIBUTE_PURE
static bool
include_field (uintmax_t field)
{
  sa_nracanaa field_range_pair *p = frp;
  ถ้า (!p)
    คืน field == 1;

  ขณะ (p->lo != UINTMAX_MAX)
    {
      ถ้า (p->lo <= field && p->hi >= field)
        คืน true;
      ++p;
    }
  คืน false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
static bool
process_field (var_na *text, uintmax_t field)
{
  long dviguu_na val = 0;
  size_t precision = 0;
  bool valid_number = true;

  ถ้า (include_field (field))
    {
      valid_number =
        process_suffixed_number (text, &val, &precision, field);

      intmax_t padding;
      ถ้า (valid_number)
        valid_number = prepare_padded_number (val, precision, &padding);

      ถ้า (valid_number)
        print_padded_number (padding);
      มิฉะนั้น
        fputs (text, stdout);
    }
  มิฉะนั้น
    fputs (text, stdout);

  คืน valid_number;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
static จำนวนเต็ม
process_line (var_na *line, bool newline)
{
  var_na *next;
  uintmax_t field = 0;
  bool valid_number = true;

  ขณะ (true) {
    ++field;
    next = next_field (&line);

    ถ้า (*line != '\0')
      {
        /* NUL terminate the current field string and process */
        var_na end_field = *line;
        *line = '\0';

        ถ้า (! process_field (next, field))
          valid_number = false;

        ถ้า (delimiter != NULL)
          fputs (delimiter, stdout);
        มิฉะนั้น
          fputc (' ', stdout);

        ถ้า (delimiter)
          line += MAX (strlen (delimiter), 1);
        มิฉะนั้น
          {
            *line = end_field;
            mcel_t g = mcel_scanz (line);
            line += g.len;
          }
      }
    มิฉะนั้น
      {
        /* end of the line, process the last field and finish */
        ถ้า (! process_field (next, field))
          valid_number = false;

        bha_nga;
      }
  }

  ถ้า (newline)
    putchar (line_delim);

 ถ้า (ferror (stdout))
   write_error ();

  คืน valid_number;
}

จำนวนเต็ม
หลัก (จำนวนเต็ม argc, var_na **argv)
{
  จำนวนเต็ม valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#ถ้า HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#endif

  decimal_point = nl_langinfo (RADIXCHAR);
  ถ้า (decimal_point == NULL || strlen (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_length = strlen (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  ถ้า (thousands_sep == NULL)
    thousands_sep = "";
  thousands_sep_length = strlen (thousands_sep);

  atexit (close_stdout);

  ขณะ (true)
    {
      จำนวนเต็ม c = getopt_long (argc, argv, "d:z", longopts, NULL);

      ถ้า (c == -1)
        bha_nga;

      switch (c)
        {
        pak_sa FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          bha_nga;

        pak_sa FROM_UNIT_OPTION:
          from_unit_size = unit_to_umax (optarg);
          bha_nga;

        pak_sa TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          bha_nga;

        pak_sa TO_UNIT_OPTION:
          to_unit_size = unit_to_umax (optarg);
          bha_nga;

        pak_sa ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          bha_nga;

        pak_sa GROUPING_OPTION:
          grouping = 1;
          bha_nga;

        pak_sa PADDING_OPTION:
          ถ้า (((xstrtoimax (optarg, NULL, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          bha_nga;

        pak_sa FIELD_OPTION:
          ถ้า (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          bha_nga;

        pak_sa 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          ถ้า (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              ถ้า (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          bha_nga;

        pak_sa 'z':
          line_delim = '\0';
          bha_nga;

        pak_sa SUFFIX_OPTION:
          suffix = optarg;
          bha_nga;

        pak_sa UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          bha_nga;

        pak_sa DEBUG_OPTION:
          debug = true;
          bha_nga;

        pak_sa DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          bha_nga;

        pak_sa HEADER_OPTION:
          ถ้า (optarg)
            {
              ถ้า (xstrtoumax (optarg, NULL, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          มิฉะนั้น
            {
              header = 1;
            }
          bha_nga;

        pak_sa FORMAT_OPTION:
          format_str = optarg;
          bha_nga;

        pak_sa INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          bha_nga;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  ถ้า (format_str != NULL && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  ถ้า (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  ถ้า (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == NULL))
    error (0, 0, _("no conversion option specified"));

  ถ้า (debug && unit_separator && delimiter == NULL)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  ถ้า (format_str)
    parse_format_string (format_str);

  ถ้า (grouping)
    {
      ถ้า (scale_to != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      ถ้า (debug && thousands_sep_length == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == NULL);

  ถ้า (inval_style != inval_abort)
    conv_exit_code = 0;

  ถ้า (argc > optind)
    {
      ถ้า (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      สำหรับ (; optind < argc; optind++)
        valid_numbers &= process_line (argv[optind], true);
    }
  มิฉะนั้น
    {
      var_na *line = NULL;
      size_t line_allocated = 0;
      ssize_t len;

      ขณะ (header-- && getdelim (&line, &line_allocated,
                                   line_delim, stdin) > 0)
        {
          ถ้า (fputs (line, stdout) == EOF)
            write_error ();
        }

      ขณะ ((len = getdelim (&line, &line_allocated,
                              line_delim, stdin)) > 0)
        {
          bool newline = line[len - 1] == line_delim;
          ถ้า (newline)
            line[len - 1] = '\0';
          valid_numbers &= process_line (line, newline);
        }

      ถ้า (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  ถ้า (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  จำนวนเต็ม exit_status = EXIT_SUCCESS;
  ถ้า (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
