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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"

#అన్తర్భావ <gmp.h>
#అన్తర్భావ <regex.h>
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "long-options.h"
#అన్తర్భావ "mcel.h"
#అన్తర్భావ "strnumcmp.h"

/* Various parts of this code assume size_t fits into unsigned long
   int, the widest unsigned type that GMP supports.  */
static_assert (SIZE_MAX <= ULONG_MAX);

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "expr"

#నిర్వచించు AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("James Youngman"), \
  proper_name ("Paul Eggert")

/* Exit statuses.  */
గణన
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    సమాసం_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    సమాసం_FAILURE
  };

/* The kinds of value we can have.  */
గణన valtype
{
  integer,
  string
};
రకనిర్వచనం గణన valtype TYPE;

/* A value is.... */
నిర్మాణం valinfo
{
  TYPE type;			/* Which kind. */
  సంఘం
  {				/* The value itself. */
    mpz_t i;
    అక్షరం *s;
  } u;
};
రకనిర్వచనం నిర్మాణం valinfo విలువ_2;

/* The arguments given to the program, minus the program name.  */
స్థిర అక్షరం **వాదనలు;

స్థిర విలువ_2 *eval (bool);
స్థిర bool nomoreargs (శూన్య);
స్థిర bool null (విలువ_2 *v);
స్థిర శూన్య printv (విలువ_2 *v);


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
స్థిర size_t
mbs_logical_cspn (అక్షరం స్థిరరాశి *s, అక్షరం స్థిరరాశి *accept)
{
  ఒకవేళ (accept[0] == '\0')
    తిరిగి 0;

  /* General case.  */
  ఒకవేళ (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      కొరకు (అక్షరం స్థిరరాశి *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          ఒకవేళ (g.len == 1)
            {
              ఒకవేళ (mbschr (accept, *p))
                తిరిగి idx;
            }
          లేకపోతే
            కొరకు (అక్షరం స్థిరరాశి *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                ఒకవేళ (mcel_eq (g, h))
                  తిరిగి idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  లేకపోతే
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      ఒకవేళ (s[i])
        తిరిగి i + 1;
    }

  /* not found */
  తిరిగి 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
స్థిర అక్షరం *
mbs_logical_substr (అక్షరం స్థిరరాశి *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? అక్షరశ్రేణి_పొడవు (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  అక్షరం స్థిరరాశి *substart = s;
  idx_t sublen = 0;
  ఒకవేళ (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  లేకపోతే ఒకవేళ (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  లేకపోతే
    కొరకు (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_బైట్లు = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        ఒకవేళ (pos <= idx)
          {
            ఒకవేళ (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_బైట్లు;
          }

        s += char_బైట్లు;
      }

  తిరిగి ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
స్థిర size_t
mbs_స్థానభ్రంశం_గమ్యం_chars (అక్షరం స్థిరరాశి *s, size_t ofs)
{
  size_t c = 0;
  కొరకు (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  తిరిగి c;
}



శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s వ్యక్తీకరణ\n\
  or:  %s OPTION\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      అక్షరం_రాయి ('\n');
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Print the విలువ యొక్క వ్యక్తీకరణ గమ్యం standard output.  A blank పంక్తి క్రింద\n\
separates increasing precedence groups.  వ్యక్తీకరణ may be:\n\
\n\
  ARG1 | ARG2       ARG1 ఒకవేళ it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 ఒకవేళ neither వాదం is null or 0, otherwise 0\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 < ARG2       ARG1 is less than ARG2\n\
  ARG1 <= ARG2      ARG1 is less than or equal గమ్యం ARG2\n\
  ARG1 = ARG2       ARG1 is equal గమ్యం ARG2\n\
  ARG1 != ARG2      ARG1 is unequal గమ్యం ARG2\n\
  ARG1 >= ARG2      ARG1 is greater than or equal గమ్యం ARG2\n\
  ARG1 > ARG2       ARG1 is greater than ARG2\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 + ARG2       arithmetic కూడిక యొక్క ARG1 and ARG2\n\
  ARG1 - ARG2       arithmetic difference యొక్క ARG1 and ARG2\n\
"), stdout);
      /* Tell xgettext that the "% A" below is not a printf-style
         format string:  xgettext:no-c-format */
      fputs (_("\
\n\
  ARG1 * ARG2       arithmetic లబ్ధం యొక్క ARG1 and ARG2\n\
  ARG1 / ARG2       arithmetic quotient యొక్క ARG1 divided by ARG2\n\
  ARG1 % ARG2       arithmetic remainder యొక్క ARG1 divided by ARG2\n\
"), stdout);
      fputs (_("\
\n\
  STRING : REGEXP   anchored నమూనా_2 సరిపోలిక యొక్క REGEXP లోపల STRING\n\
\n\
  సరిపోలిక STRING REGEXP        same as STRING : REGEXP\n\
  substr STRING POS నిడివి_2   substring యొక్క STRING, POS counted from 1\n\
  index STRING CHARS         index లోపల STRING where any CHARS is దొరికింది, or 0\n\
  నిడివి STRING              నిడివి యొక్క STRING\n\
"), stdout);
      fputs (_("\
  + TOKEN                    interpret TOKEN as a string, even ఒకవేళ it is a\n\
                               keyword like 'match' or an క్రియాకారకం like '/'\n\
\n\
  ( వ్యక్తీకరణ )             విలువ యొక్క వ్యక్తీకరణ\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need గమ్యం be escaped or quoted కొరకు shells.\n\
Comparisons are arithmetic ఒకవేళ both వాదనలు_2 are numbers, లేకపోతే lexicographical.\n\
నమూనా_3 matches తిరిగి the string matched between \\( and \\) or null; ఒకవేళ\n\
\\( and \\) are not వాడిన, they తిరిగి the సంఖ్య యొక్క characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit స్థితి_2 is 0 ఒకవేళ వ్యక్తీకరణ is neither null nor 0, 1 ఒకవేళ వ్యక్తీకరణ is null\n\
or 0, 2 ఒకవేళ వ్యక్తీకరణ is syntactically invalid, and 3 ఒకవేళ an error occurred.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}


పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (సమాసం_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, కార్యక్రమం_NAME, PACKAGE_NAME, సంస్కరణ_2,
                      usage, AUTHORS, (అక్షరం స్థిరరాశి *) ఖాళీ);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  ఒకవేళ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ఒకవేళ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (సమాసం_INVALID);
    }

  వాదనలు = argv + 1;

  విలువ_2 *v = eval (true);
  ఒకవేళ (!nomoreargs ())
    error (సమాసం_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *వాదనలు));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

స్థిర విలువ_2 *
int_విలువ (గుర్తులేని దీర్ఘ పూర్ణాంకం i)
{
  విలువ_2 *v = xmalloc (పరిమాణం *v);
  v->type = integer;
  mpz_init_సమితి_ui (v->u.i, i);
  తిరిగి v;
}

/* Return a VALUE for S.  */

స్థిర విలువ_2 *
str_విలువ (అక్షరం స్థిరరాశి *s)
{
  విలువ_2 *v = xmalloc (పరిమాణం *v);
  v->type = string;
  v->u.s = xstrdup (s);
  తిరిగి v;
}

/* Free VALUE V, including structure components.  */

స్థిర శూన్య
freev (విలువ_2 *v)
{
  ఒకవేళ (v->type == string)
    విడుదల (v->u.s);
  లేకపోతే
    mpz_clear (v->u.i);
  విడుదల (v);
}

/* Print VALUE V.  */

స్థిర శూన్య
printv (విలువ_2 *v)
{
  ఎంపిక (v->type)
    {
    స్థితి integer:
      mpz_బహిర్గతం_str (stdout, 10, v->u.i);
      అక్షరం_రాయి ('\n');
      ఆపు;
    స్థితి string:
      puts (v->u.s);
      ఆపు;
    అప్రమేయ:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_శుద్ధ
స్థిర bool
null (విలువ_2 *v)
{
  ఎంపిక (v->type)
    {
    స్థితి integer:
      తిరిగి mpz_sgn (v->u.i) == 0;
    స్థితి string:
      {
        అక్షరం స్థిరరాశి *cp = v->u.s;
        ఒకవేళ (*cp == '\0')
          తిరిగి true;

        cp += (*cp == '-');

        చేయి
          {
            ఒకవేళ (*cp != '0')
              తిరిగి false;
          }
        అయితే (*++cp);

        తిరిగి true;
      }
    అప్రమేయ:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_శుద్ధ
స్థిర bool
looks_like_integer (అక్షరం స్థిరరాశి *cp)
{
  cp += (*cp == '-');

  చేయి
    ఒకవేళ (! c_isdigit (*cp))
      తిరిగి false;
  అయితే (*++cp);

  తిరిగి true;
}

/* Coerce V to a string value (can't fail).  */

స్థిర శూన్య
tostring (విలువ_2 *v)
{
  ఎంపిక (v->type)
    {
    స్థితి integer:
      {
        అక్షరం *s = mpz_get_str (ఖాళీ, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      ఆపు;
    స్థితి string:
      ఆపు;
    అప్రమేయ:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

స్థిర bool
toarith (విలువ_2 *v)
{
  ఎంపిక (v->type)
    {
    స్థితి integer:
      తిరిగి true;
    స్థితి string:
      {
        అక్షరం *s = v->u.s;

        ఒకవేళ (! looks_like_integer (s))
          తిరిగి false;
        ఒకవేళ (mpz_init_సమితి_str (v->u.i, s, 10) != 0)
          error (సమాసం_FAILURE, ERANGE, "%s", (s));
        విడుదల (s);
        v->type = integer;
        తిరిగి true;
      }
    అప్రమేయ:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
స్థిర size_t
getsize (mpz_t i)
{
  ఒకవేళ (mpz_sgn (i) < 0)
    తిరిగి SIZE_MAX;
  ఒకవేళ (mpz_సరిపోతుంది_ulong_p (i))
    {
      గుర్తులేని దీర్ఘ పూర్ణాంకం ul = mpz_get_ui (i);
      ఒకవేళ (ul < SIZE_MAX)
        తిరిగి ul;
    }
  తిరిగి SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

స్థిర bool
nextarg (అక్షరం స్థిరరాశి *str)
{
  ఒకవేళ (*వాదనలు == ఖాళీ)
    తిరిగి false;
  లేకపోతే
    {
      bool r = streq (*వాదనలు, str);
      వాదనలు += r;
      తిరిగి r;
    }
}

/* Return true if there no more tokens.  */

స్థిర bool
nomoreargs (శూన్య)
{
  తిరిగి !*వాదనలు;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
స్థిర శూన్య
require_మరిన్ని_వాదనలు (శూన్య)
{
  ఒకవేళ (nomoreargs ())
    error (సమాసం_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(వాదనలు - 1)));
}


#నిర్వచితమైతే EVAL_TRACE
/* Print evaluation trace and args remaining.  */

స్థిర శూన్య
trace (అక్షరం స్థిరరాశి *fxn)
{
  రాయి ("%s:", fxn);
  కొరకు (అక్షరం **a = వాదనలు; *a; a++)
    రాయి (" %s", *a);
  అక్షరం_రాయి ('\n');
}
#అయితే_ముగింపు

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

స్థిర విలువ_2 *
docolon (విలువ_2 *sv, విలువ_2 *pv)
{
  tostring (sv);
  tostring (pv);

  నిర్మాణం re_registers re_regs;
  re_regs.అంకె_regs = 0;
  re_regs.ప్రారంభం = ఖాళీ;
  re_regs.చివర = ఖాళీ;

  నిర్మాణం re_నమూనా_బఫరు re_బఫరు;
  అక్షరం fastmap[UCHAR_MAX + 1];
  re_బఫరు.బఫరు = ఖాళీ;
  re_బఫరు.allocated = 0;
  re_బఫరు.fastmap = fastmap;
  re_బఫరు.translate = ఖాళీ;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_ఖాళీలు_RANGES);
  అక్షరం స్థిరరాశి *errmsg = re_compile_నమూనా (pv->u.s, అక్షరశ్రేణి_పొడవు (pv->u.s),
                                           &re_బఫరు);
  ఒకవేళ (errmsg)
    error (సమాసం_INVALID, 0, "%s", (errmsg));
  re_బఫరు.newline_anchor = 0;

  విలువ_2 *v;
  regoff_t matchlen = re_సరిపోలిక (&re_బఫరు, sv->u.s, అక్షరశ్రేణి_పొడవు (sv->u.s), 0,
                                &re_regs);
  ఒకవేళ (0 <= matchlen)
    {
      /* Were \(...\) used? */
      ఒకవేళ (re_బఫరు.re_nsub > 0)
        {
          ఒకవేళ (re_regs.చివర[1] < 0)
            v = str_విలువ ("");
          లేకపోతే
            {
              sv->u.s[re_regs.చివర[1]] = '\0';
              v = str_విలువ (sv->u.s + re_regs.ప్రారంభం[1]);
            }
        }
      లేకపోతే
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_స్థానభ్రంశం_గమ్యం_chars (sv->u.s, matchlen));
          v = int_విలువ (i);
        }
    }
  లేకపోతే ఒకవేళ (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      ఒకవేళ (re_బఫరు.re_nsub > 0)
        v = str_విలువ ("");
      లేకపోతే
        v = int_విలువ (0);
    }
  లేకపోతే
    error (సమాసం_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  ఒకవేళ (0 < re_regs.అంకె_regs)
    {
      విడుదల (re_regs.ప్రారంభం);
      విడుదల (re_regs.చివర);
    }
  re_బఫరు.fastmap = ఖాళీ;
  regfree (&re_బఫరు);
  తిరిగి v;
}

/* Handle bare operands and ( expr ) syntax.  */

స్థిర విలువ_2 *
eval7 (bool evaluate)
{
#నిర్వచితమైతే EVAL_TRACE
  trace ("eval7");
#అయితే_ముగింపు
  require_మరిన్ని_వాదనలు ();

  ఒకవేళ (nextarg ("("))
    {
      విలువ_2 *v = eval (evaluate);
      ఒకవేళ (nomoreargs ())
        error (సమాసం_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(వాదనలు - 1)));
      ఒకవేళ (!nextarg (")"))
        error (సమాసం_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *వాదనలు));
      తిరిగి v;
    }

  ఒకవేళ (nextarg (")"))
    error (సమాసం_INVALID, 0, _("syntax error: unexpected ')'"));

  తిరిగి str_విలువ (*వాదనలు++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

స్థిర విలువ_2 *
eval6 (bool evaluate)
{
#నిర్వచితమైతే EVAL_TRACE
  trace ("eval6");
#అయితే_ముగింపు
  ఒకవేళ (nextarg ("+"))
    {
      require_మరిన్ని_వాదనలు ();
      తిరిగి str_విలువ (*వాదనలు++);
    }
  లేకపోతే ఒకవేళ (nextarg ("length"))
    {
      విలువ_2 *r = eval6 (evaluate);
      tostring (r);
      విలువ_2 *v = int_విలువ (mbslen (r->u.s));
      freev (r);
      తిరిగి v;
    }
  లేకపోతే ఒకవేళ (nextarg ("match"))
    {
      విలువ_2 *l = eval6 (evaluate);
      విలువ_2 *r = eval6 (evaluate);
      విలువ_2 *v;
      ఒకవేళ (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      లేకపోతే
        v = l;
      freev (r);
      తిరిగి v;
    }
  లేకపోతే ఒకవేళ (nextarg ("index"))
    {
      విలువ_2 *l = eval6 (evaluate);
      విలువ_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      విలువ_2 *v = int_విలువ (pos);
      freev (l);
      freev (r);
      తిరిగి v;
    }
  లేకపోతే ఒకవేళ (nextarg ("substr"))
    {
      విలువ_2 *l = eval6 (evaluate);
      విలువ_2 *i1 = eval6 (evaluate);
      విలువ_2 *i2 = eval6 (evaluate);
      tostring (l);

      విలువ_2 *v;
      ఒకవేళ (!toarith (i1) || !toarith (i2))
        v = str_విలువ ("");
      లేకపోతే
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          అక్షరం *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_విలువ (s);
          విడుదల (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      తిరిగి v;
    }
  లేకపోతే
    తిరిగి eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

స్థిర విలువ_2 *
eval5 (bool evaluate)
{
#నిర్వచితమైతే EVAL_TRACE
  trace ("eval5");
#అయితే_ముగింపు
  విలువ_2 *l = eval6 (evaluate);
  అయితే (true)
    {
      ఒకవేళ (nextarg (":"))
        {
          విలువ_2 *r = eval6 (evaluate);
          ఒకవేళ (evaluate)
            {
              విలువ_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      లేకపోతే
        తిరిగి l;
    }
}

/* Handle *, /, % operators.  */

స్థిర విలువ_2 *
eval4 (bool evaluate)
{
#నిర్వచితమైతే EVAL_TRACE
  trace ("eval4");
#అయితే_ముగింపు
  విలువ_2 *l = eval5 (evaluate);
  అయితే (true)
    {
      గణన { multiply, divide, mod } fxn;

      ఒకవేళ (nextarg ("*"))
        fxn = multiply;
      లేకపోతే ఒకవేళ (nextarg ("/"))
        fxn = divide;
      లేకపోతే ఒకవేళ (nextarg ("%"))
        fxn = mod;
      లేకపోతే
        తిరిగి l;
      విలువ_2 *r = eval5 (evaluate);
      ఒకవేళ (evaluate)
        {
          ఒకవేళ (!toarith (l) || !toarith (r))
            error (సమాసం_INVALID, 0, _("non-integer argument"));
          ఒకవేళ (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (సమాసం_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

స్థిర విలువ_2 *
eval3 (bool evaluate)
{
#నిర్వచితమైతే EVAL_TRACE
  trace ("eval3");
#అయితే_ముగింపు
  విలువ_2 *l = eval4 (evaluate);
  అయితే (true)
    {
      గణన { plus, minus } fxn;

      ఒకవేళ (nextarg ("+"))
        fxn = plus;
      లేకపోతే ఒకవేళ (nextarg ("-"))
        fxn = minus;
      లేకపోతే
        తిరిగి l;
      విలువ_2 *r = eval4 (evaluate);
      ఒకవేళ (evaluate)
        {
          ఒకవేళ (!toarith (l) || !toarith (r))
            error (సమాసం_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_కూడు : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

స్థిర విలువ_2 *
eval2 (bool evaluate)
{
#నిర్వచితమైతే EVAL_TRACE
  trace ("eval2");
#అయితే_ముగింపు
  విలువ_2 *l = eval3 (evaluate);
  అయితే (true)
    {
      గణన
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      ఒకవేళ (nextarg ("<"))
        fxn = less_than;
      లేకపోతే ఒకవేళ (nextarg ("<="))
        fxn = less_equal;
      లేకపోతే ఒకవేళ (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      లేకపోతే ఒకవేళ (nextarg ("!="))
        fxn = not_equal;
      లేకపోతే ఒకవేళ (nextarg (">="))
        fxn = greater_equal;
      లేకపోతే ఒకవేళ (nextarg (">"))
        fxn = greater_than;
      లేకపోతే
        తిరిగి l;
      విలువ_2 *r = eval3 (evaluate);

      bool val = false;
      ఒకవేళ (evaluate)
        {
          tostring (l);
          tostring (r);

          పూర్ణాంకం cmp;
          ఒకవేళ (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          లేకపోతే
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              ఒకవేళ (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (సమాసం_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          ఎంపిక (fxn)
            {
            స్థితి less_than:     val = (cmp <  0); ఆపు;
            స్థితి less_equal:    val = (cmp <= 0); ఆపు;
            స్థితి equal:         val = (cmp == 0); ఆపు;
            స్థితి not_equal:     val = (cmp != 0); ఆపు;
            స్థితి greater_equal: val = (cmp >= 0); ఆపు;
            స్థితి greater_than:  val = (cmp >  0); ఆపు;
            అప్రమేయ: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_విలువ (val);
    }
}

/* Handle &.  */

స్థిర విలువ_2 *
eval1 (bool evaluate)
{
#నిర్వచితమైతే EVAL_TRACE
  trace ("eval1");
#అయితే_ముగింపు
  విలువ_2 *l = eval2 (evaluate);
  అయితే (true)
    {
      ఒకవేళ (nextarg ("&"))
        {
          విలువ_2 *r = eval2 (evaluate && !null (l));
          ఒకవేళ (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_విలువ (0);
            }
          లేకపోతే
            freev (r);
        }
      లేకపోతే
        తిరిగి l;
    }
}

/* Handle |.  */

స్థిర విలువ_2 *
eval (bool evaluate)
{
#నిర్వచితమైతే EVAL_TRACE
  trace ("eval");
#అయితే_ముగింపు
  విలువ_2 *l = eval1 (evaluate);
  అయితే (true)
    {
      ఒకవేళ (nextarg ("|"))
        {
          విలువ_2 *r = eval1 (evaluate && null (l));
          ఒకవేళ (null (l))
            {
              freev (l);
              l = r;
              ఒకవేళ (null (l))
                {
                  freev (l);
                  l = int_విలువ (0);
                }
            }
          లేకపోతే
            freev (r);
        }
      లేకపోతే
        తిరిగి l;
    }
}
