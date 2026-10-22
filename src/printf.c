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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <wchar.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "cl-strtod.h"
#અન્તર્ભાવ "octhexdigits.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "unicodeio.h"
#અન્તર્ભાવ "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printf"

#define AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
સ્થિર પૂર્ણાંક exit_status;

/* True if the POSIXLY_CORRECT environment variable is set.  */
સ્થિર bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
સ્થિર અક્ષર અચલ *અચલ cfcc_સંદેશ =
 N_("warning: %s: character(s) following character constant have been ignored");

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
  \\\"      દ્વિગુણ quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new લીટી\n\
  \\r      carriage પાછા\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal કિંમત NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal કિંમત HH (1 to 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex કિંમત HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex કિંમત HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      ARGUMENT as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      ARGUMENT is printed in a format that can be reused as shell આગત,\n\
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
  %s          '[%10s]' 'left pad'         [  ડાબું pad]\n\
  %s          '[%-10s]' 'right pad'       [જમણું pad ]\n\
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

સ્થિર શૂન્ય
verify_numeric (અક્ષર અચલ *s, અક્ષર અચલ *end)
{
  જો (s == end)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_status = EXIT_FAILURE;
    }
  નહીંતર જો (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_status = EXIT_FAILURE;
    }
  નહીંતર જો (*end)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_status = EXIT_FAILURE;
    }
}

#define STRTOX(TYPE, FUNC_NAME, LIB_FUNC_પદાવલિ)				 \
સ્થિર TYPE								 \
FUNC_NAME (અક્ષર અચલ *s)						 \
{									 \
  અક્ષર *end;								 \
  TYPE val;								 \
                                                                         \
  જો ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      unsigned અક્ષર ch = *++s;						 \
      val = ch;								 \
                                                                         \
      જો (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = strlen (s);					 \
          ssize_t bytes;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          bytes = mbrtowc (&wc, s, slen, &mbstate);			 \
          જો (0 < bytes)						 \
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
      જો (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_સંદેશ), s);					 \
    }									 \
  નહીંતર									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_પદાવલિ);						 \
      verify_numeric (s, end);						 \
    }									 \
  પાછા val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &end, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &end, 0))
STRTOX (long દ્વિગુણ, vstrtold,   cl_strtold (s, &end))

/* Output a single-character \ escape.  */

સ્થિર શૂન્ય
print_esc_char (અક્ષર c)
{
  પસંદગી (c)
    {
    સ્થિતિ 'a':			/* Alert. */
      putchar ('\a');
      તોડો;
    સ્થિતિ 'b':			/* Backspace. */
      putchar ('\b');
      તોડો;
    સ્થિતિ 'c':			/* Cancel the rest of the output. */
      exit (EXIT_SUCCESS);
      તોડો;
    સ્થિતિ 'e':			/* Escape. */
      putchar ('\x1B');
      તોડો;
    સ્થિતિ 'f':			/* Form feed. */
      putchar ('\f');
      તોડો;
    સ્થિતિ 'n':			/* New line. */
      putchar ('\n');
      તોડો;
    સ્થિતિ 'r':			/* Carriage return. */
      putchar ('\r');
      તોડો;
    સ્થિતિ 't':			/* Horizontal tab. */
      putchar ('\t');
      તોડો;
    સ્થિતિ 'v':			/* Vertical tab. */
      putchar ('\v');
      તોડો;
    મૂળભૂત:
      putchar (c);
      તોડો;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

સ્થિર પૂર્ણાંક
print_esc (અક્ષર અચલ *escstart, bool octal_0)
{
  અક્ષર અચલ *p = escstart + 1;
  પૂર્ણાંક esc_કિંમત = 0;		/* Value of \nnn escape. */
  પૂર્ણાંક esc_length;		/* Length of \nnn escape. */

  જો (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      માટે (esc_length = 0, ++p;
           esc_length < 2 && c_isxdigit (*p);
           ++esc_length, ++p)
        esc_કિંમત = esc_કિંમત * 16 + fromhex (*p);
      જો (esc_length == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      putchar (esc_કિંમત);
    }
  નહીંતર જો (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      માટે (esc_length = 0, p += octal_0 && *p == '0';
           esc_length < 3 && isoct (*p);
           ++esc_length, ++p)
        esc_કિંમત = esc_કિંમત * 8 + fromoct (*p);
      putchar (esc_કિંમત);
    }
  નહીંતર જો (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  નહીંતર જો (*p == 'u' || *p == 'U')
    {
      અક્ષર esc_char = *p;
      unsigned પૂર્ણાંક uni_કિંમત;

      uni_કિંમત = 0;
      માટે (esc_length = (esc_char == 'u' ? 4 : 8), ++p;
           esc_length > 0;
           --esc_length, ++p)
        {
          જો (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_કિંમત = uni_કિંમત * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      જો (uni_કિંમત >= 0xd800 && uni_કિંમત <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_કિંમત);

      print_unicode_char (stdout, uni_કિંમત, 0);
    }
  નહીંતર
    {
      putchar ('\\');
      જો (*p)
        {
          putchar (*p);
          p++;
        }
    }
  પાછા p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

સ્થિર શૂન્ય
print_esc_string (અક્ષર અચલ *str)
{
  માટે (; *str; str++)
    જો (*str == '\\')
      str += print_esc (str, true);
    નહીંતર
      putchar (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

સ્થિર શૂન્ય
print_direc (અક્ષર અચલ *start, અક્ષર conversion,
             bool have_field_width, પૂર્ણાંક field_width,
             bool have_precision, પૂર્ણાંક precision,
             અક્ષર અચલ *argument)
{
  અક્ષર *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    અક્ષર *q;
    અક્ષર અચલ *length_modifier;
    size_t length_modifier_len;

    પસંદગી (conversion)
      {
      સ્થિતિ 'd': સ્થિતિ 'i': સ્થિતિ 'o': સ્થિતિ 'u': સ્થિતિ 'x': સ્થિતિ 'X':
        length_modifier = "j";
        length_modifier_len = 1;
        તોડો;

      સ્થિતિ 'a': સ્થિતિ 'e': સ્થિતિ 'f': સ્થિતિ 'g':
      સ્થિતિ 'A': સ્થિતિ 'E': સ્થિતિ 'F': સ્થિતિ 'G':
        length_modifier = "L";
        length_modifier_len = 1;
        તોડો;

      મૂળભૂત:
        length_modifier = start;  /* Any valid pointer will do.  */
        length_modifier_len = 0;
        તોડો;
      }

    size_t length = strlen (start);
    p = xmalloc (length + length_modifier_len + 2);
    q = mempcpy (p, start, length);
    q = mempcpy (q, length_modifier, length_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  પસંદગી (conversion)
    {
    સ્થિતિ 'd':
    સ્થિતિ 'i':
      {
        intmax_t arg = argument ? vstrtoimax (argument) : 0;
        જો (!have_field_width)
          {
            જો (!have_precision)
              xprintf (p, arg);
            નહીંતર
              xprintf (p, precision, arg);
          }
        નહીંતર
          {
            જો (!have_precision)
              xprintf (p, field_width, arg);
            નહીંતર
              xprintf (p, field_width, precision, arg);
          }
      }
      તોડો;

    સ્થિતિ 'o':
    સ્થિતિ 'u':
    સ્થિતિ 'x':
    સ્થિતિ 'X':
      {
        uintmax_t arg = argument ? vstrtoumax (argument) : 0;
        જો (!have_field_width)
          {
            જો (!have_precision)
              xprintf (p, arg);
            નહીંતર
              xprintf (p, precision, arg);
          }
        નહીંતર
          {
            જો (!have_precision)
              xprintf (p, field_width, arg);
            નહીંતર
              xprintf (p, field_width, precision, arg);
          }
      }
      તોડો;

    સ્થિતિ 'a':
    સ્થિતિ 'A':
    સ્થિતિ 'e':
    સ્થિતિ 'E':
    સ્થિતિ 'f':
    સ્થિતિ 'F':
    સ્થિતિ 'g':
    સ્થિતિ 'G':
      {
        long દ્વિગુણ arg = argument ? vstrtold (argument) : 0;
        જો (!have_field_width)
          {
            જો (!have_precision)
              xprintf (p, arg);
            નહીંતર
              xprintf (p, precision, arg);
          }
        નહીંતર
          {
            જો (!have_precision)
              xprintf (p, field_width, arg);
            નહીંતર
              xprintf (p, field_width, precision, arg);
          }
      }
      તોડો;

    સ્થિતિ 'c':
      {
        અક્ષર c = argument ? *argument : '\0';
        જો (!have_field_width)
          xprintf (p, c);
        નહીંતર
          xprintf (p, field_width, c);
      }
      તોડો;

    સ્થિતિ 's':
      જો (!argument)
        argument = "";
      જો (!have_field_width)
        {
          જો (!have_precision)
            xprintf (p, argument);
          નહીંતર
            xprintf (p, precision, argument);
        }
      નહીંતર
        {
          જો (!have_precision)
            xprintf (p, field_width, argument);
          નહીંતર
            xprintf (p, field_width, precision, argument);
        }
      તોડો;
    }

  મુક્ત (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

સંરચના arg_cursor
{
  અક્ષર અચલ *f;	/* Pointer into 'format'.  */
  પૂર્ણાંક curr_arg;		/* Current offset.  */
  પૂર્ણાંક curr_s_arg;	/* Current sequential offset.  */
  પૂર્ણાંક end_arg;		/* End arg processed.  */
  પૂર્ણાંક direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_PURE સ્થિર સંરચના arg_cursor
get_curr_arg (પૂર્ણાંક pos, સંરચના arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  પૂર્ણાંક arg = 0;
  અક્ષર અચલ *f = ac.f;
  જો (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      પૂર્ણાંક a = *f++ - '0';
      માટે (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_add (&a, a, *f - '0');
        }
      જો (*f == '$')
        arg = v ? INT_MAX : a;
    }

  જો (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      જો (pos == 0)
        ac.direc_arg = arg;
    }
  નહીંતર
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  જો (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.end_arg = MAX (ac.end_arg, arg);
    }
  પાછા ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

સ્થિર પૂર્ણાંક
print_formatted (અક્ષર અચલ *format, પૂર્ણાંક argc, અક્ષર **argv)
{
  સંરચના arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.end_arg = ac.direc_arg = -1;
  અક્ષર અચલ *direc_start;	/* Start of % directive.  */
  અક્ષર *direc;			/* Generated % directive.  */
  અક્ષર *pdirec;			/* Pointer to current end of directive.  */
  bool have_field_width;	/* True if FIELD_WIDTH is valid.  */
  પૂર્ણાંક field_width = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  પૂર્ણાંક precision = 0;		/* Arg to second '*'.  */
  અક્ષર ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (strlen (format) + 1);

  માટે (ac.f = format; *ac.f; ac.f++)
    {
      પસંદગી (*ac.f)
        {
        સ્થિતિ '%':
          direc_start = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_field_width = have_precision = false;
          જો (*ac.f == '%')
            {
              putchar ('%');
              તોડો;
            }

          ac = get_curr_arg (0, ac);

          જો (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              જો (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              તોડો;
            }

          જો (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              જો (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              તોડો;
            }

          memset (ok, 0, કદ ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          માટે (;; ac.f++)
            {
              પસંદગી (*ac.f)
                {
#જો (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                સ્થિતિ 'I':
#endif
                સ્થિતિ '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  તોડો;
                સ્થિતિ '-': સ્થિતિ '+': સ્થિતિ ' ':
                  તોડો;
                સ્થિતિ '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  તોડો;
                સ્થિતિ '0':
                  ok['c'] = ok['s'] = 0;
                  તોડો;
                મૂળભૂત:
                  જાઓ no_more_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_more_flag_characters:

          જો (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              જો (ac.curr_arg < argc)
                {
                  intmax_t width = vstrtoimax (argv[ac.curr_arg]);
                  જો (INT_MIN <= width && width <= INT_MAX)
                    field_width = width;
                  નહીંતર
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              નહીંતર
                field_width = 0;
              have_field_width = true;
            }
          નહીંતર
            જ્યારે (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          જો (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              જો (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  જો (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      જો (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      નહીંતર જો (INT_MAX < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      નહીંતર
                        precision = prec;
                    }
                  નહીંતર
                    precision = 0;
                  have_precision = true;
                }
              નહીંતર
                જ્યારે (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          જ્યારે (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            unsigned અક્ષર conversion = *ac.f;
            પૂર્ણાંક speclen = MIN (ac.f + 1 - direc_start, INT_MAX);
            જો (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_start);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_field_width, field_width,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : ખાલી);

          તોડો;

        સ્થિતિ '\\':
          ac.f += print_esc (ac.f, false);
          તોડો;

        મૂળભૂત:
          putchar (*ac.f);
        }
    }

  મુક્ત (direc);
  પાછા MIN (argc, ac.end_arg + 1);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  અક્ષર *format;
  પૂર્ણાંક args_used;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ખાલી);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  જો (argc == 2)
    {
      જો (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      જો (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (અક્ષર *) ખાલી);
          પાછા EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  જો (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  જો (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  કરો
    {
      args_used = print_formatted (format, argc, argv);
      argc -= args_used;
      argv += args_used;
    }
  જ્યારે (args_used > 0 && argc > 0);

  જો (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  પાછા exit_status;
}
