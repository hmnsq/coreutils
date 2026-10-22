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

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "cl-strtod.h"
#అన్తర్భావ "full-write.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xstrtod.h"
#అన్తర్భావ "xvasprintf.h"

/* Roll our own isfinite/isnan rather than using <math.h>, so that we don't
   have to worry about linking -lm just for isfinite.  */
#నిర్వచితం_కాకపోతే isfinite
# define isfinite(x) ((x) * 0 == 0)
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే isnan
# define isnan(x) ((x) != (x))
#అయితే_ముగింపు

/* Limit below which seq_fast has more throughput.
   Determined with: seq 0 200 inf | pv > /dev/null  */
#నిర్వచించు SEQ_FAST_అడుగు_హద్దు 200  /* Keep in sync with texinfo description.  */
#నిర్వచించు SEQ_FAST_అడుగు_హద్దు_DIGITS 3

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "seq"

#నిర్వచించు AUTHORS proper_name ("Ulrich Drepper")

/* True if the locale settings were honored.  */
స్థిర bool locale_ok;

/* If true print all number with equal width.  */
స్థిర bool equal_వెడల్పు;

/* The string used to separate two numbers.  */
స్థిర అక్షరం స్థిరరాశి *separator = "\n";

/* The string output after all numbers have been output.
   Usually "\n" or "\0".  */
స్థిర అక్షరం స్థిరరాశి terminator[] = "\n";

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  { "equal-width", no_వాదం, ఖాళీ, 'w'},
  { "format", required_వాదం, ఖాళీ, 'f'},
  { "separator", required_వాదం, ఖాళీ, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  { ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... చివరి_3\n\
  or:  %s [OPTION]... మొదటి_3 చివరి_3\n\
  or:  %s [OPTION]... మొదటి_3 INCREMENT చివరి_3\n\
"), కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Print numbers from మొదటి_3 గమ్యం చివరి_3, లోపల అడుగులు యొక్క INCREMENT.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --format=FORMAT\n\
         use రాయి style floating-బిందువు FORMAT\n\
"));
      oputs (_("\
  -s, --separator=STRING\n\
         use STRING గమ్యం separate numbers (అప్రమేయ: \\n)\n\
"));
      oputs (_("\
  -w, --equal-వెడల్పు\n\
         equalize వెడల్పు by padding with leading zeroes\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If మొదటి_3 or INCREMENT is omitted, it defaults గమ్యం 1.  That is, an\n\
omitted INCREMENT defaults గమ్యం 1 even when చివరి_3 is smaller than మొదటి_3.\n\
The sequence యొక్క numbers చివర్లు when the కూడిక యొక్క the current సంఖ్య and\n\
INCREMENT would become greater than చివరి_3.\n\
మొదటి_3, INCREMENT, and చివరి_3 are interpreted as floating బిందువు విలువలు.\n\
INCREMENT is usually ధనాత్మక ఒకవేళ మొదటి_3 is smaller than చివరి_3, and\n\
INCREMENT is usually negative ఒకవేళ మొదటి_3 is greater than చివరి_3.\n\
INCREMENT must not be 0; none యొక్క మొదటి_3, INCREMENT and చివరి_3 may be NaN.\n\
"), stdout);
      fputs (_("\
FORMAT must be suitable కొరకు printing one వాదం యొక్క type 'double';\n\
it defaults గమ్యం %.PRECf ఒకవేళ మొదటి_3, INCREMENT, and చివరి_3 are all fixed బిందువు\n\
decimal numbers with గరిష్ఠ precision PREC, and గమ్యం %g otherwise.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* A command-line operand.  */
నిర్మాణం నిర్వాహ్యం_2
{
  /* Its value, converted to 'long double'.  */
  దీర్ఘ ద్విగుణ విలువ;

  /* Its print width, if it were printed out in a form similar to its
     input form.  An input like "-.1" is treated like "-0.1", and an
     input like "1." is treated like "1", but otherwise widths are
     left alone.  */
  size_t వెడల్పు;

  /* Number of digits after the decimal point, or INT_MAX if the
     number can't easily be expressed as a fixed-point number.  */
  పూర్ణాంకం precision;
};
రకనిర్వచనం నిర్మాణం నిర్వాహ్యం_2 నిర్వాహ్యం_2;

/* Description of what a number-generating format will generate.  */
నిర్మాణం layout
{
  /* Number of bytes before and after the number.  */
  size_t ఉపసర్గ_len;
  size_t suffix_len;
};

/* Read a long double value from the command line.
   Return if the string is correct else signal error.  */

స్థిర నిర్వాహ్యం_2
పరిశీలన_arg (అక్షరం స్థిరరాశి *arg)
{
  నిర్వాహ్యం_2 ret;

  ఒకవేళ (! xstrtold (arg, ఖాళీ, &ret.విలువ, cl_strtold))
    {
      error (0, 0, _("invalid floating point argument: %s"), quote (arg));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (isnan (ret.విలువ))
    {
      error (0, 0, _("invalid %s argument: %s"), quote_n (0, "not-a-number"),
             quote_n (1, arg));
      usage (EXIT_FAILURE);
    }

  /* We don't output spaces or '+' so don't include in width */
  అయితే (ఖాళీనా (గమ్యం_uchar (*arg)) || *arg == '+')
    arg++;

  /* Default to auto width and precision.  */
  ret.వెడల్పు = 0;
  ret.precision = గరిష్ఠ_పూర్ణాంకం;

  /* Use no precision (and possibly fast generation) for integers.  */
  అక్షరం స్థిరరాశి *decimal_బిందువు = strchr (arg, '.');
  ఒకవేళ (! decimal_బిందువు && ! strchr (arg, 'p') /* not a hex float */)
    ret.precision = 0;

  /* auto set width and precision for decimal inputs.  */
  ఒకవేళ (! arg[strcspn (arg, "xX")] && isfinite (ret.విలువ))
    {
      size_t fraction_len = 0;
      ret.వెడల్పు = అక్షరశ్రేణి_పొడవు (arg);

      ఒకవేళ (decimal_బిందువు)
        {
          fraction_len = strcspn (decimal_బిందువు + 1, "eE");
          ఒకవేళ (fraction_len <= గరిష్ఠ_పూర్ణాంకం)
            ret.precision = fraction_len;
          ret.వెడల్పు += (fraction_len == 0                       /* #.  -> #   */
                        ? -1
                        : (decimal_బిందువు == arg                 /* .#  -> 0.# */
                           || !c_isdigit (decimal_బిందువు[-1]))); /* -.# -> 0.# */
        }
      అక్షరం స్థిరరాశి *e = strchr (arg, 'e');
      ఒకవేళ (! e)
        e = strchr (arg, 'E');
      ఒకవేళ (e)
        {
          దీర్ఘ exponent = MAX (strtol (e + 1, ఖాళీ, 10), -LONG_MAX);
          ret.precision += exponent < 0 ? -exponent
                                        : - MIN (ret.precision, exponent);
          /* Don't account for e.... in the width since this is not output.  */
          ret.వెడల్పు -= అక్షరశ్రేణి_పొడవు (arg) - (e - arg);
          /* Adjust the width as per the exponent.  */
          ఒకవేళ (exponent < 0)
            {
              ఒకవేళ (decimal_బిందువు)
                {
                  ఒకవేళ (e == decimal_బిందువు + 1) /* undo #. -> # above  */
                    ret.వెడల్పు++;
                }
              లేకపోతే
                ret.వెడల్పు++;
              exponent = -exponent;
            }
          లేకపోతే
            {
              ఒకవేళ (decimal_బిందువు && ret.precision == 0 && fraction_len)
                ret.వెడల్పు--; /* discount space for '.'  */
              exponent -= MIN (fraction_len, exponent);
            }
          ret.వెడల్పు += exponent;
        }
    }

  తిరిగి ret;
}

/* If FORMAT is a valid printf format for a double argument, return
   its long double equivalent, allocated from dynamic storage, and
   store into *LAYOUT a description of the output layout; otherwise,
   report an error and exit.  */

స్థిర అక్షరం స్థిరరాశి *
long_double_format (అక్షరం స్థిరరాశి *fmt, నిర్మాణం layout *layout)
{
  size_t i;
  size_t ఉపసర్గ_len = 0;

  కొరకు (i = 0; ! (fmt[i] == '%' && fmt[i + 1] != '%'); i += (fmt[i] == '%') + 1)
    {
      ఒకవేళ (!fmt[i])
        error (EXIT_FAILURE, 0,
               _("format %s has no %% directive"), quote (fmt));
      ఉపసర్గ_len++;
    }

  i++;
  i += strspn (fmt + i, "-+#0 '");
  i += strspn (fmt + i, "0123456789");
  ఒకవేళ (fmt[i] == '.')
    {
      i++;
      i += strspn (fmt + i, "0123456789");
    }

  size_t నిడివి_modifier_స్థానభ్రంశం = i;
  bool has_L = (fmt[i] == 'L');
  i += has_L;
  ఒకవేళ (fmt[i] == '\0')
    error (EXIT_FAILURE, 0, _("format %s ends in %%"), quote (fmt));
  ఒకవేళ (! strchr ("efgaEFGA", fmt[i]))
    error (EXIT_FAILURE, 0,
           _("format %s has unknown %%%c directive"), quote (fmt), fmt[i]);

  size_t suffix_len = 0;
  కొరకు (i++; ; i += (fmt[i] == '%') + 1)
    ఒకవేళ (fmt[i] == '%' && fmt[i + 1] != '%')
      error (EXIT_FAILURE, 0, _("format %s has too many %% directives"),
             quote (fmt));
    లేకపోతే ఒకవేళ (fmt[i])
      suffix_len++;
    లేకపోతే
      {
        size_t format_కొలత = i + 1;
        అక్షరం *ldfmt = xmalloc (format_కొలత + 1);
        జ్ఞాపకం_నకలు (ldfmt, fmt, నిడివి_modifier_స్థానభ్రంశం);
        ldfmt[నిడివి_modifier_స్థానభ్రంశం] = 'L';
        strcpy (ldfmt + నిడివి_modifier_స్థానభ్రంశం + 1,
                fmt + నిడివి_modifier_స్థానభ్రంశం + has_L);
        layout->ఉపసర్గ_len = ఉపసర్గ_len;
        layout->suffix_len = suffix_len;
        తిరిగి ldfmt;
      }
}

/* Actually print the sequence of numbers in the specified range, with the
   given or default stepping and format.  */

స్థిర శూన్య
print_numbers (అక్షరం స్థిరరాశి *fmt, నిర్మాణం layout layout,
               దీర్ఘ ద్విగుణ మొదటి, దీర్ఘ ద్విగుణ అడుగు, దీర్ఘ ద్విగుణ చివరి)
{
  bool బహిర్గతం_యొక్క_range = (అడుగు < 0 ? మొదటి < చివరి : చివరి < మొదటి);

  ఒకవేళ (! బహిర్గతం_యొక్క_range)
    {
      దీర్ఘ ద్విగుణ x = మొదటి;

      కొరకు (దీర్ఘ ద్విగుణ i = 1; ; i++)
        {
          దీర్ఘ ద్విగుణ x0 = x;
          ఒకవేళ (రాయి (fmt, x) < 0)
            write_error ();
          ఒకవేళ (బహిర్గతం_యొక్క_range)
            ఆపు;

          /* Mathematically equivalent to 'x += step;', and typically
             less subject to rounding error.  */
          x = మొదటి + i * అడుగు;

          బహిర్గతం_యొక్క_range = (అడుగు < 0 ? x < చివరి : చివరి < x);

          ఒకవేళ (బహిర్గతం_యొక్క_range)
            {
              /* If the number just past LAST prints as a value equal
                 to LAST, and prints differently from the previous
                 number, then print the number.  This avoids problems
                 with rounding.  For example, with the x86 it causes
                 "seq 0 0.000001 0.000003" to print 0.000003 instead
                 of stopping at 0.000002.  */

              bool print_extra_సంఖ్య = false;
              ఒకవేళ (locale_ok)
                setlocale (LC_NUMERIC, "C");
              అక్షరం *x_str;
              పూర్ణాంకం x_strlen = asprintf (&x_str, fmt, x);
              ఒకవేళ (locale_ok)
                setlocale (LC_NUMERIC, "");
              ఒకవేళ (x_strlen < 0)
                xalloc_die ();
              x_str[x_strlen - layout.suffix_len] = '\0';

              దీర్ఘ ద్విగుణ x_val;
              ఒకవేళ (xstrtold (x_str + layout.ఉపసర్గ_len, ఖాళీ,
                            &x_val, cl_strtold)
                  && x_val == చివరి)
                {
                  అక్షరం *x0_str = ఖాళీ;
                  పూర్ణాంకం x0_strlen = asprintf (&x0_str, fmt, x0);
                  ఒకవేళ (x0_strlen < 0)
                    xalloc_die ();
                  x0_str[x0_strlen - layout.suffix_len] = '\0';
                  print_extra_సంఖ్య = !streq (x0_str, x_str);
                  విడుదల (x0_str);
                }

              విడుదల (x_str);
              ఒకవేళ (! print_extra_సంఖ్య)
                ఆపు;
            }

          ఒకవేళ (fputs (separator, stdout) == EOF)
            write_error ();
        }

      ఒకవేళ (fputs (terminator, stdout) == EOF)
        write_error ();
    }
}

/* Return the default format given FIRST, STEP, and LAST.  */
స్థిర అక్షరం స్థిరరాశి *
get_default_format (నిర్వాహ్యం_2 మొదటి, నిర్వాహ్యం_2 అడుగు, నిర్వాహ్యం_2 చివరి)
{
  స్థిర అక్షరం format_buf[పరిమాణం "%0.Lf" + 2 * INT_STRLEN_BOUND (పూర్ణాంకం)];

  పూర్ణాంకం prec = MAX (మొదటి.precision, అడుగు.precision);

  ఒకవేళ (prec != గరిష్ఠ_పూర్ణాంకం && చివరి.precision != గరిష్ఠ_పూర్ణాంకం)
    {
      ఒకవేళ (equal_వెడల్పు)
        {
          /* increase first_width by any increased precision in step */
          size_t మొదటి_వెడల్పు = మొదటి.వెడల్పు + (prec - మొదటి.precision);
          /* adjust last_width to use precision from first/step */
          size_t చివరి_వెడల్పు = చివరి.వెడల్పు + (prec - చివరి.precision);
          ఒకవేళ (చివరి.precision && prec == 0)
            చివరి_వెడల్పు--;  /* don't include space for '.' */
          ఒకవేళ (చివరి.precision == 0 && prec)
            చివరి_వెడల్పు++;  /* include space for '.' */
          ఒకవేళ (మొదటి.precision == 0 && prec)
            మొదటి_వెడల్పు++;  /* include space for '.' */
          size_t వెడల్పు = MAX (మొదటి_వెడల్పు, చివరి_వెడల్పు);
          ఒకవేళ (వెడల్పు <= గరిష్ఠ_పూర్ణాంకం)
            {
              పూర్ణాంకం w = వెడల్పు;
              sprintf (format_buf, "%%0%d.%dLf", w, prec);
              తిరిగి format_buf;
            }
        }
      లేకపోతే
        {
          sprintf (format_buf, "%%.%dLf", prec);
          తిరిగి format_buf;
        }
    }

  తిరిగి "%Lg";
}

/* The nonempty char array P represents a valid non-negative decimal integer.
   ENDP points just after the last char in P.
   Adjust the array to describe the next-larger integer and return
   whether this grows the array by one on the left.  */
స్థిర bool
incr_grows (అక్షరం *p, అక్షరం *endp)
{
  చేయి
    {
      endp--;
      ఒకవేళ (*endp < '9')
        {
          (*endp)++;
          తిరిగి false;
        }
      *endp = '0';
    }
  అయితే (p < endp);

  p[-1] = '1';
  తిరిగి true;
}

/* Compare A and B with lengths A_LEN and B_LEN.  A and B are integers
   represented by nonempty arrays of digits without redundant leading '0'.
   Return negative if A < B, 0 if A = B, positive if A > B.  */
స్థిర పూర్ణాంకం
cmp (అక్షరం స్థిరరాశి *a, idx_t a_len, అక్షరం స్థిరరాశి *b, idx_t b_len)
{
  తిరిగి a_len == b_len ? memcmp (a, b, a_len) : _GL_CMP (a_len, b_len);
}

/* Trim leading 0's from S, but if S is all 0's, leave one.
   Return a pointer to the trimmed string.  */
ATTRIBUTE_శుద్ధ
స్థిర అక్షరం స్థిరరాశి *
trim_leading_zeros (అక్షరం స్థిరరాశి *s)
{
  అక్షరం స్థిరరాశి *p = s;
  అయితే (*s == '0')
    ++s;

  /* If there were only 0's, back up, to leave one.  */
  ఒకవేళ (!*s && s != p)
    --s;
  తిరిగి s;
}

/* Print all whole numbers from A to B, inclusive -- to stdout, each
   followed by a newline.  Then exit.  */
స్థిర శూన్య
seq_fast (అక్షరం స్థిరరాశి *a, అక్షరం స్థిరరాశి *b, uintmax_t అడుగు)
{
  /* Skip past any redundant leading '0's.  Without this, our naive cmp
     function would declare 000 to be larger than 99.  */
  a = trim_leading_zeros (a);
  b = trim_leading_zeros (b);

  idx_t p_len = అక్షరశ్రేణి_పొడవు (a);
  idx_t b_len = అక్షరశ్రేణి_పొడవు (b);
  bool inf = b_len == 3 && memeq (b, "inf", 4);

  /* Allow for at least 31 digits without realloc.
     1 more than p_len is needed for the inf case.  */
  గణన { INITIAL_ALLOC_DIGITS = 31 };
  idx_t inc_కొలత = MAX (MAX (p_len + 1, b_len), INITIAL_ALLOC_DIGITS);
  /* Ensure we only increase by at most 1 digit at buffer boundaries.  */
  static_assert (SEQ_FAST_అడుగు_హద్దు_DIGITS < INITIAL_ALLOC_DIGITS - 1);

  /* Copy A (sans NUL) to end of new buffer.  */
  అక్షరం *p0 = xmalloc (inc_కొలత);
  అక్షరం *endp = p0 + inc_కొలత;
  అక్షరం *p = జ్ఞాపకం_నకలు (endp - p_len, a, p_len);

  /* Reduce number of write calls which is seen to
     give a speed-up of more than 2x over naive stdio code
     when printing the first 10^9 integers.  */
  అక్షరం buf[BUFSIZ];
  అక్షరం *buf_చివర = buf + పరిమాణం buf;
  అక్షరం *bufp = buf;

  అయితే (inf || cmp (p, endp - p, b, b_len) <= 0)
    {
      /* Append number, flushing output buffer while the number's
         digits do not fit with room for a separator or terminator.  */
      అక్షరం *pp = p;
      అయితే (buf_చివర - bufp <= endp - pp)
        {
          జ్ఞాపకం_నకలు (bufp, pp, buf_చివర - bufp);
          pp += buf_చివర - bufp;
          ఒకవేళ (నిండిన_write (STDOUT_FILENO, buf, పరిమాణం buf) != పరిమాణం buf)
            write_error ();
          bufp = buf;
        }

      /* The rest of the number, followed by a separator or terminator,
         will fit.  Tentatively append a separator.  */
      bufp = mempcpy (bufp, pp, endp - pp);
      *bufp++ = *separator;

      /* Grow number buffer if needed for the inf case.  */
      ఒకవేళ (p == p0)
        {
          అక్షరం *new_p0 = xpalloc (ఖాళీ, &inc_కొలత, 1, -1, 1);
          idx_t saved_p_len = endp - p;
          endp = new_p0 + inc_కొలత;
          p = జ్ఞాపకం_నకలు (endp - saved_p_len, p0, saved_p_len);
          విడుదల (p0);
          p0 = new_p0;
        }

      /* Compute next number.  */
      కొరకు (uintmax_t n_incr = అడుగు; n_incr; n_incr--)
        p -= incr_grows (p, endp);
    }

  /* Write the remaining buffered output with a terminator instead of
     a separator.  */
  idx_t మిగిలిన = bufp - buf;
  ఒకవేళ (మిగిలిన)
    {
      bufp[-1] = *terminator;
      ఒకవేళ (నిండిన_write (STDOUT_FILENO, buf, మిగిలిన) != మిగిలిన)
        write_error ();
    }

  నిష్క్రమించు (EXIT_SUCCESS);
}

/* Return true if S consists of at least one digit and no non-digits.  */
ATTRIBUTE_శుద్ధ
స్థిర bool
all_digits_p (అక్షరం స్థిరరాశి *s)
{
  size_t n = అక్షరశ్రేణి_పొడవు (s);
  తిరిగి c_isdigit (s[0]) && n == strspn (s, "0123456789");
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  /* The printf(3) format used for output.  */
  అక్షరం స్థిరరాశి *format_str = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  locale_ok = !!setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We have to handle negative numbers in the command line but this
     conflicts with the command line arguments.  So explicitly check first
     whether the next argument looks like a negative number.  */
  అయితే (optind < argc)
    {
      పూర్ణాంకం optc;
      ఒకవేళ (argv[optind][0] == '-'
          && ((optc = argv[optind][1]) == '.' || c_isdigit (optc)))
        {
          /* means negative number */
          ఆపు;
        }

      optc = getopt_long (argc, argv, "+f:s:w", long_options, ఖాళీ);
      ఒకవేళ (optc == -1)
        ఆపు;

      ఎంపిక (optc)
        {
        స్థితి 'f':
          format_str = optarg;
          ఆపు;

        స్థితి 's':
          separator = optarg;
          ఆపు;

        స్థితి 'w':
          equal_వెడల్పు = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  పూర్ణాంకం n_వాదనలు = argc - optind;
  ఒకవేళ (n_వాదనలు < 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (3 < n_వాదనలు)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 3]));
      usage (EXIT_FAILURE);
    }

  నిర్మాణం layout layout = { 0, 0 };
  ఒకవేళ (format_str)
    format_str = long_double_format (format_str, &layout);

  ఒకవేళ (format_str != ఖాళీ && equal_వెడల్పు)
    {
      error (0, 0, _("format string may not be specified"
                     " when printing equal width strings"));
      usage (EXIT_FAILURE);
    }

  అక్షరం స్థిరరాశి *user_ప్రారంభం = n_వాదనలు == 1 ? "1" : argv[optind];

  /* If the following hold:
     - no format string, [FIXME: relax this, eventually]
     - integer start (or no start)
     - integer end
     - integer increment <= SEQ_FAST_STEP_LIMIT
     then use the much more efficient integer-only code,
     operating on arbitrarily large numbers.  */
  నిర్వాహ్యం_2 అడుగు = { 1, 1, 0 };
  bool fast_అడుగు_ok = false;
  ఒకవేళ (n_వాదనలు != 3
      || (all_digits_p (argv[optind + 1])
          && xstrtold (argv[optind + 1], ఖాళీ, &అడుగు.విలువ, cl_strtold)
          && 0 < అడుగు.విలువ && అడుగు.విలువ <= SEQ_FAST_అడుగు_హద్దు))
    fast_అడుగు_ok = true;

  ఒకవేళ (all_digits_p (argv[optind])
      && (n_వాదనలు == 1 || all_digits_p (argv[optind + 1]))
      && (n_వాదనలు < 3 || (fast_అడుగు_ok
                         && all_digits_p (argv[optind + 2])))
      && !equal_వెడల్పు && !format_str && అక్షరశ్రేణి_పొడవు (separator) == 1)
    {
      అక్షరం స్థిరరాశి *s1 = user_ప్రారంభం;
      అక్షరం స్థిరరాశి *s2 = argv[optind + (n_వాదనలు - 1)];
      seq_fast (s1, s2, అడుగు.విలువ);
    }

  నిర్వాహ్యం_2 మొదటి = { 1, 1, 0 };
  నిర్వాహ్యం_2 చివరి = పరిశీలన_arg (argv[optind++]);

  ఒకవేళ (optind < argc)
    {
      మొదటి = చివరి;
      చివరి = పరిశీలన_arg (argv[optind++]);

      ఒకవేళ (optind < argc)
        {
          అడుగు = చివరి;
          ఒకవేళ (అడుగు.విలువ == 0)
            {
              error (0, 0, _("invalid Zero increment value: %s"),
                     quote (argv[optind - 1]));
              usage (EXIT_FAILURE);
            }

          చివరి = పరిశీలన_arg (argv[optind++]);
        }
    }

  /* Try the fast method again, for integers of the form 1e1 etc.,
     or "inf" end value.  */
  ఒకవేళ (మొదటి.precision == 0 && అడుగు.precision == 0 && చివరి.precision == 0
      && isfinite (మొదటి.విలువ) && 0 <= మొదటి.విలువ && 0 <= చివరి.విలువ
      && 0 < అడుగు.విలువ && అడుగు.విలువ <= SEQ_FAST_అడుగు_హద్దు
      && !equal_వెడల్పు && !format_str && అక్షరశ్రేణి_పొడవు (separator) == 1)
    {
      అక్షరం *s1 = (all_digits_p (user_ప్రారంభం)
                  ? xstrdup (user_ప్రారంభం) : xasprintf ("%0.Lf", మొదటి.విలువ));
      /* Ensure "inf" is used.  */
      అక్షరం *s2 = (! isfinite (చివరి.విలువ)
                  ? xstrdup ("inf") : xasprintf ("%0.Lf", చివరి.విలువ));

      ఒకవేళ (*s1 != '-' && *s2 != '-')
        seq_fast (s1, s2, అడుగు.విలువ);

      విడుదల (s1);
      విడుదల (s2);
      /* Upon any failure, let the more general code deal with it.  */
    }

  ఒకవేళ (format_str == ఖాళీ)
    format_str = get_default_format (మొదటి, అడుగు, చివరి);

  print_numbers (format_str, layout, మొదటి.విలువ, అడుగు.విలువ, చివరి.విలువ);

  main_exit (EXIT_SUCCESS);
}
