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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <langinfo.h>

#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "mbswidth.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "skipchars.h"
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

#ಅನ್ತರ್ಭಾವ "set-fields.h"

#ಯದಿ HAVE_FPSETPREC
# include <ieeefp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "numfmt"

#define AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
ಗಣನಾ { EXIT_CONVERSION_WARNINGS = 2 };

ಗಣನಾ
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

ಗಣನಾ scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", ರಿಕ್ತ
};

ಸ್ಥಿರ ಗಣನಾ scale_type ಧ್ರುವ scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ scale_to_args[] =
{
  "none", "si", "iec", "iec-i", ರಿಕ್ತ
};

ಸ್ಥಿರ ಗಣನಾ scale_type ಧ್ರುವ scale_to_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


ಗಣನಾ round_type
{
  round_ceiling,
  round_floor,
  round_from_zero,
  round_to_zero,
  round_nearest,
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", ರಿಕ್ತ
};

ಸ್ಥಿರ ಗಣನಾ round_type ಧ್ರುವ round_types[] =
{
  round_ceiling, round_floor, round_from_zero, round_to_zero, round_nearest
};


ಗಣನಾ inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ inval_args[] =
{
  "abort", "fail", "warn", "ignore", ರಿಕ್ತ
};

ಸ್ಥಿರ ಗಣನಾ inval_type ಧ್ರುವ inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"from", required_argument, ರಿಕ್ತ, FROM_OPTION},
  {"from-unit", required_argument, ರಿಕ್ತ, FROM_UNIT_OPTION},
  {"to", required_argument, ರಿಕ್ತ, TO_OPTION},
  {"to-unit", required_argument, ರಿಕ್ತ, TO_UNIT_OPTION},
  {"round", required_argument, ರಿಕ್ತ, ROUND_OPTION},
  {"padding", required_argument, ರಿಕ್ತ, PADDING_OPTION},
  {"suffix", required_argument, ರಿಕ್ತ, SUFFIX_OPTION},
  {"unit-separator", required_argument, ರಿಕ್ತ, UNIT_SEPARATOR_OPTION},
  {"grouping", no_argument, ರಿಕ್ತ, GROUPING_OPTION},
  {"delimiter", required_argument, ರಿಕ್ತ, 'd'},
  {"field", required_argument, ರಿಕ್ತ, FIELD_OPTION},
  {"debug", no_argument, ರಿಕ್ತ, DEBUG_OPTION},
  {"-debug", no_argument, ರಿಕ್ತ, DEV_DEBUG_OPTION},
  {"header", optional_argument, ರಿಕ್ತ, HEADER_OPTION},
  {"format", required_argument, ರಿಕ್ತ, FORMAT_OPTION},
  {"invalid", required_argument, ರಿಕ್ತ, INVALID_OPTION},
  {"zero-terminated", no_argument, ರಿಕ್ತ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
ಗಣನಾ { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
ಗಣನಾ { MAX_ACCEPTABLE_DIGITS = 33 };

ಸ್ಥಿರ ಗಣನಾ scale_type scale_from = scale_none;
ಸ್ಥಿರ ಗಣನಾ scale_type scale_to = scale_none;
ಸ್ಥಿರ ಗಣನಾ round_type round_style = round_from_zero;
ಸ್ಥಿರ ಗಣನಾ inval_type inval_style = inval_abort;
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *suffix = ರಿಕ್ತ;
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *unit_separator = ರಿಕ್ತ;
ಸ್ಥಿರ uintmax_t from_unit_size = 1;
ಸ್ಥಿರ uintmax_t to_unit_size = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grouping = 0;
ಸ್ಥಿರ ವರ್ಣ *padding_buffer = ರಿಕ್ತ;
ಸ್ಥಿರ idx_t padding_buffer_size = 0;
ಸ್ಥಿರ intmax_t padding_width = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ zero_padding_width = 0;
ಸ್ಥಿರ long ಪೂರ್ಣಾಙ್ಕ user_precision = -1;
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *format_str = ರಿಕ್ತ;
ಸ್ಥಿರ ವರ್ಣ *format_str_prefix = ರಿಕ್ತ;
ಸ್ಥಿರ ವರ್ಣ *format_str_suffix = ರಿಕ್ತ;

/* By default, any conversion error will terminate the program.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *delimiter = ರಿಕ್ತ;

/* line delimiter.  */
ಸ್ಥಿರ unsigned ವರ್ಣ ಸಾಲು_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
ಸ್ಥಿರ uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
ಸ್ಥಿರ bool debug;

/* will be set according to the current locale.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *decimal_point;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ decimal_point_length;
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *thousands_sep;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ thousands_sep_length;

/* debugging for developers.  Enables devmsg().  */
ಸ್ಥಿರ bool dev_debug = false;

ಸ್ಥಿರ bool
newline_or_blank (mcel_t g)
{
  ಪ್ರತ್ಯಾಗಮ g.ch == '\n' || c32issep (g.ch);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
default_scale_base (ಗಣನಾ scale_type scale)
{
  ವಿಕಲ್ಪ (scale)
    {
    ಪಕ್ಷ scale_IEC:
    ಪಕ್ಷ scale_IEC_I:
      ಪ್ರತ್ಯಾಗಮ 1024;

    ಪಕ್ಷ scale_none:
    ಪಕ್ಷ scale_auto:
    ಪಕ್ಷ scale_SI:
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ 1000;
    }
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *valid_suffixes = 1 + zero_and_valid_suffixes;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
valid_suffix (ಧ್ರುವ ವರ್ಣ suf)
{
  ಪ್ರತ್ಯಾಗಮ strchr (valid_suffixes, suf) != ರಿಕ್ತ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
suffix_power (ಧ್ರುವ ವರ್ಣ suf)
{
  ವಿಕಲ್ಪ (suf)
    {
    ಪಕ್ಷ 'k':                  /* kilo.  */
    ಪಕ್ಷ 'K':                  /* kilo or kibi.  */
      ಪ್ರತ್ಯಾಗಮ 1;

    ಪಕ್ಷ 'M':                  /* mega or mebi.  */
      ಪ್ರತ್ಯಾಗಮ 2;

    ಪಕ್ಷ 'G':                  /* giga or gibi.  */
      ಪ್ರತ್ಯಾಗಮ 3;

    ಪಕ್ಷ 'T':                  /* tera or tebi.  */
      ಪ್ರತ್ಯಾಗಮ 4;

    ಪಕ್ಷ 'P':                  /* peta or pebi.  */
      ಪ್ರತ್ಯಾಗಮ 5;

    ಪಕ್ಷ 'E':                  /* exa or exbi.  */
      ಪ್ರತ್ಯಾಗಮ 6;

    ಪಕ್ಷ 'Z':                  /* zetta or 2**70.  */
      ಪ್ರತ್ಯಾಗಮ 7;

    ಪಕ್ಷ 'Y':                  /* yotta or 2**80.  */
      ಪ್ರತ್ಯಾಗಮ 8;

    ಪಕ್ಷ 'R':                  /* ronna or 2**90.  */
      ಪ್ರತ್ಯಾಗಮ 9;

    ಪಕ್ಷ 'Q':                  /* quetta or 2**100.  */
      ಪ್ರತ್ಯಾಗಮ 10;

    ಸ್ವಭಾವ:                   /* should never happen. assert?  */
      ಪ್ರತ್ಯಾಗಮ 0;
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ವರ್ಣ ಧ್ರುವ *
suffix_power_char (ಪೂರ್ಣಾಙ್ಕ power)
{
  ವಿಕಲ್ಪ (power)
    {
    ಪಕ್ಷ 0:
      ಪ್ರತ್ಯಾಗಮ "";

    ಪಕ್ಷ 1:
      ಪ್ರತ್ಯಾಗಮ "K";

    ಪಕ್ಷ 2:
      ಪ್ರತ್ಯಾಗಮ "M";

    ಪಕ್ಷ 3:
      ಪ್ರತ್ಯಾಗಮ "G";

    ಪಕ್ಷ 4:
      ಪ್ರತ್ಯಾಗಮ "T";

    ಪಕ್ಷ 5:
      ಪ್ರತ್ಯಾಗಮ "P";

    ಪಕ್ಷ 6:
      ಪ್ರತ್ಯಾಗಮ "E";

    ಪಕ್ಷ 7:
      ಪ್ರತ್ಯಾಗಮ "Z";

    ಪಕ್ಷ 8:
      ಪ್ರತ್ಯಾಗಮ "Y";

    ಪಕ್ಷ 9:
      ಪ್ರತ್ಯಾಗಮ "R";

    ಪಕ್ಷ 10:
      ಪ್ರತ್ಯಾಗಮ "Q";

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
ಸ್ಥಿರ long ದ್ವಿಗುಣ
powerld (long ದ್ವಿಗುಣ base, ptrdiff_t x)
{
  long ದ್ವಿಗುಣ result = base;
  ಯದಿ (x == 0)
    ಪ್ರತ್ಯಾಗಮ 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  ಯಾವತ್ (--x)
    result *= base;
  ಪ್ರತ್ಯಾಗಮ result;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ long ದ್ವಿಗುಣ
absld (long ದ್ವಿಗುಣ val)
{
  ಪ್ರತ್ಯಾಗಮ val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
ಸ್ಥಿರ long ದ್ವಿಗುಣ
expld (long ದ್ವಿಗುಣ val, ಪೂರ್ಣಾಙ್ಕ base, ಪೂರ್ಣಾಙ್ಕ /*output */ *x)
{
  ಪೂರ್ಣಾಙ್ಕ power = 0;

  ಯದಿ (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      ಯಾವತ್ (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  ಯದಿ (x)
    *x = power;
  ಪ್ರತ್ಯಾಗಮ val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ intmax_t
simple_round_ceiling (long ದ್ವಿಗುಣ val)
{
  intmax_t intval = val;
  ಯದಿ (intval < val)
    intval++;
  ಪ್ರತ್ಯಾಗಮ intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ intmax_t
simple_round_floor (long ದ್ವಿಗುಣ val)
{
  ಪ್ರತ್ಯಾಗಮ -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ intmax_t
simple_round_from_zero (long ದ್ವಿಗುಣ val)
{
  ಪ್ರತ್ಯಾಗಮ val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ intmax_t
simple_round_to_zero (long ದ್ವಿಗುಣ val)
{
  ಪ್ರತ್ಯಾಗಮ val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ intmax_t
simple_round_nearest (long ದ್ವಿಗುಣ val)
{
  ಪ್ರತ್ಯಾಗಮ val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ long ದ್ವಿಗುಣ
simple_round (long ದ್ವಿಗುಣ val, ಗಣನಾ round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (long ದ್ವಿಗುಣ) INTMAX_MAX * intmax_mul;

  ವಿಕಲ್ಪ (t)
    {
    ಪಕ್ಷ round_ceiling:
      rval = simple_round_ceiling (val);
      ಭಙ್ಗ;

    ಪಕ್ಷ round_floor:
      rval = simple_round_floor (val);
      ಭಙ್ಗ;

    ಪಕ್ಷ round_from_zero:
      rval = simple_round_from_zero (val);
      ಭಙ್ಗ;

    ಪಕ್ಷ round_to_zero:
      rval = simple_round_to_zero (val);
      ಭಙ್ಗ;

    ಪಕ್ಷ round_nearest:
      rval = simple_round_nearest (val);
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      /* to silence the compiler - this should never happen.  */
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಪ್ರತ್ಯಾಗಮ (long ದ್ವಿಗುಣ) INTMAX_MAX * intmax_mul + rval;
}

ಗಣನಾ simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_OVERFLOW,
  SSE_INVALID_ಸಂಖ್ಯೆ,

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
ಸ್ಥಿರ ಗಣನಾ simple_strtod_error
simple_strtod_int (ವರ್ಣ ಧ್ರುವ *ಒಳಹರಿವು_str,
                   ವರ್ಣ **endptr, long ದ್ವಿಗುಣ *ಮೌಲ್ಯ, bool *negative)
{
  ಗಣನಾ simple_strtod_error e = SSE_OK;

  long ದ್ವಿಗುಣ val = 0;
  ಪೂರ್ಣಾಙ್ಕ digits = 0;
  bool found_digit = false;

  ಯದಿ (*ಒಳಹರಿವು_str == '-')
    {
      ಒಳಹರಿವು_str++;
      *negative = true;
    }
  ಅನ್ಯಥಾ
    *negative = false;

  *endptr = (ವರ್ಣ *) ಒಳಹರಿವು_str;
  ಯಾವತ್ (c_isdigit (**endptr))
    {
      ಪೂರ್ಣಾಙ್ಕ digit = (**endptr) - '0';

      found_digit = true;

      ಯದಿ (val || digit)
        digits++;

      ಯದಿ (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      ಯದಿ (digits > MAX_ACCEPTABLE_DIGITS)
        ಪ್ರತ್ಯಾಗಮ SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      ಯದಿ (thousands_sep_length > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_length)
          && c_isdigit ((*endptr)[thousands_sep_length]))
        (*endptr) += thousands_sep_length;
    }
  ಯದಿ (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    ಪ್ರತ್ಯಾಗಮ SSE_INVALID_ಸಂಖ್ಯೆ;
  ಯದಿ (*negative)
    val = -val;

  ಯದಿ (ಮೌಲ್ಯ)
    *ಮೌಲ್ಯ = val;

  ಪ್ರತ್ಯಾಗಮ e;
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
ಸ್ಥಿರ ಗಣನಾ simple_strtod_error
simple_strtod_float (ವರ್ಣ ಧ್ರುವ *ಒಳಹರಿವು_str,
                     ವರ್ಣ **endptr,
                     long ದ್ವಿಗುಣ *ಮೌಲ್ಯ,
                     size_t *precision)
{
  bool negative;
  ಗಣನಾ simple_strtod_error e = SSE_OK;

  ಯದಿ (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (ಒಳಹರಿವು_str, endptr, ಮೌಲ್ಯ, &negative);
  ಯದಿ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ಪ್ರತ್ಯಾಗಮ e;

  /* optional decimal point + fraction.  */
  ಯದಿ (STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    {
      ವರ್ಣ *ptr2;
      long ದ್ವಿಗುಣ val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_length;
      ಗಣನಾ simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      ಯದಿ (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        ಪ್ರತ್ಯಾಗಮ e2;
      ಯದಿ (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      ಯದಿ (neg_frac)
        ಪ್ರತ್ಯಾಗಮ SSE_INVALID_ಸಂಖ್ಯೆ;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      ಯದಿ (ಮೌಲ್ಯ)
        {
          ಯದಿ (negative)
            *ಮೌಲ್ಯ -= val_frac;
          ಅನ್ಯಥಾ
            *ಮೌಲ್ಯ += val_frac;
        }

      ಯದಿ (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  ಪ್ರತ್ಯಾಗಮ e;
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
ಸ್ಥಿರ ಗಣನಾ simple_strtod_error
simple_strtod_human (ವರ್ಣ ಧ್ರುವ *ಒಳಹರಿವು_str,
                     ವರ್ಣ **endptr, long ದ್ವಿಗುಣ *ಮೌಲ್ಯ, size_t *precision,
                     ಗಣನಾ scale_type allowed_scaling)
{
  ಪೂರ್ಣಾಙ್ಕ power = 0;
  /* 'scale_auto' is checked below.  */
  ಪೂರ್ಣಾಙ್ಕ scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, ಒಳಹರಿವು_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  ಗಣನಾ simple_strtod_error e =
    simple_strtod_float (ಒಳಹರಿವು_str, endptr, ಮೌಲ್ಯ, precision);
  ಯದಿ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ಪ್ರತ್ಯಾಗಮ e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *ಮೌಲ್ಯ, (ಪೂರ್ಣಾಙ್ಕ)*precision);

  ಯಾವತ್ (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      ಯದಿ (unit_separator)
        {
          size_t sep_len = strlen (unit_separator);
          ಯದಿ (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      ಯದಿ (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          ಯದಿ (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      ಯದಿ (**endptr == '\0')
        ಭಙ್ಗ;  /* Treat as no suffix.  */

      ಯದಿ (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          ಯದಿ (**endptr == '\0')
            ಭಙ್ಗ;

          ಪ್ರತ್ಯಾಗಮ SSE_INVALID_SUFFIX;
        }

      ಯದಿ (allowed_scaling == scale_none)
        ಪ್ರತ್ಯಾಗಮ SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      ಯದಿ (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      ಅನ್ಯಥಾ ಯದಿ (allowed_scaling == scale_IEC_I)
        {
          ಯದಿ (**endptr == 'i')
            (*endptr)++;
          ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      ಭಙ್ಗ;
    }

  long ದ್ವಿಗುಣ multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*ಮೌಲ್ಯ) = (*ಮೌಲ್ಯ) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *ಮೌಲ್ಯ, *ಮೌಲ್ಯ);

  ಪ್ರತ್ಯಾಗಮ e;
}


ಸ್ಥಿರ ಶೂನ್ಯ
simple_strtod_fatal (ಗಣನಾ simple_strtod_error err, ವರ್ಣ ಧ್ರುವ *ಒಳಹರಿವು_str)
{
  ವರ್ಣ ಧ್ರುವ *msgid = ರಿಕ್ತ;

  ವಿಕಲ್ಪ (err)
    {
    ಪಕ್ಷ SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      ಭಙ್ಗ;

    ಪಕ್ಷ SSE_INVALID_ಸಂಖ್ಯೆ:
      msgid = N_("invalid number: %s");
      ಭಙ್ಗ;

    ಪಕ್ಷ SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      ಭಙ್ಗ;

    ಪಕ್ಷ SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      ಭಙ್ಗ;

    ಪಕ್ಷ SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      ಭಙ್ಗ;

    ಪಕ್ಷ SSE_OK_PRECISION_LOSS:
    ಪಕ್ಷ SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  ಯದಿ (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (ಒಳಹರಿವು_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
double_to_human (long ದ್ವಿಗುಣ val, ಪೂರ್ಣಾಙ್ಕ precision,
                 ವರ್ಣ *buf, idx_t buf_size,
                 ಗಣನಾ scale_type scale, ಪೂರ್ಣಾಙ್ಕ group, ಗಣನಾ round_type round)
{
  ವರ್ಣ fmt[ಪರಿಮಾಣ "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  ವರ್ಣ *pfmt = fmt;
  *pfmt++ = '%';

  ಯದಿ (group)
    *pfmt++ = '\'';

  ಯದಿ (zero_padding_width)
    pfmt += sprintf (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  ಯದಿ (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      ಪ್ರತ್ಯಾಗಮ snprintf (buf, buf_size, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  ದ್ವಿಗುಣ scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  ಪೂರ್ಣಾಙ್ಕ power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  ಪೂರ್ಣಾಙ್ಕ power_adjust = 0;
  ಯದಿ (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  ಅನ್ಯಥಾ ಯದಿ (absld (val) < 10)
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
  ಯದಿ (absld (val) >= scale_base)
    {
      val /= scale_base;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  ಪೂರ್ಣಾಙ್ಕ show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  ಪೂರ್ಣಾಙ್ಕ prec = user_precision == -1 ? show_decimal_point : user_precision;

  ಪ್ರತ್ಯಾಗಮ snprintf (buf, buf_size, fmt, prec, val,
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
ಸ್ಥಿರ uintmax_t
unit_to_umax (ವರ್ಣ ಧ್ರುವ *n_string)
{
  strtol_error s_err;
  ವರ್ಣ ಧ್ರುವ *c_string = n_string;
  ವರ್ಣ *t_string = ರಿಕ್ತ;
  size_t n_len = strlen (n_string);
  ವರ್ಣ *end = ರಿಕ್ತ;
  uintmax_t n;
  ವರ್ಣ ಧ್ರುವ *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  ಯದಿ (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      memcpy (t_string, n_string, n_len);

      ಯದಿ (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
        *end = '\0';
      ಅನ್ಯಥಾ
        {
          *++end = 'B';
          *++end = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &end, 10, &n, suffixes);

  ಯದಿ (s_err != LONGINT_OK || *end || n == 0)
    {
      ಮುಕ್ತ (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  ಮುಕ್ತ (t_string);

  ಪ್ರತ್ಯಾಗಮ n;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [ಸಂಖ್ಯೆ_2]...\n\
"), program_name);
      fputs (_("\
Reformat ಸಂಖ್ಯೆ_2(s), or the numbers from standard ಒಳಹರಿವು ಯದಿ none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid ಒಳಹರಿವು\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace ಪ್ರತಿ field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these ಒಳಹರಿವು fields (ಸ್ವಭಾವ=1);\n\
         see FIELDS below ಪ್ರತಿ details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use printf style floating-point FORMAT;\n\
         see FORMAT below ಪ್ರತಿ details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         auto-scale ಒಳಹರಿವು numbers to UNITs; ಸ್ವಭಾವ is 'none';\n\
         see UNIT below ಪ್ರತಿ details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the ಒಳಹರಿವು unit size (instead of the ಸ್ವಭಾವ 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header ಸಾಲುಗಳು;\n\
         N defaults to 1 ಯದಿ not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode ಪ್ರತಿ invalid numbers;\n\
         MODE can be: abort (ಸ್ವಭಾವ), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will ಬಲ-align, negative N will ಎಡ-align;\n\
         padding is ignored ಯದಿ the output is wider than N;\n\
         the ಸ್ವಭಾವ is to automatically pad ಯದಿ a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD ಪ್ರತಿ rounding when scaling; METHOD can be:\n\
         up, down, from-zero (ಸ್ವಭಾವ), towards-zero, nearest\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         add SUFFIX to output numbers,\n\
         and accept an optional SUFFIX in ಒಳಹರಿವು numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         ಸೇರಿಸು SEP between ಸಂಖ್ಯೆ and unit on output,\n\
         and accept an optional SEP in ಒಳಹರಿವು numbers\n\
"));
      oputs (_("\
      --to=UNIT\n\
         auto-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit size (instead of the ಸ್ವಭಾವ 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
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
  N-   from N'th field, to end of ಸಾಲು\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable ಪ್ರತಿ printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (ಯದಿ supported by current locale).\n\
Optional width ಮೌಲ್ಯ (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the ಸಂಖ್ಯೆ. Optional negative ಮೌಲ್ಯಗಳು (%-10f) will ಎಡ align.\n\
Optional precision (%.1f) will override the ಒಳಹರಿವು determined precision.\n\
"), stdout);

      printf (_("\n\
Exit status is 0 ಯದಿ all ಒಳಹರಿವು numbers were successfully converted.\n\
By ಸ್ವಭಾವ, %s will stop at the first conversion error with exit status 2.\n\
With --invalid='fail' a warning is printed ಪ್ರತಿ each conversion error\n\
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
ಸ್ಥಿರ ಶೂನ್ಯ
parse_format_string (ವರ್ಣ ಧ್ರುವ *fmt)
{
  size_t i;
  size_t prefix_len = 0;
  size_t suffix_pos;
  ವರ್ಣ *endptr = ರಿಕ್ತ;
  bool zero_padding = false;

  ಪ್ರತಿ (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      ಯದಿ (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  ಯಾವತ್ (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      ಯದಿ (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      ಅನ್ಯಥಾ ಯದಿ (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      ಅನ್ಯಥಾ ಯದಿ (! skip)
        ಭಙ್ಗ;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  ಯದಿ (pad != 0)
    {
      ಯದಿ (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      ಯದಿ (pad < 0)
        padding_width = pad;
      ಅನ್ಯಥಾ
        {
          ಯದಿ (zero_padding)
            zero_padding_width = MIN (pad, INT_MAX);
          ಅನ್ಯಥಾ
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  ಯದಿ (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  ಯದಿ (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      ಯದಿ (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  ಯದಿ (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  ಪ್ರತಿ (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    ಯದಿ (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  ಯದಿ (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  ಯದಿ (fmt[suffix_pos] != '\0')
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
ಸ್ಥಿರ ಗಣನಾ simple_strtod_error
parse_human_ಸಂಖ್ಯೆ (ವರ್ಣ ಧ್ರುವ *str, long ದ್ವಿಗುಣ /*output */ *ಮೌಲ್ಯ,
                    size_t *precision)
{
  ವರ್ಣ *ptr = ರಿಕ್ತ;

  ಗಣನಾ simple_strtod_error e =
    simple_strtod_human (str, &ptr, ಮೌಲ್ಯ, precision, scale_from);
  ಯದಿ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      ಪ್ರತ್ಯಾಗಮ e;
    }

  ಯದಿ (ptr && *ptr != '\0')
    {
      ಯದಿ (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  ಪ್ರತ್ಯಾಗಮ e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
ಸ್ಥಿರ bool
prepare_padded_ಸಂಖ್ಯೆ (ಧ್ರುವ long ದ್ವಿಗುಣ val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  ಪೂರ್ಣಾಙ್ಕ x;
  expld (val, 10, &x);

  ಯದಿ (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      ಯದಿ (inval_style != inval_ignore)
        {
          ಯದಿ (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          ಅನ್ಯಥಾ
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಯದಿ (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      ಯದಿ (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಯಾವತ್ (true)
    {
      ಪೂರ್ಣಾಙ್ಕ numlen = double_to_human (val, precision_used,
                                    padding_buffer, padding_buffer_size,
                                    scale_to, grouping, round_style);
      ptrdiff_t growth;
      ಯದಿ (numlen < 0 || ckd_sub (&growth, numlen, padding_buffer_size - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      ಯದಿ (growth <= 0)
        ಭಙ್ಗ;
      padding_buffer = xpalloc (padding_buffer, &padding_buffer_size,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_buffer));

  intmax_t pad = 0;
  ಯದಿ (padding_width)
    {
      ಪೂರ್ಣಾಙ್ಕ buf_width = mbswidth (padding_buffer,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      ಯದಿ (0 <= buf_width)
        {
          ಯದಿ (padding_width < 0)
            {
              ಯದಿ (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          ಅನ್ಯಥಾ
            {
              ಯದಿ (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_padded_ಸಂಖ್ಯೆ (intmax_t padding)
{
  ಯದಿ (format_str_prefix)
    fputs (format_str_prefix, stdout);

  ಪ್ರತಿ (intmax_t p = padding; 0 < p; p--)
    putchar (' ');

  fputs (padding_buffer, stdout);

  ಪ್ರತಿ (intmax_t p = padding; p < 0; p++)
    putchar (' ');

  ಯದಿ (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
ಸ್ಥಿರ bool
process_suffixed_ಸಂಖ್ಯೆ (ವರ್ಣ *text, long ದ್ವಿಗುಣ *result,
                         size_t *precision, long ಪೂರ್ಣಾಙ್ಕ field)
{
  ವರ್ಣ saved_suffix = '\0';

  ಯದಿ (suffix)
    {
      ಯದಿ (mbs_endswith (text, suffix))
        {
          saved_suffix = *(text + strlen (text) - strlen (suffix));
          *(text + strlen (text) - strlen (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      ಅನ್ಯಥಾ
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  ವರ್ಣ *p = skip_str_matching (text, newline_or_blank, true);

  /* setup auto-padding.  */
  ಯದಿ (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      ಯದಿ (padding_width < 0)
        padding_width = strlen (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  long ದ್ವಿಗುಣ val = 0;
  ಗಣನಾ simple_strtod_error e = parse_human_ಸಂಖ್ಯೆ (p, &val, precision);
  ಯದಿ (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  ಯದಿ (from_unit_size != 1 || to_unit_size != 1)
    val = (val * from_unit_size) / to_unit_size;

  *result = val;

  ಯದಿ (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    ಪ್ರತ್ಯಾಗಮ true;
  ಅನ್ಯಥಾ
    {
      ಯದಿ (saved_suffix)
        *(text + strlen (text)) = saved_suffix;
      ಪ್ರತ್ಯಾಗಮ false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
ಸ್ಥಿರ ವರ್ಣ *
mbsmbchr (ವರ್ಣ ಧ್ರುವ *s, ವರ್ಣ ಧ್ರುವ *c)
{
  unsigned ವರ್ಣ uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  ಯದಿ (uc < 0x30 || MB_CUR_MAX == 1)
    ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *) strchr (s, uc);
  ಅನ್ಯಥಾ ಯದಿ (is_utf8_charset ())
    ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *) (uc < 0x80 ? strchr (s, uc) : strstr (s, c));
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ *(c + 1) == '\0' ? mbschr (s, uc) : (ವರ್ಣ *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
ಸ್ಥಿರ ವರ್ಣ *
next_field (ವರ್ಣ **ಸಾಲು)
{
  ವರ್ಣ *field_start = *ಸಾಲು;
  ವರ್ಣ *field_end   = field_start;

  ಯದಿ (delimiter)
    {
      ಯದಿ (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = strchr (field_start, '\0');
    }
  ಅನ್ಯಥಾ
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *ಸಾಲು = field_end;
  ಪ್ರತ್ಯಾಗಮ field_start;
}

ATTRIBUTE_PURE
ಸ್ಥಿರ bool
include_field (uintmax_t field)
{
  ಸಂರಚನಾ field_range_pair *p = frp;
  ಯದಿ (!p)
    ಪ್ರತ್ಯಾಗಮ field == 1;

  ಯಾವತ್ (p->lo != UINTMAX_MAX)
    {
      ಯದಿ (p->lo <= field && p->hi >= field)
        ಪ್ರತ್ಯಾಗಮ true;
      ++p;
    }
  ಪ್ರತ್ಯಾಗಮ false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
ಸ್ಥಿರ bool
process_field (ವರ್ಣ *text, uintmax_t field)
{
  long ದ್ವಿಗುಣ val = 0;
  size_t precision = 0;
  bool valid_ಸಂಖ್ಯೆ = true;

  ಯದಿ (include_field (field))
    {
      valid_ಸಂಖ್ಯೆ =
        process_suffixed_ಸಂಖ್ಯೆ (text, &val, &precision, field);

      intmax_t padding;
      ಯದಿ (valid_ಸಂಖ್ಯೆ)
        valid_ಸಂಖ್ಯೆ = prepare_padded_ಸಂಖ್ಯೆ (val, precision, &padding);

      ಯದಿ (valid_ಸಂಖ್ಯೆ)
        print_padded_ಸಂಖ್ಯೆ (padding);
      ಅನ್ಯಥಾ
        fputs (text, stdout);
    }
  ಅನ್ಯಥಾ
    fputs (text, stdout);

  ಪ್ರತ್ಯಾಗಮ valid_ಸಂಖ್ಯೆ;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
process_ಸಾಲು (ವರ್ಣ *ಸಾಲು, bool newline)
{
  ವರ್ಣ *next;
  uintmax_t field = 0;
  bool valid_ಸಂಖ್ಯೆ = true;

  ಯಾವತ್ (true) {
    ++field;
    next = next_field (&ಸಾಲು);

    ಯದಿ (*ಸಾಲು != '\0')
      {
        /* NUL terminate the current field string and process */
        ವರ್ಣ end_field = *ಸಾಲು;
        *ಸಾಲು = '\0';

        ಯದಿ (! process_field (next, field))
          valid_ಸಂಖ್ಯೆ = false;

        ಯದಿ (delimiter != ರಿಕ್ತ)
          fputs (delimiter, stdout);
        ಅನ್ಯಥಾ
          fputc (' ', stdout);

        ಯದಿ (delimiter)
          ಸಾಲು += MAX (strlen (delimiter), 1);
        ಅನ್ಯಥಾ
          {
            *ಸಾಲು = end_field;
            mcel_t g = mcel_scanz (ಸಾಲು);
            ಸಾಲು += g.len;
          }
      }
    ಅನ್ಯಥಾ
      {
        /* end of the line, process the last field and finish */
        ಯದಿ (! process_field (next, field))
          valid_ಸಂಖ್ಯೆ = false;

        ಭಙ್ಗ;
      }
  }

  ಯದಿ (newline)
    putchar (ಸಾಲು_delim);

 ಯದಿ (ferror (stdout))
   write_error ();

  ಪ್ರತ್ಯಾಗಮ valid_ಸಂಖ್ಯೆ;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#ಯದಿ HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#endif

  decimal_point = nl_langinfo (RADIXCHAR);
  ಯದಿ (decimal_point == ರಿಕ್ತ || strlen (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_length = strlen (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  ಯದಿ (thousands_sep == ರಿಕ್ತ)
    thousands_sep = "";
  thousands_sep_length = strlen (thousands_sep);

  atexit (close_stdout);

  ಯಾವತ್ (true)
    {
      ಪೂರ್ಣಾಙ್ಕ c = getopt_long (argc, argv, "d:z", longopts, ರಿಕ್ತ);

      ಯದಿ (c == -1)
        ಭಙ್ಗ;

      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          ಭಙ್ಗ;

        ಪಕ್ಷ FROM_UNIT_OPTION:
          from_unit_size = unit_to_umax (optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          ಭಙ್ಗ;

        ಪಕ್ಷ TO_UNIT_OPTION:
          to_unit_size = unit_to_umax (optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          ಭಙ್ಗ;

        ಪಕ್ಷ GROUPING_OPTION:
          grouping = 1;
          ಭಙ್ಗ;

        ಪಕ್ಷ PADDING_OPTION:
          ಯದಿ (((xstrtoimax (optarg, ರಿಕ್ತ, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          ಭಙ್ಗ;

        ಪಕ್ಷ FIELD_OPTION:
          ಯದಿ (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          ಯದಿ (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              ಯದಿ (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'z':
          ಸಾಲು_delim = '\0';
          ಭಙ್ಗ;

        ಪಕ್ಷ SUFFIX_OPTION:
          suffix = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ DEBUG_OPTION:
          debug = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ HEADER_OPTION:
          ಯದಿ (optarg)
            {
              ಯದಿ (xstrtoumax (optarg, ರಿಕ್ತ, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          ಅನ್ಯಥಾ
            {
              header = 1;
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ FORMAT_OPTION:
          format_str = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          ಭಙ್ಗ;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (format_str != ರಿಕ್ತ && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  ಯದಿ (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  ಯದಿ (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == ರಿಕ್ತ))
    error (0, 0, _("no conversion option specified"));

  ಯದಿ (debug && unit_separator && delimiter == ರಿಕ್ತ)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  ಯದಿ (format_str)
    parse_format_string (format_str);

  ಯದಿ (grouping)
    {
      ಯದಿ (scale_to != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      ಯದಿ (debug && thousands_sep_length == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == ರಿಕ್ತ);

  ಯದಿ (inval_style != inval_abort)
    conv_exit_code = 0;

  ಯದಿ (argc > optind)
    {
      ಯದಿ (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      ಪ್ರತಿ (; optind < argc; optind++)
        valid_numbers &= process_ಸಾಲು (argv[optind], true);
    }
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ *ಸಾಲು = ರಿಕ್ತ;
      size_t ಸಾಲು_allocated = 0;
      ssize_t len;

      ಯಾವತ್ (header-- && getdelim (&ಸಾಲು, &ಸಾಲು_allocated,
                                   ಸಾಲು_delim, stdin) > 0)
        {
          ಯದಿ (fputs (ಸಾಲು, stdout) == EOF)
            write_error ();
        }

      ಯಾವತ್ ((len = getdelim (&ಸಾಲು, &ಸಾಲು_allocated,
                              ಸಾಲು_delim, stdin)) > 0)
        {
          bool newline = ಸಾಲು[len - 1] == ಸಾಲು_delim;
          ಯದಿ (newline)
            ಸಾಲು[len - 1] = '\0';
          valid_numbers &= process_ಸಾಲು (ಸಾಲು, newline);
        }

      ಯದಿ (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  ಯದಿ (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  ಪೂರ್ಣಾಙ್ಕ exit_status = EXIT_SUCCESS;
  ಯದಿ (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
