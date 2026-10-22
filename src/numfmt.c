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

#شامل <config.h>
#شامل <float.h>
#شامل <getopt.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <langinfo.h>

#شامل "argmatch.h"
#شامل "assure.h"
#شامل "c-ctype.h"
#شامل "mbswidth.h"
#شامل "mcel.h"
#شامل "quote.h"
#شامل "skipchars.h"
#شامل "system.h"
#شامل "utf8.h"
#شامل "xstrtol.h"

#شامل "set-fields.h"

#اگر HAVE_FPSETPREC
# include <ieeefp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "numfmt"

#define AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
شمارش { EXIT_CONVERSION_WARNINGS = 2 };

شمارش
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

شمارش scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

ایستا نویسه ثابت *ثابت scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", پوچ
};

ایستا شمارش scale_type ثابت scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

ایستا نویسه ثابت *ثابت scale_to_args[] =
{
  "none", "si", "iec", "iec-i", پوچ
};

ایستا شمارش scale_type ثابت scale_to_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


شمارش round_type
{
  round_ceiling,
  round_floor,
  round_from_zero,
  round_to_zero,
  round_nearest,
};

ایستا نویسه ثابت *ثابت round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", پوچ
};

ایستا شمارش round_type ثابت round_types[] =
{
  round_ceiling, round_floor, round_from_zero, round_to_zero, round_nearest
};


شمارش inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

ایستا نویسه ثابت *ثابت inval_args[] =
{
  "abort", "fail", "warn", "ignore", پوچ
};

ایستا شمارش inval_type ثابت inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

ایستا ساختار option ثابت longopts[] =
{
  {"from", required_argument, پوچ, FROM_OPTION},
  {"from-unit", required_argument, پوچ, FROM_UNIT_OPTION},
  {"to", required_argument, پوچ, TO_OPTION},
  {"to-unit", required_argument, پوچ, TO_UNIT_OPTION},
  {"round", required_argument, پوچ, ROUND_OPTION},
  {"padding", required_argument, پوچ, PADDING_OPTION},
  {"suffix", required_argument, پوچ, SUFFIX_OPTION},
  {"unit-separator", required_argument, پوچ, UNIT_SEPARATOR_OPTION},
  {"grouping", no_argument, پوچ, GROUPING_OPTION},
  {"delimiter", required_argument, پوچ, 'd'},
  {"field", required_argument, پوچ, FIELD_OPTION},
  {"debug", no_argument, پوچ, DEBUG_OPTION},
  {"-debug", no_argument, پوچ, DEV_DEBUG_OPTION},
  {"header", optional_argument, پوچ, HEADER_OPTION},
  {"format", required_argument, پوچ, FORMAT_OPTION},
  {"invalid", required_argument, پوچ, INVALID_OPTION},
  {"zero-terminated", no_argument, پوچ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
شمارش { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
شمارش { MAX_ACCEPTABLE_DIGITS = 33 };

ایستا شمارش scale_type scale_from = scale_none;
ایستا شمارش scale_type scale_to = scale_none;
ایستا شمارش round_type round_style = round_from_zero;
ایستا شمارش inval_type inval_style = inval_abort;
ایستا نویسه ثابت *suffix = پوچ;
ایستا نویسه ثابت *unit_separator = پوچ;
ایستا uintmax_t from_unit_size = 1;
ایستا uintmax_t to_unit_size = 1;
ایستا صحیح grouping = 0;
ایستا نویسه *padding_buffer = پوچ;
ایستا idx_t padding_buffer_size = 0;
ایستا intmax_t padding_width = 0;
ایستا صحیح zero_padding_width = 0;
ایستا long صحیح user_precision = -1;
ایستا نویسه ثابت *format_str = پوچ;
ایستا نویسه *format_str_prefix = پوچ;
ایستا نویسه *format_str_suffix = پوچ;

/* By default, any conversion error will terminate the program.  */
ایستا صحیح conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
ایستا صحیح auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
ایستا نویسه ثابت *delimiter = پوچ;

/* line delimiter.  */
ایستا unsigned نویسه سطر_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
ایستا uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
ایستا bool debug;

/* will be set according to the current locale.  */
ایستا نویسه ثابت *decimal_point;
ایستا صحیح decimal_point_length;
ایستا نویسه ثابت *thousands_sep;
ایستا صحیح thousands_sep_length;

/* debugging for developers.  Enables devmsg().  */
ایستا bool dev_debug = false;

ایستا bool
newline_or_blank (mcel_t g)
{
  بازگشت g.ch == '\n' || c32issep (g.ch);
}

ایستا درونخطی صحیح
default_scale_base (شمارش scale_type scale)
{
  انتخاب (scale)
    {
    حالت scale_IEC:
    حالت scale_IEC_I:
      بازگشت 1024;

    حالت scale_none:
    حالت scale_auto:
    حالت scale_SI:
    پیشفرض:
      بازگشت 1000;
    }
}

ایستا نویسه ثابت zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
ایستا نویسه ثابت *valid_suffixes = 1 + zero_and_valid_suffixes;

ایستا درونخطی bool
valid_suffix (ثابت نویسه suf)
{
  بازگشت strchr (valid_suffixes, suf) != پوچ;
}

ایستا درونخطی صحیح
suffix_power (ثابت نویسه suf)
{
  انتخاب (suf)
    {
    حالت 'k':                  /* kilo.  */
    حالت 'K':                  /* kilo or kibi.  */
      بازگشت 1;

    حالت 'M':                  /* mega or mebi.  */
      بازگشت 2;

    حالت 'G':                  /* giga or gibi.  */
      بازگشت 3;

    حالت 'T':                  /* tera or tebi.  */
      بازگشت 4;

    حالت 'P':                  /* peta or pebi.  */
      بازگشت 5;

    حالت 'E':                  /* exa or exbi.  */
      بازگشت 6;

    حالت 'Z':                  /* zetta or 2**70.  */
      بازگشت 7;

    حالت 'Y':                  /* yotta or 2**80.  */
      بازگشت 8;

    حالت 'R':                  /* ronna or 2**90.  */
      بازگشت 9;

    حالت 'Q':                  /* quetta or 2**100.  */
      بازگشت 10;

    پیشفرض:                   /* should never happen. assert?  */
      بازگشت 0;
    }
}

ایستا درونخطی نویسه ثابت *
suffix_power_char (صحیح power)
{
  انتخاب (power)
    {
    حالت 0:
      بازگشت "";

    حالت 1:
      بازگشت "K";

    حالت 2:
      بازگشت "M";

    حالت 3:
      بازگشت "G";

    حالت 4:
      بازگشت "T";

    حالت 5:
      بازگشت "P";

    حالت 6:
      بازگشت "E";

    حالت 7:
      بازگشت "Z";

    حالت 8:
      بازگشت "Y";

    حالت 9:
      بازگشت "R";

    حالت 10:
      بازگشت "Q";

    پیشفرض:
      بازگشت "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
ایستا long دوگانه
powerld (long دوگانه base, ptrdiff_t x)
{
  long دوگانه result = base;
  اگر (x == 0)
    بازگشت 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  وقتی (--x)
    result *= base;
  بازگشت result;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
ایستا درونخطی long دوگانه
absld (long دوگانه val)
{
  بازگشت val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
ایستا long دوگانه
expld (long دوگانه val, صحیح base, صحیح /*output */ *x)
{
  صحیح power = 0;

  اگر (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      وقتی (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  اگر (x)
    *x = power;
  بازگشت val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ایستا درونخطی intmax_t
simple_round_ceiling (long دوگانه val)
{
  intmax_t intval = val;
  اگر (intval < val)
    intval++;
  بازگشت intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ایستا درونخطی intmax_t
simple_round_floor (long دوگانه val)
{
  بازگشت -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
ایستا درونخطی intmax_t
simple_round_from_zero (long دوگانه val)
{
  بازگشت val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
ایستا درونخطی intmax_t
simple_round_to_zero (long دوگانه val)
{
  بازگشت val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ایستا درونخطی intmax_t
simple_round_nearest (long دوگانه val)
{
  بازگشت val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
ایستا درونخطی long دوگانه
simple_round (long دوگانه val, شمارش round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (long دوگانه) INTMAX_MAX * intmax_mul;

  انتخاب (t)
    {
    حالت round_ceiling:
      rval = simple_round_ceiling (val);
      بشکن;

    حالت round_floor:
      rval = simple_round_floor (val);
      بشکن;

    حالت round_from_zero:
      rval = simple_round_from_zero (val);
      بشکن;

    حالت round_to_zero:
      rval = simple_round_to_zero (val);
      بشکن;

    حالت round_nearest:
      rval = simple_round_nearest (val);
      بشکن;

    پیشفرض:
      /* to silence the compiler - this should never happen.  */
      بازگشت 0;
    }

  بازگشت (long دوگانه) INTMAX_MAX * intmax_mul + rval;
}

شمارش simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_OVERFLOW,
  SSE_INVALID_عدد,

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
ایستا شمارش simple_strtod_error
simple_strtod_int (نویسه ثابت *درون_داد_str,
                   نویسه **endptr, long دوگانه *مقدار, bool *negative)
{
  شمارش simple_strtod_error e = SSE_OK;

  long دوگانه val = 0;
  صحیح digits = 0;
  bool found_digit = false;

  اگر (*درون_داد_str == '-')
    {
      درون_داد_str++;
      *negative = true;
    }
  وگرنه
    *negative = false;

  *endptr = (نویسه *) درون_داد_str;
  وقتی (c_isdigit (**endptr))
    {
      صحیح digit = (**endptr) - '0';

      found_digit = true;

      اگر (val || digit)
        digits++;

      اگر (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      اگر (digits > MAX_ACCEPTABLE_DIGITS)
        بازگشت SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      اگر (thousands_sep_length > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_length)
          && c_isdigit ((*endptr)[thousands_sep_length]))
        (*endptr) += thousands_sep_length;
    }
  اگر (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    بازگشت SSE_INVALID_عدد;
  اگر (*negative)
    val = -val;

  اگر (مقدار)
    *مقدار = val;

  بازگشت e;
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
ایستا شمارش simple_strtod_error
simple_strtod_float (نویسه ثابت *درون_داد_str,
                     نویسه **endptr,
                     long دوگانه *مقدار,
                     size_t *precision)
{
  bool negative;
  شمارش simple_strtod_error e = SSE_OK;

  اگر (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (درون_داد_str, endptr, مقدار, &negative);
  اگر (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    بازگشت e;

  /* optional decimal point + fraction.  */
  اگر (STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    {
      نویسه *ptr2;
      long دوگانه val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_length;
      شمارش simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      اگر (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        بازگشت e2;
      اگر (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      اگر (neg_frac)
        بازگشت SSE_INVALID_عدد;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      اگر (مقدار)
        {
          اگر (negative)
            *مقدار -= val_frac;
          وگرنه
            *مقدار += val_frac;
        }

      اگر (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  بازگشت e;
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
ایستا شمارش simple_strtod_error
simple_strtod_human (نویسه ثابت *درون_داد_str,
                     نویسه **endptr, long دوگانه *مقدار, size_t *precision,
                     شمارش scale_type allowed_scaling)
{
  صحیح power = 0;
  /* 'scale_auto' is checked below.  */
  صحیح scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, درون_داد_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  شمارش simple_strtod_error e =
    simple_strtod_float (درون_داد_str, endptr, مقدار, precision);
  اگر (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    بازگشت e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *مقدار, (صحیح)*precision);

  وقتی (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      اگر (unit_separator)
        {
          size_t sep_len = strlen (unit_separator);
          اگر (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      اگر (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          اگر (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      اگر (**endptr == '\0')
        بشکن;  /* Treat as no suffix.  */

      اگر (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          اگر (**endptr == '\0')
            بشکن;

          بازگشت SSE_INVALID_SUFFIX;
        }

      اگر (allowed_scaling == scale_none)
        بازگشت SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      اگر (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      وگرنه اگر (allowed_scaling == scale_IEC_I)
        {
          اگر (**endptr == 'i')
            (*endptr)++;
          وگرنه
            بازگشت SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      بشکن;
    }

  long دوگانه multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*مقدار) = (*مقدار) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *مقدار, *مقدار);

  بازگشت e;
}


ایستا تهی
simple_strtod_fatal (شمارش simple_strtod_error err, نویسه ثابت *درون_داد_str)
{
  نویسه ثابت *msgid = پوچ;

  انتخاب (err)
    {
    حالت SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      بشکن;

    حالت SSE_INVALID_عدد:
      msgid = N_("invalid number: %s");
      بشکن;

    حالت SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      بشکن;

    حالت SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      بشکن;

    حالت SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      بشکن;

    حالت SSE_OK_PRECISION_LOSS:
    حالت SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  اگر (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (درون_داد_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
ایستا صحیح
double_to_human (long دوگانه val, صحیح precision,
                 نویسه *buf, idx_t buf_size,
                 شمارش scale_type scale, صحیح group, شمارش round_type round)
{
  نویسه fmt[اندازه "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  نویسه *pfmt = fmt;
  *pfmt++ = '%';

  اگر (group)
    *pfmt++ = '\'';

  اگر (zero_padding_width)
    pfmt += sprintf (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  اگر (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      بازگشت snprintf (buf, buf_size, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  دوگانه scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  صحیح power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  صحیح power_adjust = 0;
  اگر (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  وگرنه اگر (absld (val) < 10)
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
  اگر (absld (val) >= scale_base)
    {
      val /= scale_base;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  صحیح show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  صحیح prec = user_precision == -1 ? show_decimal_point : user_precision;

  بازگشت snprintf (buf, buf_size, fmt, prec, val,
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
ایستا uintmax_t
unit_to_umax (نویسه ثابت *n_string)
{
  strtol_error s_err;
  نویسه ثابت *c_string = n_string;
  نویسه *t_string = پوچ;
  size_t n_len = strlen (n_string);
  نویسه *end = پوچ;
  uintmax_t n;
  نویسه ثابت *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  اگر (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      memcpy (t_string, n_string, n_len);

      اگر (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
        *end = '\0';
      وگرنه
        {
          *++end = 'B';
          *++end = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &end, 10, &n, suffixes);

  اگر (s_err != LONGINT_OK || *end || n == 0)
    {
      آزاد (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  آزاد (t_string);

  بازگشت n;
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [عدد_2]...\n\
"), program_name);
      fputs (_("\
Reformat عدد_2(s), or the numbers from standard درون_داد اگر none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid درون_داد\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace برای field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these درون_داد fields (پیشفرض=1);\n\
         see FIELDS below برای details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use printf style floating-point FORMAT;\n\
         see FORMAT below برای details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         auto-scale درون_داد numbers to UNITs; پیشفرض is 'none';\n\
         see UNIT below برای details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the درون_داد unit size (instead of the پیشفرض 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header سطرها;\n\
         N defaults to 1 اگر not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode برای invalid numbers;\n\
         MODE can be: abort (پیشفرض), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will راست-align, negative N will چپ-align;\n\
         padding is ignored اگر the output is wider than N;\n\
         the پیشفرض is to automatically pad اگر a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD برای rounding when scaling; METHOD can be:\n\
         up, down, from-zero (پیشفرض), towards-zero, nearest\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         add SUFFIX to output numbers,\n\
         and accept an optional SUFFIX in درون_داد numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         درج SEP between عدد and unit on output,\n\
         and accept an optional SEP in درون_داد numbers\n\
"));
      oputs (_("\
      --to=UNIT\n\
         auto-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit size (instead of the پیشفرض 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
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
  N-   from N'th field, to end of سطر\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable برای printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (اگر supported by current locale).\n\
Optional width مقدار (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the عدد. Optional negative مقادیر (%-10f) will چپ align.\n\
Optional precision (%.1f) will override the درون_داد determined precision.\n\
"), stdout);

      printf (_("\n\
Exit status is 0 اگر all درون_داد numbers were successfully converted.\n\
By پیشفرض, %s will stop at the first conversion error with exit status 2.\n\
With --invalid='fail' a warning is printed برای each conversion error\n\
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
ایستا تهی
parse_format_string (نویسه ثابت *fmt)
{
  size_t i;
  size_t prefix_len = 0;
  size_t suffix_pos;
  نویسه *endptr = پوچ;
  bool zero_padding = false;

  برای (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      اگر (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  وقتی (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      اگر (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      وگرنه اگر (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      وگرنه اگر (! skip)
        بشکن;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  اگر (pad != 0)
    {
      اگر (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      اگر (pad < 0)
        padding_width = pad;
      وگرنه
        {
          اگر (zero_padding)
            zero_padding_width = MIN (pad, INT_MAX);
          وگرنه
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  اگر (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  اگر (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      اگر (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  اگر (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  برای (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    اگر (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  اگر (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  اگر (fmt[suffix_pos] != '\0')
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
ایستا شمارش simple_strtod_error
parse_human_عدد (نویسه ثابت *str, long دوگانه /*output */ *مقدار,
                    size_t *precision)
{
  نویسه *ptr = پوچ;

  شمارش simple_strtod_error e =
    simple_strtod_human (str, &ptr, مقدار, precision, scale_from);
  اگر (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      بازگشت e;
    }

  اگر (ptr && *ptr != '\0')
    {
      اگر (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  بازگشت e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
ایستا bool
prepare_padded_عدد (ثابت long دوگانه val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  صحیح x;
  expld (val, 10, &x);

  اگر (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      اگر (inval_style != inval_ignore)
        {
          اگر (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          وگرنه
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      بازگشت false;
    }

  اگر (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      اگر (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      بازگشت false;
    }

  وقتی (true)
    {
      صحیح numlen = double_to_human (val, precision_used,
                                    padding_buffer, padding_buffer_size,
                                    scale_to, grouping, round_style);
      ptrdiff_t growth;
      اگر (numlen < 0 || ckd_sub (&growth, numlen, padding_buffer_size - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      اگر (growth <= 0)
        بشکن;
      padding_buffer = xpalloc (padding_buffer, &padding_buffer_size,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_buffer));

  intmax_t pad = 0;
  اگر (padding_width)
    {
      صحیح buf_width = mbswidth (padding_buffer,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      اگر (0 <= buf_width)
        {
          اگر (padding_width < 0)
            {
              اگر (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          وگرنه
            {
              اگر (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  بازگشت true;
}

ایستا تهی
print_padded_عدد (intmax_t padding)
{
  اگر (format_str_prefix)
    fputs (format_str_prefix, stdout);

  برای (intmax_t p = padding; 0 < p; p--)
    putchar (' ');

  fputs (padding_buffer, stdout);

  برای (intmax_t p = padding; p < 0; p++)
    putchar (' ');

  اگر (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
ایستا bool
process_suffixed_عدد (نویسه *text, long دوگانه *result,
                         size_t *precision, long صحیح field)
{
  نویسه saved_suffix = '\0';

  اگر (suffix)
    {
      اگر (mbs_endswith (text, suffix))
        {
          saved_suffix = *(text + strlen (text) - strlen (suffix));
          *(text + strlen (text) - strlen (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      وگرنه
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  نویسه *p = skip_str_matching (text, newline_or_blank, true);

  /* setup auto-padding.  */
  اگر (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      اگر (padding_width < 0)
        padding_width = strlen (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  long دوگانه val = 0;
  شمارش simple_strtod_error e = parse_human_عدد (p, &val, precision);
  اگر (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  اگر (from_unit_size != 1 || to_unit_size != 1)
    val = (val * from_unit_size) / to_unit_size;

  *result = val;

  اگر (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    بازگشت true;
  وگرنه
    {
      اگر (saved_suffix)
        *(text + strlen (text)) = saved_suffix;
      بازگشت false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
ایستا نویسه *
mbsmbchr (نویسه ثابت *s, نویسه ثابت *c)
{
  unsigned نویسه uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  اگر (uc < 0x30 || MB_CUR_MAX == 1)
    بازگشت (نویسه *) strchr (s, uc);
  وگرنه اگر (is_utf8_charset ())
    بازگشت (نویسه *) (uc < 0x80 ? strchr (s, uc) : strstr (s, c));
  وگرنه
    بازگشت *(c + 1) == '\0' ? mbschr (s, uc) : (نویسه *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
ایستا نویسه *
next_field (نویسه **سطر)
{
  نویسه *field_start = *سطر;
  نویسه *field_end   = field_start;

  اگر (delimiter)
    {
      اگر (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = strchr (field_start, '\0');
    }
  وگرنه
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *سطر = field_end;
  بازگشت field_start;
}

ATTRIBUTE_PURE
ایستا bool
include_field (uintmax_t field)
{
  ساختار field_range_pair *p = frp;
  اگر (!p)
    بازگشت field == 1;

  وقتی (p->lo != UINTMAX_MAX)
    {
      اگر (p->lo <= field && p->hi >= field)
        بازگشت true;
      ++p;
    }
  بازگشت false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
ایستا bool
process_field (نویسه *text, uintmax_t field)
{
  long دوگانه val = 0;
  size_t precision = 0;
  bool valid_عدد = true;

  اگر (include_field (field))
    {
      valid_عدد =
        process_suffixed_عدد (text, &val, &precision, field);

      intmax_t padding;
      اگر (valid_عدد)
        valid_عدد = prepare_padded_عدد (val, precision, &padding);

      اگر (valid_عدد)
        print_padded_عدد (padding);
      وگرنه
        fputs (text, stdout);
    }
  وگرنه
    fputs (text, stdout);

  بازگشت valid_عدد;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
ایستا صحیح
process_سطر (نویسه *سطر, bool newline)
{
  نویسه *next;
  uintmax_t field = 0;
  bool valid_عدد = true;

  وقتی (true) {
    ++field;
    next = next_field (&سطر);

    اگر (*سطر != '\0')
      {
        /* NUL terminate the current field string and process */
        نویسه end_field = *سطر;
        *سطر = '\0';

        اگر (! process_field (next, field))
          valid_عدد = false;

        اگر (delimiter != پوچ)
          fputs (delimiter, stdout);
        وگرنه
          fputc (' ', stdout);

        اگر (delimiter)
          سطر += MAX (strlen (delimiter), 1);
        وگرنه
          {
            *سطر = end_field;
            mcel_t g = mcel_scanz (سطر);
            سطر += g.len;
          }
      }
    وگرنه
      {
        /* end of the line, process the last field and finish */
        اگر (! process_field (next, field))
          valid_عدد = false;

        بشکن;
      }
  }

  اگر (newline)
    putchar (سطر_delim);

 اگر (ferror (stdout))
   write_error ();

  بازگشت valid_عدد;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#اگر HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#endif

  decimal_point = nl_langinfo (RADIXCHAR);
  اگر (decimal_point == پوچ || strlen (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_length = strlen (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  اگر (thousands_sep == پوچ)
    thousands_sep = "";
  thousands_sep_length = strlen (thousands_sep);

  atexit (close_stdout);

  وقتی (true)
    {
      صحیح c = getopt_long (argc, argv, "d:z", longopts, پوچ);

      اگر (c == -1)
        بشکن;

      انتخاب (c)
        {
        حالت FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          بشکن;

        حالت FROM_UNIT_OPTION:
          from_unit_size = unit_to_umax (optarg);
          بشکن;

        حالت TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          بشکن;

        حالت TO_UNIT_OPTION:
          to_unit_size = unit_to_umax (optarg);
          بشکن;

        حالت ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          بشکن;

        حالت GROUPING_OPTION:
          grouping = 1;
          بشکن;

        حالت PADDING_OPTION:
          اگر (((xstrtoimax (optarg, پوچ, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          بشکن;

        حالت FIELD_OPTION:
          اگر (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          بشکن;

        حالت 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          اگر (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              اگر (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          بشکن;

        حالت 'z':
          سطر_delim = '\0';
          بشکن;

        حالت SUFFIX_OPTION:
          suffix = optarg;
          بشکن;

        حالت UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          بشکن;

        حالت DEBUG_OPTION:
          debug = true;
          بشکن;

        حالت DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          بشکن;

        حالت HEADER_OPTION:
          اگر (optarg)
            {
              اگر (xstrtoumax (optarg, پوچ, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          وگرنه
            {
              header = 1;
            }
          بشکن;

        حالت FORMAT_OPTION:
          format_str = optarg;
          بشکن;

        حالت INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          بشکن;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (format_str != پوچ && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  اگر (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  اگر (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == پوچ))
    error (0, 0, _("no conversion option specified"));

  اگر (debug && unit_separator && delimiter == پوچ)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  اگر (format_str)
    parse_format_string (format_str);

  اگر (grouping)
    {
      اگر (scale_to != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      اگر (debug && thousands_sep_length == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == پوچ);

  اگر (inval_style != inval_abort)
    conv_exit_code = 0;

  اگر (argc > optind)
    {
      اگر (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      برای (; optind < argc; optind++)
        valid_numbers &= process_سطر (argv[optind], true);
    }
  وگرنه
    {
      نویسه *سطر = پوچ;
      size_t سطر_allocated = 0;
      ssize_t len;

      وقتی (header-- && getdelim (&سطر, &سطر_allocated,
                                   سطر_delim, stdin) > 0)
        {
          اگر (fputs (سطر, stdout) == EOF)
            write_error ();
        }

      وقتی ((len = getdelim (&سطر, &سطر_allocated,
                              سطر_delim, stdin)) > 0)
        {
          bool newline = سطر[len - 1] == سطر_delim;
          اگر (newline)
            سطر[len - 1] = '\0';
          valid_numbers &= process_سطر (سطر, newline);
        }

      اگر (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  اگر (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  صحیح exit_status = EXIT_SUCCESS;
  اگر (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
