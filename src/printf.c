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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <wchar.h>

#include "system.h"
#include "c-ctype.h"
#include "cl-strtod.h"
#include "octhexdigits.h"
#include "quote.h"
#include "unicodeio.h"
#include "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printf"

#define AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
static ціле exit_status;

/* True if the POSIXLY_CORRECT environment variable is set.  */
static bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
static char const *const cfcc_msg =
 N_("warning: %s: character(s) following character constant have been ignored");

void
usage (ціле status)
{
  якщо (status != EXIT_SUCCESS)
    emit_try_help ();
  інакше
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
  \\\"      double quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new line\n\
  \\r      carriage повернути\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal value NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal value HH (1 to 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex value HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex value HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      ARGUMENT as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      ARGUMENT is printed in a format that can be reused as shell input,\n\
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
  %s          '[%10s]' 'left pad'         [  left pad]\n\
  %s          '[%-10s]' 'right pad'       [right pad ]\n\
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

static void
verify_numeric (char const *s, char const *end)
{
  якщо (s == end)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_status = EXIT_FAILURE;
    }
  інакше якщо (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_status = EXIT_FAILURE;
    }
  інакше якщо (*end)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_status = EXIT_FAILURE;
    }
}

#define STRTOX(TYPE, FUNC_NAME, LIB_FUNC_EXPR)				 \
static TYPE								 \
FUNC_NAME (char const *s)						 \
{									 \
  char *end;								 \
  TYPE val;								 \
                                                                         \
  якщо ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      unsigned char ch = *++s;						 \
      val = ch;								 \
                                                                         \
      якщо (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = strlen (s);					 \
          ssize_t bytes;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          bytes = mbrtowc (&wc, s, slen, &mbstate);			 \
          якщо (0 < bytes)						 \
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
      якщо (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_msg), s);					 \
    }									 \
  інакше									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_EXPR);						 \
      verify_numeric (s, end);						 \
    }									 \
  повернути val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &end, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &end, 0))
STRTOX (long double, vstrtold,   cl_strtold (s, &end))

/* Output a single-character \ escape.  */

static void
print_esc_char (char c)
{
  switch (c)
    {
    case 'a':			/* Alert. */
      putchar ('\a');
      break;
    case 'b':			/* Backspace. */
      putchar ('\b');
      break;
    case 'c':			/* Cancel the rest of the output. */
      exit (EXIT_SUCCESS);
      break;
    case 'e':			/* Escape. */
      putchar ('\x1B');
      break;
    case 'f':			/* Form feed. */
      putchar ('\f');
      break;
    case 'n':			/* New line. */
      putchar ('\n');
      break;
    case 'r':			/* Carriage return. */
      putchar ('\r');
      break;
    case 't':			/* Horizontal tab. */
      putchar ('\t');
      break;
    case 'v':			/* Vertical tab. */
      putchar ('\v');
      break;
    default:
      putchar (c);
      break;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

static ціле
print_esc (char const *escstart, bool octal_0)
{
  char const *p = escstart + 1;
  ціле esc_value = 0;		/* Value of \nnn escape. */
  ціле esc_length;		/* Length of \nnn escape. */

  якщо (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      для (esc_length = 0, ++p;
           esc_length < 2 && c_isxdigit (*p);
           ++esc_length, ++p)
        esc_value = esc_value * 16 + fromhex (*p);
      якщо (esc_length == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      putchar (esc_value);
    }
  інакше якщо (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      для (esc_length = 0, p += octal_0 && *p == '0';
           esc_length < 3 && isoct (*p);
           ++esc_length, ++p)
        esc_value = esc_value * 8 + fromoct (*p);
      putchar (esc_value);
    }
  інакше якщо (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  інакше якщо (*p == 'u' || *p == 'U')
    {
      char esc_char = *p;
      unsigned ціле uni_value;

      uni_value = 0;
      для (esc_length = (esc_char == 'u' ? 4 : 8), ++p;
           esc_length > 0;
           --esc_length, ++p)
        {
          якщо (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_value = uni_value * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      якщо (uni_value >= 0xd800 && uni_value <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_value);

      print_unicode_char (stdout, uni_value, 0);
    }
  інакше
    {
      putchar ('\\');
      якщо (*p)
        {
          putchar (*p);
          p++;
        }
    }
  повернути p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

static void
print_esc_string (char const *str)
{
  для (; *str; str++)
    якщо (*str == '\\')
      str += print_esc (str, true);
    інакше
      putchar (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

static void
print_direc (char const *start, char conversion,
             bool have_field_width, ціле field_width,
             bool have_precision, ціле precision,
             char const *argument)
{
  char *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    char *q;
    char const *length_modifier;
    size_t length_modifier_len;

    switch (conversion)
      {
      case 'd': case 'i': case 'o': case 'u': case 'x': case 'X':
        length_modifier = "j";
        length_modifier_len = 1;
        break;

      case 'a': case 'e': case 'f': case 'g':
      case 'A': case 'E': case 'F': case 'G':
        length_modifier = "L";
        length_modifier_len = 1;
        break;

      default:
        length_modifier = start;  /* Any valid pointer will do.  */
        length_modifier_len = 0;
        break;
      }

    size_t length = strlen (start);
    p = xmalloc (length + length_modifier_len + 2);
    q = mempcpy (p, start, length);
    q = mempcpy (q, length_modifier, length_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  switch (conversion)
    {
    case 'd':
    case 'i':
      {
        intmax_t arg = argument ? vstrtoimax (argument) : 0;
        якщо (!have_field_width)
          {
            якщо (!have_precision)
              xprintf (p, arg);
            інакше
              xprintf (p, precision, arg);
          }
        інакше
          {
            якщо (!have_precision)
              xprintf (p, field_width, arg);
            інакше
              xprintf (p, field_width, precision, arg);
          }
      }
      break;

    case 'o':
    case 'u':
    case 'x':
    case 'X':
      {
        uintmax_t arg = argument ? vstrtoumax (argument) : 0;
        якщо (!have_field_width)
          {
            якщо (!have_precision)
              xprintf (p, arg);
            інакше
              xprintf (p, precision, arg);
          }
        інакше
          {
            якщо (!have_precision)
              xprintf (p, field_width, arg);
            інакше
              xprintf (p, field_width, precision, arg);
          }
      }
      break;

    case 'a':
    case 'A':
    case 'e':
    case 'E':
    case 'f':
    case 'F':
    case 'g':
    case 'G':
      {
        long double arg = argument ? vstrtold (argument) : 0;
        якщо (!have_field_width)
          {
            якщо (!have_precision)
              xprintf (p, arg);
            інакше
              xprintf (p, precision, arg);
          }
        інакше
          {
            якщо (!have_precision)
              xprintf (p, field_width, arg);
            інакше
              xprintf (p, field_width, precision, arg);
          }
      }
      break;

    case 'c':
      {
        char c = argument ? *argument : '\0';
        якщо (!have_field_width)
          xprintf (p, c);
        інакше
          xprintf (p, field_width, c);
      }
      break;

    case 's':
      якщо (!argument)
        argument = "";
      якщо (!have_field_width)
        {
          якщо (!have_precision)
            xprintf (p, argument);
          інакше
            xprintf (p, precision, argument);
        }
      інакше
        {
          якщо (!have_precision)
            xprintf (p, field_width, argument);
          інакше
            xprintf (p, field_width, precision, argument);
        }
      break;
    }

  free (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

struct arg_cursor
{
  char const *f;	/* Pointer into 'format'.  */
  ціле curr_arg;		/* Current offset.  */
  ціле curr_s_arg;	/* Current sequential offset.  */
  ціле end_arg;		/* End arg processed.  */
  ціле direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_PURE static struct arg_cursor
get_curr_arg (ціле pos, struct arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  ціле arg = 0;
  char const *f = ac.f;
  якщо (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      ціле a = *f++ - '0';
      для (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_add (&a, a, *f - '0');
        }
      якщо (*f == '$')
        arg = v ? INT_MAX : a;
    }

  якщо (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      якщо (pos == 0)
        ac.direc_arg = arg;
    }
  інакше
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  якщо (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.end_arg = MAX (ac.end_arg, arg);
    }
  повернути ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

static ціле
print_formatted (char const *format, ціле argc, char **argv)
{
  struct arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.end_arg = ac.direc_arg = -1;
  char const *direc_start;	/* Start of % directive.  */
  char *direc;			/* Generated % directive.  */
  char *pdirec;			/* Pointer to current end of directive.  */
  bool have_field_width;	/* True if FIELD_WIDTH is valid.  */
  ціле field_width = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  ціле precision = 0;		/* Arg to second '*'.  */
  char ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (strlen (format) + 1);

  для (ac.f = format; *ac.f; ac.f++)
    {
      switch (*ac.f)
        {
        case '%':
          direc_start = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_field_width = have_precision = false;
          якщо (*ac.f == '%')
            {
              putchar ('%');
              break;
            }

          ac = get_curr_arg (0, ac);

          якщо (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              якщо (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              break;
            }

          якщо (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              якщо (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              break;
            }

          memset (ok, 0, sizeof ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          для (;; ac.f++)
            {
              switch (*ac.f)
                {
#якщо (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                case 'I':
#endif
                case '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  break;
                case '-': case '+': case ' ':
                  break;
                case '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  break;
                case '0':
                  ok['c'] = ok['s'] = 0;
                  break;
                default:
                  goto no_more_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_more_flag_characters:

          якщо (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              якщо (ac.curr_arg < argc)
                {
                  intmax_t width = vstrtoimax (argv[ac.curr_arg]);
                  якщо (INT_MIN <= width && width <= INT_MAX)
                    field_width = width;
                  інакше
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              інакше
                field_width = 0;
              have_field_width = true;
            }
          інакше
            поки (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          якщо (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              якщо (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  якщо (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      якщо (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      інакше якщо (INT_MAX < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      інакше
                        precision = prec;
                    }
                  інакше
                    precision = 0;
                  have_precision = true;
                }
              інакше
                поки (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          поки (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            unsigned char conversion = *ac.f;
            ціле speclen = MIN (ac.f + 1 - direc_start, INT_MAX);
            якщо (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_start);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_field_width, field_width,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : NULL);

          break;

        case '\\':
          ac.f += print_esc (ac.f, false);
          break;

        default:
          putchar (*ac.f);
        }
    }

  free (direc);
  повернути MIN (argc, ac.end_arg + 1);
}

ціле
main (ціле argc, char **argv)
{
  char *format;
  ціле args_used;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != NULL);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  якщо (argc == 2)
    {
      якщо (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      якщо (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (char *) NULL);
          повернути EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  якщо (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  якщо (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  do
    {
      args_used = print_formatted (format, argc, argv);
      argc -= args_used;
      argv += args_used;
    }
  поки (args_used > 0 && argc > 0);

  якщо (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  повернути exit_status;
}
