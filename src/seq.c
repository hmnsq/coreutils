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

#شامل <config.h>
#شامل <getopt.h>
#شامل <stdio.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "c-ctype.h"
#شامل "cl-strtod.h"
#شامل "full-write.h"
#شامل "quote.h"
#شامل "xstrtod.h"
#شامل "xvasprintf.h"

/* Roll our own isfinite/isnan rather than using <math.h>, so that we don't
   have to worry about linking -lm just for isfinite.  */
#ifndef isfinite
# define isfinite(x) ((x) * 0 == 0)
#endif
#ifndef isnan
# define isnan(x) ((x) != (x))
#endif

/* Limit below which seq_fast has more throughput.
   Determined with: seq 0 200 inf | pv > /dev/null  */
#define SEQ_FAST_STEP_LIMIT 200  /* Keep in sync with texinfo description.  */
#define SEQ_FAST_STEP_LIMIT_DIGITS 3

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "seq"

#define AUTHORS proper_name ("Ulrich Drepper")

/* True if the locale settings were honored.  */
ایستا bool locale_ok;

/* If true print all number with equal width.  */
ایستا bool equal_width;

/* The string used to separate two numbers.  */
ایستا نویسه ثابت *separator = "\n";

/* The string output after all numbers have been output.
   Usually "\n" or "\0".  */
ایستا نویسه ثابت terminator[] = "\n";

ایستا ساختار option ثابت long_options[] =
{
  { "equal-width", no_argument, پوچ, 'w'},
  { "format", required_argument, پوچ, 'f'},
  { "separator", required_argument, پوچ, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  { پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... LAST\n\
  or:  %s [OPTION]... FIRST LAST\n\
  or:  %s [OPTION]... FIRST INCREMENT LAST\n\
"), program_name, program_name, program_name);
      fputs (_("\
Print numbers from FIRST to LAST, in steps of INCREMENT.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --format=FORMAT\n\
         use printf style floating-point FORMAT\n\
"));
      oputs (_("\
  -s, --separator=STRING\n\
         use STRING to separate numbers (پیشفرض: \\n)\n\
"));
      oputs (_("\
  -w, --equal-width\n\
         equalize width by padding with leading zeroes\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FIRST or INCREMENT is omitted, it defaults to 1.  That is, an\n\
omitted INCREMENT defaults to 1 even when LAST is smaller than FIRST.\n\
The sequence of numbers ends when the sum of the current عدد and\n\
INCREMENT would become greater than LAST.\n\
FIRST, INCREMENT, and LAST are interpreted as floating point مقادیر.\n\
INCREMENT is usually positive اگر FIRST is smaller than LAST, and\n\
INCREMENT is usually negative اگر FIRST is greater than LAST.\n\
INCREMENT must not be 0; none of FIRST, INCREMENT and LAST may be NaN.\n\
"), stdout);
      fputs (_("\
FORMAT must be suitable برای printing one argument of type 'double';\n\
it defaults to %.PRECf اگر FIRST, INCREMENT, and LAST are all fixed point\n\
decimal numbers with maximum precision PREC, and to %g otherwise.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A command-line operand.  */
ساختار operand
{
  /* Its value, converted to 'long double'.  */
  long دوگانه مقدار;

  /* Its print width, if it were printed out in a form similar to its
     input form.  An input like "-.1" is treated like "-0.1", and an
     input like "1." is treated like "1", but otherwise widths are
     left alone.  */
  size_t width;

  /* Number of digits after the decimal point, or INT_MAX if the
     number can't easily be expressed as a fixed-point number.  */
  صحیح precision;
};
تعریفنوع ساختار operand operand;

/* Description of what a number-generating format will generate.  */
ساختار layout
{
  /* Number of bytes before and after the number.  */
  size_t prefix_len;
  size_t suffix_len;
};

/* Read a long double value from the command line.
   Return if the string is correct else signal error.  */

ایستا operand
scan_arg (نویسه ثابت *arg)
{
  operand ret;

  اگر (! xstrtold (arg, پوچ, &ret.مقدار, cl_strtold))
    {
      error (0, 0, _("invalid floating point argument: %s"), quote (arg));
      usage (EXIT_FAILURE);
    }

  اگر (isnan (ret.مقدار))
    {
      error (0, 0, _("invalid %s argument: %s"), quote_n (0, "not-a-number"),
             quote_n (1, arg));
      usage (EXIT_FAILURE);
    }

  /* We don't output spaces or '+' so don't include in width */
  وقتی (isspace (to_uchar (*arg)) || *arg == '+')
    arg++;

  /* Default to auto width and precision.  */
  ret.width = 0;
  ret.precision = INT_MAX;

  /* Use no precision (and possibly fast generation) for integers.  */
  نویسه ثابت *decimal_point = strchr (arg, '.');
  اگر (! decimal_point && ! strchr (arg, 'p') /* not a hex float */)
    ret.precision = 0;

  /* auto set width and precision for decimal inputs.  */
  اگر (! arg[strcspn (arg, "xX")] && isfinite (ret.مقدار))
    {
      size_t fraction_len = 0;
      ret.width = strlen (arg);

      اگر (decimal_point)
        {
          fraction_len = strcspn (decimal_point + 1, "eE");
          اگر (fraction_len <= INT_MAX)
            ret.precision = fraction_len;
          ret.width += (fraction_len == 0                       /* #.  -> #   */
                        ? -1
                        : (decimal_point == arg                 /* .#  -> 0.# */
                           || !c_isdigit (decimal_point[-1]))); /* -.# -> 0.# */
        }
      نویسه ثابت *e = strchr (arg, 'e');
      اگر (! e)
        e = strchr (arg, 'E');
      اگر (e)
        {
          long exponent = MAX (strtol (e + 1, پوچ, 10), -LONG_MAX);
          ret.precision += exponent < 0 ? -exponent
                                        : - MIN (ret.precision, exponent);
          /* Don't account for e.... in the width since this is not output.  */
          ret.width -= strlen (arg) - (e - arg);
          /* Adjust the width as per the exponent.  */
          اگر (exponent < 0)
            {
              اگر (decimal_point)
                {
                  اگر (e == decimal_point + 1) /* undo #. -> # above  */
                    ret.width++;
                }
              وگرنه
                ret.width++;
              exponent = -exponent;
            }
          وگرنه
            {
              اگر (decimal_point && ret.precision == 0 && fraction_len)
                ret.width--; /* discount space for '.'  */
              exponent -= MIN (fraction_len, exponent);
            }
          ret.width += exponent;
        }
    }

  بازگشت ret;
}

/* If FORMAT is a valid printf format for a double argument, return
   its long double equivalent, allocated from dynamic storage, and
   store into *LAYOUT a description of the output layout; otherwise,
   report an error and exit.  */

ایستا نویسه ثابت *
long_double_format (نویسه ثابت *fmt, ساختار layout *layout)
{
  size_t i;
  size_t prefix_len = 0;

  برای (i = 0; ! (fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      اگر (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  i += strspn (fmt + i, "-+#0 '");
  i += strspn (fmt + i, "0123456789");
  اگر (fmt[i] == '.')
    {
      i++;
      i += strspn (fmt + i, "0123456789");
    }

  size_t length_modifier_offset = i;
  bool has_L = (fmt[i] == 'L');
  i += has_L;
  اگر (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));
  اگر (! strchr ("efgaEFGA", fmt[i]))
    error (EXIT_FAILURE, 0,
           _("format %s has unknown %%%c directive"), quote (fmt), fmt[i]);

  size_t suffix_len = 0;
  برای (i++; ; i += (fmt[i] == '%') + 1)
    اگر (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));
    وگرنه اگر (fmt[i])
      suffix_len++;
    وگرنه
      {
        size_t format_size = i + 1;
        نویسه *ldfmt = xmalloc (format_size + 1);
        memcpy (ldfmt, fmt, length_modifier_offset);
        ldfmt[length_modifier_offset] = 'L';
        strcpy (ldfmt + length_modifier_offset + 1,
                fmt + length_modifier_offset + has_L);
        layout->prefix_len = prefix_len;
        layout->suffix_len = suffix_len;
        بازگشت ldfmt;
      }
}

/* Actually print the sequence of numbers in the specified range, with the
   given or default stepping and format.  */

ایستا تهی
print_numbers (نویسه ثابت *fmt, ساختار layout layout,
               long دوگانه first, long دوگانه step, long دوگانه last)
{
  bool out_of_range = (step < 0 ? first < last : last < first);

  اگر (! out_of_range)
    {
      long دوگانه x = first;

      برای (long دوگانه i = 1; ; i++)
        {
          long دوگانه x0 = x;
          اگر (printf (fmt, x) < 0)
            write_error ();
          اگر (out_of_range)
            بشکن;

          /* Mathematically equivalent to 'x += step;', and typically
             less subject to rounding error.  */
          x = first + i * step;

          out_of_range = (step < 0 ? x < last : last < x);

          اگر (out_of_range)
            {
              /* If the number just past LAST prints as a value equal
                 to LAST, and prints differently from the previous
                 number, then print the number.  This avoids problems
                 with rounding.  For example, with the x86 it causes
                 "seq 0 0.000001 0.000003" to print 0.000003 instead
                 of stopping at 0.000002.  */

              bool print_extra_عدد = false;
              اگر (locale_ok)
                setlocale (LC_NUMERIC, "C");
              نویسه *x_str;
              صحیح x_strlen = asprintf (&x_str, fmt, x);
              اگر (locale_ok)
                setlocale (LC_NUMERIC, "");
              اگر (x_strlen < 0)
                xalloc_die ();
              x_str[x_strlen - layout.suffix_len] = '\0';

              long دوگانه x_val;
              اگر (xstrtold (x_str + layout.prefix_len, پوچ,
                            &x_val, cl_strtold)
                  && x_val == last)
                {
                  نویسه *x0_str = پوچ;
                  صحیح x0_strlen = asprintf (&x0_str, fmt, x0);
                  اگر (x0_strlen < 0)
                    xalloc_die ();
                  x0_str[x0_strlen - layout.suffix_len] = '\0';
                  print_extra_عدد = !streq (x0_str, x_str);
                  آزاد (x0_str);
                }

              آزاد (x_str);
              اگر (! print_extra_عدد)
                بشکن;
            }

          اگر (fputs (separator, stdout) == EOF)
            write_error ();
        }

      اگر (fputs (terminator, stdout) == EOF)
        write_error ();
    }
}

/* Return the default format given FIRST, STEP, and LAST.  */
ایستا نویسه ثابت *
get_default_format (operand first, operand step, operand last)
{
  ایستا نویسه format_buf[اندازه "%0.Lf" + 2 * INT_STRLEN_BOUND (صحیح)];

  صحیح prec = MAX (first.precision, step.precision);

  اگر (prec != INT_MAX && last.precision != INT_MAX)
    {
      اگر (equal_width)
        {
          /* increase first_width by any increased precision in step */
          size_t first_width = first.width + (prec - first.precision);
          /* adjust last_width to use precision from first/step */
          size_t last_width = last.width + (prec - last.precision);
          اگر (last.precision && prec == 0)
            last_width--;  /* don't include space for '.' */
          اگر (last.precision == 0 && prec)
            last_width++;  /* include space for '.' */
          اگر (first.precision == 0 && prec)
            first_width++;  /* include space for '.' */
          size_t width = MAX (first_width, last_width);
          اگر (width <= INT_MAX)
            {
              صحیح w = width;
              sprintf (format_buf, "%%0%d.%dLf", w, prec);
              بازگشت format_buf;
            }
        }
      وگرنه
        {
          sprintf (format_buf, "%%.%dLf", prec);
          بازگشت format_buf;
        }
    }

  بازگشت "%Lg";
}

/* The nonempty char array P represents a valid non-negative decimal integer.
   ENDP points just after the last char in P.
   Adjust the array to describe the next-larger integer and return
   whether this grows the array by one on the left.  */
ایستا bool
incr_grows (نویسه *p, نویسه *endp)
{
  انجامبده
    {
      endp--;
      اگر (*endp < '9')
        {
          (*endp)++;
          بازگشت false;
        }
      *endp = '0';
    }
  وقتی (p < endp);

  p[-1] = '1';
  بازگشت true;
}

/* Compare A and B with lengths A_LEN and B_LEN.  A and B are integers
   represented by nonempty arrays of digits without redundant leading '0'.
   Return negative if A < B, 0 if A = B, positive if A > B.  */
ایستا صحیح
cmp (نویسه ثابت *a, idx_t a_len, نویسه ثابت *b, idx_t b_len)
{
  بازگشت a_len == b_len ? memcmp (a, b, a_len) : _GL_CMP (a_len, b_len);
}

/* Trim leading 0's from S, but if S is all 0's, leave one.
   Return a pointer to the trimmed string.  */
ATTRIBUTE_PURE
ایستا نویسه ثابت *
trim_leading_zeros (نویسه ثابت *s)
{
  نویسه ثابت *p = s;
  وقتی (*s == '0')
    ++s;

  /* If there were only 0's, back up, to leave one.  */
  اگر (!*s && s != p)
    --s;
  بازگشت s;
}

/* Print all whole numbers from A to B, inclusive -- to stdout, each
   followed by a newline.  Then exit.  */
ایستا تهی
seq_fast (نویسه ثابت *a, نویسه ثابت *b, uintmax_t step)
{
  /* Skip past any redundant leading '0's.  Without this, our naive cmp
     function would declare 000 to be larger than 99.  */
  a = trim_leading_zeros (a);
  b = trim_leading_zeros (b);

  idx_t p_len = strlen (a);
  idx_t b_len = strlen (b);
  bool inf = b_len == 3 && memeq (b, "inf", 4);

  /* Allow for at least 31 digits without realloc.
     1 more than p_len is needed for the inf case.  */
  شمارش { INITIAL_ALLOC_DIGITS = 31 };
  idx_t inc_size = MAX (MAX (p_len + 1, b_len), INITIAL_ALLOC_DIGITS);
  /* Ensure we only increase by at most 1 digit at buffer boundaries.  */
  static_assert (SEQ_FAST_STEP_LIMIT_DIGITS < INITIAL_ALLOC_DIGITS - 1);

  /* Copy A (sans NUL) to end of new buffer.  */
  نویسه *p0 = xmalloc (inc_size);
  نویسه *endp = p0 + inc_size;
  نویسه *p = memcpy (endp - p_len, a, p_len);

  /* Reduce number of write calls which is seen to
     give a speed-up of more than 2x over naive stdio code
     when printing the first 10^9 integers.  */
  نویسه buf[BUFSIZ];
  نویسه *buf_end = buf + اندازه buf;
  نویسه *bufp = buf;

  وقتی (inf || cmp (p, endp - p, b, b_len) <= 0)
    {
      /* Append number, flushing output buffer while the number's
         digits do not fit with room for a separator or terminator.  */
      نویسه *pp = p;
      وقتی (buf_end - bufp <= endp - pp)
        {
          memcpy (bufp, pp, buf_end - bufp);
          pp += buf_end - bufp;
          اگر (full_write (STDOUT_FILENO, buf, اندازه buf) != اندازه buf)
            write_error ();
          bufp = buf;
        }

      /* The rest of the number, followed by a separator or terminator,
         will fit.  Tentatively append a separator.  */
      bufp = mempcpy (bufp, pp, endp - pp);
      *bufp++ = *separator;

      /* Grow number buffer if needed for the inf case.  */
      اگر (p == p0)
        {
          نویسه *new_p0 = xpalloc (پوچ, &inc_size, 1, -1, 1);
          idx_t saved_p_len = endp - p;
          endp = new_p0 + inc_size;
          p = memcpy (endp - saved_p_len, p0, saved_p_len);
          آزاد (p0);
          p0 = new_p0;
        }

      /* Compute next number.  */
      برای (uintmax_t n_incr = step; n_incr; n_incr--)
        p -= incr_grows (p, endp);
    }

  /* Write the remaining buffered output with a terminator instead of
     a separator.  */
  idx_t remaining = bufp - buf;
  اگر (remaining)
    {
      bufp[-1] = *terminator;
      اگر (full_write (STDOUT_FILENO, buf, remaining) != remaining)
        write_error ();
    }

  exit (EXIT_SUCCESS);
}

/* Return true if S consists of at least one digit and no non-digits.  */
ATTRIBUTE_PURE
ایستا bool
all_digits_p (نویسه ثابت *s)
{
  size_t n = strlen (s);
  بازگشت c_isdigit (s[0]) && n == strspn (s, "0123456789");
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  /* The printf(3) format used for output.  */
  نویسه ثابت *format_str = پوچ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We have to handle negative numbers in the command line but this
     conflicts with the command line arguments.  So explicitly check first
     whether the next argument looks like a negative number.  */
  وقتی (optind < argc)
    {
      صحیح optc;
      اگر (argv[optind][0] == '-'
          && ((optc = argv[optind][1]) == '.' || c_isdigit (optc)))
        {
          /* means negative number */
          بشکن;
        }

      optc = getopt_long (argc, argv, "+f:s:w", long_options, پوچ);
      اگر (optc == -1)
        بشکن;

      انتخاب (optc)
        {
        حالت 'f':
          format_str = optarg;
          بشکن;

        حالت 's':
          separator = optarg;
          بشکن;

        حالت 'w':
          equal_width = true;
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  صحیح n_args = argc - optind;
  اگر (n_args < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  اگر (3 < n_args)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 3]));
      usage (EXIT_FAILURE);
    }

  ساختار layout layout = { 0, 0 };
  اگر (format_str)
    format_str = long_double_format (format_str, &layout);

  اگر (format_str != پوچ && equal_width)
    {
      error (0, 0, _("format string may not be specified"
                     " when printing equal width strings"));
      usage (EXIT_FAILURE);
    }

  نویسه ثابت *user_start = n_args == 1 ? "1" : argv[optind];

  /* If the following hold:
     - no format string, [FIXME: relax this, eventually]
     - integer start (or no start)
     - integer end
     - integer increment <= SEQ_FAST_STEP_LIMIT
     then use the much more efficient integer-only code,
     operating on arbitrarily large numbers.  */
  operand step = { 1, 1, 0 };
  bool fast_step_ok = false;
  اگر (n_args != 3
      || (all_digits_p (argv[optind + 1])
          && xstrtold (argv[optind + 1], پوچ, &step.مقدار, cl_strtold)
          && 0 < step.مقدار && step.مقدار <= SEQ_FAST_STEP_LIMIT))
    fast_step_ok = true;

  اگر (all_digits_p (argv[optind])
      && (n_args == 1 || all_digits_p (argv[optind + 1]))
      && (n_args < 3 || (fast_step_ok
                         && all_digits_p (argv[optind + 2])))
      && !equal_width && !format_str && strlen (separator) == 1)
    {
      نویسه ثابت *s1 = user_start;
      نویسه ثابت *s2 = argv[optind + (n_args - 1)];
      seq_fast (s1, s2, step.مقدار);
    }

  operand first = { 1, 1, 0 };
  operand last = scan_arg (argv[optind++]);

  اگر (optind < argc)
    {
      first = last;
      last = scan_arg (argv[optind++]);

      اگر (optind < argc)
        {
          step = last;
          اگر (step.مقدار == 0)
            {
              error (0, 0, _("invalid Zero increment value: %s"),
                     quote (argv[optind - 1]));
              usage (EXIT_FAILURE);
            }

          last = scan_arg (argv[optind++]);
        }
    }

  /* Try the fast method again, for integers of the form 1e1 etc.,
     or "inf" end value.  */
  اگر (first.precision == 0 && step.precision == 0 && last.precision == 0
      && isfinite (first.مقدار) && 0 <= first.مقدار && 0 <= last.مقدار
      && 0 < step.مقدار && step.مقدار <= SEQ_FAST_STEP_LIMIT
      && !equal_width && !format_str && strlen (separator) == 1)
    {
      نویسه *s1 = (all_digits_p (user_start)
                  ? xstrdup (user_start) : xasprintf ("%0.Lf", first.مقدار));
      /* Ensure "inf" is used.  */
      نویسه *s2 = (! isfinite (last.مقدار)
                  ? xstrdup ("inf") : xasprintf ("%0.Lf", last.مقدار));

      اگر (*s1 != '-' && *s2 != '-')
        seq_fast (s1, s2, step.مقدار);

      آزاد (s1);
      آزاد (s2);
      /* Upon any failure, let the more general code deal with it.  */
    }

  اگر (format_str == پوچ)
    format_str = get_default_format (first, step, last);

  print_numbers (format_str, layout, first.مقدار, step.مقدار, last.مقدار);

  main_exit (EXIT_SUCCESS);
}
