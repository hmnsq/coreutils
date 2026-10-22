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
#اگر_ختم

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "numfmt"

#تعریف_کرو AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
شمار { EXIT_CONVERSION_WARNINGS = 2 };

شمار
{
  FROM_OPTION = CHAR_MAX + 1,
  FROM_UNIT_OPTION,
  مقصد_OPTION,
  مقصد_UNIT_OPTION,
  باری_OPTION,
  SUFFIX_OPTION,
  GROUPING_OPTION,
  PADDING_OPTION,
  میدان_OPTION,
  DEBUG_OPTION,
  DEV_DEBUG_OPTION,
  سرخی_OPTION,
  FORMAT_OPTION,
  INVALID_OPTION,
  UNIT_SEPARATOR_OPTION
};

شمار scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

ساکن حرف ثابت *ثابت scale_from_دلائل[] =
{
  "none", "auto", "si", "iec", "iec-i", تہی
};

ساکن شمار scale_type ثابت scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

ساکن حرف ثابت *ثابت scale_مقصد_دلائل[] =
{
  "none", "si", "iec", "iec-i", تہی
};

ساکن شمار scale_type ثابت scale_مقصد_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


شمار باری_type
{
  باری_ceiling,
  باری_floor,
  باری_from_zero,
  باری_مقصد_zero,
  باری_قریب_ترین,
};

ساکن حرف ثابت *ثابت باری_دلائل[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", تہی
};

ساکن شمار باری_type ثابت باری_types[] =
{
  باری_ceiling, باری_floor, باری_from_zero, باری_مقصد_zero, باری_قریب_ترین
};


شمار inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

ساکن حرف ثابت *ثابت inval_دلائل[] =
{
  "abort", "fail", "warn", "ignore", تہی
};

ساکن شمار inval_type ثابت inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

ساکن ساخت option ثابت longopts[] =
{
  {"from", required_دلیل, تہی, FROM_OPTION},
  {"from-unit", required_دلیل, تہی, FROM_UNIT_OPTION},
  {"to", required_دلیل, تہی, مقصد_OPTION},
  {"to-unit", required_دلیل, تہی, مقصد_UNIT_OPTION},
  {"round", required_دلیل, تہی, باری_OPTION},
  {"padding", required_دلیل, تہی, PADDING_OPTION},
  {"suffix", required_دلیل, تہی, SUFFIX_OPTION},
  {"unit-separator", required_دلیل, تہی, UNIT_SEPARATOR_OPTION},
  {"grouping", no_دلیل, تہی, GROUPING_OPTION},
  {"delimiter", required_دلیل, تہی, 'd'},
  {"field", required_دلیل, تہی, میدان_OPTION},
  {"debug", no_دلیل, تہی, DEBUG_OPTION},
  {"-debug", no_دلیل, تہی, DEV_DEBUG_OPTION},
  {"header", optional_دلیل, تہی, سرخی_OPTION},
  {"format", required_دلیل, تہی, FORMAT_OPTION},
  {"invalid", required_دلیل, تہی, INVALID_OPTION},
  {"zero-terminated", no_دلیل, تہی, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
شمار { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
شمار { MAX_ACCEPTABLE_DIGITS = 33 };

ساکن شمار scale_type scale_from = scale_none;
ساکن شمار scale_type scale_مقصد = scale_none;
ساکن شمار باری_type باری_style = باری_from_zero;
ساکن شمار inval_type inval_style = inval_abort;
ساکن حرف ثابت *suffix = تہی;
ساکن حرف ثابت *unit_separator = تہی;
ساکن uintmax_t from_unit_سائز = 1;
ساکن uintmax_t مقصد_unit_سائز = 1;
ساکن صحیح grouping = 0;
ساکن حرف *padding_بفر = تہی;
ساکن idx_t padding_بفر_سائز = 0;
ساکن intmax_t padding_چوڑائی = 0;
ساکن صحیح zero_padding_چوڑائی = 0;
ساکن طویل صحیح user_precision = -1;
ساکن حرف ثابت *format_str = تہی;
ساکن حرف *format_str_سابقہ = تہی;
ساکن حرف *format_str_suffix = تہی;

/* By default, any conversion error will terminate the program.  */
ساکن صحیح conv_exit_رمز = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
ساکن صحیح auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
ساکن حرف ثابت *delimiter = تہی;

/* line delimiter.  */
ساکن غیرعلامتی حرف سطر_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
ساکن uintmax_t سرخی = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
ساکن bool debug;

/* will be set according to the current locale.  */
ساکن حرف ثابت *decimal_نقطہ;
ساکن صحیح decimal_نقطہ_لمبائی;
ساکن حرف ثابت *thousands_sep;
ساکن صحیح thousands_sep_لمبائی;

/* debugging for developers.  Enables devmsg().  */
ساکن bool dev_debug = false;

ساکن bool
newline_or_blank (mcel_t g)
{
  واپس g.ch == '\n' || c32issep (g.ch);
}

ساکن اندرونی صحیح
default_scale_بنیاد (شمار scale_type scale)
{
  انتخاب (scale)
    {
    حالت scale_IEC:
    حالت scale_IEC_I:
      واپس 1024;

    حالت scale_none:
    حالت scale_auto:
    حالت scale_SI:
    طےشدہ:
      واپس 1000;
    }
}

ساکن حرف ثابت zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
ساکن حرف ثابت *valid_suffixes = 1 + zero_and_valid_suffixes;

ساکن اندرونی bool
valid_suffix (ثابت حرف suf)
{
  واپس strchr (valid_suffixes, suf) != تہی;
}

ساکن اندرونی صحیح
suffix_power (ثابت حرف suf)
{
  انتخاب (suf)
    {
    حالت 'k':                  /* kilo.  */
    حالت 'K':                  /* kilo or kibi.  */
      واپس 1;

    حالت 'M':                  /* mega or mebi.  */
      واپس 2;

    حالت 'G':                  /* giga or gibi.  */
      واپس 3;

    حالت 'T':                  /* tera or tebi.  */
      واپس 4;

    حالت 'P':                  /* peta or pebi.  */
      واپس 5;

    حالت 'E':                  /* exa or exbi.  */
      واپس 6;

    حالت 'Z':                  /* zetta or 2**70.  */
      واپس 7;

    حالت 'Y':                  /* yotta or 2**80.  */
      واپس 8;

    حالت 'R':                  /* ronna or 2**90.  */
      واپس 9;

    حالت 'Q':                  /* quetta or 2**100.  */
      واپس 10;

    طےشدہ:                   /* should never happen. assert?  */
      واپس 0;
    }
}

ساکن اندرونی حرف ثابت *
suffix_power_char (صحیح power)
{
  انتخاب (power)
    {
    حالت 0:
      واپس "";

    حالت 1:
      واپس "K";

    حالت 2:
      واپس "M";

    حالت 3:
      واپس "G";

    حالت 4:
      واپس "T";

    حالت 5:
      واپس "P";

    حالت 6:
      واپس "E";

    حالت 7:
      واپس "Z";

    حالت 8:
      واپس "Y";

    حالت 9:
      واپس "R";

    حالت 10:
      واپس "Q";

    طےشدہ:
      واپس "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
ساکن طویل دوہرا
powerld (طویل دوہرا بنیاد_2, ptrdiff_t x)
{
  طویل دوہرا نتیجہ = بنیاد_2;
  اگر (x == 0)
    واپس 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  جب (--x)
    نتیجہ *= بنیاد_2;
  واپس نتیجہ;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
ساکن اندرونی طویل دوہرا
absld (طویل دوہرا val)
{
  واپس val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
ساکن طویل دوہرا
expld (طویل دوہرا val, صحیح بنیاد_2, صحیح /*output */ *x)
{
  صحیح power = 0;

  اگر (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      جب (absld (val) >= بنیاد_2)
        {
          ++power;
          val /= بنیاد_2;
        }
    }
  اگر (x)
    *x = power;
  واپس val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ساکن اندرونی intmax_t
simple_باری_ceiling (طویل دوہرا val)
{
  intmax_t intval = val;
  اگر (intval < val)
    intval++;
  واپس intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ساکن اندرونی intmax_t
simple_باری_floor (طویل دوہرا val)
{
  واپس -simple_باری_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
ساکن اندرونی intmax_t
simple_باری_from_zero (طویل دوہرا val)
{
  واپس val < 0 ? simple_باری_floor (val) : simple_باری_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
ساکن اندرونی intmax_t
simple_باری_مقصد_zero (طویل دوہرا val)
{
  واپس val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ساکن اندرونی intmax_t
simple_باری_قریب_ترین (طویل دوہرا val)
{
  واپس val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
ساکن اندرونی طویل دوہرا
simple_باری (طویل دوہرا val, شمار باری_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (طویل دوہرا) INTMAX_MAX * intmax_mul;

  انتخاب (t)
    {
    حالت باری_ceiling:
      rval = simple_باری_ceiling (val);
      توڑو;

    حالت باری_floor:
      rval = simple_باری_floor (val);
      توڑو;

    حالت باری_from_zero:
      rval = simple_باری_from_zero (val);
      توڑو;

    حالت باری_مقصد_zero:
      rval = simple_باری_مقصد_zero (val);
      توڑو;

    حالت باری_قریب_ترین:
      rval = simple_باری_قریب_ترین (val);
      توڑو;

    طےشدہ:
      /* to silence the compiler - this should never happen.  */
      واپس 0;
    }

  واپس (طویل دوہرا) INTMAX_MAX * intmax_mul + rval;
}

شمار simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_حد_سے_تجاوز,
  SSE_INVALID_نمبر,

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
ساکن شمار simple_strtod_error
simple_strtod_int (حرف ثابت *داخلہ_str,
                   حرف **endptr, طویل دوہرا *قدر, bool *negative)
{
  شمار simple_strtod_error e = SSE_OK;

  طویل دوہرا val = 0;
  صحیح digits = 0;
  bool ملا_digit = false;

  اگر (*داخلہ_str == '-')
    {
      داخلہ_str++;
      *negative = true;
    }
  ورنہ
    *negative = false;

  *endptr = (حرف *) داخلہ_str;
  جب (c_isdigit (**endptr))
    {
      صحیح digit = (**endptr) - '0';

      ملا_digit = true;

      اگر (val || digit)
        digits++;

      اگر (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      اگر (digits > MAX_ACCEPTABLE_DIGITS)
        واپس SSE_حد_سے_تجاوز;

      val *= 10;
      val += digit;

      ++(*endptr);

      اگر (thousands_sep_لمبائی > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_لمبائی)
          && c_isdigit ((*endptr)[thousands_sep_لمبائی]))
        (*endptr) += thousands_sep_لمبائی;
    }
  اگر (! ملا_digit
      && ! STREQ_LEN (*endptr, decimal_نقطہ, decimal_نقطہ_لمبائی))
    واپس SSE_INVALID_نمبر;
  اگر (*negative)
    val = -val;

  اگر (قدر)
    *قدر = val;

  واپس e;
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
ساکن شمار simple_strtod_error
simple_strtod_float (حرف ثابت *داخلہ_str,
                     حرف **endptr,
                     طویل دوہرا *قدر,
                     size_t *precision)
{
  bool negative;
  شمار simple_strtod_error e = SSE_OK;

  اگر (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (داخلہ_str, endptr, قدر, &negative);
  اگر (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    واپس e;

  /* optional decimal point + fraction.  */
  اگر (STREQ_LEN (*endptr, decimal_نقطہ, decimal_نقطہ_لمبائی))
    {
      حرف *ptr2;
      طویل دوہرا val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_نقطہ_لمبائی;
      شمار simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      اگر (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        واپس e2;
      اگر (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      اگر (neg_frac)
        واپس SSE_INVALID_نمبر;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      اگر (قدر)
        {
          اگر (negative)
            *قدر -= val_frac;
          ورنہ
            *قدر += val_frac;
        }

      اگر (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  واپس e;
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
ساکن شمار simple_strtod_error
simple_strtod_human (حرف ثابت *داخلہ_str,
                     حرف **endptr, طویل دوہرا *قدر, size_t *precision,
                     شمار scale_type allowed_scaling)
{
  صحیح power = 0;
  /* 'scale_auto' is checked below.  */
  صحیح scale_بنیاد = default_scale_بنیاد (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, داخلہ_str),
          quote_n (1, decimal_نقطہ),
          MAX_UNSCALED_DIGITS);

  شمار simple_strtod_error e =
    simple_strtod_float (داخلہ_str, endptr, قدر, precision);
  اگر (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    واپس e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *قدر, (صحیح)*precision);

  جب (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      اگر (unit_separator)
        {
          size_t sep_len = لڑی_لمبائی (unit_separator);
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
        توڑو;  /* Treat as no suffix.  */

      اگر (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          اگر (**endptr == '\0')
            توڑو;

          واپس SSE_INVALID_SUFFIX;
        }

      اگر (allowed_scaling == scale_none)
        واپس SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      اگر (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_بنیاد = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_بنیاد);
        }
      ورنہ اگر (allowed_scaling == scale_IEC_I)
        {
          اگر (**endptr == 'i')
            (*endptr)++;
          ورنہ
            واپس SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      توڑو;
    }

  طویل دوہرا multiplier = powerld (scale_بنیاد, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_بنیاد, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*قدر) = (*قدر) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *قدر, *قدر);

  واپس e;
}


ساکن خالی
simple_strtod_fatal (شمار simple_strtod_error err, حرف ثابت *داخلہ_str)
{
  حرف ثابت *msgid = تہی;

  انتخاب (err)
    {
    حالت SSE_حد_سے_تجاوز:
      msgid = N_("value too large to be converted: %s");
      توڑو;

    حالت SSE_INVALID_نمبر:
      msgid = N_("invalid number: %s");
      توڑو;

    حالت SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      توڑو;

    حالت SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      توڑو;

    حالت SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      توڑو;

    حالت SSE_OK_PRECISION_LOSS:
    حالت SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  اگر (inval_style != inval_ignore)
    error (conv_exit_رمز, 0, gettext (msgid), quote (داخلہ_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
ساکن صحیح
double_مقصد_human (طویل دوہرا val, صحیح precision,
                 حرف *buf, idx_t buf_سائز,
                 شمار scale_type scale, صحیح گروہ, شمار باری_type باری_2)
{
  حرف fmt[حجم "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_چوڑائی)];
  حرف *pfmt = fmt;
  *pfmt++ = '%';

  اگر (گروہ)
    *pfmt++ = '\'';

  اگر (zero_padding_چوڑائی)
    pfmt += sprintf (pfmt, "0%d", zero_padding_چوڑائی);

  devmsg ("double_to_human:\n");

  اگر (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_باری (val, باری_2);
      val /= powerld (10, precision);

      devmsg ((گروہ) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      واپس snprintf (buf, buf_سائز, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  دوہرا scale_بنیاد = default_scale_بنیاد (scale);

  /* Normalize val to scale. */
  صحیح power = 0;
  val = expld (val, scale_بنیاد, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_بنیاد, power);

  /* Perform rounding. */
  صحیح power_adjust = 0;
  اگر (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  ورنہ اگر (absld (val) < 10)
    {
      /* for values less than 10, we allow one decimal-point digit,
         so adjust before rounding. */
      power_adjust = 1;
    }

  val *= powerld (10, power_adjust);
  val = simple_باری (val, باری_2);
  val /= powerld (10, power_adjust);

  /* two special cases after rounding:
     1. a "999.99" can turn into 1000 - so scale down
     2. a "9.99" can turn into 10 - so don't display decimal-point.  */
  اگر (absld (val) >= scale_بنیاد)
    {
      val /= scale_بنیاد;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  صحیح show_decimal_نقطہ = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_بنیاد, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  صحیح prec = user_precision == -1 ? show_decimal_نقطہ : user_precision;

  واپس snprintf (buf, buf_سائز, fmt, prec, val,
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
ساکن uintmax_t
unit_مقصد_umax (حرف ثابت *n_string)
{
  strtol_error s_err;
  حرف ثابت *c_string = n_string;
  حرف *t_string = تہی;
  size_t n_len = لڑی_لمبائی (n_string);
  حرف *انتہا = تہی;
  uintmax_t n;
  حرف ثابت *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  اگر (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      انتہا = t_string + n_len - 1;
      حافظہ_نقل (t_string, n_string, n_len);

      اگر (*انتہا == 'i' && 2 <= n_len && ! c_isdigit (*(انتہا - 1)))
        *انتہا = '\0';
      ورنہ
        {
          *++انتہا = 'B';
          *++انتہا = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &انتہا, 10, &n, suffixes);

  اگر (s_err != LONGINT_OK || *انتہا || n == 0)
    {
      آزاد (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  آزاد (t_string);

  واپس n;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [نمبر_2]...\n\
"), پروگرام_name);
      fputs (_("\
Reformat نمبر_2(s), or the numbers from standard داخلہ اگر none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid داخلہ\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead کا whitespace برائے میدان delimiter\n\
"));
      oputs (_("\
      --میدان=FIELDS\n\
         replace the numbers اندر these داخلہ fields (طےشدہ=1);\n\
         see FIELDS نیچے برائے details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use لکھو style floating-نقطہ FORMAT;\n\
         see FORMAT نیچے برائے details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         خودکار-scale داخلہ numbers مقصد UNITs; طےشدہ is 'none';\n\
         see UNIT نیچے برائے details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the داخلہ unit سائز (instead کا the طےشدہ 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping کا digits, e.g. 1,000,000.\n\
         This has no effect اندر the C/POSIX locale\n\
"));
      oputs (_("\
      --سرخی[=N]\n\
         print (without converting) the پہلا N سرخی سطریں;\n\
         N defaults مقصد 1 اگر not specified\n\
"));
      oputs (_("\
      --invalid=منوال\n\
         failure منوال_2 برائے invalid numbers;\n\
         منوال can be: abort (طےشدہ), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output مقصد N characters;\n\
         مثبت N will دایاں-align, negative N will بایاں-align;\n\
         padding is ignored اگر the output is wider than N;\n\
         the طےشدہ is مقصد automatically pad اگر a whitespace is ملا\n\
"));
      oputs (_("\
      --باری_2=طریقہ\n\
         use طریقہ برائے rounding when scaling; طریقہ can be:\n\
         اوپر, نیچے_کو, from-zero (طےشدہ), towards-zero, قریب_ترین\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         جوڑو SUFFIX مقصد output numbers,\n\
         and accept an optional SUFFIX اندر داخلہ numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         داخل_کرو SEP between نمبر and unit on output,\n\
         and accept an optional SEP اندر داخلہ numbers\n\
"));
      oputs (_("\
      --مقصد=UNIT\n\
         خودکار-scale output numbers مقصد UNITs; see UNIT نیچے\n\
"));
      oputs (_("\
      --مقصد-unit=N\n\
         the output unit سائز (instead کا the طےشدہ 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);

      fputs (_("\
\n\
UNIT options:\n"), stdout);
      fputs (_("\
  none       no خودکار-scaling is مکمل; suffixes will trigger an error\n\
"), stdout);
      fputs (_("\
  خودکار       accept optional single/two letter suffix:\n\
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
FIELDS supports cut(1) style میدان ranges:\n\
  N    N'th میدان, counted from 1\n\
  N-   from N'th میدان, مقصد انتہا کا سطر\n\
  N-M  from N'th to M'th میدان (inclusive)\n\
  -M   from پہلا مقصد M'th میدان (inclusive)\n\
  -    all fields\n\
مضرب_2 fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable برائے printing one floating-نقطہ دلیل '%f'.\n\
Optional quote (%'f) will enable --grouping (اگر supported by current locale).\n\
Optional چوڑائی قدر (%10f) will pad output. Optional zero (%010f) چوڑائی\n\
will zero pad the نمبر. Optional negative اقدار (%-10f) will بایاں align.\n\
Optional precision (%.1f) will override the داخلہ determined precision.\n\
"), stdout);

      لکھو (_("\n\
Exit حالت_2 is 0 اگر all داخلہ numbers were successfully converted.\n\
By طےشدہ, %s will stop at the پہلا conversion error with باہر_نکلو حالت_2 2.\n\
With --invalid='fail' a warning is printed برائے each conversion error\n\
and the باہر_نکلو حالت_2 is 2.  With --invalid='warn' each conversion error is\n\
diagnosed, but the باہر_نکلو حالت_2 is 0.  With --invalid='ignore' conversion\n\
errors are not diagnosed and the باہر_نکلو حالت_2 is 0.\n\
"), پروگرام_name);

      لکھو (_("\n\
Examples:\n\
  $ %s --مقصد=si 1000\n\
            -> \"1.0k\"\n\
  $ %s --مقصد=iec 2048\n\
           -> \"2.0K\"\n\
  $ %s --مقصد=iec-i 4096\n\
           -> \"4.0Ki\"\n\
  $ echo 1K | %s --from=si\n\
           -> \"1000\"\n\
  $ echo 1K | %s --from=iec\n\
           -> \"1024\"\n\
  $ df -B1 | %s --سرخی --میدان 2-4 --مقصد=si\n\
  $ ls -l  | %s --سرخی --میدان 5 --مقصد=iec\n\
  $ ls -lh | %s --سرخی --میدان 5 --from=iec --padding=10\n\
  $ ls -lh | %s --سرخی --میدان 5 --from=iec --format %%10f\n"),
              پروگرام_name, پروگرام_name, پروگرام_name,
              پروگرام_name, پروگرام_name, پروگرام_name,
              پروگرام_name, پروگرام_name, پروگرام_name);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
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
ساکن خالی
parse_format_string (حرف ثابت *fmt)
{
  size_t i;
  size_t سابقہ_len = 0;
  size_t suffix_pos;
  حرف *endptr = تہی;
  bool zero_padding = false;

  برائے (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      اگر (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      سابقہ_len++;
    }

  i++;
  جب (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      اگر (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      ورنہ اگر (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      ورنہ اگر (! skip)
        توڑو;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  اگر (pad != 0)
    {
      اگر (debug && padding_چوڑائی && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      اگر (pad < 0)
        padding_چوڑائی = pad;
      ورنہ
        {
          اگر (zero_padding)
            zero_padding_چوڑائی = MIN (pad, زیادہ_سے_زیادہ_عدد);
          ورنہ
            padding_چوڑائی = pad;
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

  برائے (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    اگر (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  اگر (سابقہ_len)
    format_str_سابقہ = ximemdup0 (fmt, سابقہ_len);
  اگر (fmt[suffix_pos] != '\0')
    format_str_suffix = xstrdup (fmt + suffix_pos);

  devmsg ("format String:\n  input: %s\n  grouping: %s\n"
                   "  padding width: %jd\n"
                   "  prefix: %s\n  suffix: %s\n",
          quote_n (0, fmt), (grouping) ? "yes" : "no",
          padding_چوڑائی,
          quote_n (1, format_str_سابقہ ? format_str_سابقہ : ""),
          quote_n (2, format_str_suffix ? format_str_suffix : ""));
}

/* Parse a numeric value (with optional suffix) from a string.
   Returns a long double value, with input precision.

   If there's an error converting the string to value - exits with
   an error.

   If there are any trailing characters after the number
   (besides a valid suffix) - exits with an error.  */
ساکن شمار simple_strtod_error
parse_human_نمبر (حرف ثابت *str, طویل دوہرا /*output */ *قدر,
                    size_t *precision)
{
  حرف *ptr = تہی;

  شمار simple_strtod_error e =
    simple_strtod_human (str, &ptr, قدر, precision, scale_from);
  اگر (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      واپس e;
    }

  اگر (ptr && *ptr != '\0')
    {
      اگر (inval_style != inval_ignore)
        error (conv_exit_رمز, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  واپس e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
ساکن bool
prepare_padded_نمبر (ثابت طویل دوہرا val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_مستعمل = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  صحیح x;
  expld (val, 10, &x);

  اگر (scale_مقصد == scale_none
      && x + precision_مستعمل > MAX_UNSCALED_DIGITS)
    {
      اگر (inval_style != inval_ignore)
        {
          اگر (precision_مستعمل)
            error (conv_exit_رمز, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_مستعمل);
          ورنہ
            error (conv_exit_رمز, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      واپس false;
    }

  اگر (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      اگر (inval_style != inval_ignore)
        error (conv_exit_رمز, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      واپس false;
    }

  جب (true)
    {
      صحیح numlen = double_مقصد_human (val, precision_مستعمل,
                                    padding_بفر, padding_بفر_سائز,
                                    scale_مقصد, grouping, باری_style);
      ptrdiff_t growth;
      اگر (numlen < 0 || ckd_sub (&growth, numlen, padding_بفر_سائز - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      اگر (growth <= 0)
        توڑو;
      padding_بفر = xpalloc (padding_بفر, &padding_بفر_سائز,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_بفر));

  intmax_t pad = 0;
  اگر (padding_چوڑائی)
    {
      صحیح buf_چوڑائی = mbswidth (padding_بفر,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      اگر (0 <= buf_چوڑائی)
        {
          اگر (padding_چوڑائی < 0)
            {
              اگر (padding_چوڑائی < -buf_چوڑائی)
                pad = padding_چوڑائی + buf_چوڑائی;
            }
          ورنہ
            {
              اگر (buf_چوڑائی < padding_چوڑائی)
                pad = padding_چوڑائی - buf_چوڑائی;
            }
        }
    }

  *padding = pad;
  واپس true;
}

ساکن خالی
print_padded_نمبر (intmax_t padding)
{
  اگر (format_str_سابقہ)
    fputs (format_str_سابقہ, stdout);

  برائے (intmax_t p = padding; 0 < p; p--)
    حرف_لکھو (' ');

  fputs (padding_بفر, stdout);

  برائے (intmax_t p = padding; p < 0; p++)
    حرف_لکھو (' ');

  اگر (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
ساکن bool
process_suffixed_نمبر (حرف *متن, طویل دوہرا *نتیجہ,
                         size_t *precision, طویل صحیح میدان)
{
  حرف saved_suffix = '\0';

  اگر (suffix)
    {
      اگر (mbs_endswith (متن, suffix))
        {
          saved_suffix = *(متن + لڑی_لمبائی (متن) - لڑی_لمبائی (suffix));
          *(متن + لڑی_لمبائی (متن) - لڑی_لمبائی (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      ورنہ
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  حرف *p = skip_str_matching (متن, newline_or_blank, true);

  /* setup auto-padding.  */
  اگر (auto_padding)
    {
      padding_چوڑائی = متن < p || 1 < میدان
                      ? mbswidth (متن,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      اگر (padding_چوڑائی < 0)
        padding_چوڑائی = لڑی_لمبائی (متن);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_چوڑائی);
    }

  طویل دوہرا val = 0;
  شمار simple_strtod_error e = parse_human_نمبر (p, &val, precision);
  اگر (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  اگر (from_unit_سائز != 1 || مقصد_unit_سائز != 1)
    val = (val * from_unit_سائز) / مقصد_unit_سائز;

  *نتیجہ = val;

  اگر (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    واپس true;
  ورنہ
    {
      اگر (saved_suffix)
        *(متن + لڑی_لمبائی (متن)) = saved_suffix;
      واپس false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_خالص
ساکن حرف *
mbsmbchr (حرف ثابت *s, حرف ثابت *c)
{
  غیرعلامتی حرف uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  اگر (uc < 0x30 || MB_CUR_MAX == 1)
    واپس (حرف *) strchr (s, uc);
  ورنہ اگر (is_utf8_charset ())
    واپس (حرف *) (uc < 0x80 ? strchr (s, uc) : لڑی_تلاش (s, c));
  ورنہ
    واپس *(c + 1) == '\0' ? mbschr (s, uc) : (حرف *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
ساکن حرف *
اگلا_میدان (حرف **سطر)
{
  حرف *میدان_آغاز = *سطر;
  حرف *میدان_انتہا   = میدان_آغاز;

  اگر (delimiter)
    {
      اگر (! (میدان_انتہا = mbsmbchr (میدان_آغاز, delimiter)))
        میدان_انتہا = strchr (میدان_آغاز, '\0');
    }
  ورنہ
    {
      /* keep any space prefix in the returned field */
      میدان_انتہا = skip_str_matching (میدان_انتہا, newline_or_blank, true);
      میدان_انتہا = skip_str_matching (میدان_انتہا, newline_or_blank, false);
    }

  *سطر = میدان_انتہا;
  واپس میدان_آغاز;
}

ATTRIBUTE_خالص
ساکن bool
include_میدان (uintmax_t میدان)
{
  ساخت میدان_range_pair *p = frp;
  اگر (!p)
    واپس میدان == 1;

  جب (p->lo != UINTMAX_MAX)
    {
      اگر (p->lo <= میدان && p->hi >= میدان)
        واپس true;
      ++p;
    }
  واپس false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
ساکن bool
process_میدان (حرف *متن, uintmax_t میدان)
{
  طویل دوہرا val = 0;
  size_t precision = 0;
  bool valid_نمبر = true;

  اگر (include_میدان (میدان))
    {
      valid_نمبر =
        process_suffixed_نمبر (متن, &val, &precision, میدان);

      intmax_t padding;
      اگر (valid_نمبر)
        valid_نمبر = prepare_padded_نمبر (val, precision, &padding);

      اگر (valid_نمبر)
        print_padded_نمبر (padding);
      ورنہ
        fputs (متن, stdout);
    }
  ورنہ
    fputs (متن, stdout);

  واپس valid_نمبر;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
ساکن صحیح
process_سطر (حرف *سطر, bool newline)
{
  حرف *اگلا;
  uintmax_t میدان = 0;
  bool valid_نمبر = true;

  جب (true) {
    ++میدان;
    اگلا = اگلا_میدان (&سطر);

    اگر (*سطر != '\0')
      {
        /* NUL terminate the current field string and process */
        حرف انتہا_میدان = *سطر;
        *سطر = '\0';

        اگر (! process_میدان (اگلا, میدان))
          valid_نمبر = false;

        اگر (delimiter != تہی)
          fputs (delimiter, stdout);
        ورنہ
          fputc (' ', stdout);

        اگر (delimiter)
          سطر += MAX (لڑی_لمبائی (delimiter), 1);
        ورنہ
          {
            *سطر = انتہا_میدان;
            mcel_t g = mcel_scanz (سطر);
            سطر += g.len;
          }
      }
    ورنہ
      {
        /* end of the line, process the last field and finish */
        اگر (! process_میدان (اگلا, میدان))
          valid_نمبر = false;

        توڑو;
      }
  }

  اگر (newline)
    حرف_لکھو (سطر_delim);

 اگر (ferror (stdout))
   write_error ();

  واپس valid_نمبر;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#اگر HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#اگر_ختم

  decimal_نقطہ = nl_langinfo (RADIXCHAR);
  اگر (decimal_نقطہ == تہی || لڑی_لمبائی (decimal_نقطہ) == 0)
    decimal_نقطہ = ".";
  decimal_نقطہ_لمبائی = لڑی_لمبائی (decimal_نقطہ);

  thousands_sep = nl_langinfo (THOUSEP);
  اگر (thousands_sep == تہی)
    thousands_sep = "";
  thousands_sep_لمبائی = لڑی_لمبائی (thousands_sep);

  atexit (close_stdout);

  جب (true)
    {
      صحیح c = getopt_long (argc, argv, "d:z", longopts, تہی);

      اگر (c == -1)
        توڑو;

      انتخاب (c)
        {
        حالت FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_دلائل, scale_from_types);
          توڑو;

        حالت FROM_UNIT_OPTION:
          from_unit_سائز = unit_مقصد_umax (optarg);
          توڑو;

        حالت مقصد_OPTION:
          scale_مقصد =
            XARGMATCH ("--to", optarg, scale_مقصد_دلائل, scale_مقصد_types);
          توڑو;

        حالت مقصد_UNIT_OPTION:
          مقصد_unit_سائز = unit_مقصد_umax (optarg);
          توڑو;

        حالت باری_OPTION:
          باری_style = XARGMATCH ("--round", optarg, باری_دلائل, باری_types);
          توڑو;

        حالت GROUPING_OPTION:
          grouping = 1;
          توڑو;

        حالت PADDING_OPTION:
          اگر (((xstrtoimax (optarg, تہی, 10, &padding_چوڑائی, "")
                & ~LONGINT_حد_سے_تجاوز)
               != LONGINT_OK)
              || padding_چوڑائی == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          توڑو;

        حالت میدان_OPTION:
          اگر (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          سیٹ_fields (optarg, SETFLD_ALLOW_DASH);
          توڑو;

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
          توڑو;

        حالت 'z':
          سطر_delim = '\0';
          توڑو;

        حالت SUFFIX_OPTION:
          suffix = optarg;
          توڑو;

        حالت UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          توڑو;

        حالت DEBUG_OPTION:
          debug = true;
          توڑو;

        حالت DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          توڑو;

        حالت سرخی_OPTION:
          اگر (optarg)
            {
              اگر (xstrtoumax (optarg, تہی, 10, &سرخی, "") != LONGINT_OK
                  || سرخی == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          ورنہ
            {
              سرخی = 1;
            }
          توڑو;

        حالت FORMAT_OPTION:
          format_str = optarg;
          توڑو;

        حالت INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_دلائل, inval_types);
          توڑو;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (format_str != تہی && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  اگر (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  اگر (debug && scale_from == scale_none && scale_مقصد == scale_none
      && !grouping && (padding_چوڑائی == 0) && (format_str == تہی))
    error (0, 0, _("no conversion option specified"));

  اگر (debug && unit_separator && delimiter == تہی)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  اگر (format_str)
    parse_format_string (format_str);

  اگر (grouping)
    {
      اگر (scale_مقصد != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      اگر (debug && thousands_sep_لمبائی == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_چوڑائی == 0 && delimiter == تہی);

  اگر (inval_style != inval_abort)
    conv_exit_رمز = 0;

  اگر (argc > optind)
    {
      اگر (debug && سرخی)
        error (0, 0, _("--header ignored with command-line input"));

      برائے (; optind < argc; optind++)
        valid_numbers &= process_سطر (argv[optind], true);
    }
  ورنہ
    {
      حرف *سطر = تہی;
      size_t سطر_allocated = 0;
      ssize_t len;

      جب (سرخی-- && getdelim (&سطر, &سطر_allocated,
                                   سطر_delim, stdin) > 0)
        {
          اگر (fputs (سطر, stdout) == EOF)
            write_error ();
        }

      جب ((len = getdelim (&سطر, &سطر_allocated,
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

  صحیح exit_حالت = EXIT_SUCCESS;
  اگر (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_حالت = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_حالت);
}
