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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <wchar.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cl-strtod.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "octhexdigits.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unicodeio.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "printf"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
ਸਥਿਰ ਪੂਰਨ exit_ਸਥਿਤੀ;

/* True if the POSIXLY_CORRECT environment variable is set.  */
ਸਥਿਰ bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ cfcc_ਸੁਨੇਹਾ =
 N_("warning: %s: character(s) following character constant have been ignored");

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s FORMAT [ਦਲੀਲ_2]...\n\
  or:  %s OPTION\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Print ਦਲੀਲ_2(s) according ਮੰਜ਼ਿਲ FORMAT, or execute according ਮੰਜ਼ਿਲ OPTION:\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FORMAT controls the output as ਵਿੱਚ C ਲਿਖੋ.  Interpreted sequences are:\n\
\n\
  \\\"      ਦੁੱਗਣਾ quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new ਸਤਰ\n\
  \\r      carriage ਵਾਪਸ\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal ਮੁੱਲ NNN (1 ਮੰਜ਼ਿਲ 3 digits)\n\
  \\xHH    byte with hexadecimal ਮੁੱਲ HH (1 ਮੰਜ਼ਿਲ 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex ਮੁੱਲ HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex ਮੁੱਲ HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      ਦਲੀਲ_2 as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      ਦਲੀਲ_2 is printed ਵਿੱਚ a format that can be reused as shell ਨਿਵੇਸ਼,\n\
          escaping non-printable characters with the POSIX $'' syntax\
\n\n\
and all C ਲਿਖੋ(3) format specifications ending with one ਦਾ diouxXfaAeEgGcs,\n\
with ARGUMENTs converted ਮੰਜ਼ਿਲ proper type ਪਹਿਲਾ.\n\
"), stdout);
      fputs (_("\
FORMAT is reused ਮੰਜ਼ਿਲ convert all specified arguments.\n\
"), stdout);
      fputs (_("\
The following ਸਾਰਣੀ shows supported conversion specifiers,\n\
with examples including usage ਦਾ ਚੌੜਾਈ, precision, and flags.\n\
\n\
  CONVERSION  Shell format example        Output\n\
  %s          '[%10s]' 'left pad'         [  ਖੱਬਾ pad]\n\
  %s          '[%-10s]' 'right pad'       [ਸੱਜਾ pad ]\n\
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
      ਲਿਖੋ (USAGE_BUILTIN_WARNING, ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਸਥਿਰ ਖਾਲੀ
verify_numeric (ਅੱਖਰ ਅਚਲ *s, ਅੱਖਰ ਅਚਲ *ਸਿਰਾ)
{
  ਜੇ (s == ਸਿਰਾ)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_ਸਥਿਤੀ = EXIT_FAILURE;
    }
  ਨਹੀਂਤਾਂ ਜੇ (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_ਸਥਿਤੀ = EXIT_FAILURE;
    }
  ਨਹੀਂਤਾਂ ਜੇ (*ਸਿਰਾ)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_ਸਥਿਤੀ = EXIT_FAILURE;
    }
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STRTOX(TYPE, FUNC_NAME, LIB_FUNC_ਵਿਅੰਜਕ)				 \
ਸਥਿਰ TYPE								 \
FUNC_NAME (ਅੱਖਰ ਅਚਲ *s)						 \
{									 \
  ਅੱਖਰ *ਸਿਰਾ;								 \
  TYPE val;								 \
                                                                         \
  ਜੇ ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch = *++s;						 \
      val = ch;								 \
                                                                         \
      ਜੇ (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = ਲੜੀ_ਲੰਬਾਈ (s);					 \
          ssize_t ਬਾਈਟਾਂ;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          ਬਾਈਟਾਂ = mbrtowc (&wc, s, slen, &mbstate);			 \
          ਜੇ (0 < ਬਾਈਟਾਂ)						 \
            {								 \
              val = wc;							 \
              s += ਬਾਈਟਾਂ - 1;						 \
            }								 \
        }								 \
                                                                         \
      /* If POSIXLY_CORRECT is not set, then give a warning that there	 \
         are characters following the character constant and that GNU	 \
         printf is ignoring those characters.  If POSIXLY_CORRECT *is*	 \
         set, then don't give the warning.  */				 \
      ਜੇ (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_ਸੁਨੇਹਾ), s);					 \
    }									 \
  ਨਹੀਂਤਾਂ									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_ਵਿਅੰਜਕ);						 \
      verify_numeric (s, ਸਿਰਾ);						 \
    }									 \
  ਵਾਪਸ val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &ਸਿਰਾ, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &ਸਿਰਾ, 0))
STRTOX (ਲੰਮਾ ਦੁੱਗਣਾ, vstrtold,   cl_strtold (s, &ਸਿਰਾ))

/* Output a single-character \ escape.  */

ਸਥਿਰ ਖਾਲੀ
print_esc_char (ਅੱਖਰ c)
{
  ਚੋਣ (c)
    {
    ਹਾਲਤ 'a':			/* Alert. */
      ਅੱਖਰ_ਲਿਖੋ ('\a');
      ਰੋਕੋ;
    ਹਾਲਤ 'b':			/* Backspace. */
      ਅੱਖਰ_ਲਿਖੋ ('\b');
      ਰੋਕੋ;
    ਹਾਲਤ 'c':			/* Cancel the rest of the output. */
      ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
      ਰੋਕੋ;
    ਹਾਲਤ 'e':			/* Escape. */
      ਅੱਖਰ_ਲਿਖੋ ('\x1B');
      ਰੋਕੋ;
    ਹਾਲਤ 'f':			/* Form feed. */
      ਅੱਖਰ_ਲਿਖੋ ('\f');
      ਰੋਕੋ;
    ਹਾਲਤ 'n':			/* New line. */
      ਅੱਖਰ_ਲਿਖੋ ('\n');
      ਰੋਕੋ;
    ਹਾਲਤ 'r':			/* Carriage return. */
      ਅੱਖਰ_ਲਿਖੋ ('\r');
      ਰੋਕੋ;
    ਹਾਲਤ 't':			/* Horizontal tab. */
      ਅੱਖਰ_ਲਿਖੋ ('\t');
      ਰੋਕੋ;
    ਹਾਲਤ 'v':			/* Vertical tab. */
      ਅੱਖਰ_ਲਿਖੋ ('\v');
      ਰੋਕੋ;
    ਮੂਲ:
      ਅੱਖਰ_ਲਿਖੋ (c);
      ਰੋਕੋ;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

ਸਥਿਰ ਪੂਰਨ
print_esc (ਅੱਖਰ ਅਚਲ *escstart, bool octal_0)
{
  ਅੱਖਰ ਅਚਲ *p = escstart + 1;
  ਪੂਰਨ esc_ਮੁੱਲ = 0;		/* Value of \nnn escape. */
  ਪੂਰਨ esc_ਲੰਮਾਈ;		/* Length of \nnn escape. */

  ਜੇ (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      ਲਈ (esc_ਲੰਮਾਈ = 0, ++p;
           esc_ਲੰਮਾਈ < 2 && c_isxdigit (*p);
           ++esc_ਲੰਮਾਈ, ++p)
        esc_ਮੁੱਲ = esc_ਮੁੱਲ * 16 + fromhex (*p);
      ਜੇ (esc_ਲੰਮਾਈ == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      ਅੱਖਰ_ਲਿਖੋ (esc_ਮੁੱਲ);
    }
  ਨਹੀਂਤਾਂ ਜੇ (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      ਲਈ (esc_ਲੰਮਾਈ = 0, p += octal_0 && *p == '0';
           esc_ਲੰਮਾਈ < 3 && isoct (*p);
           ++esc_ਲੰਮਾਈ, ++p)
        esc_ਮੁੱਲ = esc_ਮੁੱਲ * 8 + fromoct (*p);
      ਅੱਖਰ_ਲਿਖੋ (esc_ਮੁੱਲ);
    }
  ਨਹੀਂਤਾਂ ਜੇ (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  ਨਹੀਂਤਾਂ ਜੇ (*p == 'u' || *p == 'U')
    {
      ਅੱਖਰ esc_char = *p;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ uni_ਮੁੱਲ;

      uni_ਮੁੱਲ = 0;
      ਲਈ (esc_ਲੰਮਾਈ = (esc_char == 'u' ? 4 : 8), ++p;
           esc_ਲੰਮਾਈ > 0;
           --esc_ਲੰਮਾਈ, ++p)
        {
          ਜੇ (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_ਮੁੱਲ = uni_ਮੁੱਲ * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      ਜੇ (uni_ਮੁੱਲ >= 0xd800 && uni_ਮੁੱਲ <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_ਮੁੱਲ);

      print_unicode_char (stdout, uni_ਮੁੱਲ, 0);
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ_ਲਿਖੋ ('\\');
      ਜੇ (*p)
        {
          ਅੱਖਰ_ਲਿਖੋ (*p);
          p++;
        }
    }
  ਵਾਪਸ p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

ਸਥਿਰ ਖਾਲੀ
print_esc_string (ਅੱਖਰ ਅਚਲ *str)
{
  ਲਈ (; *str; str++)
    ਜੇ (*str == '\\')
      str += print_esc (str, true);
    ਨਹੀਂਤਾਂ
      ਅੱਖਰ_ਲਿਖੋ (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

ਸਥਿਰ ਖਾਲੀ
print_direc (ਅੱਖਰ ਅਚਲ *ਸ਼ੁਰੂ, ਅੱਖਰ conversion,
             bool have_ਖੇਤਰ_ਚੌੜਾਈ, ਪੂਰਨ ਖੇਤਰ_ਚੌੜਾਈ,
             bool have_precision, ਪੂਰਨ precision,
             ਅੱਖਰ ਅਚਲ *ਦਲੀਲ)
{
  ਅੱਖਰ *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    ਅੱਖਰ *q;
    ਅੱਖਰ ਅਚਲ *ਲੰਮਾਈ_modifier;
    size_t ਲੰਮਾਈ_modifier_len;

    ਚੋਣ (conversion)
      {
      ਹਾਲਤ 'd': ਹਾਲਤ 'i': ਹਾਲਤ 'o': ਹਾਲਤ 'u': ਹਾਲਤ 'x': ਹਾਲਤ 'X':
        ਲੰਮਾਈ_modifier = "j";
        ਲੰਮਾਈ_modifier_len = 1;
        ਰੋਕੋ;

      ਹਾਲਤ 'a': ਹਾਲਤ 'e': ਹਾਲਤ 'f': ਹਾਲਤ 'g':
      ਹਾਲਤ 'A': ਹਾਲਤ 'E': ਹਾਲਤ 'F': ਹਾਲਤ 'G':
        ਲੰਮਾਈ_modifier = "L";
        ਲੰਮਾਈ_modifier_len = 1;
        ਰੋਕੋ;

      ਮੂਲ:
        ਲੰਮਾਈ_modifier = ਸ਼ੁਰੂ;  /* Any valid pointer will do.  */
        ਲੰਮਾਈ_modifier_len = 0;
        ਰੋਕੋ;
      }

    size_t ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ (ਸ਼ੁਰੂ);
    p = xmalloc (ਲੰਮਾਈ + ਲੰਮਾਈ_modifier_len + 2);
    q = mempcpy (p, ਸ਼ੁਰੂ, ਲੰਮਾਈ);
    q = mempcpy (q, ਲੰਮਾਈ_modifier, ਲੰਮਾਈ_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  ਚੋਣ (conversion)
    {
    ਹਾਲਤ 'd':
    ਹਾਲਤ 'i':
      {
        intmax_t arg = ਦਲੀਲ ? vstrtoimax (ਦਲੀਲ) : 0;
        ਜੇ (!have_ਖੇਤਰ_ਚੌੜਾਈ)
          {
            ਜੇ (!have_precision)
              xprintf (p, arg);
            ਨਹੀਂਤਾਂ
              xprintf (p, precision, arg);
          }
        ਨਹੀਂਤਾਂ
          {
            ਜੇ (!have_precision)
              xprintf (p, ਖੇਤਰ_ਚੌੜਾਈ, arg);
            ਨਹੀਂਤਾਂ
              xprintf (p, ਖੇਤਰ_ਚੌੜਾਈ, precision, arg);
          }
      }
      ਰੋਕੋ;

    ਹਾਲਤ 'o':
    ਹਾਲਤ 'u':
    ਹਾਲਤ 'x':
    ਹਾਲਤ 'X':
      {
        uintmax_t arg = ਦਲੀਲ ? vstrtoumax (ਦਲੀਲ) : 0;
        ਜੇ (!have_ਖੇਤਰ_ਚੌੜਾਈ)
          {
            ਜੇ (!have_precision)
              xprintf (p, arg);
            ਨਹੀਂਤਾਂ
              xprintf (p, precision, arg);
          }
        ਨਹੀਂਤਾਂ
          {
            ਜੇ (!have_precision)
              xprintf (p, ਖੇਤਰ_ਚੌੜਾਈ, arg);
            ਨਹੀਂਤਾਂ
              xprintf (p, ਖੇਤਰ_ਚੌੜਾਈ, precision, arg);
          }
      }
      ਰੋਕੋ;

    ਹਾਲਤ 'a':
    ਹਾਲਤ 'A':
    ਹਾਲਤ 'e':
    ਹਾਲਤ 'E':
    ਹਾਲਤ 'f':
    ਹਾਲਤ 'F':
    ਹਾਲਤ 'g':
    ਹਾਲਤ 'G':
      {
        ਲੰਮਾ ਦੁੱਗਣਾ arg = ਦਲੀਲ ? vstrtold (ਦਲੀਲ) : 0;
        ਜੇ (!have_ਖੇਤਰ_ਚੌੜਾਈ)
          {
            ਜੇ (!have_precision)
              xprintf (p, arg);
            ਨਹੀਂਤਾਂ
              xprintf (p, precision, arg);
          }
        ਨਹੀਂਤਾਂ
          {
            ਜੇ (!have_precision)
              xprintf (p, ਖੇਤਰ_ਚੌੜਾਈ, arg);
            ਨਹੀਂਤਾਂ
              xprintf (p, ਖੇਤਰ_ਚੌੜਾਈ, precision, arg);
          }
      }
      ਰੋਕੋ;

    ਹਾਲਤ 'c':
      {
        ਅੱਖਰ c = ਦਲੀਲ ? *ਦਲੀਲ : '\0';
        ਜੇ (!have_ਖੇਤਰ_ਚੌੜਾਈ)
          xprintf (p, c);
        ਨਹੀਂਤਾਂ
          xprintf (p, ਖੇਤਰ_ਚੌੜਾਈ, c);
      }
      ਰੋਕੋ;

    ਹਾਲਤ 's':
      ਜੇ (!ਦਲੀਲ)
        ਦਲੀਲ = "";
      ਜੇ (!have_ਖੇਤਰ_ਚੌੜਾਈ)
        {
          ਜੇ (!have_precision)
            xprintf (p, ਦਲੀਲ);
          ਨਹੀਂਤਾਂ
            xprintf (p, precision, ਦਲੀਲ);
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (!have_precision)
            xprintf (p, ਖੇਤਰ_ਚੌੜਾਈ, ਦਲੀਲ);
          ਨਹੀਂਤਾਂ
            xprintf (p, ਖੇਤਰ_ਚੌੜਾਈ, precision, ਦਲੀਲ);
        }
      ਰੋਕੋ;
    }

  ਮੁਕਤ (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

ਬਣਤਰ arg_cursor
{
  ਅੱਖਰ ਅਚਲ *f;	/* Pointer into 'format'.  */
  ਪੂਰਨ curr_arg;		/* Current offset.  */
  ਪੂਰਨ curr_s_arg;	/* Current sequential offset.  */
  ਪੂਰਨ ਸਿਰਾ_arg;		/* End arg processed.  */
  ਪੂਰਨ direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_ਸ਼ੁੱਧ ਸਥਿਰ ਬਣਤਰ arg_cursor
get_curr_arg (ਪੂਰਨ pos, ਬਣਤਰ arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  ਪੂਰਨ arg = 0;
  ਅੱਖਰ ਅਚਲ *f = ac.f;
  ਜੇ (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      ਪੂਰਨ a = *f++ - '0';
      ਲਈ (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_ਜੋੜੋ (&a, a, *f - '0');
        }
      ਜੇ (*f == '$')
        arg = v ? ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ : a;
    }

  ਜੇ (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      ਜੇ (pos == 0)
        ac.direc_arg = arg;
    }
  ਨਹੀਂਤਾਂ
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  ਜੇ (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.ਸਿਰਾ_arg = MAX (ac.ਸਿਰਾ_arg, arg);
    }
  ਵਾਪਸ ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

ਸਥਿਰ ਪੂਰਨ
print_formatted (ਅੱਖਰ ਅਚਲ *format, ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਬਣਤਰ arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.ਸਿਰਾ_arg = ac.direc_arg = -1;
  ਅੱਖਰ ਅਚਲ *direc_ਸ਼ੁਰੂ;	/* Start of % directive.  */
  ਅੱਖਰ *direc;			/* Generated % directive.  */
  ਅੱਖਰ *pdirec;			/* Pointer to current end of directive.  */
  bool have_ਖੇਤਰ_ਚੌੜਾਈ;	/* True if FIELD_WIDTH is valid.  */
  ਪੂਰਨ ਖੇਤਰ_ਚੌੜਾਈ = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  ਪੂਰਨ precision = 0;		/* Arg to second '*'.  */
  ਅੱਖਰ ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (ਲੜੀ_ਲੰਬਾਈ (format) + 1);

  ਲਈ (ac.f = format; *ac.f; ac.f++)
    {
      ਚੋਣ (*ac.f)
        {
        ਹਾਲਤ '%':
          direc_ਸ਼ੁਰੂ = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_ਖੇਤਰ_ਚੌੜਾਈ = have_precision = false;
          ਜੇ (*ac.f == '%')
            {
              ਅੱਖਰ_ਲਿਖੋ ('%');
              ਰੋਕੋ;
            }

          ac = get_curr_arg (0, ac);

          ਜੇ (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              ਜੇ (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              ਰੋਕੋ;
            }

          ਜੇ (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              ਜੇ (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              ਰੋਕੋ;
            }

          memset (ok, 0, ਆਕਾਰ ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          ਲਈ (;; ac.f++)
            {
              ਚੋਣ (*ac.f)
                {
#ਜੇ (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                ਹਾਲਤ 'I':
#ਜੇ_ਅੰਤ
                ਹਾਲਤ '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  ਰੋਕੋ;
                ਹਾਲਤ '-': ਹਾਲਤ '+': ਹਾਲਤ ' ':
                  ਰੋਕੋ;
                ਹਾਲਤ '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  ਰੋਕੋ;
                ਹਾਲਤ '0':
                  ok['c'] = ok['s'] = 0;
                  ਰੋਕੋ;
                ਮੂਲ:
                  ਜਾਓ no_ਹੋਰ_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_ਹੋਰ_flag_characters:

          ਜੇ (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              ਜੇ (ac.curr_arg < argc)
                {
                  intmax_t ਚੌੜਾਈ = vstrtoimax (argv[ac.curr_arg]);
                  ਜੇ (INT_MIN <= ਚੌੜਾਈ && ਚੌੜਾਈ <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
                    ਖੇਤਰ_ਚੌੜਾਈ = ਚੌੜਾਈ;
                  ਨਹੀਂਤਾਂ
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              ਨਹੀਂਤਾਂ
                ਖੇਤਰ_ਚੌੜਾਈ = 0;
              have_ਖੇਤਰ_ਚੌੜਾਈ = true;
            }
          ਨਹੀਂਤਾਂ
            ਜਦੋਂ (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          ਜੇ (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              ਜੇ (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  ਜੇ (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      ਜੇ (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      ਨਹੀਂਤਾਂ ਜੇ (ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      ਨਹੀਂਤਾਂ
                        precision = prec;
                    }
                  ਨਹੀਂਤਾਂ
                    precision = 0;
                  have_precision = true;
                }
              ਨਹੀਂਤਾਂ
                ਜਦੋਂ (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          ਜਦੋਂ (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ conversion = *ac.f;
            ਪੂਰਨ speclen = MIN (ac.f + 1 - direc_ਸ਼ੁਰੂ, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
            ਜੇ (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_ਸ਼ੁਰੂ);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_ਖੇਤਰ_ਚੌੜਾਈ, ਖੇਤਰ_ਚੌੜਾਈ,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : ਨਿਰਾ);

          ਰੋਕੋ;

        ਹਾਲਤ '\\':
          ac.f += print_esc (ac.f, false);
          ਰੋਕੋ;

        ਮੂਲ:
          ਅੱਖਰ_ਲਿਖੋ (*ac.f);
        }
    }

  ਮੁਕਤ (direc);
  ਵਾਪਸ MIN (argc, ac.ਸਿਰਾ_arg + 1);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ *format;
  ਪੂਰਨ ਦਲੀਲਾਂ_ਵਰਤਿਆ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ਨਿਰਾ);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  ਜੇ (argc == 2)
    {
      ਜੇ (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      ਜੇ (streq (argv[1], "--version"))
        {
          ਸੰਸਕਰਣ_etc (stdout, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE_NAME, ਸੰਸਕਰਣ, AUTHORS,
                       (ਅੱਖਰ *) ਨਿਰਾ);
          ਵਾਪਸ EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  ਜੇ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ਜੇ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  ਕਰੋ
    {
      ਦਲੀਲਾਂ_ਵਰਤਿਆ = print_formatted (format, argc, argv);
      argc -= ਦਲੀਲਾਂ_ਵਰਤਿਆ;
      argv += ਦਲੀਲਾਂ_ਵਰਤਿਆ;
    }
  ਜਦੋਂ (ਦਲੀਲਾਂ_ਵਰਤਿਆ > 0 && argc > 0);

  ਜੇ (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  ਵਾਪਸ exit_ਸਥਿਤੀ;
}
