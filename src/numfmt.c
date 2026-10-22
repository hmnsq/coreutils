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

#అన్తర్భావ <config.h>
#అన్తర్భావ <float.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <langinfo.h>

#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "mbswidth.h"
#అన్తర్భావ "mcel.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "skipchars.h"
#అన్తర్భావ "system.h"
#అన్తర్భావ "utf8.h"
#అన్తర్భావ "xstrtol.h"

#అన్తర్భావ "set-fields.h"

#ఒకవేళ HAVE_FPSETPREC
# include <ieeefp.h>
#అయితే_ముగింపు

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "numfmt"

#నిర్వచించు AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
గణన { EXIT_CONVERSION_WARNINGS = 2 };

గణన
{
  FROM_OPTION = CHAR_MAX + 1,
  FROM_UNIT_OPTION,
  గమ్యం_OPTION,
  గమ్యం_UNIT_OPTION,
  విడత_OPTION,
  SUFFIX_OPTION,
  GROUPING_OPTION,
  PADDING_OPTION,
  క్షేత్రం_OPTION,
  DEBUG_OPTION,
  DEV_DEBUG_OPTION,
  శీర్షిక_OPTION,
  FORMAT_OPTION,
  INVALID_OPTION,
  UNIT_SEPARATOR_OPTION
};

గణన scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి scale_from_వాదనలు[] =
{
  "none", "auto", "si", "iec", "iec-i", ఖాళీ
};

స్థిర గణన scale_type స్థిరరాశి scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి scale_గమ్యం_వాదనలు[] =
{
  "none", "si", "iec", "iec-i", ఖాళీ
};

స్థిర గణన scale_type స్థిరరాశి scale_గమ్యం_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


గణన విడత_type
{
  విడత_ceiling,
  విడత_floor,
  విడత_from_zero,
  విడత_గమ్యం_zero,
  విడత_సమీప,
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి విడత_వాదనలు[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", ఖాళీ
};

స్థిర గణన విడత_type స్థిరరాశి విడత_types[] =
{
  విడత_ceiling, విడత_floor, విడత_from_zero, విడత_గమ్యం_zero, విడత_సమీప
};


గణన inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి inval_వాదనలు[] =
{
  "abort", "fail", "warn", "ignore", ఖాళీ
};

స్థిర గణన inval_type స్థిరరాశి inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"from", required_వాదం, ఖాళీ, FROM_OPTION},
  {"from-unit", required_వాదం, ఖాళీ, FROM_UNIT_OPTION},
  {"to", required_వాదం, ఖాళీ, గమ్యం_OPTION},
  {"to-unit", required_వాదం, ఖాళీ, గమ్యం_UNIT_OPTION},
  {"round", required_వాదం, ఖాళీ, విడత_OPTION},
  {"padding", required_వాదం, ఖాళీ, PADDING_OPTION},
  {"suffix", required_వాదం, ఖాళీ, SUFFIX_OPTION},
  {"unit-separator", required_వాదం, ఖాళీ, UNIT_SEPARATOR_OPTION},
  {"grouping", no_వాదం, ఖాళీ, GROUPING_OPTION},
  {"delimiter", required_వాదం, ఖాళీ, 'd'},
  {"field", required_వాదం, ఖాళీ, క్షేత్రం_OPTION},
  {"debug", no_వాదం, ఖాళీ, DEBUG_OPTION},
  {"-debug", no_వాదం, ఖాళీ, DEV_DEBUG_OPTION},
  {"header", optional_వాదం, ఖాళీ, శీర్షిక_OPTION},
  {"format", required_వాదం, ఖాళీ, FORMAT_OPTION},
  {"invalid", required_వాదం, ఖాళీ, INVALID_OPTION},
  {"zero-terminated", no_వాదం, ఖాళీ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
గణన { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
గణన { MAX_ACCEPTABLE_DIGITS = 33 };

స్థిర గణన scale_type scale_from = scale_none;
స్థిర గణన scale_type scale_గమ్యం = scale_none;
స్థిర గణన విడత_type విడత_style = విడత_from_zero;
స్థిర గణన inval_type inval_style = inval_abort;
స్థిర అక్షరం స్థిరరాశి *suffix = ఖాళీ;
స్థిర అక్షరం స్థిరరాశి *unit_separator = ఖాళీ;
స్థిర uintmax_t from_unit_కొలత = 1;
స్థిర uintmax_t గమ్యం_unit_కొలత = 1;
స్థిర పూర్ణాంకం grouping = 0;
స్థిర అక్షరం *padding_బఫరు = ఖాళీ;
స్థిర idx_t padding_బఫరు_కొలత = 0;
స్థిర intmax_t padding_వెడల్పు = 0;
స్థిర పూర్ణాంకం zero_padding_వెడల్పు = 0;
స్థిర దీర్ఘ పూర్ణాంకం user_precision = -1;
స్థిర అక్షరం స్థిరరాశి *format_str = ఖాళీ;
స్థిర అక్షరం *format_str_ఉపసర్గ = ఖాళీ;
స్థిర అక్షరం *format_str_suffix = ఖాళీ;

/* By default, any conversion error will terminate the program.  */
స్థిర పూర్ణాంకం conv_exit_సంకేతం = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
స్థిర పూర్ణాంకం auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
స్థిర అక్షరం స్థిరరాశి *delimiter = ఖాళీ;

/* line delimiter.  */
స్థిర గుర్తులేని అక్షరం పంక్తి_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
స్థిర uintmax_t శీర్షిక = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
స్థిర bool debug;

/* will be set according to the current locale.  */
స్థిర అక్షరం స్థిరరాశి *decimal_బిందువు;
స్థిర పూర్ణాంకం decimal_బిందువు_నిడివి;
స్థిర అక్షరం స్థిరరాశి *thousands_sep;
స్థిర పూర్ణాంకం thousands_sep_నిడివి;

/* debugging for developers.  Enables devmsg().  */
స్థిర bool dev_debug = false;

స్థిర bool
newline_or_blank (mcel_t g)
{
  తిరిగి g.ch == '\n' || c32issep (g.ch);
}

స్థిర అంతఃవరుస పూర్ణాంకం
default_scale_పునాది (గణన scale_type scale)
{
  ఎంపిక (scale)
    {
    స్థితి scale_IEC:
    స్థితి scale_IEC_I:
      తిరిగి 1024;

    స్థితి scale_none:
    స్థితి scale_auto:
    స్థితి scale_SI:
    అప్రమేయ:
      తిరిగి 1000;
    }
}

స్థిర అక్షరం స్థిరరాశి zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
స్థిర అక్షరం స్థిరరాశి *valid_suffixes = 1 + zero_and_valid_suffixes;

స్థిర అంతఃవరుస bool
valid_suffix (స్థిరరాశి అక్షరం suf)
{
  తిరిగి strchr (valid_suffixes, suf) != ఖాళీ;
}

స్థిర అంతఃవరుస పూర్ణాంకం
suffix_power (స్థిరరాశి అక్షరం suf)
{
  ఎంపిక (suf)
    {
    స్థితి 'k':                  /* kilo.  */
    స్థితి 'K':                  /* kilo or kibi.  */
      తిరిగి 1;

    స్థితి 'M':                  /* mega or mebi.  */
      తిరిగి 2;

    స్థితి 'G':                  /* giga or gibi.  */
      తిరిగి 3;

    స్థితి 'T':                  /* tera or tebi.  */
      తిరిగి 4;

    స్థితి 'P':                  /* peta or pebi.  */
      తిరిగి 5;

    స్థితి 'E':                  /* exa or exbi.  */
      తిరిగి 6;

    స్థితి 'Z':                  /* zetta or 2**70.  */
      తిరిగి 7;

    స్థితి 'Y':                  /* yotta or 2**80.  */
      తిరిగి 8;

    స్థితి 'R':                  /* ronna or 2**90.  */
      తిరిగి 9;

    స్థితి 'Q':                  /* quetta or 2**100.  */
      తిరిగి 10;

    అప్రమేయ:                   /* should never happen. assert?  */
      తిరిగి 0;
    }
}

స్థిర అంతఃవరుస అక్షరం స్థిరరాశి *
suffix_power_char (పూర్ణాంకం power)
{
  ఎంపిక (power)
    {
    స్థితి 0:
      తిరిగి "";

    స్థితి 1:
      తిరిగి "K";

    స్థితి 2:
      తిరిగి "M";

    స్థితి 3:
      తిరిగి "G";

    స్థితి 4:
      తిరిగి "T";

    స్థితి 5:
      తిరిగి "P";

    స్థితి 6:
      తిరిగి "E";

    స్థితి 7:
      తిరిగి "Z";

    స్థితి 8:
      తిరిగి "Y";

    స్థితి 9:
      తిరిగి "R";

    స్థితి 10:
      తిరిగి "Q";

    అప్రమేయ:
      తిరిగి "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
స్థిర దీర్ఘ ద్విగుణ
powerld (దీర్ఘ ద్విగుణ పునాది_2, ptrdiff_t x)
{
  దీర్ఘ ద్విగుణ ఫలితం = పునాది_2;
  ఒకవేళ (x == 0)
    తిరిగి 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  అయితే (--x)
    ఫలితం *= పునాది_2;
  తిరిగి ఫలితం;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
స్థిర అంతఃవరుస దీర్ఘ ద్విగుణ
absld (దీర్ఘ ద్విగుణ val)
{
  తిరిగి val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
స్థిర దీర్ఘ ద్విగుణ
expld (దీర్ఘ ద్విగుణ val, పూర్ణాంకం పునాది_2, పూర్ణాంకం /*output */ *x)
{
  పూర్ణాంకం power = 0;

  ఒకవేళ (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      అయితే (absld (val) >= పునాది_2)
        {
          ++power;
          val /= పునాది_2;
        }
    }
  ఒకవేళ (x)
    *x = power;
  తిరిగి val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
స్థిర అంతఃవరుస intmax_t
simple_విడత_ceiling (దీర్ఘ ద్విగుణ val)
{
  intmax_t intval = val;
  ఒకవేళ (intval < val)
    intval++;
  తిరిగి intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
స్థిర అంతఃవరుస intmax_t
simple_విడత_floor (దీర్ఘ ద్విగుణ val)
{
  తిరిగి -simple_విడత_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
స్థిర అంతఃవరుస intmax_t
simple_విడత_from_zero (దీర్ఘ ద్విగుణ val)
{
  తిరిగి val < 0 ? simple_విడత_floor (val) : simple_విడత_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
స్థిర అంతఃవరుస intmax_t
simple_విడత_గమ్యం_zero (దీర్ఘ ద్విగుణ val)
{
  తిరిగి val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
స్థిర అంతఃవరుస intmax_t
simple_విడత_సమీప (దీర్ఘ ద్విగుణ val)
{
  తిరిగి val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
స్థిర అంతఃవరుస దీర్ఘ ద్విగుణ
simple_విడత (దీర్ఘ ద్విగుణ val, గణన విడత_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (దీర్ఘ ద్విగుణ) INTMAX_MAX * intmax_mul;

  ఎంపిక (t)
    {
    స్థితి విడత_ceiling:
      rval = simple_విడత_ceiling (val);
      ఆపు;

    స్థితి విడత_floor:
      rval = simple_విడత_floor (val);
      ఆపు;

    స్థితి విడత_from_zero:
      rval = simple_విడత_from_zero (val);
      ఆపు;

    స్థితి విడత_గమ్యం_zero:
      rval = simple_విడత_గమ్యం_zero (val);
      ఆపు;

    స్థితి విడత_సమీప:
      rval = simple_విడత_సమీప (val);
      ఆపు;

    అప్రమేయ:
      /* to silence the compiler - this should never happen.  */
      తిరిగి 0;
    }

  తిరిగి (దీర్ఘ ద్విగుణ) INTMAX_MAX * intmax_mul + rval;
}

గణన simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_అధికప్రవాహం,
  SSE_INVALID_సంఖ్య,

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
స్థిర గణన simple_strtod_error
simple_strtod_int (అక్షరం స్థిరరాశి *ఆదానం_str,
                   అక్షరం **endptr, దీర్ఘ ద్విగుణ *విలువ, bool *negative)
{
  గణన simple_strtod_error e = SSE_OK;

  దీర్ఘ ద్విగుణ val = 0;
  పూర్ణాంకం digits = 0;
  bool దొరికింది_digit = false;

  ఒకవేళ (*ఆదానం_str == '-')
    {
      ఆదానం_str++;
      *negative = true;
    }
  లేకపోతే
    *negative = false;

  *endptr = (అక్షరం *) ఆదానం_str;
  అయితే (c_isdigit (**endptr))
    {
      పూర్ణాంకం digit = (**endptr) - '0';

      దొరికింది_digit = true;

      ఒకవేళ (val || digit)
        digits++;

      ఒకవేళ (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      ఒకవేళ (digits > MAX_ACCEPTABLE_DIGITS)
        తిరిగి SSE_అధికప్రవాహం;

      val *= 10;
      val += digit;

      ++(*endptr);

      ఒకవేళ (thousands_sep_నిడివి > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_నిడివి)
          && c_isdigit ((*endptr)[thousands_sep_నిడివి]))
        (*endptr) += thousands_sep_నిడివి;
    }
  ఒకవేళ (! దొరికింది_digit
      && ! STREQ_LEN (*endptr, decimal_బిందువు, decimal_బిందువు_నిడివి))
    తిరిగి SSE_INVALID_సంఖ్య;
  ఒకవేళ (*negative)
    val = -val;

  ఒకవేళ (విలువ)
    *విలువ = val;

  తిరిగి e;
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
స్థిర గణన simple_strtod_error
simple_strtod_float (అక్షరం స్థిరరాశి *ఆదానం_str,
                     అక్షరం **endptr,
                     దీర్ఘ ద్విగుణ *విలువ,
                     size_t *precision)
{
  bool negative;
  గణన simple_strtod_error e = SSE_OK;

  ఒకవేళ (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (ఆదానం_str, endptr, విలువ, &negative);
  ఒకవేళ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    తిరిగి e;

  /* optional decimal point + fraction.  */
  ఒకవేళ (STREQ_LEN (*endptr, decimal_బిందువు, decimal_బిందువు_నిడివి))
    {
      అక్షరం *ptr2;
      దీర్ఘ ద్విగుణ val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_బిందువు_నిడివి;
      గణన simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      ఒకవేళ (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        తిరిగి e2;
      ఒకవేళ (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      ఒకవేళ (neg_frac)
        తిరిగి SSE_INVALID_సంఖ్య;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      ఒకవేళ (విలువ)
        {
          ఒకవేళ (negative)
            *విలువ -= val_frac;
          లేకపోతే
            *విలువ += val_frac;
        }

      ఒకవేళ (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  తిరిగి e;
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
స్థిర గణన simple_strtod_error
simple_strtod_human (అక్షరం స్థిరరాశి *ఆదానం_str,
                     అక్షరం **endptr, దీర్ఘ ద్విగుణ *విలువ, size_t *precision,
                     గణన scale_type allowed_scaling)
{
  పూర్ణాంకం power = 0;
  /* 'scale_auto' is checked below.  */
  పూర్ణాంకం scale_పునాది = default_scale_పునాది (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, ఆదానం_str),
          quote_n (1, decimal_బిందువు),
          MAX_UNSCALED_DIGITS);

  గణన simple_strtod_error e =
    simple_strtod_float (ఆదానం_str, endptr, విలువ, precision);
  ఒకవేళ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    తిరిగి e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *విలువ, (పూర్ణాంకం)*precision);

  అయితే (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      ఒకవేళ (unit_separator)
        {
          size_t sep_len = అక్షరశ్రేణి_పొడవు (unit_separator);
          ఒకవేళ (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      ఒకవేళ (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          ఒకవేళ (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      ఒకవేళ (**endptr == '\0')
        ఆపు;  /* Treat as no suffix.  */

      ఒకవేళ (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          ఒకవేళ (**endptr == '\0')
            ఆపు;

          తిరిగి SSE_INVALID_SUFFIX;
        }

      ఒకవేళ (allowed_scaling == scale_none)
        తిరిగి SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      ఒకవేళ (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_పునాది = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_పునాది);
        }
      లేకపోతే ఒకవేళ (allowed_scaling == scale_IEC_I)
        {
          ఒకవేళ (**endptr == 'i')
            (*endptr)++;
          లేకపోతే
            తిరిగి SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      ఆపు;
    }

  దీర్ఘ ద్విగుణ multiplier = powerld (scale_పునాది, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_పునాది, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*విలువ) = (*విలువ) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *విలువ, *విలువ);

  తిరిగి e;
}


స్థిర శూన్య
simple_strtod_fatal (గణన simple_strtod_error err, అక్షరం స్థిరరాశి *ఆదానం_str)
{
  అక్షరం స్థిరరాశి *msgid = ఖాళీ;

  ఎంపిక (err)
    {
    స్థితి SSE_అధికప్రవాహం:
      msgid = N_("value too large to be converted: %s");
      ఆపు;

    స్థితి SSE_INVALID_సంఖ్య:
      msgid = N_("invalid number: %s");
      ఆపు;

    స్థితి SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      ఆపు;

    స్థితి SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      ఆపు;

    స్థితి SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      ఆపు;

    స్థితి SSE_OK_PRECISION_LOSS:
    స్థితి SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  ఒకవేళ (inval_style != inval_ignore)
    error (conv_exit_సంకేతం, 0, gettext (msgid), quote (ఆదానం_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
స్థిర పూర్ణాంకం
double_గమ్యం_human (దీర్ఘ ద్విగుణ val, పూర్ణాంకం precision,
                 అక్షరం *buf, idx_t buf_కొలత,
                 గణన scale_type scale, పూర్ణాంకం సమూహం, గణన విడత_type విడత_2)
{
  అక్షరం fmt[పరిమాణం "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_వెడల్పు)];
  అక్షరం *pfmt = fmt;
  *pfmt++ = '%';

  ఒకవేళ (సమూహం)
    *pfmt++ = '\'';

  ఒకవేళ (zero_padding_వెడల్పు)
    pfmt += sprintf (pfmt, "0%d", zero_padding_వెడల్పు);

  devmsg ("double_to_human:\n");

  ఒకవేళ (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_విడత (val, విడత_2);
      val /= powerld (10, precision);

      devmsg ((సమూహం) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      తిరిగి snprintf (buf, buf_కొలత, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  ద్విగుణ scale_పునాది = default_scale_పునాది (scale);

  /* Normalize val to scale. */
  పూర్ణాంకం power = 0;
  val = expld (val, scale_పునాది, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_పునాది, power);

  /* Perform rounding. */
  పూర్ణాంకం power_adjust = 0;
  ఒకవేళ (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  లేకపోతే ఒకవేళ (absld (val) < 10)
    {
      /* for values less than 10, we allow one decimal-point digit,
         so adjust before rounding. */
      power_adjust = 1;
    }

  val *= powerld (10, power_adjust);
  val = simple_విడత (val, విడత_2);
  val /= powerld (10, power_adjust);

  /* two special cases after rounding:
     1. a "999.99" can turn into 1000 - so scale down
     2. a "9.99" can turn into 10 - so don't display decimal-point.  */
  ఒకవేళ (absld (val) >= scale_పునాది)
    {
      val /= scale_పునాది;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  పూర్ణాంకం show_decimal_బిందువు = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_పునాది, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  పూర్ణాంకం prec = user_precision == -1 ? show_decimal_బిందువు : user_precision;

  తిరిగి snprintf (buf, buf_కొలత, fmt, prec, val,
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
స్థిర uintmax_t
unit_గమ్యం_umax (అక్షరం స్థిరరాశి *n_string)
{
  strtol_error s_err;
  అక్షరం స్థిరరాశి *c_string = n_string;
  అక్షరం *t_string = ఖాళీ;
  size_t n_len = అక్షరశ్రేణి_పొడవు (n_string);
  అక్షరం *చివర = ఖాళీ;
  uintmax_t n;
  అక్షరం స్థిరరాశి *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  ఒకవేళ (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      చివర = t_string + n_len - 1;
      జ్ఞాపకం_నకలు (t_string, n_string, n_len);

      ఒకవేళ (*చివర == 'i' && 2 <= n_len && ! c_isdigit (*(చివర - 1)))
        *చివర = '\0';
      లేకపోతే
        {
          *++చివర = 'B';
          *++చివర = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &చివర, 10, &n, suffixes);

  ఒకవేళ (s_err != LONGINT_OK || *చివర || n == 0)
    {
      విడుదల (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  విడుదల (t_string);

  తిరిగి n;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [సంఖ్య_2]...\n\
"), కార్యక్రమం_name);
      fputs (_("\
Reformat సంఖ్య_2(s), or the numbers from standard ఆదానం ఒకవేళ none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid ఆదానం\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead యొక్క whitespace కొరకు క్షేత్రం delimiter\n\
"));
      oputs (_("\
      --క్షేత్రం=FIELDS\n\
         replace the numbers లోపల these ఆదానం fields (అప్రమేయ=1);\n\
         see FIELDS క్రింద కొరకు details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use రాయి style floating-బిందువు FORMAT;\n\
         see FORMAT క్రింద కొరకు details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         స్వయంచాలక-scale ఆదానం numbers గమ్యం UNITs; అప్రమేయ is 'none';\n\
         see UNIT క్రింద కొరకు details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the ఆదానం unit కొలత (instead యొక్క the అప్రమేయ 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping యొక్క digits, e.g. 1,000,000.\n\
         This has no effect లోపల the C/POSIX locale\n\
"));
      oputs (_("\
      --శీర్షిక[=N]\n\
         print (without converting) the మొదటి N శీర్షిక పంక్తులు;\n\
         N defaults గమ్యం 1 ఒకవేళ not specified\n\
"));
      oputs (_("\
      --invalid=బహుళకం\n\
         failure బహుళకం_2 కొరకు invalid numbers;\n\
         బహుళకం can be: abort (అప్రమేయ), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output గమ్యం N characters;\n\
         ధనాత్మక N will కుడి-align, negative N will ఎడమ-align;\n\
         padding is ignored ఒకవేళ the output is wider than N;\n\
         the అప్రమేయ is గమ్యం automatically pad ఒకవేళ a whitespace is దొరికింది\n\
"));
      oputs (_("\
      --విడత_2=పద్ధతి\n\
         use పద్ధతి కొరకు rounding when scaling; పద్ధతి can be:\n\
         పైకి, కిందికి, from-zero (అప్రమేయ), towards-zero, సమీప\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         కూడు SUFFIX గమ్యం output numbers,\n\
         and accept an optional SUFFIX లోపల ఆదానం numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         చొప్పించు SEP between సంఖ్య and unit on output,\n\
         and accept an optional SEP లోపల ఆదానం numbers\n\
"));
      oputs (_("\
      --గమ్యం=UNIT\n\
         స్వయంచాలక-scale output numbers గమ్యం UNITs; see UNIT క్రింద\n\
"));
      oputs (_("\
      --గమ్యం-unit=N\n\
         the output unit కొలత (instead యొక్క the అప్రమేయ 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         పంక్తి delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);

      fputs (_("\
\n\
UNIT options:\n"), stdout);
      fputs (_("\
  none       no స్వయంచాలక-scaling is పూర్తి; suffixes will trigger an error\n\
"), stdout);
      fputs (_("\
  స్వయంచాలక       accept optional single/two letter suffix:\n\
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
FIELDS supports cut(1) style క్షేత్రం ranges:\n\
  N    N'th క్షేత్రం, counted from 1\n\
  N-   from N'th క్షేత్రం, గమ్యం చివర యొక్క పంక్తి\n\
  N-M  from N'th to M'th క్షేత్రం (inclusive)\n\
  -M   from మొదటి గమ్యం M'th క్షేత్రం (inclusive)\n\
  -    all fields\n\
గుణిజం_2 fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable కొరకు printing one floating-బిందువు వాదం '%f'.\n\
Optional quote (%'f) will enable --grouping (ఒకవేళ supported by current locale).\n\
Optional వెడల్పు విలువ (%10f) will pad output. Optional zero (%010f) వెడల్పు\n\
will zero pad the సంఖ్య. Optional negative విలువలు (%-10f) will ఎడమ align.\n\
Optional precision (%.1f) will override the ఆదానం determined precision.\n\
"), stdout);

      రాయి (_("\n\
Exit స్థితి_2 is 0 ఒకవేళ all ఆదానం numbers were successfully converted.\n\
By అప్రమేయ, %s will stop at the మొదటి conversion error with నిష్క్రమించు స్థితి_2 2.\n\
With --invalid='fail' a warning is printed కొరకు each conversion error\n\
and the నిష్క్రమించు స్థితి_2 is 2.  With --invalid='warn' each conversion error is\n\
diagnosed, but the నిష్క్రమించు స్థితి_2 is 0.  With --invalid='ignore' conversion\n\
errors are not diagnosed and the నిష్క్రమించు స్థితి_2 is 0.\n\
"), కార్యక్రమం_name);

      రాయి (_("\n\
Examples:\n\
  $ %s --గమ్యం=si 1000\n\
            -> \"1.0k\"\n\
  $ %s --గమ్యం=iec 2048\n\
           -> \"2.0K\"\n\
  $ %s --గమ్యం=iec-i 4096\n\
           -> \"4.0Ki\"\n\
  $ echo 1K | %s --from=si\n\
           -> \"1000\"\n\
  $ echo 1K | %s --from=iec\n\
           -> \"1024\"\n\
  $ df -B1 | %s --శీర్షిక --క్షేత్రం 2-4 --గమ్యం=si\n\
  $ ls -l  | %s --శీర్షిక --క్షేత్రం 5 --గమ్యం=iec\n\
  $ ls -lh | %s --శీర్షిక --క్షేత్రం 5 --from=iec --padding=10\n\
  $ ls -lh | %s --శీర్షిక --క్షేత్రం 5 --from=iec --format %%10f\n"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name,
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name,
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
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
స్థిర శూన్య
parse_format_string (అక్షరం స్థిరరాశి *fmt)
{
  size_t i;
  size_t ఉపసర్గ_len = 0;
  size_t suffix_pos;
  అక్షరం *endptr = ఖాళీ;
  bool zero_padding = false;

  కొరకు (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      ఒకవేళ (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      ఉపసర్గ_len++;
    }

  i++;
  అయితే (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      ఒకవేళ (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      లేకపోతే ఒకవేళ (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      లేకపోతే ఒకవేళ (! skip)
        ఆపు;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  ఒకవేళ (pad != 0)
    {
      ఒకవేళ (debug && padding_వెడల్పు && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      ఒకవేళ (pad < 0)
        padding_వెడల్పు = pad;
      లేకపోతే
        {
          ఒకవేళ (zero_padding)
            zero_padding_వెడల్పు = MIN (pad, గరిష్ఠ_పూర్ణాంకం);
          లేకపోతే
            padding_వెడల్పు = pad;
        }
    }
  i = endptr - fmt;

  ఒకవేళ (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  ఒకవేళ (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      ఒకవేళ (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  ఒకవేళ (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  కొరకు (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    ఒకవేళ (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  ఒకవేళ (ఉపసర్గ_len)
    format_str_ఉపసర్గ = ximemdup0 (fmt, ఉపసర్గ_len);
  ఒకవేళ (fmt[suffix_pos] != '\0')
    format_str_suffix = xstrdup (fmt + suffix_pos);

  devmsg ("format String:\n  input: %s\n  grouping: %s\n"
                   "  padding width: %jd\n"
                   "  prefix: %s\n  suffix: %s\n",
          quote_n (0, fmt), (grouping) ? "yes" : "no",
          padding_వెడల్పు,
          quote_n (1, format_str_ఉపసర్గ ? format_str_ఉపసర్గ : ""),
          quote_n (2, format_str_suffix ? format_str_suffix : ""));
}

/* Parse a numeric value (with optional suffix) from a string.
   Returns a long double value, with input precision.

   If there's an error converting the string to value - exits with
   an error.

   If there are any trailing characters after the number
   (besides a valid suffix) - exits with an error.  */
స్థిర గణన simple_strtod_error
parse_human_సంఖ్య (అక్షరం స్థిరరాశి *str, దీర్ఘ ద్విగుణ /*output */ *విలువ,
                    size_t *precision)
{
  అక్షరం *ptr = ఖాళీ;

  గణన simple_strtod_error e =
    simple_strtod_human (str, &ptr, విలువ, precision, scale_from);
  ఒకవేళ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      తిరిగి e;
    }

  ఒకవేళ (ptr && *ptr != '\0')
    {
      ఒకవేళ (inval_style != inval_ignore)
        error (conv_exit_సంకేతం, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  తిరిగి e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
స్థిర bool
prepare_padded_సంఖ్య (స్థిరరాశి దీర్ఘ ద్విగుణ val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_వాడిన = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  పూర్ణాంకం x;
  expld (val, 10, &x);

  ఒకవేళ (scale_గమ్యం == scale_none
      && x + precision_వాడిన > MAX_UNSCALED_DIGITS)
    {
      ఒకవేళ (inval_style != inval_ignore)
        {
          ఒకవేళ (precision_వాడిన)
            error (conv_exit_సంకేతం, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_వాడిన);
          లేకపోతే
            error (conv_exit_సంకేతం, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      తిరిగి false;
    }

  ఒకవేళ (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      ఒకవేళ (inval_style != inval_ignore)
        error (conv_exit_సంకేతం, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      తిరిగి false;
    }

  అయితే (true)
    {
      పూర్ణాంకం numlen = double_గమ్యం_human (val, precision_వాడిన,
                                    padding_బఫరు, padding_బఫరు_కొలత,
                                    scale_గమ్యం, grouping, విడత_style);
      ptrdiff_t growth;
      ఒకవేళ (numlen < 0 || ckd_sub (&growth, numlen, padding_బఫరు_కొలత - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      ఒకవేళ (growth <= 0)
        ఆపు;
      padding_బఫరు = xpalloc (padding_బఫరు, &padding_బఫరు_కొలత,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_బఫరు));

  intmax_t pad = 0;
  ఒకవేళ (padding_వెడల్పు)
    {
      పూర్ణాంకం buf_వెడల్పు = mbswidth (padding_బఫరు,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      ఒకవేళ (0 <= buf_వెడల్పు)
        {
          ఒకవేళ (padding_వెడల్పు < 0)
            {
              ఒకవేళ (padding_వెడల్పు < -buf_వెడల్పు)
                pad = padding_వెడల్పు + buf_వెడల్పు;
            }
          లేకపోతే
            {
              ఒకవేళ (buf_వెడల్పు < padding_వెడల్పు)
                pad = padding_వెడల్పు - buf_వెడల్పు;
            }
        }
    }

  *padding = pad;
  తిరిగి true;
}

స్థిర శూన్య
print_padded_సంఖ్య (intmax_t padding)
{
  ఒకవేళ (format_str_ఉపసర్గ)
    fputs (format_str_ఉపసర్గ, stdout);

  కొరకు (intmax_t p = padding; 0 < p; p--)
    అక్షరం_రాయి (' ');

  fputs (padding_బఫరు, stdout);

  కొరకు (intmax_t p = padding; p < 0; p++)
    అక్షరం_రాయి (' ');

  ఒకవేళ (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
స్థిర bool
process_suffixed_సంఖ్య (అక్షరం *పాఠ్యం, దీర్ఘ ద్విగుణ *ఫలితం,
                         size_t *precision, దీర్ఘ పూర్ణాంకం క్షేత్రం)
{
  అక్షరం saved_suffix = '\0';

  ఒకవేళ (suffix)
    {
      ఒకవేళ (mbs_endswith (పాఠ్యం, suffix))
        {
          saved_suffix = *(పాఠ్యం + అక్షరశ్రేణి_పొడవు (పాఠ్యం) - అక్షరశ్రేణి_పొడవు (suffix));
          *(పాఠ్యం + అక్షరశ్రేణి_పొడవు (పాఠ్యం) - అక్షరశ్రేణి_పొడవు (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      లేకపోతే
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  అక్షరం *p = skip_str_matching (పాఠ్యం, newline_or_blank, true);

  /* setup auto-padding.  */
  ఒకవేళ (auto_padding)
    {
      padding_వెడల్పు = పాఠ్యం < p || 1 < క్షేత్రం
                      ? mbswidth (పాఠ్యం,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      ఒకవేళ (padding_వెడల్పు < 0)
        padding_వెడల్పు = అక్షరశ్రేణి_పొడవు (పాఠ్యం);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_వెడల్పు);
    }

  దీర్ఘ ద్విగుణ val = 0;
  గణన simple_strtod_error e = parse_human_సంఖ్య (p, &val, precision);
  ఒకవేళ (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  ఒకవేళ (from_unit_కొలత != 1 || గమ్యం_unit_కొలత != 1)
    val = (val * from_unit_కొలత) / గమ్యం_unit_కొలత;

  *ఫలితం = val;

  ఒకవేళ (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    తిరిగి true;
  లేకపోతే
    {
      ఒకవేళ (saved_suffix)
        *(పాఠ్యం + అక్షరశ్రేణి_పొడవు (పాఠ్యం)) = saved_suffix;
      తిరిగి false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_శుద్ధ
స్థిర అక్షరం *
mbsmbchr (అక్షరం స్థిరరాశి *s, అక్షరం స్థిరరాశి *c)
{
  గుర్తులేని అక్షరం uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  ఒకవేళ (uc < 0x30 || MB_CUR_MAX == 1)
    తిరిగి (అక్షరం *) strchr (s, uc);
  లేకపోతే ఒకవేళ (is_utf8_charset ())
    తిరిగి (అక్షరం *) (uc < 0x80 ? strchr (s, uc) : అక్షరశ్రేణి_శోధన (s, c));
  లేకపోతే
    తిరిగి *(c + 1) == '\0' ? mbschr (s, uc) : (అక్షరం *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
స్థిర అక్షరం *
తదుపరి_క్షేత్రం (అక్షరం **పంక్తి)
{
  అక్షరం *క్షేత్రం_ప్రారంభం = *పంక్తి;
  అక్షరం *క్షేత్రం_చివర   = క్షేత్రం_ప్రారంభం;

  ఒకవేళ (delimiter)
    {
      ఒకవేళ (! (క్షేత్రం_చివర = mbsmbchr (క్షేత్రం_ప్రారంభం, delimiter)))
        క్షేత్రం_చివర = strchr (క్షేత్రం_ప్రారంభం, '\0');
    }
  లేకపోతే
    {
      /* keep any space prefix in the returned field */
      క్షేత్రం_చివర = skip_str_matching (క్షేత్రం_చివర, newline_or_blank, true);
      క్షేత్రం_చివర = skip_str_matching (క్షేత్రం_చివర, newline_or_blank, false);
    }

  *పంక్తి = క్షేత్రం_చివర;
  తిరిగి క్షేత్రం_ప్రారంభం;
}

ATTRIBUTE_శుద్ధ
స్థిర bool
include_క్షేత్రం (uintmax_t క్షేత్రం)
{
  నిర్మాణం క్షేత్రం_range_pair *p = frp;
  ఒకవేళ (!p)
    తిరిగి క్షేత్రం == 1;

  అయితే (p->lo != UINTMAX_MAX)
    {
      ఒకవేళ (p->lo <= క్షేత్రం && p->hi >= క్షేత్రం)
        తిరిగి true;
      ++p;
    }
  తిరిగి false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
స్థిర bool
process_క్షేత్రం (అక్షరం *పాఠ్యం, uintmax_t క్షేత్రం)
{
  దీర్ఘ ద్విగుణ val = 0;
  size_t precision = 0;
  bool valid_సంఖ్య = true;

  ఒకవేళ (include_క్షేత్రం (క్షేత్రం))
    {
      valid_సంఖ్య =
        process_suffixed_సంఖ్య (పాఠ్యం, &val, &precision, క్షేత్రం);

      intmax_t padding;
      ఒకవేళ (valid_సంఖ్య)
        valid_సంఖ్య = prepare_padded_సంఖ్య (val, precision, &padding);

      ఒకవేళ (valid_సంఖ్య)
        print_padded_సంఖ్య (padding);
      లేకపోతే
        fputs (పాఠ్యం, stdout);
    }
  లేకపోతే
    fputs (పాఠ్యం, stdout);

  తిరిగి valid_సంఖ్య;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
స్థిర పూర్ణాంకం
process_పంక్తి (అక్షరం *పంక్తి, bool newline)
{
  అక్షరం *తదుపరి;
  uintmax_t క్షేత్రం = 0;
  bool valid_సంఖ్య = true;

  అయితే (true) {
    ++క్షేత్రం;
    తదుపరి = తదుపరి_క్షేత్రం (&పంక్తి);

    ఒకవేళ (*పంక్తి != '\0')
      {
        /* NUL terminate the current field string and process */
        అక్షరం చివర_క్షేత్రం = *పంక్తి;
        *పంక్తి = '\0';

        ఒకవేళ (! process_క్షేత్రం (తదుపరి, క్షేత్రం))
          valid_సంఖ్య = false;

        ఒకవేళ (delimiter != ఖాళీ)
          fputs (delimiter, stdout);
        లేకపోతే
          fputc (' ', stdout);

        ఒకవేళ (delimiter)
          పంక్తి += MAX (అక్షరశ్రేణి_పొడవు (delimiter), 1);
        లేకపోతే
          {
            *పంక్తి = చివర_క్షేత్రం;
            mcel_t g = mcel_scanz (పంక్తి);
            పంక్తి += g.len;
          }
      }
    లేకపోతే
      {
        /* end of the line, process the last field and finish */
        ఒకవేళ (! process_క్షేత్రం (తదుపరి, క్షేత్రం))
          valid_సంఖ్య = false;

        ఆపు;
      }
  }

  ఒకవేళ (newline)
    అక్షరం_రాయి (పంక్తి_delim);

 ఒకవేళ (ferror (stdout))
   write_error ();

  తిరిగి valid_సంఖ్య;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#ఒకవేళ HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#అయితే_ముగింపు

  decimal_బిందువు = nl_langinfo (RADIXCHAR);
  ఒకవేళ (decimal_బిందువు == ఖాళీ || అక్షరశ్రేణి_పొడవు (decimal_బిందువు) == 0)
    decimal_బిందువు = ".";
  decimal_బిందువు_నిడివి = అక్షరశ్రేణి_పొడవు (decimal_బిందువు);

  thousands_sep = nl_langinfo (THOUSEP);
  ఒకవేళ (thousands_sep == ఖాళీ)
    thousands_sep = "";
  thousands_sep_నిడివి = అక్షరశ్రేణి_పొడవు (thousands_sep);

  atexit (close_stdout);

  అయితే (true)
    {
      పూర్ణాంకం c = getopt_long (argc, argv, "d:z", longopts, ఖాళీ);

      ఒకవేళ (c == -1)
        ఆపు;

      ఎంపిక (c)
        {
        స్థితి FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_వాదనలు, scale_from_types);
          ఆపు;

        స్థితి FROM_UNIT_OPTION:
          from_unit_కొలత = unit_గమ్యం_umax (optarg);
          ఆపు;

        స్థితి గమ్యం_OPTION:
          scale_గమ్యం =
            XARGMATCH ("--to", optarg, scale_గమ్యం_వాదనలు, scale_గమ్యం_types);
          ఆపు;

        స్థితి గమ్యం_UNIT_OPTION:
          గమ్యం_unit_కొలత = unit_గమ్యం_umax (optarg);
          ఆపు;

        స్థితి విడత_OPTION:
          విడత_style = XARGMATCH ("--round", optarg, విడత_వాదనలు, విడత_types);
          ఆపు;

        స్థితి GROUPING_OPTION:
          grouping = 1;
          ఆపు;

        స్థితి PADDING_OPTION:
          ఒకవేళ (((xstrtoimax (optarg, ఖాళీ, 10, &padding_వెడల్పు, "")
                & ~LONGINT_అధికప్రవాహం)
               != LONGINT_OK)
              || padding_వెడల్పు == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          ఆపు;

        స్థితి క్షేత్రం_OPTION:
          ఒకవేళ (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          సమితి_fields (optarg, SETFLD_ALLOW_DASH);
          ఆపు;

        స్థితి 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          ఒకవేళ (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              ఒకవేళ (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          ఆపు;

        స్థితి 'z':
          పంక్తి_delim = '\0';
          ఆపు;

        స్థితి SUFFIX_OPTION:
          suffix = optarg;
          ఆపు;

        స్థితి UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          ఆపు;

        స్థితి DEBUG_OPTION:
          debug = true;
          ఆపు;

        స్థితి DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          ఆపు;

        స్థితి శీర్షిక_OPTION:
          ఒకవేళ (optarg)
            {
              ఒకవేళ (xstrtoumax (optarg, ఖాళీ, 10, &శీర్షిక, "") != LONGINT_OK
                  || శీర్షిక == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          లేకపోతే
            {
              శీర్షిక = 1;
            }
          ఆపు;

        స్థితి FORMAT_OPTION:
          format_str = optarg;
          ఆపు;

        స్థితి INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_వాదనలు, inval_types);
          ఆపు;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (format_str != ఖాళీ && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  ఒకవేళ (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  ఒకవేళ (debug && scale_from == scale_none && scale_గమ్యం == scale_none
      && !grouping && (padding_వెడల్పు == 0) && (format_str == ఖాళీ))
    error (0, 0, _("no conversion option specified"));

  ఒకవేళ (debug && unit_separator && delimiter == ఖాళీ)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  ఒకవేళ (format_str)
    parse_format_string (format_str);

  ఒకవేళ (grouping)
    {
      ఒకవేళ (scale_గమ్యం != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      ఒకవేళ (debug && thousands_sep_నిడివి == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_వెడల్పు == 0 && delimiter == ఖాళీ);

  ఒకవేళ (inval_style != inval_abort)
    conv_exit_సంకేతం = 0;

  ఒకవేళ (argc > optind)
    {
      ఒకవేళ (debug && శీర్షిక)
        error (0, 0, _("--header ignored with command-line input"));

      కొరకు (; optind < argc; optind++)
        valid_numbers &= process_పంక్తి (argv[optind], true);
    }
  లేకపోతే
    {
      అక్షరం *పంక్తి = ఖాళీ;
      size_t పంక్తి_allocated = 0;
      ssize_t len;

      అయితే (శీర్షిక-- && getdelim (&పంక్తి, &పంక్తి_allocated,
                                   పంక్తి_delim, stdin) > 0)
        {
          ఒకవేళ (fputs (పంక్తి, stdout) == EOF)
            write_error ();
        }

      అయితే ((len = getdelim (&పంక్తి, &పంక్తి_allocated,
                              పంక్తి_delim, stdin)) > 0)
        {
          bool newline = పంక్తి[len - 1] == పంక్తి_delim;
          ఒకవేళ (newline)
            పంక్తి[len - 1] = '\0';
          valid_numbers &= process_పంక్తి (పంక్తి, newline);
        }

      ఒకవేళ (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  ఒకవేళ (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  పూర్ణాంకం exit_స్థితి = EXIT_SUCCESS;
  ఒకవేళ (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_స్థితి = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_స్థితి);
}
