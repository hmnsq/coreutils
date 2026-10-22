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
  ຖ້າ (accept[0] == '\0')
    ສົ່ງຄືນ 0;

  /* General case.  */
  ຖ້າ (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      ສຳລັບ (var_na const *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          ຖ້າ (g.len == 1)
            {
              ຖ້າ (mbschr (accept, *p))
                ສົ່ງຄືນ idx;
            }
          ບໍ່ດັ່ງນັ້ນ
            ສຳລັບ (var_na const *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                ຖ້າ (mcel_eq (g, h))
                  ສົ່ງຄືນ idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      ຖ້າ (s[i])
        ສົ່ງຄືນ i + 1;
    }

  /* not found */
  ສົ່ງຄືນ 0;
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
  ຖ້າ (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  ບໍ່ດັ່ງນັ້ນ
    ສຳລັບ (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        ຖ້າ (pos <= idx)
          {
            ຖ້າ (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
      }

  ສົ່ງຄືນ ximemdup0 (substart, sublen);
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
  ສຳລັບ (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  ສົ່ງຄືນ c;
}



void
usage (ຈຳນວນເຕັມ status)
{
  ຖ້າ (status != EXIT_SUCCESS)
    emit_try_help ();
  ບໍ່ດັ່ງນັ້ນ
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
  ARG1 | ARG2       ARG1 ຖ້າ it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 ຖ້າ neither argument is null or 0, otherwise 0\n\
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
  + TOKEN                    interpret TOKEN as a string, even ຖ້າ it is a\n\
                               keyword like 'match' or an operator like '/'\n\
\n\
  ( EXPRESSION )             value of EXPRESSION\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need to be escaped or quoted ສຳລັບ shells.\n\
Comparisons are arithmetic ຖ້າ both ARGs are numbers, ບໍ່ດັ່ງນັ້ນ lexicographical.\n\
Pattern matches ສົ່ງຄືນ the string matched between \\( and \\) or null; ຖ້າ\n\
\\( and \\) are not used, they ສົ່ງຄືນ the number of characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit status is 0 ຖ້າ EXPRESSION is neither null nor 0, 1 ຖ້າ EXPRESSION is null\n\
or 0, 2 ຖ້າ EXPRESSION is syntactically invalid, and 3 ຖ້າ an error occurred.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


ຈຳນວນເຕັມ
main (ຈຳນວນເຕັມ argc, var_na **argv)
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
  ຖ້າ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ຖ້າ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXPR_INVALID);
    }

  args = argv + 1;

  VALUE *v = eval (true);
  ຖ້າ (!nomoreargs ())
    error (EXPR_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

static VALUE *
int_value (unsigned long ຈຳນວນເຕັມ i)
{
  VALUE *v = xmalloc (parimaa_na *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  ສົ່ງຄືນ v;
}

/* Return a VALUE for S.  */

static VALUE *
str_value (var_na const *s)
{
  VALUE *v = xmalloc (parimaa_na *v);
  v->type = string;
  v->u.s = xstrdup (s);
  ສົ່ງຄືນ v;
}

/* Free VALUE V, including structure components.  */

static void
freev (VALUE *v)
{
  ຖ້າ (v->type == string)
    free (v->u.s);
  ບໍ່ດັ່ງນັ້ນ
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
      ສົ່ງຄືນ mpz_sgn (v->u.i) == 0;
    pak_sa string:
      {
        var_na const *cp = v->u.s;
        ຖ້າ (*cp == '\0')
          ສົ່ງຄືນ true;

        cp += (*cp == '-');

        do
          {
            ຖ້າ (*cp != '0')
              ສົ່ງຄືນ false;
          }
        ໃນຂະນະທີ່ (*++cp);

        ສົ່ງຄືນ true;
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
    ຖ້າ (! c_isdigit (*cp))
      ສົ່ງຄືນ false;
  ໃນຂະນະທີ່ (*++cp);

  ສົ່ງຄືນ true;
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
      ສົ່ງຄືນ true;
    pak_sa string:
      {
        var_na *s = v->u.s;

        ຖ້າ (! looks_like_integer (s))
          ສົ່ງຄືນ false;
        ຖ້າ (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (EXPR_FAILURE, ERANGE, "%s", (s));
        free (s);
        v->type = integer;
        ສົ່ງຄືນ true;
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
  ຖ້າ (mpz_sgn (i) < 0)
    ສົ່ງຄືນ SIZE_MAX;
  ຖ້າ (mpz_fits_ulong_p (i))
    {
      unsigned long ຈຳນວນເຕັມ ul = mpz_get_ui (i);
      ຖ້າ (ul < SIZE_MAX)
        ສົ່ງຄືນ ul;
    }
  ສົ່ງຄືນ SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

static bool
nextarg (var_na const *str)
{
  ຖ້າ (*args == NULL)
    ສົ່ງຄືນ false;
  ບໍ່ດັ່ງນັ້ນ
    {
      bool r = streq (*args, str);
      args += r;
      ສົ່ງຄືນ r;
    }
}

/* Return true if there no more tokens.  */

static bool
nomoreargs (void)
{
  ສົ່ງຄືນ !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
static void
require_more_args (void)
{
  ຖ້າ (nomoreargs ())
    error (EXPR_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#ifdef EVAL_TRACE
/* Print evaluation trace and args remaining.  */

static void
trace (var_na const *fxn)
{
  printf ("%s:", fxn);
  ສຳລັບ (var_na **a = args; *a; a++)
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
  ຖ້າ (errmsg)
    error (EXPR_INVALID, 0, "%s", (errmsg));
  re_buffer.newline_anchor = 0;

  VALUE *v;
  regoff_t matchlen = re_match (&re_buffer, sv->u.s, strlen (sv->u.s), 0,
                                &re_regs);
  ຖ້າ (0 <= matchlen)
    {
      /* Were \(...\) used? */
      ຖ້າ (re_buffer.re_nsub > 0)
        {
          ຖ້າ (re_regs.end[1] < 0)
            v = str_value ("");
          ບໍ່ດັ່ງນັ້ນ
            {
              sv->u.s[re_regs.end[1]] = '\0';
              v = str_value (sv->u.s + re_regs.start[1]);
            }
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_value (i);
        }
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      ຖ້າ (re_buffer.re_nsub > 0)
        v = str_value ("");
      ບໍ່ດັ່ງນັ້ນ
        v = int_value (0);
    }
  ບໍ່ດັ່ງນັ້ນ
    error (EXPR_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  ຖ້າ (0 < re_regs.num_regs)
    {
      free (re_regs.start);
      free (re_regs.end);
    }
  re_buffer.fastmap = NULL;
  regfree (&re_buffer);
  ສົ່ງຄືນ v;
}

/* Handle bare operands and ( expr ) syntax.  */

static VALUE *
eval7 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval7");
#endif
  require_more_args ();

  ຖ້າ (nextarg ("("))
    {
      VALUE *v = eval (evaluate);
      ຖ້າ (nomoreargs ())
        error (EXPR_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      ຖ້າ (!nextarg (")"))
        error (EXPR_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      ສົ່ງຄືນ v;
    }

  ຖ້າ (nextarg (")"))
    error (EXPR_INVALID, 0, _("syntax error: unexpected ')'"));

  ສົ່ງຄືນ str_value (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

static VALUE *
eval6 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval6");
#endif
  ຖ້າ (nextarg ("+"))
    {
      require_more_args ();
      ສົ່ງຄືນ str_value (*args++);
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg ("length"))
    {
      VALUE *r = eval6 (evaluate);
      tostring (r);
      VALUE *v = int_value (mbslen (r->u.s));
      freev (r);
      ສົ່ງຄືນ v;
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg ("match"))
    {
      VALUE *l = eval6 (evaluate);
      VALUE *r = eval6 (evaluate);
      VALUE *v;
      ຖ້າ (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      ບໍ່ດັ່ງນັ້ນ
        v = l;
      freev (r);
      ສົ່ງຄືນ v;
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg ("index"))
    {
      VALUE *l = eval6 (evaluate);
      VALUE *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      VALUE *v = int_value (pos);
      freev (l);
      freev (r);
      ສົ່ງຄືນ v;
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg ("substr"))
    {
      VALUE *l = eval6 (evaluate);
      VALUE *i1 = eval6 (evaluate);
      VALUE *i2 = eval6 (evaluate);
      tostring (l);

      VALUE *v;
      ຖ້າ (!toarith (i1) || !toarith (i2))
        v = str_value ("");
      ບໍ່ດັ່ງນັ້ນ
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
      ສົ່ງຄືນ v;
    }
  ບໍ່ດັ່ງນັ້ນ
    ສົ່ງຄືນ eval7 (evaluate);
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
  ໃນຂະນະທີ່ (true)
    {
      ຖ້າ (nextarg (":"))
        {
          VALUE *r = eval6 (evaluate);
          ຖ້າ (evaluate)
            {
              VALUE *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      ບໍ່ດັ່ງນັ້ນ
        ສົ່ງຄືນ l;
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
  ໃນຂະນະທີ່ (true)
    {
      ga_nanaa { multiply, divide, mod } fxn;

      ຖ້າ (nextarg ("*"))
        fxn = multiply;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg ("/"))
        fxn = divide;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg ("%"))
        fxn = mod;
      ບໍ່ດັ່ງນັ້ນ
        ສົ່ງຄືນ l;
      VALUE *r = eval5 (evaluate);
      ຖ້າ (evaluate)
        {
          ຖ້າ (!toarith (l) || !toarith (r))
            error (EXPR_INVALID, 0, _("non-integer argument"));
          ຖ້າ (fxn != multiply && mpz_sgn (r->u.i) == 0)
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
  ໃນຂະນະທີ່ (true)
    {
      ga_nanaa { plus, minus } fxn;

      ຖ້າ (nextarg ("+"))
        fxn = plus;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg ("-"))
        fxn = minus;
      ບໍ່ດັ່ງນັ້ນ
        ສົ່ງຄືນ l;
      VALUE *r = eval4 (evaluate);
      ຖ້າ (evaluate)
        {
          ຖ້າ (!toarith (l) || !toarith (r))
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
  ໃນຂະນະທີ່ (true)
    {
      ga_nanaa
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      ຖ້າ (nextarg ("<"))
        fxn = less_than;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg ("<="))
        fxn = less_equal;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg ("!="))
        fxn = not_equal;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg (">="))
        fxn = greater_equal;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (nextarg (">"))
        fxn = greater_than;
      ບໍ່ດັ່ງນັ້ນ
        ສົ່ງຄືນ l;
      VALUE *r = eval3 (evaluate);

      bool val = false;
      ຖ້າ (evaluate)
        {
          tostring (l);
          tostring (r);

          ຈຳນວນເຕັມ cmp;
          ຖ້າ (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          ບໍ່ດັ່ງນັ້ນ
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              ຖ້າ (errno)
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
  ໃນຂະນະທີ່ (true)
    {
      ຖ້າ (nextarg ("&"))
        {
          VALUE *r = eval2 (evaluate && !null (l));
          ຖ້າ (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_value (0);
            }
          ບໍ່ດັ່ງນັ້ນ
            freev (r);
        }
      ບໍ່ດັ່ງນັ້ນ
        ສົ່ງຄືນ l;
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
  ໃນຂະນະທີ່ (true)
    {
      ຖ້າ (nextarg ("|"))
        {
          VALUE *r = eval1 (evaluate && null (l));
          ຖ້າ (null (l))
            {
              freev (l);
              l = r;
              ຖ້າ (null (l))
                {
                  freev (l);
                  l = int_value (0);
                }
            }
          ບໍ່ດັ່ງນັ້ນ
            freev (r);
        }
      ບໍ່ດັ່ງນັ້ນ
        ສົ່ງຄືນ l;
    }
}
