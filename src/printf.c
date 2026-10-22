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
স্থির পূর্ণসংখ্যা exit_status;

/* True if the POSIXLY_CORRECT environment variable is set.  */
স্থির bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
স্থির অক্ষর ধ্রুবক *ধ্রুবক cfcc_বার্তা =
 N_("warning: %s: character(s) following character constant have been ignored");

শূন্য
usage (পূর্ণসংখ্যা status)
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
  \\r      carriage ফেরত\n\
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
verify_numeric (অক্ষর ধ্রুবক *s, অক্ষর ধ্রুবক *end)
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
FUNC_NAME (অক্ষর ধ্রুবক *s)						 \
{									 \
  অক্ষর *end;								 \
  TYPE val;								 \
                                                                         \
  যদি ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      unsigned অক্ষর ch = *++s;						 \
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
  ফেরত val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &end, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &end, 0))
STRTOX (long দ্বিগুণ, vstrtold,   cl_strtold (s, &end))

/* Output a single-character \ escape.  */

স্থির শূন্য
print_esc_char (অক্ষর c)
{
  নির্বাচন (c)
    {
    ক্ষেত্র 'a':			/* Alert. */
      putchar ('\a');
      বিরতি;
    ক্ষেত্র 'b':			/* Backspace. */
      putchar ('\b');
      বিরতি;
    ক্ষেত্র 'c':			/* Cancel the rest of the output. */
      exit (EXIT_SUCCESS);
      বিরতি;
    ক্ষেত্র 'e':			/* Escape. */
      putchar ('\x1B');
      বিরতি;
    ক্ষেত্র 'f':			/* Form feed. */
      putchar ('\f');
      বিরতি;
    ক্ষেত্র 'n':			/* New line. */
      putchar ('\n');
      বিরতি;
    ক্ষেত্র 'r':			/* Carriage return. */
      putchar ('\r');
      বিরতি;
    ক্ষেত্র 't':			/* Horizontal tab. */
      putchar ('\t');
      বিরতি;
    ক্ষেত্র 'v':			/* Vertical tab. */
      putchar ('\v');
      বিরতি;
    পূর্বনির্ধারিত:
      putchar (c);
      বিরতি;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

স্থির পূর্ণসংখ্যা
print_esc (অক্ষর ধ্রুবক *escstart, bool octal_0)
{
  অক্ষর ধ্রুবক *p = escstart + 1;
  পূর্ণসংখ্যা esc_মান = 0;		/* Value of \nnn escape. */
  পূর্ণসংখ্যা esc_দৈর্ঘ্য;		/* Length of \nnn escape. */

  যদি (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      জন্য (esc_দৈর্ঘ্য = 0, ++p;
           esc_দৈর্ঘ্য < 2 && c_isxdigit (*p);
           ++esc_দৈর্ঘ্য, ++p)
        esc_মান = esc_মান * 16 + fromhex (*p);
      যদি (esc_দৈর্ঘ্য == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      putchar (esc_মান);
    }
  অন্যথা যদি (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      জন্য (esc_দৈর্ঘ্য = 0, p += octal_0 && *p == '0';
           esc_দৈর্ঘ্য < 3 && isoct (*p);
           ++esc_দৈর্ঘ্য, ++p)
        esc_মান = esc_মান * 8 + fromoct (*p);
      putchar (esc_মান);
    }
  অন্যথা যদি (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  অন্যথা যদি (*p == 'u' || *p == 'U')
    {
      অক্ষর esc_char = *p;
      unsigned পূর্ণসংখ্যা uni_মান;

      uni_মান = 0;
      জন্য (esc_দৈর্ঘ্য = (esc_char == 'u' ? 4 : 8), ++p;
           esc_দৈর্ঘ্য > 0;
           --esc_দৈর্ঘ্য, ++p)
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
  ফেরত p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

স্থির শূন্য
print_esc_string (অক্ষর ধ্রুবক *str)
{
  জন্য (; *str; str++)
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
print_direc (অক্ষর ধ্রুবক *start, অক্ষর conversion,
             bool have_field_width, পূর্ণসংখ্যা field_width,
             bool have_precision, পূর্ণসংখ্যা precision,
             অক্ষর ধ্রুবক *argument)
{
  অক্ষর *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    অক্ষর *q;
    অক্ষর ধ্রুবক *দৈর্ঘ্য_modifier;
    size_t দৈর্ঘ্য_modifier_len;

    নির্বাচন (conversion)
      {
      ক্ষেত্র 'd': ক্ষেত্র 'i': ক্ষেত্র 'o': ক্ষেত্র 'u': ক্ষেত্র 'x': ক্ষেত্র 'X':
        দৈর্ঘ্য_modifier = "j";
        দৈর্ঘ্য_modifier_len = 1;
        বিরতি;

      ক্ষেত্র 'a': ক্ষেত্র 'e': ক্ষেত্র 'f': ক্ষেত্র 'g':
      ক্ষেত্র 'A': ক্ষেত্র 'E': ক্ষেত্র 'F': ক্ষেত্র 'G':
        দৈর্ঘ্য_modifier = "L";
        দৈর্ঘ্য_modifier_len = 1;
        বিরতি;

      পূর্বনির্ধারিত:
        দৈর্ঘ্য_modifier = start;  /* Any valid pointer will do.  */
        দৈর্ঘ্য_modifier_len = 0;
        বিরতি;
      }

    size_t দৈর্ঘ্য = strlen (start);
    p = xmalloc (দৈর্ঘ্য + দৈর্ঘ্য_modifier_len + 2);
    q = mempcpy (p, start, দৈর্ঘ্য);
    q = mempcpy (q, দৈর্ঘ্য_modifier, দৈর্ঘ্য_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  নির্বাচন (conversion)
    {
    ক্ষেত্র 'd':
    ক্ষেত্র 'i':
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
      বিরতি;

    ক্ষেত্র 'o':
    ক্ষেত্র 'u':
    ক্ষেত্র 'x':
    ক্ষেত্র 'X':
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
      বিরতি;

    ক্ষেত্র 'a':
    ক্ষেত্র 'A':
    ক্ষেত্র 'e':
    ক্ষেত্র 'E':
    ক্ষেত্র 'f':
    ক্ষেত্র 'F':
    ক্ষেত্র 'g':
    ক্ষেত্র 'G':
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
      বিরতি;

    ক্ষেত্র 'c':
      {
        অক্ষর c = argument ? *argument : '\0';
        যদি (!have_field_width)
          xprintf (p, c);
        অন্যথা
          xprintf (p, field_width, c);
      }
      বিরতি;

    ক্ষেত্র 's':
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
      বিরতি;
    }

  মুক্ত (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

কাঠামো arg_cursor
{
  অক্ষর ধ্রুবক *f;	/* Pointer into 'format'.  */
  পূর্ণসংখ্যা curr_arg;		/* Current offset.  */
  পূর্ণসংখ্যা curr_s_arg;	/* Current sequential offset.  */
  পূর্ণসংখ্যা end_arg;		/* End arg processed.  */
  পূর্ণসংখ্যা direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_PURE স্থির কাঠামো arg_cursor
get_curr_arg (পূর্ণসংখ্যা pos, কাঠামো arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  পূর্ণসংখ্যা arg = 0;
  অক্ষর ধ্রুবক *f = ac.f;
  যদি (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      পূর্ণসংখ্যা a = *f++ - '0';
      জন্য (; c_isdigit (*f); f++)
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
  ফেরত ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

স্থির পূর্ণসংখ্যা
print_formatted (অক্ষর ধ্রুবক *format, পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  কাঠামো arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.end_arg = ac.direc_arg = -1;
  অক্ষর ধ্রুবক *direc_start;	/* Start of % directive.  */
  অক্ষর *direc;			/* Generated % directive.  */
  অক্ষর *pdirec;			/* Pointer to current end of directive.  */
  bool have_field_width;	/* True if FIELD_WIDTH is valid.  */
  পূর্ণসংখ্যা field_width = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  পূর্ণসংখ্যা precision = 0;		/* Arg to second '*'.  */
  অক্ষর ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (strlen (format) + 1);

  জন্য (ac.f = format; *ac.f; ac.f++)
    {
      নির্বাচন (*ac.f)
        {
        ক্ষেত্র '%':
          direc_start = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_field_width = have_precision = false;
          যদি (*ac.f == '%')
            {
              putchar ('%');
              বিরতি;
            }

          ac = get_curr_arg (0, ac);

          যদি (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              যদি (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              বিরতি;
            }

          যদি (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              যদি (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              বিরতি;
            }

          memset (ok, 0, আকার ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          জন্য (;; ac.f++)
            {
              নির্বাচন (*ac.f)
                {
#যদি (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                ক্ষেত্র 'I':
#endif
                ক্ষেত্র '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  বিরতি;
                ক্ষেত্র '-': ক্ষেত্র '+': ক্ষেত্র ' ':
                  বিরতি;
                ক্ষেত্র '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  বিরতি;
                ক্ষেত্র '0':
                  ok['c'] = ok['s'] = 0;
                  বিরতি;
                পূর্বনির্ধারিত:
                  যাও no_more_flag_characters;
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
            যখন (c_isdigit (*ac.f))
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
                যখন (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          যখন (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            unsigned অক্ষর conversion = *ac.f;
            পূর্ণসংখ্যা speclen = MIN (ac.f + 1 - direc_start, INT_MAX);
            যদি (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_start);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_field_width, field_width,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : ফাঁকা);

          বিরতি;

        ক্ষেত্র '\\':
          ac.f += print_esc (ac.f, false);
          বিরতি;

        পূর্বনির্ধারিত:
          putchar (*ac.f);
        }
    }

  মুক্ত (direc);
  ফেরত MIN (argc, ac.end_arg + 1);
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  অক্ষর *format;
  পূর্ণসংখ্যা args_used;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ফাঁকা);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  যদি (argc == 2)
    {
      যদি (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      যদি (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (অক্ষর *) ফাঁকা);
          ফেরত EXIT_SUCCESS;
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

  করো
    {
      args_used = print_formatted (format, argc, argv);
      argc -= args_used;
      argv += args_used;
    }
  যখন (args_used > 0 && argc > 0);

  যদি (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  ফেরত exit_status;
}
