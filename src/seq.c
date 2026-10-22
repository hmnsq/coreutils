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
#यदि_अपरिभाषितम् isfinite
# define isfinite(x) ((x) * 0 == 0)
#यदि_अन्तः
#यदि_अपरिभाषितम् isnan
# define isnan(x) ((x) != (x))
#यदि_अन्तः

/* Limit below which seq_fast has more throughput.
   Determined with: seq 0 200 inf | pv > /dev/null  */
#परिभाषय SEQ_FAST_चरणः_सीमा 200  /* Keep in sync with texinfo description.  */
#परिभाषय SEQ_FAST_चरणः_सीमा_DIGITS 3

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "seq"

#परिभाषय AUTHORS proper_name ("Ulrich Drepper")

/* True if the locale settings were honored.  */
स्थिर bool locale_ok;

/* If true print all number with equal width.  */
स्थिर bool equal_विस्तारः;

/* The string used to separate two numbers.  */
स्थिर वर्ण ध्रुव *separator = "\n";

/* The string output after all numbers have been output.
   Usually "\n" or "\0".  */
स्थिर वर्ण ध्रुव terminator[] = "\n";

स्थिर संरचना option ध्रुव long_options[] =
{
  { "equal-width", no_तर्कः, रिक्त, 'w'},
  { "format", required_तर्कः, रिक्त, 'f'},
  { "separator", required_तर्कः, रिक्त, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  { रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... चरमः_3\n\
  or:  %s [OPTION]... FIRST चरमः_3\n\
  or:  %s [OPTION]... FIRST INCREMENT चरमः_3\n\
"), क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
Print numbers from FIRST गन्तव्यम् चरमः_3, अन्तर् चरणाः सम्बन्धि INCREMENT.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --format=FORMAT\n\
         use प्रारूप_लिख style floating-point FORMAT\n\
"));
      oputs (_("\
  -s, --separator=STRING\n\
         use STRING गन्तव्यम् separate numbers (स्वभाव: \\n)\n\
"));
      oputs (_("\
  -w, --equal-विस्तारः\n\
         equalize विस्तारः by padding with leading zeroes\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FIRST or INCREMENT is omitted, it defaults गन्तव्यम् 1.  That is, an\n\
omitted INCREMENT defaults गन्तव्यम् 1 even when चरमः_3 is smaller than FIRST.\n\
The sequence सम्बन्धि numbers अन्ताः when the योग सम्बन्धि the current सङ्ख्या and\n\
INCREMENT would become greater than चरमः_3.\n\
FIRST, INCREMENT, and चरमः_3 are interpreted as floating point मानानि.\n\
INCREMENT is usually धनात्मकम् यदि FIRST is smaller than चरमः_3, and\n\
INCREMENT is usually negative यदि FIRST is greater than चरमः_3.\n\
INCREMENT must not be 0; none सम्बन्धि FIRST, INCREMENT and चरमः_3 may be NaN.\n\
"), stdout);
      fputs (_("\
FORMAT must be suitable प्रति printing one तर्कः सम्बन्धि type 'double';\n\
it defaults गन्तव्यम् %.PRECf यदि FIRST, INCREMENT, and चरमः_3 are all fixed point\n\
decimal numbers with अधिकतमम् precision PREC, and गन्तव्यम् %g otherwise.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* A command-line operand.  */
संरचना सङ्कार्यम्_2
{
  /* Its value, converted to 'long double'.  */
  दीर्घ द्विगुण मानम्;

  /* Its print width, if it were printed out in a form similar to its
     input form.  An input like "-.1" is treated like "-0.1", and an
     input like "1." is treated like "1", but otherwise widths are
     left alone.  */
  size_t विस्तारः;

  /* Number of digits after the decimal point, or INT_MAX if the
     number can't easily be expressed as a fixed-point number.  */
  पूर्णाङ्क precision;
};
प्रकारनिर्देश संरचना सङ्कार्यम्_2 सङ्कार्यम्_2;

/* Description of what a number-generating format will generate.  */
संरचना layout
{
  /* Number of bytes before and after the number.  */
  size_t उपसर्गः_len;
  size_t suffix_len;
};

/* Read a long double value from the command line.
   Return if the string is correct else signal error.  */

स्थिर सङ्कार्यम्_2
अवलोकनम्_arg (वर्ण ध्रुव *arg)
{
  सङ्कार्यम्_2 ret;

  यदि (! xstrtold (arg, रिक्त, &ret.मानम्, cl_strtold))
    {
      error (0, 0, _("invalid floating point argument: %s"), quote (arg));
      usage (EXIT_FAILURE);
    }

  यदि (isnan (ret.मानम्))
    {
      error (0, 0, _("invalid %s argument: %s"), quote_n (0, "not-a-number"),
             quote_n (1, arg));
      usage (EXIT_FAILURE);
    }

  /* We don't output spaces or '+' so don't include in width */
  यावत् (रिक्तम्_वा (गन्तव्यम्_uchar (*arg)) || *arg == '+')
    arg++;

  /* Default to auto width and precision.  */
  ret.विस्तारः = 0;
  ret.precision = अधिकतम_पूर्णाङ्कः;

  /* Use no precision (and possibly fast generation) for integers.  */
  वर्ण ध्रुव *decimal_point = strchr (arg, '.');
  यदि (! decimal_point && ! strchr (arg, 'p') /* not a hex float */)
    ret.precision = 0;

  /* auto set width and precision for decimal inputs.  */
  यदि (! arg[strcspn (arg, "xX")] && isfinite (ret.मानम्))
    {
      size_t fraction_len = 0;
      ret.विस्तारः = वाक्यदैर्घ्यम् (arg);

      यदि (decimal_point)
        {
          fraction_len = strcspn (decimal_point + 1, "eE");
          यदि (fraction_len <= अधिकतम_पूर्णाङ्कः)
            ret.precision = fraction_len;
          ret.विस्तारः += (fraction_len == 0                       /* #.  -> #   */
                        ? -1
                        : (decimal_point == arg                 /* .#  -> 0.# */
                           || !c_isdigit (decimal_point[-1]))); /* -.# -> 0.# */
        }
      वर्ण ध्रुव *e = strchr (arg, 'e');
      यदि (! e)
        e = strchr (arg, 'E');
      यदि (e)
        {
          दीर्घ exponent = MAX (strtol (e + 1, रिक्त, 10), -LONG_MAX);
          ret.precision += exponent < 0 ? -exponent
                                        : - MIN (ret.precision, exponent);
          /* Don't account for e.... in the width since this is not output.  */
          ret.विस्तारः -= वाक्यदैर्घ्यम् (arg) - (e - arg);
          /* Adjust the width as per the exponent.  */
          यदि (exponent < 0)
            {
              यदि (decimal_point)
                {
                  यदि (e == decimal_point + 1) /* undo #. -> # above  */
                    ret.विस्तारः++;
                }
              अन्यथा
                ret.विस्तारः++;
              exponent = -exponent;
            }
          अन्यथा
            {
              यदि (decimal_point && ret.precision == 0 && fraction_len)
                ret.विस्तारः--; /* discount space for '.'  */
              exponent -= MIN (fraction_len, exponent);
            }
          ret.विस्तारः += exponent;
        }
    }

  प्रत्यागम ret;
}

/* If FORMAT is a valid printf format for a double argument, return
   its long double equivalent, allocated from dynamic storage, and
   store into *LAYOUT a description of the output layout; otherwise,
   report an error and exit.  */

स्थिर वर्ण ध्रुव *
long_double_format (वर्ण ध्रुव *fmt, संरचना layout *layout)
{
  size_t i;
  size_t उपसर्गः_len = 0;

  प्रति (i = 0; ! (fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      यदि (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      उपसर्गः_len++;
    }

  i++;
  i += strspn (fmt + i, "-+#0 '");
  i += strspn (fmt + i, "0123456789");
  यदि (fmt[i] == '.')
    {
      i++;
      i += strspn (fmt + i, "0123456789");
    }

  size_t दीर्घता_modifier_विस्थापनम् = i;
  bool has_L = (fmt[i] == 'L');
  i += has_L;
  यदि (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));
  यदि (! strchr ("efgaEFGA", fmt[i]))
    error (EXIT_FAILURE, 0,
           _("format %s has unknown %%%c directive"), quote (fmt), fmt[i]);

  size_t suffix_len = 0;
  प्रति (i++; ; i += (fmt[i] == '%') + 1)
    यदि (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));
    अन्यथा यदि (fmt[i])
      suffix_len++;
    अन्यथा
      {
        size_t format_आकार = i + 1;
        वर्ण *ldfmt = xmalloc (format_आकार + 1);
        स्मृतिं_प्रतिलिख (ldfmt, fmt, दीर्घता_modifier_विस्थापनम्);
        ldfmt[दीर्घता_modifier_विस्थापनम्] = 'L';
        वाक्यं_प्रतिलिख (ldfmt + दीर्घता_modifier_विस्थापनम् + 1,
                fmt + दीर्घता_modifier_विस्थापनम् + has_L);
        layout->उपसर्गः_len = उपसर्गः_len;
        layout->suffix_len = suffix_len;
        प्रत्यागम ldfmt;
      }
}

/* Actually print the sequence of numbers in the specified range, with the
   given or default stepping and format.  */

स्थिर शून्य
print_numbers (वर्ण ध्रुव *fmt, संरचना layout layout,
               दीर्घ द्विगुण first, दीर्घ द्विगुण चरणः, दीर्घ द्विगुण चरमः)
{
  bool बहिर्गमनम्_सम्बन्धि_range = (चरणः < 0 ? first < चरमः : चरमः < first);

  यदि (! बहिर्गमनम्_सम्बन्धि_range)
    {
      दीर्घ द्विगुण x = first;

      प्रति (दीर्घ द्विगुण i = 1; ; i++)
        {
          दीर्घ द्विगुण x0 = x;
          यदि (प्रारूप_लिख (fmt, x) < 0)
            write_error ();
          यदि (बहिर्गमनम्_सम्बन्धि_range)
            भङ्ग;

          /* Mathematically equivalent to 'x += step;', and typically
             less subject to rounding error.  */
          x = first + i * चरणः;

          बहिर्गमनम्_सम्बन्धि_range = (चरणः < 0 ? x < चरमः : चरमः < x);

          यदि (बहिर्गमनम्_सम्बन्धि_range)
            {
              /* If the number just past LAST prints as a value equal
                 to LAST, and prints differently from the previous
                 number, then print the number.  This avoids problems
                 with rounding.  For example, with the x86 it causes
                 "seq 0 0.000001 0.000003" to print 0.000003 instead
                 of stopping at 0.000002.  */

              bool print_extra_सङ्ख्या = false;
              यदि (locale_ok)
                setlocale (LC_NUMERIC, "C");
              वर्ण *x_str;
              पूर्णाङ्क x_strlen = asprintf (&x_str, fmt, x);
              यदि (locale_ok)
                setlocale (LC_NUMERIC, "");
              यदि (x_strlen < 0)
                xalloc_die ();
              x_str[x_strlen - layout.suffix_len] = '\0';

              दीर्घ द्विगुण x_val;
              यदि (xstrtold (x_str + layout.उपसर्गः_len, रिक्त,
                            &x_val, cl_strtold)
                  && x_val == चरमः)
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
                भङ्ग;
            }

          यदि (fputs (separator, stdout) == EOF)
            write_error ();
        }

      यदि (fputs (terminator, stdout) == EOF)
        write_error ();
    }
}

/* Return the default format given FIRST, STEP, and LAST.  */
स्थिर वर्ण ध्रुव *
get_default_format (सङ्कार्यम्_2 first, सङ्कार्यम्_2 चरणः, सङ्कार्यम्_2 चरमः)
{
  स्थिर वर्ण format_buf[परिमाण "%0.Lf" + 2 * INT_STRLEN_BOUND (पूर्णाङ्क)];

  पूर्णाङ्क prec = MAX (first.precision, चरणः.precision);

  यदि (prec != अधिकतम_पूर्णाङ्कः && चरमः.precision != अधिकतम_पूर्णाङ्कः)
    {
      यदि (equal_विस्तारः)
        {
          /* increase first_width by any increased precision in step */
          size_t first_विस्तारः = first.विस्तारः + (prec - first.precision);
          /* adjust last_width to use precision from first/step */
          size_t चरमः_विस्तारः = चरमः.विस्तारः + (prec - चरमः.precision);
          यदि (चरमः.precision && prec == 0)
            चरमः_विस्तारः--;  /* don't include space for '.' */
          यदि (चरमः.precision == 0 && prec)
            चरमः_विस्तारः++;  /* include space for '.' */
          यदि (first.precision == 0 && prec)
            first_विस्तारः++;  /* include space for '.' */
          size_t विस्तारः = MAX (first_विस्तारः, चरमः_विस्तारः);
          यदि (विस्तारः <= अधिकतम_पूर्णाङ्कः)
            {
              पूर्णाङ्क w = विस्तारः;
              sprintf (format_buf, "%%0%d.%dLf", w, prec);
              प्रत्यागम format_buf;
            }
        }
      अन्यथा
        {
          sprintf (format_buf, "%%.%dLf", prec);
          प्रत्यागम format_buf;
        }
    }

  प्रत्यागम "%Lg";
}

/* The nonempty char array P represents a valid non-negative decimal integer.
   ENDP points just after the last char in P.
   Adjust the array to describe the next-larger integer and return
   whether this grows the array by one on the left.  */
स्थिर bool
incr_grows (वर्ण *p, वर्ण *endp)
{
  कुरु
    {
      endp--;
      यदि (*endp < '9')
        {
          (*endp)++;
          प्रत्यागम false;
        }
      *endp = '0';
    }
  यावत् (p < endp);

  p[-1] = '1';
  प्रत्यागम true;
}

/* Compare A and B with lengths A_LEN and B_LEN.  A and B are integers
   represented by nonempty arrays of digits without redundant leading '0'.
   Return negative if A < B, 0 if A = B, positive if A > B.  */
स्थिर पूर्णाङ्क
cmp (वर्ण ध्रुव *a, idx_t a_len, वर्ण ध्रुव *b, idx_t b_len)
{
  प्रत्यागम a_len == b_len ? memcmp (a, b, a_len) : _GL_CMP (a_len, b_len);
}

/* Trim leading 0's from S, but if S is all 0's, leave one.
   Return a pointer to the trimmed string.  */
ATTRIBUTE_विशुद्धम्
स्थिर वर्ण ध्रुव *
trim_leading_zeros (वर्ण ध्रुव *s)
{
  वर्ण ध्रुव *p = s;
  यावत् (*s == '0')
    ++s;

  /* If there were only 0's, back up, to leave one.  */
  यदि (!*s && s != p)
    --s;
  प्रत्यागम s;
}

/* Print all whole numbers from A to B, inclusive -- to stdout, each
   followed by a newline.  Then exit.  */
स्थिर शून्य
seq_fast (वर्ण ध्रुव *a, वर्ण ध्रुव *b, uintmax_t चरणः)
{
  /* Skip past any redundant leading '0's.  Without this, our naive cmp
     function would declare 000 to be larger than 99.  */
  a = trim_leading_zeros (a);
  b = trim_leading_zeros (b);

  idx_t p_len = वाक्यदैर्घ्यम् (a);
  idx_t b_len = वाक्यदैर्घ्यम् (b);
  bool inf = b_len == 3 && memeq (b, "inf", 4);

  /* Allow for at least 31 digits without realloc.
     1 more than p_len is needed for the inf case.  */
  गणना { INITIAL_ALLOC_DIGITS = 31 };
  idx_t inc_आकार = MAX (MAX (p_len + 1, b_len), INITIAL_ALLOC_DIGITS);
  /* Ensure we only increase by at most 1 digit at buffer boundaries.  */
  static_assert (SEQ_FAST_चरणः_सीमा_DIGITS < INITIAL_ALLOC_DIGITS - 1);

  /* Copy A (sans NUL) to end of new buffer.  */
  वर्ण *p0 = xmalloc (inc_आकार);
  वर्ण *endp = p0 + inc_आकार;
  वर्ण *p = स्मृतिं_प्रतिलिख (endp - p_len, a, p_len);

  /* Reduce number of write calls which is seen to
     give a speed-up of more than 2x over naive stdio code
     when printing the first 10^9 integers.  */
  वर्ण buf[BUFSIZ];
  वर्ण *buf_अन्तः = buf + परिमाण buf;
  वर्ण *bufp = buf;

  यावत् (inf || cmp (p, endp - p, b, b_len) <= 0)
    {
      /* Append number, flushing output buffer while the number's
         digits do not fit with room for a separator or terminator.  */
      वर्ण *pp = p;
      यावत् (buf_अन्तः - bufp <= endp - pp)
        {
          स्मृतिं_प्रतिलिख (bufp, pp, buf_अन्तः - bufp);
          pp += buf_अन्तः - bufp;
          यदि (पूर्णम्_write (STDOUT_FILENO, buf, परिमाण buf) != परिमाण buf)
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
          वर्ण *new_p0 = xpalloc (रिक्त, &inc_आकार, 1, -1, 1);
          idx_t saved_p_len = endp - p;
          endp = new_p0 + inc_आकार;
          p = स्मृतिं_प्रतिलिख (endp - saved_p_len, p0, saved_p_len);
          मुक्त (p0);
          p0 = new_p0;
        }

      /* Compute next number.  */
      प्रति (uintmax_t n_incr = चरणः; n_incr; n_incr--)
        p -= incr_grows (p, endp);
    }

  /* Write the remaining buffered output with a terminator instead of
     a separator.  */
  idx_t remaining = bufp - buf;
  यदि (remaining)
    {
      bufp[-1] = *terminator;
      यदि (पूर्णम्_write (STDOUT_FILENO, buf, remaining) != remaining)
        write_error ();
    }

  निर्गच्छ (EXIT_SUCCESS);
}

/* Return true if S consists of at least one digit and no non-digits.  */
ATTRIBUTE_विशुद्धम्
स्थिर bool
all_digits_p (वर्ण ध्रुव *s)
{
  size_t n = वाक्यदैर्घ्यम् (s);
  प्रत्यागम c_isdigit (s[0]) && n == strspn (s, "0123456789");
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  /* The printf(3) format used for output.  */
  वर्ण ध्रुव *format_str = रिक्त;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We have to handle negative numbers in the command line but this
     conflicts with the command line arguments.  So explicitly check first
     whether the next argument looks like a negative number.  */
  यावत् (optind < argc)
    {
      पूर्णाङ्क optc;
      यदि (argv[optind][0] == '-'
          && ((optc = argv[optind][1]) == '.' || c_isdigit (optc)))
        {
          /* means negative number */
          भङ्ग;
        }

      optc = getopt_long (argc, argv, "+f:s:w", long_options, रिक्त);
      यदि (optc == -1)
        भङ्ग;

      विकल्प (optc)
        {
        पक्ष 'f':
          format_str = optarg;
          भङ्ग;

        पक्ष 's':
          separator = optarg;
          भङ्ग;

        पक्ष 'w':
          equal_विस्तारः = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  पूर्णाङ्क n_तर्काः = argc - optind;
  यदि (n_तर्काः < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  यदि (3 < n_तर्काः)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 3]));
      usage (EXIT_FAILURE);
    }

  संरचना layout layout = { 0, 0 };
  यदि (format_str)
    format_str = long_double_format (format_str, &layout);

  यदि (format_str != रिक्त && equal_विस्तारः)
    {
      error (0, 0, _("format string may not be specified"
                     " when printing equal width strings"));
      usage (EXIT_FAILURE);
    }

  वर्ण ध्रुव *user_start = n_तर्काः == 1 ? "1" : argv[optind];

  /* If the following hold:
     - no format string, [FIXME: relax this, eventually]
     - integer start (or no start)
     - integer end
     - integer increment <= SEQ_FAST_STEP_LIMIT
     then use the much more efficient integer-only code,
     operating on arbitrarily large numbers.  */
  सङ्कार्यम्_2 चरणः = { 1, 1, 0 };
  bool fast_चरणः_ok = false;
  यदि (n_तर्काः != 3
      || (all_digits_p (argv[optind + 1])
          && xstrtold (argv[optind + 1], रिक्त, &चरणः.मानम्, cl_strtold)
          && 0 < चरणः.मानम् && चरणः.मानम् <= SEQ_FAST_चरणः_सीमा))
    fast_चरणः_ok = true;

  यदि (all_digits_p (argv[optind])
      && (n_तर्काः == 1 || all_digits_p (argv[optind + 1]))
      && (n_तर्काः < 3 || (fast_चरणः_ok
                         && all_digits_p (argv[optind + 2])))
      && !equal_विस्तारः && !format_str && वाक्यदैर्घ्यम् (separator) == 1)
    {
      वर्ण ध्रुव *s1 = user_start;
      वर्ण ध्रुव *s2 = argv[optind + (n_तर्काः - 1)];
      seq_fast (s1, s2, चरणः.मानम्);
    }

  सङ्कार्यम्_2 first = { 1, 1, 0 };
  सङ्कार्यम्_2 चरमः = अवलोकनम्_arg (argv[optind++]);

  यदि (optind < argc)
    {
      first = चरमः;
      चरमः = अवलोकनम्_arg (argv[optind++]);

      यदि (optind < argc)
        {
          चरणः = चरमः;
          यदि (चरणः.मानम् == 0)
            {
              error (0, 0, _("invalid Zero increment value: %s"),
                     quote (argv[optind - 1]));
              usage (EXIT_FAILURE);
            }

          चरमः = अवलोकनम्_arg (argv[optind++]);
        }
    }

  /* Try the fast method again, for integers of the form 1e1 etc.,
     or "inf" end value.  */
  यदि (first.precision == 0 && चरणः.precision == 0 && चरमः.precision == 0
      && isfinite (first.मानम्) && 0 <= first.मानम् && 0 <= चरमः.मानम्
      && 0 < चरणः.मानम् && चरणः.मानम् <= SEQ_FAST_चरणः_सीमा
      && !equal_विस्तारः && !format_str && वाक्यदैर्घ्यम् (separator) == 1)
    {
      वर्ण *s1 = (all_digits_p (user_start)
                  ? xstrdup (user_start) : xasprintf ("%0.Lf", first.मानम्));
      /* Ensure "inf" is used.  */
      वर्ण *s2 = (! isfinite (चरमः.मानम्)
                  ? xstrdup ("inf") : xasprintf ("%0.Lf", चरमः.मानम्));

      यदि (*s1 != '-' && *s2 != '-')
        seq_fast (s1, s2, चरणः.मानम्);

      मुक्त (s1);
      मुक्त (s2);
      /* Upon any failure, let the more general code deal with it.  */
    }

  यदि (format_str == रिक्त)
    format_str = get_default_format (first, चरणः, चरमः);

  print_numbers (format_str, layout, first.मानम्, चरणः.मानम्, चरमः.मानम्);

  main_exit (EXIT_SUCCESS);
}
