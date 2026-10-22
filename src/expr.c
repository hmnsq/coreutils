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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <gmp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <regex.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "long-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mcel.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strnumcmp.h"

/* Various parts of this code assume size_t fits into unsigned long
   int, the widest unsigned type that GMP supports.  */
static_assert (SIZE_MAX <= ULONG_MAX);

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "expr"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("James Youngman"), \
  proper_name ("Paul Eggert")

/* Exit statuses.  */
ਗਿਣਤੀ
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    ਵਿਅੰਜਕ_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    ਵਿਅੰਜਕ_FAILURE
  };

/* The kinds of value we can have.  */
ਗਿਣਤੀ valtype
{
  integer,
  string
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ valtype TYPE;

/* A value is.... */
ਬਣਤਰ valinfo
{
  TYPE type;			/* Which kind. */
  ਸੰਘ
  {				/* The value itself. */
    mpz_t i;
    ਅੱਖਰ *s;
  } u;
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ valinfo ਮੁੱਲ_2;

/* The arguments given to the program, minus the program name.  */
ਸਥਿਰ ਅੱਖਰ **ਦਲੀਲਾਂ;

ਸਥਿਰ ਮੁੱਲ_2 *eval (bool);
ਸਥਿਰ bool nomoreargs (ਖਾਲੀ);
ਸਥਿਰ bool null (ਮੁੱਲ_2 *v);
ਸਥਿਰ ਖਾਲੀ printv (ਮੁੱਲ_2 *v);


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
ਸਥਿਰ size_t
mbs_logical_cspn (ਅੱਖਰ ਅਚਲ *s, ਅੱਖਰ ਅਚਲ *accept)
{
  ਜੇ (accept[0] == '\0')
    ਵਾਪਸ 0;

  /* General case.  */
  ਜੇ (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      ਲਈ (ਅੱਖਰ ਅਚਲ *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          ਜੇ (g.len == 1)
            {
              ਜੇ (mbschr (accept, *p))
                ਵਾਪਸ idx;
            }
          ਨਹੀਂਤਾਂ
            ਲਈ (ਅੱਖਰ ਅਚਲ *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                ਜੇ (mcel_eq (g, h))
                  ਵਾਪਸ idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      ਜੇ (s[i])
        ਵਾਪਸ i + 1;
    }

  /* not found */
  ਵਾਪਸ 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
ਸਥਿਰ ਅੱਖਰ *
mbs_logical_substr (ਅੱਖਰ ਅਚਲ *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? ਲੜੀ_ਲੰਬਾਈ (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  ਅੱਖਰ ਅਚਲ *substart = s;
  idx_t sublen = 0;
  ਜੇ (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  ਨਹੀਂਤਾਂ ਜੇ (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  ਨਹੀਂਤਾਂ
    ਲਈ (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_ਬਾਈਟਾਂ = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        ਜੇ (pos <= idx)
          {
            ਜੇ (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_ਬਾਈਟਾਂ;
          }

        s += char_ਬਾਈਟਾਂ;
      }

  ਵਾਪਸ ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
ਸਥਿਰ size_t
mbs_ਵਿੱਥ_ਮੰਜ਼ਿਲ_chars (ਅੱਖਰ ਅਚਲ *s, size_t ofs)
{
  size_t c = 0;
  ਲਈ (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  ਵਾਪਸ c;
}



ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s ਵਿਅੰਜਕ\n\
  or:  %s OPTION\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      ਅੱਖਰ_ਲਿਖੋ ('\n');
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Print the ਮੁੱਲ ਦਾ ਵਿਅੰਜਕ ਮੰਜ਼ਿਲ standard output.  A blank ਸਤਰ ਹੇਠਾਂ\n\
separates increasing precedence groups.  ਵਿਅੰਜਕ may be:\n\
\n\
  ARG1 | ARG2       ARG1 ਜੇ it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 ਜੇ neither ਦਲੀਲ is null or 0, otherwise 0\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 < ARG2       ARG1 is less than ARG2\n\
  ARG1 <= ARG2      ARG1 is less than or equal ਮੰਜ਼ਿਲ ARG2\n\
  ARG1 = ARG2       ARG1 is equal ਮੰਜ਼ਿਲ ARG2\n\
  ARG1 != ARG2      ARG1 is unequal ਮੰਜ਼ਿਲ ARG2\n\
  ARG1 >= ARG2      ARG1 is greater than or equal ਮੰਜ਼ਿਲ ARG2\n\
  ARG1 > ARG2       ARG1 is greater than ARG2\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 + ARG2       arithmetic ਜੋੜ ਦਾ ARG1 and ARG2\n\
  ARG1 - ARG2       arithmetic difference ਦਾ ARG1 and ARG2\n\
"), stdout);
      /* Tell xgettext that the "% A" below is not a printf-style
         format string:  xgettext:no-c-format */
      fputs (_("\
\n\
  ARG1 * ARG2       arithmetic ਗੁਣਨਫਲ ਦਾ ARG1 and ARG2\n\
  ARG1 / ARG2       arithmetic quotient ਦਾ ARG1 divided by ARG2\n\
  ARG1 % ARG2       arithmetic remainder ਦਾ ARG1 divided by ARG2\n\
"), stdout);
      fputs (_("\
\n\
  STRING : REGEXP   anchored ਨਮੂਨਾ_2 ਮੇਲ ਦਾ REGEXP ਵਿੱਚ STRING\n\
\n\
  ਮੇਲ STRING REGEXP        same as STRING : REGEXP\n\
  substr STRING POS ਲੰਮਾਈ_2   substring ਦਾ STRING, POS counted from 1\n\
  index STRING CHARS         index ਵਿੱਚ STRING where any CHARS is ਮਿਲਿਆ, or 0\n\
  ਲੰਮਾਈ STRING              ਲੰਮਾਈ ਦਾ STRING\n\
"), stdout);
      fputs (_("\
  + TOKEN                    interpret TOKEN as a string, even ਜੇ it is a\n\
                               keyword like 'match' or an ਸੰਚਾਲਕ like '/'\n\
\n\
  ( ਵਿਅੰਜਕ )             ਮੁੱਲ ਦਾ ਵਿਅੰਜਕ\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need ਮੰਜ਼ਿਲ be escaped or quoted ਲਈ shells.\n\
Comparisons are arithmetic ਜੇ both ਦਲੀਲਾਂ_2 are numbers, ਨਹੀਂਤਾਂ lexicographical.\n\
ਨਮੂਨਾ_3 matches ਵਾਪਸ the string matched between \\( and \\) or null; ਜੇ\n\
\\( and \\) are not ਵਰਤਿਆ, they ਵਾਪਸ the ਸੰਖਿਆ ਦਾ characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit ਸਥਿਤੀ is 0 ਜੇ ਵਿਅੰਜਕ is neither null nor 0, 1 ਜੇ ਵਿਅੰਜਕ is null\n\
or 0, 2 ਜੇ ਵਿਅੰਜਕ is syntactically invalid, and 3 ਜੇ an error occurred.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}


ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (ਵਿਅੰਜਕ_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE_NAME, ਸੰਸਕਰਣ_2,
                      usage, AUTHORS, (ਅੱਖਰ ਅਚਲ *) ਨਿਰਾ);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  ਜੇ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ਜੇ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (ਵਿਅੰਜਕ_INVALID);
    }

  ਦਲੀਲਾਂ = argv + 1;

  ਮੁੱਲ_2 *v = eval (true);
  ਜੇ (!nomoreargs ())
    error (ਵਿਅੰਜਕ_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *ਦਲੀਲਾਂ));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

ਸਥਿਰ ਮੁੱਲ_2 *
int_ਮੁੱਲ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ i)
{
  ਮੁੱਲ_2 *v = xmalloc (ਆਕਾਰ *v);
  v->type = integer;
  mpz_init_ਸੈੱਟ_ui (v->u.i, i);
  ਵਾਪਸ v;
}

/* Return a VALUE for S.  */

ਸਥਿਰ ਮੁੱਲ_2 *
str_ਮੁੱਲ (ਅੱਖਰ ਅਚਲ *s)
{
  ਮੁੱਲ_2 *v = xmalloc (ਆਕਾਰ *v);
  v->type = string;
  v->u.s = xstrdup (s);
  ਵਾਪਸ v;
}

/* Free VALUE V, including structure components.  */

ਸਥਿਰ ਖਾਲੀ
freev (ਮੁੱਲ_2 *v)
{
  ਜੇ (v->type == string)
    ਮੁਕਤ (v->u.s);
  ਨਹੀਂਤਾਂ
    mpz_clear (v->u.i);
  ਮੁਕਤ (v);
}

/* Print VALUE V.  */

ਸਥਿਰ ਖਾਲੀ
printv (ਮੁੱਲ_2 *v)
{
  ਚੋਣ (v->type)
    {
    ਹਾਲਤ integer:
      mpz_ਬਾਹਰਲਾ_str (stdout, 10, v->u.i);
      ਅੱਖਰ_ਲਿਖੋ ('\n');
      ਰੋਕੋ;
    ਹਾਲਤ string:
      puts (v->u.s);
      ਰੋਕੋ;
    ਮੂਲ:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
null (ਮੁੱਲ_2 *v)
{
  ਚੋਣ (v->type)
    {
    ਹਾਲਤ integer:
      ਵਾਪਸ mpz_sgn (v->u.i) == 0;
    ਹਾਲਤ string:
      {
        ਅੱਖਰ ਅਚਲ *cp = v->u.s;
        ਜੇ (*cp == '\0')
          ਵਾਪਸ true;

        cp += (*cp == '-');

        ਕਰੋ
          {
            ਜੇ (*cp != '0')
              ਵਾਪਸ false;
          }
        ਜਦੋਂ (*++cp);

        ਵਾਪਸ true;
      }
    ਮੂਲ:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
looks_like_integer (ਅੱਖਰ ਅਚਲ *cp)
{
  cp += (*cp == '-');

  ਕਰੋ
    ਜੇ (! c_isdigit (*cp))
      ਵਾਪਸ false;
  ਜਦੋਂ (*++cp);

  ਵਾਪਸ true;
}

/* Coerce V to a string value (can't fail).  */

ਸਥਿਰ ਖਾਲੀ
tostring (ਮੁੱਲ_2 *v)
{
  ਚੋਣ (v->type)
    {
    ਹਾਲਤ integer:
      {
        ਅੱਖਰ *s = mpz_get_str (ਨਿਰਾ, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      ਰੋਕੋ;
    ਹਾਲਤ string:
      ਰੋਕੋ;
    ਮੂਲ:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

ਸਥਿਰ bool
toarith (ਮੁੱਲ_2 *v)
{
  ਚੋਣ (v->type)
    {
    ਹਾਲਤ integer:
      ਵਾਪਸ true;
    ਹਾਲਤ string:
      {
        ਅੱਖਰ *s = v->u.s;

        ਜੇ (! looks_like_integer (s))
          ਵਾਪਸ false;
        ਜੇ (mpz_init_ਸੈੱਟ_str (v->u.i, s, 10) != 0)
          error (ਵਿਅੰਜਕ_FAILURE, ERANGE, "%s", (s));
        ਮੁਕਤ (s);
        v->type = integer;
        ਵਾਪਸ true;
      }
    ਮੂਲ:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
ਸਥਿਰ size_t
getsize (mpz_t i)
{
  ਜੇ (mpz_sgn (i) < 0)
    ਵਾਪਸ SIZE_MAX;
  ਜੇ (mpz_ਸਮਾਉਂਦਾ_ulong_p (i))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ ul = mpz_get_ui (i);
      ਜੇ (ul < SIZE_MAX)
        ਵਾਪਸ ul;
    }
  ਵਾਪਸ SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

ਸਥਿਰ bool
nextarg (ਅੱਖਰ ਅਚਲ *str)
{
  ਜੇ (*ਦਲੀਲਾਂ == ਨਿਰਾ)
    ਵਾਪਸ false;
  ਨਹੀਂਤਾਂ
    {
      bool r = streq (*ਦਲੀਲਾਂ, str);
      ਦਲੀਲਾਂ += r;
      ਵਾਪਸ r;
    }
}

/* Return true if there no more tokens.  */

ਸਥਿਰ bool
nomoreargs (ਖਾਲੀ)
{
  ਵਾਪਸ !*ਦਲੀਲਾਂ;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
ਸਥਿਰ ਖਾਲੀ
require_ਹੋਰ_ਦਲੀਲਾਂ (ਖਾਲੀ)
{
  ਜੇ (nomoreargs ())
    error (ਵਿਅੰਜਕ_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(ਦਲੀਲਾਂ - 1)));
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EVAL_TRACE
/* Print evaluation trace and args remaining.  */

ਸਥਿਰ ਖਾਲੀ
trace (ਅੱਖਰ ਅਚਲ *fxn)
{
  ਲਿਖੋ ("%s:", fxn);
  ਲਈ (ਅੱਖਰ **a = ਦਲੀਲਾਂ; *a; a++)
    ਲਿਖੋ (" %s", *a);
  ਅੱਖਰ_ਲਿਖੋ ('\n');
}
#ਜੇ_ਅੰਤ

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

ਸਥਿਰ ਮੁੱਲ_2 *
docolon (ਮੁੱਲ_2 *sv, ਮੁੱਲ_2 *pv)
{
  tostring (sv);
  tostring (pv);

  ਬਣਤਰ re_registers re_regs;
  re_regs.ਅੰਕ_regs = 0;
  re_regs.ਸ਼ੁਰੂ = ਨਿਰਾ;
  re_regs.ਸਿਰਾ = ਨਿਰਾ;

  ਬਣਤਰ re_ਨਮੂਨਾ_ਬਫਰ re_ਬਫਰ;
  ਅੱਖਰ fastmap[UCHAR_MAX + 1];
  re_ਬਫਰ.ਬਫਰ = ਨਿਰਾ;
  re_ਬਫਰ.allocated = 0;
  re_ਬਫਰ.fastmap = fastmap;
  re_ਬਫਰ.translate = ਨਿਰਾ;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_ਖਾਲੀ_ਥਾਵਾਂ_RANGES);
  ਅੱਖਰ ਅਚਲ *errmsg = re_compile_ਨਮੂਨਾ (pv->u.s, ਲੜੀ_ਲੰਬਾਈ (pv->u.s),
                                           &re_ਬਫਰ);
  ਜੇ (errmsg)
    error (ਵਿਅੰਜਕ_INVALID, 0, "%s", (errmsg));
  re_ਬਫਰ.newline_anchor = 0;

  ਮੁੱਲ_2 *v;
  regoff_t matchlen = re_ਮੇਲ (&re_ਬਫਰ, sv->u.s, ਲੜੀ_ਲੰਬਾਈ (sv->u.s), 0,
                                &re_regs);
  ਜੇ (0 <= matchlen)
    {
      /* Were \(...\) used? */
      ਜੇ (re_ਬਫਰ.re_nsub > 0)
        {
          ਜੇ (re_regs.ਸਿਰਾ[1] < 0)
            v = str_ਮੁੱਲ ("");
          ਨਹੀਂਤਾਂ
            {
              sv->u.s[re_regs.ਸਿਰਾ[1]] = '\0';
              v = str_ਮੁੱਲ (sv->u.s + re_regs.ਸ਼ੁਰੂ[1]);
            }
        }
      ਨਹੀਂਤਾਂ
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_ਵਿੱਥ_ਮੰਜ਼ਿਲ_chars (sv->u.s, matchlen));
          v = int_ਮੁੱਲ (i);
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      ਜੇ (re_ਬਫਰ.re_nsub > 0)
        v = str_ਮੁੱਲ ("");
      ਨਹੀਂਤਾਂ
        v = int_ਮੁੱਲ (0);
    }
  ਨਹੀਂਤਾਂ
    error (ਵਿਅੰਜਕ_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  ਜੇ (0 < re_regs.ਅੰਕ_regs)
    {
      ਮੁਕਤ (re_regs.ਸ਼ੁਰੂ);
      ਮੁਕਤ (re_regs.ਸਿਰਾ);
    }
  re_ਬਫਰ.fastmap = ਨਿਰਾ;
  regfree (&re_ਬਫਰ);
  ਵਾਪਸ v;
}

/* Handle bare operands and ( expr ) syntax.  */

ਸਥਿਰ ਮੁੱਲ_2 *
eval7 (bool evaluate)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EVAL_TRACE
  trace ("eval7");
#ਜੇ_ਅੰਤ
  require_ਹੋਰ_ਦਲੀਲਾਂ ();

  ਜੇ (nextarg ("("))
    {
      ਮੁੱਲ_2 *v = eval (evaluate);
      ਜੇ (nomoreargs ())
        error (ਵਿਅੰਜਕ_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(ਦਲੀਲਾਂ - 1)));
      ਜੇ (!nextarg (")"))
        error (ਵਿਅੰਜਕ_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *ਦਲੀਲਾਂ));
      ਵਾਪਸ v;
    }

  ਜੇ (nextarg (")"))
    error (ਵਿਅੰਜਕ_INVALID, 0, _("syntax error: unexpected ')'"));

  ਵਾਪਸ str_ਮੁੱਲ (*ਦਲੀਲਾਂ++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

ਸਥਿਰ ਮੁੱਲ_2 *
eval6 (bool evaluate)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EVAL_TRACE
  trace ("eval6");
#ਜੇ_ਅੰਤ
  ਜੇ (nextarg ("+"))
    {
      require_ਹੋਰ_ਦਲੀਲਾਂ ();
      ਵਾਪਸ str_ਮੁੱਲ (*ਦਲੀਲਾਂ++);
    }
  ਨਹੀਂਤਾਂ ਜੇ (nextarg ("length"))
    {
      ਮੁੱਲ_2 *r = eval6 (evaluate);
      tostring (r);
      ਮੁੱਲ_2 *v = int_ਮੁੱਲ (mbslen (r->u.s));
      freev (r);
      ਵਾਪਸ v;
    }
  ਨਹੀਂਤਾਂ ਜੇ (nextarg ("match"))
    {
      ਮੁੱਲ_2 *l = eval6 (evaluate);
      ਮੁੱਲ_2 *r = eval6 (evaluate);
      ਮੁੱਲ_2 *v;
      ਜੇ (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      ਨਹੀਂਤਾਂ
        v = l;
      freev (r);
      ਵਾਪਸ v;
    }
  ਨਹੀਂਤਾਂ ਜੇ (nextarg ("index"))
    {
      ਮੁੱਲ_2 *l = eval6 (evaluate);
      ਮੁੱਲ_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      ਮੁੱਲ_2 *v = int_ਮੁੱਲ (pos);
      freev (l);
      freev (r);
      ਵਾਪਸ v;
    }
  ਨਹੀਂਤਾਂ ਜੇ (nextarg ("substr"))
    {
      ਮੁੱਲ_2 *l = eval6 (evaluate);
      ਮੁੱਲ_2 *i1 = eval6 (evaluate);
      ਮੁੱਲ_2 *i2 = eval6 (evaluate);
      tostring (l);

      ਮੁੱਲ_2 *v;
      ਜੇ (!toarith (i1) || !toarith (i2))
        v = str_ਮੁੱਲ ("");
      ਨਹੀਂਤਾਂ
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          ਅੱਖਰ *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_ਮੁੱਲ (s);
          ਮੁਕਤ (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      ਵਾਪਸ v;
    }
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

ਸਥਿਰ ਮੁੱਲ_2 *
eval5 (bool evaluate)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EVAL_TRACE
  trace ("eval5");
#ਜੇ_ਅੰਤ
  ਮੁੱਲ_2 *l = eval6 (evaluate);
  ਜਦੋਂ (true)
    {
      ਜੇ (nextarg (":"))
        {
          ਮੁੱਲ_2 *r = eval6 (evaluate);
          ਜੇ (evaluate)
            {
              ਮੁੱਲ_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ l;
    }
}

/* Handle *, /, % operators.  */

ਸਥਿਰ ਮੁੱਲ_2 *
eval4 (bool evaluate)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EVAL_TRACE
  trace ("eval4");
#ਜੇ_ਅੰਤ
  ਮੁੱਲ_2 *l = eval5 (evaluate);
  ਜਦੋਂ (true)
    {
      ਗਿਣਤੀ { multiply, divide, mod } fxn;

      ਜੇ (nextarg ("*"))
        fxn = multiply;
      ਨਹੀਂਤਾਂ ਜੇ (nextarg ("/"))
        fxn = divide;
      ਨਹੀਂਤਾਂ ਜੇ (nextarg ("%"))
        fxn = mod;
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ l;
      ਮੁੱਲ_2 *r = eval5 (evaluate);
      ਜੇ (evaluate)
        {
          ਜੇ (!toarith (l) || !toarith (r))
            error (ਵਿਅੰਜਕ_INVALID, 0, _("non-integer argument"));
          ਜੇ (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (ਵਿਅੰਜਕ_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

ਸਥਿਰ ਮੁੱਲ_2 *
eval3 (bool evaluate)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EVAL_TRACE
  trace ("eval3");
#ਜੇ_ਅੰਤ
  ਮੁੱਲ_2 *l = eval4 (evaluate);
  ਜਦੋਂ (true)
    {
      ਗਿਣਤੀ { plus, minus } fxn;

      ਜੇ (nextarg ("+"))
        fxn = plus;
      ਨਹੀਂਤਾਂ ਜੇ (nextarg ("-"))
        fxn = minus;
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ l;
      ਮੁੱਲ_2 *r = eval4 (evaluate);
      ਜੇ (evaluate)
        {
          ਜੇ (!toarith (l) || !toarith (r))
            error (ਵਿਅੰਜਕ_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_ਜੋੜੋ : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

ਸਥਿਰ ਮੁੱਲ_2 *
eval2 (bool evaluate)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EVAL_TRACE
  trace ("eval2");
#ਜੇ_ਅੰਤ
  ਮੁੱਲ_2 *l = eval3 (evaluate);
  ਜਦੋਂ (true)
    {
      ਗਿਣਤੀ
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      ਜੇ (nextarg ("<"))
        fxn = less_than;
      ਨਹੀਂਤਾਂ ਜੇ (nextarg ("<="))
        fxn = less_equal;
      ਨਹੀਂਤਾਂ ਜੇ (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      ਨਹੀਂਤਾਂ ਜੇ (nextarg ("!="))
        fxn = not_equal;
      ਨਹੀਂਤਾਂ ਜੇ (nextarg (">="))
        fxn = greater_equal;
      ਨਹੀਂਤਾਂ ਜੇ (nextarg (">"))
        fxn = greater_than;
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ l;
      ਮੁੱਲ_2 *r = eval3 (evaluate);

      bool val = false;
      ਜੇ (evaluate)
        {
          tostring (l);
          tostring (r);

          ਪੂਰਨ cmp;
          ਜੇ (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          ਨਹੀਂਤਾਂ
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              ਜੇ (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (ਵਿਅੰਜਕ_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          ਚੋਣ (fxn)
            {
            ਹਾਲਤ less_than:     val = (cmp <  0); ਰੋਕੋ;
            ਹਾਲਤ less_equal:    val = (cmp <= 0); ਰੋਕੋ;
            ਹਾਲਤ equal:         val = (cmp == 0); ਰੋਕੋ;
            ਹਾਲਤ not_equal:     val = (cmp != 0); ਰੋਕੋ;
            ਹਾਲਤ greater_equal: val = (cmp >= 0); ਰੋਕੋ;
            ਹਾਲਤ greater_than:  val = (cmp >  0); ਰੋਕੋ;
            ਮੂਲ: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_ਮੁੱਲ (val);
    }
}

/* Handle &.  */

ਸਥਿਰ ਮੁੱਲ_2 *
eval1 (bool evaluate)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EVAL_TRACE
  trace ("eval1");
#ਜੇ_ਅੰਤ
  ਮੁੱਲ_2 *l = eval2 (evaluate);
  ਜਦੋਂ (true)
    {
      ਜੇ (nextarg ("&"))
        {
          ਮੁੱਲ_2 *r = eval2 (evaluate && !null (l));
          ਜੇ (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_ਮੁੱਲ (0);
            }
          ਨਹੀਂਤਾਂ
            freev (r);
        }
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ l;
    }
}

/* Handle |.  */

ਸਥਿਰ ਮੁੱਲ_2 *
eval (bool evaluate)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EVAL_TRACE
  trace ("eval");
#ਜੇ_ਅੰਤ
  ਮੁੱਲ_2 *l = eval1 (evaluate);
  ਜਦੋਂ (true)
    {
      ਜੇ (nextarg ("|"))
        {
          ਮੁੱਲ_2 *r = eval1 (evaluate && null (l));
          ਜੇ (null (l))
            {
              freev (l);
              l = r;
              ਜੇ (null (l))
                {
                  freev (l);
                  l = int_ਮੁੱਲ (0);
                }
            }
          ਨਹੀਂਤਾਂ
            freev (r);
        }
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ l;
    }
}
