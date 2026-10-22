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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <wchar.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "cl-strtod.h"
#অন্তর্ভাব "octhexdigits.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "unicodeio.h"
#অন্তর্ভাব "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printf"

#define AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
স্থির পূর্ণাঙ্ক exit_status;

/* True if the POSIXLY_CORRECT environment variable is set.  */
স্থির bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
স্থির বর্ণ ধ্রুব *ধ্রুব cfcc_বার্তা =
 N_("warning: %s: character(s) following character constant have been ignored");

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
  \\\"      দ্বিগুণ quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new সারি\n\
  \\r      carriage প্রত্যাগম\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal মান NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal মান HH (1 to 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex মান HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex মান HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      ARGUMENT as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      ARGUMENT is printed in a format that can be reused as shell প্রবেশ,\n\
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
  %s          '[%10s]' 'left pad'         [  বাম pad]\n\
  %s          '[%-10s]' 'right pad'       [ডান pad ]\n\
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

স্থির শূন্য
verify_numeric (বর্ণ ধ্রুব *s, বর্ণ ধ্রুব *end)
{
  যদি (s == end)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_status = EXIT_FAILURE;
    }
  অন্যথা যদি (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_status = EXIT_FAILURE;
    }
  অন্যথা যদি (*end)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_status = EXIT_FAILURE;
    }
}

#define STRTOX(TYPE, FUNC_NAME, LIB_FUNC_রাশিমালা)				 \
স্থির TYPE								 \
FUNC_NAME (বর্ণ ধ্রুব *s)						 \
{									 \
  বর্ণ *end;								 \
  TYPE val;								 \
                                                                         \
  যদি ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      unsigned বর্ণ ch = *++s;						 \
      val = ch;								 \
                                                                         \
      যদি (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = strlen (s);					 \
          ssize_t bytes;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          bytes = mbrtowc (&wc, s, slen, &mbstate);			 \
          যদি (0 < bytes)						 \
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
      যদি (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_বার্তা), s);					 \
    }									 \
  অন্যথা									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_রাশিমালা);						 \
      verify_numeric (s, end);						 \
    }									 \
  প্রত্যাগম val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &end, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &end, 0))
STRTOX (long দ্বিগুণ, vstrtold,   cl_strtold (s, &end))

/* Output a single-character \ escape.  */

স্থির শূন্য
print_esc_char (বর্ণ c)
{
  বিকল্প (c)
    {
    পক্ষ 'a':			/* Alert. */
      putchar ('\a');
      ভঙ্গ;
    পক্ষ 'b':			/* Backspace. */
      putchar ('\b');
      ভঙ্গ;
    পক্ষ 'c':			/* Cancel the rest of the output. */
      exit (EXIT_SUCCESS);
      ভঙ্গ;
    পক্ষ 'e':			/* Escape. */
      putchar ('\x1B');
      ভঙ্গ;
    পক্ষ 'f':			/* Form feed. */
      putchar ('\f');
      ভঙ্গ;
    পক্ষ 'n':			/* New line. */
      putchar ('\n');
      ভঙ্গ;
    পক্ষ 'r':			/* Carriage return. */
      putchar ('\r');
      ভঙ্গ;
    পক্ষ 't':			/* Horizontal tab. */
      putchar ('\t');
      ভঙ্গ;
    পক্ষ 'v':			/* Vertical tab. */
      putchar ('\v');
      ভঙ্গ;
    স্বভাব:
      putchar (c);
      ভঙ্গ;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

স্থির পূর্ণাঙ্ক
print_esc (বর্ণ ধ্রুব *escstart, bool octal_0)
{
  বর্ণ ধ্রুব *p = escstart + 1;
  পূর্ণাঙ্ক esc_মান = 0;		/* Value of \nnn escape. */
  পূর্ণাঙ্ক esc_length;		/* Length of \nnn escape. */

  যদি (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      প্রতি (esc_length = 0, ++p;
           esc_length < 2 && c_isxdigit (*p);
           ++esc_length, ++p)
        esc_মান = esc_মান * 16 + fromhex (*p);
      যদি (esc_length == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      putchar (esc_মান);
    }
  অন্যথা যদি (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      প্রতি (esc_length = 0, p += octal_0 && *p == '0';
           esc_length < 3 && isoct (*p);
           ++esc_length, ++p)
        esc_মান = esc_মান * 8 + fromoct (*p);
      putchar (esc_মান);
    }
  অন্যথা যদি (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  অন্যথা যদি (*p == 'u' || *p == 'U')
    {
      বর্ণ esc_char = *p;
      unsigned পূর্ণাঙ্ক uni_মান;

      uni_মান = 0;
      প্রতি (esc_length = (esc_char == 'u' ? 4 : 8), ++p;
           esc_length > 0;
           --esc_length, ++p)
        {
          যদি (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_মান = uni_মান * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      যদি (uni_মান >= 0xd800 && uni_মান <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_মান);

      print_unicode_char (stdout, uni_মান, 0);
    }
  অন্যথা
    {
      putchar ('\\');
      যদি (*p)
        {
          putchar (*p);
          p++;
        }
    }
  প্রত্যাগম p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

স্থির শূন্য
print_esc_string (বর্ণ ধ্রুব *str)
{
  প্রতি (; *str; str++)
    যদি (*str == '\\')
      str += print_esc (str, true);
    অন্যথা
      putchar (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

স্থির শূন্য
print_direc (বর্ণ ধ্রুব *start, বর্ণ conversion,
             bool have_field_width, পূর্ণাঙ্ক field_width,
             bool have_precision, পূর্ণাঙ্ক precision,
             বর্ণ ধ্রুব *argument)
{
  বর্ণ *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    বর্ণ *q;
    বর্ণ ধ্রুব *length_modifier;
    size_t length_modifier_len;

    বিকল্প (conversion)
      {
      পক্ষ 'd': পক্ষ 'i': পক্ষ 'o': পক্ষ 'u': পক্ষ 'x': পক্ষ 'X':
        length_modifier = "j";
        length_modifier_len = 1;
        ভঙ্গ;

      পক্ষ 'a': পক্ষ 'e': পক্ষ 'f': পক্ষ 'g':
      পক্ষ 'A': পক্ষ 'E': পক্ষ 'F': পক্ষ 'G':
        length_modifier = "L";
        length_modifier_len = 1;
        ভঙ্গ;

      স্বভাব:
        length_modifier = start;  /* Any valid pointer will do.  */
        length_modifier_len = 0;
        ভঙ্গ;
      }

    size_t length = strlen (start);
    p = xmalloc (length + length_modifier_len + 2);
    q = mempcpy (p, start, length);
    q = mempcpy (q, length_modifier, length_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  বিকল্প (conversion)
    {
    পক্ষ 'd':
    পক্ষ 'i':
      {
        intmax_t arg = argument ? vstrtoimax (argument) : 0;
        যদি (!have_field_width)
          {
            যদি (!have_precision)
              xprintf (p, arg);
            অন্যথা
              xprintf (p, precision, arg);
          }
        অন্যথা
          {
            যদি (!have_precision)
              xprintf (p, field_width, arg);
            অন্যথা
              xprintf (p, field_width, precision, arg);
          }
      }
      ভঙ্গ;

    পক্ষ 'o':
    পক্ষ 'u':
    পক্ষ 'x':
    পক্ষ 'X':
      {
        uintmax_t arg = argument ? vstrtoumax (argument) : 0;
        যদি (!have_field_width)
          {
            যদি (!have_precision)
              xprintf (p, arg);
            অন্যথা
              xprintf (p, precision, arg);
          }
        অন্যথা
          {
            যদি (!have_precision)
              xprintf (p, field_width, arg);
            অন্যথা
              xprintf (p, field_width, precision, arg);
          }
      }
      ভঙ্গ;

    পক্ষ 'a':
    পক্ষ 'A':
    পক্ষ 'e':
    পক্ষ 'E':
    পক্ষ 'f':
    পক্ষ 'F':
    পক্ষ 'g':
    পক্ষ 'G':
      {
        long দ্বিগুণ arg = argument ? vstrtold (argument) : 0;
        যদি (!have_field_width)
          {
            যদি (!have_precision)
              xprintf (p, arg);
            অন্যথা
              xprintf (p, precision, arg);
          }
        অন্যথা
          {
            যদি (!have_precision)
              xprintf (p, field_width, arg);
            অন্যথা
              xprintf (p, field_width, precision, arg);
          }
      }
      ভঙ্গ;

    পক্ষ 'c':
      {
        বর্ণ c = argument ? *argument : '\0';
        যদি (!have_field_width)
          xprintf (p, c);
        অন্যথা
          xprintf (p, field_width, c);
      }
      ভঙ্গ;

    পক্ষ 's':
      যদি (!argument)
        argument = "";
      যদি (!have_field_width)
        {
          যদি (!have_precision)
            xprintf (p, argument);
          অন্যথা
            xprintf (p, precision, argument);
        }
      অন্যথা
        {
          যদি (!have_precision)
            xprintf (p, field_width, argument);
          অন্যথা
            xprintf (p, field_width, precision, argument);
        }
      ভঙ্গ;
    }

  মুক্ত (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

সংরচনা arg_cursor
{
  বর্ণ ধ্রুব *f;	/* Pointer into 'format'.  */
  পূর্ণাঙ্ক curr_arg;		/* Current offset.  */
  পূর্ণাঙ্ক curr_s_arg;	/* Current sequential offset.  */
  পূর্ণাঙ্ক end_arg;		/* End arg processed.  */
  পূর্ণাঙ্ক direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_PURE স্থির সংরচনা arg_cursor
get_curr_arg (পূর্ণাঙ্ক pos, সংরচনা arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  পূর্ণাঙ্ক arg = 0;
  বর্ণ ধ্রুব *f = ac.f;
  যদি (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      পূর্ণাঙ্ক a = *f++ - '0';
      প্রতি (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_add (&a, a, *f - '0');
        }
      যদি (*f == '$')
        arg = v ? INT_MAX : a;
    }

  যদি (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      যদি (pos == 0)
        ac.direc_arg = arg;
    }
  অন্যথা
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  যদি (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.end_arg = MAX (ac.end_arg, arg);
    }
  প্রত্যাগম ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

স্থির পূর্ণাঙ্ক
print_formatted (বর্ণ ধ্রুব *format, পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  সংরচনা arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.end_arg = ac.direc_arg = -1;
  বর্ণ ধ্রুব *direc_start;	/* Start of % directive.  */
  বর্ণ *direc;			/* Generated % directive.  */
  বর্ণ *pdirec;			/* Pointer to current end of directive.  */
  bool have_field_width;	/* True if FIELD_WIDTH is valid.  */
  পূর্ণাঙ্ক field_width = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  পূর্ণাঙ্ক precision = 0;		/* Arg to second '*'.  */
  বর্ণ ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (strlen (format) + 1);

  প্রতি (ac.f = format; *ac.f; ac.f++)
    {
      বিকল্প (*ac.f)
        {
        পক্ষ '%':
          direc_start = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_field_width = have_precision = false;
          যদি (*ac.f == '%')
            {
              putchar ('%');
              ভঙ্গ;
            }

          ac = get_curr_arg (0, ac);

          যদি (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              যদি (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              ভঙ্গ;
            }

          যদি (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              যদি (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              ভঙ্গ;
            }

          memset (ok, 0, পরিমাণ ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          প্রতি (;; ac.f++)
            {
              বিকল্প (*ac.f)
                {
#যদি (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                পক্ষ 'I':
#endif
                পক্ষ '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  ভঙ্গ;
                পক্ষ '-': পক্ষ '+': পক্ষ ' ':
                  ভঙ্গ;
                পক্ষ '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  ভঙ্গ;
                পক্ষ '0':
                  ok['c'] = ok['s'] = 0;
                  ভঙ্গ;
                স্বভাব:
                  গচ্ছ no_more_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_more_flag_characters:

          যদি (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              যদি (ac.curr_arg < argc)
                {
                  intmax_t width = vstrtoimax (argv[ac.curr_arg]);
                  যদি (INT_MIN <= width && width <= INT_MAX)
                    field_width = width;
                  অন্যথা
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              অন্যথা
                field_width = 0;
              have_field_width = true;
            }
          অন্যথা
            যাবত্ (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          যদি (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              যদি (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  যদি (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      যদি (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      অন্যথা যদি (INT_MAX < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      অন্যথা
                        precision = prec;
                    }
                  অন্যথা
                    precision = 0;
                  have_precision = true;
                }
              অন্যথা
                যাবত্ (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          যাবত্ (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            unsigned বর্ণ conversion = *ac.f;
            পূর্ণাঙ্ক speclen = MIN (ac.f + 1 - direc_start, INT_MAX);
            যদি (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_start);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_field_width, field_width,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : রিক্ত);

          ভঙ্গ;

        পক্ষ '\\':
          ac.f += print_esc (ac.f, false);
          ভঙ্গ;

        স্বভাব:
          putchar (*ac.f);
        }
    }

  মুক্ত (direc);
  প্রত্যাগম MIN (argc, ac.end_arg + 1);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  বর্ণ *format;
  পূর্ণাঙ্ক args_used;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != রিক্ত);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  যদি (argc == 2)
    {
      যদি (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      যদি (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (বর্ণ *) রিক্ত);
          প্রত্যাগম EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  যদি (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  যদি (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  কুরু
    {
      args_used = print_formatted (format, argc, argv);
      argc -= args_used;
      argv += args_used;
    }
  যাবত্ (args_used > 0 && argc > 0);

  যদি (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  প্রত্যাগম exit_status;
}
