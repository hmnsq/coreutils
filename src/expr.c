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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <gmp.h>
#അന്തര്ഭാവ <regex.h>
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "long-options.h"
#അന്തര്ഭാവ "mcel.h"
#അന്തര്ഭാവ "strnumcmp.h"

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
എണ്ണൽ
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    വ്യഞ്ജകം_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    വ്യഞ്ജകം_FAILURE
  };

/* The kinds of value we can have.  */
എണ്ണൽ valtype
{
  integer,
  string
};
തരനിർവചനം എണ്ണൽ valtype TYPE;

/* A value is.... */
ഘടന valinfo
{
  TYPE type;			/* Which kind. */
  സംഘം
  {				/* The value itself. */
    mpz_t i;
    അക്ഷരം *s;
  } u;
};
തരനിർവചനം ഘടന valinfo മൂല്യം_2;

/* The arguments given to the program, minus the program name.  */
സ്ഥിതം അക്ഷരം **args;

സ്ഥിതം മൂല്യം_2 *eval (bool);
സ്ഥിതം bool nomoreargs (ശൂന്യം);
സ്ഥിതം bool null (മൂല്യം_2 *v);
സ്ഥിതം ശൂന്യം printv (മൂല്യം_2 *v);


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
സ്ഥിതം size_t
mbs_logical_cspn (അക്ഷരം സ്ഥിരം *s, അക്ഷരം സ്ഥിരം *accept)
{
  എങ്കിൽ (accept[0] == '\0')
    തിരികെ 0;

  /* General case.  */
  എങ്കിൽ (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      വേണ്ടി (അക്ഷരം സ്ഥിരം *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          എങ്കിൽ (g.len == 1)
            {
              എങ്കിൽ (mbschr (accept, *p))
                തിരികെ idx;
            }
          അല്ലെങ്കിൽ
            വേണ്ടി (അക്ഷരം സ്ഥിരം *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                എങ്കിൽ (mcel_eq (g, h))
                  തിരികെ idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  അല്ലെങ്കിൽ
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      എങ്കിൽ (s[i])
        തിരികെ i + 1;
    }

  /* not found */
  തിരികെ 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
സ്ഥിതം അക്ഷരം *
mbs_logical_substr (അക്ഷരം സ്ഥിരം *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? strlen (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  അക്ഷരം സ്ഥിരം *substart = s;
  idx_t sublen = 0;
  എങ്കിൽ (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  അല്ലെങ്കിൽ എങ്കിൽ (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  അല്ലെങ്കിൽ
    വേണ്ടി (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        എങ്കിൽ (pos <= idx)
          {
            എങ്കിൽ (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
      }

  തിരികെ ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
സ്ഥിതം size_t
mbs_offset_to_chars (അക്ഷരം സ്ഥിരം *s, size_t ofs)
{
  size_t c = 0;
  വേണ്ടി (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  തിരികെ c;
}



ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
Print the മൂല്യം of EXPRESSION to standard output.  A blank വരി below\n\
separates increasing precedence groups.  EXPRESSION may be:\n\
\n\
  ARG1 | ARG2       ARG1 എങ്കിൽ it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 എങ്കിൽ neither argument is null or 0, otherwise 0\n\
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
  + TOKEN                    interpret TOKEN as a string, even എങ്കിൽ it is a\n\
                               keyword like 'match' or an operator like '/'\n\
\n\
  ( EXPRESSION )             മൂല്യം of EXPRESSION\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need to be escaped or quoted വേണ്ടി shells.\n\
Comparisons are arithmetic എങ്കിൽ both ARGs are numbers, അല്ലെങ്കിൽ lexicographical.\n\
Pattern matches തിരികെ the string matched between \\( and \\) or null; എങ്കിൽ\n\
\\( and \\) are not used, they തിരികെ the സംഖ്യ of characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit status is 0 എങ്കിൽ EXPRESSION is neither null nor 0, 1 എങ്കിൽ EXPRESSION is null\n\
or 0, 2 എങ്കിൽ EXPRESSION is syntactically invalid, and 3 എങ്കിൽ an error occurred.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (വ്യഞ്ജകം_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, PROGRAM_NAME, PACKAGE_NAME, VERSION,
                      usage, AUTHORS, (അക്ഷരം സ്ഥിരം *) ശൂന്യ);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  എങ്കിൽ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  എങ്കിൽ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (വ്യഞ്ജകം_INVALID);
    }

  args = argv + 1;

  മൂല്യം_2 *v = eval (true);
  എങ്കിൽ (!nomoreargs ())
    error (വ്യഞ്ജകം_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

സ്ഥിതം മൂല്യം_2 *
int_മൂല്യം (unsigned long പൂർണ്ണസംഖ്യ i)
{
  മൂല്യം_2 *v = xmalloc (വലുപ്പം *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  തിരികെ v;
}

/* Return a VALUE for S.  */

സ്ഥിതം മൂല്യം_2 *
str_മൂല്യം (അക്ഷരം സ്ഥിരം *s)
{
  മൂല്യം_2 *v = xmalloc (വലുപ്പം *v);
  v->type = string;
  v->u.s = xstrdup (s);
  തിരികെ v;
}

/* Free VALUE V, including structure components.  */

സ്ഥിതം ശൂന്യം
freev (മൂല്യം_2 *v)
{
  എങ്കിൽ (v->type == string)
    സ്വതന്ത്രം (v->u.s);
  അല്ലെങ്കിൽ
    mpz_clear (v->u.i);
  സ്വതന്ത്രം (v);
}

/* Print VALUE V.  */

സ്ഥിതം ശൂന്യം
printv (മൂല്യം_2 *v)
{
  തിരഞ്ഞെടുപ്പ് (v->type)
    {
    അവസ്ഥ integer:
      mpz_out_str (stdout, 10, v->u.i);
      putchar ('\n');
      നിർത്തുക;
    അവസ്ഥ string:
      puts (v->u.s);
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_PURE
സ്ഥിതം bool
null (മൂല്യം_2 *v)
{
  തിരഞ്ഞെടുപ്പ് (v->type)
    {
    അവസ്ഥ integer:
      തിരികെ mpz_sgn (v->u.i) == 0;
    അവസ്ഥ string:
      {
        അക്ഷരം സ്ഥിരം *cp = v->u.s;
        എങ്കിൽ (*cp == '\0')
          തിരികെ true;

        cp += (*cp == '-');

        ചെയ്യുക
          {
            എങ്കിൽ (*cp != '0')
              തിരികെ false;
          }
        ആകുമ്പോൾ (*++cp);

        തിരികെ true;
      }
    സ്ഥിരസ്ഥിതി:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_PURE
സ്ഥിതം bool
looks_like_integer (അക്ഷരം സ്ഥിരം *cp)
{
  cp += (*cp == '-');

  ചെയ്യുക
    എങ്കിൽ (! c_isdigit (*cp))
      തിരികെ false;
  ആകുമ്പോൾ (*++cp);

  തിരികെ true;
}

/* Coerce V to a string value (can't fail).  */

സ്ഥിതം ശൂന്യം
tostring (മൂല്യം_2 *v)
{
  തിരഞ്ഞെടുപ്പ് (v->type)
    {
    അവസ്ഥ integer:
      {
        അക്ഷരം *s = mpz_get_str (ശൂന്യ, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      നിർത്തുക;
    അവസ്ഥ string:
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

സ്ഥിതം bool
toarith (മൂല്യം_2 *v)
{
  തിരഞ്ഞെടുപ്പ് (v->type)
    {
    അവസ്ഥ integer:
      തിരികെ true;
    അവസ്ഥ string:
      {
        അക്ഷരം *s = v->u.s;

        എങ്കിൽ (! looks_like_integer (s))
          തിരികെ false;
        എങ്കിൽ (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (വ്യഞ്ജകം_FAILURE, ERANGE, "%s", (s));
        സ്വതന്ത്രം (s);
        v->type = integer;
        തിരികെ true;
      }
    സ്ഥിരസ്ഥിതി:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
സ്ഥിതം size_t
getsize (mpz_t i)
{
  എങ്കിൽ (mpz_sgn (i) < 0)
    തിരികെ SIZE_MAX;
  എങ്കിൽ (mpz_fits_ulong_p (i))
    {
      unsigned long പൂർണ്ണസംഖ്യ ul = mpz_get_ui (i);
      എങ്കിൽ (ul < SIZE_MAX)
        തിരികെ ul;
    }
  തിരികെ SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

സ്ഥിതം bool
nextarg (അക്ഷരം സ്ഥിരം *str)
{
  എങ്കിൽ (*args == ശൂന്യ)
    തിരികെ false;
  അല്ലെങ്കിൽ
    {
      bool r = streq (*args, str);
      args += r;
      തിരികെ r;
    }
}

/* Return true if there no more tokens.  */

സ്ഥിതം bool
nomoreargs (ശൂന്യം)
{
  തിരികെ !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
സ്ഥിതം ശൂന്യം
require_more_args (ശൂന്യം)
{
  എങ്കിൽ (nomoreargs ())
    error (വ്യഞ്ജകം_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#ifdef EVAL_TRACE
/* Print evaluation trace and args remaining.  */

സ്ഥിതം ശൂന്യം
trace (അക്ഷരം സ്ഥിരം *fxn)
{
  printf ("%s:", fxn);
  വേണ്ടി (അക്ഷരം **a = args; *a; a++)
    printf (" %s", *a);
  putchar ('\n');
}
#endif

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

സ്ഥിതം മൂല്യം_2 *
docolon (മൂല്യം_2 *sv, മൂല്യം_2 *pv)
{
  tostring (sv);
  tostring (pv);

  ഘടന re_registers re_regs;
  re_regs.അക്കം_regs = 0;
  re_regs.start = ശൂന്യ;
  re_regs.end = ശൂന്യ;

  ഘടന re_pattern_buffer re_buffer;
  അക്ഷരം fastmap[UCHAR_MAX + 1];
  re_buffer.buffer = ശൂന്യ;
  re_buffer.allocated = 0;
  re_buffer.fastmap = fastmap;
  re_buffer.translate = ശൂന്യ;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_EMPTY_RANGES);
  അക്ഷരം സ്ഥിരം *errmsg = re_compile_pattern (pv->u.s, strlen (pv->u.s),
                                           &re_buffer);
  എങ്കിൽ (errmsg)
    error (വ്യഞ്ജകം_INVALID, 0, "%s", (errmsg));
  re_buffer.newline_anchor = 0;

  മൂല്യം_2 *v;
  regoff_t matchlen = re_match (&re_buffer, sv->u.s, strlen (sv->u.s), 0,
                                &re_regs);
  എങ്കിൽ (0 <= matchlen)
    {
      /* Were \(...\) used? */
      എങ്കിൽ (re_buffer.re_nsub > 0)
        {
          എങ്കിൽ (re_regs.end[1] < 0)
            v = str_മൂല്യം ("");
          അല്ലെങ്കിൽ
            {
              sv->u.s[re_regs.end[1]] = '\0';
              v = str_മൂല്യം (sv->u.s + re_regs.start[1]);
            }
        }
      അല്ലെങ്കിൽ
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_മൂല്യം (i);
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      എങ്കിൽ (re_buffer.re_nsub > 0)
        v = str_മൂല്യം ("");
      അല്ലെങ്കിൽ
        v = int_മൂല്യം (0);
    }
  അല്ലെങ്കിൽ
    error (വ്യഞ്ജകം_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  എങ്കിൽ (0 < re_regs.അക്കം_regs)
    {
      സ്വതന്ത്രം (re_regs.start);
      സ്വതന്ത്രം (re_regs.end);
    }
  re_buffer.fastmap = ശൂന്യ;
  regfree (&re_buffer);
  തിരികെ v;
}

/* Handle bare operands and ( expr ) syntax.  */

സ്ഥിതം മൂല്യം_2 *
eval7 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval7");
#endif
  require_more_args ();

  എങ്കിൽ (nextarg ("("))
    {
      മൂല്യം_2 *v = eval (evaluate);
      എങ്കിൽ (nomoreargs ())
        error (വ്യഞ്ജകം_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      എങ്കിൽ (!nextarg (")"))
        error (വ്യഞ്ജകം_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      തിരികെ v;
    }

  എങ്കിൽ (nextarg (")"))
    error (വ്യഞ്ജകം_INVALID, 0, _("syntax error: unexpected ')'"));

  തിരികെ str_മൂല്യം (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

സ്ഥിതം മൂല്യം_2 *
eval6 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval6");
#endif
  എങ്കിൽ (nextarg ("+"))
    {
      require_more_args ();
      തിരികെ str_മൂല്യം (*args++);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (nextarg ("length"))
    {
      മൂല്യം_2 *r = eval6 (evaluate);
      tostring (r);
      മൂല്യം_2 *v = int_മൂല്യം (mbslen (r->u.s));
      freev (r);
      തിരികെ v;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (nextarg ("match"))
    {
      മൂല്യം_2 *l = eval6 (evaluate);
      മൂല്യം_2 *r = eval6 (evaluate);
      മൂല്യം_2 *v;
      എങ്കിൽ (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      അല്ലെങ്കിൽ
        v = l;
      freev (r);
      തിരികെ v;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (nextarg ("index"))
    {
      മൂല്യം_2 *l = eval6 (evaluate);
      മൂല്യം_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      മൂല്യം_2 *v = int_മൂല്യം (pos);
      freev (l);
      freev (r);
      തിരികെ v;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (nextarg ("substr"))
    {
      മൂല്യം_2 *l = eval6 (evaluate);
      മൂല്യം_2 *i1 = eval6 (evaluate);
      മൂല്യം_2 *i2 = eval6 (evaluate);
      tostring (l);

      മൂല്യം_2 *v;
      എങ്കിൽ (!toarith (i1) || !toarith (i2))
        v = str_മൂല്യം ("");
      അല്ലെങ്കിൽ
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          അക്ഷരം *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_മൂല്യം (s);
          സ്വതന്ത്രം (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      തിരികെ v;
    }
  അല്ലെങ്കിൽ
    തിരികെ eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

സ്ഥിതം മൂല്യം_2 *
eval5 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval5");
#endif
  മൂല്യം_2 *l = eval6 (evaluate);
  ആകുമ്പോൾ (true)
    {
      എങ്കിൽ (nextarg (":"))
        {
          മൂല്യം_2 *r = eval6 (evaluate);
          എങ്കിൽ (evaluate)
            {
              മൂല്യം_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      അല്ലെങ്കിൽ
        തിരികെ l;
    }
}

/* Handle *, /, % operators.  */

സ്ഥിതം മൂല്യം_2 *
eval4 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval4");
#endif
  മൂല്യം_2 *l = eval5 (evaluate);
  ആകുമ്പോൾ (true)
    {
      എണ്ണൽ { multiply, divide, mod } fxn;

      എങ്കിൽ (nextarg ("*"))
        fxn = multiply;
      അല്ലെങ്കിൽ എങ്കിൽ (nextarg ("/"))
        fxn = divide;
      അല്ലെങ്കിൽ എങ്കിൽ (nextarg ("%"))
        fxn = mod;
      അല്ലെങ്കിൽ
        തിരികെ l;
      മൂല്യം_2 *r = eval5 (evaluate);
      എങ്കിൽ (evaluate)
        {
          എങ്കിൽ (!toarith (l) || !toarith (r))
            error (വ്യഞ്ജകം_INVALID, 0, _("non-integer argument"));
          എങ്കിൽ (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (വ്യഞ്ജകം_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

സ്ഥിതം മൂല്യം_2 *
eval3 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval3");
#endif
  മൂല്യം_2 *l = eval4 (evaluate);
  ആകുമ്പോൾ (true)
    {
      എണ്ണൽ { plus, minus } fxn;

      എങ്കിൽ (nextarg ("+"))
        fxn = plus;
      അല്ലെങ്കിൽ എങ്കിൽ (nextarg ("-"))
        fxn = minus;
      അല്ലെങ്കിൽ
        തിരികെ l;
      മൂല്യം_2 *r = eval4 (evaluate);
      എങ്കിൽ (evaluate)
        {
          എങ്കിൽ (!toarith (l) || !toarith (r))
            error (വ്യഞ്ജകം_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_add : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

സ്ഥിതം മൂല്യം_2 *
eval2 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval2");
#endif
  മൂല്യം_2 *l = eval3 (evaluate);
  ആകുമ്പോൾ (true)
    {
      എണ്ണൽ
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      എങ്കിൽ (nextarg ("<"))
        fxn = less_than;
      അല്ലെങ്കിൽ എങ്കിൽ (nextarg ("<="))
        fxn = less_equal;
      അല്ലെങ്കിൽ എങ്കിൽ (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      അല്ലെങ്കിൽ എങ്കിൽ (nextarg ("!="))
        fxn = not_equal;
      അല്ലെങ്കിൽ എങ്കിൽ (nextarg (">="))
        fxn = greater_equal;
      അല്ലെങ്കിൽ എങ്കിൽ (nextarg (">"))
        fxn = greater_than;
      അല്ലെങ്കിൽ
        തിരികെ l;
      മൂല്യം_2 *r = eval3 (evaluate);

      bool val = false;
      എങ്കിൽ (evaluate)
        {
          tostring (l);
          tostring (r);

          പൂർണ്ണസംഖ്യ cmp;
          എങ്കിൽ (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          അല്ലെങ്കിൽ
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              എങ്കിൽ (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (വ്യഞ്ജകം_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          തിരഞ്ഞെടുപ്പ് (fxn)
            {
            അവസ്ഥ less_than:     val = (cmp <  0); നിർത്തുക;
            അവസ്ഥ less_equal:    val = (cmp <= 0); നിർത്തുക;
            അവസ്ഥ equal:         val = (cmp == 0); നിർത്തുക;
            അവസ്ഥ not_equal:     val = (cmp != 0); നിർത്തുക;
            അവസ്ഥ greater_equal: val = (cmp >= 0); നിർത്തുക;
            അവസ്ഥ greater_than:  val = (cmp >  0); നിർത്തുക;
            സ്ഥിരസ്ഥിതി: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_മൂല്യം (val);
    }
}

/* Handle &.  */

സ്ഥിതം മൂല്യം_2 *
eval1 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval1");
#endif
  മൂല്യം_2 *l = eval2 (evaluate);
  ആകുമ്പോൾ (true)
    {
      എങ്കിൽ (nextarg ("&"))
        {
          മൂല്യം_2 *r = eval2 (evaluate && !null (l));
          എങ്കിൽ (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_മൂല്യം (0);
            }
          അല്ലെങ്കിൽ
            freev (r);
        }
      അല്ലെങ്കിൽ
        തിരികെ l;
    }
}

/* Handle |.  */

സ്ഥിതം മൂല്യം_2 *
eval (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval");
#endif
  മൂല്യം_2 *l = eval1 (evaluate);
  ആകുമ്പോൾ (true)
    {
      എങ്കിൽ (nextarg ("|"))
        {
          മൂല്യം_2 *r = eval1 (evaluate && null (l));
          എങ്കിൽ (null (l))
            {
              freev (l);
              l = r;
              എങ്കിൽ (null (l))
                {
                  freev (l);
                  l = int_മൂല്യം (0);
                }
            }
          അല്ലെങ്കിൽ
            freev (r);
        }
      അല്ലെങ്കിൽ
        തിരികെ l;
    }
}
