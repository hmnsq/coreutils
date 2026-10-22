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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <float.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <langinfo.h>

#અન્તર્ભાવ "argmatch.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "mbswidth.h"
#અન્તર્ભાવ "mcel.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "skipchars.h"
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "utf8.h"
#અન્તર્ભાવ "xstrtol.h"

#અન્તર્ભાવ "set-fields.h"

#જો HAVE_FPSETPREC
# include <ieeefp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "numfmt"

#define AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
ગણના { EXIT_CONVERSION_WARNINGS = 2 };

ગણના
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

ગણના scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

સ્થિર અક્ષર અચલ *અચલ scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", ખાલી
};

સ્થિર ગણના scale_type અચલ scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

સ્થિર અક્ષર અચલ *અચલ scale_to_args[] =
{
  "none", "si", "iec", "iec-i", ખાલી
};

સ્થિર ગણના scale_type અચલ scale_to_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


ગણના round_type
{
  round_ceiling,
  round_floor,
  round_from_zero,
  round_to_zero,
  round_nearest,
};

સ્થિર અક્ષર અચલ *અચલ round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", ખાલી
};

સ્થિર ગણના round_type અચલ round_types[] =
{
  round_ceiling, round_floor, round_from_zero, round_to_zero, round_nearest
};


ગણના inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

સ્થિર અક્ષર અચલ *અચલ inval_args[] =
{
  "abort", "fail", "warn", "ignore", ખાલી
};

સ્થિર ગણના inval_type અચલ inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"from", required_argument, ખાલી, FROM_OPTION},
  {"from-unit", required_argument, ખાલી, FROM_UNIT_OPTION},
  {"to", required_argument, ખાલી, TO_OPTION},
  {"to-unit", required_argument, ખાલી, TO_UNIT_OPTION},
  {"round", required_argument, ખાલી, ROUND_OPTION},
  {"padding", required_argument, ખાલી, PADDING_OPTION},
  {"suffix", required_argument, ખાલી, SUFFIX_OPTION},
  {"unit-separator", required_argument, ખાલી, UNIT_SEPARATOR_OPTION},
  {"grouping", no_argument, ખાલી, GROUPING_OPTION},
  {"delimiter", required_argument, ખાલી, 'd'},
  {"field", required_argument, ખાલી, FIELD_OPTION},
  {"debug", no_argument, ખાલી, DEBUG_OPTION},
  {"-debug", no_argument, ખાલી, DEV_DEBUG_OPTION},
  {"header", optional_argument, ખાલી, HEADER_OPTION},
  {"format", required_argument, ખાલી, FORMAT_OPTION},
  {"invalid", required_argument, ખાલી, INVALID_OPTION},
  {"zero-terminated", no_argument, ખાલી, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
ગણના { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
ગણના { MAX_ACCEPTABLE_DIGITS = 33 };

સ્થિર ગણના scale_type scale_from = scale_none;
સ્થિર ગણના scale_type scale_to = scale_none;
સ્થિર ગણના round_type round_style = round_from_zero;
સ્થિર ગણના inval_type inval_style = inval_abort;
સ્થિર અક્ષર અચલ *suffix = ખાલી;
સ્થિર અક્ષર અચલ *unit_separator = ખાલી;
સ્થિર uintmax_t from_unit_size = 1;
સ્થિર uintmax_t to_unit_size = 1;
સ્થિર પૂર્ણાંક grouping = 0;
સ્થિર અક્ષર *padding_buffer = ખાલી;
સ્થિર idx_t padding_buffer_size = 0;
સ્થિર intmax_t padding_width = 0;
સ્થિર પૂર્ણાંક zero_padding_width = 0;
સ્થિર long પૂર્ણાંક user_precision = -1;
સ્થિર અક્ષર અચલ *format_str = ખાલી;
સ્થિર અક્ષર *format_str_prefix = ખાલી;
સ્થિર અક્ષર *format_str_suffix = ખાલી;

/* By default, any conversion error will terminate the program.  */
સ્થિર પૂર્ણાંક conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
સ્થિર પૂર્ણાંક auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
સ્થિર અક્ષર અચલ *delimiter = ખાલી;

/* line delimiter.  */
સ્થિર unsigned અક્ષર લીટી_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
સ્થિર uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
સ્થિર bool debug;

/* will be set according to the current locale.  */
સ્થિર અક્ષર અચલ *decimal_point;
સ્થિર પૂર્ણાંક decimal_point_length;
સ્થિર અક્ષર અચલ *thousands_sep;
સ્થિર પૂર્ણાંક thousands_sep_length;

/* debugging for developers.  Enables devmsg().  */
સ્થિર bool dev_debug = false;

સ્થિર bool
newline_or_blank (mcel_t g)
{
  પાછા g.ch == '\n' || c32issep (g.ch);
}

સ્થિર અંતઃરેખ પૂર્ણાંક
default_scale_base (ગણના scale_type scale)
{
  પસંદગી (scale)
    {
    સ્થિતિ scale_IEC:
    સ્થિતિ scale_IEC_I:
      પાછા 1024;

    સ્થિતિ scale_none:
    સ્થિતિ scale_auto:
    સ્થિતિ scale_SI:
    મૂળભૂત:
      પાછા 1000;
    }
}

સ્થિર અક્ષર અચલ zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
સ્થિર અક્ષર અચલ *valid_suffixes = 1 + zero_and_valid_suffixes;

સ્થિર અંતઃરેખ bool
valid_suffix (અચલ અક્ષર suf)
{
  પાછા strchr (valid_suffixes, suf) != ખાલી;
}

સ્થિર અંતઃરેખ પૂર્ણાંક
suffix_power (અચલ અક્ષર suf)
{
  પસંદગી (suf)
    {
    સ્થિતિ 'k':                  /* kilo.  */
    સ્થિતિ 'K':                  /* kilo or kibi.  */
      પાછા 1;

    સ્થિતિ 'M':                  /* mega or mebi.  */
      પાછા 2;

    સ્થિતિ 'G':                  /* giga or gibi.  */
      પાછા 3;

    સ્થિતિ 'T':                  /* tera or tebi.  */
      પાછા 4;

    સ્થિતિ 'P':                  /* peta or pebi.  */
      પાછા 5;

    સ્થિતિ 'E':                  /* exa or exbi.  */
      પાછા 6;

    સ્થિતિ 'Z':                  /* zetta or 2**70.  */
      પાછા 7;

    સ્થિતિ 'Y':                  /* yotta or 2**80.  */
      પાછા 8;

    સ્થિતિ 'R':                  /* ronna or 2**90.  */
      પાછા 9;

    સ્થિતિ 'Q':                  /* quetta or 2**100.  */
      પાછા 10;

    મૂળભૂત:                   /* should never happen. assert?  */
      પાછા 0;
    }
}

સ્થિર અંતઃરેખ અક્ષર અચલ *
suffix_power_char (પૂર્ણાંક power)
{
  પસંદગી (power)
    {
    સ્થિતિ 0:
      પાછા "";

    સ્થિતિ 1:
      પાછા "K";

    સ્થિતિ 2:
      પાછા "M";

    સ્થિતિ 3:
      પાછા "G";

    સ્થિતિ 4:
      પાછા "T";

    સ્થિતિ 5:
      પાછા "P";

    સ્થિતિ 6:
      પાછા "E";

    સ્થિતિ 7:
      પાછા "Z";

    સ્થિતિ 8:
      પાછા "Y";

    સ્થિતિ 9:
      પાછા "R";

    સ્થિતિ 10:
      પાછા "Q";

    મૂળભૂત:
      પાછા "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
સ્થિર long દ્વિગુણ
powerld (long દ્વિગુણ base, ptrdiff_t x)
{
  long દ્વિગુણ result = base;
  જો (x == 0)
    પાછા 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  જ્યારે (--x)
    result *= base;
  પાછા result;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
સ્થિર અંતઃરેખ long દ્વિગુણ
absld (long દ્વિગુણ val)
{
  પાછા val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
સ્થિર long દ્વિગુણ
expld (long દ્વિગુણ val, પૂર્ણાંક base, પૂર્ણાંક /*output */ *x)
{
  પૂર્ણાંક power = 0;

  જો (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      જ્યારે (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  જો (x)
    *x = power;
  પાછા val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
સ્થિર અંતઃરેખ intmax_t
simple_round_ceiling (long દ્વિગુણ val)
{
  intmax_t intval = val;
  જો (intval < val)
    intval++;
  પાછા intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
સ્થિર અંતઃરેખ intmax_t
simple_round_floor (long દ્વિગુણ val)
{
  પાછા -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
સ્થિર અંતઃરેખ intmax_t
simple_round_from_zero (long દ્વિગુણ val)
{
  પાછા val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
સ્થિર અંતઃરેખ intmax_t
simple_round_to_zero (long દ્વિગુણ val)
{
  પાછા val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
સ્થિર અંતઃરેખ intmax_t
simple_round_nearest (long દ્વિગુણ val)
{
  પાછા val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
સ્થિર અંતઃરેખ long દ્વિગુણ
simple_round (long દ્વિગુણ val, ગણના round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (long દ્વિગુણ) INTMAX_MAX * intmax_mul;

  પસંદગી (t)
    {
    સ્થિતિ round_ceiling:
      rval = simple_round_ceiling (val);
      તોડો;

    સ્થિતિ round_floor:
      rval = simple_round_floor (val);
      તોડો;

    સ્થિતિ round_from_zero:
      rval = simple_round_from_zero (val);
      તોડો;

    સ્થિતિ round_to_zero:
      rval = simple_round_to_zero (val);
      તોડો;

    સ્થિતિ round_nearest:
      rval = simple_round_nearest (val);
      તોડો;

    મૂળભૂત:
      /* to silence the compiler - this should never happen.  */
      પાછા 0;
    }

  પાછા (long દ્વિગુણ) INTMAX_MAX * intmax_mul + rval;
}

ગણના simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_OVERFLOW,
  SSE_INVALID_સંખ્યા,

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
સ્થિર ગણના simple_strtod_error
simple_strtod_int (અક્ષર અચલ *આગત_str,
                   અક્ષર **endptr, long દ્વિગુણ *કિંમત, bool *negative)
{
  ગણના simple_strtod_error e = SSE_OK;

  long દ્વિગુણ val = 0;
  પૂર્ણાંક digits = 0;
  bool found_digit = false;

  જો (*આગત_str == '-')
    {
      આગત_str++;
      *negative = true;
    }
  નહીંતર
    *negative = false;

  *endptr = (અક્ષર *) આગત_str;
  જ્યારે (c_isdigit (**endptr))
    {
      પૂર્ણાંક digit = (**endptr) - '0';

      found_digit = true;

      જો (val || digit)
        digits++;

      જો (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      જો (digits > MAX_ACCEPTABLE_DIGITS)
        પાછા SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      જો (thousands_sep_length > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_length)
          && c_isdigit ((*endptr)[thousands_sep_length]))
        (*endptr) += thousands_sep_length;
    }
  જો (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    પાછા SSE_INVALID_સંખ્યા;
  જો (*negative)
    val = -val;

  જો (કિંમત)
    *કિંમત = val;

  પાછા e;
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
સ્થિર ગણના simple_strtod_error
simple_strtod_float (અક્ષર અચલ *આગત_str,
                     અક્ષર **endptr,
                     long દ્વિગુણ *કિંમત,
                     size_t *precision)
{
  bool negative;
  ગણના simple_strtod_error e = SSE_OK;

  જો (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (આગત_str, endptr, કિંમત, &negative);
  જો (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    પાછા e;

  /* optional decimal point + fraction.  */
  જો (STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    {
      અક્ષર *ptr2;
      long દ્વિગુણ val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_length;
      ગણના simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      જો (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        પાછા e2;
      જો (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      જો (neg_frac)
        પાછા SSE_INVALID_સંખ્યા;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      જો (કિંમત)
        {
          જો (negative)
            *કિંમત -= val_frac;
          નહીંતર
            *કિંમત += val_frac;
        }

      જો (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  પાછા e;
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
સ્થિર ગણના simple_strtod_error
simple_strtod_human (અક્ષર અચલ *આગત_str,
                     અક્ષર **endptr, long દ્વિગુણ *કિંમત, size_t *precision,
                     ગણના scale_type allowed_scaling)
{
  પૂર્ણાંક power = 0;
  /* 'scale_auto' is checked below.  */
  પૂર્ણાંક scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, આગત_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  ગણના simple_strtod_error e =
    simple_strtod_float (આગત_str, endptr, કિંમત, precision);
  જો (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    પાછા e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *કિંમત, (પૂર્ણાંક)*precision);

  જ્યારે (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      જો (unit_separator)
        {
          size_t sep_len = strlen (unit_separator);
          જો (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      જો (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          જો (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      જો (**endptr == '\0')
        તોડો;  /* Treat as no suffix.  */

      જો (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          જો (**endptr == '\0')
            તોડો;

          પાછા SSE_INVALID_SUFFIX;
        }

      જો (allowed_scaling == scale_none)
        પાછા SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      જો (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      નહીંતર જો (allowed_scaling == scale_IEC_I)
        {
          જો (**endptr == 'i')
            (*endptr)++;
          નહીંતર
            પાછા SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      તોડો;
    }

  long દ્વિગુણ multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*કિંમત) = (*કિંમત) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *કિંમત, *કિંમત);

  પાછા e;
}


સ્થિર શૂન્ય
simple_strtod_fatal (ગણના simple_strtod_error err, અક્ષર અચલ *આગત_str)
{
  અક્ષર અચલ *msgid = ખાલી;

  પસંદગી (err)
    {
    સ્થિતિ SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      તોડો;

    સ્થિતિ SSE_INVALID_સંખ્યા:
      msgid = N_("invalid number: %s");
      તોડો;

    સ્થિતિ SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      તોડો;

    સ્થિતિ SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      તોડો;

    સ્થિતિ SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      તોડો;

    સ્થિતિ SSE_OK_PRECISION_LOSS:
    સ્થિતિ SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  જો (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (આગત_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
સ્થિર પૂર્ણાંક
double_to_human (long દ્વિગુણ val, પૂર્ણાંક precision,
                 અક્ષર *buf, idx_t buf_size,
                 ગણના scale_type scale, પૂર્ણાંક group, ગણના round_type round)
{
  અક્ષર fmt[કદ "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  અક્ષર *pfmt = fmt;
  *pfmt++ = '%';

  જો (group)
    *pfmt++ = '\'';

  જો (zero_padding_width)
    pfmt += sprintf (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  જો (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      પાછા snprintf (buf, buf_size, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  દ્વિગુણ scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  પૂર્ણાંક power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  પૂર્ણાંક power_adjust = 0;
  જો (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  નહીંતર જો (absld (val) < 10)
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
  જો (absld (val) >= scale_base)
    {
      val /= scale_base;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  પૂર્ણાંક show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  પૂર્ણાંક prec = user_precision == -1 ? show_decimal_point : user_precision;

  પાછા snprintf (buf, buf_size, fmt, prec, val,
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
સ્થિર uintmax_t
unit_to_umax (અક્ષર અચલ *n_string)
{
  strtol_error s_err;
  અક્ષર અચલ *c_string = n_string;
  અક્ષર *t_string = ખાલી;
  size_t n_len = strlen (n_string);
  અક્ષર *end = ખાલી;
  uintmax_t n;
  અક્ષર અચલ *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  જો (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      memcpy (t_string, n_string, n_len);

      જો (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
        *end = '\0';
      નહીંતર
        {
          *++end = 'B';
          *++end = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &end, 10, &n, suffixes);

  જો (s_err != LONGINT_OK || *end || n == 0)
    {
      મુક્ત (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  મુક્ત (t_string);

  પાછા n;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [સંખ્યા_2]...\n\
"), program_name);
      fputs (_("\
Reformat સંખ્યા_2(s), or the numbers from standard આગત જો none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid આગત\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace માટે field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these આગત fields (મૂળભૂત=1);\n\
         see FIELDS below માટે details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use printf style floating-point FORMAT;\n\
         see FORMAT below માટે details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         auto-scale આગત numbers to UNITs; મૂળભૂત is 'none';\n\
         see UNIT below માટે details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the આગત unit size (instead of the મૂળભૂત 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header લીટીઓ;\n\
         N defaults to 1 જો not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode માટે invalid numbers;\n\
         MODE can be: abort (મૂળભૂત), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will જમણું-align, negative N will ડાબું-align;\n\
         padding is ignored જો the output is wider than N;\n\
         the મૂળભૂત is to automatically pad જો a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD માટે rounding when scaling; METHOD can be:\n\
         up, down, from-zero (મૂળભૂત), towards-zero, nearest\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         add SUFFIX to output numbers,\n\
         and accept an optional SUFFIX in આગત numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         ઉમેરો SEP between સંખ્યા and unit on output,\n\
         and accept an optional SEP in આગત numbers\n\
"));
      oputs (_("\
      --to=UNIT\n\
         auto-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit size (instead of the મૂળભૂત 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         લીટી delimiter is NUL, not newline\n\
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
  N-   from N'th field, to end of લીટી\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable માટે printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (જો supported by current locale).\n\
Optional width કિંમત (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the સંખ્યા. Optional negative કિંમતો (%-10f) will ડાબું align.\n\
Optional precision (%.1f) will override the આગત determined precision.\n\
"), stdout);

      printf (_("\n\
Exit status is 0 જો all આગત numbers were successfully converted.\n\
By મૂળભૂત, %s will stop at the first conversion error with exit status 2.\n\
With --invalid='fail' a warning is printed માટે each conversion error\n\
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
સ્થિર શૂન્ય
parse_format_string (અક્ષર અચલ *fmt)
{
  size_t i;
  size_t prefix_len = 0;
  size_t suffix_pos;
  અક્ષર *endptr = ખાલી;
  bool zero_padding = false;

  માટે (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      જો (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  જ્યારે (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      જો (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      નહીંતર જો (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      નહીંતર જો (! skip)
        તોડો;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  જો (pad != 0)
    {
      જો (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      જો (pad < 0)
        padding_width = pad;
      નહીંતર
        {
          જો (zero_padding)
            zero_padding_width = MIN (pad, INT_MAX);
          નહીંતર
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  જો (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  જો (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      જો (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  જો (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  માટે (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    જો (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  જો (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  જો (fmt[suffix_pos] != '\0')
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
સ્થિર ગણના simple_strtod_error
parse_human_સંખ્યા (અક્ષર અચલ *str, long દ્વિગુણ /*output */ *કિંમત,
                    size_t *precision)
{
  અક્ષર *ptr = ખાલી;

  ગણના simple_strtod_error e =
    simple_strtod_human (str, &ptr, કિંમત, precision, scale_from);
  જો (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      પાછા e;
    }

  જો (ptr && *ptr != '\0')
    {
      જો (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  પાછા e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
સ્થિર bool
prepare_padded_સંખ્યા (અચલ long દ્વિગુણ val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  પૂર્ણાંક x;
  expld (val, 10, &x);

  જો (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      જો (inval_style != inval_ignore)
        {
          જો (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          નહીંતર
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      પાછા false;
    }

  જો (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      જો (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      પાછા false;
    }

  જ્યારે (true)
    {
      પૂર્ણાંક numlen = double_to_human (val, precision_used,
                                    padding_buffer, padding_buffer_size,
                                    scale_to, grouping, round_style);
      ptrdiff_t growth;
      જો (numlen < 0 || ckd_sub (&growth, numlen, padding_buffer_size - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      જો (growth <= 0)
        તોડો;
      padding_buffer = xpalloc (padding_buffer, &padding_buffer_size,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_buffer));

  intmax_t pad = 0;
  જો (padding_width)
    {
      પૂર્ણાંક buf_width = mbswidth (padding_buffer,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      જો (0 <= buf_width)
        {
          જો (padding_width < 0)
            {
              જો (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          નહીંતર
            {
              જો (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  પાછા true;
}

સ્થિર શૂન્ય
print_padded_સંખ્યા (intmax_t padding)
{
  જો (format_str_prefix)
    fputs (format_str_prefix, stdout);

  માટે (intmax_t p = padding; 0 < p; p--)
    putchar (' ');

  fputs (padding_buffer, stdout);

  માટે (intmax_t p = padding; p < 0; p++)
    putchar (' ');

  જો (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
સ્થિર bool
process_suffixed_સંખ્યા (અક્ષર *text, long દ્વિગુણ *result,
                         size_t *precision, long પૂર્ણાંક field)
{
  અક્ષર saved_suffix = '\0';

  જો (suffix)
    {
      જો (mbs_endswith (text, suffix))
        {
          saved_suffix = *(text + strlen (text) - strlen (suffix));
          *(text + strlen (text) - strlen (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      નહીંતર
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  અક્ષર *p = skip_str_matching (text, newline_or_blank, true);

  /* setup auto-padding.  */
  જો (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      જો (padding_width < 0)
        padding_width = strlen (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  long દ્વિગુણ val = 0;
  ગણના simple_strtod_error e = parse_human_સંખ્યા (p, &val, precision);
  જો (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  જો (from_unit_size != 1 || to_unit_size != 1)
    val = (val * from_unit_size) / to_unit_size;

  *result = val;

  જો (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    પાછા true;
  નહીંતર
    {
      જો (saved_suffix)
        *(text + strlen (text)) = saved_suffix;
      પાછા false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
સ્થિર અક્ષર *
mbsmbchr (અક્ષર અચલ *s, અક્ષર અચલ *c)
{
  unsigned અક્ષર uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  જો (uc < 0x30 || MB_CUR_MAX == 1)
    પાછા (અક્ષર *) strchr (s, uc);
  નહીંતર જો (is_utf8_charset ())
    પાછા (અક્ષર *) (uc < 0x80 ? strchr (s, uc) : strstr (s, c));
  નહીંતર
    પાછા *(c + 1) == '\0' ? mbschr (s, uc) : (અક્ષર *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
સ્થિર અક્ષર *
next_field (અક્ષર **લીટી)
{
  અક્ષર *field_start = *લીટી;
  અક્ષર *field_end   = field_start;

  જો (delimiter)
    {
      જો (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = strchr (field_start, '\0');
    }
  નહીંતર
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *લીટી = field_end;
  પાછા field_start;
}

ATTRIBUTE_PURE
સ્થિર bool
include_field (uintmax_t field)
{
  સંરચના field_range_pair *p = frp;
  જો (!p)
    પાછા field == 1;

  જ્યારે (p->lo != UINTMAX_MAX)
    {
      જો (p->lo <= field && p->hi >= field)
        પાછા true;
      ++p;
    }
  પાછા false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
સ્થિર bool
process_field (અક્ષર *text, uintmax_t field)
{
  long દ્વિગુણ val = 0;
  size_t precision = 0;
  bool valid_સંખ્યા = true;

  જો (include_field (field))
    {
      valid_સંખ્યા =
        process_suffixed_સંખ્યા (text, &val, &precision, field);

      intmax_t padding;
      જો (valid_સંખ્યા)
        valid_સંખ્યા = prepare_padded_સંખ્યા (val, precision, &padding);

      જો (valid_સંખ્યા)
        print_padded_સંખ્યા (padding);
      નહીંતર
        fputs (text, stdout);
    }
  નહીંતર
    fputs (text, stdout);

  પાછા valid_સંખ્યા;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
સ્થિર પૂર્ણાંક
process_લીટી (અક્ષર *લીટી, bool newline)
{
  અક્ષર *next;
  uintmax_t field = 0;
  bool valid_સંખ્યા = true;

  જ્યારે (true) {
    ++field;
    next = next_field (&લીટી);

    જો (*લીટી != '\0')
      {
        /* NUL terminate the current field string and process */
        અક્ષર end_field = *લીટી;
        *લીટી = '\0';

        જો (! process_field (next, field))
          valid_સંખ્યા = false;

        જો (delimiter != ખાલી)
          fputs (delimiter, stdout);
        નહીંતર
          fputc (' ', stdout);

        જો (delimiter)
          લીટી += MAX (strlen (delimiter), 1);
        નહીંતર
          {
            *લીટી = end_field;
            mcel_t g = mcel_scanz (લીટી);
            લીટી += g.len;
          }
      }
    નહીંતર
      {
        /* end of the line, process the last field and finish */
        જો (! process_field (next, field))
          valid_સંખ્યા = false;

        તોડો;
      }
  }

  જો (newline)
    putchar (લીટી_delim);

 જો (ferror (stdout))
   write_error ();

  પાછા valid_સંખ્યા;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#જો HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#endif

  decimal_point = nl_langinfo (RADIXCHAR);
  જો (decimal_point == ખાલી || strlen (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_length = strlen (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  જો (thousands_sep == ખાલી)
    thousands_sep = "";
  thousands_sep_length = strlen (thousands_sep);

  atexit (close_stdout);

  જ્યારે (true)
    {
      પૂર્ણાંક c = getopt_long (argc, argv, "d:z", longopts, ખાલી);

      જો (c == -1)
        તોડો;

      પસંદગી (c)
        {
        સ્થિતિ FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          તોડો;

        સ્થિતિ FROM_UNIT_OPTION:
          from_unit_size = unit_to_umax (optarg);
          તોડો;

        સ્થિતિ TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          તોડો;

        સ્થિતિ TO_UNIT_OPTION:
          to_unit_size = unit_to_umax (optarg);
          તોડો;

        સ્થિતિ ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          તોડો;

        સ્થિતિ GROUPING_OPTION:
          grouping = 1;
          તોડો;

        સ્થિતિ PADDING_OPTION:
          જો (((xstrtoimax (optarg, ખાલી, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          તોડો;

        સ્થિતિ FIELD_OPTION:
          જો (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          તોડો;

        સ્થિતિ 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          જો (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              જો (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          તોડો;

        સ્થિતિ 'z':
          લીટી_delim = '\0';
          તોડો;

        સ્થિતિ SUFFIX_OPTION:
          suffix = optarg;
          તોડો;

        સ્થિતિ UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          તોડો;

        સ્થિતિ DEBUG_OPTION:
          debug = true;
          તોડો;

        સ્થિતિ DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          તોડો;

        સ્થિતિ HEADER_OPTION:
          જો (optarg)
            {
              જો (xstrtoumax (optarg, ખાલી, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          નહીંતર
            {
              header = 1;
            }
          તોડો;

        સ્થિતિ FORMAT_OPTION:
          format_str = optarg;
          તોડો;

        સ્થિતિ INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          તોડો;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (format_str != ખાલી && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  જો (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  જો (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == ખાલી))
    error (0, 0, _("no conversion option specified"));

  જો (debug && unit_separator && delimiter == ખાલી)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  જો (format_str)
    parse_format_string (format_str);

  જો (grouping)
    {
      જો (scale_to != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      જો (debug && thousands_sep_length == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == ખાલી);

  જો (inval_style != inval_abort)
    conv_exit_code = 0;

  જો (argc > optind)
    {
      જો (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      માટે (; optind < argc; optind++)
        valid_numbers &= process_લીટી (argv[optind], true);
    }
  નહીંતર
    {
      અક્ષર *લીટી = ખાલી;
      size_t લીટી_allocated = 0;
      ssize_t len;

      જ્યારે (header-- && getdelim (&લીટી, &લીટી_allocated,
                                   લીટી_delim, stdin) > 0)
        {
          જો (fputs (લીટી, stdout) == EOF)
            write_error ();
        }

      જ્યારે ((len = getdelim (&લીટી, &લીટી_allocated,
                              લીટી_delim, stdin)) > 0)
        {
          bool newline = લીટી[len - 1] == લીટી_delim;
          જો (newline)
            લીટી[len - 1] = '\0';
          valid_numbers &= process_લીટી (લીટી, newline);
        }

      જો (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  જો (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  પૂર્ણાંક exit_status = EXIT_SUCCESS;
  જો (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
