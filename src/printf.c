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
#وصف_ڪريو پروگرام_NAME "printf"

#وصف_ڪريو AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
ساكن صحيح exit_حالت;

/* True if the POSIXLY_CORRECT environment variable is set.  */
ساكن bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
ساكن حرف ثابت *ثابت cfcc_نياپو =
 N_("warning: %s: character(s) following character constant have been ignored");

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s FORMAT [دليل_2]...\n\
  or:  %s OPTION\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Print دليل_2(s) according منزل FORMAT, or execute according منزل OPTION:\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FORMAT controls the output as اندريون C لکو.  Interpreted sequences are:\n\
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
  \\n      new سٽ\n\
  \\r      carriage أرجع\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal قدر NNN (1 منزل 3 digits)\n\
  \\xHH    byte with hexadecimal قدر HH (1 منزل 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex قدر HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex قدر HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      دليل_2 as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      دليل_2 is printed اندريون a format that can be reused as shell داخلا,\n\
          escaping non-printable characters with the POSIX $'' syntax\
\n\n\
and all C لکو(3) format specifications ending with one جو diouxXfaAeEgGcs,\n\
with ARGUMENTs converted منزل proper type پهريون.\n\
"), stdout);
      fputs (_("\
FORMAT is reused منزل convert all specified arguments.\n\
"), stdout);
      fputs (_("\
The following جدول shows supported conversion specifiers,\n\
with examples including usage جو ويڪر, precision, and flags.\n\
\n\
  CONVERSION  Shell format example        Output\n\
  %s          '[%10s]' 'left pad'         [  کاٻو pad]\n\
  %s          '[%-10s]' 'right pad'       [ساڄو pad ]\n\
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
      لکو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

ساكن فراغ
verify_numeric (حرف ثابت *s, حرف ثابت *پڇاڙي)
{
  إذا (s == پڇاڙي)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_حالت = EXIT_FAILURE;
    }
  وإلا إذا (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_حالت = EXIT_FAILURE;
    }
  وإلا إذا (*پڇاڙي)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_حالت = EXIT_FAILURE;
    }
}

#وصف_ڪريو STRTOX(TYPE, FUNC_NAME, LIB_FUNC_اظهار)				 \
ساكن TYPE								 \
FUNC_NAME (حرف ثابت *s)						 \
{									 \
  حرف *پڇاڙي;								 \
  TYPE val;								 \
                                                                         \
  إذا ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      بنانشان حرف ch = *++s;						 \
      val = ch;								 \
                                                                         \
      إذا (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = لڙي_ڊيگهه (s);					 \
          ssize_t بائيٽون;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          بائيٽون = mbrtowc (&wc, s, slen, &mbstate);			 \
          إذا (0 < بائيٽون)						 \
            {								 \
              val = wc;							 \
              s += بائيٽون - 1;						 \
            }								 \
        }								 \
                                                                         \
      /* If POSIXLY_CORRECT is not set, then give a warning that there	 \
         are characters following the character constant and that GNU	 \
         printf is ignoring those characters.  If POSIXLY_CORRECT *is*	 \
         set, then don't give the warning.  */				 \
      إذا (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_نياپو), s);					 \
    }									 \
  وإلا									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_اظهار);						 \
      verify_numeric (s, پڇاڙي);						 \
    }									 \
  أرجع val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &پڇاڙي, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &پڇاڙي, 0))
STRTOX (ڊگهو مزدوج, vstrtold,   cl_strtold (s, &پڇاڙي))

/* Output a single-character \ escape.  */

ساكن فراغ
print_esc_char (حرف c)
{
  بدل (c)
    {
    حالة 'a':			/* Alert. */
      اکر_لکو ('\a');
      اكسر;
    حالة 'b':			/* Backspace. */
      اکر_لکو ('\b');
      اكسر;
    حالة 'c':			/* Cancel the rest of the output. */
      ٻاهر_نڪرو (EXIT_SUCCESS);
      اكسر;
    حالة 'e':			/* Escape. */
      اکر_لکو ('\x1B');
      اكسر;
    حالة 'f':			/* Form feed. */
      اکر_لکو ('\f');
      اكسر;
    حالة 'n':			/* New line. */
      اکر_لکو ('\n');
      اكسر;
    حالة 'r':			/* Carriage return. */
      اکر_لکو ('\r');
      اكسر;
    حالة 't':			/* Horizontal tab. */
      اکر_لکو ('\t');
      اكسر;
    حالة 'v':			/* Vertical tab. */
      اکر_لکو ('\v');
      اكسر;
    افتراضي:
      اکر_لکو (c);
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
  صحيح esc_قدر = 0;		/* Value of \nnn escape. */
  صحيح esc_ڊيگهائي;		/* Length of \nnn escape. */

  إذا (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      لكل (esc_ڊيگهائي = 0, ++p;
           esc_ڊيگهائي < 2 && c_isxdigit (*p);
           ++esc_ڊيگهائي, ++p)
        esc_قدر = esc_قدر * 16 + fromhex (*p);
      إذا (esc_ڊيگهائي == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      اکر_لکو (esc_قدر);
    }
  وإلا إذا (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      لكل (esc_ڊيگهائي = 0, p += octal_0 && *p == '0';
           esc_ڊيگهائي < 3 && isoct (*p);
           ++esc_ڊيگهائي, ++p)
        esc_قدر = esc_قدر * 8 + fromoct (*p);
      اکر_لکو (esc_قدر);
    }
  وإلا إذا (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  وإلا إذا (*p == 'u' || *p == 'U')
    {
      حرف esc_char = *p;
      بنانشان صحيح uni_قدر;

      uni_قدر = 0;
      لكل (esc_ڊيگهائي = (esc_char == 'u' ? 4 : 8), ++p;
           esc_ڊيگهائي > 0;
           --esc_ڊيگهائي, ++p)
        {
          إذا (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_قدر = uni_قدر * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      إذا (uni_قدر >= 0xd800 && uni_قدر <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_قدر);

      print_unicode_char (stdout, uni_قدر, 0);
    }
  وإلا
    {
      اکر_لکو ('\\');
      إذا (*p)
        {
          اکر_لکو (*p);
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
      اکر_لکو (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

ساكن فراغ
print_direc (حرف ثابت *شروعات, حرف conversion,
             bool have_ميدان_ويڪر, صحيح ميدان_ويڪر,
             bool have_precision, صحيح precision,
             حرف ثابت *دليل)
{
  حرف *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    حرف *q;
    حرف ثابت *ڊيگهائي_modifier;
    size_t ڊيگهائي_modifier_len;

    بدل (conversion)
      {
      حالة 'd': حالة 'i': حالة 'o': حالة 'u': حالة 'x': حالة 'X':
        ڊيگهائي_modifier = "j";
        ڊيگهائي_modifier_len = 1;
        اكسر;

      حالة 'a': حالة 'e': حالة 'f': حالة 'g':
      حالة 'A': حالة 'E': حالة 'F': حالة 'G':
        ڊيگهائي_modifier = "L";
        ڊيگهائي_modifier_len = 1;
        اكسر;

      افتراضي:
        ڊيگهائي_modifier = شروعات;  /* Any valid pointer will do.  */
        ڊيگهائي_modifier_len = 0;
        اكسر;
      }

    size_t ڊيگهائي = لڙي_ڊيگهه (شروعات);
    p = xmalloc (ڊيگهائي + ڊيگهائي_modifier_len + 2);
    q = mempcpy (p, شروعات, ڊيگهائي);
    q = mempcpy (q, ڊيگهائي_modifier, ڊيگهائي_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  بدل (conversion)
    {
    حالة 'd':
    حالة 'i':
      {
        intmax_t arg = دليل ? vstrtoimax (دليل) : 0;
        إذا (!have_ميدان_ويڪر)
          {
            إذا (!have_precision)
              xprintf (p, arg);
            وإلا
              xprintf (p, precision, arg);
          }
        وإلا
          {
            إذا (!have_precision)
              xprintf (p, ميدان_ويڪر, arg);
            وإلا
              xprintf (p, ميدان_ويڪر, precision, arg);
          }
      }
      اكسر;

    حالة 'o':
    حالة 'u':
    حالة 'x':
    حالة 'X':
      {
        uintmax_t arg = دليل ? vstrtoumax (دليل) : 0;
        إذا (!have_ميدان_ويڪر)
          {
            إذا (!have_precision)
              xprintf (p, arg);
            وإلا
              xprintf (p, precision, arg);
          }
        وإلا
          {
            إذا (!have_precision)
              xprintf (p, ميدان_ويڪر, arg);
            وإلا
              xprintf (p, ميدان_ويڪر, precision, arg);
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
        ڊگهو مزدوج arg = دليل ? vstrtold (دليل) : 0;
        إذا (!have_ميدان_ويڪر)
          {
            إذا (!have_precision)
              xprintf (p, arg);
            وإلا
              xprintf (p, precision, arg);
          }
        وإلا
          {
            إذا (!have_precision)
              xprintf (p, ميدان_ويڪر, arg);
            وإلا
              xprintf (p, ميدان_ويڪر, precision, arg);
          }
      }
      اكسر;

    حالة 'c':
      {
        حرف c = دليل ? *دليل : '\0';
        إذا (!have_ميدان_ويڪر)
          xprintf (p, c);
        وإلا
          xprintf (p, ميدان_ويڪر, c);
      }
      اكسر;

    حالة 's':
      إذا (!دليل)
        دليل = "";
      إذا (!have_ميدان_ويڪر)
        {
          إذا (!have_precision)
            xprintf (p, دليل);
          وإلا
            xprintf (p, precision, دليل);
        }
      وإلا
        {
          إذا (!have_precision)
            xprintf (p, ميدان_ويڪر, دليل);
          وإلا
            xprintf (p, ميدان_ويڪر, precision, دليل);
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
  صحيح پڇاڙي_arg;		/* End arg processed.  */
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
          v |= ckd_جوڙيو (&a, a, *f - '0');
        }
      إذا (*f == '$')
        arg = v ? وڌ_کان_وڌ_عدد : a;
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
      ac.پڇاڙي_arg = MAX (ac.پڇاڙي_arg, arg);
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
  ac.curr_arg = ac.curr_s_arg = ac.پڇاڙي_arg = ac.direc_arg = -1;
  حرف ثابت *direc_شروعات;	/* Start of % directive.  */
  حرف *direc;			/* Generated % directive.  */
  حرف *pdirec;			/* Pointer to current end of directive.  */
  bool have_ميدان_ويڪر;	/* True if FIELD_WIDTH is valid.  */
  صحيح ميدان_ويڪر = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  صحيح precision = 0;		/* Arg to second '*'.  */
  حرف ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (لڙي_ڊيگهه (format) + 1);

  لكل (ac.f = format; *ac.f; ac.f++)
    {
      بدل (*ac.f)
        {
        حالة '%':
          direc_شروعات = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_ميدان_ويڪر = have_precision = false;
          إذا (*ac.f == '%')
            {
              اکر_لکو ('%');
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
#جيڪڏهن_پورو
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
                  اذهب no_وڌيڪ_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_وڌيڪ_flag_characters:

          إذا (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              إذا (ac.curr_arg < argc)
                {
                  intmax_t ويڪر = vstrtoimax (argv[ac.curr_arg]);
                  إذا (INT_MIN <= ويڪر && ويڪر <= وڌ_کان_وڌ_عدد)
                    ميدان_ويڪر = ويڪر;
                  وإلا
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              وإلا
                ميدان_ويڪر = 0;
              have_ميدان_ويڪر = true;
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
                      وإلا إذا (وڌ_کان_وڌ_عدد < prec)
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
            بنانشان حرف conversion = *ac.f;
            صحيح speclen = MIN (ac.f + 1 - direc_شروعات, وڌ_کان_وڌ_عدد);
            إذا (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_شروعات);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_ميدان_ويڪر, ميدان_ويڪر,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : عدم);

          اكسر;

        حالة '\\':
          ac.f += print_esc (ac.f, false);
          اكسر;

        افتراضي:
          اکر_لکو (*ac.f);
        }
    }

  حرر (direc);
  أرجع MIN (argc, ac.پڇاڙي_arg + 1);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  حرف *format;
  صحيح دليلون_استعمال_ٿيل;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
          نسخو_etc (stdout, پروگرام_NAME, PACKAGE_NAME, نسخو, AUTHORS,
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
      دليلون_استعمال_ٿيل = print_formatted (format, argc, argv);
      argc -= دليلون_استعمال_ٿيل;
      argv += دليلون_استعمال_ٿيل;
    }
  طالما (دليلون_استعمال_ٿيل > 0 && argc > 0);

  إذا (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  أرجع exit_حالت;
}
