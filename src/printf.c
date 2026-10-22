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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <wchar.h>

#ضمن "system.h"
#ضمن "c-ctype.h"
#ضمن "cl-strtod.h"
#ضمن "octhexdigits.h"
#ضمن "quote.h"
#ضمن "unicodeio.h"
#ضمن "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "printf"

#تعریف_کن AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
ساكن صحيح exit_وضعیت;

/* True if the POSIXLY_CORRECT environment variable is set.  */
ساكن bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
ساكن حرف ثابت *ثابت cfcc_پیام =
 N_("warning: %s: character(s) following character constant have been ignored");

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s FORMAT [آرگومان_2]...\n\
  or:  %s OPTION\n\
"),
              برنامه_name, برنامه_name);
      fputs (_("\
Print آرگومان_2(s) according مقصد FORMAT, or execute according مقصد OPTION:\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FORMAT controls the output as درون C چاپ_کن.  Interpreted sequences are:\n\
\n\
  \\\"      مزدوج quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new سطر\n\
  \\r      carriage أرجع\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal مقدار NNN (1 مقصد 3 digits)\n\
  \\xHH    byte with hexadecimal مقدار HH (1 مقصد 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex مقدار HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex مقدار HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      آرگومان_2 as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      آرگومان_2 is printed درون a format that can be reused as shell درون_داد,\n\
          escaping non-printable characters with the POSIX $'' syntax\
\n\n\
and all C چاپ_کن(3) format specifications ending with one از diouxXfaAeEgGcs,\n\
with ARGUMENTs converted مقصد proper type اولی.\n\
"), stdout);
      fputs (_("\
FORMAT is reused مقصد convert all specified arguments.\n\
"), stdout);
      fputs (_("\
The following جدول shows supported conversion specifiers,\n\
with examples including usage از عرض, precision, and flags.\n\
\n\
  CONVERSION  Shell format example        Output\n\
  %s          '[%10s]' 'left pad'         [  چپ pad]\n\
  %s          '[%-10s]' 'right pad'       [راست pad ]\n\
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
      چاپ_کن (USAGE_BUILTIN_WARNING, برنامه_NAME);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

ساكن فراغ
verify_numeric (حرف ثابت *s, حرف ثابت *انتها)
{
  إذا (s == انتها)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_وضعیت = EXIT_FAILURE;
    }
  وإلا إذا (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_وضعیت = EXIT_FAILURE;
    }
  وإلا إذا (*انتها)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_وضعیت = EXIT_FAILURE;
    }
}

#تعریف_کن STRTOX(TYPE, FUNC_NAME, LIB_FUNC_عبارت)				 \
ساكن TYPE								 \
FUNC_NAME (حرف ثابت *s)						 \
{									 \
  حرف *انتها;								 \
  TYPE val;								 \
                                                                         \
  إذا ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      بیعلامت حرف ch = *++s;						 \
      val = ch;								 \
                                                                         \
      إذا (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = طول_رشته (s);					 \
          ssize_t بایتها;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          بایتها = mbrtowc (&wc, s, slen, &mbstate);			 \
          إذا (0 < بایتها)						 \
            {								 \
              val = wc;							 \
              s += بایتها - 1;						 \
            }								 \
        }								 \
                                                                         \
      /* If POSIXLY_CORRECT is not set, then give a warning that there	 \
         are characters following the character constant and that GNU	 \
         printf is ignoring those characters.  If POSIXLY_CORRECT *is*	 \
         set, then don't give the warning.  */				 \
      إذا (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_پیام), s);					 \
    }									 \
  وإلا									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_عبارت);						 \
      verify_numeric (s, انتها);						 \
    }									 \
  أرجع val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &انتها, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &انتها, 0))
STRTOX (دراز مزدوج, vstrtold,   cl_strtold (s, &انتها))

/* Output a single-character \ escape.  */

ساكن فراغ
print_esc_char (حرف c)
{
  بدل (c)
    {
    حالة 'a':			/* Alert. */
      حرف_چاپ_کن ('\a');
      اكسر;
    حالة 'b':			/* Backspace. */
      حرف_چاپ_کن ('\b');
      اكسر;
    حالة 'c':			/* Cancel the rest of the output. */
      خروج (EXIT_SUCCESS);
      اكسر;
    حالة 'e':			/* Escape. */
      حرف_چاپ_کن ('\x1B');
      اكسر;
    حالة 'f':			/* Form feed. */
      حرف_چاپ_کن ('\f');
      اكسر;
    حالة 'n':			/* New line. */
      حرف_چاپ_کن ('\n');
      اكسر;
    حالة 'r':			/* Carriage return. */
      حرف_چاپ_کن ('\r');
      اكسر;
    حالة 't':			/* Horizontal tab. */
      حرف_چاپ_کن ('\t');
      اكسر;
    حالة 'v':			/* Vertical tab. */
      حرف_چاپ_کن ('\v');
      اكسر;
    افتراضي:
      حرف_چاپ_کن (c);
      اكسر;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

ساكن صحيح
print_esc (حرف ثابت *escstart, bool octal_0)
{
  حرف ثابت *p = escstart + 1;
  صحيح esc_مقدار = 0;		/* Value of \nnn escape. */
  صحيح esc_درازی;		/* Length of \nnn escape. */

  إذا (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      لكل (esc_درازی = 0, ++p;
           esc_درازی < 2 && c_isxdigit (*p);
           ++esc_درازی, ++p)
        esc_مقدار = esc_مقدار * 16 + fromhex (*p);
      إذا (esc_درازی == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      حرف_چاپ_کن (esc_مقدار);
    }
  وإلا إذا (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      لكل (esc_درازی = 0, p += octal_0 && *p == '0';
           esc_درازی < 3 && isoct (*p);
           ++esc_درازی, ++p)
        esc_مقدار = esc_مقدار * 8 + fromoct (*p);
      حرف_چاپ_کن (esc_مقدار);
    }
  وإلا إذا (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  وإلا إذا (*p == 'u' || *p == 'U')
    {
      حرف esc_char = *p;
      بیعلامت صحيح uni_مقدار;

      uni_مقدار = 0;
      لكل (esc_درازی = (esc_char == 'u' ? 4 : 8), ++p;
           esc_درازی > 0;
           --esc_درازی, ++p)
        {
          إذا (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_مقدار = uni_مقدار * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      إذا (uni_مقدار >= 0xd800 && uni_مقدار <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_مقدار);

      print_unicode_char (stdout, uni_مقدار, 0);
    }
  وإلا
    {
      حرف_چاپ_کن ('\\');
      إذا (*p)
        {
          حرف_چاپ_کن (*p);
          p++;
        }
    }
  أرجع p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

ساكن فراغ
print_esc_string (حرف ثابت *str)
{
  لكل (; *str; str++)
    إذا (*str == '\\')
      str += print_esc (str, true);
    وإلا
      حرف_چاپ_کن (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

ساكن فراغ
print_direc (حرف ثابت *شروع, حرف conversion,
             bool have_فیلد_عرض, صحيح فیلد_عرض,
             bool have_precision, صحيح precision,
             حرف ثابت *آرگومان)
{
  حرف *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    حرف *q;
    حرف ثابت *درازی_modifier;
    size_t درازی_modifier_len;

    بدل (conversion)
      {
      حالة 'd': حالة 'i': حالة 'o': حالة 'u': حالة 'x': حالة 'X':
        درازی_modifier = "j";
        درازی_modifier_len = 1;
        اكسر;

      حالة 'a': حالة 'e': حالة 'f': حالة 'g':
      حالة 'A': حالة 'E': حالة 'F': حالة 'G':
        درازی_modifier = "L";
        درازی_modifier_len = 1;
        اكسر;

      افتراضي:
        درازی_modifier = شروع;  /* Any valid pointer will do.  */
        درازی_modifier_len = 0;
        اكسر;
      }

    size_t درازی = طول_رشته (شروع);
    p = xmalloc (درازی + درازی_modifier_len + 2);
    q = mempcpy (p, شروع, درازی);
    q = mempcpy (q, درازی_modifier, درازی_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  بدل (conversion)
    {
    حالة 'd':
    حالة 'i':
      {
        intmax_t arg = آرگومان ? vstrtoimax (آرگومان) : 0;
        إذا (!have_فیلد_عرض)
          {
            إذا (!have_precision)
              xprintf (p, arg);
            وإلا
              xprintf (p, precision, arg);
          }
        وإلا
          {
            إذا (!have_precision)
              xprintf (p, فیلد_عرض, arg);
            وإلا
              xprintf (p, فیلد_عرض, precision, arg);
          }
      }
      اكسر;

    حالة 'o':
    حالة 'u':
    حالة 'x':
    حالة 'X':
      {
        uintmax_t arg = آرگومان ? vstrtoumax (آرگومان) : 0;
        إذا (!have_فیلد_عرض)
          {
            إذا (!have_precision)
              xprintf (p, arg);
            وإلا
              xprintf (p, precision, arg);
          }
        وإلا
          {
            إذا (!have_precision)
              xprintf (p, فیلد_عرض, arg);
            وإلا
              xprintf (p, فیلد_عرض, precision, arg);
          }
      }
      اكسر;

    حالة 'a':
    حالة 'A':
    حالة 'e':
    حالة 'E':
    حالة 'f':
    حالة 'F':
    حالة 'g':
    حالة 'G':
      {
        دراز مزدوج arg = آرگومان ? vstrtold (آرگومان) : 0;
        إذا (!have_فیلد_عرض)
          {
            إذا (!have_precision)
              xprintf (p, arg);
            وإلا
              xprintf (p, precision, arg);
          }
        وإلا
          {
            إذا (!have_precision)
              xprintf (p, فیلد_عرض, arg);
            وإلا
              xprintf (p, فیلد_عرض, precision, arg);
          }
      }
      اكسر;

    حالة 'c':
      {
        حرف c = آرگومان ? *آرگومان : '\0';
        إذا (!have_فیلد_عرض)
          xprintf (p, c);
        وإلا
          xprintf (p, فیلد_عرض, c);
      }
      اكسر;

    حالة 's':
      إذا (!آرگومان)
        آرگومان = "";
      إذا (!have_فیلد_عرض)
        {
          إذا (!have_precision)
            xprintf (p, آرگومان);
          وإلا
            xprintf (p, precision, آرگومان);
        }
      وإلا
        {
          إذا (!have_precision)
            xprintf (p, فیلد_عرض, آرگومان);
          وإلا
            xprintf (p, فیلد_عرض, precision, آرگومان);
        }
      اكسر;
    }

  حرر (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

بنية arg_cursor
{
  حرف ثابت *f;	/* Pointer into 'format'.  */
  صحيح curr_arg;		/* Current offset.  */
  صحيح curr_s_arg;	/* Current sequential offset.  */
  صحيح انتها_arg;		/* End arg processed.  */
  صحيح direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_خالص ساكن بنية arg_cursor
get_curr_arg (صحيح pos, بنية arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  صحيح arg = 0;
  حرف ثابت *f = ac.f;
  إذا (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      صحيح a = *f++ - '0';
      لكل (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_جمع_کن (&a, a, *f - '0');
        }
      إذا (*f == '$')
        arg = v ? بیشترین_صحیح : a;
    }

  إذا (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      إذا (pos == 0)
        ac.direc_arg = arg;
    }
  وإلا
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  إذا (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.انتها_arg = MAX (ac.انتها_arg, arg);
    }
  أرجع ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

ساكن صحيح
print_formatted (حرف ثابت *format, صحيح argc, حرف **argv)
{
  بنية arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.انتها_arg = ac.direc_arg = -1;
  حرف ثابت *direc_شروع;	/* Start of % directive.  */
  حرف *direc;			/* Generated % directive.  */
  حرف *pdirec;			/* Pointer to current end of directive.  */
  bool have_فیلد_عرض;	/* True if FIELD_WIDTH is valid.  */
  صحيح فیلد_عرض = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  صحيح precision = 0;		/* Arg to second '*'.  */
  حرف ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (طول_رشته (format) + 1);

  لكل (ac.f = format; *ac.f; ac.f++)
    {
      بدل (*ac.f)
        {
        حالة '%':
          direc_شروع = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_فیلد_عرض = have_precision = false;
          إذا (*ac.f == '%')
            {
              حرف_چاپ_کن ('%');
              اكسر;
            }

          ac = get_curr_arg (0, ac);

          إذا (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              إذا (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              اكسر;
            }

          إذا (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              إذا (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              اكسر;
            }

          memset (ok, 0, حجم ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          لكل (;; ac.f++)
            {
              بدل (*ac.f)
                {
#إذا (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                حالة 'I':
#پایان_اگر
                حالة '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  اكسر;
                حالة '-': حالة '+': حالة ' ':
                  اكسر;
                حالة '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  اكسر;
                حالة '0':
                  ok['c'] = ok['s'] = 0;
                  اكسر;
                افتراضي:
                  اذهب no_بیشتر_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_بیشتر_flag_characters:

          إذا (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              إذا (ac.curr_arg < argc)
                {
                  intmax_t عرض = vstrtoimax (argv[ac.curr_arg]);
                  إذا (INT_MIN <= عرض && عرض <= بیشترین_صحیح)
                    فیلد_عرض = عرض;
                  وإلا
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              وإلا
                فیلد_عرض = 0;
              have_فیلد_عرض = true;
            }
          وإلا
            طالما (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          إذا (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              إذا (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  إذا (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      إذا (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      وإلا إذا (بیشترین_صحیح < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      وإلا
                        precision = prec;
                    }
                  وإلا
                    precision = 0;
                  have_precision = true;
                }
              وإلا
                طالما (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          طالما (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            بیعلامت حرف conversion = *ac.f;
            صحيح speclen = MIN (ac.f + 1 - direc_شروع, بیشترین_صحیح);
            إذا (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_شروع);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_فیلد_عرض, فیلد_عرض,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : عدم);

          اكسر;

        حالة '\\':
          ac.f += print_esc (ac.f, false);
          اكسر;

        افتراضي:
          حرف_چاپ_کن (*ac.f);
        }
    }

  حرر (direc);
  أرجع MIN (argc, ac.انتها_arg + 1);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  حرف *format;
  صحيح آرگومانها_استفاده_شده;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  إذا (argc == 2)
    {
      إذا (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      إذا (streq (argv[1], "--version"))
        {
          نسخه_etc (stdout, برنامه_NAME, PACKAGE_NAME, نسخه, AUTHORS,
                       (حرف *) عدم);
          أرجع EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  إذا (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  إذا (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  افعل
    {
      آرگومانها_استفاده_شده = print_formatted (format, argc, argv);
      argc -= آرگومانها_استفاده_شده;
      argv += آرگومانها_استفاده_شده;
    }
  طالما (آرگومانها_استفاده_شده > 0 && argc > 0);

  إذا (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  أرجع exit_وضعیت;
}
