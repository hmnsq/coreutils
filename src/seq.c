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

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "cl-strtod.h"
#अन्तर्भाव "full-write.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xstrtod.h"
#अन्तर्भाव "xvasprintf.h"

/* Roll our own isfinite/isnan rather than using <math.h>, so that we don't
   have to worry about linking -lm just for isfinite.  */
#ifndef है_सीमित
# define है_सीमित(x) ((x) * 0 == 0)
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
स्थिर bool locale_ok;

/* If true print all number with equal width.  */
स्थिर bool equal_width;

/* The string used to separate two numbers.  */
स्थिर वर्ण अचर *separator = "\n";

/* The string output after all numbers have been output.
   Usually "\n" or "\0".  */
स्थिर वर्ण अचर terminator[] = "\n";

स्थिर संरचना option अचर long_options[] =
{
  { "equal-width", no_argument, रिक्त, 'w'},
  { "format", required_argument, रिक्त, 'f'},
  { "separator", required_argument, रिक्त, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  { रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... LAST\n\
  or:  %s [OPTION]... FIRST LAST\n\
  or:  %s [OPTION]... FIRST INCREMENT LAST\n\
"), program_name, program_name, program_name);
      ख_माला_दो (_("\
Print numbers from FIRST to LAST, in steps of INCREMENT.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --format=FORMAT\n\
         use म_लिखो style floating-point FORMAT\n\
"));
      oputs (_("\
  -s, --separator=STRING\n\
         use STRING to separate numbers (स्वभाव: \\n)\n\
"));
      oputs (_("\
  -w, --equal-width\n\
         equalize width by padding with leading zeroes\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
If FIRST or INCREMENT is omitted, it defaults to 1.  That is, an\n\
omitted INCREMENT defaults to 1 even when LAST is smaller than FIRST.\n\
The sequence of numbers ends when the sum of the current सङ्ख्या and\n\
INCREMENT would become greater than LAST.\n\
FIRST, INCREMENT, and LAST are interpreted as floating point मानहरू.\n\
INCREMENT is usually positive यदि FIRST is smaller than LAST, and\n\
INCREMENT is usually negative यदि FIRST is greater than LAST.\n\
INCREMENT must not be 0; none of FIRST, INCREMENT and LAST may be NaN.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
FORMAT must be suitable लागि printing one argument of type 'double';\n\
it defaults to %.PRECf यदि FIRST, INCREMENT, and LAST are all fixed point\n\
decimal numbers with maximum precision PREC, and to %g otherwise.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* A command-line operand.  */
संरचना operand
{
  /* Its value, converted to 'long double'.  */
  दीर्घ द्विगुण मान;

  /* Its print width, if it were printed out in a form similar to its
     input form.  An input like "-.1" is treated like "-0.1", and an
     input like "1." is treated like "1", but otherwise widths are
     left alone.  */
  माप_प्रकार width;

  /* Number of digits after the decimal point, or INT_MAX if the
     number can't easily be expressed as a fixed-point number.  */
  पूर्णाङ्क precision;
};
प्रकारनिर्देश संरचना operand operand;

/* Description of what a number-generating format will generate.  */
संरचना layout
{
  /* Number of bytes before and after the number.  */
  माप_प्रकार prefix_len;
  माप_प्रकार suffix_len;
};

/* Read a long double value from the command line.
   Return if the string is correct else signal error.  */

स्थिर operand
scan_arg (वर्ण अचर *arg)
{
  operand ret;

  यदि (! xstrtold (arg, रिक्त, &ret.मान, cl_strtold))
    {
      error (0, 0, _("invalid floating point argument: %s"), quote (arg));
      usage (निकास_त्रुटि);
    }

  यदि (isnan (ret.मान))
    {
      error (0, 0, _("invalid %s argument: %s"), quote_n (0, "not-a-number"),
             quote_n (1, arg));
      usage (निकास_त्रुटि);
    }

  /* We don't output spaces or '+' so don't include in width */
  जबसम्म (है_खाली (to_uchar (*arg)) || *arg == '+')
    arg++;

  /* Default to auto width and precision.  */
  ret.width = 0;
  ret.precision = पूर्णांक_उच्च;

  /* Use no precision (and possibly fast generation) for integers.  */
  वर्ण अचर *decimal_point = म_अक्षर (arg, '.');
  यदि (! decimal_point && ! म_अक्षर (arg, 'p') /* not a hex float */)
    ret.precision = 0;

  /* auto set width and precision for decimal inputs.  */
  यदि (! arg[म_खोज (arg, "xX")] && है_सीमित (ret.मान))
    {
      माप_प्रकार fraction_len = 0;
      ret.width = म_माप (arg);

      यदि (decimal_point)
        {
          fraction_len = म_खोज (decimal_point + 1, "eE");
          यदि (fraction_len <= पूर्णांक_उच्च)
            ret.precision = fraction_len;
          ret.width += (fraction_len == 0                       /* #.  -> #   */
                        ? -1
                        : (decimal_point == arg                 /* .#  -> 0.# */
                           || !c_isdigit (decimal_point[-1]))); /* -.# -> 0.# */
        }
      वर्ण अचर *e = म_अक्षर (arg, 'e');
      यदि (! e)
        e = म_अक्षर (arg, 'E');
      यदि (e)
        {
          दीर्घ exponent = MAX (म_से_दीर्घ (e + 1, रिक्त, 10), -दीर्घ_उच्च);
          ret.precision += exponent < 0 ? -exponent
                                        : - MIN (ret.precision, exponent);
          /* Don't account for e.... in the width since this is not output.  */
          ret.width -= म_माप (arg) - (e - arg);
          /* Adjust the width as per the exponent.  */
          यदि (exponent < 0)
            {
              यदि (decimal_point)
                {
                  यदि (e == decimal_point + 1) /* undo #. -> # above  */
                    ret.width++;
                }
              अन्यथा
                ret.width++;
              exponent = -exponent;
            }
          अन्यथा
            {
              यदि (decimal_point && ret.precision == 0 && fraction_len)
                ret.width--; /* discount space for '.'  */
              exponent -= MIN (fraction_len, exponent);
            }
          ret.width += exponent;
        }
    }

  फर्काऊ ret;
}

/* If FORMAT is a valid printf format for a double argument, return
   its long double equivalent, allocated from dynamic storage, and
   store into *LAYOUT a description of the output layout; otherwise,
   report an error and exit.  */

स्थिर वर्ण अचर *
long_double_format (वर्ण अचर *fmt, संरचना layout *layout)
{
  माप_प्रकार i;
  माप_प्रकार prefix_len = 0;

  लागि (i = 0; ! (fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      यदि (!fmt[i])
        error (निकास_त्रुटि, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  i += म_अखोज (fmt + i, "-+#0 '");
  i += म_अखोज (fmt + i, "0123456789");
  यदि (fmt[i] == '.')
    {
      i++;
      i += म_अखोज (fmt + i, "0123456789");
    }

  माप_प्रकार length_modifier_offset = i;
  bool has_L = (fmt[i] == 'L');
  i += has_L;
  यदि (fmt[i] == '\0')
    error (निकास_त्रुटि, 0, _("format %s ends in %%"), quote (fmt));
  यदि (! म_अक्षर ("efgaEFGA", fmt[i]))
    error (निकास_त्रुटि, 0,
           _("format %s has unknown %%%c directive"), quote (fmt), fmt[i]);

  माप_प्रकार suffix_len = 0;
  लागि (i++; ; i += (fmt[i] == '%') + 1)
    यदि (fmt[i] == '%' && fmt[i + 1] != '%')
      error (निकास_त्रुटि, 0, _("format %s has too many %% directives"),
             quote (fmt));
    अन्यथा यदि (fmt[i])
      suffix_len++;
    अन्यथा
      {
        माप_प्रकार format_size = i + 1;
        वर्ण *ldfmt = xmalloc (format_size + 1);
        स_नकल (ldfmt, fmt, length_modifier_offset);
        ldfmt[length_modifier_offset] = 'L';
        म_नकल (ldfmt + length_modifier_offset + 1,
                fmt + length_modifier_offset + has_L);
        layout->prefix_len = prefix_len;
        layout->suffix_len = suffix_len;
        फर्काऊ ldfmt;
      }
}

/* Actually print the sequence of numbers in the specified range, with the
   given or default stepping and format.  */

स्थिर शून्य
print_numbers (वर्ण अचर *fmt, संरचना layout layout,
               दीर्घ द्विगुण first, दीर्घ द्विगुण step, दीर्घ द्विगुण last)
{
  bool out_of_range = (step < 0 ? first < last : last < first);

  यदि (! out_of_range)
    {
      दीर्घ द्विगुण x = first;

      लागि (दीर्घ द्विगुण i = 1; ; i++)
        {
          दीर्घ द्विगुण x0 = x;
          यदि (म_लिखो (fmt, x) < 0)
            write_error ();
          यदि (out_of_range)
            रोक;

          /* Mathematically equivalent to 'x += step;', and typically
             less subject to rounding error.  */
          x = first + i * step;

          out_of_range = (step < 0 ? x < last : last < x);

          यदि (out_of_range)
            {
              /* If the number just past LAST prints as a value equal
                 to LAST, and prints differently from the previous
                 number, then print the number.  This avoids problems
                 with rounding.  For example, with the x86 it causes
                 "seq 0 0.000001 0.000003" to print 0.000003 instead
                 of stopping at 0.000002.  */

              bool print_extra_सङ्ख्या = false;
              यदि (locale_ok)
                रखो_क्षेत्र (LC_NUMERIC, "C");
              वर्ण *x_str;
              पूर्णाङ्क x_strlen = asprintf (&x_str, fmt, x);
              यदि (locale_ok)
                रखो_क्षेत्र (LC_NUMERIC, "");
              यदि (x_strlen < 0)
                xalloc_die ();
              x_str[x_strlen - layout.suffix_len] = '\0';

              दीर्घ द्विगुण x_val;
              यदि (xstrtold (x_str + layout.prefix_len, रिक्त,
                            &x_val, cl_strtold)
                  && x_val == last)
                {
                  वर्ण *x0_str = रिक्त;
                  पूर्णाङ्क x0_strlen = asprintf (&x0_str, fmt, x0);
                  यदि (x0_strlen < 0)
                    xalloc_die ();
                  x0_str[x0_strlen - layout.suffix_len] = '\0';
                  print_extra_सङ्ख्या = !streq (x0_str, x_str);
                  मुक्त (x0_str);
                }

              मुक्त (x_str);
              यदि (! print_extra_सङ्ख्या)
                रोक;
            }

          यदि (ख_माला_दो (separator, मानक_निकास) == खातापूर्ण)
            write_error ();
        }

      यदि (ख_माला_दो (terminator, मानक_निकास) == खातापूर्ण)
        write_error ();
    }
}

/* Return the default format given FIRST, STEP, and LAST.  */
स्थिर वर्ण अचर *
get_default_format (operand first, operand step, operand last)
{
  स्थिर वर्ण format_buf[परिमाण "%0.Lf" + 2 * INT_STRLEN_BOUND (पूर्णाङ्क)];

  पूर्णाङ्क prec = MAX (first.precision, step.precision);

  यदि (prec != पूर्णांक_उच्च && last.precision != पूर्णांक_उच्च)
    {
      यदि (equal_width)
        {
          /* increase first_width by any increased precision in step */
          माप_प्रकार first_width = first.width + (prec - first.precision);
          /* adjust last_width to use precision from first/step */
          माप_प्रकार last_width = last.width + (prec - last.precision);
          यदि (last.precision && prec == 0)
            last_width--;  /* don't include space for '.' */
          यदि (last.precision == 0 && prec)
            last_width++;  /* include space for '.' */
          यदि (first.precision == 0 && prec)
            first_width++;  /* include space for '.' */
          माप_प्रकार width = MAX (first_width, last_width);
          यदि (width <= पूर्णांक_उच्च)
            {
              पूर्णाङ्क w = width;
              प्र_लिखो (format_buf, "%%0%d.%dLf", w, prec);
              फर्काऊ format_buf;
            }
        }
      अन्यथा
        {
          प्र_लिखो (format_buf, "%%.%dLf", prec);
          फर्काऊ format_buf;
        }
    }

  फर्काऊ "%Lg";
}

/* The nonempty char array P represents a valid non-negative decimal integer.
   ENDP points just after the last char in P.
   Adjust the array to describe the next-larger integer and return
   whether this grows the array by one on the left.  */
स्थिर bool
incr_grows (वर्ण *p, वर्ण *endp)
{
  गर
    {
      endp--;
      यदि (*endp < '9')
        {
          (*endp)++;
          फर्काऊ false;
        }
      *endp = '0';
    }
  जबसम्म (p < endp);

  p[-1] = '1';
  फर्काऊ true;
}

/* Compare A and B with lengths A_LEN and B_LEN.  A and B are integers
   represented by nonempty arrays of digits without redundant leading '0'.
   Return negative if A < B, 0 if A = B, positive if A > B.  */
स्थिर पूर्णाङ्क
cmp (वर्ण अचर *a, idx_t a_len, वर्ण अचर *b, idx_t b_len)
{
  फर्काऊ a_len == b_len ? स_भेद (a, b, a_len) : _GL_CMP (a_len, b_len);
}

/* Trim leading 0's from S, but if S is all 0's, leave one.
   Return a pointer to the trimmed string.  */
ATTRIBUTE_PURE
स्थिर वर्ण अचर *
trim_leading_zeros (वर्ण अचर *s)
{
  वर्ण अचर *p = s;
  जबसम्म (*s == '0')
    ++s;

  /* If there were only 0's, back up, to leave one.  */
  यदि (!*s && s != p)
    --s;
  फर्काऊ s;
}

/* Print all whole numbers from A to B, inclusive -- to stdout, each
   followed by a newline.  Then exit.  */
स्थिर शून्य
seq_fast (वर्ण अचर *a, वर्ण अचर *b, uintmax_t step)
{
  /* Skip past any redundant leading '0's.  Without this, our naive cmp
     function would declare 000 to be larger than 99.  */
  a = trim_leading_zeros (a);
  b = trim_leading_zeros (b);

  idx_t p_len = म_माप (a);
  idx_t b_len = म_माप (b);
  bool inf = b_len == 3 && memeq (b, "inf", 4);

  /* Allow for at least 31 digits without realloc.
     1 more than p_len is needed for the inf case.  */
  गणना { INITIAL_ALLOC_DIGITS = 31 };
  idx_t inc_size = MAX (MAX (p_len + 1, b_len), INITIAL_ALLOC_DIGITS);
  /* Ensure we only increase by at most 1 digit at buffer boundaries.  */
  static_assert (SEQ_FAST_STEP_LIMIT_DIGITS < INITIAL_ALLOC_DIGITS - 1);

  /* Copy A (sans NUL) to end of new buffer.  */
  वर्ण *p0 = xmalloc (inc_size);
  वर्ण *endp = p0 + inc_size;
  वर्ण *p = स_नकल (endp - p_len, a, p_len);

  /* Reduce number of write calls which is seen to
     give a speed-up of more than 2x over naive stdio code
     when printing the first 10^9 integers.  */
  वर्ण buf[बफ_मान];
  वर्ण *buf_end = buf + परिमाण buf;
  वर्ण *bufp = buf;

  जबसम्म (inf || cmp (p, endp - p, b, b_len) <= 0)
    {
      /* Append number, flushing output buffer while the number's
         digits do not fit with room for a separator or terminator.  */
      वर्ण *pp = p;
      जबसम्म (buf_end - bufp <= endp - pp)
        {
          स_नकल (bufp, pp, buf_end - bufp);
          pp += buf_end - bufp;
          यदि (full_write (STDOUT_FILENO, buf, परिमाण buf) != परिमाण buf)
            write_error ();
          bufp = buf;
        }

      /* The rest of the number, followed by a separator or terminator,
         will fit.  Tentatively append a separator.  */
      bufp = mempcpy (bufp, pp, endp - pp);
      *bufp++ = *separator;

      /* Grow number buffer if needed for the inf case.  */
      यदि (p == p0)
        {
          वर्ण *new_p0 = xpalloc (रिक्त, &inc_size, 1, -1, 1);
          idx_t saved_p_len = endp - p;
          endp = new_p0 + inc_size;
          p = स_नकल (endp - saved_p_len, p0, saved_p_len);
          मुक्त (p0);
          p0 = new_p0;
        }

      /* Compute next number.  */
      लागि (uintmax_t n_incr = step; n_incr; n_incr--)
        p -= incr_grows (p, endp);
    }

  /* Write the remaining buffered output with a terminator instead of
     a separator.  */
  idx_t remaining = bufp - buf;
  यदि (remaining)
    {
      bufp[-1] = *terminator;
      यदि (full_write (STDOUT_FILENO, buf, remaining) != remaining)
        write_error ();
    }

  निकास (निकास_सफल);
}

/* Return true if S consists of at least one digit and no non-digits.  */
ATTRIBUTE_PURE
स्थिर bool
all_digits_p (वर्ण अचर *s)
{
  माप_प्रकार n = म_माप (s);
  फर्काऊ c_isdigit (s[0]) && n == म_अखोज (s, "0123456789");
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  /* The printf(3) format used for output.  */
  वर्ण अचर *format_str = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  /* We have to handle negative numbers in the command line but this
     conflicts with the command line arguments.  So explicitly check first
     whether the next argument looks like a negative number.  */
  जबसम्म (optind < argc)
    {
      पूर्णाङ्क optc;
      यदि (argv[optind][0] == '-'
          && ((optc = argv[optind][1]) == '.' || c_isdigit (optc)))
        {
          /* means negative number */
          रोक;
        }

      optc = getopt_long (argc, argv, "+f:s:w", long_options, रिक्त);
      यदि (optc == -1)
        रोक;

      विकल्प (optc)
        {
        पक्ष 'f':
          format_str = optarg;
          रोक;

        पक्ष 's':
          separator = optarg;
          रोक;

        पक्ष 'w':
          equal_width = true;
          रोक;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (निकास_त्रुटि);
        }
    }

  पूर्णाङ्क n_args = argc - optind;
  यदि (n_args < 1)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  यदि (3 < n_args)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 3]));
      usage (निकास_त्रुटि);
    }

  संरचना layout layout = { 0, 0 };
  यदि (format_str)
    format_str = long_double_format (format_str, &layout);

  यदि (format_str != रिक्त && equal_width)
    {
      error (0, 0, _("format string may not be specified"
                     " when printing equal width strings"));
      usage (निकास_त्रुटि);
    }

  वर्ण अचर *user_start = n_args == 1 ? "1" : argv[optind];

  /* If the following hold:
     - no format string, [FIXME: relax this, eventually]
     - integer start (or no start)
     - integer end
     - integer increment <= SEQ_FAST_STEP_LIMIT
     then use the much more efficient integer-only code,
     operating on arbitrarily large numbers.  */
  operand step = { 1, 1, 0 };
  bool fast_step_ok = false;
  यदि (n_args != 3
      || (all_digits_p (argv[optind + 1])
          && xstrtold (argv[optind + 1], रिक्त, &step.मान, cl_strtold)
          && 0 < step.मान && step.मान <= SEQ_FAST_STEP_LIMIT))
    fast_step_ok = true;

  यदि (all_digits_p (argv[optind])
      && (n_args == 1 || all_digits_p (argv[optind + 1]))
      && (n_args < 3 || (fast_step_ok
                         && all_digits_p (argv[optind + 2])))
      && !equal_width && !format_str && म_माप (separator) == 1)
    {
      वर्ण अचर *s1 = user_start;
      वर्ण अचर *s2 = argv[optind + (n_args - 1)];
      seq_fast (s1, s2, step.मान);
    }

  operand first = { 1, 1, 0 };
  operand last = scan_arg (argv[optind++]);

  यदि (optind < argc)
    {
      first = last;
      last = scan_arg (argv[optind++]);

      यदि (optind < argc)
        {
          step = last;
          यदि (step.मान == 0)
            {
              error (0, 0, _("invalid Zero increment value: %s"),
                     quote (argv[optind - 1]));
              usage (निकास_त्रुटि);
            }

          last = scan_arg (argv[optind++]);
        }
    }

  /* Try the fast method again, for integers of the form 1e1 etc.,
     or "inf" end value.  */
  यदि (first.precision == 0 && step.precision == 0 && last.precision == 0
      && है_सीमित (first.मान) && 0 <= first.मान && 0 <= last.मान
      && 0 < step.मान && step.मान <= SEQ_FAST_STEP_LIMIT
      && !equal_width && !format_str && म_माप (separator) == 1)
    {
      वर्ण *s1 = (all_digits_p (user_start)
                  ? xstrdup (user_start) : xasprintf ("%0.Lf", first.मान));
      /* Ensure "inf" is used.  */
      वर्ण *s2 = (! है_सीमित (last.मान)
                  ? xstrdup ("inf") : xasprintf ("%0.Lf", last.मान));

      यदि (*s1 != '-' && *s2 != '-')
        seq_fast (s1, s2, step.मान);

      मुक्त (s1);
      मुक्त (s2);
      /* Upon any failure, let the more general code deal with it.  */
    }

  यदि (format_str == रिक्त)
    format_str = get_default_format (first, step, last);

  print_numbers (format_str, layout, first.मान, step.मान, last.मान);

  main_exit (निकास_सफल);
}
