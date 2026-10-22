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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "cl-strtod.h"
#അന്തര്ഭാവ "full-write.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "xstrtod.h"
#അന്തര്ഭാവ "xvasprintf.h"

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
സ്ഥിതം bool locale_ok;

/* If true print all number with equal width.  */
സ്ഥിതം bool equal_width;

/* The string used to separate two numbers.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *separator = "\n";

/* The string output after all numbers have been output.
   Usually "\n" or "\0".  */
സ്ഥിതം അക്ഷരം സ്ഥിരം terminator[] = "\n";

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  { "equal-width", no_argument, ശൂന്യ, 'w'},
  { "format", required_argument, ശൂന്യ, 'f'},
  { "separator", required_argument, ശൂന്യ, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  { ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
         use STRING to separate numbers (സ്ഥിരസ്ഥിതി: \\n)\n\
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
The sequence of numbers ends when the sum of the current സംഖ്യ and\n\
INCREMENT would become greater than LAST.\n\
FIRST, INCREMENT, and LAST are interpreted as floating point മൂല്യങ്ങൾ.\n\
INCREMENT is usually positive എങ്കിൽ FIRST is smaller than LAST, and\n\
INCREMENT is usually negative എങ്കിൽ FIRST is greater than LAST.\n\
INCREMENT must not be 0; none of FIRST, INCREMENT and LAST may be NaN.\n\
"), stdout);
      fputs (_("\
FORMAT must be suitable വേണ്ടി printing one argument of type 'double';\n\
it defaults to %.PRECf എങ്കിൽ FIRST, INCREMENT, and LAST are all fixed point\n\
decimal numbers with maximum precision PREC, and to %g otherwise.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A command-line operand.  */
ഘടന operand
{
  /* Its value, converted to 'long double'.  */
  long ഇരട്ട മൂല്യം;

  /* Its print width, if it were printed out in a form similar to its
     input form.  An input like "-.1" is treated like "-0.1", and an
     input like "1." is treated like "1", but otherwise widths are
     left alone.  */
  size_t width;

  /* Number of digits after the decimal point, or INT_MAX if the
     number can't easily be expressed as a fixed-point number.  */
  പൂർണ്ണസംഖ്യ precision;
};
തരനിർവചനം ഘടന operand operand;

/* Description of what a number-generating format will generate.  */
ഘടന layout
{
  /* Number of bytes before and after the number.  */
  size_t prefix_len;
  size_t suffix_len;
};

/* Read a long double value from the command line.
   Return if the string is correct else signal error.  */

സ്ഥിതം operand
scan_arg (അക്ഷരം സ്ഥിരം *arg)
{
  operand ret;

  എങ്കിൽ (! xstrtold (arg, ശൂന്യ, &ret.മൂല്യം, cl_strtold))
    {
      error (0, 0, _("invalid floating point argument: %s"), quote (arg));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (isnan (ret.മൂല്യം))
    {
      error (0, 0, _("invalid %s argument: %s"), quote_n (0, "not-a-number"),
             quote_n (1, arg));
      usage (EXIT_FAILURE);
    }

  /* We don't output spaces or '+' so don't include in width */
  ആകുമ്പോൾ (isspace (to_uchar (*arg)) || *arg == '+')
    arg++;

  /* Default to auto width and precision.  */
  ret.width = 0;
  ret.precision = INT_MAX;

  /* Use no precision (and possibly fast generation) for integers.  */
  അക്ഷരം സ്ഥിരം *decimal_point = strchr (arg, '.');
  എങ്കിൽ (! decimal_point && ! strchr (arg, 'p') /* not a hex float */)
    ret.precision = 0;

  /* auto set width and precision for decimal inputs.  */
  എങ്കിൽ (! arg[strcspn (arg, "xX")] && isfinite (ret.മൂല്യം))
    {
      size_t fraction_len = 0;
      ret.width = strlen (arg);

      എങ്കിൽ (decimal_point)
        {
          fraction_len = strcspn (decimal_point + 1, "eE");
          എങ്കിൽ (fraction_len <= INT_MAX)
            ret.precision = fraction_len;
          ret.width += (fraction_len == 0                       /* #.  -> #   */
                        ? -1
                        : (decimal_point == arg                 /* .#  -> 0.# */
                           || !c_isdigit (decimal_point[-1]))); /* -.# -> 0.# */
        }
      അക്ഷരം സ്ഥിരം *e = strchr (arg, 'e');
      എങ്കിൽ (! e)
        e = strchr (arg, 'E');
      എങ്കിൽ (e)
        {
          long exponent = MAX (strtol (e + 1, ശൂന്യ, 10), -LONG_MAX);
          ret.precision += exponent < 0 ? -exponent
                                        : - MIN (ret.precision, exponent);
          /* Don't account for e.... in the width since this is not output.  */
          ret.width -= strlen (arg) - (e - arg);
          /* Adjust the width as per the exponent.  */
          എങ്കിൽ (exponent < 0)
            {
              എങ്കിൽ (decimal_point)
                {
                  എങ്കിൽ (e == decimal_point + 1) /* undo #. -> # above  */
                    ret.width++;
                }
              അല്ലെങ്കിൽ
                ret.width++;
              exponent = -exponent;
            }
          അല്ലെങ്കിൽ
            {
              എങ്കിൽ (decimal_point && ret.precision == 0 && fraction_len)
                ret.width--; /* discount space for '.'  */
              exponent -= MIN (fraction_len, exponent);
            }
          ret.width += exponent;
        }
    }

  തിരികെ ret;
}

/* If FORMAT is a valid printf format for a double argument, return
   its long double equivalent, allocated from dynamic storage, and
   store into *LAYOUT a description of the output layout; otherwise,
   report an error and exit.  */

സ്ഥിതം അക്ഷരം സ്ഥിരം *
long_double_format (അക്ഷരം സ്ഥിരം *fmt, ഘടന layout *layout)
{
  size_t i;
  size_t prefix_len = 0;

  വേണ്ടി (i = 0; ! (fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      എങ്കിൽ (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      prefix_len++;
    }

  i++;
  i += strspn (fmt + i, "-+#0 '");
  i += strspn (fmt + i, "0123456789");
  എങ്കിൽ (fmt[i] == '.')
    {
      i++;
      i += strspn (fmt + i, "0123456789");
    }

  size_t length_modifier_offset = i;
  bool has_L = (fmt[i] == 'L');
  i += has_L;
  എങ്കിൽ (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));
  എങ്കിൽ (! strchr ("efgaEFGA", fmt[i]))
    error (EXIT_FAILURE, 0,
           _("format %s has unknown %%%c directive"), quote (fmt), fmt[i]);

  size_t suffix_len = 0;
  വേണ്ടി (i++; ; i += (fmt[i] == '%') + 1)
    എങ്കിൽ (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));
    അല്ലെങ്കിൽ എങ്കിൽ (fmt[i])
      suffix_len++;
    അല്ലെങ്കിൽ
      {
        size_t format_size = i + 1;
        അക്ഷരം *ldfmt = xmalloc (format_size + 1);
        memcpy (ldfmt, fmt, length_modifier_offset);
        ldfmt[length_modifier_offset] = 'L';
        strcpy (ldfmt + length_modifier_offset + 1,
                fmt + length_modifier_offset + has_L);
        layout->prefix_len = prefix_len;
        layout->suffix_len = suffix_len;
        തിരികെ ldfmt;
      }
}

/* Actually print the sequence of numbers in the specified range, with the
   given or default stepping and format.  */

സ്ഥിതം ശൂന്യം
print_numbers (അക്ഷരം സ്ഥിരം *fmt, ഘടന layout layout,
               long ഇരട്ട first, long ഇരട്ട step, long ഇരട്ട last)
{
  bool out_of_range = (step < 0 ? first < last : last < first);

  എങ്കിൽ (! out_of_range)
    {
      long ഇരട്ട x = first;

      വേണ്ടി (long ഇരട്ട i = 1; ; i++)
        {
          long ഇരട്ട x0 = x;
          എങ്കിൽ (printf (fmt, x) < 0)
            write_error ();
          എങ്കിൽ (out_of_range)
            നിർത്തുക;

          /* Mathematically equivalent to 'x += step;', and typically
             less subject to rounding error.  */
          x = first + i * step;

          out_of_range = (step < 0 ? x < last : last < x);

          എങ്കിൽ (out_of_range)
            {
              /* If the number just past LAST prints as a value equal
                 to LAST, and prints differently from the previous
                 number, then print the number.  This avoids problems
                 with rounding.  For example, with the x86 it causes
                 "seq 0 0.000001 0.000003" to print 0.000003 instead
                 of stopping at 0.000002.  */

              bool print_extra_സംഖ്യ = false;
              എങ്കിൽ (locale_ok)
                setlocale (LC_NUMERIC, "C");
              അക്ഷരം *x_str;
              പൂർണ്ണസംഖ്യ x_strlen = asprintf (&x_str, fmt, x);
              എങ്കിൽ (locale_ok)
                setlocale (LC_NUMERIC, "");
              എങ്കിൽ (x_strlen < 0)
                xalloc_die ();
              x_str[x_strlen - layout.suffix_len] = '\0';

              long ഇരട്ട x_val;
              എങ്കിൽ (xstrtold (x_str + layout.prefix_len, ശൂന്യ,
                            &x_val, cl_strtold)
                  && x_val == last)
                {
                  അക്ഷരം *x0_str = ശൂന്യ;
                  പൂർണ്ണസംഖ്യ x0_strlen = asprintf (&x0_str, fmt, x0);
                  എങ്കിൽ (x0_strlen < 0)
                    xalloc_die ();
                  x0_str[x0_strlen - layout.suffix_len] = '\0';
                  print_extra_സംഖ്യ = !streq (x0_str, x_str);
                  സ്വതന്ത്രം (x0_str);
                }

              സ്വതന്ത്രം (x_str);
              എങ്കിൽ (! print_extra_സംഖ്യ)
                നിർത്തുക;
            }

          എങ്കിൽ (fputs (separator, stdout) == EOF)
            write_error ();
        }

      എങ്കിൽ (fputs (terminator, stdout) == EOF)
        write_error ();
    }
}

/* Return the default format given FIRST, STEP, and LAST.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *
get_default_format (operand first, operand step, operand last)
{
  സ്ഥിതം അക്ഷരം format_buf[വലുപ്പം "%0.Lf" + 2 * INT_STRLEN_BOUND (പൂർണ്ണസംഖ്യ)];

  പൂർണ്ണസംഖ്യ prec = MAX (first.precision, step.precision);

  എങ്കിൽ (prec != INT_MAX && last.precision != INT_MAX)
    {
      എങ്കിൽ (equal_width)
        {
          /* increase first_width by any increased precision in step */
          size_t first_width = first.width + (prec - first.precision);
          /* adjust last_width to use precision from first/step */
          size_t last_width = last.width + (prec - last.precision);
          എങ്കിൽ (last.precision && prec == 0)
            last_width--;  /* don't include space for '.' */
          എങ്കിൽ (last.precision == 0 && prec)
            last_width++;  /* include space for '.' */
          എങ്കിൽ (first.precision == 0 && prec)
            first_width++;  /* include space for '.' */
          size_t width = MAX (first_width, last_width);
          എങ്കിൽ (width <= INT_MAX)
            {
              പൂർണ്ണസംഖ്യ w = width;
              sprintf (format_buf, "%%0%d.%dLf", w, prec);
              തിരികെ format_buf;
            }
        }
      അല്ലെങ്കിൽ
        {
          sprintf (format_buf, "%%.%dLf", prec);
          തിരികെ format_buf;
        }
    }

  തിരികെ "%Lg";
}

/* The nonempty char array P represents a valid non-negative decimal integer.
   ENDP points just after the last char in P.
   Adjust the array to describe the next-larger integer and return
   whether this grows the array by one on the left.  */
സ്ഥിതം bool
incr_grows (അക്ഷരം *p, അക്ഷരം *endp)
{
  ചെയ്യുക
    {
      endp--;
      എങ്കിൽ (*endp < '9')
        {
          (*endp)++;
          തിരികെ false;
        }
      *endp = '0';
    }
  ആകുമ്പോൾ (p < endp);

  p[-1] = '1';
  തിരികെ true;
}

/* Compare A and B with lengths A_LEN and B_LEN.  A and B are integers
   represented by nonempty arrays of digits without redundant leading '0'.
   Return negative if A < B, 0 if A = B, positive if A > B.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp (അക്ഷരം സ്ഥിരം *a, idx_t a_len, അക്ഷരം സ്ഥിരം *b, idx_t b_len)
{
  തിരികെ a_len == b_len ? memcmp (a, b, a_len) : _GL_CMP (a_len, b_len);
}

/* Trim leading 0's from S, but if S is all 0's, leave one.
   Return a pointer to the trimmed string.  */
ATTRIBUTE_PURE
സ്ഥിതം അക്ഷരം സ്ഥിരം *
trim_leading_zeros (അക്ഷരം സ്ഥിരം *s)
{
  അക്ഷരം സ്ഥിരം *p = s;
  ആകുമ്പോൾ (*s == '0')
    ++s;

  /* If there were only 0's, back up, to leave one.  */
  എങ്കിൽ (!*s && s != p)
    --s;
  തിരികെ s;
}

/* Print all whole numbers from A to B, inclusive -- to stdout, each
   followed by a newline.  Then exit.  */
സ്ഥിതം ശൂന്യം
seq_fast (അക്ഷരം സ്ഥിരം *a, അക്ഷരം സ്ഥിരം *b, uintmax_t step)
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
  എണ്ണൽ { INITIAL_ALLOC_DIGITS = 31 };
  idx_t inc_size = MAX (MAX (p_len + 1, b_len), INITIAL_ALLOC_DIGITS);
  /* Ensure we only increase by at most 1 digit at buffer boundaries.  */
  static_assert (SEQ_FAST_STEP_LIMIT_DIGITS < INITIAL_ALLOC_DIGITS - 1);

  /* Copy A (sans NUL) to end of new buffer.  */
  അക്ഷരം *p0 = xmalloc (inc_size);
  അക്ഷരം *endp = p0 + inc_size;
  അക്ഷരം *p = memcpy (endp - p_len, a, p_len);

  /* Reduce number of write calls which is seen to
     give a speed-up of more than 2x over naive stdio code
     when printing the first 10^9 integers.  */
  അക്ഷരം buf[BUFSIZ];
  അക്ഷരം *buf_end = buf + വലുപ്പം buf;
  അക്ഷരം *bufp = buf;

  ആകുമ്പോൾ (inf || cmp (p, endp - p, b, b_len) <= 0)
    {
      /* Append number, flushing output buffer while the number's
         digits do not fit with room for a separator or terminator.  */
      അക്ഷരം *pp = p;
      ആകുമ്പോൾ (buf_end - bufp <= endp - pp)
        {
          memcpy (bufp, pp, buf_end - bufp);
          pp += buf_end - bufp;
          എങ്കിൽ (full_write (STDOUT_FILENO, buf, വലുപ്പം buf) != വലുപ്പം buf)
            write_error ();
          bufp = buf;
        }

      /* The rest of the number, followed by a separator or terminator,
         will fit.  Tentatively append a separator.  */
      bufp = mempcpy (bufp, pp, endp - pp);
      *bufp++ = *separator;

      /* Grow number buffer if needed for the inf case.  */
      എങ്കിൽ (p == p0)
        {
          അക്ഷരം *new_p0 = xpalloc (ശൂന്യ, &inc_size, 1, -1, 1);
          idx_t saved_p_len = endp - p;
          endp = new_p0 + inc_size;
          p = memcpy (endp - saved_p_len, p0, saved_p_len);
          സ്വതന്ത്രം (p0);
          p0 = new_p0;
        }

      /* Compute next number.  */
      വേണ്ടി (uintmax_t n_incr = step; n_incr; n_incr--)
        p -= incr_grows (p, endp);
    }

  /* Write the remaining buffered output with a terminator instead of
     a separator.  */
  idx_t remaining = bufp - buf;
  എങ്കിൽ (remaining)
    {
      bufp[-1] = *terminator;
      എങ്കിൽ (full_write (STDOUT_FILENO, buf, remaining) != remaining)
        write_error ();
    }

  exit (EXIT_SUCCESS);
}

/* Return true if S consists of at least one digit and no non-digits.  */
ATTRIBUTE_PURE
സ്ഥിതം bool
all_digits_p (അക്ഷരം സ്ഥിരം *s)
{
  size_t n = strlen (s);
  തിരികെ c_isdigit (s[0]) && n == strspn (s, "0123456789");
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  /* The printf(3) format used for output.  */
  അക്ഷരം സ്ഥിരം *format_str = ശൂന്യ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We have to handle negative numbers in the command line but this
     conflicts with the command line arguments.  So explicitly check first
     whether the next argument looks like a negative number.  */
  ആകുമ്പോൾ (optind < argc)
    {
      പൂർണ്ണസംഖ്യ optc;
      എങ്കിൽ (argv[optind][0] == '-'
          && ((optc = argv[optind][1]) == '.' || c_isdigit (optc)))
        {
          /* means negative number */
          നിർത്തുക;
        }

      optc = getopt_long (argc, argv, "+f:s:w", long_options, ശൂന്യ);
      എങ്കിൽ (optc == -1)
        നിർത്തുക;

      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'f':
          format_str = optarg;
          നിർത്തുക;

        അവസ്ഥ 's':
          separator = optarg;
          നിർത്തുക;

        അവസ്ഥ 'w':
          equal_width = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  പൂർണ്ണസംഖ്യ n_args = argc - optind;
  എങ്കിൽ (n_args < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (3 < n_args)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 3]));
      usage (EXIT_FAILURE);
    }

  ഘടന layout layout = { 0, 0 };
  എങ്കിൽ (format_str)
    format_str = long_double_format (format_str, &layout);

  എങ്കിൽ (format_str != ശൂന്യ && equal_width)
    {
      error (0, 0, _("format string may not be specified"
                     " when printing equal width strings"));
      usage (EXIT_FAILURE);
    }

  അക്ഷരം സ്ഥിരം *user_start = n_args == 1 ? "1" : argv[optind];

  /* If the following hold:
     - no format string, [FIXME: relax this, eventually]
     - integer start (or no start)
     - integer end
     - integer increment <= SEQ_FAST_STEP_LIMIT
     then use the much more efficient integer-only code,
     operating on arbitrarily large numbers.  */
  operand step = { 1, 1, 0 };
  bool fast_step_ok = false;
  എങ്കിൽ (n_args != 3
      || (all_digits_p (argv[optind + 1])
          && xstrtold (argv[optind + 1], ശൂന്യ, &step.മൂല്യം, cl_strtold)
          && 0 < step.മൂല്യം && step.മൂല്യം <= SEQ_FAST_STEP_LIMIT))
    fast_step_ok = true;

  എങ്കിൽ (all_digits_p (argv[optind])
      && (n_args == 1 || all_digits_p (argv[optind + 1]))
      && (n_args < 3 || (fast_step_ok
                         && all_digits_p (argv[optind + 2])))
      && !equal_width && !format_str && strlen (separator) == 1)
    {
      അക്ഷരം സ്ഥിരം *s1 = user_start;
      അക്ഷരം സ്ഥിരം *s2 = argv[optind + (n_args - 1)];
      seq_fast (s1, s2, step.മൂല്യം);
    }

  operand first = { 1, 1, 0 };
  operand last = scan_arg (argv[optind++]);

  എങ്കിൽ (optind < argc)
    {
      first = last;
      last = scan_arg (argv[optind++]);

      എങ്കിൽ (optind < argc)
        {
          step = last;
          എങ്കിൽ (step.മൂല്യം == 0)
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
  എങ്കിൽ (first.precision == 0 && step.precision == 0 && last.precision == 0
      && isfinite (first.മൂല്യം) && 0 <= first.മൂല്യം && 0 <= last.മൂല്യം
      && 0 < step.മൂല്യം && step.മൂല്യം <= SEQ_FAST_STEP_LIMIT
      && !equal_width && !format_str && strlen (separator) == 1)
    {
      അക്ഷരം *s1 = (all_digits_p (user_start)
                  ? xstrdup (user_start) : xasprintf ("%0.Lf", first.മൂല്യം));
      /* Ensure "inf" is used.  */
      അക്ഷരം *s2 = (! isfinite (last.മൂല്യം)
                  ? xstrdup ("inf") : xasprintf ("%0.Lf", last.മൂല്യം));

      എങ്കിൽ (*s1 != '-' && *s2 != '-')
        seq_fast (s1, s2, step.മൂല്യം);

      സ്വതന്ത്രം (s1);
      സ്വതന്ത്രം (s2);
      /* Upon any failure, let the more general code deal with it.  */
    }

  എങ്കിൽ (format_str == ശൂന്യ)
    format_str = get_default_format (first, step, last);

  print_numbers (format_str, layout, first.മൂല്യം, step.മൂല്യം, last.മൂല്യം);

  main_exit (EXIT_SUCCESS);
}
