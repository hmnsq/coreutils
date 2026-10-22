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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <gmp.h>
#அந்தர்பாவ <regex.h>
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "long-options.h"
#அந்தர்பாவ "mcel.h"
#அந்தர்பாவ "strnumcmp.h"

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
எண்ணிக்கை
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    கோவை_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    கோவை_FAILURE
  };

/* The kinds of value we can have.  */
எண்ணிக்கை valtype
{
  integer,
  string
};
வகைவரை எண்ணிக்கை valtype TYPE;

/* A value is.... */
கட்டமைப்பு valinfo
{
  TYPE type;			/* Which kind. */
  ஒன்றிப்பு
  {				/* The value itself. */
    mpz_t i;
    எழுத்து *s;
  } u;
};
வகைவரை கட்டமைப்பு valinfo மதிப்பு_2;

/* The arguments given to the program, minus the program name.  */
நிலையான எழுத்து **args;

நிலையான மதிப்பு_2 *eval (bool);
நிலையான bool nomoreargs (வெறுமை);
நிலையான bool null (மதிப்பு_2 *v);
நிலையான வெறுமை printv (மதிப்பு_2 *v);


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
நிலையான size_t
mbs_logical_cspn (எழுத்து மாறிலி *s, எழுத்து மாறிலி *accept)
{
  எனில் (accept[0] == '\0')
    திரும்பு 0;

  /* General case.  */
  எனில் (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      க்கு (எழுத்து மாறிலி *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          எனில் (g.len == 1)
            {
              எனில் (mbschr (accept, *p))
                திரும்பு idx;
            }
          இல்லையெனில்
            க்கு (எழுத்து மாறிலி *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                எனில் (mcel_eq (g, h))
                  திரும்பு idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  இல்லையெனில்
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      எனில் (s[i])
        திரும்பு i + 1;
    }

  /* not found */
  திரும்பு 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
நிலையான எழுத்து *
mbs_logical_substr (எழுத்து மாறிலி *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? strlen (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  எழுத்து மாறிலி *substart = s;
  idx_t sublen = 0;
  எனில் (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  இல்லையெனில் எனில் (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  இல்லையெனில்
    க்கு (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        எனில் (pos <= idx)
          {
            எனில் (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
      }

  திரும்பு ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
நிலையான size_t
mbs_offset_to_chars (எழுத்து மாறிலி *s, size_t ofs)
{
  size_t c = 0;
  க்கு (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  திரும்பு c;
}



வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
Print the மதிப்பு of EXPRESSION to standard output.  A blank வரி below\n\
separates increasing precedence groups.  EXPRESSION may be:\n\
\n\
  ARG1 | ARG2       ARG1 எனில் it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 எனில் neither argument is null or 0, otherwise 0\n\
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
  + TOKEN                    interpret TOKEN as a string, even எனில் it is a\n\
                               keyword like 'match' or an operator like '/'\n\
\n\
  ( EXPRESSION )             மதிப்பு of EXPRESSION\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need to be escaped or quoted க்கு shells.\n\
Comparisons are arithmetic எனில் both ARGs are numbers, இல்லையெனில் lexicographical.\n\
Pattern matches திரும்பு the string matched between \\( and \\) or null; எனில்\n\
\\( and \\) are not used, they திரும்பு the எண் of characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit status is 0 எனில் EXPRESSION is neither null nor 0, 1 எனில் EXPRESSION is null\n\
or 0, 2 எனில் EXPRESSION is syntactically invalid, and 3 எனில் an error occurred.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (கோவை_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, PROGRAM_NAME, PACKAGE_NAME, VERSION,
                      usage, AUTHORS, (எழுத்து மாறிலி *) வெற்று);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  எனில் (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  எனில் (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (கோவை_INVALID);
    }

  args = argv + 1;

  மதிப்பு_2 *v = eval (true);
  எனில் (!nomoreargs ())
    error (கோவை_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

நிலையான மதிப்பு_2 *
int_மதிப்பு (unsigned long முழு i)
{
  மதிப்பு_2 *v = xmalloc (அளவு *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  திரும்பு v;
}

/* Return a VALUE for S.  */

நிலையான மதிப்பு_2 *
str_மதிப்பு (எழுத்து மாறிலி *s)
{
  மதிப்பு_2 *v = xmalloc (அளவு *v);
  v->type = string;
  v->u.s = xstrdup (s);
  திரும்பு v;
}

/* Free VALUE V, including structure components.  */

நிலையான வெறுமை
freev (மதிப்பு_2 *v)
{
  எனில் (v->type == string)
    விடுவி (v->u.s);
  இல்லையெனில்
    mpz_clear (v->u.i);
  விடுவி (v);
}

/* Print VALUE V.  */

நிலையான வெறுமை
printv (மதிப்பு_2 *v)
{
  தேர்வு (v->type)
    {
    நிலை integer:
      mpz_out_str (stdout, 10, v->u.i);
      putchar ('\n');
      நிறுத்து;
    நிலை string:
      puts (v->u.s);
      நிறுத்து;
    இயல்பு:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_PURE
நிலையான bool
null (மதிப்பு_2 *v)
{
  தேர்வு (v->type)
    {
    நிலை integer:
      திரும்பு mpz_sgn (v->u.i) == 0;
    நிலை string:
      {
        எழுத்து மாறிலி *cp = v->u.s;
        எனில் (*cp == '\0')
          திரும்பு true;

        cp += (*cp == '-');

        செய்
          {
            எனில் (*cp != '0')
              திரும்பு false;
          }
        போது (*++cp);

        திரும்பு true;
      }
    இயல்பு:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_PURE
நிலையான bool
looks_like_integer (எழுத்து மாறிலி *cp)
{
  cp += (*cp == '-');

  செய்
    எனில் (! c_isdigit (*cp))
      திரும்பு false;
  போது (*++cp);

  திரும்பு true;
}

/* Coerce V to a string value (can't fail).  */

நிலையான வெறுமை
tostring (மதிப்பு_2 *v)
{
  தேர்வு (v->type)
    {
    நிலை integer:
      {
        எழுத்து *s = mpz_get_str (வெற்று, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      நிறுத்து;
    நிலை string:
      நிறுத்து;
    இயல்பு:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

நிலையான bool
toarith (மதிப்பு_2 *v)
{
  தேர்வு (v->type)
    {
    நிலை integer:
      திரும்பு true;
    நிலை string:
      {
        எழுத்து *s = v->u.s;

        எனில் (! looks_like_integer (s))
          திரும்பு false;
        எனில் (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (கோவை_FAILURE, ERANGE, "%s", (s));
        விடுவி (s);
        v->type = integer;
        திரும்பு true;
      }
    இயல்பு:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
நிலையான size_t
getsize (mpz_t i)
{
  எனில் (mpz_sgn (i) < 0)
    திரும்பு SIZE_MAX;
  எனில் (mpz_fits_ulong_p (i))
    {
      unsigned long முழு ul = mpz_get_ui (i);
      எனில் (ul < SIZE_MAX)
        திரும்பு ul;
    }
  திரும்பு SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

நிலையான bool
nextarg (எழுத்து மாறிலி *str)
{
  எனில் (*args == வெற்று)
    திரும்பு false;
  இல்லையெனில்
    {
      bool r = streq (*args, str);
      args += r;
      திரும்பு r;
    }
}

/* Return true if there no more tokens.  */

நிலையான bool
nomoreargs (வெறுமை)
{
  திரும்பு !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
நிலையான வெறுமை
require_more_args (வெறுமை)
{
  எனில் (nomoreargs ())
    error (கோவை_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#ifdef EVAL_TRACE
/* Print evaluation trace and args remaining.  */

நிலையான வெறுமை
trace (எழுத்து மாறிலி *fxn)
{
  printf ("%s:", fxn);
  க்கு (எழுத்து **a = args; *a; a++)
    printf (" %s", *a);
  putchar ('\n');
}
#endif

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

நிலையான மதிப்பு_2 *
docolon (மதிப்பு_2 *sv, மதிப்பு_2 *pv)
{
  tostring (sv);
  tostring (pv);

  கட்டமைப்பு re_registers re_regs;
  re_regs.இலக்கம்_regs = 0;
  re_regs.start = வெற்று;
  re_regs.end = வெற்று;

  கட்டமைப்பு re_pattern_buffer re_buffer;
  எழுத்து fastmap[UCHAR_MAX + 1];
  re_buffer.buffer = வெற்று;
  re_buffer.allocated = 0;
  re_buffer.fastmap = fastmap;
  re_buffer.translate = வெற்று;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_EMPTY_RANGES);
  எழுத்து மாறிலி *errmsg = re_compile_pattern (pv->u.s, strlen (pv->u.s),
                                           &re_buffer);
  எனில் (errmsg)
    error (கோவை_INVALID, 0, "%s", (errmsg));
  re_buffer.newline_anchor = 0;

  மதிப்பு_2 *v;
  regoff_t matchlen = re_match (&re_buffer, sv->u.s, strlen (sv->u.s), 0,
                                &re_regs);
  எனில் (0 <= matchlen)
    {
      /* Were \(...\) used? */
      எனில் (re_buffer.re_nsub > 0)
        {
          எனில் (re_regs.end[1] < 0)
            v = str_மதிப்பு ("");
          இல்லையெனில்
            {
              sv->u.s[re_regs.end[1]] = '\0';
              v = str_மதிப்பு (sv->u.s + re_regs.start[1]);
            }
        }
      இல்லையெனில்
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_மதிப்பு (i);
        }
    }
  இல்லையெனில் எனில் (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      எனில் (re_buffer.re_nsub > 0)
        v = str_மதிப்பு ("");
      இல்லையெனில்
        v = int_மதிப்பு (0);
    }
  இல்லையெனில்
    error (கோவை_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  எனில் (0 < re_regs.இலக்கம்_regs)
    {
      விடுவி (re_regs.start);
      விடுவி (re_regs.end);
    }
  re_buffer.fastmap = வெற்று;
  regfree (&re_buffer);
  திரும்பு v;
}

/* Handle bare operands and ( expr ) syntax.  */

நிலையான மதிப்பு_2 *
eval7 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval7");
#endif
  require_more_args ();

  எனில் (nextarg ("("))
    {
      மதிப்பு_2 *v = eval (evaluate);
      எனில் (nomoreargs ())
        error (கோவை_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      எனில் (!nextarg (")"))
        error (கோவை_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      திரும்பு v;
    }

  எனில் (nextarg (")"))
    error (கோவை_INVALID, 0, _("syntax error: unexpected ')'"));

  திரும்பு str_மதிப்பு (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

நிலையான மதிப்பு_2 *
eval6 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval6");
#endif
  எனில் (nextarg ("+"))
    {
      require_more_args ();
      திரும்பு str_மதிப்பு (*args++);
    }
  இல்லையெனில் எனில் (nextarg ("length"))
    {
      மதிப்பு_2 *r = eval6 (evaluate);
      tostring (r);
      மதிப்பு_2 *v = int_மதிப்பு (mbslen (r->u.s));
      freev (r);
      திரும்பு v;
    }
  இல்லையெனில் எனில் (nextarg ("match"))
    {
      மதிப்பு_2 *l = eval6 (evaluate);
      மதிப்பு_2 *r = eval6 (evaluate);
      மதிப்பு_2 *v;
      எனில் (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      இல்லையெனில்
        v = l;
      freev (r);
      திரும்பு v;
    }
  இல்லையெனில் எனில் (nextarg ("index"))
    {
      மதிப்பு_2 *l = eval6 (evaluate);
      மதிப்பு_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      மதிப்பு_2 *v = int_மதிப்பு (pos);
      freev (l);
      freev (r);
      திரும்பு v;
    }
  இல்லையெனில் எனில் (nextarg ("substr"))
    {
      மதிப்பு_2 *l = eval6 (evaluate);
      மதிப்பு_2 *i1 = eval6 (evaluate);
      மதிப்பு_2 *i2 = eval6 (evaluate);
      tostring (l);

      மதிப்பு_2 *v;
      எனில் (!toarith (i1) || !toarith (i2))
        v = str_மதிப்பு ("");
      இல்லையெனில்
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          எழுத்து *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_மதிப்பு (s);
          விடுவி (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      திரும்பு v;
    }
  இல்லையெனில்
    திரும்பு eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

நிலையான மதிப்பு_2 *
eval5 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval5");
#endif
  மதிப்பு_2 *l = eval6 (evaluate);
  போது (true)
    {
      எனில் (nextarg (":"))
        {
          மதிப்பு_2 *r = eval6 (evaluate);
          எனில் (evaluate)
            {
              மதிப்பு_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      இல்லையெனில்
        திரும்பு l;
    }
}

/* Handle *, /, % operators.  */

நிலையான மதிப்பு_2 *
eval4 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval4");
#endif
  மதிப்பு_2 *l = eval5 (evaluate);
  போது (true)
    {
      எண்ணிக்கை { multiply, divide, mod } fxn;

      எனில் (nextarg ("*"))
        fxn = multiply;
      இல்லையெனில் எனில் (nextarg ("/"))
        fxn = divide;
      இல்லையெனில் எனில் (nextarg ("%"))
        fxn = mod;
      இல்லையெனில்
        திரும்பு l;
      மதிப்பு_2 *r = eval5 (evaluate);
      எனில் (evaluate)
        {
          எனில் (!toarith (l) || !toarith (r))
            error (கோவை_INVALID, 0, _("non-integer argument"));
          எனில் (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (கோவை_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

நிலையான மதிப்பு_2 *
eval3 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval3");
#endif
  மதிப்பு_2 *l = eval4 (evaluate);
  போது (true)
    {
      எண்ணிக்கை { plus, minus } fxn;

      எனில் (nextarg ("+"))
        fxn = plus;
      இல்லையெனில் எனில் (nextarg ("-"))
        fxn = minus;
      இல்லையெனில்
        திரும்பு l;
      மதிப்பு_2 *r = eval4 (evaluate);
      எனில் (evaluate)
        {
          எனில் (!toarith (l) || !toarith (r))
            error (கோவை_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_add : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

நிலையான மதிப்பு_2 *
eval2 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval2");
#endif
  மதிப்பு_2 *l = eval3 (evaluate);
  போது (true)
    {
      எண்ணிக்கை
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      எனில் (nextarg ("<"))
        fxn = less_than;
      இல்லையெனில் எனில் (nextarg ("<="))
        fxn = less_equal;
      இல்லையெனில் எனில் (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      இல்லையெனில் எனில் (nextarg ("!="))
        fxn = not_equal;
      இல்லையெனில் எனில் (nextarg (">="))
        fxn = greater_equal;
      இல்லையெனில் எனில் (nextarg (">"))
        fxn = greater_than;
      இல்லையெனில்
        திரும்பு l;
      மதிப்பு_2 *r = eval3 (evaluate);

      bool val = false;
      எனில் (evaluate)
        {
          tostring (l);
          tostring (r);

          முழு cmp;
          எனில் (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          இல்லையெனில்
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              எனில் (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (கோவை_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          தேர்வு (fxn)
            {
            நிலை less_than:     val = (cmp <  0); நிறுத்து;
            நிலை less_equal:    val = (cmp <= 0); நிறுத்து;
            நிலை equal:         val = (cmp == 0); நிறுத்து;
            நிலை not_equal:     val = (cmp != 0); நிறுத்து;
            நிலை greater_equal: val = (cmp >= 0); நிறுத்து;
            நிலை greater_than:  val = (cmp >  0); நிறுத்து;
            இயல்பு: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_மதிப்பு (val);
    }
}

/* Handle &.  */

நிலையான மதிப்பு_2 *
eval1 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval1");
#endif
  மதிப்பு_2 *l = eval2 (evaluate);
  போது (true)
    {
      எனில் (nextarg ("&"))
        {
          மதிப்பு_2 *r = eval2 (evaluate && !null (l));
          எனில் (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_மதிப்பு (0);
            }
          இல்லையெனில்
            freev (r);
        }
      இல்லையெனில்
        திரும்பு l;
    }
}

/* Handle |.  */

நிலையான மதிப்பு_2 *
eval (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval");
#endif
  மதிப்பு_2 *l = eval1 (evaluate);
  போது (true)
    {
      எனில் (nextarg ("|"))
        {
          மதிப்பு_2 *r = eval1 (evaluate && null (l));
          எனில் (null (l))
            {
              freev (l);
              l = r;
              எனில் (null (l))
                {
                  freev (l);
                  l = int_மதிப்பு (0);
                }
            }
          இல்லையெனில்
            freev (r);
        }
      இல்லையெனில்
        திரும்பு l;
    }
}
