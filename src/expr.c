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
#تعریف_کرو پروگرام_NAME "expr"

#تعریف_کرو AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("James Youngman"), \
  proper_name ("Paul Eggert")

/* Exit statuses.  */
شمار
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    اظہاریہ_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    اظہاریہ_FAILURE
  };

/* The kinds of value we can have.  */
شمار valtype
{
  integer,
  string
};
قسمتعریف شمار valtype TYPE;

/* A value is.... */
ساخت valinfo
{
  TYPE type;			/* Which kind. */
  اتحاد
  {				/* The value itself. */
    mpz_t i;
    حرف *s;
  } u;
};
قسمتعریف ساخت valinfo قدر_2;

/* The arguments given to the program, minus the program name.  */
ساکن حرف **دلائل;

ساکن قدر_2 *eval (bool);
ساکن bool nomoreargs (خالی);
ساکن bool null (قدر_2 *v);
ساکن خالی printv (قدر_2 *v);


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
ساکن size_t
mbs_logical_cspn (حرف ثابت *s, حرف ثابت *accept)
{
  اگر (accept[0] == '\0')
    واپس 0;

  /* General case.  */
  اگر (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      برائے (حرف ثابت *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          اگر (g.len == 1)
            {
              اگر (mbschr (accept, *p))
                واپس idx;
            }
          ورنہ
            برائے (حرف ثابت *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                اگر (mcel_eq (g, h))
                  واپس idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  ورنہ
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      اگر (s[i])
        واپس i + 1;
    }

  /* not found */
  واپس 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
ساکن حرف *
mbs_logical_substr (حرف ثابت *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? لڑی_لمبائی (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  حرف ثابت *substart = s;
  idx_t sublen = 0;
  اگر (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  ورنہ اگر (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  ورنہ
    برائے (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_بائٹیں = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        اگر (pos <= idx)
          {
            اگر (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_بائٹیں;
          }

        s += char_بائٹیں;
      }

  واپس ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
ساکن size_t
mbs_انحراف_مقصد_chars (حرف ثابت *s, size_t ofs)
{
  size_t c = 0;
  برائے (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  واپس c;
}



خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s عبارت\n\
  or:  %s OPTION\n\
"),
              پروگرام_name, پروگرام_name);
      حرف_لکھو ('\n');
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Print the قدر کا عبارت مقصد standard output.  A blank سطر نیچے\n\
separates increasing precedence groups.  عبارت may be:\n\
\n\
  ARG1 | ARG2       ARG1 اگر it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 اگر neither دلیل is null or 0, otherwise 0\n\
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
  ARG1 + ARG2       arithmetic مجموع کا ARG1 and ARG2\n\
  ARG1 - ARG2       arithmetic difference کا ARG1 and ARG2\n\
"), stdout);
      /* Tell xgettext that the "% A" below is not a printf-style
         format string:  xgettext:no-c-format */
      fputs (_("\
\n\
  ARG1 * ARG2       arithmetic حاصل_ضرب کا ARG1 and ARG2\n\
  ARG1 / ARG2       arithmetic quotient کا ARG1 divided by ARG2\n\
  ARG1 % ARG2       arithmetic remainder کا ARG1 divided by ARG2\n\
"), stdout);
      fputs (_("\
\n\
  STRING : REGEXP   anchored نمونہ_2 مطابقت کا REGEXP اندر STRING\n\
\n\
  مطابقت STRING REGEXP        same as STRING : REGEXP\n\
  substr STRING POS لمبائی_2   substring کا STRING, POS counted from 1\n\
  index STRING CHARS         index اندر STRING where any CHARS is ملا, or 0\n\
  لمبائی STRING              لمبائی کا STRING\n\
"), stdout);
      fputs (_("\
  + TOKEN                    interpret TOKEN as a string, even اگر it is a\n\
                               keyword like 'match' or an عامل like '/'\n\
\n\
  ( عبارت )             قدر کا عبارت\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need مقصد be escaped or quoted برائے shells.\n\
Comparisons are arithmetic اگر both دلائل_2 are numbers, ورنہ lexicographical.\n\
نمونہ_3 matches واپس the string matched between \\( and \\) or null; اگر\n\
\\( and \\) are not مستعمل, they واپس the نمبر کا characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit حالت_2 is 0 اگر عبارت is neither null nor 0, 1 اگر عبارت is null\n\
or 0, 2 اگر عبارت is syntactically invalid, and 3 اگر an error occurred.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}


صحیح
مین (صحیح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (اظہاریہ_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, پروگرام_NAME, PACKAGE_NAME, نسخہ_2,
                      usage, AUTHORS, (حرف ثابت *) تہی);

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
      usage (اظہاریہ_INVALID);
    }

  دلائل = argv + 1;

  قدر_2 *v = eval (true);
  اگر (!nomoreargs ())
    error (اظہاریہ_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *دلائل));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

ساکن قدر_2 *
int_قدر (غیرعلامتی طویل صحیح i)
{
  قدر_2 *v = xmalloc (حجم *v);
  v->type = integer;
  mpz_init_سیٹ_ui (v->u.i, i);
  واپس v;
}

/* Return a VALUE for S.  */

ساکن قدر_2 *
str_قدر (حرف ثابت *s)
{
  قدر_2 *v = xmalloc (حجم *v);
  v->type = string;
  v->u.s = xstrdup (s);
  واپس v;
}

/* Free VALUE V, including structure components.  */

ساکن خالی
freev (قدر_2 *v)
{
  اگر (v->type == string)
    آزاد (v->u.s);
  ورنہ
    mpz_clear (v->u.i);
  آزاد (v);
}

/* Print VALUE V.  */

ساکن خالی
printv (قدر_2 *v)
{
  انتخاب (v->type)
    {
    حالت integer:
      mpz_برآمد_str (stdout, 10, v->u.i);
      حرف_لکھو ('\n');
      توڑو;
    حالت string:
      puts (v->u.s);
      توڑو;
    طےشدہ:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_خالص
ساکن bool
null (قدر_2 *v)
{
  انتخاب (v->type)
    {
    حالت integer:
      واپس mpz_sgn (v->u.i) == 0;
    حالت string:
      {
        حرف ثابت *cp = v->u.s;
        اگر (*cp == '\0')
          واپس true;

        cp += (*cp == '-');

        کرو
          {
            اگر (*cp != '0')
              واپس false;
          }
        جب (*++cp);

        واپس true;
      }
    طےشدہ:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_خالص
ساکن bool
looks_like_integer (حرف ثابت *cp)
{
  cp += (*cp == '-');

  کرو
    اگر (! c_isdigit (*cp))
      واپس false;
  جب (*++cp);

  واپس true;
}

/* Coerce V to a string value (can't fail).  */

ساکن خالی
tostring (قدر_2 *v)
{
  انتخاب (v->type)
    {
    حالت integer:
      {
        حرف *s = mpz_get_str (تہی, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      توڑو;
    حالت string:
      توڑو;
    طےشدہ:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

ساکن bool
toarith (قدر_2 *v)
{
  انتخاب (v->type)
    {
    حالت integer:
      واپس true;
    حالت string:
      {
        حرف *s = v->u.s;

        اگر (! looks_like_integer (s))
          واپس false;
        اگر (mpz_init_سیٹ_str (v->u.i, s, 10) != 0)
          error (اظہاریہ_FAILURE, ERANGE, "%s", (s));
        آزاد (s);
        v->type = integer;
        واپس true;
      }
    طےشدہ:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
ساکن size_t
getsize (mpz_t i)
{
  اگر (mpz_sgn (i) < 0)
    واپس SIZE_MAX;
  اگر (mpz_سماتا_ulong_p (i))
    {
      غیرعلامتی طویل صحیح ul = mpz_get_ui (i);
      اگر (ul < SIZE_MAX)
        واپس ul;
    }
  واپس SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

ساکن bool
nextarg (حرف ثابت *str)
{
  اگر (*دلائل == تہی)
    واپس false;
  ورنہ
    {
      bool r = streq (*دلائل, str);
      دلائل += r;
      واپس r;
    }
}

/* Return true if there no more tokens.  */

ساکن bool
nomoreargs (خالی)
{
  واپس !*دلائل;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
ساکن خالی
require_مزید_دلائل (خالی)
{
  اگر (nomoreargs ())
    error (اظہاریہ_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(دلائل - 1)));
}


#اگر_تعریف_شدہ EVAL_TRACE
/* Print evaluation trace and args remaining.  */

ساکن خالی
trace (حرف ثابت *fxn)
{
  لکھو ("%s:", fxn);
  برائے (حرف **a = دلائل; *a; a++)
    لکھو (" %s", *a);
  حرف_لکھو ('\n');
}
#اگر_ختم

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

ساکن قدر_2 *
docolon (قدر_2 *sv, قدر_2 *pv)
{
  tostring (sv);
  tostring (pv);

  ساخت re_registers re_regs;
  re_regs.عدد_regs = 0;
  re_regs.آغاز = تہی;
  re_regs.انتہا = تہی;

  ساخت re_نمونہ_بفر re_بفر;
  حرف fastmap[UCHAR_MAX + 1];
  re_بفر.بفر = تہی;
  re_بفر.allocated = 0;
  re_بفر.fastmap = fastmap;
  re_بفر.translate = تہی;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_خالی_خانے_RANGES);
  حرف ثابت *errmsg = re_compile_نمونہ (pv->u.s, لڑی_لمبائی (pv->u.s),
                                           &re_بفر);
  اگر (errmsg)
    error (اظہاریہ_INVALID, 0, "%s", (errmsg));
  re_بفر.newline_anchor = 0;

  قدر_2 *v;
  regoff_t matchlen = re_مطابقت (&re_بفر, sv->u.s, لڑی_لمبائی (sv->u.s), 0,
                                &re_regs);
  اگر (0 <= matchlen)
    {
      /* Were \(...\) used? */
      اگر (re_بفر.re_nsub > 0)
        {
          اگر (re_regs.انتہا[1] < 0)
            v = str_قدر ("");
          ورنہ
            {
              sv->u.s[re_regs.انتہا[1]] = '\0';
              v = str_قدر (sv->u.s + re_regs.آغاز[1]);
            }
        }
      ورنہ
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_انحراف_مقصد_chars (sv->u.s, matchlen));
          v = int_قدر (i);
        }
    }
  ورنہ اگر (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      اگر (re_بفر.re_nsub > 0)
        v = str_قدر ("");
      ورنہ
        v = int_قدر (0);
    }
  ورنہ
    error (اظہاریہ_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  اگر (0 < re_regs.عدد_regs)
    {
      آزاد (re_regs.آغاز);
      آزاد (re_regs.انتہا);
    }
  re_بفر.fastmap = تہی;
  regfree (&re_بفر);
  واپس v;
}

/* Handle bare operands and ( expr ) syntax.  */

ساکن قدر_2 *
eval7 (bool evaluate)
{
#اگر_تعریف_شدہ EVAL_TRACE
  trace ("eval7");
#اگر_ختم
  require_مزید_دلائل ();

  اگر (nextarg ("("))
    {
      قدر_2 *v = eval (evaluate);
      اگر (nomoreargs ())
        error (اظہاریہ_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(دلائل - 1)));
      اگر (!nextarg (")"))
        error (اظہاریہ_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *دلائل));
      واپس v;
    }

  اگر (nextarg (")"))
    error (اظہاریہ_INVALID, 0, _("syntax error: unexpected ')'"));

  واپس str_قدر (*دلائل++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

ساکن قدر_2 *
eval6 (bool evaluate)
{
#اگر_تعریف_شدہ EVAL_TRACE
  trace ("eval6");
#اگر_ختم
  اگر (nextarg ("+"))
    {
      require_مزید_دلائل ();
      واپس str_قدر (*دلائل++);
    }
  ورنہ اگر (nextarg ("length"))
    {
      قدر_2 *r = eval6 (evaluate);
      tostring (r);
      قدر_2 *v = int_قدر (mbslen (r->u.s));
      freev (r);
      واپس v;
    }
  ورنہ اگر (nextarg ("match"))
    {
      قدر_2 *l = eval6 (evaluate);
      قدر_2 *r = eval6 (evaluate);
      قدر_2 *v;
      اگر (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      ورنہ
        v = l;
      freev (r);
      واپس v;
    }
  ورنہ اگر (nextarg ("index"))
    {
      قدر_2 *l = eval6 (evaluate);
      قدر_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      قدر_2 *v = int_قدر (pos);
      freev (l);
      freev (r);
      واپس v;
    }
  ورنہ اگر (nextarg ("substr"))
    {
      قدر_2 *l = eval6 (evaluate);
      قدر_2 *i1 = eval6 (evaluate);
      قدر_2 *i2 = eval6 (evaluate);
      tostring (l);

      قدر_2 *v;
      اگر (!toarith (i1) || !toarith (i2))
        v = str_قدر ("");
      ورنہ
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          حرف *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_قدر (s);
          آزاد (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      واپس v;
    }
  ورنہ
    واپس eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

ساکن قدر_2 *
eval5 (bool evaluate)
{
#اگر_تعریف_شدہ EVAL_TRACE
  trace ("eval5");
#اگر_ختم
  قدر_2 *l = eval6 (evaluate);
  جب (true)
    {
      اگر (nextarg (":"))
        {
          قدر_2 *r = eval6 (evaluate);
          اگر (evaluate)
            {
              قدر_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      ورنہ
        واپس l;
    }
}

/* Handle *, /, % operators.  */

ساکن قدر_2 *
eval4 (bool evaluate)
{
#اگر_تعریف_شدہ EVAL_TRACE
  trace ("eval4");
#اگر_ختم
  قدر_2 *l = eval5 (evaluate);
  جب (true)
    {
      شمار { multiply, divide, mod } fxn;

      اگر (nextarg ("*"))
        fxn = multiply;
      ورنہ اگر (nextarg ("/"))
        fxn = divide;
      ورنہ اگر (nextarg ("%"))
        fxn = mod;
      ورنہ
        واپس l;
      قدر_2 *r = eval5 (evaluate);
      اگر (evaluate)
        {
          اگر (!toarith (l) || !toarith (r))
            error (اظہاریہ_INVALID, 0, _("non-integer argument"));
          اگر (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (اظہاریہ_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

ساکن قدر_2 *
eval3 (bool evaluate)
{
#اگر_تعریف_شدہ EVAL_TRACE
  trace ("eval3");
#اگر_ختم
  قدر_2 *l = eval4 (evaluate);
  جب (true)
    {
      شمار { plus, minus } fxn;

      اگر (nextarg ("+"))
        fxn = plus;
      ورنہ اگر (nextarg ("-"))
        fxn = minus;
      ورنہ
        واپس l;
      قدر_2 *r = eval4 (evaluate);
      اگر (evaluate)
        {
          اگر (!toarith (l) || !toarith (r))
            error (اظہاریہ_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_جوڑو : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

ساکن قدر_2 *
eval2 (bool evaluate)
{
#اگر_تعریف_شدہ EVAL_TRACE
  trace ("eval2");
#اگر_ختم
  قدر_2 *l = eval3 (evaluate);
  جب (true)
    {
      شمار
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      اگر (nextarg ("<"))
        fxn = less_than;
      ورنہ اگر (nextarg ("<="))
        fxn = less_equal;
      ورنہ اگر (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      ورنہ اگر (nextarg ("!="))
        fxn = not_equal;
      ورنہ اگر (nextarg (">="))
        fxn = greater_equal;
      ورنہ اگر (nextarg (">"))
        fxn = greater_than;
      ورنہ
        واپس l;
      قدر_2 *r = eval3 (evaluate);

      bool val = false;
      اگر (evaluate)
        {
          tostring (l);
          tostring (r);

          صحیح cmp;
          اگر (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          ورنہ
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              اگر (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (اظہاریہ_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          انتخاب (fxn)
            {
            حالت less_than:     val = (cmp <  0); توڑو;
            حالت less_equal:    val = (cmp <= 0); توڑو;
            حالت equal:         val = (cmp == 0); توڑو;
            حالت not_equal:     val = (cmp != 0); توڑو;
            حالت greater_equal: val = (cmp >= 0); توڑو;
            حالت greater_than:  val = (cmp >  0); توڑو;
            طےشدہ: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_قدر (val);
    }
}

/* Handle &.  */

ساکن قدر_2 *
eval1 (bool evaluate)
{
#اگر_تعریف_شدہ EVAL_TRACE
  trace ("eval1");
#اگر_ختم
  قدر_2 *l = eval2 (evaluate);
  جب (true)
    {
      اگر (nextarg ("&"))
        {
          قدر_2 *r = eval2 (evaluate && !null (l));
          اگر (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_قدر (0);
            }
          ورنہ
            freev (r);
        }
      ورنہ
        واپس l;
    }
}

/* Handle |.  */

ساکن قدر_2 *
eval (bool evaluate)
{
#اگر_تعریف_شدہ EVAL_TRACE
  trace ("eval");
#اگر_ختم
  قدر_2 *l = eval1 (evaluate);
  جب (true)
    {
      اگر (nextarg ("|"))
        {
          قدر_2 *r = eval1 (evaluate && null (l));
          اگر (null (l))
            {
              freev (l);
              l = r;
              اگر (null (l))
                {
                  freev (l);
                  l = int_قدر (0);
                }
            }
          ورنہ
            freev (r);
        }
      ورنہ
        واپس l;
    }
}
