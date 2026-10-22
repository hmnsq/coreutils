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

#ضمن <config.h>
#ضمن <float.h>
#ضمن <getopt.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <langinfo.h>

#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "mbswidth.h"
#ضمن "mcel.h"
#ضمن "quote.h"
#ضمن "skipchars.h"
#ضمن "system.h"
#ضمن "utf8.h"
#ضمن "xstrtol.h"

#ضمن "set-fields.h"

#إذا HAVE_FPSETPREC
# include <ieeefp.h>
#پایان_اگر

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "numfmt"

#تعریف_کن AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
تعداد { EXIT_CONVERSION_WARNINGS = 2 };

تعداد
{
  FROM_OPTION = CHAR_MAX + 1,
  FROM_UNIT_OPTION,
  مقصد_OPTION,
  مقصد_UNIT_OPTION,
  دور_OPTION,
  SUFFIX_OPTION,
  GROUPING_OPTION,
  PADDING_OPTION,
  فیلد_OPTION,
  DEBUG_OPTION,
  DEV_DEBUG_OPTION,
  سرآیند_OPTION,
  FORMAT_OPTION,
  INVALID_OPTION,
  UNIT_SEPARATOR_OPTION
};

تعداد scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

ساكن حرف ثابت *ثابت scale_from_آرگومانها[] =
{
  "none", "auto", "si", "iec", "iec-i", عدم
};

ساكن تعداد scale_type ثابت scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

ساكن حرف ثابت *ثابت scale_مقصد_آرگومانها[] =
{
  "none", "si", "iec", "iec-i", عدم
};

ساكن تعداد scale_type ثابت scale_مقصد_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


تعداد دور_type
{
  دور_ceiling,
  دور_floor,
  دور_from_zero,
  دور_مقصد_zero,
  دور_نزدیکترین,
};

ساكن حرف ثابت *ثابت دور_آرگومانها[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", عدم
};

ساكن تعداد دور_type ثابت دور_types[] =
{
  دور_ceiling, دور_floor, دور_from_zero, دور_مقصد_zero, دور_نزدیکترین
};


تعداد inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

ساكن حرف ثابت *ثابت inval_آرگومانها[] =
{
  "abort", "fail", "warn", "ignore", عدم
};

ساكن تعداد inval_type ثابت inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

ساكن بنية option ثابت longopts[] =
{
  {"from", required_آرگومان, عدم, FROM_OPTION},
  {"from-unit", required_آرگومان, عدم, FROM_UNIT_OPTION},
  {"to", required_آرگومان, عدم, مقصد_OPTION},
  {"to-unit", required_آرگومان, عدم, مقصد_UNIT_OPTION},
  {"round", required_آرگومان, عدم, دور_OPTION},
  {"padding", required_آرگومان, عدم, PADDING_OPTION},
  {"suffix", required_آرگومان, عدم, SUFFIX_OPTION},
  {"unit-separator", required_آرگومان, عدم, UNIT_SEPARATOR_OPTION},
  {"grouping", no_آرگومان, عدم, GROUPING_OPTION},
  {"delimiter", required_آرگومان, عدم, 'd'},
  {"field", required_آرگومان, عدم, فیلد_OPTION},
  {"debug", no_آرگومان, عدم, DEBUG_OPTION},
  {"-debug", no_آرگومان, عدم, DEV_DEBUG_OPTION},
  {"header", optional_آرگومان, عدم, سرآیند_OPTION},
  {"format", required_آرگومان, عدم, FORMAT_OPTION},
  {"invalid", required_آرگومان, عدم, INVALID_OPTION},
  {"zero-terminated", no_آرگومان, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
تعداد { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
تعداد { MAX_ACCEPTABLE_DIGITS = 33 };

ساكن تعداد scale_type scale_from = scale_none;
ساكن تعداد scale_type scale_مقصد = scale_none;
ساكن تعداد دور_type دور_style = دور_from_zero;
ساكن تعداد inval_type inval_style = inval_abort;
ساكن حرف ثابت *suffix = عدم;
ساكن حرف ثابت *unit_separator = عدم;
ساكن uintmax_t from_unit_اندازه = 1;
ساكن uintmax_t مقصد_unit_اندازه = 1;
ساكن صحيح grouping = 0;
ساكن حرف *padding_میانگیر = عدم;
ساكن idx_t padding_میانگیر_اندازه = 0;
ساكن intmax_t padding_عرض = 0;
ساكن صحيح zero_padding_عرض = 0;
ساكن دراز صحيح user_precision = -1;
ساكن حرف ثابت *format_str = عدم;
ساكن حرف *format_str_پیشوند = عدم;
ساكن حرف *format_str_suffix = عدم;

/* By default, any conversion error will terminate the program.  */
ساكن صحيح conv_exit_کد = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
ساكن صحيح auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
ساكن حرف ثابت *delimiter = عدم;

/* line delimiter.  */
ساكن بیعلامت حرف سطر_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
ساكن uintmax_t سرآیند = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
ساكن bool debug;

/* will be set according to the current locale.  */
ساكن حرف ثابت *decimal_نقطه;
ساكن صحيح decimal_نقطه_درازی;
ساكن حرف ثابت *thousands_sep;
ساكن صحيح thousands_sep_درازی;

/* debugging for developers.  Enables devmsg().  */
ساكن bool dev_debug = false;

ساكن bool
newline_or_blank (mcel_t g)
{
  أرجع g.ch == '\n' || c32issep (g.ch);
}

ساكن مضمن صحيح
default_scale_پایه (تعداد scale_type scale)
{
  بدل (scale)
    {
    حالة scale_IEC:
    حالة scale_IEC_I:
      أرجع 1024;

    حالة scale_none:
    حالة scale_auto:
    حالة scale_SI:
    افتراضي:
      أرجع 1000;
    }
}

ساكن حرف ثابت zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
ساكن حرف ثابت *valid_suffixes = 1 + zero_and_valid_suffixes;

ساكن مضمن bool
valid_suffix (ثابت حرف suf)
{
  أرجع strchr (valid_suffixes, suf) != عدم;
}

ساكن مضمن صحيح
suffix_power (ثابت حرف suf)
{
  بدل (suf)
    {
    حالة 'k':                  /* kilo.  */
    حالة 'K':                  /* kilo or kibi.  */
      أرجع 1;

    حالة 'M':                  /* mega or mebi.  */
      أرجع 2;

    حالة 'G':                  /* giga or gibi.  */
      أرجع 3;

    حالة 'T':                  /* tera or tebi.  */
      أرجع 4;

    حالة 'P':                  /* peta or pebi.  */
      أرجع 5;

    حالة 'E':                  /* exa or exbi.  */
      أرجع 6;

    حالة 'Z':                  /* zetta or 2**70.  */
      أرجع 7;

    حالة 'Y':                  /* yotta or 2**80.  */
      أرجع 8;

    حالة 'R':                  /* ronna or 2**90.  */
      أرجع 9;

    حالة 'Q':                  /* quetta or 2**100.  */
      أرجع 10;

    افتراضي:                   /* should never happen. assert?  */
      أرجع 0;
    }
}

ساكن مضمن حرف ثابت *
suffix_power_char (صحيح power)
{
  بدل (power)
    {
    حالة 0:
      أرجع "";

    حالة 1:
      أرجع "K";

    حالة 2:
      أرجع "M";

    حالة 3:
      أرجع "G";

    حالة 4:
      أرجع "T";

    حالة 5:
      أرجع "P";

    حالة 6:
      أرجع "E";

    حالة 7:
      أرجع "Z";

    حالة 8:
      أرجع "Y";

    حالة 9:
      أرجع "R";

    حالة 10:
      أرجع "Q";

    افتراضي:
      أرجع "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
ساكن دراز مزدوج
powerld (دراز مزدوج پایه_2, ptrdiff_t x)
{
  دراز مزدوج نتیجه = پایه_2;
  إذا (x == 0)
    أرجع 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  طالما (--x)
    نتیجه *= پایه_2;
  أرجع نتیجه;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
ساكن مضمن دراز مزدوج
absld (دراز مزدوج val)
{
  أرجع val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
ساكن دراز مزدوج
expld (دراز مزدوج val, صحيح پایه_2, صحيح /*output */ *x)
{
  صحيح power = 0;

  إذا (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      طالما (absld (val) >= پایه_2)
        {
          ++power;
          val /= پایه_2;
        }
    }
  إذا (x)
    *x = power;
  أرجع val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ساكن مضمن intmax_t
simple_دور_ceiling (دراز مزدوج val)
{
  intmax_t intval = val;
  إذا (intval < val)
    intval++;
  أرجع intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ساكن مضمن intmax_t
simple_دور_floor (دراز مزدوج val)
{
  أرجع -simple_دور_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
ساكن مضمن intmax_t
simple_دور_from_zero (دراز مزدوج val)
{
  أرجع val < 0 ? simple_دور_floor (val) : simple_دور_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
ساكن مضمن intmax_t
simple_دور_مقصد_zero (دراز مزدوج val)
{
  أرجع val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ساكن مضمن intmax_t
simple_دور_نزدیکترین (دراز مزدوج val)
{
  أرجع val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
ساكن مضمن دراز مزدوج
simple_دور (دراز مزدوج val, تعداد دور_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (دراز مزدوج) INTMAX_MAX * intmax_mul;

  بدل (t)
    {
    حالة دور_ceiling:
      rval = simple_دور_ceiling (val);
      اكسر;

    حالة دور_floor:
      rval = simple_دور_floor (val);
      اكسر;

    حالة دور_from_zero:
      rval = simple_دور_from_zero (val);
      اكسر;

    حالة دور_مقصد_zero:
      rval = simple_دور_مقصد_zero (val);
      اكسر;

    حالة دور_نزدیکترین:
      rval = simple_دور_نزدیکترین (val);
      اكسر;

    افتراضي:
      /* to silence the compiler - this should never happen.  */
      أرجع 0;
    }

  أرجع (دراز مزدوج) INTMAX_MAX * intmax_mul + rval;
}

تعداد simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_سرریز,
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
ساكن تعداد simple_strtod_error
simple_strtod_int (حرف ثابت *درون_داد_str,
                   حرف **endptr, دراز مزدوج *مقدار, bool *negative)
{
  تعداد simple_strtod_error e = SSE_OK;

  دراز مزدوج val = 0;
  صحيح digits = 0;
  bool یافت_digit = false;

  إذا (*درون_داد_str == '-')
    {
      درون_داد_str++;
      *negative = true;
    }
  وإلا
    *negative = false;

  *endptr = (حرف *) درون_داد_str;
  طالما (c_isdigit (**endptr))
    {
      صحيح digit = (**endptr) - '0';

      یافت_digit = true;

      إذا (val || digit)
        digits++;

      إذا (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      إذا (digits > MAX_ACCEPTABLE_DIGITS)
        أرجع SSE_سرریز;

      val *= 10;
      val += digit;

      ++(*endptr);

      إذا (thousands_sep_درازی > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_درازی)
          && c_isdigit ((*endptr)[thousands_sep_درازی]))
        (*endptr) += thousands_sep_درازی;
    }
  إذا (! یافت_digit
      && ! STREQ_LEN (*endptr, decimal_نقطه, decimal_نقطه_درازی))
    أرجع SSE_INVALID_عدد;
  إذا (*negative)
    val = -val;

  إذا (مقدار)
    *مقدار = val;

  أرجع e;
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
ساكن تعداد simple_strtod_error
simple_strtod_float (حرف ثابت *درون_داد_str,
                     حرف **endptr,
                     دراز مزدوج *مقدار,
                     size_t *precision)
{
  bool negative;
  تعداد simple_strtod_error e = SSE_OK;

  إذا (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (درون_داد_str, endptr, مقدار, &negative);
  إذا (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    أرجع e;

  /* optional decimal point + fraction.  */
  إذا (STREQ_LEN (*endptr, decimal_نقطه, decimal_نقطه_درازی))
    {
      حرف *ptr2;
      دراز مزدوج val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_نقطه_درازی;
      تعداد simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      إذا (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        أرجع e2;
      إذا (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      إذا (neg_frac)
        أرجع SSE_INVALID_عدد;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      إذا (مقدار)
        {
          إذا (negative)
            *مقدار -= val_frac;
          وإلا
            *مقدار += val_frac;
        }

      إذا (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  أرجع e;
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
ساكن تعداد simple_strtod_error
simple_strtod_human (حرف ثابت *درون_داد_str,
                     حرف **endptr, دراز مزدوج *مقدار, size_t *precision,
                     تعداد scale_type allowed_scaling)
{
  صحيح power = 0;
  /* 'scale_auto' is checked below.  */
  صحيح scale_پایه = default_scale_پایه (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, درون_داد_str),
          quote_n (1, decimal_نقطه),
          MAX_UNSCALED_DIGITS);

  تعداد simple_strtod_error e =
    simple_strtod_float (درون_داد_str, endptr, مقدار, precision);
  إذا (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    أرجع e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *مقدار, (صحيح)*precision);

  طالما (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      إذا (unit_separator)
        {
          size_t sep_len = طول_رشته (unit_separator);
          إذا (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      إذا (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          إذا (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      إذا (**endptr == '\0')
        اكسر;  /* Treat as no suffix.  */

      إذا (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          إذا (**endptr == '\0')
            اكسر;

          أرجع SSE_INVALID_SUFFIX;
        }

      إذا (allowed_scaling == scale_none)
        أرجع SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      إذا (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_پایه = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_پایه);
        }
      وإلا إذا (allowed_scaling == scale_IEC_I)
        {
          إذا (**endptr == 'i')
            (*endptr)++;
          وإلا
            أرجع SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      اكسر;
    }

  دراز مزدوج multiplier = powerld (scale_پایه, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_پایه, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*مقدار) = (*مقدار) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *مقدار, *مقدار);

  أرجع e;
}


ساكن فراغ
simple_strtod_fatal (تعداد simple_strtod_error err, حرف ثابت *درون_داد_str)
{
  حرف ثابت *msgid = عدم;

  بدل (err)
    {
    حالة SSE_سرریز:
      msgid = N_("value too large to be converted: %s");
      اكسر;

    حالة SSE_INVALID_عدد:
      msgid = N_("invalid number: %s");
      اكسر;

    حالة SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      اكسر;

    حالة SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      اكسر;

    حالة SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      اكسر;

    حالة SSE_OK_PRECISION_LOSS:
    حالة SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  إذا (inval_style != inval_ignore)
    error (conv_exit_کد, 0, gettext (msgid), quote (درون_داد_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
ساكن صحيح
double_مقصد_human (دراز مزدوج val, صحيح precision,
                 حرف *buf, idx_t buf_اندازه,
                 تعداد scale_type scale, صحيح گروه, تعداد دور_type دور_2)
{
  حرف fmt[حجم "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_عرض)];
  حرف *pfmt = fmt;
  *pfmt++ = '%';

  إذا (گروه)
    *pfmt++ = '\'';

  إذا (zero_padding_عرض)
    pfmt += sprintf (pfmt, "0%d", zero_padding_عرض);

  devmsg ("double_to_human:\n");

  إذا (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_دور (val, دور_2);
      val /= powerld (10, precision);

      devmsg ((گروه) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      أرجع snprintf (buf, buf_اندازه, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  مزدوج scale_پایه = default_scale_پایه (scale);

  /* Normalize val to scale. */
  صحيح power = 0;
  val = expld (val, scale_پایه, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_پایه, power);

  /* Perform rounding. */
  صحيح power_adjust = 0;
  إذا (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  وإلا إذا (absld (val) < 10)
    {
      /* for values less than 10, we allow one decimal-point digit,
         so adjust before rounding. */
      power_adjust = 1;
    }

  val *= powerld (10, power_adjust);
  val = simple_دور (val, دور_2);
  val /= powerld (10, power_adjust);

  /* two special cases after rounding:
     1. a "999.99" can turn into 1000 - so scale down
     2. a "9.99" can turn into 10 - so don't display decimal-point.  */
  إذا (absld (val) >= scale_پایه)
    {
      val /= scale_پایه;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  صحيح show_decimal_نقطه = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_پایه, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  صحيح prec = user_precision == -1 ? show_decimal_نقطه : user_precision;

  أرجع snprintf (buf, buf_اندازه, fmt, prec, val,
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
ساكن uintmax_t
unit_مقصد_umax (حرف ثابت *n_string)
{
  strtol_error s_err;
  حرف ثابت *c_string = n_string;
  حرف *t_string = عدم;
  size_t n_len = طول_رشته (n_string);
  حرف *انتها = عدم;
  uintmax_t n;
  حرف ثابت *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  إذا (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      انتها = t_string + n_len - 1;
      کپی_حافظه (t_string, n_string, n_len);

      إذا (*انتها == 'i' && 2 <= n_len && ! c_isdigit (*(انتها - 1)))
        *انتها = '\0';
      وإلا
        {
          *++انتها = 'B';
          *++انتها = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &انتها, 10, &n, suffixes);

  إذا (s_err != LONGINT_OK || *انتها || n == 0)
    {
      حرر (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  حرر (t_string);

  أرجع n;
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [عدد_2]...\n\
"), برنامه_name);
      fputs (_("\
Reformat عدد_2(s), or the numbers from standard درون_داد إذا none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid درون_داد\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead از whitespace لكل فیلد delimiter\n\
"));
      oputs (_("\
      --فیلد=FIELDS\n\
         replace the numbers درون these درون_داد fields (افتراضي=1);\n\
         see FIELDS زیر لكل details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use چاپ_کن style floating-نقطه FORMAT;\n\
         see FORMAT زیر لكل details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         خودکار-scale درون_داد numbers مقصد UNITs; افتراضي is 'none';\n\
         see UNIT زیر لكل details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the درون_داد unit اندازه (instead از the افتراضي 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping از digits, e.g. 1,000,000.\n\
         This has no effect درون the C/POSIX locale\n\
"));
      oputs (_("\
      --سرآیند[=N]\n\
         print (without converting) the اولی N سرآیند سطرها;\n\
         N defaults مقصد 1 إذا not specified\n\
"));
      oputs (_("\
      --invalid=نما\n\
         failure نما_2 لكل invalid numbers;\n\
         نما can be: abort (افتراضي), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output مقصد N characters;\n\
         مثبت N will راست-align, negative N will چپ-align;\n\
         padding is ignored إذا the output is wider than N;\n\
         the افتراضي is مقصد automatically pad إذا a whitespace is یافت\n\
"));
      oputs (_("\
      --دور_2=روش\n\
         use روش لكل rounding when scaling; روش can be:\n\
         به_بالا, پایین, from-zero (افتراضي), towards-zero, نزدیکترین\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         جمع_کن SUFFIX مقصد output numbers,\n\
         and accept an optional SUFFIX درون درون_داد numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         درج SEP between عدد and unit on output,\n\
         and accept an optional SEP درون درون_داد numbers\n\
"));
      oputs (_("\
      --مقصد=UNIT\n\
         خودکار-scale output numbers مقصد UNITs; see UNIT زیر\n\
"));
      oputs (_("\
      --مقصد-unit=N\n\
         the output unit اندازه (instead از the افتراضي 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);

      fputs (_("\
\n\
UNIT options:\n"), stdout);
      fputs (_("\
  none       no خودکار-scaling is انجامشده; suffixes will trigger an error\n\
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
FIELDS supports cut(1) style فیلد ranges:\n\
  N    N'th فیلد, counted from 1\n\
  N-   from N'th فیلد, مقصد انتها از سطر\n\
  N-M  from N'th to M'th فیلد (inclusive)\n\
  -M   from اولی مقصد M'th فیلد (inclusive)\n\
  -    all fields\n\
مضرب_2 fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable لكل printing one floating-نقطه آرگومان '%f'.\n\
Optional quote (%'f) will enable --grouping (إذا supported by current locale).\n\
Optional عرض مقدار (%10f) will pad output. Optional zero (%010f) عرض\n\
will zero pad the عدد. Optional negative مقادیر (%-10f) will چپ align.\n\
Optional precision (%.1f) will override the درون_داد determined precision.\n\
"), stdout);

      چاپ_کن (_("\n\
Exit وضعیت is 0 إذا all درون_داد numbers were successfully converted.\n\
By افتراضي, %s will stop at the اولی conversion error with خروج وضعیت 2.\n\
With --invalid='fail' a warning is printed لكل each conversion error\n\
and the خروج وضعیت is 2.  With --invalid='warn' each conversion error is\n\
diagnosed, but the خروج وضعیت is 0.  With --invalid='ignore' conversion\n\
errors are not diagnosed and the خروج وضعیت is 0.\n\
"), برنامه_name);

      چاپ_کن (_("\n\
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
  $ df -B1 | %s --سرآیند --فیلد 2-4 --مقصد=si\n\
  $ ls -l  | %s --سرآیند --فیلد 5 --مقصد=iec\n\
  $ ls -lh | %s --سرآیند --فیلد 5 --from=iec --padding=10\n\
  $ ls -lh | %s --سرآیند --فیلد 5 --from=iec --format %%10f\n"),
              برنامه_name, برنامه_name, برنامه_name,
              برنامه_name, برنامه_name, برنامه_name,
              برنامه_name, برنامه_name, برنامه_name);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
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
ساكن فراغ
parse_format_string (حرف ثابت *fmt)
{
  size_t i;
  size_t پیشوند_len = 0;
  size_t suffix_pos;
  حرف *endptr = عدم;
  bool zero_padding = false;

  لكل (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      إذا (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      پیشوند_len++;
    }

  i++;
  طالما (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      إذا (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      وإلا إذا (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      وإلا إذا (! skip)
        اكسر;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  إذا (pad != 0)
    {
      إذا (debug && padding_عرض && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      إذا (pad < 0)
        padding_عرض = pad;
      وإلا
        {
          إذا (zero_padding)
            zero_padding_عرض = MIN (pad, بیشترین_صحیح);
          وإلا
            padding_عرض = pad;
        }
    }
  i = endptr - fmt;

  إذا (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  إذا (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      إذا (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  إذا (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  لكل (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    إذا (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  إذا (پیشوند_len)
    format_str_پیشوند = ximemdup0 (fmt, پیشوند_len);
  إذا (fmt[suffix_pos] != '\0')
    format_str_suffix = xstrdup (fmt + suffix_pos);

  devmsg ("format String:\n  input: %s\n  grouping: %s\n"
                   "  padding width: %jd\n"
                   "  prefix: %s\n  suffix: %s\n",
          quote_n (0, fmt), (grouping) ? "yes" : "no",
          padding_عرض,
          quote_n (1, format_str_پیشوند ? format_str_پیشوند : ""),
          quote_n (2, format_str_suffix ? format_str_suffix : ""));
}

/* Parse a numeric value (with optional suffix) from a string.
   Returns a long double value, with input precision.

   If there's an error converting the string to value - exits with
   an error.

   If there are any trailing characters after the number
   (besides a valid suffix) - exits with an error.  */
ساكن تعداد simple_strtod_error
parse_human_عدد (حرف ثابت *str, دراز مزدوج /*output */ *مقدار,
                    size_t *precision)
{
  حرف *ptr = عدم;

  تعداد simple_strtod_error e =
    simple_strtod_human (str, &ptr, مقدار, precision, scale_from);
  إذا (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      أرجع e;
    }

  إذا (ptr && *ptr != '\0')
    {
      إذا (inval_style != inval_ignore)
        error (conv_exit_کد, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  أرجع e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
ساكن bool
prepare_padded_عدد (ثابت دراز مزدوج val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_استفاده_شده = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  صحيح x;
  expld (val, 10, &x);

  إذا (scale_مقصد == scale_none
      && x + precision_استفاده_شده > MAX_UNSCALED_DIGITS)
    {
      إذا (inval_style != inval_ignore)
        {
          إذا (precision_استفاده_شده)
            error (conv_exit_کد, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_استفاده_شده);
          وإلا
            error (conv_exit_کد, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      أرجع false;
    }

  إذا (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      إذا (inval_style != inval_ignore)
        error (conv_exit_کد, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      أرجع false;
    }

  طالما (true)
    {
      صحيح numlen = double_مقصد_human (val, precision_استفاده_شده,
                                    padding_میانگیر, padding_میانگیر_اندازه,
                                    scale_مقصد, grouping, دور_style);
      ptrdiff_t growth;
      إذا (numlen < 0 || ckd_sub (&growth, numlen, padding_میانگیر_اندازه - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      إذا (growth <= 0)
        اكسر;
      padding_میانگیر = xpalloc (padding_میانگیر, &padding_میانگیر_اندازه,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_میانگیر));

  intmax_t pad = 0;
  إذا (padding_عرض)
    {
      صحيح buf_عرض = mbswidth (padding_میانگیر,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      إذا (0 <= buf_عرض)
        {
          إذا (padding_عرض < 0)
            {
              إذا (padding_عرض < -buf_عرض)
                pad = padding_عرض + buf_عرض;
            }
          وإلا
            {
              إذا (buf_عرض < padding_عرض)
                pad = padding_عرض - buf_عرض;
            }
        }
    }

  *padding = pad;
  أرجع true;
}

ساكن فراغ
print_padded_عدد (intmax_t padding)
{
  إذا (format_str_پیشوند)
    fputs (format_str_پیشوند, stdout);

  لكل (intmax_t p = padding; 0 < p; p--)
    حرف_چاپ_کن (' ');

  fputs (padding_میانگیر, stdout);

  لكل (intmax_t p = padding; p < 0; p++)
    حرف_چاپ_کن (' ');

  إذا (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
ساكن bool
process_suffixed_عدد (حرف *متن, دراز مزدوج *نتیجه,
                         size_t *precision, دراز صحيح فیلد)
{
  حرف saved_suffix = '\0';

  إذا (suffix)
    {
      إذا (mbs_endswith (متن, suffix))
        {
          saved_suffix = *(متن + طول_رشته (متن) - طول_رشته (suffix));
          *(متن + طول_رشته (متن) - طول_رشته (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      وإلا
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  حرف *p = skip_str_matching (متن, newline_or_blank, true);

  /* setup auto-padding.  */
  إذا (auto_padding)
    {
      padding_عرض = متن < p || 1 < فیلد
                      ? mbswidth (متن,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      إذا (padding_عرض < 0)
        padding_عرض = طول_رشته (متن);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_عرض);
    }

  دراز مزدوج val = 0;
  تعداد simple_strtod_error e = parse_human_عدد (p, &val, precision);
  إذا (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  إذا (from_unit_اندازه != 1 || مقصد_unit_اندازه != 1)
    val = (val * from_unit_اندازه) / مقصد_unit_اندازه;

  *نتیجه = val;

  إذا (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    أرجع true;
  وإلا
    {
      إذا (saved_suffix)
        *(متن + طول_رشته (متن)) = saved_suffix;
      أرجع false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_خالص
ساكن حرف *
mbsmbchr (حرف ثابت *s, حرف ثابت *c)
{
  بیعلامت حرف uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  إذا (uc < 0x30 || MB_CUR_MAX == 1)
    أرجع (حرف *) strchr (s, uc);
  وإلا إذا (is_utf8_charset ())
    أرجع (حرف *) (uc < 0x80 ? strchr (s, uc) : جستجوی_رشته (s, c));
  وإلا
    أرجع *(c + 1) == '\0' ? mbschr (s, uc) : (حرف *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
ساكن حرف *
بعدی_فیلد (حرف **سطر)
{
  حرف *فیلد_شروع = *سطر;
  حرف *فیلد_انتها   = فیلد_شروع;

  إذا (delimiter)
    {
      إذا (! (فیلد_انتها = mbsmbchr (فیلد_شروع, delimiter)))
        فیلد_انتها = strchr (فیلد_شروع, '\0');
    }
  وإلا
    {
      /* keep any space prefix in the returned field */
      فیلد_انتها = skip_str_matching (فیلد_انتها, newline_or_blank, true);
      فیلد_انتها = skip_str_matching (فیلد_انتها, newline_or_blank, false);
    }

  *سطر = فیلد_انتها;
  أرجع فیلد_شروع;
}

ATTRIBUTE_خالص
ساكن bool
include_فیلد (uintmax_t فیلد)
{
  بنية فیلد_range_pair *p = frp;
  إذا (!p)
    أرجع فیلد == 1;

  طالما (p->lo != UINTMAX_MAX)
    {
      إذا (p->lo <= فیلد && p->hi >= فیلد)
        أرجع true;
      ++p;
    }
  أرجع false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
ساكن bool
process_فیلد (حرف *متن, uintmax_t فیلد)
{
  دراز مزدوج val = 0;
  size_t precision = 0;
  bool valid_عدد = true;

  إذا (include_فیلد (فیلد))
    {
      valid_عدد =
        process_suffixed_عدد (متن, &val, &precision, فیلد);

      intmax_t padding;
      إذا (valid_عدد)
        valid_عدد = prepare_padded_عدد (val, precision, &padding);

      إذا (valid_عدد)
        print_padded_عدد (padding);
      وإلا
        fputs (متن, stdout);
    }
  وإلا
    fputs (متن, stdout);

  أرجع valid_عدد;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
ساكن صحيح
process_سطر (حرف *سطر, bool newline)
{
  حرف *بعدی;
  uintmax_t فیلد = 0;
  bool valid_عدد = true;

  طالما (true) {
    ++فیلد;
    بعدی = بعدی_فیلد (&سطر);

    إذا (*سطر != '\0')
      {
        /* NUL terminate the current field string and process */
        حرف انتها_فیلد = *سطر;
        *سطر = '\0';

        إذا (! process_فیلد (بعدی, فیلد))
          valid_عدد = false;

        إذا (delimiter != عدم)
          fputs (delimiter, stdout);
        وإلا
          fputc (' ', stdout);

        إذا (delimiter)
          سطر += MAX (طول_رشته (delimiter), 1);
        وإلا
          {
            *سطر = انتها_فیلد;
            mcel_t g = mcel_scanz (سطر);
            سطر += g.len;
          }
      }
    وإلا
      {
        /* end of the line, process the last field and finish */
        إذا (! process_فیلد (بعدی, فیلد))
          valid_عدد = false;

        اكسر;
      }
  }

  إذا (newline)
    حرف_چاپ_کن (سطر_delim);

 إذا (ferror (stdout))
   write_error ();

  أرجع valid_عدد;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#إذا HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#پایان_اگر

  decimal_نقطه = nl_langinfo (RADIXCHAR);
  إذا (decimal_نقطه == عدم || طول_رشته (decimal_نقطه) == 0)
    decimal_نقطه = ".";
  decimal_نقطه_درازی = طول_رشته (decimal_نقطه);

  thousands_sep = nl_langinfo (THOUSEP);
  إذا (thousands_sep == عدم)
    thousands_sep = "";
  thousands_sep_درازی = طول_رشته (thousands_sep);

  atexit (close_stdout);

  طالما (true)
    {
      صحيح c = getopt_long (argc, argv, "d:z", longopts, عدم);

      إذا (c == -1)
        اكسر;

      بدل (c)
        {
        حالة FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_آرگومانها, scale_from_types);
          اكسر;

        حالة FROM_UNIT_OPTION:
          from_unit_اندازه = unit_مقصد_umax (optarg);
          اكسر;

        حالة مقصد_OPTION:
          scale_مقصد =
            XARGMATCH ("--to", optarg, scale_مقصد_آرگومانها, scale_مقصد_types);
          اكسر;

        حالة مقصد_UNIT_OPTION:
          مقصد_unit_اندازه = unit_مقصد_umax (optarg);
          اكسر;

        حالة دور_OPTION:
          دور_style = XARGMATCH ("--round", optarg, دور_آرگومانها, دور_types);
          اكسر;

        حالة GROUPING_OPTION:
          grouping = 1;
          اكسر;

        حالة PADDING_OPTION:
          إذا (((xstrtoimax (optarg, عدم, 10, &padding_عرض, "")
                & ~LONGINT_سرریز)
               != LONGINT_OK)
              || padding_عرض == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          اكسر;

        حالة فیلد_OPTION:
          إذا (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          مجموعه_fields (optarg, SETFLD_ALLOW_DASH);
          اكسر;

        حالة 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          إذا (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              إذا (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          اكسر;

        حالة 'z':
          سطر_delim = '\0';
          اكسر;

        حالة SUFFIX_OPTION:
          suffix = optarg;
          اكسر;

        حالة UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          اكسر;

        حالة DEBUG_OPTION:
          debug = true;
          اكسر;

        حالة DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          اكسر;

        حالة سرآیند_OPTION:
          إذا (optarg)
            {
              إذا (xstrtoumax (optarg, عدم, 10, &سرآیند, "") != LONGINT_OK
                  || سرآیند == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          وإلا
            {
              سرآیند = 1;
            }
          اكسر;

        حالة FORMAT_OPTION:
          format_str = optarg;
          اكسر;

        حالة INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_آرگومانها, inval_types);
          اكسر;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (format_str != عدم && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  إذا (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  إذا (debug && scale_from == scale_none && scale_مقصد == scale_none
      && !grouping && (padding_عرض == 0) && (format_str == عدم))
    error (0, 0, _("no conversion option specified"));

  إذا (debug && unit_separator && delimiter == عدم)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  إذا (format_str)
    parse_format_string (format_str);

  إذا (grouping)
    {
      إذا (scale_مقصد != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      إذا (debug && thousands_sep_درازی == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_عرض == 0 && delimiter == عدم);

  إذا (inval_style != inval_abort)
    conv_exit_کد = 0;

  إذا (argc > optind)
    {
      إذا (debug && سرآیند)
        error (0, 0, _("--header ignored with command-line input"));

      لكل (; optind < argc; optind++)
        valid_numbers &= process_سطر (argv[optind], true);
    }
  وإلا
    {
      حرف *سطر = عدم;
      size_t سطر_allocated = 0;
      ssize_t len;

      طالما (سرآیند-- && getdelim (&سطر, &سطر_allocated,
                                   سطر_delim, stdin) > 0)
        {
          إذا (fputs (سطر, stdout) == EOF)
            write_error ();
        }

      طالما ((len = getdelim (&سطر, &سطر_allocated,
                              سطر_delim, stdin)) > 0)
        {
          bool newline = سطر[len - 1] == سطر_delim;
          إذا (newline)
            سطر[len - 1] = '\0';
          valid_numbers &= process_سطر (سطر, newline);
        }

      إذا (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  إذا (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  صحيح exit_وضعیت = EXIT_SUCCESS;
  إذا (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_وضعیت = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_وضعیت);
}
