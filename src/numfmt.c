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

#जर HAVE_FPSETPREC
# include <ieeefp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "numfmt"

#define AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
गणना { EXIT_CONVERSION_WARNINGS = 2 };

गणना
{
  FROM_OPTION = अक्षर_उच्च + 1,
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

स्थिर अक्षर स्थिरांक *स्थिरांक scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", रिक्त
};

स्थिर गणना scale_type स्थिरांक scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

स्थिर अक्षर स्थिरांक *स्थिरांक scale_to_args[] =
{
  "none", "si", "iec", "iec-i", रिक्त
};

स्थिर गणना scale_type स्थिरांक scale_to_types[] =
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

स्थिर अक्षर स्थिरांक *स्थिरांक round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", रिक्त
};

स्थिर गणना round_type स्थिरांक round_types[] =
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

स्थिर अक्षर स्थिरांक *स्थिरांक inval_args[] =
{
  "abort", "fail", "warn", "ignore", रिक्त
};

स्थिर गणना inval_type स्थिरांक inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

स्थिर संरचना option स्थिरांक longopts[] =
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
गणना { MAX_UNSCALED_DIGITS = द_द्वि_भग्न };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
गणना { MAX_ACCEPTABLE_DIGITS = 33 };

स्थिर गणना scale_type scale_from = scale_none;
स्थिर गणना scale_type scale_to = scale_none;
स्थिर गणना round_type round_style = round_from_zero;
स्थिर गणना inval_type inval_style = inval_abort;
स्थिर अक्षर स्थिरांक *suffix = रिक्त;
स्थिर अक्षर स्थिरांक *unit_separator = रिक्त;
स्थिर uintmax_t from_unit_size = 1;
स्थिर uintmax_t to_unit_size = 1;
स्थिर पूर्णांक grouping = 0;
स्थिर अक्षर *padding_buffer = रिक्त;
स्थिर idx_t padding_buffer_size = 0;
स्थिर intmax_t padding_width = 0;
स्थिर पूर्णांक zero_padding_width = 0;
स्थिर दीर्घ पूर्णांक user_precision = -1;
स्थिर अक्षर स्थिरांक *format_str = रिक्त;
स्थिर अक्षर *format_str_prefix = रिक्त;
स्थिर अक्षर *format_str_suffix = रिक्त;

/* By default, any conversion error will terminate the program.  */
स्थिर पूर्णांक conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
स्थिर पूर्णांक auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
स्थिर अक्षर स्थिरांक *delimiter = रिक्त;

/* line delimiter.  */
स्थिर अचिन्हित अक्षर ओळ_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
स्थिर uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
स्थिर bool debug;

/* will be set according to the current locale.  */
स्थिर अक्षर स्थिरांक *decimal_point;
स्थिर पूर्णांक decimal_point_length;
स्थिर अक्षर स्थिरांक *thousands_sep;
स्थिर पूर्णांक thousands_sep_length;

/* debugging for developers.  Enables devmsg().  */
स्थिर bool dev_debug = false;

स्थिर bool
newline_or_blank (mcel_t g)
{
  परत g.ch == '\n' || c32issep (g.ch);
}

स्थिर ओळीत पूर्णांक
default_scale_base (गणना scale_type scale)
{
  निवड (scale)
    {
    स्थिती scale_IEC:
    स्थिती scale_IEC_I:
      परत 1024;

    स्थिती scale_none:
    स्थिती scale_auto:
    स्थिती scale_SI:
    पूर्वनिर्धारित:
      परत 1000;
    }
}

स्थिर अक्षर स्थिरांक zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
स्थिर अक्षर स्थिरांक *valid_suffixes = 1 + zero_and_valid_suffixes;

स्थिर ओळीत bool
valid_suffix (स्थिरांक अक्षर suf)
{
  परत म_अक्षर (valid_suffixes, suf) != रिक्त;
}

स्थिर ओळीत पूर्णांक
suffix_power (स्थिरांक अक्षर suf)
{
  निवड (suf)
    {
    स्थिती 'k':                  /* kilo.  */
    स्थिती 'K':                  /* kilo or kibi.  */
      परत 1;

    स्थिती 'M':                  /* mega or mebi.  */
      परत 2;

    स्थिती 'G':                  /* giga or gibi.  */
      परत 3;

    स्थिती 'T':                  /* tera or tebi.  */
      परत 4;

    स्थिती 'P':                  /* peta or pebi.  */
      परत 5;

    स्थिती 'E':                  /* exa or exbi.  */
      परत 6;

    स्थिती 'Z':                  /* zetta or 2**70.  */
      परत 7;

    स्थिती 'Y':                  /* yotta or 2**80.  */
      परत 8;

    स्थिती 'R':                  /* ronna or 2**90.  */
      परत 9;

    स्थिती 'Q':                  /* quetta or 2**100.  */
      परत 10;

    पूर्वनिर्धारित:                   /* should never happen. assert?  */
      परत 0;
    }
}

स्थिर ओळीत अक्षर स्थिरांक *
suffix_power_char (पूर्णांक power)
{
  निवड (power)
    {
    स्थिती 0:
      परत "";

    स्थिती 1:
      परत "K";

    स्थिती 2:
      परत "M";

    स्थिती 3:
      परत "G";

    स्थिती 4:
      परत "T";

    स्थिती 5:
      परत "P";

    स्थिती 6:
      परत "E";

    स्थिती 7:
      परत "Z";

    स्थिती 8:
      परत "Y";

    स्थिती 9:
      परत "R";

    स्थिती 10:
      परत "Q";

    पूर्वनिर्धारित:
      परत "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
स्थिर दीर्घ द्विगुण
powerld (दीर्घ द्विगुण base, सूचक_भेद_प्रकार x)
{
  दीर्घ द्विगुण result = base;
  जर (x == 0)
    परत 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  पर्यंत (--x)
    result *= base;
  परत result;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
स्थिर ओळीत दीर्घ द्विगुण
absld (दीर्घ द्विगुण val)
{
  परत val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
स्थिर दीर्घ द्विगुण
expld (दीर्घ द्विगुण val, पूर्णांक base, पूर्णांक /*output */ *x)
{
  पूर्णांक power = 0;

  जर (val >= -द_द्वि_उच्च && val <= द_द्वि_उच्च)
    {
      पर्यंत (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  जर (x)
    *x = power;
  परत val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
स्थिर ओळीत intmax_t
simple_round_ceiling (दीर्घ द्विगुण val)
{
  intmax_t intval = val;
  जर (intval < val)
    intval++;
  परत intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
स्थिर ओळीत intmax_t
simple_round_floor (दीर्घ द्विगुण val)
{
  परत -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
स्थिर ओळीत intmax_t
simple_round_from_zero (दीर्घ द्विगुण val)
{
  परत val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
स्थिर ओळीत intmax_t
simple_round_to_zero (दीर्घ द्विगुण val)
{
  परत val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
स्थिर ओळीत intmax_t
simple_round_nearest (दीर्घ द्विगुण val)
{
  परत val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
स्थिर ओळीत दीर्घ द्विगुण
simple_round (दीर्घ द्विगुण val, गणना round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (दीर्घ द्विगुण) INTMAX_MAX * intmax_mul;

  निवड (t)
    {
    स्थिती round_ceiling:
      rval = simple_round_ceiling (val);
      थांबा;

    स्थिती round_floor:
      rval = simple_round_floor (val);
      थांबा;

    स्थिती round_from_zero:
      rval = simple_round_from_zero (val);
      थांबा;

    स्थिती round_to_zero:
      rval = simple_round_to_zero (val);
      थांबा;

    स्थिती round_nearest:
      rval = simple_round_nearest (val);
      थांबा;

    पूर्वनिर्धारित:
      /* to silence the compiler - this should never happen.  */
      परत 0;
    }

  परत (दीर्घ द्विगुण) INTMAX_MAX * intmax_mul + rval;
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
simple_strtod_int (अक्षर स्थिरांक *आदान_str,
                   अक्षर **endptr, दीर्घ द्विगुण *मूल्य, bool *negative)
{
  गणना simple_strtod_error e = SSE_OK;

  दीर्घ द्विगुण val = 0;
  पूर्णांक digits = 0;
  bool found_digit = false;

  जर (*आदान_str == '-')
    {
      आदान_str++;
      *negative = true;
    }
  अन्यथा
    *negative = false;

  *endptr = (अक्षर *) आदान_str;
  पर्यंत (c_isdigit (**endptr))
    {
      पूर्णांक digit = (**endptr) - '0';

      found_digit = true;

      जर (val || digit)
        digits++;

      जर (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      जर (digits > MAX_ACCEPTABLE_DIGITS)
        परत SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      जर (thousands_sep_length > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_length)
          && c_isdigit ((*endptr)[thousands_sep_length]))
        (*endptr) += thousands_sep_length;
    }
  जर (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    परत SSE_INVALID_संख्या;
  जर (*negative)
    val = -val;

  जर (मूल्य)
    *मूल्य = val;

  परत e;
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
simple_strtod_float (अक्षर स्थिरांक *आदान_str,
                     अक्षर **endptr,
                     दीर्घ द्विगुण *मूल्य,
                     माप_प्रकार *precision)
{
  bool negative;
  गणना simple_strtod_error e = SSE_OK;

  जर (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (आदान_str, endptr, मूल्य, &negative);
  जर (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    परत e;

  /* optional decimal point + fraction.  */
  जर (STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    {
      अक्षर *ptr2;
      दीर्घ द्विगुण val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_length;
      गणना simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      जर (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        परत e2;
      जर (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      जर (neg_frac)
        परत SSE_INVALID_संख्या;

      /* number of digits in the fractions.  */
      सूचक_भेद_प्रकार exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      जर (मूल्य)
        {
          जर (negative)
            *मूल्य -= val_frac;
          अन्यथा
            *मूल्य += val_frac;
        }

      जर (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  परत e;
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
simple_strtod_human (अक्षर स्थिरांक *आदान_str,
                     अक्षर **endptr, दीर्घ द्विगुण *मूल्य, माप_प्रकार *precision,
                     गणना scale_type allowed_scaling)
{
  पूर्णांक power = 0;
  /* 'scale_auto' is checked below.  */
  पूर्णांक scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, आदान_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  गणना simple_strtod_error e =
    simple_strtod_float (आदान_str, endptr, मूल्य, precision);
  जर (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    परत e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *मूल्य, (पूर्णांक)*precision);

  पर्यंत (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      जर (unit_separator)
        {
          माप_प्रकार sep_len = म_माप (unit_separator);
          जर (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      जर (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          जर (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      जर (**endptr == '\0')
        थांबा;  /* Treat as no suffix.  */

      जर (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          जर (**endptr == '\0')
            थांबा;

          परत SSE_INVALID_SUFFIX;
        }

      जर (allowed_scaling == scale_none)
        परत SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      जर (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      अन्यथा जर (allowed_scaling == scale_IEC_I)
        {
          जर (**endptr == 'i')
            (*endptr)++;
          अन्यथा
            परत SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      थांबा;
    }

  दीर्घ द्विगुण multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*मूल्य) = (*मूल्य) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *मूल्य, *मूल्य);

  परत e;
}


स्थिर शून्य
simple_strtod_fatal (गणना simple_strtod_error err, अक्षर स्थिरांक *आदान_str)
{
  अक्षर स्थिरांक *msgid = रिक्त;

  निवड (err)
    {
    स्थिती SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      थांबा;

    स्थिती SSE_INVALID_संख्या:
      msgid = N_("invalid number: %s");
      थांबा;

    स्थिती SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      थांबा;

    स्थिती SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      थांबा;

    स्थिती SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      थांबा;

    स्थिती SSE_OK_PRECISION_LOSS:
    स्थिती SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  जर (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (आदान_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
स्थिर पूर्णांक
double_to_human (दीर्घ द्विगुण val, पूर्णांक precision,
                 अक्षर *buf, idx_t buf_size,
                 गणना scale_type scale, पूर्णांक group, गणना round_type round)
{
  अक्षर fmt[आकार "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  अक्षर *pfmt = fmt;
  *pfmt++ = '%';

  जर (group)
    *pfmt++ = '\'';

  जर (zero_padding_width)
    pfmt += प्र_लिखो (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  जर (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      म_नकल (pfmt, ".*Lf%s");

      परत snprintf (buf, buf_size, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  द्विगुण scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  पूर्णांक power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  पूर्णांक power_adjust = 0;
  जर (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  अन्यथा जर (absld (val) < 10)
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
  जर (absld (val) >= scale_base)
    {
      val /= scale_base;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  पूर्णांक show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  म_नकल (pfmt, ".*Lf%s%s%s%s");

  पूर्णांक prec = user_precision == -1 ? show_decimal_point : user_precision;

  परत snprintf (buf, buf_size, fmt, prec, val,
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
unit_to_umax (अक्षर स्थिरांक *n_string)
{
  strtol_error s_err;
  अक्षर स्थिरांक *c_string = n_string;
  अक्षर *t_string = रिक्त;
  माप_प्रकार n_len = म_माप (n_string);
  अक्षर *end = रिक्त;
  uintmax_t n;
  अक्षर स्थिरांक *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  जर (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      स_नकल (t_string, n_string, n_len);

      जर (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
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

  जर (s_err != LONGINT_OK || *end || n == 0)
    {
      मुक्त (t_string);
      error (निकास_त्रुटि, 0, _("invalid unit size: %s"), quote (n_string));
    }

  मुक्त (t_string);

  परत n;
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [संख्या_2]...\n\
"), program_name);
      ख_माला_दो (_("\
Reformat संख्या_2(s), or the numbers from standard आदान जर none are specified.\n\
"), मानक_निकास);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid आदान\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace साठी field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these आदान fields (पूर्वनिर्धारित=1);\n\
         see FIELDS below साठी details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use म_लिखो style floating-point FORMAT;\n\
         see FORMAT below साठी details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         स्वतः-scale आदान numbers to UNITs; पूर्वनिर्धारित is 'none';\n\
         see UNIT below साठी details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the आदान unit size (instead of the पूर्वनिर्धारित 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header ओळी;\n\
         N defaults to 1 जर not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode साठी invalid numbers;\n\
         MODE can be: पात (पूर्वनिर्धारित), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will उजवा-align, negative N will डावा-align;\n\
         padding is ignored जर the output is wider than N;\n\
         the पूर्वनिर्धारित is to automatically pad जर a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD साठी rounding when scaling; METHOD can be:\n\
         up, down, from-zero (पूर्वनिर्धारित), towards-zero, nearest\n\
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
         स्वतः-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit size (instead of the पूर्वनिर्धारित 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ओळ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      ख_माला_दो (_("\
\n\
UNIT options:\n"), मानक_निकास);
      ख_माला_दो (_("\
  none       no स्वतः-scaling is done; suffixes will trigger an error\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  स्वतः       accept optional single/two letter suffix:\n\
               1K = 1000, 1k = 1000,\n\
               1Ki = 1024,\n\
               1M = 1000000,\n\
               1Mi = 1048576,\n"), मानक_निकास);
      ख_माला_दो (_("\
  si         accept optional single letter suffix:\n\
               1k = 1000, 1K = 1000,\n\
               1M = 1000000,\n\
               ...\n"), मानक_निकास);
      ख_माला_दो (_("\
  iec        accept optional single letter suffix:\n\
               1K = 1024, 1k = 1024,\n\
               1M = 1048576,\n\
               ...\n"), मानक_निकास);
      ख_माला_दो (_("\
  iec-i      accept optional two-letter suffix:\n\
               1Ki = 1024, 1ki = 1024,\n\
               1Mi = 1048576,\n\
               ...\n"), मानक_निकास);

      ख_माला_दो (_("\n\
FIELDS supports cut(1) style field ranges:\n\
  N    N'th field, counted from 1\n\
  N-   from N'th field, to end of ओळ\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), मानक_निकास);

      ख_माला_दो (_("\n\
FORMAT must be suitable साठी printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (जर supported by current locale).\n\
Optional width मूल्य (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the संख्या. Optional negative मूल्ये (%-10f) will डावा align.\n\
Optional precision (%.1f) will override the आदान determined precision.\n\
"), मानक_निकास);

      म_लिखो (_("\n\
Exit status is 0 जर all आदान numbers were successfully converted.\n\
By पूर्वनिर्धारित, %s will stop at the first conversion error with निकास status 2.\n\
With --invalid='fail' a warning is printed साठी each conversion error\n\
and the निकास status is 2.  With --invalid='warn' each conversion error is\n\
diagnosed, but the निकास status is 0.  With --invalid='ignore' conversion\n\
errors are not diagnosed and the निकास status is 0.\n\
"), program_name);

      म_लिखो (_("\n\
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
  निकास (status);
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
parse_format_string (अक्षर स्थिरांक *fmt)
{
  माप_प्रकार i;
  माप_प्रकार prefix_len = 0;
  माप_प्रकार suffix_pos;
  अक्षर *endptr = रिक्त;
  bool zero_padding = false;

  साठी (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      जर (!fmt[i])
        error (निकास_त्रुटि, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  पर्यंत (true)
    {
      माप_प्रकार skip = म_अखोज (fmt + i, " ");
      i += skip;
      जर (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      अन्यथा जर (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      अन्यथा जर (! skip)
        थांबा;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  जर (pad != 0)
    {
      जर (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      जर (pad < 0)
        padding_width = pad;
      अन्यथा
        {
          जर (zero_padding)
            zero_padding_width = MIN (pad, पूर्णांक_उच्च);
          अन्यथा
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  जर (fmt[i] == '\0')
    error (निकास_त्रुटि, 0, _("format %s ends in %%"), quote (fmt));

  जर (fmt[i] == '.')
    {
      i++;
      त्रुटि_सं = 0;
      user_precision = म_से_दीर्घ (fmt + i, &endptr, 10);
      जर (त्रुटि_सं == दुस्फल || user_precision < 0 || SIZE_MAX < user_precision
          || c_isblank (fmt[i]) || fmt[i] == '+')
        {
          /* Note we disallow negative user_precision to be
             consistent with printf(1).  POSIX states that
             negative precision is only supported (and ignored)
             when used with '.*f'.  glibc at least will malform
             output when passed a direct negative precision.  */
          error (निकास_त्रुटि, 0,
                 _("invalid precision in format %s"), quote (fmt));
        }
      i = endptr - fmt;
    }

  जर (fmt[i] != 'f')
    error (निकास_त्रुटि, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  साठी (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    जर (fmt[i] == '%' && fmt[i + 1] != '%')
      error (निकास_त्रुटि, 0, _("format %s has too many %% directives"),
             quote (fmt));

  जर (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  जर (fmt[suffix_pos] != '\0')
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
parse_human_संख्या (अक्षर स्थिरांक *str, दीर्घ द्विगुण /*output */ *मूल्य,
                    माप_प्रकार *precision)
{
  अक्षर *ptr = रिक्त;

  गणना simple_strtod_error e =
    simple_strtod_human (str, &ptr, मूल्य, precision, scale_from);
  जर (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      परत e;
    }

  जर (ptr && *ptr != '\0')
    {
      जर (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  परत e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
स्थिर bool
prepare_padded_संख्या (स्थिरांक दीर्घ द्विगुण val, माप_प्रकार precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  माप_प्रकार precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  पूर्णांक x;
  expld (val, 10, &x);

  जर (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      जर (inval_style != inval_ignore)
        {
          जर (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          अन्यथा
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      परत false;
    }

  जर (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      जर (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      परत false;
    }

  पर्यंत (true)
    {
      पूर्णांक numlen = double_to_human (val, precision_used,
                                    padding_buffer, padding_buffer_size,
                                    scale_to, grouping, round_style);
      सूचक_भेद_प्रकार growth;
      जर (numlen < 0 || ckd_sub (&growth, numlen, padding_buffer_size - 1))
        error (निकास_त्रुटि, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      जर (growth <= 0)
        थांबा;
      padding_buffer = xpalloc (padding_buffer, &padding_buffer_size,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_buffer));

  intmax_t pad = 0;
  जर (padding_width)
    {
      पूर्णांक buf_width = mbswidth (padding_buffer,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      जर (0 <= buf_width)
        {
          जर (padding_width < 0)
            {
              जर (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          अन्यथा
            {
              जर (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  परत true;
}

स्थिर शून्य
print_padded_संख्या (intmax_t padding)
{
  जर (format_str_prefix)
    ख_माला_दो (format_str_prefix, मानक_निकास);

  साठी (intmax_t p = padding; 0 < p; p--)
    अक्षर_दो (' ');

  ख_माला_दो (padding_buffer, मानक_निकास);

  साठी (intmax_t p = padding; p < 0; p++)
    अक्षर_दो (' ');

  जर (format_str_suffix)
    ख_माला_दो (format_str_suffix, मानक_निकास);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
स्थिर bool
process_suffixed_संख्या (अक्षर *text, दीर्घ द्विगुण *result,
                         माप_प्रकार *precision, दीर्घ पूर्णांक field)
{
  अक्षर saved_suffix = '\0';

  जर (suffix)
    {
      जर (mbs_endswith (text, suffix))
        {
          saved_suffix = *(text + म_माप (text) - म_माप (suffix));
          *(text + म_माप (text) - म_माप (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      अन्यथा
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  अक्षर *p = skip_str_matching (text, newline_or_blank, true);

  /* setup auto-padding.  */
  जर (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      जर (padding_width < 0)
        padding_width = म_माप (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  दीर्घ द्विगुण val = 0;
  गणना simple_strtod_error e = parse_human_संख्या (p, &val, precision);
  जर (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  जर (from_unit_size != 1 || to_unit_size != 1)
    val = (val * from_unit_size) / to_unit_size;

  *result = val;

  जर (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    परत true;
  अन्यथा
    {
      जर (saved_suffix)
        *(text + म_माप (text)) = saved_suffix;
      परत false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
स्थिर अक्षर *
mbsmbchr (अक्षर स्थिरांक *s, अक्षर स्थिरांक *c)
{
  अचिन्हित अक्षर uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  जर (uc < 0x30 || MB_CUR_MAX == 1)
    परत (अक्षर *) म_अक्षर (s, uc);
  अन्यथा जर (is_utf8_charset ())
    परत (अक्षर *) (uc < 0x80 ? म_अक्षर (s, uc) : म_माला (s, c));
  अन्यथा
    परत *(c + 1) == '\0' ? mbschr (s, uc) : (अक्षर *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
स्थिर अक्षर *
next_field (अक्षर **ओळ)
{
  अक्षर *field_start = *ओळ;
  अक्षर *field_end   = field_start;

  जर (delimiter)
    {
      जर (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = म_अक्षर (field_start, '\0');
    }
  अन्यथा
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *ओळ = field_end;
  परत field_start;
}

ATTRIBUTE_PURE
स्थिर bool
include_field (uintmax_t field)
{
  संरचना field_range_pair *p = frp;
  जर (!p)
    परत field == 1;

  पर्यंत (p->lo != UINTMAX_MAX)
    {
      जर (p->lo <= field && p->hi >= field)
        परत true;
      ++p;
    }
  परत false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
स्थिर bool
process_field (अक्षर *text, uintmax_t field)
{
  दीर्घ द्विगुण val = 0;
  माप_प्रकार precision = 0;
  bool valid_संख्या = true;

  जर (include_field (field))
    {
      valid_संख्या =
        process_suffixed_संख्या (text, &val, &precision, field);

      intmax_t padding;
      जर (valid_संख्या)
        valid_संख्या = prepare_padded_संख्या (val, precision, &padding);

      जर (valid_संख्या)
        print_padded_संख्या (padding);
      अन्यथा
        ख_माला_दो (text, मानक_निकास);
    }
  अन्यथा
    ख_माला_दो (text, मानक_निकास);

  परत valid_संख्या;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
स्थिर पूर्णांक
process_ओळ (अक्षर *ओळ, bool newline)
{
  अक्षर *next;
  uintmax_t field = 0;
  bool valid_संख्या = true;

  पर्यंत (true) {
    ++field;
    next = next_field (&ओळ);

    जर (*ओळ != '\0')
      {
        /* NUL terminate the current field string and process */
        अक्षर end_field = *ओळ;
        *ओळ = '\0';

        जर (! process_field (next, field))
          valid_संख्या = false;

        जर (delimiter != रिक्त)
          ख_माला_दो (delimiter, मानक_निकास);
        अन्यथा
          ख_अक्षर_दो (' ', मानक_निकास);

        जर (delimiter)
          ओळ += MAX (म_माप (delimiter), 1);
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
        जर (! process_field (next, field))
          valid_संख्या = false;

        थांबा;
      }
  }

  जर (newline)
    अक्षर_दो (ओळ_delim);

 जर (ख_त्रुटि (मानक_निकास))
   write_error ();

  परत valid_संख्या;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#जर HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#endif

  decimal_point = nl_langinfo (RADIXCHAR);
  जर (decimal_point == रिक्त || म_माप (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_length = म_माप (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  जर (thousands_sep == रिक्त)
    thousands_sep = "";
  thousands_sep_length = म_माप (thousands_sep);

  निकास_पर (close_stdout);

  पर्यंत (true)
    {
      पूर्णांक c = getopt_long (argc, argv, "d:z", longopts, रिक्त);

      जर (c == -1)
        थांबा;

      निवड (c)
        {
        स्थिती FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          थांबा;

        स्थिती FROM_UNIT_OPTION:
          from_unit_size = unit_to_umax (optarg);
          थांबा;

        स्थिती TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          थांबा;

        स्थिती TO_UNIT_OPTION:
          to_unit_size = unit_to_umax (optarg);
          थांबा;

        स्थिती ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          थांबा;

        स्थिती GROUPING_OPTION:
          grouping = 1;
          थांबा;

        स्थिती PADDING_OPTION:
          जर (((xstrtoimax (optarg, रिक्त, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (निकास_त्रुटि, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          थांबा;

        स्थिती FIELD_OPTION:
          जर (n_frp)
            error (निकास_त्रुटि, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          थांबा;

        स्थिती 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          जर (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              जर (optarg[g.len] != '\0')
                error (निकास_त्रुटि, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          थांबा;

        स्थिती 'z':
          ओळ_delim = '\0';
          थांबा;

        स्थिती SUFFIX_OPTION:
          suffix = optarg;
          थांबा;

        स्थिती UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          थांबा;

        स्थिती DEBUG_OPTION:
          debug = true;
          थांबा;

        स्थिती DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          थांबा;

        स्थिती HEADER_OPTION:
          जर (optarg)
            {
              जर (xstrtoumax (optarg, रिक्त, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (निकास_त्रुटि, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          अन्यथा
            {
              header = 1;
            }
          थांबा;

        स्थिती FORMAT_OPTION:
          format_str = optarg;
          थांबा;

        स्थिती INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          थांबा;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (format_str != रिक्त && grouping)
    error (निकास_त्रुटि, 0, _("--grouping cannot be combined with --format"));

  जर (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  जर (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == रिक्त))
    error (0, 0, _("no conversion option specified"));

  जर (debug && unit_separator && delimiter == रिक्त)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  जर (format_str)
    parse_format_string (format_str);

  जर (grouping)
    {
      जर (scale_to != scale_none)
        error (निकास_त्रुटि, 0, _("grouping cannot be combined with --to"));
      जर (debug && thousands_sep_length == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == रिक्त);

  जर (inval_style != inval_abort)
    conv_exit_code = 0;

  जर (argc > optind)
    {
      जर (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      साठी (; optind < argc; optind++)
        valid_numbers &= process_ओळ (argv[optind], true);
    }
  अन्यथा
    {
      अक्षर *ओळ = रिक्त;
      माप_प्रकार ओळ_allocated = 0;
      ssize_t len;

      पर्यंत (header-- && getdelim (&ओळ, &ओळ_allocated,
                                   ओळ_delim, मानक_निवेश) > 0)
        {
          जर (ख_माला_दो (ओळ, मानक_निकास) == खातापूर्ण)
            write_error ();
        }

      पर्यंत ((len = getdelim (&ओळ, &ओळ_allocated,
                              ओळ_delim, मानक_निवेश)) > 0)
        {
          bool newline = ओळ[len - 1] == ओळ_delim;
          जर (newline)
            ओळ[len - 1] = '\0';
          valid_numbers &= process_ओळ (ओळ, newline);
        }

      जर (ख_त्रुटि (मानक_निवेश))
        error (निकास_त्रुटि, त्रुटि_सं, _("error reading input"));
    }

  जर (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  पूर्णांक exit_status = निकास_सफल;
  जर (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
