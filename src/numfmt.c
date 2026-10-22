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

#만약 HAVE_FPSETPREC
# include <ieeefp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "numfmt"

#define AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
enum { EXIT_CONVERSION_WARNINGS = 2 };

enum
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

enum scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

static char const *const scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", NULL
};

static enum scale_type const scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

static char const *const scale_to_args[] =
{
  "none", "si", "iec", "iec-i", NULL
};

static enum scale_type const scale_to_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


enum round_type
{
  round_ceiling,
  round_floor,
  round_from_zero,
  round_to_zero,
  round_nearest,
};

static char const *const round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", NULL
};

static enum round_type const round_types[] =
{
  round_ceiling, round_floor, round_from_zero, round_to_zero, round_nearest
};


enum inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

static char const *const inval_args[] =
{
  "abort", "fail", "warn", "ignore", NULL
};

static enum inval_type const inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

static struct option const longopts[] =
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
enum { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
enum { MAX_ACCEPTABLE_DIGITS = 33 };

static enum scale_type scale_from = scale_none;
static enum scale_type scale_to = scale_none;
static enum round_type round_style = round_from_zero;
static enum inval_type inval_style = inval_abort;
static char const *suffix = NULL;
static char const *unit_separator = NULL;
static uintmax_t from_unit_size = 1;
static uintmax_t to_unit_size = 1;
static 정수 grouping = 0;
static char *padding_buffer = NULL;
static idx_t padding_buffer_size = 0;
static intmax_t padding_width = 0;
static 정수 zero_padding_width = 0;
static long 정수 user_precision = -1;
static char const *format_str = NULL;
static char *format_str_prefix = NULL;
static char *format_str_suffix = NULL;

/* By default, any conversion error will terminate the program.  */
static 정수 conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
static 정수 auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
static char const *delimiter = NULL;

/* line delimiter.  */
static unsigned char line_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
static uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
static bool debug;

/* will be set according to the current locale.  */
static char const *decimal_point;
static 정수 decimal_point_length;
static char const *thousands_sep;
static 정수 thousands_sep_length;

/* debugging for developers.  Enables devmsg().  */
static bool dev_debug = false;

static bool
newline_or_blank (mcel_t g)
{
  반환 g.ch == '\n' || c32issep (g.ch);
}

static inline 정수
default_scale_base (enum scale_type scale)
{
  switch (scale)
    {
    case scale_IEC:
    case scale_IEC_I:
      반환 1024;

    case scale_none:
    case scale_auto:
    case scale_SI:
    default:
      반환 1000;
    }
}

static char const zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
static char const *valid_suffixes = 1 + zero_and_valid_suffixes;

static inline bool
valid_suffix (const char suf)
{
  반환 strchr (valid_suffixes, suf) != NULL;
}

static inline 정수
suffix_power (const char suf)
{
  switch (suf)
    {
    case 'k':                  /* kilo.  */
    case 'K':                  /* kilo or kibi.  */
      반환 1;

    case 'M':                  /* mega or mebi.  */
      반환 2;

    case 'G':                  /* giga or gibi.  */
      반환 3;

    case 'T':                  /* tera or tebi.  */
      반환 4;

    case 'P':                  /* peta or pebi.  */
      반환 5;

    case 'E':                  /* exa or exbi.  */
      반환 6;

    case 'Z':                  /* zetta or 2**70.  */
      반환 7;

    case 'Y':                  /* yotta or 2**80.  */
      반환 8;

    case 'R':                  /* ronna or 2**90.  */
      반환 9;

    case 'Q':                  /* quetta or 2**100.  */
      반환 10;

    default:                   /* should never happen. assert?  */
      반환 0;
    }
}

static inline char const *
suffix_power_char (정수 power)
{
  switch (power)
    {
    case 0:
      반환 "";

    case 1:
      반환 "K";

    case 2:
      반환 "M";

    case 3:
      반환 "G";

    case 4:
      반환 "T";

    case 5:
      반환 "P";

    case 6:
      반환 "E";

    case 7:
      반환 "Z";

    case 8:
      반환 "Y";

    case 9:
      반환 "R";

    case 10:
      반환 "Q";

    default:
      반환 "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
static long double
powerld (long double base, ptrdiff_t x)
{
  long double result = base;
  만약 (x == 0)
    반환 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  동안 (--x)
    result *= base;
  반환 result;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
static inline long double
absld (long double val)
{
  반환 val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
static long double
expld (long double val, 정수 base, 정수 /*output */ *x)
{
  정수 power = 0;

  만약 (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      동안 (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  만약 (x)
    *x = power;
  반환 val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
static inline intmax_t
simple_round_ceiling (long double val)
{
  intmax_t intval = val;
  만약 (intval < val)
    intval++;
  반환 intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
static inline intmax_t
simple_round_floor (long double val)
{
  반환 -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
static inline intmax_t
simple_round_from_zero (long double val)
{
  반환 val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
static inline intmax_t
simple_round_to_zero (long double val)
{
  반환 val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
static inline intmax_t
simple_round_nearest (long double val)
{
  반환 val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
static inline long double
simple_round (long double val, enum round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (long double) INTMAX_MAX * intmax_mul;

  switch (t)
    {
    case round_ceiling:
      rval = simple_round_ceiling (val);
      break;

    case round_floor:
      rval = simple_round_floor (val);
      break;

    case round_from_zero:
      rval = simple_round_from_zero (val);
      break;

    case round_to_zero:
      rval = simple_round_to_zero (val);
      break;

    case round_nearest:
      rval = simple_round_nearest (val);
      break;

    default:
      /* to silence the compiler - this should never happen.  */
      반환 0;
    }

  반환 (long double) INTMAX_MAX * intmax_mul + rval;
}

enum simple_strtod_error
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
static enum simple_strtod_error
simple_strtod_int (char const *input_str,
                   char **endptr, long double *value, bool *negative)
{
  enum simple_strtod_error e = SSE_OK;

  long double val = 0;
  정수 digits = 0;
  bool found_digit = false;

  만약 (*input_str == '-')
    {
      input_str++;
      *negative = true;
    }
  아니면
    *negative = false;

  *endptr = (char *) input_str;
  동안 (c_isdigit (**endptr))
    {
      정수 digit = (**endptr) - '0';

      found_digit = true;

      만약 (val || digit)
        digits++;

      만약 (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      만약 (digits > MAX_ACCEPTABLE_DIGITS)
        반환 SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      만약 (thousands_sep_length > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_length)
          && c_isdigit ((*endptr)[thousands_sep_length]))
        (*endptr) += thousands_sep_length;
    }
  만약 (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    반환 SSE_INVALID_NUMBER;
  만약 (*negative)
    val = -val;

  만약 (value)
    *value = val;

  반환 e;
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
static enum simple_strtod_error
simple_strtod_float (char const *input_str,
                     char **endptr,
                     long double *value,
                     size_t *precision)
{
  bool negative;
  enum simple_strtod_error e = SSE_OK;

  만약 (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (input_str, endptr, value, &negative);
  만약 (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    반환 e;

  /* optional decimal point + fraction.  */
  만약 (STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    {
      char *ptr2;
      long double val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_length;
      enum simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      만약 (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        반환 e2;
      만약 (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      만약 (neg_frac)
        반환 SSE_INVALID_NUMBER;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      만약 (value)
        {
          만약 (negative)
            *value -= val_frac;
          아니면
            *value += val_frac;
        }

      만약 (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  반환 e;
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
static enum simple_strtod_error
simple_strtod_human (char const *input_str,
                     char **endptr, long double *value, size_t *precision,
                     enum scale_type allowed_scaling)
{
  정수 power = 0;
  /* 'scale_auto' is checked below.  */
  정수 scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, input_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  enum simple_strtod_error e =
    simple_strtod_float (input_str, endptr, value, precision);
  만약 (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    반환 e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *value, (정수)*precision);

  동안 (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      만약 (unit_separator)
        {
          size_t sep_len = strlen (unit_separator);
          만약 (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      만약 (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          만약 (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      만약 (**endptr == '\0')
        break;  /* Treat as no suffix.  */

      만약 (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          만약 (**endptr == '\0')
            break;

          반환 SSE_INVALID_SUFFIX;
        }

      만약 (allowed_scaling == scale_none)
        반환 SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      만약 (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      아니면 만약 (allowed_scaling == scale_IEC_I)
        {
          만약 (**endptr == 'i')
            (*endptr)++;
          아니면
            반환 SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      break;
    }

  long double multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*value) = (*value) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *value, *value);

  반환 e;
}


static void
simple_strtod_fatal (enum simple_strtod_error err, char const *input_str)
{
  char const *msgid = NULL;

  switch (err)
    {
    case SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      break;

    case SSE_INVALID_NUMBER:
      msgid = N_("invalid number: %s");
      break;

    case SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      break;

    case SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      break;

    case SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      break;

    case SSE_OK_PRECISION_LOSS:
    case SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  만약 (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (input_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
static 정수
double_to_human (long double val, 정수 precision,
                 char *buf, idx_t buf_size,
                 enum scale_type scale, 정수 group, enum round_type round)
{
  char fmt[sizeof "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  char *pfmt = fmt;
  *pfmt++ = '%';

  만약 (group)
    *pfmt++ = '\'';

  만약 (zero_padding_width)
    pfmt += sprintf (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  만약 (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      반환 snprintf (buf, buf_size, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  double scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  정수 power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  정수 power_adjust = 0;
  만약 (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  아니면 만약 (absld (val) < 10)
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
  만약 (absld (val) >= scale_base)
    {
      val /= scale_base;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  정수 show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  정수 prec = user_precision == -1 ? show_decimal_point : user_precision;

  반환 snprintf (buf, buf_size, fmt, prec, val,
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
unit_to_umax (char const *n_string)
{
  strtol_error s_err;
  char const *c_string = n_string;
  char *t_string = NULL;
  size_t n_len = strlen (n_string);
  char *end = NULL;
  uintmax_t n;
  char const *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  만약 (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      memcpy (t_string, n_string, n_len);

      만약 (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
        *end = '\0';
      아니면
        {
          *++end = 'B';
          *++end = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &end, 10, &n, suffixes);

  만약 (s_err != LONGINT_OK || *end || n == 0)
    {
      free (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  free (t_string);

  반환 n;
}

void
usage (정수 status)
{
  만약 (status != EXIT_SUCCESS)
    emit_try_help ();
  아니면
    {
      printf (_("\
Usage: %s [OPTION]... [NUMBER]...\n\
"), program_name);
      fputs (_("\
Reformat NUMBER(s), or the numbers from standard input 만약 none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid input\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace 위해 field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these input fields (default=1);\n\
         see FIELDS below 위해 details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use printf style floating-point FORMAT;\n\
         see FORMAT below 위해 details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         auto-scale input numbers to UNITs; default is 'none';\n\
         see UNIT below 위해 details\n\
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
         N defaults to 1 만약 not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode 위해 invalid numbers;\n\
         MODE can be: abort (default), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will right-align, negative N will left-align;\n\
         padding is ignored 만약 the output is wider than N;\n\
         the default is to automatically pad 만약 a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD 위해 rounding when scaling; METHOD can be:\n\
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
FORMAT must be suitable 위해 printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (만약 supported by current locale).\n\
Optional width value (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the number. Optional negative values (%-10f) will left align.\n\
Optional precision (%.1f) will override the input determined precision.\n\
"), stdout);

      printf (_("\n\
Exit status is 0 만약 all input numbers were successfully converted.\n\
By default, %s will stop at the first conversion error with exit status 2.\n\
With --invalid='fail' a warning is printed 위해 each conversion error\n\
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
parse_format_string (char const *fmt)
{
  size_t i;
  size_t prefix_len = 0;
  size_t suffix_pos;
  char *endptr = NULL;
  bool zero_padding = false;

  위해 (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      만약 (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  동안 (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      만약 (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      아니면 만약 (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      아니면 만약 (! skip)
        break;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  만약 (pad != 0)
    {
      만약 (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      만약 (pad < 0)
        padding_width = pad;
      아니면
        {
          만약 (zero_padding)
            zero_padding_width = MIN (pad, INT_MAX);
          아니면
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  만약 (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  만약 (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      만약 (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  만약 (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  위해 (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    만약 (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  만약 (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  만약 (fmt[suffix_pos] != '\0')
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
static enum simple_strtod_error
parse_human_number (char const *str, long double /*output */ *value,
                    size_t *precision)
{
  char *ptr = NULL;

  enum simple_strtod_error e =
    simple_strtod_human (str, &ptr, value, precision, scale_from);
  만약 (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      반환 e;
    }

  만약 (ptr && *ptr != '\0')
    {
      만약 (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  반환 e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
static bool
prepare_padded_number (const long double val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  정수 x;
  expld (val, 10, &x);

  만약 (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      만약 (inval_style != inval_ignore)
        {
          만약 (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          아니면
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      반환 false;
    }

  만약 (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      만약 (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      반환 false;
    }

  동안 (true)
    {
      정수 numlen = double_to_human (val, precision_used,
                                    padding_buffer, padding_buffer_size,
                                    scale_to, grouping, round_style);
      ptrdiff_t growth;
      만약 (numlen < 0 || ckd_sub (&growth, numlen, padding_buffer_size - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      만약 (growth <= 0)
        break;
      padding_buffer = xpalloc (padding_buffer, &padding_buffer_size,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_buffer));

  intmax_t pad = 0;
  만약 (padding_width)
    {
      정수 buf_width = mbswidth (padding_buffer,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      만약 (0 <= buf_width)
        {
          만약 (padding_width < 0)
            {
              만약 (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          아니면
            {
              만약 (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  반환 true;
}

static void
print_padded_number (intmax_t padding)
{
  만약 (format_str_prefix)
    fputs (format_str_prefix, stdout);

  위해 (intmax_t p = padding; 0 < p; p--)
    putchar (' ');

  fputs (padding_buffer, stdout);

  위해 (intmax_t p = padding; p < 0; p++)
    putchar (' ');

  만약 (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
static bool
process_suffixed_number (char *text, long double *result,
                         size_t *precision, long 정수 field)
{
  char saved_suffix = '\0';

  만약 (suffix)
    {
      만약 (mbs_endswith (text, suffix))
        {
          saved_suffix = *(text + strlen (text) - strlen (suffix));
          *(text + strlen (text) - strlen (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      아니면
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  char *p = skip_str_matching (text, newline_or_blank, true);

  /* setup auto-padding.  */
  만약 (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      만약 (padding_width < 0)
        padding_width = strlen (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  long double val = 0;
  enum simple_strtod_error e = parse_human_number (p, &val, precision);
  만약 (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  만약 (from_unit_size != 1 || to_unit_size != 1)
    val = (val * from_unit_size) / to_unit_size;

  *result = val;

  만약 (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    반환 true;
  아니면
    {
      만약 (saved_suffix)
        *(text + strlen (text)) = saved_suffix;
      반환 false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
static char *
mbsmbchr (char const *s, char const *c)
{
  unsigned char uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  만약 (uc < 0x30 || MB_CUR_MAX == 1)
    반환 (char *) strchr (s, uc);
  아니면 만약 (is_utf8_charset ())
    반환 (char *) (uc < 0x80 ? strchr (s, uc) : strstr (s, c));
  아니면
    반환 *(c + 1) == '\0' ? mbschr (s, uc) : (char *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
static char *
next_field (char **line)
{
  char *field_start = *line;
  char *field_end   = field_start;

  만약 (delimiter)
    {
      만약 (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = strchr (field_start, '\0');
    }
  아니면
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *line = field_end;
  반환 field_start;
}

ATTRIBUTE_PURE
static bool
include_field (uintmax_t field)
{
  struct field_range_pair *p = frp;
  만약 (!p)
    반환 field == 1;

  동안 (p->lo != UINTMAX_MAX)
    {
      만약 (p->lo <= field && p->hi >= field)
        반환 true;
      ++p;
    }
  반환 false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
static bool
process_field (char *text, uintmax_t field)
{
  long double val = 0;
  size_t precision = 0;
  bool valid_number = true;

  만약 (include_field (field))
    {
      valid_number =
        process_suffixed_number (text, &val, &precision, field);

      intmax_t padding;
      만약 (valid_number)
        valid_number = prepare_padded_number (val, precision, &padding);

      만약 (valid_number)
        print_padded_number (padding);
      아니면
        fputs (text, stdout);
    }
  아니면
    fputs (text, stdout);

  반환 valid_number;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
static 정수
process_line (char *line, bool newline)
{
  char *next;
  uintmax_t field = 0;
  bool valid_number = true;

  동안 (true) {
    ++field;
    next = next_field (&line);

    만약 (*line != '\0')
      {
        /* NUL terminate the current field string and process */
        char end_field = *line;
        *line = '\0';

        만약 (! process_field (next, field))
          valid_number = false;

        만약 (delimiter != NULL)
          fputs (delimiter, stdout);
        아니면
          fputc (' ', stdout);

        만약 (delimiter)
          line += MAX (strlen (delimiter), 1);
        아니면
          {
            *line = end_field;
            mcel_t g = mcel_scanz (line);
            line += g.len;
          }
      }
    아니면
      {
        /* end of the line, process the last field and finish */
        만약 (! process_field (next, field))
          valid_number = false;

        break;
      }
  }

  만약 (newline)
    putchar (line_delim);

 만약 (ferror (stdout))
   write_error ();

  반환 valid_number;
}

정수
main (정수 argc, char **argv)
{
  정수 valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#만약 HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#endif

  decimal_point = nl_langinfo (RADIXCHAR);
  만약 (decimal_point == NULL || strlen (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_length = strlen (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  만약 (thousands_sep == NULL)
    thousands_sep = "";
  thousands_sep_length = strlen (thousands_sep);

  atexit (close_stdout);

  동안 (true)
    {
      정수 c = getopt_long (argc, argv, "d:z", longopts, NULL);

      만약 (c == -1)
        break;

      switch (c)
        {
        case FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          break;

        case FROM_UNIT_OPTION:
          from_unit_size = unit_to_umax (optarg);
          break;

        case TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          break;

        case TO_UNIT_OPTION:
          to_unit_size = unit_to_umax (optarg);
          break;

        case ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          break;

        case GROUPING_OPTION:
          grouping = 1;
          break;

        case PADDING_OPTION:
          만약 (((xstrtoimax (optarg, NULL, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          break;

        case FIELD_OPTION:
          만약 (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          break;

        case 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          만약 (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              만약 (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          break;

        case 'z':
          line_delim = '\0';
          break;

        case SUFFIX_OPTION:
          suffix = optarg;
          break;

        case UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          break;

        case DEBUG_OPTION:
          debug = true;
          break;

        case DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          break;

        case HEADER_OPTION:
          만약 (optarg)
            {
              만약 (xstrtoumax (optarg, NULL, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          아니면
            {
              header = 1;
            }
          break;

        case FORMAT_OPTION:
          format_str = optarg;
          break;

        case INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          break;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  만약 (format_str != NULL && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  만약 (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  만약 (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == NULL))
    error (0, 0, _("no conversion option specified"));

  만약 (debug && unit_separator && delimiter == NULL)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  만약 (format_str)
    parse_format_string (format_str);

  만약 (grouping)
    {
      만약 (scale_to != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      만약 (debug && thousands_sep_length == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == NULL);

  만약 (inval_style != inval_abort)
    conv_exit_code = 0;

  만약 (argc > optind)
    {
      만약 (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      위해 (; optind < argc; optind++)
        valid_numbers &= process_line (argv[optind], true);
    }
  아니면
    {
      char *line = NULL;
      size_t line_allocated = 0;
      ssize_t len;

      동안 (header-- && getdelim (&line, &line_allocated,
                                   line_delim, stdin) > 0)
        {
          만약 (fputs (line, stdout) == EOF)
            write_error ();
        }

      동안 ((len = getdelim (&line, &line_allocated,
                              line_delim, stdin)) > 0)
        {
          bool newline = line[len - 1] == line_delim;
          만약 (newline)
            line[len - 1] = '\0';
          valid_numbers &= process_line (line, newline);
        }

      만약 (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  만약 (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  정수 exit_status = EXIT_SUCCESS;
  만약 (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
