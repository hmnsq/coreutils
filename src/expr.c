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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن "system.h"

#ضمن <gmp.h>
#ضمن <regex.h>
#ضمن "c-ctype.h"
#ضمن "long-options.h"
#ضمن "mcel.h"
#ضمن "strnumcmp.h"

/* Various parts of this code assume size_t fits into unsigned long
   int, the widest unsigned type that GMP supports.  */
static_assert (SIZE_MAX <= ULONG_MAX);

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "expr"

#تعریف_کن AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("James Youngman"), \
  proper_name ("Paul Eggert")

/* Exit statuses.  */
تعداد
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
تعداد valtype
{
  integer,
  string
};
تعريفنوع تعداد valtype TYPE;

/* A value is.... */
بنية valinfo
{
  TYPE type;			/* Which kind. */
  اتحاد
  {				/* The value itself. */
    mpz_t i;
    حرف *s;
  } u;
};
تعريفنوع بنية valinfo مقدار_2;

/* The arguments given to the program, minus the program name.  */
ساكن حرف **آرگومانها;

ساكن مقدار_2 *eval (bool);
ساكن bool nomoreargs (فراغ);
ساكن bool null (مقدار_2 *v);
ساكن فراغ printv (مقدار_2 *v);


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
ساكن size_t
mbs_logical_cspn (حرف ثابت *s, حرف ثابت *accept)
{
  إذا (accept[0] == '\0')
    أرجع 0;

  /* General case.  */
  إذا (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      لكل (حرف ثابت *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          إذا (g.len == 1)
            {
              إذا (mbschr (accept, *p))
                أرجع idx;
            }
          وإلا
            لكل (حرف ثابت *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                إذا (mcel_eq (g, h))
                  أرجع idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  وإلا
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      إذا (s[i])
        أرجع i + 1;
    }

  /* not found */
  أرجع 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
ساكن حرف *
mbs_logical_substr (حرف ثابت *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? طول_رشته (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  حرف ثابت *substart = s;
  idx_t sublen = 0;
  إذا (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  وإلا إذا (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  وإلا
    لكل (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_بایتها = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        إذا (pos <= idx)
          {
            إذا (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_بایتها;
          }

        s += char_بایتها;
      }

  أرجع ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
ساكن size_t
mbs_جابه_جایی_مقصد_chars (حرف ثابت *s, size_t ofs)
{
  size_t c = 0;
  لكل (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  أرجع c;
}



فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s عبارت\n\
  or:  %s OPTION\n\
"),
              برنامه_name, برنامه_name);
      حرف_چاپ_کن ('\n');
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Print the مقدار از عبارت مقصد standard output.  A blank سطر زیر\n\
separates increasing precedence groups.  عبارت may be:\n\
\n\
  ARG1 | ARG2       ARG1 إذا it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 إذا neither آرگومان is null or 0, otherwise 0\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 < ARG2       ARG1 is less than ARG2\n\
  ARG1 <= ARG2      ARG1 is less than or equal مقصد ARG2\n\
  ARG1 = ARG2       ARG1 is equal مقصد ARG2\n\
  ARG1 != ARG2      ARG1 is unequal مقصد ARG2\n\
  ARG1 >= ARG2      ARG1 is greater than or equal مقصد ARG2\n\
  ARG1 > ARG2       ARG1 is greater than ARG2\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 + ARG2       arithmetic جمع از ARG1 and ARG2\n\
  ARG1 - ARG2       arithmetic difference از ARG1 and ARG2\n\
"), stdout);
      /* Tell xgettext that the "% A" below is not a printf-style
         format string:  xgettext:no-c-format */
      fputs (_("\
\n\
  ARG1 * ARG2       arithmetic حاصلضرب از ARG1 and ARG2\n\
  ARG1 / ARG2       arithmetic quotient از ARG1 divided by ARG2\n\
  ARG1 % ARG2       arithmetic remainder از ARG1 divided by ARG2\n\
"), stdout);
      fputs (_("\
\n\
  STRING : REGEXP   anchored الگو_2 تطابق از REGEXP درون STRING\n\
\n\
  تطابق STRING REGEXP        same as STRING : REGEXP\n\
  substr STRING POS درازی_2   substring از STRING, POS counted from 1\n\
  index STRING CHARS         index درون STRING where any CHARS is یافت, or 0\n\
  درازی STRING              درازی از STRING\n\
"), stdout);
      fputs (_("\
  + TOKEN                    interpret TOKEN as a string, even إذا it is a\n\
                               keyword like 'match' or an عملگر like '/'\n\
\n\
  ( عبارت )             مقدار از عبارت\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need مقصد be escaped or quoted لكل shells.\n\
Comparisons are arithmetic إذا both آرگومانها_2 are numbers, وإلا lexicographical.\n\
الگو_3 matches أرجع the string matched between \\( and \\) or null; إذا\n\
\\( and \\) are not استفاده_شده, they أرجع the عدد از characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit وضعیت is 0 إذا عبارت is neither null nor 0, 1 إذا عبارت is null\n\
or 0, 2 إذا عبارت is syntactically invalid, and 3 إذا an error occurred.\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}


صحيح
اصلی (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (عبارت_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, برنامه_NAME, PACKAGE_NAME, نسخه_2,
                      usage, AUTHORS, (حرف ثابت *) عدم);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  إذا (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  إذا (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (عبارت_INVALID);
    }

  آرگومانها = argv + 1;

  مقدار_2 *v = eval (true);
  إذا (!nomoreargs ())
    error (عبارت_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *آرگومانها));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

ساكن مقدار_2 *
int_مقدار (بیعلامت دراز صحيح i)
{
  مقدار_2 *v = xmalloc (حجم *v);
  v->type = integer;
  mpz_init_مجموعه_ui (v->u.i, i);
  أرجع v;
}

/* Return a VALUE for S.  */

ساكن مقدار_2 *
str_مقدار (حرف ثابت *s)
{
  مقدار_2 *v = xmalloc (حجم *v);
  v->type = string;
  v->u.s = xstrdup (s);
  أرجع v;
}

/* Free VALUE V, including structure components.  */

ساكن فراغ
freev (مقدار_2 *v)
{
  إذا (v->type == string)
    حرر (v->u.s);
  وإلا
    mpz_clear (v->u.i);
  حرر (v);
}

/* Print VALUE V.  */

ساكن فراغ
printv (مقدار_2 *v)
{
  بدل (v->type)
    {
    حالة integer:
      mpz_بیرون_str (stdout, 10, v->u.i);
      حرف_چاپ_کن ('\n');
      اكسر;
    حالة string:
      puts (v->u.s);
      اكسر;
    افتراضي:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_خالص
ساكن bool
null (مقدار_2 *v)
{
  بدل (v->type)
    {
    حالة integer:
      أرجع mpz_sgn (v->u.i) == 0;
    حالة string:
      {
        حرف ثابت *cp = v->u.s;
        إذا (*cp == '\0')
          أرجع true;

        cp += (*cp == '-');

        افعل
          {
            إذا (*cp != '0')
              أرجع false;
          }
        طالما (*++cp);

        أرجع true;
      }
    افتراضي:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_خالص
ساكن bool
looks_like_integer (حرف ثابت *cp)
{
  cp += (*cp == '-');

  افعل
    إذا (! c_isdigit (*cp))
      أرجع false;
  طالما (*++cp);

  أرجع true;
}

/* Coerce V to a string value (can't fail).  */

ساكن فراغ
tostring (مقدار_2 *v)
{
  بدل (v->type)
    {
    حالة integer:
      {
        حرف *s = mpz_get_str (عدم, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      اكسر;
    حالة string:
      اكسر;
    افتراضي:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

ساكن bool
toarith (مقدار_2 *v)
{
  بدل (v->type)
    {
    حالة integer:
      أرجع true;
    حالة string:
      {
        حرف *s = v->u.s;

        إذا (! looks_like_integer (s))
          أرجع false;
        إذا (mpz_init_مجموعه_str (v->u.i, s, 10) != 0)
          error (عبارت_FAILURE, ERANGE, "%s", (s));
        حرر (s);
        v->type = integer;
        أرجع true;
      }
    افتراضي:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
ساكن size_t
getsize (mpz_t i)
{
  إذا (mpz_sgn (i) < 0)
    أرجع SIZE_MAX;
  إذا (mpz_میگنجد_ulong_p (i))
    {
      بیعلامت دراز صحيح ul = mpz_get_ui (i);
      إذا (ul < SIZE_MAX)
        أرجع ul;
    }
  أرجع SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

ساكن bool
nextarg (حرف ثابت *str)
{
  إذا (*آرگومانها == عدم)
    أرجع false;
  وإلا
    {
      bool r = streq (*آرگومانها, str);
      آرگومانها += r;
      أرجع r;
    }
}

/* Return true if there no more tokens.  */

ساكن bool
nomoreargs (فراغ)
{
  أرجع !*آرگومانها;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
ساكن فراغ
require_بیشتر_آرگومانها (فراغ)
{
  إذا (nomoreargs ())
    error (عبارت_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(آرگومانها - 1)));
}


#اگر_تعریف_شده EVAL_TRACE
/* Print evaluation trace and args remaining.  */

ساكن فراغ
trace (حرف ثابت *fxn)
{
  چاپ_کن ("%s:", fxn);
  لكل (حرف **a = آرگومانها; *a; a++)
    چاپ_کن (" %s", *a);
  حرف_چاپ_کن ('\n');
}
#پایان_اگر

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

ساكن مقدار_2 *
docolon (مقدار_2 *sv, مقدار_2 *pv)
{
  tostring (sv);
  tostring (pv);

  بنية re_registers re_regs;
  re_regs.رقم_regs = 0;
  re_regs.شروع = عدم;
  re_regs.انتها = عدم;

  بنية re_الگو_میانگیر re_میانگیر;
  حرف fastmap[UCHAR_MAX + 1];
  re_میانگیر.میانگیر = عدم;
  re_میانگیر.allocated = 0;
  re_میانگیر.fastmap = fastmap;
  re_میانگیر.translate = عدم;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_خالی_RANGES);
  حرف ثابت *errmsg = re_compile_الگو (pv->u.s, طول_رشته (pv->u.s),
                                           &re_میانگیر);
  إذا (errmsg)
    error (عبارت_INVALID, 0, "%s", (errmsg));
  re_میانگیر.newline_anchor = 0;

  مقدار_2 *v;
  regoff_t matchlen = re_تطابق (&re_میانگیر, sv->u.s, طول_رشته (sv->u.s), 0,
                                &re_regs);
  إذا (0 <= matchlen)
    {
      /* Were \(...\) used? */
      إذا (re_میانگیر.re_nsub > 0)
        {
          إذا (re_regs.انتها[1] < 0)
            v = str_مقدار ("");
          وإلا
            {
              sv->u.s[re_regs.انتها[1]] = '\0';
              v = str_مقدار (sv->u.s + re_regs.شروع[1]);
            }
        }
      وإلا
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_جابه_جایی_مقصد_chars (sv->u.s, matchlen));
          v = int_مقدار (i);
        }
    }
  وإلا إذا (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      إذا (re_میانگیر.re_nsub > 0)
        v = str_مقدار ("");
      وإلا
        v = int_مقدار (0);
    }
  وإلا
    error (عبارت_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  إذا (0 < re_regs.رقم_regs)
    {
      حرر (re_regs.شروع);
      حرر (re_regs.انتها);
    }
  re_میانگیر.fastmap = عدم;
  regfree (&re_میانگیر);
  أرجع v;
}

/* Handle bare operands and ( expr ) syntax.  */

ساكن مقدار_2 *
eval7 (bool evaluate)
{
#اگر_تعریف_شده EVAL_TRACE
  trace ("eval7");
#پایان_اگر
  require_بیشتر_آرگومانها ();

  إذا (nextarg ("("))
    {
      مقدار_2 *v = eval (evaluate);
      إذا (nomoreargs ())
        error (عبارت_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(آرگومانها - 1)));
      إذا (!nextarg (")"))
        error (عبارت_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *آرگومانها));
      أرجع v;
    }

  إذا (nextarg (")"))
    error (عبارت_INVALID, 0, _("syntax error: unexpected ')'"));

  أرجع str_مقدار (*آرگومانها++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

ساكن مقدار_2 *
eval6 (bool evaluate)
{
#اگر_تعریف_شده EVAL_TRACE
  trace ("eval6");
#پایان_اگر
  إذا (nextarg ("+"))
    {
      require_بیشتر_آرگومانها ();
      أرجع str_مقدار (*آرگومانها++);
    }
  وإلا إذا (nextarg ("length"))
    {
      مقدار_2 *r = eval6 (evaluate);
      tostring (r);
      مقدار_2 *v = int_مقدار (mbslen (r->u.s));
      freev (r);
      أرجع v;
    }
  وإلا إذا (nextarg ("match"))
    {
      مقدار_2 *l = eval6 (evaluate);
      مقدار_2 *r = eval6 (evaluate);
      مقدار_2 *v;
      إذا (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      وإلا
        v = l;
      freev (r);
      أرجع v;
    }
  وإلا إذا (nextarg ("index"))
    {
      مقدار_2 *l = eval6 (evaluate);
      مقدار_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      مقدار_2 *v = int_مقدار (pos);
      freev (l);
      freev (r);
      أرجع v;
    }
  وإلا إذا (nextarg ("substr"))
    {
      مقدار_2 *l = eval6 (evaluate);
      مقدار_2 *i1 = eval6 (evaluate);
      مقدار_2 *i2 = eval6 (evaluate);
      tostring (l);

      مقدار_2 *v;
      إذا (!toarith (i1) || !toarith (i2))
        v = str_مقدار ("");
      وإلا
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          حرف *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_مقدار (s);
          حرر (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      أرجع v;
    }
  وإلا
    أرجع eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

ساكن مقدار_2 *
eval5 (bool evaluate)
{
#اگر_تعریف_شده EVAL_TRACE
  trace ("eval5");
#پایان_اگر
  مقدار_2 *l = eval6 (evaluate);
  طالما (true)
    {
      إذا (nextarg (":"))
        {
          مقدار_2 *r = eval6 (evaluate);
          إذا (evaluate)
            {
              مقدار_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      وإلا
        أرجع l;
    }
}

/* Handle *, /, % operators.  */

ساكن مقدار_2 *
eval4 (bool evaluate)
{
#اگر_تعریف_شده EVAL_TRACE
  trace ("eval4");
#پایان_اگر
  مقدار_2 *l = eval5 (evaluate);
  طالما (true)
    {
      تعداد { multiply, divide, mod } fxn;

      إذا (nextarg ("*"))
        fxn = multiply;
      وإلا إذا (nextarg ("/"))
        fxn = divide;
      وإلا إذا (nextarg ("%"))
        fxn = mod;
      وإلا
        أرجع l;
      مقدار_2 *r = eval5 (evaluate);
      إذا (evaluate)
        {
          إذا (!toarith (l) || !toarith (r))
            error (عبارت_INVALID, 0, _("non-integer argument"));
          إذا (fxn != multiply && mpz_sgn (r->u.i) == 0)
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

ساكن مقدار_2 *
eval3 (bool evaluate)
{
#اگر_تعریف_شده EVAL_TRACE
  trace ("eval3");
#پایان_اگر
  مقدار_2 *l = eval4 (evaluate);
  طالما (true)
    {
      تعداد { plus, minus } fxn;

      إذا (nextarg ("+"))
        fxn = plus;
      وإلا إذا (nextarg ("-"))
        fxn = minus;
      وإلا
        أرجع l;
      مقدار_2 *r = eval4 (evaluate);
      إذا (evaluate)
        {
          إذا (!toarith (l) || !toarith (r))
            error (عبارت_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_جمع_کن : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

ساكن مقدار_2 *
eval2 (bool evaluate)
{
#اگر_تعریف_شده EVAL_TRACE
  trace ("eval2");
#پایان_اگر
  مقدار_2 *l = eval3 (evaluate);
  طالما (true)
    {
      تعداد
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      إذا (nextarg ("<"))
        fxn = less_than;
      وإلا إذا (nextarg ("<="))
        fxn = less_equal;
      وإلا إذا (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      وإلا إذا (nextarg ("!="))
        fxn = not_equal;
      وإلا إذا (nextarg (">="))
        fxn = greater_equal;
      وإلا إذا (nextarg (">"))
        fxn = greater_than;
      وإلا
        أرجع l;
      مقدار_2 *r = eval3 (evaluate);

      bool val = false;
      إذا (evaluate)
        {
          tostring (l);
          tostring (r);

          صحيح cmp;
          إذا (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          وإلا
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              إذا (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (عبارت_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          بدل (fxn)
            {
            حالة less_than:     val = (cmp <  0); اكسر;
            حالة less_equal:    val = (cmp <= 0); اكسر;
            حالة equal:         val = (cmp == 0); اكسر;
            حالة not_equal:     val = (cmp != 0); اكسر;
            حالة greater_equal: val = (cmp >= 0); اكسر;
            حالة greater_than:  val = (cmp >  0); اكسر;
            افتراضي: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_مقدار (val);
    }
}

/* Handle &.  */

ساكن مقدار_2 *
eval1 (bool evaluate)
{
#اگر_تعریف_شده EVAL_TRACE
  trace ("eval1");
#پایان_اگر
  مقدار_2 *l = eval2 (evaluate);
  طالما (true)
    {
      إذا (nextarg ("&"))
        {
          مقدار_2 *r = eval2 (evaluate && !null (l));
          إذا (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_مقدار (0);
            }
          وإلا
            freev (r);
        }
      وإلا
        أرجع l;
    }
}

/* Handle |.  */

ساكن مقدار_2 *
eval (bool evaluate)
{
#اگر_تعریف_شده EVAL_TRACE
  trace ("eval");
#پایان_اگر
  مقدار_2 *l = eval1 (evaluate);
  طالما (true)
    {
      إذا (nextarg ("|"))
        {
          مقدار_2 *r = eval1 (evaluate && null (l));
          إذا (null (l))
            {
              freev (l);
              l = r;
              إذا (null (l))
                {
                  freev (l);
                  l = int_مقدار (0);
                }
            }
          وإلا
            freev (r);
        }
      وإلا
        أرجع l;
    }
}
