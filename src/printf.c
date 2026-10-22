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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <wchar.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "cl-strtod.h"
#अन्तर्भाव "octhexdigits.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "unicodeio.h"
#अन्तर्भाव "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printf"

#define AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
स्थिर पूर्णांक exit_status;

/* True if the POSIXLY_CORRECT environment variable is set.  */
स्थिर bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
स्थिर अक्षर स्थिरांक *स्थिरांक cfcc_संदेश =
 N_("warning: %s: character(s) following character constant have been ignored");

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s FORMAT [ARGUMENT]...\n\
  or:  %s OPTION\n\
"),
              program_name, program_name);
      ख_माला_दो (_("\
Print ARGUMENT(s) according to FORMAT, or execute according to OPTION:\n\
\n\
"), मानक_निकास);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
FORMAT controls the output as in C म_लिखो.  Interpreted sequences are:\n\
\n\
  \\\"      द्विगुण quote\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new ओळ\n\
  \\r      carriage परत\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  \\NNN    byte with octal मूल्य NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal मूल्य HH (1 to 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex मूल्य HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex मूल्य HHHHHHHH (8 digits)\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
  %%      a single %\n\
  %b      ARGUMENT as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      ARGUMENT is printed in a format that can be reused as shell आदान,\n\
          escaping non-printable characters with the POSIX $'' syntax\
\n\n\
and all C म_लिखो(3) format specifications ending with one of diouxXfaAeEgGcs,\n\
with ARGUMENTs converted to proper type first.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
FORMAT is reused to convert all specified arguments.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
The following table shows supported conversion specifiers,\n\
with examples including usage of width, precision, and flags.\n\
\n\
  CONVERSION  Shell format example        Output\n\
  %s          '[%10s]' 'left pad'         [  डावा pad]\n\
  %s          '[%-10s]' 'right pad'       [उजवा pad ]\n\
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
"), मानक_निकास);
      म_लिखो (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

स्थिर शून्य
verify_numeric (अक्षर स्थिरांक *s, अक्षर स्थिरांक *end)
{
  जर (s == end)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_status = निकास_त्रुटि;
    }
  अन्यथा जर (त्रुटि_सं)
    {
      error (0, त्रुटि_सं, "%s", quote (s));
      exit_status = निकास_त्रुटि;
    }
  अन्यथा जर (*end)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_status = निकास_त्रुटि;
    }
}

#define STRTOX(TYPE, FUNC_NAME, LIB_FUNC_अभिव्यक्ती)				 \
स्थिर TYPE								 \
FUNC_NAME (अक्षर स्थिरांक *s)						 \
{									 \
  अक्षर *end;								 \
  TYPE val;								 \
                                                                         \
  जर ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      अचिन्हित अक्षर ch = *++s;						 \
      val = ch;								 \
                                                                         \
      जर (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          ब_अक्षर_प्रकार wc;							 \
          माप_प्रकार slen = म_माप (s);					 \
          ssize_t bytes;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          bytes = mbrtowc (&wc, s, slen, &mbstate);			 \
          जर (0 < bytes)						 \
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
      जर (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_संदेश), s);					 \
    }									 \
  अन्यथा									 \
    {									 \
      त्रुटि_सं = 0;							 \
      val = (LIB_FUNC_अभिव्यक्ती);						 \
      verify_numeric (s, end);						 \
    }									 \
  परत val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &end, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &end, 0))
STRTOX (दीर्घ द्विगुण, vstrtold,   cl_strtold (s, &end))

/* Output a single-character \ escape.  */

स्थिर शून्य
print_esc_char (अक्षर c)
{
  निवड (c)
    {
    स्थिती 'a':			/* Alert. */
      अक्षर_दो ('\a');
      थांबा;
    स्थिती 'b':			/* Backspace. */
      अक्षर_दो ('\b');
      थांबा;
    स्थिती 'c':			/* Cancel the rest of the output. */
      निकास (निकास_सफल);
      थांबा;
    स्थिती 'e':			/* Escape. */
      अक्षर_दो ('\x1B');
      थांबा;
    स्थिती 'f':			/* Form feed. */
      अक्षर_दो ('\f');
      थांबा;
    स्थिती 'n':			/* New line. */
      अक्षर_दो ('\n');
      थांबा;
    स्थिती 'r':			/* Carriage return. */
      अक्षर_दो ('\r');
      थांबा;
    स्थिती 't':			/* Horizontal tab. */
      अक्षर_दो ('\t');
      थांबा;
    स्थिती 'v':			/* Vertical tab. */
      अक्षर_दो ('\v');
      थांबा;
    पूर्वनिर्धारित:
      अक्षर_दो (c);
      थांबा;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

स्थिर पूर्णांक
print_esc (अक्षर स्थिरांक *escstart, bool octal_0)
{
  अक्षर स्थिरांक *p = escstart + 1;
  पूर्णांक esc_मूल्य = 0;		/* Value of \nnn escape. */
  पूर्णांक esc_length;		/* Length of \nnn escape. */

  जर (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      साठी (esc_length = 0, ++p;
           esc_length < 2 && c_isxdigit (*p);
           ++esc_length, ++p)
        esc_मूल्य = esc_मूल्य * 16 + fromhex (*p);
      जर (esc_length == 0)
        error (निकास_त्रुटि, 0, _("missing hexadecimal number in escape"));
      अक्षर_दो (esc_मूल्य);
    }
  अन्यथा जर (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      साठी (esc_length = 0, p += octal_0 && *p == '0';
           esc_length < 3 && isoct (*p);
           ++esc_length, ++p)
        esc_मूल्य = esc_मूल्य * 8 + fromoct (*p);
      अक्षर_दो (esc_मूल्य);
    }
  अन्यथा जर (*p && म_अक्षर ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  अन्यथा जर (*p == 'u' || *p == 'U')
    {
      अक्षर esc_char = *p;
      अचिन्हित पूर्णांक uni_मूल्य;

      uni_मूल्य = 0;
      साठी (esc_length = (esc_char == 'u' ? 4 : 8), ++p;
           esc_length > 0;
           --esc_length, ++p)
        {
          जर (! c_isxdigit (*p))
            error (निकास_त्रुटि, 0, _("missing hexadecimal number in escape"));
          uni_मूल्य = uni_मूल्य * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      जर (uni_मूल्य >= 0xd800 && uni_मूल्य <= 0xdfff)
        error (निकास_त्रुटि, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_मूल्य);

      print_unicode_char (मानक_निकास, uni_मूल्य, 0);
    }
  अन्यथा
    {
      अक्षर_दो ('\\');
      जर (*p)
        {
          अक्षर_दो (*p);
          p++;
        }
    }
  परत p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

स्थिर शून्य
print_esc_string (अक्षर स्थिरांक *str)
{
  साठी (; *str; str++)
    जर (*str == '\\')
      str += print_esc (str, true);
    अन्यथा
      अक्षर_दो (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

स्थिर शून्य
print_direc (अक्षर स्थिरांक *start, अक्षर conversion,
             bool have_field_width, पूर्णांक field_width,
             bool have_precision, पूर्णांक precision,
             अक्षर स्थिरांक *argument)
{
  अक्षर *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    अक्षर *q;
    अक्षर स्थिरांक *length_modifier;
    माप_प्रकार length_modifier_len;

    निवड (conversion)
      {
      स्थिती 'd': स्थिती 'i': स्थिती 'o': स्थिती 'u': स्थिती 'x': स्थिती 'X':
        length_modifier = "j";
        length_modifier_len = 1;
        थांबा;

      स्थिती 'a': स्थिती 'e': स्थिती 'f': स्थिती 'g':
      स्थिती 'A': स्थिती 'E': स्थिती 'F': स्थिती 'G':
        length_modifier = "L";
        length_modifier_len = 1;
        थांबा;

      पूर्वनिर्धारित:
        length_modifier = start;  /* Any valid pointer will do.  */
        length_modifier_len = 0;
        थांबा;
      }

    माप_प्रकार length = म_माप (start);
    p = xmalloc (length + length_modifier_len + 2);
    q = mempcpy (p, start, length);
    q = mempcpy (q, length_modifier, length_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  निवड (conversion)
    {
    स्थिती 'd':
    स्थिती 'i':
      {
        intmax_t arg = argument ? vstrtoimax (argument) : 0;
        जर (!have_field_width)
          {
            जर (!have_precision)
              xprintf (p, arg);
            अन्यथा
              xprintf (p, precision, arg);
          }
        अन्यथा
          {
            जर (!have_precision)
              xprintf (p, field_width, arg);
            अन्यथा
              xprintf (p, field_width, precision, arg);
          }
      }
      थांबा;

    स्थिती 'o':
    स्थिती 'u':
    स्थिती 'x':
    स्थिती 'X':
      {
        uintmax_t arg = argument ? vstrtoumax (argument) : 0;
        जर (!have_field_width)
          {
            जर (!have_precision)
              xprintf (p, arg);
            अन्यथा
              xprintf (p, precision, arg);
          }
        अन्यथा
          {
            जर (!have_precision)
              xprintf (p, field_width, arg);
            अन्यथा
              xprintf (p, field_width, precision, arg);
          }
      }
      थांबा;

    स्थिती 'a':
    स्थिती 'A':
    स्थिती 'e':
    स्थिती 'E':
    स्थिती 'f':
    स्थिती 'F':
    स्थिती 'g':
    स्थिती 'G':
      {
        दीर्घ द्विगुण arg = argument ? vstrtold (argument) : 0;
        जर (!have_field_width)
          {
            जर (!have_precision)
              xprintf (p, arg);
            अन्यथा
              xprintf (p, precision, arg);
          }
        अन्यथा
          {
            जर (!have_precision)
              xprintf (p, field_width, arg);
            अन्यथा
              xprintf (p, field_width, precision, arg);
          }
      }
      थांबा;

    स्थिती 'c':
      {
        अक्षर c = argument ? *argument : '\0';
        जर (!have_field_width)
          xprintf (p, c);
        अन्यथा
          xprintf (p, field_width, c);
      }
      थांबा;

    स्थिती 's':
      जर (!argument)
        argument = "";
      जर (!have_field_width)
        {
          जर (!have_precision)
            xprintf (p, argument);
          अन्यथा
            xprintf (p, precision, argument);
        }
      अन्यथा
        {
          जर (!have_precision)
            xprintf (p, field_width, argument);
          अन्यथा
            xprintf (p, field_width, precision, argument);
        }
      थांबा;
    }

  मुक्त (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

संरचना arg_cursor
{
  अक्षर स्थिरांक *f;	/* Pointer into 'format'.  */
  पूर्णांक curr_arg;		/* Current offset.  */
  पूर्णांक curr_s_arg;	/* Current sequential offset.  */
  पूर्णांक end_arg;		/* End arg processed.  */
  पूर्णांक direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_PURE स्थिर संरचना arg_cursor
get_curr_arg (पूर्णांक pos, संरचना arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  पूर्णांक arg = 0;
  अक्षर स्थिरांक *f = ac.f;
  जर (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      पूर्णांक a = *f++ - '0';
      साठी (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_add (&a, a, *f - '0');
        }
      जर (*f == '$')
        arg = v ? पूर्णांक_उच्च : a;
    }

  जर (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      जर (pos == 0)
        ac.direc_arg = arg;
    }
  अन्यथा
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  जर (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.end_arg = MAX (ac.end_arg, arg);
    }
  परत ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

स्थिर पूर्णांक
print_formatted (अक्षर स्थिरांक *format, पूर्णांक argc, अक्षर **argv)
{
  संरचना arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.end_arg = ac.direc_arg = -1;
  अक्षर स्थिरांक *direc_start;	/* Start of % directive.  */
  अक्षर *direc;			/* Generated % directive.  */
  अक्षर *pdirec;			/* Pointer to current end of directive.  */
  bool have_field_width;	/* True if FIELD_WIDTH is valid.  */
  पूर्णांक field_width = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  पूर्णांक precision = 0;		/* Arg to second '*'.  */
  अक्षर ok[अच_अक्षर_उच्च + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (म_माप (format) + 1);

  साठी (ac.f = format; *ac.f; ac.f++)
    {
      निवड (*ac.f)
        {
        स्थिती '%':
          direc_start = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_field_width = have_precision = false;
          जर (*ac.f == '%')
            {
              अक्षर_दो ('%');
              थांबा;
            }

          ac = get_curr_arg (0, ac);

          जर (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              जर (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              थांबा;
            }

          जर (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              जर (ac.curr_arg < argc)
                {
                  ख_माला_दो (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), मानक_निकास);
                }
              थांबा;
            }

          स_रखो (ok, 0, आकार ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          साठी (;; ac.f++)
            {
              निवड (*ac.f)
                {
#जर (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                स्थिती 'I':
#endif
                स्थिती '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  थांबा;
                स्थिती '-': स्थिती '+': स्थिती ' ':
                  थांबा;
                स्थिती '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  थांबा;
                स्थिती '0':
                  ok['c'] = ok['s'] = 0;
                  थांबा;
                पूर्वनिर्धारित:
                  जा no_more_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_more_flag_characters:

          जर (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              जर (ac.curr_arg < argc)
                {
                  intmax_t width = vstrtoimax (argv[ac.curr_arg]);
                  जर (पूर्णांक_न्यून <= width && width <= पूर्णांक_उच्च)
                    field_width = width;
                  अन्यथा
                    error (निकास_त्रुटि, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              अन्यथा
                field_width = 0;
              have_field_width = true;
            }
          अन्यथा
            पर्यंत (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          जर (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              जर (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  जर (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      जर (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      अन्यथा जर (पूर्णांक_उच्च < prec)
                        error (निकास_त्रुटि, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      अन्यथा
                        precision = prec;
                    }
                  अन्यथा
                    precision = 0;
                  have_precision = true;
                }
              अन्यथा
                पर्यंत (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          पर्यंत (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            अचिन्हित अक्षर conversion = *ac.f;
            पूर्णांक speclen = MIN (ac.f + 1 - direc_start, पूर्णांक_उच्च);
            जर (! ok[conversion])
              error (निकास_त्रुटि, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_start);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_field_width, field_width,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : रिक्त);

          थांबा;

        स्थिती '\\':
          ac.f += print_esc (ac.f, false);
          थांबा;

        पूर्वनिर्धारित:
          अक्षर_दो (*ac.f);
        }
    }

  मुक्त (direc);
  परत MIN (argc, ac.end_arg + 1);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  अक्षर *format;
  पूर्णांक args_used;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  posixly_correct = (दो_पर्या ("POSIXLY_CORRECT") != रिक्त);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  जर (argc == 2)
    {
      जर (streq (argv[1], "--help"))
        usage (निकास_सफल);

      जर (streq (argv[1], "--version"))
        {
          version_etc (मानक_निकास, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (अक्षर *) रिक्त);
          परत निकास_सफल;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  जर (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  जर (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  करा
    {
      args_used = print_formatted (format, argc, argv);
      argc -= args_used;
      argv += args_used;
    }
  पर्यंत (args_used > 0 && argc > 0);

  जर (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  परत exit_status;
}
