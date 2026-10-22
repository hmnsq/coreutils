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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <gmp.h>
#अन्तर्भाव <regex.h>
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "long-options.h"
#अन्तर्भाव "mcel.h"
#अन्तर्भाव "strnumcmp.h"

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
गणना
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    व्यंजक_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    व्यंजक_FAILURE
  };

/* The kinds of value we can have.  */
गणना valtype
{
  integer,
  string
};
प्रकारनिर्देश गणना valtype TYPE;

/* A value is.... */
संरचना valinfo
{
  TYPE type;			/* Which kind. */
  सङ्घ
  {				/* The value itself. */
    mpz_t i;
    वर्ण *s;
  } u;
};
प्रकारनिर्देश संरचना valinfo मूल्य_2;

/* The arguments given to the program, minus the program name.  */
स्थिर वर्ण **args;

स्थिर मूल्य_2 *eval (bool);
स्थिर bool nomoreargs (शून्य);
स्थिर bool null (मूल्य_2 *v);
स्थिर शून्य printv (मूल्य_2 *v);


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
स्थिर size_t
mbs_logical_cspn (वर्ण ध्रुव *s, वर्ण ध्रुव *accept)
{
  यदि (accept[0] == '\0')
    प्रत्यागम 0;

  /* General case.  */
  यदि (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      प्रति (वर्ण ध्रुव *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          यदि (g.len == 1)
            {
              यदि (mbschr (accept, *p))
                प्रत्यागम idx;
            }
          अन्यथा
            प्रति (वर्ण ध्रुव *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                यदि (mcel_eq (g, h))
                  प्रत्यागम idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  अन्यथा
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      यदि (s[i])
        प्रत्यागम i + 1;
    }

  /* not found */
  प्रत्यागम 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
स्थिर वर्ण *
mbs_logical_substr (वर्ण ध्रुव *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? strlen (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  वर्ण ध्रुव *substart = s;
  idx_t sublen = 0;
  यदि (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  अन्यथा यदि (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  अन्यथा
    प्रति (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        यदि (pos <= idx)
          {
            यदि (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
      }

  प्रत्यागम ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
स्थिर size_t
mbs_offset_to_chars (वर्ण ध्रुव *s, size_t ofs)
{
  size_t c = 0;
  प्रति (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  प्रत्यागम c;
}



शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
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
Print the मूल्य of EXPRESSION to standard output.  A blank पंक्ति below\n\
separates increasing precedence groups.  EXPRESSION may be:\n\
\n\
  ARG1 | ARG2       ARG1 यदि it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 यदि neither argument is null or 0, otherwise 0\n\
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
  + TOKEN                    interpret TOKEN as a string, even यदि it is a\n\
                               keyword like 'match' or an operator like '/'\n\
\n\
  ( EXPRESSION )             मूल्य of EXPRESSION\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need to be escaped or quoted प्रति shells.\n\
Comparisons are arithmetic यदि both ARGs are numbers, अन्यथा lexicographical.\n\
Pattern matches प्रत्यागम the string matched between \\( and \\) or null; यदि\n\
\\( and \\) are not used, they प्रत्यागम the संख्या of characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit status is 0 यदि EXPRESSION is neither null nor 0, 1 यदि EXPRESSION is null\n\
or 0, 2 यदि EXPRESSION is syntactically invalid, and 3 यदि an error occurred.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (व्यंजक_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, PROGRAM_NAME, PACKAGE_NAME, VERSION,
                      usage, AUTHORS, (वर्ण ध्रुव *) रिक्त);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  यदि (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  यदि (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (व्यंजक_INVALID);
    }

  args = argv + 1;

  मूल्य_2 *v = eval (true);
  यदि (!nomoreargs ())
    error (व्यंजक_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

स्थिर मूल्य_2 *
int_मूल्य (unsigned long पूर्णाङ्क i)
{
  मूल्य_2 *v = xmalloc (परिमाण *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  प्रत्यागम v;
}

/* Return a VALUE for S.  */

स्थिर मूल्य_2 *
str_मूल्य (वर्ण ध्रुव *s)
{
  मूल्य_2 *v = xmalloc (परिमाण *v);
  v->type = string;
  v->u.s = xstrdup (s);
  प्रत्यागम v;
}

/* Free VALUE V, including structure components.  */

स्थिर शून्य
freev (मूल्य_2 *v)
{
  यदि (v->type == string)
    मुक्त (v->u.s);
  अन्यथा
    mpz_clear (v->u.i);
  मुक्त (v);
}

/* Print VALUE V.  */

स्थिर शून्य
printv (मूल्य_2 *v)
{
  विकल्प (v->type)
    {
    पक्ष integer:
      mpz_out_str (stdout, 10, v->u.i);
      putchar ('\n');
      भङ्ग;
    पक्ष string:
      puts (v->u.s);
      भङ्ग;
    स्वभाव:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_PURE
स्थिर bool
null (मूल्य_2 *v)
{
  विकल्प (v->type)
    {
    पक्ष integer:
      प्रत्यागम mpz_sgn (v->u.i) == 0;
    पक्ष string:
      {
        वर्ण ध्रुव *cp = v->u.s;
        यदि (*cp == '\0')
          प्रत्यागम true;

        cp += (*cp == '-');

        कुरु
          {
            यदि (*cp != '0')
              प्रत्यागम false;
          }
        यावत् (*++cp);

        प्रत्यागम true;
      }
    स्वभाव:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_PURE
स्थिर bool
looks_like_integer (वर्ण ध्रुव *cp)
{
  cp += (*cp == '-');

  कुरु
    यदि (! c_isdigit (*cp))
      प्रत्यागम false;
  यावत् (*++cp);

  प्रत्यागम true;
}

/* Coerce V to a string value (can't fail).  */

स्थिर शून्य
tostring (मूल्य_2 *v)
{
  विकल्प (v->type)
    {
    पक्ष integer:
      {
        वर्ण *s = mpz_get_str (रिक्त, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      भङ्ग;
    पक्ष string:
      भङ्ग;
    स्वभाव:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

स्थिर bool
toarith (मूल्य_2 *v)
{
  विकल्प (v->type)
    {
    पक्ष integer:
      प्रत्यागम true;
    पक्ष string:
      {
        वर्ण *s = v->u.s;

        यदि (! looks_like_integer (s))
          प्रत्यागम false;
        यदि (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (व्यंजक_FAILURE, ERANGE, "%s", (s));
        मुक्त (s);
        v->type = integer;
        प्रत्यागम true;
      }
    स्वभाव:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
स्थिर size_t
getsize (mpz_t i)
{
  यदि (mpz_sgn (i) < 0)
    प्रत्यागम SIZE_MAX;
  यदि (mpz_fits_ulong_p (i))
    {
      unsigned long पूर्णाङ्क ul = mpz_get_ui (i);
      यदि (ul < SIZE_MAX)
        प्रत्यागम ul;
    }
  प्रत्यागम SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

स्थिर bool
nextarg (वर्ण ध्रुव *str)
{
  यदि (*args == रिक्त)
    प्रत्यागम false;
  अन्यथा
    {
      bool r = streq (*args, str);
      args += r;
      प्रत्यागम r;
    }
}

/* Return true if there no more tokens.  */

स्थिर bool
nomoreargs (शून्य)
{
  प्रत्यागम !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
स्थिर शून्य
require_more_args (शून्य)
{
  यदि (nomoreargs ())
    error (व्यंजक_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#ifdef EVAL_TRACE
/* Print evaluation trace and args remaining.  */

स्थिर शून्य
trace (वर्ण ध्रुव *fxn)
{
  printf ("%s:", fxn);
  प्रति (वर्ण **a = args; *a; a++)
    printf (" %s", *a);
  putchar ('\n');
}
#endif

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

स्थिर मूल्य_2 *
docolon (मूल्य_2 *sv, मूल्य_2 *pv)
{
  tostring (sv);
  tostring (pv);

  संरचना re_registers re_regs;
  re_regs.अंक_regs = 0;
  re_regs.start = रिक्त;
  re_regs.end = रिक्त;

  संरचना re_pattern_buffer re_buffer;
  वर्ण fastmap[UCHAR_MAX + 1];
  re_buffer.buffer = रिक्त;
  re_buffer.allocated = 0;
  re_buffer.fastmap = fastmap;
  re_buffer.translate = रिक्त;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_EMPTY_RANGES);
  वर्ण ध्रुव *errmsg = re_compile_pattern (pv->u.s, strlen (pv->u.s),
                                           &re_buffer);
  यदि (errmsg)
    error (व्यंजक_INVALID, 0, "%s", (errmsg));
  re_buffer.newline_anchor = 0;

  मूल्य_2 *v;
  regoff_t matchlen = re_match (&re_buffer, sv->u.s, strlen (sv->u.s), 0,
                                &re_regs);
  यदि (0 <= matchlen)
    {
      /* Were \(...\) used? */
      यदि (re_buffer.re_nsub > 0)
        {
          यदि (re_regs.end[1] < 0)
            v = str_मूल्य ("");
          अन्यथा
            {
              sv->u.s[re_regs.end[1]] = '\0';
              v = str_मूल्य (sv->u.s + re_regs.start[1]);
            }
        }
      अन्यथा
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_मूल्य (i);
        }
    }
  अन्यथा यदि (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      यदि (re_buffer.re_nsub > 0)
        v = str_मूल्य ("");
      अन्यथा
        v = int_मूल्य (0);
    }
  अन्यथा
    error (व्यंजक_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  यदि (0 < re_regs.अंक_regs)
    {
      मुक्त (re_regs.start);
      मुक्त (re_regs.end);
    }
  re_buffer.fastmap = रिक्त;
  regfree (&re_buffer);
  प्रत्यागम v;
}

/* Handle bare operands and ( expr ) syntax.  */

स्थिर मूल्य_2 *
eval7 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval7");
#endif
  require_more_args ();

  यदि (nextarg ("("))
    {
      मूल्य_2 *v = eval (evaluate);
      यदि (nomoreargs ())
        error (व्यंजक_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      यदि (!nextarg (")"))
        error (व्यंजक_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      प्रत्यागम v;
    }

  यदि (nextarg (")"))
    error (व्यंजक_INVALID, 0, _("syntax error: unexpected ')'"));

  प्रत्यागम str_मूल्य (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

स्थिर मूल्य_2 *
eval6 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval6");
#endif
  यदि (nextarg ("+"))
    {
      require_more_args ();
      प्रत्यागम str_मूल्य (*args++);
    }
  अन्यथा यदि (nextarg ("length"))
    {
      मूल्य_2 *r = eval6 (evaluate);
      tostring (r);
      मूल्य_2 *v = int_मूल्य (mbslen (r->u.s));
      freev (r);
      प्रत्यागम v;
    }
  अन्यथा यदि (nextarg ("match"))
    {
      मूल्य_2 *l = eval6 (evaluate);
      मूल्य_2 *r = eval6 (evaluate);
      मूल्य_2 *v;
      यदि (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      अन्यथा
        v = l;
      freev (r);
      प्रत्यागम v;
    }
  अन्यथा यदि (nextarg ("index"))
    {
      मूल्य_2 *l = eval6 (evaluate);
      मूल्य_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      मूल्य_2 *v = int_मूल्य (pos);
      freev (l);
      freev (r);
      प्रत्यागम v;
    }
  अन्यथा यदि (nextarg ("substr"))
    {
      मूल्य_2 *l = eval6 (evaluate);
      मूल्य_2 *i1 = eval6 (evaluate);
      मूल्य_2 *i2 = eval6 (evaluate);
      tostring (l);

      मूल्य_2 *v;
      यदि (!toarith (i1) || !toarith (i2))
        v = str_मूल्य ("");
      अन्यथा
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          वर्ण *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_मूल्य (s);
          मुक्त (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      प्रत्यागम v;
    }
  अन्यथा
    प्रत्यागम eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

स्थिर मूल्य_2 *
eval5 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval5");
#endif
  मूल्य_2 *l = eval6 (evaluate);
  यावत् (true)
    {
      यदि (nextarg (":"))
        {
          मूल्य_2 *r = eval6 (evaluate);
          यदि (evaluate)
            {
              मूल्य_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      अन्यथा
        प्रत्यागम l;
    }
}

/* Handle *, /, % operators.  */

स्थिर मूल्य_2 *
eval4 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval4");
#endif
  मूल्य_2 *l = eval5 (evaluate);
  यावत् (true)
    {
      गणना { multiply, divide, mod } fxn;

      यदि (nextarg ("*"))
        fxn = multiply;
      अन्यथा यदि (nextarg ("/"))
        fxn = divide;
      अन्यथा यदि (nextarg ("%"))
        fxn = mod;
      अन्यथा
        प्रत्यागम l;
      मूल्य_2 *r = eval5 (evaluate);
      यदि (evaluate)
        {
          यदि (!toarith (l) || !toarith (r))
            error (व्यंजक_INVALID, 0, _("non-integer argument"));
          यदि (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (व्यंजक_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

स्थिर मूल्य_2 *
eval3 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval3");
#endif
  मूल्य_2 *l = eval4 (evaluate);
  यावत् (true)
    {
      गणना { plus, minus } fxn;

      यदि (nextarg ("+"))
        fxn = plus;
      अन्यथा यदि (nextarg ("-"))
        fxn = minus;
      अन्यथा
        प्रत्यागम l;
      मूल्य_2 *r = eval4 (evaluate);
      यदि (evaluate)
        {
          यदि (!toarith (l) || !toarith (r))
            error (व्यंजक_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_add : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

स्थिर मूल्य_2 *
eval2 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval2");
#endif
  मूल्य_2 *l = eval3 (evaluate);
  यावत् (true)
    {
      गणना
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      यदि (nextarg ("<"))
        fxn = less_than;
      अन्यथा यदि (nextarg ("<="))
        fxn = less_equal;
      अन्यथा यदि (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      अन्यथा यदि (nextarg ("!="))
        fxn = not_equal;
      अन्यथा यदि (nextarg (">="))
        fxn = greater_equal;
      अन्यथा यदि (nextarg (">"))
        fxn = greater_than;
      अन्यथा
        प्रत्यागम l;
      मूल्य_2 *r = eval3 (evaluate);

      bool val = false;
      यदि (evaluate)
        {
          tostring (l);
          tostring (r);

          पूर्णाङ्क cmp;
          यदि (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          अन्यथा
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              यदि (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (व्यंजक_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          विकल्प (fxn)
            {
            पक्ष less_than:     val = (cmp <  0); भङ्ग;
            पक्ष less_equal:    val = (cmp <= 0); भङ्ग;
            पक्ष equal:         val = (cmp == 0); भङ्ग;
            पक्ष not_equal:     val = (cmp != 0); भङ्ग;
            पक्ष greater_equal: val = (cmp >= 0); भङ्ग;
            पक्ष greater_than:  val = (cmp >  0); भङ्ग;
            स्वभाव: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_मूल्य (val);
    }
}

/* Handle &.  */

स्थिर मूल्य_2 *
eval1 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval1");
#endif
  मूल्य_2 *l = eval2 (evaluate);
  यावत् (true)
    {
      यदि (nextarg ("&"))
        {
          मूल्य_2 *r = eval2 (evaluate && !null (l));
          यदि (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_मूल्य (0);
            }
          अन्यथा
            freev (r);
        }
      अन्यथा
        प्रत्यागम l;
    }
}

/* Handle |.  */

स्थिर मूल्य_2 *
eval (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval");
#endif
  मूल्य_2 *l = eval1 (evaluate);
  यावत् (true)
    {
      यदि (nextarg ("|"))
        {
          मूल्य_2 *r = eval1 (evaluate && null (l));
          यदि (null (l))
            {
              freev (l);
              l = r;
              यदि (null (l))
                {
                  freev (l);
                  l = int_मूल्य (0);
                }
            }
          अन्यथा
            freev (r);
        }
      अन्यथा
        प्रत्यागम l;
    }
}
