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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <float.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <langinfo.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mbswidth.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mcel.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "skipchars.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "set-fields.h"

#ਜੇ HAVE_FPSETPREC
# include <ieeefp.h>
#ਜੇ_ਅੰਤ

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "numfmt"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Assaf Gordon")

/* Exit code when some numbers fail to convert.  */
ਗਿਣਤੀ { EXIT_CONVERSION_WARNINGS = 2 };

ਗਿਣਤੀ
{
  FROM_OPTION = CHAR_MAX + 1,
  FROM_UNIT_OPTION,
  ਮੰਜ਼ਿਲ_OPTION,
  ਮੰਜ਼ਿਲ_UNIT_OPTION,
  ਗੇੜਾ_OPTION,
  SUFFIX_OPTION,
  GROUPING_OPTION,
  PADDING_OPTION,
  ਖੇਤਰ_OPTION,
  DEBUG_OPTION,
  DEV_DEBUG_OPTION,
  ਸਿਰਲੇਖ_OPTION,
  FORMAT_OPTION,
  INVALID_OPTION,
  UNIT_SEPARATOR_OPTION
};

ਗਿਣਤੀ scale_type
{
  scale_none,                   /* the default: no scaling.  */
  scale_auto,                   /* --from only.  */
  scale_SI,
  scale_IEC,
  scale_IEC_I                   /* 'i' suffix is required.  */
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ scale_from_ਦਲੀਲਾਂ[] =
{
  "none", "auto", "si", "iec", "iec-i", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ scale_type ਅਚਲ scale_from_types[] =
{
  scale_none, scale_auto, scale_SI, scale_IEC, scale_IEC_I
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ scale_ਮੰਜ਼ਿਲ_ਦਲੀਲਾਂ[] =
{
  "none", "si", "iec", "iec-i", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ scale_type ਅਚਲ scale_ਮੰਜ਼ਿਲ_types[] =
{
  scale_none, scale_SI, scale_IEC, scale_IEC_I
};


ਗਿਣਤੀ ਗੇੜਾ_type
{
  ਗੇੜਾ_ceiling,
  ਗੇੜਾ_floor,
  ਗੇੜਾ_from_zero,
  ਗੇੜਾ_ਮੰਜ਼ਿਲ_zero,
  ਗੇੜਾ_ਸਭ_ਤੋਂ_ਨੇੜੇ,
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ ਗੇੜਾ_ਦਲੀਲਾਂ[] =
{
  "up", "down", "from-zero", "towards-zero", "nearest", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ ਗੇੜਾ_type ਅਚਲ ਗੇੜਾ_types[] =
{
  ਗੇੜਾ_ceiling, ਗੇੜਾ_floor, ਗੇੜਾ_from_zero, ਗੇੜਾ_ਮੰਜ਼ਿਲ_zero, ਗੇੜਾ_ਸਭ_ਤੋਂ_ਨੇੜੇ
};


ਗਿਣਤੀ inval_type
{
  inval_abort,
  inval_fail,
  inval_warn,
  inval_ignore
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ inval_ਦਲੀਲਾਂ[] =
{
  "abort", "fail", "warn", "ignore", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ inval_type ਅਚਲ inval_types[] =
{
  inval_abort, inval_fail, inval_warn, inval_ignore
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"from", required_ਦਲੀਲ, ਨਿਰਾ, FROM_OPTION},
  {"from-unit", required_ਦਲੀਲ, ਨਿਰਾ, FROM_UNIT_OPTION},
  {"to", required_ਦਲੀਲ, ਨਿਰਾ, ਮੰਜ਼ਿਲ_OPTION},
  {"to-unit", required_ਦਲੀਲ, ਨਿਰਾ, ਮੰਜ਼ਿਲ_UNIT_OPTION},
  {"round", required_ਦਲੀਲ, ਨਿਰਾ, ਗੇੜਾ_OPTION},
  {"padding", required_ਦਲੀਲ, ਨਿਰਾ, PADDING_OPTION},
  {"suffix", required_ਦਲੀਲ, ਨਿਰਾ, SUFFIX_OPTION},
  {"unit-separator", required_ਦਲੀਲ, ਨਿਰਾ, UNIT_SEPARATOR_OPTION},
  {"grouping", no_ਦਲੀਲ, ਨਿਰਾ, GROUPING_OPTION},
  {"delimiter", required_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"field", required_ਦਲੀਲ, ਨਿਰਾ, ਖੇਤਰ_OPTION},
  {"debug", no_ਦਲੀਲ, ਨਿਰਾ, DEBUG_OPTION},
  {"-debug", no_ਦਲੀਲ, ਨਿਰਾ, DEV_DEBUG_OPTION},
  {"header", optional_ਦਲੀਲ, ਨਿਰਾ, ਸਿਰਲੇਖ_OPTION},
  {"format", required_ਦਲੀਲ, ਨਿਰਾ, FORMAT_OPTION},
  {"invalid", required_ਦਲੀਲ, ਨਿਰਾ, INVALID_OPTION},
  {"zero-terminated", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Maximum number of digits we can safely handle
   without precision loss, if scaling is 'none'.  */
ਗਿਣਤੀ { MAX_UNSCALED_DIGITS = LDBL_DIG };

/* Maximum number of digits we can work with.
   This is equivalent to 999Q.
   NOTE: 'long double' can handle more than that, but there's
         no official suffix assigned beyond Quetta (1000^10).  */
ਗਿਣਤੀ { MAX_ACCEPTABLE_DIGITS = 33 };

ਸਥਿਰ ਗਿਣਤੀ scale_type scale_from = scale_none;
ਸਥਿਰ ਗਿਣਤੀ scale_type scale_ਮੰਜ਼ਿਲ = scale_none;
ਸਥਿਰ ਗਿਣਤੀ ਗੇੜਾ_type ਗੇੜਾ_style = ਗੇੜਾ_from_zero;
ਸਥਿਰ ਗਿਣਤੀ inval_type inval_style = inval_abort;
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *suffix = ਨਿਰਾ;
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *unit_separator = ਨਿਰਾ;
ਸਥਿਰ uintmax_t from_unit_ਮਾਪ = 1;
ਸਥਿਰ uintmax_t ਮੰਜ਼ਿਲ_unit_ਮਾਪ = 1;
ਸਥਿਰ ਪੂਰਨ grouping = 0;
ਸਥਿਰ ਅੱਖਰ *padding_ਬਫਰ = ਨਿਰਾ;
ਸਥਿਰ idx_t padding_ਬਫਰ_ਮਾਪ = 0;
ਸਥਿਰ intmax_t padding_ਚੌੜਾਈ = 0;
ਸਥਿਰ ਪੂਰਨ zero_padding_ਚੌੜਾਈ = 0;
ਸਥਿਰ ਲੰਮਾ ਪੂਰਨ user_precision = -1;
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *format_str = ਨਿਰਾ;
ਸਥਿਰ ਅੱਖਰ *format_str_ਅਗੇਤਰ = ਨਿਰਾ;
ਸਥਿਰ ਅੱਖਰ *format_str_suffix = ਨਿਰਾ;

/* By default, any conversion error will terminate the program.  */
ਸਥਿਰ ਪੂਰਨ conv_exit_ਕੋਡ = EXIT_CONVERSION_WARNINGS;


/* auto-pad each line based on skipped whitespace.  */
ਸਥਿਰ ਪੂਰਨ auto_padding = 0;

/* field delimiter - if NULL, blanks separate fields.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *delimiter = ਨਿਰਾ;

/* line delimiter.  */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਸਤਰ_delim = '\n';

/* if non-zero, the first 'header' lines from STDIN are skipped.  */
ਸਥਿਰ uintmax_t ਸਿਰਲੇਖ = 0;

/* Debug for users: print warnings to STDERR about possible
   error (similar to sort's debug).  */
ਸਥਿਰ bool debug;

/* will be set according to the current locale.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *decimal_ਬਿੰਦੂ;
ਸਥਿਰ ਪੂਰਨ decimal_ਬਿੰਦੂ_ਲੰਮਾਈ;
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *thousands_sep;
ਸਥਿਰ ਪੂਰਨ thousands_sep_ਲੰਮਾਈ;

/* debugging for developers.  Enables devmsg().  */
ਸਥਿਰ bool dev_debug = false;

ਸਥਿਰ bool
newline_or_blank (mcel_t g)
{
  ਵਾਪਸ g.ch == '\n' || c32issep (g.ch);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
default_scale_ਅਧਾਰ (ਗਿਣਤੀ scale_type scale)
{
  ਚੋਣ (scale)
    {
    ਹਾਲਤ scale_IEC:
    ਹਾਲਤ scale_IEC_I:
      ਵਾਪਸ 1024;

    ਹਾਲਤ scale_none:
    ਹਾਲਤ scale_auto:
    ਹਾਲਤ scale_SI:
    ਮੂਲ:
      ਵਾਪਸ 1000;
    }
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ zero_and_valid_suffixes[] = "0KkMGTPEZYRQ";
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *valid_suffixes = 1 + zero_and_valid_suffixes;

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
valid_suffix (ਅਚਲ ਅੱਖਰ suf)
{
  ਵਾਪਸ strchr (valid_suffixes, suf) != ਨਿਰਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
suffix_power (ਅਚਲ ਅੱਖਰ suf)
{
  ਚੋਣ (suf)
    {
    ਹਾਲਤ 'k':                  /* kilo.  */
    ਹਾਲਤ 'K':                  /* kilo or kibi.  */
      ਵਾਪਸ 1;

    ਹਾਲਤ 'M':                  /* mega or mebi.  */
      ਵਾਪਸ 2;

    ਹਾਲਤ 'G':                  /* giga or gibi.  */
      ਵਾਪਸ 3;

    ਹਾਲਤ 'T':                  /* tera or tebi.  */
      ਵਾਪਸ 4;

    ਹਾਲਤ 'P':                  /* peta or pebi.  */
      ਵਾਪਸ 5;

    ਹਾਲਤ 'E':                  /* exa or exbi.  */
      ਵਾਪਸ 6;

    ਹਾਲਤ 'Z':                  /* zetta or 2**70.  */
      ਵਾਪਸ 7;

    ਹਾਲਤ 'Y':                  /* yotta or 2**80.  */
      ਵਾਪਸ 8;

    ਹਾਲਤ 'R':                  /* ronna or 2**90.  */
      ਵਾਪਸ 9;

    ਹਾਲਤ 'Q':                  /* quetta or 2**100.  */
      ਵਾਪਸ 10;

    ਮੂਲ:                   /* should never happen. assert?  */
      ਵਾਪਸ 0;
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ ਅਚਲ *
suffix_power_char (ਪੂਰਨ power)
{
  ਚੋਣ (power)
    {
    ਹਾਲਤ 0:
      ਵਾਪਸ "";

    ਹਾਲਤ 1:
      ਵਾਪਸ "K";

    ਹਾਲਤ 2:
      ਵਾਪਸ "M";

    ਹਾਲਤ 3:
      ਵਾਪਸ "G";

    ਹਾਲਤ 4:
      ਵਾਪਸ "T";

    ਹਾਲਤ 5:
      ਵਾਪਸ "P";

    ਹਾਲਤ 6:
      ਵਾਪਸ "E";

    ਹਾਲਤ 7:
      ਵਾਪਸ "Z";

    ਹਾਲਤ 8:
      ਵਾਪਸ "Y";

    ਹਾਲਤ 9:
      ਵਾਪਸ "R";

    ਹਾਲਤ 10:
      ਵਾਪਸ "Q";

    ਮੂਲ:
      ਵਾਪਸ "(error)";
    }
}

/* Similar to 'powl(3)' but without requiring 'libm'.  */
ਸਥਿਰ ਲੰਮਾ ਦੁੱਗਣਾ
powerld (ਲੰਮਾ ਦੁੱਗਣਾ ਅਧਾਰ_2, ptrdiff_t x)
{
  ਲੰਮਾ ਦੁੱਗਣਾ ਨਤੀਜਾ = ਅਧਾਰ_2;
  ਜੇ (x == 0)
    ਵਾਪਸ 1;                   /* note for test coverage: this is never
                                   reached, as 'powerld' won't be called if
                                   there's no suffix, hence, no "power".  */

  /* TODO: check for overflow, inf?  */
  ਜਦੋਂ (--x)
    ਨਤੀਜਾ *= ਅਧਾਰ_2;
  ਵਾਪਸ ਨਤੀਜਾ;
}

/* Similar to 'fabs(3)' but without requiring 'libm'.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਲੰਮਾ ਦੁੱਗਣਾ
absld (ਲੰਮਾ ਦੁੱਗਣਾ val)
{
  ਵਾਪਸ val < 0 ? -val : val;
}

/* Scale down 'val', returns 'updated val' and 'x', such that
     val*base^X = original val
     Similar to "frexpl(3)" but without requiring 'libm',
     allowing only integer scale, limited functionality and error checking.  */
ਸਥਿਰ ਲੰਮਾ ਦੁੱਗਣਾ
expld (ਲੰਮਾ ਦੁੱਗਣਾ val, ਪੂਰਨ ਅਧਾਰ_2, ਪੂਰਨ /*output */ *x)
{
  ਪੂਰਨ power = 0;

  ਜੇ (val >= -LDBL_MAX && val <= LDBL_MAX)
    {
      ਜਦੋਂ (absld (val) >= ਅਧਾਰ_2)
        {
          ++power;
          val /= ਅਧਾਰ_2;
        }
    }
  ਜੇ (x)
    *x = power;
  ਵਾਪਸ val;
}

/* EXTREMELY limited 'ceil' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ intmax_t
simple_ਗੇੜਾ_ceiling (ਲੰਮਾ ਦੁੱਗਣਾ val)
{
  intmax_t intval = val;
  ਜੇ (intval < val)
    intval++;
  ਵਾਪਸ intval;
}

/* EXTREMELY limited 'floor' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ intmax_t
simple_ਗੇੜਾ_floor (ਲੰਮਾ ਦੁੱਗਣਾ val)
{
  ਵਾਪਸ -simple_ਗੇੜਾ_ceiling (-val);
}

/* EXTREMELY limited 'round away from zero'.
   Assumes values that fit in intmax_t.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ intmax_t
simple_ਗੇੜਾ_from_zero (ਲੰਮਾ ਦੁੱਗਣਾ val)
{
  ਵਾਪਸ val < 0 ? simple_ਗੇੜਾ_floor (val) : simple_ਗੇੜਾ_ceiling (val);
}

/* EXTREMELY limited 'round away to zero'.
   Assumes values that fit in intmax_t.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ intmax_t
simple_ਗੇੜਾ_ਮੰਜ਼ਿਲ_zero (ਲੰਮਾ ਦੁੱਗਣਾ val)
{
  ਵਾਪਸ val;
}

/* EXTREMELY limited 'round' - without 'libm'.
   Assumes values that fit in intmax_t.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ intmax_t
simple_ਗੇੜਾ_ਸਭ_ਤੋਂ_ਨੇੜੇ (ਲੰਮਾ ਦੁੱਗਣਾ val)
{
  ਵਾਪਸ val < 0 ? val - 0.5 : val + 0.5;
}

ATTRIBUTE_CONST
ਸਥਿਰ ਸਤਰਅੰਦਰ ਲੰਮਾ ਦੁੱਗਣਾ
simple_ਗੇੜਾ (ਲੰਮਾ ਦੁੱਗਣਾ val, ਗਿਣਤੀ ਗੇੜਾ_type t)
{
  intmax_t rval;
  intmax_t intmax_mul = val / INTMAX_MAX;
  val -= (ਲੰਮਾ ਦੁੱਗਣਾ) INTMAX_MAX * intmax_mul;

  ਚੋਣ (t)
    {
    ਹਾਲਤ ਗੇੜਾ_ceiling:
      rval = simple_ਗੇੜਾ_ceiling (val);
      ਰੋਕੋ;

    ਹਾਲਤ ਗੇੜਾ_floor:
      rval = simple_ਗੇੜਾ_floor (val);
      ਰੋਕੋ;

    ਹਾਲਤ ਗੇੜਾ_from_zero:
      rval = simple_ਗੇੜਾ_from_zero (val);
      ਰੋਕੋ;

    ਹਾਲਤ ਗੇੜਾ_ਮੰਜ਼ਿਲ_zero:
      rval = simple_ਗੇੜਾ_ਮੰਜ਼ਿਲ_zero (val);
      ਰੋਕੋ;

    ਹਾਲਤ ਗੇੜਾ_ਸਭ_ਤੋਂ_ਨੇੜੇ:
      rval = simple_ਗੇੜਾ_ਸਭ_ਤੋਂ_ਨੇੜੇ (val);
      ਰੋਕੋ;

    ਮੂਲ:
      /* to silence the compiler - this should never happen.  */
      ਵਾਪਸ 0;
    }

  ਵਾਪਸ (ਲੰਮਾ ਦੁੱਗਣਾ) INTMAX_MAX * intmax_mul + rval;
}

ਗਿਣਤੀ simple_strtod_error
{
  SSE_OK = 0,
  SSE_OK_PRECISION_LOSS,
  SSE_ਉਛਾਲ,
  SSE_INVALID_ਸੰਖਿਆ,

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
ਸਥਿਰ ਗਿਣਤੀ simple_strtod_error
simple_strtod_int (ਅੱਖਰ ਅਚਲ *ਨਿਵੇਸ਼_str,
                   ਅੱਖਰ **endptr, ਲੰਮਾ ਦੁੱਗਣਾ *ਮੁੱਲ, bool *negative)
{
  ਗਿਣਤੀ simple_strtod_error e = SSE_OK;

  ਲੰਮਾ ਦੁੱਗਣਾ val = 0;
  ਪੂਰਨ digits = 0;
  bool ਮਿਲਿਆ_digit = false;

  ਜੇ (*ਨਿਵੇਸ਼_str == '-')
    {
      ਨਿਵੇਸ਼_str++;
      *negative = true;
    }
  ਨਹੀਂਤਾਂ
    *negative = false;

  *endptr = (ਅੱਖਰ *) ਨਿਵੇਸ਼_str;
  ਜਦੋਂ (c_isdigit (**endptr))
    {
      ਪੂਰਨ digit = (**endptr) - '0';

      ਮਿਲਿਆ_digit = true;

      ਜੇ (val || digit)
        digits++;

      ਜੇ (digits > MAX_UNSCALED_DIGITS)
        e = SSE_OK_PRECISION_LOSS;

      ਜੇ (digits > MAX_ACCEPTABLE_DIGITS)
        ਵਾਪਸ SSE_ਉਛਾਲ;

      val *= 10;
      val += digit;

      ++(*endptr);

      ਜੇ (thousands_sep_ਲੰਮਾਈ > 0
          && STREQ_LEN (*endptr, thousands_sep, thousands_sep_ਲੰਮਾਈ)
          && c_isdigit ((*endptr)[thousands_sep_ਲੰਮਾਈ]))
        (*endptr) += thousands_sep_ਲੰਮਾਈ;
    }
  ਜੇ (! ਮਿਲਿਆ_digit
      && ! STREQ_LEN (*endptr, decimal_ਬਿੰਦੂ, decimal_ਬਿੰਦੂ_ਲੰਮਾਈ))
    ਵਾਪਸ SSE_INVALID_ਸੰਖਿਆ;
  ਜੇ (*negative)
    val = -val;

  ਜੇ (ਮੁੱਲ)
    *ਮੁੱਲ = val;

  ਵਾਪਸ e;
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
ਸਥਿਰ ਗਿਣਤੀ simple_strtod_error
simple_strtod_float (ਅੱਖਰ ਅਚਲ *ਨਿਵੇਸ਼_str,
                     ਅੱਖਰ **endptr,
                     ਲੰਮਾ ਦੁੱਗਣਾ *ਮੁੱਲ,
                     size_t *precision)
{
  bool negative;
  ਗਿਣਤੀ simple_strtod_error e = SSE_OK;

  ਜੇ (precision)
    *precision = 0;

  /* TODO: accept locale'd grouped values for the integral part.  */
  e = simple_strtod_int (ਨਿਵੇਸ਼_str, endptr, ਮੁੱਲ, &negative);
  ਜੇ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ਵਾਪਸ e;

  /* optional decimal point + fraction.  */
  ਜੇ (STREQ_LEN (*endptr, decimal_ਬਿੰਦੂ, decimal_ਬਿੰਦੂ_ਲੰਮਾਈ))
    {
      ਅੱਖਰ *ptr2;
      ਲੰਮਾ ਦੁੱਗਣਾ val_frac = 0;
      bool neg_frac;

      (*endptr) += decimal_ਬਿੰਦੂ_ਲੰਮਾਈ;
      ਗਿਣਤੀ simple_strtod_error e2 =
        simple_strtod_int (*endptr, &ptr2, &val_frac, &neg_frac);
      ਜੇ (e2 != SSE_OK && e2 != SSE_OK_PRECISION_LOSS)
        ਵਾਪਸ e2;
      ਜੇ (e2 == SSE_OK_PRECISION_LOSS)
        e = e2;                       /* propagate warning.  */
      ਜੇ (neg_frac)
        ਵਾਪਸ SSE_INVALID_ਸੰਖਿਆ;

      /* number of digits in the fractions.  */
      ptrdiff_t exponent = ptr2 - *endptr;

      val_frac /= powerld (10, exponent);

      /* TODO: detect loss of precision (only really 18 digits
         of precision across all digits (before and after '.')).  */
      ਜੇ (ਮੁੱਲ)
        {
          ਜੇ (negative)
            *ਮੁੱਲ -= val_frac;
          ਨਹੀਂਤਾਂ
            *ਮੁੱਲ += val_frac;
        }

      ਜੇ (precision)
        *precision = exponent;

      *endptr = ptr2;
    }
  ਵਾਪਸ e;
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
ਸਥਿਰ ਗਿਣਤੀ simple_strtod_error
simple_strtod_human (ਅੱਖਰ ਅਚਲ *ਨਿਵੇਸ਼_str,
                     ਅੱਖਰ **endptr, ਲੰਮਾ ਦੁੱਗਣਾ *ਮੁੱਲ, size_t *precision,
                     ਗਿਣਤੀ scale_type allowed_scaling)
{
  ਪੂਰਨ power = 0;
  /* 'scale_auto' is checked below.  */
  ਪੂਰਨ scale_ਅਧਾਰ = default_scale_ਅਧਾਰ (allowed_scaling);

  devmsg ("simple_strtod_human:\n  input string: %s\n"
          "  locale decimal-point: %s\n"
          "  MAX_UNSCALED_DIGITS: %d\n",
          quote_n (0, ਨਿਵੇਸ਼_str),
          quote_n (1, decimal_ਬਿੰਦੂ),
          MAX_UNSCALED_DIGITS);

  ਗਿਣਤੀ simple_strtod_error e =
    simple_strtod_float (ਨਿਵੇਸ਼_str, endptr, ਮੁੱਲ, precision);
  ਜੇ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    ਵਾਪਸ e;

  devmsg ("  parsed numeric value: %Lf\n"
          "  input precision = %d\n", *ਮੁੱਲ, (ਪੂਰਨ)*precision);

  ਜਦੋਂ (**endptr)
    {
      /* process suffix.  */

      /* Skip a single blank, NBSP or specified unit separator.
         Note an explicit empty --unit-sep should disable blank matching. */
      bool matched_unit_sep = false;
      ਜੇ (unit_separator)
        {
          size_t sep_len = ਲੜੀ_ਲੰਬਾਈ (unit_separator);
          ਜੇ (STREQ_LEN (*endptr, unit_separator, sep_len))
            {
              matched_unit_sep = true;
              (*endptr) += sep_len;
            }
        }
      ਜੇ (!matched_unit_sep)
        {
          mcel_t g = mcel_scanz (*endptr);
          ਜੇ (c32issep (g.ch) || c32isnbspace (g.ch))
            (*endptr) += g.len;
        }

      ਜੇ (**endptr == '\0')
        ਰੋਕੋ;  /* Treat as no suffix.  */

      ਜੇ (!valid_suffix (**endptr))
        {
          /* Trailing blanks are allowed.  */
          *endptr = skip_str_matching (*endptr, newline_or_blank, true);
          ਜੇ (**endptr == '\0')
            ਰੋਕੋ;

          ਵਾਪਸ SSE_INVALID_SUFFIX;
        }

      ਜੇ (allowed_scaling == scale_none)
        ਵਾਪਸ SSE_VALID_BUT_FORBIDDEN_SUFFIX;

      power = suffix_power (**endptr);
      (*endptr)++;                     /* skip first suffix character.  */

      ਜੇ (allowed_scaling == scale_auto && **endptr == 'i')
        {
          /* auto-scaling enabled, and the first suffix character
             is followed by an 'i' (e.g. Ki, Mi, Gi).  */
          scale_ਅਧਾਰ = 1024;
          (*endptr)++;              /* skip 'i' in suffix.  */
          devmsg ("  Auto-scaling, found 'i', switching to base %d\n",
                  scale_ਅਧਾਰ);
        }
      ਨਹੀਂਤਾਂ ਜੇ (allowed_scaling == scale_IEC_I)
        {
          ਜੇ (**endptr == 'i')
            (*endptr)++;
          ਨਹੀਂਤਾਂ
            ਵਾਪਸ SSE_MISSING_I_SUFFIX;
        }

      *precision = 0;  /* Reset, to select precision based on scale.  */

      /* Trailing blanks are allowed.  */
      *endptr = skip_str_matching (*endptr, newline_or_blank, true);

      ਰੋਕੋ;
    }

  ਲੰਮਾ ਦੁੱਗਣਾ multiplier = powerld (scale_ਅਧਾਰ, power);

  devmsg ("  suffix power=%d^%d = %Lf\n", scale_ਅਧਾਰ, power, multiplier);

  /* TODO: detect loss of precision and overflows.  */
  (*ਮੁੱਲ) = (*ਮੁੱਲ) * multiplier;

  devmsg ("  returning value: %Lf (%LG)\n", *ਮੁੱਲ, *ਮੁੱਲ);

  ਵਾਪਸ e;
}


ਸਥਿਰ ਖਾਲੀ
simple_strtod_fatal (ਗਿਣਤੀ simple_strtod_error err, ਅੱਖਰ ਅਚਲ *ਨਿਵੇਸ਼_str)
{
  ਅੱਖਰ ਅਚਲ *msgid = ਨਿਰਾ;

  ਚੋਣ (err)
    {
    ਹਾਲਤ SSE_ਉਛਾਲ:
      msgid = N_("value too large to be converted: %s");
      ਰੋਕੋ;

    ਹਾਲਤ SSE_INVALID_ਸੰਖਿਆ:
      msgid = N_("invalid number: %s");
      ਰੋਕੋ;

    ਹਾਲਤ SSE_VALID_BUT_FORBIDDEN_SUFFIX:
      msgid = N_("rejecting suffix in input: %s (consider using --from)");
      ਰੋਕੋ;

    ਹਾਲਤ SSE_INVALID_SUFFIX:
      msgid = N_("invalid suffix in input: %s");
      ਰੋਕੋ;

    ਹਾਲਤ SSE_MISSING_I_SUFFIX:
      msgid = N_("missing 'i' suffix in input: %s (e.g Ki/Mi/Gi)");
      ਰੋਕੋ;

    ਹਾਲਤ SSE_OK_PRECISION_LOSS:
    ਹਾਲਤ SSE_OK:
      /* should never happen - this function isn't called when OK.  */
      affirm (false);
    }

  ਜੇ (inval_style != inval_ignore)
    error (conv_exit_ਕੋਡ, 0, gettext (msgid), quote (ਨਿਵੇਸ਼_str));
}

/* Convert VAL to a human format string using PRECISION in BUF of size
   BUF_SIZE.  Use SCALE, GROUP, and ROUND to format.  Return
   the number of bytes needed to represent VAL.  If this number is not
   less than BUF_SIZE, the buffer is too small; if it is negative, the
   formatting failed for some reason.  */
ਸਥਿਰ ਪੂਰਨ
double_ਮੰਜ਼ਿਲ_human (ਲੰਮਾ ਦੁੱਗਣਾ val, ਪੂਰਨ precision,
                 ਅੱਖਰ *buf, idx_t buf_ਮਾਪ,
                 ਗਿਣਤੀ scale_type scale, ਪੂਰਨ ਸਮੂਹ, ਗਿਣਤੀ ਗੇੜਾ_type ਗੇੜਾ_2)
{
  ਅੱਖਰ fmt[ਆਕਾਰ "%'0.*Lfi%s%s%s%s" + INT_STRLEN_BOUND (zero_padding_ਚੌੜਾਈ)];
  ਅੱਖਰ *pfmt = fmt;
  *pfmt++ = '%';

  ਜੇ (ਸਮੂਹ)
    *pfmt++ = '\'';

  ਜੇ (zero_padding_ਚੌੜਾਈ)
    pfmt += sprintf (pfmt, "0%d", zero_padding_ਚੌੜਾਈ);

  devmsg ("double_to_human:\n");

  ਜੇ (scale == scale_none)
    {
      val *= powerld (10, precision);
      val = simple_ਗੇੜਾ (val, ਗੇੜਾ_2);
      val /= powerld (10, precision);

      devmsg ((ਸਮੂਹ) ?
              "  no scaling, returning (grouped) value: %'.*Lf\n" :
              "  no scaling, returning value: %.*Lf\n", precision, val);

      strcpy (pfmt, ".*Lf%s");

      ਵਾਪਸ snprintf (buf, buf_ਮਾਪ, fmt, precision, val,
                       suffix ? suffix : "");
    }

  /* Scaling requested by user. */
  ਦੁੱਗਣਾ scale_ਅਧਾਰ = default_scale_ਅਧਾਰ (scale);

  /* Normalize val to scale. */
  ਪੂਰਨ power = 0;
  val = expld (val, scale_ਅਧਾਰ, &power);
  devmsg ("  scaled value to %Lf * %0.f ^ %d\n", val, scale_ਅਧਾਰ, power);

  /* Perform rounding. */
  ਪੂਰਨ power_adjust = 0;
  ਜੇ (user_precision != -1)
    power_adjust = MIN (power * 3, user_precision);
  ਨਹੀਂਤਾਂ ਜੇ (absld (val) < 10)
    {
      /* for values less than 10, we allow one decimal-point digit,
         so adjust before rounding. */
      power_adjust = 1;
    }

  val *= powerld (10, power_adjust);
  val = simple_ਗੇੜਾ (val, ਗੇੜਾ_2);
  val /= powerld (10, power_adjust);

  /* two special cases after rounding:
     1. a "999.99" can turn into 1000 - so scale down
     2. a "9.99" can turn into 10 - so don't display decimal-point.  */
  ਜੇ (absld (val) >= scale_ਅਧਾਰ)
    {
      val /= scale_ਅਧਾਰ;
      power++;
    }

  /* should "7.0" be printed as "7" ?
     if removing the ".0" is preferred, enable the fourth condition.  */
  ਪੂਰਨ show_decimal_ਬਿੰਦੂ = (val != 0) && (absld (val) < 10) && (power > 0);
  /* && (absld (val) > simple_round_floor (val))) */

  devmsg ("  after rounding, value=%Lf * %0.f ^ %d\n", val, scale_ਅਧਾਰ, power);

  strcpy (pfmt, ".*Lf%s%s%s%s");

  ਪੂਰਨ prec = user_precision == -1 ? show_decimal_ਬਿੰਦੂ : user_precision;

  ਵਾਪਸ snprintf (buf, buf_ਮਾਪ, fmt, prec, val,
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
ਸਥਿਰ uintmax_t
unit_ਮੰਜ਼ਿਲ_umax (ਅੱਖਰ ਅਚਲ *n_string)
{
  strtol_error s_err;
  ਅੱਖਰ ਅਚਲ *c_string = n_string;
  ਅੱਖਰ *t_string = ਨਿਰਾ;
  size_t n_len = ਲੜੀ_ਲੰਬਾਈ (n_string);
  ਅੱਖਰ *ਸਿਰਾ = ਨਿਰਾ;
  uintmax_t n;
  ਅੱਖਰ ਅਚਲ *suffixes = valid_suffixes;

  /* Adjust suffixes so K=1000, Ki=1024, KiB=invalid.  */
  ਜੇ (n_len && ! c_isdigit (n_string[n_len - 1]))
    {
      t_string = xmalloc (n_len + 2);
      ਸਿਰਾ = t_string + n_len - 1;
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (t_string, n_string, n_len);

      ਜੇ (*ਸਿਰਾ == 'i' && 2 <= n_len && ! c_isdigit (*(ਸਿਰਾ - 1)))
        *ਸਿਰਾ = '\0';
      ਨਹੀਂਤਾਂ
        {
          *++ਸਿਰਾ = 'B';
          *++ਸਿਰਾ = '\0';
          suffixes = zero_and_valid_suffixes;
        }

      c_string = t_string;
    }

  s_err = xstrtoumax (c_string, &ਸਿਰਾ, 10, &n, suffixes);

  ਜੇ (s_err != LONGINT_OK || *ਸਿਰਾ || n == 0)
    {
      ਮੁਕਤ (t_string);
      error (EXIT_FAILURE, 0, _("invalid unit size: %s"), quote (n_string));
    }

  ਮੁਕਤ (t_string);

  ਵਾਪਸ n;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [ਸੰਖਿਆ_2]...\n\
"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Reformat ਸੰਖਿਆ_2(s), or the numbers from standard ਨਿਵੇਸ਼ ਜੇ none are specified.\n\
"), stdout);
      emit_mandatory_arg_note ();
      oputs (_("\
      --debug\n\
         print warnings about invalid ਨਿਵੇਸ਼\n\
"));
      oputs (_("\
  -d, --delimiter=X\n\
         use X instead ਦਾ whitespace ਲਈ ਖੇਤਰ delimiter\n\
"));
      oputs (_("\
      --ਖੇਤਰ=FIELDS\n\
         replace the numbers ਵਿੱਚ these ਨਿਵੇਸ਼ fields (ਮੂਲ=1);\n\
         see FIELDS ਹੇਠਾਂ ਲਈ details\n\
"));
      oputs (_("\
      --format=FORMAT\n\
         use ਲਿਖੋ style floating-ਬਿੰਦੂ FORMAT;\n\
         see FORMAT ਹੇਠਾਂ ਲਈ details\n\
"));
      oputs (_("\
      --from=UNIT\n\
         ਸਵੈਚਾਲਿਤ-scale ਨਿਵੇਸ਼ numbers ਮੰਜ਼ਿਲ UNITs; ਮੂਲ is 'none';\n\
         see UNIT ਹੇਠਾਂ ਲਈ details\n\
"));
      oputs (_("\
      --from-unit=N\n\
         specify the ਨਿਵੇਸ਼ unit ਮਾਪ (instead ਦਾ the ਮੂਲ 1)\n\
"));
      oputs (_("\
      --grouping\n\
         use locale-defined grouping ਦਾ digits, e.g. 1,000,000.\n\
         This has no effect ਵਿੱਚ the C/POSIX locale\n\
"));
      oputs (_("\
      --ਸਿਰਲੇਖ[=N]\n\
         print (without converting) the ਪਹਿਲਾ N ਸਿਰਲੇਖ ਸਤਰਾਂ;\n\
         N defaults ਮੰਜ਼ਿਲ 1 ਜੇ not specified\n\
"));
      oputs (_("\
      --invalid=ਬਹੁਲਕ\n\
         failure ਬਹੁਲਕ_2 ਲਈ invalid numbers;\n\
         ਬਹੁਲਕ can be: abort (ਮੂਲ), fail, warn, ignore\n\
"));
      oputs (_("\
      --padding=N\n\
         pad the output ਮੰਜ਼ਿਲ N characters;\n\
         ਸਕਾਰਾਤਮਕ N will ਸੱਜਾ-align, negative N will ਖੱਬਾ-align;\n\
         padding is ignored ਜੇ the output is wider than N;\n\
         the ਮੂਲ is ਮੰਜ਼ਿਲ automatically pad ਜੇ a whitespace is ਮਿਲਿਆ\n\
"));
      oputs (_("\
      --ਗੇੜਾ_2=ਵਿਧੀ\n\
         use ਵਿਧੀ ਲਈ rounding when scaling; ਵਿਧੀ can be:\n\
         ਉੱਪਰ, ਥੱਲੇ, from-zero (ਮੂਲ), towards-zero, ਸਭ_ਤੋਂ_ਨੇੜੇ\n\
"));
      oputs (_("\
      --suffix=SUFFIX\n\
         ਜੋੜੋ SUFFIX ਮੰਜ਼ਿਲ output numbers,\n\
         and accept an optional SUFFIX ਵਿੱਚ ਨਿਵੇਸ਼ numbers\n\
"));
      oputs (_("\
      --unit-separator=SEP\n\
         ਪਾਓ SEP between ਸੰਖਿਆ and unit on output,\n\
         and accept an optional SEP ਵਿੱਚ ਨਿਵੇਸ਼ numbers\n\
"));
      oputs (_("\
      --ਮੰਜ਼ਿਲ=UNIT\n\
         ਸਵੈਚਾਲਿਤ-scale output numbers ਮੰਜ਼ਿਲ UNITs; see UNIT ਹੇਠਾਂ\n\
"));
      oputs (_("\
      --ਮੰਜ਼ਿਲ-unit=N\n\
         the output unit ਮਾਪ (instead ਦਾ the ਮੂਲ 1)\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ਸਤਰ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);

      fputs (_("\
\n\
UNIT options:\n"), stdout);
      fputs (_("\
  none       no ਸਵੈਚਾਲਿਤ-scaling is ਪੂਰਾ; suffixes will trigger an error\n\
"), stdout);
      fputs (_("\
  ਸਵੈਚਾਲਿਤ       accept optional single/two letter suffix:\n\
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
FIELDS supports cut(1) style ਖੇਤਰ ranges:\n\
  N    N'th ਖੇਤਰ, counted from 1\n\
  N-   from N'th ਖੇਤਰ, ਮੰਜ਼ਿਲ ਸਿਰਾ ਦਾ ਸਤਰ\n\
  N-M  from N'th to M'th ਖੇਤਰ (inclusive)\n\
  -M   from ਪਹਿਲਾ ਮੰਜ਼ਿਲ M'th ਖੇਤਰ (inclusive)\n\
  -    all fields\n\
ਗੁਣਜ_2 fields/ranges can be separated with commas\n\
"), stdout);

      fputs (_("\n\
FORMAT must be suitable ਲਈ printing one floating-ਬਿੰਦੂ ਦਲੀਲ '%f'.\n\
Optional quote (%'f) will enable --grouping (ਜੇ supported by current locale).\n\
Optional ਚੌੜਾਈ ਮੁੱਲ (%10f) will pad output. Optional zero (%010f) ਚੌੜਾਈ\n\
will zero pad the ਸੰਖਿਆ. Optional negative ਮੁੱਲਾਂ (%-10f) will ਖੱਬਾ align.\n\
Optional precision (%.1f) will override the ਨਿਵੇਸ਼ determined precision.\n\
"), stdout);

      ਲਿਖੋ (_("\n\
Exit ਸਥਿਤੀ is 0 ਜੇ all ਨਿਵੇਸ਼ numbers were successfully converted.\n\
By ਮੂਲ, %s will stop at the ਪਹਿਲਾ conversion error with ਬਾਹਰ_ਨਿਕਲੋ ਸਥਿਤੀ 2.\n\
With --invalid='fail' a warning is printed ਲਈ each conversion error\n\
and the ਬਾਹਰ_ਨਿਕਲੋ ਸਥਿਤੀ is 2.  With --invalid='warn' each conversion error is\n\
diagnosed, but the ਬਾਹਰ_ਨਿਕਲੋ ਸਥਿਤੀ is 0.  With --invalid='ignore' conversion\n\
errors are not diagnosed and the ਬਾਹਰ_ਨਿਕਲੋ ਸਥਿਤੀ is 0.\n\
"), ਪ੍ਰੋਗਰਾਮ_name);

      ਲਿਖੋ (_("\n\
Examples:\n\
  $ %s --ਮੰਜ਼ਿਲ=si 1000\n\
            -> \"1.0k\"\n\
  $ %s --ਮੰਜ਼ਿਲ=iec 2048\n\
           -> \"2.0K\"\n\
  $ %s --ਮੰਜ਼ਿਲ=iec-i 4096\n\
           -> \"4.0Ki\"\n\
  $ echo 1K | %s --from=si\n\
           -> \"1000\"\n\
  $ echo 1K | %s --from=iec\n\
           -> \"1024\"\n\
  $ df -B1 | %s --ਸਿਰਲੇਖ --ਖੇਤਰ 2-4 --ਮੰਜ਼ਿਲ=si\n\
  $ ls -l  | %s --ਸਿਰਲੇਖ --ਖੇਤਰ 5 --ਮੰਜ਼ਿਲ=iec\n\
  $ ls -lh | %s --ਸਿਰਲੇਖ --ਖੇਤਰ 5 --from=iec --padding=10\n\
  $ ls -lh | %s --ਸਿਰਲੇਖ --ਖੇਤਰ 5 --from=iec --format %%10f\n"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name,
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name,
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
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
ਸਥਿਰ ਖਾਲੀ
parse_format_string (ਅੱਖਰ ਅਚਲ *fmt)
{
  size_t i;
  size_t ਅਗੇਤਰ_len = 0;
  size_t suffix_pos;
  ਅੱਖਰ *endptr = ਨਿਰਾ;
  bool zero_padding = false;

  ਲਈ (i = 0; !(fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      ਜੇ (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      ਅਗੇਤਰ_len++;
    }

  i++;
  ਜਦੋਂ (true)
    {
      size_t skip = strspn (fmt + i, " ");
      i += skip;
      ਜੇ (fmt[i] == '\'')
        {
          grouping = 1;
          i++;
        }
      ਨਹੀਂਤਾਂ ਜੇ (fmt[i] == '0')
        {
          zero_padding = true;
          i++;
        }
      ਨਹੀਂਤਾਂ ਜੇ (! skip)
        ਰੋਕੋ;
    }

  intmax_t pad = strtoimax (fmt + i, &endptr, 10);

  ਜੇ (pad != 0)
    {
      ਜੇ (debug && padding_ਚੌੜਾਈ && !(zero_padding && pad > 0))
        error (0, 0, _("--format padding overriding --padding"));

      /* Set padding width and alignment.  On overflow, set widths to
         large values that cause later code to avoid undefined behavior
         and fail at a reasonable point.  */
      ਜੇ (pad < 0)
        padding_ਚੌੜਾਈ = pad;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (zero_padding)
            zero_padding_ਚੌੜਾਈ = MIN (pad, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
          ਨਹੀਂਤਾਂ
            padding_ਚੌੜਾਈ = pad;
        }
    }
  i = endptr - fmt;

  ਜੇ (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));

  ਜੇ (fmt[i] == '.')
    {
      i++;
      errno = 0;
      user_precision = strtol (fmt + i, &endptr, 10);
      ਜੇ (errno == ERANGE || user_precision < 0 || SIZE_MAX < user_precision
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

  ਜੇ (fmt[i] != 'f')
    error (EXIT_FAILURE, 0, _("invalid format %s,"
                              " directive must be %%[0]['][-][N][.][N]f"),
         quote (fmt));
  i++;
  suffix_pos = i;

  ਲਈ (; fmt[i] != '\0'; i += (fmt[i] == '%') + 1)
    ਜੇ (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));

  ਜੇ (ਅਗੇਤਰ_len)
    format_str_ਅਗੇਤਰ = ximemdup0 (fmt, ਅਗੇਤਰ_len);
  ਜੇ (fmt[suffix_pos] != '\0')
    format_str_suffix = xstrdup (fmt + suffix_pos);

  devmsg ("format String:\n  input: %s\n  grouping: %s\n"
                   "  padding width: %jd\n"
                   "  prefix: %s\n  suffix: %s\n",
          quote_n (0, fmt), (grouping) ? "yes" : "no",
          padding_ਚੌੜਾਈ,
          quote_n (1, format_str_ਅਗੇਤਰ ? format_str_ਅਗੇਤਰ : ""),
          quote_n (2, format_str_suffix ? format_str_suffix : ""));
}

/* Parse a numeric value (with optional suffix) from a string.
   Returns a long double value, with input precision.

   If there's an error converting the string to value - exits with
   an error.

   If there are any trailing characters after the number
   (besides a valid suffix) - exits with an error.  */
ਸਥਿਰ ਗਿਣਤੀ simple_strtod_error
parse_human_ਸੰਖਿਆ (ਅੱਖਰ ਅਚਲ *str, ਲੰਮਾ ਦੁੱਗਣਾ /*output */ *ਮੁੱਲ,
                    size_t *precision)
{
  ਅੱਖਰ *ptr = ਨਿਰਾ;

  ਗਿਣਤੀ simple_strtod_error e =
    simple_strtod_human (str, &ptr, ਮੁੱਲ, precision, scale_from);
  ਜੇ (e != SSE_OK && e != SSE_OK_PRECISION_LOSS)
    {
      simple_strtod_fatal (e, str);
      ਵਾਪਸ e;
    }

  ਜੇ (ptr && *ptr != '\0')
    {
      ਜੇ (inval_style != inval_ignore)
        error (conv_exit_ਕੋਡ, 0, _("invalid suffix in input %s: %s"),
               quote_n (0, str), quote_n (1, ptr));
      e = SSE_INVALID_SUFFIX;
    }
  ਵਾਪਸ e;
}


/* Print the given VAL, using the requested representation.
   The number is printed to STDOUT, with padding and alignment.  */
ਸਥਿਰ bool
prepare_padded_ਸੰਖਿਆ (ਅਚਲ ਲੰਮਾ ਦੁੱਗਣਾ val, size_t precision,
                       intmax_t *padding)
{
  /* Generate Output. */
  size_t precision_ਵਰਤਿਆ = user_precision == -1 ? precision : user_precision;

  /* Can't reliably print too-large values without auto-scaling. */
  ਪੂਰਨ x;
  expld (val, 10, &x);

  ਜੇ (scale_ਮੰਜ਼ਿਲ == scale_none
      && x + precision_ਵਰਤਿਆ > MAX_UNSCALED_DIGITS)
    {
      ਜੇ (inval_style != inval_ignore)
        {
          ਜੇ (precision_ਵਰਤਿਆ)
            error (conv_exit_ਕੋਡ, 0,
                   _("value/precision too large to be printed: '%Lg/%zu'"
                     " (consider using --to)"), val, precision_ਵਰਤਿਆ);
          ਨਹੀਂਤਾਂ
            error (conv_exit_ਕੋਡ, 0,
                   _("value too large to be printed: '%Lg'"
                     " (consider using --to)"), val);
        }
      ਵਾਪਸ false;
    }

  ਜੇ (x > MAX_ACCEPTABLE_DIGITS - 1)
    {
      ਜੇ (inval_style != inval_ignore)
        error (conv_exit_ਕੋਡ, 0, _("value too large to be printed: '%Lg'"
                                    " (cannot handle values > 999Q)"), val);
      ਵਾਪਸ false;
    }

  ਜਦੋਂ (true)
    {
      ਪੂਰਨ numlen = double_ਮੰਜ਼ਿਲ_human (val, precision_ਵਰਤਿਆ,
                                    padding_ਬਫਰ, padding_ਬਫਰ_ਮਾਪ,
                                    scale_ਮੰਜ਼ਿਲ, grouping, ਗੇੜਾ_style);
      ptrdiff_t growth;
      ਜੇ (numlen < 0 || ckd_sub (&growth, numlen, padding_ਬਫਰ_ਮਾਪ - 1))
        error (EXIT_FAILURE, 0,
               _("failed to prepare value '%Lf' for printing"), val);
      ਜੇ (growth <= 0)
        ਰੋਕੋ;
      padding_ਬਫਰ = xpalloc (padding_ਬਫਰ, &padding_ਬਫਰ_ਮਾਪ,
                                growth, -1, 1);
    }

  devmsg ("formatting output:\n  value: %Lf\n  humanized: %s\n",
          val, quote (padding_ਬਫਰ));

  intmax_t pad = 0;
  ਜੇ (padding_ਚੌੜਾਈ)
    {
      ਪੂਰਨ buf_ਚੌੜਾਈ = mbswidth (padding_ਬਫਰ,
                                MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE);
      ਜੇ (0 <= buf_ਚੌੜਾਈ)
        {
          ਜੇ (padding_ਚੌੜਾਈ < 0)
            {
              ਜੇ (padding_ਚੌੜਾਈ < -buf_ਚੌੜਾਈ)
                pad = padding_ਚੌੜਾਈ + buf_ਚੌੜਾਈ;
            }
          ਨਹੀਂਤਾਂ
            {
              ਜੇ (buf_ਚੌੜਾਈ < padding_ਚੌੜਾਈ)
                pad = padding_ਚੌੜਾਈ - buf_ਚੌੜਾਈ;
            }
        }
    }

  *padding = pad;
  ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ
print_padded_ਸੰਖਿਆ (intmax_t padding)
{
  ਜੇ (format_str_ਅਗੇਤਰ)
    fputs (format_str_ਅਗੇਤਰ, stdout);

  ਲਈ (intmax_t p = padding; 0 < p; p--)
    ਅੱਖਰ_ਲਿਖੋ (' ');

  fputs (padding_ਬਫਰ, stdout);

  ਲਈ (intmax_t p = padding; p < 0; p++)
    ਅੱਖਰ_ਲਿਖੋ (' ');

  ਜੇ (format_str_suffix)
    fputs (format_str_suffix, stdout);
}

/* Converts the TEXT number string to the requested representation,
   and handles automatic suffix addition.  */
ਸਥਿਰ bool
process_suffixed_ਸੰਖਿਆ (ਅੱਖਰ *ਪਾਠ, ਲੰਮਾ ਦੁੱਗਣਾ *ਨਤੀਜਾ,
                         size_t *precision, ਲੰਮਾ ਪੂਰਨ ਖੇਤਰ)
{
  ਅੱਖਰ saved_suffix = '\0';

  ਜੇ (suffix)
    {
      ਜੇ (mbs_endswith (ਪਾਠ, suffix))
        {
          saved_suffix = *(ਪਾਠ + ਲੜੀ_ਲੰਬਾਈ (ਪਾਠ) - ਲੜੀ_ਲੰਬਾਈ (suffix));
          *(ਪਾਠ + ਲੜੀ_ਲੰਬਾਈ (ਪਾਠ) - ਲੜੀ_ਲੰਬਾਈ (suffix)) = '\0';
          devmsg ("trimming suffix %s\n", quote (suffix));
        }
      ਨਹੀਂਤਾਂ
        devmsg ("no valid suffix found\n");
    }

  /* Skip blanks - always.  */
  ਅੱਖਰ *p = skip_str_matching (ਪਾਠ, newline_or_blank, true);

  /* setup auto-padding.  */
  ਜੇ (auto_padding)
    {
      padding_ਚੌੜਾਈ = ਪਾਠ < p || 1 < ਖੇਤਰ
                      ? mbswidth (ਪਾਠ,
                                  MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE)
                      : 0;
      ਜੇ (padding_ਚੌੜਾਈ < 0)
        padding_ਚੌੜਾਈ = ਲੜੀ_ਲੰਬਾਈ (ਪਾਠ);
      devmsg ("setting Auto-Padding to %jd characters\n", padding_ਚੌੜਾਈ);
    }

  ਲੰਮਾ ਦੁੱਗਣਾ val = 0;
  ਗਿਣਤੀ simple_strtod_error e = parse_human_ਸੰਖਿਆ (p, &val, precision);
  ਜੇ (e == SSE_OK_PRECISION_LOSS && debug)
    error (0, 0, _("large input value %s: possible precision loss"),
           quote (p));

  ਜੇ (from_unit_ਮਾਪ != 1 || ਮੰਜ਼ਿਲ_unit_ਮਾਪ != 1)
    val = (val * from_unit_ਮਾਪ) / ਮੰਜ਼ਿਲ_unit_ਮਾਪ;

  *ਨਤੀਜਾ = val;

  ਜੇ (e == SSE_OK || e == SSE_OK_PRECISION_LOSS)
    ਵਾਪਸ true;
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (saved_suffix)
        *(ਪਾਠ + ਲੜੀ_ਲੰਬਾਈ (ਪਾਠ)) = saved_suffix;
      ਵਾਪਸ false;
    }
}

/* Search for multi-byte character C in multi-byte string S.
   Return a pointer to the character, or NULL if not found.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਅੱਖਰ *
mbsmbchr (ਅੱਖਰ ਅਚਲ *s, ਅੱਖਰ ਅਚਲ *c)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ uc = *c;
   /* GB18030 is the most restrictive for the 0x30 optimization below.  */
  ਜੇ (uc < 0x30 || MB_CUR_MAX == 1)
    ਵਾਪਸ (ਅੱਖਰ *) strchr (s, uc);
  ਨਹੀਂਤਾਂ ਜੇ (is_utf8_charset ())
    ਵਾਪਸ (ਅੱਖਰ *) (uc < 0x80 ? strchr (s, uc) : ਲੜੀ_ਖੋਜ (s, c));
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ *(c + 1) == '\0' ? mbschr (s, uc) : (ਅੱਖਰ *) mbsstr (s, c);
}

/* Return a pointer to the beginning of the next field in line.
   The line pointer is moved to the end of the next field. */
ਸਥਿਰ ਅੱਖਰ *
ਅਗਲਾ_ਖੇਤਰ (ਅੱਖਰ **ਸਤਰ)
{
  ਅੱਖਰ *ਖੇਤਰ_ਸ਼ੁਰੂ = *ਸਤਰ;
  ਅੱਖਰ *ਖੇਤਰ_ਸਿਰਾ   = ਖੇਤਰ_ਸ਼ੁਰੂ;

  ਜੇ (delimiter)
    {
      ਜੇ (! (ਖੇਤਰ_ਸਿਰਾ = mbsmbchr (ਖੇਤਰ_ਸ਼ੁਰੂ, delimiter)))
        ਖੇਤਰ_ਸਿਰਾ = strchr (ਖੇਤਰ_ਸ਼ੁਰੂ, '\0');
    }
  ਨਹੀਂਤਾਂ
    {
      /* keep any space prefix in the returned field */
      ਖੇਤਰ_ਸਿਰਾ = skip_str_matching (ਖੇਤਰ_ਸਿਰਾ, newline_or_blank, true);
      ਖੇਤਰ_ਸਿਰਾ = skip_str_matching (ਖੇਤਰ_ਸਿਰਾ, newline_or_blank, false);
    }

  *ਸਤਰ = ਖੇਤਰ_ਸਿਰਾ;
  ਵਾਪਸ ਖੇਤਰ_ਸ਼ੁਰੂ;
}

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
include_ਖੇਤਰ (uintmax_t ਖੇਤਰ)
{
  ਬਣਤਰ ਖੇਤਰ_range_pair *p = frp;
  ਜੇ (!p)
    ਵਾਪਸ ਖੇਤਰ == 1;

  ਜਦੋਂ (p->lo != UINTMAX_MAX)
    {
      ਜੇ (p->lo <= ਖੇਤਰ && p->hi >= ਖੇਤਰ)
        ਵਾਪਸ true;
      ++p;
    }
  ਵਾਪਸ false;
}

/* Convert and output the given field. If it is not included in the set
   of fields to process just output the original */
ਸਥਿਰ bool
process_ਖੇਤਰ (ਅੱਖਰ *ਪਾਠ, uintmax_t ਖੇਤਰ)
{
  ਲੰਮਾ ਦੁੱਗਣਾ val = 0;
  size_t precision = 0;
  bool valid_ਸੰਖਿਆ = true;

  ਜੇ (include_ਖੇਤਰ (ਖੇਤਰ))
    {
      valid_ਸੰਖਿਆ =
        process_suffixed_ਸੰਖਿਆ (ਪਾਠ, &val, &precision, ਖੇਤਰ);

      intmax_t padding;
      ਜੇ (valid_ਸੰਖਿਆ)
        valid_ਸੰਖਿਆ = prepare_padded_ਸੰਖਿਆ (val, precision, &padding);

      ਜੇ (valid_ਸੰਖਿਆ)
        print_padded_ਸੰਖਿਆ (padding);
      ਨਹੀਂਤਾਂ
        fputs (ਪਾਠ, stdout);
    }
  ਨਹੀਂਤਾਂ
    fputs (ਪਾਠ, stdout);

  ਵਾਪਸ valid_ਸੰਖਿਆ;
}

/* Convert number in a given line of text.
   NEWLINE specifies whether to output a '\n' for this "line".  */
ਸਥਿਰ ਪੂਰਨ
process_ਸਤਰ (ਅੱਖਰ *ਸਤਰ, bool newline)
{
  ਅੱਖਰ *ਅਗਲਾ;
  uintmax_t ਖੇਤਰ = 0;
  bool valid_ਸੰਖਿਆ = true;

  ਜਦੋਂ (true) {
    ++ਖੇਤਰ;
    ਅਗਲਾ = ਅਗਲਾ_ਖੇਤਰ (&ਸਤਰ);

    ਜੇ (*ਸਤਰ != '\0')
      {
        /* NUL terminate the current field string and process */
        ਅੱਖਰ ਸਿਰਾ_ਖੇਤਰ = *ਸਤਰ;
        *ਸਤਰ = '\0';

        ਜੇ (! process_ਖੇਤਰ (ਅਗਲਾ, ਖੇਤਰ))
          valid_ਸੰਖਿਆ = false;

        ਜੇ (delimiter != ਨਿਰਾ)
          fputs (delimiter, stdout);
        ਨਹੀਂਤਾਂ
          fputc (' ', stdout);

        ਜੇ (delimiter)
          ਸਤਰ += MAX (ਲੜੀ_ਲੰਬਾਈ (delimiter), 1);
        ਨਹੀਂਤਾਂ
          {
            *ਸਤਰ = ਸਿਰਾ_ਖੇਤਰ;
            mcel_t g = mcel_scanz (ਸਤਰ);
            ਸਤਰ += g.len;
          }
      }
    ਨਹੀਂਤਾਂ
      {
        /* end of the line, process the last field and finish */
        ਜੇ (! process_ਖੇਤਰ (ਅਗਲਾ, ਖੇਤਰ))
          valid_ਸੰਖਿਆ = false;

        ਰੋਕੋ;
      }
  }

  ਜੇ (newline)
    ਅੱਖਰ_ਲਿਖੋ (ਸਤਰ_delim);

 ਜੇ (ferror (stdout))
   write_error ();

  ਵਾਪਸ valid_ਸੰਖਿਆ;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ valid_numbers = 1;
  bool locale_ok;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

#ਜੇ HAVE_FPSETPREC
  /* Enabled extended precision if needed.  */
  fpsetprec (FP_PE);
#ਜੇ_ਅੰਤ

  decimal_ਬਿੰਦੂ = nl_langinfo (RADIXCHAR);
  ਜੇ (decimal_ਬਿੰਦੂ == ਨਿਰਾ || ਲੜੀ_ਲੰਬਾਈ (decimal_ਬਿੰਦੂ) == 0)
    decimal_ਬਿੰਦੂ = ".";
  decimal_ਬਿੰਦੂ_ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ (decimal_ਬਿੰਦੂ);

  thousands_sep = nl_langinfo (THOUSEP);
  ਜੇ (thousands_sep == ਨਿਰਾ)
    thousands_sep = "";
  thousands_sep_ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ (thousands_sep);

  atexit (close_stdout);

  ਜਦੋਂ (true)
    {
      ਪੂਰਨ c = getopt_long (argc, argv, "d:z", longopts, ਨਿਰਾ);

      ਜੇ (c == -1)
        ਰੋਕੋ;

      ਚੋਣ (c)
        {
        ਹਾਲਤ FROM_OPTION:
          scale_from = XARGMATCH ("--from", optarg,
                                  scale_from_ਦਲੀਲਾਂ, scale_from_types);
          ਰੋਕੋ;

        ਹਾਲਤ FROM_UNIT_OPTION:
          from_unit_ਮਾਪ = unit_ਮੰਜ਼ਿਲ_umax (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ ਮੰਜ਼ਿਲ_OPTION:
          scale_ਮੰਜ਼ਿਲ =
            XARGMATCH ("--to", optarg, scale_ਮੰਜ਼ਿਲ_ਦਲੀਲਾਂ, scale_ਮੰਜ਼ਿਲ_types);
          ਰੋਕੋ;

        ਹਾਲਤ ਮੰਜ਼ਿਲ_UNIT_OPTION:
          ਮੰਜ਼ਿਲ_unit_ਮਾਪ = unit_ਮੰਜ਼ਿਲ_umax (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ ਗੇੜਾ_OPTION:
          ਗੇੜਾ_style = XARGMATCH ("--round", optarg, ਗੇੜਾ_ਦਲੀਲਾਂ, ਗੇੜਾ_types);
          ਰੋਕੋ;

        ਹਾਲਤ GROUPING_OPTION:
          grouping = 1;
          ਰੋਕੋ;

        ਹਾਲਤ PADDING_OPTION:
          ਜੇ (((xstrtoimax (optarg, ਨਿਰਾ, 10, &padding_ਚੌੜਾਈ, "")
                & ~LONGINT_ਉਛਾਲ)
               != LONGINT_OK)
              || padding_ਚੌੜਾਈ == 0)
            error (EXIT_FAILURE, 0, _("invalid padding value %s"),
                   quote (optarg));
          /* TODO: We probably want to apply a specific --padding
             to --header lines too.  */
          ਰੋਕੋ;

        ਹਾਲਤ ਖੇਤਰ_OPTION:
          ਜੇ (n_frp)
            error (EXIT_FAILURE, 0, _("multiple field specifications"));
          ਸੈੱਟ_fields (optarg, SETFLD_ALLOW_DASH);
          ਰੋਕੋ;

        ਹਾਲਤ 'd':
          /* Interpret -d '' to mean 'use the NUL byte as the delimiter.'  */
          ਜੇ (optarg[0] != '\0')
            {
              mcel_t g = mcel_scanz (optarg);
              /* Note we always allow single bytes, especially since mcel
                 explicitly does not avoid https://sourceware.org/PR29511
                 I.e., we ignore g.err, and rely on g.len==1 with g.err.  */
              ਜੇ (optarg[g.len] != '\0')
                error (EXIT_FAILURE, 0,
                       _("the delimiter must be a single character"));
            }
          delimiter = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'z':
          ਸਤਰ_delim = '\0';
          ਰੋਕੋ;

        ਹਾਲਤ SUFFIX_OPTION:
          suffix = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ UNIT_SEPARATOR_OPTION:
          unit_separator = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ DEBUG_OPTION:
          debug = true;
          ਰੋਕੋ;

        ਹਾਲਤ DEV_DEBUG_OPTION:
          dev_debug = true;
          debug = true;
          ਰੋਕੋ;

        ਹਾਲਤ ਸਿਰਲੇਖ_OPTION:
          ਜੇ (optarg)
            {
              ਜੇ (xstrtoumax (optarg, ਨਿਰਾ, 10, &ਸਿਰਲੇਖ, "") != LONGINT_OK
                  || ਸਿਰਲੇਖ == 0)
                error (EXIT_FAILURE, 0, _("invalid header value %s"),
                       quote (optarg));
            }
          ਨਹੀਂਤਾਂ
            {
              ਸਿਰਲੇਖ = 1;
            }
          ਰੋਕੋ;

        ਹਾਲਤ FORMAT_OPTION:
          format_str = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ INVALID_OPTION:
          inval_style = XARGMATCH ("--invalid", optarg,
                                   inval_ਦਲੀਲਾਂ, inval_types);
          ਰੋਕੋ;

          case_GETOPT_HELP_CHAR;
          case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (format_str != ਨਿਰਾ && grouping)
    error (EXIT_FAILURE, 0, _("--grouping cannot be combined with --format"));

  ਜੇ (debug && ! locale_ok)
    error (0, 0, _("failed to set locale"));

  /* Warn about no-op.  */
  ਜੇ (debug && scale_from == scale_none && scale_ਮੰਜ਼ਿਲ == scale_none
      && !grouping && (padding_ਚੌੜਾਈ == 0) && (format_str == ਨਿਰਾ))
    error (0, 0, _("no conversion option specified"));

  ਜੇ (debug && unit_separator && delimiter == ਨਿਰਾ)
    error (0, 0,
           _("field delimiters have higher precedence than unit separators"));

  ਜੇ (format_str)
    parse_format_string (format_str);

  ਜੇ (grouping)
    {
      ਜੇ (scale_ਮੰਜ਼ਿਲ != scale_none)
        error (EXIT_FAILURE, 0, _("grouping cannot be combined with --to"));
      ਜੇ (debug && thousands_sep_ਲੰਮਾਈ == 0)
        error (0, 0, _("grouping has no effect in this locale"));
    }

  auto_padding = (padding_ਚੌੜਾਈ == 0 && delimiter == ਨਿਰਾ);

  ਜੇ (inval_style != inval_abort)
    conv_exit_ਕੋਡ = 0;

  ਜੇ (argc > optind)
    {
      ਜੇ (debug && ਸਿਰਲੇਖ)
        error (0, 0, _("--header ignored with command-line input"));

      ਲਈ (; optind < argc; optind++)
        valid_numbers &= process_ਸਤਰ (argv[optind], true);
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ *ਸਤਰ = ਨਿਰਾ;
      size_t ਸਤਰ_allocated = 0;
      ssize_t len;

      ਜਦੋਂ (ਸਿਰਲੇਖ-- && getdelim (&ਸਤਰ, &ਸਤਰ_allocated,
                                   ਸਤਰ_delim, stdin) > 0)
        {
          ਜੇ (fputs (ਸਤਰ, stdout) == EOF)
            write_error ();
        }

      ਜਦੋਂ ((len = getdelim (&ਸਤਰ, &ਸਤਰ_allocated,
                              ਸਤਰ_delim, stdin)) > 0)
        {
          bool newline = ਸਤਰ[len - 1] == ਸਤਰ_delim;
          ਜੇ (newline)
            ਸਤਰ[len - 1] = '\0';
          valid_numbers &= process_ਸਤਰ (ਸਤਰ, newline);
        }

      ਜੇ (ferror (stdin))
        error (EXIT_FAILURE, errno, _("error reading input"));
    }

  ਜੇ (debug && !valid_numbers)
    error (0, 0, _("failed to convert some of the input numbers"));

  ਪੂਰਨ exit_ਸਥਿਤੀ = EXIT_SUCCESS;
  ਜੇ (!valid_numbers
      && inval_style != inval_warn && inval_style != inval_ignore)
    exit_ਸਥਿਤੀ = EXIT_CONVERSION_WARNINGS;

  main_exit (exit_ਸਥਿਤੀ);
}
