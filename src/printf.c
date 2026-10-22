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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <wchar.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "cl-strtod.h"
#అన్తర్భావ "octhexdigits.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "unicodeio.h"
#అన్తర్భావ "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "printf"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
స్థిర పూర్ణాంకం exit_స్థితి;

/* True if the POSIXLY_CORRECT environment variable is set.  */
స్థిర bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి cfcc_సందేశం =
 N_("warning: %s: character(s) following character constant have been ignored");

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s FORMAT [వాదం_2]...\n\
  or:  %s OPTION\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Print వాదం_2(s) according గమ్యం FORMAT, or execute according గమ్యం OPTION:\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FORMAT controls the output as లోపల C రాయి.  Interpreted sequences are:\n\
\n\
  \\\"      ద్విగుణ quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new పంక్తి\n\
  \\r      carriage తిరిగి\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal విలువ NNN (1 గమ్యం 3 digits)\n\
  \\xHH    byte with hexadecimal విలువ HH (1 గమ్యం 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex విలువ HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex విలువ HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      వాదం_2 as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      వాదం_2 is printed లోపల a format that can be reused as shell ఆదానం,\n\
          escaping non-printable characters with the POSIX $'' syntax\
\n\n\
and all C రాయి(3) format specifications ending with one యొక్క diouxXfaAeEgGcs,\n\
with ARGUMENTs converted గమ్యం proper type మొదటి.\n\
"), stdout);
      fputs (_("\
FORMAT is reused గమ్యం convert all specified arguments.\n\
"), stdout);
      fputs (_("\
The following పట్టిక shows supported conversion specifiers,\n\
with examples including usage యొక్క వెడల్పు, precision, and flags.\n\
\n\
  CONVERSION  Shell format example        Output\n\
  %s          '[%10s]' 'left pad'         [  ఎడమ pad]\n\
  %s          '[%-10s]' 'right pad'       [కుడి pad ]\n\
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
      రాయి (USAGE_BUILTIN_WARNING, కార్యక్రమం_NAME);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

స్థిర శూన్య
verify_numeric (అక్షరం స్థిరరాశి *s, అక్షరం స్థిరరాశి *చివర)
{
  ఒకవేళ (s == చివర)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_స్థితి = EXIT_FAILURE;
    }
  లేకపోతే ఒకవేళ (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_స్థితి = EXIT_FAILURE;
    }
  లేకపోతే ఒకవేళ (*చివర)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_స్థితి = EXIT_FAILURE;
    }
}

#నిర్వచించు STRTOX(TYPE, FUNC_NAME, LIB_FUNC_సమాసం)				 \
స్థిర TYPE								 \
FUNC_NAME (అక్షరం స్థిరరాశి *s)						 \
{									 \
  అక్షరం *చివర;								 \
  TYPE val;								 \
                                                                         \
  ఒకవేళ ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      గుర్తులేని అక్షరం ch = *++s;						 \
      val = ch;								 \
                                                                         \
      ఒకవేళ (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = అక్షరశ్రేణి_పొడవు (s);					 \
          ssize_t బైట్లు;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          బైట్లు = mbrtowc (&wc, s, slen, &mbstate);			 \
          ఒకవేళ (0 < బైట్లు)						 \
            {								 \
              val = wc;							 \
              s += బైట్లు - 1;						 \
            }								 \
        }								 \
                                                                         \
      /* If POSIXLY_CORRECT is not set, then give a warning that there	 \
         are characters following the character constant and that GNU	 \
         printf is ignoring those characters.  If POSIXLY_CORRECT *is*	 \
         set, then don't give the warning.  */				 \
      ఒకవేళ (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_సందేశం), s);					 \
    }									 \
  లేకపోతే									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_సమాసం);						 \
      verify_numeric (s, చివర);						 \
    }									 \
  తిరిగి val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &చివర, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &చివర, 0))
STRTOX (దీర్ఘ ద్విగుణ, vstrtold,   cl_strtold (s, &చివర))

/* Output a single-character \ escape.  */

స్థిర శూన్య
print_esc_char (అక్షరం c)
{
  ఎంపిక (c)
    {
    స్థితి 'a':			/* Alert. */
      అక్షరం_రాయి ('\a');
      ఆపు;
    స్థితి 'b':			/* Backspace. */
      అక్షరం_రాయి ('\b');
      ఆపు;
    స్థితి 'c':			/* Cancel the rest of the output. */
      నిష్క్రమించు (EXIT_SUCCESS);
      ఆపు;
    స్థితి 'e':			/* Escape. */
      అక్షరం_రాయి ('\x1B');
      ఆపు;
    స్థితి 'f':			/* Form feed. */
      అక్షరం_రాయి ('\f');
      ఆపు;
    స్థితి 'n':			/* New line. */
      అక్షరం_రాయి ('\n');
      ఆపు;
    స్థితి 'r':			/* Carriage return. */
      అక్షరం_రాయి ('\r');
      ఆపు;
    స్థితి 't':			/* Horizontal tab. */
      అక్షరం_రాయి ('\t');
      ఆపు;
    స్థితి 'v':			/* Vertical tab. */
      అక్షరం_రాయి ('\v');
      ఆపు;
    అప్రమేయ:
      అక్షరం_రాయి (c);
      ఆపు;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

స్థిర పూర్ణాంకం
print_esc (అక్షరం స్థిరరాశి *escstart, bool octal_0)
{
  అక్షరం స్థిరరాశి *p = escstart + 1;
  పూర్ణాంకం esc_విలువ = 0;		/* Value of \nnn escape. */
  పూర్ణాంకం esc_నిడివి;		/* Length of \nnn escape. */

  ఒకవేళ (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      కొరకు (esc_నిడివి = 0, ++p;
           esc_నిడివి < 2 && c_isxdigit (*p);
           ++esc_నిడివి, ++p)
        esc_విలువ = esc_విలువ * 16 + fromhex (*p);
      ఒకవేళ (esc_నిడివి == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      అక్షరం_రాయి (esc_విలువ);
    }
  లేకపోతే ఒకవేళ (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      కొరకు (esc_నిడివి = 0, p += octal_0 && *p == '0';
           esc_నిడివి < 3 && isoct (*p);
           ++esc_నిడివి, ++p)
        esc_విలువ = esc_విలువ * 8 + fromoct (*p);
      అక్షరం_రాయి (esc_విలువ);
    }
  లేకపోతే ఒకవేళ (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  లేకపోతే ఒకవేళ (*p == 'u' || *p == 'U')
    {
      అక్షరం esc_char = *p;
      గుర్తులేని పూర్ణాంకం uni_విలువ;

      uni_విలువ = 0;
      కొరకు (esc_నిడివి = (esc_char == 'u' ? 4 : 8), ++p;
           esc_నిడివి > 0;
           --esc_నిడివి, ++p)
        {
          ఒకవేళ (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_విలువ = uni_విలువ * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      ఒకవేళ (uni_విలువ >= 0xd800 && uni_విలువ <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_విలువ);

      print_unicode_char (stdout, uni_విలువ, 0);
    }
  లేకపోతే
    {
      అక్షరం_రాయి ('\\');
      ఒకవేళ (*p)
        {
          అక్షరం_రాయి (*p);
          p++;
        }
    }
  తిరిగి p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

స్థిర శూన్య
print_esc_string (అక్షరం స్థిరరాశి *str)
{
  కొరకు (; *str; str++)
    ఒకవేళ (*str == '\\')
      str += print_esc (str, true);
    లేకపోతే
      అక్షరం_రాయి (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

స్థిర శూన్య
print_direc (అక్షరం స్థిరరాశి *ప్రారంభం, అక్షరం conversion,
             bool have_క్షేత్రం_వెడల్పు, పూర్ణాంకం క్షేత్రం_వెడల్పు,
             bool have_precision, పూర్ణాంకం precision,
             అక్షరం స్థిరరాశి *వాదం)
{
  అక్షరం *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    అక్షరం *q;
    అక్షరం స్థిరరాశి *నిడివి_modifier;
    size_t నిడివి_modifier_len;

    ఎంపిక (conversion)
      {
      స్థితి 'd': స్థితి 'i': స్థితి 'o': స్థితి 'u': స్థితి 'x': స్థితి 'X':
        నిడివి_modifier = "j";
        నిడివి_modifier_len = 1;
        ఆపు;

      స్థితి 'a': స్థితి 'e': స్థితి 'f': స్థితి 'g':
      స్థితి 'A': స్థితి 'E': స్థితి 'F': స్థితి 'G':
        నిడివి_modifier = "L";
        నిడివి_modifier_len = 1;
        ఆపు;

      అప్రమేయ:
        నిడివి_modifier = ప్రారంభం;  /* Any valid pointer will do.  */
        నిడివి_modifier_len = 0;
        ఆపు;
      }

    size_t నిడివి = అక్షరశ్రేణి_పొడవు (ప్రారంభం);
    p = xmalloc (నిడివి + నిడివి_modifier_len + 2);
    q = mempcpy (p, ప్రారంభం, నిడివి);
    q = mempcpy (q, నిడివి_modifier, నిడివి_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  ఎంపిక (conversion)
    {
    స్థితి 'd':
    స్థితి 'i':
      {
        intmax_t arg = వాదం ? vstrtoimax (వాదం) : 0;
        ఒకవేళ (!have_క్షేత్రం_వెడల్పు)
          {
            ఒకవేళ (!have_precision)
              xprintf (p, arg);
            లేకపోతే
              xprintf (p, precision, arg);
          }
        లేకపోతే
          {
            ఒకవేళ (!have_precision)
              xprintf (p, క్షేత్రం_వెడల్పు, arg);
            లేకపోతే
              xprintf (p, క్షేత్రం_వెడల్పు, precision, arg);
          }
      }
      ఆపు;

    స్థితి 'o':
    స్థితి 'u':
    స్థితి 'x':
    స్థితి 'X':
      {
        uintmax_t arg = వాదం ? vstrtoumax (వాదం) : 0;
        ఒకవేళ (!have_క్షేత్రం_వెడల్పు)
          {
            ఒకవేళ (!have_precision)
              xprintf (p, arg);
            లేకపోతే
              xprintf (p, precision, arg);
          }
        లేకపోతే
          {
            ఒకవేళ (!have_precision)
              xprintf (p, క్షేత్రం_వెడల్పు, arg);
            లేకపోతే
              xprintf (p, క్షేత్రం_వెడల్పు, precision, arg);
          }
      }
      ఆపు;

    స్థితి 'a':
    స్థితి 'A':
    స్థితి 'e':
    స్థితి 'E':
    స్థితి 'f':
    స్థితి 'F':
    స్థితి 'g':
    స్థితి 'G':
      {
        దీర్ఘ ద్విగుణ arg = వాదం ? vstrtold (వాదం) : 0;
        ఒకవేళ (!have_క్షేత్రం_వెడల్పు)
          {
            ఒకవేళ (!have_precision)
              xprintf (p, arg);
            లేకపోతే
              xprintf (p, precision, arg);
          }
        లేకపోతే
          {
            ఒకవేళ (!have_precision)
              xprintf (p, క్షేత్రం_వెడల్పు, arg);
            లేకపోతే
              xprintf (p, క్షేత్రం_వెడల్పు, precision, arg);
          }
      }
      ఆపు;

    స్థితి 'c':
      {
        అక్షరం c = వాదం ? *వాదం : '\0';
        ఒకవేళ (!have_క్షేత్రం_వెడల్పు)
          xprintf (p, c);
        లేకపోతే
          xprintf (p, క్షేత్రం_వెడల్పు, c);
      }
      ఆపు;

    స్థితి 's':
      ఒకవేళ (!వాదం)
        వాదం = "";
      ఒకవేళ (!have_క్షేత్రం_వెడల్పు)
        {
          ఒకవేళ (!have_precision)
            xprintf (p, వాదం);
          లేకపోతే
            xprintf (p, precision, వాదం);
        }
      లేకపోతే
        {
          ఒకవేళ (!have_precision)
            xprintf (p, క్షేత్రం_వెడల్పు, వాదం);
          లేకపోతే
            xprintf (p, క్షేత్రం_వెడల్పు, precision, వాదం);
        }
      ఆపు;
    }

  విడుదల (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

నిర్మాణం arg_cursor
{
  అక్షరం స్థిరరాశి *f;	/* Pointer into 'format'.  */
  పూర్ణాంకం curr_arg;		/* Current offset.  */
  పూర్ణాంకం curr_s_arg;	/* Current sequential offset.  */
  పూర్ణాంకం చివర_arg;		/* End arg processed.  */
  పూర్ణాంకం direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_శుద్ధ స్థిర నిర్మాణం arg_cursor
get_curr_arg (పూర్ణాంకం pos, నిర్మాణం arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  పూర్ణాంకం arg = 0;
  అక్షరం స్థిరరాశి *f = ac.f;
  ఒకవేళ (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      పూర్ణాంకం a = *f++ - '0';
      కొరకు (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_కూడు (&a, a, *f - '0');
        }
      ఒకవేళ (*f == '$')
        arg = v ? గరిష్ఠ_పూర్ణాంకం : a;
    }

  ఒకవేళ (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      ఒకవేళ (pos == 0)
        ac.direc_arg = arg;
    }
  లేకపోతే
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  ఒకవేళ (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.చివర_arg = MAX (ac.చివర_arg, arg);
    }
  తిరిగి ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

స్థిర పూర్ణాంకం
print_formatted (అక్షరం స్థిరరాశి *format, పూర్ణాంకం argc, అక్షరం **argv)
{
  నిర్మాణం arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.చివర_arg = ac.direc_arg = -1;
  అక్షరం స్థిరరాశి *direc_ప్రారంభం;	/* Start of % directive.  */
  అక్షరం *direc;			/* Generated % directive.  */
  అక్షరం *pdirec;			/* Pointer to current end of directive.  */
  bool have_క్షేత్రం_వెడల్పు;	/* True if FIELD_WIDTH is valid.  */
  పూర్ణాంకం క్షేత్రం_వెడల్పు = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  పూర్ణాంకం precision = 0;		/* Arg to second '*'.  */
  అక్షరం ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (అక్షరశ్రేణి_పొడవు (format) + 1);

  కొరకు (ac.f = format; *ac.f; ac.f++)
    {
      ఎంపిక (*ac.f)
        {
        స్థితి '%':
          direc_ప్రారంభం = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_క్షేత్రం_వెడల్పు = have_precision = false;
          ఒకవేళ (*ac.f == '%')
            {
              అక్షరం_రాయి ('%');
              ఆపు;
            }

          ac = get_curr_arg (0, ac);

          ఒకవేళ (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              ఒకవేళ (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              ఆపు;
            }

          ఒకవేళ (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              ఒకవేళ (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              ఆపు;
            }

          memset (ok, 0, పరిమాణం ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          కొరకు (;; ac.f++)
            {
              ఎంపిక (*ac.f)
                {
#ఒకవేళ (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                స్థితి 'I':
#అయితే_ముగింపు
                స్థితి '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  ఆపు;
                స్థితి '-': స్థితి '+': స్థితి ' ':
                  ఆపు;
                స్థితి '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  ఆపు;
                స్థితి '0':
                  ok['c'] = ok['s'] = 0;
                  ఆపు;
                అప్రమేయ:
                  వెళ్ళు no_మరిన్ని_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_మరిన్ని_flag_characters:

          ఒకవేళ (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              ఒకవేళ (ac.curr_arg < argc)
                {
                  intmax_t వెడల్పు = vstrtoimax (argv[ac.curr_arg]);
                  ఒకవేళ (INT_MIN <= వెడల్పు && వెడల్పు <= గరిష్ఠ_పూర్ణాంకం)
                    క్షేత్రం_వెడల్పు = వెడల్పు;
                  లేకపోతే
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              లేకపోతే
                క్షేత్రం_వెడల్పు = 0;
              have_క్షేత్రం_వెడల్పు = true;
            }
          లేకపోతే
            అయితే (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          ఒకవేళ (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              ఒకవేళ (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  ఒకవేళ (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      ఒకవేళ (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      లేకపోతే ఒకవేళ (గరిష్ఠ_పూర్ణాంకం < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      లేకపోతే
                        precision = prec;
                    }
                  లేకపోతే
                    precision = 0;
                  have_precision = true;
                }
              లేకపోతే
                అయితే (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          అయితే (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            గుర్తులేని అక్షరం conversion = *ac.f;
            పూర్ణాంకం speclen = MIN (ac.f + 1 - direc_ప్రారంభం, గరిష్ఠ_పూర్ణాంకం);
            ఒకవేళ (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_ప్రారంభం);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_క్షేత్రం_వెడల్పు, క్షేత్రం_వెడల్పు,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : ఖాళీ);

          ఆపు;

        స్థితి '\\':
          ac.f += print_esc (ac.f, false);
          ఆపు;

        అప్రమేయ:
          అక్షరం_రాయి (*ac.f);
        }
    }

  విడుదల (direc);
  తిరిగి MIN (argc, ac.చివర_arg + 1);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం *format;
  పూర్ణాంకం వాదనలు_వాడిన;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ఖాళీ);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  ఒకవేళ (argc == 2)
    {
      ఒకవేళ (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      ఒకవేళ (streq (argv[1], "--version"))
        {
          సంస్కరణ_etc (stdout, కార్యక్రమం_NAME, PACKAGE_NAME, సంస్కరణ, AUTHORS,
                       (అక్షరం *) ఖాళీ);
          తిరిగి EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  ఒకవేళ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ఒకవేళ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  చేయి
    {
      వాదనలు_వాడిన = print_formatted (format, argc, argv);
      argc -= వాదనలు_వాడిన;
      argv += వాదనలు_వాడిన;
    }
  అయితే (వాదనలు_వాడిన > 0 && argc > 0);

  ఒకవేళ (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  తిరిగి exit_స్థితి;
}
