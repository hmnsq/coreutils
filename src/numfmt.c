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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <float.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <langinfo.h>

#അന്തര്ഭാവ "argmatch.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "mbswidth.h"
#അന്തര്ഭാവ "mcel.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "skipchars.h"
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "utf8.h"
#അന്തര്ഭാവ "xstrtol.h"

#അന്തര്ഭാവ "set-fields.h"

#എങ്കിൽ HAVE_FPSETPREC
# include <ieeefp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "numfmt"

#define AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
എണ്ണൽ { EXIT_CONVERSION_WARNINGS = 2 };

എണ്ണൽ
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

എണ്ണൽ scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", ശൂന്യ
};

സ്ഥിതം എണ്ണൽ scale_type സ്ഥിരം scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം scale_to_args[] =
{
  "none", "si", "iec", "iec-i", ശൂന്യ
};

സ്ഥിതം എണ്ണൽ scale_type സ്ഥിരം scale_to_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


എണ്ണൽ round_type
{
  round_ceiling,
  round_floor,
  round_from_zero,
  round_to_zero,
  round_nearest,
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", ശൂന്യ
};

സ്ഥിതം എണ്ണൽ round_type സ്ഥിരം round_types[] =
{
  round_ceiling, round_floor, round_from_zero, round_to_zero, round_nearest
};


എണ്ണൽ inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം inval_args[] =
{
  "abort", "fail", "warn", "ignore", ശൂന്യ
};

സ്ഥിതം എണ്ണൽ inval_type സ്ഥിരം inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"from", required_argument, ശൂന്യ, FROM_OPTION},
  {"from-unit", required_argument, ശൂന്യ, FROM_UNIT_OPTION},
  {"to", required_argument, ശൂന്യ, TO_OPTION},
  {"to-unit", required_argument, ശൂന്യ, TO_UNIT_OPTION},
  {"round", required_argument, ശൂന്യ, ROUND_OPTION},
  {"padding", required_argument, ശൂന്യ, PADDING_OPTION},
  {"suffix", required_argument, ശൂന്യ, SUFFIX_OPTION},
  {"unit-separator", required_argument, ശൂന്യ, UNIT_SEPARATOR_OPTION},
  {"grouping", no_argument, ശൂന്യ, GROUPING_OPTION},
  {"delimiter", required_argument, ശൂന്യ, 'd'},
  {"field", required_argument, ശൂന്യ, FIELD_OPTION},
  {"debug", no_argument, ശൂന്യ, DEBUG_OPTION},
  {"-debug", no_argument, ശൂന്യ, DEV_DEBUG_OPTION},
  {"header", optional_argument, ശൂന്യ, HEADER_OPTION},
  {"format", required_argument, ശൂന്യ, FORMAT_OPTION},
  {"invalid", required_argument, ശൂന്യ, INVALID_OPTION},
  {"zero-terminated", no_argument, ശൂന്യ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
എണ്ണൽ { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
എണ്ണൽ { MAX_ACCEPTABLE_DIGITS = 33 };

സ്ഥിതം എണ്ണൽ scale_type scale_from = scale_none;
സ്ഥിതം എണ്ണൽ scale_type scale_to = scale_none;
സ്ഥിതം എണ്ണൽ round_type round_style = round_from_zero;
സ്ഥിതം എണ്ണൽ inval_type inval_style = inval_abort;
സ്ഥിതം അക്ഷരം സ്ഥിരം *suffix = ശൂന്യ;
സ്ഥിതം അക്ഷരം സ്ഥിരം *unit_separator = ശൂന്യ;
സ്ഥിതം uintmax_t from_unit_size = 1;
സ്ഥിതം uintmax_t to_unit_size = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ grouping = 0;
സ്ഥിതം അക്ഷരം *padding_buffer = ശൂന്യ;
സ്ഥിതം idx_t padding_buffer_size = 0;
സ്ഥിതം intmax_t padding_width = 0;
സ്ഥിതം പൂർണ്ണസംഖ്യ zero_padding_width = 0;
സ്ഥിതം long പൂർണ്ണസംഖ്യ user_precision = -1;
സ്ഥിതം അക്ഷരം സ്ഥിരം *format_str = ശൂന്യ;
സ്ഥിതം അക്ഷരം *format_str_prefix = ശൂന്യ;
സ്ഥിതം അക്ഷരം *format_str_suffix = ശൂന്യ;

/* By default, any conversion error will terminate the program.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *delimiter = ശൂന്യ;

/* line delimiter.  */
സ്ഥിതം unsigned അക്ഷരം വരി_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
സ്ഥിതം uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
സ്ഥിതം bool debug;

/* will be set according to the current locale.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *decimal_point;
സ്ഥിതം പൂർണ്ണസംഖ്യ decimal_point_length;
സ്ഥിതം അക്ഷരം സ്ഥിരം *thousands_sep;
സ്ഥിതം പൂർണ്ണസംഖ്യ thousands_sep_length;

/* debugging for developers.  Enables devmsg().  */
സ്ഥിതം bool dev_debug = false;

സ്ഥിതം bool
newline_or_blank (mcel_t g)
{
  തിരികെ g.ch == '\n' || c32issep (g.ch);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
default_scale_base (എണ്ണൽ scale_type scale)
{
  തിരഞ്ഞെടുപ്പ് (scale)
    {
    അവസ്ഥ scale_IEC:
    അവസ്ഥ scale_IEC_I:
      തിരികെ 1024;

    അവസ്ഥ scale_none:
    അവസ്ഥ scale_auto:
    അവസ്ഥ scale_SI:
    സ്ഥിരസ്ഥിതി:
      തിരികെ 1000;
    }
}

സ്ഥിതം അക്ഷരം സ്ഥിരം zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
സ്ഥിതം അക്ഷരം സ്ഥിരം *valid_suffixes = 1 + zero_and_valid_suffixes;

സ്ഥിതം വരിയിൽ bool
valid_suffix (സ്ഥിരം അക്ഷരം suf)
{
  തിരികെ strchr (valid_suffixes, suf) != ശൂന്യ;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
suffix_power (സ്ഥിരം അക്ഷരം suf)
{
  തിരഞ്ഞെടുപ്പ് (suf)
    {
    അവസ്ഥ 'k':                  /* kilo.  */
    അവസ്ഥ 'K':                  /* kilo or kibi.  */
      തിരികെ 1;

    അവസ്ഥ 'M':                  /* mega or mebi.  */
      തിരികെ 2;

    അവസ്ഥ 'G':                  /* giga or gibi.  */
      തിരികെ 3;

    അവസ്ഥ 'T':                  /* tera or tebi.  */
      തിരികെ 4;

    അവസ്ഥ 'P':                  /* peta or pebi.  */
      തിരികെ 5;

    അവസ്ഥ 'E':                  /* exa or exbi.  */
      തിരികെ 6;

    അവസ്ഥ 'Z':                  /* zetta or 2**70.  */
      തിരികെ 7;

    അവസ്ഥ 'Y':                  /* yotta or 2**80.  */
      തിരികെ 8;

    അവസ്ഥ 'R':                  /* ronna or 2**90.  */
      തിരികെ 9;

    അവസ്ഥ 'Q':                  /* quetta or 2**100.  */
      തിരികെ 10;

    സ്ഥിരസ്ഥിതി:                   /* should never happen. assert?  */
      തിരികെ 0;
    }
}

സ്ഥിതം വരിയിൽ അക്ഷരം സ്ഥിരം *
suffix_power_char (പൂർണ്ണസംഖ്യ power)
{
  തിരഞ്ഞെടുപ്പ് (power)
    {
    അവസ്ഥ 0:
      തിരികെ "";

    അവസ്ഥ 1:
      തിരികെ "K";

    അവസ്ഥ 2:
      തിരികെ "M";

    അവസ്ഥ 3:
      തിരികെ "G";

    അവസ്ഥ 4:
      തിരികെ "T";

    അവസ്ഥ 5:
      തിരികെ "P";

    അവസ്ഥ 6:
      തിരികെ "E";

    അവസ്ഥ 7:
      തിരികെ "Z";

    അവസ്ഥ 8:
      തിരികെ "Y";

    അവസ്ഥ 9:
      തിരികെ "R";

    അവസ്ഥ 10:
      തിരികെ "Q";

    സ്ഥിരസ്ഥിതി:
      തിരികെ "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
സ്ഥിതം long ഇരട്ട
powerld (long ഇരട്ട base, ptrdiff_t x)
{
  long ഇരട്ട result = base;
  എങ്കിൽ (x == 0)
    തിരികെ 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  ആകുമ്പോൾ (--x)
    result *= base;
  തിരികെ result;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
സ്ഥിതം വരിയിൽ long ഇരട്ട
absld (long ഇരട്ട val)
{
  തിരികെ val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
സ്ഥിതം long ഇരട്ട
expld (long ഇരട്ട val, പൂർണ്ണസംഖ്യ base, പൂർണ്ണസംഖ്യ /*output */ *x)
{
  പൂർണ്ണസംഖ്യ power = 0;

  എങ്കിൽ (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      ആകുമ്പോൾ (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  എങ്കിൽ (x)
    *x = power;
  തിരികെ val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
സ്ഥിതം വരിയിൽ intmax_t
simple_round_ceiling (long ഇരട്ട val)
{
  intmax_t intval = val;
  എങ്കിൽ (intval < val)
    intval++;
  തിരികെ intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
സ്ഥിതം വരിയിൽ intmax_t
simple_round_floor (long ഇരട്ട val)
{
  തിരികെ -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
സ്ഥിതം വരിയിൽ intmax_t
simple_round_from_zero (long ഇരട്ട val)
{
  തിരികെ val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
സ്ഥിതം വരിയിൽ intmax_t
simple_round_to_zero (long ഇരട്ട val)
{
  തിരികെ val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
സ്ഥിതം വരിയിൽ intmax_t
simple_round_nearest (long ഇരട്ട val)
{
  തിരികെ val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
സ്ഥിതം വരിയിൽ long ഇരട്ട
simple_round (long ഇരട്ട val, എണ്ണൽ round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (long ഇരട്ട) INTMAX_MAX * intmax_mul;

  തിരഞ്ഞെടുപ്പ് (t)
    {
    അവസ്ഥ round_ceiling:
      rval = simple_round_ceiling (val);
      നിർത്തുക;

    അവസ്ഥ round_floor:
      rval = simple_round_floor (val);
      നിർത്തുക;

    അവസ്ഥ round_from_zero:
      rval = simple_round_from_zero (val);
      നിർത്തുക;

    അവസ്ഥ round_to_zero:
      rval = simple_round_to_zero (val);
      നിർത്തുക;

    അവസ്ഥ round_nearest:
      rval = simple_round_nearest (val);
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      /* to silence the compiler - this should never happen.  */
      തിരികെ 0;
    }

  തിരികെ (long ഇരട്ട) INTMAX_MAX * intmax_mul + rval;
}

എണ്ണൽ simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_OVERFLOW,
  SSE_INVALID_സംഖ്യ,

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
സ്ഥിതം എണ്ണൽ simple_strtod_error
simple_strtod_int (അക്ഷരം സ്ഥിരം *നിവേശം_str,
                   അക്ഷരം **endptr, long ഇരട്ട *മൂല്യം, bool *negative)
{
  എണ്ണൽ simple_strtod_error e = SSE_OK;

  long ഇരട്ട val = 0;
  പൂർണ്ണസംഖ്യ digits = 0;
  bool found_digit = false;

  എങ്കിൽ (*നിവേശം_str == '-')
    {
      നിവേശം_str++;
      *negative = true;
    }
  അല്ലെങ്കിൽ
    *negative = false;

  *endptr = (അക്ഷരം *) നിവേശം_str;
  ആകുമ്പോൾ (c_isdigit (**endptr))
    {
      പൂർണ്ണസംഖ്യ digit = (**endptr) - '0';

      found_digit = true;

      എങ്കിൽ (val || digit)
        digits++;

      എങ്കിൽ (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      എങ്കിൽ (digits > MAX_ACCEPTABLE_DIGITS)
        തിരികെ SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      എങ്കിൽ (thousands_sep_length > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_length)
          && c_isdigit ((*endptr)[thousands_sep_length]))
        (*endptr) += thousands_sep_length;
    }
  എങ്കിൽ (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    തിരികെ SSE_INVALID_സംഖ്യ;
  എങ്കിൽ (*negative)
    val = -val;

  എങ്കിൽ (മൂല്യം)
    *മൂല്യം = val;

  തിരികെ e;
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
സ്ഥിതം എണ്ണൽ simple_strtod_error
simple_strtod_float (അക്ഷരം സ്ഥിരം *നിവേശം_str,
                     അക്ഷരം **endptr,
                     long ഇരട്ട *മൂല്യം,
                     size_t *precision)
{
  bool negative;
  എണ്ണൽ simple_strtod_error e = SSE_OK;

  എങ്കിൽ (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (നിവേശം_str, endptr, മൂല്യം, &negative);
  എങ്കിൽ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    തിരികെ e;

  /* optional decimal point + fraction.  */
  എങ്കിൽ (STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    {
      അക്ഷരം *ptr2;
      long ഇരട്ട val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_length;
      എണ്ണൽ simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      എങ്കിൽ (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        തിരികെ e2;
      എങ്കിൽ (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      എങ്കിൽ (neg_frac)
        തിരികെ SSE_INVALID_സംഖ്യ;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      എങ്കിൽ (മൂല്യം)
        {
          എങ്കിൽ (negative)
            *മൂല്യം -= val_frac;
          അല്ലെങ്കിൽ
            *മൂല്യം += val_frac;
        }

      എങ്കിൽ (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  തിരികെ e;
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
സ്ഥിതം എണ്ണൽ simple_strtod_error
simple_strtod_human (അക്ഷരം സ്ഥിരം *നിവേശം_str,
                     അക്ഷരം **endptr, long ഇരട്ട *മൂല്യം, size_t *precision,
                     എണ്ണൽ scale_type allowed_scaling)
{
  പൂർണ്ണസംഖ്യ power = 0;
  /* 'scale_auto' is checked below.  */
  പൂർണ്ണസംഖ്യ scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, നിവേശം_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  എണ്ണൽ simple_strtod_error e =
    simple_strtod_float (നിവേശം_str, endptr, മൂല്യം, precision);
  എങ്കിൽ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    തിരികെ e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *മൂല്യം, (പൂർണ്ണസംഖ്യ)*precision);

  ആകുമ്പോൾ (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      എങ്കിൽ (unit_separator)
        {
          size_t sep_len = strlen (unit_separator);
          എങ്കിൽ (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      എങ്കിൽ (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          എങ്കിൽ (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      എങ്കിൽ (**endptr == '\0')
        നിർത്തുക;  /* Treat as no suffix.  */

      എങ്കിൽ (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          എങ്കിൽ (**endptr == '\0')
            നിർത്തുക;

          തിരികെ SSE_INVALID_SUFFIX;
        }

      എങ്കിൽ (allowed_scaling == scale_none)
        തിരികെ SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      എങ്കിൽ (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (allowed_scaling == scale_IEC_I)
        {
          എങ്കിൽ (**endptr == 'i')
            (*endptr)++;
          അല്ലെങ്കിൽ
            തിരികെ SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      നിർത്തുക;
    }

  long ഇരട്ട multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*മൂല്യം) = (*മൂല്യം) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *മൂല്യം, *മൂല്യം);

  തിരികെ e;
}


സ്ഥിതം ശൂന്യം
simple_strtod_fatal (എണ്ണൽ simple_strtod_error err, അക്ഷരം സ്ഥിരം *നിവേശം_str)
{
  അക്ഷരം സ്ഥിരം *msgid = ശൂന്യ;

  തിരഞ്ഞെടുപ്പ് (err)
    {
    അവസ്ഥ SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      നിർത്തുക;

    അവസ്ഥ SSE_INVALID_സംഖ്യ:
      msgid = N_("invalid number: %s");
      നിർത്തുക;

    അവസ്ഥ SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      നിർത്തുക;

    അവസ്ഥ SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      നിർത്തുക;

    അവസ്ഥ SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      നിർത്തുക;

    അവസ്ഥ SSE_OK_PRECISION_LOSS:
    അവസ്ഥ SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  എങ്കിൽ (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (നിവേശം_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
double_to_human (long ഇരട്ട val, പൂർണ്ണസംഖ്യ precision,
                 അക്ഷരം *buf, idx_t buf_size,
                 എണ്ണൽ scale_type scale, പൂർണ്ണസംഖ്യ group, എണ്ണൽ round_type round)
{
  അക്ഷരം fmt[വലുപ്പം "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  അക്ഷരം *pfmt = fmt;
  *pfmt++ = '%';

  എങ്കിൽ (group)
    *pfmt++ = '\'';

  എങ്കിൽ (zero_padding_width)
    pfmt += sprintf (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  എങ്കിൽ (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      തിരികെ snprintf (buf, buf_size, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  ഇരട്ട scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  പൂർണ്ണസംഖ്യ power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  പൂർണ്ണസംഖ്യ power_adjust = 0;
  എങ്കിൽ (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  അല്ലെങ്കിൽ എങ്കിൽ (absld (val) < 10)
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
  എങ്കിൽ (absld (val) >= scale_base)
    {
      val /= scale_base;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  പൂർണ്ണസംഖ്യ show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  പൂർണ്ണസംഖ്യ prec = user_precision == -1 ? show_decimal_point : user_precision;

  തിരികെ snprintf (buf, buf_size, fmt, prec, val,
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
സ്ഥിതം uintmax_t
unit_to_umax (അക്ഷരം സ്ഥിരം *n_string)
{
  strtol_error s_err;
  അക്ഷരം സ്ഥിരം *c_string = n_string;
  അക്ഷരം *t_string = ശൂന്യ;
  size_t n_len = strlen (n_string);
  അക്ഷരം *end = ശൂന്യ;
  uintmax_t n;
  അക്ഷരം സ്ഥിരം *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  എങ്കിൽ (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      memcpy (t_string, n_string, n_len);

      എങ്കിൽ (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
        *end = '\0';
      അല്ലെങ്കിൽ
        {
          *++end = 'B';
          *++end = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &end, 10, &n, suffixes);

  എങ്കിൽ (s_err != LONGINT_OK || *end || n == 0)
    {
      സ്വതന്ത്രം (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  സ്വതന്ത്രം (t_string);

  തിരികെ n;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [സംഖ്യ_2]...\n\
"), program_name);
      fputs (_("\
Reformat സംഖ്യ_2(s), or the numbers from standard നിവേശം എങ്കിൽ none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid നിവേശം\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace വേണ്ടി field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these നിവേശം fields (സ്ഥിരസ്ഥിതി=1);\n\
         see FIELDS below വേണ്ടി details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use printf style floating-point FORMAT;\n\
         see FORMAT below വേണ്ടി details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         auto-scale നിവേശം numbers to UNITs; സ്ഥിരസ്ഥിതി is 'none';\n\
         see UNIT below വേണ്ടി details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the നിവേശം unit size (instead of the സ്ഥിരസ്ഥിതി 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header വരികൾ;\n\
         N defaults to 1 എങ്കിൽ not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode വേണ്ടി invalid numbers;\n\
         MODE can be: abort (സ്ഥിരസ്ഥിതി), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will വലത്-align, negative N will ഇടത്-align;\n\
         padding is ignored എങ്കിൽ the output is wider than N;\n\
         the സ്ഥിരസ്ഥിതി is to automatically pad എങ്കിൽ a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD വേണ്ടി rounding when scaling; METHOD can be:\n\
         up, down, from-zero (സ്ഥിരസ്ഥിതി), towards-zero, nearest\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         add SUFFIX to output numbers,\n\
         and accept an optional SUFFIX in നിവേശം numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         തിരുകുക SEP between സംഖ്യ and unit on output,\n\
         and accept an optional SEP in നിവേശം numbers\n\
"));
      oputs (_("\
      --to=UNIT\n\
         auto-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit size (instead of the സ്ഥിരസ്ഥിതി 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         വരി delimiter is NUL, not newline\n\
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
  N-   from N'th field, to end of വരി\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable വേണ്ടി printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (എങ്കിൽ supported by current locale).\n\
Optional width മൂല്യം (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the സംഖ്യ. Optional negative മൂല്യങ്ങൾ (%-10f) will ഇടത് align.\n\
Optional precision (%.1f) will override the നിവേശം determined precision.\n\
"), stdout);

      printf (_("\n\
Exit status is 0 എങ്കിൽ all നിവേശം numbers were successfully converted.\n\
By സ്ഥിരസ്ഥിതി, %s will stop at the first conversion error with exit status 2.\n\
With --invalid='fail' a warning is printed വേണ്ടി each conversion error\n\
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
സ്ഥിതം ശൂന്യം
parse_format_string (അക്ഷരം സ്ഥിരം *fmt)
{
  size_t i;
  size_t prefix_len = 0;
  size_t suffix_pos;
  അക്ഷരം *endptr = ശൂന്യ;
  bool zero_padding = false;

  വേണ്ടി (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      എങ്കിൽ (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  ആകുമ്പോൾ (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      എങ്കിൽ (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (! skip)
        നിർത്തുക;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  എങ്കിൽ (pad != 0)
    {
      എങ്കിൽ (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      എങ്കിൽ (pad < 0)
        padding_width = pad;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (zero_padding)
            zero_padding_width = MIN (pad, INT_MAX);
          അല്ലെങ്കിൽ
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  എങ്കിൽ (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  എങ്കിൽ (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      എങ്കിൽ (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  എങ്കിൽ (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  വേണ്ടി (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    എങ്കിൽ (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  എങ്കിൽ (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  എങ്കിൽ (fmt[suffix_pos] != '\0')
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
സ്ഥിതം എണ്ണൽ simple_strtod_error
parse_human_സംഖ്യ (അക്ഷരം സ്ഥിരം *str, long ഇരട്ട /*output */ *മൂല്യം,
                    size_t *precision)
{
  അക്ഷരം *ptr = ശൂന്യ;

  എണ്ണൽ simple_strtod_error e =
    simple_strtod_human (str, &ptr, മൂല്യം, precision, scale_from);
  എങ്കിൽ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      തിരികെ e;
    }

  എങ്കിൽ (ptr && *ptr != '\0')
    {
      എങ്കിൽ (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  തിരികെ e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
സ്ഥിതം bool
prepare_padded_സംഖ്യ (സ്ഥിരം long ഇരട്ട val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  പൂർണ്ണസംഖ്യ x;
  expld (val, 10, &x);

  എങ്കിൽ (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      എങ്കിൽ (inval_style != inval_ignore)
        {
          എങ്കിൽ (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          അല്ലെങ്കിൽ
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      തിരികെ false;
    }

  എങ്കിൽ (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      എങ്കിൽ (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      തിരികെ false;
    }

  ആകുമ്പോൾ (true)
    {
      പൂർണ്ണസംഖ്യ numlen = double_to_human (val, precision_used,
                                    padding_buffer, padding_buffer_size,
                                    scale_to, grouping, round_style);
      ptrdiff_t growth;
      എങ്കിൽ (numlen < 0 || ckd_sub (&growth, numlen, padding_buffer_size - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      എങ്കിൽ (growth <= 0)
        നിർത്തുക;
      padding_buffer = xpalloc (padding_buffer, &padding_buffer_size,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_buffer));

  intmax_t pad = 0;
  എങ്കിൽ (padding_width)
    {
      പൂർണ്ണസംഖ്യ buf_width = mbswidth (padding_buffer,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      എങ്കിൽ (0 <= buf_width)
        {
          എങ്കിൽ (padding_width < 0)
            {
              എങ്കിൽ (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          അല്ലെങ്കിൽ
            {
              എങ്കിൽ (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  തിരികെ true;
}

സ്ഥിതം ശൂന്യം
print_padded_സംഖ്യ (intmax_t padding)
{
  എങ്കിൽ (format_str_prefix)
    fputs (format_str_prefix, stdout);

  വേണ്ടി (intmax_t p = padding; 0 < p; p--)
    putchar (' ');

  fputs (padding_buffer, stdout);

  വേണ്ടി (intmax_t p = padding; p < 0; p++)
    putchar (' ');

  എങ്കിൽ (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
സ്ഥിതം bool
process_suffixed_സംഖ്യ (അക്ഷരം *text, long ഇരട്ട *result,
                         size_t *precision, long പൂർണ്ണസംഖ്യ field)
{
  അക്ഷരം saved_suffix = '\0';

  എങ്കിൽ (suffix)
    {
      എങ്കിൽ (mbs_endswith (text, suffix))
        {
          saved_suffix = *(text + strlen (text) - strlen (suffix));
          *(text + strlen (text) - strlen (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      അല്ലെങ്കിൽ
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  അക്ഷരം *p = skip_str_matching (text, newline_or_blank, true);

  /* setup auto-padding.  */
  എങ്കിൽ (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      എങ്കിൽ (padding_width < 0)
        padding_width = strlen (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  long ഇരട്ട val = 0;
  എണ്ണൽ simple_strtod_error e = parse_human_സംഖ്യ (p, &val, precision);
  എങ്കിൽ (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  എങ്കിൽ (from_unit_size != 1 || to_unit_size != 1)
    val = (val * from_unit_size) / to_unit_size;

  *result = val;

  എങ്കിൽ (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    തിരികെ true;
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (saved_suffix)
        *(text + strlen (text)) = saved_suffix;
      തിരികെ false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
സ്ഥിതം അക്ഷരം *
mbsmbchr (അക്ഷരം സ്ഥിരം *s, അക്ഷരം സ്ഥിരം *c)
{
  unsigned അക്ഷരം uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  എങ്കിൽ (uc < 0x30 || MB_CUR_MAX == 1)
    തിരികെ (അക്ഷരം *) strchr (s, uc);
  അല്ലെങ്കിൽ എങ്കിൽ (is_utf8_charset ())
    തിരികെ (അക്ഷരം *) (uc < 0x80 ? strchr (s, uc) : strstr (s, c));
  അല്ലെങ്കിൽ
    തിരികെ *(c + 1) == '\0' ? mbschr (s, uc) : (അക്ഷരം *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
സ്ഥിതം അക്ഷരം *
next_field (അക്ഷരം **വരി)
{
  അക്ഷരം *field_start = *വരി;
  അക്ഷരം *field_end   = field_start;

  എങ്കിൽ (delimiter)
    {
      എങ്കിൽ (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = strchr (field_start, '\0');
    }
  അല്ലെങ്കിൽ
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *വരി = field_end;
  തിരികെ field_start;
}

ATTRIBUTE_PURE
സ്ഥിതം bool
include_field (uintmax_t field)
{
  ഘടന field_range_pair *p = frp;
  എങ്കിൽ (!p)
    തിരികെ field == 1;

  ആകുമ്പോൾ (p->lo != UINTMAX_MAX)
    {
      എങ്കിൽ (p->lo <= field && p->hi >= field)
        തിരികെ true;
      ++p;
    }
  തിരികെ false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
സ്ഥിതം bool
process_field (അക്ഷരം *text, uintmax_t field)
{
  long ഇരട്ട val = 0;
  size_t precision = 0;
  bool valid_സംഖ്യ = true;

  എങ്കിൽ (include_field (field))
    {
      valid_സംഖ്യ =
        process_suffixed_സംഖ്യ (text, &val, &precision, field);

      intmax_t padding;
      എങ്കിൽ (valid_സംഖ്യ)
        valid_സംഖ്യ = prepare_padded_സംഖ്യ (val, precision, &padding);

      എങ്കിൽ (valid_സംഖ്യ)
        print_padded_സംഖ്യ (padding);
      അല്ലെങ്കിൽ
        fputs (text, stdout);
    }
  അല്ലെങ്കിൽ
    fputs (text, stdout);

  തിരികെ valid_സംഖ്യ;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
process_വരി (അക്ഷരം *വരി, bool newline)
{
  അക്ഷരം *next;
  uintmax_t field = 0;
  bool valid_സംഖ്യ = true;

  ആകുമ്പോൾ (true) {
    ++field;
    next = next_field (&വരി);

    എങ്കിൽ (*വരി != '\0')
      {
        /* NUL terminate the current field string and process */
        അക്ഷരം end_field = *വരി;
        *വരി = '\0';

        എങ്കിൽ (! process_field (next, field))
          valid_സംഖ്യ = false;

        എങ്കിൽ (delimiter != ശൂന്യ)
          fputs (delimiter, stdout);
        അല്ലെങ്കിൽ
          fputc (' ', stdout);

        എങ്കിൽ (delimiter)
          വരി += MAX (strlen (delimiter), 1);
        അല്ലെങ്കിൽ
          {
            *വരി = end_field;
            mcel_t g = mcel_scanz (വരി);
            വരി += g.len;
          }
      }
    അല്ലെങ്കിൽ
      {
        /* end of the line, process the last field and finish */
        എങ്കിൽ (! process_field (next, field))
          valid_സംഖ്യ = false;

        നിർത്തുക;
      }
  }

  എങ്കിൽ (newline)
    putchar (വരി_delim);

 എങ്കിൽ (ferror (stdout))
   write_error ();

  തിരികെ valid_സംഖ്യ;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#എങ്കിൽ HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#endif

  decimal_point = nl_langinfo (RADIXCHAR);
  എങ്കിൽ (decimal_point == ശൂന്യ || strlen (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_length = strlen (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  എങ്കിൽ (thousands_sep == ശൂന്യ)
    thousands_sep = "";
  thousands_sep_length = strlen (thousands_sep);

  atexit (close_stdout);

  ആകുമ്പോൾ (true)
    {
      പൂർണ്ണസംഖ്യ c = getopt_long (argc, argv, "d:z", longopts, ശൂന്യ);

      എങ്കിൽ (c == -1)
        നിർത്തുക;

      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          നിർത്തുക;

        അവസ്ഥ FROM_UNIT_OPTION:
          from_unit_size = unit_to_umax (optarg);
          നിർത്തുക;

        അവസ്ഥ TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          നിർത്തുക;

        അവസ്ഥ TO_UNIT_OPTION:
          to_unit_size = unit_to_umax (optarg);
          നിർത്തുക;

        അവസ്ഥ ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          നിർത്തുക;

        അവസ്ഥ GROUPING_OPTION:
          grouping = 1;
          നിർത്തുക;

        അവസ്ഥ PADDING_OPTION:
          എങ്കിൽ (((xstrtoimax (optarg, ശൂന്യ, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          നിർത്തുക;

        അവസ്ഥ FIELD_OPTION:
          എങ്കിൽ (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          നിർത്തുക;

        അവസ്ഥ 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          എങ്കിൽ (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              എങ്കിൽ (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          നിർത്തുക;

        അവസ്ഥ 'z':
          വരി_delim = '\0';
          നിർത്തുക;

        അവസ്ഥ SUFFIX_OPTION:
          suffix = optarg;
          നിർത്തുക;

        അവസ്ഥ UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          നിർത്തുക;

        അവസ്ഥ DEBUG_OPTION:
          debug = true;
          നിർത്തുക;

        അവസ്ഥ DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          നിർത്തുക;

        അവസ്ഥ HEADER_OPTION:
          എങ്കിൽ (optarg)
            {
              എങ്കിൽ (xstrtoumax (optarg, ശൂന്യ, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          അല്ലെങ്കിൽ
            {
              header = 1;
            }
          നിർത്തുക;

        അവസ്ഥ FORMAT_OPTION:
          format_str = optarg;
          നിർത്തുക;

        അവസ്ഥ INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          നിർത്തുക;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (format_str != ശൂന്യ && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  എങ്കിൽ (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  എങ്കിൽ (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == ശൂന്യ))
    error (0, 0, _("no conversion option specified"));

  എങ്കിൽ (debug && unit_separator && delimiter == ശൂന്യ)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  എങ്കിൽ (format_str)
    parse_format_string (format_str);

  എങ്കിൽ (grouping)
    {
      എങ്കിൽ (scale_to != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      എങ്കിൽ (debug && thousands_sep_length == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == ശൂന്യ);

  എങ്കിൽ (inval_style != inval_abort)
    conv_exit_code = 0;

  എങ്കിൽ (argc > optind)
    {
      എങ്കിൽ (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      വേണ്ടി (; optind < argc; optind++)
        valid_numbers &= process_വരി (argv[optind], true);
    }
  അല്ലെങ്കിൽ
    {
      അക്ഷരം *വരി = ശൂന്യ;
      size_t വരി_allocated = 0;
      ssize_t len;

      ആകുമ്പോൾ (header-- && getdelim (&വരി, &വരി_allocated,
                                   വരി_delim, stdin) > 0)
        {
          എങ്കിൽ (fputs (വരി, stdout) == EOF)
            write_error ();
        }

      ആകുമ്പോൾ ((len = getdelim (&വരി, &വരി_allocated,
                              വരി_delim, stdin)) > 0)
        {
          bool newline = വരി[len - 1] == വരി_delim;
          എങ്കിൽ (newline)
            വരി[len - 1] = '\0';
          valid_numbers &= process_വരി (വരി, newline);
        }

      എങ്കിൽ (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  എങ്കിൽ (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  പൂർണ്ണസംഖ്യ exit_status = EXIT_SUCCESS;
  എങ്കിൽ (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
