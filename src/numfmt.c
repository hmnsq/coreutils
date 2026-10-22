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

#ಆದರೆ HAVE_FPSETPREC
# include <ieeefp.h>
#ಆದರೆ_ಅಂತ್ಯ

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "numfmt"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
ಎಣಿಕೆ { EXIT_CONVERSION_WARNINGS = 2 };

ಎಣಿಕೆ
{
  FROM_OPTION = CHAR_MAX + 1,
  FROM_UNIT_OPTION,
  ಗಮ್ಯ_OPTION,
  ಗಮ್ಯ_UNIT_OPTION,
  ಸುತ್ತು_OPTION,
  SUFFIX_OPTION,
  GROUPING_OPTION,
  PADDING_OPTION,
  ಕ್ಷೇತ್ರ_OPTION,
  DEBUG_OPTION,
  DEV_DEBUG_OPTION,
  ಶೀರ್ಷಿಕೆ_OPTION,
  FORMAT_OPTION,
  INVALID_OPTION,
  UNIT_SEPARATOR_OPTION
};

ಎಣಿಕೆ scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ scale_from_ವಾದಗಳು[] =
{
  "none", "auto", "si", "iec", "iec-i", ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ scale_type ಸ್ಥಿರಾಂಕ scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ scale_ಗಮ್ಯ_ವಾದಗಳು[] =
{
  "none", "si", "iec", "iec-i", ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ scale_type ಸ್ಥಿರಾಂಕ scale_ಗಮ್ಯ_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


ಎಣಿಕೆ ಸುತ್ತು_type
{
  ಸುತ್ತು_ceiling,
  ಸುತ್ತು_floor,
  ಸುತ್ತು_from_zero,
  ಸುತ್ತು_ಗಮ್ಯ_zero,
  ಸುತ್ತು_ಹತ್ತಿರದ,
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ಸುತ್ತು_ವಾದಗಳು[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ ಸುತ್ತು_type ಸ್ಥಿರಾಂಕ ಸುತ್ತು_types[] =
{
  ಸುತ್ತು_ceiling, ಸುತ್ತು_floor, ಸುತ್ತು_from_zero, ಸುತ್ತು_ಗಮ್ಯ_zero, ಸುತ್ತು_ಹತ್ತಿರದ
};


ಎಣಿಕೆ inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ inval_ವಾದಗಳು[] =
{
  "abort", "fail", "warn", "ignore", ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ inval_type ಸ್ಥಿರಾಂಕ inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"from", required_ವಾದ, ಖಾಲಿ, FROM_OPTION},
  {"from-unit", required_ವಾದ, ಖಾಲಿ, FROM_UNIT_OPTION},
  {"to", required_ವಾದ, ಖಾಲಿ, ಗಮ್ಯ_OPTION},
  {"to-unit", required_ವಾದ, ಖಾಲಿ, ಗಮ್ಯ_UNIT_OPTION},
  {"round", required_ವಾದ, ಖಾಲಿ, ಸುತ್ತು_OPTION},
  {"padding", required_ವಾದ, ಖಾಲಿ, PADDING_OPTION},
  {"suffix", required_ವಾದ, ಖಾಲಿ, SUFFIX_OPTION},
  {"unit-separator", required_ವಾದ, ಖಾಲಿ, UNIT_SEPARATOR_OPTION},
  {"grouping", no_ವಾದ, ಖಾಲಿ, GROUPING_OPTION},
  {"delimiter", required_ವಾದ, ಖಾಲಿ, 'd'},
  {"field", required_ವಾದ, ಖಾಲಿ, ಕ್ಷೇತ್ರ_OPTION},
  {"debug", no_ವಾದ, ಖಾಲಿ, DEBUG_OPTION},
  {"-debug", no_ವಾದ, ಖಾಲಿ, DEV_DEBUG_OPTION},
  {"header", optional_ವಾದ, ಖಾಲಿ, ಶೀರ್ಷಿಕೆ_OPTION},
  {"format", required_ವಾದ, ಖಾಲಿ, FORMAT_OPTION},
  {"invalid", required_ವಾದ, ಖಾಲಿ, INVALID_OPTION},
  {"zero-terminated", no_ವಾದ, ಖಾಲಿ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
ಎಣಿಕೆ { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
ಎಣಿಕೆ { MAX_ACCEPTABLE_DIGITS = 33 };

ಸ್ಥಿರ ಎಣಿಕೆ scale_type scale_from = scale_none;
ಸ್ಥಿರ ಎಣಿಕೆ scale_type scale_ಗಮ್ಯ = scale_none;
ಸ್ಥಿರ ಎಣಿಕೆ ಸುತ್ತು_type ಸುತ್ತು_style = ಸುತ್ತು_from_zero;
ಸ್ಥಿರ ಎಣಿಕೆ inval_type inval_style = inval_abort;
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *suffix = ಖಾಲಿ;
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *unit_separator = ಖಾಲಿ;
ಸ್ಥಿರ uintmax_t from_unit_ಗಾತ್ರ = 1;
ಸ್ಥಿರ uintmax_t ಗಮ್ಯ_unit_ಗಾತ್ರ = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grouping = 0;
ಸ್ಥಿರ ಅಕ್ಷರ *padding_ಬಫರ್ = ಖಾಲಿ;
ಸ್ಥಿರ idx_t padding_ಬಫರ್_ಗಾತ್ರ = 0;
ಸ್ಥಿರ intmax_t padding_ಅಗಲ = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ zero_padding_ಅಗಲ = 0;
ಸ್ಥಿರ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ user_precision = -1;
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format_str = ಖಾಲಿ;
ಸ್ಥಿರ ಅಕ್ಷರ *format_str_ಪೂರ್ವಪ್ರತ್ಯಯ = ಖಾಲಿ;
ಸ್ಥಿರ ಅಕ್ಷರ *format_str_suffix = ಖಾಲಿ;

/* By default, any conversion error will terminate the program.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ conv_exit_ಸಂಕೇತ = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *delimiter = ಖಾಲಿ;

/* line delimiter.  */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸಾಲು_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
ಸ್ಥಿರ uintmax_t ಶೀರ್ಷಿಕೆ = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
ಸ್ಥಿರ bool debug;

/* will be set according to the current locale.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *decimal_ಬಿಂದು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ decimal_ಬಿಂದು_ಉದ್ದಳತೆ;
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *thousands_sep;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ thousands_sep_ಉದ್ದಳತೆ;

/* debugging for developers.  Enables devmsg().  */
ಸ್ಥಿರ bool dev_debug = false;

ಸ್ಥಿರ bool
newline_or_blank (mcel_t g)
{
  ಹಿಂತಿರುಗಿ g.ch == '\n' || c32issep (g.ch);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
default_scale_ತಳ (ಎಣಿಕೆ scale_type scale)
{
  ಆಯ್ಕೆ (scale)
    {
    ಸ್ಥಿತಿ scale_IEC:
    ಸ್ಥಿತಿ scale_IEC_I:
      ಹಿಂತಿರುಗಿ 1024;

    ಸ್ಥಿತಿ scale_none:
    ಸ್ಥಿತಿ scale_auto:
    ಸ್ಥಿತಿ scale_SI:
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ 1000;
    }
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *valid_suffixes = 1 + zero_and_valid_suffixes;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
valid_suffix (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ suf)
{
  ಹಿಂತಿರುಗಿ strchr (valid_suffixes, suf) != ಖಾಲಿ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
suffix_power (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ suf)
{
  ಆಯ್ಕೆ (suf)
    {
    ಸ್ಥಿತಿ 'k':                  /* kilo.  */
    ಸ್ಥಿತಿ 'K':                  /* kilo or kibi.  */
      ಹಿಂತಿರುಗಿ 1;

    ಸ್ಥಿತಿ 'M':                  /* mega or mebi.  */
      ಹಿಂತಿರುಗಿ 2;

    ಸ್ಥಿತಿ 'G':                  /* giga or gibi.  */
      ಹಿಂತಿರುಗಿ 3;

    ಸ್ಥಿತಿ 'T':                  /* tera or tebi.  */
      ಹಿಂತಿರುಗಿ 4;

    ಸ್ಥಿತಿ 'P':                  /* peta or pebi.  */
      ಹಿಂತಿರುಗಿ 5;

    ಸ್ಥಿತಿ 'E':                  /* exa or exbi.  */
      ಹಿಂತಿರುಗಿ 6;

    ಸ್ಥಿತಿ 'Z':                  /* zetta or 2**70.  */
      ಹಿಂತಿರುಗಿ 7;

    ಸ್ಥಿತಿ 'Y':                  /* yotta or 2**80.  */
      ಹಿಂತಿರುಗಿ 8;

    ಸ್ಥಿತಿ 'R':                  /* ronna or 2**90.  */
      ಹಿಂತಿರುಗಿ 9;

    ಸ್ಥಿತಿ 'Q':                  /* quetta or 2**100.  */
      ಹಿಂತಿರುಗಿ 10;

    ಪೂರ್ವನಿಯೋಜಿತ:                   /* should never happen. assert?  */
      ಹಿಂತಿರುಗಿ 0;
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
suffix_power_char (ಪೂರ್ಣಾಂಕ power)
{
  ಆಯ್ಕೆ (power)
    {
    ಸ್ಥಿತಿ 0:
      ಹಿಂತಿರುಗಿ "";

    ಸ್ಥಿತಿ 1:
      ಹಿಂತಿರುಗಿ "K";

    ಸ್ಥಿತಿ 2:
      ಹಿಂತಿರುಗಿ "M";

    ಸ್ಥಿತಿ 3:
      ಹಿಂತಿರುಗಿ "G";

    ಸ್ಥಿತಿ 4:
      ಹಿಂತಿರುಗಿ "T";

    ಸ್ಥಿತಿ 5:
      ಹಿಂತಿರುಗಿ "P";

    ಸ್ಥಿತಿ 6:
      ಹಿಂತಿರುಗಿ "E";

    ಸ್ಥಿತಿ 7:
      ಹಿಂತಿರುಗಿ "Z";

    ಸ್ಥಿತಿ 8:
      ಹಿಂತಿರುಗಿ "Y";

    ಸ್ಥಿತಿ 9:
      ಹಿಂತಿರುಗಿ "R";

    ಸ್ಥಿತಿ 10:
      ಹಿಂತಿರುಗಿ "Q";

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
ಸ್ಥಿರ ದೀರ್ಘ ದ್ವಿಗುಣ
powerld (ದೀರ್ಘ ದ್ವಿಗುಣ ತಳ_2, ptrdiff_t x)
{
  ದೀರ್ಘ ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = ತಳ_2;
  ಆದರೆ (x == 0)
    ಹಿಂತಿರುಗಿ 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  ಆಗ (--x)
    ಫಲಿತಾಂಶ *= ತಳ_2;
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದೀರ್ಘ ದ್ವಿಗುಣ
absld (ದೀರ್ಘ ದ್ವಿಗುಣ val)
{
  ಹಿಂತಿರುಗಿ val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
ಸ್ಥಿರ ದೀರ್ಘ ದ್ವಿಗುಣ
expld (ದೀರ್ಘ ದ್ವಿಗುಣ val, ಪೂರ್ಣಾಂಕ ತಳ_2, ಪೂರ್ಣಾಂಕ /*output */ *x)
{
  ಪೂರ್ಣಾಂಕ power = 0;

  ಆದರೆ (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      ಆಗ (absld (val) >= ತಳ_2)
        {
          ++power;
          val /= ತಳ_2;
        }
    }
  ಆದರೆ (x)
    *x = power;
  ಹಿಂತಿರುಗಿ val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ intmax_t
simple_ಸುತ್ತು_ceiling (ದೀರ್ಘ ದ್ವಿಗುಣ val)
{
  intmax_t intval = val;
  ಆದರೆ (intval < val)
    intval++;
  ಹಿಂತಿರುಗಿ intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ intmax_t
simple_ಸುತ್ತು_floor (ದೀರ್ಘ ದ್ವಿಗುಣ val)
{
  ಹಿಂತಿರುಗಿ -simple_ಸುತ್ತು_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ intmax_t
simple_ಸುತ್ತು_from_zero (ದೀರ್ಘ ದ್ವಿಗುಣ val)
{
  ಹಿಂತಿರುಗಿ val < 0 ? simple_ಸುತ್ತು_floor (val) : simple_ಸುತ್ತು_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ intmax_t
simple_ಸುತ್ತು_ಗಮ್ಯ_zero (ದೀರ್ಘ ದ್ವಿಗುಣ val)
{
  ಹಿಂತಿರುಗಿ val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ intmax_t
simple_ಸುತ್ತು_ಹತ್ತಿರದ (ದೀರ್ಘ ದ್ವಿಗುಣ val)
{
  ಹಿಂತಿರುಗಿ val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದೀರ್ಘ ದ್ವಿಗುಣ
simple_ಸುತ್ತು (ದೀರ್ಘ ದ್ವಿಗುಣ val, ಎಣಿಕೆ ಸುತ್ತು_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (ದೀರ್ಘ ದ್ವಿಗುಣ) INTMAX_MAX * intmax_mul;

  ಆಯ್ಕೆ (t)
    {
    ಸ್ಥಿತಿ ಸುತ್ತು_ceiling:
      rval = simple_ಸುತ್ತು_ceiling (val);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ಸುತ್ತು_floor:
      rval = simple_ಸುತ್ತು_floor (val);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ಸುತ್ತು_from_zero:
      rval = simple_ಸುತ್ತು_from_zero (val);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ಸುತ್ತು_ಗಮ್ಯ_zero:
      rval = simple_ಸುತ್ತು_ಗಮ್ಯ_zero (val);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ಸುತ್ತು_ಹತ್ತಿರದ:
      rval = simple_ಸುತ್ತು_ಹತ್ತಿರದ (val);
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      /* to silence the compiler - this should never happen.  */
      ಹಿಂತಿರುಗಿ 0;
    }

  ಹಿಂತಿರುಗಿ (ದೀರ್ಘ ದ್ವಿಗುಣ) INTMAX_MAX * intmax_mul + rval;
}

ಎಣಿಕೆ simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_ಅತಿಪ್ರವಾಹ,
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
ಸ್ಥಿರ ಎಣಿಕೆ simple_strtod_error
simple_strtod_int (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಒಳಹರಿವು_str,
                   ಅಕ್ಷರ **endptr, ದೀರ್ಘ ದ್ವಿಗುಣ *ಮೌಲ್ಯ, bool *negative)
{
  ಎಣಿಕೆ simple_strtod_error e = SSE_OK;

  ದೀರ್ಘ ದ್ವಿಗುಣ val = 0;
  ಪೂರ್ಣಾಂಕ digits = 0;
  bool ಸಿಕ್ಕಿತು_digit = false;

  ಆದರೆ (*ಒಳಹರಿವು_str == '-')
    {
      ಒಳಹರಿವು_str++;
      *negative = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    *negative = false;

  *endptr = (ಅಕ್ಷರ *) ಒಳಹರಿವು_str;
  ಆಗ (c_isdigit (**endptr))
    {
      ಪೂರ್ಣಾಂಕ digit = (**endptr) - '0';

      ಸಿಕ್ಕಿತು_digit = true;

      ಆದರೆ (val || digit)
        digits++;

      ಆದರೆ (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      ಆದರೆ (digits > MAX_ACCEPTABLE_DIGITS)
        ಹಿಂತಿರುಗಿ SSE_ಅತಿಪ್ರವಾಹ;

      val *= 10;
      val += digit;

      ++(*endptr);

      ಆದರೆ (thousands_sep_ಉದ್ದಳತೆ > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_ಉದ್ದಳತೆ)
          && c_isdigit ((*endptr)[thousands_sep_ಉದ್ದಳತೆ]))
        (*endptr) += thousands_sep_ಉದ್ದಳತೆ;
    }
  ಆದರೆ (! ಸಿಕ್ಕಿತು_digit
      && ! STREQ_LEN (*endptr, decimal_ಬಿಂದು, decimal_ಬಿಂದು_ಉದ್ದಳತೆ))
    ಹಿಂತಿರುಗಿ SSE_INVALID_ಸಂಖ್ಯೆ;
  ಆದರೆ (*negative)
    val = -val;

  ಆದರೆ (ಮೌಲ್ಯ)
    *ಮೌಲ್ಯ = val;

  ಹಿಂತಿರುಗಿ e;
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
ಸ್ಥಿರ ಎಣಿಕೆ simple_strtod_error
simple_strtod_float (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಒಳಹರಿವು_str,
                     ಅಕ್ಷರ **endptr,
                     ದೀರ್ಘ ದ್ವಿಗುಣ *ಮೌಲ್ಯ,
                     size_t *precision)
{
  bool negative;
  ಎಣಿಕೆ simple_strtod_error e = SSE_OK;

  ಆದರೆ (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (ಒಳಹರಿವು_str, endptr, ಮೌಲ್ಯ, &negative);
  ಆದರೆ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ಹಿಂತಿರುಗಿ e;

  /* optional decimal point + fraction.  */
  ಆದರೆ (STREQ_LEN (*endptr, decimal_ಬಿಂದು, decimal_ಬಿಂದು_ಉದ್ದಳತೆ))
    {
      ಅಕ್ಷರ *ptr2;
      ದೀರ್ಘ ದ್ವಿಗುಣ val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_ಬಿಂದು_ಉದ್ದಳತೆ;
      ಎಣಿಕೆ simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      ಆದರೆ (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        ಹಿಂತಿರುಗಿ e2;
      ಆದರೆ (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      ಆದರೆ (neg_frac)
        ಹಿಂತಿರುಗಿ SSE_INVALID_ಸಂಖ್ಯೆ;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      ಆದರೆ (ಮೌಲ್ಯ)
        {
          ಆದರೆ (negative)
            *ಮೌಲ್ಯ -= val_frac;
          ಇಲ್ಲದಿದ್ದರೆ
            *ಮೌಲ್ಯ += val_frac;
        }

      ಆದರೆ (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  ಹಿಂತಿರುಗಿ e;
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
ಸ್ಥಿರ ಎಣಿಕೆ simple_strtod_error
simple_strtod_human (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಒಳಹರಿವು_str,
                     ಅಕ್ಷರ **endptr, ದೀರ್ಘ ದ್ವಿಗುಣ *ಮೌಲ್ಯ, size_t *precision,
                     ಎಣಿಕೆ scale_type allowed_scaling)
{
  ಪೂರ್ಣಾಂಕ power = 0;
  /* 'scale_auto' is checked below.  */
  ಪೂರ್ಣಾಂಕ scale_ತಳ = default_scale_ತಳ (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, ಒಳಹರಿವು_str),
          quote_n (1, decimal_ಬಿಂದು),
          MAX_UNSCALED_DIGITS);

  ಎಣಿಕೆ simple_strtod_error e =
    simple_strtod_float (ಒಳಹರಿವು_str, endptr, ಮೌಲ್ಯ, precision);
  ಆದರೆ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ಹಿಂತಿರುಗಿ e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *ಮೌಲ್ಯ, (ಪೂರ್ಣಾಂಕ)*precision);

  ಆಗ (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      ಆದರೆ (unit_separator)
        {
          size_t sep_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (unit_separator);
          ಆದರೆ (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      ಆದರೆ (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          ಆದರೆ (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      ಆದರೆ (**endptr == '\0')
        ನಿಲ್ಲಿಸು;  /* Treat as no suffix.  */

      ಆದರೆ (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          ಆದರೆ (**endptr == '\0')
            ನಿಲ್ಲಿಸು;

          ಹಿಂತಿರುಗಿ SSE_INVALID_SUFFIX;
        }

      ಆದರೆ (allowed_scaling == scale_none)
        ಹಿಂತಿರುಗಿ SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      ಆದರೆ (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_ತಳ = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_ತಳ);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (allowed_scaling == scale_IEC_I)
        {
          ಆದರೆ (**endptr == 'i')
            (*endptr)++;
          ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      ನಿಲ್ಲಿಸು;
    }

  ದೀರ್ಘ ದ್ವಿಗುಣ multiplier = powerld (scale_ತಳ, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_ತಳ, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*ಮೌಲ್ಯ) = (*ಮೌಲ್ಯ) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *ಮೌಲ್ಯ, *ಮೌಲ್ಯ);

  ಹಿಂತಿರುಗಿ e;
}


ಸ್ಥಿರ ಶೂನ್ಯ
simple_strtod_fatal (ಎಣಿಕೆ simple_strtod_error err, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಒಳಹರಿವು_str)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *msgid = ಖಾಲಿ;

  ಆಯ್ಕೆ (err)
    {
    ಸ್ಥಿತಿ SSE_ಅತಿಪ್ರವಾಹ:
      msgid = N_("value too large to be converted: %s");
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ SSE_INVALID_ಸಂಖ್ಯೆ:
      msgid = N_("invalid number: %s");
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ SSE_OK_PRECISION_LOSS:
    ಸ್ಥಿತಿ SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  ಆದರೆ (inval_style != inval_ignore)
    error (conv_exit_ಸಂಕೇತ, 0, gettext (msgid), quote (ಒಳಹರಿವು_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
double_ಗಮ್ಯ_human (ದೀರ್ಘ ದ್ವಿಗುಣ val, ಪೂರ್ಣಾಂಕ precision,
                 ಅಕ್ಷರ *buf, idx_t buf_ಗಾತ್ರ,
                 ಎಣಿಕೆ scale_type scale, ಪೂರ್ಣಾಂಕ ಗುಂಪು, ಎಣಿಕೆ ಸುತ್ತು_type ಸುತ್ತು_2)
{
  ಅಕ್ಷರ fmt[ಗಾತ್ರ "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_ಅಗಲ)];
  ಅಕ್ಷರ *pfmt = fmt;
  *pfmt++ = '%';

  ಆದರೆ (ಗುಂಪು)
    *pfmt++ = '\'';

  ಆದರೆ (zero_padding_ಅಗಲ)
    pfmt += sprintf (pfmt, "0%d", zero_padding_ಅಗಲ);

  devmsg ("double_to_human:\n");

  ಆದರೆ (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_ಸುತ್ತು (val, ಸುತ್ತು_2);
      val /= powerld (10, precision);

      devmsg ((ಗುಂಪು) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      ಹಿಂತಿರುಗಿ snprintf (buf, buf_ಗಾತ್ರ, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  ದ್ವಿಗುಣ scale_ತಳ = default_scale_ತಳ (scale);

  /* Normalize val to scale. */
  ಪೂರ್ಣಾಂಕ power = 0;
  val = expld (val, scale_ತಳ, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_ತಳ, power);

  /* Perform rounding. */
  ಪೂರ್ಣಾಂಕ power_adjust = 0;
  ಆದರೆ (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (absld (val) < 10)
    {
      /* for values less than 10, we allow one decimal-point digit,
         so adjust before rounding. */
      power_adjust = 1;
    }

  val *= powerld (10, power_adjust);
  val = simple_ಸುತ್ತು (val, ಸುತ್ತು_2);
  val /= powerld (10, power_adjust);

  /* two special cases after rounding:
     1. a "999.99" can turn into 1000 - so scale down
     2. a "9.99" can turn into 10 - so don't display decimal-point.  */
  ಆದರೆ (absld (val) >= scale_ತಳ)
    {
      val /= scale_ತಳ;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  ಪೂರ್ಣಾಂಕ show_decimal_ಬಿಂದು = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_ತಳ, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  ಪೂರ್ಣಾಂಕ prec = user_precision == -1 ? show_decimal_ಬಿಂದು : user_precision;

  ಹಿಂತಿರುಗಿ snprintf (buf, buf_ಗಾತ್ರ, fmt, prec, val,
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
unit_ಗಮ್ಯ_umax (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *n_string)
{
  strtol_error s_err;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *c_string = n_string;
  ಅಕ್ಷರ *t_string = ಖಾಲಿ;
  size_t n_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (n_string);
  ಅಕ್ಷರ *ಕೊನೆ = ಖಾಲಿ;
  uintmax_t n;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  ಆದರೆ (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      ಕೊನೆ = t_string + n_len - 1;
      ಸ್ಮರಣೆ_ನಕಲು (t_string, n_string, n_len);

      ಆದರೆ (*ಕೊನೆ == 'i' && 2 <= n_len && ! c_isdigit (*(ಕೊನೆ - 1)))
        *ಕೊನೆ = '\0';
      ಇಲ್ಲದಿದ್ದರೆ
        {
          *++ಕೊನೆ = 'B';
          *++ಕೊನೆ = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &ಕೊನೆ, 10, &n, suffixes);

  ಆದರೆ (s_err != LONGINT_OK || *ಕೊನೆ || n == 0)
    {
      ಬಿಡುಗಡೆ (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  ಬಿಡುಗಡೆ (t_string);

  ಹಿಂತಿರುಗಿ n;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [ಸಂಖ್ಯೆ_2]...\n\
"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Reformat ಸಂಖ್ಯೆ_2(s), or the numbers from standard ಒಳಹರಿವು ಆದರೆ none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid ಒಳಹರಿವು\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead ದ whitespace ಗಾಗಿ ಕ್ಷೇತ್ರ delimiter\n\
"));
      oputs (_("\
      --ಕ್ಷೇತ್ರ=FIELDS\n\
         replace the numbers ಒಳ these ಒಳಹರಿವು fields (ಪೂರ್ವನಿಯೋಜಿತ=1);\n\
         see FIELDS ಕೆಳಗೆ ಗಾಗಿ details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use ಬರೆ style floating-ಬಿಂದು FORMAT;\n\
         see FORMAT ಕೆಳಗೆ ಗಾಗಿ details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         ಸ್ವಯಂಚಾಲಿತ-scale ಒಳಹರಿವು numbers ಗಮ್ಯ UNITs; ಪೂರ್ವನಿಯೋಜಿತ is 'none';\n\
         see UNIT ಕೆಳಗೆ ಗಾಗಿ details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the ಒಳಹರಿವು unit ಗಾತ್ರ_2 (instead ದ the ಪೂರ್ವನಿಯೋಜಿತ 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping ದ digits, e.g. 1,000,000.\n\
         This has no effect ಒಳ the C/POSIX locale\n\
"));
      oputs (_("\
      --ಶೀರ್ಷಿಕೆ[=N]\n\
         print (without converting) the ಮೊದಲ N ಶೀರ್ಷಿಕೆ ಸಾಲುಗಳು;\n\
         N defaults ಗಮ್ಯ 1 ಆದರೆ not specified\n\
"));
      oputs (_("\
      --invalid=ಬಹುಲಕ\n\
         failure ಬಹುಲಕ_2 ಗಾಗಿ invalid numbers;\n\
         ಬಹುಲಕ can be: abort (ಪೂರ್ವನಿಯೋಜಿತ), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output ಗಮ್ಯ N characters;\n\
         ಧನಾತ್ಮಕ N will ಬಲ-align, negative N will ಎಡ-align;\n\
         padding is ignored ಆದರೆ the output is wider than N;\n\
         the ಪೂರ್ವನಿಯೋಜಿತ is ಗಮ್ಯ automatically pad ಆದರೆ a whitespace is ಸಿಕ್ಕಿತು\n\
"));
      oputs (_("\
      --ಸುತ್ತು_2=ವಿಧಾನ\n\
         use ವಿಧಾನ ಗಾಗಿ rounding when scaling; ವಿಧಾನ can be:\n\
         ಮೇಲೆ, ಕೆಳಕ್ಕೆ, from-zero (ಪೂರ್ವನಿಯೋಜಿತ), towards-zero, ಹತ್ತಿರದ\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         ಕೂಡಿಸು SUFFIX ಗಮ್ಯ output numbers,\n\
         and accept an optional SUFFIX ಒಳ ಒಳಹರಿವು numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         ಸೇರಿಸು SEP between ಸಂಖ್ಯೆ and unit on output,\n\
         and accept an optional SEP ಒಳ ಒಳಹರಿವು numbers\n\
"));
      oputs (_("\
      --ಗಮ್ಯ=UNIT\n\
         ಸ್ವಯಂಚಾಲಿತ-scale output numbers ಗಮ್ಯ UNITs; see UNIT ಕೆಳಗೆ\n\
"));
      oputs (_("\
      --ಗಮ್ಯ-unit=N\n\
         the output unit ಗಾತ್ರ_2 (instead ದ the ಪೂರ್ವನಿಯೋಜಿತ 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);

      fputs (_("\
\n\
UNIT options:\n"), stdout);
      fputs (_("\
  none       no ಸ್ವಯಂಚಾಲಿತ-scaling is ಮುಗಿದ; suffixes will trigger an error\n\
"), stdout);
      fputs (_("\
  ಸ್ವಯಂಚಾಲಿತ       accept optional single/two letter suffix:\n\
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
FIELDS supports cut(1) style ಕ್ಷೇತ್ರ ranges:\n\
  N    N'th ಕ್ಷೇತ್ರ, counted from 1\n\
  N-   from N'th ಕ್ಷೇತ್ರ, ಗಮ್ಯ ಕೊನೆ ದ ಸಾಲು\n\
  N-M  from N'th to M'th ಕ್ಷೇತ್ರ (inclusive)\n\
  -M   from ಮೊದಲ ಗಮ್ಯ M'th ಕ್ಷೇತ್ರ (inclusive)\n\
  -    all fields\n\
ಅಪವರ್ತ್ಯ_2 fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable ಗಾಗಿ printing one floating-ಬಿಂದು ವಾದ '%f'.\n\
Optional quote (%'f) will enable --grouping (ಆದರೆ supported by current locale).\n\
Optional ಅಗಲ ಮೌಲ್ಯ (%10f) will pad output. Optional zero (%010f) ಅಗಲ\n\
will zero pad the ಸಂಖ್ಯೆ. Optional negative ಮೌಲ್ಯಗಳು (%-10f) will ಎಡ align.\n\
Optional precision (%.1f) will override the ಒಳಹರಿವು determined precision.\n\
"), stdout);

      ಬರೆ (_("\n\
Exit ಸ್ಥಿತಿ_2 is 0 ಆದರೆ all ಒಳಹರಿವು numbers were successfully converted.\n\
By ಪೂರ್ವನಿಯೋಜಿತ, %s will stop at the ಮೊದಲ conversion error with ನಿರ್ಗಮಿಸು ಸ್ಥಿತಿ_2 2.\n\
With --invalid='fail' a warning is printed ಗಾಗಿ each conversion error\n\
and the ನಿರ್ಗಮಿಸು ಸ್ಥಿತಿ_2 is 2.  With --invalid='warn' each conversion error is\n\
diagnosed, but the ನಿರ್ಗಮಿಸು ಸ್ಥಿತಿ_2 is 0.  With --invalid='ignore' conversion\n\
errors are not diagnosed and the ನಿರ್ಗಮಿಸು ಸ್ಥಿತಿ_2 is 0.\n\
"), ಕಾರ್ಯಕ್ರಮ_name);

      ಬರೆ (_("\n\
Examples:\n\
  $ %s --ಗಮ್ಯ=si 1000\n\
            -> \"1.0k\"\n\
  $ %s --ಗಮ್ಯ=iec 2048\n\
           -> \"2.0K\"\n\
  $ %s --ಗಮ್ಯ=iec-i 4096\n\
           -> \"4.0Ki\"\n\
  $ echo 1K | %s --from=si\n\
           -> \"1000\"\n\
  $ echo 1K | %s --from=iec\n\
           -> \"1024\"\n\
  $ df -B1 | %s --ಶೀರ್ಷಿಕೆ --ಕ್ಷೇತ್ರ 2-4 --ಗಮ್ಯ=si\n\
  $ ls -l  | %s --ಶೀರ್ಷಿಕೆ --ಕ್ಷೇತ್ರ 5 --ಗಮ್ಯ=iec\n\
  $ ls -lh | %s --ಶೀರ್ಷಿಕೆ --ಕ್ಷೇತ್ರ 5 --from=iec --padding=10\n\
  $ ls -lh | %s --ಶೀರ್ಷಿಕೆ --ಕ್ಷೇತ್ರ 5 --from=iec --format %%10f\n"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name,
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name,
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
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
parse_format_string (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt)
{
  size_t i;
  size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len = 0;
  size_t suffix_pos;
  ಅಕ್ಷರ *endptr = ಖಾಲಿ;
  bool zero_padding = false;

  ಗಾಗಿ (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      ಆದರೆ (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      ಪೂರ್ವಪ್ರತ್ಯಯ_len++;
    }

  i++;
  ಆಗ (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      ಆದರೆ (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! skip)
        ನಿಲ್ಲಿಸು;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  ಆದರೆ (pad != 0)
    {
      ಆದರೆ (debug && padding_ಅಗಲ && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      ಆದರೆ (pad < 0)
        padding_ಅಗಲ = pad;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (zero_padding)
            zero_padding_ಅಗಲ = MIN (pad, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
          ಇಲ್ಲದಿದ್ದರೆ
            padding_ಅಗಲ = pad;
        }
    }
  i = endptr - fmt;

  ಆದರೆ (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  ಆದರೆ (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      ಆದರೆ (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  ಆದರೆ (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  ಗಾಗಿ (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    ಆದರೆ (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ_len)
    format_str_ಪೂರ್ವಪ್ರತ್ಯಯ = ximemdup0 (fmt, ಪೂರ್ವಪ್ರತ್ಯಯ_len);
  ಆದರೆ (fmt[suffix_pos] != '\0')
    format_str_suffix = xstrdup (fmt + suffix_pos);

  devmsg ("format String:\n  input: %s\n  grouping: %s\n"
                   "  padding width: %jd\n"
                   "  prefix: %s\n  suffix: %s\n",
          quote_n (0, fmt), (grouping) ? "yes" : "no",
          padding_ಅಗಲ,
          quote_n (1, format_str_ಪೂರ್ವಪ್ರತ್ಯಯ ? format_str_ಪೂರ್ವಪ್ರತ್ಯಯ : ""),
          quote_n (2, format_str_suffix ? format_str_suffix : ""));
}

/* Parse a numeric value (with optional suffix) from a string.
   Returns a long double value, with input precision.

   If there's an error converting the string to value - exits with
   an error.

   If there are any trailing characters after the number
   (besides a valid suffix) - exits with an error.  */
ಸ್ಥಿರ ಎಣಿಕೆ simple_strtod_error
parse_human_ಸಂಖ್ಯೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str, ದೀರ್ಘ ದ್ವಿಗುಣ /*output */ *ಮೌಲ್ಯ,
                    size_t *precision)
{
  ಅಕ್ಷರ *ptr = ಖಾಲಿ;

  ಎಣಿಕೆ simple_strtod_error e =
    simple_strtod_human (str, &ptr, ಮೌಲ್ಯ, precision, scale_from);
  ಆದರೆ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      ಹಿಂತಿರುಗಿ e;
    }

  ಆದರೆ (ptr && *ptr != '\0')
    {
      ಆದರೆ (inval_style != inval_ignore)
        error (conv_exit_ಸಂಕೇತ, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  ಹಿಂತಿರುಗಿ e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
ಸ್ಥಿರ bool
prepare_padded_ಸಂಖ್ಯೆ (ಸ್ಥಿರಾಂಕ ದೀರ್ಘ ದ್ವಿಗುಣ val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_ಬಳಸಿದ = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  ಪೂರ್ಣಾಂಕ x;
  expld (val, 10, &x);

  ಆದರೆ (scale_ಗಮ್ಯ == scale_none
      && x + precision_ಬಳಸಿದ > MAX_UNSCALED_DIGITS)
    {
      ಆದರೆ (inval_style != inval_ignore)
        {
          ಆದರೆ (precision_ಬಳಸಿದ)
            error (conv_exit_ಸಂಕೇತ, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_ಬಳಸಿದ);
          ಇಲ್ಲದಿದ್ದರೆ
            error (conv_exit_ಸಂಕೇತ, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      ಹಿಂತಿರುಗಿ false;
    }

  ಆದರೆ (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      ಆದರೆ (inval_style != inval_ignore)
        error (conv_exit_ಸಂಕೇತ, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      ಹಿಂತಿರುಗಿ false;
    }

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ numlen = double_ಗಮ್ಯ_human (val, precision_ಬಳಸಿದ,
                                    padding_ಬಫರ್, padding_ಬಫರ್_ಗಾತ್ರ,
                                    scale_ಗಮ್ಯ, grouping, ಸುತ್ತು_style);
      ptrdiff_t growth;
      ಆದರೆ (numlen < 0 || ckd_sub (&growth, numlen, padding_ಬಫರ್_ಗಾತ್ರ - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      ಆದರೆ (growth <= 0)
        ನಿಲ್ಲಿಸು;
      padding_ಬಫರ್ = xpalloc (padding_ಬಫರ್, &padding_ಬಫರ್_ಗಾತ್ರ,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_ಬಫರ್));

  intmax_t pad = 0;
  ಆದರೆ (padding_ಅಗಲ)
    {
      ಪೂರ್ಣಾಂಕ buf_ಅಗಲ = mbswidth (padding_ಬಫರ್,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      ಆದರೆ (0 <= buf_ಅಗಲ)
        {
          ಆದರೆ (padding_ಅಗಲ < 0)
            {
              ಆದರೆ (padding_ಅಗಲ < -buf_ಅಗಲ)
                pad = padding_ಅಗಲ + buf_ಅಗಲ;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಆದರೆ (buf_ಅಗಲ < padding_ಅಗಲ)
                pad = padding_ಅಗಲ - buf_ಅಗಲ;
            }
        }
    }

  *padding = pad;
  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_padded_ಸಂಖ್ಯೆ (intmax_t padding)
{
  ಆದರೆ (format_str_ಪೂರ್ವಪ್ರತ್ಯಯ)
    fputs (format_str_ಪೂರ್ವಪ್ರತ್ಯಯ, stdout);

  ಗಾಗಿ (intmax_t p = padding; 0 < p; p--)
    ಅಕ್ಷರ_ಬರೆ (' ');

  fputs (padding_ಬಫರ್, stdout);

  ಗಾಗಿ (intmax_t p = padding; p < 0; p++)
    ಅಕ್ಷರ_ಬರೆ (' ');

  ಆದರೆ (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
ಸ್ಥಿರ bool
process_suffixed_ಸಂಖ್ಯೆ (ಅಕ್ಷರ *ಪಠ್ಯ, ದೀರ್ಘ ದ್ವಿಗುಣ *ಫಲಿತಾಂಶ,
                         size_t *precision, ದೀರ್ಘ ಪೂರ್ಣಾಂಕ ಕ್ಷೇತ್ರ)
{
  ಅಕ್ಷರ saved_suffix = '\0';

  ಆದರೆ (suffix)
    {
      ಆದರೆ (mbs_endswith (ಪಠ್ಯ, suffix))
        {
          saved_suffix = *(ಪಠ್ಯ + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಪಠ್ಯ) - ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (suffix));
          *(ಪಠ್ಯ + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಪಠ್ಯ) - ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  ಅಕ್ಷರ *p = skip_str_matching (ಪಠ್ಯ, newline_or_blank, true);

  /* setup auto-padding.  */
  ಆದರೆ (auto_padding)
    {
      padding_ಅಗಲ = ಪಠ್ಯ < p || 1 < ಕ್ಷೇತ್ರ
                      ? mbswidth (ಪಠ್ಯ,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      ಆದರೆ (padding_ಅಗಲ < 0)
        padding_ಅಗಲ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಪಠ್ಯ);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_ಅಗಲ);
    }

  ದೀರ್ಘ ದ್ವಿಗುಣ val = 0;
  ಎಣಿಕೆ simple_strtod_error e = parse_human_ಸಂಖ್ಯೆ (p, &val, precision);
  ಆದರೆ (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  ಆದರೆ (from_unit_ಗಾತ್ರ != 1 || ಗಮ್ಯ_unit_ಗಾತ್ರ != 1)
    val = (val * from_unit_ಗಾತ್ರ) / ಗಮ್ಯ_unit_ಗಾತ್ರ;

  *ಫಲಿತಾಂಶ = val;

  ಆದರೆ (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    ಹಿಂತಿರುಗಿ true;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (saved_suffix)
        *(ಪಠ್ಯ + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಪಠ್ಯ)) = saved_suffix;
      ಹಿಂತಿರುಗಿ false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಅಕ್ಷರ *
mbsmbchr (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *c)
{
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  ಆದರೆ (uc < 0x30 || MB_CUR_MAX == 1)
    ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) strchr (s, uc);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_utf8_charset ())
    ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) (uc < 0x80 ? strchr (s, uc) : ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (s, c));
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ *(c + 1) == '\0' ? mbschr (s, uc) : (ಅಕ್ಷರ *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
ಸ್ಥಿರ ಅಕ್ಷರ *
ಮುಂದಿನ_ಕ್ಷೇತ್ರ (ಅಕ್ಷರ **ಸಾಲು)
{
  ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_ಆರಂಭ = *ಸಾಲು;
  ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_ಕೊನೆ   = ಕ್ಷೇತ್ರ_ಆರಂಭ;

  ಆದರೆ (delimiter)
    {
      ಆದರೆ (! (ಕ್ಷೇತ್ರ_ಕೊನೆ = mbsmbchr (ಕ್ಷೇತ್ರ_ಆರಂಭ, delimiter)))
        ಕ್ಷೇತ್ರ_ಕೊನೆ = strchr (ಕ್ಷೇತ್ರ_ಆರಂಭ, '\0');
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* keep any space prefix in the returned field */
      ಕ್ಷೇತ್ರ_ಕೊನೆ = skip_str_matching (ಕ್ಷೇತ್ರ_ಕೊನೆ, newline_or_blank, true);
      ಕ್ಷೇತ್ರ_ಕೊನೆ = skip_str_matching (ಕ್ಷೇತ್ರ_ಕೊನೆ, newline_or_blank, false);
    }

  *ಸಾಲು = ಕ್ಷೇತ್ರ_ಕೊನೆ;
  ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ_ಆರಂಭ;
}

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
include_ಕ್ಷೇತ್ರ (uintmax_t ಕ್ಷೇತ್ರ)
{
  ರಚನೆ ಕ್ಷೇತ್ರ_range_pair *p = frp;
  ಆದರೆ (!p)
    ಹಿಂತಿರುಗಿ ಕ್ಷೇತ್ರ == 1;

  ಆಗ (p->lo != UINTMAX_MAX)
    {
      ಆದರೆ (p->lo <= ಕ್ಷೇತ್ರ && p->hi >= ಕ್ಷೇತ್ರ)
        ಹಿಂತಿರುಗಿ true;
      ++p;
    }
  ಹಿಂತಿರುಗಿ false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
ಸ್ಥಿರ bool
process_ಕ್ಷೇತ್ರ (ಅಕ್ಷರ *ಪಠ್ಯ, uintmax_t ಕ್ಷೇತ್ರ)
{
  ದೀರ್ಘ ದ್ವಿಗುಣ val = 0;
  size_t precision = 0;
  bool valid_ಸಂಖ್ಯೆ = true;

  ಆದರೆ (include_ಕ್ಷೇತ್ರ (ಕ್ಷೇತ್ರ))
    {
      valid_ಸಂಖ್ಯೆ =
        process_suffixed_ಸಂಖ್ಯೆ (ಪಠ್ಯ, &val, &precision, ಕ್ಷೇತ್ರ);

      intmax_t padding;
      ಆದರೆ (valid_ಸಂಖ್ಯೆ)
        valid_ಸಂಖ್ಯೆ = prepare_padded_ಸಂಖ್ಯೆ (val, precision, &padding);

      ಆದರೆ (valid_ಸಂಖ್ಯೆ)
        print_padded_ಸಂಖ್ಯೆ (padding);
      ಇಲ್ಲದಿದ್ದರೆ
        fputs (ಪಠ್ಯ, stdout);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    fputs (ಪಠ್ಯ, stdout);

  ಹಿಂತಿರುಗಿ valid_ಸಂಖ್ಯೆ;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
process_ಸಾಲು (ಅಕ್ಷರ *ಸಾಲು, bool newline)
{
  ಅಕ್ಷರ *ಮುಂದಿನ;
  uintmax_t ಕ್ಷೇತ್ರ = 0;
  bool valid_ಸಂಖ್ಯೆ = true;

  ಆಗ (true) {
    ++ಕ್ಷೇತ್ರ;
    ಮುಂದಿನ = ಮುಂದಿನ_ಕ್ಷೇತ್ರ (&ಸಾಲು);

    ಆದರೆ (*ಸಾಲು != '\0')
      {
        /* NUL terminate the current field string and process */
        ಅಕ್ಷರ ಕೊನೆ_ಕ್ಷೇತ್ರ = *ಸಾಲು;
        *ಸಾಲು = '\0';

        ಆದರೆ (! process_ಕ್ಷೇತ್ರ (ಮುಂದಿನ, ಕ್ಷೇತ್ರ))
          valid_ಸಂಖ್ಯೆ = false;

        ಆದರೆ (delimiter != ಖಾಲಿ)
          fputs (delimiter, stdout);
        ಇಲ್ಲದಿದ್ದರೆ
          fputc (' ', stdout);

        ಆದರೆ (delimiter)
          ಸಾಲು += MAX (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (delimiter), 1);
        ಇಲ್ಲದಿದ್ದರೆ
          {
            *ಸಾಲು = ಕೊನೆ_ಕ್ಷೇತ್ರ;
            mcel_t g = mcel_scanz (ಸಾಲು);
            ಸಾಲು += g.len;
          }
      }
    ಇಲ್ಲದಿದ್ದರೆ
      {
        /* end of the line, process the last field and finish */
        ಆದರೆ (! process_ಕ್ಷೇತ್ರ (ಮುಂದಿನ, ಕ್ಷೇತ್ರ))
          valid_ಸಂಖ್ಯೆ = false;

        ನಿಲ್ಲಿಸು;
      }
  }

  ಆದರೆ (newline)
    ಅಕ್ಷರ_ಬರೆ (ಸಾಲು_delim);

 ಆದರೆ (ferror (stdout))
   write_error ();

  ಹಿಂತಿರುಗಿ valid_ಸಂಖ್ಯೆ;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#ಆದರೆ HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#ಆದರೆ_ಅಂತ್ಯ

  decimal_ಬಿಂದು = nl_langinfo (RADIXCHAR);
  ಆದರೆ (decimal_ಬಿಂದು == ಖಾಲಿ || ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (decimal_ಬಿಂದು) == 0)
    decimal_ಬಿಂದು = ".";
  decimal_ಬಿಂದು_ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (decimal_ಬಿಂದು);

  thousands_sep = nl_langinfo (THOUSEP);
  ಆದರೆ (thousands_sep == ಖಾಲಿ)
    thousands_sep = "";
  thousands_sep_ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (thousands_sep);

  atexit (close_stdout);

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, "d:z", longopts, ಖಾಲಿ);

      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_ವಾದಗಳು, scale_from_types);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FROM_UNIT_OPTION:
          from_unit_ಗಾತ್ರ = unit_ಗಮ್ಯ_umax (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಗಮ್ಯ_OPTION:
          scale_ಗಮ್ಯ =
            XARGMATCH ("--to", optarg, scale_ಗಮ್ಯ_ವಾದಗಳು, scale_ಗಮ್ಯ_types);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಗಮ್ಯ_UNIT_OPTION:
          ಗಮ್ಯ_unit_ಗಾತ್ರ = unit_ಗಮ್ಯ_umax (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಸುತ್ತು_OPTION:
          ಸುತ್ತು_style = XARGMATCH ("--round", optarg, ಸುತ್ತು_ವಾದಗಳು, ಸುತ್ತು_types);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ GROUPING_OPTION:
          grouping = 1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PADDING_OPTION:
          ಆದರೆ (((xstrtoimax (optarg, ಖಾಲಿ, 10, &padding_ಅಗಲ, "")
                & ~LONGINT_ಅತಿಪ್ರವಾಹ)
               != LONGINT_OK)
              || padding_ಅಗಲ == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಕ್ಷೇತ್ರ_OPTION:
          ಆದರೆ (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          ಗಣ_fields (optarg, SETFLD_ALLOW_DASH);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          ಆದರೆ (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              ಆದರೆ (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'z':
          ಸಾಲು_delim = '\0';
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ SUFFIX_OPTION:
          suffix = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEBUG_OPTION:
          debug = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಶೀರ್ಷಿಕೆ_OPTION:
          ಆದರೆ (optarg)
            {
              ಆದರೆ (xstrtoumax (optarg, ಖಾಲಿ, 10, &ಶೀರ್ಷಿಕೆ, "") != LONGINT_OK
                  || ಶೀರ್ಷಿಕೆ == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಶೀರ್ಷಿಕೆ = 1;
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FORMAT_OPTION:
          format_str = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_ವಾದಗಳು, inval_types);
          ನಿಲ್ಲಿಸು;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (format_str != ಖಾಲಿ && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  ಆದರೆ (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  ಆದರೆ (debug && scale_from == scale_none && scale_ಗಮ್ಯ == scale_none
      && !grouping && (padding_ಅಗಲ == 0) && (format_str == ಖಾಲಿ))
    error (0, 0, _("no conversion option specified"));

  ಆದರೆ (debug && unit_separator && delimiter == ಖಾಲಿ)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  ಆದರೆ (format_str)
    parse_format_string (format_str);

  ಆದರೆ (grouping)
    {
      ಆದರೆ (scale_ಗಮ್ಯ != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      ಆದರೆ (debug && thousands_sep_ಉದ್ದಳತೆ == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_ಅಗಲ == 0 && delimiter == ಖಾಲಿ);

  ಆದರೆ (inval_style != inval_abort)
    conv_exit_ಸಂಕೇತ = 0;

  ಆದರೆ (argc > optind)
    {
      ಆದರೆ (debug && ಶೀರ್ಷಿಕೆ)
        error (0, 0, _("--header ignored with command-line input"));

      ಗಾಗಿ (; optind < argc; optind++)
        valid_numbers &= process_ಸಾಲು (argv[optind], true);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ *ಸಾಲು = ಖಾಲಿ;
      size_t ಸಾಲು_allocated = 0;
      ssize_t len;

      ಆಗ (ಶೀರ್ಷಿಕೆ-- && getdelim (&ಸಾಲು, &ಸಾಲು_allocated,
                                   ಸಾಲು_delim, stdin) > 0)
        {
          ಆದರೆ (fputs (ಸಾಲು, stdout) == EOF)
            write_error ();
        }

      ಆಗ ((len = getdelim (&ಸಾಲು, &ಸಾಲು_allocated,
                              ಸಾಲು_delim, stdin)) > 0)
        {
          bool newline = ಸಾಲು[len - 1] == ಸಾಲು_delim;
          ಆದರೆ (newline)
            ಸಾಲು[len - 1] = '\0';
          valid_numbers &= process_ಸಾಲು (ಸಾಲು, newline);
        }

      ಆದರೆ (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  ಆದರೆ (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = EXIT_SUCCESS;
  ಆದರೆ (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_ಸ್ಥಿತಿ = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_ಸ್ಥಿತಿ);
}
