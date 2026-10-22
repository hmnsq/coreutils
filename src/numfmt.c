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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <float.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <langinfo.h>

#ଅନ୍ତର୍ଭାଵ "argmatch.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "mbswidth.h"
#ଅନ୍ତର୍ଭାଵ "mcel.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "skipchars.h"
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"

#ଅନ୍ତର୍ଭାଵ "set-fields.h"

#ଯଦି HAVE_FPSETPREC
# include <ieeefp.h>
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "numfmt"

#define AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
ଗଣନା { EXIT_CONVERSION_WARNINGS = 2 };

ଗଣନା
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

ଗଣନା scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ scale_from_args[] =
{
  "none", "auto", "si", "iec", "iec-i", ଖାଲି
};

ସ୍ଥିର ଗଣନା scale_type ଅଚଳ scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ scale_to_args[] =
{
  "none", "si", "iec", "iec-i", ଖାଲି
};

ସ୍ଥିର ଗଣନା scale_type ଅଚଳ scale_to_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


ଗଣନା round_type
{
  round_ceiling,
  round_floor,
  round_from_zero,
  round_to_zero,
  round_nearest,
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ round_args[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", ଖାଲି
};

ସ୍ଥିର ଗଣନା round_type ଅଚଳ round_types[] =
{
  round_ceiling, round_floor, round_from_zero, round_to_zero, round_nearest
};


ଗଣନା inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ inval_args[] =
{
  "abort", "fail", "warn", "ignore", ଖାଲି
};

ସ୍ଥିର ଗଣନା inval_type ଅଚଳ inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"from", required_argument, ଖାଲି, FROM_OPTION},
  {"from-unit", required_argument, ଖାଲି, FROM_UNIT_OPTION},
  {"to", required_argument, ଖାଲି, TO_OPTION},
  {"to-unit", required_argument, ଖାଲି, TO_UNIT_OPTION},
  {"round", required_argument, ଖାଲି, ROUND_OPTION},
  {"padding", required_argument, ଖାଲି, PADDING_OPTION},
  {"suffix", required_argument, ଖାଲି, SUFFIX_OPTION},
  {"unit-separator", required_argument, ଖାଲି, UNIT_SEPARATOR_OPTION},
  {"grouping", no_argument, ଖାଲି, GROUPING_OPTION},
  {"delimiter", required_argument, ଖାଲି, 'd'},
  {"field", required_argument, ଖାଲି, FIELD_OPTION},
  {"debug", no_argument, ଖାଲି, DEBUG_OPTION},
  {"-debug", no_argument, ଖାଲି, DEV_DEBUG_OPTION},
  {"header", optional_argument, ଖାଲି, HEADER_OPTION},
  {"format", required_argument, ଖାଲି, FORMAT_OPTION},
  {"invalid", required_argument, ଖାଲି, INVALID_OPTION},
  {"zero-terminated", no_argument, ଖାଲି, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
ଗଣନା { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
ଗଣନା { MAX_ACCEPTABLE_DIGITS = 33 };

ସ୍ଥିର ଗଣନା scale_type scale_from = scale_none;
ସ୍ଥିର ଗଣନା scale_type scale_to = scale_none;
ସ୍ଥିର ଗଣନା round_type round_style = round_from_zero;
ସ୍ଥିର ଗଣନା inval_type inval_style = inval_abort;
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *suffix = ଖାଲି;
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *unit_separator = ଖାଲି;
ସ୍ଥିର uintmax_t from_unit_size = 1;
ସ୍ଥିର uintmax_t to_unit_size = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grouping = 0;
ସ୍ଥିର ଅକ୍ଷର *padding_buffer = ଖାଲି;
ସ୍ଥିର idx_t padding_buffer_size = 0;
ସ୍ଥିର intmax_t padding_width = 0;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା zero_padding_width = 0;
ସ୍ଥିର long ପୂର୍ଣ୍ଣସଂଖ୍ୟା user_precision = -1;
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *format_str = ଖାଲି;
ସ୍ଥିର ଅକ୍ଷର *format_str_prefix = ଖାଲି;
ସ୍ଥିର ଅକ୍ଷର *format_str_suffix = ଖାଲି;

/* By default, any conversion error will terminate the program.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା conv_exit_code = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *delimiter = ଖାଲି;

/* line delimiter.  */
ସ୍ଥିର unsigned ଅକ୍ଷର ଧାଡ଼ି_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
ସ୍ଥିର uintmax_t header = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
ସ୍ଥିର bool debug;

/* will be set according to the current locale.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *decimal_point;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା decimal_point_length;
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *thousands_sep;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା thousands_sep_length;

/* debugging for developers.  Enables devmsg().  */
ସ୍ଥିର bool dev_debug = false;

ସ୍ଥିର bool
newline_or_blank (mcel_t g)
{
  ଫେରନ୍ତୁ g.ch == '\n' || c32issep (g.ch);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
default_scale_base (ଗଣନା scale_type scale)
{
  ବାଛ (scale)
    {
    ସ୍ଥିତି scale_IEC:
    ସ୍ଥିତି scale_IEC_I:
      ଫେରନ୍ତୁ 1024;

    ସ୍ଥିତି scale_none:
    ସ୍ଥିତି scale_auto:
    ସ୍ଥିତି scale_SI:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ 1000;
    }
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *valid_suffixes = 1 + zero_and_valid_suffixes;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
valid_suffix (ଅଚଳ ଅକ୍ଷର suf)
{
  ଫେରନ୍ତୁ strchr (valid_suffixes, suf) != ଖାଲି;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
suffix_power (ଅଚଳ ଅକ୍ଷର suf)
{
  ବାଛ (suf)
    {
    ସ୍ଥିତି 'k':                  /* kilo.  */
    ସ୍ଥିତି 'K':                  /* kilo or kibi.  */
      ଫେରନ୍ତୁ 1;

    ସ୍ଥିତି 'M':                  /* mega or mebi.  */
      ଫେରନ୍ତୁ 2;

    ସ୍ଥିତି 'G':                  /* giga or gibi.  */
      ଫେରନ୍ତୁ 3;

    ସ୍ଥିତି 'T':                  /* tera or tebi.  */
      ଫେରନ୍ତୁ 4;

    ସ୍ଥିତି 'P':                  /* peta or pebi.  */
      ଫେରନ୍ତୁ 5;

    ସ୍ଥିତି 'E':                  /* exa or exbi.  */
      ଫେରନ୍ତୁ 6;

    ସ୍ଥିତି 'Z':                  /* zetta or 2**70.  */
      ଫେରନ୍ତୁ 7;

    ସ୍ଥିତି 'Y':                  /* yotta or 2**80.  */
      ଫେରନ୍ତୁ 8;

    ସ୍ଥିତି 'R':                  /* ronna or 2**90.  */
      ଫେରନ୍ତୁ 9;

    ସ୍ଥିତି 'Q':                  /* quetta or 2**100.  */
      ଫେରନ୍ତୁ 10;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                   /* should never happen. assert?  */
      ଫେରନ୍ତୁ 0;
    }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅକ୍ଷର ଅଚଳ *
suffix_power_char (ପୂର୍ଣ୍ଣସଂଖ୍ୟା power)
{
  ବାଛ (power)
    {
    ସ୍ଥିତି 0:
      ଫେରନ୍ତୁ "";

    ସ୍ଥିତି 1:
      ଫେରନ୍ତୁ "K";

    ସ୍ଥିତି 2:
      ଫେରନ୍ତୁ "M";

    ସ୍ଥିତି 3:
      ଫେରନ୍ତୁ "G";

    ସ୍ଥିତି 4:
      ଫେରନ୍ତୁ "T";

    ସ୍ଥିତି 5:
      ଫେରନ୍ତୁ "P";

    ସ୍ଥିତି 6:
      ଫେରନ୍ତୁ "E";

    ସ୍ଥିତି 7:
      ଫେରନ୍ତୁ "Z";

    ସ୍ଥିତି 8:
      ଫେରନ୍ତୁ "Y";

    ସ୍ଥିତି 9:
      ଫେରନ୍ତୁ "R";

    ସ୍ଥିତି 10:
      ଫେରନ୍ତୁ "Q";

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
ସ୍ଥିର long ଦ୍ୱିଗୁଣ
powerld (long ଦ୍ୱିଗୁଣ base, ptrdiff_t x)
{
  long ଦ୍ୱିଗୁଣ result = base;
  ଯଦି (x == 0)
    ଫେରନ୍ତୁ 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  ଯେତେବେଳେ (--x)
    result *= base;
  ଫେରନ୍ତୁ result;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ long ଦ୍ୱିଗୁଣ
absld (long ଦ୍ୱିଗୁଣ val)
{
  ଫେରନ୍ତୁ val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
ସ୍ଥିର long ଦ୍ୱିଗୁଣ
expld (long ଦ୍ୱିଗୁଣ val, ପୂର୍ଣ୍ଣସଂଖ୍ୟା base, ପୂର୍ଣ୍ଣସଂଖ୍ୟା /*output */ *x)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା power = 0;

  ଯଦି (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      ଯେତେବେଳେ (absld (val) >= base)
        {
          ++power;
          val /= base;
        }
    }
  ଯଦି (x)
    *x = power;
  ଫେରନ୍ତୁ val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ intmax_t
simple_round_ceiling (long ଦ୍ୱିଗୁଣ val)
{
  intmax_t intval = val;
  ଯଦି (intval < val)
    intval++;
  ଫେରନ୍ତୁ intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ intmax_t
simple_round_floor (long ଦ୍ୱିଗୁଣ val)
{
  ଫେରନ୍ତୁ -simple_round_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ intmax_t
simple_round_from_zero (long ଦ୍ୱିଗୁଣ val)
{
  ଫେରନ୍ତୁ val < 0 ? simple_round_floor (val) : simple_round_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ intmax_t
simple_round_to_zero (long ଦ୍ୱିଗୁଣ val)
{
  ଫେରନ୍ତୁ val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ intmax_t
simple_round_nearest (long ଦ୍ୱିଗୁଣ val)
{
  ଫେରନ୍ତୁ val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
ସ୍ଥିର ଧାଡ଼ିଭିତରେ long ଦ୍ୱିଗୁଣ
simple_round (long ଦ୍ୱିଗୁଣ val, ଗଣନା round_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (long ଦ୍ୱିଗୁଣ) INTMAX_MAX * intmax_mul;

  ବାଛ (t)
    {
    ସ୍ଥିତି round_ceiling:
      rval = simple_round_ceiling (val);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି round_floor:
      rval = simple_round_floor (val);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି round_from_zero:
      rval = simple_round_from_zero (val);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି round_to_zero:
      rval = simple_round_to_zero (val);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି round_nearest:
      rval = simple_round_nearest (val);
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      /* to silence the compiler - this should never happen.  */
      ଫେରନ୍ତୁ 0;
    }

  ଫେରନ୍ତୁ (long ଦ୍ୱିଗୁଣ) INTMAX_MAX * intmax_mul + rval;
}

ଗଣନା simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_OVERFLOW,
  SSE_INVALID_ସଂଖ୍ୟା,

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
ସ୍ଥିର ଗଣନା simple_strtod_error
simple_strtod_int (ଅକ୍ଷର ଅଚଳ *ନିବେଶ_str,
                   ଅକ୍ଷର **endptr, long ଦ୍ୱିଗୁଣ *ମୂଲ୍ୟ, bool *negative)
{
  ଗଣନା simple_strtod_error e = SSE_OK;

  long ଦ୍ୱିଗୁଣ val = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା digits = 0;
  bool found_digit = false;

  ଯଦି (*ନିବେଶ_str == '-')
    {
      ନିବେଶ_str++;
      *negative = true;
    }
  ଅନ୍ୟଥା
    *negative = false;

  *endptr = (ଅକ୍ଷର *) ନିବେଶ_str;
  ଯେତେବେଳେ (c_isdigit (**endptr))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା digit = (**endptr) - '0';

      found_digit = true;

      ଯଦି (val || digit)
        digits++;

      ଯଦି (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      ଯଦି (digits > MAX_ACCEPTABLE_DIGITS)
        ଫେରନ୍ତୁ SSE_OVERFLOW;

      val *= 10;
      val += digit;

      ++(*endptr);

      ଯଦି (thousands_sep_length > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_length)
          && c_isdigit ((*endptr)[thousands_sep_length]))
        (*endptr) += thousands_sep_length;
    }
  ଯଦି (! found_digit
      && ! STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    ଫେରନ୍ତୁ SSE_INVALID_ସଂଖ୍ୟା;
  ଯଦି (*negative)
    val = -val;

  ଯଦି (ମୂଲ୍ୟ)
    *ମୂଲ୍ୟ = val;

  ଫେରନ୍ତୁ e;
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
ସ୍ଥିର ଗଣନା simple_strtod_error
simple_strtod_float (ଅକ୍ଷର ଅଚଳ *ନିବେଶ_str,
                     ଅକ୍ଷର **endptr,
                     long ଦ୍ୱିଗୁଣ *ମୂଲ୍ୟ,
                     size_t *precision)
{
  bool negative;
  ଗଣନା simple_strtod_error e = SSE_OK;

  ଯଦି (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (ନିବେଶ_str, endptr, ମୂଲ୍ୟ, &negative);
  ଯଦି (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ଫେରନ୍ତୁ e;

  /* optional decimal point + fraction.  */
  ଯଦି (STREQ_LEN (*endptr, decimal_point, decimal_point_length))
    {
      ଅକ୍ଷର *ptr2;
      long ଦ୍ୱିଗୁଣ val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_point_length;
      ଗଣନା simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      ଯଦି (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        ଫେରନ୍ତୁ e2;
      ଯଦି (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      ଯଦି (neg_frac)
        ଫେରନ୍ତୁ SSE_INVALID_ସଂଖ୍ୟା;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      ଯଦି (ମୂଲ୍ୟ)
        {
          ଯଦି (negative)
            *ମୂଲ୍ୟ -= val_frac;
          ଅନ୍ୟଥା
            *ମୂଲ୍ୟ += val_frac;
        }

      ଯଦି (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  ଫେରନ୍ତୁ e;
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
ସ୍ଥିର ଗଣନା simple_strtod_error
simple_strtod_human (ଅକ୍ଷର ଅଚଳ *ନିବେଶ_str,
                     ଅକ୍ଷର **endptr, long ଦ୍ୱିଗୁଣ *ମୂଲ୍ୟ, size_t *precision,
                     ଗଣନା scale_type allowed_scaling)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା power = 0;
  /* 'scale_auto' is checked below.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା scale_base = default_scale_base (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, ନିବେଶ_str),
          quote_n (1, decimal_point),
          MAX_UNSCALED_DIGITS);

  ଗଣନା simple_strtod_error e =
    simple_strtod_float (ନିବେଶ_str, endptr, ମୂଲ୍ୟ, precision);
  ଯଦି (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ଫେରନ୍ତୁ e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *ମୂଲ୍ୟ, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)*precision);

  ଯେତେବେଳେ (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      ଯଦି (unit_separator)
        {
          size_t sep_len = strlen (unit_separator);
          ଯଦି (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      ଯଦି (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          ଯଦି (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      ଯଦି (**endptr == '\0')
        ଭାଙ୍ଗ;  /* Treat as no suffix.  */

      ଯଦି (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          ଯଦି (**endptr == '\0')
            ଭାଙ୍ଗ;

          ଫେରନ୍ତୁ SSE_INVALID_SUFFIX;
        }

      ଯଦି (allowed_scaling == scale_none)
        ଫେରନ୍ତୁ SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      ଯଦି (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_base = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_base);
        }
      ଅନ୍ୟଥା ଯଦି (allowed_scaling == scale_IEC_I)
        {
          ଯଦି (**endptr == 'i')
            (*endptr)++;
          ଅନ୍ୟଥା
            ଫେରନ୍ତୁ SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      ଭାଙ୍ଗ;
    }

  long ଦ୍ୱିଗୁଣ multiplier = powerld (scale_base, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_base, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*ମୂଲ୍ୟ) = (*ମୂଲ୍ୟ) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *ମୂଲ୍ୟ, *ମୂଲ୍ୟ);

  ଫେରନ୍ତୁ e;
}


ସ୍ଥିର ଶୂନ୍ୟ
simple_strtod_fatal (ଗଣନା simple_strtod_error err, ଅକ୍ଷର ଅଚଳ *ନିବେଶ_str)
{
  ଅକ୍ଷର ଅଚଳ *msgid = ଖାଲି;

  ବାଛ (err)
    {
    ସ୍ଥିତି SSE_OVERFLOW:
      msgid = N_("value too large to be converted: %s");
      ଭାଙ୍ଗ;

    ସ୍ଥିତି SSE_INVALID_ସଂଖ୍ୟା:
      msgid = N_("invalid number: %s");
      ଭାଙ୍ଗ;

    ସ୍ଥିତି SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      ଭାଙ୍ଗ;

    ସ୍ଥିତି SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      ଭାଙ୍ଗ;

    ସ୍ଥିତି SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      ଭାଙ୍ଗ;

    ସ୍ଥିତି SSE_OK_PRECISION_LOSS:
    ସ୍ଥିତି SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  ଯଦି (inval_style != inval_ignore)
    error (conv_exit_code, 0, gettext (msgid), quote (ନିବେଶ_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
double_to_human (long ଦ୍ୱିଗୁଣ val, ପୂର୍ଣ୍ଣସଂଖ୍ୟା precision,
                 ଅକ୍ଷର *buf, idx_t buf_size,
                 ଗଣନା scale_type scale, ପୂର୍ଣ୍ଣସଂଖ୍ୟା group, ଗଣନା round_type round)
{
  ଅକ୍ଷର fmt[ଆକାର "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_width)];
  ଅକ୍ଷର *pfmt = fmt;
  *pfmt++ = '%';

  ଯଦି (group)
    *pfmt++ = '\'';

  ଯଦି (zero_padding_width)
    pfmt += sprintf (pfmt, "0%d", zero_padding_width);

  devmsg ("double_to_human:\n");

  ଯଦି (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_round (val, round);
      val /= powerld (10, precision);

      devmsg ((group) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      ଫେରନ୍ତୁ snprintf (buf, buf_size, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  ଦ୍ୱିଗୁଣ scale_base = default_scale_base (scale);

  /* Normalize val to scale. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା power = 0;
  val = expld (val, scale_base, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_base, power);

  /* Perform rounding. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା power_adjust = 0;
  ଯଦି (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  ଅନ୍ୟଥା ଯଦି (absld (val) < 10)
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
  ଯଦି (absld (val) >= scale_base)
    {
      val /= scale_base;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_decimal_point = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_base, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା prec = user_precision == -1 ? show_decimal_point : user_precision;

  ଫେରନ୍ତୁ snprintf (buf, buf_size, fmt, prec, val,
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
ସ୍ଥିର uintmax_t
unit_to_umax (ଅକ୍ଷର ଅଚଳ *n_string)
{
  strtol_error s_err;
  ଅକ୍ଷର ଅଚଳ *c_string = n_string;
  ଅକ୍ଷର *t_string = ଖାଲି;
  size_t n_len = strlen (n_string);
  ଅକ୍ଷର *end = ଖାଲି;
  uintmax_t n;
  ଅକ୍ଷର ଅଚଳ *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  ଯଦି (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      end = t_string + n_len - 1;
      memcpy (t_string, n_string, n_len);

      ଯଦି (*end == 'i' && 2 <= n_len && ! c_isdigit (*(end - 1)))
        *end = '\0';
      ଅନ୍ୟଥା
        {
          *++end = 'B';
          *++end = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &end, 10, &n, suffixes);

  ଯଦି (s_err != LONGINT_OK || *end || n == 0)
    {
      ମୁକ୍ତ (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  ମୁକ୍ତ (t_string);

  ଫେରନ୍ତୁ n;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [ସଂଖ୍ୟା_2]...\n\
"), program_name);
      fputs (_("\
Reformat ସଂଖ୍ୟା_2(s), or the numbers from standard ନିବେଶ ଯଦି none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid ନିବେଶ\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead of whitespace ପାଇଁ field delimiter\n\
"));
      oputs (_("\
      --field=FIELDS\n\
         replace the numbers in these ନିବେଶ fields (ପୂର୍ବନିର୍ଦ୍ଧାରିତ=1);\n\
         see FIELDS below ପାଇଁ details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use printf style floating-point FORMAT;\n\
         see FORMAT below ପାଇଁ details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         auto-scale ନିବେଶ numbers to UNITs; ପୂର୍ବନିର୍ଦ୍ଧାରିତ is 'none';\n\
         see UNIT below ପାଇଁ details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the ନିବେଶ unit size (instead of the ପୂର୍ବନିର୍ଦ୍ଧାରିତ 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping of digits, e.g. 1,000,000.\n\
         This has no effect in the C/POSIX locale\n\
"));
      oputs (_("\
      --header[=N]\n\
         print (without converting) the first N header ଧାଡ଼ିଗୁଡ଼ିକ;\n\
         N defaults to 1 ଯଦି not specified\n\
"));
      oputs (_("\
      --invalid=MODE\n\
         failure mode ପାଇଁ invalid numbers;\n\
         MODE can be: abort (ପୂର୍ବନିର୍ଦ୍ଧାରିତ), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output to N characters;\n\
         positive N will ଡାହାଣ-align, negative N will ବାମ-align;\n\
         padding is ignored ଯଦି the output is wider than N;\n\
         the ପୂର୍ବନିର୍ଦ୍ଧାରିତ is to automatically pad ଯଦି a whitespace is found\n\
"));
      oputs (_("\
      --round=METHOD\n\
         use METHOD ପାଇଁ rounding when scaling; METHOD can be:\n\
         up, down, from-zero (ପୂର୍ବନିର୍ଦ୍ଧାରିତ), towards-zero, nearest\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         add SUFFIX to output numbers,\n\
         and accept an optional SUFFIX in ନିବେଶ numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         ଭର୍ତ୍ତି SEP between ସଂଖ୍ୟା and unit on output,\n\
         and accept an optional SEP in ନିବେଶ numbers\n\
"));
      oputs (_("\
      --to=UNIT\n\
         auto-scale output numbers to UNITs; see UNIT below\n\
"));
      oputs (_("\
      --to-unit=N\n\
         the output unit size (instead of the ପୂର୍ବନିର୍ଦ୍ଧାରିତ 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ଧାଡ଼ି delimiter is NUL, not newline\n\
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
  N-   from N'th field, to end of ଧାଡ଼ି\n\
  N-M  from N'th to M'th field (inclusive)\n\
  -M   from first to M'th field (inclusive)\n\
  -    all fields\n\
Multiple fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable ପାଇଁ printing one floating-point argument '%f'.\n\
Optional quote (%'f) will enable --grouping (ଯଦି supported by current locale).\n\
Optional width ମୂଲ୍ୟ (%10f) will pad output. Optional zero (%010f) width\n\
will zero pad the ସଂଖ୍ୟା. Optional negative ମୂଲ୍ୟଗୁଡ଼ିକ (%-10f) will ବାମ align.\n\
Optional precision (%.1f) will override the ନିବେଶ determined precision.\n\
"), stdout);

      printf (_("\n\
Exit status is 0 ଯଦି all ନିବେଶ numbers were successfully converted.\n\
By ପୂର୍ବନିର୍ଦ୍ଧାରିତ, %s will stop at the first conversion error with exit status 2.\n\
With --invalid='fail' a warning is printed ପାଇଁ each conversion error\n\
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
ସ୍ଥିର ଶୂନ୍ୟ
parse_format_string (ଅକ୍ଷର ଅଚଳ *fmt)
{
  size_t i;
  size_t prefix_len = 0;
  size_t suffix_pos;
  ଅକ୍ଷର *endptr = ଖାଲି;
  bool zero_padding = false;

  ପାଇଁ (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      ଯଦି (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  ଯେତେବେଳେ (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      ଯଦି (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      ଅନ୍ୟଥା ଯଦି (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      ଅନ୍ୟଥା ଯଦି (! skip)
        ଭାଙ୍ଗ;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  ଯଦି (pad != 0)
    {
      ଯଦି (debug && padding_width && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      ଯଦି (pad < 0)
        padding_width = pad;
      ଅନ୍ୟଥା
        {
          ଯଦି (zero_padding)
            zero_padding_width = MIN (pad, INT_MAX);
          ଅନ୍ୟଥା
            padding_width = pad;
        }
    }
  i = endptr - fmt;

  ଯଦି (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  ଯଦି (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      ଯଦି (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  ଯଦି (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  ପାଇଁ (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    ଯଦି (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  ଯଦି (prefix_len)
    format_str_prefix = ximemdup0 (fmt, prefix_len);
  ଯଦି (fmt[suffix_pos] != '\0')
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
ସ୍ଥିର ଗଣନା simple_strtod_error
parse_human_ସଂଖ୍ୟା (ଅକ୍ଷର ଅଚଳ *str, long ଦ୍ୱିଗୁଣ /*output */ *ମୂଲ୍ୟ,
                    size_t *precision)
{
  ଅକ୍ଷର *ptr = ଖାଲି;

  ଗଣନା simple_strtod_error e =
    simple_strtod_human (str, &ptr, ମୂଲ୍ୟ, precision, scale_from);
  ଯଦି (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      ଫେରନ୍ତୁ e;
    }

  ଯଦି (ptr && *ptr != '\0')
    {
      ଯଦି (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  ଫେରନ୍ତୁ e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
ସ୍ଥିର bool
prepare_padded_ସଂଖ୍ୟା (ଅଚଳ long ଦ୍ୱିଗୁଣ val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_used = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା x;
  expld (val, 10, &x);

  ଯଦି (scale_to == scale_none
      && x + precision_used > MAX_UNSCALED_DIGITS)
    {
      ଯଦି (inval_style != inval_ignore)
        {
          ଯଦି (precision_used)
            error (conv_exit_code, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_used);
          ଅନ୍ୟଥା
            error (conv_exit_code, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      ଫେରନ୍ତୁ false;
    }

  ଯଦି (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      ଯଦି (inval_style != inval_ignore)
        error (conv_exit_code, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      ଫେରନ୍ତୁ false;
    }

  ଯେତେବେଳେ (true)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା numlen = double_to_human (val, precision_used,
                                    padding_buffer, padding_buffer_size,
                                    scale_to, grouping, round_style);
      ptrdiff_t growth;
      ଯଦି (numlen < 0 || ckd_sub (&growth, numlen, padding_buffer_size - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      ଯଦି (growth <= 0)
        ଭାଙ୍ଗ;
      padding_buffer = xpalloc (padding_buffer, &padding_buffer_size,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_buffer));

  intmax_t pad = 0;
  ଯଦି (padding_width)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା buf_width = mbswidth (padding_buffer,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      ଯଦି (0 <= buf_width)
        {
          ଯଦି (padding_width < 0)
            {
              ଯଦି (padding_width < -buf_width)
                pad = padding_width + buf_width;
            }
          ଅନ୍ୟଥା
            {
              ଯଦି (buf_width < padding_width)
                pad = padding_width - buf_width;
            }
        }
    }

  *padding = pad;
  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ
print_padded_ସଂଖ୍ୟା (intmax_t padding)
{
  ଯଦି (format_str_prefix)
    fputs (format_str_prefix, stdout);

  ପାଇଁ (intmax_t p = padding; 0 < p; p--)
    putchar (' ');

  fputs (padding_buffer, stdout);

  ପାଇଁ (intmax_t p = padding; p < 0; p++)
    putchar (' ');

  ଯଦି (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
ସ୍ଥିର bool
process_suffixed_ସଂଖ୍ୟା (ଅକ୍ଷର *text, long ଦ୍ୱିଗୁଣ *result,
                         size_t *precision, long ପୂର୍ଣ୍ଣସଂଖ୍ୟା field)
{
  ଅକ୍ଷର saved_suffix = '\0';

  ଯଦି (suffix)
    {
      ଯଦି (mbs_endswith (text, suffix))
        {
          saved_suffix = *(text + strlen (text) - strlen (suffix));
          *(text + strlen (text) - strlen (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      ଅନ୍ୟଥା
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  ଅକ୍ଷର *p = skip_str_matching (text, newline_or_blank, true);

  /* setup auto-padding.  */
  ଯଦି (auto_padding)
    {
      padding_width = text < p || 1 < field
                      ? mbswidth (text,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      ଯଦି (padding_width < 0)
        padding_width = strlen (text);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_width);
    }

  long ଦ୍ୱିଗୁଣ val = 0;
  ଗଣନା simple_strtod_error e = parse_human_ସଂଖ୍ୟା (p, &val, precision);
  ଯଦି (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  ଯଦି (from_unit_size != 1 || to_unit_size != 1)
    val = (val * from_unit_size) / to_unit_size;

  *result = val;

  ଯଦି (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    ଫେରନ୍ତୁ true;
  ଅନ୍ୟଥା
    {
      ଯଦି (saved_suffix)
        *(text + strlen (text)) = saved_suffix;
      ଫେରନ୍ତୁ false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_PURE
ସ୍ଥିର ଅକ୍ଷର *
mbsmbchr (ଅକ୍ଷର ଅଚଳ *s, ଅକ୍ଷର ଅଚଳ *c)
{
  unsigned ଅକ୍ଷର uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  ଯଦି (uc < 0x30 || MB_CUR_MAX == 1)
    ଫେରନ୍ତୁ (ଅକ୍ଷର *) strchr (s, uc);
  ଅନ୍ୟଥା ଯଦି (is_utf8_charset ())
    ଫେରନ୍ତୁ (ଅକ୍ଷର *) (uc < 0x80 ? strchr (s, uc) : strstr (s, c));
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ *(c + 1) == '\0' ? mbschr (s, uc) : (ଅକ୍ଷର *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
ସ୍ଥିର ଅକ୍ଷର *
next_field (ଅକ୍ଷର **ଧାଡ଼ି)
{
  ଅକ୍ଷର *field_start = *ଧାଡ଼ି;
  ଅକ୍ଷର *field_end   = field_start;

  ଯଦି (delimiter)
    {
      ଯଦି (! (field_end = mbsmbchr (field_start, delimiter)))
        field_end = strchr (field_start, '\0');
    }
  ଅନ୍ୟଥା
    {
      /* keep any space prefix in the returned field */
      field_end = skip_str_matching (field_end, newline_or_blank, true);
      field_end = skip_str_matching (field_end, newline_or_blank, false);
    }

  *ଧାଡ଼ି = field_end;
  ଫେରନ୍ତୁ field_start;
}

ATTRIBUTE_PURE
ସ୍ଥିର bool
include_field (uintmax_t field)
{
  ସଂରଚନା field_range_pair *p = frp;
  ଯଦି (!p)
    ଫେରନ୍ତୁ field == 1;

  ଯେତେବେଳେ (p->lo != UINTMAX_MAX)
    {
      ଯଦି (p->lo <= field && p->hi >= field)
        ଫେରନ୍ତୁ true;
      ++p;
    }
  ଫେରନ୍ତୁ false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
ସ୍ଥିର bool
process_field (ଅକ୍ଷର *text, uintmax_t field)
{
  long ଦ୍ୱିଗୁଣ val = 0;
  size_t precision = 0;
  bool valid_ସଂଖ୍ୟା = true;

  ଯଦି (include_field (field))
    {
      valid_ସଂଖ୍ୟା =
        process_suffixed_ସଂଖ୍ୟା (text, &val, &precision, field);

      intmax_t padding;
      ଯଦି (valid_ସଂଖ୍ୟା)
        valid_ସଂଖ୍ୟା = prepare_padded_ସଂଖ୍ୟା (val, precision, &padding);

      ଯଦି (valid_ସଂଖ୍ୟା)
        print_padded_ସଂଖ୍ୟା (padding);
      ଅନ୍ୟଥା
        fputs (text, stdout);
    }
  ଅନ୍ୟଥା
    fputs (text, stdout);

  ଫେରନ୍ତୁ valid_ସଂଖ୍ୟା;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
process_ଧାଡ଼ି (ଅକ୍ଷର *ଧାଡ଼ି, bool newline)
{
  ଅକ୍ଷର *next;
  uintmax_t field = 0;
  bool valid_ସଂଖ୍ୟା = true;

  ଯେତେବେଳେ (true) {
    ++field;
    next = next_field (&ଧାଡ଼ି);

    ଯଦି (*ଧାଡ଼ି != '\0')
      {
        /* NUL terminate the current field string and process */
        ଅକ୍ଷର end_field = *ଧାଡ଼ି;
        *ଧାଡ଼ି = '\0';

        ଯଦି (! process_field (next, field))
          valid_ସଂଖ୍ୟା = false;

        ଯଦି (delimiter != ଖାଲି)
          fputs (delimiter, stdout);
        ଅନ୍ୟଥା
          fputc (' ', stdout);

        ଯଦି (delimiter)
          ଧାଡ଼ି += MAX (strlen (delimiter), 1);
        ଅନ୍ୟଥା
          {
            *ଧାଡ଼ି = end_field;
            mcel_t g = mcel_scanz (ଧାଡ଼ି);
            ଧାଡ଼ି += g.len;
          }
      }
    ଅନ୍ୟଥା
      {
        /* end of the line, process the last field and finish */
        ଯଦି (! process_field (next, field))
          valid_ସଂଖ୍ୟା = false;

        ଭାଙ୍ଗ;
      }
  }

  ଯଦି (newline)
    putchar (ଧାଡ଼ି_delim);

 ଯଦି (ferror (stdout))
   write_error ();

  ଫେରନ୍ତୁ valid_ସଂଖ୍ୟା;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#ଯଦି HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#endif

  decimal_point = nl_langinfo (RADIXCHAR);
  ଯଦି (decimal_point == ଖାଲି || strlen (decimal_point) == 0)
    decimal_point = ".";
  decimal_point_length = strlen (decimal_point);

  thousands_sep = nl_langinfo (THOUSEP);
  ଯଦି (thousands_sep == ଖାଲି)
    thousands_sep = "";
  thousands_sep_length = strlen (thousands_sep);

  atexit (close_stdout);

  ଯେତେବେଳେ (true)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getopt_long (argc, argv, "d:z", longopts, ଖାଲି);

      ଯଦି (c == -1)
        ଭାଙ୍ଗ;

      ବାଛ (c)
        {
        ସ୍ଥିତି FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_args, scale_from_types);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି FROM_UNIT_OPTION:
          from_unit_size = unit_to_umax (optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି TO_OPTION:
          scale_to =
            XARGMATCH ("--to", optarg, scale_to_args, scale_to_types);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି TO_UNIT_OPTION:
          to_unit_size = unit_to_umax (optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି ROUND_OPTION:
          round_style = XARGMATCH ("--round", optarg, round_args, round_types);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି GROUPING_OPTION:
          grouping = 1;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି PADDING_OPTION:
          ଯଦି (((xstrtoimax (optarg, ଖାଲି, 10, &padding_width, "")
                & ~LONGINT_OVERFLOW)
               != LONGINT_OK)
              || padding_width == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          ଭାଙ୍ଗ;

        ସ୍ଥିତି FIELD_OPTION:
          ଯଦି (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          set_fields (optarg, SETFLD_ALLOW_DASH);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          ଯଦି (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              ଯଦି (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'z':
          ଧାଡ଼ି_delim = '\0';
          ଭାଙ୍ଗ;

        ସ୍ଥିତି SUFFIX_OPTION:
          suffix = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି DEBUG_OPTION:
          debug = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି HEADER_OPTION:
          ଯଦି (optarg)
            {
              ଯଦି (xstrtoumax (optarg, ଖାଲି, 10, &header, "") != LONGINT_OK
                  || header == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          ଅନ୍ୟଥା
            {
              header = 1;
            }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି FORMAT_OPTION:
          format_str = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_args, inval_types);
          ଭାଙ୍ଗ;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (format_str != ଖାଲି && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  ଯଦି (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  ଯଦି (debug && scale_from == scale_none && scale_to == scale_none
      && !grouping && (padding_width == 0) && (format_str == ଖାଲି))
    error (0, 0, _("no conversion option specified"));

  ଯଦି (debug && unit_separator && delimiter == ଖାଲି)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  ଯଦି (format_str)
    parse_format_string (format_str);

  ଯଦି (grouping)
    {
      ଯଦି (scale_to != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      ଯଦି (debug && thousands_sep_length == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_width == 0 && delimiter == ଖାଲି);

  ଯଦି (inval_style != inval_abort)
    conv_exit_code = 0;

  ଯଦି (argc > optind)
    {
      ଯଦି (debug && header)
        error (0, 0, _("--header ignored with command-line input"));

      ପାଇଁ (; optind < argc; optind++)
        valid_numbers &= process_ଧାଡ଼ି (argv[optind], true);
    }
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର *ଧାଡ଼ି = ଖାଲି;
      size_t ଧାଡ଼ି_allocated = 0;
      ssize_t len;

      ଯେତେବେଳେ (header-- && getdelim (&ଧାଡ଼ି, &ଧାଡ଼ି_allocated,
                                   ଧାଡ଼ି_delim, stdin) > 0)
        {
          ଯଦି (fputs (ଧାଡ଼ି, stdout) == EOF)
            write_error ();
        }

      ଯେତେବେଳେ ((len = getdelim (&ଧାଡ଼ି, &ଧାଡ଼ି_allocated,
                              ଧାଡ଼ି_delim, stdin)) > 0)
        {
          bool newline = ଧାଡ଼ି[len - 1] == ଧାଡ଼ି_delim;
          ଯଦି (newline)
            ଧାଡ଼ି[len - 1] = '\0';
          valid_numbers &= process_ଧାଡ଼ି (ଧାଡ଼ି, newline);
        }

      ଯଦି (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  ଯଦି (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = EXIT_SUCCESS;
  ଯଦି (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_status = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_status);
}
