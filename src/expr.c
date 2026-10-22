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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <gmp.h>
#অন্তর্ভাব <regex.h>
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "long-options.h"
#অন্তর্ভাব "mcel.h"
#অন্তর্ভাব "strnumcmp.h"

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
গণনা
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    রাশিমালা_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    রাশিমালা_FAILURE
  };

/* The kinds of value we can have.  */
গণনা valtype
{
  integer,
  string
};
প্রকারলেখ গণনা valtype TYPE;

/* A value is.... */
কাঠামো valinfo
{
  TYPE type;			/* Which kind. */
  সংঘ
  {				/* The value itself. */
    mpz_t i;
    অক্ষর *s;
  } u;
};
প্রকারলেখ কাঠামো valinfo মান_2;

/* The arguments given to the program, minus the program name.  */
স্থির অক্ষর **args;

স্থির মান_2 *eval (bool);
স্থির bool nomoreargs (শূন্য);
স্থির bool null (মান_2 *v);
স্থির শূন্য printv (মান_2 *v);


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
স্থির size_t
mbs_logical_cspn (অক্ষর ধ্রুবক *s, অক্ষর ধ্রুবক *accept)
{
  যদি (accept[0] == '\0')
    ফেরত 0;

  /* General case.  */
  যদি (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      জন্য (অক্ষর ধ্রুবক *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          যদি (g.len == 1)
            {
              যদি (mbschr (accept, *p))
                ফেরত idx;
            }
          অন্যথা
            জন্য (অক্ষর ধ্রুবক *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                যদি (mcel_eq (g, h))
                  ফেরত idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  অন্যথা
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      যদি (s[i])
        ফেরত i + 1;
    }

  /* not found */
  ফেরত 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
স্থির অক্ষর *
mbs_logical_substr (অক্ষর ধ্রুবক *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? strlen (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  অক্ষর ধ্রুবক *substart = s;
  idx_t sublen = 0;
  যদি (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  অন্যথা যদি (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  অন্যথা
    জন্য (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        যদি (pos <= idx)
          {
            যদি (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
      }

  ফেরত ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
স্থির size_t
mbs_offset_to_chars (অক্ষর ধ্রুবক *s, size_t ofs)
{
  size_t c = 0;
  জন্য (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  ফেরত c;
}



শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
Print the মান of EXPRESSION to standard output.  A blank সারি below\n\
separates increasing precedence groups.  EXPRESSION may be:\n\
\n\
  ARG1 | ARG2       ARG1 যদি it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 যদি neither argument is null or 0, otherwise 0\n\
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
  ARG1 + ARG2       arithmetic যোগ of ARG1 and ARG2\n\
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
  substr STRING POS দৈর্ঘ্য_2   substring of STRING, POS counted from 1\n\
  index STRING CHARS         index in STRING where any CHARS is found, or 0\n\
  দৈর্ঘ্য STRING              দৈর্ঘ্য of STRING\n\
"), stdout);
      fputs (_("\
  + TOKEN                    interpret TOKEN as a string, even যদি it is a\n\
                               keyword like 'match' or an operator like '/'\n\
\n\
  ( EXPRESSION )             মান of EXPRESSION\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need to be escaped or quoted জন্য shells.\n\
Comparisons are arithmetic যদি both ARGs are numbers, অন্যথা lexicographical.\n\
Pattern matches ফেরত the string matched between \\( and \\) or null; যদি\n\
\\( and \\) are not used, they ফেরত the সংখ্যা of characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit status is 0 যদি EXPRESSION is neither null nor 0, 1 যদি EXPRESSION is null\n\
or 0, 2 যদি EXPRESSION is syntactically invalid, and 3 যদি an error occurred.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (রাশিমালা_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, PROGRAM_NAME, PACKAGE_NAME, VERSION,
                      usage, AUTHORS, (অক্ষর ধ্রুবক *) ফাঁকা);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  যদি (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  যদি (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (রাশিমালা_INVALID);
    }

  args = argv + 1;

  মান_2 *v = eval (true);
  যদি (!nomoreargs ())
    error (রাশিমালা_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

স্থির মান_2 *
int_মান (unsigned long পূর্ণসংখ্যা i)
{
  মান_2 *v = xmalloc (আকার *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  ফেরত v;
}

/* Return a VALUE for S.  */

স্থির মান_2 *
str_মান (অক্ষর ধ্রুবক *s)
{
  মান_2 *v = xmalloc (আকার *v);
  v->type = string;
  v->u.s = xstrdup (s);
  ফেরত v;
}

/* Free VALUE V, including structure components.  */

স্থির শূন্য
freev (মান_2 *v)
{
  যদি (v->type == string)
    মুক্ত (v->u.s);
  অন্যথা
    mpz_clear (v->u.i);
  মুক্ত (v);
}

/* Print VALUE V.  */

স্থির শূন্য
printv (মান_2 *v)
{
  নির্বাচন (v->type)
    {
    ক্ষেত্র integer:
      mpz_out_str (stdout, 10, v->u.i);
      putchar ('\n');
      বিরতি;
    ক্ষেত্র string:
      puts (v->u.s);
      বিরতি;
    পূর্বনির্ধারিত:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_PURE
স্থির bool
null (মান_2 *v)
{
  নির্বাচন (v->type)
    {
    ক্ষেত্র integer:
      ফেরত mpz_sgn (v->u.i) == 0;
    ক্ষেত্র string:
      {
        অক্ষর ধ্রুবক *cp = v->u.s;
        যদি (*cp == '\0')
          ফেরত true;

        cp += (*cp == '-');

        করো
          {
            যদি (*cp != '0')
              ফেরত false;
          }
        যখন (*++cp);

        ফেরত true;
      }
    পূর্বনির্ধারিত:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_PURE
স্থির bool
looks_like_integer (অক্ষর ধ্রুবক *cp)
{
  cp += (*cp == '-');

  করো
    যদি (! c_isdigit (*cp))
      ফেরত false;
  যখন (*++cp);

  ফেরত true;
}

/* Coerce V to a string value (can't fail).  */

স্থির শূন্য
tostring (মান_2 *v)
{
  নির্বাচন (v->type)
    {
    ক্ষেত্র integer:
      {
        অক্ষর *s = mpz_get_str (ফাঁকা, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      বিরতি;
    ক্ষেত্র string:
      বিরতি;
    পূর্বনির্ধারিত:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

স্থির bool
toarith (মান_2 *v)
{
  নির্বাচন (v->type)
    {
    ক্ষেত্র integer:
      ফেরত true;
    ক্ষেত্র string:
      {
        অক্ষর *s = v->u.s;

        যদি (! looks_like_integer (s))
          ফেরত false;
        যদি (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (রাশিমালা_FAILURE, ERANGE, "%s", (s));
        মুক্ত (s);
        v->type = integer;
        ফেরত true;
      }
    পূর্বনির্ধারিত:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
স্থির size_t
getsize (mpz_t i)
{
  যদি (mpz_sgn (i) < 0)
    ফেরত SIZE_MAX;
  যদি (mpz_fits_ulong_p (i))
    {
      unsigned long পূর্ণসংখ্যা ul = mpz_get_ui (i);
      যদি (ul < SIZE_MAX)
        ফেরত ul;
    }
  ফেরত SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

স্থির bool
nextarg (অক্ষর ধ্রুবক *str)
{
  যদি (*args == ফাঁকা)
    ফেরত false;
  অন্যথা
    {
      bool r = streq (*args, str);
      args += r;
      ফেরত r;
    }
}

/* Return true if there no more tokens.  */

স্থির bool
nomoreargs (শূন্য)
{
  ফেরত !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
স্থির শূন্য
require_more_args (শূন্য)
{
  যদি (nomoreargs ())
    error (রাশিমালা_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#ifdef EVAL_TRACE
/* Print evaluation trace and args remaining.  */

স্থির শূন্য
trace (অক্ষর ধ্রুবক *fxn)
{
  printf ("%s:", fxn);
  জন্য (অক্ষর **a = args; *a; a++)
    printf (" %s", *a);
  putchar ('\n');
}
#endif

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

স্থির মান_2 *
docolon (মান_2 *sv, মান_2 *pv)
{
  tostring (sv);
  tostring (pv);

  কাঠামো re_registers re_regs;
  re_regs.অঙ্ক_regs = 0;
  re_regs.start = ফাঁকা;
  re_regs.end = ফাঁকা;

  কাঠামো re_pattern_বাফার re_বাফার;
  অক্ষর fastmap[UCHAR_MAX + 1];
  re_বাফার.বাফার = ফাঁকা;
  re_বাফার.allocated = 0;
  re_বাফার.fastmap = fastmap;
  re_বাফার.translate = ফাঁকা;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_EMPTY_RANGES);
  অক্ষর ধ্রুবক *errmsg = re_compile_pattern (pv->u.s, strlen (pv->u.s),
                                           &re_বাফার);
  যদি (errmsg)
    error (রাশিমালা_INVALID, 0, "%s", (errmsg));
  re_বাফার.newline_anchor = 0;

  মান_2 *v;
  regoff_t matchlen = re_match (&re_বাফার, sv->u.s, strlen (sv->u.s), 0,
                                &re_regs);
  যদি (0 <= matchlen)
    {
      /* Were \(...\) used? */
      যদি (re_বাফার.re_nsub > 0)
        {
          যদি (re_regs.end[1] < 0)
            v = str_মান ("");
          অন্যথা
            {
              sv->u.s[re_regs.end[1]] = '\0';
              v = str_মান (sv->u.s + re_regs.start[1]);
            }
        }
      অন্যথা
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_মান (i);
        }
    }
  অন্যথা যদি (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      যদি (re_বাফার.re_nsub > 0)
        v = str_মান ("");
      অন্যথা
        v = int_মান (0);
    }
  অন্যথা
    error (রাশিমালা_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  যদি (0 < re_regs.অঙ্ক_regs)
    {
      মুক্ত (re_regs.start);
      মুক্ত (re_regs.end);
    }
  re_বাফার.fastmap = ফাঁকা;
  regfree (&re_বাফার);
  ফেরত v;
}

/* Handle bare operands and ( expr ) syntax.  */

স্থির মান_2 *
eval7 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval7");
#endif
  require_more_args ();

  যদি (nextarg ("("))
    {
      মান_2 *v = eval (evaluate);
      যদি (nomoreargs ())
        error (রাশিমালা_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      যদি (!nextarg (")"))
        error (রাশিমালা_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      ফেরত v;
    }

  যদি (nextarg (")"))
    error (রাশিমালা_INVALID, 0, _("syntax error: unexpected ')'"));

  ফেরত str_মান (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

স্থির মান_2 *
eval6 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval6");
#endif
  যদি (nextarg ("+"))
    {
      require_more_args ();
      ফেরত str_মান (*args++);
    }
  অন্যথা যদি (nextarg ("length"))
    {
      মান_2 *r = eval6 (evaluate);
      tostring (r);
      মান_2 *v = int_মান (mbslen (r->u.s));
      freev (r);
      ফেরত v;
    }
  অন্যথা যদি (nextarg ("match"))
    {
      মান_2 *l = eval6 (evaluate);
      মান_2 *r = eval6 (evaluate);
      মান_2 *v;
      যদি (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      অন্যথা
        v = l;
      freev (r);
      ফেরত v;
    }
  অন্যথা যদি (nextarg ("index"))
    {
      মান_2 *l = eval6 (evaluate);
      মান_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      মান_2 *v = int_মান (pos);
      freev (l);
      freev (r);
      ফেরত v;
    }
  অন্যথা যদি (nextarg ("substr"))
    {
      মান_2 *l = eval6 (evaluate);
      মান_2 *i1 = eval6 (evaluate);
      মান_2 *i2 = eval6 (evaluate);
      tostring (l);

      মান_2 *v;
      যদি (!toarith (i1) || !toarith (i2))
        v = str_মান ("");
      অন্যথা
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          অক্ষর *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_মান (s);
          মুক্ত (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      ফেরত v;
    }
  অন্যথা
    ফেরত eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

স্থির মান_2 *
eval5 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval5");
#endif
  মান_2 *l = eval6 (evaluate);
  যখন (true)
    {
      যদি (nextarg (":"))
        {
          মান_2 *r = eval6 (evaluate);
          যদি (evaluate)
            {
              মান_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      অন্যথা
        ফেরত l;
    }
}

/* Handle *, /, % operators.  */

স্থির মান_2 *
eval4 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval4");
#endif
  মান_2 *l = eval5 (evaluate);
  যখন (true)
    {
      গণনা { multiply, divide, mod } fxn;

      যদি (nextarg ("*"))
        fxn = multiply;
      অন্যথা যদি (nextarg ("/"))
        fxn = divide;
      অন্যথা যদি (nextarg ("%"))
        fxn = mod;
      অন্যথা
        ফেরত l;
      মান_2 *r = eval5 (evaluate);
      যদি (evaluate)
        {
          যদি (!toarith (l) || !toarith (r))
            error (রাশিমালা_INVALID, 0, _("non-integer argument"));
          যদি (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (রাশিমালা_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

স্থির মান_2 *
eval3 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval3");
#endif
  মান_2 *l = eval4 (evaluate);
  যখন (true)
    {
      গণনা { plus, minus } fxn;

      যদি (nextarg ("+"))
        fxn = plus;
      অন্যথা যদি (nextarg ("-"))
        fxn = minus;
      অন্যথা
        ফেরত l;
      মান_2 *r = eval4 (evaluate);
      যদি (evaluate)
        {
          যদি (!toarith (l) || !toarith (r))
            error (রাশিমালা_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_add : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

স্থির মান_2 *
eval2 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval2");
#endif
  মান_2 *l = eval3 (evaluate);
  যখন (true)
    {
      গণনা
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      যদি (nextarg ("<"))
        fxn = less_than;
      অন্যথা যদি (nextarg ("<="))
        fxn = less_equal;
      অন্যথা যদি (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      অন্যথা যদি (nextarg ("!="))
        fxn = not_equal;
      অন্যথা যদি (nextarg (">="))
        fxn = greater_equal;
      অন্যথা যদি (nextarg (">"))
        fxn = greater_than;
      অন্যথা
        ফেরত l;
      মান_2 *r = eval3 (evaluate);

      bool val = false;
      যদি (evaluate)
        {
          tostring (l);
          tostring (r);

          পূর্ণসংখ্যা cmp;
          যদি (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          অন্যথা
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              যদি (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (রাশিমালা_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          নির্বাচন (fxn)
            {
            ক্ষেত্র less_than:     val = (cmp <  0); বিরতি;
            ক্ষেত্র less_equal:    val = (cmp <= 0); বিরতি;
            ক্ষেত্র equal:         val = (cmp == 0); বিরতি;
            ক্ষেত্র not_equal:     val = (cmp != 0); বিরতি;
            ক্ষেত্র greater_equal: val = (cmp >= 0); বিরতি;
            ক্ষেত্র greater_than:  val = (cmp >  0); বিরতি;
            পূর্বনির্ধারিত: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_মান (val);
    }
}

/* Handle &.  */

স্থির মান_2 *
eval1 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval1");
#endif
  মান_2 *l = eval2 (evaluate);
  যখন (true)
    {
      যদি (nextarg ("&"))
        {
          মান_2 *r = eval2 (evaluate && !null (l));
          যদি (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_মান (0);
            }
          অন্যথা
            freev (r);
        }
      অন্যথা
        ফেরত l;
    }
}

/* Handle |.  */

স্থির মান_2 *
eval (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval");
#endif
  মান_2 *l = eval1 (evaluate);
  যখন (true)
    {
      যদি (nextarg ("|"))
        {
          মান_2 *r = eval1 (evaluate && null (l));
          যদি (null (l))
            {
              freev (l);
              l = r;
              যদি (null (l))
                {
                  freev (l);
                  l = int_মান (0);
                }
            }
          অন্যথা
            freev (r);
        }
      অন্যথা
        ফেরত l;
    }
}
