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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "printf"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ;

/* True if the POSIXLY_CORRECT environment variable is set.  */
ಸ್ಥಿರ bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ cfcc_ಸಂದೇಶ =
 N_("warning: %s: character(s) following character constant have been ignored");

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s FORMAT [ವಾದ_2]...\n\
  or:  %s OPTION\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print ವಾದ_2(s) according ಗಮ್ಯ FORMAT, or execute according ಗಮ್ಯ OPTION:\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FORMAT controls the output as ಒಳ C ಬರೆ.  Interpreted sequences are:\n\
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
  \\r      carriage ಹಿಂತಿರುಗಿ\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal ಮೌಲ್ಯ NNN (1 ಗಮ್ಯ 3 digits)\n\
  \\xHH    byte with hexadecimal ಮೌಲ್ಯ HH (1 ಗಮ್ಯ 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex ಮೌಲ್ಯ HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex ಮೌಲ್ಯ HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      ವಾದ_2 as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      ವಾದ_2 is printed ಒಳ a format that can be reused as shell ಒಳಹರಿವು,\n\
          escaping non-printable characters with the POSIX $'' syntax\
\n\n\
and all C ಬರೆ(3) format specifications ending with one ದ diouxXfaAeEgGcs,\n\
with ARGUMENTs converted ಗಮ್ಯ proper type ಮೊದಲ.\n\
"), stdout);
      fputs (_("\
FORMAT is reused ಗಮ್ಯ convert all specified arguments.\n\
"), stdout);
      fputs (_("\
The following ಕೋಷ್ಟಕ shows supported conversion specifiers,\n\
with examples including usage ದ ಅಗಲ, precision, and flags.\n\
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
      ಬರೆ (USAGE_BUILTIN_WARNING, ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಸ್ಥಿರ ಶೂನ್ಯ
verify_numeric (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೊನೆ)
{
  ಆದರೆ (s == ಕೊನೆ)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಕೊನೆ)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
    }
}

#ವ್ಯಾಖ್ಯಾನಿಸು STRTOX(TYPE, FUNC_NAME, LIB_FUNC_ಅಭಿವ್ಯಕ್ತಿ)				 \
ಸ್ಥಿರ TYPE								 \
FUNC_NAME (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)						 \
{									 \
  ಅಕ್ಷರ *ಕೊನೆ;								 \
  TYPE val;								 \
                                                                         \
  ಆದರೆ ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch = *++s;						 \
      val = ch;								 \
                                                                         \
      ಆದರೆ (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (s);					 \
          ssize_t ಬೈಟುಗಳು;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          ಬೈಟುಗಳು = mbrtowc (&wc, s, slen, &mbstate);			 \
          ಆದರೆ (0 < ಬೈಟುಗಳು)						 \
            {								 \
              val = wc;							 \
              s += ಬೈಟುಗಳು - 1;						 \
            }								 \
        }								 \
                                                                         \
      /* If POSIXLY_CORRECT is not set, then give a warning that there	 \
         are characters following the character constant and that GNU	 \
         printf is ignoring those characters.  If POSIXLY_CORRECT *is*	 \
         set, then don't give the warning.  */				 \
      ಆದರೆ (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_ಸಂದೇಶ), s);					 \
    }									 \
  ಇಲ್ಲದಿದ್ದರೆ									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_ಅಭಿವ್ಯಕ್ತಿ);						 \
      verify_numeric (s, ಕೊನೆ);						 \
    }									 \
  ಹಿಂತಿರುಗಿ val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &ಕೊನೆ, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &ಕೊನೆ, 0))
STRTOX (ದೀರ್ಘ ದ್ವಿಗುಣ, vstrtold,   cl_strtold (s, &ಕೊನೆ))

/* Output a single-character \ escape.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_esc_char (ಅಕ್ಷರ c)
{
  ಆಯ್ಕೆ (c)
    {
    ಸ್ಥಿತಿ 'a':			/* Alert. */
      ಅಕ್ಷರ_ಬರೆ ('\a');
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'b':			/* Backspace. */
      ಅಕ್ಷರ_ಬರೆ ('\b');
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'c':			/* Cancel the rest of the output. */
      ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'e':			/* Escape. */
      ಅಕ್ಷರ_ಬರೆ ('\x1B');
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'f':			/* Form feed. */
      ಅಕ್ಷರ_ಬರೆ ('\f');
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'n':			/* New line. */
      ಅಕ್ಷರ_ಬರೆ ('\n');
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'r':			/* Carriage return. */
      ಅಕ್ಷರ_ಬರೆ ('\r');
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 't':			/* Horizontal tab. */
      ಅಕ್ಷರ_ಬರೆ ('\t');
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'v':			/* Vertical tab. */
      ಅಕ್ಷರ_ಬರೆ ('\v');
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಅಕ್ಷರ_ಬರೆ (c);
      ನಿಲ್ಲಿಸು;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
print_esc (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *escstart, bool octal_0)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = escstart + 1;
  ಪೂರ್ಣಾಂಕ esc_ಮೌಲ್ಯ = 0;		/* Value of \nnn escape. */
  ಪೂರ್ಣಾಂಕ esc_ಉದ್ದಳತೆ;		/* Length of \nnn escape. */

  ಆದರೆ (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      ಗಾಗಿ (esc_ಉದ್ದಳತೆ = 0, ++p;
           esc_ಉದ್ದಳತೆ < 2 && c_isxdigit (*p);
           ++esc_ಉದ್ದಳತೆ, ++p)
        esc_ಮೌಲ್ಯ = esc_ಮೌಲ್ಯ * 16 + fromhex (*p);
      ಆದರೆ (esc_ಉದ್ದಳತೆ == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      ಅಕ್ಷರ_ಬರೆ (esc_ಮೌಲ್ಯ);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      ಗಾಗಿ (esc_ಉದ್ದಳತೆ = 0, p += octal_0 && *p == '0';
           esc_ಉದ್ದಳತೆ < 3 && isoct (*p);
           ++esc_ಉದ್ದಳತೆ, ++p)
        esc_ಮೌಲ್ಯ = esc_ಮೌಲ್ಯ * 8 + fromoct (*p);
      ಅಕ್ಷರ_ಬರೆ (esc_ಮೌಲ್ಯ);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*p == 'u' || *p == 'U')
    {
      ಅಕ್ಷರ esc_char = *p;
      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ uni_ಮೌಲ್ಯ;

      uni_ಮೌಲ್ಯ = 0;
      ಗಾಗಿ (esc_ಉದ್ದಳತೆ = (esc_char == 'u' ? 4 : 8), ++p;
           esc_ಉದ್ದಳತೆ > 0;
           --esc_ಉದ್ದಳತೆ, ++p)
        {
          ಆದರೆ (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_ಮೌಲ್ಯ = uni_ಮೌಲ್ಯ * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      ಆದರೆ (uni_ಮೌಲ್ಯ >= 0xd800 && uni_ಮೌಲ್ಯ <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_ಮೌಲ್ಯ);

      print_unicode_char (stdout, uni_ಮೌಲ್ಯ, 0);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ_ಬರೆ ('\\');
      ಆದರೆ (*p)
        {
          ಅಕ್ಷರ_ಬರೆ (*p);
          p++;
        }
    }
  ಹಿಂತಿರುಗಿ p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_esc_string (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  ಗಾಗಿ (; *str; str++)
    ಆದರೆ (*str == '\\')
      str += print_esc (str, true);
    ಇಲ್ಲದಿದ್ದರೆ
      ಅಕ್ಷರ_ಬರೆ (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_direc (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಆರಂಭ, ಅಕ್ಷರ conversion,
             bool have_ಕ್ಷೇತ್ರ_ಅಗಲ, ಪೂರ್ಣಾಂಕ ಕ್ಷೇತ್ರ_ಅಗಲ,
             bool have_precision, ಪೂರ್ಣಾಂಕ precision,
             ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ವಾದ)
{
  ಅಕ್ಷರ *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    ಅಕ್ಷರ *q;
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಉದ್ದಳತೆ_modifier;
    size_t ಉದ್ದಳತೆ_modifier_len;

    ಆಯ್ಕೆ (conversion)
      {
      ಸ್ಥಿತಿ 'd': ಸ್ಥಿತಿ 'i': ಸ್ಥಿತಿ 'o': ಸ್ಥಿತಿ 'u': ಸ್ಥಿತಿ 'x': ಸ್ಥಿತಿ 'X':
        ಉದ್ದಳತೆ_modifier = "j";
        ಉದ್ದಳತೆ_modifier_len = 1;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'a': ಸ್ಥಿತಿ 'e': ಸ್ಥಿತಿ 'f': ಸ್ಥಿತಿ 'g':
      ಸ್ಥಿತಿ 'A': ಸ್ಥಿತಿ 'E': ಸ್ಥಿತಿ 'F': ಸ್ಥಿತಿ 'G':
        ಉದ್ದಳತೆ_modifier = "L";
        ಉದ್ದಳತೆ_modifier_len = 1;
        ನಿಲ್ಲಿಸು;

      ಪೂರ್ವನಿಯೋಜಿತ:
        ಉದ್ದಳತೆ_modifier = ಆರಂಭ;  /* Any valid pointer will do.  */
        ಉದ್ದಳತೆ_modifier_len = 0;
        ನಿಲ್ಲಿಸು;
      }

    size_t ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಆರಂಭ);
    p = xmalloc (ಉದ್ದಳತೆ + ಉದ್ದಳತೆ_modifier_len + 2);
    q = mempcpy (p, ಆರಂಭ, ಉದ್ದಳತೆ);
    q = mempcpy (q, ಉದ್ದಳತೆ_modifier, ಉದ್ದಳತೆ_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  ಆಯ್ಕೆ (conversion)
    {
    ಸ್ಥಿತಿ 'd':
    ಸ್ಥಿತಿ 'i':
      {
        intmax_t arg = ವಾದ ? vstrtoimax (ವಾದ) : 0;
        ಆದರೆ (!have_ಕ್ಷೇತ್ರ_ಅಗಲ)
          {
            ಆದರೆ (!have_precision)
              xprintf (p, arg);
            ಇಲ್ಲದಿದ್ದರೆ
              xprintf (p, precision, arg);
          }
        ಇಲ್ಲದಿದ್ದರೆ
          {
            ಆದರೆ (!have_precision)
              xprintf (p, ಕ್ಷೇತ್ರ_ಅಗಲ, arg);
            ಇಲ್ಲದಿದ್ದರೆ
              xprintf (p, ಕ್ಷೇತ್ರ_ಅಗಲ, precision, arg);
          }
      }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'o':
    ಸ್ಥಿತಿ 'u':
    ಸ್ಥಿತಿ 'x':
    ಸ್ಥಿತಿ 'X':
      {
        uintmax_t arg = ವಾದ ? vstrtoumax (ವಾದ) : 0;
        ಆದರೆ (!have_ಕ್ಷೇತ್ರ_ಅಗಲ)
          {
            ಆದರೆ (!have_precision)
              xprintf (p, arg);
            ಇಲ್ಲದಿದ್ದರೆ
              xprintf (p, precision, arg);
          }
        ಇಲ್ಲದಿದ್ದರೆ
          {
            ಆದರೆ (!have_precision)
              xprintf (p, ಕ್ಷೇತ್ರ_ಅಗಲ, arg);
            ಇಲ್ಲದಿದ್ದರೆ
              xprintf (p, ಕ್ಷೇತ್ರ_ಅಗಲ, precision, arg);
          }
      }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'a':
    ಸ್ಥಿತಿ 'A':
    ಸ್ಥಿತಿ 'e':
    ಸ್ಥಿತಿ 'E':
    ಸ್ಥಿತಿ 'f':
    ಸ್ಥಿತಿ 'F':
    ಸ್ಥಿತಿ 'g':
    ಸ್ಥಿತಿ 'G':
      {
        ದೀರ್ಘ ದ್ವಿಗುಣ arg = ವಾದ ? vstrtold (ವಾದ) : 0;
        ಆದರೆ (!have_ಕ್ಷೇತ್ರ_ಅಗಲ)
          {
            ಆದರೆ (!have_precision)
              xprintf (p, arg);
            ಇಲ್ಲದಿದ್ದರೆ
              xprintf (p, precision, arg);
          }
        ಇಲ್ಲದಿದ್ದರೆ
          {
            ಆದರೆ (!have_precision)
              xprintf (p, ಕ್ಷೇತ್ರ_ಅಗಲ, arg);
            ಇಲ್ಲದಿದ್ದರೆ
              xprintf (p, ಕ್ಷೇತ್ರ_ಅಗಲ, precision, arg);
          }
      }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'c':
      {
        ಅಕ್ಷರ c = ವಾದ ? *ವಾದ : '\0';
        ಆದರೆ (!have_ಕ್ಷೇತ್ರ_ಅಗಲ)
          xprintf (p, c);
        ಇಲ್ಲದಿದ್ದರೆ
          xprintf (p, ಕ್ಷೇತ್ರ_ಅಗಲ, c);
      }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 's':
      ಆದರೆ (!ವಾದ)
        ವಾದ = "";
      ಆದರೆ (!have_ಕ್ಷೇತ್ರ_ಅಗಲ)
        {
          ಆದರೆ (!have_precision)
            xprintf (p, ವಾದ);
          ಇಲ್ಲದಿದ್ದರೆ
            xprintf (p, precision, ವಾದ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (!have_precision)
            xprintf (p, ಕ್ಷೇತ್ರ_ಅಗಲ, ವಾದ);
          ಇಲ್ಲದಿದ್ದರೆ
            xprintf (p, ಕ್ಷೇತ್ರ_ಅಗಲ, precision, ವಾದ);
        }
      ನಿಲ್ಲಿಸು;
    }

  ಬಿಡುಗಡೆ (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

ರಚನೆ arg_cursor
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *f;	/* Pointer into 'format'.  */
  ಪೂರ್ಣಾಂಕ curr_arg;		/* Current offset.  */
  ಪೂರ್ಣಾಂಕ curr_s_arg;	/* Current sequential offset.  */
  ಪೂರ್ಣಾಂಕ ಕೊನೆ_arg;		/* End arg processed.  */
  ಪೂರ್ಣಾಂಕ direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_ಶುದ್ಧ ಸ್ಥಿರ ರಚನೆ arg_cursor
get_curr_arg (ಪೂರ್ಣಾಂಕ pos, ರಚನೆ arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  ಪೂರ್ಣಾಂಕ arg = 0;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *f = ac.f;
  ಆದರೆ (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      ಪೂರ್ಣಾಂಕ a = *f++ - '0';
      ಗಾಗಿ (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_ಕೂಡಿಸು (&a, a, *f - '0');
        }
      ಆದರೆ (*f == '$')
        arg = v ? ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ : a;
    }

  ಆದರೆ (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      ಆದರೆ (pos == 0)
        ac.direc_arg = arg;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  ಆದರೆ (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.ಕೊನೆ_arg = MAX (ac.ಕೊನೆ_arg, arg);
    }
  ಹಿಂತಿರುಗಿ ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
print_formatted (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ರಚನೆ arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.ಕೊನೆ_arg = ac.direc_arg = -1;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *direc_ಆರಂಭ;	/* Start of % directive.  */
  ಅಕ್ಷರ *direc;			/* Generated % directive.  */
  ಅಕ್ಷರ *pdirec;			/* Pointer to current end of directive.  */
  bool have_ಕ್ಷೇತ್ರ_ಅಗಲ;	/* True if FIELD_WIDTH is valid.  */
  ಪೂರ್ಣಾಂಕ ಕ್ಷೇತ್ರ_ಅಗಲ = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  ಪೂರ್ಣಾಂಕ precision = 0;		/* Arg to second '*'.  */
  ಅಕ್ಷರ ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (format) + 1);

  ಗಾಗಿ (ac.f = format; *ac.f; ac.f++)
    {
      ಆಯ್ಕೆ (*ac.f)
        {
        ಸ್ಥಿತಿ '%':
          direc_ಆರಂಭ = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_ಕ್ಷೇತ್ರ_ಅಗಲ = have_precision = false;
          ಆದರೆ (*ac.f == '%')
            {
              ಅಕ್ಷರ_ಬರೆ ('%');
              ನಿಲ್ಲಿಸು;
            }

          ac = get_curr_arg (0, ac);

          ಆದರೆ (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              ಆದರೆ (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              ನಿಲ್ಲಿಸು;
            }

          ಆದರೆ (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              ಆದರೆ (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              ನಿಲ್ಲಿಸು;
            }

          memset (ok, 0, ಗಾತ್ರ ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          ಗಾಗಿ (;; ac.f++)
            {
              ಆಯ್ಕೆ (*ac.f)
                {
#ಆದರೆ (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                ಸ್ಥಿತಿ 'I':
#ಆದರೆ_ಅಂತ್ಯ
                ಸ್ಥಿತಿ '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ '-': ಸ್ಥಿತಿ '+': ಸ್ಥಿತಿ ' ':
                  ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ '0':
                  ok['c'] = ok['s'] = 0;
                  ನಿಲ್ಲಿಸು;
                ಪೂರ್ವನಿಯೋಜಿತ:
                  ಹೋಗು no_ಇನ್ನಷ್ಟು_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_ಇನ್ನಷ್ಟು_flag_characters:

          ಆದರೆ (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              ಆದರೆ (ac.curr_arg < argc)
                {
                  intmax_t ಅಗಲ = vstrtoimax (argv[ac.curr_arg]);
                  ಆದರೆ (INT_MIN <= ಅಗಲ && ಅಗಲ <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
                    ಕ್ಷೇತ್ರ_ಅಗಲ = ಅಗಲ;
                  ಇಲ್ಲದಿದ್ದರೆ
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              ಇಲ್ಲದಿದ್ದರೆ
                ಕ್ಷೇತ್ರ_ಅಗಲ = 0;
              have_ಕ್ಷೇತ್ರ_ಅಗಲ = true;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ಆಗ (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          ಆದರೆ (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              ಆದರೆ (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  ಆದರೆ (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      ಆದರೆ (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      ಇಲ್ಲದಿದ್ದರೆ
                        precision = prec;
                    }
                  ಇಲ್ಲದಿದ್ದರೆ
                    precision = 0;
                  have_precision = true;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                ಆಗ (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          ಆಗ (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ conversion = *ac.f;
            ಪೂರ್ಣಾಂಕ speclen = MIN (ac.f + 1 - direc_ಆರಂಭ, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
            ಆದರೆ (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_ಆರಂಭ);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_ಕ್ಷೇತ್ರ_ಅಗಲ, ಕ್ಷೇತ್ರ_ಅಗಲ,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : ಖಾಲಿ);

          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\\':
          ac.f += print_esc (ac.f, false);
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          ಅಕ್ಷರ_ಬರೆ (*ac.f);
        }
    }

  ಬಿಡುಗಡೆ (direc);
  ಹಿಂತಿರುಗಿ MIN (argc, ac.ಕೊನೆ_arg + 1);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ *format;
  ಪೂರ್ಣಾಂಕ ವಾದಗಳು_ಬಳಸಿದ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ಖಾಲಿ);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  ಆದರೆ (argc == 2)
    {
      ಆದರೆ (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      ಆದರೆ (streq (argv[1], "--version"))
        {
          ಆವೃತ್ತಿ_etc (stdout, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE_NAME, ಆವೃತ್ತಿ, AUTHORS,
                       (ಅಕ್ಷರ *) ಖಾಲಿ);
          ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  ಆದರೆ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ಆದರೆ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  ಮಾಡು
    {
      ವಾದಗಳು_ಬಳಸಿದ = print_formatted (format, argc, argv);
      argc -= ವಾದಗಳು_ಬಳಸಿದ;
      argv += ವಾದಗಳು_ಬಳಸಿದ;
    }
  ಆಗ (ವಾದಗಳು_ಬಳಸಿದ > 0 && argc > 0);

  ಆದರೆ (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}
