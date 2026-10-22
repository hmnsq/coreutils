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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include "system.h"

#include <gmp.h>
#include <regex.h>
#include "c-ctype.h"
#include "long-options.h"
#include "mcel.h"
#include "strnumcmp.h"

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
ga_nanaa
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
ga_nanaa valtype
{
  integer,
  string
};
typedef ga_nanaa valtype TYPE;

/* A value is.... */
sa_nracanaa valinfo
{
  TYPE type;			/* Which kind. */
  sa_ngha
  {				/* The value itself. */
    mpz_t i;
    var_na *s;
  } u;
};
typedef sa_nracanaa valinfo VALUE;

/* The arguments given to the program, minus the program name.  */
static var_na **args;

static VALUE *eval (bool);
static bool nomoreargs (void);
static bool null (VALUE *v);
static void printv (VALUE *v);


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
static size_t
mbs_logical_cspn (var_na const *s, var_na const *accept)
{
  ប្រសិនបើ (accept[0] == '\0')
    ត្រឡប់ 0;

  /* General case.  */
  ប្រសិនបើ (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      សម្រាប់ (var_na const *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          ប្រសិនបើ (g.len == 1)
            {
              ប្រសិនបើ (mbschr (accept, *p))
                ត្រឡប់ idx;
            }
          បើមិនដូច្នេះ
            សម្រាប់ (var_na const *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                ប្រសិនបើ (mcel_eq (g, h))
                  ត្រឡប់ idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  បើមិនដូច្នេះ
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      ប្រសិនបើ (s[i])
        ត្រឡប់ i + 1;
    }

  /* not found */
  ត្រឡប់ 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
static var_na *
mbs_logical_substr (var_na const *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? strlen (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  var_na const *substart = s;
  idx_t sublen = 0;
  ប្រសិនបើ (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  បើមិនដូច្នេះ ប្រសិនបើ (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  បើមិនដូច្នេះ
    សម្រាប់ (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        ប្រសិនបើ (pos <= idx)
          {
            ប្រសិនបើ (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
      }

  ត្រឡប់ ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
static size_t
mbs_offset_to_chars (var_na const *s, size_t ofs)
{
  size_t c = 0;
  សម្រាប់ (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  ត្រឡប់ c;
}



void
usage (ចំនួនគត់ status)
{
  ប្រសិនបើ (status != EXIT_SUCCESS)
    emit_try_help ();
  បើមិនដូច្នេះ
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
  ARG1 | ARG2       ARG1 ប្រសិនបើ it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 ប្រសិនបើ neither argument is null or 0, otherwise 0\n\
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
  + TOKEN                    interpret TOKEN as a string, even ប្រសិនបើ it is a\n\
                               keyword like 'match' or an operator like '/'\n\
\n\
  ( EXPRESSION )             value of EXPRESSION\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need to be escaped or quoted សម្រាប់ shells.\n\
Comparisons are arithmetic ប្រសិនបើ both ARGs are numbers, បើមិនដូច្នេះ lexicographical.\n\
Pattern matches ត្រឡប់ the string matched between \\( and \\) or null; ប្រសិនបើ\n\
\\( and \\) are not used, they ត្រឡប់ the number of characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit status is 0 ប្រសិនបើ EXPRESSION is neither null nor 0, 1 ប្រសិនបើ EXPRESSION is null\n\
or 0, 2 ប្រសិនបើ EXPRESSION is syntactically invalid, and 3 ប្រសិនបើ an error occurred.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


ចំនួនគត់
main (ចំនួនគត់ argc, var_na **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXPR_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, PROGRAM_NAME, PACKAGE_NAME, VERSION,
                      usage, AUTHORS, (var_na const *) NULL);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  ប្រសិនបើ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ប្រសិនបើ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXPR_INVALID);
    }

  args = argv + 1;

  VALUE *v = eval (true);
  ប្រសិនបើ (!nomoreargs ())
    error (EXPR_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

static VALUE *
int_value (unsigned long ចំនួនគត់ i)
{
  VALUE *v = xmalloc (parimaa_na *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  ត្រឡប់ v;
}

/* Return a VALUE for S.  */

static VALUE *
str_value (var_na const *s)
{
  VALUE *v = xmalloc (parimaa_na *v);
  v->type = string;
  v->u.s = xstrdup (s);
  ត្រឡប់ v;
}

/* Free VALUE V, including structure components.  */

static void
freev (VALUE *v)
{
  ប្រសិនបើ (v->type == string)
    free (v->u.s);
  បើមិនដូច្នេះ
    mpz_clear (v->u.i);
  free (v);
}

/* Print VALUE V.  */

static void
printv (VALUE *v)
{
  switch (v->type)
    {
    pak_sa integer:
      mpz_out_str (stdout, 10, v->u.i);
      putchar ('\n');
      bha_nga;
    pak_sa string:
      puts (v->u.s);
      bha_nga;
    default:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_PURE
static bool
null (VALUE *v)
{
  switch (v->type)
    {
    pak_sa integer:
      ត្រឡប់ mpz_sgn (v->u.i) == 0;
    pak_sa string:
      {
        var_na const *cp = v->u.s;
        ប្រសិនបើ (*cp == '\0')
          ត្រឡប់ true;

        cp += (*cp == '-');

        do
          {
            ប្រសិនបើ (*cp != '0')
              ត្រឡប់ false;
          }
        ខណៈពេល (*++cp);

        ត្រឡប់ true;
      }
    default:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_PURE
static bool
looks_like_integer (var_na const *cp)
{
  cp += (*cp == '-');

  do
    ប្រសិនបើ (! c_isdigit (*cp))
      ត្រឡប់ false;
  ខណៈពេល (*++cp);

  ត្រឡប់ true;
}

/* Coerce V to a string value (can't fail).  */

static void
tostring (VALUE *v)
{
  switch (v->type)
    {
    pak_sa integer:
      {
        var_na *s = mpz_get_str (NULL, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      bha_nga;
    pak_sa string:
      bha_nga;
    default:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

static bool
toarith (VALUE *v)
{
  switch (v->type)
    {
    pak_sa integer:
      ត្រឡប់ true;
    pak_sa string:
      {
        var_na *s = v->u.s;

        ប្រសិនបើ (! looks_like_integer (s))
          ត្រឡប់ false;
        ប្រសិនបើ (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (EXPR_FAILURE, ERANGE, "%s", (s));
        free (s);
        v->type = integer;
        ត្រឡប់ true;
      }
    default:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
static size_t
getsize (mpz_t i)
{
  ប្រសិនបើ (mpz_sgn (i) < 0)
    ត្រឡប់ SIZE_MAX;
  ប្រសិនបើ (mpz_fits_ulong_p (i))
    {
      unsigned long ចំនួនគត់ ul = mpz_get_ui (i);
      ប្រសិនបើ (ul < SIZE_MAX)
        ត្រឡប់ ul;
    }
  ត្រឡប់ SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

static bool
nextarg (var_na const *str)
{
  ប្រសិនបើ (*args == NULL)
    ត្រឡប់ false;
  បើមិនដូច្នេះ
    {
      bool r = streq (*args, str);
      args += r;
      ត្រឡប់ r;
    }
}

/* Return true if there no more tokens.  */

static bool
nomoreargs (void)
{
  ត្រឡប់ !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
static void
require_more_args (void)
{
  ប្រសិនបើ (nomoreargs ())
    error (EXPR_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#ifdef EVAL_TRACE
/* Print evaluation trace and args remaining.  */

static void
trace (var_na const *fxn)
{
  printf ("%s:", fxn);
  សម្រាប់ (var_na **a = args; *a; a++)
    printf (" %s", *a);
  putchar ('\n');
}
#endif

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

static VALUE *
docolon (VALUE *sv, VALUE *pv)
{
  tostring (sv);
  tostring (pv);

  sa_nracanaa re_registers re_regs;
  re_regs.num_regs = 0;
  re_regs.start = NULL;
  re_regs.end = NULL;

  sa_nracanaa re_pattern_buffer re_buffer;
  var_na fastmap[UCHAR_MAX + 1];
  re_buffer.buffer = NULL;
  re_buffer.allocated = 0;
  re_buffer.fastmap = fastmap;
  re_buffer.translate = NULL;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_EMPTY_RANGES);
  var_na const *errmsg = re_compile_pattern (pv->u.s, strlen (pv->u.s),
                                           &re_buffer);
  ប្រសិនបើ (errmsg)
    error (EXPR_INVALID, 0, "%s", (errmsg));
  re_buffer.newline_anchor = 0;

  VALUE *v;
  regoff_t matchlen = re_match (&re_buffer, sv->u.s, strlen (sv->u.s), 0,
                                &re_regs);
  ប្រសិនបើ (0 <= matchlen)
    {
      /* Were \(...\) used? */
      ប្រសិនបើ (re_buffer.re_nsub > 0)
        {
          ប្រសិនបើ (re_regs.end[1] < 0)
            v = str_value ("");
          បើមិនដូច្នេះ
            {
              sv->u.s[re_regs.end[1]] = '\0';
              v = str_value (sv->u.s + re_regs.start[1]);
            }
        }
      បើមិនដូច្នេះ
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_value (i);
        }
    }
  បើមិនដូច្នេះ ប្រសិនបើ (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      ប្រសិនបើ (re_buffer.re_nsub > 0)
        v = str_value ("");
      បើមិនដូច្នេះ
        v = int_value (0);
    }
  បើមិនដូច្នេះ
    error (EXPR_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  ប្រសិនបើ (0 < re_regs.num_regs)
    {
      free (re_regs.start);
      free (re_regs.end);
    }
  re_buffer.fastmap = NULL;
  regfree (&re_buffer);
  ត្រឡប់ v;
}

/* Handle bare operands and ( expr ) syntax.  */

static VALUE *
eval7 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval7");
#endif
  require_more_args ();

  ប្រសិនបើ (nextarg ("("))
    {
      VALUE *v = eval (evaluate);
      ប្រសិនបើ (nomoreargs ())
        error (EXPR_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      ប្រសិនបើ (!nextarg (")"))
        error (EXPR_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      ត្រឡប់ v;
    }

  ប្រសិនបើ (nextarg (")"))
    error (EXPR_INVALID, 0, _("syntax error: unexpected ')'"));

  ត្រឡប់ str_value (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

static VALUE *
eval6 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval6");
#endif
  ប្រសិនបើ (nextarg ("+"))
    {
      require_more_args ();
      ត្រឡប់ str_value (*args++);
    }
  បើមិនដូច្នេះ ប្រសិនបើ (nextarg ("length"))
    {
      VALUE *r = eval6 (evaluate);
      tostring (r);
      VALUE *v = int_value (mbslen (r->u.s));
      freev (r);
      ត្រឡប់ v;
    }
  បើមិនដូច្នេះ ប្រសិនបើ (nextarg ("match"))
    {
      VALUE *l = eval6 (evaluate);
      VALUE *r = eval6 (evaluate);
      VALUE *v;
      ប្រសិនបើ (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      បើមិនដូច្នេះ
        v = l;
      freev (r);
      ត្រឡប់ v;
    }
  បើមិនដូច្នេះ ប្រសិនបើ (nextarg ("index"))
    {
      VALUE *l = eval6 (evaluate);
      VALUE *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      VALUE *v = int_value (pos);
      freev (l);
      freev (r);
      ត្រឡប់ v;
    }
  បើមិនដូច្នេះ ប្រសិនបើ (nextarg ("substr"))
    {
      VALUE *l = eval6 (evaluate);
      VALUE *i1 = eval6 (evaluate);
      VALUE *i2 = eval6 (evaluate);
      tostring (l);

      VALUE *v;
      ប្រសិនបើ (!toarith (i1) || !toarith (i2))
        v = str_value ("");
      បើមិនដូច្នេះ
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          var_na *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_value (s);
          free (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      ត្រឡប់ v;
    }
  បើមិនដូច្នេះ
    ត្រឡប់ eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

static VALUE *
eval5 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval5");
#endif
  VALUE *l = eval6 (evaluate);
  ខណៈពេល (true)
    {
      ប្រសិនបើ (nextarg (":"))
        {
          VALUE *r = eval6 (evaluate);
          ប្រសិនបើ (evaluate)
            {
              VALUE *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      បើមិនដូច្នេះ
        ត្រឡប់ l;
    }
}

/* Handle *, /, % operators.  */

static VALUE *
eval4 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval4");
#endif
  VALUE *l = eval5 (evaluate);
  ខណៈពេល (true)
    {
      ga_nanaa { multiply, divide, mod } fxn;

      ប្រសិនបើ (nextarg ("*"))
        fxn = multiply;
      បើមិនដូច្នេះ ប្រសិនបើ (nextarg ("/"))
        fxn = divide;
      បើមិនដូច្នេះ ប្រសិនបើ (nextarg ("%"))
        fxn = mod;
      បើមិនដូច្នេះ
        ត្រឡប់ l;
      VALUE *r = eval5 (evaluate);
      ប្រសិនបើ (evaluate)
        {
          ប្រសិនបើ (!toarith (l) || !toarith (r))
            error (EXPR_INVALID, 0, _("non-integer argument"));
          ប្រសិនបើ (fxn != multiply && mpz_sgn (r->u.i) == 0)
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

static VALUE *
eval3 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval3");
#endif
  VALUE *l = eval4 (evaluate);
  ខណៈពេល (true)
    {
      ga_nanaa { plus, minus } fxn;

      ប្រសិនបើ (nextarg ("+"))
        fxn = plus;
      បើមិនដូច្នេះ ប្រសិនបើ (nextarg ("-"))
        fxn = minus;
      បើមិនដូច្នេះ
        ត្រឡប់ l;
      VALUE *r = eval4 (evaluate);
      ប្រសិនបើ (evaluate)
        {
          ប្រសិនបើ (!toarith (l) || !toarith (r))
            error (EXPR_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_add : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

static VALUE *
eval2 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval2");
#endif
  VALUE *l = eval3 (evaluate);
  ខណៈពេល (true)
    {
      ga_nanaa
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      ប្រសិនបើ (nextarg ("<"))
        fxn = less_than;
      បើមិនដូច្នេះ ប្រសិនបើ (nextarg ("<="))
        fxn = less_equal;
      បើមិនដូច្នេះ ប្រសិនបើ (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      បើមិនដូច្នេះ ប្រសិនបើ (nextarg ("!="))
        fxn = not_equal;
      បើមិនដូច្នេះ ប្រសិនបើ (nextarg (">="))
        fxn = greater_equal;
      បើមិនដូច្នេះ ប្រសិនបើ (nextarg (">"))
        fxn = greater_than;
      បើមិនដូច្នេះ
        ត្រឡប់ l;
      VALUE *r = eval3 (evaluate);

      bool val = false;
      ប្រសិនបើ (evaluate)
        {
          tostring (l);
          tostring (r);

          ចំនួនគត់ cmp;
          ប្រសិនបើ (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          បើមិនដូច្នេះ
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              ប្រសិនបើ (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (EXPR_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          switch (fxn)
            {
            pak_sa less_than:     val = (cmp <  0); bha_nga;
            pak_sa less_equal:    val = (cmp <= 0); bha_nga;
            pak_sa equal:         val = (cmp == 0); bha_nga;
            pak_sa not_equal:     val = (cmp != 0); bha_nga;
            pak_sa greater_equal: val = (cmp >= 0); bha_nga;
            pak_sa greater_than:  val = (cmp >  0); bha_nga;
            default: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_value (val);
    }
}

/* Handle &.  */

static VALUE *
eval1 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval1");
#endif
  VALUE *l = eval2 (evaluate);
  ខណៈពេល (true)
    {
      ប្រសិនបើ (nextarg ("&"))
        {
          VALUE *r = eval2 (evaluate && !null (l));
          ប្រសិនបើ (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_value (0);
            }
          បើមិនដូច្នេះ
            freev (r);
        }
      បើមិនដូច្នេះ
        ត្រឡប់ l;
    }
}

/* Handle |.  */

static VALUE *
eval (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval");
#endif
  VALUE *l = eval1 (evaluate);
  ខណៈពេល (true)
    {
      ប្រសិនបើ (nextarg ("|"))
        {
          VALUE *r = eval1 (evaluate && null (l));
          ប្រសិនបើ (null (l))
            {
              freev (l);
              l = r;
              ប្រសិនបើ (null (l))
                {
                  freev (l);
                  l = int_value (0);
                }
            }
          បើមិនដូច្នេះ
            freev (r);
        }
      បើមិនដូច្នេះ
        ត្រឡប់ l;
    }
}
