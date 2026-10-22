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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <wchar.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "cl-strtod.h"
#ಅನ್ತರ್ಭಾವ "octhexdigits.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "unicodeio.h"
#ಅನ್ತರ್ಭಾವ "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printf"

#define AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ exit_status;

/* True if the POSIXLY_CORRECT environment variable is set.  */
ಸ್ಥಿರ bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ cfcc_ಸಂದೇಶ =
 N_("warning: %s: character(s) following character constant have been ignored");

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
  \\\"      ದ್ವಿಗುಣ quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new ಸಾಲು\n\
  \\r      carriage ಪ್ರತ್ಯಾಗಮ\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal ಮೌಲ್ಯ NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal ಮೌಲ್ಯ HH (1 to 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex ಮೌಲ್ಯ HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex ಮೌಲ್ಯ HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      ARGUMENT as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      ARGUMENT is printed in a format that can be reused as shell ಒಳಹರಿವು,\n\
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
  %s          '[%10s]' 'left pad'         [  ಎಡ pad]\n\
  %s          '[%-10s]' 'right pad'       [ಬಲ pad ]\n\
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

ಸ್ಥಿರ ಶೂನ್ಯ
verify_numeric (ವರ್ಣ ಧ್ರುವ *s, ವರ್ಣ ಧ್ರುವ *end)
{
  ಯದಿ (s == end)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_status = EXIT_FAILURE;
    }
  ಅನ್ಯಥಾ ಯದಿ (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_status = EXIT_FAILURE;
    }
  ಅನ್ಯಥಾ ಯದಿ (*end)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_status = EXIT_FAILURE;
    }
}

#define STRTOX(TYPE, FUNC_NAME, LIB_FUNC_ಅಭಿವ್ಯಕ್ತಿ)				 \
ಸ್ಥಿರ TYPE								 \
FUNC_NAME (ವರ್ಣ ಧ್ರುವ *s)						 \
{									 \
  ವರ್ಣ *end;								 \
  TYPE val;								 \
                                                                         \
  ಯದಿ ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      unsigned ವರ್ಣ ch = *++s;						 \
      val = ch;								 \
                                                                         \
      ಯದಿ (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = strlen (s);					 \
          ssize_t bytes;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          bytes = mbrtowc (&wc, s, slen, &mbstate);			 \
          ಯದಿ (0 < bytes)						 \
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
      ಯದಿ (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_ಸಂದೇಶ), s);					 \
    }									 \
  ಅನ್ಯಥಾ									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_ಅಭಿವ್ಯಕ್ತಿ);						 \
      verify_numeric (s, end);						 \
    }									 \
  ಪ್ರತ್ಯಾಗಮ val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &end, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &end, 0))
STRTOX (long ದ್ವಿಗುಣ, vstrtold,   cl_strtold (s, &end))

/* Output a single-character \ escape.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_esc_char (ವರ್ಣ c)
{
  ವಿಕಲ್ಪ (c)
    {
    ಪಕ್ಷ 'a':			/* Alert. */
      putchar ('\a');
      ಭಙ್ಗ;
    ಪಕ್ಷ 'b':			/* Backspace. */
      putchar ('\b');
      ಭಙ್ಗ;
    ಪಕ್ಷ 'c':			/* Cancel the rest of the output. */
      exit (EXIT_SUCCESS);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'e':			/* Escape. */
      putchar ('\x1B');
      ಭಙ್ಗ;
    ಪಕ್ಷ 'f':			/* Form feed. */
      putchar ('\f');
      ಭಙ್ಗ;
    ಪಕ್ಷ 'n':			/* New line. */
      putchar ('\n');
      ಭಙ್ಗ;
    ಪಕ್ಷ 'r':			/* Carriage return. */
      putchar ('\r');
      ಭಙ್ಗ;
    ಪಕ್ಷ 't':			/* Horizontal tab. */
      putchar ('\t');
      ಭಙ್ಗ;
    ಪಕ್ಷ 'v':			/* Vertical tab. */
      putchar ('\v');
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      putchar (c);
      ಭಙ್ಗ;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
print_esc (ವರ್ಣ ಧ್ರುವ *escstart, bool octal_0)
{
  ವರ್ಣ ಧ್ರುವ *p = escstart + 1;
  ಪೂರ್ಣಾಙ್ಕ esc_ಮೌಲ್ಯ = 0;		/* Value of \nnn escape. */
  ಪೂರ್ಣಾಙ್ಕ esc_length;		/* Length of \nnn escape. */

  ಯದಿ (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      ಪ್ರತಿ (esc_length = 0, ++p;
           esc_length < 2 && c_isxdigit (*p);
           ++esc_length, ++p)
        esc_ಮೌಲ್ಯ = esc_ಮೌಲ್ಯ * 16 + fromhex (*p);
      ಯದಿ (esc_length == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      putchar (esc_ಮೌಲ್ಯ);
    }
  ಅನ್ಯಥಾ ಯದಿ (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      ಪ್ರತಿ (esc_length = 0, p += octal_0 && *p == '0';
           esc_length < 3 && isoct (*p);
           ++esc_length, ++p)
        esc_ಮೌಲ್ಯ = esc_ಮೌಲ್ಯ * 8 + fromoct (*p);
      putchar (esc_ಮೌಲ್ಯ);
    }
  ಅನ್ಯಥಾ ಯದಿ (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  ಅನ್ಯಥಾ ಯದಿ (*p == 'u' || *p == 'U')
    {
      ವರ್ಣ esc_char = *p;
      unsigned ಪೂರ್ಣಾಙ್ಕ uni_ಮೌಲ್ಯ;

      uni_ಮೌಲ್ಯ = 0;
      ಪ್ರತಿ (esc_length = (esc_char == 'u' ? 4 : 8), ++p;
           esc_length > 0;
           --esc_length, ++p)
        {
          ಯದಿ (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_ಮೌಲ್ಯ = uni_ಮೌಲ್ಯ * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      ಯದಿ (uni_ಮೌಲ್ಯ >= 0xd800 && uni_ಮೌಲ್ಯ <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_ಮೌಲ್ಯ);

      print_unicode_char (stdout, uni_ಮೌಲ್ಯ, 0);
    }
  ಅನ್ಯಥಾ
    {
      putchar ('\\');
      ಯದಿ (*p)
        {
          putchar (*p);
          p++;
        }
    }
  ಪ್ರತ್ಯಾಗಮ p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_esc_string (ವರ್ಣ ಧ್ರುವ *str)
{
  ಪ್ರತಿ (; *str; str++)
    ಯದಿ (*str == '\\')
      str += print_esc (str, true);
    ಅನ್ಯಥಾ
      putchar (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_direc (ವರ್ಣ ಧ್ರುವ *start, ವರ್ಣ conversion,
             bool have_field_width, ಪೂರ್ಣಾಙ್ಕ field_width,
             bool have_precision, ಪೂರ್ಣಾಙ್ಕ precision,
             ವರ್ಣ ಧ್ರುವ *argument)
{
  ವರ್ಣ *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    ವರ್ಣ *q;
    ವರ್ಣ ಧ್ರುವ *length_modifier;
    size_t length_modifier_len;

    ವಿಕಲ್ಪ (conversion)
      {
      ಪಕ್ಷ 'd': ಪಕ್ಷ 'i': ಪಕ್ಷ 'o': ಪಕ್ಷ 'u': ಪಕ್ಷ 'x': ಪಕ್ಷ 'X':
        length_modifier = "j";
        length_modifier_len = 1;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'a': ಪಕ್ಷ 'e': ಪಕ್ಷ 'f': ಪಕ್ಷ 'g':
      ಪಕ್ಷ 'A': ಪಕ್ಷ 'E': ಪಕ್ಷ 'F': ಪಕ್ಷ 'G':
        length_modifier = "L";
        length_modifier_len = 1;
        ಭಙ್ಗ;

      ಸ್ವಭಾವ:
        length_modifier = start;  /* Any valid pointer will do.  */
        length_modifier_len = 0;
        ಭಙ್ಗ;
      }

    size_t length = strlen (start);
    p = xmalloc (length + length_modifier_len + 2);
    q = mempcpy (p, start, length);
    q = mempcpy (q, length_modifier, length_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  ವಿಕಲ್ಪ (conversion)
    {
    ಪಕ್ಷ 'd':
    ಪಕ್ಷ 'i':
      {
        intmax_t arg = argument ? vstrtoimax (argument) : 0;
        ಯದಿ (!have_field_width)
          {
            ಯದಿ (!have_precision)
              xprintf (p, arg);
            ಅನ್ಯಥಾ
              xprintf (p, precision, arg);
          }
        ಅನ್ಯಥಾ
          {
            ಯದಿ (!have_precision)
              xprintf (p, field_width, arg);
            ಅನ್ಯಥಾ
              xprintf (p, field_width, precision, arg);
          }
      }
      ಭಙ್ಗ;

    ಪಕ್ಷ 'o':
    ಪಕ್ಷ 'u':
    ಪಕ್ಷ 'x':
    ಪಕ್ಷ 'X':
      {
        uintmax_t arg = argument ? vstrtoumax (argument) : 0;
        ಯದಿ (!have_field_width)
          {
            ಯದಿ (!have_precision)
              xprintf (p, arg);
            ಅನ್ಯಥಾ
              xprintf (p, precision, arg);
          }
        ಅನ್ಯಥಾ
          {
            ಯದಿ (!have_precision)
              xprintf (p, field_width, arg);
            ಅನ್ಯಥಾ
              xprintf (p, field_width, precision, arg);
          }
      }
      ಭಙ್ಗ;

    ಪಕ್ಷ 'a':
    ಪಕ್ಷ 'A':
    ಪಕ್ಷ 'e':
    ಪಕ್ಷ 'E':
    ಪಕ್ಷ 'f':
    ಪಕ್ಷ 'F':
    ಪಕ್ಷ 'g':
    ಪಕ್ಷ 'G':
      {
        long ದ್ವಿಗುಣ arg = argument ? vstrtold (argument) : 0;
        ಯದಿ (!have_field_width)
          {
            ಯದಿ (!have_precision)
              xprintf (p, arg);
            ಅನ್ಯಥಾ
              xprintf (p, precision, arg);
          }
        ಅನ್ಯಥಾ
          {
            ಯದಿ (!have_precision)
              xprintf (p, field_width, arg);
            ಅನ್ಯಥಾ
              xprintf (p, field_width, precision, arg);
          }
      }
      ಭಙ್ಗ;

    ಪಕ್ಷ 'c':
      {
        ವರ್ಣ c = argument ? *argument : '\0';
        ಯದಿ (!have_field_width)
          xprintf (p, c);
        ಅನ್ಯಥಾ
          xprintf (p, field_width, c);
      }
      ಭಙ್ಗ;

    ಪಕ್ಷ 's':
      ಯದಿ (!argument)
        argument = "";
      ಯದಿ (!have_field_width)
        {
          ಯದಿ (!have_precision)
            xprintf (p, argument);
          ಅನ್ಯಥಾ
            xprintf (p, precision, argument);
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (!have_precision)
            xprintf (p, field_width, argument);
          ಅನ್ಯಥಾ
            xprintf (p, field_width, precision, argument);
        }
      ಭಙ್ಗ;
    }

  ಮುಕ್ತ (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

ಸಂರಚನಾ arg_cursor
{
  ವರ್ಣ ಧ್ರುವ *f;	/* Pointer into 'format'.  */
  ಪೂರ್ಣಾಙ್ಕ curr_arg;		/* Current offset.  */
  ಪೂರ್ಣಾಙ್ಕ curr_s_arg;	/* Current sequential offset.  */
  ಪೂರ್ಣಾಙ್ಕ end_arg;		/* End arg processed.  */
  ಪೂರ್ಣಾಙ್ಕ direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_PURE ಸ್ಥಿರ ಸಂರಚನಾ arg_cursor
get_curr_arg (ಪೂರ್ಣಾಙ್ಕ pos, ಸಂರಚನಾ arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  ಪೂರ್ಣಾಙ್ಕ arg = 0;
  ವರ್ಣ ಧ್ರುವ *f = ac.f;
  ಯದಿ (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      ಪೂರ್ಣಾಙ್ಕ a = *f++ - '0';
      ಪ್ರತಿ (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_add (&a, a, *f - '0');
        }
      ಯದಿ (*f == '$')
        arg = v ? INT_MAX : a;
    }

  ಯದಿ (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      ಯದಿ (pos == 0)
        ac.direc_arg = arg;
    }
  ಅನ್ಯಥಾ
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  ಯದಿ (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.end_arg = MAX (ac.end_arg, arg);
    }
  ಪ್ರತ್ಯಾಗಮ ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
print_formatted (ವರ್ಣ ಧ್ರುವ *format, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಸಂರಚನಾ arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.end_arg = ac.direc_arg = -1;
  ವರ್ಣ ಧ್ರುವ *direc_start;	/* Start of % directive.  */
  ವರ್ಣ *direc;			/* Generated % directive.  */
  ವರ್ಣ *pdirec;			/* Pointer to current end of directive.  */
  bool have_field_width;	/* True if FIELD_WIDTH is valid.  */
  ಪೂರ್ಣಾಙ್ಕ field_width = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  ಪೂರ್ಣಾಙ್ಕ precision = 0;		/* Arg to second '*'.  */
  ವರ್ಣ ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (strlen (format) + 1);

  ಪ್ರತಿ (ac.f = format; *ac.f; ac.f++)
    {
      ವಿಕಲ್ಪ (*ac.f)
        {
        ಪಕ್ಷ '%':
          direc_start = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_field_width = have_precision = false;
          ಯದಿ (*ac.f == '%')
            {
              putchar ('%');
              ಭಙ್ಗ;
            }

          ac = get_curr_arg (0, ac);

          ಯದಿ (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              ಯದಿ (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              ಭಙ್ಗ;
            }

          ಯದಿ (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              ಯದಿ (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              ಭಙ್ಗ;
            }

          memset (ok, 0, ಪರಿಮಾಣ ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          ಪ್ರತಿ (;; ac.f++)
            {
              ವಿಕಲ್ಪ (*ac.f)
                {
#ಯದಿ (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                ಪಕ್ಷ 'I':
#endif
                ಪಕ್ಷ '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  ಭಙ್ಗ;
                ಪಕ್ಷ '-': ಪಕ್ಷ '+': ಪಕ್ಷ ' ':
                  ಭಙ್ಗ;
                ಪಕ್ಷ '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  ಭಙ್ಗ;
                ಪಕ್ಷ '0':
                  ok['c'] = ok['s'] = 0;
                  ಭಙ್ಗ;
                ಸ್ವಭಾವ:
                  ಗಚ್ಛ no_more_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_more_flag_characters:

          ಯದಿ (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              ಯದಿ (ac.curr_arg < argc)
                {
                  intmax_t width = vstrtoimax (argv[ac.curr_arg]);
                  ಯದಿ (INT_MIN <= width && width <= INT_MAX)
                    field_width = width;
                  ಅನ್ಯಥಾ
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              ಅನ್ಯಥಾ
                field_width = 0;
              have_field_width = true;
            }
          ಅನ್ಯಥಾ
            ಯಾವತ್ (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          ಯದಿ (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              ಯದಿ (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  ಯದಿ (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      ಯದಿ (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      ಅನ್ಯಥಾ ಯದಿ (INT_MAX < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      ಅನ್ಯಥಾ
                        precision = prec;
                    }
                  ಅನ್ಯಥಾ
                    precision = 0;
                  have_precision = true;
                }
              ಅನ್ಯಥಾ
                ಯಾವತ್ (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          ಯಾವತ್ (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            unsigned ವರ್ಣ conversion = *ac.f;
            ಪೂರ್ಣಾಙ್ಕ speclen = MIN (ac.f + 1 - direc_start, INT_MAX);
            ಯದಿ (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_start);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_field_width, field_width,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : ರಿಕ್ತ);

          ಭಙ್ಗ;

        ಪಕ್ಷ '\\':
          ac.f += print_esc (ac.f, false);
          ಭಙ್ಗ;

        ಸ್ವಭಾವ:
          putchar (*ac.f);
        }
    }

  ಮುಕ್ತ (direc);
  ಪ್ರತ್ಯಾಗಮ MIN (argc, ac.end_arg + 1);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ವರ್ಣ *format;
  ಪೂರ್ಣಾಙ್ಕ args_used;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ರಿಕ್ತ);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  ಯದಿ (argc == 2)
    {
      ಯದಿ (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      ಯದಿ (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (ವರ್ಣ *) ರಿಕ್ತ);
          ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  ಯದಿ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ಯದಿ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  ಕುರು
    {
      args_used = print_formatted (format, argc, argv);
      argc -= args_used;
      argv += args_used;
    }
  ಯಾವತ್ (args_used > 0 && argc > 0);

  ಯದಿ (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  ಪ್ರತ್ಯಾಗಮ exit_status;
}
