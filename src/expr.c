/* expr -- evaluate expressions.
   Copyright (C) 1986-2026 Free Software Foundation, Inc.

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

/* Author: Mike Parker.
   Modified for arbitrary-precision calculation by James Youngman.

   This program evaluates expressions.  Each token (operator, operand,
   parenthesis) of the expression must be a separate argument.  The
   parser used is a reasonably general one, though any incarnation of
   it is language-specific.  It is especially nice for expressions.

   No parse tree is needed; a new node is evaluated immediately.
   One function can handle multiple operators all of equal precedence,
   provided they all associate ((x op x) op x).

   Define EVAL_TRACE to print an evaluation trace.  */

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل "system.h"

#شامل <gmp.h>
#شامل <regex.h>
#شامل "c-ctype.h"
#شامل "long-options.h"
#شامل "mcel.h"
#شامل "strnumcmp.h"

/* Various parts of this code assume size_t fits into unsigned long
   int, the widest unsigned type that GMP supports.  */
static_assert (SIZE_MAX <= ULONG_MAX);

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "expr"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("James Youngman"), \
  proper_name ("Paul Eggert")

/* Exit statuses.  */
شمارش
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    عبارت_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    عبارت_FAILURE
  };

/* The kinds of value we can have.  */
شمارش valtype
{
  integer,
  string
};
تعریفنوع شمارش valtype TYPE;

/* A value is.... */
ساختار valinfo
{
  TYPE type;			/* Which kind. */
  اجتماع
  {				/* The value itself. */
    mpz_t i;
    نویسه *s;
  } u;
};
تعریفنوع ساختار valinfo مقدار_2;

/* The arguments given to the program, minus the program name.  */
ایستا نویسه **args;

ایستا مقدار_2 *eval (bool);
ایستا bool nomoreargs (تهی);
ایستا bool null (مقدار_2 *v);
ایستا تهی printv (مقدار_2 *v);


/*
   Find the first occurrence in the character string STRING of any character
   in the character string ACCEPT.

   Copied from gnulib's mbscspn, with two differences:
   1. Returns 1-based position of first found character, or zero if not found.
   2. Returned value is the logical character index, NOT byte offset.

   Examples:
     mbs_logical_cspn ('hello','a')  => 0
     mbs_logical_cspn ('hello','h')  => 1
     mbs_logical_cspn ('hello','oe') => 1
     mbs_logical_cspn ('hello','lo') => 3

   In UTF-8 \xCE\xB1 is a single character (greek alpha):
     mbs_logical_cspn ('\xCE\xB1bc','\xCE\xB1') => 1
     mbs_logical_cspn ('\xCE\xB1bc','c') => 3 */
ایستا size_t
mbs_logical_cspn (نویسه ثابت *s, نویسه ثابت *accept)
{
  اگر (accept[0] == '\0')
    بازگشت 0;

  /* General case.  */
  اگر (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      برای (نویسه ثابت *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          اگر (g.len == 1)
            {
              اگر (mbschr (accept, *p))
                بازگشت idx;
            }
          وگرنه
            برای (نویسه ثابت *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                اگر (mcel_eq (g, h))
                  بازگشت idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  وگرنه
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      اگر (s[i])
        بازگشت i + 1;
    }

  /* not found */
  بازگشت 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
ایستا نویسه *
mbs_logical_substr (نویسه ثابت *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? strlen (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  نویسه ثابت *substart = s;
  idx_t sublen = 0;
  اگر (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  وگرنه اگر (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  وگرنه
    برای (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        اگر (pos <= idx)
          {
            اگر (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
      }

  بازگشت ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
ایستا size_t
mbs_offset_to_chars (نویسه ثابت *s, size_t ofs)
{
  size_t c = 0;
  برای (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  بازگشت c;
}



تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s EXPRESSION\n\
  or:  %s OPTION\n\
"),
              program_name, program_name);
      putchar ('\n');
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Print the مقدار of EXPRESSION to standard output.  A blank سطر below\n\
separates increasing precedence groups.  EXPRESSION may be:\n\
\n\
  ARG1 | ARG2       ARG1 اگر it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 اگر neither argument is null or 0, otherwise 0\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 < ARG2       ARG1 is less than ARG2\n\
  ARG1 <= ARG2      ARG1 is less than or equal to ARG2\n\
  ARG1 = ARG2       ARG1 is equal to ARG2\n\
  ARG1 != ARG2      ARG1 is unequal to ARG2\n\
  ARG1 >= ARG2      ARG1 is greater than or equal to ARG2\n\
  ARG1 > ARG2       ARG1 is greater than ARG2\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 + ARG2       arithmetic sum of ARG1 and ARG2\n\
  ARG1 - ARG2       arithmetic difference of ARG1 and ARG2\n\
"), stdout);
      /* Tell xgettext that the "% A" below is not a printf-style
         format string:  xgettext:no-c-format */
      fputs (_("\
\n\
  ARG1 * ARG2       arithmetic product of ARG1 and ARG2\n\
  ARG1 / ARG2       arithmetic quotient of ARG1 divided by ARG2\n\
  ARG1 % ARG2       arithmetic remainder of ARG1 divided by ARG2\n\
"), stdout);
      fputs (_("\
\n\
  STRING : REGEXP   anchored pattern match of REGEXP in STRING\n\
\n\
  match STRING REGEXP        same as STRING : REGEXP\n\
  substr STRING POS LENGTH   substring of STRING, POS counted from 1\n\
  index STRING CHARS         index in STRING where any CHARS is found, or 0\n\
  length STRING              length of STRING\n\
"), stdout);
      fputs (_("\
  + TOKEN                    interpret TOKEN as a string, even اگر it is a\n\
                               keyword like 'match' or an operator like '/'\n\
\n\
  ( EXPRESSION )             مقدار of EXPRESSION\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need to be escaped or quoted برای shells.\n\
Comparisons are arithmetic اگر both ARGs are numbers, وگرنه lexicographical.\n\
Pattern matches بازگشت the string matched between \\( and \\) or null; اگر\n\
\\( and \\) are not used, they بازگشت the عدد of characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit status is 0 اگر EXPRESSION is neither null nor 0, 1 اگر EXPRESSION is null\n\
or 0, 2 اگر EXPRESSION is syntactically invalid, and 3 اگر an error occurred.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


صحیح
اصلی (صحیح argc, نویسه **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (عبارت_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, PROGRAM_NAME, PACKAGE_NAME, VERSION,
                      usage, AUTHORS, (نویسه ثابت *) پوچ);

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
      usage (عبارت_INVALID);
    }

  args = argv + 1;

  مقدار_2 *v = eval (true);
  اگر (!nomoreargs ())
    error (عبارت_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

ایستا مقدار_2 *
int_مقدار (unsigned long صحیح i)
{
  مقدار_2 *v = xmalloc (اندازه *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  بازگشت v;
}

/* Return a VALUE for S.  */

ایستا مقدار_2 *
str_مقدار (نویسه ثابت *s)
{
  مقدار_2 *v = xmalloc (اندازه *v);
  v->type = string;
  v->u.s = xstrdup (s);
  بازگشت v;
}

/* Free VALUE V, including structure components.  */

ایستا تهی
freev (مقدار_2 *v)
{
  اگر (v->type == string)
    آزاد (v->u.s);
  وگرنه
    mpz_clear (v->u.i);
  آزاد (v);
}

/* Print VALUE V.  */

ایستا تهی
printv (مقدار_2 *v)
{
  انتخاب (v->type)
    {
    حالت integer:
      mpz_out_str (stdout, 10, v->u.i);
      putchar ('\n');
      بشکن;
    حالت string:
      puts (v->u.s);
      بشکن;
    پیشفرض:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_PURE
ایستا bool
null (مقدار_2 *v)
{
  انتخاب (v->type)
    {
    حالت integer:
      بازگشت mpz_sgn (v->u.i) == 0;
    حالت string:
      {
        نویسه ثابت *cp = v->u.s;
        اگر (*cp == '\0')
          بازگشت true;

        cp += (*cp == '-');

        انجامبده
          {
            اگر (*cp != '0')
              بازگشت false;
          }
        وقتی (*++cp);

        بازگشت true;
      }
    پیشفرض:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_PURE
ایستا bool
looks_like_integer (نویسه ثابت *cp)
{
  cp += (*cp == '-');

  انجامبده
    اگر (! c_isdigit (*cp))
      بازگشت false;
  وقتی (*++cp);

  بازگشت true;
}

/* Coerce V to a string value (can't fail).  */

ایستا تهی
tostring (مقدار_2 *v)
{
  انتخاب (v->type)
    {
    حالت integer:
      {
        نویسه *s = mpz_get_str (پوچ, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      بشکن;
    حالت string:
      بشکن;
    پیشفرض:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

ایستا bool
toarith (مقدار_2 *v)
{
  انتخاب (v->type)
    {
    حالت integer:
      بازگشت true;
    حالت string:
      {
        نویسه *s = v->u.s;

        اگر (! looks_like_integer (s))
          بازگشت false;
        اگر (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (عبارت_FAILURE, ERANGE, "%s", (s));
        آزاد (s);
        v->type = integer;
        بازگشت true;
      }
    پیشفرض:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
ایستا size_t
getsize (mpz_t i)
{
  اگر (mpz_sgn (i) < 0)
    بازگشت SIZE_MAX;
  اگر (mpz_fits_ulong_p (i))
    {
      unsigned long صحیح ul = mpz_get_ui (i);
      اگر (ul < SIZE_MAX)
        بازگشت ul;
    }
  بازگشت SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

ایستا bool
nextarg (نویسه ثابت *str)
{
  اگر (*args == پوچ)
    بازگشت false;
  وگرنه
    {
      bool r = streq (*args, str);
      args += r;
      بازگشت r;
    }
}

/* Return true if there no more tokens.  */

ایستا bool
nomoreargs (تهی)
{
  بازگشت !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
ایستا تهی
require_more_args (تهی)
{
  اگر (nomoreargs ())
    error (عبارت_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#ifdef EVAL_TRACE
/* Print evaluation trace and args remaining.  */

ایستا تهی
trace (نویسه ثابت *fxn)
{
  printf ("%s:", fxn);
  برای (نویسه **a = args; *a; a++)
    printf (" %s", *a);
  putchar ('\n');
}
#endif

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

ایستا مقدار_2 *
docolon (مقدار_2 *sv, مقدار_2 *pv)
{
  tostring (sv);
  tostring (pv);

  ساختار re_registers re_regs;
  re_regs.رقم_regs = 0;
  re_regs.start = پوچ;
  re_regs.end = پوچ;

  ساختار re_pattern_buffer re_buffer;
  نویسه fastmap[UCHAR_MAX + 1];
  re_buffer.buffer = پوچ;
  re_buffer.allocated = 0;
  re_buffer.fastmap = fastmap;
  re_buffer.translate = پوچ;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_EMPTY_RANGES);
  نویسه ثابت *errmsg = re_compile_pattern (pv->u.s, strlen (pv->u.s),
                                           &re_buffer);
  اگر (errmsg)
    error (عبارت_INVALID, 0, "%s", (errmsg));
  re_buffer.newline_anchor = 0;

  مقدار_2 *v;
  regoff_t matchlen = re_match (&re_buffer, sv->u.s, strlen (sv->u.s), 0,
                                &re_regs);
  اگر (0 <= matchlen)
    {
      /* Were \(...\) used? */
      اگر (re_buffer.re_nsub > 0)
        {
          اگر (re_regs.end[1] < 0)
            v = str_مقدار ("");
          وگرنه
            {
              sv->u.s[re_regs.end[1]] = '\0';
              v = str_مقدار (sv->u.s + re_regs.start[1]);
            }
        }
      وگرنه
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_مقدار (i);
        }
    }
  وگرنه اگر (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      اگر (re_buffer.re_nsub > 0)
        v = str_مقدار ("");
      وگرنه
        v = int_مقدار (0);
    }
  وگرنه
    error (عبارت_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  اگر (0 < re_regs.رقم_regs)
    {
      آزاد (re_regs.start);
      آزاد (re_regs.end);
    }
  re_buffer.fastmap = پوچ;
  regfree (&re_buffer);
  بازگشت v;
}

/* Handle bare operands and ( expr ) syntax.  */

ایستا مقدار_2 *
eval7 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval7");
#endif
  require_more_args ();

  اگر (nextarg ("("))
    {
      مقدار_2 *v = eval (evaluate);
      اگر (nomoreargs ())
        error (عبارت_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      اگر (!nextarg (")"))
        error (عبارت_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      بازگشت v;
    }

  اگر (nextarg (")"))
    error (عبارت_INVALID, 0, _("syntax error: unexpected ')'"));

  بازگشت str_مقدار (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

ایستا مقدار_2 *
eval6 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval6");
#endif
  اگر (nextarg ("+"))
    {
      require_more_args ();
      بازگشت str_مقدار (*args++);
    }
  وگرنه اگر (nextarg ("length"))
    {
      مقدار_2 *r = eval6 (evaluate);
      tostring (r);
      مقدار_2 *v = int_مقدار (mbslen (r->u.s));
      freev (r);
      بازگشت v;
    }
  وگرنه اگر (nextarg ("match"))
    {
      مقدار_2 *l = eval6 (evaluate);
      مقدار_2 *r = eval6 (evaluate);
      مقدار_2 *v;
      اگر (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      وگرنه
        v = l;
      freev (r);
      بازگشت v;
    }
  وگرنه اگر (nextarg ("index"))
    {
      مقدار_2 *l = eval6 (evaluate);
      مقدار_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      مقدار_2 *v = int_مقدار (pos);
      freev (l);
      freev (r);
      بازگشت v;
    }
  وگرنه اگر (nextarg ("substr"))
    {
      مقدار_2 *l = eval6 (evaluate);
      مقدار_2 *i1 = eval6 (evaluate);
      مقدار_2 *i2 = eval6 (evaluate);
      tostring (l);

      مقدار_2 *v;
      اگر (!toarith (i1) || !toarith (i2))
        v = str_مقدار ("");
      وگرنه
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          نویسه *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_مقدار (s);
          آزاد (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      بازگشت v;
    }
  وگرنه
    بازگشت eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

ایستا مقدار_2 *
eval5 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval5");
#endif
  مقدار_2 *l = eval6 (evaluate);
  وقتی (true)
    {
      اگر (nextarg (":"))
        {
          مقدار_2 *r = eval6 (evaluate);
          اگر (evaluate)
            {
              مقدار_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      وگرنه
        بازگشت l;
    }
}

/* Handle *, /, % operators.  */

ایستا مقدار_2 *
eval4 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval4");
#endif
  مقدار_2 *l = eval5 (evaluate);
  وقتی (true)
    {
      شمارش { multiply, divide, mod } fxn;

      اگر (nextarg ("*"))
        fxn = multiply;
      وگرنه اگر (nextarg ("/"))
        fxn = divide;
      وگرنه اگر (nextarg ("%"))
        fxn = mod;
      وگرنه
        بازگشت l;
      مقدار_2 *r = eval5 (evaluate);
      اگر (evaluate)
        {
          اگر (!toarith (l) || !toarith (r))
            error (عبارت_INVALID, 0, _("non-integer argument"));
          اگر (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (عبارت_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

ایستا مقدار_2 *
eval3 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval3");
#endif
  مقدار_2 *l = eval4 (evaluate);
  وقتی (true)
    {
      شمارش { plus, minus } fxn;

      اگر (nextarg ("+"))
        fxn = plus;
      وگرنه اگر (nextarg ("-"))
        fxn = minus;
      وگرنه
        بازگشت l;
      مقدار_2 *r = eval4 (evaluate);
      اگر (evaluate)
        {
          اگر (!toarith (l) || !toarith (r))
            error (عبارت_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_add : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

ایستا مقدار_2 *
eval2 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval2");
#endif
  مقدار_2 *l = eval3 (evaluate);
  وقتی (true)
    {
      شمارش
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      اگر (nextarg ("<"))
        fxn = less_than;
      وگرنه اگر (nextarg ("<="))
        fxn = less_equal;
      وگرنه اگر (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      وگرنه اگر (nextarg ("!="))
        fxn = not_equal;
      وگرنه اگر (nextarg (">="))
        fxn = greater_equal;
      وگرنه اگر (nextarg (">"))
        fxn = greater_than;
      وگرنه
        بازگشت l;
      مقدار_2 *r = eval3 (evaluate);

      bool val = false;
      اگر (evaluate)
        {
          tostring (l);
          tostring (r);

          صحیح cmp;
          اگر (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          وگرنه
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              اگر (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (عبارت_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          انتخاب (fxn)
            {
            حالت less_than:     val = (cmp <  0); بشکن;
            حالت less_equal:    val = (cmp <= 0); بشکن;
            حالت equal:         val = (cmp == 0); بشکن;
            حالت not_equal:     val = (cmp != 0); بشکن;
            حالت greater_equal: val = (cmp >= 0); بشکن;
            حالت greater_than:  val = (cmp >  0); بشکن;
            پیشفرض: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_مقدار (val);
    }
}

/* Handle &.  */

ایستا مقدار_2 *
eval1 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval1");
#endif
  مقدار_2 *l = eval2 (evaluate);
  وقتی (true)
    {
      اگر (nextarg ("&"))
        {
          مقدار_2 *r = eval2 (evaluate && !null (l));
          اگر (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_مقدار (0);
            }
          وگرنه
            freev (r);
        }
      وگرنه
        بازگشت l;
    }
}

/* Handle |.  */

ایستا مقدار_2 *
eval (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval");
#endif
  مقدار_2 *l = eval1 (evaluate);
  وقتی (true)
    {
      اگر (nextarg ("|"))
        {
          مقدار_2 *r = eval1 (evaluate && null (l));
          اگر (null (l))
            {
              freev (l);
              l = r;
              اگر (null (l))
                {
                  freev (l);
                  l = int_مقدار (0);
                }
            }
          وگرنه
            freev (r);
        }
      وگرنه
        بازگشت l;
    }
}
