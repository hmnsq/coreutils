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
#परिभाषय क्रमादेशः_NAME "printf"

#परिभाषय AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
स्थिर पूर्णाङ्क exit_स्थितिः;

/* True if the POSIXLY_CORRECT environment variable is set.  */
स्थिर bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
स्थिर वर्ण ध्रुव *ध्रुव cfcc_सन्देशः =
 N_("warning: %s: character(s) following character constant have been ignored");

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s FORMAT [तर्कः_2]...\n\
  or:  %s OPTION\n\
"),
              क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
Print तर्कः_2(s) according गन्तव्यम् FORMAT, or execute according गन्तव्यम् OPTION:\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FORMAT controls the output as अन्तर् C प्रारूप_लिख.  Interpreted sequences are:\n\
\n\
  \\\"      द्विगुण quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new पङ्क्तिः\n\
  \\r      carriage प्रत्यागम\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal मानम् NNN (1 गन्तव्यम् 3 digits)\n\
  \\xHH    byte with hexadecimal मानम् HH (1 गन्तव्यम् 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex मानम् HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex मानम् HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      तर्कः_2 as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      तर्कः_2 is printed अन्तर् a format that can be reused as shell निवेशः,\n\
          escaping non-printable characters with the POSIX $'' syntax\
\n\n\
and all C प्रारूप_लिख(3) format specifications ending with one सम्बन्धि diouxXfaAeEgGcs,\n\
with ARGUMENTs converted गन्तव्यम् proper type first.\n\
"), stdout);
      fputs (_("\
FORMAT is reused गन्तव्यम् convert all specified arguments.\n\
"), stdout);
      fputs (_("\
The following सारणी shows supported conversion specifiers,\n\
with examples including usage सम्बन्धि विस्तारः, precision, and flags.\n\
\n\
  CONVERSION  Shell format example        Output\n\
  %s          '[%10s]' 'left pad'         [  वामः pad]\n\
  %s          '[%-10s]' 'right pad'       [दक्षिणः pad ]\n\
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
      प्रारूप_लिख (USAGE_BUILTIN_WARNING, क्रमादेशः_NAME);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

स्थिर शून्य
verify_numeric (वर्ण ध्रुव *s, वर्ण ध्रुव *अन्तः)
{
  यदि (s == अन्तः)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_स्थितिः = EXIT_FAILURE;
    }
  अन्यथा यदि (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_स्थितिः = EXIT_FAILURE;
    }
  अन्यथा यदि (*अन्तः)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_स्थितिः = EXIT_FAILURE;
    }
}

#परिभाषय STRTOX(TYPE, FUNC_NAME, LIB_FUNC_व्यञ्जकम्)				 \
स्थिर TYPE								 \
FUNC_NAME (वर्ण ध्रुव *s)						 \
{									 \
  वर्ण *अन्तः;								 \
  TYPE val;								 \
                                                                         \
  यदि ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      अचिह्नित वर्ण ch = *++s;						 \
      val = ch;								 \
                                                                         \
      यदि (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = वाक्यदैर्घ्यम् (s);					 \
          ssize_t अष्टकानि;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          अष्टकानि = mbrtowc (&wc, s, slen, &mbstate);			 \
          यदि (0 < अष्टकानि)						 \
            {								 \
              val = wc;							 \
              s += अष्टकानि - 1;						 \
            }								 \
        }								 \
                                                                         \
      /* If POSIXLY_CORRECT is not set, then give a warning that there	 \
         are characters following the character constant and that GNU	 \
         printf is ignoring those characters.  If POSIXLY_CORRECT *is*	 \
         set, then don't give the warning.  */				 \
      यदि (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_सन्देशः), s);					 \
    }									 \
  अन्यथा									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_व्यञ्जकम्);						 \
      verify_numeric (s, अन्तः);						 \
    }									 \
  प्रत्यागम val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &अन्तः, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &अन्तः, 0))
STRTOX (दीर्घ द्विगुण, vstrtold,   cl_strtold (s, &अन्तः))

/* Output a single-character \ escape.  */

स्थिर शून्य
print_esc_char (वर्ण c)
{
  विकल्प (c)
    {
    पक्ष 'a':			/* Alert. */
      अक्षरं_लिख ('\a');
      भङ्ग;
    पक्ष 'b':			/* Backspace. */
      अक्षरं_लिख ('\b');
      भङ्ग;
    पक्ष 'c':			/* Cancel the rest of the output. */
      निर्गच्छ (EXIT_SUCCESS);
      भङ्ग;
    पक्ष 'e':			/* Escape. */
      अक्षरं_लिख ('\x1B');
      भङ्ग;
    पक्ष 'f':			/* Form feed. */
      अक्षरं_लिख ('\f');
      भङ्ग;
    पक्ष 'n':			/* New line. */
      अक्षरं_लिख ('\n');
      भङ्ग;
    पक्ष 'r':			/* Carriage return. */
      अक्षरं_लिख ('\r');
      भङ्ग;
    पक्ष 't':			/* Horizontal tab. */
      अक्षरं_लिख ('\t');
      भङ्ग;
    पक्ष 'v':			/* Vertical tab. */
      अक्षरं_लिख ('\v');
      भङ्ग;
    स्वभाव:
      अक्षरं_लिख (c);
      भङ्ग;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

स्थिर पूर्णाङ्क
print_esc (वर्ण ध्रुव *escstart, bool octal_0)
{
  वर्ण ध्रुव *p = escstart + 1;
  पूर्णाङ्क esc_मानम् = 0;		/* Value of \nnn escape. */
  पूर्णाङ्क esc_दीर्घता;		/* Length of \nnn escape. */

  यदि (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      प्रति (esc_दीर्घता = 0, ++p;
           esc_दीर्घता < 2 && c_isxdigit (*p);
           ++esc_दीर्घता, ++p)
        esc_मानम् = esc_मानम् * 16 + fromhex (*p);
      यदि (esc_दीर्घता == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      अक्षरं_लिख (esc_मानम्);
    }
  अन्यथा यदि (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      प्रति (esc_दीर्घता = 0, p += octal_0 && *p == '0';
           esc_दीर्घता < 3 && isoct (*p);
           ++esc_दीर्घता, ++p)
        esc_मानम् = esc_मानम् * 8 + fromoct (*p);
      अक्षरं_लिख (esc_मानम्);
    }
  अन्यथा यदि (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  अन्यथा यदि (*p == 'u' || *p == 'U')
    {
      वर्ण esc_char = *p;
      अचिह्नित पूर्णाङ्क uni_मानम्;

      uni_मानम् = 0;
      प्रति (esc_दीर्घता = (esc_char == 'u' ? 4 : 8), ++p;
           esc_दीर्घता > 0;
           --esc_दीर्घता, ++p)
        {
          यदि (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_मानम् = uni_मानम् * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      यदि (uni_मानम् >= 0xd800 && uni_मानम् <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_मानम्);

      print_unicode_char (stdout, uni_मानम्, 0);
    }
  अन्यथा
    {
      अक्षरं_लिख ('\\');
      यदि (*p)
        {
          अक्षरं_लिख (*p);
          p++;
        }
    }
  प्रत्यागम p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

स्थिर शून्य
print_esc_string (वर्ण ध्रुव *str)
{
  प्रति (; *str; str++)
    यदि (*str == '\\')
      str += print_esc (str, true);
    अन्यथा
      अक्षरं_लिख (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

स्थिर शून्य
print_direc (वर्ण ध्रुव *start, वर्ण conversion,
             bool have_क्षेत्रम्_विस्तारः, पूर्णाङ्क क्षेत्रम्_विस्तारः,
             bool have_precision, पूर्णाङ्क precision,
             वर्ण ध्रुव *तर्कः)
{
  वर्ण *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    वर्ण *q;
    वर्ण ध्रुव *दीर्घता_modifier;
    size_t दीर्घता_modifier_len;

    विकल्प (conversion)
      {
      पक्ष 'd': पक्ष 'i': पक्ष 'o': पक्ष 'u': पक्ष 'x': पक्ष 'X':
        दीर्घता_modifier = "j";
        दीर्घता_modifier_len = 1;
        भङ्ग;

      पक्ष 'a': पक्ष 'e': पक्ष 'f': पक्ष 'g':
      पक्ष 'A': पक्ष 'E': पक्ष 'F': पक्ष 'G':
        दीर्घता_modifier = "L";
        दीर्घता_modifier_len = 1;
        भङ्ग;

      स्वभाव:
        दीर्घता_modifier = start;  /* Any valid pointer will do.  */
        दीर्घता_modifier_len = 0;
        भङ्ग;
      }

    size_t दीर्घता = वाक्यदैर्घ्यम् (start);
    p = xmalloc (दीर्घता + दीर्घता_modifier_len + 2);
    q = mempcpy (p, start, दीर्घता);
    q = mempcpy (q, दीर्घता_modifier, दीर्घता_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  विकल्प (conversion)
    {
    पक्ष 'd':
    पक्ष 'i':
      {
        intmax_t arg = तर्कः ? vstrtoimax (तर्कः) : 0;
        यदि (!have_क्षेत्रम्_विस्तारः)
          {
            यदि (!have_precision)
              xprintf (p, arg);
            अन्यथा
              xprintf (p, precision, arg);
          }
        अन्यथा
          {
            यदि (!have_precision)
              xprintf (p, क्षेत्रम्_विस्तारः, arg);
            अन्यथा
              xprintf (p, क्षेत्रम्_विस्तारः, precision, arg);
          }
      }
      भङ्ग;

    पक्ष 'o':
    पक्ष 'u':
    पक्ष 'x':
    पक्ष 'X':
      {
        uintmax_t arg = तर्कः ? vstrtoumax (तर्कः) : 0;
        यदि (!have_क्षेत्रम्_विस्तारः)
          {
            यदि (!have_precision)
              xprintf (p, arg);
            अन्यथा
              xprintf (p, precision, arg);
          }
        अन्यथा
          {
            यदि (!have_precision)
              xprintf (p, क्षेत्रम्_विस्तारः, arg);
            अन्यथा
              xprintf (p, क्षेत्रम्_विस्तारः, precision, arg);
          }
      }
      भङ्ग;

    पक्ष 'a':
    पक्ष 'A':
    पक्ष 'e':
    पक्ष 'E':
    पक्ष 'f':
    पक्ष 'F':
    पक्ष 'g':
    पक्ष 'G':
      {
        दीर्घ द्विगुण arg = तर्कः ? vstrtold (तर्कः) : 0;
        यदि (!have_क्षेत्रम्_विस्तारः)
          {
            यदि (!have_precision)
              xprintf (p, arg);
            अन्यथा
              xprintf (p, precision, arg);
          }
        अन्यथा
          {
            यदि (!have_precision)
              xprintf (p, क्षेत्रम्_विस्तारः, arg);
            अन्यथा
              xprintf (p, क्षेत्रम्_विस्तारः, precision, arg);
          }
      }
      भङ्ग;

    पक्ष 'c':
      {
        वर्ण c = तर्कः ? *तर्कः : '\0';
        यदि (!have_क्षेत्रम्_विस्तारः)
          xprintf (p, c);
        अन्यथा
          xprintf (p, क्षेत्रम्_विस्तारः, c);
      }
      भङ्ग;

    पक्ष 's':
      यदि (!तर्कः)
        तर्कः = "";
      यदि (!have_क्षेत्रम्_विस्तारः)
        {
          यदि (!have_precision)
            xprintf (p, तर्कः);
          अन्यथा
            xprintf (p, precision, तर्कः);
        }
      अन्यथा
        {
          यदि (!have_precision)
            xprintf (p, क्षेत्रम्_विस्तारः, तर्कः);
          अन्यथा
            xprintf (p, क्षेत्रम्_विस्तारः, precision, तर्कः);
        }
      भङ्ग;
    }

  मुक्त (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

संरचना arg_cursor
{
  वर्ण ध्रुव *f;	/* Pointer into 'format'.  */
  पूर्णाङ्क curr_arg;		/* Current offset.  */
  पूर्णाङ्क curr_s_arg;	/* Current sequential offset.  */
  पूर्णाङ्क अन्तः_arg;		/* End arg processed.  */
  पूर्णाङ्क direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_विशुद्धम् स्थिर संरचना arg_cursor
get_curr_arg (पूर्णाङ्क pos, संरचना arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  पूर्णाङ्क arg = 0;
  वर्ण ध्रुव *f = ac.f;
  यदि (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      पूर्णाङ्क a = *f++ - '0';
      प्रति (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_सङ्कलय (&a, a, *f - '0');
        }
      यदि (*f == '$')
        arg = v ? अधिकतम_पूर्णाङ्कः : a;
    }

  यदि (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      यदि (pos == 0)
        ac.direc_arg = arg;
    }
  अन्यथा
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  यदि (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.अन्तः_arg = MAX (ac.अन्तः_arg, arg);
    }
  प्रत्यागम ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

स्थिर पूर्णाङ्क
print_formatted (वर्ण ध्रुव *format, पूर्णाङ्क argc, वर्ण **argv)
{
  संरचना arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.अन्तः_arg = ac.direc_arg = -1;
  वर्ण ध्रुव *direc_start;	/* Start of % directive.  */
  वर्ण *direc;			/* Generated % directive.  */
  वर्ण *pdirec;			/* Pointer to current end of directive.  */
  bool have_क्षेत्रम्_विस्तारः;	/* True if FIELD_WIDTH is valid.  */
  पूर्णाङ्क क्षेत्रम्_विस्तारः = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  पूर्णाङ्क precision = 0;		/* Arg to second '*'.  */
  वर्ण ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (वाक्यदैर्घ्यम् (format) + 1);

  प्रति (ac.f = format; *ac.f; ac.f++)
    {
      विकल्प (*ac.f)
        {
        पक्ष '%':
          direc_start = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_क्षेत्रम्_विस्तारः = have_precision = false;
          यदि (*ac.f == '%')
            {
              अक्षरं_लिख ('%');
              भङ्ग;
            }

          ac = get_curr_arg (0, ac);

          यदि (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              यदि (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              भङ्ग;
            }

          यदि (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              यदि (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              भङ्ग;
            }

          स्मृतिं_पूरय (ok, 0, परिमाण ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          प्रति (;; ac.f++)
            {
              विकल्प (*ac.f)
                {
#यदि (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                पक्ष 'I':
#यदि_अन्तः
                पक्ष '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  भङ्ग;
                पक्ष '-': पक्ष '+': पक्ष ' ':
                  भङ्ग;
                पक्ष '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  भङ्ग;
                पक्ष '0':
                  ok['c'] = ok['s'] = 0;
                  भङ्ग;
                स्वभाव:
                  गच्छ no_अधिकम्_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_अधिकम्_flag_characters:

          यदि (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              यदि (ac.curr_arg < argc)
                {
                  intmax_t विस्तारः = vstrtoimax (argv[ac.curr_arg]);
                  यदि (INT_MIN <= विस्तारः && विस्तारः <= अधिकतम_पूर्णाङ्कः)
                    क्षेत्रम्_विस्तारः = विस्तारः;
                  अन्यथा
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              अन्यथा
                क्षेत्रम्_विस्तारः = 0;
              have_क्षेत्रम्_विस्तारः = true;
            }
          अन्यथा
            यावत् (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          यदि (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              यदि (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  यदि (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      यदि (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      अन्यथा यदि (अधिकतम_पूर्णाङ्कः < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      अन्यथा
                        precision = prec;
                    }
                  अन्यथा
                    precision = 0;
                  have_precision = true;
                }
              अन्यथा
                यावत् (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          यावत् (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            अचिह्नित वर्ण conversion = *ac.f;
            पूर्णाङ्क speclen = MIN (ac.f + 1 - direc_start, अधिकतम_पूर्णाङ्कः);
            यदि (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_start);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_क्षेत्रम्_विस्तारः, क्षेत्रम्_विस्तारः,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : रिक्त);

          भङ्ग;

        पक्ष '\\':
          ac.f += print_esc (ac.f, false);
          भङ्ग;

        स्वभाव:
          अक्षरं_लिख (*ac.f);
        }
    }

  मुक्त (direc);
  प्रत्यागम MIN (argc, ac.अन्तः_arg + 1);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण *format;
  पूर्णाङ्क तर्काः_प्रयुक्तम्;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != रिक्त);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  यदि (argc == 2)
    {
      यदि (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      यदि (streq (argv[1], "--version"))
        {
          संस्करणम्_etc (stdout, क्रमादेशः_NAME, PACKAGE_NAME, संस्करणम्, AUTHORS,
                       (वर्ण *) रिक्त);
          प्रत्यागम EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  यदि (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  यदि (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  कुरु
    {
      तर्काः_प्रयुक्तम् = print_formatted (format, argc, argv);
      argc -= तर्काः_प्रयुक्तम्;
      argv += तर्काः_प्रयुक्तम्;
    }
  यावत् (तर्काः_प्रयुक्तम् > 0 && argc > 0);

  यदि (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  प्रत्यागम exit_स्थितिः;
}
