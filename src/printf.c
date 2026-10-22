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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <wchar.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "cl-strtod.h"
#അന്തര്ഭാവ "octhexdigits.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "unicodeio.h"
#അന്തര്ഭാവ "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printf"

#define AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ exit_status;

/* True if the POSIXLY_CORRECT environment variable is set.  */
സ്ഥിതം bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം cfcc_സന്ദേശം =
 N_("warning: %s: character(s) following character constant have been ignored");

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
  \\\"      ഇരട്ട quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new വരി\n\
  \\r      carriage തിരികെ\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal മൂല്യം NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal മൂല്യം HH (1 to 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex മൂല്യം HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex മൂല്യം HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      ARGUMENT as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      ARGUMENT is printed in a format that can be reused as shell നിവേശം,\n\
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
  %s          '[%10s]' 'left pad'         [  ഇടത് pad]\n\
  %s          '[%-10s]' 'right pad'       [വലത് pad ]\n\
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

സ്ഥിതം ശൂന്യം
verify_numeric (അക്ഷരം സ്ഥിരം *s, അക്ഷരം സ്ഥിരം *end)
{
  എങ്കിൽ (s == end)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_status = EXIT_FAILURE;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_status = EXIT_FAILURE;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (*end)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_status = EXIT_FAILURE;
    }
}

#define STRTOX(TYPE, FUNC_NAME, LIB_FUNC_വ്യഞ്ജകം)				 \
സ്ഥിതം TYPE								 \
FUNC_NAME (അക്ഷരം സ്ഥിരം *s)						 \
{									 \
  അക്ഷരം *end;								 \
  TYPE val;								 \
                                                                         \
  എങ്കിൽ ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      unsigned അക്ഷരം ch = *++s;						 \
      val = ch;								 \
                                                                         \
      എങ്കിൽ (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = strlen (s);					 \
          ssize_t bytes;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          bytes = mbrtowc (&wc, s, slen, &mbstate);			 \
          എങ്കിൽ (0 < bytes)						 \
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
      എങ്കിൽ (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_സന്ദേശം), s);					 \
    }									 \
  അല്ലെങ്കിൽ									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_വ്യഞ്ജകം);						 \
      verify_numeric (s, end);						 \
    }									 \
  തിരികെ val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &end, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &end, 0))
STRTOX (long ഇരട്ട, vstrtold,   cl_strtold (s, &end))

/* Output a single-character \ escape.  */

സ്ഥിതം ശൂന്യം
print_esc_char (അക്ഷരം c)
{
  തിരഞ്ഞെടുപ്പ് (c)
    {
    അവസ്ഥ 'a':			/* Alert. */
      putchar ('\a');
      നിർത്തുക;
    അവസ്ഥ 'b':			/* Backspace. */
      putchar ('\b');
      നിർത്തുക;
    അവസ്ഥ 'c':			/* Cancel the rest of the output. */
      exit (EXIT_SUCCESS);
      നിർത്തുക;
    അവസ്ഥ 'e':			/* Escape. */
      putchar ('\x1B');
      നിർത്തുക;
    അവസ്ഥ 'f':			/* Form feed. */
      putchar ('\f');
      നിർത്തുക;
    അവസ്ഥ 'n':			/* New line. */
      putchar ('\n');
      നിർത്തുക;
    അവസ്ഥ 'r':			/* Carriage return. */
      putchar ('\r');
      നിർത്തുക;
    അവസ്ഥ 't':			/* Horizontal tab. */
      putchar ('\t');
      നിർത്തുക;
    അവസ്ഥ 'v':			/* Vertical tab. */
      putchar ('\v');
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      putchar (c);
      നിർത്തുക;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
print_esc (അക്ഷരം സ്ഥിരം *escstart, bool octal_0)
{
  അക്ഷരം സ്ഥിരം *p = escstart + 1;
  പൂർണ്ണസംഖ്യ esc_മൂല്യം = 0;		/* Value of \nnn escape. */
  പൂർണ്ണസംഖ്യ esc_length;		/* Length of \nnn escape. */

  എങ്കിൽ (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      വേണ്ടി (esc_length = 0, ++p;
           esc_length < 2 && c_isxdigit (*p);
           ++esc_length, ++p)
        esc_മൂല്യം = esc_മൂല്യം * 16 + fromhex (*p);
      എങ്കിൽ (esc_length == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      putchar (esc_മൂല്യം);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      വേണ്ടി (esc_length = 0, p += octal_0 && *p == '0';
           esc_length < 3 && isoct (*p);
           ++esc_length, ++p)
        esc_മൂല്യം = esc_മൂല്യം * 8 + fromoct (*p);
      putchar (esc_മൂല്യം);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  അല്ലെങ്കിൽ എങ്കിൽ (*p == 'u' || *p == 'U')
    {
      അക്ഷരം esc_char = *p;
      unsigned പൂർണ്ണസംഖ്യ uni_മൂല്യം;

      uni_മൂല്യം = 0;
      വേണ്ടി (esc_length = (esc_char == 'u' ? 4 : 8), ++p;
           esc_length > 0;
           --esc_length, ++p)
        {
          എങ്കിൽ (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_മൂല്യം = uni_മൂല്യം * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      എങ്കിൽ (uni_മൂല്യം >= 0xd800 && uni_മൂല്യം <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_മൂല്യം);

      print_unicode_char (stdout, uni_മൂല്യം, 0);
    }
  അല്ലെങ്കിൽ
    {
      putchar ('\\');
      എങ്കിൽ (*p)
        {
          putchar (*p);
          p++;
        }
    }
  തിരികെ p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

സ്ഥിതം ശൂന്യം
print_esc_string (അക്ഷരം സ്ഥിരം *str)
{
  വേണ്ടി (; *str; str++)
    എങ്കിൽ (*str == '\\')
      str += print_esc (str, true);
    അല്ലെങ്കിൽ
      putchar (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

സ്ഥിതം ശൂന്യം
print_direc (അക്ഷരം സ്ഥിരം *start, അക്ഷരം conversion,
             bool have_field_width, പൂർണ്ണസംഖ്യ field_width,
             bool have_precision, പൂർണ്ണസംഖ്യ precision,
             അക്ഷരം സ്ഥിരം *argument)
{
  അക്ഷരം *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    അക്ഷരം *q;
    അക്ഷരം സ്ഥിരം *length_modifier;
    size_t length_modifier_len;

    തിരഞ്ഞെടുപ്പ് (conversion)
      {
      അവസ്ഥ 'd': അവസ്ഥ 'i': അവസ്ഥ 'o': അവസ്ഥ 'u': അവസ്ഥ 'x': അവസ്ഥ 'X':
        length_modifier = "j";
        length_modifier_len = 1;
        നിർത്തുക;

      അവസ്ഥ 'a': അവസ്ഥ 'e': അവസ്ഥ 'f': അവസ്ഥ 'g':
      അവസ്ഥ 'A': അവസ്ഥ 'E': അവസ്ഥ 'F': അവസ്ഥ 'G':
        length_modifier = "L";
        length_modifier_len = 1;
        നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
        length_modifier = start;  /* Any valid pointer will do.  */
        length_modifier_len = 0;
        നിർത്തുക;
      }

    size_t length = strlen (start);
    p = xmalloc (length + length_modifier_len + 2);
    q = mempcpy (p, start, length);
    q = mempcpy (q, length_modifier, length_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  തിരഞ്ഞെടുപ്പ് (conversion)
    {
    അവസ്ഥ 'd':
    അവസ്ഥ 'i':
      {
        intmax_t arg = argument ? vstrtoimax (argument) : 0;
        എങ്കിൽ (!have_field_width)
          {
            എങ്കിൽ (!have_precision)
              xprintf (p, arg);
            അല്ലെങ്കിൽ
              xprintf (p, precision, arg);
          }
        അല്ലെങ്കിൽ
          {
            എങ്കിൽ (!have_precision)
              xprintf (p, field_width, arg);
            അല്ലെങ്കിൽ
              xprintf (p, field_width, precision, arg);
          }
      }
      നിർത്തുക;

    അവസ്ഥ 'o':
    അവസ്ഥ 'u':
    അവസ്ഥ 'x':
    അവസ്ഥ 'X':
      {
        uintmax_t arg = argument ? vstrtoumax (argument) : 0;
        എങ്കിൽ (!have_field_width)
          {
            എങ്കിൽ (!have_precision)
              xprintf (p, arg);
            അല്ലെങ്കിൽ
              xprintf (p, precision, arg);
          }
        അല്ലെങ്കിൽ
          {
            എങ്കിൽ (!have_precision)
              xprintf (p, field_width, arg);
            അല്ലെങ്കിൽ
              xprintf (p, field_width, precision, arg);
          }
      }
      നിർത്തുക;

    അവസ്ഥ 'a':
    അവസ്ഥ 'A':
    അവസ്ഥ 'e':
    അവസ്ഥ 'E':
    അവസ്ഥ 'f':
    അവസ്ഥ 'F':
    അവസ്ഥ 'g':
    അവസ്ഥ 'G':
      {
        long ഇരട്ട arg = argument ? vstrtold (argument) : 0;
        എങ്കിൽ (!have_field_width)
          {
            എങ്കിൽ (!have_precision)
              xprintf (p, arg);
            അല്ലെങ്കിൽ
              xprintf (p, precision, arg);
          }
        അല്ലെങ്കിൽ
          {
            എങ്കിൽ (!have_precision)
              xprintf (p, field_width, arg);
            അല്ലെങ്കിൽ
              xprintf (p, field_width, precision, arg);
          }
      }
      നിർത്തുക;

    അവസ്ഥ 'c':
      {
        അക്ഷരം c = argument ? *argument : '\0';
        എങ്കിൽ (!have_field_width)
          xprintf (p, c);
        അല്ലെങ്കിൽ
          xprintf (p, field_width, c);
      }
      നിർത്തുക;

    അവസ്ഥ 's':
      എങ്കിൽ (!argument)
        argument = "";
      എങ്കിൽ (!have_field_width)
        {
          എങ്കിൽ (!have_precision)
            xprintf (p, argument);
          അല്ലെങ്കിൽ
            xprintf (p, precision, argument);
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (!have_precision)
            xprintf (p, field_width, argument);
          അല്ലെങ്കിൽ
            xprintf (p, field_width, precision, argument);
        }
      നിർത്തുക;
    }

  സ്വതന്ത്രം (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

ഘടന arg_cursor
{
  അക്ഷരം സ്ഥിരം *f;	/* Pointer into 'format'.  */
  പൂർണ്ണസംഖ്യ curr_arg;		/* Current offset.  */
  പൂർണ്ണസംഖ്യ curr_s_arg;	/* Current sequential offset.  */
  പൂർണ്ണസംഖ്യ end_arg;		/* End arg processed.  */
  പൂർണ്ണസംഖ്യ direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_PURE സ്ഥിതം ഘടന arg_cursor
get_curr_arg (പൂർണ്ണസംഖ്യ pos, ഘടന arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  പൂർണ്ണസംഖ്യ arg = 0;
  അക്ഷരം സ്ഥിരം *f = ac.f;
  എങ്കിൽ (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      പൂർണ്ണസംഖ്യ a = *f++ - '0';
      വേണ്ടി (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_add (&a, a, *f - '0');
        }
      എങ്കിൽ (*f == '$')
        arg = v ? INT_MAX : a;
    }

  എങ്കിൽ (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      എങ്കിൽ (pos == 0)
        ac.direc_arg = arg;
    }
  അല്ലെങ്കിൽ
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  എങ്കിൽ (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.end_arg = MAX (ac.end_arg, arg);
    }
  തിരികെ ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
print_formatted (അക്ഷരം സ്ഥിരം *format, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  ഘടന arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.end_arg = ac.direc_arg = -1;
  അക്ഷരം സ്ഥിരം *direc_start;	/* Start of % directive.  */
  അക്ഷരം *direc;			/* Generated % directive.  */
  അക്ഷരം *pdirec;			/* Pointer to current end of directive.  */
  bool have_field_width;	/* True if FIELD_WIDTH is valid.  */
  പൂർണ്ണസംഖ്യ field_width = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  പൂർണ്ണസംഖ്യ precision = 0;		/* Arg to second '*'.  */
  അക്ഷരം ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (strlen (format) + 1);

  വേണ്ടി (ac.f = format; *ac.f; ac.f++)
    {
      തിരഞ്ഞെടുപ്പ് (*ac.f)
        {
        അവസ്ഥ '%':
          direc_start = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_field_width = have_precision = false;
          എങ്കിൽ (*ac.f == '%')
            {
              putchar ('%');
              നിർത്തുക;
            }

          ac = get_curr_arg (0, ac);

          എങ്കിൽ (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              എങ്കിൽ (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              നിർത്തുക;
            }

          എങ്കിൽ (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              എങ്കിൽ (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              നിർത്തുക;
            }

          memset (ok, 0, വലുപ്പം ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          വേണ്ടി (;; ac.f++)
            {
              തിരഞ്ഞെടുപ്പ് (*ac.f)
                {
#എങ്കിൽ (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                അവസ്ഥ 'I':
#endif
                അവസ്ഥ '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  നിർത്തുക;
                അവസ്ഥ '-': അവസ്ഥ '+': അവസ്ഥ ' ':
                  നിർത്തുക;
                അവസ്ഥ '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  നിർത്തുക;
                അവസ്ഥ '0':
                  ok['c'] = ok['s'] = 0;
                  നിർത്തുക;
                സ്ഥിരസ്ഥിതി:
                  പോകുക no_more_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_more_flag_characters:

          എങ്കിൽ (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              എങ്കിൽ (ac.curr_arg < argc)
                {
                  intmax_t width = vstrtoimax (argv[ac.curr_arg]);
                  എങ്കിൽ (INT_MIN <= width && width <= INT_MAX)
                    field_width = width;
                  അല്ലെങ്കിൽ
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              അല്ലെങ്കിൽ
                field_width = 0;
              have_field_width = true;
            }
          അല്ലെങ്കിൽ
            ആകുമ്പോൾ (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          എങ്കിൽ (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              എങ്കിൽ (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  എങ്കിൽ (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      എങ്കിൽ (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      അല്ലെങ്കിൽ എങ്കിൽ (INT_MAX < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      അല്ലെങ്കിൽ
                        precision = prec;
                    }
                  അല്ലെങ്കിൽ
                    precision = 0;
                  have_precision = true;
                }
              അല്ലെങ്കിൽ
                ആകുമ്പോൾ (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          ആകുമ്പോൾ (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            unsigned അക്ഷരം conversion = *ac.f;
            പൂർണ്ണസംഖ്യ speclen = MIN (ac.f + 1 - direc_start, INT_MAX);
            എങ്കിൽ (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_start);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_field_width, field_width,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : ശൂന്യ);

          നിർത്തുക;

        അവസ്ഥ '\\':
          ac.f += print_esc (ac.f, false);
          നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
          putchar (*ac.f);
        }
    }

  സ്വതന്ത്രം (direc);
  തിരികെ MIN (argc, ac.end_arg + 1);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  അക്ഷരം *format;
  പൂർണ്ണസംഖ്യ args_used;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ശൂന്യ);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  എങ്കിൽ (argc == 2)
    {
      എങ്കിൽ (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      എങ്കിൽ (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (അക്ഷരം *) ശൂന്യ);
          തിരികെ EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  എങ്കിൽ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  എങ്കിൽ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  ചെയ്യുക
    {
      args_used = print_formatted (format, argc, argv);
      argc -= args_used;
      argv += args_used;
    }
  ആകുമ്പോൾ (args_used > 0 && argc > 0);

  എങ്കിൽ (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  തിരികെ exit_status;
}
