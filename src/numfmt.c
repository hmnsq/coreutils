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

#समावेश <config.h>
#समावेश <float.h>
#समावेश <getopt.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <langinfo.h>

#समावेश "argmatch.h"
#समावेश "assure.h"
#समावेश "c-ctype.h"
#समावेश "mbswidth.h"
#समावेश "mcel.h"
#समावेश "quote.h"
#समावेश "skipchars.h"
#समावेश "system.h"
#समावेश "utf8.h"
#समावेश "xstrtol.h"

#समावेश "set-fields.h"

#अगर HAVE_FPSETPREC
# include <ieeefp.h>
#पूर्ण_अगर

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "numfmt"

#घोषणा AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
क्रमागत { EXIT_CONVERSION_WARNINGS = 2 };

क्रमागत
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

क्रमागत scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

अटल अक्षर स्थिर *स्थिर scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", शून्य
};

अटल क्रमागत scale_type स्थिर scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

अटल अक्षर स्थिर *स्थिर scale_to_args[] =
{
  "none", "si", "iec", "iec-i", शून्य
};

अटल क्रमागत scale_type स्थिर scale_to_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


क्रमागत round_type
{
  round_ceiling,
  round_floor,
  round_from_zero,
  round_to_zero,
  round_nearest,
};

अटल अक्षर स्थिर *स्थिर round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", शून्य
};

अटल क्रमागत round_type स्थिर round_types[] =
{
  round_ceiling, round_floor, round_from_zero, round_to_zero, round_nearest
};


क्रमागत inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

अटल अक्षर स्थिर *स्थिर inval_args[] =
{
  "abort", "fail", "warn", "ignore", शून्य
};

अटल क्रमागत inval_type स्थिर inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"from", required_argument, शून्य, FROM_OPTION},
  {"from-unit", required_argument, शून्य, FROM_UNIT_OPTION},
  {"to", required_argument, शून्य, TO_OPTION},
  {"to-unit", required_argument, शून्य, TO_UNIT_OPTION},
  {"round", required_argument, शून्य, ROUND_OPTION},
  {"padding", required_argument, शून्य, PADDING_OPTION},
  {"suffix", required_argument, शून्य, SUFFIX_OPTION},
  {"unit-separator", required_argument, शून्य, UNIT_SEPARATOR_OPTION},
  {"grouping", no_argument, शून्य, GROUPING_OPTION},
  {"delimiter", required_argument, शून्य, 'd'},
  {"field", required_argument, शून्य, FIELD_OPTION},
  {"debug", no_argument, शून्य, DEBUG_OPTION},
  {"-debug", no_argument, शून्य, DEV_DEBUG_OPTION},
  {"header", optional_argument, शून्य, HEADER_OPTION},
  {"format", required_argument, शून्य, FORMAT_OPTION},
  {"invalid", required_argument, शून्य, INVALID_OPTION},
  {"zero-terminated", no_argument, शून्य, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
क्रमागत { MAX_UNSCALED_DIGITS = द_द्वि_भग्न };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
क्रमागत { MAX_ACCEPTABLE_DIGITS = 33 };

अटल क्रमागत scale_type scale_from = scale_none;
अटल क्रमागत scale_type scale_to = scale_none;
अटल क्रमागत round_type round_style = round_from_zero;
अटल क्रमागत inval_type inval_style = inval_abort;
अटल अक्षर स्थिर *suffix = शून्य;
अटल अक्षर स्थिर *unit_separator = शून्य;
अटल uintmax_t from_unit_आकार = 1;
अटल uintmax_t to_unit_आकार = 1;
अटल पूर्णांक grouping = 0;
अटल अक्षर *padding_बफर = शून्य;
अटल idx_t padding_बफर_आकार = 0;
अटल intmax_t padding_width = 0;
अटल पूर्णांक zero_padding_width = 0;
अटल दीर्घ पूर्णांक user_precision = -1;
अटल अक्षर स्थिर *format_str = शून्य;
अटल अक्षर *format_str_prefix = शून्य;
अटल अक्षर *format_str_suffix = शून्य;

/* By default, any conversion error will terminate the program.  */
अटल पूर्णांक conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
अटल पूर्णांक auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
अटल अक्षर स्थिर *delimiter = शून्य;

/* line delimiter.  */
अटल अचिन्हित अक्षर पंक्ति_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
अटल uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
अटल bool debug;

/* will be set according to the current locale.  */
अटल अक्षर स्थिर *decimal_point;
अटल पूर्णांक decimal_point_लंबाई;
अटल अक्षर स्थिर *thousands_sep;
अटल पूर्णांक thousands_sep_लंबाई;

/* debugging for developers.  Enables devmsg().  */
अटल bool dev_debug = false;

अटल bool
newline_or_blank (mcel_t g)
{
  वापस g.ch == '\n' || c32issep (g.ch);
}

अटल अंतरभूत पूर्णांक
default_scale_base (क्रमागत scale_type scale)
{
  चयन (scale)
    {
    हाल scale_IEC:
    हाल scale_IEC_I:
      वापस 1024;

    हाल scale_none:
    हाल scale_auto:
    हाल scale_SI:
    शेष:
      वापस 1000;
    }
}

अटल अक्षर स्थिर zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
अटल अक्षर स्थिर *valid_suffixes = 1 + zero_and_valid_suffixes;

अटल अंतरभूत bool
valid_suffix (स्थिर अक्षर suf)
{
  वापस म_अक्षर (valid_suffixes, suf) != शून्य;
}

अटल अंतरभूत पूर्णांक
suffix_power (स्थिर अक्षर suf)
{
  चयन (suf)
    {
    हाल 'k':                  /* kilo.  */
    हाल 'K':                  /* kilo or kibi.  */
      वापस 1;

    हाल 'M':                  /* mega or mebi.  */
      वापस 2;

    हाल 'G':                  /* giga or gibi.  */
      वापस 3;

    हाल 'T':                  /* tera or tebi.  */
      वापस 4;

    हाल 'P':                  /* peta or pebi.  */
      वापस 5;

    हाल 'E':                  /* exa or exbi.  */
      वापस 6;

    हाल 'Z':                  /* zetta or 2**70.  */
      वापस 7;

    हाल 'Y':                  /* yotta or 2**80.  */
      वापस 8;

    हाल 'R':                  /* ronna or 2**90.  */
      वापस 9;

    हाल 'Q':                  /* quetta or 2**100.  */
      वापस 10;

    शेष:                   /* should never happen. assert?  */
      वापस 0;
    }
}

अटल अंतरभूत अक्षर स्थिर *
suffix_power_char (पूर्णांक power)
{
  चयन (power)
    {
    हाल 0:
      वापस "";

    हाल 1:
      वापस "K";

    हाल 2:
      वापस "M";

    हाल 3:
      वापस "G";

    हाल 4:
      वापस "T";

    हाल 5:
      वापस "P";

    हाल 6:
      वापस "E";

    हाल 7:
      वापस "Z";

    हाल 8:
      वापस "Y";

    हाल 9:
      वापस "R";

    हाल 10:
      वापस "Q";

    शेष:
      वापस "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
अटल दीर्घ द्विगुन
powerld (दीर्घ द्विगुन base, सूचक_भेद_प्रकार x)
{
  दीर्घ द्विगुन परिणाम = base;
  अगर (x == 0)
    वापस 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  जबतक (--x)
    परिणाम *= base;
  वापस परिणाम;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
अटल अंतरभूत दीर्घ द्विगुन
absld (दीर्घ द्विगुन val)
{
  वापस val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
अटल दीर्घ द्विगुन
expld (दीर्घ द्विगुन val, पूर्णांक base, पूर्णांक /*output */ *x)
{
  पूर्णांक power = 0;

  अगर (val >= -द_द्वि_उच्च && val <= द_द्वि_उच्च)
    {
      जबतक (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  अगर (x)
    *x = power;
  वापस val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
अटल अंतरभूत intmax_t
simple_round_ceiling (दीर्घ द्विगुन val)
{
  intmax_t intval = val;
  अगर (intval < val)
    intval++;
  वापस intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
अटल अंतरभूत intmax_t
simple_round_floor (दीर्घ द्विगुन val)
{
  वापस -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
अटल अंतरभूत intmax_t
simple_round_from_zero (दीर्घ द्विगुन val)
{
  वापस val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
अटल अंतरभूत intmax_t
simple_round_to_zero (दीर्घ द्विगुन val)
{
  वापस val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
अटल अंतरभूत intmax_t
simple_round_nearest (दीर्घ द्विगुन val)
{
  वापस val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
अटल अंतरभूत दीर्घ द्विगुन
simple_round (दीर्घ द्विगुन val, क्रमागत round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (दीर्घ द्विगुन) INTMAX_MAX * intmax_mul;

  चयन (t)
    {
    हाल round_ceiling:
      rval = simple_round_ceiling (val);
      अवरोध;

    हाल round_floor:
      rval = simple_round_floor (val);
      अवरोध;

    हाल round_from_zero:
      rval = simple_round_from_zero (val);
      अवरोध;

    हाल round_to_zero:
      rval = simple_round_to_zero (val);
      अवरोध;

    हाल round_nearest:
      rval = simple_round_nearest (val);
      अवरोध;

    शेष:
      /* to silence the compiler - this should never happen.  */
      वापस 0;
    }

  वापस (दीर्घ द्विगुन) INTMAX_MAX * intmax_mul + rval;
}

क्रमागत simple_strtod_error
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
अटल क्रमागत simple_strtod_error
simple_strtod_int (अक्षर स्थिर *निवेश_str,
                   अक्षर **endptr, दीर्घ द्विगुन *मूल्य, bool *negative)
{
  क्रमागत simple_strtod_error e = SSE_OK;

  दीर्घ द्विगुन val = 0;
  पूर्णांक digits = 0;
  bool found_digit = false;

  अगर (*निवेश_str == '-')
    {
      निवेश_str++;
      *negative = true;
    }
  अन्यथा
    *negative = false;

  *endptr = (अक्षर *) निवेश_str;
  जबतक (c_isdigit (**endptr))
    {
      पूर्णांक digit = (**endptr) - '0';

      found_digit = true;

      अगर (val || digit)
        digits++;

      अगर (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      अगर (digits > MAX_ACCEPTABLE_DIGITS)
        वापस SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      अगर (thousands_sep_लंबाई > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_लंबाई)
          && c_isdigit ((*endptr)[thousands_sep_लंबाई]))
        (*endptr) += thousands_sep_लंबाई;
    }
  अगर (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_लंबाई))
    वापस SSE_INVALID_संख्या;
  अगर (*negative)
    val = -val;

  अगर (मूल्य)
    *मूल्य = val;

  वापस e;
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
अटल क्रमागत simple_strtod_error
simple_strtod_float (अक्षर स्थिर *निवेश_str,
                     अक्षर **endptr,
                     दीर्घ द्विगुन *मूल्य,
                     माप_प्रकार *precision)
{
  bool negative;
  क्रमागत simple_strtod_error e = SSE_OK;

  अगर (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (निवेश_str, endptr, मूल्य, &negative);
  अगर (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    वापस e;

  /* optional decimal point + fraction.  */
  अगर (STREQ_LEN (*endptr, decimal_point, decimal_point_लंबाई))
    {
      अक्षर *ptr2;
      दीर्घ द्विगुन val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_लंबाई;
      क्रमागत simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      अगर (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        वापस e2;
      अगर (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      अगर (neg_frac)
        वापस SSE_INVALID_संख्या;

      /* number of digits in the fractions.  */
      सूचक_भेद_प्रकार exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      अगर (मूल्य)
        {
          अगर (negative)
            *मूल्य -= val_frac;
          अन्यथा
            *मूल्य += val_frac;
        }

      अगर (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  वापस e;
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
अटल क्रमागत simple_strtod_error
simple_strtod_human (अक्षर स्थिर *निवेश_str,
                     अक्षर **endptr, दीर्घ द्विगुन *मूल्य, माप_प्रकार *precision,
                     क्रमागत scale_type allowed_scaling)
{
  पूर्णांक power = 0;
  /* 'scale_auto' is checked below.  */
  पूर्णांक scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, निवेश_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  क्रमागत simple_strtod_error e =
    simple_strtod_float (निवेश_str, endptr, मूल्य, precision);
  अगर (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    वापस e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *मूल्य, (पूर्णांक)*precision);

  जबतक (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      अगर (unit_separator)
        {
          माप_प्रकार sep_len = म_माप (unit_separator);
          अगर (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      अगर (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          अगर (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      अगर (**endptr == '\0')
        अवरोध;  /* Treat as no suffix.  */

      अगर (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          अगर (**endptr == '\0')
            अवरोध;

          वापस SSE_INVALID_SUFFIX;
        }

      अगर (allowed_scaling == scale_none)
        वापस SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      अगर (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      अन्यथा अगर (allowed_scaling == scale_IEC_I)
        {
          अगर (**endptr == 'i')
            (*endptr)++;
          अन्यथा
            वापस SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      अवरोध;
    }

  दीर्घ द्विगुन multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*मूल्य) = (*मूल्य) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *मूल्य, *मूल्य);

  वापस e;
}


अटल व्योम
simple_strtod_fatal (क्रमागत simple_strtod_error err, अक्षर स्थिर *निवेश_str)
{
  अक्षर स्थिर *msgid = शून्य;

  चयन (err)
    {
    हाल SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      अवरोध;

    हाल SSE_INVALID_संख्या:
      msgid = N_("invalid number: %s");
      अवरोध;

    हाल SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      अवरोध;

    हाल SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      अवरोध;

    हाल SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      अवरोध;

    हाल SSE_OK_PRECISION_LOSS:
    हाल SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  अगर (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (निवेश_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
अटल पूर्णांक
double_to_human (दीर्घ द्विगुन val, पूर्णांक precision,
                 अक्षर *buf, idx_t buf_आकार,
                 क्रमागत scale_type scale, पूर्णांक group, क्रमागत round_type round)
{
  अक्षर fmt[माप "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  अक्षर *pfmt = fmt;
  *pfmt++ = '%';

  अगर (group)
    *pfmt++ = '\'';

  अगर (zero_padding_width)
    pfmt += प्र_लिखो (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  अगर (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      म_नकल (pfmt, ".*Lf%s");

      वापस snprintf (buf, buf_आकार, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  द्विगुन scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  पूर्णांक power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  पूर्णांक power_adjust = 0;
  अगर (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  अन्यथा अगर (absld (val) < 10)
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
  अगर (absld (val) >= scale_base)
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

  वापस snprintf (buf, buf_आकार, fmt, prec, val,
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
अटल uintmax_t
unit_to_umax (अक्षर स्थिर *n_string)
{
  strtol_error s_err;
  अक्षर स्थिर *c_string = n_string;
  अक्षर *t_string = शून्य;
  माप_प्रकार n_len = म_माप (n_string);
  अक्षर *end = शून्य;
  uintmax_t n;
  अक्षर स्थिर *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  अगर (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      स_नकल (t_string, n_string, n_len);

      अगर (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
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

  अगर (s_err != LONGINT_OK || *end || n == 0)
    {
      मुक्त (t_string);
      error (निकास_त्रुटि, 0, _("invalid unit size: %s"), quote (n_string));
    }

  मुक्त (t_string);

  वापस n;
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [संख्या_2]...\n\
"), program_name);
      ख_माला_दो (_("\
Reformat संख्या_2(s), or the numbers from standard निवेश अगर none are specified.\n\
"), मानक_निकास);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid निवेश\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace क्रम field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these निवेश fields (शेष=1);\n\
         see FIELDS below क्रम details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use म_लिखो style floating-point FORMAT;\n\
         see FORMAT below क्रम details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         स्वतः-scale निवेश numbers to UNITs; शेष is 'none';\n\
         see UNIT below क्रम details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the निवेश unit आकार (instead of the शेष 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header पंक्तियाँ;\n\
         N defaults to 1 अगर not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode क्रम invalid numbers;\n\
         MODE can be: पात (शेष), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will दायाँ-align, negative N will बायाँ-align;\n\
         padding is ignored अगर the output is wider than N;\n\
         the शेष is to automatically pad अगर a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD क्रम rounding when scaling; METHOD can be:\n\
         up, down, from-zero (शेष), towards-zero, nearest\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         add SUFFIX to output numbers,\n\
         and accept an optional SUFFIX in निवेश numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         जोड़ो SEP between संख्या and unit on output,\n\
         and accept an optional SEP in निवेश numbers\n\
"));
      oputs (_("\
      --to=UNIT\n\
         स्वतः-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit आकार (instead of the शेष 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पंक्ति delimiter is NUL, not newline\n\
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
  N-   from N'th field, to end of पंक्ति\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), मानक_निकास);

      ख_माला_दो (_("\n\
FORMAT must be suitable क्रम printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (अगर supported by current locale).\n\
Optional width मूल्य (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the संख्या. Optional negative मूल्यावली (%-10f) will बायाँ align.\n\
Optional precision (%.1f) will override the निवेश determined precision.\n\
"), मानक_निकास);

      म_लिखो (_("\n\
Exit status is 0 अगर all निवेश numbers were successfully converted.\n\
By शेष, %s will stop at the first conversion error with निकास status 2.\n\
With --invalid='fail' a warning is printed क्रम each conversion error\n\
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
अटल व्योम
parse_format_string (अक्षर स्थिर *fmt)
{
  माप_प्रकार i;
  माप_प्रकार prefix_len = 0;
  माप_प्रकार suffix_pos;
  अक्षर *endptr = शून्य;
  bool zero_padding = false;

  क्रम (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      अगर (!fmt[i])
        error (निकास_त्रुटि, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  जबतक (true)
    {
      माप_प्रकार skip = म_अखोज (fmt + i, " ");
      i += skip;
      अगर (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      अन्यथा अगर (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      अन्यथा अगर (! skip)
        अवरोध;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  अगर (pad != 0)
    {
      अगर (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      अगर (pad < 0)
        padding_width = pad;
      अन्यथा
        {
          अगर (zero_padding)
            zero_padding_width = MIN (pad, पूर्णांक_उच्च);
          अन्यथा
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  अगर (fmt[i] == '\0')
    error (निकास_त्रुटि, 0, _("format %s ends in %%"), quote (fmt));

  अगर (fmt[i] == '.')
    {
      i++;
      त्रुटि_सं = 0;
      user_precision = म_से_दीर्घ (fmt + i, &endptr, 10);
      अगर (त्रुटि_सं == दुस्फल || user_precision < 0 || SIZE_MAX < user_precision
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

  अगर (fmt[i] != 'f')
    error (निकास_त्रुटि, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  क्रम (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    अगर (fmt[i] == '%' && fmt[i + 1] != '%')
      error (निकास_त्रुटि, 0, _("format %s has too many %% directives"),
             quote (fmt));

  अगर (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  अगर (fmt[suffix_pos] != '\0')
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
अटल क्रमागत simple_strtod_error
parse_human_संख्या (अक्षर स्थिर *str, दीर्घ द्विगुन /*output */ *मूल्य,
                    माप_प्रकार *precision)
{
  अक्षर *ptr = शून्य;

  क्रमागत simple_strtod_error e =
    simple_strtod_human (str, &ptr, मूल्य, precision, scale_from);
  अगर (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      वापस e;
    }

  अगर (ptr && *ptr != '\0')
    {
      अगर (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  वापस e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
अटल bool
prepare_padded_संख्या (स्थिर दीर्घ द्विगुन val, माप_प्रकार precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  माप_प्रकार precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  पूर्णांक x;
  expld (val, 10, &x);

  अगर (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      अगर (inval_style != inval_ignore)
        {
          अगर (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          अन्यथा
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      वापस false;
    }

  अगर (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      अगर (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      वापस false;
    }

  जबतक (true)
    {
      पूर्णांक numlen = double_to_human (val, precision_used,
                                    padding_बफर, padding_बफर_आकार,
                                    scale_to, grouping, round_style);
      सूचक_भेद_प्रकार growth;
      अगर (numlen < 0 || ckd_sub (&growth, numlen, padding_बफर_आकार - 1))
        error (निकास_त्रुटि, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      अगर (growth <= 0)
        अवरोध;
      padding_बफर = xpalloc (padding_बफर, &padding_बफर_आकार,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_बफर));

  intmax_t pad = 0;
  अगर (padding_width)
    {
      पूर्णांक buf_width = mbswidth (padding_बफर,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      अगर (0 <= buf_width)
        {
          अगर (padding_width < 0)
            {
              अगर (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          अन्यथा
            {
              अगर (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  वापस true;
}

अटल व्योम
print_padded_संख्या (intmax_t padding)
{
  अगर (format_str_prefix)
    ख_माला_दो (format_str_prefix, मानक_निकास);

  क्रम (intmax_t p = padding; 0 < p; p--)
    अक्षर_दो (' ');

  ख_माला_दो (padding_बफर, मानक_निकास);

  क्रम (intmax_t p = padding; p < 0; p++)
    अक्षर_दो (' ');

  अगर (format_str_suffix)
    ख_माला_दो (format_str_suffix, मानक_निकास);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
अटल bool
process_suffixed_संख्या (अक्षर *text, दीर्घ द्विगुन *परिणाम,
                         माप_प्रकार *precision, दीर्घ पूर्णांक field)
{
  अक्षर saved_suffix = '\0';

  अगर (suffix)
    {
      अगर (mbs_endswith (text, suffix))
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
  अगर (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      अगर (padding_width < 0)
        padding_width = म_माप (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  दीर्घ द्विगुन val = 0;
  क्रमागत simple_strtod_error e = parse_human_संख्या (p, &val, precision);
  अगर (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  अगर (from_unit_आकार != 1 || to_unit_आकार != 1)
    val = (val * from_unit_आकार) / to_unit_आकार;

  *परिणाम = val;

  अगर (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    वापस true;
  अन्यथा
    {
      अगर (saved_suffix)
        *(text + म_माप (text)) = saved_suffix;
      वापस false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
अटल अक्षर *
mbsmbchr (अक्षर स्थिर *s, अक्षर स्थिर *c)
{
  अचिन्हित अक्षर uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  अगर (uc < 0x30 || MB_CUR_MAX == 1)
    वापस (अक्षर *) म_अक्षर (s, uc);
  अन्यथा अगर (is_utf8_charset ())
    वापस (अक्षर *) (uc < 0x80 ? म_अक्षर (s, uc) : म_माला (s, c));
  अन्यथा
    वापस *(c + 1) == '\0' ? mbschr (s, uc) : (अक्षर *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
अटल अक्षर *
अग्र_field (अक्षर **पंक्ति)
{
  अक्षर *field_start = *पंक्ति;
  अक्षर *field_end   = field_start;

  अगर (delimiter)
    {
      अगर (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = म_अक्षर (field_start, '\0');
    }
  अन्यथा
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *पंक्ति = field_end;
  वापस field_start;
}

ATTRIBUTE_PURE
अटल bool
include_field (uintmax_t field)
{
  काष्ठा field_range_pair *p = frp;
  अगर (!p)
    वापस field == 1;

  जबतक (p->lo != UINTMAX_MAX)
    {
      अगर (p->lo <= field && p->hi >= field)
        वापस true;
      ++p;
    }
  वापस false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
अटल bool
process_field (अक्षर *text, uintmax_t field)
{
  दीर्घ द्विगुन val = 0;
  माप_प्रकार precision = 0;
  bool valid_संख्या = true;

  अगर (include_field (field))
    {
      valid_संख्या =
        process_suffixed_संख्या (text, &val, &precision, field);

      intmax_t padding;
      अगर (valid_संख्या)
        valid_संख्या = prepare_padded_संख्या (val, precision, &padding);

      अगर (valid_संख्या)
        print_padded_संख्या (padding);
      अन्यथा
        ख_माला_दो (text, मानक_निकास);
    }
  अन्यथा
    ख_माला_दो (text, मानक_निकास);

  वापस valid_संख्या;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
अटल पूर्णांक
process_पंक्ति (अक्षर *पंक्ति, bool newline)
{
  अक्षर *अग्र;
  uintmax_t field = 0;
  bool valid_संख्या = true;

  जबतक (true) {
    ++field;
    अग्र = अग्र_field (&पंक्ति);

    अगर (*पंक्ति != '\0')
      {
        /* NUL terminate the current field string and process */
        अक्षर end_field = *पंक्ति;
        *पंक्ति = '\0';

        अगर (! process_field (अग्र, field))
          valid_संख्या = false;

        अगर (delimiter != शून्य)
          ख_माला_दो (delimiter, मानक_निकास);
        अन्यथा
          ख_अक्षर_दो (' ', मानक_निकास);

        अगर (delimiter)
          पंक्ति += MAX (म_माप (delimiter), 1);
        अन्यथा
          {
            *पंक्ति = end_field;
            mcel_t g = mcel_scanz (पंक्ति);
            पंक्ति += g.len;
          }
      }
    अन्यथा
      {
        /* end of the line, process the last field and finish */
        अगर (! process_field (अग्र, field))
          valid_संख्या = false;

        अवरोध;
      }
  }

  अगर (newline)
    अक्षर_दो (पंक्ति_delim);

 अगर (ख_त्रुटि (मानक_निकास))
   write_error ();

  वापस valid_संख्या;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#अगर HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#पूर्ण_अगर

  decimal_point = nl_langinfo (RADIXCHAR);
  अगर (decimal_point == शून्य || म_माप (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_लंबाई = म_माप (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  अगर (thousands_sep == शून्य)
    thousands_sep = "";
  thousands_sep_लंबाई = म_माप (thousands_sep);

  निकास_पर (close_stdout);

  जबतक (true)
    {
      पूर्णांक c = getopt_long (argc, argv, "d:z", longopts, शून्य);

      अगर (c == -1)
        अवरोध;

      चयन (c)
        {
        हाल FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          अवरोध;

        हाल FROM_UNIT_OPTION:
          from_unit_आकार = unit_to_umax (optarg);
          अवरोध;

        हाल TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          अवरोध;

        हाल TO_UNIT_OPTION:
          to_unit_आकार = unit_to_umax (optarg);
          अवरोध;

        हाल ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          अवरोध;

        हाल GROUPING_OPTION:
          grouping = 1;
          अवरोध;

        हाल PADDING_OPTION:
          अगर (((xstrtoimax (optarg, शून्य, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (निकास_त्रुटि, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          अवरोध;

        हाल FIELD_OPTION:
          अगर (n_frp)
            error (निकास_त्रुटि, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          अवरोध;

        हाल 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          अगर (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              अगर (optarg[g.len] != '\0')
                error (निकास_त्रुटि, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          अवरोध;

        हाल 'z':
          पंक्ति_delim = '\0';
          अवरोध;

        हाल SUFFIX_OPTION:
          suffix = optarg;
          अवरोध;

        हाल UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          अवरोध;

        हाल DEBUG_OPTION:
          debug = true;
          अवरोध;

        हाल DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          अवरोध;

        हाल HEADER_OPTION:
          अगर (optarg)
            {
              अगर (xstrtoumax (optarg, शून्य, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (निकास_त्रुटि, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          अन्यथा
            {
              header = 1;
            }
          अवरोध;

        हाल FORMAT_OPTION:
          format_str = optarg;
          अवरोध;

        हाल INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          अवरोध;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (format_str != शून्य && grouping)
    error (निकास_त्रुटि, 0, _("--grouping cannot be combined with --format"));

  अगर (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  अगर (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == शून्य))
    error (0, 0, _("no conversion option specified"));

  अगर (debug && unit_separator && delimiter == शून्य)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  अगर (format_str)
    parse_format_string (format_str);

  अगर (grouping)
    {
      अगर (scale_to != scale_none)
        error (निकास_त्रुटि, 0, _("grouping cannot be combined with --to"));
      अगर (debug && thousands_sep_लंबाई == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == शून्य);

  अगर (inval_style != inval_abort)
    conv_exit_code = 0;

  अगर (argc > optind)
    {
      अगर (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      क्रम (; optind < argc; optind++)
        valid_numbers &= process_पंक्ति (argv[optind], true);
    }
  अन्यथा
    {
      अक्षर *पंक्ति = शून्य;
      माप_प्रकार पंक्ति_allocated = 0;
      ssize_t len;

      जबतक (header-- && getdelim (&पंक्ति, &पंक्ति_allocated,
                                   पंक्ति_delim, मानक_निवेश) > 0)
        {
          अगर (ख_माला_दो (पंक्ति, मानक_निकास) == खातापूर्ण)
            write_error ();
        }

      जबतक ((len = getdelim (&पंक्ति, &पंक्ति_allocated,
                              पंक्ति_delim, मानक_निवेश)) > 0)
        {
          bool newline = पंक्ति[len - 1] == पंक्ति_delim;
          अगर (newline)
            पंक्ति[len - 1] = '\0';
          valid_numbers &= process_पंक्ति (पंक्ति, newline);
        }

      अगर (ख_त्रुटि (मानक_निवेश))
        error (निकास_त्रुटि, त्रुटि_सं, _("error reading input"));
    }

  अगर (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  पूर्णांक exit_status = निकास_सफल;
  अगर (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
