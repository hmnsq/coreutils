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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <wchar.h>

#شامل "system.h"
#شامل "c-ctype.h"
#شامل "cl-strtod.h"
#شامل "octhexdigits.h"
#شامل "quote.h"
#شامل "unicodeio.h"
#شامل "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "printf"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
ساکن صحیح exit_حالت;

/* True if the POSIXLY_CORRECT environment variable is set.  */
ساکن bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
ساکن حرف ثابت *ثابت cfcc_پیغام =
 N_("warning: %s: character(s) following character constant have been ignored");

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s FORMAT [دلیل_2]...\n\
  or:  %s OPTION\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Print دلیل_2(s) according مقصد FORMAT, or execute according مقصد OPTION:\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FORMAT controls the output as اندر C لکھو.  Interpreted sequences are:\n\
\n\
  \\\"      دوہرا quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new سطر\n\
  \\r      carriage واپس\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal قدر NNN (1 مقصد 3 digits)\n\
  \\xHH    byte with hexadecimal قدر HH (1 مقصد 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex قدر HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex قدر HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      دلیل_2 as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      دلیل_2 is printed اندر a format that can be reused as shell داخلہ,\n\
          escaping non-printable characters with the POSIX $'' syntax\
\n\n\
and all C لکھو(3) format specifications ending with one کا diouxXfaAeEgGcs,\n\
with ARGUMENTs converted مقصد proper type پہلا.\n\
"), stdout);
      fputs (_("\
FORMAT is reused مقصد convert all specified arguments.\n\
"), stdout);
      fputs (_("\
The following جدول shows supported conversion specifiers,\n\
with examples including usage کا چوڑائی, precision, and flags.\n\
\n\
  CONVERSION  Shell format example        Output\n\
  %s          '[%10s]' 'left pad'         [  بایاں pad]\n\
  %s          '[%-10s]' 'right pad'       [دایاں pad ]\n\
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
      لکھو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

ساکن خالی
verify_numeric (حرف ثابت *s, حرف ثابت *انتہا)
{
  اگر (s == انتہا)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_حالت = EXIT_FAILURE;
    }
  ورنہ اگر (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_حالت = EXIT_FAILURE;
    }
  ورنہ اگر (*انتہا)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_حالت = EXIT_FAILURE;
    }
}

#تعریف_کرو STRTOX(TYPE, FUNC_NAME, LIB_FUNC_اظہاریہ)				 \
ساکن TYPE								 \
FUNC_NAME (حرف ثابت *s)						 \
{									 \
  حرف *انتہا;								 \
  TYPE val;								 \
                                                                         \
  اگر ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      غیرعلامتی حرف ch = *++s;						 \
      val = ch;								 \
                                                                         \
      اگر (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = لڑی_لمبائی (s);					 \
          ssize_t بائٹیں;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          بائٹیں = mbrtowc (&wc, s, slen, &mbstate);			 \
          اگر (0 < بائٹیں)						 \
            {								 \
              val = wc;							 \
              s += بائٹیں - 1;						 \
            }								 \
        }								 \
                                                                         \
      /* If POSIXLY_CORRECT is not set, then give a warning that there	 \
         are characters following the character constant and that GNU	 \
         printf is ignoring those characters.  If POSIXLY_CORRECT *is*	 \
         set, then don't give the warning.  */				 \
      اگر (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_پیغام), s);					 \
    }									 \
  ورنہ									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_اظہاریہ);						 \
      verify_numeric (s, انتہا);						 \
    }									 \
  واپس val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &انتہا, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &انتہا, 0))
STRTOX (طویل دوہرا, vstrtold,   cl_strtold (s, &انتہا))

/* Output a single-character \ escape.  */

ساکن خالی
print_esc_char (حرف c)
{
  انتخاب (c)
    {
    حالت 'a':			/* Alert. */
      حرف_لکھو ('\a');
      توڑو;
    حالت 'b':			/* Backspace. */
      حرف_لکھو ('\b');
      توڑو;
    حالت 'c':			/* Cancel the rest of the output. */
      باہر_نکلو (EXIT_SUCCESS);
      توڑو;
    حالت 'e':			/* Escape. */
      حرف_لکھو ('\x1B');
      توڑو;
    حالت 'f':			/* Form feed. */
      حرف_لکھو ('\f');
      توڑو;
    حالت 'n':			/* New line. */
      حرف_لکھو ('\n');
      توڑو;
    حالت 'r':			/* Carriage return. */
      حرف_لکھو ('\r');
      توڑو;
    حالت 't':			/* Horizontal tab. */
      حرف_لکھو ('\t');
      توڑو;
    حالت 'v':			/* Vertical tab. */
      حرف_لکھو ('\v');
      توڑو;
    طےشدہ:
      حرف_لکھو (c);
      توڑو;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

ساکن صحیح
print_esc (حرف ثابت *escstart, bool octal_0)
{
  حرف ثابت *p = escstart + 1;
  صحیح esc_قدر = 0;		/* Value of \nnn escape. */
  صحیح esc_لمبائی;		/* Length of \nnn escape. */

  اگر (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      برائے (esc_لمبائی = 0, ++p;
           esc_لمبائی < 2 && c_isxdigit (*p);
           ++esc_لمبائی, ++p)
        esc_قدر = esc_قدر * 16 + fromhex (*p);
      اگر (esc_لمبائی == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      حرف_لکھو (esc_قدر);
    }
  ورنہ اگر (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      برائے (esc_لمبائی = 0, p += octal_0 && *p == '0';
           esc_لمبائی < 3 && isoct (*p);
           ++esc_لمبائی, ++p)
        esc_قدر = esc_قدر * 8 + fromoct (*p);
      حرف_لکھو (esc_قدر);
    }
  ورنہ اگر (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  ورنہ اگر (*p == 'u' || *p == 'U')
    {
      حرف esc_char = *p;
      غیرعلامتی صحیح uni_قدر;

      uni_قدر = 0;
      برائے (esc_لمبائی = (esc_char == 'u' ? 4 : 8), ++p;
           esc_لمبائی > 0;
           --esc_لمبائی, ++p)
        {
          اگر (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_قدر = uni_قدر * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      اگر (uni_قدر >= 0xd800 && uni_قدر <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_قدر);

      print_unicode_char (stdout, uni_قدر, 0);
    }
  ورنہ
    {
      حرف_لکھو ('\\');
      اگر (*p)
        {
          حرف_لکھو (*p);
          p++;
        }
    }
  واپس p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

ساکن خالی
print_esc_string (حرف ثابت *str)
{
  برائے (; *str; str++)
    اگر (*str == '\\')
      str += print_esc (str, true);
    ورنہ
      حرف_لکھو (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

ساکن خالی
print_direc (حرف ثابت *آغاز, حرف conversion,
             bool have_میدان_چوڑائی, صحیح میدان_چوڑائی,
             bool have_precision, صحیح precision,
             حرف ثابت *دلیل)
{
  حرف *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    حرف *q;
    حرف ثابت *لمبائی_modifier;
    size_t لمبائی_modifier_len;

    انتخاب (conversion)
      {
      حالت 'd': حالت 'i': حالت 'o': حالت 'u': حالت 'x': حالت 'X':
        لمبائی_modifier = "j";
        لمبائی_modifier_len = 1;
        توڑو;

      حالت 'a': حالت 'e': حالت 'f': حالت 'g':
      حالت 'A': حالت 'E': حالت 'F': حالت 'G':
        لمبائی_modifier = "L";
        لمبائی_modifier_len = 1;
        توڑو;

      طےشدہ:
        لمبائی_modifier = آغاز;  /* Any valid pointer will do.  */
        لمبائی_modifier_len = 0;
        توڑو;
      }

    size_t لمبائی = لڑی_لمبائی (آغاز);
    p = xmalloc (لمبائی + لمبائی_modifier_len + 2);
    q = mempcpy (p, آغاز, لمبائی);
    q = mempcpy (q, لمبائی_modifier, لمبائی_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  انتخاب (conversion)
    {
    حالت 'd':
    حالت 'i':
      {
        intmax_t arg = دلیل ? vstrtoimax (دلیل) : 0;
        اگر (!have_میدان_چوڑائی)
          {
            اگر (!have_precision)
              xprintf (p, arg);
            ورنہ
              xprintf (p, precision, arg);
          }
        ورنہ
          {
            اگر (!have_precision)
              xprintf (p, میدان_چوڑائی, arg);
            ورنہ
              xprintf (p, میدان_چوڑائی, precision, arg);
          }
      }
      توڑو;

    حالت 'o':
    حالت 'u':
    حالت 'x':
    حالت 'X':
      {
        uintmax_t arg = دلیل ? vstrtoumax (دلیل) : 0;
        اگر (!have_میدان_چوڑائی)
          {
            اگر (!have_precision)
              xprintf (p, arg);
            ورنہ
              xprintf (p, precision, arg);
          }
        ورنہ
          {
            اگر (!have_precision)
              xprintf (p, میدان_چوڑائی, arg);
            ورنہ
              xprintf (p, میدان_چوڑائی, precision, arg);
          }
      }
      توڑو;

    حالت 'a':
    حالت 'A':
    حالت 'e':
    حالت 'E':
    حالت 'f':
    حالت 'F':
    حالت 'g':
    حالت 'G':
      {
        طویل دوہرا arg = دلیل ? vstrtold (دلیل) : 0;
        اگر (!have_میدان_چوڑائی)
          {
            اگر (!have_precision)
              xprintf (p, arg);
            ورنہ
              xprintf (p, precision, arg);
          }
        ورنہ
          {
            اگر (!have_precision)
              xprintf (p, میدان_چوڑائی, arg);
            ورنہ
              xprintf (p, میدان_چوڑائی, precision, arg);
          }
      }
      توڑو;

    حالت 'c':
      {
        حرف c = دلیل ? *دلیل : '\0';
        اگر (!have_میدان_چوڑائی)
          xprintf (p, c);
        ورنہ
          xprintf (p, میدان_چوڑائی, c);
      }
      توڑو;

    حالت 's':
      اگر (!دلیل)
        دلیل = "";
      اگر (!have_میدان_چوڑائی)
        {
          اگر (!have_precision)
            xprintf (p, دلیل);
          ورنہ
            xprintf (p, precision, دلیل);
        }
      ورنہ
        {
          اگر (!have_precision)
            xprintf (p, میدان_چوڑائی, دلیل);
          ورنہ
            xprintf (p, میدان_چوڑائی, precision, دلیل);
        }
      توڑو;
    }

  آزاد (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

ساخت arg_cursor
{
  حرف ثابت *f;	/* Pointer into 'format'.  */
  صحیح curr_arg;		/* Current offset.  */
  صحیح curr_s_arg;	/* Current sequential offset.  */
  صحیح انتہا_arg;		/* End arg processed.  */
  صحیح direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_خالص ساکن ساخت arg_cursor
get_curr_arg (صحیح pos, ساخت arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  صحیح arg = 0;
  حرف ثابت *f = ac.f;
  اگر (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      صحیح a = *f++ - '0';
      برائے (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_جوڑو (&a, a, *f - '0');
        }
      اگر (*f == '$')
        arg = v ? زیادہ_سے_زیادہ_عدد : a;
    }

  اگر (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      اگر (pos == 0)
        ac.direc_arg = arg;
    }
  ورنہ
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  اگر (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.انتہا_arg = MAX (ac.انتہا_arg, arg);
    }
  واپس ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

ساکن صحیح
print_formatted (حرف ثابت *format, صحیح argc, حرف **argv)
{
  ساخت arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.انتہا_arg = ac.direc_arg = -1;
  حرف ثابت *direc_آغاز;	/* Start of % directive.  */
  حرف *direc;			/* Generated % directive.  */
  حرف *pdirec;			/* Pointer to current end of directive.  */
  bool have_میدان_چوڑائی;	/* True if FIELD_WIDTH is valid.  */
  صحیح میدان_چوڑائی = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  صحیح precision = 0;		/* Arg to second '*'.  */
  حرف ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (لڑی_لمبائی (format) + 1);

  برائے (ac.f = format; *ac.f; ac.f++)
    {
      انتخاب (*ac.f)
        {
        حالت '%':
          direc_آغاز = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_میدان_چوڑائی = have_precision = false;
          اگر (*ac.f == '%')
            {
              حرف_لکھو ('%');
              توڑو;
            }

          ac = get_curr_arg (0, ac);

          اگر (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              اگر (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              توڑو;
            }

          اگر (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              اگر (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              توڑو;
            }

          memset (ok, 0, حجم ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          برائے (;; ac.f++)
            {
              انتخاب (*ac.f)
                {
#اگر (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                حالت 'I':
#اگر_ختم
                حالت '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  توڑو;
                حالت '-': حالت '+': حالت ' ':
                  توڑو;
                حالت '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  توڑو;
                حالت '0':
                  ok['c'] = ok['s'] = 0;
                  توڑو;
                طےشدہ:
                  جاؤ no_مزید_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_مزید_flag_characters:

          اگر (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              اگر (ac.curr_arg < argc)
                {
                  intmax_t چوڑائی = vstrtoimax (argv[ac.curr_arg]);
                  اگر (INT_MIN <= چوڑائی && چوڑائی <= زیادہ_سے_زیادہ_عدد)
                    میدان_چوڑائی = چوڑائی;
                  ورنہ
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              ورنہ
                میدان_چوڑائی = 0;
              have_میدان_چوڑائی = true;
            }
          ورنہ
            جب (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          اگر (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              اگر (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  اگر (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      اگر (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      ورنہ اگر (زیادہ_سے_زیادہ_عدد < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      ورنہ
                        precision = prec;
                    }
                  ورنہ
                    precision = 0;
                  have_precision = true;
                }
              ورنہ
                جب (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          جب (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            غیرعلامتی حرف conversion = *ac.f;
            صحیح speclen = MIN (ac.f + 1 - direc_آغاز, زیادہ_سے_زیادہ_عدد);
            اگر (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_آغاز);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_میدان_چوڑائی, میدان_چوڑائی,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : تہی);

          توڑو;

        حالت '\\':
          ac.f += print_esc (ac.f, false);
          توڑو;

        طےشدہ:
          حرف_لکھو (*ac.f);
        }
    }

  آزاد (direc);
  واپس MIN (argc, ac.انتہا_arg + 1);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف *format;
  صحیح دلائل_مستعمل;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != تہی);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  اگر (argc == 2)
    {
      اگر (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      اگر (streq (argv[1], "--version"))
        {
          نسخہ_etc (stdout, پروگرام_NAME, PACKAGE_NAME, نسخہ, AUTHORS,
                       (حرف *) تہی);
          واپس EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  اگر (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  اگر (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  کرو
    {
      دلائل_مستعمل = print_formatted (format, argc, argv);
      argc -= دلائل_مستعمل;
      argv += دلائل_مستعمل;
    }
  جب (دلائل_مستعمل > 0 && argc > 0);

  اگر (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  واپس exit_حالت;
}
