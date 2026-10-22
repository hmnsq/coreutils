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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <gmp.h>
#અન્તર્ભાવ <regex.h>
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "long-options.h"
#અન્તર્ભાવ "mcel.h"
#અન્તર્ભાવ "strnumcmp.h"

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
ગણના
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    પદાવલિ_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    પદાવલિ_FAILURE
  };

/* The kinds of value we can have.  */
ગણના valtype
{
  integer,
  string
};
પ્રકારવ્યાખ્યા ગણના valtype TYPE;

/* A value is.... */
સંરચના valinfo
{
  TYPE type;			/* Which kind. */
  સંઘ
  {				/* The value itself. */
    mpz_t i;
    અક્ષર *s;
  } u;
};
પ્રકારવ્યાખ્યા સંરચના valinfo કિંમત_2;

/* The arguments given to the program, minus the program name.  */
સ્થિર અક્ષર **args;

સ્થિર કિંમત_2 *eval (bool);
સ્થિર bool nomoreargs (શૂન્ય);
સ્થિર bool null (કિંમત_2 *v);
સ્થિર શૂન્ય printv (કિંમત_2 *v);


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
સ્થિર size_t
mbs_logical_cspn (અક્ષર અચલ *s, અક્ષર અચલ *accept)
{
  જો (accept[0] == '\0')
    પાછા 0;

  /* General case.  */
  જો (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      માટે (અક્ષર અચલ *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          જો (g.len == 1)
            {
              જો (mbschr (accept, *p))
                પાછા idx;
            }
          નહીંતર
            માટે (અક્ષર અચલ *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                જો (mcel_eq (g, h))
                  પાછા idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  નહીંતર
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      જો (s[i])
        પાછા i + 1;
    }

  /* not found */
  પાછા 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
સ્થિર અક્ષર *
mbs_logical_substr (અક્ષર અચલ *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? strlen (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  અક્ષર અચલ *substart = s;
  idx_t sublen = 0;
  જો (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  નહીંતર જો (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  નહીંતર
    માટે (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        જો (pos <= idx)
          {
            જો (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
      }

  પાછા ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
સ્થિર size_t
mbs_offset_to_chars (અક્ષર અચલ *s, size_t ofs)
{
  size_t c = 0;
  માટે (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  પાછા c;
}



શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
Print the કિંમત of EXPRESSION to standard output.  A blank લીટી below\n\
separates increasing precedence groups.  EXPRESSION may be:\n\
\n\
  ARG1 | ARG2       ARG1 જો it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 જો neither argument is null or 0, otherwise 0\n\
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
  + TOKEN                    interpret TOKEN as a string, even જો it is a\n\
                               keyword like 'match' or an operator like '/'\n\
\n\
  ( EXPRESSION )             કિંમત of EXPRESSION\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need to be escaped or quoted માટે shells.\n\
Comparisons are arithmetic જો both ARGs are numbers, નહીંતર lexicographical.\n\
Pattern matches પાછા the string matched between \\( and \\) or null; જો\n\
\\( and \\) are not used, they પાછા the સંખ્યા of characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit status is 0 જો EXPRESSION is neither null nor 0, 1 જો EXPRESSION is null\n\
or 0, 2 જો EXPRESSION is syntactically invalid, and 3 જો an error occurred.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (પદાવલિ_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, PROGRAM_NAME, PACKAGE_NAME, VERSION,
                      usage, AUTHORS, (અક્ષર અચલ *) ખાલી);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  જો (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  જો (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (પદાવલિ_INVALID);
    }

  args = argv + 1;

  કિંમત_2 *v = eval (true);
  જો (!nomoreargs ())
    error (પદાવલિ_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

સ્થિર કિંમત_2 *
int_કિંમત (unsigned long પૂર્ણાંક i)
{
  કિંમત_2 *v = xmalloc (કદ *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  પાછા v;
}

/* Return a VALUE for S.  */

સ્થિર કિંમત_2 *
str_કિંમત (અક્ષર અચલ *s)
{
  કિંમત_2 *v = xmalloc (કદ *v);
  v->type = string;
  v->u.s = xstrdup (s);
  પાછા v;
}

/* Free VALUE V, including structure components.  */

સ્થિર શૂન્ય
freev (કિંમત_2 *v)
{
  જો (v->type == string)
    મુક્ત (v->u.s);
  નહીંતર
    mpz_clear (v->u.i);
  મુક્ત (v);
}

/* Print VALUE V.  */

સ્થિર શૂન્ય
printv (કિંમત_2 *v)
{
  પસંદગી (v->type)
    {
    સ્થિતિ integer:
      mpz_out_str (stdout, 10, v->u.i);
      putchar ('\n');
      તોડો;
    સ્થિતિ string:
      puts (v->u.s);
      તોડો;
    મૂળભૂત:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_PURE
સ્થિર bool
null (કિંમત_2 *v)
{
  પસંદગી (v->type)
    {
    સ્થિતિ integer:
      પાછા mpz_sgn (v->u.i) == 0;
    સ્થિતિ string:
      {
        અક્ષર અચલ *cp = v->u.s;
        જો (*cp == '\0')
          પાછા true;

        cp += (*cp == '-');

        કરો
          {
            જો (*cp != '0')
              પાછા false;
          }
        જ્યારે (*++cp);

        પાછા true;
      }
    મૂળભૂત:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_PURE
સ્થિર bool
looks_like_integer (અક્ષર અચલ *cp)
{
  cp += (*cp == '-');

  કરો
    જો (! c_isdigit (*cp))
      પાછા false;
  જ્યારે (*++cp);

  પાછા true;
}

/* Coerce V to a string value (can't fail).  */

સ્થિર શૂન્ય
tostring (કિંમત_2 *v)
{
  પસંદગી (v->type)
    {
    સ્થિતિ integer:
      {
        અક્ષર *s = mpz_get_str (ખાલી, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      તોડો;
    સ્થિતિ string:
      તોડો;
    મૂળભૂત:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

સ્થિર bool
toarith (કિંમત_2 *v)
{
  પસંદગી (v->type)
    {
    સ્થિતિ integer:
      પાછા true;
    સ્થિતિ string:
      {
        અક્ષર *s = v->u.s;

        જો (! looks_like_integer (s))
          પાછા false;
        જો (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (પદાવલિ_FAILURE, ERANGE, "%s", (s));
        મુક્ત (s);
        v->type = integer;
        પાછા true;
      }
    મૂળભૂત:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
સ્થિર size_t
getsize (mpz_t i)
{
  જો (mpz_sgn (i) < 0)
    પાછા SIZE_MAX;
  જો (mpz_fits_ulong_p (i))
    {
      unsigned long પૂર્ણાંક ul = mpz_get_ui (i);
      જો (ul < SIZE_MAX)
        પાછા ul;
    }
  પાછા SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

સ્થિર bool
nextarg (અક્ષર અચલ *str)
{
  જો (*args == ખાલી)
    પાછા false;
  નહીંતર
    {
      bool r = streq (*args, str);
      args += r;
      પાછા r;
    }
}

/* Return true if there no more tokens.  */

સ્થિર bool
nomoreargs (શૂન્ય)
{
  પાછા !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
સ્થિર શૂન્ય
require_more_args (શૂન્ય)
{
  જો (nomoreargs ())
    error (પદાવલિ_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#ifdef EVAL_TRACE
/* Print evaluation trace and args remaining.  */

સ્થિર શૂન્ય
trace (અક્ષર અચલ *fxn)
{
  printf ("%s:", fxn);
  માટે (અક્ષર **a = args; *a; a++)
    printf (" %s", *a);
  putchar ('\n');
}
#endif

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

સ્થિર કિંમત_2 *
docolon (કિંમત_2 *sv, કિંમત_2 *pv)
{
  tostring (sv);
  tostring (pv);

  સંરચના re_registers re_regs;
  re_regs.અંક_regs = 0;
  re_regs.start = ખાલી;
  re_regs.end = ખાલી;

  સંરચના re_pattern_buffer re_buffer;
  અક્ષર fastmap[UCHAR_MAX + 1];
  re_buffer.buffer = ખાલી;
  re_buffer.allocated = 0;
  re_buffer.fastmap = fastmap;
  re_buffer.translate = ખાલી;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_EMPTY_RANGES);
  અક્ષર અચલ *errmsg = re_compile_pattern (pv->u.s, strlen (pv->u.s),
                                           &re_buffer);
  જો (errmsg)
    error (પદાવલિ_INVALID, 0, "%s", (errmsg));
  re_buffer.newline_anchor = 0;

  કિંમત_2 *v;
  regoff_t matchlen = re_match (&re_buffer, sv->u.s, strlen (sv->u.s), 0,
                                &re_regs);
  જો (0 <= matchlen)
    {
      /* Were \(...\) used? */
      જો (re_buffer.re_nsub > 0)
        {
          જો (re_regs.end[1] < 0)
            v = str_કિંમત ("");
          નહીંતર
            {
              sv->u.s[re_regs.end[1]] = '\0';
              v = str_કિંમત (sv->u.s + re_regs.start[1]);
            }
        }
      નહીંતર
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_કિંમત (i);
        }
    }
  નહીંતર જો (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      જો (re_buffer.re_nsub > 0)
        v = str_કિંમત ("");
      નહીંતર
        v = int_કિંમત (0);
    }
  નહીંતર
    error (પદાવલિ_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  જો (0 < re_regs.અંક_regs)
    {
      મુક્ત (re_regs.start);
      મુક્ત (re_regs.end);
    }
  re_buffer.fastmap = ખાલી;
  regfree (&re_buffer);
  પાછા v;
}

/* Handle bare operands and ( expr ) syntax.  */

સ્થિર કિંમત_2 *
eval7 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval7");
#endif
  require_more_args ();

  જો (nextarg ("("))
    {
      કિંમત_2 *v = eval (evaluate);
      જો (nomoreargs ())
        error (પદાવલિ_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      જો (!nextarg (")"))
        error (પદાવલિ_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      પાછા v;
    }

  જો (nextarg (")"))
    error (પદાવલિ_INVALID, 0, _("syntax error: unexpected ')'"));

  પાછા str_કિંમત (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

સ્થિર કિંમત_2 *
eval6 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval6");
#endif
  જો (nextarg ("+"))
    {
      require_more_args ();
      પાછા str_કિંમત (*args++);
    }
  નહીંતર જો (nextarg ("length"))
    {
      કિંમત_2 *r = eval6 (evaluate);
      tostring (r);
      કિંમત_2 *v = int_કિંમત (mbslen (r->u.s));
      freev (r);
      પાછા v;
    }
  નહીંતર જો (nextarg ("match"))
    {
      કિંમત_2 *l = eval6 (evaluate);
      કિંમત_2 *r = eval6 (evaluate);
      કિંમત_2 *v;
      જો (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      નહીંતર
        v = l;
      freev (r);
      પાછા v;
    }
  નહીંતર જો (nextarg ("index"))
    {
      કિંમત_2 *l = eval6 (evaluate);
      કિંમત_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      કિંમત_2 *v = int_કિંમત (pos);
      freev (l);
      freev (r);
      પાછા v;
    }
  નહીંતર જો (nextarg ("substr"))
    {
      કિંમત_2 *l = eval6 (evaluate);
      કિંમત_2 *i1 = eval6 (evaluate);
      કિંમત_2 *i2 = eval6 (evaluate);
      tostring (l);

      કિંમત_2 *v;
      જો (!toarith (i1) || !toarith (i2))
        v = str_કિંમત ("");
      નહીંતર
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          અક્ષર *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_કિંમત (s);
          મુક્ત (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      પાછા v;
    }
  નહીંતર
    પાછા eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

સ્થિર કિંમત_2 *
eval5 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval5");
#endif
  કિંમત_2 *l = eval6 (evaluate);
  જ્યારે (true)
    {
      જો (nextarg (":"))
        {
          કિંમત_2 *r = eval6 (evaluate);
          જો (evaluate)
            {
              કિંમત_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      નહીંતર
        પાછા l;
    }
}

/* Handle *, /, % operators.  */

સ્થિર કિંમત_2 *
eval4 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval4");
#endif
  કિંમત_2 *l = eval5 (evaluate);
  જ્યારે (true)
    {
      ગણના { multiply, divide, mod } fxn;

      જો (nextarg ("*"))
        fxn = multiply;
      નહીંતર જો (nextarg ("/"))
        fxn = divide;
      નહીંતર જો (nextarg ("%"))
        fxn = mod;
      નહીંતર
        પાછા l;
      કિંમત_2 *r = eval5 (evaluate);
      જો (evaluate)
        {
          જો (!toarith (l) || !toarith (r))
            error (પદાવલિ_INVALID, 0, _("non-integer argument"));
          જો (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (પદાવલિ_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

સ્થિર કિંમત_2 *
eval3 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval3");
#endif
  કિંમત_2 *l = eval4 (evaluate);
  જ્યારે (true)
    {
      ગણના { plus, minus } fxn;

      જો (nextarg ("+"))
        fxn = plus;
      નહીંતર જો (nextarg ("-"))
        fxn = minus;
      નહીંતર
        પાછા l;
      કિંમત_2 *r = eval4 (evaluate);
      જો (evaluate)
        {
          જો (!toarith (l) || !toarith (r))
            error (પદાવલિ_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_add : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

સ્થિર કિંમત_2 *
eval2 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval2");
#endif
  કિંમત_2 *l = eval3 (evaluate);
  જ્યારે (true)
    {
      ગણના
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      જો (nextarg ("<"))
        fxn = less_than;
      નહીંતર જો (nextarg ("<="))
        fxn = less_equal;
      નહીંતર જો (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      નહીંતર જો (nextarg ("!="))
        fxn = not_equal;
      નહીંતર જો (nextarg (">="))
        fxn = greater_equal;
      નહીંતર જો (nextarg (">"))
        fxn = greater_than;
      નહીંતર
        પાછા l;
      કિંમત_2 *r = eval3 (evaluate);

      bool val = false;
      જો (evaluate)
        {
          tostring (l);
          tostring (r);

          પૂર્ણાંક cmp;
          જો (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          નહીંતર
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              જો (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (પદાવલિ_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          પસંદગી (fxn)
            {
            સ્થિતિ less_than:     val = (cmp <  0); તોડો;
            સ્થિતિ less_equal:    val = (cmp <= 0); તોડો;
            સ્થિતિ equal:         val = (cmp == 0); તોડો;
            સ્થિતિ not_equal:     val = (cmp != 0); તોડો;
            સ્થિતિ greater_equal: val = (cmp >= 0); તોડો;
            સ્થિતિ greater_than:  val = (cmp >  0); તોડો;
            મૂળભૂત: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_કિંમત (val);
    }
}

/* Handle &.  */

સ્થિર કિંમત_2 *
eval1 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval1");
#endif
  કિંમત_2 *l = eval2 (evaluate);
  જ્યારે (true)
    {
      જો (nextarg ("&"))
        {
          કિંમત_2 *r = eval2 (evaluate && !null (l));
          જો (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_કિંમત (0);
            }
          નહીંતર
            freev (r);
        }
      નહીંતર
        પાછા l;
    }
}

/* Handle |.  */

સ્થિર કિંમત_2 *
eval (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval");
#endif
  કિંમત_2 *l = eval1 (evaluate);
  જ્યારે (true)
    {
      જો (nextarg ("|"))
        {
          કિંમત_2 *r = eval1 (evaluate && null (l));
          જો (null (l))
            {
              freev (l);
              l = r;
              જો (null (l))
                {
                  freev (l);
                  l = int_કિંમત (0);
                }
            }
          નહીંતર
            freev (r);
        }
      નહીંતર
        પાછા l;
    }
}
