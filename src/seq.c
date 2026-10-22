/* seq - print sequence of numbers to standard output.
   Copyright (C) 1994-2026 Free Software Foundation, Inc.

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

/* Written by Ulrich Drepper.  */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "cl-strtod.h"
#ಅನ್ತರ್ಭಾವ "full-write.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xstrtod.h"
#ಅನ್ತರ್ಭಾವ "xvasprintf.h"

/* Roll our own isfinite/isnan rather than using <math.h>, so that we don't
   have to worry about linking -lm just for isfinite.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ isfinite
# define isfinite(x) ((x) * 0 == 0)
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ isnan
# define isnan(x) ((x) != (x))
#ಆದರೆ_ಅಂತ್ಯ

/* Limit below which seq_fast has more throughput.
   Determined with: seq 0 200 inf | pv > /dev/null  */
#ವ್ಯಾಖ್ಯಾನಿಸು SEQ_FAST_ಹೆಜ್ಜೆ_ಎಲ್ಲೆ 200  /* Keep in sync with texinfo description.  */
#ವ್ಯಾಖ್ಯಾನಿಸು SEQ_FAST_ಹೆಜ್ಜೆ_ಎಲ್ಲೆ_DIGITS 3

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "seq"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Ulrich Drepper")

/* True if the locale settings were honored.  */
ಸ್ಥಿರ bool locale_ok;

/* If true print all number with equal width.  */
ಸ್ಥಿರ bool equal_ಅಗಲ;

/* The string used to separate two numbers.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *separator = "\n";

/* The string output after all numbers have been output.
   Usually "\n" or "\0".  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ terminator[] = "\n";

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  { "equal-width", no_ವಾದ, ಖಾಲಿ, 'w'},
  { "format", required_ವಾದ, ಖಾಲಿ, 'f'},
  { "separator", required_ವಾದ, ಖಾಲಿ, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  { ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... ಕೊನೆಯ_3\n\
  or:  %s [OPTION]... ಮೊದಲ_3 ಕೊನೆಯ_3\n\
  or:  %s [OPTION]... ಮೊದಲ_3 INCREMENT ಕೊನೆಯ_3\n\
"), ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print numbers from ಮೊದಲ_3 ಗಮ್ಯ ಕೊನೆಯ_3, ಒಳ ಹೆಜ್ಜೆಗಳು ದ INCREMENT.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --format=FORMAT\n\
         use ಬರೆ style floating-ಬಿಂದು FORMAT\n\
"));
      oputs (_("\
  -s, --separator=STRING\n\
         use STRING ಗಮ್ಯ separate numbers (ಪೂರ್ವನಿಯೋಜಿತ: \\n)\n\
"));
      oputs (_("\
  -w, --equal-ಅಗಲ\n\
         equalize ಅಗಲ by padding with leading zeroes\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If ಮೊದಲ_3 or INCREMENT is omitted, it defaults ಗಮ್ಯ 1.  That is, an\n\
omitted INCREMENT defaults ಗಮ್ಯ 1 even when ಕೊನೆಯ_3 is smaller than ಮೊದಲ_3.\n\
The sequence ದ numbers ತುದಿಗಳು when the ಮೊತ್ತ ದ the current ಸಂಖ್ಯೆ and\n\
INCREMENT would become greater than ಕೊನೆಯ_3.\n\
ಮೊದಲ_3, INCREMENT, and ಕೊನೆಯ_3 are interpreted as floating ಬಿಂದು ಮೌಲ್ಯಗಳು.\n\
INCREMENT is usually ಧನಾತ್ಮಕ ಆದರೆ ಮೊದಲ_3 is smaller than ಕೊನೆಯ_3, and\n\
INCREMENT is usually negative ಆದರೆ ಮೊದಲ_3 is greater than ಕೊನೆಯ_3.\n\
INCREMENT must not be 0; none ದ ಮೊದಲ_3, INCREMENT and ಕೊನೆಯ_3 may be NaN.\n\
"), stdout);
      fputs (_("\
FORMAT must be suitable ಗಾಗಿ printing one ವಾದ ದ type 'double';\n\
it defaults ಗಮ್ಯ %.PRECf ಆದರೆ ಮೊದಲ_3, INCREMENT, and ಕೊನೆಯ_3 are all fixed ಬಿಂದು\n\
decimal numbers with ಗರಿಷ್ಠ precision PREC, and ಗಮ್ಯ %g otherwise.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* A command-line operand.  */
ರಚನೆ ಕಾರ್ಯಾಂಶ_2
{
  /* Its value, converted to 'long double'.  */
  ದೀರ್ಘ ದ್ವಿಗುಣ ಮೌಲ್ಯ;

  /* Its print width, if it were printed out in a form similar to its
     input form.  An input like "-.1" is treated like "-0.1", and an
     input like "1." is treated like "1", but otherwise widths are
     left alone.  */
  size_t ಅಗಲ;

  /* Number of digits after the decimal point, or INT_MAX if the
     number can't easily be expressed as a fixed-point number.  */
  ಪೂರ್ಣಾಂಕ precision;
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ಕಾರ್ಯಾಂಶ_2 ಕಾರ್ಯಾಂಶ_2;

/* Description of what a number-generating format will generate.  */
ರಚನೆ layout
{
  /* Number of bytes before and after the number.  */
  size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len;
  size_t suffix_len;
};

/* Read a long double value from the command line.
   Return if the string is correct else signal error.  */

ಸ್ಥಿರ ಕಾರ್ಯಾಂಶ_2
ಪರಿಶೀಲನೆ_arg (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg)
{
  ಕಾರ್ಯಾಂಶ_2 ret;

  ಆದರೆ (! xstrtold (arg, ಖಾಲಿ, &ret.ಮೌಲ್ಯ, cl_strtold))
    {
      error (0, 0, _("invalid floating point argument: %s"), quote (arg));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (isnan (ret.ಮೌಲ್ಯ))
    {
      error (0, 0, _("invalid %s argument: %s"), quote_n (0, "not-a-number"),
             quote_n (1, arg));
      usage (EXIT_FAILURE);
    }

  /* We don't output spaces or '+' so don't include in width */
  ಆಗ (ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*arg)) || *arg == '+')
    arg++;

  /* Default to auto width and precision.  */
  ret.ಅಗಲ = 0;
  ret.precision = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;

  /* Use no precision (and possibly fast generation) for integers.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *decimal_ಬಿಂದು = strchr (arg, '.');
  ಆದರೆ (! decimal_ಬಿಂದು && ! strchr (arg, 'p') /* not a hex float */)
    ret.precision = 0;

  /* auto set width and precision for decimal inputs.  */
  ಆದರೆ (! arg[strcspn (arg, "xX")] && isfinite (ret.ಮೌಲ್ಯ))
    {
      size_t fraction_len = 0;
      ret.ಅಗಲ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (arg);

      ಆದರೆ (decimal_ಬಿಂದು)
        {
          fraction_len = strcspn (decimal_ಬಿಂದು + 1, "eE");
          ಆದರೆ (fraction_len <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
            ret.precision = fraction_len;
          ret.ಅಗಲ += (fraction_len == 0                       /* #.  -> #   */
                        ? -1
                        : (decimal_ಬಿಂದು == arg                 /* .#  -> 0.# */
                           || !c_isdigit (decimal_ಬಿಂದು[-1]))); /* -.# -> 0.# */
        }
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *e = strchr (arg, 'e');
      ಆದರೆ (! e)
        e = strchr (arg, 'E');
      ಆದರೆ (e)
        {
          ದೀರ್ಘ exponent = MAX (strtol (e + 1, ಖಾಲಿ, 10), -LONG_MAX);
          ret.precision += exponent < 0 ? -exponent
                                        : - MIN (ret.precision, exponent);
          /* Don't account for e.... in the width since this is not output.  */
          ret.ಅಗಲ -= ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (arg) - (e - arg);
          /* Adjust the width as per the exponent.  */
          ಆದರೆ (exponent < 0)
            {
              ಆದರೆ (decimal_ಬಿಂದು)
                {
                  ಆದರೆ (e == decimal_ಬಿಂದು + 1) /* undo #. -> # above  */
                    ret.ಅಗಲ++;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                ret.ಅಗಲ++;
              exponent = -exponent;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಆದರೆ (decimal_ಬಿಂದು && ret.precision == 0 && fraction_len)
                ret.ಅಗಲ--; /* discount space for '.'  */
              exponent -= MIN (fraction_len, exponent);
            }
          ret.ಅಗಲ += exponent;
        }
    }

  ಹಿಂತಿರುಗಿ ret;
}

/* If FORMAT is a valid printf format for a double argument, return
   its long double equivalent, allocated from dynamic storage, and
   store into *LAYOUT a description of the output layout; otherwise,
   report an error and exit.  */

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
long_double_format (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt, ರಚನೆ layout *layout)
{
  size_t i;
  size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len = 0;

  ಗಾಗಿ (i = 0; ! (fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      ಆದರೆ (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      ಪೂರ್ವಪ್ರತ್ಯಯ_len++;
    }

  i++;
  i += strspn (fmt + i, "-+#0 '");
  i += strspn (fmt + i, "0123456789");
  ಆದರೆ (fmt[i] == '.')
    {
      i++;
      i += strspn (fmt + i, "0123456789");
    }

  size_t ಉದ್ದಳತೆ_modifier_ಸ್ಥಾನಾಂತರ = i;
  bool has_L = (fmt[i] == 'L');
  i += has_L;
  ಆದರೆ (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));
  ಆದರೆ (! strchr ("efgaEFGA", fmt[i]))
    error (EXIT_FAILURE, 0,
           _("format %s has unknown %%%c directive"), quote (fmt), fmt[i]);

  size_t suffix_len = 0;
  ಗಾಗಿ (i++; ; i += (fmt[i] == '%') + 1)
    ಆದರೆ (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fmt[i])
      suffix_len++;
    ಇಲ್ಲದಿದ್ದರೆ
      {
        size_t format_ಗಾತ್ರ = i + 1;
        ಅಕ್ಷರ *ldfmt = xmalloc (format_ಗಾತ್ರ + 1);
        ಸ್ಮರಣೆ_ನಕಲು (ldfmt, fmt, ಉದ್ದಳತೆ_modifier_ಸ್ಥಾನಾಂತರ);
        ldfmt[ಉದ್ದಳತೆ_modifier_ಸ್ಥಾನಾಂತರ] = 'L';
        strcpy (ldfmt + ಉದ್ದಳತೆ_modifier_ಸ್ಥಾನಾಂತರ + 1,
                fmt + ಉದ್ದಳತೆ_modifier_ಸ್ಥಾನಾಂತರ + has_L);
        layout->ಪೂರ್ವಪ್ರತ್ಯಯ_len = ಪೂರ್ವಪ್ರತ್ಯಯ_len;
        layout->suffix_len = suffix_len;
        ಹಿಂತಿರುಗಿ ldfmt;
      }
}

/* Actually print the sequence of numbers in the specified range, with the
   given or default stepping and format.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_numbers (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt, ರಚನೆ layout layout,
               ದೀರ್ಘ ದ್ವಿಗುಣ ಮೊದಲ, ದೀರ್ಘ ದ್ವಿಗುಣ ಹೆಜ್ಜೆ, ದೀರ್ಘ ದ್ವಿಗುಣ ಕೊನೆಯ)
{
  bool ಹೊರಹೋಗು_ದ_range = (ಹೆಜ್ಜೆ < 0 ? ಮೊದಲ < ಕೊನೆಯ : ಕೊನೆಯ < ಮೊದಲ);

  ಆದರೆ (! ಹೊರಹೋಗು_ದ_range)
    {
      ದೀರ್ಘ ದ್ವಿಗುಣ x = ಮೊದಲ;

      ಗಾಗಿ (ದೀರ್ಘ ದ್ವಿಗುಣ i = 1; ; i++)
        {
          ದೀರ್ಘ ದ್ವಿಗುಣ x0 = x;
          ಆದರೆ (ಬರೆ (fmt, x) < 0)
            write_error ();
          ಆದರೆ (ಹೊರಹೋಗು_ದ_range)
            ನಿಲ್ಲಿಸು;

          /* Mathematically equivalent to 'x += step;', and typically
             less subject to rounding error.  */
          x = ಮೊದಲ + i * ಹೆಜ್ಜೆ;

          ಹೊರಹೋಗು_ದ_range = (ಹೆಜ್ಜೆ < 0 ? x < ಕೊನೆಯ : ಕೊನೆಯ < x);

          ಆದರೆ (ಹೊರಹೋಗು_ದ_range)
            {
              /* If the number just past LAST prints as a value equal
                 to LAST, and prints differently from the previous
                 number, then print the number.  This avoids problems
                 with rounding.  For example, with the x86 it causes
                 "seq 0 0.000001 0.000003" to print 0.000003 instead
                 of stopping at 0.000002.  */

              bool print_extra_ಸಂಖ್ಯೆ = false;
              ಆದರೆ (locale_ok)
                setlocale (LC_NUMERIC, "C");
              ಅಕ್ಷರ *x_str;
              ಪೂರ್ಣಾಂಕ x_strlen = asprintf (&x_str, fmt, x);
              ಆದರೆ (locale_ok)
                setlocale (LC_NUMERIC, "");
              ಆದರೆ (x_strlen < 0)
                xalloc_die ();
              x_str[x_strlen - layout.suffix_len] = '\0';

              ದೀರ್ಘ ದ್ವಿಗುಣ x_val;
              ಆದರೆ (xstrtold (x_str + layout.ಪೂರ್ವಪ್ರತ್ಯಯ_len, ಖಾಲಿ,
                            &x_val, cl_strtold)
                  && x_val == ಕೊನೆಯ)
                {
                  ಅಕ್ಷರ *x0_str = ಖಾಲಿ;
                  ಪೂರ್ಣಾಂಕ x0_strlen = asprintf (&x0_str, fmt, x0);
                  ಆದರೆ (x0_strlen < 0)
                    xalloc_die ();
                  x0_str[x0_strlen - layout.suffix_len] = '\0';
                  print_extra_ಸಂಖ್ಯೆ = !streq (x0_str, x_str);
                  ಬಿಡುಗಡೆ (x0_str);
                }

              ಬಿಡುಗಡೆ (x_str);
              ಆದರೆ (! print_extra_ಸಂಖ್ಯೆ)
                ನಿಲ್ಲಿಸು;
            }

          ಆದರೆ (fputs (separator, stdout) == EOF)
            write_error ();
        }

      ಆದರೆ (fputs (terminator, stdout) == EOF)
        write_error ();
    }
}

/* Return the default format given FIRST, STEP, and LAST.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
get_default_format (ಕಾರ್ಯಾಂಶ_2 ಮೊದಲ, ಕಾರ್ಯಾಂಶ_2 ಹೆಜ್ಜೆ, ಕಾರ್ಯಾಂಶ_2 ಕೊನೆಯ)
{
  ಸ್ಥಿರ ಅಕ್ಷರ format_buf[ಗಾತ್ರ "%0.Lf" + 2 * INT_STRLEN_BOUND (ಪೂರ್ಣಾಂಕ)];

  ಪೂರ್ಣಾಂಕ prec = MAX (ಮೊದಲ.precision, ಹೆಜ್ಜೆ.precision);

  ಆದರೆ (prec != ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ && ಕೊನೆಯ.precision != ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
    {
      ಆದರೆ (equal_ಅಗಲ)
        {
          /* increase first_width by any increased precision in step */
          size_t ಮೊದಲ_ಅಗಲ = ಮೊದಲ.ಅಗಲ + (prec - ಮೊದಲ.precision);
          /* adjust last_width to use precision from first/step */
          size_t ಕೊನೆಯ_ಅಗಲ = ಕೊನೆಯ.ಅಗಲ + (prec - ಕೊನೆಯ.precision);
          ಆದರೆ (ಕೊನೆಯ.precision && prec == 0)
            ಕೊನೆಯ_ಅಗಲ--;  /* don't include space for '.' */
          ಆದರೆ (ಕೊನೆಯ.precision == 0 && prec)
            ಕೊನೆಯ_ಅಗಲ++;  /* include space for '.' */
          ಆದರೆ (ಮೊದಲ.precision == 0 && prec)
            ಮೊದಲ_ಅಗಲ++;  /* include space for '.' */
          size_t ಅಗಲ = MAX (ಮೊದಲ_ಅಗಲ, ಕೊನೆಯ_ಅಗಲ);
          ಆದರೆ (ಅಗಲ <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
            {
              ಪೂರ್ಣಾಂಕ w = ಅಗಲ;
              sprintf (format_buf, "%%0%d.%dLf", w, prec);
              ಹಿಂತಿರುಗಿ format_buf;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          sprintf (format_buf, "%%.%dLf", prec);
          ಹಿಂತಿರುಗಿ format_buf;
        }
    }

  ಹಿಂತಿರುಗಿ "%Lg";
}

/* The nonempty char array P represents a valid non-negative decimal integer.
   ENDP points just after the last char in P.
   Adjust the array to describe the next-larger integer and return
   whether this grows the array by one on the left.  */
ಸ್ಥಿರ bool
incr_grows (ಅಕ್ಷರ *p, ಅಕ್ಷರ *endp)
{
  ಮಾಡು
    {
      endp--;
      ಆದರೆ (*endp < '9')
        {
          (*endp)++;
          ಹಿಂತಿರುಗಿ false;
        }
      *endp = '0';
    }
  ಆಗ (p < endp);

  p[-1] = '1';
  ಹಿಂತಿರುಗಿ true;
}

/* Compare A and B with lengths A_LEN and B_LEN.  A and B are integers
   represented by nonempty arrays of digits without redundant leading '0'.
   Return negative if A < B, 0 if A = B, positive if A > B.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *a, idx_t a_len, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *b, idx_t b_len)
{
  ಹಿಂತಿರುಗಿ a_len == b_len ? memcmp (a, b, a_len) : _GL_CMP (a_len, b_len);
}

/* Trim leading 0's from S, but if S is all 0's, leave one.
   Return a pointer to the trimmed string.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
trim_leading_zeros (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = s;
  ಆಗ (*s == '0')
    ++s;

  /* If there were only 0's, back up, to leave one.  */
  ಆದರೆ (!*s && s != p)
    --s;
  ಹಿಂತಿರುಗಿ s;
}

/* Print all whole numbers from A to B, inclusive -- to stdout, each
   followed by a newline.  Then exit.  */
ಸ್ಥಿರ ಶೂನ್ಯ
seq_fast (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *a, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *b, uintmax_t ಹೆಜ್ಜೆ)
{
  /* Skip past any redundant leading '0's.  Without this, our naive cmp
     function would declare 000 to be larger than 99.  */
  a = trim_leading_zeros (a);
  b = trim_leading_zeros (b);

  idx_t p_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (a);
  idx_t b_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (b);
  bool inf = b_len == 3 && memeq (b, "inf", 4);

  /* Allow for at least 31 digits without realloc.
     1 more than p_len is needed for the inf case.  */
  ಎಣಿಕೆ { INITIAL_ALLOC_DIGITS = 31 };
  idx_t inc_ಗಾತ್ರ = MAX (MAX (p_len + 1, b_len), INITIAL_ALLOC_DIGITS);
  /* Ensure we only increase by at most 1 digit at buffer boundaries.  */
  static_assert (SEQ_FAST_ಹೆಜ್ಜೆ_ಎಲ್ಲೆ_DIGITS < INITIAL_ALLOC_DIGITS - 1);

  /* Copy A (sans NUL) to end of new buffer.  */
  ಅಕ್ಷರ *p0 = xmalloc (inc_ಗಾತ್ರ);
  ಅಕ್ಷರ *endp = p0 + inc_ಗಾತ್ರ;
  ಅಕ್ಷರ *p = ಸ್ಮರಣೆ_ನಕಲು (endp - p_len, a, p_len);

  /* Reduce number of write calls which is seen to
     give a speed-up of more than 2x over naive stdio code
     when printing the first 10^9 integers.  */
  ಅಕ್ಷರ buf[BUFSIZ];
  ಅಕ್ಷರ *buf_ಕೊನೆ = buf + ಗಾತ್ರ buf;
  ಅಕ್ಷರ *bufp = buf;

  ಆಗ (inf || cmp (p, endp - p, b, b_len) <= 0)
    {
      /* Append number, flushing output buffer while the number's
         digits do not fit with room for a separator or terminator.  */
      ಅಕ್ಷರ *pp = p;
      ಆಗ (buf_ಕೊನೆ - bufp <= endp - pp)
        {
          ಸ್ಮರಣೆ_ನಕಲು (bufp, pp, buf_ಕೊನೆ - bufp);
          pp += buf_ಕೊನೆ - bufp;
          ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, buf, ಗಾತ್ರ buf) != ಗಾತ್ರ buf)
            write_error ();
          bufp = buf;
        }

      /* The rest of the number, followed by a separator or terminator,
         will fit.  Tentatively append a separator.  */
      bufp = mempcpy (bufp, pp, endp - pp);
      *bufp++ = *separator;

      /* Grow number buffer if needed for the inf case.  */
      ಆದರೆ (p == p0)
        {
          ಅಕ್ಷರ *new_p0 = xpalloc (ಖಾಲಿ, &inc_ಗಾತ್ರ, 1, -1, 1);
          idx_t saved_p_len = endp - p;
          endp = new_p0 + inc_ಗಾತ್ರ;
          p = ಸ್ಮರಣೆ_ನಕಲು (endp - saved_p_len, p0, saved_p_len);
          ಬಿಡುಗಡೆ (p0);
          p0 = new_p0;
        }

      /* Compute next number.  */
      ಗಾಗಿ (uintmax_t n_incr = ಹೆಜ್ಜೆ; n_incr; n_incr--)
        p -= incr_grows (p, endp);
    }

  /* Write the remaining buffered output with a terminator instead of
     a separator.  */
  idx_t ಉಳಿದ = bufp - buf;
  ಆದರೆ (ಉಳಿದ)
    {
      bufp[-1] = *terminator;
      ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, buf, ಉಳಿದ) != ಉಳಿದ)
        write_error ();
    }

  ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
}

/* Return true if S consists of at least one digit and no non-digits.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
all_digits_p (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  size_t n = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (s);
  ಹಿಂತಿರುಗಿ c_isdigit (s[0]) && n == strspn (s, "0123456789");
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  /* The printf(3) format used for output.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format_str = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We have to handle negative numbers in the command line but this
     conflicts with the command line arguments.  So explicitly check first
     whether the next argument looks like a negative number.  */
  ಆಗ (optind < argc)
    {
      ಪೂರ್ಣಾಂಕ optc;
      ಆದರೆ (argv[optind][0] == '-'
          && ((optc = argv[optind][1]) == '.' || c_isdigit (optc)))
        {
          /* means negative number */
          ನಿಲ್ಲಿಸು;
        }

      optc = getopt_long (argc, argv, "+f:s:w", long_options, ಖಾಲಿ);
      ಆದರೆ (optc == -1)
        ನಿಲ್ಲಿಸು;

      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'f':
          format_str = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          separator = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'w':
          equal_ಅಗಲ = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಪೂರ್ಣಾಂಕ n_ವಾದಗಳು = argc - optind;
  ಆದರೆ (n_ವಾದಗಳು < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (3 < n_ವಾದಗಳು)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 3]));
      usage (EXIT_FAILURE);
    }

  ರಚನೆ layout layout = { 0, 0 };
  ಆದರೆ (format_str)
    format_str = long_double_format (format_str, &layout);

  ಆದರೆ (format_str != ಖಾಲಿ && equal_ಅಗಲ)
    {
      error (0, 0, _("format string may not be specified"
                     " when printing equal width strings"));
      usage (EXIT_FAILURE);
    }

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *user_ಆರಂಭ = n_ವಾದಗಳು == 1 ? "1" : argv[optind];

  /* If the following hold:
     - no format string, [FIXME: relax this, eventually]
     - integer start (or no start)
     - integer end
     - integer increment <= SEQ_FAST_STEP_LIMIT
     then use the much more efficient integer-only code,
     operating on arbitrarily large numbers.  */
  ಕಾರ್ಯಾಂಶ_2 ಹೆಜ್ಜೆ = { 1, 1, 0 };
  bool fast_ಹೆಜ್ಜೆ_ok = false;
  ಆದರೆ (n_ವಾದಗಳು != 3
      || (all_digits_p (argv[optind + 1])
          && xstrtold (argv[optind + 1], ಖಾಲಿ, &ಹೆಜ್ಜೆ.ಮೌಲ್ಯ, cl_strtold)
          && 0 < ಹೆಜ್ಜೆ.ಮೌಲ್ಯ && ಹೆಜ್ಜೆ.ಮೌಲ್ಯ <= SEQ_FAST_ಹೆಜ್ಜೆ_ಎಲ್ಲೆ))
    fast_ಹೆಜ್ಜೆ_ok = true;

  ಆದರೆ (all_digits_p (argv[optind])
      && (n_ವಾದಗಳು == 1 || all_digits_p (argv[optind + 1]))
      && (n_ವಾದಗಳು < 3 || (fast_ಹೆಜ್ಜೆ_ok
                         && all_digits_p (argv[optind + 2])))
      && !equal_ಅಗಲ && !format_str && ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (separator) == 1)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s1 = user_ಆರಂಭ;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s2 = argv[optind + (n_ವಾದಗಳು - 1)];
      seq_fast (s1, s2, ಹೆಜ್ಜೆ.ಮೌಲ್ಯ);
    }

  ಕಾರ್ಯಾಂಶ_2 ಮೊದಲ = { 1, 1, 0 };
  ಕಾರ್ಯಾಂಶ_2 ಕೊನೆಯ = ಪರಿಶೀಲನೆ_arg (argv[optind++]);

  ಆದರೆ (optind < argc)
    {
      ಮೊದಲ = ಕೊನೆಯ;
      ಕೊನೆಯ = ಪರಿಶೀಲನೆ_arg (argv[optind++]);

      ಆದರೆ (optind < argc)
        {
          ಹೆಜ್ಜೆ = ಕೊನೆಯ;
          ಆದರೆ (ಹೆಜ್ಜೆ.ಮೌಲ್ಯ == 0)
            {
              error (0, 0, _("invalid Zero increment value: %s"),
                     quote (argv[optind - 1]));
              usage (EXIT_FAILURE);
            }

          ಕೊನೆಯ = ಪರಿಶೀಲನೆ_arg (argv[optind++]);
        }
    }

  /* Try the fast method again, for integers of the form 1e1 etc.,
     or "inf" end value.  */
  ಆದರೆ (ಮೊದಲ.precision == 0 && ಹೆಜ್ಜೆ.precision == 0 && ಕೊನೆಯ.precision == 0
      && isfinite (ಮೊದಲ.ಮೌಲ್ಯ) && 0 <= ಮೊದಲ.ಮೌಲ್ಯ && 0 <= ಕೊನೆಯ.ಮೌಲ್ಯ
      && 0 < ಹೆಜ್ಜೆ.ಮೌಲ್ಯ && ಹೆಜ್ಜೆ.ಮೌಲ್ಯ <= SEQ_FAST_ಹೆಜ್ಜೆ_ಎಲ್ಲೆ
      && !equal_ಅಗಲ && !format_str && ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (separator) == 1)
    {
      ಅಕ್ಷರ *s1 = (all_digits_p (user_ಆರಂಭ)
                  ? xstrdup (user_ಆರಂಭ) : xasprintf ("%0.Lf", ಮೊದಲ.ಮೌಲ್ಯ));
      /* Ensure "inf" is used.  */
      ಅಕ್ಷರ *s2 = (! isfinite (ಕೊನೆಯ.ಮೌಲ್ಯ)
                  ? xstrdup ("inf") : xasprintf ("%0.Lf", ಕೊನೆಯ.ಮೌಲ್ಯ));

      ಆದರೆ (*s1 != '-' && *s2 != '-')
        seq_fast (s1, s2, ಹೆಜ್ಜೆ.ಮೌಲ್ಯ);

      ಬಿಡುಗಡೆ (s1);
      ಬಿಡುಗಡೆ (s2);
      /* Upon any failure, let the more general code deal with it.  */
    }

  ಆದರೆ (format_str == ಖಾಲಿ)
    format_str = get_default_format (ಮೊದಲ, ಹೆಜ್ಜೆ, ಕೊನೆಯ);

  print_numbers (format_str, layout, ಮೊದಲ.ಮೌಲ್ಯ, ಹೆಜ್ಜೆ.ಮೌಲ್ಯ, ಕೊನೆಯ.ಮೌಲ್ಯ);

  main_exit (EXIT_SUCCESS);
}
