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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cl-strtod.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "full-write.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtod.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xvasprintf.h"

/* Roll our own isfinite/isnan rather than using <math.h>, so that we don't
   have to worry about linking -lm just for isfinite.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ isfinite
# define isfinite(x) ((x) * 0 == 0)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ isnan
# define isnan(x) ((x) != (x))
#ਜੇ_ਅੰਤ

/* Limit below which seq_fast has more throughput.
   Determined with: seq 0 200 inf | pv > /dev/null  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SEQ_FAST_ਕਦਮ_ਹੱਦਬੰਦੀ 200  /* Keep in sync with texinfo description.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SEQ_FAST_ਕਦਮ_ਹੱਦਬੰਦੀ_DIGITS 3

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "seq"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Ulrich Drepper")

/* True if the locale settings were honored.  */
ਸਥਿਰ bool locale_ok;

/* If true print all number with equal width.  */
ਸਥਿਰ bool equal_ਚੌੜਾਈ;

/* The string used to separate two numbers.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *separator = "\n";

/* The string output after all numbers have been output.
   Usually "\n" or "\0".  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ terminator[] = "\n";

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  { "equal-width", no_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  { "format", required_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  { "separator", required_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  { ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... ਆਖਰੀ_3\n\
  or:  %s [OPTION]... ਪਹਿਲਾ_3 ਆਖਰੀ_3\n\
  or:  %s [OPTION]... ਪਹਿਲਾ_3 INCREMENT ਆਖਰੀ_3\n\
"), ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Print numbers from ਪਹਿਲਾ_3 ਮੰਜ਼ਿਲ ਆਖਰੀ_3, ਵਿੱਚ ਕਦਮਾਂ ਦਾ INCREMENT.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --format=FORMAT\n\
         use ਲਿਖੋ style floating-ਬਿੰਦੂ FORMAT\n\
"));
      oputs (_("\
  -s, --separator=STRING\n\
         use STRING ਮੰਜ਼ਿਲ separate numbers (ਮੂਲ: \\n)\n\
"));
      oputs (_("\
  -w, --equal-ਚੌੜਾਈ\n\
         equalize ਚੌੜਾਈ by padding with leading zeroes\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If ਪਹਿਲਾ_3 or INCREMENT is omitted, it defaults ਮੰਜ਼ਿਲ 1.  That is, an\n\
omitted INCREMENT defaults ਮੰਜ਼ਿਲ 1 even when ਆਖਰੀ_3 is smaller than ਪਹਿਲਾ_3.\n\
The sequence ਦਾ numbers ਸਿਰੇ when the ਜੋੜ ਦਾ the current ਸੰਖਿਆ and\n\
INCREMENT would become greater than ਆਖਰੀ_3.\n\
ਪਹਿਲਾ_3, INCREMENT, and ਆਖਰੀ_3 are interpreted as floating ਬਿੰਦੂ ਮੁੱਲਾਂ.\n\
INCREMENT is usually ਸਕਾਰਾਤਮਕ ਜੇ ਪਹਿਲਾ_3 is smaller than ਆਖਰੀ_3, and\n\
INCREMENT is usually negative ਜੇ ਪਹਿਲਾ_3 is greater than ਆਖਰੀ_3.\n\
INCREMENT must not be 0; none ਦਾ ਪਹਿਲਾ_3, INCREMENT and ਆਖਰੀ_3 may be NaN.\n\
"), stdout);
      fputs (_("\
FORMAT must be suitable ਲਈ printing one ਦਲੀਲ ਦਾ type 'double';\n\
it defaults ਮੰਜ਼ਿਲ %.PRECf ਜੇ ਪਹਿਲਾ_3, INCREMENT, and ਆਖਰੀ_3 are all fixed ਬਿੰਦੂ\n\
decimal numbers with ਵੱਧ_ਤੋਂ_ਵੱਧ precision PREC, and ਮੰਜ਼ਿਲ %g otherwise.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* A command-line operand.  */
ਬਣਤਰ ਸੰਕਾਰਜ_2
{
  /* Its value, converted to 'long double'.  */
  ਲੰਮਾ ਦੁੱਗਣਾ ਮੁੱਲ;

  /* Its print width, if it were printed out in a form similar to its
     input form.  An input like "-.1" is treated like "-0.1", and an
     input like "1." is treated like "1", but otherwise widths are
     left alone.  */
  size_t ਚੌੜਾਈ;

  /* Number of digits after the decimal point, or INT_MAX if the
     number can't easily be expressed as a fixed-point number.  */
  ਪੂਰਨ precision;
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਸੰਕਾਰਜ_2 ਸੰਕਾਰਜ_2;

/* Description of what a number-generating format will generate.  */
ਬਣਤਰ layout
{
  /* Number of bytes before and after the number.  */
  size_t ਅਗੇਤਰ_len;
  size_t suffix_len;
};

/* Read a long double value from the command line.
   Return if the string is correct else signal error.  */

ਸਥਿਰ ਸੰਕਾਰਜ_2
ਜਾਂਚ_arg (ਅੱਖਰ ਅਚਲ *arg)
{
  ਸੰਕਾਰਜ_2 ret;

  ਜੇ (! xstrtold (arg, ਨਿਰਾ, &ret.ਮੁੱਲ, cl_strtold))
    {
      error (0, 0, _("invalid floating point argument: %s"), quote (arg));
      usage (EXIT_FAILURE);
    }

  ਜੇ (isnan (ret.ਮੁੱਲ))
    {
      error (0, 0, _("invalid %s argument: %s"), quote_n (0, "not-a-number"),
             quote_n (1, arg));
      usage (EXIT_FAILURE);
    }

  /* We don't output spaces or '+' so don't include in width */
  ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*arg)) || *arg == '+')
    arg++;

  /* Default to auto width and precision.  */
  ret.ਚੌੜਾਈ = 0;
  ret.precision = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;

  /* Use no precision (and possibly fast generation) for integers.  */
  ਅੱਖਰ ਅਚਲ *decimal_ਬਿੰਦੂ = strchr (arg, '.');
  ਜੇ (! decimal_ਬਿੰਦੂ && ! strchr (arg, 'p') /* not a hex float */)
    ret.precision = 0;

  /* auto set width and precision for decimal inputs.  */
  ਜੇ (! arg[strcspn (arg, "xX")] && isfinite (ret.ਮੁੱਲ))
    {
      size_t fraction_len = 0;
      ret.ਚੌੜਾਈ = ਲੜੀ_ਲੰਬਾਈ (arg);

      ਜੇ (decimal_ਬਿੰਦੂ)
        {
          fraction_len = strcspn (decimal_ਬਿੰਦੂ + 1, "eE");
          ਜੇ (fraction_len <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
            ret.precision = fraction_len;
          ret.ਚੌੜਾਈ += (fraction_len == 0                       /* #.  -> #   */
                        ? -1
                        : (decimal_ਬਿੰਦੂ == arg                 /* .#  -> 0.# */
                           || !c_isdigit (decimal_ਬਿੰਦੂ[-1]))); /* -.# -> 0.# */
        }
      ਅੱਖਰ ਅਚਲ *e = strchr (arg, 'e');
      ਜੇ (! e)
        e = strchr (arg, 'E');
      ਜੇ (e)
        {
          ਲੰਮਾ exponent = MAX (strtol (e + 1, ਨਿਰਾ, 10), -LONG_MAX);
          ret.precision += exponent < 0 ? -exponent
                                        : - MIN (ret.precision, exponent);
          /* Don't account for e.... in the width since this is not output.  */
          ret.ਚੌੜਾਈ -= ਲੜੀ_ਲੰਬਾਈ (arg) - (e - arg);
          /* Adjust the width as per the exponent.  */
          ਜੇ (exponent < 0)
            {
              ਜੇ (decimal_ਬਿੰਦੂ)
                {
                  ਜੇ (e == decimal_ਬਿੰਦੂ + 1) /* undo #. -> # above  */
                    ret.ਚੌੜਾਈ++;
                }
              ਨਹੀਂਤਾਂ
                ret.ਚੌੜਾਈ++;
              exponent = -exponent;
            }
          ਨਹੀਂਤਾਂ
            {
              ਜੇ (decimal_ਬਿੰਦੂ && ret.precision == 0 && fraction_len)
                ret.ਚੌੜਾਈ--; /* discount space for '.'  */
              exponent -= MIN (fraction_len, exponent);
            }
          ret.ਚੌੜਾਈ += exponent;
        }
    }

  ਵਾਪਸ ret;
}

/* If FORMAT is a valid printf format for a double argument, return
   its long double equivalent, allocated from dynamic storage, and
   store into *LAYOUT a description of the output layout; otherwise,
   report an error and exit.  */

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
long_double_format (ਅੱਖਰ ਅਚਲ *fmt, ਬਣਤਰ layout *layout)
{
  size_t i;
  size_t ਅਗੇਤਰ_len = 0;

  ਲਈ (i = 0; ! (fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      ਜੇ (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      ਅਗੇਤਰ_len++;
    }

  i++;
  i += strspn (fmt + i, "-+#0 '");
  i += strspn (fmt + i, "0123456789");
  ਜੇ (fmt[i] == '.')
    {
      i++;
      i += strspn (fmt + i, "0123456789");
    }

  size_t ਲੰਮਾਈ_modifier_ਵਿੱਥ = i;
  bool has_L = (fmt[i] == 'L');
  i += has_L;
  ਜੇ (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));
  ਜੇ (! strchr ("efgaEFGA", fmt[i]))
    error (EXIT_FAILURE, 0,
           _("format %s has unknown %%%c directive"), quote (fmt), fmt[i]);

  size_t suffix_len = 0;
  ਲਈ (i++; ; i += (fmt[i] == '%') + 1)
    ਜੇ (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));
    ਨਹੀਂਤਾਂ ਜੇ (fmt[i])
      suffix_len++;
    ਨਹੀਂਤਾਂ
      {
        size_t format_ਮਾਪ = i + 1;
        ਅੱਖਰ *ldfmt = xmalloc (format_ਮਾਪ + 1);
        ਸਮ੍ਰਿਤੀ_ਨਕਲ (ldfmt, fmt, ਲੰਮਾਈ_modifier_ਵਿੱਥ);
        ldfmt[ਲੰਮਾਈ_modifier_ਵਿੱਥ] = 'L';
        strcpy (ldfmt + ਲੰਮਾਈ_modifier_ਵਿੱਥ + 1,
                fmt + ਲੰਮਾਈ_modifier_ਵਿੱਥ + has_L);
        layout->ਅਗੇਤਰ_len = ਅਗੇਤਰ_len;
        layout->suffix_len = suffix_len;
        ਵਾਪਸ ldfmt;
      }
}

/* Actually print the sequence of numbers in the specified range, with the
   given or default stepping and format.  */

ਸਥਿਰ ਖਾਲੀ
print_numbers (ਅੱਖਰ ਅਚਲ *fmt, ਬਣਤਰ layout layout,
               ਲੰਮਾ ਦੁੱਗਣਾ ਪਹਿਲਾ, ਲੰਮਾ ਦੁੱਗਣਾ ਕਦਮ, ਲੰਮਾ ਦੁੱਗਣਾ ਆਖਰੀ)
{
  bool ਬਾਹਰਲਾ_ਦਾ_range = (ਕਦਮ < 0 ? ਪਹਿਲਾ < ਆਖਰੀ : ਆਖਰੀ < ਪਹਿਲਾ);

  ਜੇ (! ਬਾਹਰਲਾ_ਦਾ_range)
    {
      ਲੰਮਾ ਦੁੱਗਣਾ x = ਪਹਿਲਾ;

      ਲਈ (ਲੰਮਾ ਦੁੱਗਣਾ i = 1; ; i++)
        {
          ਲੰਮਾ ਦੁੱਗਣਾ x0 = x;
          ਜੇ (ਲਿਖੋ (fmt, x) < 0)
            write_error ();
          ਜੇ (ਬਾਹਰਲਾ_ਦਾ_range)
            ਰੋਕੋ;

          /* Mathematically equivalent to 'x += step;', and typically
             less subject to rounding error.  */
          x = ਪਹਿਲਾ + i * ਕਦਮ;

          ਬਾਹਰਲਾ_ਦਾ_range = (ਕਦਮ < 0 ? x < ਆਖਰੀ : ਆਖਰੀ < x);

          ਜੇ (ਬਾਹਰਲਾ_ਦਾ_range)
            {
              /* If the number just past LAST prints as a value equal
                 to LAST, and prints differently from the previous
                 number, then print the number.  This avoids problems
                 with rounding.  For example, with the x86 it causes
                 "seq 0 0.000001 0.000003" to print 0.000003 instead
                 of stopping at 0.000002.  */

              bool print_extra_ਸੰਖਿਆ = false;
              ਜੇ (locale_ok)
                setlocale (LC_NUMERIC, "C");
              ਅੱਖਰ *x_str;
              ਪੂਰਨ x_strlen = asprintf (&x_str, fmt, x);
              ਜੇ (locale_ok)
                setlocale (LC_NUMERIC, "");
              ਜੇ (x_strlen < 0)
                xalloc_die ();
              x_str[x_strlen - layout.suffix_len] = '\0';

              ਲੰਮਾ ਦੁੱਗਣਾ x_val;
              ਜੇ (xstrtold (x_str + layout.ਅਗੇਤਰ_len, ਨਿਰਾ,
                            &x_val, cl_strtold)
                  && x_val == ਆਖਰੀ)
                {
                  ਅੱਖਰ *x0_str = ਨਿਰਾ;
                  ਪੂਰਨ x0_strlen = asprintf (&x0_str, fmt, x0);
                  ਜੇ (x0_strlen < 0)
                    xalloc_die ();
                  x0_str[x0_strlen - layout.suffix_len] = '\0';
                  print_extra_ਸੰਖਿਆ = !streq (x0_str, x_str);
                  ਮੁਕਤ (x0_str);
                }

              ਮੁਕਤ (x_str);
              ਜੇ (! print_extra_ਸੰਖਿਆ)
                ਰੋਕੋ;
            }

          ਜੇ (fputs (separator, stdout) == EOF)
            write_error ();
        }

      ਜੇ (fputs (terminator, stdout) == EOF)
        write_error ();
    }
}

/* Return the default format given FIRST, STEP, and LAST.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
get_default_format (ਸੰਕਾਰਜ_2 ਪਹਿਲਾ, ਸੰਕਾਰਜ_2 ਕਦਮ, ਸੰਕਾਰਜ_2 ਆਖਰੀ)
{
  ਸਥਿਰ ਅੱਖਰ format_buf[ਆਕਾਰ "%0.Lf" + 2 * INT_STRLEN_BOUND (ਪੂਰਨ)];

  ਪੂਰਨ prec = MAX (ਪਹਿਲਾ.precision, ਕਦਮ.precision);

  ਜੇ (prec != ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ && ਆਖਰੀ.precision != ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
    {
      ਜੇ (equal_ਚੌੜਾਈ)
        {
          /* increase first_width by any increased precision in step */
          size_t ਪਹਿਲਾ_ਚੌੜਾਈ = ਪਹਿਲਾ.ਚੌੜਾਈ + (prec - ਪਹਿਲਾ.precision);
          /* adjust last_width to use precision from first/step */
          size_t ਆਖਰੀ_ਚੌੜਾਈ = ਆਖਰੀ.ਚੌੜਾਈ + (prec - ਆਖਰੀ.precision);
          ਜੇ (ਆਖਰੀ.precision && prec == 0)
            ਆਖਰੀ_ਚੌੜਾਈ--;  /* don't include space for '.' */
          ਜੇ (ਆਖਰੀ.precision == 0 && prec)
            ਆਖਰੀ_ਚੌੜਾਈ++;  /* include space for '.' */
          ਜੇ (ਪਹਿਲਾ.precision == 0 && prec)
            ਪਹਿਲਾ_ਚੌੜਾਈ++;  /* include space for '.' */
          size_t ਚੌੜਾਈ = MAX (ਪਹਿਲਾ_ਚੌੜਾਈ, ਆਖਰੀ_ਚੌੜਾਈ);
          ਜੇ (ਚੌੜਾਈ <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
            {
              ਪੂਰਨ w = ਚੌੜਾਈ;
              sprintf (format_buf, "%%0%d.%dLf", w, prec);
              ਵਾਪਸ format_buf;
            }
        }
      ਨਹੀਂਤਾਂ
        {
          sprintf (format_buf, "%%.%dLf", prec);
          ਵਾਪਸ format_buf;
        }
    }

  ਵਾਪਸ "%Lg";
}

/* The nonempty char array P represents a valid non-negative decimal integer.
   ENDP points just after the last char in P.
   Adjust the array to describe the next-larger integer and return
   whether this grows the array by one on the left.  */
ਸਥਿਰ bool
incr_grows (ਅੱਖਰ *p, ਅੱਖਰ *endp)
{
  ਕਰੋ
    {
      endp--;
      ਜੇ (*endp < '9')
        {
          (*endp)++;
          ਵਾਪਸ false;
        }
      *endp = '0';
    }
  ਜਦੋਂ (p < endp);

  p[-1] = '1';
  ਵਾਪਸ true;
}

/* Compare A and B with lengths A_LEN and B_LEN.  A and B are integers
   represented by nonempty arrays of digits without redundant leading '0'.
   Return negative if A < B, 0 if A = B, positive if A > B.  */
ਸਥਿਰ ਪੂਰਨ
cmp (ਅੱਖਰ ਅਚਲ *a, idx_t a_len, ਅੱਖਰ ਅਚਲ *b, idx_t b_len)
{
  ਵਾਪਸ a_len == b_len ? memcmp (a, b, a_len) : _GL_CMP (a_len, b_len);
}

/* Trim leading 0's from S, but if S is all 0's, leave one.
   Return a pointer to the trimmed string.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
trim_leading_zeros (ਅੱਖਰ ਅਚਲ *s)
{
  ਅੱਖਰ ਅਚਲ *p = s;
  ਜਦੋਂ (*s == '0')
    ++s;

  /* If there were only 0's, back up, to leave one.  */
  ਜੇ (!*s && s != p)
    --s;
  ਵਾਪਸ s;
}

/* Print all whole numbers from A to B, inclusive -- to stdout, each
   followed by a newline.  Then exit.  */
ਸਥਿਰ ਖਾਲੀ
seq_fast (ਅੱਖਰ ਅਚਲ *a, ਅੱਖਰ ਅਚਲ *b, uintmax_t ਕਦਮ)
{
  /* Skip past any redundant leading '0's.  Without this, our naive cmp
     function would declare 000 to be larger than 99.  */
  a = trim_leading_zeros (a);
  b = trim_leading_zeros (b);

  idx_t p_len = ਲੜੀ_ਲੰਬਾਈ (a);
  idx_t b_len = ਲੜੀ_ਲੰਬਾਈ (b);
  bool inf = b_len == 3 && memeq (b, "inf", 4);

  /* Allow for at least 31 digits without realloc.
     1 more than p_len is needed for the inf case.  */
  ਗਿਣਤੀ { INITIAL_ALLOC_DIGITS = 31 };
  idx_t inc_ਮਾਪ = MAX (MAX (p_len + 1, b_len), INITIAL_ALLOC_DIGITS);
  /* Ensure we only increase by at most 1 digit at buffer boundaries.  */
  static_assert (SEQ_FAST_ਕਦਮ_ਹੱਦਬੰਦੀ_DIGITS < INITIAL_ALLOC_DIGITS - 1);

  /* Copy A (sans NUL) to end of new buffer.  */
  ਅੱਖਰ *p0 = xmalloc (inc_ਮਾਪ);
  ਅੱਖਰ *endp = p0 + inc_ਮਾਪ;
  ਅੱਖਰ *p = ਸਮ੍ਰਿਤੀ_ਨਕਲ (endp - p_len, a, p_len);

  /* Reduce number of write calls which is seen to
     give a speed-up of more than 2x over naive stdio code
     when printing the first 10^9 integers.  */
  ਅੱਖਰ buf[BUFSIZ];
  ਅੱਖਰ *buf_ਸਿਰਾ = buf + ਆਕਾਰ buf;
  ਅੱਖਰ *bufp = buf;

  ਜਦੋਂ (inf || cmp (p, endp - p, b, b_len) <= 0)
    {
      /* Append number, flushing output buffer while the number's
         digits do not fit with room for a separator or terminator.  */
      ਅੱਖਰ *pp = p;
      ਜਦੋਂ (buf_ਸਿਰਾ - bufp <= endp - pp)
        {
          ਸਮ੍ਰਿਤੀ_ਨਕਲ (bufp, pp, buf_ਸਿਰਾ - bufp);
          pp += buf_ਸਿਰਾ - bufp;
          ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, buf, ਆਕਾਰ buf) != ਆਕਾਰ buf)
            write_error ();
          bufp = buf;
        }

      /* The rest of the number, followed by a separator or terminator,
         will fit.  Tentatively append a separator.  */
      bufp = mempcpy (bufp, pp, endp - pp);
      *bufp++ = *separator;

      /* Grow number buffer if needed for the inf case.  */
      ਜੇ (p == p0)
        {
          ਅੱਖਰ *new_p0 = xpalloc (ਨਿਰਾ, &inc_ਮਾਪ, 1, -1, 1);
          idx_t saved_p_len = endp - p;
          endp = new_p0 + inc_ਮਾਪ;
          p = ਸਮ੍ਰਿਤੀ_ਨਕਲ (endp - saved_p_len, p0, saved_p_len);
          ਮੁਕਤ (p0);
          p0 = new_p0;
        }

      /* Compute next number.  */
      ਲਈ (uintmax_t n_incr = ਕਦਮ; n_incr; n_incr--)
        p -= incr_grows (p, endp);
    }

  /* Write the remaining buffered output with a terminator instead of
     a separator.  */
  idx_t ਬਚਿਆ = bufp - buf;
  ਜੇ (ਬਚਿਆ)
    {
      bufp[-1] = *terminator;
      ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, buf, ਬਚਿਆ) != ਬਚਿਆ)
        write_error ();
    }

  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
}

/* Return true if S consists of at least one digit and no non-digits.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
all_digits_p (ਅੱਖਰ ਅਚਲ *s)
{
  size_t n = ਲੜੀ_ਲੰਬਾਈ (s);
  ਵਾਪਸ c_isdigit (s[0]) && n == strspn (s, "0123456789");
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  /* The printf(3) format used for output.  */
  ਅੱਖਰ ਅਚਲ *format_str = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We have to handle negative numbers in the command line but this
     conflicts with the command line arguments.  So explicitly check first
     whether the next argument looks like a negative number.  */
  ਜਦੋਂ (optind < argc)
    {
      ਪੂਰਨ optc;
      ਜੇ (argv[optind][0] == '-'
          && ((optc = argv[optind][1]) == '.' || c_isdigit (optc)))
        {
          /* means negative number */
          ਰੋਕੋ;
        }

      optc = getopt_long (argc, argv, "+f:s:w", long_options, ਨਿਰਾ);
      ਜੇ (optc == -1)
        ਰੋਕੋ;

      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'f':
          format_str = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          separator = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'w':
          equal_ਚੌੜਾਈ = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਪੂਰਨ n_ਦਲੀਲਾਂ = argc - optind;
  ਜੇ (n_ਦਲੀਲਾਂ < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (3 < n_ਦਲੀਲਾਂ)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 3]));
      usage (EXIT_FAILURE);
    }

  ਬਣਤਰ layout layout = { 0, 0 };
  ਜੇ (format_str)
    format_str = long_double_format (format_str, &layout);

  ਜੇ (format_str != ਨਿਰਾ && equal_ਚੌੜਾਈ)
    {
      error (0, 0, _("format string may not be specified"
                     " when printing equal width strings"));
      usage (EXIT_FAILURE);
    }

  ਅੱਖਰ ਅਚਲ *user_ਸ਼ੁਰੂ = n_ਦਲੀਲਾਂ == 1 ? "1" : argv[optind];

  /* If the following hold:
     - no format string, [FIXME: relax this, eventually]
     - integer start (or no start)
     - integer end
     - integer increment <= SEQ_FAST_STEP_LIMIT
     then use the much more efficient integer-only code,
     operating on arbitrarily large numbers.  */
  ਸੰਕਾਰਜ_2 ਕਦਮ = { 1, 1, 0 };
  bool fast_ਕਦਮ_ok = false;
  ਜੇ (n_ਦਲੀਲਾਂ != 3
      || (all_digits_p (argv[optind + 1])
          && xstrtold (argv[optind + 1], ਨਿਰਾ, &ਕਦਮ.ਮੁੱਲ, cl_strtold)
          && 0 < ਕਦਮ.ਮੁੱਲ && ਕਦਮ.ਮੁੱਲ <= SEQ_FAST_ਕਦਮ_ਹੱਦਬੰਦੀ))
    fast_ਕਦਮ_ok = true;

  ਜੇ (all_digits_p (argv[optind])
      && (n_ਦਲੀਲਾਂ == 1 || all_digits_p (argv[optind + 1]))
      && (n_ਦਲੀਲਾਂ < 3 || (fast_ਕਦਮ_ok
                         && all_digits_p (argv[optind + 2])))
      && !equal_ਚੌੜਾਈ && !format_str && ਲੜੀ_ਲੰਬਾਈ (separator) == 1)
    {
      ਅੱਖਰ ਅਚਲ *s1 = user_ਸ਼ੁਰੂ;
      ਅੱਖਰ ਅਚਲ *s2 = argv[optind + (n_ਦਲੀਲਾਂ - 1)];
      seq_fast (s1, s2, ਕਦਮ.ਮੁੱਲ);
    }

  ਸੰਕਾਰਜ_2 ਪਹਿਲਾ = { 1, 1, 0 };
  ਸੰਕਾਰਜ_2 ਆਖਰੀ = ਜਾਂਚ_arg (argv[optind++]);

  ਜੇ (optind < argc)
    {
      ਪਹਿਲਾ = ਆਖਰੀ;
      ਆਖਰੀ = ਜਾਂਚ_arg (argv[optind++]);

      ਜੇ (optind < argc)
        {
          ਕਦਮ = ਆਖਰੀ;
          ਜੇ (ਕਦਮ.ਮੁੱਲ == 0)
            {
              error (0, 0, _("invalid Zero increment value: %s"),
                     quote (argv[optind - 1]));
              usage (EXIT_FAILURE);
            }

          ਆਖਰੀ = ਜਾਂਚ_arg (argv[optind++]);
        }
    }

  /* Try the fast method again, for integers of the form 1e1 etc.,
     or "inf" end value.  */
  ਜੇ (ਪਹਿਲਾ.precision == 0 && ਕਦਮ.precision == 0 && ਆਖਰੀ.precision == 0
      && isfinite (ਪਹਿਲਾ.ਮੁੱਲ) && 0 <= ਪਹਿਲਾ.ਮੁੱਲ && 0 <= ਆਖਰੀ.ਮੁੱਲ
      && 0 < ਕਦਮ.ਮੁੱਲ && ਕਦਮ.ਮੁੱਲ <= SEQ_FAST_ਕਦਮ_ਹੱਦਬੰਦੀ
      && !equal_ਚੌੜਾਈ && !format_str && ਲੜੀ_ਲੰਬਾਈ (separator) == 1)
    {
      ਅੱਖਰ *s1 = (all_digits_p (user_ਸ਼ੁਰੂ)
                  ? xstrdup (user_ਸ਼ੁਰੂ) : xasprintf ("%0.Lf", ਪਹਿਲਾ.ਮੁੱਲ));
      /* Ensure "inf" is used.  */
      ਅੱਖਰ *s2 = (! isfinite (ਆਖਰੀ.ਮੁੱਲ)
                  ? xstrdup ("inf") : xasprintf ("%0.Lf", ਆਖਰੀ.ਮੁੱਲ));

      ਜੇ (*s1 != '-' && *s2 != '-')
        seq_fast (s1, s2, ਕਦਮ.ਮੁੱਲ);

      ਮੁਕਤ (s1);
      ਮੁਕਤ (s2);
      /* Upon any failure, let the more general code deal with it.  */
    }

  ਜੇ (format_str == ਨਿਰਾ)
    format_str = get_default_format (ਪਹਿਲਾ, ਕਦਮ, ਆਖਰੀ);

  print_numbers (format_str, layout, ਪਹਿਲਾ.ਮੁੱਲ, ਕਦਮ.ਮੁੱਲ, ਆਖਰੀ.ਮੁੱਲ);

  main_exit (EXIT_SUCCESS);
}
