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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <wchar.h>

#شامل "system.h"
#شامل "c-ctype.h"
#شامل "cl-strtod.h"
#شامل "octhexdigits.h"
#شامل "quote.h"
#شامل "unicodeio.h"
#شامل "xprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printf"

#define AUTHORS proper_name ("David MacKenzie")

/* The value to return to the calling program.  */
ایستا صحیح exit_status;

/* True if the POSIXLY_CORRECT environment variable is set.  */
ایستا bool posixly_correct;

/* This message appears in N_() here rather than just in _() below because
   the sole use would have been in a #define.  */
ایستا نویسه ثابت *ثابت cfcc_پیام =
 N_("warning: %s: character(s) following character constant have been ignored");

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
  \\\"      دوگانه quote\n\
"), stdout);
      fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new سطر\n\
  \\r      carriage بازگشت\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
      fputs (_("\
  \\NNN    byte with octal مقدار NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal مقدار HH (1 to 2 digits)\n\
  \\uHHHH  Unicode (ISO/IEC 10646) character with hex مقدار HHHH (4 digits)\n\
  \\UHHHHHHHH  Unicode character with hex مقدار HHHHHHHH (8 digits)\n\
"), stdout);
      fputs (_("\
\n\
  %%      a single %\n\
  %b      ARGUMENT as a string with '\\' escapes interpreted,\n\
          except that octal escapes should have a leading 0 like \\0NNN\n\
  %q      ARGUMENT is printed in a format that can be reused as shell درون_داد,\n\
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
  %s          '[%10s]' 'left pad'         [  چپ pad]\n\
  %s          '[%-10s]' 'right pad'       [راست pad ]\n\
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

ایستا تهی
verify_numeric (نویسه ثابت *s, نویسه ثابت *end)
{
  اگر (s == end)
    {
      error (0, 0, _("%s: expected a numeric value"), quote (s));
      exit_status = EXIT_FAILURE;
    }
  وگرنه اگر (errno)
    {
      error (0, errno, "%s", quote (s));
      exit_status = EXIT_FAILURE;
    }
  وگرنه اگر (*end)
    {
      error (0, 0, _("%s: value not completely converted"), quote (s));
      exit_status = EXIT_FAILURE;
    }
}

#define STRTOX(TYPE, FUNC_NAME, LIB_FUNC_عبارت)				 \
ایستا TYPE								 \
FUNC_NAME (نویسه ثابت *s)						 \
{									 \
  نویسه *end;								 \
  TYPE val;								 \
                                                                         \
  اگر ((*s == '\"' || *s == '\'') && *(s + 1))				 \
    {									 \
      unsigned نویسه ch = *++s;						 \
      val = ch;								 \
                                                                         \
      اگر (MB_CUR_MAX > 1 && *(s + 1))					 \
        {								 \
          mbstate_t mbstate; mbszero (&mbstate);			 \
          wchar_t wc;							 \
          size_t slen = strlen (s);					 \
          ssize_t bytes;						 \
          /* Use mbrtowc not mbrtoc32, as per POSIX.  */		 \
          bytes = mbrtowc (&wc, s, slen, &mbstate);			 \
          اگر (0 < bytes)						 \
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
      اگر (*++s != 0 && !posixly_correct)				 \
        error (0, 0, _(cfcc_پیام), s);					 \
    }									 \
  وگرنه									 \
    {									 \
      errno = 0;							 \
      val = (LIB_FUNC_عبارت);						 \
      verify_numeric (s, end);						 \
    }									 \
  بازگشت val;								 \
}									 \

STRTOX (intmax_t,    vstrtoimax, strtoimax (s, &end, 0))
STRTOX (uintmax_t,   vstrtoumax, strtoumax (s, &end, 0))
STRTOX (long دوگانه, vstrtold,   cl_strtold (s, &end))

/* Output a single-character \ escape.  */

ایستا تهی
print_esc_char (نویسه c)
{
  انتخاب (c)
    {
    حالت 'a':			/* Alert. */
      putchar ('\a');
      بشکن;
    حالت 'b':			/* Backspace. */
      putchar ('\b');
      بشکن;
    حالت 'c':			/* Cancel the rest of the output. */
      exit (EXIT_SUCCESS);
      بشکن;
    حالت 'e':			/* Escape. */
      putchar ('\x1B');
      بشکن;
    حالت 'f':			/* Form feed. */
      putchar ('\f');
      بشکن;
    حالت 'n':			/* New line. */
      putchar ('\n');
      بشکن;
    حالت 'r':			/* Carriage return. */
      putchar ('\r');
      بشکن;
    حالت 't':			/* Horizontal tab. */
      putchar ('\t');
      بشکن;
    حالت 'v':			/* Vertical tab. */
      putchar ('\v');
      بشکن;
    پیشفرض:
      putchar (c);
      بشکن;
    }
}

/* Print a \ escape sequence starting at ESCSTART.
   Return the number of characters in the escape sequence
   besides the backslash.
   If OCTAL_0 is nonzero, octal escapes are of the form \0ooo, where o
   is an octal digit; otherwise they are of the form \ooo.  */

ایستا صحیح
print_esc (نویسه ثابت *escstart, bool octal_0)
{
  نویسه ثابت *p = escstart + 1;
  صحیح esc_مقدار = 0;		/* Value of \nnn escape. */
  صحیح esc_length;		/* Length of \nnn escape. */

  اگر (*p == 'x')
    {
      /* A hexadecimal \xhh escape sequence must have 1 or 2 hex. digits.  */
      برای (esc_length = 0, ++p;
           esc_length < 2 && c_isxdigit (*p);
           ++esc_length, ++p)
        esc_مقدار = esc_مقدار * 16 + fromhex (*p);
      اگر (esc_length == 0)
        error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
      putchar (esc_مقدار);
    }
  وگرنه اگر (isoct (*p))
    {
      /* Parse \0ooo (if octal_0 && *p == '0') or \ooo (otherwise).
         Allow \ooo if octal_0 && *p != '0'; this is an undocumented
         extension to POSIX that is compatible with Bash 2.05b.  */
      برای (esc_length = 0, p += octal_0 && *p == '0';
           esc_length < 3 && isoct (*p);
           ++esc_length, ++p)
        esc_مقدار = esc_مقدار * 8 + fromoct (*p);
      putchar (esc_مقدار);
    }
  وگرنه اگر (*p && strchr ("\"\\abcefnrtv", *p))
    print_esc_char (*p++);
  وگرنه اگر (*p == 'u' || *p == 'U')
    {
      نویسه esc_char = *p;
      unsigned صحیح uni_مقدار;

      uni_مقدار = 0;
      برای (esc_length = (esc_char == 'u' ? 4 : 8), ++p;
           esc_length > 0;
           --esc_length, ++p)
        {
          اگر (! c_isxdigit (*p))
            error (EXIT_FAILURE, 0, _("missing hexadecimal number in escape"));
          uni_مقدار = uni_مقدار * 16 + fromhex (*p);
        }

      /* Error for invalid code points 0000D800 through 0000DFFF inclusive.
         Note print_unicode_char() would print the literal \u.. in this case. */
      اگر (uni_مقدار >= 0xd800 && uni_مقدار <= 0xdfff)
        error (EXIT_FAILURE, 0, _("invalid universal character name \\%c%0*x"),
               esc_char, (esc_char == 'u' ? 4 : 8), uni_مقدار);

      print_unicode_char (stdout, uni_مقدار, 0);
    }
  وگرنه
    {
      putchar ('\\');
      اگر (*p)
        {
          putchar (*p);
          p++;
        }
    }
  بازگشت p - escstart - 1;
}

/* Print string STR, evaluating \ escapes. */

ایستا تهی
print_esc_string (نویسه ثابت *str)
{
  برای (; *str; str++)
    اگر (*str == '\\')
      str += print_esc (str, true);
    وگرنه
      putchar (*str);
}

/* Evaluate a printf conversion specification.  START is the start of
   the directive, and CONVERSION specifies the type of conversion.
   FIELD_WIDTH and PRECISION are the field width and precision for '*'
   values, if HAVE_FIELD_WIDTH and HAVE_PRECISION are true, respectively.
   ARGUMENT is the argument to be formatted.  */

ایستا تهی
print_direc (نویسه ثابت *start, نویسه conversion,
             bool have_field_width, صحیح field_width,
             bool have_precision, صحیح precision,
             نویسه ثابت *argument)
{
  نویسه *p;		/* Null-terminated copy of % directive. */

  /* Create a null-terminated copy of the % directive, with an
     intmax_t-wide length modifier substituted for any existing
     integer length modifier.  */
  {
    نویسه *q;
    نویسه ثابت *length_modifier;
    size_t length_modifier_len;

    انتخاب (conversion)
      {
      حالت 'd': حالت 'i': حالت 'o': حالت 'u': حالت 'x': حالت 'X':
        length_modifier = "j";
        length_modifier_len = 1;
        بشکن;

      حالت 'a': حالت 'e': حالت 'f': حالت 'g':
      حالت 'A': حالت 'E': حالت 'F': حالت 'G':
        length_modifier = "L";
        length_modifier_len = 1;
        بشکن;

      پیشفرض:
        length_modifier = start;  /* Any valid pointer will do.  */
        length_modifier_len = 0;
        بشکن;
      }

    size_t length = strlen (start);
    p = xmalloc (length + length_modifier_len + 2);
    q = mempcpy (p, start, length);
    q = mempcpy (q, length_modifier, length_modifier_len);
    *q++ = conversion;
    *q = '\0';
  }

  انتخاب (conversion)
    {
    حالت 'd':
    حالت 'i':
      {
        intmax_t arg = argument ? vstrtoimax (argument) : 0;
        اگر (!have_field_width)
          {
            اگر (!have_precision)
              xprintf (p, arg);
            وگرنه
              xprintf (p, precision, arg);
          }
        وگرنه
          {
            اگر (!have_precision)
              xprintf (p, field_width, arg);
            وگرنه
              xprintf (p, field_width, precision, arg);
          }
      }
      بشکن;

    حالت 'o':
    حالت 'u':
    حالت 'x':
    حالت 'X':
      {
        uintmax_t arg = argument ? vstrtoumax (argument) : 0;
        اگر (!have_field_width)
          {
            اگر (!have_precision)
              xprintf (p, arg);
            وگرنه
              xprintf (p, precision, arg);
          }
        وگرنه
          {
            اگر (!have_precision)
              xprintf (p, field_width, arg);
            وگرنه
              xprintf (p, field_width, precision, arg);
          }
      }
      بشکن;

    حالت 'a':
    حالت 'A':
    حالت 'e':
    حالت 'E':
    حالت 'f':
    حالت 'F':
    حالت 'g':
    حالت 'G':
      {
        long دوگانه arg = argument ? vstrtold (argument) : 0;
        اگر (!have_field_width)
          {
            اگر (!have_precision)
              xprintf (p, arg);
            وگرنه
              xprintf (p, precision, arg);
          }
        وگرنه
          {
            اگر (!have_precision)
              xprintf (p, field_width, arg);
            وگرنه
              xprintf (p, field_width, precision, arg);
          }
      }
      بشکن;

    حالت 'c':
      {
        نویسه c = argument ? *argument : '\0';
        اگر (!have_field_width)
          xprintf (p, c);
        وگرنه
          xprintf (p, field_width, c);
      }
      بشکن;

    حالت 's':
      اگر (!argument)
        argument = "";
      اگر (!have_field_width)
        {
          اگر (!have_precision)
            xprintf (p, argument);
          وگرنه
            xprintf (p, precision, argument);
        }
      وگرنه
        {
          اگر (!have_precision)
            xprintf (p, field_width, argument);
          وگرنه
            xprintf (p, field_width, precision, argument);
        }
      بشکن;
    }

  آزاد (p);
}

/* Set curr_arg from indexed %i$ or otherwise next in sequence.
   POS can be 0,1,2,3 corresponding to
   [%][width][.precision][conversion] respectively.  */

ساختار arg_cursor
{
  نویسه ثابت *f;	/* Pointer into 'format'.  */
  صحیح curr_arg;		/* Current offset.  */
  صحیح curr_s_arg;	/* Current sequential offset.  */
  صحیح end_arg;		/* End arg processed.  */
  صحیح direc_arg;	/* Arg for main directive.  */
};
ATTRIBUTE_PURE ایستا ساختار arg_cursor
get_curr_arg (صحیح pos, ساختار arg_cursor ac)
{
  /* Convert sequences like "123$" by hand to avoid problems with strtol,
     which might treat "$" as part of the number in some locales.  */
  صحیح arg = 0;
  نویسه ثابت *f = ac.f;
  اگر (pos < 3 && c_isdigit (*f))
    {
      bool v = false;
      صحیح a = *f++ - '0';
      برای (; c_isdigit (*f); f++)
        {
          v |= ckd_mul (&a, a, 10);
          v |= ckd_add (&a, a, *f - '0');
        }
      اگر (*f == '$')
        arg = v ? INT_MAX : a;
    }

  اگر (0 < arg)
    {
      /* Process indexed %i$ format.  */
      arg--;
      ac.f = f + 1;
      اگر (pos == 0)
        ac.direc_arg = arg;
    }
  وگرنه
    {
      /* Process sequential arg.  */
      arg = (pos == 0 ? (ac.direc_arg = -1)
             : pos < 3 || ac.direc_arg < 0 ? ++ac.curr_s_arg
             : ac.direc_arg);
    }

  اگر (0 <= arg)
    {
      ac.curr_arg = arg;
      ac.end_arg = MAX (ac.end_arg, arg);
    }
  بازگشت ac;
}

/* Print the text in FORMAT, using ARGV (with ARGC elements) for
   arguments to any '%' directives.
   Return the number of elements of ARGV used.  */

ایستا صحیح
print_formatted (نویسه ثابت *format, صحیح argc, نویسه **argv)
{
  ساختار arg_cursor ac;
  ac.curr_arg = ac.curr_s_arg = ac.end_arg = ac.direc_arg = -1;
  نویسه ثابت *direc_start;	/* Start of % directive.  */
  نویسه *direc;			/* Generated % directive.  */
  نویسه *pdirec;			/* Pointer to current end of directive.  */
  bool have_field_width;	/* True if FIELD_WIDTH is valid.  */
  صحیح field_width = 0;		/* Arg to first '*'.  */
  bool have_precision;		/* True if PRECISION is valid.  */
  صحیح precision = 0;		/* Arg to second '*'.  */
  نویسه ok[UCHAR_MAX + 1];	/* ok['x'] is true if %x is allowed.  */

  direc = xmalloc (strlen (format) + 1);

  برای (ac.f = format; *ac.f; ac.f++)
    {
      انتخاب (*ac.f)
        {
        حالت '%':
          direc_start = ac.f;
          pdirec = direc;
          *pdirec++ = *ac.f++;
          have_field_width = have_precision = false;
          اگر (*ac.f == '%')
            {
              putchar ('%');
              بشکن;
            }

          ac = get_curr_arg (0, ac);

          اگر (*ac.f == 'b')
            {
              /* FIXME: Field width and precision are not supported
                 for %b, even though POSIX requires it.  */
              ac = get_curr_arg (3, ac);
              اگر (ac.curr_arg < argc)
                print_esc_string (argv[ac.curr_arg]);
              بشکن;
            }

          اگر (*ac.f == 'q')
            {
              ac = get_curr_arg (3, ac);
              اگر (ac.curr_arg < argc)
                {
                  fputs (quotearg_style (shell_escape_quoting_style,
                                         argv[ac.curr_arg]), stdout);
                }
              بشکن;
            }

          memset (ok, 0, اندازه ok);
          ok['a'] = ok['A'] = ok['c'] = ok['d'] = ok['e'] = ok['E'] =
            ok['f'] = ok['F'] = ok['g'] = ok['G'] = ok['i'] = ok['o'] =
            ok['s'] = ok['u'] = ok['x'] = ok['X'] = 1;

          برای (;; ac.f++)
            {
              انتخاب (*ac.f)
                {
#اگر (__GLIBC__ == 2 && 2 <= __GLIBC_MINOR__) || 3 <= __GLIBC__
                حالت 'I':
#endif
                حالت '\'':
                  ok['a'] = ok['A'] = ok['c'] = ok['e'] = ok['E'] =
                    ok['o'] = ok['s'] = ok['x'] = ok['X'] = 0;
                  بشکن;
                حالت '-': حالت '+': حالت ' ':
                  بشکن;
                حالت '#':
                  ok['c'] = ok['d'] = ok['i'] = ok['s'] = ok['u'] = 0;
                  بشکن;
                حالت '0':
                  ok['c'] = ok['s'] = 0;
                  بشکن;
                پیشفرض:
                  برو no_more_flag_characters;
                }
              *pdirec++ = *ac.f;
            }
        no_more_flag_characters:

          اگر (*ac.f == '*')
            {
              *pdirec++ = *ac.f++;

              ac = get_curr_arg (1, ac);

              اگر (ac.curr_arg < argc)
                {
                  intmax_t width = vstrtoimax (argv[ac.curr_arg]);
                  اگر (INT_MIN <= width && width <= INT_MAX)
                    field_width = width;
                  وگرنه
                    error (EXIT_FAILURE, 0, _("invalid field width: %s"),
                           quote (argv[ac.curr_arg]));
                }
              وگرنه
                field_width = 0;
              have_field_width = true;
            }
          وگرنه
            وقتی (c_isdigit (*ac.f))
              *pdirec++ = *ac.f++;
          اگر (*ac.f == '.')
            {
              *pdirec++ = *ac.f++;
              ok['c'] = 0;
              اگر (*ac.f == '*')
                {
                  *pdirec++ = *ac.f++;

                  ac = get_curr_arg (2, ac);

                  اگر (ac.curr_arg < argc)
                    {
                      intmax_t prec = vstrtoimax (argv[ac.curr_arg]);
                      اگر (prec < 0)
                        {
                          /* A negative precision is taken as if the
                             precision were omitted, so -1 is safe
                             here even if prec < INT_MIN.  */
                          precision = -1;
                        }
                      وگرنه اگر (INT_MAX < prec)
                        error (EXIT_FAILURE, 0, _("invalid precision: %s"),
                               quote (argv[ac.curr_arg]));
                      وگرنه
                        precision = prec;
                    }
                  وگرنه
                    precision = 0;
                  have_precision = true;
                }
              وگرنه
                وقتی (c_isdigit (*ac.f))
                  *pdirec++ = *ac.f++;
            }

          *pdirec++ = '\0';

          وقتی (*ac.f == 'l' || *ac.f == 'L' || *ac.f == 'h'
                 || *ac.f == 'j' || *ac.f == 't' || *ac.f == 'z')
            ++ac.f;

          {
            unsigned نویسه conversion = *ac.f;
            صحیح speclen = MIN (ac.f + 1 - direc_start, INT_MAX);
            اگر (! ok[conversion])
              error (EXIT_FAILURE, 0,
                     _("%.*s: invalid conversion specification"),
                     speclen, direc_start);
          }

          ac = get_curr_arg (3, ac);

          print_direc (direc, *ac.f,
                       have_field_width, field_width,
                       have_precision, precision,
                       ac.curr_arg < argc ? argv[ac.curr_arg] : پوچ);

          بشکن;

        حالت '\\':
          ac.f += print_esc (ac.f, false);
          بشکن;

        پیشفرض:
          putchar (*ac.f);
        }
    }

  آزاد (direc);
  بازگشت MIN (argc, ac.end_arg + 1);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  نویسه *format;
  صحیح args_used;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  posixly_correct = (getenv ("POSIXLY_CORRECT") != پوچ);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  اگر (argc == 2)
    {
      اگر (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      اگر (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (نویسه *) پوچ);
          بازگشت EXIT_SUCCESS;
        }
    }

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  اگر (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  اگر (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  format = argv[1];
  argc -= 2;
  argv += 2;

  انجامبده
    {
      args_used = print_formatted (format, argc, argv);
      argc -= args_used;
      argv += args_used;
    }
  وقتی (args_used > 0 && argc > 0);

  اگر (argc > 0)
    error (0, 0,
           _("warning: ignoring excess arguments, starting with %s"),
           quote (argv[0]));

  بازگشت exit_status;
}
