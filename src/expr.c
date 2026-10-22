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
#define PROGRAM_NAME "expr"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("James Youngman"), \
  proper_name ("Paul Eggert")

/* Exit statuses.  */
تعداد
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    EXPR_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    EXPR_FAILURE
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
تعريفنوع بنية valinfo VALUE;

/* The arguments given to the program, minus the program name.  */
ساكن حرف **args;

ساكن VALUE *eval (bool);
ساكن bool nomoreargs (فراغ);
ساكن bool null (VALUE *v);
ساكن فراغ printv (VALUE *v);


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
  𐡄𐡍 (accept[0] == '\0')
    أرجع 0;

  /* General case.  */
  𐡄𐡍 (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      لكل (حرف ثابت *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          𐡄𐡍 (g.len == 1)
            {
              𐡄𐡍 (mbschr (accept, *p))
                أرجع idx;
            }
          وإلا
            لكل (حرف ثابت *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                𐡄𐡍 (mcel_eq (g, h))
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
      𐡄𐡍 (s[i])
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
  idx_t llen = mb_cur_max <= 1 ? strlen (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  حرف ثابت *substart = s;
  idx_t sublen = 0;
  𐡄𐡍 (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  وإلا 𐡄𐡍 (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  وإلا
    لكل (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        𐡄𐡍 (pos <= idx)
          {
            𐡄𐡍 (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
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
mbs_offset_to_chars (حرف ثابت *s, size_t ofs)
{
  size_t c = 0;
  لكل (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  أرجع c;
}



فراغ
usage (صحيح status)
{
  𐡄𐡍 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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
Print the value of EXPRESSION to standard output.  A blank line below\n\
separates increasing precedence groups.  EXPRESSION may be:\n\
\n\
  ARG1 | ARG2       ARG1 𐡄𐡍 it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 𐡄𐡍 neither argument is null or 0, otherwise 0\n\
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
  + TOKEN                    interpret TOKEN as a string, even 𐡄𐡍 it is a\n\
                               keyword like 'match' or an operator like '/'\n\
\n\
  ( EXPRESSION )             value of EXPRESSION\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need to be escaped or quoted لكل shells.\n\
Comparisons are arithmetic 𐡄𐡍 both ARGs are numbers, وإلا lexicographical.\n\
Pattern matches أرجع the string matched between \\( and \\) or null; 𐡄𐡍\n\
\\( and \\) are not used, they أرجع the number of characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit status is 0 𐡄𐡍 EXPRESSION is neither null nor 0, 1 𐡄𐡍 EXPRESSION is null\n\
or 0, 2 𐡄𐡍 EXPRESSION is syntactically invalid, and 3 𐡄𐡍 an error occurred.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


صحيح
main (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXPR_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, PROGRAM_NAME, PACKAGE_NAME, VERSION,
                      usage, AUTHORS, (حرف ثابت *) عدم);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  𐡄𐡍 (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  𐡄𐡍 (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXPR_INVALID);
    }

  args = argv + 1;

  VALUE *v = eval (true);
  𐡄𐡍 (!nomoreargs ())
    error (EXPR_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

ساكن VALUE *
int_value (unsigned long صحيح i)
{
  VALUE *v = xmalloc (حجم *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  أرجع v;
}

/* Return a VALUE for S.  */

ساكن VALUE *
str_value (حرف ثابت *s)
{
  VALUE *v = xmalloc (حجم *v);
  v->type = string;
  v->u.s = xstrdup (s);
  أرجع v;
}

/* Free VALUE V, including structure components.  */

ساكن فراغ
freev (VALUE *v)
{
  𐡄𐡍 (v->type == string)
    حرر (v->u.s);
  وإلا
    mpz_clear (v->u.i);
  حرر (v);
}

/* Print VALUE V.  */

ساكن فراغ
printv (VALUE *v)
{
  بدل (v->type)
    {
    حالة integer:
      mpz_out_str (stdout, 10, v->u.i);
      putchar ('\n');
      اكسر;
    حالة string:
      puts (v->u.s);
      اكسر;
    افتراضي:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_PURE
ساكن bool
null (VALUE *v)
{
  بدل (v->type)
    {
    حالة integer:
      أرجع mpz_sgn (v->u.i) == 0;
    حالة string:
      {
        حرف ثابت *cp = v->u.s;
        𐡄𐡍 (*cp == '\0')
          أرجع true;

        cp += (*cp == '-');

        𐡏𐡁𐡃
          {
            𐡄𐡍 (*cp != '0')
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

ATTRIBUTE_PURE
ساكن bool
looks_like_integer (حرف ثابت *cp)
{
  cp += (*cp == '-');

  𐡏𐡁𐡃
    𐡄𐡍 (! c_isdigit (*cp))
      أرجع false;
  طالما (*++cp);

  أرجع true;
}

/* Coerce V to a string value (can't fail).  */

ساكن فراغ
tostring (VALUE *v)
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
toarith (VALUE *v)
{
  بدل (v->type)
    {
    حالة integer:
      أرجع true;
    حالة string:
      {
        حرف *s = v->u.s;

        𐡄𐡍 (! looks_like_integer (s))
          أرجع false;
        𐡄𐡍 (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (EXPR_FAILURE, ERANGE, "%s", (s));
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
  𐡄𐡍 (mpz_sgn (i) < 0)
    أرجع SIZE_MAX;
  𐡄𐡍 (mpz_fits_ulong_p (i))
    {
      unsigned long صحيح ul = mpz_get_ui (i);
      𐡄𐡍 (ul < SIZE_MAX)
        أرجع ul;
    }
  أرجع SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

ساكن bool
nextarg (حرف ثابت *str)
{
  𐡄𐡍 (*args == عدم)
    أرجع false;
  وإلا
    {
      bool r = streq (*args, str);
      args += r;
      أرجع r;
    }
}

/* Return true if there no more tokens.  */

ساكن bool
nomoreargs (فراغ)
{
  أرجع !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
ساكن فراغ
require_more_args (فراغ)
{
  𐡄𐡍 (nomoreargs ())
    error (EXPR_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#ifdef EVAL_TRACE
/* Print evaluation trace and args remaining.  */

ساكن فراغ
trace (حرف ثابت *fxn)
{
  printf ("%s:", fxn);
  لكل (حرف **a = args; *a; a++)
    printf (" %s", *a);
  putchar ('\n');
}
#endif

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

ساكن VALUE *
docolon (VALUE *sv, VALUE *pv)
{
  tostring (sv);
  tostring (pv);

  بنية re_registers re_regs;
  re_regs.num_regs = 0;
  re_regs.start = عدم;
  re_regs.end = عدم;

  بنية re_pattern_buffer re_buffer;
  حرف fastmap[UCHAR_MAX + 1];
  re_buffer.buffer = عدم;
  re_buffer.allocated = 0;
  re_buffer.fastmap = fastmap;
  re_buffer.translate = عدم;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_EMPTY_RANGES);
  حرف ثابت *errmsg = re_compile_pattern (pv->u.s, strlen (pv->u.s),
                                           &re_buffer);
  𐡄𐡍 (errmsg)
    error (EXPR_INVALID, 0, "%s", (errmsg));
  re_buffer.newline_anchor = 0;

  VALUE *v;
  regoff_t matchlen = re_match (&re_buffer, sv->u.s, strlen (sv->u.s), 0,
                                &re_regs);
  𐡄𐡍 (0 <= matchlen)
    {
      /* Were \(...\) used? */
      𐡄𐡍 (re_buffer.re_nsub > 0)
        {
          𐡄𐡍 (re_regs.end[1] < 0)
            v = str_value ("");
          وإلا
            {
              sv->u.s[re_regs.end[1]] = '\0';
              v = str_value (sv->u.s + re_regs.start[1]);
            }
        }
      وإلا
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_value (i);
        }
    }
  وإلا 𐡄𐡍 (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      𐡄𐡍 (re_buffer.re_nsub > 0)
        v = str_value ("");
      وإلا
        v = int_value (0);
    }
  وإلا
    error (EXPR_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  𐡄𐡍 (0 < re_regs.num_regs)
    {
      حرر (re_regs.start);
      حرر (re_regs.end);
    }
  re_buffer.fastmap = عدم;
  regfree (&re_buffer);
  أرجع v;
}

/* Handle bare operands and ( expr ) syntax.  */

ساكن VALUE *
eval7 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval7");
#endif
  require_more_args ();

  𐡄𐡍 (nextarg ("("))
    {
      VALUE *v = eval (evaluate);
      𐡄𐡍 (nomoreargs ())
        error (EXPR_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      𐡄𐡍 (!nextarg (")"))
        error (EXPR_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      أرجع v;
    }

  𐡄𐡍 (nextarg (")"))
    error (EXPR_INVALID, 0, _("syntax error: unexpected ')'"));

  أرجع str_value (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

ساكن VALUE *
eval6 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval6");
#endif
  𐡄𐡍 (nextarg ("+"))
    {
      require_more_args ();
      أرجع str_value (*args++);
    }
  وإلا 𐡄𐡍 (nextarg ("length"))
    {
      VALUE *r = eval6 (evaluate);
      tostring (r);
      VALUE *v = int_value (mbslen (r->u.s));
      freev (r);
      أرجع v;
    }
  وإلا 𐡄𐡍 (nextarg ("match"))
    {
      VALUE *l = eval6 (evaluate);
      VALUE *r = eval6 (evaluate);
      VALUE *v;
      𐡄𐡍 (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      وإلا
        v = l;
      freev (r);
      أرجع v;
    }
  وإلا 𐡄𐡍 (nextarg ("index"))
    {
      VALUE *l = eval6 (evaluate);
      VALUE *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      VALUE *v = int_value (pos);
      freev (l);
      freev (r);
      أرجع v;
    }
  وإلا 𐡄𐡍 (nextarg ("substr"))
    {
      VALUE *l = eval6 (evaluate);
      VALUE *i1 = eval6 (evaluate);
      VALUE *i2 = eval6 (evaluate);
      tostring (l);

      VALUE *v;
      𐡄𐡍 (!toarith (i1) || !toarith (i2))
        v = str_value ("");
      وإلا
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          حرف *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_value (s);
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

ساكن VALUE *
eval5 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval5");
#endif
  VALUE *l = eval6 (evaluate);
  طالما (true)
    {
      𐡄𐡍 (nextarg (":"))
        {
          VALUE *r = eval6 (evaluate);
          𐡄𐡍 (evaluate)
            {
              VALUE *v = docolon (l, r);
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

ساكن VALUE *
eval4 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval4");
#endif
  VALUE *l = eval5 (evaluate);
  طالما (true)
    {
      تعداد { multiply, divide, mod } fxn;

      𐡄𐡍 (nextarg ("*"))
        fxn = multiply;
      وإلا 𐡄𐡍 (nextarg ("/"))
        fxn = divide;
      وإلا 𐡄𐡍 (nextarg ("%"))
        fxn = mod;
      وإلا
        أرجع l;
      VALUE *r = eval5 (evaluate);
      𐡄𐡍 (evaluate)
        {
          𐡄𐡍 (!toarith (l) || !toarith (r))
            error (EXPR_INVALID, 0, _("non-integer argument"));
          𐡄𐡍 (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (EXPR_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

ساكن VALUE *
eval3 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval3");
#endif
  VALUE *l = eval4 (evaluate);
  طالما (true)
    {
      تعداد { plus, minus } fxn;

      𐡄𐡍 (nextarg ("+"))
        fxn = plus;
      وإلا 𐡄𐡍 (nextarg ("-"))
        fxn = minus;
      وإلا
        أرجع l;
      VALUE *r = eval4 (evaluate);
      𐡄𐡍 (evaluate)
        {
          𐡄𐡍 (!toarith (l) || !toarith (r))
            error (EXPR_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_add : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

ساكن VALUE *
eval2 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval2");
#endif
  VALUE *l = eval3 (evaluate);
  طالما (true)
    {
      تعداد
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      𐡄𐡍 (nextarg ("<"))
        fxn = less_than;
      وإلا 𐡄𐡍 (nextarg ("<="))
        fxn = less_equal;
      وإلا 𐡄𐡍 (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      وإلا 𐡄𐡍 (nextarg ("!="))
        fxn = not_equal;
      وإلا 𐡄𐡍 (nextarg (">="))
        fxn = greater_equal;
      وإلا 𐡄𐡍 (nextarg (">"))
        fxn = greater_than;
      وإلا
        أرجع l;
      VALUE *r = eval3 (evaluate);

      bool val = false;
      𐡄𐡍 (evaluate)
        {
          tostring (l);
          tostring (r);

          صحيح cmp;
          𐡄𐡍 (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          وإلا
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              𐡄𐡍 (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (EXPR_INVALID, 0,
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
      l = int_value (val);
    }
}

/* Handle &.  */

ساكن VALUE *
eval1 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval1");
#endif
  VALUE *l = eval2 (evaluate);
  طالما (true)
    {
      𐡄𐡍 (nextarg ("&"))
        {
          VALUE *r = eval2 (evaluate && !null (l));
          𐡄𐡍 (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_value (0);
            }
          وإلا
            freev (r);
        }
      وإلا
        أرجع l;
    }
}

/* Handle |.  */

ساكن VALUE *
eval (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval");
#endif
  VALUE *l = eval1 (evaluate);
  طالما (true)
    {
      𐡄𐡍 (nextarg ("|"))
        {
          VALUE *r = eval1 (evaluate && null (l));
          𐡄𐡍 (null (l))
            {
              freev (l);
              l = r;
              𐡄𐡍 (null (l))
                {
                  freev (l);
                  l = int_value (0);
                }
            }
          وإلا
            freev (r);
        }
      وإلا
        أرجع l;
    }
}
