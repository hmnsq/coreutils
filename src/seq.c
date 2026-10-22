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

#ضمن <config.h>
#ضمن <getopt.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "c-ctype.h"
#ضمن "cl-strtod.h"
#ضمن "full-write.h"
#ضمن "quote.h"
#ضمن "xstrtod.h"
#ضمن "xvasprintf.h"

/* Roll our own isfinite/isnan rather than using <math.h>, so that we don't
   have to worry about linking -lm just for isfinite.  */
#اگر_تعریف_نشده isfinite
# define isfinite(x) ((x) * 0 == 0)
#پایان_اگر
#اگر_تعریف_نشده isnan
# define isnan(x) ((x) != (x))
#پایان_اگر

/* Limit below which seq_fast has more throughput.
   Determined with: seq 0 200 inf | pv > /dev/null  */
#تعریف_کن SEQ_FAST_گام_حد 200  /* Keep in sync with texinfo description.  */
#تعریف_کن SEQ_FAST_گام_حد_DIGITS 3

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "seq"

#تعریف_کن AUTHORS proper_name ("Ulrich Drepper")

/* True if the locale settings were honored.  */
ساكن bool locale_ok;

/* If true print all number with equal width.  */
ساكن bool equal_عرض;

/* The string used to separate two numbers.  */
ساكن حرف ثابت *separator = "\n";

/* The string output after all numbers have been output.
   Usually "\n" or "\0".  */
ساكن حرف ثابت terminator[] = "\n";

ساكن بنية option ثابت long_options[] =
{
  { "equal-width", no_آرگومان, عدم, 'w'},
  { "format", required_آرگومان, عدم, 'f'},
  { "separator", required_آرگومان, عدم, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  { عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... آخرین_3\n\
  or:  %s [OPTION]... اولی_3 آخرین_3\n\
  or:  %s [OPTION]... اولی_3 INCREMENT آخرین_3\n\
"), برنامه_name, برنامه_name, برنامه_name);
      fputs (_("\
Print numbers from اولی_3 مقصد آخرین_3, درون گامها از INCREMENT.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --format=FORMAT\n\
         use چاپ_کن style floating-نقطه FORMAT\n\
"));
      oputs (_("\
  -s, --separator=STRING\n\
         use STRING مقصد separate numbers (افتراضي: \\n)\n\
"));
      oputs (_("\
  -w, --equal-عرض\n\
         equalize عرض by padding with leading zeroes\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If اولی_3 or INCREMENT is omitted, it defaults مقصد 1.  That is, an\n\
omitted INCREMENT defaults مقصد 1 even when آخرین_3 is smaller than اولی_3.\n\
The sequence از numbers دوسر when the جمع از the current عدد and\n\
INCREMENT would become greater than آخرین_3.\n\
اولی_3, INCREMENT, and آخرین_3 are interpreted as floating نقطه مقادیر.\n\
INCREMENT is usually مثبت إذا اولی_3 is smaller than آخرین_3, and\n\
INCREMENT is usually negative إذا اولی_3 is greater than آخرین_3.\n\
INCREMENT must not be 0; none از اولی_3, INCREMENT and آخرین_3 may be NaN.\n\
"), stdout);
      fputs (_("\
FORMAT must be suitable لكل printing one آرگومان از type 'double';\n\
it defaults مقصد %.PRECf إذا اولی_3, INCREMENT, and آخرین_3 are all fixed نقطه\n\
decimal numbers with بیشینه precision PREC, and مقصد %g otherwise.\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* A command-line operand.  */
بنية عملوند_2
{
  /* Its value, converted to 'long double'.  */
  دراز مزدوج مقدار;

  /* Its print width, if it were printed out in a form similar to its
     input form.  An input like "-.1" is treated like "-0.1", and an
     input like "1." is treated like "1", but otherwise widths are
     left alone.  */
  size_t عرض;

  /* Number of digits after the decimal point, or INT_MAX if the
     number can't easily be expressed as a fixed-point number.  */
  صحيح precision;
};
تعريفنوع بنية عملوند_2 عملوند_2;

/* Description of what a number-generating format will generate.  */
بنية layout
{
  /* Number of bytes before and after the number.  */
  size_t پیشوند_len;
  size_t suffix_len;
};

/* Read a long double value from the command line.
   Return if the string is correct else signal error.  */

ساكن عملوند_2
پیمایش_arg (حرف ثابت *arg)
{
  عملوند_2 ret;

  إذا (! xstrtold (arg, عدم, &ret.مقدار, cl_strtold))
    {
      error (0, 0, _("invalid floating point argument: %s"), quote (arg));
      usage (EXIT_FAILURE);
    }

  إذا (isnan (ret.مقدار))
    {
      error (0, 0, _("invalid %s argument: %s"), quote_n (0, "not-a-number"),
             quote_n (1, arg));
      usage (EXIT_FAILURE);
    }

  /* We don't output spaces or '+' so don't include in width */
  طالما (آیا_فاصله (مقصد_uchar (*arg)) || *arg == '+')
    arg++;

  /* Default to auto width and precision.  */
  ret.عرض = 0;
  ret.precision = بیشترین_صحیح;

  /* Use no precision (and possibly fast generation) for integers.  */
  حرف ثابت *decimal_نقطه = strchr (arg, '.');
  إذا (! decimal_نقطه && ! strchr (arg, 'p') /* not a hex float */)
    ret.precision = 0;

  /* auto set width and precision for decimal inputs.  */
  إذا (! arg[strcspn (arg, "xX")] && isfinite (ret.مقدار))
    {
      size_t fraction_len = 0;
      ret.عرض = طول_رشته (arg);

      إذا (decimal_نقطه)
        {
          fraction_len = strcspn (decimal_نقطه + 1, "eE");
          إذا (fraction_len <= بیشترین_صحیح)
            ret.precision = fraction_len;
          ret.عرض += (fraction_len == 0                       /* #.  -> #   */
                        ? -1
                        : (decimal_نقطه == arg                 /* .#  -> 0.# */
                           || !c_isdigit (decimal_نقطه[-1]))); /* -.# -> 0.# */
        }
      حرف ثابت *e = strchr (arg, 'e');
      إذا (! e)
        e = strchr (arg, 'E');
      إذا (e)
        {
          دراز exponent = MAX (strtol (e + 1, عدم, 10), -LONG_MAX);
          ret.precision += exponent < 0 ? -exponent
                                        : - MIN (ret.precision, exponent);
          /* Don't account for e.... in the width since this is not output.  */
          ret.عرض -= طول_رشته (arg) - (e - arg);
          /* Adjust the width as per the exponent.  */
          إذا (exponent < 0)
            {
              إذا (decimal_نقطه)
                {
                  إذا (e == decimal_نقطه + 1) /* undo #. -> # above  */
                    ret.عرض++;
                }
              وإلا
                ret.عرض++;
              exponent = -exponent;
            }
          وإلا
            {
              إذا (decimal_نقطه && ret.precision == 0 && fraction_len)
                ret.عرض--; /* discount space for '.'  */
              exponent -= MIN (fraction_len, exponent);
            }
          ret.عرض += exponent;
        }
    }

  أرجع ret;
}

/* If FORMAT is a valid printf format for a double argument, return
   its long double equivalent, allocated from dynamic storage, and
   store into *LAYOUT a description of the output layout; otherwise,
   report an error and exit.  */

ساكن حرف ثابت *
long_double_format (حرف ثابت *fmt, بنية layout *layout)
{
  size_t i;
  size_t پیشوند_len = 0;

  لكل (i = 0; ! (fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      إذا (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      پیشوند_len++;
    }

  i++;
  i += strspn (fmt + i, "-+#0 '");
  i += strspn (fmt + i, "0123456789");
  إذا (fmt[i] == '.')
    {
      i++;
      i += strspn (fmt + i, "0123456789");
    }

  size_t درازی_modifier_جابه_جایی = i;
  bool has_L = (fmt[i] == 'L');
  i += has_L;
  إذا (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));
  إذا (! strchr ("efgaEFGA", fmt[i]))
    error (EXIT_FAILURE, 0,
           _("format %s has unknown %%%c directive"), quote (fmt), fmt[i]);

  size_t suffix_len = 0;
  لكل (i++; ; i += (fmt[i] == '%') + 1)
    إذا (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));
    وإلا إذا (fmt[i])
      suffix_len++;
    وإلا
      {
        size_t format_اندازه = i + 1;
        حرف *ldfmt = xmalloc (format_اندازه + 1);
        کپی_حافظه (ldfmt, fmt, درازی_modifier_جابه_جایی);
        ldfmt[درازی_modifier_جابه_جایی] = 'L';
        strcpy (ldfmt + درازی_modifier_جابه_جایی + 1,
                fmt + درازی_modifier_جابه_جایی + has_L);
        layout->پیشوند_len = پیشوند_len;
        layout->suffix_len = suffix_len;
        أرجع ldfmt;
      }
}

/* Actually print the sequence of numbers in the specified range, with the
   given or default stepping and format.  */

ساكن فراغ
print_numbers (حرف ثابت *fmt, بنية layout layout,
               دراز مزدوج اولی, دراز مزدوج گام, دراز مزدوج آخرین)
{
  bool بیرون_از_range = (گام < 0 ? اولی < آخرین : آخرین < اولی);

  إذا (! بیرون_از_range)
    {
      دراز مزدوج x = اولی;

      لكل (دراز مزدوج i = 1; ; i++)
        {
          دراز مزدوج x0 = x;
          إذا (چاپ_کن (fmt, x) < 0)
            write_error ();
          إذا (بیرون_از_range)
            اكسر;

          /* Mathematically equivalent to 'x += step;', and typically
             less subject to rounding error.  */
          x = اولی + i * گام;

          بیرون_از_range = (گام < 0 ? x < آخرین : آخرین < x);

          إذا (بیرون_از_range)
            {
              /* If the number just past LAST prints as a value equal
                 to LAST, and prints differently from the previous
                 number, then print the number.  This avoids problems
                 with rounding.  For example, with the x86 it causes
                 "seq 0 0.000001 0.000003" to print 0.000003 instead
                 of stopping at 0.000002.  */

              bool print_extra_عدد = false;
              إذا (locale_ok)
                setlocale (LC_NUMERIC, "C");
              حرف *x_str;
              صحيح x_strlen = asprintf (&x_str, fmt, x);
              إذا (locale_ok)
                setlocale (LC_NUMERIC, "");
              إذا (x_strlen < 0)
                xalloc_die ();
              x_str[x_strlen - layout.suffix_len] = '\0';

              دراز مزدوج x_val;
              إذا (xstrtold (x_str + layout.پیشوند_len, عدم,
                            &x_val, cl_strtold)
                  && x_val == آخرین)
                {
                  حرف *x0_str = عدم;
                  صحيح x0_strlen = asprintf (&x0_str, fmt, x0);
                  إذا (x0_strlen < 0)
                    xalloc_die ();
                  x0_str[x0_strlen - layout.suffix_len] = '\0';
                  print_extra_عدد = !streq (x0_str, x_str);
                  حرر (x0_str);
                }

              حرر (x_str);
              إذا (! print_extra_عدد)
                اكسر;
            }

          إذا (fputs (separator, stdout) == EOF)
            write_error ();
        }

      إذا (fputs (terminator, stdout) == EOF)
        write_error ();
    }
}

/* Return the default format given FIRST, STEP, and LAST.  */
ساكن حرف ثابت *
get_default_format (عملوند_2 اولی, عملوند_2 گام, عملوند_2 آخرین)
{
  ساكن حرف format_buf[حجم "%0.Lf" + 2 * INT_STRLEN_BOUND (صحيح)];

  صحيح prec = MAX (اولی.precision, گام.precision);

  إذا (prec != بیشترین_صحیح && آخرین.precision != بیشترین_صحیح)
    {
      إذا (equal_عرض)
        {
          /* increase first_width by any increased precision in step */
          size_t اولی_عرض = اولی.عرض + (prec - اولی.precision);
          /* adjust last_width to use precision from first/step */
          size_t آخرین_عرض = آخرین.عرض + (prec - آخرین.precision);
          إذا (آخرین.precision && prec == 0)
            آخرین_عرض--;  /* don't include space for '.' */
          إذا (آخرین.precision == 0 && prec)
            آخرین_عرض++;  /* include space for '.' */
          إذا (اولی.precision == 0 && prec)
            اولی_عرض++;  /* include space for '.' */
          size_t عرض = MAX (اولی_عرض, آخرین_عرض);
          إذا (عرض <= بیشترین_صحیح)
            {
              صحيح w = عرض;
              sprintf (format_buf, "%%0%d.%dLf", w, prec);
              أرجع format_buf;
            }
        }
      وإلا
        {
          sprintf (format_buf, "%%.%dLf", prec);
          أرجع format_buf;
        }
    }

  أرجع "%Lg";
}

/* The nonempty char array P represents a valid non-negative decimal integer.
   ENDP points just after the last char in P.
   Adjust the array to describe the next-larger integer and return
   whether this grows the array by one on the left.  */
ساكن bool
incr_grows (حرف *p, حرف *endp)
{
  افعل
    {
      endp--;
      إذا (*endp < '9')
        {
          (*endp)++;
          أرجع false;
        }
      *endp = '0';
    }
  طالما (p < endp);

  p[-1] = '1';
  أرجع true;
}

/* Compare A and B with lengths A_LEN and B_LEN.  A and B are integers
   represented by nonempty arrays of digits without redundant leading '0'.
   Return negative if A < B, 0 if A = B, positive if A > B.  */
ساكن صحيح
cmp (حرف ثابت *a, idx_t a_len, حرف ثابت *b, idx_t b_len)
{
  أرجع a_len == b_len ? memcmp (a, b, a_len) : _GL_CMP (a_len, b_len);
}

/* Trim leading 0's from S, but if S is all 0's, leave one.
   Return a pointer to the trimmed string.  */
ATTRIBUTE_خالص
ساكن حرف ثابت *
trim_leading_zeros (حرف ثابت *s)
{
  حرف ثابت *p = s;
  طالما (*s == '0')
    ++s;

  /* If there were only 0's, back up, to leave one.  */
  إذا (!*s && s != p)
    --s;
  أرجع s;
}

/* Print all whole numbers from A to B, inclusive -- to stdout, each
   followed by a newline.  Then exit.  */
ساكن فراغ
seq_fast (حرف ثابت *a, حرف ثابت *b, uintmax_t گام)
{
  /* Skip past any redundant leading '0's.  Without this, our naive cmp
     function would declare 000 to be larger than 99.  */
  a = trim_leading_zeros (a);
  b = trim_leading_zeros (b);

  idx_t p_len = طول_رشته (a);
  idx_t b_len = طول_رشته (b);
  bool inf = b_len == 3 && memeq (b, "inf", 4);

  /* Allow for at least 31 digits without realloc.
     1 more than p_len is needed for the inf case.  */
  تعداد { INITIAL_ALLOC_DIGITS = 31 };
  idx_t inc_اندازه = MAX (MAX (p_len + 1, b_len), INITIAL_ALLOC_DIGITS);
  /* Ensure we only increase by at most 1 digit at buffer boundaries.  */
  static_assert (SEQ_FAST_گام_حد_DIGITS < INITIAL_ALLOC_DIGITS - 1);

  /* Copy A (sans NUL) to end of new buffer.  */
  حرف *p0 = xmalloc (inc_اندازه);
  حرف *endp = p0 + inc_اندازه;
  حرف *p = کپی_حافظه (endp - p_len, a, p_len);

  /* Reduce number of write calls which is seen to
     give a speed-up of more than 2x over naive stdio code
     when printing the first 10^9 integers.  */
  حرف buf[BUFSIZ];
  حرف *buf_انتها = buf + حجم buf;
  حرف *bufp = buf;

  طالما (inf || cmp (p, endp - p, b, b_len) <= 0)
    {
      /* Append number, flushing output buffer while the number's
         digits do not fit with room for a separator or terminator.  */
      حرف *pp = p;
      طالما (buf_انتها - bufp <= endp - pp)
        {
          کپی_حافظه (bufp, pp, buf_انتها - bufp);
          pp += buf_انتها - bufp;
          إذا (پر_write (STDOUT_FILENO, buf, حجم buf) != حجم buf)
            write_error ();
          bufp = buf;
        }

      /* The rest of the number, followed by a separator or terminator,
         will fit.  Tentatively append a separator.  */
      bufp = mempcpy (bufp, pp, endp - pp);
      *bufp++ = *separator;

      /* Grow number buffer if needed for the inf case.  */
      إذا (p == p0)
        {
          حرف *new_p0 = xpalloc (عدم, &inc_اندازه, 1, -1, 1);
          idx_t saved_p_len = endp - p;
          endp = new_p0 + inc_اندازه;
          p = کپی_حافظه (endp - saved_p_len, p0, saved_p_len);
          حرر (p0);
          p0 = new_p0;
        }

      /* Compute next number.  */
      لكل (uintmax_t n_incr = گام; n_incr; n_incr--)
        p -= incr_grows (p, endp);
    }

  /* Write the remaining buffered output with a terminator instead of
     a separator.  */
  idx_t مانده = bufp - buf;
  إذا (مانده)
    {
      bufp[-1] = *terminator;
      إذا (پر_write (STDOUT_FILENO, buf, مانده) != مانده)
        write_error ();
    }

  خروج (EXIT_SUCCESS);
}

/* Return true if S consists of at least one digit and no non-digits.  */
ATTRIBUTE_خالص
ساكن bool
all_digits_p (حرف ثابت *s)
{
  size_t n = طول_رشته (s);
  أرجع c_isdigit (s[0]) && n == strspn (s, "0123456789");
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  /* The printf(3) format used for output.  */
  حرف ثابت *format_str = عدم;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We have to handle negative numbers in the command line but this
     conflicts with the command line arguments.  So explicitly check first
     whether the next argument looks like a negative number.  */
  طالما (optind < argc)
    {
      صحيح optc;
      إذا (argv[optind][0] == '-'
          && ((optc = argv[optind][1]) == '.' || c_isdigit (optc)))
        {
          /* means negative number */
          اكسر;
        }

      optc = getopt_long (argc, argv, "+f:s:w", long_options, عدم);
      إذا (optc == -1)
        اكسر;

      بدل (optc)
        {
        حالة 'f':
          format_str = optarg;
          اكسر;

        حالة 's':
          separator = optarg;
          اكسر;

        حالة 'w':
          equal_عرض = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  صحيح n_آرگومانها = argc - optind;
  إذا (n_آرگومانها < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  إذا (3 < n_آرگومانها)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 3]));
      usage (EXIT_FAILURE);
    }

  بنية layout layout = { 0, 0 };
  إذا (format_str)
    format_str = long_double_format (format_str, &layout);

  إذا (format_str != عدم && equal_عرض)
    {
      error (0, 0, _("format string may not be specified"
                     " when printing equal width strings"));
      usage (EXIT_FAILURE);
    }

  حرف ثابت *user_شروع = n_آرگومانها == 1 ? "1" : argv[optind];

  /* If the following hold:
     - no format string, [FIXME: relax this, eventually]
     - integer start (or no start)
     - integer end
     - integer increment <= SEQ_FAST_STEP_LIMIT
     then use the much more efficient integer-only code,
     operating on arbitrarily large numbers.  */
  عملوند_2 گام = { 1, 1, 0 };
  bool fast_گام_ok = false;
  إذا (n_آرگومانها != 3
      || (all_digits_p (argv[optind + 1])
          && xstrtold (argv[optind + 1], عدم, &گام.مقدار, cl_strtold)
          && 0 < گام.مقدار && گام.مقدار <= SEQ_FAST_گام_حد))
    fast_گام_ok = true;

  إذا (all_digits_p (argv[optind])
      && (n_آرگومانها == 1 || all_digits_p (argv[optind + 1]))
      && (n_آرگومانها < 3 || (fast_گام_ok
                         && all_digits_p (argv[optind + 2])))
      && !equal_عرض && !format_str && طول_رشته (separator) == 1)
    {
      حرف ثابت *s1 = user_شروع;
      حرف ثابت *s2 = argv[optind + (n_آرگومانها - 1)];
      seq_fast (s1, s2, گام.مقدار);
    }

  عملوند_2 اولی = { 1, 1, 0 };
  عملوند_2 آخرین = پیمایش_arg (argv[optind++]);

  إذا (optind < argc)
    {
      اولی = آخرین;
      آخرین = پیمایش_arg (argv[optind++]);

      إذا (optind < argc)
        {
          گام = آخرین;
          إذا (گام.مقدار == 0)
            {
              error (0, 0, _("invalid Zero increment value: %s"),
                     quote (argv[optind - 1]));
              usage (EXIT_FAILURE);
            }

          آخرین = پیمایش_arg (argv[optind++]);
        }
    }

  /* Try the fast method again, for integers of the form 1e1 etc.,
     or "inf" end value.  */
  إذا (اولی.precision == 0 && گام.precision == 0 && آخرین.precision == 0
      && isfinite (اولی.مقدار) && 0 <= اولی.مقدار && 0 <= آخرین.مقدار
      && 0 < گام.مقدار && گام.مقدار <= SEQ_FAST_گام_حد
      && !equal_عرض && !format_str && طول_رشته (separator) == 1)
    {
      حرف *s1 = (all_digits_p (user_شروع)
                  ? xstrdup (user_شروع) : xasprintf ("%0.Lf", اولی.مقدار));
      /* Ensure "inf" is used.  */
      حرف *s2 = (! isfinite (آخرین.مقدار)
                  ? xstrdup ("inf") : xasprintf ("%0.Lf", آخرین.مقدار));

      إذا (*s1 != '-' && *s2 != '-')
        seq_fast (s1, s2, گام.مقدار);

      حرر (s1);
      حرر (s2);
      /* Upon any failure, let the more general code deal with it.  */
    }

  إذا (format_str == عدم)
    format_str = get_default_format (اولی, گام, آخرین);

  print_numbers (format_str, layout, اولی.مقدار, گام.مقدار, آخرین.مقدار);

  main_exit (EXIT_SUCCESS);
}
