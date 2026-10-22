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
#اگر_غیر_تعریف_شدہ isfinite
# define isfinite(x) ((x) * 0 == 0)
#اگر_ختم
#اگر_غیر_تعریف_شدہ isnan
# define isnan(x) ((x) != (x))
#اگر_ختم

/* Limit below which seq_fast has more throughput.
   Determined with: seq 0 200 inf | pv > /dev/null  */
#تعریف_کرو SEQ_FAST_قدم_حد_بندی 200  /* Keep in sync with texinfo description.  */
#تعریف_کرو SEQ_FAST_قدم_حد_بندی_DIGITS 3

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "seq"

#تعریف_کرو AUTHORS proper_name ("Ulrich Drepper")

/* True if the locale settings were honored.  */
ساکن bool locale_ok;

/* If true print all number with equal width.  */
ساکن bool equal_چوڑائی;

/* The string used to separate two numbers.  */
ساکن حرف ثابت *separator = "\n";

/* The string output after all numbers have been output.
   Usually "\n" or "\0".  */
ساکن حرف ثابت terminator[] = "\n";

ساکن ساخت option ثابت long_options[] =
{
  { "equal-width", no_دلیل, تہی, 'w'},
  { "format", required_دلیل, تہی, 'f'},
  { "separator", required_دلیل, تہی, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  { تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... آخری_3\n\
  or:  %s [OPTION]... پہلا_3 آخری_3\n\
  or:  %s [OPTION]... پہلا_3 INCREMENT آخری_3\n\
"), پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
Print numbers from پہلا_3 مقصد آخری_3, اندر اقدام کا INCREMENT.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --format=FORMAT\n\
         use لکھو style floating-نقطہ FORMAT\n\
"));
      oputs (_("\
  -s, --separator=STRING\n\
         use STRING مقصد separate numbers (طےشدہ: \\n)\n\
"));
      oputs (_("\
  -w, --equal-چوڑائی\n\
         equalize چوڑائی by padding with leading zeroes\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If پہلا_3 or INCREMENT is omitted, it defaults مقصد 1.  That is, an\n\
omitted INCREMENT defaults مقصد 1 even when آخری_3 is smaller than پہلا_3.\n\
The sequence کا numbers سرے when the مجموع کا the current نمبر and\n\
INCREMENT would become greater than آخری_3.\n\
پہلا_3, INCREMENT, and آخری_3 are interpreted as floating نقطہ اقدار.\n\
INCREMENT is usually مثبت اگر پہلا_3 is smaller than آخری_3, and\n\
INCREMENT is usually negative اگر پہلا_3 is greater than آخری_3.\n\
INCREMENT must not be 0; none کا پہلا_3, INCREMENT and آخری_3 may be NaN.\n\
"), stdout);
      fputs (_("\
FORMAT must be suitable برائے printing one دلیل کا type 'double';\n\
it defaults مقصد %.PRECf اگر پہلا_3, INCREMENT, and آخری_3 are all fixed نقطہ\n\
decimal numbers with زیادہ_سے_زیادہ precision PREC, and مقصد %g otherwise.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* A command-line operand.  */
ساخت عملیہ_2
{
  /* Its value, converted to 'long double'.  */
  طویل دوہرا قدر;

  /* Its print width, if it were printed out in a form similar to its
     input form.  An input like "-.1" is treated like "-0.1", and an
     input like "1." is treated like "1", but otherwise widths are
     left alone.  */
  size_t چوڑائی;

  /* Number of digits after the decimal point, or INT_MAX if the
     number can't easily be expressed as a fixed-point number.  */
  صحیح precision;
};
قسمتعریف ساخت عملیہ_2 عملیہ_2;

/* Description of what a number-generating format will generate.  */
ساخت layout
{
  /* Number of bytes before and after the number.  */
  size_t سابقہ_len;
  size_t suffix_len;
};

/* Read a long double value from the command line.
   Return if the string is correct else signal error.  */

ساکن عملیہ_2
جائزہ_arg (حرف ثابت *arg)
{
  عملیہ_2 ret;

  اگر (! xstrtold (arg, تہی, &ret.قدر, cl_strtold))
    {
      error (0, 0, _("invalid floating point argument: %s"), quote (arg));
      usage (EXIT_FAILURE);
    }

  اگر (isnan (ret.قدر))
    {
      error (0, 0, _("invalid %s argument: %s"), quote_n (0, "not-a-number"),
             quote_n (1, arg));
      usage (EXIT_FAILURE);
    }

  /* We don't output spaces or '+' so don't include in width */
  جب (خلا_ہے (مقصد_uchar (*arg)) || *arg == '+')
    arg++;

  /* Default to auto width and precision.  */
  ret.چوڑائی = 0;
  ret.precision = زیادہ_سے_زیادہ_عدد;

  /* Use no precision (and possibly fast generation) for integers.  */
  حرف ثابت *decimal_نقطہ = strchr (arg, '.');
  اگر (! decimal_نقطہ && ! strchr (arg, 'p') /* not a hex float */)
    ret.precision = 0;

  /* auto set width and precision for decimal inputs.  */
  اگر (! arg[strcspn (arg, "xX")] && isfinite (ret.قدر))
    {
      size_t fraction_len = 0;
      ret.چوڑائی = لڑی_لمبائی (arg);

      اگر (decimal_نقطہ)
        {
          fraction_len = strcspn (decimal_نقطہ + 1, "eE");
          اگر (fraction_len <= زیادہ_سے_زیادہ_عدد)
            ret.precision = fraction_len;
          ret.چوڑائی += (fraction_len == 0                       /* #.  -> #   */
                        ? -1
                        : (decimal_نقطہ == arg                 /* .#  -> 0.# */
                           || !c_isdigit (decimal_نقطہ[-1]))); /* -.# -> 0.# */
        }
      حرف ثابت *e = strchr (arg, 'e');
      اگر (! e)
        e = strchr (arg, 'E');
      اگر (e)
        {
          طویل exponent = MAX (strtol (e + 1, تہی, 10), -LONG_MAX);
          ret.precision += exponent < 0 ? -exponent
                                        : - MIN (ret.precision, exponent);
          /* Don't account for e.... in the width since this is not output.  */
          ret.چوڑائی -= لڑی_لمبائی (arg) - (e - arg);
          /* Adjust the width as per the exponent.  */
          اگر (exponent < 0)
            {
              اگر (decimal_نقطہ)
                {
                  اگر (e == decimal_نقطہ + 1) /* undo #. -> # above  */
                    ret.چوڑائی++;
                }
              ورنہ
                ret.چوڑائی++;
              exponent = -exponent;
            }
          ورنہ
            {
              اگر (decimal_نقطہ && ret.precision == 0 && fraction_len)
                ret.چوڑائی--; /* discount space for '.'  */
              exponent -= MIN (fraction_len, exponent);
            }
          ret.چوڑائی += exponent;
        }
    }

  واپس ret;
}

/* If FORMAT is a valid printf format for a double argument, return
   its long double equivalent, allocated from dynamic storage, and
   store into *LAYOUT a description of the output layout; otherwise,
   report an error and exit.  */

ساکن حرف ثابت *
long_double_format (حرف ثابت *fmt, ساخت layout *layout)
{
  size_t i;
  size_t سابقہ_len = 0;

  برائے (i = 0; ! (fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      اگر (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      سابقہ_len++;
    }

  i++;
  i += strspn (fmt + i, "-+#0 '");
  i += strspn (fmt + i, "0123456789");
  اگر (fmt[i] == '.')
    {
      i++;
      i += strspn (fmt + i, "0123456789");
    }

  size_t لمبائی_modifier_انحراف = i;
  bool has_L = (fmt[i] == 'L');
  i += has_L;
  اگر (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));
  اگر (! strchr ("efgaEFGA", fmt[i]))
    error (EXIT_FAILURE, 0,
           _("format %s has unknown %%%c directive"), quote (fmt), fmt[i]);

  size_t suffix_len = 0;
  برائے (i++; ; i += (fmt[i] == '%') + 1)
    اگر (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));
    ورنہ اگر (fmt[i])
      suffix_len++;
    ورنہ
      {
        size_t format_سائز = i + 1;
        حرف *ldfmt = xmalloc (format_سائز + 1);
        حافظہ_نقل (ldfmt, fmt, لمبائی_modifier_انحراف);
        ldfmt[لمبائی_modifier_انحراف] = 'L';
        strcpy (ldfmt + لمبائی_modifier_انحراف + 1,
                fmt + لمبائی_modifier_انحراف + has_L);
        layout->سابقہ_len = سابقہ_len;
        layout->suffix_len = suffix_len;
        واپس ldfmt;
      }
}

/* Actually print the sequence of numbers in the specified range, with the
   given or default stepping and format.  */

ساکن خالی
print_numbers (حرف ثابت *fmt, ساخت layout layout,
               طویل دوہرا پہلا, طویل دوہرا قدم, طویل دوہرا آخری)
{
  bool برآمد_کا_range = (قدم < 0 ? پہلا < آخری : آخری < پہلا);

  اگر (! برآمد_کا_range)
    {
      طویل دوہرا x = پہلا;

      برائے (طویل دوہرا i = 1; ; i++)
        {
          طویل دوہرا x0 = x;
          اگر (لکھو (fmt, x) < 0)
            write_error ();
          اگر (برآمد_کا_range)
            توڑو;

          /* Mathematically equivalent to 'x += step;', and typically
             less subject to rounding error.  */
          x = پہلا + i * قدم;

          برآمد_کا_range = (قدم < 0 ? x < آخری : آخری < x);

          اگر (برآمد_کا_range)
            {
              /* If the number just past LAST prints as a value equal
                 to LAST, and prints differently from the previous
                 number, then print the number.  This avoids problems
                 with rounding.  For example, with the x86 it causes
                 "seq 0 0.000001 0.000003" to print 0.000003 instead
                 of stopping at 0.000002.  */

              bool print_extra_نمبر = false;
              اگر (locale_ok)
                setlocale (LC_NUMERIC, "C");
              حرف *x_str;
              صحیح x_strlen = asprintf (&x_str, fmt, x);
              اگر (locale_ok)
                setlocale (LC_NUMERIC, "");
              اگر (x_strlen < 0)
                xalloc_die ();
              x_str[x_strlen - layout.suffix_len] = '\0';

              طویل دوہرا x_val;
              اگر (xstrtold (x_str + layout.سابقہ_len, تہی,
                            &x_val, cl_strtold)
                  && x_val == آخری)
                {
                  حرف *x0_str = تہی;
                  صحیح x0_strlen = asprintf (&x0_str, fmt, x0);
                  اگر (x0_strlen < 0)
                    xalloc_die ();
                  x0_str[x0_strlen - layout.suffix_len] = '\0';
                  print_extra_نمبر = !streq (x0_str, x_str);
                  آزاد (x0_str);
                }

              آزاد (x_str);
              اگر (! print_extra_نمبر)
                توڑو;
            }

          اگر (fputs (separator, stdout) == EOF)
            write_error ();
        }

      اگر (fputs (terminator, stdout) == EOF)
        write_error ();
    }
}

/* Return the default format given FIRST, STEP, and LAST.  */
ساکن حرف ثابت *
get_default_format (عملیہ_2 پہلا, عملیہ_2 قدم, عملیہ_2 آخری)
{
  ساکن حرف format_buf[حجم "%0.Lf" + 2 * INT_STRLEN_BOUND (صحیح)];

  صحیح prec = MAX (پہلا.precision, قدم.precision);

  اگر (prec != زیادہ_سے_زیادہ_عدد && آخری.precision != زیادہ_سے_زیادہ_عدد)
    {
      اگر (equal_چوڑائی)
        {
          /* increase first_width by any increased precision in step */
          size_t پہلا_چوڑائی = پہلا.چوڑائی + (prec - پہلا.precision);
          /* adjust last_width to use precision from first/step */
          size_t آخری_چوڑائی = آخری.چوڑائی + (prec - آخری.precision);
          اگر (آخری.precision && prec == 0)
            آخری_چوڑائی--;  /* don't include space for '.' */
          اگر (آخری.precision == 0 && prec)
            آخری_چوڑائی++;  /* include space for '.' */
          اگر (پہلا.precision == 0 && prec)
            پہلا_چوڑائی++;  /* include space for '.' */
          size_t چوڑائی = MAX (پہلا_چوڑائی, آخری_چوڑائی);
          اگر (چوڑائی <= زیادہ_سے_زیادہ_عدد)
            {
              صحیح w = چوڑائی;
              sprintf (format_buf, "%%0%d.%dLf", w, prec);
              واپس format_buf;
            }
        }
      ورنہ
        {
          sprintf (format_buf, "%%.%dLf", prec);
          واپس format_buf;
        }
    }

  واپس "%Lg";
}

/* The nonempty char array P represents a valid non-negative decimal integer.
   ENDP points just after the last char in P.
   Adjust the array to describe the next-larger integer and return
   whether this grows the array by one on the left.  */
ساکن bool
incr_grows (حرف *p, حرف *endp)
{
  کرو
    {
      endp--;
      اگر (*endp < '9')
        {
          (*endp)++;
          واپس false;
        }
      *endp = '0';
    }
  جب (p < endp);

  p[-1] = '1';
  واپس true;
}

/* Compare A and B with lengths A_LEN and B_LEN.  A and B are integers
   represented by nonempty arrays of digits without redundant leading '0'.
   Return negative if A < B, 0 if A = B, positive if A > B.  */
ساکن صحیح
cmp (حرف ثابت *a, idx_t a_len, حرف ثابت *b, idx_t b_len)
{
  واپس a_len == b_len ? memcmp (a, b, a_len) : _GL_CMP (a_len, b_len);
}

/* Trim leading 0's from S, but if S is all 0's, leave one.
   Return a pointer to the trimmed string.  */
ATTRIBUTE_خالص
ساکن حرف ثابت *
trim_leading_zeros (حرف ثابت *s)
{
  حرف ثابت *p = s;
  جب (*s == '0')
    ++s;

  /* If there were only 0's, back up, to leave one.  */
  اگر (!*s && s != p)
    --s;
  واپس s;
}

/* Print all whole numbers from A to B, inclusive -- to stdout, each
   followed by a newline.  Then exit.  */
ساکن خالی
seq_fast (حرف ثابت *a, حرف ثابت *b, uintmax_t قدم)
{
  /* Skip past any redundant leading '0's.  Without this, our naive cmp
     function would declare 000 to be larger than 99.  */
  a = trim_leading_zeros (a);
  b = trim_leading_zeros (b);

  idx_t p_len = لڑی_لمبائی (a);
  idx_t b_len = لڑی_لمبائی (b);
  bool inf = b_len == 3 && memeq (b, "inf", 4);

  /* Allow for at least 31 digits without realloc.
     1 more than p_len is needed for the inf case.  */
  شمار { INITIAL_ALLOC_DIGITS = 31 };
  idx_t inc_سائز = MAX (MAX (p_len + 1, b_len), INITIAL_ALLOC_DIGITS);
  /* Ensure we only increase by at most 1 digit at buffer boundaries.  */
  static_assert (SEQ_FAST_قدم_حد_بندی_DIGITS < INITIAL_ALLOC_DIGITS - 1);

  /* Copy A (sans NUL) to end of new buffer.  */
  حرف *p0 = xmalloc (inc_سائز);
  حرف *endp = p0 + inc_سائز;
  حرف *p = حافظہ_نقل (endp - p_len, a, p_len);

  /* Reduce number of write calls which is seen to
     give a speed-up of more than 2x over naive stdio code
     when printing the first 10^9 integers.  */
  حرف buf[BUFSIZ];
  حرف *buf_انتہا = buf + حجم buf;
  حرف *bufp = buf;

  جب (inf || cmp (p, endp - p, b, b_len) <= 0)
    {
      /* Append number, flushing output buffer while the number's
         digits do not fit with room for a separator or terminator.  */
      حرف *pp = p;
      جب (buf_انتہا - bufp <= endp - pp)
        {
          حافظہ_نقل (bufp, pp, buf_انتہا - bufp);
          pp += buf_انتہا - bufp;
          اگر (بھرا_write (STDOUT_FILENO, buf, حجم buf) != حجم buf)
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
          حرف *new_p0 = xpalloc (تہی, &inc_سائز, 1, -1, 1);
          idx_t saved_p_len = endp - p;
          endp = new_p0 + inc_سائز;
          p = حافظہ_نقل (endp - saved_p_len, p0, saved_p_len);
          آزاد (p0);
          p0 = new_p0;
        }

      /* Compute next number.  */
      برائے (uintmax_t n_incr = قدم; n_incr; n_incr--)
        p -= incr_grows (p, endp);
    }

  /* Write the remaining buffered output with a terminator instead of
     a separator.  */
  idx_t بقایا = bufp - buf;
  اگر (بقایا)
    {
      bufp[-1] = *terminator;
      اگر (بھرا_write (STDOUT_FILENO, buf, بقایا) != بقایا)
        write_error ();
    }

  باہر_نکلو (EXIT_SUCCESS);
}

/* Return true if S consists of at least one digit and no non-digits.  */
ATTRIBUTE_خالص
ساکن bool
all_digits_p (حرف ثابت *s)
{
  size_t n = لڑی_لمبائی (s);
  واپس c_isdigit (s[0]) && n == strspn (s, "0123456789");
}

صحیح
مین (صحیح argc, حرف **argv)
{
  /* The printf(3) format used for output.  */
  حرف ثابت *format_str = تہی;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We have to handle negative numbers in the command line but this
     conflicts with the command line arguments.  So explicitly check first
     whether the next argument looks like a negative number.  */
  جب (optind < argc)
    {
      صحیح optc;
      اگر (argv[optind][0] == '-'
          && ((optc = argv[optind][1]) == '.' || c_isdigit (optc)))
        {
          /* means negative number */
          توڑو;
        }

      optc = getopt_long (argc, argv, "+f:s:w", long_options, تہی);
      اگر (optc == -1)
        توڑو;

      انتخاب (optc)
        {
        حالت 'f':
          format_str = optarg;
          توڑو;

        حالت 's':
          separator = optarg;
          توڑو;

        حالت 'w':
          equal_چوڑائی = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  صحیح n_دلائل = argc - optind;
  اگر (n_دلائل < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  اگر (3 < n_دلائل)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 3]));
      usage (EXIT_FAILURE);
    }

  ساخت layout layout = { 0, 0 };
  اگر (format_str)
    format_str = long_double_format (format_str, &layout);

  اگر (format_str != تہی && equal_چوڑائی)
    {
      error (0, 0, _("format string may not be specified"
                     " when printing equal width strings"));
      usage (EXIT_FAILURE);
    }

  حرف ثابت *user_آغاز = n_دلائل == 1 ? "1" : argv[optind];

  /* If the following hold:
     - no format string, [FIXME: relax this, eventually]
     - integer start (or no start)
     - integer end
     - integer increment <= SEQ_FAST_STEP_LIMIT
     then use the much more efficient integer-only code,
     operating on arbitrarily large numbers.  */
  عملیہ_2 قدم = { 1, 1, 0 };
  bool fast_قدم_ok = false;
  اگر (n_دلائل != 3
      || (all_digits_p (argv[optind + 1])
          && xstrtold (argv[optind + 1], تہی, &قدم.قدر, cl_strtold)
          && 0 < قدم.قدر && قدم.قدر <= SEQ_FAST_قدم_حد_بندی))
    fast_قدم_ok = true;

  اگر (all_digits_p (argv[optind])
      && (n_دلائل == 1 || all_digits_p (argv[optind + 1]))
      && (n_دلائل < 3 || (fast_قدم_ok
                         && all_digits_p (argv[optind + 2])))
      && !equal_چوڑائی && !format_str && لڑی_لمبائی (separator) == 1)
    {
      حرف ثابت *s1 = user_آغاز;
      حرف ثابت *s2 = argv[optind + (n_دلائل - 1)];
      seq_fast (s1, s2, قدم.قدر);
    }

  عملیہ_2 پہلا = { 1, 1, 0 };
  عملیہ_2 آخری = جائزہ_arg (argv[optind++]);

  اگر (optind < argc)
    {
      پہلا = آخری;
      آخری = جائزہ_arg (argv[optind++]);

      اگر (optind < argc)
        {
          قدم = آخری;
          اگر (قدم.قدر == 0)
            {
              error (0, 0, _("invalid Zero increment value: %s"),
                     quote (argv[optind - 1]));
              usage (EXIT_FAILURE);
            }

          آخری = جائزہ_arg (argv[optind++]);
        }
    }

  /* Try the fast method again, for integers of the form 1e1 etc.,
     or "inf" end value.  */
  اگر (پہلا.precision == 0 && قدم.precision == 0 && آخری.precision == 0
      && isfinite (پہلا.قدر) && 0 <= پہلا.قدر && 0 <= آخری.قدر
      && 0 < قدم.قدر && قدم.قدر <= SEQ_FAST_قدم_حد_بندی
      && !equal_چوڑائی && !format_str && لڑی_لمبائی (separator) == 1)
    {
      حرف *s1 = (all_digits_p (user_آغاز)
                  ? xstrdup (user_آغاز) : xasprintf ("%0.Lf", پہلا.قدر));
      /* Ensure "inf" is used.  */
      حرف *s2 = (! isfinite (آخری.قدر)
                  ? xstrdup ("inf") : xasprintf ("%0.Lf", آخری.قدر));

      اگر (*s1 != '-' && *s2 != '-')
        seq_fast (s1, s2, قدم.قدر);

      آزاد (s1);
      آزاد (s2);
      /* Upon any failure, let the more general code deal with it.  */
    }

  اگر (format_str == تہی)
    format_str = get_default_format (پہلا, قدم, آخری);

  print_numbers (format_str, layout, پہلا.قدر, قدم.قدر, آخری.قدر);

  main_exit (EXIT_SUCCESS);
}
