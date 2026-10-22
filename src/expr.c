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
#تعریف_کړه پروګرام_NAME "expr"

#تعریف_کړه AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("James Youngman"), \
  proper_name ("Paul Eggert")

/* Exit statuses.  */
تعداد
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    بیان_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    بیان_FAILURE
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
تعريفنوع بنية valinfo ارزښت_2;

/* The arguments given to the program, minus the program name.  */
ساكن حرف **دلیلونه;

ساكن ارزښت_2 *eval (bool);
ساكن bool nomoreargs (فراغ);
ساكن bool null (ارزښت_2 *v);
ساكن فراغ printv (ارزښت_2 *v);


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
  idx_t llen = mb_cur_max <= 1 ? مزي_اوږدوالی (s) : mbslen (s); /* logical length */

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
        idx_t char_بایټونه = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        إذا (pos <= idx)
          {
            إذا (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_بایټونه;
          }

        s += char_بایټونه;
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
mbs_دننه_واټن_منزل_chars (حرف ثابت *s, size_t ofs)
{
  size_t c = 0;
  لكل (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  أرجع c;
}



فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s افاده\n\
  or:  %s OPTION\n\
"),
              پروګرام_name, پروګرام_name);
      توری_چاپ_کړه ('\n');
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Print the ارزښت د افاده منزل standard output.  A blank کرښه لاندې\n\
separates increasing precedence groups.  افاده may be:\n\
\n\
  ARG1 | ARG2       ARG1 إذا it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 إذا neither دلیل is null or 0, otherwise 0\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 < ARG2       ARG1 is less than ARG2\n\
  ARG1 <= ARG2      ARG1 is less than or equal منزل ARG2\n\
  ARG1 = ARG2       ARG1 is equal منزل ARG2\n\
  ARG1 != ARG2      ARG1 is unequal منزل ARG2\n\
  ARG1 >= ARG2      ARG1 is greater than or equal منزل ARG2\n\
  ARG1 > ARG2       ARG1 is greater than ARG2\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 + ARG2       arithmetic مجموعه د ARG1 and ARG2\n\
  ARG1 - ARG2       arithmetic difference د ARG1 and ARG2\n\
"), stdout);
      /* Tell xgettext that the "% A" below is not a printf-style
         format string:  xgettext:no-c-format */
      fputs (_("\
\n\
  ARG1 * ARG2       arithmetic حاصل_ضرب د ARG1 and ARG2\n\
  ARG1 / ARG2       arithmetic quotient د ARG1 divided by ARG2\n\
  ARG1 % ARG2       arithmetic remainder د ARG1 divided by ARG2\n\
"), stdout);
      fputs (_("\
\n\
  STRING : REGEXP   anchored نمونه_2 سمون د REGEXP دننه STRING\n\
\n\
  سمون STRING REGEXP        same as STRING : REGEXP\n\
  substr STRING POS اوږدتیا_2   substring د STRING, POS counted from 1\n\
  index STRING CHARS         index دننه STRING where any CHARS is وموندل, or 0\n\
  اوږدتیا STRING              اوږدتیا د STRING\n\
"), stdout);
      fputs (_("\
  + TOKEN                    interpret TOKEN as a string, even إذا it is a\n\
                               keyword like 'match' or an چلوونکی like '/'\n\
\n\
  ( افاده )             ارزښت د افاده\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need منزل be escaped or quoted لكل shells.\n\
Comparisons are arithmetic إذا both دلیلونه_2 are numbers, وإلا lexicographical.\n\
نمونه_3 matches أرجع the string matched between \\( and \\) or null; إذا\n\
\\( and \\) are not کارول_شوی, they أرجع the شمېره د characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit حالت is 0 إذا افاده is neither null nor 0, 1 إذا افاده is null\n\
or 0, 2 إذا افاده is syntactically invalid, and 3 إذا an error occurred.\n\
"), stdout);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}


صحيح
اصلي (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (بیان_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, پروګرام_NAME, PACKAGE_NAME, نسخه_2,
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
      usage (بیان_INVALID);
    }

  دلیلونه = argv + 1;

  ارزښت_2 *v = eval (true);
  إذا (!nomoreargs ())
    error (بیان_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *دلیلونه));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

ساكن ارزښت_2 *
int_ارزښت (بینښه اوږد صحيح i)
{
  ارزښت_2 *v = xmalloc (حجم *v);
  v->type = integer;
  mpz_init_سېټ_ui (v->u.i, i);
  أرجع v;
}

/* Return a VALUE for S.  */

ساكن ارزښت_2 *
str_ارزښت (حرف ثابت *s)
{
  ارزښت_2 *v = xmalloc (حجم *v);
  v->type = string;
  v->u.s = xstrdup (s);
  أرجع v;
}

/* Free VALUE V, including structure components.  */

ساكن فراغ
freev (ارزښت_2 *v)
{
  إذا (v->type == string)
    حرر (v->u.s);
  وإلا
    mpz_clear (v->u.i);
  حرر (v);
}

/* Print VALUE V.  */

ساكن فراغ
printv (ارزښت_2 *v)
{
  بدل (v->type)
    {
    حالة integer:
      mpz_بهر_str (stdout, 10, v->u.i);
      توری_چاپ_کړه ('\n');
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
null (ارزښت_2 *v)
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
tostring (ارزښت_2 *v)
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
toarith (ارزښت_2 *v)
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
        إذا (mpz_init_سېټ_str (v->u.i, s, 10) != 0)
          error (بیان_FAILURE, ERANGE, "%s", (s));
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
  إذا (mpz_ځاییږي_ulong_p (i))
    {
      بینښه اوږد صحيح ul = mpz_get_ui (i);
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
  إذا (*دلیلونه == عدم)
    أرجع false;
  وإلا
    {
      bool r = streq (*دلیلونه, str);
      دلیلونه += r;
      أرجع r;
    }
}

/* Return true if there no more tokens.  */

ساكن bool
nomoreargs (فراغ)
{
  أرجع !*دلیلونه;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
ساكن فراغ
require_نور_دلیلونه (فراغ)
{
  إذا (nomoreargs ())
    error (بیان_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(دلیلونه - 1)));
}


#که_تعریف_شوی EVAL_TRACE
/* Print evaluation trace and args remaining.  */

ساكن فراغ
trace (حرف ثابت *fxn)
{
  چاپ_کړه ("%s:", fxn);
  لكل (حرف **a = دلیلونه; *a; a++)
    چاپ_کړه (" %s", *a);
  توری_چاپ_کړه ('\n');
}
#د_که_پای

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

ساكن ارزښت_2 *
docolon (ارزښت_2 *sv, ارزښت_2 *pv)
{
  tostring (sv);
  tostring (pv);

  بنية re_registers re_regs;
  re_regs.عدد_regs = 0;
  re_regs.پیل = عدم;
  re_regs.پای_ټکی = عدم;

  بنية re_نمونه_بفر re_بفر;
  حرف fastmap[UCHAR_MAX + 1];
  re_بفر.بفر = عدم;
  re_بفر.allocated = 0;
  re_بفر.fastmap = fastmap;
  re_بفر.translate = عدم;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_تشه_RANGES);
  حرف ثابت *errmsg = re_compile_نمونه (pv->u.s, مزي_اوږدوالی (pv->u.s),
                                           &re_بفر);
  إذا (errmsg)
    error (بیان_INVALID, 0, "%s", (errmsg));
  re_بفر.newline_anchor = 0;

  ارزښت_2 *v;
  regoff_t matchlen = re_سمون (&re_بفر, sv->u.s, مزي_اوږدوالی (sv->u.s), 0,
                                &re_regs);
  إذا (0 <= matchlen)
    {
      /* Were \(...\) used? */
      إذا (re_بفر.re_nsub > 0)
        {
          إذا (re_regs.پای_ټکی[1] < 0)
            v = str_ارزښت ("");
          وإلا
            {
              sv->u.s[re_regs.پای_ټکی[1]] = '\0';
              v = str_ارزښت (sv->u.s + re_regs.پیل[1]);
            }
        }
      وإلا
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_دننه_واټن_منزل_chars (sv->u.s, matchlen));
          v = int_ارزښت (i);
        }
    }
  وإلا إذا (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      إذا (re_بفر.re_nsub > 0)
        v = str_ارزښت ("");
      وإلا
        v = int_ارزښت (0);
    }
  وإلا
    error (بیان_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  إذا (0 < re_regs.عدد_regs)
    {
      حرر (re_regs.پیل);
      حرر (re_regs.پای_ټکی);
    }
  re_بفر.fastmap = عدم;
  regfree (&re_بفر);
  أرجع v;
}

/* Handle bare operands and ( expr ) syntax.  */

ساكن ارزښت_2 *
eval7 (bool evaluate)
{
#که_تعریف_شوی EVAL_TRACE
  trace ("eval7");
#د_که_پای
  require_نور_دلیلونه ();

  إذا (nextarg ("("))
    {
      ارزښت_2 *v = eval (evaluate);
      إذا (nomoreargs ())
        error (بیان_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(دلیلونه - 1)));
      إذا (!nextarg (")"))
        error (بیان_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *دلیلونه));
      أرجع v;
    }

  إذا (nextarg (")"))
    error (بیان_INVALID, 0, _("syntax error: unexpected ')'"));

  أرجع str_ارزښت (*دلیلونه++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

ساكن ارزښت_2 *
eval6 (bool evaluate)
{
#که_تعریف_شوی EVAL_TRACE
  trace ("eval6");
#د_که_پای
  إذا (nextarg ("+"))
    {
      require_نور_دلیلونه ();
      أرجع str_ارزښت (*دلیلونه++);
    }
  وإلا إذا (nextarg ("length"))
    {
      ارزښت_2 *r = eval6 (evaluate);
      tostring (r);
      ارزښت_2 *v = int_ارزښت (mbslen (r->u.s));
      freev (r);
      أرجع v;
    }
  وإلا إذا (nextarg ("match"))
    {
      ارزښت_2 *l = eval6 (evaluate);
      ارزښت_2 *r = eval6 (evaluate);
      ارزښت_2 *v;
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
      ارزښت_2 *l = eval6 (evaluate);
      ارزښت_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      ارزښت_2 *v = int_ارزښت (pos);
      freev (l);
      freev (r);
      أرجع v;
    }
  وإلا إذا (nextarg ("substr"))
    {
      ارزښت_2 *l = eval6 (evaluate);
      ارزښت_2 *i1 = eval6 (evaluate);
      ارزښت_2 *i2 = eval6 (evaluate);
      tostring (l);

      ارزښت_2 *v;
      إذا (!toarith (i1) || !toarith (i2))
        v = str_ارزښت ("");
      وإلا
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          حرف *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_ارزښت (s);
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

ساكن ارزښت_2 *
eval5 (bool evaluate)
{
#که_تعریف_شوی EVAL_TRACE
  trace ("eval5");
#د_که_پای
  ارزښت_2 *l = eval6 (evaluate);
  طالما (true)
    {
      إذا (nextarg (":"))
        {
          ارزښت_2 *r = eval6 (evaluate);
          إذا (evaluate)
            {
              ارزښت_2 *v = docolon (l, r);
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

ساكن ارزښت_2 *
eval4 (bool evaluate)
{
#که_تعریف_شوی EVAL_TRACE
  trace ("eval4");
#د_که_پای
  ارزښت_2 *l = eval5 (evaluate);
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
      ارزښت_2 *r = eval5 (evaluate);
      إذا (evaluate)
        {
          إذا (!toarith (l) || !toarith (r))
            error (بیان_INVALID, 0, _("non-integer argument"));
          إذا (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (بیان_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

ساكن ارزښت_2 *
eval3 (bool evaluate)
{
#که_تعریف_شوی EVAL_TRACE
  trace ("eval3");
#د_که_پای
  ارزښت_2 *l = eval4 (evaluate);
  طالما (true)
    {
      تعداد { plus, minus } fxn;

      إذا (nextarg ("+"))
        fxn = plus;
      وإلا إذا (nextarg ("-"))
        fxn = minus;
      وإلا
        أرجع l;
      ارزښت_2 *r = eval4 (evaluate);
      إذا (evaluate)
        {
          إذا (!toarith (l) || !toarith (r))
            error (بیان_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_جمع_کړه : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

ساكن ارزښت_2 *
eval2 (bool evaluate)
{
#که_تعریف_شوی EVAL_TRACE
  trace ("eval2");
#د_که_پای
  ارزښت_2 *l = eval3 (evaluate);
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
      ارزښت_2 *r = eval3 (evaluate);

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
                  error (بیان_INVALID, 0,
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
      l = int_ارزښت (val);
    }
}

/* Handle &.  */

ساكن ارزښت_2 *
eval1 (bool evaluate)
{
#که_تعریف_شوی EVAL_TRACE
  trace ("eval1");
#د_که_پای
  ارزښت_2 *l = eval2 (evaluate);
  طالما (true)
    {
      إذا (nextarg ("&"))
        {
          ارزښت_2 *r = eval2 (evaluate && !null (l));
          إذا (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_ارزښت (0);
            }
          وإلا
            freev (r);
        }
      وإلا
        أرجع l;
    }
}

/* Handle |.  */

ساكن ارزښت_2 *
eval (bool evaluate)
{
#که_تعریف_شوی EVAL_TRACE
  trace ("eval");
#د_که_پای
  ارزښت_2 *l = eval1 (evaluate);
  طالما (true)
    {
      إذا (nextarg ("|"))
        {
          ارزښت_2 *r = eval1 (evaluate && null (l));
          إذا (null (l))
            {
              freev (l);
              l = r;
              إذا (null (l))
                {
                  freev (l);
                  l = int_ارزښت (0);
                }
            }
          وإلا
            freev (r);
        }
      وإلا
        أرجع l;
    }
}
