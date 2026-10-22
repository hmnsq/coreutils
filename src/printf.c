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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <wchar.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "cl-strtod.h"
#ଅନ୍ତର୍ଭାଵ "octhexdigits.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "unicodeio.h"
#ଅନ୍ତର୍ଭାଵ "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printf"

#define AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status;

/* True if the POSIXLY_CORRECT environment variable is set.  */
ସ୍ଥିର bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ cfcc_ସନ୍ଦେଶ =
 N_("warning: %s: character(s) following character constant have been ignored");

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
  \\\"      ଦ୍ୱିଗୁଣ quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new ଧାଡ଼ି\n\
  \\r      carriage ଫେରନ୍ତୁ\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal ମୂଲ୍ୟ NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal ମୂଲ୍ୟ HH (1 to 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex ମୂଲ୍ୟ HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex ମୂଲ୍ୟ HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      ARGUMENT as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      ARGUMENT is printed in a format that can be reused as shell ନିବେଶ,\n\
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
  %s          '[%10s]' 'left pad'         [  ବାମ pad]\n\
  %s          '[%-10s]' 'right pad'       [ଡାହାଣ pad ]\n\
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

ସ୍ଥିର ଶୂନ୍ୟ
verify_numeric (ଅକ୍ଷର ଅଚଳ *s, ଅକ୍ଷର ଅଚଳ *end)
{
  ଯଦି (s == end)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_status = EXIT_FAILURE;
    }
  ଅନ୍ୟଥା ଯଦି (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_status = EXIT_FAILURE;
    }
  ଅନ୍ୟଥା ଯଦି (*end)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_status = EXIT_FAILURE;
    }
}

#define STRTOX(TYPE, FUNC_NAME, LIB_FUNC_ପଦସମଷ୍ଟି)				 \
ସ୍ଥିର TYPE								 \
FUNC_NAME (ଅକ୍ଷର ଅଚଳ *s)						 \
{									 \
  ଅକ୍ଷର *end;								 \
  TYPE val;								 \
                                                                         \
  ଯଦି ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      unsigned ଅକ୍ଷର ch = *++s;						 \
      val = ch;								 \
                                                                         \
      ଯଦି (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = strlen (s);					 \
          ssize_t bytes;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          bytes = mbrtowc (&wc, s, slen, &mbstate);			 \
          ଯଦି (0 < bytes)						 \
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
      ଯଦି (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_ସନ୍ଦେଶ), s);					 \
    }									 \
  ଅନ୍ୟଥା									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_ପଦସମଷ୍ଟି);						 \
      verify_numeric (s, end);						 \
    }									 \
  ଫେରନ୍ତୁ val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &end, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &end, 0))
STRTOX (long ଦ୍ୱିଗୁଣ, vstrtold,   cl_strtold (s, &end))

/* Output a single-character \ escape.  */

ସ୍ଥିର ଶୂନ୍ୟ
print_esc_char (ଅକ୍ଷର c)
{
  ବାଛ (c)
    {
    ସ୍ଥିତି 'a':			/* Alert. */
      putchar ('\a');
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'b':			/* Backspace. */
      putchar ('\b');
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'c':			/* Cancel the rest of the output. */
      exit (EXIT_SUCCESS);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'e':			/* Escape. */
      putchar ('\x1B');
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'f':			/* Form feed. */
      putchar ('\f');
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'n':			/* New line. */
      putchar ('\n');
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'r':			/* Carriage return. */
      putchar ('\r');
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 't':			/* Horizontal tab. */
      putchar ('\t');
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'v':			/* Vertical tab. */
      putchar ('\v');
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      putchar (c);
      ଭାଙ୍ଗ;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
print_esc (ଅକ୍ଷର ଅଚଳ *escstart, bool octal_0)
{
  ଅକ୍ଷର ଅଚଳ *p = escstart + 1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା esc_ମୂଲ୍ୟ = 0;		/* Value of \nnn escape. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା esc_length;		/* Length of \nnn escape. */

  ଯଦି (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      ପାଇଁ (esc_length = 0, ++p;
           esc_length < 2 && c_isxdigit (*p);
           ++esc_length, ++p)
        esc_ମୂଲ୍ୟ = esc_ମୂଲ୍ୟ * 16 + fromhex (*p);
      ଯଦି (esc_length == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      putchar (esc_ମୂଲ୍ୟ);
    }
  ଅନ୍ୟଥା ଯଦି (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      ପାଇଁ (esc_length = 0, p += octal_0 && *p == '0';
           esc_length < 3 && isoct (*p);
           ++esc_length, ++p)
        esc_ମୂଲ୍ୟ = esc_ମୂଲ୍ୟ * 8 + fromoct (*p);
      putchar (esc_ମୂଲ୍ୟ);
    }
  ଅନ୍ୟଥା ଯଦି (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  ଅନ୍ୟଥା ଯଦି (*p == 'u' || *p == 'U')
    {
      ଅକ୍ଷର esc_char = *p;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା uni_ମୂଲ୍ୟ;

      uni_ମୂଲ୍ୟ = 0;
      ପାଇଁ (esc_length = (esc_char == 'u' ? 4 : 8), ++p;
           esc_length > 0;
           --esc_length, ++p)
        {
          ଯଦି (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_ମୂଲ୍ୟ = uni_ମୂଲ୍ୟ * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      ଯଦି (uni_ମୂଲ୍ୟ >= 0xd800 && uni_ମୂଲ୍ୟ <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_ମୂଲ୍ୟ);

      print_unicode_char (stdout, uni_ମୂଲ୍ୟ, 0);
    }
  ଅନ୍ୟଥା
    {
      putchar ('\\');
      ଯଦି (*p)
        {
          putchar (*p);
          p++;
        }
    }
  ଫେରନ୍ତୁ p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

ସ୍ଥିର ଶୂନ୍ୟ
print_esc_string (ଅକ୍ଷର ଅଚଳ *str)
{
  ପାଇଁ (; *str; str++)
    ଯଦି (*str == '\\')
      str += print_esc (str, true);
    ଅନ୍ୟଥା
      putchar (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

ସ୍ଥିର ଶୂନ୍ୟ
print_direc (ଅକ୍ଷର ଅଚଳ *start, ଅକ୍ଷର conversion,
             bool have_field_width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା field_width,
             bool have_precision, ପୂର୍ଣ୍ଣସଂଖ୍ୟା precision,
             ଅକ୍ଷର ଅଚଳ *argument)
{
  ଅକ୍ଷର *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    ଅକ୍ଷର *q;
    ଅକ୍ଷର ଅଚଳ *length_modifier;
    size_t length_modifier_len;

    ବାଛ (conversion)
      {
      ସ୍ଥିତି 'd': ସ୍ଥିତି 'i': ସ୍ଥିତି 'o': ସ୍ଥିତି 'u': ସ୍ଥିତି 'x': ସ୍ଥିତି 'X':
        length_modifier = "j";
        length_modifier_len = 1;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'a': ସ୍ଥିତି 'e': ସ୍ଥିତି 'f': ସ୍ଥିତି 'g':
      ସ୍ଥିତି 'A': ସ୍ଥିତି 'E': ସ୍ଥିତି 'F': ସ୍ଥିତି 'G':
        length_modifier = "L";
        length_modifier_len = 1;
        ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        length_modifier = start;  /* Any valid pointer will do.  */
        length_modifier_len = 0;
        ଭାଙ୍ଗ;
      }

    size_t length = strlen (start);
    p = xmalloc (length + length_modifier_len + 2);
    q = mempcpy (p, start, length);
    q = mempcpy (q, length_modifier, length_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  ବାଛ (conversion)
    {
    ସ୍ଥିତି 'd':
    ସ୍ଥିତି 'i':
      {
        intmax_t arg = argument ? vstrtoimax (argument) : 0;
        ଯଦି (!have_field_width)
          {
            ଯଦି (!have_precision)
              xprintf (p, arg);
            ଅନ୍ୟଥା
              xprintf (p, precision, arg);
          }
        ଅନ୍ୟଥା
          {
            ଯଦି (!have_precision)
              xprintf (p, field_width, arg);
            ଅନ୍ୟଥା
              xprintf (p, field_width, precision, arg);
          }
      }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'o':
    ସ୍ଥିତି 'u':
    ସ୍ଥିତି 'x':
    ସ୍ଥିତି 'X':
      {
        uintmax_t arg = argument ? vstrtoumax (argument) : 0;
        ଯଦି (!have_field_width)
          {
            ଯଦି (!have_precision)
              xprintf (p, arg);
            ଅନ୍ୟଥା
              xprintf (p, precision, arg);
          }
        ଅନ୍ୟଥା
          {
            ଯଦି (!have_precision)
              xprintf (p, field_width, arg);
            ଅନ୍ୟଥା
              xprintf (p, field_width, precision, arg);
          }
      }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'a':
    ସ୍ଥିତି 'A':
    ସ୍ଥିତି 'e':
    ସ୍ଥିତି 'E':
    ସ୍ଥିତି 'f':
    ସ୍ଥିତି 'F':
    ସ୍ଥିତି 'g':
    ସ୍ଥିତି 'G':
      {
        long ଦ୍ୱିଗୁଣ arg = argument ? vstrtold (argument) : 0;
        ଯଦି (!have_field_width)
          {
            ଯଦି (!have_precision)
              xprintf (p, arg);
            ଅନ୍ୟଥା
              xprintf (p, precision, arg);
          }
        ଅନ୍ୟଥା
          {
            ଯଦି (!have_precision)
              xprintf (p, field_width, arg);
            ଅନ୍ୟଥା
              xprintf (p, field_width, precision, arg);
          }
      }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'c':
      {
        ଅକ୍ଷର c = argument ? *argument : '\0';
        ଯଦି (!have_field_width)
          xprintf (p, c);
        ଅନ୍ୟଥା
          xprintf (p, field_width, c);
      }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 's':
      ଯଦି (!argument)
        argument = "";
      ଯଦି (!have_field_width)
        {
          ଯଦି (!have_precision)
            xprintf (p, argument);
          ଅନ୍ୟଥା
            xprintf (p, precision, argument);
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (!have_precision)
            xprintf (p, field_width, argument);
          ଅନ୍ୟଥା
            xprintf (p, field_width, precision, argument);
        }
      ଭାଙ୍ଗ;
    }

  ମୁକ୍ତ (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

ସଂରଚନା arg_cursor
{
  ଅକ୍ଷର ଅଚଳ *f;	/* Pointer into 'format'.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା curr_arg;		/* Current offset.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା curr_s_arg;	/* Current sequential offset.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା end_arg;		/* End arg processed.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_PURE ସ୍ଥିର ସଂରଚନା arg_cursor
get_curr_arg (ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos, ସଂରଚନା arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା arg = 0;
  ଅକ୍ଷର ଅଚଳ *f = ac.f;
  ଯଦି (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା a = *f++ - '0';
      ପାଇଁ (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_add (&a, a, *f - '0');
        }
      ଯଦି (*f == '$')
        arg = v ? INT_MAX : a;
    }

  ଯଦି (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      ଯଦି (pos == 0)
        ac.direc_arg = arg;
    }
  ଅନ୍ୟଥା
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  ଯଦି (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.end_arg = MAX (ac.end_arg, arg);
    }
  ଫେରନ୍ତୁ ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
print_formatted (ଅକ୍ଷର ଅଚଳ *format, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ସଂରଚନା arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.end_arg = ac.direc_arg = -1;
  ଅକ୍ଷର ଅଚଳ *direc_start;	/* Start of % directive.  */
  ଅକ୍ଷର *direc;			/* Generated % directive.  */
  ଅକ୍ଷର *pdirec;			/* Pointer to current end of directive.  */
  bool have_field_width;	/* True if FIELD_WIDTH is valid.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା field_width = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା precision = 0;		/* Arg to second '*'.  */
  ଅକ୍ଷର ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (strlen (format) + 1);

  ପାଇଁ (ac.f = format; *ac.f; ac.f++)
    {
      ବାଛ (*ac.f)
        {
        ସ୍ଥିତି '%':
          direc_start = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_field_width = have_precision = false;
          ଯଦି (*ac.f == '%')
            {
              putchar ('%');
              ଭାଙ୍ଗ;
            }

          ac = get_curr_arg (0, ac);

          ଯଦି (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              ଯଦି (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              ଭାଙ୍ଗ;
            }

          ଯଦି (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              ଯଦି (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              ଭାଙ୍ଗ;
            }

          memset (ok, 0, ଆକାର ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          ପାଇଁ (;; ac.f++)
            {
              ବାଛ (*ac.f)
                {
#ଯଦି (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                ସ୍ଥିତି 'I':
#endif
                ସ୍ଥିତି '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  ଭାଙ୍ଗ;
                ସ୍ଥିତି '-': ସ୍ଥିତି '+': ସ୍ଥିତି ' ':
                  ଭାଙ୍ଗ;
                ସ୍ଥିତି '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  ଭାଙ୍ଗ;
                ସ୍ଥିତି '0':
                  ok['c'] = ok['s'] = 0;
                  ଭାଙ୍ଗ;
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                  ଯାଅ no_more_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_more_flag_characters:

          ଯଦି (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              ଯଦି (ac.curr_arg < argc)
                {
                  intmax_t width = vstrtoimax (argv[ac.curr_arg]);
                  ଯଦି (INT_MIN <= width && width <= INT_MAX)
                    field_width = width;
                  ଅନ୍ୟଥା
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              ଅନ୍ୟଥା
                field_width = 0;
              have_field_width = true;
            }
          ଅନ୍ୟଥା
            ଯେତେବେଳେ (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          ଯଦି (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              ଯଦି (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  ଯଦି (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      ଯଦି (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      ଅନ୍ୟଥା ଯଦି (INT_MAX < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      ଅନ୍ୟଥା
                        precision = prec;
                    }
                  ଅନ୍ୟଥା
                    precision = 0;
                  have_precision = true;
                }
              ଅନ୍ୟଥା
                ଯେତେବେଳେ (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          ଯେତେବେଳେ (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            unsigned ଅକ୍ଷର conversion = *ac.f;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା speclen = MIN (ac.f + 1 - direc_start, INT_MAX);
            ଯଦି (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_start);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_field_width, field_width,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : ଖାଲି);

          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\\':
          ac.f += print_esc (ac.f, false);
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          putchar (*ac.f);
        }
    }

  ମୁକ୍ତ (direc);
  ଫେରନ୍ତୁ MIN (argc, ac.end_arg + 1);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଅକ୍ଷର *format;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା args_used;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != ଖାଲି);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  ଯଦି (argc == 2)
    {
      ଯଦି (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      ଯଦି (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (ଅକ୍ଷର *) ଖାଲି);
          ଫେରନ୍ତୁ EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  ଯଦି (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ଯଦି (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  କର
    {
      args_used = print_formatted (format, argc, argv);
      argc -= args_used;
      argv += args_used;
    }
  ଯେତେବେଳେ (args_used > 0 && argc > 0);

  ଯଦି (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  ଫେରନ୍ତୁ exit_status;
}
