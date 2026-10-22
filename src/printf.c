/* printf - format and print data
   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <wchar.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "cl-strtod.h"
#அந்தர்பாவ "octhexdigits.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "unicodeio.h"
#அந்தர்பாவ "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printf"

#define AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
நிலையான முழு exit_status;

/* True if the POSIXLY_CORRECT environment variable is set.  */
நிலையான bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
நிலையான எழுத்து மாறிலி *மாறிலி cfcc_செய்தி =
 N_("warning: %s: character(s) following character constant have been ignored");

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s FORMAT [ARGUMENT]...\n\
  or:  %s OPTION\n\
"),
              program_name, program_name);
      fputs (_("\
Print ARGUMENT(s) according to FORMAT, or execute according to OPTION:\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FORMAT controls the output as in C printf.  Interpreted sequences are:\n\
\n\
  \\\"      இரட்டை quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new வரி\n\
  \\r      carriage திரும்பு\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal மதிப்பு NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal மதிப்பு HH (1 to 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex மதிப்பு HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex மதிப்பு HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      ARGUMENT as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      ARGUMENT is printed in a format that can be reused as shell உள்ளீடு,\n\
          escaping non-printable characters with the POSIX $'' syntax\
\n\n\
and all C printf(3) format specifications ending with one of diouxXfaAeEgGcs,\n\
with ARGUMENTs converted to proper type first.\n\
"), stdout);
      fputs (_("\
FORMAT is reused to convert all specified arguments.\n\
"), stdout);
      fputs (_("\
The following table shows supported conversion specifiers,\n\
with examples including usage of width, precision, and flags.\n\
\n\
  CONVERSION  Shell format example        Output\n\
  %s          '[%10s]' 'left pad'         [  இடது pad]\n\
  %s          '[%-10s]' 'right pad'       [வலது pad ]\n\
  %c %s       '%c %.3s' un unibyte        u uni\n\
\n\
  %d %i       \"%'d ;%04i\" 1234 '-1'       1,234 ;-001\n\
  %o          '%#o' $((2#110100100))      0644\n\
  %u          '%u' 0xFFFFFFFF             4294967295\n\
  %x          '0x%08x' 11259375           0x00abcdef\n\
\n\
  %a %f       '%1$a %1$f' 0.0009765625    0x8p-13 0.000977\n\
  %e %f       '%1$e %1$f' 123.456789      1.234568e+02 123.456789\n\
  %g          '%1$g %1$.2g' 1.2345678e3   1234.57 1.2e+03\n\
\n\
  %b          '%bfoo' 'bar\\cbaz' qux      bar\n\
  %q          '%q ' 'a' \\;b $'c\\n'        a ';b' 'c'$'\\n'\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

நிலையான வெறுமை
verify_numeric (எழுத்து மாறிலி *s, எழுத்து மாறிலி *end)
{
  எனில் (s == end)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_status = EXIT_FAILURE;
    }
  இல்லையெனில் எனில் (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_status = EXIT_FAILURE;
    }
  இல்லையெனில் எனில் (*end)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_status = EXIT_FAILURE;
    }
}

#define STRTOX(TYPE, FUNC_NAME, LIB_FUNC_கோவை)				 \
நிலையான TYPE								 \
FUNC_NAME (எழுத்து மாறிலி *s)						 \
{									 \
  எழுத்து *end;								 \
  TYPE val;								 \
                                                                         \
  எனில் ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      unsigned எழுத்து ch = *++s;						 \
      val = ch;								 \
                                                                         \
      எனில் (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = strlen (s);					 \
          ssize_t bytes;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          bytes = mbrtowc (&wc, s, slen, &mbstate);			 \
          எனில் (0 < bytes)						 \
            {								 \
              val = wc;							 \
              s += bytes - 1;						 \
            }								 \
        }								 \
                                                                         \
      /* If POSIXLY_CORRECT is not set, then give a warning that there	 \
         are characters following the character constant and that GNU	 \
         printf is ignoring those characters.  If POSIXLY_CORRECT *is*	 \
         set, then don't give the warning.  */				 \
      எனில் (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_செய்தி), s);					 \
    }									 \
  இல்லையெனில்									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_கோவை);						 \
      verify_numeric (s, end);						 \
    }									 \
  திரும்பு val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &end, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &end, 0))
STRTOX (long இரட்டை, vstrtold,   cl_strtold (s, &end))

/* Output a single-character \ escape.  */

நிலையான வெறுமை
print_esc_char (எழுத்து c)
{
  தேர்வு (c)
    {
    நிலை 'a':			/* Alert. */
      putchar ('\a');
      நிறுத்து;
    நிலை 'b':			/* Backspace. */
      putchar ('\b');
      நிறுத்து;
    நிலை 'c':			/* Cancel the rest of the output. */
      exit (EXIT_SUCCESS);
      நிறுத்து;
    நிலை 'e':			/* Escape. */
      putchar ('\x1B');
      நிறுத்து;
    நிலை 'f':			/* Form feed. */
      putchar ('\f');
      நிறுத்து;
    நிலை 'n':			/* New line. */
      putchar ('\n');
      நிறுத்து;
    நிலை 'r':			/* Carriage return. */
      putchar ('\r');
      நிறுத்து;
    நிலை 't':			/* Horizontal tab. */
      putchar ('\t');
      நிறுத்து;
    நிலை 'v':			/* Vertical tab. */
      putchar ('\v');
      நிறுத்து;
    இயல்பு:
      putchar (c);
      நிறுத்து;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

நிலையான முழு
print_esc (எழுத்து மாறிலி *escstart, bool octal_0)
{
  எழுத்து மாறிலி *p = escstart + 1;
  முழு esc_மதிப்பு = 0;		/* Value of \nnn escape. */
  முழு esc_length;		/* Length of \nnn escape. */

  எனில் (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      க்கு (esc_length = 0, ++p;
           esc_length < 2 && c_isxdigit (*p);
           ++esc_length, ++p)
        esc_மதிப்பு = esc_மதிப்பு * 16 + fromhex (*p);
      எனில் (esc_length == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      putchar (esc_மதிப்பு);
    }
  இல்லையெனில் எனில் (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      க்கு (esc_length = 0, p += octal_0 && *p == '0';
           esc_length < 3 && isoct (*p);
           ++esc_length, ++p)
        esc_மதிப்பு = esc_மதிப்பு * 8 + fromoct (*p);
      putchar (esc_மதிப்பு);
    }
  இல்லையெனில் எனில் (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  இல்லையெனில் எனில் (*p == 'u' || *p == 'U')
    {
      எழுத்து esc_char = *p;
      unsigned முழு uni_மதிப்பு;

      uni_மதிப்பு = 0;
      க்கு (esc_length = (esc_char == 'u' ? 4 : 8), ++p;
           esc_length > 0;
           --esc_length, ++p)
        {
          எனில் (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_மதிப்பு = uni_மதிப்பு * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      எனில் (uni_மதிப்பு >= 0xd800 && uni_மதிப்பு <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_மதிப்பு);

      print_unicode_char (stdout, uni_மதிப்பு, 0);
    }
  இல்லையெனில்
    {
      putchar ('\\');
      எனில் (*p)
        {
          putchar (*p);
          p++;
        }
    }
  திரும்பு p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

நிலையான வெறுமை
print_esc_string (எழுத்து மாறிலி *str)
{
  க்கு (; *str; str++)
    எனில் (*str == '\\')
      str += print_esc (str, true);
    இல்லையெனில்
      putchar (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

நிலையான வெறுமை
print_direc (எழுத்து மாறிலி *start, எழுத்து conversion,
             bool have_field_width, முழு field_width,
             bool have_precision, முழு precision,
             எழுத்து மாறிலி *argument)
{
  எழுத்து *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    எழுத்து *q;
    எழுத்து மாறிலி *length_modifier;
    size_t length_modifier_len;

    தேர்வு (conversion)
      {
      நிலை 'd': நிலை 'i': நிலை 'o': நிலை 'u': நிலை 'x': நிலை 'X':
        length_modifier = "j";
        length_modifier_len = 1;
        நிறுத்து;

      நிலை 'a': நிலை 'e': நிலை 'f': நிலை 'g':
      நிலை 'A': நிலை 'E': நிலை 'F': நிலை 'G':
        length_modifier = "L";
        length_modifier_len = 1;
        நிறுத்து;

      இயல்பு:
        length_modifier = start;  /* Any valid pointer will do.  */
        length_modifier_len = 0;
        நிறுத்து;
      }

    size_t length = strlen (start);
    p = xmalloc (length + length_modifier_len + 2);
    q = mempcpy (p, start, length);
    q = mempcpy (q, length_modifier, length_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  தேர்வு (conversion)
    {
    நிலை 'd':
    நிலை 'i':
      {
        intmax_t arg = argument ? vstrtoimax (argument) : 0;
        எனில் (!have_field_width)
          {
            எனில் (!have_precision)
              xprintf (p, arg);
            இல்லையெனில்
              xprintf (p, precision, arg);
          }
        இல்லையெனில்
          {
            எனில் (!have_precision)
              xprintf (p, field_width, arg);
            இல்லையெனில்
              xprintf (p, field_width, precision, arg);
          }
      }
      நிறுத்து;

    நிலை 'o':
    நிலை 'u':
    நிலை 'x':
    நிலை 'X':
      {
        uintmax_t arg = argument ? vstrtoumax (argument) : 0;
        எனில் (!have_field_width)
          {
            எனில் (!have_precision)
              xprintf (p, arg);
            இல்லையெனில்
              xprintf (p, precision, arg);
          }
        இல்லையெனில்
          {
            எனில் (!have_precision)
              xprintf (p, field_width, arg);
            இல்லையெனில்
              xprintf (p, field_width, precision, arg);
          }
      }
      நிறுத்து;

    நிலை 'a':
    நிலை 'A':
    நிலை 'e':
    நிலை 'E':
    நிலை 'f':
    நிலை 'F':
    நிலை 'g':
    நிலை 'G':
      {
        long இரட்டை arg = argument ? vstrtold (argument) : 0;
        எனில் (!have_field_width)
          {
            எனில் (!have_precision)
              xprintf (p, arg);
            இல்லையெனில்
              xprintf (p, precision, arg);
          }
        இல்லையெனில்
          {
            எனில் (!have_precision)
              xprintf (p, field_width, arg);
            இல்லையெனில்
              xprintf (p, field_width, precision, arg);
          }
      }
      நிறுத்து;

    நிலை 'c':
      {
        எழுத்து c = argument ? *argument : '\0';
        எனில் (!have_field_width)
          xprintf (p, c);
        இல்லையெனில்
          xprintf (p, field_width, c);
      }
      நிறுத்து;

    நிலை 's':
      எனில் (!argument)
        argument = "";
      எனில் (!have_field_width)
        {
          எனில் (!have_precision)
            xprintf (p, argument);
          இல்லையெனில்
            xprintf (p, precision, argument);
        }
      இல்லையெனில்
        {
          எனில் (!have_precision)
            xprintf (p, field_width, argument);
          இல்லையெனில்
            xprintf (p, field_width, precision, argument);
        }
      நிறுத்து;
    }

  விடுவி (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

கட்டமைப்பு arg_cursor
{
  எழுத்து மாறிலி *f;	/* Pointer into 'format'.  */
  முழு curr_arg;		/* Current offset.  */
  முழு curr_s_arg;	/* Current sequential offset.  */
  முழு end_arg;		/* End arg processed.  */
  முழு direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_PURE நிலையான கட்டமைப்பு arg_cursor
get_curr_arg (முழு pos, கட்டமைப்பு arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  முழு arg = 0;
  எழுத்து மாறிலி *f = ac.f;
  எனில் (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      முழு a = *f++ - '0';
      க்கு (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_add (&a, a, *f - '0');
        }
      எனில் (*f == '$')
        arg = v ? INT_MAX : a;
    }

  எனில் (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      எனில் (pos == 0)
        ac.direc_arg = arg;
    }
  இல்லையெனில்
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  எனில் (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.end_arg = MAX (ac.end_arg, arg);
    }
  திரும்பு ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

நிலையான முழு
print_formatted (எழுத்து மாறிலி *format, முழு argc, எழுத்து **argv)
{
  கட்டமைப்பு arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.end_arg = ac.direc_arg = -1;
  எழுத்து மாறிலி *direc_start;	/* Start of % directive.  */
  எழுத்து *direc;			/* Generated % directive.  */
  எழுத்து *pdirec;			/* Pointer to current end of directive.  */
  bool have_field_width;	/* True if FIELD_WIDTH is valid.  */
  முழு field_width = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  முழு precision = 0;		/* Arg to second '*'.  */
  எழுத்து ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (strlen (format) + 1);

  க்கு (ac.f = format; *ac.f; ac.f++)
    {
      தேர்வு (*ac.f)
        {
        நிலை '%':
          direc_start = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_field_width = have_precision = false;
          எனில் (*ac.f == '%')
            {
              putchar ('%');
              நிறுத்து;
            }

          ac = get_curr_arg (0, ac);

          எனில் (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              எனில் (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              நிறுத்து;
            }

          எனில் (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              எனில் (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              நிறுத்து;
            }

          memset (ok, 0, அளவு ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          க்கு (;; ac.f++)
            {
              தேர்வு (*ac.f)
                {
#எனில் (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                நிலை 'I':
#endif
                நிலை '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  நிறுத்து;
                நிலை '-': நிலை '+': நிலை ' ':
                  நிறுத்து;
                நிலை '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  நிறுத்து;
                நிலை '0':
                  ok['c'] = ok['s'] = 0;
                  நிறுத்து;
                இயல்பு:
                  செல் no_more_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_more_flag_characters:

          எனில் (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              எனில் (ac.curr_arg < argc)
                {
                  intmax_t width = vstrtoimax (argv[ac.curr_arg]);
                  எனில் (INT_MIN <= width && width <= INT_MAX)
                    field_width = width;
                  இல்லையெனில்
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              இல்லையெனில்
                field_width = 0;
              have_field_width = true;
            }
          இல்லையெனில்
            போது (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          எனில் (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              எனில் (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  எனில் (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      எனில் (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      இல்லையெனில் எனில் (INT_MAX < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      இல்லையெனில்
                        precision = prec;
                    }
                  இல்லையெனில்
                    precision = 0;
                  have_precision = true;
                }
              இல்லையெனில்
                போது (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          போது (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            unsigned எழுத்து conversion = *ac.f;
            முழு speclen = MIN (ac.f + 1 - direc_start, INT_MAX);
            எனில் (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_start);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_field_width, field_width,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : வெற்று);

          நிறுத்து;

        நிலை '\\':
          ac.f += print_esc (ac.f, false);
          நிறுத்து;

        இயல்பு:
          putchar (*ac.f);
        }
    }

  விடுவி (direc);
  திரும்பு MIN (argc, ac.end_arg + 1);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  எழுத்து *format;
  முழு args_used;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != வெற்று);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  எனில் (argc == 2)
    {
      எனில் (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      எனில் (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (எழுத்து *) வெற்று);
          திரும்பு EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  எனில் (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  எனில் (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  செய்
    {
      args_used = print_formatted (format, argc, argv);
      argc -= args_used;
      argv += args_used;
    }
  போது (args_used > 0 && argc > 0);

  எனில் (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  திரும்பு exit_status;
}
